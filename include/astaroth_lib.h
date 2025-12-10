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
#ifndef AC_LIB_H
#define AC_LIB_H

//#ifdef __cplusplus
//  class AcLibHandle
//  {
//	  private:
//		  void* handle;
//	  public:
//		  AcLibHandle(void* _handle) : handle(_handle){}
//		  ~AcLibHandle() {dlclose(handle); }
//  };
//#else
//  typedef void* AcLibHandle;
//#endif
typedef void* AcLibHandle;
static AcLibHandle astarothLibHandle=NULL, kernelsLibHandle=NULL, utilsLibHandle=NULL;

#endif
