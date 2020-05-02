int gfxdrv_kill=0;

int GfxDrv_PrepareFramebuf()
{
	btesh2_gfxcon_fb_dirty=3;
	if(!btesh2_gfxcon_framebuf)
	{
		btesh2_gfxcon_framebuf=malloc(640*480*4);
	}
	return(0);
}

void GfxDrv_BeginDrawing()
{
}

void GfxDrv_EndDrawing()
{
}

int SoundDev_WriteStereoSamples(short *buffer, int cnt)
{
	return(0);
}

int SoundDev_WriteStereoSamples2(
	short *buffer, int cnt, int ovcnt)
{
//	return(SoundDev_WriteRawSamples(buffer, cnt*2, ovcnt*2));
	return(0);
}

int GfxDrv_MainLoop(void (*fcn)())
{
	static int lt=0;
	int ct, dt;
	
	ct=FRGL_TimeMS();
	while(!gfxdrv_kill)
	{
		ct=FRGL_TimeMS();
		dt=ct-lt;
		if((dt>=0) && (dt<10))
		{
#ifdef __liunux
			usleep(1);
#endif
			continue;
		}
		
		fcn();
		lt=ct;
	}
}
