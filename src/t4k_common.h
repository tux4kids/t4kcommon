/*
   t4k_common.h: Contains public headers for the t4k_common library

   Copyright 2009, 2010.
   Author: Brendan Luchen
   Project email: <tuxmath-devel@lists.sourceforge.net>
   Project website: http://tux4kids.alioth.debian.org

t4k_common.h is part of the t4k_common library.

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

/**
  \file t4k_common.h
  \mainpage Tux4Kids_common

  \section intro_sec Description
  A library of common functions used in these Tux4Kids games
  - Tux, of Math Command
  - Tux Typing

  Part of "Tux4Kids" Project

  http://www.tux4kids.com/

  Please look through t4k_common.h for detailed documentation.

  \section copyright_sec Copyright
  See COPYING file that comes with this distribution.
*/

#ifndef TUX4KIDS_COMMON_H
#define TUX4KIDS_COMMON_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

/*
 * Debugging macros
 *
 * All macros take a debug mask as their first argument, which should roughly
 * indicate what part of the program the debugging info is related to, and
 * which can be one of the flags defined below, a program-specific flag, or
 * an OR'd combination.
 *
 * DEBUGVAR prints out the name and value of a string variable
 */
#ifndef DEBUGMSG
/** DEBUGVAR prints out the name and value of a string variable */
# define DEBUGVAR(mask, Expr) if((mask) & (debug_status)) { fprintf(stderr, #Expr ": %s\n", (Expr)); fflush(stderr); }
/** DEBUGVARX prints out the name and hex value of an integral variable */
# define DEBUGVARX(mask, Expr) if((mask) & (debug_status)) { fprintf(stderr, #Expr ": %x\n", (Expr)); fflush(stderr); }
/** DEBUGVARF prints out the name and decimal value of a floating point variable */
# define DEBUGVARF(mask, Expr) if((mask) & (debug_status)) { fprintf(stderr, #Expr ": %f\n", (Expr)); fflush(stderr); }
/** DEBUGCODE is a conditional and should be followed by a code block */
# define DEBUGCODE(mask) if((mask) & debug_status)
/** DEBUGMSG prints out a specific message, which can be formatted like printf */
# define DEBUGMSG(mask, ...) if((mask) & debug_status){ fprintf(stderr, __VA_ARGS__); fflush(stderr); }
#endif

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
# define rmask 0xff000000 /**< SDL red mask */
# define gmask 0x00ff0000 /**< SDL green mask */
# define bmask 0x0000ff00 /**< SDL blue mask */
# define amask 0x000000ff /**< SDL alpha mask */
#else
# define rmask 0x000000ff /**< SDL red mask */
# define gmask 0x0000ff00 /**< SDL green mask */
# define bmask 0x00ff0000 /**< SDL blue mask */
# define amask 0xff000000 /**< SDL alpha mask */
#endif

//Hold off on gettext until we decide if we are really going to
//use it from within t4kcommon - DSB
#define _(String) String
//#define _(String) gettext (String)

#ifndef bool
typedef enum { false, true } bool;
#endif

/* these values have to match those used in games */
static const int debug_loaders       = 1 << 0; /**< Debug image loading code */
static const int debug_menu          = 1 << 1; /**< Debug menu code */
static const int debug_menu_parser   = 1 << 2; /**< Debug XML parsing for menus */
static const int debug_sdl           = 1 << 3; /**< Debug image txf and other support code */
static const int debug_all           = ~0;     /**< Enable all debugging output (messy!) */

extern int debug_status;

/**
 * Games defining custom debug flags should use this constant to ensure
 * consistency with t4k_common values.
 *
 * For example: <code><pre>
 * const int debug_something_of_mine = 1 << START_CUSTOM_DEBUG;
 * const int debug_something_else    = 2 << START_CUSTOM_DEBUG;
 * const int debug_some_more_stuff   = 4 << START_CUSTOM_DEBUG;
 *  </pre></code>
 */
#define START_CUSTOM_DEBUG 4

/* FIXME: global vars such as screen should be hidden when all games
   are using only getters (such as GetScreen() ) */
extern SDL_Surface* screen;
extern SDL_Rect menu_rect, stop_rect, prev_rect, next_rect;
extern SDL_Surface *stop_button, *prev_arrow, *next_arrow, *prev_gray, *next_gray;


#define MAX_SPRITE_FRAMES 15 /**< The max number of images a single sprite can use */

/**
 * \struct sprite
 * \brief an animated sprite using a collection of SDL_Surfaces
 */
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
 * Special values used by RunMenu.
 */
enum {
	RUN_MAIN_MENU = -3, /**< can be used in .xml menu structures but should not be declared in activities' lists. */
	QUIT = -2, /**< user decided to quit application */
	STOP = -1 /**< user pressed the stop button */
};

/**
 * Strategies for determining menu font sizes
 */
typedef enum
{
  MF_UNIFORM, /**< All menus are searched and the largest size that will fit on all menus is used. */
  MF_BESTFIT, /**< Menus are searched separately for the largest fonts they can accommodate */
  MF_EXACTLY /**< The font size given is used directly; text may run off the screen. */
} MFStrategy;


/* from tk4-menu.c */

/* from tk4-loaders.c */
#define IMG_REGULAR         0x01
#define IMG_COLORKEY        0x02
#define IMG_ALPHA           0x04
#define IMG_MODES           0x07

#define IMG_NOT_REQUIRED    0x10
#define IMG_NO_PNG_FALLBACK 0x20

//TODO flesh out doc comments
//TODO separate headers for different areas a la SDL?


/* from t4k-main.c ------------------------------ */


/**
 * \brief Initialize Tux4Kids-Common
 * \param debug_flags The flags used for debugging output.
 * Games may define their own debug flags, but several are available by default:
 * - debug_loaders
 * - debug_menu
 * - debug_menu_parser
 * - debug_sdl
 * - debug_all
 */
void            InitT4KCommon(int debug_flags);


/**
 * \brief Handle events that should have consistent effects everywhere in the program
 * \param event the event to check
 * \return 0 if no action is necessary, 1 if the screen should be redrawn
 */
int             T4K_HandleStdEvents      (const SDL_Event* event);



/* from t4k-menu.c --------------------------------------- */


/**
 * \brief Specify the set of activities the menu system should handle
 * \param num The number of activities. acts should have num elements.
 * \param acts An array of strings, each an activity provided by the game
 */
void            T4K_SetActivitiesList       (int num, char** acts);


/**
 * \brief Set optional sound effects and music for menus
 * \param mus_path The path to background music. *NOT* used!
 * \param click The sound effect to play when an item is clicked
 * \param hover The sound effect to play when an item is highlighted
 */
void            T4K_SetMenuSounds           (char* mus_path, Mix_Chunk* click, Mix_Chunk* hover);


/**
 * \brief Set the prefix that is used whe loading menu sprites
 * \param pref the prefix that is used whe loading menu sprites
 */
void            T4K_SetMenuSpritePrefix      (char* pref);


/**
 * \brief Set the font size for managed menus.
 * \param strategy How to determine menus' font size
 * \param size A literal size to use. This will be ignored unless strategy is MF_EXACTLY
 */
void            T4K_SetMenuFontSize          (MFStrategy strategy, int size);


/**
 * \brief Dynamically create a simple menu. All given strings are copied
 * \param index The unique index of the menu
 * \param items The number of items in the menu
 * \param item_names
 * \param sprite_names
 * \param title The title of the menu
 * \param trailer An optional item appended to the end of item_names
 */
void            T4K_CreateOneLevelMenu      (int index, int items, char** item_names, char** sprite_names, char* title, char* trailer);


/**
 * \brief RunMenu - main function to display the menu and run the event loop
 * this function is a modified copy of choose_menu_item()
 * \param index The unique index of the menu
 * \param return_choice if true, then return chosen value instead of running handle_activity()
 * \param draw_background A function that draws game-specific items, called once per frame
 * \param handle_event A function to process game-specific events
 * \param handle_animations A function to animate game-specific items
 * \param handle_activity A function to start an activity when the user selects it
 * \return
 */
int             T4K_RunMenu                 (int index, bool return_choice, void (*draw_background)(), int (*handle_event)(SDL_Event*), void (*handle_animations)(), int (*handle_activity)(int, int));


/**
 * \brief prerender a single menu based on the screen resolution
 * \param index The unique index of the menu
 */
void            T4K_PrerenderMenu           (int index);


/**
 * \brief prerender all menus, arrows and stop button
   this function should be invoked after every resolution change
 */
void            T4K_PrerenderAll            ();


/**
 * \brief load menu from given XML file and store its tree under given index in "menus" array
 * \param index The unique index of the menu
 * \param file_name
 */
void            T4K_LoadMenu                (int index, const char* file_name);


/**
 * \brief free all loaded menu trees
 */
void            T4K_UnloadMenus             (void);


/* from tk4-sdl.c ------------------------------------- */


/**
 * \brief Return a pointer to the screen we're using, as an alternative to making screen a global variable
 * \return
 */
SDL_Surface*    T4K_GetScreen               ();


/**
 * \brief creates a translucent button with rounded ends and draws it on the screen.
   All colors and alpha values are supported. This is equivalent to <code>T4K_DrawButtonOn(T4K_GetScreen());</code>
 * \param target_rect the bounding rectangle for the button
 * \param radius The radius of the arcs on each corner. A smaller radius results in sharper edges.
 * \param r R component of the button's color
 * \param g G component of the button's color
 * \param b B component of the button's color
 * \param a The opacity of the button
 */
void            T4K_DrawButton              (SDL_Rect* target_rect, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);


/**
 * \brief creates a translucent button with rounded ends and draws it on the given surface.
 *  All colors and alpha values are supported.
 *
 *  This function creates a temporary surface to blit onto target.
 *  If performance is an issue, consider using T4K_CreateButton to save the surface.
 *
 * \param target The SDL_Surface to draw on
 * \param target_rect the bounding rectangle for the button
 * \param radius The radius of the arcs on each corner. A smaller radius results in sharper edges.
 * \param r R component of the button's color
 * \param g G component of the button's color
 * \param b B component of the button's color
 * \param a The opacity of the button
 */
void            T4K_DrawButtonOn            (SDL_Surface* target, SDL_Rect* target_rect, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);


/**
 * \brief creates a translucent button with rounded ends
   All colors and alpha values are supported.
 * \param w The width of the button
 * \param h The height of the button
 * \param radius The radius of the arcs on each corner. A smaller radius results in sharper edges.
 * \param r R component of the button's color
 * \param g G component of the button's color
 * \param b B component of the button's color
 * \param a The opacity of the button
 * \return a w x h translucent button with no text
 */
SDL_Surface*    T4K_CreateButton            (int w, int h, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);


/**
 * \brief Round the corners of a surface by erasing edge pixels
 * \param s The surface to process
 * \param radius The radius of the arcs on each corner. A smaller radius results in sharper edges.
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
 * \param S1 The first surface
 * \param S2 The second surface
 * \param gamma A value between 0.0 and 1.0, representing the weight assigned to the first surface.
 * \return
 */
SDL_Surface*    T4K_Blend                   (SDL_Surface *S1, SDL_Surface *S2, float gamma);


/**
 * \brief free every surface in the array together with the array itself
 * \param surfs an array of SDL_Surface pointers to free
 * \param length The size of the array
 */
void            T4K_FreeSurfaceArray        (SDL_Surface** surfs, int length);


/**
 * \brief Text whether the point (x, y) is inside the SDL_Rect r
 * \param r the bounding rect
 * \param x x coordinate to test
 * \param y y coordinate to test
 * \return whether (x, y) is inside the SDL_Rect r.
 */
int             T4K_inRect                  (SDL_Rect r, int x, int y);


/**
 * \brief Write an SDL_Rect with dimensions based on screen dimensions
 * \param rect A pointer to the rect to fill
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
 * \param bits An exponent between 1 and 8. Realistically, 1 and 2 are the only useful values
 */
void            T4K_DarkenScreen            (Uint8 bits);


/**
 * \brief change window size (unstable, works only in windowed mode)
 * \param new_res_x
 * \param new_res_y
 */
void            T4K_ChangeWindowSize        (int new_res_x, int new_res_y);


/**
 * \brief Switch between fullscreen and windowed mode. Resolution switching callbacks are invoked.
 */
void            T4K_SwitchScreenMode        (void);


/** A function to handle a resolution switch, which should take parameters for the new horizontal and vertical screen dimensions */
typedef void (*ResSwitchCallback)(int resx, int resy);
/**
 * \brief Register a callback to reposition and redraw screen elements when
 * the resolution is changed
 * \param callback A function to be called when resolution changes
 */
void            T4K_OnResolutionSwitch      (ResSwitchCallback callback);


/**
 * \brief Block application until SDL receives an appropriate event.
 * Use sparingly.
 * \param events a single or OR'd combination of event masks.
 * \return the event type received
 */
SDL_EventType   T4K_WaitForEvent            (SDL_EventMask events);


/**
 * \brief Scale an existing surface
 * \param src The original surface, which is left unscathed
 * \param new_w The width of the new surface
 * \param new_h The height of the new surface
 * \return a newly allocated SDL_Surface
 */
SDL_Surface*    T4K_zoom                    (SDL_Surface* src, int new_w, int new_h);


/**
 * \brief perform a wipe from the current screen image to a new one.
 * \param newbkg The image to wipe to
 * \param type The WipeStyle to use
 * \param segments
 * \param duration the length of the animation in milliseconds
 * \return
 */
int             T4K_TransWipe               (const SDL_Surface* newbkg, WipeStyle type, int segments, int duration);


/**
 * \brief Initialize the blit queue system. This must be called before
 * T4K_ResetBlitQueue,
 * T4K_AddRect,
 * T4K_DrawSprite,
 * T4K_DrawObject,
 * T4K_EraseObject,
 * T4K_EraseSprite or
 * T4K_UpdateScreen
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
 * \return 1 on success, 0 on failure
 */
int             T4K_Setup_SDL_Text          (void);


/**
 * \brief Shut down the backend used for text-drawing functions
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


/* from tk4-loaders.c ------------------------------------------ */


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

   The loader backend is chosen depending on file extension,
   If an SVG file is not found, try to load its PNG equivalent
   (unless IMG_NO_PNG_FALLBACK is set in mode)
 * \param file_name
 * \param mode
 * \param width
 * \param height
 * \return
 */
SDL_Surface*    T4K_LoadScaledImage         (const char* file_name, int mode, int width, int height);


/**
 * \brief Same as LoadScaledImage but preserve image proportions
 *  and fit it into max_width x max_height rectangle.
 *
 * NOTE: Returned surface is not necessarily max_width x max_height !
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
 * \brief Load a multiple-frame sprite from disk. This function loads an SVG sprite or multiple PNGs as needed
 * \param name The filename of the sprite to load, <em>without</em> an extension
 * \param mode loader flags to use, semantics the same as LoadImage
 * \return The loaded sprite
 */
sprite*         T4K_LoadSprite              (const char* name, int mode);


/**
 * \brief Load a multiple-frame sprite from disk and scale it to the given dimensions. This function loads an SVG sprite or multiple PNGs as needed
 * \param name The filename of the sprite to load, <em>without</em> an extension
 * \param mode loader flags to use, semantics the same as LoadImage
 * \param width
 * \param height
 * \return The loaded sprite
 */
sprite*         T4K_LoadScaledSprite        (const char* name, int mode, int width, int height);


/**
 * \brief Same as LoadScaledSprite but preserve image proportions
 *  and fit it into max_width x max_height rectangle.
 * NOTE: Returned surface is not necessarily max_width x max_height !
 *
 * \param name The filename of the sprite to load, <em>without</em> an extension
 * \param mode loader flags to use, semantics the same as LoadImage
 * \param max_width
 * \param max_height
 * \return The loaded sprite
 */
sprite*         T4K_LoadSpriteOfBoundingBox (const char* name, int mode, int max_width, int max_height);


/**
 * \brief Flip (reflect) a sprite over one or both axes
 * \param in The original image
 * \param X if nonzero, the image is flipped horizontally
 * \param Y if nonzero, the image is flipped vertically
 * \return A newly allocated transformed sprite
 * \see T4K_Flip
 */
sprite*         T4K_FlipSprite              (sprite* in, int X, int Y);


/**
 * \brief Free memory allocated for a loaded sprite
 * \param gfx The sprite to free
 */
void            T4K_FreeSprite              (sprite* gfx);


/**
 * \brief Advance a sprite's frame
 * \param s The sprite to advance
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


/* from tk4-audio.c ------------------------------------------- */


const static int T4K_AUDIO_PLAY_ONCE    =  0;
const static int T4K_AUDIO_LOOP_FOREVER = -1;

/**
 * \brief play sound once and exit
 * \param sound
 */
void            T4K_PlaySound               (Mix_Chunk* sound);


/**
 * \brief play sound "loops" times, -1 for infinite
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
 * \brief attempts to load and play the music file
 * \param music_path
 * \param loops
 */
void            T4K_AudioMusicLoad          (char* music_path, int loops);


/**
 * \brief attempts to unload any music data that was
 * loaded using the audioMusicLoad function
 */
void            T4K_AudioMusicUnload        ();


/**
 * \brief
 * \return
 */
bool            T4K_IsPlayingMusic          ();


/**
 * \brief attempts to play the passed music data, stopping current music if necessary
 * \param musicData
 * \param loops the number of times to loop, or -1 forever
 */
void            T4K_AudioMusicPlay          (Mix_Music *musicData, int loops);


/**
 * \brief Enable/disable sound
 * \param enabled
 */
void            T4K_AudioEnable             (bool enabled);


/**
 * \brief Toggle sound enablement
 */
void            T4K_AudioToggle             ();

#endif

