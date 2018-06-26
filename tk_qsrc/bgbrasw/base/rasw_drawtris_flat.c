#include <bgbrasw.h>

#ifdef BGBRASW_SS_YUVA
#define tgt_rgba	tgt_ssyuva
#define tgt_zbuf	tgt_sszbuf
#endif

// #define XY_SHL	8
#define XY_SHR	8
// #define XY_SHR2	0

#define XY_SHL	0
#define XY_SHR2	8

void BGBRASW_DrawPrimitive_TriangleFlatBasic(
	BGBRASW_Context *ctx,
	BGBRASW_Primitive *prim, int flag)
{
	bgbrasw_pixel clr;
	int xs, ys, ys1, yc, ye0, ye1, ye2;
	int xc0, xc1, xc0v, xc1v;
	int x0, x1, x2, y0, y1, y2;
	int v0, v1, v2;
//	int s0, s1, s2, t0, t1, t2;
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
				x2=prim->xy[v2][0];		y2=prim->xy[v2][1];		}
			else
			{	x0=prim->xy[v0][0];		y0=prim->xy[v0][1];
				x1=prim->xy[v2][0];		y1=prim->xy[v2][1];
				x2=prim->xy[v1][0];		y2=prim->xy[v1][1];		}
		}else
			{	x0=prim->xy[v2][0];		y0=prim->xy[v2][1];
				x1=prim->xy[v0][0];		y1=prim->xy[v0][1];
				x2=prim->xy[v1][0];		y2=prim->xy[v1][1];		}
	}else
	{
		//102, 120, 210
		if(y1<y2)
		{
			//Bot=Y1
			if(y0<y2)
			{	x0=prim->xy[v1][0];		y0=prim->xy[v1][1];
				x1=prim->xy[v0][0];		y1=prim->xy[v0][1];
				x2=prim->xy[v2][0];		y2=prim->xy[v2][1];		}
			else
			{	x0=prim->xy[v1][0];		y0=prim->xy[v1][1];
				x1=prim->xy[v2][0];		y1=prim->xy[v2][1];
				x2=prim->xy[v0][0];		y2=prim->xy[v0][1];		}
		}else
			{	x0=prim->xy[v2][0];		y0=prim->xy[v2][1];
				x1=prim->xy[v1][0];		y1=prim->xy[v1][1];
				x2=prim->xy[v0][0];		y2=prim->xy[v0][1];		}
	}

	clr=prim->rgba[0];
//	xs=x1-x0;
//	ys=ye1-ye0;

	ye0=y0>>XY_SHR2;
	ye1=y1>>XY_SHR2;
	ye2=y2>>XY_SHR2;

//	if(ys>0)
	if(ye1>ye0)
	{
		ys=ye1-ye0;
		ys1=ye2-ye0;
		xc0=x0<<XY_SHL;
		xc1=x0<<XY_SHL;
		xc0v=((x1-x0)<<XY_SHL)/ys;
		xc1v=((x2-x0)<<XY_SHL)/ys1;

		if(xc1v<xc0v)
			{ i=xc0v; xc0v=xc1v; xc1v=i; }

		yc=ye0;
		while(yc<ye1)
		{
			xa=xc0>>XY_SHR; xb=xc1>>XY_SHR;
			prim->tabs->drawSpanFlatBasic(prim->tabs,
				ctx->tgt_rgba+yc*ctx->tgt_xs+xa, xb-xa, clr);
			xc0+=xc0v; xc1+=xc1v; yc++;
		}
	}else
	{
		xc0=x0<<XY_SHL;
		xc1=x0<<XY_SHL;
		if(x1<x0) { xc0=x1<<XY_SHL; }
		if(x1>x0) { xc1=x1<<XY_SHL; }
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

		yc=ye1;
		while(yc<ye2)
		{
			xa=xc0>>XY_SHR; xb=xc1>>XY_SHR;
			prim->tabs->drawSpanFlatBasic(prim->tabs,
				ctx->tgt_rgba+yc*ctx->tgt_xs+xa, xb-xa, clr);
			xc0+=xc0v; xc1+=xc1v; yc++;
		}
	}
}

void BGBRASW_DrawPrimitive_TriangleFlatInterp(
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

	int xs, ys, ys1, yc, ye0, ye1, ye2;
	int xc0, xc1, xc0v, xc1v;
	int x0, x1, x2, y0, y1, y2;
	int v0, v1, v2;
//	int s0, s1, s2, t0, t1, t2;
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
				clr0=prim->rgba[v0];	clr1=prim->rgba[v1];
				clr2=prim->rgba[v2];
			}
			else
			{	x0=prim->xy[v0][0];		y0=prim->xy[v0][1];
				x1=prim->xy[v2][0];		y1=prim->xy[v2][1];
				x2=prim->xy[v1][0];		y2=prim->xy[v1][1];
				clr0=prim->rgba[v0];	clr1=prim->rgba[v2];
				clr2=prim->rgba[v1];
			}
		}else
			{	x0=prim->xy[v2][0];		y0=prim->xy[v2][1];
				x1=prim->xy[v0][0];		y1=prim->xy[v0][1];
				x2=prim->xy[v1][0];		y2=prim->xy[v1][1];
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
				clr0=prim->rgba[v1];	clr1=prim->rgba[v0];
				clr2=prim->rgba[v2];
			}
			else
			{	x0=prim->xy[v1][0];		y0=prim->xy[v1][1];
				x1=prim->xy[v2][0];		y1=prim->xy[v2][1];
				x2=prim->xy[v0][0];		y2=prim->xy[v0][1];
				clr0=prim->rgba[v1];	clr1=prim->rgba[v2];
				clr2=prim->rgba[v0];
			}
		}else
			{	x0=prim->xy[v2][0];		y0=prim->xy[v2][1];
				x1=prim->xy[v1][0];		y1=prim->xy[v1][1];
				x2=prim->xy[v0][0];		y2=prim->xy[v0][1];
				clr0=prim->rgba[v2];	clr1=prim->rgba[v1];
				clr2=prim->rgba[v0];
			}
	}

	clr=prim->rgba[0];
//	xs=x1-x0;
//	ys=ye1-ye0;

	ye0=y0>>XY_SHR2;
	ye1=y1>>XY_SHR2;
	ye2=y2>>XY_SHR2;

//	if(ys>0)
	if(ye1>ye0)
	{
		ys=ye1-ye0;
		ys1=ye2-ye0;
		xc0=x0<<XY_SHL;
		xc1=x0<<XY_SHL;
		xc0v=((x1-x0)<<XY_SHL)/ys;
		xc1v=((x2-x0)<<XY_SHL)/ys1;

		cra=BGBRASW_PIXEL_R(clr0)<<16;	cga=BGBRASW_PIXEL_G(clr0)<<16;
		cba=BGBRASW_PIXEL_B(clr0)<<16;	caa=BGBRASW_PIXEL_A(clr0)<<16;
		crb=cra;	cgb=cga;	cbb=cba;	cab=caa;

		if(xc1v<xc0v)
		{
			i=xc0v; xc0v=xc1v; xc1v=i;

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

			xa=xc0>>XY_SHR; xb=xc1>>XY_SHR;
			prim->tabs->drawSpanFlatInterp(prim->tabs,
				ctx->tgt_rgba+yc*ctx->tgt_xs+xa, xb-xa, clr4, clr5);
			xc0+=xc0v; xc1+=xc1v; yc++;
		}
	}else
	{
		cra=BGBRASW_PIXEL_R(clr0)<<16;	cga=BGBRASW_PIXEL_G(clr0)<<16;
		cba=BGBRASW_PIXEL_B(clr0)<<16;	caa=BGBRASW_PIXEL_A(clr0)<<16;
		crb=cra;	cgb=cga;	cbb=cba;	cab=caa;

		xc0=x0<<XY_SHL;
		xc1=x0<<XY_SHL;
		if(x1<x0)
		{
			xc0=x1<<XY_SHL;
			cra=BGBRASW_PIXEL_R(clr1)<<16;	cga=BGBRASW_PIXEL_G(clr1)<<16;
			cba=BGBRASW_PIXEL_B(clr1)<<16;	caa=BGBRASW_PIXEL_A(clr1)<<16;
		}
		if(x1>x0)
		{
			xc1=x1<<XY_SHL;
			crb=BGBRASW_PIXEL_R(clr1)<<16;	cgb=BGBRASW_PIXEL_G(clr1)<<16;
			cbb=BGBRASW_PIXEL_B(clr1)<<16;	cab=BGBRASW_PIXEL_A(clr1)<<16;
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
			clr4=BGBRASW_MAKEPIXEL(cra>>16, cga>>16, cba>>16, caa>>16);
			clr5=BGBRASW_MAKEPIXEL(crb>>16, cgb>>16, cbb>>16, cab>>16);
			cra+=crav;	cga+=cgav;		cba+=cbav;	caa+=caav;
			crb+=crbv;	cgb+=cgbv;		cbb+=cbbv;	cab+=cabv;

			xa=xc0>>XY_SHR; xb=xc1>>XY_SHR;
			prim->tabs->drawSpanFlatInterp(prim->tabs,
				ctx->tgt_rgba+yc*ctx->tgt_xs+xa, xb-xa, clr4, clr5);
			xc0+=xc0v; xc1+=xc1v; yc++;
		}
	}
}

void BGBRASW_DrawPrimitive_TriangleFlatBasicZTest(
	BGBRASW_Context *ctx,
	BGBRASW_Primitive *prim, int flag)
{
	bgbrasw_pixel clr;
	bgbrasw_pixel clr0, clr1, clr2, clr3;
	bgbrasw_pixel clr4, clr5, clr6, clr7;
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
		zsa=z0;		zsb=z0;

		if(xc1v<xc0v)
		{
			i=xc0v; xc0v=xc1v; xc1v=i;

			zsav=((s32)(z2-z0))/ys1;
			zsbv=((s32)(z1-z0))/ys;
			zsav&=BGBRASW_MASK_DEPTH;
			zsbv&=BGBRASW_MASK_DEPTH;
		}else
		{
			zsav=((s32)(z1-z0))/ys;
			zsbv=((s32)(z2-z0))/ys1;
			zsav&=BGBRASW_MASK_DEPTH;
			zsbv&=BGBRASW_MASK_DEPTH;
		}

		yc=ye0;
		while(yc<ye1)
		{
			xa=xc0>>XY_SHR; xb=xc1>>XY_SHR;
			prim->tabs->drawSpanFlatBasicZTest(prim->tabs,
				ctx->tgt_rgba+yc*ctx->tgt_xs+xa,
				ctx->tgt_zbuf+yc*ctx->tgt_xs+xa,
				xb-xa, clr0, zsa, zsb);
			zsa+=zsav;		zsb+=zsbv;
			xc0+=xc0v; xc1+=xc1v; yc++;
		}
	}else
	{
		zsa=z0;			zsb=z0;
		xc0=x0<<XY_SHL;		xc1=x0<<XY_SHL;
		if(x1<x0)
		{
			zsa=z1;
			xc0=x1<<XY_SHL;
		}
		if(x1>x0)
		{
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

		zsav=((s32)(z2-zsa))/ys;
		zsbv=((s32)(z2-zsb))/ys;
		zsav&=BGBRASW_MASK_DEPTH;
		zsbv&=BGBRASW_MASK_DEPTH;

		yc=ye1;
		while(yc<ye2)
		{
			xa=xc0>>XY_SHR; xb=xc1>>XY_SHR;
			prim->tabs->drawSpanFlatBasicZTest(prim->tabs,
				ctx->tgt_rgba+yc*ctx->tgt_xs+xa,
				ctx->tgt_zbuf+yc*ctx->tgt_xs+xa,
				xb-xa,
				clr0, zsa, zsb);

			zsa+=zsav;		zsb+=zsbv;
//			stsa+=stsav;	stta+=sttav;
//			stsb+=stsbv;	sttb+=sttbv;
			xc0+=xc0v; xc1+=xc1v; yc++;
		}
	}
}

void BGBRASW_DrawPrimitive_TriangleFlatInterpZTest(
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
				clr0=prim->rgba[v0];	clr1=prim->rgba[v1];
				clr2=prim->rgba[v2];	z0=prim->z[v0];
				z1=prim->z[v1];			z2=prim->z[v2];
			}
			else
			{	x0=prim->xy[v0][0];		y0=prim->xy[v0][1];
				x1=prim->xy[v2][0];		y1=prim->xy[v2][1];
				x2=prim->xy[v1][0];		y2=prim->xy[v1][1];
				clr0=prim->rgba[v0];	clr1=prim->rgba[v2];
				clr2=prim->rgba[v1];	z0=prim->z[v0];
				z1=prim->z[v2];			z2=prim->z[v1];
			}
		}else
			{	x0=prim->xy[v2][0];		y0=prim->xy[v2][1];
				x1=prim->xy[v0][0];		y1=prim->xy[v0][1];
				x2=prim->xy[v1][0];		y2=prim->xy[v1][1];
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
				clr0=prim->rgba[v1];	clr1=prim->rgba[v0];
				clr2=prim->rgba[v2];	z0=prim->z[v1];
				z1=prim->z[v0];			z2=prim->z[v2];
			}
			else
			{	x0=prim->xy[v1][0];		y0=prim->xy[v1][1];
				x1=prim->xy[v2][0];		y1=prim->xy[v2][1];
				x2=prim->xy[v0][0];		y2=prim->xy[v0][1];
				clr0=prim->rgba[v1];	clr1=prim->rgba[v2];
				clr2=prim->rgba[v0];	z0=prim->z[v1];
				z1=prim->z[v2];			z2=prim->z[v0];
			}
		}else
			{	x0=prim->xy[v2][0];		y0=prim->xy[v2][1];
				x1=prim->xy[v1][0];		y1=prim->xy[v1][1];
				x2=prim->xy[v0][0];		y2=prim->xy[v0][1];
				clr0=prim->rgba[v2];	clr1=prim->rgba[v1];
				clr2=prim->rgba[v0];	z0=prim->z[v2];
				z1=prim->z[v1];			z2=prim->z[v0];
			}
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
		zsa=z0;				zsb=z0;

		cra=BGBRASW_PIXEL_R(clr0)<<16;	cga=BGBRASW_PIXEL_G(clr0)<<16;
		cba=BGBRASW_PIXEL_B(clr0)<<16;	caa=BGBRASW_PIXEL_A(clr0)<<16;
		crb=cra;	cgb=cga;	cbb=cba;	cab=caa;

		if(xc1v<xc0v)
		{
			i=xc0v; xc0v=xc1v; xc1v=i;

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

//			sts0=stsa>>mip;		stt0=stta>>mip;
//			sts1=stsb>>mip;		stt1=sttb>>mip;

			xa=xc0>>XY_SHR; xb=xc1>>XY_SHR;
			prim->tabs->drawSpanFlatInterpZTest(prim->tabs,
				ctx->tgt_rgba+yc*ctx->tgt_xs+xa,
				ctx->tgt_zbuf+yc*ctx->tgt_xs+xa,
				xb-xa, clr4, clr5, zsa, zsb);

			zsa+=zsav;		zsb+=zsbv;
			xc0+=xc0v; xc1+=xc1v; yc++;
		}
	}else
	{
		cra=BGBRASW_PIXEL_R(clr0)<<16;	cga=BGBRASW_PIXEL_G(clr0)<<16;
		cba=BGBRASW_PIXEL_B(clr0)<<16;	caa=BGBRASW_PIXEL_A(clr0)<<16;
		crb=cra;	cgb=cga;	cbb=cba;	cab=caa;

		zsa=z0;				zsb=z0;

		xc0=x0<<XY_SHL;
		xc1=x0<<XY_SHL;
		if(x1<x0)
		{
			cra=BGBRASW_PIXEL_R(clr1)<<16;	cga=BGBRASW_PIXEL_G(clr1)<<16;
			cba=BGBRASW_PIXEL_B(clr1)<<16;	caa=BGBRASW_PIXEL_A(clr1)<<16;
			zsa=z1;
			xc0=x1<<XY_SHL;
		}
		if(x1>x0)
		{
			crb=BGBRASW_PIXEL_R(clr1)<<16;	cgb=BGBRASW_PIXEL_G(clr1)<<16;
			cbb=BGBRASW_PIXEL_B(clr1)<<16;	cab=BGBRASW_PIXEL_A(clr1)<<16;
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
			clr4=BGBRASW_MAKEPIXEL(cra>>16, cga>>16, cba>>16, caa>>16);
			clr5=BGBRASW_MAKEPIXEL(crb>>16, cgb>>16, cbb>>16, cab>>16);
			cra+=crav;	cga+=cgav;		cba+=cbav;	caa+=caav;
			crb+=crbv;	cgb+=cgbv;		cbb+=cbbv;	cab+=cabv;

			xa=xc0>>XY_SHR; xb=xc1>>XY_SHR;
			prim->tabs->drawSpanFlatInterpZTest(prim->tabs,
				ctx->tgt_rgba+yc*ctx->tgt_xs+xa,
				ctx->tgt_zbuf+yc*ctx->tgt_xs+xa,
				xb-xa, clr4, clr5, zsa, zsb);

			zsa+=zsav;		zsb+=zsbv;
			xc0+=xc0v; xc1+=xc1v; yc++;
		}
	}
}

void BGBRASW_DrawPrimitive_TriangleFlatTestBlend(
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
				clr0=prim->rgba[v0];	clr1=prim->rgba[v1];
				clr2=prim->rgba[v2];	z0=prim->z[v0];
				z1=prim->z[v1];			z2=prim->z[v2];
			}
			else
			{	x0=prim->xy[v0][0];		y0=prim->xy[v0][1];
				x1=prim->xy[v2][0];		y1=prim->xy[v2][1];
				x2=prim->xy[v1][0];		y2=prim->xy[v1][1];
				clr0=prim->rgba[v0];	clr1=prim->rgba[v2];
				clr2=prim->rgba[v1];	z0=prim->z[v0];
				z1=prim->z[v2];			z2=prim->z[v1];
			}
		}else
			{	x0=prim->xy[v2][0];		y0=prim->xy[v2][1];
				x1=prim->xy[v0][0];		y1=prim->xy[v0][1];
				x2=prim->xy[v1][0];		y2=prim->xy[v1][1];
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
				clr0=prim->rgba[v1];	clr1=prim->rgba[v0];
				clr2=prim->rgba[v2];	z0=prim->z[v1];
				z1=prim->z[v0];			z2=prim->z[v2];
			}
			else
			{	x0=prim->xy[v1][0];		y0=prim->xy[v1][1];
				x1=prim->xy[v2][0];		y1=prim->xy[v2][1];
				x2=prim->xy[v0][0];		y2=prim->xy[v0][1];
				clr0=prim->rgba[v1];	clr1=prim->rgba[v2];
				clr2=prim->rgba[v0];	z0=prim->z[v1];
				z1=prim->z[v2];			z2=prim->z[v0];
			}
		}else
			{	x0=prim->xy[v2][0];		y0=prim->xy[v2][1];
				x1=prim->xy[v1][0];		y1=prim->xy[v1][1];
				x2=prim->xy[v0][0];		y2=prim->xy[v0][1];
				clr0=prim->rgba[v2];	clr1=prim->rgba[v1];
				clr2=prim->rgba[v0];	z0=prim->z[v2];
				z1=prim->z[v1];			z2=prim->z[v0];
			}
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
		zsa=z0;				zsb=z0;

		cra=BGBRASW_PIXEL_R(clr0)<<16;	cga=BGBRASW_PIXEL_G(clr0)<<16;
		cba=BGBRASW_PIXEL_B(clr0)<<16;	caa=BGBRASW_PIXEL_A(clr0)<<16;
		crb=cra;	cgb=cga;	cbb=cba;	cab=caa;

		if(xc1v<xc0v)
		{
			i=xc0v; xc0v=xc1v; xc1v=i;

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

//			sts0=stsa>>mip;		stt0=stta>>mip;
//			sts1=stsb>>mip;		stt1=sttb>>mip;

			xa=xc0>>XY_SHR; xb=xc1>>XY_SHR;
			prim->tabs->drawSpanFlat(prim->tabs,
				ctx->tgt_rgba+yc*ctx->tgt_xs+xa,
				ctx->tgt_zbuf+yc*ctx->tgt_xs+xa,
				xb-xa, clr4, clr5, zsa, zsb);

			zsa+=zsav;		zsb+=zsbv;
			xc0+=xc0v; xc1+=xc1v; yc++;
		}
	}else
	{
		cra=BGBRASW_PIXEL_R(clr0)<<16;	cga=BGBRASW_PIXEL_G(clr0)<<16;
		cba=BGBRASW_PIXEL_B(clr0)<<16;	caa=BGBRASW_PIXEL_A(clr0)<<16;
		crb=cra;	cgb=cga;	cbb=cba;	cab=caa;

		zsa=z0;				zsb=z0;

		xc0=x0<<XY_SHL;
		xc1=x0<<XY_SHL;
		if(x1<x0)
		{
			cra=BGBRASW_PIXEL_R(clr1)<<16;	cga=BGBRASW_PIXEL_G(clr1)<<16;
			cba=BGBRASW_PIXEL_B(clr1)<<16;	caa=BGBRASW_PIXEL_A(clr1)<<16;
			zsa=z1;
			xc0=x1<<XY_SHL;
		}
		if(x1>x0)
		{
			crb=BGBRASW_PIXEL_R(clr1)<<16;	cgb=BGBRASW_PIXEL_G(clr1)<<16;
			cbb=BGBRASW_PIXEL_B(clr1)<<16;	cab=BGBRASW_PIXEL_A(clr1)<<16;
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
			clr4=BGBRASW_MAKEPIXEL(cra>>16, cga>>16, cba>>16, caa>>16);
			clr5=BGBRASW_MAKEPIXEL(crb>>16, cgb>>16, cbb>>16, cab>>16);
			cra+=crav;	cga+=cgav;		cba+=cbav;	caa+=caav;
			crb+=crbv;	cgb+=cgbv;		cbb+=cbbv;	cab+=cabv;

			xa=xc0>>XY_SHR; xb=xc1>>XY_SHR;
			prim->tabs->drawSpanFlat(prim->tabs,
				ctx->tgt_rgba+yc*ctx->tgt_xs+xa,
				ctx->tgt_zbuf+yc*ctx->tgt_xs+xa,
				xb-xa, clr4, clr5, zsa, zsb);

			zsa+=zsav;		zsb+=zsbv;
			xc0+=xc0v; xc1+=xc1v; yc++;
		}
	}
}
