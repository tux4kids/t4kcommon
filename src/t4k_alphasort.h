/*
   t4k_alphasort.h:

   Header for alphasort replacement function when not provided by platform.
   
   Copyright 2011 by David Bruce, to extent a single function
   prototype is copyrightable.

t4k_alphasort.h is part of the t4k_common library.

t4k_common is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

t4k_common is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef T4K_ALPHASORT_H
#define T4K_ALPHASORT_H

int alphasort(const struct dirent** d1, const struct dirent** d2);

#endif


