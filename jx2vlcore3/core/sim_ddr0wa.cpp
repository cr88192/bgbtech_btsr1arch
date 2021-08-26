#include "VMmiModDdrWa.h"
#include "verilated.h"

VMmiModDdrWa *top = new VMmiModDdrWa;

vluint64_t main_time = 0;

uint16_t	*ddr_ram;

int ddr_cmdhi, ddr_cmdlo;
int ddr_cyc;

int ddr_parm_wl=3;	//CAS WL*2+1
int ddr_parm_rl=7;	//CAS RL*2+1
int ddr_row, ddr_col, ddr_bank;
int ddr_state, ddr_cas, ddr_burst;
int ddr_bcol, ddr_bstate;
int ddr_rowopen, ddr_doap;

int ddr_burstlen;
int ddr_mr0;
int ddr_mr1;
int ddr_mr2;
int ddr_mr3;


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
	int		nxtburst, nxtbcol, nxtbstate;
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

	nxtburst = ddr_burst;
	nxtbcol = ddr_bcol;
	nxtbstate = ddr_bstate;

	if(ddr_bstate==1)
	{
		if(ddr_burst>0)
		{
//			ddr_burst--;
			nxtburst = ddr_burst-1;

			pos=(ddr_row<<13)+(ddr_bank<<10)+ddr_bcol;
			pos&=(1<<28)-1;
			data=ddr_ram[pos>>1];
			nxtbcol=ddr_bcol+2;

			*rdata=data;
//			*rdqs=(ddr_burst&1)?1:2;
			*rdqs=(nxtburst&1)?1:2;

//			printf("RD %08X: %04X\n", pos, data);

		}else
		{
			*rdata=0xBAAD;
		}
	}else if(ddr_bstate==2)
	{
		if(ddr_burst>0)
		{
			if((dqs&3)!=(clk&3))
			{
				printf("DDR DQS Issue %d!=%d\n", dqs&3, clk&3);
			}
			
//			ddr_burst--;
			nxtburst = ddr_burst-1;

			data=*rdata;
			pos=(ddr_row<<13)+(ddr_bank<<10)+ddr_bcol;
			pos&=(1<<28)-1;
			ddr_ram[pos>>1]=data;
			nxtbcol=ddr_bcol+2;
			
//			printf("ST %08X = %04X\n", pos, data);
		}
	}else
	{
		*rdata=0xBAAD;
	}

	if(ddr_cas>=0)
	{
		ddr_cas--;
		if((ddr_state==1) && (ddr_burst<=0))
			*rdqs=2;

		if(ddr_cas<0)
		{
//			ddr_burst=ddr_burstlen;
			nxtburst = ddr_burstlen;
			nxtbcol = ddr_col;
			nxtbstate = ddr_state;
			
			if(ddr_doap)
				ddr_rowopen=0;
		}
	}
	
	ddr_burst = nxtburst;
	ddr_bcol = nxtbcol;
	ddr_bstate = nxtbstate;

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
				if(!ddr_rowopen)
					printf("DDR, Read Active Row: Row Not Active\n");
			
//				printf("Read Active Row, Ba=%X Col=%X\n", bank, addr);
//				ddr_col=addr;
				ddr_col=(addr&0x03FF)|((addr>>1)&0x0400);
				ddr_bank=bank;
				ddr_state=1;
//				ddr_cas=ddr_parm_rl*2+1;
//				ddr_cas=ddr_parm_rl*2;
				ddr_cas=ddr_parm_rl*2-1;
//				ddr_cas=ddr_parm_rl*2-2;
//				ddr_cas=4*2+1;
//				ddr_burst=ddr_burstlen;
				ddr_doap=(addr&0x0400)!=0;
			}else
			{
				if(!ddr_rowopen)
					printf("DDR, Write Active Row: Row Not Active\n");

//				printf("Write Active Row, Ba=%X Col=%X\n", bank, addr);
//				ddr_col=addr;
				ddr_col=(addr&0x03FF)|((addr>>1)&0x0400);
				ddr_bank=bank;
				ddr_state=2;
//				ddr_cas=ddr_parm_wl*2+1;
//				ddr_cas=ddr_parm_wl*2;
//				ddr_cas=ddr_parm_wl*2-1;
				ddr_cas=ddr_parm_wl*2-2;
//				ddr_burst=ddr_burstlen;
				ddr_doap=(addr&0x0400)!=0;
			}
		}
	}else
	{
		if(cmd&SIMDDR_MSK_CAS)
		{
			if(cmd&SIMDDR_MSK_WE)
			{
				if(ddr_rowopen)
					printf("DDR, Activate Row: Row Still Active\n");

				ddr_rowopen=1;
//				printf("Activate Row\n");
				ddr_row=addr;
			}else
			{
//				printf("Precharge Row\n");
				ddr_rowopen=0;
			}
		}else
		{
			if(cmd&SIMDDR_MSK_WE)
			{
				printf("Auto-Refresh Row\n");
				ddr_rowopen=0;
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
	int *shuf;
	uint64_t tw0, tw1, td0;
	uint64_t tr0, tr1, td1;
	int ddrlclk, cmd, data, dqs;
	int n, inh;
	int wn, rn, wdn, rdn, lim, bn1, lim1;
	int rqsq, nsq, sqdn;
	int i, j, k, l;

	Verilated::commandArgs(argc, argv);
	
	ddr_ram=(uint16_t *)malloc(256<<20);
	
//	for(i=0; i<(128<<20); i++)
//		ddr_ram[i]=rand();

//	memset(ddr_ram, 0, 256<<20);
	memset(ddr_ram, 0x55, 256<<20);


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
	rqsq=0;	nsq=0;
	
//	lim=8388608-4;

//	lim=6144*1024;

//	lim=4194304;
	lim=1<<18;
//	lim=8;
//	lim=16;
//	lim=64;
//	lim=65;
//	lim=128;

	shuf=(int *)malloc(lim*sizeof(int));

	l=lim/4;
	for(i=0; i<(l+16); i++)
		shuf[i]=i;

	for(i=0; i<l; i++)
	{
		j=(rand()*rand())&(l-1);
		k=shuf[i]; shuf[i]=shuf[j]; shuf[j]=k;
	}


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
			}
		}else
		{
			top->reset=0;
			top->reset2=0;
		}

		top->clock = (main_time>>1)&1;
		top->clock2 = (main_time>>0)&1;
	
//		top->clock = (main_time>>2)&1;
//		top->clock2 = (main_time>>1)&1;
//		top->clock2 = (main_time>>0)&1;
//		top->clock2 = ((main_time*3)/4)&1;
		main_time++;

		sqdn=0;
		if((top->memOK==1) && (top->memOpSqO==rqsq) && (rqsq!=0))
		{
			sqdn=1;
		}

		if((top->memOK==1) && (top->memOpSqO==nsq))
		{
			rqsq=nsq;
			nsq=(nsq+1)&15;
		}

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
			if((top->memOK==1) && !sqdn)
			{			
				if((top->memOpm==0x1F) &&
					(top->memDataOut[0]!=0x55555555) &&
					(top->memAddr != top->memAddrSw) &&
					!wdn)
				{
					printf("\nAddr=%X %X\n", top->memAddr, top->memAddrSw);
					printf("Swap Mismatch, Got=%X, In=%X Img=%X\n",
						top->memDataOut[0],
						top->memDataIn[ 0],
						imgbuf[bn1*4+ 0]);
					break;
				}

				top->memOpm=0;
//				printf("Wr OK\n");
				if(!wdn)
				{
					printf("\r%d/%d", wn, lim);
					fflush(stdout);
					wdn=1;
					wn+=4;
				}				
			}else
				if(top->memOK==2)
			{
//				printf("Wr Hold\n");
			}else
				if((top->memOK==0) || sqdn)
			{
//				bn1=wn^(0x5A5A5A5C&(lim-1));
//				bn1=(((wn^(wn>>8))*251)>>8)&(lim-1);
//				bn1=wn;

				bn1=shuf[(wn+4)>>2]<<2;
				top->memAddr=0x1000000 + (bn1*16);

				bn1=shuf[wn>>2]<<2;
			
//				printf("Wr Ready\n");
//				top->memAddr=0x1000000 + (bn1*16);
				top->memAddrSw=0x1000000 + (bn1*16);
				top->memDataIn[ 0]=imgbuf[bn1*4+ 0];
				top->memDataIn[ 1]=imgbuf[bn1*4+ 1];
				top->memDataIn[ 2]=imgbuf[bn1*4+ 2];
				top->memDataIn[ 3]=imgbuf[bn1*4+ 3];
				top->memDataIn[ 4]=imgbuf[bn1*4+ 4];
				top->memDataIn[ 5]=imgbuf[bn1*4+ 5];
				top->memDataIn[ 6]=imgbuf[bn1*4+ 6];
				top->memDataIn[ 7]=imgbuf[bn1*4+ 7];
				top->memDataIn[ 8]=imgbuf[bn1*4+ 8];
				top->memDataIn[ 9]=imgbuf[bn1*4+ 9];
				top->memDataIn[10]=imgbuf[bn1*4+10];
				top->memDataIn[11]=imgbuf[bn1*4+11];
				top->memDataIn[12]=imgbuf[bn1*4+12];
				top->memDataIn[13]=imgbuf[bn1*4+13];
				top->memDataIn[14]=imgbuf[bn1*4+14];
				top->memDataIn[15]=imgbuf[bn1*4+15];

				top->memOpSqI=nsq;
//				top->memOpm=0x17;
				top->memOpm=0x1F;
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
//			bn1=rn^(0xA5A5A5AC&(lim-1));
//			bn1=(((rn^(rn>>8))*251)>>8)&(lim-1);
//			bn1=rn;
			bn1=shuf[rn>>2]<<2;

			if((top->memOK==1) && !sqdn)
			{
//				printf("Rd OK\n");
				if(!rdn)
				{
					if(	(top->memDataOut[ 0]!=imgbuf[bn1*4+ 0]) ||
						(top->memDataOut[ 1]!=imgbuf[bn1*4+ 1]) ||
						(top->memDataOut[ 2]!=imgbuf[bn1*4+ 2]) ||
						(top->memDataOut[ 3]!=imgbuf[bn1*4+ 3]) ||
						(top->memDataOut[ 4]!=imgbuf[bn1*4+ 4]) ||
						(top->memDataOut[ 5]!=imgbuf[bn1*4+ 5]) ||
						(top->memDataOut[ 6]!=imgbuf[bn1*4+ 6]) ||
						(top->memDataOut[ 7]!=imgbuf[bn1*4+ 7]) ||
						(top->memDataOut[ 8]!=imgbuf[bn1*4+ 8]) ||
						(top->memDataOut[ 9]!=imgbuf[bn1*4+ 9]) ||
						(top->memDataOut[10]!=imgbuf[bn1*4+10]) ||
						(top->memDataOut[11]!=imgbuf[bn1*4+11]) ||
						(top->memDataOut[12]!=imgbuf[bn1*4+12]) ||
						(top->memDataOut[13]!=imgbuf[bn1*4+13]) ||
						(top->memDataOut[14]!=imgbuf[bn1*4+14]) ||
						(top->memDataOut[15]!=imgbuf[bn1*4+15]) )
					{
						printf("\nAddr=%X\n", top->memAddr);
						printf("Expect-A %08X-%08X-%08X-%08X\n",
							imgbuf[bn1*4+ 0], imgbuf[bn1*4+ 1],
							imgbuf[bn1*4+ 2], imgbuf[bn1*4+ 3]);
						printf("Expect-B %08X-%08X-%08X-%08X\n",
							imgbuf[bn1*4+ 4], imgbuf[bn1*4+ 5],
							imgbuf[bn1*4+ 6], imgbuf[bn1*4+ 7]);
						printf("Expect-C %08X-%08X-%08X-%08X\n",
							imgbuf[bn1*4+ 8], imgbuf[bn1*4+ 9],
							imgbuf[bn1*4+10], imgbuf[bn1*4+11]);
						printf("Expect-D %08X-%08X-%08X-%08X\n",
							imgbuf[bn1*4+12], imgbuf[bn1*4+13],
							imgbuf[bn1*4+14], imgbuf[bn1*4+15]);

						printf("Got-A %08X-%08X-%08X-%08X\n",
							top->memDataOut[ 0], top->memDataOut[ 1],
							top->memDataOut[ 2], top->memDataOut[ 3]);
						printf("Got-B %08X-%08X-%08X-%08X\n",
							top->memDataOut[ 4], top->memDataOut[ 5],
							top->memDataOut[ 6], top->memDataOut[ 7]);
						printf("Got-C %08X-%08X-%08X-%08X\n",
							top->memDataOut[ 8], top->memDataOut[ 9],
							top->memDataOut[10], top->memDataOut[11]);
						printf("Got-D %08X-%08X-%08X-%08X\n",
							top->memDataOut[12], top->memDataOut[13],
							top->memDataOut[14], top->memDataOut[15]);
						break;
					}
				
					printf("\r%d/%d", rn, lim);
					fflush(stdout);
					top->memOpm=0;
					rdn=1;
					rn+=4;
				}
			}else
				if(top->memOK==2)
			{
//				printf("Rd Hold\n");
			}else
				if((top->memOK==0) || sqdn)
			{
//				printf("Rd OK\n");

				top->memAddr=0x1000000 + (bn1*16);
//				top->memDataIn[0]=imgbuf[wn*4+0];
//				top->memDataIn[1]=imgbuf[wn*4+1];
//				top->memDataIn[2]=imgbuf[wn*4+2];
//				top->memDataIn[3]=imgbuf[wn*4+3];
				top->memOpSqI=nsq;
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
#endif

//		top->eval();

	}
	
	td0=tw1-tw0;
	td1=tr1-tr0;
	
	lim1=lim/4;
	
//	printf("store %d cyc/op\n", (td0/2)/lim);
//	printf("load %d cyc/op\n", (td1/2)/lim);

	wdn=(td0/4)/lim1;
	rdn=(td1/4)/lim1;

	printf("store %d cyc/op, %.2fMB @ 50MHz/s\n", wdn, (50.0/wdn)*64);
	printf("load %d cyc/op, %.2fMB @ 50MHz/s\n", rdn, (50.0/rdn)*64);
	
	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
