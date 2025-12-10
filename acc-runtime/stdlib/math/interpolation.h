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
#ifndef AC_MATH_INTERPOLATION_H
#define AC_MATH_INTERPOLATION_H

Stencil interpolate_middle_left
{
	[0][0][-1] = 0.5,
	[0][0][0] = 0.5
}

Stencil interpolate_middle_right
{
	[0][0][1] = 0.5,
	[0][0][0] = 0.5
}
Stencil interpolate_middle_down
{
	[0][-1][0] = 0.5,
	[0][0][0] = 0.5
}

Stencil interpolate_middle_up
{
	[0][1][0] = 0.5,
	[0][0][0] = 0.5
}

Stencil interpolate_middle_back
{
	[-1][0][0] = 0.5,
	[0][0][0] = 0.5
}
Stencil interpolate_middle_front
{
	[1][0][0] = 0.5,
	[0][0][0] = 0.5
}

#endif
