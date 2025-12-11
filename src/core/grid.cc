/*
 * Copyright (C) 2025 Aalto University
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#if AC_MPI_ENABLED
/**
 * Quick overview of the MPI implementation:
 *
 * The halo is partitioned into segments, each segment is assigned a HaloExchangeTask.
 * A HaloExchangeTask sends local data as a halo to a neighbor
 * and receives halo data from a (possibly different) neighbor.
 *
 * struct PackedData is used for packing and unpacking. Holds the actual data in
 *                   the halo partition (wrapped by HaloMessage)
 * struct Grid contains information about the local GPU device, decomposition,
 *             the total mesh dimensions, tasks, and MPI requests

 * Basic steps:
 *   1) Distribute the mesh among ranks
 *   2) Integrate & communicate
 *     - start inner integration and at the same time, pack halo data and send it to neighbors
 *     - as halo data is received and unpacked, integrate segments whose dependencies are ready
 *     - sync and start again
 *   3) Gather the mesh to rank 0 for postprocessing
 *
 * This file contains the grid interface, with algorithms and high level functionality
 * The nitty gritty of the MPI communication is defined in task.cc
 */

#include "astaroth.h"
#include "task.h"

#include <cstring> //memcpy
#include <mpi.h>
#include <utility> //std::swap

#include "decomposition.h" //getPid3D, morton3D
#include "errchk.h"
#include "math_utils.h"
/* Internal interface to grid (a global variable)  */
typedef struct Grid {
    Device device;
    AcMesh submesh;
    uint3_64 decomposition;
    bool initialized;
    int3 nn;
    std::vector<HaloExchangeTask> halo_exchange_tasks;
    std::vector<HaloExchangeTask> face_exchange_tasks;
    std::vector<HaloExchangeTask> edge_exchange_tasks;
    std::vector<HaloExchangeTask> corner_exchange_tasks;
    std::vector<ComputeTask> compute_tasks;
    ComputeTask* inner_integration_task;
    MPI_Request* recv_reqs;
    MPI_Request* send_reqs;
    MPI_Request* curr_recv_reqs;
    MPI_Request* back_recv_reqs;
    MPI_Request* curr_send_reqs;
    MPI_Request* back_send_reqs;

} Grid;

static Grid grid = {};

static void
gridSwapRequestBuffers()
{
    // Assumption SWAP_CHAIN_LENGTH = 2 in these swaps
    std::swap(grid.curr_recv_reqs, grid.back_recv_reqs);
    std::swap(grid.curr_send_reqs, grid.back_send_reqs);
}

AcResult
acGridSynchronizeStream(const Stream stream)
{
    ERRCHK(grid.initialized);
    acDeviceSynchronizeStream(grid.device, stream);
    MPI_Barrier(MPI_COMM_WORLD);
    return AC_SUCCESS;
}

AcResult
acGridRandomize(void)
{
    ERRCHK(grid.initialized);

    AcMesh host;
    acHostMeshCreate(grid.submesh.info, &host);
    acHostMeshRandomize(&host);
    acDeviceLoadMesh(grid.device, STREAM_DEFAULT, host);
    acHostMeshDestroy(&host);

    return AC_SUCCESS;
}

AcResult
acGridInit(const AcMeshInfo info)
{
    ERRCHK(!grid.initialized);

    // Check that MPI is initialized
    int nprocs, pid;
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Decompose
    AcMeshInfo submesh_info = info;
    const uint3_64 decomp   = decompose(nprocs);
    const int3 pid3d        = getPid3D(pid, decomp);

    MPI_Barrier(MPI_COMM_WORLD);
    printf("Processor %s. Process %d of %d: (%d, %d, %d)\n", processor_name, pid, nprocs, pid3d.x,
           pid3d.y, pid3d.z);
    printf("Decomposition: %lu, %lu, %lu\n", decomp.x, decomp.y, decomp.z);
    printf("Mesh size: %d, %d, %d\n", info.int_params[AC_nx], info.int_params[AC_ny],
           info.int_params[AC_nz]);
    fflush(stdout);
    MPI_Barrier(MPI_COMM_WORLD);
    ERRCHK_ALWAYS(info.int_params[AC_nx] % decomp.x == 0);
    ERRCHK_ALWAYS(info.int_params[AC_ny] % decomp.y == 0);
    ERRCHK_ALWAYS(info.int_params[AC_nz] % decomp.z == 0);

    // Check that mixed precision is correctly configured, AcRealPacked == AC_MPI_TYPE
    // CAN BE REMOVED IF MIXED PRECISION IS SUPPORTED AS A PREPROCESSOR FLAG
    int mpi_type_size;
    MPI_Type_size(AC_MPI_TYPE, &mpi_type_size);
    ERRCHK_ALWAYS(sizeof(AcRealPacked) == mpi_type_size);

    const int submesh_nx                       = info.int_params[AC_nx] / decomp.x;
    const int submesh_ny                       = info.int_params[AC_ny] / decomp.y;
    const int submesh_nz                       = info.int_params[AC_nz] / decomp.z;
    submesh_info.int_params[AC_nx]             = submesh_nx;
    submesh_info.int_params[AC_ny]             = submesh_ny;
    submesh_info.int_params[AC_nz]             = submesh_nz;
    submesh_info.int3_params[AC_global_grid_n] = (int3){
        info.int_params[AC_nx],
        info.int_params[AC_ny],
        info.int_params[AC_nz],
    };
    submesh_info.int3_params[AC_multigpu_offset] = pid3d *
                                                   (int3){submesh_nx, submesh_ny, submesh_nz};
    acHostUpdateBuiltinParams(&submesh_info);

    // GPU alloc
    int devices_per_node = -1;
    cudaGetDeviceCount(&devices_per_node);

    Device device;
    acDeviceCreate(pid % devices_per_node, submesh_info, &device);

    // CPU alloc
    AcMesh submesh;
    acHostMeshCreate(submesh_info, &submesh);

    // Setup the global grid structure
    grid.device        = device;
    grid.submesh       = submesh;
    grid.decomposition = decomp;

    // Configure
    const int3 nn = (int3){
        device->local_config.int_params[AC_nx],
        device->local_config.int_params[AC_ny],
        device->local_config.int_params[AC_nz],
    };

    grid.nn = nn;

    // Create compute tasks
    grid.compute_tasks.clear();
    grid.compute_tasks.reserve(NUM_SEGMENTS);

    grid.inner_integration_task = new ComputeTask(device, Region::id_to_tag((int3){0, 0, 0}), nn,
                                                  STREAM_26);

    for (int tag = 0; tag < NUM_SEGMENTS; tag++) {
        grid.compute_tasks.emplace_back(device, tag, nn, (Stream)(tag + STREAM_DEFAULT));
    }

    // Create halo exchange tasks
    grid.halo_exchange_tasks.clear();
    grid.halo_exchange_tasks.reserve(NUM_SEGMENTS);

    grid.recv_reqs = new MPI_Request[NUM_SEGMENTS * SWAP_CHAIN_LENGTH];
    grid.send_reqs = new MPI_Request[NUM_SEGMENTS * SWAP_CHAIN_LENGTH];

    // This below assumes SWAP_CHAIN_LENGTH == 2
    grid.curr_recv_reqs = grid.recv_reqs;
    grid.back_recv_reqs = &grid.recv_reqs[NUM_SEGMENTS];
    grid.curr_send_reqs = grid.send_reqs;
    grid.back_send_reqs = &grid.send_reqs[NUM_SEGMENTS];

    for (int tag = 0; tag < NUM_SEGMENTS; tag++) {
        grid.halo_exchange_tasks.emplace_back(device, tag, nn, decomp, grid.recv_reqs,
                                              grid.send_reqs);
    }
    // Dependencies
    for (auto& halo_task : grid.halo_exchange_tasks) {
        if (halo_task.active) {
            int3 h_id = halo_task.output_region->id;
            for (auto& comp_task : grid.compute_tasks) {
                int3 c_id = comp_task.output_region->id;
                if (((h_id.x == 0) || (h_id.x == c_id.x)) &&
                    ((h_id.y == 0) || (h_id.y == c_id.y)) &&
                    ((h_id.z == 0) || (h_id.z == c_id.z))) {

                    // Comp tasks depend on halo tasks in the same iteration, offset = 0
                    // Halo tasks depend on comp tasks in the prev iteration, offset = 1
                    halo_task.registerDependent(&comp_task, 0);
                    comp_task.registerDependent(&halo_task, 1);
                }
            }
        }
    }

    for (auto& comp_task_1 : grid.compute_tasks) {
        int3 c1_id = comp_task_1.output_region->id;
        for (auto& comp_task_2 : grid.compute_tasks) {
            int3 c2_id = comp_task_2.output_region->id;
            if (((c1_id.x == 0) || (c2_id.x == 0) || (c1_id.x == c2_id.x)) &&
                ((c1_id.y == 0) || (c2_id.y == 0) || (c1_id.y == c2_id.y)) &&
                ((c1_id.z == 0) || (c2_id.z == 0) || (c1_id.z == c2_id.z))) {

                // Comp tasks depend on comp tasks from the prev iteration, offset = 1
                comp_task_1.registerDependent(&comp_task_2, 1);
            }
        }
        comp_task_1.registerDependent(grid.inner_integration_task, 1);
        grid.inner_integration_task->registerDependent(&comp_task_1, 1);
    }

    grid.initialized = true;

    acGridSynchronizeStream(STREAM_ALL);
    return AC_SUCCESS;
}

AcResult
acGridQuit(void)
{
    ERRCHK(grid.initialized);
    acGridSynchronizeStream(STREAM_ALL);

    grid.halo_exchange_tasks.clear();

    for (int i = 0; i < NUM_SEGMENTS * SWAP_CHAIN_LENGTH; i++) {
        MPI_Request* req = &(grid.recv_reqs[i]);
        if (*req != MPI_REQUEST_NULL) {
            MPI_Cancel(req);
            MPI_Request_free(req);
        }
    }

    for (int i = 0; i < NUM_SEGMENTS * SWAP_CHAIN_LENGTH; i++) {
        MPI_Request* req = &(grid.send_reqs[i]);
        if (*req != MPI_REQUEST_NULL) {
            MPI_Wait(req, MPI_STATUS_IGNORE);
            // MPI_Request_free(req);
        }
    }

    delete[] grid.recv_reqs;
    delete[] grid.send_reqs;

    grid.compute_tasks.clear();
    delete grid.inner_integration_task;

    grid.initialized   = false;
    grid.decomposition = (uint3_64){0, 0, 0};
    acHostMeshDestroy(&grid.submesh);
    acDeviceDestroy(grid.device);

    return AC_SUCCESS;
}

AcResult
acGridLoadScalarUniform(const Stream stream, const AcRealParam param, const AcReal value)
{
    ERRCHK(grid.initialized);
    acGridSynchronizeStream(stream);

    const int root_proc = 0;
    AcReal buffer       = value;
    MPI_Bcast(&buffer, 1, AC_MPI_TYPE, root_proc, MPI_COMM_WORLD);

    acDeviceLoadScalarUniform(grid.device, stream, param, buffer);
    return AC_SUCCESS;
}

AcResult
acGridLoadVectorUniform(const Stream stream, const AcReal3Param param, const AcReal3 value)
{
    ERRCHK(grid.initialized);
    acGridSynchronizeStream(stream);

    const int root_proc = 0;
    AcReal3 buffer      = value;
    MPI_Bcast(&buffer, 3, AC_MPI_TYPE, root_proc, MPI_COMM_WORLD);

    acDeviceLoadVectorUniform(grid.device, stream, param, buffer);
    return AC_SUCCESS;
}

// TODO: do with packed data
AcResult
acGridLoadMesh(const Stream stream, const AcMesh host_mesh)
{
    ERRCHK(grid.initialized);
    acGridSynchronizeStream(stream);

#if AC_VERBOSE
    printf("Distributing mesh...\n");
    fflush(stdout);
#endif

    int pid, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    ERRCHK_ALWAYS(&grid.submesh);

    // Submesh nn
    const int3 nn = (int3){
        grid.submesh.info.int_params[AC_nx],
        grid.submesh.info.int_params[AC_ny],
        grid.submesh.info.int_params[AC_nz],
    };

    // Send to self
    if (pid == 0) {
        for (int vtxbuf = 0; vtxbuf < NUM_VTXBUF_HANDLES; ++vtxbuf) {
            // For pencils
            for (int k = NGHOST; k < NGHOST + nn.z; ++k) {
                for (int j = NGHOST; j < NGHOST + nn.y; ++j) {
                    const int i       = NGHOST;
                    const int count   = nn.x;
                    const int src_idx = acVertexBufferIdx(i, j, k, host_mesh.info);
                    const int dst_idx = acVertexBufferIdx(i, j, k, grid.submesh.info);
                    memcpy(&grid.submesh.vertex_buffer[vtxbuf][dst_idx], //
                           &host_mesh.vertex_buffer[vtxbuf][src_idx],    //
                           count * sizeof(host_mesh.vertex_buffer[i][0]));
                }
            }
        }
    }

    for (int vtxbuf = 0; vtxbuf < NUM_VTXBUF_HANDLES; ++vtxbuf) {
        // For pencils
        for (int k = NGHOST; k < NGHOST + nn.z; ++k) {
            for (int j = NGHOST; j < NGHOST + nn.y; ++j) {
                const int i     = NGHOST;
                const int count = nn.x;

                if (pid != 0) {
                    const int dst_idx = acVertexBufferIdx(i, j, k, grid.submesh.info);
                    // Recv
                    MPI_Status status;
                    MPI_Recv(&grid.submesh.vertex_buffer[vtxbuf][dst_idx], count, AC_MPI_TYPE, 0, 0,
                             MPI_COMM_WORLD, &status);
                }
                else {
                    for (int tgt_pid = 1; tgt_pid < nprocs; ++tgt_pid) {
                        const int3 tgt_pid3d = getPid3D(tgt_pid, grid.decomposition);
                        const int src_idx    = acVertexBufferIdx(i + tgt_pid3d.x * nn.x, //
                                                              j + tgt_pid3d.y * nn.y, //
                                                              k + tgt_pid3d.z * nn.z, //
                                                              host_mesh.info);

                        // Send
                        MPI_Send(&host_mesh.vertex_buffer[vtxbuf][src_idx], count, AC_MPI_TYPE,
                                 tgt_pid, 0, MPI_COMM_WORLD);
                    }
                }
            }
        }
    }

    acDeviceLoadMesh(grid.device, stream, grid.submesh);
    return AC_SUCCESS;
}

// TODO: do with packed data
AcResult
acGridStoreMesh(const Stream stream, AcMesh* host_mesh)
{
    ERRCHK(grid.initialized);
    acGridSynchronizeStream(stream);

    acDeviceStoreMesh(grid.device, stream, &grid.submesh);
    acGridSynchronizeStream(stream);

#if AC_VERBOSE
    printf("Gathering mesh...\n");
    fflush(stdout);
#endif

    int pid, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if (pid == 0)
        ERRCHK_ALWAYS(host_mesh);

    // Submesh nn
    const int3 nn = (int3){
        grid.submesh.info.int_params[AC_nx],
        grid.submesh.info.int_params[AC_ny],
        grid.submesh.info.int_params[AC_nz],
    };

    // Submesh mm
    const int3 mm = (int3){
        grid.submesh.info.int_params[AC_mx],
        grid.submesh.info.int_params[AC_my],
        grid.submesh.info.int_params[AC_mz],
    };

    // Send to self
    if (pid == 0) {
        for (int vtxbuf = 0; vtxbuf < NUM_VTXBUF_HANDLES; ++vtxbuf) {
            // For pencils
            for (int k = 0; k < mm.z; ++k) {
                for (int j = 0; j < mm.y; ++j) {
                    const int i       = 0;
                    const int count   = mm.x;
                    const int src_idx = acVertexBufferIdx(i, j, k, grid.submesh.info);
                    const int dst_idx = acVertexBufferIdx(i, j, k, host_mesh->info);
                    memcpy(&host_mesh->vertex_buffer[vtxbuf][dst_idx],   //
                           &grid.submesh.vertex_buffer[vtxbuf][src_idx], //
                           count * sizeof(grid.submesh.vertex_buffer[i][0]));
                }
            }
        }
    }

    for (int vtxbuf = 0; vtxbuf < NUM_VTXBUF_HANDLES; ++vtxbuf) {
        // For pencils
        for (int k = 0; k < mm.z; ++k) {
            for (int j = 0; j < mm.y; ++j) {
                const int i     = 0;
                const int count = mm.x;
                if (pid != 0) {
                    // Send
                    const int src_idx = acVertexBufferIdx(i, j, k, grid.submesh.info);
                    MPI_Send(&grid.submesh.vertex_buffer[vtxbuf][src_idx], count, AC_MPI_TYPE, 0, 0,
                             MPI_COMM_WORLD);
                }
                else {
                    for (int tgt_pid = 1; tgt_pid < nprocs; ++tgt_pid) {
                        const int3 tgt_pid3d = getPid3D(tgt_pid, grid.decomposition);
                        const int dst_idx    = acVertexBufferIdx(i + tgt_pid3d.x * nn.x, //
                                                              j + tgt_pid3d.y * nn.y, //
                                                              k + tgt_pid3d.z * nn.z, //
                                                              host_mesh->info);

                        // Recv
                        MPI_Status status;
                        MPI_Recv(&host_mesh->vertex_buffer[vtxbuf][dst_idx], count, AC_MPI_TYPE,
                                 tgt_pid, 0, MPI_COMM_WORLD, &status);
                    }
                }
            }
        }
    }
    return AC_SUCCESS;
#include <iostream>

AcResult
acGridIntegrate(const Stream stream, const AcReal dt)
{
    ERRCHK(grid.initialized);
    acGridSynchronizeStream(stream);
    const Device device = grid.device;

    acGridLoadScalarUniform(stream, AC_dt, dt);
    acDeviceSynchronizeStream(device, stream);
    cudaSetDevice(device->id);

    size_t num_iterations = 3;

    for (auto& halo_task : grid.halo_exchange_tasks) {
        if (halo_task.active) {
            halo_task.setIterationParams(0, num_iterations);
        }
    }

    for (auto& compute_task : grid.compute_tasks) {
        compute_task.setIterationParams(0, num_iterations);
    }
    grid.inner_integration_task->setIterationParams(0, num_iterations);

    bool ready;
    do {
        ready = true;

        grid.inner_integration_task->update();
        ready &= grid.inner_integration_task->isFinished();

        for (auto& halo_task : grid.halo_exchange_tasks) {
            if (halo_task.active) {
                halo_task.update();
                ready &= halo_task.isFinished();
            }
        }

        for (auto& compute_task : grid.compute_tasks) {
            compute_task.update();
            ready &= compute_task.isFinished();
        }
    } while (!ready);

    if (num_iterations % 2 != 0) {
        gridSwapRequestBuffers();
        acDeviceSwapBuffers(device);
    }
    return AC_SUCCESS;
}

AcResult
acGridPeriodicBoundconds(const Stream stream)
{
    ERRCHK(grid.initialized);
    acGridSynchronizeStream(stream);

    // Active halo exchange tasks
    for (auto& halo_task : grid.halo_exchange_tasks) {
        if (halo_task.active) {
            halo_task.syncVBA();
            halo_task.pack();
            halo_task.send();
        }
    }

    MPI_Waitall(NUM_SEGMENTS, grid.curr_recv_reqs, MPI_STATUSES_IGNORE);
    for (auto& halo_task : grid.halo_exchange_tasks) {
        if (halo_task.active) {
            halo_task.unpack();
            halo_task.sync();
            halo_task.receive();
        }
    }

    // Inactive halo exchange tasks (i.e. possibly corners)
    for (auto& halo_task : grid.halo_exchange_tasks) {
        if (!halo_task.active) {
            halo_task.syncVBA();
            halo_task.pack();
            halo_task.exchange();
        }
    }
    for (auto& halo_task : grid.halo_exchange_tasks) {
        if (!halo_task.active) {
            halo_task.wait_recv();
            halo_task.unpack();
            halo_task.sync();
        }
    }

    MPI_Waitall(NUM_SEGMENTS * SWAP_CHAIN_LENGTH, grid.send_reqs, MPI_STATUSES_IGNORE);
    gridSwapRequestBuffers();
    return AC_SUCCESS;
}

static AcResult
distributedScalarReduction(const AcReal local_result, const ReductionType rtype, AcReal* result)
{

    MPI_Op op;
    if (rtype == RTYPE_MAX) {
        op = MPI_MAX;
    }
    else if (rtype == RTYPE_MIN) {
        op = MPI_MIN;
    }
    else if (rtype == RTYPE_RMS || rtype == RTYPE_RMS_EXP || rtype == RTYPE_SUM) {
        op = MPI_SUM;
    }
    else {
        ERROR("Unrecognised rtype");
    }

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    AcReal mpi_res;
    MPI_Reduce(&local_result, &mpi_res, 1, AC_MPI_TYPE, op, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        if (rtype == RTYPE_RMS || rtype == RTYPE_RMS_EXP) {
            const AcReal inv_n = AcReal(1.) /
                                 (grid.nn.x * grid.decomposition.x * grid.nn.y *
                                  grid.decomposition.y * grid.nn.z * grid.decomposition.z);
            mpi_res = sqrt(inv_n * mpi_res);
        }
        *result = mpi_res;
    }
    return AC_SUCCESS;
}

AcResult
acGridReduceScal(const Stream stream, const ReductionType rtype,
                 const VertexBufferHandle vtxbuf_handle, AcReal* result)
{
    ERRCHK(grid.initialized);
    const Device device = grid.device;
    acGridSynchronizeStream(STREAM_ALL);

    AcReal local_result;
    acDeviceReduceScal(device, stream, rtype, vtxbuf_handle, &local_result);

    return distributedScalarReduction(local_result, rtype, result);
}

AcResult
acGridReduceVec(const Stream stream, const ReductionType rtype, const VertexBufferHandle vtxbuf0,
                const VertexBufferHandle vtxbuf1, const VertexBufferHandle vtxbuf2, AcReal* result)
{
    ERRCHK(grid.initialized);
    const Device device = grid.device;
    acGridSynchronizeStream(STREAM_ALL);

    AcReal local_result;
    acDeviceReduceVec(device, stream, rtype, vtxbuf0, vtxbuf1, vtxbuf2, &local_result);

    return distributedScalarReduction(local_result, rtype, result);
}


#endif // AC_MPI_ENABLED
