#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#if 0
typedef unsigned char byte;
typedef signed char sbyte;

typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

typedef unsigned long long u64;
typedef signed long long s64;

#ifdef _MSC_VER
#define force_inline __forceinline
#define default_inline __inline
#endif

#ifdef __GNUC__
#define force_inline inline
#define default_inline inline
#endif

#ifndef force_inline
#define force_inline
#define default_inline
#endif

#endif

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
#include "bt1h_encode.c"
#include "bt1h_decode.c"
#include "bt1h_targa.c"


void *bgbrpi_talloc(char *str, int sz)
{
	void *ptr;
	
	ptr=malloc(sz);
	memset(ptr, 0, sz);
	return(ptr);
}

void *bgbrpi_malloc(int sz)
{
	void *ptr;
	
	ptr=malloc(sz);
	memset(ptr, 0, sz);
	return(ptr);
}

#include "bgbrpi_dummyavi.h"
#include "w32_dummyavi.c"

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
	
//	e=(er+eg+eb)/3;
//	e=(er+2*eg+eb)/4;
	e=0.35*er+0.5*eg+0*15*eb;
	n=xs*ys;
//	printf("RMSE: Er=%.3f Eg=%.3f Eb=%.3f Eavg=%.3f\n",
//		sqrt(er/n), sqrt(eg/n), sqrt(eb/n), sqrt(e/n));

	e=sqrt(e/n);
	return(e);
}

BTIC1H_PTune *main_ptune[64];

void btic1h_ptune_average(
	BTIC1H_PTune *pta,
	BTIC1H_PTune *ptb,
	BTIC1H_PTune *ptc)
{
	ptc->dflat  =(pta->dflat  +ptb->dflat  )*0.5;
	ptc->d2x2   =(pta->d2x2   +ptb->d2x2   )*0.5;
	ptc->d4x4x1 =(pta->d4x4x1 +ptb->d4x4x1 )*0.5;
	ptc->d4x4x2 =(pta->d4x4x2 +ptb->d4x4x2 )*0.5;
	ptc->d4x4x3 =(pta->d4x4x3 +ptb->d4x4x3 )*0.5;
	ptc->dchflat=(pta->dchflat+ptb->dchflat)*0.5;

	ptc->dflate  =(pta->dflate  +ptb->dflate  )*0.5;
	ptc->d2x2e   =(pta->d2x2e   +ptb->d2x2e   )*0.5;
	ptc->d4x4x1e =(pta->d4x4x1e +ptb->d4x4x1e )*0.5;
	ptc->d4x4x2e =(pta->d4x4x2e +ptb->d4x4x2e )*0.5;
	ptc->d4x4x3e =(pta->d4x4x3e +ptb->d4x4x3e )*0.5;
	ptc->dchflate=(pta->dchflate+ptb->dchflate)*0.5;

	ptc->qyp     =(pta->qyp     +ptb->qyp     )*0.5;
	ptc->quvp    =(pta->quvp    +ptb->quvp    )*0.5;
	ptc->qdyp    =(pta->qdyp    +ptb->qdyp    )*0.5;
	ptc->qyi     =(pta->qyi     +ptb->qyi     )*0.5;
	ptc->quvi    =(pta->quvi    +ptb->quvi    )*0.5;
	ptc->qdyi    =(pta->qdyi    +ptb->qdyi    )*0.5;
}

float btic1h_ptune_rlerp(float a, float b)
{
	float f, g, h, c;
	
	f=(rand()&4095)/4095.0;
	g=(rand()&4095)/4095.0;
	h=(rand()&4095)/4095.0;
	g=2.0*g-1.0;
	c=(a*(1.0-f))+(b*f);
//	c=c*(1.0+g*0.1)+g*0.025;
//	c=c*(1.0+g*0.1)+h*0.1;
	c=c*(1.0+g*0.05)+h*0.025;
	return(c);
}

void btic1h_ptune_breed(
	BTIC1H_PTune *pta,
	BTIC1H_PTune *ptb,
	BTIC1H_PTune *ptc)
{
	ptc->dflat  =btic1h_ptune_rlerp(pta->dflat  ,ptb->dflat  );
	ptc->d2x2   =btic1h_ptune_rlerp(pta->d2x2   ,ptb->d2x2   );
	ptc->d4x4x1 =btic1h_ptune_rlerp(pta->d4x4x1 ,ptb->d4x4x1 );
	ptc->d4x4x2 =btic1h_ptune_rlerp(pta->d4x4x2 ,ptb->d4x4x2 );
	ptc->d4x4x3 =btic1h_ptune_rlerp(pta->d4x4x3 ,ptb->d4x4x3 );
	ptc->dchflat=btic1h_ptune_rlerp(pta->dchflat,ptb->dchflat);

	ptc->dflate  =btic1h_ptune_rlerp(pta->dflate  ,ptb->dflate  );
	ptc->d2x2e   =btic1h_ptune_rlerp(pta->d2x2e   ,ptb->d2x2e   );
	ptc->d4x4x1e =btic1h_ptune_rlerp(pta->d4x4x1e ,ptb->d4x4x1e );
	ptc->d4x4x2e =btic1h_ptune_rlerp(pta->d4x4x2e ,ptb->d4x4x2e );
	ptc->d4x4x3e =btic1h_ptune_rlerp(pta->d4x4x3e ,ptb->d4x4x3e );
	ptc->dchflate=btic1h_ptune_rlerp(pta->dchflate,ptb->dchflate);

	ptc->qyp     =btic1h_ptune_rlerp(pta->qyp     ,ptb->qyp     );
	ptc->quvp    =btic1h_ptune_rlerp(pta->quvp    ,ptb->quvp    );
	ptc->qdyp    =btic1h_ptune_rlerp(pta->qdyp    ,ptb->qdyp    );
	ptc->qyi     =btic1h_ptune_rlerp(pta->qyi     ,ptb->qyi     );
	ptc->quvi    =btic1h_ptune_rlerp(pta->quvi    ,ptb->quvi    );
	ptc->qdyi    =btic1h_ptune_rlerp(pta->qdyi    ,ptb->qdyi    );

	ptc->dyem    =btic1h_ptune_rlerp(pta->dyem    ,ptb->dyem    );
	ptc->duvem   =btic1h_ptune_rlerp(pta->duvem   ,ptb->duvem   );
	ptc->dyen    =btic1h_ptune_rlerp(pta->dyen    ,ptb->dyen    );
	ptc->duven   =btic1h_ptune_rlerp(pta->duven   ,ptb->duven   );
	ptc->dyeme   =btic1h_ptune_rlerp(pta->dyeme   ,ptb->dyeme   );
	ptc->duveme  =btic1h_ptune_rlerp(pta->duveme  ,ptb->duveme  );
	ptc->dyene   =btic1h_ptune_rlerp(pta->dyene   ,ptb->dyene   );
	ptc->duvene  =btic1h_ptune_rlerp(pta->duvene  ,ptb->duvene  );
}

void btic1h_ptune_dumpfd(
	BTIC1H_PTune *pt, FILE *fd)
{
//	ptune->parmfl=BTIC1H_PTFL_BLKENC|
//				BTIC1H_PTFL_BLKSKIP|
//				BTIC1H_PTFL_LQUANT;
				
	if((pt->parmfl)&BTIC1H_PTFL_BLKENC)
	{
		fprintf(fd, "fxx=%2.2f+%2.2f ", pt->dflat   , pt->dflate  );
		fprintf(fd, "22x=%2.2f+%2.2f ", pt->d2x2    , pt->d2x2e   );
		fprintf(fd, "441=%2.2f+%2.2f ", pt->d4x4x1  , pt->d4x4x1e );
		fprintf(fd, "\n");
		fprintf(fd, "442=%2.2f+%2.2f ", pt->d4x4x2  , pt->d4x4x2e );
		fprintf(fd, "443=%2.2f+%2.2f ", pt->d4x4x3  , pt->d4x4x3e );
		fprintf(fd, "chf=%2.2f+%2.2f", pt->dchflat  , pt->dchflate);
		fprintf(fd, "\n");
	}

	if((pt->parmfl)&BTIC1H_PTFL_BLKSKIP)
	{
		fprintf(fd, "yem=%2.2f+%2.2f ", pt->dyem    , pt->dyeme   );
		fprintf(fd, "uvm=%2.2f+%2.2f ", pt->duvem   , pt->duveme  );
		fprintf(fd, "yen=%2.2f+%2.2f ", pt->dyen    , pt->dyene   );
		fprintf(fd, "uvn=%2.2f+%2.2f ", pt->duven   , pt->duvene  );
		fprintf(fd, "\n");
	}

	if((pt->parmfl)&BTIC1H_PTFL_LQUANTI)
	{
		fprintf(fd, "qyxi=%2.2f ", pt->qyi );
		fprintf(fd, "quvi=%2.2f ", pt->quvi);
		fprintf(fd, "qdyi=%2.2f ", pt->qdyi);
		fprintf(fd, "\n");
	}

	if((pt->parmfl)&BTIC1H_PTFL_LQUANTP)
	{
		fprintf(fd, "qyxp=%2.2f ", pt->qyp );
		fprintf(fd, "quvp=%2.2f ", pt->quvp);
		fprintf(fd, "qdyp=%2.2f ", pt->qdyp);
		fprintf(fd, "\n");
	}

	fprintf(fd, "eb=%2.2f bpp=%2.2f", pt->ebit, pt->ebpp);
	fprintf(fd, "\n");
}

void btic1h_ptune_dump(
	BTIC1H_PTune *pt)
{
	btic1h_ptune_dumpfd(pt, stdout);
}

// #define HALFSAMPLE

int main(int argc, char *argv[])
{
	BGBBTJ_AVI_Context *ctx;
	BTIC1H_Context *btectx;
	BTIC1H_Context *btdctx;
	BTIC1H_PTune *ptune;
	FILE *logfd;

	byte *ibuf, *ibuf2, *obuf, *tbuf;
	char *ifn;
	double dt, fsz;
	double f, g, h, e, ebit;
	double qff, tgte;
	int tgsz;
	int tsz, xs1, ys1, xsi, ysi, ptrov, ptron, ptgen;
	int fc, xs, ys, qf, qfl, qfn;
	int t0, t1, t2;
	int l0, l1;
	int i, j, k, l;

	ifn=NULL;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			continue;
		}
		ifn=argv[i];
	}
	
	for(i=0; i<64; i++)
	{
		ptune=malloc(sizeof(BTIC1H_PTune));
		memset(ptune, 0, sizeof(BTIC1H_PTune));
		main_ptune[i]=ptune;
	
		ptune=main_ptune[i];
		ptune->dflat  =(rand()&4095)/2048.0;
		ptune->d2x2   =(rand()&4095)/2048.0;
		ptune->d4x4x1 =(rand()&4095)/2048.0;
		ptune->d4x4x2 =(rand()&4095)/2048.0;
		ptune->d4x4x3 =(rand()&4095)/2048.0;
		ptune->dchflat=(rand()&4095)/2048.0;

		ptune->dflate  =(rand()&4095)/2048.0;
		ptune->d2x2e   =(rand()&4095)/2048.0;
		ptune->d4x4x1e =(rand()&4095)/2048.0;
		ptune->d4x4x2e =(rand()&4095)/2048.0;
		ptune->d4x4x3e =(rand()&4095)/2048.0;
		ptune->dchflate=(rand()&4095)/2048.0;

		ptune->dyem  =(rand()&4095)/2048.0;
		ptune->duvem =(rand()&4095)/2048.0;
		ptune->dyen  =(rand()&4095)/2048.0;
		ptune->duven =(rand()&4095)/2048.0;
		ptune->dyeme =(rand()&4095)/2048.0;
		ptune->duveme=(rand()&4095)/2048.0;
		ptune->dyene =(rand()&4095)/2048.0;
		ptune->duvene=(rand()&4095)/2048.0;

		ptune->qyp =(rand()&4095)/2048.0;
		ptune->quvp=(rand()&4095)/2048.0;
		ptune->qdyp=(rand()&4095)/2048.0;
		ptune->qyi =(rand()&4095)/2048.0;
		ptune->quvi=(rand()&4095)/2048.0;
		ptune->qdyi=(rand()&4095)/2048.0;
		
//		ptune->parmfl=BTIC1H_PTFL_BLKENC|
//			BTIC1H_PTFL_BLKSKIP|
//			BTIC1H_PTFL_LQUANT;
	}

#if 1
//	for(i=0; i<64; i++)
	for(i=0; i<16; i++)
	{
//		ptune=main_ptune[16];
		ptune=main_ptune[i];

		ptune->dflat  =0.33;
		ptune->d2x2   =0.70;
		ptune->d4x4x1 =0.75;
		ptune->d4x4x2 =2.00;
		ptune->d4x4x3 =8.00;
		ptune->dchflat=0.35;

		ptune->dflate  =0.5;
		ptune->d2x2e   =0.7;
		ptune->d4x4x1e =1.0;
		ptune->d4x4x2e =2.0;
		ptune->d4x4x3e =2.0;
		ptune->dchflate=0.7;

		ptune->dyem  =0.30;
		ptune->duvem =0.50;
		ptune->dyen  =0.60;
		ptune->duven =0.80;
		ptune->dyeme =0.25;
		ptune->duveme=0.25;
		ptune->dyene =0.50;
		ptune->duvene=0.50;

		ptune->qyp =1.0/5;
		ptune->quvp=1.0/4;
		ptune->qdyp=1.0/3;
		ptune->qyi =1.0/6.5;
		ptune->quvi=1.0/6;
		ptune->qdyi=1.0/5;
	}
#endif

#if 1
	for(i=0; i<32; i++)
	{
		j=rand()&31;
		k=rand()&31;
		btic1h_ptune_breed(
			main_ptune[j],
			main_ptune[k],
			main_ptune[32+i]);
	}
#endif
	logfd=fopen("ptune_log0.txt", "wt");

	if(!ifn)
	{
		printf("No Input\n");
		return(-1);
	}

	ctx=BGBBTJ_AVI_LoadAVI(ifn);
	if(!ctx)
	{
		printf("Failed load AVI %s\n", ifn);
		return(-1);
	}
	
#ifdef HALFSAMPLE
	xsi=ctx->bmihead->biWidth;
	ysi=ctx->bmihead->biHeight;
	xs=ctx->bmihead->biWidth>>1;
	ys=ctx->bmihead->biHeight>>1;
	fsz=xs*ys/1000000.0;
#else
	xs=ctx->bmihead->biWidth;
	ys=ctx->bmihead->biHeight;
	fsz=xs*ys/1000000.0;
	printf("%d x %d, fsz=%f Mpix\n", xs, ys, fsz);
#endif

	btectx=BTIC1H_AllocContext();
	btdctx=BTIC1H_AllocContext();
	btectx->xs=xs;
	btectx->ys=ys;
	btdctx->xs=xs;
	btdctx->ys=ys;

#ifdef HALFSAMPLE
	ibuf=malloc(xs*ys*4);
#endif
	obuf=malloc(xs*ys*4);
	tbuf=malloc(1<<24);

	qf=75; ptrov=0; ptron=0; ptgen=0;
	qff=75;
	qfn=99;
	
	qfl=BTIC1H_QFL_DBGPTUNE;
	qfl|=BTIC1H_QFL_PFRAME;

	if(qfl&BTIC1H_QFL_PFRAME)
	{
		tgsz=1500000/30;
		for(i=0; i<64; i++)
		{
			ptune=main_ptune[i];
			ptune->parmfl=BTIC1H_PTFL_BLKENC|
				BTIC1H_PTFL_BLKSKIP|
				BTIC1H_PTFL_LQUANTP;
		}
	}else
	{
		tgsz=1500000/5;

		for(i=0; i<64; i++)
		{
			ptune=main_ptune[i];
//			ptune->parmfl=BTIC1H_PTFL_BLKENC|
//				BTIC1H_PTFL_BLKSKIP|
//				BTIC1H_PTFL_LQUANT;
			ptune->parmfl=BTIC1H_PTFL_LQUANTI;
		}
	}

	fc=0; t0=clock(); t1=t0;
//	while(t1<(t0+(30*CLOCKS_PER_SEC)))
	while(1)
	{
		if(qfl&BTIC1H_QFL_PFRAME)
		{
//			if(!(fc&31))
			if(!(fc&127))
			{
				ibuf=BGBBTJ_AVI_DecodeFrameBGRA(ctx);
				fc++;

				k=qfl&(~(BTIC1H_QFL_PFRAME|BTIC1H_QFL_DBGPTUNE));
				k=k|BTIC1H_QFL_IFRAME;
				tsz=BTIC1H_EncodeCtx(btectx, ibuf, tbuf, 1<<24,
					xs, ys, qf|k, BTIC1H_PXF_BGRX);

				BTIC1H_DecodeCtx(btdctx, tbuf, obuf, tsz,
					xs*ys*4, &xs1, &ys1, BTIC1H_PXF_BGRX);
				continue;
			}
		}
	
#ifdef HALFSAMPLE
		ibuf2=BGBBTJ_AVI_DecodeFrameBGRA(ctx);
		for(i=0; i<ys; i++)
			for(j=0; j<ys; j++)
		{
			k=(i*xs+j)*4;
			l0=((i*2+0)*xsi+j*2)*4;
			l1=((i*2+1)*xsi+j*2)*4;
//			ibuf[k+0]=ibuf2[l0+0];
//			ibuf[k+1]=ibuf2[l0+1];
//			ibuf[k+2]=ibuf2[l0+2];
//			ibuf[k+3]=ibuf2[l0+3];

			ibuf[k+0]=(ibuf2[l0+0]+ibuf2[l0+4]+
				ibuf2[l1+0]+ibuf2[l1+4])>>2;
			ibuf[k+1]=(ibuf2[l0+1]+ibuf2[l0+5]+
				ibuf2[l1+1]+ibuf2[l1+5])>>2;
			ibuf[k+2]=(ibuf2[l0+2]+ibuf2[l0+6]+
				ibuf2[l1+2]+ibuf2[l1+6])>>2;
			ibuf[k+3]=(ibuf2[l0+3]+ibuf2[l0+7]+
				ibuf2[l1+3]+ibuf2[l1+7])>>2;
		}
#else
		ibuf=BGBBTJ_AVI_DecodeFrameBGRA(ctx);
#endif

		fc++;

		ptune=main_ptune[ptrov];
		ptrov=(ptrov+1)&63;
		if(!ptrov)ptron++;
		btic1h_dbg_ptune=ptune;

		tsz=BTIC1H_EncodeCtx(btectx, ibuf, tbuf, 1<<24,
			xs, ys, qf|qfl, BTIC1H_PXF_BGRX);

		BTIC1H_DecodeCtx(btdctx, tbuf, obuf, tsz,
			xs*ys*4, &xs1, &ys1, BTIC1H_PXF_BGRX);

		f=((float)(tgsz-tsz))/tgsz;
		g=qff+f*10;
		qff=(qff*0.99)+(g*0.01);
		
		qf=qff;
		
		if(qf<10)
			{ qf=10; qff=qf; }
//		if(qf>100)qf=100;
		if(qf>qfn)
			{ qf=qfn; qff=qf; }

//		tgte=25.0/(101-qf);
//		tgte=25.0/((qf+1)/4);
//		tgte=32.0*((101-qf)/100.0);
//		tgte=16.0*((101-qf)/100.0);
		tgte=16.0*((100-qf)/100.0);
		tgte=tgte*tgte;

		e=checkrmse(ibuf, obuf, xs, ys);
		h=(tsz*8.0)/(xs*ys);
		f=e/tgte;
//		f=f*f;
		
		ebit=h*e*(f+1);
//		ebit=h*e;
//		ebit=h*e*e;
		ptune->ebit+=ebit;
		ptune->ebpp+=h;
		ptune->ermse+=e;

		t1=clock();
		t2=t1-t0;
		dt=t2/((double)CLOCKS_PER_SEC);
		if(dt==0)dt=0.01;
		
		printf(
			"                                      "
			"                                    \r");
		printf("%.1fs %.1f fps, %3.1f Mpix/sec ",
			dt, fc/dt, fsz*(fc/dt));
		printf("sz=%d eb=%.1f e=%2.1f qf=%d rtj=%d/%d/%d ",
			tsz, ebit, e, qf, ptrov, ptron, ptgen);
		printf("\r");

		if(ptron>=8)
		{
			k=ptron;
			ptron=0;
			ptgen++;

#if 1
			for(i=0; i<64; i++)
			{
				ptune=main_ptune[i];
				ptune->ebit/=k;
				ptune->ebpp/=k;
				ptune->ermse/=k;
			}
#endif

			for(i=0; i<64; i++)
				for(j=i+1; j<64; j++)
			{
				if(main_ptune[j]->ebit < main_ptune[i]->ebit)
				{
					ptune=main_ptune[i];
					main_ptune[i]=main_ptune[j];
					main_ptune[j]=ptune;
				}
			}

#if 0
			for(i=0; i<32; i++)
			{
				btic1h_ptune_average(
					main_ptune[i+0],
					main_ptune[i+1],
					main_ptune[i+1]);
			}
#endif

			for(i=0; i<32; i++)
			{
				j=rand()&31;
				k=rand()&31;
				btic1h_ptune_breed(
					main_ptune[j],
					main_ptune[k],
					main_ptune[32+i]);
			}
			
			btic1h_ptune_dump(main_ptune[0]);

			fprintf(logfd, "Gen %d\n", ptgen);
			for(i=0; i<8; i++)
			{
				fprintf(logfd, "%d:\n", i);
				btic1h_ptune_dumpfd(main_ptune[i], logfd);
				fprintf(logfd, "\n");
			}

			for(i=0; i<64; i++)
			{
				ptune=main_ptune[i];
				ptune->ebit=0;
				ptune->ebpp=0;
				ptune->ermse=0;
			}
		}
	}
	printf("\n");
}
