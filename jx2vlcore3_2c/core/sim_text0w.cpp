#include <stdio.h>
#include <stdlib.h>

#include "VModTxtNtW.h"
#include "verilated.h"

#define CONGFXB_NOMAIN
#include "congfxbe.c"


VModTxtNtW *top = new VModTxtNtW;

vluint64_t main_time = 0;

int main(int argc, char **argv, char **env)
{
	FILE *ofd;

	byte *ibuf, *dbuf;
	char *ifn, *ofn;
	u64 dste, blk64;
	u32 blk;

	uint32_t *obuf;
	uint32_t *imgbuf;
	int bpos, bplim;
	int imgba, imgba0, irq, irqtck, irqsblk;
	int xs, ys, cxs, cys, ystr, cblks;
	int y, x;
	int cy, dy, cu, cv;
	int pcy, pdy, pcu, pcv;
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

	obuf=(uint32_t *)malloc(1<<23);
	memset(obuf, 0, 1<<23);
	bpos=8;
//	bplim=(1<<22)*8;
//	bplim=(1<<23)*2;
	bplim=(1<<23)/2;
	obuf[0]=0x55AA4100;

	imgbuf=(uint32_t *)malloc(16384*4);
	imgba=0;

//	ibuf=BTIC1H_Img_LoadTGA(ifn, &xs, &ys);

//	if(!ibuf)
//	{
//		printf("Failed Load %s\n", ifn);
//	}

	dbuf=(byte *)malloc(xs*ys*4);

	dste=0;

	imgba=0;
	irq=1;
	imgba0=imgba;
	irqtck=100000*16;
//	irqsblk=1024;
	irqsblk=768;
//	irqtck=0;
//	top->busOE=0;
	top->busOpm=0;

	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>3)&1;
//		top->mode = 3;
		
//		top->baseAddr=0xDECAB00;
//		top->idxAddr=0x100;
//		top->idxDisp=3;

		if(irq)
		{
			blk=imgbuf[imgba&16383];
			if(blk)
			{
//				printf("Blk %08X\n", blk);
				top->busAddr=0x0000ACA00000LL+((imgba*4)&4095);
				top->busInData=blk;
//				top->busWR=1;
//				top->busOE=0;
				top->busOpm=0x12;
			}else
			{
//				top->busWR=0;
//				top->busOE=0;
				top->busOpm=0x00;
			}
		}else
		{
#if 0
			top->busAddr=0x0000ACA1FF20LL;
			top->busWR=0;
			top->busOE=1;
			if(!top->busHold && top->clock)
				imgba0=top->busData;
#endif
		}

		top->eval();

		main_time++;

		if(!(main_time&15))
		{
//			if(top->pwmOut)
//				obuf[bpos>>5]|=(1<<(bpos&31));

//			j=top->pwmOut&15;
//			obuf[bpos>>3]|=(j<<((bpos&7)*4));

			j=top->pwmOut&65535;
			obuf[bpos*2+0]=j;
			obuf[bpos*2+1]=j>>8;

			bpos++;
			
			if(irq)
//			if(1)
			{
//				imgba=(imgba+1)&8191;
//				if(imgba<(8192+2048))

//				if(imgba<(4096+1024))
//					imgba=(imgba+1)&16383;
//				if(imgba>=(8192+2048))
//					imgba=0;
			
#if 1
				imgba=(imgba+1);
				if(imgba>=cblks)
					imgba-=cblks;
				irqsblk--;
				if(irqsblk<=0)
					irq=0;
#endif
			}
		}
		
		if(bpos>=bplim)
		{
			break;
		}
	}
	
	ofd=fopen("sim_text0_pwm.dat", "wb");
//	fwrite(obuf, 1, bpos/8, ofd);
//	fwrite(obuf, 1, (bpos/8)*4, ofd);
	fwrite(obuf, 1, bpos*2, ofd);
	fclose(ofd);
	
	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
