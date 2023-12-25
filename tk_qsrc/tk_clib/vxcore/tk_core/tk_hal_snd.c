#define BUFFER_CHAN		2
#define BUFFER_SIZE		(BUFFER_CHAN*8192)

static short dma_buffer[BUFFER_SIZE];
static int dma_pos;

static int dma_chan;
static int dma_rate;
static int dma_bits;

void TKGDI_Snd_Submit(void);

void	TKGDI_Snd_SetupOutputMode(int chan, int rate, int bits)
{
	int rtm;
	
	dma_chan=chan;
	dma_rate=rate;
	dma_bits=bits;
	
	switch(rate)
	{
	case 8000:		rtm=0x0000;		break;
	case 11025:		rtm=0x0010;		break;
	case 16000:		rtm=0x0020;		break;
	case 22050:		rtm=0x0030;		break;
	case 32000:		rtm=0x0040;		break;
	case 44100:		rtm=0x0050;		break;
	case 64000:		rtm=0x0060;		break;
	case 88200:		rtm=0x0070;		break;
	case 48000:		rtm=0x0090;		break;
	case 96000:		rtm=0x00B0;		break;
	default:		rtm=0x0020;		break;
	}
	
//	if(bits==16)
//		rtm|=0x0003;
//	if(chan==1)
//		rtm|=0x0008;
//	if(chan==2)
//		rtm|=0x000C;

//	((u32 *)0xFFFFF009F000ULL)[0]=0x0029;
	((u32 *)0xFFFFF009F000ULL)[0]=rtm|0x0009;

}

void	TKGDI_Snd_WriteStereoSamples(short *mixbuf, int nsamp)
{
	TKGDI_Snd_WriteStereoSamples2(mixbuf, nsamp, nsamp*2);
}

void	TKGDI_Snd_WriteStereoSamples2(short *mixbuf, int nsamp, int nsamp2)
{
	short *dmabuf;
	int b;
	int i;
	
	dmabuf=dma_buffer;
	
//	b=dma_pos;
	b=(dma_pos+512)&8191;
//	b=(dma_pos+1024)&8191;
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

void	TKGDI_Snd_WriteMonoSamples2(short *mixbuf, int nsamp, int nsamp2)
{
	short *dmabuf;
	int b, v;
	int i;
	
	dmabuf=dma_buffer;
	
	b=(dma_pos+512)&8191;
	for(i=0; i<nsamp2; i++)
	{
		v=mixbuf[i];
		v=((u16)v)|(v<<16);
		((u32 *)dmabuf)[b]=v;
		b=(b+1)&8191;
	}
	
	dma_pos=(dma_pos+nsamp)&8191;
}

void	TKGDI_Snd_WriteMonoSamples2_8b(
	byte *mixbuf, int nsamp, int nsamp2)
{
	short *dmabuf;
	int b, v;
	int i;
	
	dmabuf=dma_buffer;
	
	b=(dma_pos+512)&8191;
	for(i=0; i<nsamp2; i++)
	{
		v=mixbuf[i];
		v=(v-128)<<8;
		v=((u16)v)|(v<<16);
		((u32 *)dmabuf)[b]=v;
		b=(b+1)&8191;
	}
	
	dma_pos=(dma_pos+nsamp)&8191;
}

void	TKGDI_Snd_WriteStereoSamples2_8b(
	byte *mixbuf, int nsamp, int nsamp2)
{
	short *dmabuf;
	int b, v, v0, v1;
	int i;
	
	dmabuf=dma_buffer;
	
	b=(dma_pos+512)&8191;
	for(i=0; i<nsamp2; i++)
	{
		v0=mixbuf[i*2+0];
		v1=mixbuf[i*2+1];
		v0=(v0-128)<<8;
		v1=(v1-128)<<8;
		v=((u16)v0)|(v1<<16);
		((u32 *)dmabuf)[b]=v;
		b=(b+1)&8191;
	}
	
	dma_pos=(dma_pos+nsamp)&8191;
}

void	TKGDI_Snd_WriteModeSamples2(void *mixbuf, int nsamp, int nsamp2)
{
	if((dma_chan==2) && (dma_bits==16))
	{
		TKGDI_Snd_WriteStereoSamples2(mixbuf, nsamp, nsamp2);
		return;
	}

	if((dma_chan==1) && (dma_bits==16))
	{
		TKGDI_Snd_WriteMonoSamples2(mixbuf, nsamp, nsamp2);
		return;
	}

	if((dma_chan==2) && (dma_bits==8))
	{
		TKGDI_Snd_WriteStereoSamples2_8b(mixbuf, nsamp, nsamp2);
		return;
	}

	if((dma_chan==1) && (dma_bits==8))
	{
		TKGDI_Snd_WriteMonoSamples2_8b(mixbuf, nsamp, nsamp2);
		return;
	}
	
	tk_printf("TKGDI_Snd_WriteModeSamples2: Bad Mode "
		"chan=%d rate=%d bits=%d\n",
		dma_chan, dma_rate, dma_bits);
}


void	TKGDI_Snd_Submit()
{
	TKGDI_Snd_Submit();
}


static int wbufrov, dmarov=-1;

int TKGDI_Snd_GetDevDMAPos(void)
{
	int *sreg;
	int idma;
	return(wbufrov);
}

int TKGDI_Snd_GetDMAPos(void)
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

#if 1
u32 sblk0_enc4x(u64 v);

__asm {
sblk0_enc4x:
	PCVTSW2H	R4, R6
	MOV			0xC200C200C200C200, R3
	PADD.H		R6, R3, R7
	PCVTH2AL	R7, R2
	
//	BREAK
	
	RTS
};

#else
u32 sblk0_enc4x(u64 v)
{
	u32 v1;
	int s0, s1, s2, s3;
	
	s0=sblk0_enc((s16)(v>> 0));
	s1=sblk0_enc((s16)(v>>16));
	s2=sblk0_enc((s16)(v>>32));
	s3=sblk0_enc((s16)(v>>48));
	v1=s0|(s1<<8)|(s2<<16)|(s3<<24);
	return(v1);
}
#endif

static volatile u32 *snd_dmabuf=NULL;
static byte snd_dmarov=0;
static u32 snd_dmapred=0;

static short snd_tsblk[256];

int TK_GetApproxMHz(void);

void TKGDI_Snd_Submit(void)
{
	static int olddma;
	u64 v0, v1, v2;
//	short *sbufl, *sbufr;
	short *buf;
	int dma, idma, tdma;
	int lv, rv;
	int s0, s1, s2, s3;
	int b, n, d, b1, n1, d1;
	int i, j, k, l;

	dma=TKGDI_Snd_GetDMAPos();
	idma=TKGDI_Snd_GetDevDMAPos();
	tdma=idma+500;

	if(dmarov<0)
	{
		olddma=dma;
		wbufrov=dma;
		dmarov=tdma;
	}

	buf=(short *)dma_buffer;
	
	snd_dmabuf=(u32 *)0xFFFFF0090000ULL;

	b=olddma;
	n=dma-b;
	b=dma;

	if(n<0)
		n+=8192;

	if(n<=0)
		return;
	
	d=dmarov-tdma;
	d=(d<<(31-13))>>(31-13);
	d=d^(d>>31);
	if(d>2200)
		dmarov=tdma;

#if 1
	b1=b&(~3); n1=(n+3)&(~3);
	d1=dmarov&(~3);

	((u32 *)0xFFFFF009F000ULL)[0]=0x0029;

	for(i=0; i<n1; i+=4)
	{
		j=(i+b1)&8191;
		k=(d1+i)&8191;

#if 0
		s0=sblk0_enc(buf[(j+0)*2+0]);
		s1=sblk0_enc(buf[(j+1)*2+0]);
		s2=sblk0_enc(buf[(j+2)*2+0]);
		s3=sblk0_enc(buf[(j+3)*2+0]);

		snd_dmabuf[k>>2]=s0|(s1<<8)|(s2<<16)|(s3<<24);
#endif

#if 1
		l=(j+0)*2;
		s0=buf[l+0];	s1=buf[l+2];
		s2=buf[l+4];	s3=buf[l+6];
		v0=	(((u64)(s0&0xFFFF))<< 0) |
			(((u64)(s1&0xFFFF))<<16) |
			(((u64)(s2&0xFFFF))<<32) |
			(((u64)(s3&0xFFFF))<<48) ;
		snd_dmabuf[k>>2]=sblk0_enc4x(v0);
#endif

	}
#endif

	olddma=dma;
	wbufrov+=n;
	dmarov+=n;
}

void TKGDI_Snd_SilenceAll(void)
{
	int i;
	
	for(i=0; i<2048; i++)
	{
		snd_dmabuf[i]=0;
	}
}