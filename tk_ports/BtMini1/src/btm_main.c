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


#ifdef _WIN32

u32 d_16to24table[65536];

int I_SystemInit()
{
	int cr, cg, cb;
	int i, j, k;

	for(i=0; i<65536; i++)
	{
		cr=(i>>10)&31;	cg=(i>>5)&31;	cb=(i>>0)&31;
		cr=(cr<<3)|(cr>>2);
		cg=(cg<<3)|(cg>>2);
		cb=(cb<<3)|(cb>>2);
		j=0xFF000000|(cb<<16)|(cg<<8)|(cr<<0);
		d_16to24table[i]=j;
	}

	SoundDev_Init();
	GfxDrv_Start();
	
	btesh2_gfxcon_fbxs=640;
	btesh2_gfxcon_fbys=400;
	
	GfxDrv_PrepareFramebuf();
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

	GfxDrv_BeginDrawing();
	
	for(y=0; y<ys; y++)
	{
//		cs=fbuf+(ys-y-1)*xs;
		cs=fbuf+y*xs;
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

	GfxDrv_EndDrawing();
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


u16 i_keyrov[256];
byte i_keypos;

void I_UpdateKeys (void)
{
	u16 *cs;
	int key;
	
	cs=FRGL_GetKeybuf();
	while(*cs)
	{
		key=*cs++;
		if(!(key&0x8000))
			i_keyrov[i_keypos++]=key;
	}
}
#endif

#ifdef __BJX2__

int gfxdrv_kill=0;

int I_SystemInit()
{
//	SoundDev_Init();

	conbufa=(u32 *)0xF00A0000;
	((u32 *)0xF00BFF00)[0]=0x0095;		//320x200x16bpp, RGB555
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

int I_SystemFrame(u16 *fbuf, int xs, int ys)
{
	u64 *ct, *cte, ct1, *cs0, *cs1, *cs2, *cs3;
	int x, y;

//	SoundDev_WriteStereoSamples(tsampbuf, tsamplen);
//	SoundDev_Submit();

	ct=(u64 *)conbufa;
	
	for(y=0; y<ys; y+=4)
	{
#if 1
		if(xs==320)
		{
//			cs0=(u64 *)(fbuf+((ys-y-1)*xs));
//			cs3=(u64 *)(fbuf+((ys-y-4)*xs));
//			I_FinishUpdate_ScanCopyVf(cs3, ct, 80);

			cs0=(u64 *)(fbuf+(y*xs));
			I_FinishUpdate_ScanCopy(cs0, ct, 80);

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

	((u32 *)0xF00BFF00)[8]=vid_frnum;
	vid_frnum++;
}

unsigned int TK_GetTimeMs(void);

int I_TimeMS()
{
	return(TK_GetTimeMs());
}

u32 i_keymask[8];
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

float	skybox_tris[6*6*6];

float cam_ang_yaw;
float cam_ang_pitch;
float cam_org[3];
float cam_vel[3];
float cam_ivel[3];
int cam_mvflags;

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
	rgb0=(u32 *)(skybox_tris+5);
	st0=xyz0+3;
	
	for(i=0; i<6; i++)
	{
		TKRA_Vec3F_Copy(vtx_xyz+cube_tris[i*6+0]*4, xyz0+(i*6+0)*6);
		TKRA_Vec3F_Copy(vtx_xyz+cube_tris[i*6+1]*4, xyz0+(i*6+1)*6);
		TKRA_Vec3F_Copy(vtx_xyz+cube_tris[i*6+2]*4, xyz0+(i*6+2)*6);
		TKRA_Vec3F_Copy(vtx_xyz+cube_tris[i*6+3]*4, xyz0+(i*6+3)*6);
		TKRA_Vec3F_Copy(vtx_xyz+cube_tris[i*6+4]*4, xyz0+(i*6+4)*6);
		TKRA_Vec3F_Copy(vtx_xyz+cube_tris[i*6+5]*4, xyz0+(i*6+5)*6);

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

		st0[(i*6+0)*6+0]=0.0+sofs;		st0[(i*6+0)*6+1]=0.0+tofs;
		st0[(i*6+1)*6+0]=0.5+sofs;		st0[(i*6+1)*6+1]=0.0+tofs;
		st0[(i*6+2)*6+0]=0.5+sofs;		st0[(i*6+2)*6+1]=0.5+tofs;
		st0[(i*6+3)*6+0]=0.0+sofs;		st0[(i*6+3)*6+1]=0.0+tofs;
		st0[(i*6+4)*6+0]=0.5+sofs;		st0[(i*6+4)*6+1]=0.5+tofs;
		st0[(i*6+5)*6+0]=0.0+sofs;		st0[(i*6+5)*6+1]=0.5+tofs;
		
		for(j=0; j<6; j++)
			st0[(i*6+j)*6+1]=1.0-st0[(i*6+j)*6+1];
		
		rgb0[(i*6+0)*6]=0xFFFFFFFFU;
		rgb0[(i*6+1)*6]=0xFFFFFFFFU;
		rgb0[(i*6+2)*6]=0xFFFFFFFFU;
		rgb0[(i*6+3)*6]=0xFFFFFFFFU;
		rgb0[(i*6+4)*6]=0xFFFFFFFFU;
		rgb0[(i*6+5)*6]=0xFFFFFFFFU;
	}
}

int BTMGL_DrawSkybox()
{
	tkra_glMatrixMode(TKRA_MODELVIEW);
	tkra_glPushMatrix();

	tkra_glTranslatef(cam_org[0], cam_org[1], cam_org[2]);

	tkra_glBindTexture(TKRA_TEXTURE_2D, 1);

	tkra_glVertexPointer(3, TKRA_GL_FLOAT, 6*4, skybox_tris+0);
	tkra_glTexCoordPointer(2, TKRA_GL_FLOAT, 6*4, skybox_tris+3);
	tkra_glColorPointer(4, TKRA_GL_UNSIGNED_BYTE, 6*4, skybox_tris+5);
//	tkra_glDrawArrays(TKRA_GL_TRIANGLES, 0, 6*6);
	tkra_glDrawArrays(TKRA_GL_TRIANGLES, 0, 6*5);

//	tkra_glDrawElements(TKRA_GL_TRIANGLES, 6*6, TKRA_GL_INT, cube_tris);

	tkra_glPopMatrix();
}

byte *BTM_LoadFile(char *name, int *rsz)
{
	byte *buf;
	FILE *fd;
	int sz, i;
	
	fd=fopen(name, "rb");
	if(!fd)
	{
		printf("BTM_LoadFile: Failed open %s\n", name);
		return(NULL);
	}
	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	buf=malloc(sz+24);
	i=fread(buf, 1, sz, fd);
	fclose(fd);
	
	if(i!=sz)
	{
		printf("BTM_LoadFile: Size mismatch %s exp=%d got=%d\n",
			name, sz, i);
		if(i>0)
		{
			sz=i;
		}else
		{
			free(buf);
			return(NULL);
		}
	}
	
//	*(u64 *)(buf+sz+0)=0;
//	*(u64 *)(buf+sz+8)=0;
	memset(buf+sz, 0, 16);
	
	*rsz=sz;
	return(buf);
}

int BTM_StoreFile(char *name, void *buf, int sz)
{
	FILE *fd;
	
	fd=fopen(name, "wb");
	if(!fd)
		return(-1);
	
	fwrite(buf, 1, sz, fd);
	fclose(fd);
	return(0);
}

byte *wrl_zmap;

byte *wrl_nmap;

float BTM_NoisePt8F(int ix, int iy, int mskx, int msky)
{
	int ix1, iy1, p;
	float f;

	ix&=mskx;
	iy&=msky;

	ix1=(2*ix-iy);
	iy1=(2*iy-ix);
	p=iy1*15+ix1;
	f=wrl_nmap[p&255]*(1.0/128)-1.0;
	return(f);
}

float BTM_Noise8F(float x, float y, int mskx, int msky)
{
	float f0, f1, f2, f3;
	float f4, f5, f6;
	float fx, fy;
	int ix, iy;
	int p0, p1, p2, p3;

	ix=x;		iy=y;
	fx=x-ix;	fy=y-iy;

	f0=BTM_NoisePt8F(ix+0, iy+0, mskx, msky);
	f1=BTM_NoisePt8F(ix+1, iy+0, mskx, msky);
	f2=BTM_NoisePt8F(ix+0, iy+1, mskx, msky);
	f3=BTM_NoisePt8F(ix+1, iy+1, mskx, msky);
	f4=(1.0-fx)*f0+(fx*f1);
	f5=(1.0-fx)*f2+(fx*f3);
	f6=(1.0-fy)*f4+(fy*f5);
	return(f6);
}

// float cam_ang_yaw;
// float cam_ang_pitch;
// float cam_org[3];
byte btm_noclip;

int main(int argc, char *argv[])
{
	TKRA_Context *ractx;
	BTM_World *wrl;
//	BTM_Screen *scr;
//	BTM_TexImg *tex;
//	BTM_TexImg *skytex;
	
	int tt, ltt, dt;
//	u32 *screen, *ct;
//	u16 *fbuf, *cs, *act;
	u16 *tex0;
	u16 *fbuf;
	byte *tbuf;
	u64 wpos;
	int x, y, z, xs, ys, txs, tys;
	int afrac, astep, mdl_ntris;
	int i, j, k, l;
	float f0, f1, f2;
	float ang, dtf, frc;

	printf("Init Skybox\n");

	BTMGL_InitSkybox();

	printf("Init TKRA\n");

	ractx=TKRA_AllocContext();
	TKRA_SetupScreen(ractx, 320, 200);
	TKRA_SetupForState(ractx);
	TKRA_SetCurrentContext(ractx);

	tkra_glViewport(0, 0, 320, 200);

	printf("Load Textures\n");

#if 0
	tex0=BTIC1H_Img_LoadTGA5551("sky1b1.tga", &txs, &tys);

//	raimg=TKRA_GetTexImg(ractx, 1);
//	TKRA_UpdateTexImg(raimg, tex0, txs, tys, -1);
//	TKRA_BindTexImg(ractx, raimg);

	tkra_glBindTexture(TKRA_TEXTURE_2D, 1);
	tkra_glTexImage2D(TKRA_TEXTURE_2D, 0, 3, txs, tys, 0,
		TKRA_RGBA, TKRA_GL_UNSIGNED_SHORT_5_5_5_1, tex0);
#endif

//	tbuf=BTM_LoadFile("dummy.dds", &j);

	btmgl_filter_min=GL_NEAREST_MIPMAP_NEAREST;
	btmgl_filter_max=GL_NEAREST;

	tbuf=BTM_LoadFile("sky1b1.dds", &j);
	if(tbuf)
	{
		tkra_glBindTexture(TKRA_TEXTURE_2D, 1);
		BTMGL_UploadCompressed(tbuf, 1, 1);
	}

//	for(y=0; y<256; y++)
//		for(x=0; x<256; x++)
//	{
//		z=btmra_morton8(x, y);
//		skytex->rgb[z]=tex0[(255-y)*256+x];
//	}

#if 0
//	tex0=BTIC1H_Img_LoadTGA5551("atlas0_far.tga", &txs, &tys);
	tex0=BTIC1H_Img_LoadTGA5551("atlas0_2a.tga", &txs, &tys);
	tkra_glBindTexture(TKRA_TEXTURE_2D, 2);
	tkra_glTexImage2D(TKRA_TEXTURE_2D, 0, 4, txs, tys, 0,
		TKRA_RGBA, TKRA_GL_UNSIGNED_SHORT_5_5_5_1, tex0);
#endif

	tbuf=BTM_LoadFile("atlas0_2a.dds", &j);
	if(tbuf)
	{
		tkra_glBindTexture(TKRA_TEXTURE_2D, 2);
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



//	wrl=BTM_AllocWorld(7, 4);
	wrl=BTM_AllocWorld(8, 6);

	wrl_nmap=malloc(256);
	for(i=0; i<256; i++)
		wrl_nmap[i]=rand();
	
	wrl_zmap=malloc(256*256);

	printf("Generate Terrain\n");

	for(i=0; i<256; i++)
		for(j=0; j<256; j++)
	{
		l=	BTM_Noise8F(i/32.0, j/32.0, 7, 7)*4 +
			BTM_Noise8F(i/8.0, j/8.0, 31, 31)*2 +
			BTM_Noise8F(i/2.0, j/2.0, 127, 127)*1 +
			26;

		for(k=l; k<24; k++)
			{ BTM_SetWorldBlockXYZ(wrl, i, j, k, BTM_BLKTY_WATER); }

		for(k=l-4; k<l; k++)
		{
			if(l>25)
				BTM_SetWorldBlockXYZ(wrl, i, j, k, BTM_BLKTY_DIRT);
			else
				BTM_SetWorldBlockXYZ(wrl, i, j, k, BTM_BLKTY_SAND);
		}

		for(k=0; k<l-3; k++)
			{ BTM_SetWorldBlockXYZ(wrl, i, j, k, BTM_BLKTY_STONE); }

		if(l>25)
			{ BTM_SetWorldBlockXYZ(wrl, i, j, l-1, BTM_BLKTY_GRASS); }
		else
			{ BTM_SetWorldBlockXYZ(wrl, i, j, l-1, BTM_BLKTY_SAND); }
		
//		if(!(rand()&63))
		if(!(rand()&255) && (l>25))
		{
			BTM_GenTree(wrl, i, j, l);
		}
	}

//	scr=BTMRA_AllocScreen(320, 200);
//	tex=BTMRA_AllocTexture(6);
//	skytex=BTMRA_AllocTexture(8);

	BTM_UpdateWorldBlockOcc(wrl);

	printf("Go\n");

	I_SystemInit();

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
	cam_org[2]=32;

	tkra_glDepthRange(0.0, 1.0);
	tkra_glClearDepth(0.99);

	tkra_glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	tkra_glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
	
	tkra_glClearColor(0.3, 0.6, 0.9, 1.0);

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

		I_UpdateKeys();

		if(I_KeyDown(K_LEFTARROW))
			cam_ang_yaw-=dt*(90/1000.0);
		if(I_KeyDown(K_RIGHTARROW))
			cam_ang_yaw+=dt*(90/1000.0);
		
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

			if(I_KeyDown(K_UPARROW))
			{
				cam_org[0]+=dt*(12/1000.0)*sin(cam_ang_yaw*(M_PI/180));
				cam_org[1]-=dt*(12/1000.0)*cos(cam_ang_yaw*(M_PI/180));
			}
			if(I_KeyDown(K_DOWNARROW))
			{
				cam_org[0]-=dt*(12/1000.0)*sin(cam_ang_yaw*(M_PI/180));
				cam_org[1]+=dt*(12/1000.0)*cos(cam_ang_yaw*(M_PI/180));
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
			
			if(I_KeyDown(K_HOME))
			{
				if(cam_mvflags&1)
				{
					if(cam_vel[2]<0)
						cam_vel[2]=0;
					cam_vel[2]+=6;
				}
//				else
//					cam_ivel[2]=12;
			}
			if(I_KeyDown(K_END))
				cam_ivel[2]=-12;

			if(I_KeyDown(K_UPARROW))
			{
				cam_ivel[0]= 7*sin(cam_ang_yaw*(M_PI/180));
				cam_ivel[1]=-7*cos(cam_ang_yaw*(M_PI/180));
			}
			if(I_KeyDown(K_DOWNARROW))
			{
				cam_ivel[0]=-7*sin(cam_ang_yaw*(M_PI/180));
				cam_ivel[1]= 7*cos(cam_ang_yaw*(M_PI/180));
			}
			
			frc=1.0-8*dtf;
			if(frc<0)
				frc=0;

			if(TKRA_Vec3F_DotProduct(cam_vel, cam_vel)<0.2)
				frc=0;

			if(cam_mvflags&1)
			{
				f0=TKRA_Vec3F_DotProduct(cam_vel, cam_ivel);
				f1=TKRA_Vec3F_DotProduct(cam_vel, cam_vel)+0.01;
				f2=f0/f1;
			
				if(f2<=0.9)
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
				{
					cam_vel[0]+=cam_ivel[0]*dtf*0.1;
					cam_vel[1]+=cam_ivel[1]*dtf*0.1;
					cam_vel[2]+=cam_ivel[2]*dtf*0.1;
				}
			}
			
			BTM_CheckWorldMoveVel(wrl, dtf,
				cam_org, cam_vel,
				cam_org, cam_vel, &cam_mvflags);
		}
		
		if(I_CheckCheatStr("idclip"))
		{
			btm_noclip=!btm_noclip;
			printf("Noclip %d\n", btm_noclip);
		}
		
		if(cam_org[0]<0)
			{ cam_org[0]+=1<<wrl->xsh; }
		if(cam_org[1]<0)
			{ cam_org[1]+=1<<wrl->xsh; }
		if(cam_org[0]>=(1<<wrl->xsh))
			{ cam_org[0]-=1<<wrl->xsh; }
		if(cam_org[1]>=(1<<wrl->xsh))
			{ cam_org[1]-=1<<wrl->xsh; }

		if(cam_org[2]<0)
			{ cam_org[2]=0; }
		if(cam_org[2]>(1<<wrl->zsh))
			{ cam_org[2]=1<<wrl->zsh; }
			
		if(cam_ang_pitch>85)
			cam_ang_pitch=85;
		if(cam_ang_pitch<-85)
			cam_ang_pitch=-85;
		
		wrl->cam_yaw=(byte)(cam_ang_yaw*(256.0/360.0));
		wrl->cam_pitch=(byte)(cam_ang_pitch*(256.0/360.0));
		
		wrl->cam_org=
			((u64)((u16)(cam_org[0]*256))<< 0) |
			((u64)((u16)(cam_org[1]*256))<<24) |
			((u64)((u16)(cam_org[2]*256))<<48) ;
		
//		BTM_RaycastScene(wrl);
		BTM_RaycastSceneQuad(wrl);

//		memset(fbuf, 0, 320*200*2);
//		memset(fbuf, 0x7F, 320*200*2);


// 		tkra_glClear(TKRA_GL_DEPTH_BUFFER_BIT);
		tkra_glClear(TKRA_GL_DEPTH_BUFFER_BIT | TKRA_GL_COLOR_BUFFER_BIT);

		tkra_glMatrixMode(TKRA_MODELVIEW);
		tkra_glLoadIdentity();

		tkra_glMatrixMode(TKRA_PROJECTION);
		tkra_glLoadIdentity();
		tkra_glFrustum(-1, 1, -0.75, 0.75, 1.0, 1024);
		tkra_glRotatef(90, 1, 0, 0);
		tkra_glRotatef(cam_ang_pitch, 1, 0, 0);

		tkra_glRotatef(-cam_ang_yaw, 0, 0, 1);
		tkra_glTranslatef(-cam_org[0], -cam_org[1], -cam_org[2]);

		tkra_glDisable(GL_CULL_FACE);
		tkra_glDepthFunc(GL_LEQUAL);
		tkra_glEnable(GL_DEPTH_TEST);
//		tkra_glEnable(GL_BLEND);
		tkra_glDisable(GL_BLEND);

//		BTMGL_DrawSkybox();

		tkra_glDisable(GL_BLEND);

		BTMGL_DrawSceneBlocks(wrl);

#if 0
		BTMRA_DrawSky(scr, skytex, 0);

//		BTMRA_DrawWall(scr, tex, wpos, tt/8);
//		BTMRA_DrawWall(scr, tex, wpos+(2<<8), tt/8);

		BTMRA_DrawBox(scr, tex, wpos, tt/32, 0x3F);

		BTMRA_DrawBox(scr, tex, wpos-(((u64)1)<<56), tt/32, 0x3F);
		BTMRA_DrawBox(scr, tex, wpos+(((u64)1)<<56), tt/32, 0x3F);
#endif

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

		l=cam_org[0];
		BTM_DrawCharFBuf(fbuf, 320, 320-24, 16,
			'0'+((l/100)%10), 0x7FFF, 0x0000);
		BTM_DrawCharFBuf(fbuf, 320, 320-16, 16, '0'+((l/10)%10), 0x7FFF, 0x0000);
		BTM_DrawCharFBuf(fbuf, 320, 320- 8, 16, '0'+(l%10), 0x7FFF, 0x0000);

		l=cam_org[1];
		BTM_DrawCharFBuf(fbuf, 320, 320-24, 24, '0'+((l/100)%10), 0x7FFF, 0x0000);
		BTM_DrawCharFBuf(fbuf, 320, 320-16, 24, '0'+((l/10)%10), 0x7FFF, 0x0000);
		BTM_DrawCharFBuf(fbuf, 320, 320- 8, 24, '0'+(l%10), 0x7FFF, 0x0000);

		l=cam_org[2];
		BTM_DrawCharFBuf(fbuf, 320, 320-24, 32, '0'+((l/100)%10), 0x7FFF, 0x0000);
		BTM_DrawCharFBuf(fbuf, 320, 320-16, 32, '0'+((l/10)%10), 0x7FFF, 0x0000);
		BTM_DrawCharFBuf(fbuf, 320, 320- 8, 32, '0'+(l%10), 0x7FFF, 0x0000);

//		tkra_glVertexPointer(3, TKRA_GL_FLOAT, 6*4, mdl_tris+0);
//		tkra_glTexCoordPointer(2, TKRA_GL_FLOAT, 6*4, mdl_tris+3);
//		tkra_glColorPointer(4, TKRA_GL_UNSIGNED_BYTE, 6*4, mdl_tris+5);
//		tkra_glDrawArrays(TKRA_GL_TRIANGLES, 0, mdl_ntris*3);


		I_SystemFrame(fbuf, xs, ys);
	}

#if 0
	memset(tsampbuf, 0, 16384*2);
//	SoundDev_WriteStereoSamples(tsampbuf, 2048);
//	SoundDev_WriteStereoSamples2(tsampbuf, 1024, 1024);

//	TKRA_LoadPly("bun_zipper_res4.ply", &mdl_tris, &mdl_ntris);
//	LoadTrisStl("Utah_teapot.stl", &mdl_tris, &mdl_ntris);

	printf("Model %d triangles\n", mdl_ntris);

	tex0=BTIC1H_Img_LoadTGA555("Street256.tga", &txs, &tys);

	ractx=TKRA_AllocContext();
	TKRA_SetupScreen(ractx, 320, 200);
	raimg=TKRA_GetTexImg(ractx, 1);
	TKRA_UpdateTexImg(raimg, tex0, txs, tys, -1);
//	TKRA_UpdateTexImg(raimg, tex0, txs, tys, 0);

	TKRA_BindTexImg(ractx, raimg);
	TKRA_SetupForState(ractx);

	TKRA_SetCurrentContext(ractx);

	ractx->mat_tproj=TKRA_MatrixIdentify();
//	__debugbreak();

	TKRA_UnpackMatrix16fv(ractx->mat_tproj, xyz0);
	printf("Identity\n");
	printf("%f %f %f %f\n", xyz0[ 0], xyz0[ 1], xyz0[ 2], xyz0[ 3]);
	printf("%f %f %f %f\n", xyz0[ 4], xyz0[ 5], xyz0[ 6], xyz0[ 7]);
	printf("%f %f %f %f\n", xyz0[ 8], xyz0[ 9], xyz0[10], xyz0[11]);
	printf("%f %f %f %f\n", xyz0[12], xyz0[13], xyz0[14], xyz0[15]);


//	ractx->mat_tproj=TKRA_MatrixSetupFrustum(ractx->mat_tproj,
//		-1, 1,  -1, 1,  1, 8192);

//	ractx->mat_tproj=TKRA_MatrixTranslatef(ractx->mat_tproj, 0, 0, 1);

	ractx->mat_xform=TKRA_MatrixIdentify();
//	ractx->mat_xform=TKRA_MatrixTranslatef(ractx->mat_xform, 0, 0, 15);
//	ractx->mat_xform=TKRA_MatrixTranslatef(ractx->mat_xform, 0, 0, -25);

	ractx->mat_xproj=TKRA_MatrixIdentify();
	ractx->mat_xproj=TKRA_MatrixSetupFrustum(ractx->mat_xproj,
		-1, 1,  -1, 1,  1.0, 8192);
	ractx->mat_xproj=TKRA_MatrixTranslatef(ractx->mat_xproj, 0, 0, -25);

	ractx->mat_tproj=TKRA_MatrixMultiply(
		ractx->mat_xform, ractx->mat_xproj);
//	ractx->mat_tproj=TKRA_MatrixMultiply(
//		ractx->mat_xproj, ractx->mat_xform);

	TKRA_UnpackMatrix16fv(ractx->mat_xform, xyz0);
	printf("XForm\n");
	printf("%f %f %f %f\n", xyz0[ 0], xyz0[ 1], xyz0[ 2], xyz0[ 3]);
	printf("%f %f %f %f\n", xyz0[ 4], xyz0[ 5], xyz0[ 6], xyz0[ 7]);
	printf("%f %f %f %f\n", xyz0[ 8], xyz0[ 9], xyz0[10], xyz0[11]);
	printf("%f %f %f %f\n", xyz0[12], xyz0[13], xyz0[14], xyz0[15]);

	TKRA_UnpackMatrix16fv(ractx->mat_xproj, xyz0);
	printf("XProj\n");
	printf("%f %f %f %f\n", xyz0[ 0], xyz0[ 1], xyz0[ 2], xyz0[ 3]);
	printf("%f %f %f %f\n", xyz0[ 4], xyz0[ 5], xyz0[ 6], xyz0[ 7]);
	printf("%f %f %f %f\n", xyz0[ 8], xyz0[ 9], xyz0[10], xyz0[11]);
	printf("%f %f %f %f\n", xyz0[12], xyz0[13], xyz0[14], xyz0[15]);

	TKRA_UnpackMatrix16fv(ractx->mat_tproj, xyz0);
	printf("TProj\n");
	printf("%f %f %f %f\n", xyz0[ 0], xyz0[ 1], xyz0[ 2], xyz0[ 3]);
	printf("%f %f %f %f\n", xyz0[ 4], xyz0[ 5], xyz0[ 6], xyz0[ 7]);
	printf("%f %f %f %f\n", xyz0[ 8], xyz0[ 9], xyz0[10], xyz0[11]);
	printf("%f %f %f %f\n", xyz0[12], xyz0[13], xyz0[14], xyz0[15]);

	xs=320;
	ys=200;
//	fbuf=malloc(xs*ys*sizeof(u16));
	fbuf=ractx->screen_rgb;

	memcpy(fbuf, tex0, 256*200*2);
	memset(ractx->screen_zbuf, 255, 320*200*2);

#if 0
//	v1_parm[TKRA_VX_XPOS]=64<<16;
//	v1_parm[TKRA_VX_XPOS]=32<<16;
	v1_parm[TKRA_VX_XPOS]=(-32)<<16;
//	v1_parm[TKRA_VX_YPOS]=184<<16;
	v1_parm[TKRA_VX_YPOS]=224<<16;

//	v2_parm[TKRA_VX_XPOS]=256<<16;
	v2_parm[TKRA_VX_XPOS]=444<<16;
//	v2_parm[TKRA_VX_YPOS]=184<<16;
	v2_parm[TKRA_VX_YPOS]=160<<16;

	v3_parm[TKRA_VX_XPOS]=160<<16;
	v3_parm[TKRA_VX_YPOS]=16<<16;

	v1_parm[TKRA_VX_ZPOS]=16<<16;
	v2_parm[TKRA_VX_ZPOS]=16<<16;
	v3_parm[TKRA_VX_ZPOS]=16<<16;

//	v1_parm[TKRA_VX_CPOS]=0xFFFFFFFFFFFFFFFFULL;
//	v2_parm[TKRA_VX_CPOS]=0xFFFFFFFFFFFFFFFFULL;
//	v3_parm[TKRA_VX_CPOS]=0xFFFFFFFFFFFFFFFFULL;

	v1_parm[TKRA_VX_CPOS]=0xFFFFFFFF00000000ULL;
	v2_parm[TKRA_VX_CPOS]=0xFFFF0000FFFF0000ULL;
	v3_parm[TKRA_VX_CPOS]=0xFFFF00000000FFFFULL;

	v1_parm[TKRA_VX_TPOS]=0x0000000000000000ULL;
	v2_parm[TKRA_VX_TPOS]=0x0000000000FF0000ULL;
	v3_parm[TKRA_VX_TPOS]=0x00FF000000800000ULL;

	TKRA_WalkTriangle(ractx, v1_parm, v2_parm, v3_parm);
#endif

	for(i=0; i<8; i++)
	{
		vtx_xyz[i*4+0]=(i&1)?(10.0):(-10.0);
		vtx_xyz[i*4+1]=(i&2)?(10.0):(-10.0);
		vtx_xyz[i*4+2]=(i&4)?(10.0):(-10.0);
		vtx_xyz[i*4+3]=0;
		vtx_rgb[i]=0xFF000000|
			((i&4)?0x00FF0000:0x007F0000)|
			((i&2)?0x0000FF00:0x00007F00)|
			((i&1)?0x000000FF:0x0000007F);
	}

	tt=I_TimeMS();
	ltt=tt;
	while(!gfxdrv_kill)
	{
		tt=I_TimeMS();
		dt=tt-ltt;
		ltt=tt;

		if(!dt)
		{
			continue;
		}

		memset(fbuf, 0, 320*200*2);
//		memset(ractx->screen_zbuf, 255, 320*200*2);
		memset(ractx->screen_zbuf, 255, 160*100*2);

#if 0
		xyz0[0]=-0.75;	xyz0[1]=-0.75;	xyz0[ 2]=2.0;
		xyz0[4]= 0.75;	xyz0[5]=-0.75;	xyz0[ 6]=2.0;
		xyz0[8]= 0.0;	xyz0[9]= 0.75;	xyz0[10]=2.0;
		st0[0]=0.0;		st0[1]=0.0;
		st0[2]=1.0;		st0[3]=0.0;
		st0[4]=0.5;		st0[5]=1.0;
//		rgb0[0]=0xFFFFFFFF;
//		rgb0[1]=0xFFFFFFFF;
//		rgb0[2]=0xFFFFFFFF;

		rgb0[0]=0xFFFF8080;
		rgb0[1]=0xFF80FF80;
		rgb0[2]=0xFF8080FF;
		
		tst_rot2d(xyz0+0, xyz0+0, tt/2500.0);
		tst_rot2d(xyz0+4, xyz0+4, tt/2500.0);
		tst_rot2d(xyz0+8, xyz0+8, tt/2500.0);

		TKRA_DrawTriangleArrayBasic(ractx, 
			xyz0,	16,
			st0,	8,
			rgb0,	4,
			1);
#endif

		for(i=0; i<6; i++)
		{
			TKRA_Vec3F_Copy(vtx_xyz+cube_tris[i*6+0]*4, xyz0+(i*6+0)*4);
			TKRA_Vec3F_Copy(vtx_xyz+cube_tris[i*6+1]*4, xyz0+(i*6+1)*4);
			TKRA_Vec3F_Copy(vtx_xyz+cube_tris[i*6+2]*4, xyz0+(i*6+2)*4);
			TKRA_Vec3F_Copy(vtx_xyz+cube_tris[i*6+3]*4, xyz0+(i*6+3)*4);
			TKRA_Vec3F_Copy(vtx_xyz+cube_tris[i*6+4]*4, xyz0+(i*6+4)*4);
			TKRA_Vec3F_Copy(vtx_xyz+cube_tris[i*6+5]*4, xyz0+(i*6+5)*4);
			
			rgb0[i*6+0]=vtx_rgb[cube_tris[i*6+0]];
			rgb0[i*6+1]=vtx_rgb[cube_tris[i*6+1]];
			rgb0[i*6+2]=vtx_rgb[cube_tris[i*6+2]];
			rgb0[i*6+3]=vtx_rgb[cube_tris[i*6+3]];
			rgb0[i*6+4]=vtx_rgb[cube_tris[i*6+4]];
			rgb0[i*6+5]=vtx_rgb[cube_tris[i*6+5]];

			st0[(i*6+0)*2+0]=0.0;		st0[(i*6+0)*2+1]=0.0;
			st0[(i*6+1)*2+0]=1.0;		st0[(i*6+1)*2+1]=0.0;
			st0[(i*6+2)*2+0]=1.0;		st0[(i*6+2)*2+1]=1.0;
			st0[(i*6+3)*2+0]=0.0;		st0[(i*6+3)*2+1]=0.0;
			st0[(i*6+4)*2+0]=1.0;		st0[(i*6+4)*2+1]=1.0;
			st0[(i*6+5)*2+0]=0.0;		st0[(i*6+5)*2+1]=1.0;
		}
		
#if 0
		ang=tt/2500.0;
		for(i=0; i<(6*6); i++)
		{
			tst_rot2d_xz(xyz0+i*4, xyz0+i*4, ang);
		}

		ang=tt/25000.0;
		for(i=0; i<(6*6); i++)
		{
			tst_rot2d_xy(xyz0+i*4, xyz0+i*4, ang);
		}

		TKRA_DrawTriangleArrayBasic(ractx, 
			xyz0,	16,
			st0,	8,
			rgb0,	4,
			12);
#endif

		ang=tt/2500.0;

		ractx->mat_xform=TKRA_MatrixIdentify();
//		ractx->mat_xform=TKRA_MatrixTranslatef(ractx->mat_xform, 0, 0, 15);
		ractx->mat_xform=TKRA_MatrixRotatef(ractx->mat_xform,
			ang*100, 0, 1, 0);
		ractx->mat_xform=TKRA_MatrixRotatef(ractx->mat_xform,
			ang*10, 0, 0, 1);

		ractx->mat_tproj=TKRA_MatrixMultiply(
			ractx->mat_xform, ractx->mat_xproj);

//		TKRA_DrawTriangleArrayBasic(ractx, 
//			mdl_tris+0,	6*4,
//			mdl_tris+3,	6*4,
//			mdl_tris+5,	6*4,
//			mdl_ntris);

		tkra_glVertexPointer(3, TKRA_GL_FLOAT, 6*4, mdl_tris+0);
		tkra_glTexCoordPointer(2, TKRA_GL_FLOAT, 6*4, mdl_tris+3);
		tkra_glColorPointer(4, TKRA_GL_UNSIGNED_BYTE, 6*4, mdl_tris+5);
		tkra_glDrawArrays(TKRA_GL_TRIANGLES, 0, mdl_ntris*3);

//		dt*=2;
		
		I_SystemFrame(fbuf, xs, ys);
	}
#endif
}
