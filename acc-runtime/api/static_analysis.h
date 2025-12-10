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
#ifdef __cplusplus
extern "C"
{
#endif
bool
is_raytracing_kernel(const AcKernel kernel);
int
num_fields_ray_accessed_read_and_written(const AcKernel kernel);
AcBool3
raytracing_step_direction(const AcKernel kernel);
AcBool3
raytracing_directions(const AcKernel kernel);
int
raytracing_number_of_directions(const AcKernel kernel);
bool
is_coop_raytracing_kernel(const AcKernel kernel);
bool
profile_is_reduced(const Profile profile);
#ifdef __cplusplus
}
#endif
