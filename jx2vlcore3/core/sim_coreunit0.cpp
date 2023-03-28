#include "VCoreUnit.h"
#include "verilated.h"

VCoreUnit *top = new VCoreUnit;

vluint64_t main_time = 0;
vluint64_t main_time3p = 0;

int do_qmt;


#define CLOCK_400MHZ			//Enable 300MHz and 400MHz Clock

#define CLOCK_200MHZ			//Enable 200MHz Clock
// #define CLOCK_200_AS_133		//Scale 200MHz clock to 133MHz
#define CLOCK_200_AS_150		//Scale 200MHz clock to 150MHz



#ifdef __linux
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#endif

#if defined(linux) || defined(__EMSCRIPTEN__)
#include <sys/time.h>
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

byte kbbuf[256];
byte kbrov;
byte kbirov;
// byte gfxdrv_kill;
byte fkey;

byte fmode;

byte ps2kbbuf[256];
byte ps2kbrov;
byte ps2kbirov;

// #ifdef __linux
#if 0
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


int BTSR1_MainAddScanKeyByte(int k)
{
	ps2kbbuf[ps2kbrov]=k;
	ps2kbrov=(ps2kbrov+1)&255;
	return(0);
}

byte usb_kbmsg[32];
byte usb_kbmsgsz;
byte usb_kbsticky;

u16 usb_kbstbuf[32];
byte usb_kbstpos;

int BTSR1_MainAddUsbKey(int key)
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
		
		if(kix&0xFF)
		{
			usb_kbsticky=1;
		}
	}
	
	usb_kbmsg[ 0]=0xC3;	//PID: IN
	usb_kbmsg[ 1]=shift;	//Shift
	usb_kbmsg[ 2]=0x00;
	usb_kbmsg[ 3]=keys[0];
	usb_kbmsg[ 4]=keys[1];
	usb_kbmsg[ 5]=keys[2];
	usb_kbmsg[ 6]=keys[3];
	usb_kbmsg[ 7]=keys[4];
	usb_kbmsg[ 8]=keys[5];
	usb_kbmsg[ 9]=0;
	usb_kbmsg[10]=0;
	usb_kbmsgsz=11;

	return(0);
}

int BTSR1_MainAddTranslateKey(int key)
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
	
	int sc, wasst;
	int i, j, k;

	if(do_qmt)
	{
//		byte usb_kbsticky;
//		u16 usb_kbstbuf[32];
//		byte usb_kbstpos;

		wasst=0;
		if(!usb_kbsticky && (usb_kbstpos>0))
		{
			/* Handle held-back key releases. */
			for(i=0; i<usb_kbstpos; i++)
			{
				k=usb_kbstbuf[i];
				BTSR1_MainAddUsbKey(k);
				if(!(k&0x8000))
					break;
				for(j=i; j<usb_kbstpos; j++)
					usb_kbstbuf[i]=usb_kbstbuf[i+1];
				usb_kbstpos--;
			}
//			usb_kbstpos=0;
//			usb_kbsticky=0;
			wasst=1;
		}

//		if((usb_kbsticky && (key&0x8000)) || wasst)
		if(0)
		{
			/* If sticky and key is released, hold back key release. */
			usb_kbstbuf[usb_kbstpos++]=key;
			usb_kbstbuf[usb_kbstpos]=0;
		}else
		{
			BTSR1_MainAddUsbKey(key);
		}
		return(0);
	}

	sc=ps2_key2scan[key&0xFF];
	if(!sc)
		return(0);

	if(key&0x8000)
	{
		if(sc&0x100)
			BTSR1_MainAddScanKeyByte(0xE0);
		BTSR1_MainAddScanKeyByte(0xF0);
		BTSR1_MainAddScanKeyByte(sc&0xFF);
		return(0);
	}

	if(sc&0x100)
		BTSR1_MainAddScanKeyByte(0xE0);
	BTSR1_MainAddScanKeyByte(sc&0xFF);

	return(0);
}

int btn_reset;

int BTSR1_MainPollKeyboard(void)
{
	u16 *kb;
	int i, j, k, l;

//	FRGL_GetKeybuf();

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
		
		if(FRGL_KeyDown(K_SHIFT))
		{
			if(k==K_F12)
			{
				btn_reset = !btn_reset;
				
				if(btn_reset)
				{
					printf("\n\n\n\n\n=== RESET ===\n\n\n\n\n");
				}
				
				continue;
			}

			if(k==(0x8000|K_F12))
			{
				continue;
			}
		}

		BTSR1_MainAddTranslateKey(k);
		BTSR1_MainAddKey(k);
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


#if 1
uint32_t *rombuf;
uint32_t *srambuf;
uint32_t *drambuf;

uint32_t *srambuf2;
uint32_t *drambuf2;
#endif

// uint16_t *drambuf;

int sdc_spipos;

int sdc_spibit(int bit, int cs)
{
	static int init=0;
	static int pos, tvi, tvo, tvn;
	static int ret;

	if(!init)
	{
		printf("sdc_spibit: Init\n");
		init=1;
		tvo=0xFFFF;
		pos=8;
		ret=1;
	}

	if(!cs)
	{
//		pos++;
//		pos--;
//		if(pos>=8)
//		if(pos==0)
		if(pos<=0)
		{
//			if(pos<0)
//				tvi=0xFF;

//			tvo=tvn;
//			tvn=btesh2_spimmc_XrByte(NULL, tvi);
//			tvo=(tvo<<8)|btesh2_spimmc_XrByte(NULL, tvi);
			tvo=tvo|btesh2_spimmc_XrByte(NULL, tvi);
//			printf("sdc_spibit: tvi=%02X tvo=%02X\n", tvi, tvo);
//			pos=0; tvi=0;
			pos=8; tvi=0;
		}
		
//		printf("sdc_spibit bit=%d\n", pos);

//		tvi=(tvi>>1)|(bit<<7);
//		ret=tvo&1;	tvo>>=1;
		tvi=(tvi<<1)|(bit&1);
//		ret=(tvo>>7)&1;
		ret=(tvo>>14)&1;
		tvo=tvo<<1;
		pos--;
	}else
	{
//		printf("sdc_spibit: Non-Select\n");
		tvo=0xFFFF;
		tvi=0xFF;
//		tvn=0xFF;
		pos=8;
		ret=1;
	}
	
	sdc_spipos=pos;
	return(ret);
}

#define	SIMDDR_BUSRT	4
// #define	SIMDDR_BUSRT	8

#define SIMDDR_SHL_A15	23
#define SIMDDR_SHL_A14	22
#define SIMDDR_SHL_A13	21

#define SIMDDR_SHL_CS	20
#define SIMDDR_SHL_RAS	19
#define SIMDDR_SHL_CAS	18
#define SIMDDR_SHL_WE	17
#define SIMDDR_SHL_CKE	16
#define SIMDDR_SHL_BA	14

#define SIMDDR_MSK_CS	(1<<20)
#define SIMDDR_MSK_RAS	(1<<19)
#define SIMDDR_MSK_CAS	(1<<18)
#define SIMDDR_MSK_WE	(1<<17)
#define SIMDDR_MSK_CKE	(1<<16)
#define SIMDDR_MSK_BA	(3<<14)

#define SIMDDR_MSK_NOP	(SIMDDR_MSK_RAS|SIMDDR_MSK_CAS|SIMDDR_MSK_WE)


int ddr_cmdhi, ddr_cmdlo;
int ddr_cyc;

int ddr_parm_wl=3;	//CAS WL*2+1
int ddr_parm_rl=7;	//CAS RL*2+1
int ddr_row, ddr_col, ddr_bank;
int ddr_state, ddr_cas, ddr_burst;
int ddr_bcol, ddr_bstate;
int ddr_rowopen, ddr_doap;

int ddr_burstlen;
int ddr_mr0;
int ddr_mr1;
int ddr_mr2;
int ddr_mr3;
int ddr_is_ddr3;

uint16_t	*ddr_ram;

#if 0
int SimDdr(int clk, int cmd, int *rdqs, int *rdata)
{
	int		data, row, col, bank, pos, dqs;
	int addr, cas;

//Cs;
//			cmd=(cmd<<1)|top->ddrRas;
//			cmd=(cmd<<1)|top->ddrCas;
//			cmd=(cmd<<1)|top->ddrWe;
//			cmd=(cmd<<1)|top->ddrCke;
//			cmd=(cmd<<2)|top->ddrBa

	dqs=*rdqs;
	data=*rdata;
#if 0
//	printf("%03X %04X\n", cmd, data);
	printf("Cs=%d Ras=%d Cas=%d We=%d Cke=%d Ba=%d A=%04X D=%04X\n",
		(cmd>>20)&1,	
		(cmd>>19)&1,	(cmd>>18)&1,
		(cmd>>17)&1,	(cmd>>16)&1,
		(cmd>>14)&3,	(cmd&0x3FFF),
		data);
#endif
	
	if(ddr_cas>=0)
	{
		ddr_cas--;
		if(ddr_state==1)
			*rdqs=2;
	}else if(ddr_state==1)
	{
		if(ddr_burst>0)
		{
			ddr_burst--;

			pos=(ddr_row<<13)+(ddr_bank<<10)+ddr_col;
			pos&=(1<<27)-1;
			data=ddr_ram[pos>>1];
			ddr_col+=2;

			*rdata=data;
			*rdqs=(ddr_burst&1)?1:2;
		}
	}else if(ddr_state==2)
	{
		if(ddr_burst>0)
		{
			if((dqs&3)!=(clk&3))
			{
				printf("DDR DQS Issue %d!=%d\n", dqs&3, clk&3);
			}
			
			ddr_burst--;

			data=*rdata;
			pos=(ddr_row<<13)+(ddr_bank<<10)+ddr_col;
			pos&=(1<<27)-1;
			ddr_ram[pos>>1]=data;
			ddr_col+=2;
		}
	}
		
//	if(!clk)
	if(!(clk&1))
	{
		return(0);
	}
	
//	addr=(cmd&0x3FFF);
	addr=(cmd&0x1FFF);
	bank=(cmd>>13)&7;
	addr|=((cmd>>SIMDDR_SHL_A13)&7)<<13;
	
	if(cmd&SIMDDR_MSK_CS)
	{
//		printf("Command Inhibit\n");
		return(0);
	}

	if(cmd&SIMDDR_MSK_RAS)
	{
		if(cmd&SIMDDR_MSK_CAS)
		{
			if(cmd&SIMDDR_MSK_WE)
			{
//				printf("No-Op\n");
			}else
			{
				printf("Burst Terminate\n");
			}
		}else
		{
			if(cmd&SIMDDR_MSK_WE)
			{
//				printf("Read Active Row\n");
				ddr_col=addr;
				ddr_bank=bank;
				ddr_state=1;
//				ddr_cas=ddr_parm_rl*2+1;
//				ddr_cas=ddr_parm_rl*2;
				ddr_cas=ddr_parm_rl*2-1;
//				ddr_cas=ddr_parm_rl*2-2;
//				ddr_cas=4*2+1;
//				ddr_burst=SIMDDR_BUSRT;
				ddr_burst=ddr_burstlen;
			}else
			{
//				printf("Write Active Row\n");
				ddr_col=addr;
				ddr_bank=bank;
				ddr_state=2;
//				ddr_cas=ddr_parm_wl*2+1;
//				ddr_cas=ddr_parm_wl*2;
				ddr_cas=ddr_parm_wl*2-2;
//				ddr_burst=SIMDDR_BUSRT;
				ddr_burst=ddr_burstlen;
			}
		}
	}else
	{
		if(cmd&SIMDDR_MSK_CAS)
		{
			if(cmd&SIMDDR_MSK_WE)
			{
//				printf("Activate Row\n");
				ddr_row=addr;
			}else
			{
//				printf("Precharge Row\n");
			}
		}else
		{
			if(cmd&SIMDDR_MSK_WE)
			{
				printf("Auto-Refresh Row\n");
			}else
			{
//				printf("Load Mode Register A=%04X V=%04X\n", addr, data);
				printf("Load Mode Register C=%04X\n", cmd&0xFFFF);
//				switch((cmd>>14)&3)
				switch((cmd>>13)&7)
				{
				case 0:		ddr_mr0=addr;	break;
				case 1:		ddr_mr1=addr;	break;
				case 2:		ddr_mr2=addr;	break;
				case 3:		ddr_mr3=addr;	break;
				}
				
				ddr_burstlen=(ddr_mr0&1)?8:4;
				
				cas=(ddr_mr0>>4)&7;
//				ddr_parm_rl=cas*2+1;		//CAS RL*2+1
//				ddr_parm_wl=ddr_parm_rl-2;	//CAS WL=RL-1
				ddr_parm_rl=cas;			//CAS RL*2+1
				ddr_parm_wl=ddr_parm_rl-1;	//CAS WL=RL-1

				if(ddr_is_ddr3)
				{
					switch(ddr_mr0&3)
					{
					case 0:		ddr_burstlen=8;		break;
					case 1:		ddr_burstlen=4;		break;
					case 2:		ddr_burstlen=4;		break;
					case 3:		ddr_burstlen=-1;	break;
					}

					cas=((ddr_mr0>>4)&7)+4;
					ddr_parm_rl=cas;
					col=((ddr_mr2>>3)&7)+5;
					ddr_parm_wl=col;
				}

				printf("BurstLen=%d, CAS=%d\n", ddr_burstlen, cas);

#if 1
				printf("  DLL=%s\n", (ddr_mr1&1)?"Disable":"Enable");
				printf("  ODS=%s\n", (ddr_mr1&2)?"Reduce":"Full");

				printf("  DQS#=%s\n", (ddr_mr1&1024)?"Disable":"Enable");
				printf("  RDQS=%s\n", (ddr_mr1&2048)?"Yes":"No");
				printf("  Outp=%s\n", (ddr_mr1&4096)?"Disable":"Enable");
#endif
			}
		}
	}
}
#endif

#if 1
int SimDdr(int clk, int cmd, int *rdqs, int *rdata)
{
	int		nxtburst, nxtbcol, nxtbstate;
	int		data, row, col, bank, pos;
	int addr, cas, dqs;

//Cs;
//			cmd=(cmd<<1)|top->ddrRas;
//			cmd=(cmd<<1)|top->ddrCas;
//			cmd=(cmd<<1)|top->ddrWe;
//			cmd=(cmd<<1)|top->ddrCke;
//			cmd=(cmd<<2)|top->ddrBa

	dqs=*rdqs;
	data=*rdata;

#if 0
//	printf("%03X %04X\n", cmd, data);
	printf("Cs=%d Ras=%d Cas=%d We=%d Cke=%d Ba=%d A=%04X D=%04X\n",
		(cmd>>20)&1,	
		(cmd>>19)&1,	(cmd>>18)&1,
		(cmd>>17)&1,	(cmd>>16)&1,
		(cmd>>14)&3,	(cmd&0x3FFF),
		data);
#endif

	nxtburst = ddr_burst;
	nxtbcol = ddr_bcol;
	nxtbstate = ddr_bstate;

	if(ddr_bstate==1)
	{
		if(ddr_burst>0)
		{
//			ddr_burst--;
			nxtburst = ddr_burst-1;

			pos=(ddr_row<<13)+(ddr_bank<<10)+ddr_bcol;
//			pos&=(1<<28)-1;
//			data=ddr_ram[pos>>1];
//			nxtbcol=ddr_bcol+2;

			pos&=(1<<27)-1;
			data=ddr_ram[pos];
			nxtbcol=ddr_bcol+1;

			*rdata=data;
//			*rdqs=(ddr_burst&1)?1:2;
			*rdqs=(nxtburst&1)?1:2;

//			printf("RD %08X: %04X\n", pos, data);

		}else
		{
			*rdata=0xBAAD;
		}
	}else if(ddr_bstate==2)
	{
		if(ddr_burst>0)
		{
			if((dqs&3)!=(clk&3))
			{
				printf("DDR DQS Issue %d!=%d\n", dqs&3, clk&3);
			}
			
//			ddr_burst--;
			nxtburst = ddr_burst-1;

			data=*rdata;
			pos=(ddr_row<<13)+(ddr_bank<<10)+ddr_bcol;
//			pos&=(1<<28)-1;
//			ddr_ram[pos>>1]=data;
//			nxtbcol=ddr_bcol+2;

			pos&=(1<<27)-1;
			ddr_ram[pos]=data;
			nxtbcol=ddr_bcol+1;
			
//			printf("ST %08X = %04X\n", pos, data);
		}
	}else
	{
		*rdata=0xBAAD;
	}

	if(ddr_cas>=0)
	{
		ddr_cas--;
		if((ddr_state==1) && (ddr_burst<=0))
			*rdqs=2;

		if(ddr_cas<0)
		{
//			ddr_burst=ddr_burstlen;
			nxtburst = ddr_burstlen;
			nxtbcol = ddr_col;
			nxtbstate = ddr_state;
			
			if(ddr_doap)
				ddr_rowopen=0;
		}
	}
	
	ddr_burst = nxtburst;
	ddr_bcol = nxtbcol;
	ddr_bstate = nxtbstate;

//	if(!clk)
	if(!(clk&1))
	{
		return(0);
	}


//	addr=(cmd&0x3FFF);
	addr=(cmd&0x1FFF);
	bank=(cmd>>13)&7;
	addr|=((cmd>>SIMDDR_SHL_A13)&7)<<13;
	
	if(cmd&SIMDDR_MSK_CS)
	{
//		printf("Command Inhibit\n");
		return(0);
	}

	if(cmd&SIMDDR_MSK_RAS)
	{
		if(cmd&SIMDDR_MSK_CAS)
		{
			if(cmd&SIMDDR_MSK_WE)
			{
//				printf("No-Op\n");
			}else
			{
				printf("Burst Terminate\n");
			}
		}else
		{
			if(cmd&SIMDDR_MSK_WE)
			{
				if(!ddr_rowopen)
					printf("DDR, Read Active Row: Row Not Active\n");
			
//				printf("Read Active Row, Ba=%X Col=%X\n", bank, addr);
//				ddr_col=addr;
				ddr_col=(addr&0x03FF)|((addr>>1)&0x0400);
				ddr_bank=bank;
				ddr_state=1;
//				ddr_cas=ddr_parm_rl*2+1;
//				ddr_cas=ddr_parm_rl*2;
				ddr_cas=ddr_parm_rl*2-1;
//				ddr_cas=ddr_parm_rl*2-2;
//				ddr_cas=4*2+1;
//				ddr_burst=ddr_burstlen;
				ddr_doap=(addr&0x0400)!=0;
			}else
			{
				if(!ddr_rowopen)
					printf("DDR, Write Active Row: Row Not Active\n");

//				printf("Write Active Row, Ba=%X Col=%X\n", bank, addr);
//				ddr_col=addr;
				ddr_col=(addr&0x03FF)|((addr>>1)&0x0400);
				ddr_bank=bank;
				ddr_state=2;
//				ddr_cas=ddr_parm_wl*2+1;
//				ddr_cas=ddr_parm_wl*2;
//				ddr_cas=ddr_parm_wl*2-1;
				ddr_cas=ddr_parm_wl*2-2;
//				ddr_burst=ddr_burstlen;
				ddr_doap=(addr&0x0400)!=0;
			}
		}
	}else
	{
		if(cmd&SIMDDR_MSK_CAS)
		{
			if(cmd&SIMDDR_MSK_WE)
			{
				if(ddr_rowopen)
					printf("DDR, Activate Row: Row Still Active\n");

				ddr_rowopen=1;
//				printf("Activate Row\n");
				ddr_row=addr;
			}else
			{
//				printf("Precharge Row\n");
				ddr_rowopen=0;
			}
		}else
		{
			if(cmd&SIMDDR_MSK_WE)
			{
				printf("Auto-Refresh Row\n");
				ddr_rowopen=0;
			}else
			{
				printf("Load Mode Register MR%u C=%04X\n",
					(cmd>>13)&7, cmd&0xFFFF);
//				switch((cmd>>14)&3)
				switch((cmd>>13)&7)
				{
				case 0:		ddr_mr0=addr;	break;
				case 1:		ddr_mr1=addr;	break;
				case 2:		ddr_mr2=addr;	break;
				case 3:		ddr_mr3=addr;	break;
				}
				
				ddr_burstlen=(ddr_mr0&1)?8:4;
				
				cas=(ddr_mr0>>4)&7;
//				ddr_parm_rl=cas*2+1;		//CAS RL*2+1
//				ddr_parm_wl=ddr_parm_rl-2;	//CAS WL=RL-1
				ddr_parm_rl=cas;			//CAS RL*2+1
				ddr_parm_wl=ddr_parm_rl-1;	//CAS WL=RL-1
				
				if(ddr_is_ddr3)
				{
					switch(ddr_mr0&3)
					{
					case 0:		ddr_burstlen=8;		break;
					case 1:		ddr_burstlen=4;		break;
					case 2:		ddr_burstlen=4;		break;
					case 3:		ddr_burstlen=-1;	break;
					}

					cas=((ddr_mr0>>4)&7)+4;
					ddr_parm_rl=cas;
					col=((ddr_mr2>>3)&7)+5;
					ddr_parm_wl=col;
				}

//				printf("BurstLen=%d, CAS=%d\n", ddr_burstlen, cas);
				printf("BurstLen=%d, CAS(RL)=%d CAS(WL)=%d\n",
					ddr_burstlen, ddr_parm_rl, ddr_parm_wl);

#if 1
				printf("  DLL=%s\n", (ddr_mr1&1)?"Disable":"Enable");
				printf("  ODS=%s\n", (ddr_mr1&2)?"Reduce":"Full");

				printf("  DQS#=%s\n", (ddr_mr1&1024)?"Disable":"Enable");
				printf("  RDQS=%s\n", (ddr_mr1&2048)?"Yes":"No");
				printf("  Outp=%s\n", (ddr_mr1&4096)?"Disable":"Enable");
#endif
			}
		}
	}
}
#endif

int update_ddr()
{
	static int ddrlclk;
	int cmd, data, dqs;
	int n, inh;
	int i, j, k;

	if(top->ddrClk!=ddrlclk)
	{
		ddrlclk=top->ddrClk;
//			cmd=top->ddrCmd;

		dqs=((top->ddrDqsP_O)&1)|((top->ddrDqsN_O)&2);
		if(!top->ddrDqs_En)
			dqs=0;

		cmd=0;
		cmd=(cmd<<3)|((top->ddrAddr>>13)&0x7);
		cmd=(cmd<<1)|top->ddrCs;
		cmd=(cmd<<1)|top->ddrRas;
		cmd=(cmd<<1)|top->ddrCas;
		cmd=(cmd<<1)|top->ddrWe;
		cmd=(cmd<<1)|top->ddrCke;
//		cmd=(cmd<<2)|top->ddrBa;
//		cmd=(cmd<<14)|top->ddrAddr;

		cmd=(cmd<<3)|top->ddrBa;
//		cmd=(cmd<<13)|top->ddrAddr;
		cmd=(cmd<<13)|(top->ddrAddr&0x1FFF);

//			data=top->ddrData;
		data=top->ddrDataO;
		SimDdr(top->ddrClk, cmd, &dqs, &data);
//			top->ddrData=data;
		top->ddrDataI=data;
		top->ddrDqsP_I=(dqs&1)?3:0;
		top->ddrDqsN_I=(dqs&2)?3:0;

#if 0
		if((cmd&SIMDDR_MSK_NOP)!=SIMDDR_MSK_NOP)
		{
			printf("update_ddr: cmd=%04X clk=%d dqs=%d data=%04X->%04X\n",
				cmd, top->ddrClk, dqs,
				top->ddrDataO, top->ddrDataI);
		}
#endif
	}
}

int update_uart();

short usb_txbuf[256];
byte usb_txposs;
byte usb_txpose;

short usb_rxbuf[256];
byte usb_rxposs;
byte usb_rxpose;

uint16_t TKUSB_Crc16(byte *data, uint32_t sz);

byte usb_hidmsg[64];
byte usb_hidmsgsz;

// byte usb_kbmsg[32];
// byte usb_kbmsgsz;

int usb_hidkbreport()
{
	int i, j, k;
	
	printf("usb_hidkbreport sz=%d\n", usb_kbmsgsz);
	
	if(usb_txpose==usb_txposs)
	{
		usb_txpose=0;
		usb_txposs=0;
	}
	
	for(i=0; i<usb_kbmsgsz; i++)
	{
		usb_txbuf[usb_txpose]=usb_kbmsg[i];
		usb_txpose=(usb_txpose+1)&255;
	}

	usb_txbuf[usb_txpose]=0x100;
	usb_txpose=(usb_txpose+1)&255;
	
	usb_kbsticky=0;
}

int usb_hidctrlreport()
{
	int i, j, k;
	
	printf("usb_hidkbreport sz=%d\n", usb_kbmsgsz);
	
	if(usb_txpose==usb_txposs)
	{
		usb_txpose=0;
		usb_txposs=0;
	}
	
	for(i=0; i<usb_hidmsgsz; i++)
	{
		usb_txbuf[usb_txpose]=usb_hidmsg[i];
		usb_txpose=(usb_txpose+1)&255;
	}

	usb_txbuf[usb_txpose]=0x100;
	usb_txpose=(usb_txpose+1)&255;
}

int usb_gethiddesc(int len)
{
	int i, j, k, l;
	
	printf("usb_gethiddesc sz=%d\n", len);
	
	if(usb_txpose==usb_txposs)
	{
		usb_txpose=0;
		usb_txposs=0;
	}
	
	l=len+3;
	memset(usb_hidmsg, 0, 64);
	usb_hidmsg[0]=0xC3;
	usb_hidmsg[1]=18;
	usb_hidmsg[2]=1;
	usb_hidmsg[3]=0x10;
	usb_hidmsg[4]=0x01;

	usb_hidmsg[18]=0x01;
	
	k=~TKUSB_Crc16(usb_hidmsg+1, len);
	usb_hidmsg[len+1]=k;
	usb_hidmsg[len+2]=k>>8;
	
	usb_hidmsgsz=l;
}

int usb_issetup;

int usb_parsesetup()
{
	printf("usb_parsesetup: %02X %02X %02X %02X %02X %02X %02X %02X\n",
		usb_rxbuf[(usb_rxposs+1)&0xFF],
		usb_rxbuf[(usb_rxposs+2)&0xFF],
		usb_rxbuf[(usb_rxposs+3)&0xFF],
		usb_rxbuf[(usb_rxposs+4)&0xFF],
		usb_rxbuf[(usb_rxposs+5)&0xFF],
		usb_rxbuf[(usb_rxposs+6)&0xFF],
		usb_rxbuf[(usb_rxposs+7)&0xFF],
		usb_rxbuf[(usb_rxposs+8)&0xFF]);
	
	if(usb_rxbuf[(usb_rxposs+1)&0xFF]==0x00)
	{
		if(usb_rxbuf[(usb_rxposs+2)&0xFF]==0x05)
		{
			printf("usb_parsesetup: Set Address\n");
		}

		if(usb_rxbuf[(usb_rxposs+2)&0xFF]==0x09)
		{
			printf("usb_parsesetup: Set Configuration\n");
		}
	}


	if(usb_rxbuf[(usb_rxposs+1)&0xFF]==0x80)
	{
		if(usb_rxbuf[(usb_rxposs+2)&0xFF]==0x06)
		{
			printf("usb_parsesetup: Get Descriptor\n");
			usb_gethiddesc(usb_rxbuf[(usb_rxposs+7)&0xFF]);
		}
	}

	if(usb_rxbuf[(usb_rxposs+1)&0xFF]==0x1A)
	{
		if(usb_rxbuf[(usb_rxposs+2)&0xFF]==0x01)
		{
			memcpy(usb_hidmsg, usb_kbmsg, usb_kbmsgsz);
			usb_hidmsgsz=usb_kbmsgsz;
//			usb_hidkbreport();
		}
	}
	
	usb_txbuf[usb_txpose]=0xD2;
	usb_txpose=(usb_txpose+1)&255;

	usb_txbuf[usb_txpose]=0x100;
	usb_txpose=(usb_txpose+1)&255;

	return(0);
}

byte TKUSB_Crc5(uint32_t v, int vl, int ival)
{
	static const byte bvtab[19] = {
		0x1E, 0x15, 0x03, 0x06, 0x0C, 0x18, 0x19, 0x1B,
		0x1F, 0x17, 0x07, 0x0E, 0x1C, 0x11, 0x0B, 0x16,
		0x05, 0x0A, 0x14
	};
	byte rv;
	int i;

	rv = ival;
	for(i=0; i<vl; i++)
	{
		if (v&(1<<i))
			{ rv^=bvtab[19-vl+i]; }
	}
	return(rv);
}

byte TKUSB_Crc5_11b(uint16_t val)
	{ return(TKUSB_Crc5(val, 11, 0x02)); }
byte TKUSB_Crc5_19b(uint32_t val)
	{ return(TKUSB_Crc5(val, 19, 0x1D)); }


uint16_t TKUSB_DoCrc16Step(uint16_t crc0, byte value)
{
	uint16_t crc;
	int i;
	crc=crc0^(((uint16_t)value)<<8);
	for(i=0; i<8; i++)
	{
		if(crc&0x8000)
//			{ crc=(crc<<1)^0x1021; }
			{ crc=(crc<<1)^0x8005; }
		else
			{ crc=crc<<1; }
	}
	return(crc);
}

uint16_t TKUSB_Crc16(byte *data, uint32_t sz)
{
	byte *cs, *cse;
	uint16_t crc;

	crc=0xFFFF;
	cs=data; cse=data+sz;
	while (cs<cse)
		{ crc=TKUSB_DoCrc16Step(crc, *cs++); }
	return(crc);
}

byte TKUSB_BitFlip8(byte v)
{
	static const byte fliptab[16]={
		0x0, 0x8, 0x4, 0xC, 0x2, 0xA, 0x6, 0xE,
		0x1, 0x9, 0x5, 0xD, 0x3, 0xB, 0x7, 0xF};
	byte v1;
	v1=(fliptab[v&15]<<4)|(fliptab[(v>>4)&15]);
	return(v1);
}

int usb_parsedata()
{
	int i, j, k, l;

	l=usb_rxpose-usb_rxposs;
	l=(l-4)&255;
	j=0xFFFF;
	for(i=0; i<l; i++)
	{
//		j=TKUSB_DoCrc16Step(j, usb_rxbuf[(usb_rxposs+i+1)&0xFF]);
		j=TKUSB_DoCrc16Step(j,
			TKUSB_BitFlip8(usb_rxbuf[(usb_rxposs+i+1)&0xFF]));
	}
	j=j^0xFFFF;
	
	k=	(usb_rxbuf[(usb_rxpose-2)&0xFF]<<8) |
		usb_rxbuf[(usb_rxpose-3)&0xFF];

	if(j!=k)
	{
		printf("usb_parsedata: CRC Problem: %04X %04X\n", j, k);
	}else
	{
		printf("usb_parsedata: CRC OK: %04X\n", j);
	}

	if(usb_issetup)
	{
		usb_parsesetup();
		usb_issetup=0;
		return(0);
	}

	printf("usb_parsedata: Unhandled Data\n", j);

	return(0);
}

int usb_parserxeop()
{
	int tg, k;

	printf("usb_parserxeop\n");

	if(usb_rxbuf[usb_rxposs]==0x69)
	{
		tg=	(usb_rxbuf[(usb_rxposs+1)&0xFF]   )	|
			(usb_rxbuf[(usb_rxposs+2)&0xFF]<<8) ;
		k=TKUSB_Crc5_11b(tg&0x07FF);
		if((tg>>11)!=k)
		{
			printf("usb_parserxeop: CRC5 Bad %04X\n", tg);
			usb_rxposs=usb_rxpose;
			return(0);
		}
		
//		if(usb_rxbuf[(usb_rxposs+1)&0xFF]==0x01)
		if(	(usb_rxbuf[(usb_rxposs+1)&0xFF]==0x80) ||
			(usb_rxbuf[(usb_rxposs+1)&0xFF]==0x81)	)
		{
			usb_hidkbreport();
		}

//		if(usb_rxbuf[(usb_rxposs+1)&0xFF]==0x01)
		if(	(usb_rxbuf[(usb_rxposs+1)&0xFF]==0x00) ||
			(usb_rxbuf[(usb_rxposs+1)&0xFF]==0x01)	)
		{
			usb_hidctrlreport();
		}
	}else
		if(usb_rxbuf[usb_rxposs]==0x2D)
	{
		tg=	(usb_rxbuf[(usb_rxposs+1)&0xFF]   )	|
			(usb_rxbuf[(usb_rxposs+2)&0xFF]<<8) ;
		k=TKUSB_Crc5_11b(tg&0x07FF);
		if((tg>>11)!=k)
		{
			printf("usb_parserxeop: CRC5 Bad %04X\n", tg);
			usb_rxposs=usb_rxpose;
			return(0);
		}

//		usb_parsesetup();
		usb_issetup=1;
	}else
		if(	(usb_rxbuf[usb_rxposs]==0x4B) ||
			(usb_rxbuf[usb_rxposs]==0xC3))
	{
		usb_parsedata();
	}else
		if(usb_rxbuf[usb_rxposs]==0xD2)
	{
		/* ACK */
	}
	else
	{
		printf("usb_parserxeop: unhandled %02X\n",
			usb_rxbuf[usb_rxposs]);
	}
	
	usb_rxposs=usb_rxpose;
}

int update_usb()
{
	static short xmit, xmit0;
	static byte xmitpos, xmitmd, xmitmd0, xmitpos0;
	static byte xmitclk_val;
	static int xmitclk_cnt;
	static byte xmit_upd, txwin, txwin0;
	static byte tclk, tdat, lclk, tlclk, tdatl;
	static byte b, pb, clkr, lclkr;
	static byte rxdat, rxldat, rxbit, rxldat2;
	static byte rxclk, rxclkl, rxmd, rxby, didsb;
	static int rxwin1, rxwin2, rxacc;

	static long acc_clk100, acc_clka, acc_clkb;

	if(top->clock_100==tlclk)
		return(0);
	
	if(!top->clock_100)
	{
		tlclk=top->clock_100;
		top->usb_clkdat_i=tdat;
		return(0);
	}

	acc_clk100++;

	clkr = top->usb_clkref&1;
	if(clkr && !lclkr)
		acc_clkb++;
	
	lclkr = clkr;

#if 0
	if(!(acc_clk100&0xFFFFF))
	{
		printf("USB Clk: A=%.3fMHz B=%.3fMHz\n",
			(100.0*acc_clka)/acc_clk100,
			(100.0*acc_clkb)/acc_clk100);
	}
#endif

	rxdat=top->usb_clkdat_o;
	if(rxdat!=rxldat)
	{
		printf("update_usb: RX State %d, win2=%04X\n", rxdat, rxwin2);
		rxldat=rxdat;
		rxclk=0;
		rxacc=0;
	}
	
//	rxclk=!(((rxacc-16384)>>15)&1);
	rxclk=!(((rxacc-8192)>>15)&1);
	
//	if(rxclk==64)
	if(rxclk && !rxclkl)
	{
		acc_clka++;
	
		rxbit=(rxdat!=rxldat2);
		rxldat2=rxdat;
		
		didsb=0;
		
		rxwin1=(rxwin1>>1)|(rxbit<<15);
		rxwin2=(rxwin2>>1)|(rxbit<<15);

		if(rxdat==0)
		{
			rxwin1=0x55;
			rxwin2=0x55;
		}
		
		if((rxwin1&0xFE00)==0x8000)
		{
			printf("update_usb: RX Bit Stuff\n");
			/* Stuffed Bit */
			rxwin2=(rxwin2<<1)&0xFFFF;
			didsb=1;
		}else if((rxwin1&0xFE00)==0x0000)
		{
			if(rxmd && rxdat)
				printf("update_usb: Missing Stuffed Bit\n");
		}
		
		if(rxdat)
		{
			printf("update_usb: RX Bit %d, win1=%04X win2=%04X md=%X\n",
				rxdat, rxwin1, rxwin2, rxmd);
		}
	
		if(!rxmd)
		{
//			if((rxwin2&0xFF80)==0x7F80)
			if((rxwin2&0xFF00)==0x7F00)
			{
				printf("update_usb: Sync\n");
				rxmd=8;
			}
		}else if((rxmd>=8) && (rxmd<16))
		{
			if(!didsb)
				rxmd++;

			if(rxmd==16)
			{
				rxby=rxwin2>>8;
				rxby=~rxby;
				printf("update_usb: rxby=%02X\n", rxby);
				usb_rxbuf[usb_rxpose]=rxby;
				usb_rxpose=(usb_rxpose+1)&255;
				rxmd=8;
			}
			
			if(rxdat==0)
			{
				printf("update_usb: EOP\n");
				usb_rxbuf[usb_rxpose]=0x100;
				usb_rxpose=(usb_rxpose+1)&255;
				rxmd=0;
				rxwin1=0x5555;
				rxwin2=0x5555;
				usb_parserxeop();
			}
		}else
		{
			printf("update_usb: Bad Mode\n");
			rxmd=0;
		}
	}
//	rxclk+=4;

	if(xmitpos<=0)
	{
		if(usb_txposs!=usb_txpose)
		{
//			printf("update_usb: TX Non-Empty, %X..%X\n",
//				usb_txposs, usb_txpose);
		}

//		xmitclk_cnt--;
//		if(xmitclk_cnt<=0)
		if(rxclk && !rxclkl)
		{
//			if(!xmitclk_val)
			if(0)
			{
//				tclk=1;
//				tdat=1;

				xmitclk_val=1;
				xmitclk_cnt=33;
			}else
			{
				if(usb_txposs!=usb_txpose)
				{
					xmit=usb_txbuf[usb_txposs];
//					if(xmitmd&0x30)
						usb_txposs=(usb_txposs+1)&255;
					xmitpos=8;
					
					printf("update_usb: send=%02X\n", xmit);
					xmit^=0xFF;
				}else if(xmitmd&0x30)
				{
					xmit=0x100;
					xmitpos=8;
					printf("update_usb: send EOP\n", xmit);
				}

//				tclk=1;
//				tdat=1;

				xmitclk_val=1;
				xmitclk_cnt=33;
				xmit_upd=1;
			}
		}

		if(xmitmd==0)
		{
			tdat=2;		/* Low Speed */
		}

		tdatl=tdat;
	}else
	{
//		printf("update_usb: Bit-Active, xmit=%02X pos=%d md=%X\n",
//			xmit, xmitpos, xmitmd);

		xmitpos0=xmitpos;
		xmitmd0=xmitmd;
		xmit0=xmit;
		txwin0=txwin;
		
//		if(xmit_upd)
		if(1)
		{
			switch(xmitpos)
			{
				case  8: b=(xmit>>0)&1; break;
				case  7: b=(xmit>>1)&1; break;
				case  6: b=(xmit>>2)&1; break;
				case  5: b=(xmit>>3)&1; break;
				case  4: b=(xmit>>4)&1; break;
				case  3: b=(xmit>>5)&1; break;
				case  2: b=(xmit>>6)&1; break;
				case  1: b=(xmit>>7)&1; break;
			}
//			b=!b;
			
//			tdat=b;
			xmit_upd=0;
		}

//		xmitclk_cnt--;
//		if(xmitclk_cnt<=0)
		if(rxclk && !rxclkl)
		{
			xmitclk_val=!xmitclk_val;
			xmitclk_cnt=67;
			tclk=xmitclk_val;
			xmit_upd=1;

//			txwin=(txwin>>1)|(b<<7);
			txwin=(txwin0>>1)|(b<<7);
			
			if(!(xmitmd&0x30))
			{
//				if(xmitmd<8)
				if(xmitmd<7)
//				if(xmitmd<10)
				{
					b=1;
//					if(!xmitclk_val)
					xmitmd++;
					txwin=0x55;
				}else
				{
					b=0;
//					if(!xmitclk_val)
					xmitmd=0x10;
				}

				txwin=(txwin0>>1)|(b<<7);
			}
//			else if((txwin&0xFC)==0x00)
			else if((txwin&0xFE)==0x00)
			{
				printf("update_usb: TX Bit Stuff\n");

				/* bit stuff */
				txwin|=0x80;
				b=1;
			}else
			{
				xmitpos--;
				xmit_upd=0;
				
				if((xmitpos<=0) && (usb_txposs!=usb_txpose) && !(xmit&0x100))
				{
					xmit=usb_txbuf[usb_txposs];
					usb_txposs=(usb_txposs+1)&255;
					xmitpos=8;
					printf("update_usb: B-send=%02X\n", xmit);
					xmit^=0xFF;
				}
			}
			
			if(b)
			{
				if(tdatl==1)
					tdat=2;
				if(tdatl==2)
					tdat=1;
//				tdatl=tdat;
			}else
			{
				tdat=tdatl;
			}
			
			if(xmit0&0x100)
			{
				printf("update_usb: EOP Byte\n");
				tdat=0;
				if(xmitpos<=0)
				{
					xmitmd=0;
					txwin=0x55;
				}
			}
			
//			if(!xmitclk_val)
//			txwin=(txwin>>1)|(b<<7);
			
			printf("update_usb: Send Bit %d %X->%X pos=%d->%d md=%02X->%02X\n",
				b,
				tdatl, tdat,
				xmitpos0, xmitpos,
				xmitmd0, xmitmd);
		}

		tdatl=tdat;
	}


	rxclkl=rxclk;
	rxacc+=983;

	tlclk=top->clock_100;
	top->usb_clkdat_i=tdat;
	
	if(tclk!=lclk)
	{
		lclk=tclk;
	}
}

int update_ps2kb()
{
	static byte xmit;
	static byte xmitpos;
	static byte xmitclk_val;
	static int xmitclk_cnt;
	static byte xmit_upd;
	static byte tclk, tdat, lclk, tlclk;
	static byte b, pb;
	
	update_uart();
	update_usb();
	
//	if(top->clock_50 && !tlclk)
	if(top->clock_100 && !tlclk)
//	if(!top->clock_100 && tlclk)
	{
		if(xmitpos<=0)
		{
			xmitclk_cnt--;
			if(xmitclk_cnt<=0)
			{
				if(!xmitclk_val)
				{
					tclk=1;
					tdat=1;

					xmitclk_val=1;
//					xmitclk_cnt=6250;
					xmitclk_cnt=625;
				}else
				{
					if(ps2kbirov!=ps2kbrov)
					{
						xmit=ps2kbbuf[ps2kbirov];
						ps2kbirov=(ps2kbirov+1)&255;
						xmitpos=11;
						
						printf("update_ps2kb: scan=%02X\n", xmit);
					}

			//		top->ps2_clk_i=1;
			//		top->ps2_data_i=1;
					tclk=1;
					tdat=1;

					xmitclk_val=1;
//					xmitclk_cnt=6250;
					xmitclk_cnt=625;
					xmit_upd=1;
				}
			}
		}else
		{
			if(xmit_upd)
			{
				switch(xmitpos)
				{
					case 11: b=0; pb=1; break;
					case 10: b=(xmit>>0)&1; break;
					case  9: b=(xmit>>1)&1; break;
					case  8: b=(xmit>>2)&1; break;
					case  7: b=(xmit>>3)&1; break;
					case  6: b=(xmit>>4)&1; break;
					case  5: b=(xmit>>5)&1; break;
					case  4: b=(xmit>>6)&1; break;
					case  3: b=(xmit>>7)&1; break;
					case  2: b=pb; break;
					case  1: b=1; break;
				}
	//			top->ps2_data_i=b;
				tdat=b;
				xmit_upd=0;
			}

			xmitclk_cnt--;
			if(xmitclk_cnt<=0)
			{
				xmitclk_val=!xmitclk_val;
//				xmitclk_cnt=6250;
				xmitclk_cnt=625;
	//			top->ps2_clk_i=xmitclk_val;
				tclk=xmitclk_val;
				xmit_upd=1;

	//			if(xmitclk_val)
				if(!xmitclk_val)
				{
					pb=pb^b;
					xmitpos--;
					xmit_upd=0;
//					tdat=b;
				}else
				{
					tdat=b;
				}
				
	//			printf("update_ps2kb: clk=%d data=%d\n",
	//				top->ps2_clk_i, top->ps2_data_i);
	//				tclk, tdat);
			}
		}
	}

	tlclk=top->clock_100;
//	tlclk=top->clock_50;

	top->ps2kb_clk_i=tclk;
	top->ps2kb_data_i=tdat;
	
	if(tclk!=lclk)
	{
//		printf("update_ps2kb: clk=%d data=%d\n",
//			tclk, tdat);
		lclk=tclk;
	}
}

int update_uart()
{
	static byte tclk, tlclk, pos, bit, bval;
	static int frac;

	tclk=top->clock_100;
	if(!tclk || tlclk)
	{
		tlclk=tclk;
		return(0);
	}

	if(tclk==tlclk)
		return(0);
	tlclk=tclk;

	bit=top->uartTxD;
	
	if(!pos)
	{
		if(!bit)
		{
			pos=9;
			frac=100000000/115200;
			frac+=(frac>>2);
		}
	}
	
	if(frac>0)
	{
		frac--;
		return(0);
	}else if(pos>0)
	{
		frac=100000000/115200;
		pos--;
		bval=(bval>>1)|(bit<<7);
		
		if(pos==1)
		{
//			printf("%02X ", bval);
			if((bval>0) && (bval<127))
			{
				fputc(bval, stdout);
				fflush(stdout);
			}
		}
	}

	return(0);
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

int sim_fb_drawbox(uint32_t *fbuf, int xs, int ys,
	int x0, int y0, int x1, int y1, uint32_t clr)
{
	int lx, ly, tx, ty;
	
	if(y0>y1)
		{ tx=y0; y0=y1; y1=tx; }
	if(x0>x1)
		{ tx=x0; x0=x1; x1=tx; }
	
	for(ly=y0; ly<=y1; ly++)
	{
		for(lx=x0; lx<=x1; lx++)
		{
			fbuf[ly*xs+lx]=clr;
		}
	}
	return(0);
}

int sim_fb_draw7seg(uint32_t *fbuf, int xs, int ys,
	int led_x, int led_y, int led_r,
	uint32_t led_clo, uint32_t led_chi, int mask)
{
	int lw;
	
	lw=led_r>>2;
	
	/* Top */
	sim_fb_drawbox(
		fbuf, xs, ys,
		led_x+lw, led_y-led_r*2, led_x+led_r, led_y-led_r*2-lw,
		(mask&1)?led_chi:led_clo);

	/* Top-Right */
	sim_fb_drawbox(
		fbuf, xs, ys,
		led_x+led_r, led_y-led_r*2, led_x+led_r+lw, led_y-led_r-lw,
		(mask&2)?led_chi:led_clo);

	/* Lower-Right */
	sim_fb_drawbox(
		fbuf, xs, ys,
		led_x+led_r, led_y-led_r, led_x+led_r+lw, led_y-lw,
		(mask&4)?led_chi:led_clo);
	
	/* Bottom */
	sim_fb_drawbox(
		fbuf, xs, ys,
		led_x+lw, led_y-lw, led_x+led_r, led_y,
		(mask&8)?led_chi:led_clo);

	/* Top-Left */
	sim_fb_drawbox(
		fbuf, xs, ys,
		led_x, led_y-led_r*2, led_x+lw, led_y-led_r-lw,
		(mask&0x20)?led_chi:led_clo);

	/* Lower-Left */
	sim_fb_drawbox(
		fbuf, xs, ys,
		led_x, led_y-led_r, led_x+lw, led_y-lw,
		(mask&0x10)?led_chi:led_clo);

	/* Middle */
	sim_fb_drawbox(
		fbuf, xs, ys,
		led_x+lw, led_y-led_r, led_x+led_r, led_y-led_r-lw,
		(mask&0x40)?led_chi:led_clo);
}

int sim_fb_draw7seg_8x(uint32_t *fbuf, int xs, int ys,
	int led_x, int led_y, int led_r,
	uint32_t led_clo, uint32_t led_chi,
	int mask1, int mask2)
{
	int i;
	
	for(i=0; i<8; i++)
	{
		sim_fb_draw7seg(fbuf, xs, ys,
			led_x+i*led_r*2, led_y, led_r, led_clo, led_chi,
			!(mask2&(1<<i))?(~mask1):0);
	}
}

int BTM_DrawGlyphFBuf(uint32_t *fbuf, int ystr, int cx, int cy,
	u64 pix, uint32_t c1, uint32_t c0)
{
	uint32_t *ct;
	int y, p1;
	
	ct=fbuf+(cy*ystr)+cx;
	for(y=0; y<8; y++)
	{
		p1=(pix>>((7-y)*8))&255;

		if(p1&0x80)		{ ct[0]=c1; }
		else			{ ct[0]=c0; }
		if(p1&0x40)		{ ct[1]=c1; }
		else			{ ct[1]=c0; }
		if(p1&0x20)		{ ct[2]=c1; }
		else			{ ct[2]=c0; }
		if(p1&0x10)		{ ct[3]=c1; }
		else			{ ct[3]=c0; }

		if(p1&0x08)		{ ct[4]=c1; }
		else			{ ct[4]=c0; }
		if(p1&0x04)		{ ct[5]=c1; }
		else			{ ct[5]=c0; }
		if(p1&0x02)		{ ct[6]=c1; }
		else			{ ct[6]=c0; }
		if(p1&0x01)		{ ct[7]=c1; }
		else			{ ct[7]=c0; }

		ct+=ystr;
	}
	return(0);
}

int BTM_DrawGlyphFBuf2x(uint32_t *fbuf, int ystr, int cx, int cy,
	u64 pix, uint32_t c1, uint32_t c0)
{
	uint32_t *ct0, *ct1;
	int y, p1;
	
	ct0=fbuf+(cy*ystr)+cx;
	ct1=ct0+ystr;
	for(y=0; y<8; y++)
	{
		p1=(pix>>((7-y)*8))&255;

		if(p1&0x80)		{ ct0[0]=c1; ct0[1]=c1; ct1[0]=c1; ct1[1]=c1; }
		else			{ ct0[0]=c0; ct0[1]=c0; ct1[0]=c0; ct1[1]=c0; }
		if(p1&0x40)		{ ct0[2]=c1; ct0[3]=c1; ct1[2]=c1; ct1[3]=c1; }
		else			{ ct0[2]=c0; ct0[3]=c0; ct1[2]=c0; ct1[3]=c0; }
		if(p1&0x20)		{ ct0[4]=c1; ct0[5]=c1; ct1[4]=c1; ct1[5]=c1; }
		else			{ ct0[4]=c0; ct0[5]=c0; ct1[4]=c0; ct1[5]=c0; }
		if(p1&0x10)		{ ct0[6]=c1; ct0[7]=c1; ct1[6]=c1; ct1[7]=c1; }
		else			{ ct0[6]=c0; ct0[7]=c0; ct1[6]=c0; ct1[7]=c0; }

		if(p1&0x08)		{ ct0[ 8]=c1; ct0[ 9]=c1; ct1[ 8]=c1; ct1[ 9]=c1; }
		else			{ ct0[ 8]=c0; ct0[ 9]=c0; ct1[ 8]=c0; ct1[ 9]=c0; }
		if(p1&0x04)		{ ct0[10]=c1; ct0[11]=c1; ct1[10]=c1; ct1[11]=c1; }
		else			{ ct0[10]=c0; ct0[11]=c0; ct1[10]=c0; ct1[11]=c0; }
		if(p1&0x02)		{ ct0[12]=c1; ct0[13]=c1; ct1[12]=c1; ct1[13]=c1; }
		else			{ ct0[12]=c0; ct0[13]=c0; ct1[12]=c0; ct1[13]=c0; }
		if(p1&0x01)		{ ct0[14]=c1; ct0[15]=c1; ct1[14]=c1; ct1[15]=c1; }
		else			{ ct0[14]=c0; ct0[15]=c0; ct1[14]=c0; ct1[15]=c0; }

		ct0+=2*ystr;
		ct1+=2*ystr;
	}
	return(0);
}

int BTM_DrawCharFBuf(uint32_t *fbuf, int ystr, int x, int y,
	byte ch, uint32_t c1, uint32_t c0)
{
	BTM_DrawGlyphFBuf(fbuf, ystr, x, y, btesh2_gfxcon_glyphs[ch], c1, c0);
	return(0);
}

int BTM_DrawStrFBuf(uint32_t *fbuf, int ystr, int x, int y,
	char *str, uint32_t c1, uint32_t c0)
{
	char *s;
	int ch;
	
	s=str;
	while(*s)
	{
		ch=*s++;
		BTM_DrawGlyphFBuf(fbuf, ystr, x, y, btesh2_gfxcon_glyphs[ch], c1, c0);
		x+=8;
	}
	return(0);
}

int BTM_DrawStrFBuf2x(uint32_t *fbuf, int ystr, int x, int y,
	char *str, uint32_t c1, uint32_t c0)
{
	char *s;
	int ch;
	
	s=str;
	while(*s)
	{
		ch=*s++;
		BTM_DrawGlyphFBuf2x(fbuf, ystr, x, y, btesh2_gfxcon_glyphs[ch], c1, c0);
		x+=16;
	}
	return(0);
}

int BTM_DrawDecimalFBuf(uint32_t *fbuf, int ystr, int x, int y,
	uint64_t val, int n, uint32_t c1, uint32_t c0)
{
	char tb[64];
	uint64_t v;
	int i, j, k;
	
	v=val;
	for(i=0; i<n; i++)
	{
		tb[n-i-1]='0'+(v%10);
		v/=10;
	}
	tb[n]=0;
//	BTM_DrawStrFBuf(fbuf, ystr, x, y, tb, c1, c0);
	BTM_DrawStrFBuf2x(fbuf, ystr, x, y, tb, c1, c0);
	return(0);
}

int main(int argc, char **argv, char **env)
{
	char tb[256];
	BJX2_Context *ctx;
	cdec_imgbuf *vgactx;
	FILE *fd;
	int lclk, mhz;
	int sdc_lclk, sdc_lbit, sdc_cbit;
	int tt_start, tt_frame, tt_reset;
	int cnt_dled, cnt_h1, cnt_h2,
		cnt_d1, cnt_d2, cnt_d3, cnt_d4,
		cnt_d5, cnt_d6, cnt_d7, cnt_d8;
	int clk300, lclk300, clk150, lclk150, clk75;
	uint64_t rvq;
	int t0, t1, t2;
	int i, j, k;

	mhz=100;
	do_qmt=0;

	for(i=1; i<argc; i++)
	{
		if(!strcmp(argv[i], "--qmt"))
			do_qmt=1;
	}

	Verilated::commandArgs(argc, argv);

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
//		(char *)"BOOTLOAD.SYS",
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

	JX2R_UseImageAddFile(
		(char *)"ID1/AUTOEXEC.CFG",
		(char *)"../../tk_qsrc/id1/autoexec1.cfg");
#endif

#if 1
	JX2R_UseImageAddFile(
		(char *)"HTIC/HTIC.EXE",
		(char *)"../../tk_ports/HticSrc/htic_bjx2.exe");
	JX2R_UseImageAddFile(
		(char *)"HTIC/HTICULZ.WAD",
		(char *)"../../tk_ports/HticSrc/hticulz.wad");
#endif

#ifndef NOSWAP
	JX2R_UseImageAddFileBuffer("swapfile.sys", (byte *)NULL, 384*(1<<20));
#endif

	rombuf=(uint32_t *)malloc(32768);
//	srambuf=(uint32_t *)malloc(8192);
//	drambuf=(uint32_t *)malloc(1<<27);
//	srambuf2=(uint32_t *)malloc(8192);
//	drambuf2=(uint32_t *)malloc(1<<27);	

//	memset(srambuf, 0, 8192);
//	memset(drambuf, 0, 1<<27);
//	memset(srambuf2, 0, 8192);
//	memset(drambuf2, 0, 1<<27);

//	ddr_ram=(uint16_t *)malloc(1<<27);
//	ddr_ram=(uint16_t *)malloc(1<<28);

#if 0
	fd=fopen("../../tst_jx2boot.bin", "rb");
	if(fd)
	{
//		t2=fread(rombuf, 1, 32768, fd);
		fclose(fd);
	}
#endif

//	fd=NULL;

	btesh2_gfxcon_fbxs = 800;
	btesh2_gfxcon_fbys = 600;

	GfxDrv_Start();
	SoundDev_Init();
	BTSR1_MainInitKeyboard();

	BTSR1_MainAddUsbKey(0);

	ddr_ram=(uint16_t *)malloc(1<<28);
	memset(ddr_ram, 0, 1<<28);
	
//	for(i=0; i<(1<<26); i++)
//		ddr_ram[i]=rand();

	ctx=(BJX2_Context *)malloc(sizeof(BJX2_Context)+256);
	memset(ctx, 0, sizeof(BJX2_Context));
//	jx2_ctx=ctx;

	vgactx=(cdec_imgbuf *)malloc(sizeof(cdec_imgbuf)+256);
	memset(vgactx, 0, sizeof(cdec_imgbuf));

	CDEC_SetupForStream(vgactx);

	ctx->tgt_mhz=mhz;
	ctx->rcp_mhz=((1048576/ctx->tgt_mhz)+7)>>4;
	ctx->ttick_rst=(ctx->tgt_mhz*1000000)/1024;
	ctx->ttick_hk=ctx->ttick_rst;

	printf("Start CoreUnit\n");

//	btesh2_gfxcon_fbxs = 800;
//	btesh2_gfxcon_fbys = 600;

//	GfxDrv_Start();
//	SoundDev_Init();

//	BTSR1_MainInitKeyboard();

	tt_start=FRGL_TimeMS();
	tt_frame=tt_start;
	tt_reset=1024;

	top->ddrModeIn=0;
	if(do_qmt)
		top->ddrModeIn=1;

//	ddr_is_ddr3 = 0;
	ddr_is_ddr3 = top->ddrModeIn&1;

	while (!Verilated::gotFinish())
	{
		t1=FRGL_TimeMS();
		t2=t1-tt_frame;

		cnt_dled++;
		cnt_h1+=(top->dbg_exHold1!=0);
		cnt_h2+=(top->dbg_exHold2!=0);
		cnt_d1+=(top->dbg_outStatus1!=0);
		cnt_d2+=(top->dbg_outStatus2!=0);
		cnt_d3+=(top->dbg_outStatus3!=0);
		cnt_d4+=(top->dbg_outStatus4!=0);
		cnt_d5+=(top->dbg_outStatus5!=0);
		cnt_d6+=(top->dbg_outStatus6!=0);
		cnt_d7+=(top->dbg_outStatus7!=0);
		cnt_d8+=(top->dbg_outStatus8!=0);

//		if(t2>16)
//		if((t2>16) && (cnt_dled>8))
		if((t2>64) && (cnt_dled>32))
		{
			BTSR1_MainPollKeyboard();

//			jx2i_gfxcon_dirty=1;
//			JX2I_GfxCon_Update();

			GfxDrv_PrepareFramebuf();
			
			cdec_getimage(vgactx, btesh2_gfxcon_framebuf, 800, 600);

			cnt_h1=(256*cnt_h1)/cnt_dled;
			cnt_h2=(256*cnt_h2)/cnt_dled;
			cnt_d1=(256*cnt_d1)/cnt_dled;
			cnt_d2=(256*cnt_d2)/cnt_dled;
			cnt_d3=(256*cnt_d3)/cnt_dled;
			cnt_d4=(256*cnt_d4)/cnt_dled;
			cnt_d5=(256*cnt_d5)/cnt_dled;
			cnt_d6=(256*cnt_d6)/cnt_dled;
			cnt_d7=(256*cnt_d7)/cnt_dled;
			cnt_d8=(256*cnt_d8)/cnt_dled;
			
			cnt_h1=0xFF000000+(cnt_h1<<16)+(cnt_h1<<8);
			cnt_h2=0xFF000000+(cnt_h2<<16)+(cnt_h2<<8);

			cnt_d1=0xFF000000+(cnt_d1<<16)+(cnt_d1<<8);
			cnt_d2=0xFF000000+(cnt_d2<<16)+(cnt_d2<<8);
			cnt_d3=0xFF000000+(cnt_d3<<16)+(cnt_d3<<8);
			cnt_d4=0xFF000000+(cnt_d4<<16)+(cnt_d4<<8);
			cnt_d5=0xFF000000+(cnt_d5<<16)+(cnt_d5<<8);
			cnt_d6=0xFF000000+(cnt_d6<<16)+(cnt_d6<<8);
			cnt_d7=0xFF000000+(cnt_d7<<16)+(cnt_d7<<8);
			cnt_d8=0xFF000000+(cnt_d8<<16)+(cnt_d8<<8);

			sim_fb_drawled(
				(uint32_t *)btesh2_gfxcon_framebuf, 800, 600,
//				1*16, 600-16, 6, top->dbg_exHold1?0xFFFFFF00:0xFF000000);
				1*16, 600-16, 6, cnt_h1);
			sim_fb_drawled(
				(uint32_t *)btesh2_gfxcon_framebuf, 800, 600,
//				2*16, 600-16, 6, top->dbg_exHold2?0xFFFFFF00:0xFF000000);
				2*16, 600-16, 6, cnt_h2);

			sim_fb_drawled(
				(uint32_t *)btesh2_gfxcon_framebuf, 800, 600,
//				4*16, 600-16, 6, top->dbg_outStatus1?0xFFFFFF00:0xFF000000);
				4*16, 600-16, 6, cnt_d1);
			sim_fb_drawled(
				(uint32_t *)btesh2_gfxcon_framebuf, 800, 600,
//				5*16, 600-16, 6, top->dbg_outStatus2?0xFFFFFF00:0xFF000000);
				5*16, 600-16, 6, cnt_d2);
			sim_fb_drawled(
				(uint32_t *)btesh2_gfxcon_framebuf, 800, 600,
//				6*16, 600-16, 6, top->dbg_outStatus3?0xFFFFFF00:0xFF000000);
				6*16, 600-16, 6, cnt_d3);
			sim_fb_drawled(
				(uint32_t *)btesh2_gfxcon_framebuf, 800, 600,
//				7*16, 600-16, 6, top->dbg_outStatus4?0xFFFFFF00:0xFF000000);
				7*16, 600-16, 6, cnt_d4);

			sim_fb_drawled(
				(uint32_t *)btesh2_gfxcon_framebuf, 800, 600,
//				9*16, 600-16, 6, top->dbg_outStatus5?0xFFFFFF00:0xFF000000);
				9*16, 600-16, 6, cnt_d5);
			sim_fb_drawled(
				(uint32_t *)btesh2_gfxcon_framebuf, 800, 600,
//				10*16, 600-16, 6, top->dbg_outStatus6?0xFFFFFF00:0xFF000000);
				10*16, 600-16, 6, cnt_d6);
			sim_fb_drawled(
				(uint32_t *)btesh2_gfxcon_framebuf, 800, 600,
//				11*16, 600-16, 6, top->dbg_outStatus7?0xFFFFFF00:0xFF000000);
				11*16, 600-16, 6, cnt_d7);
			sim_fb_drawled(
				(uint32_t *)btesh2_gfxcon_framebuf, 800, 600,
//				12*16, 600-16, 6, top->dbg_outStatus8?0xFFFFFF00:0xFF000000);
				12*16, 600-16, 6, cnt_d8);

			sim_fb_drawled(
				(uint32_t *)btesh2_gfxcon_framebuf, 800, 600,
				14*16, 600-16, 6, top->aud_mono_out?0xFFFFFF00:0xFF000000);
//				12*16, 600-16, 6, cnt_d8);

			sim_fb_drawled(
				(uint32_t *)btesh2_gfxcon_framebuf, 800, 600,
				16*16, 600-16, 8, (tt_reset>0)?0xFFFF0000:0xFF000000);
//				12*16, 600-16, 6, cnt_d8);

			sim_fb_draw7seg_8x(
				(uint32_t *)btesh2_gfxcon_framebuf, 800, 600,
//				14*16, 600-16, 12, 0xFF000000, 0xFFFFFF00,
				18*16, 600-16, 12, 0xFF000000, 0xFFFFFF00,
				top->seg_outSegBit,
				top->seg_outCharBit
				);
			
			rvq=(ctx->tot_cyc*ctx->rcp_mhz)>>16;
//			BTM_DrawDecimalFBuf(
//				(uint32_t *)btesh2_gfxcon_framebuf, 800,
//				2*8, 600-48, rvq, 12, 0xFFFFFF80, 0xFF008000);

			sprintf(tb, "%06d.%06d",
				(int)(rvq/1000000),
				(int)(rvq%1000000));

			BTM_DrawStrFBuf2x(
				(uint32_t *)btesh2_gfxcon_framebuf, 800,
				2*8, 600-48, tb, 0xFFFFFF80, 0xFF008000);


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

//		BTSR1_MainPollKeyboard();

		if(btn_reset)
		{
			tt_reset=1024;
		}


		if(tt_reset>0)
		{
			top->reset_200=1;
			top->reset_150=1;
			top->reset_100=1;
			top->reset_75=1;
			top->reset_50=1;
			tt_reset--;
		}else
		{
			top->reset_200=0;
			top->reset_150=0;
			top->reset_100=0;
			top->reset_75=0;
			top->reset_50=0;
		}

#ifdef CLOCK_400MHZ
#if 0
		clk300 = ((main_time*192)>>8)&1;
		if(clk300 && !lclk300)
			clk150 = !clk150;
		lclk300 = clk300;
		if(clk150 && !lclk150)
			clk75 = !clk75;
		lclk150 = clk150;

		top->clock_300 = clk300;
		top->clock_150 = clk150;
		top->clock_75 = clk75;
#endif

		main_time3p=(main_time*192)>>8;
		top->clock_300 = (main_time3p>>0)&1;
		top->clock_150 = (main_time3p>>1)&1;
		top->clock_75  = (main_time3p>>2)&1;

//		top->clock_300 = ((main_time*192)>>8)&1;
		top->clock_200 = (main_time>>1)&1;
//		top->clock_150 = ((main_time*192)>>9)&1;
		top->clock_100 = (main_time>>2)&1;
//		top->clock_75 = ((main_time*96)>>9)&1;
		top->clock_50  = (main_time>>3)&1;
		ctx->tot_cyc=main_time>>3;
#else

#ifdef CLOCK_200MHZ
		top->clock_300 = (main_time>>0)&1;
		top->clock_200 = (main_time>>0)&1;
		top->clock_150 = ((main_time*192)>>8)&1;
		top->clock_100 = (main_time>>1)&1;
		top->clock_75 = ((main_time*96)>>8)&1;
		top->clock_50  = (main_time>>2)&1;
		ctx->tot_cyc=main_time>>2;

// #ifdef CLOCK_200_AS_133
//		top->clock_200 = ((main_time*170)>>8)&1;
// #endif

// #ifdef CLOCK_200_AS_150
//		top->clock_200 = ((main_time*192)>>8)&1;
// #endif

//		top->clock_200 = !top->clock_200;  //Debug: Flip Clock Polarity
#else
		top->clock_300 = (main_time>>0)&1;
		top->clock_200 = (main_time>>0)&1;
		top->clock_150 = (main_time>>0)&1;
		top->clock_100 = (main_time>>0)&1;
		top->clock_75  = (main_time>>1)&1;
		top->clock_50  = (main_time>>1)&1;
		ctx->tot_cyc=main_time>>1;
#endif
#endif
		main_time++;
		top->sdc_dat_i=sdc_cbit;
		
		if(top->clock_100 && (lclk!=top->clock_100))
		{
//			i=(top->vgaRed<<8)|(top->vgaGrn<<4)|(top->vgaBlu<<0)|
//				(top->vgaHsync<<12)|(top->vgaVsync<<13);

			i=((top->vgaRed>>1)<<8)|((top->vgaGrn>>1)<<4)|((top->vgaBlu>>1)<<0)|
				(top->vgaHsync<<12)|(top->vgaVsync<<13);

			CDEC_UpdateForStreamCycle(vgactx, i);

//			printf("Cycle\n");

			ctx->ttick_hk--;
			if(ctx->ttick_hk<=0)
			{
//				printf("Clock IRQ\n");
				ctx->ttick_hk=ctx->ttick_rst;
			}

//			top->sdc_dat_i=sdc_lbit;
			
			if(top->sdc_clk!=sdc_lclk)
			{
				if(top->sdc_clk)
//				if(!top->sdc_clk)
				{
//					top->sdc_dat_i=
					sdc_lbit=
						sdc_spibit(top->sdc_cmd, top->sdc_dat_o&8);
//					top->sdc_dat_i=sdc_lbit;
//					sdc_cbit=sdc_lbit;
				}else
				{
					top->sdc_dat_i=sdc_lbit;
					sdc_cbit=sdc_lbit;
				}

#if 0
				printf("sd-spi: clk=%d mosi=%d miso=%d cs=%d pos=%d\n",
					top->sdc_clk,
					(top->sdc_cmd)&1, (top->sdc_dat_i)&1,
					(top->sdc_dat_o&8)!=0,
					sdc_spipos);
#endif
			
				sdc_lclk=top->sdc_clk;
			}
			
		}

//		if(!top->ps2kb_clk_i)
//			printf("sim: ps2kb_clk_i=%d\n", top->ps2kb_clk_i);

		update_ps2kb();
		
//		ctx->tot_cyc=main_time>>1;
//		ctx->tot_cyc=main_time>>2;
		
		lclk = top->clock_100;
//		top->mode = 3;
		
//		top->baseAddr=0xDECAB00;
//		top->idxAddr=0x100;
//		top->idxDisp=3;

		if(top->clock_100)
		{
//			printf("Cycle %lld\n", jx2_ctx->tot_cyc);
		
//			MemUpdateForBus();
//			if(CheckDebugSanity()<=0)
//				break;
		}

		top->eval();

		update_ddr();

//		main_time++;
		
		if(gfxdrv_kill)
		{
			break;
		}
	}

	t1=FRGL_TimeMS();
	t2=t1-tt_start;
	
	printf("%.3fMHz\n", ctx->tot_cyc/(t2*1000.0));
	
	BTSR1_MainDeinitKeyboard();
	
	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
