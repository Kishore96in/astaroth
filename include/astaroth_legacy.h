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
/*
 * =============================================================================
 * Legacy interface
 * =============================================================================
 */
/** Allocates all memory and initializes the devices visible to the caller. Should be
 * called before any other function in this interface. */
FUNC_DEFINE(AcResult, acInit,(const AcMeshInfo mesh_info));

/** Frees all GPU allocations and resets all devices in the node. Should be
 * called at exit. */
FUNC_DEFINE(AcResult, acQuit,(void));

/** Synchronizes a specific stream. All streams are synchronized if STREAM_ALL is passed as a
 * parameter*/
FUNC_DEFINE(AcResult, acSynchronizeStream,(const Stream stream));

/** */
FUNC_DEFINE(AcResult, acSynchronizeMesh,(void));

/** Loads a constant to the memories of the devices visible to the caller */
FUNC_DEFINE(AcResult, acLoadDeviceConstant,(const AcRealParam param, const AcReal value));

/** Loads an AcMesh to the devices visible to the caller */
FUNC_DEFINE(AcResult, acLoad,(const AcMesh host_mesh));

/** Sets the whole mesh to some value */
FUNC_DEFINE(AcResult, acSetVertexBuffer,(const VertexBufferHandle handle, const AcReal value));

/** Stores the AcMesh distributed among the devices visible to the caller back to the host*/
FUNC_DEFINE(AcResult, acStore,(AcMesh* host_mesh));

/** Performs Runge-Kutta 3 integration. Note: Boundary conditions are not applied after the final
 * substep and the user is responsible for calling acBoundcondStep before reading the data. */
FUNC_DEFINE(AcResult, acIntegrate,(const AcReal dt));

/** Performs Runge-Kutta 3 integration. Note: Boundary conditions are not applied after the final
 * substep and the user is responsible for calling acBoundcondStep before reading the data.
 * Has customizable boundary conditions. */
FUNC_DEFINE(AcResult, acIntegrateGBC,(const AcMeshInfo config, const AcReal dt));

/** Applies periodic boundary conditions for the Mesh distributed among the devices visible to
 * the caller*/
FUNC_DEFINE(AcResult, acBoundcondStep,(void));

/** Applies general outer boundary conditions for the Mesh distributed among the devices visible to
 * the caller*/
FUNC_DEFINE(AcResult, acBoundcondStepGBC,(const AcMeshInfo config));

/** Does a scalar reduction with the data stored in some vertex buffer */
FUNC_DEFINE(AcReal, acReduceScal,(const AcReduction reduction, const VertexBufferHandle vtxbuf_handle));

/** Does a vector reduction with vertex buffers where the vector components are (a, b, c) */
FUNC_DEFINE(AcReal, acReduceVec,(const AcReduction reduction, const VertexBufferHandle a,
                   const VertexBufferHandle b, const VertexBufferHandle c));

/** Does a reduction for an operation which requires a vector and a scalar with vertex buffers
 *  * where the vector components are (a, b, c) and scalr is (d) */
FUNC_DEFINE(AcReal, acReduceVecScal,(const AcReduction reduction, const VertexBufferHandle a,
                       const VertexBufferHandle b, const VertexBufferHandle c,
                       const VertexBufferHandle d));

/** Stores a subset of the mesh stored across the devices visible to the caller back to host memory.
 */
FUNC_DEFINE(AcResult, acStoreWithOffset,(const int3 dst, const size_t num_vertices, AcMesh* host_mesh));

/** Will potentially be deprecated in later versions. Added only to fix backwards compatibility with
 * PC for now.*/
FUNC_DEFINE(AcResult, acIntegrateStep,(const int isubstep, const AcReal dt));
FUNC_DEFINE(AcResult, acIntegrateStepWithOffset,(const int isubstep, const AcReal dt, const Volume start,
                                   const Volume end));
FUNC_DEFINE(AcResult, acSynchronize,(void));
FUNC_DEFINE(AcResult, acLoadWithOffset,(const AcMesh host_mesh, const int3 src, const int num_vertices));
