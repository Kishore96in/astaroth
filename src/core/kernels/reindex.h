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
#ifdef __cplusplus
extern "C" 
{
#endif
AcResult acReindex(const cudaStream_t stream, //
                   const AcReal* in, const AcIndex in_offset,
                   const AcIndex in_shape, //
                   AcReal* out, const AcIndex out_offset,
                   const AcIndex out_shape, const AcShape block_shape);

/**
AcResult acReindexCross(const cudaStream_t stream, //
                        const VertexBufferArray vba, const AcIndex in_offset,
                        const AcShape in_shape, //
                        AcReal* out, const AcIndex out_offset,
                        const AcShape out_shape, const AcShape block_shape);

**/
#ifdef __cplusplus
}
#endif
