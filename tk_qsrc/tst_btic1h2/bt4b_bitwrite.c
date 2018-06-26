// #define BTIC4B_DBG_NOSMTF

static int lqtvq_encrice8[16*512];		//0-15=bits, 16-19=len, 20-23='Rk

//static int lqtvq_decrice8[16*256];		//0-15=sym, 16-19=len, 20-23='Rk
static u16 lqtvq_decrice8[16*256];		//0-15=sym, 16-19=len, 20-23='Rk
static byte lqtvq_decriceq8[256];		//Q table
static byte lqtvq_decricenk8[256];		//Next K Table

static byte lqtvq_initrice=0;

void BTIC4B_InitRice(void)
{
	int q;
	int i, j, k, k1, l;

	if(lqtvq_initrice)
		return;
	lqtvq_initrice=1;

	for(i=0; i<256; i++)
	{
		for(j=0; (i>>j)&1; j++);
		lqtvq_decriceq8[i]=j;
	}

	for(i=0; i<256; i++)
	{
		k=(i>>4)&15; q=i&15;
		k1=k;
		if(!q)
		{
			if(k1>0)k1--;
		}else if(q>1)
		{
			j=q;
			while(j>1)
				{ k1++; j=j>>1; }
			if(k1>15)k1=15;
		}
		lqtvq_decricenk8[i]=k1;
	}
		
	for(k=0; k<16; k++)
	{
		for(i=0; i<512; i++)
		{
			q=i>>k;
			l=q+k+1;
			
			k1=k;
			if(!q)
			{
				if(k1>0)k1--;
			}else if(q>1)
			{
				j=q;
				while(j>1)
					{ k1++; j=j>>1; }
				if(k1>15)k1=15;
			}

#if 0
			j=((1<<q)-1)|((i&((1<<k)-1))<<(q+1));
			if(l<=8)
			{
				lqtvq_decrice8[(k<<8)|(j&255)]=i|(l<<16)|(k1<<20);
			}
			else
				{ lqtvq_decrice8[(k<<8)|(j&255)]=0; }
#endif

			if((l>=16) || (q>=8))
			{
				lqtvq_encrice8[(k<<9)|(i&511)]=0;
				continue;
			}
			
			j=((1<<q)-1)|((i&((1<<k)-1))<<(q+1));
			lqtvq_encrice8[(k<<9)|(i&511)]=j|(l<<16)|(k1<<20);
		}

		for(i=0; i<256; i++)
		{
			q=lqtvq_decriceq8[i];
			l=q+k+1;
			if(l>8)
			{
				lqtvq_decrice8[(k<<8)|i]=0;
				continue;
			}
			k1=lqtvq_decricenk8[(k<<4)|(q&15)];
			j=(i>>(q+1))&((1<<k)-1);
			j=j|(q<<k);
//			lqtvq_decrice8[(k<<8)|i]=j|(l<<16)|(k1<<20);
			lqtvq_decrice8[(k<<8)|i]=j|(l<<8)|(k1<<12);
		}
	}
}

void BTIC4B_SetupWriteBits(BTIC4B_Context *ctx, byte *buf, int sz)
{
	ctx->ct=buf;
	ctx->bit_win=0;
	ctx->bit_pos=0;
	
//	ctx->stat_bits=0;
}

byte *BTIC4B_EndWriteBits(BTIC4B_Context *ctx)
{
	u32 bw;
	int bp;
	int i;
	
	bp=ctx->bit_pos;
	bw=ctx->bit_win;

	i=(bp+7)>>3;
	*(u32 *)ctx->ct=bw;
	ctx->ct+=i;
	ctx->bit_win=(bw>>(i<<3));
	ctx->bit_pos=0;
	return(ctx->ct);
}

void BTIC4B_WriteNBits(BTIC4B_Context *ctx,
	int bits, int len)
{
	u32 bw;
	int bp;
	int i;
	
	bits&=(1<<len)-1;
	bp=ctx->bit_pos;
	bw=ctx->bit_win;
	bw=bw|(bits<<bp);
	bp=bp+len;

	i=bp>>3;
	*(u32 *)ctx->ct=bw;
	ctx->ct+=i;
	ctx->bit_win=(bw>>(i<<3));
	ctx->bit_pos=bp&7;
	
	ctx->stat_bits+=len;
}

void BTIC4B_WriteNBitsNM(BTIC4B_Context *ctx,
	int bits, int len)
{
//	BTIC4B_WriteNBits(ctx, bits, len);

#if 1
	u32 bw;
	int bp;
//	int i;
	
	bp=ctx->bit_pos;
//	bw=ctx->bit_win;
//	bw=bw|(bits<<bp);
	bw=ctx->bit_win|(bits<<bp);
	bp=bp+len;

//	i=bp>>3;
	*(u32 *)ctx->ct=bw;
//	ctx->ct+=i;
//	ctx->bit_win=(bw>>(i<<3));
	ctx->ct+=bp>>3;
	ctx->bit_win=(bw>>(bp&(~7)));
	ctx->bit_pos=bp&7;
	
	ctx->stat_bits+=len;
#endif
}

#if 0
void BTIC4B_Write8BitsNM(BTIC4B_Context *ctx, int bits)
{
	BTIC4B_WriteNBits(ctx, bits, 8);
}

void BTIC4B_Write16BitsNM(BTIC4B_Context *ctx, int bits)
{
	BTIC4B_WriteNBits(ctx, bits, 16);
}

void BTIC4B_Write24BitsNM(BTIC4B_Context *ctx, int bits)
{
	BTIC4B_WriteNBits(ctx, bits, 24);
}
#endif

#if 1
void BTIC4B_Write8BitsNM(BTIC4B_Context *ctx, int bits)
{
	u32 bw;
	bw=ctx->bit_win|(bits<<ctx->bit_pos);
	*ctx->ct++=bw;
	ctx->bit_win=(bw>>8);
	ctx->stat_bits+=8;
}

void BTIC4B_Write16BitsNM(BTIC4B_Context *ctx, int bits)
{
	u32 bw;
	bw=ctx->bit_win|(bits<<ctx->bit_pos);
	*(u32 *)ctx->ct=bw;
	ctx->ct+=2;
	ctx->bit_win=(bw>>16);

	ctx->stat_bits+=16;
}

void BTIC4B_Write24BitsNM(BTIC4B_Context *ctx, int bits)
{
	u32 bw;
	bw=ctx->bit_win|(bits<<ctx->bit_pos);
	*(u32 *)ctx->ct=bw;
	ctx->ct+=3;
	ctx->bit_win=(bw>>24);

	ctx->stat_bits+=24;
}
#endif

void BTIC4B_Write32Bits(BTIC4B_Context *ctx, u32 bits)
{
	BTIC4B_Write16BitsNM(ctx, (u16)bits);
	BTIC4B_Write16BitsNM(ctx, bits>>16);
}

void BTIC4B_Write64Bits(BTIC4B_Context *ctx, u64 bits)
{
	BTIC4B_Write32Bits(ctx, (u32)bits);
	BTIC4B_Write32Bits(ctx, (u32)(bits>>32));
}

void BTIC4B_WriteAdRiceILL(BTIC4B_Context *ctx, int val, byte *rk)
{
	int i, j, k, l;

	k=*rk;
	i=val>>k;

#if 1
	if(i>=8)
	{
		i=8; j=val>>5; k+=3;
		while(j>0)
			{ i++; j=j>>3; k++; }
		if(k>=15)k=15;
		BTIC4B_WriteNBits(ctx, (1<<i)-1, i+1);
		BTIC4B_WriteNBits(ctx, val, 5+(i-8)*3);
		*rk=k;
		return;
	}
#endif
	
	BTIC4B_WriteNBits(ctx, (1<<i)-1, i+1);
	BTIC4B_WriteNBits(ctx, val, k);
	
	if(!i)
	{
		if(k>0)k--;
		*rk=k;
	}else if(i>1)
	{
		j=i;
		while(j>1)
			{ k++; j=j>>1; }
		if(k>15)k=15;
		*rk=k;
	}
}

void BTIC4B_WriteAdRiceLL(BTIC4B_Context *ctx, int val, byte *rk)
{
	int i, j, k, l;

#if 1
	if(!(val>>9))
	{
		j=lqtvq_encrice8[(*rk<<9)|val];
		if(j)
		{
			BTIC4B_WriteNBitsNM(ctx, (u16)j, (j>>16)&15);
			*rk=(j>>20)&15;
			return;
		}
	}
#endif

	BTIC4B_WriteAdRiceILL(ctx, val, rk);	
}

void BTIC4B_WriteAdRiceSymLL(BTIC4B_Context *ctx, int val, byte *rk)
{
	int i, j, k, l;

#if 1
	j=lqtvq_encrice8[(*rk<<9)|val];
	if(j)
	{
		BTIC4B_WriteNBitsNM(ctx, (u16)j, (j>>16)&15);
		*rk=(j>>20)&15;
		return;
	}
#endif

	BTIC4B_WriteAdRiceILL(ctx, val, rk);	
}

void BTIC4B_WriteAdSRiceLL(BTIC4B_Context *ctx, int val, byte *rk)
{
	BTIC4B_WriteAdRiceLL(ctx, (val<<1)^(val>>31), rk);
}

void BTIC4B_WriteAdSRiceLLb(
	BTIC4B_Context *ctx, int val, byte *rk, int *rst)
{
	int i0, i1;
	i0=ctx->stat_bits;
	BTIC4B_WriteAdRiceLL(ctx, (val<<1)^(val>>31), rk);
	i1=ctx->stat_bits;
	*rst+=i1-i0;
}

void BTIC4B_ResetSmtfDefault(BTIC4B_Context *ctx, BTIC4B_SmtfState *st)
{
	int i;
	
	for(i=0; i<256; i++)
		{ st->tab[i]=i; st->idx[i]=i; }
	st->rov=0;	st->rk=2;
}

void BTIC4B_ResetSmtfReverse(BTIC4B_Context *ctx, BTIC4B_SmtfState *st)
{
	int i, j;
	
	for(i=0; i<256; i++)
		{ j=255-i; st->tab[i]=j; st->idx[j]=i; }
	st->rov=0;	st->rk=2;
}

int BTIC4B_EmitIndexSymbolSmtf(BTIC4B_Context *ctx,
	BTIC4B_SmtfState *st, int val)
{
	int i0, i1, i2, i3;
	int i;

#if 0
	i=(byte)((st->idx[val])-(st->rov));
	if(!i)
		{ return(i); }
	if(i<32)
	{
		i0=(byte)(st->rov+i);		i1=(byte)(st->rov+i-1);
		i2=st->tab[i0];		i3=st->tab[i1];
		st->tab[i0]=i3;		st->tab[i1]=i2;
		st->idx[i2]=i1;		st->idx[i3]=i0;
		return(i);
	}

	i0=(byte)(st->rov+i);	i1=(byte)(st->rov-1);
	i2=st->tab[i0];		i3=st->tab[i1];
	st->tab[i0]=i3;		st->tab[i1]=i2;
	st->idx[i2]=i1;		st->idx[i3]=i0;
	st->rov--;
	return(i);
#endif

#if 1
	i=st->idx[val];
	i0=i;
//	i1=i>>1;
//	i1=(i*3)>>2;
	i1=(i*7)>>3;
	i2=st->tab[i0];		i3=st->tab[i1];
	st->tab[i0]=i3;		st->tab[i1]=i2;
	st->idx[i2]=i1;		st->idx[i3]=i0;
	return(i);
#endif
}

void BTIC4B_WriteSymbolSmtfI(BTIC4B_Context *ctx,
	BTIC4B_SmtfState *st, int val)
{
	int i0, i1, i2, i3;
	int i;

#ifdef BTIC4B_DBG_NOSMTF
	BTIC4B_WriteAdRiceSymLL(ctx, val, &(st->rk));
	return;
#endif

#if 0
	i=(byte)((st->idx[val])-(st->rov));
	if(!i)
	{
		BTIC4B_WriteAdRiceSymLL(ctx, i, &(st->rk));
		return;
	}
	if(i<32)
	{
		i0=(byte)(st->rov+i);		i1=(byte)(st->rov+i-1);
		i2=st->tab[i0];		i3=st->tab[i1];
		st->tab[i0]=i3;		st->tab[i1]=i2;
		st->idx[i2]=i1;		st->idx[i3]=i0;
		BTIC4B_WriteAdRiceSymLL(ctx, i, &(st->rk));
		return;
	}

	i0=(byte)(st->rov+i);	i1=(byte)(st->rov-1);
	i2=st->tab[i0];		i3=st->tab[i1];
	st->tab[i0]=i3;		st->tab[i1]=i2;
	st->idx[i2]=i1;		st->idx[i3]=i0;
	st->rov--;

	BTIC4B_WriteAdRiceSymLL(ctx, i, &(st->rk));
#endif

#if 1
	i=st->idx[val];
	i0=i;
//	i1=i>>1;
//	i1=(i*3)>>2;
	i1=(i*7)>>3;
	i2=st->tab[i0];		i3=st->tab[i1];
	st->tab[i0]=i3;		st->tab[i1]=i2;
	st->idx[i2]=i1;		st->idx[i3]=i0;
	BTIC4B_WriteAdRiceSymLL(ctx, i, &(st->rk));
#endif

}

#if 1
void BTIC4B_WriteSymbolSmtf(BTIC4B_Context *ctx,
	BTIC4B_SmtfState *st, int val)
{
	int i0, i1;

	i0=ctx->stat_bits;
	BTIC4B_WriteSymbolSmtfI(ctx, st, val);
	i1=ctx->stat_bits;
	st->bits+=i1-i0;
	st->cnt++;
}
#endif
