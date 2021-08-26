#include <stdio.h>
#include <stdlib.h>

#include "VRbiMemL2DcWa.h"
#include "verilated.h"

// #define CONGFXB_NOMAIN
// #include "congfxbe.c"


VRbiMemL2DcWa *top = new VRbiMemL2DcWa;

vluint64_t main_time = 0;

vluint64_t main_time_st0s;
vluint64_t main_time_st0e;
vluint64_t main_time_ld0s;
vluint64_t main_time_ld0e;

vluint64_t main_time_st1s;
vluint64_t main_time_st1e;

vluint64_t main_time_ld1s;
vluint64_t main_time_ld1e;

vluint64_t main_accnt_st0s;
vluint64_t main_accnt_st0e;
vluint64_t main_accnt_ld0s;
vluint64_t main_accnt_ld0e;
vluint64_t main_accnt_st1s;
vluint64_t main_accnt_st1e;
vluint64_t main_accnt_ld1s;
vluint64_t main_accnt_ld1e;


vluint64_t main_time_end0;

uint32_t *dram_buf;

static int dram_lseq;
static int dram_count;


void update_bus()
{
	int ta, tast, sq;
	
	if(top->ddrMemAddr&63)
		printf("update_bus: address misaligned %X\n", top->ddrMemAddr);
	
	ta=((top->ddrMemAddr)>>2)&0x00FFFFFF;
	tast=ta;
	sq=top->ddrOpSqO;

	if((top->ddrMemOpm&0x18)==0x18)
	{
		tast=((top->ddrMemAddrSw)>>2)&0x00FFFFFF;
	}
	
	top->ddrMemOK=0;

	if(top->ddrMemOpm&8)
	{
		top->ddrMemDataIn[ 0]=dram_buf[ta+ 0];
		top->ddrMemDataIn[ 1]=dram_buf[ta+ 1];
		top->ddrMemDataIn[ 2]=dram_buf[ta+ 2];
		top->ddrMemDataIn[ 3]=dram_buf[ta+ 3];
		top->ddrMemDataIn[ 4]=dram_buf[ta+ 4];
		top->ddrMemDataIn[ 5]=dram_buf[ta+ 5];
		top->ddrMemDataIn[ 6]=dram_buf[ta+ 6];
		top->ddrMemDataIn[ 7]=dram_buf[ta+ 7];
		top->ddrMemDataIn[ 8]=dram_buf[ta+ 8];
		top->ddrMemDataIn[ 9]=dram_buf[ta+ 9];
		top->ddrMemDataIn[10]=dram_buf[ta+10];
		top->ddrMemDataIn[11]=dram_buf[ta+11];
		top->ddrMemDataIn[12]=dram_buf[ta+12];
		top->ddrMemDataIn[13]=dram_buf[ta+13];
		top->ddrMemDataIn[14]=dram_buf[ta+14];
		top->ddrMemDataIn[15]=dram_buf[ta+15];

		top->ddrMemOK=1;
		top->ddrOpSqI=sq;		
	}
	
	if(top->ddrMemOpm&16)
	{
		dram_buf[tast+ 0]=top->ddrMemDataOut[ 0];
		dram_buf[tast+ 1]=top->ddrMemDataOut[ 1];
		dram_buf[tast+ 2]=top->ddrMemDataOut[ 2];
		dram_buf[tast+ 3]=top->ddrMemDataOut[ 3];
		dram_buf[tast+ 4]=top->ddrMemDataOut[ 4];
		dram_buf[tast+ 5]=top->ddrMemDataOut[ 5];
		dram_buf[tast+ 6]=top->ddrMemDataOut[ 6];
		dram_buf[tast+ 7]=top->ddrMemDataOut[ 7];
		dram_buf[tast+ 8]=top->ddrMemDataOut[ 8];
		dram_buf[tast+ 9]=top->ddrMemDataOut[ 9];
		dram_buf[tast+10]=top->ddrMemDataOut[10];
		dram_buf[tast+11]=top->ddrMemDataOut[11];
		dram_buf[tast+12]=top->ddrMemDataOut[12];
		dram_buf[tast+13]=top->ddrMemDataOut[13];
		dram_buf[tast+14]=top->ddrMemDataOut[14];
		dram_buf[tast+15]=top->ddrMemDataOut[15];

		top->ddrMemOK=1;
		top->ddrOpSqI=sq;

#if 0
//		printf("WR %08X\n", ta);
		printf("WR A=%08X D=%08X-%08X-%08X-%08X\n", tast,
			dram_buf[tast+0], dram_buf[tast+1],
			dram_buf[tast+2], dram_buf[tast+3]);
#endif
	}
	
	if(top->ddrMemOpm&0x18)
	{
		if(sq!=dram_lseq)
			dram_count++;
		dram_lseq=sq;
	}else
	{
		dram_lseq=-1;
	}
}

uint32_t ring_req_opm;
uint32_t ring_req_seq;
uint64_t ring_req_addr;
uint32_t ring_req_data_a;
uint32_t ring_req_data_b;
uint32_t ring_req_data_c;
uint32_t ring_req_data_d;
uint32_t ring_req_done;

uint32_t ring_resp_opm;
uint32_t ring_resp_seq;
uint64_t ring_resp_addr;
uint32_t ring_resp_data_a;
uint32_t ring_resp_data_b;
uint32_t ring_resp_data_c;
uint32_t ring_resp_data_d;
uint32_t ring_resp_done;

void update_ring()
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
	
	static int			lclock;

	if(!(top->clock))
	{	
#if 1
		top->memSeqIn=l2seq2;
		top->memOpmIn=l2opm2;
		top->memAddrIn=l2addr2;
		top->memDataIn[0]=l2data2a;
		top->memDataIn[1]=l2data2b;
		top->memDataIn[2]=l2data2c;
		top->memDataIn[3]=l2data2d;
#endif

#if 0
		top->memSeqIn=l2seq4;
		top->memOpmIn=l2opm4;
		top->memAddrIn=l2addr4;
		top->memDataIn[0]=l2data4a;
		top->memDataIn[1]=l2data4b;
		top->memDataIn[2]=l2data4c;
		top->memDataIn[3]=l2data4d;
#endif
	}

	if(top->clock==lclock)
		return;
	lclock=top->clock;

	if(top->clock)
	{	
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

		l2seq1=top->memSeqOut;
		l2opm1=top->memOpmOut;
		l2addr1=top->memAddrOut;
		l2data1a=top->memDataOut[0];
		l2data1b=top->memDataOut[1];
		l2data1c=top->memDataOut[2];
		l2data1d=top->memDataOut[3];

#if 0
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

	if(((l2opm1&255)==0x00) && !ring_req_done && ring_req_opm)
	{
		l2opm1=ring_req_opm;
		l2seq1=ring_req_seq;
		l2addr1=ring_req_addr;
		l2data1a=ring_req_data_a;
		l2data1b=ring_req_data_b;
		l2data1c=ring_req_data_c;
		l2data1d=ring_req_data_d;
		ring_req_done=1;
	}

	if((l2opm1&255)==0x60)
	{
//		printf("OK Store\n");

		ring_resp_opm=l2opm1;
		ring_resp_seq=l2seq1;
		ring_resp_addr=l2addr1;
		ring_resp_data_a=l2data1a;
		ring_resp_data_b=l2data1b;
		ring_resp_data_c=l2data1c;
		ring_resp_data_d=l2data1d;
		ring_resp_done=1;

		l2opm1=0;	l2seq1=0;
		l2addr1=0;
		l2data1a=0;	l2data1b=0;
		l2data1c=0;	l2data1d=0;
	}

	if((l2opm1&255)==0x70)
	{
//		printf("OK Load\n");

		ring_resp_opm=l2opm1;
		ring_resp_seq=l2seq1;
		ring_resp_addr=l2addr1;
		ring_resp_data_a=l2data1a;
		ring_resp_data_b=l2data1b;
		ring_resp_data_c=l2data1c;
		ring_resp_data_d=l2data1d;
		ring_resp_done=1;

		l2opm1=0;	l2seq1=0;
		l2addr1=0;
		l2data1a=0;	l2data1b=0;
		l2data1c=0;	l2data1d=0;
	}
}

int main(int argc, char **argv, char **env)
{
	FILE *ofd;

//	byte *ibuf, *dbuf;
	char *ifn, *ofn;
//	u64 dste, blk64;
//	u32 blk;

	uint32_t *obuf;
	uint32_t *imgbuf;
	uint64_t  tt0, tt1, dt0, dt1;
	int bpos, bplim;
	int imgba, imgba0, irq, irqtck, irqsblk;
	int xs, ys, cxs, cys, ystr, cblks;
	int y, x;
	int cy, dy, cu, cv;
	int pcy, pdy, pcu, pcv;
	int wn, rn, wn2, rn2, wdn, rdn, lim, bn1, inh;
	int i, j, k;
	
	Verilated::commandArgs(argc, argv);

	ifn=NULL;
	ofn=NULL;

	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			continue;
		}
		if(!ifn)
			{ ifn=argv[i]; continue; }
		if(!ofn)
			{ ofn=argv[i]; continue; }
	}

	dram_buf=(uint32_t *)malloc((1<<26)+128);

	imgbuf=(uint32_t *)malloc((1<<26)+128);
	
	for(i=0; i<16777216; i++)
	{
		k=rand();
		k=k*65521+rand();
		k=k*65521+rand();
		imgbuf[i]=k;
	}
	
	dram_count=0;

//	obuf=(uint32_t *)malloc(1<<23);
//	memset(obuf, 0, 1<<23);
//	bpos=8;
//	bplim=(1<<22)*8;
//	bplim=(1<<23)*2;
//	bplim=(1<<23)/2;
//	obuf[0]=0x55AA4100;

//	imgbuf=(uint32_t *)malloc(16384*4);
//	imgba=0;

//	ibuf=BTIC1H_Img_LoadTGA(ifn, &xs, &ys);

//	if(!ibuf)
//	{
//		printf("Failed Load %s\n", ifn);
//	}

//	dbuf=(byte *)malloc(xs*ys*4);

	top->unitNodeId=0x84;

	wn=0;	rn=0;
	wn2=0;	rn2=0;
	wdn=0;	rdn=0;

//	lim=1<<8;

//	lim=1<<12;
//	lim=1<<13;

//	lim=4194304;
	lim=1<<18;
//	lim=1<<20;

	main_time_st0s = main_time;
	main_accnt_st0s = dram_count;
	inh=3;
	printf("Begin\n");
	while (!Verilated::gotFinish())
	{
		top->clock = main_time&1;
		main_time++;

		if(top->clock)
		{
//			printf("Cycle\n");
		}

		update_bus();
		update_ring();
		top->eval();

#if 1
		if(inh>0)
		{
			inh--;
			update_bus();
			top->eval();
			continue;
		}
		
		if((wn<lim) || wdn)
		{
			if(ring_resp_done)
			{
//				printf("Wr OK\n");
				if(!wdn)
				{
					printf("\r%d/%d", wn, lim);
					fflush(stdout);
					ring_req_done=0;
					ring_resp_done=0;
					ring_req_opm=0x0000;
					wdn=1;
					wn++;
					inh=3;
				}
			}else
			{
//				bn1=wn;
//				bn1=wn^15;
				bn1=wn^(0x5A5A5A5A&(lim-1));

//				if(!ring_req_done)
//					printf("Wr Ready\n");

				ring_req_addr=0x1000000 + (bn1*16);
				ring_req_data_a=imgbuf[bn1*4+0];
				ring_req_data_b=imgbuf[bn1*4+1];
				ring_req_data_c=imgbuf[bn1*4+2];
				ring_req_data_d=imgbuf[bn1*4+3];
				ring_req_seq=0x0040;
				ring_req_opm=0x00A7;
				wdn=0;
				inh=3;

				if(wn>=lim)
				{
					ring_req_opm=0x00;
					printf("\n\n");
					main_time_st0e = main_time;
					main_time_ld0s = main_time;
					main_accnt_st0e = dram_count;
					main_accnt_ld0s = dram_count;
				}
			}
		}else
			if((rn<lim) || rdn)
		{
//			bn1=rn;
//			bn1=rn^15;
			bn1=rn^(0xA5A5A5A5&(lim-1));

			if(ring_resp_done)
			{
				if(!rdn && !wdn)
				{
					if(	(ring_resp_data_a!=imgbuf[bn1*4+0]) ||
						(ring_resp_data_b!=imgbuf[bn1*4+1]) ||
						(ring_resp_data_c!=imgbuf[bn1*4+2]) ||
						(ring_resp_data_d!=imgbuf[bn1*4+3]) )
					{
						printf("\nAddr=%X\n", bn1);
						printf("Expect %08X-%08X-%08X-%08X\n",
							imgbuf[bn1*4+0], imgbuf[bn1*4+1],
							imgbuf[bn1*4+2], imgbuf[bn1*4+3]);
						printf("Got %08X-%08X-%08X-%08X\n",
							ring_resp_data_a, ring_resp_data_b,
							ring_resp_data_c, ring_resp_data_d);
						break;
					}
				
					printf("\r%d/%d", rn, lim);
					fflush(stdout);

					ring_req_done=0;
					ring_resp_done=0;
					ring_req_opm=0x0000;

					rdn=1;
					rn++;
					inh=3;
				}
			}else
			{
				ring_req_addr=0x1000000 + (bn1*16);
				ring_req_seq=0x0040;
				ring_req_opm=0x0097;

				rdn=0;
				wdn=0;
//				inh=3;

				if(rn>=lim)
				{
//					main_time_end0 = main_time;
					main_time_ld0e = main_time;
					main_time_ld1s = main_time;

					main_accnt_ld0e = dram_count;
					main_accnt_ld1s = dram_count;

					printf("\n\n");
//					break;
				}
			}
		}
		else if(rn2<lim)
		{
			if(ring_resp_done)
			{
				if(1)
				{				
					printf("\r%d/%d", rn2, lim);
					fflush(stdout);
					ring_req_done=0;
					ring_resp_done=0;
					ring_req_opm=0x0000;
					rn2++;
				}
			}else
			{
				ring_req_addr=0x1000000;
				ring_req_seq=0x0040;
				ring_req_opm=0x0097;
			}

			if(rn2>=lim)
			{
				main_time_ld1e = main_time;
				main_accnt_ld1e = dram_count;
				printf("\n\n");
				break;
			}
		}
#endif

		update_bus();
		top->eval();
	}
	
	dt0 = main_time_st0e - main_time_st0s;
	dt1 = main_time_ld0e - main_time_ld0s;
	printf("%f cycles / store (miss 0)\n", (dt0*0.5)/lim);
	printf("%f cycles / load (miss 0)\n", (dt1*0.5)/lim);

	dt1 = main_time_ld1e - main_time_ld1s;
	printf("%f cycles / load (hit 0)\n", (dt1*0.5)/lim);
	

	dt0 = main_accnt_st0e - main_accnt_st0s;
	dt1 = main_accnt_ld0e - main_accnt_ld0s;
	printf("%f access / store (miss 0)\n", (dt0*1.0)/lim);
	printf("%f access / load (miss 0)\n", (dt1*1.0)/lim);

	dt1 = main_accnt_ld1e - main_accnt_ld1s;
	printf("%f access / load (hit 0)\n", (dt1*1.0)/lim);
	
//	ofd=fopen("sim_text0_pwm.dat", "wb");
//	fwrite(obuf, 1, bpos/8, ofd);
//	fwrite(obuf, 1, (bpos/8)*4, ofd);
//	fwrite(obuf, 1, bpos*2, ofd);
//	fclose(ofd);
	
	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
