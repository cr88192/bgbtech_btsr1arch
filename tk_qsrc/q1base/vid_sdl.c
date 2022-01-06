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

// #include "gfxdrv_keys.h"

void GfxDrv_EndDrawing(void);
int GfxDrv_Start();
void GfxDrv_BeginDrawing();
void GfxDrv_EndDrawing(void);
int GfxDrv_PrepareFramebuf();



void __hint_use_egpr()
{
}

#define	BASEWIDTH	320
#define	BASEHEIGHT	200

byte	vid_buffer[BASEWIDTH*BASEHEIGHT*2];
short	zbuffer[BASEWIDTH*BASEHEIGHT];
byte	surfcache[256*1024];

int vid_flashblend;

unsigned short	d_8to16table[256];
unsigned	d_8to24table[256];
unsigned	d_16to24table[65536];

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

int R_PixelBlend(int pixa, int pixb)
{
	return(((pixa&0x7BDE)+(pixb&0x7BDE))>>1);
}

int VID_BlendFlash(int pix, int flash)
{
	int pix1;
	switch((flash>>16)&7)
	{
	case 0:
		pix1=pix;
		break;
	case 1:
		pix1=R_PixelBlend(pix, flash);
		pix1=R_PixelBlend(pix, pix1);
		pix1=R_PixelBlend(pix, pix1);
		break;
	case 2:
		pix1=R_PixelBlend(pix, flash);
		pix1=R_PixelBlend(pix, pix1);
		break;
	case 3:
		pix1=R_PixelBlend(pix, flash);
		pix1=R_PixelBlend(pix1, flash);
		pix1=R_PixelBlend(pix, pix1);
		break;
	case 4:
		pix1=R_PixelBlend(pix, flash);
		break;
	case 5:
		pix1=R_PixelBlend(pix, flash);
		pix1=R_PixelBlend(pix1, flash);
		break;
	case 6:
		pix1=R_PixelBlend(pix, flash);
		pix1=R_PixelBlend(pix1, flash);
		pix1=R_PixelBlend(pix1, flash);
		break;
	case 7:
		pix1=flash&65535;
		break;
	}
	return(pix1);
}

#if 1
int VID_ColorMap16(int pix, int light)
{
	int llv, pix1;

#if 0
	llv = 255-(((light>>8)&255)*3);
	
	pix1 =
		((((pix&0x7C00)*llv)>>8)&0x7C00) |
		((((pix&0x03E0)*llv)>>8)&0x03E0) |
		((((pix&0x001F)*llv)>>8)&0x001F) ;
	return(pix1);
	
#endif

#if 1
//	pix1=pix;
	pix1=0;
//	llv=(light>>12)&15;
//	llv=light>>10;
//	llv=15-((light>>12)&15);
//	llv=15-((light>>10)&15);
//	llv = 255-(((light>>8)&255)*3);
	llv = 255-((byte)(light>>8)*3u);
	llv >>= 4;

	if((llv&15)==15)
		return(pix);

//	llv=15;
	if(llv&1)
		pix1 = pix1 + ((pix&0x4210)>>4);
	if(llv&2)
		pix1 = pix1 + ((pix&0x6318)>>3);
	if(llv&4)
		pix1 = pix1 + ((pix&0x739C)>>2);
	if(llv&8)
		pix1 = pix1 + ((pix&0x7BDE)>>1);
//		pix1 = (pix1&0x7BDE)>>1;
	return(pix1);
#endif

}
#endif

#if 0
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
#endif

void Sys_SendKeyEvents (void)
{
}

void	VID_ShiftPaletteVec (int dr, int dg, int db, int dpcnt)
{
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
		
		d_8to16table[i]=
			((cr<<7)&0x7C00)|
			((cg<<2)&0x03E0)|
			((cb>>3)&0x001F);
	}
	
	d_8to16table[255]=0x7FFF;

	for(i=0; i<65536; i++)
	{
		cr=(i>>10)&31;	cg=(i>> 5)&31;	cb=(i>> 0)&31;
		cr=(cr<<3)|(cr>>2);
		cg=(cg<<3)|(cg>>2);
		cb=(cb<<3)|(cb>>2);
		
		cr=cr+(cr>>1);	if(cr>255)cr=255;
		cg=cg+(cg>>1);	if(cg>255)cg=255;
		cb=cb+(cb>>1);	if(cb>255)cb=255;
		
		d_16to24table[i]=
			0xFF000000U|
			(cr<<16)|
			(cg<< 8)|
			(cb<< 0);
	}
}

void	VID_ShiftPalette (unsigned char *palette)
{
}

byte *btesh2_gfxcon_framebuf;
int btesh2_gfxcon_fb_dirty;
int btesh2_gfxcon_fbxs;
int btesh2_gfxcon_fbys;
int btesh2_gfxcon_swaprb;

void	VID_Init (unsigned char *palette)
{
	int i, j, k;

	btesh2_gfxcon_fbxs=BASEWIDTH * 2;
	btesh2_gfxcon_fbys=BASEHEIGHT * 2;

	GfxDrv_Start();
	
	GfxDrv_PrepareFramebuf();


	vid.maxwarpwidth = vid.width = vid.conwidth = BASEWIDTH;
	vid.maxwarpheight = vid.height = vid.conheight = BASEHEIGHT;
	vid.aspect = 1.0;
	vid.numpages = 1;
	vid.colormap = host_colormap;
	vid.fullbright = 256 - LittleLong (*((int *)vid.colormap + 2048));
	vid.buffer = vid.conbuffer = vid_buffer;
	vid.rowbytes = vid.conrowbytes = BASEWIDTH;

	vid.rowbytes = vid.conrowbytes = 2*BASEWIDTH;
	r_pixbytes=2;

	host_colormap16 = malloc (131072);
	vid.colormap16 = host_colormap16;


	VID_SetPalette(palette);

	printf("VID_Init: C\n");

	if(host_colormap16)
	{
		for(i=0; i<65536; i++)
		{
			host_colormap16[i]=d_8to16table[host_colormap[i]];
		}
	}


	d_pzbuffer = zbuffer;
	D_InitCaches (surfcache, sizeof(surfcache));
}

void	VID_Shutdown (void)
{
}

u16 *FRGL_GetKeybuf();

void I_HandleInput (void)
{
	u16 *kb;
	int j;

	kb=FRGL_GetKeybuf();
	
	while(*kb)
	{
		j=*kb++;
//		Key_Event(j&4095, !(j&0x8000));
		Key_Event(j&255, !(j&0x8000));
	}
}

void	VID_Update (vrect_t *rects)
{
	u16 *cs;
	u32 *ct0, *ct1;
	int x, y, p;

	I_HandleInput();

	GfxDrv_BeginDrawing();

	if(!btesh2_gfxcon_framebuf)
		return;

	for(y=0; y<BASEHEIGHT; y++)
	{
		cs=((u16 *)vid_buffer)+(y*BASEWIDTH);
//		ct=((u16 *)vid_vram)+(y*BASEWIDTH);
//		ct=((u32 *)btesh2_gfxcon_framebuf)+(y*BASEWIDTH);
		ct0=((u32 *)btesh2_gfxcon_framebuf)+((y*2+0)*(2*BASEWIDTH));
		ct1=((u32 *)btesh2_gfxcon_framebuf)+((y*2+1)*(2*BASEWIDTH));
		for(x=0; x<BASEWIDTH; x++)
		{
//			*ct++=d_8to16table[*cs++];
			p=*cs++;

//			p=0x5555;

			if(vid_flashblend)
			{
//				p=VID_BlendFlash(p, vid_flashblend);
			}

			*ct0++=d_16to24table[p];
			*ct0++=d_16to24table[p];
			*ct1++=d_16to24table[p];
			*ct1++=d_16to24table[p];
		}
	}

	btesh2_gfxcon_fb_dirty=1;
	GfxDrv_EndDrawing();
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


