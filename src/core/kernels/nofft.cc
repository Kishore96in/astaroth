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
#include "host_datatypes.h"
#include "ac_fft.h"
#include <stdio.h>
#include <cstdlib>

AcResult
acFFTForwardTransformSymmetricR2C(const AcReal*, const Volume, const Volume, const Volume, AcComplex*) {
	fprintf(stderr,"FATAL: need to have FFT_ENABLED=ON for acFFTForwardTransform!\n");
	fflush(stderr);
	exit(EXIT_FAILURE);
	return AC_FAILURE;
}

AcResult
acFFTForwardTransformR2C(const AcReal*, const Volume, const Volume, const Volume, AcComplex*) {
	fprintf(stderr,"FATAL: need to have FFT_ENABLED=ON for acFFTForwardTransform!\n");
	fflush(stderr);
	exit(EXIT_FAILURE);
	return AC_FAILURE;
}

AcResult
acFFTBackwardTransformSymmetricC2R(const AcComplex*,const Volume, const Volume,const Volume, AcReal*)
{
	fprintf(stderr,"FATAL: need to have FFT_ENABLED=ON for acFFTBackwardTransform!\n");
	fflush(stderr);
	exit(EXIT_FAILURE);
	return AC_FAILURE;
}
AcResult
acFFTBackwardTransformC2R(const AcComplex*,const Volume, const Volume,const Volume, AcReal*)
{
	fprintf(stderr,"FATAL: need to have FFT_ENABLED=ON for acFFTBackwardTransform!\n");
	fflush(stderr);
	exit(EXIT_FAILURE);
	return AC_FAILURE;
}

AcResult
acFFTForwardTransformR2Planar(const AcReal*, const Volume, const Volume, const Volume, AcReal*, AcReal*)
{
	fprintf(stderr,"FATAL: need to have FFT_ENABLED=ON for acFFTForwardTransformR2Planar!\n");
	fflush(stderr);
	exit(EXIT_FAILURE);
	return AC_FAILURE;
}

AcResult
acFFTForwardTransformR2PlanarBatched(const AcReal*, const Volume, const Volume, const Volume, AcReal*, AcReal*, int)
{
	fprintf(stderr,"FATAL: need to have FFT_ENABLED=ON for acFFTForwardTransformR2PlanarBatched!\n");
	fflush(stderr);
	exit(EXIT_FAILURE);
	return AC_FAILURE;
}

AcResult
acFFTForwardTransformPlanar(const AcReal*, const AcReal*,const Volume, const Volume, const Volume, AcReal*, AcReal*)
{
	fprintf(stderr,"FATAL: need to have FFT_ENABLED=ON for acFFTForwardTransformPlanar!\n");
	fflush(stderr);
	exit(EXIT_FAILURE);
	return AC_FAILURE;
}
AcResult
acFFTBackwardTransformPlanar(const AcReal*, const AcReal*,const Volume, const Volume, const Volume, AcReal*, AcReal*)
{
	fprintf(stderr,"FATAL: need to have FFT_ENABLED=ON for acFFTBackwardTransformPlanar!\n");
	fflush(stderr);
	exit(EXIT_FAILURE);
	return AC_FAILURE;
}
AcResult
acFFTBackwardTransformPlanar2R(const AcReal*, const AcReal*,const Volume, const Volume, const Volume, AcReal*)
{
	fprintf(stderr,"FATAL: need to have FFT_ENABLED=ON for acFFTBackwardTransformPlanar2R!\n");
	fflush(stderr);
	exit(EXIT_FAILURE);
	return AC_FAILURE;
}
AcResult
acFFTInit(const AcCommunicator*, const int*)
{
	return AC_SUCCESS;
}

AcResult
acFFTQuit()
{
	return AC_SUCCESS;
}
