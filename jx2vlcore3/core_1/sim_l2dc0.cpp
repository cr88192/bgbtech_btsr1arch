#include <stdio.h>
#include <stdlib.h>

#include "VMemL2Dc.h"
#include "verilated.h"

// #define CONGFXB_NOMAIN
// #include "congfxbe.c"


VMemL2Dc *top = new VMemL2Dc;

vluint64_t main_time = 0;

uint32_t *dram_buf;

void update_bus()
{
	int ta;
	
	ta=((top->ddrMemAddr)>>2)&0x00FFFFFF;
	
	top->ddrMemOK=0;

	if(top->ddrMemOpm&8)
	{
		top->ddrMemDataIn[0]=dram_buf[ta+0];
		top->ddrMemDataIn[1]=dram_buf[ta+1];
		top->ddrMemDataIn[2]=dram_buf[ta+2];
		top->ddrMemDataIn[3]=dram_buf[ta+3];
		top->ddrMemOK=1;
		
//		printf("RD A=%08X D=%08X-%08X-%08X-%08X\n", ta,
//			dram_buf[ta+0], dram_buf[ta+1],
//			dram_buf[ta+2], dram_buf[ta+3]);
	}else if(top->ddrMemOpm&16)
	{
		dram_buf[ta+0]=top->ddrMemDataOut[0];
		dram_buf[ta+1]=top->ddrMemDataOut[1];
		dram_buf[ta+2]=top->ddrMemDataOut[2];
		dram_buf[ta+3]=top->ddrMemDataOut[3];
		top->ddrMemOK=1;

//		printf("WR %08X\n", ta);
//		printf("WR A=%08X D=%08X-%08X-%08X-%08X\n", ta,
//			dram_buf[ta+0], dram_buf[ta+1],
//			dram_buf[ta+2], dram_buf[ta+3]);
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
	int bpos, bplim;
	int imgba, imgba0, irq, irqtck, irqsblk;
	int xs, ys, cxs, cys, ystr, cblks;
	int y, x;
	int cy, dy, cu, cv;
	int pcy, pdy, pcu, pcv;
	int wn, rn, wdn, rdn, lim, bn1;
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

	wn=0;	rn=0;
	wdn=0;	rdn=0;
	
//	lim=4194304;
	lim=1<<18;
//	lim=1<<20;

	printf("Begin\n");
	while (!Verilated::gotFinish())
	{
		top->clock = main_time&1;
		
		if((wn<lim) || wdn)
		{
			if(top->memOK==1)
			{
//				printf("Wr OK\n");
				if(!wdn)
				{
					printf("\r%d/%d", wn, lim);
					fflush(stdout);
					top->memOpm=0;
					wdn=1;
					wn++;
				}
			}else
				if(top->memOK==2)
			{
//				printf("Wr Hold\n");
			}else
				if(top->memOK==0)
			{
				bn1=wn^(0x5A5A5A5A&(lim-1));
			
//				printf("Wr Ready\n");
				top->memAddr=0x1000000 + (bn1*16);
				top->memDataIn[0]=imgbuf[bn1*4+0];
				top->memDataIn[1]=imgbuf[bn1*4+1];
				top->memDataIn[2]=imgbuf[bn1*4+2];
				top->memDataIn[3]=imgbuf[bn1*4+3];
				top->memOpm=0x17;
				wdn=0;

				if(wn>=lim)
				{
					printf("\n");
				}
			}
		}else
			if((rn<lim) || rdn)
		{
			bn1=rn^(0xA5A5A5A5&(lim-1));

			if(top->memOK==1)
			{
				if(!rdn)
				{
					if(	(top->memDataOut[0]!=imgbuf[bn1*4+0]) ||
						(top->memDataOut[1]!=imgbuf[bn1*4+1]) ||
						(top->memDataOut[2]!=imgbuf[bn1*4+2]) ||
						(top->memDataOut[3]!=imgbuf[bn1*4+3]) )
					{
						printf("\nExpect %08X-%08X-%08X-%08X\n",
							imgbuf[bn1*4+0], imgbuf[bn1*4+1],
							imgbuf[bn1*4+2], imgbuf[bn1*4+3]);
						printf("Got %08X-%08X-%08X-%08X\n",
							top->memDataOut[0], top->memDataOut[1],
							top->memDataOut[2], top->memDataOut[3]);
						break;
					}
				
					printf("\r%d/%d", rn, lim);
					fflush(stdout);
					top->memOpm=0;
					rdn=1;
					rn++;
				}
			}else
				if(top->memOK==2)
			{
			}else
				if(top->memOK==0)
			{
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
					printf("\n");
					break;
				}
			}
		}

		update_bus();

		top->eval();

		main_time++;

	}
	
//	ofd=fopen("sim_text0_pwm.dat", "wb");
//	fwrite(obuf, 1, bpos/8, ofd);
//	fwrite(obuf, 1, (bpos/8)*4, ofd);
//	fwrite(obuf, 1, bpos*2, ofd);
//	fclose(ofd);
	
	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
