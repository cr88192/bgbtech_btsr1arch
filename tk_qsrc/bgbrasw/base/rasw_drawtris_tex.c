#include <bgbrasw.h>

#ifdef BGBRASW_SS_YUVA
#define tgt_rgba	tgt_ssyuva
#define tgt_zbuf	tgt_sszbuf
#endif

// #define ST_SHL	8
#define ST_SHL	0
#define ST_SHR	0
#define ST_MIPSHR	8

// #define ST_SHL	12
// #define ST_SHR	4

// #define XY_SHL	8
#define XY_SHR	8
// #define XY_SHR2	0

#define XY_SHL	0
#define XY_SHR2	8

void BGBRASW_DrawPrimitive_TriangleTexBasic(
	BGBRASW_Context *ctx,
	BGBRASW_Primitive *prim, int flag)
{
	bgbrasw_pixel clr;
	bgbrasw_pixel *tergba;
	int xs, ys, yc, ys1, ye0, ye1, ye2;
	int xc0, xc1, xc0v, xc1v;
	int x0, x1, x2, y0, y1, y2;
	int v0, v1, v2;
	int stsa, stsb, stsav, stsbv;
	int stta, sttb, sttav, sttbv;
	int st0s, st0t, st1s, st1t;
	int st2s, st2t, st3s, st3t;
	int sts0, stt0, sts1, stt1;
	int txs, tys, mip;
//	int s0, s1, s2;
	int t0, t1, t2;
	int xa, xb;
	int i, j, k;

//	if(flag&1)return;
//	return;

	if(flag&1)	{ v0=0; v1=2; v2=3; }
	else		{ v0=0; v1=1; v2=2; }

	y0=prim->xy[v0][1];
	y1=prim->xy[v1][1];
	y2=prim->xy[v2][1];
	if(y0<y1)
	{
		//012, 021, 201
		if(y0<y2)
		{
			//Bot=Y0
			if(y1<y2)
			{	x0=prim->xy[v0][0];		y0=prim->xy[v0][1];
				x1=prim->xy[v1][0];		y1=prim->xy[v1][1];
				x2=prim->xy[v2][0];		y2=prim->xy[v2][1];
				st0s=prim->st[v0][0];	st0t=prim->st[v0][1];
				st1s=prim->st[v1][0];	st1t=prim->st[v1][1];
				st2s=prim->st[v2][0];	st2t=prim->st[v2][1];
			}
			else
			{	x0=prim->xy[v0][0];		y0=prim->xy[v0][1];
				x1=prim->xy[v2][0];		y1=prim->xy[v2][1];
				x2=prim->xy[v1][0];		y2=prim->xy[v1][1];
				st0s=prim->st[v0][0];	st0t=prim->st[v0][1];
				st1s=prim->st[v2][0];	st1t=prim->st[v2][1];
				st2s=prim->st[v1][0];	st2t=prim->st[v1][1];
			}
		}else
			{	x0=prim->xy[v2][0];		y0=prim->xy[v2][1];
				x1=prim->xy[v0][0];		y1=prim->xy[v0][1];
				x2=prim->xy[v1][0];		y2=prim->xy[v1][1];
				st0s=prim->st[v2][0];	st0t=prim->st[v2][1];
				st1s=prim->st[v0][0];	st1t=prim->st[v0][1];
				st2s=prim->st[v1][0];	st2t=prim->st[v1][1];
			}
	}else
	{
		//102, 120, 210
		if(y1<y2)
		{
			//Bot=Y1
			if(y0<y2)
			{	x0=prim->xy[v1][0];		y0=prim->xy[v1][1];
				x1=prim->xy[v0][0];		y1=prim->xy[v0][1];
				x2=prim->xy[v2][0];		y2=prim->xy[v2][1];
				st0s=prim->st[v1][0];	st0t=prim->st[v1][1];
				st1s=prim->st[v0][0];	st1t=prim->st[v0][1];
				st2s=prim->st[v2][0];	st2t=prim->st[v2][1];
			}
			else
			{	x0=prim->xy[v1][0];		y0=prim->xy[v1][1];
				x1=prim->xy[v2][0];		y1=prim->xy[v2][1];
				x2=prim->xy[v0][0];		y2=prim->xy[v0][1];
				st0s=prim->st[v1][0];	st0t=prim->st[v1][1];
				st1s=prim->st[v2][0];	st1t=prim->st[v2][1];
				st2s=prim->st[v0][0];	st2t=prim->st[v0][1];
			}
		}else
			{	x0=prim->xy[v2][0];		y0=prim->xy[v2][1];
				x1=prim->xy[v1][0];		y1=prim->xy[v1][1];
				x2=prim->xy[v0][0];		y2=prim->xy[v0][1];
				st0s=prim->st[v2][0];	st0t=prim->st[v2][1];
				st1s=prim->st[v1][0];	st1t=prim->st[v1][1];
				st2s=prim->st[v0][0];	st2t=prim->st[v0][1];
			}
	}

//	clr=prim->rgba[0];
//	xs=x1-x0;
//	ys=ye1-ye0;

//	mip=0;
//	tergba=prim->tex->rgba[0];
//	txs=prim->tex->xs;
//	tys=prim->tex->ys;

	xs=max(x2-x1, max(x2-x0, x1-x0));
	ys=max(y2-y1, max(y2-y0, y1-y0));
	t0=max(st2s-st1s, max(st2s-st0s, st1s-st0s));
	t1=max(st2t-st1t, max(st2t-st0t, st1t-st0t));
	xs=xs>>XY_SHR2;	ys=ys>>XY_SHR2;
#ifdef ST_MIPSHR
	t0=t0>>ST_MIPSHR;
	t1=t1>>ST_MIPSHR;
#endif
//	j=(abs(st1s-st0s)+abs(st1t-st0t))/(xs+ys);
//	mip=bgbrasw_log2dn(j);
//	i0=st1s-st0s; i1=st1t-st0t;
	j=(t0*t0+t1*t1)/(xs*xs+ys*ys+1);
	mip=(bgbrasw_log2dn(j)+1)/2;
//	mip=0;

	tergba=prim->tex->rgba[mip];
	txs=prim->tex->xs>>mip;
	tys=prim->tex->ys>>mip;
	if(!txs)txs=1;
	if(!tys)tys=1;

	if(!tergba)
	{
		mip=0;
		tergba=prim->tex->rgba[0];
		txs=prim->tex->xs;
		tys=prim->tex->ys;
	}

	ye0=y0>>XY_SHR2;
	ye1=y1>>XY_SHR2;
	ye2=y2>>XY_SHR2;

//	if(ys>0)
	if(ye1>ye0)
	{
		ys=ye1-ye0;
		ys1=ye2-ye0;
		xc0=x0<<XY_SHL;		xc1=x0<<XY_SHL;
		xc0v=((x1-x0)<<XY_SHL)/ys;
		xc1v=((x2-x0)<<XY_SHL)/ys1;

		stsa=(st0s<<ST_SHL);		stta=(st0t<<ST_SHL);
		stsb=stsa;			sttb=stta;

#if 0
		if(xc1v<xc0v)
		{
			i=xc0v; xc0v=xc1v; xc1v=i;
			stsav=((st1s-st0s)<<ST_SHL)/ys;
			sttav=((st1t-st0t)<<ST_SHL)/ys;
			stsbv=((st2s-st0s)<<ST_SHL)/ys1;
			sttbv=((st2t-st0t)<<ST_SHL)/ys1;
		}else
		{
			stsav=((st2s-st0s)<<ST_SHL)/ys1;
			sttav=((st2t-st0t)<<ST_SHL)/ys1;
			stsbv=((st1s-st0s)<<ST_SHL)/ys;
			sttbv=((st1t-st0t)<<ST_SHL)/ys;
		}
#endif

#if 1
		if(xc1v<xc0v)
		{
			i=xc0v; xc0v=xc1v; xc1v=i;
			stsav=((st2s-st0s)<<ST_SHL)/ys1;
			sttav=((st2t-st0t)<<ST_SHL)/ys1;
			stsbv=((st1s-st0s)<<ST_SHL)/ys;
			sttbv=((st1t-st0t)<<ST_SHL)/ys;
		}else
		{
			stsav=((st1s-st0s)<<ST_SHL)/ys;
			sttav=((st1t-st0t)<<ST_SHL)/ys;
			stsbv=((st2s-st0s)<<ST_SHL)/ys1;
			sttbv=((st2t-st0t)<<ST_SHL)/ys1;
		}
#endif

		yc=ye0;
		while(yc<ye1)
		{
			sts0=stsa>>(ST_SHR+mip);		stt0=stta>>(ST_SHR+mip);
			sts1=stsb>>(ST_SHR+mip);		stt1=sttb>>(ST_SHR+mip);

			xa=xc0>>XY_SHR; xb=xc1>>XY_SHR;
			prim->tabs->drawSpanTextureBasic(prim->tabs,
				ctx->tgt_rgba+yc*ctx->tgt_xs+xa, xb-xa,
				tergba, txs, tys,
				sts0, stt0, sts1, stt1);

			stsa+=stsav;	stta+=sttav;
			stsb+=stsbv;	sttb+=sttbv;
			xc0+=xc0v; xc1+=xc1v; yc++;
		}
	}else
	{
		stsa=(st0s<<ST_SHL);		stta=(st0t<<ST_SHL);
		stsb=stsa;			sttb=stta;

		xc0=x0<<XY_SHL;
		xc1=x0<<XY_SHL;
		if(x1<x0)
		{
			stsa=(st1s<<ST_SHL);		stta=(st1t<<ST_SHL);
			xc0=x1<<XY_SHL;
		}
		if(x1>x0)
		{
			stsb=(st1s<<ST_SHL);		sttb=(st1t<<ST_SHL);
			xc1=x1<<XY_SHL;
		}
	}

//	ys=ye2-ye1;

//	if(ys>0)
	if(ye2>ye1)
	{
		ys=ye2-ye1;
		xa=xc0>>XY_SHL; xb=xc1>>XY_SHL;
		if(x1<xa) { xa=x1; xc0=x1<<XY_SHL; }
		if(x1>xb) { xb=x1; xc1=x1<<XY_SHL; }
//		ys=y2-yc;
		xc0v=((x2-xa)<<XY_SHL)/ys;
		xc1v=((x2-xb)<<XY_SHL)/ys;

//		sts0=stsa>>(8+mip);		stt0=stta>>(8+mip);
//		sts1=stsb>>(8+mip);		stt1=sttb>>(8+mip);
		sts0=stsa>>ST_SHL;		stt0=stta>>ST_SHL;
		sts1=stsb>>ST_SHL;		stt1=sttb>>ST_SHL;
		stsav=((st2s-sts0)<<ST_SHL)/ys;
		sttav=((st2t-stt0)<<ST_SHL)/ys;
		stsbv=((st2s-sts1)<<ST_SHL)/ys;
		sttbv=((st2t-stt1)<<ST_SHL)/ys;

		yc=ye1;
		while(yc<ye2)
		{
//			sts0=stsa>>(8+mip);		stt0=stta>>(8+mip);
//			sts1=stsb>>(8+mip);		stt1=sttb>>(8+mip);
			sts0=stsa>>(ST_SHR+mip);		stt0=stta>>(ST_SHR+mip);
			sts1=stsb>>(ST_SHR+mip);		stt1=sttb>>(ST_SHR+mip);

			xa=xc0>>XY_SHR; xb=xc1>>XY_SHR;
			prim->tabs->drawSpanTextureBasic(prim->tabs,
				ctx->tgt_rgba+yc*ctx->tgt_xs+xa, xb-xa,
				tergba, txs, tys,
				sts0, stt0, sts1, stt1);

			stsa+=stsav;	stta+=sttav;
			stsb+=stsbv;	sttb+=sttbv;

//			xa=xc0>>XY_SHR; xb=xc1>>XY_SHR;
//			BGBRASW_DrawSpanFlatBasic(
//				ctx->tgt_rgba+yc*ctx->tgt_xs+xa, xb-xa, clr);
			xc0+=xc0v; xc1+=xc1v; yc++;
		}
	}
}

void BGBRASW_DrawPrimitive_TriangleTexInterp(
	BGBRASW_Context *ctx,
	BGBRASW_Primitive *prim, int flag)
{
	bgbrasw_pixel clr;
	bgbrasw_pixel clr0, clr1, clr2, clr3;
	bgbrasw_pixel clr4, clr5, clr6, clr7;
	int cra, crb, crav, crbv;
	int cga, cgb, cgav, cgbv;
	int cba, cbb, cbav, cbbv;
	int caa, cab, caav, cabv;

	bgbrasw_pixel *tergba;
	int xs, ys, yc, ys1, ye0, ye1, ye2;
	int xc0, xc1, xc0v, xc1v;
	int x0, x1, x2, y0, y1, y2;
	int v0, v1, v2;
	int stsa, stsb, stsav, stsbv;
	int stta, sttb, sttav, sttbv;
	int st0s, st0t, st1s, st1t;
	int st2s, st2t, st3s, st3t;
	int sts0, stt0, sts1, stt1;
	int txs, tys, mip;
//	int s0, s1, s2;
	int t0, t1, t2;
	int xa, xb;
	int i, j, k;

	if(flag&1)	{ v0=0; v1=2; v2=3; }
	else		{ v0=0; v1=1; v2=2; }

	y0=prim->xy[v0][1];
	y1=prim->xy[v1][1];
	y2=prim->xy[v2][1];
	if(y0<y1)
	{
		//012, 021, 201
		if(y0<y2)
		{
			//Bot=Y0
			if(y1<y2)
			{	x0=prim->xy[v0][0];		y0=prim->xy[v0][1];
				x1=prim->xy[v1][0];		y1=prim->xy[v1][1];
				x2=prim->xy[v2][0];		y2=prim->xy[v2][1];
				st0s=prim->st[v0][0];	st0t=prim->st[v0][1];
				st1s=prim->st[v1][0];	st1t=prim->st[v1][1];
				st2s=prim->st[v2][0];	st2t=prim->st[v2][1];
				clr0=prim->rgba[v0];	clr1=prim->rgba[v1];
				clr2=prim->rgba[v2];
			}
			else
			{	x0=prim->xy[v0][0];		y0=prim->xy[v0][1];
				x1=prim->xy[v2][0];		y1=prim->xy[v2][1];
				x2=prim->xy[v1][0];		y2=prim->xy[v1][1];
				st0s=prim->st[v0][0];	st0t=prim->st[v0][1];
				st1s=prim->st[v2][0];	st1t=prim->st[v2][1];
				st2s=prim->st[v1][0];	st2t=prim->st[v1][1];
				clr0=prim->rgba[v0];	clr1=prim->rgba[v2];
				clr2=prim->rgba[v1];
			}
		}else
			{	x0=prim->xy[v2][0];		y0=prim->xy[v2][1];
				x1=prim->xy[v0][0];		y1=prim->xy[v0][1];
				x2=prim->xy[v1][0];		y2=prim->xy[v1][1];
				st0s=prim->st[v2][0];	st0t=prim->st[v2][1];
				st1s=prim->st[v0][0];	st1t=prim->st[v0][1];
				st2s=prim->st[v1][0];	st2t=prim->st[v1][1];
				clr0=prim->rgba[v2];	clr1=prim->rgba[v0];
				clr2=prim->rgba[v1];
			}
	}else
	{
		//102, 120, 210
		if(y1<y2)
		{
			//Bot=Y1
			if(y0<y2)
			{	x0=prim->xy[v1][0];		y0=prim->xy[v1][1];
				x1=prim->xy[v0][0];		y1=prim->xy[v0][1];
				x2=prim->xy[v2][0];		y2=prim->xy[v2][1];
				st0s=prim->st[v1][0];	st0t=prim->st[v1][1];
				st1s=prim->st[v0][0];	st1t=prim->st[v0][1];
				st2s=prim->st[v2][0];	st2t=prim->st[v2][1];
				clr0=prim->rgba[v1];	clr1=prim->rgba[v0];
				clr2=prim->rgba[v2];
			}
			else
			{	x0=prim->xy[v1][0];		y0=prim->xy[v1][1];
				x1=prim->xy[v2][0];		y1=prim->xy[v2][1];
				x2=prim->xy[v0][0];		y2=prim->xy[v0][1];
				st0s=prim->st[v1][0];	st0t=prim->st[v1][1];
				st1s=prim->st[v2][0];	st1t=prim->st[v2][1];
				st2s=prim->st[v0][0];	st2t=prim->st[v0][1];
				clr0=prim->rgba[v1];	clr1=prim->rgba[v2];
				clr2=prim->rgba[v0];
			}
		}else
			{	x0=prim->xy[v2][0];		y0=prim->xy[v2][1];
				x1=prim->xy[v1][0];		y1=prim->xy[v1][1];
				x2=prim->xy[v0][0];		y2=prim->xy[v0][1];
				st0s=prim->st[v2][0];	st0t=prim->st[v2][1];
				st1s=prim->st[v1][0];	st1t=prim->st[v1][1];
				st2s=prim->st[v0][0];	st2t=prim->st[v0][1];
				clr0=prim->rgba[v2];	clr1=prim->rgba[v1];
				clr2=prim->rgba[v0];
			}
	}

	xs=max(x2-x1, max(x2-x0, x1-x0));
	ys=max(y2-y1, max(y2-y0, y1-y0));
	t0=max(st2s-st1s, max(st2s-st0s, st1s-st0s));
	t1=max(st2t-st1t, max(st2t-st0t, st1t-st0t));
	xs=xs>>XY_SHR2;	ys=ys>>XY_SHR2;
#ifdef ST_MIPSHR
	t0=t0>>ST_MIPSHR;
	t1=t1>>ST_MIPSHR;
#endif
//	j=(abs(st1s-st0s)+abs(st1t-st0t))/(xs+ys);
//	mip=bgbrasw_log2dn(j);
//	i0=st1s-st0s; i1=st1t-st0t;
	j=(t0*t0+t1*t1)/(xs*xs+ys*ys+1);
	mip=(bgbrasw_log2dn(j)+1)/2;
//	mip=1;

	tergba=prim->tex->rgba[mip];
	txs=prim->tex->xs>>mip;
	tys=prim->tex->ys>>mip;
	if(!txs)txs=1;
	if(!tys)tys=1;

	if(!tergba)
	{
		mip=0;
		tergba=prim->tex->rgba[0];
		txs=prim->tex->xs;
		tys=prim->tex->ys;
	}

	ye0=y0>>XY_SHR2;
	ye1=y1>>XY_SHR2;
	ye2=y2>>XY_SHR2;

	if(ye1>ye0)
	{
		ys=ye1-ye0;
		ys1=ye2-ye0;
		xc0=x0<<XY_SHL;		xc1=x0<<XY_SHL;
		xc0v=((x1-x0)<<XY_SHL)/ys;
		xc1v=((x2-x0)<<XY_SHL)/ys1;

		stsa=(st0s<<ST_SHL);		stta=(st0t<<ST_SHL);
		stsb=stsa;			sttb=stta;

		cra=BGBRASW_PIXEL_R(clr0)<<16;	cga=BGBRASW_PIXEL_G(clr0)<<16;
		cba=BGBRASW_PIXEL_B(clr0)<<16;	caa=BGBRASW_PIXEL_A(clr0)<<16;
		crb=cra;	cgb=cga;	cbb=cba;	cab=caa;

		if(xc1v<xc0v)
		{
			i=xc0v; xc0v=xc1v; xc1v=i;

			stsav=((st2s-st0s)<<ST_SHL)/ys1;
			sttav=((st2t-st0t)<<ST_SHL)/ys1;
			stsbv=((st1s-st0s)<<ST_SHL)/ys;
			sttbv=((st1t-st0t)<<ST_SHL)/ys;

			crav=((BGBRASW_PIXEL_R(clr2)-BGBRASW_PIXEL_R(clr0))<<16)/ys1;
			cgav=((BGBRASW_PIXEL_G(clr2)-BGBRASW_PIXEL_G(clr0))<<16)/ys1;
			cbav=((BGBRASW_PIXEL_B(clr2)-BGBRASW_PIXEL_B(clr0))<<16)/ys1;
			caav=((BGBRASW_PIXEL_A(clr2)-BGBRASW_PIXEL_A(clr0))<<16)/ys1;
			crbv=((BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<16)/ys;
			cgbv=((BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<16)/ys;
			cbbv=((BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<16)/ys;
			cabv=((BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<16)/ys;
		}else
		{
			stsav=((st1s-st0s)<<ST_SHL)/ys;
			sttav=((st1t-st0t)<<ST_SHL)/ys;
			stsbv=((st2s-st0s)<<ST_SHL)/ys1;
			sttbv=((st2t-st0t)<<ST_SHL)/ys1;

			crav=((BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<16)/ys;
			cgav=((BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<16)/ys;
			cbav=((BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<16)/ys;
			caav=((BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<16)/ys;
			crbv=((BGBRASW_PIXEL_R(clr2)-BGBRASW_PIXEL_R(clr0))<<16)/ys1;
			cgbv=((BGBRASW_PIXEL_G(clr2)-BGBRASW_PIXEL_G(clr0))<<16)/ys1;
			cbbv=((BGBRASW_PIXEL_B(clr2)-BGBRASW_PIXEL_B(clr0))<<16)/ys1;
			cabv=((BGBRASW_PIXEL_A(clr2)-BGBRASW_PIXEL_A(clr0))<<16)/ys1;
		}

		yc=ye0;
		while(yc<ye1)
		{
			clr4=BGBRASW_MAKEPIXEL(cra>>16, cga>>16, cba>>16, caa>>16);
			clr5=BGBRASW_MAKEPIXEL(crb>>16, cgb>>16, cbb>>16, cab>>16);
			cra+=crav;	cga+=cgav;		cba+=cbav;	caa+=caav;
			crb+=crbv;	cgb+=cgbv;		cbb+=cbbv;	cab+=cabv;

			sts0=stsa>>(ST_SHR+mip);		stt0=stta>>(ST_SHR+mip);
			sts1=stsb>>(ST_SHR+mip);		stt1=sttb>>(ST_SHR+mip);

			xa=xc0>>XY_SHR; xb=xc1>>XY_SHR;
			prim->tabs->drawSpanTextureInterp(prim->tabs,
				ctx->tgt_rgba+yc*ctx->tgt_xs+xa, xb-xa,
				tergba, txs, tys,
				sts0, stt0, sts1, stt1,
				clr4, clr5);

			stsa+=stsav;	stta+=sttav;
			stsb+=stsbv;	sttb+=sttbv;
			xc0+=xc0v; xc1+=xc1v; yc++;
		}
	}else
	{
		cra=BGBRASW_PIXEL_R(clr0)<<16;	cga=BGBRASW_PIXEL_G(clr0)<<16;
		cba=BGBRASW_PIXEL_B(clr0)<<16;	caa=BGBRASW_PIXEL_A(clr0)<<16;
		crb=cra;	cgb=cga;	cbb=cba;	cab=caa;

		stsa=(st0s<<ST_SHL);		stta=(st0t<<ST_SHL);
		stsb=stsa;			sttb=stta;

		xc0=x0<<XY_SHL;
		xc1=x0<<XY_SHL;
		if(x1<x0)
		{
			cra=BGBRASW_PIXEL_R(clr1)<<16;	cga=BGBRASW_PIXEL_G(clr1)<<16;
			cba=BGBRASW_PIXEL_B(clr1)<<16;	caa=BGBRASW_PIXEL_A(clr1)<<16;
			stsa=(st1s<<ST_SHL);		stta=(st1t<<ST_SHL);
			xc0=x1<<XY_SHL;
		}
		if(x1>x0)
		{
			crb=BGBRASW_PIXEL_R(clr1)<<16;	cgb=BGBRASW_PIXEL_G(clr1)<<16;
			cbb=BGBRASW_PIXEL_B(clr1)<<16;	cab=BGBRASW_PIXEL_A(clr1)<<16;
			stsb=(st1s<<ST_SHL);		sttb=(st1t<<ST_SHL);
			xc1=x1<<XY_SHL;
		}
	}

	if(ye2>ye1)
	{
		ys=ye2-ye1;
		xa=xc0>>XY_SHL; xb=xc1>>XY_SHL;
		if(x1<xa) { xa=x1; xc0=x1<<XY_SHL; }
		if(x1>xb) { xb=x1; xc1=x1<<XY_SHL; }
		xc0v=((x2-xa)<<XY_SHL)/ys;
		xc1v=((x2-xb)<<XY_SHL)/ys;

//		sts0=stsa>>(8+mip);		stt0=stta>>(8+mip);
//		sts1=stsb>>(8+mip);		stt1=sttb>>(8+mip);
		sts0=stsa>>ST_SHL;		stt0=stta>>ST_SHL;
		sts1=stsb>>ST_SHL;		stt1=sttb>>ST_SHL;
		stsav=((st2s-sts0)<<ST_SHL)/ys;
		sttav=((st2t-stt0)<<ST_SHL)/ys;
		stsbv=((st2s-sts1)<<ST_SHL)/ys;
		sttbv=((st2t-stt1)<<ST_SHL)/ys;

		clr4=BGBRASW_MAKEPIXEL(cra>>16, cga>>16, cba>>16, caa>>16);
		clr5=BGBRASW_MAKEPIXEL(crb>>16, cgb>>16, cbb>>16, cab>>16);
		crav=((BGBRASW_PIXEL_R(clr2)-BGBRASW_PIXEL_R(clr4))<<16)/ys;
		cgav=((BGBRASW_PIXEL_G(clr2)-BGBRASW_PIXEL_G(clr4))<<16)/ys;
		cbav=((BGBRASW_PIXEL_B(clr2)-BGBRASW_PIXEL_B(clr4))<<16)/ys;
		caav=((BGBRASW_PIXEL_A(clr2)-BGBRASW_PIXEL_A(clr4))<<16)/ys;
		crbv=((BGBRASW_PIXEL_R(clr2)-BGBRASW_PIXEL_R(clr5))<<16)/ys;
		cgbv=((BGBRASW_PIXEL_G(clr2)-BGBRASW_PIXEL_G(clr5))<<16)/ys;
		cbbv=((BGBRASW_PIXEL_B(clr2)-BGBRASW_PIXEL_B(clr5))<<16)/ys;
		cabv=((BGBRASW_PIXEL_A(clr2)-BGBRASW_PIXEL_A(clr5))<<16)/ys;

		yc=ye1;
		while(yc<ye2)
		{
			sts0=stsa>>(ST_SHR+mip);		stt0=stta>>(ST_SHR+mip);
			sts1=stsb>>(ST_SHR+mip);		stt1=sttb>>(ST_SHR+mip);

			clr4=BGBRASW_MAKEPIXEL(cra>>16, cga>>16, cba>>16, caa>>16);
			clr5=BGBRASW_MAKEPIXEL(crb>>16, cgb>>16, cbb>>16, cab>>16);
			cra+=crav;	cga+=cgav;		cba+=cbav;	caa+=caav;
			crb+=crbv;	cgb+=cgbv;		cbb+=cbbv;	cab+=cabv;

			xa=xc0>>XY_SHR; xb=xc1>>XY_SHR;
			prim->tabs->drawSpanTextureInterp(prim->tabs,
				ctx->tgt_rgba+yc*ctx->tgt_xs+xa, xb-xa,
				tergba, txs, tys,
				sts0, stt0, sts1, stt1,
				clr4, clr5);

			stsa+=stsav;	stta+=sttav;
			stsb+=stsbv;	sttb+=sttbv;
			xc0+=xc0v; xc1+=xc1v; yc++;
		}
	}
}

void BGBRASW_DrawPrimitive_TriangleTexBasicZTest(
	BGBRASW_Context *ctx,
	BGBRASW_Primitive *prim, int flag)
{
	bgbrasw_pixel clr;
	bgbrasw_pixel clr0, clr1, clr2, clr3;
	bgbrasw_pixel clr4, clr5, clr6, clr7;
//	int cra, crb, crav, crbv;
//	int cga, cgb, cgav, cgbv;
//	int cba, cbb, cbav, cbbv;
//	int caa, cab, caav, cabv;

	bgbrasw_pixel *tergba;
	int xs, ys, yc, ys1, ye0, ye1, ye2;
	int xc0, xc1, xc0v, xc1v;
	int x0, x1, x2, y0, y1, y2;
	bgbrasw_zbuf z0, z1, z2;
	int v0, v1, v2;
	int stsa, stsb, stsav, stsbv;
	int stta, sttb, sttav, sttbv;
	int st0s, st0t, st1s, st1t;
	int st2s, st2t, st3s, st3t;
	int sts0, stt0, sts1, stt1;
	int zsa, zsb, zsav, zsbv;
	int txs, tys, mip;
//	int s0, s1, s2;
	int t0, t1, t2;
	int xa, xb;
	int i, j, k;

//	return;

	if(flag&1)	{ v0=0; v1=2; v2=3; }
	else		{ v0=0; v1=1; v2=2; }

	y0=prim->xy[v0][1];
	y1=prim->xy[v1][1];
	y2=prim->xy[v2][1];
	if(y0<y1)
	{
		//012, 021, 201
		if(y0<y2)
		{
			//Bot=Y0
			if(y1<y2)
			{	x0=prim->xy[v0][0];		y0=prim->xy[v0][1];
				x1=prim->xy[v1][0];		y1=prim->xy[v1][1];
				x2=prim->xy[v2][0];		y2=prim->xy[v2][1];
				st0s=prim->st[v0][0];	st0t=prim->st[v0][1];
				st1s=prim->st[v1][0];	st1t=prim->st[v1][1];
				st2s=prim->st[v2][0];	st2t=prim->st[v2][1];
				clr0=prim->rgba[v0];	clr1=prim->rgba[v1];
				clr2=prim->rgba[v2];	z0=prim->z[v0];
				z1=prim->z[v1];			z2=prim->z[v2];
			}
			else
			{	x0=prim->xy[v0][0];		y0=prim->xy[v0][1];
				x1=prim->xy[v2][0];		y1=prim->xy[v2][1];
				x2=prim->xy[v1][0];		y2=prim->xy[v1][1];
				st0s=prim->st[v0][0];	st0t=prim->st[v0][1];
				st1s=prim->st[v2][0];	st1t=prim->st[v2][1];
				st2s=prim->st[v1][0];	st2t=prim->st[v1][1];
				clr0=prim->rgba[v0];	clr1=prim->rgba[v2];
				clr2=prim->rgba[v1];	z0=prim->z[v0];
				z1=prim->z[v2];			z2=prim->z[v1];
			}
		}else
			{	x0=prim->xy[v2][0];		y0=prim->xy[v2][1];
				x1=prim->xy[v0][0];		y1=prim->xy[v0][1];
				x2=prim->xy[v1][0];		y2=prim->xy[v1][1];
				st0s=prim->st[v2][0];	st0t=prim->st[v2][1];
				st1s=prim->st[v0][0];	st1t=prim->st[v0][1];
				st2s=prim->st[v1][0];	st2t=prim->st[v1][1];
				clr0=prim->rgba[v2];	clr1=prim->rgba[v0];
				clr2=prim->rgba[v1];	z0=prim->z[v2];
				z1=prim->z[v0];			z2=prim->z[v1];
			}
	}else
	{
		//102, 120, 210
		if(y1<y2)
		{
			//Bot=Y1
			if(y0<y2)
			{	x0=prim->xy[v1][0];		y0=prim->xy[v1][1];
				x1=prim->xy[v0][0];		y1=prim->xy[v0][1];
				x2=prim->xy[v2][0];		y2=prim->xy[v2][1];
				st0s=prim->st[v1][0];	st0t=prim->st[v1][1];
				st1s=prim->st[v0][0];	st1t=prim->st[v0][1];
				st2s=prim->st[v2][0];	st2t=prim->st[v2][1];
				clr0=prim->rgba[v1];	clr1=prim->rgba[v0];
				clr2=prim->rgba[v2];	z0=prim->z[v1];
				z1=prim->z[v0];			z2=prim->z[v2];
			}
			else
			{	x0=prim->xy[v1][0];		y0=prim->xy[v1][1];
				x1=prim->xy[v2][0];		y1=prim->xy[v2][1];
				x2=prim->xy[v0][0];		y2=prim->xy[v0][1];
				st0s=prim->st[v1][0];	st0t=prim->st[v1][1];
				st1s=prim->st[v2][0];	st1t=prim->st[v2][1];
				st2s=prim->st[v0][0];	st2t=prim->st[v0][1];
				clr0=prim->rgba[v1];	clr1=prim->rgba[v2];
				clr2=prim->rgba[v0];	z0=prim->z[v1];
				z1=prim->z[v2];			z2=prim->z[v0];
			}
		}else
			{	x0=prim->xy[v2][0];		y0=prim->xy[v2][1];
				x1=prim->xy[v1][0];		y1=prim->xy[v1][1];
				x2=prim->xy[v0][0];		y2=prim->xy[v0][1];
				st0s=prim->st[v2][0];	st0t=prim->st[v2][1];
				st1s=prim->st[v1][0];	st1t=prim->st[v1][1];
				st2s=prim->st[v0][0];	st2t=prim->st[v0][1];
				clr0=prim->rgba[v2];	clr1=prim->rgba[v1];
				clr2=prim->rgba[v0];	z0=prim->z[v2];
				z1=prim->z[v1];			z2=prim->z[v0];
			}
	}

	xs=max(x2-x1, max(x2-x0, x1-x0));
	ys=max(y2-y1, max(y2-y0, y1-y0));
	t0=max(st2s-st1s, max(st2s-st0s, st1s-st0s));
	t1=max(st2t-st1t, max(st2t-st0t, st1t-st0t));
	xs=xs>>XY_SHR2;	ys=ys>>XY_SHR2;
#ifdef ST_MIPSHR
	t0=t0>>ST_MIPSHR;
	t1=t1>>ST_MIPSHR;
#endif
//	j=(abs(st1s-st0s)+abs(st1t-st0t))/(xs+ys);
//	mip=bgbrasw_log2dn(j);
//	i0=st1s-st0s; i1=st1t-st0t;
	j=(t0*t0+t1*t1)/(xs*xs+ys*ys+1);
	mip=(bgbrasw_log2dn(j)+1)/2;
	if(mip>15)mip=15;
//	mip=0;

	tergba=prim->tex->rgba[mip];
	txs=prim->tex->xs>>mip;
	tys=prim->tex->ys>>mip;
	if(!txs)txs=1;
	if(!tys)tys=1;

	if(!tergba)
	{
		mip=0;
		tergba=prim->tex->rgba[0];
		txs=prim->tex->xs;
		tys=prim->tex->ys;
	}

	ye0=y0>>XY_SHR2;
	ye1=y1>>XY_SHR2;
	ye2=y2>>XY_SHR2;

	if(ye1>ye0)
	{
		ys=ye1-ye0;
		ys1=ye2-ye0;
		xc0=x0<<XY_SHL;		xc1=x0<<XY_SHL;
		xc0v=((x1-x0)<<XY_SHL)/ys;
		xc1v=((x2-x0)<<XY_SHL)/ys1;

		stsa=(st0s<<ST_SHL);		stta=(st0t<<ST_SHL);
		stsb=stsa;			sttb=stta;
		zsa=z0;				zsb=z0;

//		cra=BGBRASW_PIXEL_R(clr0)<<16;	cga=BGBRASW_PIXEL_G(clr0)<<16;
//		cba=BGBRASW_PIXEL_B(clr0)<<16;	caa=BGBRASW_PIXEL_A(clr0)<<16;
//		crb=cra;	cgb=cga;	cbb=cba;	cab=caa;

		if(xc1v<xc0v)
		{
			i=xc0v; xc0v=xc1v; xc1v=i;

			stsav=((st2s-st0s)<<ST_SHL)/ys1;
			sttav=((st2t-st0t)<<ST_SHL)/ys1;
			stsbv=((st1s-st0s)<<ST_SHL)/ys;
			sttbv=((st1t-st0t)<<ST_SHL)/ys;

			zsav=((s32)(z2-z0))/ys1;
			zsbv=((s32)(z1-z0))/ys;
			zsav&=BGBRASW_MASK_DEPTH;
			zsbv&=BGBRASW_MASK_DEPTH;

//			crav=((BGBRASW_PIXEL_R(clr2)-BGBRASW_PIXEL_R(clr0))<<16)/ys1;
//			cgav=((BGBRASW_PIXEL_G(clr2)-BGBRASW_PIXEL_G(clr0))<<16)/ys1;
//			cbav=((BGBRASW_PIXEL_B(clr2)-BGBRASW_PIXEL_B(clr0))<<16)/ys1;
//			caav=((BGBRASW_PIXEL_A(clr2)-BGBRASW_PIXEL_A(clr0))<<16)/ys1;
//			crbv=((BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<16)/ys;
//			cgbv=((BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<16)/ys;
//			cbbv=((BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<16)/ys;
//			cabv=((BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<16)/ys;
		}else
		{
			stsav=((st1s-st0s)<<ST_SHL)/ys;
			sttav=((st1t-st0t)<<ST_SHL)/ys;
			stsbv=((st2s-st0s)<<ST_SHL)/ys1;
			sttbv=((st2t-st0t)<<ST_SHL)/ys1;

			zsav=((s32)(z1-z0))/ys;
			zsbv=((s32)(z2-z0))/ys1;
			zsav&=BGBRASW_MASK_DEPTH;
			zsbv&=BGBRASW_MASK_DEPTH;

//			crav=((BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<16)/ys;
//			cgav=((BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<16)/ys;
//			cbav=((BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<16)/ys;
//			caav=((BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<16)/ys;
//			crbv=((BGBRASW_PIXEL_R(clr2)-BGBRASW_PIXEL_R(clr0))<<16)/ys1;
//			cgbv=((BGBRASW_PIXEL_G(clr2)-BGBRASW_PIXEL_G(clr0))<<16)/ys1;
//			cbbv=((BGBRASW_PIXEL_B(clr2)-BGBRASW_PIXEL_B(clr0))<<16)/ys1;
//			cabv=((BGBRASW_PIXEL_A(clr2)-BGBRASW_PIXEL_A(clr0))<<16)/ys1;
		}

		yc=ye0;
		while(yc<ye1)
		{
//			clr4=BGBRASW_MAKEPIXEL(cra>>16, cga>>16, cba>>16, caa>>16);
//			clr5=BGBRASW_MAKEPIXEL(crb>>16, cgb>>16, cbb>>16, cab>>16);
//			cra+=crav;	cga+=cgav;		cba+=cbav;	caa+=caav;
//			crb+=crbv;	cgb+=cgbv;		cbb+=cbbv;	cab+=cabv;

			sts0=stsa>>(ST_SHR+mip);		stt0=stta>>(ST_SHR+mip);
			sts1=stsb>>(ST_SHR+mip);		stt1=sttb>>(ST_SHR+mip);

			xa=xc0>>XY_SHR; xb=xc1>>XY_SHR;
			prim->tabs->drawSpanTextureBasicZTest(prim->tabs,
				ctx->tgt_rgba+yc*ctx->tgt_xs+xa,
				ctx->tgt_zbuf+yc*ctx->tgt_xs+xa,
				xb-xa,
				tergba, txs, tys,
				sts0, stt0, sts1, stt1,
				zsa, zsb);

			zsa+=zsav;		zsb+=zsbv;
			stsa+=stsav;	stta+=sttav;
			stsb+=stsbv;	sttb+=sttbv;
			xc0+=xc0v; xc1+=xc1v; yc++;
		}
	}else
	{
//		cra=BGBRASW_PIXEL_R(clr0)<<16;	cga=BGBRASW_PIXEL_G(clr0)<<16;
//		cba=BGBRASW_PIXEL_B(clr0)<<16;	caa=BGBRASW_PIXEL_A(clr0)<<16;
//		crb=cra;	cgb=cga;	cbb=cba;	cab=caa;

		stsa=(st0s<<ST_SHL);		stta=(st0t<<ST_SHL);
		stsb=stsa;			sttb=stta;
		zsa=z0;				zsb=z0;

		xc0=x0<<XY_SHL;
		xc1=x0<<XY_SHL;
		if(x1<x0)
		{
//			cra=BGBRASW_PIXEL_R(clr1)<<16;	cga=BGBRASW_PIXEL_G(clr1)<<16;
//			cba=BGBRASW_PIXEL_B(clr1)<<16;	caa=BGBRASW_PIXEL_A(clr1)<<16;
			stsa=(st1s<<ST_SHL);		stta=(st1t<<ST_SHL);
			zsa=z1;
			xc0=x1<<XY_SHL;
		}
		if(x1>x0)
		{
//			crb=BGBRASW_PIXEL_R(clr1)<<16;	cgb=BGBRASW_PIXEL_G(clr1)<<16;
//			cbb=BGBRASW_PIXEL_B(clr1)<<16;	cab=BGBRASW_PIXEL_A(clr1)<<16;
			stsb=(st1s<<ST_SHL);		sttb=(st1t<<ST_SHL);
			zsb=z1;
			xc1=x1<<XY_SHL;
		}
	}

	if(ye2>ye1)
	{
		ys=ye2-ye1;
		xa=xc0>>XY_SHL; xb=xc1>>XY_SHL;
		if(x1<xa) { xa=x1; xc0=x1<<XY_SHL; }
		if(x1>xb) { xb=x1; xc1=x1<<XY_SHL; }
		xc0v=((x2-xa)<<XY_SHL)/ys;
		xc1v=((x2-xb)<<XY_SHL)/ys;

//		sts0=stsa>>(8+mip);		stt0=stta>>(8+mip);
//		sts1=stsb>>(8+mip);		stt1=sttb>>(8+mip);
		sts0=stsa>>ST_SHL;		stt0=stta>>ST_SHL;
		sts1=stsb>>ST_SHL;		stt1=sttb>>ST_SHL;
		stsav=((st2s-sts0)<<ST_SHL)/ys;
		sttav=((st2t-stt0)<<ST_SHL)/ys;
		stsbv=((st2s-sts1)<<ST_SHL)/ys;
		sttbv=((st2t-stt1)<<ST_SHL)/ys;

		zsav=((s32)(z2-zsa))/ys;
		zsbv=((s32)(z2-zsb))/ys;
		zsav&=BGBRASW_MASK_DEPTH;
		zsbv&=BGBRASW_MASK_DEPTH;

//		clr4=BGBRASW_MAKEPIXEL(cra>>16, cga>>16, cba>>16, caa>>16);
//		clr5=BGBRASW_MAKEPIXEL(crb>>16, cgb>>16, cbb>>16, cab>>16);
//		crav=((BGBRASW_PIXEL_R(clr2)-BGBRASW_PIXEL_R(clr4))<<16)/ys;
//		cgav=((BGBRASW_PIXEL_G(clr2)-BGBRASW_PIXEL_G(clr4))<<16)/ys;
//		cbav=((BGBRASW_PIXEL_B(clr2)-BGBRASW_PIXEL_B(clr4))<<16)/ys;
//		caav=((BGBRASW_PIXEL_A(clr2)-BGBRASW_PIXEL_A(clr4))<<16)/ys;
//		crbv=((BGBRASW_PIXEL_R(clr2)-BGBRASW_PIXEL_R(clr5))<<16)/ys;
//		cgbv=((BGBRASW_PIXEL_G(clr2)-BGBRASW_PIXEL_G(clr5))<<16)/ys;
//		cbbv=((BGBRASW_PIXEL_B(clr2)-BGBRASW_PIXEL_B(clr5))<<16)/ys;
//		cabv=((BGBRASW_PIXEL_A(clr2)-BGBRASW_PIXEL_A(clr5))<<16)/ys;

		yc=ye1;
		while(yc<ye2)
		{
			sts0=stsa>>(ST_SHR+mip);		stt0=stta>>(ST_SHR+mip);
			sts1=stsb>>(ST_SHR+mip);		stt1=sttb>>(ST_SHR+mip);

//			clr4=BGBRASW_MAKEPIXEL(cra>>16, cga>>16, cba>>16, caa>>16);
//			clr5=BGBRASW_MAKEPIXEL(crb>>16, cgb>>16, cbb>>16, cab>>16);
//			cra+=crav;	cga+=cgav;		cba+=cbav;	caa+=caav;
//			crb+=crbv;	cgb+=cgbv;		cbb+=cbbv;	cab+=cabv;

			xa=xc0>>XY_SHR; xb=xc1>>XY_SHR;
			prim->tabs->drawSpanTextureBasicZTest(prim->tabs,
				ctx->tgt_rgba+yc*ctx->tgt_xs+xa,
				ctx->tgt_zbuf+yc*ctx->tgt_xs+xa,
				xb-xa,
				tergba, txs, tys,
				sts0, stt0, sts1, stt1,
				zsa, zsb);

			zsa+=zsav;		zsb+=zsbv;
			stsa+=stsav;	stta+=sttav;
			stsb+=stsbv;	sttb+=sttbv;
			xc0+=xc0v; xc1+=xc1v; yc++;
		}
	}
}

void BGBRASW_DrawPrimitive_TriangleTexInterpZTest(
	BGBRASW_Context *ctx,
	BGBRASW_Primitive *prim, int flag)
{
	bgbrasw_pixel clr;
	bgbrasw_pixel clr0, clr1, clr2, clr3;
	bgbrasw_pixel clr4, clr5, clr6, clr7;
	int cra, crb, crav, crbv;
	int cga, cgb, cgav, cgbv;
	int cba, cbb, cbav, cbbv;
	int caa, cab, caav, cabv;

	bgbrasw_pixel *tergba;
	int xs, ys, yc, ys1, ye0, ye1, ye2;
	int xc0, xc1, xc0v, xc1v;
	int x0, x1, x2, y0, y1, y2;
	bgbrasw_zbuf z0, z1, z2;
	int v0, v1, v2;
	int stsa, stsb, stsav, stsbv;
	int stta, sttb, sttav, sttbv;
	int st0s, st0t, st1s, st1t;
	int st2s, st2t, st3s, st3t;
	int sts0, stt0, sts1, stt1;
	int zsa, zsb, zsav, zsbv;
	int txs, tys, mip;
//	int s0, s1, s2;
	int t0, t1, t2;
	int xa, xb;
	int i, j, k;

//	return;

	if(flag&1)	{ v0=0; v1=2; v2=3; }
	else		{ v0=0; v1=1; v2=2; }

	y0=prim->xy[v0][1];
	y1=prim->xy[v1][1];
	y2=prim->xy[v2][1];
	if(y0<y1)
	{
		//012, 021, 201
		if(y0<y2)
		{
			//Bot=Y0
			if(y1<y2)
			{	x0=prim->xy[v0][0];		y0=prim->xy[v0][1];
				x1=prim->xy[v1][0];		y1=prim->xy[v1][1];
				x2=prim->xy[v2][0];		y2=prim->xy[v2][1];
				st0s=prim->st[v0][0];	st0t=prim->st[v0][1];
				st1s=prim->st[v1][0];	st1t=prim->st[v1][1];
				st2s=prim->st[v2][0];	st2t=prim->st[v2][1];
				clr0=prim->rgba[v0];	clr1=prim->rgba[v1];
				clr2=prim->rgba[v2];	z0=prim->z[v0];
				z1=prim->z[v1];			z2=prim->z[v2];
			}
			else
			{	x0=prim->xy[v0][0];		y0=prim->xy[v0][1];
				x1=prim->xy[v2][0];		y1=prim->xy[v2][1];
				x2=prim->xy[v1][0];		y2=prim->xy[v1][1];
				st0s=prim->st[v0][0];	st0t=prim->st[v0][1];
				st1s=prim->st[v2][0];	st1t=prim->st[v2][1];
				st2s=prim->st[v1][0];	st2t=prim->st[v1][1];
				clr0=prim->rgba[v0];	clr1=prim->rgba[v2];
				clr2=prim->rgba[v1];	z0=prim->z[v0];
				z1=prim->z[v2];			z2=prim->z[v1];
			}
		}else
			{	x0=prim->xy[v2][0];		y0=prim->xy[v2][1];
				x1=prim->xy[v0][0];		y1=prim->xy[v0][1];
				x2=prim->xy[v1][0];		y2=prim->xy[v1][1];
				st0s=prim->st[v2][0];	st0t=prim->st[v2][1];
				st1s=prim->st[v0][0];	st1t=prim->st[v0][1];
				st2s=prim->st[v1][0];	st2t=prim->st[v1][1];
				clr0=prim->rgba[v2];	clr1=prim->rgba[v0];
				clr2=prim->rgba[v1];	z0=prim->z[v2];
				z1=prim->z[v0];			z2=prim->z[v1];
			}
	}else
	{
		//102, 120, 210
		if(y1<y2)
		{
			//Bot=Y1
			if(y0<y2)
			{	x0=prim->xy[v1][0];		y0=prim->xy[v1][1];
				x1=prim->xy[v0][0];		y1=prim->xy[v0][1];
				x2=prim->xy[v2][0];		y2=prim->xy[v2][1];
				st0s=prim->st[v1][0];	st0t=prim->st[v1][1];
				st1s=prim->st[v0][0];	st1t=prim->st[v0][1];
				st2s=prim->st[v2][0];	st2t=prim->st[v2][1];
				clr0=prim->rgba[v1];	clr1=prim->rgba[v0];
				clr2=prim->rgba[v2];	z0=prim->z[v1];
				z1=prim->z[v0];			z2=prim->z[v2];
			}
			else
			{	x0=prim->xy[v1][0];		y0=prim->xy[v1][1];
				x1=prim->xy[v2][0];		y1=prim->xy[v2][1];
				x2=prim->xy[v0][0];		y2=prim->xy[v0][1];
				st0s=prim->st[v1][0];	st0t=prim->st[v1][1];
				st1s=prim->st[v2][0];	st1t=prim->st[v2][1];
				st2s=prim->st[v0][0];	st2t=prim->st[v0][1];
				clr0=prim->rgba[v1];	clr1=prim->rgba[v2];
				clr2=prim->rgba[v0];	z0=prim->z[v1];
				z1=prim->z[v2];			z2=prim->z[v0];
			}
		}else
			{	x0=prim->xy[v2][0];		y0=prim->xy[v2][1];
				x1=prim->xy[v1][0];		y1=prim->xy[v1][1];
				x2=prim->xy[v0][0];		y2=prim->xy[v0][1];
				st0s=prim->st[v2][0];	st0t=prim->st[v2][1];
				st1s=prim->st[v1][0];	st1t=prim->st[v1][1];
				st2s=prim->st[v0][0];	st2t=prim->st[v0][1];
				clr0=prim->rgba[v2];	clr1=prim->rgba[v1];
				clr2=prim->rgba[v0];	z0=prim->z[v2];
				z1=prim->z[v1];			z2=prim->z[v0];
			}
	}

	xs=max(x2-x1, max(x2-x0, x1-x0));
	ys=max(y2-y1, max(y2-y0, y1-y0));
	t0=max(st2s-st1s, max(st2s-st0s, st1s-st0s));
	t1=max(st2t-st1t, max(st2t-st0t, st1t-st0t));
	xs=xs>>XY_SHR2;	ys=ys>>XY_SHR2;
#ifdef ST_MIPSHR
	t0=t0>>ST_MIPSHR;
	t1=t1>>ST_MIPSHR;
#endif
//	j=(abs(st1s-st0s)+abs(st1t-st0t))/(xs+ys);
//	mip=bgbrasw_log2dn(j);
//	i0=st1s-st0s; i1=st1t-st0t;
	j=(t0*t0+t1*t1)/(xs*xs+ys*ys+1);
	mip=(bgbrasw_log2dn(j)+1)/2;
//	mip=1;

	tergba=prim->tex->rgba[mip];
	txs=prim->tex->xs>>mip;
	tys=prim->tex->ys>>mip;
	if(!txs)txs=1;
	if(!tys)tys=1;

	if(!tergba)
	{
		mip=0;
		tergba=prim->tex->rgba[0];
		txs=prim->tex->xs;
		tys=prim->tex->ys;
	}

	ye0=y0>>XY_SHR2;
	ye1=y1>>XY_SHR2;
	ye2=y2>>XY_SHR2;

	if(ye1>ye0)
	{
		ys=ye1-ye0;
		ys1=ye2-ye0;
		xc0=x0<<XY_SHL;		xc1=x0<<XY_SHL;
		xc0v=((x1-x0)<<XY_SHL)/ys;
		xc1v=((x2-x0)<<XY_SHL)/ys1;

		stsa=(st0s<<ST_SHL);		stta=(st0t<<ST_SHL);
		stsb=stsa;			sttb=stta;
		zsa=z0;				zsb=z0;

		cra=BGBRASW_PIXEL_R(clr0)<<16;	cga=BGBRASW_PIXEL_G(clr0)<<16;
		cba=BGBRASW_PIXEL_B(clr0)<<16;	caa=BGBRASW_PIXEL_A(clr0)<<16;
		crb=cra;	cgb=cga;	cbb=cba;	cab=caa;

		if(xc1v<xc0v)
		{
			i=xc0v; xc0v=xc1v; xc1v=i;

			stsav=((st2s-st0s)<<ST_SHL)/ys1;
			sttav=((st2t-st0t)<<ST_SHL)/ys1;
			stsbv=((st1s-st0s)<<ST_SHL)/ys;
			sttbv=((st1t-st0t)<<ST_SHL)/ys;

			zsav=((s32)(z2-z0))/ys1;
			zsbv=((s32)(z1-z0))/ys;
			zsav&=BGBRASW_MASK_DEPTH;
			zsbv&=BGBRASW_MASK_DEPTH;

			crav=((BGBRASW_PIXEL_R(clr2)-BGBRASW_PIXEL_R(clr0))<<16)/ys1;
			cgav=((BGBRASW_PIXEL_G(clr2)-BGBRASW_PIXEL_G(clr0))<<16)/ys1;
			cbav=((BGBRASW_PIXEL_B(clr2)-BGBRASW_PIXEL_B(clr0))<<16)/ys1;
			caav=((BGBRASW_PIXEL_A(clr2)-BGBRASW_PIXEL_A(clr0))<<16)/ys1;
			crbv=((BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<16)/ys;
			cgbv=((BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<16)/ys;
			cbbv=((BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<16)/ys;
			cabv=((BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<16)/ys;
		}else
		{
			stsav=((st1s-st0s)<<ST_SHL)/ys;
			sttav=((st1t-st0t)<<ST_SHL)/ys;
			stsbv=((st2s-st0s)<<ST_SHL)/ys1;
			sttbv=((st2t-st0t)<<ST_SHL)/ys1;

			zsav=((s32)(z1-z0))/ys;
			zsbv=((s32)(z2-z0))/ys1;
			zsav&=BGBRASW_MASK_DEPTH;
			zsbv&=BGBRASW_MASK_DEPTH;

			crav=((BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<16)/ys;
			cgav=((BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<16)/ys;
			cbav=((BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<16)/ys;
			caav=((BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<16)/ys;
			crbv=((BGBRASW_PIXEL_R(clr2)-BGBRASW_PIXEL_R(clr0))<<16)/ys1;
			cgbv=((BGBRASW_PIXEL_G(clr2)-BGBRASW_PIXEL_G(clr0))<<16)/ys1;
			cbbv=((BGBRASW_PIXEL_B(clr2)-BGBRASW_PIXEL_B(clr0))<<16)/ys1;
			cabv=((BGBRASW_PIXEL_A(clr2)-BGBRASW_PIXEL_A(clr0))<<16)/ys1;
		}

		yc=ye0;
		while(yc<ye1)
		{
			clr4=BGBRASW_MAKEPIXEL(cra>>16, cga>>16, cba>>16, caa>>16);
			clr5=BGBRASW_MAKEPIXEL(crb>>16, cgb>>16, cbb>>16, cab>>16);
			cra+=crav;	cga+=cgav;		cba+=cbav;	caa+=caav;
			crb+=crbv;	cgb+=cgbv;		cbb+=cbbv;	cab+=cabv;

			sts0=stsa>>(ST_SHR+mip);		stt0=stta>>(ST_SHR+mip);
			sts1=stsb>>(ST_SHR+mip);		stt1=sttb>>(ST_SHR+mip);

			xa=xc0>>XY_SHR; xb=xc1>>XY_SHR;
			prim->tabs->drawSpanTextureInterpZTest(prim->tabs,
				ctx->tgt_rgba+yc*ctx->tgt_xs+xa,
				ctx->tgt_zbuf+yc*ctx->tgt_xs+xa,
				xb-xa,
				tergba, txs, tys,
				sts0, stt0, sts1, stt1,
				clr4, clr5, zsa, zsb);

			zsa+=zsav;		zsb+=zsbv;
			stsa+=stsav;	stta+=sttav;
			stsb+=stsbv;	sttb+=sttbv;
			xc0+=xc0v; xc1+=xc1v; yc++;
		}
	}else
	{
		cra=BGBRASW_PIXEL_R(clr0)<<16;	cga=BGBRASW_PIXEL_G(clr0)<<16;
		cba=BGBRASW_PIXEL_B(clr0)<<16;	caa=BGBRASW_PIXEL_A(clr0)<<16;
		crb=cra;	cgb=cga;	cbb=cba;	cab=caa;

		stsa=(st0s<<ST_SHL);		stta=(st0t<<ST_SHL);
		stsb=stsa;			sttb=stta;
		zsa=z0;				zsb=z0;

		xc0=x0<<XY_SHL;
		xc1=x0<<XY_SHL;
		if(x1<x0)
		{
			cra=BGBRASW_PIXEL_R(clr1)<<16;	cga=BGBRASW_PIXEL_G(clr1)<<16;
			cba=BGBRASW_PIXEL_B(clr1)<<16;	caa=BGBRASW_PIXEL_A(clr1)<<16;
			stsa=(st1s<<ST_SHL);		stta=(st1t<<ST_SHL);
			zsa=z1;
			xc0=x1<<XY_SHL;
		}
		if(x1>x0)
		{
			crb=BGBRASW_PIXEL_R(clr1)<<16;	cgb=BGBRASW_PIXEL_G(clr1)<<16;
			cbb=BGBRASW_PIXEL_B(clr1)<<16;	cab=BGBRASW_PIXEL_A(clr1)<<16;
			stsb=(st1s<<ST_SHL);		sttb=(st1t<<ST_SHL);
			zsb=z1;
			xc1=x1<<XY_SHL;
		}
	}

	if(ye2>ye1)
	{
		ys=ye2-ye1;
		xa=xc0>>XY_SHL; xb=xc1>>XY_SHL;
		if(x1<xa) { xa=x1; xc0=x1<<XY_SHL; }
		if(x1>xb) { xb=x1; xc1=x1<<XY_SHL; }
		xc0v=((x2-xa)<<XY_SHL)/ys;
		xc1v=((x2-xb)<<XY_SHL)/ys;

//		sts0=stsa>>(ST_SHR+mip);		stt0=stta>>(ST_SHR+mip);
//		sts1=stsb>>(ST_SHR+mip);		stt1=sttb>>(ST_SHR+mip);
		sts0=stsa>>ST_SHL;		stt0=stta>>ST_SHL;
		sts1=stsb>>ST_SHL;		stt1=sttb>>ST_SHL;
		stsav=((st2s-sts0)<<ST_SHL)/ys;
		sttav=((st2t-stt0)<<ST_SHL)/ys;
		stsbv=((st2s-sts1)<<ST_SHL)/ys;
		sttbv=((st2t-stt1)<<ST_SHL)/ys;

		zsav=((s32)(z2-zsa))/ys;
		zsbv=((s32)(z2-zsb))/ys;
		zsav&=BGBRASW_MASK_DEPTH;
		zsbv&=BGBRASW_MASK_DEPTH;

		clr4=BGBRASW_MAKEPIXEL(cra>>16, cga>>16, cba>>16, caa>>16);
		clr5=BGBRASW_MAKEPIXEL(crb>>16, cgb>>16, cbb>>16, cab>>16);
		crav=((BGBRASW_PIXEL_R(clr2)-BGBRASW_PIXEL_R(clr4))<<16)/ys;
		cgav=((BGBRASW_PIXEL_G(clr2)-BGBRASW_PIXEL_G(clr4))<<16)/ys;
		cbav=((BGBRASW_PIXEL_B(clr2)-BGBRASW_PIXEL_B(clr4))<<16)/ys;
		caav=((BGBRASW_PIXEL_A(clr2)-BGBRASW_PIXEL_A(clr4))<<16)/ys;
		crbv=((BGBRASW_PIXEL_R(clr2)-BGBRASW_PIXEL_R(clr5))<<16)/ys;
		cgbv=((BGBRASW_PIXEL_G(clr2)-BGBRASW_PIXEL_G(clr5))<<16)/ys;
		cbbv=((BGBRASW_PIXEL_B(clr2)-BGBRASW_PIXEL_B(clr5))<<16)/ys;
		cabv=((BGBRASW_PIXEL_A(clr2)-BGBRASW_PIXEL_A(clr5))<<16)/ys;

		yc=ye1;
		while(yc<ye2)
		{
			sts0=stsa>>(ST_SHR+mip);		stt0=stta>>(ST_SHR+mip);
			sts1=stsb>>(ST_SHR+mip);		stt1=sttb>>(ST_SHR+mip);

			clr4=BGBRASW_MAKEPIXEL(cra>>16, cga>>16, cba>>16, caa>>16);
			clr5=BGBRASW_MAKEPIXEL(crb>>16, cgb>>16, cbb>>16, cab>>16);
			cra+=crav;	cga+=cgav;		cba+=cbav;	caa+=caav;
			crb+=crbv;	cgb+=cgbv;		cbb+=cbbv;	cab+=cabv;

			xa=xc0>>XY_SHR; xb=xc1>>XY_SHR;
			prim->tabs->drawSpanTextureInterpZTest(prim->tabs,
				ctx->tgt_rgba+yc*ctx->tgt_xs+xa,
				ctx->tgt_zbuf+yc*ctx->tgt_xs+xa,
				xb-xa,
				tergba, txs, tys,
				sts0, stt0, sts1, stt1,
				clr4, clr5, zsa, zsb);

			zsa+=zsav;		zsb+=zsbv;
			stsa+=stsav;	stta+=sttav;
			stsb+=stsbv;	sttb+=sttbv;
			xc0+=xc0v; xc1+=xc1v; yc++;
		}
	}
}

void BGBRASW_DrawPrimitive_TriangleTexTestBlend(
	BGBRASW_Context *ctx,
	BGBRASW_Primitive *prim, int flag)
{
	bgbrasw_pixel clr;
	bgbrasw_pixel clr0, clr1, clr2, clr3;
	bgbrasw_pixel clr4, clr5, clr6, clr7;
	int cra, crb, crav, crbv;
	int cga, cgb, cgav, cgbv;
	int cba, cbb, cbav, cbbv;
	int caa, cab, caav, cabv;

	BGBRASW_DrawSpanTex_ft drawSpan;
	bgbrasw_pixel *tergba;
	int xs, ys, yc, ys1, ye0, ye1, ye2;
	int xc0, xc1, xc0v, xc1v;
	int x0, x1, x2, y0, y1, y2;
	bgbrasw_zbuf z0, z1, z2;
	int v0, v1, v2;
	int stsa, stsb, stsav, stsbv;
	int stta, sttb, sttav, sttbv;
	int st0s, st0t, st1s, st1t;
	int st2s, st2t, st3s, st3t;
	int sts0, stt0, sts1, stt1;
	int zsa, zsb, zsav, zsbv;
	int txs, tys, mip;
//	int s0, s1, s2;
	int t0, t1, t2;
	int xa, xb;
	int i, j, k;

	if(flag&1)	{ v0=0; v1=2; v2=3; }
	else		{ v0=0; v1=1; v2=2; }

	y0=prim->xy[v0][1];
	y1=prim->xy[v1][1];
	y2=prim->xy[v2][1];
	if(y0<y1)
	{
		//012, 021, 201
		if(y0<y2)
		{
			//Bot=Y0
			if(y1<y2)
			{	x0=prim->xy[v0][0];		y0=prim->xy[v0][1];
				x1=prim->xy[v1][0];		y1=prim->xy[v1][1];
				x2=prim->xy[v2][0];		y2=prim->xy[v2][1];
				st0s=prim->st[v0][0];	st0t=prim->st[v0][1];
				st1s=prim->st[v1][0];	st1t=prim->st[v1][1];
				st2s=prim->st[v2][0];	st2t=prim->st[v2][1];
				clr0=prim->rgba[v0];	clr1=prim->rgba[v1];
				clr2=prim->rgba[v2];	z0=prim->z[v0];
				z1=prim->z[v1];			z2=prim->z[v2];
			}
			else
			{	x0=prim->xy[v0][0];		y0=prim->xy[v0][1];
				x1=prim->xy[v2][0];		y1=prim->xy[v2][1];
				x2=prim->xy[v1][0];		y2=prim->xy[v1][1];
				st0s=prim->st[v0][0];	st0t=prim->st[v0][1];
				st1s=prim->st[v2][0];	st1t=prim->st[v2][1];
				st2s=prim->st[v1][0];	st2t=prim->st[v1][1];
				clr0=prim->rgba[v0];	clr1=prim->rgba[v2];
				clr2=prim->rgba[v1];	z0=prim->z[v0];
				z1=prim->z[v2];			z2=prim->z[v1];
			}
		}else
			{	x0=prim->xy[v2][0];		y0=prim->xy[v2][1];
				x1=prim->xy[v0][0];		y1=prim->xy[v0][1];
				x2=prim->xy[v1][0];		y2=prim->xy[v1][1];
				st0s=prim->st[v2][0];	st0t=prim->st[v2][1];
				st1s=prim->st[v0][0];	st1t=prim->st[v0][1];
				st2s=prim->st[v1][0];	st2t=prim->st[v1][1];
				clr0=prim->rgba[v2];	clr1=prim->rgba[v0];
				clr2=prim->rgba[v1];	z0=prim->z[v2];
				z1=prim->z[v0];			z2=prim->z[v1];
			}
	}else
	{
		//102, 120, 210
		if(y1<y2)
		{
			//Bot=Y1
			if(y0<y2)
			{	x0=prim->xy[v1][0];		y0=prim->xy[v1][1];
				x1=prim->xy[v0][0];		y1=prim->xy[v0][1];
				x2=prim->xy[v2][0];		y2=prim->xy[v2][1];
				st0s=prim->st[v1][0];	st0t=prim->st[v1][1];
				st1s=prim->st[v0][0];	st1t=prim->st[v0][1];
				st2s=prim->st[v2][0];	st2t=prim->st[v2][1];
				clr0=prim->rgba[v1];	clr1=prim->rgba[v0];
				clr2=prim->rgba[v2];	z0=prim->z[v1];
				z1=prim->z[v0];			z2=prim->z[v2];
			}
			else
			{	x0=prim->xy[v1][0];		y0=prim->xy[v1][1];
				x1=prim->xy[v2][0];		y1=prim->xy[v2][1];
				x2=prim->xy[v0][0];		y2=prim->xy[v0][1];
				st0s=prim->st[v1][0];	st0t=prim->st[v1][1];
				st1s=prim->st[v2][0];	st1t=prim->st[v2][1];
				st2s=prim->st[v0][0];	st2t=prim->st[v0][1];
				clr0=prim->rgba[v1];	clr1=prim->rgba[v2];
				clr2=prim->rgba[v0];	z0=prim->z[v1];
				z1=prim->z[v2];			z2=prim->z[v0];
			}
		}else
			{	x0=prim->xy[v2][0];		y0=prim->xy[v2][1];
				x1=prim->xy[v1][0];		y1=prim->xy[v1][1];
				x2=prim->xy[v0][0];		y2=prim->xy[v0][1];
				st0s=prim->st[v2][0];	st0t=prim->st[v2][1];
				st1s=prim->st[v1][0];	st1t=prim->st[v1][1];
				st2s=prim->st[v0][0];	st2t=prim->st[v0][1];
				clr0=prim->rgba[v2];	clr1=prim->rgba[v1];
				clr2=prim->rgba[v0];	z0=prim->z[v2];
				z1=prim->z[v1];			z2=prim->z[v0];
			}
	}

	xs=max(x2-x1, max(x2-x0, x1-x0));
	ys=max(y2-y1, max(y2-y0, y1-y0));
	t0=max(st2s-st1s, max(st2s-st0s, st1s-st0s));
	t1=max(st2t-st1t, max(st2t-st0t, st1t-st0t));
	xs=xs>>XY_SHR2;	ys=ys>>XY_SHR2;
#ifdef ST_MIPSHR
	t0=t0>>ST_MIPSHR;
	t1=t1>>ST_MIPSHR;
#endif
//	j=(abs(st1s-st0s)+abs(st1t-st0t))/(xs+ys);
//	mip=bgbrasw_log2dn(j);
//	i0=st1s-st0s; i1=st1t-st0t;
	j=(t0*t0+t1*t1)/(xs*xs+ys*ys+1);
//	mip=(bgbrasw_log2dn(j)+1)/2;
	k=bgbrasw_log2f8(j);
	mip=((k>>8)+1)/2;
//	mip=1;

	prim->tabs->tmp_tex=prim->tex;
	prim->tabs->tmp_fracmip=k;

	if(k>256)
		{ drawSpan=prim->tabs->drawSpanTex_min; }
	else if(k>64)
		{ drawSpan=prim->tabs->drawSpanTex_mag; }
	else
		{ drawSpan=prim->tabs->drawSpanTex_mag2; }

	tergba=prim->tex->rgba[mip];
	txs=prim->tex->xs>>mip;
	tys=prim->tex->ys>>mip;
	if(!txs)txs=1;
	if(!tys)tys=1;

	if(!tergba)
	{
		mip=0;
		tergba=prim->tex->rgba[0];
		txs=prim->tex->xs;
		tys=prim->tex->ys;
	}

	ye0=y0>>XY_SHR2;
	ye1=y1>>XY_SHR2;
	ye2=y2>>XY_SHR2;

	if(ye1>ye0)
	{
		ys=ye1-ye0;
		ys1=ye2-ye0;
		xc0=x0<<XY_SHL;		xc1=x0<<XY_SHL;
		xc0v=((x1-x0)<<XY_SHL)/ys;
		xc1v=((x2-x0)<<XY_SHL)/ys1;

		stsa=(st0s<<ST_SHL);		stta=(st0t<<ST_SHL);
		stsb=stsa;			sttb=stta;
		zsa=z0;				zsb=z0;

		cra=BGBRASW_PIXEL_R(clr0)<<16;	cga=BGBRASW_PIXEL_G(clr0)<<16;
		cba=BGBRASW_PIXEL_B(clr0)<<16;	caa=BGBRASW_PIXEL_A(clr0)<<16;
		crb=cra;	cgb=cga;	cbb=cba;	cab=caa;

		if(xc1v<xc0v)
		{
			i=xc0v; xc0v=xc1v; xc1v=i;

			stsav=((st2s-st0s)<<ST_SHL)/ys1;
			sttav=((st2t-st0t)<<ST_SHL)/ys1;
			stsbv=((st1s-st0s)<<ST_SHL)/ys;
			sttbv=((st1t-st0t)<<ST_SHL)/ys;

			zsav=((s32)(z2-z0))/ys1;
			zsbv=((s32)(z1-z0))/ys;
			zsav&=BGBRASW_MASK_DEPTH;
			zsbv&=BGBRASW_MASK_DEPTH;

			crav=((BGBRASW_PIXEL_R(clr2)-BGBRASW_PIXEL_R(clr0))<<16)/ys1;
			cgav=((BGBRASW_PIXEL_G(clr2)-BGBRASW_PIXEL_G(clr0))<<16)/ys1;
			cbav=((BGBRASW_PIXEL_B(clr2)-BGBRASW_PIXEL_B(clr0))<<16)/ys1;
			caav=((BGBRASW_PIXEL_A(clr2)-BGBRASW_PIXEL_A(clr0))<<16)/ys1;
			crbv=((BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<16)/ys;
			cgbv=((BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<16)/ys;
			cbbv=((BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<16)/ys;
			cabv=((BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<16)/ys;
		}else
		{
			stsav=((st1s-st0s)<<ST_SHL)/ys;
			sttav=((st1t-st0t)<<ST_SHL)/ys;
			stsbv=((st2s-st0s)<<ST_SHL)/ys1;
			sttbv=((st2t-st0t)<<ST_SHL)/ys1;

			zsav=((s32)(z1-z0))/ys;
			zsbv=((s32)(z2-z0))/ys1;
			zsav&=BGBRASW_MASK_DEPTH;
			zsbv&=BGBRASW_MASK_DEPTH;

			crav=((BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<16)/ys;
			cgav=((BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<16)/ys;
			cbav=((BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<16)/ys;
			caav=((BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<16)/ys;
			crbv=((BGBRASW_PIXEL_R(clr2)-BGBRASW_PIXEL_R(clr0))<<16)/ys1;
			cgbv=((BGBRASW_PIXEL_G(clr2)-BGBRASW_PIXEL_G(clr0))<<16)/ys1;
			cbbv=((BGBRASW_PIXEL_B(clr2)-BGBRASW_PIXEL_B(clr0))<<16)/ys1;
			cabv=((BGBRASW_PIXEL_A(clr2)-BGBRASW_PIXEL_A(clr0))<<16)/ys1;
		}

		yc=ye0;
		while(yc<ye1)
		{
			clr4=BGBRASW_MAKEPIXEL(cra>>16, cga>>16, cba>>16, caa>>16);
			clr5=BGBRASW_MAKEPIXEL(crb>>16, cgb>>16, cbb>>16, cab>>16);
			cra+=crav;	cga+=cgav;		cba+=cbav;	caa+=caav;
			crb+=crbv;	cgb+=cgbv;		cbb+=cbbv;	cab+=cabv;

			sts0=stsa>>(ST_SHR+mip);		stt0=stta>>(ST_SHR+mip);
			sts1=stsb>>(ST_SHR+mip);		stt1=sttb>>(ST_SHR+mip);

			xa=xc0>>XY_SHR; xb=xc1>>XY_SHR;
//			xa=(xc0+128)>>XY_SHR; xb=(xc1+128)>>XY_SHR;
//			xa=(xc0+ 64)>>XY_SHR; xb=(xc1+192)>>XY_SHR;
//			BGBRASW_DrawSpanTextureInterpTestBlend(
//			prim->tabs->drawSpanTex(prim->tabs,
			drawSpan(prim->tabs,
				ctx->tgt_rgba+yc*ctx->tgt_xs+xa,
				ctx->tgt_zbuf+yc*ctx->tgt_xs+xa,
				xb-xa,
				tergba, txs, tys,
				sts0, stt0, sts1, stt1,
				clr4, clr5, zsa, zsb);

			zsa+=zsav;		zsb+=zsbv;
			stsa+=stsav;	stta+=sttav;
			stsb+=stsbv;	sttb+=sttbv;
			xc0+=xc0v; xc1+=xc1v; yc++;
		}
	}else
	{
		cra=BGBRASW_PIXEL_R(clr0)<<16;	cga=BGBRASW_PIXEL_G(clr0)<<16;
		cba=BGBRASW_PIXEL_B(clr0)<<16;	caa=BGBRASW_PIXEL_A(clr0)<<16;
		crb=cra;	cgb=cga;	cbb=cba;	cab=caa;

		stsa=(st0s<<ST_SHL);		stta=(st0t<<ST_SHL);
		stsb=stsa;			sttb=stta;
		zsa=z0;				zsb=z0;

		xc0=x0<<XY_SHL;
		xc1=x0<<XY_SHL;
		if(x1<x0)
		{
			cra=BGBRASW_PIXEL_R(clr1)<<16;	cga=BGBRASW_PIXEL_G(clr1)<<16;
			cba=BGBRASW_PIXEL_B(clr1)<<16;	caa=BGBRASW_PIXEL_A(clr1)<<16;
			stsa=(st1s<<ST_SHL);		stta=(st1t<<ST_SHL);
			zsa=z1;
			xc0=x1<<XY_SHL;
		}
		if(x1>x0)
		{
			crb=BGBRASW_PIXEL_R(clr1)<<16;	cgb=BGBRASW_PIXEL_G(clr1)<<16;
			cbb=BGBRASW_PIXEL_B(clr1)<<16;	cab=BGBRASW_PIXEL_A(clr1)<<16;
			stsb=(st1s<<ST_SHL);		sttb=(st1t<<ST_SHL);
			zsb=z1;
			xc1=x1<<XY_SHL;
		}
	}

	if(ye2>ye1)
	{
		ys=ye2-ye1;
		xa=xc0>>XY_SHL; xb=xc1>>XY_SHL;
		if(x1<xa) { xa=x1; xc0=x1<<XY_SHL; }
		if(x1>xb) { xb=x1; xc1=x1<<XY_SHL; }
		xc0v=((x2-xa)<<XY_SHL)/ys;
		xc1v=((x2-xb)<<XY_SHL)/ys;

//		sts0=stsa>>(8+mip);		stt0=stta>>(8+mip);
//		sts1=stsb>>(8+mip);		stt1=sttb>>(8+mip);
		sts0=stsa>>ST_SHL;		stt0=stta>>ST_SHL;
		sts1=stsb>>ST_SHL;		stt1=sttb>>ST_SHL;
		stsav=((st2s-sts0)<<ST_SHL)/ys;
		sttav=((st2t-stt0)<<ST_SHL)/ys;
		stsbv=((st2s-sts1)<<ST_SHL)/ys;
		sttbv=((st2t-stt1)<<ST_SHL)/ys;

		zsav=((s32)(z2-zsa))/ys;
		zsbv=((s32)(z2-zsb))/ys;
		zsav&=BGBRASW_MASK_DEPTH;
		zsbv&=BGBRASW_MASK_DEPTH;

		clr4=BGBRASW_MAKEPIXEL(cra>>16, cga>>16, cba>>16, caa>>16);
		clr5=BGBRASW_MAKEPIXEL(crb>>16, cgb>>16, cbb>>16, cab>>16);
		crav=((BGBRASW_PIXEL_R(clr2)-BGBRASW_PIXEL_R(clr4))<<16)/ys;
		cgav=((BGBRASW_PIXEL_G(clr2)-BGBRASW_PIXEL_G(clr4))<<16)/ys;
		cbav=((BGBRASW_PIXEL_B(clr2)-BGBRASW_PIXEL_B(clr4))<<16)/ys;
		caav=((BGBRASW_PIXEL_A(clr2)-BGBRASW_PIXEL_A(clr4))<<16)/ys;
		crbv=((BGBRASW_PIXEL_R(clr2)-BGBRASW_PIXEL_R(clr5))<<16)/ys;
		cgbv=((BGBRASW_PIXEL_G(clr2)-BGBRASW_PIXEL_G(clr5))<<16)/ys;
		cbbv=((BGBRASW_PIXEL_B(clr2)-BGBRASW_PIXEL_B(clr5))<<16)/ys;
		cabv=((BGBRASW_PIXEL_A(clr2)-BGBRASW_PIXEL_A(clr5))<<16)/ys;

		yc=ye1;
		while(yc<ye2)
		{
			sts0=stsa>>(ST_SHR+mip);		stt0=stta>>(ST_SHR+mip);
			sts1=stsb>>(ST_SHR+mip);		stt1=sttb>>(ST_SHR+mip);

			clr4=BGBRASW_MAKEPIXEL(cra>>16, cga>>16, cba>>16, caa>>16);
			clr5=BGBRASW_MAKEPIXEL(crb>>16, cgb>>16, cbb>>16, cab>>16);
			cra+=crav;	cga+=cgav;		cba+=cbav;	caa+=caav;
			crb+=crbv;	cgb+=cgbv;		cbb+=cbbv;	cab+=cabv;

			xa=xc0>>XY_SHR; xb=xc1>>XY_SHR;
//			xa=(xc0+128)>>XY_SHR; xb=(xc1+128)>>XY_SHR;
//			xa=(xc0+ 64)>>XY_SHR; xb=(xc1+192)>>XY_SHR;
//			prim->tabs->drawSpanTex(prim->tabs,
			drawSpan(prim->tabs,
				ctx->tgt_rgba+yc*ctx->tgt_xs+xa,
				ctx->tgt_zbuf+yc*ctx->tgt_xs+xa,
				xb-xa,
				tergba, txs, tys,
				sts0, stt0, sts1, stt1,
				clr4, clr5, zsa, zsb);

			zsa+=zsav;		zsb+=zsbv;
			stsa+=stsav;	stta+=sttav;
			stsb+=stsbv;	sttb+=sttbv;
			xc0+=xc0v; xc1+=xc1v; yc++;
		}
	}
}
