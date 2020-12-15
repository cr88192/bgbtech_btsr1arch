#include "h2def.h"

#include <time.h>

// #include "i_system.h"
// #include "i_sound.h"
// #include "i_video.h"
// #include "i_net.h"
// #include "v_video.h"

#include "gfxdrv_keys.h"

// #include "bgbmid/bgbmid.h"

typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

// int	mb_used = 6;

long long __smullq(int a, int b)
{
	return(((s64)a) * ((s64)b));
}

void I_InitNetwork (void)
{
    doomcom = malloc (sizeof (*doomcom) );
    memset (doomcom, 0, sizeof(*doomcom) );

	doomcom-> ticdup = 1;
	doomcom-> extratics = 0;

	netgame = false;
	doomcom->id = DOOMCOM_ID;
	doomcom->numplayers = doomcom->numnodes = 1;
	doomcom->deathmatch = false;
	doomcom->consoleplayer = 0;
	return;
}

void I_NetCmd (void)
{
}

void I_Error (char *str, ...)
{
	__debugbreak();
}


int	Key_Event (int c, int dn)
{
    event_t event;
    int c1;

	switch(c)
	{
	case	K_BACKSPACE: 	c1=KEY_BACKSPACE; break;
	case	K_UPARROW: 		c1=KEY_UPARROW; break;
	case	K_DOWNARROW:	c1=KEY_DOWNARROW; break;
	case	K_LEFTARROW: 	c1=KEY_LEFTARROW; break;
	case	K_RIGHTARROW: 	c1=KEY_RIGHTARROW; break;
	case	K_CTRL:			c1=KEY_RCTRL; break;
	case	K_ALT:			c1=KEY_RALT; break;
	case	K_SHIFT:		c1=KEY_RSHIFT; break;

	default:
		c1=c; break;
	}

	if(dn)
	{
		event.type = ev_keydown;
		event.data1 = c1;
		H2_PostEvent(&event);
		return(0);
	}

	event.type = ev_keyup;
	event.data1 = c1;
	H2_PostEvent(&event);
	return(0);
}


int vid_clamp255(int v)
{
	if(v<0)return(0);
	if(v>255)return(255);
	return(v);
}


#ifndef BASEWIDTH
#define BASEWIDTH SCREENWIDTH
#define BASEHEIGHT SCREENHEIGHT
#endif

unsigned short	d_8to16table[256];
unsigned int	d_8to24table[256];

unsigned int	d_16to24table[1<<16];

int vid_flashblend;

byte *btesh2_gfxcon_framebuf;
int btesh2_gfxcon_fb_dirty;
int btesh2_gfxcon_fbxs;
int btesh2_gfxcon_fbys;

void I_InitGraphics (void)
{
	static int init=0;
	
	if(init)
		return;
	init=1;

	GfxDrv_Start();
	
	btesh2_gfxcon_fbxs=BASEWIDTH;
	btesh2_gfxcon_fbys=BASEHEIGHT;

	GfxDrv_PrepareFramebuf();

	screen=malloc(BASEWIDTH*BASEHEIGHT*
		sizeof(dt_scrpix));
}

int VID_BlendEven16(int pixa, int pixb)
{
	int pix;
	pix=((pixa&0xFBDF)+(pixb&0xFBDF))>>1;
	return(pix);
}

u64 VID_BlendEven4x16(u64 pixa, u64 pixb)
{
	u64 pix;
	pix=((pixa&0xFBDEFBDEFBDEFBDEULL)>>1)+
		((pixb&0xFBDEFBDEFBDEFBDEULL)>>1);
	return(pix);
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
		pix1=VID_BlendEven16(pix, flash);
		pix1=VID_BlendEven16(pix, pix1);
		pix1=VID_BlendEven16(pix, pix1);
		break;
	case 2:
		pix1=VID_BlendEven16(pix, flash);
		pix1=VID_BlendEven16(pix, pix1);
		break;
	case 3:
		pix1=VID_BlendEven16(pix, flash);
		pix1=VID_BlendEven16(pix1, flash);
		pix1=VID_BlendEven16(pix, pix1);
		break;
	case 4:
		pix1=VID_BlendEven16(pix, flash);
		break;
	case 5:
		pix1=VID_BlendEven16(pix, flash);
		pix1=VID_BlendEven16(pix1, flash);
		break;
	case 6:
		pix1=VID_BlendEven16(pix, flash);
		pix1=VID_BlendEven16(pix1, flash);
		pix1=VID_BlendEven16(pix1, flash);
		break;
	case 7:
		pix1=flash&65535;
		break;
	}
	return(pix1);
}

u64 VID_BlendFlash4x(u64 pix, int flash)
{
	u64 pix1, fpix;
	
	fpix=(u16)flash;
	fpix|=(fpix<<16);
	fpix|=(fpix<<32);
	
	switch((flash>>16)&7)
	{
	case 0:
		pix1=pix;
		break;
	case 1:
		pix1=VID_BlendEven4x16(pix, fpix);
		pix1=VID_BlendEven4x16(pix, pix1);
		pix1=VID_BlendEven4x16(pix, pix1);
		break;
	case 2:
		pix1=VID_BlendEven4x16(pix, fpix);
		pix1=VID_BlendEven4x16(pix, pix1);
		break;
	case 3:
		pix1=VID_BlendEven4x16(pix, fpix);
		pix1=VID_BlendEven4x16(pix1, fpix);
		pix1=VID_BlendEven4x16(pix, pix1);
		break;
	case 4:
		pix1=VID_BlendEven4x16(pix, fpix);
		break;
	case 5:
		pix1=VID_BlendEven4x16(pix, fpix);
		pix1=VID_BlendEven4x16(pix1, fpix);
		break;
	case 6:
		pix1=VID_BlendEven4x16(pix, fpix);
		pix1=VID_BlendEven4x16(pix1, fpix);
		pix1=VID_BlendEven4x16(pix1, fpix);
		break;
	case 7:
		pix1=fpix;
		break;
	}
	return(pix1);
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
		Key_Event(j&4095, !(j&0x8000));
	}
}

void I_StartFrame (void)
{
	dt_scrpix *cs;
	u32 *ct;
	int x, y, p;

	I_HandleInput();

	GfxDrv_BeginDrawing();
	
//	R_ClearCZBuf();

//	for(y=0; y<BASEHEIGHT; y++)
	for(y=0; y<viewheight; y++)
	{
//		cs=screens[0]+(y*BASEWIDTH);
		cs=screen+((y+viewwindowy)*BASEWIDTH)+viewwindowx;
//		for(x=0; x<BASEWIDTH; x++)
		for(x=0; x<viewwidth; x++)
		{
//			*cs++=(16<<10)|(16<<5)|16;
//			*cs++=((((x^y)&1)?18:14)<<10)|(16<<5)|16;
//			*cs++=((((x^y)&4)?20:12)<<10)|(16<<5)|16;
			*cs++=((((x^y)&2)?18:14)<<10)|(16<<5)|16;
		}
	}
}

void I_UpdateNoBlit (void)
{
}

void I_ShutdownGraphics(void)
{
}

void I_SetPalette (byte *palette)
{
	int cr, cg, cb;
	int cy, cu, cv, cu1, cv1;
	int i, j, k;
	
	for(i=0; i<256; i++)
	{
		cr=palette[i*3+0];
		cg=palette[i*3+1];
		cb=palette[i*3+2];
#if 0
#ifdef CONGFX		
		cy=(2*cg+cr+cb)/4;
		cu=((cb-cg)/2)+128;
		cv=((cr-cg)/2)+128;
#else
		cy=((  77*cr +150*cg + 29*cb + 127)>>8);
		cu=((- 43*cr - 85*cg +128*cb + 127)>>8)+128;
		cv=(( 128*cr -107*cg - 21*cb + 127)>>8)+128;
#endif

		cy=vid_clamp255(cy+2)>>2;
		cu=vid_clamp255(cu+4)>>3;
		cv=vid_clamp255(cv+4)>>3;
		d_8to16table[i]=(cy<<10)|(cv<<5)|cu;
#endif

		d_8to16table[i]=
			((cr<<7)&0x7C00)|
			((cg<<2)&0x03E0)|
			((cb>>3)&0x001F);

//		d_8to24table[i]=0xFF000000|(cr<<16)|(cg<<8)|cb;
		d_8to24table[i]=0xFF000000|(cb<<16)|(cg<<8)|cr;
	}
	
//	d_8to16table[255]=0x7FFF;
//	d_8to16table[255]=0x00FF;

	for(i=0; i<65536; i++)
	{
#if 0
		cy=(i>>10)&63;	cy=(cy<<2)|(cy>>4);
		cv=(i>> 5)&31;	cv=(cv<<3);
		cu=(i    )&31;	cu=(cu<<3);
		
#if 1
		cu1=(cu-128)*2;
		cv1=(cv-128)*2;	
		cg=cy-((cu1+cv1)>>2);
		cb=cg+cu1;
		cr=cg+cv1;
#endif
#endif

		cr=(i>>10)&31;	cr=(cr<<3)|(cr>>2);
		cg=(i>> 5)&31;	cg=(cg<<3)|(cg>>2);
		cb=(i>> 0)&31;	cb=(cb<<3)|(cb>>2);

//		cr=cy;
//		cg=cy;
//		cb=cy;

		cr=vid_clamp255(cr);
		cg=vid_clamp255(cg);
		cb=vid_clamp255(cb);

		d_16to24table[i]=0xFF000000|(cb<<16)|(cg<<8)|cr;
	}

}

#if 0
#define I_PIX16_RED		0x43E0
#define I_PIX16_GREEN	0x8000
#define I_PIX16_PURP	0x83FF
#define I_PIX16_BLUE	0x401F
#define I_PIX16_YEL		0xC1E0
#endif

#if 1
#define I_PIX16_RED		0x7C00
#define I_PIX16_GREEN	0x03E0
#define I_PIX16_BLUE	0x001F
#define I_PIX16_PURP	0x7C1F
#define I_PIX16_YEL		0x7FE0
#endif

void I_SetPaletteIndex (int idx)
{
	switch(idx)
	{
	case 0:		vid_flashblend = 0;						break;
	case 1:		vid_flashblend = (1<<16)|I_PIX16_RED;	break;
	case 2:		vid_flashblend = (1<<16)|I_PIX16_RED;	break;
	case 3:		vid_flashblend = (2<<16)|I_PIX16_RED;	break;
	case 4:		vid_flashblend = (3<<16)|I_PIX16_RED;	break;
	case 5:		vid_flashblend = (4<<16)|I_PIX16_RED;	break;
	case 6:		vid_flashblend = (5<<16)|I_PIX16_RED;	break;
	case 7:		vid_flashblend = (6<<16)|I_PIX16_RED;	break;
	case 8:		vid_flashblend = (7<<16)|I_PIX16_RED;	break;

	case 9:		vid_flashblend = (1<<16)|I_PIX16_YEL;	break;
	case 10:	vid_flashblend = (1<<16)|I_PIX16_YEL;	break;
	case 11:	vid_flashblend = (2<<16)|I_PIX16_YEL;	break;
	case 12:	vid_flashblend = (2<<16)|I_PIX16_YEL;	break;

	case 13:	vid_flashblend = (1<<16)|I_PIX16_PURP;	break;
	case 14:	vid_flashblend = (1<<16)|I_PIX16_PURP;	break;
	case 15:	vid_flashblend = (2<<16)|I_PIX16_PURP;	break;
	case 16:	vid_flashblend = (3<<16)|I_PIX16_PURP;	break;
	case 17:	vid_flashblend = (4<<16)|I_PIX16_PURP;	break;
	case 18:	vid_flashblend = (5<<16)|I_PIX16_PURP;	break;
	case 19:	vid_flashblend = (6<<16)|I_PIX16_PURP;	break;
	case 20:	vid_flashblend = (7<<16)|I_PIX16_PURP;	break;

	case 21:	vid_flashblend = (2<<16)|I_PIX16_BLUE;	break;
	case 22:	vid_flashblend = (2<<16)|I_PIX16_YEL;	break;

	default:
		vid_flashblend = 0;
		break;
	}
}

void I_FinishUpdate (void)
{
	dt_scrpix *cs;
	u32 *ct;
	int x, y, p;

// byte *btesh2_gfxcon_framebuf;
// int btesh2_gfxcon_fb_dirty;

	for(y=0; y<BASEHEIGHT; y++)
	{
		cs=screen+(y*BASEWIDTH);
//		ct=((u16 *)vid_vram)+(y*BASEWIDTH);
		ct=((u32 *)btesh2_gfxcon_framebuf)+(y*BASEWIDTH);
		for(x=0; x<BASEWIDTH; x++)
		{
//			*ct++=d_8to16table[*cs++];
			p=*cs++;
			
//			if(p<0x100)
//			if(0)
//			if(1)
			if(sizeof(dt_scrpix)==1)
			{
				*ct++=d_8to24table[p];
			}else
			{
				if(vid_flashblend)
					{ p=VID_BlendFlash(p, vid_flashblend); }

				*ct++=d_16to24table[p];
			}
		}
	}

	btesh2_gfxcon_fb_dirty=1;
	
	GfxDrv_EndDrawing();
}

void I_Update (void)
{
	I_FinishUpdate();
	I_UpdateSound();
	I_SubmitSound();
}

void I_ReadScreen (dt_scrpix *scr)
{
    memcpy (scr, screen, SCREENWIDTH*SCREENHEIGHT*sizeof(dt_scrpix));
}

void I_StartTic (void)
{
}


int UpdateState;


void S_StartSongName(char *songLump, dt_bool loop)
{
}

#if 0
void S_StartSound(mobj_t *origin, int sound_id)
{
	S_StartSoundAtVolume(origin, sound_id, 127);
}

void S_StartSoundAtVolume(mobj_t *origin, int sound_id, int volume)
{
}
#endif

#if 0
void S_UpdateSounds(mobj_t *listener)
{
}
#endif

//
// I_GetTime
// returns time in 1/70th second tics
//
int  I_GetTime (void)
{
//    struct timeval	tp;
//    struct timezone	tzp;
    int			newtics;
    static int		basetime=0;
  
//    gettimeofday(&tp, &tzp);
//   if (!basetime)
//	basetime = tp.tv_sec;
//    newtics = (tp.tv_sec-basetime)*TICRATE + tp.tv_usec*TICRATE/1000000;

	if (!basetime)
		basetime = clock();
	newtics = clock() - basetime;
	newtics = newtics * (TICRATE/1000.0);

    return newtics;
}

int  I_GetTimeMs (void)
{
	return(FRGL_TimeMS());

#if 0
    int			newtics;
    static int		basetime=0;
  
	if (!basetime)
		basetime = clock();
	newtics = clock() - basetime;
	newtics = newtics * (TICRATE/1000.0);

    return newtics;
#endif
}

void I_WaitVBL(int vbls)
{
}

void I_Quit(void)
{
}

#if 0
void S_PauseSound(void)
{
}

void S_ResumeSound(void)
{
}

void S_StopSound(mobj_t *origin)
{
}

#endif

byte *I_AllocLow (int length)
{
	return(malloc(length));
}

byte *I_ZoneBase (int *size)
{
	void *ptr;
	int sz;
	
	sz=1<<28;
	ptr=malloc(sz);
	memset(ptr, 0, sz);
	*size=sz;
	
	return(ptr);
}

int DisplayTicker = 0;

void I_CheckExternDriver(void)
{
}

void I_Init (void)
{
	M_InitSoftDiv();
	I_InitGraphics();
	I_InitSound();

//	extern void I_StartupTimer(void);

//	novideo = M_CheckParm("novideo");
//	ST_Message("  I_StartupDPMI\n");
//	I_StartupDPMI();
//	ST_Message("  I_StartupMouse ");
//	I_StartupMouse();
//	tprintf("I_StartupJoystick ",1);
//	I_StartupJoystick();
//	tprintf("I_StartupKeyboard ",1);
//	I_StartupKeyboard();
//	ST_Message("  S_Init... ");
	S_Init();
	//IO_StartupTimer();
//	S_Start();
}
