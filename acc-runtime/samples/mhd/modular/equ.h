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
#include "../lib/operators.h"

//#if AC_EMBEDDED=1
#include "fieldecs.h"
// declaration list tb generated from PC-provided physics modules variables
#include "pardecs.h"

daa_dt() {
    return cross(vecvalue(UU), curl(AA)) + AC_eta * veclaplace(AA)
}

dlnrho_dt() {
    return -dot(vecvalue(UU), gradient(LNRHO)) - divergence(UU)
}

duu_dt() {
    S = stress_tensor(UU)
    cs2_sound = AC_cs * AC_cs
    cs2 = cs2_sound * exp(AC_gamma * value(SS) / AC_cp + (AC_gamma - 1.) * (value(LNRHO) - AC_lnrho0))

    j = (1. / AC_mu0) * (gradient_of_divergence(AA) - veclaplace(AA))
    B = curl(AA)
    inv_rho = 1. / exp(value(LNRHO))

    mom = - gradients(UU) * vecvalue(UU)
          - cs2 * ((1. / AC_cp) * gradient(SS) + gradient(LNRHO))
          + inv_rho * cross(j, B)
          + AC_nu * (veclaplace(UU) + (1. / 3.) * gradient_of_divergence(UU)
                  + 2. * S * gradient(LNRHO))
          + AC_zeta * gradient_of_divergence(UU)

    return mom
}

dlnT_dt() {
    return AC_lnT0
         + AC_gamma * value(SS) / AC_cp
         + (AC_gamma - 1.) * (value(LNRHO) - AC_lnrho0)
}

dss_dt() {
    S = stress_tensor(UU)
    inv_pT = 1. / (exp(value(LNRHO)) * exp(lnT()))
    j = (1. / AC_mu0) * (gradient_of_divergence(AA) - veclaplace(AA))
    RHS = (0) - (0) + AC_eta * AC_mu0 * dot(j, j) +
                       2. * exp(value(LNRHO)) * AC_nu * contract(S) +
                       AC_zeta * exp(value(LNRHO)) * divergence(UU) * divergence(UU)

    return -dot(vecvalue(UU), gradient(SS)) + inv_pT * RHS + heat_conduction()
}

