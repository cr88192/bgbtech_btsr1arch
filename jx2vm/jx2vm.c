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


int BJX2_MainAddKeyByte(BJX2_Context *ctx, int k)
{
	ctx->kbbuf[ctx->kbrov]=k;
	ctx->kbrov=(ctx->kbrov+1)&255;
	ctx->kbirq++;
	return(0);
}

int BJX2_MainPollKeyboard(BJX2_Context *ctx)
{
	u16 *kb;
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


int bjx2_vmputc(BJX2_Context *ctx, int val)
{
	if(bjx2_vmoutlog)
		fputc(val, bjx2_vmoutlog);

	fputc(val, stdout);
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
int ms0, lms1;

void btesh_main_iterate()
{
	s64 cyc;
	int ms1, ms, dtms;
	int i, rcy, rsc;

	ms1=FRGL_TimeMS();
	ms=ms1-ms0;
//	cyc=100000LL*ms;
	cyc=ctx->tgt_mhz*1000LL*ms;
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

	BJX2_SndSblk_Update(ctx, dtms);
	JX2I_GfxCon_Update();

//	rcy=ctx->tgt_mhz*1000000/45;
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

	do {
		i=BJX2_RunLimit(ctx, rcy);

		if(i || ctx->req_kill)
			break;

		ms1=FRGL_TimeMS();
		ms=ms1-lms1;
	} while((ctx->tot_cyc<cyc) && (ms<28));
	
// #ifndef linux
	if(i || ctx->req_kill)
		gfxdrv_kill=1;
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

	char *ifn;
	double tsec;
	int t0, t1, tt, fbtt, tvus;
	int ifmd, rdsz, mhz;
	int i;
	
	rd_n_add=0;
	rd_n_exp=0;
	rd_n_map=0;
	ifn=NULL;
	ifmd=0; rdsz=128;
	mhz=100;
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
		JX2R_UseImageAddFile(rd_add[i], NULL);
	for(i=0; i<rd_n_exp; i++)
		JX2R_UseImageAddFile(rd_exp[i], NULL);
	
	ctx=BJX2_AllocContext();
	BJX2_MemDefineROM(ctx,		"ROM",	0x00000000, 0x00007FFF);
	BJX2_MemDefineRAM(ctx,		"SRAM",	0x0000C000, 0x0000DFFF);
//	BJX2_MemDefineMmgp(ctx,		"MMGP",	0x0000E000, 0x0000E3FF);
//	BJX2_MemDefineGfxCon(ctx,	"CGFX",	0x000A0000, 0x000AFFFF);
	BJX2_MemDefineRAM(ctx,		"DRAM",	0x01000000, 0x18000000);

//	BJX2_MemDefineMmgp(ctx,		"MMGP",	0xA000E000, 0xA000E3FF);
//	BJX2_MemDefineSndSblk(ctx,	"SBAU",	0xA0080000, 0xA0081FFF);
//	BJX2_MemDefineGfxCon(ctx,	"CGFX",	0xA00A0000, 0xA00AFFFF);

	BJX2_MemDefineMmgp(ctx,		"MMGP",	0xF000E000, 0xF000E3FF);
	BJX2_MemDefineSndSblk(ctx,	"SBAU",	0xF0080000, 0xF0081FFF);
	BJX2_MemDefineGfxCon(ctx,	"CGFX",	0xF00A0000, 0xF00AFFFF);

#ifndef BJX2_ADDR32
//	BJX2_MemDefineMmgp(ctx,		"MMGP",	0xFFFFFFFFA000E000, 0xFFFFFFFFA000E3FF);
//	BJX2_MemDefineSndSblk(ctx,	"SBAU",	0xFFFFFFFFA0080000, 0xFFFFFFFFA0081FFF);
//	BJX2_MemDefineGfxCon(ctx,	"CGFX",	0xFFFFFFFFA00A0000, 0xFFFFFFFFA00AFFFF);

	BJX2_MemDefineMmgp(ctx,		"MMGP",	0xFFFFFFFFF000E000, 0xFFFFFFFFF000E3FF);
	BJX2_MemDefineSndSblk(ctx,	"SBAU",	0xFFFFFFFFF0080000, 0xFFFFFFFFF0081FFF);
	BJX2_MemDefineGfxCon(ctx,	"CGFX",	0xFFFFFFFFF00A0000, 0xFFFFFFFFF00AFFFF);
#endif
	
	if(ifn)
	{
		BJX2_ContextLoadRom(ctx, ifn);
	}

	for(i=0; i<rd_n_map; i++)
	{
		BJX2_ContextLoadMap(ctx, rd_map[i]);
	}

	ctx->use_jit=0;
	if(mhz>100)
		ctx->use_jit=1;

#if defined(ARM) || defined(ARM64)
	ctx->use_jit=1;
#endif

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

#ifdef __linux
	i = fcntl(0, F_GETFL, 0);
	fcntl(0, F_SETFL, i | O_NONBLOCK);
	btesh2_ttynoncanon();
#endif

	GfxDrv_Start();
	SoundDev_Init();
	JX2I_GfxCon_Startup();

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


//		tsec=tt/((double)CLOCKS_PER_SEC);
		tsec=tt/1000.0;
		
//		BJX2_DbgDump(ctx);
		printf("%.2fs %.2fMIPS %.2fMHz usec=%3.2f%% timer_irq=%4.2fHz\n",
			tsec,
			ctx->tot_ops/(tsec*1000000.0),
			ctx->tot_cyc/(tsec*1000000.0),
			(tvus/10000.0)/tsec,
			ctx->nttick_irq/tsec);
		printf("Exit Status %04X\n", i);
	}
#endif

	fflush(stdout);
}
