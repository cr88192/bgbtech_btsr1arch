/*
 Copyright (c) 2018-2020 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

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

int GfxDrv_MouseGetRelPos(int *dx, int *dy, int *mb)
{
	*dx=0;
	*dy=0;
	*mb=0;
	return(0);
}

int GfxDrv_MouseGetPos(int *mx, int *my, int *mb)
{
	*dx=0;
	*dy=0;
	*mb=0;
	return(0);
}

int GfxDrv_MouseSetPos(int mx, int my)
{
	return(0);
}
