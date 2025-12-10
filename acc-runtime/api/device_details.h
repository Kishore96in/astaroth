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
#define rocprim__warpSize() rocprim::device_warp_size()
#define rocprim__warpId()   rocprim::warp_id()
#define rocprim__warp_shuffle_down rocprim::warp_shuffle_down
#define rocprim__warp_shuffle rocprim::warp_shuffle

#if AC_CPU_BUILD

#else

#if AC_USE_HIP
#include <hip/hip_runtime.h> // Needed in files that include kernels
#include <rocprim/rocprim.hpp>
#include <hip/hip_cooperative_groups.h>
#else
#include <cooperative_groups.h>
#endif

#endif
