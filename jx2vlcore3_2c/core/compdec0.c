/*
Decode Composite, 100MHz PWM bitstream

Alternate: 100MHz PCM4
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

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

byte pwmcnt4[16]={
	0, 1, 1, 2,	1, 2, 2, 3,
	1, 2, 2, 3,	2, 3, 3, 4};

byte pwmtab8[256];
byte pwmtab16[65536];

//byte qamstab16[65536];
//byte qamctab16[65536];
//byte qamstab16b[65536];
//byte qamctab16b[65536];

//byte qamstab16pa[32][65536];
// byte qamctab16pa[32][65536];
//byte qamstab16pb[32][65536];
// byte qamctab16pb[32][65536];

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
#if 0
		f=0; g=0;
		for(j=0; j<16; j++)
		{
			k=((i>>j)&1)*2-1;
//			l=(j&1)?((j&14)+17):(j&14);
			l=j;
			
//			h=(j/512.0)*(73.0*(2*M_PI));
			h=(l/100.0)*(3.579545454*(2*M_PI));
			f+=sin(h)*k*(1.0/16);
			g+=cos(h)*k*(1.0/16);

//			f+=sin(j*(2*M_PI)*(1.0/7.0))*k;
//			g+=cos(j*(2*M_PI)*(1.0/7.0))*k;
		}
		k=f*127+128;
		l=g*127+128;

//		k=f*(127.0/4)+128;
//		l=g*(127.0/4)+128;

		k=clamp255(k);
		l=clamp255(l);
		qamstab16[i]=k;
		qamctab16[i]=l;
#endif

#if 0
		f=0; g=0;
		for(j=0; j<16; j++)
		{
			k=((i>>j)&1)*2-1;
			l=j+16;
			h=(l/100.0)*(3.579545454*(2*M_PI));
			f+=sin(h)*k*(1.0/16);
			g+=cos(h)*k*(1.0/16);
		}
		k=f*127.0+128;
		l=g*127.0+128;
//		k=f*(127.0/4)+128;
//		l=g*(127.0/4)+128;
		k=clamp255(k);
		l=clamp255(l);
		qamstab16b[i]=k;
		qamctab16b[i]=l;
#endif

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

#define CDEC_LINE_RAWXMAX 820
#define CDEC_LINE_RAWYMAX 530

#define CDEC_LINE_XOFS 58
//#define CDEC_LINE_XOFS 60
#define CDEC_LINE_YOFS 20

typedef struct cdec_imgbuf_s cdec_imgbuf;

struct cdec_imgbuf_s
{
u32 *bits;
int bpos;
int bpsz;

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


int cdec_readbits(u32 *bits, int pos, int nb)
{
	u64 w;
	int i, j, k;
	
	i=pos>>5;
	j=pos&31;
	
	w=bits[i]|(((u64)(bits[i+1]))<<32);
	k=(w>>j)&((1<<nb)-1);
	return(k);
}

int CDEC_PeekPwm8(cdec_imgbuf *ctx, int ofs)
{
	u32 p0;
	int i, j;

	if((ctx->bsmode==0) ||
		(ctx->bsmode==1))
	{
		i=cdec_readbits(ctx->bits, ctx->bpos+ofs, 8);
		j=pwmtab8[i];
		return(j);
	}
	
	if(ctx->bsmode==2)
	{
		p0=ctx->bits[(ctx->bpos+ofs)>>3];
		i=	((p0    )&15)+((p0>> 4)&15)+
			((p0>> 8)&15)+((p0>>12)&15)+
			((p0>>16)&15)+((p0>>20)&15)+
			((p0>>24)&15)+((p0>>28)&15);
		i=i<<1;
		return(i);
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
	int i, j, k;

//	i=cdec_readbits(ctx->bits, ctx->bpos, 16);
//	py=pwmtab16[i];

//	i=cdec_readbits(ctx->bits, ctx->bpos, 8);
//	py=pwmtab8[i];
	py=CDEC_PeekPwm8(ctx, 0);

//	pu=128;
//	pv=128;
	
//	if(py<50)
//	if(py<5)
	if(py<30)
	{
		/* verical resync pulse */
		if(ctx->xpos &&
			(ctx->xpos>50) &&
			(ctx->xpos<64))
		{
			if(ctx->ypos>200)
				ctx->frame++;

			ctx->ypos=0;
			ctx->xpos=0;
			ctx->bpos+=8;
			ctx->neqp=0;
			return(0);
		}

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

		ctx->xpos=0;
		ctx->bpos+=8;
		return(0);
	}

#if 1
	if((ctx->bsmode==0) ||
		(ctx->bsmode==1))
	{
		bp=(ctx->bpos*150137)>>17;
		bp=bp+ctx->qaofs;
		p0=cdec_readbits(ctx->bits, ctx->bpos+ 0, 16);
		p1=cdec_readbits(ctx->bits, ctx->bpos+16, 16);
		p4=(p0  )&255;	p5=(p0>>8)&255;
		p6=(p1  )&255;	p7=(p1>>8)&255;

		bp1=(bp+0+3)&31;
	//	bp2=(bp+8+3)&31;
	//	bp2=(bp-8+3)&31;

		bp1=(bp+0+0)&31;
		bp2=(bp+8+0)&31;

		py=(pwmtab16[p0]+pwmtab16[p1])/2;
	//	py=(pwmtab8[p4]+pwmtab8[p5]*3+pwmtab8[p6]*3+pwmtab8[p7])/8;

		pv=(qamstab8pa[bp1][p4]+qamstab8pb[bp1][p5]+
			qamstab8pc[bp1][p6]+qamstab8pd[bp1][p7])-384;
		pu=(qamstab8pa[bp2][p4]+qamstab8pb[bp2][p5]+
			qamstab8pc[bp2][p6]+qamstab8pd[bp2][p7])-384;
	}

	if(ctx->bsmode==2)
	{
//		bp=(ctx->bpos*150137)>>17;
		bp=(ctx->bpos*150137)>>16;
		bp=bp+ctx->qaofs;
		
		p0=CDEC_PeekPwm8(ctx,  0);
		p1=CDEC_PeekPwm8(ctx,  8);
		p2=CDEC_PeekPwm8(ctx, 16);
		p3=CDEC_PeekPwm8(ctx, 24);

//		py=(p0+p1+p2+p3)/4;
		py=(p0+3*p1+3*p2+p3)/8;

#if 1
//		j=bp&31;
		j=63-(bp&63);
		i=j&15;
		switch((j>>4)&3)
		{
		case 0:
			p4=((p0*(15-i))+(p1*i))/16;
			p5=((p1*(15-i))+(p2*i))/16;
			p6=((p2*(15-i))+(p3*i))/16;
			p7=((p3*(15-i))+(p0*i))/16;
			break;
		case 1:
			p4=((p1*(15-i))+(p2*i))/16;
			p5=((p2*(15-i))+(p3*i))/16;
			p6=((p3*(15-i))+(p0*i))/16;
			p7=((p0*(15-i))+(p1*i))/16;
			break;
		case 2:
			p4=((p2*(15-i))+(p3*i))/16;
			p5=((p3*(15-i))+(p0*i))/16;
			p6=((p0*(15-i))+(p1*i))/16;
			p7=((p1*(15-i))+(p2*i))/16;
			break;
		case 3:
			p4=((p3*(15-i))+(p0*i))/16;
			p5=((p0*(15-i))+(p1*i))/16;
			p6=((p1*(15-i))+(p2*i))/16;
			p7=((p2*(15-i))+(p3*i))/16;
			break;
		}
#endif

//		pv=((p4+p5)-(p6+p7))/2+128;
//		pu=((p5+p6)-(p7+p0))/2+128;
//		pu=((p7+p0)-(p5+p6))/2+128;

		pv=((p4+p5)-(p6+p7))+128;
		pu=((p7+p0)-(p5+p6))+128;
//		pu=255-pu;
		
		pu=clamp255(pu);
		pv=clamp255(pv);
	}
#endif

	if((ctx->xpos>15) && (ctx->xpos<40))
	{
#if 1
		if((ctx->bsmode==0) ||
			(ctx->bsmode==1))
		{
			j=bp+1; k=bp-1;
	//		p2=(qamstab16pa[j&31][p0]+qamstab16pb[j&31][p1])-128;
	//		p3=(qamstab16pa[k&31][p0]+qamstab16pb[k&31][p1])-128;

			p2=(qamstab8pa[j&31][p4]+qamstab8pb[j&31][p5]+
				qamstab8pc[j&31][p6]+qamstab8pd[j&31][p7])-384;
			p3=(qamstab8pa[k&31][p4]+qamstab8pb[k&31][p5]+
				qamstab8pc[k&31][p6]+qamstab8pd[k&31][p7])-384;
			
			if(p2>pv)
				ctx->qaofs=(ctx->qaofs+1)&31;
			if(p3>pv)
				ctx->qaofs=(ctx->qaofs-1)&31;
			
			if((pv>136) && (pv>ctx->bestqi))
				ctx->bestqi=pv;
		}
#endif

		if(ctx->bsmode==2)
		{
//			pv=((p4+p5)-(p6+p7))+128;
//			pu=((p7+p0)-(p5+p6))+128;

			cv=(p4-p6)+128;
			cu=(p5-p7)+128;
			if(cu>cv)
				ctx->qaofs=(ctx->qaofs-1)&63;
			if(cv>cu)
				ctx->qaofs=(ctx->qaofs+1)&63;
		}
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
	
//	cu=pu;
//	cv=pv;

	if(ctx->bestqi>128)
	{
		j=255-ctx->bestqi;
//		k=256+j;
		k=128+j;

		cu=(((pu-128)*k)>>7)+128;
		cv=(((pv-128)*k)>>7)+128;

//		cu=(pu-128)*1.4+128;
//		cv=(pv-128)*1.4+128;
		cu=clamp255(cu);
		cv=clamp255(cv);
	}else
	{
		cu=128;
		cv=128;
	}

//	cu=(ctx->lcu+cu)/2;
//	cv=(ctx->lcv+cv)/2;

	cu=(ctx->lcu+pu)/2;
	cv=(ctx->lcv+pv)/2;

//	cu=(3*ctx->lcu+pu)/4;
//	cv=(3*ctx->lcv+pv)/4;
//	cu=(7*ctx->lcu+pu)/8;
//	cv=(7*ctx->lcv+pv)/8;
//	cu=(15.0*ctx->lcu+pu)/16.0;
//	cv=(15.0*ctx->lcv+pv)/16.0;
	ctx->lcu=cu;
	ctx->lcv=cv;
	
	ctx->bpos+=8;

	if(ctx->frame>1)
//	if(0)
//	if(1)
	{
		lcy=ctx->ybuf[ctx->ypos*CDEC_LINE_RAWXMAX+ctx->xpos];
		lcu=ctx->ubuf[ctx->ypos*CDEC_LINE_RAWXMAX+ctx->xpos];
		lcv=ctx->vbuf[ctx->ypos*CDEC_LINE_RAWXMAX+ctx->xpos];

		cy=(lcy+cy)/2;
		cu=(lcu+cu)/2;
		cv=(lcv+cv)/2;
		
//		cy=(3*lcy+cy)/4;
//		cu=(3*lcu+cu)/4;
//		cv=(3*lcv+cv)/4;

//		cy=(7*lcy+cy)/8;
//		cu=(7*lcu+cu)/8;
//		cv=(7*lcv+cv)/8;
	}

	ctx->ybuf[ctx->ypos*CDEC_LINE_RAWXMAX+ctx->xpos]=cy;
	ctx->ubuf[ctx->ypos*CDEC_LINE_RAWXMAX+ctx->xpos]=cu;
	ctx->vbuf[ctx->ypos*CDEC_LINE_RAWXMAX+ctx->xpos]=cv;
	ctx->xpos++;
	if(ctx->xpos>=CDEC_LINE_RAWXMAX)
	{
		printf("CDEC_LINE_RAWXMAX\n");
		ctx->xpos=0;
		ctx->ypos+=2;
	}
	
	if(ctx->ypos>=CDEC_LINE_RAWYMAX)
	{
		printf("CDEC_LINE_RAWYMAX\n");
		ctx->ypos=!(ctx->ypos&1);
	}

	return(0);
}

int cdec_getimage(cdec_imgbuf *ctx, byte *obuf)
{
	int cy, cu, cv, cu1, cv1;
	int cr, cg, cb;
	int i, j, k;
	
	for(i=0; i<480; i++)
		for(j=0; j<640; j++)
	{
//		k=(i+2*CDEC_LINE_YOFS)*CDEC_LINE_RAWXMAX+(j+CDEC_LINE_XOFS);
//		k=(479-(i&(~1)))+2*CDEC_LINE_YOFS;
//		k=(479-(i|1))+2*CDEC_LINE_YOFS;
		k=(479-i)+2*CDEC_LINE_YOFS;
		k=(k*CDEC_LINE_RAWXMAX)+(j+CDEC_LINE_XOFS);
		cy=ctx->ybuf[k];
		cu=ctx->ubuf[k];
		cv=ctx->vbuf[k];
		
//		cu=128;
//		cv=128;
		
//		cr=cy;
//		cg=cy;
//		cb=cy;

//		cg=cy-(cu+cv-256)/2;
//		cr=cg+2*(cv-128);
//		cb=cg+2*(cu-128);

		cu1=cu-128;
		cv1=cv-128;
		cr = cy + 0.9469 * cv1 + 0.6236 * cu1;
		cg = cy - 0.2748 * cv1 - 0.6357 * cu1;
		cb = cy - 1.1 * cv1 + 1.7 * cu1;
		
		cr=clamp255(cr);
		cg=clamp255(cg);
		cb=clamp255(cb);
		
		obuf[(i*640+j)*4+0]=cr;
		obuf[(i*640+j)*4+1]=cg;
		obuf[(i*640+j)*4+2]=cb;
		obuf[(i*640+j)*4+3]=255;
	}
	return(0);
}

byte *loadfile(char *path, int *rsz)
{
	byte *buf;
	FILE *fd;
	int sz, i;
	
	fd=fopen(path, "rb");
	if(!fd)
		return(NULL);

	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	buf=malloc(sz);
	i=fread(buf, 1, sz, fd);
	fclose(fd);
	
	*rsz=sz;
	return(buf);
}

int storefile(char *name, byte *ibuf, int isz)
{
	byte *tbuf;
	FILE *fd;
	int sz;

	fd=fopen(name, "wb");
	if(!fd)
	{
		printf("Fail Open Write %s\n", name);
		return(-1);
	}

	fwrite(ibuf, 1, isz, fd);
	fclose(fd);
	return(0);
}

int main(int argc, char *argv[])
{
	cdec_imgbuf *ctx;
	byte *ibuf, *dbuf;
	byte *obuf;
	char *ifn, *ofn;
	double f, g;
	int t0, t1, t2, t3;
	int isz, n;
	int i, j, k;
	
	t0=clock();
	pwmtab_init();
	t1=clock();
	
	t2=t1-t0;
	printf("Init %d\n", t2);
	
	ifn=NULL;
	ofn=NULL;

	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			continue;
		}
		if(!ifn)
			{ ifn=argv[i]; continue; }
		if(!ofn)
			{ ofn=argv[i]; continue; }
	}

//	if(!ifn || !ofn)
	if(!ifn)
	{
		printf("No Input File %s\n", ifn);
		return(-1);
	}

	ibuf=loadfile(ifn, &isz);
	
	if(!ibuf)
	{
		printf("Failed Load %s\n", ifn);
		return(0);
	}
	
	ctx=malloc(sizeof(cdec_imgbuf));
	memset(ctx, 0, sizeof(cdec_imgbuf));
	
	ctx->bits=(u32 *)ibuf;
//	ctx->bpsz=isz/4;
	ctx->bpsz=isz*32;
	
	if(ctx->bits[0]==0x55AA4100)
	{
		ctx->bits=((u32 *)ibuf)+1;
		ctx->bpsz=((isz-4)/4)*8;
		ctx->bsmode=2;
	}

	t0=clock();
	
//	while((ctx->bpos>>5)<ctx->bpsz)
	while(ctx->bpos<ctx->bpsz)
	{
		cdec_decode0y(ctx);
	}

	t1=clock();
	
	t2=t1-t0;
	printf("Frame %d\n", t2);

#if 0
	t0=clock(); t1=t0; n=0;
	t3=t0+(10*CLOCKS_PER_SEC);
	while(t1<t3)
	{
		f=t2/((double)CLOCKS_PER_SEC);
		g=n/f;
		printf("Frame %d %d %ffps\r", n, t2, g);
		ctx->bpos=0;
		while((ctx->bpos>>5)<ctx->bpsz)
			{ cdec_decode0y(ctx); }

		n++;
		t1=clock();
		t2=t1-t0;
	}

	printf("\n");
#endif

	dbuf=malloc(640*480*4);
	cdec_getimage(ctx, dbuf);
	BTIC1H_Img_SaveTGA("cdec_frame.tga", dbuf, 640, 480);
	return(0);
}
