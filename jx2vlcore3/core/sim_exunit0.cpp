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
byte gfxdrv_kill;
byte fkey;

byte fmode;

#ifdef __linux
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


BJX2_Context	*jx2_ctx;

u32 mmgp_data[256];

uint32_t mmio_ReadDWord(BJX2_Context *ctx, uint32_t addr)
{
	u32 *mmio;
	s64 rvq, dcyc;
	int val;

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
		val=0;
		break;
	}
	return(val);
}

uint32_t mmio_WriteDWord(BJX2_Context *ctx, uint32_t addr, uint32_t val)
{
	u32 *mmio;
	int v;

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

int main(int argc, char **argv, char **env)
{
	BJX2_Context *ctx;
	FILE *fd;
	uint32_t addr;
	byte is_rom, is_sram, is_mmio;
	int opm_latch;
	int lclk, mhz;
	int tt_start;
	int t0, t1, t2;

	mhz=100;

//	JX2R_UseImageCreateRamdisk(128*1024);
	JX2R_UseImageCreateRamdisk(32*1024);
	JX2R_UseImageAddFile(
		(char *)"BOOTLOAD.SYS",
		(char *)"../tk_qsrc/doomsrc2/doom_bjx2.exe");

	Verilated::commandArgs(argc, argv);

	rombuf=(uint32_t *)malloc(32768);
	srambuf=(uint32_t *)malloc(8192);

	drambuf=(uint32_t *)malloc(1<<28);

	fd=fopen("../../tst_jx2boot.bin", "rb");
	if(fd)
	{
		fread(rombuf, 1, 32768, fd);
		fclose(fd);
	}

	ctx=(BJX2_Context *)malloc(sizeof(BJX2_Context));
	jx2_ctx=ctx;

	ctx->tgt_mhz=mhz;
	ctx->rcp_mhz=((1048576/ctx->tgt_mhz)+7)>>4;
	ctx->ttick_rst=(ctx->tgt_mhz*1000000)/1024;
	ctx->ttick_hk=ctx->ttick_rst;

	printf("Start ExUnit\n");

	BTSR1_MainInitKeyboard();

	tt_start=FRGL_TimeMS();

	while (!Verilated::gotFinish())
	{
		BTSR1_MainPollKeyboard();
	
		top->clock = (main_time>>0)&1;
		
		if(top->clock && (lclk!=top->clock))
		{
//			printf("Cycle\n");

			ctx->ttick_hk--;
			if(ctx->ttick_hk<=0)
			{
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

		top->eval();


		if(top->memOpm)
		{
			addr=top->memAddr;
			top->memOK=0;
			
			is_rom	= (addr>=0x00000000) && (addr<0x00008000);
			is_sram	= (addr>=0x0000C000) && (addr<0x0000E000);
			is_mmio = (addr>=0xA0000000) && (addr<0xC0000000);

			if(top->memOpm&0x08)
			{
				if(is_mmio)
				{
					top->memDataIn[0]=
						mmio_ReadDWord(jx2_ctx, addr&0xFFFFFF);
					top->memOK=1;
				}
				if(is_rom)
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
				}else
				{
					top->memDataIn[0]=drambuf[((addr>>2)+0)&0x3FFFFFF];
					top->memDataIn[1]=drambuf[((addr>>2)+1)&0x3FFFFFF];
					top->memDataIn[2]=drambuf[((addr>>2)+2)&0x3FFFFFF];
					top->memDataIn[3]=drambuf[((addr>>2)+3)&0x3FFFFFF];
					top->memOK=1;
				}

				printf("DRAM-LD  %08X  %08X %08X %08X %08X\n",
					addr,
					top->memDataIn[0], top->memDataIn[1],
					top->memDataIn[2], top->memDataIn[3]);
			}

			if(top->memOpm&0x10)
			{
				if(is_mmio)
				{
					mmio_WriteDWord(jx2_ctx, addr&0xFFFFFF,
						top->memDataOut[0]);
					top->memOK=1;
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
				}else
				{
					drambuf[((addr>>2)+0)&0x3FFFFFF]=top->memDataOut[0];
					drambuf[((addr>>2)+1)&0x3FFFFFF]=top->memDataOut[1];
					drambuf[((addr>>2)+2)&0x3FFFFFF]=top->memDataOut[2];
					drambuf[((addr>>2)+3)&0x3FFFFFF]=top->memDataOut[3];
					top->memOK=1;
				}

				printf("DRAM-ST  %08X  %08X %08X %08X %08X\n",
					addr,
					top->memDataOut[0], top->memDataOut[1],
					top->memDataOut[2], top->memDataOut[3]);
			}
		}else
		{
			top->memOK=0;
		}

#if 0
//		top->mmioOK = 0;
		if(top->mmioOpm && !opm_latch)
		{
			top->mmioOK = 0;
			opm_latch=1;
//			printf("MMIO OPM=%X Addr=%X\n", top->mmioOpm, top->mmioAddr);
			if((top->mmioAddr&0x0FFFF000)==0x0000E000)
			{
				if(top->mmioOpm&0x08)
				{
					top->mmioInData=
						mmio_ReadDWord(jx2_ctx, top->mmioAddr);
					top->mmioOK = 1;
				}else if(top->mmioOpm&0x10)
				{
					mmio_WriteDWord(jx2_ctx, top->mmioAddr,
						top->mmioOutData);
					top->mmioOK = 1;
				}else
				{
					top->mmioOK = 1;
				}
			}else
			{
//				printf("OPM=%X Addr=%X\n", top->mmioOpm, top->mmioAddr);
				top->mmioOK = 3;
			}
		}else
		{
			if(!(top->mmioOpm&0x18))
			{
				opm_latch=0;
				top->mmioOK = 0;
			}
		}
#endif

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
	
	BTSR1_MainDeinitKeyboard();
	
	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
