/*
   t4k_scandir.h:

   Header for scandir replacement function when not provided by platform.

   Copyright 2011 by David Bruce, to extent a single function
   prototype is copyrightable.

   t4k_scandir.h is part of the t4k_common library.

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

//==============================================================================
//
//! \file
//!     t4k_scandir.h
//!
//! \mainpage
//!     Tux4Kids_common
//!
//! \section intro_sec Description
//!     NOTE: this is DEPRECATED and will be removed - tuxmath, tuxtype,
//!     and other programs should use T4K_scandir() instead which is
//!     designed to always be available, either as a wrapper for the
//!     platform's scandir() or via our replacement. 
//!     
//!
//!     Part of "Tux4Kids" Project
//!
//!     http://www.tux4kids.com/
//!
//!
//! \section copyright_sec Copyright
//! 
//!     See COPYING file that comes with this distribution.
//!
#ifndef T4K_SCANDIR_H
#define T4K_SCANDIR_H

//==============================================================================
//!
//!  scandir
//!
//! \brief 
//!     Replacement scandir program for platforms lacking this function
//! 
//!     From man scandir (arg names adapted):
//!     The scandir() function scans the directory dirname, calling sdfilter() 
//!     on each directory entry. Entries for which sdfilter() returns nonzero
//!     are stored in strings allocated via malloc(3), sorted using qsort(3)
//!     with the comparison function dcomp(), and collected in array namelist
//!     which is allocated via  malloc(3).  If  sdfilter  is NULL, all entries
//!     are selected.
//!
//!     The  alphasort()  and versionsort() functions can be used as the comparison
//!     function compar().  The former sorts directory entries using strcoll(3), 
//!     the latter using strverscmp(3) on the strings (*a)->d_name and (*b)->d_name.
//!
//! \param
//!     const char *dirname
//!
//! \param
//!     struct dirent*** namelist
//!
//! \param
//!     int (*sdfilter)(struct dirent *)
//!
//! \param
//!     int (*dcomp)(const void *, const void *)
//!
//! \return 
//!     The scandir() function returns the number of directory entries selected
//!     or -1 if an error occurs.

int scandir(const char *dirname, struct dirent ***namelist, int (*sdfilter)(struct dirent *), int (*dcomp)(const void *, const void *));

#endif


