/*
   t4k_globals.h: Contains function prototypes for non-API functions
   global to t4k_common, along with some preprocessor macros.

   Copyright 2009, 2010.
   Author: Brendan Luchen
   Project email: <tuxmath-devel@lists.sourceforge.net>
   Project website: http://tux4kids.alioth.debian.org

t4k_globals.h is part of the t4k_common library.

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


#ifndef GLOBALS_H
#define GLOBALS_H


//FIXME this seems weird - config.h is where the "HAVE_FOO" autoheader macros live!
//#ifdef HAVE_CONFIG_H
#include "t4k_common-config.h"
//#endif
#include "gettext.h"
#include "t4k_common.h"
#include "SDL.h"

#define REG_RGBA 16,16,96,96
#define SEL_RGBA 16,16,128,128

#define MAX_FPS 30
#define PIXEL_BITS 32
#define DEFAULT_FONT_NAME "AndikaDesRevG.ttf"
#define PATH_MAX 1024
#define FONT_NAME_LENGTH 64
#define ERASE_MARGIN 5


extern int debug_status;

extern SDL_Color red, yellow, white, black;

void putpixel8(SDL_Surface * surface, int x, int y, Uint32 pixel);
void putpixel16(SDL_Surface * surface, int x, int y, Uint32 pixel);
void putpixel24(SDL_Surface * surface, int x, int y, Uint32 pixel);
void putpixel32(SDL_Surface * surface, int x, int y, Uint32 pixel);

extern void (*putpixels[]) (SDL_Surface *, int, int, Uint32);

Uint32 getpixel8(SDL_Surface * surface, int x, int y);
Uint32 getpixel16(SDL_Surface * surface, int x, int y);
Uint32 getpixel24(SDL_Surface * surface, int x, int y);
Uint32 getpixel32(SDL_Surface * surface, int x, int y);

extern Uint32(*getpixels[]) (SDL_Surface *, int, int);

/* Non-API global functions */
/* From t4k_menu.c */
int         size_text(const char* text, int font_size, int* width, int* height);
/* From t4k_loaders.c */
const char* find_file(const char* base_name);
void T4K_GetUserDataDir(char *opt_path, char* suffix); //TODO make t4k_fileops.c
/* From t4k_sdl.c */
void internal_res_switch_handler(ResSwitchCallback callback);

#endif
