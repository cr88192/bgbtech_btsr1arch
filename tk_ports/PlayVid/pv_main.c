// byte *btesh2_gfxcon_framebuf;
// int btesh2_gfxcon_fb_dirty;

u16 tsampbuf[32768+16384];
int tsamplen;
int tsamprovs;
int tsamprove;

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
	u32 *screen, *ct;
	u16 *cs;
	int x, y;

	screen=(u32 *)btesh2_gfxcon_framebuf;

//	SoundDev_WriteStereoSamples(ctx->sampbuf, k);
//	SoundDev_WriteStereoSamples(tsampbuf, tsamplen);
//	SoundDev_Submit();

	GfxDrv_BeginDrawing();
	
	for(y=0; y<ys; y++)
	{
		cs=fbuf+(ys-y-1)*xs;
		ct=screen+(y*btesh2_gfxcon_fbxs);
		
		for(x=0; x<xs; x++)
		{
			ct[x]=d_16to24table[cs[x]];
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
	SoundDev_Init();

	conbufa=(u32 *)0xF00A0000;
	((u32 *)0xF00BFF00)[0]=0x0095;		//320x200x16bpp, RGB555
}

void I_FinishUpdate_ScanCopyVf(u16 *ics, u32 *ict, int blkn);

__asm {
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
			cs3=(u64 *)(fbuf+((ys-y-4)*xs));
			I_FinishUpdate_ScanCopyVf(cs3, ct, 80);
			ct+=(80*4);
			continue;
		}
#endif

#if 1
		cs0=(u64 *)(fbuf+((ys-y-1)*xs));
		cs1=(u64 *)(fbuf+((ys-y-2)*xs));
		cs2=(u64 *)(fbuf+((ys-y-3)*xs));
		cs3=(u64 *)(fbuf+((ys-y-4)*xs));
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

int main(int argc, char *argv[])
{
	BGBBTJ_AVI_Context *ctx;
	int tt, ltt, dt, audt;
//	u32 *screen, *ct;
//	u16 *fbuf, *cs, *act;
	u16 *fbuf;
	int x, y, xs, ys;
	int afrac, astep;
	int i, j, k, l;

	I_SystemInit();

	BGBBTJ_AVI_Init();
	
//	ctx=BGBBTJ_AVI_LoadAVI("BadApple_CV6.avi");
//	ctx=BGBBTJ_AVI_LoadAVI("Heist_CV0.avi");
	ctx=BGBBTJ_AVI_LoadAVI("TestOut_CV0.avi");
//	ctx=BGBBTJ_AVI_LoadAVI("TestOut_CV1_5B.avi");

	xs=ctx->bmihead->biWidth;
	ys=ctx->bmihead->biHeight;

	memset(tsampbuf, 0, 16384*2);

	tsamprovs=0;
//	tsamprove=512;
	tsamprove=2048;
//	tsamprove=4096;

	SoundDev_WriteStereoSamples(tsampbuf, 256);
//	SoundDev_WriteStereoSamples(tsampbuf, 2048);
//	SoundDev_WriteStereoSamples2(tsampbuf, 1024, 1024);

	audt=0;
	tt=I_TimeMS();
	ltt=tt;
	while(!gfxdrv_kill)
	{
		tt=I_TimeMS();
		dt=tt-ltt;
//		ltt=tt;

		if(!dt)
//		if(dt<4)
		{
			continue;
		}

		ltt=tt;
		
//		dt*=2;

		fbuf=BGBBTJ_AVI_FrameRawClrs(ctx, dt*1000, BGBBTJ_JPG_RGB555);
		
		if((ctx->wavefmt->nSamplesPerSec)==16000)
//		if(0)
		{
			astep=256;
			k=(ctx->esampbuf-ctx->sampbuf)/2;
			l=k;

			if(l)
			{
//				printf("%d\n", l);
			}

			if((tsamprove<tsamprovs) && ((tsamprove+l)>tsamprovs))
			{
				printf("PV_Main: Audio Skip Adjust A\n");
				tsamprove=(tsamprove+(2*l))&16383;
			}


#if 0
			j=tsamprove*2;
			for(i=0; i<l; i++)
			{
				tsampbuf[j+0]=ctx->sampbuf[i*2+0];
				tsampbuf[j+1]=ctx->sampbuf[i*2+1];
				j=(j+2)&32767;
			}
			tsamprove=j/2;
#endif

			memcpy(tsampbuf+(tsamprove*2), ctx->sampbuf, l*4);
			
			j=tsamprove+l;
			if(j>16384)
			{
				k=j-16384;
				memcpy(tsampbuf, ctx->sampbuf+(l-k)*2, k*4);
			}
			tsamprove=j&16383;

//			memcpy(tsampbuf, ctx->sampbuf, l*4);
//			tsamplen=l;

#if 0
			for(i=0; i<l; i++)
			{
				j=((i>>5)&1)?1024:(-1024);
				tsampbuf[i*2+0]=j;
				tsampbuf[i*2+1]=j;
			}
#endif
		}else
		{
	//		astep=(256*16000)/(ctx->wavefmt->nSamplesPerSec);
			astep=(256*(ctx->wavefmt->nSamplesPerSec))/16000;
			k=(ctx->esampbuf-ctx->sampbuf)/2;
	//		l=(k*astep)>>8;
			l=(k*16000)/(ctx->wavefmt->nSamplesPerSec);

			if((tsamprove<tsamprovs) && ((tsamprove+l)>tsamprovs))
			{
				printf("PV_Main: Audio Skip Adjust B\n");
				tsamprove=(tsamprove+(2*l))&16383;
			}

			j=tsamprove*2;
			afrac=0;
			for(i=0; i<l; i++)
			{
//				j=i*2;
				k=(afrac>>8)*2;
				afrac+=astep;
				tsampbuf[j+0]=ctx->sampbuf[k+0];
				tsampbuf[j+1]=ctx->sampbuf[k+1];
				j=(j+2)&32767;
			}
			tsamprove=j>>1;
			tsamplen=l;
		}

//		SoundDev_WriteStereoSamples(ctx->sampbuf, l);
		
		ctx->esampbuf=ctx->sampbuf;

#if 0
		j=tsamprovs;
		l=dt*16;
		if(l>0)
		{
//			SoundDev_WriteStereoSamples2(tsampbuf+(j*2), l, l*2);
			SoundDev_WriteStereoSamples(tsampbuf+(j*2), l);
			SoundDev_Submit();
			tsamprovs=(j+l)&16383;
		}
#endif

#if 1
		audt+=dt;
		while(audt>16)
		{
			l=16*16;
			audt-=16;
		
			j=tsamprovs;
			if(	(tsamprovs<tsamprove) &&
				((j+2*l)>=tsamprove))
			{
				printf("PV_Main: Audio Skip Adjust C\n");
				tsamprove=(tsamprove+4*l)&16383;
			}
			
			SoundDev_WriteStereoSamples2(tsampbuf+(j*2), l, l*2);
//			SoundDev_WriteStereoSamples(tsampbuf+(j*2), l);
			SoundDev_Submit();
			tsamprovs=(j+l)&16383;
		}
#endif

#if 0
		if(l>0)
		{
			SoundDev_WriteStereoSamples(tsampbuf, l);
//			SoundDev_WriteStereoSamples2(tsampbuf, l, l);
	//		SoundDev_WriteStereoSamples2(tsampbuf, l*2, l*2);
			SoundDev_Submit();
		}
#endif


//		if(!(ctx->flags&1) && !tsamplen)
		if(!(ctx->flags&1))
			continue;
		
		I_SystemFrame(fbuf, xs, ys);
	}
}
