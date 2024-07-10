/*
Copyright (C) 2022  Brendan G Bohannon

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

// byte *btesh2_gfxcon_framebuf;
// int btesh2_gfxcon_fb_dirty;

u16 tsampbuf[32768];
int tsamplen;

u32 *conbufa;

int vid_frnum;

int		SoundDev_WriteStereoSamples(short *mixbuf, int nsamp);
int		SoundDev_WriteStereoSamples2(short *mixbuf, int nsamp, int nsamp2);
void	SoundDev_Submit();
int		SoundDev_Init();

#ifndef _WIN32
#define USE_TKGDI
#include <tkgdi/tkgdi.h>
#endif

#ifdef _WIN32

u32 d_16to24table[65536];

int I_SystemInit()
{
	int cr, cg, cb;
	int i, j, k;

	printf("I_SystemInit: A0\n");

	for(i=0; i<65536; i++)
	{
		cr=(i>>10)&31;	cg=(i>>5)&31;	cb=(i>>0)&31;
		cr=(cr<<3)|(cr>>2);
		cg=(cg<<3)|(cg>>2);
		cb=(cb<<3)|(cb>>2);
		j=0xFF000000|(cb<<16)|(cg<<8)|(cr<<0);
		d_16to24table[i]=j;
	}

	printf("I_SystemInit: A1\n");

	SoundDev_Init();

	printf("I_SystemInit: A2\n");

	GfxDrv_Start();

	printf("I_SystemInit: A3\n");

//	tk_con_disable();
	
#ifndef PGL_NATIVEGL
	btesh2_gfxcon_fbxs=640;
	btesh2_gfxcon_fbys=400;
	
	GfxDrv_PrepareFramebuf();
#endif

	printf("I_SystemInit: A4\n");

	return(0);
}

int I_BeginFrame(u16 *fbuf, int xs, int ys)
{
#ifdef PGL_NATIVEGL
	GfxDrv_BeginDrawing();
#endif

	return(0);
}

int I_SystemFrame(u16 *fbuf, int xs, int ys)
{
	u32 *screen, *ct0, *ct1;
	u16 *cs;
	int x, y, px;

	screen=(u32 *)btesh2_gfxcon_framebuf;

//	SoundDev_WriteStereoSamples(ctx->sampbuf, k);
//	SoundDev_WriteStereoSamples(tsampbuf, tsamplen);
//	SoundDev_Submit();

	FRGL_EndInputFrame();


#ifndef PGL_NATIVEGL
	GfxDrv_BeginDrawing();

	for(y=0; y<ys; y++)
	{
		cs=fbuf+(ys-y-1)*xs;
//		cs=fbuf+y*xs;
		ct0=screen+((y*2+0)*btesh2_gfxcon_fbxs);
		ct1=screen+((y*2+1)*btesh2_gfxcon_fbxs);
		
		for(x=0; x<xs; x++)
		{
//			ct[x]=d_16to24table[cs[x]];
			px=d_16to24table[cs[x]];
			ct0[x*2+0]=px;	ct0[x*2+1]=px;
			ct1[x*2+0]=px;	ct1[x*2+1]=px;
//			ct[x]=0xFFFFFFFF;
		}
	}
	
	btesh2_gfxcon_fb_dirty=1;
#endif

	GfxDrv_EndDrawing();
	return(0);
}


int I_TimeMS()
{
	int tt;
	tt=FRGL_TimeMS();
	return(tt);
}

int I_KeyDown(int key)
{
	return(FRGL_KeyDown(key));
}

int I_KeyDownL(int key)
{
	return(FRGL_KeyDownL(key));
}


u16 i_keyrov[256];
byte i_keypos;

void I_UpdateKeys (void)
{
	u16 *cs;
	int key;

//	FRGL_EndInputFrame();

	cs=FRGL_GetKeybuf();
	while(*cs)
	{
		key=*cs++;
		if(!(key&0x8000))
			i_keyrov[i_keypos++]=key;
		BTM_ConHandleKey(key);
		BTM_InventoryHandleKey(btm_wrl, key);
	}
}
#endif

#ifdef __BJX2__

// TKGDI_BITMAPINFOHEADER i_t_dibinfo;
TKGDI_BITMAPINFOHEADER *i_dibinfo = NULL;
TKGHDC i_hDc;

void I_InitTkGdi()
{
	if(i_dibinfo)
		return;
		
//	i_dibinfo = &i_t_dibinfo;
	i_dibinfo = malloc(sizeof(TKGDI_BITMAPINFOHEADER));
	memset(i_dibinfo, 0, sizeof(TKGDI_BITMAPINFOHEADER));

	i_dibinfo->biSize=sizeof(TKGDI_BITMAPINFOHEADER);
	i_dibinfo->biWidth=320;
	i_dibinfo->biHeight=200;
	i_dibinfo->biBitCount=16;

//	tk_printf("  1\n", hDc);

	i_hDc=tkgCreateDisplay(i_dibinfo);
	tkgSetWindowTitle(i_hDc, "BtMini");

//	i_dibinfo->biHeight=-200;
	
//	screen_fbuf=tkgTryMapFrameBuffer(i_hDc, i_dibinfo);
}


int gfxdrv_kill=0;

int I_SystemInit()
{
//	SoundDev_Init();

//	conbufa=(u32 *)0xFFFFF00A0000ULL;
//	((u32 *)0xFFFFF00BFF00ULL)[0]=0x0095;		//320x200x16bpp, RGB555
//	tk_con_disable();

	I_InitTkGdi();
}

void I_FinishUpdate_ScanCopy(u16 *ics, u32 *ict, int blkn);
void I_FinishUpdate_ScanCopyVf(u16 *ics, u32 *ict, int blkn);

__asm {
I_FinishUpdate_ScanCopy:

	CMP/GT	0, R6
	BF .done

	.loop:
	MOV.Q	(R4,    0), R20
	MOV.Q	(R4,  640), R21
	MOV.Q	(R4, 1280), R22
	MOV.Q	(R4, 1920), R23
	MOV.Q	R20, (R5,  0)
	MOV.Q	R21, (R5,  8)
	ADD		-1, R6			| MOV.Q		R22, (R5, 16)
	ADD		8, R4			| MOV.Q		R23, (R5, 24)
	ADD		32, R5			| TEST		R6, R6
	BF .loop

	.done:
	RTSU

I_FinishUpdate_ScanCopyVf:

	CMP/GT	0, R6
	BF .done

	.loop:
	MOV.Q	(R4, 1920), R20
	MOV.Q	(R4, 1280), R21
	MOV.Q	(R4,  640), R22
	MOV.Q	(R4,    0), R23
	MOV.Q	R20, (R5,  0)
	MOV.Q	R21, (R5,  8)
	ADD		-1, R6			| MOV.Q		R22, (R5, 16)
	ADD		8, R4			| MOV.Q		R23, (R5, 24)
	ADD		32, R5			| TEST		R6, R6
	BF .loop

	.done:
	RTSU
};

int I_BeginFrame(u16 *fbuf, int xs, int ys)
{
	return(0);
}

int I_SystemFrame(u16 *fbuf, int xs, int ys)
{
	u64 *ct, *cte, ct1, *cs0, *cs1, *cs2, *cs3;
	int x, y;

//	SoundDev_WriteStereoSamples(tsampbuf, tsamplen);
//	SoundDev_Submit();

	tkgBlitImage(i_hDc, 0, 0, i_dibinfo, fbuf);

#if 0
	ct=(u64 *)conbufa;
	
	for(y=0; y<ys; y+=4)
	{
#if 1
		if(xs==320)
		{
//			cs0=(u64 *)(fbuf+((ys-y-1)*xs));
			cs3=(u64 *)(fbuf+((ys-y-4)*xs));
			I_FinishUpdate_ScanCopyVf(cs3, ct, 80);

//			cs0=(u64 *)(fbuf+(y*xs));
//			I_FinishUpdate_ScanCopy(cs0, ct, 80);

			ct+=(80*4);
			continue;
		}
#endif

#if 1
		cs0=(u64 *)(fbuf+((y+0)*xs));
		cs1=(u64 *)(fbuf+((y+1)*xs));
		cs2=(u64 *)(fbuf+((y+2)*xs));
		cs3=(u64 *)(fbuf+((y+3)*xs));

//		cs0=(u64 *)(fbuf+((ys-y-1)*xs));
//		cs1=(u64 *)(fbuf+((ys-y-2)*xs));
//		cs2=(u64 *)(fbuf+((ys-y-3)*xs));
//		cs3=(u64 *)(fbuf+((ys-y-4)*xs));
		ct=((u64 *)conbufa)+(y*80);

//		cte=ct+(80*4);
		cte=ct+((xs>>2)*4);
//		for(x=0; x<xs; x+=4)
		while(ct<cte)
		{
			ct[0]=cs0[0];	ct[1]=cs1[0];
			ct[2]=cs2[0];	ct[3]=cs3[0];
			ct+=4;	cs0++;	cs1++;	cs2++;	cs3++;
		}
#endif
	}

	((u32 *)0xFFFFF00BFF00ULL)[8]=vid_frnum;
	vid_frnum++;
#endif
}

unsigned int TK_GetTimeMs(void);

int I_TimeMS()
{
	return(TK_GetTimeMs());
}

u32 i_keymask[8];
u32 i_lkeymask[8];
u16 i_keyrov[256];
byte i_keypos;

void I_KeyEvent(int key, int dn)
{
	if(dn)
	{
		i_keymask[key>>5]|=1<<(key&31);
		i_keyrov[i_keypos++]=key;
	}else
	{
		i_keymask[key>>5]&=~(1<<(key&31));
	}
}

void I_UpdateKeys (void)
{
	TKGDI_EVENT t_imsg;
	TKGDI_EVENT *imsg;
	int i, j, c, dn;

	memcpy(i_lkeymask, i_keymask, 8*4);

	if(i_hDc>1)
	{
		thrd_yield();
		imsg=&t_imsg;

		while(1)
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
				
				I_KeyEvent (c, dn);
			}
		}
		
		return;
	}

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
			c&=0x7FFF;
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
		
		I_KeyEvent (c, dn);
	}
}

int I_KeyDown(int key)
{
	return(i_keymask[key>>5]&(1<<(key&31)));
//	return(0);
}

int I_KeyDownL(int key)
{
	return(i_lkeymask[key>>5]&(1<<(key&31)));
//	return(0);
}

int GfxDrv_MouseGetPos(int *rmx, int *rmy, int *rmb)
{
	*rmx=0;
	*rmy=0;
	*rmb=0;
	return(0);
}

int GfxDrv_MouseSetPos(int mx, int my)
{
	return(0);
}

int SoundDev_WriteStereoSamples2(s16 *samp, int cnt, int ovcnt)
{
	return(0);
}

#endif


int I_CheckCheatStr(char *str)
{
	byte rov;
	int i, l;
	
	l=strlen(str);
	rov=(i_keypos-l)&255;
	for(i=0; i<l; i++)
	{
		if(i_keyrov[(rov+i)&255]!=str[i])
			break;
	}
	if(i>=l)
	{
		i_keyrov[i_keypos++]=0x7F;
		return(1);
	}
	return(0);
}

int tst_rot2d_xy(float *iv, float *ov, float ang)
{
	float tx, ty;
	tx=iv[0];	ty=iv[1];
	ov[0]=cos(ang)*tx-sin(ang)*ty;
	ov[1]=sin(ang)*tx+cos(ang)*ty;
	return(0);
}

int tst_rot2d_xz(float *iv, float *ov, float ang)
{
	float tx, ty;
	tx=iv[0];	ty=iv[2];
	ov[0]=cos(ang)*tx-sin(ang)*ty;
	ov[2]=sin(ang)*tx+cos(ang)*ty;
	return(0);
}

static const int  cube_tris[6*6]=
{
	0, 1, 5,  0, 5, 4,	//-Y
	1, 3, 7,  1, 7, 5,	//+X
	3, 2, 6,  3, 6, 7,	//+Y
	2, 0, 4,  2, 4, 6,	//-X
	4, 5, 7,  4, 7, 6,	//+Z
	1, 0, 2,  1, 2, 3	//-Z
};

float	skybox_tris[6*6*8];

float cam_ang_yaw;
float cam_ang_pitch;
float cam_org[3];
float cam_vel[3];
float cam_ivel[3];
// int cam_mvflags;

int skybox_tex;
int skybox_tex_stars;
int skybox_onetex;

byte btm_dopause;

int BTMGL_InitSkybox()
{
	float vtx_xyz[8*4];
	float *xyz0, *st0;
	u32 *rgb0;
	float min, max, sofs, tofs;
	int i, j, k;
	
	max= 512;
	min=-512;
	
	for(i=0; i<8; i++)
	{
		vtx_xyz[i*4+0]=(i&1)?max:min;
		vtx_xyz[i*4+1]=(i&2)?max:min;
		vtx_xyz[i*4+2]=(i&4)?max:min;
	}
	
	xyz0=skybox_tris;
	rgb0=(u32 *)(skybox_tris+3);
	st0=xyz0+4;
	
	for(i=0; i<6; i++)
	{
		TKRA_Vec3F_Copy(vtx_xyz+cube_tris[i*6+0]*4, xyz0+(i*6+0)*8);
		TKRA_Vec3F_Copy(vtx_xyz+cube_tris[i*6+1]*4, xyz0+(i*6+1)*8);
		TKRA_Vec3F_Copy(vtx_xyz+cube_tris[i*6+2]*4, xyz0+(i*6+2)*8);
		TKRA_Vec3F_Copy(vtx_xyz+cube_tris[i*6+3]*4, xyz0+(i*6+3)*8);
		TKRA_Vec3F_Copy(vtx_xyz+cube_tris[i*6+4]*4, xyz0+(i*6+4)*8);
		TKRA_Vec3F_Copy(vtx_xyz+cube_tris[i*6+5]*4, xyz0+(i*6+5)*8);

//		st0[(i*6+0)*6+0]=0.0;		st0[(i*6+0)*6+1]=0.0;
//		st0[(i*6+1)*6+0]=1.0;		st0[(i*6+1)*6+1]=0.0;
//		st0[(i*6+2)*6+0]=1.0;		st0[(i*6+2)*6+1]=1.0;
//		st0[(i*6+3)*6+0]=0.0;		st0[(i*6+3)*6+1]=0.0;
//		st0[(i*6+4)*6+0]=1.0;		st0[(i*6+4)*6+1]=1.0;
//		st0[(i*6+5)*6+0]=0.0;		st0[(i*6+5)*6+1]=1.0;

		sofs=0;
		tofs=0;

		if(i&1)
			sofs=0.5;

		if(i==4)
			{ sofs=0; tofs=0.5; }
		if(i==5)
			{ sofs=0.5; tofs=0.5; }

		st0[(i*6+0)*8+0]=0.0+sofs;		st0[(i*6+0)*8+1]=0.0+tofs;
		st0[(i*6+1)*8+0]=0.5+sofs;		st0[(i*6+1)*8+1]=0.0+tofs;
		st0[(i*6+2)*8+0]=0.5+sofs;		st0[(i*6+2)*8+1]=0.5+tofs;
		st0[(i*6+3)*8+0]=0.0+sofs;		st0[(i*6+3)*8+1]=0.0+tofs;
		st0[(i*6+4)*8+0]=0.5+sofs;		st0[(i*6+4)*8+1]=0.5+tofs;
		st0[(i*6+5)*8+0]=0.0+sofs;		st0[(i*6+5)*8+1]=0.5+tofs;

		st0[(i*6+0)*8+2]=0.0;			st0[(i*6+0)*8+3]=0.0;
		st0[(i*6+1)*8+2]=1.0;			st0[(i*6+1)*8+3]=0.0;
		st0[(i*6+2)*8+2]=1.0;			st0[(i*6+2)*8+3]=1.0;
		st0[(i*6+3)*8+2]=0.0;			st0[(i*6+3)*8+3]=0.0;
		st0[(i*6+4)*8+2]=1.0;			st0[(i*6+4)*8+3]=1.0;
		st0[(i*6+5)*8+2]=0.0;			st0[(i*6+5)*8+3]=1.0;
		
		for(j=0; j<6; j++)
			st0[(i*6+j)*8+1]=1.0-st0[(i*6+j)*8+1];
		
		rgb0[(i*6+0)*8]=0xFFFFFFFFU;
		rgb0[(i*6+1)*8]=0xFFFFFFFFU;
		rgb0[(i*6+2)*8]=0xFFFFFFFFU;
		rgb0[(i*6+3)*8]=0xFFFFFFFFU;
		rgb0[(i*6+4)*8]=0xFFFFFFFFU;
		rgb0[(i*6+5)*8]=0xFFFFFFFFU;
	}
	return(0);
}

int BTMGL_DrawSkybox()
{
	if(skybox_tex<=0)
		return(0);

	pglMatrixMode(TKRA_MODELVIEW);
	pglPushMatrix();

	pglTranslatef(cam_org[0], cam_org[1], cam_org[2]);

	pglBindTexture(TKRA_TEXTURE_2D, skybox_tex);

	pglVertexPointer(3, TKRA_GL_FLOAT, 8*4, skybox_tris+0);

	if(skybox_onetex)
		pglTexCoordPointer(2, TKRA_GL_FLOAT, 8*4, skybox_tris+6);
	else
		pglTexCoordPointer(2, TKRA_GL_FLOAT, 8*4, skybox_tris+4);

	pglColorPointer(4, TKRA_GL_UNSIGNED_BYTE, 8*4, skybox_tris+3);
//	pglDrawArrays(TKRA_GL_TRIANGLES, 0, 6*6);
	pglDrawArrays(TKRA_GL_TRIANGLES, 0, 6*5);

//	pglDrawElements(TKRA_GL_TRIANGLES, 6*6, TKRA_GL_INT, cube_tris);

	pglPopMatrix();
	return(0);
}


// float cam_ang_yaw;
// float cam_ang_pitch;
// float cam_org[3];
byte btm_noclip;
byte btm_mlook;
byte btm_lmlook;

static float btm_skycolor[24*3]={
0.7, 0.3, 0.2,		//0600
0.5, 0.4, 0.7,		//0700

0.3, 0.6, 0.9,		//0800
0.3, 0.6, 0.9,		//0900
0.3, 0.6, 0.9,		//1000
0.3, 0.6, 0.9,		//1100
0.3, 0.6, 0.9,		//1200
0.3, 0.6, 0.9,		//1300
0.3, 0.6, 0.9,		//1400
0.3, 0.6, 0.9,		//1500
0.3, 0.6, 0.9,		//1600

0.5, 0.4, 0.7,		//1700
0.7, 0.3, 0.2,		//1800
0.2, 0.1, 0.1,		//1900

0.0, 0.0, 0.0,		//2000
0.0, 0.0, 0.0,		//2100
0.0, 0.0, 0.0,		//2200
0.0, 0.0, 0.0,		//2300
0.0, 0.0, 0.0,		//0000
0.0, 0.0, 0.0,		//0100
0.0, 0.0, 0.0,		//0200
0.0, 0.0, 0.0,		//0300
0.0, 0.0, 0.0,		//0400

0.2, 0.1, 0.1,		//0500
};

int BTM_InterpolateDaySky(BTM_World *wrl)
{
	static int lhr=-1;
	double f, g;
	float cr, cg, cb, cy, cra, cga, cba, crb, cgb, cbb;
	int bix0, bix1, hr;
	int i, j, k;

	if(wrl->daytimer>=1440000)
	{
		wrl->daytimer-=1440000;
		wrl->day++;
	}
	
	hr=((wrl->daytimer/60000)+6)%24;
	if(hr!=lhr)
	{
		printf("Hour %04d\n", hr*100);
		lhr=hr;
		
		BTM_PrintAllocStats();
	}

#if 0
	switch((wrl->daytimer/180000))
	{
		case 0:			case 3:
			pglClearColor(0.5, 0.6, 0.7, 1.0);
			break;
		case 1:		case 2:
			pglClearColor(0.3, 0.6, 0.9, 1.0);
			break;
		case 4:		case 7:
			pglClearColor(0.7, 0.6, 0.3, 1.0);
			break;
		case 5:		case 6:
			pglClearColor(0.1, 0.1, 0.1, 1.0);
			break;
			
	}
#endif

	f=(wrl->daytimer/60000.0);
	bix0=f;
	g=f-bix0;
	bix1=(bix0+1)%24;
	
	cra=btm_skycolor[bix0*3+0];
	cga=btm_skycolor[bix0*3+1];
	cba=btm_skycolor[bix0*3+2];

	crb=btm_skycolor[bix1*3+0];
	cgb=btm_skycolor[bix1*3+1];
	cbb=btm_skycolor[bix1*3+2];
	
	cr=((1.0-g)*cra)+(g*crb);
	cg=((1.0-g)*cga)+(g*cgb);
	cb=((1.0-g)*cba)+(g*cbb);

	pglClearColor(cr, cg, cb, 1.0);
	
	cy=(cr+2*cg+cb)/4;
	cy=cy*(1.0/((0.3+2*0.6+0.9)/4));
	
	i=16*cy;
	if(i<4)i=4;
	if(i>16)i=16;

	skybox_tex=0;
	skybox_onetex=0;
	if(i<=4)
	{
		skybox_tex=skybox_tex_stars;
		skybox_onetex=1;
	}

	wrl->daylight=i;
	return(0);
}

volatile int tt_ray;
volatile int tt_draw;
volatile int tt_drawblock;
volatile int tt_tick;

volatile int tt_rayframe;
volatile int tt_drawframe;

// BTM_World *btm_wrl;


int BTM_ConCmd_Noclip(BTM_ConCmd *cmd, char **args)
{
	btm_noclip=!btm_noclip;
	BTM_ConPrintf("Noclip %d\n", btm_noclip);
	return(0);
}

int BTM_ConCmd_Time(BTM_ConCmd *cmd, char **args)
{
	int hh, mm, hm, tt;

	if(args[1])
	{
		hm=atoi(args[1]);
		BTM_SetWorldTimeHhMm(btm_wrl, hm);

//		hh=hm/100;
//		mm=hm%100;

//		hh=hh-6;
//		if(hh<0)
//			hh+=24;
		
//		tt=((hh*60+mm)*1000);
//		btm_wrl->daytimer=tt;
	}else
	{
//		tt=btm_wrl->daytimer;
//		hh=tt/60000;
//		mm=(tt-(hh*60000))/1000;
		
//		hh+=6;
//		if(hh>=24)
//			hh-=24;
		
//		hm=hh*100+mm;

		hm=BTM_GetWorldTimeHhMm(btm_wrl);
		BTM_ConPrintf("Time %04d\n", hm);
	}

//	wrl->daytimer=(2*60*1000);
	return(0);
}

int BTM_ConCmd_Instance(BTM_ConCmd *cmd, char **args)
{
	char tb[64];
	char *sn1, *sn2, *s, *t;

	int cx, cy, cz;
	if(args[1])
	{
		s=args[1]; t=tb;
		while(*s && *s!='.')
			*t++=*s++;
		*t++=0;
		
		sn1=bccx_strdup(tb);
		sn2=NULL;
		
		if(*s=='.')
		{
			s++;
			t=tb;
			while(*s && *s!='.')
				*t++=*s++;
			*t++=0;
			sn2=bccx_strdup(tb);
		}
	
		cx=floor(cam_org[0]);
		cy=floor(cam_org[1]);
		cz=floor(cam_org[2])-2;
	
//		BTM_SetupLocalSeedXY(btm_wrl, cx, cy);
		BTM_InstanceStructureAt(btm_wrl,
			cx, cy, cz, sn1, sn2);
	}
	return(0);
}

int BTM_PlayerPhysTick(BTM_World *wrl, int dt)
{
	static int acdt;
	float dtf;
	
	if(btm_noclip)
		return(0);
	
	acdt+=dt;
	
	if(acdt<25)
		return(0);
	
	dtf=acdt/1000.0;
	BTMGL_LockWorld();
	BTM_CheckWorldMoveVel(wrl, dtf,
		cam_org, cam_vel,
		cam_org, cam_vel, &cam_mvflags);
	BTMGL_UnlockWorld();

	acdt=0;

	return(0);
}

int BTM_RayTick(void *ptr, int dt)
{
	static int wrldt;
	BTM_World *wrl;
	int t0, t1, tt;

	if(btm_dopause)
		return(0);

	wrl=ptr;

	wrldt+=dt;
	if(wrldt>5000)
	{
		BTMGL_LockWorld();
		BTM_CheckUnloadRegions(wrl);
		BTMGL_UnlockWorld();

		printf("Raycast %f%%\n", (100.0*tt_ray)/wrldt);
		printf("DrawBlk %f%%\n", (100.0*tt_drawblock)/wrldt);
		printf("Draw    %f%%\n", (100.0*tt_draw)/wrldt);
		printf("Tick    %f%%\n", (100.0*tt_tick)/wrldt);

		printf("Ray fps  %f\n", (1.0*tt_rayframe)/(wrldt/1000.0));
		printf("Draw fps %f\n", (1.0*tt_drawframe)/(wrldt/1000.0));

		tt_ray=0;
		tt_drawblock=0;
		tt_draw=0;
		tt_tick=0;

		tt_rayframe=0;
		tt_drawframe=0;

		wrldt=0;
	}

	BTM_PlayerPhysTick(wrl, dt);

#if 1
	t0=I_TimeMS();
	
//	BTMGL_LockWorld();

//	BTM_RaycastScene(wrl);
	BTM_RaycastSceneQuad(wrl);
	t1=I_TimeMS();
	tt_ray+=t1-t0;

//	memset(fbuf, 0, 320*200*2);
//	memset(fbuf, 0x7F, 320*200*2);

	t0=I_TimeMS();

	BTMGL_DrawSceneBlocks(wrl);

	BTMGL_DrawWorldEntities(wrl);

	t1=I_TimeMS();
	tt_drawblock+=t1-t0;
#endif

#if 1
	t1=I_TimeMS();
	tt_draw+=t1-t0;

	t0=I_TimeMS();

	BTM_BlockTickWorld(wrl, dt);

	t1=I_TimeMS();
	tt_tick+=t1-t0;
#endif

	tt_rayframe++;

	return(0);
}

int btm_raythreadproc(void *ptr)
{
	int tt, ltt, dt;

	ltt=I_TimeMS();
	while(!gfxdrv_kill)
	{
		tt=I_TimeMS();
		dt=tt-ltt;
		if(dt<32)
			continue;
		ltt=tt;

		BTM_RayTick(ptr, dt);
	}

	return(0);
}

int main(int argc, char *argv[])
{
	char tb[256];
	float tv0[3], tv1[3];
	TKRA_Context *ractx;
	BTM_World *wrl;
	BTM_MobEntity *mob;
//	BTM_Screen *scr;
//	BTM_TexImg *tex;
//	BTM_TexImg *skytex;
	
	int tt, ltt, dt, wrldt;
	int t0, t1, t2;
//	u32 *screen, *ct;
//	u16 *fbuf, *cs, *act;
	u16 *tex0;
	u16 *fbuf;
	byte *tbuf;
	u64 wpos;
	u32 tblk;
	int x, y, z, xs, ys, txs, tys;
	int mx, my, mb, lmb;
	int afrac, astep, mdl_ntris;
	int i, j, k, l;
	float f0, f1, f2, f3;
	float ang, dtf, frc;

	printf("Init VFS\n");

	FS_Init();

	printf("Def Cvar 1\n");

//	BTM_ConAddCommand("noclip", BTM_ConCmd_Noclip);
	BTM_ConAddCmdVar("noclip", BTM_ConCmd_Noclip, &btm_noclip, 0x3F);
	BTM_ConAddCommand("time", BTM_ConCmd_Time);
	BTM_ConAddCommand("instance", BTM_ConCmd_Instance);

	BTM_ConAddCvar("r_drawdist", &btm_drawdist, 0);
	BTM_ConAddCvar("mlook", &btm_mlook, 0x3F);

	BTM_ConAddCvar("vol_sfx", &tkm_vol_sfx,
		BTM_UITY_PERCENT256|BTM_CVTY_UBYTE);
	BTM_ConAddCvar("vol_music", &tkm_vol_music,
		BTM_UITY_PERCENT256|BTM_CVTY_UBYTE);

	printf("System Init 1\n");

	I_SystemInit();

	printf("System Init 2\n");

	BTM_PGL_InitOpenGlFuncs();

	printf("Init Skybox\n");

	BTMGL_InitSkybox();

	printf("Init TKRA\n");

	ractx=TKRA_AllocContext();
	TKRA_SetupScreen(ractx, 320, 200);
	TKRA_SetupForState(ractx);
	TKRA_SetCurrentContext(ractx);

	pglViewport(0, 0, 320, 200);

	printf("Load Textures\n");

#if 0
	tex0=BTIC1H_Img_LoadTGA5551("sky1b1.tga", &txs, &tys);

//	raimg=TKRA_GetTexImg(ractx, 1);
//	TKRA_UpdateTexImg(raimg, tex0, txs, tys, -1);
//	TKRA_BindTexImg(ractx, raimg);

	pglBindTexture(TKRA_TEXTURE_2D, 1);
	pglTexImage2D(TKRA_TEXTURE_2D, 0, 3, txs, tys, 0,
		TKRA_RGBA, TKRA_GL_UNSIGNED_SHORT_5_5_5_1, tex0);
#endif

//	tbuf=BTM_LoadFileTmp("gfx/dummy.dds", &j);

//	btmgl_filter_min=GL_NEAREST_MIPMAP_NEAREST;
	btmgl_filter_min=GL_NEAREST_MIPMAP_LINEAR;
//	btmgl_filter_min=GL_LINEAR_MIPMAP_NEAREST;
	btmgl_filter_max=GL_NEAREST;
//	btmgl_filter_max=GL_LINEAR;

#if 1
	j=0;
	skybox_tex_stars=1;
	tbuf=BTM_LoadFileTmp("gfx/sky2.dds", &j);
	if(tbuf)
	{
		pglBindTexture(TKRA_TEXTURE_2D, skybox_tex_stars);
		BTMGL_UploadCompressed(tbuf, 1, 1);
	}
#endif

//	for(y=0; y<256; y++)
//		for(x=0; x<256; x++)
//	{
//		z=btmra_morton8(x, y);
//		skytex->rgb[z]=tex0[(255-y)*256+x];
//	}

#if 0
//	tex0=BTIC1H_Img_LoadTGA5551("atlas0_far.tga", &txs, &tys);
	tex0=BTIC1H_Img_LoadTGA5551("atlas0_2a.tga", &txs, &tys);
	pglBindTexture(TKRA_TEXTURE_2D, 2);
	pglTexImage2D(TKRA_TEXTURE_2D, 0, 4, txs, tys, 0,
		TKRA_RGBA, TKRA_GL_UNSIGNED_SHORT_5_5_5_1, tex0);
#endif

	tbuf=BTM_LoadFileTmp("gfx/atlas0_2a.dds", &j);
//	tbuf=BTM_LoadFileTmp("gfx/atlas0_2b.dds", &j);
	if(tbuf)
	{
		pglBindTexture(TKRA_TEXTURE_2D, 2);
		BTMGL_UploadCompressed(tbuf, 1, 1);
	}

//	tex0=BTIC1H_Img_LoadTGA555("006.tga", &txs, &tys);
//	tex0=BTIC1H_Img_LoadTGA555("004.tga", &txs, &tys);
//	tex0=BTIC1H_Img_LoadTGA555("002.tga", &txs, &tys);

//	memset(tex->rgb, 0x7F, 64*64*2);
//	memcpy(tex->rgb, tex0, 64*64*2);
//	for(y=0; y<64; y++)
//		for(x=0; x<64; x++)
//	{
//		z=btmra_morton8(x, y);
//		tex->rgb[z]=tex0[(63-y)*64+x];
//	}

//	btm_drawdist=96;
//	btm_drawdist=128;
	btm_drawdist=BTM_TARGET_DRAWDIST;
//	btm_drawdist=192;

	BTM_RaycastInitTables();

//	wrl=BTM_AllocWorld(7, 4);
//	wrl=BTM_AllocWorld(8, 6);
	wrl=BTM_AllocWorld(7, 7);

	btm_wrl=wrl;

	BTM_SetupWorldSeed(wrl, 0x12345678ULL);

#if 0
	for(j=0; j<16; j++)
	{
		wrl->tg_nmap[j]=malloc(256);
		for(i=0; i<256; i++)
			wrl->tg_nmap[j][i]=rand();
	}
#endif
	
//	wrl_zmap=malloc(256*256);

	printf("Generate Terrain\n");

//	for(i=0; i<256; i++)
//		for(j=0; j<256; j++)
//	{
//		BTM_GenerateBaseXY(wrl, i, j);
//	}

//	scr=BTMRA_AllocScreen(320, 200);
//	tex=BTMRA_AllocTexture(6);
//	skytex=BTMRA_AllocTexture(8);

//	BTM_UpdateWorldBlockOcc(wrl);

	printf("Go\n");

//	I_SystemInit();

	xs=320;
	ys=200;
//	fbuf=malloc(xs*ys*sizeof(u16));
//	fbuf=scr->rgb;
	fbuf=ractx->screen_rgb;


	x=128<<8;
	y=128<<8;
	z=32<<8;

	wpos=(((u64)x)<<0)|(((u64)y)<<24)|(((u64)z)<<48);
	wrl->cam_org=wpos;


	x=0<<8;
	y=4<<8;
	z=0<<8;

	wpos=(((u64)x)<<0)|(((u64)y)<<24)|(((u64)z)<<48);

	cam_ang_yaw=0;
	cam_ang_pitch=0;
	
	cam_org[0]=128;
	cam_org[1]=128;
	cam_org[2]=100;

//	pglDepthRange(0.0, 1.0);
	pglDepthRange(0.0, 0.75);
	pglClearDepth(0.999);

	pglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	pglBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
	pglAlphaFunc(GL_GEQUAL, 0.5);
	
	pglClearColor(0.3, 0.6, 0.9, 1.0);
	wrl->daytimer=360;

//	BTM_LoadMenu("dialog/mainmenu0.xml");
	BTM_LoadMenu("dialog/menus.xml");
	
	BTM_ConPrintString("Console String\n");

	wrl->cam_org=
		((u64)((u32)(cam_org[0]*256)&0xFFFFFFU)<< 0) |
		((u64)((u32)(cam_org[1]*256)&0xFFFFFFU)<<24) |
		((u64)((u32)(cam_org[2]*256)&0x00FFFFU)<<48) ;

	wrl->cam_yaw=(byte)(cam_ang_yaw*(256.0/360.0));
	wrl->cam_pitch=(byte)(cam_ang_pitch*(256.0/360.0));

	BTM_LoadWorldGlobals(wrl);

#ifdef BTM_TARGET_SMALL
	if(btm_drawdist>BTM_TARGET_DRAWDIST)
		btm_drawdist=BTM_TARGET_DRAWDIST;
#endif

	cam_org[0]=((wrl->cam_org>> 0)&0x00FFFFFFU)*(1.0/256);
	cam_org[1]=((wrl->cam_org>>24)&0x00FFFFFFU)*(1.0/256);
	cam_org[2]=((wrl->cam_org>>48)&0x0000FFFFU)*(1.0/256);

	cam_ang_yaw=wrl->cam_yaw*(360.0/256.0);
	cam_ang_pitch=wrl->cam_pitch*(360.0/256.0);
	
	if(wrl->cam_flags&1)
		btm_noclip=1;
	if(wrl->cam_flags&2)
		btm_mlook=1;

//	BTM_PlaySong("music/PiTink1.mod");
//	BTM_PlaySong("music/Life2.mod");
//	BTM_PlaySong("music/Life3.s3m");
	BTM_PlaySong("music/musix-shine.mod");
//	BTM_PlaySong("music/kc-techrockloop.s3m");
//	BTM_PlaySong("music/kj_jose_-_a_new_frontend.s3m");
//	BTM_PlaySong("music/Noise0.s3m");

#ifdef BTM_RAYTHREAD
	thThread(btm_raythreadproc, wrl);
#endif

	mx=0; my=0;
	mb=0;

	wrldt=0;
	tt=I_TimeMS();
	ltt=tt;
	while(!gfxdrv_kill)
	{
		tt=I_TimeMS();
		dt=tt-ltt;

//		if(!dt)
		if(dt<8)
			{ continue; }

		ltt=tt;

		btmgl_time_ms=tt;

// #ifndef BTM_RAYTHREAD
#if 0
		wrldt+=dt;
		if(wrldt>5000)
		{
			BTMGL_LockWorld();
			BTM_CheckUnloadRegions(wrl);
			BTMGL_UnlockWorld();

			printf("Raycast %f%%\n", (100.0*tt_ray)/wrldt);
			printf("DrawBlk %f%%\n", (100.0*tt_drawblock)/wrldt);
			printf("Draw    %f%%\n", (100.0*tt_draw)/wrldt);
			printf("Tick    %f%%\n", (100.0*tt_tick)/wrldt);

			tt_ray=0;
			tt_drawblock=0;
			tt_draw=0;
			tt_tick=0;

			wrldt=0;
		}
#endif
		
		if(dt>500)
			dt=500;

		tt_drawframe++;

		I_BeginFrame(fbuf, xs, ys);

		I_UpdateKeys();

		if(I_KeyDown(K_ESC) && !I_KeyDownL(K_ESC))
		{
			BTM_ToggleMenu();
			BTM_PlaySample("sound/player/step.wav", 64);
		}
		
		BTM_MenuDoInput(dt);
		BTM_ConDoInput(dt);
		BTM_InvenDoInput(dt);
		BTM_DoMixSamples(dt);

		lmb=mb;
		GfxDrv_MouseGetPos(&mx, &my, &mb);

		BTM_InterpolateDaySky(wrl);

		wrl->cam_flags&=~3;
		if(btm_noclip)
			wrl->cam_flags|=1;
		if(btm_mlook)
			wrl->cam_flags|=2;

		btm_dopause=1;
		if(!BTM_MenuDownP() && !BTM_ConDownP() && !BTM_InvenOpenP())
		{
			wrl->daytimer+=dt;
			btm_dopause=0;
		
//			if(mb&2)
			if(btm_mlook)
			{
//				if(lmb&2)
				if(btm_lmlook)
				{
					cam_ang_yaw+=mx*0.5;
					cam_ang_pitch-=my*0.5;
				}
				GfxDrv_MouseSetPos(0, 0);
			}

			if((mb&2) && !(lmb&2))
			{
				btm_mlook=!btm_mlook;
			}
			
			btm_lmlook=btm_mlook;
		
			if(I_KeyDown('\\'))
			{
				if(wrl->scr_lhit)
				{
					BTMGL_LockWorld();
					wrl->sel_blk=BTM_GetWorldBlockCix(wrl, wrl->scr_lhit);
					wrl->sel_bt=wrl->sel_blk&255;
					BTMGL_UnlockWorld();
				}
			}

			if(I_KeyDown('[') && !I_KeyDownL('['))
			{
				wrl->sel_bt=(wrl->sel_bt-1)&255;
			}

			if(I_KeyDown(']') && !I_KeyDownL(']'))
			{
				wrl->sel_bt=(wrl->sel_bt+1)&255;
			}

			if((I_KeyDown(K_INS) && !I_KeyDownL(K_INS)) ||
				((mb&4) && !(lmb&4)))
			{
				if(wrl->scr_lahit && (wrl->sel_bt>=4))
				{
					tblk=wrl->sel_bt;
					
					if(BTM_BlockIsOrientedP(wrl, tblk))
					{
						switch(((int)(cam_ang_yaw/45))&7)
						{
							case 7:		case 0:
								tblk|=0<<8; break;
							case 1:		case 2:
								tblk|=1<<8; break;
							case 3:		case 4:
								tblk|=2<<8; break;
							case 5:		case 6:
								tblk|=3<<8; break;
						}
					}
				
					BTMGL_LockWorld();
					BTM_SetWorldBlockCix(wrl, wrl->scr_lahit, tblk);
					BTM_UpdateWorldBlockOccCix2(wrl, wrl->scr_lahit);
					BTMGL_UnlockWorld();
				}
			}

	//		if(I_KeyDown('=') && !I_KeyDownL('='))
			if(I_KeyDown(K_DEL) && !I_KeyDownL(K_DEL))
			{
				if(wrl->scr_lhit && (wrl->sel_bt>=4))
				{
					BTMGL_LockWorld();
					BTM_SetWorldBlockCix(wrl, wrl->scr_lhit, wrl->sel_bt);
					BTM_UpdateWorldBlockOccCix2(wrl, wrl->scr_lhit);
					BTMGL_UnlockWorld();
				}
			}

	//		if(I_KeyDown(K_DEL) && !I_KeyDownL(K_DEL))
			if((I_KeyDown(K_BACKSPACE) && !I_KeyDownL(K_BACKSPACE)) ||
				((mb&1) && !(lmb&1)))
			{
				if(wrl->scr_lhit)
				{
					BTMGL_LockWorld();
					BTM_SetWorldBlockCix(wrl, wrl->scr_lhit, BTM_BLKTY_AIR2);
					BTM_UpdateWorldBlockOccCix2(wrl, wrl->scr_lhit);
					BTMGL_UnlockWorld();
				}
			}

			if(I_KeyDown(K_ENTER) && !I_KeyDownL(K_ENTER))
			{
				BTMGL_LockWorld();

				mob=BTM_QueryWorldEntityForRay(wrl,
					wrl->scr_laspos, wrl->scr_laepos);
				if(mob)
				{
					BTM_EventPlayerUseMob(wrl, mob);
				}

				BTMGL_UnlockWorld();
			}

			if(!I_KeyDown(K_SHIFT) && !btm_mlook)
			{
				if(I_KeyDown(K_LEFTARROW))
					cam_ang_yaw-=dt*(90/1000.0);
				if(I_KeyDown(K_RIGHTARROW))
					cam_ang_yaw+=dt*(90/1000.0);
			}
			
			if(I_KeyDown(K_PGDN))
				cam_ang_pitch-=dt*(90/1000.0);
			if(I_KeyDown(K_PGUP))
				cam_ang_pitch+=dt*(90/1000.0);

			if(btm_noclip)
			{
				if(I_KeyDown(K_HOME))
					cam_org[2]+=dt*(12/1000.0);
				if(I_KeyDown(K_END))
					cam_org[2]-=dt*(12/1000.0);

				if(I_KeyDown(K_UPARROW) ||
						I_KeyDown('w') ||
						I_KeyDown('W'))
				{
					cam_org[0]+=dt*(12/1000.0)*sin(cam_ang_yaw*(M_PI/180));
					cam_org[1]-=dt*(12/1000.0)*cos(cam_ang_yaw*(M_PI/180));
				}
				if(I_KeyDown(K_DOWNARROW) ||
						I_KeyDown('s') ||
						I_KeyDown('S'))
				{
					cam_org[0]-=dt*(12/1000.0)*sin(cam_ang_yaw*(M_PI/180));
					cam_org[1]+=dt*(12/1000.0)*cos(cam_ang_yaw*(M_PI/180));
				}
				
				if(I_KeyDown(K_SHIFT) || btm_lmlook)
				{
					if(I_KeyDown(K_LEFTARROW) ||
						I_KeyDown('a') ||
						I_KeyDown('A'))
					{
						cam_org[0]-=dt*(12/1000.0)*
							cos(cam_ang_yaw*(M_PI/180));
						cam_org[1]-=dt*(12/1000.0)*
							sin(cam_ang_yaw*(M_PI/180));
					}
					if(I_KeyDown(K_RIGHTARROW) ||
						I_KeyDown('d') ||
						I_KeyDown('D'))
					{
						cam_org[0]+=dt*(12/1000.0)*
							cos(cam_ang_yaw*(M_PI/180));
						cam_org[1]+=dt*(12/1000.0)*
							sin(cam_ang_yaw*(M_PI/180));
					}
				}

			}else
			{
				cam_ivel[0]=0;
				cam_ivel[1]=0;
				cam_ivel[2]=0;

				dtf=(dt/1000.0);
			
	//			if(dtf>0.1)
	//				cam_vel[2]-=5*dtf;
	//			else
					cam_vel[2]-=16*dtf;
				
				if(cam_mvflags&4)
				{
					frc=1.0-4*dtf;
					cam_vel[2]=cam_vel[2]*frc+26*dtf;
				}else
					if(cam_mvflags&2)
				{
					frc=1.0-2*dtf;
					cam_vel[2]=cam_vel[2]*frc+20*dtf;
				}

				
				if(I_KeyDown(K_HOME))
				{
					if(cam_mvflags&1)
					{
						BTM_PlaySample("sound/player/jump1.wav", 64);

						if(cam_vel[2]<0)
							cam_vel[2]=0;
						cam_vel[2]+=6;
						cam_mvflags&=~1;
					}else if(cam_mvflags&2)
					{
						cam_ivel[2]=12;
					}
	//				else
	//					cam_ivel[2]=12;
				}
				if(I_KeyDown(K_END))
				{
					cam_ivel[2]=-12;
				}

				if(I_KeyDown(K_SHIFT))
				{
					if(I_KeyDown(K_UPARROW) ||
						I_KeyDown('W'))
					{
						cam_ivel[0]= 14*sin(cam_ang_yaw*(M_PI/180));
						cam_ivel[1]=-14*cos(cam_ang_yaw*(M_PI/180));
					}
					if(I_KeyDown(K_DOWNARROW) ||
						I_KeyDown('S'))
					{
						cam_ivel[0]=-14*sin(cam_ang_yaw*(M_PI/180));
						cam_ivel[1]= 14*cos(cam_ang_yaw*(M_PI/180));
					}
				}else
				{
					if(I_KeyDown(K_UPARROW) ||
						I_KeyDown('w'))
					{
						cam_ivel[0]= 7*sin(cam_ang_yaw*(M_PI/180));
						cam_ivel[1]=-7*cos(cam_ang_yaw*(M_PI/180));
					}
					if(I_KeyDown(K_DOWNARROW) ||
						I_KeyDown('s'))
					{
						cam_ivel[0]=-7*sin(cam_ang_yaw*(M_PI/180));
						cam_ivel[1]= 7*cos(cam_ang_yaw*(M_PI/180));
					}
				}
				
				if(I_KeyDown(K_SHIFT) || btm_lmlook)
				{
					if(I_KeyDown(K_LEFTARROW) ||
						I_KeyDown('a') ||
						I_KeyDown('A'))
					{
						cam_ivel[0]=-7*cos(cam_ang_yaw*(M_PI/180));
						cam_ivel[1]=-7*sin(cam_ang_yaw*(M_PI/180));
					}
					if(I_KeyDown(K_RIGHTARROW) ||
						I_KeyDown('d') ||
						I_KeyDown('D'))
					{
						cam_ivel[0]= 7*cos(cam_ang_yaw*(M_PI/180));
						cam_ivel[1]= 7*sin(cam_ang_yaw*(M_PI/180));
					}
				}

				frc=1.0-8*dtf;

				if(cam_mvflags&2)
					frc=1.0-1*dtf;


				if(frc<0)
					frc=0;

				if(TKRA_Vec3F_DotProduct(cam_vel, cam_vel)<0.2)
					frc=0;

				TKRA_Vec3F_Normalize(cam_vel, tv0);
				f0=TKRA_Vec3F_Normalize(cam_ivel, tv1);
				f2=TKRA_Vec3F_DotProduct(tv0, tv1);

				if(cam_mvflags&1)
				{
//					f0=TKRA_Vec3F_DotProduct(cam_vel, cam_ivel);
//					f1=TKRA_Vec3F_DotProduct(cam_vel, cam_vel)+0.01;
//					f2=f0/f1;
				
//					if(f2<=0.9)
//					if(f2<=0.7)
					if(f2<=0.8)
//					if(f2<=0.3)
//					if(f0<1.0)
					{
						cam_vel[0]*=frc;
						cam_vel[1]*=frc;
	//					TKRA_Vec3F_Scale(cam_vel, frc, cam_vel);
					}else
					{
						f3=sqrt(f2);
//						cam_vel[0]*=f2;
//						cam_vel[1]*=f2;
						cam_vel[0]*=f3;
						cam_vel[1]*=f3;
					}
				}else
					if(cam_mvflags&2)
				{
//					f0=TKRA_Vec3F_DotProduct(cam_vel, cam_ivel);
//					f1=TKRA_Vec3F_DotProduct(cam_vel, cam_vel)+0.01;
//					f2=f0/f1;
				
					if(f2<=0.9)
//					if(f2<=0.3)
					{
						cam_vel[0]*=frc;
						cam_vel[1]*=frc;
	//					TKRA_Vec3F_Scale(cam_vel, frc, cam_vel);
					}
				}
				
				if(TKRA_Vec3F_DotProduct(cam_vel, cam_ivel)<49)
				{
					if(cam_mvflags&1)
					{
						cam_vel[0]+=cam_ivel[0]*dtf*4.0;
						cam_vel[1]+=cam_ivel[1]*dtf*4.0;
						cam_vel[2]+=cam_ivel[2]*dtf*4.0;
					}else
						if(cam_mvflags&2)
					{
						cam_vel[0]+=cam_ivel[0]*dtf*1.0;
						cam_vel[1]+=cam_ivel[1]*dtf*1.0;
						cam_vel[2]+=cam_ivel[2]*dtf*1.0;
					}else
					{
						cam_vel[0]+=cam_ivel[0]*dtf*0.1;
						cam_vel[1]+=cam_ivel[1]*dtf*0.1;
						cam_vel[2]+=cam_ivel[2]*dtf*0.1;
					}
				}
				
//				BTM_PlayerPhysTick(wrl, dt);
				
//				BTM_CheckWorldMoveVel(wrl, dtf,
//					cam_org, cam_vel,
//					cam_org, cam_vel, &cam_mvflags);
			}
		}
		
		if(I_CheckCheatStr("idclip"))
		{
			btm_noclip=!btm_noclip;
			printf("Noclip %d\n", btm_noclip);
		}

		if(I_CheckCheatStr("idday"))
		{
			wrl->daytimer=(2*60*1000);
		}

		if(I_CheckCheatStr("idnight"))
		{
			wrl->daytimer=((12+2)*60*1000);
		}
		
//		if(cam_org[0]<0)
//			{ cam_org[0]+=1<<wrl->xsh; }
//		if(cam_org[1]<0)
//			{ cam_org[1]+=1<<wrl->xsh; }
//		if(cam_org[0]>=(1<<wrl->xsh))
//			{ cam_org[0]-=1<<wrl->xsh; }
//		if(cam_org[1]>=(1<<wrl->xsh))
//			{ cam_org[1]-=1<<wrl->xsh; }

		if(cam_org[0]<0)
			{ cam_org[0]+=1<<16; }
		if(cam_org[1]<0)
			{ cam_org[1]+=1<<16; }
		if(cam_org[0]>=(1<<16))
			{ cam_org[0]-=1<<16; }
		if(cam_org[1]>=(1<<16))
			{ cam_org[1]-=1<<16; }

		if(cam_org[2]<0)
			{ cam_org[2]=0; }
//		if(cam_org[2]>(1<<wrl->zsh))
		if(cam_org[2]>128)
			{ cam_org[2]=128; }
			
		if(cam_ang_pitch>85)
			cam_ang_pitch=85;
		if(cam_ang_pitch<-85)
			cam_ang_pitch=-85;
		
		wrl->cam_yaw=(byte)(cam_ang_yaw*(256.0/360.0));
		wrl->cam_pitch=(byte)(cam_ang_pitch*(256.0/360.0));
		
		wrl->cam_fw[0]=
			sin(cam_ang_yaw  *(M_PI/180))*
			cos(cam_ang_pitch*(M_PI/180));
		wrl->cam_fw[1]=
			-cos(cam_ang_yaw*(M_PI/180))*
			 cos(cam_ang_pitch*(M_PI/180));
		wrl->cam_fw[2]=
			 sin(cam_ang_pitch*(M_PI/180));

		wrl->cam_rt[0]=
			cos(cam_ang_yaw*(M_PI/180));
		wrl->cam_rt[1]=
			sin(cam_ang_yaw*(M_PI/180));
		wrl->cam_rt[2]=0;


		wrl->cam_up[0]=
			 sin(cam_ang_yaw  *(M_PI/180))*
			-sin(cam_ang_pitch*(M_PI/180));
		wrl->cam_up[1]=
			-cos(cam_ang_yaw*(M_PI/180))*
			-sin(cam_ang_pitch*(M_PI/180));
		wrl->cam_up[2]=
			 cos(cam_ang_pitch*(M_PI/180));
		
//		wrl->cam_org=
//			((u64)((u16)(cam_org[0]*256))<< 0) |
//			((u64)((u16)(cam_org[1]*256))<<24) |
//			((u64)((u16)(cam_org[2]*256))<<48) ;

		wrl->cam_org=
			((u64)((u32)(cam_org[0]*256)&0xFFFFFFU)<< 0) |
			((u64)((u32)(cam_org[1]*256)&0xFFFFFFU)<<24) |
			((u64)((u32)(cam_org[2]*256)&0x00FFFFU)<<48) ;

		wrl->cam_vel=
			((u64)((u32)(cam_vel[0]*256)&0xFFFFFFU)<< 0) |
			((u64)((u32)(cam_vel[1]*256)&0xFFFFFFU)<<24) |
			((u64)((u32)(cam_vel[2]*256)&0x00FFFFU)<<48) ;
		
		BTM_SoundSetVpos(
			wrl->cam_org, wrl->cam_vel,
			wrl->cam_yaw, wrl->cam_pitch);
		
#ifndef BTM_RAYTHREAD
		BTM_RayTick(wrl, dt);
#endif

#if 0
		t0=I_TimeMS();
		
//		BTM_RaycastScene(wrl);
		BTM_RaycastSceneQuad(wrl);
		t1=I_TimeMS();
		tt_ray+=t1-t0;

//		memset(fbuf, 0, 320*200*2);
//		memset(fbuf, 0x7F, 320*200*2);

		t0=I_TimeMS();

		BTMGL_DrawSceneBlocks(wrl);

		t1=I_TimeMS();
		tt_drawblock+=t1-t0;
#endif

		t0=I_TimeMS();

// 		pglClear(TKRA_GL_DEPTH_BUFFER_BIT);
		pglClear(TKRA_GL_DEPTH_BUFFER_BIT | TKRA_GL_COLOR_BUFFER_BIT);

		pglMatrixMode(TKRA_MODELVIEW);
		pglLoadIdentity();

#if 0
		pglRotatef(90, 1, 0, 0);
		pglRotatef(cam_ang_pitch, 1, 0, 0);

		pglRotatef(-cam_ang_yaw, 0, 0, 1);
		pglTranslatef(-cam_org[0], -cam_org[1], -cam_org[2]);
#endif

		pglMatrixMode(TKRA_PROJECTION);
		pglLoadIdentity();
//		pglFrustum(-1, 1, -0.75, 0.75, 1.0, 1024);
//		pglFrustum(-1, 1,  0.75, -0.75, 1.0, 1024);
//		pglFrustum(-1*0.5, 1*0.5,  0.75*0.5, -0.75*0.5, 1.0*0.5, 1024*1.0);
		pglFrustum(-0.1, 0.1,  0.075, -0.075, 0.1, 1024);

#if 1
		pglRotatef(90, 1, 0, 0);
		pglRotatef(cam_ang_pitch, 1, 0, 0);

		pglRotatef(-cam_ang_yaw, 0, 0, 1);
		pglTranslatef(-cam_org[0], -cam_org[1], -cam_org[2]);
#endif

		pglDisable(GL_CULL_FACE);
		pglDepthFunc(GL_LEQUAL);
		pglEnable(GL_DEPTH_TEST);
//		pglEnable(GL_BLEND);
		pglDisable(GL_BLEND);

//		BTMGL_DrawSkybox();

		pglDisable(GL_BLEND);

//		BTMGL_DrawSceneBlocks(wrl);
		BTMGL_DrawSceneArrays(wrl);
//		BTMGL_DrawWorldEntities(wrl);
		BTMGL_DrawVisibleSprites(wrl);


#if 0
		BTMRA_DrawSky(scr, skytex, 0);

//		BTMRA_DrawWall(scr, tex, wpos, tt/8);
//		BTMRA_DrawWall(scr, tex, wpos+(2<<8), tt/8);

		BTMRA_DrawBox(scr, tex, wpos, tt/32, 0x3F);

		BTMRA_DrawBox(scr, tex, wpos-(((u64)1)<<56), tt/32, 0x3F);
		BTMRA_DrawBox(scr, tex, wpos+(((u64)1)<<56), tt/32, 0x3F);
#endif


		pglMatrixMode(TKRA_MODELVIEW);
		pglLoadIdentity();

		pglMatrixMode(TKRA_PROJECTION);
		pglLoadIdentity();
//		pglOrtho(-160, 160, 100, -100, -999.0, 999.0);
		pglOrtho(-160, 160, -100, 100, -999.0, 999.0);

		pglDisable(GL_CULL_FACE);
		pglDisable(GL_DEPTH_TEST);
		pglDisable(GL_BLEND);
		pglEnable(GL_ALPHA_TEST);

		j=wrl->sel_bt;
		k=btmgl_vox_atlas_side[j&255];
		x=k&15;
		y=(k>>4)&15;
//		y=15-((j>>4)&15);

		f0=(x+0)*(1.0/16)+(1.0/128);
		f1=(x+1)*(1.0/16)-(1.0/128);
		f2=(y+0)*(1.0/16)+(1.0/128);
		f3=(y+1)*(1.0/16)-(1.0/128);

		pglColor4f(1.0, 1.0, 1.0, 1.0);
		pglBindTexture(GL_TEXTURE_2D, 2);
		pglBegin(GL_QUADS);
		pglTexCoord2f(f0, f2);
		pglVertex2f(64, -66);
		pglTexCoord2f(f0, f3);
		pglVertex2f(64, -98);
		pglTexCoord2f(f1, f3);
		pglVertex2f(96, -98);
		pglTexCoord2f(f1, f2);
		pglVertex2f(96, -66);

		pglEnd();
		
		sprintf(tb, "%02X\n%s", wrl->sel_bt,
			BTM_BlockMiniDesc(wrl, wrl->sel_bt));
		BTM_DrawString8px(96, -74, tb, 0xFFFFFFFFU);

		BTM_DrawInventory(wrl);
		BTM_DrawMenu();
		BTM_DrawConsole();

		sprintf(tb, "%03d", 1000/dt);
		BTM_DrawString8px(160-24, 100-8, tb, 0xFFFFFFFFU);

		sprintf(tb, "%04d", dt);
		BTM_DrawString8px(160-32, 100-16, tb, 0xFFFFFFFFU);

		sprintf(tb, "%05d", (int)floor(cam_org[0]));
		BTM_DrawString8px(160-40, 100-24, tb, 0xFFFFFFFFU);
		sprintf(tb, "%05d", (int)floor(cam_org[1]));
		BTM_DrawString8px(160-40, 100-32, tb, 0xFFFFFFFFU);
		sprintf(tb, "%03d", (int)floor(cam_org[2]));
		BTM_DrawString8px(160-24, 100-40, tb, 0xFFFFFFFFU);

		BTM_DrawString8px(160-24, 100-40, NULL, 0xFFFFFFFFU);

#if 0
		l=1000/dt;
		BTM_DrawCharFBuf(fbuf, 320, 320-24, 0, '0'+((l/100)%10), 0x7FFF, 0x0000);
		BTM_DrawCharFBuf(fbuf, 320, 320-16, 0, '0'+((l/10)%10), 0x7FFF, 0x0000);
		BTM_DrawCharFBuf(fbuf, 320, 320- 8, 0, '0'+(l%10), 0x7FFF, 0x0000);

		l=dt;
		BTM_DrawCharFBuf(fbuf, 320, 320-32, 8,
			'0'+((l/1000)%10), 0x7FFF, 0x0000);
		BTM_DrawCharFBuf(fbuf, 320, 320-24, 8, '0'+((l/100)%10), 0x7FFF, 0x0000);
		BTM_DrawCharFBuf(fbuf, 320, 320-16, 8, '0'+((l/10)%10), 0x7FFF, 0x0000);
		BTM_DrawCharFBuf(fbuf, 320, 320- 8, 8, '0'+(l%10), 0x7FFF, 0x0000);

		l=(u16)(cam_org[0]);
		BTM_DrawCharFBuf(fbuf, 320, 320-40, 16, '0'+((l/10000)%10), 0x7FFF, 0x0000);
		BTM_DrawCharFBuf(fbuf, 320, 320-32, 16, '0'+((l/1000)%10), 0x7FFF, 0x0000);
		BTM_DrawCharFBuf(fbuf, 320, 320-24, 16,
			'0'+((l/100)%10), 0x7FFF, 0x0000);
		BTM_DrawCharFBuf(fbuf, 320, 320-16, 16, '0'+((l/10)%10), 0x7FFF, 0x0000);
		BTM_DrawCharFBuf(fbuf, 320, 320- 8, 16, '0'+(l%10), 0x7FFF, 0x0000);

		l=(u16)(cam_org[1]);
		BTM_DrawCharFBuf(fbuf, 320, 320-40, 24, '0'+((l/10000)%10), 0x7FFF, 0x0000);
		BTM_DrawCharFBuf(fbuf, 320, 320-32, 24, '0'+((l/1000)%10), 0x7FFF, 0x0000);
		BTM_DrawCharFBuf(fbuf, 320, 320-24, 24, '0'+((l/100)%10), 0x7FFF, 0x0000);
		BTM_DrawCharFBuf(fbuf, 320, 320-16, 24, '0'+((l/10)%10), 0x7FFF, 0x0000);
		BTM_DrawCharFBuf(fbuf, 320, 320- 8, 24, '0'+(l%10), 0x7FFF, 0x0000);

		l=(u16)(cam_org[2]);
		BTM_DrawCharFBuf(fbuf, 320, 320-24, 32, '0'+((l/100)%10), 0x7FFF, 0x0000);
		BTM_DrawCharFBuf(fbuf, 320, 320-16, 32, '0'+((l/10)%10), 0x7FFF, 0x0000);
		BTM_DrawCharFBuf(fbuf, 320, 320- 8, 32, '0'+(l%10), 0x7FFF, 0x0000);

//		pglVertexPointer(3, TKRA_GL_FLOAT, 6*4, mdl_tris+0);
//		pglTexCoordPointer(2, TKRA_GL_FLOAT, 6*4, mdl_tris+3);
//		pglColorPointer(4, TKRA_GL_UNSIGNED_BYTE, 6*4, mdl_tris+5);
//		pglDrawArrays(TKRA_GL_TRIANGLES, 0, mdl_ntris*3);

#endif


		I_SystemFrame(fbuf, xs, ys);
	}
	return(0);
}
