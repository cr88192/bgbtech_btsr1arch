#ifndef RP2POST_H12E_C
#define RP2POST_H12E_C

#ifndef RP2POST_H12D_C
#error This assumes a Unity Build strategy following H12D and an RP2 impl
#endif

typedef struct PostRp2Huff_EncState_s PostRp2Huff_EncState;

struct PostRp2Huff_EncState_s {
	byte *cs;
	byte *ct;
	byte *cse;
	byte *cte;
	u32 win;
	sbyte pos;

	u32 hfetab[3][256];
};

void PostRp2Huff_WriteBits(PostRp2Huff_EncState *ctx, int val, int bits)
{
//	if((bits>24) || (bits<0))
//		{ debug_break }

	ctx->win|=(val&((1<<bits)-1))<<ctx->pos;
	ctx->pos+=bits;
	while(ctx->pos>=8)
	{
		*ctx->ct++=ctx->win;
		ctx->win>>=8;
		ctx->pos-=8;
	}
}

void PostRp2Huff_FlushWriteBits(PostRp2Huff_EncState *ctx)
{
	while(ctx->pos>0)
	{
		*ctx->ct++=ctx->win;
		ctx->win>>=8;
		ctx->pos-=8;
	}
	ctx->pos=0;
}

int postrp2huff_log2u(int val)
{
	int i, v;
	
	v=val; i=0;
	while(v)
		{ i++; v=v>>1; }
	return(i);
}

void PostRp2Huff_WritePackVLI(PostRp2Huff_EncState *ctx, int val, int pfsz)
{
	int e;
	e=postrp2huff_log2u(val);
	PostRp2Huff_WriteBits(ctx, e, pfsz);
	PostRp2Huff_WriteBits(ctx, val, e-1);
}

void PostRp2Huff_SetupEncTableLengths(u32 *htab, byte *cls)
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
			k=PostRp2Huff_TransposeWord(tc)>>3;
			htab[i]=(l<<16)|k;
			c++;
		}
		c=c<<1;
	}
	
//	if(c>8192)
//		{ debug_break }
}

void PostRp2Huff_EncodeHuffSym(PostRp2Huff_EncState *ctx, u32 *hetab, int sym)
{
	int b;
	b=hetab[sym];
	
//	if(!((b>>16)&15))
//		{ debug_break }
	
	if(((b>>16)<1) || ((b>>16)>13))
		{ debug_break }
//		__debugbreak();
	
	PostRp2Huff_WriteBits(ctx, b, b>>16);
}

void PostRp2Huff_EncodeHuffSymbolBlob(PostRp2Huff_EncState *ctx,
	u32 *hetab, byte *buf, int len)
{
	byte *cs, *cse;
	
	cs=buf; cse=buf+len;
	while(cs<cse)
		{ PostRp2Huff_EncodeHuffSym(ctx, hetab, *cs++); }
}

void PostRp2Huff_EncodeHuffSymbolXBlob(PostRp2Huff_EncState *ctx,
	u32 *hetab, byte *buf, int len)
{
	byte *cs, *cse;
	int nbi0, nbi1, nbi2, nbi3;
	int i, j, k, l;

	PostRp2Huff_PrintBlobCheck(0, buf, len);

//	if(1)
//	if(len<512)
//	if(len<384)
//	if(len<256)
	if(len<192)
//	if(len<128)
	{
		PostRp2Huff_WriteBits(ctx, 0, 2);
		cs=buf; cse=buf+len;
		while(cs<cse)
			{ PostRp2Huff_EncodeHuffSym(ctx, hetab, *cs++); }
		return;
	}

	nbi0=0;	nbi1=0;
	nbi2=0;	nbi3=0;
//	l=len>>2;
	l=(len+3)>>2;
	for(i=0; i<l; i++)
	{
		nbi0+=hetab[buf[i*4+0]]>>16;
		nbi1+=hetab[buf[i*4+1]]>>16;
		nbi2+=hetab[buf[i*4+2]]>>16;
		nbi3+=hetab[buf[i*4+3]]>>16;
	}

	PostRp2Huff_WriteBits(ctx, 1, 2);
	PostRp2Huff_WritePackVLI(ctx, nbi0, 5);
	PostRp2Huff_WritePackVLI(ctx, nbi1, 5);
	PostRp2Huff_WritePackVLI(ctx, nbi2, 5);
	PostRp2Huff_WritePackVLI(ctx, nbi3, 5);

	for(i=0; i<l; i++)
		{ PostRp2Huff_EncodeHuffSym(ctx, hetab, buf[i*4+0]); }
	for(i=0; i<l; i++)
		{ PostRp2Huff_EncodeHuffSym(ctx, hetab, buf[i*4+1]); }
	for(i=0; i<l; i++)
		{ PostRp2Huff_EncodeHuffSym(ctx, hetab, buf[i*4+2]); }
	for(i=0; i<l; i++)
		{ PostRp2Huff_EncodeHuffSym(ctx, hetab, buf[i*4+3]); }
}

void PostRp2Huff_EncodeRawBlob(PostRp2Huff_EncState *ctx,
	byte *buf, int len)
{
	byte *cs, *cse;
	
	cs=buf; cse=buf+len;
	while(cs<cse)
		{ PostRp2Huff_WriteBits(ctx, *cs++, 8); }
}

int PostRp2Huff_WritePackedLengths(PostRp2Huff_EncState *ctx, byte *cls)
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
				PostRp2Huff_WriteBits(ctx, 0xF, 4);
//				PostRp2Huff_WriteBits(ctx, 3, 2);
				PostRp2Huff_WriteBits(ctx, 1, 2);
				PostRp2Huff_WriteBits(ctx, 0, 6);
				break;
			}
			
			if((n>=3) && (n<=18))
			{
				PostRp2Huff_WriteBits(ctx, 0xE, 4);
				PostRp2Huff_WriteBits(ctx, n-3, 4);
				s=s1;
				continue;
			}
			if((n>=19) && (n<=82))
			{
				PostRp2Huff_WriteBits(ctx, 0xF, 4);
				PostRp2Huff_WriteBits(ctx, 0, 2);
				PostRp2Huff_WriteBits(ctx, n-19, 6);
				s=s1;
				continue;
			}

			if(n>82)
			{
				PostRp2Huff_WriteBits(ctx, 0xF, 4);
				PostRp2Huff_WriteBits(ctx, 0, 2);
				PostRp2Huff_WriteBits(ctx, 63, 6);
				s+=81;
				continue;
			}

			ll=0;
			PostRp2Huff_WriteBits(ctx, 0x0, 4);
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
				PostRp2Huff_WriteBits(ctx, 0xF, 4);
				PostRp2Huff_WriteBits(ctx, 1, 2);
				PostRp2Huff_WriteBits(ctx, n-3, 6);
				s=s1;
				continue;
			}

#if 1
			if(n>66)
			{
				PostRp2Huff_WriteBits(ctx, 0xF, 4);
				PostRp2Huff_WriteBits(ctx, 1, 2);
				PostRp2Huff_WriteBits(ctx, 63, 6);
				s+=65;
				continue;
			}
#endif
		}
#endif

		if((l>=1) && (l<=13))
		{
			PostRp2Huff_WriteBits(ctx, l, 4);
			ll=l;
			continue;
		}
		
//		{ debug_break }
	}
	return(0);
}

int PostRp2Huff_BalanceTree_r(
	short *nodes, short *nlen, int root, int h, int ml)
{
	int h0, h1, h2, h3;
	int l0, l1, l2;

	if(root<0)
	{
//		printf("L");
		return(0);
	}

//	printf("{");

	h1=PostRp2Huff_BalanceTree_r(nodes, nlen, nodes[root*2+0], h+1, ml);
	h2=PostRp2Huff_BalanceTree_r(nodes, nlen, nodes[root*2+1], h+1, ml);
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

void PostRp2Huff_CalcLengths_r(short *nodes, byte *cl, int root, int h)
{
	if(root<0)
	{
		cl[(-root)-1]=h;
		return;
	}

	PostRp2Huff_CalcLengths_r(nodes, cl, nodes[root*2+0], h+1);
	PostRp2Huff_CalcLengths_r(nodes, cl, nodes[root*2+1], h+1);
}

int PostRp2Huff_BuildLengths(int *stat, int nc, byte *cl, int ml)
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

	i=8;
	while((i--) && (k>ml))
		k=PostRp2Huff_BalanceTree_r(nodes, nlen, l, 0, ml);
	if(k>ml)
	{
		printf("tree balance failure\n");
		printf("tree depth %d, org %d, %d nodes\n", k, j, nn);
		return(-2);
	}

	PostRp2Huff_CalcLengths_r(nodes, cl, l, 0);
	return(0);
}

int PostRp2Huff_BuildLengthsAdjust(int *stat, int nc, byte *cl, int ml)
{
	int i, j;

	while(1)
	{
		j=PostRp2Huff_BuildLengths(stat, nc, cl, ml);
		if(j<0)
			printf("PostRp2Huff_BuildLengthsAdjust: Huff Fail %d\n", j);

		for(i=0; i<nc; i++)
			if(stat[i] && !cl[i])
				break;
		if(i>=nc)break;

		printf("PostRp2Huff_BuildLengthsAdjust: Fiddle Adjust\n");
		for(i=0; i<nc; i++)
			stat[i]++;
		continue;
	}
	return(0);
}

int PostRp2Huff_BuildLengths2(int *stat, byte *cls)
{
	int i, j, k;

	PostRp2Huff_BuildLengthsAdjust(stat, 256, cls, POSTRP2HUFF_HTABNB);
	return(0);
}


int PostRp2Huff_StatBufferRp2(byte *ibuf, int ibsz,
	int *stat_t, int *stat_l, int *stat_d)
{
	byte *cs, *cse, *cs1, *csr, *ct;
	u32 tag;
	int tsz, nr;
	int i, j, k;

//	ctx=&t_ctx;

	cs=ibuf;
	cse=ibuf+ibsz;
	while(cs<cse)
	{
		tag=*(u32 *)cs;
		if(!(tag&0x01))			{ tsz=2; nr=(tag>>1)&7; }
		else if(!(tag&0x02))	{ tsz=3; nr=(tag>>2)&7; }
		else if(!(tag&0x04))	{ tsz=4; nr=(tag>>3)&7; }
		else if(!(tag&0x08))	{ tsz=1; nr=(((tag>>4)&15)+1)*8; }
		else if(!(tag&0x10))
		{
			nr=(tag>>5)&7;
			if(tag&0x100)		{ tsz=3; }
			else				{ tsz=2; }
		}
		else if(!(tag&0x20))	{ tsz=1; nr=(tag>>6)&3; }
		else if(!(tag&0x40))	{ tsz=2; nr=(((tag>>7)&511)+1)*8; }
		else if(!(tag&0x80))
		{
			if(tag&0x100)
				{ tsz=6; nr=(tag>>9)&7; }
			else
				{ tsz=2; nr=0; }
		}
		else
		{
			if(!(tag&0x100))
				{ tsz=5; nr=(tag>>9)&7; }
			else
				{ tsz=-1; }
		}
		
		if(tsz<1)
			return(-1);
		
		if((cs+(tsz+nr))>cse)
			break;
		
		if(nr>=3072)
		{
			stat_l[cs[0]]++;
			stat_l[cs[1]]++;
		}
		
		stat_t[*cs++]++;
		for(i=1; i<tsz; i++)
			stat_d[*cs++]++;
		for(i=0; i<nr; i++)
			stat_l[*cs++]++;
	}
	return(cs-ibuf);
}

int PostRp2Huff_EstimateBufferRp2HuffSize(byte *ibuf, int ibsz)
{
	int stat_t[256];
	int stat_l[256];
	int stat_d[256];
	byte cl_t[256];
	byte cl_l[256];
	byte cl_d[256];
	int psz_t, psz_l, psz_d, psz_al;
	int bsz_t, bsz_l, bsz_d;
	int tsz_t, tsz_l, tsz_d;
	int pad_t, pad_l, pad_d;
	int raw_t, raw_l, raw_d;
	PostRp2Huff_EncState *ctx;
	byte *cs, *cse, *cs0, *cslh;
	int tti, needhuff;
	int tot;
	int i, j, k, l;

	tot=0;
	cs=ibuf;
	cse=ibuf+ibsz;

	while(cs<cse)
	{
		memset(stat_t, 0, 256*sizeof(int));
		memset(stat_l, 0, 256*sizeof(int));
		memset(stat_d, 0, 256*sizeof(int));
		
		k=cse-cs;
		l=1<<20;
		if(k<l)
			l=k;
		i=PostRp2Huff_StatBufferRp2(cs, l, stat_t, stat_l, stat_d);
		if(i<=0)
			return(-1);
		cs+=i;
		l=i;
		
		PostRp2Huff_BuildLengths2(stat_t, cl_t);
		PostRp2Huff_BuildLengths2(stat_l, cl_l);
		PostRp2Huff_BuildLengths2(stat_d, cl_d);
		
		psz_t=0;
		psz_l=0;
		psz_d=0;
		for(i=0; i<256; i++)
		{
			psz_t+=cl_t[i]*stat_t[i];
			psz_l+=cl_l[i]*stat_l[i];
			psz_d+=cl_d[i]*stat_d[i];
		}
		
		k=(l>>12)*(16*3+16*4*3+16);
		k+=psz_t;
		k+=psz_l;
		k+=psz_d;
		
		k=((k+7)>>3);
		tot+=k;
	}
	
	return(tot);
}

/* Estimate a lower bound for what entropy coding could give. */
int PostRp2Huff_EstimateBufferRp2MinEntropySize(byte *ibuf, int ibsz)
{
	int stat_t[256];
	int stat_l[256];
	int stat_d[256];
	byte cl_t[256];
	byte cl_l[256];
	byte cl_d[256];
	int psz_t, psz_l, psz_d, psz_al;
	int bsz_t, bsz_l, bsz_d;
	int tsz_t, tsz_l, tsz_d;
	int pad_t, pad_l, pad_d;
	int raw_t, raw_l, raw_d;
	PostRp2Huff_EncState *ctx;
	byte *cs, *cse, *cs0, *cslh;
	int tti, needhuff;
	int tot;
	int i, j, k, l;

	tot=0;
	cs=ibuf;
	cse=ibuf+ibsz;

	while(cs<cse)
	{
		memset(stat_t, 0, 256*sizeof(int));
		memset(stat_l, 0, 256*sizeof(int));
		memset(stat_d, 0, 256*sizeof(int));

#if 0		
		for(i=0; i<256; i++)
		{
			stat_t[i]=1;
			stat_l[i]=1;
			stat_d[i]=1;
		}
#endif
		
		k=cse-cs;
		l=1<<18;
		if(k<l)
			l=k;
		i=PostRp2Huff_StatBufferRp2(cs, l, stat_t, stat_l, stat_d);
		if(i<=0)
			return(-1);
		cs+=i;
		l=i;
		
		PostRp2Huff_BuildLengths2(stat_t, cl_t);
		PostRp2Huff_BuildLengths2(stat_l, cl_l);
		PostRp2Huff_BuildLengths2(stat_d, cl_d);
		
		psz_t=0;
		psz_l=0;
		psz_d=0;
		for(i=0; i<256; i++)
		{
			psz_t+=cl_t[i]*stat_t[i];
			psz_l+=cl_l[i]*stat_l[i];
			psz_d+=cl_d[i]*stat_d[i];
		}
		
		k=0;
		k+=psz_t;
		k+=psz_l;
		k+=psz_d;
		
		k=((k+7)>>3);
		tot+=k;
	}
	
	return(tot);
}

int PostRp2Huff_SplitBuffersRp2(byte *ibuf, int ibsz, int obmax,
	byte *buf_t, byte *buf_l, byte *buf_d,
	int *rsz_t, int *rsz_l, int *rsz_d)
{
	byte *cs, *cse, *cs1, *csr, *ct;
	byte *ct_t, *ct_l, *ct_d;
	byte *cte_t, *cte_l, *cte_d;
	u32 tag;
	int tsz, nr;
	int i, j, k;

//	ctx=&t_ctx;

	ct_t=buf_t;
	ct_l=buf_l;
	ct_d=buf_d;

	cte_t=buf_t+obmax;
	cte_l=buf_l+obmax;
	cte_d=buf_d+obmax;

	cs=ibuf;
	cse=ibuf+ibsz;
	while(cs<cse)
	{
		tag=*(u32 *)cs;
		if(!(tag&0x01))			{ tsz=2; nr=(tag>>1)&7; }
		else if(!(tag&0x02))	{ tsz=3; nr=(tag>>2)&7; }
		else if(!(tag&0x04))	{ tsz=4; nr=(tag>>3)&7; }
		else if(!(tag&0x08))	{ tsz=1; nr=(((tag>>4)&15)+1)*8; }
		else if(!(tag&0x10))
		{
			nr=(tag>>5)&7;
			if(tag&0x100)		{ tsz=3; }
			else				{ tsz=2; }
		}
		else if(!(tag&0x20))
			{ tsz=1; nr=(tag>>6)&3; }
		else if(!(tag&0x40))
			{ tsz=2; nr=(((tag>>7)&511)+1)*8; }
		else if(!(tag&0x80))
		{
			if(tag&0x100)
				{ tsz=6; nr=(tag>>9)&7; }
			else
				{ tsz=2; nr=0; }
		}
		else if(!(tag&0x100))
			{ tsz=5; nr=(tag>>9)&7; }
		else
			{ tsz=-1; }
		
		if((ct_t+1)>=cte_t)
			break;
		if((ct_l+nr)>=cte_l)
			break;
		if((ct_d+(tsz-1))>=cte_d)
			break;
		
		if(tsz<1)
			return(-1);
		
		*ct_t++=*cs++;
		for(i=1; i<tsz; i++)
			*ct_d++=*cs++;
		for(i=0; i<nr; i++)
			*ct_l++=*cs++;
	}
	
	*rsz_t=ct_t-buf_t;
	*rsz_l=ct_l-buf_l;
	*rsz_d=ct_d-buf_d;

	return(cs-ibuf);
}

// #define POSTRP2HUFF_RAWBIAS		1.06
#define POSTRP2HUFF_RAWBIAS		1.05
// #define POSTRP2HUFF_RAWBIAS		1.03
// #define POSTRP2HUFF_RAWBIAS		1.00
// #define POSTRP2HUFF_RAWBIAS		0.95

int PostRp2Huff_EncodeBufferPostRp2B(
	byte *obuf, byte *ibuf, int obsz, int ibsz)
{
	static byte blob_t[POSTRP2HUFF_MAXBLOB+16];
	static byte blob_l[POSTRP2HUFF_MAXBLOB+16];
	static byte blob_d[POSTRP2HUFF_MAXBLOB+16];
	static PostRp2Huff_EncState t_ctx;
	int stat_t[256];
	int stat_l[256];
	int stat_d[256];
	byte cl_t[256];
	byte cl_l[256];
	byte cl_d[256];
	int psz_t, psz_l, psz_d, psz_al;
	int bsz_t, bsz_l, bsz_d;
	int tsz_t, tsz_l, tsz_d;
	int pad_t, pad_l, pad_d;
	int raw_t, raw_l, raw_d;
	PostRp2Huff_EncState *ctx;
	byte *cs, *cse, *cs0, *cslh;
	int tti, needhuff;
	
	int i, j, k, l;

	ctx=&t_ctx;
	
//	l=PostRp2Huff_EstimateBufferRp2HuffSize(ibuf, ibsz);
//	printf("  Est: %6d -> %6d %.2f%%\n", ibsz, l, (100.0*l)/ibsz);
	
	memset(stat_t, 0, 256*sizeof(int));
	memset(stat_l, 0, 256*sizeof(int));
	memset(stat_d, 0, 256*sizeof(int));
	k=ibsz;
//	l=(1<<20)+16384;
	l=(1<<21);
	if(k>l)
		k=l;
	i=PostRp2Huff_StatBufferRp2(ibuf, k, stat_t, stat_l, stat_d);
	if(i<0)
		return(-1);
	
	PostRp2Huff_BuildLengths2(stat_t, cl_t);
	PostRp2Huff_BuildLengths2(stat_l, cl_l);
	PostRp2Huff_BuildLengths2(stat_d, cl_d);

#if 0
	for(i=0; i<16; i++)
	{
		for(j=0; j<16; j++)
		{
			printf("%2d ", cl_t[i*16+j]);
		}
		printf("\n");
	}
	printf("\n");
	
	for(i=0; i<16; i++)
	{
		for(j=0; j<16; j++)
		{
			printf("%2d ", cl_l[i*16+j]);
		}
		printf("\n");
	}
	printf("\n");
	
	for(i=0; i<16; i++)
	{
		for(j=0; j<16; j++)
		{
			printf("%2d ", cl_d[i*16+j]);
		}
		printf("\n");
	}
	printf("\n");

	l=0; k=0;
	for(i=0; i<256; i++)
		{ l+=cl_t[i]*stat_t[i]; k+=stat_t[i]; }
	psz_t=(l+7)>>3; bsz_t=k;

	l=0; k=0;
	for(i=0; i<256; i++)
		{ l+=cl_l[i]*stat_l[i]; k+=stat_l[i]; }
	psz_l=(l+7)>>3; bsz_l=k;

	l=0; k=0;
	for(i=0; i<256; i++)
		{ l+=cl_d[i]*stat_d[i]; k+=stat_d[i]; }
	psz_d=(l+7)>>3; bsz_d=k;
#endif

	pad_t=0;
	pad_l=0;
	pad_d=0;
	for(i=0; i<256; i++)
	{
		if(!cl_t[pad_t] || (cl_t[i] && (cl_t[i]<cl_t[pad_t])))
			pad_t=i;
		if(!cl_l[pad_l] || (cl_l[i] && (cl_l[i]<cl_l[pad_l])))
			pad_l=i;
		if(!cl_d[pad_d] || (cl_d[i] && (cl_d[i]<cl_d[pad_d])))
			pad_d=i;
	}

#if 0	
	printf("  T: %6d -> %6d %.2f%%\n", bsz_t, psz_t, (100.0*psz_t)/bsz_t);
	printf("  L: %6d -> %6d %.2f%%\n", bsz_l, psz_l, (100.0*psz_l)/bsz_l);
	printf("  D: %6d -> %6d %.2f%%\n", bsz_d, psz_d, (100.0*psz_d)/bsz_d);
#endif

	PostRp2Huff_SetupEncTableLengths(ctx->hfetab[0], cl_t);
	PostRp2Huff_SetupEncTableLengths(ctx->hfetab[1], cl_l);
	PostRp2Huff_SetupEncTableLengths(ctx->hfetab[2], cl_d);
	
	obuf[0]=0x20;
	obuf[1]=0x00;
	
	ctx->ct=obuf+2;
	ctx->cte=obuf+obsz;
	ctx->pos=0;
	ctx->win=0;
	
	needhuff=1;
	cslh=ibuf;
	cs=ibuf; cse=ibuf+ibsz;
	while(cs<cse)
	{
		cs0=cs;

#if 1
		if((cs-cslh)>(1<<20))
		{
			/* Rebuild Huffman tables once per MB for larger streams. */
			memset(stat_t, 0, 256*sizeof(int));
			memset(stat_l, 0, 256*sizeof(int));
			memset(stat_d, 0, 256*sizeof(int));
			cslh=cs;

#if 0
			for(i=0; i<256; i++)
			{
				stat_t[i]=1;
				stat_l[i]=1;
				stat_d[i]=1;
			}
#endif
			
//			l=(1<<20)+16384;
			l=(1<<21);
			k=cse-cs;
			if(l<k)		k=l;
			i=PostRp2Huff_StatBufferRp2(cs, k, stat_t, stat_l, stat_d);
			if(i<0)
				return(-1);
			
			PostRp2Huff_BuildLengths2(stat_t, cl_t);
			PostRp2Huff_BuildLengths2(stat_l, cl_l);
			PostRp2Huff_BuildLengths2(stat_d, cl_d);
			needhuff=1;

			PostRp2Huff_SetupEncTableLengths(ctx->hfetab[0], cl_t);
			PostRp2Huff_SetupEncTableLengths(ctx->hfetab[1], cl_l);
			PostRp2Huff_SetupEncTableLengths(ctx->hfetab[2], cl_d);

			pad_t=0;
			pad_l=0;
			pad_d=0;
			for(i=0; i<256; i++)
			{
				if(!cl_t[pad_t] || (cl_t[i] && (cl_t[i]<cl_t[pad_t])))
					pad_t=i;
				if(!cl_l[pad_l] || (cl_l[i] && (cl_l[i]<cl_l[pad_l])))
					pad_l=i;
				if(!cl_d[pad_d] || (cl_d[i] && (cl_d[i]<cl_d[pad_d])))
					pad_d=i;
			}
		}
#endif
		
		if(!needhuff && ((cs[0]&0x7F)==0x3F) && (cs[1]>=0xC0))
		{
			/* Large Raw-Bytes Blob, Encode Special */
			k=(cs[1]<<1)|(cs[0]>>7);
			tsz_l=2+((k+1)*8);

			l=0;
			for(i=0; i<tsz_l; i++)
				{ l+=cl_l[blob_l[i]]; }
			
			k=l>>3;
			if((k*POSTRP2HUFF_RAWBIAS)>tsz_l)
			{
				/* Raw Bytes Chunk */
				PostRp2Huff_WriteBits(ctx, 1, 4);
				PostRp2Huff_WritePackVLI(ctx, tsz_l, 4);
				PostRp2Huff_EncodeRawBlob(ctx, cs, tsz_l);
				cs+=tsz_l;
				continue;
			}

#if 1
			l=0;
			for(i=0; i<tsz_l; i++)
			{
				j=cl_t[cs[i]];
				if(j<1) {l=-1; break; }
				l+=j;
			}
			psz_t=l;

			l=0;
			for(i=0; i<tsz_l; i++)
			{
				j=cl_l[cs[i]];
				if(j<1) {l=-1; break; }
				l+=j;
			}
			psz_l=l;

			l=0;
			for(i=0; i<tsz_l; i++)
			{
				j=cl_d[cs[i]];
				if(j<1) {l=-1; break; }
				l+=j;
			}
			psz_d=l;
#endif
		
			/* Single-Table */
			tti=1;
			l=psz_l;
			if((psz_t>0) && (psz_t<l))
				{ tti=0; l=psz_t; }
			if((psz_d>0) && (psz_d<l))
				{ tti=2; l=psz_d; }
			
			PostRp2Huff_WriteBits(ctx, 2, 4);
			PostRp2Huff_WriteBits(ctx, tti, 2);	//TTi: Lit
			PostRp2Huff_WriteBits(ctx, 2, 2);	//TTg: Reuse Table

			PostRp2Huff_WritePackVLI(ctx, tsz_l, 4);
			PostRp2Huff_EncodeHuffSymbolXBlob(ctx,
				ctx->hfetab[tti], cs, tsz_l);
			cs+=tsz_l;
			continue;
		}
		
		i=PostRp2Huff_SplitBuffersRp2(cs, cse-cs, POSTRP2HUFF_MAXBLOB,
			blob_t, blob_l, blob_d,
			&tsz_t, &tsz_l, &tsz_d);
		if(i<0)
			return(-1);
			
		if(!i)
		{
			i=PostRp2Huff_SplitBuffersRp2(cs, cse-cs, POSTRP2HUFF_MAXBLOB,
				blob_t, blob_l, blob_d,
				&tsz_t, &tsz_l, &tsz_d);
			return(-1);
		}

		cs+=i;

		blob_t[tsz_t+0]=pad_t;		blob_t[tsz_t+1]=pad_t;
		blob_t[tsz_t+2]=pad_t;		blob_t[tsz_t+3]=pad_t;

		blob_l[tsz_l+0]=pad_l;		blob_l[tsz_l+1]=pad_l;
		blob_l[tsz_l+2]=pad_l;		blob_l[tsz_l+3]=pad_l;

		blob_d[tsz_d+0]=pad_d;		blob_d[tsz_d+1]=pad_d;
		blob_d[tsz_d+2]=pad_d;		blob_d[tsz_d+3]=pad_d;

#if 1
		l=0;
		for(i=0; i<tsz_t; i++)
			{ l+=cl_t[blob_t[i]]; }
		psz_t=l;

		l=0;
		for(i=0; i<tsz_l; i++)
			{ l+=cl_l[blob_l[i]]; }
		psz_l=l;

		l=0;
		for(i=0; i<tsz_d; i++)
			{ l+=cl_d[blob_d[i]]; }
		psz_d=l;
		
		j=cs-cs0;
		l=0;
		for(i=0; i<j; i++)
		{
			k=cl_l[cs0[i]];
			if(k<1) { l=-1; break; }
			l+=k;
		}
		psz_al=l;
#endif

		j=cs-cs0;
		k=(psz_t+psz_l+psz_d+10+48)>>3;
		if(!needhuff && ((k*POSTRP2HUFF_RAWBIAS)>j))
		{
			/* If chunk compresses poorly, emit as raw blob. */
			j=cs-cs0;
			PostRp2Huff_WriteBits(ctx, 1, 4);
			PostRp2Huff_WritePackVLI(ctx, j, 4);
			PostRp2Huff_EncodeRawBlob(ctx, cs0, j);
			continue;
		}

#if 0
		j=cs-cs0;
		k=(psz_t+psz_l+psz_d+10+48);
		if(!needhuff && (psz_al>0) &&
			((psz_al*POSTRP2HUFF_RAWBIAS)<k))
		{
			/* Single-Table */
			tti=1;
			PostRp2Huff_WriteBits(ctx, 2, 4);
			PostRp2Huff_WriteBits(ctx, tti, 2);	//TTi: Lit
			PostRp2Huff_WriteBits(ctx, 2, 2);	//TTg: Reuse Table

			PostRp2Huff_WritePackVLI(ctx, j, 4);
			PostRp2Huff_EncodeHuffSymbolXBlob(ctx,
				ctx->hfetab[tti], cs0, j);
//			cs+=tsz_l;
			continue;
		}
#endif

		raw_t=((psz_t>>3)*POSTRP2HUFF_RAWBIAS)>tsz_t;
		raw_l=((psz_l>>3)*POSTRP2HUFF_RAWBIAS)>tsz_l;
		raw_d=((psz_d>>3)*POSTRP2HUFF_RAWBIAS)>tsz_d;

		/* Multi-Table Chunk */
		PostRp2Huff_WriteBits(ctx, 3, 4);
		
		if(needhuff)
		{
			raw_t=0;
			raw_l=0;
			raw_d=0;
			needhuff=0;

			PostRp2Huff_WriteBits(ctx, 1, 2);
			PostRp2Huff_WritePackedLengths(ctx, cl_t);
			PostRp2Huff_WriteBits(ctx, 1, 2);
			PostRp2Huff_WritePackedLengths(ctx, cl_l);
			PostRp2Huff_WriteBits(ctx, 1, 2);
			PostRp2Huff_WritePackedLengths(ctx, cl_d);
		}else
		{
			PostRp2Huff_WriteBits(ctx, raw_t?0:2, 2);
			PostRp2Huff_WriteBits(ctx, raw_l?0:2, 2);
			PostRp2Huff_WriteBits(ctx, raw_d?0:2, 2);
		}

		PostRp2Huff_WritePackVLI(ctx, tsz_t, 4);
		PostRp2Huff_WritePackVLI(ctx, tsz_l, 4);
		PostRp2Huff_WritePackVLI(ctx, tsz_d, 4);

		if(raw_t)
		{
			PostRp2Huff_EncodeRawBlob(ctx, blob_t, tsz_t);
		}else
		{
			PostRp2Huff_EncodeHuffSymbolXBlob(ctx,
				ctx->hfetab[0], blob_t, tsz_t);
		}

		if(raw_l)
		{
			PostRp2Huff_EncodeRawBlob(ctx, blob_l, tsz_l);
		}else
		{
			PostRp2Huff_EncodeHuffSymbolXBlob(ctx,
				ctx->hfetab[1], blob_l, tsz_l);
		}

		if(raw_d)
		{
			PostRp2Huff_EncodeRawBlob(ctx, blob_d, tsz_d);
		}else
		{
			PostRp2Huff_EncodeHuffSymbolXBlob(ctx,
				ctx->hfetab[2], blob_d, tsz_d);
		}
	}
	PostRp2Huff_WriteBits(ctx, 0, 4);
	PostRp2Huff_FlushWriteBits(ctx);
	
	return(ctx->ct-obuf);
}

int PostRp2Huff_EncodeBufferPostRp2(byte *obuf, byte *ibuf, int ibsz)
{
	return(PostRp2Huff_EncodeBufferPostRp2B(obuf, ibuf, ibsz*1.5, ibsz));
}

int PostRp2Huff_EncodeBufferPostRp2Test(byte *obuf, byte *ibuf, int ibsz)
{
	static byte *st_tbuf;
	static byte st_sztbuf;
	int osz, tsz;
	
	if(st_tbuf && (ibsz>=st_sztbuf))
	{
		free(st_tbuf);
		st_tbuf=NULL;
	}
	
	if(!st_tbuf)
	{
		tsz=4096;
		while(tsz<=ibsz)
			tsz=tsz+(tsz>>1);
		st_tbuf=malloc(tsz);
		st_sztbuf=tsz;
	}
	
	printblob_rov=0;
	osz=PostRp2Huff_EncodeBufferPostRp2(obuf, ibuf, ibsz);
//	printf("\n");

	printblob_rov=0;
	tsz=PostRp2Huff_DecodeBufferPostRp2(st_tbuf, obuf, osz);

//	printf("Enc %d -> %d\n", ibsz, osz);
	
	if(tsz!=ibsz)
	{
		printf("Size Error %d -> %d\n", ibsz, tsz);
		return(-1);
	}
	
	if(memcmp(st_tbuf, ibuf, ibsz))
	{
		printf("Data Error %d -> %d\n", ibsz, tsz);
		return(-1);
	}
	
	return(osz);
}

int PostRp2Huff_EncodeBufferPostRp2TestB(
	byte *obuf, byte *ibuf, int obsz, int ibsz)
{
	return(PostRp2Huff_EncodeBufferPostRp2Test(obuf, ibuf, ibsz));
}

#endif
