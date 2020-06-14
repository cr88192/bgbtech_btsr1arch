#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "rt_def.h"

#include "rt_sound.h"
#include "fx_man.h"
#include "music.h"

#define		FX_POS_SHL	12

typedef struct {
byte *data;
int len;
int maxlen;
int pos;
int prio;
int step;
byte lvol;
byte rvol;
long userval;
}fx_channel_t;

int fx_chanmask = 0;

fx_channel_t	*fx_chan[32];

int SoundDev_Init(void);
void	SoundDev_WriteStereoSamples(short *mixbuf, int nsamp);
void	SoundDev_Submit();


char *FX_ErrorString( int ErrorNumber )
{
	return(NULL);
}

int   FX_SetupCard( int SoundCard, fx_device *device )
{
	int i, j, k;

	device->MaxVoices=16;
	device->MaxSampleBits=8;
	device->MaxChannels=2;
	
	for(i=0; i<16; i++)
	{
		fx_chan[i]=malloc(sizeof(fx_channel_t));
		memset(fx_chan[i], 0, sizeof(fx_channel_t));
	}

// #ifdef _WIN32
	SoundDev_Init();
// #endif
	
	return(0);
}

int   FX_GetBlasterSettings( fx_blaster_config *blaster )
{
	return(0);
}

void (*fx_callback_f)( unsigned long sndnum );


int fx_mixbuf_l[8192];
int fx_mixbuf_r[8192];
short fx_mixbuf[16384];
int fx_mixtime;

#ifdef __BJX2__
int FX_UpdateMix_A(
	int *mixl, int *mixr,
	byte *src, int cnt,
	int pos, int step,
	int lvol, int rvol);

__asm {
FX_UpdateMix_A:
	CMPGT	0, R7
	BF		.L1
.L0:
	SHAD	R20, -12, R18
	ADD		R20, R21, R20	|	MOVU.B	(R6, R18), R16
	ADD		R16, -128, R16
	MULS.W	R16, R22, R16	|	MOV.L	(R4), R2
	MULS.W	R16, R23, R17	|	ADD		R7, -1, R7
	ADD		R2, R16, R16	|	MOV.L	(R5), R3
	ADD		R3, R17, R17	|	MOV.L	R16, (R4)
	ADD		R4, 4, R4		|	MOV.L	R17, (R5)
	ADD		R5, 4, R5		|	CMPGT	0, R7
	BT		.L0
.L1:
	RTSU
}

#endif

int FX_UpdateMix(int mixstep)
{
	byte *dat, *cs;
	int *mixl, *mixr;
	int pos, pos1, len, l1, lvol, rvol, ovmix, step;
	int i, j, k;

	ovmix=mixstep*2;
	mixl=fx_mixbuf_l;
	mixr=fx_mixbuf_r;

	for(j=0; j<ovmix; j++)
	{
		mixl[j]=0;
		mixr[j]=0;
	}

	for(i=1; i<16; i++)
	{
		if(!(fx_chanmask&(1<<i)))
			continue;
		
		dat=fx_chan[i]->data;
		pos=fx_chan[i]->pos;
		len=fx_chan[i]->len;
		step=fx_chan[i]->step;
		lvol=fx_chan[i]->lvol;
		rvol=fx_chan[i]->rvol;
		
		cs=dat+pos;
		l1=ovmix;
//		if((pos+l1)>len)
//			l1=len-pos;

		if((pos+l1*step)>(len<<FX_POS_SHL))
			l1=((len<<FX_POS_SHL)-pos)/step;
		
#if 0
		pos1=pos;
		for(j=0; j<l1; j++)
		{
			k=pos1>>FX_POS_SHL;
			pos1+=step;
//			k=(cs[k]-128);
			k=(dat[k]-128);
			mixl[j]+=k*lvol;
			mixr[j]+=k*rvol;
		}
#endif

#ifdef __BJX2__
		FX_UpdateMix_A(
			fx_mixbuf_l,	fx_mixbuf_r,	dat,	l1,
			pos,			step,			lvol,	rvol);
#else
		pos1=pos;
		j=l1;
		mixl=fx_mixbuf_l;
		mixr=fx_mixbuf_r;
		while((j--)>0)
		{
			k=pos1>>FX_POS_SHL;
			k=(dat[k]-128);
			*mixl=(*mixl)+(k*lvol);
			*mixr=(*mixr)+(k*rvol);
			pos1+=step;
			mixl++;
			mixr++;
		}
#endif

		pos=pos+mixstep*step;
		fx_chan[i]->pos=pos;
		k=pos>>FX_POS_SHL;
//		if(pos>=len)
		if(k>=len)
		{
			if(fx_callback_f)
				fx_callback_f(fx_chan[i]->userval);
			fx_chanmask&=~(1<<i);
		}
	}
}

int FX_Update()
{
	register int *mixl, *mixr;
	register short *mixb;
	int ms, dt, dt1, ns1, ns2;
	register int lv, rv;
	int i, j, k, l;

	ms=I_TimeMS();
	dt=ms-fx_mixtime;
	fx_mixtime=ms;
	
	while(dt>0)
	{
		dt1=dt;
		if(dt1>256)
			dt1=256;
		dt-=dt1;
	
//		ns1=dt1*11;
		ns2=dt1*16;
//		FX_UpdateMix(ns1);
		FX_UpdateMix(ns2);
		
		mixl=fx_mixbuf_l;
		mixr=fx_mixbuf_r;
		mixb=fx_mixbuf;
		
		l=(ns2*2);
		for(i=0; i<l; i++)
		{
//			j=(i*11)/16;
//			j=i;
			lv=mixl[i];
			rv=mixr[i];
			
			if((lv<=-32000))lv=-32000;
			if((lv>  32000))lv= 32000;
			if((rv<=-32000))rv=-32000;
			if((rv>  32000))rv= 32000;
			
			mixb[0]=lv;
			mixb[1]=rv;
			mixb+=2;
			
//			mixb[i*2+0]=lv;
//			mixb[i*2+1]=rv;
		}

// #ifdef _WIN32
		SoundDev_WriteStereoSamples(fx_mixbuf, ns2);
// #endif
	}

	SoundDev_Submit();

	return(0);
}

#if 1
int   FX_SetupSoundBlaster( fx_blaster_config blaster, int *MaxVoices, int *MaxSampleBits, int *MaxChannels )
{
	return(0);
}
#endif

int   FX_Init( int SoundCard, int numvoices, int numchannels, int samplebits, unsigned mixrate )
{
	return(0);
}

int   FX_Shutdown( void )
{
	return(0);
}

#if 1
int   FX_SetCallBack( void ( *function )( unsigned long ) )
{
	fx_callback_f = function;
	return(0);
}
#endif

void  FX_SetVolume( int volume )
{
}

int   FX_GetVolume( void )
{
	return(0);
}

#if 1
void  FX_SetReverseStereo( int setting )
{
}

int   FX_GetReverseStereo( void )
{
	return(0);
}

void  FX_SetReverb( int reverb )
{
}
#endif

int FX_VoiceAvailable( int priority )
{
	int i, j, cnt, bi;

	for(i=1; i<16; i++)
	{
		if(!(fx_chanmask&(1<<i)))
			return(i);
	}
	
	bi=1;
	for(i=1; i<16; i++)
	{
		if(fx_chan[i]->prio < fx_chan[bi]->prio)
			bi=i;
	}

	if(fx_chan[bi]->prio < priority)
		return(bi);

	return(0);
}

#if 1
int FX_SetPan( int handle, int vol, int left, int right )
{
	return(0);
}

int FX_SetPitch( int handle, int pitchoffset )
{
	return(0);
}
#endif

int FX_FlushChan(int chn)
{
	if(fx_chanmask&(1<<chn))
	{
		if(fx_callback_f)
			fx_callback_f(fx_chan[chn]->userval);
		fx_chanmask&=~(1<<chn);
	}
	return(0);
}

int FX_CheckExpandChan(int chn, int len)
{
	if((fx_chan[chn]->data) && (len>fx_chan[chn]->maxlen))
	{
		free(fx_chan[chn]->data);
		fx_chan[chn]->data=NULL;
	}

	if(!fx_chan[chn]->data)
	{
		fx_chan[chn]->data=malloc(len+16);
		fx_chan[chn]->maxlen=len;
	}
	return(0);
}

int FX_PlayVOC3D( char *ptr, int pitchoffset, int angle, int distance,
       int priority, unsigned long callbackval )
{
	byte *seg_ptr[256];
	int seg_len[256];
	int nseg;
	byte *cs, *cs0, *ct;
	int chn, tag, sz, len, vol, lvol, rvol, rate;
	int i, j, k;
	
//	printf("FX_PlayVOC3D %X prio=%d\n", callbackval, priority);

	chn=FX_VoiceAvailable(priority);
	if(chn<=0)
	{
		printf("FX_PlayVOC3D: No Chan, %X prio=%d\n", callbackval, priority);
		return(0);
	}

//	vol=255/(distance+1);
//	vol=1024/(distance+1);
	vol=4096/(distance+1);
	if(vol>255)vol=255;
//	vol=255;

	lvol=vol;
	rvol=vol;
	
	i=8-abs(angle-8);
	j=8-abs(angle-24);
	if(i<0)i=0;
	if(j<0)j=0;
//	lvol=(vol*(j+12))/16;
//	rvol=(vol*(i+12))/16;
	lvol=(vol*(j+4))/8;
	rvol=(vol*(i+4))/8;

//	if(vol<8)
//		return(0);


	FX_FlushChan(chn);
	rate=11025;
	
	len=0; nseg=0;
	cs=(byte *)(ptr+0x1A);
	while(*cs)
	{
		tag=*(int *)cs;
		cs0=cs+4;
		sz=(tag>>8);
		cs=cs0+sz;
		
//		printf("%d\n", tag&255);
		if((tag&255)==1)
		{
			rate=1000000/(256-cs0[0]);
			len+=sz-2;
			seg_ptr[nseg]=cs0+2;
			seg_len[nseg]=sz-2;
			nseg++;
		}

		if((tag&255)==2)
		{
			seg_ptr[nseg]=cs0;
			seg_len[nseg]=sz;
			nseg++;
		}

		if((tag&255)==9)
		{
			rate=*(int *)cs0;
			len+=sz-12;
			seg_ptr[nseg]=cs0+12;
			seg_len[nseg]=sz-12;
			nseg++;		
		}
	}
	
//	printf("Len=%d\n", len);
	
	FX_CheckExpandChan(chn, len);
	fx_chan[chn]->len=len;
	fx_chan[chn]->pos=0;
//	fx_chan[chn]->step=(11*4096)/16;
//	fx_chan[chn]->step=((11025+pitchoffset)*4096)/16000;
	fx_chan[chn]->step=((rate+pitchoffset)*4096)/16000;

	fx_chan[chn]->lvol=lvol;
	fx_chan[chn]->rvol=rvol;
	fx_chan[chn]->prio=priority;
	fx_chan[chn]->userval=callbackval;

	fx_chanmask|=1<<chn;
	
	ct=fx_chan[chn]->data;
	for(i=0; i<nseg; i++)
	{
		sz=seg_len[i];
		memcpy(ct, seg_ptr[i], sz);
		ct+=sz;
	}
	
	return(chn);
}

#define FOURCC(a, b, c, d)		((a)|((b)<<8)|((c)<<16)|((d)<<24))

int FX_PlayWAV3D( char *ptr, int pitchoffset, int angle, int distance,
       int priority, unsigned long callbackval )
{
	byte *seg_ptr[256];
	int seg_len[256];
	int nseg;
	byte *cs, *cse, *cs0, *ct;
	int chn, tag, sz, len, vol, lvol, rvol;
	int rate, chan, bits;
	int i, j, k;
	
//	printf("FX_PlayWAV3D\n");

//	printf("FX_PlayWAV3D %X prio=%d\n", callbackval, priority);

	cs=(byte *)ptr;
	sz=*(int *)(cs+4);
	if(memcmp(cs+0, "RIFF", 4))
	{
		printf("FX_PlayWAV3D: RIFF magic fail\n");
		return(0);
	}
	if(memcmp(cs+8, "WAVE", 4))
	{
		printf("FX_PlayWAV3D: WAVE magic fail\n");
		return(0);
	}
	cs=cs+12;
	cse=cs+sz;
	if(cs>=cse)
	{
		printf("FX_PlayWAV3D: RIFF size fail\n");
		return(0);
	}

	chn=FX_VoiceAvailable(priority);
	if(chn<=0)
	{
		printf("FX_PlayWAV3D: No Chan, %X prio=%d\n", callbackval, priority);
		return(0);
	}

//	vol=255/(distance+1);
//	vol=1024/(distance+1);
	vol=4096/(distance+1);
	if(vol>255)vol=255;
//	vol=255;

	lvol=vol;
	rvol=vol;
	
	i=8-abs(angle-8);
	j=8-abs(angle-24);
	if(i<0)i=0;
	if(j<0)j=0;
//	lvol=(vol*(j+12))/16;
//	rvol=(vol*(i+12))/16;
	lvol=(vol*(j+4))/8;
	rvol=(vol*(i+4))/8;

//	if(vol<8)
//		return(0);


	FX_FlushChan(chn);
	
	len=0; nseg=0;
	rate=7000;

#if 0
	cs=(byte *)(ptr+0x1A);
	while(*cs)
	{
		tag=*(int *)cs;
		cs0=cs+4;
		sz=(tag>>8);
		cs=cs0+sz;
		
//		printf("%d\n", tag&255);
		if((tag&255)==1)
		{
			len+=sz-2;
			seg_ptr[nseg]=cs0+2;
			seg_len[nseg]=sz-2;
			nseg++;
		}
	}
#endif

	while(cs<cse)
	{
		tag=*(int *)cs;
		sz=*(int *)(cs+4);
		if(sz<0)
			break;
		cs0=cs+8;
		cs=cs+8+((sz+1)&(~1));
		
		if(tag==FOURCC('f', 'm', 't', ' '))
		{
			rate=*(u32 *)(cs0+4);
			chan=*(u16 *)(cs0+2);
			bits=*(u16 *)(cs0+14);
			continue;
		}

		if(tag==FOURCC('d', 'a', 't', 'a'))
		{
			len+=sz;
			seg_ptr[nseg]=cs0;
			seg_len[nseg]=sz;
			nseg++;
			continue;
		}
	}
	
//	printf("Len=%d\n", len);
	
	FX_CheckExpandChan(chn, len);
	fx_chan[chn]->len=len;
	fx_chan[chn]->pos=0;
//	fx_chan[chn]->step=(11*4096)/16;
	fx_chan[chn]->step=((rate+pitchoffset)*4096)/16000;

	fx_chan[chn]->lvol=lvol;
	fx_chan[chn]->rvol=rvol;
	fx_chan[chn]->prio=priority;
	fx_chan[chn]->userval=callbackval;

	fx_chanmask|=1<<chn;
	
	ct=fx_chan[chn]->data;
	for(i=0; i<nseg; i++)
	{
		sz=seg_len[i];
		memcpy(ct, seg_ptr[i], sz);
		ct+=sz;
	}

	ct=fx_chan[chn]->data; vol=0;
	for(i=0; i<len; i++)
	{
		k=ct[i]-128;
		if(k<0)k=-k;
		if(k>vol)
			vol=k;
	}
	
	if(vol<64)
	{
		for(i=0; i<len; i++)
		{
			k=ct[i]-128;
			k=128+(2*k);
			ct[i]=k;
		}
	}
	
	return(chn);
}

#if 1
int FX_Pan3D( int handle, int angle, int distance )
{
	return(0);
}
#endif

int FX_SoundActive( int handle )
{
	if(fx_chanmask & (1<<handle))
		return(1);

	return(0);
}

int FX_SoundsPlaying( void )
{
	int i, j, cnt;

	cnt=0;
	for(i=0; i<16; i++)
		if(fx_chanmask & (1<<i))
			cnt++;

	return(cnt);
}

int FX_StopSound( int handle )
{
	FX_FlushChan(handle);
	fx_chanmask &= ~(1<<handle);
	return(0);
}

int FX_StopAllSounds( void )
{
	int i;
	
	for(i=0; i<16; i++)
		FX_FlushChan(i);

	fx_chanmask = 0;

	return(0);
}

#if 1
int FX_StartDemandFeedPlayback( void ( *function )( char **ptr, unsigned long *length ),
       int rate, int pitchoffset, int vol, int left, int right,
       int priority, unsigned long callbackval )
{
	return(0);
}
#endif

#if 1
int  FX_StartRecording( int MixRate, void ( *function )( char *ptr, int length ) )
{
	return(0);
}

void FX_StopRecord( void )
{
}
#endif

