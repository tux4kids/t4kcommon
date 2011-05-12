//==============================================================================
//
//   t4k_test.h: Test program for t4k_common library
//
//   Copyright 2011.
//   Author: David Bruce.
//   Project email: <tuxmath-devel@lists.sourceforge.net>
//   Project website: http://tux4kids.alioth.debian.org
//
//   t4k_test.h is part of the t4k_common library.
//
//   t4k_test is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//
//   t4k_test is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//==============================================================================

//==============================================================================
//
//! \file
//!     t4k_test.h
//!
//! \mainpage
//!     Tux4Kids_common
//!
//! \section intro_sec Description
//!     
//!     Test program for t4k_common library.
//!
//!     Part of "Tux4Kids" Project
//!
//!     http://www.tux4kids.com/
//!
//!     Please look through t4k_common.h for detailed documentation.
//!
//! \section copyright_sec Copyright
//! 
//!     See COPYING file that comes with this distribution.
//!

#include "config.h"
#include "t4k_common.h"

int main(int argc, char* argv[])
{

    int i;

    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
        {
            /* Display help message: */

            fprintf(stderr, "\nt4k_test, a test program for the t4k_common library.\n"); 
            fprintf(stderr, "Run program with:\n"
          	  "--version, -v          - Display version number.\n"
          	  "--copyright, -c        - Display copyright and license information.\n"
          	  "--help, -h             - Display this help message.\n"
          	  "--silent, -s           - (Default) - run without interaction.\n"
          	  "--fullscreen, -f       - Run additional interactive sound and graphics tests in fullscreen mode.\n"
          	  "--windowed, -w         - Run additional interactive sound and graphics tests in window.\n"
          	  );

            fprintf(stderr, "\n");
            exit(0);
        }
        else if (strcmp(argv[i], "--version") == 0 ||
                strcmp(argv[i], "-v") == 0)
        {
            fprintf(stderr, "t4k_test program for t4k_common library, Version %s.\n", VERSION);
            exit(0);
        }
        else /* Warn for unknown option, except debug flags */
            /* that we deal with separately:               */
        {
             fprintf(stderr, "Unknown option: %s\n", argv[i]);
        }
    }/* end of command-line args */

    fprintf(stderr, "Entering t4k_test (test program for t4k_common).\n");
    InitT4KCommon(debug_all);
    fprintf(stderr, "t4k_test exiting normally.\n");
    return 0;
}

