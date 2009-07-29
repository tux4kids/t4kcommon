//
// C Interface: SDL_extras
//
// Description: A few handy functions for using SDL graphics.
//
//
// Author: David Bruce,,, <dbruce@tampabay.rr.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
// (briefly, GPL v3 or later).
//

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


/* FIXME get rid of as much global data as possible, esp. pointers */
SDL_Surface*    GetScreen();

/* Non-text graphics functions: */
void            DrawButton(SDL_Rect* target_rect, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void            DrawButtonOn(SDL_Surface* target, SDL_Rect* target_rect, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
SDL_Surface*    CreateButton(int w, int h, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void            RoundCorners(SDL_Surface* s, Uint16 radius);

#endif
