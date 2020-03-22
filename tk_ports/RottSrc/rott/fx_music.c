#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "rt_def.h"

#include "rt_sound.h"
#include "fx_man.h"
#include "music.h"



char *MUSIC_ErrorString( int ErrorNumber )
{
	return(NULL);
}

int   MUSIC_Init( int SoundCard, int Address )
{
	return(0);
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
}

void  MUSIC_SetMidiChannelVolume( int channel, int volume )
{
}

void  MUSIC_ResetMidiChannelVolumes( void )
{
}

int   MUSIC_GetVolume( void )
{
	return(0);
}

void  MUSIC_SetLoopFlag( int loopflag )
{
}

#if 1
int   MUSIC_SongPlaying( void )
{
	return(0);
}

void  MUSIC_Continue( void )
{
}

void  MUSIC_Pause( void )
{
}

int   MUSIC_StopSong( void )
{
	return(0);
}

int   MUSIC_PlaySong( unsigned char *song, int loopflag )
{
	return(0);
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

