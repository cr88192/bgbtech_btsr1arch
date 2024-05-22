/*
 Copyright (c) 2018-2022 Brendan G Bohannon

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

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

#ifdef _WIN32
#include <windows.h>
#endif

#define USE_FENV
#include <fenv.h>

#include "jx2i_multi.c"

FILE *bjx2_vmoutlog=NULL;

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
}

void GfxDrv_Start()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	btesh_main_screen =
		SDL_SetVideoMode(640, 400, 32, SDL_SWSURFACE);
//		SDL_SetVideoMode(640, 480, 32, SDL_OPENGL);
	btesh2_gfxcon_fbxs=640;
	btesh2_gfxcon_fbys=400;
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


int BJX2_MainAddScanMouseByte(BJX2_Context *ctx, int k)
{
	ctx->ps2msbuf[ctx->ps2msrov]=k;
	ctx->ps2msrov=(ctx->ps2msrov+1)&255;
	return(0);
}

int BJX2_MainAddScanKeyByte(BJX2_Context *ctx, int k)
{
	ctx->ps2kbbuf[ctx->ps2kbrov]=k;
	ctx->ps2kbrov=(ctx->ps2kbrov+1)&255;
	return(0);
}

int BJX2_MainAddUsbKey(BJX2_Context *ctx, int key)
{
	static const short usb_key2scan[256]={
		0x000, 0x000, 0x000, 0x000,  0x000, 0x000, 0x000, 0x000,	//00..07
		0x02A, 0x02B, 0x000, 0x000,  0x000, 0x028, 0x000, 0x000,	//08..0F
		0x000, 0x000, 0x000, 0x000,  0x000, 0x000, 0x000, 0x000,	//10..17
		0x000, 0x000, 0x000, 0x029,  0x000, 0x000, 0x000, 0x000,	//18..1F

		0x02C, 0x31E, 0x334, 0x320,  0x321, 0x322, 0x324, 0x234,	//20..27
		0x326, 0x327, 0x325, 0x32E,  0x236, 0x22D, 0x237, 0x238,	//28..2F
		0x227, 0x21E, 0x21F, 0x220,  0x221, 0x222, 0x223, 0x224,	//30..37
		0x225, 0x226, 0x333, 0x233,  0x336, 0x22E, 0x337, 0x338,	//38..3F

		0x01F, 0x304, 0x305, 0x306,  0x307, 0x308, 0x309, 0x30A,	//40..47
		0x30B, 0x30C, 0x30D, 0x30E,  0x30F, 0x310, 0x311, 0x312,	//48..4F
		0x313, 0x314, 0x315, 0x316,  0x317, 0x318, 0x319, 0x31A,	//50..57
		0x31B, 0x31C, 0x31D, 0x32F,  0x331, 0x330, 0x323, 0x32D,	//58..5F

		0x235, 0x204, 0x205, 0x206,  0x207, 0x208, 0x209, 0x20A,	//60..67
		0x20B, 0x20C, 0x20D, 0x20E,  0x20F, 0x210, 0x211, 0x212,	//68..6F
		0x213, 0x214, 0x215, 0x216,  0x217, 0x218, 0x219, 0x21A,	//70..77
		0x21B, 0x21C, 0x21D, 0x22F,  0x231, 0x230, 0x235, 0x04C,	//78..7F

		0x052, 0x051, 0x050, 0x04F,  0x000, 0x000, 0x000, 0x03A,	//80..87
		0x03B, 0x03C, 0x03D, 0x03E,  0x03F, 0x040, 0x041, 0x042,	//88..8F
		0x043, 0x044, 0x045, 0x04A,  0x04E, 0x04B, 0x04A, 0x04D,	//90..97
		0x048, 0x000, 0x000, 0x000,  0x000, 0x000, 0x000, 0x000,	//98..9F

		0x039, 0x047, 0x000, 0x000,  0x000, 0x000, 0x000, 0x000,	//A0..A7
		0x000, 0x000, 0x000, 0x000,  0x000, 0x000, 0x000, 0x000,	//A8..AF
		0x000, 0x000, 0x000, 0x000,  0x000, 0x000, 0x000, 0x000,	//B0..B7
		0x000, 0x000, 0x000, 0x000,  0x000, 0x000, 0x000, 0x000,	//B8..BF

		0x000, 0x000, 0x000, 0x000,  0x000, 0x000, 0x000, 0x000,	//C0..C7
		0x000, 0x000, 0x000, 0x000,  0x057, 0x056, 0x055, 0x054,	//C8..CF
		0x062, 0x059, 0x05A, 0x05B,  0x05C, 0x05D, 0x05E, 0x05F,	//D0..D7
		0x060, 0x061, 0x058, 0x063,  0x000, 0x000, 0x000, 0x000,	//D8..DF

		0x000, 0x000, 0x000, 0x000,  0x000, 0x000, 0x000, 0x000,	//E0..E7
		0x000, 0x000, 0x000, 0x000,  0x000, 0x000, 0x000, 0x000,	//E8..EF
		0x000, 0x000, 0x000, 0x000,  0x000, 0x000, 0x000, 0x000,	//F0..F7
		0x000, 0x000, 0x000, 0x000,  0x000, 0x000, 0x000, 0x000,	//F8..FF
	};

	static short shift;
	static byte keys[8];
	int kix;
	int i, j, k;
	
	kix=usb_key2scan[key&0xFF];
	keys[6]=0;
	keys[7]=0;
	
	if(key&0x8000)
	{
		for(i=0; i<6; i++)
		{
			if(keys[i]==(kix&0xFF))
			{
				for(j=0; (i+j)<6; j++)
					keys[i+j]=keys[i+j+1];
			}
		}

		for(i=0; i<6; i++)
		{
			if(keys[i])
				break;
		}
		
		if(i>=6)
		{
			if(shift&0x100)
				shift|= 0x102;
			else
				shift&=~0x122;
		}

		if((key&0xFFF)==K_SHIFT)
		{
			shift&=~0x122;
		}

		if((key&0xFFF)==K_CTRL)
		{
			shift&=~0x001;
		}

		if((key&0xFFF)==K_ALT)
		{
			shift&=~0x004;
		}
	}else
	{
		if(kix&0xFF)
		{
			for(i=0; i<6; i++)
			{
				if(keys[i]==(kix&0xFF))
					break;
				if(!keys[i])
				{
					keys[i]=kix&0xFF;
					break;
				}
			}
		}
		
		if(kix&0x200)
		{
			if(kix&0x100)
			{
				shift|=0x02;
			}else
			{
				shift&=~0x02;
			}
		}

		if((key&0xFFF)==K_SHIFT)
		{
			shift|=0x102;
		}

		if((key&0xFFF)==K_CTRL)
		{
			shift|=0x001;
		}

		if((key&0xFFF)==K_ALT)
		{
			shift|=0x004;
		}
	}
	
	ctx->usbkb_report[0]=shift;	//Shift
	ctx->usbkb_report[1]=0x00;
	ctx->usbkb_report[2]=keys[0];
	ctx->usbkb_report[3]=keys[1];
	ctx->usbkb_report[4]=keys[2];
	ctx->usbkb_report[5]=keys[3];
	ctx->usbkb_report[6]=keys[4];
	ctx->usbkb_report[7]=keys[5];

	return(0);
}

int BJX2_MainAddTranslateKey(BJX2_Context *ctx, int key)
{
	static const short ps2_key2scan[256]={
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,		//00..07
		0x066, 0x00D, 0x000, 0x000, 0x000, 0x05A, 0x000, 0x000,		//08..0F
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,		//10..17
		0x000, 0x000, 0x000, 0x076, 0x000, 0x000, 0x000, 0x000,		//18..1F
		0x029, 0x016, 0x052, 0x026, 0x025, 0x02E, 0x03D, 0x052,		//20..27
		0x046, 0x045, 0x03E, 0x055, 0x041, 0x04E, 0x049, 0x04A,		//28..2F
		0x045, 0x016, 0x01E, 0x026, 0x025, 0x02E, 0x036, 0x03D,		//30..37
		0x03E, 0x046, 0x04C, 0x04C, 0x041, 0x055, 0x049, 0x04A,		//38..3F
		0x01E, 0x01C, 0x032, 0x021, 0x023, 0x024, 0x02B, 0x034,		//40..47
		0x033, 0x043, 0x03B, 0x042, 0x04B, 0x03A, 0x031, 0x044,		//48..4F
		0x04D, 0x015, 0x02D, 0x01B, 0x02C, 0x03C, 0x02A, 0x01D,		//50..57
		0x022, 0x035, 0x01A, 0x054, 0x05D, 0x05B, 0x036, 0x04E,		//58..5F
		0x00E, 0x01C, 0x032, 0x021, 0x023, 0x024, 0x02B, 0x034,		//60..67
		0x033, 0x043, 0x03B, 0x042, 0x04B, 0x03A, 0x031, 0x044,		//68..6F
		0x04D, 0x015, 0x02D, 0x01B, 0x02C, 0x03C, 0x02A, 0x01D,		//70..77
		0x022, 0x035, 0x01A, 0x054, 0x05D, 0x05B, 0x00E, 0x171,		//78..7F
		0x175, 0x172, 0x16B, 0x174, 0x011, 0x014, 0x012, 0x005,		//80..87
		0x006, 0x004, 0x00C, 0x003, 0x00B, 0x083, 0x00A, 0x001,		//88..8F
		0x009, 0x078, 0x007, 0x170, 0x000, 0x17A, 0x17D, 0x16C,		//90..97
		0x169, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,		//98..9F
		0x058, 0x07E, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,		//A0..A7
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,		//A8..AF
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,		//B0..B7
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,		//B8..BF
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,		//C0..C7
		0x000, 0x000, 0x000, 0x000, 0x079, 0x07B, 0x07C, 0x14A,		//C8..CF
		0x070, 0x069, 0x072, 0x07A, 0x06B, 0x073, 0x074, 0x06C,		//D0..D7
		0x075, 0x07D, 0x15A, 0x071, 0x000, 0x000, 0x000, 0x000,		//D8..DF
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,		//E0..E7
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,		//E8..EF
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,		//F0..F7
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,		//F8..FF
	};
	
	int sc;
	
	BJX2_MainAddUsbKey(ctx, key);
	
	if(ctx->do_usb_hid)
	{
		/* USB HID disables PS/2 inputs */
		return(0);
	}
	
	sc=ps2_key2scan[key&0xFF];
	if(!sc)
		return(0);

	if(key&0x8000)
	{
		if(sc&0x100)
			BJX2_MainAddScanKeyByte(ctx, 0xE0);
		BJX2_MainAddScanKeyByte(ctx, 0xF0);
		BJX2_MainAddScanKeyByte(ctx, sc&0xFF);
//		BJX2_MainAddTranslateKey(ctx, key&0x7FFF);
		return(0);
	}

	if(sc&0x100)
		BJX2_MainAddScanKeyByte(ctx, 0xE0);
	BJX2_MainAddScanKeyByte(ctx, sc&0xFF);
	return(0);
}

int BJX2_MainAddKeyByte(BJX2_Context *ctx, int k)
{
	ctx->kbbuf[ctx->kbrov]=k;
	ctx->kbrov=(ctx->kbrov+1)&255;
	ctx->kbirq++;
	return(0);
}

int BJX2_MainPollKeyboard(BJX2_Context *ctx)
{
	static int lmb, msflg, msrt, l1msrt, l2msrt;
	static int lvmx, lvmy, lvmd;
	u16 *kb;
	int mdx, mdy, vmx, vmy, amx, amy, amsx, amsy, mb;
	int t0;
	int i, j, k;

#ifdef _WIN32
	while(_kbhit())
	{
		j=_getch();
		
		if(j=='`')
			ctx->req_kill=1;

//		if(j=='\r')
//			j=0x0A0D;
		
		ctx->kbbuf[ctx->kbrov]=j;
		ctx->kbrov=(ctx->kbrov+1)&255;
		ctx->kbirq=1;
		
		if(j>>8)
		{
			ctx->kbbuf[ctx->kbrov]=j>>8;
			ctx->kbrov=(ctx->kbrov+1)&255;
			ctx->kbirq++;
		}
	}
#endif

#ifdef linux
	j=fgetc(stdin);
	while(j>=0)
	{
//		printf("key %d\n", j);
	
		if(j=='`')
			ctx->req_kill=1;

//		if(j=='\r')
//			j=0x0A0D;
		
		ctx->kbbuf[ctx->kbrov]=j;
		ctx->kbrov=(ctx->kbrov+1)&255;
		ctx->kbirq=1;
		
		if(j>>8)
		{
			ctx->kbbuf[ctx->kbrov]=j>>8;
			ctx->kbrov=(ctx->kbrov+1)&255;
			ctx->kbirq++;
		}

		j=fgetc(stdin);
	}
#endif

#if 1
	kb=FRGL_GetKeybuf();
	while(*kb)
	{
		k=*kb++;
		
		lvmd=0;

		if(FRGL_KeyDown(K_SHIFT))
		{
			if(k==K_F5)
			{
				/* Graphical Debugger Escape */
				jx2i_gfxcon_isdbg=!jx2i_gfxcon_isdbg;
				jx2i_gfxcon_dirty=1;
				BJX2_GfxDebugActivate(ctx);
				continue;
			}
		}

		if(!jx2i_gfxcon_isdbg)
		{
			BJX2_MainAddTranslateKey(ctx, k);
		}else
		{
			if(k==K_F5)
			{
				/* Continue */
				jx2i_gfxcon_isdbg=0;
				jx2i_gfxcon_dirty=1;
				continue;
			}
			BJX2_GfxDebugAddKey(ctx, k);
		}

#if 0
		if(k&0x8000)
		{
			j=k&4095;
			if(j<0x7F)
			{
				BJX2_MainAddKeyByte(ctx, j|0x80);
			}else if(j<0xFF)
			{
				BJX2_MainAddKeyByte(ctx, 0xFF);
				BJX2_MainAddKeyByte(ctx, j);
			}
		}else
		{
			j=k&4095;
			if(j<0x7F)
			{
				BJX2_MainAddKeyByte(ctx, j);
			}else if(j<0xFF)
			{
				BJX2_MainAddKeyByte(ctx, 0x7F);
				BJX2_MainAddKeyByte(ctx, j);
			}
		}
#endif
	}

	if(ctx->ps2txmsrov!=ctx->ps2txmsirov)
	{
		i=ctx->ps2txmsbuf[ctx->ps2txmsirov];
		switch(i)
		{
		case 0xFF:
		case 0xF6:
			msflg=0;
			BJX2_MainAddScanMouseByte(ctx, 0xFA);
			ctx->ps2txmsirov=(ctx->ps2txmsirov+1)&0xFF;
			break;
		case 0xFE:
			lmb=-1;
			ctx->ps2txmsirov=(ctx->ps2txmsirov+1)&0xFF;
			break;

		case 0xF5:
			msflg&=~1;
			BJX2_MainAddScanMouseByte(ctx, 0xFA);
			ctx->ps2txmsirov=(ctx->ps2txmsirov+1)&0xFF;
			break;
		case 0xF4:
			msflg|=1;
			BJX2_MainAddScanMouseByte(ctx, 0xFA);
			ctx->ps2txmsirov=(ctx->ps2txmsirov+1)&0xFF;
			break;

		case 0xF3:
			if(((ctx->ps2txmsirov+1)&0xFF)!=ctx->ps2txmsrov)
			{
				l2msrt=l1msrt;
				l1msrt=msrt;
				msrt=ctx->ps2txmsbuf[(ctx->ps2txmsirov+1)&0xFF];
				BJX2_MainAddScanMouseByte(ctx, 0xFA);
				ctx->ps2txmsirov=(ctx->ps2txmsirov+2)&0xFF;
				
				if((l2msrt==200) && (l1msrt==100) && (msrt==80))
				{
					msflg|=8;
				}

				if((l2msrt==200) && (l1msrt==200) && (msrt==80))
				{
					msflg|=8;
				}
			}
			break;

		case 0xF2:
			BJX2_MainAddScanMouseByte(ctx, 0xFA);
			if(msflg&8)
				BJX2_MainAddScanMouseByte(ctx, 0x03);
			else
				BJX2_MainAddScanMouseByte(ctx, 0x00);
			ctx->ps2txmsirov=(ctx->ps2txmsirov+1)&0xFF;
			break;

		case 0xF0:
			msflg|=2;
			BJX2_MainAddScanMouseByte(ctx, 0xFA);
			ctx->ps2txmsirov=(ctx->ps2txmsirov+1)&0xFF;
			break;

		case 0xEB:
			msflg|=4;
			BJX2_MainAddScanMouseByte(ctx, 0xFA);
			ctx->ps2txmsirov=(ctx->ps2txmsirov+1)&0xFF;
			break;

		case 0xEA:
			msflg&=~2;
			BJX2_MainAddScanMouseByte(ctx, 0xFA);
			ctx->ps2txmsirov=(ctx->ps2txmsirov+1)&0xFF;
			break;


		default:
			ctx->ps2txmsirov = ctx->ps2txmsrov;
			break;
		}
	}
	
	t0=FRGL_TimeMS();

	if(((msflg&3)==1) || (msflg&4))
	{
		i=bjx2_vmgetmousepos(ctx, &vmx, &vmy);
		
		if((lvmx!=vmx) || (lvmy!=vmy))
			lvmd=0;
		
		if(lvmd && (t0>(lvmd+500)))
			lvmd=0;

		if(i>0)
		{
			GfxDrv_MouseGetPos(&amx, &amy, &mb);
			amsx=(amx/2)+320;
			amsy=(amy/2)+200;
			
			mdx=amsx-vmx;
			mdy=amsy-vmy;
			
//			if((mdx>25) && (mdx<25))
//				mdx=0;
//			if((mdy>25) && (mdy<25))
//				mdy=0;

			if(lvmd)
			{
				mdx=0;
				mdy=0;
			}
			
#if 0
			if(mdx || mdy)
			{
				printf("amx/amy=%d,%d vmx/vmy=%d,%d mdx/mdy=%d,%d\n",
					amsx, amsy,
					vmx, vmy,
					mdx, mdy);
			}
#endif
			
			lvmx=vmx;
			lvmy=vmy;
			if(mdx || mdy)
			{
				lvmd=FRGL_TimeMS();
			}
		}else
		{
			GfxDrv_MouseGetRelPos(&mdx, &mdy, &mb);
		}

		if(mdx || mdy || (mb!=lmb))
		{
//			j=0x08|(mb&7);
			j=0x08;
			if(mb&1)
				j|=0x01;
			if(mb&2)
				j|=0x04;
			if(mb&4)
				j|=0x02;
			if(mdx<0)
				j|=0x10;
			if(mdy<0)
				j|=0x20;
			if((mdx^(mdx>>31))&(~255))
				j|=0x40;
			if((mdy^(mdy>>31))&(~255))
				j|=0x80;

			lmb=mb;
			msflg&=~4;

			BJX2_MainAddScanMouseByte(ctx, j);
			BJX2_MainAddScanMouseByte(ctx, mdx&0xFF);
			BJX2_MainAddScanMouseByte(ctx, mdy&0xFF);
			
			if(msflg&8)
			{
				j=GfxDrv_MouseGetWheelDelta();
				if(j<-8)	j=-8;
				if(j>7)		j= 7;
				j&=15;
				BJX2_MainAddScanMouseByte(ctx, j);
			}
		}
	}
#endif

	return(0);
}



BJX2_FILE *bjx2_fopen(char *name, char *mode)
{
	FILE *fd;
	fd=fopen(name, mode);
	return((BJX2_FILE *)fd);
}

int bjx2_fclose(BJX2_FILE *fd)
{
	fclose((FILE *)fd);
	return(0);
}

int bjx2_feof(BJX2_FILE *fd)
{
	return(feof((FILE *)fd));
}

s64 bjx2_ftell(BJX2_FILE *fd)
{
	return(ftell((FILE *)fd));
}

int bjx2_fseek(BJX2_FILE *fd, s64 pos, int md)
{
	return(fseek((FILE *)fd, pos, md));
}

int bjx2_fread(void *buf, int n, int m, BJX2_FILE *fd)
{
	return(fread(buf, n, m, (FILE *)fd));
}

int bjx2_fwrite(void *buf, int n, int m, BJX2_FILE *fd)
{
	return(fwrite(buf, n, m, (FILE *)fd));
}

int bjx2_fgetc(BJX2_FILE *fd)
{
	return(fgetc((FILE *)fd));
}

int bjx2_fputc(int val, BJX2_FILE *fd)
{
	return(fputc(val, (FILE *)fd));
}

char *bjx2_fgets(char *buf, int lim, BJX2_FILE *fd)
{
	char *t;
	int i;
	
	t=buf;
	while(!bjx2_feof(fd))
	{
		i=bjx2_fgetc(fd);
		if(i=='\n')
		{
			break;
		}
		if(i=='\r')
		{
			continue;
		}
		if(i<0)
		{
			break;
		}
		
		*t++=i;
	}
	
	*t=0;
	
	return(buf);
}

int bjx2_fflush(BJX2_FILE *fd)
{
	return(fflush((FILE *)fd));
}

//char bjx2_vmputlinebuf[256];
//byte bjx2_vmputlinepos;

int bjx2_vmcheckdbg(BJX2_Context *ctx, char *line)
{
	char tb[256], tb1[256];
	char *s, *t, *s1, *t1;
	u32 a_lo, a_hi;
	u64 addr;
	int i, j, k;
	
	if(!strncmp(line, "TKPE!LDA:", 9))
	{
		s=line+9;
		t=tb;
		while(*s && (*s!='='))
			*t++=*s++;
		*t++=0;
		
		s1=tb+strlen(tb);
		t1=tb1;
		while((s1>tb) && (*s1!='/'))
			s1--;
//		if(*s1=='/')
		if(1)
		{
			s1++;
			while(*s1 && *s1!='.')
				*t1++=*s1++;
			*t1++=0;
			strncpy(tb, tb1, 255);
		}
		
		if(*s=='=')
		{
			s++;
			sscanf(s, "%04X_%08X", &a_hi, &a_lo);
			addr=(((u64)a_hi)<<32)|a_lo;
			
			for(i=0; i<ctx->n_map_img; i++)
			{
				if(!strcmp(ctx->map_img_name[i], tb))
				{
					ctx->map_img_base[i]=addr;
					break;
				}
			}
			
			if(i>=ctx->n_map_img)
			{
				i=ctx->n_map_img++;
				ctx->map_img_name[i]=strdup(tb);
				ctx->map_img_base[i]=addr;
			}
		}
	}

	if(!strncmp(line, "TKPE!MS_X:", 10))
	{
		s=line+10;
		sscanf(s, "%04X_%08X", &a_hi, &a_lo);
		addr=(((u64)a_hi)<<32)|a_lo;
		ctx->vm_mousex=addr;
	}

	if(!strncmp(line, "TKPE!MS_Y:", 10))
	{
		s=line+10;
		sscanf(s, "%04X_%08X", &a_hi, &a_lo);
		addr=(((u64)a_hi)<<32)|a_lo;
		ctx->vm_mousey=addr;
	}

	return(0);
}

int bjx2_vmputc(BJX2_Context *ctx, int val)
{
	if(val=='\n')
	{
		ctx->puts_linebuf[ctx->puts_linepos++]=val;

		ctx->puts_linebuf[ctx->puts_linepos]=0;
		ctx->puts_linepos=0;

		bjx2_vmcheckdbg(ctx, ctx->puts_linebuf);

		fputs(ctx->puts_linebuf, bjx2_vmoutlog);
		fputs(ctx->puts_linebuf, stdout);
	}else
	{
		ctx->puts_linebuf[ctx->puts_linepos++]=val;
	}

#if 0
	if(bjx2_vmoutlog)
		fputc(val, bjx2_vmoutlog);

	fputc(val, stdout);
#endif

	return(0);

#if 0
	static char pbuf[1024];
	static int pbpos=0;
	
	if((val>=' '))
	{
		pbuf[pbpos++]=val;
		return(0);
	}
	
	if(val=='\n')
	{
		pbuf[pbpos++]=val;
		pbuf[pbpos++]=0;
//		BGBRPI_AddConMessage(pbuf);
		puts(pbuf);
		pbpos=0;
		return(0);
	}
#endif

	return(0);
}

int bjx2_vmgetmousepos(BJX2_Context *ctx, int *rmx, int *rmy)
{
	if(!ctx->vm_mousex || !ctx->vm_mousey)
		return(-1);
	*rmx=BJX2_MemGetDWord(ctx, ctx->vm_mousex);
	*rmy=BJX2_MemGetDWord(ctx, ctx->vm_mousey);
	return(1);
}

int BJX2_VmMsgRxUpdate(BJX2_Context *ctx)
{
	return(0);
}

int BJX2_VmMsgTxUpdate(BJX2_Context *ctx)
{
//	BGBDT_Jx2Vm_UpdateMessages();
	return(0);
}



BJX2_Context *ctx;
BJX2_Context *ctx_c2;
int ms0, lms1, lms2;

void btesh_main_iterate()
{
	s64 cyc;
	int ms1, ms, dtms, dtms2;
	int i, rcy, rsc;

	ms1=FRGL_TimeMS();
	ms=ms1-ms0;
	
//	if((ms>1000) || (ms<0))
//		ms=0;
	
//	cyc=100000LL*ms;
//	cyc=ctx->tgt_mhz*1000LL*ms;
	cyc=ctx->tgt_mhz*1000LL*(ms+120);
	dtms=ms1-lms1;
	lms1=ms1;

	if(ctx->tot_cyc>cyc)
	{
		BJX2_MainPollKeyboard(ctx);
		BJX2_SndSblk_Update(ctx, dtms);
#ifdef linux
		usleep(0);
#else
//		Sleep(1);
		Sleep(0);
#endif
		return;
	}

	if(jx2i_gfxcon_isdbg)
	{
		ms0+=dtms;
//		cyc=0;
	}

	BJX2_SndSblk_Update(ctx, dtms);
	JX2I_GfxCon_Update();

//	rcy=ctx->tgt_mhz*1000000/45;
//	rcy=(ctx->tgt_mhz*1000000LL)/288;
//	rcy=(ctx->tgt_mhz*1000000LL)/144;
	rcy=(ctx->tgt_mhz*1000000LL)/72;
//	rcy=(ctx->tgt_mhz*1000000LL)/45;
//	rcy=(ctx->tgt_mhz*1000000LL)/30;
//	rcy=(ctx->tgt_mhz*1000000LL)/15;
	
//	rsc=256+((ctx->tgt_mhz-100)>>3);
	rsc=256+((ctx->tgt_mhz-100)>>2);
//	rsc=256+((ctx->tgt_mhz-100)>>1);
//	rsc=(256*ctx->tgt_mhz)/100;

//	if(rsc>256)
//		rcy=(((s64)rcy)*rsc)>>8;

	GfxDrv_BeginDrawing();
	BJX2_MainPollKeyboard(ctx);
//	i=BJX2_RunLimit(ctx, 999999);
//	i=BJX2_RunLimit(ctx, 9999999);
//	i=BJX2_RunLimit(ctx, 5000000);
//	i=BJX2_RunLimit(ctx, 2500000);
//	i=BJX2_RunLimit(ctx, rcy);
//	lms2=ms1;

	if(jx2i_gfxcon_isdbg)
	{
		BJX2_GfxDebugRefresh(ctx);
	}

	i=0;
	if(!jx2i_gfxcon_isdbg)
	{
		do {
			i=BJX2_RunLimit(ctx, rcy);

			if(i || ctx->req_kill)
				break;

			ms1=FRGL_TimeMS();
			ms=ms1-lms1;

	//		dtms2=ms1-lms2;
	//		lms2=ms1;
	//		BJX2_SndSblk_Update(ctx, dtms2);
		} while((ctx->tot_cyc<cyc) && (ms<28));
	}

	if(i==BJX2_FLT_EMUBREAK)
	{
		ctx->status=0;
		jx2i_gfxcon_isdbg=1;
		i=0;
	}

	if(i || ctx->req_kill)
	{
		jx2i_gfxcon_isdbg=0;
		gfxdrv_kill=1;
	}

#if 1
	if(!jx2i_gfxcon_isdbg)
	{
		if(ctx_c2)
		{
			do {
				i=BJX2_RunLimit(ctx_c2, rcy);

				if(i || ctx_c2->req_kill)
					break;

				ms1=FRGL_TimeMS();
				ms=ms1-lms1;
			} while((ctx_c2->tot_cyc<cyc) && (ms<28));
		}
	}
#endif

// #ifndef linux
//	if(i || ctx->req_kill)
//		gfxdrv_kill=1;
// #endif
	
//	t1=clock();
//	if(t1>t0)
//		tt+=t1-t0;
//	t0=t1;

	GfxDrv_EndDrawing();
}

int main(int argc, char *argv[])
{
	char *rd_add[64];
	char *rd_exp[64];
	char *rd_map[64];
	int rd_n_add;
	int rd_n_exp;
	int rd_n_map;
	char tb1[256], tb2[256];

	char *ifn, *s, *t, *l1icfg, *l1dcfg, *l2cfg, *isacfg;
	double tsec;
	int t0, t1, tt, fbtt, tvus;
	int ifmd, rdsz, mhz, usejit, swapsz, chkbss, nomemcost, walltime;
	int dousbhid, dorast, nowex;
	int i;
	
	rd_n_add=0;
	rd_n_exp=0;
	rd_n_map=0;
	ifn=NULL;
//	ifmd=0; rdsz=128;
//	ifmd=0; rdsz=256;
	ifmd=0; rdsz=512;
//	swapsz=384;
	swapsz=0;
//	mhz=100; usejit=0; nomemcost=0;
	mhz=50; usejit=0; nomemcost=0;
	chkbss=0;
	l1icfg=NULL;
	l1dcfg=NULL;
	isacfg=NULL;
	l2cfg=NULL;
	walltime=0;
	dousbhid=0;
	dorast=1;
	nowex=0;
	
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "--rd_sz"))
				{ rdsz=atoi(argv[i+1]); i++; continue; }
			if(!strcmp(argv[i], "--rd_add"))
				{ ifmd=1; continue; }
			if(!strcmp(argv[i], "--rd_exp"))
				{ ifmd=2; continue; }
			if(!strcmp(argv[i], "--rd_map"))
				{ ifmd=3; continue; }

			if(!strcmp(argv[i], "--mhz"))
				{ mhz=atoi(argv[i+1]); i++; continue; }

			if(!strcmp(argv[i], "--walltime"))
				{ walltime=1; continue; }

			if(!strcmp(argv[i], "--wallspeed"))
			{
				walltime=1;
				nomemcost=3;
				mhz=900;
				continue;
			}

			if(!strcmp(argv[i], "--swap"))
				{ swapsz=atoi(argv[i+1]); i++; continue; }
			if(!strcmp(argv[i], "--noswap"))
				{ swapsz=0; continue; }

			if(!strcmp(argv[i], "--l1cfg"))
				{ l1icfg=argv[i+1]; l1dcfg=l1icfg; i++; continue; }
			if(!strcmp(argv[i], "--l1icfg"))
				{ l1icfg=argv[i+1]; i++; continue; }
			if(!strcmp(argv[i], "--l1dcfg"))
				{ l1dcfg=argv[i+1]; i++; continue; }

			if(!strcmp(argv[i], "--l2cfg"))
				{ l2cfg=argv[i+1]; i++; continue; }

			if(!strcmp(argv[i], "--isacfg"))
				{ isacfg=argv[i+1]; i++; continue; }

			if(!strcmp(argv[i], "--jit"))
				{ usejit=1; continue; }
			if(!strcmp(argv[i], "--nojit"))
				{ usejit=2; continue; }

			if(!strcmp(argv[i], "--memcost"))
				{ nomemcost=2; continue; }
			if(!strcmp(argv[i], "--nomemcost"))
				{ nomemcost=3; continue; }

			if(!strcmp(argv[i], "--chkbss"))
				{ chkbss=1; continue; }

			if(!strcmp(argv[i], "--usbhid"))
				{ dousbhid=1; continue; }

			if(!strcmp(argv[i], "--norast"))
				{ dorast=0; continue; }
			if(!strcmp(argv[i], "--nowex"))
				{ nowex|=1; continue; }
			if(!strcmp(argv[i], "--opssc"))
				{ nowex|=2; continue; }
			if(!strcmp(argv[i], "--breakperf"))
				{ nowex|=2; continue; }

			continue;
		}
		
		if(!ifn)
			{ ifn=argv[i]; continue; }

		if(ifmd==1)
			{ rd_add[rd_n_add++]=argv[i]; continue; }
		if(ifmd==2)
			{ rd_exp[rd_n_exp++]=argv[i]; continue; }
		if(ifmd==3)
			{ rd_map[rd_n_map++]=argv[i]; continue; }
	}
	
	bjx2_vmoutlog=fopen("bjx2_vmoutlog.txt", "wb");
	
	JX2R_UseImageCreateRamdisk(rdsz*1024);
	
	for(i=0; i<rd_n_add; i++)
	{
		JX2R_UseImageAddFile(rd_add[i], NULL);
	}
	for(i=0; i<rd_n_exp; i++)
		JX2R_UseImageAddExport(rd_exp[i], NULL);
	
	if(swapsz>0)
	{
		JX2R_UseImageAddFileBuffer("swapfile.sys", NULL, swapsz*(1<<20));
	}
	
	ctx=BJX2_AllocContext();
//	BJX2_MemDefineROM(ctx,		"ROM",	0x00000000U, 0x00007FFFU);
	BJX2_MemDefineROM(ctx,		"ROM",	0x00000000U, 0x0000BFFFU);
//	BJX2_MemDefineRAM(ctx,		"SRAM",	0x0000C000U, 0x0000DFFFU);
	BJX2_MemDefineRAM(ctx,		"SRAM",	0x0000C000U, 0x0000FFFFU);

	BJX2_MemDefineROM(ctx,		"ZERO",	0x00010000U, 0x0003FFFFU);

//	BJX2_MemDefineMmgp(ctx,		"MMGP",	0x0000E000U, 0x0000E3FFU);
//	BJX2_MemDefineGfxCon(ctx,	"CGFX",	0x000A0000U, 0x000AFFFFU);
//	BJX2_MemDefineRAM(ctx,		"DRAM",	0x01000000U, 0x18000000U);
//	BJX2_MemDefineRAM(ctx,		"DRAM",	0x01000000U, 0x08000000U);
//	BJX2_MemDefineModRAM(ctx,	"DRAM",	0x01000000U, 0x18000000U, 0x08000000U);
	BJX2_MemDefineModRAM(ctx,	"DRAM",	0x01000000U, 0x3F000000U, 0x08000000U);

//	BJX2_MemDefineMmgp(ctx,		"MMGP",	0xA000E000U, 0xA000E3FFU);
//	BJX2_MemDefineSndSblk(ctx,	"SBAU",	0xA0080000U, 0xA0081FFFU);
//	BJX2_MemDefineGfxCon(ctx,	"CGFX",	0xA00A0000U, 0xA00AFFFFU);

	BJX2_MemDefineUsbBuf(ctx,	"MMSB",	0xF0008000U, 0xF000BFFFU);
	BJX2_MemDefineMmgp(ctx,		"MMGP",	0xF000E000U, 0xF000EFFFU);
	BJX2_MemDefineSndSblk(ctx,	"SBAU",	0xF0080000U, 0xF0081FFFU);
//	BJX2_MemDefineGfxCon(ctx,	"CGFX",	0xF00A0000U, 0xF00AFFFFU);
	BJX2_MemDefineSmus(ctx,		"SMUS",	0xF008C000U, 0xF008FFFFU);
	BJX2_MemDefineSndAuPcm(ctx,	"SPCM",	0xF0090000U, 0xF009FFFFU);
	BJX2_MemDefineGfxCon(ctx,	"CGFX",	0xF00A0000U, 0xF00BFFFFU);

#ifndef BJX2_ADDR32
// #if 0
//	BJX2_MemDefineMmgp(ctx,		"MMGP",	0xFFFFFFFFA000E000, 0xFFFFFFFFA000E3FF);
//	BJX2_MemDefineSndSblk(ctx,	"SBAU",	0xFFFFFFFFA0080000, 0xFFFFFFFFA0081FFF);
//	BJX2_MemDefineGfxCon(ctx,	"CGFX",	0xFFFFFFFFA00A0000, 0xFFFFFFFFA00AFFFF);

	BJX2_MemDefineUsbBuf(ctx,	"MUSB",	0xFFFFF0008000LL, 0xFFFFF000BFFFLL);
	BJX2_MemDefineMmgp(ctx,		"MMGP",	0xFFFFF000E000LL, 0xFFFFF000EFFFLL);
	BJX2_MemDefineSndSblk(ctx,	"SBAU",	0xFFFFF0080000LL, 0xFFFFF0081FFFLL);
//	BJX2_MemDefineGfxCon(ctx,	"CGFX",	0xFFFFF00A0000LL, 0xFFFFF00AFFFFLL);
	BJX2_MemDefineSmus(ctx,		"SMUS",	0xFFFFF008C000LL, 0xFFFFF008FFFFLL);
	BJX2_MemDefineSndAuPcm(ctx,	"SPCM",	0xFFFFF0090000LL, 0xFFFFF009FFFFLL);
	BJX2_MemDefineGfxCon(ctx,	"CGFX",	0xFFFFF00A0000LL, 0xFFFFF00BFFFFLL);
#endif
	
#ifndef BJX2_ADDR32
	BJX2_MemDefineUsbBuf(ctx,	"MUSB",	0xF00000008000LL, 0xF0000000BFFFLL);
	BJX2_MemDefineMmgp(ctx,		"MMGP",	0xF0000000E000LL, 0xF0000000EFFFLL);
	BJX2_MemDefineSndSblk(ctx,	"SBAU",	0xF00000080000LL, 0xF00000081FFFLL);
	BJX2_MemDefineSmus(ctx,		"SMUS",	0xF0000008C000LL, 0xF0000008FFFFLL);
	BJX2_MemDefineSndAuPcm(ctx,	"SPCM",	0xF00000090000LL, 0xF0000009FFFFLL);
	BJX2_MemDefineGfxCon(ctx,	"CGFX",	0xF000000A0000LL, 0xF000000BFFFFLL);
#endif

	BJX2_MemDefineEdgeWalk(ctx, "EDGE", 0x0000F000C000LL, 0x0000F000CFFFLL);
	BJX2_MemDefineEdgeWalk(ctx, "EDGE", 0xFFFFF000C000LL, 0xFFFFF000CFFFLL);

	BJX2_SetCpuConfig(ctx, isacfg);
	
	if(nowex&1)
	{
//		ctx->cfg_fflags&=~BJX2_FFLAG_WEX;
//		ctx->cfg_fflags&=~BJX2_FFLAG_WEX3W;
		ctx->cfg_fdflags|=BJX2_FFLAG_WEX|BJX2_FFLAG_WEX3W;
	}

	if(nowex&2)
	{
		ctx->do_opssc=1;
	}

	if(nowex&4)
	{
		ctx->do_breakperf=1;
	}

	if(!l1icfg)
		l1icfg="16k1";
	if(!l1dcfg)
	{
//		l1dcfg="32k1";
		l1dcfg="16k1";
	}
	if(!l2cfg)
//		l2cfg="128k2";
		l2cfg="256k1";

//	if(l1cfg)
	BJX2_MemSimSetConfigL1I(ctx, l1icfg);
	BJX2_MemSimSetConfigL1D(ctx, l1dcfg);
	BJX2_MemSimSetConfigL2(ctx, l2cfg);
	
	ctx->use_walltime=walltime;
	ctx->do_usb_hid=dousbhid;

	ctx->do_edgewalk=dorast;
	
	BJX2_ContextSetupZero(ctx);
	if(ifn)
	{
		BJX2_ContextLoadRom(ctx, ifn);
	}

	for(i=0; i<rd_n_map; i++)
	{
		BJX2_ContextLoadMap(ctx, rd_map[i], NULL);
	}

	for(i=0; i<rd_n_add; i++)
	{
		s=rd_add[i];
		t=tb1;
		while(*s && (*s!='='))
			*t++=*s++;
		*t++=0;

		if(*s=='=')
			s++;

		t=tb2;
		while(*s)
			*t++=*s++;
		*t++=0;

		if(tb1[0] && tb2[0])
		{
			strcat(tb2, ".map");
			BJX2_ContextLoadMap(ctx, tb2, tb1);
		}
			
//		JX2R_UseImageAddFile(rd_add[i], NULL);
	}

	if(chkbss)
	{
		ctx->dbg_data_start=BJX2_DbgAddrForName(ctx, "__data_start");
		ctx->dbg_data_end=BJX2_DbgAddrForName(ctx, "__data_end");
		ctx->dbg_bss_start=BJX2_DbgAddrForName(ctx, "__bss_start");
		ctx->dbg_bss_end=BJX2_DbgAddrForName(ctx, "__bss_end");
	}else
	{
		ctx->dbg_data_start=0;
		ctx->dbg_data_end=0;
		ctx->dbg_bss_start=0;
		ctx->dbg_bss_end=0;
	}

//	ctx_c2=BJX2_CreateSubContext(ctx);
	ctx_c2=NULL;
	
	ctx->core_id=0;
	if(ctx_c2)
		ctx_c2->core_id=1;

	ctx->use_jit=0;
	ctx->no_memcost=0;
	if(mhz>200)
//		ctx->use_jit=1;
		ctx->no_memcost=1;

#if defined(ARM) || defined(ARM64)
//	ctx->use_jit=1;
//	ctx_c2->use_jit=1;
	ctx->no_memcost=1;
	if(ctx_c2)
		ctx_c2->no_memcost=1;
#endif

	if(usejit)
	{
		ctx->use_jit=(usejit&1);
		if(ctx_c2)
			ctx_c2->use_jit=(usejit&1);
	}

	if(nomemcost)
	{
		ctx->no_memcost=(nomemcost&1);
		if(ctx_c2)
			ctx_c2->no_memcost=(nomemcost&1);
	}

//	ctx->ttick_hk=3052;
//	ctx->ttick_rst=3052;
	
	ctx->tgt_mhz=mhz;
//	ctx->tgt_mhz=100;
//	ctx->tgt_mhz=200;
//	ctx->tgt_mhz=300;
	ctx->rcp_mhz=((1048576/ctx->tgt_mhz)+7)>>4;
	ctx->ttick_rst=(ctx->tgt_mhz*1000000)/1024;
//	ctx->ttick_rst=100000000/1024;
	ctx->ttick_hk=ctx->ttick_rst;

	BJX2_ContextPowerOnState(ctx);

	if(ctx_c2)
	{
		ctx_c2->tgt_mhz=ctx->tgt_mhz;
		ctx_c2->rcp_mhz=ctx->rcp_mhz;
		ctx_c2->ttick_rst=ctx->ttick_rst;
		ctx_c2->ttick_hk=ctx->ttick_hk;

		ctx_c2->do_opssc=ctx->do_opssc;
		ctx_c2->cfg_fflags=ctx->cfg_fflags;
		ctx_c2->cfg_fdflags=ctx->cfg_fdflags;

		BJX2_ContextPowerOnState(ctx);
	}

//	ctx_c2=NULL;

#ifdef __linux
	i = fcntl(0, F_GETFL, 0);
	fcntl(0, F_SETFL, i | O_NONBLOCK);
	btesh2_ttynoncanon();
#endif

	GfxDrv_Start();
	SoundDev_Init();
	JX2I_GfxCon_Startup(ctx);
	SMus_Init(ctx);

//	t0=clock();
	t0=FRGL_TimeMS();

	ms0=FRGL_TimeMS();
	GfxDrv_MainLoop(btesh_main_iterate);

//	t1=clock();
	t1=FRGL_TimeMS();
	tt=t1-t0;
	
	fbtt=t1-jx2i_gfxcon_cbffms;
	
	tvus=BJX2_Interp_GetVirtualUsec(ctx);

	SoundDev_DeInit();

#ifdef __linux
	btesh2_resettermios();
#endif

//	t0=clock();
//	t1=t0;
//	tt=0;
//	while(1)
//	{
//		
//		if(i)
//			break;
//		if(ctx->req_kill)
//			break;
//	}
//	if(i)

	BJX2_DbgDump(ctx);
	
	if(bjx2_vmoutlog)
		fclose(bjx2_vmoutlog);

	i=ctx->status;

#if 1
	if(1)
	{
		if(fbtt>0)
		{
			tsec=fbtt/1000.0;
			printf("%.2f fps\n", jx2i_gfxcon_cbfrnum/tsec);

		}

		if(tt>0)
		{
			tsec=tt/1000.0;
			printf("%.2f TLB miss/sec\n", (ctx->tot_cnt_tlbmiss)/tsec);
		}


//		tsec=tt/((double)CLOCKS_PER_SEC);
		tsec=tt/1000.0;
		
//		BJX2_DbgDump(ctx);
		printf("%.2fs %.2fMIPS %.2fMHz usec=%3.2f%% timer_irq=%4.2fHz\n",
			tsec,
			ctx->tot_ops/(tsec*1000000.0),
			ctx->tot_cyc/(tsec*1000000.0),
			(tvus/10000.0)/tsec,
			ctx->nttick_irq/tsec);
		printf("Avg Bundle: %f\n", (ctx->tot_ops*1.0)/ctx->tot_nbops);
		printf("Exit Status %04X\n", i);
	}
#endif

	fflush(stdout);
}
