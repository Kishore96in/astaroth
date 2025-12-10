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
utility Kernel randomize() {

// results in (-rng_scale, rng_scale] range

    rng_scale = 1e-5

    for field in 0:NUM_VTXBUF_HANDLES{
        r = 2.0 * rand_uniform() - 1.0
        write(Field(field), rng_scale * r)
    }
}
utility Kernel constant() {
    
    AC_value = 1.0

    for field in 0:NUM_FIELDS {
        write(Field(field), AC_value)
    }
}
