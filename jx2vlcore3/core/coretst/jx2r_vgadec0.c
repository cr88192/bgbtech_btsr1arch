/*
Decode Composite, 100MHz PWM bitstream

Alternate: 100MHz 3x PCM4
	Signal stored as a series of 4-bit PCM samples, LSB first.

60Hz, 262.5 lines per frame (interlace).
Line Freq(1) = 15.750 kHz
Line Freq(2) = 15.734 kHz
Pixel Freq = 5.040 MHz

Pixel = ~19.84 bits (320).
Pixel = ~9.92 bits (640).
Pixel = ~9.92 bits (640).
Pixel = ~9.03 bits (704).

Pixel Time = 0.09us
Pixel Time(B) = 0.0822us

HSync Pulse
	Pulse Width = 4.6 - 4.8 us
	Back Porch  = 4.6 - 4.8 us (~ 52px / 59px)
	  Color Burst = 2.5 us (Center)
	Front Porch  = ~1.5 us
	Scanline Image 52.6us
	Scanline Total 63.5us

Vsync Pulse
	Line Width = 4.4 - 4.8 us
	Sync Pulse Width = 1/2 line (31.75us)

Equalizing Pulse
	Pulse Width = 2.2 - 2.4 us
	Sync Line Width = 1/2 line (31.75us)
	
VSync
	9 lines total.
	5 equalizing pulses.
	5 sync pulses.
	5/6 more equalizing pulses (6 for odd field).
*/

#if 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <time.h>

typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;

#include "bt1h_targa.c"

#endif


#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

byte pwmcnt4[16]={
	0, 1, 1, 2,	1, 2, 2, 3,
	1, 2, 2, 3,	2, 3, 3, 4};

byte pwmtab8[256];
byte pwmtab16[65536];

byte qamstab8pa[32][256];
byte qamstab8pb[32][256];
byte qamstab8pc[32][256];
byte qamstab8pd[32][256];

int clamp255(int clr)
{
	if(clr<0)return(0);
	if(clr>255)return(255);
	return(clr);
}

int pwmtab_init()
{
	static int init=0;
	float f, g, h;
	float f0, f1, f2, f3;
	float h0, h1, h2, h3;
	int p0, p1, p2, p3;
	int i, j, k, l;
	
	if(init)
		return(0);
	init=1;

	for(i=0; i<256; i++)
	{
		j=pwmcnt4[(i>>4)&15]+pwmcnt4[(i)&15];
		k=(j*255)/8;
		pwmtab8[i]=k;

#if 1
		for(k=0; k<32; k++)
		{
			f0=0; f1=0; f2=0; f3=0;
			for(j=0; j<8; j++)
			{
				l=((i>>j)&1)*2-1;

				h0=((j+k+ 0)/32.0+0.0)*(2*M_PI);
				h1=((j+k+ 8)/32.0+0.0)*(2*M_PI);
				h2=((j+k+16)/32.0+0.0)*(2*M_PI);
				h3=((j+k+24)/32.0+0.0)*(2*M_PI);
				f0+=sin(h0)*l*(1.0/16);
				f1+=sin(h1)*l*(1.0/16);
				f2+=sin(h2)*l*(1.0/16);
				f3+=sin(h3)*l*(1.0/16);
			}
			p0=clamp255(f0*127.0+128);
			p1=clamp255(f1*127.0+128);
			p2=clamp255(f2*127.0+128);
			p3=clamp255(f3*127.0+128);
			qamstab8pa[k][i]=p0;
			qamstab8pb[k][i]=p1;
			qamstab8pc[k][i]=p2;
			qamstab8pd[k][i]=p3;
		}
#endif
	}

	for(i=0; i<65536; i++)
	{
		j=	pwmcnt4[(i>>12)&15]+pwmcnt4[(i>>8)&15]+
			pwmcnt4[(i>> 4)&15]+pwmcnt4[(i   )&15];
		k=(j*255)/16;
		pwmtab16[i]=k;
	}
	
#if 0
	for(i=0; i<65536; i++)
	{
#if 1
		for(k=0; k<32; k++)
		{
			f=0; g=0;
			for(j=0; j<16; j++)
			{
				h=((j+k+0)/32.0+0.0)*(2*M_PI);
				l=((i>>j)&1)*2-1;
				f+=sin(h)*l*(1.0/16);
//				g+=cos(h)*l*(1.0/16);
			}
			p0=clamp255(f*127.0+128);
//			p1=clamp255(g*127.0+128);

			f=0; g=0;
			for(j=0; j<16; j++)
			{
				h=((j+k+16)/32.0)*(2*M_PI);
				l=((i>>j)&1)*2-1;
				f+=sin(h)*l*(1.0/16);
//				g+=cos(h)*l*(1.0/16);
			}
			p2=clamp255(f*127.0+128);
//			p3=clamp255(g*127.0+128);

			qamstab16pa[k][i]=p0;
//			qamctab16pa[k][i]=p1;
			qamstab16pb[k][i]=p2;
//			qamctab16pb[k][i]=p3;
		}
#endif
	}
#endif
	
	return(1);
}

// #define CDEC_LINE_RAWXMAX 820
#define CDEC_LINE_RAWXMAX 1440
//#define CDEC_LINE_RAWYMAX 530
#define CDEC_LINE_RAWYMAX 720

#define CDEC_LINE_XOFS 58
//#define CDEC_LINE_XOFS 60
#define CDEC_LINE_YOFS 20

typedef struct cdec_imgbuf_s cdec_imgbuf;

struct cdec_imgbuf_s
{
u16 *bits;
int bpos;
int bpsz;
int epos;

int lcy, lcu, lcv;

byte ybuf[CDEC_LINE_RAWXMAX*CDEC_LINE_RAWYMAX];
byte ubuf[CDEC_LINE_RAWXMAX*CDEC_LINE_RAWYMAX];
byte vbuf[CDEC_LINE_RAWXMAX*CDEC_LINE_RAWYMAX];
u16 xpos;
u16 ypos;
byte neqp;
sbyte qaofs;	//QAM phase offset
byte bestqi;	//best QAM intensity
int frame;

byte bsmode;	//bitstream mode
};

int cdec_decode0y(cdec_imgbuf *ctx);

int CDEC_SetupForStream(cdec_imgbuf *ctx)
{
	ctx->bits=(u16 *)malloc(4096*2);
	ctx->bpos=0;
	ctx->epos=0;
}

int CDEC_UpdateForStreamCycle(cdec_imgbuf *ctx, int bits)
{
	if(ctx->epos>=4088)
	{
		while(ctx->bpos<ctx->epos)
			{ cdec_decode0y(ctx); }
		ctx->bpos=0;
		ctx->epos=0;
	}

	ctx->bits[ctx->epos++]=bits;
	while((ctx->bpos+4)<=ctx->epos)
		{ cdec_decode0y(ctx); }
	return(0);
}

int CDEC_PeekPwm8I(cdec_imgbuf *ctx, int ofs, int shl)
{
	u32 p0, p1, p2, p3;
	int i, j;

//	if(ctx->bsmode==3)
	if(1)
	{
//		p0=ctx->bits[((ctx->bpos+ofs)>>1)+0];
//		p1=ctx->bits[((ctx->bpos+ofs)>>1)+1];
//		p2=ctx->bits[((ctx->bpos+ofs)>>1)+2];
//		p3=ctx->bits[((ctx->bpos+ofs)>>1)+3];

		p0=ctx->bits[(ctx->bpos+ofs)+0];
		p1=ctx->bits[(ctx->bpos+ofs)+1];
		p2=ctx->bits[(ctx->bpos+ofs)+2];
		p3=ctx->bits[(ctx->bpos+ofs)+3];

		i=	((p0>>shl)&15)+((p1>>shl)&15)+
			((p2>>shl)&15)+((p3>>shl)&15);
		i=i<<2;
		return(i);
	}
	
	return(0);
}

int CDEC_PeekPwm8Y(cdec_imgbuf *ctx, int ofs)
{
	int cr, cg, cb;
	int cy, cu, cv;
	
	cr=CDEC_PeekPwm8I(ctx, ofs, 8);
	cg=CDEC_PeekPwm8I(ctx, ofs, 4);
	cb=CDEC_PeekPwm8I(ctx, ofs, 0);
	
	cy=(2*cg+cr+cb)/4;
	return(cy);
}

int CDEC_PeekPwm8U(cdec_imgbuf *ctx, int ofs)
{
	int cr, cg, cb;
	int cy, cu, cv;
	
	cr=CDEC_PeekPwm8I(ctx, ofs, 8);
	cg=CDEC_PeekPwm8I(ctx, ofs, 4);
	cb=CDEC_PeekPwm8I(ctx, ofs, 0);
	
	cy=((cb-cg)/2)+128;
	return(cy);
}

int CDEC_PeekPwm8V(cdec_imgbuf *ctx, int ofs)
{
	int cr, cg, cb;
	int cy, cu, cv;
	
	cr=CDEC_PeekPwm8I(ctx, ofs, 8);
	cg=CDEC_PeekPwm8I(ctx, ofs, 4);
	cb=CDEC_PeekPwm8I(ctx, ofs, 0);
	
	cy=((cr-cg)/2)+128;
	return(cy);
}

int CDEC_PeekHSync(cdec_imgbuf *ctx)
{
	u32 p0, p1, p2, p3;
	int i, j;

//	if(ctx->bsmode==3)
	if(1)
	{
//		p0=ctx->bits[ctx->bpos>>1];
		p0=ctx->bits[ctx->bpos];
		return(!(p0&0x1000));
	}
	
	return(0);
}

int CDEC_PeekVSync(cdec_imgbuf *ctx)
{
	u32 p0, p1, p2, p3;
	int i, j;

//	if(ctx->bsmode==3)
	if(1)
	{
//		p0=ctx->bits[ctx->bpos>>1];
		p0=ctx->bits[ctx->bpos];
		return(!(p0&0x2000));
	}
	
	return(0);
}

int cdec_decode0y(cdec_imgbuf *ctx)
{
	int p0, p1, p2, p3;
	int p4, p5, p6, p7;
	int py, pu, pv;
	int cy, cu, cv;
	int lcy, lcu, lcv;
	int bp, bp1, bp2;
	int hsyn, vsyn;
	int i, j, k;

	py=CDEC_PeekPwm8Y(ctx, 0);
	pu=CDEC_PeekPwm8U(ctx, 0);
	pv=CDEC_PeekPwm8V(ctx, 0);
	hsyn=CDEC_PeekHSync(ctx);
	vsyn=CDEC_PeekVSync(ctx);

//	vsyn=CDEC_PeekHSync(ctx);
//	hsyn=CDEC_PeekVSync(ctx);

//	if(py<50)
//	if(py<5)
//	if(py<30)
	if(hsyn||vsyn)
	{
		/* verical resync pulse */
//		if(ctx->xpos &&
//			(ctx->xpos>50) &&
//			(ctx->xpos<64))
		if(vsyn)
		{
			if(ctx->ypos>200)
			{
				printf("Frame %d %d\n", ctx->xpos, ctx->ypos);
				ctx->frame++;
			}

			ctx->ypos=0;
			ctx->xpos=0;
//			ctx->bpos+=8;
//			ctx->bpos+=4;
			ctx->bpos++;
			ctx->neqp=0;
			return(0);
		}

		if(hsyn)
		{
			if(ctx->xpos>600)
				ctx->ypos++;
			ctx->xpos=0;
//			ctx->bpos+=8;
//			ctx->bpos+=4;
			ctx->bpos++;
			return(0);
		}

#if 0
		/* equalization pulse */
		if(ctx->xpos &&
			(ctx->xpos>300) &&
			(ctx->xpos<420))
		{
			ctx->neqp++;
			ctx->xpos=0;
			ctx->bpos+=8;
			return(0);
		}
		
		if(ctx->xpos>600)
		{
			ctx->ypos+=2;
			ctx->ypos&=~1;
			ctx->ypos|=!(ctx->neqp&1);
//			ctx->ypos|=!(ctx->frame&1);
			
//			ctx->qaofs=14;
			ctx->qaofs=0;
//			ctx->qaofs=7;
//			ctx->qaofs=21;

			ctx->bestqi=0;

			if(ctx->ypos>=530)
			{
				ctx->ypos=!(ctx->ypos&1);
			}
		}
#endif

		ctx->xpos=0;
		ctx->ypos=0;
//		ctx->bpos+=8;
		ctx->bpos+=4;
		return(0);
	}

	if(ctx->bsmode==3)
	{
//		pu=128;
//		pv=128;
	}

//	pu=128;
//	pv=128;

//	cy=((py-76)*365)>>8;
	cy=((py-76)*440)>>8;
//	if(cy<0)cy=0;
//	if(cy>255)cy=255;

	cy=clamp255(cy);

//	cy=(3*ctx->lcy+cy)/4;
//	cy=(ctx->lcy+cy)/2;

	ctx->lcy=cy;
	
	cu=pu;
	cv=pv;

	cu=(((pu-128)*440)>>8)+128;
	cv=(((pv-128)*440)>>8)+128;

	cu=clamp255(cu);
	cv=clamp255(cv);

//	cu=128;
//	cv=128;

//	cu=(ctx->lcu+pu)/2;
//	cv=(ctx->lcv+pv)/2;

	ctx->lcu=cu;
	ctx->lcv=cv;
	
//	ctx->bpos+=8;
	ctx->bpos+=4;

#if 0
	if(ctx->frame>1)
	{
		lcy=ctx->ybuf[ctx->ypos*CDEC_LINE_RAWXMAX+ctx->xpos];
		lcu=ctx->ubuf[ctx->ypos*CDEC_LINE_RAWXMAX+ctx->xpos];
		lcv=ctx->vbuf[ctx->ypos*CDEC_LINE_RAWXMAX+ctx->xpos];

//		cy=(lcy+cy)/2;
//		cu=(lcu+cu)/2;
//		cv=(lcv+cv)/2;

		cy=(lcy+3*cy)/4;
		cu=(lcu+3*cu)/4;
		cv=(lcv+3*cv)/4;
	}
#endif

	ctx->ybuf[ctx->ypos*CDEC_LINE_RAWXMAX+ctx->xpos]=cy;
	ctx->ubuf[ctx->ypos*CDEC_LINE_RAWXMAX+ctx->xpos]=cu;
	ctx->vbuf[ctx->ypos*CDEC_LINE_RAWXMAX+ctx->xpos]=cv;

	ctx->ybuf[ctx->ypos*CDEC_LINE_RAWXMAX+ctx->xpos+1]=255;
	ctx->ybuf[ctx->ypos*CDEC_LINE_RAWXMAX+ctx->xpos+2]=255;
	ctx->ybuf[ctx->ypos*CDEC_LINE_RAWXMAX+ctx->xpos+3]=255;
	ctx->ybuf[ctx->ypos*CDEC_LINE_RAWXMAX+ctx->xpos+4]=255;

//	ctx->ybuf[ctx->ypos*CDEC_LINE_RAWXMAX+ctx->xpos+1]=0;
//	ctx->ubuf[ctx->ypos*CDEC_LINE_RAWXMAX+ctx->xpos+1]=255;
//	ctx->vbuf[ctx->ypos*CDEC_LINE_RAWXMAX+ctx->xpos+1]=255;

	ctx->xpos++;
	if(ctx->xpos>=CDEC_LINE_RAWXMAX)
	{
		printf("CDEC_LINE_RAWXMAX\n");
		ctx->xpos=0;
		ctx->ypos++;
	}
	
	if(ctx->ypos>=CDEC_LINE_RAWYMAX)
	{
		printf("CDEC_LINE_RAWYMAX\n");
//		ctx->ypos=!(ctx->ypos&1);
		ctx->ypos=0;
	}

	return(0);
}

int cdec_getimage(cdec_imgbuf *ctx, byte *obuf, int xs, int ys)
{
	int cy, cu, cv, cu1, cv1;
	int cr, cg, cb, vf;
	int i, j, k;
	
//	for(i=0; i<480; i++)
//		for(j=0; j<640; j++)

	for(i=0; i<ys; i++)
		for(j=0; j<xs; j++)
	{
//		k=(i+2*CDEC_LINE_YOFS)*CDEC_LINE_RAWXMAX+(j+CDEC_LINE_XOFS);
//		k=(479-(i&(~1)))+2*CDEC_LINE_YOFS;
//		k=(479-(i|1))+2*CDEC_LINE_YOFS;
//		k=(479-i)+2*CDEC_LINE_YOFS;
//		k=(ys-i-1)+2*CDEC_LINE_YOFS;
		k=i+2*CDEC_LINE_YOFS;
		k=(k*CDEC_LINE_RAWXMAX)+(j+CDEC_LINE_XOFS);
		cy=ctx->ybuf[k];
		cu=ctx->ubuf[k];
		cv=ctx->vbuf[k];
		
//		cu=128;
//		cv=128;
		
//		cr=cy;
//		cg=cy;
//		cb=cy;

		cu1=cu-128;
		cv1=cv-128;

		cg=(2*cy-cu1-cv1)/2;
//		cg=cy-(cu+cv-256)/2;
		cr=cg+2*(cv-128);
		cb=cg+2*(cu-128);

//		cu1=cu-128;
//		cv1=cv-128;
//		cr = cy + 0.9469 * cv1 + 0.6236 * cu1;
//		cg = cy - 0.2748 * cv1 - 0.6357 * cu1;
//		cb = cy - 1.1 * cv1 + 1.7 * cu1;
		
		cr=clamp255(cr);
		cg=clamp255(cg);
		cb=clamp255(cb);
		
//		obuf[(i*640+j)*4+0]=cr;
//		obuf[(i*640+j)*4+1]=cg;
//		obuf[(i*640+j)*4+2]=cb;
//		obuf[(i*640+j)*4+3]=255;

		obuf[(i*xs+j)*4+0]=cr;
		obuf[(i*xs+j)*4+1]=cg;
		obuf[(i*xs+j)*4+2]=cb;
		obuf[(i*xs+j)*4+3]=255;
	}
	return(0);
}
