/*
 * STF+AdRice post-compressor for RP2.
 *
 * Escape Magic: 10-00
 *
 * Uses 3 contexts: Tag, Length/Distance, and Literal.
 *
 * Mostly separates bytes by category and then uses the appropriate context.
 *
 * Uses an AdRice implementation that steps by a fraction of a bit.
 * The slower stepping was better for compression in this case.
 * Where, Rice involves splitting the value into a unary coded prefix (Q)
 * and a K bit suffix. In AdRice, the K value is adjusted based on Q.
 * Rather than directly incrementing/decrementing K, was more effective to
 * step with a partial bit here to slow the adaptation.
 *
 * STF: Swap Towards Front
 * As each symbol is encoded, it is swapped towards the front of the list.
 * This will typically swap symbols towards an approximation of an optimal order.
 */

#define BTM_STFRK_ADBSHL	5
#define BTM_STFRK_ADBINIT	6

typedef struct BTM_StfRkCtx_s BTM_StfRkCtx;
struct BTM_StfRkCtx_s {
byte *cst;
u32 win;
byte pos;
};

int BTM_StfRk_PeekBits(BTM_StfRkCtx *ctx, int bits)
{
	u32 v;
	v=*(u32 *)(ctx->cst);
	return((v>>ctx->pos)&((1<<bits)-1));
}

void BTM_StfRk_SkipBits(BTM_StfRkCtx *ctx, int bits)
{
	int p;

	p=ctx->pos+bits;
	ctx->cst+=p>>3;
	ctx->pos=p&7;
}

void BTM_StfRk_SetupReadBits(BTM_StfRkCtx *ctx, byte *buf)
{
	ctx->cst=buf;
	ctx->pos=0;
}

#if 1
static const byte btm_stfrk_riceqtab[256]={
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 7,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 8
};

int BTM_StfRk_ReadAdRiceB(BTM_StfRkCtx *ctx, byte *rkf)
{
	int b, q, r, v, kf, kfb, skb;
	int p;

	kfb=*rkf;
	kf=kfb>>BTM_STFRK_ADBSHL;
	b=BTM_StfRk_PeekBits(ctx, 16);
	q=btm_stfrk_riceqtab[b&0xFF];
	if(q==8)
	{
		kfb++;
		skb=16;
		v=(b>>8)&0xFF;
	}else
	{
		r=(b>>(q+1))&((1<<kf)-1);
		skb=q+kf+1;
		v=(q<<kf)|r;
		if(!q && kfb)
			{ kfb--; }
		else if(q>1)
			{ kfb++; }
	}

	BTM_StfRk_SkipBits(ctx, skb);
	*rkf=kfb;
	return(v);
}
#endif

int BTM_StfRk_ReadAdRiceSTF(BTM_StfRkCtx *ctx, byte *rkf, byte *stftab)
{
	int i0, i1, v0, v1;
	
	i0=BTM_StfRk_ReadAdRiceB(ctx, rkf);
	i1=(i0*15)>>4;
	v0=stftab[i0];	v1=stftab[i1];
	stftab[i0]=v1;	stftab[i1]=v0;
	return(v0);
}



void BTM_StfRk_WriteBits(BTM_StfRkCtx *ctx, int val, int bits)
{
	ctx->win|=(val&((1U<<bits)-1))<<ctx->pos;
	ctx->pos+=bits;
	while(ctx->pos>=8)
	{
		*ctx->cst++=ctx->win;
		ctx->win>>=8;
		ctx->pos-=8;
	}
}

void BTM_StfRk_FlushBits(BTM_StfRkCtx *ctx)
{
	int p;
	p=ctx->pos;
	while(p>0)
	{
		*ctx->cst++=ctx->win;
		ctx->win>>=8;
		p-=8;
	}
	ctx->pos=0;
}

void BTM_StfRk_WriteAdRiceB(BTM_StfRkCtx *ctx, int val, byte *rkf)
{
	int q, kf, kfb;
	
	kfb=*rkf;
	kf=kfb>>BTM_STFRK_ADBSHL;

	q=val>>kf;

	if(q>=8)
	{
		kfb++; *rkf=kfb;
		BTM_StfRk_WriteBits(ctx, 0xFF, 8);
		BTM_StfRk_WriteBits(ctx, val, 8);
		return;
	}

	BTM_StfRk_WriteBits(ctx, (1<<q)-1, q+1);
	BTM_StfRk_WriteBits(ctx, val&((1<<kf)-1), kf);
	if(!q && kfb)
		{ kfb--; *rkf=kfb; }
	else if(q>1)
		{ kfb++; *rkf=kfb; }
}

void BTM_StfRk_WriteAdRiceSTF(BTM_StfRkCtx *ctx, int val,
	byte *rkf, byte *stfvtab, byte *stfitab)
{
	int i0, i1, v0, v1;
	
	i0=stfitab[val];
	i1=(i0*15)>>4;
	v0=stfvtab[i0];	v1=stfvtab[i1];
	stfvtab[i0]=v1;	stfvtab[i1]=v0;
	stfitab[v1]=i0;	stfitab[v0]=i1;
	
	BTM_StfRk_WriteAdRiceB(ctx, i0, rkf);
}

void BTM_StfRk_EncodeInit(BTM_StfRkCtx *ctx, byte *buf)
{
	ctx->cst=buf;
	ctx->win=0;
	ctx->pos=0;
}

void BTM_StfRk_DecodeInit(BTM_StfRkCtx *ctx, byte *buf)
{
	ctx->cst=buf;
	ctx->win=*(u32 *)buf;
	ctx->pos=0;
}

int BTM_StfRk_CheckPostRp2Blob(byte *ibuf)
{
	if((ibuf[0]==0x10) && (ibuf[1]==0x00))
		return(1);
	return(0);
}

int BTM_StfRk_EncodeBufferPostRp2(byte *obuf, byte *ibuf, int ibsz)
{
	byte prbtab_t[256], prbidx_t[256];
	byte prbtab_d[256], prbidx_d[256];
	byte prbtab_l[256], prbidx_l[256];
	BTM_StfRkCtx t_ctx;
	BTM_StfRkCtx *ctx;
	byte *cs, *cse, *cs1, *csr, *ct;
	byte rk_t, rk_d, rk_l;
	u32 tag;
	int tsz, nr;
	int i, j, k;

	ctx=&t_ctx;

	ct=obuf;
	*ct++=0x10;
	*ct++=0x00;
	
	BTM_StfRk_EncodeInit(ctx, ct);

	for(i=0; i<256; i++)
	{
		prbtab_t[i]=i;	prbidx_t[i]=i;
		prbtab_d[i]=i;	prbidx_d[i]=i;
		prbtab_l[i]=i;	prbidx_l[i]=i;
	}

	rk_t=BTM_STFRK_ADBINIT<<BTM_STFRK_ADBSHL;
	rk_d=BTM_STFRK_ADBINIT<<BTM_STFRK_ADBSHL;
	rk_l=BTM_STFRK_ADBINIT<<BTM_STFRK_ADBSHL;

	cs=ibuf;
	cse=ibuf+ibsz;
	while(cs<cse)
	{
		tag=*(u32 *)cs;
		if(!(tag&0x01))			{ tsz=2; nr=(tag>>1)&7; }
		else if(!(tag&0x02))	{ tsz=3; nr=(tag>>2)&7; }
		else if(!(tag&0x04))	{ tsz=4; nr=(tag>>3)&7; }
		else if(!(tag&0x08))	{ tsz=1; nr=(((tag>>4)&15)+1)*8; }
		else if(!(tag&0x10))	{ tsz=-1; nr=(tag>>5)&7; }
		else if(!(tag&0x20))	{ tsz=1; nr=(tag>>6)&3; }
		else if(!(tag&0x40))	{ tsz=2; nr=(((tag>>7)&511)+1)*8; }
		else if(!(tag&0x80))
		{
			if(tag&0x100)
				{ tsz=6; nr=(tag>>9)&7; }
			else
				{ tsz=2; nr=0; }
		}
		else { tsz=-1; }
		
		if(tsz<1)
			return(-1);
		
		BTM_StfRk_WriteAdRiceSTF(ctx, *cs++, &rk_t, prbtab_t, prbidx_t);
		for(i=1; i<tsz; i++)
			{ BTM_StfRk_WriteAdRiceSTF(ctx, *cs++, &rk_d, prbtab_d, prbidx_d); }
		for(i=0; i<nr; i++)
			{ BTM_StfRk_WriteAdRiceSTF(ctx, *cs++, &rk_l, prbtab_l, prbidx_l); }
	}
	BTM_StfRk_FlushBits(ctx);
	
//	printf("  rk_t=%d rk_d=%d rk_l=%d\n", rk_t>>5, rk_d>>5, rk_l>>5);
	
	return(ctx->cst-obuf);
}

int BTM_StfRk_DecodeBufferPostRp2(byte *obuf, byte *ibuf, int ibsz)
{
	byte prbtab_t[256];
	byte prbtab_d[256];
	byte prbtab_l[256];
	BTM_StfRkCtx t_ctx;
	BTM_StfRkCtx *ctx;
	byte *cs, *cse, *cs1, *csr, *ct;
	byte rk_t, rk_d, rk_l;
	u32 tag, tag1, tag2, tag3;
	int lc, tsz, tsi, nr, tg;
	int i, j, k;

	if((ibuf[0]!=0x10) || (ibuf[1]!=0x00))
		return(-1);

	ctx=&t_ctx;
	BTM_StfRk_DecodeInit(ctx, ibuf+2);
	
	ct=obuf;

	tag=0; tag1=0; tag2=0; tag3=0;

	for(i=0; i<256; i++)
	{
		prbtab_t[i]=i;
		prbtab_d[i]=i;
		prbtab_l[i]=i;
	}
	
	rk_t=BTM_STFRK_ADBINIT<<BTM_STFRK_ADBSHL;
	rk_d=BTM_STFRK_ADBINIT<<BTM_STFRK_ADBSHL;
	rk_l=BTM_STFRK_ADBINIT<<BTM_STFRK_ADBSHL;

	while(1)
	{
		tg=BTM_StfRk_ReadAdRiceSTF(ctx, &rk_t, prbtab_t);
		if(!(tg&0x01))			{ tsz=2; }
		else if(!(tg&0x02))	{ tsz=3; }
		else if(!(tg&0x04))	{ tsz=4; }
		else if(!(tg&0x08))	{ tsz=1; }
		else if(!(tg&0x10))	{ tsz=1; }
		else if(!(tg&0x20))	{ tsz=1; }
		else if(!(tg&0x40))	{ tsz=2; }
		else if(!(tg&0x80))	{ tsz=2; }
		tsi=1;
		
		*ct++=tg;
		if(tg==0x1F)
			break;
		
		tag=tg;

		if(tg==0x7F)
		{
			j=BTM_StfRk_ReadAdRiceSTF(ctx, &rk_d, prbtab_d);
			*ct++=j;
			tag|=j<<8;
			tsi=2;
			if(tag&0x0100)
				tsz=6;
		}
		
		for(i=tsi; i<tsz; i++)
		{
			j=BTM_StfRk_ReadAdRiceSTF(ctx, &rk_d, prbtab_d);
			*ct++=j;
			if(i<4)
				tag|=j<<(i*8);
		}
		
		if(!tag)
			break;
		
		if(!(tag&0x01))			{ tsz=2; nr=(tag>>1)&7; }
		else if(!(tag&0x02))	{ tsz=3; nr=(tag>>2)&7; }
		else if(!(tag&0x04))	{ tsz=4; nr=(tag>>3)&7; }
		else if(!(tag&0x08))	{ tsz=1; nr=(((tag>>4)&15)+1)*8; }
		else if(!(tag&0x10))	{ tsz=1; nr=(tag>>5)&7; }
		else if(!(tag&0x20))	{ tsz=1; nr=(tag>>6)&3; }
		else if(!(tag&0x40))	{ tsz=2; nr=(((tag>>7)&511)+1)*8; }	
		else if(!(tag&0x80))	{ nr=(tag&0x0100)?((tag>>9)&7):0; }	
		for(i=0; i<nr; i++)
			{ *ct++=BTM_StfRk_ReadAdRiceSTF(ctx, &rk_l, prbtab_l); }

		tag3=tag2;
		tag2=tag1;
		tag1=tag;
	}
	return(ct-obuf);
}

int BTM_StfRk_DummyDecodeBufferPostRp2(byte *obuf, byte *ibuf, int ibsz)
{
	byte prbtab_t[256];
	byte prbtab_d[256];
	byte prbtab_l[256];
	BTM_StfRkCtx t_ctx;
	BTM_StfRkCtx *ctx;
	byte *cs, *cse, *cs1, *csr, *ct;
	byte rk_t, rk_d, rk_l;
	u32 tag, tag1, tag2, tag3;
	int lc, tsz, tsi, nr, tg;
	int i, j, k;

	if((ibuf[0]!=0x10) || (ibuf[1]!=0x00))
		return(-1);

	ctx=&t_ctx;
	BTM_StfRk_DecodeInit(ctx, ibuf+2);
	
	ct=obuf;

	tag=0; tag1=0; tag2=0; tag3=0;

	for(i=0; i<256; i++)
	{
		prbtab_t[i]=i;
		prbtab_d[i]=i;
		prbtab_l[i]=i;
	}
	
	rk_t=BTM_STFRK_ADBINIT<<BTM_STFRK_ADBSHL;
	rk_d=BTM_STFRK_ADBINIT<<BTM_STFRK_ADBSHL;
	rk_l=BTM_STFRK_ADBINIT<<BTM_STFRK_ADBSHL;

	while(1)
	{
		tg=BTM_StfRk_ReadAdRiceSTF(ctx, &rk_t, prbtab_t);
		if(!(tg&0x01))			{ tsz=2; }
		else if(!(tg&0x02))	{ tsz=3; }
		else if(!(tg&0x04))	{ tsz=4; }
		else if(!(tg&0x08))	{ tsz=1; }
		else if(!(tg&0x10))	{ tsz=1; }
		else if(!(tg&0x20))	{ tsz=1; }
		else if(!(tg&0x40))	{ tsz=2; }
		else if(!(tg&0x80))	{ tsz=2; }
		tsi=1;
		
//		*ct++=tg;
		if((*ct++)!=tg)
			break;

		if(tg==0x1F)
			break;
		
		tag=tg;

		if(tg==0x7F)
		{
			j=BTM_StfRk_ReadAdRiceSTF(ctx, &rk_d, prbtab_d);
//			*ct++=j;
			if((*ct++)!=j)
				break;
			tag|=j<<8;
			tsi=2;
			if(tag&0x0100)
				tsz=6;
		}
		
		for(i=tsi; i<tsz; i++)
		{
			j=BTM_StfRk_ReadAdRiceSTF(ctx, &rk_d, prbtab_d);
//			*ct++=j;
			if((*ct++)!=j)
				break;
			if(i<4)
				tag|=j<<(i*8);
		}
		
		if(!tag)
			break;
		
		if(!(tag&0x01))			{ tsz=2; nr=(tag>>1)&7; }
		else if(!(tag&0x02))	{ tsz=3; nr=(tag>>2)&7; }
		else if(!(tag&0x04))	{ tsz=4; nr=(tag>>3)&7; }
		else if(!(tag&0x08))	{ tsz=1; nr=(((tag>>4)&15)+1)*8; }
		else if(!(tag&0x10))	{ tsz=1; nr=(tag>>5)&7; }
		else if(!(tag&0x20))	{ tsz=1; nr=(tag>>6)&3; }
		else if(!(tag&0x40))	{ tsz=2; nr=(((tag>>7)&511)+1)*8; }	
		else if(!(tag&0x80))	{ nr=(tag&0x0100)?((tag>>9)&7):0; }	
		for(i=0; i<nr; i++)
		{
			j=BTM_StfRk_ReadAdRiceSTF(ctx, &rk_l, prbtab_l);
//			*ct++=j;
			if((*ct++)!=j)
				break;
		}

		tag3=tag2;
		tag2=tag1;
		tag1=tag;
	}
	return(ct-obuf);
}
