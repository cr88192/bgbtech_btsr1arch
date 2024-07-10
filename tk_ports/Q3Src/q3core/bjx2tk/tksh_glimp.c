/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
#include <errno.h>
#include <stdio.h>

#include <dirent.h>
#include <unistd.h>
#include <sys/mman.h>
// #include <sys/time.h>

#include <dlfcn.h>

#include <tkgdi/tkgdi.h>

#include "../renderer/tr_local.h"


qboolean ( * qwglSwapIntervalEXT)( int interval );
void ( * qglMultiTexCoord2fARB )( GLenum texture, float s, float t );
void ( * qglActiveTextureARB )( GLenum texture );
void ( * qglClientActiveTextureARB )( GLenum texture );


void ( * qglLockArraysEXT)( int, int);
void ( * qglUnlockArraysEXT) ( void );

// TKGDI_BITMAPINFOHEADER i_t_dibinfo;
TKGDI_BITMAPINFOHEADER *i_dibinfo = NULL;
TKGHDC i_hDc;

u16		*i_screen;

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

int TKRA_SetupContextBasic(int xs, int ys);
void *TKRA_GetCurrentScreenBuffer_RGB(void);
void *tkra_wglGetProcAddress(char *name);

qboolean QGL_Init( const char *dllname );

void I_InitTkGl()
{
//	TKRA_Context *ractx;
//	ractx=TKRA_AllocContext();
//	TKRA_SetupScreen(ractx, 320, 200);
//	TKRA_SetupForState(ractx);
//	TKRA_SetCurrentContext(ractx);

	TKRA_SetupContextBasic(320, 200);
	i_screen = TKRA_GetCurrentScreenBuffer_RGB();
	QGL_Init("");
}


void		GLimp_EndFrame( void )
{
	qglFinish();
	i_screen=TKRA_GetCurrentScreenBuffer_RGB();
	if(i_screen)
	{
		tkgBlitImage(i_hDc, 0, 0, i_dibinfo, i_screen);
	}
}

int 		GLimp_Init( void )
{
	I_InitTkGdi();
	I_InitTkGl();

	glConfig.stereoEnabled = qfalse;

	glConfig.driverType = GLDRV_STANDALONE;
	glConfig.colorBits = 16;
	glConfig.depthBits = 16;
	glConfig.stencilBits = 0;

	glConfig.vidWidth = 320;
	glConfig.vidHeight = 200;
	glConfig.windowAspect = (1.0 * 320) / 200;
	glConfig.displayFrequency = 60;
	glConfig.isFullscreen = 0;

	glConfig.textureCompression = TC_S3TC;
	glConfig.textureEnvAddAvailable = qfalse;

	qglClearColor (1,0,0,0);
	qglCullFace(GL_FRONT);
//	qglCullFace(GL_BACK);
	qglEnable(GL_TEXTURE_2D);

	qglDisable(GL_CULL_FACE);
	qglDepthFunc(GL_LEQUAL);
	qglDisable(GL_BLEND);

	qglEnable(GL_ALPHA_TEST);
	qglAlphaFunc(GL_GREATER, 0.666);

	qglPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	qglShadeModel (GL_FLAT);

	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	qglBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//	qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);


	qglViewport( 0, 0, 320, 200 );
	qglScissor( 0, 0, 320, 200 );
	qglMatrixMode(GL_PROJECTION);
    qglLoadIdentity ();
//	qglOrtho (0, 320, 200, 0, 0, 1);
	qglMatrixMode(GL_MODELVIEW);
    qglLoadIdentity ();
    
    ri.Cvar_Set( "r_textureMode", "GL_NEAREST_MIPMAP_NEAREST" );

}

void		GLimp_Shutdown( void ) {
}

void		GLimp_EnableLogging( qboolean enable ) {
}

void GLimp_LogComment( char *comment ) {
}

#if 0
qboolean QGL_Init( const char *dllname ) {
	return qtrue;
}

void		QGL_Shutdown( void ) {
}
#endif

void		GLimp_SetGamma(
	unsigned char red[256],
	unsigned char green[256],
	unsigned char blue[256] )
{
}

void GLimp_WakeRenderer( void *data )
{
}

void *GLimp_RendererSleep( void )
{
	return(NULL);
}

void GLimp_FrontEndSleep( void )
{
}

qboolean GLimp_SpawnRenderThread( void (*function)( void ) )
{
	__debugbreak();
	return qfalse;
}
