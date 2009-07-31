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

void playsound(Mix_Chunk* sound)
{
 Mix_PlayChannel(-1, sound, 0);
}

Mix_Music *defaultMusic = NULL; // holds music for audioMusicLoad/unload

/* audioMusicLoad attempts to load and play the music file 
 * Note: loops == -1 means forever
 */
void audioMusicLoad(Mix_Music* music, int loops)
{
  audioMusicUnload(); // make sure defaultMusic is clear
  defaultMusic = music;
  Mix_PlayMusic(defaultMusic, loops);
}


/* audioMusicUnload attempts to unload any music data that was
 * loaded using the audioMusicLoad function
 */
void audioMusicUnload( void ) {

  if ( defaultMusic )
    Mix_FreeMusic( defaultMusic );

  defaultMusic=NULL;
}

/* audioMusicPlay attempts to play the passed music data. 
 * if a music file was loaded using the audioMusicLoad
 * it will be stopped and unloaded
 * Note: loops == -1 means forever
 */
void audioMusicPlay( Mix_Music *musicData, int loops ) { 

  audioMusicUnload();        
  Mix_PlayMusic( musicData, loops );
}
