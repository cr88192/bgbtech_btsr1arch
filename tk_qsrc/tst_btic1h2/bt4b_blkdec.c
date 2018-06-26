/* 8x8x3+UV4x4x2 */
void BTIC4B_DecBlock0BGRA(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	int ytab[8], utab[4], vtab[4];
	byte *ct0, *ct1;
	int acy, acu, acv, dcy, dcu, dcv;
	int mcy, mcu, mcv, ncy, ncu, ncv;

	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int cy0, cy1, cy2, cy3;
	int cy, cu, cv, cu1, cv1;

	u32 px0, px1, px2, px3;
	u32 pxy0, pxy1, pxu, pxv;
	int i, j, k;
	
	acy=*(s16 *)(blkbuf+ 4);
	acu=*(s16 *)(blkbuf+ 6);
	acv=*(s16 *)(blkbuf+ 8);
	dcy=*(s16 *)(blkbuf+10);
	dcu=*(s16 *)(blkbuf+12);
	dcv=*(s16 *)(blkbuf+14);
	
	mcy=acy-(dcy>>1); ncy=mcy+dcy;
	mcu=acu-(dcu>>1); ncu=mcu+dcu;
	mcv=acv-(dcv>>1); ncv=mcv+dcv;

	ytab[0]=mcy;
	ytab[1]=(14*mcy+ 2*ncy)>>4;
	ytab[2]=(11*mcy+ 5*ncy)>>4;
	ytab[3]=( 9*mcy+ 7*ncy)>>4;
	ytab[4]=( 7*mcy+ 9*ncy)>>4;
	ytab[5]=( 5*mcy+11*ncy)>>4;
	ytab[6]=( 2*mcy+14*ncy)>>4;
	ytab[7]=ncy;

	utab[0]=mcu;
	utab[1]=(11*mcu+ 5*ncu)>>4;
	utab[2]=( 5*mcu+11*ncu)>>4;
	utab[3]=ncu;

	vtab[0]=mcv;
	vtab[1]=(11*mcv+ 5*ncv)>>4;
	vtab[2]=( 5*mcv+11*ncv)>>4;
	vtab[3]=ncv;

//	pxu=*(u32 *)(blkbuf+40);
//	pxv=*(u32 *)(blkbuf+44);
	pxu=btic4b_getu32le(blkbuf+40);
	pxv=btic4b_getu32le(blkbuf+44);

	for(i=0; i<4; i++)
	{
		ct0=ibuf+(i*2+0)*ystr;
		ct1=ibuf+(i*2+1)*ystr;
//		pxy0=*(u32 *)(blkbuf+16+i*6+0);
//		pxy1=*(u32 *)(blkbuf+16+i*6+3);
		pxy0=btic4b_getu32le(blkbuf+16+i*6+0);
		pxy1=btic4b_getu32le(blkbuf+16+i*6+3);
		
		for(j=0; j<4; j++)
		{
			k=j*6;
			cy0=ytab[(pxy0>>(k+0))&7];
			cy1=ytab[(pxy0>>(k+3))&7];
			cy2=ytab[(pxy1>>(k+0))&7];
			cy3=ytab[(pxy1>>(k+3))&7];

			k=(i*4+j)*2;
			cu=utab[(pxu>>k)&3];
			cv=vtab[(pxv>>k)&3];

			ctx->ClrDec4(cy0, cy1, cy2, cy3, cu, cv,
				(u32 *)ct0, (u32 *)ct1);
			ct0+=8;		ct1+=8;
		}
	}
}

force_inline void lqtvq_fillu32_8x1(u32 *px, u32 v)
{
#ifdef BT4B_XMMINTRIN
// #if 0
	__m128i x0;
	x0=_mm_set_epi32(v, v, v, v);
	_mm_storeu_si128((__m128i *)(px+0), x0);
	_mm_storeu_si128((__m128i *)(px+4), x0);
#else
	px[0]=v; px[1]=v; px[2]=v; px[3]=v;
	px[4]=v; px[5]=v; px[6]=v; px[7]=v;
#endif
}

force_inline void lqtvq_fillu32_8x1_2x1(u32 *px, u32 u, u32 v)
{
#ifdef BT4B_XMMINTRIN
// #if 0
	__m128i x0, x1;
	x0=_mm_set_epi32(u, u, u, u);
	x1=_mm_set_epi32(v, v, v, v);
	_mm_storeu_si128((__m128i *)(px+0), x0);
	_mm_storeu_si128((__m128i *)(px+4), x1);
//	_mm_storeu_si128((__m128i *)(px+0), _mm_set_epi32(u, u, u, u));
//	_mm_storeu_si128((__m128i *)(px+4), _mm_set_epi32(v, v, v, v));
#else
	px[0]=u; px[1]=u; px[2]=u; px[3]=u;
	px[4]=v; px[5]=v; px[6]=v; px[7]=v;
#endif
}

force_inline void lqtvq_fillu32_8x1_4x1(u32 *px,
	u32 v0, u32 v1, u32 v2, u32 v3)
{
// #ifdef BT4B_XMMINTRIN
#if 0
	__m128i x0, x1;
	x0=_mm_set_epi32(v1, v1, v0, v0);
	x1=_mm_set_epi32(v3, v3, v2, v2);
	_mm_storeu_si128((__m128i *)(px+0), x0);
	_mm_storeu_si128((__m128i *)(px+4), x1);
#else
	px[0]=v0; px[1]=v0; px[2]=v1; px[3]=v1;
	px[4]=v2; px[5]=v2; px[6]=v3; px[7]=v3;
#endif
}

force_inline void lqtvq_fillu32_8x1_8x1(u32 *px,
	u32 v0, u32 v1, u32 v2, u32 v3,
	u32 v4, u32 v5, u32 v6, u32 v7)
{
	px[0]=v0; px[1]=v1; px[2]=v2; px[3]=v3;
	px[4]=v4; px[5]=v5; px[6]=v6; px[7]=v7;
}

force_inline void lqtvq_fillu32_4x1(u32 *px, u32 v)
{
#ifdef BT4B_XMMINTRIN
	__m128i x0;
	x0=_mm_set_epi32(v, v, v, v);
	_mm_storeu_si128((__m128i *)(px+0), x0);
#else
	px[0]=v; px[1]=v; px[2]=v; px[3]=v;
#endif
}


void BTIC4B_DecBlockFlatBGRA(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	byte *ct0, *ct1;
	byte *ct2, *ct3;
	int acy, acu, acv, dcy, dcu, dcv;
	int mcy, mcu, mcv, ncy, ncu, ncv;

	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int cy0, cy1, cy2, cy3;
	int cy, cu, cv, cu1, cv1;

	u32 pxy0, pxy1, pxu, pxv;
	int cr, cg, cb;
	int fl;
	int i, j, k;
	
	acy=*(s16 *)(blkbuf+ 4);
	acu=*(s16 *)(blkbuf+ 6);
	acv=*(s16 *)(blkbuf+ 8);

	pxu=ctx->ClrDec1(acy, acu, acv);

#if 0
	fl=blkbuf[28];
	if(fl)
//	if(0)
	{
		if(fl&4)
//		if(0)
		{
			lqtvq_fillu32_4x1((u32 *)(ibuf+0*ystr+8*4), pxu);
			lqtvq_fillu32_4x1((u32 *)(ibuf+1*ystr+8*4), pxu);
			lqtvq_fillu32_4x1((u32 *)(ibuf+2*ystr+8*4), pxu);
			lqtvq_fillu32_4x1((u32 *)(ibuf+3*ystr+8*4), pxu);
			lqtvq_fillu32_4x1((u32 *)(ibuf+4*ystr+8*4), pxu);
			lqtvq_fillu32_4x1((u32 *)(ibuf+5*ystr+8*4), pxu);
			lqtvq_fillu32_4x1((u32 *)(ibuf+6*ystr+8*4), pxu);
			lqtvq_fillu32_4x1((u32 *)(ibuf+7*ystr+8*4), pxu);
		}

		if(fl&8)
		{
			lqtvq_fillu32_8x1((u32 *)(ibuf+ 8*ystr), pxu);
			lqtvq_fillu32_8x1((u32 *)(ibuf+ 9*ystr), pxu);
			lqtvq_fillu32_8x1((u32 *)(ibuf+10*ystr), pxu);
			lqtvq_fillu32_8x1((u32 *)(ibuf+11*ystr), pxu);
		}

		lqtvq_fillu32_4x1((u32 *)(ibuf+4*ystr+4*4), pxu);
		lqtvq_fillu32_4x1((u32 *)(ibuf+5*ystr+4*4), pxu);
		lqtvq_fillu32_4x1((u32 *)(ibuf+6*ystr+4*4), pxu);
		lqtvq_fillu32_4x1((u32 *)(ibuf+7*ystr+4*4), pxu);
		
		cr=(pxu>>16)&255;
		cg=(pxu>> 8)&255;
		cb=(pxu    )&255;
		
//		if(fl&16)
		if(0)
		{
			for(i=0; i<8; i++)
			{
				ibuf[i*ystr+ 0]=(3*ibuf[i*ystr+ 0]+  cb)>>2;
				ibuf[i*ystr+ 1]=(3*ibuf[i*ystr+ 1]+  cg)>>2;
				ibuf[i*ystr+ 2]=(3*ibuf[i*ystr+ 2]+  cr)>>2;
				ibuf[i*ystr+ 4]=(  ibuf[i*ystr+ 4]+  cb)>>1;
				ibuf[i*ystr+ 5]=(  ibuf[i*ystr+ 5]+  cg)>>1;
				ibuf[i*ystr+ 6]=(  ibuf[i*ystr+ 6]+  cr)>>1;
				ibuf[i*ystr+ 8]=(  ibuf[i*ystr+ 8]+3*cb)>>2;
				ibuf[i*ystr+ 9]=(  ibuf[i*ystr+ 9]+3*cg)>>2;
				ibuf[i*ystr+10]=(  ibuf[i*ystr+10]+3*cr)>>2;
				*(u32 *)(ibuf+i*ystr+12)=pxu;
			}
		}else
		{
			lqtvq_fillu32_4x1((u32 *)(ibuf+0*ystr), pxu);
			lqtvq_fillu32_4x1((u32 *)(ibuf+1*ystr), pxu);
			lqtvq_fillu32_4x1((u32 *)(ibuf+2*ystr), pxu);
			lqtvq_fillu32_4x1((u32 *)(ibuf+3*ystr), pxu);
			lqtvq_fillu32_4x1((u32 *)(ibuf+4*ystr), pxu);
			lqtvq_fillu32_4x1((u32 *)(ibuf+5*ystr), pxu);
			lqtvq_fillu32_4x1((u32 *)(ibuf+6*ystr), pxu);
			lqtvq_fillu32_4x1((u32 *)(ibuf+7*ystr), pxu);
		}

//		if(fl&32)
		if(0)
		{
			for(i=0; i<4; i++)
			{
				j=(i+4)*4;
				ibuf[0*ystr+j+ 0]=(3*ibuf[0*ystr+j+ 0]+  cb)>>2;
				ibuf[0*ystr+j+ 1]=(3*ibuf[0*ystr+j+ 1]+  cg)>>2;
				ibuf[0*ystr+j+ 2]=(3*ibuf[0*ystr+j+ 2]+  cr)>>2;
				ibuf[1*ystr+j+ 4]=(  ibuf[1*ystr+j+ 4]+  cb)>>1;
				ibuf[1*ystr+j+ 5]=(  ibuf[1*ystr+j+ 5]+  cg)>>1;
				ibuf[1*ystr+j+ 6]=(  ibuf[1*ystr+j+ 6]+  cr)>>1;
				ibuf[2*ystr+j+ 8]=(  ibuf[2*ystr+j+ 8]+3*cb)>>2;
				ibuf[2*ystr+j+ 9]=(  ibuf[2*ystr+j+ 9]+3*cg)>>2;
				ibuf[2*ystr+j+10]=(  ibuf[2*ystr+j+10]+3*cr)>>2;
				*(u32 *)(ibuf+3*ystr+j)=pxu;
			}
		}else
		{
			lqtvq_fillu32_4x1((u32 *)(ibuf+0*ystr+4*4), pxu);
			lqtvq_fillu32_4x1((u32 *)(ibuf+1*ystr+4*4), pxu);
			lqtvq_fillu32_4x1((u32 *)(ibuf+2*ystr+4*4), pxu);
			lqtvq_fillu32_4x1((u32 *)(ibuf+3*ystr+4*4), pxu);
		}
		
		return;
	}
#endif

#if 1
	lqtvq_fillu32_8x1((u32 *)(ibuf+0*ystr), pxu);
	lqtvq_fillu32_8x1((u32 *)(ibuf+1*ystr), pxu);
	lqtvq_fillu32_8x1((u32 *)(ibuf+2*ystr), pxu);
	lqtvq_fillu32_8x1((u32 *)(ibuf+3*ystr), pxu);
	lqtvq_fillu32_8x1((u32 *)(ibuf+4*ystr), pxu);
	lqtvq_fillu32_8x1((u32 *)(ibuf+5*ystr), pxu);
	lqtvq_fillu32_8x1((u32 *)(ibuf+6*ystr), pxu);
	lqtvq_fillu32_8x1((u32 *)(ibuf+7*ystr), pxu);
#endif
}

void BTIC4B_DecBlockFlatBGRA_FA(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr, int ca)
{
#ifdef BT4B_XMMINTRIN
	__m128i x0;
#endif
	byte *ct0, *ct1;
	byte *ct2, *ct3;
	int acy, acu, acv, dcy, dcu, dcv;
	int mcy, mcu, mcv, ncy, ncu, ncv;

	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int cy0, cy1, cy2, cy3;
	int cy, cu, cv, cu1, cv1;

	u32 pxy0, pxy1, pxu, pxv;
	int i, j, k;
	
	acy=*(s16 *)(blkbuf+ 4);
	acu=*(s16 *)(blkbuf+ 6);
	acv=*(s16 *)(blkbuf+ 8);

	pxu=ctx->ClrDec1A(acy, acu, acv, ca);

#ifdef BT4B_XMMINTRIN
	x0=_mm_set_epi32(pxu, pxu, pxu, pxu);
	ct0=ibuf+0*ystr;	ct1=ibuf+1*ystr;
	ct2=ibuf+2*ystr;	ct3=ibuf+3*ystr;
	_mm_storeu_si128((__m128i *)(ct0+ 0), x0);
	_mm_storeu_si128((__m128i *)(ct0+16), x0);
	_mm_storeu_si128((__m128i *)(ct1+ 0), x0);
	_mm_storeu_si128((__m128i *)(ct1+16), x0);
	_mm_storeu_si128((__m128i *)(ct2+ 0), x0);
	_mm_storeu_si128((__m128i *)(ct2+16), x0);
	_mm_storeu_si128((__m128i *)(ct3+ 0), x0);
	_mm_storeu_si128((__m128i *)(ct3+16), x0);
	ct0=ibuf+4*ystr;	ct1=ibuf+5*ystr;
	ct2=ibuf+6*ystr;	ct3=ibuf+7*ystr;
	_mm_storeu_si128((__m128i *)(ct0+ 0), x0);
	_mm_storeu_si128((__m128i *)(ct0+16), x0);
	_mm_storeu_si128((__m128i *)(ct1+ 0), x0);
	_mm_storeu_si128((__m128i *)(ct1+16), x0);
	_mm_storeu_si128((__m128i *)(ct2+ 0), x0);
	_mm_storeu_si128((__m128i *)(ct2+16), x0);
	_mm_storeu_si128((__m128i *)(ct3+ 0), x0);
	_mm_storeu_si128((__m128i *)(ct3+16), x0);
#else
	lqtvq_fillu32_8x1((u32 *)(ibuf+0*ystr), pxu);
	lqtvq_fillu32_8x1((u32 *)(ibuf+1*ystr), pxu);
	lqtvq_fillu32_8x1((u32 *)(ibuf+2*ystr), pxu);
	lqtvq_fillu32_8x1((u32 *)(ibuf+3*ystr), pxu);
	lqtvq_fillu32_8x1((u32 *)(ibuf+4*ystr), pxu);
	lqtvq_fillu32_8x1((u32 *)(ibuf+5*ystr), pxu);
	lqtvq_fillu32_8x1((u32 *)(ibuf+6*ystr), pxu);
	lqtvq_fillu32_8x1((u32 *)(ibuf+7*ystr), pxu);
#endif
}

void BTIC4B_DecBlockFlatHalfBGRA_FA(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr, int ca)
{
#ifdef BT4B_XMMINTRIN
	__m128i x0;
#endif
	byte *ct0, *ct1;
	byte *ct2, *ct3;
	int acy, acu, acv, dcy, dcu, dcv;
	int mcy, mcu, mcv, ncy, ncu, ncv;

	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int cy0, cy1, cy2, cy3;
	int cy, cu, cv, cu1, cv1;

	u32 pxy0, pxy1, pxu, pxv;
	int i, j, k;
	
	acy=*(s16 *)(blkbuf+ 4);
	acu=*(s16 *)(blkbuf+ 6);
	acv=*(s16 *)(blkbuf+ 8);

	pxu=ctx->ClrDec1A(acy, acu, acv, ca);

#ifdef BT4B_XMMINTRIN
	x0=_mm_set_epi32(pxu, pxu, pxu, pxu);
	ct0=ibuf+0*ystr;	ct1=ibuf+1*ystr;
	ct2=ibuf+2*ystr;	ct3=ibuf+3*ystr;
	_mm_storeu_si128((__m128i *)(ct0), x0);
	_mm_storeu_si128((__m128i *)(ct1), x0);
	_mm_storeu_si128((__m128i *)(ct2), x0);
	_mm_storeu_si128((__m128i *)(ct3), x0);
	ct0=ibuf+4*ystr;	ct1=ibuf+5*ystr;
	ct2=ibuf+6*ystr;	ct3=ibuf+7*ystr;
	_mm_storeu_si128((__m128i *)(ct0), x0);
	_mm_storeu_si128((__m128i *)(ct1), x0);
	_mm_storeu_si128((__m128i *)(ct2), x0);
	_mm_storeu_si128((__m128i *)(ct3), x0);
#else
	lqtvq_fillu32_4x1((u32 *)(ibuf+0*ystr), pxu);
	lqtvq_fillu32_4x1((u32 *)(ibuf+1*ystr), pxu);
	lqtvq_fillu32_4x1((u32 *)(ibuf+2*ystr), pxu);
	lqtvq_fillu32_4x1((u32 *)(ibuf+3*ystr), pxu);
	lqtvq_fillu32_4x1((u32 *)(ibuf+4*ystr), pxu);
	lqtvq_fillu32_4x1((u32 *)(ibuf+5*ystr), pxu);
	lqtvq_fillu32_4x1((u32 *)(ibuf+6*ystr), pxu);
	lqtvq_fillu32_4x1((u32 *)(ibuf+7*ystr), pxu);
#endif
}

/* XxXx2 */
void BTIC4B_DecBlock1BGRA(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
//	int ytab[8], utab[4], vtab[4];
	u32 rgbtab[4];
	byte *ct0, *ct1;
	byte *ct2, *ct3;
	int acy, acu, acv, dcy, dcu, dcv;
	int mcy, mcu, mcv, ncy, ncu, ncv;

	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int cy0, cy1, cy2, cy3;
	int cy, cu, cv, cu1, cv1;

	u32 px0, px1, px2, px3;
	u32 px4, px5, px6, px7;
	u32 px8, px9, pxA, pxB;
	u32 pxC, pxD, pxE, pxF;

	u32 pxy0, pxy1, pxu, pxv;
	int i, j, k;
	
	acy=*(s16 *)(blkbuf+ 4);
	acu=*(s16 *)(blkbuf+ 6);
	acv=*(s16 *)(blkbuf+ 8);
	dcy=*(s16 *)(blkbuf+10);
	dcu=*(s16 *)(blkbuf+12);
	dcv=*(s16 *)(blkbuf+14);

	ctx->ClrDec2T(blkbuf[0],
		acy, acu, acv, dcy, dcu, dcv,
		&cr0, &cg0, &cb0, &cr3, &cg3, &cb3);

	ctx->ClrDecPack4(cr0, cg0, cb0,
		cr3, cg3, cb3, 255, rgbtab);

//	cr1=(11*cr0+ 5*cr3)>>4;
//	cg1=(11*cg0+ 5*cg3)>>4;
//	cb1=(11*cb0+ 5*cb3)>>4;
//	cr2=( 5*cr0+11*cr3)>>4;
//	cg2=( 5*cg0+11*cg3)>>4;
//	cb2=( 5*cb0+11*cb3)>>4;

//	rgbtab[0]=0xFF000000|(cr0<<16)|(cg0<<8)|cb0;
//	rgbtab[1]=0xFF000000|(cr1<<16)|(cg1<<8)|cb1;
//	rgbtab[2]=0xFF000000|(cr2<<16)|(cg2<<8)|cb2;
//	rgbtab[3]=0xFF000000|(cr3<<16)|(cg3<<8)|cb3;

//	rgbtab[0]=ctx->ClrDecPack1A(cr0, cg0, cb0, 255);
//	rgbtab[1]=ctx->ClrDecPack1A(cr1, cg1, cb1, 255);
//	rgbtab[2]=ctx->ClrDecPack1A(cr2, cg2, cb2, 255);
//	rgbtab[3]=ctx->ClrDecPack1A(cr3, cg3, cb3, 255);

	switch(blkbuf[0]&0x1F)
	{
	case 0x09:
		pxy0=blkbuf[16];
		for(i=0; i<2; i++)
		{
			pxu=rgbtab[(pxy0>>(i*4+0))&3];
			pxv=rgbtab[(pxy0>>(i*4+2))&3];
			ct0=ibuf+(i*4+0)*ystr;	ct1=ibuf+(i*4+1)*ystr;
			ct2=ibuf+(i*4+2)*ystr;	ct3=ibuf+(i*4+3)*ystr;
			lqtvq_fillu32_8x1_2x1((u32 *)ct0, pxu, pxv);
			lqtvq_fillu32_8x1_2x1((u32 *)ct1, pxu, pxv);
			lqtvq_fillu32_8x1_2x1((u32 *)ct2, pxu, pxv);
			lqtvq_fillu32_8x1_2x1((u32 *)ct3, pxu, pxv);
		}
		break;
	case 0x0A:
		pxy0=*(u16 *)(blkbuf+16);
		for(i=0; i<2; i++)
		{
			px0=rgbtab[(pxy0>>(i*8+0))&3];
			px1=rgbtab[(pxy0>>(i*8+2))&3];
			px2=rgbtab[(pxy0>>(i*8+4))&3];
			px3=rgbtab[(pxy0>>(i*8+6))&3];

			ct0=ibuf+(i*4+0)*ystr;	ct1=ibuf+(i*4+1)*ystr;
			ct2=ibuf+(i*4+2)*ystr;	ct3=ibuf+(i*4+3)*ystr;
			lqtvq_fillu32_8x1_4x1((u32 *)ct0, px0, px1, px2, px3);
			lqtvq_fillu32_8x1_4x1((u32 *)ct1, px0, px1, px2, px3);
			lqtvq_fillu32_8x1_4x1((u32 *)ct2, px0, px1, px2, px3);
			lqtvq_fillu32_8x1_4x1((u32 *)ct3, px0, px1, px2, px3);
		}
		break;
	case 0x0B:
		pxy0=*(u16 *)(blkbuf+16);
		for(i=0; i<4; i++)
		{
			pxu=rgbtab[(pxy0>>(i*4+0))&3];
			pxv=rgbtab[(pxy0>>(i*4+2))&3];

			ct0=ibuf+(i*2+0)*ystr;	ct1=ibuf+(i*2+1)*ystr;
			lqtvq_fillu32_8x1_2x1((u32 *)ct0, pxu, pxv);
			lqtvq_fillu32_8x1_2x1((u32 *)ct1, pxu, pxv);
		}
		break;
	case 0x0C:
		pxy0=btic4b_getu32le(blkbuf+16);
		for(i=0; i<4; i++)
		{
			px0=rgbtab[(pxy0>>(i*8+0))&3];
			px1=rgbtab[(pxy0>>(i*8+2))&3];
			px2=rgbtab[(pxy0>>(i*8+4))&3];
			px3=rgbtab[(pxy0>>(i*8+6))&3];

			ct0=ibuf+(i*2+0)*ystr;	ct1=ibuf+(i*2+1)*ystr;
			lqtvq_fillu32_8x1_4x1((u32 *)ct0, px0, px1, px2, px3);
			lqtvq_fillu32_8x1_4x1((u32 *)ct1, px0, px1, px2, px3);
		}
		break;
	case 0x0D:
		for(i=0; i<2; i++)
		{
			pxy0=btic4b_getu32le(blkbuf+16+i*4);
			pxF=rgbtab[(pxy0>>30)&3];	pxE=rgbtab[(pxy0>>28)&3];
			pxD=rgbtab[(pxy0>>26)&3];	pxC=rgbtab[(pxy0>>24)&3];
			pxB=rgbtab[(pxy0>>22)&3];	pxA=rgbtab[(pxy0>>20)&3];
			px9=rgbtab[(pxy0>>18)&3];	px8=rgbtab[(pxy0>>16)&3];
			px7=rgbtab[(pxy0>>14)&3];	px6=rgbtab[(pxy0>>12)&3];
			px5=rgbtab[(pxy0>>10)&3];	px4=rgbtab[(pxy0>> 8)&3];
			px3=rgbtab[(pxy0>> 6)&3];	px2=rgbtab[(pxy0>> 4)&3];
			px1=rgbtab[(pxy0>> 2)&3];	px0=rgbtab[(pxy0>> 0)&3];

			ct0=ibuf+(i*4+0)*ystr;	ct1=ibuf+(i*4+1)*ystr;
			ct2=ibuf+(i*4+2)*ystr;	ct3=ibuf+(i*4+3)*ystr;

			lqtvq_fillu32_8x1_8x1((u32 *)ct0,
				px0, px1, px2, px3, px4, px5, px6, px7);
			lqtvq_fillu32_8x1_8x1((u32 *)ct1,
				px0, px1, px2, px3, px4, px5, px6, px7);
			lqtvq_fillu32_8x1_8x1((u32 *)ct2,
				px8, px9, pxA, pxB, pxC, pxD, pxE, pxF);
			lqtvq_fillu32_8x1_8x1((u32 *)ct3,
				px8, px9, pxA, pxB, pxC, pxD, pxE, pxF);
		}
		break;
	case 0x0E:
		for(i=0; i<2; i++)
		{
			pxy0=btic4b_getu32le(blkbuf+16+i*4);
			pxF=rgbtab[(pxy0>>30)&3];	pxE=rgbtab[(pxy0>>28)&3];
			pxD=rgbtab[(pxy0>>26)&3];	pxC=rgbtab[(pxy0>>24)&3];
			pxB=rgbtab[(pxy0>>22)&3];	pxA=rgbtab[(pxy0>>20)&3];
			px9=rgbtab[(pxy0>>18)&3];	px8=rgbtab[(pxy0>>16)&3];
			px7=rgbtab[(pxy0>>14)&3];	px6=rgbtab[(pxy0>>12)&3];
			px5=rgbtab[(pxy0>>10)&3];	px4=rgbtab[(pxy0>> 8)&3];
			px3=rgbtab[(pxy0>> 6)&3];	px2=rgbtab[(pxy0>> 4)&3];
			px1=rgbtab[(pxy0>> 2)&3];	px0=rgbtab[(pxy0>> 0)&3];

			ct0=ibuf+(i*4+0)*ystr;	ct1=ibuf+(i*4+1)*ystr;
			ct2=ibuf+(i*4+2)*ystr;	ct3=ibuf+(i*4+3)*ystr;

			lqtvq_fillu32_8x1_4x1((u32 *)ct0, px0, px1, px2, px3);
			lqtvq_fillu32_8x1_4x1((u32 *)ct1, px4, px5, px6, px7);
			lqtvq_fillu32_8x1_4x1((u32 *)ct2, px8, px9, pxA, pxB);
			lqtvq_fillu32_8x1_4x1((u32 *)ct3, pxC, pxD, pxE, pxF);
		}
		break;
	case 0x0F:
		for(i=0; i<4; i++)
		{
			pxy0=btic4b_getu32le(blkbuf+16+i*4);
			pxF=rgbtab[(pxy0>>30)&3];	pxE=rgbtab[(pxy0>>28)&3];
			pxD=rgbtab[(pxy0>>26)&3];	pxC=rgbtab[(pxy0>>24)&3];
			pxB=rgbtab[(pxy0>>22)&3];	pxA=rgbtab[(pxy0>>20)&3];
			px9=rgbtab[(pxy0>>18)&3];	px8=rgbtab[(pxy0>>16)&3];
			px7=rgbtab[(pxy0>>14)&3];	px6=rgbtab[(pxy0>>12)&3];
			px5=rgbtab[(pxy0>>10)&3];	px4=rgbtab[(pxy0>> 8)&3];
			px3=rgbtab[(pxy0>> 6)&3];	px2=rgbtab[(pxy0>> 4)&3];
			px1=rgbtab[(pxy0>> 2)&3];	px0=rgbtab[(pxy0>> 0)&3];
			ct0=ibuf+(i*2+0)*ystr;	ct1=ibuf+(i*2+1)*ystr;

			lqtvq_fillu32_8x1_8x1((u32 *)ct0,
				px0, px1, px2, px3, px4, px5, px6, px7);
			lqtvq_fillu32_8x1_8x1((u32 *)ct1,
				px8, px9, pxA, pxB, pxC, pxD, pxE, pxF);
		}
		break;
	}
}

/* 8x8x3 */
void BTIC4B_DecBlock2BGRA(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	u32 rgbtab[8];
	byte *ct0, *ct1;
	byte *ct2, *ct3;
	int acy, acu, acv, dcy, dcu, dcv;
	int mcy, mcu, mcv, ncy, ncu, ncv;

	int cr0, cr1, cr2, cr3, cr4, cr5, cr6, cr7;
	int cg0, cg1, cg2, cg3, cg4, cg5, cg6, cg7;
	int cb0, cb1, cb2, cb3, cb4, cb5, cb6, cb7;
	int cy0, cy1, cy2, cy3;
	int cy, cu, cv, cu1, cv1;

	u32 px0, px1, px2, px3;
	u32 px4, px5, px6, px7;
	u32 px8, px9, pxA, pxB;
	u32 pxC, pxD, pxE, pxF;

	u32 pxy0, pxy1, pxu, pxv;
	int i, j, k;
	
	acy=*(s16 *)(blkbuf+ 4);
	acu=*(s16 *)(blkbuf+ 6);
	acv=*(s16 *)(blkbuf+ 8);
	dcy=*(s16 *)(blkbuf+10);
	dcu=*(s16 *)(blkbuf+12);
	dcv=*(s16 *)(blkbuf+14);

	ctx->ClrDec2T(blkbuf[0],
		acy, acu, acv, dcy, dcu, dcv,
		&cr0, &cg0, &cb0, &cr7, &cg7, &cb7);

//	cr1=(14*cr0+ 2*cr7)>>4;	cg1=(14*cg0+ 2*cg7)>>4;	cb1=(14*cb0+ 2*cb7)>>4;
//	cr2=(11*cr0+ 5*cr7)>>4;	cg2=(11*cg0+ 5*cg7)>>4;	cb2=(11*cb0+ 5*cb7)>>4;
	cr3=( 9*cr0+ 7*cr7)>>4;	cg3=( 9*cg0+ 7*cg7)>>4;	cb3=( 9*cb0+ 7*cb7)>>4;
	cr4=( 7*cr0+ 9*cr7)>>4;	cg4=( 7*cg0+ 9*cg7)>>4;	cb4=( 7*cb0+ 9*cb7)>>4;
//	cr5=( 5*cr0+11*cr7)>>4;	cg5=( 5*cg0+11*cg7)>>4;	cb5=( 5*cb0+11*cb7)>>4;
//	cr6=( 2*cr0+14*cr7)>>4;	cg6=( 2*cg0+14*cg7)>>4;	cb6=( 2*cb0+14*cb7)>>4;

//	rgbtab[0]=0xFF000000|(cr0<<16)|(cg0<<8)|cb0;
//	rgbtab[1]=0xFF000000|(cr1<<16)|(cg1<<8)|cb1;
//	rgbtab[2]=0xFF000000|(cr2<<16)|(cg2<<8)|cb2;
//	rgbtab[3]=0xFF000000|(cr3<<16)|(cg3<<8)|cb3;
//	rgbtab[4]=0xFF000000|(cr4<<16)|(cg4<<8)|cb4;
//	rgbtab[5]=0xFF000000|(cr5<<16)|(cg5<<8)|cb5;
//	rgbtab[6]=0xFF000000|(cr6<<16)|(cg6<<8)|cb6;
//	rgbtab[7]=0xFF000000|(cr7<<16)|(cg7<<8)|cb7;

//	rgbtab[0]=ctx->ClrDecPack1(cr0, cg0, cb0);
//	rgbtab[1]=ctx->ClrDecPack1(cr1, cg1, cb1);
//	rgbtab[2]=ctx->ClrDecPack1(cr2, cg2, cb2);
//	rgbtab[3]=ctx->ClrDecPack1(cr3, cg3, cb3);
//	rgbtab[4]=ctx->ClrDecPack1(cr4, cg4, cb4);
//	rgbtab[5]=ctx->ClrDecPack1(cr5, cg5, cb5);
//	rgbtab[6]=ctx->ClrDecPack1(cr6, cg6, cb6);
//	rgbtab[7]=ctx->ClrDecPack1(cr7, cg7, cb7);

	ctx->ClrDecPack4(cr0, cg0, cb0,
		cr3, cg3, cb3, 255, rgbtab+0);
	ctx->ClrDecPack4(cr4, cg4, cb4,
		cr7, cg7, cb7, 255, rgbtab+4);

	for(i=0; i<4; i++)
	{
//		pxy0=*(u32 *)(blkbuf+16+i*6+3);
//		pxy1=*(u32 *)(blkbuf+16+i*6+0);
		pxy0=btic4b_getu32le(blkbuf+16+i*6+3);
		pxy1=btic4b_getu32le(blkbuf+16+i*6+0);
		pxF=rgbtab[(pxy0>>21)&7];	pxE=rgbtab[(pxy0>>18)&7];
		pxD=rgbtab[(pxy0>>15)&7];	pxC=rgbtab[(pxy0>>12)&7];
		pxB=rgbtab[(pxy0>> 9)&7];	pxA=rgbtab[(pxy0>> 6)&7];
		px9=rgbtab[(pxy0>> 3)&7];	px8=rgbtab[(pxy0>> 0)&7];
		px7=rgbtab[(pxy1>>21)&7];	px6=rgbtab[(pxy1>>18)&7];
		px5=rgbtab[(pxy1>>15)&7];	px4=rgbtab[(pxy1>>12)&7];
		px3=rgbtab[(pxy1>> 9)&7];	px2=rgbtab[(pxy1>> 6)&7];
		px1=rgbtab[(pxy1>> 3)&7];	px0=rgbtab[(pxy1>> 0)&7];
		ct0=ibuf+(i*2+0)*ystr;	ct1=ibuf+(i*2+1)*ystr;

		lqtvq_fillu32_8x1_8x1((u32 *)ct0,
			px0, px1, px2, px3, px4, px5, px6, px7);
		lqtvq_fillu32_8x1_8x1((u32 *)ct1,
			px8, px9, pxA, pxB, pxC, pxD, pxE, pxF);
	}
}

/* 8x8x4 */
void BTIC4B_DecBlock3BGRA(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	u32 rgbtab[16];
	byte *ct0, *ct1;
	byte *ct2, *ct3;
	int acy, acu, acv, dcy, dcu, dcv;
	int mcy, mcu, mcv, ncy, ncu, ncv;

	int cr0, cr1, cr2, cr3, cr4, cr5, cr6, cr7;
	int cg0, cg1, cg2, cg3, cg4, cg5, cg6, cg7;
	int cb0, cb1, cb2, cb3, cb4, cb5, cb6, cb7;
	int cr8, cr9, crA, crB, crC, crD, crE, crF;
	int cg8, cg9, cgA, cgB, cgC, cgD, cgE, cgF;
	int cb8, cb9, cbA, cbB, cbC, cbD, cbE, cbF;

	int cy0, cy1, cy2, cy3;
	int cy, cu, cv, cu1, cv1;

	u32 px0, px1, px2, px3;
	u32 px4, px5, px6, px7;
	u32 px8, px9, pxA, pxB;
	u32 pxC, pxD, pxE, pxF;

	u32 pxy0, pxy1, pxu, pxv;
	int i, j, k;
	
	acy=*(s16 *)(blkbuf+ 4);	acu=*(s16 *)(blkbuf+ 6);
	acv=*(s16 *)(blkbuf+ 8);	dcy=*(s16 *)(blkbuf+10);
	dcu=*(s16 *)(blkbuf+12);	dcv=*(s16 *)(blkbuf+14);

	ctx->ClrDec2T(blkbuf[0],
		acy, acu, acv, dcy, dcu, dcv,
		&cr0, &cg0, &cb0, &crF, &cgF, &cbF);

//	cr1=(30*cr0+ 2*crF)>>5;	cg1=(30*cg0+ 2*cgF)>>5;	cb1=(30*cb0+ 2*cbF)>>5;
//	cr2=(28*cr0+ 4*crF)>>5;	cg2=(28*cg0+ 4*cgF)>>5;	cb2=(28*cb0+ 4*cbF)>>5;
	cr3=(26*cr0+ 6*crF)>>5;	cg3=(26*cg0+ 6*cgF)>>5;	cb3=(26*cb0+ 6*cbF)>>5;
	cr4=(23*cr0+ 9*crF)>>5;	cg4=(23*cg0+ 9*cgF)>>5;	cb4=(23*cb0+ 9*cbF)>>5;
//	cr5=(21*cr0+11*crF)>>5;	cg5=(21*cg0+11*cgF)>>5;	cb5=(21*cb0+11*cbF)>>5;
//	cr6=(19*cr0+13*crF)>>5;	cg6=(19*cg0+13*cgF)>>5;	cb6=(19*cb0+13*cbF)>>5;
	cr7=(17*cr0+15*crF)>>5;	cg7=(17*cg0+15*cgF)>>5;	cb7=(17*cb0+15*cbF)>>5;
	cr8=(15*cr0+17*crF)>>5;	cg8=(15*cg0+17*cgF)>>5;	cb8=(15*cb0+17*cbF)>>5;
//	cr9=(13*cr0+19*crF)>>5;	cg9=(13*cg0+19*cgF)>>5;	cb9=(13*cb0+19*cbF)>>5;
//	crA=(11*cr0+21*crF)>>5;	cgA=(11*cg0+21*cgF)>>5;	cbA=(11*cb0+21*cbF)>>5;
	crB=( 9*cr0+23*crF)>>5;	cgB=( 9*cg0+23*cgF)>>5;	cbB=( 9*cb0+23*cbF)>>5;
	crC=( 6*cr0+26*crF)>>5;	cgC=( 6*cg0+26*cgF)>>5;	cbC=( 6*cb0+26*cbF)>>5;
//	crD=( 4*cr0+28*crF)>>5;	cgD=( 4*cg0+28*cgF)>>5;	cbD=( 4*cb0+28*cbF)>>5;
//	crE=( 2*cr0+30*crF)>>5;	cgE=( 2*cg0+30*cgF)>>5;	cbE=( 2*cb0+30*cbF)>>5;

#if 0
	rgbtab[ 0]=0xFF000000|(cr0<<16)|(cg0<<8)|cb0;
	rgbtab[ 1]=0xFF000000|(cr1<<16)|(cg1<<8)|cb1;
	rgbtab[ 2]=0xFF000000|(cr2<<16)|(cg2<<8)|cb2;
	rgbtab[ 3]=0xFF000000|(cr3<<16)|(cg3<<8)|cb3;
	rgbtab[ 4]=0xFF000000|(cr4<<16)|(cg4<<8)|cb4;
	rgbtab[ 5]=0xFF000000|(cr5<<16)|(cg5<<8)|cb5;
	rgbtab[ 6]=0xFF000000|(cr6<<16)|(cg6<<8)|cb6;
	rgbtab[ 7]=0xFF000000|(cr7<<16)|(cg7<<8)|cb7;
	rgbtab[ 8]=0xFF000000|(cr8<<16)|(cg8<<8)|cb8;
	rgbtab[ 9]=0xFF000000|(cr9<<16)|(cg9<<8)|cb9;
	rgbtab[10]=0xFF000000|(crA<<16)|(cgA<<8)|cbA;
	rgbtab[11]=0xFF000000|(crB<<16)|(cgB<<8)|cbB;
	rgbtab[12]=0xFF000000|(crC<<16)|(cgC<<8)|cbC;
	rgbtab[13]=0xFF000000|(crD<<16)|(cgD<<8)|cbD;
	rgbtab[14]=0xFF000000|(crE<<16)|(cgE<<8)|cbE;
	rgbtab[15]=0xFF000000|(crF<<16)|(cgF<<8)|cbF;
#endif

#if 0
	rgbtab[ 0]=ctx->ClrDecPack1(cr0, cg0, cb0);
	rgbtab[ 1]=ctx->ClrDecPack1(cr1, cg1, cb1);
	rgbtab[ 2]=ctx->ClrDecPack1(cr2, cg2, cb2);
	rgbtab[ 3]=ctx->ClrDecPack1(cr3, cg3, cb3);
	rgbtab[ 4]=ctx->ClrDecPack1(cr4, cg4, cb4);
	rgbtab[ 5]=ctx->ClrDecPack1(cr5, cg5, cb5);
	rgbtab[ 6]=ctx->ClrDecPack1(cr6, cg6, cb6);
	rgbtab[ 7]=ctx->ClrDecPack1(cr7, cg7, cb7);
	rgbtab[ 8]=ctx->ClrDecPack1(cr8, cg8, cb8);
	rgbtab[ 9]=ctx->ClrDecPack1(cr9, cg9, cb9);
	rgbtab[10]=ctx->ClrDecPack1(crA, cgA, cbA);
	rgbtab[11]=ctx->ClrDecPack1(crB, cgB, cbB);
	rgbtab[12]=ctx->ClrDecPack1(crC, cgC, cbC);
	rgbtab[13]=ctx->ClrDecPack1(crD, cgD, cbD);
	rgbtab[14]=ctx->ClrDecPack1(crE, cgE, cbE);
	rgbtab[15]=ctx->ClrDecPack1(crF, cgF, cbF);
#endif

	ctx->ClrDecPack4(cr0, cg0, cb0,
		cr3, cg3, cb3, 255, rgbtab+0);
	ctx->ClrDecPack4(cr4, cg4, cb4,
		cr7, cg7, cb7, 255, rgbtab+4);
	ctx->ClrDecPack4(cr8, cg8, cb8,
		crB, cgB, cbB, 255, rgbtab+8);
	ctx->ClrDecPack4(crC, cgC, cbC,
		crF, cgF, cbF, 255, rgbtab+12);


	for(i=0; i<4; i++)
	{
		pxy0=btic4b_getu32le(blkbuf+16+i*8+4);
		pxy1=btic4b_getu32le(blkbuf+16+i*8+0);
		pxF=rgbtab[(pxy0>>28)&15];	pxE=rgbtab[(pxy0>>24)&15];
		pxD=rgbtab[(pxy0>>20)&15];	pxC=rgbtab[(pxy0>>16)&15];
		pxB=rgbtab[(pxy0>>12)&15];	pxA=rgbtab[(pxy0>> 8)&15];
		px9=rgbtab[(pxy0>> 4)&15];	px8=rgbtab[(pxy0>> 0)&15];
		px7=rgbtab[(pxy1>>28)&15];	px6=rgbtab[(pxy1>>24)&15];
		px5=rgbtab[(pxy1>>20)&15];	px4=rgbtab[(pxy1>>16)&15];
		px3=rgbtab[(pxy1>>12)&15];	px2=rgbtab[(pxy1>> 8)&15];
		px1=rgbtab[(pxy1>> 4)&15];	px0=rgbtab[(pxy1>> 0)&15];
		ct0=ibuf+(i*2+0)*ystr;	ct1=ibuf+(i*2+1)*ystr;

		lqtvq_fillu32_8x1_8x1((u32 *)ct0,
			px0, px1, px2, px3, px4, px5, px6, px7);
		lqtvq_fillu32_8x1_8x1((u32 *)ct1,
			px8, px9, pxA, pxB, pxC, pxD, pxE, pxF);
	}
}

/* Y8x8x2+UV */
void BTIC4B_DecBlock4BGRA(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	int ytab[4], utab[4], vtab[4];
	byte *ct0, *ct1, *ct2, *ct3;
	int acy, acu, acv, dcy, dcu, dcv;
	int mcy, mcu, mcv, ncy, ncu, ncv;

	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int cy0, cy1, cy2, cy3;
	int cy, cu, cv, cu0, cv0, cu1, cv1, cu2, cv2, cu3, cv3;
	u32 pxy, pxu, pxv, pxy0, pxy1, pxu0, pxv0, pxu1, pxv1;
	int bt;
	int i, j, k;
	
	bt=blkbuf[0]&0x1F;
	acy=*(s16 *)(blkbuf+ 4);
	acu=*(s16 *)(blkbuf+ 6);
	acv=*(s16 *)(blkbuf+ 8);
	dcy=*(s16 *)(blkbuf+10);
	dcu=*(s16 *)(blkbuf+12);
	dcv=*(s16 *)(blkbuf+14);
	
	mcy=acy-(dcy>>1); ncy=mcy+dcy;
	mcu=acu-(dcu>>1); ncu=mcu+dcu;
	mcv=acv-(dcv>>1); ncv=mcv+dcv;

	ytab[0]=mcy;
	ytab[1]=(11*mcy+ 5*ncy)>>4;
	ytab[2]=( 5*mcy+11*ncy)>>4;
	ytab[3]=ncy;

	utab[0]=mcu;
	utab[1]=(11*mcu+ 5*ncu)>>4;
	utab[2]=( 5*mcu+11*ncu)>>4;
	utab[3]=ncu;

	vtab[0]=mcv;
	vtab[1]=(11*mcv+ 5*ncv)>>4;
	vtab[2]=( 5*mcv+11*ncv)>>4;
	vtab[3]=ncv;
	
	if(bt==0x18)
	{
		pxu=btic4b_getu32le(blkbuf+32);
		pxv=btic4b_getu32le(blkbuf+36);

		for(i=0; i<4; i++)
		{
			ct0=ibuf+(i*2+0)*ystr;
			ct1=ibuf+(i*2+1)*ystr;
//			pxy0=btic4b_getu32le(blkbuf+16+(3-i)*4+0);
			pxy0=btic4b_getu32le(blkbuf+16+i*4+0);
		
			for(j=0; j<4; j++)
			{
				k=j*4;
				cy3=ytab[(pxy0>>(k+18))&3];
				cy2=ytab[(pxy0>>(k+16))&3];
				cy1=ytab[(pxy0>>(k+ 2))&3];
				cy0=ytab[(pxy0>>(k+ 0))&3];
				k=(i*4+j)*2;
				cu=utab[(pxu>>k)&3];
				cv=vtab[(pxv>>k)&3];

				ctx->ClrDec4(cy0, cy1, cy2, cy3, cu, cv,
					(u32 *)ct0, (u32 *)ct1);
				ct0+=8;		ct1+=8;
			}
		}
		return;
	}

	if(bt==0x1A)
	{
		for(i=0; i<4; i++)
		{
			j=(i>>1)*4;
			pxu=btic4b_getu32le(blkbuf+32+j);
			pxv=btic4b_getu32le(blkbuf+40+j);

			ct0=ibuf+(i*2+0)*ystr;
			ct1=ibuf+(i*2+1)*ystr;
			pxy0=btic4b_getu32le(blkbuf+16+i*4+0);
		
			for(j=0; j<4; j++)
			{
				k=j*4;
				cy3=ytab[(pxy0>>(k+18))&3];
				cy2=ytab[(pxy0>>(k+16))&3];
				cy1=ytab[(pxy0>>(k+ 2))&3];
				cy0=ytab[(pxy0>>(k+ 0))&3];

				k=((i&1)*8+j)*2;
				cu0=utab[(pxu>>(k+0))&3];
				cu1=utab[(pxu>>(k+8))&3];
				cv0=vtab[(pxv>>(k+0))&3];
				cv1=vtab[(pxv>>(k+8))&3];

				ctx->ClrDec4B(cy0, cy1, cy2, cy3,
					cu0, cv0, cu1, cv1,
					(u32 *)ct0, (u32 *)ct1);
				ct0+=8;		ct1+=8;
			}
		}
		return;
	}

	if(bt==0x1B)
	{
		for(i=0; i<4; i++)
		{
			ct0=ibuf+(i*2+0)*ystr;
			ct1=ibuf+(i*2+1)*ystr;
			pxy0=btic4b_getu32le(blkbuf+16+i*4+0);
			pxu0=btic4b_getu32le(blkbuf+32+i*4+0);
			pxv0=btic4b_getu32le(blkbuf+48+i*4+0);
		
			for(j=0; j<4; j++)
			{
				k=j*4;
				cy3=ytab[(pxy0>>(k+18))&3];
				cy2=ytab[(pxy0>>(k+16))&3];
				cy1=ytab[(pxy0>>(k+ 2))&3];
				cy0=ytab[(pxy0>>(k+ 0))&3];

				cu3=utab[(pxu0>>(k+18))&3];
				cu2=utab[(pxu0>>(k+16))&3];
				cu1=utab[(pxu0>>(k+ 2))&3];
				cu0=utab[(pxu0>>(k+ 0))&3];
				cv3=vtab[(pxv0>>(k+18))&3];
				cv2=vtab[(pxv0>>(k+16))&3];
				cv1=vtab[(pxv0>>(k+ 2))&3];
				cv0=vtab[(pxv0>>(k+ 0))&3];

				ctx->ClrDec4C(cy0, cy1, cy2, cy3,
					cu0, cv0, cu1, cv1,
					cu2, cv2, cu3, cv3,
					(u32 *)ct0, (u32 *)ct1);
				ct0+=8;		ct1+=8;
			}
		}
		return;
	}

	if(bt==0x14)
	{
		pxy=*(byte *)(blkbuf+16);
		i=*(byte *)(blkbuf+17);
		pxu=i&15;
		pxv=i>>4;
		
		utab[1]=utab[3];
		vtab[1]=vtab[3];

		for(i=0; i<2; i++)
		{
			ct0=ibuf+(i*4+0)*ystr;
			ct1=ibuf+(i*4+1)*ystr;		
			ct2=ibuf+(i*4+2)*ystr;
			ct3=ibuf+(i*4+3)*ystr;		
			for(j=0; j<2; j++)
			{
				k=(i*2+j)*2;
				cy=ytab[(pxy>>k)&3];
				k=i*2+j;
				cu=utab[(pxu>>k)&1];
				cv=vtab[(pxv>>k)&1];

				pxy1=ctx->ClrDec1(cy, cu, cv);
				((u32 *)ct0)[0]=pxy1;	((u32 *)ct0)[1]=pxy1;
				((u32 *)ct0)[2]=pxy1;	((u32 *)ct0)[3]=pxy1;
				((u32 *)ct1)[0]=pxy1;	((u32 *)ct1)[1]=pxy1;
				((u32 *)ct1)[2]=pxy1;	((u32 *)ct1)[3]=pxy1;
				((u32 *)ct2)[0]=pxy1;	((u32 *)ct2)[1]=pxy1;
				((u32 *)ct2)[2]=pxy1;	((u32 *)ct2)[3]=pxy1;
				((u32 *)ct3)[0]=pxy1;	((u32 *)ct3)[1]=pxy1;
				((u32 *)ct3)[2]=pxy1;	((u32 *)ct3)[3]=pxy1;
				ct0+=16;	ct1+=16;	ct2+=16;	ct3+=16;
			}
		}
		return;
	}

	if(bt==0x15)
	{
		pxy=btic4b_getu32le(blkbuf+16);
		pxu=*(byte *)(blkbuf+20);
		pxv=*(byte *)(blkbuf+21);

		for(i=0; i<4; i++)
		{
			ct0=ibuf+(i*2+0)*ystr;
			ct1=ibuf+(i*2+1)*ystr;		
			for(j=0; j<4; j++)
			{
				k=(i*4+j)*2;
				cy=ytab[(pxy>>k)&3];
				k=((i>>1)*2+(j>>1))*2;
				cu=utab[(pxu>>k)&3];
				cv=vtab[(pxv>>k)&3];

				pxy1=ctx->ClrDec1(cy, cu, cv);
				((u32 *)ct0)[0]=pxy1;	((u32 *)ct0)[1]=pxy1;
				((u32 *)ct1)[0]=pxy1;	((u32 *)ct1)[1]=pxy1;
				ct0+=8;		ct1+=8;
			}
		}
		return;
	}

	if(bt==0x16)
	{
		pxy=btic4b_getu32le(blkbuf+16);
		pxu=btic4b_getu32le(blkbuf+20);
		pxv=btic4b_getu32le(blkbuf+24);

		for(i=0; i<4; i++)
		{
			ct0=ibuf+(i*2+0)*ystr;
			ct1=ibuf+(i*2+1)*ystr;		
			for(j=0; j<4; j++)
			{
				k=(i*4+j)*2;
				cy=ytab[(pxy>>k)&3];
				cu=utab[(pxu>>k)&3];
				cv=vtab[(pxv>>k)&3];

				pxy1=ctx->ClrDec1(cy, cu, cv);
				((u32 *)ct0)[0]=pxy1;	((u32 *)ct0)[1]=pxy1;
				((u32 *)ct1)[0]=pxy1;	((u32 *)ct1)[1]=pxy1;
				ct0+=8;		ct1+=8;
			}
		}
		return;
	}

	if(bt==0x17)
	{
//		pxu=btic4b_getu32le(blkbuf+32);
//		pxv=btic4b_getu32le(blkbuf+36);
		pxu=*(byte *)(blkbuf+32);
		pxv=*(byte *)(blkbuf+33);

		for(i=0; i<4; i++)
		{
			ct0=ibuf+(i*2+0)*ystr;
			ct1=ibuf+(i*2+1)*ystr;
			pxy0=btic4b_getu32le(blkbuf+16+i*4+0);
		
			for(j=0; j<4; j++)
			{
				k=j*4;
				cy3=ytab[(pxy0>>(k+18))&3];
				cy2=ytab[(pxy0>>(k+16))&3];
				cy1=ytab[(pxy0>>(k+ 2))&3];
				cy0=ytab[(pxy0>>(k+ 0))&3];
				k=((i>>1)*2+(j>>1))*2;
				cu=utab[(pxu>>k)&3];
				cv=vtab[(pxv>>k)&3];

				ctx->ClrDec4(cy0, cy1, cy2, cy3, cu, cv,
					(u32 *)ct0, (u32 *)ct1);
				ct0+=8;		ct1+=8;
			}
		}
		return;
	}
}


/* 8x8x2+UV4x4x2 LDR8 */
void BTIC4B_DecBlock5BGRA(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	int ytab[4], utab[4], vtab[4];
	byte *ct0, *ct1;
	int acy, acu, acv, dcy, dcu, dcv;
	int mcy, mcu, mcv, ncy, ncu, ncv;

	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int cy0, cy1, cy2, cy3;
	int cy, cu, cv, cu0, cv0, cu1, cv1, cu2, cv2, cu3, cv3;

	u32 pxy0, pxy1, pxu, pxv;
	int i, j, k;
	
	acy=blkbuf[2];
	acu=(blkbuf[3]-128)<<1;
	acv=(blkbuf[4]-128)<<1;
	dcy=blkbuf[5];
	dcu=(blkbuf[6]-128)<<1;
	dcv=(blkbuf[7]-128)<<1;
	
	mcy=acy-(dcy>>1); ncy=mcy+dcy;
	mcu=acu-(dcu>>1); ncu=mcu+dcu;
	mcv=acv-(dcv>>1); ncv=mcv+dcv;

	ytab[0]=mcy;
	ytab[1]=(11*mcy+ 5*ncy)>>4;
	ytab[2]=( 5*mcy+11*ncy)>>4;
	ytab[3]=ncy;

	utab[0]=mcu;
	utab[1]=(11*mcu+ 5*ncu)>>4;
	utab[2]=( 5*mcu+11*ncu)>>4;
	utab[3]=ncu;

	vtab[0]=mcv;
	vtab[1]=(11*mcv+ 5*ncv)>>4;
	vtab[2]=( 5*mcv+11*ncv)>>4;
	vtab[3]=ncv;
	
//	if(blkbuf[0]==0x10)
	if(1)
	{
		pxu=btic4b_getu32le(blkbuf+24);
		pxv=btic4b_getu32le(blkbuf+28);

		for(i=0; i<4; i++)
		{
			ct0=ibuf+(i*2+0)*ystr;
			ct1=ibuf+(i*2+1)*ystr;
			pxy0=btic4b_getu32le(blkbuf+8+i*4+0);
		
			for(j=0; j<4; j++)
			{
				k=j*4;
				cy3=ytab[(pxy0>>(k+18))&3];
				cy2=ytab[(pxy0>>(k+16))&3];
				cy1=ytab[(pxy0>>(k+ 2))&3];
				cy0=ytab[(pxy0>>(k+ 0))&3];
				k=(i*4+j)*2;
				cu=utab[(pxu>>k)&3];
				cv=vtab[(pxv>>k)&3];

				ctx->ClrDec4(cy0, cy1, cy2, cy3, cu, cv,
					(u32 *)ct0, (u32 *)ct1);
				ct0+=8;	ct1+=8;
			}
		}
		return;
	}
}

void BTIC4B_DecBlock7BGRA(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	int ytab[8], utab[4], vtab[4];
	byte *ct0, *ct1;
	int acy, acu, acv, dcy, dcu, dcv;
	int mcy, mcu, mcv, ncy, ncu, ncv;

	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int cy0, cy1, cy2, cy3;
	int cu0, cu1, cu2, cu3;
	int cv0, cv1, cv2, cv3;
	int cy, cu, cv;

	u32 pxy0, pxy1, pxu, pxv;
	int i, j, k;
	
	acy=blkbuf[2];
	acu=(blkbuf[3]-128)<<1;
	acv=(blkbuf[4]-128)<<1;
	dcy=blkbuf[5];
	dcu=(blkbuf[6]-128)<<1;
	dcv=(blkbuf[7]-128)<<1;
	
	mcy=acy-(dcy>>1); ncy=mcy+dcy;
	mcu=acu-(dcu>>1); ncu=mcu+dcu;
	mcv=acv-(dcv>>1); ncv=mcv+dcv;

	ytab[0]=mcy;
	ytab[1]=(14*mcy+ 2*ncy)>>4;
	ytab[2]=(11*mcy+ 5*ncy)>>4;
	ytab[3]=( 9*mcy+ 7*ncy)>>4;
	ytab[4]=( 7*mcy+ 9*ncy)>>4;
	ytab[5]=( 5*mcy+11*ncy)>>4;
	ytab[6]=( 2*mcy+14*ncy)>>4;
	ytab[7]=ncy;

	utab[0]=mcu;
	utab[1]=(11*mcu+ 5*ncu)>>4;
	utab[2]=( 5*mcu+11*ncu)>>4;
	utab[3]=ncu;

	vtab[0]=mcv;
	vtab[1]=(11*mcv+ 5*ncv)>>4;
	vtab[2]=( 5*mcv+11*ncv)>>4;
	vtab[3]=ncv;
	
//	pxu=btic4b_getu32le(blkbuf+40);
//	pxv=btic4b_getu32le(blkbuf+44);

	for(i=0; i<4; i++)
	{
		ct0=ibuf+(i*2+0)*ystr;
		ct1=ibuf+(i*2+1)*ystr;
//		pxy0=*(u32 *)(blkbuf+8+i*6+0);
//		pxy1=*(u32 *)(blkbuf+8+i*6+3);
//		pxu=*(u32 *)(blkbuf+32+i*4);
//		pxv=*(u32 *)(blkbuf+48+i*4);
		pxy0=btic4b_getu32le(blkbuf+8+i*6+0);
		pxy1=btic4b_getu32le(blkbuf+8+i*6+3);
		pxu=btic4b_getu32le(blkbuf+32+i*4);
		pxv=btic4b_getu32le(blkbuf+48+i*4);

//btic4b_getu32le
		
		for(j=0; j<4; j++)
		{
			k=j*6;
			cy0=ytab[(pxy0>>(k+0))&7];
			cy1=ytab[(pxy0>>(k+3))&7];
			cy2=ytab[(pxy1>>(k+0))&7];
			cy3=ytab[(pxy1>>(k+3))&7];

			k=j*4;
			cu3=utab[(pxu>>(k+18))&3];
			cu2=utab[(pxu>>(k+16))&3];
			cu1=utab[(pxu>>(k+ 2))&3];
			cu0=utab[(pxu>>(k+ 0))&3];

			cv3=vtab[(pxv>>(k+18))&3];
			cv2=vtab[(pxv>>(k+16))&3];
			cv1=vtab[(pxv>>(k+ 2))&3];
			cv0=vtab[(pxv>>(k+ 0))&3];

			ctx->ClrDec4C(cy0, cy1, cy2, cy3,
				cu0, cv0, cu1, cv1, cu2, cv2, cu3, cv3,
				(u32 *)ct0, (u32 *)ct1);
			ct0+=8;	ct1+=8;
		}
	}
}

/* XxXx1 */
void BTIC4B_DecBlock6BGRA(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	u32 rgbtab[4];
	byte *ct0, *ct1;
	byte *ct2, *ct3;
	int acy, acu, acv, dcy, dcu, dcv;
	int mcy, mcu, mcv, ncy, ncu, ncv;

	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int cy0, cy1, cy2, cy3;
	int cy, cu, cv, cu1, cv1;

	u32 px0, px1, px2, px3;
	u32 px4, px5, px6, px7;
	u32 px8, px9, pxA, pxB;
	u32 pxC, pxD, pxE, pxF;

	u32 pxy0, pxy1, pxu, pxv;
	int i, j, k;
	
	acy=*(s16 *)(blkbuf+ 4);
	acu=*(s16 *)(blkbuf+ 6);
	acv=*(s16 *)(blkbuf+ 8);
	dcy=*(s16 *)(blkbuf+10);
	dcu=*(s16 *)(blkbuf+12);
	dcv=*(s16 *)(blkbuf+14);
	
	mcy=acy-(dcy>>1); ncy=mcy+dcy;
	mcu=acu-(dcu>>1); ncu=mcu+dcu;
	mcv=acv-(dcv>>1); ncv=mcv+dcv;

	rgbtab[0]=ctx->ClrDec1(mcy, acu, acv);
	rgbtab[1]=ctx->ClrDec1(ncy, acu, acv);

	switch(blkbuf[0]&0x1F)
	{
	case 0x01:
		pxy0=blkbuf[16];
		for(i=0; i<2; i++)
		{
			pxu=rgbtab[(pxy0>>(i*2+0))&1];
			pxv=rgbtab[(pxy0>>(i*2+1))&1];
			ct0=ibuf+(i*4+0)*ystr;	ct1=ibuf+(i*4+1)*ystr;
			ct2=ibuf+(i*4+2)*ystr;	ct3=ibuf+(i*4+3)*ystr;
			lqtvq_fillu32_8x1_2x1((u32 *)ct0, pxu, pxv);
			lqtvq_fillu32_8x1_2x1((u32 *)ct1, pxu, pxv);
			lqtvq_fillu32_8x1_2x1((u32 *)ct2, pxu, pxv);
			lqtvq_fillu32_8x1_2x1((u32 *)ct3, pxu, pxv);
		}
		break;
	case 0x02:
		pxy0=blkbuf[16];
		for(i=0; i<2; i++)
		{
			px0=rgbtab[(pxy0>>(i*4+0))&1];
			px1=rgbtab[(pxy0>>(i*4+1))&1];
			px2=rgbtab[(pxy0>>(i*4+2))&1];
			px3=rgbtab[(pxy0>>(i*4+3))&1];

			ct0=ibuf+(i*4+0)*ystr;	ct1=ibuf+(i*4+1)*ystr;
			ct2=ibuf+(i*4+2)*ystr;	ct3=ibuf+(i*4+3)*ystr;
			lqtvq_fillu32_8x1_4x1((u32 *)ct0, px0, px1, px2, px3);
			lqtvq_fillu32_8x1_4x1((u32 *)ct1, px0, px1, px2, px3);
			lqtvq_fillu32_8x1_4x1((u32 *)ct2, px0, px1, px2, px3);
			lqtvq_fillu32_8x1_4x1((u32 *)ct3, px0, px1, px2, px3);
		}
		break;
	case 0x03:
		pxy0=blkbuf[16];
		for(i=0; i<4; i++)
		{
			pxu=rgbtab[(pxy0>>(i*2+0))&1];
			pxv=rgbtab[(pxy0>>(i*2+1))&1];
			ct0=ibuf+(i*2+0)*ystr;	ct1=ibuf+(i*2+1)*ystr;
			lqtvq_fillu32_8x1_2x1((u32 *)ct0, pxu, pxv);
			lqtvq_fillu32_8x1_2x1((u32 *)ct1, pxu, pxv);
		}
		break;
	case 0x04:
		pxy0=*(u16 *)(blkbuf+16);
		for(i=0; i<4; i++)
		{
			px0=rgbtab[(pxy0>>(i*4+0))&1];
			px1=rgbtab[(pxy0>>(i*4+1))&1];
			px2=rgbtab[(pxy0>>(i*4+2))&1];
			px3=rgbtab[(pxy0>>(i*4+3))&1];
			ct0=ibuf+(i*2+0)*ystr;	ct1=ibuf+(i*2+1)*ystr;
			lqtvq_fillu32_8x1_4x1((u32 *)ct0, px0, px1, px2, px3);
			lqtvq_fillu32_8x1_4x1((u32 *)ct1, px0, px1, px2, px3);
		}
		break;
	case 0x05:
		for(i=0; i<2; i++)
		{
			pxy0=*(u16 *)(blkbuf+16+i*2);
			pxF=rgbtab[(pxy0>>15)&1];	pxE=rgbtab[(pxy0>>14)&1];
			pxD=rgbtab[(pxy0>>13)&1];	pxC=rgbtab[(pxy0>>12)&1];
			pxB=rgbtab[(pxy0>>11)&1];	pxA=rgbtab[(pxy0>>10)&1];
			px9=rgbtab[(pxy0>> 9)&1];	px8=rgbtab[(pxy0>> 8)&1];
			px7=rgbtab[(pxy0>> 7)&1];	px6=rgbtab[(pxy0>> 6)&1];
			px5=rgbtab[(pxy0>> 5)&1];	px4=rgbtab[(pxy0>> 4)&1];
			px3=rgbtab[(pxy0>> 3)&1];	px2=rgbtab[(pxy0>> 2)&1];
			px1=rgbtab[(pxy0>> 1)&1];	px0=rgbtab[(pxy0>> 0)&1];

			ct0=ibuf+(i*4+0)*ystr;	ct1=ibuf+(i*4+1)*ystr;
			ct2=ibuf+(i*4+2)*ystr;	ct3=ibuf+(i*4+3)*ystr;

			lqtvq_fillu32_8x1_8x1((u32 *)ct0,
				px0, px1, px2, px3, px4, px5, px6, px7);
			lqtvq_fillu32_8x1_8x1((u32 *)ct1,
				px0, px1, px2, px3, px4, px5, px6, px7);
			lqtvq_fillu32_8x1_8x1((u32 *)ct2,
				px8, px9, pxA, pxB, pxC, pxD, pxE, pxF);
			lqtvq_fillu32_8x1_8x1((u32 *)ct3,
				px8, px9, pxA, pxB, pxC, pxD, pxE, pxF);
		}
		break;
	case 0x06:
		for(i=0; i<2; i++)
		{
			pxy0=*(u16 *)(blkbuf+16+i*2);
			pxF=rgbtab[(pxy0>>15)&1];	pxE=rgbtab[(pxy0>>14)&1];
			pxD=rgbtab[(pxy0>>13)&1];	pxC=rgbtab[(pxy0>>12)&1];
			pxB=rgbtab[(pxy0>>11)&1];	pxA=rgbtab[(pxy0>>10)&1];
			px9=rgbtab[(pxy0>> 9)&1];	px8=rgbtab[(pxy0>> 8)&1];
			px7=rgbtab[(pxy0>> 7)&1];	px6=rgbtab[(pxy0>> 6)&1];
			px5=rgbtab[(pxy0>> 5)&1];	px4=rgbtab[(pxy0>> 4)&1];
			px3=rgbtab[(pxy0>> 3)&1];	px2=rgbtab[(pxy0>> 2)&1];
			px1=rgbtab[(pxy0>> 1)&1];	px0=rgbtab[(pxy0>> 0)&1];

			ct0=ibuf+(i*4+0)*ystr;	ct1=ibuf+(i*4+1)*ystr;
			ct2=ibuf+(i*4+2)*ystr;	ct3=ibuf+(i*4+3)*ystr;

			lqtvq_fillu32_8x1_4x1((u32 *)ct0, px0, px1, px2, px3);
			lqtvq_fillu32_8x1_4x1((u32 *)ct1, px4, px5, px6, px7);
			lqtvq_fillu32_8x1_4x1((u32 *)ct2, px8, px9, pxA, pxB);
			lqtvq_fillu32_8x1_4x1((u32 *)ct3, pxC, pxD, pxE, pxF);
		}
		break;
	case 0x07:
		for(i=0; i<4; i++)
		{
			pxy0=*(u16 *)(blkbuf+16+i*2);
			pxF=rgbtab[(pxy0>>15)&1];	pxE=rgbtab[(pxy0>>14)&1];
			pxD=rgbtab[(pxy0>>13)&1];	pxC=rgbtab[(pxy0>>12)&1];
			pxB=rgbtab[(pxy0>>11)&1];	pxA=rgbtab[(pxy0>>10)&1];
			px9=rgbtab[(pxy0>> 9)&1];	px8=rgbtab[(pxy0>> 8)&1];
			px7=rgbtab[(pxy0>> 7)&1];	px6=rgbtab[(pxy0>> 6)&1];
			px5=rgbtab[(pxy0>> 5)&1];	px4=rgbtab[(pxy0>> 4)&1];
			px3=rgbtab[(pxy0>> 3)&1];	px2=rgbtab[(pxy0>> 2)&1];
			px1=rgbtab[(pxy0>> 1)&1];	px0=rgbtab[(pxy0>> 0)&1];

			ct0=ibuf+(i*2+0)*ystr;	ct1=ibuf+(i*2+1)*ystr;

			lqtvq_fillu32_8x1_8x1((u32 *)ct0,
				px0, px1, px2, px3, px4, px5, px6, px7);
			lqtvq_fillu32_8x1_8x1((u32 *)ct1,
				px8, px9, pxA, pxB, pxC, pxD, pxE, pxF);
		}
		break;
	}
}

/*
0000
0003
0033
0333

0012 90 06
0112 94 16
1223 E9 6B
1233 F9 6F

0123 E8
*/

void BTIC4B_DecBlockGrad4BGRA(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	static const u32 grtab[16]={
		0xF0F0F0F0,	0xFFFF0000,	0x0F0F0F0F,	0x0000FFFF,
		0x3F0F0300,	0xFCF0C000,	0x00C0F0FC,	0x00030F3F,
		0xE8E8E8E8, 0xFFAA5500, 0x1B1B1B1B, 0x0055AAFF,
		0xF9E99490, 0x6F6B1606, 0x06166B6F, 0x9094E9F9};
	byte tblk[128];
	int i, j;
	memcpy(tblk, blkbuf, 64);
	
	i=*(u32 *)(tblk+16);
	
	if(!(i&(~15)))
	{
		tblk[0]=0x0C;
		*(u32 *)(tblk+16)=grtab[blkbuf[16]&15];
		BTIC4B_DecBlock1BGRA(ctx, tblk, ibuf, ystr);
		return;
	}

	if(1)
	{
		j=i&65535;
		tblk[0]=0x0F;
//		*(u32 *)(tblk+16)=grtab[blkbuf[16]&15];
		*(u32 *)(tblk+16)=btic4b_grad16_ipattab2[j*4+0];
		*(u32 *)(tblk+20)=btic4b_grad16_ipattab2[j*4+1];
		*(u32 *)(tblk+24)=btic4b_grad16_ipattab2[j*4+2];
		*(u32 *)(tblk+28)=btic4b_grad16_ipattab2[j*4+3];
		BTIC4B_DecBlock1BGRA(ctx, tblk, ibuf, ystr);
		return;
	}
}


/* 8x8x4+UV4x4x4 */
void BTIC4B_DecBlock8BGRA(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	int ytab[16], utab[16], vtab[16];
	byte *ct0, *ct1;
	int acy, acu, acv, dcy, dcu, dcv;
	int mcy, mcu, mcv, ncy, ncu, ncv;

	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int cy0, cy1, cy2, cy3;
	int cy, cu, cv, cu1, cv1;

	u32 px0, px1, px2, px3;
	u32 pxy0, pxy1, pxu, pxv;
	int i, j, k;
	
	acy=*(s16 *)(blkbuf+ 4);
	acu=*(s16 *)(blkbuf+ 6);
	acv=*(s16 *)(blkbuf+ 8);
	dcy=*(s16 *)(blkbuf+10);
	dcu=*(s16 *)(blkbuf+12);
	dcv=*(s16 *)(blkbuf+14);
	
	mcy=acy-(dcy>>1); ncy=mcy+dcy;
	mcu=acu-(dcu>>1); ncu=mcu+dcu;
	mcv=acv-(dcv>>1); ncv=mcv+dcv;

	ytab[0]=mcy;
	ytab[ 1]=(30*mcy+ 2*ncy)>>5;	ytab[ 2]=(28*mcy+ 4*ncy)>>5;
	ytab[ 3]=(26*mcy+ 6*ncy)>>5;	ytab[ 4]=(23*mcy+ 9*ncy)>>5;
	ytab[ 5]=(21*mcy+11*ncy)>>5;	ytab[ 6]=(19*mcy+13*ncy)>>5;
	ytab[ 7]=(17*mcy+15*ncy)>>5;	ytab[ 8]=(15*mcy+17*ncy)>>5;
	ytab[ 9]=(13*mcy+19*ncy)>>5;	ytab[10]=(11*mcy+21*ncy)>>5;
	ytab[11]=( 9*mcy+23*ncy)>>5;	ytab[12]=( 6*mcy+26*ncy)>>5;
	ytab[13]=( 4*mcy+28*ncy)>>5;	ytab[14]=( 2*mcy+30*ncy)>>5;
	ytab[15]=ncy;

	utab[0]=mcu;
	utab[ 1]=(30*mcu+ 2*ncu)>>5;	utab[ 2]=(28*mcu+ 4*ncu)>>5;
	utab[ 3]=(26*mcu+ 6*ncu)>>5;	utab[ 4]=(23*mcu+ 9*ncu)>>5;
	utab[ 5]=(21*mcu+11*ncu)>>5;	utab[ 6]=(19*mcu+13*ncu)>>5;
	utab[ 7]=(17*mcu+15*ncu)>>5;	utab[ 8]=(15*mcu+17*ncu)>>5;
	utab[ 9]=(13*mcu+19*ncu)>>5;	utab[10]=(11*mcu+21*ncu)>>5;
	utab[11]=( 9*mcu+23*ncu)>>5;	utab[12]=( 6*mcu+26*ncu)>>5;
	utab[13]=( 4*mcu+28*ncu)>>5;	utab[14]=( 2*mcu+30*ncu)>>5;
	utab[15]=ncu;

	vtab[0]=mcv;
	vtab[ 1]=(30*mcv+ 2*ncv)>>5;	vtab[ 2]=(28*mcv+ 4*ncv)>>5;
	vtab[ 3]=(26*mcv+ 6*ncv)>>5;	vtab[ 4]=(23*mcv+ 9*ncv)>>5;
	vtab[ 5]=(21*mcv+11*ncv)>>5;	vtab[ 6]=(19*mcv+13*ncv)>>5;
	vtab[ 7]=(17*mcv+15*ncv)>>5;	vtab[ 8]=(15*mcv+17*ncv)>>5;
	vtab[ 9]=(13*mcv+19*ncv)>>5;	vtab[10]=(11*mcv+21*ncv)>>5;
	vtab[11]=( 9*mcv+23*ncv)>>5;	vtab[12]=( 6*mcv+26*ncv)>>5;
	vtab[13]=( 4*mcv+28*ncv)>>5;	vtab[14]=( 2*mcv+30*ncv)>>5;
	vtab[15]=ncv;
	
	for(i=0; i<4; i++)
	{
		ct0=ibuf+(i*2+0)*ystr;
		ct1=ibuf+(i*2+1)*ystr;
		pxy0=btic4b_getu32le(blkbuf+16+i*8+0);
		pxy1=btic4b_getu32le(blkbuf+16+i*8+4);
		pxu=btic4b_getu32le(blkbuf+48+((i>>1)*4));
		pxv=btic4b_getu32le(blkbuf+56+((i>>1)*4));
		
		for(j=0; j<4; j++)
		{
			k=j*8;
			cy0=ytab[(pxy0>>(k+0))&15];
			cy1=ytab[(pxy0>>(k+4))&15];
			cy2=ytab[(pxy1>>(k+0))&15];
			cy3=ytab[(pxy1>>(k+4))&15];

			k=(i*4+j)*4;
			cu=utab[(pxu>>k)&15];
			cv=vtab[(pxv>>k)&15];

			ctx->ClrDec4(cy0, cy1, cy2, cy3, cu, cv,
				(u32 *)ct0, (u32 *)ct1);
			ct0+=8;		ct1+=8;
		}
	}
}

/* 8x8x3+UV4x8x3 */
void BTIC4B_DecBlock9BGRA(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	int ytab[8], utab[8], vtab[8];
	byte *ct0, *ct1;
	int acy, acu, acv, dcy, dcu, dcv;
	int mcy, mcu, mcv, ncy, ncu, ncv;

	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int cy0, cy1, cy2, cy3;
	int cy, cu0, cv0, cu1, cv1;

	u32 px0, px1, px2, px3;
	u32 pxy0, pxy1, pxu, pxv;
	int i, j, k;
	
	acy=*(s16 *)(blkbuf+ 4);
	acu=*(s16 *)(blkbuf+ 6);
	acv=*(s16 *)(blkbuf+ 8);
	dcy=*(s16 *)(blkbuf+10);
	dcu=*(s16 *)(blkbuf+12);
	dcv=*(s16 *)(blkbuf+14);
	
	mcy=acy-(dcy>>1); ncy=mcy+dcy;
	mcu=acu-(dcu>>1); ncu=mcu+dcu;
	mcv=acv-(dcv>>1); ncv=mcv+dcv;

	ytab[0]=mcy;
	ytab[1]=(14*mcy+ 2*ncy)>>4;
	ytab[2]=(11*mcy+ 5*ncy)>>4;
	ytab[3]=( 9*mcy+ 7*ncy)>>4;
	ytab[4]=( 7*mcy+ 9*ncy)>>4;
	ytab[5]=( 5*mcy+11*ncy)>>4;
	ytab[6]=( 2*mcy+14*ncy)>>4;
	ytab[7]=ncy;

	utab[0]=mcu;
	utab[1]=(14*mcu+ 2*ncu)>>4;
	utab[2]=(11*mcu+ 5*ncu)>>4;
	utab[3]=( 9*mcu+ 7*ncu)>>4;
	utab[4]=( 7*mcu+ 9*ncu)>>4;
	utab[5]=( 5*mcu+11*ncu)>>4;
	utab[6]=( 2*mcu+14*ncu)>>4;
	utab[7]=ncu;

	vtab[0]=mcv;
	vtab[1]=(14*mcv+ 2*ncv)>>4;
	vtab[2]=(11*mcv+ 5*ncv)>>4;
	vtab[3]=( 9*mcv+ 7*ncv)>>4;
	vtab[4]=( 7*mcv+ 9*ncv)>>4;
	vtab[5]=( 5*mcv+11*ncv)>>4;
	vtab[6]=( 2*mcv+14*ncv)>>4;
	vtab[7]=ncv;
	
	for(i=0; i<4; i++)
	{
		ct0=ibuf+(i*2+0)*ystr;
		ct1=ibuf+(i*2+1)*ystr;
		pxy0=btic4b_getu32le(blkbuf+16+i*6+0);
		pxy1=btic4b_getu32le(blkbuf+16+i*6+3);
		pxu=btic4b_getu32le(blkbuf+40+i*3);
		pxv=btic4b_getu32le(blkbuf+52+i*3);
		
		for(j=0; j<4; j++)
		{
			k=j*6;
			cy0=ytab[(pxy0>>(k+0))&7];
			cy1=ytab[(pxy0>>(k+3))&7];
			cy2=ytab[(pxy1>>(k+0))&7];
			cy3=ytab[(pxy1>>(k+3))&7];
			k=j*3;
			cu0=utab[(pxu>>(k+ 0))&7];
			cu1=utab[(pxu>>(k+12))&7];
			cv0=vtab[(pxv>>(k+ 0))&7];
			cv1=vtab[(pxv>>(k+12))&7];

			ctx->ClrDec4B(cy0, cy1, cy2, cy3,
				cu0, cv0, cu1, cv1,
				(u32 *)ct0, (u32 *)ct1);
			ct0+=8;		ct1+=8;
		}
	}
}

/* 8x8x4+UV8x8x3 */
void BTIC4B_DecBlock10BGRA(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	int ytab[16], utab[16], vtab[16];
	byte *ct0, *ct1;
	int acy, acu, acv, dcy, dcu, dcv;
	int mcy, mcu, mcv, ncy, ncu, ncv;

	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int cy0, cy1, cy2, cy3;
	int cy, cu0, cv0, cu1, cv1, cu2, cv2, cu3, cv3;

	u32 px0, px1, px2, px3;
	u32 pxy0, pxy1, pxu0, pxu1, pxv0, pxv1;
	int i, j, k;
	
	acy=*(s16 *)(blkbuf+ 4);
	acu=*(s16 *)(blkbuf+ 6);
	acv=*(s16 *)(blkbuf+ 8);
	dcy=*(s16 *)(blkbuf+10);
	dcu=*(s16 *)(blkbuf+12);
	dcv=*(s16 *)(blkbuf+14);
	
	mcy=acy-(dcy>>1); ncy=mcy+dcy;
	mcu=acu-(dcu>>1); ncu=mcu+dcu;
	mcv=acv-(dcv>>1); ncv=mcv+dcv;

	ytab[0]=mcy;
	ytab[ 1]=(30*mcy+ 2*ncy)>>5;	ytab[ 2]=(28*mcy+ 4*ncy)>>5;
	ytab[ 3]=(26*mcy+ 6*ncy)>>5;	ytab[ 4]=(23*mcy+ 9*ncy)>>5;
	ytab[ 5]=(21*mcy+11*ncy)>>5;	ytab[ 6]=(19*mcy+13*ncy)>>5;
	ytab[ 7]=(17*mcy+15*ncy)>>5;	ytab[ 8]=(15*mcy+17*ncy)>>5;
	ytab[ 9]=(13*mcy+19*ncy)>>5;	ytab[10]=(11*mcy+21*ncy)>>5;
	ytab[11]=( 9*mcy+23*ncy)>>5;	ytab[12]=( 6*mcy+26*ncy)>>5;
	ytab[13]=( 4*mcy+28*ncy)>>5;	ytab[14]=( 2*mcy+30*ncy)>>5;
	ytab[15]=ncy;

	utab[0]=mcu;
	utab[1]=(14*mcu+ 2*ncu)>>4;
	utab[2]=(11*mcu+ 5*ncu)>>4;
	utab[3]=( 9*mcu+ 7*ncu)>>4;
	utab[4]=( 7*mcu+ 9*ncu)>>4;
	utab[5]=( 5*mcu+11*ncu)>>4;
	utab[6]=( 2*mcu+14*ncu)>>4;
	utab[7]=ncu;

	vtab[0]=mcv;
	vtab[1]=(14*mcv+ 2*ncv)>>4;
	vtab[2]=(11*mcv+ 5*ncv)>>4;
	vtab[3]=( 9*mcv+ 7*ncv)>>4;
	vtab[4]=( 7*mcv+ 9*ncv)>>4;
	vtab[5]=( 5*mcv+11*ncv)>>4;
	vtab[6]=( 2*mcv+14*ncv)>>4;
	vtab[7]=ncv;
	
	for(i=0; i<4; i++)
	{
		ct0=ibuf+(i*2+0)*ystr;
		ct1=ibuf+(i*2+1)*ystr;
		pxy0=btic4b_getu32le(blkbuf+16+i*8+0);
		pxy1=btic4b_getu32le(blkbuf+16+i*8+4);
		pxu0=btic4b_getu32le(blkbuf+48+(i*6+0));
		pxu1=btic4b_getu32le(blkbuf+48+(i*6+3));
		pxv0=btic4b_getu32le(blkbuf+56+(i*6+0));
		pxv1=btic4b_getu32le(blkbuf+56+(i*6+3));
		
		for(j=0; j<4; j++)
		{
			k=j*8;
			cy0=ytab[(pxy0>>(k+0))&15];
			cy1=ytab[(pxy0>>(k+4))&15];
			cy2=ytab[(pxy1>>(k+0))&15];
			cy3=ytab[(pxy1>>(k+4))&15];

			k=j*6;
			cu0=utab[(pxu0>>(k+0))&7];
			cu1=utab[(pxu0>>(k+3))&7];
			cu2=utab[(pxu1>>(k+0))&7];
			cu3=utab[(pxu1>>(k+3))&7];
			cv0=vtab[(pxv0>>(k+0))&7];
			cv1=vtab[(pxv0>>(k+3))&7];
			cv2=vtab[(pxv1>>(k+0))&7];
			cv3=vtab[(pxv1>>(k+3))&7];

			ctx->ClrDec4C(cy0, cy1, cy2, cy3,
				cu0, cv0, cu1, cv1,
				cu2, cv2, cu3, cv3,
				(u32 *)ct0, (u32 *)ct1);
			ct0+=8;		ct1+=8;
		}
	}
}


force_inline void lqtvq_fillau32_8x1(byte *px, byte v)
{
	px[ 3]=v; px[ 7]=v; px[11]=v; px[15]=v;
	px[19]=v; px[23]=v; px[27]=v; px[31]=v;
}

force_inline void lqtvq_fillau32_8x1_2x1(byte *px, byte u, byte v)
{
	px[ 3]=u; px[ 7]=u; px[11]=u; px[15]=u;
	px[19]=v; px[23]=v; px[27]=v; px[31]=v;
}

force_inline void lqtvq_fillau32_8x1_4x1(byte *px,
	byte v0, byte v1, byte v2, byte v3)
{
	px[ 3]=v0; px[ 7]=v0; px[11]=v1; px[15]=v1;
	px[19]=v2; px[23]=v2; px[27]=v3; px[31]=v3;
}

force_inline void lqtvq_fillau32_8x1_8x1(byte *px,
	byte v0, byte v1, byte v2, byte v3,
	byte v4, byte v5, byte v6, byte v7)
{
	px[ 3]=v0; px[ 7]=v1; px[11]=v2; px[15]=v3;
	px[19]=v4; px[23]=v5; px[27]=v6; px[31]=v7;
}

void BTIC4B_DecBlockAlphaBGRA(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	s16 tab[16];
	byte *ct0, *ct1, *ct2, *ct3;
	u32 pxy0;
	byte px0, px1, px2, px3, px4, px5, px6, px7;
	byte px8, px9, pxA, pxB, pxC, pxD, pxE, pxF;
	int ma, na, ca, da;
	int i, j, k;
	
//	if(*(u16 *)(blkbuf+2)==0x00FF)
//		return;

	if((blkbuf[1]&0x1F)==0)
	{
		ca=blkbuf[2];
		lqtvq_fillau32_8x1(ibuf+0*ystr, ca);
		lqtvq_fillau32_8x1(ibuf+1*ystr, ca);
		lqtvq_fillau32_8x1(ibuf+2*ystr, ca);
		lqtvq_fillau32_8x1(ibuf+3*ystr, ca);
		lqtvq_fillau32_8x1(ibuf+4*ystr, ca);
		lqtvq_fillau32_8x1(ibuf+5*ystr, ca);
		lqtvq_fillau32_8x1(ibuf+6*ystr, ca);
		lqtvq_fillau32_8x1(ibuf+7*ystr, ca);
		return;
	}
	
	switch(blkbuf[1]&0x1F)
	{
	case 0x0C:
	case 0x0F:
		ca=blkbuf[2];
		da=blkbuf[3];

		ma=ca-(da>>1); na=ma+da;
		ma=lqtvq_clamp255(ma);
		na=lqtvq_clamp255(na);
		
		tab[0]=ma;
		tab[1]=(11*ma+ 5*na)>>4;
		tab[2]=( 5*ma+11*na)>>4;
		tab[3]=na;
		break;
	case 0x01:	case 0x02:	case 0x03:
		ca=*(s16 *)(blkbuf+48+0);
		da=*(s16 *)(blkbuf+48+2);
		ma=ca-(da>>1); na=ma+da;
		ma=lqtvq_clamp255(ma);
		na=lqtvq_clamp255(na);
		tab[0]=ma;
		tab[1]=na;
		break;

	case 0x04:	case 0x05:
	case 0x07:	case 0x08:
	case 0x0A:	case 0x0B:
		ca=*(s16 *)(blkbuf+48+0);
		da=*(s16 *)(blkbuf+48+2);
		ma=ca-(da>>1); na=ma+da;
		ma=lqtvq_clamp255(ma);
		na=lqtvq_clamp255(na);
		tab[0]=ma;
		tab[1]=(11*ma+ 5*na)>>4;
		tab[2]=( 5*ma+11*na)>>4;
		tab[3]=na;
		break;
	case 0x06:
		ca=*(s16 *)(blkbuf+48+0);
		da=*(s16 *)(blkbuf+48+2);
		ma=ca-(da>>1); na=ma+da;
		ma=lqtvq_clamp255(ma);
		na=lqtvq_clamp255(na);
		tab[0]=ma;
		tab[1]=(11*ma+ 5*na)>>4;
		tab[2]=( 5*ma+11*na)>>4;
		tab[3]=na;
		break;
	}

	switch(blkbuf[1]&0x1F)
	{
	case 0x00:
		break;
	case 0x01:
		pxy0=*(byte *)(blkbuf+48+8);
		px0=tab[(pxy0>>0)&1];
		px1=tab[(pxy0>>1)&1];
		px2=tab[(pxy0>>2)&1];
		px3=tab[(pxy0>>3)&1];
		ct0=ibuf+(0*4+0)*ystr;	ct1=ibuf+(0*4+1)*ystr;
		ct2=ibuf+(0*4+2)*ystr;	ct3=ibuf+(0*4+3)*ystr;
		lqtvq_fillau32_8x1_2x1(ct0, px0, px1);
		lqtvq_fillau32_8x1_2x1(ct1, px0, px1);
		lqtvq_fillau32_8x1_2x1(ct2, px0, px1);
		lqtvq_fillau32_8x1_2x1(ct3, px0, px1);
		ct0=ibuf+(1*4+0)*ystr;	ct1=ibuf+(1*4+1)*ystr;
		ct2=ibuf+(1*4+2)*ystr;	ct3=ibuf+(1*4+3)*ystr;
		lqtvq_fillau32_8x1_2x1(ct0, px2, px3);
		lqtvq_fillau32_8x1_2x1(ct1, px2, px3);
		lqtvq_fillau32_8x1_2x1(ct2, px2, px3);
		lqtvq_fillau32_8x1_2x1(ct3, px2, px3);
		break;
	case 0x02:
		pxy0=*(u16 *)(blkbuf+48+8);
		for(i=0; i<4; i++)
		{
			px0=tab[(pxy0>>(i*4+0))&1];
			px1=tab[(pxy0>>(i*4+1))&1];
			px2=tab[(pxy0>>(i*4+2))&1];
			px3=tab[(pxy0>>(i*4+3))&1];
			ct0=ibuf+(i*2+0)*ystr;	ct1=ibuf+(i*2+1)*ystr;
			lqtvq_fillau32_8x1_4x1(ct0, px0, px1, px2, px3);
			lqtvq_fillau32_8x1_4x1(ct1, px0, px1, px2, px3);
		}
		break;
	case 0x03:
		for(i=0; i<4; i++)
		{
			pxy0=*(u16 *)(blkbuf+48+8+i*2);
			pxF=tab[(pxy0>>15)&1];	pxE=tab[(pxy0>>14)&1];
			pxD=tab[(pxy0>>13)&1];	pxC=tab[(pxy0>>12)&1];
			pxB=tab[(pxy0>>11)&1];	pxA=tab[(pxy0>>10)&1];
			px9=tab[(pxy0>> 9)&1];	px8=tab[(pxy0>> 8)&1];
			px7=tab[(pxy0>> 7)&1];	px6=tab[(pxy0>> 6)&1];
			px5=tab[(pxy0>> 5)&1];	px4=tab[(pxy0>> 4)&1];
			px3=tab[(pxy0>> 3)&1];	px2=tab[(pxy0>> 2)&1];
			px1=tab[(pxy0>> 1)&1];	px0=tab[(pxy0>> 0)&1];
			lqtvq_fillau32_8x1_8x1(ibuf+(i*2+0)*ystr,
				px0, px1, px2, px3, px4, px5, px6, px7);
			lqtvq_fillau32_8x1_8x1(ibuf+(i*2+1)*ystr,
				px8, px9, pxA, pxB, pxC, pxD, pxE, pxF);
		}
		break;

	case 0x04:
		pxy0=*(byte *)(blkbuf+48+8);
		px0=tab[(pxy0>>0)&3];
		px1=tab[(pxy0>>2)&3];
		px2=tab[(pxy0>>4)&3];
		px3=tab[(pxy0>>6)&3];
		ct0=ibuf+(0*4+0)*ystr;	ct1=ibuf+(0*4+1)*ystr;
		ct2=ibuf+(0*4+2)*ystr;	ct3=ibuf+(0*4+3)*ystr;
		lqtvq_fillau32_8x1_2x1(ct0, px0, px1);
		lqtvq_fillau32_8x1_2x1(ct1, px0, px1);
		lqtvq_fillau32_8x1_2x1(ct2, px0, px1);
		lqtvq_fillau32_8x1_2x1(ct3, px0, px1);
		ct0=ibuf+(1*4+0)*ystr;	ct1=ibuf+(1*4+1)*ystr;
		ct2=ibuf+(1*4+2)*ystr;	ct3=ibuf+(1*4+3)*ystr;
		lqtvq_fillau32_8x1_2x1(ct0, px2, px3);
		lqtvq_fillau32_8x1_2x1(ct1, px2, px3);
		lqtvq_fillau32_8x1_2x1(ct2, px2, px3);
		lqtvq_fillau32_8x1_2x1(ct3, px2, px3);
		break;
	case 0x05:
		pxy0=btic4b_getu32le(blkbuf+48+8);
		for(i=0; i<4; i++)
		{
			px0=tab[(pxy0>>(i*8+0))&3];
			px1=tab[(pxy0>>(i*8+2))&3];
			px2=tab[(pxy0>>(i*8+4))&3];
			px3=tab[(pxy0>>(i*8+6))&3];
			ct0=ibuf+(i*2+0)*ystr;	ct1=ibuf+(i*2+1)*ystr;
			lqtvq_fillau32_8x1_4x1(ct0, px0, px1, px2, px3);
			lqtvq_fillau32_8x1_4x1(ct1, px0, px1, px2, px3);
		}
		break;
	case 0x0C:
		for(i=0; i<4; i++)
		{
			pxy0=btic4b_getu32le(blkbuf+48+i*4);
			pxF=tab[(pxy0>>30)&3];	pxE=tab[(pxy0>>28)&3];
			pxD=tab[(pxy0>>26)&3];	pxC=tab[(pxy0>>24)&3];
			pxB=tab[(pxy0>>22)&3];	pxA=tab[(pxy0>>20)&3];
			px9=tab[(pxy0>>18)&3];	px8=tab[(pxy0>>16)&3];
			px7=tab[(pxy0>>14)&3];	px6=tab[(pxy0>>12)&3];
			px5=tab[(pxy0>>10)&3];	px4=tab[(pxy0>> 8)&3];
			px3=tab[(pxy0>> 6)&3];	px2=tab[(pxy0>> 4)&3];
			px1=tab[(pxy0>> 2)&3];	px0=tab[(pxy0>> 0)&3];

			lqtvq_fillau32_8x1_8x1(ibuf+(i*2+0)*ystr,
				px0, px1, px2, px3, px4, px5, px6, px7);
			lqtvq_fillau32_8x1_8x1(ibuf+(i*2+1)*ystr,
				px8, px9, pxA, pxB, pxC, pxD, pxE, pxF);
		}
		break;
	default:
		break;
	}
}

void BTIC4B_DecBlockBGRA(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	if(!(*(u16 *)blkbuf))
	{
		BTIC4B_DecBlockFlatBGRA_FA(
			ctx, blkbuf, ibuf, ystr, blkbuf[2]);
		return;
	}

	switch(blkbuf[0]&0x1F)
	{
	case 0x00:
		if(*(u16 *)(blkbuf+2)!=0x00FF)
//		if(((btic4b_getu32leblkbuf)&0xFFFFFF00)!=0x00FF0000)
		{
//			if(!blkbuf[1] && !blkbuf[3])
			if(!(blkbuf[1]&0x1F))
			{
				BTIC4B_DecBlockFlatBGRA_FA(
					ctx, blkbuf, ibuf, ystr, blkbuf[2]);
				break;
			}

			BTIC4B_DecBlockFlatBGRA(ctx, blkbuf, ibuf, ystr);
			BTIC4B_DecBlockAlphaBGRA(ctx, blkbuf, ibuf, ystr);
			break;
		}
		BTIC4B_DecBlockFlatBGRA(ctx, blkbuf, ibuf, ystr);
		break;

	case 0x01: case 0x02:
	case 0x03: case 0x04:
	case 0x05: case 0x06:
	case 0x07:
		BTIC4B_DecBlock6BGRA(ctx, blkbuf, ibuf, ystr);
		if(*(u16 *)(blkbuf+2)!=0x00FF)
			BTIC4B_DecBlockAlphaBGRA(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x08:
		BTIC4B_DecBlockGrad4BGRA(ctx, blkbuf, ibuf, ystr);
		if(*(u16 *)(blkbuf+2)!=0x00FF)
			BTIC4B_DecBlockAlphaBGRA(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x09: case 0x0A:
	case 0x0B: case 0x0C:
	case 0x0D: case 0x0E:
	case 0x0F:
		BTIC4B_DecBlock1BGRA(ctx, blkbuf, ibuf, ystr);
		if(*(u16 *)(blkbuf+2)!=0x00FF)
//		if(((*(u32 *)blkbuf)&0xFFFFFF00)!=0x00FF0000)
			BTIC4B_DecBlockAlphaBGRA(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x10:
		BTIC4B_DecBlock2BGRA(ctx, blkbuf, ibuf, ystr);
		if(*(u16 *)(blkbuf+2)!=0x00FF)
//		if(((*(u32 *)blkbuf)&0xFFFFFF00)!=0x00FF0000)
			BTIC4B_DecBlockAlphaBGRA(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x11:
		BTIC4B_DecBlock3BGRA(ctx, blkbuf, ibuf, ystr);
		if(*(u16 *)(blkbuf+2)!=0x00FF)
//		if(((*(u32 *)blkbuf)&0xFFFFFF00)!=0x00FF0000)
			BTIC4B_DecBlockAlphaBGRA(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x12:
		BTIC4B_DecBlockFlatBGRA(ctx, blkbuf, ibuf, ystr); //TEMP
		break;
	case 0x13:
		BTIC4B_DecBlock5BGRA(ctx, blkbuf, ibuf, ystr);
		if(*(u16 *)(blkbuf+2)!=0x00FF)
			BTIC4B_DecBlockAlphaBGRA(ctx, blkbuf, ibuf, ystr);
		break;

	case 0x19:
		BTIC4B_DecBlock7BGRA(ctx, blkbuf, ibuf, ystr);
		break;

	case 0x14:
	case 0x15:	case 0x16:
	case 0x17:	case 0x18:
	case 0x1A:	case 0x1B:
		BTIC4B_DecBlock4BGRA(ctx, blkbuf, ibuf, ystr);
		if(*(u16 *)(blkbuf+2)!=0x00FF)
//		if(((*(u32 *)blkbuf)&0xFFFFFF00)!=0x00FF0000)
			BTIC4B_DecBlockAlphaBGRA(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x1C:
		BTIC4B_DecBlock0BGRA(ctx, blkbuf, ibuf, ystr);
		if(*(u16 *)(blkbuf+2)!=0x00FF)
//		if(((*(u32 *)blkbuf)&0xFFFFFF00)!=0x00FF0000)
			BTIC4B_DecBlockAlphaBGRA(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x1D:
		BTIC4B_DecBlock8BGRA(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x1E:
		BTIC4B_DecBlock9BGRA(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x1F:
		BTIC4B_DecBlock10BGRA(ctx, blkbuf, ibuf, ystr);
		break;

	default:
		break;
	}
	
//	ibuf[0]=blkbuf[0];
}

void BTIC4B_DecBlockBGRX(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	switch(blkbuf[0]&0x1F)
	{
	case 0x00:
		BTIC4B_DecBlockFlatBGRA(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x01: case 0x02:
	case 0x03: case 0x04:
	case 0x05: case 0x06:
	case 0x07:
		BTIC4B_DecBlock6BGRA(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x08:
		BTIC4B_DecBlockGrad4BGRA(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x09: case 0x0A:
	case 0x0B: case 0x0C:
	case 0x0D: case 0x0E:
	case 0x0F:
		BTIC4B_DecBlock1BGRA(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x10:
		BTIC4B_DecBlock2BGRA(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x11:
		BTIC4B_DecBlock3BGRA(ctx, blkbuf, ibuf, ystr);
		break;

	case 0x13:
		BTIC4B_DecBlock5BGRA(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x14:
	case 0x15:	case 0x16:
	case 0x17:	case 0x18:
	case 0x1A:	case 0x1B:
		BTIC4B_DecBlock4BGRA(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x1C:
		BTIC4B_DecBlock0BGRA(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x1D:
		BTIC4B_DecBlock8BGRA(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x1E:
		BTIC4B_DecBlock9BGRA(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x1F:
		BTIC4B_DecBlock10BGRA(ctx, blkbuf, ibuf, ystr);
		break;
	default:
		break;
	}
}

void BTIC4B_DecBlockHalfBGRA(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	u32 tbuf[64];
	u32 *ct0, *ct1, *ct2, *ct3;

	if(!(*(u16 *)blkbuf))
	{
		BTIC4B_DecBlockFlatHalfBGRA_FA(
			ctx, blkbuf, ibuf, ystr, blkbuf[2]);
		return;
	}

	BTIC4B_DecBlockBGRA(ctx, blkbuf, (byte *)tbuf, 8*4);
	ct0=(u32 *)(ibuf+0*ystr);	ct1=(u32 *)(ibuf+1*ystr);
	ct2=(u32 *)(ibuf+2*ystr);	ct3=(u32 *)(ibuf+3*ystr);
	ct0[0]=tbuf[ 0]; ct0[1]=tbuf[ 2]; ct0[2]=tbuf[ 4]; ct0[3]=tbuf[ 6];
	ct1[0]=tbuf[16]; ct1[1]=tbuf[18]; ct1[2]=tbuf[20]; ct1[3]=tbuf[22];
	ct2[0]=tbuf[32]; ct2[1]=tbuf[34]; ct2[2]=tbuf[36]; ct2[3]=tbuf[38];
	ct3[0]=tbuf[48]; ct3[1]=tbuf[50]; ct3[2]=tbuf[52]; ct3[3]=tbuf[54];
}

void BTIC4B_DecBlockHalfBGRX(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	u32 tbuf[64];
	u32 *ct0, *ct1, *ct2, *ct3;

	if(!(*(u16 *)blkbuf))
	{
		BTIC4B_DecBlockFlatHalfBGRA_FA(
			ctx, blkbuf, ibuf, ystr, 255);
		return;
	}

	BTIC4B_DecBlockBGRX(ctx, blkbuf, (byte *)tbuf, 8*4);
	ct0=(u32 *)(ibuf+0*ystr);	ct1=(u32 *)(ibuf+1*ystr);
	ct2=(u32 *)(ibuf+2*ystr);	ct3=(u32 *)(ibuf+3*ystr);
	ct0[0]=tbuf[ 0]; ct0[1]=tbuf[ 2]; ct0[2]=tbuf[ 4]; ct0[3]=tbuf[ 6];
	ct1[0]=tbuf[ 8]; ct1[1]=tbuf[10]; ct1[2]=tbuf[12]; ct1[3]=tbuf[14];
	ct2[0]=tbuf[16]; ct2[1]=tbuf[18]; ct2[2]=tbuf[20]; ct2[3]=tbuf[22];
	ct3[0]=tbuf[24]; ct3[1]=tbuf[26]; ct3[2]=tbuf[28]; ct3[3]=tbuf[30];
}

void BTIC4B_DecBlockBGR(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	u32 tbuf[256];
//	byte *ct0, *ct1, *ct2, *ct3;
	u32 *cs;
	byte *ct;
	int i;

	BTIC4B_DecBlockBGRX(ctx, blkbuf, (byte *)tbuf, 16*4);

//	ct0=(ibuf+0*ystr);	ct1=(ibuf+1*ystr);
//	ct2=(ibuf+2*ystr);	ct3=(ibuf+3*ystr);
	
	for(i=0; i<8; i++)
	{
		cs=tbuf+i*16;	ct=(ibuf+i*ystr);
		btic4b_setu24lef(ct+ 0, cs[0]);	btic4b_setu24lef(ct+ 3, cs[1]);
		btic4b_setu24lef(ct+ 6, cs[2]);	btic4b_setu24lef(ct+ 9, cs[3]);
		btic4b_setu24lef(ct+12, cs[4]);	btic4b_setu24lef(ct+15, cs[5]);
		btic4b_setu24lef(ct+18, cs[6]);	btic4b_setu24le (ct+21, cs[7]);
	}
}

#if 0
void BTIC4B_DecBlockRGBA(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	byte tbuf[64];
	int cu, cv, du, dv;
	int i, j;

	memcpy(tbuf, blkbuf, 64);
	cu=*(s16 *)(tbuf+ 6);
	cv=*(s16 *)(tbuf+ 8);
	du=*(s16 *)(tbuf+12);
	dv=*(s16 *)(tbuf+14);
	*(s16 *)(tbuf+ 6)=cv;
	*(s16 *)(tbuf+ 8)=cu;
	*(s16 *)(tbuf+12)=dv;
	*(s16 *)(tbuf+14)=du;
	BTIC4B_DecBlockBGRA(ctx, tbuf, ibuf, ystr);
}

void BTIC4B_DecBlockRGBX(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	byte tbuf[64];
	int cu, cv, du, dv;
	int i, j;

	memcpy(tbuf, blkbuf, 64);
	cu=*(s16 *)(tbuf+ 6);
	cv=*(s16 *)(tbuf+ 8);
	du=*(s16 *)(tbuf+12);
	dv=*(s16 *)(tbuf+14);
	*(s16 *)(tbuf+ 6)=cv;
	*(s16 *)(tbuf+ 8)=cu;
	*(s16 *)(tbuf+12)=dv;
	*(s16 *)(tbuf+14)=du;
	BTIC4B_DecBlockBGRX(ctx, tbuf, ibuf, ystr);
}
#endif

void BTIC4B_DecBlockEdgeClrs(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf,
	int ystr, int xf, int yf, int clrs)
{
	byte tbuf[64*4];
	u32 *cs, *ct;
	byte *ctb;
	int i, j;

	if(ctx->xstr==3)
	{
		BTIC4B_DecBlockBGRX(ctx, blkbuf, tbuf, 8*4);

		for(i=0; i<yf; i++)
		{
			cs=(u32 *)(tbuf+i*8*4);
			ctb=ibuf+i*ystr;
			for(j=0; j<xf; j++)
				{ btic4b_setu24le(ctb, *cs); ctb+=3; cs++; }
		}

		return;
	}

	ctx->DecBlock(ctx, blkbuf, tbuf, 8*4);
//	BTIC4B_DecBlockBGRA(ctx, blkbuf, tbuf, 8*4);
	if(xf==8)
	{
		for(i=0; i<yf; i++)
		{
			cs=(u32 *)(tbuf+i*8*4);
			ct=(u32 *)(ibuf+i*ystr);
			ct[0]=cs[0]; ct[1]=cs[1]; ct[2]=cs[2]; ct[3]=cs[3];
			ct[4]=cs[4]; ct[5]=cs[5]; ct[6]=cs[6]; ct[7]=cs[7];
		}
		return;
	}

	for(i=0; i<yf; i++)
	{
		cs=(u32 *)(tbuf+i*8*4);
		ct=(u32 *)(ibuf+i*ystr);
//		if(xf==8)
//		{	ct[0]=cs[0]; ct[1]=cs[1]; ct[2]=cs[2]; ct[3]=cs[3];
//			ct[4]=cs[4]; ct[5]=cs[5]; ct[6]=cs[6]; ct[7]=cs[7];
//			continue;	}
		j=xf;
		if(j>=4)
		{	ct[0]=cs[0]; ct[1]=cs[1]; ct[2]=cs[2]; ct[3]=cs[3];
			ct+=4; cs+=4; j-=4;		}
		if(j>=2)
		{	ct[0]=cs[0]; ct[1]=cs[1];
			ct+=2; cs+=2; j-=2;		}
		if(j)
			{ ct[0]=cs[0]; }
	}
}

u32 BTIC4B_DecImageClrPack1_LDR8(int cr, int cg, int cb)
{
	return(0xFF000000|(cr<<16)|(cg<<8)|cb);
}

u32 BTIC4B_DecImageClrPack1A_LDR8(int cr, int cg, int cb, int ca)
{
	return((ca<<24)|(cr<<16)|(cg<<8)|cb);
}

u32 BTIC4B_DecImageClrPack1_12R11F(int cr, int cg, int cb)
{
	return(((cb>>1)<<22)|(cg<<11)|cr);
}

u32 BTIC4B_DecImageClrPack1A_12R11F(int cr, int cg, int cb, int ca)
{
	return(((cb>>1)<<22)|(cg<<11)|cr);
}

u32 BTIC4B_DecImageClrPack1_16R11F(int cr, int cg, int cb)
{
	return(((cb>>5)<<22)|((cg>>4)<<11)|(cr>>4));
}

u32 BTIC4B_DecImageClrPack1A_16R11F(int cr, int cg, int cb, int ca)
{
	return(((cb>>5)<<22)|((cg>>4)<<11)|(cr>>4));
}

void BTIC4B_DecImageClrPack4_LDR8(
	int cr0, int cg0, int cb0,
	int cr3, int cg3, int cb3,
	int ca, u32 *tab)
{
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	cr1=(11*cr0+ 5*cr3)>>4;
	cg1=(11*cg0+ 5*cg3)>>4;
	cb1=(11*cb0+ 5*cb3)>>4;
	cr2=( 5*cr0+11*cr3)>>4;
	cg2=( 5*cg0+11*cg3)>>4;
	cb2=( 5*cb0+11*cb3)>>4;

	tab[0]=(ca<<24)|(cr0<<16)|(cg0<<8)|cb0;
	tab[1]=(ca<<24)|(cr1<<16)|(cg1<<8)|cb1;
	tab[2]=(ca<<24)|(cr2<<16)|(cg2<<8)|cb2;
	tab[3]=(ca<<24)|(cr3<<16)|(cg3<<8)|cb3;
}

void BTIC4B_DecImageClrPack4_12R11F(
	int cr0, int cg0, int cb0,
	int cr3, int cg3, int cb3,
	int ca, u32 *tab)
{
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	cr1=(11*cr0+ 5*cr3)>>4;
	cg1=(11*cg0+ 5*cg3)>>4;
	cb1=(11*cb0+ 5*cb3)>>4;
	cr2=( 5*cr0+11*cr3)>>4;
	cg2=( 5*cg0+11*cg3)>>4;
	cb2=( 5*cb0+11*cb3)>>4;

	tab[0]=((cb0>>1)<<22)|(cg0<<11)|cr0;
	tab[1]=((cb1>>1)<<22)|(cg1<<11)|cr1;
	tab[2]=((cb2>>1)<<22)|(cg2<<11)|cr2;
	tab[3]=((cb3>>1)<<22)|(cg3<<11)|cr3;
}

void BTIC4B_DecImageClrPack4_16R11F(
	int cr0, int cg0, int cb0,
	int cr3, int cg3, int cb3,
	int ca, u32 *tab)
{
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	cr1=(11*cr0+ 5*cr3)>>4;
	cg1=(11*cg0+ 5*cg3)>>4;
	cb1=(11*cb0+ 5*cb3)>>4;
	cr2=( 5*cr0+11*cr3)>>4;
	cg2=( 5*cg0+11*cg3)>>4;
	cb2=( 5*cb0+11*cb3)>>4;

	tab[0]=((cb0>>5)<<22)|((cg0>>4)<<11)|(cr0>>4);
	tab[1]=((cb1>>5)<<22)|((cg1>>4)<<11)|(cr1>>4);
	tab[2]=((cb2>>5)<<22)|((cg2>>4)<<11)|(cr2>>4);
	tab[3]=((cb3>>5)<<22)|((cg3>>4)<<11)|(cr3>>4);
}

u32 BTIC4B_DecImageClr1_GDbDr(int cy, int cu, int cv)
{
	u32 px;
	int cr, cg, cb;
	
	cr=cy+cv; cg=cy; cb=cy+cu;
	if((cr|cg|cb)>>8)
	{
		cr=lqtvq_clamp255(cr);
		cg=lqtvq_clamp255(cg);
		cb=lqtvq_clamp255(cb);
	}
	px=0xFF000000|(cr<<16)|(cg<<8)|cb;
	return(px);
}

u32 BTIC4B_DecImageClr1A_GDbDr(int cy, int cu, int cv, int ca)
{
	u32 px;
	int cr, cg, cb;
	
	cr=cy+cv; cg=cy; cb=cy+cu;
	if((cr|cg|cb|ca)>>8)
	{
		cr=lqtvq_clamp255(cr);
		cg=lqtvq_clamp255(cg);
		cb=lqtvq_clamp255(cb);
		ca=lqtvq_clamp255(ca);
	}
	px=(ca<<24)|(cr<<16)|(cg<<8)|cb;
	return(px);
}

void BTIC4B_DecImageClrDec2T_GDbDr(int tag,
	int acy, int acu, int acv,
	int dcy, int dcu, int dcv,
	int *rr0, int *rg0, int *rb0,
	int *rr1, int *rg1, int *rb1)
{
	int mcy, mcu, mcv;
	int ncy, ncu, ncv;
	int cr0, cg0, cb0;
	int cr3, cg3, cb3;

	mcy=acy-(dcy>>1); ncy=mcy+dcy;
	mcu=acu-(dcu>>1); ncu=mcu+dcu;
	mcv=acv-(dcv>>1); ncv=mcv+dcv;

	if(tag&0x40)
	{
		cg0=mcy; cb0=cg0+mcu; cr0=cg0+mcv;
		cg3=ncy; cb3=cg3+ncu; cr3=cg3+ncv;
	}else
	{
		cg0=mcy; cb0=cg0+acu; cr0=cg0+acv;
		cg3=ncy; cb3=cg3+acu; cr3=cg3+acv;
	}

	if((cr0|cg0|cb0)>>8)
	{
		cr0=lqtvq_clamp255(cr0);
		cg0=lqtvq_clamp255(cg0);
		cb0=lqtvq_clamp255(cb0);
	}

	if((cr3|cg3|cb3)>>8)
	{
		cr3=lqtvq_clamp255(cr3);
		cg3=lqtvq_clamp255(cg3);
		cb3=lqtvq_clamp255(cb3);
	}
	
	*rr0=cr0;	*rg0=cg0;	*rb0=cb0;
	*rr1=cr3;	*rg1=cg3;	*rb1=cb3;
}

void BTIC4B_DecImageClrDec4_GDbDr(
	int cy0, int cy1, int cy2, int cy3, int cu, int cv,
	u32 *rpx0, u32 *rpx1)
{
	u32 px0, px1, px2, px3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;

	cg0=cy0; cb0=cg0+cu; cr0=cg0+cv;
	cg1=cy1; cb1=cg1+cu; cr1=cg1+cv;
	cg2=cy2; cb2=cg2+cu; cr2=cg2+cv;
	cg3=cy3; cb3=cg3+cu; cr3=cg3+cv;
	
	if((cr0|cr1|cr2|cr3)>>8)
	{
		cr0=lqtvq_clamp255(cr0);
		cr1=lqtvq_clamp255(cr1);
		cr2=lqtvq_clamp255(cr2);
		cr3=lqtvq_clamp255(cr3);
	}

	if((cg0|cg1|cg2|cg3)>>8)
	{
		cg0=lqtvq_clamp255(cg0);
		cg1=lqtvq_clamp255(cg1);
		cg2=lqtvq_clamp255(cg2);
		cg3=lqtvq_clamp255(cg3);
	}

	if((cb0|cb1|cb2|cb3)>>8)
	{
		cb0=lqtvq_clamp255(cb0);
		cb1=lqtvq_clamp255(cb1);
		cb2=lqtvq_clamp255(cb2);
		cb3=lqtvq_clamp255(cb3);
	}
	
	px0=0xFF000000|(cr0<<16)|(cg0<<8)|cb0;
	px1=0xFF000000|(cr1<<16)|(cg1<<8)|cb1;
	px2=0xFF000000|(cr2<<16)|(cg2<<8)|cb2;
	px3=0xFF000000|(cr3<<16)|(cg3<<8)|cb3;
	rpx0[0]=px0;	rpx0[1]=px1;
	rpx1[0]=px2;	rpx1[1]=px3;
}

void BTIC4B_DecImageClrDec4B_GDbDr(
	int cy0, int cy1, int cy2, int cy3,
	int cu0, int cv0, int cu1, int cv1,
	u32 *rpx0, u32 *rpx1)
{
	u32 px0, px1, px2, px3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;

	cg0=cy0; cb0=cg0+cu0; cr0=cg0+cv0;
	cg1=cy1; cb1=cg1+cu0; cr1=cg1+cv0;
	cg2=cy2; cb2=cg2+cu1; cr2=cg2+cv1;
	cg3=cy3; cb3=cg3+cu1; cr3=cg3+cv1;
	
	if((cr0|cr1|cr2|cr3)>>8)
	{
		cr0=lqtvq_clamp255(cr0);
		cr1=lqtvq_clamp255(cr1);
		cr2=lqtvq_clamp255(cr2);
		cr3=lqtvq_clamp255(cr3);
	}

	if((cg0|cg1|cg2|cg3)>>8)
	{
		cg0=lqtvq_clamp255(cg0);
		cg1=lqtvq_clamp255(cg1);
		cg2=lqtvq_clamp255(cg2);
		cg3=lqtvq_clamp255(cg3);
	}

	if((cb0|cb1|cb2|cb3)>>8)
	{
		cb0=lqtvq_clamp255(cb0);
		cb1=lqtvq_clamp255(cb1);
		cb2=lqtvq_clamp255(cb2);
		cb3=lqtvq_clamp255(cb3);
	}
	
	px0=0xFF000000|(cr0<<16)|(cg0<<8)|cb0;
	px1=0xFF000000|(cr1<<16)|(cg1<<8)|cb1;
	px2=0xFF000000|(cr2<<16)|(cg2<<8)|cb2;
	px3=0xFF000000|(cr3<<16)|(cg3<<8)|cb3;
	rpx0[0]=px0;	rpx0[1]=px1;
	rpx1[0]=px2;	rpx1[1]=px3;
}

void BTIC4B_DecImageClrDec4C_GDbDr(
	int cy0, int cy1, int cy2, int cy3,
	int cu0, int cv0, int cu1, int cv1,
	int cu2, int cv2, int cu3, int cv3,
	u32 *rpx0, u32 *rpx1)
{
	u32 px0, px1, px2, px3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;

	cg0=cy0; cb0=cg0+cu0; cr0=cg0+cv0;
	cg1=cy1; cb1=cg1+cu1; cr1=cg1+cv1;
	cg2=cy2; cb2=cg2+cu2; cr2=cg2+cv2;
	cg3=cy3; cb3=cg3+cu3; cr3=cg3+cv3;
	
	if((cr0|cr1|cr2|cr3)>>8)
	{
		cr0=lqtvq_clamp255(cr0);
		cr1=lqtvq_clamp255(cr1);
		cr2=lqtvq_clamp255(cr2);
		cr3=lqtvq_clamp255(cr3);
	}

	if((cg0|cg1|cg2|cg3)>>8)
	{
		cg0=lqtvq_clamp255(cg0);
		cg1=lqtvq_clamp255(cg1);
		cg2=lqtvq_clamp255(cg2);
		cg3=lqtvq_clamp255(cg3);
	}

	if((cb0|cb1|cb2|cb3)>>8)
	{
		cb0=lqtvq_clamp255(cb0);
		cb1=lqtvq_clamp255(cb1);
		cb2=lqtvq_clamp255(cb2);
		cb3=lqtvq_clamp255(cb3);
	}
	
	px0=0xFF000000|(cr0<<16)|(cg0<<8)|cb0;
	px1=0xFF000000|(cr1<<16)|(cg1<<8)|cb1;
	px2=0xFF000000|(cr2<<16)|(cg2<<8)|cb2;
	px3=0xFF000000|(cr3<<16)|(cg3<<8)|cb3;
	rpx0[0]=px0;	rpx0[1]=px1;
	rpx1[0]=px2;	rpx1[1]=px3;
}

u32 BTIC4B_DecImageClr1_GDbDrSW(int cy, int cu, int cv)
	{ return(BTIC4B_DecImageClr1_GDbDr(cy, cv, cu)); }
u32 BTIC4B_DecImageClr1A_GDbDrSW(int cy, int cu, int cv, int ca)
	{ return(BTIC4B_DecImageClr1A_GDbDr(cy, cv, cu, ca)); }

void BTIC4B_DecImageClrDec2T_GDbDrSW(int tag,
	int acy, int acu, int acv,
	int dcy, int dcu, int dcv,
	int *rr0, int *rg0, int *rb0,
	int *rr1, int *rg1, int *rb1)
{
	BTIC4B_DecImageClrDec2T_GDbDr(
		tag, acy, acv, acu, dcy, dcv, dcu,
		rr0, rg0, rb0, rr1, rg1, rb1);
}

void BTIC4B_DecImageClrDec4_GDbDrSW(
	int cy0, int cy1, int cy2, int cy3, int cu, int cv,
	u32 *rpx0, u32 *rpx1)
{
	BTIC4B_DecImageClrDec4_GDbDr(
		cy0, cy1, cy2, cy3, cv, cu, rpx0, rpx1);
}

void BTIC4B_DecImageClrDec4B_GDbDrSW(
	int cy0, int cy1, int cy2, int cy3,
	int cu0, int cv0, int cu1, int cv1,
	u32 *rpx0, u32 *rpx1)
{
	BTIC4B_DecImageClrDec4B_GDbDr(
		cy0, cy1, cy2, cy3, cv0, cu0, cv1, cu1, rpx0, rpx1);
}

void BTIC4B_DecImageClrDec4C_GDbDrSW(
	int cy0, int cy1, int cy2, int cy3,
	int cu0, int cv0, int cu1, int cv1,
	int cu2, int cv2, int cu3, int cv3,
	u32 *rpx0, u32 *rpx1)
{
	BTIC4B_DecImageClrDec4C_GDbDr(
		cy0, cy1, cy2, cy3, cv0, cu0, cv1, cu1,
		cv2, cu2, cv3, cu3, rpx0, rpx1);
}

#if 1
u32 BTIC4B_DecImageClr1_RCT(int cy, int cu, int cv)
{
	u32 px;
	int cr, cg, cb;
	
	cg=cy-((cu+cv)>>2);
	cr=cg+cv;	cb=cg+cu;
	if((cr|cg|cb)>>8)
	{
		cr=lqtvq_clamp255(cr);
		cg=lqtvq_clamp255(cg);
		cb=lqtvq_clamp255(cb);
	}
	px=0xFF000000|(cr<<16)|(cg<<8)|cb;
	return(px);
}

u32 BTIC4B_DecImageClr1A_RCT(int cy, int cu, int cv, int ca)
{
	u32 px;
	int cr, cg, cb;
	
	cg=cy-((cu+cv)>>2);
	cr=cg+cv;	cb=cg+cu;
	if((cr|cg|cb|ca)>>8)
	{
		cr=lqtvq_clamp255(cr);
		cg=lqtvq_clamp255(cg);
		cb=lqtvq_clamp255(cb);
		ca=lqtvq_clamp255(ca);
	}
	px=(ca<<24)|(cr<<16)|(cg<<8)|cb;
	return(px);
}

void BTIC4B_DecImageClrDec2T_RCT(int tag,
	int acy, int acu, int acv,
	int dcy, int dcu, int dcv,
	int *rr0, int *rg0, int *rb0,
	int *rr1, int *rg1, int *rb1)
{
	int mcy, mcu, mcv;
	int ncy, ncu, ncv;
	int cr0, cg0, cb0;
	int cr3, cg3, cb3;

	mcy=acy-(dcy>>1); ncy=mcy+dcy;
	mcu=acu-(dcu>>1); ncu=mcu+dcu;
	mcv=acv-(dcv>>1); ncv=mcv+dcv;

	if(tag&0x40)
	{
		cg0=mcy-((mcu+mcv)>>2); cb0=cg0+mcu; cr0=cg0+mcv;
		cg3=ncy-((ncu+ncv)>>2); cb3=cg3+ncu; cr3=cg3+ncv;
	}else
	{
		cg0=mcy-((acu+acv)>>2); cb0=cg0+acu; cr0=cg0+acv;
		cg3=ncy-((acu+acv)>>2); cb3=cg3+acu; cr3=cg3+acv;
	}

	if((cr0|cg0|cb0)>>8)
	{
		cr0=lqtvq_clamp255(cr0);
		cg0=lqtvq_clamp255(cg0);
		cb0=lqtvq_clamp255(cb0);
	}

	if((cr3|cg3|cb3)>>8)
	{
		cr3=lqtvq_clamp255(cr3);
		cg3=lqtvq_clamp255(cg3);
		cb3=lqtvq_clamp255(cb3);
	}
	
	*rr0=cr0;	*rg0=cg0;	*rb0=cb0;
	*rr1=cr3;	*rg1=cg3;	*rb1=cb3;
}

void BTIC4B_DecImageClrDec4_RCT(
	int cy0, int cy1, int cy2, int cy3, int cu, int cv,
	u32 *rpx0, u32 *rpx1)
{
	u32 px0, px1, px2, px3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;

	cg0=cy0-((cu+cv)>>2); cb0=cg0+cu; cr0=cg0+cv;
	cg1=cy1-((cu+cv)>>2); cb1=cg1+cu; cr1=cg1+cv;
	cg2=cy2-((cu+cv)>>2); cb2=cg2+cu; cr2=cg2+cv;
	cg3=cy3-((cu+cv)>>2); cb3=cg3+cu; cr3=cg3+cv;
	
	if((cr0|cr1|cr2|cr3)>>8)
	{
		cr0=lqtvq_clamp255(cr0);
		cr1=lqtvq_clamp255(cr1);
		cr2=lqtvq_clamp255(cr2);
		cr3=lqtvq_clamp255(cr3);
	}

	if((cg0|cg1|cg2|cg3)>>8)
	{
		cg0=lqtvq_clamp255(cg0);
		cg1=lqtvq_clamp255(cg1);
		cg2=lqtvq_clamp255(cg2);
		cg3=lqtvq_clamp255(cg3);
	}

	if((cb0|cb1|cb2|cb3)>>8)
	{
		cb0=lqtvq_clamp255(cb0);
		cb1=lqtvq_clamp255(cb1);
		cb2=lqtvq_clamp255(cb2);
		cb3=lqtvq_clamp255(cb3);
	}
	
	px0=0xFF000000|(cr0<<16)|(cg0<<8)|cb0;
	px1=0xFF000000|(cr1<<16)|(cg1<<8)|cb1;
	px2=0xFF000000|(cr2<<16)|(cg2<<8)|cb2;
	px3=0xFF000000|(cr3<<16)|(cg3<<8)|cb3;
	rpx0[0]=px0;	rpx0[1]=px1;
	rpx1[0]=px2;	rpx1[1]=px3;
}

void BTIC4B_DecImageClrDec4B_RCT(
	int cy0, int cy1, int cy2, int cy3,
	int cu0, int cv0, int cu1, int cv1,
	u32 *rpx0, u32 *rpx1)
{
	u32 px0, px1, px2, px3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;

	cg0=cy0-((cu0+cv0)>>2); cb0=cg0+cu0; cr0=cg0+cv0;
	cg1=cy1-((cu0+cv0)>>2); cb1=cg1+cu0; cr1=cg1+cv0;
	cg2=cy2-((cu1+cv1)>>2); cb2=cg2+cu1; cr2=cg2+cv1;
	cg3=cy3-((cu1+cv1)>>2); cb3=cg3+cu1; cr3=cg3+cv1;
	
	if((cr0|cr1|cr2|cr3)>>8)
	{
		cr0=lqtvq_clamp255(cr0);
		cr1=lqtvq_clamp255(cr1);
		cr2=lqtvq_clamp255(cr2);
		cr3=lqtvq_clamp255(cr3);
	}

	if((cg0|cg1|cg2|cg3)>>8)
	{
		cg0=lqtvq_clamp255(cg0);
		cg1=lqtvq_clamp255(cg1);
		cg2=lqtvq_clamp255(cg2);
		cg3=lqtvq_clamp255(cg3);
	}

	if((cb0|cb1|cb2|cb3)>>8)
	{
		cb0=lqtvq_clamp255(cb0);
		cb1=lqtvq_clamp255(cb1);
		cb2=lqtvq_clamp255(cb2);
		cb3=lqtvq_clamp255(cb3);
	}
	
	px0=0xFF000000|(cr0<<16)|(cg0<<8)|cb0;
	px1=0xFF000000|(cr1<<16)|(cg1<<8)|cb1;
	px2=0xFF000000|(cr2<<16)|(cg2<<8)|cb2;
	px3=0xFF000000|(cr3<<16)|(cg3<<8)|cb3;
	rpx0[0]=px0;	rpx0[1]=px1;
	rpx1[0]=px2;	rpx1[1]=px3;
}

void BTIC4B_DecImageClrDec4C_RCT(
	int cy0, int cy1, int cy2, int cy3,
	int cu0, int cv0, int cu1, int cv1,
	int cu2, int cv2, int cu3, int cv3,
	u32 *rpx0, u32 *rpx1)
{
	u32 px0, px1, px2, px3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;

	cg0=cy0-((cu0+cv0)>>2); cb0=cg0+cu0; cr0=cg0+cv0;
	cg1=cy1-((cu1+cv1)>>2); cb1=cg1+cu1; cr1=cg1+cv1;
	cg2=cy2-((cu2+cv2)>>2); cb2=cg2+cu2; cr2=cg2+cv2;
	cg3=cy3-((cu3+cv3)>>2); cb3=cg3+cu3; cr3=cg3+cv3;
	
	if((cr0|cr1|cr2|cr3)>>8)
	{
		cr0=lqtvq_clamp255(cr0);
		cr1=lqtvq_clamp255(cr1);
		cr2=lqtvq_clamp255(cr2);
		cr3=lqtvq_clamp255(cr3);
	}

	if((cg0|cg1|cg2|cg3)>>8)
	{
		cg0=lqtvq_clamp255(cg0);
		cg1=lqtvq_clamp255(cg1);
		cg2=lqtvq_clamp255(cg2);
		cg3=lqtvq_clamp255(cg3);
	}

	if((cb0|cb1|cb2|cb3)>>8)
	{
		cb0=lqtvq_clamp255(cb0);
		cb1=lqtvq_clamp255(cb1);
		cb2=lqtvq_clamp255(cb2);
		cb3=lqtvq_clamp255(cb3);
	}
	
	px0=0xFF000000|(cr0<<16)|(cg0<<8)|cb0;
	px1=0xFF000000|(cr1<<16)|(cg1<<8)|cb1;
	px2=0xFF000000|(cr2<<16)|(cg2<<8)|cb2;
	px3=0xFF000000|(cr3<<16)|(cg3<<8)|cb3;
	rpx0[0]=px0;	rpx0[1]=px1;
	rpx1[0]=px2;	rpx1[1]=px3;
}


u32 BTIC4B_DecImageClr1_RCTSW(int cy, int cu, int cv)
	{ return(BTIC4B_DecImageClr1_RCT(cy, cv, cu)); }
u32 BTIC4B_DecImageClr1A_RCTSW(int cy, int cu, int cv, int ca)
{
	u32 px;
	int cr, cg, cb;
	
	cg=cy-((cu+cv)>>2);
	cr=cg+cv;	cb=cg+cu;
	if((cr|cg|cb|ca)>>8)
//	if((cr|cg|cb)>>8)
	{
		cr=lqtvq_clamp255(cr);
		cg=lqtvq_clamp255(cg);
		cb=lqtvq_clamp255(cb);
		ca=lqtvq_clamp255(ca);
	}
	px=(ca<<24)|(cb<<16)|(cg<<8)|cr;
	return(px);

//	return(BTIC4B_DecImageClr1A_RCT(cy, cv, cu, ca));
}

void BTIC4B_DecImageClrDec2T_RCTSW(int tag,
	int acy, int acu, int acv,
	int dcy, int dcu, int dcv,
	int *rr0, int *rg0, int *rb0,
	int *rr1, int *rg1, int *rb1)
{
	BTIC4B_DecImageClrDec2T_RCT(
		tag, acy, acv, acu, dcy, dcv, dcu,
		rr0, rg0, rb0, rr1, rg1, rb1);
}

void BTIC4B_DecImageClrDec4_RCTSW(
	int cy0, int cy1, int cy2, int cy3, int cu, int cv,
	u32 *rpx0, u32 *rpx1)
{
	BTIC4B_DecImageClrDec4_RCT(
		cy0, cy1, cy2, cy3, cv, cu,
		rpx0, rpx1);
}

void BTIC4B_DecImageClrDec4B_RCTSW(
	int cy0, int cy1, int cy2, int cy3,
	int cu0, int cv0, int cu1, int cv1,
	u32 *rpx0, u32 *rpx1)
{
	BTIC4B_DecImageClrDec4B_RCT(
		cy0, cy1, cy2, cy3,
		cv0, cu0, cv1, cu1,
		rpx0, rpx1);
}

void BTIC4B_DecImageClrDec4C_RCTSW(
	int cy0, int cy1, int cy2, int cy3,
	int cu0, int cv0, int cu1, int cv1,
	int cu2, int cv2, int cu3, int cv3,
	u32 *rpx0, u32 *rpx1)
{
	BTIC4B_DecImageClrDec4C_RCT(
		cy0, cy1, cy2, cy3,
		cv0, cu0, cv1, cu1,
		cv2, cu2, cv3, cu3,
		rpx0, rpx1);
}

#endif

#if 1
u32 BTIC4B_DecImageClr1_RCT16F_11F(int cy, int cu, int cv)
{
	u32 px;
	int cr, cg, cb;
	
	cg=cy-((cu+cv)>>2);
	cr=cg+cv;	cb=cg+cu;
	if((cr|cg|cb)>>15)
	{
		cr=lqtvq_clamp32767U(cr);
		cg=lqtvq_clamp32767U(cg);
		cb=lqtvq_clamp32767U(cb);
	}
	
	cr=cr>>4;	cg=cg>>4;	cb=cb>>5;
	px=(cb<<22)|(cg<<11)|cr;
	return(px);
}

u32 BTIC4B_DecImageClr1A_RCT16F_11F(int cy, int cu, int cv, int ca)
	{ return(BTIC4B_DecImageClr1_RCT16F_11F(cy, cu, cv)); }

void BTIC4B_DecImageClrDec2T_RCT16F_11F(int tag,
	int acy, int acu, int acv,
	int dcy, int dcu, int dcv,
	int *rr0, int *rg0, int *rb0,
	int *rr1, int *rg1, int *rb1)
{
	int mcy, mcu, mcv;
	int ncy, ncu, ncv;
	int cr0, cg0, cb0;
	int cr3, cg3, cb3;

	mcy=acy-(dcy>>1); ncy=mcy+dcy;
	mcu=acu-(dcu>>1); ncu=mcu+dcu;
	mcv=acv-(dcv>>1); ncv=mcv+dcv;

	if(tag&0x40)
	{
		cg0=mcy-((mcu+mcv)>>2); cb0=cg0+mcu; cr0=cg0+mcv;
		cg3=ncy-((ncu+ncv)>>2); cb3=cg3+ncu; cr3=cg3+ncv;
	}else
	{
		cg0=mcy-((acu+acv)>>2); cb0=cg0+acu; cr0=cg0+acv;
		cg3=ncy-((acu+acv)>>2); cb3=cg3+acu; cr3=cg3+acv;
	}

	if((cr0|cg0|cb0)>>15)
	{
		cr0=lqtvq_clamp32767U(cr0);
		cg0=lqtvq_clamp32767U(cg0);
		cb0=lqtvq_clamp32767U(cb0);
	}

	if((cr3|cg3|cb3)>>15)
	{
		cr3=lqtvq_clamp32767U(cr3);
		cg3=lqtvq_clamp32767U(cg3);
		cb3=lqtvq_clamp32767U(cb3);
	}
	
	*rr0=cr0;	*rg0=cg0;	*rb0=cb0;
	*rr1=cr3;	*rg1=cg3;	*rb1=cb3;
}

void BTIC4B_DecImageClrDec4_RCT16F_11F(
	int cy0, int cy1, int cy2, int cy3, int cu, int cv,
	u32 *rpx0, u32 *rpx1)
{
	u32 px0, px1, px2, px3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;

	cg0=cy0-((cu+cv)>>2); cb0=cg0+cu; cr0=cg0+cv;
	cg1=cy1-((cu+cv)>>2); cb1=cg1+cu; cr1=cg1+cv;
	cg2=cy2-((cu+cv)>>2); cb2=cg2+cu; cr2=cg2+cv;
	cg3=cy3-((cu+cv)>>2); cb3=cg3+cu; cr3=cg3+cv;
	
	if((cr0|cr1|cr2|cr3)>>15)
	{
		cr0=lqtvq_clamp32767U(cr0);
		cr1=lqtvq_clamp32767U(cr1);
		cr2=lqtvq_clamp32767U(cr2);
		cr3=lqtvq_clamp32767U(cr3);
	}

	if((cg0|cg1|cg2|cg3)>>15)
	{
		cg0=lqtvq_clamp32767U(cg0);
		cg1=lqtvq_clamp32767U(cg1);
		cg2=lqtvq_clamp32767U(cg2);
		cg3=lqtvq_clamp32767U(cg3);
	}

	if((cb0|cb1|cb2|cb3)>>15)
	{
		cb0=lqtvq_clamp32767U(cb0);
		cb1=lqtvq_clamp32767U(cb1);
		cb2=lqtvq_clamp32767U(cb2);
		cb3=lqtvq_clamp32767U(cb3);
	}

//	cr0=(cr0+7)>>4;	cg0=(cg0+7)>>4;	cb0=(cb0+15)>>5;
//	cr1=(cr1+7)>>4;	cg1=(cg1+7)>>4;	cb1=(cb1+15)>>5;
//	cr2=(cr2+7)>>4;	cg2=(cg2+7)>>4;	cb2=(cb2+15)>>5;
//	cr3=(cr3+7)>>4;	cg3=(cg3+7)>>4;	cb3=(cb3+15)>>5;
	
	cr0=cr0>>4;	cg0=cg0>>4;	cb0=cb0>>5;
	cr1=cr1>>4;	cg1=cg1>>4;	cb1=cb1>>5;
	cr2=cr2>>4;	cg2=cg2>>4;	cb2=cb2>>5;
	cr3=cr3>>4;	cg3=cg3>>4;	cb3=cb3>>5;
	px0=(cb0<<22)|(cg0<<11)|cr0;
	px1=(cb1<<22)|(cg1<<11)|cr1;
	px2=(cb2<<22)|(cg2<<11)|cr2;
	px3=(cb3<<22)|(cg3<<11)|cr3;
	rpx0[0]=px0;	rpx0[1]=px1;
	rpx1[0]=px2;	rpx1[1]=px3;
}

void BTIC4B_DecImageClrDec4B_RCT16F_11F(
	int cy0, int cy1, int cy2, int cy3,
	int cu0, int cv0, int cu1, int cv1,
	u32 *rpx0, u32 *rpx1)
{
	u32 px0, px1, px2, px3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;

	cg0=cy0-((cu0+cv0)>>2); cb0=cg0+cu0; cr0=cg0+cv0;
	cg1=cy1-((cu0+cv0)>>2); cb1=cg1+cu0; cr1=cg1+cv0;
	cg2=cy2-((cu1+cv1)>>2); cb2=cg2+cu1; cr2=cg2+cv1;
	cg3=cy3-((cu1+cv1)>>2); cb3=cg3+cu1; cr3=cg3+cv1;
	
	if((cr0|cr1|cr2|cr3)>>15)
	{
		cr0=lqtvq_clamp32767U(cr0);
		cr1=lqtvq_clamp32767U(cr1);
		cr2=lqtvq_clamp32767U(cr2);
		cr3=lqtvq_clamp32767U(cr3);
	}

	if((cg0|cg1|cg2|cg3)>>15)
	{
		cg0=lqtvq_clamp32767U(cg0);
		cg1=lqtvq_clamp32767U(cg1);
		cg2=lqtvq_clamp32767U(cg2);
		cg3=lqtvq_clamp32767U(cg3);
	}

	if((cb0|cb1|cb2|cb3)>>15)
	{
		cb0=lqtvq_clamp32767U(cb0);
		cb1=lqtvq_clamp32767U(cb1);
		cb2=lqtvq_clamp32767U(cb2);
		cb3=lqtvq_clamp32767U(cb3);
	}

	cr0=cr0>>4;	cg0=cg0>>4;	cb0=cb0>>5;
	cr1=cr1>>4;	cg1=cg1>>4;	cb1=cb1>>5;
	cr2=cr2>>4;	cg2=cg2>>4;	cb2=cb2>>5;
	cr3=cr3>>4;	cg3=cg3>>4;	cb3=cb3>>5;

	px0=(cb0<<22)|(cg0<<11)|cr0;
	px1=(cb1<<22)|(cg1<<11)|cr1;
	px2=(cb2<<22)|(cg2<<11)|cr2;
	px3=(cb3<<22)|(cg3<<11)|cr3;
	rpx0[0]=px0;	rpx0[1]=px1;
	rpx1[0]=px2;	rpx1[1]=px3;
}

void BTIC4B_DecImageClrDec4C_RCT16F_11F(
	int cy0, int cy1, int cy2, int cy3,
	int cu0, int cv0, int cu1, int cv1,
	int cu2, int cv2, int cu3, int cv3,
	u32 *rpx0, u32 *rpx1)
{
	u32 px0, px1, px2, px3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;

	cg0=cy0-((cu0+cv0)>>2); cb0=cg0+cu0; cr0=cg0+cv0;
	cg1=cy1-((cu1+cv1)>>2); cb1=cg1+cu1; cr1=cg1+cv1;
	cg2=cy2-((cu2+cv2)>>2); cb2=cg2+cu2; cr2=cg2+cv2;
	cg3=cy3-((cu3+cv3)>>2); cb3=cg3+cu3; cr3=cg3+cv3;
	
	if((cr0|cr1|cr2|cr3)>>8)
	{
		cr0=lqtvq_clamp32767U(cr0);
		cr1=lqtvq_clamp32767U(cr1);
		cr2=lqtvq_clamp32767U(cr2);
		cr3=lqtvq_clamp32767U(cr3);
	}

	if((cg0|cg1|cg2|cg3)>>8)
	{
		cg0=lqtvq_clamp32767U(cg0);
		cg1=lqtvq_clamp32767U(cg1);
		cg2=lqtvq_clamp32767U(cg2);
		cg3=lqtvq_clamp32767U(cg3);
	}

	if((cb0|cb1|cb2|cb3)>>8)
	{
		cb0=lqtvq_clamp32767U(cb0);
		cb1=lqtvq_clamp32767U(cb1);
		cb2=lqtvq_clamp32767U(cb2);
		cb3=lqtvq_clamp32767U(cb3);
	}

	cr0=cr0>>4;	cg0=cg0>>4;	cb0=cb0>>5;
	cr1=cr1>>4;	cg1=cg1>>4;	cb1=cb1>>5;
	cr2=cr2>>4;	cg2=cg2>>4;	cb2=cb2>>5;
	cr3=cr3>>4;	cg3=cg3>>4;	cb3=cb3>>5;

	px0=(cb0<<22)|(cg0<<11)|cr0;
	px1=(cb1<<22)|(cg1<<11)|cr1;
	px2=(cb2<<22)|(cg2<<11)|cr2;
	px3=(cb3<<22)|(cg3<<11)|cr3;
	rpx0[0]=px0;	rpx0[1]=px1;
	rpx1[0]=px2;	rpx1[1]=px3;
}


u32 btic4b_pxrgbx2rgb11f(u32 px)
{
	u32 px2;
	int cr, cg, cb;
	cr=lqtvq_btohf[(px>>16)&255];
	cg=lqtvq_btohf[(px>> 8)&255];
	cb=lqtvq_btohf[(px    )&255];
	cr=cr>>4;	cg=cg>>4;	cb=cb>>5;
	px2=(cb<<22)|(cg<<11)|cr;
	return(px2);
}

u32 BTIC4B_DecImageClr1_RCT_11FB(int cy, int cu, int cv)
{
	u32 px0, px1;
	px0=BTIC4B_DecImageClr1_RCT(cy, cu, cv);
	px1=btic4b_pxrgbx2rgb11f(px0);
	return(px1);
}

u32 BTIC4B_DecImageClr1A_RCT_11FB(int cy, int cu, int cv, int ca)
{
	u32 px0, px1;
	px0=BTIC4B_DecImageClr1A_RCT(cy, cu, cv, ca);
	px1=btic4b_pxrgbx2rgb11f(px0);
	return(px1);
}

void BTIC4B_DecImageClrDec2T_RCT_11FB(int tag,
	int acy, int acu, int acv,
	int dcy, int dcu, int dcv,
	int *rr0, int *rg0, int *rb0,
	int *rr1, int *rg1, int *rb1)
{
	BTIC4B_DecImageClrDec2T_RCT(tag,
		acy, acu, acv, dcy, dcu, dcv,
		rr0, rg0, rb0, rr1, rg1, rb1);
	*rr0=lqtvq_btohf[lqtvq_clamp255(*rr0)];
	*rg0=lqtvq_btohf[lqtvq_clamp255(*rg0)];
	*rb0=lqtvq_btohf[lqtvq_clamp255(*rb0)];
	*rr1=lqtvq_btohf[lqtvq_clamp255(*rr1)];
	*rg1=lqtvq_btohf[lqtvq_clamp255(*rg1)];
	*rb1=lqtvq_btohf[lqtvq_clamp255(*rb1)];
}


void BTIC4B_DecImageClrDec4_RCT_11FB(
	int cy0, int cy1, int cy2, int cy3, int cu, int cv,
	u32 *rpx0, u32 *rpx1)
{
	BTIC4B_DecImageClrDec4_RCT(cy0, cy1, cy2, cy3, cu, cv, rpx0, rpx1);
	rpx0[0]=btic4b_pxrgbx2rgb11f(rpx0[0]);
	rpx0[1]=btic4b_pxrgbx2rgb11f(rpx0[1]);
	rpx1[0]=btic4b_pxrgbx2rgb11f(rpx1[0]);
	rpx1[1]=btic4b_pxrgbx2rgb11f(rpx1[1]);
}

void BTIC4B_DecImageClrDec4B_RCT_11FB(
	int cy0, int cy1, int cy2, int cy3,
	int cu0, int cv0, int cu1, int cv1,
	u32 *rpx0, u32 *rpx1)
{
	BTIC4B_DecImageClrDec4B_RCT(cy0, cy1, cy2, cy3,
		cu0, cv0, cu1, cv1, rpx0, rpx1);
	rpx0[0]=btic4b_pxrgbx2rgb11f(rpx0[0]);
	rpx0[1]=btic4b_pxrgbx2rgb11f(rpx0[1]);
	rpx1[0]=btic4b_pxrgbx2rgb11f(rpx1[0]);
	rpx1[1]=btic4b_pxrgbx2rgb11f(rpx1[1]);
}

void BTIC4B_DecImageClrDec4C_RCT_11FB(
	int cy0, int cy1, int cy2, int cy3,
	int cu0, int cv0, int cu1, int cv1,
	int cu2, int cv2, int cu3, int cv3,
	u32 *rpx0, u32 *rpx1)
{
	BTIC4B_DecImageClrDec4C_RCT(cy0, cy1, cy2, cy3,
		cu0, cv0, cu1, cv1, cu2, cv2, cu3, cv3,
		rpx0, rpx1);
	rpx0[0]=btic4b_pxrgbx2rgb11f(rpx0[0]);
	rpx0[1]=btic4b_pxrgbx2rgb11f(rpx0[1]);
	rpx1[0]=btic4b_pxrgbx2rgb11f(rpx1[0]);
	rpx1[1]=btic4b_pxrgbx2rgb11f(rpx1[1]);
}
#endif

#if 1
u32 BTIC4B_DecImageClr1_RCT12F_11F(int cy, int cu, int cv)
{
	u32 px;
	int cr, cg, cb;
	
	cg=cy-((cu+cv)>>2);
	cr=cg+cv;	cb=cg+cu;
	if((cr|cg|cb)>>11)
	{
		cr=lqtvq_clamp2047(cr);
		cg=lqtvq_clamp2047(cg);
		cb=lqtvq_clamp2047(cb);
	}
	
	px=((cb>>1)<<22)|(cg<<11)|cr;
	return(px);
}

u32 BTIC4B_DecImageClr1A_RCT12F_11F(int cy, int cu, int cv, int ca)
	{ return(BTIC4B_DecImageClr1_RCT12F_11F(cy, cu, cv)); }

void BTIC4B_DecImageClrDec2T_RCT12F_11F(int tag,
	int acy, int acu, int acv,
	int dcy, int dcu, int dcv,
	int *rr0, int *rg0, int *rb0,
	int *rr1, int *rg1, int *rb1)
{
	int mcy, mcu, mcv;
	int ncy, ncu, ncv;
	int cr0, cg0, cb0;
	int cr3, cg3, cb3;

	mcy=acy-(dcy>>1); ncy=mcy+dcy;
	mcu=acu-(dcu>>1); ncu=mcu+dcu;
	mcv=acv-(dcv>>1); ncv=mcv+dcv;

	if(tag&0x40)
	{
		cg0=mcy-((mcu+mcv)>>2); cb0=cg0+mcu; cr0=cg0+mcv;
		cg3=ncy-((ncu+ncv)>>2); cb3=cg3+ncu; cr3=cg3+ncv;
	}else
	{
		cg0=mcy-((acu+acv)>>2); cb0=cg0+acu; cr0=cg0+acv;
		cg3=ncy-((acu+acv)>>2); cb3=cg3+acu; cr3=cg3+acv;
	}

	if((cr0|cg0|cb0)>>11)
	{
		cr0=lqtvq_clamp2047(cr0);
		cg0=lqtvq_clamp2047(cg0);
		cb0=lqtvq_clamp2047(cb0);
	}

	if((cr3|cg3|cb3)>>11)
	{
		cr3=lqtvq_clamp2047(cr3);
		cg3=lqtvq_clamp2047(cg3);
		cb3=lqtvq_clamp2047(cb3);
	}
	
	*rr0=cr0;	*rg0=cg0;	*rb0=cb0;
	*rr1=cr3;	*rg1=cg3;	*rb1=cb3;
}

void BTIC4B_DecImageClrDec4_RCT12F_11F(
	int cy0, int cy1, int cy2, int cy3, int cu, int cv,
	u32 *rpx0, u32 *rpx1)
{
	u32 px0, px1, px2, px3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;

//	cu=cv;

	cg0=cy0-((cu+cv)>>2); cb0=cg0+cu; cr0=cg0+cv;
	cg1=cy1-((cu+cv)>>2); cb1=cg1+cu; cr1=cg1+cv;
	cg2=cy2-((cu+cv)>>2); cb2=cg2+cu; cr2=cg2+cv;
	cg3=cy3-((cu+cv)>>2); cb3=cg3+cu; cr3=cg3+cv;
	
	if((cr0|cr1|cr2|cr3)>>11)
	{
		cr0=lqtvq_clamp2047(cr0);
		cr1=lqtvq_clamp2047(cr1);
		cr2=lqtvq_clamp2047(cr2);
		cr3=lqtvq_clamp2047(cr3);
	}

	if((cg0|cg1|cg2|cg3)>>11)
	{
		cg0=lqtvq_clamp2047(cg0);
		cg1=lqtvq_clamp2047(cg1);
		cg2=lqtvq_clamp2047(cg2);
		cg3=lqtvq_clamp2047(cg3);
	}

	if((cb0|cb1|cb2|cb3)>>11)
	{
		cb0=lqtvq_clamp2047(cb0);
		cb1=lqtvq_clamp2047(cb1);
		cb2=lqtvq_clamp2047(cb2);
		cb3=lqtvq_clamp2047(cb3);
	}
	
	px0=((cb0>>1)<<22)|(cg0<<11)|cr0;
	px1=((cb1>>1)<<22)|(cg1<<11)|cr1;
	px2=((cb2>>1)<<22)|(cg2<<11)|cr2;
	px3=((cb3>>1)<<22)|(cg3<<11)|cr3;
	rpx0[0]=px0;	rpx0[1]=px1;
	rpx1[0]=px2;	rpx1[1]=px3;
}

void BTIC4B_DecImageClrDec4B_RCT12F_11F(
	int cy0, int cy1, int cy2, int cy3,
	int cu0, int cv0, int cu1, int cv1,
	u32 *rpx0, u32 *rpx1)
{
	u32 px0, px1, px2, px3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;

	cg0=cy0-((cu0+cv0)>>2); cb0=cg0+cu0; cr0=cg0+cv0;
	cg1=cy1-((cu0+cv0)>>2); cb1=cg1+cu0; cr1=cg1+cv0;
	cg2=cy2-((cu1+cv1)>>2); cb2=cg2+cu1; cr2=cg2+cv1;
	cg3=cy3-((cu1+cv1)>>2); cb3=cg3+cu1; cr3=cg3+cv1;
	
	if((cr0|cr1|cr2|cr3)>>11)
	{
		cr0=lqtvq_clamp2047(cr0);
		cr1=lqtvq_clamp2047(cr1);
		cr2=lqtvq_clamp2047(cr2);
		cr3=lqtvq_clamp2047(cr3);
	}

	if((cg0|cg1|cg2|cg3)>>11)
	{
		cg0=lqtvq_clamp2047(cg0);
		cg1=lqtvq_clamp2047(cg1);
		cg2=lqtvq_clamp2047(cg2);
		cg3=lqtvq_clamp2047(cg3);
	}

	if((cb0|cb1|cb2|cb3)>>11)
	{
		cb0=lqtvq_clamp2047(cb0);
		cb1=lqtvq_clamp2047(cb1);
		cb2=lqtvq_clamp2047(cb2);
		cb3=lqtvq_clamp2047(cb3);
	}

	px0=((cb0>>1)<<22)|(cg0<<11)|cr0;
	px1=((cb1>>1)<<22)|(cg1<<11)|cr1;
	px2=((cb2>>1)<<22)|(cg2<<11)|cr2;
	px3=((cb3>>1)<<22)|(cg3<<11)|cr3;
	rpx0[0]=px0;	rpx0[1]=px1;
	rpx1[0]=px2;	rpx1[1]=px3;
}

void BTIC4B_DecImageClrDec4C_RCT12F_11F(
	int cy0, int cy1, int cy2, int cy3,
	int cu0, int cv0, int cu1, int cv1,
	int cu2, int cv2, int cu3, int cv3,
	u32 *rpx0, u32 *rpx1)
{
	u32 px0, px1, px2, px3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;

	cg0=cy0-((cu0+cv0)>>2); cb0=cg0+cu0; cr0=cg0+cv0;
	cg1=cy1-((cu1+cv1)>>2); cb1=cg1+cu1; cr1=cg1+cv1;
	cg2=cy2-((cu2+cv2)>>2); cb2=cg2+cu2; cr2=cg2+cv2;
	cg3=cy3-((cu3+cv3)>>2); cb3=cg3+cu3; cr3=cg3+cv3;
	
	if((cr0|cr1|cr2|cr3)>>11)
	{
		cr0=lqtvq_clamp2047(cr0);
		cr1=lqtvq_clamp2047(cr1);
		cr2=lqtvq_clamp2047(cr2);
		cr3=lqtvq_clamp2047(cr3);
	}

	if((cg0|cg1|cg2|cg3)>>11)
	{
		cg0=lqtvq_clamp2047(cg0);
		cg1=lqtvq_clamp2047(cg1);
		cg2=lqtvq_clamp2047(cg2);
		cg3=lqtvq_clamp2047(cg3);
	}

	if((cb0|cb1|cb2|cb3)>>11)
	{
		cb0=lqtvq_clamp2047(cb0);
		cb1=lqtvq_clamp2047(cb1);
		cb2=lqtvq_clamp2047(cb2);
		cb3=lqtvq_clamp2047(cb3);
	}

	px0=((cb0>>1)<<22)|(cg0<<11)|cr0;
	px1=((cb1>>1)<<22)|(cg1<<11)|cr1;
	px2=((cb2>>1)<<22)|(cg2<<11)|cr2;
	px3=((cb3>>1)<<22)|(cg3<<11)|cr3;
	rpx0[0]=px0;	rpx0[1]=px1;
	rpx1[0]=px2;	rpx1[1]=px3;
}
#endif

void BTIC4B_DecImageSetupClrsI(BTIC4B_Context *ctx, int clrs)
{
	int sw, xstr;

	BTIC4B_InitScTables();
	
	xstr=0;
	switch(clrs&127)
	{
	case BTIC4B_CLRS_RGBA:
//		ctx->DecBlock=BTIC4B_DecBlockRGBA;
		ctx->DecBlock=BTIC4B_DecBlockBGRA;
		ctx->DecBlockHalf=BTIC4B_DecBlockHalfBGRA;
		xstr=4;
		sw=1;
		break;
	case BTIC4B_CLRS_BGRA:
		ctx->DecBlock=BTIC4B_DecBlockBGRA;
		ctx->DecBlockHalf=BTIC4B_DecBlockHalfBGRA;
		xstr=4;
		sw=0;
		break;
	case BTIC4B_CLRS_RGBX:
//		ctx->DecBlock=BTIC4B_DecBlockRGBX;
		ctx->DecBlock=BTIC4B_DecBlockBGRX;
		ctx->DecBlockHalf=BTIC4B_DecBlockHalfBGRX;
		xstr=4;
		sw=1;
		break;
	case BTIC4B_CLRS_BGRX:
		ctx->DecBlock=BTIC4B_DecBlockBGRX;
		ctx->DecBlockHalf=BTIC4B_DecBlockHalfBGRX;
		xstr=4;
		sw=0;
		break;
	case BTIC4B_CLRS_RGB11F:
		ctx->DecBlock=BTIC4B_DecBlockBGRX;
		ctx->DecBlockHalf=BTIC4B_DecBlockHalfBGRX;
		xstr=4;
		sw=2;
		break;

	case BTIC4B_CLRS_RGB:
		ctx->DecBlock=BTIC4B_DecBlockBGR;
		ctx->DecBlockHalf=BTIC4B_DecBlockHalfBGRX;
		xstr=3;
		sw=1;
		break;
	case BTIC4B_CLRS_BGR:
		ctx->DecBlock=BTIC4B_DecBlockBGR;
		ctx->DecBlockHalf=BTIC4B_DecBlockHalfBGRX;
		xstr=3;
		sw=0;
		break;
	}
	
	ctx->xstr=xstr;
	
//	ctx->DecBlock=DecBlock;
	
//	ctx->clrt=BTIC4B_CLRT_RCT;
	
	switch(ctx->clrt)
	{
	case BTIC4B_CLRT_GDBDR:
		ctx->ClrDecPack1=BTIC4B_DecImageClrPack1_LDR8;
		ctx->ClrDecPack1A=BTIC4B_DecImageClrPack1A_LDR8;
		ctx->ClrDecPack4=BTIC4B_DecImageClrPack4_LDR8;
		if(sw)
		{
			ctx->ClrDec1=BTIC4B_DecImageClr1_GDbDrSW;
			ctx->ClrDec1A=BTIC4B_DecImageClr1A_GDbDrSW;
			ctx->ClrDec2T=BTIC4B_DecImageClrDec2T_GDbDrSW;
			ctx->ClrDec4=BTIC4B_DecImageClrDec4_GDbDrSW;
			ctx->ClrDec4B=BTIC4B_DecImageClrDec4B_GDbDrSW;
			ctx->ClrDec4C=BTIC4B_DecImageClrDec4C_GDbDrSW;
		}else
		{
			ctx->ClrDec1=BTIC4B_DecImageClr1_GDbDr;
			ctx->ClrDec1A=BTIC4B_DecImageClr1A_GDbDr;
			ctx->ClrDec2T=BTIC4B_DecImageClrDec2T_GDbDr;
			ctx->ClrDec4=BTIC4B_DecImageClrDec4_GDbDr;
			ctx->ClrDec4B=BTIC4B_DecImageClrDec4B_GDbDr;
			ctx->ClrDec4C=BTIC4B_DecImageClrDec4C_GDbDr;
		}
		break;
	case BTIC4B_CLRT_RCT:
		ctx->ClrDecPack1=BTIC4B_DecImageClrPack1_LDR8;
		ctx->ClrDecPack1A=BTIC4B_DecImageClrPack1A_LDR8;
		ctx->ClrDecPack4=BTIC4B_DecImageClrPack4_LDR8;
		if(sw==1)
		{
			ctx->ClrDec1=BTIC4B_DecImageClr1_RCTSW;
			ctx->ClrDec1A=BTIC4B_DecImageClr1A_RCTSW;
			ctx->ClrDec2T=BTIC4B_DecImageClrDec2T_RCTSW;
			ctx->ClrDec4=BTIC4B_DecImageClrDec4_RCTSW;
			ctx->ClrDec4B=BTIC4B_DecImageClrDec4B_RCTSW;
			ctx->ClrDec4C=BTIC4B_DecImageClrDec4C_RCTSW;
		}else if(sw==0)
		{
			ctx->ClrDec1=BTIC4B_DecImageClr1_RCT;
			ctx->ClrDec1A=BTIC4B_DecImageClr1A_RCT;
			ctx->ClrDec2T=BTIC4B_DecImageClrDec2T_RCT;
			ctx->ClrDec4=BTIC4B_DecImageClrDec4_RCT;
			ctx->ClrDec4B=BTIC4B_DecImageClrDec4B_RCT;
			ctx->ClrDec4C=BTIC4B_DecImageClrDec4C_RCT;
		}else if(sw==2)
		{
			if(ctx->imgt==BTIC4B_IMGT_HDR12)
			{
				ctx->ClrDecPack1=BTIC4B_DecImageClrPack1_12R11F;
				ctx->ClrDecPack1A=BTIC4B_DecImageClrPack1A_12R11F;
				ctx->ClrDecPack4=BTIC4B_DecImageClrPack4_12R11F;

				ctx->ClrDec1=BTIC4B_DecImageClr1_RCT12F_11F;
				ctx->ClrDec1A=BTIC4B_DecImageClr1A_RCT12F_11F;
				ctx->ClrDec2T=BTIC4B_DecImageClrDec2T_RCT12F_11F;
				ctx->ClrDec4=BTIC4B_DecImageClrDec4_RCT12F_11F;
				ctx->ClrDec4B=BTIC4B_DecImageClrDec4B_RCT12F_11F;
				ctx->ClrDec4C=BTIC4B_DecImageClrDec4C_RCT12F_11F;
			}else if((ctx->imgt==BTIC4B_IMGT_HDR16) ||
				(ctx->imgt==BTIC4B_IMGT_HDR16A))
			{
				ctx->ClrDecPack1=BTIC4B_DecImageClrPack1_16R11F;
				ctx->ClrDecPack1A=BTIC4B_DecImageClrPack1A_16R11F;
				ctx->ClrDecPack4=BTIC4B_DecImageClrPack4_16R11F;

				ctx->ClrDec1=BTIC4B_DecImageClr1_RCT16F_11F;
				ctx->ClrDec1A=BTIC4B_DecImageClr1A_RCT16F_11F;
				ctx->ClrDec2T=BTIC4B_DecImageClrDec2T_RCT16F_11F;
				ctx->ClrDec4=BTIC4B_DecImageClrDec4_RCT16F_11F;
				ctx->ClrDec4B=BTIC4B_DecImageClrDec4B_RCT16F_11F;
				ctx->ClrDec4C=BTIC4B_DecImageClrDec4C_RCT16F_11F;
			}else
			{
				ctx->ClrDec1=BTIC4B_DecImageClr1_RCT_11FB;
				ctx->ClrDec1A=BTIC4B_DecImageClr1A_RCT_11FB;
				ctx->ClrDec2T=BTIC4B_DecImageClrDec2T_RCT_11FB;
				ctx->ClrDec4=BTIC4B_DecImageClrDec4_RCT_11FB;
				ctx->ClrDec4B=BTIC4B_DecImageClrDec4B_RCT_11FB;
				ctx->ClrDec4C=BTIC4B_DecImageClrDec4C_RCT_11FB;
			}
		}
		break;
	default:
		ctx->ClrDecPack1=BTIC4B_DecImageClrPack1_LDR8;
		ctx->ClrDecPack1A=BTIC4B_DecImageClrPack1A_LDR8;
		ctx->ClrDecPack4=BTIC4B_DecImageClrPack4_LDR8;

		ctx->ClrDec1=BTIC4B_DecImageClr1_RCT;
		ctx->ClrDec1A=BTIC4B_DecImageClr1A_RCT;
		ctx->ClrDec2T=BTIC4B_DecImageClrDec2T_RCT;
		ctx->ClrDec4=BTIC4B_DecImageClrDec4_RCT;
		ctx->ClrDec4B=BTIC4B_DecImageClrDec4B_RCT;
		ctx->ClrDec4C=BTIC4B_DecImageClrDec4C_RCT;
		break;
	}
}

void BTIC4B_DecImageClrsI(BTIC4B_Context *ctx,
	byte *blks, byte *ibuf,
	int xs, int ys, int clrs)
{
	void (*DecBlock)(BTIC4B_Context *ctx,
		byte *blkbuf, byte *ibuf, int ystr);
	byte *cs, *ct, *ibuf1;
	int xs1, ys1, xs2, ys2, xf, yf, xstr, ystr;
	int bi, sw, vf;
	int i, j, k;
	
	vf=ctx->flip;
	if(ys<0)
	{
		ys=-ys;
		vf=!vf;
	}
	
	BTIC4B_DecImageSetupClrsI(ctx, clrs);
	DecBlock=ctx->DecBlock;
	xstr=ctx->xstr;
	
	ibuf1=ibuf;
//	ystr=xs*4;
	ystr=xs*xstr;
	ystr=(ystr+3)&(~3);

	xs1=xs>>3;	ys1=ys>>3;
	xs2=(xs+7)>>3;	ys2=(ys+7)>>3;
	xf=xs&7;	yf=ys&7;
	
	if(vf)
	{
		ibuf1=ibuf+(ys-1)*ystr;
		ystr=-ystr;
	}
	
	for(i=0; i<ys1; i++)
	{
		cs=ibuf1+(i*8)*ystr;
//		ct=blks+((i*xs2)<<6);
		ct=blks+(i*xs2)*ctx->blksz;
		for(j=0; j<xs1; j++)
		{
#if 0
//			if(ct[0]==0)
//			if(*((u16 *)ct)==0xFF00)
			if(*((u16 *)ct)==0x0000)
			{
				k=0;
				if(j>0)k|=1;
				if(i>0)k|=2;
				if((j+2)<xs1)k|=4;
//				if((i+2)<ys1)k|=8;
				if((j>0) && (*(u16 *)(ct-ctx->blksz)==0x0000))k|=16;
//				if((i>0) && (*(u16 *)(ct-xs2*ctx->blksz)==0x0000))k|=32;
				ct[28]=k;
			}
#endif

			DecBlock(ctx, ct, cs, ystr);
//			ct+=64; cs+=32;
			ct+=ctx->blksz;
			cs+=8*xstr;
		}
		if(xf)
		{
//			if(*((u16 *)ct)==0x0000)
//				ct[28]=0;

			BTIC4B_DecBlockEdgeClrs(ctx, ct, cs,
				ystr, xf, 8, clrs);
		}
	}
	if(yf)
	{
		cs=ibuf1+(i*8)*ystr;
//		ct=blks+((i*xs2)<<6);
		ct=blks+(i*xs2)*ctx->blksz;
		for(j=0; j<xs1; j++)
		{
//			if(*((u16 *)ct)==0x0000)
//				ct[28]=0;

			BTIC4B_DecBlockEdgeClrs(ctx, ct, cs,
				ystr, 8, yf, clrs);
//			ct+=64; cs+=32;
			ct+=ctx->blksz;
			cs+=8*xstr;
		}
		if(xf)
		{
//			if(*((u16 *)ct)==0x0000)
//				ct[28]=0;
			BTIC4B_DecBlockEdgeClrs(ctx, ct, cs,
				ystr, xf, yf, clrs);
		}
	}
}


void BTIC4B_ConvImageBC7n(BTIC4B_Context *ctx,
	byte *iblock, int iblkstr,
	byte *oblock, int xs, int ys);
void BTIC4B_ConvImageBC7nMip(BTIC4B_Context *ctx,
	byte *iblock, int iblkstr,
	byte *oblock, int xs, int ys);
void BTIC4B_ConvImageBC1n(BTIC4B_Context *ctx,
	byte *iblock, int iblkstr,
	byte *oblock, int xs, int ys);
void BTIC4B_ConvImageBC1nMip(BTIC4B_Context *ctx,
	byte *iblock, int iblkstr,
	byte *oblock, int xs, int ys);
void BTIC4B_ConvImageBC3n(BTIC4B_Context *ctx,
	byte *iblock, int iblkstr,
	byte *oblock, int xs, int ys);
void BTIC4B_ConvImageBC3nMip(BTIC4B_Context *ctx,
	byte *iblock, int iblkstr,
	byte *oblock, int xs, int ys);

void BTIC4B_DecImageClrs(BTIC4B_Context *ctx,
	byte *blks, byte *ibuf,
	int xs, int ys, int clrs)
{
	switch(clrs)
	{
	case BTIC4B_CLRS_RGBA:	case BTIC4B_CLRS_BGRA:
	case BTIC4B_CLRS_RGBX:	case BTIC4B_CLRS_BGRX:
	case BTIC4B_CLRS_RGB:	case BTIC4B_CLRS_BGR:
		BTIC4B_DecImageClrsI(ctx, blks, ibuf, xs, ys, clrs);
		break;
	case BTIC4B_CLRS_RGB11F:
		BTIC4B_DecImageClrsI(ctx, blks, ibuf, xs, ys, clrs);
		break;

	case BTIC4B_CLRS_BC7:
		BTIC4B_ConvImageBC7n(ctx, blks, ctx->blksz, ibuf, xs, ys);
		break;
	case BTIC4B_CLRS_BC7MIP:
		BTIC4B_ConvImageBC7nMip(ctx, blks, ctx->blksz, ibuf, xs, ys);
		break;

	case BTIC4B_CLRS_BC1:
		BTIC4B_ConvImageBC1n(ctx, blks, ctx->blksz, ibuf, xs, ys);
		break;
	case BTIC4B_CLRS_BC1MIP:
		BTIC4B_ConvImageBC1nMip(ctx, blks, ctx->blksz, ibuf, xs, ys);
		break;
	case BTIC4B_CLRS_BC3:
		BTIC4B_ConvImageBC3n(ctx, blks, ctx->blksz, ibuf, xs, ys);
		break;
	case BTIC4B_CLRS_BC3MIP:
		BTIC4B_ConvImageBC3nMip(ctx, blks, ctx->blksz, ibuf, xs, ys);
		break;

	case BTIC4B_CLRS_BC4:
		BTIC4B_ConvImageBC4n(ctx, blks, ctx->blksz, ibuf, xs, ys);
		break;
	case BTIC4B_CLRS_BC4MIP:
		BTIC4B_ConvImageBC4nMip(ctx, blks, ctx->blksz, ibuf, xs, ys);
		break;
	case BTIC4B_CLRS_BC5:
		BTIC4B_ConvImageBC5n(ctx, blks, ctx->blksz, ibuf, xs, ys);
		break;
	case BTIC4B_CLRS_BC5MIP:
		BTIC4B_ConvImageBC5nMip(ctx, blks, ctx->blksz, ibuf, xs, ys);
		break;
	}
}
