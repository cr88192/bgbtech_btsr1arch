#include "VMmiModDdr3.h"
#include "verilated.h"

VMmiModDdr3 *top = new VMmiModDdr3;

vluint64_t main_time = 0;

uint16_t	*ddr_ram;

int ddr_cmdhi, ddr_cmdlo;
int ddr_cyc;

int ddr_parm_wl=3;	//CAS WL*2+1
int ddr_parm_rl=7;	//CAS RL*2+1
int ddr_row, ddr_col, ddr_bank;
int ddr_state, ddr_cas, ddr_burst;

int ddr_burstlen;
int ddr_mr0;
int ddr_mr1;
int ddr_mr2;
int ddr_mr3;

#if 0
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
#endif

#define	SIMDDR_BUSRT	4
// #define	SIMDDR_BUSRT	8

#define SIMDDR_SHL_A15	23
#define SIMDDR_SHL_A14	22
#define SIMDDR_SHL_A13	21

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

#define SIMDDR_MSK_NOP	(SIMDDR_MSK_RAS|SIMDDR_MSK_CAS|SIMDDR_MSK_WE)


#if 1
int SimDdr(int clk, int cmd, int *rdqs, int *rdata)
{
	int		data, row, col, bank, pos;
	int addr, cas, dqs;

//Cs;
//			cmd=(cmd<<1)|top->ddrRas;
//			cmd=(cmd<<1)|top->ddrCas;
//			cmd=(cmd<<1)|top->ddrWe;
//			cmd=(cmd<<1)|top->ddrCke;
//			cmd=(cmd<<2)|top->ddrBa

	dqs=*rdqs;
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
		if(ddr_state==1)
			*rdqs=2;
	}else if(ddr_state==1)
	{
		if(ddr_burst>0)
		{
			ddr_burst--;

			pos=(ddr_row<<13)+(ddr_bank<<10)+ddr_col;
			pos&=(1<<28)-1;
			data=ddr_ram[pos>>1];
			ddr_col+=2;

			*rdata=data;
			*rdqs=(ddr_burst&1)?1:2;

//			printf("RD %08X: %04X\n", pos, data);

		}
	}else if(ddr_state==2)
	{
		if(ddr_burst>0)
		{
			if((dqs&3)!=(clk&3))
			{
				printf("DDR DQS Issue %d!=%d\n", dqs&3, clk&3);
			}
			
			ddr_burst--;

			data=*rdata;
			pos=(ddr_row<<13)+(ddr_bank<<10)+ddr_col;
			pos&=(1<<28)-1;
			ddr_ram[pos>>1]=data;
			ddr_col+=2;
			
//			printf("ST %08X = %04X\n", pos, data);
		}
	}
	
//	if(!clk)
	if(!(clk&1))
	{
		return(0);
	}
	
//	addr=(cmd&0x3FFF);
	addr=(cmd&0x1FFF);
	bank=(cmd>>13)&7;
	addr|=((cmd>>SIMDDR_SHL_A13)&7)<<13;
	
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
//				printf("No-Op\n");
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
				ddr_bank=bank;
				ddr_state=1;
//				ddr_cas=ddr_parm_rl*2+1;
//				ddr_cas=ddr_parm_rl*2;
				ddr_cas=ddr_parm_rl*2-1;
//				ddr_cas=ddr_parm_rl*2-2;
//				ddr_cas=4*2+1;
				ddr_burst=ddr_burstlen;
			}else
			{
//				printf("Write Active Row\n");
				ddr_col=addr;
				ddr_bank=bank;
				ddr_state=2;
//				ddr_cas=ddr_parm_wl*2+1;
//				ddr_cas=ddr_parm_wl*2;
//				ddr_cas=ddr_parm_wl*2-1;
				ddr_cas=ddr_parm_wl*2-2;
				ddr_burst=ddr_burstlen;
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
				printf("Load Mode Register C=%04X\n", cmd&0xFFFF);
//				switch((cmd>>14)&3)
				switch((cmd>>13)&7)
				{
				case 0:		ddr_mr0=addr;	break;
				case 1:		ddr_mr1=addr;	break;
				case 2:		ddr_mr2=addr;	break;
				case 3:		ddr_mr3=addr;	break;
				}
				
				ddr_burstlen=(ddr_mr0&1)?8:4;
				
				cas=(ddr_mr0>>4)&7;
//				ddr_parm_rl=cas*2+1;		//CAS RL*2+1
//				ddr_parm_wl=ddr_parm_rl-2;	//CAS WL=RL-1
				ddr_parm_rl=cas;			//CAS RL*2+1
				ddr_parm_wl=ddr_parm_rl-1;	//CAS WL=RL-1
				
				printf("BurstLen=%d, CAS=%d\n", ddr_burstlen, cas);

#if 1
				printf("  DLL=%s\n", (ddr_mr1&1)?"Disable":"Enable");
				printf("  ODS=%s\n", (ddr_mr1&2)?"Reduce":"Full");

				printf("  DQS#=%s\n", (ddr_mr1&1024)?"Disable":"Enable");
				printf("  RDQS=%s\n", (ddr_mr1&2048)?"Yes":"No");
				printf("  Outp=%s\n", (ddr_mr1&4096)?"Disable":"Enable");
#endif
			}
		}
	}
}
#endif

int mem_tile32b;

int main(int argc, char **argv, char **env)
{
	uint32_t *imgbuf;
	uint64_t tw0, tw1, td0;
	uint64_t tr0, tr1, td1;
	int ddrlclk, cmd, data, dqs;
	int n, inh;
	int wn, rn, wdn, rdn, lim, bn1;
	int i, j, k;

	Verilated::commandArgs(argc, argv);
	
	ddr_ram=(uint16_t *)malloc(256<<20);
	
//	for(i=0; i<(128<<20); i++)
//		ddr_ram[i]=rand();

	memset(ddr_ram, 0, 256<<20);


	imgbuf=(uint32_t *)malloc((1<<28)+128);
	
//	for(i=0; i<16777216; i++)
	for(i=0; i<(1<<26); i++)
	{
		k=rand();
		k=k*65521+rand();
		k=k*65521+rand();
		imgbuf[i]=k;
	}

	wn=0;	rn=0;
	wdn=0;	rdn=0;
	
//	lim=8388608-4;

//	lim=6144*1024;

//	lim=4194304;
	lim=1<<18;
//	lim=8;
//	lim=16;
//	lim=64;
//	lim=65;
//	lim=128;

	tw0 = main_time;
	mem_tile32b = 0;

	printf("Begin\n");
	top->memOpm=0x0;
	n=0;
	inh=256;
	while (!Verilated::gotFinish())
	{
		if(main_time<256)
		{
			top->reset=1;
			top->reset2=1;
			
			if(main_time>16)
			{
				i=top->memDataOut[0];
				i=i&65535;
				if(i==0x1234)
				{
					if(!mem_tile32b)
						printf("Detect 32B Cache Line\n");
					mem_tile32b = 1;
				}
			}
		}else
		{
			top->reset=0;
			top->reset2=0;
		}
	
		top->clock = (main_time>>0)&1;
		top->clock2 = (main_time>>0)&1;
		main_time++;

//		if(!top->clock)
//		{
//			top->eval();
//			continue;
//		}

		if(top->reset)
		{
			top->eval();
			continue;
		}

		top->eval();

		if((wn<lim) || wdn)
		{
			if(top->memOK==1)
			{
				top->memOpm=0;
//				printf("Wr OK\n");
				if(!wdn)
				{
					printf("\r%d/%d", wn, lim);
					fflush(stdout);
					wdn=1;
					if(mem_tile32b)
						wn+=2;
					else
						wn++;
				}
			}else
				if(top->memOK==2)
			{
//				printf("Wr Hold\n");
			}else
				if(top->memOK==0)
			{
//				bn1=wn^(0x5A5A5A5A&(lim-1));
				bn1=wn;
			
//				printf("Wr Ready\n");
				top->memAddr=0x1000000 + (bn1*16);
				top->memDataIn[0]=imgbuf[bn1*4+0];
				top->memDataIn[1]=imgbuf[bn1*4+1];
				top->memDataIn[2]=imgbuf[bn1*4+2];
				top->memDataIn[3]=imgbuf[bn1*4+3];
				if(mem_tile32b)
				{
					top->memDataIn[4]=imgbuf[bn1*4+4];
					top->memDataIn[5]=imgbuf[bn1*4+5];
					top->memDataIn[6]=imgbuf[bn1*4+6];
					top->memDataIn[7]=imgbuf[bn1*4+7];
				}
				top->memOpm=0x17;
				wdn=0;

				if(wn>=lim)
				{
					printf("\n");
					tw1 = main_time;
					tr0 = main_time;
				}
			}
		}else
			if((rn<lim) || rdn)
		{
//			bn1=rn^(0xA5A5A5A5&(lim-1));
			bn1=rn;

			if(top->memOK==1)
			{
//				printf("Rd OK\n");
				if(!rdn)
				{
					if(	(top->memDataOut[0]!=imgbuf[bn1*4+0]) ||
						(top->memDataOut[1]!=imgbuf[bn1*4+1]) ||
						(top->memDataOut[2]!=imgbuf[bn1*4+2]) ||
						(top->memDataOut[3]!=imgbuf[bn1*4+3]) )
					{
						printf("Addr=%X\n", top->memAddr);
						printf("\nExpect %08X-%08X-%08X-%08X\n",
							imgbuf[bn1*4+0], imgbuf[bn1*4+1],
							imgbuf[bn1*4+2], imgbuf[bn1*4+3]);
						printf("Got %08X-%08X-%08X-%08X\n",
							top->memDataOut[0], top->memDataOut[1],
							top->memDataOut[2], top->memDataOut[3]);
						break;
					}

					if(mem_tile32b)
					{
						if(	(top->memDataOut[4]!=imgbuf[bn1*4+4]) ||
							(top->memDataOut[5]!=imgbuf[bn1*4+5]) ||
							(top->memDataOut[6]!=imgbuf[bn1*4+6]) ||
							(top->memDataOut[7]!=imgbuf[bn1*4+7]) )
						{
							printf("\nExpect-B %08X-%08X-%08X-%08X\n",
								imgbuf[bn1*4+4], imgbuf[bn1*4+5],
								imgbuf[bn1*4+6], imgbuf[bn1*4+7]);
							printf("Got-B %08X-%08X-%08X-%08X\n",
								top->memDataOut[4], top->memDataOut[5],
								top->memDataOut[6], top->memDataOut[7]);
							break;
						}
					}
				
					printf("\r%d/%d", rn, lim);
					fflush(stdout);
					top->memOpm=0;
					rdn=1;
					if(mem_tile32b)
						rn+=2;
					else
						rn++;
				}
			}else
				if(top->memOK==2)
			{
//				printf("Rd Hold\n");
			}else
				if(top->memOK==0)
			{
//				printf("Rd OK\n");

				top->memAddr=0x1000000 + (bn1*16);
//				top->memDataIn[0]=imgbuf[wn*4+0];
//				top->memDataIn[1]=imgbuf[wn*4+1];
//				top->memDataIn[2]=imgbuf[wn*4+2];
//				top->memDataIn[3]=imgbuf[wn*4+3];
//				top->memOpm=0x17;
				top->memOpm=0x0F;
				rdn=0;

				if(rn>=lim)
				{
					tr1 = main_time;
					printf("\n");
					break;
				}
			}
		}

//		top->eval();

#if 1
		if(top->ddrClk!=ddrlclk)
		{
			dqs=((top->ddrDqsP_O)&1)|((top->ddrDqsN_O)&2);
			if(!top->ddrDqs_En)
				dqs=0;

			ddrlclk=top->ddrClk;
//			cmd=top->ddrCmd;

			cmd=0;
			cmd=(cmd<<3)|((top->ddrAddr>>13)&0x7);
			cmd=(cmd<<1)|top->ddrCs;
			cmd=(cmd<<1)|top->ddrRas;
			cmd=(cmd<<1)|top->ddrCas;
			cmd=(cmd<<1)|top->ddrWe;
			cmd=(cmd<<1)|top->ddrCke;
//			cmd=(cmd<<2)|top->ddrBa;
			cmd=(cmd<<3)|top->ddrBa;
//			cmd=(cmd<<14)|top->ddrAddr;
//			cmd=(cmd<<13)|top->ddrAddr;
			cmd=(cmd<<13)|(top->ddrAddr&0x1FFF);

//			data=top->ddrData;
			data=top->ddrData_O;
			SimDdr(top->ddrClk, cmd, &dqs, &data);
//			top->ddrData=data;
			top->ddrData_I=data;
			top->ddrDqsP_I=(dqs&1)?3:0;
			top->ddrDqsN_I=(dqs&2)?3:0;
		}

#if 0
//		if((cmd&SIMDDR_MSK_NOP)!=SIMDDR_MSK_NOP)
		if(1)
		{
			printf("update_ddr: clk=%d, "
				"DDR cmd=%04X clk=%d dqs=%d data=%04X->%04X\n",
				top->clock,
				cmd, top->ddrClk, dqs,
				top->ddrData_O, top->ddrData_I);
		}
#endif
#endif

#if 0	
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
		
//		if(main_time>768)
		if(main_time>1536)
			break;
#endif

//		top->eval();

	}
	
	td0=tw1-tw0;
	td1=tr1-tr0;
	
	printf("store %d cyc/op\n", (td0/2)/lim);
	printf("load %d cyc/op\n", (td1/2)/lim);
	
	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
