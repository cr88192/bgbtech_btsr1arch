void BTIC1H_Rice_WriteNBitsRange(BTIC1H_Context *ctx, int v, int n)
{
#ifdef BT1H_RANGE_USEN4
	BTIC1H_Range_OutputContextModelBitsN4(
		ctx, v, n, ctx->rc_mdl_raw, ctx->rc_msk_raw);
#else
	BTIC1H_Range_OutputContextModelBits(
		ctx, v, n, ctx->rc_mdl_raw, ctx->rc_msk_raw);
#endif
	ctx->bs_bits+=n;
}

void BTIC1H_Rice_WriteNBitsRangeRc(BTIC1H_Context *ctx, int v, int n)
{
#ifdef BT1H_RANGE_USEN4
	BTIC1H_Range_OutputContextModelBitsN4(
		ctx, v, n, ctx->rc_mdl_rc, ctx->rc_msk_raw);
#else
	BTIC1H_Range_OutputContextModelBits(
		ctx, v, n, ctx->rc_mdl_rc, ctx->rc_msk_raw);
#endif
	ctx->bs_bits+=n;
}

void BTIC1H_Rice_Write8BitsRange(BTIC1H_Context *ctx, int v)
{
//	BTIC1H_Range_OutputFixedBits(ctx, v, 8);
	BTIC1H_Range_EncodeSymbol8(
		ctx, v, ctx->rc_mdl_raw, ctx->rc_msk_raw);
	ctx->bs_bits+=8;
}

void BTIC1H_Rice_Write16BitsRange(BTIC1H_Context *ctx, int v)
{
//	BTIC1H_Range_OutputFixedBits(ctx, v, 16);
	BTIC1H_Range_EncodeSymbol8(
		ctx, (v>>8)&255, ctx->rc_mdl_raw, ctx->rc_msk_raw);
	BTIC1H_Range_EncodeSymbol8(
		ctx, v&255, ctx->rc_mdl_raw, ctx->rc_msk_raw);
	ctx->bs_bits+=16;
}

void BTIC1H_Rice_WriteAdRiceRange(BTIC1H_Context *ctx, int v, int *rk)
{
	int p, b, n, bp, bw;
	int i, j, k;
	
	k=*rk;
	
	p=v>>k;

	j=p;

	while(j>=16)
		{ BTIC1H_Rice_WriteNBitsRangeRc(ctx, 0xFFFF, 16); j-=16; }
	BTIC1H_Rice_WriteNBitsRangeRc(ctx, 0xFFFE, j+1);

	BTIC1H_Rice_WriteNBitsRangeRc(ctx, v, k);

#if 1
	if(p!=1)
	{
		if(p>1)
		{
			j=p;
			while(j>1)
				{ k++; j>>=1; }
		}else
		{
			if(k>0)k--;
		}
		*rk=k;
	}
#endif
}

void BTIC1H_Rice_WriteAdSRiceRange(BTIC1H_Context *ctx, int v, int *rk)
{
	BTIC1H_Rice_WriteAdRiceRange(ctx, (v<<1)^(v>>31), rk);
}


void BTIC1H_Rice_WriteAdRiceDcRange(BTIC1H_Context *ctx, int v, int *rk)
{
	int p, b, n, bp, bw;
	int i, j, k;
	
	k=*rk;

	p=v>>k;

	if(p>=(8+k))
	{
		j=v; i=1;
		while(j>=(1<<(5+k)))
			{ j=j>>(5+k); i++; }

		BTIC1H_Rice_WriteNBitsRangeRc(ctx, 0xFFFFFFFEUL, 8+k+i);
		BTIC1H_Rice_WriteNBitsRangeRc(ctx, v, i*(5+k));

//		BTIC1H_Rice_WriteNBits(ctx, 0xFFFE, 8+i);
//		BTIC1H_Rice_WriteNBits(ctx, v, i*5);
		k+=2*i;
		if(k>15)k=15;
		*rk=k;
		return;
	}

#if 0
	i=p+k+1;
	if(i<=16)
	{
		j=v&((1<<k)-1);
		BTIC1H_Rice_WriteNBitsRangeRc(ctx, (0xFFFE<<k)|j, i);

		if(p!=1)
		{
			if(!p)
				{ if(k>0)k--; }
			else if(p>1)
			{
				j=p;
				while(j>1)
					{ k++; j>>=1; }
				if(k>15)k=15;
			}
			*rk=k;
		}
		return;
	}
#endif

	j=p;

	while(j>=16)
		{ BTIC1H_Rice_WriteNBitsRangeRc(ctx, 0xFFFF, 16); j-=16; }
	BTIC1H_Rice_WriteNBitsRangeRc(ctx, 0xFFFE, j+1);

	BTIC1H_Rice_WriteNBitsRangeRc(ctx, v, k);

	if(p!=1)
	{
		if(p>1)
		{
			j=p;
			while(j>1)
				{ k++; j>>=1; }
			if(k>15)k=15;
		}else
		{
			if(k>0)k--;
		}
		*rk=k;
	}
}

void BTIC1H_Rice_WriteAdSRiceDcRange(BTIC1H_Context *ctx, int v, int *rk)
{
	BTIC1H_Rice_WriteAdRiceDcRange(ctx, (v<<1)^(v>>31), rk);
}


void BTIC1H_Rice_FlushBitsRange(BTIC1H_Context *ctx)
{
	BTIC1H_Range_FlushWBits(ctx);
}


void BTIC1H_Rice_SetupWriteRange(BTIC1H_Context *ctx, byte *obuf, int osz)
{
	BTIC1H_Range_SetupEncode(ctx, obuf, osz);
	ctx->bs_bits=0;

	ctx->WriteNBits=BTIC1H_Rice_WriteNBitsRange;
	ctx->Write8Bits=BTIC1H_Rice_Write8BitsRange;
	ctx->Write16Bits=BTIC1H_Rice_Write16BitsRange;
	ctx->WriteAdRice=BTIC1H_Rice_WriteAdRiceRange;
	ctx->WriteAdSRice=BTIC1H_Rice_WriteAdSRiceRange;
	ctx->WriteAdRiceDc=BTIC1H_Rice_WriteAdRiceDcRange;
	ctx->WriteAdSRiceDc=BTIC1H_Rice_WriteAdSRiceDcRange;
	ctx->FlushBits=BTIC1H_Rice_FlushBitsBasic;
}

void BTIC1H_Rice_SetupWriteRangeBits(BTIC1H_Context *ctx,
	byte *obuf, int osz, int bits)
{
	BTIC1H_Range_SetupContextBits(ctx, bits);
	BTIC1H_Rice_SetupWriteRange(ctx, obuf, osz);
}
