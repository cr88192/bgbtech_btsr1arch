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

#endif

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
	0, 1, 5,  0, 5, 4,
	1, 3, 7,  1, 7, 5,
	3, 2, 6,  3, 6, 7,
	2, 0, 4,  2, 4, 6,
	4, 5, 7,  4, 7, 6,
	1, 0, 2,  1, 2, 3
};

int main(int argc, char *argv[])
{
	u64	v1_parm[TKRA_VX_NPARM];
	u64	v2_parm[TKRA_VX_NPARM];
	u64	v3_parm[TKRA_VX_NPARM];

	float vtx_xyz[8*4];
	u32 vtx_rgb[8];

	float xyz0[64*4];
	float st0[64*2];
	u32 rgb0[64];

	TKRA_Context *ractx;
	TKRA_TexImage *raimg;
	int tt, ltt, dt;
//	u32 *screen, *ct;
//	u16 *fbuf, *cs, *act;
	u16 *tex0;
	u16 *fbuf;
	int x, y, xs, ys, txs, tys;
	int afrac, astep;
	int i, j, k, l;
	float ang;

	I_SystemInit();

	memset(tsampbuf, 0, 16384*2);
//	SoundDev_WriteStereoSamples(tsampbuf, 2048);
//	SoundDev_WriteStereoSamples2(tsampbuf, 1024, 1024);

	tex0=BTIC1H_Img_LoadTGA555("Street256.tga", &txs, &tys);

	ractx=TKRA_AllocContext();
	TKRA_SetupScreen(ractx, 320, 200);
	raimg=TKRA_GetTexImg(ractx, 1);
	TKRA_UpdateTexImg(raimg, tex0, txs, tys, -1);

	TKRA_BindTexImg(ractx, raimg);
	TKRA_SetupForState(ractx);

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
	ractx->mat_xform=TKRA_MatrixTranslatef(ractx->mat_xform, 0, 0, -25);

	ractx->mat_xproj=TKRA_MatrixIdentify();
	ractx->mat_xproj=TKRA_MatrixSetupFrustum(ractx->mat_xproj,
		-1, 1,  -1, 1,  1.0, 8192);

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

#if 1
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

//		dt*=2;
		
		I_SystemFrame(fbuf, xs, ys);
	}
}
