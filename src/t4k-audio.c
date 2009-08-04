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
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/


#include "tux4kids-common.h"
#include "t4k-globals.h"

Mix_Music *default_music;

void PlaySound(Mix_Chunk* sound)
{
  if(sound)
    Mix_PlayChannel(-1, sound, 0);
}

/* audioMusicLoad attempts to load and play the music file
 * Note: loops == -1 means forever
 */
void AudioMusicLoad(char* music_path, int loops)
{
  audioMusicUnload(); // make sure defaultMusic is clear
  default_music = LoadMusic(music_path);
  Mix_PlayMusic(default_music, loops);
}

/* audioMusicUnload attempts to unload any music data that was
 * loaded using the audioMusicLoad function
 */
void AudioMusicUnload()
{
  if(default_music)
    Mix_FreeMusic(default_music);
  default_music = NULL;
}

bool IsPlayingMusic()
{
  return (default_music != NULL);
}

/* audioMusicPlay attempts to play the passed music data.
 * if a music file was loaded using the audioMusicLoad
 * it will be stopped and unloaded
 * Note: loops == -1 means forever
 */
void AudioMusicPlay(Mix_Music *musicData, int loops)
{
  audioMusicUnload();
  Mix_PlayMusic(musicData, loops);
}

