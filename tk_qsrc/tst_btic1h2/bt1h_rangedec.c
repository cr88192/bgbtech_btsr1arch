int BTIC1H_Range_InputByte(BTIC1H_Context *ctx)
{
//	if(ctx->cs>ctx->cse)
//		return(0);
	return(*ctx->bs_cs++);
}

#if 1
force_inline void BTIC1H_Range_NormalizeInput16(BTIC1H_Context *ctx)
{
//	u32 ku;
	int i, j, k;
	j=ctx->rmin^ctx->rmax;
//	j=ctx->rmax-ctx->rmin;
	i=	(!(j>>24))+(!(j>>16));
	j=i<<3;
	k=*(u16 *)ctx->bs_cs;
	k=(u16)((k<<8)|(k>>8));
	ctx->rmin=(ctx->rmin<<j);
//	ctx->rmax=(ctx->rmax<<j)|((1<<j)-1);
	ctx->rmax=((ctx->rmax+1)<<j)-1;
	ctx->rval=(ctx->rval<<j)|(k>>(16-j));
	ctx->bs_cs+=i;
}
#endif

#if 0
force_inline void BTIC1H_Range_NormalizeInput16(BTIC1H_Context *ctx)
{
	if(!((ctx->rmin^ctx->rmax)>>24))
	{
		ctx->rmin<<=8;
		ctx->rmax=(ctx->rmax<<8)|255;
		ctx->rval=(ctx->rval<<8)|(*ctx->bs_cs++);
	}

#if 1
	if(!((ctx->rmin^ctx->rmax)>>24))
	{
		ctx->rmin<<=8;
		ctx->rmax=(ctx->rmax<<8)|255;
		ctx->rval=(ctx->rval<<8)|(*ctx->bs_cs++);
	}
#endif
}
#endif

force_inline void BTIC1H_Range_NormalizeInput8(BTIC1H_Context *ctx)
{
	int i, j, k;
	j=ctx->rmin^ctx->rmax;
	i=(!(j>>24));
	j=i<<3;
	ctx->rmin=(ctx->rmin<<j);
	ctx->rmax=(ctx->rmax<<j)|((1<<j)-1);
	ctx->rval=(ctx->rval<<j)|((*ctx->bs_cs)>>(8-j));
	ctx->bs_cs+=i;
}

void BTIC1H_Range_NormalizeInput(BTIC1H_Context *ctx)
{
#if 1
//	BTIC1H_Range_NormalizeInput8(ctx);
//	BTIC1H_Range_NormalizeInput8(ctx);
	BTIC1H_Range_NormalizeInput16(ctx);

#if 0
	BTIC1H_Range_NormalizeInput16(ctx);
	BTIC1H_Range_NormalizeInput16(ctx);
#endif
#endif

#if 0
	u32 ku;
	int m1, m2;
	int i, j, k;
	j=ctx->rmin^ctx->rmax;
	i=	(!(j>>24))+(!(j>>16))+
		(!(j>> 8))+(! j     );
	j=i<<3;
	ku=*(u32 *)ctx->bs_cs;
	ku=(ku>>24)|(ku<<24)|
		((ku>>8)&0x0000FF00)|
		((ku<<8)&0x00FF0000);
	m1=(j>=32)-1;
	m2=(!j)-1;
	ctx->rmin=((ctx->rmin<<j)&m1);
	ctx->rmax=((ctx->rmax<<j)&m1)|(((1<<j)&m1)-1);
	ctx->rval=((ctx->rval<<j)&m1)|((ku>>(32-j))&m2);
	ctx->bs_cs+=i;
#endif

#if 0
	while(!((ctx->rmin^ctx->rmax)>>24))
//	if(!((ctx->rmin^ctx->rmax)>>24))
	{
		ctx->rmin<<=8;
		ctx->rmax=(ctx->rmax<<8)|255;
		ctx->rval=(ctx->rval<<8)|(*ctx->bs_cs++);
	}
#endif

#if 0
//	while(!((ctx->rmin^ctx->rmax)>>24))
	if(!((ctx->rmin^ctx->rmax)>>24))
	{
		ctx->rmin<<=8;
		ctx->rmax=(ctx->rmax<<8)|255;
		ctx->rval=(ctx->rval<<8)|(*ctx->bs_cs++);
	}
#endif

#if 0
	if((ctx->rmin^ctx->rmax)>>24)
		return;
	ctx->rmin<<=8;
	ctx->rmax=(ctx->rmax<<8)|255;
	ctx->rval=(ctx->rval<<8)|(*ctx->bs_cs++);

	if((ctx->rmin^ctx->rmax)>>24)
		return;
	ctx->rmin<<=8;
	ctx->rmax=(ctx->rmax<<8)|255;
	ctx->rval=(ctx->rval<<8)|(*ctx->bs_cs++);

	if((ctx->rmin^ctx->rmax)>>24)
		return;
	ctx->rmin<<=8;
	ctx->rmax=(ctx->rmax<<8)|255;
	ctx->rval=(ctx->rval<<8)|(*ctx->bs_cs++);

	if((ctx->rmin^ctx->rmax)>>24)
		return;
	ctx->rmin<<=8;
	ctx->rmax=(ctx->rmax<<8)|255;
	ctx->rval=(ctx->rval<<8)|(*ctx->bs_cs++);
#endif
}

int BTIC1H_Range_InputFixedBit(BTIC1H_Context *ctx)
{
	u32 r, r2, v, i;

	r=ctx->rmax-ctx->rmin;
	v=ctx->rmin+(r>>1);

	i=(ctx->rval>v);
	if(i)ctx->rmin=v+1;
	else ctx->rmax=v;

	BTIC1H_Range_NormalizeInput(ctx);
	return(i);
}

int BTIC1H_Range_InputBit(BTIC1H_Context *ctx, u32 w)
{
	u32 r, r2, v, i;

	r=ctx->rmax-ctx->rmin;
	v=ctx->rmin+(r>>BTLZA_BITARITH_WBITS)*w;

	i=(ctx->rval>v);
	if(i)ctx->rmin=v+1;
	else ctx->rmax=v;

	BTIC1H_Range_NormalizeInput(ctx);
	return(i);
}

#if 1
int BTIC1H_Range_InputModelBit(BTIC1H_Context *ctx, byte *mctx)
{
	u32 r, v, w;
	int i, j, k;

	r=ctx->rmax-ctx->rmin; w=*mctx;
	v=ctx->rmin+(r>>BTLZA_BITARITH_WBITS)*w;

	i=(ctx->rval>v);
	if(i)
	{
		ctx->rmin=v+1;
		w=btic1h_range_weightadjust[256+w];
		*mctx=w;
	}else
	{
		ctx->rmax=v;
		w=btic1h_range_weightadjust[w];
		*mctx=w;
	}

	BTIC1H_Range_NormalizeInput(ctx);
	return(i);
}
#endif

#if 0
int BTIC1H_Range_InputModelBit(BTIC1H_Context *ctx, byte *mctx)
{
	u32 r, v, w, m, n;
	int i, j, k;

	m=ctx->rmin; n=ctx->rmax;
	r=n-m; w=*mctx;
	v=m+(r>>BTLZA_BITARITH_WBITS)*w;

#if 0
	i=(ctx->rval>v);
	if(i)
	{
		ctx->rmin=v+1;
		ctx->rmax=n;
	}else
	{
		ctx->rmin=m;
		ctx->rmax=v;
	}
#endif

	i=(ctx->rval>v);
	j=i-1; k=~j;
	ctx->rmin=((v+1)&k)|(m&j);
	ctx->rmax=((n  )&k)|(v&j);

	w=btic1h_range_weightadjust[(i<<8)+w];
	*mctx=w;

	BTIC1H_Range_NormalizeInput(ctx);
	return(i);
}
#endif

#if 1
int BTIC1H_Range_InputModelBitN(BTIC1H_Context *ctx, byte *mctx)
{
	u32 r, v, w;
	int i, j, k;

	r=ctx->rmax-ctx->rmin; w=*mctx;
	v=ctx->rmin+(r>>BTLZA_BITARITH_WBITS)*w;

	i=(ctx->rval>v);
	if(i)
	{
		ctx->rmin=v+1;
		w=btic1h_range_weightadjust[256+w];
		*mctx=w;
	}else
	{
		ctx->rmax=v;
		w=btic1h_range_weightadjust[w];
		*mctx=w;
	}

//	BTIC1H_Range_NormalizeInput(ctx);
	return(i);
}
#endif


int BTIC1H_Range_InputFixedBits(BTIC1H_Context *ctx, int n)
{
	int i, j, k, l;

	i=n; j=0;
	while(i--)
	{
		k=BTIC1H_Range_InputFixedBit(ctx);
		j=(j<<1)|k;
	}
	return(j);
}

int BTIC1H_Range_InputContextFixedBits(BTIC1H_Context *ctx, int n)
{
	int i, j, k, l;

	i=n; j=ctx->wctx;
	while(i--)
	{
		k=BTIC1H_Range_InputFixedBit(ctx);
		j=(j<<1)|k;
	}
	ctx->wctx=j;
	return(j&((1<<n)-1));
}

int BTIC1H_Range_InputContextModelBits(BTIC1H_Context *ctx,
	int n, byte *mdl, int ctxmask)
{
	int i, j, k, l;

	i=n; j=ctx->wctx; k=ctxmask;
	while(i--)
	{
		l=BTIC1H_Range_InputModelBit(ctx, mdl+(j&k));
		j=(j<<1)|l;
	}
	ctx->wctx=j;
	return(j&((1<<n)-1));
}

int BTIC1H_Range_InputContextModelBitsN4(BTIC1H_Context *ctx,
	int n, byte *mdl, int ctxmask)
{
	int i, j, k, l;

	i=n; j=ctx->wctx; k=ctxmask;
	
	while(i>=4)
	{
		l=BTIC1H_Range_InputModelBitN(ctx, mdl+(j&k));
		j=(j<<1)|l;
		l=BTIC1H_Range_InputModelBitN(ctx, mdl+(j&k));
		j=(j<<1)|l;
		l=BTIC1H_Range_InputModelBitN(ctx, mdl+(j&k));
		j=(j<<1)|l;
		l=BTIC1H_Range_InputModelBitN(ctx, mdl+(j&k));
		j=(j<<1)|l;
		BTIC1H_Range_NormalizeInput(ctx);
		i-=4;
	}
	
	if(i>0)
	{
		while(i--)
		{
			l=BTIC1H_Range_InputModelBitN(ctx, mdl+(j&k));
			j=(j<<1)|l;
		}
		BTIC1H_Range_NormalizeInput(ctx);
	}

	ctx->wctx=j;
	return(j&((1<<n)-1));
}

int BTIC1H_Range_SetupDecode(BTIC1H_Context *ctx, byte *in, int sz)
{
	int i;

	ctx->bs_cs=in;
	ctx->bs_cse=in+sz;
	ctx->rmin=BTLZA_BITARITH_LOWER;
	ctx->rmax=BTLZA_BITARITH_UPPER;
//	ctx->range=BTLZA_BITARITH_UPPER;

	ctx->rval=BTLZA_BITARITH_LOWER;
	for(i=0; i<4; i++)
		ctx->rval=(ctx->rval<<8)|BTIC1H_Range_InputByte(ctx);
	return(0);
}

int BTIC1H_Range_SetupBeginDecode(BTIC1H_Context *ctx)
{
	int i;

	if(ctx->rmax)
	{
		return(0);
	}

	ctx->rmin=BTLZA_BITARITH_LOWER;
	ctx->rmax=BTLZA_BITARITH_UPPER;
//	ctx->range=BTLZA_BITARITH_UPPER;

	ctx->rval=BTLZA_BITARITH_LOWER;
	for(i=0; i<4; i++)
		ctx->rval=(ctx->rval<<8)|BTIC1H_Range_InputByte(ctx);
	return(0);
}

int BTIC1H_Range_SetupEndDecode(BTIC1H_Context *ctx)
{
	ctx->rmin=0;
	ctx->rmax=0;
	return(0);
}

int BTIC1H_Range_DecodeSymbol8(BTIC1H_Context *ctx,
	byte *mdl, int ctxmask)
{
	int i, j, k, v;

	j=ctx->wctx; k=ctxmask;

#ifndef BT1H_RANGE_USEN4S8
	i=BTIC1H_Range_InputModelBit(ctx, mdl+(j&k)); j=(j<<1)|i;
	i=BTIC1H_Range_InputModelBit(ctx, mdl+(j&k)); j=(j<<1)|i;
	i=BTIC1H_Range_InputModelBit(ctx, mdl+(j&k)); j=(j<<1)|i;
	i=BTIC1H_Range_InputModelBit(ctx, mdl+(j&k)); j=(j<<1)|i;
	i=BTIC1H_Range_InputModelBit(ctx, mdl+(j&k)); j=(j<<1)|i;
	i=BTIC1H_Range_InputModelBit(ctx, mdl+(j&k)); j=(j<<1)|i;
	i=BTIC1H_Range_InputModelBit(ctx, mdl+(j&k)); j=(j<<1)|i;
	i=BTIC1H_Range_InputModelBit(ctx, mdl+(j&k)); j=(j<<1)|i;
#endif

#ifdef BT1H_RANGE_USEN4S8
	i=BTIC1H_Range_InputModelBitN(ctx, mdl+(j&k)); j=(j<<1)|i;
	i=BTIC1H_Range_InputModelBitN(ctx, mdl+(j&k)); j=(j<<1)|i;
	i=BTIC1H_Range_InputModelBitN(ctx, mdl+(j&k)); j=(j<<1)|i;
	i=BTIC1H_Range_InputModelBitN(ctx, mdl+(j&k)); j=(j<<1)|i;
	BTIC1H_Range_NormalizeInput(ctx);
	
	i=BTIC1H_Range_InputModelBitN(ctx, mdl+(j&k)); j=(j<<1)|i;
	i=BTIC1H_Range_InputModelBitN(ctx, mdl+(j&k)); j=(j<<1)|i;
	i=BTIC1H_Range_InputModelBitN(ctx, mdl+(j&k)); j=(j<<1)|i;
	i=BTIC1H_Range_InputModelBitN(ctx, mdl+(j&k)); j=(j<<1)|i;
	BTIC1H_Range_NormalizeInput(ctx);
#endif

	ctx->wctx=j;
	return(j&0xFF);
}

#if 1
int BTIC1H_Range_DecodeSymbol1(
	BTIC1H_Context *ctx, byte *mdl, int ctxmask)
{
	int i, j, k, l;

	j=ctx->wctx; k=ctxmask;
	l=BTIC1H_Range_InputModelBit(ctx, mdl+(j&k));
	j=(j<<1)|l;
	ctx->wctx=j;
	return(j&1);
}
#endif

int BTIC1H_Rice_NextByteRange(BTIC1H_Context *ctx)
{
	return(BTIC1H_Range_DecodeSymbol8(ctx,
		ctx->rc_mdl_raw, ctx->rc_msk_raw));
}

int BTIC1H_Rice_ReadNBitsRange(BTIC1H_Context *ctx, int n)
{
#ifdef BT1H_RANGE_USEN4
	return(BTIC1H_Range_InputContextModelBitsN4(
		ctx, n, ctx->rc_mdl_raw, ctx->rc_msk_raw));
#else
	return(BTIC1H_Range_InputContextModelBits(
		ctx, n, ctx->rc_mdl_raw, ctx->rc_msk_raw));
#endif
}

void BTIC1H_Rice_SkipNBitsRange(BTIC1H_Context *ctx, int n)
{
#ifdef BT1H_RANGE_USEN4
	BTIC1H_Range_InputContextModelBitsN4(
		ctx, n, ctx->rc_mdl_raw, ctx->rc_msk_raw);
#else
	BTIC1H_Range_InputContextModelBits(
		ctx, n, ctx->rc_mdl_raw, ctx->rc_msk_raw);
#endif
}

int BTIC1H_Rice_ReadNBitsRangeRc(BTIC1H_Context *ctx, int n)
{
#ifdef BT1H_RANGE_USEN4
	return(BTIC1H_Range_InputContextModelBitsN4(
		ctx, n, ctx->rc_mdl_rc, ctx->rc_msk_raw));
#else
	return(BTIC1H_Range_InputContextModelBits(
		ctx, n, ctx->rc_mdl_rc, ctx->rc_msk_raw));
#endif
}

void BTIC1H_Rice_SkipNBitsRangeRc(BTIC1H_Context *ctx, int n)
{
#ifdef BT1H_RANGE_USEN4
	BTIC1H_Range_InputContextModelBitsN4(
		ctx, n, ctx->rc_mdl_rc, ctx->rc_msk_raw);
#else
	BTIC1H_Range_InputContextModelBits(
		ctx, n, ctx->rc_mdl_rc, ctx->rc_msk_raw);
#endif
}

int BTIC1H_Rice_Read8BitsRange(BTIC1H_Context *ctx)
{
	return(BTIC1H_Range_DecodeSymbol8(ctx,
		ctx->rc_mdl_raw, ctx->rc_msk_raw));
}

int BTIC1H_Rice_Read16BitsRange(BTIC1H_Context *ctx)
{
	int i, j, k;
	i=BTIC1H_Range_DecodeSymbol8(ctx,
		ctx->rc_mdl_raw, ctx->rc_msk_raw);
	j=BTIC1H_Range_DecodeSymbol8(ctx,
		ctx->rc_mdl_raw, ctx->rc_msk_raw);
	k=(i<<8)|j;
	return(k);
}

#if 1
int BTIC1H_Rice_ReadBitRange(BTIC1H_Context *ctx)
{
	return(BTIC1H_Range_DecodeSymbol1(ctx,
		ctx->rc_mdl_rc, ctx->rc_msk_raw));
}
#endif

default_inline int BTIC1H_Rice_ReadRiceRangeQ(BTIC1H_Context *ctx)
{
#ifdef BT1H_RANGE_USEN4
	byte *mdl;
	int i, j, k, l;

	mdl=ctx->rc_mdl_rc;
	i=0; j=ctx->wctx; k=ctx->rc_msk_raw;
	while(1)
	{
		l=BTIC1H_Range_InputModelBitN(ctx, mdl+(j&k));
		j=(j<<1)|l;
		if(!l)break;
		
		i++;
		if(!(i&3))
		{
			BTIC1H_Range_NormalizeInput(ctx);
		}
	}
	BTIC1H_Range_NormalizeInput(ctx);

	ctx->wctx=j;
	return(i);
#endif

#ifndef BT1H_RANGE_USEN4
	int i, j, k;

	i=0;
	while(BTIC1H_Rice_ReadBitRange(ctx))
		i++;
	return(i);
#endif
}

int BTIC1H_Rice_ReadAdRiceRange(BTIC1H_Context *ctx, int *rk)
{
	int i, j, k;
	
	k=*rk;
	i=BTIC1H_Rice_ReadRiceRangeQ(ctx);
	j=BTIC1H_Rice_ReadNBitsRangeRc(ctx, k);
	j=(i<<k)|j;

#if 1
	if(i!=1)
	{
		if(i>1)
		{
			while(i>1)
				{ k++; i=i>>1; }
			if(k>15)k=15;
		}else
			{ if(k>0)k--; }
		*rk=k;
	}
#endif

	return(j);
}

int BTIC1H_Rice_ReadAdSRiceRange(BTIC1H_Context *ctx, int *rk)
{
	int i;
	i=BTIC1H_Rice_ReadAdRiceRange(ctx, rk);
	i=(i>>1)^((i<<31)>>31);
	return(i);
}

int BTIC1H_Rice_ReadAdRiceDcRange(BTIC1H_Context *ctx, int *rk)
	{ return(BTIC1H_Rice_ReadAdRiceRange(ctx, rk)); }
int BTIC1H_Rice_ReadAdSRiceDcRange(BTIC1H_Context *ctx, int *rk)
	{ return(BTIC1H_Rice_ReadAdSRiceRange(ctx, rk)); }

int BTIC1H_Rice_SetupReadRange(
	BTIC1H_Context *ctx, byte *buf, int szbuf)
{
	BTIC1H_Rice_InitTables();

	ctx->NextByte=BTIC1H_Rice_NextByteRange;
	ctx->ReadNBits=BTIC1H_Rice_ReadNBitsRange;
	ctx->Read8Bits=BTIC1H_Rice_Read8BitsRange;
	ctx->Read16Bits=BTIC1H_Rice_Read16BitsRange;
	ctx->SkipNBits=BTIC1H_Rice_SkipNBitsRange;
	ctx->ReadAdRice=BTIC1H_Rice_ReadAdRiceRange;
	ctx->ReadAdSRice=BTIC1H_Rice_ReadAdSRiceRange;
	ctx->ReadAdRiceDc=BTIC1H_Rice_ReadAdRiceDcRange;
	ctx->ReadAdSRiceDc=BTIC1H_Rice_ReadAdSRiceDcRange;

	BTIC1H_Range_SetupDecode(ctx, buf, szbuf);
	return(0);
}

int BTIC1H_DecodeReadVLI(BTIC1H_Context *ctx, byte **rcs)
{
	byte *cs;
	int i;
	
	cs=*rcs;
	i=*cs++;
	
	if(i<0x80)
	{
	}else if(i<0xC0)
	{
		i=i&0x3F;
		i=(i<<8)|(*cs++);
	}else if(i<0xE0)
	{
		i=i&0x1F;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
	}else if(i<0xF0)
	{
		i=i&0x0F;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
	}else if(i<0xF8)
	{
		i=i&0x07;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
	}
	
	*rcs=cs;
	return(i);
}

int BTIC1H_Rice_SetupRead2(
	BTIC1H_Context *ctx, byte *buf, int szbuf)
{
	byte *cs, *cse, *cs1, *cs2;
	int cim, cxbits;
	int i, j, k, l;
	
	cs=buf; cse=buf+szbuf;
	
	i=*cs++;
	cim=i&15;
	cxbits=8+((i>>4)&7);
	
	ctx->slscl=0;
	ctx->clryuv=0;
	ctx->clrdty=0;

	while(i&128)
	{
		i=cs[0];
		l=(i>>4)&7;
//		if(!l)l=cs[1];
//		cs1=cs+l;

		j=i&15;
		cs1=cs+1;
		if(l==0)
			{ l=BTIC1H_DecodeReadVLI(ctx, &cs1); }
		if(j==0)
			{ j=BTIC1H_DecodeReadVLI(ctx, &cs1); }
		cs2=cs1;
		cs1=cs+l;

//		switch(i&15)
		switch(j)
		{
		case 1:
			ctx->slscl=BTIC1H_DecodeReadVLI(ctx, &cs2)*4;
			break;
		case 2:
			j=BTIC1H_DecodeReadVLI(ctx, &cs2);
			ctx->clryuv=j&7;
			ctx->clrdty=(j>>3)&7;
			break;
		default:
			break;
		}
		
		cs=cs1;
	}

	switch(cim)
	{
	case 0:
		BTIC1H_Rice_SetupRead(ctx, cs, cse-cs);
		break;
	case 1:
		BTIC1H_Range_SetupContextBits(ctx, cxbits);
		BTIC1H_Rice_SetupReadRange(ctx, cs, cse-cs);
		break;
	default:
		break;
	}
	return(0);
}
