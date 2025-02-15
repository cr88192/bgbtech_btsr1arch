#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "rt_def.h"
#include "rt_vid.h"
#include "rt_draw.h"
#include "rt_menu.h"
#include "rt_scale.h"
#include "rt_floor.h"
#include "rt_view.h"
#include "f_scale.h"
#include "isr.h"

#ifdef __BJX2__
#include <tkgdi/tkgdi.h>
#endif

#define MAXSCREENHEIGHT			200
#define MAXSCREENWIDTH			 320
#define SCREENBWIDE				 96
#define MAXVIEWWIDTH				320
// #define SCREENWIDTH				 96				  // default screen width in bytes

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


#ifdef __BJX2__

// TKGDI_BITMAPINFOHEADER i_t_dibinfo;
TKGDI_BITMAPINFOHEADER *i_dibinfo = NULL;
TKGDI_BITMAPINFOHEADER *i_dibinfo_ix = NULL;
TKGHDC i_hDc;

u16 *screen_fbrgb;

byte *screen_fbix;
u32 *screen_palix;

void I_InitTkGdi()
{
	if(i_dibinfo)
		return;
	
	printf("I_InitTkGdi\n");
	
//	i_dibinfo = &i_t_dibinfo;
	i_dibinfo = malloc(sizeof(TKGDI_BITMAPINFOHEADER));
	memset(i_dibinfo, 0, sizeof(TKGDI_BITMAPINFOHEADER));

	i_dibinfo_ix = malloc(sizeof(TKGDI_BITMAPINFOHEADER)+1024);
	memset(i_dibinfo_ix, 0, sizeof(TKGDI_BITMAPINFOHEADER));

	i_dibinfo->biSize=sizeof(TKGDI_BITMAPINFOHEADER);
	i_dibinfo->biWidth=320;
	i_dibinfo->biHeight=200;

//	i_dibinfo->biWidth=640;
//	i_dibinfo->biHeight=400;

//	i_dibinfo->biWidth=800;
//	i_dibinfo->biHeight=600;

	i_dibinfo->biBitCount=16;

//	tk_printf("  1\n", hDc);

	i_hDc=tkgCreateDisplay(i_dibinfo);
	tkgSetWindowTitle(i_hDc, "ROTT");

	i_dibinfo->biHeight=-200;
	
//	screen_fbuf=tkgTryMapFrameBuffer(i_hDc, i_dibinfo);

	screen_fbrgb=malloc(320*200*2);

#if 1
	i_dibinfo_ix->biSize=sizeof(TKGDI_BITMAPINFOHEADER);
	i_dibinfo_ix->biWidth=SCREENAWIDTH;
	i_dibinfo_ix->biHeight=-200;
	i_dibinfo_ix->biPlanes=1;
	i_dibinfo_ix->biBitCount=8;
	i_dibinfo_ix->biSizeImage=SCREENAWIDTH*200;
//	i_dibinfo_ix->biClrUsed=255;
	
	screen_palix=(u32 *)(((byte *)i_dibinfo_ix)+
		sizeof(TKGDI_BITMAPINFOHEADER));
//	screen_fbix=malloc(320*200);
#endif

	printf("I_InitTkGdi: OK\n");
}

#endif


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

#ifdef RT_USE_SDL
		cc=0xFF000000|(cr<<16)|(cg<<8)|cb;
#else
		cc=0xFF000000|(cb<<16)|(cg<<8)|cr;
#endif

		d_16to24table[i]=cc;
	}
}

static int i_lastframems;

#ifdef __BJX2__
int I_GetTimeMs (void)
{
	return(TK_GetTimeMs());
}
#endif

#ifdef _WIN32
int  I_GetTimeMs (void)
{
	return(FRGL_TimeMS());
}
#endif

#ifdef RT_USE_SDL
int  I_GetTimeMs (void)
{
	return(FRGL_TimeMS());
}
#endif


void I_DrawFramerate()
{
	char tb[16];
	int t0, dt, fps;
	
	t0=I_GetTimeMs();
	dt=t0-i_lastframems;
	i_lastframems=t0;
	if((dt<=0) || (dt>=1000))
		return;
	
	fps=1000/dt;

//	HU_DrawDecNum(320-12, 2, fps, 2);
//	DrSmallNumber(fps, 320-16, 2);
//	DrSmallNumber(fps, 320-12, 2);

#if 1
//	tb[0]='0'+((fps/100)%10);
//	tb[1]='0'+((fps/ 10)%10);
//	tb[2]='0'+((fps    )%10);
//	tb[3]=0;

	tb[0]='0'+((fps/ 10)%10);
	tb[1]='0'+((fps    )%10);
	tb[2]=0;
	
//	CurrentFont = smallfont;
	CurrentFont = tinyfont;
	
//	HU_DrawString(320-22, 2, tb);
	VW_DrawClippedString(320-14, 2+16, tb);
#endif
}


void GraphicsMode ( void )
{
	int i, j, k;

	if(!screenbuf)
	{

		printf("Debug A1 %s:%d\n", __FILE__, __LINE__);

#ifdef _WIN32
		GfxDrv_Start();

		btesh2_gfxcon_fbxs=320;
		btesh2_gfxcon_fbys=200;

		GfxDrv_PrepareFramebuf();
#endif

#ifdef RT_USE_SDL
//		btesh2_gfxcon_fbxs=320;
//		btesh2_gfxcon_fbys=200;

		btesh2_gfxcon_fbxs=640;
		btesh2_gfxcon_fbys=400;

		GfxDrv_Start();

		GfxDrv_PrepareFramebuf();
#endif

		printf("Debug A2 %s:%d\n", __FILE__, __LINE__);

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

		printf("Debug A3 %s:%d\n", __FILE__, __LINE__);

		I_StartFrame();

		printf("Debug A4 %s:%d\n", __FILE__, __LINE__);

		I_SetupScreenTables();

		printf("Debug A5 %s:%d\n", __FILE__, __LINE__);
	}

	for (i=0;i<MAXSCANLINES;i++)
	{
//		ylookup[i]=i*320;
		ylookup[i]=i*96;
	}

	printf("Debug A6 %s:%d\n", __FILE__, __LINE__);
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

#ifdef __BJX2__
void VGAWRITEBUF_CopyPow2(byte *cs, byte *ct, byte *cte);
__asm {
	VGAWRITEBUF_CopyPow2:

	SUB		R6, 32, R7

	CMPQGT	R5, R7
	BF		.L3
.L2:
	MOV.Q	(R4), R16
	SHLD.Q	R16,  -8, R17	|	MOV.B	R16, (R5,  0)
	SHLD.Q	R16, -16, R18	|	MOV.B	R17, (R5,  4)
	SHLD.Q	R16, -24, R19	|	MOV.B	R18, (R5,  8)
	SHLD.Q	R16, -32, R20	|	MOV.B	R19, (R5, 12)
	SHLD.Q	R16, -40, R21	|	MOV.B	R20, (R5, 16)
	SHLD.Q	R16, -48, R22	|	MOV.B	R21, (R5, 20)
	SHLD.Q	R16, -56, R23	|	MOV.B	R22, (R5, 24)
	ADD		 8, R4			|	MOV.B	R23, (R5, 28)
	ADD		32, R5
	CMPQGT	R5, R7
	BT		.L2
.L3:

	CMPQGT	R5, R6
	BF		.L5
.L4:
	MOVU.B	(R4), R3
	ADD		1, R4		|	MOV.B	R3, (R5)
	ADD		4, R5
	CMPQGT	R5, R6
	BT		.L4
.L5:
	RTSU
}
#endif

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
#ifdef __BJX2__
		VGAWRITEBUF_CopyPow2(cs, ct, cte);
#else
		ct1e=cte-16;
//		while((ct+16)<=cte)
		while(ct<=ct1e)
		{
			v=*(int *)cs;
			ct[ 0]=v    ;	ct[ 4]=v>> 8;
			ct[ 8]=v>>16;	ct[12]=v>>24;

//			ct[ 0]=cs[0];	ct[ 4]=cs[1];
//			ct[ 8]=cs[2];	ct[12]=cs[3];
			cs+=4;			ct+=16;
		}
		while(ct<cte)
		{
			v=*cs++;
			*ct=v;
			ct+=4;
		}
#endif
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
//		__debugbreak();
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
	
	cr=0; cg=0; cb=0;
	
	cs=pal;
	for(i=0; i<256; i++)
	{
		VL_GetColor(i, &cr, &cg, &cb);
		cs[0]=cr;	cs[1]=cg;	cs[2]=cb;
		cs+=3;
	}
}

// #ifdef _WIN32
#if defined(_WIN32) || defined(RT_USE_SDL)
u16 *FRGL_GetKeybuf();
void I_HandleInput (void)
{
	u16 *kb;
	int j;

#ifdef _WIN32
	GfxDrv_HandleInput();
#endif

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
//	int c, dn;

	TKGDI_EVENT t_imsg;
	TKGDI_EVENT *imsg;
	int i, j, c, n, dn;

//	printf("Debug B1 %s:%d\n", __FILE__, __LINE__);

	if(i_hDc>1)
	{
//		printf("Debug B2 %s:%d\n", __FILE__, __LINE__);

		thrd_yield();
		imsg=&t_imsg;

		n=64;
		while((n--)>0)
		{
			j=tkgPollEvent(i_hDc, imsg);
			if(j<1)
				break;
			if(imsg->fccMsg==0)
				break;
			if(imsg->fccMsg==TKGDI_FCC_keyb)
			{
				c=imsg->wParm1;
				dn=!(c&0x8000);
				c=c&0x7FFF;
			
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
				
				if(c>=256)
					continue;
				
				Key_Event (c, dn);
			}
		}
		
		return;
	}

//	printf("Debug B3 %s:%d\n", __FILE__, __LINE__);

	n=64;
//	while(*kb)
	while((tk_kbhit()>0) && ((n--)>0))
	{
//		printf("Debug B3-1 %s:%d\n", __FILE__, __LINE__);

		c=tk_getch();
		if(c<0)
			break;
	
//		printf("Debug B3-2 %s:%d\n", __FILE__, __LINE__);

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

//	printf("Debug B4 %s:%d\n", __FILE__, __LINE__);
}
#endif

// #ifdef _WIN32
#if defined(_WIN32) || defined(RT_USE_SDL)
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

// #ifdef _WIN32
#if defined(_WIN32) || defined(RT_USE_SDL)
void I_FinishUpdate (void)
{
	int x, y, c;

#if 0
	for(y=196; y<200; y++)
		for(x=0; x<256; x++)
	{
		screenbuf[(y-4)*SCREENAWIDTH+x]=(x/2);
		screenbuf[(y-0)*SCREENAWIDTH+x]=128+(x/2);
	}
#endif
	
	I_DrawFramerate();

	for(y=0; y<200; y++)
		for(x=0; x<320; x++)
	{
		c=screenbuf[y*SCREENAWIDTH+x];
		c=screen_pal[c];
		c=d_16to24table[c];
#ifdef RT_USE_SDL
		((u32 *)btesh2_gfxcon_framebuf)[(y*2+0)*640+(x*2+0)]=c;
		((u32 *)btesh2_gfxcon_framebuf)[(y*2+0)*640+(x*2+1)]=c;
		((u32 *)btesh2_gfxcon_framebuf)[(y*2+1)*640+(x*2+0)]=c;
		((u32 *)btesh2_gfxcon_framebuf)[(y*2+1)*640+(x*2+1)]=c;
#else
		((u32 *)btesh2_gfxcon_framebuf)[y*320+x]=c;
#endif
	}

	btesh2_gfxcon_fb_dirty=1;
	GfxDrv_EndDrawing();	
}
#endif

#ifdef __BJX2__

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
#if 0
	MOV		screen_pal, R5
	SHLD	R4, -24, R6
	SHLD	R4, -16, R7		|	MOVU.W	(R5, R6), R2
	EXTU.B	R7, R7			|	SHLD	R4, -8, R6
	SHLD.Q	R2, 16, R2		|	MOVU.W	(R5, R7), R3
	EXTU.B	R6, R6			|	OR		R3, R2
	SHLD.Q	R2, 16, R2		|	MOVU.W	(R5, R6), R3
	EXTU.B	R4, R7			|	OR		R3, R2
	SHLD.Q	R2, 16, R2		|	MOVU.W	(R5, R7), R3
	OR		R3, R2
	RTSU
#endif

#if 1
	MOV		screen_pal, R5
	SHLD	R4, -24, R6		|	SHLD	R4, -16, R7
	EXTU.B	R7, R7			|	MOVU.W	(R5, R6), R16
	SHLD	R4, -8, R6		|	MOVU.W	(R5, R7), R17
	EXTU.B	R6, R6			|	EXTU.B	R4, R7		
	SHLD.Q	R16, 16, R16	|	MOVU.W	(R5, R6), R18
	OR		R17, R16		|	MOVU.W	(R5, R7), R19
	
	SHLD.Q	R18, 16, R18
	OR		R18, R19, R17
	MOVLD	R16, R17, R2
	RTSU
#endif
};


void I_FinishUpdate (void)
{
	u32 *conbufa;
	u16 *pal;
	byte *ics, *icsb;
	u32 *ict;
	u64 *ictq;
	int bx, by, by2;
	int i, j, k;

//	u64 pxa, pxb, pxc, pxd;
	u32 ixa, ixb, ixc, ixd;
	register u64 pxa, pxb, pxc, pxd;

	__hint_use_egpr();

	I_InitTkGdi();

	I_DrawFramerate();

#if 0
//	conbufa=(u32 *)0xA00A0000;
	conbufa=(u32 *)0xFFFFF00A0000ULL;

	((u32 *)0xFFFFF00BFF00ULL)[8]=vid_frnum;
	vid_frnum++;

	((u32 *)0xFFFFF00BFF00ULL)[0]=0x0095;		//320x200x16bpp, RGB555
//	((u32 *)0xF00BFF00)[0]=0x0000;		//320x200, Color Cell
#endif

	ics=screenbuf;
//	ict=conbufa;
	pal=screen_pal;

	if(!ics)
		return;

#if 0
	ictq=(u64 *)screen_fbrgb;
	for(by=0; by<200; by++)
	{
		icsb=ics;
		for(bx=0; bx<80; bx++)
		{
			ixa=*((u32 *)icsb);
			pxa=I_FinishUpdate_Repack8to16(ixa);
			*ictq++=pxa;
			icsb+=4;
		}
		ics+=SCREENAWIDTH;
	}
	tkgBlitImage(i_hDc, 0, 0, i_dibinfo, screen_fbrgb);
#endif

#if 1
	for(i=0; i<256; i++)
	{
		j=screen_pal[i];
		k=	0xFF000000U |
			((j&0x7C00)<<9) |
			((j&0x03F0)<<6) |
			((j&0x001F)<<3) ;
		screen_palix[i]=k;
	}

#if 0
	ict=(u32 *)screen_fbix;
	for(by=0; by<200; by++)
	{
		memcpy(ict, ics, 320);
		ics+=SCREENAWIDTH;
		ict+=80;
	}
	tkgBlitImage(i_hDc, 0, 0, i_dibinfo_ix, screen_fbix);
#endif

	tkgBlitImage(i_hDc, 0, 0, i_dibinfo_ix, screenbuf);

#endif

#if 0
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
#endif
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
	FX_Update();
	I_FinishUpdate();
	I_StartFrame();
}

void FlipPage ( void )
{
	FX_Update();
	I_FinishUpdate();
	I_StartFrame();
}

void TurnOffTextCursor ( void )
{
}

void VH_UpdateScreen (void)
{
	FX_Update();
	I_FinishUpdate();
	I_StartFrame();
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

int I_TimeMS(void)
{
	int ms, tic, dt;

#ifdef _WIN32
	ms=FRGL_TimeMS();
#endif

#ifdef _BGBCC
	ms=TK_GetTimeMs();
#endif

#ifdef RT_USE_SDL
	ms=FRGL_TimeMS();
#endif

	return(ms);
}

long long __int32_dmuls(int a, int b);

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

#ifdef RT_USE_SDL
	ms=FRGL_TimeMS();
#endif

#ifdef _BGBCC
	tic=__int32_dmuls(ms, 35)>>10;
#else
	tic=ms*(35.0/1000.0);
#endif
	dt=tic-ltic;
	ltic=tic;
	
	if(dt<0)
		dt=0;
	
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

#ifdef __BJX2__
void R_DrawFilmPost_A1 (byte *ct, int count, byte *src);
void R_DrawFilmPost_A2 (byte *ct, int count, byte *src);

__asm {
R_DrawFilmPost_A1:
	CMPGT	0, R5
//	CMPGT	-1, R5
	BF .L3
	.L2:
	ADD		-1, R5		|	MOV.B	(R6), R16
	ADD		1, R6		|	MOV.B	R16, (R4)
	ADD		384, R4		|	CMPGT	0, R5
	BT .L2
	.L3:
	RTSU

R_DrawFilmPost_A2:
	CMPGT	0, R5
//	CMPGT	-1, R5
	BF .L3
	.L2:
	ADD		-1, R5		|	MOV.B		(R6), R16
	ADD		1, R6		|	PSHUF.B		R16, 0, R17
							MOV.W		R17, (R4)
	ADD		384, R4		|	CMPGT		0, R5
	BT 		.L2
	.L3:
	RTSU
}
#endif

void DrawFilmPost (unsigned buf, byte * src, int height)
{
#if 1
	byte *cs, *ct, *cse;
	int i, a, pix;

//	if(!(screen_planemask&(screen_planemask-1)))
	if(screen_planemask_ispow2)
	{
#ifdef __BJX2__
		ct = VGA_GETASPTR(buf);
		R_DrawFilmPost_A1 (ct, height, src);
		return;
#else
		ct = VGA_GETASPTR(buf);
		cs=src;
		cse=src+height;
		while(cs<cse)
		{
			*ct=*cs++;
			ct+=384;
		}
		return;
#endif
	}

	if(screen_planemask_ispair)
	{
#ifdef __BJX2__
		ct = VGA_GETASPTR(buf);
		R_DrawFilmPost_A2 (ct, height, src);
		return;
#else
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
#endif
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

#ifdef __BJX2__
void R_DrawWallColumn_A1 (byte *ct, int count,
	byte *src, byte *cmap, fixed_t frac, fixed_t fracstep);
void R_DrawWallColumn_A2 (byte *ct, int count,
	byte *src, byte *cmap, fixed_t frac, fixed_t fracstep);

__asm {
R_DrawWallColumn_A1:
	CMPGT	1, R5
	BF .L1
	.L0:
	ADD		R20, R21, R23	|	SHAD	R20, -16, R18
	AND		R18, 63, R18	|	SHAD	R23, -16, R22
	ADD		R23, R21, R20	|	MOV.B	(R6, R18), R19
	AND		R22, 63, R22	|	MOV.B	(R7, R19), R16
								MOV.B	(R6, R22), R23
								MOV.B	(R7, R23), R17
	ADD		R4, 384, R3		|	MOV.B	R16, (R4)
	ADD		R3, 384, R4		|	MOV.B	R17, (R3)
//								CMPGT	1, R5
								CMPGT	2, R5
	ADD		-2, R5
	BT .L0
	.L1:

//	CMPGT	0, R5
	CMPGT	-1, R5
	BF .L3
	.L2:
	SHAD	R20, -16, R22
	AND		R22, 63, R22
	MOV.B	(R6, R22), R23
	MOV.B	(R7, R23), R16
	MOV.B	R16, (R4)
	ADD		384, R4
	ADD		R21, R20	|	CMPGT	0, R5
	ADD		-1, R5
	BT .L2
	.L3:

	RTSU

R_DrawWallColumn_A2:

#if 1
	CMPGT	1, R5
	BF .L1
	.L0:
	ADD		R20, R21, R23	|	SHAD	R20, -16, R18
	AND		R18, 63, R18	|	SHAD	R23, -16, R22
	ADD		R23, R21, R20	|	MOV.B	(R6, R18), R19
	AND		R22, 63, R22	|	MOV.B	(R7, R19), R16
								MOV.B	(R6, R22), R23
	PSHUF.B	R16, 0, R16		|	MOV.B	(R7, R23), R17
	PSHUF.B	R17, 0, R17		|	CMPGT	1, R5
	ADD		R4, 384, R3		|	MOV.W	R16, (R4)
	ADD		R3, 384, R4		|	MOV.W	R17, (R3)
	ADD		-2, R5
	BT .L0
	.L1:
#endif

//	CMPGT	0, R5
	CMPGT	-1, R5
	BF .L3
	.L2:
	SHAD	R20, -16, R22
	AND		R22, 63, R22
	MOV.B	(R6, R22), R23
	MOVU.B	(R7, R23), R16
	PSHUF.B	R16, 0, R17
	MOV.W	R17, (R4)
	ADD		384, R4
	ADD		R21, R20
	CMPGT	0, R5
	ADD		-1, R5
	BT .L2
	.L3:
	RTSU
}
#endif

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
#ifdef __BJX2__
		R_DrawWallColumn_A2 (ct, count, src, cmap, frac, fracstep);
		return;
#else
		do 
		{
			pix = cmap[src[(frac>>FRACBITS)&63]];
			ct[0] = pix;	ct[1] = pix;
			ct += 96*4;
			frac += fracstep;
		} while (count--);
		return;
#endif
	}
#endif

#ifdef __BJX2__
	R_DrawWallColumn_A1 (ct, count, src, cmap, frac, fracstep);
#else
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
#endif
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

#ifdef __BJX2__
void R_DrawRow_A1 (byte *ct, int count,
	byte *src, byte *cmap, fixed_t frac, fixed_t fracstep);
void R_DrawRow_A2 (byte *ct, int count,
	byte *src, byte *cmap, fixed_t frac, fixed_t fracstep);

__asm {
R_DrawRow_A1:
	MOV		0x3F80, R1
//	CMPGT	0, R5
	CMPGT	-1, R5
	BF .L3
	.L2:
	SHAD	R20, -8, R22	|	SHAD	R20, -17, R23
	AND		R22, 127, R22	|	AND		R23, R1, R23
	OR		R22, R23, R22
	MOV.B	(R6, R22), R23
	MOVU.B	(R7, R23), R17
	MOV.B	R17, (R4)
	ADD		4, R4
	ADD		R21, R20	|	CMPGT	0, R5
	ADD		-1, R5
	BT .L2
	.L3:
	RTSU

R_DrawRow_A2:
#if 1
	MOV		0x3F80, R1
	CMPGT	1, R5
	BF .L1
	.L0:
	ADD		R20, R21, R17	|	SHAD	R20, -8, R18
	SHAD	R20, -17, R19	|	SHAD	R17, -8, R22
	SHAD	R17, -17, R23	|	AND		R18, 127, R18
	AND		R19, R1, R19	|	AND		R22, 127, R22
	AND		R23, R1, R23	|	OR		R18, R19, R18
	OR		R22, R23, R22	|	MOV.B	(R6, R18), R19
	ADD		R17, R21, R20	|	MOV.B	(R6, R22), R23
								MOVU.B	(R7, R19), R16
	PSHUF.B	R16, 0, R16		|	MOVU.B	(R7, R23), R17
	PSHUF.B	R17, 0, R17		|	ADD		R5, -2, R5
	ADD		R4, 4, R3		|	MOV.W	R16, (R4)
	ADD		R3, 4, R4		|	MOV.W	R17, (R3)

	CMPGT	1, R5
	BT .L0
	.L1:
#endif

	MOV		0x3F80, R1
//	CMPGT	0, R5
	CMPGT	-1, R5
	BF .L3
	.L2:
	SHAD	R20, -8, R22	|	SHAD	R20, -17, R23
	AND		R22, 127, R22	|	AND		R23, R1, R23
	OR		R22, R23, R22
	MOV.B	(R6, R22), R23
	MOVU.B	(R7, R23), R17
	PSHUF.B	R17, 0, R17
	MOV.W	R17, (R4)
	ADD		4, R4
	ADD		R21, R20	|	CMPGT	0, R5
	ADD		-1, R5
	BT .L2
	.L3:
	RTSU
}
#endif

void DrawRow(int count, unsigned dest, byte * src)
{
	byte *ct;
	byte *cmap;
	fixed_t		xfrac, xstep, ystep;
	fixed_t		yfrac;
	u32			frac, fracstep;
	int			spot, pix; 

	if((count--)<=0)
		return;

	xfrac = mr_xfrac; 
	yfrac = mr_yfrac; 
	xstep = mr_xstep;
	ystep = mr_ystep;
	
	frac		= (yfrac<<16)+(xfrac&0x7FFF);
	fracstep	= (ystep<<16)+(xstep&0x7FFF);

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
#ifdef __BJX2__
		R_DrawRow_A2(ct, count, src, cmap, frac, fracstep);
		return;
#else
		do 
		{
//			spot = ((yfrac>>(8-7))&(127*128)) + ((xfrac>>8)&127);
			spot = ((frac>>(24-7))&(127*128)) + ((frac>>8)&127);
			pix = cmap[src[spot]];
			ct[0] = pix;
			ct[1] = pix;
			ct += 4;
//			xfrac += xstep; 
//			yfrac += ystep;
			frac += fracstep; 
		} while (count--); 
		return;
#endif
	}

#ifdef __BJX2__
		R_DrawRow_A2(ct, count, src, cmap, frac, fracstep);
		return;
#else
	do 
	{
//		spot = ((yfrac>>(8-7))&(127*128)) + ((xfrac>>8)&127);
		spot = ((frac>>(24-7))&(127*128)) + ((frac>>8)&127);
		pix = cmap[src[spot]];
		*ct = pix;
		ct += 4;
//		xfrac += xstep; 
//		yfrac += ystep;
		frac += fracstep; 
	} while (count--);
#endif 
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

#ifdef __BJX2__
void R_DrawColumn_A1 (byte *ct, int count,
	byte *src, byte *cmap, fixed_t frac, fixed_t fracstep);
void R_DrawColumn_A2 (byte *ct, int count,
	byte *src, byte *cmap, fixed_t frac, fixed_t fracstep);

__asm {
R_DrawColumn_A1:
	CMPGT	1, R5
	BF .L1
	.L0:
	ADD		R20, R21, R23	|	SHAD	R20, -16, R18
	AND		R18, 127, R18	|	SHAD	R23, -16, R22
	ADD		R23, R21, R20	|	MOV.B	(R6, R18), R19
	AND		R22, 127, R22	|	MOV.B	(R7, R19), R16
								MOV.B	(R6, R22), R23
								MOV.B	(R7, R23), R17
	ADD		R4, 384, R3		|	MOV.B	R16, (R4)
	ADD		R3, 384, R4		|	MOV.B	R17, (R3)
//								CMPGT	1, R5
								CMPGT	2, R5
	ADD		-2, R5
	BT .L0
	.L1:

//	CMPGT	0, R5
	CMPGT	-1, R5
	BF .L3
	.L2:
	SHAD	R20, -16, R22
	AND		R22, 127, R22
	MOV.B	(R6, R22), R23
	MOV.B	(R7, R23), R16
	MOV.B	R16, (R4)
	ADD		384, R4
	ADD		R21, R20	|	CMPGT	0, R5
	ADD		-1, R5
	BT .L2
	.L3:

	RTSU

R_DrawColumn_A2:

#if 1
	CMPGT	1, R5
	BF .L1
	.L0:
	ADD		R20, R21, R23	|	SHAD	R20, -16, R18
	AND		R18, 127, R18	|	SHAD	R23, -16, R22
	ADD		R23, R21, R20	|	MOV.B	(R6, R18), R19
	AND		R22, 127, R22	|	MOV.B	(R7, R19), R16
								MOV.B	(R6, R22), R23
	PSHUF.B	R16, 0, R16		|	MOV.B	(R7, R23), R17
	PSHUF.B	R17, 0, R17		|	CMPGT	1, R5
	ADD		R4, 384, R3		|	MOV.W	R16, (R4)
	ADD		R3, 384, R4		|	MOV.W	R17, (R3)
	ADD		-2, R5
	BT .L0
	.L1:
#endif

//	CMPGT	0, R5
	CMPGT	-1, R5
	BF .L3
	.L2:
	SHAD	R20, -16, R22
	AND		R22, 127, R22
	MOV.B	(R6, R22), R23
	MOVU.B	(R7, R23), R16
	PSHUF.B	R16, 0, R17
	MOV.W	R17, (R4)
	ADD		384, R4
	ADD		R21, R20
	CMPGT	0, R5
	ADD		-1, R5
	BT .L2
	.L3:
	RTSU
}
#endif

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
#ifdef __BJX2__
// #if 0
		R_DrawColumn_A2 (ct, count-1, dc_source, shadingtable, frac, fracstep);
		return;
#else
		do 
		{
			pix = shadingtable[dc_source[(frac>>FRACBITS)&127]];
			ct[0] = pix;	ct[1] = pix;
			ct += 96*4;

			frac += fracstep;
		
		} while (count--); 
		return;
#endif
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

#ifdef __BJX2__
// #if 0
	R_DrawColumn_A1 (ct, count-1, dc_source, shadingtable, frac, fracstep);
	return;
#else
	do 
	{
		pix = shadingtable[dc_source[(frac>>FRACBITS)&127]];
//		VGAWRITE(dest, pix);
//		dest += 96; 
		*ct = pix;
		ct += 96*4;
		frac += fracstep;
	} while (count--); 
	return;
#endif
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

#ifdef __BJX2__
	R_DrawColumn_A1 (ct, count, src, cmap, frac, fracstep);
	return;
#else
	do 
	{
		pix = cmap[src[(frac>>FRACBITS)&127]];
//		VGAWRITE(dest, pix);
//		dest += 96; 
		*ct = pix;
		ct += 4*96;
		frac += fracstep;
	
	} while (count--); 
#endif
}
