#include "VRbiMemL1A.h"
#include "verilated.h"

// #define HAS_OPS48
#define HAS_WEX

VRbiMemL1A *top = new VRbiMemL1A;

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
	top->l2mSeqIn=l2seq2;
	top->l2mOpmIn=l2opm2;
	top->l2mAddrIn=l2addr2;
	top->l2mDataIn[0]=l2data2a;
	top->l2mDataIn[1]=l2data2b;
	top->l2mDataIn[2]=l2data2c;
	top->l2mDataIn[3]=l2data2d;
#endif

#if 0
	top->l2mSeqIn=l2seq4;
	top->l2mOpmIn=l2opm4;
	top->l2mAddrIn=l2addr4;
	top->l2mDataIn[0]=l2data4a;
	top->l2mDataIn[1]=l2data4b;
	top->l2mDataIn[2]=l2data4c;
	top->l2mDataIn[3]=l2data4d;
#endif

#if 0
	top->l2mSeqIn=l2seq5[10];
	top->l2mOpmIn=l2opm5[10];
	top->l2mAddrIn=l2addr5[10];
	top->l2mDataIn[0]=l2data5a[10];
	top->l2mDataIn[1]=l2data5b[10];
	top->l2mDataIn[2]=l2data5c[10];
	top->l2mDataIn[3]=l2data5d[10];
#endif

#if 0
	top->l2mSeqIn=l2seq5[14];
	top->l2mOpmIn=l2opm5[14];
	top->l2mAddrIn=l2addr5[14];
	top->l2mDataIn[0]=l2data5a[14];
	top->l2mDataIn[1]=l2data5b[14];
	top->l2mDataIn[2]=l2data5c[14];
	top->l2mDataIn[3]=l2data5d[14];
#endif

#if 0
	top->l2mSeqIn=l2seq5[24];
	top->l2mOpmIn=l2opm5[24];
	top->l2mAddrIn=l2addr5[24];
	top->l2mDataIn[0]=l2data5a[24];
	top->l2mDataIn[1]=l2data5b[24];
	top->l2mDataIn[2]=l2data5c[24];
	top->l2mDataIn[3]=l2data5d[24];
#endif

	if(top->clock==lclock)
		return;
	lclock=top->clock;
	
	if(top->clock)
	{
		for(i=31; i>0; i--)
		{
			l2seq5[i]=l2seq5[i-1];
			l2opm5[i]=l2opm5[i-1];
			l2addr5[i]=l2addr5[i-1];
			l2data5a[i]=l2data5a[i-1];
			l2data5b[i]=l2data5b[i-1];
			l2data5c[i]=l2data5c[i-1];
			l2data5d[i]=l2data5d[i-1];
		}

		l2seq5[0]=l2seq4;
		l2opm5[0]=l2opm4;
		l2addr5[0]=l2addr4;
		l2data5a[0]=l2data4a;
		l2data5b[0]=l2data4b;
		l2data5c[0]=l2data4c;
		l2data5d[0]=l2data4d;
	
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

		l2seq1=top->l2mSeqOut;
		l2opm1=top->l2mOpmOut;
		l2addr1=top->l2mAddrOut;
		l2data1a=top->l2mDataOut[0];
		l2data1b=top->l2mDataOut[1];
		l2data1c=top->l2mDataOut[2];
		l2data1d=top->l2mDataOut[3];

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
	isDRam	= (addr>=0x01000000) && (addr<=0x0FFFFFFF);
	
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
	
//	addr=

#if 0
	if(top->memOpm)
	{
		addr=top->memAddr;
		top->memOK=0;
		
		isRom	= (addr>=0x00000000) && (addr<=0x00007FFF);
		isSRam	= (addr>=0x0000C000) && (addr<=0x0000DFFF);
//		isSRam	= (addr>=0x0000C000) && (addr<=0x0000E010);
		isDRam	= (addr>=0x01000000) && (addr<=0x0FFFFFFF);

		top->memDataIn[0]=0;
		top->memDataIn[1]=0;
		top->memDataIn[2]=0;
		top->memDataIn[3]=0;

		if(top->memOpm&0x08)
		{
			if(isRom)
			{
				top->memDataIn[0]=rombuf[((addr>>2)+0)&0x1FFF];
				top->memDataIn[1]=rombuf[((addr>>2)+1)&0x1FFF];
				top->memDataIn[2]=rombuf[((addr>>2)+2)&0x1FFF];
				top->memDataIn[3]=rombuf[((addr>>2)+3)&0x1FFF];
				top->memOK=1;
			}

			if(isSRam)
			{
				top->memDataIn[0]=srambuf[((addr>>2)+0)&0x7FF];
				top->memDataIn[1]=srambuf[((addr>>2)+1)&0x7FF];
				top->memDataIn[2]=srambuf[((addr>>2)+2)&0x7FF];
				top->memDataIn[3]=srambuf[((addr>>2)+3)&0x7FF];
				top->memOK=1;
			}

			if(isDRam)
			{
				top->memDataIn[0]=drambuf[((addr>>2)+0)&0x3FFFFF];
				top->memDataIn[1]=drambuf[((addr>>2)+1)&0x3FFFFF];
				top->memDataIn[2]=drambuf[((addr>>2)+2)&0x3FFFFF];
				top->memDataIn[3]=drambuf[((addr>>2)+3)&0x3FFFFF];
				top->memOK=1;
			}

			top->memOK=1;

#if 1
			printf("%08X LD  %08X %08X %08X %08X\n",
				addr,
				top->memDataIn[0], top->memDataIn[1],
				top->memDataIn[2], top->memDataIn[3]);
#endif
		}

		if(top->memOpm&0x10)
		{
			if(isSRam)
			{
				srambuf[((addr>>2)+0)&0x7FF]=top->memDataOut[0];
				srambuf[((addr>>2)+1)&0x7FF]=top->memDataOut[1];
				srambuf[((addr>>2)+2)&0x7FF]=top->memDataOut[2];
				srambuf[((addr>>2)+3)&0x7FF]=top->memDataOut[3];
				top->memOK=1;
			}

			if(isDRam)
			{
				drambuf[((addr>>2)+0)&0x3FFFFF]=top->memDataOut[0];
				drambuf[((addr>>2)+1)&0x3FFFFF]=top->memDataOut[1];
				drambuf[((addr>>2)+2)&0x3FFFFF]=top->memDataOut[2];
				drambuf[((addr>>2)+3)&0x3FFFFF]=top->memDataOut[3];
				top->memOK=1;
			}

			top->memOK=1;

			printf("%08X ST  %08X %08X %08X %08X\n",
				addr,
				top->memDataOut[0], top->memDataOut[1],
				top->memDataOut[2], top->memDataOut[3]);
		}
	}else
	{
//		top->memOK=0;
	}
#endif
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

//	LoadRomBuf();

	top->regInSr=0;
	top->regInMmcr=0;
	top->regInKrr=0;
	top->dcInHold=0;

	fail=0;

	n=0;
	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
		main_time++;

		top->dcInAddr[0]=0;
		top->dcInAddr[1]=0;
		top->dcInAddr[2]=0;
		top->dcInOpm=0;
		top->icInPcWxe=1;

//		addr=0xC000|(n*2);
		addr=0x0000|(n*2);
		top->icInPcAddr[0]=addr;
		top->icInPcAddr[1]=0;
		top->icInPcAddr[2]=0;
//		top->icRegInOpm=0xA;
//		top->icRegInData=rand();

//		if(top->clock)
			MemUpdateForBus();
		top->eval();
		
		if(!top->clock)
			continue;

		if(top->icOutPcOK==1)
		{
			if(!(n&7))
			{
				printf("%04X  ", n*2);
			}
		
			stp=top->icOutPcStep;
		
			v0=(top->icOutPcVal[0])&65535;
			v2=(top->icOutPcVal[1])&65535;
			v4=(top->icOutPcVal[2])&65535;
			printf("%04X ", v0);
			
			v1=GetRomWord(n);
			
			if(v0!=v1)
			{
				printf("Mismatch (IC1) Got=%04X Expect=%04X A=%04X\n",
					v0, v1, n*2);
				fail|=1;
				break;
			}

			v1=((top->icOutPcVal[0])>>16)&65535;
			v3=((top->icOutPcVal[1])>>16)&65535;
			v5=((top->icOutPcVal[2])>>16)&65535;
			
			stp1=1;

			if((v0&0xF000)==0x7000)
				stp1=2;
			if((v0&0xF000)==0x9000)
				stp1=2;

			if((v0&0xE000)==0xE000)
			{
				stp1=2;

// #ifdef HAS_OPS48
#if 0
				if((v0&0xEE00)==0xEC00)
					{ stp1=3; }
				if((v0&0xEE00)==0xEE00)
					{ stp1=3; }
#endif

// #ifdef HAS_WEX
#if 0
//				if(((v&0xFC00)==0xF400) || ((v&0xFF00)==0xF900))
//				if((v0&0xFC00)==0xF400)
				if(((v0&0xFC00)==0xF400) || ((v0&0xFC00)==0xFC00) ||
					((v0&0xFE00)==0xEA00) || ((v0&0xFE00)==0xEE00)	)
				{
//					if(((v2&0xFC00)==0xF400) || ((v2&0xFF00)==0xF900))
//					if((v2&0xFC00)==0xF400)
					if(	((v2&0xFC00)==0xF400) || ((v2&0xFC00)==0xFC00) ||
						((v2&0xFE00)==0xEA00) || ((v2&0xFE00)==0xEE00)	)
					{
						stp1=6;
//					}else if((v2&0xE000)==0xE000)
					}else if(((v2&0xE000)==0xE000) || ((v0&0xFE00)!=0xFE00))
					{
						stp1=4;
					}else
					{
						stp1=3;
					}
				}
#endif

#if 0
				if(((v0&0xFF00)==0xF400) && ((v1&0xC000)==0xC000))
				{
					if(((v2&0xFF00)==0xF400) && ((v3&0xC000)==0xC000))
					{
						stp1=6;
					}else
					{
						stp1=4;
					}
				}
#endif

#if 1
				if((v0&0xFE00)==0xFE00)
				{
					if(((v2&0xFE00)==0xFE00) ||
						((v2&0xFC00)==0xF400) || ((v2&0xFC00)==0xFC00) ||
						((v2&0xFE00)==0xEA00) || ((v2&0xFE00)==0xEE00) ||
						((v2&0xF800)==0x7800) || ((v2&0xF800)==0x9800)
						)
					{
						stp1=6;
					}else if(
						((v2&0xE000)==0xE000) ||
						((v2&0xF000)==0x7000) ||
						((v2&0xF000)==0x9000))
					{
						stp1=4;
					}else
					{
						stp1=3;
					}
				}
#endif

			}
			
//			if(stp!=stp1)
			if(stp!=(stp1*2))
			{
				printf("Step Mismatch %04X != %04X (Exp)"
						" (%04X-%04X-%04X-%04X-%04X-%04X)\n",
					stp, (stp1*2), v0, v1, v2, v3, v4, v5);
				fail|=1;
				break;
			}

//			printf("W %08X\n", v);
			n++;
//			inh=4;
			if(!(n&7))
			{
				printf("\n");
			}
		}
		
		if((n*2)>=32752)
//		if((n*2)>=32768)
//		if((n*2)>=16384)
//		if((n*2)>=8192)
			break;

	}
	printf("\n");

	if(fail)
	{
		delete top;
		exit(0);
	}


#if 1
	n=0;
	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
		main_time++;

		top->dcInAddr[0]=0;
		top->dcInAddr[1]=0;
		top->dcInAddr[2]=0;
		top->dcInOpm=0;

		n1=(n^(n>>8))&1023;

		addr=0x0000|(n1*2);
		top->icInPcAddr[0]=0;
		top->icInPcAddr[1]=0;
		top->icInPcAddr[2]=0;

//		if(top->clock)
			MemUpdateForBus();
		top->eval();

		if(!top->clock)
			continue;

		if(top->dcOutHold)
			{ top->dcInHold=1; continue; }

		printf("%d/%d\r", n, 32752/2);
		fflush(stdout);

		if(top->icOutPcOK==1)
		{
			stp=top->icOutPcStep;
		
			v=(top->icOutPcVal[0])&65535;
			v0=(top->icOutPcVal[0])&65535;
			v2=(top->icOutPcVal[1])&65535;
			v4=(top->icOutPcVal[2])&65535;
			
			v1=GetRomWord(n1);
			
			if(v!=v1)
			{
				printf("Mismatch (IC2) Got=%04X Expect=%04X A=%04X\n",
					v, v1, n1*2);
				fail|=1;
				break;
			}
			
			stp1=1;
#if 0
			if((v&0xE000)==0xE000)
			{
				stp1=2;
#ifdef HAS_OPS48
				if((v&0xEE00)==0xEC00)
					{ stp1=3; }
				if((v&0xEE00)==0xEE00)
					{ stp1=3; }
#endif
			}
#endif
			if((v0&0xE000)==0xE000)
			{
				stp1=2;

				if(((v0&0xFC00)==0xF400) || ((v0&0xFC00)==0xFC00) ||
					((v0&0xFE00)==0xEA00) || ((v0&0xFE00)==0xEE00)	)
				{
					if(	((v2&0xFC00)==0xF400) || ((v2&0xFC00)==0xFC00) ||
						((v2&0xFE00)==0xEA00) || ((v2&0xFE00)==0xEE00)	)
					{
						stp1=6;
					}else if(((v2&0xE000)==0xE000) || ((v0&0xFE00)!=0xFE00))
					{
						stp1=4;
					}else
					{
						stp1=3;
					}
				}
			}

#if 0
//			if(stp!=stp1)
			if(stp!=(stp1*2))
			{
				printf("Step Mismatch %04X != %04X\n", stp, (stp1*2));
				fail|=1;
				break;
			}
#endif

			n++;
		}
		
//		if((n*2)>=32752)
		if((n*8)>=32752)
			break;

	}
	printf("\n");

	if(fail)
	{
		delete top;
		exit(0);
	}
#endif


#if 0
	delete top;
	exit(0);
#endif

	for(i=0; i<8; i++)
	{
		top->dcInOpm=0x00;
		top->dcInHold=0;
		top->clock = (main_time>>0)&1;
		main_time++;
		MemUpdateForBus();
		top->eval();
	}


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
			printf("Clock %d SR0 %d  \n", (int)(main_time/2), n);
			fflush(stdout);
		}

//		addr=0xC000|(n*4);
		addr=0x01000000|(n*4);
		top->dcInAddr[0]=addr;
		top->dcInAddr[1]=0;
		top->dcInAddr[2]=0;
		top->dcInOpm=0x12;
		top->dcInVal=membuf[n];
		top->dcInHold=0;

//		if(inh==1)
//			top->dcInHold=1;
//		if(!inh && (top->dcOutOK==2))
//			{ top->dcInHold=1; }
		if(top->dcOutHold)
			{ top->dcInHold=1; }

//		if(top->clock)
//		{
			MemUpdateForBus();
//		}

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

#if 0
		printf("W %d\n", n);
		n++;
//		if(n>=2048)
		if(n>=4096)
//		if(n>=16384)
			break;
#endif

#if 1
//		if(top->dcOutOK==2)
		if(top->dcInHold)
		{
			printf("Busy %d\n", n);
		}else
			if(inh)
		{
			printf("Inhibit Cycle\n");
			inh--;
		}else
//			if(top->dcOutOK==1)
			if(1)
		{
			printf("W %d\n", n);
			n++;
//			if(n>=8)
//			if(n>=256)
//			if(n>=2048)
//			if(n>=2058)
			if(n>=4096)
//			if(n>=16384)
				break;
			inh=0;
//			inh=1;
//			inh=2;
//			inh=3;
		}else if(top->dcOutOK==0)
		{
			fail|=1;
			printf("Fail, Expect OK\n");
			break;
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
		top->dcInOpm=0x00;
		top->clock = (main_time>>0)&1;
		main_time++;
		MemUpdateForBus();
		top->eval();
	}

#if 1
	n=0; inh=2; rdy=0;
	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
		main_time++;

		if(top->clock)
		{
			printf("Clock %d SR1 %d  \n", (int)(main_time/2), n);
			fflush(stdout);
		}

//		addr=0xC000|(n*4);
		addr=0x01000000|(n*4);
		top->dcInAddr[0]=addr;
		top->dcInAddr[1]=0;
		top->dcInAddr[2]=0;
		top->dcInOpm=0x0A;
		top->dcInHold=0;
//		top->dcInVal=membuf[n];
		v=top->dcOutVal;
		v1=membuf[n];

//		if(inh==1)
//			top->dcInHold=1;
//		if(top->dcOutOK==2)
//		if(!inh && (top->dcOutOK==2))
		if(top->dcOutHold)
			{ top->dcInHold=1; }

//		if(top->clock)
//		{
			MemUpdateForBus();
//		}
		top->eval();

		if(!top->clock)
			continue;

//		if(top->dcOutHold)
//			{ top->dcInHold=1; continue; }

#if 0
		if(inh)
		{
			printf("Inhibit Cycle\n");
			inh--;
			continue;
		}
#endif

		v=top->dcOutVal;

//		if(top->dcOutOK==2)
		if(top->dcInHold)
		{
			printf("Busy\n");
		}else
			if(inh)
		{
			printf("Inhibit Cycle\n");
			inh--;
		}else
//			if(top->dcOutOK==1)
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
		
//			printf("W %d\n", n);
//			inh=1;
			inh=2;
//			inh=3;
			n++;
//			if(n>=8)
//			if(n>=256)
			if(n>=2048)
//			if(n>=16384)
				break;
		}
#if 0
		else if(top->dcOutOK==0)
		{
			fail|=1;
			printf("Fail, Expect OK\n");
			break;
		}
#endif
	}

	if(fail)
	{
		delete top;
		exit(0);
	}
#endif



#if 1
	top->dcInHold=0;
	n=0; inh=2; rdy=0;
	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
		main_time++;

		if(top->clock)
		{
			printf("Clock %d SR2 %d  \n", (int)(main_time/2), n);
			fflush(stdout);
		}

//		addr=0xC000|(n*4);
		addr=0x01000000|(n*8);
		top->dcInAddr[0]=addr;
		top->dcInAddr[1]=0;
		top->dcInAddr[2]=0;
		top->dcInOpm=0x0F;
		top->dcInHold=0;
//		top->dcInVal=membuf[n];
		lv0=top->dcOutVal;
		lv1=top->dcOutValB;

		v0=membuf[n*2+0];
		v1=membuf[n*2+1];
		v2=membuf[n*2+2];
		v3=membuf[n*2+3];
		lv2=v0|(((uint64_t)v1)<<32);
		lv3=v2|(((uint64_t)v3)<<32);

//		if(inh==1)
//			top->dcInHold=1;
//		if(top->dcOutOK==2)
//		if(!inh && (top->dcOutOK==2))
//			{ top->dcInHold=1; }
		if(top->dcOutHold)
			{ top->dcInHold=1; }

//		if(top->clock)
//		{
			MemUpdateForBus();
//		}
		top->eval();

		if(!top->clock)
			continue;

//		if(top->dcOutHold)
//			{ top->dcInHold=1; }

#if 0
		if(inh)
		{
			printf("Inhibit Cycle\n");
			inh--;
			continue;
		}
#endif

		lv0=top->dcOutVal;
		lv1=top->dcOutValB;

		v0=membuf[n*2+0];
		v1=membuf[n*2+1];
		v2=membuf[n*2+2];
		v3=membuf[n*2+3];
		lv2=v0|(((uint64_t)v1)<<32);
		lv3=v2|(((uint64_t)v3)<<32);

//		if(top->dcOutOK==2)
		if(top->dcInHold)
		{
			printf("Busy\n");
		}else
			if(inh)
		{
			printf("Inhibit Cycle\n");
			inh--;
		}else
//			if(top->dcOutOK==1)
		{
			printf("SR2 %04X Got=%016llX-%016llX Expect=%016llX-%016llX %s\n",
				addr, lv0, lv1, lv2, lv3,
					((lv0==lv2) && (lv1==lv3))?"Pass":"Fail");
					
//			if(v1!=v)
			if(((lv0!=lv2) || (lv1!=lv3)))
			{
				printf("Mismatch, Ending Test (SRAM)\n");
				fail|=2;
				break;
			}
		
//			printf("W %d\n", n);
//			inh=1;
			inh=2;
//			inh=3;
			n++;
//			if(n>=2048)
//			if(n>=2047)
			if(n>=2046)
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

#if 0
	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
		main_time++;
		top->dcInAddr=0;
		top->dcInOpm=0;
		top->dcInHold=0;
		top->dcInVal=0;
		if(top->dcOutHold)
			{ top->dcInHold=1; }
		MemUpdateForBus();
		top->eval();
		if(!top->clock)
			continue;
		if(!top->dcInHold)
			break;
	}
#endif

#if 1
	top->dcInHold=0;
	n=0; inh=2; rdy=0;
	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
		main_time++;

		if(top->clock)
		{
//			printf("Clock %d MMIO-0 %d  \n", (int)(main_time/2), n);
			fflush(stdout);
		}

		addr=0xF0000000|(n*4);
		top->dcInAddr[0]=addr;
		top->dcInAddr[1]=0;
		top->dcInAddr[2]=0;
		top->dcInOpm=0x12;
		top->dcInHold=0;
		top->dcInVal=membuf[n];

		if(top->dcOutHold)
			{ top->dcInHold=1; }

		MemUpdateForBus();
		top->eval();

		if(!top->clock)
			continue;

		if(inh)
		{
			printf("MMIO: Inhibit Cycle\n");
			inh--;
			continue;
		}

		v=top->dcOutVal;

		if(top->dcInHold)
		{
//			printf("MMIO: Busy\n");
		}else
		{		
			printf("MMIO W %d\n", n);
			inh=0;
//			inh=1;
//			inh=2;
//			inh=3;
			n++;
			if(n>=256)
				break;
		}
	}

	if(fail)
	{
		delete top;
		exit(0);
	}
#endif

#if 0
	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
		main_time++;
		top->dcInAddr=0;
		top->dcInOpm=0;
		top->dcInHold=0;
		top->dcInVal=0;
		if(top->dcOutHold)
			{ top->dcInHold=1; }
		MemUpdateForBus();
		top->eval();
		if(!top->clock)
			continue;
		if(!top->dcInHold)
			break;
	}
#endif

#if 1
	top->dcInHold=0;
	n=0; inh=2; rdy=0;
	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
		main_time++;

		if(top->clock)
		{
			printf("Clock %d MMIO-1 %d  \n", (int)(main_time/2), n);
			fflush(stdout);
		}

//		addr=0xC000|(n*4);
		addr=0xF0000000|(n*4);
		top->dcInAddr[0]=addr;
		top->dcInAddr[1]=0;
		top->dcInAddr[2]=0;
		top->dcInOpm=0x0A;
		top->dcInHold=0;
//		top->dcInVal=membuf[n];
		v=top->dcOutVal;
		v1=membuf[n];

		if(top->dcOutHold)
			{ top->dcInHold=1; }

		MemUpdateForBus();
		top->eval();

		if(!top->clock)
			continue;

#if 0
		if(inh)
		{
			printf("Inhibit Cycle\n");
			inh--;
			continue;
		}
#endif

		v=top->dcOutVal;

		if(top->dcInHold)
		{
			printf("Busy\n");
		}else
			if(inh)
		{
			printf("Inhibit Cycle\n");
			inh--;
		}else
		{
			printf("MMIO-1 %04X Got=%08X Expect=%08X %s\n",
				addr, v, v1,
					(v1==v)?"Pass":"Fail");
					
			if(v1!=v)
			{
				printf("Mismatch, Ending Test (MMIO)\n");
				fail|=2;
				break;
			}
		
//			printf("W %d\n", n);
//			inh=1;
			inh=2;
//			inh=3;
			n++;
			if(n>=256)
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
