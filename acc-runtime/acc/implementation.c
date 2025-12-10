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
#include "implementation.h"

typedef struct {
  size_t x, y, z;
} Volume;

static Volume
as_volume(const int x, const int y, const int z)
{
  return (Volume){as_size_t(x), as_size_t(y), as_size_t(z)};
}

static size_t
get_smem(const Volume tpb, const size_t stencil_order,
         const size_t bytes_per_elem)
{
  return 0;
}

static bool
is_valid_configuration(const Volume tpb, const Volume dim)
{
  return false;
}
