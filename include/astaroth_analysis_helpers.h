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
#include <vector>
std::vector<KernelAnalysisInfo>
get_kernel_analysis_info(const AcMeshInfo config);

KernelAnalysisInfo
get_kernel_analysis_info(const AcMeshInfo config, const AcKernel kernel);

KernelAnalysisInfo
get_kernel_analysis_info(const AcMeshInfo config, const AcKernel kernel, const acKernelInputParams input_params);

bool
kernel_has_profile_stencil_ops(const KernelAnalysisInfo info);

bool
kernel_has_stencil_ops(const KernelAnalysisInfo info);

bool
kernel_updates_vtxbuf(const KernelAnalysisInfo info);

bool
kernel_writes_to_input(const Field field, const KernelAnalysisInfo info);

bool
kernel_writes_to_output(const Field field,const KernelAnalysisInfo info);

bool
is_raytracing_kernel(const KernelAnalysisInfo info);

AcBool3
raytracing_step_direction(const KernelAnalysisInfo info);

AcBoundary
get_stencil_boundaries(const Stencil stencil);

AcBoundary
get_ray_boundaries(const AcRay ray);

std::array<int,NUM_FIELDS>
get_fields_kernel_depends_on_boundaries(const AcMeshInfo config, const std::array<int,NUM_FIELDS>& fields_already_depend_on_boundaries, const KernelAnalysisInfo info);

AcBoundary
get_kernel_depends_on_boundaries(const AcMeshInfo config, const std::array<int,NUM_FIELDS>& fields_already_depend_on_boundaries, const KernelAnalysisInfo info);

std::vector<AcBoundary>
get_kernel_depends_on_boundaries(const AcMeshInfo config, const std::array<int,NUM_FIELDS>& fields_already_depend_on_boundaries, const KernelAnalysisInfo* info);

bool
kernel_reduces_scalar(const KernelAnalysisInfo info);

bool
kernel_reduces_something(const KernelAnalysisInfo info);

bool
kernel_writes_profile(const AcProfileType prof_type, const KernelAnalysisInfo info);

bool
kernel_reduces_only_profiles(const AcProfileType prof_type, const KernelAnalysisInfo info);

bool
kernel_does_only_profile_reductions(const AcProfileType prof_type, const KernelAnalysisInfo info);

bool
kernel_calls_ray(const AcRay ray, const KernelAnalysisInfo info);

bool
kernel_uses_rays(const KernelAnalysisInfo info);

bool
kernel_only_writes_profile(const AcProfileType prof_type, const KernelAnalysisInfo info);

std::vector<std::array<AcBoundary,NUM_PROFILES+1>>
compute_kernel_call_computes_profile_across_halos(const AcMeshInfo config, const std::vector<AcKernel>& calls, const std::vector<KernelAnalysisInfo>& info);

std::vector<std::array<AcBoundary,NUM_PROFILES+1>>
compute_kernel_call_computes_profile_across_halos_static(const AcMeshInfo config, const std::vector<AcKernel>& calls, const std::vector<KernelAnalysisInfo>& info);

std::tuple<int3,int3>
get_stencil_dims(const Stencil stencil);

int
get_stencil_halo_type(const AcMeshInfo info, const Stencil stencil);

bool
kernel_calls_stencil(const Stencil stencil, const KernelAnalysisInfo info);

std::tuple<int3,int3>
get_kernel_radius(const AcMeshInfo config, const KernelAnalysisInfo info);

AcBoundary
stencil_accesses_boundaries(const AcMeshInfo info, const Stencil stencil);

