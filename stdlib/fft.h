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
#ifdef AC_MATH_FFT_INCLUDED
AcResult
ac_fft_split_diffusion_update(Field f, const AcReal dt, const AcReal diffusion_coeff, Field real_src, Field imag_src, Field real_dst, Field imag_dst)
{
	Device dev = acGridGetDevice();
	acDeviceFFTR2Planar(dev,f,real_src,imag_src);
	acDeviceSetInput(dev,AC_FFT_REAL_SRC,int(real_src));
	acDeviceSetInput(dev,AC_FFT_IMAG_SRC,int(imag_src));
	acDeviceSetInput(dev,AC_FFT_REAL_DST,int(real_dst));
	acDeviceSetInput(dev,AC_FFT_IMAG_DST,int(imag_dst));
	acDeviceSetInput(dev,AC_FFT_SPLIT_DIFFUSION_UPDATE_DT,dt);
	acDeviceSetInput(dev,AC_FFT_SPLIT_DIFFUSION_UPDATE_DIFFUSION_COEFF,diffusion_coeff);
    	acGridExecuteTaskGraph(acGetOptimizedDSLTaskGraph(AC_fft_split_diffusion_update_step),1);
	acDeviceFFTBackwardTransformPlanar2R(dev, real_dst, imag_dst, f);
	return AC_SUCCESS;
}
#endif
