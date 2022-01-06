/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
// vid_win.c -- Win32 video driver

#include "../renderer/tr_local.h"
#include "../qcommon/qcommon.h"
#include "resource.h"
#include "glw_win.h"
#include "win_local.h"

#ifdef USE_TKRA


// #include "quakedef.h"
// #include "winquake.h"
// #include "d_local.h"
// #include "resource.h"

#define MAX_MODE_LIST	30
#define VID_ROW_SIZE	3

#define WARP_WIDTH		320
#define WARP_HEIGHT		200
#define MAXWIDTH		10000
#define MAXHEIGHT		10000
#define BASEWIDTH		320
#define BASEHEIGHT		200


typedef struct {
int		x;
int		y;
int		width;
int		height;
}vrect_t;

glwstate_t glw_state;


qboolean	dibonly;

extern int		Minimized;

HWND		mainwindow;

HWND WINAPI InitializeWindow (HINSTANCE hInstance, int nCmdShow);

int			DIBWidth, DIBHeight;
qboolean	DDActive;
RECT		WindowRect;
DWORD		WindowStyle, ExWindowStyle;

int			window_center_x, window_center_y, window_x, window_y, window_width, window_height;
RECT		window_rect;

static DEVMODE	gdevmode;
static qboolean	startwindowed = 1, windowed_mode_set;
static int		firstupdate = 1;
static qboolean	vid_initialized = qfalse, vid_palettized;
static int		lockcount;
static int		vid_fulldib_on_focus_mode;
static qboolean	force_minimized, in_mode_set, is_mode0x13, force_mode_set;
static int		vid_stretched, windowed_mouse;
static qboolean	palette_changed, syscolchg, vid_mode_set, hide_window, pal_is_nostatic;
static HICON	hIcon;

// viddef_t	vid;				// global video state
int		vid_width;
int		vid_height;
u16		*vid_buffer;

int		texture_extension_number = 1;
int		texture_mode = GL_LINEAR;
qboolean isPermedia = qfalse;
qboolean		scr_skipupdate = qfalse;

// cvar_t	gl_ztrick = {"gl_ztrick","1"};
// cvar_t	gl_driver = {"gl_driver","opengl32.dll"};

unsigned char d_15to8table[65536];

const char *gl_vendor = "BGB";
const char *gl_renderer = "TKRA";
const char *gl_version = "1.1";
const char *gl_extensions = "";

qboolean gl_mtexable = qfalse;

float		gldepthmin, gldepthmax;

#define MODE_WINDOWED			0
#define MODE_SETTABLE_WINDOW	2
#define NO_MODE					(MODE_WINDOWED - 1)
#define MODE_FULLSCREEN_DEFAULT	(MODE_WINDOWED + 3)

#if 0
// Note that 0 is MODE_WINDOWED
cvar_t		vid_mode = {"vid_mode","0", qfalse};
// Note that 0 is MODE_WINDOWED
cvar_t		_vid_default_mode = {"_vid_default_mode","0", qtrue};
// Note that 3 is MODE_FULLSCREEN_DEFAULT
cvar_t		_vid_default_mode_win = {"_vid_default_mode_win","3", qtrue};
cvar_t		vid_wait = {"vid_wait","0"};
cvar_t		vid_nopageflip = {"vid_nopageflip","0", qtrue};
cvar_t		_vid_wait_override = {"_vid_wait_override", "0", qtrue};
cvar_t		vid_config_x = {"vid_config_x","800", qtrue};
cvar_t		vid_config_y = {"vid_config_y","600", qtrue};
cvar_t		vid_stretch_by_2 = {"vid_stretch_by_2","1", qtrue};
cvar_t		_windowed_mouse = {"_windowed_mouse","0", qtrue};
cvar_t		vid_fullscreen_mode = {"vid_fullscreen_mode","3", qtrue};
cvar_t		vid_windowed_mode = {"vid_windowed_mode","0", qtrue};
cvar_t		block_switch = {"block_switch","0", qtrue};
cvar_t		vid_window_x = {"vid_window_x", "0", qtrue};
cvar_t		vid_window_y = {"vid_window_y", "0", qtrue};
#endif

typedef struct {
	int		width;
	int		height;
} lmode_t;

lmode_t	lowresmodes[] = {
	{320, 200},
	{320, 240},
	{400, 300},
	{512, 384},
};

int			vid_modenum = NO_MODE;
int			vid_testingmode, vid_realmode;
double		vid_testendtime;
int			vid_default = MODE_WINDOWED;
static int	windowed_default;

typedef int modestate_t;

// modestate_t	modestate = MS_UNINIT;
modestate_t	modestate = 0;

static byte		*vid_surfcache;
static int		vid_surfcachesize;
static int		VID_highhunkmark;

unsigned char	vid_curpal[256*3];

unsigned short	d_8to16table[256];
unsigned	d_8to24table[256];

// int     driver = mode;
int     driver = 0;
BOOL    useWinDirect = TRUE;
BOOL	useDirectDraw = TRUE;
// MGLDC	*mgldc = NULL,*memdc = NULL,*dibdc = NULL,*windc = NULL;

typedef struct {
	modestate_t	type;
	int			width;
	int			height;
	int			modenum;
	int			mode13;
	int			stretched;
	int			dib;
	int			fullscreen;
	int			bpp;
	int			halfscreen;
	char		modedesc[13];
} vmode_t;

static vmode_t	modelist[MAX_MODE_LIST];
static int		nummodes;
static vmode_t	*pcurrentmode;

int		aPage;					// Current active display page
int		vPage;					// Current visible display page
int		waitVRT = qtrue;			// True to wait for retrace on flip

static vmode_t	badmode;

static byte	backingbuf[48*24];

HBITMAP hBmp;
HDC hdcDib;
HGDIOBJ hdlSel;
u32 *fbufDib;

void VID_MenuDraw (void);
void VID_MenuKey (int key);

LONG WINAPI MainWndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void AppActivate(BOOL fActive, BOOL minimize);

void GLimp_Init (void);

void	VID_Update (vrect_t *rects);

void	VID_Init (unsigned char *palette);


/*
================
VID_RememberWindowPos
================
*/
void VID_RememberWindowPos (void)
{
	RECT	rect;
}


/*
================
VID_CheckWindowXY
================
*/
void VID_CheckWindowXY (void)
{
}


/*
================
VID_UpdateWindowStatus
================
*/
void VID_UpdateWindowStatus (void)
{

	window_rect.left = window_x;
	window_rect.top = window_y;
	window_rect.right = window_x + window_width;
	window_rect.bottom = window_y + window_height;
	window_center_x = (window_rect.left + window_rect.right) / 2;
	window_center_y = (window_rect.top + window_rect.bottom) / 2;

//	IN_UpdateClipCursor ();
}


/*
================
ClearAllStates
================
*/
void ClearAllStates (void)
{
	int		i;
	
// send an up event for each key, to make sure the server clears them all
//	for (i=0 ; i<256 ; i++)
//	{
//		Key_Event (i, qfalse);
//	}

	Key_ClearStates ();
	IN_ClearStates ();
}


/*
================
VID_CheckAdequateMem
================
*/
qboolean VID_CheckAdequateMem (int width, int height)
{
	int		tbuffersize;

	return qtrue;
}


/*
================
VID_AllocBuffers
================
*/
qboolean VID_AllocBuffers (int width, int height)
{
	int		tsize, tbuffersize;

//	VID_highhunkmark = Hunk_HighMark ();
	
	return qtrue;
}


void initFatalError(void)
{
	exit(EXIT_FAILURE);
}

void registerAllDispDrivers(void)
{
}


void registerAllMemDrivers(void)
{
	/* Register memory context drivers */
}

#if 0
/*
=================
VID_InitFullDIB
=================
*/
void VID_InitFullDIB (HINSTANCE hInstance)
{
	DEVMODE	devmode;
	int		i, j, modenum, cmodes, existingmode, originalnummodes, lowestres;
	int		numlowresmodes, bpp, done;
	int		cstretch, istretch, mstretch;
	BOOL	stat;

// enumerate 8 bpp modes
	originalnummodes = nummodes;
	modenum = 0;
	lowestres = 99999;

	do
	{
		stat = EnumDisplaySettings (NULL, modenum, &devmode);

		if ((devmode.dmBitsPerPel == 8) &&
			(devmode.dmPelsWidth <= MAXWIDTH) &&
			(devmode.dmPelsHeight <= MAXHEIGHT) &&
			(nummodes < MAX_MODE_LIST))
		{
			devmode.dmFields = DM_BITSPERPEL |
							   DM_PELSWIDTH |
							   DM_PELSHEIGHT;

			if (ChangeDisplaySettings (&devmode, CDS_TEST | CDS_FULLSCREEN) ==
					DISP_CHANGE_SUCCESSFUL)
			{
//				modelist[nummodes].type = MS_FULLDIB;
				modelist[nummodes].width = devmode.dmPelsWidth;
				modelist[nummodes].height = devmode.dmPelsHeight;
				modelist[nummodes].modenum = 0;
				modelist[nummodes].mode13 = 0;
				modelist[nummodes].stretched = 0;
				modelist[nummodes].halfscreen = 0;
				modelist[nummodes].dib = 1;
				modelist[nummodes].fullscreen = 1;
				modelist[nummodes].bpp = devmode.dmBitsPerPel;
				sprintf (modelist[nummodes].modedesc, "%dx%d",
						 devmode.dmPelsWidth, devmode.dmPelsHeight);

			// if the width is more than twice the height, reduce it by half because this
			// is probably a dual-screen monitor
				if (!COM_CheckParm("-noadjustaspect"))
				{
					if (modelist[nummodes].width > (modelist[nummodes].height << 1))
					{
						modelist[nummodes].width >>= 1;
						modelist[nummodes].halfscreen = 1;
						sprintf (modelist[nummodes].modedesc, "%dx%d",
								 modelist[nummodes].width,
								 modelist[nummodes].height);
					}
				}

				for (i=originalnummodes, existingmode = 0 ; i<nummodes ; i++)
				{
					if ((modelist[nummodes].width == modelist[i].width) &&
						(modelist[nummodes].height == modelist[i].height))
					{
						existingmode = 1;
						break;
					}
				}

				if (!existingmode)
				{
					if (modelist[nummodes].width < lowestres)
						lowestres = modelist[nummodes].width;

					nummodes++;
				}
			}
		}

		modenum++;
	} while (stat);

// see if any of them were actually settable; if so, this is our mode list,
// else enumerate all modes; our mode list is whichever ones are settable
// with > 8 bpp
	if (nummodes == originalnummodes)
	{
		modenum = 0;
		lowestres = 99999;

		Con_SafePrintf ("No 8-bpp fullscreen DIB modes found\n");

		do
		{
			stat = EnumDisplaySettings (NULL, modenum, &devmode);

			if ((((devmode.dmPelsWidth <= MAXWIDTH) &&
				  (devmode.dmPelsHeight <= MAXHEIGHT)) ||
				 (!COM_CheckParm("-noadjustaspect") &&
				  (devmode.dmPelsWidth <= (MAXWIDTH*2)) &&
				  (devmode.dmPelsWidth > (devmode.dmPelsHeight*2)))) &&
				(nummodes < MAX_MODE_LIST) &&
				(devmode.dmBitsPerPel > 8))
			{
				devmode.dmFields = DM_BITSPERPEL |
								   DM_PELSWIDTH |
								   DM_PELSHEIGHT;

				if (ChangeDisplaySettings (&devmode, CDS_TEST | CDS_FULLSCREEN) ==
						DISP_CHANGE_SUCCESSFUL)
				{
//					modelist[nummodes].type = MS_FULLDIB;
					modelist[nummodes].width = devmode.dmPelsWidth;
					modelist[nummodes].height = devmode.dmPelsHeight;
					modelist[nummodes].modenum = 0;
					modelist[nummodes].mode13 = 0;
					modelist[nummodes].stretched = 0;
					modelist[nummodes].halfscreen = 0;
					modelist[nummodes].dib = 1;
					modelist[nummodes].fullscreen = 1;
					modelist[nummodes].bpp = devmode.dmBitsPerPel;
					sprintf (modelist[nummodes].modedesc, "%dx%d",
							 devmode.dmPelsWidth, devmode.dmPelsHeight);

				// if the width is more than twice the height, reduce it by half because this
				// is probably a dual-screen monitor
					if (!COM_CheckParm("-noadjustaspect"))
					{
						if (modelist[nummodes].width > (modelist[nummodes].height*2))
						{
							modelist[nummodes].width >>= 1;
							modelist[nummodes].halfscreen = 1;
							sprintf (modelist[nummodes].modedesc, "%dx%d",
									 modelist[nummodes].width,
									 modelist[nummodes].height);
						}
					}

					for (i=originalnummodes, existingmode = 0 ; i<nummodes ; i++)
					{
						if ((modelist[nummodes].width == modelist[i].width) &&
							(modelist[nummodes].height == modelist[i].height))
						{
						// pick the lowest available bpp
							if (modelist[nummodes].bpp < modelist[i].bpp)
								modelist[i] = modelist[nummodes];

							existingmode = 1;
							break;
						}
					}

					if (!existingmode)
					{
						if (modelist[nummodes].width < lowestres)
							lowestres = modelist[nummodes].width;

						nummodes++;
					}
				}
			}

			modenum++;
		} while (stat);
	}

// see if there are any low-res modes that aren't being reported
	numlowresmodes = sizeof(lowresmodes) / sizeof(lowresmodes[0]);
	bpp = 8;
	done = 0;

// first make sure the driver doesn't just answer yes to all tests
	devmode.dmBitsPerPel = 8;
	devmode.dmPelsWidth = 42;
	devmode.dmPelsHeight = 37;
	devmode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	if (ChangeDisplaySettings (&devmode, CDS_TEST | CDS_FULLSCREEN) ==
			DISP_CHANGE_SUCCESSFUL)
	{
		done = 1;
	}

	while (!done)
	{
		for (j=0 ; (j<numlowresmodes) && (nummodes < MAX_MODE_LIST) ; j++)
		{
			devmode.dmBitsPerPel = bpp;
			devmode.dmPelsWidth = lowresmodes[j].width;
			devmode.dmPelsHeight = lowresmodes[j].height;
			devmode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			if (ChangeDisplaySettings (&devmode, CDS_TEST | CDS_FULLSCREEN) ==
					DISP_CHANGE_SUCCESSFUL)
			{
//					modelist[nummodes].type = MS_FULLDIB;
					modelist[nummodes].width = devmode.dmPelsWidth;
					modelist[nummodes].height = devmode.dmPelsHeight;
					modelist[nummodes].modenum = 0;
					modelist[nummodes].mode13 = 0;
					modelist[nummodes].stretched = 0;
					modelist[nummodes].halfscreen = 0;
					modelist[nummodes].dib = 1;
					modelist[nummodes].fullscreen = 1;
					modelist[nummodes].bpp = devmode.dmBitsPerPel;
					sprintf (modelist[nummodes].modedesc, "%dx%d",
							 devmode.dmPelsWidth, devmode.dmPelsHeight);

			// we only want the lowest-bpp version of each mode
				for (i=originalnummodes, existingmode = 0 ; i<nummodes ; i++)
				{
					if ((modelist[nummodes].width == modelist[i].width)   &&
						(modelist[nummodes].height == modelist[i].height) &&
						(modelist[nummodes].bpp >= modelist[i].bpp))
					{
						existingmode = 1;
						break;
					}
				}

				if (!existingmode)
				{
					if (modelist[nummodes].width < lowestres)
						lowestres = modelist[nummodes].width;

					nummodes++;
				}
			}
		}

		switch (bpp)
		{
			case 8:
				bpp = 16;
				break;

			case 16:
				bpp = 32;
				break;

			case 32:
				done = 1;
				break;
		}
	}

// now add the lowest stretch-by-2 pseudo-modes between 320-wide
// (inclusive) and lowest real res (not inclusive)
// don't bother if we have a real VGA mode 0x13 mode
	if (!is_mode0x13)
	{
		for (i=originalnummodes, cstretch=0 ; i<nummodes ; i++)
		{
			if (((modelist[i].width >> 1) < lowestres) &&
				((modelist[i].width >> 1) >= 320))
			{
				lowestres = modelist[i].width >> 1;
				cstretch = 1;
				mstretch = i;
			}
		}

		if ((nummodes + cstretch) > MAX_MODE_LIST)
			cstretch = MAX_MODE_LIST - nummodes;

		if (cstretch > 0)
		{
			for (i=(nummodes-1) ; i>=originalnummodes ; i--)
				modelist[i+cstretch] = modelist[i];

			nummodes += cstretch;
			istretch = originalnummodes;

			modelist[istretch] = modelist[mstretch];
			modelist[istretch].width >>= 1;
			modelist[istretch].height >>= 1;
			modelist[istretch].stretched = 1;
			sprintf (modelist[istretch].modedesc, "%dx%d",
					 modelist[istretch].width, modelist[istretch].height);
		}
	}

	if (nummodes != originalnummodes)
		vid_default = MODE_FULLSCREEN_DEFAULT;
	else
		Con_SafePrintf ("No fullscreen DIB modes found\n");
}
#endif

/*
=================
VID_NumModes
=================
*/
int VID_NumModes (void)
{
	return nummodes;
}

	
/*
=================
VID_GetModePtr
=================
*/
vmode_t *VID_GetModePtr (int modenum)
{

	if ((modenum >= 0) && (modenum < nummodes))
		return &modelist[modenum];
	else
		return &badmode;
}


/*
=================
VID_CheckModedescFixup
=================
*/
void VID_CheckModedescFixup (int mode)
{
	int		x, y, stretch;

#if 0
	if (mode == MODE_SETTABLE_WINDOW)
	{
//		modelist[mode].stretched = (int)vid_stretch_by_2.value;
//		stretch = modelist[mode].stretched;
		stretch = 0;

		if (vid_config_x.value < (320 << stretch))
			vid_config_x.value = 320 << stretch;

		if (vid_config_y.value < (200 << stretch))
			vid_config_y.value = 200 << stretch;

		x = (int)vid_config_x.value;
		y = (int)vid_config_y.value;
		sprintf (modelist[mode].modedesc, "%dx%d", x, y);
		modelist[mode].width = x;
		modelist[mode].height = y;
	}
#endif
}


/*
=================
VID_GetModeDescriptionMemCheck
=================
*/
char *VID_GetModeDescriptionMemCheck (int mode)
{
	char		*pinfo;
	vmode_t		*pv;

	if ((mode < 0) || (mode >= nummodes))
		return NULL;

	VID_CheckModedescFixup (mode);

	pv = VID_GetModePtr (mode);
	pinfo = pv->modedesc;

	if (VID_CheckAdequateMem (pv->width, pv->height))
	{
		return pinfo;
	}
	else
	{
		return NULL;
	}
}


/*
=================
VID_GetModeDescription
=================
*/
char *VID_GetModeDescription (int mode)
{
	char		*pinfo;
	vmode_t		*pv;

	if ((mode < 0) || (mode >= nummodes))
		return NULL;

	VID_CheckModedescFixup (mode);

	pv = VID_GetModePtr (mode);
	pinfo = pv->modedesc;
	return pinfo;
}


#if 0
/*
=================
VID_GetModeDescription2

Tacks on "windowed" or "fullscreen"
=================
*/
char *VID_GetModeDescription2 (int mode)
{
	static char	pinfo[40];
	vmode_t		*pv;

	if ((mode < 0) || (mode >= nummodes))
		return NULL;

	VID_CheckModedescFixup (mode);

	pv = VID_GetModePtr (mode);

#if 0
	if (modelist[mode].type == MS_FULLSCREEN)
	{
		sprintf(pinfo,"%s fullscreen", pv->modedesc);
	}
	else if (modelist[mode].type == MS_FULLDIB)
	{
		sprintf(pinfo,"%s fullscreen", pv->modedesc);
	}
	else
	{
		sprintf(pinfo, "%s windowed", pv->modedesc);
	}
#endif

	return pinfo;
}


// KJB: Added this to return the mode driver name in description for console

char *VID_GetExtModeDescription (int mode)
{
	static char	pinfo[40];
	vmode_t		*pv;

	if ((mode < 0) || (mode >= nummodes))
		return NULL;

	VID_CheckModedescFixup (mode);

	pv = VID_GetModePtr (mode);
	if (modelist[mode].type == MS_FULLSCREEN)
	{
	}
	else if (modelist[mode].type == MS_FULLDIB)
	{
		sprintf(pinfo,"%s fullscreen DIB", pv->modedesc);
	}
	else
	{
		sprintf(pinfo, "%s windowed", pv->modedesc);
	}

	return pinfo;
}
#endif

void DestroyDIBWindow (void)
{

//	if (modestate == MS_WINDOWED)
//	{
//	}
}


void DestroyFullscreenWindow (void)
{
//	if (modestate == MS_FULLSCREEN)
//	{
//	}
}



void DestroyFullDIBWindow (void)
{
//	if (modestate == MS_FULLDIB)
//	{
//		ChangeDisplaySettings (NULL, CDS_FULLSCREEN);
//	}
}

int r_pixbytes;

qboolean VID_SetWindowedMode (int modenum)
{
	HDC				hdc;
	BITMAPINFO dib, *pdib;
	void			*pix;
	void			*ractx;
//	pixel_format_t	pf;
	qboolean		stretched;
	int				lastmodestate;
	LONG			wlong;
	int cx, cy, wsx, wsy;

#if 0
	if (!windowed_mode_set)
	{
		if (COM_CheckParm ("-resetwinpos"))
		{
			Cvar_SetValue ("vid_window_x", 0.0);
			Cvar_SetValue ("vid_window_y", 0.0);
		}

		windowed_mode_set;
	}
#endif

	VID_CheckModedescFixup (modenum);

	DDActive = 0;
	lastmodestate = modestate;

	DestroyFullscreenWindow ();
	DestroyFullDIBWindow ();

//	if (windc)
//		MGL_destroyDC(windc);
//	if (dibdc)
//		MGL_destroyDC(dibdc);
//	windc = dibdc = NULL;

// KJB: Signal to the MGL that we are going back to windowed mode
//	if (!MGL_changeDisplayMode(grWINDOWED))
//		initFatalError();

	WindowRect.top = WindowRect.left = 0;

	WindowRect.right = modelist[modenum].width;
	WindowRect.bottom = modelist[modenum].height;
	stretched = modelist[modenum].stretched;
	stretched=0;

	DIBWidth = modelist[modenum].width;
	DIBHeight = modelist[modenum].height;

	if (stretched)
	{
		DIBWidth >>= 1;
		DIBHeight >>= 1;
	}

	WindowStyle = WS_OVERLAPPED | WS_BORDER | WS_CAPTION | WS_SYSMENU |
				  WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPSIBLINGS |
				  WS_CLIPCHILDREN;
	ExWindowStyle = 0;
	AdjustWindowRectEx(&WindowRect, WindowStyle, FALSE, 0);

// the first time we're called to set the mode, create the window we'll use
// for the rest of the session
	if (!vid_mode_set)
	{
		mainwindow = CreateWindowEx (
			 ExWindowStyle,
			 "WinQuake",
			 "WinQuake",
			 WindowStyle,
			 0, 0,
			 WindowRect.right - WindowRect.left,
			 WindowRect.bottom - WindowRect.top,
			 NULL,
			 NULL,
//			 g_wv.hInstance,
			 g_wv.hInstance,
			 NULL);

		if (!mainwindow)
			Sys_Error ("Couldn't create DIB window");

	// tell MGL to use this window for fullscreen modes
//		MGL_registerFullScreenWindow (mainwindow);

		vid_mode_set = qtrue;
	}
	else
	{
		SetWindowLong(mainwindow, GWL_STYLE, WindowStyle | WS_VISIBLE);
		SetWindowLong(mainwindow, GWL_EXSTYLE, ExWindowStyle);
	}

	wsx=WindowRect.right - WindowRect.left;
	wsy=WindowRect.bottom - WindowRect.top;
	cx=GetSystemMetrics(SM_CXSCREEN);
	cy=GetSystemMetrics(SM_CYSCREEN);
	cx=(cx-wsx)/2; cy=(cy-wsy)/2;
	if(cx<0)cx=0;
	if(cy<0)cy=0;

	window_center_x=cx+(wsx/2);
	window_center_y=cy+(wsy/2)+24;

//	Cvar_SetValue ("vid_window_x", cx);
//	Cvar_SetValue ("vid_window_y", cy);

	window_width=wsx;
	window_height=wsy;

	if (!SetWindowPos (mainwindow,
					   NULL,
					   cx, cy,
					   WindowRect.right - WindowRect.left,
					   WindowRect.bottom - WindowRect.top,
					   SWP_NOCOPYBITS | SWP_NOZORDER |
						SWP_HIDEWINDOW))
	{
		Sys_Error ("Couldn't resize DIB window");
	}

	if (hide_window)
		return qtrue;

// position and show the DIB window
	VID_CheckWindowXY ();
//	SetWindowPos (mainwindow, NULL, (int)vid_window_x.value,
//				  (int)vid_window_y.value, 0, 0,
//				  SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW | SWP_DRAWFRAME);

	if (force_minimized)
		ShowWindow (mainwindow, SW_MINIMIZE);
	else
		ShowWindow (mainwindow, SW_SHOWDEFAULT);

	UpdateWindow (mainwindow);

//	modestate = MS_WINDOWED;
	vid_fulldib_on_focus_mode = 0;

// because we have set the background brush for the window to NULL
// (to avoid flickering when re-sizing the window on the desktop),
// we clear the window to black when created, otherwise it will be
// empty while Quake starts up.
	hdc = GetDC(mainwindow);
	PatBlt(hdc,0,0,WindowRect.right,WindowRect.bottom,BLACKNESS);
//	ReleaseDC(mainwindow, hdc);

	pdib=&dib;
	memset(pdib, 0, sizeof(BITMAPINFO));
	
	pdib->bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
	pdib->bmiHeader.biWidth         = DIBWidth;
	pdib->bmiHeader.biHeight        = DIBHeight;
	pdib->bmiHeader.biPlanes        = 1;
	pdib->bmiHeader.biBitCount      = 32;
	pdib->bmiHeader.biCompression   = BI_RGB;
	pdib->bmiHeader.biSizeImage     = 0;
	pdib->bmiHeader.biXPelsPerMeter = 0;
	pdib->bmiHeader.biYPelsPerMeter = 0;
	pdib->bmiHeader.biClrUsed       = 0;
	pdib->bmiHeader.biClrImportant  = 0;

	pix=NULL;
	hBmp=CreateDIBSection(hdc, pdib, DIB_RGB_COLORS, &pix, NULL, 0);

	GdiFlush();
	memset(pix, 0, DIBWidth*DIBHeight*4);
	fbufDib=pix;
	
	hdcDib=CreateCompatibleDC(hdc);
	hdlSel=SelectObject(hdcDib, hBmp);

	ReleaseDC(mainwindow, hdc);

	/* Create the MGL window DC and the MGL memory DC */
//	if ((windc = MGL_createWindowedDC(mainwindow)) == NULL)
//		MGL_fatalError("Unable to create Windowed DC!");

//	if ((dibdc = MGL_createMemoryDC(DIBWidth,DIBHeight,8,&pf)) == NULL)
//		MGL_fatalError("Unable to create Memory DC!");

//	MGL_makeCurrentDC(dibdc);

#if 1
//	vid_buffer = malloc(DIBWidth*DIBHeight*4);
	vid_buffer = Q_Malloc(DIBWidth*DIBHeight*4);
	vid_height = DIBHeight;
	vid_width = DIBWidth;

	for(cy=0; cy<DIBHeight; cy++)
		for(cx=0; cx<DIBWidth; cx++)
	{
		((short *)(vid_buffer))[cy*DIBWidth+cx]=rand();
	}
#endif

	TKRA_SetupContextBasic(DIBWidth, DIBHeight);

	QGL_Init(NULL);
	
//	GLimp_Init();

	vid_stretched = stretched;

	SendMessage (mainwindow, WM_SETICON, (WPARAM)TRUE, (LPARAM)hIcon);
	SendMessage (mainwindow, WM_SETICON, (WPARAM)FALSE, (LPARAM)hIcon);

	return qtrue;
}

void VID_SetDefaultMode (void)
{
	if (vid_initialized)
		VID_SetMode (0, vid_curpal);

//	IN_DeactivateMouse ();
}


int VID_SetMode (int modenum, unsigned char *palette)
{
	int				original_mode, temp, dummy;
	qboolean		stat;
    MSG				msg;
	HDC				hdc;

	while ((modenum >= nummodes) || (modenum < 0))
	{
		if (vid_modenum == NO_MODE)
		{
			if (modenum == vid_default)
			{
				modenum = windowed_default;
			}
			else
			{
				modenum = vid_default;
			}

			Cvar_SetValue ("vid_mode", (float)modenum);
		}
		else
		{
			Cvar_SetValue ("vid_mode", (float)vid_modenum);
			return 0;
		}
	}

	if (!force_mode_set && (modenum == vid_modenum))
		return qtrue;

// so Con_Printfs don't mess us up by forcing vid and snd updates
//	temp = scr_disabled_for_loading;
//	scr_disabled_for_loading = qtrue;
	in_mode_set = qtrue;

//	CDAudio_Pause ();
//	S_ClearBuffer ();

	if (vid_modenum == NO_MODE)
		original_mode = windowed_default;
	else
		original_mode = vid_modenum;

	stat = VID_SetWindowedMode(modenum);

#if 0
//	if (_windowed_mouse.value)
	if(0)
	{
		stat = VID_SetWindowedMode(modenum);
		IN_ActivateMouse ();
		IN_HideMouse ();
	}
	else
	{
		IN_DeactivateMouse ();
		IN_ShowMouse ();
		stat = VID_SetWindowedMode(modenum);
	}
#endif

#if 0
	// Set either the fullscreen or windowed mode
	if (modelist[modenum].type == MS_WINDOWED)
	{
		if (_windowed_mouse.value)
		{
			stat = VID_SetWindowedMode(modenum);
			IN_ActivateMouse ();
			IN_HideMouse ();
		}
		else
		{
			IN_DeactivateMouse ();
			IN_ShowMouse ();
			stat = VID_SetWindowedMode(modenum);
		}
	}
	else if (modelist[modenum].type == MS_FULLDIB)
	{
		stat = VID_SetFullDIBMode(modenum);
		IN_ActivateMouse ();
		IN_HideMouse ();
	}
	else
	{
		stat = VID_SetFullscreenMode(modenum);
		IN_ActivateMouse ();
		IN_HideMouse ();
	}
#endif

//	window_width = DIBWidth;
//	window_height = DIBHeight;

//	window_width = vid.width << vid_stretched;
//	window_height = vid.height << vid_stretched;
	VID_UpdateWindowStatus ();

//	CDAudio_Resume ();
//	scr_disabled_for_loading = temp;

	if (!stat)
	{
//		VID_RestoreOldMode (original_mode);
		return qfalse;
	}

//	if (hide_window)
//		return qtrue;

// now we try to make sure we get the focus on the mode switch, because
// sometimes in some systems we don't.  We grab the foreground, then
// finish setting up, pump all our messages, and sleep for a little while
// to let messages finish bouncing around the system, then we put
// ourselves at the top of the z order, then grab the foreground again,
// Who knows if it helps, but it probably doesn't hurt
	if (!force_minimized)
		SetForegroundWindow (mainwindow);

	hdc = GetDC(NULL);

//	if (GetDeviceCaps(hdc, RASTERCAPS) & RC_PALETTE)
//		vid_palettized = qtrue;
//	else
		vid_palettized = qfalse;

//	VID_SetPalette (palette);

	ReleaseDC(NULL,hdc);

	vid_modenum = modenum;
	Cvar_SetValue ("vid_mode", (float)vid_modenum);

#if 0
	if (!VID_AllocBuffers (vid.width, vid.height))
	{
	// couldn't get memory for this mode; try to fall back to previous mode
		VID_RestoreOldMode (original_mode);
		return qfalse;
	}

//	D_InitCaches (vid_surfcache, vid_surfcachesize);
#endif

	while (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
	{
      	TranslateMessage (&msg);
      	DispatchMessage (&msg);
	}

	Sleep (100);

	if (!force_minimized)
	{
		SetWindowPos (mainwindow, HWND_TOP, 0, 0, 0, 0,
				  SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW |
				  SWP_NOCOPYBITS);

		SetForegroundWindow (mainwindow);
	}

// fix the leftover Alt from any Alt-Tab or the like that switched us away
	ClearAllStates ();

//	if (!msg_suppress_1)
//		Con_SafePrintf ("%s\n", VID_GetModeDescription (vid_modenum));

//	VID_SetPalette (palette);

	in_mode_set = qfalse;
//	vid.recalc_refdef = 1;

	return qtrue;
}

void VID_LockBuffer (void)
{
}
		
		
void VID_UnlockBuffer (void)
{
}


int VID_ForceUnlockedAndReturnState (void)
{
	int	lk;

	lk=0;

	return lk;
}


void VID_ForceLockState (int lk)
{

//	if (!dibdc && lk)
	if (lk)
	{
		lockcount = 0;
		VID_LockBuffer ();
	}

	lockcount = lk;
}

int vid_clamp255(int v)
{
	if(v<0)return(0);
	if(v>255)return(255);
	return(v);
}

void GLimp_LogComment( char *comment ) 
{
}

void GLimp_WakeRenderer( void *data )
{
}

void GLimp_Shutdown( void )
{
}

void *GLimp_RendererSleep( void )
{
}

qboolean GLimp_SpawnRenderThread( void (*function)( void ) )
{
	__debugbreak();
	return qfalse;
}

void GLimp_FrontEndSleep( void )
{
}


void GLimp_Init (void)
{
	cvar_t	*cv;

	if(vid_initialized)
		return;

	ri.Printf( PRINT_ALL, "Initializing OpenGL subsystem\n" );

	cv = ri.Cvar_Get( "win_hinstance", "", 0 );
//	sscanf( cv->string, "%i", (int *)&g_wv.hInstance );
	sscanf( cv->string, "%p", &g_wv.hInstance );

	cv = ri.Cvar_Get( "win_wndproc", "", 0 );
//	sscanf( cv->string, "%i", (int *)&glw_state.wndproc );
	sscanf( cv->string, "%p", &glw_state.wndproc );

	VID_Init(NULL);

	QGL_Init(NULL);
	
//	VID_SetMode(MODE_WINDOWED, NULL);

	gl_vendor = qglGetString (GL_VENDOR);
	ri.Printf( PRINT_ALL, "GL_VENDOR: %s\n", gl_vendor);
	gl_renderer = qglGetString (GL_RENDERER);
	ri.Printf( PRINT_ALL, "GL_RENDERER: %s\n", gl_renderer);

	gl_version = qglGetString (GL_VERSION);
	ri.Printf( PRINT_ALL, "GL_VERSION: %s\n", gl_version);
	gl_extensions = qglGetString (GL_EXTENSIONS);
//	ri.Printf( PRINT_ALL, "GL_EXTENSIONS: %s\n", gl_extensions);

	Q_strncpyz( glConfig.vendor_string, gl_vendor,
		sizeof( glConfig.vendor_string ) );
	Q_strncpyz( glConfig.renderer_string, gl_renderer,
		sizeof( glConfig.renderer_string ) );
	Q_strncpyz( glConfig.version_string, gl_version,
		sizeof( glConfig.version_string ) );
	Q_strncpyz( glConfig.extensions_string, gl_extensions,
		sizeof( glConfig.extensions_string ) );

	glConfig.stereoEnabled = qfalse;

	glConfig.driverType = GLDRV_STANDALONE;
	glConfig.colorBits = 16;
	glConfig.depthBits = 16;
	glConfig.stencilBits = 0;

	glConfig.vidWidth = DIBWidth;
	glConfig.vidHeight = DIBHeight;
	glConfig.windowAspect = (1.0 * DIBWidth) / DIBHeight;
	glConfig.displayFrequency = 60;
	glConfig.isFullscreen = 0;

	glConfig.textureCompression = TC_S3TC;
	glConfig.textureEnvAddAvailable = qfalse;

	qglClearColor (1,0,0,0);
	qglCullFace(GL_FRONT);
	qglEnable(GL_TEXTURE_2D);

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
}

#if 0
void GL_BeginRendering (int *x, int *y, int *width, int *height)
{
	extern cvar_t gl_clear;

	*x = *y = 0;
//	*width = WindowRect.right - WindowRect.left;
//	*height = WindowRect.bottom - WindowRect.top;
	*width = vid.width;
	*height = vid.height;

//    if (!qwglMakeCurrent( maindc, baseRC ))
//		Sys_Error ("wglMakeCurrent failed");

//	qglViewport (*x, *y, *width, *height);
}
#endif

// extern qboolean	mouseactive;  // from in_win.c

void *TKRA_GetCurrentScreenBuffer_RGB(void);

void GLimp_EndFrame (void)
{
	int y, yn;
	void *sbuf;
	vrect_t	rect;

//	if (!scr_skipupdate || block_drawing)
//		SwapBuffers(maindc);

#if 0
// handle the mouse state when windowed if that's changed
//	if (modestate == MS_WINDOWED)
	if(1)
	{
//		if (!_windowed_mouse.value)
		if(1)
		{
			if (windowed_mouse)	{
//				IN_DeactivateMouse ();
//				IN_ShowMouse ();
				windowed_mouse = qfalse;
			}
		} else {
#if 0
			windowed_mouse = qtrue;
			if (key_dest == key_game && !mouseactive && ActiveApp) {
				IN_ActivateMouse ();
				IN_HideMouse ();
			} else if (mouseactive && key_dest != key_game) {
				IN_DeactivateMouse ();
				IN_ShowMouse ();
			}
#endif
		}
	}
#endif

//	Sys_SendKeyEvents();

	rect.x = 0;
	rect.y = 0;
	rect.width = vid_width;
	rect.height = vid_height;
//	rect.pnext = 0;
	
	sbuf=TKRA_GetCurrentScreenBuffer_RGB();
	if(sbuf)
	{
//		memcpy(vid.buffer, sbuf, vid.width*vid.height*2);

		for(y=0; y<vid_height; y++)
		{
			yn=vid_height-y-1;
			memcpy(
				((u16 *)(vid_buffer))+(y*vid_width),
				((u16 *)(sbuf))+(yn*vid_width),
				vid_width*2);
		}

	}

	VID_Update(&rect);
}


void	VID_Init (unsigned char *palette)
{
	WNDCLASS		wc;
	HDC				hdc;
//	int				i;

	int		i, bestmatch, bestmatchmetric, t, dr, dg, db;
	int		basenummodes;
	byte	*ptmp;

#if 0
	Cvar_RegisterVariable (&vid_mode);
	Cvar_RegisterVariable (&vid_wait);
	Cvar_RegisterVariable (&vid_nopageflip);
	Cvar_RegisterVariable (&_vid_wait_override);
	Cvar_RegisterVariable (&_vid_default_mode);
	Cvar_RegisterVariable (&_vid_default_mode_win);
	Cvar_RegisterVariable (&vid_config_x);
	Cvar_RegisterVariable (&vid_config_y);
	Cvar_RegisterVariable (&vid_stretch_by_2);
	Cvar_RegisterVariable (&_windowed_mouse);
	Cvar_RegisterVariable (&vid_fullscreen_mode);
	Cvar_RegisterVariable (&vid_windowed_mode);
	Cvar_RegisterVariable (&block_switch);
	Cvar_RegisterVariable (&vid_window_x);
	Cvar_RegisterVariable (&vid_window_y);

	Cmd_AddCommand ("vid_testmode", VID_TestMode_f);
	Cmd_AddCommand ("vid_nummodes", VID_NumModes_f);
	Cmd_AddCommand ("vid_describecurrentmode", VID_DescribeCurrentMode_f);
	Cmd_AddCommand ("vid_describemode", VID_DescribeMode_f);
	Cmd_AddCommand ("vid_describemodes", VID_DescribeModes_f);
	Cmd_AddCommand ("vid_forcemode", VID_ForceMode_f);
	Cmd_AddCommand ("vid_windowed", VID_Windowed_f);
	Cmd_AddCommand ("vid_fullscreen", VID_Fullscreen_f);
	Cmd_AddCommand ("vid_minimize", VID_Minimize_f);
#endif

	dibonly = qtrue;

//	VID_InitMGLDIB (g_wv.hInstance);

//	hIcon = LoadIcon (g_wv.hInstance, MAKEINTRESOURCE (IDI_ICON2));

	/* Register the frame class */
    wc.style         = 0;
    wc.lpfnWndProc   = (WNDPROC)MainWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = g_wv.hInstance;
    wc.hIcon         = 0;
    wc.hCursor       = LoadCursor (NULL,IDC_ARROW);
	wc.hbrBackground = NULL;
    wc.lpszMenuName  = 0;
    wc.lpszClassName = "WinQuake";

    if (!RegisterClass (&wc) )
		Sys_Error ("Couldn't register window class");

#if 1
//	modelist[0].type = MS_WINDOWED;
	modelist[0].width = 320;
	modelist[0].height = 240;
	strcpy (modelist[0].modedesc, "320x240");
	modelist[0].mode13 = 0;
	modelist[0].modenum = MODE_WINDOWED;
	modelist[0].stretched = 0;
	modelist[0].dib = 1;
	modelist[0].fullscreen = 0;
	modelist[0].halfscreen = 0;
	modelist[0].bpp = 8;

//	modelist[1].type = MS_WINDOWED;
	modelist[1].width = 640;
	modelist[1].height = 480;
	strcpy (modelist[1].modedesc, "640x480");
	modelist[1].mode13 = 0;
	modelist[1].modenum = MODE_WINDOWED + 1;
	modelist[1].stretched = 1;
	modelist[1].dib = 1;
	modelist[1].fullscreen = 0;
	modelist[1].halfscreen = 0;
	modelist[1].bpp = 8;

//	modelist[2].type = MS_WINDOWED;
	modelist[2].width = 800;
	modelist[2].height = 600;
	strcpy (modelist[2].modedesc, "800x600");
	modelist[2].mode13 = 0;
	modelist[2].modenum = MODE_WINDOWED + 2;
	modelist[2].stretched = 1;
	modelist[2].dib = 1;
	modelist[2].fullscreen = 0;
	modelist[2].halfscreen = 0;
	modelist[2].bpp = 8;

//	vid_default = MODE_WINDOWED;
//	vid_default = 2;
	vid_default = 1;

	windowed_default = vid_default;

	nummodes = 3;	// reserve space for windowed mode
#endif


	basenummodes = nummodes;

	vid_testingmode = 0;

#if 0
// GDI doesn't let us remap palette index 0, so we'll remap color
// mappings from that black to another one
	bestmatchmetric = 256*256*3;

	for (i=1 ; i<256 ; i++)
	{
		dr = palette[0] - palette[i*3];
		dg = palette[1] - palette[i*3+1];
		db = palette[2] - palette[i*3+2];

		t = (dr * dr) + (dg * dg) + (db * db);

		if (t < bestmatchmetric)
		{
			bestmatchmetric = t;
			bestmatch = i;

			if (t == 0)
				break;
		}
	}

//	for (i=0, ptmp = vid.colormap ; i<(1<<(VID_CBITS+8)) ; i++, ptmp++)
//	{
//		if (*ptmp == 0)
//			*ptmp = bestmatch;
//	}
#endif

	startwindowed = 1;

//	if (hwnd_dialog)
//		DestroyWindow (hwnd_dialog);

// sound initialization has to go here, preceded by a windowed mode set,
// so there's a window for DirectSound to work with but we're not yet
// fullscreen so the "hardware already in use" dialog is visible if it
// gets displayed

// keep the window minimized until we're ready for the first real mode set
	hide_window = qtrue;
	VID_SetMode (MODE_WINDOWED, palette);
	hide_window = qfalse;
	S_Init ();

	vid_initialized = qtrue;

	force_mode_set = qtrue;
	VID_SetMode (vid_default, palette);
	force_mode_set = qfalse;

	vid_realmode = vid_modenum;

//	VID_SetPalette (palette);

//	vid_menudrawfn = VID_MenuDraw;
//	vid_menukeyfn = VID_MenuKey;

	strcpy (badmode.modedesc, "Bad mode");
	
//	_windowed_mouse.value=0;
}


void	VID_Shutdown (void)
{
	HDC				hdc;
	int				dummy;

	if (vid_initialized)
	{
//		if (modestate == MS_FULLDIB)
//			ChangeDisplaySettings (NULL, CDS_FULLSCREEN);

		PostMessage (HWND_BROADCAST, WM_PALETTECHANGED, (WPARAM)mainwindow, (LPARAM)0);
		PostMessage (HWND_BROADCAST, WM_SYSCOLORCHANGE, (WPARAM)0, (LPARAM)0);

//		AppActivate(qfalse, qfalse);
		DestroyDIBWindow ();
		DestroyFullscreenWindow ();
		DestroyFullDIBWindow ();

//		if (hwnd_dialog)
//			DestroyWindow (hwnd_dialog);

		if (mainwindow)
			DestroyWindow(mainwindow);

//		MGL_exit();

		vid_testingmode = 0;
		vid_initialized = 0;
	}
}


/*
================
FlipScreen
================
*/
void FlipScreen(vrect_t *rects)
{
	HRESULT		ddrval;
	byte *ics;
	u16 *ict16, *ics16;
	u32 *ict;
	int cr, cg, cb;
	int pix, pix2;
	int i, j, k;

#if 1
//	if(host_colormap16)
	if(1)
	{
//		r_pixbytes=2;
	
		ics16=vid_buffer;
		ict=fbufDib;
		for(i=0; i<DIBHeight; i++)
		{
			ict=fbufDib+(DIBHeight-i-1)*DIBWidth;

			for(j=0; j<DIBWidth; j++)
			{
				pix=*ics16++;
				cr=(pix>>10)&31; cr=(cr<<3)|(cr>>2);
				cg=(pix>> 5)&31; cg=(cg<<3)|(cg>>2);
				cb=(pix>> 0)&31; cb=(cb<<3)|(cb>>2);
				pix2=0xFF000000|(cr<<16)|(cg<<8)|cb;
				*ict++=pix2;
			}
		}
	}
#endif

	if(1)
	{
		HDC hdcScreen;

		hdcScreen = GetDC(mainwindow);

		BitBlt(hdcScreen,
			0,
			0,
			DIBWidth,
			DIBHeight,
			hdcDib,
			0, 0,
			SRCCOPY);

		GdiFlush();

		ReleaseDC(mainwindow, hdcScreen);
	}
}


void	VID_Update (vrect_t *rects)
{
	vrect_t	rect;
	RECT	trect;

	// We've drawn the frame; copy it to the screen
	FlipScreen (rects);
}


#if 0

//==========================================================================

byte        scantokey[128] = 
					{ 
//  0           1       2       3       4       5       6       7 
//  8           9       A       B       C       D       E       F 
	0  ,    27,     '1',    '2',    '3',    '4',    '5',    '6', 
	'7',    '8',    '9',    '0',    '-',    '=',    K_BACKSPACE, 9, // 0 
	'q',    'w',    'e',    'r',    't',    'y',    'u',    'i', 
	'o',    'p',    '[',    ']',    13 ,    K_CTRL,'a',  's',      // 1 
	'd',    'f',    'g',    'h',    'j',    'k',    'l',    ';', 
	'\'' ,    '`',    K_SHIFT,'\\',  'z',    'x',    'c',    'v',      // 2 
	'b',    'n',    'm',    ',',    '.',    '/',    K_SHIFT,'*', 
	K_ALT,' ',   0  ,    K_F1, K_F2, K_F3, K_F4, K_F5,   // 3 
	K_F6, K_F7, K_F8, K_F9, K_F10,  K_PAUSE,    0  , K_HOME, 
	K_UPARROW,K_PGUP,'-',K_LEFTARROW,'5',K_RIGHTARROW,'+',K_END, //4 
	K_DOWNARROW,K_PGDN,K_INS,K_DEL,0,0,             0,              K_F11, 
	K_F12,0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,        // 5
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0, 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,        // 6 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0, 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0         // 7 
}; 

/*
=======
MapKey

Map from windows to quake keynums
=======
*/
int MapKey (int key)
{
	key = (key>>16)&255;
	if (key > 127)
		return 0;

	return scantokey[key];
}

void AppActivate(BOOL fActive, BOOL minimize)
/****************************************************************************
*
* Function:     AppActivate
* Parameters:   fActive - True if app is activating
*
* Description:  If the application is activating, then swap the system
*               into SYSPAL_NOSTATIC mode so that our palettes will display
*               correctly.
*
****************************************************************************/
{
    HDC			hdc;
    int			i, t;
	static BOOL	sound_active;

	ActiveApp = fActive;

// messy, but it seems to work
	if (vid_fulldib_on_focus_mode)
	{
		Minimized = minimize;

		if (Minimized)
			ActiveApp = qfalse;
	}

//	MGL_appActivate(windc, ActiveApp);

	if (vid_initialized)
	{
	// yield the palette if we're losing the focus
		hdc = GetDC(NULL);

		if (GetDeviceCaps(hdc, RASTERCAPS) & RC_PALETTE)
		{
			if (ActiveApp)
			{
				if ((modestate == MS_WINDOWED) || (modestate == MS_FULLDIB))
				{
					if (GetSystemPaletteUse(hdc) == SYSPAL_STATIC)
					{
					// switch to SYSPAL_NOSTATIC and remap the colors
						SetSystemPaletteUse(hdc, SYSPAL_NOSTATIC);
						syscolchg = qtrue;
						pal_is_nostatic = qtrue;
					}
				}
			}
			else if (pal_is_nostatic)
			{
				if (GetSystemPaletteUse(hdc) == SYSPAL_NOSTATIC)
				{
				// switch back to SYSPAL_STATIC and the old mapping
					SetSystemPaletteUse(hdc, SYSPAL_STATIC);
					syscolchg = qtrue;
				}

				pal_is_nostatic = qfalse;
			}
		}

//		if (!Minimized)
//			VID_SetPalette (vid_curpal);

		scr_fullupdate = 0;

		ReleaseDC(NULL,hdc);
	}

// enable/disable sound on focus gain/loss
	if (!ActiveApp && sound_active)
	{
		S_BlockSound ();
		S_ClearBuffer ();
		sound_active = qfalse;
	}
	else if (ActiveApp && !sound_active)
	{
		S_UnblockSound ();
		S_ClearBuffer ();
		sound_active = qtrue;
	}

// minimize/restore fulldib windows/mouse-capture normal windows on demand
	if (!in_mode_set)
	{
		if (ActiveApp)
		{
			if (vid_fulldib_on_focus_mode)
			{
				if (vid_initialized)
				{
					msg_suppress_1 = qtrue;	// don't want to see normal mode set message
					VID_SetMode (vid_fulldib_on_focus_mode, vid_curpal);
					msg_suppress_1 = qfalse;

					t = in_mode_set;
					in_mode_set = qtrue;
					AppActivate (qtrue, qfalse);
					in_mode_set = t;
				}

				IN_ActivateMouse ();
				IN_HideMouse ();
			}
			else if ((modestate == MS_WINDOWED) && _windowed_mouse.value)
			{
				IN_ActivateMouse ();
				IN_HideMouse ();
			}
		}

		if (!ActiveApp)
		{
			if (modestate == MS_FULLDIB)
			{
				if (vid_initialized)
				{
					force_minimized = qtrue;
					i = vid_fulldib_on_focus_mode;
					msg_suppress_1 = qtrue;	// don't want to see normal mode set message
					VID_SetMode (windowed_default, vid_curpal);
					msg_suppress_1 = qfalse;
					vid_fulldib_on_focus_mode = i;
					force_minimized = qfalse;

				// we never seem to get WM_ACTIVATE inactive from this mode set, so we'll
				// do it manually
					t = in_mode_set;
					in_mode_set = qtrue;
					AppActivate (qfalse, qtrue);
					in_mode_set = t;
				}

				IN_DeactivateMouse ();
				IN_ShowMouse ();
			}
			else if ((modestate == MS_WINDOWED) && _windowed_mouse.value)
			{
				IN_DeactivateMouse ();
				IN_ShowMouse ();
			}
		}
	}
}
#endif


#if 0
/*
================
VID_HandlePause
================
*/
void VID_HandlePause (qboolean pause)
{

	if ((modestate == MS_WINDOWED) && _windowed_mouse.value)
	{
		if (pause)
		{
			IN_DeactivateMouse ();
			IN_ShowMouse ();
		}
		else
		{
			IN_ActivateMouse ();
			IN_HideMouse ();
		}
	}
}


/*
===================================================================

MAIN WINDOW

===================================================================
*/

// LONG CDAudio_MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/* main window procedure */
LONG WINAPI MainWndProc (
    HWND    hWnd,
    UINT    uMsg,
    WPARAM  wParam,
    LPARAM  lParam)
{
	LONG			lRet = 0;
	int				fwKeys, xPos, yPos, fActive, fMinimized, temp;
	HDC				hdc;
	PAINTSTRUCT		ps;
	static int		recursiveflag;

	switch (uMsg)
	{
		case WM_CREATE:
			break;

		case WM_SYSCOMMAND:

		// Check for maximize being hit
			switch (wParam & ~0x0F)
			{
				case SC_MAXIMIZE:
				// if minimized, bring up as a window before going fullscreen,
				// so MGL will have the right state to restore
					if (Minimized)
					{
						force_mode_set = qtrue;
						VID_SetMode (vid_modenum, vid_curpal);
						force_mode_set = qfalse;
					}

					VID_SetMode ((int)vid_fullscreen_mode.value, vid_curpal);
					break;

                case SC_SCREENSAVE:
                case SC_MONITORPOWER:
					if (modestate != MS_WINDOWED)
					{
					// don't call DefWindowProc() because we don't want to start
					// the screen saver fullscreen
						break;
					}

				// fall through windowed and allow the screen saver to start

				default:
					if (!in_mode_set)
					{
						S_BlockSound ();
						S_ClearBuffer ();
					}

					lRet = DefWindowProc (hWnd, uMsg, wParam, lParam);

					if (!in_mode_set)
					{
						S_UnblockSound ();
					}
			}
			break;

		case WM_MOVE:
			window_x = (int) LOWORD(lParam);
			window_y = (int) HIWORD(lParam);
			VID_UpdateWindowStatus ();

			if ((modestate == MS_WINDOWED) && !in_mode_set && !Minimized)
				VID_RememberWindowPos ();

			break;

		case WM_SIZE:
			Minimized = qfalse;
			
			if (!(wParam & SIZE_RESTORED))
			{
				if (wParam & SIZE_MINIMIZED)
					Minimized = qtrue;
			}
			break;

		case WM_SYSCHAR:
		// keep Alt-Space from happening
			break;

		case WM_ACTIVATE:
			fActive = LOWORD(wParam);
			fMinimized = (BOOL) HIWORD(wParam);
			AppActivate(!(fActive == WA_INACTIVE), fMinimized);

		// fix the leftover Alt from any Alt-Tab or the like that switched us away
			ClearAllStates ();

			if (!in_mode_set)
			{
//				if (windc)
//					MGL_activatePalette(windc,qtrue);

//				VID_SetPalette(vid_curpal);
			}

			break;

		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);

			if (!in_mode_set && host_initialized)
			{
//				SCR_UpdateWholeScreen ();
			}

			EndPaint(hWnd, &ps);
			break;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if (!in_mode_set)
				Key_Event (MapKey(lParam), qtrue);
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			if (!in_mode_set)
				Key_Event (MapKey(lParam), qfalse);
			break;

	// this is complicated because Win32 seems to pack multiple mouse events into
	// one update sometimes, so we always check all states and look for events
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEMOVE:
			if (!in_mode_set)
			{
				temp = 0;

				if (wParam & MK_LBUTTON)
					temp |= 1;

				if (wParam & MK_RBUTTON)
					temp |= 2;

				if (wParam & MK_MBUTTON)
					temp |= 4;

				IN_MouseEvent (temp);
			}
			break;

		// JACK: This is the mouse wheel with the Intellimouse
		// Its delta is either positive or neg, and we generate the proper
		// Event.
		case WM_MOUSEWHEEL: 
			if ((short) HIWORD(wParam) > 0) {
				Key_Event(K_MWHEELUP, qtrue);
				Key_Event(K_MWHEELUP, qfalse);
			} else {
				Key_Event(K_MWHEELDOWN, qtrue);
				Key_Event(K_MWHEELDOWN, qfalse);
			}
			break;
		// KJB: Added these new palette functions
		case WM_PALETTECHANGED:
			if ((HWND)wParam == hWnd)
				break;
			/* Fall through to WM_QUERYNEWPALETTE */
		case WM_QUERYNEWPALETTE:
			hdc = GetDC(NULL);

			if (GetDeviceCaps(hdc, RASTERCAPS) & RC_PALETTE)
				vid_palettized = qtrue;
			else
				vid_palettized = qfalse;

			ReleaseDC(NULL,hdc);

			scr_fullupdate = 0;

#if 0
			if (vid_initialized && !in_mode_set && windc && MGL_activatePalette(windc,qfalse) && !Minimized)
			{
//				VID_SetPalette (vid_curpal);
				InvalidateRect (mainwindow, NULL, qfalse);

			// specifically required if WM_QUERYNEWPALETTE realizes a new palette
				lRet = TRUE;
			}
#endif
			break;

		case WM_DISPLAYCHANGE:
			if (!in_mode_set && (modestate == MS_WINDOWED) && !vid_fulldib_on_focus_mode)
			{
				force_mode_set = qtrue;
				VID_SetMode (vid_modenum, vid_curpal);
				force_mode_set = qfalse;
			}
			break;

   	    case WM_CLOSE:
		// this causes Close in the right-click task bar menu not to work, but right
		// now bad things happen if Close is handled in that case (garbage and a
		// crash on Win95)
			if (!in_mode_set)
			{
				if (MessageBox (mainwindow, "Are you sure you want to quit?", "Confirm Exit",
							MB_YESNO | MB_SETFOREGROUND | MB_ICONQUESTION) == IDYES)
				{
					Sys_Quit ();
				}
			}
			break;

		case MM_MCINOTIFY:
//            lRet = CDAudio_MessageHandler (hWnd, uMsg, wParam, lParam);
			break;

		default:
            /* pass all unhandled messages to DefWindowProc */
            lRet = DefWindowProc (hWnd, uMsg, wParam, lParam);
	        break;
    }

    /* return 0 if handled message, 1 if not */
    return lRet;
}
#endif

#endif
