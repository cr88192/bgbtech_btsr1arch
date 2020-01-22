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
// DESCRIPTION:
//	System interface for sound.
//
//-----------------------------------------------------------------------------

static const char
rcsid[] = "$Id: i_unix.c,v 1.5 1997/02/03 22:45:10 b1 Exp $";

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <math.h>

// Timer stuff. Experimental.
#include <time.h>
#include <signal.h>

#include "z_zone.h"

#include "i_system.h"
#include "i_sound.h"
#include "m_argv.h"
#include "m_misc.h"
#include "w_wad.h"

#include "doomdef.h"

static int flag = 0;

// The number of internal mixing channels,
//	the samples calculated for each mixing step,
//	the size of the 16bit, 2 hardware channel (stereo)
//	mixing buffer, and the samplerate of the raw data.

// Needed for calling the actual sound output.
#define SAMPLECOUNT		512
// #define SAMPLECOUNT		2048
// #define SAMPLECOUNT		1024
#define NUM_CHANNELS		8
// It is 2 for 16bit, and 2 for two channels.
#define BUFMUL					4
#define MIXBUFFERSIZE		(SAMPLECOUNT*BUFMUL)

#define SAMPLERATE		11025	// Hz
// #define SAMPLERATE		16000	// Hz
#define SAMPLESIZE		2	 	// 16bit

// The actual lengths of all sound effects.
int 		lengths[NUMSFX];

// The actual output device.
int	audio_fd;

// The global mixing buffer.
// Basically, samples from all active internal channels
//	are modifed and added, and stored in the buffer
//	that is submitted to the audio device.
signed short	mixbuffer[MIXBUFFERSIZE];


// The channel step amount...
unsigned int	channelstep[NUM_CHANNELS];
// ... and a 0.16 bit remainder of last step.
unsigned int	channelstepremainder[NUM_CHANNELS];


// The channel data pointers, start and end.
unsigned char*	channels[NUM_CHANNELS];
unsigned char*	channelsend[NUM_CHANNELS];


// Time/gametic that the channel started playing,
//	used to determine oldest, which automatically
//	has lowest priority.
// In case number of active sounds exceeds
//	available channels.
int		channelstart[NUM_CHANNELS];

// The sound in channel handles,
//	determined on registration,
//	might be used to unregister/stop/modify,
//	currently unused.
int 		channelhandles[NUM_CHANNELS];

// SFX id of the playing sound effect.
// Used to catch duplicates (like chainsaw).
int		channelids[NUM_CHANNELS];			

// Pitch to stepping lookup, unused.
int		steptable[256];

// Volume lookups.
int		vol_lookup[128*256];

// Hardware left and right channel volume lookup.
int*		channelleftvol_lookup[NUM_CHANNELS];
int*		channelrightvol_lookup[NUM_CHANNELS];

int			channelleftvol[NUM_CHANNELS];
int			channelrightvol[NUM_CHANNELS];


//
// This function loads the sound data from the WAD lump,
//	for single sound.
//
void*
getsfx
( char*		 sfxname,
	int*			len )
{
	unsigned char*		sfx;
	unsigned char*		paddedsfx;
	int				 i;
	int				 size;
	int				 paddedsize;
	char				name[40];
	int				 sfxlump;

	
	// Get the sound data from the WAD, allocate lump
	//	in zone memory.
	sprintf(name, "ds%s", sfxname);

	// Now, there is a severe problem with the
	//	sound handling, in it is not (yet/anymore)
	//	gamemode aware. That means, sounds from
	//	DOOM II will be requested even with DOOM
	//	shareware.
	// The sound list is wired into sounds.c,
	//	which sets the external variable.
	// I do not do runtime patches to that
	//	variable. Instead, we will use a
	//	default sound for replacement.
	if ( W_CheckNumForName(name) == -1 )
	{
		if ( W_CheckNumForName("DSPISTOL") != -1 )
		{
			sfxlump = W_GetNumForName("DSPISTOL");
		}else
			if ( W_CheckNumForName("STFPOW") != -1 )
		{
			sfxlump = W_GetNumForName("STFPOW");
		}else
			if ( W_CheckNumForName("TICTOC") != -1 )
		{
			sfxlump = W_GetNumForName("TICTOC");
		}else
		{
			sfxlump = W_GetNumForName("DSPISTOL");
		}
	}
	else
	{
		sfxlump = W_GetNumForName(name);
	}
	
	size = W_LumpLength( sfxlump );

	// Debug.
	// fprintf( stderr, "." );
	//fprintf( stderr, " -loading	%s (lump %d, %d bytes)\n",
	//		 sfxname, sfxlump, size );
	//fflush( stderr );
	
	sfx = (unsigned char*)W_CacheLumpNum( sfxlump, PU_STATIC );

	// Pads the sound effect out to the mixing buffer size.
	// The original realloc would interfere with zone memory.
	paddedsize = ((size-8 + (SAMPLECOUNT-1)) / SAMPLECOUNT) * SAMPLECOUNT;

	// Allocate from zone memory.
//	paddedsfx = (unsigned char*)Z_Malloc( paddedsize+8, PU_STATIC, 0 );
	paddedsfx = (unsigned char*)Z_Malloc( paddedsize+16, PU_STATIC, 0 );
	// ddt: (unsigned char *) realloc(sfx, paddedsize+8);
	// This should interfere with zone memory handling,
	//	which does not kick in in the soundserver.

	// Now copy and pad.
	memcpy(	paddedsfx, sfx, size );
	for (i=size ; i<paddedsize+8 ; i++)
		paddedsfx[i] = 128;

	// Remove the cached lump.
	Z_Free( sfx );
	
	// Preserve padded length.
	*len = paddedsize;

	// Return allocated padded data.
	return (void *) (paddedsfx + 8);
}





//
// This function adds a sound to the
//	list of currently active sounds,
//	which is maintained as a given number
//	(eight, usually) of internal channels.
// Returns a handle.
//
int
addsfx
( int		sfxid,
	int		volume,
	int		step,
	int		seperation )
{
	static unsigned short	handlenums = 0;
 
	int		i;
	int		rc = -1;
	
	int		oldest = gametic;
	int		oldestnum = 0;
	int		slot;

	int		rightvol;
	int		leftvol;

	// Chainsaw troubles.
	// Play these sound effects only one at a time.
	if ( sfxid == sfx_sawup
	 || sfxid == sfx_sawidl
	 || sfxid == sfx_sawful
	 || sfxid == sfx_sawhit
	 || sfxid == sfx_stnmov
	 || sfxid == sfx_pistol	 )
	{
	// Loop all channels, check.
	for (i=0 ; i<NUM_CHANNELS ; i++)
	{
		// Active, and using the same SFX?
		if ( (channels[i])
		 && (channelids[i] == sfxid) )
		{
		// Reset.
		channels[i] = 0;
		// We are sure that iff,
		//	there will only be one.
		break;
		}
	}
	}

	// Loop all channels to find oldest SFX.
	for (i=0; (i<NUM_CHANNELS) && (channels[i]); i++)
	{
	if (channelstart[i] < oldest)
	{
		oldestnum = i;
		oldest = channelstart[i];
	}
	}

	// Tales from the cryptic.
	// If we found a channel, fine.
	// If not, we simply overwrite the first one, 0.
	// Probably only happens at startup.
	if (i == NUM_CHANNELS)
		slot = oldestnum;
	else
		slot = i;

	// Okay, in the less recent channel,
	//	we will handle the new SFX.
	// Set pointer to raw data.
	channels[slot] = (unsigned char *) S_sfx[sfxid].data;
	// Set pointer to end of raw data.
	channelsend[slot] = channels[slot] + lengths[sfxid];

	// Reset current handle number, limited to 0..100.
	if (!handlenums)
		handlenums = 100;

	// Assign current handle number.
	// Preserved so sounds could be stopped (unused).
	channelhandles[slot] = rc = handlenums++;

	// Set stepping???
	// Kinda getting the impression this is never used.
	channelstep[slot] = step;
	// ???
	channelstepremainder[slot] = 0;
	// Should be gametic, I presume.
	channelstart[slot] = gametic;

	// Separation, that is, orientation/stereo.
	//	range is: 1 - 256
	seperation += 1;

	// Per left/right channel.
	//	x^2 seperation,
	//	adjust volume properly.
	leftvol =
		volume - ((volume*seperation*seperation) >> 16); ///(256*256);
	seperation = seperation - 257;
	rightvol =
		volume - ((volume*seperation*seperation) >> 16);	

	// Sanity check, clamp volume.
	if (rightvol < 0 || rightvol > 127)
	I_Error("rightvol out of bounds");
	
	if (leftvol < 0 || leftvol > 127)
	I_Error("leftvol out of bounds");
	
	// Get the proper lookup table piece
	//	for this volume level???
	channelleftvol_lookup[slot] = &vol_lookup[leftvol*256];
	channelrightvol_lookup[slot] = &vol_lookup[rightvol*256];

	channelleftvol[slot] = (leftvol*240)>>7;
	channelrightvol[slot] = (rightvol*240)>>7;

	// Preserve sound SFX id,
	//	e.g. for avoiding duplicates of chainsaw.
	channelids[slot] = sfxid;

	// You tell me.
	return rc;
}





//
// SFX API
// Note: this was called by S_Init.
// However, whatever they did in the
// old DPMS based DOS version, this
// were simply dummies in the Linux
// version.
// See soundserver initdata().
//
void I_SetChannels()
{
	// Init internal lookups (raw data, mixing buffer, channels).
	// This function sets up internal lookups used during
	//	the mixing process. 
	int		i;
	int		j;
	
	int*	steptablemid = steptable + 128;
	
	// Okay, reset internal mixing channels to zero.
	/*for (i=0; i<NUM_CHANNELS; i++)
	{
	channels[i] = 0;
	}*/

	// This table provides step widths for pitch parameters.
	// I fail to see that this is currently used.
	for (i=-128 ; i<128 ; i++)
		steptablemid[i] = (int)(pow(2.0, (i/64.0))*65536.0);
	
	
	// Generates volume lookup tables
	//	which also turn the unsigned samples
	//	into signed samples.
	for (i=0 ; i<128 ; i++)
	for (j=0 ; j<256 ; j++)
		vol_lookup[i*256+j] = (i*(j-128)*256)/127;
}	

 
void I_SetSfxVolume(int volume)
{
	// Identical to DOS.
	// Basically, this should propagate
	//	the menu/config file setting
	//	to the state variable used in
	//	the mixing.
	snd_SfxVolume = volume;
}

// MUSIC API - dummy. Some code from DOS version.
void I_SetMusicVolume(int volume)
{
	// Internal state variable.
	snd_MusicVolume = volume;
	// Now set volume on output device.
	// Whatever( snd_MusciVolume );
}


//
// Retrieve the raw data lump index
//	for a given SFX name.
//
int I_GetSfxLumpNum(sfxinfo_t* sfx)
{
	char namebuf[32];
	sprintf(namebuf, "ds%s", sfx->name);
	return W_GetNumForName(namebuf);
}

//
// Starting a sound means adding it
//	to the current list of active sounds
//	in the internal channels.
// As the SFX info struct contains
//	e.g. a pointer to the raw data,
//	it is ignored.
// As our sound handling does not handle
//	priority, it is ignored.
// Pitching (that is, increased speed of playback)
//	is set, but currently not used by mixing.
//
int
I_StartSound
( int		id,
	int		vol,
	int		sep,
	int		pitch,
	int		priority )
{
	// UNUSED
	priority = 0;
	
	// Returns a handle (not used).
	id = addsfx( id, vol, steptable[pitch], sep );

	// fprintf( stderr, "/handle is %d\n", id );
	
	return id;
}



void I_StopSound (int handle)
{
	// You need the handle returned by StartSound.
	// Would be looping all channels,
	//	tracking down the handle,
	//	an setting the channel to zero.
	
	// UNUSED.
	handle = 0;
}


int I_SoundIsPlaying(int handle)
{
	// Ouch.
	return gametic < handle;
}




//
// This function loops all active (internal) sound
//	channels, retrieves a given number of samples
//	from the raw sound data, modifies it according
//	to the current (internal) channel parameters,
//	mixes the per channel samples into the global
//	mixbuffer, clamping it to the allowed range,
//	and sets up everything for transferring the
//	contents of the mixbuffer to the (two)
//	hardware channels (left and right, that is).
//
// This function currently supports only 16bit.
//
void I_UpdateSound( void )
{
	int tmixbuf[64];

#if 1
	// Mix current sound data.
	// Data, from raw sound, for right and left.
	register int		sample;
	register int		dl;
	register int		dr;
	
	// Pointers in global mixbuffer, left, right, end.
	signed short*		leftout;
	signed short*		rightout;
	signed short*		leftend;

	register	int*		tmixl;
	register	int*		tmixr;

	// Step in mixbuffer, left and right, thus two.
	int				step;

	// Mixing channel index.
	int				chan;
	int				samp;
	
	unsigned int chstp;
	byte 	**chanptr;
	unsigned int *chanstp;
	byte	*chancur;
	byte	*chanend;
	int		**clvl;
	int		**crvl;

	int		**clv;
	int		**crv;
	int		nchan;
	int		nsamp;
	
	register int		lvol;
	register int		rvol;
	int		chanstep;
	
	__hint_use_egpr();
	
#if 1
	chanptr = channels;
	clvl	= channelleftvol_lookup;
	crvl	= channelrightvol_lookup;
	clv		= channelleftvol;
	crv		= channelrightvol;
	chanstp	= channelstepremainder;
#endif
		
	// Left and right channel
	//	are in global mixbuffer, alternating.
	leftout = mixbuffer;
	rightout = mixbuffer+1;
	step = 2;
	
	tmixl	= tmixbuf+0;
	tmixr	= tmixbuf+1;

	// Determine end, for left channel only
	//	(right channel is implicit).
	leftend = mixbuffer + SAMPLECOUNT*step;

	//BGB: count channels
	nchan = 0;
	for ( chan = 0; chan < NUM_CHANNELS; chan++ )
	{
		chancur = chanptr[ chan ];
		if (!chancur)
			continue;
		nchan=chan+1;
	}
	
	if(!nchan)
	{
		nsamp = leftend - leftout;

		for(samp=0; samp < nsamp; samp += step)
		{
			leftout[samp] = 0;
			rightout[samp] = 0;
		}
		
		return;
	}

#if 0
	// Mix sounds into the mixing buffer.
	// Loop over step*SAMPLECOUNT,
	//	that is 512 values for two channels.
	while (leftout != leftend)
	{
		// Reset left/right value. 
		dl = 0;
		dr = 0;

#if 0
		// Love thy L2 chache - made this a loop.
		// Now more channels could be set at compile time
		//	as well. Thus loop those	channels.
		for ( chan = 0; chan < NUM_CHANNELS; chan++ )
		{
			// Check channel, if active.
			if (channels[ chan ])
			{
				// Get the raw data from the channel. 
				sample = *channels[ chan ];
				// Add left and right part
				//	for this channel (sound)
				//	to the current data.
				// Adjust volume accordingly.
				dl += channelleftvol_lookup[ chan ][sample];
				dr += channelrightvol_lookup[ chan ][sample];
				// Increment index ???
				channelstepremainder[ chan ] += channelstep[ chan ];
				// MSB is next sample???
				channels[ chan ] += channelstepremainder[ chan ] >> 16;
				// Limit to LSB???
				channelstepremainder[ chan ] &= 65536-1;

				// Check whether we are done.
				if (channels[ chan ] >= channelsend[ chan ])
					channels[ chan ] = 0;
			}
		}
#endif

#if 1
//		for ( chan = 0; chan < NUM_CHANNELS; chan++ )
		for ( chan = 0; chan < nchan; chan++ )
		{
			chancur = chanptr[ chan ];
			if (!chancur)
				continue;
			chstp = chanstp[ chan ];

			sample = *chancur;
//			dl += clvl[ chan ][sample];
//			dr += crvl[ chan ][sample];

			dl += __int_mulsw(clv[chan], sample-128);
			dr += __int_mulsw(crv[chan], sample-128);

			chstp += channelstep[ chan ];
			chancur += chstp >> 16;
			chstp &= 65535;
			if (chancur >= channelsend[ chan ])
				chancur = NULL;

			chanptr[ chan ] = chancur;
			chanstp[ chan ] = chstp;
		}
#endif

		// Clamp to range. Left hardware channel.
		// Has been char instead of short.
		// if (dl > 127) *leftout = 127;
		// else if (dl < -128) *leftout = -128;
		// else *leftout = dl;

#if 0
		if (dl > 0x7fff)
			*leftout = 0x7fff;
		else if (dl < -0x8000)
			*leftout = -0x8000;
		else
			*leftout = dl;

		// Same for right hardware channel.
		if (dr > 0x7fff)
			*rightout = 0x7fff;
		else if (dr < -0x8000)
			*rightout = -0x8000;
		else
			*rightout = dr;
#endif

//		if(dl<(-32767)) dl=-32767;
//		if(dl>( 32767)) dl= 32767;
//		if(dr<(-32767)) dr=-32767;
//		if(dr>( 32767)) dr= 32767;

		dl = __int_clamp(dl, -32000, 32000);
		dr = __int_clamp(dr, -32000, 32000);

		*leftout = dl;
		*rightout = dr;

		// Increment current pointers in mixbuffer.
		leftout += step;
		rightout += step;
	}
#endif


#if 1
	while (leftout != leftend)
	{
		nsamp = leftend - leftout;
		if(nsamp > 64)
			nsamp = 64;

#if 1
		for(samp=0; samp < nsamp; samp += step)
		{
//			leftout[samp] = 0;
//			rightout[samp] = 0;
			tmixl[samp] = 0;
			tmixr[samp] = 0;
		}
#endif

		for ( chan = 0; chan < nchan; chan++ )
		{
			chancur = chanptr[ chan ];
			if (!chancur)
				continue;
			chanend = channelsend[ chan ];
				
			chstp = chanstp[ chan ];
			lvol = clv[chan];
			rvol = crv[chan];
			chanstep = channelstep[ chan ];

			for(samp=0; samp < nsamp; samp += step)
			{
//				dl = leftout[samp];
//				dr = rightout[samp];
				dl = tmixl[samp];
				dr = tmixr[samp];
				sample = *chancur;
				dl += __int_mulsw(lvol, sample-128);
				dr += __int_mulsw(rvol, sample-128);
//				dl = __int_clamp(dl, -32000, 32000);
//				dr = __int_clamp(dr, -32000, 32000);
//				leftout[samp] = dl;
//				rightout[samp] = dr;
				tmixl[samp] = dl;
				tmixr[samp] = dr;

				chstp += chanstep;
				chancur += chstp >> 16;
				chstp &= 65535;

//				if (chancur >= chanend)
//					break;
			}

			if (chancur >= chanend)
				chancur = NULL;
			chanptr[ chan ] = chancur;
			chanstp[ chan ] = chstp;
		}

#if 1
		for(samp=0; samp < nsamp; samp += step)
		{
			dl = tmixl[samp];
			dr = tmixr[samp];
			dl = __int_clamp(dl, -32000, 32000);
			dr = __int_clamp(dr, -32000, 32000);
			leftout[samp] = dl;
			rightout[samp] = dr;
		}
#endif

		leftout += nsamp;
		rightout += nsamp;
	}
#endif


#endif

}


// void I_MusicGetAdvanceSamples(short *buf, int sn, int snmax);
void	SoundDev_WriteStereoSamples(short *mixbuf, int nsamp);

void I_MusicSubmit(void);

static int iss_curms, iss_lastms;


// 
// This would be used to write out the mixbuffer
//	during each game loop update.
// Updates sound buffer and audio device at runtime. 
// It is called during Timer interrupt with SNDINTR.
// Mixing now done synchronous, and
//	only output be done asynchronous?
//
void
I_SubmitSound(void)
{
	I_SubmitSound2(0);
}

void
I_SubmitSound2(int extra)
{
	static short mixbuf2[SAMPLECOUNT*2*2];
//	static short mixbuf_mus[SAMPLECOUNT*2*2];

	short *mix1, *mix2, *mix3, *mix4;
	
	int i, j, k;
	int n, ns, dt, musvol;
	// Write it to DSP device.
//	write(audio_fd, mixbuffer, SAMPLECOUNT*BUFMUL);

//	n=SAMPLECOUNT*1.451247;
	n=(SAMPLECOUNT*1486)>>10;

	iss_curms=TK_GetTimeMs();
	dt=iss_curms-iss_lastms;
	iss_lastms=iss_curms;
	ns=dt*16;
	if(ns<0)ns=0;
	if(ns>n)ns=n;
	
	if(ns<=0)
		return;

//	printf("dt=%d ns=%d\n", dt, ns);

//	I_MusicGetAdvanceSamples(mixbuf_mus, ns, SAMPLECOUNT*1.5);

//	musvol=snd_MusicVolume;

#if 1
//	n=SAMPLECOUNT*1.451247;
	n=(SAMPLECOUNT*1486)>>10;
	mix1=mixbuffer; mix2=mixbuf2;
	k=0;
	for(i=0; i<n; i++)
	{
//		j=i*(1.0/1.451247);
//		j=(i*2822)>>12;
		j=k>>12;
		k+=2822;
//		mixbuf2[i*2+0]=mixbuffer[j*2+0];
//		mixbuf2[i*2+1]=mixbuffer[j*2+1];
//		mix3=mix1+j*2;	mix4=mix2+i*2;
//		*((u32 *)mix4)=*((u32 *)mix3);
		((u32 *)mix2)[i]=((u32 *)mix1)[j];
	}
#endif

	if(ns>0)
	{
//		SoundDev_WriteStereoSamples(mixbuffer, SAMPLECOUNT);
		if(extra&1)
		{
			SoundDev_WriteStereoSamples2(mixbuf2, n, n);
		}else
		{
//			SoundDev_WriteStereoSamples(mixbuf2, ns);
			SoundDev_WriteStereoSamples2(mixbuf2, ns, n);
		}
		SoundDev_Submit();
	}
	
	I_MusicSubmit();
}



void
I_UpdateSoundParams
( int	handle,
	int	vol,
	int	sep,
	int	pitch)
{
	// I fail too see that this is used.
	// Would be using the handle to identify
	//	on which channel the sound might be active,
	//	and resetting the channel parameters.

	// UNUSED.
	handle = vol = sep = pitch = 0;
}




void I_ShutdownSound(void)
{	
	// Wait till all pending sounds are finished.
	int done = 0;
	int i;

	// FIXME (below).
	fprintf( stderr, "I_ShutdownSound: NOT finishing pending sounds\n");
	fflush( stderr );
	
	while ( !done )
	{
		for( i=0 ; i<8 && !channels[i] ; i++);
		
		// FIXME. No proper channel output.
		//if (i==8)
		done=1;
	}
	
	// Cleaning up -releasing the DSP device.
//	close ( audio_fd );

	// Done.
	return;
}






void
I_InitSound()
{
	static int init=0;
	
#if 1 
	int i;

	if(init)
	{
		printf("I_InitSound: Skip\n");
		return;
	}
	init=1;

	
	// Secure and configure sound device first.
	fprintf( stderr, "I_InitSound: ");
	
	i=SAMPLERATE;
	
//	SoundDev_Init();
	
	fprintf(stderr, " configured audio device\n" );

	
	// Initialize external data (all sounds) at start, keep static.
	fprintf( stderr, "I_InitSound: ");
	
	for (i=1 ; i<NUMSFX ; i++)
	{ 
		// Alias? Example is the chaingun sound linked to pistol.
		if (!S_sfx[i].link)
		{
			// Load data from WAD file.
			S_sfx[i].data = getsfx( S_sfx[i].name, &lengths[i] );
		}	
		else
		{
			// Previously loaded already?
			S_sfx[i].data = S_sfx[i].link->data;
			lengths[i] = lengths[(S_sfx[i].link - S_sfx)/sizeof(sfxinfo_t)];
		}
	}

	fprintf( stderr, " pre-cached all sound data\n");
	
	// Now initialize mixbuffer with zero.
	for ( i = 0; i< MIXBUFFERSIZE; i++ )
		mixbuffer[i] = 0;
	
	// Finished initialization.
	fprintf(stderr, "I_InitSound: sound module ready\n");

#endif
}


// Interrupt handler.
void I_HandleSoundTimer( int ignore )
{
	return;
}

// Get the interrupt. Set duration in millisecs.
int I_SoundSetTimer( int duration_of_tick )
{
	return(-1);
}

// Remove the interrupt. Set duration to zero.
void I_SoundDelTimer()
{
}
