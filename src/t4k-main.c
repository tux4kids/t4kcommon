/*
  t4k-main.c

  Functions used to initialize the tux4kids-common library.

  Part of "Tux4Kids" Project
  http://www.tux4kids.com/

  Author: Boleslaw Kulbabinski <bkulbabinski@gmail.com>, (C) 2009

  Copyright: See COPYING file that comes with this distribution.
*/

#include "tux4kids-common.h"
#include "t4k-globals.h"

int dbg_status;

/* these values have to match those used in games */
const int dbg_loaders       = 1 << 2;
const int dbg_menu          = 1 << 4;
const int dbg_menu_parser   = 1 << 5;
const int dbg_sdl           = 1 << 10;
const int dbg_all           = ~0;

SDL_Color red, yellow, white, black;


/* set global variables */
void InitT4KCommon(int debug_flags)
{
  dbg_status = debug_flags;

  black.r       = 0x00; black.g       = 0x00; black.b       = 0x00;
  red.r         = 0xff; red.g         = 0x00; red.b         = 0x00;
  white.r       = 0xff; white.g       = 0xff; white.b       = 0xff;
  yellow.r      = 0xff; yellow.g      = 0xff; yellow.b      = 0x00;
}

