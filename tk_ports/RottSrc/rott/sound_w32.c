#include <stdio.h>

// #include <bteifgl.h>

#ifdef _WIN32

#include <windows.h>
#include <mmsystem.h>


//#define	WAV_BUFFERS				16
#define	WAV_BUFFERS				128
#define	WAV_MASK				(WAV_BUFFERS-1)
#define	WAV_BUFFER_SIZE			0x1000
#define SECONDARY_BUFFER_SIZE	0x10000

HWAVEOUT    hWaveOut; 
DWORD	gSndBufSize;

HANDLE		hData;
HPSTR		lpData, lpData2;

HGLOBAL		hWaveHdr;
LPWAVEHDR	lpWaveHdr;

static int	sample16, samples;
static int	snd_sent, snd_completed;
static short	*snd_rover;

int SoundDev_DeInit()
{
	int i;

	waveOutReset(hWaveOut);

	if(lpWaveHdr)
	{
		for(i=0; i<WAV_BUFFERS; i++)
			waveOutUnprepareHeader(hWaveOut, lpWaveHdr+i, sizeof(WAVEHDR));
	}

	waveOutClose(hWaveOut);

	if(hWaveHdr)
	{
		GlobalUnlock(hWaveHdr); 
		GlobalFree(hWaveHdr);
	}

	if(hData)
	{
		GlobalUnlock(hData);
		GlobalFree(hData);
	}

	return(0);
}

int SoundDev_Init()
{
	WAVEFORMATEX format; 
	int i, j;
	HRESULT hr;
	short *buf;

	snd_sent=0;
	snd_completed=0;

	memset(&format, 0, sizeof(format));
	format.wFormatTag=WAVE_FORMAT_PCM;
	format.nChannels=2;
	format.wBitsPerSample=16;
//	format.nSamplesPerSec=44100;
	format.nSamplesPerSec=16000;
	format.nBlockAlign=format.nChannels*format.wBitsPerSample/8;
	format.cbSize=0;
	format.nAvgBytesPerSec=format.nSamplesPerSec*format.nBlockAlign; 
	
	/* Open a waveform device for output using window callback. */ 
	while((hr=waveOutOpen((LPHWAVEOUT)&hWaveOut, WAVE_MAPPER, 
		&format, 0, 0L, CALLBACK_NULL))!=MMSYSERR_NOERROR)
	{
		printf("waveOutOpen failed\n");
		return(-1);
	} 

	gSndBufSize=WAV_BUFFERS*WAV_BUFFER_SIZE;
	hData=GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, gSndBufSize); 
	if(!hData) 
	{ 
		printf("Sound: Out of memory.\n");
		SoundDev_DeInit();
		return(-1); 
	}
	lpData=GlobalLock(hData);
	if(!lpData)
	{ 
		printf("Sound: Failed to lock.\n");
		SoundDev_DeInit();
		return(-1); 
	} 
	memset(lpData, 0, gSndBufSize);

	hWaveHdr=GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, 
		(DWORD) sizeof(WAVEHDR)*WAV_BUFFERS); 

	if(hWaveHdr==NULL)
	{ 
		printf("Sound: Failed to Alloc header.\n");
		SoundDev_DeInit();
		return(-1); 
	} 

	lpWaveHdr =(LPWAVEHDR) GlobalLock(hWaveHdr); 

	if(lpWaveHdr==NULL)
	{ 
		printf("Sound: Failed to lock header.\n");
		SoundDev_DeInit();
		return(-1); 
	}

	memset(lpWaveHdr, 0, sizeof(WAVEHDR)*WAV_BUFFERS);

	// After allocation, set up and prepare headers.
	for(i=0; i<WAV_BUFFERS; i++)
	{
		lpWaveHdr[i].dwBufferLength=WAV_BUFFER_SIZE; 
		lpWaveHdr[i].lpData=lpData+i*WAV_BUFFER_SIZE;

		if(waveOutPrepareHeader(hWaveOut, lpWaveHdr+i, sizeof(WAVEHDR)) !=
				MMSYSERR_NOERROR)
		{
			printf("Sound: Failed to prepare wave headers\n");
			SoundDev_DeInit();
			return(-1);
		}
	}

	samples=gSndBufSize/(16/8);
	sample16 =(16/8)-1;

	return(0);
}

int SoundDev_GetDMAPos()
{
	MMTIME tm;
	int i, s;

	s=snd_sent*WAV_BUFFER_SIZE;
	s>>=sample16;
	s&=(samples-1);

	return(s);
}

void SoundDev_Submit()
{
	LPWAVEHDR	h;
	int			wResult;

	while(1)
	{
		if(snd_completed==snd_sent)
		{
//			printf("Sound overrun\n");
			break;
		}

		if(!(lpWaveHdr[snd_completed&WAV_MASK].dwFlags&WHDR_DONE))
			break;

		snd_completed++;	// this buffer has been played
	}

	while(((snd_sent-snd_completed)>>sample16)<4)
//	while(((snd_sent-snd_completed)>>sample16)<16)
	{
		h=lpWaveHdr+(snd_sent&WAV_MASK);

		snd_sent++;

		wResult=waveOutWrite(hWaveOut, h, sizeof(WAVEHDR)); 

		if(wResult!=MMSYSERR_NOERROR)
		{ 
			printf("Sound: Failed to write block to device\n");
			SoundDev_DeInit();
			return; 
		} 
	}
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

int SoundDev_PaintSamples(short *buffer, int cnt)
{
	int j0, j1, j2;
	int i, j, k, l, snb;
	short *buf, *rover;
	float f, g;

//	printf("Writing %d samples\n", cnt);

	buf=(short *)lpData;
	rover=snd_rover;
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
//	if(1)
	{
		if(cnt>0)
			{ memcpy(rover, buffer, cnt*sizeof(short)); }
		return(0);
	}

	memcpy(rover, buffer, cnt*sizeof(short));
#if 0
	if(cnt>=16)
	{
		for(i=0; i<16; i++)
		{
			j=i;
			if(j>=16)j=16;
			j0=rover[i];
			j1=buffer[i];
//			j2=((j0*(16-j))+(j1*j))/16;
//			j2=j1;
			j2=(j0+j1)/2;
//			j2=j0*(j/16.0)+j1*(1.0-(j/16.0));
			rover[i]=j2;
		}
		for(; i<cnt; i++)
			{ rover[i]=buffer[i]; }
//		memcpy(rover+16, buffer+16, (cnt-16)*sizeof(short));
	}else
	{
//		for(i=0; i<cnt; i++)
//		{
//			rover[i]=((rover[i]*(16-i))+(buffer[i]*i))>>4;
//		}
	}
#endif

#if 0
	if(snb && (rover>(buf+32)) && ((rover+32)<(buf+samples)))
	{
#if 0
		k=(rover[-2]+rover[-1]+rover[0]+rover[1])/4;
		for(i=0; i<32; i++)
		{
			f=i/32.0;
			rover[i]=(rover[i]*f)+(k*(1.0-f));
			rover[-i]=(rover[-i]*f)+(k*(1.0-f));
		}
#endif

#if 1
		k=(rover[-2]+rover[0])/2;
		l=(rover[-1]+rover[1])/2;
		for(i=-15; i<=15; i++)
		{
			f=fabs(i/16.0);
			rover[i*2+0]=(rover[i*2+0]*f)+(k*(1.0-f));
			rover[i*2+1]=(rover[i*2+1]*f)+(l*(1.0-f));
		}
#endif
	}
#endif

	return(0);
}

int SoundDev_WriteRawSamples(short *buffer,
	int cnt, int ovcnt)
{
	int i, l, d, d2, rs;
	float f, g;
	short *buf, *dma, *dma2;
	static short *ldma;

//	printf("Writing %d samples\n", cnt);

	buf=(short *)lpData;
	dma=buf+SoundDev_GetDMAPos();

	if(!snd_rover)snd_rover=dma;

//	dma2=buf+(((dma-buf)+((44100/40)&(~3)))&(samples-1));
//	dma2=buf+(((dma-buf)+((44100/10)&(~3)))&(samples-1));
//	dma2=buf+(((dma-buf)+((44100/20)&(~3)))&(samples-1));
	dma2=buf+(((dma-buf)+((44100/15)&(~3)))&(samples-1));
//	dma2=buf+(((dma-buf)+(44100/20))&(samples-1));
//	dma2=buf+(((dma-buf)+44100/10)&(samples-1));
//	d=abs(snd_rover-dma2);
//	d2=abs(snd_rover-(dma2+samples));
//	if(d2<d)d=d2;

//	dma2=dma;

	rs=0;
//	if((abs(snd_rover-dma2)/2)>(44100/5) && (dma2>ldma))
	if((abs(snd_rover-dma2)/2)>(44100/10) && (dma2>ldma))
//	if((abs(snd_rover-dma2)/2)>(44100/15) && (dma2>ldma))
//	if((d/2)>(44100/10) && (dma2>ldma))
//	if((abs(snd_rover-dma)/2)>(44100/10) && (dma>ldma))
//	if((abs(snd_rover-dma)/2)>(44100/15) && (dma>ldma))
//	if((abs(snd_rover-dma)/2)>(44100/20) && (dma>ldma))
	{
//		printf("Sound: Resync %d\n", snd_rover-dma);
//		printf("Sound: Resync %d\n", snd_rover-dma2);
		printf("Sound: Resync %d %.3fs\n",
			(int)(snd_rover-dma2), (snd_rover-dma2)/(2*44100.0));
		rs=1;

#if 1
//		l=dma-snd_rover;
		l=dma2-snd_rover;
		if(l>0)
		{
			for(i=0; i<l; i++)
				snd_rover[i]=0;
		}
#endif

//		BGBMID_Sound_MixPopReduceStereo2(buf, samples, snd_rover-buf, 64);
//		BGBMID_Sound_MixPopReduceStereo2(buf, samples, dma-buf, 64);

		BGBMID_Sound_MixPopReduceStereo2(buf, samples, snd_rover-buf, 64);
		BGBMID_Sound_MixPopReduceStereo2(buf, samples, dma2-buf, 64);

//		BGBMID_Sound_MixPopReduceStereo(snd_rover, 64);
//		BGBMID_Sound_MixPopReduceStereo(dma, 64);

//		snd_rover=dma;
		snd_rover=dma2;
	}else
	{
//		printf("Sync=%d ", snd_rover-dma2);
	}
	ldma=dma2;

	SoundDev_PaintSamples(buffer, ovcnt);

	if(rs)
	{
//		BGBMID_Sound_MixPopReduceStereo(snd_rover, 64);
		BGBMID_Sound_MixPopReduceStereo2(buf, samples, snd_rover-buf, 64);
	}

#if 0
	if(rs)
	{
		for(i=0; i<16; i++)
		{
			f=i/16;
//			snd_rover[i*2+0]*=f;	snd_rover[i*2+1]*=f;
//			snd_rover[-i*2+0]*=f;	snd_rover[-i*2+1]*=f;
			dma[i*2+0]*=f;			dma[i*2+1]*=f;
			dma[-i*2+0]*=f;			dma[-i*2+1]*=f;
		}
	}
#endif

	snd_rover+=cnt;
	while((snd_rover-buf)>=samples)
		snd_rover-=samples;

	SoundDev_PaintSamples(buffer, ovcnt);

//	BGBMID_Sound_MixPopReduceStereo(snd_rover, 16);

	SoundDev_Submit();

	return(0);
}

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

