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


void BTIC4B_SetupReadBits(BTIC4B_Context *ctx, byte *buf, int sz)
{
	ctx->cs=buf;
//	ctx->bit_win=0;
	ctx->bit_pos=0;
//	ctx->bit_win=*(u32 *)(ctx->cs);
	ctx->bit_win=btic4b_getu32le(ctx->cs);
}

byte *BTIC4B_EndReadBits(BTIC4B_Context *ctx)
{
	u32 bw;
	int bp;
	int i;
	
	bp=ctx->bit_pos;
	bw=ctx->bit_win;
	i=(bp+7)>>3;
	ctx->cs+=i;
	ctx->bit_win=(bw>>(i<<3));
	ctx->bit_pos=0;
	return(ctx->cs);
}

int BTIC4B_ReadNBitsNM(BTIC4B_Context *ctx, int len)
{
	u32 bw;
	int bp;
	int bits;
	
	bp=ctx->bit_pos;
	bw=ctx->bit_win;

	bits=bw>>bp;
	bp=bp+len;

	ctx->cs+=bp>>3;
//	ctx->bit_win=*(u32 *)ctx->cs;
	ctx->bit_win=btic4b_getu32le(ctx->cs);
	ctx->bit_pos=bp&7;
	return(bits);
}

int BTIC4B_ReadNBits(BTIC4B_Context *ctx, int len)
{
	u32 bw;
	int bp;
	int bits;
	
	bp=ctx->bit_pos;
	bw=ctx->bit_win;

	bits=(bw>>bp)&((1<<len)-1);
	bp=bp+len;

	ctx->cs+=bp>>3;
//	ctx->bit_win=*(u32 *)ctx->cs;
	ctx->bit_win=btic4b_getu32le(ctx->cs);
	ctx->bit_pos=bp&7;
	return(bits);
}

#if 0
int BTIC4B_Read8BitsNM(BTIC4B_Context *ctx)
{
	return(BTIC4B_ReadNBits(ctx, 8));
}

int BTIC4B_Read16BitsNM(BTIC4B_Context *ctx)
{
	return(BTIC4B_ReadNBits(ctx, 16));
}

int BTIC4B_Read24BitsNM(BTIC4B_Context *ctx)
{
	return(BTIC4B_ReadNBits(ctx, 24));
}

int BTIC4B_Read16Bits(BTIC4B_Context *ctx)
{
	return(BTIC4B_ReadNBits(ctx, 16));
}

int BTIC4B_Read24Bits(BTIC4B_Context *ctx)
{
	return(BTIC4B_ReadNBits(ctx, 24));
}

u32 BTIC4B_Read32Bits(BTIC4B_Context *ctx)
{
	int l, h;
	
	l=BTIC4B_Read16Bits(ctx);
	h=BTIC4B_Read16BitsNM(ctx);
	return((h<<16)|l);
}
#endif

#if 1
int BTIC4B_Read8BitsNM(BTIC4B_Context *ctx)
{
	int bits;
	bits=ctx->bit_win>>ctx->bit_pos;
	ctx->cs++;
//	ctx->bit_win=*(u32 *)ctx->cs;
	ctx->bit_win=btic4b_getu32le(ctx->cs);
	return(bits);
}

int BTIC4B_Read16BitsNM(BTIC4B_Context *ctx)
{
	int bits;
	bits=ctx->bit_win>>ctx->bit_pos;
	ctx->cs+=2;
//	ctx->bit_win=*(u32 *)ctx->cs;
	ctx->bit_win=btic4b_getu32le(ctx->cs);
	return(bits);
}

int BTIC4B_Read24BitsNM(BTIC4B_Context *ctx)
{
	int bits;
	bits=ctx->bit_win>>ctx->bit_pos;
	ctx->cs+=3;
//	ctx->bit_win=*(u32 *)ctx->cs;
	ctx->bit_win=btic4b_getu32le(ctx->cs);
	return(bits);
}

int BTIC4B_Read8Bits(BTIC4B_Context *ctx)
{
	return((byte)BTIC4B_Read8BitsNM(ctx));
}

int BTIC4B_Read16Bits(BTIC4B_Context *ctx)
{
	return((u16)BTIC4B_Read16BitsNM(ctx));
}

int BTIC4B_Read24Bits(BTIC4B_Context *ctx)
{
	return(BTIC4B_Read24BitsNM(ctx)&16777215);
}

#if 0
u32 BTIC4B_Read32Bits(BTIC4B_Context *ctx)
{
	u32 bits, bw1, m;

	bits=ctx->bit_win>>ctx->bit_pos;
	ctx->cs+=4;
//	bw1=*(u32 *)ctx->cs;
	bw1=btic4b_getu32le(ctx->cs);
	ctx->bit_win=bw1;
	m=~((ctx->bit_pos-1)>>31);
	bits|=(bw1<<(32-ctx->bit_pos))&m;
	return(bits);
}
#endif

#if 1
u32 BTIC4B_Read32Bits(BTIC4B_Context *ctx)
{
	byte *cs;
	u64 bw1;
	u32 bits;

	cs=ctx->cs;
	bw1=btic4b_getu64le(cs);
	ctx->cs=cs+4;
	bits=bw1>>ctx->bit_pos;
	ctx->bit_win=bw1>>32;
	return(bits);
}
#endif

#if 0
u32 BTIC4B_Read32Bits(BTIC4B_Context *ctx)
{
	int l, h;
	
	l=BTIC4B_Read16Bits(ctx);
	h=BTIC4B_Read16BitsNM(ctx);
	return((h<<16)|l);
}
#endif

#endif

u64 BTIC4B_Read64Bits(BTIC4B_Context *ctx)
{
	u32 l, m, h;

//	l=BTIC4B_Read24Bits(ctx);
//	m=BTIC4B_Read24Bits(ctx);
//	h=BTIC4B_Read16BitsNM(ctx);
//	return((((u64)h)<<48)|(((u64)m)<<24)|l);
	
	l=BTIC4B_Read32Bits(ctx);
	h=BTIC4B_Read32Bits(ctx);
	return((((u64)h)<<32)|l);
}

void BTIC4B_SkipNBits(BTIC4B_Context *ctx, int len)
{
	int bp;
	bp=ctx->bit_pos+len;
	ctx->cs+=bp>>3;
//	ctx->bit_win=*(u32 *)ctx->cs;
	ctx->bit_win=btic4b_getu32le(ctx->cs);
	ctx->bit_pos=bp&7;
}

void BTIC4B_Skip8Bits(BTIC4B_Context *ctx)
{
	ctx->cs++;
//	ctx->bit_win=*(u32 *)ctx->cs;
	ctx->bit_win=btic4b_getu32le(ctx->cs);
}

int BTIC4B_PeekNBitsNM(BTIC4B_Context *ctx, int len)
{
	u32 bw;
	int bp, bits;
	
	bp=ctx->bit_pos;
	bw=ctx->bit_win;
	bits=bw>>bp;
	return(bits);
}

//force_inline
int BTIC4B_Peek8Bits(BTIC4B_Context *ctx)
{
	u32 bw;
	int bp, bits;
	
	bp=ctx->bit_pos;
	bw=ctx->bit_win;
	bits=(byte)(bw>>bp);
	return(bits);
}

int BTIC4B_ReadRiceQ(BTIC4B_Context *ctx)
{
	int v, b;
	int i, j, k, l;

	b=BTIC4B_Peek8Bits(ctx);
	if(b!=255)
	{
		v=lqtvq_decriceq8[b];
		BTIC4B_SkipNBits(ctx, v+1);
		return(v);
	}

	BTIC4B_Skip8Bits(ctx);
	v=8;
	b=BTIC4B_Peek8Bits(ctx);
	while(b==255)
		{ BTIC4B_Skip8Bits(ctx); v+=8; b=BTIC4B_Peek8Bits(ctx); }
	i=lqtvq_decriceq8[b];
	BTIC4B_SkipNBits(ctx, i+1);
	return(v+i);
}

int BTIC4B_ReadAdRiceILL(BTIC4B_Context *ctx, byte *rk)
{
	int q, k, b, v;
	int j;
	
	k=*rk;
	q=BTIC4B_ReadRiceQ(ctx);

#if 1
	if(q>=8)
	{
		v=BTIC4B_ReadNBits(ctx, 5+(q-8)*3);
		k=k+3+(q-8);
		if(k>15)k=15;
		*rk=k;
		return(v);
	}
#endif
	
	b=BTIC4B_ReadNBits(ctx, k);
	v=(q<<k)|b;

#if 0
	if(!q)
	{
		if(k>0)k--;
		*rk=k;
	}else if(q>1)
	{
		j=q;
		while(j>1)
			{ k++; j=j>>1; }
		if(k>15)k=15;
		*rk=k;
	}
#endif

	*rk=lqtvq_decricenk8[(k<<4)|q];
	return(v);
}

int BTIC4B_ReadAdRiceLL(BTIC4B_Context *ctx, byte *rk)
{
	int v, b;
	int i, j, k, l;

#if 1
	b=BTIC4B_Peek8Bits(ctx);
	j=lqtvq_decrice8[(*rk<<8)|b];
	
	if(j)
	{
#if 0
//		i=(u16)j;
		BTIC4B_SkipNBits(ctx, (j>>16)&15);
		*rk=(j>>20)&15;
//		return(i);
		return((u16)j);
#endif

#if 1
		BTIC4B_SkipNBits(ctx, (j>>8)&15);
		*rk=(j>>12)&15;
		return((byte)j);
#endif
	}
#endif

	i=BTIC4B_ReadAdRiceILL(ctx, rk);
	return(i);
}

int BTIC4B_ReadAdSRiceLL(BTIC4B_Context *ctx, byte *rk)
{
	int v;
	
	v=BTIC4B_ReadAdRiceLL(ctx, rk);
	v=(v>>1)^((v<<31)>>31);
	return(v);
}

#if 1
int BTIC4B_DecodeSymbolIndexSmtf(BTIC4B_Context *ctx,
	BTIC4B_SmtfState *st, int i)
{
	int i0, i1, i2, i3;

#if 1
	i0=i;
//	i1=i>>1;
//	i1=(i*3)>>2;
	i1=(i*7)>>3;
	i2=st->tab[i0];		i3=st->tab[i1];
	st->tab[i0]=i3;		st->tab[i1]=i2;
//	st->idx[i2]=i1;		st->idx[i3]=i0;
	return(i2);
#endif

#if 0
	if(!i)
	{
		i0=(byte)(st->rov+i);
		i2=st->tab[i0];
		return(i2);
	}

	if(i<32)
	{
		i0=(byte)(st->rov+i);		i1=(byte)(st->rov+i-1);
		i2=st->tab[i0];		i3=st->tab[i1];
		st->tab[i0]=i3;		st->tab[i1]=i2;
//		st->idx[i2]=i1;		st->idx[i3]=i0;
		return(i2);
	}

	i0=(byte)(st->rov+i);	i1=(byte)(st->rov-1);
	i2=st->tab[i0];		i3=st->tab[i1];
	st->tab[i0]=i3;		st->tab[i1]=i2;
//	st->idx[i2]=i1;		st->idx[i3]=i0;
	st->rov--;
	return(i2);
#endif
}
#endif

int BTIC4B_ReadSymbolSmtf(BTIC4B_Context *ctx,
	BTIC4B_SmtfState *st)
{
	int i0, i1, i2, i3;
	int i;

#ifdef BTIC4B_DBG_NOSMTF
	i=BTIC4B_ReadAdRiceLL(ctx, &(st->rk));
	return(i);
#endif

#if 0
	i=BTIC4B_ReadAdRiceLL(ctx, &(st->rk));
	if(!i)
	{
		i0=(byte)(st->rov+i);
		i2=st->tab[i0];
		return(i2);
	}

	if(i<32)
	{
		i0=(byte)(st->rov+i);		i1=(byte)(st->rov+i-1);
		i2=st->tab[i0];		i3=st->tab[i1];
		st->tab[i0]=i3;		st->tab[i1]=i2;
//		st->idx[i2]=i1;		st->idx[i3]=i0;
		return(i2);
	}

	i0=(byte)(st->rov+i);	i1=(byte)(st->rov-1);
	i2=st->tab[i0];		i3=st->tab[i1];
	st->tab[i0]=i3;		st->tab[i1]=i2;
//	st->idx[i2]=i1;		st->idx[i3]=i0;
	st->rov--;
	return(i2);
#endif

#if 1
	i=BTIC4B_ReadAdRiceLL(ctx, &(st->rk));
	i0=i;
//	i1=i>>1;
//	i1=(i*3)>>2;
	i1=(i*7)>>3;
	i2=st->tab[i0];		i3=st->tab[i1];
	st->tab[i0]=i3;		st->tab[i1]=i2;
//	i2=*(st->tab+i0);	i3=*(st->tab+i1);
//	*(st->tab+i0)=i3;	*(st->tab+i1)=i2;
//	st->idx[i2]=i1;		st->idx[i3]=i0;
	return(i2);
#endif
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

