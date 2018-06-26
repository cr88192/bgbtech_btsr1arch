#include "i_system.h"
#include "i_sound.h"
#include "i_video.h"
#include "i_net.h"
#include "v_video.h"

typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

int	mb_used = 6;


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


void I_InitMusic(void)
{
}

void I_ShutdownMusic(void)
{
}

void I_SetMusicVolume(int volume)
{
}

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


void I_Init (void)
{
    I_InitSound();
}

int  I_GetHeapSize (void)
{
    return mb_used*1024*1024;
}

byte* I_ZoneBase (int*	size)
{
    *size = mb_used*1024*1024;
    return (byte *) malloc (*size);
}

unsigned int TK_GetTimeMs(void);

int I_GetTime (void)
{
	int t;
	t = TK_GetTimeMs();
	t=t>>4;
	return(t);
}


void I_StartFrame (void)
{
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

    fflush( stderr );

    I_ShutdownGraphics();
    
    exit(-1);
}


#ifndef BASEWIDTH
#define BASEWIDTH SCREENWIDTH
#define BASEHEIGHT SCREENHEIGHT
#endif

unsigned short	d_8to16table[256];
u32 *vid_vram;
u32 *vid_vreg;


int vid_clamp255(int v)
{
	if(v<0)return(0);
	if(v>255)return(255);
	return(v);
}

void I_InitGraphics (void)
{
	vid_vreg=(u32 *)0xA05F8000;
	vid_vreg[(0x44/4)]=0x000D;

	vid_vreg[(0x5C/4)]=
		(BASEWIDTH-1)|
		((BASEHEIGHT-1)<<10)|
		((BASEWIDTH+1)<<20);

	vid_vram=(u32 *)0xA5000000;

	vid_vreg[(0x44/4)]=0x0085;

	vid_vreg[(0x5C/4)]=
		(BASEWIDTH/2-1)|
		((BASEHEIGHT-1)<<10)|
		((BASEWIDTH/2+1)<<20);
		
	screens[0]=malloc(BASEWIDTH*BASEHEIGHT);
	
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

//		d2d_8to24table[i]=0xFF000000|(cr<<16)|(cg<<8)|cb;
	}
	
	d_8to16table[255]=0x7FFF;
}


void I_UpdateNoBlit (void)
{
}

void IN_Init (void)
{
	tk_puts("\x1B%~");
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

void I_FinishUpdate (void)
{
	byte *cs;
	u16 *ct;
	int x, y;
	
	for(y=0; y<BASEHEIGHT; y++)
	{
		cs=screens[0]+(y*BASEWIDTH);
		ct=((u16 *)vid_vram)+(y*BASEWIDTH);
		for(x=0; x<BASEWIDTH; x++)
		{
			*ct++=d_8to16table[*cs++];
		}
	}
	
	IN_Commands();
}

void I_WaitVBL(int count)
{
}

void I_ReadScreen (byte* scr)
{
    memcpy (scr, screens[0], SCREENWIDTH*SCREENHEIGHT);
}

void I_BeginRead (void)
{
}

void I_EndRead (void)
{
}
