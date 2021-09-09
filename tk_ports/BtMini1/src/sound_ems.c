#ifdef __EMSCRIPTEN__

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mouse.h>
#include <SDL/SDL_keyboard.h>
#include <SDL/SDL_audio.h>
#endif

byte *audio_chunk=NULL;
int audio_len;
int audio_pos;
int audio_pos2;
short *snd_rover=NULL;

void fill_audio(void *udata, Uint8 *stream, int len)
{
	int len2, rem, len3;
	if(!audio_chunk)
		return;

	rem=audio_len-audio_pos;

//	len=(len>rem?rem:len);
//	SDL_MixAudio(stream,
//		audio_chunk+audio_pos, len,
//		SDL_MIX_MAXVOLUME);

//	SDL_MixAudioFormat(stream,
//		audio_chunk+audio_pos,
//		AUDIO_S16,
//		len,
//		SDL_MIX_MAXVOLUME);

	len2=(len>rem?rem:len);
	len3=len-len2;
	memcpy(stream, audio_chunk+audio_pos, len2);
	if(len3>0)
	{
		if(len3>audio_len)
			len3=audio_len;
		memcpy(stream+len2, audio_chunk, len3);
	}

	audio_pos += len;
	if(audio_pos>=audio_len)
		audio_pos-=audio_len;
}

int SoundDev_DeInit()
{
	return(0);
}

int SoundDev_Init()
{
	SDL_AudioSpec wanted;
//	extern void fill_audio(void *udata, Uint8 *stream, int len);

	/* Set the audio format */
	wanted.freq=44100;
	wanted.format=AUDIO_S16;
	wanted.channels=2;	/* 1 = mono, 2 = stereo */
	wanted.samples=1024;  /* Good low-latency value for callback */
	wanted.callback=fill_audio;
	wanted.userdata=NULL;

	/* Open the audio device, forcing the desired format */
	if ( SDL_OpenAudio(&wanted, NULL) < 0 ) {
		printf("SoundDev_Init: Couldn't open audio: %s\n", SDL_GetError());
		return(-1);
	}
	
	audio_len=65536*2*sizeof(s16);
	audio_chunk=malloc(audio_len);
	audio_pos=0;

	return(0);
}

int SoundDev_PaintSamples(short *buffer, int cnt)
{
	int j0, j1, j2;
	int i, j, k, l, snb;
	int samples;
	short *buf, *rover;
	float f, g;

	buf=(short *)audio_chunk;
	rover=(short *)(audio_chunk+audio_pos);
	samples=audio_len/2;
	snb=1;

	while(((rover-buf)+cnt)>=samples)
	{
		l=samples-(rover-buf);
		memcpy(rover, buffer, l*sizeof(short));
		rover=buf;

		buffer+=l;
		cnt-=l;
		snb=0;
	}

	if(!snb)
	{
		if(cnt>0)
			{ memcpy(rover, buffer, cnt*sizeof(short)); }
		return(0);
	}

	if(cnt>0)
		memcpy(rover, buffer, cnt*sizeof(short));

	return(0);
}

void BGBMID_Sound_MixPopReduceStereo(short *buffer, int cnt)
{
	float f;
	int i, j, k, l;
	
	k=((int)buffer[-2]+(int)buffer[0])/2;
	l=((int)buffer[-1]+(int)buffer[1])/2;

	for(i=0; i<cnt; i++)
	{
		f=i/(float)cnt;
		buffer[i*2+0]=(buffer[i*2+0]*f)+(k*(1.0-f));
		buffer[i*2+1]=(buffer[i*2+1]*f)+(l*(1.0-f));
		buffer[-i*2+0]=(buffer[-i*2+0]*f)+(k*(1.0-f));
		buffer[-i*2+1]=(buffer[-i*2+1]*f)+(l*(1.0-f));
	}
}

void BGBMID_Sound_MixPopReduceStereo2(
	short *buffer, int size, int offs, int cnt)
{
	if(offs<(2*cnt))return;
	if((offs+(2*cnt))>=size)return;
	BGBMID_Sound_MixPopReduceStereo(buffer+offs, cnt);
}

int SoundDev_WriteRawSamples(short *buffer,
	int cnt, int ovcnt)
{
	static short *ldma;
	int i, l, d, d2, rs;
	int samples;
	float f, g;
	short *buf, *dma, *dma2;

//	printf("Writing %d samples\n", cnt);

	buf=(short *)audio_chunk;
	dma=(short *)(audio_chunk+audio_pos);
	samples=audio_len/2;
//	dma=buf+SoundDev_GetDMAPos();

	if(!snd_rover)snd_rover=dma;

	dma2=buf+(((dma-buf)+((44100/15)&(~3)))&(samples-1));

	rs=0;
	if((abs(snd_rover-dma2)/2)>(44100/10) && (dma2>ldma))
	{
//		printf("Sound: Resync %d %.3fs\n",
//			snd_rover-dma2, (snd_rover-dma2)/(2*44100.0));
		rs=1;

#if 1
		l=dma2-snd_rover;
		if(l>0)
		{
			for(i=0; i<l; i++)
				snd_rover[i]=0;
		}
#endif

		BGBMID_Sound_MixPopReduceStereo2(buf, samples, snd_rover-buf, 64);
		BGBMID_Sound_MixPopReduceStereo2(buf, samples, dma2-buf, 64);
		snd_rover=dma2;
	}

	ldma=dma2;

	SoundDev_PaintSamples(buffer, ovcnt);

	if(rs)
	{
		BGBMID_Sound_MixPopReduceStereo2(buf, samples, snd_rover-buf, 64);
	}

	snd_rover+=cnt;
	while((snd_rover-buf)>=samples)
		snd_rover-=samples;

	SoundDev_PaintSamples(buffer, ovcnt);

//	SoundDev_Submit();

	return(0);
}

// int SoundDev_WriteRawSamples(short *buffer,
//	int cnt, int ovcnt)
// {
//	return(0);
// }

int SoundDev_WriteStereoSamples(short *buffer, int cnt)
{
	return(SoundDev_WriteRawSamples(buffer, cnt*2, cnt*2));
}

int SoundDev_WriteStereoSamples2(
	short *buffer, int cnt, int ovcnt)
{
	return(SoundDev_WriteRawSamples(buffer, cnt*2, ovcnt*2));
}

int SoundDev_WriteSamples(short *buffer, int cnt)
{
	static short *tbuf=NULL;
	static int tbcnt=0;
	int i;
	
	if(!tbuf)
	{
		i=16384;
		while(cnt>i)i=i+(i>>1);
		tbuf=malloc(i*2*sizeof(short));
		tbcnt=i;
	}
	
	if(cnt>tbcnt)
	{
		i=tbcnt;
		while(cnt>i)i=i+(i>>1);
		tbuf=realloc(tbuf, i*2*sizeof(short));
		tbcnt=i;
	}
	
	for(i=0; i<cnt; i++)
	{
		tbuf[i*2+0]=buffer[i];
		tbuf[i*2+1]=buffer[i];
	}
	
	i=SoundDev_WriteRawSamples(tbuf, cnt*2, cnt*2);
	return(i);
}

#endif
