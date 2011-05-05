/*
   t4k_main.c
   Functions used to initialize the t4k_common library

   Copyright 2009, 2010.
Authors: Boleslaw Kulbabinski Brendan Luchen
Project email: <tuxmath-devel@lists.sourceforge.net>
Project website: http://tux4kids.alioth.debian.org

t4k_main.c is part of the t4k_common library.

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

int T4K_HandleStdEvents (const SDL_Event* event)
{
    int ret = 0;

    if (event->type != SDL_KEYDOWN)
	return 0;

    SDLKey key = event->key.keysym.sym;

    /* Toggle screen mode: */
    if (key == SDLK_F10)
    {
	//    Opts_SetGlobalOpt(FULLSCREEN, !Opts_GetGlobalOpt(FULLSCREEN) );
	T4K_SwitchScreenMode();
	//    game_recalc_positions();
	ret = 1;
    }

    /* Toggle music: */
#ifndef NOSOUND
    else if (key == SDLK_F11)
    {
	T4K_AudioToggle();
    }
#endif

    return ret;
}
