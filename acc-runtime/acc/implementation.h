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

// IMPLEMENTATION        : defined as acc-runtime compile option
// MAX_THREADS_PER_BLOCK : defined as acc-runtime compile option

// Implementations
#define IMPLICIT_CACHING (1)
#define EXPLICIT_ROLLING_PINGPONG (2)
#define EXPLICIT_CACHING_3D_BLOCKING (3)
#define EXPLICIT_CACHING_4D_BLOCKING (4)
#define EXPLICIT_PINGPONG_txw (5)
#define EXPLICIT_PINGPONG_txy (6)
#define EXPLICIT_CACHING (7) // Old, note changed number
			     //
#define EXPLICIT_PINGPONG_txyblocked (8)
#define EXPLICIT_PINGPONG_txyz (9)

#define EXPLICIT_ROLLING_PINGPONG_BLOCKSIZE                                    \
  (NUM_FIELDS <= 4 ? NUM_FIELDS : 4) // Must be less than NUM_FIELDS
