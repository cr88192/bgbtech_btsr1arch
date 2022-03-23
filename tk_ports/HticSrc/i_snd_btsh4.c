#include "doomdef.h"

#define BUFFER_CHAN		2
// #define BUFFER_CHAN		1
//#define BUFFER_SIZE		(BUFFER_CHAN*8192*2)
#define BUFFER_SIZE		(BUFFER_CHAN*8192)

//unsigned char dma_buffer[BUFFER_SIZE];
short dma_buffer[BUFFER_SIZE+256];
// unsigned char pend_buffer[BUFFER_SIZE];

int dma_pos;
volatile u32 *snd_dmabuf=NULL;

static int wbufrov, dmarov=-1;

void	SoundDev_WriteStereoSamples2(short *mixbuf, int nsamp, int nsamp2);
void SNDDMA_Submit(void);

void	SoundDev_WriteStereoSamples(short *mixbuf, int nsamp)
{
	SoundDev_WriteStereoSamples2(mixbuf, nsamp, nsamp*2);
}

void	SoundDev_WriteStereoSamples2(short *mixbuf, int nsamp, int nsamp2)
{
	short *dmabuf;
	int b;
	int i;
	
//	printf("ns=%d\n", nsamp);
	
	dmabuf=dma_buffer;
	
//	b=dma_pos;
//	b=(dma_pos+512)&8191;
//	b=(dma_pos+1024)&8191;
	b=(dma_pos+2048)&8191;
//	b=(dma_pos+3072)&8191;
	for(i=0; i<nsamp2; i++)
	{
//		dma_buffer[b*2+0]=mixbuf[i*2+0];
//		dma_buffer[b*2+1]=mixbuf[i*2+1];
//		dmabuf[b*2+0]=mixbuf[i*2+0];
//		dmabuf[b*2+1]=mixbuf[i*2+1];
		((u32 *)dmabuf)[b]=((u32 *)mixbuf)[i];
		b=(b+1)&8191;
	}
	
	dma_pos=(dma_pos+nsamp)&8191;
}

void	SoundDev_Submit()
{
	SNDDMA_Submit();
}

void	SoundDev_Init()
{
	dma_pos=0;
	dmarov=-1;
}

int SNDDMA_GetDevDMAPos(void)
{
//	int *sreg;
//	int idma;
//	return(wbufrov);

	int idma;
	idma=((u32 *)0xFFFFF009F000ULL)[8];
//	return(idma&8191);
	return(idma);
}

int SNDDMA_GetDMAPos(void)
{
	u32 ms;
	return(dma_pos);
}

int sblk0_enc(int v)
{
	int e, v0, v1;

	if(v<0)
	{
		v1=sblk0_enc(-v);
		return(0x80|v1);
	}
	
	if(v>=256)
	{
		e=0;
		v0=v>>3;
		if(v0&(~31))	{ v0=v0>>1; e++; }
		if(v0&(~31))	{ v0=v0>>1; e++; }
		if(v0&(~31))	{ v0=v0>>1; e++; }
		if(v0&(~31))	{ v0=v0>>1; e++; }
		if(v0&(~31))	{ v0=v0>>1; e++; }
		if(v0&(~31))	{ v0=v0>>1; e++; }
		if(v0&(~31))	{ v0=v0>>1; e++; }
//		if(v0&(~31))	{ v0=v0>>1; e++; }
//		if(v0&(~31))	{ v0=v0>>1; e++; }
//		if(v0&(~31))	{ v0=v0>>1; e++; }

		v1=(e<<4)|(v0&15);
		if(v0&(~31))	{ v1=0x7F; }
//		if(e>7)
//			v1=0x7F;
	}else
	{
		v1=v>>4;
	}
	
	return(v1);
}

void SNDDMA_Submit(void)
{
	static int olddma;
	short *buf;
	int dma, idma, tdma;
	int lv, rv;
	int s0, s1, s2, s3;
	int b, n, d, b1, n1, d1;
	int i, j, k, l;

	buf=(short *)dma_buffer;
	snd_dmabuf=(u32 *)0xFFFFF0090000ULL;

	dma=SNDDMA_GetDMAPos();
	idma=SNDDMA_GetDevDMAPos();
//	tdma=idma+500;
//	tdma=(idma+1024)&8191;
//	tdma=(idma+0)&8191;
	tdma=(idma+2048)&8191;
//	tdma=(idma+4096)&8191;

	if(dmarov<0)
	{
		olddma=dma;
		wbufrov=dma;
		dmarov=tdma;

//		olddma=tdma;
//		wbufrov=tdma;
//		dma_pos=tdma;
	}

	b=olddma;
	n=dma-b;
	b=dma;

	if(n<0)
		n+=8192;

	if(n<=0)
	{
		olddma=dma;
		return;
	}

	d=dmarov-tdma;
//	d=(d<<(31-13))>>(31-13);
//	d=(d<<(31-12))>>(31-12);
	d=d^(d>>31);
	if(d>4096)
		d=8192-d;
//	if(d>2200)
	if(d>3072)
	{
		printf("Snd RS1 p=%d hw=%d d=%d\n", dmarov, tdma, d);
		dmarov=tdma;
//		olddma=tdma;
//		wbufrov=tdma;
//		dma_pos=tdma;
	}

#if 1
	b1=b&(~3); n1=(n+3)&(~3);
	d1=dmarov&(~3);

//	((u32 *)0xF009F000)[0]=0x002B;
//	((u32 *)0xF009F000)[0]=0x002A;
//	((u32 *)0xF009F000)[0]=0x0028;
	((u32 *)0xFFFFF009F000ULL)[0]=0x0029;
//	for(i=0; i<n; i+=2)
//	for(i=0; i<n; i+=4)
	for(i=0; i<n1; i+=4)
	{
//		j=(i+b)&8191;
		j=(i+b1)&8191;
//		k=(dmarov+i)&8191;
//		k=(dmarov+i)&16383;
//		k=(d1+i)&16383;
		k=(d1+i)&8191;
//		k=(d1+i)&8191;

		s0=sblk0_enc(buf[(j+0)*2+0]);
		s1=sblk0_enc(buf[(j+1)*2+0]);
		s2=sblk0_enc(buf[(j+2)*2+0]);
		s3=sblk0_enc(buf[(j+3)*2+0]);

		snd_dmabuf[k>>2]=s0|(s1<<8)|(s2<<16)|(s3<<24);
	}
#endif

	olddma=dma;
	wbufrov+=n;
//	dmarov+=n;
	dmarov=(dmarov+n)&8191;
}
