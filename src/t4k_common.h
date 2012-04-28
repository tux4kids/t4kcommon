//==============================================================================
//
//   t4k_common.h: Contains public headers for the t4k_common library
//
//   Copyright 2009, 2010, 2011.
//   Authors: Brendan Luchen, David Bruce.
//   Project email: <tuxmath-devel@lists.sourceforge.net>
//   Project website: http://tux4kids.alioth.debian.org
//
//   t4k_common.h is part of the t4k_common library.
//
//   t4k_common is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//
//   t4k_common is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//==============================================================================

//==============================================================================
//
//! \file
//!     t4k_common.h
//!
//! \mainpage
//!     Tux4Kids_common
//!
//! \section intro_sec Description
//!     
//!     A library of common functions used in these Tux4Kids games
//!     - Tux, of Math Command
//!     - Tux Typing
//!
//!     Part of "Tux4Kids" Project
//!
//!     http://www.tux4kids.com/
//!
//!     Please look through t4k_common.h for detailed documentation.
//!
//! \section copyright_sec Copyright
//! 
//!     See COPYING file that comes with this distribution.
//!

#ifndef TUX4KIDS_COMMON_H
#define TUX4KIDS_COMMON_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <wchar.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

//==============================================================================
// Debugging macros
//
// All macros take a debug mask as their first argument, which should roughly
// indicate what part of the program the debugging info is related to, and
// which can be one of the flags defined below, a program-specific flag, or
// an OR'd combination.
//
// DEBUGVAR prints out the name and value of a string variable
#ifndef DEBUGMSG

//! DEBUGVAR prints out the name and value of a string variable
#define DEBUGVAR(mask, Expr) \
    if((mask) & (debug_status)) \
{ \
    fprintf(stderr, #Expr ": %s\n", (Expr)); fflush(stderr); \
}

//! DEBUGVARX prints out the name and hex value of an integral variable
#define DEBUGVARX(mask, Expr) \
    if((mask) & (debug_status)) \
{ \
    fprintf(stderr, #Expr ": %x\n", (Expr)); fflush(stderr); \
}

//! DEBUGVARF prints out the name and decimal value of a floating point variable
#define DEBUGVARF(mask, Expr) \
    if((mask) & (debug_status)) \
{ \
    fprintf(stderr, #Expr ": %f\n", (Expr)); fflush(stderr); \
}

//! DEBUGCODE is a conditional and should be followed by a code block
#define DEBUGCODE(mask) if((mask) & debug_status)

//! DEBUGMSG prints out a specific message, which can be formatted like printf
#define DEBUGMSG(mask, ...) \
    if((mask) & debug_status) \
{ \
    fprintf(stderr, __VA_ARGS__); fflush(stderr); \
}
#endif

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define rmask 0xff000000 //!< SDL red mask
#define gmask 0x00ff0000 //!< SDL green mask
#define bmask 0x0000ff00 //!< SDL blue mask
#define amask 0x000000ff //!< SDL alpha mask
#else
#define rmask 0x000000ff //!< SDL red mask
#define gmask 0x0000ff00 //!< SDL green mask
#define bmask 0x00ff0000 //!< SDL blue mask
#define amask 0xff000000 //!< SDL alpha mask
#endif

#ifndef bool
typedef enum
{
    false, 
    true
}
bool;
#endif

// Debug macros
#define DEBUGCODE(mask) if((mask) & debug_status)
#define DEBUGMSG(mask, ...) \
    if((mask) & debug_status) \
{ \
    fprintf(stderr, __VA_ARGS__); fflush(stderr); \
}

// These values have to match those used in games.
static const int debug_loaders       = 1 << 0; //!< Debug image loading code
static const int debug_menu          = 1 << 1; //!< Debug menu code
static const int debug_menu_parser   = 1 << 2; //!< Debug XML parsing for menus
static const int debug_sdl           = 1 << 3; //!< Debug image txf and other support code 
static const int debug_linewrap      = 1 << 4; //!< Debug for linewrap functions
static const int debug_i18n	     = 1 << 5; //!< Debug for gettext and UTF conversions
static const int debug_all           = ~0;     //!< Enable all debugging output (messy!)

extern int debug_status;

//Define our color constants:
static const SDL_Color black        = {0x00, 0x00, 0x00, 0xff};
static const SDL_Color gray         = {0x80, 0x80, 0x80, 0xff};
static const SDL_Color dark_blue    = {0x00, 0x00, 0x60, 0xff};
static const SDL_Color red          = {0xff, 0x00, 0x00, 0xff};
static const SDL_Color white        = {0xff, 0xff, 0xff, 0xff};
static const SDL_Color yellow       = {0xff, 0xff, 0x00, 0xff};
static const SDL_Color bright_green = {0x60, 0xff, 0x00, 0xff};

//!
//! Games defining custom debug flags should use this constant to ensure
//! consistency with t4k_common values.
//!
//! For example: <code><pre>
//! const int debug_something_of_mine = 1 << START_CUSTOM_DEBUG;
//! const int debug_something_else    = 2 << START_CUSTOM_DEBUG;
//! const int debug_some_more_stuff   = 4 << START_CUSTOM_DEBUG;
//! </pre></code>
//!
#define START_CUSTOM_DEBUG 4

// FIXME: global vars such as screen should be hidden when all games
// are using only getters (such as GetScreen() )
extern SDL_Surface* screen;
extern SDL_Rect menu_rect, stop_rect, prev_rect, next_rect;
extern SDL_Surface *stop_button, *prev_arrow, *next_arrow, *prev_gray, *next_gray;


#define MAX_SPRITE_FRAMES 15 /**< The max number of images a single sprite can use */

//==============================================================================
//!
//! \struct
//!     sprite
//!
//! \brief
//!     An animated sprite using a collection of SDL_Surfaces.
//!
typedef struct
{
    SDL_Surface *frame[MAX_SPRITE_FRAMES];
    SDL_Surface *default_img;
    int num_frames;
    int cur;
}
sprite;

//==============================================================================
//!
//! \enum
//!     WipeStyle
//!
//! \brief
//!     Different transition effects used by TransWipe().
//!
typedef enum
{
    WIPE_BLINDS_VERT,
    WIPE_BLINDS_HORIZ,
    WIPE_BLINDS_BOX,
    RANDOM_WIPE,
    NUM_WIPES
}
WipeStyle;

//==============================================================================
//!
//! Special values used by RunMenu.
//!
enum
{
    RUN_MAIN_MENU = -3,  //!< can be used in .xml menu structures but should not be declared in activities' lists.
    QUIT          = -2,  //!< user decided to quit application
    STOP          = -1   //!< user pressed the stop button
};

//==============================================================================
//!
//! \enum
//!     MFStrategy
//!
//! Strategies for determining menu font sizes.
//!
typedef enum
{
    MF_UNIFORM,   //!< All menus are searched and the largest size that will fit on all menus is used.
    MF_BESTFIT,   //!< Menus are searched separately for the largest fonts they can accommodate.
    MF_EXACTLY    //!< The font size given is used directly; text may run off the screen.
}
MFStrategy;

// from tk4_loaders.c
#define IMG_REGULAR         0x01
#define IMG_COLORKEY        0x02
#define IMG_ALPHA           0x04
#define IMG_MODES           0x07

#define IMG_NOT_REQUIRED    0x10
#define IMG_NO_PNG_FALLBACK 0x20

#define MAX_LINES           128 //!< Maximum lines to wrap.
#define MAX_LINEWIDTH       256 //!< Maximum characters of each line.

static char wrapped_lines[MAX_LINES][MAX_LINEWIDTH]; //!< Global buffer for wrapped lines.

//TODO separate headers for different areas a la SDL?

//==============================================================================
//                     Public Definitions in t4k_main.c                       
//==============================================================================

//==============================================================================
//
//  InitT4KCommon
//
//! \brief 
//!     Initialize Tux4Kids-Common
//! 
//! \param
//!     debug_flags     - The flags used for debugging output.
//!     
//!     Games may define their own debug flags, but several are available 
//!     by default:
//!     - debug_loaders
//!     - debug_menu
//!     - debug_menu_parser
//!     - debug_sdl
//!     - debug_all
//!
//! \return
//!     1 if successful, 0 otherwise
//!
int InitT4KCommon( int debug_flags );

//==============================================================================
//
//  CleanupT4KCommon
//
//! \brief 
//!     Cleanup Tux4Kids-Common
//! 
//! \param
//!     None
//!
//! \return
//!     None
//!
void CleanupT4KCommon( void );

//==============================================================================
//  
//  T4K_HandleStdEvents
//
//! \brief
//!     Handle events that should have consistent effects everywhere 
//!     in the program.
//! 
//! \param 
//!     event      - the event to check
//! 
//! \return
//!     0          - if no action is necessary 
//! \return
//!     1          - if the screen should be redrawn
//!
int T4K_HandleStdEvents( const SDL_Event* event );


//============================================================================== 
//                     Public Definitions in t4k_menu.c
//==============================================================================

//==============================================================================
//
//  T4K_SetActivitiesList
//
//! \brief
//!     Specify the set of activities the menu system should handle.
//! 
//! \param 
//!     num       - The number of activities. acts should have num elements.
//! \param 
//!     acts      - An array of strings, each an activity provided by the game.
//!
//! \return
//!     None
//!
void T4K_SetActivitiesList( int    num, 
                            char** acts 
                          );

//==============================================================================
// 
//  T4K_SetMenuSounds
//
//! \brief 
//!    Set optional sound effects and music for menus.
//! 
//! \param 
//!     mus_path      - The path to background music. *NOT* used!
//! \param 
//!     click         - The sound effect to play when an item is clicked
//! \param
//!     hover         - The sound effect to play when an item is highlighted
//!
//! \return
//!     None
//!
void T4K_SetMenuSounds( char*      mus_path, 
                        Mix_Chunk* click, 
                        Mix_Chunk* hover 
                      );

//==============================================================================
//
//  T4K_SetMenuSpritePrefix
//
//! \brief
//!     Set the prefix that is used whe loading menu sprites.
//! 
//! \param
//!     pref    - The prefix that is used whe loading menu sprites.
//!
//! \return
//!     None
//!
void T4K_SetMenuSpritePrefix( char* pref );

//==============================================================================
// 
//  T4K_SetMenuFontSize
//  
//! \brief
//!     Set the font size for managed menus.
//! 
//! \param 
//!     strategy     - How to determine menus' font size
//! \param 
//!     size         - A literal size to use. This will be ignored unless 
//!                    strategy is MF_EXACTLY.
//!
//! \return
//!     None
//!
void T4K_SetMenuFontSize( MFStrategy strategy,
                          int        size
                        );

//==============================================================================
//
//  T4K_CreateOneLevelMenu
//
//! \brief
//!     Dynamically create a simple menu. All given strings are copied.
//! 
//! \param 
//!     index        - The unique index of the menu.
//! \param 
//!     items        - The number of items in the menu.
//! \param 
//!     item_names   - The name of the items
//! \param 
//!     sprite_names - The name of sprite or icon.
//! \param 
//!     title        - The title of the menu.
//! \param 
//!     trailer      - An optional item appended to the end of item_names.
//!
//! \return
//!     None
//! 
void T4K_CreateOneLevelMenu( int     index,
                             int     items,
                             char**  item_names, 
                             char**  sprite_names, 
                             char*   title, 
                             char*   trailer
                            );

//============================================================================== 
//
//  T4K_RunMenu
//
//! \brief
//!     RunMenu - main function to display the menu and run the event loop
//!     this function is a modified copy of choose_menu_item()
//! 
//! \param 
//!     index             -  The unique index of the menu.
//! \param 
//!    return_choice      -  If true, then return chosen value instead of 
//!                          running handle_activity().
//! \param 
//!     draw_background   -  A function that draws game-specific items, 
//!                          called once per frame.
//! \param
//!     handle_event      -  A function to process game-specific events.
//! \param
//!     handle_animations -  A function to animate game-specific items.
//! \param 
//!     handle_activity   -  A function to start an activity when the user 
//!                          selects it.
//! 
//! \return
//!     Returns a value operation of the user in the menu.
//!
int T4K_RunMenu( int    index,
                 bool   return_choice,
                 void   (*draw_background)(),
                 int    (*handle_event)(SDL_Event*), 
                 void   (*handle_animations)(),
                 int    (*handle_activity)(int, int)
               );

//==============================================================================
// 
//  T4K_PrerenderMenu
//
//! \brief
//!     Prerender a single menu based on the screen resolution.
//! 
//! \param
//!    index    - The unique index of the menu.
//!
//! \return
//!    None
//!
void T4K_PrerenderMenu( int index );

//============================================================================== 
//
//  T4K_PrerenderAll
// 
//! \brief 
//!     Prerender all menus, arrows and stop button.
//!     This function should be invoked after every resolution change.
//!
//! \param
//!     None
//!
//! \return
//!     None
//!
void T4K_PrerenderAll( void );

//============================================================================== 
//
//  T4K_LoadMenu
//
//! \brief
//!     Load menu from given XML file and store its tree under given 
//!     index in "menus" array.
//! 
//! \param
//!     index       - The unique index of the menu.
//! \param 
//!     file_name   - The file name of the xml menu file.
//!
//! \return
//!     None
//!
void T4K_LoadMenu( int         index,
                   const char* file_name
                 );

//============================================================================== 
//  
//  T4K_UnloadMenus
//
//! \brief
//!     free all loaded menu trees
//!
//! \param
//!     None
//! 
//! \return
//!     None
//!
void T4K_UnloadMenus( void );


//==============================================================================
//                     Public Definitions in tk4_sdl.c
//==============================================================================

//============================================================================== 
//
//  T4K_GetScreen
//
//! \brief
//!     Return a pointer to the screen we're using, as an alternative 
//!     to making screen a global variable.
//!
//! \param
//!     None
//! 
//! \return
//!     The surface of the screen.
//!
SDL_Surface* T4K_GetScreen( void );


//============================================================================== 
//
//  T4K_GetResolutions
//
//! \brief
//!     Provide current values of x and y resolutions for windowed and 
//!     fullscreen modes.
//! 
//! \param
//!     win_x        - Will hold the width of windowed resolution.
//! \param
//!     win_y        - Will hold the height of windowed resolution.
//! \param
//!     full_x       - Will hold the width of fullscreen resolution.
//! \param
//!     full_y       - Will hold the  height of fullscreen resolution.
//!
//! \return
//!     1            - Successfull call of the function.
//! \return
//!     0            - Failed operation. 
//!
int T4K_GetResolutions( int* win_x,
                        int* win_y,
                        int* full_x,
                        int* full_y
                      );

//==============================================================================
// 
//  T4K_DrawButton
//
//! \brief
//!     Creates a translucent button with rounded ends and draws it on the 
//!     screen. All colors and alpha values are supported. This is 
//!     equivalent to <code>T4K_DrawButtonOn(T4K_GetScreen());</code>
//! 
//! \param
//!     target_rect      - The bounding rectangle for the button.
//! \param 
//!     radius           - The radius of the arcs on each corner.
//!                        A smaller radius results in sharper edges.
//! \param
//!     r                - R component of the button's color.
//! \param 
//!     g                - G component of the button's color.
//! \param 
//!     b                - B component of the button's color.
//! \param
//!     a                - The opacity of the button.
//!
//! \return
//!     None
//!
void T4K_DrawButton( SDL_Rect* target_rect,
                     int       radius,
                     Uint8     r,
                     Uint8     g,
                     Uint8     b,
                     Uint8     a
                   );

//============================================================================== 
//
//  T4K_DrawButtonOn
// 
//! \brief
//!     Creates a translucent button with rounded ends and draws it on 
//!     the given surface.
//!     All colors and alpha values are supported.
//!
//!     This function creates a temporary surface to blit onto target.
//!     If performance is an issue, consider using T4K_CreateButton 
//!     to save the surface.
//!
//! \param
//!     target        - The SDL_Surface to draw on
//! \param
//!     target_rect   - the bounding rectangle for the button
//! \param
//!     radius        - The radius of the arcs on each corner. 
//!                     A smaller radius results in sharper edges.
//! \param
//!     r             - R component of the button's color
//! \param
//!     g             - G component of the button's color
//! \param
//!     b             - B component of the button's color
//! \param 
//!     a             - The opacity of the button
//!
//! \return
//!     None
//!
void T4K_DrawButtonOn( SDL_Surface* target,
                       SDL_Rect*    target_rect,
                       int          radius,
                       Uint8        r,
                       Uint8        g,
                       Uint8        b,
                       Uint8        a 
                      );

//============================================================================== 
//
//  T4K_CreateButton
//
//! \brief
//!     Creates a translucent button with rounded ends
//!     All colors and alpha values are supported.
//! 
//! \param
//!     w        - The width of the button
//! \param
//!     h        - The height of the button
//! \param
//!     radius   - The radius of the arcs on each corner.
//!                A smaller radius results in sharper edges.
//! \param
//!     r        - R component of the button's color
//! \param
//!     g        - G component of the button's color
//! \param
//!     b        - B component of the button's color
//! \param
//!     a        - The opacity of the button
//! 
//! \return
//!     Surface of the translucent button with no text
//!
SDL_Surface* T4K_CreateButton( int    w,
                               int    h,
                               int    radius,
                               Uint8  r,
                               Uint8  g,
                               Uint8  b,
                               Uint8  a 
                              );

//==============================================================================
//
//  T4K_RoundCorners
//
//! \brief
//!     Round the corners of a surface by erasing edge pixels.
//! 
//! \param
//!    s        - The surface to process.
//! \param
//!    radius   - The radius of the arcs on each corner. 
//!               A smaller radius results in sharper edges.
//!
//! \return
//!    None
//!
void T4K_RoundCorners( SDL_Surface* s,
                       Uint16       radius
                     );

//==============================================================================
//
//  T4K_Flip
//
//! \brief
//!     Flip a surface vertically or horizontally.
//! 
//! \param
//!     in        - The source surface
//! \param
//!     x         - if nonzero, the image will be flipped horizontally
//! \param
//!     y         - if nonzero, the image will be flipped vertically
//! 
//! \return
//!     Returns flipped surface.
//!
SDL_Surface* T4K_Flip( SDL_Surface* in,
                       int          x,
                       int          y
                     );

//==============================================================================
//
//  T4K_Blend
//
//! \brief
//!     Blend two surfaces together. The third argument is between 0.0 and
//!     1.0, and represents the weight assigned to the first surface.  If
//!     the pointer to the second surface is NULL, this performs fading.
//!
//!     Currently this works only with RGBA images, but this is largely to
//!     make the (fast) pointer arithmetic work out; it could be easily
//!     generalized to other image types.
//! 
//! \param
//!     S1       - The first surface
//! \param
//!     S2       - The second surface
//! \param 
//!    gamma    - A value between 0.0 and 1.0, representing the weight 
//!                assigned to the first surface.
//! 
//! \return
//!    If successfull, it will returns the new blended surface 
//!    otherwise it will return a NULL value. 
//!
SDL_Surface* T4K_Blend( SDL_Surface* S1,
                        SDL_Surface* S2,
                        float        gamma
                      );

//==============================================================================
//
//  T4K_FreeSurfaceArray
//
//! \brief
//!     Free every surface in the array together with the array itself.
//! 
//! \param
//!     surfs        - An array of SDL_Surface pointers to free.
//! \param 
//!     length       - The size of the array.
//!
//! \return
//!     None
//!
void T4K_FreeSurfaceArray( SDL_Surface** surfs, 
                           int           length
                         );

//==============================================================================
// 
//  T4K_inRect
//
//! \brief
//!     Tells whether the point (x, y) is inside the SDL_Rect r.
//! 
//! \param
//!     r        - The bounding rect.
//! \param 
//!     x        - The x coordinate to test.
//! \param 
//!     y        - The y coordinate to test.
//! 
//! \return 
//!     1        - The x and y are inside of the SDL_Rect r.
//! \return
//!     0        - The x and y are outside of the SDL_Rect r.
//!
int T4K_inRect( SDL_Rect r,
                int      x,
                int      y
              );

//==============================================================================
// 
//  T4K_SetRect
//
//! \brief
//!     This function will write an SDL_Rect with dimensions based 
//!     on screen dimensions.
//! 
//! \param
//!     rect        - A pointer to the rect to fill.
//! \param
//!     pos         - Four floats between 0.0 and 1.0 which specify 
//!                   the desired x, y, w and h as a percentage of 
//!                   screen dimensions.
//!
//! \return
//!     None
//!
void T4K_SetRect( SDL_Rect*    rect,
                  const float* pos
                );

//==============================================================================
// 
//  T4K_UpdateRect
//
//! \brief
//!     Wrap a call to SDL_UpdateRect.
//! 
//! \param 
//!     surf        - The surface to update.
//! \param 
//!     rect        - The dimension of the surface to update.
//!
//! \return
//!     None
//!
void T4K_UpdateRect( SDL_Surface* surf,
                     SDL_Rect*    rect
                   );

//==============================================================================
//
//  T4K_DarkenScreen
//
//! \brief
//!     Darkens the screen by a factor of 2^bits.
//! 
//! \param
//!     bits       - An exponent between 1 and 8. Realistically, 
//!                 1 and 2 are the only useful values.
//!
//! \return
//!     None
//!
void T4K_DarkenScreen( Uint8 bits );

//==============================================================================
//
//  T4K_ChangeWindowSize
//
//! \brief
//!     This function will change window size (unstable, works only 
//!     in windowed mode).
//! 
//! \param
//!     new_res_x        - The new width of the window.
//! \param
//!     new_res_y        - The new height of the window.
//! 
//! \return
//!     None
//!
void T4K_ChangeWindowSize( int new_res_x,
                           int new_res_y
                         );

//==============================================================================
// 
//  T4K_SwitchScreenMode
//
//! \brief
//!     Switch between fullscreen and windowed mode.
//!     Resolution switching callbacks are invoked.
//!
//! \param
//!     None
//!
//! \return 
//!     None
//!
void T4K_SwitchScreenMode( void );

//==============================================================================
//
//! A function to handle a resolution switch, which should take 
//! parameters for the new horizontal and vertical screen dimensions.
//!
typedef void (*ResSwitchCallback)(int resx, int resy);

//==============================================================================
// 
//  T4K_OnResolutionSwitch
//
//! \brief 
//!     Register a callback to reposition and redraw screen elements when
//!     the resolution is changed.
//!
//! \param 
//!     callback         - A function to be called when resolution changes.
//!
//! \return
//!     None
//!
void T4K_OnResolutionSwitch( ResSwitchCallback callback );

//==============================================================================
// 
//  T4K_WaitForEvent
//
//! \brief
//!     Block application until SDL receives an appropriate event.
//!     Use sparingly.
//!
//! \param
//!     events        - A single or OR'd combination of event masks.
//! 
//! \return 
//!     The event type received.
//!
SDL_EventType T4K_WaitForEvent( SDL_EventMask events );

//==============================================================================
//
//  T4K_zoom
//
//! \brief 
//!     This function will scale an existing surface.
//! 
//! \param
//!     src         - The original surface, which is left unscathed.
//! \param 
//!     new_w       - The width of the new surface.
//! \param 
//!     new_h       - The height of the new surface.
//!
//! \return 
//!     Will return a newly allocated SDL_Surface.
//!
SDL_Surface* T4K_zoom( SDL_Surface* src,
                       int          new_w,
                       int          new_h
                     );

//==============================================================================
// 
//  T4K_TransWipe
//
//! \brief 
//!     Perform a wipe from the current screen image to a new one.
//! 
//! \param 
//!     newbkg       - The new image to wipe.
//! \param 
//!     type         - The WipeStyle to use.
//! \param 
//!     segments     - The number of division of the screen.
//! \param 
//!     duration     - The length of the animation in milliseconds.
//! 
//! \return
//!     Return 0 if newbkg is NULL and new background surface width and height
//!     is not equal to screen width and height otherwise returns 1.
//!
int T4K_TransWipe( const SDL_Surface* newbkg,
                   WipeStyle          type,
                   int                segments,
                   int                duration
                 );

//==============================================================================
// 
//  T4K_InitBlitQueue
//
//! \brief
//!     Initialize the blit queue system. This must be called before
//!     T4K_ResetBlitQueue,
//!     T4K_AddRect,
//!     T4K_DrawSprite,
//!     T4K_DrawObject,
//!     T4K_EraseObject,
//!     T4K_EraseSprite or
//!     T4K_UpdateScreen
//! 
//! \param
//!     None
//!
//! \return
//!     None
//!
void T4K_InitBlitQueue( void );

//==============================================================================
//
//  T4K_ResetBlitQueue
// 
//! \brief 
//!     Just set the number of pending updates to zero.
//!
//! \param 
//!     None
//!
//! \return
//!     None
//!
void T4K_ResetBlitQueue( void );

//==============================================================================
// 
//  T4K_AddRect
//
//! \brief
//!     Don't actually blit a surface, but add a rect to be updated 
//!     next update.
//!
//! \param
//!    src        - The source dimension of the screen.  
//! \param 
//!    dst        - The destination dimension of the screen.
//!
//! \return
//!    1          - Add SDL_Rect successful.
//! \return
//!    0          - If invalid parameter and add SDL_Rect failed. 
//!
int T4K_AddRect( SDL_Rect* src,
                 SDL_Rect* dst
               );

//==============================================================================
//
//  T4K_DrawSprite
//
//! \brief
//!     This function will draw the sprite in the screen.
//! \param 
//!     gfx        - Holds the sprite object.
//! \param 
//!     x          - The x position to draw the sprite. 
//! \param 
//!     y          - The y position to draw the sprite. 
//! 
//! \return
//!     1          - Successful draw of the sprite.
//! \return
//!     0          - Failed to draw the sprite.
//!
int T4K_DrawSprite( sprite* gfx,
                    int     x,
                    int     y
                  );

//==============================================================================
//
//  T4K_DrawObject
//
//! \brief
//!     Draw an object at the specified location. No respect to clipping.
//!
//! \param
//!     surf        - Holds the surface to draw.
//! \param
//!     x           - The x position to draw the object. 
//! \param
//!     y           - The y position to draw the object. 
//! 
//! \return
//!     1           - Successful draw of the object.
//! \return
//!     0           - Failed to draw the object.
//!
int T4K_DrawObject( SDL_Surface* surf,
                    int          x,
                    int          y
                  );

//==============================================================================
//
//  T4K_UpdateScreen
//
//! \brief
//!     Update the screen and increment the frame counter.
//! 
//! \param
//!     frame        - Increments every update of the screen.
//!
//! \return
//!     None
//!
void T4K_UpdateScreen( int* frame );

//==============================================================================
//
//  T4K_EraseSprite
//
//! \brief
//!     Basically puts in an order to overdraw sprite 
//!     with corresponding rect of bkgd img.
//! 
//! \param
//!     img        - Sprite to delete.
//! \param
//!     curr_bkgd  - Surface of the background.
//! \param
//!     x          - x coordinate
//! \param
//!     y          - y coordinate
//! 
//! \return
//!     1          - Successful erase of the spriate.
//! \return
//!     0          - Failed to erase the sprite.
//!
int T4K_EraseSprite( sprite*      img,
                     SDL_Surface* curr_bkgd,
                     int          x,
                     int          y
                   );

//==============================================================================
//
//  T4K_EraseObject
//
//! \brief
//!     Erase an object from the screen.
//! 
//! \param
//!     surf        - Surface to delete
//! \param
//!     curr_bkgd   - Current background
//! \param
//!     x           - x coordinate
//! \param
//!     y           - y coordinate
//! 
//! \return
//!     1           - Successful operation
//! \return
//!     0           - Failed operation 
//!
int T4K_EraseObject( SDL_Surface* surf,
                     SDL_Surface* curr_bkgd,
                     int          x,
                     int          y
                   );

//==============================================================================
// 
//  T4K_SetFontName
//
//! \brief
//!     Set the "global" font name.
//! 
//! \param
//!     name        - Name of the font to set.
//! 
//! \return
//!     None
//!
void T4K_SetFontName( const char* name );

//==============================================================================
// 
//  T4K_AskFontName
// 
//! \brief
//!     Get the "global" font name.
//! 
//! \param
//!     None
//! 
//! \return 
//!     Returns the "global" font name.
//!
const char* T4K_AskFontName( void );

//==============================================================================
//
//  T4K_Setup_SDL_Text
//
//! \brief
//!     Initialize the backend (Pango or TTF) used for text-drawing functions.
//! 
//! \param
//!     None
//!
//! \return
//!     1        - Successful initialization fo the text-drawing backend.
//! \return
//!     0        - Failed initialization.
//!
int T4K_Setup_SDL_Text( void );

//==============================================================================
// 
//  T4K_Cleanup_SDL_Text
// 
//! \brief
//!     Shut down the backend used for text-drawing functions.
//! 
//! \param
//!     None
//!
//! \return
//!     None
//!
void T4K_Cleanup_SDL_Text( void );

//==============================================================================
// 
//  T4K_BlackOutline
// 
//! \brief
//!     T4K_BlackOutline() creates a surface containing text of the designated
//!     foreground color, surrounded by a black shadow, on a transparent
//!     background.  The appearance can be tuned by adjusting the number of
//!     background copies and the offset where the foreground text is
//!     finally written (see below).
//! 
//! \param 
//!     t       - The text to draw
//! \param 
//!     size    - the font size to use
//! \param
//!     c       - the fill color to use
//! 
//! \return
//!     Returns newly created surface.
//!
SDL_Surface* T4K_BlackOutline( const char* t,
                               int         size,
                               const SDL_Color*  c
                             );

//==============================================================================
// 
//  T4K_SimpleText
//
//! \brief
//!     Returns a non-outlined surface using either SDL_Pango or SDL_ttf.
//! 
//! \param 
//!     t        - String to draw.
//! \param
//!     size     - Font size
//! \param
//!     col      - Color
//! 
//! \return 
//!     A non-outlined surface using either SDL_Pango or SDL_ttf.
//!
SDL_Surface* T4K_SimpleText( const char*      t,
                             int              size,
                             const SDL_Color* col
                           );

//==============================================================================
//
//  T4K_SimpleTextWithOffset
//
//! \brief
//!     Same as T4K_SimpleText, but the text offset is also stored.
//! 
//! \param
//!     t             - String to draw.
//! \param 
//!     size          - Font size
//! \param 
//!     col           - Color
//! \param
//!     glyph_off     - Set the var in which to store the offset.
//! 
//! \return
//!     A non-outlined surface using either SDL_Pango or SDL_ttf. 
//!
SDL_Surface* T4K_SimpleTextWithOffset( const char*       t,
                                       int               size,
                                       const SDL_Color*  col,
                                       int*              glyph_offset
                                     );

//==============================================================================
//  T4K_CharsForWidth
//
//! \brief
//!     Calculate how long a string for a given fontsize will fit within a
//!     given pixel width.  The estimate is based on strings of 'x'.
//! 
//! \param
//!     fontsize      - Font size
//! \param 
//!     pixel_width   - Width in pixels of the desired text box
//! \return
//!     strlen() of the longest string of 'x' that fits 
//!
int T4K_CharsForWidth( int fontsize,
                       int pixel_width
                     );

//==============================================================================
//                  Public Definitions in t4k_loaders.c
//==============================================================================

//==============================================================================
// 
//  T4K_AddDataPrefix
// 
//! \brief
//!     Add a directory that should be searched when loading assets.
//! 
//! \param 
//!     path        - Directory path to set as prefix of the data.
//! 
//! \return
//!     None
//!
void T4K_AddDataPrefix( const char* path );

//==============================================================================
//
//  T4K_CheckFile
//
//! \brief
//!     Check whether a file exists.
//! 
//! \param
//!    file        - File to check.
//! 
//! \return 
//!    1           - The file is valid. 
//! \return
//!    0           - The file is not valid.
//!
int T4K_CheckFile( const char* file );

//==============================================================================
// 
//  T4K_RemoveSlash
//
//! \brief
//!     Remove a trailing slash from a file path.
//! 
//! \param
//!     path       - A path that may or may not end in a slash.
//! 
//! \return        
//!     A path that does not end in a slash.
//!
char* T4K_RemoveSlash( char *path );

//==============================================================================
// 
//  T4K_LoadImage
//
//! \brief
//!     Load an image without resizing it
//!
//! \param
//!     file_name       - File name of the image.
//! \param 
//!     mode            - Image mode.
//! 
//! \return
//!     If successful it will return the newly created surface
//!     otherwise it will return a NULL value.
//!
SDL_Surface* T4K_LoadImage( const char* file_name,
                            int         mode
                          );

//==============================================================================
// 
//  T4K_LoadScaledImage
//
//! \brief 
//!     Load an image and resize it to given dimensions.
//!     If width or height is negative no resizing is applied.
//!
//!     The loader backend is chosen depending on file extension,
//!     If an SVG file is not found, try to load its PNG equivalent
//!     (unless IMG_NO_PNG_FALLBACK is set in mode)
//!
//! \param 
//!     file_name        - File name of the image.
//! \param
//!     mode             - Image Mode
//! \param
//!     width            - Width size of the image.
//! \param
//!     height           - Height size of the image.
//! 
//! \return
//!     If successful it will return the newly created surface
//!     otherwise it will return a NULL value.
//!
SDL_Surface* T4K_LoadScaledImage( const char* file_name,
                                  int         mode,
                                  int         width,
                                  int         height
                                );

//==============================================================================
// 
//  T4K_LoadImageOfBoundingBox
//
//! \brief
//!     Same as LoadScaledImage but preserve image proportions
//!     and fit it into max_width x max_height rectangle.
//!
//!     NOTE: Returned surface is not necessarily max_width x max_height!
//! 
//! \param
//!     file_name        - File name of the image.
//! \param
//!     mode             - Image modes.
//! \param
//!     max_width        - Max size of width of image.
//! \param
//!     max_height       - Max size of height of image.
//! 
//! \return
//!     If successful it will return the newly created surface
//!     otherwise it will return a NULL value.
//!
SDL_Surface* T4K_LoadImageOfBoundingBox( const char* file_name,
                                         int         mode,
                                         int         max_width,
                                         int         max_height
                                       );

//=============================================================================
// 
//  T4K_LoadBkgd
//
//! \brief
//!     A wrapper for LoadImage() that optimizes
//!     the format of background image.
//! 
//! \param
//!     file_name        - File name of background image.
//! \param
//!     width            - Width of the image.
//! \param
//!     height           - Height of the image.
//! 
//! \return
//!     If successful it will return the newly created surface
//!     otherwise it will return a NULL value.
//!
SDL_Surface* T4K_LoadBkgd( const char* file_name,
	int         width,
	int         height
	);

//=============================================================================
//
//  T4K_LoadBothBkgds
//
//! \brief
//!     Load backgrounds for both fullscreen and windowed resolution.
//! 
//! \param
//!     file_name        - File name of background image.
//! \param
//!     fs_bkgd          - Fullscreen surface.
//! \param
//!     win_bkgd         - Windowed surface
//! 
//! \return
//!     1                - Successful loading of background.
//! \return
//!     0                - Failed loading of background.
//!
int T4K_LoadBothBkgds( const char*   file_name,
                       SDL_Surface** fs_bkgd,
                       SDL_Surface** win_bkgd
                     );

//==============================================================================
//
//  T4K_LoadSprite
//
//! \brief
//!     Load a multiple-frame sprite from disk. 
//!     This function loads an SVG sprite or multiple PNGs as needed.
//! 
//! \param
//!     name        - The filename of the sprite to load, 
//!                   <em>without</em> an extension.
//! \param
//!     mode        - Loader flags to use, semantics the same as LoadImage.
//! 
//! \return
//!     If successful it will return the loaded sprite, otherwise it will
//!     return a NULL value.
//!
sprite* T4K_LoadSprite( const char* name,
                        int         mode
                      );

//==============================================================================
//
//  T4K_LoadScaledSprite 
// 
//! \brief
//!     Load a multiple-frame sprite from disk and scale it to the 
//!     given dimensions. This function loads an SVG sprite or 
//!     multiple PNGs as needed.
//! 
//! \param
//!     name        - The filename of the sprite to load,
//!                   <em>without</em> an extension.
//! \param
//!     mode        - Loader flags to use, semantics the same as LoadImage.
//! \param 
//!     width       - Width of sprite.
//! \param 
//!     height      - Height of sprite.
//! 
//! \return
//!     If successful it will return the loaded sprite, otherwise it will
//!     return a NULL value.
//!
sprite* T4K_LoadScaledSprite( const char* name,
                              int         mode,
                              int         width,
                              int         height
                            );

//==============================================================================
// 
//  T4K_LoadSpriteOfBoundingBox
//
//! \brief
//!     Same as LoadScaledSprite but preserve image proportions
//!     and fit it into max_width x max_height rectangle.
//!     NOTE: Returned surface is not necessarily max_width x max_height !
//!
//! \param
//!     name        - The filename of the sprite to load, 
//!                   <em>without</em> an extension.
//! \param
//!     mode        - Loader flags to use, semantics the same as LoadImage.
//! \param
//!     max_width   - Max width of sprite.
//! \param 
//!     max_height  - Max height of sprite.
//! 
//! \return
//!     If successful it will return the loaded sprite, otherwise it will
//!     return a NULL value.
//!
sprite* T4K_LoadSpriteOfBoundingBox( const char* name,
                                     int         mode,
                                     int         max_width,
                                     int max_height
                                   );

//==============================================================================
// 
//  T4K_FlipSprite
// 
//! \brief
//!     Flip (reflect) a sprite over one or both axes.
//! 
//! \param
//!     in        - The original image
//! \param 
//!     X         - If nonzero, the image is flipped horizontally.
//! \param
//!     Y         - If nonzero, the image is flipped vertically.
//! 
//! \return
//!     A newly allocated transformed sprite.
//! 
//! \see 
//!     T4K_Flip
//!
sprite* T4K_FlipSprite( sprite* in,
                        int     X,
                        int     Y 
                      );

//==============================================================================
//
//  T4K_FreeSprite
//
//! \brief
//!     Free memory allocated for a loaded sprite.
//!
//! \param
//!     gfx        - The sprite to free.
//!
//! \return
//!     None
//!
void T4K_FreeSprite( sprite* gfx );

//==============================================================================
// 
//  T4K_NextFrame
//
//! \brief
//!     Advance a sprite's frame.
//! 
//! \param
//!     s        - The sprite to advance.
//!
//! \return
//!     None
//!
void T4K_NextFrame( sprite* s );

//==============================================================================
// 
//  T4K_LoadSound
//
//! \brief
//!     Load a sound/music patch from a file.
//! 
//! \param
//!     datafile        - File name of the sound data.
//! 
//! \return
//!     Returns new created sound effect. 
//!
Mix_Chunk* T4K_LoadSound( char* datafile );

//==============================================================================
// 
//  T4K_LoadMusic
//
//! \brief
//!     Load music from a datafile
//! 
//! \param
//!     datafile     - File name of the music date.
//! 
//! \return
//!     Returns new created music.
//!
Mix_Music* T4K_LoadMusic( char *datafile );


//==============================================================================
//                  Public Definitions from t4k_audio.c
//==============================================================================

const static int T4K_AUDIO_PLAY_ONCE    =  0;
const static int T4K_AUDIO_LOOP_FOREVER = -1;

//==============================================================================
// 
//  T4K_PlaySound
//
//! \brief
//!     Play sound once and then exit.
//! 
//! \param
//!     sound        - Sound effect to play.
//!
//! \return
//!     None
//!
void T4K_PlaySound( Mix_Chunk* sound );

//==============================================================================
// 
//  T4K_PlaySoundLoop
//
//! \brief
//!     Play sound "loops" times, -1 for infinite.
//! 
//! \param 
//!     sound        - Sound effect to play.
//! \param 
//!     loops        - Number of repeats to play the sound.
//!
//! \return
//!     None
//!
void T4K_PlaySoundLoop( Mix_Chunk* sound,
                        int        loops
                      );

//==============================================================================
// 
//  T4K_AudioHaltChannel
//
//! \brief
//!     Will stop the channel specified in channel. If -1 is passed as 
//!     the parameter to channel, all channels will be stopped.
//!
//! \param
//!     channel         - Parameter to stop.
//!
//! \return
//!     None
//!
void T4K_AudioHaltChannel( int channel );

//==============================================================================
//
//  T4K_AudioMusicLoad
//
//! \brief
//!     Attempts to load and play the music file.
//!
//! \param
//!     music_path        - Path of the music file.
//! \param 
//!     loops             - Number of loops.
//! 
//! \return
//!     None
//!
void T4K_AudioMusicLoad( char* music_path,
                         int   loops
                       );

//==============================================================================
// 
//  T4K_AudioMusicUnload
//
//! \brief
//!     Attempts to unload any music data that was
//!     loaded using the audioMusicLoad function.
//!
//! \param
//!     None
//! 
//! \return
//!     None
//! 
void T4K_AudioMusicUnload( void );

//==============================================================================
// 
//  T4K_IsPlayingMusic
//
//! \brief
//!     This function will check if a music is currently playing.
//! 
//! \param
//!     None
//!
//! \return
//!     true       - If a music is currently playing.
//! \return
//!     false      - If a music is not playing.
//!
bool T4K_IsPlayingMusic( void );

//==============================================================================
// 
//  T4K_AudioMusicPlay
//
//! \brief
//!     Attempts to play the passed music data, stopping current music if
//!     necessary.
//! 
//! \param
//!     musicData       - Data of music.
//! \param 
//!     loops           - The number of times to loop, or -1 forever
//!
//! \return
//!     None
//!
void T4K_AudioMusicPlay( Mix_Music* musicData,
                         int        loops
                       );

//==============================================================================
// 
//  T4K_AudioEnable
//
//! \brief
//!     Enable/disable sound
//! 
//! \param 
//!     enabled        - Set true if Audio is enabled or false to disable 
//!                      the audition.
//! 
//! \return
//!     None
//!
void T4K_AudioEnable( bool enabled );

//==============================================================================
//
//  T4K_AudioToggle
//
//! \brief
//!     Toggle sound enablement.
//!
//! \param
//!     None
//!
//! \return
//!     None
//!
void T4K_AudioToggle( void );


//=============================================================================
//                      Public Definitions for t4k_linewrap.c
//=============================================================================

//=============================================================================
//
//  T4K_LineWrap
//
//! \brief
//!     This function takes an input string (can be in essentially arbitrary
//!     encoding) and loads it into an array of strings, each corresponding
//!     to one line of output text.
//!    
//! \param
//!     input         - A null-terminated input string.
//! \param
//!     str_list      - A PRE_ALLOCATED array of characters pointers. This
//!                     must be at least of size str_list[max_lines][max_width]
//! \param
//!     width         - The desired number of characters per line. Note that
//!                     words with more characters than "width" are not
//!                     hypenated, so it's possible to get a line that is
//!                     longer than "width".
//! \param
//!     max_lines     - Memory-safety parameters for str_list.
//! \param
//!     max_width     - Memory-safety parameters for str_list.
//!
//! \return
//!     Linewrap returns the number of lines used to format the strings.
//!
int T4K_LineWrap( const char* input,
                  char        str_list[MAX_LINES][MAX_LINEWIDTH],
                  int         width,
                  int         max_lines,
                  int         max_width
                );

//=============================================================================
//
//  T4K_LineWrapInsBreaks
//
//! \brief
//!     This function takes an input string and inserts newline characters at
//!     places determined by the linebreak library, returning a single string.
//!    
//! \param
//!     input         - A null-terminated input string.
//! \para
//!     output        - Pre-allocated location to contain string with breaks
//!                     inserted. 
//! \param
//!     width         - The desired number of characters per line. Note that
//!                     words with more characters than "width" are not
//!                     hypenated, so it's possible to get a line that is
//!                     longer than "width".
//! \param
//!     max_lines     - Memory-safety parameters for str_list.
//! \param
//!     max_width     - Memory-safety parameters for str_list.
//!
//! \return
//!     Linewrap returns the number of lines used to format the strings.
//!
int T4K_LineWrapInsBreaks( const char* input,
                           char*       output,
                           int         width,
                           int         max_lines,
                           int         max_width
                         );

//=============================================================================
//
//  T4K_LineWrapList
//
//! /brief
//!     This function takes a NULL_terminated array of strings and performs
//!     translations and linewrapping, outputting another NULL-terminated
//!     array.
//!
//! /param
//!     input         - A null-terminated input string.
//! /param
//!     str_list      - A PRE_ALLOCATED array of characters pointers. This
//!                     must be at least of size str_list[max_lines][max_width]
//! /param
//!     width         - The desired number of characters per line. Note that
//!                     words with more characters than "width" are not
//!                     hypenated, so it's possible to get a line that is
//!                     longer than "width".
//! /param
//!     max_lines     - Memory-safety parameters for str_list.
//! /param
//!     max_width     - Memory-safety parameters for str_list.
//!
//! /return
//!     None
//!
void T4K_LineWrapList( const char input[MAX_LINES][MAX_LINEWIDTH],
                       char       str_list[MAX_LINES][MAX_LINEWIDTH],
                       int        width,
                       int        max_lines,
                       int        max_width
                     );


//=============================================================================
//                      Public Definitions for t4k_throttle.c
//=============================================================================


//=============================================================================
//
//  T4K_Throttle
//
//! /brief
//!     Use this simple function to keep a loop from eating all CPU.
//!     Here we use SDL_Delay() to wait to return until 'loop_msec'
//!     milliseconds after it returned the last time. Per SDL docs,
//!     the granularity is likely no better than 10 msec
//!
//! /param
//!     loop_msec     - The desired loop duration, in msec
//! /param
//!     last_t        - The valid location of a Uint32 where timing can be
//!                     stored between invocations of this function.
//!
//! /return
//!     None
//!
void T4K_Throttle( int     loop_msec,
                   Uint32* last_t
                 );


//=============================================================================
//                      Public Definitions for t4k_convert_utf.c
//=============================================================================


//=============================================================================
//
//  T4K_ConvertFromUTF8
//
//! /brief
//!     A simple wrapper for using GNU iconv() to convert UTF-8 text to wchar_t
//!     ("Unicode") text.
//!
//! /param
//!     wide_word     - ptr to wchar_t buffer to contain converted text.
//! /param
//!     UTF8_word     - ptr to UTF-8 string to be converted.
//! /param
//!     max_length    - generally the length of the output buffer to avoid
//!                     overflow.  It also must be no greater than the buffer
//!                     length used internally in this function (i.e. 
//!                     UTF_BUF_LENGTH, currently 1024).
//!
//! /return
//!     Returns the length of the wchar_t string generated, 0 on errors.
//!
int T4K_ConvertFromUTF8( wchar_t*    wide_word, 
                         const char* UTF8_word, 
                         int         max_length
                       );

//=============================================================================
//
//  T4K_ConvertToUTF8
//
//! /brief
//!     A simple wrapper for using GNU iconv() to convert wchar_t
//!     ("Unicode") text to UTF-8 text. 
//!
//! /param
//!     wide_word     - ptr to wchar_t string to be converted.
//! /param
//!     UTF8_word     - ptr to buffer to hold UTF-8 string after conversion.
//! /param
//!     max_length    - generally the length of the output buffer to avoid
//!                     overflow.  It also must be no greater than the buffer
//!                     length used internally in this function (i.e. 
//!                     UTF_BUF_LENGTH, currently 1024).
//!
//! /return
//!     Returns the length of the UTF-8 string generated, 0 on errors.
//!
int T4K_ConvertToUTF8( const wchar_t* wide_word, 
                       char*          UTF8_word, 
                       int            max_length
                     );


//=============================================================================
//                      Public Definitions for t4k_replacements.c
//=============================================================================

//==============================================================================
//
//  T4K_alphasort
//
//! \brief 
//!     Wrapper for alphasort function, using either platform's alphasort or our
//!     replacement in t4k_replacements if not provided by system.
//! 
//! \param
//!     const struct dirent** d1 - first arg to be compared
//!
//! \param
//!     const struct dirent** d2 - second arg to be compared
//!
//! \return 
//!     From man alphasort: "alphasort() and versionsort() functions return an
//!     integer less than, equal to, or greater than zero if the first argument
//!     is considered to be  respectively  less than, equal to, or greater than
//!     the second."
//!
int T4K_alphasort( const struct dirent** d1,
                   const struct dirent** d2
                 );

//==============================================================================
//
//  T4K_scandir
//
//! \brief 
//!     Wrapper for scandir function, using either platform's scandir or our
//!     replacement in t4k_replacements if not provided by system.
//! 
//!     From man scandir (arg names adapted):
//!     The scandir() function scans the directory dirname, calling sdfilter() 
//!     on each directory entry. Entries for which sdfilter() returns nonzero
//!     are stored in strings allocated via malloc(3), sorted using qsort(3)
//!     with the comparison function dcomp(), and collected in array namelist
//!     which is allocated via  malloc(3).  If  sdfilter  is NULL, all entries
//!     are selected.
//!
//!     The alphasort() and versionsort() functions can be used as the comparison
//!     function compar(). The former sorts directory entries using strcoll(3), 
//!     the latter using strverscmp(3) on the strings (*a)->d_name and (*b)->d_name.
//!
//! \param
//!     const char *dirname
//!
//! \param
//!     struct dirent*** namelist
//!
//! \param
//!     int (*sdfilter)(struct dirent *)
//!
//! \param
//!     int (*dcomp)(const void *, const void *)
//!
//! \return 
//!     The scandir() function returns the number of directory entries selected
//!     or -1 if an error occurs.
//!
int T4K_scandir( const char*      dirname,
                 struct dirent*** namelist,
                 int (*sdfilter)(struct dirent *),
                 int (*dcomp)(const void *, const void *)
               );

#endif /* TUX4KIDS_COMMON_H */

