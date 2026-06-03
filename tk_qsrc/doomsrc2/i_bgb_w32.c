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

extern byte	st_oddframe;		//BGB: Odd Frame
extern byte	st_do3dglasses;		//BGB: Do 3D glasses effect
extern int		r_view_xadjust;

// int	mb_used = 6;

long long __smullq(int a, int b)
{
	return(((s64)a) * ((s64)b));
}

u64 __int32_dmulu(u32 a, u32 b)
{
	return(((u64)a) * ((u64)b));
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

int vid_flashblend;

byte *btesh2_gfxcon_framebuf;
int btesh2_gfxcon_fb_dirty;
int btesh2_gfxcon_fbxs;
int btesh2_gfxcon_fbys;

void I_InitGraphics (void)
{
	GfxDrv_Start();
	
	btesh2_gfxcon_fbxs=BASEWIDTH;
	btesh2_gfxcon_fbys=BASEHEIGHT;

#ifdef OBRA
	btesh2_gfxcon_fbxs=BASEWIDTH*2;
	btesh2_gfxcon_fbys=BASEHEIGHT*2;
#endif

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
u64 VID_BlendEven4x16(u64 pixa, u64 pixb)
{
	u64 pix;
//	pix=((pixa&0xFBDEFBDEFBDEFBDEULL)>>1)+
//		((pixb&0xFBDEFBDEFBDEFBDEULL)>>1);
	pix=((pixa&0x7BDE7BDE7BDE7BDEULL)>>1)+
		((pixb&0x7BDE7BDE7BDE7BDEULL)>>1);
	return(pix);
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
#endif

#define BASEWIDTH_X2 (BASEWIDTH*2)
#define BASEHEIGHT_X2 (BASEHEIGHT*2)

void I_FinishUpdate (void)
{
	static byte bayer4[16]={
	 1,  5,  2,  6,
	 9, 13, 10, 14,
	 3,  7,  4,  8,
	11, 15, 12, 16,
	};

	static byte *screen_luma_e;
	static byte *screen_luma_o;
	static byte *screen_luma;
	dt_scrpix *cs;
	u32 *ct, *ct0, *ct1;
	byte *cty, *cty_e, *cty_o;
	int px0, px1, px2, px3, px4, px5;
	int cr0, cr1, cr2, cr3, cr4;
	int cg0, cg1, cg2, cg3, cg4;
	int cb0, cb1, cb2, cb3, cb4;
	int dr1, dr2, dr3, dr4;
	int dg1, dg2, dg3, dg4;
	int db1, db2, db3, db4;
	int dr, dg, db, cy, cy0, cy1;
	int x, y, z, p;

	st_oddframe = !st_oddframe;
	if(st_do3dglasses)
	{
		r_view_xadjust=st_oddframe?1:(-1);
		colormaps_bcur = colormaps_an3d;
	}else
	{
		r_view_xadjust=0;
		colormaps_bcur = colormaps_base;
	}

#ifdef OBRA

	if(!screen_luma)
	{
		screen_luma_e=malloc(BASEWIDTH*BASEHEIGHT*4);
		screen_luma_o=malloc(BASEWIDTH*BASEHEIGHT*4);
	}
	
	screen_luma=st_oddframe?screen_luma_o:screen_luma_e;

#if 1
	for(y=0; y<BASEHEIGHT_X2; y++)
	{
		cs=screens[0]+((y>>1)*BASEWIDTH);
		cty=screen_luma+(y*BASEWIDTH_X2);
		for(x=0; x<BASEWIDTH_X2; x++)
		{
			px0=cs[x>>1];
			px1=px0; px2=px0;
			px3=px0; px4=px0;
			if((x-1)>0)					px1=cs[(x-1)>>1];
			if((x+1)<BASEWIDTH_X2)		px2=cs[(x+1)>>1];
			if((y-1)>0)					px3=cs[(x-BASEWIDTH_X2)>>1];
			if((y+1)<BASEHEIGHT_X2)		px4=cs[(x+BASEWIDTH_X2)>>1];

			cr0=(px0>>10)&31;	cr1=(px1>>10)&31;
			cr2=(px2>>10)&31;	cr3=(px3>>10)&31;
			cr4=(px4>>10)&31;
			cg0=(px0>>5)&31;	cg1=(px1>>5)&31;
			cg2=(px2>>5)&31;	cg3=(px3>>5)&31;
			cg4=(px4>>5)&31;
			cb0=(px0>>0)&31;	cb1=(px1>>0)&31;
			cb2=(px2>>0)&31;	cb3=(px3>>0)&31;
			cb4=(px4>>0)&31;
			dr1=cr1-cr0;	dr2=cr2-cr0;
			dr3=cr3-cr0;	dr4=cr4-cr0;

			dg1=cg1-cg0;	dg2=cg2-cg0;
			dg3=cg3-cg0;	dg4=cg4-cg0;

			db1=cb1-cb0;	db2=cb2-cb0;
			db3=cb3-cb0;	db4=cb4-cb0;

			dr=dr1*dr1 + dr2*dr2 + dr3*dr3 + dr4*dr4;
			dg=dg1*dg1 + dg2*dg2 + dg3*dg3 + dg4*dg4;
			db=db1*db1 + db2*db2 + db3*db3 + db4*db4;

			dr>>=0;
			dg>>=0;
			db>>=0;

			cy=(cr0+(2*cg0)+cb0);
			cy=(cy>>1) + dr + dg + db;

			if(cy>255)cy=255;
			cty[x]=cy;
		}
	}

	for(y=0; y<BASEHEIGHT_X2; y++)
	{
		ct=((u32 *)btesh2_gfxcon_framebuf)+(y*BASEWIDTH_X2);
		cty=screen_luma+(y*BASEWIDTH_X2);

		if(st_do3dglasses)
		{
			cty_e=screen_luma_e+(y*BASEWIDTH_X2);
			cty_o=screen_luma_o+(y*BASEWIDTH_X2);
		}else
		{
			cty_e=screen_luma+(y*BASEWIDTH_X2);
			cty_o=screen_luma+(y*BASEWIDTH_X2);
		}

		px4=0xFF00FF00;
		px5=0x00FF00FF;

		if(st_do3dglasses==1)
		{
			px4=0xFF0000FF;
			px5=0x00FFFF00;
		}

		if(st_do3dglasses==2)
		{
			px4=0xFFFF00FF;
			px5=0x0000FF00;
		}

		for(x=0; x<BASEWIDTH_X2; x++)
		{
//			p=(x>>2)^(y>>11);
//			p^=p>>13;
//			p^=p>>7;

//			p=(x>>2)-(y>>2);
//			p=(p*251)>>8;
		
//			z=((y&3)*4)+(x&3);
//			z^=(p&15);
//			cr0=bayer4[z]*15;

//			p=(2*x-y)-(2*y-x);
//			p=(p*251)>>8;

#if 0
			z=2*y-x;
			p=(13*y)+(7*x)+(19*z);
			p^=(p<<11);
			p^=(p>>17);
			p^=(p<<13);
			p^=(p>>19);
//			p=p*251+17;
//			p=p*251+11;
			p^=(p>>7);
			p^=(p<<5);
			p>>=8;
#endif

//			z=((y&3)*4)+(x&3)+p;
			z=((y&3)*4)+(x&3);
			cr0=bayer4[z&15]*15;

//			cr0=((p&15)+1)*15;
			
			if(st_do3dglasses)
			{
				cy0=cty_e[x];
				cy1=cty_o[x];

				cy0>>=1;	cy1>>=1;

				px0=(cy0>cr0)?px4:0xFF000000U;
				z^=0xF;
				cr0=bayer4[z]*15;
				px0|=(cy1>cr0)?px5:0;
			}else
			{
				cy=cty[x];			
//				px0=cy|(cy<<8)|(cy<<16)|(255<<24);
				px0=(cy>cr0)?0xFFFFFFFFU:0xFF000000U;
			}
			ct[x]=px0;
		}
	}
#endif


#if 0
	for(y=0; y<BASEHEIGHT; y++)
	{
		cs=screens[0]+(y*BASEWIDTH);
		ct=((u32 *)btesh2_gfxcon_framebuf)+(y*BASEWIDTH);
		cty=screen_luma+(y*BASEWIDTH);
		for(x=0; x<BASEWIDTH; x++)
		{
			px0=cs[x];
			px1=px0; px2=px0;
			px3=px0; px4=px0;
			if((x-1)>0)		px1=cs[x-1];
			if((x+1)<0)		px2=cs[x+1];
			if((y-1)>0)		px3=cs[x-BASEWIDTH];
			if((y+1)<0)		px4=cs[x+BASEWIDTH];

			cr0=(px0>>10)&31;	cr1=(px1>>10)&31;
			cr2=(px2>>10)&31;	cr3=(px3>>10)&31;
			cr4=(px4>>10)&31;
			cg0=(px0>>5)&31;	cg1=(px1>>5)&31;
			cg2=(px2>>5)&31;	cg3=(px3>>5)&31;
			cg4=(px4>>5)&31;
			cb0=(px0>>0)&31;	cb1=(px1>>0)&31;
			cb2=(px2>>0)&31;	cb3=(px3>>0)&31;
			cb4=(px4>>0)&31;
			dr1=cr1-cr0;	dr2=cr2-cr0;
			dr3=cr3-cr0;	dr4=cr4-cr0;

			dg1=cg1-cg0;	dg2=cg2-cg0;
			dg3=cg3-cg0;	dg4=cg4-cg0;

			db1=cb1-cb0;	db2=cb2-cb0;
			db3=cb3-cb0;	db4=cb4-cb0;

			dr=dr1*dr1 + dr2*dr2 + dr3*dr3 + dr4*dr4;
			dg=dg1*dg1 + dg2*dg2 + dg3*dg3 + dg4*dg4;
			db=db1*db1 + db2*db2 + db3*db3 + db4*db4;

			dr>>=0;
			dg>>=0;
			db>>=0;

			cy=(cr0+(2*cg0)+cb0);
			cy=(cy>>1) + dr + dg + db;

			if(cy>255)cy=255;
			cty[x]=cy;
		}
	}

	for(y=0; y<BASEHEIGHT; y++)
	{
		ct0=((u32 *)btesh2_gfxcon_framebuf)+((y*2+0)*BASEWIDTH*2);
		ct1=((u32 *)btesh2_gfxcon_framebuf)+((y*2+1)*BASEWIDTH*2);
		cty=screen_luma+(y*BASEWIDTH);

		if(st_do3dglasses)
		{
			cty_e=screen_luma_e+(y*BASEWIDTH);
			cty_o=screen_luma_o+(y*BASEWIDTH);
		}else
		{
			cty_e=screen_luma+(y*BASEWIDTH);
			cty_o=screen_luma+(y*BASEWIDTH);
		}

		px4=0xFF00FF00;
		px5=0x00FF00FF;

		if(st_do3dglasses==1)
		{
			px4=0xFF0000FF;
			px5=0x00FFFF00;
		}

		if(st_do3dglasses==2)
		{
			px4=0xFFFF00FF;
			px5=0x0000FF00;
		}

		for(x=0; x<BASEWIDTH; x++)
		{
			z=((y&1)*8)+((x&1)*2);
			cr0=bayer4[z+0]*15;
			cr1=bayer4[z+1]*15;
			cr2=bayer4[z+4]*15;
			cr3=bayer4[z+5]*15;
			

			if(st_do3dglasses)
			{
				cy0=cty_e[x];
				cy1=cty_o[x];

				cy0>>=1;	cy1>>=1;

//				cy0=(cy0*5)>>3;
//				cy1=(cy1*5)>>3;

				px0=(cy0>cr0)?px4:0xFF000000U;
				px1=(cy0>cr1)?px4:0xFF000000U;
				px2=(cy0>cr2)?px4:0xFF000000U;
				px3=(cy0>cr3)?px4:0xFF000000U;

	#if 1
				z^=0xA;
				cr3=bayer4[z+0]*15;
				cr1=bayer4[z+1]*15;
				cr2=bayer4[z+4]*15;
				cr0=bayer4[z+5]*15;
	#endif

				px0|=(cy1>cr0)?px5:0;
				px1|=(cy1>cr1)?px5:0;
				px2|=(cy1>cr2)?px5:0;
				px3|=(cy1>cr3)?px5:0;
			}else
			{
				cy=cty[x];
			
				px0=cy|(cy<<8)|(cy<<16)|(255<<24);
				px0=(cy>cr0)?0xFFFFFFFFU:0xFF000000U;
				px1=(cy>cr1)?0xFFFFFFFFU:0xFF000000U;
				px2=(cy>cr2)?0xFFFFFFFFU:0xFF000000U;
				px3=(cy>cr3)?0xFFFFFFFFU:0xFF000000U;
			}

			ct0[x*2+0]=px0;		ct0[x*2+1]=px1;
			ct1[x*2+0]=px2;		ct1[x*2+1]=px3;
		}
	}
#endif

	btesh2_gfxcon_fb_dirty=1;
	GfxDrv_EndDrawing();
	return;
#endif

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

			if(vid_flashblend)
			{
				p=VID_BlendFlash(p, vid_flashblend);
			}

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
