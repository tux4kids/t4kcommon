/*
  tk4-globals.h

  Part of "Tux4Kids" Project
  http://www.tux4kids.org/

  Copyright: See COPYING file that comes with this distribution
*/

#ifndef GLOBALS_H
#define GLOBALS_H

#include "SDL.h"

#define REG_RGBA 16,16,96,96
#define SEL_RGBA 16,16,128,128

#define MAX_FPS 30
#define PIXEL_BITS 32
#define DEFAULT_FONT_NAME "AndikaDesRevG.ttf"
#define PATH_MAX 1024
#define FONT_NAME_LENGTH 64

extern int dbg_status;

extern const int dbg_loaders;
extern const int dbg_menu;
extern const int dbg_menu_parser;
extern const int dbg_sdl;
extern const int dbg_all;

extern SDL_Color red, yellow, white, black;
/* debug macros */
#define DEBUGCODE(mask) if((mask) & dbg_status)
#define DEBUGMSG(mask, ...) if((mask) & dbg_status){ fprintf(stderr, __VA_ARGS__); fflush(stderr); }

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


#endif
