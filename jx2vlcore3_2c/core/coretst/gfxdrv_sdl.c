#ifdef __linux
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// #if defined(linux) || defined(__EMSCRIPTEN__)
#if defined(__EMSCRIPTEN__)
#define USE_SDL
#endif

// #if defined(linux) || defined(__EMSCRIPTEN__)
#ifdef USE_SDL
#include <SDL/SDL.h>
#include <SDL/SDL_mouse.h>
#include <SDL/SDL_keyboard.h>
#endif

#if defined(linux) || defined(__EMSCRIPTEN__)
#include <sys/time.h>
#endif


#ifdef __linux
static struct termios old_termios;

void btesh2_ttynoncanon(void)
{
	struct termios new_termios;

	tcgetattr(0, &old_termios);
	new_termios=old_termios;
	new_termios.c_lflag&=(~ICANON);
//	new_termios.c_lflag&=(~ECHO);
	new_termios.c_cc[VTIME]=0;
	new_termios.c_cc[VMIN]=1;
	tcsetattr(0, TCSANOW, &new_termios);
	return;
}

void btesh2_resettermios(void)
{
	tcsetattr(0, TCSANOW, &old_termios);
	return;
}
#endif


// #if defined(__EMSCRIPTEN__) || defined(linux)
#ifdef USE_SDL

SDL_Surface *btesh_main_screen;
int gfxdrv_kill;

#if 0
static int scantokey[256]=
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

static int scantokeyb[256]=
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
#endif

#if 1
static int scantokey[256]=
{
  0,    0,    0,    0,  'a',  'b',  'c',  'd', //  0-  7
'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l', //  8- 15
'm',  'n',  'o',  'p',  'q',  'r',  's',  't', // 16- 23
'u',  'v',  'w',  'x',  'y',  'z',  '1',  '2', // 24- 31
'3',  '4',  '5',  '6',  '7',  '8',  '9',  '0', // 32- 39
 13,   27,    8,    9,  ' ',  '-',  '=',  '[', // 40- 47
']', '\\', '\\',  ';',  '`', '\'',  ',',  '.', // 48- 55
'/',  160,  135,  136,  137,  138,  139,  140, // 56- 63
141,  142,  143,  144,  145,  146,    0,  161, // 64- 71
153,  147,  151,  150,  127,  152,  149,  131, // 72- 79
130,  129,  128,    0,  207,  206,  205,  204, // 80- 87
218,  209,  210,  211,  212,  213,  214,  215, // 88- 95
216,  217,  208,  219,  '`',    0,    0,  '=', // 96-103
  0,    0,    0,    0,    0,    0,    0,    0, //104-111
  0,    0,    0,    0,    0,    0,    0,    0, //112-119
  0,    0,    0,    0,    0,    0,    0,    0, //120-127
  0,    0,    0,    0,    0,    0,    0,    0, //128-135
  0,    0,    0,    0,    0,    0,    0,    0, //136-143
  0,    0,    0,    0,    0,    0,    0,    0, //144-151
  0,    0,    0,    0,    0,    0,    0,    0, //152-159
  0,    0,    0,    0,    0,    0,    0,    0, //160-167
  0,    0,    0,    0,    0,    0,    0,    0, //168-175
  0,    0,    0,    0,    0,    0,    0,    0, //176-183
  0,    0,  ';',  '=',    0,  '-',    0,    0, //184-191
  0,    0,    0,    0,    0,    0,    0,    0, //192-199
  0,    0,    0,    0,    0,    0,    0,    0, //200-207
  0,    0,    0,    0,    0,    0,    0,    0, //208-215
  0,    0,    0,    0,    0,    0,    0,    0, //216-223
133,  134,  132,    0,  133,  134,  132,    0, //224-231
};
#endif

#if 0
int GfxDrv_MapKey(int key)
{
	int al, val;

//	frgl_printf("Key: 0x%08X\n", key);

	if(key>255)
		return(0);
	val=scantokey[key];
	return(val);
}
#endif

#if 1
int GfxDrv_MapKey(int key)
{
	int k1;
	
	k1=0;
	switch(key)
	{
	case SDLK_UP:			k1=K_UPARROW;		break;
	case SDLK_DOWN:			k1=K_DOWNARROW;		break;
	case SDLK_LEFT:			k1=K_LEFTARROW;		break;
	case SDLK_RIGHT:		k1=K_RIGHTARROW;	break;

	case SDLK_INSERT:		k1=K_INS;			break;
	case SDLK_HOME:			k1=K_HOME;			break;
	case SDLK_END:			k1=K_END;			break;
	case SDLK_PAGEUP:		k1=K_PGUP;			break;
	case SDLK_PAGEDOWN:		k1=K_PGDN;			break;
	case SDLK_DELETE:		k1=K_DEL;			break;

	case SDLK_F1:			k1=K_F1;			break;
	case SDLK_F2:			k1=K_F2;			break;
	case SDLK_F3:			k1=K_F3;			break;
	case SDLK_F4:			k1=K_F4;			break;
	case SDLK_F5:			k1=K_F5;			break;
	case SDLK_F6:			k1=K_F6;			break;
	case SDLK_F7:			k1=K_F7;			break;
	case SDLK_F8:			k1=K_F8;			break;
	case SDLK_F9:			k1=K_F9;			break;
	case SDLK_F10:			k1=K_F10;			break;
	case SDLK_F11:			k1=K_F11;			break;
	case SDLK_F12:			k1=K_F12;			break;

	case SDLK_LCTRL:		k1=K_CTRL;			break;
	case SDLK_RCTRL:		k1=K_CTRL;			break;

	case SDLK_LSHIFT:		k1=K_SHIFT;			break;
	case SDLK_RSHIFT:		k1=K_SHIFT;			break;
	case SDLK_LALT:			k1=K_ALT;			break;
	case SDLK_RALT:			k1=K_ALT;			break;


	default:
		if(key<128)
			k1=key;
		break;

//	case SDLK_F1:			k1=K_F1;			break;
//	case SDLK_F1:			k1=K_F1;			break;
	}
	return(k1);
}
#endif

void GfxDrv_UpdateKeyboard()
{
	byte *keys;
	int i;

#if 0
	keys=SDL_GetKeyboardState(NULL);
	if(!keys)
		return;
	
//	for(i=0; i<256; i++)
//	for(i=0; i<1024; i++)
	for(i=0; i<2048; i++)
	{
		if(keys[i] && !gfxdrv_lastkeys[i])
		{
			printf("keydown %02X\n", i);
//			GfxDev_Key_Event(i, 1);
			gfxdrv_lastkeys[i]=1;
		}else if(!keys[i] && gfxdrv_lastkeys[i])
		{
			printf("keyup %02X\n", i);
//			GfxDev_Key_Event(i, 0);
			gfxdrv_lastkeys[i]=0;
		}
	}
#endif
}

void GfxDrv_UpdateEvents()
{
	SDL_Event ev;
	int i, j;

	while(SDL_PollEvent(&ev))
	{
		switch(ev.type)
		{
		case SDL_KEYDOWN:
//			i=ev.key.keysym.scancode;
			i=ev.key.keysym.sym;
			j=GfxDrv_MapKey(i);
			if(!j)
			{
				printf("EV Key Dn %02X %02X\n", i, j);
				break;
			}
			GfxDev_Key_Event(j, 1);
			break;
		case SDL_KEYUP:
//			i=ev.key.keysym.scancode;
			i=ev.key.keysym.sym;
			j=GfxDrv_MapKey(i);
			if(!j)
			{
				printf("EV Key Up %02X %02X\n", i, j);
				break;
			}
			GfxDev_Key_Event(j, 0);
			break;
			
//		case SDL_WINDOWEVENT_CLOSE:
		case SDL_QUIT:
//			ctx->req_kill=1;
			gfxdrv_kill=1;
			break;

		default:
			break;
		}
	}
}

int gfxdrv_locksurf=0;

int GfxDrv_PrepareFramebuf()
{	
	if(gfxdrv_locksurf)
		return(0);

	if(SDL_MUSTLOCK(btesh_main_screen))
	{
		SDL_LockSurface(btesh_main_screen);
		gfxdrv_locksurf=1;
	}

	btesh2_gfxcon_framebuf=(byte *)btesh_main_screen->pixels;
//	btesh2_gfxcon_framebuf=(byte *)malloc(960*720*4);
	gfxdrv_locksurf=1;
	return(1);
}

void GfxDrv_BeginDrawing()
{
	if(SDL_MUSTLOCK(btesh_main_screen) && gfxdrv_locksurf)
	{
		SDL_UnlockSurface(btesh_main_screen);
		gfxdrv_locksurf=0;
		btesh2_gfxcon_framebuf=NULL;
	}
	SDL_Flip(btesh_main_screen);

	SDL_PumpEvents();
	GfxDrv_UpdateKeyboard();
	GfxDrv_UpdateEvents();

#if 0
	if(SDL_MUSTLOCK(btesh_main_screen))
	{
		SDL_LockSurface(btesh_main_screen);
		gfxdrv_locksurf=1;
	}

	btesh2_gfxcon_framebuf=(u32 *)btesh_main_screen->pixels;
#endif

//	BTESH2_GfxCon_BlinkState((FRGL_TimeMS()>>9)&1);
//	BTESH2_GfxCon_Redraw();
//	BTESH2_DCGFX_RedrawScreen();
}

void GfxDrv_EndDrawing()
{
//	memcpy(btesh_main_screen->pixels, btesh2_gfxcon_framebuf,
//		640*400*sizeof(u32));
}

void GfxDrv_Start()
{
	int xs, ys;
	
	xs=btesh2_gfxcon_fbxs;
	ys=btesh2_gfxcon_fbys;
	
	if(!xs || !ys)
	{
		xs=640;
		ys=400;
	}
	
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	btesh_main_screen =
		SDL_SetVideoMode(xs, ys, 32, SDL_SWSURFACE);
//		SDL_SetVideoMode(640, 400, 32, SDL_SWSURFACE);
//		SDL_SetVideoMode(640, 480, 32, SDL_OPENGL);
//	btesh2_gfxcon_fbxs=640;
//	btesh2_gfxcon_fbys=400;
	btesh2_gfxcon_fbxs=xs;
	btesh2_gfxcon_fbys=ys;

	btesh2_gfxcon_swaprb=1;
//	btesh2_gfxcon_fbsz=btesh2_gfxcon_fbxs*btesh2_gfxcon_fbys;
}
#endif

#ifdef linux
void SoundDev_Submit()
{
//	return(0);
}

int SoundDev_WriteStereoSamples(short *buffer, int cnt)
{
	return(0);
}

int SoundDev_Init()
{
	return(0);
}

int SoundDev_DeInit()
{
	return(0);
}
#endif

// #ifdef linux
// #if 0
#if defined(linux) && !defined(USE_SDL)
int	gfxdrv_kill=0;

int GfxDrv_Start()
{
	return(0);
}

int GfxDrv_PrepareFramebuf()
{
	return(0);
}

void GfxDrv_BeginDrawing()
{
}

void GfxDrv_EndDrawing(void)
{
}
#endif

#ifdef linux
int GfxDrv_MainLoop(void (*fcn)())
{
	static int lt=0;
	int ct, dt;
	
	ct=FRGL_TimeMS();
	while(!gfxdrv_kill)
	{
		ct=FRGL_TimeMS();
		dt=ct-lt;
//		if((dt>=0) && (dt<10))
		if((dt>=0) && (dt<5))
		{
//			Sleep(1);
			usleep(0);
			continue;
		}
		
		fcn();
		lt=ct;
	}
	return(0);
}

#endif
