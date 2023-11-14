/*
   t4k_tts.c:

   Text-To-Speach-related functions.

   Copyright 2013.
Author: Nalin.x.Linux <Nalin.x.Linux@gmail.com>
Project email: <tuxmath-devel@lists.sourceforge.net>
Project website: http://tux4kids.alioth.debian.org

t4k_tts.c is part of the t4k_common library.

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


#include "t4k_globals.h"
#include "t4k_common.h"


#include "SDL_thread.h"

SDL_Thread *tts_thread;
int text_to_speech_status;

#if WITH_ESPEAK == 1

#if WITH_ESPEAK_NG
# include <espeak-ng/speak_lib.h>
#else
# include <speak_lib.h>
#endif

/* TTS annoncement should be in thread otherwise 
 * it will freez the game till announcemrnt finishes */
int tts_thread_func(void *arg)
{
	espeak_POSITION_TYPE position_type = POS_CHARACTER;
	tts_argument recived = *((tts_argument*)(arg));
	fprintf(stderr,"\nSpeaking : %s - %d",recived.text,recived.mode);
	if (recived.mode == INTERRUPT)
		T4K_Tts_cancel();
	else
		T4K_Tts_wait();
	
	int Size = strlen(recived.text)+1;
	espeak_Synth(recived.text, Size, 0, position_type, 0,	espeakCHARS_AUTO,0, NULL);	
	espeak_Synchronize();
	return 1;
}


//terminate the current speech
void T4K_Tts_cancel()
{
	espeak_Cancel();
}

//wait till current text is spoken
void T4K_Tts_wait()
{
	while (espeak_IsPlaying() && tts_thread)
	{};
	SDL_Delay(30); 
}

//This function should be called at begining 
int T4K_Tts_init()
{
	if (text_to_speech_status)
	{
		if(espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 500, NULL, 0 ) == -1)
			return 0;
		else
			return 1;
	}
}

/*Used to set person in TTS. in the case of espeak we will set 
 * language by this. return False if language is not available  */
int T4K_Tts_set_voice(char voice_name[]){
	if (espeak_SetVoiceByName(voice_name) == EE_OK)
		return 1;
	else
		return 0;
}


//Stop the speech if it is speaking
void T4K_Tts_stop(){
	if (tts_thread)
	{
		SDL_KillThread(tts_thread);
		tts_thread = NULL;
		espeak_Cancel();
	}
}


//TTS Parameters
void T4K_Tts_set_volume(int volume){
espeak_SetParameter(espeakVOLUME,2*volume,0);
}

/* Set the rate of TTS.
 * Hear in case of espeak the rate is ranging
 * from 80 to 450. So we multiply the given
 * rate with 3.7 and add 80 to get exact
 * rate for espeak  */
void T4K_Tts_set_rate(int rate){
espeak_SetParameter(espeakRATE,(3.7*rate)+80,0);
}

void T4K_Tts_set_pitch(int pitch){
espeak_SetParameter(espeakPITCH,pitch,0);
}

/* Function used to read a text
 * 
 * DEFAULT_VALUE (30) can be passed for rate and pitch
 * 
 * if mode = INTERRUPT then terminate the currently speaking 
 * text and read the new text.
 * 
 * if mode = APPEND then wait till speaking is finished 
 * then read the new text */
void T4K_Tts_say(int rate,int pitch,int mode, const char* text, ...){
	tts_argument data_to_pass;
	
	
	if (text_to_speech_status){
	
	T4K_Tts_set_rate(rate);
    T4K_Tts_set_pitch(pitch);

	//Getting the formated text
	va_list list;
	va_start(list,text);
	vsprintf(data_to_pass.text,text,list);
	va_end(list);
	
	//Passing mode
	data_to_pass.mode = mode;
	
	//Calling threded function to say.	
	tts_thread = SDL_CreateThread(tts_thread_func, &data_to_pass);
	}
}	





#else

#include <libspeechd.h>


SPDConnection *spd_connection;
int text_to_speech_speaking;

/* TTS annoncement should be in thread otherwise 
 * it will freez the game till announcemrnt finishes */
int tts_thread_func(void *arg)
{
	tts_argument recived = *((tts_argument*)(arg));
	fprintf(stderr,"\nSPD : %s - %d",recived.text,recived.mode);
	
	text_to_speech_speaking = 0;
	
	if (recived.mode == INTERRUPT)
	{	
		T4K_Tts_cancel();
		spd_say(spd_connection, 1, recived.text);
	}
	else
	{

		
		spd_say(spd_connection, 1, recived.text);
		
	}
	
		printf("\nWaiting");
		T4K_Tts_wait();
		printf("\nWaiting over");	

	return 1;
}


//terminate the current speech
void T4K_Tts_cancel()
{
	spd_cancel(spd_connection);
}

//wait till current text is spoken
void T4K_Tts_wait()
{

	while(text_to_speech_speaking == 0)
	{
		SDL_Delay(1);
	};
	 
	text_to_speech_speaking = 1;
}

/* Callback for Speech Dispatcher notifications */
void end_of_speech(size_t msg_id, size_t client_id, SPDNotificationType type)
{
	text_to_speech_speaking = type;
}


//This function should be called at begining 
int T4K_Tts_init()
{
	if (text_to_speech_status)
	{
		spd_connection = spd_open("linux-client221", "linux connection","user_name", SPD_MODE_THREADED);
		text_to_speech_speaking = 1;
		spd_connection->callback_end = end_of_speech;
		spd_set_notification_on(spd_connection, SPD_END);
		return 1;
	}
}

/*Used to set person in TTS. in the case of espeak we will set 
 * language by this. return False if language is not available  */
int T4K_Tts_set_voice(char voice_name[]){
	if (text_to_speech_status)
	{
		spd_set_synthesis_voice(spd_connection,voice_name);
	}
	return 1;
	
}


//Stop the speech if it is speaking
void T4K_Tts_stop(){
	if (tts_thread)
	{
		SDL_KillThread(tts_thread);
		tts_thread = NULL;
		spd_cancel(spd_connection);
		text_to_speech_speaking = 1;
	}
}


//TTS Parameters
void T4K_Tts_set_volume(int volume){
	if (text_to_speech_status)
	{
		spd_set_volume(spd_connection,volume);
	}
}


void T4K_Tts_set_rate(int rate){
	if (text_to_speech_status)
	{
		spd_set_voice_rate(spd_connection,rate);
	}

}

void T4K_Tts_set_pitch(int pitch){
	if (text_to_speech_status)
	{
		spd_set_voice_pitch(spd_connection,pitch);
	}
}

/* Function used to read a text
 * 
 * DEFAULT_VALUE (30) can be passed for rate and pitch
 * 
 * if mode = INTERRUPT then terminate the currently speaking 
 * text and read the new text.
 * 
 * if mode = APPEND then wait till speaking is finished 
 * then read the new text */
void T4K_Tts_say(int rate,int pitch,int mode, const char* text, ...){
	tts_argument data_to_pass;
	
	
	if (text_to_speech_status){
	
	T4K_Tts_set_rate(rate);
    T4K_Tts_set_pitch(pitch);

	//Getting the formated text
	va_list list;
	va_start(list,text);
	vsprintf(data_to_pass.text,text,list);
	va_end(list);
	
	//Passing mode
	data_to_pass.mode = mode;
	
	//Calling threded function to say.	
	tts_thread = SDL_CreateThread(tts_thread_func, &data_to_pass);
	}
}	


#endif
