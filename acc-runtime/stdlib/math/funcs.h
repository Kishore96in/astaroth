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


norm(real3 vec)
{
	return sqrt(dot(vec,vec))
}
elemental abs(real x)
{
	return fabs(x)
}

elemental abs(int x)
{
	return AC_abs(x)
}
set_min_val(real val, real min_val)
{
	return max(val,min_val)
}
set_max_val(real val, real max_val)
{
	return min(val,max_val)
}
set_zero_below_threshold(real val, real threshold)
{
	return val > threshold ? val : 0.0;
}
epsilon(real x)
{
	suppress_unused_warning(x)
	return AC_REAL_EPSILON;
}

matmul_transpose(Matrix a, real3 b)
{
	suppress_unused_warning(a)
	suppress_unused_warning(b)
	fatal_error_message(true,"Not implemented matmul_tranpose")
	return real3(0.0,0.0,0.0)
}
sign(real val, real set_sign)
{

	return abs(val)*(set_sign < 0.0 ? -1.0 : 1.0);
}

modulo(real a, real p)
{
	return a - floor(a/p)*p
}
nint(real x)
{
	return round(x)
}
//TP: placeholder. Please port from Pencil code general.f90 to DSL :)
inline spline_integral(real[] a,b)
{
	print("NOT IMPLEMENTED spline_integral!\n")
	return a
}
tanh_step_function(real position, real center, real steepness, real L, real R)
{
	return L+(R-L)*0.5*(1.0 + tanh(steepness*(position-center)))	
}
tanh_step_function(real position, real center, real steepness, real3 L, real3 R)
{
	return L+(R-L)*0.5*(1.0 + tanh(steepness*(position-center)))
}
minmod_alt(real a, real b)
{
	return sign(0.5,a)*max(0.0,min(abs(a),sign(1.0,a)*b))
}
precise_sqrt_dot2(real3 a)
{
	a_max = max(abs(a)) + AC_REAL_EPSILON
	a_scaled = a/a_max
	b = dot(a_scaled,a_scaled)
	return a_max*sqrt(b)
}
quintic_step(real x, real x0, real width, real shift)
{
	real xi = (x-x0)/(width+AC_REAL_MIN)-shift
	xi = max(xi,-1.0)
	xi = min(xi,1.0)
        return 0.5 + xi*(0.9375 + (xi*xi)*(-0.625 + (xi*xi)*0.1875))
}
quintic_step(real x, real x0, real width)
{
	return quintic_step(x,x0,width,0.0)
}
