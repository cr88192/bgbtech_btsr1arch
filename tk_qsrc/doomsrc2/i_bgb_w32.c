#include "i_system.h"
#include "i_sound.h"
#include "i_video.h"
#include "i_net.h"
#include "v_video.h"

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

byte *btesh2_gfxcon_framebuf;
int btesh2_gfxcon_fb_dirty;
int btesh2_gfxcon_fbxs;
int btesh2_gfxcon_fbys;

void I_InitGraphics (void)
{
	GfxDrv_Start();
	
	btesh2_gfxcon_fbxs=BASEWIDTH;
	btesh2_gfxcon_fbys=BASEHEIGHT;

	GfxDrv_PrepareFramebuf();

	screens[0]=malloc(BASEWIDTH*BASEHEIGHT*
		sizeof(dt_scrpix));
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
	
	R_ClearCZBuf();

//	for(y=0; y<BASEHEIGHT; y++)
	for(y=0; y<viewheight; y++)
	{
//		cs=screens[0]+(y*BASEWIDTH);
		cs=screens[0]+((y+viewwindowy)*BASEWIDTH)+viewwindowx;
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

		cy=vid_clamp255(cy+2)>>2;
		cu=vid_clamp255(cu+4)>>3;
		cv=vid_clamp255(cv+4)>>3;
		d_8to16table[i]=(cy<<10)|(cv<<5)|cu;

//		d_8to24table[i]=0xFF000000|(cr<<16)|(cg<<8)|cb;
		d_8to24table[i]=0xFF000000|(cb<<16)|(cg<<8)|cr;
	}
	
//	d_8to16table[255]=0x7FFF;
//	d_8to16table[255]=0x00FF;

	for(i=0; i<65536; i++)
	{
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

//		cr=cy;
//		cg=cy;
//		cb=cy;

		cr=vid_clamp255(cr);
		cg=vid_clamp255(cg);
		cb=vid_clamp255(cb);

		d_16to24table[i]=0xFF000000|(cb<<16)|(cg<<8)|cr;
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
		cs=screens[0]+(y*BASEWIDTH);
//		ct=((u16 *)vid_vram)+(y*BASEWIDTH);
		ct=((u32 *)btesh2_gfxcon_framebuf)+(y*BASEWIDTH);
		for(x=0; x<BASEWIDTH; x++)
		{
//			*ct++=d_8to16table[*cs++];
			p=*cs++;
//			if(p<0x100)
			if(0)
			{
				*ct++=d_8to24table[p];
			}else
			{
				*ct++=d_16to24table[p];
			}
		}
	}

	btesh2_gfxcon_fb_dirty=1;
	
	GfxDrv_EndDrawing();
}

void I_ReadScreen (dt_scrpix *scr)
{
    memcpy (scr, screens[0], SCREENWIDTH*SCREENHEIGHT*sizeof(dt_scrpix));
}

void I_StartTic (void)
{
}
