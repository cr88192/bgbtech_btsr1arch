#include "quakedef.h"

// #define BUFFER_CHAN		2
#define BUFFER_CHAN		1
#define BUFFER_SIZE		(BUFFER_CHAN*8192*2)

extern int			soundtime;		// sample PAIRS
extern int   		paintedtime; 	// sample PAIRS
extern int			endtime;		// sample PAIRS

unsigned char dma_buffer[BUFFER_SIZE];
unsigned char pend_buffer[BUFFER_SIZE];

qboolean SNDDMA_Init(void)
{
	u32 *sreg;

	sreg=(u32 *)0x007F8000;
	sreg[1]=11025;
	sreg[2]=255;
	sreg[3]=255;

	shm = &sn;
	shm->splitbuffer = 0;

	shm->speed = 11025;
	shm->samplebits = 16;
	shm->channels = BUFFER_CHAN;

	shm->soundalive = true;
	shm->samples = sizeof(dma_buffer) / (shm->samplebits/8);
	shm->samplepos = 0;
	shm->submission_chunk = 1;
	shm->buffer = (unsigned char *)dma_buffer;
	
	return(1);
}

static int wbufrov, dmarov=-1;

int SNDDMA_GetDevDMAPos(void)
{
	int *sreg;
	int idma;

	sreg=(int *)0x007F8000;
	idma=sreg[0];
	return(idma);
//	return(wbufrov);
}

int SNDDMA_GetDMAPos(void)
{
	shm->samplepos = (int)(realtime*shm->speed*shm->channels) & (shm->samples-1);
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
	int b, n, d;
	int i, j, k;

	dma=SNDDMA_GetDMAPos();
	idma=SNDDMA_GetDevDMAPos();
	tdma=idma+500;

	if(dmarov<0)
	{
		olddma=dma;
		wbufrov=dma;
//		wbufrov=paintedtime;
//		wbufrov=paintedtime-1000;
//		dmarov=dma+500;
		dmarov=tdma;
	}

	sbufl=(short *)0x007F0000;
	sbufr=(short *)0x007F4000;
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
	
	d=dmarov-tdma;
	d=(d<<(31-13))>>(31-13);
	d=d^(d>>31);
	if(d>2200)
		dmarov=tdma;
	
//	for(i=0; i<8192; i++)
	for(i=0; i<n; i++)
	{
//		j=(i+wbufrov)&8191;
		j=(i+b)&8191;
//		j=(i)&8191;
//		j=(paintedtime+i)&8191;
		k=(dmarov+i)&8191;
//		sbufl[i]=buf[i*2+0];
//		sbufr[i]=buf[i*2+1];
//		sbufl[j]=buf[i*2+0];
//		sbufr[j]=buf[i*2+1];

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

	olddma=dma;
	wbufrov+=n;
	dmarov+=n;
}
