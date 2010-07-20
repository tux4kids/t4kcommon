/**
  tux4kids-common

  Library of common functions used in Tux4Kids games.

  Part of "Tux4Kids" Project
  http://www.tux4kids.com/

  Copyright: See COPYING file that comes with this distribution.
  \file t4k_common.h
*/

#ifndef TUX4KIDS_COMMON_H
#define TUX4KIDS_COMMON_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

#ifndef DEBUGVAR
# define DEBUGVAR(mask, Expr) if((mask) & (debug_status)) { fprintf(stderr, #Expr ": %s\n", (Expr)); fflush(stderr); }
#endif

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
# define rmask 0xff000000
# define gmask 0x00ff0000
# define bmask 0x0000ff00
# define amask 0x000000ff
#else
# define rmask 0x000000ff
# define gmask 0x0000ff00
# define bmask 0x00ff0000
# define amask 0xff000000
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
extern SDL_Rect menu_rect, stop_rect, prev_rect, next_rect;
extern SDL_Surface *stop_button, *prev_arrow, *next_arrow, *prev_gray, *next_gray;


#define MAX_SPRITE_FRAMES 15

typedef struct {
  SDL_Surface *frame[MAX_SPRITE_FRAMES];
  SDL_Surface *default_img;
  int num_frames;
  int cur;
} sprite;

/**
 * \enum WipeStyle
 * \brief Different transition effects used by TransWipe()
 */
typedef enum
{
  WIPE_BLINDS_VERT,
  WIPE_BLINDS_HORIZ,
  WIPE_BLINDS_BOX,
  RANDOM_WIPE,
  NUM_WIPES
} WipeStyle;

/**
 * special values used by RunMenu. RUN_MAIN_MENU is a special
 * activity that can be used in .xml menu structures but should not
 * be declared in activities' lists.
 * RunMenu returning QUIT indicates that user decided to quit application while
 * running the menu. Returning STOP indicates that user pressed stop button. 
 */
enum { 
	RUN_MAIN_MENU = -3, 
	QUIT = -2, 
	STOP = -1 
};



/* from tk4-menu.c */

/* from tk4-loaders.c */
#define IMG_REGULAR         0x01
#define IMG_COLORKEY        0x02
#define IMG_ALPHA           0x04
#define IMG_MODES           0x07

#define IMG_NOT_REQUIRED    0x10
#define IMG_NO_PNG_FALLBACK 0x20

//TODO most of these functions are documented...somewhere. That should end up
//in this header eventually

/* from t4k-main.c */
/**
 * \brief Initialize Tux4Kids-Common
 * \param debug_flags The flags used for debugging output
 * Games may define their own debug flags, but several are available by default:
 * debug_loaders      
 * debug_menu         
 * debug_menu_parser  
 * debug_sdl          
 * debug_all          
 */
void            InitT4KCommon(int debug_flags);

/* from t4k-menu.c */
/**
 * \brief 
 * \param num
 * \param acts
 */
void            T4K_SetActivitiesList       (int num, char** acts);
/**
 * \brief 
 * \param mus_path
 * \param click
 * \param hover
 */
void            T4K_SetMenuSounds           (char* mus_path, Mix_Chunk* click, Mix_Chunk* hover);
/**
 * \brief Set the prefix that is used whe loading menu sprites
 * \param pref the prefix that is used whe loading menu sprites
 */
void            T4K_SetMenuSpritePrefix      (char* pref);
/**
 * \brief create a simple one-level menu. All given strings are copied
 * \param index
 * \param items
 * \param item_names
 * \param sprite_names
 * \param title
 * \param trailer
 */
void            T4K_CreateOneLevelMenu      (int index, int items, char** item_names, char** sprite_names, char* title, char* trailer);
/**
 * \brief RunMenu - main function to display the menu and run the event loop
 * this function is a modified copy of choose_menu_item()
 * \param index
 * \param return_choice if true, then return chosen value instead of running handle_activity()
 * \return 
 */
int             T4K_RunMenu                 (int index, bool return_choice, void (*draw_background)(), int (*handle_event)(SDL_Event*), void (*handle_animations)(), int (*handle_activity)(int, int));
/**
 * \brief 
 * \param index
 */
void            T4K_PrerenderMenu           (int index);
/**
 * \brief prerender arrows, stop button and all non-NULL menus from menus[] array
   this function should be invoked after every resolution change
 */
void            T4K_PrerenderAll            ();
/**
 * \brief load menu from given XML file and store its tree under given index in "menus" array
 * \param index
 * \param file_name
 */
void            T4K_LoadMenu                (int index, const char* file_name);
/**
 * \brief free all loaded menu trees
 */
void            T4K_UnloadMenus             (void);

/* from tk4-sdl.c */
/**
 * \brief Return a pointer to the screen we're using, as an alternative to making screen a global variable
 * \return 
 */
SDL_Surface*    T4K_GetScreen               ();
/**
 * \brief creates a translucent button with rounded ends and draws it on the screen.
   All colors and alpha values are supported.
 * \param target_rect
 * \param radius
 * \param r
 * \param g
 * \param b
 * \param a
 */
void            T4K_DrawButton              (SDL_Rect* target_rect, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
/**
 * \brief creates a translucent button with rounded ends and draws it on the given surface.
   All colors and alpha values are supported.
 * \param target
 * \param target_rect
 * \param radius
 * \param r
 * \param g
 * \param b
 * \param a
 */
void            T4K_DrawButtonOn            (SDL_Surface* target, SDL_Rect* target_rect, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
/**
 * \brief creates a translucent button with rounded ends
   All colors and alpha values are supported.
 * \param w
 * \param h
 * \param radius
 * \param r
 * \param g
 * \param b
 * \param a
 * \return 
 */
SDL_Surface*    T4K_CreateButton            (int w, int h, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
/**
 * \brief 
 * \param s
 * \param radius
 */
void            T4K_RoundCorners            (SDL_Surface* s, Uint16 radius);
/**
 * \brief Flip a surface vertically or horizontally
 * \param in The source surface
 * \param x if nonzero, the image will be flipped horizontally
 * \param y if nonzero, the image will be flipped vertically
 * \return a newly created SDL_Surface* 
 */
SDL_Surface*    T4K_Flip                    (SDL_Surface *in, int x, int y);
/**
 * \brief Blend two surfaces together. The third argument is between 0.0 and
   1.0, and represents the weight assigned to the first surface.  If
   the pointer to the second surface is NULL, this performs fading.

   Currently this works only with RGBA images, but this is largely to
   make the (fast) pointer arithmetic work out; it could be easily
   generalized to other image types.
 * \param S1
 * \param S2
 * \param gamma
 * \return 
 */
SDL_Surface*    T4K_Blend                   (SDL_Surface *S1, SDL_Surface *S2, float gamma);
/**
 * \brief free every surface in the array together with the array itself
 * \param surfs
 * \param length
 */
void            T4K_FreeSurfaceArray        (SDL_Surface** surfs, int length);
/**
 * \brief Text whether (x, y) is inside the SDL_Rect r
 * \param r
 * \param x
 * \param y
 * \return whether (x, y) is inside the SDL_Rect r.
 */
int             T4K_inRect                  (SDL_Rect r, int x, int y);
/**
 * \brief Fill in an SDL_Rect with absolute values based on screen dimensions
 * \param rect
 * \param pos four floats between 0.0 and 1.0 which specify the desired x, y, w 
 * and h as a percentage of screen dimensions
 */
void            T4K_SetRect                 (SDL_Rect* rect, const float* pos);
/**
 * \brief Wrap a call to SDL_UpdateRect
 * \param surf
 * \param rect
 */
void            T4K_UpdateRect              (SDL_Surface* surf, SDL_Rect* rect);
/**
 * \brief Darkens the screen by a factor of 2^bits
 * \param bits
 */
void            T4K_DarkenScreen            (Uint8 bits);
/**
 * \brief change window size (works only in windowed mode)
 * \param new_res_x
 * \param new_res_y
 */
void            T4K_ChangeWindowSize        (int new_res_x, int new_res_y);
/**
 * \brief switch between fullscreen and windowed mode. This does <em>not</em>
 * perform any resizing of menu items, so a call to T4K_PrerenderAll may be
 * necessary
 */
void            T4K_SwitchScreenMode        (void);
/**
 * \brief Block application until SDL receives an appropriate event. 
 * Use sparingly.
 * \param events a single or OR'd combination of event masks. 
 * \return the event type received
 */
SDL_EventType   T4K_WaitForEvent            (SDL_EventMask events);
/**
 * \brief Resize an existing surface
 * \param src
 * \param new_w
 * \param new_h
 * \return 
 */
SDL_Surface*    T4K_zoom                    (SDL_Surface* src, int new_w, int new_h);
/**
 * \brief a single or OR'd combination of event masks. 
 * \param newbkg
 * \param type
 * \param segments
 * \param duration
 * \return 
 */
int             T4K_TransWipe               (const SDL_Surface* newbkg, int type, int segments, int duration);
/**
 * \brief 
 */
void            T4K_InitBlitQueue           (void);
/**
 * \brief just set the number of pending updates to zero
 */
void            T4K_ResetBlitQueue          (void);
/**
 * \brief Don't actually blit a surface,    but add a rect to be updated next    update
 * \param src
 * \param dst
 * \return 
 */
int             T4K_AddRect                 (SDL_Rect* src, SDL_Rect* dst);
/**
 * \brief 
 * \param gfx
 * \param x
 * \param y
 * \return 
 */
int             T4K_DrawSprite              (sprite* gfx, int x, int y);
/**
 * \brief Draw an object at the specified location. No respect to clipping!
 * \param surf
 * \param x
 * \param y
 * \return 
 */
int             T4K_DrawObject              (SDL_Surface* surf, int x, int y);
/**
 * \brief Update the screen and increment the frame counter
 * \param frame
 */
void            T4K_UpdateScreen            (int* frame);
/**
 * \brief basically puts in an order to overdraw sprite with corresponding rect of bkgd img                                                
 * \param img
 * \param curr_bkgd
 * \param x
 * \param y
 * \return 
 */
int             T4K_EraseSprite             (sprite* img, SDL_Surface* curr_bkgd, int x, int y);
/**
 * \brief Erase an object from the screen
 * \param surf
 * \param curr_bkgd
 * \param x
 * \param y
 * \return 
 */
int             T4K_EraseObject             (SDL_Surface* surf, SDL_Surface* curr_bkgd, int x, int y);
/**
 * \brief Set the "global" font name
 * \param name
 */
void            T4K_SetFontName             (const char* name);
/**
 * \brief get the "global" font name
 * \return the "global" font name
 */
const char*     T4K_AskFontName             (void);
/**
 * \brief Initialize the backend (Pango or TTF) used for text-drawing functions
 * \return 
 */
int             T4K_Setup_SDL_Text          (void);
/**
 * \brief 
 */
void            T4K_Cleanup_SDL_Text        (void);
/**
 * \brief T4K_BlackOutline() creates a surface containing text of the designated
 * foreground color, surrounded by a black shadow, on a transparent
 * background.  The appearance can be tuned by adjusting the number of
 * background copies and the offset where the foreground text is
 * finally written (see below).                                        
 * \param t The text to draw
 * \param size the font size to use
 * \param c the fill color to use
 * \return 
 */
SDL_Surface*    T4K_BlackOutline            (const char* t, int size, SDL_Color* c);
/**
 * \brief returns a non-outlined surf using either SDL_Pango or SDL_ttf
 * \param t
 * \param size
 * \param col
 * \return a non-outlined surf using either SDL_Pango or SDL_ttf
 */
SDL_Surface*    T4K_SimpleText              (const char *t, int size, SDL_Color* col);
/**
 * \brief Same as T4K_SimpleText, but the text offset is also stored
 * \param t
 * \param size
 * \param col
 * \param glyph_offset the var in which to store the offset
 * \return 
 */
SDL_Surface*    T4K_SimpleTextWithOffset    (const char *t, int size, SDL_Color* col, int *glyph_offset);

/* from tk4-loaders.c */
/**
 * \brief Add a directory that should be searched when loading assets 
 * \param path
 */
void            T4K_AddDataPrefix           (const char* path);
/**
 * \brief Check whether a file exists
 * \param file
 * \return 1 if valid file, 0 if not:
 */
int             T4K_CheckFile               (const char* file);

/**
 * \brief Remove a trailing slash from a file path
 * \param path a path that may or may not end in a slash
 * \return a path that does not end in a slash
 */
char*           T4K_RemoveSlash(char *path);

/**
 * \brief Load an image without resizing it
 * \param file_name
 * \param mode
 * \return 
 */
SDL_Surface*    T4K_LoadImage               (const char* file_name, int mode);
/**
 * \brief Load an image and resize it to given dimensions.
   If width or height is negative no resizing is applied.
   The loader (load_svg() or IMG_Load()) is chosen depending on file extension,
   If an SVG file is not found try to load its PNG equivalent
   (unless IMG_NO_PNG_FALLBACK is set)
 * \param file_name
 * \param mode
 * \param width
 * \param height
 * \return 
 */
SDL_Surface*    T4K_LoadScaledImage         (const char* file_name, int mode, int width, int height);
/**
 * \brief Same as LoadScaledImage but preserve image proportions
   and fit it into max_width x max_height rectangle.
   Returned surface is not necessarily max_width x max_height !
 * \param file_name
 * \param mode
 * \param max_width
 * \param max_height
 * \return 
 */
SDL_Surface*    T4K_LoadImageOfBoundingBox  (const char* file_name, int mode, int max_width, int max_height);
/**
 * \brief a wrapper for LoadImage() that optimizes
   the format of background image
 * \param file_name
 * \param width
 * \param height
 * \return 
 */
SDL_Surface*    T4K_LoadBkgd                (const char* file_name, int width, int height);
/**
 * \brief 
 * \param name
 * \param mode
 * \return 
 */
sprite*         T4K_LoadSprite              (const char* name, int mode);
/**
 * \brief 
 * \param name
 * \param mode
 * \param width
 * \param height
 * \return 
 */
sprite*         T4K_LoadScaledSprite        (const char* name, int mode, int width, int height);
/**
 * \brief 
 * \param name
 * \param mode
 * \param max_width
 * \param max_height
 * \return 
 */
sprite*         T4K_LoadSpriteOfBoundingBox (const char* name, int mode, int max_width, int max_height);
/**
 * \brief 
 * \param in
 * \param X
 * \param Y
 * \return 
 */
sprite*         T4K_FlipSprite              (sprite* in, int X, int Y);
/**
 * \brief 
 * \param gfx
 */
void            T4K_FreeSprite              (sprite* gfx);
/**
 * \brief 
 * \param s
 */
void            T4K_NextFrame               (sprite* s);
/**
 * \brief Load a sound/music patch from a file.
 * \param datafile
 * \return 
 */
Mix_Chunk*      T4K_LoadSound               (char* datafile);
/**
 * \brief Load music from a datafile
 * \param datafile
 * \return 
 */
Mix_Music*      T4K_LoadMusic               (char *datafile);

/* from tk4-audio.c */
/**
 * \brief 
 * \param sound
 */
void            T4K_PlaySound               (Mix_Chunk* sound);
/**
 * \brief 
 * \param sound
 * \param loops
 */
void            T4K_PlaySoundLoop           (Mix_Chunk* sound, int loops);
/**
 * \brief 
 * \param channel
 */
void            T4K_AudioHaltChannel        ( int channel );
/**
 * \brief 
 * \param music_path
 * \param loops
 */
void            T4K_AudioMusicLoad          (char* music_path, int loops);
/**
 * \brief 
 */
void            T4K_AudioMusicUnload        ();
/**
 * \brief 
 * \return 
 */
bool            T4K_IsPlayingMusic          ();
/**
 * \brief 
 * \param musicData
 * \param loops
 */
void            T4K_AudioMusicPlay          (Mix_Music *musicData, int loops);

#endif
