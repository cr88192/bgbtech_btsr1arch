
// I_SOUND.C

#include <stdio.h>
#include <math.h>
#include "doomdef.h"
// #include "dmx.h"
#include "sounds.h"
#include "i_sound.h"

#include <tkgdi/tkgdi.h>


extern sfxinfo_t S_sfx[];

// #define SAMPLECOUNT		512
#define SAMPLECOUNT		256
#define NUM_CHANNELS		8
// It is 2 for 16bit, and 2 for two channels.
#define BUFMUL									4
#define MIXBUFFERSIZE		(SAMPLECOUNT*BUFMUL)

#define SAMPLERATE		11025	// Hz
// #define SAMPLERATE		16000	// Hz
#define SAMPLESIZE		2	 	// 16bit


int 		lengths[NUMSFX];
signed short	mixbuffer[MIXBUFFERSIZE];

unsigned int	channelstep[NUM_CHANNELS];
unsigned int	channelstepremainder[NUM_CHANNELS];

unsigned char*		channels[NUM_CHANNELS];
unsigned char*		channelsend[NUM_CHANNELS];

int			channelstart[NUM_CHANNELS+4];
int 		channelhandles[NUM_CHANNELS+4];
int			channelids[NUM_CHANNELS+4];

int			channel_lvol[NUM_CHANNELS+4];			
int			channel_rvol[NUM_CHANNELS+4];			

int		steptable[256];

void	SoundDev_WriteStereoSamples(short *mixbuf, int nsamp);
void	SoundDev_WriteStereoSamples2(short *mixbuf, int nsamp, int nsamp2);

void	*I_GetSfxLump( int sfxlump, int *len )
{
	unsigned char*		sfx;
	unsigned char*		paddedsfx;
	int					i;
	int					size;
	int					paddedsize;
	
	size = W_LumpLength( sfxlump );

	sfx = (unsigned char*)W_CacheLumpNum( sfxlump, PU_STATIC );

	paddedsize = ((size-8 + (SAMPLECOUNT-1)) / SAMPLECOUNT) * SAMPLECOUNT;

	paddedsfx = (unsigned char*)Z_Malloc( paddedsize+8, PU_STATIC, 0 );

	memcpy(	paddedsfx, sfx, size );
	for (i=size ; i<paddedsize+8 ; i++)
		paddedsfx[i] = 128;

	Z_Free( sfx );
	
	*len = paddedsize;
	return (void *) (paddedsfx + 8);
}

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
	
	int		oldest;
	int		oldestnum;
	int		slot;

	int		rightvol;
	int		leftvol;

	oldest = gametic;
	oldestnum = 0;

	volume*=4;

	// Loop all channels to find oldest SFX.
	for (i=0; (i<NUM_CHANNELS) && (channels[i]); i++)
	{
		if (channelstart[i] < oldest)
		{
			oldestnum = i;
			oldest = channelstart[i];
		}
	}

	if (i == NUM_CHANNELS)
		slot = oldestnum;
	else
		slot = i;

	if(slot < 0)
		return(-1);

	if(slot >= NUM_CHANNELS)
		return(-1);

	channels[slot] = (unsigned char *) S_sfx[sfxid].snd_ptr;
	channelsend[slot] = channels[slot] + lengths[sfxid];

//	printf("addsfx: ptr=%p len=%d\n", channels[slot], lengths[sfxid]);

	if (!handlenums)
		handlenums = 100;

	channelhandles[slot] = rc = handlenums++;

	channelstep[slot] = step;
	channelstepremainder[slot] = 0;
	channelstart[slot] = gametic;

	seperation += 1;

	leftvol =
		volume - ((volume*seperation*seperation) >> 16); ///(256*256);
	seperation = seperation - 257;
	rightvol =
		volume - ((volume*seperation*seperation) >> 16);	

	rightvol = __int_clamp(rightvol, 0, 127);
	leftvol = __int_clamp(leftvol, 0, 127);

	// Sanity check, clamp volume.
//	if (rightvol < 0 || rightvol > 127)
//		I_Error("rightvol out of bounds");
	
//	if (leftvol < 0 || leftvol > 127)
//		I_Error("leftvol out of bounds");
	
//	channelleftvol_lookup[slot] = &vol_lookup[leftvol*256];
//	channelrightvol_lookup[slot] = &vol_lookup[rightvol*256];

	channelids[slot] = sfxid;
	channel_lvol[slot] = leftvol;
	channel_rvol[slot] = rightvol;

	return rc;
}



/*
===============
=
= I_StartupTimer
=
===============
*/

int tsm_ID = -1;

void I_StartupTimer (void)
{
}

void I_ShutdownTimer (void)
{
}

/*
 *
 *                           SOUND HEADER & DATA
 *
 *
 */

const char snd_prefixen[] = { 'P', 'P', 'A', 'S', 'S', 'S', 'M',
  'M', 'M', 'S' };

int snd_Channels;
int snd_DesiredMusicDevice, snd_DesiredSfxDevice;
int snd_MusicDevice,    // current music card # (index to dmxCodes)
	snd_SfxDevice,      // current sfx card # (index to dmxCodes)
	snd_MaxVolume,      // maximum volume for sound
	snd_MusicVolume;    // maximum volume for music
int dmxCodes[NUM_SCARDS]; // the dmx code for a given card

int     snd_SBport, snd_SBirq, snd_SBdma;       // sound blaster variables
int     snd_Mport;                              // midi variables

extern boolean  snd_MusicAvail, // whether music is available
		snd_SfxAvail;   // whether sfx are available

void I_SetSfxVolume(int volume)
{
  snd_MaxVolume = volume; // THROW AWAY?
}

/*
 *
 *                                 SOUND FX API
 *
 */

// Gets lump nums of the named sound.  Returns pointer which will be
// passed to I_StartSound() when you want to start an SFX.  Must be
// sure to pass this to UngetSoundEffect() so that they can be
// freed!


int I_GetSfxLumpNum(sfxinfo_t *sound)
{
  char namebuf[9];

  if(sound->name == 0)
		return 0;
  if (sound->link) sound = sound->link;
//  sprintf(namebuf, "d%c%s", snd_prefixen[snd_SfxDevice], sound->name);
  return W_GetNumForName(sound->name);

}

//int I_StartSound (int id, void *data, int vol, int sep, int pitch, int priority)
int I_StartSound (int id, int vol, int sep, int pitch, int priority)
{
	id = addsfx( id, vol, steptable[pitch], sep );
	return id;
}

void I_StopSound(int handle)
{
	int i;
	for (i=0; (i<NUM_CHANNELS); i++)
	{
		if(!channels[i])
			continue;

		if(channelhandles[i] == handle)
			channels[i] = NULL;
	}
}

int I_SoundIsPlaying(int handle)
{
	int i;

	for (i=0; (i<NUM_CHANNELS); i++)
	{
		if(!channels[i])
			continue;

		if(channelhandles[i] == handle)
			return(1);
	}

	return(0);
}

void I_UpdateSoundParams(int handle, int vol, int sep, int pitch)
{
}


void I_UpdateSound( void )
{
	// Mix current sound data.
	// Data, from raw sound, for right and left.
	register unsigned int	sample;
	register int		dl;
	register int		dr;
	
	// Pointers in global mixbuffer, left, right, end.
	signed short*		leftout;
	signed short*		rightout;
	signed short*		leftend;
	// Step in mixbuffer, left and right, thus two.
	int				step;

	// Mixing channel index.
	int				chan;
	int				lvol, rvol;
	
	unsigned int chstp;
	byte 	**chanptr;
	unsigned int *chanstp;
	byte	*chancur;
//	int		**clvl;
//	int		**crvl;
	
#if 1
	chanptr = channels;
//	clvl	= channelleftvol_lookup;
//	crvl	= channelrightvol_lookup;
	chanstp	= channelstepremainder;
#endif
		
	// Left and right channel
	//	are in global mixbuffer, alternating.
	leftout = mixbuffer;
	rightout = mixbuffer+1;
	step = 2;

	// Determine end, for left channel only
	//	(right channel is implicit).
	leftend = mixbuffer + SAMPLECOUNT*step;

	// Mix sounds into the mixing buffer.
	// Loop over step*SAMPLECOUNT,
	//	that is 512 values for two channels.
	while (leftout != leftend)
	{
		// Reset left/right value. 
		dl = 0;
		dr = 0;

		// Love thy L2 chache - made this a loop.
		// Now more channels could be set at compile time
		//	as well. Thus loop those	channels.

		for ( chan = 0; chan < NUM_CHANNELS; chan++ )
		{
			chancur = chanptr[ chan ];
			if (!chancur)
				continue;
			chstp = chanstp[ chan ];

			lvol = channel_lvol[chan];
			rvol = channel_rvol[chan];

			sample = *chancur;
//			dl += clvl[ chan ][sample];
//			dr += crvl[ chan ][sample];

			dl += (sample-128)*lvol;
			dr += (sample-128)*rvol;

			chstp += channelstep[ chan ];
			chancur += chstp >> 16;
			chstp &= 65535;
			if (chancur >= channelsend[ chan ])
				chancur = NULL;

			chanptr[ chan ] = chancur;
			chanstp[ chan ] = chstp;
		}

		if(dl<(-32767)) dl=-32767;
		if(dl>( 32767)) dl= 32767;
		if(dr<(-32767)) dr=-32767;
		if(dr>( 32767)) dr= 32767;
		*leftout = dl;
		*rightout = dr;

		// Increment current pointers in mixbuffer.
		leftout += step;
		rightout += step;
	}
}


/*
 *
 *                                                      SOUND STARTUP STUFF
 *
 *
 */

// inits all sound stuff

int i_sound_init;

void I_StartupSound (void)
{
	char tmp[80];
	int rc, i, j;

	i_sound_init = 1;

	for (i=-128 ; i<128 ; i++)
	{
//		steptable[i+128] = (int)(pow(2.0, (i/64.0))*65536.0);
		if(i>=0)
			j=(((i<<10)&0xFFFF)|0x10000)<<(i>>6);
		else
			j=(((i<<10)&0xFFFF)|0x10000)>>(-(i>>6));
		steptable[i+128] = j;
	}

	// inits sound library timer stuff
	I_StartupTimer();

//	SoundDev_Init();

}

void I_InitSound (void)
{
	I_StartupSound();
}

void I_SubmitSound2(int extra);

void I_SubmitSound(void)
{
	I_SubmitSound2(0);
}

TKGHSND hSndDev;
TKGDI_WAVEFORMATEX i_snd_t_info;
TKGDI_WAVEFORMATEX *i_snd_info = NULL;

void I_SubmitSound2(int extra)
{
//	static short mixbuf2[SAMPLECOUNT*2*2];
//	static short mixbuf_mus[SAMPLECOUNT*2*2];
	static short *mixbuf2;
	static int curms, lastms;
	
	int i, j;
	int n, ns, dt, musvol;

	if(!i_snd_info)
	{
		i_snd_info = &i_snd_t_info;
		memset(i_snd_info, 0, sizeof(TKGDI_WAVEFORMATEX));
		
		i_snd_info->wFormatTag=TKGDI_WAVE_FORMAT_PCM;
		i_snd_info->nChannels=2;
		i_snd_info->nSamplesPerSec=16000;
		i_snd_info->nAvgBytesPerSec=16000*4;
		i_snd_info->nBlockAlign=4;
		i_snd_info->wBitsPerSample=16;
		i_snd_info->cbSize=sizeof(TKGDI_WAVEFORMATEX);

		hSndDev = tkgCreateAudioDevice(0, TKGDI_FCC_auds, i_snd_info);
		mixbuf2 = tkgGlobalAlloc(SAMPLECOUNT*2*2*sizeof(short));
	}

	n=(SAMPLECOUNT*1486)>>10;

	curms=I_GetTimeMs();
	dt=curms-lastms;
	lastms=curms;
	ns=dt*16;
	if(ns<0)ns=0;
	if(ns>n)ns=n;

	musvol=snd_MusicVolume;

//	n=SAMPLECOUNT*1.451247;
	n=(SAMPLECOUNT*1486)>>10;
	for(i=0; i<n; i++)
	{
//		j=i/1.451247;
		j=(i*2822)>>12;
		mixbuf2[i*2+0]=mixbuffer[j*2+0];
		mixbuf2[i*2+1]=mixbuffer[j*2+1];
	}

#if 1
	if(ns>0)
	{
		if(extra)
		{
//			SoundDev_WriteStereoSamples(mixbuf2, n);
			tkgWriteSamples(hSndDev, mixbuf2, n, n);
		}else
		{
//			SoundDev_WriteStereoSamples(mixbuf2, ns);
			tkgWriteSamples(hSndDev, mixbuf2, ns, n);
		}

		SoundDev_Submit();
	}
#endif
}



// shuts down all sound stuff

void I_ShutdownSound (void)
{
//  DMX_DeInit();
  I_ShutdownTimer();
}

void I_SetChannels(int channels)
{
//  WAV_PlayMode(channels, SND_SAMPLERATE);
}
