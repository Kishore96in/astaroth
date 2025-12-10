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
#include "astaroth_random.h"

static std::mt19937 rng(19937123);

void
seed_rng(uint32_t seed)
{
    rng.seed(seed);
}

std::mt19937&
get_rng()
{
    return rng;
}

/** Produces a uniformly random real number in the half-open interval [0,1). */
AcReal
random_uniform_real_01(void)
{
    std::uniform_real_distribution<AcReal> u01(0, 1);
    return u01(get_rng());
}
