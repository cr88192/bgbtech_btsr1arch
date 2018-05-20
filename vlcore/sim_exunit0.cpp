#include "VBsrExUnit.h"
#include "verilated.h"

VBsrExUnit *top = new VBsrExUnit;

vluint64_t main_time = 0;

int main(int argc, char **argv, char **env)
{
	int lclk;
	Verilated::commandArgs(argc, argv);

	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
		
		if(top->clock && (lclk!=top->clock))
		{
			printf("Cycle\n");
		}
		
		lclk = top->clock;
//		top->mode = 3;
		
//		top->baseAddr=0xDECAB00;
//		top->idxAddr=0x100;
//		top->idxDisp=3;

		top->eval();

		main_time++;
		
		if(main_time>64)
		{
//			printf("%llX\n", (long long)(top->outAddr));
			break;
		}
	}
	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
