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
#ifndef AC_GRID_TRANSFER_FUNCTIONS_H
#define AC_GRID_TRANSFER_FUNCTIONS_H
/*
 * Meant to be launched on the coarse grid
 */
restrict_full_weighting(Field fine_residual, Field coarse_residual)
{
	i = 2*vertexIdx.x + 1 - NGHOST 
	j = 2*vertexIdx.y + 1 - NGHOST 
	k = 2*vertexIdx.z + 1 - NGHOST 
	res = 0.0
	int di
	int dj
	int dk
	for di in -1:2
	{
		for dj in -1:2
		{
			for dk in -1:2
			{
				//Facet class 3 has the weight 1
				weight = 1
				facet_class = abs(di) + abs(dj) + abs(dk)
				if(facet_class == 0)
				{
					weight = 8
				}
				else if(facet_class == 1)
				{
					weight = 4
				}
				else if(facet_class == 2)
				{
					weight = 2
				}
				res += weight*fine_residual[i+di][j+dj][k+dk]
			}
		}
	}
	res /= 64.0
	write(coarse_residual,res);
}

/*
 * Meant to be launched on the fine grid
 */
trilinear_prolongation(Field coarse_residual)
{

	const int3 shifted_index = vertexIdx - NGHOST + 1
	const bool I_even = (shifted_index.x % 2 == 0)
	const bool J_even = (shifted_index.y % 2 == 0)
	const bool K_even = (shifted_index.z % 2 == 0)

	const int3 coarse_vertexIdx = (shifted_index/ 2) + NGHOST - 1

	if(I_even && J_even && K_even)
	{
		return coarse_residual[coarse_vertexIdx.x][coarse_vertexIdx.y][coarse_vertexIdx.z]
	}
	if(I_even && J_even && !K_even)
	{
		return 0.5*(
								coarse_residual[coarse_vertexIdx.x][coarse_vertexIdx.y][coarse_vertexIdx.z]
								+coarse_residual[coarse_vertexIdx.x][coarse_vertexIdx.y][coarse_vertexIdx.z+1]
				)
	}
	if(I_even && !J_even && K_even)
	{
		return 0.5*(
								coarse_residual[coarse_vertexIdx.x][coarse_vertexIdx.y][coarse_vertexIdx.z]
								+coarse_residual[coarse_vertexIdx.x][coarse_vertexIdx.y+1][coarse_vertexIdx.z]
				)
	}
	if(!I_even && J_even && K_even)
	{
		return 0.5*(
								coarse_residual[coarse_vertexIdx.x][coarse_vertexIdx.y][coarse_vertexIdx.z]
								+coarse_residual[coarse_vertexIdx.x+1][coarse_vertexIdx.y][coarse_vertexIdx.z]
				)
	}
	if(I_even && !J_even && !K_even)
	{
		return 0.25*(
								coarse_residual[coarse_vertexIdx.x][coarse_vertexIdx.y][coarse_vertexIdx.z]
								+coarse_residual[coarse_vertexIdx.x][coarse_vertexIdx.y+1][coarse_vertexIdx.z]
								+coarse_residual[coarse_vertexIdx.x][coarse_vertexIdx.y][coarse_vertexIdx.z+1]
								+coarse_residual[coarse_vertexIdx.x][coarse_vertexIdx.y+1][coarse_vertexIdx.z+1]
				)
	}
	if(!I_even && J_even && !K_even)
	{
		return 0.25*(
								coarse_residual[coarse_vertexIdx.x][coarse_vertexIdx.y][coarse_vertexIdx.z]
								+coarse_residual[coarse_vertexIdx.x+1][coarse_vertexIdx.y][coarse_vertexIdx.z]
								+coarse_residual[coarse_vertexIdx.x][coarse_vertexIdx.y][coarse_vertexIdx.z+1]
								+coarse_residual[coarse_vertexIdx.x+1][coarse_vertexIdx.y][coarse_vertexIdx.z+1]
				)
	}
	if(!I_even && !J_even && K_even)
	{
		return 0.25*(
								coarse_residual[coarse_vertexIdx.x][coarse_vertexIdx.y][coarse_vertexIdx.z]
								+coarse_residual[coarse_vertexIdx.x+1][coarse_vertexIdx.y][coarse_vertexIdx.z]
								+coarse_residual[coarse_vertexIdx.x][coarse_vertexIdx.y+1][coarse_vertexIdx.z]
								+coarse_residual[coarse_vertexIdx.x+1][coarse_vertexIdx.y+1][coarse_vertexIdx.z]
				)
	}
	if(!I_even && !J_even && !K_even)
	{
		return 0.125*(
								coarse_residual[coarse_vertexIdx.x][coarse_vertexIdx.y][coarse_vertexIdx.z]
								+coarse_residual[coarse_vertexIdx.x+1][coarse_vertexIdx.y][coarse_vertexIdx.z]
								+coarse_residual[coarse_vertexIdx.x][coarse_vertexIdx.y+1][coarse_vertexIdx.z]
								+coarse_residual[coarse_vertexIdx.x][coarse_vertexIdx.y][coarse_vertexIdx.z+1]
								+coarse_residual[coarse_vertexIdx.x+1][coarse_vertexIdx.y+1][coarse_vertexIdx.z]
								+coarse_residual[coarse_vertexIdx.x+1][coarse_vertexIdx.y][coarse_vertexIdx.z+1]
								+coarse_residual[coarse_vertexIdx.x][coarse_vertexIdx.y+1][coarse_vertexIdx.z+1]
								+coarse_residual[coarse_vertexIdx.x+1][coarse_vertexIdx.y+1][coarse_vertexIdx.z+1]
				)
	}
	return 0.0
}
#endif
