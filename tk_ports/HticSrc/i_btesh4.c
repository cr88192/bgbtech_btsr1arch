#include "doomdef.h"
#include "gfxdrv_keys.h"

// #include "i_system.h"
// #include "i_sound.h"
// #include "i_video.h"
// #include "i_net.h"
// #include "v_video.h"

#include <stdarg.h>

#ifndef BASEWIDTH
#define BASEWIDTH SCREENWIDTH
#define BASEHEIGHT SCREENHEIGHT
#endif

typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

// int	mb_used = 12;
// int	mb_used = 16;
int		mb_used = 20;


void S_StartSong(int song, boolean loop)
{
	S_ChangeMusic(song, loop);
}

void I_BeginRead (void)
{
}

void I_EndRead (void)
{
}

void S_SetMaxVolume (boolean fullproc)
{
}

void S_GetChannelInfo (SoundInfo_t *s)
{
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


void I_Update(void)
{
	I_FinishUpdate();
//	I_UpdateSound();
//	I_SubmitSound();
	D_UpdateSound();
}

#if 0
void I_InitSound()
{
}

void I_UpdateSound(void)
{
}

void I_SubmitSound(void)
{
}

void I_ShutdownSound(void)
{
}


void I_SetChannels()
{
}

int I_GetSfxLumpNum (sfxinfo_t* sfxinfo )
{
}


int
I_StartSound
( int		id,
  int		vol,
  int		sep,
  int		pitch,
  int		priority )
{
}

void I_StopSound(int handle)
{
}

int I_SoundIsPlaying(int handle)
{
}

void I_UpdateSoundParams
( int		handle,
  int		vol,
  int		sep,
  int		pitch )
{
}
#endif


#if 0
void I_InitMusic(void)
{
}

void I_ShutdownMusic(void)
{
}
#endif

#if 0
void I_SetMusicVolume(int volume)
{
}
#endif

#if 0
void I_PauseSong(int handle)
{
}

void I_ResumeSong(int handle)
{
}

int I_RegisterSong(void *data)
{
}

void I_PlaySong
( int		handle,
  int		looping )
{
}

void I_StopSong(int handle)
{
}

void I_UnRegisterSong(int handle)
{
}
#endif


void I_Init (void)
{
	M_InitSoftDiv();
    I_InitSound();
	S_Init();
	I_InitMusic();
}

int  I_GetHeapSize (void)
{
    return mb_used*1024*1024;
}

byte* I_ZoneBase (int*	size)
{
	byte *ptr;
    *size = mb_used*1024*1024;
    ptr = (byte *) malloc (*size);
    TKMM_Malloc(1);
    return ptr;
}

unsigned int TK_GetTimeMs(void);

int I_GetTime (void)
{
	int t;
	t = TK_GetTimeMs();
//	t=t>>5;
//	t=t*(TICRATE/1000.0);
	t=(t*35LL)>>10;
	return(t);
}

int I_GetTimeMs (void)
{
	return(TK_GetTimeMs());
}

void I_StartFrame (void)
{
#if 0
	static int frnum=0;

	dt_scrpix *cs;
	u32 *ct;
	int px1, px2, width;
	u64 px3;
	int x, y, p, frn;
	
	frnum = (frnum+1)&7;
	frn = frnum^(frnum>>1);
	width = scaledviewwidth / 16;

	for(y=0; y<viewheight; y++)
	{
//		if((y&7)!=frnum)
		if((y&7)!=frn)
			continue;
	
		if(y&2)
		{
			px1=(18<<10)|(16<<5)|16;
			px2=(14<<10)|(16<<5)|16;
		}else
		{
			px1=(14<<10)|(16<<5)|16;
			px2=(18<<10)|(16<<5)|16;
		}
		
		px1|=(px1<<16);
		px2|=(px2<<16);
		px3=((u32)px1)|(((u64)((u32)px2))<<32);

		cs=screen+((y+viewwindowy)*BASEWIDTH)+viewwindowx;
//		for(x=0; x<viewwidth; x++)
//		for(x=0; x<(BASEWIDTH/4); x++)
//		for(x=0; x<(BASEWIDTH/16); x++)
		for(x=0; x<width; x++)
		{
			((u64 *)cs)[0]=px3;
			((u64 *)cs)[1]=px3;
			((u64 *)cs)[2]=px3;
			((u64 *)cs)[3]=px3;
			cs+=16;
		}
	}
#endif
}

void I_StartTic (void)
{
}

ticcmd_t	emptycmd;
ticcmd_t*	I_BaseTiccmd(void)
{
    return &emptycmd;
}


void I_Quit (void)
{
    I_ShutdownGraphics();
    exit(0);
}


byte* I_AllocLow (int length)
{
    byte*	mem;
        
    mem = (byte *)malloc (length);
    memset (mem,0,length);
    return mem;
}

void I_Tactile (int on, int off, int total)
{
}


void I_Error (char *error, ...)
{

    va_list	argptr;

    // Message first.
    va_start (argptr,error);
    fprintf (stderr, "Error: ");
    vfprintf (stderr,error,argptr);
    fprintf (stderr, "\n");
    va_end (argptr);

	__debugbreak();

    fflush( stderr );

    I_ShutdownGraphics();
    
    exit(-1);
}


unsigned short	d_8to16table[256];
u32 *vid_vram;
u32 *vid_vreg;

int vid_flashblend;

// extern int		dirtybox[4]; 

u16 *vid_lastscreen;

int vid_clamp255(int v)
{
	if(v<0)return(0);
	if(v>255)return(255);
	return(v);
}

void I_InitGraphics (void)
{
//	vid_vreg=(u32 *)0xA05F8000;
//	vid_vreg[(0x44/4)]=0x000D;

//	vid_vreg[(0x5C/4)]=
//		(BASEWIDTH-1)|
//		((BASEHEIGHT-1)<<10)|
//		((BASEWIDTH+1)<<20);

//	vid_vram=(u32 *)0xA5000000;

//	vid_vreg[(0x44/4)]=0x0085;

//	vid_vreg[(0x5C/4)]=
//		(BASEWIDTH/2-1)|
//		((BASEHEIGHT-1)<<10)|
//		((BASEWIDTH/2+1)<<20);
	
	tk_con_disable();
	
	screen=malloc(BASEWIDTH*BASEHEIGHT*2);
	
	vid_lastscreen=malloc(BASEWIDTH*BASEHEIGHT*2);
	
	IN_Init();
}


void I_ShutdownGraphics(void)
{
}

void I_SetPalette (byte* palette)
{
	int cr, cg, cb;
	int cy, cu, cv;
	int i, j, k;
	
	for(i=0; i<256; i++)
	{
		cr=palette[i*3+0];
		cg=palette[i*3+1];
		cb=palette[i*3+2];
		
#if 0

//#ifdef CONGFX
// #if 1
#if 0
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

//		d2d_8to24table[i]=0xFF000000|(cr<<16)|(cg<<8)|cb;
	}
	
//	d_8to16table[255]=0x7FFF;
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

void I_UpdateNoBlit (void)
{
}

void IN_Init (void)
{
//	tk_puts("\x1B%~");
}

void IN_Shutdown (void)
{
}


#define	K_TAB			9
#define	K_ENTER			13
#define	K_ESCAPE		27
#define	K_SPACE			32

// normal keys should be passed as lowercased ascii

#define	K_BACKSPACE		127
#define	K_UPARROW		128
#define	K_DOWNARROW		129
#define	K_LEFTARROW		130
#define	K_RIGHTARROW	131

#define	K_ALT			132
#define	K_CTRL			133
#define	K_SHIFT			134
#define	K_F1			135
#define	K_F2			136
#define	K_F3			137
#define	K_F4			138
#define	K_F5			139
#define	K_F6			140
#define	K_F7			141
#define	K_F8			142
#define	K_F9			143
#define	K_F10			144
#define	K_F11			145
#define	K_F12			146
#define	K_INS			147
#define	K_DEL			148
#define	K_PGDN			149
#define	K_PGUP			150
#define	K_HOME			151
#define	K_END			152

#define K_PAUSE			255

//
// mouse buttons generate virtual keys
//
#define	K_MOUSE1		200
#define	K_MOUSE2		201
#define	K_MOUSE3		202

//
// joystick buttons
//
#define	K_JOY1			203
#define	K_JOY2			204
#define	K_JOY3			205
#define	K_JOY4			206

//
// aux keys are for multi-buttoned joysticks to generate so they can use
// the normal binding process
//
#define	K_AUX1			207
#define	K_AUX2			208
#define	K_AUX3			209
#define	K_AUX4			210
#define	K_AUX5			211
#define	K_AUX6			212
#define	K_AUX7			213
#define	K_AUX8			214
#define	K_AUX9			215
#define	K_AUX10			216
#define	K_AUX11			217
#define	K_AUX12			218
#define	K_AUX13			219
#define	K_AUX14			220
#define	K_AUX15			221
#define	K_AUX16			222
#define	K_AUX17			223
#define	K_AUX18			224
#define	K_AUX19			225
#define	K_AUX20			226
#define	K_AUX21			227
#define	K_AUX22			228
#define	K_AUX23			229
#define	K_AUX24			230
#define	K_AUX25			231
#define	K_AUX26			232
#define	K_AUX27			233
#define	K_AUX28			234
#define	K_AUX29			235
#define	K_AUX30			236
#define	K_AUX31			237
#define	K_AUX32			238

// JACK: Intellimouse(c) Mouse Wheel Support

#define K_MWHEELUP		239
#define K_MWHEELDOWN	240


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
		D_PostEvent(&event);
		return(0);
	}

	event.type = ev_keyup;
	event.data1 = c1;
	D_PostEvent(&event);
	return(0);
}

void IN_Commands (void)
{
	int c, dn;

	while(tk_kbhit())
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
		
		if(c>=256)
			continue;
		
		Key_Event (c, dn);
	}
}

#if 0
void	VID_ConGfx_EncBlock16P(u16 *src, u16 *lsrc, u32 *rdsta, u32 *rdstb)
{
	u16 tpxb[16];
	u16 *cs, *ct;
	int cmin, cmax;
	register int cavg, calo, cahi;
	int clra, clrb;
	u32 px2;
	u32 dxa, dxb;
	u64 lpx0, lpx1, lpx2, lpx3, lpx4;
	int cy, cu, cv, cu2, cv2;
	int cr, cg, cb;
	register int k0, k1;
//	register int k2, k3;
	int i, j, k;

	__hint_use_egpr();

#if 0
	ct=lsrc; cs=src;
	for(i=0; i<4; i++)
	{
		if(*(u64 *)cs!=*(u64 *)ct)
			break;
		cs+=BASEWIDTH;
		ct+=BASEWIDTH;
	}
	
	if(i>=4)
		return;
	for(; i<4; i++)
	{
		*(u64 *)ct=*(u64 *)cs;
		cs+=BASEWIDTH;
		ct+=BASEWIDTH;
	}
#endif

#if 0
	ct=lsrc; cs=src; i=0;
	while(1)
	{
		if(*(u64 *)cs!=*(u64 *)ct)	break;
		cs+=BASEWIDTH;	ct+=BASEWIDTH; i++;
		if(*(u64 *)cs!=*(u64 *)ct)	break;
		cs+=BASEWIDTH;	ct+=BASEWIDTH; i++;
		if(*(u64 *)cs!=*(u64 *)ct)	break;
		cs+=BASEWIDTH;	ct+=BASEWIDTH; i++;
		if(*(u64 *)cs!=*(u64 *)ct)	break;
		return;
	}
	
//	if(i>=4)
//		return;
	ct=lsrc; cs=src;
	for(i=0; i<4; i++)
	{
		px3=*(u64 *)cs;
		*(u64 *)ct=px3;
		((u64 *)tpxb)[i]=px3;
		cs+=BASEWIDTH;
		ct+=BASEWIDTH;
	}

	calo=65536;	cahi=-1;
	ct=tpxb;
	for(i=0; i<4; i++)
	{
		px3=*(u64 *)ct;
		k0=(u16)(px3    );
		k1=(u16)(px3>>16);
		calo=__int_min(calo, k0);
		cahi=__int_max(cahi, k0);
		k0=(u16)(px3>>32);
		calo=__int_min(calo, k1);
		cahi=__int_max(cahi, k1);
		k1=(u16)(px3>>48);
		calo=__int_min(calo, k0);
		cahi=__int_max(cahi, k0);
		calo=__int_min(calo, k1);
		cahi=__int_max(cahi, k1);
		ct+=4;
	}
#endif

#if 1
	ct=lsrc; cs=src; i=0;
	lpx0=*(u64 *)cs;
	lpx4=*(u64 *)ct;
//	i|=(lpx0!=*(u64 *)ct);
	cs+=BASEWIDTH;		ct+=BASEWIDTH;
	i|=(lpx0!=lpx4);

	lpx1=*(u64 *)cs;
	lpx4=*(u64 *)ct;
//	i|=(lpx1!=*(u64 *)ct);
	cs+=BASEWIDTH;		ct+=BASEWIDTH;
	i|=(lpx1!=lpx4);

	lpx2=*(u64 *)cs;
	lpx4=*(u64 *)ct;
//	i|=(lpx2!=*(u64 *)ct);
	cs+=BASEWIDTH;		ct+=BASEWIDTH;
	i|=(lpx2!=lpx4);

	lpx3=*(u64 *)cs;
	lpx4=*(u64 *)ct;
//	i|=(lpx3!=*(u64 *)ct);
	i|=(lpx3!=lpx4);

	if(!i)	return;

	ct=lsrc; cs=src;
	*(u64 *)ct=lpx0;
//	((u64 *)tpxb)[0]=lpx0;
	ct+=BASEWIDTH;
	*(u64 *)ct=lpx1;
//	((u64 *)tpxb)[1]=lpx1;
	ct+=BASEWIDTH;
	*(u64 *)ct=lpx2;
//	((u64 *)tpxb)[2]=lpx2;
	ct+=BASEWIDTH;
	*(u64 *)ct=lpx3;
//	((u64 *)tpxb)[3]=lpx3;

	calo=65536;	cahi=-1;

	k0=(u16)(lpx0    );		k1=(u16)(lpx0>>16);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	k0=(u16)(lpx0>>32);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
	k1=(u16)(lpx0>>48);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);

	k0=(u16)(lpx1    );		k1=(u16)(lpx1>>16);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	k0=(u16)(lpx1>>32);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
	k1=(u16)(lpx1>>48);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);

#if 1
	k0=(u16)(lpx2    );		k1=(u16)(lpx2>>16);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	k0=(u16)(lpx2>>32);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
	k1=(u16)(lpx2>>48);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
#endif

#if 0
	k0=(u16)(lpx3    );		k1=(u16)(lpx3>>16);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	k0=(u16)(lpx3>>32);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
	k1=(u16)(lpx3>>48);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
#endif

#endif

#if 0
	calo=65536;	cahi=-1;
	ct=tpxb; cs=src;
	for(i=0; i<4; i++)
	{
		k0=cs[0];
		k1=cs[1];
		ct[0]=k0;
		calo=__int_min(calo, k0);
		cahi=__int_max(cahi, k0);
		k0=cs[2];
		ct[1]=k1;
		calo=__int_min(calo, k1);
		cahi=__int_max(cahi, k1);
		k1=cs[3];
		ct[2]=k0;
		calo=__int_min(calo, k0);
		cahi=__int_max(cahi, k0);
		ct[3]=k1;
		calo=__int_min(calo, k1);
		cahi=__int_max(cahi, k1);
		cs+=BASEWIDTH;
		ct+=4;
	}
#endif

	cmin=calo;
	cmax=cahi;
	
	cavg=(cmin+cmax)>>1;
	calo=(cmin+cavg)>>1;
	cahi=(cmax+cavg)>>1;

#if 0
	ct=tpxb; px2=0;
	for(i=0; i<4; i++)
	{
		k0=ct[0];
		k1=ct[1];

		px2=(px2<<2)|((k0>cavg)<<1);
		px2|=(k0>cahi)|(calo>k0);

		k0=ct[2];

		px2=(px2<<2)|((k1>cavg)<<1);
		px2|=(k1>cahi)|(calo>k1);

		k1=ct[3];

		px2=(px2<<2)|((k0>cavg)<<1);
		px2|=(k0>cahi)|(calo>k0);

		px2=(px2<<2)|((k1>cavg)<<1);
		px2|=(k1>cahi)|(calo>k1);

		ct+=4;
	}
	px2=px2^((~(px2>>1))&0x55555555);
#endif

#if 1
	ct=tpxb; px2=0;

	k0=(u16)(lpx0    );
	k1=(u16)(lpx0>>16);
	px2=(px2<<2)|((k0>cavg)<<1);
	px2|=(k0>cahi)|(calo>k0);
	k0=(u16)(lpx0>>32);
	px2=(px2<<2)|((k1>cavg)<<1);
	px2|=(k1>cahi)|(calo>k1);
	k1=(u16)(lpx0>>48);
	px2=(px2<<2)|((k0>cavg)<<1);
	px2|=(k0>cahi)|(calo>k0);
	px2=(px2<<2)|((k1>cavg)<<1);
	px2|=(k1>cahi)|(calo>k1);

//	__debugbreak();

	k0=(u16)(lpx1    );
	k1=(u16)(lpx1>>16);
	px2=(px2<<2)|((k0>cavg)<<1);
	px2|=(k0>cahi)|(calo>k0);
	k0=(u16)(lpx1>>32);
	px2=(px2<<2)|((k1>cavg)<<1);
	px2|=(k1>cahi)|(calo>k1);
	k1=(u16)(lpx1>>48);
	px2=(px2<<2)|((k0>cavg)<<1);
	px2|=(k0>cahi)|(calo>k0);
	px2=(px2<<2)|((k1>cavg)<<1);
	px2|=(k1>cahi)|(calo>k1);

	k0=(u16)(lpx2    );
	k1=(u16)(lpx2>>16);
	px2=(px2<<2)|((k0>cavg)<<1);
	px2|=(k0>cahi)|(calo>k0);
	k0=(u16)(lpx2>>32);
	px2=(px2<<2)|((k1>cavg)<<1);
	px2|=(k1>cahi)|(calo>k1);
	k1=(u16)(lpx2>>48);
	px2=(px2<<2)|((k0>cavg)<<1);
	px2|=(k0>cahi)|(calo>k0);
	px2=(px2<<2)|((k1>cavg)<<1);
	px2|=(k1>cahi)|(calo>k1);

	k0=(u16)(lpx3    );
	k1=(u16)(lpx3>>16);
	px2=(px2<<2)|((k0>cavg)<<1);
	px2|=(k0>cahi)|(calo>k0);
	k0=(u16)(lpx3>>32);
	px2=(px2<<2)|((k1>cavg)<<1);
	px2|=(k1>cahi)|(calo>k1);
	k1=(u16)(lpx3>>48);
	px2=(px2<<2)|((k0>cavg)<<1);
	px2|=(k0>cahi)|(calo>k0);
	px2=(px2<<2)|((k1>cavg)<<1);
	px2|=(k1>cahi)|(calo>k1);

	px2=px2^((~(px2>>1))&0x55555555);
#endif

#if 0
	cy=(cmin>>11)&31;	cv=(cmin>> 5)&31;	cu=(cmin    )&31;
	cu2=(cu-16)<<1;	cv2=(cv-16)<<1;
	cg=cy-((cu2+cv2)>>1);
	cb=cg+cu2;	cr=cg+cv2;

	if((cr|cg|cb)&(~31))
	{
		cr=__int_clamp(cr, 0, 31);
		cg=__int_clamp(cg, 0, 31);
		cb=__int_clamp(cb, 0, 31);
	}
	clrb=(cr<<10)|(cg<<5)|cb;

	cy=(cmax>>11)&31;	cv=(cmax>> 5)&31;	cu=(cmax    )&31;
	cu2=(cu-16)<<1;	cv2=(cv-16)<<1;
	cg=cy-((cu2+cv2)>>1);
	cb=cg+cu2;	cr=cg+cv2;

	if((cr|cg|cb)&(~31))
	{
		cr=__int_clamp(cr, 0, 31);
		cg=__int_clamp(cg, 0, 31);
		cb=__int_clamp(cb, 0, 31);
	}
	clra=(cr<<10)|(cg<<5)|cb;

	dxa=0xC0000000|(clrb<<15)|clra;
	dxb=px2;
#endif

#if 1
	clrb=cmin>>1;
	clra=cmax>>1;

	dxa=0xC0000000|(clrb<<15)|clra;
	dxb=px2;
#endif

	*rdsta=dxa;
	*rdstb=dxb;
}
#endif

#if 1
void	VID_ConGfx_EncBlock16P(u16 *src, u16 *lsrc, u32 *rdsta, u32 *rdstb)
{
//	u16 tpxb[16];
	u16 *cs, *ct;
	int cmin, cmax;
	register int cavg, calo, cahi;
	int clra, clrb;
	u32 px2;
	u32 dxa, dxb;
	u64 lpx0, lpx1, lpx2, lpx3, lpx4;
//	int cy, cu, cv, cu2, cv2;
//	int cr, cg, cb;
	register int k0, k1;
//	register int k2, k3;
	int i, j, k;

	__hint_use_egpr();

#if 1

#if 1
	ct=lsrc; cs=src; i=0;
	lpx0=*(u64 *)cs;
	lpx4=*(u64 *)ct;
	cs+=BASEWIDTH;		ct+=BASEWIDTH;
	i|=(lpx0!=lpx4);
//	i|=(lpx0!=(*(u64 *)ct));

	lpx1=*(u64 *)cs;
	lpx4=*(u64 *)ct;
	cs+=BASEWIDTH;		ct+=BASEWIDTH;
	i|=(lpx1!=lpx4);
//	i|=(lpx1!=(*(u64 *)ct));

//	if(i)	goto gotpix;

	lpx2=*(u64 *)cs;
	lpx4=*(u64 *)ct;
	cs+=BASEWIDTH;		ct+=BASEWIDTH;
	i|=(lpx2!=lpx4);
//	i|=(lpx2!=(*(u64 *)ct));

	lpx3=*(u64 *)cs;
	lpx4=*(u64 *)ct;
	i|=(lpx3!=lpx4);
//	i|=(lpx3!=(*(u64 *)ct));

	if(!i)	return;
	
//	gotpix:
#endif

#if 0
	cs=src;
	lpx0=*(u64 *)cs;
	cs+=BASEWIDTH;
	lpx1=*(u64 *)cs;
	ct+=BASEWIDTH;
	lpx2=*(u64 *)cs;
	cs+=BASEWIDTH;
	lpx3=*(u64 *)cs;

	ct=lsrc;
	i=0;
	lpx4=*(u64 *)ct;
	ct+=BASEWIDTH;
	i|=(lpx0!=lpx4);
//	i|=(lpx0!=(*(u64 *)ct));

	lpx4=*(u64 *)ct;
	cs+=BASEWIDTH;
	i|=(lpx1!=lpx4);
//	i|=(lpx1!=(*(u64 *)ct));

//	if(i)	goto gotpix;

	lpx4=*(u64 *)ct;
	ct+=BASEWIDTH;
	i|=(lpx2!=lpx4);
//	i|=(lpx2!=(*(u64 *)ct));

	lpx4=*(u64 *)ct;
	i|=(lpx3!=lpx4);
//	i|=(lpx3!=(*(u64 *)ct));

	if(!i)	return;
	
//	gotpix:
#endif

	ct=lsrc; cs=src;
	*(u64 *)ct=lpx0;
	ct+=BASEWIDTH;
	*(u64 *)ct=lpx1;
	ct+=BASEWIDTH;
	*(u64 *)ct=lpx2;
	ct+=BASEWIDTH;
	*(u64 *)ct=lpx3;

	calo=65536;	cahi=-1;

#if 0
	k0=(u16)(lpx0    );		k1=(u16)(lpx0>>16);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	k0=(u16)(lpx0>>32);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
	k1=(u16)(lpx0>>48);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
#endif

#if 1
//	k0=(u16)(lpx0    );
	k1=(u16)(lpx0>>16);
//	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
//	k0=(u16)(lpx0>>32);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
	k1=(u16)(lpx0>>48);
//	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
#endif

#if 0
	k0=(u16)(lpx1    );		k1=(u16)(lpx1>>16);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	k0=(u16)(lpx1>>32);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
	k1=(u16)(lpx1>>48);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);

	k0=(u16)(lpx2    );		k1=(u16)(lpx2>>16);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	k0=(u16)(lpx2>>32);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
	k1=(u16)(lpx2>>48);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
#endif

#if 1
	k0=(u16)(lpx1    );	
//	k1=(u16)(lpx1>>16);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	k0=(u16)(lpx1>>32);
//	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
//	k1=(u16)(lpx1>>48);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
//	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);

//	k0=(u16)(lpx2    );
	k1=(u16)(lpx2>>16);
//	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
//	k0=(u16)(lpx2>>32);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
	k1=(u16)(lpx2>>48);
//	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
#endif

#if 0
	k0=(u16)(lpx3    );				k1=(u16)(lpx3>>16);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	k0=(u16)(lpx3>>32);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
	k1=(u16)(lpx3>>48);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
#endif

#if 1
	k0=(u16)(lpx3    );
//	k1=(u16)(lpx3>>16);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	k0=(u16)(lpx3>>32);
//	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
//	k1=(u16)(lpx3>>48);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
//	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
#endif

#endif

	cmin=calo;
	cmax=cahi;
	
	cavg=(cmin+cmax)>>1;
	calo=(cmin+cavg)>>1;
	cahi=(cmax+cavg)>>1;

#if 1
//	ct=tpxb;
	px2=0;

	k0=(u16)(lpx0    );				k1=(u16)(lpx0>>16);
	px2=(px2<<2)|((k0>cavg)<<1);	px2|=(k0>cahi)|(calo>k0);
	k0=(u16)(lpx0>>32);
	px2=(px2<<2)|((k1>cavg)<<1);	px2|=(k1>cahi)|(calo>k1);
	k1=(u16)(lpx0>>48);
	px2=(px2<<2)|((k0>cavg)<<1);	px2|=(k0>cahi)|(calo>k0);
	px2=(px2<<2)|((k1>cavg)<<1);	px2|=(k1>cahi)|(calo>k1);

	k0=(u16)(lpx1    );				k1=(u16)(lpx1>>16);
	px2=(px2<<2)|((k0>cavg)<<1);	px2|=(k0>cahi)|(calo>k0);
	k0=(u16)(lpx1>>32);
	px2=(px2<<2)|((k1>cavg)<<1);	px2|=(k1>cahi)|(calo>k1);
	k1=(u16)(lpx1>>48);
	px2=(px2<<2)|((k0>cavg)<<1);	px2|=(k0>cahi)|(calo>k0);
	px2=(px2<<2)|((k1>cavg)<<1);	px2|=(k1>cahi)|(calo>k1);

	k0=(u16)(lpx2    );				k1=(u16)(lpx2>>16);
	px2=(px2<<2)|((k0>cavg)<<1);	px2|=(k0>cahi)|(calo>k0);
	k0=(u16)(lpx2>>32);
	px2=(px2<<2)|((k1>cavg)<<1);	px2|=(k1>cahi)|(calo>k1);
	k1=(u16)(lpx2>>48);
	px2=(px2<<2)|((k0>cavg)<<1);	px2|=(k0>cahi)|(calo>k0);
	px2=(px2<<2)|((k1>cavg)<<1);	px2|=(k1>cahi)|(calo>k1);

	k0=(u16)(lpx3    );				k1=(u16)(lpx3>>16);
	px2=(px2<<2)|((k0>cavg)<<1);	px2|=(k0>cahi)|(calo>k0);
	k0=(u16)(lpx3>>32);
	px2=(px2<<2)|((k1>cavg)<<1);	px2|=(k1>cahi)|(calo>k1);
	k1=(u16)(lpx3>>48);
	px2=(px2<<2)|((k0>cavg)<<1);	px2|=(k0>cahi)|(calo>k0);
	px2=(px2<<2)|((k1>cavg)<<1);	px2|=(k1>cahi)|(calo>k1);

	px2=px2^((~(px2>>1))&0x55555555);
#endif

	clrb=cmin>>1;						clra=cmax>>1;
	dxa=0xC0000000|(clrb<<15)|clra;		dxb=px2;
	*rdsta=dxa;							*rdstb=dxb;
}
#endif

int TK_EncBlock16Q(u16 *src, u16 *lsrc, u32 *rdst);

#if 1
int VID_ConGfx_EncBlock16Q(u16 *src, u16 *lsrc, u32 *rdst)
{
	u16 *cs1, *cs2;
	u16 *lcs1, *lcs2;
	int rt;
	
	cs1=src;
	cs2=src+(BASEWIDTH<<2);

	lcs1=lsrc;
	lcs2=lsrc+(BASEWIDTH<<2);

//	VID_ConGfx_EncBlock16P(cs1+0, lcs1+0, rdst+0, rdst+4);
//	VID_ConGfx_EncBlock16P(cs1+4, lcs1+4, rdst+1, rdst+5);
//	VID_ConGfx_EncBlock16P(cs2+0, lcs2+0, rdst+2, rdst+6);
//	VID_ConGfx_EncBlock16P(cs2+4, lcs2+4, rdst+3, rdst+7);

//	TK_EncBlock16P(cs1+0, lcs1+0, rdst+0, rdst+4);
//	TK_EncBlock16P(cs1+4, lcs1+4, rdst+1, rdst+5);
//	TK_EncBlock16P(cs2+0, lcs2+0, rdst+2, rdst+6);
//	TK_EncBlock16P(cs2+4, lcs2+4, rdst+3, rdst+7);

	rt=TK_EncBlock16P(cs1+0, lcs1+0, rdst+3, rdst+7);
	rt|=TK_EncBlock16P(cs1+4, lcs1+4, rdst+2, rdst+6)<<1;
	rt|=TK_EncBlock16P(cs2+0, lcs2+0, rdst+1, rdst+5)<<2;
	rt|=TK_EncBlock16P(cs2+4, lcs2+4, rdst+0, rdst+4)<<3;
	return(rt);
}
#endif


/*
Higher Quality Sub-Block Encoder
 */
#if 1
int		VID_ConGfx_EncBlock16HP(u16 *src, u16 *lsrc, u32 *rdsta, u32 *rdstb)
{
//	u16 tpxb[16];
	u16 *cs, *ct;
	int cmin, cmax;
	register int cavg, calo, cahi;
	int clra, clrb;
	u32 px2;
	u32 dxa, dxb;
	u64 lpx0, lpx1, lpx2, lpx3, lpx4;
	int cy, cu, cv;
	int cu0, cv0, cu1, cv1;
	int cu2, cv2, cu3, cv3;
	int acy, acu, acv;
//	int cr, cg, cb;
	register int k0, k1;
//	register int k2, k3;
	int i, j, k;

//	__hint_use_egpr();

#if 1
	lpx0=*(u64 *)(src+(BASEWIDTH*0));
	lpx1=*(u64 *)(src+(BASEWIDTH*1));
	lpx2=*(u64 *)(src+(BASEWIDTH*2));
	lpx3=*(u64 *)(src+(BASEWIDTH*3));

	calo=65536;	cahi=-1;

	k0=(u16)(lpx0    );		k1=(u16)(lpx0>>16);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	k0=(u16)(lpx0>>32);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
	k1=(u16)(lpx0>>48);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);

	k0=(u16)(lpx1    );		k1=(u16)(lpx1>>16);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	k0=(u16)(lpx1>>32);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
	k1=(u16)(lpx1>>48);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);

	k0=(u16)(lpx2    );		k1=(u16)(lpx2>>16);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	k0=(u16)(lpx2>>32);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
	k1=(u16)(lpx2>>48);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);

	k0=(u16)(lpx3    );				k1=(u16)(lpx3>>16);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	k0=(u16)(lpx3>>32);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
	k1=(u16)(lpx3>>48);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);

#endif

	acu=0;
	acv=0;

	k0=(u16)(lpx0    );
	k1=(u16)(lpx0>>32);
	acu+=k0&0x001F; 	acv+=k0&0x03E0;
	acu+=k1&0x001F; 	acv+=k1&0x03E0;

	k0=(u16)(lpx1>>16);
	k1=(u16)(lpx1>>48);
	acu+=k0&0x001F; 	acv+=k0&0x03E0;
	acu+=k1&0x001F; 	acv+=k1&0x03E0;

	k0=(u16)(lpx2    );
	k1=(u16)(lpx2>>32);
	acu+=k0&0x001F; 	acv+=k0&0x03E0;
	acu+=k1&0x001F; 	acv+=k1&0x03E0;

	k0=(u16)(lpx3>>16);
	k1=(u16)(lpx3>>48);
	acu+=k0&0x001F; 	acv+=k0&0x03E0;
	acu+=k1&0x001F; 	acv+=k1&0x03E0;

//	acu>>=3;
//	acv>>=3;

	acu=(acu+0x04)>>3;
	acv=(acv+0x40)>>3;
	acu=__int_clamp(acu, 0, 0x001F);
	acv=__int_clamp(acv, 0, 0x03E0);

	cmin=calo;
	cmax=cahi;
	
	cavg=(cmin+cmax)>>1;
	calo=(cmin+cavg)>>1;
	cahi=(cmax+cavg)>>1;

//	cu=((cmin&0x001F)+acu)>>1;
//	cv=((cmin&0x03E0)+acv)>>1;
	cu=((cmin&0x001F)+acu+0x01)>>1;
	cv=((cmin&0x03E0)+acv+0x10)>>1;
	cu=__int_clamp(cu, 0, 0x001F);
	cv=__int_clamp(cv, 0, 0x03E0);
	cmin=(cmin&0xFC00)|(cv&0x03E0)|(cu&0x001F);

//	cu=((cmax&0x001F)+acu)>>1;
//	cv=((cmax&0x03E0)+acv)>>1;
	cu=((cmax&0x001F)+acu+0x01)>>1;
	cv=((cmax&0x03E0)+acv+0x10)>>1;
	cu=__int_clamp(cu, 0, 0x001F);
	cv=__int_clamp(cv, 0, 0x03E0);
	cmax=(cmax&0xFC00)|(cv&0x03E0)|(cu&0x001F);

#if 1
	px2=0;

	k0=(u16)(lpx0    );				k1=(u16)(lpx0>>16);
	px2=(px2<<2)|((k0>cavg)<<1);	px2|=(k0>cahi)|(calo>k0);
	k0=(u16)(lpx0>>32);
	px2=(px2<<2)|((k1>cavg)<<1);	px2|=(k1>cahi)|(calo>k1);
	k1=(u16)(lpx0>>48);
	px2=(px2<<2)|((k0>cavg)<<1);	px2|=(k0>cahi)|(calo>k0);
	px2=(px2<<2)|((k1>cavg)<<1);	px2|=(k1>cahi)|(calo>k1);

	k0=(u16)(lpx1    );				k1=(u16)(lpx1>>16);
	px2=(px2<<2)|((k0>cavg)<<1);	px2|=(k0>cahi)|(calo>k0);
	k0=(u16)(lpx1>>32);
	px2=(px2<<2)|((k1>cavg)<<1);	px2|=(k1>cahi)|(calo>k1);
	k1=(u16)(lpx1>>48);
	px2=(px2<<2)|((k0>cavg)<<1);	px2|=(k0>cahi)|(calo>k0);
	px2=(px2<<2)|((k1>cavg)<<1);	px2|=(k1>cahi)|(calo>k1);

	k0=(u16)(lpx2    );				k1=(u16)(lpx2>>16);
	px2=(px2<<2)|((k0>cavg)<<1);	px2|=(k0>cahi)|(calo>k0);
	k0=(u16)(lpx2>>32);
	px2=(px2<<2)|((k1>cavg)<<1);	px2|=(k1>cahi)|(calo>k1);
	k1=(u16)(lpx2>>48);
	px2=(px2<<2)|((k0>cavg)<<1);	px2|=(k0>cahi)|(calo>k0);
	px2=(px2<<2)|((k1>cavg)<<1);	px2|=(k1>cahi)|(calo>k1);

	k0=(u16)(lpx3    );				k1=(u16)(lpx3>>16);
	px2=(px2<<2)|((k0>cavg)<<1);	px2|=(k0>cahi)|(calo>k0);
	k0=(u16)(lpx3>>32);
	px2=(px2<<2)|((k1>cavg)<<1);	px2|=(k1>cahi)|(calo>k1);
	k1=(u16)(lpx3>>48);
	px2=(px2<<2)|((k0>cavg)<<1);	px2|=(k0>cahi)|(calo>k0);
	px2=(px2<<2)|((k1>cavg)<<1);	px2|=(k1>cahi)|(calo>k1);

	px2=px2^((~(px2>>1))&0x55555555);
#endif

	cu0=(((lpx0    )&0x001F)+((lpx1>>16)&0x001F)+
		 ((lpx2    )&0x001F)+((lpx3>>16)&0x001F))>>2;
	cu1=(((lpx0>>32)&0x001F)+((lpx1>>48)&0x001F)+
		 ((lpx2>>32)&0x001F)+((lpx3>>48)&0x001F))>>2;

	cu2=(((lpx0    )&0x001F)+((lpx0>>32)&0x001F)+
		 ((lpx1>>16)&0x001F)+((lpx1>>48)&0x001F))>>2;
	cu3=(((lpx2    )&0x001F)+((lpx2>>32)&0x001F)+
		 ((lpx3>>16)&0x001F)+((lpx3>>48)&0x001F))>>2;

	cv0=(((lpx0    )&0x03E0)+((lpx1>>16)&0x03E0)+
		 ((lpx2    )&0x03E0)+((lpx3>>16)&0x03E0))>>2;
	cv1=(((lpx0>>32)&0x03E0)+((lpx1>>48)&0x03E0)+
		 ((lpx2>>32)&0x03E0)+((lpx3>>48)&0x03E0))>>2;

	cv2=(((lpx0    )&0x03E0)+((lpx0>>32)&0x03E0)+
		 ((lpx1>>16)&0x03E0)+((lpx1>>48)&0x03E0))>>2;
	cv3=(((lpx2    )&0x03E0)+((lpx2>>32)&0x03E0)+
		 ((lpx3>>16)&0x03E0)+((lpx3>>48)&0x03E0))>>2;
	
	cv0&=0x03C0;	cv1&=0x03C0;
	cv2&=0x03C0;	cv3&=0x03C0;

	acu=abs(cu0-cu1)+abs(cu2-cu3) +
		((abs(cv0-cv1)+abs(cv2-cv3))>>5) ;
//	if(acu>((cmax-cmin)>>10))
	if(acu>((cmax-cmin)>> 8))
	{
		k=cmin; cmin=cmax; cmax=k;
		px2=~px2;
//		clra&=~32;		clrb&=~32;
		clra&=0xFC00;
		clrb&=0xFC00;
		
		if(abs(cu2-cu3)>abs(cu0-cu1))
		{
			clra|=cu2;
			clrb|=cu3|32;
		}else
		{
			clra|=cu0;
			clrb|=cu1;
		}

		if(abs(cv2-cv3)>abs(cv0-cv1))
		{
			clra|=cv2|32;
			clrb|=cv3;
		}else
		{
			clra|=cv0;
			clrb|=cv1;
		}
	}

	clrb=cmin>>1;						clra=cmax>>1;
	dxa=0xC0000000|(clrb<<15)|clra;		dxb=px2;
	*rdsta=dxa;							*rdstb=dxb;
	
	return(1);
}
#endif

/*
Higher Quality Block Encoder
 */

#if 1
int VID_ConGfx_EncBlock16HQ(u16 *src, u16 *lsrc, u32 *rdst)
{
	u16 *cs1, *cs2;
	u16 *lcs1, *lcs2;
	int rt;
	
	cs1=src;
	cs2=src+(BASEWIDTH<<2);

	lcs1=lsrc;
	lcs2=lsrc+(BASEWIDTH<<2);

//	VID_ConGfx_EncBlock16P(cs1+0, lcs1+0, rdst+0, rdst+4);
//	VID_ConGfx_EncBlock16P(cs1+4, lcs1+4, rdst+1, rdst+5);
//	VID_ConGfx_EncBlock16P(cs2+0, lcs2+0, rdst+2, rdst+6);
//	VID_ConGfx_EncBlock16P(cs2+4, lcs2+4, rdst+3, rdst+7);

	VID_ConGfx_EncBlock16HP(cs1+0, lcs1+0, rdst+3, rdst+7);
	VID_ConGfx_EncBlock16HP(cs1+4, lcs1+4, rdst+2, rdst+6);
	VID_ConGfx_EncBlock16HP(cs2+0, lcs2+0, rdst+1, rdst+5);
	VID_ConGfx_EncBlock16HP(cs2+4, lcs2+4, rdst+0, rdst+4);
	return(1);
}
#endif

#if 0
int VID_ConGfx_EncBlock16HQ(u16 *src, u16 *lsrc, u32 *rdst)
{
	u64 lpixa[8*2];
	u64 lpixa_u[8];
	u64 lpixa_v[8];
	u64 lpx0, lpx1, lpx2, lpx3;
	u16 *cs;
	int calo, cahi, cmin, cmax, cavg;
	int calo_u, cahi_u, cmin_u, cmax_u, cavg_u;
	int calo_v, cahi_v, cmin_v, cmax_v, cavg_v;

	u32 px0, px1, px2, px3;
	u32 px4, px5, px6, px7;

	int k, k0, k1;
	int x, y;
	int i, j;
	
	calo  =65535;	cahi  =0;
	calo_u=65535;	cahi_u=0;
	calo_v=65535;	cahi_v=0;
	
	cs=src;
	for(y=0; y<8; y++)
	{
		lpx0=((u64 *)cs)[0];
		lpx1=((u64 *)cs)[1];
		lpixa[y*2+0]=lpx0;
		lpixa[y*2+1]=lpx1;
		
		cs+=BASEWIDTH;

		k0=(u16)(lpx0    );
		calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
		k1=(u16)(lpx0>>16);
		calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
		k0=(u16)(lpx0>>32);
		calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
		k1=(u16)(lpx0>>48);
		calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);

		k0=(u16)(lpx1    );
		calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
		k1=(u16)(lpx1>>16);
		calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
		k0=(u16)(lpx1>>32);
		calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
		k1=(u16)(lpx1>>48);
		calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);

		
		k=lpx0    ;		k0=(k&0x03E0);		k1=(k&0x001F);
		calo_u=__int_min(calo_u, k0);	cahi_u=__int_max(cahi_u, k0);
		calo_v=__int_min(calo_v, k1);	cahi_v=__int_max(cahi_v, k1);
		lpx2=k0;				lpx3=k1;
		
		k=lpx0>>32;		k0=(k&0x03E0);		k1=(k&0x001F);
		calo_u=__int_min(calo_u, k0);	cahi_u=__int_max(cahi_u, k0);
		calo_v=__int_min(calo_v, k1);	cahi_v=__int_max(cahi_v, k1);
		lpx2=(lpx2<<16)|k0;		lpx3=(lpx3<<16)|k1;

		k=lpx1    ;		k0=(k&0x03E0);		k1=(k&0x001F);
		calo_u=__int_min(calo_u, k0);	cahi_u=__int_max(cahi_u, k0);
		calo_v=__int_min(calo_v, k1);	cahi_v=__int_max(cahi_v, k1);
		lpx2=(lpx2<<16)|k0;		lpx3=(lpx3<<16)|k1;

		k=lpx1>>32;		k0=(k&0x03E0);		k1=(k&0x001F);
		calo_u=__int_min(calo_u, k0);	cahi_u=__int_max(cahi_u, k0);
		calo_v=__int_min(calo_v, k1);	cahi_v=__int_max(cahi_v, k1);
		lpx2=(lpx2<<16)|k0;		lpx3=(lpx3<<16)|k1;
		
		lpixa_u[y]=lpx2;
		lpixa_v[y]=lpx3;
	}
	
	cmin  =calo  ;	cmax  =cahi  ;	cavg  =(calo  +cahi  )>>1;
	cmin_u=calo_u;	cmax_u=cahi_u;	cavg_u=(calo_u+cahi_u)>>1;
	cmin_v=calo_v;	cmax_v=cahi_v;	cavg_v=(calo_v+cahi_v)>>1;

	calo  =(cmin  +cavg  )>>1;	cahi  =(cmax  +cavg  )>>1;
	calo_u=(cmin_u+cavg_u)>>1;	cahi_u=(cmax_u+cavg_u)>>1;
	calo_v=(cmin_v+cavg_v)>>1;	cahi_v=(cmax_v+cavg_v)>>1;

	for(i=0; i<4; i++)
	{
		lpx0=lpixa_u[i*2+0];	lpx1=lpixa_u[i*2+1];
		lpx2=lpixa_v[i*2+0];	lpx3=lpixa_v[i*2+1];
		lpx0=(lpx0+lpx1)>>1;
		lpx1=(lpx2+lpx3)>>1;
		lpx0&=0x03E003E003E003E0ULL;
		lpx1&=0x001F001F001F001FULL;
		lpixa_u[i]=lpx0;
		lpixa_v[i]=lpx1;
	}

	px0=0;	px1=0;
	px2=0;	px3=0;
	for(i=0; i<4; i++)
	{
		lpx0=lpixa[i*2+0];	lpx1=lpixa[i*2+1];
		lpx2=lpixa[i*2+8];	lpx3=lpixa[i*2+9];
	
		k0=(u16)(lpx0>>0);
		px0=(px0<<2)|((k0>cavg)<<1);
		px0|=(k0>cahi)|(calo>k0);
		k1=(u16)(lpx0>>16);
		px0=(px0<<2)|((k1>cavg)<<1);
		px0|=(k1>cahi)|(calo>k1);
		k0=(u16)(lpx0>>32);
		px0=(px0<<2)|((k0>cavg)<<1);
		px0|=(k0>cahi)|(calo>k0);
		k1=(u16)(lpx0>>48);
		px0=(px0<<2)|((k1>cavg)<<1);
		px0|=(k1>cahi)|(calo>k1);

		k0=(u16)(lpx1>>0);
		px1=(px1<<2)|((k0>cavg)<<1);
		px1|=(k0>cahi)|(calo>k0);
		k1=(u16)(lpx1>>16);
		px1=(px1<<2)|((k1>cavg)<<1);
		px1|=(k1>cahi)|(calo>k1);
		k0=(u16)(lpx1>>32);
		px1=(px1<<2)|((k0>cavg)<<1);
		px1|=(k0>cahi)|(calo>k0);
		k1=(u16)(lpx1>>48);
		px1=(px1<<2)|((k1>cavg)<<1);
		px1|=(k1>cahi)|(calo>k1);

		k0=(u16)(lpx2>>0);
		px2=(px2<<2)|((k0>cavg)<<1);
		px2|=(k0>cahi)|(calo>k0);
		k1=(u16)(lpx2>>16);
		px2=(px2<<2)|((k1>cavg)<<1);
		px2|=(k1>cahi)|(calo>k1);
		k0=(u16)(lpx2>>32);
		px2=(px2<<2)|((k0>cavg)<<1);
		px2|=(k0>cahi)|(calo>k0);
		k1=(u16)(lpx2>>48);
		px2=(px2<<2)|((k1>cavg)<<1);
		px2|=(k1>cahi)|(calo>k1);

		k0=(u16)(lpx3>>0);
		px3=(px3<<2)|((k0>cavg)<<1);
		px3|=(k0>cahi)|(calo>k0);
		k1=(u16)(lpx3>>16);
		px3=(px3<<2)|((k1>cavg)<<1);
		px3|=(k1>cahi)|(calo>k1);
		k0=(u16)(lpx3>>32);
		px3=(px3<<2)|((k0>cavg)<<1);
		px3|=(k0>cahi)|(calo>k0);
		k1=(u16)(lpx3>>48);
		px3=(px3<<2)|((k1>cavg)<<1);
		px3|=(k1>cahi)|(calo>k1);
	}

	px0=px0^((~(px0>>1))&0x55555555);
	px1=px1^((~(px1>>1))&0x55555555);
	px2=px2^((~(px2>>1))&0x55555555);
	px3=px3^((~(px3>>1))&0x55555555);

	for(i=0; i<2; i++)
	{
		lpx0=lpixa_u[i*2+0];	lpx1=lpixa_u[i*2+1];
		lpx2=lpixa_v[i*2+0];	lpx3=lpixa_v[i*2+1];

		k0=(u16)(lpx0>> 0);
		px6=(px6<<2)|((k0>cavg_u)<<1);
		px6|=(k0>cahi_u)|(calo_u>k0);
		k1=(u16)(lpx0>>16);
		px6=(px6<<2)|((k1>cavg_u)<<1);
		px6|=(k1>cahi_u)|(calo_u>k1);

		k0=(u16)(lpx1>> 0);
		px6=(px6<<2)|((k0>cavg_u)<<1);
		px6|=(k0>cahi_u)|(calo_u>k0);
		k1=(u16)(lpx1>>16);
		px6=(px6<<2)|((k1>cavg_u)<<1);
		px6|=(k1>cahi_u)|(calo_u>k1);

		k0=(u16)(lpx0>>32);
		px6=(px6<<2)|((k0>cavg_u)<<1);
		px6|=(k0>cahi_u)|(calo_u>k0);
		k1=(u16)(lpx0>>48);
		px6=(px6<<2)|((k1>cavg_u)<<1);
		px6|=(k1>cahi_u)|(calo_u>k1);

		k0=(u16)(lpx1>>32);
		px6=(px6<<2)|((k0>cavg_u)<<1);
		px6|=(k0>cahi_u)|(calo_u>k0);
		k1=(u16)(lpx1>>48);
		px6=(px6<<2)|((k1>cavg_u)<<1);
		px6|=(k1>cahi_u)|(calo_u>k1);


		k0=(u16)(lpx2>> 0);
		px7=(px7<<2)|((k0>cavg_v)<<1);
		px7|=(k0>cahi_v)|(calo_v>k0);
		k1=(u16)(lpx2>>16);
		px7=(px7<<2)|((k1>cavg_v)<<1);
		px7|=(k1>cahi_v)|(calo_v>k1);

		k0=(u16)(lpx3>> 0);
		px7=(px7<<2)|((k0>cavg_v)<<1);
		px7|=(k0>cahi_v)|(calo_v>k0);
		k1=(u16)(lpx3>>16);
		px7=(px7<<2)|((k1>cavg_v)<<1);
		px7|=(k1>cahi_v)|(calo_v>k1);

		k0=(u16)(lpx2>>32);
		px7=(px7<<2)|((k0>cavg_v)<<1);
		px7|=(k0>cahi_v)|(calo_v>k0);
		k1=(u16)(lpx2>>48);
		px7=(px7<<2)|((k1>cavg_v)<<1);
		px7|=(k1>cahi_v)|(calo_v>k1);

		k0=(u16)(lpx3>>32);
		px7=(px7<<2)|((k0>cavg_v)<<1);
		px7|=(k0>cahi_v)|(calo_v>k0);
		k1=(u16)(lpx3>>48);
		px7=(px7<<2)|((k1>cavg_v)<<1);
		px7|=(k1>cahi_v)|(calo_v>k1);
	}

	px6=px6^((~(px6>>1))&0x55555555);
	px7=px7^((~(px7>>1))&0x55555555);
	

//	px6=0x5A5A5A5A;
//	px7=0x5A5A5A5A;

//	px6=0x69966996;
//	px7=0x5A5A5A5A;

//	px6=0;
//	px7=0;

	cmin>>=8;	cmin_u>>=2;		cmin_v<<=3;
	cmax>>=8;	cmax_u>>=2;		cmax_v<<=3;
	
//	cmin_u=128;		cmax_u=128;
//	cmin_v=128;		cmax_v=128;
	
	px4=0xA0000000 | (cmax<<16) |
		(cmax_v<<8) | (cmax_u);
	px5=(cmin<<16) | (cmin_v<<8) | (cmin_u);

	rdst[0]=px4;
	rdst[1]=px5;
	rdst[2]=px7;
	rdst[3]=px6;

	rdst[4]=px3;
	rdst[5]=px2;
	rdst[6]=px1;
	rdst[7]=px0;
}
#endif

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

int vid_frnum=0;

u64		r_colmask[32];
byte	i_scrflash;
byte	i_scr_bnc[40*25];

void R_CellMarkBox (int bx0, int bx1, int by0, int by1);

void I_FinishUpdate_ScanCopy(u16 *ics, u32 *ict, int blkn);
void I_FinishUpdate_ScanCopyVbl(u16 *ics, u32 *ict, int blkn, int vblend);

#define I_SCR_BMP128K


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
//	DrSmallNumber(fps, 320-16, 2);
	DrSmallNumber(fps, 320-12, 2);

#if 0
//	tb[0]='0'+((fps/100)%10);
//	tb[1]='0'+((fps/ 10)%10);
//	tb[2]='0'+((fps    )%10);
//	tb[3]=0;

	tb[0]='0'+((fps/ 10)%10);
	tb[1]='0'+((fps    )%10);
	tb[2]=0;
	
//	HU_DrawString(320-22, 2, tb);
#endif
}

void I_FinishUpdate (void)
{
	u32 *conbufa;
	int bx, by, by2;

//	u64 pxa, pxb, pxc, pxd;
	register u64 pxa, pxb, pxc, pxd;
	u64 colmask;
	byte *ics;
	u16 *ict16, *ics16, *ics16b;
	u16 *icl16, *icl16b;
	u16 *icz16;
	u32 *ict;
	u32 bxa, bxb, bxc, bxd;
	int pix, bn, brt;
	int i, j, k;

	__hint_use_egpr();

	I_DrawFramerate();

//	R_CellMarkBox(
//		dirtybox[BOXLEFT],		dirtybox[BOXRIGHT],
//		dirtybox[BOXBOTTOM],	dirtybox[BOXTOP]);

//	R_CellMarkBox(
//		dirtybox[2],	dirtybox[3],
//		dirtybox[1],	dirtybox[0]);

//	conbufa=(u32 *)0xA00A0000;
	conbufa=(u32 *)0xF00A0000;

//	((u32 *)0xF00BFF00)[8]=vid_frnum;
//	vid_frnum++;

#ifdef I_SCR_BMP128K
//	((u32 *)0xF00BFF00)[0]=0x0015;		//320x200x16bpp, YUV655
	((u32 *)0xF00BFF00)[0]=0x0095;		//320x200x16bpp, RGB555

//	((u32 *)0xF00BFF00)[0]=0x0025;		//320x200x16bpp
//	((u32 *)0xF00BFF00)[0]=0x0005;		//320x200x16bpp
//	((u32 *)0xF00BFF00)[0]=0x0010;		//320x200x16bpp
#else
	((u32 *)0xF00BFF00)[0]=0x0000;		//320x200
#endif

	ics16=(u16 *)screen;
	icl16=vid_lastscreen;
	ict=conbufa;
	
	if(!ics16)
		return;

// #if 1
#ifdef I_SCR_BMP128K
// #if 0
	bn=0;
	for(by=0; by<50; by++)
	{
		ics16b=ics16;
//		icl16b=icl16;

		if(!ics16)
			return;

#if 0
		I_FinishUpdate_ScanCopy(ics16, ict, 80);
		ict+=80*8;
#endif

#if 1
		if(!vid_flashblend)
		{
			I_FinishUpdate_ScanCopy(ics16, ict, 80);
			ict+=80*8;
			ics16+=4*BASEWIDTH;
			continue;
		}else
		{
			I_FinishUpdate_ScanCopyVbl(ics16, ict, 80, vid_flashblend);
			ict+=80*8;
			ics16+=4*BASEWIDTH;
			continue;
		}
#endif

#if 0
		for(bx=0; bx<80; bx++)
		{
//			pxa=*(u64 *)(ics16b+0*BASEWIDTH);
//			pxb=*(u64 *)(ics16b+1*BASEWIDTH);
//			pxc=*(u64 *)(ics16b+2*BASEWIDTH);
//			pxd=*(u64 *)(ics16b+3*BASEWIDTH);

			pxa=((u64 *)ics16b)[(0*BASEWIDTH)/4];
			pxb=((u64 *)ics16b)[(1*BASEWIDTH)/4];
			pxc=((u64 *)ics16b)[(2*BASEWIDTH)/4];
			pxd=((u64 *)ics16b)[(3*BASEWIDTH)/4];

			if(vid_flashblend)
			{
				pxa=VID_BlendFlash4x(pxa, vid_flashblend);
				pxb=VID_BlendFlash4x(pxb, vid_flashblend);
				pxc=VID_BlendFlash4x(pxc, vid_flashblend);
				pxd=VID_BlendFlash4x(pxd, vid_flashblend);
			}

#if 0
			ict[0]=pxa;			ict[2]=pxb;
			ict[4]=pxc;			ict[6]=pxd;
			ict[1]=pxa>>32;		ict[3]=pxb>>32;
			ict[5]=pxc>>32;		ict[7]=pxd>>32;
#else
			((u64 *)ict)[0]=pxa;	((u64 *)ict)[1]=pxb;
			((u64 *)ict)[2]=pxc;	((u64 *)ict)[3]=pxd;
#endif

//			bn++;
			ict+=8;
			ics16b+=4;
//			icl16b+=4;
		}
#endif

#if 0
		for(bx=0; bx<40; bx++)
		{
			pxa=((u64 *)ics16b)[(0*BASEWIDTH)/4];
			pxb=((u64 *)ics16b)[(1*BASEWIDTH)/4];
			pxc=((u64 *)ics16b)[(2*BASEWIDTH)/4];
			pxd=((u64 *)ics16b)[(3*BASEWIDTH)/4];

			((u64 *)ict)[0]=pxa;	((u64 *)ict)[1]=pxb;
			((u64 *)ict)[2]=pxc;	((u64 *)ict)[3]=pxd;
//			ict+=8;
//			ics16b+=4;

			pxa=((u64 *)ics16b)[(0*BASEWIDTH)/4+1];
			pxb=((u64 *)ics16b)[(1*BASEWIDTH)/4+1];
			pxc=((u64 *)ics16b)[(2*BASEWIDTH)/4+1];
			pxd=((u64 *)ics16b)[(3*BASEWIDTH)/4+1];

			((u64 *)ict)[4]=pxa;	((u64 *)ict)[5]=pxb;
			((u64 *)ict)[6]=pxc;	((u64 *)ict)[7]=pxd;

			ict+=16;
			ics16b+=8;
		}
#endif

		ics16+=4*BASEWIDTH;
//		icl16+=4*BASEWIDTH;
	}
#endif

// #if 1
#ifndef I_SCR_BMP128K
	bn=0;
	for(by=0; by<25; by++)
	{
//		colmask=r_colmask[by];
//		if(!i_scrflash)
//			colmask=-1;

		colmask=-1;
		
		if(!colmask)
		{
			ics16+=8*BASEWIDTH;
			icl16+=8*BASEWIDTH;
			ict+=8*40;
			continue;
		}

		ics16b=ics16;
		icl16b=icl16;
		for(bx=0; bx<40; bx++)
		{
//			by2=by;

//			if(colmask&(1LL<<bx))
			brt=VID_ConGfx_EncBlock16Q(ics16b, icl16b, ict);

#if 0
			if(brt)
			{
#if 0
				k=i_scr_bnc[bn];
				if(!k && ((brt&15)!=15))
				{
//					icl16b[0]=~ics16b[0];
//					icl16b[4]=~ics16b[4];
//					icl16b[(4*BASEWIDTH)+0]=~ics16b[0];
//					icl16b[(4*BASEWIDTH)+4]=~ics16b[4];

					if(!(brt&1))
						icl16b[(0*BASEWIDTH)+0]^=0x5555;
					if(!(brt&2))
						icl16b[(0*BASEWIDTH)+4]^=0x5555;
					if(!(brt&4))
						icl16b[(4*BASEWIDTH)+0]^=0x5555;
					if(!(brt&8))
						icl16b[(4*BASEWIDTH)+4]^=0x5555;

					VID_ConGfx_EncBlock16Q(ics16b, icl16b, ict);
				}
#endif

//				i_scr_bnc[bn]=8;
				i_scr_bnc[bn]=6;
			}else
			{
				brt=i_scr_bnc[bn];
				if(brt>0)
				{
					brt--;
					if(!brt)
					{
						VID_ConGfx_EncBlock16HQ(ics16b, icl16b, ict);
					}
					i_scr_bnc[bn]=brt;
				}
			}
#endif
			
			bn++;
			
			ict+=8;
			ics16b+=8;
			icl16b+=8;
		}
		ics16+=8*BASEWIDTH;
		icl16+=8*BASEWIDTH;
	}
#endif
	
//	ict[0]=vid_frnum;
//	conbufa[8100]=vid_frnum;
	
//	if(i_scrflash)
//		i_scrflash--;
//	else
//		i_scrflash=8;

	((u32 *)0xF00BFF00)[8]=vid_frnum;
	vid_frnum++;

	IN_Commands();
	
	M_ClearBox(dirtybox);
//	R_ClearCZBuf();
	
	for(i=0; i<25; i++)
		r_colmask[i]=0;
}

#if 0
void I_FinishUpdate (void)
{
	byte *cs;
	u16 *ct;
	int x, y;
	
	for(y=0; y<BASEHEIGHT; y++)
	{
		cs=screen+(y*BASEWIDTH);
		ct=((u16 *)vid_vram)+(y*BASEWIDTH);
		for(x=0; x<BASEWIDTH; x++)
		{
			*ct++=d_8to16table[*cs++];
		}
	}
	
	IN_Commands();
}
#endif

void I_WaitVBL(int count)
{
}

void I_ReadScreen (dt_scrpix *scr)
{
    memcpy (scr, screen, SCREENWIDTH*SCREENHEIGHT*sizeof(dt_scrpix));
}

void I_BeginRead (void)
{
}

void I_EndRead (void)
{
}
