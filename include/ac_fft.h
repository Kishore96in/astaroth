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

#include "device_headers.h"
#ifdef __cplusplus
extern "C" {
#endif
AcResult acFFTBackwardTransformSymmetricC2R(const AcComplex* transformed_in, const Volume domain_size, const Volume subdomain_size,const Volume starting_point, AcReal* buffer);
AcResult acFFTForwardTransformSymmetricR2C(const AcReal* buffer, const Volume domain_size, const Volume subdomain_size, const Volume starting_point, AcComplex* transformed_in);
AcResult acFFTForwardTransformR2C(const AcReal* src, const Volume domain_size, const Volume subdomain_size, const Volume starting_point, AcComplex* dst);
AcResult acFFTForwardTransformPlanar(const AcReal* real_src, const AcReal* imag_src ,const Volume domain_size, const Volume subdomain_size, const Volume starting_point, AcReal* real_dst, AcReal* imag_dst);
AcResult acFFTBackwardTransformPlanar(const AcReal* real_src, const AcReal* imag_src ,const Volume domain_size, const Volume subdomain_size, const Volume starting_point, AcReal* real_dst, AcReal* imag_dst);
AcResult acFFTBackwardTransformPlanar2R(const AcReal* real_src, const AcReal* imag_src ,const Volume domain_size, const Volume subdomain_size, const Volume starting_point, AcReal* dst);
AcResult acFFTForwardTransformR2Planar(const AcReal* src,const Volume domain_size, const Volume subdomain_size, const Volume starting_point, AcReal* real_dst, AcReal* imag_dst);
AcResult acFFTForwardTransformR2PlanarBatched(const AcReal* src,const Volume domain_size, const Volume subdomain_size, const Volume starting_point, AcReal* real_dst, AcReal* imag_dst, const int batch_size);
AcResult acFFTForwardTransformR2HermitianPlanarBatched(const AcReal* src,const Volume domain_size, const Volume subdomain_size, const Volume starting_point, AcReal* real_dst, AcReal* imag_dst, const int batch_size, cudaStream_t stream);
AcResult acFFTBackwardTransformC2R(const AcComplex* transformed_in, const Volume domain_size, const Volume subdomain_size,const Volume starting_point, AcReal* buffer);
AcResult acFFTInit(const AcCommunicator* comm, const int* global_offset);
AcResult acFFTQuit();
#ifdef __cplusplus
}
#endif
