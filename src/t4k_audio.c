/*
   t4k_audio.c:

   Audio-related functions.

   Copyright 2003, 2006, 2009, 2010.
Authors: Sam Hart, Jesse Andrews, David Bruce, Brendan Luchen
Project email: <tuxmath-devel@lists.sourceforge.net>
Project website: http://tux4kids.alioth.debian.org

t4k_audio.c is part of the t4k_common library.

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



#include "t4k_common.h"
#include "t4k_globals.h"

/* SDL3_mixer replaced the old channel/Mix_Chunk/Mix_Music model with a
   MIX_Mixer/MIX_Audio/MIX_Track object model. To keep the rest of the
   codebase working against Mix_Chunk and Mix_Music pointers (now aliases
   for MIX_Audio, see t4k_common.h) with the old play-once-and-forget
   semantics, each sound effect gets its own MIX_Track that destroys
   itself via a stopped-callback when playback ends. Music uses a single
   persistent MIX_Track since only one music track plays at a time. */

static MIX_Mixer* mixer = NULL;
static MIX_Track* music_track = NULL;

static bool audio_enabled = true;
static int music_loops = 0;
static Mix_Music *default_music = NULL;
static float sfx_gain = 1.0f;
static float music_gain = 1.0f;

const char* MUSIC_DIR = "sounds";

/* Volume scale used by T4K_Audio{Get,Set}{Sound,Music}Volume(), matching
 * the old SDL1/2-era Mix_Volume() range so existing callers don't change. */
#define T4K_AUDIO_MAX_VOLUME 128

static bool play_track(MIX_Track* track, int loops)
{
    SDL_PropertiesID props = SDL_CreateProperties();
    bool ok;

    SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, loops);
    ok = MIX_PlayTrack(track, props);
    SDL_DestroyProperties(props);
    return ok;
}

int T4K_AudioOpen(int frequency, int channels)
{
    SDL_AudioSpec spec;

    if (!MIX_Init())
        return 0;

    SDL_zero(spec);
    spec.freq = frequency;
    spec.format = SDL_AUDIO_S16;
    spec.channels = channels;

    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
    if (!mixer)
    {
        MIX_Quit();
        return 0;
    }

    music_track = MIX_CreateTrack(mixer);
    if (!music_track)
    {
        MIX_DestroyMixer(mixer);
        mixer = NULL;
        MIX_Quit();
        return 0;
    }

    return 1;
}

void T4K_AudioClose(void)
{
    if (music_track)
    {
        MIX_DestroyTrack(music_track);
        music_track = NULL;
    }
    if (mixer)
    {
        MIX_DestroyMixer(mixer);
        mixer = NULL;
    }
    MIX_Quit();
}

MIX_Mixer* T4K_GetMixer(void)
{
    return mixer;
}

static void sfx_track_stopped(void* userdata, MIX_Track* track)
{
    (void)userdata;
    MIX_DestroyTrack(track);
}

// play sound once and exit
void T4K_PlaySound(Mix_Chunk* sound)
{
    T4K_PlaySoundLoop(sound, 0);
}

// play sound "loops" times, -1 for infinite
void T4K_PlaySoundLoop(Mix_Chunk* sound, int loops)
{
    MIX_Track* track;

    if (!sound || !audio_enabled || !mixer)
        return;

    track = MIX_CreateTrack(mixer);
    if (!track)
        return;

    MIX_SetTrackAudio(track, sound);
    MIX_SetTrackGain(track, sfx_gain);
    MIX_SetTrackStoppedCallback(track, sfx_track_stopped, NULL);

    if (!play_track(track, loops))
        MIX_DestroyTrack(track);
}

void T4K_AudioHaltChannel( int channel )
{
    /* SDL3_mixer has no fixed channel indices - every sound effect gets
       its own track that is destroyed when it stops. -1 (halt everything)
       is the only case that still makes sense here. */
    if (channel == -1 && mixer)
        MIX_StopAllTracks(mixer, 0);
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
}

/* audioMusicUnload attempts to unload any music data that was
 * loaded using the audioMusicLoad function
 */
void T4K_AudioMusicUnload()
{
    if (music_track)
        MIX_StopTrack(music_track, 0);
    if(default_music)
        MIX_DestroyAudio(default_music);
    default_music = NULL;
}

bool T4K_IsPlayingMusic()
{
    return music_track && MIX_TrackPlaying(music_track);
}

void T4K_AudioMusicPause(void)
{
    if (music_track)
        MIX_PauseTrack(music_track);
}

void T4K_AudioMusicResume(void)
{
    if (music_track)
        MIX_ResumeTrack(music_track);
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
    if (audio_enabled && music_track && musicData)
    {
        MIX_SetTrackAudio(music_track, musicData);
        MIX_SetTrackGain(music_track, music_gain);
        play_track(music_track, loops);
    }
}

void T4K_AudioEnable(bool enabled)
{
    if (audio_enabled == enabled)
	return;

    audio_enabled = enabled;
    if (audio_enabled)
    {
        if (default_music && music_track)
        {
            MIX_SetTrackAudio(music_track, default_music);
            MIX_SetTrackGain(music_track, music_gain);
            play_track(music_track, music_loops);
        }
    }
    else
    {
        if (mixer)
            MIX_StopAllTracks(mixer, 0);
        if (music_track)
            MIX_StopTrack(music_track, MIX_TrackMSToFrames(music_track, 100));
    }
}

void T4K_AudioToggle()
{
    T4K_AudioEnable(!audio_enabled);
}

/* Sound-effect and music volume, kept on the old 0-128 scale used by
 * the SDL1/2-era Mix_Volume()/Mix_VolumeMusic() so existing callers don't
 * need to change.
 */
int T4K_AudioGetSoundVolume(void)
{
    return (int)(sfx_gain * T4K_AUDIO_MAX_VOLUME);
}

void T4K_AudioSetSoundVolume(int volume)
{
    if (volume < 0)
        volume = 0;
    if (volume > T4K_AUDIO_MAX_VOLUME)
        volume = T4K_AUDIO_MAX_VOLUME;
    sfx_gain = (float)volume / T4K_AUDIO_MAX_VOLUME;
}

int T4K_AudioGetMusicVolume(void)
{
    return (int)(music_gain * T4K_AUDIO_MAX_VOLUME);
}

void T4K_AudioSetMusicVolume(int volume)
{
    if (volume < 0)
        volume = 0;
    if (volume > T4K_AUDIO_MAX_VOLUME)
        volume = T4K_AUDIO_MAX_VOLUME;
    music_gain = (float)volume / T4K_AUDIO_MAX_VOLUME;
    if (music_track)
        MIX_SetTrackGain(music_track, music_gain);
}
