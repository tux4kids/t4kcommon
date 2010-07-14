/*
  t4k-main.c

  Functions used to initialize the tux4kids-common library.

  Part of "Tux4Kids" Project
  http://www.tux4kids.com/

  Author: Boleslaw Kulbabinski <bkulbabinski@gmail.com>, (C) 2009

  Copyright: See COPYING file that comes with this distribution.
*/

#include "t4k_common.h"
#include "t4k_globals.h"

int debug_status;

/* these values have to match those used in games */
const int debug_loaders       = 1 << 0;
const int debug_menu          = 1 << 1;
const int debug_menu_parser   = 1 << 2;
const int debug_sdl           = 1 << 3;
const int debug_all           = ~0;

SDL_Color red, yellow, white, black;


/* set global variables */
/* TODO look into support for locale switching at runtime
** http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=490115 
** */
void InitT4KCommon(int debug_flags)
{
  printf("Initializing " PACKAGE_STRING "\n");

  debug_status = debug_flags;

  black.r       = 0x00; black.g       = 0x00; black.b       = 0x00;
  red.r         = 0xff; red.g         = 0x00; red.b         = 0x00;
  white.r       = 0xff; white.g       = 0xff; white.b       = 0xff;
  yellow.r      = 0xff; yellow.g      = 0xff; yellow.b      = 0x00;
}

