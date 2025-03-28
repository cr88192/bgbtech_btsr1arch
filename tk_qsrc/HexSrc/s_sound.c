// Emacs style mode select	 -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
// GNU General Public License for more details.
//
// $Log:$
//
// DESCRIPTION:	none
//
//-----------------------------------------------------------------------------


static const char
rcsid[] = "$Id: s_sound.c,v 1.6 1997/02/03 22:45:12 b1 Exp $";



#include <stdio.h>
#include <stdlib.h>

#if 0
#include "i_system.h"
#include "i_sound.h"
#include "sounds.h"
#include "s_sound.h"

#include "z_zone.h"
#include "m_random.h"
#include "w_wad.h"

#include "doomdef.h"
#include "p_local.h"

#include "doomstat.h"
#endif

#include "h2def.h"

#include "i_sound.h"
#include "r_local.h"


// Purpose?
const char snd_prefixen[]
= { 'P', 'P', 'A', 'S', 'S', 'S', 'M', 'M', 'M', 'S', 'S', 'S' };

#define S_MAX_VOLUME		127

// when to clip out sounds
// Does not fit the large outdoor areas.
#define S_CLIPPING_DIST		(1200*0x10000)

// Distance tp origin when sounds should be maxed out.
// This should relate to movement clipping resolution
// (see BLOCKMAP handling).
// Originally: (200*0x10000).
#define S_CLOSE_DIST		(160*0x10000)


#define S_ATTENUATOR		((S_CLIPPING_DIST-S_CLOSE_DIST)>>FRACBITS)

// Adjustable by menu.
#define NORM_VOLUME			snd_MaxVolume

#define NORM_PITCH	 		128
#define NORM_PRIORITY		64
#define NORM_SEP		128

#define S_PITCH_PERTURB		1
#define S_STEREO_SWING		(96*0x10000)

// percent attenuation from front to back
#define S_IFRACVOL		30

#define NA			0
#define S_NUMCHANNELS		2


extern sfxinfo_t S_sfx[];

// Current music/sfx card - index useless
//	w/o a reference LUT in a sound module.
extern int snd_MusicDevice;
extern int snd_SfxDevice;
// Config file? Same disclaimer as above.
extern int snd_DesiredMusicDevice;
extern int snd_DesiredSfxDevice;


#if 0
typedef struct
{
	// sound information (if null, channel avail.)
	sfxinfo_t*	sfxinfo;

	// origin of sound
	void*	origin;

	// handle of the sound being played
	int		handle;
	
} channel_t;
#endif

// the set of channels available
static channel_t*	s_channels;

// These are not used, but should be (menu).
// Maximum volume of a sound effect.
// Internal default is max out of 0-15.
int 		snd_SfxVolume = 15;

// Maximum volume of music. Useless so far.
int 		snd_MusicVolume = 15; 



// whether songs are mus_paused
static dt_bool		mus_paused;	

// music currently being played
static musicinfo_t*	mus_playing=0;

// following is set
//	by the defaults code in M_misc:
// number of channels available
int			numChannels;	

static int		nextcleanup;



//
// Internals.
//
int
S_getChannel
( void*		origin,
	sfxinfo_t*	sfxinfo );


int
S_AdjustSoundParams
( mobj_t*	listener,
	mobj_t*	source,
	int*		vol,
	int*		sep,
	int*		pitch );

void S_StopChannel(int cnum);

static int s_init_wasinit = 314;

//
// Initializes sound stuff, including volume
// Sets channels, SFX and music volume,
//	allocates channel buffer, sets S_sfx lookup.
//
void S_Init()
// ( int		sfxVolume,
//	int		musicVolume )
{	
	int		i;
	
//	if(s_init_wasinit != 314)
//		__debugbreak();
	s_init_wasinit = 1;

	if(!numChannels)
		numChannels = 8;

//	fprintf( stderr, "S_Init: default sfx volume %d\n", sfxVolume);

	// Whatever these did with DMX, these are rather dummies now.
	I_SetChannels();

	I_SetSfxVolume(8);
	
//	S_SetSfxVolume(sfxVolume);
	// No music with Linux - another dummy.
//	S_SetMusicVolume(musicVolume);

	// Allocating the internal channels for mixing
	// (the maximum numer of sounds rendered
	// simultaneously) within zone memory.
	s_channels =
//		(channel_t *) Z_Malloc(numChannels*sizeof(channel_t), PU_STATIC, 0);
		(channel_t *) Z_Malloc(10*sizeof(channel_t), PU_STATIC, 0);
	
	// Free all channels for use
//	for (i=0 ; i<numChannels ; i++)
//	for (i=0 ; i<10 ; i++)
//		s_channels[i].sfxinfo = 0;
	
	// no sounds are playing, and they are not mus_paused
	mus_paused = 0;

	// Note that sounds have not been cached (yet).
	for (i=1 ; i<NUMSFX ; i++)
		S_sfx[i].lumpnum = S_sfx[i].usefulness = -1;
}

#if 0
int spmus[]=
{
	// Song - Who? - Where?
	
	mus_e3m4,	// American	e4m1
	mus_e3m2,	// Romero	e4m2
	mus_e3m3,	// Shawn	e4m3
	mus_e1m5,	// American	e4m4
	mus_e2m7,	// Tim 	e4m5
	mus_e2m4,	// Romero	e4m6
	mus_e2m6,	// J.Anderson	e4m7 CHIRON.WAD
	mus_e2m5,	// Shawn	e4m8
	mus_e1m9	// Tim		e4m9
};
#endif

//
// Per level startup code.
// Kills playing sounds at start of level,
//	determines music if any, changes music.
//
void S_Start(void)
{
	int cnum;
	int mnum;

	if(!numChannels)
		numChannels = 8;

	// kill all playing sounds at start of level
	//	(trust me - a good idea)
	for (cnum=0 ; cnum<numChannels ; cnum++)
	{
		if (s_channels[cnum].sfxinfo)
			S_StopChannel(cnum);
	}
	
	// start new music for the level
	mus_paused = 0;

#if 0
	if (gamemode == commercial)
	mnum = mus_runnin + gamemap - 1;
	else
	{	
	if (gameepisode < 4)
		mnum = mus_e1m1 + (gameepisode-1)*9 + gamemap-1;
	else
		mnum = spmus[gamemap-1];
	}	
#endif
	
	// HACK FOR COMMERCIAL
	//	if (commercial && mnum > mus_e3m9)	
	//		mnum -= mus_e3m9;
	
//	S_ChangeMusic(mnum, true);
	
	nextcleanup = 15;
}	





void
S_StartSoundAtVolume
(	mobj_t	*origin_p,
	int		sfx_id,
	int		volume )
{

	int		rc;
	int		sep;
	int		pitch;
	int		priority;
	sfxinfo_t*	sfx;
	int		cnum;
	
	mobj_t*	origin = (mobj_t *) origin_p;
	
	
	// Debug.
//	fprintf( stderr,
//	printf(
//			 "S_StartSoundAtVolume: playing sound %d (%s) V=%d\n",
//			 sfx_id, S_sfx[sfx_id].tagName, volume );
	
	// check for bogus sound #
	if (sfx_id < 1 || sfx_id > NUMSFX)
		I_Error("Bad sfx #: %d", sfx_id);
	
	sfx = &S_sfx[sfx_id];
	
#if 0
	// Initialize sound parameters
	if (sfx->link)
	{
	pitch = sfx->pitch;
	priority = sfx->priority;
	volume += sfx->volume;
	
	if (volume < 1)
		return;
	
	if (volume > snd_SfxVolume)
		volume = snd_SfxVolume;
	}	
	else
	{
	pitch = NORM_PITCH;
	priority = NORM_PRIORITY;
	}
#endif

	pitch = NORM_PITCH;
	priority = NORM_PRIORITY;
	sep = 0;

	// Check to see if it is audible,
	//	and if not, modify the params
//	if (origin && origin != players[consoleplayer].mo)
	if (origin && (origin != players[consoleplayer].mo) && (volume<16))
	{
		rc = S_AdjustSoundParams(players[consoleplayer].mo,
					 origin,
					 &volume,
					 &sep,
					 &pitch);
		
		if ( origin->x == players[consoleplayer].mo->x
		 && origin->y == players[consoleplayer].mo->y)
		{	
			sep 	= NORM_SEP;
		}
		
		if (!rc)
			return;
	}	
	else
	{
		volume = 16;
		sep = NORM_SEP;
	}

#if 0
	// hacks to vary the sfx pitches
	if (sfx_id >= sfx_sawup
		&& sfx_id <= sfx_sawhit)
	{	
	pitch += 8 - (M_Random()&15);
	
	if (pitch<0)
		pitch = 0;
	else if (pitch>255)
		pitch = 255;
	}
	else if (sfx_id != sfx_itemup
		 && sfx_id != sfx_tink)
	{
	pitch += 16 - (M_Random()&31);
	
	if (pitch<0)
		pitch = 0;
	else if (pitch>255)
		pitch = 255;
	}
#endif

	// kill old sound
	S_StopSound(origin);

	// try to find a channel
	cnum = S_getChannel(origin, sfx);
	
	if (cnum<0)
	{
		printf("S_StartSoundAtVolume: No Free Channels\n");
		return;
	}

	//
	// This is supposed to handle the loading/caching.
	// For some odd reason, the caching is done nearly
	//	each time the sound is needed?
	//
	
	// get lumpnum if necessary
	if (sfx->lumpnum < 0)
	{
		sfx->lumpnum = I_GetSfxLumpNum(sfx);
		if(sfx->lumpnum<0)
		{
			printf("S_StartSoundAtVolume: GetSfxLumpNum failed\n");
			return;
		}
	}

#ifndef SNDSRV
	// cache data if necessary
	if (!sfx->snd_ptr)
	{
	//		fprintf( stderr,
	//			 "S_StartSoundAtVolume: 16bit and not pre-cached - wtf?\n");

		// DOS remains, 8bit handling
		if(sfx->lumpnum>0)
			sfx->snd_ptr = (void *) W_CacheLumpNum(sfx->lumpnum, PU_MUSIC);

		// fprintf( stderr,
		//		 "S_StartSoundAtVolume: loading %d (lump %d) : 0x%x\n",
		//		 sfx_id, sfx->lumpnum, (int)sfx->data );
	
	}
#endif
	
	// increase the usefulness
	if (sfx->usefulness++ < 0)
		sfx->usefulness = 1;
	
	s_channels[cnum].sfxinfo = sfx;
	
	// Assigns the handle to one of the channels in the
	//	mix/output buffer.
	s_channels[cnum].handle = I_StartSound(sfx_id,
						 /*sfx->data,*/
						 volume,
						 sep,
						 pitch,
						 priority);

//	printf("S_StartSoundAtVolume: OK %d/%d\n",
//		cnum, s_channels[cnum].handle);
}	

void
S_StartSound
(	mobj_t	*origin,
	int		sfx_id )
{
	S_StartSoundAtVolume(origin, sfx_id, snd_SfxVolume); 
}


void S_StopSound(mobj_t *origin)
{

	int cnum;

	for (cnum=0 ; cnum<numChannels ; cnum++)
	{
//		if (s_channels[cnum].sfxinfo && s_channels[cnum].origin == origin)
		if (s_channels[cnum].sfxinfo && (s_channels[cnum].mo == origin))
		{
			S_StopChannel(cnum);
			break;
		}
	}
}


//
// Stop and resume music, during game PAUSE.
//
void S_PauseSound(void)
{
	if (mus_playing && !mus_paused)
	{
//		I_PauseSong(mus_playing->handle);
		mus_paused = true;
	}
}

void S_ResumeSound(void)
{
	if (mus_playing && mus_paused)
	{
//		I_ResumeSong(mus_playing->handle);
		mus_paused = false;
	}
}


//
// Updates music & sounds
//
void S_UpdateSounds(mobj_t* listener_p)
{
	int		audible;
	int		cnum;
	int		volume;
	int		sep;
	int		pitch;
	sfxinfo_t*	sfx;
	channel_t*	c;
	
	mobj_t*	listener = (mobj_t*)listener_p;
	
	for (cnum=0 ; cnum<numChannels ; cnum++)
	{
	c = &s_channels[cnum];
	sfx = c->sfxinfo;

	if (c->sfxinfo)
	{
		if (I_SoundIsPlaying(c->handle))
		{
			// initialize parameters
			volume = snd_SfxVolume;
			pitch = NORM_PITCH;
			sep = NORM_SEP;

#if 0
			if (sfx->link)
			{
				pitch = sfx->pitch;
				volume += sfx->volume;
				if (volume < 1)
				{
				S_StopChannel(cnum);
				continue;
				}
				else if (volume > snd_SfxVolume)
				{
				volume = snd_SfxVolume;
				}
			}
#endif

			// check non-local sounds for distance clipping
			//	or modify their params
	//		if (c->origin && listener_p != c->origin)
			if (c->mo && listener_p != c->mo)
			{
				audible = S_AdjustSoundParams(listener,
	//							c->origin,
								c->mo,
								&volume,
								&sep,
								&pitch);
				
				if (!audible)
				{
//					printf("S_UpdateSounds: Inaudible\n");
					S_StopChannel(cnum);
				}
				else
					I_UpdateSoundParams(c->handle, volume, sep, pitch);
			}
		}
		else
		{
//			printf("S_UpdateSounds: Done Playing\n");
			// if channel is allocated but sound has stopped,
			//	free it
			S_StopChannel(cnum);
		}
	}
	}
	// kill music if it is a single-play && finished
	// if (	mus_playing
	//		&& !I_QrySongPlaying(mus_playing->handle)
	//		&& !mus_paused )
	// S_StopMusic();
}

#if 0
void S_SetMusicVolume(int volume)
{
	if (volume < 0 || volume > 127)
	{
	I_Error("Attempt to set music volume at %d",
		volume);
	}	

	I_SetMusicVolume(127);
	I_SetMusicVolume(volume);
	snd_MusicVolume = volume;
}
#endif


void S_SetSfxVolume(int volume)
{

	if (volume < 0 || volume > 127)
	I_Error("Attempt to set sfx volume at %d", volume);

	snd_SfxVolume = volume;

}

#if 0

//
// Starts some music with the music id found in sounds.h.
//
void S_StartMusic(int m_id)
{
	S_ChangeMusic(m_id, false);
}

void
S_ChangeMusic
( int			musicnum,
	int			looping )
{
	musicinfo_t*	music;
	char		namebuf[9];

	if ( (musicnum <= mus_None)
	 || (musicnum >= NUMMUSIC) )
	{
	I_Error("Bad music number %d", musicnum);
	}
	else
	music = &S_music[musicnum];

	if (mus_playing == music)
	return;

	// shutdown old music
	S_StopMusic();

	if(gamemode==heretic)
	{
		if (music->lumpnum<=0)
		{
			sprintf(namebuf, "mus_%s", music->name);
			music->lumpnum = W_GetNumForName(namebuf);
		}
	}

	// get lumpnum if neccessary
//	if (!music->lumpnum)
	if (music->lumpnum<=0)
	{
		sprintf(namebuf, "d_%s", music->name);
		music->lumpnum = W_GetNumForName(namebuf);
	}

	// load & register it
	music->data = (void *) W_CacheLumpNum(music->lumpnum, PU_MUSIC);
	music->handle = I_RegisterSong(music->data);

	// play it
	I_PlaySong(music->handle, looping);

	mus_playing = music;
}


void S_StopMusic(void)
{
	if (mus_playing)
	{
		if (mus_paused)
			I_ResumeSong(mus_playing->handle);

		I_StopSong(mus_playing->handle);
		I_UnRegisterSong(mus_playing->handle);

		if(mus_playing->data)
			Z_ChangeTag(mus_playing->data, PU_CACHE);
		
		mus_playing->data = 0;
		mus_playing = 0;
	}
}
#endif

void S_SetMusicVolume(void)
{
	I_SetMusicVolume(snd_MusicVolume);
}


void S_StopChannel(int cnum)
{

	int		i;
	channel_t*	c = &s_channels[cnum];

	if (c->sfxinfo)
	{
		// stop the sound playing
		if (I_SoundIsPlaying(c->handle))
		{
	#ifdef SAWDEBUG
			if (c->sfxinfo == &S_sfx[sfx_sawful])
			fprintf(stderr, "stopped\n");
	#endif
			I_StopSound(c->handle);
		}

		// check to see
		//	if other channels are playing the sound
		for (i=0 ; i<numChannels ; i++)
		{
			if (cnum != i
			&& c->sfxinfo == s_channels[i].sfxinfo)
			{
			break;
			}
		}
		
		// degrade usefulness of sound data
		c->sfxinfo->usefulness--;

		c->mo = NULL;
		c->sfxinfo = 0;
	}
}



//
// Changes volume, stereo-separation, and pitch variables
//	from the norm of a sound effect to be played.
// If the sound is not audible, returns a 0.
// Otherwise, modifies parameters and returns 1.
//
int
S_AdjustSoundParams
( mobj_t*	listener,
	mobj_t*	source,
	int*		vol,
	int*		sep,
	int*		pitch )
{
	fixed_t	approx_dist;
	fixed_t	adx;
	fixed_t	ady;
	angle_t	angle;

	// calculate the distance to sound origin
	//	and clip it if necessary
	adx = abs(listener->x - source->x);
	ady = abs(listener->y - source->y);

	// From _GG1_ p.428. Appox. eucledian distance fast.
	approx_dist = adx + ady - ((adx < ady ? adx : ady)>>1);
	
	if (gamemap != 8
		&& approx_dist > S_CLIPPING_DIST)
	{
		return 0;
	}
	
	// angle of source to listener
	angle = R_PointToAngle2(listener->x,
				listener->y,
				source->x,
				source->y);

	if (angle > listener->angle)
	angle = angle - listener->angle;
	else
	angle = angle + (0xffffffff - listener->angle);

	angle >>= ANGLETOFINESHIFT;

	// stereo separation
	*sep = 128 - (FixedMul(S_STEREO_SWING,finesine[angle])>>FRACBITS);

	// volume calculation
	if (approx_dist < S_CLOSE_DIST)
	{
		*vol = snd_SfxVolume;
	}
	else if (gamemap == 8)
	{
		if (approx_dist > S_CLIPPING_DIST)
			approx_dist = S_CLIPPING_DIST;

		*vol = 15+ ((snd_SfxVolume-15)
				*((S_CLIPPING_DIST - approx_dist)>>FRACBITS))
			/ S_ATTENUATOR;
	}
	else
	{
		// distance effect
		*vol = (snd_SfxVolume
			* ((S_CLIPPING_DIST - approx_dist)>>FRACBITS))
			/ S_ATTENUATOR; 
	}
	
	return (*vol > 0);
}


//
// S_getChannel :
//	 If none available, return -1.	Otherwise channel #.
//
int
S_getChannel(
	void		*origin,
	sfxinfo_t	*sfxinfo )
{
	// channel number to use
	int		cnum;
	
	channel_t*	c;

	// Find an open channel
	for (cnum=0 ; cnum<numChannels ; cnum++)
	{
		if (!s_channels[cnum].sfxinfo)
			break;

//		else if (origin &&	s_channels[cnum].origin ==	origin)
		if (origin &&	s_channels[cnum].mo ==	origin)
		{
			S_StopChannel(cnum);
			break;
		}
	}

	// None available
	if (cnum == numChannels)
	{
		// Look for lower priority
		for (cnum=0 ; cnum<numChannels ; cnum++)
			if (s_channels[cnum].sfxinfo->priority >= sfxinfo->priority)
				break;

		if (cnum == numChannels)
		{
			// FUCK!	No lower priority.	Sorry, Charlie.	
			return -1;
		}
		else
		{
			// Otherwise, kick out lower priority.
			S_StopChannel(cnum);
		}
	}

	c = &s_channels[cnum];

	// channel is decided to be cnum.
	c->sfxinfo = sfxinfo;
//	c->origin = origin;
	c->mo = origin;

	return cnum;
}


void S_StopAllSound(void)
{
}

int song_lumpnum;
int song_handle;
byte *song_data;

void S_StopMusic(void)
{
	if (song_handle>0)
	{
		if (mus_paused)
			I_ResumeSong(song_handle);

		I_StopSong(song_handle);
		I_UnRegisterSong(song_handle);

		if(song_data)
			Z_ChangeTag(song_data, PU_CACHE);
		
		song_data = 0;
		song_handle = 0;
	}
}

void S_StartSongName(char *songLump, dt_bool loop)
{
	int lumpnum;

	S_StopMusic();

	printf("S_StartSongName: %s\n", songLump);

	lumpnum = W_GetNumForName(songLump);
	song_lumpnum=lumpnum;

	// load & register it
	song_data = (void *) W_CacheLumpNum(lumpnum, PU_MUSIC);
	song_handle = I_RegisterSong(song_data);

	// play it
	I_PlaySong(song_handle, loop);

//	mus_playing = music;
}

void S_StartSong(int song, dt_bool loop)
{
	char *str;
	
	str=P_GetMapSongLump(song);
	if(!str)
		str="chess";

	S_StartSongName(str, loop);
}


int S_GetSoundID(char *name)
{
	int i;

	for(i=0; i<NUMSFX; i++)
	{
		if(!strcmp(S_sfx[i].tagName, name))
		{
			return(i);
		}
	}
	
	return(-1);
}

dt_bool S_GetSoundPlayingInfo(mobj_t *mobj, int sound_id)
{
	return(false);
}

void S_GetChannelInfo(SoundInfo_t *s)
{
	sfxinfo_t	*sfx;
	channel_t	*c;
	ChanInfo_t	*ci;
	int i, j, k;

	s->channelCount=numChannels;

	for(i=0; i<numChannels; i++)
	{
//		c = &s_channels[i];
		c = s_channels+i;
		sfx = c->sfxinfo;
		ci = (s->chan)+i;

		if(ci->mo && !sfx)
		{
//			__debugbreak();

			ci->mo = NULL;
			continue;
		}

		ci->id = c->sound_id;
		ci->mo = c->mo;
		ci->priority = c->priority;
//		ci->name = sfx->lumpname;
		ci->name = sfx->tagName;
		ci->distance = 0;
//		__debugbreak();

		if(ci->mo && !ci->name)
		{
			__debugbreak();
		}

	}
}
