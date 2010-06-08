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
#include "SDL_image.h"
#include "SDL_mixer.h"

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

//Hold off on gettext until we decide if we are really going to
//use it from within t4kcommon - DSB
#define _(String) String
//#define _(String) gettext (String)

typedef enum { false, true } bool;

extern const int debug_loaders;
extern const int debug_menu;
extern const int debug_menu_parser;
extern const int debug_sdl;
extern const int debug_all;

/* FIXME: global vars such as screen should be hidden when all games
   are using only getters (such as GetScreen() ) */
extern SDL_Surface* screen;

#define MAX_SPRITE_FRAMES 15

typedef struct {
  SDL_Surface *frame[MAX_SPRITE_FRAMES];
  SDL_Surface *default_img;
  int num_frames;
  int cur;
} sprite;

/* from t4k-main.c */
void            InitT4KCommon(int debug_flags);

/* from tk4-menu.c */

/* special values used by RunMenu. RUN_MAIN_MENU is a special
   activity that can be used in .xml menu structures but should not
   be declared in activities' lists.
   RunMenu returning QUIT indicates that user decided to quit application while
   running the menu. Returning STOP indicates that user pressed stop button. */
enum { RUN_MAIN_MENU = -3, QUIT = -2, STOP = -1 };


/* Windows is silly and defines some things to win32 calls. Perhaps we should
** adopt some sort of prefix to get around this --BML
*/
#ifdef WIN32
#undef LoadMenu
#undef PlaySound
#undef SetRect
#undef LoadImage
#endif

/* Evil, ugly externs */
extern SDL_Rect menu_rect, stop_rect, prev_rect, next_rect;
extern SDL_Surface *stop_button, *prev_arrow, *next_arrow, *prev_gray, *next_gray;

//TODO most of these functions are documented...somewhere. That should end up
//in this header eventually


#ifdef USE_T4K_PREFIX //Use T4K_FuncName() convention
# define PREFIXIFY(base_name) \
         T4K_ ## base_name
#else //Use FuncName() convention with no prefix
# define PREFIXIFY(base_name) base_name
#endif

void            PREFIXIFY( SetActivitiesList       ) (int num, char** acts);
void            PREFIXIFY( SetMenuSounds           ) (char* mus_path, Mix_Chunk* click, Mix_Chunk* hover);
void            PREFIXIFY( SetImagePathPrefix      ) (char* pref);
void            PREFIXIFY( CreateOneLevelMenu      ) (int index, int items, char** item_names, char** sprite_names, char* title, char* trailer);
int             PREFIXIFY( RunMenu                 ) (int index, bool return_choice, void (*draw_background)(), int (*handle_event)(SDL_Event*), void (*handle_animations)(), int (*handle_activity)(int, int));
void            PREFIXIFY( PrerenderMenu           ) (int index);
void            PREFIXIFY( PrerenderAll            ) ();
void            PREFIXIFY( LoadMenu                ) (int index, const char* file_name);
void            PREFIXIFY( UnloadMenus             ) (void);

/* from tk4-sdl.c */

/* For TransWipe(): */
enum
{
  WIPE_BLINDS_VERT,
  WIPE_BLINDS_HORIZ,
  WIPE_BLINDS_BOX,
  RANDOM_WIPE,
  NUM_WIPES
};
	 
SDL_Surface*    PREFIXIFY( GetScreen               ) ();
void            PREFIXIFY( DrawButton              ) (SDL_Rect* target_rect, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void            PREFIXIFY( DrawButtonOn            ) (SDL_Surface* target, SDL_Rect* target_rect, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
SDL_Surface*    PREFIXIFY( CreateButton            ) (int w, int h, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void            PREFIXIFY( RoundCorners            ) (SDL_Surface* s, Uint16 radius);
SDL_Surface*    PREFIXIFY( Flip                    ) (SDL_Surface *in, int x, int y);
SDL_Surface*    PREFIXIFY( Blend                   ) (SDL_Surface *S1, SDL_Surface *S2, float gamma);
void            PREFIXIFY( FreeSurfaceArray        ) (SDL_Surface** surfs, int length);
int             PREFIXIFY( inRect                  ) (SDL_Rect r, int x, int y);
void            PREFIXIFY( SetRect                 ) (SDL_Rect* rect, const float* pos);
void            PREFIXIFY( UpdateRect              ) (SDL_Surface* surf, SDL_Rect* rect);
void            PREFIXIFY( DarkenScreen            ) (Uint8 bits);
void            PREFIXIFY( ChangeWindowSize        ) (int new_res_x, int new_res_y);
void            PREFIXIFY( SwitchScreenMode        ) (void);
SDL_EventType   PREFIXIFY( WaitForEvent            ) (SDL_EventMask events);
SDL_Surface*    PREFIXIFY( zoom                    ) (SDL_Surface* src, int new_w, int new_h);
int             PREFIXIFY( TransWipe               ) (const SDL_Surface* newbkg, int type, int segments, int duration);
void            PREFIXIFY( InitBlitQueue           ) (void);
void            PREFIXIFY( ResetBlitQueue          ) (void);
int             PREFIXIFY( AddRect                 ) (SDL_Rect* src, SDL_Rect* dst);
int             PREFIXIFY( DrawSprite              ) (sprite* gfx, int x, int y);
int             PREFIXIFY( DrawObject              ) (SDL_Surface* surf, int x, int y);
void            PREFIXIFY( UpdateScreen            ) (int* frame);
int             PREFIXIFY( EraseSprite             ) (sprite* img, SDL_Surface* curr_bkgd, int x, int y);
int             PREFIXIFY( EraseObject             ) (SDL_Surface* surf, SDL_Surface* curr_bkgd, int x, int y);

/* Prototypes for t4k-sdl.c text functions */
int             PREFIXIFY( Setup_SDL_Text          ) (void);
void            PREFIXIFY( Cleanup_SDL_Text        ) (void);
SDL_Surface*    PREFIXIFY( BlackOutline            ) (const char* t, int size, SDL_Color* c);
SDL_Surface*    PREFIXIFY( SimpleText              ) (const char *t, int size, SDL_Color* col);
SDL_Surface*    PREFIXIFY( SimpleTextWithOffset    ) (const char *t, int size, SDL_Color* col, int *glyph_offset);

/* from tk4-loaders.c */
#define IMG_REGULAR         0x01
#define IMG_COLORKEY        0x02
#define IMG_ALPHA           0x04
#define IMG_MODES           0x07

#define IMG_NOT_REQUIRED    0x10
#define IMG_NO_PNG_FALLBACK 0x20


/**
 * Add a path to search for data (in addition to t4kcommon's install path and 
 * the root directory) This affects how LoadImage and friends are able to
 * find files.
 */
void            PREFIXIFY( AddDataPrefix           ) (const char* path);
int             PREFIXIFY( CheckFile               ) (const char* file);
SDL_Surface*    PREFIXIFY( LoadImage               ) (const char* file_name, int mode);
SDL_Surface*    PREFIXIFY( LoadScaledImage         ) (const char* file_name, int mode, int width, int height);
SDL_Surface*    PREFIXIFY( LoadImageOfBoundingBox  ) (const char* file_name, int mode, int max_width, int max_height);
SDL_Surface*    PREFIXIFY( LoadBkgd                ) (const char* file_name, int width, int height);
sprite*         PREFIXIFY( LoadSprite              ) (const char* name, int mode);
sprite*         PREFIXIFY( LoadScaledSprite        ) (const char* name, int mode, int width, int height);
sprite*         PREFIXIFY( LoadSpriteOfBoundingBox ) (const char* name, int mode, int max_width, int max_height);
sprite*         PREFIXIFY( FlipSprite              ) (sprite* in, int X, int Y);
void            PREFIXIFY( FreeSprite              ) (sprite* gfx);
void            PREFIXIFY( NextFrame               ) (sprite* s);
Mix_Chunk*      PREFIXIFY( LoadSound               ) (char* datafile);
Mix_Music*      PREFIXIFY( LoadMusic               ) (char *datafile);

/* from tk4-loaders.c */

void            PREFIXIFY( PlaySound               ) (Mix_Chunk* sound);
void            PREFIXIFY( PlaySoundLoop           ) (Mix_Chunk* sound, int loops);
void            PREFIXIFY( AudioHaltChannel        ) ( int channel );
void            PREFIXIFY( AudioMusicLoad          ) (char* music_path, int loops);
void            PREFIXIFY( AudioMusicUnload        ) ();
bool            PREFIXIFY( IsPlayingMusic          ) ();
void            PREFIXIFY( AudioMusicPlay          ) (Mix_Music *musicData, int loops);

#endif
