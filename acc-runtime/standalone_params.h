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

input real AC_dt
input real AC_current_time
global output real AC_dt_min

// Additional params needed by standalone & standalone_mpi
// diagnostics period
int AC_save_steps

// snapshot period
int AC_bin_steps
real AC_bin_save_t

// slices output period
int AC_slice_steps
real AC_slice_save_t

// maximun number of time snapshots during runtime 
// Set AC_num_snapshots < 0 for unlimited snapshots
int AC_num_snapshots

// max simulation time
int AC_max_steps
real AC_max_time

// Forcing parameter generation period (if forcing is on)
int AC_forcing_period_steps
real AC_forcing_period_t

// Initial time step index, default is 0
int AC_start_step = 0

run_const bool AC_additive_timestep = true
global output real ALFVEN_SPEED_MAX
global output real UU_MAX_ADVEC
global output real AD_ONE_FLUID_MAX_ADVEC
global output real AC_MAX_SHOCK
run_const bool AC_timestep_calc_with_rhs = true

//  Run params
real AC_cdt
real AC_cdtv
//TP: same default as in PC
real AC_cdtv3 = 0.01
real AC_cdts
