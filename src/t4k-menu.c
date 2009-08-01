/*
  t4k-menu.c

  Functions responsible for loading, parsing and displaying game menu.

  Part of "Tux4Kids" Project
  http://www.tux4kids.com/

  Author: Boleslaw Kulbabinski <bkulbabinski@gmail.com>, (C) 2009

  Copyright: See COPYING file that comes with this distribution.
*/

#include "tux4kids-common.h"
#include "t4k-globals.h"
#include "t4k-compiler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct mNode {
  struct mNode* parent;

  char* title;
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

#define QUIT -2
#define STOP -1
#define RUN_MAIN_MENU -3

int n_of_activities;
char** activities;

char* data_prefix;

Mix_Chunk* snd_click;
Mix_Chunk* snd_hover;
Mix_Music* menu_music;

#define N_OF_MENUS 10
MenuNode* menus[N_OF_MENUS];

/* font size used in current resolution */
int curr_font_size;

/* buffer size used when reading attributes or names */
const int buf_size = 128;

/* actions available while viewing the menu */
enum { NONE, CLICK, PAGEUP, PAGEDOWN, STOP_ESC, RESIZED };

/* stop button, left and right arrow positions do not
   depend on currently displayed menu */
SDL_Rect menu_rect, stop_rect, prev_rect, next_rect, menu_title_rect;
SDL_Surface *stop_button, *prev_arrow, *next_arrow, *prev_gray, *next_gray;

/*TODO: move these constants into a config file (maybe together with
  titleGetScreen() paths and rects ? ) */
const float menu_pos[4] = {0.38, 0.23, 0.55, 0.72};
const float stop_pos[4] = {0.94, 0.0, 0.06, 0.06};
const float prev_pos[4] = {0.87, 0.93, 0.06, 0.06};
const float next_pos[4] = {0.94, 0.93, 0.06, 0.06};
const char* stop_path = "/images/status/stop.svg";
const char* prev_path = "/images/status/left.svg";
const char* next_path = "/images/status/right.svg";
const char* prev_gray_path = "/images/status/left_gray.svg";
const char* next_gray_path = "/images/status/right_gray.svg";
const float button_gap = 0.2, text_h_gap = 0.4, text_w_gap = 0.5, button_radius = 0.27;
const int min_font_size = 8, default_font_size = 20, max_font_size = 40;



/* local functions */
MenuNode*       create_empty_node();
char*           get_attribute_name(const char* token);
char*           get_attribute_value(const char* token);
void            read_attributes(FILE* xml_file, MenuNode* node);
MenuNode*       load_menu_from_file(FILE* xml_file, MenuNode* parent);
void            free_menu(MenuNode* menu);

SDL_Surface**   render_buttons(MenuNode* menu, bool selected);
void            prerender_menu(MenuNode* menu);
char*           find_longest_text(MenuNode* menu, int* length);
void            set_font_size();


/* initialization of menu module */
void SetActivitiesList(int num, char** acts)
{
  n_of_activities = num;
  activities = acts;
}

void SetMenuSounds(Mix_Music* music, Mix_Chunk* click, Mix_Chunk* hover)
{
  snd_click = click;
  snd_hover = hover;
  menu_music = music;
}

void SetImagePathPrefix(char* pref)
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
  new_node->parent = NULL;
  new_node->title = NULL;
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

/* read attributes and fill appropriate node fields */
void read_attributes(FILE* xml_file, MenuNode* node)
{
  char attr_name[buf_size];
  char attr_val[buf_size];
  int i;

  /* read tokens until closing '>' is found */
  do
  {
    fscanf(xml_file, " %[^=\n]", attr_name);

    DEBUGMSG(dbg_menu_parser, "read_attributes(): read attribute name: %s\n", attr_name);
    if(strchr(attr_name, '>'))
      break;

    fscanf(xml_file, "=\"%[^\"]\"", attr_val);
    DEBUGMSG(dbg_menu_parser, "read_attributes(): read attribute value: %s\n", attr_val);

    if(strcmp(attr_name, "title") == 0)
      node->title = strdup(attr_val);
    else if(strcmp(attr_name, "entries") == 0)
      node->submenu_size = atoi(attr_val);
    else if(strcmp(attr_name, "param") == 0)
      node->param = atoi(attr_val);
    else if(strcmp(attr_name, "sprite") == 0)
      node->icon_name = strdup(attr_val);
    else if(strcmp(attr_name, "run") == 0)
    {
      if(strcmp(attr_val, "RUN_MAIN_MENU") == 0)
        node->activity = RUN_MAIN_MENU;
      for(i = 0; i < n_of_activities; i++)
        if(strcmp(attr_val, activities[i]) == 0)
          node->activity = i;
    }
    else
      DEBUGMSG(dbg_menu_parser, "read_attributes(): unknown attribute %s , omitting\n", attr_name);

  } while(strchr(attr_val, '>') == NULL);
}

/* recursively read and parse given xml menu file and create menu tree
   return NULL in case of problems */
MenuNode* load_menu_from_file(FILE* xml_file, MenuNode* parent)
{
  MenuNode* new_node = create_empty_node();
  char buffer[buf_size];
  int i;

  new_node->parent = parent;

  DEBUGMSG(dbg_menu_parser, "entering load_menu_from_file()\n");
  fscanf(xml_file, " < %s", buffer);

  if(strcmp(buffer, "menu") == 0)
  {
    read_attributes(xml_file, new_node);
    if(new_node->title == NULL)
    {
      DEBUGMSG(dbg_menu_parser, "load_menu_from_file(): no title attribute, exiting\n");
      return NULL;
    }

    if(new_node->submenu_size > 0)
    {
      new_node->submenu = malloc(new_node->submenu_size * sizeof(MenuNode));
      for(i = 0; i < new_node->submenu_size; i++)
        new_node->submenu[i] = load_menu_from_file(xml_file, new_node);
    }

    fscanf(xml_file, " </%[^>\n]> ", buffer);
    if(strcmp(buffer, "menu") != 0)
      DEBUGMSG(dbg_menu_parser, "load_menu_from_file(): warning - no closing menu tag, found %s instead\n", buffer);
  }
  else if(strcmp(buffer, "item") == 0)
  {
    read_attributes(xml_file, new_node);
    if(new_node->title == NULL)
    {
      DEBUGMSG(dbg_menu_parser, "load_menu_from_file(): no title attribute, exiting\n");
      return NULL;
    }
  }
  else
  {
    DEBUGMSG(dbg_menu_parser, "load_menu_from_file(): unknown tag: %s\n, exiting\n", buffer);
    return NULL;
  }

  DEBUGMSG(dbg_menu_parser, "load_menu_from_file(): node loaded successfully\n");
  return new_node;
}

/* recursively free all non-NULL pointers in a menu tree */
void free_menu(MenuNode* menu)
{
  int i;

  DEBUGMSG(dbg_menu, "entering free_menu()\n");
  if(menu != NULL)
  {
    if(menu->title != NULL)
      free(menu->title);
    if(menu->icon_name != NULL)
      free(menu->icon_name);
    if(menu->icon != NULL)
      FreeSprite(menu->icon);

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

    free(menu);
  }
}

/* create a simple one-level menu without sprites.
   all given strings are copied */
void CreateOneLevelMenu(int index, int items, char** item_names, char* title, char* trailer)
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



/* Display the menu and run the event loop.
   if return_choice = true then return chosen value instead of
   running handle_activity()
   this function is a modified copy of choose_menu_item() */
int RunMenu(int index, bool return_choice, void (*draw_background)(), int (*handle_event)(SDL_Event*), void (*handle_animations)(), int (*handle_activity)(int, int))
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
  int items;

  int action = NONE;

  Uint32 frame_start = 0;       //For keeping frame rate constant
  Uint32 frame_now = 0;
  Uint32 frame_counter = 0;
  int loc = -1;                  //The currently selected menu item
  int old_loc = -1;
  int click_flag = 1;

  for(;;) /* one loop body execution for one menu page */
  {
    DEBUGMSG(dbg_menu, "run_menu(): drawing whole new menu page\n");

    draw_background();
    /* render buttons for current menu page */
    menu_item_unselected = render_buttons(menu, false);
    menu_item_selected = render_buttons(menu, true);
    items = min(menu->entries_per_screen, menu->submenu_size - menu->first_entry);

    DEBUGMSG(dbg_menu, "run_menu(): drawing %d buttons\n", items);
    for(i = 0; i < items; i++)
    {
      if(loc == i)
        SDL_BlitSurface(menu_item_selected[i], NULL, GetScreen(), &menu->submenu[menu->first_entry + i]->button_rect);
      else
        SDL_BlitSurface(menu_item_unselected[i], NULL, GetScreen(), &menu->submenu[menu->first_entry + i]->button_rect);
      if(menu->submenu[menu->first_entry + i]->icon)
        SDL_BlitSurface(menu->submenu[menu->first_entry + i]->icon->default_img, NULL, GetScreen(), &menu->submenu[menu->first_entry + i]->icon_rect);
    }

    SDL_BlitSurface(stop_button, NULL, GetScreen(), &stop_rect);

    if(menu->entries_per_screen < menu->submenu_size)
    {
      /* display arrows */
      if(menu->first_entry > 0)
        SDL_BlitSurface(prev_arrow, NULL, GetScreen(), &prev_rect);
      else
        SDL_BlitSurface(prev_gray, NULL, GetScreen(), &prev_rect);
      if(menu->first_entry + items < menu->submenu_size)
        SDL_BlitSurface(next_arrow, NULL, GetScreen(), &next_rect);
      else
        SDL_BlitSurface(next_gray, NULL, GetScreen(), &next_rect);
    }

    if(menu->show_title)
    {
      menu_title_rect = menu->submenu[0]->button_rect;
      menu_title_rect.y = menu_rect.y - menu_title_rect.h;
      title_surf = BlackOutline(_(menu->title), curr_font_size, &red);
      SDL_BlitSurface(title_surf, NULL, GetScreen(), &menu_title_rect);
      SDL_FreeSurface(title_surf);
    }
    SDL_UpdateRect(GetScreen(), 0, 0, 0, 0);

    SDL_WM_GrabInput(SDL_GRAB_OFF);

    while (SDL_PollEvent(&event));  // clear pending events

    /******** Main loop: *********/
    stop = false;
    DEBUGMSG(dbg_menu, "run_menu(): entering menu loop\n");
    while (!stop)
    {
      frame_start = SDL_GetTicks();         /* For keeping frame rate constant.*/

      action = NONE;
      while (!stop && SDL_PollEvent(&event))
      {
        switch (event.type)
        {
          case SDL_QUIT:
          {
            FreeSurfaceArray(menu_item_unselected, items);
            FreeSurfaceArray(menu_item_selected, items);
            return QUIT;
          }

          case SDL_MOUSEMOTION:
          {
            loc = -1;
            for (i = 0; i < items; i++)
            {
              if (inRect(menu->submenu[menu->first_entry + i]->button_rect, event.motion.x, event.motion.y))
              {
                if(snd_hover)
                  PlaySound(snd_hover);
                loc = i;
                break;   /* from for loop */
              }
            }

            /* "Left" button - make click if button active: */
            if(inRect(prev_rect, event.motion.x, event.motion.y)
               && menu->first_entry > 0)
            {
              if(click_flag)
              {
                if(snd_hover)
                  PlaySound(snd_hover);
                click_flag = 0;
              }
            }

            /* "Right" button - make click if button active: */
            else if(inRect(next_rect, event.motion.x, event.motion.y)
               && menu->first_entry + items < menu->submenu_size)
            {
              if(click_flag)
              {
                if(snd_hover)
                  PlaySound(snd_hover);
                click_flag = 0;
              }
            }

            /* "stop" button */
            else if (inRect(stop_rect, event.motion.x, event.motion.y ))
            {
              if(click_flag)
              {
                if(snd_hover)
                  PlaySound(snd_hover);
                click_flag = 0;
              }
            }

            else  // Mouse outside of arrow rects - re-enable click sound:
              click_flag = 1;

            break;
          }

          case SDL_MOUSEBUTTONDOWN:
          {
            loc = -1;  // By default, don't be in any entry
            for (i = 0; i < items; i++)
            {
              if (inRect(menu->submenu[menu->first_entry + i]->button_rect, event.motion.x, event.motion.y))
              {
                // Play sound if loc is being changed:
                if(snd_click)
                  PlaySound(snd_click);
                loc = i;
                action = CLICK;
                break;   /* from for loop */
              }
            }

            /* "Left" button */
            if (inRect(prev_rect, event.motion.x, event.motion.y)
               && menu->first_entry > 0)
            {
              if(snd_click)
                PlaySound(snd_click);
              action = PAGEUP;
            }

            /* "Right" button - go to next page: */
            else if (inRect(next_rect, event.motion.x, event.motion.y )
               && menu->first_entry + items < menu->submenu_size)
            {
              if(snd_click)
                PlaySound(snd_click);
              action = PAGEDOWN;
            }

            /* "Stop" button - go to main menu: */
            else if (inRect(stop_rect, event.button.x, event.button.y ))
            {
              if(snd_click)
                PlaySound(snd_click);
              action = STOP_ESC;
            }

            break;
          } /* End of case SDL_MOUSEDOWN */

          case SDL_KEYDOWN:
          {
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
                  PlaySound(snd_click);
                action = CLICK;
                break;
              }

              /* Go to previous page, if present: */
              case SDLK_LEFT:
              case SDLK_PAGEUP:
              {
                if(snd_click)
                  PlaySound(snd_click);
                if (menu->first_entry > 0)
                  action = PAGEUP;
                break;
              }

              /* Go to next page, if present: */
              case SDLK_RIGHT:
              case SDLK_PAGEDOWN:
              {
                if(snd_click)
                  PlaySound(snd_click);
                if (menu->first_entry + items < menu->submenu_size)
                  action = PAGEDOWN;
                break;
              }

              /* Go up one entry, if present: */
              case SDLK_UP:
              {
                if(snd_hover)
                  PlaySound(snd_hover);
                if (loc > 0)
                  loc--;
                else if (menu->submenu_size <= menu->entries_per_screen) 
                  loc = menu->submenu_size - 1;  // wrap around if only 1 GetScreen()
                else if (menu->first_entry > 0)
                {
                  loc = menu->entries_per_screen - 1;
                  action = PAGEUP;
                }
                break;
              }

              case SDLK_DOWN:
              {
                if(snd_hover)
                  PlaySound(snd_hover);
                if (loc + 1 < min(menu->submenu_size, menu->entries_per_screen))
                  loc++;
                else if (menu->submenu_size <= menu->entries_per_screen) 
                  loc = 0;  // wrap around if only 1 GetScreen()
                else if (menu->first_entry + menu->entries_per_screen < menu->submenu_size)
                {
                  loc = 0;
                  action = PAGEDOWN;
                }
                break;
              }

              /* Toggle GetScreen() mode: */
              case SDLK_F10:
              {
                SwitchScreenMode();
                action = RESIZED;
                break;
              }

              /* Toggle menu music: */
              case SDLK_F11:
              {
                if(IsPlayingMusic())
                {
                  AudioMusicUnload();
                }
                else if(menu_music)
                {
                  AudioMusicLoad(menu_music, -1);
                }
                break;
              }

              default:
              {
                /* Some other key - do nothing. */
              }

              break;  /* To get out of _outer_ switch/case statement */
            }  /* End of key switch statement */
          }  // End of case SDL_KEYDOWN in outer switch statement
        }  // End event switch statement

        if (old_loc != loc) {
          DEBUGMSG(dbg_menu, "run_menu(): changed button focus, old=%d, new=%d\n", old_loc, loc);
          if(old_loc >= 0 && old_loc < items)
          {
            tmp_rect = menu->submenu[old_loc + menu->first_entry]->button_rect;
            SDL_BlitSurface(menu_item_unselected[old_loc], NULL, GetScreen(), &tmp_rect);
            if(menu->submenu[menu->first_entry + old_loc]->icon)
              SDL_BlitSurface(menu->submenu[menu->first_entry + old_loc]->icon->default_img, NULL, GetScreen(), &menu->submenu[menu->first_entry + old_loc]->icon_rect);
            SDL_UpdateRect(GetScreen(), tmp_rect.x, tmp_rect.y, tmp_rect.w, tmp_rect.h);
          }
          if(loc >= 0 && loc < items)
          {
            tmp_rect = menu->submenu[loc + menu->first_entry]->button_rect;
            SDL_BlitSurface(menu_item_selected[loc], NULL, GetScreen(), &tmp_rect);
            if(menu->submenu[menu->first_entry + loc]->icon)
            {
              SDL_BlitSurface(menu->submenu[menu->first_entry + loc]->icon->default_img, NULL, GetScreen(), &menu->submenu[menu->first_entry + loc]->icon_rect);
              menu->submenu[menu->first_entry + loc]->icon->cur = 0;
            }
            SDL_UpdateRect(GetScreen(), tmp_rect.x, tmp_rect.y, tmp_rect.w, tmp_rect.h);
          }
          old_loc = loc;
        }

        if(handle_event(&event))
          stop = true;

        switch(action)
        {
          case RESIZED:
            RenderTitleScreen();
            menu->first_entry = 0;
            PrerenderAll();
            stop = true;
            break;

          case CLICK:
            if(loc < 0 || loc >= items)
            {
              DEBUGMSG(dbg_menu, "run_menu(): incorrect location for CLICK action (%d) !\n", loc);
            }
            else
            {
              tmp_node = menu->submenu[menu->first_entry + loc];
              if(tmp_node->submenu_size == 0)
              {
                if(return_choice)
                {
                  FreeSurfaceArray(menu_item_unselected, items);
                  FreeSurfaceArray(menu_item_selected, items);
                  return tmp_node->activity;
                }
                else
                {
                  if(tmp_node->activity == RUN_MAIN_MENU)
                  {
                    /* go back to the root of this menu */
                    menu = menus[index];
                  }
                  else
                  {
                    if(handle_activity(tmp_node->activity, tmp_node->param) == QUIT)
                    {
                      DEBUGMSG(dbg_menu, "run_menu(): handle_activity() returned QUIT message, exiting.\n");
                      FreeSurfaceArray(menu_item_unselected, items);
                      FreeSurfaceArray(menu_item_selected, items);
                      return QUIT;
                    }
                  }
                }
              }
              else
              {
                menu->first_entry = 0;
                menu = tmp_node;
                menu->first_entry = 0;
              }
              stop = true;
            }
            break;

          case STOP_ESC:
            if(menu->parent == NULL)
            {
              FreeSurfaceArray(menu_item_unselected, items);
              FreeSurfaceArray(menu_item_selected, items);
              return STOP;
            }
            else
              menu = menu->parent;
            stop = true;
            break;

          case PAGEUP:
            menu->first_entry -= menu->entries_per_screen;
            stop = true;
            break;

          case PAGEDOWN:
            menu->first_entry += menu->entries_per_screen;
            stop = true;
            break;
        }

      }  // End of SDL_PollEvent while loop

      if(stop)
        break;

      if(!stop && frame_counter % 5 == 0 && loc >= 0 && loc < items)
      {
        tmp_sprite = menu->submenu[menu->first_entry + loc]->icon;
        if(tmp_sprite)
        {
          SDL_BlitSurface(menu_item_selected[loc], NULL, GetScreen(), &menu->submenu[menu->first_entry + loc]->icon_rect);
          SDL_BlitSurface(tmp_sprite->frame[tmp_sprite->cur], NULL, GetScreen(), &menu->submenu[menu->first_entry + loc]->icon_rect);
          UpdateRect(GetScreen(), &menu->submenu[menu->first_entry + loc]->icon_rect);
          NextFrame(tmp_sprite);
        }
      }

      handle_animations();

      /* Wait so we keep frame rate constant: */
      frame_now = SDL_GetTicks();
      if (frame_now < frame_start)
        frame_start = frame_now;  // in case the timer wraps around
      if((frame_now - frame_start) < 1000 / MAX_FPS)
        SDL_Delay(1000 / MAX_FPS - (frame_now - frame_start));

      frame_counter++;
    } // End of while(!stop) loop

    /* free button surfaces */
    DEBUGMSG(dbg_menu, "run_menu(): freeing %d button surfaces\n", items);
    FreeSurfaceArray(menu_item_unselected, items);
    FreeSurfaceArray(menu_item_selected, items);
  }

  return QUIT;
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
    DEBUGMSG(dbg_menu, "render_buttons(): failed to allocate memory for buttons!\n");
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

    SDL_BlitSurface(GetScreen(), &curr_rect, menu_items[i], NULL);
    /* button */
    if(selected)
      tmp_surf = CreateButton(curr_rect.w, curr_rect.h, button_radius * curr_rect.h, SEL_RGBA);
    else
      tmp_surf = CreateButton(curr_rect.w, curr_rect.h, button_radius * curr_rect.h, REG_RGBA);

    SDL_BlitSurface(tmp_surf, NULL, menu_items[i], NULL);
    SDL_FreeSurface(tmp_surf);

    /* text */
    tmp_surf = BlackOutline(_(menu->submenu[menu->first_entry + i]->title),
                            curr_font_size, selected ? &yellow : &white);
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

  if(NULL == menu)
  {
    DEBUGMSG(dbg_menu, "prerender_menu(): NULL pointer, exiting !\n");
    return;
  }

  if(0 == menu->submenu_size)
  {
    DEBUGMSG(dbg_menu, "prerender_menu(): no submenu, exiting.\n");
    return;
  }

  for(i = 0; i < menu->submenu_size; i++)
  {
    if(menu->submenu[i]->icon_name)
      found_icons = true;
    temp_surf = NULL;
    temp_surf = SimpleText(_(menu->submenu[i]->title), curr_font_size, &black);
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
      FreeSprite(curr_node->icon);

    if(curr_node->icon_name)
    {
      sprintf(filename, "%s/images/sprites/%s", data_prefix, curr_node->icon_name);
      DEBUGMSG(dbg_menu, "prerender_menu(): loading sprite %s for item #%d.\n", filename, i);
      curr_node->icon = LoadSpriteOfBoundingBox(filename, IMG_ALPHA, button_h, button_h);
    }
    else
      DEBUGMSG(dbg_menu, "prerender_menu(): no sprite for item #%d.\n", i);

    prerender_menu(menu->submenu[i]);
  }
}

char* find_longest_text(MenuNode* menu, int* length)
{
  SDL_Surface* text = NULL;
  char *ret = NULL, *temp = NULL;
  int i;

  if(menu->submenu_size == 0)
  {
    text = SimpleText(_(menu->title), curr_font_size, &black);
    if(text->w > *length)
    {
      *length = text->w;
      ret = menu->title;
    }
    SDL_FreeSurface(text);
  }
  else
  {
    for(i = 0; i < menu->submenu_size; i++)
    {
      temp = find_longest_text(menu->submenu[i], length);
      if(temp)
        ret = temp;
    }
  }
  return ret;
}

/* find the longest text in all existing menus and binary search
   for the best font size */
void set_font_size()
{
  char* longest = NULL;
  char* temp;
  SDL_Surface* surf;
  int length = 0, i, min_f, max_f, mid_f;

  curr_font_size = default_font_size;

  for(i = 0; i < N_OF_MENUS; i++)
  {
    if(menus[i])
    {
      temp = find_longest_text(menus[i], &length);
      if(temp)
        longest = temp;
    }
  }

  if(!longest)
    return;

  min_f = min_font_size;
  max_f = max_font_size;

  while(min_f < max_f)
  {
    mid_f = (min_f + max_f) / 2;
    surf = SimpleText(_(longest), mid_f, &black);
    if(surf->w + (1.0 + 2.0 * text_w_gap) * (1.0 + 2.0 * text_h_gap) * surf->h < menu_rect.w)
      min_f = mid_f + 1;
    else
      max_f = mid_f;
    SDL_FreeSurface(surf);
  }

  curr_font_size = min_f;
}

/* prerender arrows, stop button and all non-NULL menus from menus[] array
   this function should be invoked after every resolution change */
void PrerenderAll()
{
  int i;
  char fn[PATH_MAX];

  SetRect(&menu_rect, menu_pos);

  SetRect(&stop_rect, stop_pos);
  if(stop_button)
    SDL_FreeSurface(stop_button);
  sprintf("%s%s", data_prefix, stop_path);
  stop_button = LoadImageOfBoundingBox(fn, IMG_ALPHA, stop_rect.w, stop_rect.h);
  /* move button to the right */
  stop_rect.x = GetScreen()->w - stop_button->w;

  SetRect(&prev_rect, prev_pos);
  if(prev_arrow)
    SDL_FreeSurface(prev_arrow);
  sprintf("%s%s", data_prefix, prev_path);
  prev_arrow = LoadImageOfBoundingBox(fn, IMG_ALPHA, prev_rect.w, prev_rect.h);
  if(prev_gray)
    SDL_FreeSurface(prev_gray);
  sprintf("%s%s", data_prefix, prev_gray_path);
  prev_gray = LoadImageOfBoundingBox(fn, IMG_ALPHA, prev_rect.w, prev_rect.h);
  /* move button to the right */
  prev_rect.x += prev_rect.w - prev_arrow->w;

  SetRect(&next_rect, next_pos);
  if(next_arrow)
    SDL_FreeSurface(next_arrow);
  sprintf("%s%s", data_prefix, next_path);
  next_arrow = LoadImageOfBoundingBox(fn, IMG_ALPHA, next_rect.w, next_rect.h);
  if(next_gray)
    SDL_FreeSurface(next_gray);
  sprintf("%s%s", data_prefix, next_gray_path);
  next_gray = LoadImageOfBoundingBox(fn, IMG_ALPHA, next_rect.w, next_rect.h);

  set_font_size();

  for(i = 0; i < N_OF_MENUS; i++)
    if(menus[i])
      prerender_menu(menus[i]);
}

void LoadMenu(int index, const char* file_name)
{
  FILE* menu_file = NULL;
  int i;

  if(menus[index])
  {
    free_menu(menus[index]);
    menus[index] = NULL;
  }

  menu_file = fopen(file_name, "r");
  if(menu_file == NULL)
  {
    DEBUGMSG(dbg_menu, "LoadMenu(): Could not load %s !\n", file_name);
  }
  else
  {
    menus[index] = load_menu_from_file(menu_file, NULL);
    fclose(menu_file);
  }
}



/* free all loaded menu trees */
void UnloadMenus(void)
{
  int i;

  DEBUGMSG(dbg_menu, "entering UnloadMenus()\n");

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
      DEBUGMSG(dbg_menu, "UnloadMenus(): freeing menu #%d\n", i);
      free_menu(menus[i]);
    }

  DEBUGMSG(dbg_menu, "leaving UnloadMenus()\n");
}

