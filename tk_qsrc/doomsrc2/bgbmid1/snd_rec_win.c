#include <stdio.h>

#include <bgbmid.h>

#ifdef _WIN32

#include <windows.h>
#include <mmsystem.h>


#define	WAV_BUFFERS				32
#define	WAV_MASK				(WAV_BUFFERS-1)
#define	WAV_BUFFER_SIZE			0x2000
// #define SECONDARY_BUFFER_SIZE	0x10000

HWAVEIN    hWaveIn; 
DWORD	gSndInBufSize;

HANDLE		hInData;
HPSTR		lpInData, lpInData2;

HGLOBAL		hInWaveHdr;
LPWAVEHDR	lpInWaveHdr;

static int	sample16, samples;
static int	snd_sent, snd_completed;
static short	*snd_rover;

BMID_API int BGBMID_SoundDev_RecDeInit()
{
	int i;

	waveInStop(hWaveIn);

	waveInReset(hWaveIn);

	if(lpInWaveHdr)
	{
		for(i=0; i<WAV_BUFFERS; i++)
			waveInUnprepareHeader(hWaveIn, lpInWaveHdr+i, sizeof(WAVEHDR));
	}

	waveInClose(hWaveIn);

	if(hInWaveHdr)
	{
		GlobalUnlock(hInWaveHdr); 
		GlobalFree(hInWaveHdr);
	}

	if(hInData)
	{
		GlobalUnlock(hInData);
		GlobalFree(hInData);
	}

	return(0);
}

BMID_API int BGBMID_SoundDev_RecInit()
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
	format.nSamplesPerSec=44100;
	format.nBlockAlign=format.nChannels*format.wBitsPerSample/8;
	format.cbSize=0;
	format.nAvgBytesPerSec=format.nSamplesPerSec*format.nBlockAlign; 
	
	/* Open a waveform device for output using window callback. */ 
	while((hr=waveInOpen((LPHWAVEIN)&hWaveIn, WAVE_MAPPER, 
		&format, 0, 0L, CALLBACK_NULL))!=MMSYSERR_NOERROR)
	{
		printf("SoundRec: waveInOpen failed\n");
		return(-1);
	} 

	gSndInBufSize=WAV_BUFFERS*WAV_BUFFER_SIZE;
	hInData=GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, gSndInBufSize); 
	if(!hInData) 
	{ 
		printf("SoundRec: Out of memory.\n");
		BGBMID_SoundDev_RecDeInit();
		return(-1); 
	}
	lpInData=GlobalLock(hInData);
	if(!lpInData)
	{ 
		printf("SoundRec: Failed to lock.\n");
		BGBMID_SoundDev_RecDeInit();
		return(-1); 
	} 
	memset(lpInData, 0, gSndInBufSize);

	hInWaveHdr=GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, 
		(DWORD) sizeof(WAVEHDR)*WAV_BUFFERS); 

	if(hInWaveHdr==NULL)
	{ 
		printf("Sound: Failed to Alloc header.\n");
		BGBMID_SoundDev_RecDeInit();
		return(-1); 
	} 

	lpInWaveHdr =(LPWAVEHDR) GlobalLock(hInWaveHdr); 

	if(lpInWaveHdr==NULL)
	{ 
		printf("SoundRec: Failed to lock header.\n");
		BGBMID_SoundDev_RecDeInit();
		return(-1); 
	}

	memset(lpInWaveHdr, 0, sizeof(WAVEHDR)*WAV_BUFFERS);

	// After allocation, set up and prepare headers.
	for(i=0; i<WAV_BUFFERS; i++)
	{
		lpInWaveHdr[i].dwBufferLength=WAV_BUFFER_SIZE; 
		lpInWaveHdr[i].lpData=lpInData+i*WAV_BUFFER_SIZE;

		if(waveInPrepareHeader(hWaveIn, lpInWaveHdr+i, sizeof(WAVEHDR)) !=
				MMSYSERR_NOERROR)
		{
			printf("SoundRec: Failed to prepare wave headers\n");
			BGBMID_SoundDev_RecDeInit();
			return(-1);
		}
	}

	samples=gSndInBufSize/(16/8);
	sample16 =(16/8)-1;

	waveInStart(hWaveIn);

	return(0);
}

BMID_API int BGBMID_SoundDev_GetRecDMAPos()
{
	MMTIME tm;
	int i, s;

//	s=snd_sent*WAV_BUFFER_SIZE;
	s=snd_completed*WAV_BUFFER_SIZE;
	s>>=sample16;
	s&=(samples-1);

	return(s);
}

BMID_API void BGBMID_SoundDev_RecSubmit()
{
	LPWAVEHDR	h;
	int			wResult;

	while(1)
	{
		if(snd_completed==snd_sent)
		{
			printf("SoundRec: Sound overrun\n");
			break;
		}

		if(!(lpInWaveHdr[snd_completed&WAV_MASK].dwFlags&WHDR_DONE))
			break;

		snd_completed++;	// this buffer has been played
	}

	while(((snd_sent-snd_completed)>>sample16)<4)
//	while(((snd_sent-snd_completed)>>sample16)<16)
	{
		h=lpInWaveHdr+(snd_sent&WAV_MASK);

		snd_sent++;

//		wResult=waveInWrite(hWaveIn, h, sizeof(WAVEHDR)); 
		wResult=waveInAddBuffer(hWaveIn, h, sizeof(WAVEHDR)); 

		if(wResult!=MMSYSERR_NOERROR)
		{ 
			printf("SoundRec: Failed to send block to device\n");
			BGBMID_SoundDev_RecDeInit();
			return; 
		} 
	}
}

BMID_API int BGBMID_SoundDev_RecCopySamples(short *buffer, int cnt)
{
	int i, j, k, l, snb;
	short *buf, *rover;
	float f, g;

	buf=(short *)lpInData;
	rover=snd_rover;
	snb=1;

	while(((rover-buf)+cnt)>=samples)
	{
		l=samples-(rover-buf);
//		memcpy(rover, buffer, l*sizeof(short));
		memcpy(buffer, rover, l*sizeof(short));
		rover=buf;

		buffer+=l;
		cnt-=l;
		snb=0;
	}
	if(cnt>0)
	{
//		memcpy(rover, buffer, cnt*sizeof(short));
		memcpy(buffer, rover, cnt*sizeof(short));
	}

	return(0);
}

BMID_API int BGBMID_SoundDev_RecReadRawSamples(short *buffer,
	int cnt, int ovcnt)
{
	int i, l, d, d2, rs;
	float f, g;
	short *buf, *dma, *dma2;
	static short *ldma;

//	printf("Writing %d samples\n", cnt);

	buf=(short *)lpInData;
	dma=buf+BGBMID_SoundDev_GetRecDMAPos();

	if(!snd_rover)snd_rover=dma;

//	dma2=buf+(((dma-buf)+((44100/20)&(~3)))&(samples-1));
	dma2=buf+(((dma-buf)-((44100/20)&(~3)))&(samples-1));
//	dma2=dma;
//	dma2=buf+(((dma-buf)-((44100/10)&(~3)))&(samples-1));

	rs=0;
	if((abs(snd_rover-dma2)/2)>(44100/10) && (dma2>ldma))
	{
//		printf("Sound: Resync %d\n", snd_rover-dma);
//		printf("SoundRec: Resync %d\n", snd_rover-dma2);
		rs=1;

#if 0
//		l=dma-snd_rover;
		l=dma2-snd_rover;
		if(l>0)
		{
			for(i=0; i<l; i++)
				snd_rover[i]=0;
		}
#endif

//		BGBMID_Sound_MixPopReduceStereo2(buf, samples, snd_rover-buf, 64);
//		BGBMID_Sound_MixPopReduceStereo2(buf, samples, dma2-buf, 64);

//		snd_rover=dma;
		snd_rover=dma2;
	}else
	{
//		printf("Sync=%d ", snd_rover-dma2);
	}
	ldma=dma2;

	BGBMID_SoundDev_RecCopySamples(buffer, ovcnt);

//	f=0;
//	for(i=0; i<cnt; i++)
//		f+=buffer[i];
//	f=f/cnt;
//	printf("SoundRec: Vol %f\n", f);

	if(rs)
	{
//		BGBMID_Sound_MixPopReduceStereo(snd_rover, 64);
//		BGBMID_Sound_MixPopReduceStereo2(buf, samples, snd_rover-buf, 64);
	}

	snd_rover+=cnt;
	while((snd_rover-buf)>=samples)
		snd_rover-=samples;

//	BGBMID_SoundDev_PaintSamples(buffer, ovcnt);

	BGBMID_SoundDev_RecSubmit();

	return(0);
}

BMID_API int BGBMID_SoundDev_RecReadStereoSamples(short *buffer, int cnt)
{
	return(BGBMID_SoundDev_RecReadRawSamples(buffer, cnt*2, cnt*2));
}

BMID_API int BGBMID_SoundDev_RecReadStereoSamples2(
	short *buffer, int cnt, int ovcnt)
{
	return(BGBMID_SoundDev_RecReadRawSamples(buffer, cnt*2, ovcnt*2));
}

#if 0
BMID_API int BGBMID_SoundDev_PaintSamples(short *buffer, int cnt)
{
	int i, j, k, l, snb;
	short *buf, *rover;
	float f, g;

//	printf("Writing %d samples\n", cnt);

	buf=(short *)lpInData;
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
	if(cnt>0)memcpy(rover, buffer, cnt*sizeof(short));

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

BMID_API int BGBMID_SoundDev_WriteRawSamples(short *buffer,
	int cnt, int ovcnt)
{
	int i, l, d, d2, rs;
	float f, g;
	short *buf, *dma, *dma2;
	static short *ldma;

//	printf("Writing %d samples\n", cnt);

	buf=(short *)lpInData;
	dma=buf+BGBMID_SoundDev_GetRecDMAPos();

	if(!snd_rover)snd_rover=dma;

//	dma2=buf+(((dma-buf)+((44100/40)&(~3)))&(samples-1));
//	dma2=buf+(((dma-buf)+((44100/10)&(~3)))&(samples-1));
	dma2=buf+(((dma-buf)+((44100/20)&(~3)))&(samples-1));
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

	BGBMID_SoundDev_PaintSamples(buffer, ovcnt);

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

	BGBMID_SoundDev_PaintSamples(buffer, ovcnt);

//	BGBMID_Sound_MixPopReduceStereo(snd_rover, 16);

	BGBMID_SoundDev_RecSubmit();

	return(0);
}

BMID_API int BGBMID_SoundDev_WriteStereoSamples(short *buffer, int cnt)
{
	return(BGBMID_SoundDev_WriteRawSamples(buffer, cnt*2, cnt*2));
}

BMID_API int BGBMID_SoundDev_WriteStereoSamples2(
	short *buffer, int cnt, int ovcnt)
{
	return(BGBMID_SoundDev_WriteRawSamples(buffer, cnt*2, ovcnt*2));
}

BMID_API int BGBMID_SoundDev_WriteSamples(short *buffer, int cnt)
{
	static short *tbuf=NULL;
	static int tbcnt=0;
	int i;
	
	if(!tbuf)
	{
		i=16384;
		while(cnt>i)i=i+(i>>1);
		tbuf=bgbmid_malloc(i*2*sizeof(short));
		tbcnt=i;
	}
	
	if(cnt>tbcnt)
	{
		i=tbcnt;
		while(cnt>i)i=i+(i>>1);
		tbuf=bgbmid_realloc(tbuf, i*2*sizeof(short));
		tbcnt=i;
	}
	
	for(i=0; i<cnt; i++)
	{
		tbuf[i*2+0]=buffer[i];
		tbuf[i*2+1]=buffer[i];
	}
	
	i=BGBMID_SoundDev_WriteRawSamples(tbuf, cnt*2, cnt*2);
	return(i);
}

#endif

#endif
