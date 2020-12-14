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

// #include "i_system.h"
//#include "i_sound.h"
//#include "sounds.h"
//#include "s_sound.h"

// #include "z_zone.h"
//#include "m_random.h"
//#include "w_wad.h"

#include "doomdef.h"
// #include "p_local.h"
// #include "doomstat.h"

extern sfxinfo_t S_sfx[];
int 		lengths[NUMSFX];

void S_StopMusic(void);


// Purpose?
// const char snd_prefixen[]
// = { 'P', 'P', 'A', 'S', 'S', 'S', 'M', 'M', 'M', 'S', 'S', 'S' };

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
static boolean		mus_paused;	

// music currently being played
static musicinfo_t*	mus_playing=0;

// following is set
//	by the defaults code in M_misc:
// number of channels available
int			numChannels;	

static int		nextcleanup;


void S_StopSound(void *origin);


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
void S_Init
( int		sfxVolume,
	int		musicVolume )
{	
	int		i;
	
//	if(s_init_wasinit != 314)
//		__debugbreak();

	s_init_wasinit = 1;

	fprintf( stderr, "S_Init: default sfx volume %d\n", sfxVolume);

	I_StartupSound();
	
	numChannels = 8;

	// Whatever these did with DMX, these are rather dummies now.
	I_SetChannels();
	
	S_SetSfxVolume(sfxVolume);
	// No music with Linux - another dummy.
	S_SetMusicVolume(musicVolume);

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

#if 1
	// kill all playing sounds at start of level
	//	(trust me - a good idea)
	for (cnum=0 ; cnum<numChannels ; cnum++)
	{
//		if (s_channels[cnum].sfxinfo)
		if (s_channels[cnum].sound_id>0)
			S_StopChannel(cnum);
	}
#endif

#if 1
	// start new music for the level
	mus_paused = 0;
	
//	if (gamemode == commercial)
//	mnum = mus_runnin + gamemap - 1;
//	else
//	{	
//	if (gameepisode < 4)
//		mnum = mus_e1m1 + (gameepisode-1)*9 + gamemap-1;
//	else
//		mnum = spmus[gamemap-1];
//	}	

	mnum = 0 + (gameepisode-1)*9 + gamemap-1;
	
	// HACK FOR COMMERCIAL
	//	if (commercial && mnum > mus_e3m9)	
	//		mnum -= mus_e3m9;
	
	S_ChangeMusic(mnum, true);
	
	nextcleanup = 15;
#endif
}	




void	*I_GetSfxLump( int sfxlump, int *len );

void
S_StartSoundAtVolume
( void*		origin_p,
	int		sfx_id,
	int		volume )
{
#if 1
	int		rc;
	int		sep;
	int		pitch;
	int		priority;
	sfxinfo_t*	sfx;
	int		cnum;
	int		len;
	
	mobj_t*	origin = (mobj_t *) origin_p;
	
	
	// Debug.
//	printf(
//			 "S_StartSoundAtVolume: playing sound %d (%s)\n",
//			 sfx_id, S_sfx[sfx_id].name );
	
	// check for bogus sound #
	if (sfx_id < 1 || sfx_id > NUMSFX)
	I_Error("Bad sfx #: %d", sfx_id);
	
	sfx = &S_sfx[sfx_id];
	
	// Initialize sound parameters
#if 0
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


	// Check to see if it is audible,
	//	and if not, modify the params
	if (origin && origin != players[consoleplayer].mo)
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
//		printf("S_StartSoundAtVolume: No Channel\n");
		return;
	}

	//
	// This is supposed to handle the loading/caching.
	// For some odd reason, the caching is done nearly
	//	each time the sound is needed?
	//
	
	// get lumpnum if necessary
	if (sfx->lumpnum < 0)
		sfx->lumpnum = I_GetSfxLumpNum(sfx);

	if (!sfx->snd_ptr)
	{
		if(sfx->lumpnum>0)
		{
#if 0
			len = W_LumpLength(sfx->lumpnum);
//			len = ((len-8)/512)*512;
//			len = (((len-8)+511)/512)*512;
			len = ((len-8)+511)&(~511);
//			lengths[sfx-S_sfx] = len;
			lengths[sfx_id] = len;
			
			sfx->snd_ptr = (void *) W_CacheLumpNum(sfx->lumpnum, PU_MUSIC);	
#endif

			sfx->snd_ptr = I_GetSfxLump(sfx->lumpnum, &(lengths[sfx_id]));
		}
	}

	// increase the usefulness
	if (sfx->usefulness++ < 0)
		sfx->usefulness = 1;
	
	// Assigns the handle to one of the channels in the
	//	mix/output buffer.
	s_channels[cnum].handle = I_StartSound(sfx_id,
						 /*sfx->data,*/
						 volume,
						 sep,
						 pitch,
						 priority);
#endif

}	

void
S_StartSound
( void*		origin,
	int		sfx_id )
{	
	S_StartSoundAtVolume(origin, sfx_id, snd_SfxVolume); 
}


void S_StopSound(void *origin)
{
#if 1
	int cnum;

	for (cnum=0 ; cnum<numChannels ; cnum++)
	{
//		if (s_channels[cnum].sfxinfo && s_channels[cnum].origin == origin)
		if ((s_channels[cnum].sound_id>0) &&
			s_channels[cnum].mo == ((mobj_t *)origin))
		{
			S_StopChannel(cnum);
			break;
		}
	}
#endif
}


//
// Stop and resume music, during game PAUSE.
//
void S_PauseSound(void)
{
#if 0
	if (mus_playing && !mus_paused)
	{
		I_PauseSong(mus_playing->handle);
		mus_paused = true;
	}
#endif
}

void S_ResumeSound(void)
{
#if 0
	if (mus_playing && mus_paused)
	{
	I_ResumeSong(mus_playing->handle);
	mus_paused = false;
	}
#endif
}


//
// Updates music & sounds
//
void S_UpdateSounds(void* listener_p)
{
#if 0
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
//		sfx = c->sfxinfo;

		if (c->sound_id <= 0)
			continue;

		sfx = &(S_sfx[c->sound_id]);

	//	if (c->sfxinfo)
		if(sfx)
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
//					pitch = sfx->pitch;
					pitch = 128;
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
//				if (c->origin && listener_p != c->origin)
				if (c->mo && ((mobj_t *)listener_p) != c->mo)
				{
					audible = S_AdjustSoundParams(listener,
//									c->origin,
									c->mo,
									&volume,
									&sep,
									&pitch);
					
					if (!audible)
					{
						S_StopChannel(cnum);
					}
					else
						I_UpdateSoundParams(c->handle, volume, sep, pitch);
				}
			}
			else
			{
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
#endif
}

#if 1
int S_SetMusicVolume(int volume)
{
#if 0
	if (volume < 0 || volume > 127)
	{
	I_Error("Attempt to set music volume at %d",
		volume);
	}	

	I_SetMusicVolume(127);
	I_SetMusicVolume(volume);
	snd_MusicVolume = volume;
#endif
}
#endif


#if 1
int S_SetSfxVolume(int volume)
{
//	if (volume < 0 || volume > 127)
//	I_Error("Attempt to set sfx volume at %d", volume);
//	snd_SfxVolume = volume;
}
#endif

//
// Starts some music with the music id found in sounds.h.
//
void S_StartMusic(int m_id)
{
	S_ChangeMusic(m_id, false);
}

extern musicinfo_t S_music[];

#if 1
int
S_ChangeMusic
( int			musicnum,
	int			looping )
{
#if 1
	musicinfo_t*	music;
	char		namebuf[9];

//	if ( (musicnum <= mus_None)
//	 || (musicnum >= NUMMUSIC) )
//	{
//	I_Error("Bad music number %d", musicnum);
//	}
//	else

	music = &S_music[musicnum];

	if (mus_playing == music)
		return(0);

	// shutdown old music
	S_StopMusic();

//	if(gamemode==heretic)
//	{
//		if (music->lumpnum<=0)
//		{
//			sprintf(namebuf, "mus_%s", music->name);
//			music->lumpnum = W_GetNumForName(namebuf);
//		}
//	}

	// get lumpnum if neccessary
//	if (!music->lumpnum)
	if (music->lumpnum<=0)
	{
//		sprintf(namebuf, "d_%s", music->name);
		sprintf(namebuf, "%s", music->name);
		music->lumpnum = W_GetNumForName(namebuf);
		
		if(music->lumpnum<=0)
		{
			printf("No Music %s\n", namebuf);
		}
	}

	// load & register it
	music->data = (void *) W_CacheLumpNum(music->lumpnum, PU_MUSIC);
	music->handle = I_RegisterSong(music->data);

	// play it
	I_PlaySong(music->handle, looping);

	mus_playing = music;
#endif
}
#endif


void S_StopMusic(void)
{
#if 1
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
#endif
}

void S_StopChannel(int cnum)
{
#if 1
	int		i;
	channel_t	*c;
	sfxinfo_t	*sfx;

	c = &s_channels[cnum];
	if(c->sound_id<=0)
		return;

	sfx = &(S_sfx[c->sound_id]);

	// stop the sound playing
	if (I_SoundIsPlaying(c->handle))
	{
		I_StopSound(c->handle);
	}

	// check to see
	//	if other channels are playing the sound
	for (i=0 ; i<numChannels ; i++)
	{
//		if ((cnum != i) && c->sfxinfo == s_channels[i].sfxinfo)
		if ((cnum != i) && c->sound_id == s_channels[i].sound_id)
		{
			break;
		}
	}
	
	// degrade usefulness of sound data
//	c->sfxinfo->usefulness--;
	sfx->usefulness--;
//	c->sfxinfo = 0;
	c->sound_id = 0;
#endif
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
	
//	if (gamemap != 8
//		&& approx_dist > S_CLIPPING_DIST)
	if (approx_dist > S_CLIPPING_DIST)
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
#if 0
	else if (gamemap == 8)
	{
		if (approx_dist > S_CLIPPING_DIST)
			approx_dist = S_CLIPPING_DIST;

//		*vol = 15+ ((snd_SfxVolume-15)
//				*((S_CLIPPING_DIST - approx_dist)>>FRACBITS))
//			/ S_ATTENUATOR;

		*vol = 15 + M_SoftDivS( ((snd_SfxVolume-15) *
			((S_CLIPPING_DIST - approx_dist)>>FRACBITS)),
			S_ATTENUATOR);
	}
#endif
	else
	{
		// distance effect
//		*vol = (snd_SfxVolume
//			* ((S_CLIPPING_DIST - approx_dist)>>FRACBITS))
//			/ S_ATTENUATOR; 
		*vol = M_SoftDivS( (snd_SfxVolume *
			((S_CLIPPING_DIST - approx_dist)>>FRACBITS)),
			S_ATTENUATOR);
	}
	
	return (*vol > 0);
}


//
// S_getChannel :
//	 If none available, return -1.	Otherwise channel #.
//
int
S_getChannel
( void*		origin,
	sfxinfo_t*	sfxinfo )
{
#if 1
	// channel number to use
	int		cnum;	
	channel_t*	c;
	sfxinfo_t*	sfx;

	// Find an open channel
	for (cnum=0 ; cnum<numChannels ; cnum++)
	{
		if (s_channels[cnum].sound_id <= 0)
			break;

		sfx = &(S_sfx[s_channels[cnum].sound_id]);

//		if (!s_channels[cnum].sfxinfo)
//			break;

//		if (origin &&	s_channels[cnum].origin ==	origin)
		if (origin && (s_channels[cnum].mo == ((mobj_t *)origin)))
		{
			S_StopChannel(cnum);
			break;
		}
		
		if (!I_SoundIsPlaying(s_channels[cnum].handle))
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
		{
			sfx = &(S_sfx[s_channels[cnum].sound_id]);
//			if (s_channels[cnum].sfxinfo->priority >= sfxinfo->priority)
			if (sfx->priority >= sfxinfo->priority)
				break;
		}

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
//	c->sfxinfo = sfxinfo;
	c->sound_id = sfxinfo - S_sfx;
//	c->origin = origin;
	c->mo = origin;

	return cnum;
#endif
}

