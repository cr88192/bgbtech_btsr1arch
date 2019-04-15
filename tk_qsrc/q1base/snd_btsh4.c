#include "quakedef.h"

// #define BUFFER_CHAN		2
#define BUFFER_CHAN		1
#define BUFFER_SIZE		(BUFFER_CHAN*8192*2)

extern int			soundtime;		// sample PAIRS
extern int   		paintedtime; 	// sample PAIRS
extern int			endtime;		// sample PAIRS

unsigned char dma_buffer[BUFFER_SIZE];
// unsigned char dma_buffer[BUFFER_SIZE*2];
// unsigned char pend_buffer[BUFFER_SIZE];


int Sblkau_SampToPred9(int val)
{
	int v, ex, sg;
	
	if(val>=0)
		{ v=val; sg=0; }
	else
		{ v=-val; sg=1; }
	
	ex=0; v=v>>2;
	if(v>=64)
	{
		while(v>=64)
			{ ex++; v=v>>1; }
		if(ex>7)
			{ ex=7; v=63; }
	}else
	{
		v=(v>>1);
	}
	return((sg<<8)|(ex<<5)|(v&31));
}

int Sblkau_Pred9ToSamp(int val)
{
	int sg, ex, fr, v;
	
	sg=(val>>8)&1;
	ex=(val>>5)&7;
	if(ex)
		fr=32|(val&31);
	else
		fr=(val&31);
	v=fr<<(ex+2);
	if(sg)v=~v;
	return(v);
}

static int sblkau_rangetab[64]=
{
       0,     1,     2,     3,
       4,     5,     6,     7,
       8,    10,    12,    14,
      16,    20,    24,    28,
      32,    40,    48,    56,
      64,    80,    96,   112,
     128,   160,   192,   224,
     256,   320,   384,   448,
     512,   640,   768,   960,
    1024,  1280,  1536,  1920,
    2048,  2560,  3072,  3840,
    4096,  5120,  6144,  7680,
    8192, 10240, 12288, 15360,
   16384, 20480, 24576, 30720,
   32768, 40960, 49152, 61440,
   65536, 81920, 98304,     0
};

int sblkau_stat[16];

int Sblkau_StepForSamp(int val)
{
	int st, ra, v;
	
	v=val;
	if(v<0)v=-v;
	
	st=1;
	ra=sblkau_rangetab[st];
//	ra|=ra>>4;
//	ra|=ra>>8;
//	ra--;
//	ra*=0.75;

	while(v>(ra>>1))
	{
		st++;
		ra=sblkau_rangetab[st];
	}
	
	return(st);
}

int Sblkau_DeltaForSamp(int ss, int dsv)
{
	int v, ra;

	ra=sblkau_rangetab[ss&63];

	if(dsv&4)
	{
		v=-(ra*((dsv&3)*2+1))/7;
//		v=-((ra*((dsv&3)+1))>>2);
	}
	else
	{
		v=(ra*((dsv&3)*2+1))/7;
//		v=((ra*((dsv&3)+1))>>2);
	}
	return(v);
}

int Sblkau_SampForDelta(int ss, int pr, int val)
{
	int ra, d, dsv, dsg;

	ra=sblkau_rangetab[ss&63];

	if(!ra)ra=1;
	d=val-pr;
	dsg=0;
	if(d<0)
		{ dsg=4; d=-d; }
//	dsv=(4*d+(ra/2))/ra;
//	dsv=(4*d-(ra/2))/ra;
//	dsv=(4*d)/ra;
	dsv=D_SoftDiv(4*d, ra);
//	dsv=(byte)dsv;
//	dsv=(4*d+(ra/4))/ra;
//	dsv=(4*d-(ra/4))/ra;
	if(dsv>3)dsv=3;
	dsv|=dsg;
	
//	tk_printf("%d/%d,%d ", d, ra, dsv);
	
	return(dsv);
}

int Sblkau_AdjustStepForSamp(int ss, int dsv)
{
	if((dsv&3)==0)ss--;
//	if((dsv&3)==3)ss++;
//	if((dsv&3)==2)ss++;
	if((dsv&3)==3)ss+=2;
//	if(ss<1)ss=1;
//	if(ss<0)ss=0;
	ss=__int_clamp(ss, 1, 63);

	return(ss);
}

int Sblkau_EncodeBlock(short *ibuf, u32 *obuf, u32 *rpred)
{
	int samp_c[128];
	int samp_s0[128];
	int samp_s[32];
	int samp_c4[32];
	u64 bit0, bit1, bit2, bit3, bit4, bit5;
	u32 bis0, bis1, bis2;
	u32 pred;
	u16 spc, sps;
	int cpr, spr;
	int ssc, sss;
	int pr, ss, spmono;
	int d, ra, dsg, dsv;
	int i, j, k;
	
	pred=*rpred;

	for(i=0; i<128; i++)
	{
		j=ibuf[i*2+0];
		k=ibuf[i*2+1];
		samp_c[i]=(j+k)>>1;
		samp_s0[i]=(j-k)>>1;
	}

	bit0=0;
	for(i=0; i<32; i++)
	{
		j=(	samp_s0[i*4+0] +
			samp_s0[i*4+1] +
			samp_s0[i*4+2] +
			samp_s0[i*4+3] )>>2;
		samp_s[i]=j;
		bit0+=j*j;
	}

	for(i=0; i<32; i++)
	{
		j=(	samp_c[i*4+0] +
			samp_c[i*4+1] +
			samp_c[i*4+2] +
			samp_c[i*4+3] )>>2;
		samp_c4[i]=j;
	}
	
	spmono=0;
//	if(pred&0x00010000)
//		spmono=1;
//	if(bit0<(1<<16))
//		spmono=1;
//	if(bit0>(1<<20))
//		spmono=0;

	if(spmono)
	{
		if(!(pred&0x00010000))
			pred=0;
		
		if(!pred)
		{
			spr=samp_s[0];
			cpr=((ibuf[0]+ibuf[1])>>1)-spr;

			ssc=Sblkau_StepForSamp(cpr);
			sss=Sblkau_StepForSamp(samp_s[1]-samp_s[0]);
			
			spc=(Sblkau_SampToPred9(cpr)<<7)|(ssc<<1);
			sps=(Sblkau_SampToPred9(spr)<<7)|(sss<<1);
			pred=(sps<<16)|spc;
		}
	
		for(i=0; i<32; i++)
		{
			j=(	samp_c[i*4+0] +
				samp_c[i*4+1] +
				samp_c[i*4+2] +
				samp_c[i*4+3] )>>2;
			samp_s[i]=j;
		}
		
		spmono=1;
		
		sblkau_stat[1]++;
	}else
	{
		if(pred&0x00010000)
			pred=0;

		if(!pred)
		{
			cpr=(ibuf[0]+ibuf[1])>>1;
			spr=(ibuf[0]-ibuf[1])>>1;

			ssc=Sblkau_StepForSamp(samp_c[1]-samp_c[0]);
			sss=Sblkau_StepForSamp(samp_s[1]-samp_s[0]);
			
			spc=(Sblkau_SampToPred9(cpr)<<7)|(ssc<<1);
			sps=(Sblkau_SampToPred9(spr)<<7)|(sss<<1);
			pred=(sps<<16)|spc;
		}
	
		sblkau_stat[0]++;
	}


	bis0=0;	bis1=0; bis2=0;
	pr=spr; ss=sss;
	for(i=0; i<32; i++)
	{
		dsv=Sblkau_SampForDelta(ss, pr, samp_s[i]);

		pr=pr+Sblkau_DeltaForSamp(ss, dsv);
		ss=Sblkau_AdjustStepForSamp(ss, dsv);

		samp_s[i]=pr;

		bis0=(bis1<<29)|(bis0>>3);
		bis1=(bis2<<29)|(bis1>>3);
		bis2=(dsv <<29)|(bis2>>3);
	}
	spr=pr; sss=ss;

	if(spmono)
	{
		for(i=0; i<128; i++)
		{
			k=samp_s[i>>2];
			j=(ibuf[i*2+0]+ibuf[i*2+1])>>1;
			samp_c[i]=(j-k);
		}
	}

	bit0=0;	bit1=0;
	bit2=0;	bit3=0;
	bit4=0;	bit5=0;
	
	pr=cpr; ss=ssc;
	for(i=0; i<128; i++)
	{
		dsv=Sblkau_SampForDelta(ss, pr, samp_c[i]);
		pr=pr+Sblkau_DeltaForSamp(ss, dsv);
		ss=Sblkau_AdjustStepForSamp(ss, dsv);

		bit0=      (bit1<<61)|(bit0>>3);
		bit1=      (bit2<<61)|(bit1>>3);
		bit2=      (bit3<<61)|(bit2>>3);
		bit3=      (bit4<<61)|(bit3>>3);
		bit4=      (bit5<<61)|(bit4>>3);
		bit5=(((u64)dsv)<<61)|(bit5>>3);
	}
	cpr=pr; ssc=ss;

	if(spmono)
		pred|=0x00010000;
	else
		pred&=~0x00010000;

	obuf[ 0]=pred;

	obuf[ 1]=bis0;
	obuf[ 2]=bis1;
	obuf[ 3]=bis2;

	obuf[ 4]=bit0    ;
	obuf[ 5]=bit0>>32;
	obuf[ 6]=bit1    ;
	obuf[ 7]=bit1>>32;
	obuf[ 8]=bit2    ;
	obuf[ 9]=bit2>>32;
	obuf[10]=bit3    ;
	obuf[11]=bit3>>32;
	obuf[12]=bit4    ;
	obuf[13]=bit4>>32;
	obuf[14]=bit5    ;
	obuf[15]=bit5>>32;

#if 0
	tk_printf("%08X %08X %08X %08X\n", obuf[ 0], obuf[ 1], obuf[ 2], obuf[ 3]);
	tk_printf("%08X %08X %08X %08X\n", obuf[ 4], obuf[ 5], obuf[ 6], obuf[ 7]);
	tk_printf("%08X %08X %08X %08X\n", obuf[ 8], obuf[ 9], obuf[10], obuf[11]);
	tk_printf("%08X %08X %08X %08X\n", obuf[12], obuf[13], obuf[14], obuf[15]);
	
//	__debugbreak();
#endif

	spc=(Sblkau_SampToPred9(cpr)<<7)|(ssc<<1);
	sps=(Sblkau_SampToPred9(spr)<<7)|(sss<<1);
	pred=(sps<<16)|spc;

	if(spmono)
		pred|=0x00010000;
	else
		pred&=~0x00010000;

	*rpred=pred;
}

qboolean SNDDMA_Init(void)
{
	u32 *sreg;

#if 0
	sreg=(u32 *)0x007F8000;
	sreg[1]=11025;
	sreg[2]=255;
	sreg[3]=255;
#endif

	shm = &sn;
	shm->splitbuffer = 0;

//	shm->speed = 11025;
	shm->speed = 16000;
	shm->samplebits = 16;
	shm->channels = BUFFER_CHAN;

	shm->soundalive = true;
	shm->samples = sizeof(dma_buffer) / (shm->samplebits/8);
	shm->samplepos = 0;
	shm->submission_chunk = 1;
	shm->buffer = (unsigned char *)dma_buffer;
	
	if(shm->samples != 8192)
		__debugbreak();
	
	return(1);
}

static int wbufrov, dmarov=-1;

int SNDDMA_GetDevDMAPos(void)
{
	int *sreg;
	int idma;

//	sreg=(int *)0x007F8000;
//	idma=sreg[0];
//	return(idma);
	return(wbufrov);
}

int SNDDMA_GetDMAPos(void)
{
	int i, j, k;
	
//	i = (int)(realtime*shm->speed*shm->channels);
//	j = i & (shm->samples-1);
//	shm->samplepos = j;
//	__debugbreak();
//	return j;

	shm->samplepos = (int)(realtime*shm->speed*shm->channels) & (shm->samples-1);
//	tk_printf("SNDDMA_GetDMAPos: %f %d\n", realtime, shm->samplepos);	
	return shm->samplepos;
}

int SNDDMA_GetSamples(void)
{
	return(0);
}

void SNDDMA_Shutdown(void)
{
}

static u32 *snd_dmabuf=NULL;
static byte snd_dmarov=0;
static u32 snd_dmapred=0;

static short snd_tsblk[256];

void SNDDMA_Submit(void)
{
	static int olddma;
//	short *sbufl, *sbufr;
	short *buf;
	int dma, idma, tdma;
	int b, n, d, b1, n1;
	int i, j, k, l;

	dma=SNDDMA_GetDMAPos();
	idma=SNDDMA_GetDevDMAPos();
	tdma=idma+500;

	if(dmarov<0)
	{
		olddma=dma;
		wbufrov=dma;
		dmarov=tdma;
	}

//	sbufl=(short *)0x007F0000;
//	sbufr=(short *)0x007F4000;
	buf=(short *)dma_buffer;
	
	snd_dmabuf=(u32 *)0xA0080000;

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

#if 0
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
//			sbufl[k]=buf[j*2+0];
//			sbufr[k]=buf[j*2+1];
		}else
		{
//			sbufl[k]=buf[j];
//			sbufr[k]=buf[j];
		}
	}
#endif

	b1=(b>>7); n1=(n+(b&127)+255)>>7;
	snd_dmapred=0;

	for(i=0; i<n1; i++)
	{
		snd_dmapred=0;

		j=(b1+i)&63;
		k=((b1+i)<<7)&8191;
		
		for(l=0; l<128; l++)
		{
			snd_tsblk[l*2+0]=buf[k+l];
			snd_tsblk[l*2+1]=buf[k+l];

//			snd_tsblk[(l*2+0)&255]=buf[k+l];
//			snd_tsblk[(l*2+1)&255]=buf[k+l];

//			snd_tsblk[l*2+0]=(((l>>3)^i)&1)?1024:(-1024);
//			snd_tsblk[l*2+1]=(((l>>3)^i)&1)?1024:(-1024);
		}
		
		Sblkau_EncodeBlock(snd_tsblk,
			snd_dmabuf+(j<<4), &snd_dmapred);

//		Sblkau_EncodeBlock(buf+(k<<1),
//			snd_dmabuf+(j<<4), &snd_dmapred);
	}


	olddma=dma;
	wbufrov+=n;
	dmarov+=n;
}
