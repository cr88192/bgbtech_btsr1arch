// #define BTIC4B_BYTES

// #ifdef BTIC4B_BYTES
#if 0
int BTIC4B_DecodeUVLI(BTIC4B_Context *ctx)
{
	int i;

	i=*ctx->cs++;
	if(i<0x80)
	{
		return(i);
	}else if(i<0xC0)
	{
		i=(i&0x7F<<8)|(*ctx->cs++);
		return(i);
	}else if(i<0xE0)
	{
		i=(i&0x3F<<8)|(*ctx->cs++);
		i=(i<<8)|(*ctx->cs++);
		return(i);
	}else if(i<0xF0)
	{
		i=(i&0x1F<<8)|(*ctx->cs++);
		i=(i<<8)|(*ctx->cs++);
		i=(i<<8)|(*ctx->cs++);
		return(i);
	}else if(i<0xF0)
	{
		i=(i&0x0F<<8)|(*ctx->cs++);
		i=(i<<8)|(*ctx->cs++);
		i=(i<<8)|(*ctx->cs++);
		i=(i<<8)|(*ctx->cs++);
		return(i);
	}
}

int BTIC4B_DecReadCountVal(BTIC4B_Context *ctx)
{
	return(BTIC4B_DecodeUVLI(ctx));
}

int BTIC4B_DecReadGenericUVal(BTIC4B_Context *ctx)
{
	return(BTIC4B_DecodeUVLI(ctx));
}

int BTIC4B_DecReadGenericVal(BTIC4B_Context *ctx)
{
	int v;
	v=BTIC4B_DecodeUVLI(ctx);
	v=(v>>1)^((v<<31)>>31);
	return(v);
}

int BTIC4B_DecReadCommand(BTIC4B_Context *ctx)
{
	return(*ctx->cs++);
}

void BTIC4B_DecColorYUV(BTIC4B_Context *ctx)
{
	int dcy, dcu, dcv;
	int i;
	
	if((*ctx->cs)&1)
	{
		i=*ctx->cs++;
		i|=(*ctx->cs++)<<8;
		dcy=(i>>1)&31;
		dcu=(i>>6)&31;
		dcv=(i>>11)&31;
	}else
	{
		i=BTIC4B_DecodeUVLI(ctx);
		dcy=0;	dcu=0;	dcv=0;
		if(i&0x02)dcy=BTIC4B_DecodeUVLI(ctx);
		if(i&0x04)dcu=BTIC4B_DecodeUVLI(ctx);
		if(i&0x08)dcv=BTIC4B_DecodeUVLI(ctx);
	}

	dcy=(dcy>>1)^((dcy<<31)>>31);
	dcu=(dcu>>1)^((dcu<<31)>>31);
	dcv=(dcv>>1)^((dcv<<31)>>31);

	ctx->cy+=dcy*ctx->qy;
	ctx->cu+=dcu*ctx->quv;
	ctx->cv+=dcv*ctx->quv;
//	ctx->dy+=ddy*ctx->qdy;
}

void BTIC4B_DecColorYUVD(BTIC4B_Context *ctx)
{
	int dcy, dcu, dcv, ddy, ddu, ddv;
	int i;
	
	if((*ctx->cs)&1)
	{
		i=*ctx->cs++;
		i|=(*ctx->cs++)<<8;
		i|=(*ctx->cs++)<<16;
		dcy=(i>>1)&31;
		dcu=(i>>6)&31;
		dcv=(i>>11)&31;
		ddy=(i>>16)&31;
		if((i>>21)&1)
		{
			i|=(*ctx->cs++)<<24;
			ddu=(i>>22)&31;
			ddv=(i>>27)&31;
		}else
		{
			ddu=0;
			ddv=0;
		}
	}else
	{
		i=BTIC4B_DecodeUVLI(ctx);
		dcy=0;	dcu=0;	dcv=0;
		ddy=0;	ddu=0;	ddv=0;
		if(i&0x02)dcy=BTIC4B_DecodeUVLI(ctx);
		if(i&0x04)dcu=BTIC4B_DecodeUVLI(ctx);
		if(i&0x08)dcv=BTIC4B_DecodeUVLI(ctx);
		if(i&0x10)ddy=BTIC4B_DecodeUVLI(ctx);
		if(i&0x20)ddu=BTIC4B_DecodeUVLI(ctx);
		if(i&0x40)ddv=BTIC4B_DecodeUVLI(ctx);
	}

	dcy=(dcy>>1)^((dcy<<31)>>31);
	dcu=(dcu>>1)^((dcu<<31)>>31);
	dcv=(dcv>>1)^((dcv<<31)>>31);
	ddy=(ddy>>1)^((ddy<<31)>>31);
	ddu=(ddu>>1)^((ddu<<31)>>31);
	ddv=(ddv>>1)^((ddv<<31)>>31);

	ctx->cy+=dcy*ctx->qy;
	ctx->cu+=dcu*ctx->quv;
	ctx->cv+=dcv*ctx->quv;
	ctx->dy+=ddy*ctx->qdy;
	ctx->du+=ddu*ctx->qduv;
	ctx->dv+=ddv*ctx->qduv;
}

void BTIC4B_DecColorYUVDyuv(BTIC4B_Context *ctx)
{
	int dcy, dcu, dcv, ddy, ddu, ddv;
	int i;
	
	if((*ctx->cs)&1)
	{
		i=*ctx->cs++;
		i|=(*ctx->cs++)<<8;
		i|=(*ctx->cs++)<<16;
		dcy=(i>>1)&31;
		dcu=(i>>6)&31;
		dcv=(i>>11)&31;
		ddy=(i>>16)&31;
		if((i>>21)&1)
		{
			i|=(*ctx->cs++)<<24;
			ddu=(i>>22)&31;
			ddv=(i>>27)&31;
		}else
		{
			ddu=0;
			ddv=0;
		}
	}else
	{
		i=BTIC4B_DecodeUVLI(ctx);
		dcy=0;	dcu=0;
		dcv=0;	ddy=0;
		ddu=0;	ddv=0;
		if(i&0x02)dcy=BTIC4B_DecodeUVLI(ctx);
		if(i&0x04)dcu=BTIC4B_DecodeUVLI(ctx);
		if(i&0x08)dcv=BTIC4B_DecodeUVLI(ctx);
		if(i&0x10)ddy=BTIC4B_DecodeUVLI(ctx);
		if(i&0x20)ddu=BTIC4B_DecodeUVLI(ctx);
		if(i&0x40)ddv=BTIC4B_DecodeUVLI(ctx);
	}

	dcy=(dcy>>1)^((dcy<<31)>>31);
	dcu=(dcu>>1)^((dcu<<31)>>31);
	dcv=(dcv>>1)^((dcv<<31)>>31);
	ddy=(ddy>>1)^((ddy<<31)>>31);
	ddu=(ddu>>1)^((ddu<<31)>>31);
	ddv=(ddv>>1)^((ddv<<31)>>31);

	ctx->cy+=dcy*ctx->qy;
	ctx->cu+=dcu*ctx->quv;
	ctx->cv+=dcv*ctx->quv;
	ctx->dy+=ddy*ctx->qdy;
	ctx->du+=ddu*ctx->qduv;
	ctx->dv+=ddv*ctx->qduv;
}

force_inline void BTIC4B_DecRead8B(BTIC4B_Context *ctx, byte *buf)
{
	*buf=*ctx->cs++;
}

force_inline void BTIC4B_DecRead16B(BTIC4B_Context *ctx, byte *buf)
{
	*(u16 *)buf=*(u16 *)ctx->cs;
	ctx->cs+=2;
}

force_inline void BTIC4B_DecRead32B(BTIC4B_Context *ctx, byte *buf)
{
	*(u32 *)buf=*(u32 *)ctx->cs;
	ctx->cs+=4;
}

force_inline void BTIC4B_DecRead64B(BTIC4B_Context *ctx, byte *buf)
{
	*(u64 *)buf=*(u64 *)ctx->cs;
	ctx->cs+=8;
}

force_inline void BTIC4B_DecRead96B(BTIC4B_Context *ctx, byte *buf)
{
	*(u64 *)(buf+0)=*(u64 *)(ctx->cs+0);
	*(u32 *)(buf+8)=*(u32 *)(ctx->cs+8);
	ctx->cs+=12;
}

force_inline void BTIC4B_DecRead128B(BTIC4B_Context *ctx, byte *buf)
{
	*(u64 *)(buf+0)=*(u64 *)(ctx->cs+0);
	*(u64 *)(buf+8)=*(u64 *)(ctx->cs+8);
	ctx->cs+=16;
}

force_inline void BTIC4B_DecRead192B(BTIC4B_Context *ctx, byte *buf)
{
	*(u64 *)(buf+ 0)=*(u64 *)(ctx->cs+ 0);
	*(u64 *)(buf+ 8)=*(u64 *)(ctx->cs+ 8);
	*(u64 *)(buf+16)=*(u64 *)(ctx->cs+16);
	ctx->cs+=24;
}

force_inline void BTIC4B_DecRead256B(BTIC4B_Context *ctx, byte *buf)
{
	*(u64 *)(buf+ 0)=*(u64 *)(ctx->cs+ 0);
	*(u64 *)(buf+ 8)=*(u64 *)(ctx->cs+ 8);
	*(u64 *)(buf+16)=*(u64 *)(ctx->cs+16);
	*(u64 *)(buf+24)=*(u64 *)(ctx->cs+24);
	ctx->cs+=32;
}

#else
int BTIC4B_DecReadCommand(BTIC4B_Context *ctx)
{
	int i;
	i=BTIC4B_ReadSymbolSmtf(ctx, &(ctx->sm_cmd));
	return(i);
}

int BTIC4B_DecReadMask(BTIC4B_Context *ctx)
{
	int i, j;
	i=BTIC4B_ReadAdRiceLL(ctx, &(ctx->sm_mask.rk));
	if(!i)
		return(ctx->cmask);
	j=BTIC4B_DecodeSymbolIndexSmtf(ctx, &(ctx->sm_mask), i-1);
	if(j&1)ctx->cmask=j;
	return(j);
}

int BTIC4B_DecReadGenericUVal(BTIC4B_Context *ctx)
{
	int i;
	i=BTIC4B_ReadAdRiceLL(ctx, &(ctx->rk_misc));
	return(i);
}

int BTIC4B_DecReadGenericVal(BTIC4B_Context *ctx)
{
	int i;
	i=BTIC4B_ReadAdSRiceLL(ctx, &(ctx->rk_misc));
	return(i);
}

int BTIC4B_DecReadCountVal(BTIC4B_Context *ctx)
	{ return(BTIC4B_ReadAdRiceLL(ctx, &(ctx->rk_cnt))); }

int BTIC4B_DecReadValCy(BTIC4B_Context *ctx)
	{ return(BTIC4B_ReadAdSRiceLL(ctx, &(ctx->rk_cy))); }
int BTIC4B_DecReadValCuv(BTIC4B_Context *ctx)
	{ return(BTIC4B_ReadAdSRiceLL(ctx, &(ctx->rk_cuv))); }
int BTIC4B_DecReadValDy(BTIC4B_Context *ctx)
	{ return(BTIC4B_ReadAdSRiceLL(ctx, &(ctx->rk_dy))); }
int BTIC4B_DecReadValDuv(BTIC4B_Context *ctx)
	{ return(BTIC4B_ReadAdSRiceLL(ctx, &(ctx->rk_duv))); }

void BTIC4B_DecColorYUV(BTIC4B_Context *ctx)
{
	int dcy, dcu, dcv;
	int m;
	int i;
	
	if(ctx->cmask)
	{
		m=BTIC4B_DecReadMask(ctx);	
		dcy=0;	dcu=0;	dcv=0;
		if(m&0x02)dcy=BTIC4B_DecReadValCy(ctx);
		if(m&0x04)dcu=BTIC4B_DecReadValCuv(ctx);
		if(m&0x08)dcv=BTIC4B_DecReadValCuv(ctx);
	}else
	{
		dcy=BTIC4B_DecReadValCy(ctx);
		dcu=BTIC4B_DecReadValCuv(ctx);
		dcv=BTIC4B_DecReadValCuv(ctx);
	}

	ctx->cy+=dcy*ctx->qy;
	ctx->cu+=dcu*ctx->quv;
	ctx->cv+=dcv*ctx->quv;
}

void BTIC4B_DecColorYUVD(BTIC4B_Context *ctx)
{
	int dcy, dcu, dcv, ddy, ddu, ddv;
	int i;
	
	if(ctx->cmask)
	{
		i=BTIC4B_DecReadMask(ctx);
		dcy=0;	dcu=0;	dcv=0;
		ddy=0;	ddu=0;	ddv=0;

		if(i&0x02)dcy=BTIC4B_DecReadValCy(ctx);
		if(i&0x04)dcu=BTIC4B_DecReadValCuv(ctx);
		if(i&0x08)dcv=BTIC4B_DecReadValCuv(ctx);
		if(i&0x10)ddy=BTIC4B_DecReadValDy(ctx);
		
		if(!(i&0x80))
//		if(0)
		{
			if(i&0x20)ddu=BTIC4B_DecReadValDuv(ctx);
			if(i&0x40)ddv=BTIC4B_DecReadValDuv(ctx);
		}
	}else
	{
		dcy=BTIC4B_DecReadValCy(ctx);
		dcu=BTIC4B_DecReadValCuv(ctx);
		dcv=BTIC4B_DecReadValCuv(ctx);
		ddy=BTIC4B_DecReadValDy(ctx);
		ddu=0;		ddv=0;
	}

	ctx->cy+=dcy*ctx->qy;
	ctx->cu+=dcu*ctx->quv;
	ctx->cv+=dcv*ctx->quv;
	ctx->dy+=ddy*ctx->qdy;
	ctx->du+=ddu*ctx->qduv;
	ctx->dv+=ddv*ctx->qduv;
}

void BTIC4B_DecColorYUVDyuv(BTIC4B_Context *ctx)
{
	int dcy, dcu, dcv, ddy, ddu, ddv;
	int i;
	
	if(ctx->cmask)
	{
		i=BTIC4B_DecReadMask(ctx);
		dcy=0;	dcu=0;
		dcv=0;	ddy=0;
		ddu=0;	ddv=0;
		if(i&0x02)dcy=BTIC4B_DecReadValCy(ctx);
		if(i&0x04)dcu=BTIC4B_DecReadValCuv(ctx);
		if(i&0x08)dcv=BTIC4B_DecReadValCuv(ctx);
		if(i&0x10)ddy=BTIC4B_DecReadValDy(ctx);
		if(i&0x20)ddu=BTIC4B_DecReadValDuv(ctx);
		if(i&0x40)ddv=BTIC4B_DecReadValDuv(ctx);
	}else
	{
		dcy=BTIC4B_DecReadValCy(ctx);
		dcu=BTIC4B_DecReadValCuv(ctx);
		dcv=BTIC4B_DecReadValCuv(ctx);
		ddy=BTIC4B_DecReadValDy(ctx);
		ddu=BTIC4B_DecReadValDuv(ctx);
		ddv=BTIC4B_DecReadValDuv(ctx);
	}

	ctx->cy+=dcy*ctx->qy;
	ctx->cu+=dcu*ctx->quv;
	ctx->cv+=dcv*ctx->quv;
	ctx->dy+=ddy*ctx->qdy;
	ctx->du+=ddu*ctx->qduv;
	ctx->dv+=ddv*ctx->qduv;
}

void BTIC4B_DecColorY(BTIC4B_Context *ctx)
{
	int dcy;
	int i;
	
	if(ctx->cmask)
	{
		i=BTIC4B_DecReadMask(ctx);
		dcy=0;
		if(i&0x02)dcy=BTIC4B_DecReadValCy(ctx);
	}else
	{
		dcy=BTIC4B_DecReadValCy(ctx);
	}

	ctx->cy+=dcy*ctx->qy;
}

void BTIC4B_DecColorYD(BTIC4B_Context *ctx)
{
	int dcy, ddy;
	int i;
	
	if(ctx->cmask)
	{
		i=BTIC4B_DecReadMask(ctx);
		dcy=0;	ddy=0;

		if(i&0x02)dcy=BTIC4B_DecReadValCy(ctx);
		if(i&0x10)ddy=BTIC4B_DecReadValDy(ctx);
	}else
	{
		dcy=BTIC4B_DecReadValCy(ctx);
		ddy=BTIC4B_DecReadValDy(ctx);
	}

	ctx->cy+=dcy*ctx->qy;
	ctx->dy+=ddy*ctx->qdy;
}


force_inline void BTIC4B_DecRead8B(BTIC4B_Context *ctx, byte *buf)
{
	*buf=BTIC4B_Read8BitsNM(ctx);
}

force_inline void BTIC4B_DecRead16B(BTIC4B_Context *ctx, byte *buf)
{
	*(u16 *)buf=BTIC4B_Read16BitsNM(ctx);
}

force_inline void BTIC4B_DecRead32B(BTIC4B_Context *ctx, byte *buf)
{
	*(u32 *)buf=BTIC4B_Read32Bits(ctx);
}

force_inline void BTIC4B_DecRead48B(BTIC4B_Context *ctx, byte *buf)
{
	((u32 *)buf)[0]=BTIC4B_Read32Bits(ctx);
	((u16 *)buf)[2]=BTIC4B_Read16BitsNM(ctx);
}

force_inline void BTIC4B_DecRead64B(BTIC4B_Context *ctx, byte *buf)
{
	((u64 *)buf)[0]=BTIC4B_Read64Bits(ctx);
}

force_inline void BTIC4B_DecRead96B(BTIC4B_Context *ctx, byte *buf)
{
	((u64 *)buf)[0]=BTIC4B_Read64Bits(ctx);
	((u32 *)buf)[2]=BTIC4B_Read32Bits(ctx);
}

force_inline void BTIC4B_DecRead128B(BTIC4B_Context *ctx, byte *buf)
{
	((u64 *)buf)[0]=BTIC4B_Read64Bits(ctx);
	((u64 *)buf)[1]=BTIC4B_Read64Bits(ctx);
}

force_inline void BTIC4B_DecRead192B(BTIC4B_Context *ctx, byte *buf)
{
	((u64 *)buf)[0]=BTIC4B_Read64Bits(ctx);
	((u64 *)buf)[1]=BTIC4B_Read64Bits(ctx);
	((u64 *)buf)[2]=BTIC4B_Read64Bits(ctx);
}

force_inline void BTIC4B_DecRead256B(BTIC4B_Context *ctx, byte *buf)
{
	((u64 *)buf)[0]=BTIC4B_Read64Bits(ctx);
	((u64 *)buf)[1]=BTIC4B_Read64Bits(ctx);
	((u64 *)buf)[2]=BTIC4B_Read64Bits(ctx);
	((u64 *)buf)[3]=BTIC4B_Read64Bits(ctx);
}

#endif

void BTIC4B_FillBlockHeadTag(BTIC4B_Context *ctx, byte *blk, int tag)
{
	int cy, cu, cv, dy, du, dv, m;
//	*(u16 *)(blk+ 0)=tag;
//	*(u16 *)(blk+ 2)=0x00FF;

	if(ctx->cmask && !(ctx->cmask&0x80))
		tag|=0x40;

	*(u32 *)(blk+ 0)=tag|0x00FF0000;

//	*(u16 *)(blk+ 4)=lqtvq_clamp65535(ctx->cy);
//	*(u16 *)(blk+ 6)=lqtvq_clamp65535(ctx->cu);
//	*(u16 *)(blk+ 8)=lqtvq_clamp65535(ctx->cv);
//	*(u16 *)(blk+10)=lqtvq_clamp65535(ctx->dy);
//	*(u16 *)(blk+12)=lqtvq_clamp65535(ctx->du);
//	*(u16 *)(blk+14)=lqtvq_clamp65535(ctx->dv);

#if 0
	*(s16 *)(blk+ 4)=ctx->cy;
	*(s16 *)(blk+ 6)=ctx->cu;
	*(s16 *)(blk+ 8)=ctx->cv;
	*(s16 *)(blk+10)=ctx->dy;
	*(s16 *)(blk+12)=ctx->du;
	*(s16 *)(blk+14)=ctx->dv;
#endif

#if 1
	*(s16 *)(blk+ 4)=lqtvq_clamp32767S(ctx->cy);
	*(s16 *)(blk+ 6)=lqtvq_clamp32767S(ctx->cu);
	*(s16 *)(blk+ 8)=lqtvq_clamp32767S(ctx->cv);
	*(s16 *)(blk+10)=lqtvq_clamp32767S(ctx->dy);
	*(s16 *)(blk+12)=lqtvq_clamp32767S(ctx->du);
	*(s16 *)(blk+14)=lqtvq_clamp32767S(ctx->dv);
#endif

#if 1
	*(s32 *)(blk+16)=0;
	*(s32 *)(blk+20)=0;
	*(s32 *)(blk+24)=0;
	*(s32 *)(blk+28)=0;
#endif

#if 0
	cy=ctx->cy;	cu=ctx->cu;	cv=ctx->cv;
	dy=ctx->dy;	du=ctx->du;	dv=ctx->dv;

//	m=	(cy^(cy>>31))|(cu^(cu>>31))|(cv^(cv>>31))|
//		(dy^(dy>>31))|(du^(du>>31))|(dv^(dv>>31));

	m=	(cy-((s16)cy))|(cu-((s16)cu))|(cv-((s16)cv))|
		(dy-((s16)dy))|(du-((s16)du))|(dv-((s16)dv));

//	if(m>>16)
	if(m)
	{
		cy=lqtvq_clamp32767S(cy);
		cu=lqtvq_clamp32767S(cu);
		cv=lqtvq_clamp32767S(cv);
		dy=lqtvq_clamp32767S(dy);
		du=lqtvq_clamp32767S(du);
		dv=lqtvq_clamp32767S(dv);
	}

	*(s16 *)(blk+ 4)=cy;
	*(s16 *)(blk+ 6)=cu;
	*(s16 *)(blk+ 8)=cv;
	*(s16 *)(blk+10)=dy;
	*(s16 *)(blk+12)=du;
	*(s16 *)(blk+14)=dv;
#endif
}

void BTIC4B_FillBlockHeadL8(BTIC4B_Context *ctx, byte *blk, int tag)
{
	if(ctx->cmask && !(ctx->cmask&0x80))
		tag|=0x40;

	*(u16 *)(blk+ 0)=tag;
	*(blk+2)=lqtvq_clamp255(ctx->cy);
//	*(blk+3)=lqtvq_clamp255(ctx->cu);
//	*(blk+4)=lqtvq_clamp255(ctx->cv);
	*(blk+5)=lqtvq_clamp255(ctx->dy);
//	*(blk+6)=lqtvq_clamp255(ctx->du);
//	*(blk+7)=lqtvq_clamp255(ctx->dv);

	*(blk+3)=lqtvq_clamp255((ctx->cu>>1)+128);
	*(blk+4)=lqtvq_clamp255((ctx->cv>>1)+128);
//	*(blk+6)=lqtvq_clamp255(ctx->du>>1);
//	*(blk+7)=lqtvq_clamp255(ctx->dv>>1);
	*(blk+6)=lqtvq_clamp255((ctx->du>>1)+128);
	*(blk+7)=lqtvq_clamp255((ctx->dv>>1)+128);
}


void BTIC4B_FillBlockHeadAlphaTag(BTIC4B_Context *ctx, byte *blk, int tag)
{
	if((blk[0]&0x1F)==0x19)
	{
		BTIC4B_DBGTRAP
		return;
	}

	blk[1]=tag;
	blk[2]=lqtvq_clamp255(ctx->cy);
	blk[3]=lqtvq_clamp255(ctx->dy);
	*(s16 *)(blk+48+0)=lqtvq_clamp32767S(ctx->cy);
	*(s16 *)(blk+48+2)=lqtvq_clamp32767S(ctx->dy);
}

void BTIC4B_FillBlockHeadAlphaTagL8(BTIC4B_Context *ctx,
	byte *blk, int tag)
{
	if((blk[0]&0x1F)==0x19)
	{
		BTIC4B_DBGTRAP
		return;
	}

	blk[1]=tag;
	blk[2]=lqtvq_clamp255(ctx->cy);
	blk[3]=lqtvq_clamp255(ctx->dy);
}

void BTIC4B_DecodeReadBytes(BTIC4B_Context *ctx, byte *blk, int len)
{
	memcpy(blk, ctx->cs, len);
	ctx->cs+=len;
}

void BTIC4B_DecodeCopyBlocks(BTIC4B_Context *ctx,
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

void BTIC4B_DecodeCopyAlphaBlocks(BTIC4B_Context *ctx,
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
		memcpy(ct+48, cs+48, 16);
		ct+=ctx->blksz; cs+=ctx->blksz;
	}
}

void BTIC4B_DecodeSetPredictor(BTIC4B_Context *ctx, int pred);
void BTIC4B_DecodeDirtyL8(BTIC4B_Context *ctx);

void BTIC4B_DecodeSetParm(BTIC4B_Context *ctx, int var, int val)
{
	if(var==-2)
		{ BTIC4B_DecodeSetPredictor(ctx, val); }
}

void BTIC4B_DecodeEnableFeature(BTIC4B_Context *ctx, int var)
{
	if(var==-1)
	{
		ctx->cmask=0xFF;
		return;
	}
	
	if(var==-3)
	{
		BTIC4B_Grad16_Init();
		ctx->usegrad=1;
		return;
	}
}

void BTIC4B_DecodeDisableFeature(BTIC4B_Context *ctx, int var)
{
	if(var==-1)
	{
		ctx->cmask=0x00;
		return;
	}
	
	if(var==-3)
	{
		ctx->usegrad=0;
		return;
	}
}

byte *BTIC4B_DecSetupDecBlockInner(BTIC4B_Context *ctx, byte *ct, int op)
{
	int i;
//	byte *ct;

	switch(op)
	{
	case 0x00:
		BTIC4B_FillBlockHeadTag(ctx, ct, 0);
		break;

	case 0x01:
		BTIC4B_FillBlockHeadTag(ctx, ct, op);
		*(ct+16)=BTIC4B_ReadNBits(ctx, 4);
		break;
	case 0x02:
	case 0x03:
		BTIC4B_FillBlockHeadTag(ctx, ct, op);
		BTIC4B_DecRead8B(ctx, ct+16);
		break;
	case 0x04:
		BTIC4B_FillBlockHeadTag(ctx, ct, op);
		BTIC4B_DecRead16B(ctx, ct+16);
		break;
	case 0x05:
	case 0x06:
		BTIC4B_FillBlockHeadTag(ctx, ct, op);
		BTIC4B_DecRead32B(ctx, ct+16);
		break;
	case 0x07:
		BTIC4B_FillBlockHeadTag(ctx, ct, op);
		BTIC4B_DecRead64B(ctx, ct+16);
		break;

	case 0x08:
		BTIC4B_FillBlockHeadTag(ctx, ct, op);
		if(ctx->usegrad)
		{
			i=BTIC4B_ReadNBits(ctx, 5);
			if(i&1)
				{ i=(i>>2)|(BTIC4B_ReadNBits(ctx, 13)<<3); }
			else
				{ i>>=1; }
			*(u32 *)(ct+16)=i;
			break;
		}
//		*(ct+16)=BTIC4B_ReadNBits(ctx, 4);
		*(u32 *)(ct+16)=BTIC4B_ReadNBits(ctx, 4);
		break;

	case 0x09:
		BTIC4B_FillBlockHeadTag(ctx, ct, op);
		BTIC4B_DecRead8B(ctx, ct+16);
		break;
	case 0x0A:	case 0x0B:
		BTIC4B_FillBlockHeadTag(ctx, ct, op);
		BTIC4B_DecRead16B(ctx, ct+16);
		break;
	case 0x0C:
		BTIC4B_FillBlockHeadTag(ctx, ct, op);
		BTIC4B_DecRead32B(ctx, ct+16);
		break;
	case 0x0D:	case 0x0E:
		BTIC4B_FillBlockHeadTag(ctx, ct, op);
		BTIC4B_DecRead64B(ctx, ct+16);
		break;
	case 0x0F:
		BTIC4B_FillBlockHeadTag(ctx, ct, op);
		BTIC4B_DecRead128B(ctx, ct+16);
		break;
	case 0x10:
		BTIC4B_FillBlockHeadTag(ctx, ct, op);
		BTIC4B_DecRead192B(ctx, ct+16);
		break;
	case 0x11:
		BTIC4B_FillBlockHeadTag(ctx, ct, op);
		BTIC4B_DecRead256B(ctx, ct+16);
		break;

	case 0x13:
		BTIC4B_DecodeDirtyL8(ctx);
		BTIC4B_FillBlockHeadL8(ctx, ct, op);
		BTIC4B_DecRead128B(ctx, ct+ 8);
		BTIC4B_DecRead32B(ctx, ct+24);
		BTIC4B_DecRead32B(ctx, ct+28);
		break;
	case 0x14:
		BTIC4B_FillBlockHeadTag(ctx, ct, op);
		BTIC4B_DecRead16B(ctx, ct+16);
		break;
	case 0x15:
		BTIC4B_FillBlockHeadTag(ctx, ct, op);
		BTIC4B_DecRead32B(ctx, ct+16);
		BTIC4B_DecRead8B(ctx, ct+20);
		BTIC4B_DecRead8B(ctx, ct+21);
		break;
	case 0x16:
		BTIC4B_FillBlockHeadTag(ctx, ct, op);
		BTIC4B_DecRead32B(ctx, ct+16);
		BTIC4B_DecRead32B(ctx, ct+20);
		BTIC4B_DecRead32B(ctx, ct+24);
		break;
	case 0x17:
		BTIC4B_FillBlockHeadTag(ctx, ct, op);
		BTIC4B_DecRead128B(ctx, ct+16);
		BTIC4B_DecRead8B(ctx, ct+32);
		BTIC4B_DecRead8B(ctx, ct+33);
		break;
	case 0x18:
		BTIC4B_FillBlockHeadTag(ctx, ct, op);
		BTIC4B_DecRead128B(ctx, ct+16);
		BTIC4B_DecRead32B(ctx, ct+32);
		BTIC4B_DecRead32B(ctx, ct+36);
		break;
	case 0x19:
		BTIC4B_DecodeDirtyL8(ctx);
		BTIC4B_FillBlockHeadL8(ctx, ct, op);
		BTIC4B_DecRead192B(ctx, ct+ 8);
		BTIC4B_DecRead128B(ctx, ct+32);
		BTIC4B_DecRead128B(ctx, ct+48);
		break;
	case 0x1A:
		BTIC4B_FillBlockHeadTag(ctx, ct, op);
		BTIC4B_DecRead128B(ctx, ct+16);
		BTIC4B_DecRead64B(ctx, ct+32);
		BTIC4B_DecRead64B(ctx, ct+40);
		break;
	case 0x1B:
		BTIC4B_FillBlockHeadTag(ctx, ct, op);
		BTIC4B_DecRead128B(ctx, ct+16);
		BTIC4B_DecRead128B(ctx, ct+32);
		BTIC4B_DecRead128B(ctx, ct+48);
		break;
	case 0x1C:
		BTIC4B_FillBlockHeadTag(ctx, ct, op);
		BTIC4B_DecRead192B(ctx, ct+16);
		BTIC4B_DecRead32B(ctx, ct+40);
		BTIC4B_DecRead32B(ctx, ct+44);
		break;
	case 0x1D:
		BTIC4B_FillBlockHeadTag(ctx, ct, op);
		BTIC4B_DecRead256B(ctx, ct+16);
		BTIC4B_DecRead64B(ctx, ct+48);
		BTIC4B_DecRead64B(ctx, ct+56);
		break;
	case 0x1E:
		BTIC4B_FillBlockHeadTag(ctx, ct, op);
		BTIC4B_DecRead192B(ctx, ct+16);
		BTIC4B_DecRead96B(ctx, ct+40);
		BTIC4B_DecRead96B(ctx, ct+52);
		break;
	case 0x1F:
		BTIC4B_FillBlockHeadTag(ctx, ct, op);
		BTIC4B_DecRead256B(ctx, ct+16);
		BTIC4B_DecRead192B(ctx, ct+48);
		BTIC4B_DecRead192B(ctx, ct+72);
		break;
	}
	return(ct+ctx->blksz);
}

force_inline void BTIC4B_DecGetBlkPredClrs(BTIC4B_Context *ctx,
	byte *blk, int *rcyuvd)
{
	static const u32 mb=(1<<0x13)|(1<<0x19);
	int i, j;

	i=blk[0];
	if((i&0x20)|((mb>>(i&0x1F))&1))
//	if(((blk[0]&0x1F)==0x19) || ((blk[0]&0x1F)==0x13))
	{
		if(i&0x20)
		{
			rcyuvd[0]=ctx->cy;	rcyuvd[1]=ctx->cu;
			rcyuvd[2]=ctx->cv;	rcyuvd[3]=ctx->dy;
			rcyuvd[4]=ctx->du;	rcyuvd[5]=ctx->dv;
			return;
		}
		
		rcyuvd[0]=blk[2];
		rcyuvd[1]=(blk[3]-128)<<1;
		rcyuvd[2]=(blk[4]-128)<<1;
		rcyuvd[3]=blk[5];
		rcyuvd[4]=(blk[6]-128)<<1;
		rcyuvd[5]=(blk[7]-128)<<1;
	}else
	{
		rcyuvd[0]=*(s16 *)(blk+ 4);
		rcyuvd[1]=*(s16 *)(blk+ 6);
		rcyuvd[2]=*(s16 *)(blk+ 8);
		rcyuvd[3]=*(s16 *)(blk+10);
		rcyuvd[4]=*(s16 *)(blk+12);
		rcyuvd[5]=*(s16 *)(blk+14);
	}

#if 0
	switch(blk[0]&0x3F)
	{
	case 0x00:	case 0x01:	case 0x02:	case 0x03:
	case 0x04:	case 0x05:	case 0x06:	case 0x07:
	case 0x08:	case 0x09:	case 0x0A:	case 0x0B:
	case 0x0C:	case 0x0D:	case 0x0E:	case 0x0F:
	case 0x10:	case 0x11:	case 0x12:	case 0x14:
	case 0x15:	case 0x16:	case 0x17:	case 0x18:
	case 0x1A:	case 0x1B:	case 0x1C:	case 0x1D:
	case 0x1E:	case 0x1F:
		rcyuvd[0]=*(s16 *)(blk+ 4);
		rcyuvd[1]=*(s16 *)(blk+ 6);
		rcyuvd[2]=*(s16 *)(blk+ 8);
		rcyuvd[3]=*(s16 *)(blk+10);
		rcyuvd[4]=*(s16 *)(blk+12);
		rcyuvd[5]=*(s16 *)(blk+14);
		break;
	case 0x13:	case 0x19:
		rcyuvd[0]=blk[2];
		rcyuvd[1]=(blk[3]-128)<<1;
		rcyuvd[2]=(blk[4]-128)<<1;
		rcyuvd[3]=blk[5];
		rcyuvd[4]=(blk[6]-128)<<1;
		rcyuvd[5]=(blk[7]-128)<<1;
		break;
	default:
		rcyuvd[0]=ctx->cy;	rcyuvd[1]=ctx->cu;
		rcyuvd[2]=ctx->cv;	rcyuvd[3]=ctx->dy;
		rcyuvd[4]=ctx->du;	rcyuvd[5]=ctx->dv;
		break;
	}
#endif
}

force_inline void BTIC4B_DecGetBlkPredClrsFast(BTIC4B_Context *ctx,
	byte *blk, int *rcyuvd)
{
	rcyuvd[0]=*(s16 *)(blk+ 4);
	rcyuvd[1]=*(s16 *)(blk+ 6);
	rcyuvd[2]=*(s16 *)(blk+ 8);
	rcyuvd[3]=*(s16 *)(blk+10);
	rcyuvd[4]=*(s16 *)(blk+12);
	rcyuvd[5]=*(s16 *)(blk+14);
}

void BTIC4B_DecGetBlkPredClrs3Gen(BTIC4B_Context *ctx,
	byte *blka, byte *blkb, byte *blkc,
	int *rcyuv)
{
	BTIC4B_DecGetBlkPredClrs(ctx, blka, rcyuv+ 0);
	BTIC4B_DecGetBlkPredClrs(ctx, blkb, rcyuv+ 6);
	BTIC4B_DecGetBlkPredClrs(ctx, blkc, rcyuv+12);
}

void BTIC4B_DecGetBlkPredClrs3Fast(BTIC4B_Context *ctx,
	byte *blka, byte *blkb, byte *blkc,
	int *rcyuv)
{
	BTIC4B_DecGetBlkPredClrsFast(ctx, blka, rcyuv+ 0);
	BTIC4B_DecGetBlkPredClrsFast(ctx, blkb, rcyuv+ 6);
	BTIC4B_DecGetBlkPredClrsFast(ctx, blkc, rcyuv+12);
}

#if 0
int BTIC4B_Pred_Paeth(int a, int b, int c)
{
	int p, pa, pb, pc;

	p=a+b-c;
	pa=(p>a)?(p-a):(a-p);
	pb=(p>b)?(p-b):(b-p);
	pc=(p>c)?(p-c):(c-p);

	p=(pa<=pb)?((pa<=pc)?a:c):((pb<=pc)?b:c);
	return(p);
}
#endif

#if 0
int BTIC4B_Pred_Paeth(int a, int b, int c)
{
	int p, pa, pb, pc;
	int ma, mb, mc;
	p=a+b-c;
	pa=p-a; pa=pa*pa;
	pb=p-b; pb=pb*pb;
	pc=p-c; pc=pc*pc;
	ma=((pb-pa)>>24);
	mb=((pc-pb)>>24);
	mc=((pc-pa)>>24);
	p=(ma&((mb&c)|((~mb)&b))) | ((~ma)&((mc&c)|((~mc)&a)));
	return(p);
}
#endif

#if 1
force_inline int BTIC4B_Pred_Paeth(int a, int b, int c)
{
	int p, pa, pb, pc;
	int ma, mb, mc;
	p=a+b-c;

	pa=p-a;	pb=p-b;	pc=p-c;
	pa=pa^(pa>>31);	pb=pb^(pb>>31);	pc=pc^(pc>>31);
	ma=((pb-pa)>>31);
	mb=((pc-pb)>>31);
	mc=((pc-pa)>>31);
	p=(ma&((mb&c)|((~mb)&b))) | ((~ma)&((mc&c)|((~mc)&a)));
	return(p);
}
#endif

#if 0
force_inline void BTIC4B_Pred_Paeth3(
	int a0, int b0, int c0,
	int a1, int b1, int c1,
	int a2, int b2, int c2,
	int *rp0, int *rp1, int *rp2)
{
	int p0, pa0, pb0, pc0;
	int p1, pa1, pb1, pc1;
	int p2, pa2, pb2, pc2;
	int ma0, mb0, mc0;
	int ma1, mb1, mc1;
	int ma2, mb2, mc2;
	p0=a0+b0-c0;
	p1=a1+b1-c1;
	p2=a2+b2-c2;
	pa0=p0-a0;	pb0=p0-b0;	pc0=p0-c0;
	pa1=p1-a1;	pb1=p1-b1;	pc1=p1-c1;
	pa2=p2-a2;	pb2=p2-b2;	pc2=p2-c2;
	pa0=pa0^(pa0>>31);		pb0=pb0^(pb0>>31);		pc0=pc0^(pc0>>31);
	pa1=pa1^(pa1>>31);		pb1=pb1^(pb1>>31);		pc1=pc1^(pc1>>31);
	pa2=pa2^(pa2>>31);		pb2=pb2^(pb2>>31);		pc2=pc2^(pc2>>31);
	ma0=((pb0-pa0)>>31);	mb0=((pc0-pb0)>>31);	mc0=((pc0-pa0)>>31);
	ma1=((pb1-pa1)>>31);	mb1=((pc1-pb1)>>31);	mc1=((pc1-pa1)>>31);
	ma2=((pb2-pa2)>>31);	mb2=((pc2-pb2)>>31);	mc2=((pc2-pa2)>>31);
	*rp0=(ma0&((mb0&c0)|((~mb0)&b0))) | ((~ma0)&((mc0&c0)|((~mc0)&a0)));
	*rp1=(ma1&((mb1&c1)|((~mb1)&b1))) | ((~ma1)&((mc1&c1)|((~mc1)&a1)));
	*rp2=(ma2&((mb2&c2)|((~mb2)&b2))) | ((~ma2)&((mc2&c2)|((~mc2)&a2)));
}
#endif

#if 1
force_inline void BTIC4B_Pred_Paeth3J(
	int a0, int b0, int c0,
	int a1, int b1, int c1,
	int a2, int b2, int c2,
	int *rp0, int *rp1, int *rp2)
{
	int p0, pa0, pb0, pc0;
	int p1, pa1, pb1, pc1;
	int p2, pa2, pb2, pc2;
	int ma0, mb0, mc0;
	int ma1, mb1, mc1;
	int ma2, mb2, mc2;

	p0=a0+b0-c0;
	pa0=p0-a0;	pb0=p0-b0;	pc0=p0-c0;
	pa0=pa0^(pa0>>31);		pb0=pb0^(pb0>>31);		pc0=pc0^(pc0>>31);
	ma0=((pb0-pa0)>>31);	mb0=((pc0-pb0)>>31);	mc0=((pc0-pa0)>>31);

	ma2=(~ma0)&(~mc0);
	mb2=ma0&(~mb0);
	mc2=(ma0&mb0)|((~ma0)&mc0);

	*rp0=(ma2&a0)|(mb2&b0)|(mc2&c0);
	*rp1=(ma2&a1)|(mb2&b1)|(mc2&c1);
	*rp2=(ma2&a2)|(mb2&b2)|(mc2&c2);
}
#endif

force_inline int BTIC4B_Pred_HalfLin(int a, int b, int c)
	{ return((3*a+3*b-2*c)>>2); }
force_inline int BTIC4B_Pred_HalfAvg(int a, int b, int c)
	{ return((3*a+3*b+2*c)>>3); }
// force_inline int BTIC4B_Pred_HalfAvg(int a, int b, int c)
//	{ return((7*a+7*b+2*c)>>4); }


#if 0
void BTIC4B_DecUpdateCtxPred(BTIC4B_Context *ctx,
	byte *pba, byte *pbb, byte *pbc, byte pred)
{
	int pyuva[6], pyuvb[6], pyuvc[6], pyuvd[6];
	
	BTIC4B_DecGetBlkPredClrs(ctx, pba, pyuva);
	BTIC4B_DecGetBlkPredClrs(ctx, pbb, pyuvb);
	BTIC4B_DecGetBlkPredClrs(ctx, pbc, pyuvc);

	if(pred==2)
	{
		ctx->cy=BTIC4B_Pred_HalfLin(pyuvc[0], pyuvb[0], pyuva[0]);
		ctx->cu=BTIC4B_Pred_HalfLin(pyuvc[1], pyuvb[1], pyuva[1]);
		ctx->cv=BTIC4B_Pred_HalfLin(pyuvc[2], pyuvb[2], pyuva[2]);
		ctx->dy=BTIC4B_Pred_HalfLin(pyuvc[3], pyuvb[3], pyuva[3]);
		ctx->du=BTIC4B_Pred_HalfLin(pyuvc[4], pyuvb[4], pyuva[4]);
		ctx->dv=BTIC4B_Pred_HalfLin(pyuvc[5], pyuvb[5], pyuva[5]);
		return;
	}

	if(pred==4)
	{
		BTIC4B_Pred_Paeth3J(
			pyuvc[0], pyuvb[0], pyuva[0],
			pyuvc[1], pyuvb[1], pyuva[1],
			pyuvc[2], pyuvb[2], pyuva[2],
			&(ctx->cy), &(ctx->cu), &(ctx->cv));
		BTIC4B_Pred_Paeth3J(
			pyuvc[3], pyuvb[3], pyuva[3],
			pyuvc[4], pyuvb[4], pyuva[4],
			pyuvc[5], pyuvb[5], pyuva[5],
			&(ctx->dy), &(ctx->du), &(ctx->dv));
		return;
	}

	if(pred==3)
	{
		ctx->cy=BTIC4B_Pred_HalfAvg(pyuvc[0], pyuvb[0], pyuva[0]);
		ctx->cu=BTIC4B_Pred_HalfAvg(pyuvc[1], pyuvb[1], pyuva[1]);
		ctx->cv=BTIC4B_Pred_HalfAvg(pyuvc[2], pyuvb[2], pyuva[2]);
		ctx->dy=BTIC4B_Pred_HalfAvg(pyuvc[3], pyuvb[3], pyuva[3]);
		ctx->du=BTIC4B_Pred_HalfAvg(pyuvc[4], pyuvb[4], pyuva[4]);
		ctx->dv=BTIC4B_Pred_HalfAvg(pyuvc[5], pyuvb[5], pyuva[5]);
		return;
	}

	if(pred==1)
	{
		ctx->cy=BTIC4B_Pred_Paeth(pyuvc[0], pyuvb[0], pyuva[0]);
		ctx->cu=BTIC4B_Pred_Paeth(pyuvc[1], pyuvb[1], pyuva[1]);
		ctx->cv=BTIC4B_Pred_Paeth(pyuvc[2], pyuvb[2], pyuva[2]);
		ctx->dy=BTIC4B_Pred_Paeth(pyuvc[3], pyuvb[3], pyuva[3]);
		ctx->du=BTIC4B_Pred_Paeth(pyuvc[4], pyuvb[4], pyuva[4]);
		ctx->dv=BTIC4B_Pred_Paeth(pyuvc[5], pyuvb[5], pyuva[5]);
		return;
	}


#if 0
	switch(pred)
	{
	case 0:
		break;
	case 1:
		ctx->cy=BTIC4B_Pred_Paeth(pyuvc[0], pyuvb[0], pyuva[0]);
		ctx->cu=BTIC4B_Pred_Paeth(pyuvc[1], pyuvb[1], pyuva[1]);
		ctx->cv=BTIC4B_Pred_Paeth(pyuvc[2], pyuvb[2], pyuva[2]);
		ctx->dy=BTIC4B_Pred_Paeth(pyuvc[3], pyuvb[3], pyuva[3]);
		ctx->du=BTIC4B_Pred_Paeth(pyuvc[4], pyuvb[4], pyuva[4]);
		ctx->dv=BTIC4B_Pred_Paeth(pyuvc[5], pyuvb[5], pyuva[5]);
		break;
	default:
		break;
	}
#endif
}
#endif

void BTIC4B_DecUpdateCtxPredV(BTIC4B_Context *ctx,
	byte *pba, byte *pbb, byte *pbc, byte pred,
	int *rpyc)
{
	int pyuva[6], pyuvb[6], pyuvc[6];
	
	BTIC4B_DecGetBlkPredClrs(ctx, pba, pyuva);
	BTIC4B_DecGetBlkPredClrs(ctx, pbb, pyuvb);
	BTIC4B_DecGetBlkPredClrs(ctx, pbc, pyuvc);
	
	switch(pred)
	{
	case 0:
		break;
	case 1:
		rpyc[0]=BTIC4B_Pred_Paeth(pyuvc[0], pyuvb[0], pyuva[0]);
		rpyc[1]=BTIC4B_Pred_Paeth(pyuvc[1], pyuvb[1], pyuva[1]);
		rpyc[2]=BTIC4B_Pred_Paeth(pyuvc[2], pyuvb[2], pyuva[2]);
		rpyc[3]=BTIC4B_Pred_Paeth(pyuvc[3], pyuvb[3], pyuva[3]);
		rpyc[4]=BTIC4B_Pred_Paeth(pyuvc[4], pyuvb[4], pyuva[4]);
		rpyc[5]=BTIC4B_Pred_Paeth(pyuvc[5], pyuvb[5], pyuva[5]);
		break;
	case 2:
		rpyc[0]=BTIC4B_Pred_HalfLin(pyuvc[0], pyuvb[0], pyuva[0]);
		rpyc[1]=BTIC4B_Pred_HalfLin(pyuvc[1], pyuvb[1], pyuva[1]);
		rpyc[2]=BTIC4B_Pred_HalfLin(pyuvc[2], pyuvb[2], pyuva[2]);
		rpyc[3]=BTIC4B_Pred_HalfLin(pyuvc[3], pyuvb[3], pyuva[3]);
		rpyc[4]=BTIC4B_Pred_HalfLin(pyuvc[4], pyuvb[4], pyuva[4]);
		rpyc[5]=BTIC4B_Pred_HalfLin(pyuvc[5], pyuvb[5], pyuva[5]);
		return;
	case 3:
		rpyc[0]=BTIC4B_Pred_HalfAvg(pyuvc[0], pyuvb[0], pyuva[0]);
		rpyc[1]=BTIC4B_Pred_HalfAvg(pyuvc[1], pyuvb[1], pyuva[1]);
		rpyc[2]=BTIC4B_Pred_HalfAvg(pyuvc[2], pyuvb[2], pyuva[2]);
		rpyc[3]=BTIC4B_Pred_HalfAvg(pyuvc[3], pyuvb[3], pyuva[3]);
		rpyc[4]=BTIC4B_Pred_HalfAvg(pyuvc[4], pyuvb[4], pyuva[4]);
		rpyc[5]=BTIC4B_Pred_HalfAvg(pyuvc[5], pyuvb[5], pyuva[5]);
		return;
	case 4:
		BTIC4B_Pred_Paeth3J(
			pyuvc[0], pyuvb[0], pyuva[0],
			pyuvc[1], pyuvb[1], pyuva[1],
			pyuvc[2], pyuvb[2], pyuva[2],
			rpyc+0, rpyc+1, rpyc+2);
		BTIC4B_Pred_Paeth3J(
			pyuvc[3], pyuvb[3], pyuva[3],
			pyuvc[4], pyuvb[4], pyuva[4],
			pyuvc[5], pyuvb[5], pyuva[5],
			rpyc+3, rpyc+4, rpyc+5);
		break;
	default:
		break;
	}
}

void BTIC4B_DecUpdateCtxPred(BTIC4B_Context *ctx,
	byte *pba, byte *pbb, byte *pbc, byte pred)
{
	int pyuvd[6];
	BTIC4B_DecUpdateCtxPredV(ctx, pba, pbb, pbc, pred, pyuvd);
	ctx->cy=pyuvd[0];	ctx->cu=pyuvd[1];
	ctx->cv=pyuvd[2];	ctx->dy=pyuvd[3];
	ctx->du=pyuvd[4];	ctx->dv=pyuvd[5];
}

void BTIC4B_DecUpdatePred_Generic(BTIC4B_Context *ctx,
	byte *ct, byte *blks)
{
	int pyuvd[6];
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
//		BTIC4B_DecUpdateCtxPred(ctx,
//			ctpa, ctpb, ctpc, ctx->pred);
		BTIC4B_DecUpdateCtxPredV(ctx,
			ctpa, ctpb, ctpc, ctx->pred, pyuvd);
		ctx->cy=pyuvd[0];	ctx->cu=pyuvd[1];
		ctx->cv=pyuvd[2];	ctx->dy=pyuvd[3];
		ctx->du=pyuvd[4];	ctx->dv=pyuvd[5];
	}
}

void BTIC4B_DecUpdatePred_HalfAvg(BTIC4B_Context *ctx,
	byte *ct, byte *blks)
{
	int pyuv[3*6];
	byte *ctpa, *ctpb, *ctpc;
	int xs1;

	xs1=ctx->xsb;
	ctpa=ct-(xs1+1)*ctx->blksz;
	if(ctpa>=blks)
	{
		ctpb=ct-xs1*ctx->blksz;
		ctpc=ct-ctx->blksz;

//		BTIC4B_DecGetBlkPredClrs(ctx, ctpa, pyuva);
//		BTIC4B_DecGetBlkPredClrs(ctx, ctpb, pyuvb);
//		BTIC4B_DecGetBlkPredClrs(ctx, ctpc, pyuvc);

		ctx->DecGetBlkPredClrs3(ctx, ctpa, ctpb, ctpc, pyuv);

		ctx->cy=BTIC4B_Pred_HalfAvg(pyuv[12], pyuv[ 6], pyuv[0]);
		ctx->cu=BTIC4B_Pred_HalfAvg(pyuv[13], pyuv[ 7], pyuv[1]);
		ctx->cv=BTIC4B_Pred_HalfAvg(pyuv[14], pyuv[ 8], pyuv[2]);
		ctx->dy=BTIC4B_Pred_HalfAvg(pyuv[15], pyuv[ 9], pyuv[3]);
		ctx->du=BTIC4B_Pred_HalfAvg(pyuv[16], pyuv[10], pyuv[4]);
		ctx->dv=BTIC4B_Pred_HalfAvg(pyuv[17], pyuv[11], pyuv[5]);
	}
}

void BTIC4B_DecUpdatePred_Paeth3J(BTIC4B_Context *ctx,
	byte *ct, byte *blks)
{
	int pyuv[3*6];
//	int pyuvd[6];
	byte *ctpa, *ctpb, *ctpc;
	int xs1;

	xs1=ctx->xsb;
	ctpa=ct-(xs1+1)*ctx->blksz;
	if(ctpa>=blks)
	{
		ctpb=ct-xs1*ctx->blksz;
		ctpc=ct-ctx->blksz;

//		BTIC4B_DecGetBlkPredClrs(ctx, ctpa, pyuva);
//		BTIC4B_DecGetBlkPredClrs(ctx, ctpb, pyuvb);
//		BTIC4B_DecGetBlkPredClrs(ctx, ctpc, pyuvc);

		ctx->DecGetBlkPredClrs3(ctx, ctpa, ctpb, ctpc, pyuv);

		BTIC4B_Pred_Paeth3J(
			pyuv[12], pyuv[ 6], pyuv[ 0],
			pyuv[13], pyuv[ 7], pyuv[ 1],
			pyuv[14], pyuv[ 8], pyuv[ 2],
			&(ctx->cy), &(ctx->cu), &(ctx->cv));
		BTIC4B_Pred_Paeth3J(
			pyuv[15], pyuv[ 9], pyuv[ 3],
			pyuv[16], pyuv[10], pyuv[ 4],
			pyuv[17], pyuv[11], pyuv[ 5],
			&(ctx->dy), &(ctx->du), &(ctx->dv));
	}
}

void BTIC4B_DecUpdatePred_HalfAvgF(BTIC4B_Context *ctx,
	byte *ct, byte *blks)
{
	int pyuv[3*6];
	byte *ctpa, *ctpb, *ctpc;
	int xs1;

	xs1=ctx->xsb;
	ctpa=ct-(xs1+1)*ctx->blksz;
	if(ctpa>=blks)
	{
		ctpb=ct-xs1*ctx->blksz;
		ctpc=ct-ctx->blksz;

//		BTIC4B_DecGetBlkPredClrs(ctx, ctpa, pyuva);
//		BTIC4B_DecGetBlkPredClrs(ctx, ctpb, pyuvb);
//		BTIC4B_DecGetBlkPredClrs(ctx, ctpc, pyuvc);

//		ctx->DecGetBlkPredClrs3(ctx, ctpa, ctpb, ctpc, pyuv);

		BTIC4B_DecGetBlkPredClrsFast(ctx, ctpa, pyuv+ 0);
		BTIC4B_DecGetBlkPredClrsFast(ctx, ctpb, pyuv+ 6);
		BTIC4B_DecGetBlkPredClrsFast(ctx, ctpc, pyuv+12);

		ctx->cy=BTIC4B_Pred_HalfAvg(pyuv[12], pyuv[ 6], pyuv[0]);
		ctx->cu=BTIC4B_Pred_HalfAvg(pyuv[13], pyuv[ 7], pyuv[1]);
		ctx->cv=BTIC4B_Pred_HalfAvg(pyuv[14], pyuv[ 8], pyuv[2]);
		ctx->dy=BTIC4B_Pred_HalfAvg(pyuv[15], pyuv[ 9], pyuv[3]);
		ctx->du=BTIC4B_Pred_HalfAvg(pyuv[16], pyuv[10], pyuv[4]);
		ctx->dv=BTIC4B_Pred_HalfAvg(pyuv[17], pyuv[11], pyuv[5]);
	}
}

void BTIC4B_DecUpdatePred_Paeth3JF(BTIC4B_Context *ctx,
	byte *ct, byte *blks)
{
	int pyuv[3*6];
//	int pyuvd[6];
	byte *ctpa, *ctpb, *ctpc;
	int xs1;

	xs1=ctx->xsb;
	ctpa=ct-(xs1+1)*ctx->blksz;
	if(ctpa>=blks)
	{
		ctpb=ct-xs1*ctx->blksz;
		ctpc=ct-ctx->blksz;

//		BTIC4B_DecGetBlkPredClrs(ctx, ctpa, pyuva);
//		BTIC4B_DecGetBlkPredClrs(ctx, ctpb, pyuvb);
//		BTIC4B_DecGetBlkPredClrs(ctx, ctpc, pyuvc);

//		ctx->DecGetBlkPredClrs3(ctx, ctpa, ctpb, ctpc, pyuv);

		BTIC4B_DecGetBlkPredClrsFast(ctx, ctpa, pyuv+ 0);
		BTIC4B_DecGetBlkPredClrsFast(ctx, ctpb, pyuv+ 6);
		BTIC4B_DecGetBlkPredClrsFast(ctx, ctpc, pyuv+12);

		BTIC4B_Pred_Paeth3J(
			pyuv[12], pyuv[ 6], pyuv[ 0],
			pyuv[13], pyuv[ 7], pyuv[ 1],
			pyuv[14], pyuv[ 8], pyuv[ 2],
			&(ctx->cy), &(ctx->cu), &(ctx->cv));
		BTIC4B_Pred_Paeth3J(
			pyuv[15], pyuv[ 9], pyuv[ 3],
			pyuv[16], pyuv[10], pyuv[ 4],
			pyuv[17], pyuv[11], pyuv[ 5],
			&(ctx->dy), &(ctx->du), &(ctx->dv));
	}
}

force_inline void BTIC4B_DecUpdatePred(
		BTIC4B_Context *ctx, byte *ct, byte *blks)
	{ ctx->DecUpdatePred(ctx, ct, blks); }

void BTIC4B_DecUpdatePred_None(BTIC4B_Context *ctx,
	byte *ct, byte *blks)
{
}

void BTIC4B_DecodeSetPredictor(BTIC4B_Context *ctx, int pred)
{
	ctx->pred=pred;
	
	if(ctx->pred_l8p)
	{
		ctx->DecGetBlkPredClrs3=BTIC4B_DecGetBlkPredClrs3Gen;
		switch(pred)
		{
		case 0:
			ctx->DecUpdatePred=BTIC4B_DecUpdatePred_None;
			break;
		case 3:
			ctx->DecUpdatePred=BTIC4B_DecUpdatePred_HalfAvg;
			break;
		case 4:
			ctx->DecUpdatePred=BTIC4B_DecUpdatePred_Paeth3J;
			break;
		default:
			ctx->DecUpdatePred=BTIC4B_DecUpdatePred_Generic;
			break;
		}
	}
	else
	{
		ctx->DecGetBlkPredClrs3=BTIC4B_DecGetBlkPredClrs3Fast;
		switch(pred)
		{
		case 0:
			ctx->DecUpdatePred=BTIC4B_DecUpdatePred_None;
			break;
		case 3:
			ctx->DecUpdatePred=BTIC4B_DecUpdatePred_HalfAvgF;
			break;
		case 4:
			ctx->DecUpdatePred=BTIC4B_DecUpdatePred_Paeth3JF;
			break;
		default:
			ctx->DecUpdatePred=BTIC4B_DecUpdatePred_Generic;
			break;
		}
	}
}

void BTIC4B_DecodeDirtyL8(BTIC4B_Context *ctx)
{
	if(!ctx->pred_l8p)
	{
		ctx->pred_l8p=1;
//		ctx->DecGetBlkPredClrs3=BTIC4B_DecGetBlkPredClrs3Gen;
		BTIC4B_DecodeSetPredictor(ctx, ctx->pred);
	}
}

int BTIC4B_DecImgBlocks(BTIC4B_Context *ctx,
	byte *cbuf, int cbsz,
	byte *blks, byte *lblks,
	int xs, int ys)
{
	byte opb[256];
	byte oprov;

//	byte blkb[64];
	byte *cs, *cse;
	byte *ct, *lcs, *cte;
	int xs1, ys1, nblk;
	int op, lop0, lop1, ret;
	int i, j, k, l, n;

	BTIC4B_SetupContextQf(ctx, 100);

// #ifdef BTIC4B_BYTES
#if 0
	ctx->cs=cbuf; cse=cbuf+cbsz;
#else
	BTIC4B_InitScTables();
	BTIC4B_InitRice();
	BTIC4B_SetupReadBits(ctx, cbuf, cbsz);
#endif

	xs1=(xs+7)>>3;
	ys1=(ys+7)>>3;
	nblk=xs1*ys1;

//	ctx->cs=cbuf; cse=cbuf+cbsz;

	memset(opb, 255, 256);
	oprov=0;

	ctx->pred_l8p=0;
	BTIC4B_DecodeSetPredictor(ctx, 0);

	ct=blks; lcs=lblks; ret=0; lop0=-1; op=-1;
	cte=blks+nblk*ctx->blksz;
//	while(((ctx->cs)<cse) && !ret)
	while(!ret && (ct<cte))
	{
#if 0
		k=BTIC4B_DecReadCommand(ctx);
		if(k!=0x4C)
		{
			__asm { int 3 }
			break;
		}
#endif

		lop1=lop0; lop0=op;
		op=BTIC4B_DecReadCommand(ctx);
		opb[oprov++]=op;
				
		switch(op)
		{
		case 0x00:
			BTIC4B_DecUpdatePred(ctx, ct, blks);
			BTIC4B_DecColorYUV(ctx);
			BTIC4B_FillBlockHeadTag(ctx, ct, 0);
			ct=ct+ctx->blksz;
//			ct=BTIC4B_DecSetupDecBlockInner(ctx, ct, op);
			break;
		
		case 0x01:
		case 0x02:	case 0x03:
		case 0x04:	case 0x05:
		case 0x06:	case 0x07:
		case 0x09:
		case 0x0A:	case 0x0B:
		case 0x0C:	case 0x0D:
		case 0x0E:	case 0x0F:
		case 0x10:	case 0x11:
//		case 0x13:
//		case 0x14:
			BTIC4B_DecUpdatePred(ctx, ct, blks);
			BTIC4B_DecColorYUVD(ctx);
			ct=BTIC4B_DecSetupDecBlockInner(ctx, ct, op);
			break;

//		case 0x10:
		case 0x13:
		case 0x14:	case 0x15:
		case 0x16:	case 0x17:
		case 0x18:	case 0x19:
		case 0x1A:	case 0x1B:
		case 0x1C:	case 0x1D:
		case 0x1E:	case 0x1F:
			BTIC4B_DecUpdatePred(ctx, ct, blks);
			BTIC4B_DecColorYUVDyuv(ctx);
			ct=BTIC4B_DecSetupDecBlockInner(ctx, ct, op);
			break;

		case 0x20:
			n=BTIC4B_DecReadCountVal(ctx);
			for(i=0; i<n; i++)
			{
				BTIC4B_DecUpdatePred(ctx, ct, blks);
				ct=BTIC4B_DecSetupDecBlockInner(ctx, ct, op&0x1F);
			}
			break;

		case 0x21:	case 0x22:	case 0x23:
		case 0x24:	case 0x25:	case 0x26:	case 0x27:
		case 0x28:	case 0x29:	case 0x2A:	case 0x2B:
		case 0x2C:	case 0x2D:	case 0x2E:	case 0x2F:
		case 0x30:	case 0x31:	case 0x32:	case 0x33:
		case 0x34:	case 0x35:	case 0x36:	case 0x37:
		case 0x38:	case 0x39:	case 0x3A:	case 0x3B:
		case 0x3C:	case 0x3D:	case 0x3E:	case 0x3F:
			n=BTIC4B_DecReadCountVal(ctx);
			for(i=0; i<n; i++)
			{
				BTIC4B_DecUpdatePred(ctx, ct, blks);
				ct=BTIC4B_DecSetupDecBlockInner(ctx, ct, op&0x1F);
			}
			break;
		
		case 0x40:
			ret=1; break;

		case 0x41:
			BTIC4B_DecodeDirtyL8(ctx);
			n=BTIC4B_DecReadCountVal(ctx);
			lcs=lblks+(ct-blks);
			BTIC4B_DecodeCopyBlocks(ctx, ct, lcs, n);
			ct+=n*ctx->blksz;
			break;
		case 0x42:
			BTIC4B_DecodeDirtyL8(ctx);
			n=BTIC4B_DecReadCountVal(ctx);
			j=BTIC4B_DecReadGenericVal(ctx);
			i=BTIC4B_DecReadGenericVal(ctx);
			lcs=lblks+(ct-blks);
			BTIC4B_DecodeCopyBlocks(ctx, ct, lcs+((i*xs1+j)*ctx->blksz), n);
			ct+=n*ctx->blksz;
			break;
		case 0x43:
			i=BTIC4B_DecReadGenericVal(ctx);
			j=BTIC4B_DecReadGenericVal(ctx);
			BTIC4B_DecodeSetParm(ctx, i, j);
			break;
		
		case 0x45:
			ctx->qy=BTIC4B_DecReadGenericUVal(ctx);
			ctx->quv=BTIC4B_DecReadGenericUVal(ctx);
			ctx->qdy=BTIC4B_DecReadGenericUVal(ctx);
			ctx->qduv=BTIC4B_DecReadGenericUVal(ctx);
			break;
		case 0x46:
			i=BTIC4B_DecReadGenericVal(ctx);
			BTIC4B_DecodeEnableFeature(ctx, i);
			break;
		case 0x47:
			i=BTIC4B_DecReadGenericVal(ctx);
			BTIC4B_DecodeDisableFeature(ctx, i);
			break;

		case 0x48:
			n=BTIC4B_DecReadCountVal(ctx);
			for(i=0; i<n; i++)
			{
				BTIC4B_DecUpdatePred(ctx, ct, blks);
				BTIC4B_DecColorYUV(ctx);
				BTIC4B_FillBlockHeadTag(ctx, ct, 0);
				ct=ct+ctx->blksz;
			}
			break;
		
		case 0x4C:
			break;

		default:
//			*(int *)-1=-1;
//			__asm { int 3 }
			BTIC4B_DBGTRAP
			ret=-1;
			break;
		}
	}
	return(ret);
}

byte *BTIC4B_DecSetupDecAlphaBlockInner(BTIC4B_Context *ctx,
	byte *ct, int op)
{
//	byte *ct;

	switch(op)
	{
	case 0x00:
		BTIC4B_FillBlockHeadAlphaTagL8(ctx, ct, 0);
		break;
	case 0x01:
		BTIC4B_FillBlockHeadAlphaTag(ctx, ct, op);
		*(ct+48+8)=BTIC4B_ReadNBits(ctx, 4);
		break;
	case 0x02:
		BTIC4B_FillBlockHeadAlphaTag(ctx, ct, op);
		*(u16 *)(ct+48+8)=BTIC4B_Read16Bits(ctx);
		break;
	case 0x03:
		BTIC4B_FillBlockHeadAlphaTag(ctx, ct, op);
		BTIC4B_DecRead64B(ctx, ct+48+8);
		break;
	case 0x04:
		BTIC4B_FillBlockHeadAlphaTag(ctx, ct, op);
		*(ct+48+8)=BTIC4B_Read8Bits(ctx);
		break;
	case 0x05:
		BTIC4B_FillBlockHeadAlphaTag(ctx, ct, op);
		*(u32 *)(ct+48+8)=BTIC4B_Read32Bits(ctx);
		break;
	case 0x06:
		BTIC4B_FillBlockHeadAlphaTag(ctx, ct, op);
		BTIC4B_DecRead48B(ctx, ct+48+8);
		break;
	case 0x07:
		BTIC4B_FillBlockHeadAlphaTag(ctx, ct, op);
		BTIC4B_DecRead64B(ctx, ct+48+8);
		break;
	case 0x08:
		BTIC4B_FillBlockHeadAlphaTag(ctx, ct, op);
		break;

	case 0x0A:
	case 0x0B:
		BTIC4B_FillBlockHeadAlphaTag(ctx, ct, op);
		BTIC4B_DecRead64B(ctx, ct+48+8);
		break;
	case 0x0C:
		BTIC4B_FillBlockHeadAlphaTagL8(ctx, ct, op);
		BTIC4B_DecRead128B(ctx, ct+48);
		break;
	case 0x0D:
		BTIC4B_FillBlockHeadAlphaTag(ctx, ct, op);
		BTIC4B_DecRead128B(ctx, ct+64+8);
		break;
	case 0x0E:
		BTIC4B_FillBlockHeadAlphaTag(ctx, ct, op);
		BTIC4B_DecRead192B(ctx, ct+64+8);
		break;
	case 0x0F:
		BTIC4B_FillBlockHeadAlphaTagL8(ctx, ct, op);
		BTIC4B_DecRead256B(ctx, ct+64);
		break;
	default:
		break;
	}
	return(ct+ctx->blksz);
}

int BTIC4B_DecImgAlphaBlocks(BTIC4B_Context *ctx,
	byte *cbuf, int cbsz,
	byte *blks, byte *lblks,
	int xs, int ys)
{
	byte opb[256];
	byte oprov;

//	byte blkb[64];
	byte *cs, *cse;
	byte *ct, *lcs;
	int xs1, ys1;
	int op, lop0, lop1, ret;
	int i, j, k, l, n;

	BTIC4B_SetupContextQf(ctx, 100);

// #ifdef BTIC4B_BYTES
#if 0
	ctx->cs=cbuf; cse=cbuf+cbsz;
#else
	BTIC4B_InitScTables();
	BTIC4B_InitRice();
	BTIC4B_SetupReadBits(ctx, cbuf, cbsz);
#endif

	xs1=(xs+7)>>3;
	ys1=(ys+7)>>3;

//	ctx->cs=cbuf; cse=cbuf+cbsz;

	memset(opb, 255, 256);
	oprov=0;

	ct=blks; lcs=lblks; ret=0; lop0=-1; op=-1;
	while(!ret)
	{
		lop1=lop0; lop0=op;
		op=BTIC4B_DecReadCommand(ctx);
		opb[oprov++]=op;
		
		switch(op)
		{
		case 0x00:
			BTIC4B_DecColorY(ctx);
			BTIC4B_FillBlockHeadAlphaTag(ctx, ct, 0);
			ct=ct+ctx->blksz;
			break;
		
		case 0x01:
		case 0x02:	case 0x03:
		case 0x04:	case 0x05:
		case 0x06:	case 0x07:
		case 0x09:
		case 0x0A:	case 0x0B:
		case 0x0C:	case 0x0D:
		case 0x0E:	case 0x0F:
			BTIC4B_DecColorYD(ctx);
			ct=BTIC4B_DecSetupDecAlphaBlockInner(ctx, ct, op);
			break;

		case 0x20:	case 0x21:	case 0x22:	case 0x23:
		case 0x24:	case 0x25:	case 0x26:	case 0x27:
		case 0x28:	case 0x29:	case 0x2A:	case 0x2B:
		case 0x2C:	case 0x2D:	case 0x2E:	case 0x2F:
			n=BTIC4B_DecReadCountVal(ctx);
			for(i=0; i<n; i++)
				{ ct=BTIC4B_DecSetupDecAlphaBlockInner(ctx, ct, op&0x1F); }
			break;
		
		case 0x40:
			ret=1; break;

		case 0x41:
			n=BTIC4B_DecReadCountVal(ctx);
			lcs=lblks+(ct-blks);
//			BTIC4B_DecodeCopyBlocks(ctx, ct, lcs, n);
			ct+=n*ctx->blksz;
			break;
		case 0x42:
			n=BTIC4B_DecReadCountVal(ctx);
			i=BTIC4B_DecReadGenericVal(ctx);
			j=BTIC4B_DecReadGenericVal(ctx);
			lcs=lblks+(ct-blks);
			BTIC4B_DecodeCopyAlphaBlocks(ctx, ct, lcs+((i*xs1+j)*64), n);
			ct+=n*ctx->blksz;
			break;
		case 0x43:
			i=BTIC4B_DecReadGenericVal(ctx);
			j=BTIC4B_DecReadGenericVal(ctx);
			BTIC4B_DecodeSetParm(ctx, i, j);
			break;
		
		case 0x45:
			ctx->qy=BTIC4B_DecReadGenericUVal(ctx);
			ctx->qdy=BTIC4B_DecReadGenericUVal(ctx);
			break;
		case 0x46:
			i=BTIC4B_DecReadGenericVal(ctx);
			BTIC4B_DecodeEnableFeature(ctx, i);
			break;
		case 0x47:
			i=BTIC4B_DecReadGenericVal(ctx);
			BTIC4B_DecodeDisableFeature(ctx, i);
			break;

		case 0x48:
			n=BTIC4B_DecReadCountVal(ctx);
			for(i=0; i<n; i++)
			{
				BTIC4B_DecColorY(ctx);
				BTIC4B_FillBlockHeadTag(ctx, ct, 0);
				ct=ct+ctx->blksz;
			}
			break;

		case 0x4C:
			break;

		default:
//			*(int *)-1=-1;
//			__asm { int 3 }
			BTIC4B_DBGTRAP
			ret=-1;
			break;
		}
	}
	return(ret);
}

byte *BTIC4B_BufReadUVLI(byte *cs, u64 *rval)
{
	u64 i;

	i=*cs++;
	if(i<0x80)
	{
		if(rval)*rval=i;
		return(cs);
	}else if(i<0xC0)
	{
		i=((i&0x7F)<<8)|(*cs++);
		if(rval)*rval=i;
		return(cs);
	}else if(i<0xE0)
	{
		i=((i&0x3F)<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		if(rval)*rval=i;
		return(cs);
	}else if(i<0xF0)
	{
		i=((i&0x1F)<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		if(rval)*rval=i;
		return(cs);
	}else if(i<0xF8)
	{
		i=((i&0x0F)<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		if(rval)*rval=i;
		return(cs);
	}else if(i<0xFC)
	{
		i=((i&0x07)<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		if(rval)*rval=i;
		return(cs);
	}else if(i<0xFE)
	{
		i=((i&0x03)<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		if(rval)*rval=i;
		return(cs);
	}else if(i<0xFF)
	{
		i=((i&0x01)<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		if(rval)*rval=i;
		return(cs);
	}else
	{
		i=(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		if(rval)*rval=i;
		return(cs);
	}
	
	return(cs);
}

BTIC4B_API int BTIC4B_DecodeImgHeaderDataCtx(BTIC4B_Context *ctx,
	byte *ics, int isz)
{
	u64 xs2, ys2, mod, qfl;
	byte *cs, *cse;

	cs=ics; cse=ics+isz;
	
	cs=BTIC4B_BufReadUVLI(cs, &xs2);
	cs=BTIC4B_BufReadUVLI(cs, &ys2);
	cs=BTIC4B_BufReadUVLI(cs, &mod);
	if(cs<cse)
		{ cs=BTIC4B_BufReadUVLI(cs, &qfl); }
	else
		{ qfl=0; }
	
	ctx->imgt=mod&7;
	ctx->qfl=qfl;
	
	switch((mod>>3)&3)
	{
	case 0:		ctx->blksz=32; break;
	case 1:		ctx->blksz=48; break;
	case 2:		ctx->blksz=64; break;
	case 3:		ctx->blksz=96; break;
	}

	switch((mod>>5)&3)
	{
	case 0:		ctx->clrt=BTIC4B_CLRT_GDBDR; break;
	case 1:		ctx->clrt=BTIC4B_CLRT_RCT; break;
	case 2:		ctx->clrt=BTIC4B_CLRT_YCBCR; break;
	case 3:		ctx->clrt=BTIC4B_CLRT_YCBCR; break;
	}
	return(0);
}

BTIC4B_API int BTIC4B_DecodeImgBufferCtx(BTIC4B_Context *ctx,
	byte *cbuf, int cbsz, byte *ibuf, int xs, int ys, int clrfl)
{
	byte *csib, *csibe, *csax, *csaxe;
	byte *cs, *cs1, *cse;
	int i, j, k;
	
	if(!cbsz)
		return(0);

	csib=NULL; csax=NULL;
	cs=cbuf; cse=cbuf+cbsz;
	while(cs<cse)
	{
		if((*cs>=0x00) && (*cs<=0x1F))
		{
			i=(cs[0]<<8)|(cs[1]);
			j=cs[2]|(cs[3]<<8);
			cs1=cs+i;
			
			if(j==BTIC4B_TCC_I0)
				{ csib=cs+4; csibe=cs1; }
			if(j==BTIC4B_TCC_AX)
				{ csax=cs+4; csaxe=cs1; }
			
			if(j==BTIC4B_TCC_HX)
				{ BTIC4B_DecodeImgHeaderDataCtx(ctx, cs+4, i-4); }
			
			cs=cs1;
			continue;
		}
		if((*cs>=0x20) && (*cs<=0x3F))
		{
			i=(cs[0]<<8)|(cs[1]);
			j=cs[2]|(cs[3]<<8)|(cs[4]<<16)|(cs[5]<<24);
			cs1=cs+i;
			cs=cs1;
			continue;
		}

		if((*cs>=0x40) && (*cs<=0x5F))
		{
			i=((cs[0]&0x1F)<<24)|(cs[1]<<16)|(cs[2]<<8)|(cs[3]);
			j=cs[4]|(cs[5]<<8);
			cs1=cs+i;
			
			if(j==BTIC4B_TCC_I0)
				{ csib=cs+6; csibe=cs1; }
			if(j==BTIC4B_TCC_AX)
				{ csax=cs+6; csaxe=cs1; }
			cs=cs1;
			continue;
		}

		if((*cs>=0x60) && (*cs<=0x7F))
		{
			i=((cs[0]&0x1F)<<24)|(cs[1]<<16)|(cs[2]<<8)|(cs[3]);
			j=cs[4]|(cs[5]<<8)|(cs[6]<<16)|(cs[7]<<24);
			cs1=cs+i;
			cs=cs1;
			continue;
		}
		
		break;
	}

	if(!csib)
		return(BTIC4B_ERRS_NOIMAGE);

	if((clrfl&0xFF)==0xFF)
		return(0);

	if(!ctx->blks)
	{
		ctx->xs=xs;
		ctx->ys=ys;
		ctx->xsb=(xs+7)>>3;
		ctx->ysb=(ys+7)>>3;
		ctx->nblk=ctx->xsb*ctx->ysb;
//		ctx->nblk=(ctx->xsb+1)*(ctx->ysb+1);
		if(!ctx->blksz)
			ctx->blksz=64;
		
		ctx->blks=malloc(ctx->nblk*ctx->blksz+256);
		ctx->lblks=malloc(ctx->nblk*ctx->blksz+256);
	}

	BTIC4B_DecImgBlocks(ctx, csib, csibe-csib,
		ctx->blks, ctx->lblks, xs, ys);
	if(csax)
	{
		BTIC4B_DecImgAlphaBlocks(ctx, csax, csaxe-csax,
			ctx->blks, ctx->lblks, xs, ys);
	}
	BTIC4B_DecImageClrs(ctx, ctx->blks, ibuf, xs, ys, clrfl);

	cs1=ctx->blks;
	ctx->blks=ctx->lblks;
	ctx->lblks=cs1;

	return(0);
}

//u32 btic4b_getu32le(byte *buf)
//{
//	return(buf[0]|(buf[1]<<8)|(buf[2]<<16)|(buf[3]<<24));
//}

BTIC4B_API byte *BTIC4B_BufBmpGetImg(byte *buf,
	int *rxs, int *rys,
	u32 *rfcc, int *risz)
{
	int ofs;
	
	if((buf[0]!='B') || (buf[1]!='M'))
		return(NULL);

//	ofs=*(u32 *)(buf+0x0A);
//	*rxs=*(u32 *)(buf+0x12);
//	*rys=*(u32 *)(buf+0x16);
//	*rfcc=*(u32 *)(buf+0x1E);
//	*risz=*(u32 *)(buf+0x22);

	ofs=btic4b_getu32le(buf+0x0A);
	*rxs=btic4b_getu32le(buf+0x12);
	*rys=btic4b_getu32le(buf+0x16);
	*rfcc=btic4b_getu32le(buf+0x1E);
	*risz=btic4b_getu32le(buf+0x22);

	return(buf+ofs);
}

BTIC4B_API int BTIC4B_DecodeImgBmpBufferCtx(BTIC4B_Context *ctx,
	byte *cbuf, int cbsz, byte *ibuf, int *rxs, int *rys, int clrfl)
{
	byte *tbuf;
	u32 fcc, px;
	int xs, ys, txs, tys, tsz, npx, tnpx;
	int i, j, k, n;
	
	tbuf=BTIC4B_BufBmpGetImg(cbuf, &xs, &ys, &fcc, &tsz);
	
	if(!ibuf)
	{
		if(fcc && (fcc!=BTIC4B_FCC_BT4B))
			return(BTIC4B_ERRS_BADFCC);

		if((fcc==BTIC4B_FCC_BT4B) && ((clrfl&0xFF)==0xFF))
		{
			BTIC4B_DecodeImgBufferCtx(ctx,
				tbuf, tsz, NULL, xs, ys, clrfl);
		}

		if(rxs)*rxs=xs;
		if(rys)*rys=ys;
		return(0);
	}

	if(rxs)txs=*rxs;
	if(rys)tys=*rys;
	if(txs&&tys)
	{
		npx=((xs+7)>>3)*((ys+7)>>3);
		tnpx=((txs+7)>>3)*((tys+7)>>3);
		if(npx>tnpx)
			return(BTIC4B_ERRS_BADIBUFSZ);
	}
	
//	if(fcc!=BTIC4B_FCC_BT4B)
//		return(BTIC4B_ERRS_BADFCC);

	if(fcc==0)
	{
		if(cbuf[0x1C]==32)
		{
			switch(clrfl&127)
			{
			case BTIC4B_CLRS_RGBA:
			case BTIC4B_CLRS_RGBX:
				n=xs*ys;
				for(i=0; i<n; i++)
				{
					px=((u32 *)tbuf)[i];
					px=(px&0xFF00FF00)|
						((px<<16)&0x00FF0000)|
						((px>>16)&0x000000FF);
					((u32 *)ibuf)[i]=px;
				}
				break;
			case BTIC4B_CLRS_BGRA:
			case BTIC4B_CLRS_BGRX:
				memcpy(ibuf, tbuf, xs*ys*4);
				break;
			default:
				break;
			}
			return(0);
		}

		if(cbuf[0x1C]==24)
		{
			switch(clrfl&127)
			{
			case BTIC4B_CLRS_RGBA:
			case BTIC4B_CLRS_RGBX:
				n=xs*ys;
				for(i=0; i<n; i++)
				{
					px=*((u32 *)(tbuf+(i*3)));
					px=px|0xFF000000;
					px=(px&0xFF00FF00)|
						((px<<16)&0x00FF0000)|
						((px>>16)&0x000000FF);
					((u32 *)ibuf)[i]=px;
				}
				break;
			case BTIC4B_CLRS_BGRA:
			case BTIC4B_CLRS_BGRX:
				n=xs*ys;
				for(i=0; i<n; i++)
				{
					px=*((u32 *)(tbuf+(i*3)));
					px=px|0xFF000000;
					((u32 *)ibuf)[i]=px;
				}
				break;
			default:
				break;
			}
			return(0);
		}
		return(BTIC4B_ERRS_BADFCC);
	}

	if(fcc==BTIC4B_FCC_BT4B)
	{
		i=BTIC4B_DecodeImgBufferCtx(ctx, tbuf, tsz, ibuf, xs, ys, clrfl);
		if(*rxs)*rxs=xs;
		if(*rys)*rys=ys;
		return(i);
	}

	return(BTIC4B_ERRS_BADFCC);
}

BTIC4B_API int BTIC4B_DecodeImgBmpBuffer(byte *cbuf, int cbsz,
	byte *ibuf, int *rxs, int *rys, int clrfl)
{
	BTIC4B_Context *ctx;
	int i;
	
	ctx=BTIC4B_AllocContext();
	i=BTIC4B_DecodeImgBmpBufferCtx(ctx, cbuf, cbsz,
		ibuf, rxs, rys, clrfl);
	BTIC4B_FreeContext(ctx);
	return(i);
}

BTIC4B_API int BTIC4B_DecodeImgBmpBuffer2(byte *cbuf, int cbsz,
	byte *ibuf, int *rxs, int *rys, int *imgt, int *clrt)
{
	BTIC4B_Context *ctx;
	int i;
	
	ctx=BTIC4B_AllocContext();
	i=BTIC4B_DecodeImgBmpBufferCtx(ctx, cbuf, cbsz,
		ibuf, rxs, rys, 0xFF);
		
	if(imgt)*imgt=ctx->imgt|(ctx->qfl&(~255));
	if(clrt)*clrt=ctx->clrt;
	BTIC4B_FreeContext(ctx);
	return(i);
}
