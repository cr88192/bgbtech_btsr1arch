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

//	uint32_t *obuf;
	uint16_t *obuf;
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

	obuf=(uint16_t *)malloc(1<<28);
	memset(obuf, 0, 1<<28);
//	bpos=8;
	bpos=2;
//	bplim=(1<<22)*8;
//	bplim=(1<<23)*2;
//	bplim=(1<<23)/2;
	bplim=(1<<24)/2;
//	bplim=(1<<26)/2;
//	obuf[0]=0x55AAC100;

	imgbuf=(uint32_t *)malloc(16384*4);
	imgba=0;

	ibuf=BTIC1H_Img_LoadTGA(ifn, &xs, &ys);

	if(!ibuf)
	{
		printf("Failed Load %s\n", ifn);
	}

	dbuf=(byte *)malloc(xs*ys*4);

	for(i=0; i<8192; i++)
	{
		j=rand()*rand()*rand();
		imgbuf[i]=j;
	}
	cblks=40*25*8;

	for(i=0; i<(40*25); i++)
	{
		imgbuf[i*8+0]|=0xC0000000UL;
		imgbuf[i*8+1]|=0xC0000000UL;
	}

	dste=0;

#if 0
	if(xs==320)
	{
		printf("Is 320\n");

		cxs=xs/4;
		cys=ys/4;
		ystr=xs*4;
		cblks=cxs*cys*8;

//		for(y=0; y<60; y++)
		for(y=0; y<25; y++)
	//		for(x=0; x<80; x++)
			for(x=0; x<40; x++)
		{
			k=((((cys-y-1)*4)*xs)+(x*4))*4;
			dste=0;
			if(0)
			{
				vqenc64(ibuf+k, ystr, &blk64, &dste);
				j=y*160+(x*2);
				imgbuf[j+0]=blk64>>32;
				imgbuf[j+1]=blk64;
			}
			else
			{
				vqenc(ibuf+k, ystr, &blk, &dste);
				j=(y*40+x)*8;
				imgbuf[j]=blk;
			}
		}

//		imgbuf[8192+2047]=1;
//		imgbuf[0x9F00/4]=1;
	}

	if(xs==640)
	{
		printf("Is 640\n");

		cxs=xs/4;
		cys=ys/8;
		ystr=xs*8;
		cblks=cxs*cys;

		for(y=0; y<60; y++)
	//		for(x=0; x<80; x++)
			for(x=0; x<160; x++)
		{
			k=((((cys-y-1)*8)*xs)+(x*4))*4;
			dste=0;
			vqenc(ibuf+k, ystr, &blk, &dste);
//			vqenc(ibuf+k, ystr, &blk, NULL);

			j=y*160+x;
			imgbuf[j]=blk;
		}
//		imgbuf[8192+2047]=0;
		imgbuf[0x9F00/4]=0;
	}
#endif

	imgba=0;
	irq=1;
	imgba0=imgba;
//	irqtck=100000*16;
	irqtck=1600;
//	irqsblk=1024;
//	irqsblk=768;
	irqsblk=8;
//	irqtck=0;
//	top->busOE=0;
	top->busOpm=0;

	while (!Verilated::gotFinish())
	{
//		top->clock = (main_time>>3)&1;
//		top->clock = (main_time>>1)&1;
		top->clock = (main_time>>0)&1;
//		top->mode = 3;
		
//		top->baseAddr=0xDECAB00;
//		top->idxAddr=0x100;
//		top->idxDisp=3;

		irqtck--;
		if(irqtck<=0)
//		if(0)
		{
			irq=1;
//			irqtck=100000*16;
//			irqtck=16000;
//			irqsblk=64;

			irqtck=160;
			irqsblk=8;

//			irqsblk=1020;
//			irqsblk=288;
//			irqsblk=320;
//			irqsblk=360;
//			irqsblk=420;
//			irqsblk=480;

			if(imgba0)
//			if(0)
			{
				imgba=imgba0;
				printf("IRQ BA=%d\n", imgba0);
			}
		}

		if(irq)
		{
			blk=imgbuf[imgba&16383];
			if(blk)
			{
//				printf("Blk %08X\n", blk);
//				top->busAddr=0xACA00000LL+((imgba*4)&4095);
//				top->busAddr=0x000A0000LL+((imgba*4)&4095);
//				top->busAddr=0x000A0000LL+((imgba*8)&4095);
//				top->busAddr=0x000A0000LL+((imgba*8)&8191);
				top->busAddr=0x000A0000LL+(imgba&8191)*4;
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
			top->busOpm=0x00;

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

//		if(!(main_time&15))
		if(top->clock)
		{
//			if(top->pwmOut)
//				obuf[bpos>>5]|=(1<<(bpos&31));

//			j=top->pwmOut&15;
//			obuf[bpos>>3]|=(j<<((bpos&7)*4));

			j=top->pwmOut&0xFFFF;
//			obuf[bpos>>1]|=(j<<((bpos&1)*16));
			obuf[bpos]=j;
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
//				imgba=(imgba+1);
//				if(imgba>=cblks)
//					imgba-=cblks;
				irqsblk--;
				if(irqsblk<=0)
				{
//					printf("Block Advance %X\n", imgba);
					imgba=(imgba+1);
					if(imgba>=cblks)
						imgba-=cblks;

					irq=0;
				}
#endif
			}
		}
		
		if(bpos>=bplim)
		{
			break;
		}
	}
	
	ofd=fopen("sim_fbtst0_pwm.dat", "wb");
//	fwrite(obuf, 1, bpos/8, ofd);
//	fwrite(obuf, 1, (bpos/8)*4, ofd);
	fwrite(obuf, 1, bpos*2, ofd);
	fclose(ofd);
	
	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
