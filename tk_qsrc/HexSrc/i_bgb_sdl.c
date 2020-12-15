#include "h2def.h"

// #include "i_system.h"
// #include "i_sound.h"
// #include "i_video.h"
// #include "i_net.h"
// #include "v_video.h"

#include "r_local.h"

#include "gfxdrv_keys.h"

// #include "bgbmid/bgbmid.h"

// typedef unsigned short u16;
// typedef signed short s16;
// typedef unsigned int u32;
// typedef signed int s32;

// int	mb_used = 6;

int		UpdateState;
int		DisplayTicker;

void I_FinishUpdate (void);

int FRGL_TimeMS();

void GfxDrv_Start();
int GfxDrv_PrepareFramebuf();
void GfxDrv_BeginDrawing();
void GfxDrv_EndDrawing();

void I_SubmitSound2(int extra);
void I_SubmitSound(void);
void I_UpdateSound(void);


void I_Update(void)
{
	I_FinishUpdate();
	I_UpdateSound();
	I_SubmitSound();
}


long long __smullq(int a, int b)
{
	return(((s64)a) * ((s64)b));
}

s64 __int32_dmuls(s32 a, s32 b)
{
	return(((s64)a) * ((s64)b));
}

u64 __int32_dmulu(u32 a, u32 b)
{
	return(((u64)a) * ((u64)b));
}

#ifdef __linux
//void __debugbreak()
//{
//}
#endif

void I_Error(char *str, ...)
{
	DT_BREAKPOINT
//	__debugbreak();
}

#if 1
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
#endif

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
int btesh2_gfxcon_swaprb;

extern lighttable_t	*colormaps;


void I_InitGraphics (void)
{
	static int gfx_isinit=0;

	if(gfx_isinit)
		return;
	gfx_isinit=1;

	printf("I_InitGraphics: SDL\n");

	btesh2_gfxcon_fbxs=BASEWIDTH * 2;
	btesh2_gfxcon_fbys=BASEHEIGHT * 2;

//	printf("I_InitGraphics: SDL\n");

//	btesh2_gfxcon_fbxs=BASEWIDTH;
//	btesh2_gfxcon_fbys=BASEHEIGHT;

	GfxDrv_Start();
	
//	btesh2_gfxcon_fbxs=BASEWIDTH;
//	btesh2_gfxcon_fbys=BASEHEIGHT;

	GfxDrv_PrepareFramebuf();

	screen=malloc(BASEWIDTH*BASEHEIGHT*
		sizeof(dt_scrpix));
	
	if(colormaps)
	{
		I_SetPalette(W_CacheLumpName("PLAYPAL", PU_CACHE));
	}
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
//		cs=screens[0]+((y+viewwindowy)*BASEWIDTH)+viewwindowx;
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
		
#ifdef CONGFX		
		cy=(2*cg+cr+cb)/4;
		cu=((cb-cg)/2)+128;
		cv=((cr-cg)/2)+128;
#else
		cy=((  77*cr +150*cg + 29*cb + 127)>>8);
		cu=((- 43*cr - 85*cg +128*cb + 127)>>8)+128;
		cv=(( 128*cr -107*cg - 21*cb + 127)>>8)+128;
#endif

//		cy=vid_clamp255(cy+2)>>2;
//		cu=vid_clamp255(cu+4)>>3;
//		cv=vid_clamp255(cv+4)>>3;
//		d_8to16table[i]=(cy<<10)|(cv<<5)|cu;

		d_8to16table[i]=((cr>>3)<<10)|((cg>>3)<<5)|(cb>>3);

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

		cr=(i>>10)&31;	cg=(i>> 5)&31;	cb=(i>> 0)&31;
		cr=(cr<<3)|(cr>>2);
		cg=(cg<<3)|(cg>>2);
		cb=(cb<<3)|(cb>>2);

//		cr=cy;
//		cg=cy;
//		cb=cy;

		cr=vid_clamp255(cr);
		cg=vid_clamp255(cg);
		cb=vid_clamp255(cb);

		if(btesh2_gfxcon_swaprb)
		{
			k=cr; cr=cb; cb=k;
		}
		
		d_16to24table[i]=0xFF000000|(cb<<16)|(cg<<8)|cr;
	}

}

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
	case 10:	vid_flashblend = (2<<16)|I_PIX16_YEL;	break;
	case 11:	vid_flashblend = (3<<16)|I_PIX16_YEL;	break;
	case 12:	vid_flashblend = (4<<16)|I_PIX16_YEL;	break;

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

int R_PixelBlend(int pixa, int pixb);

#if 0
int R_PixelBlend(int pixa, int pixb)
{
	return(((pixa&0x7BDE)+(pixb&0x7BDE))>>1);
}
#endif

int VID_BlendEven16(int pixa, int pixb)
{
	int pix;
	pix=((pixa&0xFBDF)+(pixb&0xFBDF))>>1;
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

static int i_lastframems;

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
	DrSmallNumber(fps, 320-12, 2);
}

void I_FinishUpdate (void)
{
	dt_scrpix *cs;
//	u32 *ct;
	u32 *ct0, *ct1;
	int x, y, p;

// byte *btesh2_gfxcon_framebuf;
// int btesh2_gfxcon_fb_dirty;

	if(!btesh2_gfxcon_framebuf)
		return;

	I_DrawFramerate();

	for(y=0; y<BASEHEIGHT; y++)
	{
//		cs=screens[0]+(y*BASEWIDTH);
		cs=screen+(y*BASEWIDTH);
//		ct=((u16 *)vid_vram)+(y*BASEWIDTH);
//		ct=((u32 *)btesh2_gfxcon_framebuf)+(y*BASEWIDTH);
		ct0=((u32 *)btesh2_gfxcon_framebuf)+((y*2+0)*(2*BASEWIDTH));
		ct1=((u32 *)btesh2_gfxcon_framebuf)+((y*2+1)*(2*BASEWIDTH));
		for(x=0; x<BASEWIDTH; x++)
		{
//			*ct++=d_8to16table[*cs++];
			p=*cs++;

			if(vid_flashblend)
			{
				p=VID_BlendFlash(p, vid_flashblend);
			}

//			if(p<0x100)
			if(0)
			{
//				*ct++=d_8to24table[p];
				*ct0++=d_8to24table[p];
				*ct0++=d_8to24table[p];
				*ct1++=d_8to24table[p];
				*ct1++=d_8to24table[p];
			}else
			{
//				*ct++=d_16to24table[p];
				*ct0++=d_16to24table[p];
				*ct0++=d_16to24table[p];
				*ct1++=d_16to24table[p];
				*ct1++=d_16to24table[p];
			}
		}
	}

	btesh2_gfxcon_fb_dirty=1;
	GfxDrv_EndDrawing();
}

void I_ReadScreen (dt_scrpix *scr)
{
//    memcpy (scr, screens[0], SCREENWIDTH*SCREENHEIGHT*sizeof(dt_scrpix));
    memcpy (scr, screen, SCREENWIDTH*SCREENHEIGHT*sizeof(dt_scrpix));
}

void I_StartTic (void)
{
}


// int	mb_used = 6;
int	mb_used = 12;


byte*	I_AllocLow(int length)
{
    byte*	mem;
        
    mem = (byte *)malloc (length);
    memset (mem,0,length);
    return mem;
}

int  I_GetHeapSize (void)
{
    return mb_used*1024*1024;
}

byte* I_ZoneBase (int*	size)
{
	byte *ptr;
	int sz;
	sz = mb_used*1024*1024;
    ptr = (byte *) malloc (sz);
    memset(ptr, 0, sz);
    *size = sz;
    return ptr;
}


int  I_GetTime (void)
{
    int			newtics;
    static int		basetime=0;
  
	if (!basetime)
		basetime = FRGL_TimeMS();
	newtics = FRGL_TimeMS() - basetime;
	newtics = newtics * (TICRATE/1000.0);

    return newtics;
}

int  I_GetTimeMs (void)
{
	return(FRGL_TimeMS());
}

void I_WaitVBL(int vbls)
{
}

void I_Quit(void)
{
	exit(0);
}

void I_CheckExternDriver(void)
{
}

void I_Init (void)
{
	M_InitSoftDiv();
	I_InitGraphics();
//	I_InitSound();

	S_Init();
}

#if 0
void S_StartSongName(char *songLump, dt_bool loop)
{
}

void S_StartSong(int song, dt_bool loop)
{
}
#endif

void I_BeginRead (void)
{
}

void I_EndRead (void)
{
}

#if 0
void S_SetMaxVolume (dt_bool fullproc)
{
}

void S_GetChannelInfo (SoundInfo_t *s)
{
}
#endif


#if 1
byte *i_mus_songs[32];
int i_mus_song_mask;

//
// MUSIC API.
// Still no music done.
// Remains. Dummies.
//

void I_InitMusic(void)	
{
}

void I_ShutdownMusic(void)
{
}

static int	looping=0;
static int	musicdies=-1;

void I_PlaySong(int handle, int looping)
{
	// UNUSED.
//	handle = looping = 0;
//	musicdies = gametic + TICRATE*30;
}

#if 0
void I_PauseSong (int handle)
{
	// UNUSED.
	handle = 0;
}

void I_ResumeSong (int handle)
{
	// UNUSED.
	handle = 0;
}
#endif

void I_StopSong(int handle)
{
	// UNUSED.
	handle = 0;
	
	looping = 0;
	musicdies = 0;
}

void I_UnRegisterSong(int handle)
{
	i_mus_song_mask&=~(1<<handle);
}

int I_RegisterSong(void* data)
{
	int i;
	
	for(i=1; i<32; i++)
		if(!(i_mus_song_mask&(1<<i)))
			break;
	
	i_mus_songs[i]=data;
	i_mus_song_mask|=(1<<i);
	return(i);

//	data = NULL;
//	return 1;
}

// Is the song playing?
int I_QrySongPlaying(int handle)
{
	// UNUSED.
	handle = 0;
	return looping || musicdies > gametic;
}
#endif
