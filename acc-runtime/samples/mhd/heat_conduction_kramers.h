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
heat_conduction_kramers() {

// Calculates diffusive term in entropy equation for Kramers conductivity.

      cv1 = 1./AC_cv_sound
      rho1 = exp(-value(VTXBUF_LNRHO))
      lnTT = AC_lnT0+cv1*value(VTXBUF_ENTROPY)+(AC_gamma-1)*(value(VTXBUF_LNRHO)-AC_lnrho0)
      glnrho = gradient(VTXBUF_LNRHO)

      glnTT  = cv1*gradient(VTXBUF_ENTROPY) + (AC_gamma-1)*glnrho
      del2lnTT = cv1*laplace(VTXBUF_ENTROPY) + (AC_gamma-1)*laplace(VTXBUF_LNRHO)

      Krho1 = AC_hcond0_kramers * pow(rho1,(2.*AC_n_kramers+1.)) * pow(exp(lnTT),(6.5*AC_n_kramers))   // = K/rho

      g2=dot(-2.*AC_n_kramers*glnrho+(6.5*AC_n_kramers+1)*glnTT,glnTT)
      return Krho1*(del2lnTT)+g2
}
