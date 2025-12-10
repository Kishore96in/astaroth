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
#pragma once
#include "device_headers.h"
#ifdef __cplusplus
extern "C" 
{
#endif
cudaError_t
acDriverGetVersion(int* dst);
cudaError_t
acRuntimeGetVersion(int* dst);
cudaError_t
acStreamSynchronize(cudaStream_t stream);
cudaError_t
acDeviceSynchronize();
cudaError_t
acSetDevice(const int id);
cudaError_t
acGetDeviceCount(int* dst);
//TP: cudaMemcpyKind,cudaDeviceProp,cudaDeviceAttr and cudaSharedMemConfig are only defined for C++ sources
#ifdef __cplusplus
cudaError_t
acDeviceSetSharedMemConfig(const cudaSharedMemConfig config);
cudaError_t
acMemcpy(void* dst, const void* src, const size_t bytes, cudaMemcpyKind kind);
cudaError_t
acMemcpyAsync(void* dst, const void* src, const size_t bytes, cudaMemcpyKind kind, const cudaStream_t stream);
cudaError_t 
acGetDeviceProperties(cudaDeviceProp* prop, int  device);
cudaError_t
acDeviceGetAttribute(int* dst, cudaDeviceAttr attr, int device);
#endif
cudaError_t
acStreamDestroy(cudaStream_t stream);
cudaError_t
acMemcpyPeerAsync(AcReal* dst, int dst_id, const AcReal* src, int src_id, const size_t bytes, const cudaStream_t stream);
cudaError_t
acMemGetInfo(size_t* free_mem, size_t* total_mem);
cudaError_t
acStreamQuery(cudaStream_t stream);
const char*
acGetErrorString(cudaError_t err);
const char*
acGetErrorName(cudaError_t err);
cudaError_t
acDeviceGetStreamPriorityRange(int* leastPriority, int* greatestPriority);
cudaError_t
acStreamCreateWithPriority(cudaStream_t* stream, unsigned int flags, int priority);
cudaError_t
acMalloc(void** dst, const size_t bytes);
cudaError_t
acFree(void* dst);
cudaError_t
acFreeHost(void* dst);
cudaError_t
acMallocHost(void** dst, const size_t bytes);
cudaError_t
acGetDevice(int* dst);
cudaError_t
acGetLastError();
cudaError_t
acEventCreate(cudaEvent_t* event);
cudaError_t
acEventRecord(cudaEvent_t event);
cudaError_t
acEventSynchronize(cudaEvent_t event);
cudaError_t
acEventElapsedTime(float* time, cudaEvent_t start_event, cudaEvent_t end_event);
cudaError_t
acEventDestroy(cudaEvent_t event);
cudaError_t
acOccupancyMaxActiveBlocksPerMultiprocessor(int* numBlocks, const void* func, int blockSize, size_t smemSize);
cudaError_t
acLaunchCooperativeKernel(void* func,dim3 bpg,dim3 tpb,void** args,size_t smem,cudaStream_t stream);
cudaError_t
acDeviceGetPCIBusId(char* pciBusId, int len, int device);
cudaError_t
acPeekAtLastError();

#if PROFILING_ENABLED
cudaError_t
acProfilerStart();
cudaError_t
acProfilerStop();
#endif

#ifdef __cplusplus
}
cudaError_t
acMemcpy(AcReal* dst, const AcReal* src, const size_t bytes, cudaMemcpyKind kind);
cudaError_t
acMemcpyAsync(AcReal* dst, const AcReal* src, const size_t bytes, cudaMemcpyKind kind, const cudaStream_t stream);
cudaError_t
acStreamCreateWithPriority(cudaStream_t* dst, int option, int priority);
#endif
