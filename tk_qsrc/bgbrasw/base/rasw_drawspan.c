#include <bgbrasw.h>

void BGBRASW_DrawSpanFlatBasic(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, int npix,
	bgbrasw_pixel clr)
{
	bgbrasw_pixel *ct, *cte;

	ct=span; cte=span+npix;

	while((ct+16)<=cte)
	{
		*ct++=clr;	*ct++=clr;	*ct++=clr;	*ct++=clr;
		*ct++=clr;	*ct++=clr;	*ct++=clr;	*ct++=clr;
		*ct++=clr;	*ct++=clr;	*ct++=clr;	*ct++=clr;
		*ct++=clr;	*ct++=clr;	*ct++=clr;	*ct++=clr;
	}
	if((ct+8)<=cte)
	{
		*ct++=clr;	*ct++=clr;	*ct++=clr;	*ct++=clr;
		*ct++=clr;	*ct++=clr;	*ct++=clr;	*ct++=clr;
	}
	if((ct+4)<=cte)
		{ *ct++=clr; *ct++=clr; *ct++=clr; *ct++=clr; }

	while(ct<cte)
		{ *ct++=clr; }
}

void BGBRASW_DrawSpanFlatInterp(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, int npix,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1)
{
	bgbrasw_pixel *ct, *cte;
	int cr, crv;
	int cg, cgv;
	int cb, cbv;
	int ca, cav;
	int crag, crag2, cragv;
	int crrb, crrb2, crrbv;
	int clr, npix1;

#if 1
	if(clr0==clr1)
	{
		BGBRASW_DrawSpanFlatBasic(tabs, span, npix, clr0);
		return;
	}
#endif

	if(npix<=0)
		return;

#if 0
	if(npix>1024)
	{
		npix1=npix>>1;
		clr=((clr0>>1)&0x7F7F7F7F)+((clr1>>1)&0x7F7F7F7F);
		BGBRASW_DrawSpanFlatInterp(span, npix1, clr0, clr);
		BGBRASW_DrawSpanFlatInterp(span+npix1, npix-npix1, clr, clr1);
	}
#endif

#if 1
	crag=clr0&0xFF00FF00;
	crrb=(clr0<<8)&0xFF00FF00;
//	crag2=clr1&0xFF00FF00;
//	crrb2=(clr1<<8)&0xFF00FF00;

//	cr=BGBRASW_PIXEL_R(clr0)<<8;	cg=BGBRASW_PIXEL_G(clr0)<<8;
//	cb=BGBRASW_PIXEL_B(clr0)<<8;	ca=BGBRASW_PIXEL_A(clr0)<<8;

	crv=((BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<8)/npix;
	cgv=((BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<8)/npix;
	cbv=((BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<8)/npix;
	cav=((BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<8)/npix;
	cragv=((cav<<16)&0xFFFF0000)|(cgv&0x00000FFFF);
	crrbv=((crv<<16)&0xFFFF0000)|(cbv&0x00000FFFF);

//	crv=((BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<24)/npix;
//	cgv=((BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<8)/npix;
//	cbv=((BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<8)/npix;
//	cav=((BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<24)/npix;
//	cragv=(cav&0xFFFF0000)|(cgv&0x00000FFFF);
//	crrbv=(crv&0xFFFF0000)|(cbv&0x00000FFFF);

	ct=span; cte=span+npix;

#if 1
	while((ct+8)<=cte)
	{
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv; *ct++=clr;
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv; *ct++=clr;
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv; *ct++=clr;
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv; *ct++=clr;
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv; *ct++=clr;
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv; *ct++=clr;
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv; *ct++=clr;
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv; *ct++=clr;
	}
#endif

//	while((ct+4)<=cte)
	if((ct+4)<=cte)
	{
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv; *ct++=clr;
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv; *ct++=clr;
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv; *ct++=clr;
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv; *ct++=clr;
//		crag=crag&0xFFF8FFF8;
//		crrb=crrb&0xFFF8FFF8;
	}

	while(ct<cte)
	{
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv;
//		crag=(crag+cragv)&0xFFFEFFFE;
//		crrb=(crrb+crrbv)&0xFFFEFFFE;
		*ct++=clr;
	}
#endif

#if 0
//	npix1=npix+1;
	cr=BGBRASW_PIXEL_R(clr0)<<16;	cg=BGBRASW_PIXEL_G(clr0)<<16;
	cb=BGBRASW_PIXEL_B(clr0)<<16;	ca=BGBRASW_PIXEL_A(clr0)<<16;
	crv=((int)(BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<16)/npix;
	cgv=((int)(BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<16)/npix;
	cbv=((int)(BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<16)/npix;
	cav=((int)(BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<16)/npix;

	ct=span; cte=span+npix;
	while(ct<cte)
	{
		clr=BGBRASW_MAKEPIXEL(cr>>16, cg>>16, cb>>16, ca>>16);
//		clr=BGBRASW_MAKEPIXEL(
//			(cr>>16)&255, (cg>>16)&255,
//			(cb>>16)&255, (ca>>16)&255);
		cr+=crv;	cg+=cgv;
		cb+=cbv;	ca+=cav;
		*ct++=clr;
	}
#endif
}

void BGBRASW_DrawSpanFlatBasicZTest(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix,
	bgbrasw_pixel clr, bgbrasw_zbuf z0, bgbrasw_zbuf z1)
{
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	u32 tz, tzv, tz2;

	if(npix<=0)
		return;

//	tz=z0; tzv=((s32)(z1-z0))/npix;
	tz=z0; tzv=bgbrasw_fixdiv_s32_u16(z1-z0, npix);
	tzv&=BGBRASW_MASK_DEPTH;

	ct=span; cte=span+npix;
	ctz=spanz; ctze=spanz+npix;
	while(ct<cte)
	{
//		tz2=tz&BGBRASW_MASK_DEPTH;
		tz2=tz;
		if(tz2>(*ctz))
		{
			//Z fail
			ct++; ctz++;
			tz+=tzv;
			continue;
		}
		//Z pass
		*ct++=clr;
		*ctz++=tz2;
//		*ctz++=tz2|(z0&BGBRASW_MASK_STENCIL);
		tz+=tzv;
	}
}

void BGBRASW_DrawSpanFlatInterpZTest(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1,
	bgbrasw_zbuf z0, bgbrasw_zbuf z1)
{
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	u32 tz, tzv, tz2;
	int cr, crv;
	int cg, cgv;
	int cb, cbv;
	int ca, cav;
	int clr, npix1;

	if(npix<=0)
		return;

	npix1=npix;
//	npix1=npix+1;
	cr=BGBRASW_PIXEL_R(clr0)<<16;	cg=BGBRASW_PIXEL_G(clr0)<<16;
	cb=BGBRASW_PIXEL_B(clr0)<<16;	ca=BGBRASW_PIXEL_A(clr0)<<16;
	crv=((int)(BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<16)/npix1;
	cgv=((int)(BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<16)/npix1;
	cbv=((int)(BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<16)/npix1;
	cav=((int)(BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<16)/npix1;
//	tz=z0; tzv=((s32)(z1-z0))/npix1;
	tz=z0; tzv=bgbrasw_fixdiv_s32_u16(z1-z0, npix);
	tzv&=BGBRASW_MASK_DEPTH;

	ct=span; cte=span+npix;
	ctz=spanz; ctze=spanz+npix;
	while(ct<cte)
	{
//		tz2=tz&BGBRASW_MASK_DEPTH;
		tz2=tz;
		if(tz2>(*ctz))
		{
			//Z fail
			cr+=crv;	cg+=cgv;
			cb+=cbv;	ca+=cav;
			ct++; ctz++;
			tz+=tzv;
			continue;
		}
		//Z pass
		clr=BGBRASW_MAKEPIXEL(cr>>16, cg>>16, cb>>16, ca>>16);
		cr+=crv;	cg+=cgv;
		cb+=cbv;	ca+=cav;
		*ct++=clr;
		*ctz++=tz2;
//		*ctz++=tz2|(z0&BGBRASW_MASK_STENCIL);
		tz+=tzv;
	}
}

void BGBRASW_DrawSpanTextureBasic(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, int npix,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t)
{
	bgbrasw_pixel *ct, *cte;
	int ts, tt, tsv, ttv, tx, ty;
	int npix1;

	if(npix<=0)return;
	ts=st0s; tt=st0t;
	tsv=(st1s-st0s)/npix;
	ttv=(st1t-st0t)/npix;
//	tsv=bgbrasw_fixdiv_s32_u16(st1s-st0s, npix);
//	ttv=bgbrasw_fixdiv_s32_u16(st1t-st0t, npix);

//	npix1=npix+1;
//	ts=st0s<<8; tt=st0t<<8;
//	tsv=((st1s-st0s)<<8)/npix1;
//	ttv=((st1t-st0t)<<8)/npix1;
	ct=span; cte=span+npix;
	while(ct<cte)
	{
		tx=(ts>>8)&(txs-1);
		ty=(tt>>8)&(tys-1);
//		tx=((ts+128)>>8)&(txs-1);
//		ty=((tt+128)>>8)&(tys-1);
		*ct++=tex[ty*txs+tx];
		ts+=tsv; tt+=ttv;
	}
}

void BGBRASW_DrawSpanTextureInterp(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, int npix,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1)
{
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	int ts, tt, tsv, ttv, tx, ty;
	int cr, crv, crt;
	int cg, cgv, cgt;
	int cb, cbv, cbt;
	int ca, cav, cat;
	int clr, clrt, npix1;

	if(npix<=0)return;

//	npix1=npix+1;
//	npix1=npix;
//	cr=BGBRASW_PIXEL_R(clr0)<<16;	cg=BGBRASW_PIXEL_G(clr0)<<16;
//	cb=BGBRASW_PIXEL_B(clr0)<<16;	ca=BGBRASW_PIXEL_A(clr0)<<16;
//	crv=((int)(BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<16)/npix1;
//	cgv=((int)(BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<16)/npix1;
//	cbv=((int)(BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<16)/npix1;
//	cav=((int)(BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<16)/npix1;

	cr=BGBRASW_PIXEL_R(clr0)<<8;	cg=BGBRASW_PIXEL_G(clr0)<<8;
	cb=BGBRASW_PIXEL_B(clr0)<<8;	ca=BGBRASW_PIXEL_A(clr0)<<8;
	crv=((int)(BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<8)/npix;
	cgv=((int)(BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<8)/npix;
	cbv=((int)(BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<8)/npix;
	cav=((int)(BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<8)/npix;

	ts=st0s; tt=st0t;
	tsv=(st1s-st0s)/npix;
	ttv=(st1t-st0t)/npix;
//	tsv=bgbrasw_fixdiv_s32_u16(st1s-st0s, npix);
//	ttv=bgbrasw_fixdiv_s32_u16(st1t-st0t, npix);

//	ts=st0s<<8; tt=st0t<<8;
//	tsv=((st1s-st0s)<<8)/npix1;
//	ttv=((st1t-st0t)<<8)/npix1;

	ct=span; cte=span+npix;
	while(ct<cte)
	{
		tx=(ts>>8)&(txs-1);
		ty=(tt>>8)&(tys-1);
//		tx=((ts+128)>>8)&(txs-1);
//		ty=((tt+128)>>8)&(tys-1);
		clrt=tex[ty*txs+tx];
//		crt=(BGBRASW_PIXEL_R(clrt)*(cr>>8));
//		cgt=(BGBRASW_PIXEL_G(clrt)*(cg>>8));
//		cbt=(BGBRASW_PIXEL_B(clrt)*(cb>>8));
//		cat=(BGBRASW_PIXEL_A(clrt)*(ca>>8));

#ifdef BGBRASW_SS_YUVA
		clr=BGBRASW_MAKEPIXEL(cr>>8, cg>>8, cb>>8, ca>>8);
		clr=bgbrasw_colormul_yymod(clrt, clr);
#else
		crt=(BGBRASW_PIXEL_R(clrt)*cr);
		cgt=(BGBRASW_PIXEL_G(clrt)*cg);
		cbt=(BGBRASW_PIXEL_B(clrt)*cb);
		cat=(BGBRASW_PIXEL_A(clrt)*ca);
		clr=BGBRASW_MAKEPIXEL(crt>>16, cgt>>16, cbt>>16, cat>>16);
//		clr=clrt;
#endif

		//Z pass
		*ct++=clr;
		cr+=crv;	cg+=cgv;
		cb+=cbv;	ca+=cav;
		ts+=tsv;	tt+=ttv;
	}
}

void BGBRASW_DrawSpanTextureBasicZTest(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t,
	bgbrasw_zbuf z0, bgbrasw_zbuf z1)
{
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	int ts, tt, tsv, ttv, tx, ty, dsv, dtv;
	int txsn1, tysn1;
	u32 tz, tzv, tz2;
	int npix1, npfrac;

	if(npix<=0)return;
	ts=st0s; tt=st0t;
	tz=z0; tzv=bgbrasw_fixdiv_s32_u16(z1-z0, npix);
	tzv&=BGBRASW_MASK_DEPTH;

	tsv=bgbrasw_fixdiv_s32_u16(st1s-st0s, npix);
	ttv=bgbrasw_fixdiv_s32_u16(st1t-st0t, npix);
	txsn1=txs-1;	tysn1=tys-1;

#if 1
	ct=span; cte=span+npix;
	ctz=spanz; ctze=spanz+npix;

	if(tz>(*ctz))
	{
		while(ct<cte)
		{
			if(tz<=(*ctz))
				goto loopPass;
			loopFail:
			//Z fail
			ct++; ctz++;
			ts+=tsv; tt+=ttv; tz+=tzv;
			continue;
		}
	}else
	{
		loopPass:
#if 1
		while((ct+4)<=cte)
		{
			if(tz>(*ctz)) { goto loopFail; }
			tx=(ts>>8)&txsn1; ty=(tt>>8)&tysn1;
			*ct++=tex[ty*txs+tx]; *ctz++=tz;
			ts+=tsv; tt+=ttv; tz+=tzv;

			if(tz>(*ctz)) { goto loopFail; }
			tx=(ts>>8)&txsn1; ty=(tt>>8)&tysn1;
			*ct++=tex[ty*txs+tx]; *ctz++=tz;
			ts+=tsv; tt+=ttv; tz+=tzv;

			if(tz>(*ctz)) { goto loopFail; }
			tx=(ts>>8)&txsn1; ty=(tt>>8)&tysn1;
			*ct++=tex[ty*txs+tx]; *ctz++=tz;
			ts+=tsv; tt+=ttv; tz+=tzv;

			if(tz>(*ctz)) { goto loopFail; }
			tx=(ts>>8)&txsn1; ty=(tt>>8)&tysn1;
			*ct++=tex[ty*txs+tx]; *ctz++=tz;
			ts+=tsv; tt+=ttv; tz+=tzv;
		}
#endif

		while(ct<cte)
		{
			if(tz>(*ctz))
				goto loopFail;
//			loopPass:
			//Z pass
//			tx=(ts>>8)&(txs-1);
//			ty=(tt>>8)&(tys-1);
			tx=(ts>>8)&txsn1;
			ty=(tt>>8)&tysn1;
			*ct++=tex[ty*txs+tx];
			*ctz++=tz;
			ts+=tsv; tt+=ttv; tz+=tzv;
		}
	}
#endif

#if 0
	ct=span; cte=span+npix;
	ctz=spanz; ctze=spanz+npix;
	while(ct<cte)
	{
		if(tz>(*ctz))
		{
			//Z fail
			ct++; ctz++;
			ts+=tsv; tt+=ttv; tz+=tzv;
			continue;
		}
		//Z pass
		tx=(ts>>8)&(txs-1);
		ty=(tt>>8)&(tys-1);
		*ct++=tex[ty*txs+tx];
//		*ctz++=tz2|(z0&BGBRASW_MASK_STENCIL);
		*ctz++=tz;
		ts+=tsv; tt+=ttv; tz+=tzv;
	}
#endif
}

void BGBRASW_DrawSpanTextureInterpZTest(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1,
	bgbrasw_zbuf z0, bgbrasw_zbuf z1)
{
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	u32 tz, tzv;
	u32 tz0, tz1, tz2, tz3;
	int ts, tt, tsv, ttv, tx, ty, dsv, dtv;
	int cr, crv, crt, cg, cgv, cgt;
	int cb, cbv, cbt, ca, cav, cat;
	int txsn1, tysn1;
	int clr, clrt, npix1, npfrac;

	if(npix<=0)return;

	cr=BGBRASW_PIXEL_R(clr0)<<8;	cg=BGBRASW_PIXEL_G(clr0)<<8;
	cb=BGBRASW_PIXEL_B(clr0)<<8;	ca=BGBRASW_PIXEL_A(clr0)<<8;
//	crv=((int)(BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<8)/npix;
//	cgv=((int)(BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<8)/npix;
//	cbv=((int)(BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<8)/npix;
//	cav=((int)(BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<8)/npix;

//	npfrac=256/npix;
	npfrac=bgbrasw_fixdiv_s32_u16(256, npix);
	crv=(BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))*npfrac;
	cgv=(BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))*npfrac;
	cbv=(BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))*npfrac;
	cav=(BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))*npfrac;

//	tz=z0; tzv=((s32)(z1-z0))/npix;
//	tz=z0; tzv=(((s32)(z1-z0))>>8)*npfrac;
	tz=z0; tzv=bgbrasw_fixdiv_s32_u16(z1-z0, npix);
	tzv&=BGBRASW_MASK_DEPTH;

	ts=st0s; tt=st0t;
//	tsv=(st1s-st0s)/npix;
//	ttv=(st1t-st0t)/npix;
//	tsv=((st1s-st0s)*npfrac)>>8;
//	ttv=((st1t-st0t)*npfrac)>>8;
//	tsv=((st1s-st0s)>>8)*npfrac;
//	ttv=((st1t-st0t)>>8)*npfrac;
//	tsv=((st1s-st0s)*npfrac+128)>>8;
//	ttv=((st1t-st0t)*npfrac+128)>>8;

	tsv=bgbrasw_fixdiv_s32_u16(st1s-st0s, npix);
	ttv=bgbrasw_fixdiv_s32_u16(st1t-st0t, npix);
	txsn1=txs-1; tysn1=tys-1;

#if 0
	dsv=st1s-st0s;	dtv=st1t-st0t;
	tx=(dsv^dtv)&0xFFFC000; ty=tx^0xFFFC000;
	if(!tx || !ty)
	{
		npfrac=4096/npix;
		tsv=(dsv*npfrac)>>12;
		ttv=(dtv*npfrac)>>12;
//		tsv=(dsv*npfrac+2048)>>12;
//		ttv=(dtv*npfrac+2048)>>12;
	}else
	{
		tsv=(st1s-st0s)/npix;
		ttv=(st1t-st0t)/npix;
	}
#endif

	ct=span; cte=span+npix;
	ctz=spanz; ctze=spanz+npix;

#if 0
	if(!cav && (ca==0xFF00))
		while((ct+4)<=cte)
	{
		tz0=tz;			tz1=tz+tzv;
		tz2=tz+2*tzv;	tz3=tz+3*tzv;
//		if(tz>(*ctz))
		if((tz0>ctz[0]) && (tz3>ctz[3]) &&
			(tz1>ctz[1]) && (tz2>ctz[2]))
		{
			//Z fail
			cr+=crv<<2;	cg+=cgv<<2;
			cb+=cbv<<2;	ca+=cav<<2;
			ct+=4; ctz+=4;
			ts+=tsv<<2; tt+=ttv<<2; tz+=tzv<<2;

			while((ct+8)<=cte)
			{
				//Pixels 0-3
				tz0=tz;			tz1=tz+tzv;
				tz2=tz+2*tzv;	tz3=tz+3*tzv;
				if((tz0<=ctz[0]) || (tz3<=ctz[3]) ||
					(tz1<=ctz[1]) || (tz2<=ctz[2]))
						break;
				cr+=crv<<2;	cg+=cgv<<2;
				cb+=cbv<<2;	ca+=cav<<2;
				ct+=4; ctz+=4;
				ts+=tsv<<2; tt+=ttv<<2; tz+=tzv<<2;

				//Pixels 4-7
				tz0=tz;			tz1=tz+tzv;
				tz2=tz+2*tzv;	tz3=tz+3*tzv;
				if((tz0<=ctz[0]) || (tz3<=ctz[3]) ||
					(tz1<=ctz[1]) || (tz2<=ctz[2]))
						break;
				cr+=crv<<2;	cg+=cgv<<2;
				cb+=cbv<<2;	ca+=cav<<2;
				ct+=4; ctz+=4;
				ts+=tsv<<2; tt+=ttv<<2; tz+=tzv<<2;
			}
			continue;
		}

		//Pixel 0
		if(tz<=(*ctz))
		{
			tx=(ts>>8)&(txs-1);		ty=(tt>>8)&(tys-1);
			clrt=tex[ty*txs+tx];
//			cat= (((clrt>>16)&0xFF00)*ca)     &0xFF000000;
			cat=0xFF000000;
			crt= (((byte)(clrt>>16))*cr)     &0x00FF0000;
			cgt=((((byte)(clrt>> 8))*cg)>> 8)&0x0000FF00;
			cbt=((((byte) clrt     )*cb)>>16);
			clr=cat|crt|cgt|cbt;
			*ct=clr;	*ctz=tz;
		}
		ct++;		ctz++;
		cr+=crv;	cg+=cgv;	cb+=cbv;	ca+=cav;
		ts+=tsv; tt+=ttv; tz+=tzv;

		//Pixel 1
		if(tz<=(*ctz))
		{
			tx=(ts>>8)&(txs-1);		ty=(tt>>8)&(tys-1);
			clrt=tex[ty*txs+tx];
//			cat= (((clrt>>16)&0xFF00)*ca)     &0xFF000000;
			cat=0xFF000000;
			crt= (((byte)(clrt>>16))*cr)     &0x00FF0000;
			cgt=((((byte)(clrt>> 8))*cg)>> 8)&0x0000FF00;
			cbt=((((byte) clrt     )*cb)>>16);
			clr=cat|crt|cgt|cbt;
			*ct=clr;	*ctz=tz;
		}
		ct++;		ctz++;
		cr+=crv;	cg+=cgv;	cb+=cbv;	ca+=cav;
		ts+=tsv; tt+=ttv; tz+=tzv;

		//Pixel 2
		if(tz<=(*ctz))
		{
			tx=(ts>>8)&(txs-1);		ty=(tt>>8)&(tys-1);
			clrt=tex[ty*txs+tx];
//			cat= (((clrt>>16)&0xFF00)*ca)     &0xFF000000;
			cat=0xFF000000;
			crt= (((byte)(clrt>>16))*cr)     &0x00FF0000;
			cgt=((((byte)(clrt>> 8))*cg)>> 8)&0x0000FF00;
			cbt=((((byte) clrt     )*cb)>>16);
			clr=cat|crt|cgt|cbt;
			*ct=clr;	*ctz=tz;
		}
		ct++;		ctz++;
		cr+=crv;	cg+=cgv;	cb+=cbv;	ca+=cav;
		ts+=tsv; tt+=ttv; tz+=tzv;

		//Pixel 3
		if(tz<=(*ctz))
		{
			tx=(ts>>8)&(txs-1);		ty=(tt>>8)&(tys-1);
			clrt=tex[ty*txs+tx];
//			cat= (((clrt>>16)&0xFF00)*ca)     &0xFF000000;
			cat=0xFF000000;
			crt= (((byte)(clrt>>16))*cr)     &0x00FF0000;
			cgt=((((byte)(clrt>> 8))*cg)>> 8)&0x0000FF00;
			cbt=((((byte) clrt     )*cb)>>16);
			clr=cat|crt|cgt|cbt;
			*ct=clr;	*ctz=tz;
		}
		ct++;		ctz++;
		cr+=crv;	cg+=cgv;	cb+=cbv;	ca+=cav;
		ts+=tsv; tt+=ttv; tz+=tzv;
	}
#endif

#if 1
//	ct=span; cte=span+npix;
//	ctz=spanz; ctze=spanz+npix;
	if(tz>(*ctz))
	{
		//Z fail
		while(ct<cte)
		{
			if(tz<=(*ctz))
				goto loopPass;
			loopFail:
			cr+=crv;	cg+=cgv;
			cb+=cbv;	ca+=cav;
			ct++; ctz++;
			ts+=tsv; tt+=ttv; tz+=tzv;
			continue;
		}
	}else
	{
		loopPass:
#ifndef BGBRASW_SS_YUVA
		while((ct+4)<=cte)
		{
			if(tz>(*ctz))
				goto loopFail;
			tx=(ts>>8)&txsn1;	ty=(tt>>8)&tysn1;
			clrt=tex[ty*txs+tx];
			cat= (((clrt>>16)&0xFF00)*ca)    &0xFF000000;
			crt= (((byte)(clrt>>16))*cr)     &0x00FF0000;
			cgt=((((byte)(clrt>> 8))*cg)>> 8)&0x0000FF00;
			cbt=((((byte) clrt     )*cb)>>16);
			clr=cat|crt|cgt|cbt;
			*ct++=clr;	*ctz++=tz;
			cr+=crv;	cg+=cgv;
			cb+=cbv;	ca+=cav;
			ts+=tsv; tt+=ttv; tz+=tzv;

			if(tz>(*ctz))
				goto loopFail;
			tx=(ts>>8)&txsn1;	ty=(tt>>8)&tysn1;
			clrt=tex[ty*txs+tx];
			cat= (((clrt>>16)&0xFF00)*ca)    &0xFF000000;
			crt= (((byte)(clrt>>16))*cr)     &0x00FF0000;
			cgt=((((byte)(clrt>> 8))*cg)>> 8)&0x0000FF00;
			cbt=((((byte) clrt     )*cb)>>16);
			clr=cat|crt|cgt|cbt;
			*ct++=clr;	*ctz++=tz;
			cr+=crv;	cg+=cgv;
			cb+=cbv;	ca+=cav;
			ts+=tsv; tt+=ttv; tz+=tzv;

			if(tz>(*ctz))
				goto loopFail;
			tx=(ts>>8)&txsn1;	ty=(tt>>8)&tysn1;
			clrt=tex[ty*txs+tx];
			cat= (((clrt>>16)&0xFF00)*ca)    &0xFF000000;
			crt= (((byte)(clrt>>16))*cr)     &0x00FF0000;
			cgt=((((byte)(clrt>> 8))*cg)>> 8)&0x0000FF00;
			cbt=((((byte) clrt     )*cb)>>16);
			clr=cat|crt|cgt|cbt;
			*ct++=clr;	*ctz++=tz;
			cr+=crv;	cg+=cgv;
			cb+=cbv;	ca+=cav;
			ts+=tsv; tt+=ttv; tz+=tzv;

			if(tz>(*ctz))
				goto loopFail;
			tx=(ts>>8)&txsn1;	ty=(tt>>8)&tysn1;
			clrt=tex[ty*txs+tx];
			cat= (((clrt>>16)&0xFF00)*ca)    &0xFF000000;
			crt= (((byte)(clrt>>16))*cr)     &0x00FF0000;
			cgt=((((byte)(clrt>> 8))*cg)>> 8)&0x0000FF00;
			cbt=((((byte) clrt     )*cb)>>16);
			clr=cat|crt|cgt|cbt;
			*ct++=clr;	*ctz++=tz;
			cr+=crv;	cg+=cgv;
			cb+=cbv;	ca+=cav;
			ts+=tsv; tt+=ttv; tz+=tzv;
		}
#endif

		while(ct<cte)
		{
			if(tz>(*ctz))
				goto loopFail;
//			loopPass:
			tx=(ts>>8)&txsn1;
			ty=(tt>>8)&tysn1;
			clrt=tex[ty*txs+tx];

#ifdef BGBRASW_SS_YUVA
			clr=BGBRASW_MAKEPIXEL(cr>>8, cg>>8, cb>>8, ca>>8);
			clr=bgbrasw_colormul_yymod(clrt, clr);
#else
			cat= (((clrt>>16)&0xFF00)*ca)     &0xFF000000;
//			cat=0xFF000000;
			crt= (((byte)(clrt>>16))*cr)     &0x00FF0000;
			cgt=((((byte)(clrt>> 8))*cg)>> 8)&0x0000FF00;
			cbt=((((byte) clrt     )*cb)>>16);
			clr=cat|crt|cgt|cbt;
#endif

			//Z pass
			*ct++=clr;
			*ctz++=tz;

			cr+=crv;	cg+=cgv;
			cb+=cbv;	ca+=cav;
			ts+=tsv; tt+=ttv; tz+=tzv;
		}
	}
#endif

#if 0
//	ct=span; cte=span+npix;
//	ctz=spanz; ctze=spanz+npix;
	while(ct<cte)
	{
		if(tz>(*ctz))
		{
			//Z fail
			while(ct<cte)
			{
				if(tz<=(*ctz))
					goto loopPass;
				loopFail:
				cr+=crv;	cg+=cgv;
				cb+=cbv;	ca+=cav;
				ct++; ctz++;
				ts+=tsv; tt+=ttv; tz+=tzv;
				continue;
			}
		}

		while(ct<cte)
		{
			if(tz>(*ctz))
				goto loopFail;
			loopPass:
			tx=(ts>>8)&(txs-1);
			ty=(tt>>8)&(tys-1);
			clrt=tex[ty*txs+tx];

			cat= (((clrt>>16)&0xFF00)*ca)     &0xFF000000;
//			cat=0xFF000000;
			crt= (((byte)(clrt>>16))*cr)     &0x00FF0000;
			cgt=((((byte)(clrt>> 8))*cg)>> 8)&0x0000FF00;
			cbt=((((byte) clrt     )*cb)>>16);
			clr=cat|crt|cgt|cbt;

			//Z pass
			*ct++=clr;
			*ctz++=tz;

			cr+=crv;	cg+=cgv;
			cb+=cbv;	ca+=cav;
			ts+=tsv; tt+=ttv; tz+=tzv;
		}
	}
#endif

#if 0
//	ct=span; cte=span+npix;
//	ctz=spanz; ctze=spanz+npix;
	while(ct<cte)
	{
		if(tz>(*ctz))
		{
			//Z fail
			cr+=crv;	cg+=cgv;
			cb+=cbv;	ca+=cav;
			ct++; ctz++;
			ts+=tsv; tt+=ttv; tz+=tzv;
			continue;
		}

		tx=(ts>>8)&(txs-1);
		ty=(tt>>8)&(tys-1);
//		tx=((ts+128)>>8)&(txs-1);
//		ty=((tt+128)>>8)&(tys-1);
		clrt=tex[ty*txs+tx];
//		crt=(BGBRASW_PIXEL_R(clrt)*cr);
//		cgt=(BGBRASW_PIXEL_G(clrt)*cg);
//		cbt=(BGBRASW_PIXEL_B(clrt)*cb);
//		cat=(BGBRASW_PIXEL_A(clrt)*ca);
//		clr=BGBRASW_MAKEPIXEL(crt>>16, cgt>>16, cbt>>16, cat>>16);

//		cat=((((clrt>>24)&255)*ca)>>16)<<24;
//		crt=((((clrt>>16)&255)*cr)>>16)<<16;
//		cgt=((((clrt>> 8)&255)*cg)>>16)<<8;

//		cat=((((clrt>>24)&255)*ca)<< 8)&0xFF000000;
		cat= (((clrt>>16)&0xFF00)*ca)     &0xFF000000;
//		cat=0xFF000000;
//		crt= (((clrt>>16)&0x00FF)*cr)     &0x00FF0000;
//		cgt=((((clrt>> 8)&0x00FF)*cg)>> 8)&0x0000FF00;
//		cbt= (((clrt     &0x00FF)*cb)>>16);
		crt= (((byte)(clrt>>16))*cr)     &0x00FF0000;
		cgt=((((byte)(clrt>> 8))*cg)>> 8)&0x0000FF00;
		cbt=((((byte) clrt     )*cb)>>16);
		clr=cat|crt|cgt|cbt;

		//Z pass
		*ct++=clr;
		*ctz++=tz;

		cr+=crv;	cg+=cgv;
		cb+=cbv;	ca+=cav;
		ts+=tsv; tt+=ttv; tz+=tzv;
	}
#endif
}

void BGBRASW_DrawSpanFlatBasicTestBlend(
	BGBRASW_TestBlendData *testData,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix,
	bgbrasw_pixel clr, bgbrasw_zbuf z0, bgbrasw_zbuf z1)
{
	BGBRASW_TestBlendFunc_ft testBlend;
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	u32 tz, tzv, tz2;

	if(npix<=0)
		return;

#ifdef BGBRASW_SS_YUVA
	clr=bgbrasw_pixel2yypixel(clr);
#endif

//	tz=z0; tzv=((s32)(z1-z0))/npix;
	tz=z0; tzv=bgbrasw_fixdiv_s32_u16(z1-z0, npix);
	tzv&=BGBRASW_MASK_DEPTH;

	testBlend=testData->testAndBlend;
	ct=span; cte=span+npix;
	ctz=spanz; ctze=spanz+npix;
	while(ct<cte)
	{
		tz2=tz;
		testBlend(testData, &clr, &tz2, ct, ctz);
		ct++; ctz++; tz+=tzv;
	}
}

void BGBRASW_DrawSpanFlatInterpTestBlend(
	BGBRASW_TestBlendData *testData,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1, bgbrasw_zbuf z0, bgbrasw_zbuf z1)
{
	BGBRASW_TestBlendFunc_ft testBlend;
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	u32 tz, tzv, tz2;
	int cr, crv;
	int cg, cgv;
	int cb, cbv;
	int ca, cav;
	int clr, npix1;

	if(npix<=0)
		return;

	if(clr0==clr1)
	{
		BGBRASW_DrawSpanFlatBasicTestBlend(
			testData, span, spanz, npix, clr0, z0, z1);
		return;
	}

	npix1=npix;
//	npix1=npix+1;
	cr=BGBRASW_PIXEL_R(clr0)<<16;	cg=BGBRASW_PIXEL_G(clr0)<<16;
	cb=BGBRASW_PIXEL_B(clr0)<<16;	ca=BGBRASW_PIXEL_A(clr0)<<16;
	crv=((int)(BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<16)/npix1;
	cgv=((int)(BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<16)/npix1;
	cbv=((int)(BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<16)/npix1;
	cav=((int)(BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<16)/npix1;
//	tz=z0; tzv=((s32)(z1-z0))/npix1;
	tz=z0; tzv=bgbrasw_fixdiv_s32_u16(z1-z0, npix);
	tzv&=BGBRASW_MASK_DEPTH;

	testBlend=testData->testAndBlend;
	ct=span; cte=span+npix;
	ctz=spanz; ctze=spanz+npix;
	while(ct<cte)
	{
		tz2=tz;
		clr=BGBRASW_MAKEPIXEL(cr>>16, cg>>16, cb>>16, ca>>16);
#ifdef BGBRASW_SS_YUVA
		clr=bgbrasw_pixel2yypixel(clr);
#endif
		testBlend(testData, &clr, &tz2, ct, ctz);
		ct++;		ctz++;
		cr+=crv;	cg+=cgv;
		cb+=cbv;	ca+=cav;
		tz+=tzv;
	}
}

void BGBRASW_DrawSpanTextureBasicTestBlend(
	BGBRASW_TestBlendData *testData,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t,
	bgbrasw_zbuf z0, bgbrasw_zbuf z1)
{
	BGBRASW_TestBlendFunc_ft testBlend;
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	u32 tz, tzv, tz2;
	int ts, tt, tsv, ttv, tx, ty;
	int cr, crv, crt;
	int cg, cgv, cgt;
	int cb, cbv, cbt;
	int ca, cav, cat;
	int clr, clrt, npix1;

	if(npix<=0)return;

//	npix1=npix+1;
	npix1=npix;
//	tz=z0; tzv=((s32)(z1-z0))/npix1;
	tz=z0; tzv=bgbrasw_fixdiv_s32_u16(z1-z0, npix);
	tzv&=BGBRASW_MASK_DEPTH;

	ts=st0s; tt=st0t;
	tsv=(st1s-st0s)/npix;
	ttv=(st1t-st0t)/npix;
//	tsv=bgbrasw_fixdiv_s32_u16(st1s-st0s, npix);
//	ttv=bgbrasw_fixdiv_s32_u16(st1t-st0t, npix);

	testBlend=testData->testAndBlend;
	ct=span; cte=span+npix;
	ctz=spanz; ctze=spanz+npix;
	while(ct<cte)
	{
		tz2=tz;
		tx=(ts>>8)&(txs-1);
		ty=(tt>>8)&(tys-1);
//		tx=((ts+128)>>8)&(txs-1);
//		ty=((tt+128)>>8)&(tys-1);
		clr=tex[ty*txs+tx];

		testBlend(testData, &clr, &tz2, ct, ctz);
		ct++; ctz++;
		ts+=tsv;	tt+=ttv;
		tz+=tzv;
	}
}

void BGBRASW_DrawSpanTextureInterpTestBlend(
	BGBRASW_TestBlendData *testData,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1,
	bgbrasw_zbuf z0, bgbrasw_zbuf z1)
{
	BGBRASW_TestBlendFunc_ft testBlend;
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	u32 tz, tzv, tz2;
	int ts, tt, tsv, ttv, tx, ty;
	int cr, crv, crt;
	int cg, cgv, cgt;
	int cb, cbv, cbt;
	int ca, cav, cat;
	int clr, clrt, npix1, npfrac;

	if(npix<=0)return;

	if((clr0==clr1) && (clr0==0xFFFFFFFF))
	{
		BGBRASW_DrawSpanTextureBasicTestBlend(
			testData, span, spanz, npix,
			tex, txs, tys,
			st0s, st0t, st1s, st1t,
			z0, z1);
		return;
	}

//	npix1=npix+1;
	npix1=npix;
#if 0
	cr=BGBRASW_PIXEL_R(clr0)<<8;	cg=BGBRASW_PIXEL_G(clr0)<<8;
	cb=BGBRASW_PIXEL_B(clr0)<<8;	ca=BGBRASW_PIXEL_A(clr0)<<8;
	crv=((int)(BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<8)/npix1;
	cgv=((int)(BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<8)/npix1;
	cbv=((int)(BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<8)/npix1;
	cav=((int)(BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<8)/npix1;
#endif
#if 0
	cr=BGBRASW_PIXEL_R(clr0)<<16;	cg=BGBRASW_PIXEL_G(clr0)<<16;
	cb=BGBRASW_PIXEL_B(clr0)<<16;	ca=BGBRASW_PIXEL_A(clr0)<<16;
	crv=((int)(BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<16)/npix1;
	cgv=((int)(BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<16)/npix1;
	cbv=((int)(BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<16)/npix1;
	cav=((int)(BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<16)/npix1;
#endif
#if 1
	npfrac=bgbrasw_fixdiv_s32_u16(65536, npix);
	cr=BGBRASW_PIXEL_R(clr0)<<8;	cg=BGBRASW_PIXEL_G(clr0)<<8;
	cb=BGBRASW_PIXEL_B(clr0)<<8;	ca=BGBRASW_PIXEL_A(clr0)<<8;
	crv=((BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))*npfrac)>>8;
	cgv=((BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))*npfrac)>>8;
	cbv=((BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))*npfrac)>>8;
	cav=((BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))*npfrac)>>8;
#endif
//	tz=z0; tzv=((s32)(z1-z0))/npix1;
	tz=z0; tzv=bgbrasw_fixdiv_s32_u16(z1-z0, npix);
	tzv&=BGBRASW_MASK_DEPTH;

	ts=st0s; tt=st0t;
	tsv=(st1s-st0s)/npix;
	ttv=(st1t-st0t)/npix;
//	tsv=bgbrasw_fixdiv_s32_u16(st1s-st0s, npix);
//	ttv=bgbrasw_fixdiv_s32_u16(st1t-st0t, npix);

//	ts=st0s<<8; tt=st0t<<8;
//	tsv=((st1s-st0s)<<8)/npix1;
//	ttv=((st1t-st0t)<<8)/npix1;

	testBlend=testData->testAndBlend;
	ct=span; cte=span+npix;
	ctz=spanz; ctze=spanz+npix;
	while(ct<cte)
	{
//		clr=BGBRASW_MAKEPIXEL(cr>>16, cg>>16, cb>>16, ca>>16);
//		tz2=(tz&BGBRASW_MASK_DEPTH)|(z0&BGBRASW_MASK_STENCIL);
		tz2=tz;

		tx=(ts>>8)&(txs-1);
		ty=(tt>>8)&(tys-1);
//		tx=((ts+128)>>8)&(txs-1);
//		ty=((tt+128)>>8)&(tys-1);
		clrt=tex[ty*txs+tx];

#ifdef BGBRASW_SS_YUVA
		clr=BGBRASW_MAKEPIXEL(cr>>8, cg>>8, cb>>8, ca>>8);
		clr=bgbrasw_colormul_yymod(clrt, clr);
#else
//		crt=(BGBRASW_PIXEL_R(clrt)*(cr>>8));
//		cgt=(BGBRASW_PIXEL_G(clrt)*(cg>>8));
//		cbt=(BGBRASW_PIXEL_B(clrt)*(cb>>8));
//		cat=(BGBRASW_PIXEL_A(clrt)*(ca>>8));
		crt=(BGBRASW_PIXEL_R(clrt)*cr);
		cgt=(BGBRASW_PIXEL_G(clrt)*cg);
		cbt=(BGBRASW_PIXEL_B(clrt)*cb);
		cat=(BGBRASW_PIXEL_A(clrt)*ca);

//		clr=BGBRASW_MAKEPIXEL(crt>>8, cgt>>8, cbt>>8, cat>>8);
		clr=BGBRASW_MAKEPIXEL(crt>>16, cgt>>16, cbt>>16, cat>>16);
#endif

		testBlend(testData, &clr, &tz2, ct, ctz);
//		testData->testAndBlend(testData, &clr, &tz2, ct, ctz);
		ct++; ctz++;

		cr+=crv;	cg+=cgv;
		cb+=cbv;	ca+=cav;
		ts+=tsv;	tt+=ttv;
		tz+=tzv;
	}
}

bgbrasw_pixel bgbrasw_interp_linear(
	bgbrasw_pixel clr0, bgbrasw_pixel clr1,
	bgbrasw_pixel clr2, bgbrasw_pixel clr3,
	int fx, int fy)
{
	bgbrasw_pixel clr0ag, clr0rb;
	bgbrasw_pixel clr1ag, clr1rb;
	bgbrasw_pixel clr2ag, clr2rb;
	bgbrasw_pixel clr3ag, clr3rb;
	bgbrasw_pixel clr4ag, clr4rb;
	bgbrasw_pixel clr5ag, clr5rb;
	bgbrasw_pixel clr6ag, clr6rb;
	bgbrasw_pixel clr7ag, clr7rb;
	bgbrasw_pixel clr;
	int nx, ny;

	nx=255-fx;	ny=255-fy;
	clr0ag=(clr0>>8)&0x00FF00FF;	clr0rb=(clr0   )&0x00FF00FF;
	clr1ag=(clr1>>8)&0x00FF00FF;	clr1rb=(clr1   )&0x00FF00FF;
	clr2ag=(clr2>>8)&0x00FF00FF;	clr2rb=(clr2   )&0x00FF00FF;
	clr3ag=(clr3>>8)&0x00FF00FF;	clr3rb=(clr3   )&0x00FF00FF;
	clr4ag=nx*clr0ag+fx*clr1ag;		clr4rb=nx*clr0rb+fx*clr1rb;
	clr5ag=nx*clr2ag+fx*clr3ag;		clr5rb=nx*clr2rb+fx*clr3rb;
	clr4ag=(clr4ag>>8)&0x00FF00FF;	clr4rb=(clr4rb>>8)&0x00FF00FF;
	clr5ag=(clr5ag>>8)&0x00FF00FF;	clr5rb=(clr5rb>>8)&0x00FF00FF;
	clr6ag=ny*clr4ag+fy*clr5ag;		clr6rb=ny*clr4rb+fy*clr5rb;
//	clr=(clr6ag&0xFF00FF00UL)|(clr6rb&0x00FF00FFUL);
	clr=(clr6ag&0xFF00FF00UL)|((clr6rb>>8)&0x00FF00FFUL);
	return(clr);
}

void BGBRASW_DrawSpanTextureLinearInterpTestBlend(
	BGBRASW_TestBlendData *testData,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1,
	bgbrasw_zbuf z0, bgbrasw_zbuf z1)
{
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	bgbrasw_pixel tclr0, tclr1, tclr2, tclr3;
	u32 tz, tzv, tz2;
	int ts, tt, tsv, ttv;
	int tx, ty;
	int tx0, ty0, tx1, ty1;
	int tx2, ty2, tx3, ty3;
	int cr, crv, crt;
	int cg, cgv, cgt;
	int cb, cbv, cbt;
	int ca, cav, cat;
	int clr, clrt, npix1;

	if(npix<=0)return;

//	npix1=npix+1;
	npix1=npix;
	cr=BGBRASW_PIXEL_R(clr0)<<8;	cg=BGBRASW_PIXEL_G(clr0)<<8;
	cb=BGBRASW_PIXEL_B(clr0)<<8;	ca=BGBRASW_PIXEL_A(clr0)<<8;
	crv=((int)(BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<8)/npix;
	cgv=((int)(BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<8)/npix;
	cbv=((int)(BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<8)/npix;
	cav=((int)(BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<8)/npix;
//	tz=z0; tzv=((s32)(z1-z0))/npix;
	tz=z0; tzv=bgbrasw_fixdiv_s32_u16(z1-z0, npix);
	tzv&=BGBRASW_MASK_DEPTH;

	ts=st0s; tt=st0t;
//	tsv=(st1s-st0s)/npix;
//	ttv=(st1t-st0t)/npix;
	tsv=bgbrasw_fixdiv_s32_u16(st1s-st0s, npix);
	ttv=bgbrasw_fixdiv_s32_u16(st1t-st0t, npix);

//	ts=st0s<<8; tt=st0t<<8;
//	tsv=((st1s-st0s)<<8)/npix1;
//	ttv=((st1t-st0t)<<8)/npix1;

	ct=span; cte=span+npix;
	ctz=spanz; ctze=spanz+npix;
	while(ct<cte)
	{
//		clr=BGBRASW_MAKEPIXEL(cr>>16, cg>>16, cb>>16, ca>>16);
//		tz2=(tz&BGBRASW_MASK_DEPTH)|(z0&BGBRASW_MASK_STENCIL);
		tz2=tz;

		tx=ts>>8;	ty=tt>>8;
		tx0=(tx  )&(txs-1);	ty0=(ty  )&(tys-1);
		tx1=(tx+1)&(txs-1);	ty1=(ty  )&(tys-1);
		tx2=(tx  )&(txs-1);	ty2=(ty+1)&(tys-1);
		tx3=(tx+1)&(txs-1);	ty3=(ty+1)&(tys-1);
		tclr0=tex[ty0*txs+tx0];	tclr1=tex[ty1*txs+tx1];
		tclr2=tex[ty2*txs+tx2];	tclr3=tex[ty3*txs+tx3];
//		clrt=tclr0;
		clrt=bgbrasw_interp_linear(
			tclr0, tclr1, tclr2, tclr3,
			ts&255, tt&255);
		
//		tx=(ts>>8)&(txs-1);
//		ty=(tt>>8)&(tys-1);
//		tx=((ts+128)>>8)&(txs-1);
//		ty=((tt+128)>>8)&(tys-1);
//		clrt=tex[ty*txs+tx];
//		crt=(BGBRASW_PIXEL_R(clrt)*(cr>>8));
//		cgt=(BGBRASW_PIXEL_G(clrt)*(cg>>8));
//		cbt=(BGBRASW_PIXEL_B(clrt)*(cb>>8));
//		cat=(BGBRASW_PIXEL_A(clrt)*(ca>>8));
		crt=(BGBRASW_PIXEL_R(clrt)*cr);
		cgt=(BGBRASW_PIXEL_G(clrt)*cg);
		cbt=(BGBRASW_PIXEL_B(clrt)*cb);
		cat=(BGBRASW_PIXEL_A(clrt)*ca);

		clr=BGBRASW_MAKEPIXEL(crt>>16, cgt>>16, cbt>>16, cat>>16);

		testData->testAndBlend(testData, &clr, &tz2, ct, ctz);
		ct++; ctz++;

		cr+=crv;	cg+=cgv;
		cb+=cbv;	ca+=cav;
		ts+=tsv;	tt+=ttv;
		tz+=tzv;
	}
}
