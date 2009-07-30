/*
  tux4kids-common

  Library of common functions used in Tux4Kids games.

  Part of "Tux4Kids" Project
  http://www.tux4kids.com/

  Copyright: See COPYING file that comes with this distribution.
*/

#ifndef TUX4KIDS_COMMON_H
#define TUX4KIDS_COMMON_H

#include "SDL.h"

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define rmask 0xff000000
#define gmask 0x00ff0000
#define bmask 0x0000ff00
#define amask 0x000000ff
#else
#define rmask 0x000000ff
#define gmask 0x0000ff00
#define bmask 0x00ff0000
#define amask 0xff000000
#endif


/* functions from t4k-main.c */
void            SetDebugMode(int dbg_flags);

/* functions from tk4-sdl.c */
SDL_Surface*    GetScreen();
void            DrawButton(SDL_Rect* target_rect, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void            DrawButtonOn(SDL_Surface* target, SDL_Rect* target_rect, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
SDL_Surface*    CreateButton(int w, int h, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void            RoundCorners(SDL_Surface* s, Uint16 radius);

#endif
