#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "rt_def.h"

#include "rt_sound.h"
#include "fx_man.h"
#include "music.h"


char *FX_ErrorString( int ErrorNumber )
{
}

int   FX_SetupCard( int SoundCard, fx_device *device )
{
}

int   FX_GetBlasterSettings( fx_blaster_config *blaster )
{
}

int   FX_SetupSoundBlaster( fx_blaster_config blaster, int *MaxVoices, int *MaxSampleBits, int *MaxChannels )
{
}

int   FX_Init( int SoundCard, int numvoices, int numchannels, int samplebits, unsigned mixrate )
{
}

int   FX_Shutdown( void )
{
}

int   FX_SetCallBack( void ( *function )( unsigned long ) )
{
}

void  FX_SetVolume( int volume )
{
}

int   FX_GetVolume( void )
{
}


void  FX_SetReverseStereo( int setting )
{
}

int   FX_GetReverseStereo( void )
{
}

void  FX_SetReverb( int reverb )
{
}

void  FX_SetFastReverb( int reverb )
{
}

int   FX_GetMaxReverbDelay( void )
{
}

int   FX_GetReverbDelay( void )
{
}

void  FX_SetReverbDelay( int delay )
{
}


int FX_VoiceAvailable( int priority )
{
}

int FX_EndLooping( int handle )
{
}

int FX_SetPan( int handle, int vol, int left, int right )
{
}

int FX_SetPitch( int handle, int pitchoffset )
{
}

int FX_SetFrequency( int handle, int frequency )
{
}


int FX_PlayVOC( char *ptr, int pitchoffset, int vol, int left, int right,
       int priority, unsigned long callbackval )
{
}

int FX_PlayLoopedVOC( char *ptr, long loopstart, long loopend,
       int pitchoffset, int vol, int left, int right, int priority,
       unsigned long callbackval )
{
}

int FX_PlayWAV( char *ptr, int pitchoffset, int vol, int left, int right,
       int priority, unsigned long callbackval )
{
}

int FX_PlayLoopedWAV( char *ptr, long loopstart, long loopend,
       int pitchoffset, int vol, int left, int right, int priority,
       unsigned long callbackval )
{
}

int FX_PlayVOC3D( char *ptr, int pitchoffset, int angle, int distance,
       int priority, unsigned long callbackval )
{
}

int FX_PlayWAV3D( char *ptr, int pitchoffset, int angle, int distance,
       int priority, unsigned long callbackval )
{
}

int FX_PlayRaw( char *ptr, unsigned long length, unsigned rate,
       int pitchoffset, int vol, int left, int right, int priority,
       unsigned long callbackval )
{
}

int FX_PlayLoopedRaw( char *ptr, unsigned long length, char *loopstart,
       char *loopend, unsigned rate, int pitchoffset, int vol, int left,
       int right, int priority, unsigned long callbackval )
{
}

int FX_Pan3D( int handle, int angle, int distance )
{
}

int FX_SoundActive( int handle )
{
}

int FX_SoundsPlaying( void )
{
}

int FX_StopSound( int handle )
{
}

int FX_StopAllSounds( void )
{
}

int FX_StartDemandFeedPlayback( void ( *function )( char **ptr, unsigned long *length ),
       int rate, int pitchoffset, int vol, int left, int right,
       int priority, unsigned long callbackval )
{
}

int  FX_StartRecording( int MixRate, void ( *function )( char *ptr, int length ) )
{
}

void FX_StopRecord( void )
{
}




char *MUSIC_ErrorString( int ErrorNumber )
{
}

int   MUSIC_Init( int SoundCard, int Address )
{
}

int   MUSIC_Shutdown( void )
{
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
}

void  MUSIC_SetLoopFlag( int loopflag )
{
}

int   MUSIC_SongPlaying( void )
{
}

void  MUSIC_Continue( void )
{
}

void  MUSIC_Pause( void )
{
}

int   MUSIC_StopSong( void )
{
}

int   MUSIC_PlaySong( unsigned char *song, int loopflag )
{
}

void  MUSIC_SetContext( int context )
{
}

int   MUSIC_GetContext( void )
{
}

void  MUSIC_SetSongTick( unsigned long PositionInTicks )
{
}

void  MUSIC_SetSongTime( unsigned long milliseconds )
{
}

void  MUSIC_SetSongPosition( int measure, int beat, int tick )
{
}

void  MUSIC_GetSongPosition( songposition *pos )
{
}

void  MUSIC_GetSongLength( songposition *pos )
{
}

int   MUSIC_FadeVolume( int tovolume, int milliseconds )
{
}

int   MUSIC_FadeActive( void )
{
}

void  MUSIC_StopFade( void )
{
}

void  MUSIC_RerouteMidiChannel( int channel, int (*function)( int event, int c1, int c2 ) )
{
}

void  MUSIC_RegisterTimbreBank( unsigned char *timbres )
{
}


