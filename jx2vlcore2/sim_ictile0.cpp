#include "VBsrIcTile.h"
#include "verilated.h"

VBsrIcTile *top = new VBsrIcTile;

vluint64_t main_time = 0;

int main(int argc, char **argv, char **env)
{
	Verilated::commandArgs(argc, argv);

	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
//		top->mode = 3;
		
//		top->baseAddr=0xDECAB00;
//		top->idxAddr=0x100;
//		top->idxDisp=3;

		top->eval();

		main_time++;
		
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
