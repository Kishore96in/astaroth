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
bool
kernel_has_stencil_call(const int curr_kernel)
{
	//Skip one since it is the value stencil
	for(int i = 1; i < NUM_STENCILS; ++i)
		for(int j = 0; j < NUM_ALL_FIELDS; ++j)
			if(stencils_accessed[curr_kernel][j][i]) return true;
	return false;
}
bool
kernel_reduces_profile(const int curr_kernel)
{
	for(int j = 0; j < NUM_PROFILES; ++j)
		if(reduced_profiles[curr_kernel][j]) return true;
	return false;
}
bool
kernel_reads_profile(const int curr_kernel)
{
	for(int j = 0; j < NUM_PROFILES; ++j)
		if(read_profiles[curr_kernel][j]) return true;
	return false;
}
bool
kernel_is_pure_reduce_kernel(const int curr_kernel)
{
	if(!has_mem_access_info) return false;
	//TP: for the moment only pure reduce kernels, that reduce a profile, have block loops
	if(kernel_has_stencil_call(curr_kernel)) return false;
	if(kernel_reduces_profile(curr_kernel)) return true;
	return false;
}
bool
kernel_has_block_loops(const int curr_kernel)
{
	return has_mem_access_info && kernel_reduces_profile(curr_kernel);
}
