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


SDL_Color red, yellow, white, black;


/* set global variables */
/* TODO look into support for locale switching at runtime
** http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=490115 
** TODO seems like SDL_Init and friends should be done here...
** 
*/
void InitT4KCommon(int debug_flags)
{
  printf("Initializing " PACKAGE_STRING "\n");

  debug_status = debug_flags;

  black.r       = 0x00; black.g       = 0x00; black.b       = 0x00;
  red.r         = 0xff; red.g         = 0x00; red.b         = 0x00;
  white.r       = 0xff; white.g       = 0xff; white.b       = 0xff;
  yellow.r      = 0xff; yellow.g      = 0xff; yellow.b      = 0x00;
  
  T4K_InitBlitQueue();
}

