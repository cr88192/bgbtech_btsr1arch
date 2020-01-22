#ifdef _WIN32

#include <windows.h>
#include <commctrl.h>

#include <GL/GL.h>

#define __BOOLEAN__

#include "h2def.h"

// #include <bteifgl.h>
#include "gfxdrv_keys.h"


HWND	mainwindow;
HGLRC	mainrc;
HDC		maindc;
RECT	mainrect;

HINSTANCE gfxdrv_hInstance;
int gfxdrv_nCmdShow;
int	*gfxdrv_rkill;
int	gfxdrv_kill=0;

volatile int gfxdrv_waitdc=0;
volatile int gfxdrv_waitok=0;
volatile int gfxdrv_dt_swap=0;

void *gfxdrv_mutex=NULL;
int gfxdrv_dtt_swap;

int window_center_x;
int window_center_y;
int window_mouse_buttons;
int window_width;
int window_height;
int window_active;
int window_lastactive;
int window_fullscreen;

// int window_def_width=640;
// int window_def_height=480;
//int window_def_width=1024;
//int window_def_height=768;

//int window_def_width=1280;
//int window_def_height=960;

int window_def_width=960;
int window_def_height=720;

char *window_def_label="BTEIFGL";
int window_def_fullscreen=0;
int window_fullscreen_width=0;
int window_fullscreen_height=0;
int window_desktop_width=1680;
int window_desktop_height=1050;
int window_max_width=0;
int window_max_height=0;

char *gl_vendor, *gl_renderer, *gl_version, *gl_extensions;

void GfxDrv_V_HandleMessages();
void FRGL_EndInputFrame();

void gfxdrv_lock()
{
//	thLockFastMutex(gfxdrv_mutex);
}

void gfxdrv_unlock()
{
//	thUnlockFastMutex(gfxdrv_mutex);
}

void gfxdrv_sleep(int ms)
{
}

static int gfxdrv_log2up(int v)
{
	int i, j;
	
	i=v; j=0;
	while(i>1) { i=(i+1)>>1; j++; }
	return(j);
}

void GfxDrv_GetWindowSize(int *xs, int *ys)
	{ *xs=window_width; *ys=window_height; }

void GfxDrv_GetWindowTexSize(int *xs, int *ys)
{
	*xs=1<<gfxdrv_log2up(window_width);
	*ys=1<<gfxdrv_log2up(window_height);
}

void GfxDrv_GetWindowMaxSize(int *xs, int *ys)
	{ *xs=window_max_width; *ys=window_max_height; }

void GfxDrv_GetWindowMaxTexSize(int *xs, int *ys)
{
	*xs=1<<gfxdrv_log2up(window_max_width);
	*ys=1<<gfxdrv_log2up(window_max_height);
}

int GfxDrv_WindowIsActiveP(void)
	{ return(window_active); }
int GfxDrv_WindowIsFullActiveP(void)
	{ return(window_active && window_lastactive); }
int GfxDrv_WindowIsFullscreenP(void)
	{ return(window_fullscreen); }

void GfxDrv_BeginDrawing()
{
	int t0, t1;
	gfxdrv_dtt_swap=0;
	
	t0=clock();
	GfxDrv_V_HandleMessages();
	glViewport(0, 0, window_width, window_height);
	glClearColor(0, 0, 0, 1);
//	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	t1=clock();
	gfxdrv_dtt_swap+=t1-t0;
}

void GfxDrv_KeyEvent(int key, int down)
{
	GfxDev_Key_Event(key, down);
}

byte *btesh2_gfxcon_framebuf;
int btesh2_gfxcon_fb_dirty;
int btesh2_gfxcon_fbxs;
int btesh2_gfxcon_fbys;

void GfxDrv_EndDrawing(void)
{
	int w1, h1;
	int t0, t1;

	t0=clock();

//	if(btesh2_gfxcon_framebuf && btesh2_gfxcon_fb_dirty)
//	if(btesh2_gfxcon_framebuf)
	if(btesh2_gfxcon_framebuf && (btesh2_gfxcon_fb_dirty>0))
	{
		glViewport(0, 0, window_width, window_height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

//		w1=window_width/2;
//		h1=window_height/2;

		w1=btesh2_gfxcon_fbxs/2;
		h1=btesh2_gfxcon_fbys/2;

		glOrtho(-w1, w1, -h1, h1, -999999999, 999999999);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

//		glRasterPos2i(0, 0);

//		glWindowPos2i(0, 0);
		glRasterPos2i(-btesh2_gfxcon_fbxs/2, btesh2_gfxcon_fbys/2);
//		glRasterPos2i(-btesh2_gfxcon_fbxs/2, 0);
//		glWindowPos2i(0, btesh2_gfxcon_fbys);
//		glPixelZoom(1, -1);

		glPixelZoom((float)window_width/btesh2_gfxcon_fbxs,
			-(float)window_height/btesh2_gfxcon_fbys);

		glDrawPixels(btesh2_gfxcon_fbxs, btesh2_gfxcon_fbys,
			GL_RGBA, GL_UNSIGNED_BYTE, btesh2_gfxcon_framebuf);
		btesh2_gfxcon_fb_dirty--;
	}

//	glFinish();
	SwapBuffers(maindc);
	
	FRGL_EndInputFrame();

	window_lastactive=window_active;

	if(gfxdrv_waitdc)
	{
		gfxdrv_lock();
		wglMakeCurrent(NULL, NULL);
		gfxdrv_waitok=1;
		gfxdrv_unlock();

		while(gfxdrv_waitdc>0)
			gfxdrv_sleep(10);
		gfxdrv_waitok=0;
		gfxdrv_sleep(10);

		gfxdrv_lock();
		wglMakeCurrent(maindc, mainrc);
		gfxdrv_unlock();
	}

	t1=clock();
	gfxdrv_dtt_swap+=t1-t0;
	
	gfxdrv_dt_swap=gfxdrv_dtt_swap;
}

void GfxDrv_V_HandleMessages()
{
	MSG	msg;

	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void GfxDrv_V_CenterWindow(HWND hWnd,
	int width, int height) //AH:ignore
{
	RECT rect;
	int cx, cy;

	cx=GetSystemMetrics(SM_CXSCREEN);
	cy=GetSystemMetrics(SM_CYSCREEN);
	cx=(cx-width)/2; cy=(cy-height)/2;
	if(cx>cy*2)cx/=2;
	if(cx<0)cx=0;
	if(cy<0)cy=0;
	SetWindowPos(hWnd, NULL, cx, cy, 0, 0,
		SWP_NOSIZE|SWP_NOZORDER|SWP_SHOWWINDOW|SWP_DRAWFRAME);
//	SetWindowPos(hWnd, NULL, cx, cy, width, height,
//		SWP_NOZORDER|SWP_SHOWWINDOW|SWP_DRAWFRAME);
	window_center_x=cx+(width/2);
	window_center_y=cy+(height/2)+24;
	window_width=width;
	window_height=height;
}

int GfxDrv_V_SetWindowedMode()
{
	static DWORD ws=
		WS_OVERLAPPED|WS_BORDER|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX;
	RECT rect;

	ChangeDisplaySettings (NULL, 0);
	window_fullscreen=0;

	SetWindowLong(mainwindow, GWL_STYLE, ws);
	SetWindowPos(mainwindow, NULL, 0, 0, 0, 0,
		SWP_NOSIZE|SWP_NOZORDER|SWP_SHOWWINDOW|
		SWP_DRAWFRAME|SWP_FRAMECHANGED);

	rect.top=rect.left=0;
	rect.right=window_def_width;
	rect.bottom=window_def_height;

	AdjustWindowRectEx(&rect, ws, FALSE, 0);

	window_width=rect.right-rect.left;
	window_height=rect.bottom-rect.top;

	MoveWindow(mainwindow, 0, 0, window_width, window_height, TRUE );
		
	GfxDrv_V_CenterWindow(mainwindow, window_width, window_height);
	return(0);
}

int GfxDrv_V_GetDesktopSize()
{
	DEVMODE devmode;

	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);
	window_desktop_width=devmode.dmPelsWidth;
	window_desktop_height=devmode.dmPelsHeight;
	
	if(window_fullscreen_width<=0)
	{
		window_fullscreen_width=window_desktop_width;
		window_fullscreen_height=window_desktop_height;
	}
	
	window_max_width=window_def_width;
	window_max_height=window_def_height;
	if(window_fullscreen_width>window_max_width)
		{ window_max_width=window_fullscreen_width; }
	if(window_fullscreen_height>window_max_height)
		{ window_max_height=window_fullscreen_height; }
	return(0);
}

int GfxDrv_V_SetFullscreenMode()
{
	DEVMODE devmode;
	RECT rect;
	DWORD ws;
	int width, height;
	int i;

//	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);
//	width=devmode.dmPelsWidth;
//	height=devmode.dmPelsHeight;

//	width=window_def_width;
//	height=window_def_height;
	width=window_fullscreen_width;
	height=window_fullscreen_height;

	devmode.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
	devmode.dmBitsPerPel=32;
	devmode.dmPelsWidth=width;
	devmode.dmPelsHeight=height;
	devmode.dmSize=sizeof(devmode);

	i=ChangeDisplaySettingsEx(NULL, &devmode, NULL, CDS_FULLSCREEN, NULL);

//	if(ChangeDisplaySettings(&devmode, CDS_FULLSCREEN)!= 
//		DISP_CHANGE_SUCCESSFUL)
	if(i!=DISP_CHANGE_SUCCESSFUL)
	{
		printf("GfxDrv_V_SetFullscreenMode: Failed To Set Fullscreen\n");
		window_fullscreen=0;
		return(-1);
	}

	rect.top=rect.left=0;
	rect.right=width;
	rect.bottom=height;

	ws=WS_POPUP;
	AdjustWindowRectEx(&rect, ws, FALSE, 0);

	SetWindowLong(mainwindow, GWL_STYLE, ws);
	SetWindowPos(mainwindow, NULL, 0, 0, 0, 0,
		SWP_NOSIZE|SWP_NOZORDER|SWP_SHOWWINDOW|SWP_FRAMECHANGED);

	window_width=rect.right-rect.left;
	window_height=rect.bottom-rect.top;

	MoveWindow(mainwindow, 0, 0, window_width, window_height, TRUE );

	window_center_x=window_width/2;
	window_center_y=window_height/2;

	return(1);
}

void GfxDrv_UpdateMode()
{
#if 0
	gfxdrv_lock();
	wglMakeCurrent(NULL, NULL);
//	gfxdrv_waitok=1;
	gfxdrv_unlock();

	if(maindc && mainwindow)
	{
		ReleaseDC(mainwindow, maindc);
		maindc=NULL;
	}
#endif

	if(window_fullscreen)
	{
		GfxDrv_V_SetFullscreenMode();
	}else
	{
		GfxDrv_V_SetWindowedMode();
	}

#if 0
	GfxDrv_SetupMainDC();

	gfxdrv_lock();
	wglMakeCurrent(maindc, mainrc);
	gfxdrv_unlock();
#endif
}

int GfxDrv_V_SetupWindow()
{
//	static DWORD ws=
//		WS_OVERLAPPED|WS_BORDER|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX;
	static DWORD ws=WS_POPUP;

	HDC	hdc;
	RECT rect;
	int	lastmodestate, width, height;

	GfxDrv_V_GetDesktopSize();

	mainrect.top=mainrect.left=0;
//	mainrect.right=window_def_width;
//	mainrect.bottom=window_def_height;
//	mainrect.right=window_desktop_width;
//	mainrect.bottom=window_desktop_height;
	mainrect.right=window_max_width;
	mainrect.bottom=window_max_height;

	rect=mainrect;
	AdjustWindowRectEx(&rect, ws, FALSE, 0);

	width=rect.right-rect.left;
	height=rect.bottom-rect.top;

	mainwindow=CreateWindowEx(
		0,
		"bgbui",
		window_def_label,
		ws,
		rect.left, rect.top,
		width, height,
		NULL, NULL,
		gfxdrv_hInstance,
		NULL);

	GfxDrv_V_CenterWindow(mainwindow,
		mainrect.right-mainrect.left,
		mainrect.bottom-mainrect.top);

	ShowWindow(mainwindow, SW_SHOWDEFAULT);
	UpdateWindow(mainwindow);

//	hdc=GetDC(mainwindow);
//	PatBlt(hdc, 0, 0, mainrect.right, mainrect.bottom, BLACKNESS);
//	ReleaseDC(mainwindow, hdc);

	GfxDrv_V_HandleMessages();
	Sleep(100);
	SetWindowPos(mainwindow, HWND_TOP, 0, 0, 0, 0,
				  SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW |
				  SWP_NOCOPYBITS);
	SetForegroundWindow(mainwindow);
	return(-1);
}

void GfxDrv_InitGL()
{
	gl_vendor=(char *)glGetString(GL_VENDOR);
	gl_renderer=(char *)glGetString(GL_RENDERER);
	gl_version=(char *)glGetString(GL_VERSION);
	gl_extensions=(char *)glGetString(GL_EXTENSIONS);

	printf("OpenGL Vendor=%s\n", gl_vendor);
	printf("OpenGL Renderer=%s\n", gl_renderer);
	printf("OpenGL Version=%s\n", gl_version);
//	printf("OpenGL Extensions=%s\n", gl_extensions);

//	BGBBTJ_SetGlExtensions(gl_extensions);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_SMOOTH);

	glClearColor(0, 0, 0, 1);
	glCullFace(GL_FRONT);
	glEnable(GL_TEXTURE_2D);

	glAlphaFunc(GL_GREATER, 0.75);
	glDisable(GL_ALPHA_TEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);
}

void GfxDrv_InitGL2()
{
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_SMOOTH);

	glClearColor(0, 0, 0, 1);
	glCullFace(GL_FRONT);
	glEnable(GL_TEXTURE_2D);

	glAlphaFunc(GL_GREATER, 0.75);
	glDisable(GL_ALPHA_TEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GfxDrv_Shutdown()
{
   	HGLRC hrc;
   	HDC	  hdc;

	if(window_fullscreen)
		{ ChangeDisplaySettings (NULL, 0); }

	hrc=wglGetCurrentContext();
	hdc=wglGetCurrentDC();
	wglMakeCurrent(NULL, NULL);
	if(hrc)
		wglDeleteContext(hrc);
	if(hdc && mainwindow)
		ReleaseDC(mainwindow, hdc);
	if(maindc && mainwindow)
		ReleaseDC(mainwindow, maindc);
}

static int scantokey[128]=
{
	0, K_ESC, '1','2','3','4','5','6', 
	'7','8','9','0','-','=', K_BACKSPACE, 9,
	'q','w','e','r','t','y','u','i', 
	'o','p','[',']',13 , K_CTRL, 'a','s',
	'd','f','g','h','j','k','l',';', 
	'\'', '`', K_SHIFT,'\\', 'z','x','c','v',
	'b','n','m',',','.',K_NUMPAD_DIV, K_SHIFT,'*', 
	K_ALT, ' ', K_CAPSLOCK, K_F1, K_F2, K_F3, K_F4, K_F5,
	K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE, K_SCROLL,  K_HOME, 
	K_UPARROW, K_PGUP, '-', K_LEFTARROW,'5', K_RIGHTARROW, '+', K_END,
	K_DOWNARROW, K_PGDN, K_INS, K_DEL, 0, 0, 0,
	K_F11, 
	K_F12, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0
}; 

static int scantokeyb[128]=
{
	0, K_ESC, '1','2','3','4','5','6', 
	'7','8','9','0','-','=', K_BACKSPACE, 9,
	'q','w','e','r','t','y','u','i', 
	'o','p','[',']',13 , K_CTRL, 'a','s',
	'd','f','g','h','j','k','l',';', 
	'\'', '`', K_SHIFT,'\\', 'z','x','c','v',
	'b','n','m',',','.','/', K_SHIFT, K_NUMPAD_MUL, 
	K_ALT, ' ', 0, K_F1, K_F2, K_F3, K_F4, K_F5,
	K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE, 0,  K_NUMPAD7, 
	K_NUMPAD8, K_NUMPAD9, K_NUMPAD_SUB, K_NUMPAD4, K_NUMPAD5, K_NUMPAD6,
	K_NUMPAD_ADD, K_NUMPAD1,
	K_NUMPAD2, K_NUMPAD3, K_NUMPAD0, K_DEL, 0, 0, 0,
	K_F11, 
	K_F12, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0
}; 

int GfxDrv_MapKey(int key)
{
	int al, val;

//	frgl_printf("Key: 0x%08X\n", key);

	al=(key>>24)&1;
	key=(key>>16)&255;
	if(key>127)
		return 0;
//	if(!scantokey[key])
//		frgl_printf("no mapping for key 0x%02X\n", key);

	val=scantokey[key];
	if(!al)
		val=scantokeyb[key];

//	frgl_printf("Map Key: %d\n", val);

	return(val);
}

void GfxDrv_AppActivate(int active, int minimized)
{
	window_active=active&&(!minimized);

	if(window_fullscreen)
	{
		if(window_active)
		{
			GfxDrv_V_SetFullscreenMode();
		}else
		{
			ChangeDisplaySettings(NULL, 0);
		}
	}
}

LONG WINAPI GfxDrv_MainWndProc(
	HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) //AH:ignore
{
	LONG ret;
	int xp, yp;
	int i, j;

	ret=1;
	switch(uMsg)
	{
		case WM_CREATE:
			break;
		case WM_SYSCHAR:
			break;
		case WM_ACTIVATE:
			i = LOWORD(wParam);
			j = (BOOL) HIWORD(wParam);

			GfxDrv_AppActivate(i!=WA_INACTIVE, j);
			break;

		case WM_MOVE:
			if(!window_fullscreen)
			{
				xp=(int) LOWORD(lParam);
				yp=(int) HIWORD(lParam);
				window_center_x=xp+(window_width/2);
				window_center_y=yp+(window_height/2);
			}
			break;

		case WM_SYSKEYDOWN:
			if(wParam==0xFF)break;
			if(wParam==0x0D)
			{
				window_fullscreen=!window_fullscreen;
				GfxDrv_UpdateMode();
			}

			if(wParam==0x3E)
			{
				if(gfxdrv_rkill)*gfxdrv_rkill=1;
				gfxdrv_kill=1;
			}

			GfxDrv_KeyEvent(GfxDrv_MapKey(lParam),-1);
			break;

		case WM_KEYDOWN:
//		case WM_SYSKEYDOWN:
			if(wParam==0xFF)break;
			GfxDrv_KeyEvent(GfxDrv_MapKey(lParam),-1);
			break;
			
		case WM_KEYUP:
		case WM_SYSKEYUP:
			if(wParam==0xFF)break;
			GfxDrv_KeyEvent(GfxDrv_MapKey(lParam), 0);
			break;

		case WM_LBUTTONDOWN:	case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:	case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:	case WM_MBUTTONUP:
		case WM_MOUSEMOVE:
			i=0;
			if(wParam&MK_LBUTTON)i|=1;
			if(wParam&MK_RBUTTON)i|=4;
			if(wParam&MK_MBUTTON)i|=2;
			j=window_mouse_buttons^i;
			window_mouse_buttons=i;
			if(j&1)GfxDrv_KeyEvent(K_MOUSE1, (i&1)?(-1):0);
			if(j&2)GfxDrv_KeyEvent(K_MOUSE2, (i&2)?(-1):0);
			if(j&4)GfxDrv_KeyEvent(K_MOUSE3, (i&4)?(-1):0);
//			if(FRGL_CvarGetInt("hide_os_cursor"))
//				SetCursor(NULL);
			break;

		case WM_MOUSEWHEEL: 
			if(((short)HIWORD(wParam))>0)
			{
				GfxDrv_KeyEvent(K_MWHEELUP, -1);
				GfxDrv_KeyEvent(K_MWHEELUP, 0);
			}else
			{
				GfxDrv_KeyEvent(K_MWHEELDOWN, -1);
				GfxDrv_KeyEvent(K_MWHEELDOWN, 0);
			}
			break;

		case WM_SIZE:
			xp=(int)LOWORD(lParam);
			yp=(int)HIWORD(lParam);
			window_width=xp;
			window_height=yp;
			break;

		case WM_CLOSE:
			if(gfxdrv_rkill)*gfxdrv_rkill=1;
			gfxdrv_kill=1;
			break;

		case WM_DESTROY:
			if(mainwindow)
				DestroyWindow(mainwindow);
			PostQuitMessage(0);
			break;
		default:
			ret=DefWindowProc(hWnd, uMsg, wParam, lParam);
			break;
	}
	return(ret);
}

void GfxDrv_SetDefaults(char *label, int width, int height)
{
	if(label)window_def_label=label;
	window_def_width=width;
	window_def_height=height;
}

void GfxDrv_SetDefaultFullscreen(int fs)
{
	window_def_fullscreen=fs;
}

void GfxDrv_SetFullscreenResolution(int width, int height)
{
	window_fullscreen_width=width;
	window_fullscreen_height=height;
}

int GfxDrv_SetupMainDC()
{
	PIXELFORMATDESCRIPTOR pfd;
	int i;

#if 1
	maindc=GetDC(mainwindow);

	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize=sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion=1;
	pfd.dwFlags=PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
	pfd.iPixelType=PFD_TYPE_RGBA;
	pfd.cColorBits=24;
	pfd.cDepthBits=32;
	pfd.cStencilBits=16;
	pfd.iLayerType=PFD_MAIN_PLANE;

	i=ChoosePixelFormat(maindc, &pfd);
	if(!i)return(-1);
	i=SetPixelFormat(maindc, i, &pfd);
	if(!i)return(-1);
#endif
	return(0);
}

int GfxDrv_Start()
{
//	PIXELFORMATDESCRIPTOR pfd;
	WNDCLASS wc;
	HDC hdc;
	int i;

//	FRGL_CvarSetDefault("hide_os_cursor", "0");

//	MessageBox(NULL, "GL Init failed", "Error", MB_OK);

//	gfxdrv_mutex=thFastMutex();

	if(!gfxdrv_hInstance)
		gfxdrv_hInstance=GetModuleHandle(NULL);

	memset(&wc, 0, sizeof(WNDCLASS));
	wc.lpfnWndProc=(WNDPROC)GfxDrv_MainWndProc;
	wc.hInstance=gfxdrv_hInstance;
	wc.lpszClassName="bgbui";

	i=RegisterClass(&wc);
	if(!i)return(-1);

	hdc=GetDC(NULL);
	if(GetDeviceCaps(hdc, RASTERCAPS) & RC_PALETTE)
		return(-1);
	ReleaseDC(NULL, hdc);

	GfxDrv_V_SetupWindow();
	if(window_def_fullscreen)
	{
		window_fullscreen=1;
		GfxDrv_V_SetFullscreenMode();
	}

	GfxDrv_SetupMainDC();

	mainrc=wglCreateContext(maindc);
	if(!mainrc)return(-1);

	i=wglMakeCurrent(maindc, mainrc);
	if(!i)return(-1);

//	if(FRGL_CvarGetInt("hide_os_cursor"))
//		SetCursor(NULL);
	GfxDrv_InitGL();

	if(!window_def_fullscreen)
	{
		GfxDrv_V_SetWindowedMode();
	}

	
//	btesh2_gfxcon_fbxs=window_width;
//	btesh2_gfxcon_fbys=window_height;
//	btesh2_gfxcon_fbsz=btesh2_gfxcon_fbxs*btesh2_gfxcon_fbys;

	return(0);
}

int GfxDrv_SetupThreadShareLists()
{
	HGLRC	altrc;
	int i;

//	*(int *)-1=-1;

	gfxdrv_waitdc++;
	while(!gfxdrv_waitok)
		gfxdrv_sleep(10);

	gfxdrv_lock();

	altrc=wglCreateContext(maindc);
	if(!altrc)
	{
		gfxdrv_unlock();
		return(-1);
	}

	i=wglShareLists(mainrc, altrc);
	if(!i)
	{
		gfxdrv_unlock();
		return(-1);
	}

	i=wglMakeCurrent(maindc, altrc);
	if(!i)
	{
		gfxdrv_unlock();
		return(-1);
	}
	
	GfxDrv_InitGL2();
	
	gfxdrv_waitdc--;
	gfxdrv_unlock();

	return(0);
}

int GfxDrv_TeardownThreadShareLists()
{
   	HGLRC hrc;

	hrc=wglGetCurrentContext();
	wglMakeCurrent(NULL, NULL);
	if(hrc)
		wglDeleteContext(hrc);
	return(0);
}

int GfxDrv_Init(void *hInstance, int nCmdShow, int *rkill)
{
	gfxdrv_hInstance=hInstance;
	gfxdrv_nCmdShow=nCmdShow;
	gfxdrv_rkill=rkill;
	return(0);
}

int GfxDrv_SetInstance(void *hInstance, int nCmdShow)
{
	gfxdrv_hInstance=hInstance;
	gfxdrv_nCmdShow=nCmdShow;
//	gfxdrv_rkill=rkill;
	return(0);
}

int GfxDrv_SetKill(int *rkill)
{
	gfxdrv_rkill=rkill;
	return(0);
}

int GfxDrv_GetDtSwap()
{
	return(gfxdrv_dt_swap);
}

int GfxDrv_PrepareFramebuf()
{
	btesh2_gfxcon_fb_dirty=3;
	if(!btesh2_gfxcon_framebuf)
	{
		btesh2_gfxcon_framebuf=malloc(window_width*window_height*4);
	}
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
			Sleep(1);
			continue;
		}
		
		fcn();
		lt=ct;
	}
	return(0);
}

#endif

