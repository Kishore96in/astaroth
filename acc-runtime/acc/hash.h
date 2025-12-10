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
#include <hashtable.h>
static inline const char*
intern(const char* buffer)
{
  if(!buffer)
	  return NULL;
  const char* str_from_map = (const char*)hashmap_get(&string_intern_hashmap, buffer, strlen(buffer));
  if(str_from_map)
	  return str_from_map;
  const char* dup = strdup(buffer);
  hashmap_put(&string_intern_hashmap,dup, strlen(dup),(void*)dup);
  return dup;
}
