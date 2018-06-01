#include "VBsrTstMem.h"
#include "verilated.h"

VBsrTstMem *top = new VBsrTstMem;

vluint64_t main_time = 0;

int main(int argc, char **argv, char **env)
{
	uint32_t *membuf;
	uint32_t v;
	int addr, n, inh;
	int i, j, k;

	membuf=(uint32_t *)malloc(16384);
	for(i=0; i<(16384/4); i++)
	{
		membuf[i]=rand()*rand();
	}

	Verilated::commandArgs(argc, argv);

	n=0; inh=2;
	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
		main_time++;

		if(top->clock && (top->clock!=(main_time&1)))
		{
			printf("Clock %d\n", main_time/2);
		}

//		top->mode = 3;
		
//		top->baseAddr=0xDECAB00;
//		top->idxAddr=0x100;
//		top->idxDisp=3;

		if(n<2048)
//		if(n<1024)
		{
			addr=0xC000|(n*4);
			top->regInAddr=addr;
			top->regInOpm=0x12;
			top->regInData=membuf[n];

			if(inh)
			{
				top->regInOpm=0;
				inh--;

				top->eval();
				continue;
			}

			top->eval();

			if(top->regOutOK==1)
//			if((top->regOutOK==1) && top->clock)
//			if((top->regOutOK==1) && !top->clock)
			{
				printf("W %d\n", n);
				n++; inh=4;
			}
			
			if(n>=2048)
//			if(n>=1024)
			{
				main_time=0;
				addr=0xC000;
			}
			
			continue;
		}

		top->regInAddr=addr;
		top->regInOpm=0xA;
		top->regInData=rand();

		top->eval();

		if(top->regOutOK==1)
		{
			v=membuf[(addr>>2)&2047];
			
			inh--;
			if(inh<=0)
			{
				printf("%04X Got=%08X Expect=%08X %s\n",
					addr, top->regOutData, v,
						(top->regOutData==v)?"Pass":"Fail");
						
				if(top->regOutData!=v)
					break;
//				addr=0xC000|(rand()&0x1FFC);
				addr=addr+4;
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
	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
