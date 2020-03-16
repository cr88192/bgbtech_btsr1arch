#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "rt_def.h"
#include "rt_vid.h"
#include "rt_draw.h"
#include "rt_scale.h"
#include "rt_floor.h"
#include "rt_view.h"
#include "f_scale.h"
#include "isr.h"


#define MAXSCREENHEIGHT			200
#define MAXSCREENWIDTH			 320
#define SCREENBWIDE				 96
#define MAXVIEWWIDTH				320
#define SCREENWIDTH				 96				  // default screen width in bytes

#define SCREENAWIDTH			 (96*4)

u16		screen_pal[256];

int	 linewidth;
int	 screensize;

unsigned	 ylookup[MAXSCREENHEIGHT];

unsigned		page1start;
unsigned		page2start;
unsigned		page3start;

unsigned	bufferofs = 0;
unsigned	displayofs = 0;
byte	screen_planemask;
byte	screen_readplane;
byte	screen_writeplane;
byte	screen_planemask_ispow2;
byte	screen_planemask_ispair;
int		screen_buffersz;
int		screen_buffermsk;

byte *screenbuf = NULL;


void I_StartFrame (void);
void I_FinishUpdate (void);


byte *btesh2_gfxcon_framebuf;
int btesh2_gfxcon_fb_dirty;
int btesh2_gfxcon_fbxs;
int btesh2_gfxcon_fbys;

u32	d_16to24table[65536];

void I_SetupScreenTables()
{
	int cr, cg, cb, cc;
	int i, j, k;
	
	for(i=0; i<65536; i++)
	{
		cr=(i>>10)&31;	cr=(cr<<3)|(cr>>2);
		cg=(i>> 5)&31;	cg=(cg<<3)|(cg>>2);
		cb=(i>> 0)&31;	cb=(cb<<3)|(cb>>2);
//		cc=0xFF000000|(cr<<16)|(cg<<8)|cb;
		cc=0xFF000000|(cb<<16)|(cg<<8)|cr;
		d_16to24table[i]=cc;
	}
}


void GraphicsMode ( void )
{
	int i, j, k;

	if(!screenbuf)
	{
#ifdef _WIN32
		GfxDrv_Start();

		btesh2_gfxcon_fbxs=320;
		btesh2_gfxcon_fbys=200;

		GfxDrv_PrepareFramebuf();
#endif

//		screen_buffersz = (96*4*200)+(96*4*200);
		screen_buffersz = (1<<18);
		screen_buffermsk = (screen_buffersz-1)&(~3);
//		screen_buffersz = 360*241;
		screenbuf = malloc(screen_buffersz);
		bufferofs = 0;
		displayofs = 0;

		screensize = 96*200;
		linewidth = 96;
		
		page1start = 0;
		page2start = 0;
		page3start = 0;
		
		I_StartFrame();
		I_SetupScreenTables();
	}

	for (i=0;i<MAXSCANLINES;i++)
	{
//		ylookup[i]=i*320;
		ylookup[i]=i*96;
	}
}

void TextMode ( void )
{
}

void VL_ClearVideo (byte color)
{
	memset(screenbuf, color, screen_buffersz);
}


#if 1
void VGAMAPMASK(int mask)
{	
	screen_planemask=mask&15;
	
	switch(screen_planemask)
	{
	case 1:
		screen_writeplane=0;
		screen_planemask_ispow2=1;
		screen_planemask_ispair=0;
		break;
	case 2:
		screen_writeplane=1;
		screen_planemask_ispow2=1;
		screen_planemask_ispair=0;
		break;
	case 4:
		screen_writeplane=2;
		screen_planemask_ispow2=1;
		screen_planemask_ispair=0;
		break;
	case 8:
		screen_writeplane=3;
		screen_planemask_ispow2=1;
		screen_planemask_ispair=0;
		break;

	case 3:
		screen_writeplane=0;
		screen_planemask_ispow2=0;
		screen_planemask_ispair=1;
		break;
	case 6:
		screen_writeplane=1;
		screen_planemask_ispow2=0;
		screen_planemask_ispair=1;
		break;
	case 12:
		screen_writeplane=2;
		screen_planemask_ispow2=0;
		screen_planemask_ispair=1;
		break;

	default:
		screen_writeplane=0;
		screen_planemask_ispow2=0;
		screen_planemask_ispair=0;
		break;
	}
}

void VGAREADMAP(int plane)
{
	screen_readplane=plane;
}

void VGAWRITEMAP(int plane)
{
	screen_writeplane=plane;
	screen_planemask=1<<plane;
	screen_planemask_ispow2=1;
	screen_planemask_ispair=0;
//	VGAMAPMASK(1<<plane);
}

#if 1
void *VGA_GetAsPtr(unsigned addr)
{
	byte *buf;
	int a1, msk;

	a1=(addr<<2)&screen_buffermsk;
//	msk=screen_planemask;
	buf=screenbuf+a1+screen_writeplane;
	return(buf);
}
#endif

// #define VGA_GETASPTR(addr)	(screenbuf+((addr)<<2)+screen_writeplane)
#define VGA_GETASPTR(addr)	(screenbuf+(((addr)&0x7fff)<<2)+screen_writeplane)

byte VGAREAD(unsigned addr)
{
	byte *buf;
	int a1, msk;

	a1=(addr<<2)&screen_buffermsk;
	buf=screenbuf+a1+screen_readplane;
	return(*buf);
}

void VGAWRITE(unsigned addr, byte clr)
{
	byte *buf;
	int a1, msk;

	a1=(addr<<2)&screen_buffermsk;
	msk=screen_planemask;
	buf=screenbuf+a1;
	if(msk&1)	buf[0]=clr;
	if(msk&2)	buf[1]=clr;
	if(msk&4)	buf[2]=clr;
	if(msk&8)	buf[3]=clr;
}

void VGAWRITEBUF(unsigned addr, byte *src, int sz)
{
	byte *cs, *ct, *cte, *ct1e;
	int i, v;

	cs=src;
	ct=VGA_GETASPTR(addr);
	cte=ct+(sz*4);
	
	if(screen_planemask==15)
	{
		while(ct<cte)
		{
			v=*cs++;
			v|=(v<<8); v|=v<<16;
			*(u32 *)ct=v;
			ct+=4;
		}
		return;
	}
	
//	if(!(screen_planemask&(screen_planemask-1)))
	if(screen_planemask_ispow2)
	{
		ct1e=cte-16;
//		while((ct+16)<=cte)
		while(ct<=ct1e)
		{
			ct[ 0]=cs[0];	ct[ 4]=cs[1];
			ct[ 8]=cs[2];	ct[12]=cs[3];
			cs+=4;			ct+=16;
		}
		while(ct<cte)
		{
			v=*cs++;
			*ct=v;
			ct+=4;
		}
		return;
	}
	
	if(screen_planemask_ispair)
	{
		while(ct<cte)
		{
			v=*cs++;
			ct[0]=v;
			ct[1]=v;
			ct+=4;
		}
		return;
	}
	
	for(i=0; i<sz; i++)
		VGAWRITE(addr+i, src[i]);
}

void VGAREADBUF(byte *dst, unsigned addr, int sz)
{
	int i;
	
	for(i=0; i<sz; i++)
		dst[i]=VGAREAD(addr+i);
}

void VGAMEMCPY(unsigned dst, unsigned src, int sz)
{
	byte *ct, *cte;
	int i;
	
	for(i=0; i<sz; i++)
		VGAWRITE(dst+i, VGAREAD(src+i));
}

void VGAMEMSET(unsigned addr, byte val, int sz)
{
	byte *ct, *cte;
	int i, v;
	
	if(screen_planemask==15)
	{
		v=val|(val<<8); v|=v<<16;
		ct=VGA_GETASPTR(addr);
		cte=ct+(sz*4);
		while(ct<cte)
		{
			*(u32 *)ct=v;
			ct+=4;
		}
		return;
	}
	
	for(i=0; i<sz; i++)
		VGAWRITE(addr+i, val);
}
#endif

void VL_SetVGAPlaneMode ( void )
{
	int i;

	GraphicsMode();
}

void *VL_SetVGA_Mode13LFB ( void )
{
	GraphicsMode();
	return(screenbuf);
}

void VL_FillPalette (int red, int green, int blue)
{
	int i;

	for(i=0; i<256; i++)
		VL_SetColor(i, red, green, blue);
}

void VL_SetColor  (int color, int red, int green, int blue)
{
	int cr, cg, cb;
	
//	cr=(red		>>3)&31;
//	cg=(green	>>3)&31;
//	cb=(blue	>>3)&31;

//	cr=(red		+4>>3);
//	cg=(green	+4>>3);
//	cb=(blue	+4>>3);

	cr=(red		+1>>1);
	cg=(green	+1>>1);
	cb=(blue	+1>>1);

	if(cr>31)cr=31;
	if(cg>31)cg=31;
	if(cb>31)cb=31;

	screen_pal[color]=(cr<<10)|(cg<<5)|(cb);
}

void VL_GetColor  (int color, int *red, int *green, int *blue)
{
	int cr, cg, cb, cc;

	cc=screen_pal[color];
	cr=(cc>>10)&31;	cr=(cr<<3)|(cr>>2);
	cg=(cc>> 5)&31;	cg=(cg<<3)|(cg>>2);
	cb=(cc>> 0)&31;	cb=(cb<<3)|(cb>>2);
	*red=cr;
	*green=cg;
	*blue=cb;
}

void SetBorderColor (int color)
{
	/* NOP */
}

void VL_SetPalette (byte *pal)
{
	byte *cs;
	int i;
	
	cs=pal;
	for(i=0; i<256; i++)
	{
		VL_SetColor(i, cs[0], cs[1], cs[2]);
		cs+=3;
	}
}

void VL_GetPalette (byte *pal)
{
	byte *cs;
	int cr, cg, cb;
	int i;
	
	cs=pal;
	for(i=0; i<256; i++)
	{
		VL_GetColor(i, &cr, &cg, &cb);
		cs[0]=cr;	cs[1]=cg;	cs[2]=cb;
		cs+=3;
	}
}

#ifdef _WIN32
u16 *FRGL_GetKeybuf();
void I_HandleInput (void)
{
	u16 *kb;
	int j;

	GfxDrv_HandleInput();

	kb=FRGL_GetKeybuf();
	
	while(*kb)
	{
		j=*kb++;
		Key_Event(j&4095, !(j&0x8000));
	}
}
#endif

#ifdef _BGBCC
void I_HandleInput (void)
{
	int c, dn;

//	while(*kb)
	while(tk_kbhit()>0)
	{
		c=tk_getch();
	
		switch(c)
		{
		case 0x7F:
			c=tk_getch(); dn=1;
			break;
		case 0xFF:
			c=tk_getch(); dn=0;
			break;
		case 0x80:
			c=tk_getch();
			c=(c<<8)|tk_getch();
			dn=(c&0x8000)?0:1;
			break;
		default:
			dn=(c&0x80)?0:1;
			c=c&0x7F;
			break;
		}
		
#if 0
		switch(c)
		{
		case   8: c=K_BACKSPACE; break;
		case 153: c=K_PAUSE; break;
		case 154: c=K_MWHEELUP; break;
		case 155: c=K_MWHEELDOWN; break;
		case 157: c=K_MOUSE1; break;
		case 158: c=K_MOUSE2; break;
		case 159: c=K_MOUSE3; break;
		default: break;
		}
#endif
		
//		j=*kb++;
//		Key_Event(j&4095, !(j&0x8000));

		Key_Event (c, dn);
	}
}
#endif

#ifdef _WIN32
void I_StartFrame (void)
{
//	dt_scrpix *cs;
//	u32 *ct;
	int x, y, p;

	I_HandleInput();

	GfxDrv_BeginDrawing();
	
}
#endif

#ifdef _BGBCC
void I_StartFrame (void)
{
	I_HandleInput();
}
#endif

#ifdef _WIN32
void I_FinishUpdate (void)
{
	int x, y, c;

	for(y=0; y<200; y++)
		for(x=0; x<320; x++)
	{
		c=screenbuf[y*SCREENAWIDTH+x];
		c=screen_pal[c];
		c=d_16to24table[c];
		((u32 *)btesh2_gfxcon_framebuf)[y*320+x]=c;
	}

	btesh2_gfxcon_fb_dirty=1;
	GfxDrv_EndDrawing();	
}
#endif

#ifdef _BGBCC

u64 I_FinishUpdate_Repack8to16 (u32 pxa);

#if 0
u64 I_FinishUpdate_Repack8to16 (u32 pxa)
{
	u16 *pal;
	u64 px;
	
	pal=screen_pal;
	px=         pal[(pxa>>24)&255];
	px=(px<<16)|pal[(pxa>>16)&255];
	px=(px<<16)|pal[(pxa>> 8)&255];
	px=(px<<16)|pal[(pxa    )&255];
	return(px);
}
#endif

static int vid_frnum;

#define		I_FINISHUPDATE_REPACK(pxa, ixa)		\
			pxa=          pal[(ixa>>24)&255];	\
			pxa=(pxa<<16)|pal[(ixa>>16)&255];	\
			pxa=(pxa<<16)|pal[(ixa>> 8)&255];	\
			pxa=(pxa<<16)|pal[(ixa    )&255];

__asm {
I_FinishUpdate_Repack8to16:
	MOV		screen_pal, R5
	SHLD	R4, -24, R6
	SHLD	R4, -16, R7		|	MOV.W	(R5, R6), R2
	EXTU.B	R7, R7			|	SHLD	R4, -8, R6
	SHLD.Q	R2, 16, R2		|	MOV.W	(R5, R7), R3
	EXTU.B	R6, R6			|	OR		R3, R2
	SHLD.Q	R2, 16, R2		|	MOV.W	(R5, R6), R3
	EXTU.B	R4, R7			|	OR		R3, R2
	SHLD.Q	R2, 16, R2		|	MOV.W	(R5, R7), R3
	OR		R3, R2			|	RTSU
};


void I_FinishUpdate (void)
{
	u32 *conbufa;
	u16 *pal;
	byte *ics, *icsb;
	u32 *ict;
	int bx, by, by2;

//	u64 pxa, pxb, pxc, pxd;
	u32 ixa, ixb, ixc, ixd;
	register u64 pxa, pxb, pxc, pxd;

	__hint_use_egpr();

//	conbufa=(u32 *)0xA00A0000;
	conbufa=(u32 *)0xF00A0000;

	((u32 *)0xF00BFF00)[8]=vid_frnum;
	vid_frnum++;

	((u32 *)0xF00BFF00)[0]=0x0095;		//320x200x16bpp, RGB555
//	((u32 *)0xF00BFF00)[0]=0x0000;		//320x200, Color Cell

	ics=screenbuf;
	ict=conbufa;
	pal=screen_pal;

	for(by=0; by<50; by++)
	{
		icsb=ics;

		for(bx=0; bx<80; bx++)
		{
			ixa=((u32 *)icsb)[(0*SCREENAWIDTH)/4];
			ixb=((u32 *)icsb)[(1*SCREENAWIDTH)/4];
			ixc=((u32 *)icsb)[(2*SCREENAWIDTH)/4];
			ixd=((u32 *)icsb)[(3*SCREENAWIDTH)/4];
			
			pxa=I_FinishUpdate_Repack8to16(ixa);
			pxb=I_FinishUpdate_Repack8to16(ixb);
			pxc=I_FinishUpdate_Repack8to16(ixc);
			pxd=I_FinishUpdate_Repack8to16(ixd);

//			I_FINISHUPDATE_REPACK(pxa, ixa);
//			I_FINISHUPDATE_REPACK(pxb, ixb);
//			I_FINISHUPDATE_REPACK(pxc, ixc);
//			I_FINISHUPDATE_REPACK(pxd, ixd);

			((u64 *)ict)[0]=pxa;	((u64 *)ict)[1]=pxb;
			((u64 *)ict)[2]=pxc;	((u64 *)ict)[3]=pxd;

			ict+=8;
			icsb+=4;
		}

		ics+=4*SCREENAWIDTH;
	}
}
#endif

void TurnShakeOff( void )
{
}

void VL_ClearBuffer (unsigned buf, byte color)
{
	VGAMAPMASK(15);
	VGAMEMSET(buf, color, screensize);
}

void WaitVBL( void )
{
}

void XFlipPage ( void )
{
	I_FinishUpdate();
	I_StartFrame();
}

void FlipPage ( void )
{
	I_FinishUpdate();
	I_StartFrame();
}

void TurnOffTextCursor ( void )
{
}

void VH_UpdateScreen (void)
{
}

void RefreshClear (void)
{
}

void ISR_SetTime(int tc)
{
	ticcount=tc;
}


void I_StartupTimer(void)
{
}

void I_ShutdownTimer(void)
{
}


//void I_StartupKeyboard(void)
//{
//}

void I_ShutdownKeyboard(void)
{
}

void PollMouseMove(void)
{
}

void PollJoystickMove(void)
{
}


void I_PollTimer(void)
{
	static int ltic;
	int ms, tic, dt;

#ifdef _WIN32
	ms=FRGL_TimeMS();
#endif

#ifdef _BGBCC
	ms=TK_GetTimeMs();
#endif

	tic=ms*(35.0/1000.0);
	dt=tic-ltic;
	ltic=tic;
	
	ticcount+=dt;
}

void I_Delay(int tics)
{
}

void R_DrawFilmColumn (unsigned buf)
{
	unsigned ct, cte;
	byte *src, *ct2, *ct2e;
	int i, j, k, n, isc;
	
	return;
	
	n=cin_yh-cin_yl;
	src=cin_source;
	isc=cin_iscale;

//	if(!(screen_planemask&(screen_planemask-1)))
	if(screen_planemask_ispow2)
	{
		ct2 = VGA_GETASPTR(buf);
		ct2e = ct2+n;
		i=0;
		while(ct2<ct2e)
		{
			j=i>>16;
			k=src[j];
			*ct2=k;
			ct2+=384;
			i+=isc;
		}
		return;
	}

	i=0;
	ct=buf; cte=ct+n;
	while(ct<cte)
	{
		j=i>>16;
		k=src[j];
		VGAWRITE(ct, k);
//		ct+=384;
		ct+=96;
		i+=isc;
	}
}

void DrawFilmPost (unsigned buf, byte * src, int height)
{
#if 1
	byte *cs, *ct, *cse;
	int i, a, pix;

//	if(!(screen_planemask&(screen_planemask-1)))
	if(screen_planemask_ispow2)
	{
		ct = VGA_GETASPTR(buf);
		cs=src;
		cse=src+height;
		while(cs<cse)
		{
			*ct=*cs++;
			ct+=384;
		}
		return;
	}

	if(screen_planemask_ispair)
	{
		ct = VGA_GETASPTR(buf);
		cs=src;
		cse=src+height;
		while(cs<cse)
		{
			pix=*cs++;
			ct[0]=pix;
			ct[1]=pix;
			ct+=384;
		}
		return;
	}
#endif

//	int i, a;

	a=buf;
	for(i=0; i<height; i++)
	{
		VGAWRITE(a, src[i]);
		a+=96;
	}
}

void DrawPost (int height, char * column, unsigned buf)
{
	DrawFilmPost(buf, column, height);
}

void  DrawHeightPost (int height, byte * src, unsigned buf)
{
	DrawFilmPost(buf, src, height);
}

void R_DrawWallColumn (unsigned buf)
{
	int			count; 
	unsigned	dest;
	byte		*ct;
	byte		*cmap, *src;
	fixed_t		frac;
	fixed_t		fracstep;
	int			pix;

//	return; 
 
//	count = dc_yh - dc_yl; 
	count = (dc_yh - dc_yl) - 1;

	// Zero length, column does not exceed a pixel.
	if (count <= 0) 
		return; 

//	dest = ylookup[dc_yl] + columnofs[dc_x];  
	dest = buf + ylookup[dc_yl];

	ct = VGA_GETASPTR(dest);

	fracstep = dc_iscale; 
	frac = dc_texturemid + (dc_yl-centery)*fracstep;

	cmap = shadingtable;
	src = dc_source;

#if 1
	if(screen_planemask==15)
	{
		do 
		{
			pix = cmap[src[(frac>>FRACBITS)&63]];
			ct[0] = pix;	ct[1] = pix;
			ct[2] = pix;	ct[3] = pix;
			ct += 96*4;

			frac += fracstep;
		
		} while (count--); 
		return;
	}
#endif

#if 1
//	if((screen_planemask==0xC) || (screen_planemask==0x3))
	if(screen_planemask_ispair)
	{
		do 
		{
			pix = cmap[src[(frac>>FRACBITS)&63]];
			ct[0] = pix;	ct[1] = pix;
			ct += 96*4;
			frac += fracstep;
		} while (count--); 
		return;
	}
#endif

	do 
	{
//		pix = shadingtable[dc_source[(frac>>FRACBITS)&127]];
		pix = cmap[src[(frac>>FRACBITS)&63]];
//		VGAWRITE(dest, pix);
//		dest += 96;
		*ct = pix;
		ct += 96*4;

		frac += fracstep;
	
	} while (count--); 
}

void  DrawMenuPost (int height, byte * src, unsigned buf)
{
	DrawFilmPost(buf, src, height);
}

void  DrawMapPost (int height, byte * src, unsigned buf)
{
	DrawFilmPost(buf, src, height);
}

void DrawSkyPost (unsigned buf, byte * src, int height)
{
	DrawFilmPost(buf, src, height);
}

void DrawRow(int count, unsigned dest, byte * src)
{
	byte *ct;
	byte *cmap;
	fixed_t		xfrac, xstep, ystep;
	fixed_t		yfrac; 
	int			spot, pix; 

	if((count--)<=0)
		return;

	xfrac = mr_xfrac; 
	yfrac = mr_yfrac; 
	xstep = mr_xstep;
	ystep = mr_ystep;

	ct = VGA_GETASPTR(dest);
	cmap = shadingtable;

	if(screen_planemask==0xF)
	{
		do 
		{
			spot = ((yfrac>>(8-7))&(127*128)) + ((xfrac>>8)&127);
			pix = cmap[src[spot]];
			ct[0] = pix;
			ct[1] = pix;
			ct[2] = pix;
			ct[3] = pix;
			ct += 4;
			xfrac += xstep; 
			yfrac += ystep;
		} while (count--); 
		return;
	}

//	if((screen_planemask==0xC) || (screen_planemask==0x3))
	if(screen_planemask_ispair)
	{
		do 
		{
			spot = ((yfrac>>(8-7))&(127*128)) + ((xfrac>>8)&127);
			pix = cmap[src[spot]];
			ct[0] = pix;
			ct[1] = pix;
			ct += 4;
			xfrac += xstep; 
			yfrac += ystep;
		} while (count--); 
		return;
	}

	do 
	{
		spot = ((yfrac>>(8-7))&(127*128)) + ((xfrac>>8)&127);
		pix = cmap[src[spot]];
		*ct = pix;
		ct += 4;
		xfrac += xstep; 
		yfrac += ystep;
	} while (count--); 
}

void DrawRotRow(int count, unsigned dest, byte * src)
{
#if 0
	fixed_t		xfrac;
	fixed_t		yfrac; 
	int			spot, pix; 

	xfrac = mr_xfrac; 
	yfrac = mr_yfrac; 

	do 
	{
		if((xfrac>>(8+7)) | (yfrac>>(8+7)))
		{
			dest++;
			xfrac += mr_xstep; 
			yfrac += mr_ystep;
			continue;
		}
	
//		spot = ((yfrac>>(16-6))&(63*64)) + ((xfrac>>16)&63);
//		spot = ((yfrac>>(8-6))&(63*64)) + ((xfrac>>8)&63);
//		spot = ((yfrac>>(16-7))&(127*128)) + ((xfrac>>16)&127);
		spot = ((yfrac>>(8-7))&(127*128)) + ((xfrac>>8)&127);
		pix = shadingtable[src[spot]];
		VGAWRITE(dest++, pix);
		xfrac += mr_xstep; 
		yfrac += mr_ystep;
	
	} while (count--);
#endif
}

void DrawMaskedRotRow(int count, unsigned dest, byte * src)
{
//	DrawRow(count, dest, src);
	DrawRotRow(count, dest, src);
}

void R_DrawColumn (unsigned buf)
{
	int			count; 
	unsigned	dest; 
	fixed_t		frac;
	fixed_t		fracstep;
	byte		*ct;
	int			pix;
 
	count = dc_yh - dc_yl; 
//	count = (dc_yh - dc_yl) - 1;

	// Zero length, column does not exceed a pixel.
	if (count <= 0) 
		return; 

//	dest = ylookup[dc_yl] + columnofs[dc_x];  
	dest = buf + ylookup[dc_yl];

	ct = VGA_GETASPTR(dest);

	fracstep = dc_iscale; 
	frac = dc_texturemid + (dc_yl-centery)*fracstep; 

#if 1
	if(screen_planemask==15)
	{
		do 
		{
			pix = shadingtable[dc_source[(frac>>FRACBITS)&127]];
			ct[0] = pix;	ct[1] = pix;
			ct[2] = pix;	ct[3] = pix;
			ct += 96*4;

			frac += fracstep;
		
		} while (count--); 
		return;
	}

//	if((screen_planemask==0xC) || (screen_planemask==0x3))
	if(screen_planemask_ispair)
	{
		do 
		{
			pix = shadingtable[dc_source[(frac>>FRACBITS)&127]];
			ct[0] = pix;	ct[1] = pix;
			ct += 96*4;

			frac += fracstep;
		
		} while (count--); 
		return;
	}
#endif

//	if(screen_planemask&(screen_planemask-1))
	if(!screen_planemask_ispow2)
	{
		do 
		{
			pix = shadingtable[dc_source[(frac>>FRACBITS)&127]];
			VGAWRITE(dest, pix);
			dest += 96; 
			frac += fracstep;
		
		} while (count--); 
		return;
	}

	do 
	{
		pix = shadingtable[dc_source[(frac>>FRACBITS)&127]];
//		VGAWRITE(dest, pix);
//		dest += 96; 
		*ct = pix;
		ct += 96*4;
		frac += fracstep;
	
	} while (count--); 
}

void R_DrawSolidColumn (int color, unsigned buf)
{
	int			count; 
	unsigned	dest; 
 
	count = dc_yh - dc_yl; 
	if (count < 0) 
		return; 

	dest = buf + ylookup[dc_yl];

	do 
	{
		VGAWRITE(dest, color);
		dest += 96; 	
	} while (count--); 
}

void R_TransColumn (unsigned buf)
{
	byte		*ct;
	int			count; 
	unsigned	dest; 
	fixed_t		frac;
	fixed_t		fracstep;
	int			pix;
 
//	count = (dc_yh - dc_yl) - 1; 
	count = (dc_yh - dc_yl); 

//	return; 

	// Zero length, column does not exceed a pixel.
	if (count <= 0) 
		return; 

//	dest = ylookup[dc_yl] + columnofs[dc_x];  
	dest = buf + ylookup[dc_yl];

	fracstep = dc_iscale; 
	frac = dc_texturemid + (dc_yl-centery)*fracstep; 

	ct = VGA_GETASPTR(dest);
	if(screen_planemask_ispair)
	{
		do 
		{
			pix = ct[0];
			pix = shadingtable[pix];
			ct[0] = pix;	ct[1] = pix;
			ct += 96*4;
		} while (count--); 
		return;
	}

	if(screen_planemask_ispow2)
	{
		do 
		{
			pix = ct[0];
			pix = shadingtable[pix];
			ct[0] = pix;
			ct += 96*4;
		} while (count--); 
		return;
	}

	do 
	{
		pix = VGAREAD(dest);
		pix = shadingtable[pix];
//		pix = shadingtable[dc_source[(frac>>FRACBITS)&127]];
		VGAWRITE(dest, pix);
		dest += 96; 
//		frac += fracstep;
	
	} while (count--); 
}

void R_DrawClippedColumn (unsigned buf)
{
	int			count; 
	unsigned	dest; 
	byte		*ct;
	byte		*cmap;
	byte		*src;
	fixed_t		frac;
	fixed_t		fracstep;
	int			pix;
 
	count = dc_yh - dc_yl; 

	// Zero length, column does not exceed a pixel.
	if (count < 0) 
		return; 

//	dest = ylookup[dc_yl] + columnofs[dc_x];  
	dest = buf + ylookup[dc_yl];

	ct = VGA_GETASPTR(dest);
	cmap = shadingtable;
	src = dc_source;

	fracstep = dc_iscale; 
	frac = dc_texturemid + (dc_yl-centeryclipped)*fracstep; 

	do 
	{
		pix = cmap[src[(frac>>FRACBITS)&127]];
//		VGAWRITE(dest, pix);
//		dest += 96; 
		*ct = pix;
		ct += 4*96;
		frac += fracstep;
	
	} while (count--); 
}
