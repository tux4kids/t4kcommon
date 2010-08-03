/***************************************************************************
 -  file: audio.c
 -  description: this file contains audio related functions
                            -------------------
    begin                : Jan 22, 2003
    copyright            : Sam Hart, Jesse Andrews (C) 2003
    email                : tuxtype-dev@tux4kids.net

    Modified for use in tuxmath by David Bruce - 2006.
    email                : <dbruce@tampabay.rr.com>
                           <tuxmath-devel@lists.sourceforge.net>
    Modified further for use in libt4k_common - 2009
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/


#include "t4k_common.h"
#include "t4k_globals.h"

static bool audio_enabled = true;
static int music_loops = 0;
static Mix_Music *default_music = NULL;

const char* MUSIC_DIR = "sounds";

// play sound once and exit
void T4K_PlaySound(Mix_Chunk* sound)
{
  T4K_PlaySoundLoop(sound, 0);
}

// play sound "loops" times, -1 for infinite
void T4K_PlaySoundLoop(Mix_Chunk* sound, int loops)
{
  if(sound && audio_enabled)
    Mix_PlayChannel(-1, sound, loops);
}

void T4K_AudioHaltChannel( int channel )
{
      Mix_HaltChannel(channel);
}

/* audioMusicLoad attempts to load and play the music file
 * Note: loops == -1 means forever
 */
void T4K_AudioMusicLoad(char* music_path, int loops)
{
  if (audio_enabled)
  {
    default_music = T4K_LoadMusic(music_path);
    T4K_AudioMusicPlay(default_music, loops);
  }
//  T4K_AudioMusicUnload(); // make sure defaultMusic is clear
//  default_music = T4K_LoadMusic(music_path);
//  music_loops = loops;
//  if (audio_enabled)
//    Mix_PlayMusic(default_music, loops);
}

/* audioMusicUnload attempts to unload any music data that was
 * loaded using the audioMusicLoad function
 */
void T4K_AudioMusicUnload()
{
  if(default_music)
    Mix_FreeMusic(default_music);
  default_music = NULL;
}

bool T4K_IsPlayingMusic()
{
  return (default_music != NULL);
}

/* audioMusicPlay attempts to play the passed music data.
 * if a music file was loaded using the audioMusicLoad
 * it will be stopped and unloaded
 * Note: loops == -1 means forever
 */
void T4K_AudioMusicPlay(Mix_Music *musicData, int loops)
{
  if (musicData != default_music)
  {
    T4K_AudioMusicUnload(); //FIXME this feels buggy...
  }
  music_loops = loops;
  if (audio_enabled)
    Mix_PlayMusic(musicData, loops);
}

void T4K_AudioEnable(bool enabled)
{
  if (audio_enabled == enabled) 
    return;
  
  audio_enabled = enabled;
  if (audio_enabled)
  {
    if (default_music)
      Mix_PlayMusic(default_music, music_loops);
  }
  else
  {
    Mix_HaltChannel(-1);
    Mix_FadeOutMusic(100);
  }
}

void T4K_AudioToggle()
{
  T4K_AudioEnable(!audio_enabled);   
}
