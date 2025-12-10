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
global output real CG_rTr
global output real CG_pTAp
global output real CG_rp1Trp1

cg_compute_inner_products(real Ap,real p,real r)
{
	reduce_sum(r*r,CG_rTr)	
	reduce_sum(p*Ap,CG_pTAp)	
}

cg_advance_solution(Field x,real Ap,real p,Field r)
{
	alpha = CG_rTr/CG_pTAp
	write(x,x + alpha*p)
	rp1 = r - alpha*Ap
	write(r,rp1)
	reduce_sum(rp1*rp1,CG_rp1Trp1)	
	
}

cg_next_direction(Field p, Field r)
{
	beta = CG_rp1Trp1/CG_rTr
	write(p, r + beta*p)
}
