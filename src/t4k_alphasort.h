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

//==============================================================================

//==============================================================================
//
//! \file
//!     t4k_alphasort.h
//!
//! \mainpage
//!     Tux4Kids_common
//!
//! \section intro_sec Description
//!     
//!     Conditionally included header for replacement alphasort function
//!     To use, program should check whether platform provides this
//!     function using e.g. autoconf:
//!
//!     #ifdef HAVE_ALPHASORT
//!     #include <t4k_alphasort.h>
//!     #endif
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
#ifndef T4K_ALPHASORT_H
#define T4K_ALPHASORT_H

//==============================================================================
//
//  alphasort
//
//! \brief 
//!     Replacement alphasort program for platforms lacking this function
//! 
//! \param
//!     const struct dirent** d1 - first arg to be compared
//!
//! \param
//!     const struct dirent** d2 - second arg to be compared
//!
//! \return 
//!     From man alphasort: "alphasort() and versionsort() functions return an
//!     integer less than, equal to, or greater than zero if the first argument
//!     is considered to be  respectively  less than, equal to, or greater than
//!     the second."
int alphasort(const struct dirent** d1, const struct dirent** d2);

#endif


