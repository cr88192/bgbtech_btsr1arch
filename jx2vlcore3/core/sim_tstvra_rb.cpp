#include "VRbiMemVramA.h"
#include "verilated.h"

// #define HAS_OPS48
#define HAS_WEX

VRbiMemVramA *top = new VRbiMemVramA;

vluint64_t main_time = 0;

// static int write_words=256;
static int write_words=2048;

// static int write_words2=4096;

//static int write_words2=8192;
static int write_words2=16384;
// static int write_words2=32768;
// static int write_words2=65536;

uint32_t *rombuf;
uint32_t *srambuf;
uint32_t *drambuf;

uint32_t mmiobuf[1024];

int LoadRomBuf()
{
	char tb[256];
	char tba[64], tbb[64], tbc[64], tbd[64];
	FILE *fda, *fdb, *fdc, *fdd;
	uint32_t va, vb, vc, vd;
	int n;
	
//	fda=fopen("bootrom_1a.txt", "rt");
//	fdb=fopen("bootrom_1b.txt", "rt");
//	fdc=fopen("bootrom_1c.txt", "rt");
//	fdd=fopen("bootrom_1d.txt", "rt");

	fda=fopen("bootrom_1.txt", "rt");
	
	rombuf=(uint32_t *)(malloc(32768));
	memset(rombuf, 0, 32768);
	n=0;
	
	while(!feof(fda) && (n<2048))
	{
		memset(tba, 0, 64); va=0;
		memset(tbb, 0, 64); vb=0;
		memset(tbc, 0, 64); vc=0;
		memset(tbd, 0, 64); vd=0;

		fgets(tb, 255, fda);
		memcpy(tbd, tb+ 0, 8);
		memcpy(tbc, tb+ 8, 8);
		memcpy(tbb, tb+16, 8);
		memcpy(tba, tb+24, 8);
		
//		fgets(tba, 64, fda);
//		fgets(tbb, 64, fdb);
//		fgets(tbc, 64, fdc);
//		fgets(tbd, 64, fdd);
		sscanf(tba, "%08X", &va);
		sscanf(tbb, "%08X", &vb);
		sscanf(tbc, "%08X", &vc);
		sscanf(tbd, "%08X", &vd);
		
//		printf("%04X  %s %s %s %s\n", n*16, tba, tbb, tbc, tbd);
//		printf("%04X  %08X %08X %08X %08X\n", n*16, va, vb, vc, vd);
		
		rombuf[n*4+0]=va;
		rombuf[n*4+1]=vb;
		rombuf[n*4+2]=vc;
		rombuf[n*4+3]=vd;
		n++;
	}
	
	printf("ROM, Loaded %d bytes\n", n*16);
	
//	fgetc(stdin);
	
	fclose(fda);
//	fclose(fdb);
//	fclose(fdc);
//	fclose(fdd);
	return(0);
}

int GetRomWord(int n)
{
	uint32_t v;
	
	v=rombuf[n>>1];
	if(n&1)v>>=16;
	return(v&0xFFFF);
}

uint64_t main_buslines;

void MemUpdateForBus()
{
	static uint32_t	l2addr1, l2addr2;
	static uint32_t	l2seq1, l2seq2;
	static uint32_t	l2opm1, l2opm2;
	static uint32_t	l2data1a, l2data2a;
	static uint32_t	l2data1b, l2data2b;
	static uint32_t	l2data1c, l2data2c;
	static uint32_t	l2data1d, l2data2d;

	static uint32_t	l2addr3, l2addr4;
	static uint32_t	l2seq3, l2seq4;
	static uint32_t	l2opm3, l2opm4;
	static uint32_t	l2data3a, l2data4a;
	static uint32_t	l2data3b, l2data4b;
	static uint32_t	l2data3c, l2data4c;
	static uint32_t	l2data3d, l2data4d;

	static uint32_t	l2addr5[32];
	static uint32_t	l2seq5[32];
	static uint32_t	l2opm5[32];
	static uint32_t	l2data5a[32];
	static uint32_t	l2data5b[32];
	static uint32_t	l2data5c[32];
	static uint32_t	l2data5d[32];

	static	int			lclock;
	int i;

#if 1
	top->memSeqIn=l2seq2;
	top->memOpmIn=l2opm2;
	top->memAddrIn=l2addr2;
	top->memDataIn[0]=l2data2a;
	top->memDataIn[1]=l2data2b;
	top->memDataIn[2]=l2data2c;
	top->memDataIn[3]=l2data2d;
#endif

	if(top->clock==lclock)
		return;
	lclock=top->clock;
	
	if(top->clock)
	{
		l2seq2=l2seq1;
		l2opm2=l2opm1;
		l2addr2=l2addr1;
		l2data2a=l2data1a;
		l2data2b=l2data1b;
		l2data2c=l2data1c;
		l2data2d=l2data1d;

		l2seq1=top->memSeqOut;
		l2opm1=top->memOpmOut;
		l2addr1=top->memAddrOut;
		l2data1a=top->memDataOut[0];
		l2data1b=top->memDataOut[1];
		l2data1c=top->memDataOut[2];
		l2data1d=top->memDataOut[3];

#if 1
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
	

	int addr, isRom, isSRam, isDRam, isMmio;
	
	addr	= l2addr1;
	isRom	= (addr>=0x00000000) && (addr<=0x00007FFF);
	isSRam	= (addr>=0x0000C000) && (addr<=0x0000DFFF);
	isDRam	= (addr>=0x01000000) && (addr<=0x3FFFFFFF);
	
	isMmio = 0;
	if(((l2opm1&0xF0)==0x90) && ((l2opm1&0x0F)!=0x07))
		isMmio = 1;
	if(((l2opm1&0xF0)==0xA0) && ((l2opm1&0x0F)!=0x07))
		isMmio = 2;
	
	if(isMmio==1)
	{
		l2data1a=mmiobuf[(addr>>2)&1023];
		l2data1b=0;
		l2data1c=0;
		l2data1d=0;
		l2opm1=0x70;
	}
	
	if(isMmio==2)
	{
		mmiobuf[(addr>>2)&1023]=l2data1a;
		l2data1b=0;
		l2data1c=0;
		l2data1d=0;
		l2opm1=0x70;
	}
	
	if(l2opm1==0x97)
	{
		main_buslines++;
		if(isRom)
		{
			l2data1a=rombuf[((addr>>2)+0)&0x1FFF];
			l2data1b=rombuf[((addr>>2)+1)&0x1FFF];
			l2data1c=rombuf[((addr>>2)+2)&0x1FFF];
			l2data1d=rombuf[((addr>>2)+3)&0x1FFF];
			l2opm1=0x70;
		}

		if(isSRam)
		{
			l2data1a=srambuf[((addr>>2)+0)&0x7FF];
			l2data1b=srambuf[((addr>>2)+1)&0x7FF];
			l2data1c=srambuf[((addr>>2)+2)&0x7FF];
			l2data1d=srambuf[((addr>>2)+3)&0x7FF];
			l2opm1=0x70;
		}

		if(isDRam)
		{
			l2data1a=drambuf[((addr>>2)+0)&0x3FFFFF];
			l2data1b=drambuf[((addr>>2)+1)&0x3FFFFF];
			l2data1c=drambuf[((addr>>2)+2)&0x3FFFFF];
			l2data1d=drambuf[((addr>>2)+3)&0x3FFFFF];
			l2opm1=0x70;
		}
	}

	if(l2opm1==0xA7)
	{
		main_buslines++;

		if(isRom)
		{
			l2opm1=0x60;
		}

		if(isSRam)
		{
			srambuf[((addr>>2)+0)&0x7FF]=l2data1a;
			srambuf[((addr>>2)+1)&0x7FF]=l2data1b;
			srambuf[((addr>>2)+2)&0x7FF]=l2data1c;
			srambuf[((addr>>2)+3)&0x7FF]=l2data1d;
			l2opm1=0x60;
		}

		if(isDRam)
		{
			drambuf[((addr>>2)+0)&0x3FFFFF]=l2data1a;
			drambuf[((addr>>2)+1)&0x3FFFFF]=l2data1b;
			drambuf[((addr>>2)+2)&0x3FFFFF]=l2data1c;
			drambuf[((addr>>2)+3)&0x3FFFFF]=l2data1d;
			l2opm1=0x60;
		}
	}
}

int main(int argc, char **argv, char **env)
{
//	uint32_t *drambuf;
	uint32_t *membuf;
	uint32_t v, v0, v1, v2, v3, v4, v5;
	uint64_t lv0, lv1, lv2, lv3;

	uint64_t sr0_cyc0, sr0_cyc1, sr0_n;
	uint64_t sr0_bus0, sr0_bus1;
	double f, g, h;
	int fail;
	int addr, n, n1, inh, stp, stp1, rdy;
	int i, j, k;

//	LoadRomBuf();

	membuf=(uint32_t *)malloc(65536);
	for(i=0; i<(65536/4); i++)
	{
		membuf[i]=rand()*rand();
	}

	drambuf=(uint32_t *)malloc(1<<24);
	srambuf=(uint32_t *)malloc(8192);

	LoadRomBuf();

	Verilated::commandArgs(argc, argv);

	fail=0;


	sr0_cyc0=main_time;
	sr0_bus0=main_buslines;

#if 1
	n=0; inh=2; rdy=0;
	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
		main_time++;

		if(top->clock)
		{
//			printf("Clock %d SR0 %d  \n", (int)(main_time/2), n);
//			fflush(stdout);
		}

		if(inh)
		{
			MemUpdateForBus();
			top->eval();

			if(top->clock)inh--;
			continue;
		}

//		addr=0xC000|(n*4);
		addr=0xF00A0000|(n*4);
		top->regInAddr=addr;
		top->regInOpm=0x12;
		top->regInVal=membuf[n];
//		top->dcInHold=0;

//		if(inh==1)
//			top->dcInHold=1;
//		if(!inh && (top->dcOutOK==2))
//			{ top->dcInHold=1; }
//		if(top->dcOutHold)
//			{ top->dcInHold=1; }

		MemUpdateForBus();

		top->eval();

//		if(top->dcOutHold)
//			{ top->dcInHold=1; continue; }

		if(!top->clock)
			continue;

#if 0
		if(inh)
		{
			inh--;
			continue;
		}
#endif

#if 1
		if(top->regOutOK==2)
//		if(top->dcInHold)
		{
			printf("Busy %d\n", n);
		}else
			if(inh)
		{
			printf("Inhibit Cycle\n");
			inh--;
		}else
			if(top->regOutOK==1)
//			if(1)
		{
			printf("W %d\n", n);
			n++;
			if(n>=4096)
				break;

			top->regInOpm=0;
//			inh=0;
//			inh=1;
			inh=2;
//			inh=3;
		}else if(top->regOutOK==0)
		{
//			fail|=1;
//			printf("Fail, Expect OK\n");
//			break;
		}
#endif
	}

	sr0_cyc1=main_time;
	sr0_bus1=main_buslines;
	sr0_n=n;
	
//	fail=1;
	

	if(fail)
	{
		delete top;
		exit(0);
	}
#endif

	for(i=0; i<4; i++)
	{
		top->regInOpm=0x00;
		top->clock = (main_time>>0)&1;
		main_time++;
		MemUpdateForBus();
		top->eval();
	}

	printf("\n\n");

#if 1
	n=0; inh=2; rdy=0;
	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
		main_time++;

		if(top->clock)
		{
//			printf("Clock %d SR1 %d  \n", (int)(main_time/2), n);
//			fflush(stdout);
		}

		if(inh)
		{
			MemUpdateForBus();
			top->eval();

			if(top->clock)inh--;
			continue;
		}

//		addr=0xC000|(n*4);
		addr=0xF00A0000|(n*4);
		top->regInAddr=addr;
		top->regInOpm=0x0A;
//		top->regInHold=0;
//		top->regInVal=membuf[n];
		v=top->regOutVal;
		v1=membuf[n];

		MemUpdateForBus();

		top->eval();

		if(!top->clock)
			continue;

#if 1
		if(inh)
		{
			printf("Inhibit Cycle\n");
			inh--;
			continue;
		}
#endif

		v=top->regOutVal;

		if(top->regOutOK==2)
//		if(top->dcInHold)
		{
			printf("Busy\n");
		}else
			if(inh)
		{
			printf("Inhibit Cycle\n");
			inh--;
		}else
			if(top->regOutOK==1)
		{
			printf("SR1 %04X Got=%08X Expect=%08X %s\n",
				addr, v, v1,
					(v1==v)?"Pass":"Fail");
					
			if(v1!=v)
			{
				printf("Mismatch, Ending Test (SRAM)\n");
				fail|=2;
				break;
			}

			top->regInOpm=0;
		
//			printf("W %d\n", n);
//			inh=0;
//			inh=1;
//			inh=2;
//			inh=3;
			inh=4;
			n++;
//			if(n>=8)
//			if(n>=256)
			if(n>=2048)
//			if(n>=16384)
				break;
		}
	}

	if(fail)
	{
		delete top;
		exit(0);
	}
#endif


	lv0=(sr0_cyc1-sr0_cyc0)>>1;
	lv1=sr0_bus1-sr0_bus0;
//	sr0_n=n;

	f=lv0*(1.0/lv1);
	g=(50*16)/f;

	printf("%f cyc/store\n", lv0*(1.0/sr0_n));
	printf("%f cycles/line, %f MB/s (@ 50MHz) \n", f, g);

	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
