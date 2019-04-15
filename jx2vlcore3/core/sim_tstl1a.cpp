#include "VMemL1A.h"
#include "verilated.h"

VMemL1A *top = new VMemL1A;

vluint64_t main_time = 0;

// static int write_words=256;
static int write_words=2048;

static int write_words2=4096;

//static int write_words2=8192;
// static int write_words2=16384;
// static int write_words2=32768;
// static int write_words2=65536;

uint32_t *rombuf;
uint32_t *srambuf;
uint32_t *drambuf;

int LoadRomBuf()
{
	char tba[64], tbb[64], tbc[64], tbd[64];
	FILE *fda, *fdb, *fdc, *fdd;
	uint32_t va, vb, vc, vd;
	int n;
	
	fda=fopen("bootrom_1a.txt", "rt");
	fdb=fopen("bootrom_1b.txt", "rt");
	fdc=fopen("bootrom_1c.txt", "rt");
	fdd=fopen("bootrom_1d.txt", "rt");
	
	rombuf=(uint32_t *)(malloc(32768));
	memset(rombuf, 0, 32768);
	n=0;
	
	while(!feof(fda) && (n<2048))
	{
		memset(tba, 0, 64); va=0;
		memset(tbb, 0, 64); vb=0;
		memset(tbc, 0, 64); vc=0;
		memset(tbd, 0, 64); vd=0;
		fgets(tba, 64, fda);
		fgets(tbb, 64, fdb);
		fgets(tbc, 64, fdc);
		fgets(tbd, 64, fdd);
		sscanf(tba, "%08X", &va);
		sscanf(tbb, "%08X", &vb);
		sscanf(tbc, "%08X", &vc);
		sscanf(tbd, "%08X", &vd);
		
//		printf("%04X  %s %s %s %s\n", n*16, tba, tbb, tbc, tbd);
		printf("%04X  %08X %08X %08X %08X\n", n*16, va, vb, vc, vd);
		
		rombuf[n*4+0]=va;
		rombuf[n*4+1]=vb;
		rombuf[n*4+2]=vc;
		rombuf[n*4+3]=vd;
		n++;
	}
	
	printf("ROM, Loaded %d bytes\n", n*16);
	
//	fgetc(stdin);
	
	fclose(fda);
	fclose(fdb);
	fclose(fdc);
	fclose(fdd);
	return(0);
}

int GetRomWord(int n)
{
	uint32_t v;
	
	v=rombuf[n>>1];
	if(n&1)v>>=16;
	return(v&0xFFFF);
}

void MemUpdateForBus()
{
	int addr, isRom, isSRam, isDRam;

	if(top->memOpm)
	{
		addr=top->memAddr;
		top->memOK=0;
		
		isRom	= (addr>=0x00000000) && (addr<=0x00007FFF);
//		isSRam	= (addr>=0x0000C000) && (addr<=0x0000DFFF);
		isSRam	= (addr>=0x0000C000) && (addr<=0x0000E010);
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

#if 0
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

			printf("%08X ST  %08X %08X %08X %08X\n",
				addr,
				top->memDataOut[0], top->memDataOut[1],
				top->memDataOut[2], top->memDataOut[3]);
		}
	}else
	{
		top->memOK=0;
	}
}

int main(int argc, char **argv, char **env)
{
//	uint32_t *drambuf;
	uint32_t *membuf;
	uint32_t v, v1;
	int fail;
	int addr, n, n1, inh, stp, stp1, rdy;
	int i, j, k;

//	LoadRomBuf();

	membuf=(uint32_t *)malloc(16384);
	for(i=0; i<(16384/4); i++)
	{
		membuf[i]=rand()*rand();
	}

	drambuf=(uint32_t *)malloc(1<<24);
	srambuf=(uint32_t *)malloc(8192);

	LoadRomBuf();

	Verilated::commandArgs(argc, argv);

//	LoadRomBuf();

	fail=0;

	n=0;
	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
		main_time++;

		top->dcInAddr=0;
		top->dcInOpm=0;

//		addr=0xC000|(n*2);
		addr=0x0000|(n*2);
		top->icInPcAddr=addr;
//		top->icRegInOpm=0xA;
//		top->icRegInData=rand();

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
		
			v=(top->icOutPcVal)&65535;;
			printf("%04X ", v);
			
			v1=GetRomWord(n);
			
			if(v!=v1)
			{
				printf("Mismatch (IC1) Got=%04X Expect=%04X A=%04X\n",
					v, v1, n*2);
				fail|=1;
				break;
			}
			
			stp1=1;
			if((v&0xE000)==0xE000)
			{
				stp1=2;
				if((v&0xEE00)==0xEC00)
					{ stp1=3; }
				if((v&0xEE00)==0xEE00)
					{ stp1=3; }
			}
			
			if(stp!=stp1)
			{
				printf("Step Mismatch %04X != %04X\n", stp, stp1);
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

		top->dcInAddr=0;
		top->dcInOpm=0;

		n1=(n^(n>>8))&1023;

		addr=0x0000|(n1*2);
		top->icInPcAddr=addr;

		MemUpdateForBus();
		top->eval();

		if(!top->clock)
			continue;

		printf("%d/%d\r", n, 32752/2);
		fflush(stdout);

		if(top->icOutPcOK==1)
		{
			stp=top->icOutPcStep;
		
			v=(top->icOutPcVal)&65535;;
			
			v1=GetRomWord(n1);
			
			if(v!=v1)
			{
				printf("Mismatch (IC2) Got=%04X Expect=%04X A=%04X\n",
					v, v1, n1*2);
				fail|=1;
				break;
			}
			
			stp1=1;
			if((v&0xE000)==0xE000)
			{
				stp1=2;
				if((v&0xEE00)==0xEC00)
					{ stp1=3; }
				if((v&0xEE00)==0xEE00)
					{ stp1=3; }
			}
			
			if(stp!=stp1)
			{
				printf("Step Mismatch %04X != %04X\n", stp, stp1);
				fail|=1;
				break;
			}

			n++;
		}
		
		if((n*2)>=32752)
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


#if 1
	n=0; inh=2; rdy=0;
	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
		main_time++;

		if(top->clock)
		{
			printf("Clock %d SR %d  \n", (int)(main_time/2), n);
			fflush(stdout);
		}

		addr=0xC000|(n*4);
		top->dcInAddr=addr;
		top->dcInOpm=0x12;
		top->dcInVal=membuf[n];

		MemUpdateForBus();
		top->eval();

		if(!top->clock)
			continue;

		if(top->dcOutOK==1)
		{
			printf("W %d\n", n);
			n++;
			if(n>=2048)
				break;
		}
	}

	if(fail)
	{
		delete top;
		exit(0);
	}
#endif

#if 1
	n=0; inh=2; rdy=0;
	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
		main_time++;

		if(top->clock)
		{
			printf("Clock %d SR %d  \n", (int)(main_time/2), n);
			fflush(stdout);
		}

		addr=0xC000|(n*4);
		top->dcInAddr=addr;
		top->dcInOpm=0x0A;
//		top->dcInVal=membuf[n];
		v=top->dcOutVal;
		v1=membuf[n];

		MemUpdateForBus();
		top->eval();

		if(!top->clock)
			continue;

		if(top->dcOutOK==1)
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
			n++;
			if(n>=2048)
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
	n=0; inh=2; rdy=0;
	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
		main_time++;

		if(top->clock && (top->clock!=(main_time&1)))
		{
//			printf("Clock %d\n", (int)(main_time/2));
			printf("Clock %d SR %d\n", (int)(main_time/2), n);
		}

//		top->mode = 3;
		
//		top->baseAddr=0xDECAB00;
//		top->idxAddr=0x100;
//		top->idxDisp=3;

		if(inh)
		{
			top->dcRegInOpm=0;
			inh--;

			top->eval();
			continue;
		}

		if(top->dcRegOutOK==0)
			rdy=1;

		if(n<write_words)
//		if(n<1024)
		{
			addr=0xC000|(n*4);
			top->dcRegInAddr=addr;
			top->dcRegInOpm=0x12;
			top->dcRegInData=membuf[n];

#if 0
			if(inh)
			{
				top->dcRegInOpm=0;
				inh--;

				top->eval();
				continue;
			}
#endif

			top->eval();

			if(top->dcRegOutOK==1)
//			if((top->regOutOK==1) && top->clock)
//			if((top->regOutOK==1) && !top->clock)
			{
				printf("W %d\n", n);
				n++;
//				inh=4;
				inh=2;
			}
			
			if(n>=write_words)
//			if(n>=1024)
			{
				main_time=0;
				addr=0xC000;
			}
			
			continue;
		}

		top->dcRegInAddr=addr;
		top->dcRegInOpm=0xA;
		top->dcRegInData=rand();

		top->eval();

		if(top->dcRegOutOK==1)
		{
			v=membuf[(addr>>2)&2047];
			
			inh--;
			if(inh<=0)
			{
				v1=(uint32_t)(top->dcRegOutData);
				printf("%04X Got=%08X Expect=%08X %s\n",
					addr, v1, v,
						(v1==v)?"Pass":"Fail");
						
				if(v1!=v)
				{
					printf("Mismatch, Ending Test (SRAM)\n");
					fail|=2;
					break;
				}
//				addr=0xC000|(rand()&0x1FFC);
				addr=addr+4;
				inh=2;
//				inh=4;
			}
		}else
		{
			inh=0;
		}

//		main_time++;
		
		if(main_time>1024)
//		if(main_time>64)
//		if(main_time>16)
		{
//			printf("%llX\n", (long long)(top->outAddr));
			break;
		}
	}

	if(fail)
	{
		delete top;
		exit(0);
	}
#endif

	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
