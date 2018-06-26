/*
Box: Simple Rectangular Box
Vertex 0: Bottom Left
Vertex 1: Bottom Right
Vertex 2: Top Left
Vertex 3: Top Right
*/

#include <bgbrasw.h>

void BGBRASW_DrawPrimitive_BoxFlatBasic(
	BGBRASW_Context *ctx,
	BGBRASW_Primitive *prim)
{
	bgbrasw_pixel clr;
	int xs, ys, x0, x1, y0, y1, yc;
	int i, j, k;

	x0=prim->xy[0][0];	x1=prim->xy[1][0];
	y0=prim->xy[0][1];	y1=prim->xy[2][1];
	
	clr=prim->rgba[0];

	xs=x1-x0;	ys=y1-y0;
	yc=y0;
	while(yc<y1)
	{
		prim->tabs->drawSpanFlatBasic(prim->tabs,
			ctx->tgt_rgba+yc*ctx->tgt_xs+x0, xs, clr);
		yc++;
	}
}

void BGBRASW_DrawPrimitive_BoxFlatInterp(
	BGBRASW_Context *ctx,
	BGBRASW_Primitive *prim)
{
	bgbrasw_pixel clr0, clr1, clr2, clr3;
	bgbrasw_pixel clr4, clr5;
	int xs, ys, x0, x1, y0, y1, yc;
	int cra, crb, crav, crbv;
	int cga, cgb, cgav, cgbv;
	int cba, cbb, cbav, cbbv;
	int caa, cab, caav, cabv;
	int i, j, k;

	x0=prim->xy[0][0];	x1=prim->xy[1][0];
	y0=prim->xy[0][1];	y1=prim->xy[2][1];
	xs=x1-x0;	ys=y1-y0;
	
	clr0=prim->rgba[0];	clr1=prim->rgba[1];
	clr2=prim->rgba[2];	clr3=prim->rgba[3];

	cra=BGBRASW_PIXEL_R(clr0)<<16;	cga=BGBRASW_PIXEL_G(clr0)<<16;
	cba=BGBRASW_PIXEL_B(clr0)<<16;	caa=BGBRASW_PIXEL_A(clr0)<<16;
	crav=((int)(BGBRASW_PIXEL_R(clr2)-BGBRASW_PIXEL_R(clr0))<<16)/(ys+1);
	cgav=((int)(BGBRASW_PIXEL_G(clr2)-BGBRASW_PIXEL_G(clr0))<<16)/(ys+1);
	cbav=((int)(BGBRASW_PIXEL_B(clr2)-BGBRASW_PIXEL_B(clr0))<<16)/(ys+1);
	caav=((int)(BGBRASW_PIXEL_A(clr2)-BGBRASW_PIXEL_A(clr0))<<16)/(ys+1);

	crb=BGBRASW_PIXEL_R(clr1)<<16;	cgb=BGBRASW_PIXEL_G(clr1)<<16;
	cbb=BGBRASW_PIXEL_B(clr1)<<16;	cab=BGBRASW_PIXEL_A(clr1)<<16;
	crbv=((int)(BGBRASW_PIXEL_R(clr3)-BGBRASW_PIXEL_R(clr1))<<16)/(ys+1);
	cgbv=((int)(BGBRASW_PIXEL_G(clr3)-BGBRASW_PIXEL_G(clr1))<<16)/(ys+1);
	cbbv=((int)(BGBRASW_PIXEL_B(clr3)-BGBRASW_PIXEL_B(clr1))<<16)/(ys+1);
	cabv=((int)(BGBRASW_PIXEL_A(clr3)-BGBRASW_PIXEL_A(clr1))<<16)/(ys+1);

	yc=y0;
	while(yc<y1)
	{
		clr4=BGBRASW_MAKEPIXEL(cra>>16, cga>>16, cba>>16, caa>>16);
		clr5=BGBRASW_MAKEPIXEL(crb>>16, cgb>>16, cbb>>16, cab>>16);
		cra+=crav;	cga+=cgav;		cba+=cbav;	caa+=caav;
		crb+=crbv;	cgb+=cgbv;		cbb+=cbbv;	cab+=cabv;

		prim->tabs->drawSpanFlatInterp(prim->tabs,
			ctx->tgt_rgba+yc*ctx->tgt_xs+x0, xs, clr4, clr5);
		yc++;
	}
}

void BGBRASW_DrawPrimitive_BoxTexBasic(
	BGBRASW_Context *ctx,
	BGBRASW_Primitive *prim)
{
	bgbrasw_pixel clr;
	bgbrasw_pixel *tergba;
	int xs, ys, x0, x1, y0, y1, yc;
	int stsa, stsb, stsav, stsbv;
	int stta, sttb, sttav, sttbv;
	int st0s, st0t, st1s, st1t;
	int st2s, st2t, st3s, st3t;
	int sts0, stt0, sts1, stt1;
	int txs, tys, mip;
	int i0, i1, i2, i3;
	int i, j, k;

	x0=prim->xy[0][0];	x1=prim->xy[1][0];
	y0=prim->xy[0][1];	y1=prim->xy[2][1];
	xs=x1-x0;	ys=y1-y0;

	st0s=prim->st[0][0];	st0t=prim->st[0][1];
	st1s=prim->st[1][0];	st1t=prim->st[1][1];
	st2s=prim->st[2][0];	st2t=prim->st[2][1];
	st3s=prim->st[3][0];	st3t=prim->st[3][1];
	
	stsav=((st2s-st0s)<<8)/(ys+1);
	sttav=((st2t-st0t)<<8)/(ys+1);
	stsbv=((st3s-st1s)<<8)/(ys+1);
	sttbv=((st3t-st1t)<<8)/(ys+1);

//	clr=prim->rgba[0];

	stsa=(st0s<<8);
	stta=(st0t<<8);
	stsb=(st1s<<8);
	sttb=(st1t<<8);

//	j=(abs(st1s-st0s)+abs(st1t-st0t))/(xs+ys);
//	mip=bgbrasw_log2dn(j);
	i0=st1s-st0s; i1=st1t-st0t;
	j=(i0*i0+i1*i1)/(xs*xs+ys*ys);
	mip=(bgbrasw_log2dn(j)+1)/2;
//	mip=1;

	tergba=prim->tex->rgba[mip];
	txs=prim->tex->xs>>mip;
	tys=prim->tex->ys>>mip;

	if(!tergba)
	{
		mip=0;
		tergba=prim->tex->rgba[0];
		txs=prim->tex->xs;
		tys=prim->tex->ys;
	}

	yc=y0;
	while(yc<y1)
	{
#if 0
		sts0=stsa>>8;		stt0=stta>>8;
		sts1=stsb>>8;		stt1=sttb>>8;

		j=(abs(sts1-sts0)+abs(stt1-stt0))/xs;
		mip=bgbrasw_log2dn(j);
		tergba=prim->tex->rgba[mip];
		txs=prim->tex->xs>>mip;
		tys=prim->tex->ys>>mip;

		sts0=sts0>>mip;		stt0=stt0>>mip;
		sts1=sts1>>mip;		stt1=stt1>>mip;
#endif

//		sts0=stsa>>(8+mip);		stt0=stta>>(8+mip);
//		sts1=stsb>>(8+mip);		stt1=sttb>>(8+mip);
		sts0=stsa>>mip;		stt0=stta>>mip;
		sts1=stsb>>mip;		stt1=sttb>>mip;

		prim->tabs->drawSpanTextureBasic(prim->tabs,
			ctx->tgt_rgba+yc*ctx->tgt_xs+x0, xs,
			tergba, txs, tys,
			sts0, stt0, sts1, stt1);
	
//		prim->tabs->drawSpanTextureBasic(
//			ctx->tgt_rgba+yc*ctx->tgt_xs+x0, xs,
//			tergba, txs, tys,
//			stsa>>8, stta>>8, stsb>>8, sttb>>8);
		stsa+=stsav;	stta+=sttav;
		stsb+=stsbv;	sttb+=sttbv;
		yc++;
	}
}

void BGBRASW_DrawPrimitive_BoxTexInterp(
	BGBRASW_Context *ctx,
	BGBRASW_Primitive *prim)
{
	bgbrasw_pixel clr0, clr1, clr2, clr3;
	bgbrasw_pixel clr4, clr5;
	bgbrasw_pixel *tergba;
	int xs, ys, x0, x1, y0, y1, yc;
	int cra, crb, crav, crbv;
	int cga, cgb, cgav, cgbv;
	int cba, cbb, cbav, cbbv;
	int caa, cab, caav, cabv;
	int stsa, stsb, stsav, stsbv;
	int stta, sttb, sttav, sttbv;
	int st0s, st0t, st1s, st1t;
	int st2s, st2t, st3s, st3t;
	int sts0, stt0, sts1, stt1;
	int i0, i1, i2, i3;
	int txs, tys, mip;
	int i, j, k;

	x0=prim->xy[0][0];	x1=prim->xy[1][0];
	y0=prim->xy[0][1];	y1=prim->xy[2][1];
	xs=x1-x0;	ys=y1-y0;
	
	clr0=prim->rgba[0];	clr1=prim->rgba[1];
	clr2=prim->rgba[2];	clr3=prim->rgba[3];

	cra=BGBRASW_PIXEL_R(clr0)<<16;	cga=BGBRASW_PIXEL_G(clr0)<<16;
	cba=BGBRASW_PIXEL_B(clr0)<<16;	caa=BGBRASW_PIXEL_A(clr0)<<16;
	crav=((int)(BGBRASW_PIXEL_R(clr2)-BGBRASW_PIXEL_R(clr0))<<16)/(ys+1);
	cgav=((int)(BGBRASW_PIXEL_G(clr2)-BGBRASW_PIXEL_G(clr0))<<16)/(ys+1);
	cbav=((int)(BGBRASW_PIXEL_B(clr2)-BGBRASW_PIXEL_B(clr0))<<16)/(ys+1);
	caav=((int)(BGBRASW_PIXEL_A(clr2)-BGBRASW_PIXEL_A(clr0))<<16)/(ys+1);

	crb=BGBRASW_PIXEL_R(clr1)<<16;	cgb=BGBRASW_PIXEL_G(clr1)<<16;
	cbb=BGBRASW_PIXEL_B(clr1)<<16;	cab=BGBRASW_PIXEL_A(clr1)<<16;
	crbv=((int)(BGBRASW_PIXEL_R(clr3)-BGBRASW_PIXEL_R(clr1))<<16)/(ys+1);
	cgbv=((int)(BGBRASW_PIXEL_G(clr3)-BGBRASW_PIXEL_G(clr1))<<16)/(ys+1);
	cbbv=((int)(BGBRASW_PIXEL_B(clr3)-BGBRASW_PIXEL_B(clr1))<<16)/(ys+1);
	cabv=((int)(BGBRASW_PIXEL_A(clr3)-BGBRASW_PIXEL_A(clr1))<<16)/(ys+1);

	st0s=prim->st[0][0];	st0t=prim->st[0][1];
	st1s=prim->st[1][0];	st1t=prim->st[1][1];
	st2s=prim->st[2][0];	st2t=prim->st[2][1];
	st3s=prim->st[3][0];	st3t=prim->st[3][1];
	
	stsav=((st2s-st0s)<<8)/(ys+1);
	sttav=((st2t-st0t)<<8)/(ys+1);
	stsbv=((st3s-st1s)<<8)/(ys+1);
	sttbv=((st3t-st1t)<<8)/(ys+1);

	stsa=(st0s<<8);		stta=(st0t<<8);
	stsb=(st1s<<8);		sttb=(st1t<<8);

	mip=0;
//	j=(abs(st1s-st0s)+abs(st1t-st0t))/(xs+ys);
//	mip=bgbrasw_log2dn(j);
	i0=st1s-st0s; i1=st1t-st0t;
	j=(i0*i0+i1*i1)/(xs*xs+ys*ys);
	mip=(bgbrasw_log2dn(j)+1)/2;
//	mip=1;

	tergba=prim->tex->rgba[mip];
	txs=prim->tex->xs>>mip;
	tys=prim->tex->ys>>mip;

	if(!tergba)
	{
		mip=0;
		tergba=prim->tex->rgba[0];
		txs=prim->tex->xs;
		tys=prim->tex->ys;
	}

	yc=y0;
	while(yc<y1)
	{
		clr4=BGBRASW_MAKEPIXEL(cra>>16, cga>>16, cba>>16, caa>>16);
		clr5=BGBRASW_MAKEPIXEL(crb>>16, cgb>>16, cbb>>16, cab>>16);
		cra+=crav;	cga+=cgav;		cba+=cbav;	caa+=caav;
		crb+=crbv;	cgb+=cgbv;		cbb+=cbbv;	cab+=cabv;

//		sts0=stsa>>(8+mip);		stt0=stta>>(8+mip);
//		sts1=stsb>>(8+mip);		stt1=sttb>>(8+mip);
		sts0=stsa>>mip;		stt0=stta>>mip;
		sts1=stsb>>mip;		stt1=sttb>>mip;

//		prim->tabs->drawSpanTextureBasic(
//			ctx->tgt_rgba+yc*ctx->tgt_xs+x0, xs,
//			tergba, txs, tys,
//			sts0, stt0, sts1, stt1);

		prim->tabs->drawSpanTextureInterp(prim->tabs,
			ctx->tgt_rgba+yc*ctx->tgt_xs+x0, xs,
			tergba, txs, tys,
			sts0, stt0, sts1, stt1,
			clr4, clr5);
	
//		prim->tabs->drawSpanTextureInterp(
//			ctx->tgt_rgba+yc*ctx->tgt_xs+x0, xs,
//			tergba, txs, tys,
//			stsa>>8, stta>>8, stsb>>8, sttb>>8,
//			clr4, clr5);

		stsa+=stsav;	stta+=sttav;
		stsb+=stsbv;	sttb+=sttbv;
		yc++;
	}
}


void BGBRASW_DrawPrimitive_BoxTexBasicZTest(
	BGBRASW_Context *ctx,
	BGBRASW_Primitive *prim)
{
	bgbrasw_pixel clr;
	bgbrasw_pixel *tergba;
	int xs, ys, x0, x1, y0, y1, yc;
	int z0, z1, z2, z3, za, zb, zav, zbv;
	int stsa, stsb, stsav, stsbv;
	int stta, sttb, sttav, sttbv;
	int st0s, st0t, st1s, st1t;
	int st2s, st2t, st3s, st3t;
	int sts0, stt0, sts1, stt1;
	int txs, tys, mip;
	int i, j, k;

	x0=prim->xy[0][0];	x1=prim->xy[1][0];
	y0=prim->xy[0][1];	y1=prim->xy[2][1];
	xs=x1-x0;	ys=y1-y0;

	st0s=prim->st[0][0];	st0t=prim->st[0][1];
	st1s=prim->st[1][0];	st1t=prim->st[1][1];
	st2s=prim->st[2][0];	st2t=prim->st[2][1];
	st3s=prim->st[3][0];	st3t=prim->st[3][1];
	
	stsav=((st2s-st0s)<<8)/(ys+1);
	sttav=((st2t-st0t)<<8)/(ys+1);
	stsbv=((st3s-st1s)<<8)/(ys+1);
	sttbv=((st3t-st1t)<<8)/(ys+1);

	stsa=(st0s<<8);	stta=(st0t<<8);
	stsb=(st1s<<8);	sttb=(st1t<<8);

	z0=prim->z[0];	z1=prim->z[1];
	z2=prim->z[2];	z3=prim->z[3];
	za=z0; zb=z1;
	zav=(z2-z0)/(ys+1);
	zbv=(z3-z1)/(ys+1);

	yc=y0;
	while(yc<y1)
	{
		tergba=prim->tex->rgba[0];
		txs=prim->tex->xs;
		tys=prim->tex->ys;
		mip=0;

		sts0=stsa>>mip;		stt0=stta>>mip;
		sts1=stsb>>mip;		stt1=sttb>>mip;

		prim->tabs->drawSpanTextureBasicZTest(prim->tabs,
			ctx->tgt_rgba+yc*ctx->tgt_xs+x0,
			ctx->tgt_zbuf+yc*ctx->tgt_xs+x0, xs,
			tergba, txs, tys,
			sts0, stt0, sts1, stt1,
			za, zb);

//		prim->tabs->drawSpanTextureBasicZTest(
//			ctx->tgt_rgba+yc*ctx->tgt_xs+x0,
//			ctx->tgt_zbuf+yc*ctx->tgt_xs+x0, xs,
//			tergba, txs, tys,
//			stsa>>8, stta>>8, stsb>>8, sttb>>8,
//			256, 256);
		za+=zav;		zb+=zbv;
		stsa+=stsav;	stta+=sttav;
		stsb+=stsbv;	sttb+=sttbv;
		yc++;
	}
}

void BGBRASW_DrawPrimitive_BoxTexInterpZTest(
	BGBRASW_Context *ctx,
	BGBRASW_Primitive *prim)
{
	bgbrasw_pixel clr0, clr1, clr2, clr3;
	bgbrasw_pixel clr4, clr5;
	bgbrasw_pixel *tergba;
	int xs, ys, x0, x1, y0, y1, yc;
	int z0, z1, z2, z3, za, zb, zav, zbv;
	int cra, crb, crav, crbv;
	int cga, cgb, cgav, cgbv;
	int cba, cbb, cbav, cbbv;
	int caa, cab, caav, cabv;
	int stsa, stsb, stsav, stsbv;
	int stta, sttb, sttav, sttbv;
	int st0s, st0t, st1s, st1t;
	int st2s, st2t, st3s, st3t;
	int sts0, stt0, sts1, stt1;
	int txs, tys, mip;
	int i, j, k;

	x0=prim->xy[0][0];	x1=prim->xy[1][0];
	y0=prim->xy[0][1];	y1=prim->xy[2][1];
	xs=x1-x0;	ys=y1-y0;
	
	clr0=prim->rgba[0];	clr1=prim->rgba[1];
	clr2=prim->rgba[2];	clr3=prim->rgba[3];

	cra=BGBRASW_PIXEL_R(clr0)<<16;	cga=BGBRASW_PIXEL_G(clr0)<<16;
	cba=BGBRASW_PIXEL_B(clr0)<<16;	caa=BGBRASW_PIXEL_A(clr0)<<16;
	crav=((BGBRASW_PIXEL_R(clr2)-BGBRASW_PIXEL_R(clr0))<<16)/(ys+1);
	cgav=((BGBRASW_PIXEL_G(clr2)-BGBRASW_PIXEL_G(clr0))<<16)/(ys+1);
	cbav=((BGBRASW_PIXEL_B(clr2)-BGBRASW_PIXEL_B(clr0))<<16)/(ys+1);
	caav=((BGBRASW_PIXEL_A(clr2)-BGBRASW_PIXEL_A(clr0))<<16)/(ys+1);

	crb=BGBRASW_PIXEL_R(clr0)<<16;	cgb=BGBRASW_PIXEL_G(clr0)<<16;
	cbb=BGBRASW_PIXEL_B(clr0)<<16;	cab=BGBRASW_PIXEL_A(clr0)<<16;
	crbv=((BGBRASW_PIXEL_R(clr3)-BGBRASW_PIXEL_R(clr1))<<16)/(ys+1);
	cgbv=((BGBRASW_PIXEL_G(clr3)-BGBRASW_PIXEL_G(clr1))<<16)/(ys+1);
	cbbv=((BGBRASW_PIXEL_B(clr3)-BGBRASW_PIXEL_B(clr1))<<16)/(ys+1);
	cabv=((BGBRASW_PIXEL_A(clr3)-BGBRASW_PIXEL_A(clr1))<<16)/(ys+1);

	st0s=prim->st[0][0];	st0t=prim->st[0][1];
	st1s=prim->st[1][0];	st1t=prim->st[1][1];
	st2s=prim->st[2][0];	st2t=prim->st[2][1];
	st3s=prim->st[3][0];	st3t=prim->st[3][1];
	
	stsav=((st2s-st0s)<<8)/(ys+1);
	sttav=((st2t-st0t)<<8)/(ys+1);
	stsbv=((st3s-st1s)<<8)/(ys+1);
	sttbv=((st3t-st1t)<<8)/(ys+1);

	stsa=(st0s<<8);		stta=(st0t<<8);
	stsb=(st1s<<8);		sttb=(st1t<<8);

	z0=prim->z[0];	z1=prim->z[1];
	z2=prim->z[2];	z3=prim->z[3];
	za=z0; zb=z1;
	zav=(z2-z0)/(ys+1);
	zbv=(z3-z1)/(ys+1);

	yc=y0;
	while(yc<y1)
	{
		clr4=BGBRASW_MAKEPIXEL(cra>>16, cga>>16, cba>>16, caa>>16);
		clr5=BGBRASW_MAKEPIXEL(crb>>16, cgb>>16, cbb>>16, cab>>16);
		cra+=crav;	cga+=cgav;		cba+=cbav;	caa+=caav;
		crb+=crbv;	cgb+=cgbv;		cbb+=cbbv;	cab+=cabv;

		tergba=prim->tex->rgba[0];
		txs=prim->tex->xs;
		tys=prim->tex->ys;
		mip=0;
	
		sts0=stsa>>mip;		stt0=stta>>mip;
		sts1=stsb>>mip;		stt1=sttb>>mip;

		prim->tabs->drawSpanTextureInterpZTest(prim->tabs,
			ctx->tgt_rgba+yc*ctx->tgt_xs+x0,
			ctx->tgt_zbuf+yc*ctx->tgt_xs+x0, xs,
			tergba, txs, tys,
			sts0, stt0, sts1, stt1,
			clr4, clr5, 256, 256);

//		prim->tabs->drawSpanTextureInterpZTest(
//			ctx->tgt_rgba+yc*ctx->tgt_xs+x0,
//			ctx->tgt_zbuf+yc*ctx->tgt_xs+x0, xs,
//			tergba, txs, tys,
//			stsa>>8, stta>>8, stsb>>8, sttb>>8,
//			clr4, clr5, 256, 256);

		za+=zav;		zb+=zbv;
		stsa+=stsav;	stta+=sttav;
		stsb+=stsbv;	sttb+=sttbv;
		yc++;
	}
}

void BGBRASW_DrawPrimitive_BoxTestBlend(
	BGBRASW_Context *ctx,
	BGBRASW_Primitive *prim)
{
	bgbrasw_pixel clr0, clr1, clr2, clr3;
	bgbrasw_pixel clr4, clr5;
	bgbrasw_zbuf zb0, zb1, zb2, zb3;
	u32 tza, tzav, tz2;
	u32 tzb, tzbv;
	bgbrasw_pixel *tergba;
	int xs, ys, x0, x1, y0, y1, yc;
	int cra, crb, crav, crbv;
	int cga, cgb, cgav, cgbv;
	int cba, cbb, cbav, cbbv;
	int caa, cab, caav, cabv;
	int stsa, stsb, stsav, stsbv;
	int stta, sttb, sttav, sttbv;
	int st0s, st0t, st1s, st1t;
	int st2s, st2t, st3s, st3t;
	int sts0, stt0, sts1, stt1;
	int txs, tys, mip;
	int i, j, k;

	x0=prim->xy[0][0];	x1=prim->xy[1][0];
	y0=prim->xy[0][1];	y1=prim->xy[2][1];
	xs=x1-x0;	ys=y1-y0;
	
	clr0=prim->rgba[0];	clr1=prim->rgba[1];
	clr2=prim->rgba[2];	clr3=prim->rgba[3];
	
	zb0=prim->z[0];	zb1=prim->z[1];
	zb2=prim->z[2];	zb3=prim->z[3];

	cra=BGBRASW_PIXEL_R(clr0)<<16;	cga=BGBRASW_PIXEL_G(clr0)<<16;
	cba=BGBRASW_PIXEL_B(clr0)<<16;	caa=BGBRASW_PIXEL_A(clr0)<<16;
	crav=((BGBRASW_PIXEL_R(clr2)-BGBRASW_PIXEL_R(clr0))<<16)/(ys+1);
	cgav=((BGBRASW_PIXEL_G(clr2)-BGBRASW_PIXEL_G(clr0))<<16)/(ys+1);
	cbav=((BGBRASW_PIXEL_B(clr2)-BGBRASW_PIXEL_B(clr0))<<16)/(ys+1);
	caav=((BGBRASW_PIXEL_A(clr2)-BGBRASW_PIXEL_A(clr0))<<16)/(ys+1);

	crb=BGBRASW_PIXEL_R(clr0)<<16;	cgb=BGBRASW_PIXEL_G(clr0)<<16;
	cbb=BGBRASW_PIXEL_B(clr0)<<16;	cab=BGBRASW_PIXEL_A(clr0)<<16;
	crbv=((BGBRASW_PIXEL_R(clr3)-BGBRASW_PIXEL_R(clr1))<<16)/(ys+1);
	cgbv=((BGBRASW_PIXEL_G(clr3)-BGBRASW_PIXEL_G(clr1))<<16)/(ys+1);
	cbbv=((BGBRASW_PIXEL_B(clr3)-BGBRASW_PIXEL_B(clr1))<<16)/(ys+1);
	cabv=((BGBRASW_PIXEL_A(clr3)-BGBRASW_PIXEL_A(clr1))<<16)/(ys+1);

	st0s=prim->st[0][0];	st0t=prim->st[0][1];
	st1s=prim->st[1][0];	st1t=prim->st[1][1];
	st2s=prim->st[2][0];	st2t=prim->st[2][1];
	st3s=prim->st[3][0];	st3t=prim->st[3][1];
	
	stsav=((st2s-st0s)<<8)/(ys+1);
	sttav=((st2t-st0t)<<8)/(ys+1);
	stsbv=((st3s-st1s)<<8)/(ys+1);
	sttbv=((st3t-st1t)<<8)/(ys+1);

	stsa=(st0s<<8);		stta=(st0t<<8);
	stsb=(st1s<<8);		sttb=(st1t<<8);

	tza=zb0;	tzb=zb1;
	tzav=(zb2-zb0)/(ys+1);
	tzbv=(zb3-zb1)/(ys+1);

	yc=y0;
	while(yc<y1)
	{
		clr4=BGBRASW_MAKEPIXEL(cra>>16, cga>>16, cba>>16, caa>>16);
		clr5=BGBRASW_MAKEPIXEL(crb>>16, cgb>>16, cbb>>16, cab>>16);
		cra+=crav;	cga+=cgav;		cba+=cbav;	caa+=caav;
		crb+=crbv;	cgb+=cgbv;		cbb+=cbbv;	cab+=cabv;

		tergba=prim->tex->rgba[0];
		txs=prim->tex->xs;
		tys=prim->tex->ys;
		mip=0;
	
		sts0=stsa>>mip;		stt0=stta>>mip;
		sts1=stsb>>mip;		stt1=sttb>>mip;
		tz2=(tza&BGBRASW_MASK_DEPTH)|(zb0&BGBRASW_MASK_STENCIL);

//		prim->tabs->drawSpanTex(prim->tabs,
		prim->tabs->drawSpanTex_min(prim->tabs,
			ctx->tgt_rgba+yc*ctx->tgt_xs+x0,
			ctx->tgt_zbuf+yc*ctx->tgt_xs+x0, xs,
			tergba, txs, tys,
			sts0, stt0, sts1, stt1,
			clr4, clr5, tz2, tzb);

//		prim->tabs->drawSpanTextureInterpZTest(
//			ctx->tgt_rgba+yc*ctx->tgt_xs+x0,
//			ctx->tgt_zbuf+yc*ctx->tgt_xs+x0, xs,
//			tergba, txs, tys,
//			stsa>>8, stta>>8, stsb>>8, sttb>>8,
//			clr4, clr5, 256, 256);

		tza+=tzav;		tzb+=tzbv;
		stsa+=stsav;	stta+=sttav;
		stsb+=stsbv;	sttb+=sttbv;
		yc++;
	}
}


void BGBRASW_DrawPrimitive_Box(
	BGBRASW_Context *ctx,
	BGBRASW_Primitive *prim)
{
	switch(prim->mode)
	{
	case BGBRASW_PMODE_FLATBASIC:
		BGBRASW_DrawPrimitive_BoxFlatBasic(ctx, prim);
		break;
	case BGBRASW_PMODE_FLATINTERP:
		BGBRASW_DrawPrimitive_BoxFlatInterp(ctx, prim);
		break;

	case BGBRASW_PMODE_TEXBASIC:
		BGBRASW_DrawPrimitive_BoxTexBasic(ctx, prim);
		break;
	case BGBRASW_PMODE_TEXINTERP:
		BGBRASW_DrawPrimitive_BoxTexInterp(ctx, prim);
		break;
	case BGBRASW_PMODE_TEXBASICZTEST:
		BGBRASW_DrawPrimitive_BoxTexBasicZTest(ctx, prim);
		break;
	case BGBRASW_PMODE_TEXINTERPZTEST:
		BGBRASW_DrawPrimitive_BoxTexInterpZTest(ctx, prim);
		break;
	default:
		break;
	}
}
