/*
Uses a transform buffer.

TransBuf
	VLN litLen;
	VLN matchLen;
	VLN matchDist;

VLN's consist of a chain of bytes, with the MSB indicating another byte follows.
VLN's are encoded in MSB-first order.
 */


#define TKULZ_HASH_SZ	4096
// #define TKULZ_HASH_SZ	2048
// #define TKULZ_HASH_SZ	1024

// #define TKULZ_HASH_LVL	16
// #define TKULZ_HASH_LVL	32
#define TKULZ_HASH_LVL	64

// #define TKULZ_MAX_DIST	(1<<20)
#define TKULZ_MAX_DIST	(1<<18)
// #define TKULZ_MAX_DIST	(1<<15)

// #define TKULZ_MAX_MATCH	(1<<12)
// #define TKULZ_MAX_MATCH	(1<<14)
#define TKULZ_MAX_MATCH	(1<<16)
// #define TKULZ_MAX_MATCH	(1<<8)

#define TKULZ_TBLK_SZ	(1<<16)
#define TKULZ_TBUF_SZ	((1<<16)+1024)

#ifndef TKULZ_BYTE
#define TKULZ_BYTE
typedef unsigned char		byte;
typedef unsigned short	u16;
typedef unsigned int		u32;

typedef signed char		sbyte;
typedef signed short		s16;
typedef signed int		s32;
#endif

typedef struct TKuLZ_EncState_s TKuLZ_EncState;

struct TKuLZ_EncState_s {
	byte *ct;
	u32 win;
	sbyte pos;
	byte status;

//	byte *hash_pos[TKULZ_HASH_SZ*TKULZ_HASH_LVL];
//	byte hash_rov[TKULZ_HASH_SZ];

	byte **hash_pos;
	byte *hash_rov;
	
	byte tbuf[TKULZ_TBUF_SZ];

	u32 hetab_t[256];
	u32 hetab_l[256];
	u32 hetab_d[256];

	byte hltab_t[256];
	byte hltab_l[256];
	byte hltab_d[256];

	int hstab_t[256];
	int hstab_l[256];
	int hstab_d[256];

//	byte *cs;
//	u16 htab_t[4096];
//	u16 htab_l[4096];
//	u16 htab_d[4096];
};

void TKuLZ_WriteBits(TKuLZ_EncState *ctx, int val, int bits)
{
	if((bits>24) || (bits<0))
		__debugbreak();

	ctx->win|=(val&((1<<bits)-1))<<ctx->pos;
	ctx->pos+=bits;
	while(ctx->pos>=8)
	{
		*ctx->ct++=ctx->win;
		ctx->win>>=8;
		ctx->pos-=8;
	}
}

void TKuLZ_FlushWriteBits(TKuLZ_EncState *ctx)
{
	while(ctx->pos>0)
	{
		*ctx->ct++=ctx->win;
		ctx->win>>=8;
		ctx->pos-=8;
	}
	ctx->pos=0;
}

void TKuLZ_SetupEncTableLengths(TKuLZ_EncState *ctx, u32 *htab, byte *cls)
{
	int c, l, tc;
	int i, j, k;

	for(i=0; i<256; i++)
		htab[i]=0;
	
	c=0;
	for(l=1; l<14; l++)
	{
		for(i=0; i<256; i++)
			if(cls[i]==l)
		{
			tc=c<<(13-l);
			k=TKuLZ_TransposeWord(tc)>>3;
			htab[i]=(l<<16)|k;
			c++;
		}
		c=c<<1;
	}
	
//	if(c>8192)
//		__debugbreak();
}

void TKuLZ_EncodeHuffSym(TKuLZ_EncState *ctx, u32 *hetab, int sym)
{
	int b;
	b=hetab[sym];
	
	if(!((b>>16)&15))
		__debugbreak();
	
	TKuLZ_WriteBits(ctx, b, b>>16);
}

void TKuLZ_EncodeTagSym(TKuLZ_EncState *ctx, int sym)
	{ TKuLZ_EncodeHuffSym(ctx, ctx->hetab_t, sym); }

void TKuLZ_EncodeLitSym(TKuLZ_EncState *ctx, int sym)
	{ TKuLZ_EncodeHuffSym(ctx, ctx->hetab_l, sym); }

void TKuLZ_EncodeDistSym(TKuLZ_EncState *ctx, int sym)
	{ TKuLZ_EncodeHuffSym(ctx, ctx->hetab_d, sym); }

void TKuLZ_EncodeDistance(TKuLZ_EncState *ctx, int val)
{
	int eb, v;

	if(val<0)
		__debugbreak();

	if(val<8)
	{
		TKuLZ_EncodeDistSym(ctx, val);
		return;
	}
	
	eb=0; v=val;
	while(v>=16)
		{ eb++; v=v>>1; }
	TKuLZ_EncodeDistSym(ctx, ((eb+1)<<3)|(v&7));
	TKuLZ_WriteBits(ctx, val, eb);
}


void TKuLZ_StatTagSym(TKuLZ_EncState *ctx, int sym)
	{ ctx->hstab_t[sym]++; }
void TKuLZ_StatLitSym(TKuLZ_EncState *ctx, int sym)
	{ ctx->hstab_l[sym]++; }
void TKuLZ_StatDistSym(TKuLZ_EncState *ctx, int sym)
	{ ctx->hstab_d[sym]++; }

void TKuLZ_StatDistance(TKuLZ_EncState *ctx, int val)
{
	int eb, v;

	if(val<8)
		{ TKuLZ_StatDistSym(ctx, val); return; }
	eb=0; v=val;
	while(v>=16)
		{ eb++; v=v>>1; }
	TKuLZ_StatDistSym(ctx, ((eb+1)<<3)|(v&7));
}

int TKuLZ_ReadVLN(byte **rcs)
{
	byte *cs;
	int i, j;
	cs=*rcs;
	i=*cs++; j=0;
	while(i&0x80)
		{ j=(j<<7)|(i&0x7F); i=*cs++; }
	j=(j<<7)|(i&0x7F);
	*rcs=cs;
	return(j);
}

void TKuLZ_WriteVLN(byte **rct, int val)
{
	byte tb[8];
	byte *ct;
	int nt, v;
	
	if(val<0)
		__debugbreak();
	
	ct=*rct;
	nt=0; v=val;
	tb[nt++]=(v&0x7F); v=(v>>7);
	while(v)
		{ tb[nt++]=0x80|(v&0x7F); v=(v>>7); }
	while(nt--)
		{ *ct++=tb[nt]; }
	*rct=ct;
}

void TKuLZ_EncodeTransBuf(TKuLZ_EncState *ctx, byte *tbuf, int sztb)
{
	byte *cs, *cse;
	int ll, ml, md, ed, tg;
	int i, j, k;
	
	cs=tbuf; cse=cs+sztb;
	while(cs<cse)
	{
		ll=TKuLZ_ReadVLN(&cs);
		ml=TKuLZ_ReadVLN(&cs);
		md=TKuLZ_ReadVLN(&cs);
		ed=-1;
			
		if(!md && ml)
		{
			if(ml>1)
				ed=TKuLZ_ReadVLN(&cs);
//			__debugbreak();
		}
		
		tg=0;
		if(ll<15)
			{ tg|=ll<<4; }
		else
			{ tg|=0xF0; }
		if(ml<15)
			{ tg|=ml; }
		else
			{ tg|=0x0F; }
		
		TKuLZ_EncodeTagSym(ctx, tg);
		if(ll>=15)
			TKuLZ_EncodeDistance(ctx, ll-15);
		if(ml>=15)
			TKuLZ_EncodeDistance(ctx, ml-15);
		TKuLZ_EncodeDistance(ctx, md);
		if(ed>=0)
			TKuLZ_EncodeDistance(ctx, ed);
		
		i=ll;
		while(i--)
			TKuLZ_EncodeLitSym(ctx, *cs++);
	}
}

void TKuLZ_StatTransBuf(TKuLZ_EncState *ctx, byte *tbuf, int sztb)
{
	byte *cs, *cse;
	int ll, ml, md, ed, tg;
	int i, j, k;
	
	cs=tbuf; cse=cs+sztb;
	while(cs<cse)
	{
		ll=TKuLZ_ReadVLN(&cs);
		ml=TKuLZ_ReadVLN(&cs);
		md=TKuLZ_ReadVLN(&cs);
		ed=-1;
		
		if(!md && ml)
		{
			if(ml>1)
				ed=TKuLZ_ReadVLN(&cs);
//			__debugbreak();
		}
		
		tg=0;
		if(ll<15)
			{ tg|=ll<<4; }
		else
			{ tg|=0xF0; }
		if(ml<15)
			{ tg|=ml; }
		else
			{ tg|=0x0F; }
		
		TKuLZ_StatTagSym(ctx, tg);
		if(ll>=15)
			TKuLZ_StatDistance(ctx, ll-15);
		if(ml>=15)
			TKuLZ_StatDistance(ctx, ml-15);
		TKuLZ_StatDistance(ctx, md);
		if(ed>=0)
			TKuLZ_StatDistance(ctx, ed);
		
		i=ll;
		while(i--)
			TKuLZ_StatLitSym(ctx, *cs++);
	}
}


int TKuLZ_WritePackedLengths(TKuLZ_EncState *ctx, byte *cls)
{
	byte *s, *s1, *se;
	int l, ll, n;
	
	s=cls; se=cls+256; ll=-1;
	while(s<se)
	{
		l=*s++;
		if(!l)
		{
			s1=s;
			while((s1<se) && !(*s1))s1++;
			n=(s1-s)+1;
			
			if((s1>=se) && (n>18))
			{
				TKuLZ_WriteBits(ctx, 0xF, 4);
//				TKuLZ_WriteBits(ctx, 3, 2);
				TKuLZ_WriteBits(ctx, 1, 2);
				TKuLZ_WriteBits(ctx, 0, 6);
				break;
			}
			
			if((n>=3) && (n<=18))
			{
				TKuLZ_WriteBits(ctx, 0xE, 4);
				TKuLZ_WriteBits(ctx, n-3, 4);
				s=s1;
				continue;
			}
			if((n>=19) && (n<=82))
			{
				TKuLZ_WriteBits(ctx, 0xF, 4);
				TKuLZ_WriteBits(ctx, 0, 2);
				TKuLZ_WriteBits(ctx, n-19, 6);
				s=s1;
				continue;
			}

			if(n>82)
			{
				TKuLZ_WriteBits(ctx, 0xF, 4);
				TKuLZ_WriteBits(ctx, 0, 2);
				TKuLZ_WriteBits(ctx, 63, 6);
				s+=81;
				continue;
			}

			ll=0;
			TKuLZ_WriteBits(ctx, 0x0, 4);
			continue;
		}

#if 1
		if(l==ll)
		{
			s1=s;
			while((s1<se) && (*s1==ll))s1++;
			n=(s1-s)+1;

			if((n>=4) && (n<=66))
			{
				TKuLZ_WriteBits(ctx, 0xF, 4);
				TKuLZ_WriteBits(ctx, 1, 2);
				TKuLZ_WriteBits(ctx, n-3, 6);
				s=s1;
				continue;
			}

#if 1
			if(n>66)
			{
				TKuLZ_WriteBits(ctx, 0xF, 4);
				TKuLZ_WriteBits(ctx, 1, 2);
				TKuLZ_WriteBits(ctx, 63, 6);
				s+=65;
				continue;
			}
#endif
		}
#endif

		if((l>=1) && (l<=13))
		{
			TKuLZ_WriteBits(ctx, l, 4);
			ll=l;
			continue;
		}
		
		__debugbreak();
	}
}


int PDZ2_BalanceTree_r(short *nodes, short *nlen, int root, int h, int ml)
{
	int h0, h1, h2, h3;
	int l0, l1, l2;

	if(root<0)
	{
//		printf("L");
		return(0);
	}

//	printf("{");

	h1=PDZ2_BalanceTree_r(nodes, nlen, nodes[root*2+0], h+1, ml);
	h2=PDZ2_BalanceTree_r(nodes, nlen, nodes[root*2+1], h+1, ml);
	h0=((h1>h2)?h1:h2)+1;
	nlen[root]=h0;

	if((h+h0)<=ml)	//depth limit not exceeded
	{
//		printf("}");
		return(h0);
	}

	//ok, so part of the tree is too deep
//	if((h1+1)<h2)
	if(h1<h2)
	{
		l0=nodes[root*2+1];
//		if(l0<0)return(h0);	//can't rebalance leaves

		l1=nodes[l0*2+1];
		nodes[l0*2+1]=nodes[l0*2+0];
		nodes[l0*2+0]=nodes[root*2+0];
		nodes[root*2+0]=l0;
		nodes[root*2+1]=l1;
	}else
//		if((h2+1)<h1)
		if(h2<h1)
	{
		l0=nodes[root*2+0];
//		if(l0<0)return(h0);	//can't rebalance leaves

		l1=nodes[l0*2+0];
		nodes[l0*2+0]=nodes[l0*2+1];
		nodes[l0*2+1]=nodes[root*2+1];
		nodes[root*2+0]=l1;
		nodes[root*2+1]=l0;
	}else
	{
//		printf("bal}");
		//rotating would be ineffective or would make things worse...
		return(h0);
	}

	//recalc depth of modified sub-tree
	l1=nodes[l0*2+0];
	l2=nodes[l0*2+1];
	h1=(l1<0)?0:nlen[l1];
	h2=(l2<0)?0:nlen[l2];
	h3=((h1>h2)?h1:h2)+1;
	nlen[l0]=h3;

	//recalc height of root node
	l1=nodes[root*2+0];
	l2=nodes[root*2+1];
	h1=(l1<0)?0:nlen[l1];
	h2=(l2<0)?0:nlen[l2];
	h0=((h1>h2)?h1:h2)+1;
	nlen[root]=h0;

//	printf("rebal}");

	return(h0);
}

void PDZ2_CalcLengths_r(short *nodes, byte *cl, int root, int h)
{
	if(root<0)
	{
		cl[(-root)-1]=h;
		return;
	}

	PDZ2_CalcLengths_r(nodes, cl, nodes[root*2+0], h+1);
	PDZ2_CalcLengths_r(nodes, cl, nodes[root*2+1], h+1);
}

int PDZ2_BuildLengths(int *stat, int nc, byte *cl, int ml)
{
	static short nodes[1024], nlen[512];
	static short roots[512], clen[512];
	static int cnts[512];
	int nr, nn;
	int i, j, k, l;

	nr=0; nn=0;
	for(i=0; i<nc; i++)
	{
		if(!stat[i])continue;
		roots[nr]=-(i+1);
		cnts[nr]=stat[i];
		clen[nr]=0;
		nr++;
	}

	for(i=0; i<nc; i++)cl[i]=0;
	if(!nr)
	{
		printf("empty tree\n");
		return(-1);
	}


	while(nr>1)
	{
		if(cnts[0]>=cnts[1]) { j=0; k=1; }
			else { j=1; k=0; }
		for(i=2; i<nr; i++)
		{
			if(cnts[i]<=cnts[k])
			{
				j=k; k=i;
				continue;
			}
			if(cnts[i]<=cnts[j])
			{
				j=i;
				continue;
			}
		}

		nlen[nn]=((clen[j]>clen[k])?clen[j]:clen[k])+1;
		nodes[nn*2+0]=roots[j];
		nodes[nn*2+1]=roots[k];

		roots[nr]=nn;
		cnts[nr]=cnts[j]+cnts[k];
		clen[nr]=nlen[nn];

//		printf("%d %d %d\n", cnts[j], cnts[k], cnts[nr]);

		nn++; nr++;

		l=0;
		for(i=0; i<nr; i++)
		{
			if((i==j) || (i==k))continue;
			roots[l]=roots[i];
			cnts[l]=cnts[i];
			clen[l]=clen[i];
			l++;
		}
		nr=l;
	}

	l=roots[0];
	j=clen[0];
	k=j;

	i=4;
	while((i--) && (k>ml))
		k=PDZ2_BalanceTree_r(nodes, nlen, l, 0, ml);
	if(k>ml)
	{
		printf("tree balance failure\n");
		printf("tree depth %d, org %d, %d nodes\n", k, j, nn);
		return(-2);
	}

	PDZ2_CalcLengths_r(nodes, cl, l, 0);
	return(0);
}

int PDZ2_BuildLengthsAdjust(int *stat, int nc, byte *cl, int ml)
{
	int i, j;

	while(1)
	{
		j=PDZ2_BuildLengths(stat, nc, cl, ml);
		if(j<0)
			printf("PDZ2_BuildLengthsAdjust: Huff Fail %d\n", j);

		for(i=0; i<nc; i++)
			if(stat[i] && !cl[i])
				break;
		if(i>=nc)break;

		printf("PDZ2_BuildLengthsAdjust: Fiddle Adjust\n");
		for(i=0; i<nc; i++)
			stat[i]++;
		continue;
	}
	return(0);
}

int TKuLZ_BuildLengths(TKuLZ_EncState *ctx, int *stat, byte *cls)
{
	int i, j, k;

//	PDZ2_BuildLengthsAdjust(stat, 256, cls, 12);
	PDZ2_BuildLengthsAdjust(stat, 256, cls, TKULZ_HTABNB);
	
//	for(i=0; i<256; i++)
//		cls[i]=8;

	return(0);
}

int TKuLZ_HashForBytePos(TKuLZ_EncState *ctx, byte *cs)
{
	int h;

	h=cs[0];			h=(h*251)+cs[1];
	h=(h*251)+cs[2];	h=h*251+1;
	h&=(TKULZ_HASH_SZ-1);
	return(h);
}

void TKuLZ_EncHashBytePos(TKuLZ_EncState *ctx, byte *cs)
{
	int h, r;
	
	h=TKuLZ_HashForBytePos(ctx, cs);
	r=ctx->hash_rov[h];
	r=(r-1)&(TKULZ_HASH_LVL-1);
	ctx->hash_pos[(h*TKULZ_HASH_LVL)+r]=cs;
	ctx->hash_rov[h]=r;
}

void TKuLZ_EncHashByteRun(TKuLZ_EncState *ctx, byte *cs, int len)
{
	int i;
	
	for(i=0; i<len; i++)
		TKuLZ_EncHashBytePos(ctx, cs+i);
}

int TKuLZ_LookupMatch(TKuLZ_EncState *ctx,
	byte *cs, byte *cse, int *rml, int *rmd)
{
	byte *ct, *cs1, *ct1, *cs1e;
	int bl, bd;
	int h, r, d;
	int i, j;

	h=TKuLZ_HashForBytePos(ctx, cs);
	r=ctx->hash_rov[h];
	
	bl=0; bd=0;
	for(i=0; i<TKULZ_HASH_LVL; i++)
	{
		j=(r+i)&(TKULZ_HASH_LVL-1);
		ct=ctx->hash_pos[(h*TKULZ_HASH_LVL)+j];
		if(!ct)break;
		
		cs1=cs; ct1=ct;
		cs1e=cs+TKULZ_MAX_MATCH;
		if(cse<cs1e)
			cs1e=cse;
		while(cs1<cs1e)
		{
			if((*cs1)!=(*ct1))
				break;
			cs1++; ct1++;
		}

		j=cs1-cs;
		d=cs-ct;

//		if(d>=2048)
//			if(j<4)
//				j=0;

		if(d>=4096)
			if(j<5)
				j=0;
		
//		if(d>=32768)
		if(d>=16384)
//		if(d>=8192)
			if(j<7)
				j=0;

//		if(d>=65536)
//			if(j<8)
//				j=0;
		
		
		if((j>bl) && (d>0) && (d<TKULZ_MAX_DIST))
			{ bl=j; bd=d; }
	}

	*rml=bl;
	*rmd=bd;
	return(bl>=3);
}

int TKuLZ_TransEncodeBlock(TKuLZ_EncState *ctx,
	byte *dst, byte *src, int ssz)
{
	byte *cs, *ct, *cse, *csl;
	int ml, md, ll;
	int ml1, md1;
	int ml2, md2;
	int lml, lmd;
	int i0, i1, i2;
	
	cs=src; ct=dst; cse=src+ssz;
	csl=src;
	lml=-1; lmd=-1;
	
	while(cs<cse)
	{
		i0=TKuLZ_LookupMatch(ctx, cs, cse, &ml, &md);
		i1=TKuLZ_LookupMatch(ctx, cs+1, cse, &ml1, &md1);
		i2=TKuLZ_LookupMatch(ctx, cs+2, cse, &ml2, &md2);
	
//		i0=0;
		
		if(i0 && i1 && (ml1>(ml+1)))
			i0=0;
		if(i0 && i2 && (ml2>(ml+2)))
			i0=0;
	
		if(i0)
		{
			ll=cs-csl;
			
			if((ml==lml) && (md==lmd))
			{
				TKuLZ_WriteVLN(&ct, ll);
				TKuLZ_WriteVLN(&ct, 1);
				TKuLZ_WriteVLN(&ct, 0);
			}else
#if 1
				if((ml==lml) && (ml>32))
			{
				TKuLZ_WriteVLN(&ct, ll);
				TKuLZ_WriteVLN(&ct, 2);
				TKuLZ_WriteVLN(&ct, 0);
				TKuLZ_WriteVLN(&ct, md);
			}else
				if((md==lmd) && (md>64))
			{
				TKuLZ_WriteVLN(&ct, ll);
				TKuLZ_WriteVLN(&ct, 3);
				TKuLZ_WriteVLN(&ct, 0);
				TKuLZ_WriteVLN(&ct, ml-3);
			}else
#endif
			{
				TKuLZ_WriteVLN(&ct, ll);
				TKuLZ_WriteVLN(&ct, ml-3);
				TKuLZ_WriteVLN(&ct, md);
			}
			
			while(csl<cs)
				{ *ct++=*csl++; }

			TKuLZ_EncHashByteRun(ctx, cs, ml);
			cs+=ml;
			csl=cs;
			
			lml=ml; lmd=md;
		}else
		{
			TKuLZ_EncHashBytePos(ctx, cs);
			cs++;
		}
	}

	ll=cs-csl;

	TKuLZ_WriteVLN(&ct, ll);
	TKuLZ_WriteVLN(&ct, 0);
	TKuLZ_WriteVLN(&ct, 0);
	while(csl<cs)
		{ *ct++=*csl++; }
	
	return(ct-dst);
}

int TKuLZ_EncodeBlock(TKuLZ_EncState *ctx,
	byte *src, int ssz)
{
	int tsz;
	int i;

//	memset(ctx->hstab_t, 0, 256*sizeof(int));
//	memset(ctx->hstab_l, 0, 256*sizeof(int));
//	memset(ctx->hstab_d, 0, 256*sizeof(int));

	for(i=0; i<256; i++)
	{
		ctx->hstab_t[i]=0;
		ctx->hstab_l[i]=0;
		ctx->hstab_d[i]=0;
	}

#if 0
	for(i=0; i<256; i++)
	{
//		ctx->hstab_t[i]=1;
//		ctx->hstab_l[i]=1;
//		ctx->hstab_d[i]=1;
	}

	for(i=0; i<128; i++)
	{
//		ctx->hstab_d[i]=1;
	}
#endif

	tsz=TKuLZ_TransEncodeBlock(ctx, ctx->tbuf, src, ssz);
	
	if(tsz>TKULZ_TBUF_SZ)
		__debugbreak();
		
	TKuLZ_StatTransBuf(ctx, ctx->tbuf, tsz);
	
	TKuLZ_BuildLengths(ctx, ctx->hstab_t, ctx->hltab_t);
	TKuLZ_BuildLengths(ctx, ctx->hstab_l, ctx->hltab_l);
	TKuLZ_BuildLengths(ctx, ctx->hstab_d, ctx->hltab_d);
	
	TKuLZ_SetupEncTableLengths(ctx, ctx->hetab_t, ctx->hltab_t);
	TKuLZ_SetupEncTableLengths(ctx, ctx->hetab_l, ctx->hltab_l);
	TKuLZ_SetupEncTableLengths(ctx, ctx->hetab_d, ctx->hltab_d);

	TKuLZ_WriteBits(ctx, 0x4, 4);
	TKuLZ_WritePackedLengths(ctx, ctx->hltab_t);
	TKuLZ_WriteBits(ctx, 0x5, 4);
	TKuLZ_WritePackedLengths(ctx, ctx->hltab_l);
	TKuLZ_WriteBits(ctx, 0x6, 4);
	TKuLZ_WritePackedLengths(ctx, ctx->hltab_d);

	TKuLZ_WriteBits(ctx, 0x2, 4);
	TKuLZ_EncodeTransBuf(ctx, ctx->tbuf, tsz);
	
	return(0);
}

int TKuLZ_SetupEncodeBuffer(TKuLZ_EncState *ctx,
	byte *dst, int dsz)
{
	ctx->ct=dst;
	ctx->win=0;
	ctx->pos=0;
	ctx->status=0;

	ctx->hash_pos=malloc(TKULZ_HASH_SZ*TKULZ_HASH_LVL*sizeof(byte *));
	ctx->hash_rov=malloc(TKULZ_HASH_SZ*sizeof(byte));
	
	memset(ctx->hash_pos, 0, TKULZ_HASH_SZ*TKULZ_HASH_LVL*sizeof(byte *));
	memset(ctx->hash_rov, 0, TKULZ_HASH_SZ*sizeof(byte));
}

int TKuLZ_EncodeBuffer(TKuLZ_EncState *ctx,
	byte *dst, byte *src, int dsz, int ssz)
{
	byte *cs, *cse;
	int bsz;
	
	TKuLZ_SetupEncodeBuffer(ctx, dst, dsz);
	
	cs=src; cse=cs+ssz;
	while(cs<cse)
	{
		bsz=cse-cs;
		if(bsz>TKULZ_TBLK_SZ)
			bsz=TKULZ_TBLK_SZ;
		TKuLZ_EncodeBlock(ctx, cs, bsz);
		cs+=bsz;
	}
	TKuLZ_WriteBits(ctx, 0x0, 4);
	TKuLZ_FlushWriteBits(ctx);

	return(ctx->ct-dst);
}
