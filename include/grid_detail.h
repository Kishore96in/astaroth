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
// Header for accessing Astaroth internals

#include <mpi.h>

/**
Returns the MPI communicator used by all Astaroth processes.

If MPI was initialized with MPI_Init* instead of ac_MPI_Init, this will return MPI_COMM_WORLD
 */
//FUNC_DEFINE(MPI_Comm, acGridMPIComm,());
