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
typedef struct device_s* Device;
  typedef struct AcBuffer{
      AcReal* data;
      size_t count;
      bool on_device;
      AcShape shape;
#ifdef __cplusplus
      const AcReal& operator[](const int index) {return data[index];}
#endif
  } AcBuffer;

#ifdef __cplusplus
extern "C" 
{
#endif

void
ac_unset_floating_point_exceptions();

void
ac_restore_floating_point_exceptions();

const char* acLibraryVersion(const char* library, const int counter, const AcCommunicator* comm);

size_t
acShapeCount(const AcShape shape);

int
acGetNumOfWarps(const dim3 bpg, const dim3 tpb);

int
acGetCurrentDevice();

bool
acSupportsCooperativeLaunches();

AcReal
get_reduce_state_flush_var_real(const AcReduceOp state);

int
get_reduce_state_flush_var_int(const AcReduceOp state);

float
get_reduce_state_flush_var_float(const AcReduceOp state);

size_t acGetSizeFromDim(const int dim, const Volume dims);

Volume acGetVolumeFromShape(const AcShape shape);
int acMemUsage();

size_t
acGetAmountOfDeviceMemoryFree();

#ifdef __cplusplus
cudaDeviceProp
get_device_prop();
#endif

size_t
acDeviceResize(void** dst,const size_t old_bytes,const size_t new_bytes);

Volume
get_bpg(Volume dims, const Volume tpb);

AcBuffer acBufferCreate(const AcShape shape, const bool on_device);
AcBuffer acBufferCreateTransposed(const AcBuffer src, const AcMeshOrder order);
AcBuffer acTransposeBuffer(const AcBuffer src, const AcMeshOrder order, const cudaStream_t stream);

AcShape  acGetTransposeBufferShape(const AcMeshOrder order, const Volume dims);
AcShape  acGetReductionShape(const AcProfileType type, const AcMeshDims dims);

AcBuffer
acBufferRemoveHalos(const AcBuffer buffer_in, const int3 halo_sizes, const cudaStream_t stream);

void acBufferDestroy(AcBuffer* buffer);

AcResult acBufferMigrate(const AcBuffer in, AcBuffer* out);
AcBuffer acBufferCopy(const AcBuffer in, const bool on_device);

#ifdef __cplusplus
}

int3
ceil(AcReal3 a);

size3_t
ceil_div(const size3_t& a, const int3& b);

size3_t
ceil_div(const size3_t& a, const size3_t& b);

int3
ceil_div(const int3& a, const int3& b);

size_t
ceil_div(const size_t& a, const size_t& b); 

void
acDeviceMalloc(void** dst, const size_t bytes);
void
acDeviceMalloc(AcReal** dst, const size_t bytes);

void
acDeviceFree(void** dst, const int bytes);
void
acDeviceFree(AcReal** dst, const int bytes);
void
acDeviceFree(AcComplex** dst, const int bytes);

#endif
AcMeshOrder acGetMeshOrderForProfile(const AcProfileType type);

// Returns the number of elements contained within shape
size_t acShapeSize(const AcShape shape);

