#include <stdio.h>
#include <stdlib.h>

#include "VModAudPcm.h"
#include "verilated.h"

#define CONGFXB_NOMAIN
#include "congfxbe.c"


VModAudPcm *top = new VModAudPcm;

vluint64_t main_time = 0;

int main(int argc, char **argv, char **env)
{
	FILE *ofd;

	byte *ibuf, *dbuf;
	char *ifn, *ofn;
	u64 dste, blk64;
	u32 blk;

	uint8_t *obuf;
	uint8_t *imgbuf;
	int bpos, bplim;
	int imgba, imgba0, irq, irqtck, irqsblk;
	int lstok, isinit;
	int pcmacc, pcmdiv, pcmrst;
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

	obuf=(uint8_t *)malloc(1<<20);
	memset(obuf, 0, 1<<20);
	bpos=0;
//	bplim=(1<<20)/2;
	bplim=(1<<16);
//	obuf[0]=0x55AA4100;

	imgbuf=(uint8_t *)malloc(1<<20);
	imgba=0;

	for(i=0; i<8192; i++)
	{
		imgbuf[i]=(i&8)?255:0;
	}

	dbuf=(byte *)malloc(xs*ys*4);

	dste=0;

	imgba=0;
	irq=1;
	imgba0=imgba;
//	irqtck=100000*16;
//	irqsblk=1024;
//	irqsblk=768;
//	irqtck=0;
//	top->busOE=0;
	top->busOpm=0;
	pcmrst=2268;
	pcmdiv=0;
	pcmacc=0;
	isinit=0;

	while (!Verilated::gotFinish())
	{
		top->clock = main_time&1;

		if(imgba<8192)
		{
			if(top->busOK==0)
			{
//				printf("MMIO Ready\n");
			
				top->busOpm=0x12;
				top->busAddr=0xF0090000+imgba;
				top->busInData=*(uint32_t *)(imgbuf+imgba);

				if(!isinit)
				{
					top->busOpm=0x12;
					top->busAddr=0xF009F000;
					top->busInData=0x0029;
				}
			}
			
			if(top->busOK==1)
			{
//				printf("MMIO OK\n");

				top->busOpm=0;
				top->busAddr=0;
				top->busInData=0;
				isinit=1;
				
				if(top->busOK!=lstok)
				{
//					printf("%d/%d\n", imgba, 4096);
					imgba++;
				}
			}

			if(top->busOK==2)
			{
//				printf("MMIO Hold\n");
			}
		}

		lstok=top->busOK;

		top->eval();

		main_time++;

#if 1
		if(top->clock)
		{
			if(pcmdiv<=0)
			{
				printf("%d/%d\r", bpos, bplim);
				fflush(stdout);
				
				obuf[bpos++]=(255*pcmacc)/pcmrst;
				pcmdiv=pcmrst;
				pcmacc=0;
			}else
			{
				pcmacc+=top->pwmOut&1;
				pcmdiv--;
			}
		}
#endif
		
		if(bpos>=bplim)
		{
			break;
		}
	}
	
	ofd=fopen("sim_aud_pcm.dat", "wb");
//	fwrite(obuf, 1, bpos/8, ofd);
//	fwrite(obuf, 1, (bpos/8)*4, ofd);
	fwrite(obuf, 1, bpos, ofd);
	fclose(ofd);
	
	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
