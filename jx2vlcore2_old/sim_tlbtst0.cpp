#include "VJx2MmuTlb.h"
#include "verilated.h"

VJx2MmuTlb *top = new VJx2MmuTlb;

vluint64_t main_time = 0;

int main(int argc, char **argv, char **env)
{
	int64_t *ptab;
	int64_t addri, addro, addrp, tea, ptel, pteh;
	int n, exc;
	int i, j, k;

	Verilated::commandArgs(argc, argv);


	ptab=(int64_t *)malloc(4096*sizeof(int64_t));
	
	for(i=0; i<4096; i++)
	{
		addri=(i<<12)+(0x01000000);
		ptab[i]=addri|1;
	}
	


	top->regInMMCR=1;
	top->regInKRR=0;
	top->regInSR=0;

	n=0;

	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
		main_time++;

		top->eval();

//		top->mode = 3;
		
//		top->baseAddr=0xDECAB00;
//		top->idxAddr=0x100;
//		top->idxDisp=3;

		addri=0x00100000+(n*251);
		top->regInAddr=addri;
		top->regInOpm=0x00;

		addro=top->regOutAddr;

		addrp=(ptab[(addri>>12)&4095]&0xFFFFFFFFF000ULL)|(addri&0x0FFF);
		
		if(top->regOutOK==2)
		{
			printf("TLB Hold\n");
			continue;
		}
		
		if(top->regOutExc&0x8000)
		{
			exc=top->regOutExc;
			tea=top->regOutTea;
			printf("EXC: %04X %08X\n", (int)exc, (int)tea);
			
			if((exc&0xFFFF)==0xA001)
			{
				printf("TLB Miss\n");
				
				ptel=ptab[(tea>>12)&4095];
				pteh=tea&0xFFFFFFFFF000ULL;
				
				top->regInOpm=0x04;
				top->regInData[0]=ptel;
				top->regInData[1]=ptel>>32;
				top->regInData[2]=pteh;
				top->regInData[3]=pteh>>32;
			}
		}
		
		if(top->clock)
		{
			printf("In=%016llX Out=%016llX Expect=%016llX %s\n",
				(long long)addri,
				(long long)addro,
				(long long)addrp,
				(addro==addrp)?"Pass":"Fail");
			
			if(addro==addrp)
				n++;
			if(((n*251)>>12)>=3072)
				break;
		}
		
//		if(main_time>64)
//		{
//			printf("%llX\n", (long long)(top->outAddr));
//			break;
//		}
	}
	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
