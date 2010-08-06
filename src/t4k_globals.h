/*
  tk4-globals.h

  Part of "Tux4Kids" Project
  http://www.tux4kids.org/

  Copyright: See COPYING file that comes with this distribution
*/

#ifndef GLOBALS_H
#define GLOBALS_H

#include "t4k_common.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

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
/* debug macros */
#define DEBUGCODE(mask) if((mask) & debug_status)
#define DEBUGMSG(mask, ...) if((mask) & debug_status){ fprintf(stderr, __VA_ARGS__); fflush(stderr); }

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
