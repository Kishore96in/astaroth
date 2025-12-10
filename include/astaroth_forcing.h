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

#include "acc_runtime.h"

typedef struct {
    AcReal magnitude;
    AcReal3 k_force;
    AcReal3 ff_hel_re;
    AcReal3 ff_hel_im;
    AcReal phase;
    AcReal kaver;
} ForcingParams;

#ifdef __cplusplus
extern "C" {
#endif

/** Generates PC-inspired forcing params (vaisala_interactionoflarge_2021:
 * https://doi.org/10.3847/1538-4357/abceca) */
// ForcingParams generateForcingParams(const AcReal relhel, const AcReal magnitude, const AcReal
// kmin,
//                                     const AcReal kmax);

/** Generates exact PC forcing params based on PC user manual,
 * brandenburg_crosshelically_2019 10.1002/asna.201913602, and used in pekkila_gpuaccelerated_2025
 */
ForcingParams generateHelicalForcingParams(const AcReal relhel, const AcReal magnitude,
                                           const AcReal kmin, const AcReal kmax);

int loadForcingParamsToMeshInfo(const ForcingParams forcing_params, AcMeshInfo* info);

void printForcingParams(const ForcingParams forcing_params);

#ifdef __cplusplus
} // extern "C"
#endif
