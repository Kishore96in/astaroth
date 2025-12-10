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
#include <stdint.h> //uint64_t
#include "math_utils.h" //uint64_t, uint3_64
#include "datatypes.h"
#include "errchk.h"
#include "astaroth_base.h"
extern "C"
{
	#include "astaroth_grid.h"
}

typedef struct {
    size_t ndims;
    size_t* global_dims; // [ndims]
    size_t* local_dims;  // [ndims]

    size_t nlayers;
    size_t* decomposition;        // [nlayers*ndims]
    size_t* global_decomposition; // [ndims]
} AcDecompositionInfo;

void acDecompositionInfoPrint(const AcDecompositionInfo info);

AcDecompositionInfo acDecompositionInfoCreate(const size_t ndims, const size_t* global_dims,
                                              const size_t nlayers,
                                              const size_t* partitions_per_layer);

void acDecompositionInfoDestroy(AcDecompositionInfo* info);

int acGetHierarchicalPid(const int3 pid_input, const AcDecompositionInfo info);

int3 acGetHierarchicalPid3D(const int i, const AcDecompositionInfo info);

// void acVerifyDecomposition(const AcDecompositionInfo info);

// --------------------
// Backwards compatibility
// --------------------
void compat_acDecompositionInit(const size_t ndims, const size_t* global_dims, const size_t nlayers,
                                const size_t* partitions_per_layer);
void compat_acDecompositionQuit(void);

uint3_64 decompose(const uint64_t target, const AcDecomposeStrategy decompose_strategy);

int getPid(const int3 pid_raw, const uint3_64 decomp,const AcProcMappingStrategy proc_mapping_strategy);

int3 getPid3D(const uint64_t pid, const uint3_64 decomp,const AcProcMappingStrategy proc_mapping_strategy);
void acVerifyDecomposition(const uint3_64 decomp, const AcProcMappingStrategy proc_mapping_strategy);
void acInitDecomposition(const bool two_dimensional_setup);
