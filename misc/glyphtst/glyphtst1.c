#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <time.h>


#ifndef BTIC2F_BYTE
#define BTIC2F_BYTE
typedef unsigned char			byte;
typedef unsigned short		u16;
typedef unsigned int			u32;
typedef unsigned long long	u64;

typedef signed char			sbyte;
typedef signed short			s16;
typedef signed int			s32;
typedef signed long long		s64;
#endif

#include "bt1h_targa.c"

#include "load_gtf.c"

int main()
{
	u16 cp1252mod[32]={
		0x20AC, 0x0000, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021,
		0x02C6, 0x2030, 0x0160, 0x2039, 0x0152, 0x0000, 0x017D, 0x0000,
		0x0000, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,
		0x02DC, 0x2122, 0x0161, 0x203A, 0x0153, 0x0000, 0x017E, 0x0178};
	char tb[256];
	byte tbu[256];
	BGTF_FontImage *font;
	BGTF_FontGlyph *glif;
	byte *ibuf, *obuf;
	char *ifn;
	int xs, ys, cxs, cys;
	int cr, cg, cb, cco, ccb;
	int cy0, cy1, cy2, cy3;
	int p, x, y, cx, cy, x0, y0, x1, y1;
	int i, j, k;
	
	
//	font=BGTF_LoadFontImage("test0.gtf");
	font=BGTF_LoadFontImage("Cantarell-Bold.gtf");
//	font=BGTF_LoadFontImage("Cantarell-Regular.gtf");


	xs=2048;
	ys=2048;
	cxs=128;
	cys=128;
	
	obuf=malloc(xs*ys*4);
	ibuf=malloc(xs*ys);
	
	k=xs*ys;
	
	for(i=0; i<k; i++)
	{
		cr=128;
		cg=128;
		cb=128;
	
		obuf[i*4+0]=cr;
		obuf[i*4+1]=cg;
		obuf[i*4+2]=cb;
		obuf[i*4+3]=255;
	}
	
	for(i=0; i<256; i++)
	{
		j=i;
		if((i&0xE0)==0x80)
			j=cp1252mod[i&0x1F];
		glif=BGTF_ImageGetGlyph(font, j);
		if(!glif)
			continue;

		cx=(i>>0)&15;
		cy=(i>>4)&15;
		
//		cx=(glif->utf>>0)&15;
//		cy=(glif->utf>>4)&15;
		
		cy=15-cy;

		BGTF_GlyphDrawIndex8(glif,
			ibuf, xs, ys, cx*cxs, cy*cys,
			cxs, cys, 15, 0);

		for(y=0; y<cys; y++)
			for(x=0; x<cxs; x++)
		{
			k=((cy*cys+y)*xs)+(cx*cxs+x);
			
			cg=ibuf[k]?255:0;
			cr=cg;
			cb=cg;

			if(ibuf[k]==0xF8)
				{ cr=192; cg=192; cb=192; }
			if(ibuf[k]==0xF9)
				{ cr=0; cg=0; cb=0; }
			if(ibuf[k]==0xFA)
				{ cr=255; cg=255; cb=255; }
			if(ibuf[k]==0xFB)
				{ cr=64; cg=64; cb=64; }
			
			obuf[k*4+0]=cr;
			obuf[k*4+1]=cg;
			obuf[k*4+2]=cb;
			obuf[k*4+3]=255;
		}

#if 0
		for(y=0; y<cys; y++)
			for(x=0; x<cxs; x++)
		{
			j=BGTF_GlyphCheckPoint(glif, (x-16)*12, (y-16)*12, 12);
			if(j)
			{
				cg=0;
			}else
			{
				cg=255;
			}
			cr=cg;
			cb=cg;
			
			k=((cy*cys+y)*xs)+(cx*cxs+x);
			
			obuf[k*4+0]=cr;
			obuf[k*4+1]=cg;
			obuf[k*4+2]=cb;
			obuf[k*4+3]=255;
		}
#endif
	}

	BTIC1H_Img_SaveTGA("glyph0b.tga", obuf, xs, ys);


	cxs=24;
	cys=24;
	for(i=0; i<256; i++)
	{
		j=i;
		if((i&0xE0)==0x80)
			j=cp1252mod[i&0x1F];

		cx=(i>>0)&15;
		cy=(i>>4)&15;
				
		cy=15-cy;
		
		BGTF_FetchFontGlyphBits(tbu, cxs, cys, font, j);

#if 1
		for(y=0; y<cys; y++)
			for(x=0; x<cxs; x++)
		{
			k=y*cys+x;
			j=(tbu[k>>3]>>(k&7))&1;
		
			if(j)
			{
				cg=0;
			}else
			{
				cg=255;
			}
			cr=cg;
			cb=cg;
			
			k=((cy*cys+y)*(cxs*16))+(cx*cxs+x);
			
			obuf[k*4+0]=cr;
			obuf[k*4+1]=cg;
			obuf[k*4+2]=cb;
			obuf[k*4+3]=255;
		}
#endif
	}

	BTIC1H_Img_SaveTGA("glyph0c.tga", obuf, 16*cxs, 16*cys);
}
