#include "VExUnit.h"
#include "verilated.h"

VExUnit *top = new VExUnit;

vluint64_t main_time = 0;



#ifdef __linux
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#endif

#if defined(linux) || defined(__EMSCRIPTEN__)
#include <sys/time.h>
#endif

#ifdef __linux
// #define USE_SDL
#endif

// #define JX2_SKIP_L2_MODEL			//skip trying to model L2 behavior.
// #define JX2_L2_MISS_CYC		30	//skip trying to model L2 behavior.
#define JX2_L2_MISS_CYC		10		//skip trying to model L2 behavior.

#ifdef L2FAST
#define JX2_SKIP_L2_MODEL			//skip trying to model L2 behavior.
#endif

typedef unsigned char byte;
typedef signed char sbyte;

#include "coretst/jx2_tstmulti.c"

#define FKEY_F1		0x81
#define FKEY_F2		0x82
#define FKEY_F3		0x83
#define FKEY_F4		0x84
#define FKEY_F5		0x85
#define FKEY_F6		0x86
#define FKEY_F7		0x87
#define FKEY_F8		0x88
#define FKEY_F9		0x89
#define FKEY_F10	0x8A
#define FKEY_F11	0x8B
#define FKEY_F12	0x8C

#define FKEY_KEYUP	0x90
#define FKEY_KEYDN	0x91
#define FKEY_KEYLF	0x92
#define FKEY_KEYRT	0x93

byte kbbuf[256+8];
byte kbrov;
byte kbirov;
// byte gfxdrv_kill;
byte fkey;

byte fmode;

BJX2_Context	*jx2_ctx;

u32 mmgp_data[256];


#if 0
// #ifdef __linux
static struct termios old_termios;

void btesh2_ttynoncanon(void)
{
	struct termios new_termios;

	tcgetattr(0, &old_termios);
	new_termios=old_termios;
	new_termios.c_lflag&=(~ICANON);
	new_termios.c_lflag&=(~ECHO);
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

int BTSR1_MainAddKey(int key)
{
	kbbuf[kbrov]=key;
	kbrov=(kbrov+1)&255;
}

int BTSR1_MainCheckKey(void)
{
	return(kbirov!=kbrov);
}

int BTSR1_MainGetKey(void)
{
	int i;
	
	if(kbirov==kbrov)
		return(-1);
	
	i=kbbuf[kbirov];
	kbirov=(kbirov+1)&255;
	return(i);
}

byte btsr1_keystate;
int btsr1_keyval;

void BTSR1_ProcessEscKey(int v)
{
	printf("%02X(%c) ", v, v);

	if(btsr1_keystate==0)
	{
//		BTSR1_MoveCursor(0, 19);
//		printf("\n");
		if(v=='[')
			{ btsr1_keystate=1; return; }
		if(v=='O')
			{ btsr1_keystate=2; return; }
		return;
	}

	if(btsr1_keystate==1)
	{
		if((v>='0') && (v<='9'))
		{
			btsr1_keyval=(btsr1_keyval*10)+(v-'0');
			return;
		}
		
		if(v=='~')
		{
			switch(btsr1_keyval)
			{
			case 15:	fkey=FKEY_F5; break;
			case 17:	fkey=FKEY_F6; break;
			case 18:	fkey=FKEY_F7; break;
			case 19:	fkey=FKEY_F8; break;
			case 20:	fkey=FKEY_F9; break;
			case 21:	fkey=FKEY_F10; break;
			case 23:	fkey=FKEY_F11; break;
			case 24:	fkey=FKEY_F12; break;
			}
			btsr1_keystate=0;
			btsr1_keyval=0;
			return;
		}

		if(v=='A')
			{ fkey=FKEY_KEYUP; btsr1_keystate=0; return; }
		if(v=='B')
			{ fkey=FKEY_KEYDN; btsr1_keystate=0; return; }
		if(v=='C')
			{ fkey=FKEY_KEYRT; btsr1_keystate=0; return; }
		if(v=='D')
			{ fkey=FKEY_KEYLF; btsr1_keystate=0; return; }

		if(v=='[')
			{ btsr1_keystate=3; return; }

		btsr1_keystate=0;

		return;
	}

	if(btsr1_keystate==2)
	{
		if(v=='P')
			{ fkey=FKEY_F1; btsr1_keystate=0; return; }
		if(v=='Q')
			{ fkey=FKEY_F2; btsr1_keystate=0; return; }
		if(v=='R')
			{ fkey=FKEY_F3; btsr1_keystate=0; return; }
		if(v=='S')
			{ fkey=FKEY_F4; btsr1_keystate=0; return; }

		btsr1_keystate=0;
		return;
	}

	if(btsr1_keystate==3)
	{
		if(v=='A')
			{ fkey=FKEY_F1; btsr1_keystate=0; return; }
		if(v=='B')
			{ fkey=FKEY_F2; btsr1_keystate=0; return; }
		if(v=='C')
			{ fkey=FKEY_F3; btsr1_keystate=0; return; }
		if(v=='D')
			{ fkey=FKEY_F4; btsr1_keystate=0; return; }
		if(v=='E')
			{ fkey=FKEY_F5; btsr1_keystate=0; return; }

		btsr1_keystate=0;
		return;
	}
}


int BJX2_MainAddScanKeyByte(BJX2_Context *ctx, int k)
{
	printf("KB(%X)", k);
	fflush(stdout);

	ctx->ps2kbbuf[ctx->ps2kbrov]=k;
	ctx->ps2kbrov=(ctx->ps2kbrov+1)&255;
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
	
	sc=ps2_key2scan[key&0xFF];
	if(!sc)
	{
		printf("SKIP(%X)", key);
		fflush(stdout);
		return(0);
	}

	printf("KEY(%X)", key);
	fflush(stdout);

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

#if 0
int BJX2_MainAddKeyByte(BJX2_Context *ctx, int k)
{
	ctx->kbbuf[ctx->kbrov]=k;
	ctx->kbrov=(ctx->kbrov+1)&255;
	ctx->kbirq++;
	return(0);
}
#endif


int BTSR1_MainPollKeyboard(void)
{
	u16 *kb;
	int i, j, k, l;

#ifdef _WIN32
	while(_kbhit())
	{
		j=_getch();
		
		printf("%02X ", j);
		
		if(j==0x00)
		{
			printf("\n");
			printf("%02X ", j);
			
			j=_getch();
			printf("%02X ", j);

			switch(j)
			{
			case 0x3B: fkey=FKEY_F1; break;
			case 0x3C: fkey=FKEY_F2; break;
			case 0x3D: fkey=FKEY_F3; break;
			case 0x3E: fkey=FKEY_F4; break;
			case 0x3F: fkey=FKEY_F5; break;
			case 0x40: fkey=FKEY_F6; break;
			case 0x41: fkey=FKEY_F7; break;
			case 0x42: fkey=FKEY_F8; break;
			case 0x43: fkey=FKEY_F9; break;
			case 0x44: fkey=FKEY_F10; break;
			}

			while(_kbhit())
			{
				j=_getch();
				printf("%02X ", j);
			}

			printf("\n");
		}
		
//		if(j=='`')
//		if((j=='`') || (j=='\x1B'))
		if(j=='\x1B')
		{
//			j=_getch();
//			if(j=='P')
//			{
//				fkey=1;
//				j=_getch();
//				continue;
//			}
			
			gfxdrv_kill=1;
		}

//		if(j=='\r')
//			j=0x0A0D;
		
		BTSR1_MainAddKey(j);
		
		if(j>>8)
		{
			BTSR1_MainAddKey(j);
		}
	}
#endif

#ifdef __linux
	j=fgetc(stdin);
	while(j>0)
	{
//		if(j=='`')
//		if((j=='`') || (j=='\x1B'))
		if(j=='\x1B')
		{
//			BTSR1_MoveCursor(0, 21);

//			printf("%02X ", j);
			j=fgetc(stdin);
			if((j=='\x1B') || (j<=0))
			{
				gfxdrv_kill=1;
				continue;
			}

			while(j>0)
			{
//				printf("%02X(%c) ", j, j);
				BTSR1_ProcessEscKey(j);
				j=fgetc(stdin);
			}

			btsr1_keystate=0;
//			printf("\n");
			continue;

#if 0
			if(j=='%')
			{
				j=fgetc(stdin);

				if(j=='P')
				{
					printf("%02 ", j);
					fkey=1;
					j=fgetc(stdin);
					continue;
				}
				continue;
			}
#endif

			gfxdrv_kill=1;
		}
		
//		kbbuf[kbrov]=j;
//		kbrov=(kbrov+1)&255;
		BTSR1_MainAddKey(j);

		j=fgetc(stdin);
	}
#endif

#if 1
	kb=FRGL_GetKeybuf();
	while(*kb)
	{
		k=*kb++;
		BJX2_MainAddTranslateKey(jx2_ctx, k);
//		BTSR1_MainAddKey(k);

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
#endif
}

int BTSR1_MainInitKeyboard(void)
{
	int i;

#ifdef __linux
	i = fcntl(0, F_GETFL, 0);
	fcntl(0, F_SETFL, i | O_NONBLOCK);
	btesh2_ttynoncanon();
#endif
}

int BTSR1_MainDeinitKeyboard(void)
{
#ifdef __linux
	btesh2_resettermios();
#endif
}

static char tbuf[256];
static byte tbpos;

char *BTSR1_CheckGetString()
{
	int i;
	
	i=BTSR1_MainGetKey();
	while(i>0)
	{
		if((i=='\b') || (i==127))
		{
			if(tbpos>0)
			{
				fputs("\b \b", stdout);
				tbpos--;
			}
			i=BTSR1_MainGetKey();
			continue;
		}

		if(i=='\r')
			break;
		if(i=='\n')
			break;

		tbuf[tbpos++]=i;
		fputc(i, stdout);
		
		i=BTSR1_MainGetKey();
	}
	
	tbuf[tbpos]=0;
	if(i>0)
	{
		fputc('\n', stdout);
		tbpos=0;
		return(tbuf);
	}

	return(NULL);
}

#if 0
int FRGL_TimeMS()
{
#ifdef _WIN32
	static unsigned int init;
	unsigned int t;

	t=timeGetTime();
	if(!init)init=t;

	return((unsigned int)(t-init));
#else

#ifdef __EMSCRIPTEN__
	struct timeval	tp;
	static int      secbase; 

	gettimeofday(&tp, NULL);  
	if(!secbase)secbase=tp.tv_sec;
	return(((tp.tv_sec-secbase)*1000)+tp.tv_usec/1000);
#endif

#ifndef linux
	static int init;
	int t;

	t=clock();
	t*=CLOCKS_PER_SEC/1000.0;
//	t=FRGL_TimeMS();

	if(!init)init=t;

	return((unsigned int)(t-init));
#endif
#ifdef linux
	struct timeval	tp;
	static int      secbase; 

	gettimeofday(&tp, NULL);  
	if(!secbase)secbase=tp.tv_sec;
	return(((tp.tv_sec-secbase)*1000)+tp.tv_usec/1000);
#endif
#endif
}
#endif

static int mmio_spideb=0;
static int mmio_ps2deb=0;

uint32_t mmio_ReadDWord(BJX2_Context *ctx, uint32_t addr)
{
	u32 *mmio;
	s64 rvq, dcyc;
	int val;

//	if((addr&0xF0000)==0xA0000)
//	{
//		return(0);
//	}

	if((addr&0x0FFE0000)==0xA0000)
	{
		val=BJX2_MemGfxConCb_GetDWord(ctx, NULL, addr&0x1FFFF);
		return(val);
	}

	if((addr&0x0FFF0000)==0x90000)
	{
		return(0);
	}

	if((addr&0x0FFF0000)==0x80000)
	{
		return(0);
	}

	mmio=mmgp_data;

	switch(addr&0xFFFFFF)
	{
	case 0xE000:
		rvq=(ctx->tot_cyc*ctx->rcp_mhz)>>16;
		val=rvq;
		break;
	case 0xE004:
		rvq=(ctx->tot_cyc*ctx->rcp_mhz)>>16;
		val=rvq>>32;
		break;

	case 0xE008:
		val=ctx->ttick_hk;
		break;
	case 0xE00C:
		val=ctx->ttick_rst;
		break;

	case 0xE010:
		val=0;
//		if(kbirov!=kbrov)
//		{
//			val=kbbuf[kbirov];
//			kbirov=(kbirov+1)&255;
//		}
		break;

	case 0xE014:
		val=0; break;

	case 0xE018:
		val=0;
//		if(kbirov!=kbrov)
//			val|=1;
		break;

	case 0xE01C:
		val=0; break;

	case 0xE030:
		val=mmio[0x10];
//		if(mmgp_spi_delcyc>0)
//			val|=2;
		break;
	case 0xE034:
		val=mmio[0x11];
		mmio_spideb=0;
		break;
	case 0xE038:
		val=mmio[0x12];
		mmio_spideb=0;
		break;
	case 0xE03C:
		val=mmio[0x13];
		mmio_spideb=0;
		break;


	case 0xE040:
		val=0;
		if(ctx->ps2kbirov!=ctx->ps2kbrov)
		{
			val=ctx->ps2kbbuf[ctx->ps2kbirov];
//			ctx->ps2kbirov=(ctx->ps2kbirov+1)&255;
			mmio_ps2deb=1;
		}
		break;

	case 0xE044:
		val=0; break;

	case 0xE048:
		val=0;
		if(mmio_ps2deb)
		{
			ctx->ps2kbirov=(ctx->ps2kbirov+1)&255;
			mmio_ps2deb=0;
		}
		if(ctx->ps2kbirov!=ctx->ps2kbrov)
			val|=1;
		break;

	case 0xE800:	case 0xE804:
	case 0xE808:	case 0xE80C:
	case 0xE810:	case 0xE814:
	case 0xE818:	case 0xE81C:
	case 0xE820:	case 0xE824:
	case 0xE828:	case 0xE82C:
	case 0xE830:	case 0xE834:
	case 0xE838:	case 0xE83C:
		val=0;
		break;

	default:
//		val=0;
		val=0x55AA55AA;
		break;
	}
	return(val);
}

uint32_t mmio_WriteDWord(BJX2_Context *ctx, uint32_t addr, uint32_t val)
{
	u32 *mmio;
	u64 lv0, lv1;
	int v;
	int i, j, k;

	if((addr&0x0FFE0000)==0xA0000)
	{
		BJX2_MemGfxConCb_SetDWord(ctx, NULL, addr&0x1FFFF, val);
		return(0);
	}

	if((addr&0x0FFF0000)==0x80000)
	{
		return(0);
	}

	if((addr&0x0FFF0000)==0x90000)
	{
		return(0);
	}

	mmio=mmgp_data;

	switch(addr&0xFFFFFF)
	{
	case 0xE000:
		break;
	case 0xE004:
		break;
	case 0xE008:
		ctx->ttick_hk=val;
		break;
	case 0xE00C:
		ctx->ttick_rst=val;
		break;
	case 0xE010:
		break;
	case 0xE014:
//		printf("UART TX %02X(%c)\n", val, val);
		fputc(val, stdout);
		break;
	case 0xE018:
		break;
	case 0xE01C:
		break;

	case 0xE030:
		if(val&0x20)
		{
			if(mmio_spideb)
				break;
		
			lv0=mmio[0x12]|(((u64)mmio[0x13])<<32);

			lv1=0;
			for(i=0; i<8; i++)
			{
//				mmgp_spi_delcyc+=(ctx->tgt_mhz*8)/5;
				v=btesh2_spimmc_XrByte(ctx, (lv0>>(i*8))&255);
				lv1|=((u64)(v&255))<<(i*8);
			}
			mmio[0x12]=lv1;
			mmio[0x13]=lv1>>32;
			mmio_spideb = 1;

//			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_IOPOKE);
		}

		if(val&0x02)
		{
			if(mmio_spideb)
				break;
		
			v=mmio[0x11];
			v=btesh2_spimmc_XrByte(ctx, v);
			mmio[0x11]=v;
		}
	
		v=btesh2_spimmc_XrCtl(ctx, val);

//		printf("SPI XrCtl %02X %02X\n", val, v);

		mmio[0x10]=v&255;
		if(v&0x10000)
			mmio[0x11]=(v>>8)&255;
		break;
	case 0xE034:
//		v=btesh2_spimmc_XrByte(ctx, val);
//		mmio[0x11]=v;
		mmio[0x11]=val;

//		printf("SPI XrData %02X %02X\n", val, v);

#if 0
		if((val!=0xFF) && (v!=0xFF))
			printf("%02X->%02X ", val, v);
		else if((val!=0xFF))
			printf("%02X-> ", val);
		else if((v!=0xFF))
			printf("->%02X ", v);
#endif

//		mmgp_spi_delcyc+=200;
//		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_IOPOKE);
		break;

	case 0xE038:
		mmio[0x12]=val;
		break;
	case 0xE03C:
		mmio[0x13]=val;
		break;

	default:
		break;
	}
	return(0);
}

uint32_t *rombuf;
uint32_t *srambuf;
uint32_t *drambuf;

uint32_t *srambuf2;
uint32_t *drambuf2;

int membus_seed;
int membus_256bit;

int MemBusRand()
{
	membus_seed=(membus_seed*(65521*251))+1;
	return((membus_seed>>24)&255);
}

static int			l2dc_hash_addr[8192];
static int			l2dc_hash_addr2[8192];
static byte		l2dc_hash_dirty[8192];
static byte		l2dc_hash_dirty2[8192];
static byte		l2dc_hash_epoch[8192];
static int			l2dc_miss_cyc;
static int			l2dc_miss_ix;
static int			l2dc_miss_addr;
static int			l2dc_miss_seq;
static int			l2dc_miss_ldb;

static int			l2dc_miss_pogocnt;

static int			l2dc_stat_hit;
static int			l2dc_stat_hit2;
static int			l2dc_stat_miss;

static int			l2dc_evict_addr[8];
static int			l2dc_evict_ix[8];

static int			l2m_seqhash[4096];
static int			l2m_hash_tst[8][8192];
static int			l2m_hash_hit[8];
static int			l2m_hash_miss[8];


void MemUpdateForBusRing()
{
	static uint64_t	rng;
	static uint64_t	l2addr1, l2addr2;
	static uint32_t	l2seq1, l2seq2;
	static uint32_t	l2opm1, l2opm2;
	static uint32_t	l2data1a, l2data2a;
	static uint32_t	l2data1b, l2data2b;
	static uint32_t	l2data1c, l2data2c;
	static uint32_t	l2data1d, l2data2d;

	static uint64_t	l2addr3, l2addr4;
	static uint32_t	l2seq3, l2seq4;
	static uint32_t	l2opm3, l2opm4;
	static uint32_t	l2data3a, l2data4a;
	static uint32_t	l2data3b, l2data4b;
	static uint32_t	l2data3c, l2data4c;
	static uint32_t	l2data3d, l2data4d;

	static	int			lclock;
	int		ix, sx;
	int		i, j, k;

	top->unitNodeId = 0x04;
	
	rng=(rng*0x0000FEDCBA987654ULL)+
		(((l2addr2>>4)+l2data2a)&65535);
	l2opm2^=((rng>>48)&0x8000);

#if 1
	top->memSeqIn=l2seq2;
	top->memOpmIn=l2opm2;
	top->memAddrInA[0]=l2addr2;
	top->memAddrInA[1]=l2addr2>>32;
	top->memAddrInA[2]=0;
	top->memDataIn[0]=l2data2a;
	top->memDataIn[1]=l2data2b;
	top->memDataIn[2]=l2data2c;
	top->memDataIn[3]=l2data2d;
#endif

#if 0
	top->memSeqIn=l2seq4;
	top->memOpmIn=l2opm4;
	top->memAddrInA[0]=l2addr4;
	top->memAddrInA[1]=l2addr4>>32;
	top->memAddrInA[2]=0;
	top->memDataIn[0]=l2data4a;
	top->memDataIn[1]=l2data4b;
	top->memDataIn[2]=l2data4c;
	top->memDataIn[3]=l2data4d;
#endif

	if(top->clock==lclock)
		return;
	lclock=top->clock;
	
	if(top->clock)
	{	
		l2seq4=l2seq3;
		l2opm4=l2opm3;
		l2addr4=l2addr3;
		l2data4a=l2data3a;
		l2data4b=l2data3b;
		l2data4c=l2data3c;
		l2data4d=l2data3d;

		l2seq3=l2seq2;
		l2opm3=l2opm2;
		l2addr3=l2addr2;
		l2data3a=l2data2a;
		l2data3b=l2data2b;
		l2data3c=l2data2c;
		l2data3d=l2data2d;

		l2seq2=l2seq1;
		l2opm2=l2opm1;
		l2addr2=l2addr1;
		l2data2a=l2data1a;
		l2data2b=l2data1b;
		l2data2c=l2data1c;
		l2data2d=l2data1d;

		l2seq1=top->memSeqOut;
		l2opm1=top->memOpm;
		l2addr1=top->memAddrA[0] | (((uint64_t)top->memAddrA[1])<<32);
		l2data1a=top->memDataOut[0];
		l2data1b=top->memDataOut[1];
		l2data1c=top->memDataOut[2];
		l2data1d=top->memDataOut[3];

#if 0
		if(l2opm1)
		{
			printf("L2-1 O=%04X S=%04X A=%08X D=%08X_%08X_%08X_%08X\n",
				l2opm1, l2seq1, l2addr1,
				l2data1d, l2data1c, l2data1b, l2data1a);
		}

		if(l2opm2)
		{
			printf("L2-2 O=%04X S=%04X A=%08X D=%08X_%08X_%08X_%08X\n",
				l2opm2, l2seq2, l2addr2,
				l2data2d, l2data2c, l2data2b, l2data2a);
		}
#endif
	}
	

	int addr, isRom, isSRam, isDRam,
		isZero, isNopPg, isRtsPg,
		isMmio, isCcmd, isSkip;
	int isL2mRepeat, l2Epoch;
	
	const char *src_unit;
	
	src_unit="?";
	switch((l2seq1>>6)&15)
	{
	case 0x0:	case 0x1:
	case 0x2:	case 0x3:
		src_unit="Bridge";
		break;

	case 0x4:	src_unit="L1I$A";	break;
	case 0x5:	src_unit="L1I$B";	break;
	case 0x6:	src_unit="L1I$M";	break;
	case 0x7:	src_unit="L1I$N";	break;

	case 0x8:	src_unit="L1D$A";	break;
	case 0x9:	src_unit="L1D$B";	break;
	case 0xA:	src_unit="L1D$M";	break;
	case 0xB:	src_unit="L1D$N";	break;

	case 0xC:	src_unit="TLB-A";	break;
	case 0xD:	src_unit="TLB-B";	break;
	case 0xE:	src_unit="TLB-M";	break;
	case 0xF:	src_unit="TLB-N";	break;
	}
	
	
	addr	= l2addr1;
	isRom	= (addr>=0x00000000) && (addr<=0x00007FFF);
	isSRam	= (addr>=0x0000C000) && (addr<=0x0000DFFF);
//	isDRam	= (addr>=0x01000000) && (addr<=0x0FFFFFFF);
//	isDRam	= (addr>=0x01000000) && (addr<=0x7FFFFFFF);
	isDRam	= (addr>=0x01000000) && (addr<=0x3FFFFFFF);
	isZero	= (addr>=0x00010000) && (addr<=0x0001FFFF);
	isNopPg	= (addr>=0x00020000) && (addr<=0x0002FFFF);
	isRtsPg	= (addr>=0x00030000) && (addr<=0x0003FFFF);
	
	isMmio = 0;
	if(((l2opm1&0xF0)==0x90) &&
			((l2opm1&0x0F)!=0x07) &&
			((l2opm1&0x0F)!=0x04) )
		isMmio = 1;
	if(((l2opm1&0xF0)==0xA0) &&
			((l2opm1&0x0F)!=0x07) &&
			((l2opm1&0x0F)!=0x04))
		isMmio = 2;
	
	isCcmd = 0;
	if((l2opm1&0xF0)==0x80)
		isCcmd = 1;
	
	isSkip = 0;
	isL2mRepeat = 1;
//	l2Epoch = (main_time>>9)&15;
//	l2Epoch = (main_time>>13)&15;
	l2Epoch = (main_time>>15)&15;

#ifndef JX2_SKIP_L2_MODEL
//	sx=((l2seq1*65521)>>16)&4095;
	sx=((l2seq1*65521)>>16)&7;
	if((l2opm1&255) && (l2m_seqhash[sx]!=l2seq1))
//	if(l2opm1&255)
	{
		l2m_seqhash[sx]=l2seq1;
		isL2mRepeat = 0;

		for(i=0; i<8; i++)
		{
			switch(i)
			{
			case 0:
				ix=addr>>4;
				break;
			case 1:	
				ix=(addr>>4);
				ix=ix^(ix>>12);
				break;
			case 2:	
				ix=(addr>>4);
				ix=ix^(ix>>12)^((ix>>15)&0xFF8);
				break;
			case 3:	
				ix=(addr>>4);
				ix=ix^(ix>>13);
				break;
			case 4:	
				ix=(addr>>4);
				ix=ix^(ix>>13)^(ix>>7);
				break;
			case 5:	
				ix=(addr>>4);
				ix=ix^(ix>>12)^(ix>>16);
				break;
			default:
				ix=addr>>4;
				break;
			}
		
			ix=ix&8191;
			if(l2m_hash_tst[i][ix]==addr)
				l2m_hash_hit[i]++;
			else
				l2m_hash_miss[i]++;
			l2m_hash_tst[i][ix]=addr;
		}
	}
	
//	static int			l2dc_hash[8192];
//	static int			l2dc_miss_cyc;
//	static int			l2dc_miss_ix;
//	static int			l2dc_miss_addr;

//	if(isDRam && ((l2opm1&0xFF)!=0) &&
//	if(isDRam && ((l2opm1&0xFF)!=0) &&
//		(((l2opm1&0x0F)==7) || ((l2opm1&0x0F)==4)))
	if(isDRam && (
		((l2opm1&0xFF)==0x97) ||
		((l2opm1&0xFF)==0xA7) ||
		((l2opm1&0xFF)==0x94) )	)
	{
		if(l2dc_miss_cyc>0)
		{
			l2dc_miss_cyc--;
			if(l2dc_miss_cyc<=0)
			{
				for(i=7; i>0; i--)
				{
					l2dc_evict_addr[i]=l2dc_evict_addr[i-1];
					l2dc_evict_ix[i]=l2dc_evict_ix[i-1];
				}
				l2dc_evict_addr[0] = l2dc_hash_addr[l2dc_miss_ix];
				l2dc_evict_ix[0] = l2dc_miss_ix;

//				l2dc_hash_addr2[l2dc_miss_ix] = l2dc_hash_addr[l2dc_miss_ix];
//				l2dc_hash_dirty2[l2dc_miss_ix] = l2dc_hash_dirty[l2dc_miss_ix];
//				l2dc_hash_addr[l2dc_miss_ix]=l2dc_miss_addr;
//				l2dc_hash_dirty[l2dc_miss_ix]=0;

				if(l2dc_miss_ldb)
				{
					l2dc_hash_addr2[l2dc_miss_ix]=l2dc_miss_addr;
					l2dc_hash_dirty2[l2dc_miss_ix]=0;
				}else
				{
					l2dc_hash_addr[l2dc_miss_ix]=l2dc_miss_addr;
					l2dc_hash_dirty[l2dc_miss_ix]=0;
					l2dc_hash_epoch[l2dc_miss_ix]=l2Epoch;
				}
			}

//			isSkip = 1;
		}

		ix=(addr>>4);
//		ix=ix^(ix>>13)^(ix>>7);
		ix=ix^(ix>>12);

//		ix=(addr>>4)^(addr>>16)^(addr>>20);

		ix=ix&8191;
		
		if(!isL2mRepeat)
		{
			if(l2dc_hash_addr[ix]==addr)
				l2dc_stat_hit++;
			if((l2dc_hash_addr2[ix]==addr) &&
					((l2opm1&0xFF)!=0xA7) && ((l2opm1&0xFF)!=0xA4))
				l2dc_stat_hit2++;
		}
		
		if((l2dc_hash_addr2[ix]==addr) &&
			(	((l2opm1&0xFF)==0xA7) ||
				((l2opm1&0xFF)==0xA4)	))
		{
//			l2dc_hash_dirty2[ix]=1;
		}else
		{
			i=l2dc_hash_dirty[ix];
			if(!i && ((l2opm1&0xFF)==0xA7))
			{
//				l2dc_hash_addr2[ix]=l2dc_hash_addr[ix];
				l2dc_hash_addr[ix]=addr;
				l2dc_hash_dirty[ix]=1;
				l2dc_hash_epoch[ix]=l2Epoch;
			}
		}
		
//		if(l2dc_hash_addr[ix]!=addr)
		if((l2dc_hash_addr[ix]!=addr) &&
			((l2dc_hash_addr2[ix]!=addr) ||
				((l2opm1&0xFF)==0xA7) ||
				((l2opm1&0xFF)==0xA4)))
//		if(0)
		{
//			i=l2dc_hash_dirty[ix];
//			for(j=0; j<7; j++)
//				if(addr==l2dc_evict_addr[j])
//					break;
		
//			if(!i && (addr==l2dc_evict_addr[j]))
			if(0)
			{
				l2dc_hash_addr[ix]=l2dc_evict_addr[j];
				l2dc_hash_dirty[ix]=0;
			}else
			{
				if(!isL2mRepeat)
					{ l2dc_stat_miss++; }

				isSkip = 1;
	//			if(l2dc_miss_cyc<=0)
	//			if((l2dc_miss_cyc<=0) && (l2dc_miss_seq<=0))
				if((l2dc_miss_cyc<=0) &&
					((l2dc_miss_seq<=0) || (l2dc_miss_ix!=ix)))
				{
					l2dc_miss_ix=ix;
					l2dc_miss_addr=addr;
//					l2dc_miss_cyc=30;
					l2dc_miss_cyc=JX2_L2_MISS_CYC;
					l2dc_miss_seq=l2seq1;

					l2dc_miss_ldb=0;
					if((l2opm1&0xFF)!=0xA7)
					{
//						if(l2dc_hash_dirty[ix])
						if(l2dc_hash_dirty[ix] || (addr&16))
							l2dc_miss_ldb=1;
					}
					
					if(l2dc_hash_dirty[ix] && ((l2Epoch-l2dc_hash_epoch[ix])&8))
						l2dc_miss_ldb=0;
					
				}else
				{
					if((l2dc_miss_cyc<=0) && (l2dc_miss_seq>0))
					{
						printf("L2: POGO\n");
						l2dc_miss_pogocnt++;
						if(l2dc_miss_pogocnt>4096)
						{
							printf("L2: POGO RESET\n");
							l2dc_miss_pogocnt=0;
							l2dc_miss_seq=0;
						}
					}
				}
			}
		}else
		{
			if((l2opm1&0xFF)==0xA7)
			{
				l2dc_hash_dirty[ix]=1;
				l2dc_hash_epoch[ix]=l2Epoch;
			}
		}
		
		if(!isSkip)
		{
			if(l2seq1==l2dc_miss_seq)
				l2dc_miss_seq=0;
		}
	}

	if((l2opm1&0xFF)==0x94)
		isSkip = 0;
	if((l2opm1&0xFF)==0xA4)
		isSkip = 0;

#endif
	
	if(isMmio==1)
	{
		if((l2opm1&0x07)==0x03)
		{
			l2data1a=mmio_ReadDWord(jx2_ctx, (addr+0)&0xFFFFFC);
			l2data1b=mmio_ReadDWord(jx2_ctx, (addr+4)&0xFFFFFC);
		}else
		{
			l2data1a=mmio_ReadDWord(jx2_ctx, addr&0xFFFFFC);
			l2data1b=0;
		}
		l2data1c=0;
		l2data1d=0;
		l2opm1=0x70;
	}
	
	if(isMmio==2)
	{
		if((l2opm1&0x07)==0x03)
		{
			mmio_WriteDWord(jx2_ctx,
				(addr+0)&0xFFFFFF,
				l2data1a);
			mmio_WriteDWord(jx2_ctx,
				(addr+4)&0xFFFFFF,
				l2data1b);
		}else
		{
			mmio_WriteDWord(jx2_ctx, addr&0xFFFFFF,
				l2data1a);
		}
		l2data1c=0;
		l2data1d=0;
		l2opm1=0x70;
	}

	if(isCcmd)
	{
		l2data1c=0;
		l2data1d=0;
		l2opm1=0x70;
	}
	
	if((((l2opm1&0xFF)==0x97) || ((l2opm1&0xFF)==0x94)) && !isSkip)
//	if((((l2opm1&0xFF)==0x97) ||
//		((l2opm1&0xFF)==0x94) ||
//		((l2opm1&0xFF)==0xA4)) && !isSkip)
	{
		if(addr&15)
		{
			printf("Bad Line Address\n");
		}
	
//		main_buslines++;
		if(isRom)
		{
			l2data1a=rombuf[((addr>>2)+0)&0x1FFF];
			l2data1b=rombuf[((addr>>2)+1)&0x1FFF];
			l2data1c=rombuf[((addr>>2)+2)&0x1FFF];
			l2data1d=rombuf[((addr>>2)+3)&0x1FFF];
//			l2opm1=0x70;
			l2opm1=0x70 | ((l2opm1>>8)&0x0F);
		}else
			if(isSRam)
		{
			l2data1a=srambuf[((addr>>2)+0)&0x7FF];
			l2data1b=srambuf[((addr>>2)+1)&0x7FF];
			l2data1c=srambuf[((addr>>2)+2)&0x7FF];
			l2data1d=srambuf[((addr>>2)+3)&0x7FF];
//			l2opm1=0x70;
			l2opm1=0x70 | ((l2opm1>>8)&0x0F);
		}else
			if(isZero)
		{
			l2data1a=0;
			l2data1b=0;
			l2data1c=0;
			l2data1d=0;
//			l2opm1=0x70;
			l2opm1=0x70 | ((l2opm1>>8)&0x0F);
		}else
			if(isNopPg)
		{
			l2data1a=0x30003000;
			l2data1b=0x30003000;
			l2data1c=0x30003000;
			l2data1d=0x30003000;
//			l2opm1=0x70;
			l2opm1=0x70 | ((l2opm1>>8)&0x0F);
		}else
			if(isRtsPg)
		{
			l2data1a=0x30103010;
			l2data1b=0x30103010;
			l2data1c=0x30103010;
			l2data1d=0x30103010;
//			l2opm1=0x70;
			l2opm1=0x70 | ((l2opm1>>8)&0x0F);
		}else
			if(isDRam)
		{
			l2data1a=drambuf[((addr>>2)+0)&0x1FFFFFF];
			l2data1b=drambuf[((addr>>2)+1)&0x1FFFFFF];
			l2data1c=drambuf[((addr>>2)+2)&0x1FFFFFF];
			l2data1d=drambuf[((addr>>2)+3)&0x1FFFFFF];
//			l2opm1=0x70;
			l2opm1=0x70 | ((l2opm1>>8)&0x0F);
		}else
		{
			printf("Bad Address %08X %s\n", addr, src_unit);
			printf("L2-1 O=%04X S=%04X A=%08X D=%08X_%08X_%08X_%08X\n",
				l2opm1, l2seq1, l2addr1,
				l2data1d, l2data1c, l2data1b, l2data1a);
			l2data1a=0;
			l2data1b=0;
			l2data1c=0;
			l2data1d=0;
//			l2opm1=0x70;
			l2opm1=0x70 | ((l2opm1>>8)&0x0F);
		}
	}

	if(((l2opm1&0xFF)==0xA4) && !isSkip)
	{
		l2opm1=0x70 | ((l2opm1>>8)&0x0F);
		l2data1a=0x9FA49FA4;
		l2data1b=0x9FA49FA4;
		l2data1c=0x9FA49FA4;
		l2data1d=0x9FA49FA4;
	}

	if(((l2opm1&0xFF)==0xA7) && !isSkip)
	{
		if(addr&15)
		{
			printf("Bad Line Address\n");
		}
	
//		main_buslines++;

		if(isRom || isZero || isNopPg || isRtsPg || ((l2opm1&0xFF)==0xA4))
		{
			l2opm1=0x60;
		}else
			if(isSRam)
		{
			srambuf[((addr>>2)+0)&0x7FF]=l2data1a;
			srambuf[((addr>>2)+1)&0x7FF]=l2data1b;
			srambuf[((addr>>2)+2)&0x7FF]=l2data1c;
			srambuf[((addr>>2)+3)&0x7FF]=l2data1d;
			l2opm1=0x60;
		}
		else if(isDRam)
		{
			drambuf[((addr>>2)+0)&0x1FFFFFF]=l2data1a;
			drambuf[((addr>>2)+1)&0x1FFFFFF]=l2data1b;
			drambuf[((addr>>2)+2)&0x1FFFFFF]=l2data1c;
			drambuf[((addr>>2)+3)&0x1FFFFFF]=l2data1d;
			l2opm1=0x60;
		}else
		{
			printf("Bad Address %08X %s\n", addr, src_unit);
			printf("L2-1 O=%04X S=%04X A=%08X D=%08X_%08X_%08X_%08X\n",
				l2opm1, l2seq1, l2addr1,
				l2data1d, l2data1c, l2data1b, l2data1a);
			l2opm1=0x60;
		}
	}
}

void MemUpdateForBus()
{
	static byte mmio_latched=0;
	static int opm_rand;
	byte is_rom, is_sram, is_dram, is_mmio, is_zero;
	byte is_sram_b, is_dram_b;
	int opm_latch, swapfault;
	uint32_t addr;
	uint32_t addrb;

	if(top->memSeqOut!=0xFF00)
	{
		MemUpdateForBusRing();
		return;
	}

	if(!top->clock)
		return;

	if(top->memOpm)
	{
		addr=top->memAddrA[0];
		addrb=top->memAddrB[0];
		top->memOK=0;
		
		is_rom	= (addr>=0x00000000) && (addr<=0x00007FFF);
		is_sram	= (addr>=0x0000C000) && (addr<=0x0000DFFF);
		is_dram	= (addr>=0x01000000) && (addr<=0x18000000);
//		is_mmio = (addr>=0xA0000000) && (addr<=0xC0000000);
		is_mmio = (addr>=0xF0000000) && (addr<=0xFFFFFFFF);
		
		is_zero = (addr>=0x00010000) && (addr<=0x0001FFFF);

		is_sram_b	= (addrb>=0x0000C000) && (addrb<=0x0000DFFF);
		is_dram_b	= (addrb>=0x01000000) && (addrb<=0x18000000);
		
		if(	(top->memDataOut[0]==0x55BAADAA) ||
			(top->memDataOut[1]==0x55BAADAA) ||
			(top->memDataOut[2]==0x55BAADAA) ||
			(top->memDataOut[3]==0x55BAADAA))
		{
			printf("MemUpdateForBus: Bad Pattern Seen\n");
		}

		if((top->memOpm&0x18)==0x18)
		{
			while(!opm_rand)
				opm_rand=MemBusRand();

			swapfault=0;
//			if((opm_rand&3)==0)
			if((opm_rand&15)==0)
				swapfault=1;
			
			if(!is_dram || !is_dram_b)
				swapfault=1;
			
			if(swapfault)
			{
				top->memOK=3;
			}
			else if(is_sram_b)
			{
				srambuf[((addrb>>2)+0)&0x7FF]=top->memDataOut[0];
				srambuf[((addrb>>2)+1)&0x7FF]=top->memDataOut[1];
				srambuf[((addrb>>2)+2)&0x7FF]=top->memDataOut[2];
				srambuf[((addrb>>2)+3)&0x7FF]=top->memDataOut[3];
				if(membus_256bit)
				{
					srambuf[((addrb>>2)+4)&0x7FF]=top->memDataOut[4];
					srambuf[((addrb>>2)+5)&0x7FF]=top->memDataOut[5];
					srambuf[((addrb>>2)+6)&0x7FF]=top->memDataOut[6];
					srambuf[((addrb>>2)+7)&0x7FF]=top->memDataOut[7];
				}
				top->memOK=1;
			}else if(is_dram_b)
			{
				drambuf[((addrb>>2)+0)&0x1FFFFFF]=top->memDataOut[0];
				drambuf[((addrb>>2)+1)&0x1FFFFFF]=top->memDataOut[1];
				drambuf[((addrb>>2)+2)&0x1FFFFFF]=top->memDataOut[2];
				drambuf[((addrb>>2)+3)&0x1FFFFFF]=top->memDataOut[3];
				if(membus_256bit)
				{
					drambuf[((addrb>>2)+4)&0x1FFFFFF]=top->memDataOut[4];
					drambuf[((addrb>>2)+5)&0x1FFFFFF]=top->memDataOut[5];
					drambuf[((addrb>>2)+6)&0x1FFFFFF]=top->memDataOut[6];
					drambuf[((addrb>>2)+7)&0x1FFFFFF]=top->memDataOut[7];
				}
				top->memOK=1;
			}

			if(swapfault)
			{
				top->memDataIn[0]=0;
				top->memDataIn[1]=0;
				top->memDataIn[2]=0;
				top->memDataIn[3]=0;
				if(membus_256bit)
				{
					top->memDataIn[4]=0;
					top->memDataIn[5]=0;
					top->memDataIn[6]=0;
					top->memDataIn[7]=0;
				}
				top->memOK=3;
			}
			else if(is_rom)
			{
				top->memDataIn[0]=rombuf[((addr>>2)+0)&0x1FFF];
				top->memDataIn[1]=rombuf[((addr>>2)+1)&0x1FFF];
				top->memDataIn[2]=rombuf[((addr>>2)+2)&0x1FFF];
				top->memDataIn[3]=rombuf[((addr>>2)+3)&0x1FFF];
				if(membus_256bit)
				{
					top->memDataIn[4]=rombuf[((addr>>2)+4)&0x1FFF];
					top->memDataIn[5]=rombuf[((addr>>2)+5)&0x1FFF];
					top->memDataIn[6]=rombuf[((addr>>2)+6)&0x1FFF];
					top->memDataIn[7]=rombuf[((addr>>2)+7)&0x1FFF];
				}
				top->memOK=1;
			}else
				if(is_sram)
			{
				top->memDataIn[0]=srambuf[((addr>>2)+0)&0x7FF];
				top->memDataIn[1]=srambuf[((addr>>2)+1)&0x7FF];
				top->memDataIn[2]=srambuf[((addr>>2)+2)&0x7FF];
				top->memDataIn[3]=srambuf[((addr>>2)+3)&0x7FF];
				if(membus_256bit)
				{
					top->memDataIn[4]=srambuf[((addr>>2)+4)&0x7FF];
					top->memDataIn[5]=srambuf[((addr>>2)+5)&0x7FF];
					top->memDataIn[6]=srambuf[((addr>>2)+6)&0x7FF];
					top->memDataIn[7]=srambuf[((addr>>2)+7)&0x7FF];
				}
				top->memOK=1;
			}else if(is_dram)
			{
				top->memDataIn[0]=drambuf[((addr>>2)+0)&0x1FFFFFF];
				top->memDataIn[1]=drambuf[((addr>>2)+1)&0x1FFFFFF];
				top->memDataIn[2]=drambuf[((addr>>2)+2)&0x1FFFFFF];
				top->memDataIn[3]=drambuf[((addr>>2)+3)&0x1FFFFFF];
				if(membus_256bit)
				{
					top->memDataIn[4]=drambuf[((addr>>2)+4)&0x1FFFFFF];
					top->memDataIn[5]=drambuf[((addr>>2)+5)&0x1FFFFFF];
					top->memDataIn[6]=drambuf[((addr>>2)+6)&0x1FFFFFF];
					top->memDataIn[7]=drambuf[((addr>>2)+7)&0x1FFFFFF];
				}
				top->memOK=1;

				opm_rand=-1;
			}else if(is_zero)
			{
				top->memDataIn[0]=0;
				top->memDataIn[1]=0;
				top->memDataIn[2]=0;
				top->memDataIn[3]=0;
				if(membus_256bit)
				{
					top->memDataIn[4]=0;
					top->memDataIn[5]=0;
					top->memDataIn[6]=0;
					top->memDataIn[7]=0;
				}
				top->memOK=1;
			}else
			{
				printf("Sim Bad Addr %08X\n", addr);
				top->memDataIn[0]=0xFFFFFFFFU;
				top->memDataIn[1]=0xFFFFFFFFU;
				top->memDataIn[2]=0xFFFFFFFFU;
				top->memDataIn[3]=0xFFFFFFFFU;
				if(membus_256bit)
				{
					top->memDataIn[4]=0xFFFFFFFFU;
					top->memDataIn[5]=0xFFFFFFFFU;
					top->memDataIn[6]=0xFFFFFFFFU;
					top->memDataIn[7]=0xFFFFFFFFU;
				}
//				top->memOK=1;
				top->memOK=3;
			}
		}else
			if(top->memOpm&0x08)
		{
			opm_rand=0;

			if(is_mmio)
			{
				if((top->memOpm&0x07)==0x03)
				{
					top->memDataIn[0]=
						mmio_ReadDWord(jx2_ctx, (addr+0)&0xFFFFFC);
					top->memDataIn[1]=
						mmio_ReadDWord(jx2_ctx, (addr+4)&0xFFFFFC);
				}else
				{
					top->memDataIn[0]=
						mmio_ReadDWord(jx2_ctx, addr&0xFFFFFC);
					top->memDataIn[1]=0;
				}
				top->memOK=1;
				
				if((top->memDataIn[0]==0x55AA55AA) || (addr&3))
				{
					printf("Sim Bad Addr %08X\n", addr);
					top->memDataIn[0]=0xFFFFFFFFU;
					top->memDataIn[1]=0xFFFFFFFFU;
					top->memDataIn[2]=0xFFFFFFFFU;
					top->memDataIn[3]=0xFFFFFFFFU;
					if(membus_256bit)
					{
						top->memDataIn[4]=0xFFFFFFFFU;
						top->memDataIn[5]=0xFFFFFFFFU;
						top->memDataIn[6]=0xFFFFFFFFU;
						top->memDataIn[7]=0xFFFFFFFFU;
					}
					top->memOK=3;
				}
			}else if(is_rom)
			{
				top->memDataIn[0]=rombuf[((addr>>2)+0)&0x1FFF];
				top->memDataIn[1]=rombuf[((addr>>2)+1)&0x1FFF];
				top->memDataIn[2]=rombuf[((addr>>2)+2)&0x1FFF];
				top->memDataIn[3]=rombuf[((addr>>2)+3)&0x1FFF];
				if(membus_256bit)
				{
					top->memDataIn[4]=rombuf[((addr>>2)+4)&0x1FFF];
					top->memDataIn[5]=rombuf[((addr>>2)+5)&0x1FFF];
					top->memDataIn[6]=rombuf[((addr>>2)+6)&0x1FFF];
					top->memDataIn[7]=rombuf[((addr>>2)+7)&0x1FFF];
				}
				top->memOK=1;
			}else
				if(is_sram)
			{
				top->memDataIn[0]=srambuf[((addr>>2)+0)&0x7FF];
				top->memDataIn[1]=srambuf[((addr>>2)+1)&0x7FF];
				top->memDataIn[2]=srambuf[((addr>>2)+2)&0x7FF];
				top->memDataIn[3]=srambuf[((addr>>2)+3)&0x7FF];
				if(membus_256bit)
				{
					top->memDataIn[4]=srambuf[((addr>>2)+4)&0x7FF];
					top->memDataIn[5]=srambuf[((addr>>2)+5)&0x7FF];
					top->memDataIn[6]=srambuf[((addr>>2)+6)&0x7FF];
					top->memDataIn[7]=srambuf[((addr>>2)+7)&0x7FF];
				}
				top->memOK=1;
			}else if(is_dram)
			{
				top->memDataIn[0]=drambuf[((addr>>2)+0)&0x1FFFFFF];
				top->memDataIn[1]=drambuf[((addr>>2)+1)&0x1FFFFFF];
				top->memDataIn[2]=drambuf[((addr>>2)+2)&0x1FFFFFF];
				top->memDataIn[3]=drambuf[((addr>>2)+3)&0x1FFFFFF];
				if(membus_256bit)
				{
					top->memDataIn[4]=drambuf[((addr>>2)+4)&0x1FFFFFF];
					top->memDataIn[5]=drambuf[((addr>>2)+5)&0x1FFFFFF];
					top->memDataIn[6]=drambuf[((addr>>2)+6)&0x1FFFFFF];
					top->memDataIn[7]=drambuf[((addr>>2)+7)&0x1FFFFFF];
				}
				top->memOK=1;
			}else if(is_zero)
			{
				top->memDataIn[0]=0;
				top->memDataIn[1]=0;
				top->memDataIn[2]=0;
				top->memDataIn[3]=0;
				if(membus_256bit)
				{
					top->memDataIn[4]=0;
					top->memDataIn[5]=0;
					top->memDataIn[6]=0;
					top->memDataIn[7]=0;
				}
				top->memOK=1;
			}else
			{
				printf("Sim Bad Addr %08X\n", addr);
				top->memDataIn[0]=0xFFFFFFFFU;
				top->memDataIn[1]=0xFFFFFFFFU;
				top->memDataIn[2]=0xFFFFFFFFU;
				top->memDataIn[3]=0xFFFFFFFFU;
				if(membus_256bit)
				{
					top->memDataIn[4]=0xFFFFFFFFU;
					top->memDataIn[5]=0xFFFFFFFFU;
					top->memDataIn[6]=0xFFFFFFFFU;
					top->memDataIn[7]=0xFFFFFFFFU;
				}
//				top->memOK=1;
				top->memOK=3;
			}

#if 0
			printf("LD  %08X  %08X %08X %08X %08X\n",
				addr,
				top->memDataIn[0], top->memDataIn[1],
				top->memDataIn[2], top->memDataIn[3]);
#endif
		}else
		if(top->memOpm&0x10)
		{
			opm_rand=0;

			top->memDataIn[0]=0xFFFFFFFFU;
			top->memDataIn[1]=0xFFFFFFFFU;
			top->memDataIn[2]=0xFFFFFFFFU;
			top->memDataIn[3]=0xFFFFFFFFU;
			if(membus_256bit)
			{
				top->memDataIn[4]=0xFFFFFFFFU;
				top->memDataIn[5]=0xFFFFFFFFU;
				top->memDataIn[6]=0xFFFFFFFFU;
				top->memDataIn[7]=0xFFFFFFFFU;
			}

			if(is_mmio)
			{
				if(!mmio_latched)
				{
					if((top->memOpm&0x07)==0x03)
					{
						mmio_WriteDWord(jx2_ctx,
							(addr+0)&0xFFFFFF,
							top->memDataOut[0]);
						mmio_WriteDWord(jx2_ctx,
							(addr+4)&0xFFFFFF,
							top->memDataOut[1]);
					}else
					{
						mmio_WriteDWord(jx2_ctx, addr&0xFFFFFF,
							top->memDataOut[0]);
					}
				}
				top->memOK=1;
				mmio_latched=1;
			}else
				if(is_rom)
			{
//					top->memOK=2;
				top->memOK=1;
			}else
				if(is_sram)
			{
				srambuf[((addr>>2)+0)&0x7FF]=top->memDataOut[0];
				srambuf[((addr>>2)+1)&0x7FF]=top->memDataOut[1];
				srambuf[((addr>>2)+2)&0x7FF]=top->memDataOut[2];
				srambuf[((addr>>2)+3)&0x7FF]=top->memDataOut[3];
				if(membus_256bit)
				{
					srambuf[((addr>>2)+4)&0x7FF]=top->memDataOut[4];
					srambuf[((addr>>2)+5)&0x7FF]=top->memDataOut[5];
					srambuf[((addr>>2)+6)&0x7FF]=top->memDataOut[6];
					srambuf[((addr>>2)+7)&0x7FF]=top->memDataOut[7];
				}
				top->memOK=1;
			}else if(is_dram)
			{
				drambuf[((addr>>2)+0)&0x1FFFFFF]=top->memDataOut[0];
				drambuf[((addr>>2)+1)&0x1FFFFFF]=top->memDataOut[1];
				drambuf[((addr>>2)+2)&0x1FFFFFF]=top->memDataOut[2];
				drambuf[((addr>>2)+3)&0x1FFFFFF]=top->memDataOut[3];
				if(membus_256bit)
				{
					drambuf[((addr>>2)+4)&0x1FFFFFF]=top->memDataOut[4];
					drambuf[((addr>>2)+5)&0x1FFFFFF]=top->memDataOut[5];
					drambuf[((addr>>2)+6)&0x1FFFFFF]=top->memDataOut[6];
					drambuf[((addr>>2)+7)&0x1FFFFFF]=top->memDataOut[7];
				}
				top->memOK=1;
			}else if(is_zero)
			{
				top->memOK=1;
			}else
			{
				printf("Sim Bad Addr %08X\n", addr);
				top->memOK=3;
			}

#if 0
			printf("ST  %08X  %08X %08X %08X %08X\n",
				addr,
				top->memDataOut[0], top->memDataOut[1],
				top->memDataOut[2], top->memDataOut[3]);
#endif
		}else
			if(top->memOpm!=0)
		{
			top->memOK=1;
		}
	}else
	{
		top->memOK=0;
		mmio_latched=0;
		if(opm_rand<0)
			opm_rand=0;
	}
}

uint32_t MemGetDWord(int addr)
{
	byte is_rom, is_sram, is_dram, is_mmio;
	int opm_latch;
	uint32_t addr1;
	uint32_t lo, hi, val;

	if(addr&3)
	{
		addr1=addr&(~3);
		lo=MemGetDWord(addr1+0);
		hi=MemGetDWord(addr1+4);
		val=(lo>>((addr&3)*8)) | (hi<<(32-((addr&3)*8)));
		return(val);
	}

	is_rom	= (addr>=0x00000000) && (addr<=0x00007FFF);
	is_sram	= (addr>=0x0000C000) && (addr<=0x0000DFFF);
	is_dram	= (addr>=0x01000000) && (addr<=0x18000000);
	is_mmio = (addr>=0xF0000000) && (addr<=0xFFFFFFFF);

	if(is_rom)
	{
		val=rombuf[(addr>>2)&0x1FFF];
		return(val);
	}

	if(is_sram)
	{
//		val=srambuf[(addr>>2)&0x7FF];
		val=srambuf2[(addr>>2)&0x7FF];
		return(val);
	}

	if(is_dram)
	{
//		val=drambuf[(addr>>2)&0x3FFFFFF];
		val=drambuf2[(addr>>2)&0x1FFFFFF];
		return(val);
	}

	if(is_mmio)
	{
		val=mmio_ReadDWord(jx2_ctx, addr&0x0FFFFFFC);
		return(val);
	}

	return(0x55AA55AA);

}

uint64_t MemGetQWord(int addr)
{
	uint32_t lo, hi;
	uint64_t val;

	lo=MemGetDWord(addr+0);
	hi=MemGetDWord(addr+4);
	val=((uint64_t)lo) | (((uint64_t)hi)<<32);
	return(val);
}

int MemGetWord(int addr)
{
	return((int16_t)MemGetDWord(addr));
}

int MemGetByte(int addr)
{
	return((int8_t)MemGetDWord(addr));
}

int MemSetDWord(int addr, uint32_t val)
{
	byte is_rom, is_sram, is_dram, is_mmio;
	int opm_latch;
	uint32_t addr1;
	uint32_t lo, hi;

	if(addr&3)
	{
		addr1=addr&(~3);
		lo=MemGetDWord(addr1+0);
		hi=MemGetDWord(addr1+4);
		
		switch(addr&3)
		{
		case 1:
			lo=(lo&0x000000FF)|(val<< 8);
			hi=(hi&0xFFFFFF00)|(val>>24);
			break;
		case 2:
			lo=(lo&0x0000FFFF)|(val<<16);
			hi=(hi&0xFFFF0000)|(val>>16);
			break;
		case 3:
			lo=(lo&0x00FFFFFF)|(val<<24);
			hi=(hi&0xFF000000)|(val>> 8);
			break;
		}

		MemSetDWord(addr1+0, lo);
		MemSetDWord(addr1+4, hi);
		return(1);
	}

	is_rom	= (addr>=0x00000000) && (addr<=0x00007FFF);
	is_sram	= (addr>=0x0000C000) && (addr<=0x0000DFFF);
	is_dram	= (addr>=0x01000000) && (addr<=0x18000000);

	if(is_rom)
	{
		return(-1);
	}

	if(is_sram)
	{
		srambuf2[(addr>>2)&0x7FF]=val;
		return(1);
	}

	if(is_dram)
	{
		drambuf2[(addr>>2)&0x1FFFFFF]=val;
		return(1);
	}

	return(-1);
}

int MemSetQWord(int addr, uint64_t val)
{
	int i;
	i=MemSetDWord(addr+0, val);
	MemSetDWord(addr+4, val>>32);
	return(i);
}

int MemSetWord(int addr, int val)
{
	uint32_t lo;
	int i;
	lo=MemGetDWord(addr);
	lo=(lo&0xFFFF0000)|(val&0x0000FFFF);
	i=MemSetDWord(addr, lo);
	return(i);
}

int MemSetByte(int addr, int val)
{
	uint32_t lo;
	int i;
	lo=MemGetDWord(addr);
	lo=(lo&0xFFFFFF00)|(val&0x000000FF);
	i=MemSetDWord(addr, lo);
	return(i);
}

#if 0
int CheckDebugSanityIS()
{
	uint32_t dpc;
	uint64_t distr, distr2;

	dpc=top->dbgOutPc;
//	distr=top->dbgOutIstr;
	distr=((uint64_t)top->dbgOutIstr[0]) |
		(((uint64_t)top->dbgOutIstr[1])<<32);
	
//	if(!dpc)
	if(top->dbgExHold1)
		return(1);
	
	distr2=MemGetQWord(dpc);
	
	if(distr!=distr2)
	{
//		printf("IC Read Mismatch PC=%08X Got %016llX Expect %016llX\n",
//			dpc,
//			distr, distr2);

		printf("IC Read Mismatch PC=%08X Got "
				"%04X_%04X_%04X_%04X Expect %04X_%04X_%04X_%04X\n",
			dpc,
			(distr >> 0)&0xFFFF,	(distr >>16)&0xFFFF,
			(distr >>32)&0xFFFF,	(distr >>48)&0xFFFF,
			(distr2>> 0)&0xFFFF,	(distr2>>16)&0xFFFF,
			(distr2>>32)&0xFFFF,	(distr2>>48)&0xFFFF
			);
		return(0);
	}
	
	return(1);
}
#endif

int MemSimUpdateCache(
	uint32_t addr, uint64_t val1, uint64_t val2,
	int opm, int dcok)
{
	static int laddr1, laddr2;
	static int lopm1, lopm2;
	uint64_t val3;

	laddr2=laddr1;
	laddr1=addr;
	
	lopm2=lopm1;
	lopm1=opm;

#if 0
	if(dcok==0)
	{
//		printf("MemSimUpdateCache: D$ OK=Ready\n");
		return(1);
	}

	if(dcok==2)
	{
		printf("MemSimUpdateCache: D$ OK=Hold\n");
		return(1);
	}

	if(dcok==1)
		printf("MemSimUpdateCache: D$ OK=OK\n");
	if(dcok==3)
		printf("MemSimUpdateCache: D$ OK=Fault\n");
#endif

	if(opm&0x10)
	{
//		printf("MemSimUpdateCache: Store A=%08X V=%08X\n", addr, val1);

		switch(opm&7)
		{
		case 0x0:
		case 0x4:
			MemSetByte(addr, val1);
			break;
		case 0x1:
		case 0x5:
			MemSetWord(addr, val1);
			break;
		case 0x2:
		case 0x6:
			MemSetDWord(addr, val1);
			break;
		case 0x3:
		case 0x7:
			MemSetQWord(addr, val1);
			break;
		}
	}

	if(lopm2&0x08)
	{
		switch(lopm2&7)
		{
		case 0x0:	val3=(int8_t)(MemGetByte(laddr2));		break;
		case 0x1:	val3=(int16_t)(MemGetWord(laddr2));		break;
		case 0x2:	val3=(int32_t)(MemGetDWord(laddr2));	break;
		case 0x3:	val3=(int64_t)(MemGetQWord(laddr2));	break;
		case 0x4:	val3=(uint8_t)(MemGetByte(laddr2));		break;
		case 0x5:	val3=(uint16_t)(MemGetWord(laddr2));	break;
		case 0x6:	val3=(uint32_t)(MemGetDWord(laddr2));	break;
		case 0x7:	val3=(uint64_t)(MemGetQWord(laddr2));	break;
		}
		
//		if(val3!=val2)
		if((val3!=val2) && (dcok==1))
		{
			printf("MemSimUpdateCache: D$ Mismatch "
				"Addr=%08X Opm=%X Expect %016llX Got %016llX\n",
				laddr2, lopm2, val3, val2);
			return(0);
		}
	}

	return(1);
}

#if 0
int CheckDebugSanityDS()
{
	uint32_t addr;
	uint64_t val1, val2;
	int dcop, dcok;
	int i;

//	dpc=top->dbgOutPc;
//	distr=top->dbgOutIstr;
	
	addr=top->dbgDcInAddr;
	val1=top->dbgDcInVal;
	val2=top->dbgDcOutVal;
	dcop=top->dbgDcInOpm;
	dcok=top->dbgDcOutOK;
	
//	if(!dpc)
	if(top->dbgExHold2)
		return(1);
	
	if((addr&0xF0000000)==0xF0000000)
		return(1);
	
	i=MemSimUpdateCache(addr, val1, val2, dcop, dcok);
	if(i<=0)
		return(0);
	
	return(1);
}
#endif

int CheckDebugSanity()
{
//	if(CheckDebugSanityIS()<=0)
//		return(0);
//	if(CheckDebugSanityDS()<=0)
//		return(0);
	return(1);
}

int sim_fb_drawled(uint32_t *fbuf, int xs, int ys,
	int led_x, int led_y, int led_r, uint32_t led_c)
{
	int lx, ly, tx, ty, ld, lr2;
	
	lr2=led_r*led_r;
	
	for(ly=-led_r; ly<led_r; ly++)
	{
		for(lx=-led_r; lx<led_r; lx++)
		{
			ld=(lx*lx)+(ly*ly);
			if(ld>lr2)
				continue;
			tx=led_x+lx;
			ty=led_y+ly;
			
			fbuf[ty*xs+tx]=led_c;
		}
	}
	return(0);
}

int main(int argc, char **argv, char **env)
{
	BJX2_Context *ctx;
	FILE *fd;
	uint64_t tot_bdl, lpc, cpc;
	int lclk, mhz;
	int tt_start;
	int ctick, ctick_rst;
	int x0, x1, x2, x3;
	int t0, t1, t2;
	int tt_frame;
	int cnt_dled, cnt_h1, cnt_h2,
		cnt_d1, cnt_d2, cnt_d3, cnt_d4,
		cnt_d5, cnt_d6, cnt_d7, cnt_d8;
	int i, j, k;

//	mhz=100;
	mhz=50;

//	JX2R_UseImageCreateRamdisk(128*1024);
	JX2R_UseImageCreateRamdisk(512*1024);
//	JX2R_UseImageCreateRamdisk(32*1024);

#if 1
	JX2R_UseImageAddFile(
		(char *)"BOOTLOAD.SYS",
		(char *)"../../tk_qsrc/tk_clib/tkshell.sys");
#endif

#if 1
	JX2R_UseImageAddFile(
//		(char *)"BOOTLOAD.SYS",
		(char *)"DOOM.EXE",
		(char *)"../../tk_qsrc/doomsrc2/doom_bjx2.exe");
//	JX2R_UseImageAddFile(
//		(char *)"DOOM1.WAD",
//		(char *)"../../tk_qsrc/doomsrc2/doom1.wad");
	JX2R_UseImageAddFile(
		(char *)"DOOMU.WAD",
//		(char *)"../../tk_qsrc/doomsrc2/doom.wad");
		(char *)"../../tk_qsrc/doomsrc2/doomulz.wad");
#endif

#if 1
	JX2R_UseImageAddFile(
//		(char *)"BOOTLOAD.SYS",
		(char *)"QUAKE.EXE",
		(char *)"../../tk_qsrc/quake_b64b.exe");

	JX2R_UseImageAddFile(
		(char *)"GLQUAKE.EXE",
		(char *)"../../tk_qsrc/glquake_tkra_b64b.exe");

//	JX2R_UseImageAddFile(
//		(char *)"ID1/PAK0.PAK",
//		(char *)"../../tk_qsrc/id1/pak0.pak");

	JX2R_UseImageAddFile(
		(char *)"ID1/PAK0LZ.WAD",
		(char *)"../../tk_qsrc/id1/pak0lz.wad");
	JX2R_UseImageAddFile(
		(char *)"ID1/PAK1LZ.WAD",
		(char *)"../../tk_qsrc/id1/pak1lz.wad");
	JX2R_UseImageAddFile(
		(char *)"ID1/PAK2LZ.WAD",
		(char *)"../../tk_qsrc/id1/pak2lz.wad");

//	JX2R_UseImageAddFile(
//		(char *)"ID1/AUTOEXEC.CFG",
//		(char *)"../../tk_qsrc/id1/autoexec1.cfg");
#endif

#if 1
	JX2R_UseImageAddFile(
		(char *)"HTIC/HTIC.EXE",
		(char *)"../../tk_ports/HticSrc/htic_bjx2.exe");
	JX2R_UseImageAddFile(
		(char *)"HTIC/HTICULZ.WAD",
		(char *)"../../tk_ports/HticSrc/hticulz.wad");
#endif

#if 1
	JX2R_UseImageAddFile(
		(char *)"ROTT/ROTT.EXE",
		(char *)"../../tk_ports/RottSrc/rott/rott_bjx2.exe");
	JX2R_UseImageAddFile(
		(char *)"ROTT/DARKWAR.WAD",
		(char *)"../../tk_ports/RottSrc/darkwlz.wad");
	JX2R_UseImageAddFile(
		(char *)"ROTT/DARKWAR.RTL",
		(char *)"../../tk_ports/RottSrc/darkwar.rtl");
	JX2R_UseImageAddFile(
		(char *)"ROTT/DARKWAR.RTC",
		(char *)"../../tk_ports/RottSrc/darkwar.rtc");
	JX2R_UseImageAddFile(
		(char *)"ROTT/DEMO1_3.DMO",
		(char *)"../../tk_ports/RottSrc/demo1_3.dmo");
	JX2R_UseImageAddFile(
		(char *)"ROTT/DEMO2_3.DMO",
		(char *)"../../tk_ports/RottSrc/demo2_3.dmo");
#endif

#ifndef NOSWAP
	JX2R_UseImageAddFileBuffer("swapfile.sys", (byte *)NULL, 384*(1<<20));
#endif

	Verilated::commandArgs(argc, argv);

	rombuf=(uint32_t *)malloc(32768);
	srambuf=(uint32_t *)malloc(8192);

	drambuf=(uint32_t *)malloc(1<<27);

	srambuf2=(uint32_t *)malloc(8192);
	drambuf2=(uint32_t *)malloc(1<<27);
	
	memset(srambuf, 0, 8192);
	memset(drambuf, 0, 1<<27);

	memset(srambuf2, 0, 8192);
	memset(drambuf2, 0, 1<<27);

	fd=fopen("../../tst_jx2boot.bin", "rb");
	if(fd)
	{
		t2=fread(rombuf, 1, 32768, fd);
		fclose(fd);
	}

	ctx=(BJX2_Context *)malloc(sizeof(BJX2_Context));
	memset(ctx, 0, sizeof(BJX2_Context));
	jx2_ctx=ctx;

	ctx->tgt_mhz=mhz;
	ctx->rcp_mhz=((1048576/ctx->tgt_mhz)+7)>>4;
	ctx->ttick_rst=(ctx->tgt_mhz*1000000)/1024;
	ctx->ttick_hk=ctx->ttick_rst;
	
	lpc=0;
	tot_bdl=0;

	GfxDrv_Start();
	SoundDev_Init();
	JX2I_GfxCon_Startup();

	printf("Start ExUnit\n");

	BTSR1_MainInitKeyboard();

	tt_start=FRGL_TimeMS();
	tt_frame=tt_start;
	membus_256bit=0;
	
//	ctick_rst=48828;
//	ctick=ctick_rst;

	while (!Verilated::gotFinish())
	{
		t1=FRGL_TimeMS();
		t2=t1-tt_frame;

		cnt_dled++;
		cnt_h1+=(top->dbgExHold1!=0);
		cnt_h2+=(top->dbgExHold2!=0);
		cnt_d1+=(top->dbgOutStatus1!=0);
		cnt_d2+=(top->dbgOutStatus2!=0);
		cnt_d3+=(top->dbgOutStatus3!=0);
		cnt_d4+=(top->dbgOutStatus4!=0);
		cnt_d5+=(top->dbgOutStatus5!=0);
		cnt_d6+=(top->dbgOutStatus6!=0);
		cnt_d7+=(top->dbgOutStatus7!=0);
		cnt_d8+=(top->dbgOutStatus8!=0);

//		if(t2>16)
//		if((t2>16) && (cnt_dled>8))
		if((t2>64) && (cnt_dled>32))
		{
			BTSR1_MainPollKeyboard();

			jx2i_gfxcon_dirty=1;
			JX2I_GfxCon_Update();

			cnt_h1=(255*cnt_h1)/cnt_dled;
			cnt_h2=(255*cnt_h2)/cnt_dled;
			cnt_d1=(255*cnt_d1)/cnt_dled;
			cnt_d2=(255*cnt_d2)/cnt_dled;
			cnt_d3=(255*cnt_d3)/cnt_dled;
			cnt_d4=(255*cnt_d4)/cnt_dled;
			cnt_d5=(255*cnt_d5)/cnt_dled;
			cnt_d6=(255*cnt_d6)/cnt_dled;
			cnt_d7=(255*cnt_d7)/cnt_dled;
			cnt_d8=(255*cnt_d8)/cnt_dled;
			
			cnt_h1=0xFF00007F+(cnt_h1<<16)+(cnt_h1<<8)-(cnt_h1>>1);
			cnt_h2=0xFF00007F+(cnt_h2<<16)+(cnt_h2<<8)-(cnt_h2>>1);

			cnt_d1=0xFF00007F+(cnt_d1<<16)+(cnt_d1<<8)-(cnt_d1>>1);
			cnt_d2=0xFF00007F+(cnt_d2<<16)+(cnt_d2<<8)-(cnt_d2>>1);
			cnt_d3=0xFF00007F+(cnt_d3<<16)+(cnt_d3<<8)-(cnt_d3>>1);
			cnt_d4=0xFF00007F+(cnt_d4<<16)+(cnt_d4<<8)-(cnt_d4>>1);
			cnt_d5=0xFF00007F+(cnt_d5<<16)+(cnt_d5<<8)-(cnt_d5>>1);
			cnt_d6=0xFF00007F+(cnt_d6<<16)+(cnt_d6<<8)-(cnt_d6>>1);
			cnt_d7=0xFF00007F+(cnt_d7<<16)+(cnt_d7<<8)-(cnt_d7>>1);
			cnt_d8=0xFF00007F+(cnt_d8<<16)+(cnt_d8<<8)-(cnt_d8>>1);

			sim_fb_drawled(
				(uint32_t *)btesh2_gfxcon_framebuf,
				btesh2_gfxcon_fbxs, btesh2_gfxcon_fbys,
				btesh2_gfxcon_fbxs-6, btesh2_gfxcon_fbys-1*8, 3,
//				top->dbgExHold1?0xFFFFFF00:0xFF00007F);
				cnt_h1);

			sim_fb_drawled(
				(uint32_t *)btesh2_gfxcon_framebuf,
				btesh2_gfxcon_fbxs, btesh2_gfxcon_fbys,
				btesh2_gfxcon_fbxs-6, btesh2_gfxcon_fbys-2*8, 3,
//				top->dbgExHold2?0xFFFFFF00:0xFF00007F);
				cnt_h2);


			sim_fb_drawled(
				(uint32_t *)btesh2_gfxcon_framebuf,
				btesh2_gfxcon_fbxs, btesh2_gfxcon_fbys,
				btesh2_gfxcon_fbxs-6, btesh2_gfxcon_fbys-4*8, 3,
//				top->dbgOutStatus1?0xFFFFFF00:0xFF00007F);
				cnt_d1);

			sim_fb_drawled(
				(uint32_t *)btesh2_gfxcon_framebuf,
				btesh2_gfxcon_fbxs, btesh2_gfxcon_fbys,
				btesh2_gfxcon_fbxs-6, btesh2_gfxcon_fbys-5*8, 3,
//				top->dbgOutStatus2?0xFFFFFF00:0xFF00007F);
				cnt_d2);

			sim_fb_drawled(
				(uint32_t *)btesh2_gfxcon_framebuf,
				btesh2_gfxcon_fbxs, btesh2_gfxcon_fbys,
				btesh2_gfxcon_fbxs-6, btesh2_gfxcon_fbys-6*8, 3,
//				top->dbgOutStatus3?0xFFFFFF00:0xFF00007F);
				cnt_d3);

			sim_fb_drawled(
				(uint32_t *)btesh2_gfxcon_framebuf,
				btesh2_gfxcon_fbxs, btesh2_gfxcon_fbys,
				btesh2_gfxcon_fbxs-6, btesh2_gfxcon_fbys-7*8, 3,
//				top->dbgOutStatus4?0xFFFFFF00:0xFF00007F);
				cnt_d4);


			sim_fb_drawled(
				(uint32_t *)btesh2_gfxcon_framebuf,
				btesh2_gfxcon_fbxs, btesh2_gfxcon_fbys,
				btesh2_gfxcon_fbxs-6, btesh2_gfxcon_fbys-9*8, 3,
//				top->dbgOutStatus5?0xFFFFFF00:0xFF00007F);
				cnt_d5);

			sim_fb_drawled(
				(uint32_t *)btesh2_gfxcon_framebuf,
				btesh2_gfxcon_fbxs, btesh2_gfxcon_fbys,
				btesh2_gfxcon_fbxs-6, btesh2_gfxcon_fbys-10*8, 3,
//				top->dbgOutStatus6?0xFFFFFF00:0xFF00007F);
				cnt_d6);

			sim_fb_drawled(
				(uint32_t *)btesh2_gfxcon_framebuf,
				btesh2_gfxcon_fbxs, btesh2_gfxcon_fbys,
				btesh2_gfxcon_fbxs-6, btesh2_gfxcon_fbys-11*8, 3,
//				top->dbgOutStatus7?0xFFFFFF00:0xFF00007F);
				cnt_d7);

			sim_fb_drawled(
				(uint32_t *)btesh2_gfxcon_framebuf,
				btesh2_gfxcon_fbxs, btesh2_gfxcon_fbys,
				btesh2_gfxcon_fbxs-6, btesh2_gfxcon_fbys-12*8, 3,
//				top->dbgOutStatus8?0xFFFFFF00:0xFF00007F);
				cnt_d8);

			cnt_dled=0;
			cnt_h1=0;	cnt_h2=0;
			cnt_d1=0;	cnt_d2=0;	cnt_d3=0;	cnt_d4=0;
			cnt_d5=0;	cnt_d6=0;	cnt_d7=0;	cnt_d8=0;

			GfxDrv_BeginDrawing();

			GfxDrv_EndDrawing();
			
			t1=FRGL_TimeMS();
			tt_frame=t1;
//			continue;
		}

		top->clock = (main_time>>0)&1;

		top->reset=0;
		if(main_time<256)
		{
			top->reset=1;
//			if(top->memOpm==0x02)
			if(top->memOpm==0x14)
				membus_256bit=1;
		}

//		if(top->clock)
//			top->memBusExc=0x0000;
		
		if(top->clock && (lclk!=top->clock))
		{
//			printf("Cycle\n");

			ctx->ttick_hk--;
			if(ctx->ttick_hk<=0)
			{
//				top->memBusExc=0xC001;
			
//				printf("Clock IRQ\n");
				ctx->ttick_hk=ctx->ttick_rst;
			}
		}
		
		jx2_ctx->tot_cyc=main_time>>1;

//		cpc=top->dbgOutPc[0];
		cpc=top->dbgOutPc;
//		if(lpc!=top->dbgOutPc)
		if(lpc!=cpc)
		{
			lpc=cpc;
			tot_bdl++;
		}

		main_time++;

		lclk = top->clock;
//		top->mode = 3;
		
//		top->baseAddr=0xDECAB00;
//		top->idxAddr=0x100;
//		top->idxDisp=3;

//		if(top->clock)
		if(1)
		{
//			printf("Cycle %lld\n", jx2_ctx->tot_cyc);
		
			MemUpdateForBus();
			
			if(CheckDebugSanity()<=0)
				break;
		}

		top->eval();

//		main_time++;
		
//		if(main_time>256)
//		if(main_time>1024)
//		if(main_time>2048)
//		if(main_time>4096)
//		if(main_time>16384)
//		if(main_time>65536)
//		if(main_time>(1<<20))
		if(gfxdrv_kill)
		{
//			printf("%llX\n", (long long)(top->outAddr));
			break;
		}
	}

	t1=FRGL_TimeMS();
	t2=t1-tt_start;

	printf("%.3fMHz\n", jx2_ctx->tot_cyc/(t2*1000.0));
	printf("%.3f bdl/clk\n", (1.0*tot_bdl)/(jx2_ctx->tot_cyc+1));

	SoundDev_DeInit();
	
	BTSR1_MainDeinitKeyboard();
	
	for(i=0; i<8; i++)
	{
		j=l2m_hash_hit[i]+l2m_hash_miss[i];
		if(!j)
			continue;
		printf("Hash %d: %d/%d %.2f%%\n",
			i, l2m_hash_hit[i], j,
			(100.0*l2m_hash_hit[i])/j);
	}
	
	j=l2dc_stat_hit+l2dc_stat_hit2+l2dc_stat_miss;
	if(j>0)
	{
		printf("L2 Hit 1/2/Miss %d/%d/%d %.2f%%/%.2f%%/%.2f%%\n",
			l2dc_stat_hit, l2dc_stat_hit2, l2dc_stat_miss,
			(100.0*l2dc_stat_hit )/j,
			(100.0*l2dc_stat_hit2)/j,
			(100.0*l2dc_stat_miss)/j);
	}
	
	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
