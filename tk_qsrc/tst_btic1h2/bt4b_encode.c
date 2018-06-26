int btic4b_subfold(int a, int b)
{
	int c;
	c=a-b;
	c=(c<<1)^(c>>31);
	return(c);
}


// #define BTIC4B_BYTES

#ifdef BTIC4B_BYTES
void BTIC4B_EncEmitGenericUVal(BTIC4B_Context *ctx, int cv)
{
	if(cv<0x80)
	{
		*ctx->ct++=cv;
	}else if(cv<0x4000)
	{
		*ctx->ct++=0x80|(cv>>8);
		*ctx->ct++=cv;
	}else if(cv<0x200000)
	{
		*ctx->ct++=0xC0|(cv>>16);
		*ctx->ct++=cv>>8;
		*ctx->ct++=cv;
	}else if(cv<0x10000000)
	{
		*ctx->ct++=0xE0|(cv>>16);
		*ctx->ct++=cv>>16;
		*ctx->ct++=cv>>8;
		*ctx->ct++=cv;
	}else if(cv<0x10000000)
	{
		*ctx->ct++=0xF0;
		*ctx->ct++=cv>>24;
		*ctx->ct++=cv>>16;
		*ctx->ct++=cv>>8;
		*ctx->ct++=cv;
	}
}

void BTIC4B_EncEmitGenericVal(BTIC4B_Context *ctx, int cv)
{
	BTIC4B_EncEmitGenericUVal(ctx, (cv<<1)^(cv>>31));
}

void BTIC4B_EncEmitCount(BTIC4B_Context *ctx, int cv)
{
	BTIC4B_EncEmitGenericUVal(ctx, (cv<<1)^(cv>>31));
}

force_inline void BTIC4B_EncEmitCommand(BTIC4B_Context *ctx, int cmd)
{
	*ctx->ct++=cmd;
}

#else

void BTIC4B_EncEmitGenericUVal(BTIC4B_Context *ctx, int cv)
{
	BTIC4B_WriteAdRiceLL(ctx, cv, &(ctx->rk_misc));
}

void BTIC4B_EncEmitGenericVal(BTIC4B_Context *ctx, int cv)
{
	BTIC4B_WriteAdSRiceLL(ctx, cv, &(ctx->rk_misc));
}

void BTIC4B_EncEmitCount(BTIC4B_Context *ctx, int cv)
{
	BTIC4B_WriteAdRiceLL(ctx, cv, &(ctx->rk_cnt));
}

force_inline void BTIC4B_EncEmitCommand(BTIC4B_Context *ctx, int cmd)
{
	BTIC4B_WriteSymbolSmtf(ctx, &(ctx->sm_cmd), cmd);
}

#endif

#ifndef BTIC4B_BYTES
force_inline int BTIC4B_EncEmitMask(BTIC4B_Context *ctx, int msk, int mmsk)
{
	int i0, i1;
	int i, j, k;
//	*ctx->ct++=cmd;
//	BTIC4B_WriteSymbolSmtf(ctx, &(ctx->sm_mask), cmd);

//	if(((ctx->cmask&mmsk)==msk) ||
//		(((ctx->cmask|msk)&mmsk)==(ctx->cmask&mmsk)))
	if((ctx->cmask==(ctx->cmask|msk)) &&
		((ctx->cmask&mmsk)==((ctx->imask|msk)&mmsk)))
//	if((ctx->cmask&mmsk)==msk)
	{
		i0=ctx->stat_bits;
		BTIC4B_WriteAdRiceLL(ctx, 0, &(ctx->sm_mask.rk));

		i1=ctx->stat_bits;
		ctx->sm_mask.bits+=i1-i0;
		ctx->sm_mask.cnt++;

		return(ctx->cmask);
	}

	msk=(ctx->imask&(~mmsk))|msk|0;
//	msk=(ctx->imask&(~mmsk))|msk|1;
	
	if(((ctx->imask|msk)&mmsk)==(ctx->imask&mmsk))
	{
		msk=ctx->imask|1;
	}

//	msk|=0x80;

	i0=ctx->stat_bits;

	i=BTIC4B_EmitIndexSymbolSmtf(ctx, &(ctx->sm_mask), msk);
	BTIC4B_WriteAdRiceLL(ctx, i+1, &(ctx->sm_mask.rk));
	if(msk&1)
		ctx->cmask=msk;

	i1=ctx->stat_bits;
	ctx->sm_mask.bits+=i1-i0;
	ctx->sm_mask.cnt++;

	return(msk);
}
#endif

void BTIC4B_EmitEnableMask(BTIC4B_Context *ctx)
{
	BTIC4B_EncEmitCommand(ctx, 0x46);
	BTIC4B_WriteAdSRiceLL(ctx, -1, &ctx->rk_misc);
	ctx->cmask=0xFF;
}

void BTIC4B_EmitDisableMask(BTIC4B_Context *ctx)
{
	BTIC4B_EncEmitCommand(ctx, 0x47);
	BTIC4B_WriteAdSRiceLL(ctx, -1, &ctx->rk_misc);
	ctx->cmask=0x00;
}

void BTIC4B_EmitSetParm(BTIC4B_Context *ctx, int var, int val)
{
	BTIC4B_EncEmitCommand(ctx, 0x43);
	BTIC4B_WriteAdSRiceLL(ctx, var, &ctx->rk_misc);
	BTIC4B_WriteAdSRiceLL(ctx, val, &ctx->rk_misc);
	
	if(var==-2)
	{
		ctx->pred=val;
	}
}

int BTIC4B_EncCheckEnableMask(BTIC4B_Context *ctx)
{
	int i, j, k, j0, j1;

//	return(0);

	if(ctx->stat_yuvcnt<256)
		return(0);

	if(ctx->cmask)
	{
//		j=ctx->stat_yuvcnt>>1;
		j=(ctx->stat_yuvcnt*5)>>3;
		k=(ctx->stat_yuvcnt*3)>>3;
		j0=-j;
		j1=-k;
	
		i=ctx->imask;
		if(ctx->yuv_cz[0]<j0)i&=(~0x02);
		if(ctx->yuv_cz[1]<j0)i&=(~0x04);
		if(ctx->yuv_cz[2]<j0)i&=(~0x08);
		if(ctx->yuv_cz[3]<j0)i&=(~0x10);
		if(ctx->yuv_cz[4]<j0)i&=(~0x20);
		if(ctx->yuv_cz[5]<j0)i&=(~0x40);

//		if(ctx->yuv_cz[0]>j)i|=0x02;
//		if(ctx->yuv_cz[1]>j)i|=0x04;
//		if(ctx->yuv_cz[2]>j)i|=0x08;
//		if(ctx->yuv_cz[3]>j)i|=0x10;
//		if(ctx->yuv_cz[4]>j)i|=0x20;
//		if(ctx->yuv_cz[5]>j)i|=0x40;

		if(ctx->yuv_cz[0]>j1)i|=0x02;
		if(ctx->yuv_cz[1]>j1)i|=0x04;
		if(ctx->yuv_cz[2]>j1)i|=0x08;
		if(ctx->yuv_cz[3]>j1)i|=0x10;
		if(ctx->yuv_cz[4]>j1)i|=0x20;
		if(ctx->yuv_cz[5]>j1)i|=0x40;

		if(i==0xFF)
		{
			BTIC4B_EmitDisableMask(ctx);
		}

		ctx->imask=i;
		return(0);
	}

//	j=ctx->stat_yuvcnt>>1;
	j=(ctx->stat_yuvcnt*5)>>3;
	j0=-j;

	i=0xFF;
	if(ctx->yuv_cz[0]<j0)i&=(~0x02);
	if(ctx->yuv_cz[1]<j0)i&=(~0x04);
	if(ctx->yuv_cz[2]<j0)i&=(~0x08);
	if(ctx->yuv_cz[3]<j0)i&=(~0x10);
	if(ctx->yuv_cz[4]<j0)i&=(~0x20);
	if(ctx->yuv_cz[5]<j0)i&=(~0x40);

	if(i==0xFF)
		return(0);

	BTIC4B_EmitEnableMask(ctx);
	ctx->imask=i;
	return(0);
}

void BTIC4B_FastEncYUV(BTIC4B_Context *ctx, int cy, int cu, int cv)
{
	int dcy, dcu, dcv, mv;
	int adcy, adcu, adcv;
	int i0, i1;
	int i, j, k;
	
//	dcy=((cy-ctx->cy)*ctx->qfy +128)>>8;
//	dcu=((cu-ctx->cu)*ctx->qfuv+128)>>8;
//	dcv=((cv-ctx->cv)*ctx->qfuv+128)>>8;

	dcy=((cy-ctx->cy)*ctx->qfy +2048)>>12;
	dcu=((cu-ctx->cu)*ctx->qfuv+2048)>>12;
	dcv=((cv-ctx->cv)*ctx->qfuv+2048)>>12;

#ifdef BTIC4B_BYTES

	adcy=(dcy<<1)^(dcy>>31);
	adcu=(dcu<<1)^(dcu>>31);
	adcv=(dcv<<1)^(dcv>>31);
	k=adcy|adcu|adcv;

#if 1
	if(!k)
	{
		*ctx->ct++=0;
	}else if(!(k>>5))
	{
		mv=(adcv<<11)|(adcu<<6)|(adcy<<1)|1;
		*ctx->ct++=mv;
		*ctx->ct++=mv>>8;
	}else
#endif
//	if(1)
	{
		mv=0;
		if(dcy)mv|=0x02;
		if(dcu)mv|=0x04;
		if(dcv)mv|=0x08;

		*ctx->ct++=mv;
		if(mv&0x02)BTIC4B_EncEmitGenericVal(ctx, dcy);
		if(mv&0x04)BTIC4B_EncEmitGenericVal(ctx, dcu);
		if(mv&0x08)BTIC4B_EncEmitGenericVal(ctx, dcv);
	}

#else
	i0=ctx->stat_bits;

	if(ctx->cmask)
	{
		mv=0;
		if(dcy)mv|=0x02;
		if(dcu)mv|=0x04;
		if(dcv)mv|=0x08;

		ctx->yuv_cz[0]+=(dcy!=0)*2-1;
		ctx->yuv_cz[1]+=(dcu!=0)*2-1;
		ctx->yuv_cz[2]+=(dcv!=0)*2-1;
	
		mv=BTIC4B_EncEmitMask(ctx, mv, 0x0E);
//		if(mv&0x02)BTIC4B_WriteAdSRiceLL(ctx, dcy, &ctx->rk_cy );
//		if(mv&0x04)BTIC4B_WriteAdSRiceLL(ctx, dcu, &ctx->rk_cuv);
//		if(mv&0x08)BTIC4B_WriteAdSRiceLL(ctx, dcv, &ctx->rk_cuv);

		if(mv&0x02)BTIC4B_WriteAdSRiceLLb(ctx,
			dcy, &ctx->rk_cy , ctx->stat_yuvcbits+0);
		if(mv&0x04)BTIC4B_WriteAdSRiceLLb(ctx,
			dcu, &ctx->rk_cuv, ctx->stat_yuvcbits+1);
		if(mv&0x08)BTIC4B_WriteAdSRiceLLb(ctx,
			dcv, &ctx->rk_cuv, ctx->stat_yuvcbits+2);
	}else
	{
		ctx->yuv_cz[0]+=(dcy!=0)*2-1;
		ctx->yuv_cz[1]+=(dcu!=0)*2-1;
		ctx->yuv_cz[2]+=(dcv!=0)*2-1;
	
//		BTIC4B_WriteAdSRiceLL(ctx, dcy, &ctx->rk_cy );
//		BTIC4B_WriteAdSRiceLL(ctx, dcu, &ctx->rk_cuv);
//		BTIC4B_WriteAdSRiceLL(ctx, dcv, &ctx->rk_cuv);

		BTIC4B_WriteAdSRiceLLb(ctx, dcy, &ctx->rk_cy , ctx->stat_yuvcbits+0);
		BTIC4B_WriteAdSRiceLLb(ctx, dcu, &ctx->rk_cuv, ctx->stat_yuvcbits+1);
		BTIC4B_WriteAdSRiceLLb(ctx, dcv, &ctx->rk_cuv, ctx->stat_yuvcbits+2);
	}

	i1=ctx->stat_bits;
	ctx->stat_yuvbits+=i1-i0;
	ctx->stat_yuvcnt++;

#endif

	ctx->cy+=dcy*ctx->qy;
	ctx->cu+=dcu*ctx->quv;
	ctx->cv+=dcv*ctx->quv;
	
#if 0
	if(	(ctx->cy<   0) || (ctx->cy>255) ||
		(ctx->cu<-255) || (ctx->cu>255) ||
		(ctx->cv<-255) || (ctx->cv>255))
	{
		__asm { int 3 }
	}
#endif
}

void BTIC4B_FastEncYUVD(BTIC4B_Context *ctx,
	int cy, int cu, int cv, int dy)
{
	int dcy, dcu, dcv, ddy;
	int adcy, adcu, adcv, addy;
	int i0, i1;
	int k, mv;
	
//	dcy=((cy-ctx->cy)*ctx->qfy +128)>>8;
//	dcu=((cu-ctx->cu)*ctx->qfuv+128)>>8;
//	dcv=((cv-ctx->cv)*ctx->qfuv+128)>>8;
//	ddy=((dy-ctx->dy)*ctx->qfdy+128)>>8;

	dcy=((cy-ctx->cy)*ctx->qfy +2048)>>12;
	dcu=((cu-ctx->cu)*ctx->qfuv+2048)>>12;
	dcv=((cv-ctx->cv)*ctx->qfuv+2048)>>12;
	ddy=((dy-ctx->dy)*ctx->qfdy+2048)>>12;

#ifndef BTIC4B_BYTES
	i0=ctx->stat_bits;

	if(ctx->cmask)
	{
		mv=0;
		if(dcy)mv|=0x02;
		if(dcu)mv|=0x04;
		if(dcv)mv|=0x08;
		if(ddy)mv|=0x10;

		ctx->yuv_cz[0]+=(dcy!=0)*2-1;
		ctx->yuv_cz[1]+=(dcu!=0)*2-1;
		ctx->yuv_cz[2]+=(dcv!=0)*2-1;
		ctx->yuv_cz[3]+=(ddy!=0)*2-1;

		mv=BTIC4B_EncEmitMask(ctx, mv, 0x1E);
//		if(mv&0x02)BTIC4B_WriteAdSRiceLL(ctx, dcy, &ctx->rk_cy );
//		if(mv&0x04)BTIC4B_WriteAdSRiceLL(ctx, dcu, &ctx->rk_cuv);
//		if(mv&0x08)BTIC4B_WriteAdSRiceLL(ctx, dcv, &ctx->rk_cuv);
//		if(mv&0x10)BTIC4B_WriteAdSRiceLL(ctx, ddy, &ctx->rk_dy );

		if(mv&0x02)BTIC4B_WriteAdSRiceLLb(ctx,
			dcy, &ctx->rk_cy , ctx->stat_yuvcbits+0);
		if(mv&0x04)BTIC4B_WriteAdSRiceLLb(ctx,
			dcu, &ctx->rk_cuv, ctx->stat_yuvcbits+1);
		if(mv&0x08)BTIC4B_WriteAdSRiceLLb(ctx,
			dcv, &ctx->rk_cuv, ctx->stat_yuvcbits+2);
		if(mv&0x10)BTIC4B_WriteAdSRiceLLb(ctx,
			ddy, &ctx->rk_dy , ctx->stat_yuvcbits+3);
	}else
	{
		ctx->yuv_cz[0]+=(dcy!=0)*2-1;
		ctx->yuv_cz[1]+=(dcu!=0)*2-1;
		ctx->yuv_cz[2]+=(dcv!=0)*2-1;
		ctx->yuv_cz[3]+=(ddy!=0)*2-1;

//		BTIC4B_WriteAdSRiceLL(ctx, dcy, &ctx->rk_cy );
//		BTIC4B_WriteAdSRiceLL(ctx, dcu, &ctx->rk_cuv);
//		BTIC4B_WriteAdSRiceLL(ctx, dcv, &ctx->rk_cuv);
//		BTIC4B_WriteAdSRiceLL(ctx, ddy, &ctx->rk_dy );

		BTIC4B_WriteAdSRiceLLb(ctx, dcy, &ctx->rk_cy , ctx->stat_yuvcbits+0);
		BTIC4B_WriteAdSRiceLLb(ctx, dcu, &ctx->rk_cuv, ctx->stat_yuvcbits+1);
		BTIC4B_WriteAdSRiceLLb(ctx, dcv, &ctx->rk_cuv, ctx->stat_yuvcbits+2);
		BTIC4B_WriteAdSRiceLLb(ctx, ddy, &ctx->rk_dy , ctx->stat_yuvcbits+3);
	}

	i1=ctx->stat_bits;
	ctx->stat_yuvbits+=i1-i0;
	ctx->stat_yuvcnt++;
#else
	adcy=(dcy<<1)^(dcy>>31);
	adcu=(dcu<<1)^(dcu>>31);
	adcv=(dcv<<1)^(dcv>>31);
	addy=(ddy<<1)^(ddy>>31);
	k=adcy|adcu|adcv|addy;

	if(!k)
	{
		*ctx->ct++=0;
	}else if(!(k>>5))
	{
		mv=(addy<<16)|(adcv<<11)|(adcu<<6)|(adcy<<1)|1;
		*ctx->ct++=mv;
		*ctx->ct++=mv>>8;
		*ctx->ct++=mv>>16;
	}else
	{
		mv=0;
		if(dcy)mv|=0x02;
		if(dcu)mv|=0x04;
		if(dcv)mv|=0x08;
		if(ddy)mv|=0x10;

		*ctx->ct++=mv;
		if(mv&0x02)BTIC4B_EncEmitGenericVal(ctx, dcy);
		if(mv&0x04)BTIC4B_EncEmitGenericVal(ctx, dcu);
		if(mv&0x08)BTIC4B_EncEmitGenericVal(ctx, dcv);
		if(mv&0x10)BTIC4B_EncEmitGenericVal(ctx, ddy);
	}
#endif

	ctx->cy+=dcy*ctx->qy;
	ctx->cu+=dcu*ctx->quv;
	ctx->cv+=dcv*ctx->quv;
	ctx->dy+=ddy*ctx->qdy;
	
#if 0
	if(	(ctx->cy<   0) || (ctx->cy>255) ||
		(ctx->cu<-255) || (ctx->cu>255) ||
		(ctx->cv<-255) || (ctx->cv>255) ||
		(ctx->dy<-255) || (ctx->dy>255))
	{
		__asm { int 3 }
	}
#endif
}

void BTIC4B_FastEncYUVDyuv(BTIC4B_Context *ctx,
	int cy, int cu, int cv,
	int dy, int du, int dv)
{
	int dcy, dcu, dcv, ddy, ddu, ddv;
	int adcy, adcu, adcv, addy, addu, addv;
	int i0, i1;
	int k, mv;
	
//	dcy=((cy-ctx->cy)*ctx->qfy  +128)>>8;
//	dcu=((cu-ctx->cu)*ctx->qfuv +128)>>8;
//	dcv=((cv-ctx->cv)*ctx->qfuv +128)>>8;
//	ddy=((dy-ctx->dy)*ctx->qfdy +128)>>8;
//	ddu=((du-ctx->du)*ctx->qfduv+128)>>8;
//	ddv=((dv-ctx->dv)*ctx->qfduv+128)>>8;

	dcy=((cy-ctx->cy)*ctx->qfy  +2048)>>12;
	dcu=((cu-ctx->cu)*ctx->qfuv +2048)>>12;
	dcv=((cv-ctx->cv)*ctx->qfuv +2048)>>12;
	ddy=((dy-ctx->dy)*ctx->qfdy +2048)>>12;
	ddu=((du-ctx->du)*ctx->qfduv+2048)>>12;
	ddv=((dv-ctx->dv)*ctx->qfduv+2048)>>12;

#ifndef BTIC4B_BYTES

	i0=ctx->stat_bits;

	if(ctx->cmask)
	{
		mv=0;
		if(dcy)mv|=0x02;
		if(dcu)mv|=0x04;
		if(dcv)mv|=0x08;
		if(ddy)mv|=0x10;
		if(ddu)mv|=0x20;
		if(ddv)mv|=0x40;

		ctx->yuv_cz[0]+=(dcy!=0)*2-1;
		ctx->yuv_cz[1]+=(dcu!=0)*2-1;
		ctx->yuv_cz[2]+=(dcv!=0)*2-1;
		ctx->yuv_cz[3]+=(ddy!=0)*2-1;
		ctx->yuv_cz[4]+=(ddu!=0)*2-1;
		ctx->yuv_cz[5]+=(ddv!=0)*2-1;

		mv=BTIC4B_EncEmitMask(ctx, mv, 0x7E);
//		if(mv&0x02)BTIC4B_WriteAdSRiceLL(ctx, dcy, &ctx->rk_cy );
//		if(mv&0x04)BTIC4B_WriteAdSRiceLL(ctx, dcu, &ctx->rk_cuv);
//		if(mv&0x08)BTIC4B_WriteAdSRiceLL(ctx, dcv, &ctx->rk_cuv);
//		if(mv&0x10)BTIC4B_WriteAdSRiceLL(ctx, ddy, &ctx->rk_dy );
//		if(mv&0x20)BTIC4B_WriteAdSRiceLL(ctx, ddu, &ctx->rk_duv);
//		if(mv&0x40)BTIC4B_WriteAdSRiceLL(ctx, ddv, &ctx->rk_duv);

		if(mv&0x02)BTIC4B_WriteAdSRiceLLb(ctx,
			dcy, &ctx->rk_cy , ctx->stat_yuvcbits+0);
		if(mv&0x04)BTIC4B_WriteAdSRiceLLb(ctx,
			dcu, &ctx->rk_cuv, ctx->stat_yuvcbits+1);
		if(mv&0x08)BTIC4B_WriteAdSRiceLLb(ctx,
			dcv, &ctx->rk_cuv, ctx->stat_yuvcbits+2);
		if(mv&0x10)BTIC4B_WriteAdSRiceLLb(ctx,
			ddy, &ctx->rk_dy , ctx->stat_yuvcbits+3);
		if(mv&0x20)BTIC4B_WriteAdSRiceLLb(ctx,
			ddu, &ctx->rk_duv, ctx->stat_yuvcbits+4);
		if(mv&0x40)BTIC4B_WriteAdSRiceLLb(ctx,
			ddv, &ctx->rk_duv, ctx->stat_yuvcbits+5);
	}else
	{
		ctx->yuv_cz[0]+=(dcy!=0)*2-1;
		ctx->yuv_cz[1]+=(dcu!=0)*2-1;
		ctx->yuv_cz[2]+=(dcv!=0)*2-1;
		ctx->yuv_cz[3]+=(ddy!=0)*2-1;
		ctx->yuv_cz[4]+=(ddu!=0)*2-1;
		ctx->yuv_cz[5]+=(ddv!=0)*2-1;

//		BTIC4B_WriteAdSRiceLL(ctx, dcy, &ctx->rk_cy );
//		BTIC4B_WriteAdSRiceLL(ctx, dcu, &ctx->rk_cuv);
//		BTIC4B_WriteAdSRiceLL(ctx, dcv, &ctx->rk_cuv);
//		BTIC4B_WriteAdSRiceLL(ctx, ddy, &ctx->rk_dy );
//		BTIC4B_WriteAdSRiceLL(ctx, ddu, &ctx->rk_duv);
//		BTIC4B_WriteAdSRiceLL(ctx, ddv, &ctx->rk_duv);

		BTIC4B_WriteAdSRiceLLb(ctx, dcy, &ctx->rk_cy , ctx->stat_yuvcbits+0);
		BTIC4B_WriteAdSRiceLLb(ctx, dcu, &ctx->rk_cuv, ctx->stat_yuvcbits+1);
		BTIC4B_WriteAdSRiceLLb(ctx, dcv, &ctx->rk_cuv, ctx->stat_yuvcbits+2);
		BTIC4B_WriteAdSRiceLLb(ctx, ddy, &ctx->rk_dy , ctx->stat_yuvcbits+3);
		BTIC4B_WriteAdSRiceLLb(ctx, ddu, &ctx->rk_duv, ctx->stat_yuvcbits+4);
		BTIC4B_WriteAdSRiceLLb(ctx, ddv, &ctx->rk_duv, ctx->stat_yuvcbits+5);
	}

	i1=ctx->stat_bits;
	ctx->stat_yuvbits+=i1-i0;
	ctx->stat_yuvcnt++;
#else

	adcy=(dcy<<1)^(dcy>>31);
	adcu=(dcu<<1)^(dcu>>31);
	adcv=(dcv<<1)^(dcv>>31);
	addy=(ddy<<1)^(ddy>>31);
	addu=(ddu<<1)^(ddu>>31);
	addv=(ddv<<1)^(ddv>>31);
	k=adcy|adcu|adcv|addy|addu|addv;

	if(!k)
	{
		*ctx->ct++=0;
	}else if(!(k>>5))
	{
		mv=(addv<<27)|(addu<<22)|(1<<21)|(addy<<16)|
			(adcv<<11)|(adcu<<6)|(adcy<<1)|1;
		*(u32 *)ctx->ct=mv;
		ctx->ct+=4;
	}else
	{
		mv=0;
		if(dcy)mv|=0x02;
		if(dcu)mv|=0x04;
		if(dcv)mv|=0x08;
		if(ddy)mv|=0x10;
		if(ddu)mv|=0x20;
		if(ddv)mv|=0x40;

		*ctx->ct++=mv;
		if(mv&0x02)BTIC4B_EncEmitGenericVal(ctx, dcy);
		if(mv&0x04)BTIC4B_EncEmitGenericVal(ctx, dcu);
		if(mv&0x08)BTIC4B_EncEmitGenericVal(ctx, dcv);
		if(mv&0x10)BTIC4B_EncEmitGenericVal(ctx, ddy);
		if(mv&0x20)BTIC4B_EncEmitGenericVal(ctx, ddu);
		if(mv&0x40)BTIC4B_EncEmitGenericVal(ctx, ddv);
	}

#endif

	ctx->cy+=dcy*ctx->qy;
	ctx->cu+=dcu*ctx->quv;
	ctx->cv+=dcv*ctx->quv;
	ctx->dy+=ddy*ctx->qdy;
	ctx->du+=ddu*ctx->qduv;
	ctx->dv+=ddv*ctx->qduv;

#if 0
	if(	(ctx->cy<   0) || (ctx->cy>255) ||
		(ctx->cu<-255) || (ctx->cu>255) ||
		(ctx->cv<-255) || (ctx->cv>255) ||
		(ctx->dy<-255) || (ctx->dy>255) ||
		(ctx->du<-255) || (ctx->du>255) ||
		(ctx->dv<-255) || (ctx->dv>255))
	{
		__asm { int 3 }
	}
#endif
}


#if 1
void BTIC4B_FastEncY(BTIC4B_Context *ctx, int cy)
{
	int dcy, mv;
	int adcy;
	int i, j, k;
	
	dcy=((cy-ctx->cy)*ctx->qfy +2048)>>12;

#ifdef BTIC4B_BYTES
	adcy=(dcy<<1)^(dcy>>31);
	if(1)
	{
		mv=0;
		if(dcy)mv|=0x02;
		*ctx->ct++=mv;
		if(mv&0x02)BTIC4B_EncEmitGenericVal(ctx, dcy);
	}
#else
	if(ctx->cmask)
	{
		mv=0;
		if(dcy)mv|=0x02;
		mv=BTIC4B_EncEmitMask(ctx, mv, 0x02);
		if(mv&0x02)BTIC4B_WriteAdSRiceLL(ctx, dcy, &ctx->rk_cy );
	}else
	{
		BTIC4B_WriteAdSRiceLL(ctx, dcy, &ctx->rk_cy );
	}
#endif

	ctx->cy+=dcy*ctx->qy;
}

void BTIC4B_FastEncYD(BTIC4B_Context *ctx, int cy, int dy)
{
	int dcy, ddy, mv;
	int adcy, addy;
	int i, j, k;
	
	dcy=((cy-ctx->cy)*ctx->qfy +2048)>>12;
	ddy=((dy-ctx->dy)*ctx->qfdy+2048)>>12;

#ifdef BTIC4B_BYTES

	adcy=(dcy<<1)^(dcy>>31);
	addy=(ddy<<1)^(ddy>>31);
	k=adcy|addy;

#if 1
	if(!k)
	{
		*ctx->ct++=0;
	}else if(!(k>>7))
	{
		mv=(addy<<8)|(adcy<<1)|1;
		*ctx->ct++=mv;
		*ctx->ct++=mv>>8;
	}else
#endif
//	if(1)
	{
		mv=0;
		if(dcy)mv|=0x02;
		if(ddy)mv|=0x10;

		*ctx->ct++=mv;
		if(mv&0x02)BTIC4B_EncEmitGenericVal(ctx, dcy);
		if(mv&0x10)BTIC4B_EncEmitGenericVal(ctx, ddy);
	}

#else
	if(ctx->cmask)
	{
		mv=0;
		if(dcy)mv|=0x02;
		if(ddy)mv|=0x10;
	
		mv=BTIC4B_EncEmitMask(ctx, mv, 0x12);
		if(mv&0x02)BTIC4B_WriteAdSRiceLL(ctx, dcy, &ctx->rk_cy);
		if(mv&0x10)BTIC4B_WriteAdSRiceLL(ctx, ddy, &ctx->rk_dy);
	}else
	{
		BTIC4B_WriteAdSRiceLL(ctx, dcy, &ctx->rk_cy);
		BTIC4B_WriteAdSRiceLL(ctx, ddy, &ctx->rk_dy);
	}
#endif

	ctx->cy+=dcy*ctx->qy;
	ctx->dy+=ddy*ctx->qdy;
}

void BTIC4B_FastEncA(BTIC4B_Context *ctx, int cy)
{
	BTIC4B_FastEncY(ctx, cy);
}

void BTIC4B_FastEncAD(BTIC4B_Context *ctx, int cy, int dy)
{
	BTIC4B_FastEncYD(ctx, cy, dy);
}

#endif


#ifndef BTIC4B_BYTES

force_inline void BTIC4B_EncEmit4B(BTIC4B_Context *ctx, byte *buf)
{
	BTIC4B_WriteNBits(ctx, *buf, 4);
	ctx->stat_pixbits+=4;
}

force_inline void BTIC4B_EncEmit8B(BTIC4B_Context *ctx, byte *buf)
{
	BTIC4B_Write8BitsNM(ctx, *buf);
	ctx->stat_pixbits+=8;
}

force_inline void BTIC4B_EncEmit16B(BTIC4B_Context *ctx, byte *buf)
{
	BTIC4B_Write16BitsNM(ctx, *(u16 *)buf);
	ctx->stat_pixbits+=16;
}

force_inline void BTIC4B_EncEmit32B(BTIC4B_Context *ctx, byte *buf)
{
	BTIC4B_Write32Bits(ctx, *(u32 *)buf);
	ctx->stat_pixbits+=32;
}

force_inline void BTIC4B_EncEmit48B(BTIC4B_Context *ctx, byte *buf)
{
	BTIC4B_Write32Bits(ctx, *(u32 *)buf);
	BTIC4B_Write16BitsNM(ctx, *(u16 *)(buf+4));
	ctx->stat_pixbits+=48;
}

force_inline void BTIC4B_EncEmit64B(BTIC4B_Context *ctx, byte *buf)
{
	BTIC4B_Write64Bits(ctx, ((u64 *)buf)[0]);
	ctx->stat_pixbits+=64;
}

force_inline void BTIC4B_EncEmit96B(BTIC4B_Context *ctx, byte *buf)
{
	BTIC4B_Write64Bits(ctx, ((u64 *)buf)[0]);
	BTIC4B_Write32Bits(ctx, ((u32 *)buf)[2]);
	ctx->stat_pixbits+=96;
}

force_inline void BTIC4B_EncEmit128B(BTIC4B_Context *ctx, byte *buf)
{
	BTIC4B_Write64Bits(ctx, ((u64 *)buf)[0]);
	BTIC4B_Write64Bits(ctx, ((u64 *)buf)[1]);
	ctx->stat_pixbits+=128;
}

force_inline void BTIC4B_EncEmit192B(BTIC4B_Context *ctx, byte *buf)
{
	BTIC4B_Write64Bits(ctx, ((u64 *)buf)[0]);
	BTIC4B_Write64Bits(ctx, ((u64 *)buf)[1]);
	BTIC4B_Write64Bits(ctx, ((u64 *)buf)[2]);
	ctx->stat_pixbits+=191;
}

force_inline void BTIC4B_EncEmit256B(BTIC4B_Context *ctx, byte *buf)
{
	BTIC4B_Write64Bits(ctx, ((u64 *)buf)[0]);
	BTIC4B_Write64Bits(ctx, ((u64 *)buf)[1]);
	BTIC4B_Write64Bits(ctx, ((u64 *)buf)[2]);
	BTIC4B_Write64Bits(ctx, ((u64 *)buf)[3]);
	ctx->stat_pixbits+=256;
}

force_inline byte *BTIC4B_EncFlushBits(BTIC4B_Context *ctx)
{
	return(BTIC4B_EndWriteBits(ctx));
}

#else

force_inline void BTIC4B_EncEmit8B(BTIC4B_Context *ctx, byte *buf)
{
	*ctx->ct++=*buf;
}

force_inline void BTIC4B_EncEmit16B(BTIC4B_Context *ctx, byte *buf)
{
	*(u16 *)ctx->ct=*(u16 *)buf;
	ctx->ct+=2;
}

force_inline void BTIC4B_EncEmit32B(BTIC4B_Context *ctx, byte *buf)
{
	*(u32 *)ctx->ct=*(u32 *)buf;
	ctx->ct+=4;
}

force_inline void BTIC4B_EncEmit64B(BTIC4B_Context *ctx, byte *buf)
{
	*(u64 *)ctx->ct=*(u64 *)buf;
	ctx->ct+=8;
}

force_inline void BTIC4B_EncEmit96B(BTIC4B_Context *ctx, byte *buf)
{
	*(u64 *)ctx->ct=*(u64 *)buf;
	*(u32 *)ctx->ct=*(u32 *)buf;
	ctx->ct+=12;
}

force_inline void BTIC4B_EncEmit128B(BTIC4B_Context *ctx, byte *buf)
{
	((u64 *)ctx->ct)[0]=((u64 *)buf)[0];
	((u64 *)ctx->ct)[1]=((u64 *)buf)[1];
	ctx->ct+=16;
}

force_inline void BTIC4B_EncEmit192B(BTIC4B_Context *ctx, byte *buf)
{
	((u64 *)ctx->ct)[0]=((u64 *)buf)[0];
	((u64 *)ctx->ct)[1]=((u64 *)buf)[1];
	((u64 *)ctx->ct)[2]=((u64 *)buf)[2];
	ctx->ct+=24;
}

force_inline void BTIC4B_EncEmit256B(BTIC4B_Context *ctx, byte *buf)
{
	((u64 *)ctx->ct)[0]=((u64 *)buf)[0];
	((u64 *)ctx->ct)[1]=((u64 *)buf)[1];
	((u64 *)ctx->ct)[2]=((u64 *)buf)[2];
	((u64 *)ctx->ct)[3]=((u64 *)buf)[3];
	ctx->ct+=32;
}

force_inline byte *BTIC4B_EncFlushBits(BTIC4B_Context *ctx)
{
	return(ctx->ct);
}

#endif

int BTIC4B_SetupContextQf(BTIC4B_Context *ctx, int qf)
{
	float qsc, qsc15, qscc;
	
	qsc=(100-(qf&127))/25.0;
	qsc15=pow(qsc, 1.5);
	qscc=qsc;
	
	switch(ctx->imgt)
	{
	case BTIC4B_IMGT_LDR8:
	case BTIC4B_IMGT_LDR8A:
		qsc*=0.5;	qsc15*=0.5;
		qscc=qsc;
		break;
	case BTIC4B_IMGT_LDR10:
		qsc*=1.0;	qsc15*=1.0;
		qscc=qsc;
		break;
	case BTIC4B_IMGT_LDR12:
		qsc*=2.5;	qsc15*=2.5;
		qscc=qsc;
		break;
	case BTIC4B_IMGT_HDR16:
	case BTIC4B_IMGT_HDR16A:
		qsc*=8.5;	qsc15*=8.5;
		qscc=qsc*0.75;
		break;
	case BTIC4B_IMGT_HDR12:
		qsc*=0.75;	qsc15*=0.75;
		qscc=qsc*0.5;
		break;
	}
		
	ctx->qfl=qf;
	
	if(!ctx->blksz)
		ctx->blksz=64;
	
//	ctx->qdy_flat=16*qsc;
//	ctx->qdy_flat=20*qsc;
//	ctx->qdy_2x2x2=32*qsc;

	ctx->qdy_flat=32*qsc;
	ctx->qdy_2x2x2=44*qsc;

//	ctx->qdy_4x4x2=48*qsc;
//	ctx->qdy_4x4x2=64*qsc;

//	ctx->qdy_4x4x2=64*qsc15;
	ctx->qdy_4x4x2=80*qsc15;

//	ctx->qdy_4x4x2=56*qsc;
//	ctx->qdy_8x8x2=80*qsc;
//	ctx->qdy_8x8x2=80*qsc15;
//	ctx->qdy_8x8x2=96*qsc;
//	ctx->qdy_8x8x2=96*qsc15;
//	ctx->qdy_8x8x2=112*qsc;
	ctx->qdy_8x8x2=112*qsc15;

//	ctx->qdy_8x8x3=192*qsc;
//	ctx->qdy_8x8x2=128*qsc;
	ctx->qdy_8x8x3=224*qsc15;

//	ctx->qduv_flat=16*qsc;
//	ctx->qduv_flat=32*qsc;
	ctx->qduv_flat=40*qsc15;
//	ctx->qduv_flat=48*qsc;
//	ctx->qduv_flat=64*qsc;
	ctx->qduv_2x2=96*qsc15;
//	ctx->qduv_2x2=128*qsc;

	ctx->qdce_sc=16*(1.6/qsc15);
//	ctx->qdce_sc=20*(1.6/qsc15);
//	ctx->qdce_sc=8*(1.6/qsc15);

	ctx->rk_cy=2;
	ctx->rk_cuv=2;
	ctx->rk_dy=2;
	ctx->rk_duv=2;
	ctx->rk_cnt=2;
	ctx->rk_misc=2;

//	ctx->qy=10*qsc;
	ctx->qy=8*qscc;
//	ctx->qy=6*qsc;

//	ctx->quv=8*qsc;
	ctx->quv=10*qscc;
//	ctx->quv=16*qsc;

//	ctx->qdy=6*qsc;
//	ctx->qdy=10*qsc;
//	ctx->qduv=12*qsc;

	ctx->qdy=12*qscc;
	ctx->qduv=14*qscc;

//	ctx->qduv=8*qsc;
//	ctx->qduv=16*qsc;
	if(ctx->qy<1)ctx->qy=1;
	if(ctx->quv<1)ctx->quv=1;
	if(ctx->qdy<1)ctx->qdy=1;
	if(ctx->qduv<1)ctx->qduv=1;

//	ctx->qfy=256/ctx->qy;
//	ctx->qfuv=256/ctx->quv;
//	ctx->qfdy=256/ctx->qdy;
//	ctx->qfduv=256/ctx->qduv;

	ctx->qfy=4096/ctx->qy;
	ctx->qfuv=4096/ctx->quv;
	ctx->qfdy=4096/ctx->qdy;
	ctx->qfduv=4096/ctx->qduv;
	
	ctx->cy=0;	ctx->cu=0;	ctx->cv=0;
	ctx->dy=0;	ctx->du=0;	ctx->dv=0;
	ctx->cmask=0;
	ctx->imask=0;
	ctx->pred=0;
	
	BTIC4B_ResetSmtfDefault(ctx, &(ctx->sm_cmd));
	BTIC4B_ResetSmtfReverse(ctx, &(ctx->sm_mask));

	return(0);
}

void BTIC4B_EncUpdatePredV(BTIC4B_Context *ctx,
	byte *ct, byte *blks, int *rpyc);

int BTIC4B_EncBlockPriorColorP(BTIC4B_Context *ctx,
	byte *blk, byte *blks)
{
	int pyc[6];
	int dcy, dcu, dcv, ddy, ddu, ddv;
	int adcy, adcu, adcv, addy, addu, addv;
	int cy, cu, cv, dy, du, dv;
	int k, mv, bt;

//	if(sblk[0]!=dblk[0])
//		return(0);

	BTIC4B_EncUpdatePredV(ctx, blk, blks, pyc);

	bt=blk[0];
	cy=*(s16 *)(blk+ 4);
	cu=*(s16 *)(blk+ 6);
	cv=*(s16 *)(blk+ 8);
	dy=*(s16 *)(blk+10);
	du=*(s16 *)(blk+12);
	dv=*(s16 *)(blk+14);

	dcy=((cy-pyc[0])*ctx->qfy  +2048)>>12;
	dcu=((cu-pyc[1])*ctx->qfuv +2048)>>12;
	dcv=((cv-pyc[2])*ctx->qfuv +2048)>>12;
	ddy=((dy-pyc[3])*ctx->qfdy +2048)>>12;
	ddu=((du-pyc[4])*ctx->qfduv+2048)>>12;
	ddv=((dv-pyc[5])*ctx->qfduv+2048)>>12;

	if(dcy|dcu|dcv)
		return(0);
	
	if(bt>0)
	{
		if(ddy)
			return(0);
		if((bt>=0x15) && (ddu|ddv))
			return(0);
	}
	return(1);

}


void BTIC4B_EncodeCopyBlocks(BTIC4B_Context *ctx,
	byte *dblks, byte *sblks, int cnt)
{
	byte *cs, *ct;
	int i;
	
	cs=sblks; ct=dblks;
	for(i=0; i<cnt; i++)
	{
		ct[1]=cs[1];
		ct[2]=cs[2];
		ct[3]=cs[3];
		memcpy(ct, cs, ctx->blksz);
		ct[0]|=0x20;
		ct+=ctx->blksz; cs+=ctx->blksz;
	}

//	memcpy(dblks, sblks, cnt*ctx->blksz);
}

int BTIC4B_CheckSkip(BTIC4B_Context *ctx,
	byte *blk, byte *lblk, int blksz,
	int qf, int dyem, int dyen,
	int duvem, int duven,
	int *rerr)
{
	byte pxa[64*4], pxb[64*4];
	int i0, i1, i2, i3, i4, i5, i6, i7;
	int j0, j1, j2, j3, j4, j5, j6, j7;
	int cy, cu, cv, cd, cm;
	int cy1, cu1, cv1, cd1, cm1;
	int ca, cb, ca1, cb1;
	int e0, e1, e2, e3, edy, euv;
	int mcy, ncy, dcy;
	int i, j, k, l;

	if(!memcmp(blk, lblk, blksz))
	{
		if(rerr)*rerr=0;
		return(2);
	}

#if 0
	if(!btic1h_errfrac4_init)
	{
		for(i=0; i<16; i++)
			for(j=0; j<16; j++)
		{
			btic1h_errfrac4_tab[i][j]=
				btic1h_errfrac2(i>>2, j>>2)+
				btic1h_errfrac2(i&3, j&3);
		}
		
		for(i=0; i<4096; i++)
		{
			j=	((i>>4)&0xC0)|((i>>3)&0x30)|
				((i>>2)&0x0C)|((i>>1)&0x03);
			btic1h_errfrac12to8_tab[i]=j;
		}
		
		btic1h_errfrac4_init=1;
	}
#endif

//	return(0);

//	cy=blk[0]; cu=blk[1]; cv=blk[2]; cd=blk[3];
//	cy1=lblk[0]; cu1=lblk[1]; cv1=lblk[2]; cd1=lblk[3];
//	cm=0; cm1=0;

	cy =*(s16 *)(blk + 4);	cu =*(s16 *)(blk + 6);
	cv =*(s16 *)(blk + 8);	cd =*(s16 *)(blk +10);
	cy1=*(s16 *)(lblk+ 4);	cu1=*(s16 *)(lblk+ 6);
	cv1=*(s16 *)(lblk+ 8);	cd1=*(s16 *)(lblk+10);
	cm=blk[0];				cm1=lblk[0];

#if 1
	e0=btic4b_subfold(cy1, cy);
	e1=btic4b_subfold(cd1, cd);
	e2=btic4b_subfold(cu1, cu);
	e3=btic4b_subfold(cv1, cv);
	
	edy=e0;
	euv=e2+e3;
	
//	if((e0>dyen) || (e1>dyen) || (e2>duven) || (e3>duven))
	if((e0>dyen) || (e1>dyen) || (euv>duven))
	{
//		e3=e2+e3;
		if(rerr)*rerr=e0+(e1>>1)+euv;
		return(0);
	}
#endif

#if 1
	ca=cy+(cd>>1);		cb=cy-(cd>>1);
	ca1=cy1+(cd1>>1);	cb1=cy1-(cd1>>1);
	e0=btic4b_subfold(ca1, cb);
	e1=btic4b_subfold(cb1, ca);

//	if((e0<dyem) && (e1<dyem) && (e2<duvem) && (e3<duvem))
	if((e0<dyem) && (e1<dyem) && (euv<duvem))
	{
		e2=(e0>e1)?e0:e1;
		if(rerr)*rerr=e2+euv;
		return(2);
	}
#endif

#if 1
	mcy=ca; ncy=ca;
	if(cb<mcy)mcy=cb;
	if(cb>ncy)ncy=cb;
	if(ca1<mcy)mcy=ca1;
	if(ca1>ncy)ncy=ca1;
	if(cb1<mcy)mcy=cb1;
	if(cb1>ncy)ncy=cb1;
	
	dcy=ncy-mcy;
#endif

	BTIC4B_DecBlockBGRX(ctx, blk, pxa, 8*4);
	BTIC4B_DecBlockBGRX(ctx, lblk, pxb, 8*4);
	
	e0=0;
	for(i=0; i<64; i++)
	{
		j=pxa[i*4+0]-pxb[i*4+0];
		e0+=j*j;
		j=pxa[i*4+1]-pxb[i*4+1];
		e0+=j*j;
		j=pxa[i*4+2]-pxb[i*4+2];
		e0+=j*j;
	}
	e1=sqrt(e0/(3*64));
	
	if(e1>=dyem)
		return(0);
	return(1);

#if 0
	if(cm!=cm1)
	{
		e2=dcy;
		if(rerr)*rerr=e2+euv;
		return(0);
	}
#endif

#if 0
	switch(cm)
	{
	case 0:
		e3=	btic1h_errfrac8(blk[4], lblk[4])+
			btic1h_errfrac8(blk[5], lblk[5])+
			btic1h_errfrac8(blk[6], lblk[6])+
			btic1h_errfrac8(blk[7], lblk[7]);
		break;
	case 1:
		e3=btic1h_errfrac8(blk[6], lblk[6])*4;
		break;
//	case 2:
//		e3=16*9;
//		break;
	case 8:		case 9:
		e3=btic1h_errfrac4(blk[6]&15, lblk[6]&15)*8;
		break;
	case 10:	case 11:
	case 12:	case 13:
		e3=(btic1h_errfrac8(blk[6], lblk[6])+
			btic1h_errfrac8(blk[7], lblk[7]))*2;
		break;

	case 14:	case 16:
	case 17:	case 24:
	case 27:
		e3=	btic1h_errfrac8(blk[12], lblk[12])+
			btic1h_errfrac8(blk[13], lblk[13])+
			btic1h_errfrac8(blk[14], lblk[14])+
			btic1h_errfrac8(blk[15], lblk[15]);
		break;

	case 15:
	case 21:	case 22:
	case 25:	case 26:
		i=(blk[10]<<16)|(blk[11]<<8)|blk[12];
		j=(blk[13]<<16)|(blk[14]<<8)|blk[15];
		i0=btic1h_errfrac12to8_tab[(i>>12)&4095];
		i1=btic1h_errfrac12to8_tab[(i    )&4095];
		i2=btic1h_errfrac12to8_tab[(j>>12)&4095];
		i3=btic1h_errfrac12to8_tab[(j    )&4095];
		i=(lblk[10]<<16)|(lblk[11]<<8)|lblk[12];
		j=(lblk[13]<<16)|(lblk[14]<<8)|lblk[15];
		j0=btic1h_errfrac12to8_tab[(i>>12)&4095];
		j1=btic1h_errfrac12to8_tab[(i    )&4095];
		j2=btic1h_errfrac12to8_tab[(j>>12)&4095];
		j3=btic1h_errfrac12to8_tab[(j    )&4095];

		e3=	btic1h_errfrac8(i0, j0)+
			btic1h_errfrac8(i1, j1)+
			btic1h_errfrac8(i2, j2)+
			btic1h_errfrac8(i3, j3);
		break;

	case 18:	case 19:
	case 23:
		e3=btic1h_errfrac8(blk[6], lblk[6])*4;
		break;

	case 20:
		e3=	btic1h_errfrac8(blk[12], lblk[12])+
			btic1h_errfrac8(blk[13], lblk[13])+
			btic1h_errfrac8(blk[14], lblk[14])+
			btic1h_errfrac8(blk[15], lblk[15]);
		break;

	default:
		e3=16*9;
		break;
	}

//	e0=btic4b_subfold(cy1, cy);
//	e1=(cd>cd1)?cd:cd1;
//	e2=e0+((e1*e3)>>7);
//	e1=cd+cd1;
//	e2=(e0-cd-cd1)+((e1*e3)>>7);

//	e1=(cd+cd1)>>1;
//	e2=e0+((e1*e3)>>7);
	e2=edy+((dcy*e3)>>7);

	if(rerr)*rerr=e2+euv;
	return(e2<dyem);
#endif

//	return(0);
}


int BTIC4B_EncEmitDbgCmdPfx(BTIC4B_Context *ctx)
{
//	BTIC4B_EncEmitCommand(ctx, 0x4C);	//Debug
	return(0);
}

void BTIC4B_DecUpdateCtxPred(BTIC4B_Context *ctx,
	byte *pba, byte *pbb, byte *pbc, byte pred);
void BTIC4B_DecUpdateCtxPredV(BTIC4B_Context *ctx,
	byte *pba, byte *pbb, byte *pbc, byte pred,
	int *rpyc);

force_inline void BTIC4B_EncUpdatePred(BTIC4B_Context *ctx,
	byte *ct, byte *blks)
{
	byte *ctpa, *ctpb, *ctpc;
	int xs1;

	if(!ctx->pred)
		return;

	xs1=ctx->xsb;
	ctpa=ct-(xs1+1)*ctx->blksz;
	if(ctpa>=blks)
	{
		ctpb=ct-xs1*ctx->blksz;
		ctpc=ct-ctx->blksz;
		BTIC4B_DecUpdateCtxPred(ctx,
			ctpa, ctpb, ctpc, ctx->pred);
	}
}

void BTIC4B_EncUpdatePredV(BTIC4B_Context *ctx,
	byte *ct, byte *blks, int *rpyc)
{
	byte *ctpa, *ctpb, *ctpc;
	int xs1;

	if(!ctx->pred)
	{
		rpyc[0]=ctx->cy;
		rpyc[1]=ctx->cu;
		rpyc[2]=ctx->cv;
		rpyc[3]=ctx->dy;
		rpyc[4]=ctx->du;
		rpyc[5]=ctx->dv;
		return;
	}

	xs1=ctx->xsb;
	ctpa=ct-(xs1+1)*ctx->blksz;
	if(ctpa>=blks)
	{
		ctpb=ct-xs1*ctx->blksz;
		ctpc=ct-ctx->blksz;
		BTIC4B_DecUpdateCtxPredV(ctx,
			ctpa, ctpb, ctpc, ctx->pred, rpyc);
	}
}

force_inline void BTIC4B_EncUpdatePredPost(BTIC4B_Context *ctx,
	byte *blk, byte *blks)
{
	if(!ctx->pred)
		return;
		
//	return;

	if(((blk[0]&0x3F)==0x13) ||
		((blk[0]&0x3F)==0x19))
	{
		*(blk+2)=lqtvq_clamp255(ctx->cy);
		*(blk+5)=lqtvq_clamp255(ctx->dy);
		*(blk+3)=lqtvq_clamp255((ctx->cu>>1)+128);
		*(blk+4)=lqtvq_clamp255((ctx->cv>>1)+128);
		*(blk+6)=lqtvq_clamp255((ctx->du>>1)+128);
		*(blk+7)=lqtvq_clamp255((ctx->dv>>1)+128);
		return;
	}

	*(s16 *)(blk+ 4)=lqtvq_clamp32767S(ctx->cy);
	*(s16 *)(blk+ 6)=lqtvq_clamp32767S(ctx->cu);
	*(s16 *)(blk+ 8)=lqtvq_clamp32767S(ctx->cv);
	*(s16 *)(blk+10)=lqtvq_clamp32767S(ctx->dy);
	*(s16 *)(blk+12)=lqtvq_clamp32767S(ctx->du);
	*(s16 *)(blk+14)=lqtvq_clamp32767S(ctx->dv);
}

int BTIC4B_EncImgBlocks(BTIC4B_Context *ctx,
	byte *cbuf, byte *blks, byte *lblks, int xs, int ys, int qf)
{
	static const int voffs[6*4][2]={
		{-1, 0},{ 0,-1},{1, 0},{ 0, 1},
		{-1, 1},{ 1, 1},{1,-1},{-1,-1},
		{-2, 0},{ 0,-2},{2, 0},{ 0, 2},
		{-2, 1},{ 1,-2},{2, 1},{ 1, 2},
		{-2,-1},{-1,-2},{2,-1},{-1, 2},
		{-2, 2},{ 2, 2},{2,-2},{-2,-2}
		};

	byte *cs, *cs1, *cse, *csl, *csle, *cs1e, *cs2, *csl2, *csle2;
	byte *lblks2;
	byte *blkb;
	int xs1, ys1;
	int bi, bc, bc2;
	int vx, vy, vo;
	int i, j, k, n;
	
	BTIC4B_SetupContextQf(ctx, qf);
	
	BTIC4B_DecImageSetupClrsI(ctx, BTIC4B_CLRS_BGRX);

#ifdef BTIC4B_BYTES
	ctx->ct=cbuf;
#else
	BTIC4B_InitScTables();
	BTIC4B_InitRice();
	BTIC4B_SetupWriteBits(ctx, cbuf, xs*ys*8);
#endif

	xs1=(xs+7)>>3;
	ys1=(ys+7)>>3;
	n=xs1*ys1;
	
	BTIC4B_EncEmitDbgCmdPfx(ctx);	//Debug
	BTIC4B_EncEmitCommand(ctx, 0x45);
	BTIC4B_EncEmitGenericUVal(ctx, ctx->qy);
	BTIC4B_EncEmitGenericUVal(ctx, ctx->quv);
	BTIC4B_EncEmitGenericUVal(ctx, ctx->qdy);
	BTIC4B_EncEmitGenericUVal(ctx, ctx->qduv);

//	if((qf&127)<50)
	if(qf&BTIC4B_QFL_USEPRED)
	{
//		BTIC4B_EmitSetParm(ctx, -2, 1);
//		BTIC4B_EmitSetParm(ctx, -2, 2);
//		BTIC4B_EmitSetParm(ctx, -2, 3);
		BTIC4B_EmitSetParm(ctx, -2, 4);
	}
	
	cs=blks;
	csl=lblks;
	
	cse=cs+n*64;
	csle=csl+n*64;
	
	lblks2=lblks;
	csle2=csle;

	while(cs<cse)
	{
		if(lblks)
		{
			csl=lblks+(cs-blks);
			for(i=0; i<32; i++)
			{
				cs2=cs+i*ctx->blksz;
				csl2=csl+i*ctx->blksz;
				if(cs2>=cse)break;
				if(BTIC4B_CheckSkip(ctx, cs2, csl2, ctx->blksz,
						qf, ctx->qdy_2x2x2/4, ctx->qdy_4x4x2,
							ctx->qduv_2x2/4, 2*ctx->qduv_2x2, &k)<=0)
					break;
			}
			
			if(i>0)
			{
				BTIC4B_EncEmitCommand(ctx, 0x41);
				BTIC4B_EncEmitCount(ctx, i);

				BTIC4B_EncodeCopyBlocks(ctx, cs, csl, i);
				cs+=i*ctx->blksz;
				csl+=i*ctx->blksz;
				continue;
			}

#ifdef BT4B_USETSKIP
			csl=lblks+(cs-blks);
// #if 0
			i=0;
//			for(j=0; j<4; j++)
			for(j=0; j<BT4B_TSKIP_MAX; j++)
			{
				vx=voffs[j][0];
				vy=voffs[j][1];

				vo=(vy*ctx->xsb+vx)*ctx->blksz;
				for(i=0; i<32; i++)
				{
					cs2=cs+i*ctx->blksz;
					csl2=csl+i*ctx->blksz+vo;
					if(cs2>=cse)break;
					if((csl2<lblks2) || (csl2>=csle2))
						break;
					if(BTIC4B_CheckSkip(ctx, cs2, csl2, ctx->blksz,
							qf, ctx->qdy_2x2x2/4, ctx->qdy_4x4x2,
							ctx->qduv_2x2/4, 2*ctx->qduv_2x2, &k)<=0)
						break;
				}
				if(i>0)break;
			}

			if(i>0)
			{
				BTIC4B_EncEmitCommand(ctx, 0x42);
				BTIC4B_EncEmitCount(ctx, i);
				BTIC4B_EncEmitGenericVal(ctx, vx);
				BTIC4B_EncEmitGenericVal(ctx, vy);

				csl2=csl+vo;

				BTIC4B_EncodeCopyBlocks(ctx, cs, csl2, i);

				cs+=i*ctx->blksz;
				csl+=i*ctx->blksz;
//				t1=GPIO_GetTimeUS();
				continue;
			}
#endif
		}

		blkb=cs;
		cs+=ctx->blksz;
		csl+=ctx->blksz;

//		cs1e=cs1+63*ctx->blksz;
		cs1e=cs+31*ctx->blksz;
		if(cse<cs1e)cs1e=cse;

		if(blkb[0]==0x00)
		{
			cs1=cs; bc=1; bc2=0;
//			cs1e=cs1+63*64;
//			if(cse<cs1e)cs1e=cse;
			while((cs1<cs1e) && (cs1[0]==blkb[0]))
			{
				if(BTIC4B_EncBlockPriorColorP(ctx, cs1, blks))
					{ bc2++; }
				else
					{ bc2=0; }
				if(bc2>1)
					break;
				cs1+=ctx->blksz; bc++;
			}
		}else
		{
			bc=0;
		}

		cs1=blkb; bc2=0;
		while((cs1<cs1e) && (cs1[0]==blkb[0]))
		{
			if(!BTIC4B_EncBlockPriorColorP(ctx, cs1, blks))
				break;
			cs1+=ctx->blksz; bc2++;
		}
		
//		bc2=0;

		BTIC4B_EncCheckEnableMask(ctx);
		BTIC4B_EncEmitDbgCmdPfx(ctx);	//Debug

//		switch(*cs)
		switch(blkb[0])
		{
		case 0x00:
#if 1
			if(bc2>2)
			{
				BTIC4B_EncEmitCommand(ctx, 0x20);
				BTIC4B_EncEmitCount(ctx, bc2);
//				cs=blkb+(bc2*64);
				cs=blkb;
				for(i=0; i<bc2; i++)
				{
					BTIC4B_EncUpdatePred(ctx, cs, blks);
					BTIC4B_EncUpdatePredPost(ctx, cs, blks);
					cs+=ctx->blksz;
					csl+=ctx->blksz;
				}
				break;
			}
			if(bc>1)
			{
				BTIC4B_EncEmitCommand(ctx, 0x48);
				BTIC4B_EncEmitCount(ctx, bc);
				
				cs=blkb;
				for(i=0; i<bc; i++)
				{
					BTIC4B_EncUpdatePred(ctx, cs, blks);
					BTIC4B_FastEncYUV(ctx,
						*(s16 *)(cs+ 4), *(s16 *)(cs+ 6),
						*(s16 *)(cs+ 8));
					BTIC4B_EncUpdatePredPost(ctx, cs, blks);
					cs+=ctx->blksz;
					csl+=ctx->blksz;
				}
				break;
			}
#endif
			
			BTIC4B_EncEmitCommand(ctx, 0x00);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUV(ctx,
				*(s16 *)(blkb+ 4), *(s16 *)(blkb+ 6), *(s16 *)(blkb+ 8));
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;

		case 0x01:
#if 1
			if(bc2>3)
			{
				BTIC4B_EncEmitCommand(ctx, 0x21);
				BTIC4B_EncEmitCount(ctx, bc2);
				cs=blkb;
				for(i=0; i<bc2; i++)
				{
					BTIC4B_EncUpdatePred(ctx, cs, blks);
					BTIC4B_EncEmit4B(ctx, cs+16);
					BTIC4B_EncUpdatePredPost(ctx, cs, blks);
					cs+=ctx->blksz;
					csl+=ctx->blksz;
				}
				break;
			}
#endif
			BTIC4B_EncEmitCommand(ctx, 0x01);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVD(ctx,
				*(s16 *)(blkb+ 4), *(s16 *)(blkb+ 6),
				*(s16 *)(blkb+ 8), *(s16 *)(blkb+10));
			BTIC4B_EncEmit4B(ctx, blkb+16);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;
		case 0x02:	case 0x03:
#if 1
			if(bc2>3)
			{
				BTIC4B_EncEmitCommand(ctx, 0x20|blkb[0]);
				BTIC4B_EncEmitCount(ctx, bc2);
				cs=blkb;
				for(i=0; i<bc2; i++)
				{
					BTIC4B_EncUpdatePred(ctx, cs, blks);
					BTIC4B_EncEmit8B(ctx, cs+16);
					BTIC4B_EncUpdatePredPost(ctx, cs, blks);
					cs+=ctx->blksz;
					csl+=ctx->blksz;
				}
				break;
			}
#endif
			BTIC4B_EncEmitCommand(ctx, blkb[0]);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVD(ctx,
				*(s16 *)(blkb+ 4), *(s16 *)(blkb+ 6),
				*(s16 *)(blkb+ 8), *(s16 *)(blkb+10));
			BTIC4B_EncEmit8B(ctx, blkb+16);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;
		case 0x04:
#if 1
			if(bc2>3)
			{
				BTIC4B_EncEmitCommand(ctx, 0x24);
				BTIC4B_EncEmitCount(ctx, bc2);
				cs=blkb;
				for(i=0; i<bc2; i++)
				{
					BTIC4B_EncUpdatePred(ctx, cs, blks);
					BTIC4B_EncEmit16B(ctx, cs+16);
					BTIC4B_EncUpdatePredPost(ctx, cs, blks);
					cs+=ctx->blksz;
					csl+=ctx->blksz;
				}
				break;
			}
#endif
			BTIC4B_EncEmitCommand(ctx, 0x04);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVD(ctx,
				*(s16 *)(blkb+ 4), *(s16 *)(blkb+ 6),
				*(s16 *)(blkb+ 8), *(s16 *)(blkb+10));
			BTIC4B_EncEmit16B(ctx, blkb+16);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;
		case 0x05:	case 0x06:
#if 1
			if(bc2>3)
			{
				BTIC4B_EncEmitCommand(ctx, 0x20|blkb[0]);
				BTIC4B_EncEmitCount(ctx, bc2);
				cs=blkb;
				for(i=0; i<bc2; i++)
				{
					BTIC4B_EncUpdatePred(ctx, cs, blks);
					BTIC4B_EncEmit32B(ctx, cs+16);
					BTIC4B_EncUpdatePredPost(ctx, cs, blks);
					cs+=ctx->blksz;
					csl+=ctx->blksz;
				}
				break;
			}
#endif
			BTIC4B_EncEmitCommand(ctx, blkb[0]);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVD(ctx,
				*(s16 *)(blkb+ 4), *(s16 *)(blkb+ 6),
				*(s16 *)(blkb+ 8), *(s16 *)(blkb+10));
			BTIC4B_EncEmit32B(ctx, blkb+16);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;
		case 0x07:
#if 1
			if(bc2>3)
			{
				BTIC4B_EncEmitCommand(ctx, 0x27);
				BTIC4B_EncEmitCount(ctx, bc2);
				cs=blkb;
				for(i=0; i<bc2; i++)
				{
					BTIC4B_EncUpdatePred(ctx, cs, blks);
					BTIC4B_EncEmit64B(ctx, cs+16);
					BTIC4B_EncUpdatePredPost(ctx, cs, blks);
					cs+=ctx->blksz;
					csl+=ctx->blksz;
				}
				break;
			}
#endif
			BTIC4B_EncEmitCommand(ctx, 0x07);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVD(ctx,
				*(s16 *)(blkb+ 4), *(s16 *)(blkb+ 6),
				*(s16 *)(blkb+ 8), *(s16 *)(blkb+10));
			BTIC4B_EncEmit64B(ctx, blkb+16);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;

		case 0x09:
#if 1
			if(bc2>3)
			{
				BTIC4B_EncEmitCommand(ctx, 0x29);
				BTIC4B_EncEmitCount(ctx, bc2);
				
				cs=blkb;
				for(i=0; i<bc2; i++)
				{
					BTIC4B_EncUpdatePred(ctx, cs, blks);
					BTIC4B_EncEmit8B(ctx, cs+16);
					BTIC4B_EncUpdatePredPost(ctx, cs, blks);
					cs+=ctx->blksz;
					csl+=ctx->blksz;
				}
				break;
			}
#endif
			BTIC4B_EncEmitCommand(ctx, 0x09);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVD(ctx,
				*(s16 *)(blkb+ 4), *(s16 *)(blkb+ 6),
				*(s16 *)(blkb+ 8), *(s16 *)(blkb+10));
			BTIC4B_EncEmit8B(ctx, blkb+16);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;
		case 0x0A:	case 0x0B:
#if 1
			if(bc2>4)
			{
				BTIC4B_EncEmitCommand(ctx, 0x20|blkb[0]);
				BTIC4B_EncEmitCount(ctx, bc2);
				cs=blkb;
				for(i=0; i<bc2; i++)
				{
					BTIC4B_EncUpdatePred(ctx, cs, blks);
					BTIC4B_EncEmit16B(ctx, cs+16);
					BTIC4B_EncUpdatePredPost(ctx, cs, blks);
					cs+=ctx->blksz;
					csl+=ctx->blksz;
				}
				break;
			}
#endif
			BTIC4B_EncEmitCommand(ctx, blkb[0]);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVD(ctx,
				*(s16 *)(blkb+ 4), *(s16 *)(blkb+ 6),
				*(s16 *)(blkb+ 8), *(s16 *)(blkb+10));
			BTIC4B_EncEmit16B(ctx, blkb+16);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;
		case 0x0C:
#if 1
			if(bc2>4)
			{
				BTIC4B_EncEmitCommand(ctx, 0x20|blkb[0]);
				BTIC4B_EncEmitCount(ctx, bc2);
				cs=blkb;
				for(i=0; i<bc2; i++)
				{
					BTIC4B_EncUpdatePred(ctx, cs, blks);
					BTIC4B_EncEmit32B(ctx, cs+16);
					BTIC4B_EncUpdatePredPost(ctx, cs, blks);
					cs+=ctx->blksz;
					csl+=ctx->blksz;
				}
				break;
			}
#endif
			BTIC4B_EncEmitCommand(ctx, 0x0C);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVD(ctx,
				*(s16 *)(blkb+ 4), *(s16 *)(blkb+ 6),
				*(s16 *)(blkb+ 8), *(s16 *)(blkb+10));
			BTIC4B_EncEmit32B(ctx, blkb+16);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;

		case 0x0D:	case 0x0E:
#if 1
			if(bc2>4)
			{
				BTIC4B_EncEmitCommand(ctx, 0x20|blkb[0]);
				BTIC4B_EncEmitCount(ctx, bc2);
				cs=blkb;
				for(i=0; i<bc2; i++)
				{
					BTIC4B_EncUpdatePred(ctx, cs, blks);
					BTIC4B_EncEmit64B(ctx, cs+16);
					BTIC4B_EncUpdatePredPost(ctx, cs, blks);
					cs+=ctx->blksz;
					csl+=ctx->blksz;
				}
				break;
			}
#endif
			BTIC4B_EncEmitCommand(ctx, blkb[0]);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVD(ctx,
				*(s16 *)(blkb+ 4), *(s16 *)(blkb+ 6),
				*(s16 *)(blkb+ 8), *(s16 *)(blkb+10));
			BTIC4B_EncEmit64B(ctx, blkb+16);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;
		case 0x0F:
			BTIC4B_EncEmitCommand(ctx, 0x0F);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVD(ctx,
				*(s16 *)(blkb+ 4), *(s16 *)(blkb+ 6),
				*(s16 *)(blkb+ 8), *(s16 *)(blkb+10));
			BTIC4B_EncEmit128B(ctx, blkb+16);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;
		case 0x10:
			BTIC4B_EncEmitCommand(ctx, blkb[0]);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVD(ctx,
				*(s16 *)(blkb+ 4), *(s16 *)(blkb+ 6),
				*(s16 *)(blkb+ 8), *(s16 *)(blkb+10));
			BTIC4B_EncEmit192B(ctx, blkb+16);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;
		case 0x11:
			BTIC4B_EncEmitCommand(ctx, blkb[0]);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVD(ctx,
				*(s16 *)(blkb+ 4), *(s16 *)(blkb+ 6),
				*(s16 *)(blkb+ 8), *(s16 *)(blkb+10));
			BTIC4B_EncEmit256B(ctx, blkb+16);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;

		case 0x13:
			BTIC4B_EncEmitCommand(ctx, 0x13);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVDyuv(ctx,
				blkb[2], (blkb[3]-128)<<1, (blkb[4]-128)<<1,
				blkb[5], (blkb[6]-128)<<1, (blkb[7]-128)<<1);
			BTIC4B_EncEmit128B(ctx, blkb+8);
			BTIC4B_EncEmit32B(ctx, blkb+24);
			BTIC4B_EncEmit32B(ctx, blkb+28);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;
		case 0x14:
			BTIC4B_EncEmitCommand(ctx, 0x14);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVDyuv(ctx,
				*(s16 *)(blkb+ 4), *(s16 *)(blkb+ 6), *(s16 *)(blkb+ 8),
				*(s16 *)(blkb+10), *(s16 *)(blkb+12), *(s16 *)(blkb+14));
			BTIC4B_EncEmit16B(ctx, blkb+16);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;
		case 0x15:
			BTIC4B_EncEmitCommand(ctx, 0x15);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVDyuv(ctx,
				*(s16 *)(blkb+ 4), *(s16 *)(blkb+ 6), *(s16 *)(blkb+ 8),
				*(s16 *)(blkb+10), *(s16 *)(blkb+12), *(s16 *)(blkb+14));
			BTIC4B_EncEmit32B(ctx, blkb+16);
			BTIC4B_EncEmit8B(ctx, blkb+20);
			BTIC4B_EncEmit8B(ctx, blkb+21);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;
		case 0x16:
			BTIC4B_EncEmitCommand(ctx, 0x16);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVDyuv(ctx,
				*(s16 *)(blkb+ 4), *(s16 *)(blkb+ 6), *(s16 *)(blkb+ 8),
				*(s16 *)(blkb+10), *(s16 *)(blkb+12), *(s16 *)(blkb+14));
			BTIC4B_EncEmit32B(ctx, blkb+16);
			BTIC4B_EncEmit32B(ctx, blkb+20);
			BTIC4B_EncEmit32B(ctx, blkb+24);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;
		case 0x17:
			BTIC4B_EncEmitCommand(ctx, 0x17);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVDyuv(ctx,
				*(s16 *)(blkb+ 4), *(s16 *)(blkb+ 6), *(s16 *)(blkb+ 8),
				*(s16 *)(blkb+10), *(s16 *)(blkb+12), *(s16 *)(blkb+14));
			BTIC4B_EncEmit128B(ctx, blkb+16);
			BTIC4B_EncEmit8B(ctx, blkb+32);
			BTIC4B_EncEmit8B(ctx, blkb+33);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;
		case 0x18:
			BTIC4B_EncEmitCommand(ctx, 0x18);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVDyuv(ctx,
				*(s16 *)(blkb+ 4), *(s16 *)(blkb+ 6), *(s16 *)(blkb+ 8),
				*(s16 *)(blkb+10), *(s16 *)(blkb+12), *(s16 *)(blkb+14));
			BTIC4B_EncEmit128B(ctx, blkb+16);
			BTIC4B_EncEmit32B(ctx, blkb+32);
			BTIC4B_EncEmit32B(ctx, blkb+36);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;
		case 0x19:
			BTIC4B_EncEmitCommand(ctx, 0x19);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVDyuv(ctx,
				blkb[2], (blkb[3]-128)<<1, (blkb[4]-128)<<1,
				blkb[5], (blkb[6]-128)<<1, (blkb[7]-128)<<1);
			BTIC4B_EncEmit192B(ctx, blkb+8);
			BTIC4B_EncEmit128B(ctx, blkb+32);
			BTIC4B_EncEmit128B(ctx, blkb+48);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;
		case 0x1A:
			BTIC4B_EncEmitCommand(ctx, 0x1A);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVDyuv(ctx,
				*(s16 *)(blkb+ 4), *(s16 *)(blkb+ 6), *(s16 *)(blkb+ 8),
				*(s16 *)(blkb+10), *(s16 *)(blkb+12), *(s16 *)(blkb+14));
			BTIC4B_EncEmit128B(ctx, blkb+16);
			BTIC4B_EncEmit64B(ctx, blkb+32);
			BTIC4B_EncEmit64B(ctx, blkb+40);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;
		case 0x1B:
			BTIC4B_EncEmitCommand(ctx, 0x1B);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVDyuv(ctx,
				*(s16 *)(blkb+ 4), *(s16 *)(blkb+ 6), *(s16 *)(blkb+ 8),
				*(s16 *)(blkb+10), *(s16 *)(blkb+12), *(s16 *)(blkb+14));
			BTIC4B_EncEmit128B(ctx, blkb+16);
			BTIC4B_EncEmit128B(ctx, blkb+32);
			BTIC4B_EncEmit128B(ctx, blkb+48);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;
		case 0x1C:
			BTIC4B_EncEmitCommand(ctx, 0x1C);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVDyuv(ctx,
				*(s16 *)(blkb+ 4), *(s16 *)(blkb+ 6), *(s16 *)(blkb+ 8),
				*(s16 *)(blkb+10), *(s16 *)(blkb+12), *(s16 *)(blkb+14));
			BTIC4B_EncEmit192B(ctx, blkb+16);
			BTIC4B_EncEmit32B(ctx, blkb+40);
			BTIC4B_EncEmit32B(ctx, blkb+44);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;
		case 0x1D:
			BTIC4B_EncEmitCommand(ctx, 0x1D);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVDyuv(ctx,
				*(s16 *)(blkb+ 4), *(s16 *)(blkb+ 6), *(s16 *)(blkb+ 8),
				*(s16 *)(blkb+10), *(s16 *)(blkb+12), *(s16 *)(blkb+14));
			BTIC4B_EncEmit256B(ctx, blkb+16);
			BTIC4B_EncEmit64B(ctx, blkb+48);
			BTIC4B_EncEmit64B(ctx, blkb+56);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;
		case 0x1E:
			BTIC4B_EncEmitCommand(ctx, 0x1E);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVDyuv(ctx,
				*(s16 *)(blkb+ 4), *(s16 *)(blkb+ 6), *(s16 *)(blkb+ 8),
				*(s16 *)(blkb+10), *(s16 *)(blkb+12), *(s16 *)(blkb+14));
			BTIC4B_EncEmit192B(ctx, blkb+16);
			BTIC4B_EncEmit96B(ctx, blkb+40);
			BTIC4B_EncEmit96B(ctx, blkb+52);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;
		case 0x1F:
			BTIC4B_EncEmitCommand(ctx, 0x1F);
			BTIC4B_EncUpdatePred(ctx, blkb, blks);
			BTIC4B_FastEncYUVDyuv(ctx,
				*(s16 *)(blkb+ 4), *(s16 *)(blkb+ 6), *(s16 *)(blkb+ 8),
				*(s16 *)(blkb+10), *(s16 *)(blkb+12), *(s16 *)(blkb+14));
			BTIC4B_EncEmit256B(ctx, blkb+16);
			BTIC4B_EncEmit192B(ctx, blkb+48);
			BTIC4B_EncEmit192B(ctx, blkb+72);
			BTIC4B_EncUpdatePredPost(ctx, blkb, blks);
			break;
		default:
//			__asm { int 3 }
//			*(int *)-1=-1;
			BTIC4B_DBGTRAP
			break;
		}
	}

	BTIC4B_EncEmitDbgCmdPfx(ctx);
	BTIC4B_EncEmitCommand(ctx, 0x40);
	
	blkb=BTIC4B_EncFlushBits(ctx);
	return(blkb-cbuf);
}


#if 1
int BTIC4B_EncImgAlphaBlocks(BTIC4B_Context *ctx,
	byte *cbuf, byte *blks, byte *lblks, int xs, int ys, int qf)
{
	byte *cs, *cs1, *cse, *csl;
	byte *blkb;
	int xs1, ys1;
	int bi, bc, bc2;
	int i, j, k, n;

	xs1=(xs+7)>>3;
	ys1=(ys+7)>>3;
	n=xs1*ys1;

#if 1
	cs=blks;
	cse=cs+n*64;

	cs1=cs; bc=0;
	while(cs1<cse)
	{
		if((*(u16 *)(cs1+2))!=0x00FF)
			break;
		cs1+=64;
	}
	if(cs1>=cse)
		return(0);
#endif

	BTIC4B_SetupContextQf(ctx, qf);

#ifdef BTIC4B_BYTES
	ctx->ct=cbuf;
#else
	BTIC4B_SetupWriteBits(ctx, cbuf, xs*ys*8);
#endif

	
	BTIC4B_EncEmitDbgCmdPfx(ctx);	//Debug

	BTIC4B_EncEmitCommand(ctx, 0x45);
	BTIC4B_EncEmitGenericUVal(ctx, ctx->qy);
	BTIC4B_EncEmitGenericUVal(ctx, ctx->qdy);
	
	cs=blks;
	csl=lblks;
	
	cse=cs+n*64;
	
	while(cs<cse)
	{
		blkb=cs;
		cs+=64;

		BTIC4B_EncEmitDbgCmdPfx(ctx);	//Debug

		cs1=blkb; bc=0;
		while(cs1<cse)
		{
			if(((*(u16 *)(cs1+2))!=0x00FF) &&
					((cs1[0]&0x1F)!=0x19) && ((cs1[0]&0x1F)!=0x13))
				break;
			cs1+=64; bc++;
		}
		
		if(cs1>=cse)
			break;

		if(bc)
		{
			BTIC4B_EncEmitCommand(ctx, 0x41);
			BTIC4B_EncEmitCount(ctx, bc);
			cs=blkb+(bc*64);
			continue;
		}

		bc2=0; cs1=blkb;
		while(cs1<cse)
		{
			if(cs1[1]!=0)
				break;
			if(cs1[2]!=ctx->cy)
				break;
			if(((cs1[0]&0x1F)==0x19) ||
					((cs1[0]&0x1F)==0x13))
				break;
			cs1+=64; bc2++;
		}
		
		if(bc2>1)
		{
			BTIC4B_EncEmitCommand(ctx, 0x20);
			BTIC4B_EncEmitCount(ctx, bc2);
			cs=blkb+(bc2*64);
			continue;
		}

//		switch(*cs)
		switch(blkb[1])
		{
		case 0x00:
			BTIC4B_EncEmitCommand(ctx, 0x00);
			BTIC4B_FastEncA(ctx, blkb[2]);
			break;
		case 0x01:
			BTIC4B_EncEmitCommand(ctx, 0x01);
			BTIC4B_FastEncAD(ctx,
				*(s16 *)(blkb+ 48+0),
				*(s16 *)(blkb+ 48+2));
			BTIC4B_EncEmit4B(ctx, blkb+48+8);
			break;
		case 0x02:
			BTIC4B_EncEmitCommand(ctx, 0x02);
			BTIC4B_FastEncAD(ctx,
				*(s16 *)(blkb+ 48+0),
				*(s16 *)(blkb+ 48+2));
			BTIC4B_EncEmit16B(ctx, blkb+48+8);
			break;
		case 0x03:
			BTIC4B_EncEmitCommand(ctx, 0x03);
			BTIC4B_FastEncAD(ctx,
				*(s16 *)(blkb+ 48+0),
				*(s16 *)(blkb+ 48+2));
			BTIC4B_EncEmit64B(ctx, blkb+48+8);
			break;
		case 0x04:
			BTIC4B_EncEmitCommand(ctx, 0x04);
			BTIC4B_FastEncAD(ctx,
				*(s16 *)(blkb+ 48+0),
				*(s16 *)(blkb+ 48+2));
			BTIC4B_EncEmit8B(ctx, blkb+48+8);
			break;
		case 0x05:
			BTIC4B_EncEmitCommand(ctx, 0x05);
			BTIC4B_FastEncAD(ctx,
				*(s16 *)(blkb+ 48+0),
				*(s16 *)(blkb+ 48+2));
			BTIC4B_EncEmit32B(ctx, blkb+48+8);
			break;
		case 0x06:
			BTIC4B_EncEmitCommand(ctx, 0x06);
			BTIC4B_FastEncAD(ctx,
				*(s16 *)(blkb+ 48+0),
				*(s16 *)(blkb+ 48+2));
			BTIC4B_EncEmit48B(ctx, blkb+48+8);
			break;
		case 0x07:
			BTIC4B_EncEmitCommand(ctx, 0x07);
			BTIC4B_FastEncAD(ctx,
				*(s16 *)(blkb+ 48+0),
				*(s16 *)(blkb+ 48+2));
			BTIC4B_EncEmit64B(ctx, blkb+48+8);
			break;
		case 0x08:
			BTIC4B_EncEmitCommand(ctx, 0x08);
			BTIC4B_FastEncA(ctx, *(s16 *)(blkb+48+0));
			break;


		case 0x0A:	case 0x0B:
			BTIC4B_EncEmitCommand(ctx, blkb[0]);
			BTIC4B_FastEncAD(ctx,
				*(s16 *)(blkb+48+0), *(s16 *)(blkb+48+2));
			BTIC4B_EncEmit64B(ctx, blkb+48+8);
			break;

		case 0x0C:
			BTIC4B_EncEmitCommand(ctx, 0x0C);
			BTIC4B_FastEncAD(ctx, blkb[2], blkb[3]);
			BTIC4B_EncEmit128B(ctx, blkb+48);
			break;

		case 0x0D:
			BTIC4B_EncEmitCommand(ctx, 0x0D);
			BTIC4B_FastEncAD(ctx,
				*(s16 *)(blkb+64+0), *(s16 *)(blkb+64+2));
			BTIC4B_EncEmit128B(ctx, blkb+64+8);
			break;
		case 0x0E:
			BTIC4B_EncEmitCommand(ctx, 0x0E);
			BTIC4B_FastEncAD(ctx,
				*(s16 *)(blkb+64+0), *(s16 *)(blkb+64+2));
			BTIC4B_EncEmit192B(ctx, blkb+64+8);
			break;
		case 0x0F:
			BTIC4B_EncEmitCommand(ctx, 0x0F);
			BTIC4B_FastEncAD(ctx, blkb[2], blkb[3]);
			BTIC4B_EncEmit256B(ctx, blkb+64);
			break;

		default:
//			__asm { int 3 }
			BTIC4B_DBGTRAP
			break;
		}
	}

	if(blkb==blks)
		return(0);

	BTIC4B_EncEmitDbgCmdPfx(ctx);
	BTIC4B_EncEmitCommand(ctx, 0x40);
	
	blkb=BTIC4B_EncFlushBits(ctx);
	return(blkb-cbuf);
}
#endif

byte *BTIC4B_EncEmitUVLI(byte *ct, u64 val)
{
	if(val<0x80)
	{
		*ct++=val;
		return(ct);
	}
	if(val<0x4000)
	{
		*ct++=0x80|(val>>8);
		*ct++=val;
		return(ct);
	}
	if(val<0x200000)
	{
		*ct++=0xC0|(val>>16);
		*ct++=val>>8;		*ct++=val;
		return(ct);
	}
	if(val<0x10000000)
	{
		*ct++=0xE0|(val>>24);
		*ct++=val>>16;		*ct++=val>>8;
		*ct++=val;
		return(ct);
	}
	if(val<0x800000000ULL)
	{
		*ct++=0xF0|(val>>32);
		*ct++=val>>24;		*ct++=val>>16;
		*ct++=val>>8;		*ct++=val;
		return(ct);
	}
	if(val<0x40000000000ULL)
	{
		*ct++=0xF8|(val>>40);
		*ct++=val>>32;		*ct++=val>>24;
		*ct++=val>>16;		*ct++=val>>8;
		*ct++=val;
		return(ct);
	}
	if(val<0x2000000000000ULL)
	{
		*ct++=0xFC|(val>>48);
		*ct++=val>>40;		*ct++=val>>32;
		*ct++=val>>24;		*ct++=val>>16;
		*ct++=val>>8;		*ct++=val;
		return(ct);
	}
	if(val<0x100000000000000ULL)
	{
		*ct++=0xFE;		*ct++=val>>48;
		*ct++=val>>40;	*ct++=val>>32;
		*ct++=val>>24;	*ct++=val>>16;
		*ct++=val>>8;	*ct++=val;
		return(ct);
	}

	*ct++=0xFF;		*ct++=val>>56;
	*ct++=val>>48;	*ct++=val>>40;
	*ct++=val>>32;	*ct++=val>>24;
	*ct++=val>>16;	*ct++=val>>8;
	*ct++=val;
	return(ct);
}

BTIC4B_API byte *BTIC4B_EncodeBufEmitHeadCtx(
	BTIC4B_Context *ctx, byte *ict)
{
	byte *ct;
	int mod, bs;
	int i;

	switch(ctx->blksz)
	{
	case 32: bs=0; break;
	case 48: bs=1; break;
	case 64: bs=2; break;
	case 96: bs=3; break;
	}

	mod=ctx->imgt|(bs<<3)|(ctx->clrt<<5);

	ct=ict;
	*ct++=0;	*ct++=0;
	*ct++='H';	*ct++='X';
	ct=BTIC4B_EncEmitUVLI(ct, ctx->xs);
	ct=BTIC4B_EncEmitUVLI(ct, ctx->ys);
	ct=BTIC4B_EncEmitUVLI(ct, mod);
	ct=BTIC4B_EncEmitUVLI(ct, ctx->qfl);
	
	i=ct-ict;
	ict[0]=i>>8; ict[1]=i;
	return(ct);
}

BTIC4B_API int BTIC4B_EncodeImgBufferCtx(BTIC4B_Context *ctx,
	byte *obuf, int cbsz, byte *ibuf, int xs, int ys, int qfl, int clrs)
{
	byte *ct, *ct0, *cs1;
	int sz, sz1;
	int i, j, k, l;

	if(!ctx->blks)
	{
		ctx->xs=xs;
		ctx->ys=ys;
		ctx->xsb=(xs+7)>>3;
		ctx->ysb=(ys+7)>>3;
		ctx->nblk=ctx->xsb*ctx->ysb;
		ctx->blksz=64;
		ctx->blks=malloc(ctx->nblk*ctx->blksz+256);
		ctx->lblks=malloc(ctx->nblk*ctx->blksz+256);
	}

	ct=obuf;

	ctx->imgt=BTIC4B_IMGT_LDR8A;
	ctx->clrt=BTIC4B_CLRT_RCT;
	
	if((clrs&0x7F)==BTIC4B_CLRS_RGB11F)
		ctx->imgt=BTIC4B_IMGT_HDR12;
//		ctx->imgt=BTIC4B_IMGT_HDR16;

	if((clrs&0x7F)==BTIC4B_CLRS_RGB48F)
//		ctx->imgt=BTIC4B_IMGT_HDR12;
		ctx->imgt=BTIC4B_IMGT_HDR16;

	if((clrs&0x7F)==BTIC4B_CLRS_RGB8E8)
		ctx->imgt=BTIC4B_IMGT_HDR16;
	
	BTIC4B_SetupContextQf(ctx, qfl);
//	BTIC4B_EncImageBGRA(ctx, ctx->blks, ibuf, xs, ys);
	BTIC4B_EncImageClrs(ctx, ctx->blks, ibuf, xs, ys, clrs);
	
	if(!(qfl&BTIC4B_QFL_PFRAME))
		ct=BTIC4B_EncodeBufEmitHeadCtx(ctx, ct);
	
	ct0=ct+16;
	sz1=BTIC4B_EncImgBlocks(ctx,
		ct0, ctx->blks,
		(qfl&BTIC4B_QFL_PFRAME)?ctx->lblks:NULL,
		xs, ys, qfl);
	
	if(sz1>=(8192-4))
	{
		i=sz1+6;
		*ct++=0x40+(i>>24);	*ct++=(i>>16);
		*ct++=(i>> 8);		*ct++=(i    );
		*ct++='I';			*ct++='0';
	}else
	{
		i=sz1+4;
		*ct++=0x00+(i>>8);	*ct++=(i    );
		*ct++='I';			*ct++='0';
	}
	memmove(ct, ct0, sz1);
	ct+=sz1;

#if 1
	if((ctx->imgt==BTIC4B_IMGT_LDR8A) ||
		(ctx->imgt==BTIC4B_IMGT_HDR16A))
	{
		ct0=ct+16;
		sz1=BTIC4B_EncImgAlphaBlocks(ctx,
			ct0, ctx->blks,
			(qfl&BTIC4B_QFL_PFRAME)?ctx->lblks:NULL,
			xs, ys, qfl);
		if(sz1>0)
		{
			if(sz1>=(8192-4))
			{
				i=sz1+6;
				*ct++=0x40+(i>>24);	*ct++=(i>>16);
				*ct++=(i>> 8);		*ct++=(i    );
				*ct++='A';			*ct++='X';
			}else
			{
				i=sz1+4;
				*ct++=0x00+(i>>8);	*ct++=(i    );
				*ct++='A';			*ct++='X';
			}
			memmove(ct, ct0, sz1);
			ct+=sz1;
		}
	}
#endif

	cs1=ctx->blks;
	ctx->blks=ctx->lblks;
	ctx->lblks=cs1;

	sz=ct-obuf;
	return(sz);
}


BTIC4B_API int BTIC4B_DumpStatsCtx(BTIC4B_Context *ctx)
{
	int i, j, k;
	printf("rk_cy=%d rk_cuv=%d rk_dy=%d rk_duv=%d\n",
		ctx->rk_cy, ctx->rk_cuv, ctx->rk_dy, ctx->rk_duv);
	printf("rk_cnt=%d rk_misc=%d\n",
		ctx->rk_cnt, ctx->rk_misc);

	printf("stat_yuvbits=%d/%d (Ab=%.2f, Tb=%.2f%%)\n",
		ctx->stat_yuvbits, ctx->stat_yuvcnt, 
		(ctx->stat_yuvbits/(ctx->stat_yuvcnt+1.0)),
		(100.0*ctx->stat_yuvbits)/(ctx->stat_bits+1.0));

	printf("  Cy=%d/%d (Ab=%.2f, Tb=%.2f%%)\n",
		ctx->stat_yuvcbits[0], ctx->stat_yuvcnt, 
		(ctx->stat_yuvcbits[0]/(ctx->stat_yuvcnt+1.0)),
		(100.0*ctx->stat_yuvcbits[0])/(ctx->stat_bits+1.0));
	printf("  Cu=%d/%d (Ab=%.2f, Tb=%.2f%%)\n",
		ctx->stat_yuvcbits[1], ctx->stat_yuvcnt, 
		(ctx->stat_yuvcbits[1]/(ctx->stat_yuvcnt+1.0)),
		(100.0*ctx->stat_yuvcbits[1])/(ctx->stat_bits+1.0));
	printf("  Cv=%d/%d (Ab=%.2f, Tb=%.2f%%)\n",
		ctx->stat_yuvcbits[2], ctx->stat_yuvcnt, 
		(ctx->stat_yuvcbits[2]/(ctx->stat_yuvcnt+1.0)),
		(100.0*ctx->stat_yuvcbits[2])/(ctx->stat_bits+1.0));
	printf("  Dy=%d/%d (Ab=%.2f, Tb=%.2f%%)\n",
		ctx->stat_yuvcbits[3], ctx->stat_yuvcnt, 
		(ctx->stat_yuvcbits[3]/(ctx->stat_yuvcnt+1.0)),
		(100.0*ctx->stat_yuvcbits[3])/(ctx->stat_bits+1.0));
	printf("  Du=%d/%d (Ab=%.2f, Tb=%.2f%%)\n",
		ctx->stat_yuvcbits[4], ctx->stat_yuvcnt, 
		(ctx->stat_yuvcbits[4]/(ctx->stat_yuvcnt+1.0)),
		(100.0*ctx->stat_yuvcbits[4])/(ctx->stat_bits+1.0));
	printf("  Dv=%d/%d (Ab=%.2f, Tb=%.2f%%)\n",
		ctx->stat_yuvcbits[5], ctx->stat_yuvcnt, 
		(ctx->stat_yuvcbits[5]/(ctx->stat_yuvcnt+1.0)),
		(100.0*ctx->stat_yuvcbits[5])/(ctx->stat_bits+1.0));
	printf("sm_mask.rk=%d sm_mask.rov=%d sm_mask.bits=%d/%d "
			"(Ab=%.2f, Tb=%.2f%%)\n",
		ctx->sm_mask.rk, ctx->sm_mask.rov,
		ctx->sm_mask.bits, ctx->sm_mask.cnt,
		ctx->sm_mask.bits/(ctx->sm_mask.cnt+1.0),
		(100.0*ctx->sm_mask.bits)/(ctx->stat_bits+1.0));

	printf("yuv>Z: Cy=%d(%.2f%%) Cu=%d(%.2f%%) Cv=%d(%.2f%%)\n"
			"    Dy=%d(%.2f%%) Du=%d(%.2f%%) Dv=%d(%.2f%%)\n",
		ctx->yuv_cz[0], (100.0*ctx->yuv_cz[0])/(ctx->stat_yuvcnt+1.0),
		ctx->yuv_cz[1], (100.0*ctx->yuv_cz[1])/(ctx->stat_yuvcnt+1.0),
		ctx->yuv_cz[2], (100.0*ctx->yuv_cz[2])/(ctx->stat_yuvcnt+1.0),
		ctx->yuv_cz[3], (100.0*ctx->yuv_cz[3])/(ctx->stat_yuvcnt+1.0),
		ctx->yuv_cz[4], (100.0*ctx->yuv_cz[4])/(ctx->stat_yuvcnt+1.0),
		ctx->yuv_cz[5], (100.0*ctx->yuv_cz[5])/(ctx->stat_yuvcnt+1.0));

	printf("sm_cmd.rk=%d sm_cmd.rov=%d sm_cmd.bits=%d/%d "
			"(Ab=%.2f, Tb=%.2f%%)\n",
		ctx->sm_cmd.rk, ctx->sm_cmd.rov,
		ctx->sm_cmd.bits, ctx->sm_cmd.cnt,
		ctx->sm_cmd.bits/(ctx->sm_cmd.cnt+1.0),
		(100.0*ctx->sm_cmd.bits)/(ctx->stat_bits+1.0));
	for(i=0; i<4; i++)
	{
		printf("    ");
		for(j=0; j<16; j++)
		{
			k=(byte)(ctx->sm_cmd.rov+(i*16+j));
			printf("%02X ", ctx->sm_cmd.tab[k]);
		}
		printf("\n");
	}
	
	printf("pixel bits=%d (%.2f%%)\n",
		ctx->stat_pixbits,
		(100.0*ctx->stat_pixbits)/(ctx->stat_bits+1.0));

	return(0);
}

BTIC4B_API int BTIC4B_EncFlattenBmp(byte *obuf,
	int xs, int ys, int fcc,
	byte *ibuf, int isz)
{
	byte hbuf[256];
	int sz1, sz2, sz3, dpm;
	
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
	
	memcpy(obuf, hbuf, sz2);
	memmove(obuf+sz2, ibuf, isz);
	return(sz2+isz);
}

BTIC4B_API int BTIC4B_EncodeImgBmpBufferCtx(BTIC4B_Context *ctx,
	byte *obuf, int cbsz, byte *ibuf, int xs, int ys, int qfl, int clrs)
{
	byte *tbuf;
	int tsz;
	int sz;

	tbuf=obuf+256;

	tsz=BTIC4B_EncodeImgBufferCtx(ctx,
		tbuf, cbsz-256, ibuf, xs, ys, qfl, clrs);
	sz=BTIC4B_EncFlattenBmp(obuf, xs, ys, BTIC4B_FCC_BT4B, tbuf, tsz);
	return(sz);
}

BTIC4B_API BTIC4B_Context *BTIC4B_AllocContext(void)
{
	BTIC4B_Context *ctx;

	ctx=malloc(sizeof(BTIC4B_Context));
	memset(ctx, 0, sizeof(BTIC4B_Context));
	return(ctx);
}

BTIC4B_API void BTIC4B_FreeContext(BTIC4B_Context *ctx)
{
	if(ctx->blks)
		free(ctx->blks);
	if(ctx->lblks)
		free(ctx->lblks);
	if(ctx->pblk)
		free(ctx->pblk);
	free(ctx);
}

BTIC4B_API int BTIC4B_EncodeImgBmpBuffer(
	byte *obuf, int cbsz, byte *ibuf, int xs, int ys, int qfl, int clrs)
{
	BTIC4B_Context *ctx;
	int i;
	
	ctx=BTIC4B_AllocContext();
	i=BTIC4B_EncodeImgBmpBufferCtx(ctx, obuf, cbsz,
		ibuf, xs, ys, qfl, clrs);
	BTIC4B_FreeContext(ctx);
	return(i);
}
