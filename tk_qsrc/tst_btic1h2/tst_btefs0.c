#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "bt1h_common.h"
#include "bt1h_rice.c"
#include "bt1h_decrice.c"
#include "bt1h_bitrange.c"
#include "bt1h_rangedec.c"
#include "bt1h_rangeenc.c"

#include "bt1h_btepfs.c"

int test_encode(byte *ibuf, byte *obuf, int fsz, int tn)
{
	BTIC1H_Context tctx;
	BTIC1H_Context *ctx;

	double f, g, ft0;
	long long tisz, tosz, te;
	int t0, t1, t2, t3;
	int k0, k1;
	int i, j, k;

	ctx=&tctx;
	memset(ctx, 0, sizeof(BTIC1H_Context));

//	BTIC1H_Rice_SetupWriteRangeBits(ctx, obuf, fsz, 12);

	t0=clock(); t1=t0; tisz=0; tosz=0;
	while(t1<(t0+(30*CLOCKS_PER_SEC)))
	{
		BTIC1H_Rice_SetupWriteRangeBits(ctx, obuf, fsz, 12);
//		BTIC1H_Rice_SetupWriteBTEFS(ctx, obuf, fsz);

		for(i=0; i<fsz; i++)
		{
			ctx->Write8Bits(ctx, ibuf[i]);
		}

		j=ctx->bs_ct-obuf;
		tisz+=fsz; tosz+=j;
		
		t1=clock();
		t2=t1-t0;
		ft0=t2/((double)CLOCKS_PER_SEC);
		printf("  %.2fs %.2fMB/s %.2f%%\r",
			ft0, tisz/(1024.0*1024.0*ft0),
			(100.0*tosz)/(tisz+1.0));
	}
	printf("\n");
	
	BTIC1H_BTEFS_DumpStats(ctx);
}

int main(int argc, char *argv[])
{
	FILE *ifd;
	byte *ibuf;
	byte *tbuf1, *tbuf2;
	double f, g, ft0;
	long long tsz, te;
	int t0, t1, t2, t3;
	int fsz;
	int i, j, k;
	
//	ifd=fopen("StreetHolland.tga", "rb");
	ifd=fopen("bt1hvfw.dll", "rb");
//	ifd=fopen("bgbgc_log.txt", "rb");
	if(!ifd)
	{
		return(-1);
	}
	
	fseek(ifd, 0, 2);
	fsz=ftell(ifd);
	fseek(ifd, 0, 0);
	
	ibuf=malloc(fsz+64);
	fread(ibuf, 1, fsz, ifd);
	
	tbuf1=malloc(2*fsz+64);

	for(i=0; i<1; i++)
	{
		test_encode(ibuf, tbuf1, fsz, i);
	}
}
