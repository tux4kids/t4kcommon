/*
   t4k-menu.c

   Functions responsible for loading, parsing and displaying game menu.

   Copyright 2009, 2010.
Authors: Boleslaw Kulbabinski, Brendan Luchen
Project email: <tuxmath-devel@lists.sourceforge.net>
Project website: http://tux4kids.alioth.debian.org

t4k_menu.c is part of the t4k_common library.

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



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "t4k_globals.h"
#include "t4k_common.h"

#define MENU_DIR "menus"


/*
   representation of a menu tree node
   */

struct mNode {
    struct mNode* parent;

    char* title;
    char* desc;
    int font_size;

    char* icon_name;
    sprite* icon;

    SDL_Rect button_rect;
    SDL_Rect icon_rect;
    SDL_Rect text_rect;

    /* submenu_size = 0 if no submenu */
    int submenu_size;
    struct mNode** submenu;

    /* these fields are used only if submenu_size = 0 */
    int activity;
    int param;

    /* these fields are used only if submenu_size > 0 */
    bool show_title;
    int entries_per_screen;
    int first_entry;
};

typedef struct mNode MenuNode;


/*
   menu globals
   */

/* activities array is used to parse xml menu files */
static int n_of_activities;
static char** activities;

/* prefix that is added to sprite paths */
static char* data_prefix;

static Mix_Chunk* snd_click;
static Mix_Chunk* snd_hover;
static char* music_path;

/* font size used in current resolution */
//static int curr_font_size;

/* how font size should be calculated */
static MFStrategy font_strategy;

/* buffer size used when reading attributes or names */
static const int buf_size = 512;

/* maximum menu buttons per page */
static const int MAX_PAGE_SIZE = 8;

/* actions available while viewing the menu */
enum { NONE, CLICK, PAGEUP, PAGEDOWN, STOP_ESC, RESIZED };

/* menus is a global array when user can save up to 10 loaded menus.
   From outside this file we identify menu trees by their ids (indexes in this array)
   so as not to expose MenuNode struct to the user */
#define N_OF_MENUS 10
static MenuNode* menus[N_OF_MENUS];

/* stop button, left and right arrow positions do not
   depend on currently displayed menu */
SDL_Rect menu_rect, stop_rect, prev_rect, next_rect, menu_title_rect;
SDL_Surface *stop_button, *prev_arrow, *next_arrow, *prev_gray, *next_gray;
/* Surfaces for "tooltips" description panel: */
SDL_Surface *desc_prerendered = NULL, *desc_panel = NULL;


/*
   positioning constants
   */

/* TODO: maybe it is better to move these constants into a config file ? */
/* X Y W H */
const float menu_pos[4] = {0.38, 0.23, 0.55, 0.72};
const float stop_pos[4] = {0.94, 0.0, 0.06, 0.06};
const float prev_pos[4] = {0.87, 0.93, 0.06, 0.06};
const float next_pos[4] = {0.94, 0.93, 0.06, 0.06};
const float desc_panel_pos[4] = {0.05, 0.3, 0.3, 0.5};
const char* stop_path = "menu/stop.svg";
const char* prev_path = "menu/left.svg";
const char* next_path = "menu/right.svg";
const char* prev_gray_path = "menu/left_gray.svg";
const char* next_gray_path = "menu/right_gray.svg";
const float button_gap = 0.18, text_h_gap = 0.35, text_w_gap = 0.5, button_radius = 0.27;
const int min_font_size = 8, default_font_size = 20, max_font_size = 33;



/*
   local functions
   */

MenuNode*       create_empty_node();
char*           get_attribute_name(const char* token);
char*           get_attribute_value(const char* token);
void            free_menu(MenuNode* menu);

SDL_Surface**   render_buttons(MenuNode* menu, bool selected);
char*           find_title_length(MenuNode* menu, int* length);
char*           find_longest_text(MenuNode* menu, int* length);
int             find_longest_menu_page(MenuNode* menu);
void            set_font_size(bool uniform);
void            prerender_menu(MenuNode* menu);
int		min(int a, int b);
int		max(int a, int b);
void            prerender_panel();

/* Calculated estimate of chars per line fitting into desc_panel */
int desc_chars_per_line(int fontsize);

void set_font_size_explicitly(MenuNode* menu, int size);
void set_menu_font_size(MenuNode* menu);
int binsearch(int min_f, int max_f, const char* text);

/*
   functions initializing the menu module
   (they shoul be called before any other menu activity)
   */

void T4K_SetActivitiesList(int num, char** acts)
{
    n_of_activities = num;
    activities = acts;
}

void T4K_SetMenuSounds(char* mus_path, Mix_Chunk* click, Mix_Chunk* hover)
{
    snd_click = click;
    snd_hover = hover;
    music_path = mus_path;
}

/* prefix that is used whe loading menu sprites */
void T4K_SetMenuSpritePrefix(char* pref)
{
    data_prefix = pref;
}

/*
   functions responsible for parsing menu files
   and creating menu trees
   */

/* creates new MenuNode struct with all fields set to NULL (or 0) */
MenuNode* create_empty_node()
{
    MenuNode* new_node = malloc(sizeof(MenuNode));
    if (new_node == NULL)
    {
        DEBUGMSG(debug_menu, "malloc(): Can't allocate memory for a new menu node.\n");
        return NULL;
    }
    new_node->parent = NULL;
    new_node->title = NULL;
    new_node->desc = NULL;
    new_node->icon_name = NULL;
    new_node->icon = NULL;
    new_node->submenu_size = 0;
    new_node->submenu = NULL;
    new_node->activity = 0;
    new_node->param = 0;
    new_node->first_entry = 0;
    new_node->show_title = false;

    return new_node;
}

/* "Translate" a xmlNode into a MenuNode (recursively) */
/* NOTE: The activities system is now case insensitive when dealing with IDs */
MenuNode *menu_TranslateNode(xmlNode *node) {
    MenuNode *tnode = NULL;
    int i;
    
    if (node == NULL)
    {
        DEBUGMSG(debug_menu, "menu_TranslateNode(): NULL pointer, exiting !\n");
        return NULL;
    }

    if(node->type == XML_ELEMENT_NODE) {
	xmlAttr *current, *child;
	tnode = create_empty_node();

	for(current = node->properties; current; current = current->next) {
	    if(xmlStrcasecmp(current->name, (const xmlChar *)"title") == 0) {
		tnode->title = strdup((const char *)current->children->content);
	    } else if(xmlStrcasecmp(current->name, (const xmlChar *)"run") == 0) {
		if(strcmp((const char *)current->children->content, "RUN_MAIN_MENU") == 0)
		    tnode->activity = RUN_MAIN_MENU;
		else
		    for(i = 0; i < n_of_activities; i++)
			if(strcasecmp((const char *)current->children->content, activities[i]) == 0)
			    tnode->activity = i;
	    } else if(xmlStrcasecmp(current->name, (const xmlChar *)"param") == 0) {
		tnode->param = atoi((const char *)current->children->content);
	    } else if(xmlStrcasecmp(current->name, (const xmlChar *)"desc") == 0) {
		tnode->desc = strdup((const char *)current->children->content);
	    } else if(xmlStrcasecmp(current->name, (const xmlChar *)"sprite") == 0) {
		tnode->icon_name = strdup((const char *)current->children->content);
	    } else if(xmlStrcasecmp(current->name, (const xmlChar *)"entries") == 0) {
		/* Prevent memory leaks in case of multiple entries attibutes */
		if(tnode->submenu != NULL)
		    free(tnode->submenu);
		tnode->submenu_size = atoi((const char *)current->children->content);
		tnode->submenu = malloc(tnode->submenu_size * sizeof(MenuNode));
	    }
	}


	/* Now add child nodes. */
	if(xmlStrcasecmp(node->name, (const xmlChar *)"menu") == 0) {
	    i = 0;
	    for(child = node->children; child; child = child->next) {
		if(child->type == XML_ELEMENT_NODE) {
		    tnode->submenu[i++] = menu_TranslateNode(child);
		}
	    }
	}
    }

    return tnode;
}

MenuNode *menu_LoadFile(char *file) {
    xmlDoc *menu;
    xmlNode *root;

    if ((debug_menu_parser) & debug_status)
        menu = xmlReadFile(file, NULL, XML_PARSE_RECOVER);
    else
        menu = xmlReadFile(file, NULL, XML_PARSE_RECOVER | XML_PARSE_NOERROR | XML_PARSE_NOWARNING);
    
    if(menu == NULL) {
	DEBUGMSG(debug_menu_parser, "menu_LoadFile: Failed to parse and load file. (`%s`)\n", file);
	return NULL;
    }

    /* Get the root element. */
    root = xmlDocGetRootElement(menu);

    /* Now, translate the xmlDoc to a MenuNode and return it. */
    return menu_TranslateNode(root);
}

/* recursively free all non-NULL pointers in a menu tree */
void free_menu(MenuNode* menu)
{
    int i;

    DEBUGMSG(debug_menu, "entering free_menu()\n");
    if(menu != NULL)
    {
	if(menu->title != NULL)
	    free(menu->title);
	if(menu->desc != NULL)
	    free(menu->desc);
	if(menu->icon_name != NULL)
	    free(menu->icon_name);
	if(menu->icon != NULL)
	    T4K_FreeSprite(menu->icon);

	if(menu->submenu != NULL)
	{
	    for(i = 0; i < menu->submenu_size; i++)
		if(menu->submenu[i] != NULL)
		{
		    free_menu(menu->submenu[i]);
		    menu->submenu[i] = NULL;
		}
	    free(menu->submenu);
	}
	/* Note that we don't need to set the menu->foo items to NULL */
	/* because they are being deallocated here:                   */
	free(menu);
    }
    DEBUGMSG(debug_menu, "leaving free_menu()\n");
}

/* create a simple one-level menu.
   All given strings are copied */
void T4K_CreateOneLevelMenu(int index, int items, char** item_names, char** sprite_names, char* title, char* trailer)
{
    MenuNode* menu = create_empty_node();
    int i;

    if(title)
    {
	menu->title = strdup(title);
	menu->show_title = true;
    }
    menu->submenu_size = items + (trailer ? 1 : 0);
    menu->submenu = (MenuNode**) malloc(menu->submenu_size * sizeof(MenuNode*));
    for(i = 0; i < items; i++)
    {
	menu->submenu[i] = create_empty_node();
	menu->submenu[i]->title = strdup(item_names[i]);
	if(sprite_names && sprite_names[i])
	    menu->submenu[i]->icon_name = strdup(sprite_names[i]);
	menu->submenu[i]->activity = i;
    }

    if(trailer)
    {
	menu->submenu[items] = create_empty_node();
	menu->submenu[items]->title = strdup(trailer);
	menu->submenu[items]->activity = items;
    }

    menus[index] = menu;
}

/* load menu from given XML file and store its tree under given index
   in "menus" array */
void T4K_LoadMenu(int index, const char* file_name)
{
    const char* fn = NULL;
    char temp[T4K_PATH_MAX];

    if(file_name == NULL)
	return;

    if(menus[index])
    {
	free_menu(menus[index]);
	menus[index] = NULL;
    }

    snprintf(temp, T4K_PATH_MAX, MENU_DIR "/%s", file_name);
    fn = find_file(temp);
    DEBUGMSG(debug_loaders|debug_menu, "T4K_Loadmenu(): looking in %s\n", fn);

    menus[index] = menu_LoadFile((char *)fn);
}

/* free all loaded menu trees */
void T4K_UnloadMenus(void)
{
    int i;

    DEBUGMSG(debug_menu, "entering T4K_UnloadMenus()\n");

    if(stop_button)
    {
	SDL_FreeSurface(stop_button);
	stop_button = NULL;
    }

    if(prev_arrow)
    {
	SDL_FreeSurface(prev_arrow);
	prev_arrow = NULL;
    }

    if(next_arrow)
    {
	SDL_FreeSurface(next_arrow);
	next_arrow = NULL;
    }

    for(i = 0; i < N_OF_MENUS; i++)
	if(menus[i] != NULL)
	{
	    DEBUGMSG(debug_menu, "T4K_UnloadMenus(): freeing menu #%d\n", i);
	    free_menu(menus[i]);
	    menus[i] = NULL;
	}

    DEBUGMSG(debug_menu, "leaving T4K_UnloadMenus()\n");
}


/*
   RunMenu - main function to display the menu and run the event loop
   if return_choice = true then return chosen value instead of
   running handle_activity()
   this function is a modified copy of choose_menu_item()
   */
int T4K_RunMenu(int index, bool return_choice, void (*draw_background)(), int (*handle_event)(SDL_Event*), void (*handle_animations)(), int (*handle_activity)(int, int))
{
    SDL_Surface** menu_item_unselected = NULL;
    SDL_Surface** menu_item_selected = NULL;
    SDL_Surface* title_surf;
    SDL_Event event;
    MenuNode* menu = menus[index];
    MenuNode* tmp_node;

    SDL_Rect tmp_rect;
    sprite* tmp_sprite;
    int i;
    int stop = 0;
    int first_loop = 1;
    int items;
    int old_w, old_h;

    int action = NONE;

    Uint32 frame_start = 0;       //For keeping frame rate constant
    Uint32 frame_now = 0;
    Uint32 frame_counter = 0;
    //int loc = -1;                  //The currently selected menu item
    int loc = 0;                  //Start with focus on first item
    int old_loc = 0;
    int click_flag = 1;
    int using_scroll = 0;

    internal_res_switch_handler(&T4K_PrerenderAll);

    for(;;) /* one loop body execution for one menu page */
    {
	DEBUGMSG(debug_menu, "run_menu(): drawing whole new menu page\n");

	draw_background();

	/* render buttons for current menu page */
	menu_item_unselected = render_buttons(menu, false);
	menu_item_selected = render_buttons(menu, true);
	items = min(menu->entries_per_screen, menu->submenu_size - menu->first_entry);

	/* draw buttons */
	DEBUGMSG(debug_menu, "run_menu(): drawing %d buttons\n", items);
	for(i = 0; i < items; i++)
	{
	    if(loc == i)
		SDL_BlitSurface(menu_item_selected[i], NULL, T4K_GetScreen(), &menu->submenu[menu->first_entry + i]->button_rect);
	    else
		SDL_BlitSurface(menu_item_unselected[i], NULL, T4K_GetScreen(), &menu->submenu[menu->first_entry + i]->button_rect);
	    if(menu->submenu[menu->first_entry + i]->icon)
		SDL_BlitSurface(menu->submenu[menu->first_entry + i]->icon->default_img, NULL, T4K_GetScreen(), &menu->submenu[menu->first_entry + i]->icon_rect);
	}

	SDL_BlitSurface(stop_button, NULL, T4K_GetScreen(), &stop_rect);

	/* check if there is a need to draw menu arrows */
	if(menu->entries_per_screen < menu->submenu_size)
	{
	    /* display arrows */
	    if(menu->first_entry > 0)
		SDL_BlitSurface(prev_arrow, NULL, T4K_GetScreen(), &prev_rect);
	    else
		SDL_BlitSurface(prev_gray, NULL, T4K_GetScreen(), &prev_rect);
	    if(menu->first_entry + items < menu->submenu_size)
		SDL_BlitSurface(next_arrow, NULL, T4K_GetScreen(), &next_rect);
	    else
		SDL_BlitSurface(next_gray, NULL, T4K_GetScreen(), &next_rect);
	}

	/* display red menu title (if present) */
	if(menu->show_title)
	{
	    menu_title_rect = menu->submenu[0]->button_rect;
	    menu_title_rect.y = menu_rect.y - menu_title_rect.h;
	    title_surf = T4K_BlackOutline(_(menu->title), menu->font_size, &red);
	    SDL_BlitSurface(title_surf, NULL, T4K_GetScreen(), &menu_title_rect);
	    SDL_FreeSurface(title_surf);
	}

	prerender_panel();

	SDL_UpdateRect(T4K_GetScreen(), 0, 0, 0, 0);

	SDL_WM_GrabInput(SDL_GRAB_OFF);

	while (SDL_PollEvent(&event));  // clear pending events

	/******** Main loop: *********/
	stop = false;
	DEBUGMSG(debug_menu, "run_menu(): entering menu loop\n");
	while (!stop)
	{
	    frame_start = SDL_GetTicks();         /* For keeping frame rate constant.*/

	    action = NONE;
	    while (!stop && (SDL_PollEvent(&event) || first_loop))
	    {
		switch (event.type)
		{
		    /* user decided to quit the application (for example by closing the window) */
		    case SDL_QUIT:
			{
			    T4K_FreeSurfaceArray(menu_item_unselected, items);
			    T4K_FreeSurfaceArray(menu_item_selected, items);
			    if(desc_panel != NULL)
				SDL_FreeSurface(desc_panel);
			    return QUIT;
			}

		    case SDL_MOUSEMOTION:
			{
			    if(!using_scroll)
				loc = -1;
			    for (i = 0; i < items; i++)
			    {
				if (T4K_inRect(menu->submenu[menu->first_entry + i]->button_rect, event.motion.x, event.motion.y))
				{
				    if(old_loc != i && snd_hover)
					T4K_PlaySound(snd_hover);
				    loc = i;
				    break;   /* from for loop */
				}
			    }

			    /* "Left" button - make click if button active: */
			    if(T4K_inRect(prev_rect, event.motion.x, event.motion.y)
				    && menu->first_entry > 0)
			    {
				if(click_flag)
				{
				    if(snd_hover)
					T4K_PlaySound(snd_hover);
				    click_flag = 0;
				}
			    }

			    /* "Right" button - make click if button active: */
			    else if(T4K_inRect(next_rect, event.motion.x, event.motion.y)
				    && menu->first_entry + items < menu->submenu_size)
			    {
				if(click_flag)
				{
				    if(snd_hover)
					T4K_PlaySound(snd_hover);
				    click_flag = 0;
				}
			    }

			    /* "stop" button */
			    else if (T4K_inRect(stop_rect, event.motion.x, event.motion.y ))
			    {
				if(click_flag)
				{
				    if(snd_hover)
					T4K_PlaySound(snd_hover);
				    click_flag = 0;
				}
			    }

			    else  // Mouse outside of arrow rects - re-enable click sound:
				click_flag = 1;

			    break;
			}

		    case SDL_MOUSEBUTTONDOWN:
			{
			    /* Handle scroll events: */
			    if(event.button.button == SDL_BUTTON_WHEELUP)
			    {
				using_scroll = 1;
				if(snd_hover)
				    T4K_PlaySound(snd_hover);
				if (loc > 0)
				    loc--;
				else if (menu->submenu_size <= menu->entries_per_screen) 
				    loc = menu->submenu_size - 1;  // wrap around if only 1 T4K_GetScreen()
				else if (menu->first_entry > 0)
				{
				    loc = menu->entries_per_screen - 1;
				    action = PAGEUP;
				}
				break;
			    }

			    else if(event.button.button == SDL_BUTTON_WHEELDOWN)
			    {
				using_scroll = 1;
				if(snd_hover)
				    T4K_PlaySound(snd_hover);
				if (loc + 1 < min(menu->submenu_size, menu->entries_per_screen))
				    loc++;
				else if (menu->submenu_size <= menu->entries_per_screen) 
				    loc = 0;  // wrap around if only 1 T4K_GetScreen()
				else if (menu->first_entry + menu->entries_per_screen < menu->submenu_size)
				{
				    loc = 0;
				    action = PAGEDOWN;
				}
				break;
			    }
			    else for (i = 0; i < items; i++) //Handle non-scroll events within button rects
			    {
				if (T4K_inRect(menu->submenu[menu->first_entry + i]->button_rect, event.motion.x, event.motion.y))
				{
				    // Play sound if loc is being changed:
				    if(snd_click)
					T4K_PlaySound(snd_click);
				    loc = i;
				    action = CLICK;
				    using_scroll = 0;
				    break;   /* from for loop */
				}
			    }


			    /* "Left" button */
			    if (T4K_inRect(prev_rect, event.motion.x, event.motion.y)
				    && menu->first_entry > 0)
			    {
				if(snd_click)
				    T4K_PlaySound(snd_click);
				action = PAGEUP;
				using_scroll = 0;
			    }

			    /* "Right" button - go to next page: */
			    else if (T4K_inRect(next_rect, event.motion.x, event.motion.y )
				    && menu->first_entry + items < menu->submenu_size)
			    {
				if(snd_click)
				    T4K_PlaySound(snd_click);
				action = PAGEDOWN;
				using_scroll = 0;
			    }

			    /* "Stop" button - go to main menu: */
			    else if (T4K_inRect(stop_rect, event.button.x, event.button.y ))
			    {
				if(snd_click)
				    T4K_PlaySound(snd_click);
				action = STOP_ESC;
			    }
			    else if(loc != -1) //If focus is set from scrolling, use click to activate
			    {
				if(snd_click)
				    T4K_PlaySound(snd_click);
				action = CLICK;
			    }

			    break;
			} /* End of case SDL_MOUSEDOWN */

		    case SDL_KEYDOWN:
			{
			    using_scroll = 0;
			    /* Proceed according to particular key pressed: */
			    switch (event.key.keysym.sym)
			    {
				case SDLK_ESCAPE:
				    {
					action = STOP_ESC;
					break;
				    }

				case SDLK_RETURN:
				case SDLK_SPACE:
				case SDLK_KP_ENTER:
				    {
					if(snd_click)
					    T4K_PlaySound(snd_click);
					action = CLICK;
					break;
				    }

				    /* Go to previous page, if present: */
				case SDLK_LEFT:
				case SDLK_PAGEUP:
				case SDLK_h:   //(Vim-like, see also below.)
				    {
					if(snd_click)
					    T4K_PlaySound(snd_click);
					if (menu->first_entry > 0)
					    action = PAGEUP;
					break;
				    }

				    /* Go to next page, if present: */
				case SDLK_RIGHT:
				case SDLK_PAGEDOWN:
				case SDLK_l:
				    {
					if(snd_click)
					    T4K_PlaySound(snd_click);
					if (menu->first_entry + items < menu->submenu_size)
					    action = PAGEDOWN;
					break;
				    }

				    /* Go up one entry, if present: */
				case SDLK_UP:
				case SDLK_k:    // For grade-school Vim users
				    {
					if(snd_hover)
					    T4K_PlaySound(snd_hover);
					if (loc > 0)
					    loc--;
					else if (menu->submenu_size <= menu->entries_per_screen) 
					    loc = menu->submenu_size - 1;  // wrap around if only 1 T4K_GetScreen()
					else if (menu->first_entry > 0)
					{
					    loc = menu->entries_per_screen - 1;
					    action = PAGEUP;
					}
					break;
				    }

				case SDLK_DOWN:
				case SDLK_j:    // For grade-school Vim users
				    {
					if(snd_hover)
					    T4K_PlaySound(snd_hover);
					if (loc + 1 < min(menu->submenu_size, menu->entries_per_screen) && loc+1 < menu->submenu_size-menu->first_entry)
					    loc++;
					else if (menu->submenu_size <= menu->entries_per_screen) 
					    loc = 0;  // wrap around if only 1 T4K_GetScreen()
					else if (menu->first_entry + menu->entries_per_screen < menu->submenu_size)
					{
					    loc = 0;
					    action = PAGEDOWN;
					}
					break;
				    }

				case SDLK_TAB:
				    {
					/* See if [Shift] pressed to decide if we go up or down: */
					if(event.key.keysym.mod & KMOD_SHIFT) //go up
					{
					    if(snd_hover)
						T4K_PlaySound(snd_hover);
					    if (loc > 0)
						loc--;
					    else if (menu->submenu_size <= menu->entries_per_screen) 
						loc = menu->submenu_size - 1;  // wrap around if only 1 T4K_GetScreen()
					    else if (menu->first_entry > 0)
					    {
						loc = menu->entries_per_screen - 1;
						action = PAGEUP;
					    }
					    break;
					}
					else  //[Shift] not pressed so go down:
					{
					    if(snd_hover)
						T4K_PlaySound(snd_hover);
					    if (loc + 1 < min(menu->submenu_size, menu->entries_per_screen))
						loc++;
					    else if (menu->submenu_size <= menu->entries_per_screen) 
						loc = 0;  // wrap around if only 1 T4K_GetScreen()
					    else if (menu->first_entry + menu->entries_per_screen < menu->submenu_size)
					    {
						loc = 0;
						action = PAGEDOWN;
					    }
					    break;
					}
				    }


				    //              /* Toggle T4K_GetScreen() mode: */
				    //              case SDLK_F10:
				    //              {
				    //                T4K_SwitchScreenMode();
				    //                action = RESIZED;
				    //                break;
				    //              }
				    //
				    //              /* Toggle menu music: */
				    //              case SDLK_F11:
				    //              {
				    //                if(T4K_IsPlayingMusic())
				    //                {
				    //                  T4K_AudioMusicUnload();
				    //                }
				    //                else if(music_path)
				    //                {
				    //                  T4K_AudioMusicUnload(music_path, -1);
				    //                }
				    //                break;
				    //              }

				default:
				    {
					/* Some other key - do nothing. */
				    }

				    break;  /* To get out of _outer_ switch/case statement */
			    }  /* End of key switch statement */
			}  // End of case SDL_KEYDOWN in outer switch statement
		}  // End event switch statement

		/* check if catched event causes any changes to titlescreen,
		   if handle_event() returns 1, menu should be redrawn */
		if(handle_event(&event))
		    stop = true;
		if (T4K_HandleStdEvents(&event) )
		    stop = true;

		/* handle button focus */
		if (old_loc != loc || first_loop || event.key.keysym.sym == SDLK_F10) {
		    DEBUGMSG(debug_menu, "run_menu(): changed button focus, old=%d, new=%d\n", old_loc, loc);

		    first_loop = 0;

		    int key = event.key.keysym.sym;

		    if (key == SDLK_F10) {
			T4K_PrerenderAll();	 // Important when the screen is being RESIZED
			prerender_panel();   // To adjust the description panel size with new resolution
			if (loc == -1 && old_loc == -1)
			    loc = 0;
			else if (loc == -1)
			    loc = old_loc;     // To ensure a valid value for loc
		    }

		    if(old_loc >= 0 && old_loc < items)
		    {
			tmp_rect = menu->submenu[old_loc + menu->first_entry]->button_rect;
			SDL_BlitSurface(menu_item_unselected[old_loc], NULL, T4K_GetScreen(), &tmp_rect);
			if(menu->submenu[menu->first_entry + old_loc]->icon)
			    SDL_BlitSurface(menu->submenu[menu->first_entry + old_loc]->icon->default_img,
				    NULL, T4K_GetScreen(), &menu->submenu[menu->first_entry + old_loc]->icon_rect);
			SDL_UpdateRect(T4K_GetScreen(), tmp_rect.x, tmp_rect.y, tmp_rect.w, tmp_rect.h);
		    }

		    /* Announce the menu item if index is not out of bonds */
		    if(loc + menu->first_entry >= 0 && loc <= items)
		    {

				if (menu->submenu[loc + menu->first_entry]->desc == NULL)
					T4K_Tts_say(DEFAULT_VALUE,DEFAULT_VALUE,INTERRUPT,"%s",
					_(menu->submenu[loc + menu->first_entry]->title));
				else
					T4K_Tts_say(DEFAULT_VALUE,DEFAULT_VALUE,INTERRUPT,"%s. %s",
					_(menu->submenu[loc + menu->first_entry]->title),_(menu->submenu[loc + menu->first_entry]->desc));

			}

		    if(loc >= 0 && loc < items)
		    {
			tmp_rect = menu->submenu[loc + menu->first_entry]->button_rect;
			SDL_BlitSurface(menu_item_selected[loc], NULL, T4K_GetScreen(), &tmp_rect);
			if(menu->submenu[menu->first_entry + loc]->icon)
			{
			    SDL_BlitSurface(menu->submenu[menu->first_entry + loc]->icon->default_img,
				    NULL, T4K_GetScreen(), &menu->submenu[menu->first_entry + loc]->icon_rect);
			    menu->submenu[menu->first_entry + loc]->icon->cur = 0;
			}
			SDL_UpdateRect(T4K_GetScreen(), tmp_rect.x, tmp_rect.y, tmp_rect.w, tmp_rect.h);

			// Set and render new description text
			{
			    char *desc = _(menu->submenu[loc + menu->first_entry]->desc);
			    char out[256];
			    int char_width;
			    // Clear old rendered text:
			    SDL_FreeSurface(desc_prerendered);
			    desc_prerendered = NULL;
			    if(desc == NULL)
				desc = "";
			    char_width = desc_chars_per_line(T4K_TOOLTIP_FONTSIZE);
			    T4K_LineWrapInsBreaks(desc, out, char_width, 64, 64);
			    //        desc_prerendered = T4K_SimpleText(out, T4K_TOOLTIP_FONTSIZE, &yellow);
			    if (strcmp(desc, "") != 0)
				desc_prerendered = T4K_BlackOutline(out, T4K_TOOLTIP_FONTSIZE, &yellow);
			}

			if(desc_prerendered) {
			    SDL_Rect pos = {T4K_GetScreen()->w * desc_panel_pos[0], T4K_GetScreen()->h * desc_panel_pos[1]};
			    SDL_BlitSurface(desc_panel, NULL, T4K_GetScreen(), &pos);
			    SDL_BlitSurface(desc_prerendered, NULL, T4K_GetScreen(), &pos);
			    SDL_Flip(T4K_GetScreen());
			}

		    }
		    old_loc = loc;
		}

		/* handle special action that was caused by an event */
		switch(action)
		{
		    //          case RESIZED:
		    //            menu->first_entry = 0;
		    //            T4K_PrerenderAll();
		    //            stop = true;
		    //            break;

		    case CLICK:
			if(loc < 0 || loc >= items)
			{
			    DEBUGMSG(debug_menu, "run_menu(): incorrect location for CLICK action (%d) !\n", loc);
			}
			else
			{
			    tmp_node = menu->submenu[menu->first_entry + loc];
			    if(tmp_node->submenu_size == 0)
			    {
				if(return_choice)
				{
				    /* return choice instead of running a handler function */
				    T4K_FreeSurfaceArray(menu_item_unselected, items);
				    T4K_FreeSurfaceArray(menu_item_selected, items);
				    return tmp_node->activity;
				}
				else
				{
				    if(tmp_node->activity == RUN_MAIN_MENU)
				    {
					/* go back to the root of this menu */
					menu = menus[index];
					loc = 0;
				    }
				    else
				    {
					/* we are going to run a handler function that probably will run a game,
					   save current screen resolution in case it is changed while running a game */
					old_w = T4K_GetScreen()->w;
					old_h = T4K_GetScreen()->h;
					internal_res_switch_handler(NULL);
					if(handle_activity(tmp_node->activity, tmp_node->param) == QUIT)
					{
					    /* user decided to quit while playing a game */
					    DEBUGMSG(debug_menu, "run_menu(): handle_activity() returned QUIT message, exiting.\n");
					    T4K_FreeSurfaceArray(menu_item_unselected, items);
					    T4K_FreeSurfaceArray(menu_item_selected, items);
					    return QUIT;
					}
					if(old_w != T4K_GetScreen()->w || old_h != T4K_GetScreen()->h)
					    T4K_PrerenderAll(); /* resolution has changed */
				    }
				}
			    }
			    else
			    {
				menu->first_entry = 0;
				menu = tmp_node;
				menu->first_entry = 0;
				loc = 0;
			    }
			    stop = true;
			}
			break;

		    case STOP_ESC:
			if(menu->parent == NULL)
			{
			    T4K_FreeSurfaceArray(menu_item_unselected, items);
			    T4K_FreeSurfaceArray(menu_item_selected, items);
			    return STOP;
			}
			else
			{
			    menu = menu->parent;
			    loc = 0;
			}
			stop = true;
			break;

		    case PAGEUP:
			menu->first_entry -= menu->entries_per_screen;
			loc = 0;
			stop = true;
			break;

		    case PAGEDOWN:
			menu->first_entry += menu->entries_per_screen;
			loc = 0;
			stop = true;
			break;
		}

	    }  // End of SDL_PollEvent while loop

		if(stop)
		{
			/* Set and render new description text when menu is scrolled - Nalin */
			if(action == PAGEUP || action == PAGEDOWN)
			{
				char *desc = _(menu->submenu[loc + menu->first_entry]->desc);
				char out[256];
				int char_width;
				// Clear old rendered text:
				SDL_FreeSurface(desc_prerendered);
				desc_prerendered = NULL;
				if(desc == NULL)
				desc = "";
				char_width = desc_chars_per_line(T4K_TOOLTIP_FONTSIZE);
				T4K_LineWrapInsBreaks(desc, out, char_width, 64, 64);
				//        desc_prerendered = T4K_SimpleText(out, T4K_TOOLTIP_FONTSIZE, &yellow);
				if (strcmp(desc, "") != 0)
				desc_prerendered = T4K_BlackOutline(out, T4K_TOOLTIP_FONTSIZE, &yellow);
			}

			/* Announce the item again when page is scrolled or mouse hover*/
		    if(loc >= 0 && loc < items)
            {
    			if (menu->submenu[loc + menu->first_entry]->desc == NULL)
	    			T4K_Tts_say(DEFAULT_VALUE,DEFAULT_VALUE,INTERRUPT,"%s",
		    		_(menu->submenu[loc + menu->first_entry]->title));
			    else
				    T4K_Tts_say(DEFAULT_VALUE,DEFAULT_VALUE,INTERRUPT,"%s. %s",
    				_(menu->submenu[loc + menu->first_entry]->title),_(menu->submenu[loc + menu->first_entry]->desc));
            }
			/* whole menu will be redrawn so there is no need to draw anything now */
			break;
		}

	    /* handle icon animation of selected button */
	    if(!stop && frame_counter % 5 == 0 && loc >= 0 && loc < items)
	    {
		tmp_sprite = menu->submenu[menu->first_entry + loc]->icon;
		if(tmp_sprite)
		{
		    SDL_BlitSurface(menu_item_selected[loc], NULL, T4K_GetScreen(), &menu->submenu[menu->first_entry + loc]->icon_rect);
		    SDL_BlitSurface(tmp_sprite->frame[tmp_sprite->cur], NULL, T4K_GetScreen(), &menu->submenu[menu->first_entry + loc]->icon_rect);
		    T4K_UpdateRect(T4K_GetScreen(), &menu->submenu[menu->first_entry + loc]->icon_rect);
		    T4K_NextFrame(tmp_sprite);
		}
	    }

	    /* handle titlescreen animations */
	    handle_animations();

	    if(desc_prerendered) {
		SDL_Rect pos = {T4K_GetScreen()->w * desc_panel_pos[0], T4K_GetScreen()->h * desc_panel_pos[1]};
		SDL_BlitSurface(desc_panel, NULL, T4K_GetScreen(), &pos);
		SDL_BlitSurface(desc_prerendered, NULL, T4K_GetScreen(), &pos);
		SDL_Flip(T4K_GetScreen());
	    }

	    /* Wait so we keep frame rate constant: */
	    frame_now = SDL_GetTicks();
	    if (frame_now < frame_start)
		frame_start = frame_now;  // in case the timer wraps around
	    if((frame_now - frame_start) < 1000 / MAX_FPS)
		SDL_Delay(1000 / MAX_FPS - (frame_now - frame_start));

	    frame_counter++;
	} // End of while(!stop) loop

	/* free button surfaces */
	DEBUGMSG(debug_menu, "run_menu(): freeing %d button surfaces\n", items);

	T4K_FreeSurfaceArray(menu_item_unselected, items);
	T4K_FreeSurfaceArray(menu_item_selected, items);
    }

    return QUIT;
}

/*
   functins responsible for rendering menus
   */


/* prerender the description panel */
void prerender_panel() {

    SDL_Rect panelclip = {T4K_GetScreen()->w * desc_panel_pos[0],
	T4K_GetScreen()->h * desc_panel_pos[1],
	T4K_GetScreen()->w * desc_panel_pos[2],
	T4K_GetScreen()->h * desc_panel_pos[3]};
    if(desc_panel != NULL)
	SDL_FreeSurface(desc_panel);
    desc_panel = T4K_CreateButton(panelclip.w - panelclip.x, panelclip.h - panelclip.y, 8, 0xff, 0xff, 0xff, 100);
    SDL_BlitSurface(desc_panel, NULL, T4K_GetScreen(), &panelclip);
    SDL_BlitSurface(T4K_GetScreen(), &panelclip, desc_panel, NULL);
}

/* return button surfaces that are currently displayed (without sprites) */
SDL_Surface** render_buttons(MenuNode* menu, bool selected)
{
    SDL_Surface** menu_items = NULL;
    SDL_Rect curr_rect;
    SDL_Surface* tmp_surf = NULL;
    int i;
    int items = min(menu->entries_per_screen, menu->submenu_size - menu->first_entry);

    menu_items = (SDL_Surface**) malloc(items * sizeof(SDL_Surface*));
    if(NULL == menu_items)
    {
	DEBUGMSG(debug_menu, "render_buttons(): failed to allocate memory for buttons!\n");
	return NULL;  // error
    }

    for (i = 0; i < items; i++)
    {
	curr_rect = menu->submenu[menu->first_entry + i]->button_rect;
	menu_items[i] = SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_SRCALPHA,
		curr_rect.w,
		curr_rect.h,
		32,
		rmask, gmask, bmask, amask);

	SDL_BlitSurface(T4K_GetScreen(), &curr_rect, menu_items[i], NULL);
	/* button */
	if(selected)
	    tmp_surf = T4K_CreateButton(curr_rect.w, curr_rect.h, button_radius * curr_rect.h, SEL_RGBA);
	else
	    tmp_surf = T4K_CreateButton(curr_rect.w, curr_rect.h, button_radius * curr_rect.h, REG_RGBA);

	SDL_BlitSurface(tmp_surf, NULL, menu_items[i], NULL);
	SDL_FreeSurface(tmp_surf);

	/* text */
	tmp_surf = T4K_BlackOutline(_(menu->submenu[menu->first_entry + i]->title),
		menu->font_size, selected ? &yellow : &white);
	SDL_BlitSurface(tmp_surf, NULL, menu_items[i], &menu->submenu[menu->first_entry + i]->text_rect);
	SDL_FreeSurface(tmp_surf);
    }

    return menu_items;
}

/* recursively load sprites and calculate button rects
   to fit into current screen */
void prerender_menu(MenuNode* menu)
{
    SDL_Surface* temp_surf;
    MenuNode* curr_node;
    int i, imod, max_text_h = 0, max_text_w = 0;
    int button_h, button_w;
    bool found_icons = false;
    char filename[buf_size];

    DEBUGMSG(debug_menu, "Entering prerender_menu()\n");
    if(NULL == menu)
    {
	DEBUGMSG(debug_menu, "prerender_menu(): NULL pointer, exiting !\n");
	return;
    }

    if(0 == menu->submenu_size)
    {
	DEBUGMSG(debug_menu, "prerender_menu(): no submenu, exiting.\n");
	return;
    }

    for(i = 0; i < menu->submenu_size; i++)
    {
	if(menu->submenu[i]->icon_name)
	    found_icons = true;
	temp_surf = NULL;

	temp_surf = T4K_SimpleText(_(menu->submenu[i]->title), menu->font_size, &black);
	if(temp_surf)
	{
	    max_text_h = max(max_text_h, temp_surf->h);
	    max_text_w = max(max_text_w, temp_surf->w);
	    SDL_FreeSurface(temp_surf);
	}
    }

    button_h = (1.0 + 2.0 * text_h_gap) * max_text_h;
    button_w = max_text_w + ( (found_icons ? 1.0 : 0.0) + 2.0 * text_w_gap) * button_h;

    menu->entries_per_screen = (int) ( (menu_rect.h - button_gap * button_h) /
	    ( (1.0 + button_gap) * button_h ) );

    for(i = 0; i < menu->submenu_size; i++)
    {
	curr_node = menu->submenu[i];
	curr_node->button_rect.x = menu_rect.x;
	imod = i % menu->entries_per_screen;
	curr_node->button_rect.y = menu_rect.y + imod * button_h + (imod + 1) * button_gap * button_h;
	curr_node->button_rect.w = button_w;
	curr_node->button_rect.h = button_h;

	curr_node->icon_rect = curr_node->button_rect;
	curr_node->icon_rect.w = curr_node->icon_rect.h;

	curr_node->text_rect.x = ( (found_icons ? 1.0 : 0.0) + text_w_gap) * curr_node->icon_rect.w;
	curr_node->text_rect.y = text_h_gap * max_text_h;
	curr_node->text_rect.h = max_text_h;
	curr_node->text_rect.w = max_text_w;

	if(curr_node->icon)
	{
	    T4K_FreeSprite(curr_node->icon);
	    curr_node->icon = NULL;
	}

	if(curr_node->icon_name)
	{
	    sprintf(filename, "%s/%s", data_prefix, curr_node->icon_name);
	    DEBUGMSG(debug_menu, "prerender_menu(): loading sprite %s for item #%d.\n", filename, i);
	    curr_node->icon = T4K_LoadSpriteOfBoundingBox(filename, IMG_ALPHA, button_h, button_h);
	}
	if (!curr_node->icon_name || !curr_node->icon) //if no sprite, or it failed to load
	    DEBUGMSG(debug_menu, "prerender_menu(): no sprite for item #%d.\n", i);

	prerender_menu(menu->submenu[i]);
    }
    DEBUGMSG(debug_menu, "Leaving prerender_menu()\n");
}

void T4K_PrerenderMenu(int index)
{
    if (font_strategy == MF_EXACTLY)
	menus[index]->font_size = default_font_size;
    else 
	set_menu_font_size(menus[index]);

    prerender_menu(menus[index]);
}

/* recursively search for longest menu caption in currently
   used menus and in current language 
   Note the dual return values--if a string longer than length
   is found, length will be set to that value and the return value
   will be the string found. O/W, length is untouched and the
   function returns NULL.
   */
char* find_longest_text(MenuNode* menu, int* length)
{
    char *ret = NULL, *temp = NULL;
    int i;
    int w = 0;
    
    if (menu == NULL)
    {
        DEBUGMSG(debug_menu, "find_longest_text(): NULL pointer, exiting !\n");
        return NULL;
    }
    
    DEBUGMSG(debug_menu, "Entering find_longest_text()\n");
    if(menu->submenu_size == 0) //"leaf" menu
    {
	/*
	   Use a small size (8) here, since text should be proportional and we're only
	   interested in *which* text is longest, not its specific size
	   */
	if (size_text(_(menu->title), 8, &w, NULL))
	{
	    fprintf(stderr, "Warning, couldn't render %s\n", menu->title);
	    w = 0;
	}
	if (w > *length)
	{
	    *length = w;
	    ret = menu->title;
	}
    }
    else //nonleaf menu, recurse
    {
	for(i = 0; i < menu->submenu_size; i++)
	{
	    temp = find_longest_text(menu->submenu[i], length);
	    if(temp)
		ret = temp;
	}
    }
    DEBUGMSG(debug_menu, "Leaving find_longest_text()\n");
    return ret;
}

/* recursively search for longest (sub)menu that is not longer
   than MAX_PAGE_SIZE to ensure better menu outlook */
int find_longest_menu_page(MenuNode* menu)
{
    int longest = 0, i;
    
    if (menu == NULL)
    {
        DEBUGMSG(debug_menu, "find_longest_menu_page(): NULL pointer, exiting !\n");
        return 0;
    }

    if(menu->submenu_size <= MAX_PAGE_SIZE)
	longest = menu->submenu_size;

    for(i = 0; i < menu->submenu_size; i++)
	longest = max(longest, find_longest_menu_page(menu->submenu[i]));

    return longest;
}



/* Calculate how many chars can fit on each line of "tooltips" box.
 * NOTE - the "guts" of this function have been moved into
 * T4K_CharsForWidth() for more general use.
 */
int desc_chars_per_line(int fontsize)
{
    if(desc_panel == NULL)
	return 0;
    else
	return T4K_CharsForWidth(fontsize, desc_panel->w);
}



/* find the longest text in all existing menus and binary search
   for the best font size */
void set_font_size(bool uniform)
{
    int i, j;
    char* longest=NULL;
    int w = 0;


    if (!uniform)
    {
	//if not uniform, do each submenu separately
	for (i = 0; i < N_OF_MENUS; ++i)
	    if (menus[i])
		set_menu_font_size(menus[i]);
	return;   
    }

    for (j = 0; j < N_OF_MENUS; ++j)
    {
	if (menus[j])
	    for (i = 0; i < menus[j]->submenu_size; ++i)
		find_longest_text(menus[j]->submenu[i], &w);
    }

    if(!longest) return;

    for (i = 0; i < N_OF_MENUS; ++i)
	if (menus[i])
	    set_font_size_explicitly(menus[i], binsearch(min_font_size, max_font_size, _(longest)) );
}

void set_menu_font_size(MenuNode* menu)
{
    int length = 0, i, min_f, max_f;
    int w = 0, h = 0;
    char* longest=NULL;
    
    if (menu == NULL)
    {
        DEBUGMSG(debug_menu, "set_menu_font_size(): NULL pointer, exiting !\n");
        return;
    }
    
    for(i = 0; i < menu->submenu_size; i++)
    {
	size_text(_(menu->submenu[i]->title), 8, &w, &h);
	if(w > length)
	{
	    length = w;
	    longest=menu->submenu[i]->title;
	}

	set_menu_font_size(menu->submenu[i]);
    }

    if(!longest) return;

    min_f = min_font_size;
    max_f = max_font_size;

    menu->font_size=binsearch(min_f, max_f, _(longest));

}
int binsearch(int min_f, int max_f, const char* text)
{
    int mid_f, w, h;

    while(min_f < max_f)
    {
	mid_f = (min_f + max_f) / 2;
	if (size_text(text, mid_f, &w, &h) ) while(1);
	if(w + (1.0 + 2.0 * text_w_gap) * (1.0 + 2.0 * text_h_gap) * h < menu_rect.w)
	    min_f = mid_f + 1;
	else
	    max_f = mid_f;
    }
    return min_f;
}
/* recursively set all submenus to a specific font size */
void set_font_size_explicitly(MenuNode* menu, int size)
{
    int i;
    if (menu == NULL) return;

    menu->font_size = size;
    for (i = 0; i < menu->submenu_size; ++i)
	set_font_size_explicitly(menu->submenu[i], size);
}

void T4K_SetMenuFontSize(MFStrategy strategy, int size)
{
    int i = 0;
    font_strategy = strategy;

    if (strategy != MF_EXACTLY && strategy != MF_UNIFORM && strategy != MF_BESTFIT)
	DEBUGMSG(debug_menu, "Invalid font strategy: %d; using default font size %d\n", 
		strategy, (size = default_font_size));

    if (strategy == MF_EXACTLY)
	for (i = 0; i < N_OF_MENUS; ++i)
	    set_font_size_explicitly(menus[i], size);

}
/* prerender arrows, stop button and all non-NULL menus from menus[] array
   this function should be invoked after every resolution change */
void T4K_PrerenderAll()
{
    int i;

    T4K_SetRect(&menu_rect, menu_pos);

    T4K_SetRect(&stop_rect, stop_pos);
    if(stop_button)
	SDL_FreeSurface(stop_button);
    stop_button = T4K_LoadImageOfBoundingBox(stop_path, IMG_ALPHA, stop_rect.w, stop_rect.h);
    /* move button to the right */
    stop_rect.x = T4K_GetScreen()->w - stop_button->w;

    T4K_SetRect(&prev_rect, prev_pos);
    if(prev_arrow)
	SDL_FreeSurface(prev_arrow);
    prev_arrow = T4K_LoadImageOfBoundingBox(prev_path, IMG_ALPHA, prev_rect.w, prev_rect.h);
    if(prev_gray)
	SDL_FreeSurface(prev_gray);
    prev_gray = T4K_LoadImageOfBoundingBox(prev_gray_path, IMG_ALPHA, prev_rect.w, prev_rect.h);
    /* move button to the right */
    prev_rect.x += prev_rect.w - prev_arrow->w;

    T4K_SetRect(&next_rect, next_pos);
    if(next_arrow)
	SDL_FreeSurface(next_arrow);
    next_arrow = T4K_LoadImageOfBoundingBox(next_path, IMG_ALPHA, next_rect.w, next_rect.h);
    if(next_gray)
	SDL_FreeSurface(next_gray);
    next_gray = T4K_LoadImageOfBoundingBox(next_gray_path, IMG_ALPHA, next_rect.w, next_rect.h);

    if (font_strategy == MF_EXACTLY)
	; //no fitting necessary
    else if (font_strategy == MF_UNIFORM)
	set_font_size(true);
    else if (font_strategy == MF_BESTFIT)
	set_font_size(false);


    for(i = 0; i < N_OF_MENUS; i++)
	if(menus[i])
	    T4K_PrerenderMenu(i);
    SDL_UpdateRect(T4K_GetScreen(), 0, 0, 0, 0);
}

int min(int a, int b)
{
    if(a < b) return a;
    else return b;
}

int max(int a, int b)
{
    if(a > b) return a;
    else return b;
}





