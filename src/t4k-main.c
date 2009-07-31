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

char* data_prefix;

/* these values have to match those used in games */
const int dbg_loaders       = 1 << 2;
const int dbg_menu          = 1 << 4;
const int dbg_menu_parser   = 1 << 5;
const int dbg_sdl           = 1 << 10;
const int dbg_all           = ~0;

void SetDebugMode(int dbg_flags)
{
  dbg_status = dbg_flags;
}

void SetDataPrefix(char* pref)
{
  data_prefix = pref;
}
