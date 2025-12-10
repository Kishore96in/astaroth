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

grid_position() {
//MR: generalize, using x,y,z?
//TP: implicitly assumes equidistant grid
    return ((globalVertexIdx - AC_nmin)*AC_ds) + AC_first_gridpoint
}

grid_position(int3 local_point) {
//TP: implicitly assumes equidistant grid
    global_point = (local_point-AC_nmin) + AC_multigpu_offset
    return global_point*AC_ds + AC_first_gridpoint
}

grid_center() {
//MR: generalize, using x,y,z?
//TP: implicitly assumes equidistant grid
    return (0.5*AC_len) + AC_first_gridpoint;
}
