typedef struct BTRLZ0_EncContext_s BTRLZ0_EncContext;

//#define TKELZ_HASH_SZ	1024
#define TKELZ_HASH_SZ	4096
#define TKELZ_HASH_DN	64
#define TKELZ_HASH_DN1	63

#define TKELZ_CHHASH_SZ	16384
#define TKELZ_CHAIN_SZ	65536

struct BTRLZ0_EncContext_s {
byte *bs_cs;
byte *bs_cse;
u32 bs_win;
byte bs_pos;

byte *bs_ct;

byte tab_tag[520];
byte tab_lit[520];
byte rk_rlen;
byte rk_mlen;
byte rk_dist;


byte	*chn_base;
byte	*chn_ptrs[TKELZ_CHAIN_SZ];
u16		chn_next[TKELZ_CHAIN_SZ];
u16		chn_hash[TKELZ_CHHASH_SZ];

byte *hash[TKELZ_HASH_SZ*TKELZ_HASH_DN];
byte hrov[TKELZ_HASH_SZ];
byte *cs;
byte *ct;
int maxlen;
int maxdist;
u32	csum;
byte cmp;

byte *pred_cs1;
byte *pred_cs2;
int pred_l1, pred_d1;
int pred_l2, pred_d2;
};


int BTRLZ0_WriteBits(BTRLZ0_EncContext *ctx, int v, int n)
{
	byte *ct;
	u32 win, p;
	win=ctx->bs_win;
	ct=ctx->bs_ct;
	p=ctx->bs_pos;
	win|=(v&((1<<n)-1))<<p;
	*(u32 *)ct=win;
	p+=n;
	ct+=p>>3;
	win>>=p&0x18;
	ctx->bs_win=win;
	ctx->bs_ct=ct;
	ctx->bs_pos=p&7;
	return(0);
}

int BTRLZ0_FlushBits(BTRLZ0_EncContext *ctx)
{
	BTRLZ0_WriteBits(ctx, 0, 7);
	ctx->bs_pos=0;
	return(0);
}

int BTRLZ0_WriteAdRice(BTRLZ0_EncContext *ctx, int val, byte *rk)
{
	int q, v, k;
	
	v=val&255;
	k=*rk;
	q=val>>k;
	if(q>=8)
	{
		BTRLZ0_WriteBits(ctx, 0x00, 8);
		BTRLZ0_WriteBits(ctx, v, 8);
//		k++;
		k+=2;
		*rk=k;
		return(0);
	}

	BTRLZ0_WriteBits(ctx, 1<<q, q+1);
	BTRLZ0_WriteBits(ctx, v, k);

	if(!q)	k--;
	if(q>1)	k++;
//	if(q>1)	k+=q>>1;
	if(k<0)	k=0;
	*rk=k;
	return(0);
}

int BTRLZ0_WriteStfAdRice(BTRLZ0_EncContext *ctx, int val, byte *tab)
{
	int ix, rov, v, v1, i0, i1;
	
	v=val&255;
	i0=tab[260+v];
	
//	ix=BTRLZ0_ReadAdRice(ctx, tab+257);
	rov=tab[256];
	ix=(i0-rov)&255;
	
	BTRLZ0_WriteAdRice(ctx, ix, tab+257);
	
	i0=(rov+ix)&255;
	v=tab[i0];
	if(ix>=32)
	{
		i1=(rov+255)&255;
		v1=tab[i1];
		tab[i0]=v1;
		tab[i1]=v;
		tab[260+v1]=i0;
		tab[260+v ]=i1;
		rov=(rov-1)&255;
		tab[256]=rov;
	}else if(ix>0)
	{
		i1=(i0-1)&255;
		v1=tab[i1];
		tab[i0]=v1;
		tab[i1]=v;
		tab[260+v1]=i0;
		tab[260+v ]=i1;
	}
	
	return(0);
}

int BTRLZ0_EncInitStfAdRice(BTRLZ0_EncContext *ctx, byte *tab)
{
	int i;
	for(i=0; i<256; i++)
	{
		tab[i]=i;
		tab[260+i]=i;
	}
	tab[256]=0;
	tab[257]=4;
	return(0);
}

int BTRLZ0_WriteVlnAdRice(BTRLZ0_EncContext *ctx, int val, byte *rk)
{
	int v, p, e;
	if(val<32)
	{
		BTRLZ0_WriteAdRice(ctx, val, rk);
		return(0);
	}
	
	v=val; e=0;
	while(v>=32)
		{ v=v>>1; e++; }
	p=((e+1)<<4)|(v&15);
	BTRLZ0_WriteAdRice(ctx, p, rk);
	BTRLZ0_WriteBits(ctx, val, e);
	return(0);
}


int BTRLZ0_CalcHash(byte *cs)
{
	int h;
	
	h=*(u32 *)cs;
//	h=get_u32le(cs);
	h=((h*4093)>>12)&(TKELZ_HASH_SZ-1);
	return(h);
}

int BTRLZ0_CalcHashB(byte *cs)
{
	int h;
	
	h=*(u32 *)cs;
//	h=get_u32le(cs);
	h^=h>>16;
	h=((h*4093)>>12)&(TKELZ_CHHASH_SZ-1);
	return(h);
}

int BTRLZ0_CheckMatch(byte *s0, byte *s1, byte *cse)
{
	byte *cs, *ct;
	
	cs=s0; ct=s1;
	while((cs+4)<=cse)
	{
		if(*(u32 *)cs!=*(u32 *)ct)
//		if(get_u32le(cs)!=get_u32le(ct))
			break;
		cs+=4; ct+=4;
	}
	while(cs<cse)
	{
		if(*cs!=*ct)
			break;
		cs++; ct++;
	}
	
	return(cs-s0);
}

int BTRLZ0_HashByte(BTRLZ0_EncContext *ctx, byte *cs)
{
	int h, hr, rv;
	int i;

	rv=(cs-ctx->chn_base)&(TKELZ_CHAIN_SZ-1);

	h=BTRLZ0_CalcHash(cs);
	
	hr=ctx->hrov[h];
	hr=(hr-1)&TKELZ_HASH_DN1;
	ctx->hrov[h]=hr;
	ctx->hash[(h*TKELZ_HASH_DN)+hr]=cs;

	h=BTRLZ0_CalcHashB(cs);

	ctx->chn_ptrs[rv]=cs;
	ctx->chn_next[rv]=ctx->chn_hash[h];
	ctx->chn_hash[h]=rv;

	return(0);
}

int BTRLZ0_HashStr(BTRLZ0_EncContext *ctx, byte *cs, int l)
{
	byte *cse;
	
	cse=cs+l;
	while(cs<cse)
	{
		BTRLZ0_HashByte(ctx, cs);
		cs++;
	}
	return(0);
}

int BTRLZ0_LookupMatch(BTRLZ0_EncContext *ctx,
	byte *str, byte *stre,
	int *rl, int *rd)
{
	byte *cs1, *cse;
	int h, hr, bl, bd;
	int i, j, k, l, d, n, ml, md, ld;

	if(str==ctx->pred_cs2)
	{
		bl=ctx->pred_l2;
		bd=ctx->pred_d2;

		*rl=bl;
		*rd=bd;
		return((bl>=4) || ((bl>=3) && (bd<512)));
	}

	if(str==ctx->pred_cs1)
	{
		bl=ctx->pred_l1;
		bd=ctx->pred_d1;

		*rl=bl;
		*rd=bd;
		return((bl>=4) || ((bl>=3) && (bd<512)));
	}

	ml=ctx->maxlen;
	md=ctx->maxdist;
//	cse=str+259;
//	cse=str+1023;
	cse=str+ml;
	if(cse>stre)
		cse=stre;
	
	h=BTRLZ0_CalcHash(str);
	hr=ctx->hrov[h];
	
	bl=0; bd=0;
	
	for(i=0; i<TKELZ_HASH_DN; i++)
	{
		cs1=ctx->hash[(h*TKELZ_HASH_DN)+((hr+i)&TKELZ_HASH_DN1)];
		if(!cs1)continue;
		l=BTRLZ0_CheckMatch(str, cs1, cse);
		d=str-cs1;
		if(d<=0)
			continue;
//		if((l>bl) && (d<65536))
//		if((l>bl) && (d<131072))
		if((l>bl) && (d<md))
			{ bl=l; bd=d; }
	}

//	ctx->chn_ptrs[rv]=cs;
//	ctx->chn_next[rv]=ctx->chn_hash[h];
//	ctx->chn_hash[h]=rv;

	h=BTRLZ0_CalcHashB(str);

	i=ctx->chn_hash[h]; n=1024; ld=0;
	while(n--)
	{
		cs1=ctx->chn_ptrs[i];
		if(!cs1)continue;

		d=str-cs1;
		if(d<=ld)
			break;
		ld=d;
		if(d>md)
			break;

		l=BTRLZ0_CheckMatch(str, cs1, cse);		
		if((l>bl) && (d<md))
			{ bl=l; bd=d; }
		i=ctx->chn_next[i];
	}

	ctx->pred_cs2=ctx->pred_cs1;
	ctx->pred_l2=ctx->pred_l1;
	ctx->pred_d2=ctx->pred_d1;
	ctx->pred_cs1=str;
	ctx->pred_l1=bl;
	ctx->pred_d1=bd;

	*rl=bl;
	*rd=bd;

	return(bl>=3);

//	return((bl>=4) ||
//		((bl>=3) && (bd<512) && (ctx->cmp==3)));
}

int tgvlz_log2u(int val)
{
	int i, v;
	
	v=val; i=0;
	while(v)
		{ i++; v=v>>1; }
	return(i);
}

#if 1
int BTRLZ0_EstRawCost(BTRLZ0_EncContext *ctx, int rl)
{
	int i, j, c;

	if(ctx->cmp==4)
	{
		c=rl+1;
		if(rl>=15)
		{
			i=rl-15;
			while(i>=255)
				{ c++; i-=255; }
			c++;
		}
		c+=3;
		return(c);
	}

	c=rl;
	i=rl;
	while(i>=4096)
		{ c+=2; i-=4096; }
	if(i>(128+7))
		{ c+=2; i=i&7; }
	if(i>=8)
		{ c+=1; i=i&7; }

//	c=(rl>>3)+rl;
//	c=(rl>>7)+rl+(rl>=8);
	return(c);
}

int BTRLZ0_EstMatchCost(BTRLZ0_EncContext *ctx, int rl, int bl, int bd)
{
	int c, i;

	if(ctx->cmp==4)
	{
		c=rl+1;
		if(rl>=15)
		{
			i=rl-15;
			while(i>=255)
				{ c++; i-=255; }
			c++;
		}
		if(bl>=19)
		{
			i=bl-19;
			while(i>=255)
				{ c++; i-=255; }
			c++;
		}
		
		return(c);
	}

//	c=(rl>>3)+rl;
//	c=(rl>>7)+rl+(rl>=8);
	c=BTRLZ0_EstRawCost(ctx, rl);
	if((bl<=10) && (bd<512))
		{ c+=2; }
	else if((bl<=67) && (bd<8192))
		{ c+=3; }
	else if((bl<=515) && (bd<131072))
		{ c+=4; }
	else if((bl<=16383) && (bd<(1<<22)))
	{
		c+=1+((bl<128)?1:2)+((bd<32768)?2:3);
	}
	return(c);
}
#endif

#if 1
int BTRLZ0_LookupMatchB(BTRLZ0_EncContext *ctx,
	byte *cs, byte *cse, byte *lcs,
	int *rl, int *rd)
{
	int l, d, l1, d1;
	int rl0, rl1, rl2;
	int mc0, mc1;
	int i, j, k;

	i=BTRLZ0_LookupMatch(ctx, cs, cse, &l, &d);
	rl0=cs-lcs;
	rl1=rl0+1;	rl2=rl0+2;
	
	mc0=BTRLZ0_EstMatchCost(ctx, rl0, l, d);
	mc1=BTRLZ0_EstRawCost(ctx, rl0+l);
	
	if(mc0>=mc1)
	{
		/* Skip match if it costs more than it saves. */
		i=0;
	}
	
	if(i)
	{
		/* Check if a better match is just around the corner. */

		j=BTRLZ0_LookupMatch(ctx, cs+1, cse, &l1, &d1);
		if(j && (l1>(l+1)) && ((rl0>>3)==(rl1>>3)))
			i=0;

		mc1=BTRLZ0_EstMatchCost(ctx, rl1, l1, d1);
//		if(j && (l1>(l+1)) && (mc1<=mc0))
//		if(j && (l1>(l+0)) && (mc1<=mc0))
		if(j && ((mc1-l1)<=(mc0-l)))
			i=0;

//		if(j && (l1>(l+1)))
//		if(j && (l1>(l+2)))
//			i=0;

		if(i)
		{
			j=BTRLZ0_LookupMatch(ctx, cs+2, cse, &l1, &d1);

			mc1=BTRLZ0_EstMatchCost(ctx, rl2, l1, d1);
//			if(j && (l1>(l+0)) && (mc1<=mc0))
			if(j && ((mc1-l1)<(mc0-l)))
//			if(j && ((mc1-l1)<=(mc0-l)))
				i=0;
		}
	}
	
	*rl=l;
	*rd=d;
	return(i);
}
#endif



int BTRLZ0_EncodeBuffer(BTRLZ0_EncContext *ctx,
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	byte *cs, *cse, *lcs;
	byte *ct;
	u32 v;
	int pl, pd, tg;
	int l, d, rl, l1, d1;
	int i, j, k;
	
	ctx->chn_base=ibuf;
	cs=ibuf; cse=ibuf+ibsz;

	ctx->bs_ct=obuf;
	ctx->bs_win=0;
	ctx->bs_pos=0;

	ctx->cmp=3;
	ctx->maxlen=16384;
	ctx->maxdist=131072;

	BTRLZ0_EncInitStfAdRice(ctx, ctx->tab_tag);
	BTRLZ0_EncInitStfAdRice(ctx, ctx->tab_lit);
	ctx->rk_rlen=4;
	ctx->rk_mlen=4;
	ctx->rk_dist=4;

	pd=0; pl=0;
	
	lcs=cs;
	while(cs<cse)
	{
//		i=BTRLZ0_LookupMatch(ctx, cs, cse, &l, &d);
		i=BTRLZ0_LookupMatchB(ctx, cs, cse, lcs, &l, &d);

		if(!i)
		{
			BTRLZ0_HashByte(ctx, cs);
			cs++;
			continue;
		}
			
		rl=cs-lcs;

//		ctx->stat_rlen[tgvlz_log2u(rl)]++;
//		ctx->stat_len[tgvlz_log2u(l)]++;
//		ctx->stat_dist[tgvlz_log2u(d)]++;
		
		while(rl>=65536)
		{
			k=65536;
			tg=0xF1;
			BTRLZ0_WriteStfAdRice(ctx, tg, ctx->tab_tag);
			BTRLZ0_WriteVlnAdRice(ctx, k-15, &ctx->rk_rlen);
			BTRLZ0_WriteVlnAdRice(ctx, 0, &ctx->rk_dist);
			while(k--)
				{ BTRLZ0_WriteStfAdRice(ctx, *lcs++, ctx->tab_lit); }
			rl=cs-lcs;
		}
		
		//Encode Match
		tg=0;
		if(rl<15)
			tg|=rl<<4;
		else
			tg|=0xF0;
		if(l<18)
			tg|=l-3;
		else
			tg|=0x0F;
		BTRLZ0_WriteStfAdRice(ctx, tg, ctx->tab_tag);
		if(rl>=15)
			BTRLZ0_WriteVlnAdRice(ctx, rl-15, &ctx->rk_rlen);
		if(l>=18)
			BTRLZ0_WriteVlnAdRice(ctx, l-18, &ctx->rk_mlen);
		BTRLZ0_WriteVlnAdRice(ctx, d, &ctx->rk_dist);

		while(rl--)
			{ BTRLZ0_WriteStfAdRice(ctx, *lcs++, ctx->tab_lit); }

		pl=l; pd=d;
		
		BTRLZ0_HashStr(ctx, cs, l);
		cs+=l;
		lcs=cs;
	}

	rl=cs-lcs;

	if(rl>0)
	{
		tg=0;
		if(rl<15)
			tg|=rl<<4;
		else
			tg|=0xF0;
		BTRLZ0_WriteStfAdRice(ctx, tg, ctx->tab_tag);
		if(rl>=15)
			BTRLZ0_WriteVlnAdRice(ctx, rl-15, &ctx->rk_rlen);
		BTRLZ0_WriteVlnAdRice(ctx, 0, &ctx->rk_dist);

		while(rl--)
			{ BTRLZ0_WriteStfAdRice(ctx, *lcs++, ctx->tab_lit); }
	}else
	{
		BTRLZ0_WriteStfAdRice(ctx, 0x00, ctx->tab_tag);
		BTRLZ0_WriteVlnAdRice(ctx, 0, &ctx->rk_dist);
	}
	
	BTRLZ0_FlushBits(ctx);
	ct=ctx->bs_ct;
	
	return(ct-obuf);
}
