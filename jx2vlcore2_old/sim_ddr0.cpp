#include "VJx2ModDdr.h"
#include "verilated.h"

VJx2ModDdr *top = new VJx2ModDdr;

vluint64_t main_time = 0;

uint16_t	*ddr_ram;

int ddr_cmdhi, ddr_cmdlo;
int ddr_cyc;

int ddr_parm_wl=3;	//CAS WL*2+1
int ddr_parm_rl=7;	//CAS RL*2+1
int ddr_row, ddr_col, ddr_bank;
int ddr_state, ddr_cas, ddr_burst;

int SimDdr(int clk, int cmd, int *rdata)
{
	int data, row, col, bank, pos;
	
	data=*rdata;
//	printf("%03X %04X\n", cmd, data);

	if(!(clk&1))
	{
		ddr_cmdhi=cmd;
	}else
	{
		ddr_cmdlo=cmd;
		ddr_cyc++;
	}

	if((ddr_state==1) && (ddr_cas<=0))
	{
		printf("%04d %03X-%03X %04X  ", ddr_cyc, ddr_cmdhi, ddr_cmdlo, data);

		pos=(ddr_row<<13)+(ddr_bank<<10)+ddr_col;
		pos&=(1<<28)-1;
		data=ddr_ram[pos>>1];
		ddr_col+=2;

		printf("Read, Bl=%d A=%08X Col=%03X D=%04X\n",
			ddr_burst, pos, (ddr_col-2), data);
		
		ddr_burst--;
		if(ddr_burst<=0)
			{ ddr_state=0; }

		*rdata=data;
		return(0);
	}

	if((ddr_state==2) && (ddr_cas<=0))
	{
		printf("%04d %03X-%03X %04X  ", ddr_cyc, ddr_cmdhi, ddr_cmdlo, data);

		pos=(ddr_row<<13)+(ddr_bank<<10)+ddr_col;
		pos&=(1<<28)-1;
		ddr_ram[pos>>1]=data;
		ddr_col+=2;

		printf("Write, Bl=%d A=%08X Col=%03X D=%04X\n",
			ddr_burst, pos, (ddr_col-2), data);
		
		ddr_burst--;
		if(ddr_burst<=0)
			{ ddr_state=0; }

		*rdata=data;
		return(0);
	}

//	if(clk&1)
	if(!(clk&1))
	{
//		ddr_cmdhi=cmd;
		if(ddr_cas>0)
		{
//			printf("CAS=%d\n", ddr_cas);
			ddr_cas--;
//			break;
		}
	}else
	{
//		ddr_cmdlo=cmd;
//		ddr_cyc++;

		printf("%04d %03X-%03X %04X  ", ddr_cyc, ddr_cmdhi, ddr_cmdlo, data);

		switch(ddr_cmdhi&7)
		{
		case 7:
			if(ddr_cas>0)
			{
				printf("CAS=%d\n", ddr_cas>>1);
				ddr_cas--;
				break;
			}
			printf("NOP\n");
			break;
		case 3:
			switch((ddr_cmdhi>>3)&3)
			{
			case 3:
				printf("Precharge All Banks\n");
				break;
			case 1:
				printf("Precharge Bank %d\n", (cmd>>7)&7);
				break;
			case 0: case 2:
				printf("Burst Terminate\n");
				break;
			}
			break;
		case 5:
			col=((ddr_cmdlo&0x3FE)<<1)|((ddr_cmdlo>>5)&3);
			col<<=1;
			bank=(ddr_cmdhi>>7)&7;
			printf("Read Col=%03X Bank=%X\n", col, bank);
			ddr_col=col;
			ddr_bank=bank;
			ddr_cas=ddr_parm_rl;
			ddr_burst=8;
			ddr_state=1;
			break;
		case 1:
			col=((ddr_cmdlo&0x3FE)<<1)|((ddr_cmdlo>>5)&3);
			col<<=1;
			bank=(ddr_cmdhi>>7)&7;
			printf("Write Col=%03X Bank=%X\n", col, bank);
			ddr_col=col;
			ddr_cas=ddr_parm_wl;
			ddr_burst=8;
			ddr_state=2;
			break;
		case 2: case 6:
			row=(ddr_cmdlo&1023)|(((ddr_cmdhi>>2)&31)<<10);
			bank=(ddr_cmdhi>>7)&7;
			printf("Activate Row=%03X Bank=%X\n", row, bank);
			ddr_row=row;
			break;
		case 4:
			printf("Refresh One Bank\n");
			break;
		case 0:
			if(ddr_cmdhi&8)
			{
				printf("Mode Read\n");
			}else
			{
				row=((ddr_cmdhi>>4)&63)|((ddr_cmdlo&3)<<6);
				col=(ddr_cmdlo>>2)&255;
				printf("Mode Write: Reg=%02X Val=%02X\n", row, col);
				
				switch(row)
				{
				case 0x02:
					switch(col&15)
					{
#if 0
					case 1: ddr_parm_rl=3*2+1; ddr_parm_wl=1*2+1; break;
					case 2: ddr_parm_rl=4*2+1; ddr_parm_wl=2*2+1; break;
					case 3: ddr_parm_rl=5*2+1; ddr_parm_wl=2*2+1; break;
					case 4: ddr_parm_rl=6*2+1; ddr_parm_wl=3*2+1; break;
					case 5: ddr_parm_rl=7*2+1; ddr_parm_wl=4*2+1; break;
					case 6: ddr_parm_rl=8*2+1; ddr_parm_wl=4*2+1; break;
#endif

					case 1: ddr_parm_rl=3*2+0; ddr_parm_wl=1*2+1; break;
					case 2: ddr_parm_rl=4*2+0; ddr_parm_wl=2*2+1; break;
					case 3: ddr_parm_rl=5*2+0; ddr_parm_wl=2*2+1; break;
					case 4: ddr_parm_rl=6*2+0; ddr_parm_wl=3*2+1; break;
					case 5: ddr_parm_rl=7*2+0; ddr_parm_wl=4*2+1; break;
					case 6: ddr_parm_rl=8*2+0; ddr_parm_wl=4*2+1; break;
					default:
						break;
					}
					break;
				default:
					break;
				}
			}
			break;
		}
	}
	
	*rdata=data;
}

int main(int argc, char **argv, char **env)
{
	int ddrlclk, cmd, data;
	int n, inh;
	int i, j, k;

	Verilated::commandArgs(argc, argv);
	
	ddr_ram=(uint16_t *)malloc(256<<20);
	
	for(i=0; i<(128<<20); i++)
		ddr_ram[i]=rand();

	top->memOpm=0x0;
	n=0;
	inh=256;
	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
		main_time++;

		top->eval();

		if(top->ddrClk!=ddrlclk)
		{
			ddrlclk=top->ddrClk;
			cmd=top->ddrCmd;
//			data=top->ddrData;
			data=top->ddrData_O;
			SimDdr(top->ddrClk, cmd, &data);
//			top->ddrData=data;
			top->ddrData_I=data;
		}
		
		if(inh>0)
		{
			inh--;
			continue;
		}
		
		top->memOpm=0x0F;
		top->memAddr=n*16;
		
//		printf("OK=%d\n", top->memOK);
		
		if(top->memOK==1)
		{
			printf("Got %08X %08X %08X %08X\n",
				top->memDataOut[0], top->memDataOut[1],
				top->memDataOut[2], top->memDataOut[3]);
		
			top->memOpm=0x00;
			inh=4;
			n++;
		}
		
		if(main_time>768)
			break;
	}
	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
