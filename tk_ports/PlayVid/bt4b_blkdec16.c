
/* 8x8x3+UV4x4x2 */
void BTIC4B_DecBlock0RGB16(BTIC4B_Context *ctx,
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
			ct0+=4;		ct1+=4;
		}
	}
}

force_inline void lqtvq_fillu16_8x1(u32 *px, u32 v)
{
	u32 v1;
	v1=v|(v<<16);
	px[0]=v1; px[1]=v1; px[2]=v1; px[3]=v1;
}


force_inline void lqtvq_fillu16_8x1_2x1(u32 *px, u32 u, u32 v)
{
	u32 u1, v1;
	u1=u|(u<<16);
	v1=v|(v<<16);
	px[0]=u1; px[1]=u1; px[2]=v1; px[3]=v1;
}

force_inline void lqtvq_fillu16_8x1_4x1(u32 *px,
	u32 v0, u32 v1, u32 v2, u32 v3)
{
	v0=v0|(v0<<16);	v1=v1|(v1<<16);
	v2=v2|(v2<<16);	v3=v3|(v3<<16);
	px[0]=v0; px[1]=v1; px[2]=v2; px[3]=v3;
}

force_inline void lqtvq_fillu16_8x1_8x1(u32 *pxa,
	u32 v0, u32 v1, u32 v2, u32 v3,
	u32 v4, u32 v5, u32 v6, u32 v7)
{
	u16 *px;
	px=(u16 *)pxa;
	px[0]=v0; px[1]=v1; px[2]=v2; px[3]=v3;
	px[4]=v4; px[5]=v5; px[6]=v6; px[7]=v7;
}

force_inline void lqtvq_fillu16_4x1(u32 *px, u32 v)
{
	v=v|(v<<16);
	px[0]=v; px[1]=v;
}

void BTIC4B_DecBlockFlatRGB16(BTIC4B_Context *ctx,
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
	u64 *ct;
	u64 pxu1;
	int cr, cg, cb;
	int fl;
	int i, j, k;
	
	acy=*(s16 *)(blkbuf+ 4);
	acu=*(s16 *)(blkbuf+ 6);
	acv=*(s16 *)(blkbuf+ 8);

	pxu=ctx->ClrDec1(acy, acu, acv);
	
	pxu1=pxu|(pxu<<16);
	pxu1=pxu1|(pxu1<<32);
	ct=(u64 *)ibuf;						ct[0]=pxu1;		ct[1]=pxu1;
	ct=(u64 *)(((byte *)ct)+ystr);		ct[0]=pxu1;		ct[1]=pxu1;
	ct=(u64 *)(((byte *)ct)+ystr);		ct[0]=pxu1;		ct[1]=pxu1;
	ct=(u64 *)(((byte *)ct)+ystr);		ct[0]=pxu1;		ct[1]=pxu1;
	ct=(u64 *)(((byte *)ct)+ystr);		ct[0]=pxu1;		ct[1]=pxu1;
	ct=(u64 *)(((byte *)ct)+ystr);		ct[0]=pxu1;		ct[1]=pxu1;
	ct=(u64 *)(((byte *)ct)+ystr);		ct[0]=pxu1;		ct[1]=pxu1;
	ct=(u64 *)(((byte *)ct)+ystr);		ct[0]=pxu1;		ct[1]=pxu1;

#if 0
	lqtvq_fillu16_8x1((u32 *)(ibuf+0*ystr), pxu);
	lqtvq_fillu16_8x1((u32 *)(ibuf+1*ystr), pxu);
	lqtvq_fillu16_8x1((u32 *)(ibuf+2*ystr), pxu);
	lqtvq_fillu16_8x1((u32 *)(ibuf+3*ystr), pxu);
	lqtvq_fillu16_8x1((u32 *)(ibuf+4*ystr), pxu);
	lqtvq_fillu16_8x1((u32 *)(ibuf+5*ystr), pxu);
	lqtvq_fillu16_8x1((u32 *)(ibuf+6*ystr), pxu);
	lqtvq_fillu16_8x1((u32 *)(ibuf+7*ystr), pxu);
#endif
}



/* XxXx2 */
void BTIC4B_DecBlock1RGB16(BTIC4B_Context *ctx,
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
			lqtvq_fillu16_8x1_2x1((u32 *)ct0, pxu, pxv);
			lqtvq_fillu16_8x1_2x1((u32 *)ct1, pxu, pxv);
			lqtvq_fillu16_8x1_2x1((u32 *)ct2, pxu, pxv);
			lqtvq_fillu16_8x1_2x1((u32 *)ct3, pxu, pxv);
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
			lqtvq_fillu16_8x1_4x1((u32 *)ct0, px0, px1, px2, px3);
			lqtvq_fillu16_8x1_4x1((u32 *)ct1, px0, px1, px2, px3);
			lqtvq_fillu16_8x1_4x1((u32 *)ct2, px0, px1, px2, px3);
			lqtvq_fillu16_8x1_4x1((u32 *)ct3, px0, px1, px2, px3);
		}
		break;
	case 0x0B:
		pxy0=*(u16 *)(blkbuf+16);
		for(i=0; i<4; i++)
		{
			pxu=rgbtab[(pxy0>>(i*4+0))&3];
			pxv=rgbtab[(pxy0>>(i*4+2))&3];

			ct0=ibuf+(i*2+0)*ystr;	ct1=ibuf+(i*2+1)*ystr;
			lqtvq_fillu16_8x1_2x1((u32 *)ct0, pxu, pxv);
			lqtvq_fillu16_8x1_2x1((u32 *)ct1, pxu, pxv);
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
			lqtvq_fillu16_8x1_4x1((u32 *)ct0, px0, px1, px2, px3);
			lqtvq_fillu16_8x1_4x1((u32 *)ct1, px0, px1, px2, px3);
		}
		break;
	case 0x0D:
		for(i=0; i<2; i++)
		{
			pxy0=btic4b_getu32le(blkbuf+16+i*4);

#if 0
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

			lqtvq_fillu16_8x1_8x1((u32 *)ct0,
				px0, px1, px2, px3, px4, px5, px6, px7);
			lqtvq_fillu16_8x1_8x1((u32 *)ct1,
				px0, px1, px2, px3, px4, px5, px6, px7);
			lqtvq_fillu16_8x1_8x1((u32 *)ct2,
				px8, px9, pxA, pxB, pxC, pxD, pxE, pxF);
			lqtvq_fillu16_8x1_8x1((u32 *)ct3,
				px8, px9, pxA, pxB, pxC, pxD, pxE, pxF);
#endif

#if 1
			ct0=ibuf+(i*4+0)*ystr;	ct1=ibuf+(i*4+1)*ystr;
			ct2=ibuf+(i*4+2)*ystr;	ct3=ibuf+(i*4+3)*ystr;

			px1=rgbtab[(pxy0>> 2)&3];	px0=rgbtab[(pxy0>> 0)&3];
			px3=rgbtab[(pxy0>> 6)&3];	px2=rgbtab[(pxy0>> 4)&3];
			((u16 *)ct0)[0]=px0;		((u16 *)ct0)[1]=px1;
			((u16 *)ct1)[0]=px0;		((u16 *)ct1)[1]=px1;
			((u16 *)ct0)[2]=px2;		((u16 *)ct0)[3]=px3;
			((u16 *)ct1)[2]=px2;		((u16 *)ct1)[3]=px3;
			px5=rgbtab[(pxy0>>10)&3];	px4=rgbtab[(pxy0>> 8)&3];
			px7=rgbtab[(pxy0>>14)&3];	px6=rgbtab[(pxy0>>12)&3];
			((u16 *)ct0)[4]=px4;		((u16 *)ct0)[5]=px5;
			((u16 *)ct1)[4]=px4;		((u16 *)ct1)[5]=px5;
			((u16 *)ct0)[6]=px6;		((u16 *)ct0)[7]=px7;
			((u16 *)ct1)[6]=px6;		((u16 *)ct1)[7]=px7;
			px9=rgbtab[(pxy0>>18)&3];	px8=rgbtab[(pxy0>>16)&3];
			pxB=rgbtab[(pxy0>>22)&3];	pxA=rgbtab[(pxy0>>20)&3];
			((u16 *)ct2)[0]=px8;		((u16 *)ct2)[1]=px9;
			((u16 *)ct3)[0]=px8;		((u16 *)ct3)[1]=px9;
			((u16 *)ct2)[2]=pxA;		((u16 *)ct2)[3]=pxB;
			((u16 *)ct3)[2]=pxA;		((u16 *)ct3)[3]=pxB;
			pxD=rgbtab[(pxy0>>26)&3];	pxC=rgbtab[(pxy0>>24)&3];
			pxF=rgbtab[(pxy0>>30)&3];	pxE=rgbtab[(pxy0>>28)&3];
			((u16 *)ct2)[4]=pxC;		((u16 *)ct2)[5]=pxD;
			((u16 *)ct3)[4]=pxC;		((u16 *)ct3)[5]=pxD;
			((u16 *)ct2)[6]=pxE;		((u16 *)ct2)[7]=pxF;
			((u16 *)ct3)[6]=pxE;		((u16 *)ct3)[7]=pxF;
#endif
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

			lqtvq_fillu16_8x1_4x1((u32 *)ct0, px0, px1, px2, px3);
			lqtvq_fillu16_8x1_4x1((u32 *)ct1, px4, px5, px6, px7);
			lqtvq_fillu16_8x1_4x1((u32 *)ct2, px8, px9, pxA, pxB);
			lqtvq_fillu16_8x1_4x1((u32 *)ct3, pxC, pxD, pxE, pxF);
		}
		break;
	case 0x0F:
		for(i=0; i<4; i++)
		{
			pxy0=btic4b_getu32le(blkbuf+16+i*4);

#if 0
			pxF=rgbtab[(pxy0>>30)&3];	pxE=rgbtab[(pxy0>>28)&3];
			pxD=rgbtab[(pxy0>>26)&3];	pxC=rgbtab[(pxy0>>24)&3];
			pxB=rgbtab[(pxy0>>22)&3];	pxA=rgbtab[(pxy0>>20)&3];
			px9=rgbtab[(pxy0>>18)&3];	px8=rgbtab[(pxy0>>16)&3];
			px7=rgbtab[(pxy0>>14)&3];	px6=rgbtab[(pxy0>>12)&3];
			px5=rgbtab[(pxy0>>10)&3];	px4=rgbtab[(pxy0>> 8)&3];
			px3=rgbtab[(pxy0>> 6)&3];	px2=rgbtab[(pxy0>> 4)&3];
			px1=rgbtab[(pxy0>> 2)&3];	px0=rgbtab[(pxy0>> 0)&3];
			ct0=ibuf+(i*2+0)*ystr;	ct1=ibuf+(i*2+1)*ystr;

			lqtvq_fillu16_8x1_8x1((u32 *)ct0,
				px0, px1, px2, px3, px4, px5, px6, px7);
			lqtvq_fillu16_8x1_8x1((u32 *)ct1,
				px8, px9, pxA, pxB, pxC, pxD, pxE, pxF);
#endif

#if 1
			ct0=ibuf+(i*2+0)*ystr;	ct1=ibuf+(i*2+1)*ystr;

			px1=rgbtab[(pxy0>> 2)&3];	px0=rgbtab[(pxy0>> 0)&3];
			px3=rgbtab[(pxy0>> 6)&3];	px2=rgbtab[(pxy0>> 4)&3];
			((u16 *)ct0)[0]=px0;		((u16 *)ct0)[1]=px1;
			((u16 *)ct0)[2]=px2;		((u16 *)ct0)[3]=px3;
			px5=rgbtab[(pxy0>>10)&3];	px4=rgbtab[(pxy0>> 8)&3];
			px7=rgbtab[(pxy0>>14)&3];	px6=rgbtab[(pxy0>>12)&3];
			((u16 *)ct0)[4]=px4;		((u16 *)ct0)[5]=px5;
			((u16 *)ct0)[6]=px6;		((u16 *)ct0)[7]=px7;
			px9=rgbtab[(pxy0>>18)&3];	px8=rgbtab[(pxy0>>16)&3];
			pxB=rgbtab[(pxy0>>22)&3];	pxA=rgbtab[(pxy0>>20)&3];
			((u16 *)ct1)[0]=px8;		((u16 *)ct1)[1]=px9;
			((u16 *)ct1)[2]=pxA;		((u16 *)ct1)[3]=pxB;
			pxD=rgbtab[(pxy0>>26)&3];	pxC=rgbtab[(pxy0>>24)&3];
			pxF=rgbtab[(pxy0>>30)&3];	pxE=rgbtab[(pxy0>>28)&3];
			((u16 *)ct1)[4]=pxC;		((u16 *)ct1)[5]=pxD;
			((u16 *)ct1)[6]=pxE;		((u16 *)ct1)[7]=pxF;
#endif

		}
		break;
	}
}

/* 8x8x3 */
void BTIC4B_DecBlock2RGB16(BTIC4B_Context *ctx,
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

	cr3=( 9*cr0+ 7*cr7)>>4;	cg3=( 9*cg0+ 7*cg7)>>4;	cb3=( 9*cb0+ 7*cb7)>>4;
	cr4=( 7*cr0+ 9*cr7)>>4;	cg4=( 7*cg0+ 9*cg7)>>4;	cb4=( 7*cb0+ 9*cb7)>>4;

	ctx->ClrDecPack4(cr0, cg0, cb0,
		cr3, cg3, cb3, 255, rgbtab+0);
	ctx->ClrDecPack4(cr4, cg4, cb4,
		cr7, cg7, cb7, 255, rgbtab+4);

	for(i=0; i<4; i++)
	{
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

#if 0
		lqtvq_fillu16_8x1_8x1((u32 *)ct0,
			px0, px1, px2, px3, px4, px5, px6, px7);
		lqtvq_fillu16_8x1_8x1((u32 *)ct1,
			px8, px9, pxA, pxB, pxC, pxD, pxE, pxF);
#endif

#if 0
		((u16 *)ct0)[0]=px0;		((u16 *)ct0)[1]=px1;
		((u16 *)ct0)[2]=px2;		((u16 *)ct0)[3]=px3;
		((u16 *)ct0)[4]=px4;		((u16 *)ct0)[5]=px5;
		((u16 *)ct0)[6]=px6;		((u16 *)ct0)[7]=px7;
		((u16 *)ct1)[0]=px8;		((u16 *)ct1)[1]=px9;
		((u16 *)ct1)[2]=pxA;		((u16 *)ct1)[3]=pxB;
		((u16 *)ct1)[4]=pxC;		((u16 *)ct1)[5]=pxD;
		((u16 *)ct1)[6]=pxE;		((u16 *)ct1)[7]=pxF;
#endif

#if 1
		px0=px0|(px1<<16);		px2=px2|(px3<<16);
		px4=px4|(px5<<16);		px6=px6|(px7<<16);
		((u32 *)ct0)[0]=px0;	((u32 *)ct0)[1]=px2;
		((u32 *)ct0)[2]=px4;	((u32 *)ct0)[3]=px6;
		px8=px8|(px9<<16);		pxA=pxA|(pxB<<16);
		pxC=pxC|(pxD<<16);		pxE=pxE|(pxF<<16);
		((u32 *)ct1)[0]=px8;	((u32 *)ct1)[1]=pxA;
		((u32 *)ct1)[2]=pxC;	((u32 *)ct1)[3]=pxE;
#endif
	}
}

/* 8x8x4 */
void BTIC4B_DecBlock3RGB16(BTIC4B_Context *ctx,
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

	cr3=(26*cr0+ 6*crF)>>5;	cg3=(26*cg0+ 6*cgF)>>5;	cb3=(26*cb0+ 6*cbF)>>5;
	cr4=(23*cr0+ 9*crF)>>5;	cg4=(23*cg0+ 9*cgF)>>5;	cb4=(23*cb0+ 9*cbF)>>5;
	cr7=(17*cr0+15*crF)>>5;	cg7=(17*cg0+15*cgF)>>5;	cb7=(17*cb0+15*cbF)>>5;
	cr8=(15*cr0+17*crF)>>5;	cg8=(15*cg0+17*cgF)>>5;	cb8=(15*cb0+17*cbF)>>5;
	crB=( 9*cr0+23*crF)>>5;	cgB=( 9*cg0+23*cgF)>>5;	cbB=( 9*cb0+23*cbF)>>5;
	crC=( 6*cr0+26*crF)>>5;	cgC=( 6*cg0+26*cgF)>>5;	cbC=( 6*cb0+26*cbF)>>5;

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

#if 0
		lqtvq_fillu16_8x1_8x1((u32 *)ct0,
			px0, px1, px2, px3, px4, px5, px6, px7);
		lqtvq_fillu16_8x1_8x1((u32 *)ct1,
			px8, px9, pxA, pxB, pxC, pxD, pxE, pxF);
#endif

#if 1
		((u16 *)ct0)[0]=px0;		((u16 *)ct0)[1]=px1;
		((u16 *)ct0)[2]=px2;		((u16 *)ct0)[3]=px3;
		((u16 *)ct0)[4]=px4;		((u16 *)ct0)[5]=px5;
		((u16 *)ct0)[6]=px6;		((u16 *)ct0)[7]=px7;
		((u16 *)ct1)[0]=px8;		((u16 *)ct1)[1]=px9;
		((u16 *)ct1)[2]=pxA;		((u16 *)ct1)[3]=pxB;
		((u16 *)ct1)[4]=pxC;		((u16 *)ct1)[5]=pxD;
		((u16 *)ct1)[6]=pxE;		((u16 *)ct1)[7]=pxF;
#endif
	}
}

/* Y8x8x2+UV */
void BTIC4B_DecBlock4RGB16(BTIC4B_Context *ctx,
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
				ct0+=4;		ct1+=4;
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
				ct0+=4;		ct1+=4;
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
				ct0+=4;		ct1+=4;
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
				((u16 *)ct0)[0]=pxy1;	((u16 *)ct0)[1]=pxy1;
				((u16 *)ct0)[2]=pxy1;	((u16 *)ct0)[3]=pxy1;
				((u16 *)ct1)[0]=pxy1;	((u16 *)ct1)[1]=pxy1;
				((u16 *)ct1)[2]=pxy1;	((u16 *)ct1)[3]=pxy1;
				((u16 *)ct2)[0]=pxy1;	((u16 *)ct2)[1]=pxy1;
				((u16 *)ct2)[2]=pxy1;	((u16 *)ct2)[3]=pxy1;
				((u16 *)ct3)[0]=pxy1;	((u16 *)ct3)[1]=pxy1;
				((u16 *)ct3)[2]=pxy1;	((u16 *)ct3)[3]=pxy1;
				ct0+=8;	ct1+=8;	ct2+=8;	ct3+=8;
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
				((u16 *)ct0)[0]=pxy1;	((u16 *)ct0)[1]=pxy1;
				((u16 *)ct1)[0]=pxy1;	((u16 *)ct1)[1]=pxy1;
				ct0+=4;		ct1+=4;
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
				((u16 *)ct0)[0]=pxy1;	((u16 *)ct0)[1]=pxy1;
				((u16 *)ct1)[0]=pxy1;	((u16 *)ct1)[1]=pxy1;
				ct0+=4;		ct1+=4;
			}
		}
		return;
	}

	if(bt==0x17)
	{
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
				ct0+=4;		ct1+=4;
			}
		}
		return;
	}
}


/* 8x8x2+UV4x4x2 LDR8 */
void BTIC4B_DecBlock5RGB16(BTIC4B_Context *ctx,
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
				ct0+=4;	ct1+=4;
			}
		}
		return;
	}
}

void BTIC4B_DecBlock7RGB16(BTIC4B_Context *ctx,
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
			ct0+=4;	ct1+=4;
		}
	}
}

/* XxXx1 */
void BTIC4B_DecBlock6RGB16(BTIC4B_Context *ctx,
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
			lqtvq_fillu16_8x1_2x1((u32 *)ct0, pxu, pxv);
			lqtvq_fillu16_8x1_2x1((u32 *)ct1, pxu, pxv);
			lqtvq_fillu16_8x1_2x1((u32 *)ct2, pxu, pxv);
			lqtvq_fillu16_8x1_2x1((u32 *)ct3, pxu, pxv);
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
			lqtvq_fillu16_8x1_4x1((u32 *)ct0, px0, px1, px2, px3);
			lqtvq_fillu16_8x1_4x1((u32 *)ct1, px0, px1, px2, px3);
			lqtvq_fillu16_8x1_4x1((u32 *)ct2, px0, px1, px2, px3);
			lqtvq_fillu16_8x1_4x1((u32 *)ct3, px0, px1, px2, px3);
		}
		break;
	case 0x03:
		pxy0=blkbuf[16];
		for(i=0; i<4; i++)
		{
			pxu=rgbtab[(pxy0>>(i*2+0))&1];
			pxv=rgbtab[(pxy0>>(i*2+1))&1];
			ct0=ibuf+(i*2+0)*ystr;	ct1=ibuf+(i*2+1)*ystr;
			lqtvq_fillu16_8x1_2x1((u32 *)ct0, pxu, pxv);
			lqtvq_fillu16_8x1_2x1((u32 *)ct1, pxu, pxv);
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
			lqtvq_fillu16_8x1_4x1((u32 *)ct0, px0, px1, px2, px3);
			lqtvq_fillu16_8x1_4x1((u32 *)ct1, px0, px1, px2, px3);
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

#if 0
			lqtvq_fillu16_8x1_8x1((u32 *)ct0,
				px0, px1, px2, px3, px4, px5, px6, px7);
			lqtvq_fillu16_8x1_8x1((u32 *)ct1,
				px0, px1, px2, px3, px4, px5, px6, px7);
			lqtvq_fillu16_8x1_8x1((u32 *)ct2,
				px8, px9, pxA, pxB, pxC, pxD, pxE, pxF);
			lqtvq_fillu16_8x1_8x1((u32 *)ct3,
				px8, px9, pxA, pxB, pxC, pxD, pxE, pxF);
#endif

#if 1
			((u16 *)ct0)[0]=px0;		((u16 *)ct0)[1]=px1;
			((u16 *)ct0)[2]=px2;		((u16 *)ct0)[3]=px3;
			((u16 *)ct0)[4]=px4;		((u16 *)ct0)[5]=px5;
			((u16 *)ct0)[6]=px6;		((u16 *)ct0)[7]=px7;

			((u16 *)ct1)[0]=px0;		((u16 *)ct1)[1]=px1;
			((u16 *)ct1)[2]=px2;		((u16 *)ct1)[3]=px3;
			((u16 *)ct1)[4]=px4;		((u16 *)ct1)[5]=px5;
			((u16 *)ct1)[6]=px6;		((u16 *)ct1)[7]=px7;

			((u16 *)ct2)[0]=px8;		((u16 *)ct2)[1]=px9;
			((u16 *)ct2)[2]=pxA;		((u16 *)ct2)[3]=pxB;
			((u16 *)ct2)[4]=pxC;		((u16 *)ct2)[5]=pxD;
			((u16 *)ct2)[6]=pxE;		((u16 *)ct2)[7]=pxF;

			((u16 *)ct3)[0]=px8;		((u16 *)ct3)[1]=px9;
			((u16 *)ct3)[2]=pxA;		((u16 *)ct3)[3]=pxB;
			((u16 *)ct3)[4]=pxC;		((u16 *)ct3)[5]=pxD;
			((u16 *)ct3)[6]=pxE;		((u16 *)ct3)[7]=pxF;
#endif
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

			lqtvq_fillu16_8x1_4x1((u32 *)ct0, px0, px1, px2, px3);
			lqtvq_fillu16_8x1_4x1((u32 *)ct1, px4, px5, px6, px7);
			lqtvq_fillu16_8x1_4x1((u32 *)ct2, px8, px9, pxA, pxB);
			lqtvq_fillu16_8x1_4x1((u32 *)ct3, pxC, pxD, pxE, pxF);
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

#if 0
			lqtvq_fillu16_8x1_8x1((u32 *)ct0,
				px0, px1, px2, px3, px4, px5, px6, px7);
			lqtvq_fillu16_8x1_8x1((u32 *)ct1,
				px8, px9, pxA, pxB, pxC, pxD, pxE, pxF);
#endif

#if 1
			((u16 *)ct0)[0]=px0;		((u16 *)ct0)[1]=px1;
			((u16 *)ct0)[2]=px2;		((u16 *)ct0)[3]=px3;
			((u16 *)ct0)[4]=px4;		((u16 *)ct0)[5]=px5;
			((u16 *)ct0)[6]=px6;		((u16 *)ct0)[7]=px7;
			((u16 *)ct1)[0]=px8;		((u16 *)ct1)[1]=px9;
			((u16 *)ct1)[2]=pxA;		((u16 *)ct1)[3]=pxB;
			((u16 *)ct1)[4]=pxC;		((u16 *)ct1)[5]=pxD;
			((u16 *)ct1)[6]=pxE;		((u16 *)ct1)[7]=pxF;
#endif
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

void BTIC4B_DecBlockGrad4RGB16(BTIC4B_Context *ctx,
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
//	if(1)
	{
		tblk[0]=0x0C;
		*(u32 *)(tblk+16)=grtab[blkbuf[16]&15];
		BTIC4B_DecBlock1RGB16(ctx, tblk, ibuf, ystr);
		return;
	}

#if 1
	if(1)
	{
		j=i&65535;
		tblk[0]=0x0F;
//		*(u32 *)(tblk+16)=grtab[blkbuf[16]&15];

		*(u32 *)(tblk+16)=btic4b_grad16_ipattab2[j*4+0];
		*(u32 *)(tblk+20)=btic4b_grad16_ipattab2[j*4+1];
		*(u32 *)(tblk+24)=btic4b_grad16_ipattab2[j*4+2];
		*(u32 *)(tblk+28)=btic4b_grad16_ipattab2[j*4+3];
		BTIC4B_DecBlock1RGB16(ctx, tblk, ibuf, ystr);
		return;
	}
#endif
}


/* 8x8x4+UV4x4x4 */
void BTIC4B_DecBlock8RGB16(BTIC4B_Context *ctx,
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
			ct0+=4;		ct1+=4;
		}
	}
}

/* 8x8x3+UV4x8x3 */
void BTIC4B_DecBlock9RGB16(BTIC4B_Context *ctx,
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
			ct0+=4;		ct1+=4;
		}
	}
}

/* 8x8x4+UV8x8x3 */
void BTIC4B_DecBlock10RGB16(BTIC4B_Context *ctx,
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
			ct0+=4;		ct1+=4;
		}
	}
}



#if 1
void BTIC4B_DecImageClrDec4_GDbDr16(
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
	
	cr0>>=3;	cg0>>=3;	cb0>>=3;
	cr1>>=3;	cg1>>=3;	cb1>>=3;
	cr2>>=3;	cg2>>=3;	cb2>>=3;
	cr3>>=3;	cg3>>=3;	cb3>>=3;
	
	px0=(cr0<<10)|(cg0<<5)|cb0;
	px1=(cr1<<10)|(cg1<<5)|cb1;
	px2=(cr2<<10)|(cg2<<5)|cb2;
	px3=(cr3<<10)|(cg3<<5)|cb3;
	rpx0[0]=px0|(px1<<16);
	rpx1[0]=px2|(px3<<16);
}

void BTIC4B_DecImageClrDec4B_GDbDr16(
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
	
	cr0>>=3;	cg0>>=3;	cb0>>=3;
	cr1>>=3;	cg1>>=3;	cb1>>=3;
	cr2>>=3;	cg2>>=3;	cb2>>=3;
	cr3>>=3;	cg3>>=3;	cb3>>=3;
	
	px0=(cr0<<10)|(cg0<<5)|cb0;
	px1=(cr1<<10)|(cg1<<5)|cb1;
	px2=(cr2<<10)|(cg2<<5)|cb2;
	px3=(cr3<<10)|(cg3<<5)|cb3;
	rpx0[0]=px0|(px1<<16);
	rpx1[0]=px2|(px3<<16);
}

void BTIC4B_DecImageClrDec4C_GDbDr16(
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
	
	cr0>>=3;	cg0>>=3;	cb0>>=3;
	cr1>>=3;	cg1>>=3;	cb1>>=3;
	cr2>>=3;	cg2>>=3;	cb2>>=3;
	cr3>>=3;	cg3>>=3;	cb3>>=3;
	
	px0=(cr0<<10)|(cg0<<5)|cb0;
	px1=(cr1<<10)|(cg1<<5)|cb1;
	px2=(cr2<<10)|(cg2<<5)|cb2;
	px3=(cr3<<10)|(cg3<<5)|cb3;
	rpx0[0]=px0|(px1<<16);
	rpx1[0]=px2|(px3<<16);
}
#endif

#if 1
u32 BTIC4B_DecImageClr1_RCT16(int cy, int cu, int cv)
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
	cr=cr>>3;	cg=cg>>3;	cb=cb>>3;
	px=(cr<<10)|(cg<<5)|cb;
	return(px);
}

u32 BTIC4B_DecImageClr1A_RCT16(int cy, int cu, int cv, int ca)
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
	cr=cr>>3;	cg=cg>>3;	cb=cb>>3;
	px=(cr<<10)|(cg<<5)|cb;
	return(px);
}

void BTIC4B_DecImageClrDec4_RCT16(
	int cy0, int cy1, int cy2, int cy3, int cu, int cv,
	u32 *rpx0, u32 *rpx1)
{
	u32 px0, px1, px2, px3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;

//	cg0=cy0-((cu+cv)>>2); cb0=cg0+cu; cr0=cg0+cv;
//	cg1=cy1-((cu+cv)>>2); cb1=cg1+cu; cr1=cg1+cv;
//	cg2=cy2-((cu+cv)>>2); cb2=cg2+cu; cr2=cg2+cv;
//	cg3=cy3-((cu+cv)>>2); cb3=cg3+cu; cr3=cg3+cv;

	cu>>=3;		cv>>=3;
	cg0=(cy0>>3)-((cu+cv)>>2); cb0=cg0+cu; cr0=cg0+cv;
	cg1=(cy1>>3)-((cu+cv)>>2); cb1=cg1+cu; cr1=cg1+cv;
	cg2=(cy2>>3)-((cu+cv)>>2); cb2=cg2+cu; cr2=cg2+cv;
	cg3=(cy3>>3)-((cu+cv)>>2); cb3=cg3+cu; cr3=cg3+cv;
	
	if((cr0|cr1|cr2|cr3)>>5)
	{
		cr0=lqtvq_clamp31(cr0);
		cr1=lqtvq_clamp31(cr1);
		cr2=lqtvq_clamp31(cr2);
		cr3=lqtvq_clamp31(cr3);
	}

	if((cg0|cg1|cg2|cg3)>>5)
	{
		cg0=lqtvq_clamp31(cg0);
		cg1=lqtvq_clamp31(cg1);
		cg2=lqtvq_clamp31(cg2);
		cg3=lqtvq_clamp31(cg3);
	}

	if((cb0|cb1|cb2|cb3)>>5)
	{
		cb0=lqtvq_clamp31(cb0);
		cb1=lqtvq_clamp31(cb1);
		cb2=lqtvq_clamp31(cb2);
		cb3=lqtvq_clamp31(cb3);
	}

//	cr0=cr0>>3;	cg0=cg0>>3;	cb0=cb0>>3;
//	cr1=cr1>>3;	cg1=cg1>>3;	cb1=cb1>>3;
//	cr2=cr2>>3;	cg2=cg2>>3;	cb2=cb2>>3;
//	cr3=cr3>>3;	cg3=cg3>>3;	cb3=cb3>>3;

	px0=(cr0<<10)|(cg0<<5)|cb0;
	px1=(cr1<<10)|(cg1<<5)|cb1;
	px2=(cr2<<10)|(cg2<<5)|cb2;
	px3=(cr3<<10)|(cg3<<5)|cb3;
	rpx0[0]=px0|(px1<<16);
	rpx1[0]=px2|(px3<<16);
}

void BTIC4B_DecImageClrDec4B_RCT16(
	int cy0, int cy1, int cy2, int cy3,
	int cu0, int cv0, int cu1, int cv1,
	u32 *rpx0, u32 *rpx1)
{
	u32 px0, px1, px2, px3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;

	cu0>>=3;		cv0>>=3;
	cu1>>=3;		cv1>>=3;
	cg0=(cy0>>3)-((cu0+cv0)>>2); cb0=cg0+cu0; cr0=cg0+cv0;
	cg1=(cy1>>3)-((cu0+cv0)>>2); cb1=cg1+cu0; cr1=cg1+cv0;
	cg2=(cy2>>3)-((cu1+cv1)>>2); cb2=cg2+cu1; cr2=cg2+cv1;
	cg3=(cy3>>3)-((cu1+cv1)>>2); cb3=cg3+cu1; cr3=cg3+cv1;
	
	if((cr0|cr1|cr2|cr3)>>5)
	{
		cr0=lqtvq_clamp31(cr0);
		cr1=lqtvq_clamp31(cr1);
		cr2=lqtvq_clamp31(cr2);
		cr3=lqtvq_clamp31(cr3);
	}

	if((cg0|cg1|cg2|cg3)>>5)
	{
		cg0=lqtvq_clamp31(cg0);
		cg1=lqtvq_clamp31(cg1);
		cg2=lqtvq_clamp31(cg2);
		cg3=lqtvq_clamp31(cg3);
	}

	if((cb0|cb1|cb2|cb3)>>5)
	{
		cb0=lqtvq_clamp31(cb0);
		cb1=lqtvq_clamp31(cb1);
		cb2=lqtvq_clamp31(cb2);
		cb3=lqtvq_clamp31(cb3);
	}

	px0=(cr0<<10)|(cg0<<5)|cb0;
	px1=(cr1<<10)|(cg1<<5)|cb1;
	px2=(cr2<<10)|(cg2<<5)|cb2;
	px3=(cr3<<10)|(cg3<<5)|cb3;
	rpx0[0]=px0|(px1<<16);
	rpx1[0]=px2|(px3<<16);
}

void BTIC4B_DecImageClrDec4C_RCT16(
	int cy0, int cy1, int cy2, int cy3,
	int cu0, int cv0, int cu1, int cv1,
	int cu2, int cv2, int cu3, int cv3,
	u32 *rpx0, u32 *rpx1)
{
	u32 px0, px1, px2, px3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;

	cu0>>=3;		cv0>>=3;
	cu1>>=3;		cv1>>=3;
	cu2>>=3;		cv2>>=3;
	cu3>>=3;		cv3>>=3;
	cg0=(cy0>>3)-((cu0+cv0)>>2); cb0=cg0+cu0; cr0=cg0+cv0;
	cg1=(cy1>>3)-((cu1+cv1)>>2); cb1=cg1+cu1; cr1=cg1+cv1;
	cg2=(cy2>>3)-((cu2+cv2)>>2); cb2=cg2+cu2; cr2=cg2+cv2;
	cg3=(cy3>>3)-((cu3+cv3)>>2); cb3=cg3+cu3; cr3=cg3+cv3;
	
	if((cr0|cr1|cr2|cr3)>>5)
	{
		cr0=lqtvq_clamp31(cr0);
		cr1=lqtvq_clamp31(cr1);
		cr2=lqtvq_clamp31(cr2);
		cr3=lqtvq_clamp31(cr3);
	}

	if((cg0|cg1|cg2|cg3)>>5)
	{
		cg0=lqtvq_clamp31(cg0);
		cg1=lqtvq_clamp31(cg1);
		cg2=lqtvq_clamp31(cg2);
		cg3=lqtvq_clamp31(cg3);
	}

	if((cb0|cb1|cb2|cb3)>>5)
	{
		cb0=lqtvq_clamp31(cb0);
		cb1=lqtvq_clamp31(cb1);
		cb2=lqtvq_clamp31(cb2);
		cb3=lqtvq_clamp31(cb3);
	}

	px0=(cr0<<10)|(cg0<<5)|cb0;
	px1=(cr1<<10)|(cg1<<5)|cb1;
	px2=(cr2<<10)|(cg2<<5)|cb2;
	px3=(cr3<<10)|(cg3<<5)|cb3;
	rpx0[0]=px0|(px1<<16);
	rpx1[0]=px2|(px3<<16);
}
#endif


u32 BTIC4B_DecImageClrPack1_RGB16(int cr, int cg, int cb)
{
	return(((cr<<7)&0x7C00)|((cg<<2)&0x03E0)|(cb>>3));
}

u32 BTIC4B_DecImageClrPack1A_RGB16(int cr, int cg, int cb, int ca)
{
	return(((cr<<7)&0x7C00)|((cg<<2)&0x03E0)|(cb>>3));
}

void BTIC4B_DecImageClrPack4_RGB16(
	int cr0a, int cg0a, int cb0a,
	int cr3a, int cg3a, int cb3a,
	int ca, u32 *tab)
{
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int cr3, cg3, cb3;

	cr0=cr0a>>3;
	cg0=cg0a>>3;
	cb0=cb0a>>3;
	cr1=(11*cr0a+ 5*cr3a)>>7;
	cg1=(11*cg0a+ 5*cg3a)>>7;
	cb1=(11*cb0a+ 5*cb3a)>>7;
	cr2=( 5*cr0a+11*cr3a)>>7;
	cg2=( 5*cg0a+11*cg3a)>>7;
	cb2=( 5*cb0a+11*cb3a)>>7;
	cr3=cr3a>>3;
	cg3=cg3a>>3;
	cb3=cb3a>>3;

	tab[0]=(cr0<<10)|(cg0<<5)|cb0;
	tab[1]=(cr1<<10)|(cg1<<5)|cb1;
	tab[2]=(cr2<<10)|(cg2<<5)|cb2;
	tab[3]=(cr3<<10)|(cg3<<5)|cb3;
}

void BTIC4B_DecBlockRGB16(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	if(blkbuf[0]&0x80)
		return;
	blkbuf[0]|=0x80;

	switch(blkbuf[0]&0x1F)
	{
	case 0x00:
		BTIC4B_DecBlockFlatRGB16(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x01: case 0x02:
	case 0x03: case 0x04:
	case 0x05: case 0x06:
	case 0x07:
		BTIC4B_DecBlock6RGB16(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x08:
		BTIC4B_DecBlockGrad4RGB16(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x09: case 0x0A:
	case 0x0B: case 0x0C:
	case 0x0D: case 0x0E:
	case 0x0F:
		BTIC4B_DecBlock1RGB16(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x10:
		BTIC4B_DecBlock2RGB16(ctx, blkbuf, ibuf, ystr);
		break;

#if 1
	case 0x11:
		BTIC4B_DecBlock3RGB16(ctx, blkbuf, ibuf, ystr);
		break;

	case 0x13:
		BTIC4B_DecBlock5RGB16(ctx, blkbuf, ibuf, ystr);
		break;
#endif

#if 1
	case 0x14:
	case 0x15:	case 0x16:
	case 0x17:	case 0x18:
	case 0x1A:	case 0x1B:
		BTIC4B_DecBlock4RGB16(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x1C:
		BTIC4B_DecBlock0RGB16(ctx, blkbuf, ibuf, ystr);
		break;
#endif

#if 1
	case 0x1D:
		BTIC4B_DecBlock8RGB16(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x1E:
		BTIC4B_DecBlock9RGB16(ctx, blkbuf, ibuf, ystr);
		break;
	case 0x1F:
		BTIC4B_DecBlock10RGB16(ctx, blkbuf, ibuf, ystr);
		break;
#endif
	default:
		break;
	}
}

void BTIC4B_DecBlockHalfRGB16(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	u16 tbuf[64];
	u16 *ct0, *ct1, *ct2, *ct3;

	BTIC4B_DecBlockRGB16(ctx, blkbuf, (byte *)tbuf, 8*4);
	ct0=(u16 *)(ibuf+0*ystr);	ct1=(u16 *)(ibuf+1*ystr);
	ct2=(u16 *)(ibuf+2*ystr);	ct3=(u16 *)(ibuf+3*ystr);
	ct0[0]=tbuf[ 0]; ct0[1]=tbuf[ 2]; ct0[2]=tbuf[ 4]; ct0[3]=tbuf[ 6];
	ct1[0]=tbuf[16]; ct1[1]=tbuf[18]; ct1[2]=tbuf[20]; ct1[3]=tbuf[22];
	ct2[0]=tbuf[32]; ct2[1]=tbuf[34]; ct2[2]=tbuf[36]; ct2[3]=tbuf[38];
	ct3[0]=tbuf[48]; ct3[1]=tbuf[50]; ct3[2]=tbuf[52]; ct3[3]=tbuf[54];
}
