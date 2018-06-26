#include <bgbrasw.h>

void BGBRASW_DrawPrimitive_Line(
	BGBRASW_Context *ctx,
	BGBRASW_Primitive *prim)
{
	bgbrasw_pixel clr0, clr1, clr2, clr3;
	bgbrasw_pixel clr4, clr5;
	bgbrasw_zbuf zb0, zb1, zb2, zb3;
	u32 tza, tzav, tz2;
	u32 tzb, tzbv;
	bgbrasw_pixel *tergba;
	int xs, ys, x0, x1, y0, y1;
	int xc, yc, xd, yd, td, ta;
	int xsg, ysg;
	int cra, crb, crav, crbv;
	int cga, cgb, cgav, cgbv;
	int cba, cbb, cbav, cbbv;
	int caa, cab, caav, cabv;
	int txs, tys, mip;
	int i, j, k;

	x0=prim->xy[0][0];	x1=prim->xy[1][0];
	y0=prim->xy[0][1];	y1=prim->xy[1][1];
	xs=x1-x0;	ys=y1-y0;

	xd=xs; yd=ys; xsg=1; ysg=1;
	if(xd<0) { xd=-xd; xsg=-1; }
	if(yd<0) { yd=-yd; ysg=-1; }
	td=xd+yd;
	
	clr0=prim->rgba[0];	clr1=prim->rgba[1];	
	zb0=prim->z[0];	zb1=prim->z[1];

	if(yd>=xd)
	{
		cra=BGBRASW_PIXEL_R(clr0)<<16;	cga=BGBRASW_PIXEL_G(clr0)<<16;
		cba=BGBRASW_PIXEL_B(clr0)<<16;	caa=BGBRASW_PIXEL_A(clr0)<<16;
		crav=((BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<16)/(yd+1);
		cgav=((BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<16)/(yd+1);
		cbav=((BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<16)/(yd+1);
		caav=((BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<16)/(yd+1);

		tza=zb0;
		tzav=(zb1-zb0)/(yd+1);

		xc=x0; yc=y0; ta=0;
		for(i=0; i<yd; i++)
		{
			clr4=BGBRASW_MAKEPIXEL(cra>>16, cga>>16, cba>>16, caa>>16);
			cra+=crav;	cga+=cgav;		cba+=cbav;	caa+=caav;

			tz2=(tza&BGBRASW_MASK_DEPTH)|(zb0&BGBRASW_MASK_STENCIL);

			ctx->tgt_rgba[yc*ctx->tgt_xs+xc]=clr4;
			ctx->tgt_zbuf[yc*ctx->tgt_xs+xc]=tza;

			tza+=tzav;

			ta+=xd;
			if(ta>=yd)
				{ xs+=xsg; ta-=yd; }
			yc+=ysg;
		}
	}else
	{
		cra=BGBRASW_PIXEL_R(clr0)<<16;	cga=BGBRASW_PIXEL_G(clr0)<<16;
		cba=BGBRASW_PIXEL_B(clr0)<<16;	caa=BGBRASW_PIXEL_A(clr0)<<16;
		crav=((BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<16)/(xd+1);
		cgav=((BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<16)/(xd+1);
		cbav=((BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<16)/(xd+1);
		caav=((BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<16)/(xd+1);

		tza=zb0;
		tzav=(zb1-zb0)/(xd+1);

		xc=x0; yc=y0; ta=0;
		for(i=0; i<xd; i++)
		{
			clr4=BGBRASW_MAKEPIXEL(cra>>16, cga>>16, cba>>16, caa>>16);
			cra+=crav;	cga+=cgav;		cba+=cbav;	caa+=caav;

			tz2=(tza&BGBRASW_MASK_DEPTH)|(zb0&BGBRASW_MASK_STENCIL);

			ctx->tgt_rgba[yc*ctx->tgt_xs+xc]=clr4;
			ctx->tgt_zbuf[yc*ctx->tgt_xs+xc]=tza;

			tza+=tzav;

			ta+=yd;
			if(ta>=xd)
				{ ys+=ysg; ta-=xd; }
			xc+=xsg;
		}
	}
}
