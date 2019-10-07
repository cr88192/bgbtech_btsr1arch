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
//		BJX2_MainAddTranslateKey(ctx, k);
		BTSR1_MainAddKey(k);

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


BJX2_Context	*jx2_ctx;

u32 mmgp_data[256];

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
		if(kbirov!=kbrov)
		{
			val=kbbuf[kbirov];
			kbirov=(kbirov+1)&255;
		}
		break;

	case 0xE014:
		val=0; break;

	case 0xE018:
		val=0;
		if(kbirov!=kbrov)
			val|=1;
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
	int v;

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
		v=btesh2_spimmc_XrCtl(ctx, val);

//		printf("SPI XrCtl %02X %02X\n", val, v);

		mmio[0x10]=v&255;
		if(v&0x10000)
			mmio[0x11]=(v>>8)&255;
		break;
	case 0xE034:
		v=btesh2_spimmc_XrByte(ctx, val);
		mmio[0x11]=v;

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

void MemUpdateForBus()
{
	static byte mmio_latched=0;
	byte is_rom, is_sram, is_dram, is_mmio;
	int opm_latch;
	uint32_t addr;

	if(top->memOpm)
	{
		addr=top->memAddr;
		top->memOK=0;
		
		is_rom	= (addr>=0x00000000) && (addr<=0x00007FFF);
		is_sram	= (addr>=0x0000C000) && (addr<=0x0000DFFF);
		is_dram	= (addr>=0x01000000) && (addr<=0x18000000);
//		is_mmio = (addr>=0xA0000000) && (addr<=0xC0000000);
		is_mmio = (addr>=0xF0000000) && (addr<=0xFFFFFFFF);

		if(top->memOpm&0x08)
		{
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
				}
				top->memOK=1;
				
				if((top->memDataIn[0]==0x55AA55AA) || (addr&3))
				{
					top->memDataIn[0]=0xFFFFFFFFU;
					top->memDataIn[1]=0xFFFFFFFFU;
					top->memDataIn[2]=0xFFFFFFFFU;
					top->memDataIn[3]=0xFFFFFFFFU;
					top->memOK=3;
				}
			}else if(is_rom)
			{
				top->memDataIn[0]=rombuf[((addr>>2)+0)&0x1FFF];
				top->memDataIn[1]=rombuf[((addr>>2)+1)&0x1FFF];
				top->memDataIn[2]=rombuf[((addr>>2)+2)&0x1FFF];
				top->memDataIn[3]=rombuf[((addr>>2)+3)&0x1FFF];
				top->memOK=1;
			}else
				if(is_sram)
			{
				top->memDataIn[0]=srambuf[((addr>>2)+0)&0x7FF];
				top->memDataIn[1]=srambuf[((addr>>2)+1)&0x7FF];
				top->memDataIn[2]=srambuf[((addr>>2)+2)&0x7FF];
				top->memDataIn[3]=srambuf[((addr>>2)+3)&0x7FF];
				top->memOK=1;
			}else if(is_dram)
			{
				top->memDataIn[0]=drambuf[((addr>>2)+0)&0x1FFFFFF];
				top->memDataIn[1]=drambuf[((addr>>2)+1)&0x1FFFFFF];
				top->memDataIn[2]=drambuf[((addr>>2)+2)&0x1FFFFFF];
				top->memDataIn[3]=drambuf[((addr>>2)+3)&0x1FFFFFF];
				top->memOK=1;
			}else
			{
				top->memDataIn[0]=0xFFFFFFFFU;
				top->memDataIn[1]=0xFFFFFFFFU;
				top->memDataIn[2]=0xFFFFFFFFU;
				top->memDataIn[3]=0xFFFFFFFFU;
//				top->memOK=1;
				top->memOK=3;
			}

#if 0
			printf("LD  %08X  %08X %08X %08X %08X\n",
				addr,
				top->memDataIn[0], top->memDataIn[1],
				top->memDataIn[2], top->memDataIn[3]);
#endif
		}

		if(top->memOpm&0x10)
		{
			top->memDataIn[0]=0xFFFFFFFFU;
			top->memDataIn[1]=0xFFFFFFFFU;
			top->memDataIn[2]=0xFFFFFFFFU;
			top->memDataIn[3]=0xFFFFFFFFU;

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
				top->memOK=1;
			}else if(is_dram)
			{
				drambuf[((addr>>2)+0)&0x1FFFFFF]=top->memDataOut[0];
				drambuf[((addr>>2)+1)&0x1FFFFFF]=top->memDataOut[1];
				drambuf[((addr>>2)+2)&0x1FFFFFF]=top->memDataOut[2];
				drambuf[((addr>>2)+3)&0x1FFFFFF]=top->memDataOut[3];
				top->memOK=1;
			}else
			{
				top->memOK=3;
			}

#if 0
			printf("ST  %08X  %08X %08X %08X %08X\n",
				addr,
				top->memDataOut[0], top->memDataOut[1],
				top->memDataOut[2], top->memDataOut[3]);
#endif
		}
	}else
	{
		top->memOK=0;
		mmio_latched=0;
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

int CheckDebugSanity()
{
	if(CheckDebugSanityIS()<=0)
		return(0);
	if(CheckDebugSanityDS()<=0)
		return(0);
	return(1);
}

int main(int argc, char **argv, char **env)
{
	BJX2_Context *ctx;
	FILE *fd;
	int lclk, mhz;
	int tt_start;
	int ctick, ctick_rst;
	int t0, t1, t2;
	int tt_frame;

	mhz=100;

	JX2R_UseImageCreateRamdisk(128*1024);
//	JX2R_UseImageCreateRamdisk(32*1024);

#if 1
	JX2R_UseImageAddFile(
		(char *)"BOOTLOAD.SYS",
		(char *)"../../tk_qsrc/tk_clib/tkshell.sys");

	JX2R_UseImageAddFile(
//		(char *)"BOOTLOAD.SYS",
		(char *)"DOOM.EXE",
		(char *)"../../tk_qsrc/doomsrc2/doom_bjx2.exe");
	JX2R_UseImageAddFile(
		(char *)"DOOM1.WAD",
		(char *)"../../tk_qsrc/doomsrc2/doom1.wad");
#endif

#if 1
	JX2R_UseImageAddFile(
//		(char *)"BOOTLOAD.SYS",
		(char *)"QUAKE.EXE",
		(char *)"../../tk_qsrc/quake_b64b.exe");
	JX2R_UseImageAddFile(
		(char *)"ID1/PAK0.PAK",
		(char *)"../../tk_qsrc/id1/pak0.pak");
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
	jx2_ctx=ctx;

	ctx->tgt_mhz=mhz;
	ctx->rcp_mhz=((1048576/ctx->tgt_mhz)+7)>>4;
	ctx->ttick_rst=(ctx->tgt_mhz*1000000)/1024;
	ctx->ttick_hk=ctx->ttick_rst;

	GfxDrv_Start();
	SoundDev_Init();
	JX2I_GfxCon_Startup();

	printf("Start ExUnit\n");

	BTSR1_MainInitKeyboard();

	tt_start=FRGL_TimeMS();
	tt_frame=tt_start;
	
//	ctick_rst=48828;
//	ctick=ctick_rst;

	while (!Verilated::gotFinish())
	{
		t1=FRGL_TimeMS();
		t2=t1-tt_frame;

		if(t2>16)
		{
			BTSR1_MainPollKeyboard();

			jx2i_gfxcon_dirty=1;
			JX2I_GfxCon_Update();

			GfxDrv_BeginDrawing();

			GfxDrv_EndDrawing();
			
			tt_frame=t1;
//			continue;
		}

		top->clock = (main_time>>0)&1;

		if(top->clock)
			top->memBusExc=0x0000;
		
		if(top->clock && (lclk!=top->clock))
		{
//			printf("Cycle\n");

			ctx->ttick_hk--;
			if(ctx->ttick_hk<=0)
			{
				top->memBusExc=0xC001;
			
//				printf("Clock IRQ\n");
				ctx->ttick_hk=ctx->ttick_rst;
			}
		}
		
		jx2_ctx->tot_cyc=main_time>>1;
		
		lclk = top->clock;
//		top->mode = 3;
		
//		top->baseAddr=0xDECAB00;
//		top->idxAddr=0x100;
//		top->idxDisp=3;

		if(top->clock)
		{
//			printf("Cycle %lld\n", jx2_ctx->tot_cyc);
		
			MemUpdateForBus();
			
			if(CheckDebugSanity()<=0)
				break;
		}

		top->eval();

		main_time++;
		
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

	SoundDev_DeInit();
	
	BTSR1_MainDeinitKeyboard();
	
	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
