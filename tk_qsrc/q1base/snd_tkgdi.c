#include "quakedef.h"

#include <tkgdi/tkgdi.h>

// #define BUFFER_CHAN		2
#define BUFFER_CHAN		1
#define BUFFER_SIZE		(BUFFER_CHAN*8192*2)

#define SAMPLERATE		16000

extern int			soundtime;		// sample PAIRS
extern int   		paintedtime; 	// sample PAIRS
extern int			endtime;		// sample PAIRS

// unsigned char dma_buffer[BUFFER_SIZE];
unsigned char *dma_buffer;
unsigned char pend_buffer[BUFFER_SIZE];

TKGHSND hSndDev;
TKGDI_WAVEFORMATEX i_snd_t_info;
TKGDI_WAVEFORMATEX *i_snd_info = NULL;

qboolean SNDDMA_Init(void)
{
	u32 *sreg;

	if(!i_snd_info)
	{
		i_snd_info = &i_snd_t_info;
		memset(i_snd_info, 0, sizeof(TKGDI_WAVEFORMATEX));
		
		i_snd_info->wFormatTag=TKGDI_WAVE_FORMAT_PCM;
		i_snd_info->nChannels=BUFFER_CHAN;
		i_snd_info->nSamplesPerSec=SAMPLERATE;
		i_snd_info->nAvgBytesPerSec=SAMPLERATE*(BUFFER_CHAN*2);
		i_snd_info->nBlockAlign=(BUFFER_CHAN*2);
		i_snd_info->wBitsPerSample=16;
		i_snd_info->cbSize=sizeof(TKGDI_WAVEFORMATEX);

		hSndDev = tkgCreateAudioDevice(0, TKGDI_FCC_auds, i_snd_info);
		dma_buffer = tkgGlobalAlloc(BUFFER_SIZE);
	}

//	sreg=(u32 *)0x007F8000;
//	sreg[1]=11025;
//	sreg[2]=255;
//	sreg[3]=255;

	shm = &sn;
	shm->splitbuffer = 0;

//	shm->speed = 11025;
	shm->speed = SAMPLERATE;
	shm->samplebits = 16;
	shm->channels = BUFFER_CHAN;

	shm->soundalive = true;
//	shm->samples = sizeof(dma_buffer) / (shm->samplebits/8);
	shm->samples = BUFFER_SIZE / (shm->samplebits/8);
	shm->samplepos = 0;
	shm->submission_chunk = 1;
	shm->buffer = (unsigned char *)dma_buffer;
	
	return(1);
}

static int wbufrov, dmarov=-1;

#if 0
int SNDDMA_GetDevDMAPos(void)
{
	int *sreg;
	int idma;

	sreg=(int *)0x007F8000;
	idma=sreg[0];
	return(idma);
//	return(wbufrov);
}
#endif

int SNDDMA_GetDMAPos(void)
{
	shm->samplepos = (int)(realtime*shm->speed*shm->channels) &
		(shm->samples-1);
	return shm->samplepos;
}

int SNDDMA_GetSamples(void)
{
	return(0);
}

void SNDDMA_Shutdown(void)
{
}

void SNDDMA_Submit(void)
{
	static int olddma;
	short *sbufl, *sbufr;
	short *buf;
	int dma, idma, tdma;
	int b, n, d, b1, n1, n2;
	int i, j, k;

	dma=SNDDMA_GetDMAPos();
//	idma=SNDDMA_GetDevDMAPos();
//	tdma=idma+500;
	tdma=dma;

	if(dmarov<0)
	{
		olddma=dma;
		wbufrov=dma;
//		wbufrov=paintedtime;
//		wbufrov=paintedtime-1000;
//		dmarov=dma+500;
		dmarov=tdma;
	}

//	sbufl=(short *)0x007F0000;
//	sbufr=(short *)0x007F4000;
	buf=(short *)dma_buffer;
	
//	b=soundtime;
//	b=0;
//	b=wbufrov;
//	n=paintedtime-b;
//	n=(paintedtime/2)-b;

//	b=olddma/2;
//	n=(dma/2)-b;

	b=olddma;
	n=dma-b;
	b=dma;

//	n = (paintedendtime - paintedtime) * shm->channels;

	if(n<0)
		n+=8192;

	if(n<0)
		return;

//	b=paintedtime;
//	n=endtime-b;
	
//	d=dmarov-tdma;
//	d=(d<<(31-13))>>(31-13);
//	d=d^(d>>31);
//	if(d>2200)
//		dmarov=tdma;

#if 0
//	for(i=0; i<8192; i++)
	for(i=0; i<n; i++)
	{
		j=(i+b)&8191;
		k=(dmarov+i)&8191;

		if(BUFFER_CHAN==2)
		{
			sbufl[k]=buf[j*2+0];
			sbufr[k]=buf[j*2+1];
		}else
		{
			sbufl[k]=buf[j];
			sbufr[k]=buf[j];
		}
	}
#endif

	b1=b&8191;

	if(BUFFER_CHAN==2)
	{
		if((b1+n)>8192)
		{
			n1=8192-b1;
			n2=n-n1;
			tkgWriteSamples(hSndDev, buf+b1, n1, n1);
			tkgWriteSamples(hSndDev, buf+0, n2, n2);
		}else if((b1+(n*2))<8192)
		{
			tkgWriteSamples(hSndDev, buf+b1, n, n*2);
		}else
		{
			tkgWriteSamples(hSndDev, buf+b1, n, n);
		}
	}else
	{
		if((b1+n)>8192)
		{
			n1=8192-b1;
			n2=n-n1;
			tkgWriteSamples(hSndDev, buf+b1, n1, n1);
			tkgWriteSamples(hSndDev, buf+0, n2, n2);
		}else if((b1+(n*2))<8192)
		{
			tkgWriteSamples(hSndDev, buf+b1, n, n*2);
		}else
		{
			tkgWriteSamples(hSndDev, buf+b1, n, n);
		}
	}

	olddma=dma;
	wbufrov+=n;
	dmarov+=n;
}
