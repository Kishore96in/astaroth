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
#ifndef UNUSED

#define UNUSED __attribute__((unused))


#endif
#ifndef HOST_DEVICE
#if (defined(__CUDACC__) || defined(__HIP_PLATFORM_AMD__)) && !defined(AC_CPU_CODE)
#define HOST_DEVICE __host__ __device__ UNUSED
#define HOST_DEVICE_INLINE __host__ __device__ __forceinline__ constexpr UNUSED
#define HOST_INLINE __host__  inline UNUSED
#else
#define HOST_DEVICE UNUSED
#define HOST_DEVICE_INLINE inline constexpr UNUSED
#define HOST_INLINE  inline UNUSED
#endif // __CUDACC__ || __HIPCC__
#endif //HOST_DEVICE
