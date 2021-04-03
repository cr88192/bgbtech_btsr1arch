typedef struct BTRLZ0_DecContext_s BTRLZ0_DecContext;

struct BTRLZ0_DecContext_s {
byte *bs_cs;
byte *bs_cse;
// u32 bs_win;
byte bs_pos;

byte *ct;

byte tab_tag[258];
byte tab_lit[258];
byte rk_rlen;
byte rk_mlen;
byte rk_dist;
};

u32 BTRLZ0_ReadBits(BTRLZ0_DecContext *ctx, int n)
{
	byte *cs;
	byte p;
	u32 win, v;
	cs=ctx->bs_cs;		p=ctx->bs_pos;
	win=*(u32 *)cs;		v=(win>>p)&((1<<n)-1);
	p+=n;				cs+=p>>3;
	ctx->bs_cs=cs;		ctx->bs_pos=p&7;
	return(v);
}

u32 BTRLZ0_PeekBits(BTRLZ0_DecContext *ctx)
{
	byte *cs;
	byte p;
	u32 win, v;
	cs=ctx->bs_cs;		p=ctx->bs_pos;
	win=*(u32 *)cs;		v=(win>>p);
	return(v);
}

int BTRLZ0_SkipBits(BTRLZ0_DecContext *ctx, int n)
{
	byte *cs;
	byte p;
	cs=ctx->bs_cs;	p=ctx->bs_pos;
	p+=n;			cs+=p>>3;
	ctx->bs_cs=cs;	ctx->bs_pos=p&7;
	return(0);
}

int BTRLZ0_ReadAdRice(BTRLZ0_DecContext *ctx, byte *rk)
{
	static const u64 qtab = 0x0102010301020104ULL;
	int b, q, q1, k, p, v;
	
	k=*rk;
	b=BTRLZ0_PeekBits(ctx);
//	q=btrlz0_qtab[b&255];
	q =  ((qtab>>((b<<2)&0x3C))&15);
	q1=4+((qtab>>((b>>2)&0x3C))&15);
	if(q&4)		q=q1;
	if(q>=8)
	{
		BTRLZ0_SkipBits(ctx, 8);
		v=BTRLZ0_ReadBits(ctx, 8);
//		k++;
		k+=2;
		*rk=k;
		return(v);
	}	
	BTRLZ0_SkipBits(ctx, q+1);
	p=BTRLZ0_ReadBits(ctx, k);
	v=(q<<k)|p;
	if(!q)	k--;
	if(q>1)	k++;
//	if(q>1)	k+=q>>1;
	if(k<0)	k=0;
	*rk=k;
	return(v);
}

int BTRLZ0_ReadStfAdRice(BTRLZ0_DecContext *ctx, byte *tab)
{
	int ix, rov, v, v1, i0, i1;
	ix=BTRLZ0_ReadAdRice(ctx, tab+257);
	rov=tab[256];
	i0=(rov+ix)&255;
	v=tab[i0];
	if(ix>=32)
	{
		i1=(rov+255)&255;	v1=tab[i1];
		tab[i0]=v1;			tab[i1]=v;
		rov=(rov-1)&255;	tab[256]=rov;
	}else if(ix>0)
	{
		i1=(i0-1)&255;	v1=tab[i1];
		tab[i0]=v1;		tab[i1]=v;
	}
	
	return(v);
}

int BTRLZ0_InitStfAdRice(BTRLZ0_DecContext *ctx, byte *tab)
{
	int i;
	for(i=0; i<256; i++)
		tab[i]=i;
	tab[256]=0;
	tab[257]=4;
	return(0);
}

int BTRLZ0_ReadVlnAdRice(BTRLZ0_DecContext *ctx, byte *rk)
{
	int v, eb, p;
	v=BTRLZ0_ReadAdRice(ctx, rk);
	if(v>=32)
	{
		eb=(v>>4)-1;
		p=BTRLZ0_ReadBits(ctx, eb);
		v=((16|(v&15))<<eb)|p;
	}
	return(v);
}

int BTRLZ0_DecodeLiterals(BTRLZ0_DecContext *ctx, byte *ict, int len)
{
	byte *ct;
	int l;
	ct=ict; l=len;
	while(l--)
		{ *ct++=BTRLZ0_ReadStfAdRice(ctx, ctx->tab_lit); }
	return(0);
}

int BTRLZ0_MatchCopy(byte *ict, int ml, int md)
{
	byte *cs, *ct, *cte;
	if(md>=ml)
		{ memcpy(ict, ict-md, ml); }
	else if(md==1)
		{ memset(ict, *(ict-1), ml); }
	else
	{
		cs=ict-md;	ct=ict;	cte=ict+ml;
		while(ct<cte)
			{ *ct++=*cs++; }
	}
	return(0);
}

int BTRLZ0_DecodeBody(BTRLZ0_DecContext *ctx)
{
	byte *ct, *ct0;
	int tag, rl, ml, md;
	
	ct=ctx->ct;
	ct0=ct;
	while(1)
	{
		tag=BTRLZ0_ReadStfAdRice(ctx, ctx->tab_tag);
		
		rl=(tag>>4)&15;		ml=(tag&15)+3;
		if(rl>=15)
			rl+=BTRLZ0_ReadVlnAdRice(ctx, &(ctx->rk_rlen));
		if(ml>=18)
			ml+=BTRLZ0_ReadVlnAdRice(ctx, &(ctx->rk_mlen));
		md=BTRLZ0_ReadVlnAdRice(ctx, &(ctx->rk_dist));
		if(!md)
		{
			BTRLZ0_DecodeLiterals(ctx, ct, rl);
			ct+=rl;
			if(ml==4)
				continue;
			break;
		}
		BTRLZ0_DecodeLiterals(ctx, ct, rl);
		ct+=rl;
		BTRLZ0_MatchCopy(ct, ml, md);
		ct+=ml;
	}
	ctx->ct=ct;
	return(ct-ct0);
}

int BTRLZ0_DecodeBuffer(BTRLZ0_DecContext *ctx,
	byte *ibuf, byte *obuf, int isz, int osz)
{
	int i;
	ctx->bs_cs=ibuf;	ctx->bs_cse=ibuf+isz;
	ctx->bs_pos=0;		ctx->ct=obuf;
	BTRLZ0_InitStfAdRice(ctx, ctx->tab_tag);
	BTRLZ0_InitStfAdRice(ctx, ctx->tab_lit);
	ctx->rk_rlen=4;		ctx->rk_mlen=4;
	ctx->rk_dist=4;
	i=BTRLZ0_DecodeBody(ctx);
	return(i);
}
