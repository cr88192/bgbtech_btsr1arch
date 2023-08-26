#include <stdio.h>
#include <stdlib.h>

#include "VModEdgeWalk.h"
#include "verilated.h"

typedef uint8_t byte;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#include "bt1h_targa.c"

VModEdgeWalk *top = new VModEdgeWalk;

vluint64_t main_time = 0;

uint64_t *dram_buf;
int dsaddr_l;
int dsopm_l;

struct ew_request_s {
uint64_t parm[20];
};

struct ew_request_s ew_reqarr[] = {

{	0x00000000014000A1ULL,	//00: Control
	0x0000014001400000ULL,	//01: Ystrt, Yend
	0x0004000000020000ULL,	//02: Cbuf, Zbuf
	0x0000000000010000ULL,	//03: Texture
	0x000000007FFF0000ULL,	//04: Base Left  Z/X
	0x014000007FFF0000ULL,	//05: Base Right Z/X
	0x0000000000000000ULL,	//06: Step Left  Z/X
	0x0000000000000000ULL,	//07: Step Right Z/X
	0x0000000000000000ULL,	//08: Base Left  S/T
	0x0000000000000000ULL,	//09: Base Right S/T
	0x0000000000000000ULL,	//0A: Step Left  S/T
	0x0000000000000000ULL,	//0B: Step Right S/T
	0xFF0000FFFF0000FFULL,	//0C: Base Left  RGBA
	0xFF00FF0000FFFF00ULL,	//0D: Base Right RGBA
	0x00000146FEB90146ULL,	//0E: Step Left  RGBA
	0x0000FEB90146FEB9ULL,	//0F: Step Right RGBA
	0x00C8014000000000ULL	//10: Clip X0/Y0/X1/Y1
},


//{	0x00000000000167E1ULL,	//00: Control
{	0x00000000001167E1ULL,	//00: Control
	0x0000014000960032ULL,	//01: Ystrt, Yend
	0x0004000000020000ULL,	//02: Cbuf, Zbuf
	0x00A0000000010020ULL,	//03: Texture
	0x007800001FFF0000ULL,	//04: Base Left  Z/X
	0x00C800001FFF0000ULL,	//05: Base Right Z/X
	0xFFFF000000000000ULL,	//06: Step Left  Z/X
	0x0001000000000000ULL,	//07: Step Right Z/X
	0x0000000000000000ULL,	//08: Base Left  S/T
	0x0000000000200000ULL,	//09: Base Right S/T
	0x0000500000000000ULL,	//0A: Step Left  S/T
	0x0000500000000000ULL,	//0B: Step Right S/T
	0xFF0000FFFF0000FFULL,	//0C: Base Left  RGBA
	0xFF00FF0000FFFF00ULL,	//0D: Base Right RGBA
	0x00000146FEB90146ULL,	//0E: Step Left  RGBA
	0x0000FEB90146FEB9ULL,	//0F: Step Right RGBA
	0x00C8014000000000ULL	//10: Clip X0/Y0/X1/Y1
},

{	0x00000000000167E1ULL,	//00: Control
	0x0000014000960032ULL,	//01: Ystrt, Yend
	0x0004000000020000ULL,	//02: Cbuf, Zbuf
	0x00A0000000010020ULL,	//03: Texture
	0x006000003FFF0000ULL,	//04: Base Left  Z/X
	0x00E000003FFF0000ULL,	//05: Base Right Z/X
	0xFFFF000000000000ULL,	//06: Step Left  Z/X
	0x0001000000000000ULL,	//07: Step Right Z/X
	0x0000000000000000ULL,	//08: Base Left  S/T
	0x0000000000200000ULL,	//09: Base Right S/T
	0x0000500000000000ULL,	//0A: Step Left  S/T
	0x0000500000000000ULL,	//0B: Step Right S/T
	0xFF00FF0000FFFF00ULL,	//0D: Base Right RGBA
	0xFF0000FFFF0000FFULL,	//0C: Base Left  RGBA
	0x0000FEB90146FEB9ULL,	//0F: Step Right RGBA
	0x00000146FEB90146ULL,	//0E: Step Left  RGBA
	0x00C8014000000000ULL	//10: Clip X0/Y0/X1/Y1
},

{0,0,0,0}  //End
};

u64 scale_st_rcp_z(u64 st, u16 z)
{
	u64 st1;
	s32 s0, t0, s1, t1, s2, t2;
	s32 z_rcp;
	
	if(z==0)
		return(st);
	
	s0=(s32)(st>> 0);
	t0=(s32)(st>>32);
	
	z_rcp=0x100000000LL/z;
	
	s1=(((s64)s0)*z_rcp)>>20;
	t1=(((s64)t0)*z_rcp)>>20;
	
	st1=(((u64)((u32)t1))<<32)|((u32)s1);
	
//	printf("scale_st_rcp_z")
	
	return(st1);
}

static int addr_rgbbuf=0x020000;
static int addr_zbuf=0x040000;
static int sz_fbuf=0x020000;
static int addr_tex=0x010000;

void update_bus()
{
	static int inh, okl;
	int ta;
	
	if(dsopm_l && top->dsOpm)
	{
		if(dsopm_l != top->dsOpm)
		{
			printf("update_bus: Bad Opm Shift %02X %02X\n",
				dsopm_l, top->dsOpm);
		}
		if(dsaddr_l != top->dsAddr)
		{
			printf("update_bus: Bad Addr Shift %02X %02X\n",
				dsaddr_l, top->dsAddr);
		}
	}

	if(dsopm_l && !top->dsOpm)
	{
		if(okl!=1)
		{
			printf("update_bus: Drop Early %02X %02X\n",
				dsaddr_l, top->dsAddr);
		}
	}
	
	ta=((top->dsAddr)>>3)&0x00FFFFFF;
	
	top->dsOK=0;

	if(top->dsOpm&8)
	{
		top->dsDataInLo=dram_buf[ta+0];
		top->dsDataInHi=dram_buf[ta+1];
		top->dsOK=1;

		if(inh>0)
		{
			inh--;
			top->dsOK=2;
		}
		
		if(top->dsOpm!=dsopm_l)
			printf("RD A=%08X D=%016llX\n", ta*8, dram_buf[ta]);
	}else if(top->dsOpm&16)
	{
		dram_buf[ta]=top->dsDataOutLo;
		if((top->dsOpm&7)==7)
			dram_buf[ta+1]=top->dsDataOutHi;
		top->dsOK=1;

		if(inh>0)
		{
			inh--;
			top->dsOK=2;
		}
		
//		printf("WR %08X\n", ta);
		if(top->dsOpm!=dsopm_l)
			printf("WR A=%08X D=%016llX\n", ta*8, dram_buf[ta]);
	}else
	{
		inh=8;
	}
	
	dsaddr_l=top->dsAddr;
	dsopm_l=top->dsOpm;
	okl=top->dsOK;
}

#if 0
static byte imgmap[16*16]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0,
0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,
0,0,0,1,1,1,0,0,1,1,1,1,0,0,0,0,
0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,
0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,
0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,
0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,
0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,
0,0,0,1,1,1,0,1,1,1,1,0,0,0,0,0,
0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,
0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,
0,0,0,1,1,1,0,0,0,1,1,1,1,0,0,0,
0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
#endif

#if 1
static byte imgmap[32*32]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0,
0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,
0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,1,0,0,0,0,
0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,
0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,
0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,
0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,1,0,0,0,0,0,
0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,
0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,
0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,1,0,0,0,
0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,
0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,
0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,
0,0,0,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,
0,0,0,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,
0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

};
#endif


u64 enc_getblock(byte *img, int ix, int xstr)
{
	u64 blk;
	u32 px;
	int x, y;
	int i, j, k;
	
	px=0;
	for(i=0; i<16; i++)
	{
		j=(ix<<4)+i;
		
		x=0; y=0;
		if(j&0x0001)	x|=0x01;
		if(j&0x0002)	y|=0x01;
		if(j&0x0004)	x|=0x02;
		if(j&0x0008)	y|=0x02;
		if(j&0x0010)	x|=0x04;
		if(j&0x0020)	y|=0x04;
		if(j&0x0040)	x|=0x08;
		if(j&0x0080)	y|=0x08;
		if(j&0x0100)	x|=0x10;
		if(j&0x0200)	y|=0x10;
		if(j&0x0400)	x|=0x20;
		if(j&0x0800)	y|=0x20;
		if(j&0x1000)	x|=0x40;
		if(j&0x2000)	y|=0x40;
		if(j&0x4000)	x|=0x80;
		if(j&0x8000)	y|=0x80;
		
		k=img[(y*xstr)+x];
		if(k)
			k=3;
		px|=k<<(i*2);
	}
	
	blk=0x7FFF98C6ULL|(((u64)px)<<32);
	return(blk);
}

int main(int argc, char **argv, char **env)
{
	FILE *ofd;

//	byte *ibuf, *dbuf;
	char *ifn, *ofn;
//	u64 dste, blk64;
//	u32 blk;

//	uint32_t *obuf;
//	uint32_t *imgbuf;

	uint64_t ew_status;

	int rq_opm, rq_addr;

	int bpos, bplim;
	int imgba, imgba0, irq, irqtck, irqsblk;
	int xs, ys, cxs, cys, ystr, cblks;
	int y, x;
	int cy, dy, cu, cv;
	int pcy, pdy, pcu, pcv;
	int wn, rn, wdn, rdn, lim, bn1, inh;
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

	dram_buf=(uint64_t *)malloc((1<<24)*sizeof(uint64_t)+128);

	for(i=0; i<(sz_fbuf>>3); i++)
	{
		k=addr_rgbbuf+i*8;
		dram_buf[k>>3]=0x5555AAAA5555AAAAULL;
		k=addr_zbuf+i*8;
		dram_buf[k>>3]=0xFFFFFFFFFFFFFFFFULL;
	}
	
	k=addr_tex+0*8;
	dram_buf[k>>3]=0x5555AAAA7FFF7FFFULL;

	// 0003 3000
	// 0003 3000
	// 0033 3300
	// 0033 3300
	// 003300FF
	// CC00FF00

	// 0333 3330
	// 0333 3330
	// 3333 3333
	// 3333 3333
	// 33FFFFFF
	// FFCCFFFF

	k=addr_tex+4*8;
//	dram_buf[(k>>3)+0]=0x003300FF7FFF1CE7ULL;
//	dram_buf[(k>>3)+1]=0xCC00FF007FFF1CE7ULL;
//	dram_buf[(k>>3)+2]=0x33FFFFFF7FFF1CE7ULL;
//	dram_buf[(k>>3)+3]=0xFFCCFFFF7FFF1CE7ULL;

	dram_buf[(k>>3)+0]=0x003300FF7FFF98C6ULL;
	dram_buf[(k>>3)+1]=0xCC00FF007FFF98C6ULL;
	dram_buf[(k>>3)+2]=0x33FFFFFF7FFF98C6ULL;
	dram_buf[(k>>3)+3]=0xFFCCFFFF7FFF98C6ULL;

	for(i=0; i<64; i++)
	{
		dram_buf[(k>>3)+i]=enc_getblock(imgmap, i, 32);
	}

//	imgbuf=(uint32_t *)malloc((1<<26)+128);
	
//	for(i=0; i<16777216; i++)
//	{
//		k=rand();
//		k=k*65521+rand();
//		k=k*65521+rand();
//		imgbuf[i]=k;
//	}

	wn=0;	rn=0;
	wdn=0;	rdn=0;
	rq_opm=0;
	
//	lim=4194304;
	lim=1<<18;
//	lim=1<<20;

	inh=3;
	printf("Begin\n");
	while (!Verilated::gotFinish())
	{
		top->clock = main_time&1;
		main_time++;

#if 1
		if(top->clock)
//		if(!(top->clock))
		{
			update_bus();
			top->eval();
			continue;
		}
#endif
		
		if(inh)
		{
			inh--;
			printf("Inhibit\n");
			update_bus();
			top->eval();
			continue;
		}
		
		if(rq_opm && (top->busOK==0))
		{
//			printf("Req and OK=0\n");
			update_bus();
			top->eval();
			continue;
		}
		
		if(!rq_opm && (top->busOK!=0))
		{
//			printf("No-Req and OK=1\n");
			update_bus();
			top->eval();
			continue;
		}
		
//		if(!ew_reqarr[rn].parm[0] && !(ew_status&0x5))
//			break;
		if(!wn)
		{
//			printf("WN=0\n");
			rq_opm=0xB;
			rq_addr=0xC000+(0*8);
			top->busAddr=rq_addr;
			top->busOpm=rq_opm;
			if(top->busOK==1)
			{
//				printf("WN=0, OK\n");
				rq_opm=0;
				ew_status=top->busDataOut;
				top->busOpm=rq_opm;
//				printf("	Status=%016llX\n", ew_status);
				if(!(ew_status&1))
				{
					if(!ew_reqarr[rn].parm[0] && !(ew_status&0x5))
						break;

					if(ew_reqarr[rn].parm[0])
						wn++;
				}else
				{
//					printf("WN=0, Busy\n");
				}
			}
		}else
		{
			printf("WN=%d\n", wn);
			rq_opm=0x13;
			rq_addr=0xC000+(wn*8);
			top->busAddr=rq_addr;
			top->busOpm=rq_opm;
			top->busDataIn=ew_reqarr[rn].parm[wn];
			
			if((wn>=0x08) && (wn<=0x0B) &&
				ew_reqarr[rn].parm[0]&(1<<20))
			{
				top->busDataIn=scale_st_rcp_z(
					ew_reqarr[rn].parm[wn],
					(signed short)(ew_reqarr[rn].parm[4]>>16));
			}
			
			if(wn>=18)
			{
				rq_addr=0xC000+(0*8);
				top->busAddr=rq_addr;
				top->busDataIn=ew_reqarr[rn].parm[0];
			}

			if(wn>=19)
			{
				rq_opm=0x00;
				top->busOpm=rq_opm;
			}
			
			if(top->busOK==1)
			{
				rq_opm=0;
				ew_status=top->busDataOut;
				top->busOpm=rq_opm;
				wn++;
				if(wn>=19)
				{
					if(ew_reqarr[rn].parm[0])
					{
						wn=0;
						rn++;
						inh=8;
					}else
					{
						wn=19;
					}
				}
			}
		}
		
		update_bus();
		top->eval();
	}
	
	printf("End, %lld cycles\n", main_time/2);
	
	BTIC1H_Img_SaveTGA555("sim_edgewalk0.tga",
		(u16 *)(dram_buf+(addr_rgbbuf>>3)), 320, -200);

	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
