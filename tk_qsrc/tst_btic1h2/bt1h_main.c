/*
Copyright (C) 2015 by Brendan G Bohannon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "bt1h_common.h"

#include "bt1h_shim_s2tc.c"
#include "bt1h_shim_bc7.c"

#include "bt1h_thread.c"
#include "bt1h_thwork.c"

#include "bt1h_rice.c"
#include "bt1h_decrice.c"
#include "bt1h_bitrange.c"
#include "bt1h_rangedec.c"
#include "bt1h_rangeenc.c"

#include "bt1h_blkenc.c"
#include "bt1h_blkdec.c"
#include "bt1h_encmisc.c"
#include "bt1h_encode.c"
#include "bt1h_decode.c"
#include "bt1h_targa.c"

// #include "bt2d_dct.c"
// #include "bt2d_encode.c"

double curtime()
{
#ifdef _WIN32
	int t0;
	t0=clock();
	return(t0/((double)CLOCKS_PER_SEC));
#else
	static time_t base=0;
	struct timeval tv;
	time_t ds;
	
	gettimeofday(&tv, NULL);
	if(!base)
		base=tv.tv_sec;
	
	ds=tv.tv_sec-base;
	return((ds)+(tv.tv_usec*0.000001));
//	return((double)ds);
#endif
}

s64 curtime_ms()
{
	return(curtime()*1000);
}

double checkrmse(byte *ibuf1, byte *ibuf2, int xs, int ys)
{
	double e, er, eg, eb;
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int dr, dg, db;
	int n;
	int i, j, k;
	
	er=0; eg=0; eb=0;
	for(i=0; i<ys; i++)
		for(j=0; j<xs; j++)
	{
		k=(i*xs+j)*4;
		cr0=ibuf1[k+0];	cg0=ibuf1[k+1];	cb0=ibuf1[k+2];
		cr1=ibuf2[k+0];	cg1=ibuf2[k+1];	cb1=ibuf2[k+2];
		dr=cr1-cr0;		dg=cg1-cg0;		db=cb1-cb0;
		er+=dr*dr;
		eg+=dg*dg;
		eb+=db*db;
	}
	
	e=(er+eg+eb)/3;
	n=xs*ys;
	printf("RMSE: Er=%.3f Eg=%.3f Eb=%.3f Eavg=%.3f\n",
		sqrt(er/n), sqrt(eg/n), sqrt(eb/n), sqrt(e/n));

	e=sqrt(e/n);
	return(e);
}

int dump_bmp(char *name, int xs, int ys, int fcc, byte *ibuf, int isz)
{
	byte hbuf[256];
	FILE *fd;
	int sz1, sz2, sz3, dpm;
	
	fd=fopen(name, "wb");
	if(!fd)
	{
		printf("Fail Open %s\n", name);
		return(-1);
	}

	sz1=54+isz;
	sz2=54;
	sz3=40;
	dpm=2880;
	
	hbuf[0x00]='B';		hbuf[0x01]='M';
	hbuf[0x02]=sz1;		hbuf[0x03]=sz1>>8;
	hbuf[0x04]=sz1>>16;	hbuf[0x05]=sz1>>24;
	hbuf[0x06]=0;		hbuf[0x07]=0;
	hbuf[0x08]=0;		hbuf[0x09]=0;
	hbuf[0x0A]=sz2;		hbuf[0x0B]=sz2>>8;
	hbuf[0x0C]=sz2>>16;	hbuf[0x0D]=sz2>>24;

	hbuf[0x0E]=sz3;		hbuf[0x0F]=sz3>>8;
	hbuf[0x10]=sz3>>16;	hbuf[0x11]=sz3>>24;
	hbuf[0x12]=xs;		hbuf[0x13]=xs>>8;
	hbuf[0x14]=xs>>16;	hbuf[0x15]=xs>>24;
	hbuf[0x16]=ys;		hbuf[0x17]=ys>>8;
	hbuf[0x18]=ys>>16;	hbuf[0x19]=ys>>24;

	hbuf[0x1A]=1;		hbuf[0x1B]=0;
	hbuf[0x1C]=32;		hbuf[0x1D]=0;
	hbuf[0x1E]=fcc;		hbuf[0x1F]=fcc>>8;
	hbuf[0x20]=fcc>>16;	hbuf[0x21]=fcc>>24;
	hbuf[0x22]=isz;		hbuf[0x23]=isz>>8;
	hbuf[0x24]=isz>>16;	hbuf[0x25]=isz>>24;
	hbuf[0x26]=dpm;		hbuf[0x27]=dpm>>8;
	hbuf[0x28]=dpm>>16;	hbuf[0x29]=dpm>>24;
	hbuf[0x2A]=dpm;		hbuf[0x2B]=dpm>>8;
	hbuf[0x2C]=dpm>>16;	hbuf[0x2D]=dpm>>24;

	hbuf[0x2E]=0;		hbuf[0x2F]=0;
	hbuf[0x30]=0;		hbuf[0x31]=0;

	hbuf[0x32]=0;		hbuf[0x33]=0;
	hbuf[0x34]=0;		hbuf[0x35]=0;
	
	fwrite(hbuf, 1, sz2, fd);
	fwrite(ibuf, 1, isz, fd);
	
	fclose(fd);
	return(0);
}

int main(int argc, char *argv[])
{
	byte *ibuf, *yibuf, *obuf, *blks, *yibuf2;
	byte *tbuf, *tbuf1, *ct1, *ibuf2;
	BTIC1H_Context *ctx;
	FILE *tfd;
	double f, g, h, e;
	int xs, ys, xs1, ys1;
	int cr, cg, cb, cy, cu, cv, cu1, cv1;
	int t0, t1, t2, t3;
	int n, n1, nf, ncf, qf, nwe, nwd;
	int i, j, k, l;

	nwe=8; nwd=8;

	for(i=1; i<argc; i++)
	{
		if(!strncmp(argv[i], "--th=", 5))
		{
			nwe=atoi(argv[i]+5);
			nwd=nwe;
			btic1h_workqueue_defaultworkers=nwe;
//			btic1h_workqueue_defaultworkers=atoi(argv[i]+5);
			continue;
		}

		if(!strncmp(argv[i], "--thd=", 6))
		{
			nwd=atoi(argv[i]+6);
			continue;
		}

		if(!strncmp(argv[i], "--the=", 6))
		{
			nwe=atoi(argv[i]+6);
			continue;
		}
	}

//	BTIC1H_InitCamera();

	ibuf=BTIC1H_Img_LoadTGA("StreetHollandSD.tga", &xs, &ys);
	ibuf2=BTIC1H_Img_LoadTGA("StreetHollandSD_q45.tga", &xs1, &ys1);
//	ibuf=BTIC1H_Img_LoadTGA("3917969_f260.tga", &xs, &ys);

	ibuf=BTIC1H_Img_LoadTGA("screencap0.tga", &xs, &ys);
	ibuf2=BTIC1H_Img_LoadTGA("screencap0.tga", &xs1, &ys1);

//	ibuf=BTIC1H_Img_LoadTGA("MLP_FIM1.tga", &xs, &ys);
//	ibuf2=BTIC1H_Img_LoadTGA("MLP_FIM1_q95.tga", &xs1, &ys1);

	ibuf=BTIC1H_Img_LoadTGA("MLP_FIM3.tga", &xs, &ys);
//	ibuf2=BTIC1H_Img_LoadTGA("MLP_FIM3.tga", &xs1, &ys1);
	ibuf2=BTIC1H_Img_LoadTGA("MLP_FIM3_q75.tga", &xs1, &ys1);

	yibuf=malloc(xs*ys*2);

	if(!ibuf || !ibuf2)
	{
		printf("failed load test images\n");
		return(-1);
	}

	obuf=malloc(xs*ys*4);
//	blks=malloc(xs*ys*2);

	tbuf=malloc(1<<24);
	tbuf1=malloc(1<<24);


#if 0
	for(i=0; i<ys; i++)
		for(j=0; j<xs; j++)
	{
		cr=ibuf[(i*xs+j)*4+0];
		cg=ibuf[(i*xs+j)*4+1];
		cb=ibuf[(i*xs+j)*4+2];
		
		cy=(( 19595*cr + 38470*cg + 7471*cb + 32768)>>16)+0;
		cu=((-11056*cr - 21712*cg +32768*cb + 32768)>>16)+128;
		cv=(( 32768*cr - 27440*cg - 5328*cb + 32768)>>16)+128;
		
//		cu=128;		cv=128;
		
		cy=clamp255(cy);
		cu=clamp255(cu);
		cv=clamp255(cv);
		
//		k=j&(~1);
		yibuf[(i*xs+j)*2+0]=cy;
//		yibuf[(i*xs+k)*2+1]=cu;
//		yibuf[(i*xs+k)*2+3]=cv;
//		yibuf[(i*xs+j)*2+1]=(j&1)?cv:cu;
//		yibuf[(i*xs+j)*2+1]=(yibuf[(i*xs+j)*2+1]+((j&1)?cv:cu))>>1;
//		yibuf[(i*xs+j)*2+3]=(j&1)?cu:cv;

		if(j&1)
		{
			k=j&(~1);
			yibuf[(i*xs+k)*2+1]=(yibuf[(i*xs+k)*2+1]+cu)>>1;
			yibuf[(i*xs+k)*2+3]=(yibuf[(i*xs+k)*2+3]+cv)>>1;
		}else
		{
			yibuf[(i*xs+j)*2+1]=cu;
			yibuf[(i*xs+j)*2+3]=cv;
		}
	}

//	tbuf=BTIC1H_GetCameraFBuf();
//	if(tbuf)
//	{
//		usleep(1000000);
//		memcpy(yibuf, tbuf, xs*ys*2);
//	}

	obuf=malloc(xs*ys*4);
//	blks=malloc(xs*ys*2);

	tbuf=malloc(1<<24);
	tbuf1=malloc(1<<24);

	for(i=0; i<ys; i++)
		for(j=0; j<xs; j++)
	{
		k=j&(~1);
		cy=yibuf[(i*xs+j)*2+0];
		cu=yibuf[(i*xs+k)*2+1];
		cv=yibuf[(i*xs+k)*2+3];

		cu1=cu-128; cv1=cv-128;
		cr=65536*cy           +91881*cv1;
		cg=65536*cy- 22554*cu1-46802*cv1;
		cb=65536*cy+116130*cu1;
//		cr>>=16; cg>>=16; cb>>=16;

		cr=(cr+32768)>>16;
		cg=(cg+32768)>>16;
		cb=(cb+32768)>>16;

		cr=clamp255(cr);
		cg=clamp255(cg);
		cb=clamp255(cb);

		obuf[(i*xs+j)*4+0]=cr;
		obuf[(i*xs+j)*4+1]=cg;
		obuf[(i*xs+j)*4+2]=cb;
	}

	printf("QCLRS: ");
	checkrmse(ibuf, obuf, xs, ys);

	for(i=0; i<ys; i++)
		for(j=0; j<xs; j++)
	{
#if 1
		cr=ibuf[(i*xs+j)*4+0];
		cg=ibuf[(i*xs+j)*4+1];
		cb=ibuf[(i*xs+j)*4+2];

//		cy=(2*cg+cb+cr)>>2;
//		cu=((cb-cy)>>1)+128;
//		cv=((cr-cy)>>1)+128;

		cy=((cg<<2)+3*cr+cb+4)>>3;
//		cy=((cg<<2)+2*cr+2*cb+4)>>3;
		cu=((2*cb-cr-cg+2)>>2)+128;
		cv=((2*cr-cb-cg+2)>>2)+128;
//		cu=((4*cb-3*cr-1*cg+4)>>3)+128;
//		cv=((4*cr-2*cb-2*cg+4)>>3)+128;

		cy=clamp255(cy);
		cu=clamp255(cu);
		cv=clamp255(cv);
#endif

//		k=j&(~1);
//		cy=yibuf[(i*xs+j)*2+0];
//		cu=yibuf[(i*xs+k)*2+1];
//		cv=yibuf[(i*xs+k)*2+3];

		cu1=cu-128; cv1=cv-128;
		cr=65536*cy           +91881*cv1;
		cg=65536*cy- 22554*cu1-46802*cv1;
		cb=65536*cy+116130*cu1;
		cr>>=16; cg>>=16; cb>>=16;

//		cr=(cr+32768)>>16;
//		cg=(cg+32768)>>16;
//		cb=(cb+32768)>>16;

//		cu1=cu-128; cv1=cv-128;
//		cb=cy+cu1*2;
//		cr=cy+cv1*2;
//		cg=(4*cy-cb-cr)>>1;

		cr=clamp255(cr);
		cg=clamp255(cg);
		cb=clamp255(cb);

		obuf[(i*xs+j)*4+0]=cr;
		obuf[(i*xs+j)*4+1]=cg;
		obuf[(i*xs+j)*4+2]=cb;
	}

	printf("QCLRS2: ");
	checkrmse(ibuf, obuf, xs, ys);

#endif

//	memset(obuf, 0, xs*ys*4);

//	qf=65;
//	qf=85;
//	qf=95;
//	qf=90;
	qf=75;

	qf=qf|BTIC1H_QFL_IFRAME;
//	qf=qf|BTIC1H_QFL_USERC;
	qf=qf|BTIC1H_QFL_USESLICE;
//	qf=qf|BTIC1H_QFL_USEGDBDR;

//	qf=qf|BTIC1H_QFL_USEMIP;
	
	xs1=xs>>2; ys1=ys>>2; n=xs1*ys1;
	blks=malloc(n*32);

	btic1h_workqueue_defaultworkers=nwe;

	ctx=BTIC1H_AllocContext();
	ctx->xs=xs;
	ctx->ys=ys;
	ctx->flip=0;

//	t0=clock();
	t0=curtime_ms();
	t1=t0; nf=0; ncf=0;
//	while((t1>=t0) && (t1<(t0+(1*CLOCKS_PER_SEC))))
	while((t1>=t0) && (t1<(t0+1000)))
	{
		yibuf2=yibuf;

//		ct1=BTIC2D_EncodeBlocksBufferYUY2(tbuf, 1<<20, yibuf2, xs, ys, 85);
		
//		BTIC1H_EncodeImageYUY2(blks, 32, yibuf2, xs, ys, qf);
//		ct1=BTIC1H_EncodeBlocksBuffer(tbuf, blks, NULL, n,
//			32, 1<<20, &n1, qf);

#if 1
		i=BTIC1H_EncodeCtx(ctx, ibuf, tbuf, 1<<24,
			xs, ys, qf, BTIC1H_PXF_RGBX);
#endif

//		i=BTIC1H_EncodeCtx(ctx, yibuf2, tbuf, 1<<20,
//			xs, ys, qf, BTIC1H_PXF_YUYV);
		ct1=tbuf+i;

		nf++;
//		t1=clock();
		t1=curtime_ms();
		
//		if(!(nf&15))
		if(1)
		{
			t2=t1-t0;
//			f=(t2)/((double)CLOCKS_PER_SEC);
			f=(t2)/1000.0;
			printf("fc=%d dt=%f fps=%f Mpix=%f, ncf=%d cfps=%f\r",
				nf, f, nf/f, (nf/f)*(xs*ys*(1.0/1000000)),
				ncf, (ncf/f)*(xs*ys*(1.0/1000000)));
		}
	}
	printf("\n");
	
//	BTIC1H_DumpGeneralEncodeStats();
	BTIC1H_DumpEncodeStats(ctx);
	
	h=((ct1-tbuf)*8.0)/(xs*ys);
	printf("Output Size %d / %d (%.3f bpp)\n", ct1-tbuf, n*7,
		h);
	
	BTIC1H_DestroyContext(ctx);
	
	tfd=fopen("tst1g_raw0.dat", "wb");
	if(tfd)
	{
		fwrite(tbuf, 1, ct1-tbuf, tfd);
		fclose(tfd);
	}

	dump_bmp("tst1g_raw0.bmp", xs, ys, BTIC1H_FCC_bt1h,
		tbuf, ct1-tbuf);

//	dump_bmp("tst1g_raw0.bmp", xs, ys, 0, tbuf, xs*ys*4);

	memcpy(tbuf1, tbuf, 1<<20);

	btic1h_workqueue_defaultworkers=nwd;

	ctx=BTIC1H_AllocContext();
	ctx->xs=xs;
	ctx->ys=ys;
	ctx->flip=0;

	if(ctx->blks)
		memset(ctx->blks, 0, n*32);

#if 1
//	t0=clock();
	t0=curtime_ms();
	t1=t0; nf=0; ncf=0;
//	while((t1>=t0) && (t1<(t0+(15*CLOCKS_PER_SEC))))
	while((t1>=t0) && (t1<(t0+15000)))
	{
		
//		BTIC1H_EncodeImageYUY2(blks, yibuf2, xs, ys, 85);
//		ct1=BTIC1H_EncodeBlocksBuffer(tbuf, blks, NULL, n, 16, 1<<20, &n1);

//		BTIC1H_DecodeBlocksBuffer(tbuf, blks, NULL, n, 32,
//			xs1, ys1, ct1-tbuf);
//		BTIC1H_DecodeImageMB2B(blks, 32, obuf, xs, ys, 4);

//		BTIC1H_DecodeCtx(ctx, tbuf, obuf, ct1-tbuf,
//			xs*ys*4, &xs1, &ys1, BTIC1H_PXF_RGBX);

//		BTIC1H_DecodeCtx(ctx, tbuf, obuf, ct1-tbuf,
//			xs*ys*4, &xs1, &ys1, BTIC1H_PXF_BC7);

		BTIC1H_DecodeCtx(ctx, tbuf, obuf, ct1-tbuf,
			xs*ys*4, &xs1, &ys1, BTIC1H_PXF_BC1);

		nf++;
//		t1=clock();
		t1=curtime_ms();
		
//		if(!(nf&15))
		if(1)
		{
			t2=t1-t0;
//			f=(t2)/((double)CLOCKS_PER_SEC);
			f=(t2)/1000.0;
			printf("fc=%d dt=%f fps=%f Mpix=%f, ncf=%d cfps=%f\r",
				nf, f, nf/f, (nf/f)*(xs*ys*(1.0/1000000)),
				ncf, (ncf/f)*(xs*ys*(1.0/1000000)));
		}
	}
	printf("\n");
#endif

	if(memcmp(tbuf1, tbuf, 1<<20))
	{
		printf("Buffer Contents Changed\n");
	}

#if 1
	memset(ctx->blks, 0, n*32);

//	memset(blks, 0, n*32);
	memset(obuf, 0, xs*ys*4);

//	BTIC1H_DecodeBlocksBuffer(tbuf, blks, NULL, n, 32, xs1, ys1, ct1-tbuf);
//	BTIC1H_DecodeImageMB2B(blks, 32, obuf, xs, ys, 4);

	BTIC1H_DecodeCtx(ctx, tbuf, obuf, ct1-tbuf,
		xs*ys*4, &xs1, &ys1, BTIC1H_PXF_RGBA);

//	BTIC1H_DecodeCtx(ctx, tbuf, tbuf1, ct1-tbuf,
//		xs*ys*4, &xs1, &ys1, BTIC1H_PXF_BC7);
//	BTIC1H_S2TC_DecodeImage(tbuf1, 16, obuf, xs, ys, 4, 0);
//	BTIC1H_BC7_DecodeImage(tbuf1, obuf, xs, ys, 4, 0);
	
	BTIC1H_Img_SaveTGA("tst1g_out0.tga", obuf, xs, ys);

	printf("Q_JPG: ");
	checkrmse(ibuf, ibuf2, xs, ys);
	
	printf("QBT1H: ");
	e=checkrmse(ibuf, obuf, xs, ys);
	printf("Ebit: %f\n", h*e);
#endif

	return(0);
}
