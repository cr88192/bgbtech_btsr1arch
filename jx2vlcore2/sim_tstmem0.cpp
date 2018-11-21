#include "VJx2MemCache.h"
#include "verilated.h"

VJx2MemCache *top = new VJx2MemCache;

vluint64_t main_time = 0;

// static int write_words=256;
static int write_words=2048;

static int write_words2=4096;

//static int write_words2=8192;
//static int write_words2=16384;
// static int write_words2=32768;
// static int write_words2=65536;

uint32_t *rombuf;

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

int main(int argc, char **argv, char **env)
{
	uint32_t *drambuf;
	uint32_t *membuf;
	uint32_t v, v1;
	int fail;
	int addr, n, inh, stp, stp1, rdy;
	int i, j, k;

//	LoadRomBuf();

	membuf=(uint32_t *)malloc(16384);
	for(i=0; i<(16384/4); i++)
	{
		membuf[i]=rand()*rand();
	}

	drambuf=(uint32_t *)malloc(1<<24);

	LoadRomBuf();

	Verilated::commandArgs(argc, argv);

//	LoadRomBuf();

	fail=0;

	n=0;
	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
		main_time++;

		top->dcRegInAddr=0;
		top->dcRegInOpm=0;

//		addr=0xC000|(n*2);
		addr=0x0000|(n*2);
		top->icRegInPc=addr;
//		top->icRegInOpm=0xA;
//		top->icRegInData=rand();

		top->eval();

		if(top->icRegOutPcOK==1)
		{
			if(!(n&7))
			{
				printf("%04X  ", n*2);
			}
		
			stp=top->icRegStepPc;
		
			v=(top->icRegOutPcVal)&65535;;
			printf("%04X ", v);
			
			v1=GetRomWord(n);
			
			if(v!=v1)
			{
				printf("Mismatch Got=%04X Expect=%04X A=%04X\n", v, v1, n*2);
				fail|=1;
				break;
			}
			
			stp1=1;
			if((v&0xF000)==0xF000)
			{
				stp1=2;
				if((v&0xFE00)==0xFC00)
					{ stp1=3; }
				if((v&0xFE00)==0xFE00)
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

#if 0
	delete top;
	exit(0);
#endif

	n=0; inh=2; rdy=0;
	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
		main_time++;

		if(top->clock && (top->clock!=(main_time&1)))
		{
			printf("Clock %d\n", (int)(main_time/2));
		}

//		top->mode = 3;
		
//		top->baseAddr=0xDECAB00;
//		top->idxAddr=0x100;
//		top->idxDisp=3;

		if(top->dcRegOutOK==0)
			rdy=1;

		if(n<write_words)
//		if(n<1024)
		{
			addr=0xC000|(n*4);
			top->dcRegInAddr=addr;
			top->dcRegInOpm=0x12;
			top->dcRegInData=membuf[n];

			if(inh)
			{
				top->dcRegInOpm=0;
				inh--;

				top->eval();
				continue;
			}

			top->eval();

			if(top->dcRegOutOK==1)
//			if((top->regOutOK==1) && top->clock)
//			if((top->regOutOK==1) && !top->clock)
			{
				printf("W %d\n", n);
				n++;
				inh=4;
//				inh=2;
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
//				inh=2;
				inh=4;
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


	top->dcRegInOpm=0x00;
	top->dcRegInAddr=0;

#if 1
	n=0; inh=4;
	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
		main_time++;

		if(top->clock && (top->clock!=(main_time&1)))
		{
			printf("Clock %d\n", (int)(main_time/2));
		}

		if(n>=write_words2)
//		if(n>=(write_words+256))
			break;

		if(top->memOpm)
		{
			addr=top->memAddr;
			top->memOK=0;

			if(top->memOpm&0x08)
			{			
				top->memInData[0]=drambuf[((addr>>2)+0)&0x3FFFFF];
				top->memInData[1]=drambuf[((addr>>2)+1)&0x3FFFFF];
				top->memInData[2]=drambuf[((addr>>2)+2)&0x3FFFFF];
				top->memInData[3]=drambuf[((addr>>2)+3)&0x3FFFFF];
				top->memOK=1;

				printf("%08X LD  %08X %08X %08X %08X\n",
					addr,
					top->memInData[0], top->memInData[1],
					top->memInData[2], top->memInData[3]);
			}

			if(top->memOpm&0x10)
			{
				drambuf[((addr>>2)+0)&0x3FFFFF]=top->memOutData[0];
				drambuf[((addr>>2)+1)&0x3FFFFF]=top->memOutData[1];
				drambuf[((addr>>2)+2)&0x3FFFFF]=top->memOutData[2];
				drambuf[((addr>>2)+3)&0x3FFFFF]=top->memOutData[3];
				top->memOK=1;

				printf("%08X ST  %08X %08X %08X %08X\n",
					addr,
					top->memOutData[0], top->memOutData[1],
					top->memOutData[2], top->memOutData[3]);
			}
		}else
		{
			top->memOK=0;
		}

		addr=0x01000000+(n*4);
		top->dcRegInAddr=addr;
		top->dcRegInOpm=0x12;
//		top->dcRegInData=membuf[n];

		k=((n*65521)>>16)&2047;
		top->dcRegInData=membuf[k];

		if(inh)
		{
			top->dcRegInOpm=0;
			inh--;

			top->eval();
			continue;
		}

		top->eval();

		if(top->dcRegOutOK==1)
		{
			printf("W %d\n", n);
			n++;
//			inh=2;
			inh=4;
		}
	}
#endif


	top->dcRegInOpm=0x00;
	top->dcRegInAddr=0;
	top->memOK=0;

	n=0; inh=16;
	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
		main_time++;

		if(top->clock && (top->clock!=(main_time&1)))
		{
			printf("Clock %d\n", (int)(main_time/2));
		}

		if(inh)
		{
			top->eval();
			inh--;
			continue;
		}
		
		if(n>=write_words2)
			break;

		if(top->memOpm)
		{
			addr=top->memAddr;

			if(top->memOpm&0x08)
			{			
				top->memInData[0]=drambuf[((addr>>2)+0)&0x3FFFFF];
				top->memInData[1]=drambuf[((addr>>2)+1)&0x3FFFFF];
				top->memInData[2]=drambuf[((addr>>2)+2)&0x3FFFFF];
				top->memInData[3]=drambuf[((addr>>2)+3)&0x3FFFFF];
				top->memOK=1;

				printf("%08X LD2  %08X %08X %08X %08X\n",
					addr,
					top->memInData[0], top->memInData[1],
					top->memInData[2], top->memInData[3]);
			}

			if(top->memOpm&0x10)
			{
				drambuf[((addr>>2)+0)&0x3FFFFF]=top->memOutData[0];
				drambuf[((addr>>2)+1)&0x3FFFFF]=top->memOutData[1];
				drambuf[((addr>>2)+2)&0x3FFFFF]=top->memOutData[2];
				drambuf[((addr>>2)+3)&0x3FFFFF]=top->memOutData[3];
				top->memOK=1;

				printf("%08X ST2  %08X %08X %08X %08X\n",
					addr,
					top->memOutData[0], top->memOutData[1],
					top->memOutData[2], top->memOutData[3]);
			}
		}else
		{
			top->memOK=0;
		}

		addr=0x01000000+(n*4);

		top->dcRegInAddr=addr;
		top->dcRegInOpm=0xA;
		top->dcRegInData=rand();

		top->eval();

		if(top->dcRegOutOK==1)
		{
//			v=membuf[(addr>>2)&2047];
			k=((n*65521)>>16)&2047;
			v=membuf[k];
			
			inh--;
			if(inh<=0)
			{
				v1=(uint32_t)(top->dcRegOutData);
				printf("%04X Got=%08X Expect=%08X %s\n",
					addr, v1, v,
						(v1==v)?"Pass":"Fail");
						
				if(v1!=v)
				{
					printf("Mismatch, Ending Test (DRAM)\n");
					break;
				}

				top->dcRegInOpm=0;

//				addr=0xC000|(rand()&0x1FFC);
//				addr=addr+4;
				n++;
//				inh=2;
				inh=4;
			}
		}else
		{
			inh=0;
		}
	}

	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
