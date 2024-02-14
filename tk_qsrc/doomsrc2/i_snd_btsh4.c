#include "doomdef.h"
#include "doomtype.h"

#define BUFFER_CHAN		2
// #define BUFFER_CHAN		1
//#define BUFFER_SIZE		(BUFFER_CHAN*8192*2)
#define BUFFER_SIZE		(BUFFER_CHAN*8192)

extern int			soundtime;		// sample PAIRS
extern int   		paintedtime; 	// sample PAIRS
extern int			endtime;		// sample PAIRS

//unsigned char dma_buffer[BUFFER_SIZE];
short dma_buffer[BUFFER_SIZE];
// unsigned char pend_buffer[BUFFER_SIZE];

int dma_pos;

void SNDDMA_Submit(void);
void	SoundDev_WriteStereoSamples2(short *mixbuf, int nsamp, int nsamp2);

void	SoundDev_WriteStereoSamples(short *mixbuf, int nsamp)
{
	SoundDev_WriteStereoSamples2(mixbuf, nsamp, nsamp*2);
}

void	SoundDev_WriteStereoSamples2(short *mixbuf, int nsamp, int nsamp2)
{
	short *dmabuf;
	int b;
	int i;
	
//	SNDDMA_GetDMAPos();

//	printf("ns=%d\n", nsamp);
	
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

void	SoundDev_Submit()
{
	SNDDMA_Submit();
}

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
//	__debugbreak();
	return(v);
}

int Sblkau_SampForDelta(int ss, int pr, int val)
{
	int ra, d, dsv, dsg;

	ra=sblkau_rangetab[ss&63];

	if(!ra)
		__debugbreak();

	if(!ra)ra=1;
	d=val-pr;
	dsg=0;
	if(d<0)
		{ dsg=4; d=-d; }
//	dsv=(4*d+(ra/2))/ra;
//	dsv=(4*d-(ra/2))/ra;
	dsv=(4*d)/ra;
//	dsv=M_SoftDivS(4*d, ra);
//	dsv=(byte)dsv;
	if(dsv!=((u16)dsv))
		__debugbreak();
//	dsv=(4*d+(ra/4))/ra;
//	dsv=(4*d-(ra/4))/ra;
//	if(dsv>3)dsv=3;
	dsv=__int_clamp(dsv, 0, 3);
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
	ss=__int_clamp(ss, 1, 62);

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

		if(((s16)pr)!=pr)
			__debugbreak();

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

		if(((s16)pr)!=pr)
			__debugbreak();

		if(dsv&(~7))
			__debugbreak();

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

	spc=(Sblkau_SampToPred9(cpr)<<7)|(ssc<<1);
	sps=(Sblkau_SampToPred9(spr)<<7)|(sss<<1);
	pred=(sps<<16)|spc;

	if(spmono)
		pred|=0x00010000;
	else
		pred&=~0x00010000;

	*rpred=pred;
}

static int wbufrov, dmarov=-1;

int SNDDMA_GetDevDMAPos(void)
{
	int *sreg;
	int idma;
	return(wbufrov);
}

int SNDDMA_GetDMAPos(void)
{
	u32 ms;
	
//	ms=TK_GetTimeMs();
//	dma_pos=(ms*16)&8191;
	
//	shm->samplepos = (int)(realtime*shm->speed*shm->channels) & (shm->samples-1);
//	return shm->samplepos;
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

static volatile u32 *snd_dmabuf=NULL;
static byte snd_dmarov=0;
static u32 snd_dmapred=0;

static short snd_tsblk[256];

int TK_GetApproxMHz(void);

void SNDDMA_Submit(void)
{
	static int olddma;
//	short *sbufl, *sbufr;
	short *buf;
	int dma, idma, tdma;
	int lv, rv;
	int s0, s1, s2, s3;
	int b, n, d, b1, n1, d1;
	int i, j, k, l;
	
//	l=TK_GetApproxMHz();
//	if(l<75)
//		return;

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
	
#ifdef __ADDR_X48__
	snd_dmabuf=(u32 *)0xFFFFF0090000ULL;
#else
//	snd_dmabuf=(u32 *)0xA0080000;
//	snd_dmabuf=(u32 *)0xF0080000;
	snd_dmabuf=(u32 *)0xF0090000UL;
#endif

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

#if 1
	b1=b&(~3); n1=(n+3)&(~3);
	d1=dmarov&(~3);

#ifdef __ADDR_X48__
	((u32 *)0xFFFFF009F000ULL)[0]=0x0029;
#else
//	((u32 *)0xF009F000)[0]=0x002B;
//	((u32 *)0xF009F000)[0]=0x002A;
//	((u32 *)0xF009F000)[0]=0x0028;
	((u32 *)0xF009F000UL)[0]=0x0029;
#endif
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

#if 0
	b1=(b>>7); n1=(n+(b&127)+255)>>7;
	snd_dmapred=0;

	for(i=0; i<n1; i++)
	{
		snd_dmapred=0;

		j=(b1+i)&63;
		k=((b1+i)<<7)&8191;
		
		for(l=0; l<128; l++)
		{
//			snd_tsblk[l*2+0]=buf[k+l];
//			snd_tsblk[l*2+1]=buf[k+l];

			lv=buf[(k+l)*2+0];
			rv=buf[(k+l)*2+1];
			
			if(lv<-4096)lv=0;
			if(lv> 4096)lv=0;

			if(rv<-4096)rv=0;
			if(rv> 4096)rv=0;

//			lv=(l&4)?2048:-2048;
//			rv=(l&4)?2048:-2048;

			snd_tsblk[l*2+0]=lv;
			snd_tsblk[l*2+1]=rv;

//			snd_tsblk[l*2+0]=(l&4)?2048:-2048;
//			snd_tsblk[l*2+1]=(l&4)?2048:-2048;
		}
		
		Sblkau_EncodeBlock(snd_tsblk,
			snd_dmabuf+(j<<4), &snd_dmapred);
	}
#endif

	olddma=dma;
	wbufrov+=n;
	dmarov+=n;
}
