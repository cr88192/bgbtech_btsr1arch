#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "rt_def.h"

#include "w_wad.h"
#include "z_zone.h"

#include "rt_sound.h"
#include "fx_man.h"
#include "music.h"


int snd_MusicVolume;

int song_lumpnum;
int song_handle;
byte *song_data;



char *MUSIC_ErrorString( int ErrorNumber )
{
	return("-");
}

int   MUSIC_Init( int SoundCard, int Address )
{
	return(MUSIC_Ok);
}

int   MUSIC_Shutdown( void )
{
	return(0);
}

void  MUSIC_SetMaxFMMidiChannel( int channel )
{
}

void  MUSIC_SetVolume( int volume )
{
	snd_MusicVolume = volume;
}

void  MUSIC_SetMidiChannelVolume( int channel, int volume )
{
}

void  MUSIC_ResetMidiChannelVolumes( void )
{
}

int   MUSIC_GetVolume( void )
{
	return(snd_MusicVolume);
}

void  MUSIC_SetLoopFlag( int loopflag )
{
}

#if 1
int   MUSIC_SongPlaying( void )
{
	return(song_handle);
}

void  MUSIC_Continue( void )
{
}

void  MUSIC_Pause( void )
{
}

int   MUSIC_StopSong( void )
{
	if (song_handle>0)
	{
//		if (mus_paused)
//			I_ResumeSong(song_handle);

		I_StopSong(song_handle);
		I_UnRegisterSong(song_handle);

		if(song_data)
			Z_ChangeTag(song_data, PU_CACHE);
		
		song_data = 0;
		song_handle = 0;
	}
	return(0);
}

int   MUSIC_PlaySong( unsigned char *song, int loopflag )
{
	int lumpnum, sz;

	MUSIC_StopSong();

	printf("S_StartSongName: %s\n", song);

//	lumpnum = W_GetNumForName(song);
	lumpnum = W_CheckNumForName(song);
	if(lumpnum<=0)
	{
		printf("S_StartSongName: No Song %s\n", song);
		return(0);
	}
	
	song_lumpnum=lumpnum;
	sz=W_LumpLength (lumpnum);

	printf("S_StartSongName: Size=%d\n", sz);

	// load & register it
//	song_data = (void *) W_CacheLumpNum(lumpnum, PU_MUSIC);
	song_data = (void *) W_CacheLumpNum(lumpnum, PU_STATIC);
	song_handle = I_RegisterSong(song_data, sz);

	// play it
	I_PlaySong(song_handle, loopflag);

//	mus_playing = music;
	return(song_handle);
}
#endif

#if 0
void  MUSIC_SetContext( int context )
{
}

int   MUSIC_GetContext( void )
{
	return(0);
}
#endif

#if 0
void  MUSIC_SetSongTick( unsigned long PositionInTicks )
{
}
#endif

#if 1
void  MUSIC_SetSongTime( unsigned long milliseconds )
{
}

void  MUSIC_SetSongPosition( int measure, int beat, int tick )
{
}

void  MUSIC_GetSongPosition( songposition *pos )
{
}
#endif

#if 0
void  MUSIC_GetSongLength( songposition *pos )
{
}
#endif

int   MUSIC_FadeVolume( int tovolume, int milliseconds )
{
	return(0);
}

int   MUSIC_FadeActive( void )
{
	return(0);
}

#if 0
void  MUSIC_StopFade( void )
{
}
#endif

#if 0
void  MUSIC_RerouteMidiChannel( int channel, int (*function)( int event, int c1, int c2 ) )
{
}

void  MUSIC_RegisterTimbreBank( unsigned char *timbres )
{
}
#endif

