/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
// vid_null.c -- null video driver to aid porting efforts

#include "quakedef.h"
#include "d_local.h"

viddef_t	vid;				// global video state

// #define	BASEWIDTH	320
// #define	BASEHEIGHT	200
//#define	BASEHEIGHT	240

// #define	BASEWIDTH	480
// #define	BASEHEIGHT	360

// #define SCRZBUF

// #if 1
// #ifndef CONGFX
// #if !defined(CONGFX) && !defined(_BGBCC)
// #if !defined(CONGFX) && !defined(SCRZBUF)
#if 0

#define	BASEWIDTH	640
#define	BASEHEIGHT	480

#else

#define	BASEWIDTH	320
#define	BASEHEIGHT	200

#endif

#ifdef SCRZBUF

#define	DISPWIDTH	(BASEWIDTH*2)
#define	DISPHEIGHT	(BASEHEIGHT*2)

#else

#define	DISPWIDTH	BASEWIDTH
#define	DISPHEIGHT	BASEHEIGHT

#endif

//byte	vid_buffer[BASEWIDTH*(BASEHEIGHT+4)];
// byte	vid_buffer[BASEWIDTH*BASEHEIGHT*2];
// short	zbuffer[BASEWIDTH*(BASEHEIGHT+4)];
// byte	surfcache[256*1024];

byte	*vid_buffer;
short	*zbuffer;
byte	*surfcache;

unsigned short	d_8to16table[256];
unsigned	d_8to24table[256];
unsigned	d2d_8to24table[256];

u32 *vid_vram;
u32 *vid_vreg;

int vid_clamp255(int v)
{
	if(v<0)return(0);
	if(v>255)return(255);
	return(v);
}

int VID_BlendEven16(int pixa, int pixb)
{
	int pix;
	pix=((pixa&0xFBDF)+(pixb&0xFBDF))>>1;
	return(pix);
}

int VID_Blend66_16(int pixa, int pixb)
{
	int y0, y1, y2, u0, u1, u2, v0, v1, v2;
	
#if 0
	y0=(pixa&0xFC00);	y1=(pixb&0xFC00);	 y2=(11*y0+5*y1+0x02000)>>4;
	u0=(pixa&0x001F);	u1=(pixb&0x001F);	 u2=(11*u0+5*u1+0x00008)>>4;
	v0=(pixa&0x03E0);	v1=(pixb&0x03E0);	 v2=(11*v0+5*v1+0x00100)>>4;
	return((y2&0xFC00)|(u2&0x001F)|(v2&0x03E0));
//	return(y2|u2|v2);
#endif

	return(VID_BlendEven16(pixa, VID_BlendEven16(pixa, pixb)));
}

int VID_Blend75_16(int pixa, int pixb)
{
	int y0, y1, y2, u0, u1, u2, v0, v1, v2;
	
//	y0=(pixa&0xFC00);	y1=(pixb&0xFC00);	 y2=(12*y0+4*y1+0x01FFF)>>4;
//	u0=(pixa&0x001F);	u1=(pixb&0x001F);	 u2=(12*u0+4*u1+0x00007)>>4;
//	v0=(pixa&0x03E0);	v1=(pixb&0x03E0);	 v2=(12*v0+4*v1+0x000FF)>>4;

	y0=(pixa&0xFC00);	y1=(pixb&0xFC00);	 y2=(12*y0+4*y1+0x02000)>>4;
	u0=(pixa&0x001F);	u1=(pixb&0x001F);	 u2=(12*u0+4*u1+0x00008)>>4;
	v0=(pixa&0x03E0);	v1=(pixb&0x03E0);	 v2=(12*v0+4*v1+0x00100)>>4;

	return((y2&0xFC00)|(u2&0x001F)|(v2&0x03E0));

//	return(VID_BlendEven16(pixa, VID_BlendEven16(pixa, pixb)));
}

int VID_Blend88_16(int pixa, int pixb)
{
	int y0, y1, y2, u0, u1, u2, v0, v1, v2;
	
	y0=(pixa&0xFC00);	y1=(pixb&0xFC00);	 y2=(14*y0+2*y1+0x02000)>>4;
	u0=(pixa&0x001F);	u1=(pixb&0x001F);	 u2=(14*u0+2*u1+0x00008)>>4;
	v0=(pixa&0x03E0);	v1=(pixb&0x03E0);	 v2=(14*v0+2*v1+0x00100)>>4;
	return((y2&0xFC00)|(u2&0x001F)|(v2&0x03E0));

//	return(VID_BlendEven16(pixa, VID_BlendEven16(pixa, pixb)));
}

int VID_BlendRatio16(int pixa, int pixb, int rat)
{
	int y0, y1, y2, u0, u1, u2, v0, v1, v2;
	
	y0=(pixa&0xFC00);	y1=(pixb&0xFC00);
	y2=((255-rat)*y0+rat*y1+0x02000)>>8;
	u0=(pixa&0x001F);	u1=(pixb&0x001F);
	u2=((255-rat)*u0+rat*u1+0x00008)>>8;
	v0=(pixa&0x03E0);	v1=(pixb&0x03E0);
	v2=((255-rat)*v0+rat*v1+0x00100)>>8;
	return((y2&0xFC00)|(u2&0x001F)|(v2&0x03E0));

//	return(VID_BlendEven16(pixa, VID_BlendEven16(pixa, pixb)));
}

int VID_ScanBlendRatio16(u16 *dpix, u16 *spix, int cnt, int pixb, int rat)
{
	u16 *cs, *ct, *cse;
	int pixa, pixc;
	int y0, y1, y2, u0, u1, u2, v0, v1, v2;
	
	cs=spix; ct=dpix; cse=spix+cnt;
	
	while(cs<cse)
	{
		pixa=*cs++;
		y0=(pixa&0xFC00);	y1=(pixb&0xFC00);
		y2=((255-rat)*y0+rat*y1+0x02000)>>8;
		u0=(pixa&0x001F);	u1=(pixb&0x001F);
		u2=((255-rat)*u0+rat*u1+0x00008)>>8;
		v0=(pixa&0x03E0);	v1=(pixb&0x03E0);
		v2=((255-rat)*v0+rat*v1+0x00100)>>8;
		pixc=(y2&0xFC00)|(u2&0x001F)|(v2&0x03E0);
		*ct++=pixc;
	}

	return(0);
//	return(VID_BlendEven16(pixa, VID_BlendEven16(pixa, pixb)));
}

int VID_ColorMap16(int pix, int light)
{
	int d, m, m1, sc, scuv;
	int y1, u1, v1;

#if 0
//	d = ((l+(l>>2)) & 0xFC00);
//	d = (light & 0xFC00)-(8<<10);

	d = (light & 0xFC00)-(6<<10);
	d=(d+(d>>1))&((s16)0xFC00);

	pix= pix - d;
//	if(pix<0x0000)pix=0x01EF;
//	if(pix>0xFFFF)pix=0xFDEF;
	if(pix<0x0210)pix=0x0210;
	if(pix>0xFE10)pix=0xFE10;
#endif

#if 0
//	sc=(256+64)-((light>>8)&255)*4;
//	sc=(256+128)-((light>>8)&255)*8;
//	sc=(256+256)-((light>>8)&255)*8;

	sc=(512+128)-((light>>8)&255)*10;
//	scuv=(256+64)-((light>>8)&255)*5;
//	scuv=256-((light>>8)&255)*4;
//	scuv=256-((light>>8)&255)*5;
//	scuv=384-((light>>8)&255)*5;
	scuv=sc;

	if(sc<0)sc=0;
	y1=(pix&0xFC00);
	u1=(pix&0x001F);
	v1=(pix&0x03E0);
	y1=(y1*sc+0x01FFFF)>>8;
	u1=(((u1-0x0010)*scuv+0x00007F)>>8)+0x0010;
	v1=(((v1-0x0200)*scuv+0x000FFF)>>8)+0x0200;
	if((y1>>16) || (v1>>10) || (u1>>5))
//	if(1)
	{
		if(y1<0x0000)y1=0x0000;
		if(y1>0xFFFF)y1=0xFC00;
		if(u1<0x0000)u1=0x0000;
		if(u1>0x001F)u1=0x001F;
		if(v1<0x0000)v1=0x0000;
		if(v1>0x03EF)v1=0x03E0;
	}
	
	pix=(y1&0xFC00)|(u1&0x001F)|(v1&0x03E0);
#endif

#if 0
	sc=(512+128)-((light>>8)&255)*10;

	if(sc<0)sc=0;
	y1=(pix&0xFC00);
	y1=(y1*sc+0x01FFFF)>>8;
	if(y1>>16)
	{
		m=~(y1>>31);
//		m=(y1<<15)>>31;
//		y1=(y1&m)|m;
		y1=(u16)m;
//		if(y1<0x0000)y1=0x0000;
//		if(y1>0xFFFF)y1=0xFC00;
	}
	
//	pix=(y1&0xFC00)|(u1&0x001F)|(v1&0x03E0);
	pix=(y1&0xFC00)|(pix&0x03FF);
#endif

#if 1
	sc=(512+128)-((light>>8)&255)*10;
	scuv=sc;

	if(sc<0)sc=0;
	y1=(pix&0xFC00);
	u1=(pix&0x001F);
	v1=(pix&0x03E0);
	y1=(y1*sc+0x01FFFF)>>8;
//	u1=(((u1-0x0010)*scuv+0x00007F)>>8)+0x0010;
//	v1=(((v1-0x0200)*scuv+0x000FFF)>>8)+0x0200;
	u1=(((u1-0x0010)*scuv)>>8)+0x0010;
	v1=(((v1-0x0200)*scuv)>>8)+0x0200;

	if((y1>>16) | (v1>>10) | (u1>>5))
	{
		m=~(y1>>31);		m1=~((y1<<15)>>31);
		y1=(y1&m1)|(m&(~m1));
		m=~(u1>>31);		m1=~((u1<<26)>>31);
		u1=(u1&m1)|(m&(~m1));
		m=~(v1>>31);		m1=~((v1<<21)>>31);
		v1=(v1&m1)|(m&(~m1));
	}
	
	pix=(y1&0xFC00)|(u1&0x001F)|(v1&0x03E0);
#endif

	return(pix);
}

void	VID_SetPalette (unsigned char *palette)
{
	int cr, cg, cb;
	int cy, cu, cv;
	int i, j, k;
	
	for(i=0; i<256; i++)
	{
		cr=palette[i*3+0];
		cg=palette[i*3+1];
		cb=palette[i*3+2];
		
//		cr=cr*1.5;
//		cg=cg*1.5;
//		cb=cb*1.5;

#ifdef CONGFX		
		cy=(2*cg+cr+cb)/4;
		cu=((cb-cg)/2)+128;
		cv=((cr-cg)/2)+128;
#else
		cy=((  77*cr +150*cg + 29*cb + 127)>>8);
		cu=((- 43*cr - 85*cg +128*cb + 127)>>8)+128;
		cv=(( 128*cr -107*cg - 21*cb + 127)>>8)+128;
#endif

//		cy+=16;
		
//		cy=vid_clamp255(cy+1)>>2;
//		cu=vid_clamp255(cu+3)>>3;
//		cv=vid_clamp255(cv+3)>>3;
		cy=vid_clamp255(cy+2)>>2;
		cu=vid_clamp255(cu+4)>>3;
		cv=vid_clamp255(cv+4)>>3;
		d_8to16table[i]=(cy<<10)|(cv<<5)|cu;

//		d_8to16table[i]=((cy>>2)<<10)|((cv>>3)<<5)|(cu>>3);
		
//		d_8to16table[i]=((cr>>3)<<10)|((cg>>3)<<5)|(cb>>3);
//		d_8to24table[i]=0xFF000000|(cr<<16)|(cg<<8)|cb;
		d2d_8to24table[i]=0xFF000000|(cr<<16)|(cg<<8)|cb;
	}
	
	d_8to16table[255]=0x7FFF;
}

void	VID_ShiftPalette (unsigned char *palette)
{
//	VID_SetPalette(palette);
}

u16 vid_blendv;
int vid_blendp;

void	VID_ShiftPaletteVec (int dr, int dg, int db, int dpcnt)
{
	int cy, cu, cv;
	
//	cy=(dr+2*dg+db)/4;
//	cu=(db-dg)/2+128;
//	cv=(dr-dg)/2+128;

	cy=((  77*dr +150*dg + 29*db + 127)>>8);
	cu=((- 43*dr - 85*dg +128*db + 127)>>8)+128;
	cv=(( 128*dr -107*dg - 21*db + 127)>>8)+128;
	
	cy=vid_clamp255(cy);
	cu=vid_clamp255(cu);
	cv=vid_clamp255(cv);
	
	cy>>=2; cu>>=3; cv>>=3;
	
	vid_blendv=(cy<<10)|(cv<<5)|cu;
	vid_blendp=dpcnt;
}

void	VID_Init (unsigned char *palette)
{
	u32 *ict;
	int i, j, k;

	vid_buffer=malloc(BASEWIDTH*BASEHEIGHT*2);
	zbuffer=malloc(BASEWIDTH*BASEHEIGHT*2);
	surfcache=malloc(BASEWIDTH*BASEHEIGHT*3*2);
//	surfcache=malloc(512*1024);

	printf("VID_Init:\n");

//	vid.maxwarpwidth = vid.width = vid.conwidth = BASEWIDTH;
//	vid.maxwarpheight = vid.height = vid.conheight = BASEHEIGHT;

	vid.width = vid.conwidth = BASEWIDTH;
	vid.height = vid.conheight = BASEHEIGHT;
	vid.maxwarpwidth = WARP_WIDTH;
	vid.maxwarpheight = WARP_HEIGHT;

	vid.aspect = 1.0;
	vid.numpages = 1;
	vid.colormap = host_colormap;
	vid.fullbright = 256 - LittleLong (*((int *)vid.colormap + 2048));
//	vid.buffer = vid.conbuffer = vid_buffer;
	vid.buffer = vid.conbuffer = vid_buffer + (2*BASEWIDTH);
	vid.rowbytes = vid.conrowbytes = BASEWIDTH;
	
	d_pzbuffer = zbuffer;
//	D_InitCaches (surfcache, sizeof(surfcache));
//	D_InitCaches (surfcache, 512*1024);
	D_InitCaches (surfcache, BASEWIDTH*BASEHEIGHT*3*2);

#ifndef CONGFX
	vid_vreg=(u32 *)0xA05F8000;
	vid_vreg[(0x44/4)]=0x000D;
//	vid_vreg[(0x5C/4)]=320|(240<<10)|(320<<20);
//	vid_vreg[(0x5C/4)]=319|(239<<10)|(321<<20);
//	vid_vreg[(0x5C/4)]=319|(199<<10)|(321<<20);

//	vid_vreg[(0x5C/4)]=
//		(BASEWIDTH-1)|
//		((BASEHEIGHT-1)<<10)|
//		((BASEWIDTH+1)<<20);
	vid_vreg[(0x5C/4)]=
		(DISPWIDTH-1)|
		((DISPHEIGHT-1)<<10)|
		((DISPWIDTH+1)<<20);

	vid_vram=(u32 *)0xA5000000;
#endif

#if 1
//	vid.buffer = vid.conbuffer = (byte *) vid_vram;
	vid.buffer = vid.conbuffer = vid_buffer;
	vid.rowbytes = vid.conrowbytes = 2*BASEWIDTH;
	r_pixbytes=2;

//	host_colormap16 = Hunk_AllocName (131072, "colormap16");
	host_colormap16 = malloc (131072);

	vid.colormap16 = host_colormap16;
#endif

#ifndef CONGFX

#if 1
//	vid_vreg[(0x44/4)]=0x0005;
//	vid_vreg[(0x44/4)]=0x0001;
//	vid_vreg[(0x44/4)]=0x0081;
	vid_vreg[(0x44/4)]=0x0085;

//	vid_vreg[(0x5C/4)]=
//		(BASEWIDTH/2-1)|
//		((BASEHEIGHT-1)<<10)|
//		((BASEWIDTH/2+1)<<20);
	vid_vreg[(0x5C/4)]=
		(DISPWIDTH/2-1)|
		((DISPHEIGHT-1)<<10)|
		((DISPWIDTH/2+1)<<20);
#endif

#endif

	printf("VID_Init: B\n");

	VID_SetPalette(palette);

	printf("VID_Init: C\n");

	if(host_colormap16)
	{
		for(i=0; i<65536; i++)
		{
			host_colormap16[i]=d_8to16table[host_colormap[i]];
		}
	}

#if 0
	ict=vid_vram;
	for(i=0; i<240; i++)
	{
		for(j=0; j<320; j++)
		{
//			ict[j]=d_8to24table[ics[j]];
			*ict++=0xFFFFFFFF;
		}
	}

	for(i=0; i<100; i++)
	{
		ict=vid_vram+i*320;
		for(j=0; j<256; j++)
		{
//			ict[j]=d_8to24table[ics[j]];
			*ict++=d_8to24table[j];
		}
	}
#endif
}

void	VID_Shutdown (void)
{
}

#ifdef CONGFX

u16 VID_ConGfx_EncBlock16_YuvToRGB555(u16 clra)
{
	int cy, cu, cv, cu1, cv1;
	int cr, cg, cb;
	int clrb;
	
#if 1
	cy=(clra>>10)&63;
	cv=(clra>> 5)&31;
	cu=(clra    )&31;
	
	cu1=cu-16;
	cv1=cv-16;
//	cu1=0;
//	cv1=0;
	
	cg=cy>>1;
//	cg=cy-(cu1+cv1);
//	cg=(2*cy-(cu1+cv1))>>1;
	cb=cg+cu1;
	cr=cg+cv1;
#endif

#if 0
		cy=(pxa>>8)&0xFC;
		cv=(pxa>>2)&0xF8;
		cu=(pxa<<3)&0xF8;
		cy=cy|(cy>>6);
		if(cu<128)cu=cu|(cu>>5);
		if(cv<128)cv=cv|(cv>>5);

		cu1=(cu-128)<<1; cv1=(cv-128)<<1;
		cg=cy-((cu1+cv1)>>2);
		cb=cg+cu1;
		cr=cg+cv1;
		
		cr=cr>>3;
		cg=cg>>3;
		cb=cb>>3;
#endif
	
	if((cr|cg|cb)&0xE0)
	{
		if(cr<0)cr=0;
		if(cr>31)cr=31;
		if(cg<0)cg=0;
		if(cg>31)cg=31;
		if(cb<0)cb=0;
		if(cb>31)cb=31;
	}
	
	clrb=(cr<<10)|(cg<<5)|cb;
//	if(!clrb)clrb|=1;
	return(clrb);
}

u16 VID_ConGfx_EncBlock16_YuvToRGB444(u16 clra)
{
	int cy, cu, cv, cu1, cv1;
	int cr, cg, cb;
	int clrb;
	
#if 1
	cy=(clra>>10)&63;
	cv=(clra>> 5)&31;
	cu=(clra    )&31;
	
	cu1=cu-16;
	cv1=cv-16;
//	cu1=0;
//	cv1=0;
	
	cu1=cu1<<1;
	cv1=cv1<<1;
	
	cg=cy;
//	cg=cy>>2;
//	cg=(cy>>2)-((cu1+cv1)>>4);
//	cg=cy-((cu1+cv1)>>1);
//	cg=cy-(cu1+cv1);
//	cg=(2*cy-(cu1+cv1))>>1;
	cb=cg+cu1;
	cr=cg+cv1;
//	cb=cg+(cu1>>1);
//	cr=cg+(cv1>>1);

	cr=(cr+1)>>2;
	cg=(cg+1)>>2;
	cb=(cb+1)>>2;

#endif
	
	if((cr|cg|cb)&0xF0)
	{
		if(cr<0)cr=0;
		if(cr>15)cr=15;
		if(cg<0)cg=0;
		if(cg>15)cg=15;
		if(cb<0)cb=0;
		if(cb>15)cb=15;
	}
	
	clrb=(cb<<8)|(cg<<4)|cr;
	return(clrb);
}

void	VID_ConGfx_EncBlock16(u16 *src, u32 *dsta, u32 *dstb)
{
	const sbyte idxtab[8]={0,0, 0,1,2,3, 3,3};
	u16 tpxb[16];
	u16 *cs, *cs1, *ct;
	u16 px, px1;
	int cmin, cmax, cavg;
	u16 clra, clrb;
	u32 clrc, clrd;
	u32 px2;
	u32 dxa, dxb;
	int cy, cu, cv;
	int cy0, cy1, cu0, cu1, cv0, cv1;
	int cr, cg, cb, cd;
	int l0, l0b;
	int x, y;
	int i, j, k;

	cmin=65536;
	cmax=-1;
//	ct=tpxb+16;
	ct=tpxb; cs=src;
	for(y=0; y<4; y++)
	{
//		cs1=src+y*BASEWIDTH;
		cs1=cs;
		cs+=BASEWIDTH;
		for(x=0; x<4; x++)
		{
			px=*cs1++;
//			*(--ct)=px;
			*ct++=px;

//			px1=px;
			px1=px&0xFC00;

			if(px1<cmin)cmin=px;
			if(px1>cmax)cmax=px;
		}
	}
	
//	cmin=0x7FFF;
//	cmax=0x7FFF;
	
	cavg=(cmin+cmax)/2;
	
#if 1
	l0=(2*16777216)/(cmax-cavg+1);
	l0b=(4*16777216);
	ct=tpxb; px1=0; px2=0;
	for(i=0; i<16; i++)
	{
		px=*ct++;
		j=idxtab[((px-cavg)*l0+l0b)>>24];
		px2=(px2<<2)|j;
		px1=(px1<<1)|(px>cavg);
	}
	
//	px2=0xC33CC33C;
#endif

#if 0
	ct=tpxb; px1=0;
	for(i=0; i<16; i++)
	{
		px=*ct++;
		px1=px1<<1;
		if(px>cavg)
			px1|=1;
	}
#endif

//	px1&=0xFFFF;
//	px1=0;


//	conbufa[by2*80+bx]=0x30550000|px1;
//	conbufb[by2*80+bx]=(clrb<<16)|clra;

	cy0=cmin>>10;	cv0=(cmin>> 5)&31;	cu0=(cmin    )&31;
	cy1=cmax>>10;	cv1=(cmax>> 5)&31;	cu1=(cmax    )&31;
	cy=(cy0+cy1)>>1;
	cu=cu0+cu1;
	cv=cv0+cv1;

	j=(cu0-cu1); j=j^(j>>31);
	k=(cv0-cv1); k=k^(k>>31);

//	if((j+k)>8)
//	if((j+k)>4)
//	if((j+k)>12)
//	if(1)
	if(0)
	{
		clrb=VID_ConGfx_EncBlock16_YuvToRGB444(cmin);
		clra=VID_ConGfx_EncBlock16_YuvToRGB444(cmax);
		clrc=(clrb<<12)|clra;
		dxa=0x07000000|clrc;
		dxb=px2;

//		clrb=VID_ConGfx_EncBlock16_YuvToRGB555(cmin);
//		clra=VID_ConGfx_EncBlock16_YuvToRGB555(cmax);
//		dxa=0x00550000|px1;
//		dxb=(clrb<<16)|clra;
//		if(!dxb)dxb|=1;
		*dsta=dxa;
		*dstb=dxb;
		return;
	}

//	cu1=cu-32;	cv1=cv-32;	
	cu1=(cu-32)<<1;	cv1=(cv-32)<<1;
//	cg=cy;
//	cg=(4*cy-cu1-cv1)>>1;
	cg=cy-((cu1+cv1)>>1);
	cb=cg+cu1;	cr=cg+cv1;
	cd=cy1-cy0;
	
	if((cr|cg|cb|cd)&0xC0)
	{
		if(cr<0)cr=0;
		if(cg<0)cg=0;
		if(cb<0)cb=0;
		if(cd<0)cd=0;
		if(cr>63)cr=63;
		if(cg>63)cg=63;
		if(cb>63)cb=63;
		if(cd>63)cd=63;
	}
	
	clrc=cr|(cg<<6)|(cb<<12)|(cd<<18);
	dxa=0x03000000|clrc;
	dxb=px2;
	if(!dxb)dxb|=1;

//	clrb=VID_ConGfx_EncBlock16_YuvToRGB555(cmin);
//	clra=VID_ConGfx_EncBlock16_YuvToRGB555(cmax);
//	dxa=0x00550000|px1;
//	dxb=(clrb<<16)|clra;
//	if(!dxb)dxb|=1;
	*dsta=dxa;
	*dstb=dxb;
}
#endif

void tk_putc(int val);
int tk_puts(char *msg);

void	VID_ConGfx_SendB85_32(u32 bits)
{
	int v, v0, v1, v2, v3, v4;
	
	v=bits;
//	v4=v%85; v=v/85;
//	v3=v%85; v=v/85;
//	v2=v%85; v=v/85;
//	v1=v%85; v=v/85;
//	v0=v%85;

#if 0
	v4=v;
	v3=v4/85;
	v2=v3/85;
	v1=v2/85;
	v0=v1/85;
	v4-=v3*85;
	v3-=v2*85;
	v2-=v1*85;
	v1-=v0*85;
	
	tk_putc('!'+v0);
	tk_putc('!'+v1);
	tk_putc('!'+v2);
	tk_putc('!'+v3);
	tk_putc('!'+v4);
#endif

#if 1
	v4=v;
	v3=v4>>7;
	v2=v3>>7;
	v1=v2>>7;
	v0=v1>>7;
	
	tk_putc(0x80+(v0&127));
	tk_putc(0x80+(v1&127));
	tk_putc(0x80+(v2&127));
	tk_putc(0x80+(v3&127));
	tk_putc(0x80+(v4&127));
#endif

}

void	VID_Update (vrect_t *rects)
{
#ifdef CONGFX
	u32 *conbufa, *conbufb;
	int bx, by, by2;
#endif

	byte *ics;
	u16 *ict16, *ics16, *ics16b;
	u16 *icz16;
	u32 *ict;
	u32 bxa, bxb;
	int pix;
	int i, j, k;

#ifdef CONGFX
	conbufa=(u32 *)0xA0FF0000;
	conbufb=conbufa+(80*61);

	tk_puts("\x1B[1;1H");
//	tk_putc(0x1B);
//	tk_putc('$');

	if(host_colormap16)
	{
		r_pixbytes=2;

		tk_putc(0x1F);
		ics16=(u16 *)vid.buffer;
		for(by=0; by<50; by++)
		{
			tk_putc(0x1F);

			ics16b=ics16;
			for(bx=0; bx<80; bx++)
			{
				by2=by;

//				k=by2*80+bx;
//				VID_ConGfx_EncBlock16(ics16b, conbufa+k, conbufb+k);

				VID_ConGfx_EncBlock16(ics16b, &bxa, &bxb);
				if(bxb)
				{
					k=0x30000000;
//					if((bx+1)>=80)
//						k=0x20000000;
					VID_ConGfx_SendB85_32(bxa|k);
					VID_ConGfx_SendB85_32(bxb);
				}else
				{
					k=0x10000000;
//					if((bx+1)>=80)
//						k=0;
					VID_ConGfx_SendB85_32(bxa|k);
				}

				ics16b+=4;
			}
			ics16+=4*BASEWIDTH;
		}
	}
#endif

#ifndef CONGFX

	if(host_colormap16)
	{
		r_pixbytes=2;

		if(vid_blendp)
		{
			ics16=(u16 *)vid.buffer;
			ict16=(u16 *)vid_vram;
			for(i=0; i<BASEHEIGHT; i++)
			{
				ict16=((u16 *)vid_vram)+(i*DISPWIDTH);

#if 0
				for(j=0; j<BASEWIDTH; j++)
				{
					pix=*ics16++;
					pix=VID_BlendRatio16(pix, vid_blendv, vid_blendp);
					*ict16++=pix;
				}
#endif

#if 1
				VID_ScanBlendRatio16(
					ict16, ics16, BASEWIDTH,
					vid_blendv, vid_blendp);
				ics16+=BASEWIDTH;
#endif
			}
		}else
		{
			if(BASEWIDTH!=DISPWIDTH)
			{
				ics16=(u16 *)vid.buffer;
				ict16=(u16 *)vid_vram;
				icz16=(u16 *)d_pzbuffer;

				for(i=0; i<BASEHEIGHT; i++)
				{
#if 0
					ict16=((u16 *)vid_vram)+(i*DISPWIDTH);
					for(j=0; j<BASEWIDTH; j++)
					{
						pix=*ics16++;
						*ict16++=pix;
					}
#endif

#ifdef SCRZBUF
					for(j=0; j<BASEWIDTH; j++)
					{
						pix=icz16[j];
						
						if(pix>=0x4000)
						{
							pix=0xFFFF;
						}else if(pix>=0x2000)
						{
							pix=((pix<<1)&0x3FFF)|0xC000;
						}else if(pix>=0x1000)
						{
							pix=((pix<<2)&0x3FFF)|0x8000;
						}else if(pix>=0x0800)
						{
							pix=((pix<<3)&0x3FFF)|0x4000;
						}else
						{
//							pix=((pix<<4)&0x3FFF);
							pix=(pix<<4);
						}
						
//						if(pix<0x4000)pix<<=1;
//						if(pix<0x1000)pix<<=1;
//						if(pix<0x0400)pix<<=1;
//						if(pix<0x0100)pix<<=1;
//						pix=((pix<<6)&0xFFFFFC00)+0x0210;
//						pix=((pix<<2)&0xFFFFFC00)+0x0210;
						pix=((pix<<4)&0xFFFFFC00)+0x0210;
						if(pix<0x0210)pix=0x0210;
						if(pix>0xFE10)pix=0xFE10;
						ict16[BASEWIDTH+j]=pix;
					}

//					Q_memcpy(ict16+BASEWIDTH, icz16, BASEWIDTH*2);
					icz16+=BASEWIDTH;
#endif

					Q_memcpy(ict16, ics16, BASEWIDTH*2);
					ict16+=DISPWIDTH;
					ics16+=BASEWIDTH;
				}

				return;
			}
		
			Q_memcpy(vid_vram, vid.buffer, BASEWIDTH*BASEHEIGHT*2);
//			memcpy(vid_vram, vid.buffer, BASEWIDTH*BASEHEIGHT*2);

//			__debugbreak();
		}
		return;
	}
	
//	return;

#if 1
	ics=vid.buffer;
	ict16=(u16 *)vid_vram;
	for(i=0; i<BASEHEIGHT; i++)
	{
		ict16=((u16 *)vid_vram)+(i*DISPWIDTH);
		for(j=0; j<BASEWIDTH; j+=8)
		{
			ict16[0]=d_8to16table[ics[0]];
			ict16[1]=d_8to16table[ics[1]];
			ict16[2]=d_8to16table[ics[2]];
			ict16[3]=d_8to16table[ics[3]];
			ict16[4]=d_8to16table[ics[4]];
			ict16[5]=d_8to16table[ics[5]];
			ict16[6]=d_8to16table[ics[6]];
			ict16[7]=d_8to16table[ics[7]];
			ict16+=8; ics+=8;
		}
	}

#endif

#if 0
	ics=vid.buffer;
	ict=vid_vram;
	for(i=0; i<BASEHEIGHT; i++)
	{
//		for(j=0; j<BASEWIDTH; j++)
		for(j=0; j<BASEWIDTH; j+=8)
		{
//			ict[j]=d_8to24table[ics[j]];
//			*ict++=d_8to24table[*ics++];

			ict[0]=d2d_8to24table[ics[0]];
			ict[1]=d2d_8to24table[ics[1]];
			ict[2]=d2d_8to24table[ics[2]];
			ict[3]=d2d_8to24table[ics[3]];
			ict[4]=d2d_8to24table[ics[4]];
			ict[5]=d2d_8to24table[ics[5]];
			ict[6]=d2d_8to24table[ics[6]];
			ict[7]=d2d_8to24table[ics[7]];
			ict+=8; ics+=8;
		}
	}

#endif

//	for(j=0; j<256; j++)
//	{
//		*ict++=d_8to24table[j];
//	}
#endif

}

/*
================
D_BeginDirectRect
================
*/
void D_BeginDirectRect (int x, int y, byte *pbitmap, int width, int height)
{
}


/*
================
D_EndDirectRect
================
*/
void D_EndDirectRect (int x, int y, int width, int height)
{
}


