#include "VCoreUnit.h"
#include "verilated.h"

VCoreUnit *top = new VCoreUnit;

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
// byte gfxdrv_kill;
byte fkey;

byte fmode;

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


int sdc_spibit(int bit, int cs)
{
	static int init=0;
	static int pos, tvi, tvo, tvn;
	int ret;

	if(!init)
	{
		init=1;
		tvo=0xFFFF;
		pos=8;
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
			printf("sdc_spibit: tvi=%02X tvo=%02X\n", tvi, tvo);
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
		tvo=0xFFFF;
		tvi=0xFF;
//		tvn=0xFF;
		pos=8;
	}
	return(ret);
}

#define	SIMDDR_BUSRT	4
// #define	SIMDDR_BUSRT	8

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


int ddr_cmdhi, ddr_cmdlo;
int ddr_cyc;

int ddr_parm_wl=3;	//CAS WL*2+1
int ddr_parm_rl=7;	//CAS RL*2+1
int ddr_row, ddr_col, ddr_bank;
int ddr_state, ddr_cas, ddr_burst;

uint16_t	*ddr_ram;

#if 1
int SimDdr(int clk, int cmd, int *rdata)
{
	int		data, row, col, bank, pos;
	int addr;

//Cs;
//			cmd=(cmd<<1)|top->ddrRas;
//			cmd=(cmd<<1)|top->ddrCas;
//			cmd=(cmd<<1)|top->ddrWe;
//			cmd=(cmd<<1)|top->ddrCke;
//			cmd=(cmd<<2)|top->ddrBa

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
		}
	}else if(ddr_state==2)
	{
		if(ddr_burst>0)
		{
			ddr_burst--;

			data=*rdata;
			pos=(ddr_row<<13)+(ddr_bank<<10)+ddr_col;
			pos&=(1<<27)-1;
			ddr_ram[pos>>1]=data;
			ddr_col+=2;
		}
	}
		
	addr=(cmd&0x3FFF);
	
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
				printf("No-Op\n");
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
				ddr_state=1;
//				ddr_cas=ddr_parm_rl*2+1;
				ddr_cas=4*2+1;
				ddr_burst=SIMDDR_BUSRT;
			}else
			{
				printf("Write Active Row\n");
				ddr_col=addr;
				ddr_state=2;
				ddr_cas=ddr_parm_wl*2+1;
				ddr_burst=SIMDDR_BUSRT;
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
				printf("Load Mode Register\n");
			}
		}
	}
}
#endif

int update_ddr()
{
	int ddrlclk, cmd, data;
	int n, inh;
	int i, j, k;

	if(top->ddrClk!=ddrlclk)
	{
		ddrlclk=top->ddrClk;
//			cmd=top->ddrCmd;

		cmd=0;
		cmd=(cmd<<1)|top->ddrCs;
		cmd=(cmd<<1)|top->ddrRas;
		cmd=(cmd<<1)|top->ddrCas;
		cmd=(cmd<<1)|top->ddrWe;
		cmd=(cmd<<1)|top->ddrCke;
		cmd=(cmd<<2)|top->ddrBa;
		cmd=(cmd<<14)|top->ddrAddr;

//			data=top->ddrData;
		data=top->ddrDataO;
		SimDdr(top->ddrClk, cmd, &data);
//			top->ddrData=data;
		top->ddrDataI=data;
	}
}

int main(int argc, char **argv, char **env)
{
	BJX2_Context *ctx;
	cdec_imgbuf *vgactx;
	FILE *fd;
	int lclk, mhz;
	int sdc_lclk;
	int tt_start, tt_frame;
	int t0, t1, t2;
	int i, j, k;

	mhz=100;

	JX2R_UseImageCreateRamdisk(128*1024);
//	JX2R_UseImageCreateRamdisk(32*1024);
	JX2R_UseImageAddFile(
		(char *)"BOOTLOAD.SYS",
		(char *)"../../tk_qsrc/doomsrc2/doom_bjx2.exe");
	JX2R_UseImageAddFile(
		(char *)"DOOM1.WAD",
		(char *)"../../tk_qsrc/doomsrc2/doom1.wad");

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

//	ddr_ram=(uint16_t *)malloc(1<<27);
	ddr_ram=(uint16_t *)malloc(1<<28);

#if 1
	fd=fopen("../../tst_jx2boot.bin", "rb");
	if(fd)
	{
		t2=fread(rombuf, 1, 32768, fd);
		fclose(fd);
	}
#endif

	ctx=(BJX2_Context *)malloc(sizeof(BJX2_Context));
//	jx2_ctx=ctx;

	vgactx=(cdec_imgbuf *)malloc(sizeof(cdec_imgbuf));
	memset(vgactx, 0, sizeof(cdec_imgbuf));

	CDEC_SetupForStream(vgactx);

	ctx->tgt_mhz=mhz;
	ctx->rcp_mhz=((1048576/ctx->tgt_mhz)+7)>>4;
	ctx->ttick_rst=(ctx->tgt_mhz*1000000)/1024;
	ctx->ttick_hk=ctx->ttick_rst;

	printf("Start CoreUnit\n");

	btesh2_gfxcon_fbxs = 800;
	btesh2_gfxcon_fbys = 600;

	GfxDrv_Start();
	SoundDev_Init();

	BTSR1_MainInitKeyboard();

	tt_start=FRGL_TimeMS();

	while (!Verilated::gotFinish())
	{
		t1=FRGL_TimeMS();
		t2=t1-tt_frame;

		if(t2>16)
		{
			BTSR1_MainPollKeyboard();

//			jx2i_gfxcon_dirty=1;
//			JX2I_GfxCon_Update();

			GfxDrv_PrepareFramebuf();
			
			cdec_getimage(vgactx, btesh2_gfxcon_framebuf, 800, 600);

			GfxDrv_BeginDrawing();

			GfxDrv_EndDrawing();
			
			tt_frame=t1;
//			continue;
		}

//		BTSR1_MainPollKeyboard();
	
		top->clock = (main_time>>0)&1;
		
		if(top->clock && (lclk!=top->clock))
		{
			i=(top->vgaRed<<8)|(top->vgaGrn<<4)|(top->vgaBlu<<0)|
				(top->vgaHsync<<12)|(top->vgaVsync<<13);
			CDEC_UpdateForStreamCycle(vgactx, i);

//			printf("Cycle\n");

			ctx->ttick_hk--;
			if(ctx->ttick_hk<=0)
			{
//				printf("Clock IRQ\n");
				ctx->ttick_hk=ctx->ttick_rst;
			}
			
			if(top->sdc_clk!=sdc_lclk)
			{
				if(top->sdc_clk)
				{
					top->sdc_dat_i=
						sdc_spibit(top->sdc_cmd, top->sdc_dat_o&8);
				}
			
				sdc_lclk=top->sdc_clk;
			}
		}
		
		ctx->tot_cyc=main_time>>1;
		
		lclk = top->clock;
//		top->mode = 3;
		
//		top->baseAddr=0xDECAB00;
//		top->idxAddr=0x100;
//		top->idxDisp=3;

		if(top->clock)
		{
//			printf("Cycle %lld\n", jx2_ctx->tot_cyc);
		
//			MemUpdateForBus();
//			if(CheckDebugSanity()<=0)
//				break;
		}

		top->eval();

		update_ddr();

		main_time++;
		
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
