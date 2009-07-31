/*
  tk4-globals.h

  Part of "Tux4Kids" Project
  http://www.tux4kids.org/

  Copyright: See COPYING file that comes with this distribution
*/

#ifndef GLOBALS_H
#define GLOBALS_H

typedef enum { false, true } bool;

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

#define REG_RGBA 16,16,96,96
#define SEL_RGBA 16,16,128,128

#define MAX_FPS 30

extern int dbg_status;

extern const int dbg_loaders;
extern const int dbg_menu;
extern const int dbg_menu_parser;
extern const int dbg_sdl;
extern const int dbg_all;

extern char* data_prefix;

/* debug macros */
#define DEBUGCODE(mask) if((mask) & dbg_status)
#define DEBUGMSG(mask, ...) if((mask) & dbg_status){ fprintf(stderr, __VA_ARGS__); fflush(stderr); }

#endif
