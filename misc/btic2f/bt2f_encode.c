typedef struct BTIC2F_EncState_s BTIC2F_EncState;

struct BTIC2F_EncState_s {
	byte *ct;
	u32 win;
	sbyte pos;
	byte status;
	
	int xs, ys;
	int bxs, bys;
	int fl;
	
//	s16 *imgbuf_y;
//	s16 *imgbuf_u;
//	s16 *imgbuf_v;
	
	int *blkbuf_y;
	int *blkbuf_u;
	int *blkbuf_v;

	int dcp_y;			//DC predictor, Y
	int dcp_u;			//DC predictor, U
	int dcp_v;			//DC predictor, V
	
	int yeps;			//Y epsilon
	byte	is444;		//high quality mode
	
	u32 hetab_dc[256];
	u32 hetab_ac[256];

	byte hltab_dc[256];
	byte hltab_ac[256];

	int hstab_dc[256];
	int hstab_ac[256];
	
	byte qtab_y[64];
	byte qtab_uv[64];

void (*EncodeTransImageBlock)(
	BTIC2F_EncState *ctx, int x, int y,
	byte *iptr, int xstride, int ystride,
	int xclip, int yclip);
void (*StatTransBlock)(BTIC2F_EncState *ctx, int x, int y);
void (*EncodeTransBlock)(BTIC2F_EncState *ctx, int x, int y);

};

BTIC2F_EncState *BTIC2F_AllocEncodeContext()
{
	BTIC2F_EncState *ctx;
	ctx=malloc(sizeof(BTIC2F_EncState));
	memset(ctx, 0, sizeof(BTIC2F_EncState));
	return(ctx);
}

void BTIC2F_WriteBits(BTIC2F_EncState *ctx, int val, int bits)
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

void BTIC2F_FlushWriteBits(BTIC2F_EncState *ctx)
{
	while(ctx->pos>0)
	{
		*ctx->ct++=ctx->win;
		ctx->win>>=8;
		ctx->pos-=8;
	}
	ctx->pos=0;
}

void BTIC2F_SetupEncTableLengths(BTIC2F_EncState *ctx, u32 *htab, byte *cls)
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
			k=BTIC2F_TransposeWord(tc)>>3;
			htab[i]=(l<<16)|k;
			c++;
		}
		c=c<<1;
	}
	
//	if(c>8192)
//		__debugbreak();
}

void BTIC2F_EncodeHuffSym(BTIC2F_EncState *ctx, u32 *hetab, int sym)
{
	int b;
	b=hetab[sym];
	
	if(!((b>>16)&15))
		__debugbreak();
	
	BTIC2F_WriteBits(ctx, b, b>>16);
}

int BTIC2F_SetupEncodeBuffer(BTIC2F_EncState *ctx,
	byte *dst, int dsz)
{
	ctx->ct=dst;
	ctx->win=0;
	ctx->pos=0;
	ctx->status=0;
	return(0);
}


int BTIC2F_WritePackedLengths(BTIC2F_EncState *ctx, byte *cls)
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
				BTIC2F_WriteBits(ctx, 0xF, 4);
//				BTIC2F_WriteBits(ctx, 3, 2);
				BTIC2F_WriteBits(ctx, 1, 2);
				BTIC2F_WriteBits(ctx, 0, 6);
				break;
			}
			
			if((n>=3) && (n<=18))
			{
				BTIC2F_WriteBits(ctx, 0xE, 4);
				BTIC2F_WriteBits(ctx, n-3, 4);
				s=s1;
				continue;
			}
			if((n>=19) && (n<=82))
			{
				BTIC2F_WriteBits(ctx, 0xF, 4);
				BTIC2F_WriteBits(ctx, 0, 2);
				BTIC2F_WriteBits(ctx, n-19, 6);
				s=s1;
				continue;
			}

			if(n>82)
			{
				BTIC2F_WriteBits(ctx, 0xF, 4);
				BTIC2F_WriteBits(ctx, 0, 2);
				BTIC2F_WriteBits(ctx, 63, 6);
				s+=81;
				continue;
			}

			ll=0;
			BTIC2F_WriteBits(ctx, 0x0, 4);
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
				BTIC2F_WriteBits(ctx, 0xF, 4);
				BTIC2F_WriteBits(ctx, 1, 2);
				BTIC2F_WriteBits(ctx, n-3, 6);
				s=s1;
				continue;
			}

#if 1
			if(n>66)
			{
				BTIC2F_WriteBits(ctx, 0xF, 4);
				BTIC2F_WriteBits(ctx, 1, 2);
				BTIC2F_WriteBits(ctx, 63, 6);
				s+=65;
				continue;
			}
#endif
		}
#endif

		if((l>=1) && (l<=13))
		{
			BTIC2F_WriteBits(ctx, l, 4);
			ll=l;
			continue;
		}
		
		__debugbreak();
	}
	return(0);
}

int BTIC2F_BalanceTree_r(short *nodes, short *nlen, int root, int h, int ml)
{
	int h0, h1, h2, h3;
	int l0, l1, l2;

	if(root<0)
	{
//		printf("L");
		return(0);
	}

//	printf("{");

	h1=BTIC2F_BalanceTree_r(nodes, nlen, nodes[root*2+0], h+1, ml);
	h2=BTIC2F_BalanceTree_r(nodes, nlen, nodes[root*2+1], h+1, ml);
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

void BTIC2F_CalcLengths_r(short *nodes, byte *cl, int root, int h)
{
	if(root<0)
	{
		cl[(-root)-1]=h;
		return;
	}

	BTIC2F_CalcLengths_r(nodes, cl, nodes[root*2+0], h+1);
	BTIC2F_CalcLengths_r(nodes, cl, nodes[root*2+1], h+1);
}

int BTIC2F_BuildLengthsI(int *stat, int nc, byte *cl, int ml)
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
		k=BTIC2F_BalanceTree_r(nodes, nlen, l, 0, ml);
	if(k>ml)
	{
		printf("tree balance failure\n");
		printf("tree depth %d, org %d, %d nodes\n", k, j, nn);
		return(-2);
	}

	BTIC2F_CalcLengths_r(nodes, cl, l, 0);
	return(0);
}

int BTIC2F_BuildLengthsAdjust(int *stat, int nc, byte *cl, int ml)
{
	int i, j;

	while(1)
	{
		j=BTIC2F_BuildLengthsI(stat, nc, cl, ml);
		if(j<0)
			printf("BTIC2F_BuildLengthsAdjust: Huff Fail %d\n", j);

		for(i=0; i<nc; i++)
			if(stat[i] && !cl[i])
				break;
		if(i>=nc)break;

		printf("BTIC2F_BuildLengthsAdjust: Fiddle Adjust\n");
		for(i=0; i<nc; i++)
			stat[i]++;
		continue;
	}
	return(0);
}

int BTIC2F_BuildLengths(BTIC2F_EncState *ctx, int *stat, byte *cls)
{
	int i, j, k;

//	BTIC2F_BuildLengthsAdjust(stat, 256, cls, 12);
	BTIC2F_BuildLengthsAdjust(stat, 256, cls, BTIC2F_HTABNB);
	
//	for(i=0; i<256; i++)
//		cls[i]=8;

	return(0);
}




void BTIC2F_EncodeValueDcU(BTIC2F_EncState *ctx, int val)
{
	int eb, v;

	if(val<0)
		__debugbreak();

	if(val<4)
	{
		BTIC2F_EncodeHuffSym(ctx, ctx->hetab_dc, val);
		return;
	}
	
	eb=0; v=val;
	while(v>=4)
		{ eb++; v=v>>1; }
	if(eb>14)
		{ __debugbreak(); }
	BTIC2F_EncodeHuffSym(ctx, ctx->hetab_dc, ((eb+1)<<1)|(v&1));
	BTIC2F_WriteBits(ctx, val, eb);
}

void BTIC2F_EncodeValueDc(BTIC2F_EncState *ctx, int val)
{
	int v;
	v=(val<<1)^(val>>31);
	BTIC2F_EncodeValueDcU(ctx, v);
}

void BTIC2F_StatValueDcU(BTIC2F_EncState *ctx, int val)
{
	int eb, v;

	if(val<4)
		{ ctx->hstab_dc[val]++; return; }
	eb=0; v=val;
	while(v>=4)
		{ eb++; v=v>>1; }
	if(eb>14)
		{ __debugbreak(); }
	ctx->hstab_dc[((eb+1)<<1)|(v&1)]++;
}

void BTIC2F_StatValueDc(BTIC2F_EncState *ctx, int val)
{
	int v;
	v=(val<<1)^(val>>31);
	BTIC2F_StatValueDcU(ctx, v);
}



void BTIC2F_EncodeValueAcU(BTIC2F_EncState *ctx, int zc, int val)
{
	int eb, v;

	if(val<0)
		__debugbreak();
		
	if((zc<0) || (zc>7))
		{ __debugbreak(); }

	if(val<4)
	{
		BTIC2F_EncodeHuffSym(ctx, ctx->hetab_ac, (zc<<5)|val);
		return;
	}
	
	eb=0; v=val;
	while(v>=4)
		{ eb++; v=v>>1; }
	if(eb>14)
		{ __debugbreak(); }
	BTIC2F_EncodeHuffSym(ctx, ctx->hetab_ac, (zc<<5)|((eb+1)<<1)|(v&1));
	BTIC2F_WriteBits(ctx, val, eb);
}

void BTIC2F_EncodeValueAc(BTIC2F_EncState *ctx, int zc, int val)
{
	int v;
	v=(val<<1)^(val>>31);
	BTIC2F_EncodeValueAcU(ctx, zc, v);
}

void BTIC2F_StatValueAcU(BTIC2F_EncState *ctx, int zc, int val)
{
	int eb, v;

	if(val<0)
		{ __debugbreak(); }

	if((zc<0) || (zc>7))
		{ __debugbreak(); }

	if(val<4)
		{ ctx->hstab_ac[(zc<<5)|val]++; return; }
	eb=0; v=val;
	while(v>=4)
		{ eb++; v=v>>1; }
	if(eb>14)
		{ __debugbreak(); }
	ctx->hstab_ac[(zc<<5)|((eb+1)<<1)|(v&1)]++;
}

void BTIC2F_StatValueAc(BTIC2F_EncState *ctx, int zc, int val)
{
	int v;
	v=(val<<1)^(val>>31);
	BTIC2F_StatValueAcU(ctx, zc, v);
}


static const byte btic2f_zigzag[64]={
 0,  1,  5,  6, 14, 15, 27, 28,
 2,  4,  7, 13, 16, 26, 29, 42,
 3,  8, 12, 17, 25, 30, 41, 43,
 9, 11, 18, 24, 31, 40, 44, 53,
10, 19, 23, 32, 39, 45, 52, 54,
20, 22, 33, 38, 46, 51, 55, 60,
21, 34, 37, 47, 50, 56, 59, 61,
35, 36, 48, 49, 57, 58, 62, 63
};

int btic2f_log2u(int val)
{
	int v, e;
	v=val; e=0;
	while(v>1)
		{ v=(v+1)>>1; e++; }
	return(e);
}

int btic2f_log2d(int val)
{
	int v, e;
	v=val; e=0;
	while(v>1)
		{ v=v>>1; e++; }
	return(e);
}

void BTIC2F_EncodeQuantBlockAc(BTIC2F_EncState *ctx,
	int *iblk, int *tblk, byte *qtab)
{
	int q0, q1, q2, q3, v, qv;
	int cn, zc;
	
	if(ctx->is444)
	{
		for(cn=1; cn<64; cn++)
		{
			zc=btic2f_inv_zigzag[cn];
			
			v=iblk[zc];
			qv=qtab[zc];
			
			if(qv==1)
			{
				tblk[cn]=v;
				continue;
			}
			
			q0=v/qv;
			tblk[cn]=q0;

			if(v>=0)
				q1=(v+(qv>>1))/qv;
			else
				q1=(v-(qv>>1))/qv;
			tblk[cn]=q1;
		}

		return;
	}
	
	for(cn=1; cn<64; cn++)
	{
//		zc=btic2f_zigzag[cn];
		zc=btic2f_inv_zigzag[cn];
//		tblk[cn]=iblk[zc]/qtab[zc];
		
		v=iblk[zc];
		qv=qtab[zc];
		q0=v/qv;
		tblk[cn]=q0;

#if 1
		if(v>=0)
			q1=(v+(qv>>1))/qv;
		else
			q1=(v-(qv>>1))/qv;

		q2=(q0<<1)^(q0>>31);
		q3=(q1<<1)^(q1>>31);
//		if(btic2f_log2u(q2)==btic2f_log2u(q3))
//		if((btic2f_log2u(q2)==btic2f_log2u(q3)) || (q3<4))
		if((btic2f_log2d(q2)==btic2f_log2d(q3)) || (q3<4))
			tblk[cn]=q1;
		else
			tblk[cn]=q0;
#endif
	}

#if 1
	for(cn=2; cn<64; cn++)
	{
		if((tblk[cn-1]==0) && (abs(tblk[cn])<2))
			tblk[cn]=0;
	}
#endif

#if 1
	for(cn=32; cn<64; cn++)
		if(abs(tblk[cn])>1)
			break;
	if(cn>=64)
	{
		for(cn=32; cn<64; cn++)
			tblk[cn]=0;
	}
#endif
	
#if 1
	for(cn=16; cn<64; cn++)
		if(abs(tblk[cn])>1)
			break;
	if(cn>=64)
	{
		for(cn=16; cn<64; cn++)
			tblk[cn]=0;
	}
#endif
}

void BTIC2F_EncodeBlockAc(BTIC2F_EncState *ctx, int *iblk, byte *qtab)
{
	int tblk[64];
	int cn, zc;
	
#if 0
	for(cn=1; cn<64; cn++)
	{
//		zc=btic2f_zigzag[cn];
		zc=btic2f_inv_zigzag[cn];
		tblk[cn]=iblk[zc]/qtab[zc];
	}
#endif
	BTIC2F_EncodeQuantBlockAc(ctx, iblk, tblk, qtab);
	
	cn=1;
	while(cn<64)
	{
		for(zc=0; (cn+zc)<64; zc++)
		{
			if(tblk[cn+zc])
				break;
		}
		if((cn+zc)>=64)
		{
			BTIC2F_EncodeValueAc(ctx, 0, 0);
			break;
		}
		
		if(zc>7)zc=7;
		BTIC2F_EncodeValueAc(ctx, zc, tblk[cn+zc]);
		cn+=zc+1;
	}
}

void BTIC2F_StatBlockAc(BTIC2F_EncState *ctx, int *iblk, byte *qtab)
{
	int tblk[64];
	int cn, zc;

#if 0
	for(cn=1; cn<64; cn++)
	{
//		zc=btic2f_zigzag[cn];
		zc=btic2f_inv_zigzag[cn];
		tblk[cn]=iblk[zc]/qtab[zc];
	}
#endif

	BTIC2F_EncodeQuantBlockAc(ctx, iblk, tblk, qtab);

	
	cn=1;
	while(cn<64)
	{
		for(zc=0; (cn+zc)<64; zc++)
		{
			if(tblk[cn+zc])
				break;
		}
		if((cn+zc)>=64)
		{
			BTIC2F_StatValueAc(ctx, 0, 0);
			break;
		}
		
		if(zc>7)zc=7;
		BTIC2F_StatValueAc(ctx, zc, tblk[cn+zc]);
		cn+=zc+1;
	}
}

void BTIC2F_EncodeTransBlock420(BTIC2F_EncState *ctx, int x, int y)
{
	int *cs;
	int dcy, dvy, qy;
	int dcu, dvu, qu;
	int dcv, dvv, qv;

	dcy=ctx->dcp_y;
	dcu=ctx->dcp_u;
	dcv=ctx->dcp_v;
	qy=ctx->qtab_y[0];
	qu=ctx->qtab_uv[0];
	qv=ctx->qtab_uv[0];

	cs=ctx->blkbuf_y+((((y*2+0)*(ctx->bxs*2))+(x*2+0))*64);
	dvy=(cs[0]-dcy)/qy;
	dcy=dcy+(dvy*qy);
	BTIC2F_EncodeValueDc(ctx, dvy);
	BTIC2F_EncodeBlockAc(ctx, cs, ctx->qtab_y);

	cs=ctx->blkbuf_y+((((y*2+1)*(ctx->bxs*2))+(x*2+0))*64);
	dvy=(cs[0]-dcy)/qy;
	dcy=dcy+(dvy*qy);
	BTIC2F_EncodeValueDc(ctx, dvy);
	BTIC2F_EncodeBlockAc(ctx, cs, ctx->qtab_y);

	cs=ctx->blkbuf_y+((((y*2+1)*(ctx->bxs*2))+(x*2+1))*64);
	dvy=(cs[0]-dcy)/qy;
	dcy=dcy+(dvy*qy);
	BTIC2F_EncodeValueDc(ctx, dvy);
	BTIC2F_EncodeBlockAc(ctx, cs, ctx->qtab_y);

	cs=ctx->blkbuf_y+((((y*2+0)*(ctx->bxs*2))+(x*2+1))*64);
	dvy=(cs[0]-dcy)/qy;
	dcy=dcy+(dvy*qy);
	BTIC2F_EncodeValueDc(ctx, dvy);
	BTIC2F_EncodeBlockAc(ctx, cs, ctx->qtab_y);


	cs=ctx->blkbuf_u+(((y*ctx->bxs)+x)*64);
	dvu=(cs[0]-dcu)/qu;
	dcu=dcu+(dvu*qu);
	BTIC2F_EncodeValueDc(ctx, dvu);
	BTIC2F_EncodeBlockAc(ctx, cs, ctx->qtab_uv);

	cs=ctx->blkbuf_v+(((y*ctx->bxs)+x)*64);
	dvv=(cs[0]-dcv)/qv;
	dcv=dcv+(dvv*qv);
	BTIC2F_EncodeValueDc(ctx, dvv);
	BTIC2F_EncodeBlockAc(ctx, cs, ctx->qtab_uv);

	ctx->dcp_y=dcy;
	ctx->dcp_u=dcu;
	ctx->dcp_v=dcv;
}

void BTIC2F_StatTransBlock420(BTIC2F_EncState *ctx, int x, int y)
{
	int *cs;
	int dcy, dvy, qy;
	int dcu, dvu, qu;
	int dcv, dvv, qv;

	dcy=ctx->dcp_y;
	dcu=ctx->dcp_u;
	dcv=ctx->dcp_v;
	qy=ctx->qtab_y[0];
	qu=ctx->qtab_uv[0];
	qv=ctx->qtab_uv[0];

	cs=ctx->blkbuf_y+((((y*2+0)*(ctx->bxs*2))+(x*2+0))*64);
	dvy=(cs[0]-dcy)/qy;
	dcy=dcy+(dvy*qy);
	BTIC2F_StatValueDc(ctx, dvy);
	BTIC2F_StatBlockAc(ctx, cs, ctx->qtab_y);

	cs=ctx->blkbuf_y+((((y*2+1)*(ctx->bxs*2))+(x*2+0))*64);
	dvy=(cs[0]-dcy)/qy;
	dcy=dcy+(dvy*qy);
	BTIC2F_StatValueDc(ctx, dvy);
	BTIC2F_StatBlockAc(ctx, cs, ctx->qtab_y);

	cs=ctx->blkbuf_y+((((y*2+1)*(ctx->bxs*2))+(x*2+1))*64);
	dvy=(cs[0]-dcy)/qy;
	dcy=dcy+(dvy*qy);
	BTIC2F_StatValueDc(ctx, dvy);
	BTIC2F_StatBlockAc(ctx, cs, ctx->qtab_y);

	cs=ctx->blkbuf_y+((((y*2+0)*(ctx->bxs*2))+(x*2+1))*64);
	dvy=(cs[0]-dcy)/qy;
	dcy=dcy+(dvy*qy);
	BTIC2F_StatValueDc(ctx, dvy);
	BTIC2F_StatBlockAc(ctx, cs, ctx->qtab_y);


	cs=ctx->blkbuf_u+(((y*ctx->bxs)+x)*64);
	dvu=(cs[0]-dcu)/qu;
	dcu=dcu+(dvu*qu);
	BTIC2F_StatValueDc(ctx, dvu);
	BTIC2F_StatBlockAc(ctx, cs, ctx->qtab_uv);

	cs=ctx->blkbuf_v+(((y*ctx->bxs)+x)*64);
	dvv=(cs[0]-dcv)/qv;
	dcv=dcv+(dvv*qv);
	BTIC2F_StatValueDc(ctx, dvv);
	BTIC2F_StatBlockAc(ctx, cs, ctx->qtab_uv);

	ctx->dcp_y=dcy;
	ctx->dcp_u=dcu;
	ctx->dcp_v=dcv;
}

#if 1
void BTIC2F_EncodeTransBlock444(BTIC2F_EncState *ctx, int x, int y)
{
	int *cs;
	int dcy, dvy, qy;
	int dcu, dvu, qu;
	int dcv, dvv, qv;

	dcy=ctx->dcp_y;
	dcu=ctx->dcp_u;
	dcv=ctx->dcp_v;
	qy=ctx->qtab_y[0];
	qu=ctx->qtab_uv[0];
	qv=ctx->qtab_uv[0];

	cs=ctx->blkbuf_y+(((y*ctx->bxs)+x)*64);
	dvy=(cs[0]-dcy)/qy;
	dcy=dcy+(dvy*qy);
	BTIC2F_EncodeValueDc(ctx, dvy);
	BTIC2F_EncodeBlockAc(ctx, cs, ctx->qtab_y);

	cs=ctx->blkbuf_u+(((y*ctx->bxs)+x)*64);
	dvu=(cs[0]-dcu)/qu;
	dcu=dcu+(dvu*qu);
	BTIC2F_EncodeValueDc(ctx, dvu);
	BTIC2F_EncodeBlockAc(ctx, cs, ctx->qtab_uv);

	cs=ctx->blkbuf_v+(((y*ctx->bxs)+x)*64);
	dvv=(cs[0]-dcv)/qv;
	dcv=dcv+(dvv*qv);
	BTIC2F_EncodeValueDc(ctx, dvv);
	BTIC2F_EncodeBlockAc(ctx, cs, ctx->qtab_uv);

	ctx->dcp_y=dcy;
	ctx->dcp_u=dcu;
	ctx->dcp_v=dcv;
}

void BTIC2F_StatTransBlock444(BTIC2F_EncState *ctx, int x, int y)
{
	int *cs;
	int dcy, dvy, qy;
	int dcu, dvu, qu;
	int dcv, dvv, qv;

	dcy=ctx->dcp_y;
	dcu=ctx->dcp_u;
	dcv=ctx->dcp_v;
	qy=ctx->qtab_y[0];
	qu=ctx->qtab_uv[0];
	qv=ctx->qtab_uv[0];

	cs=ctx->blkbuf_y+(((y*ctx->bxs)+x)*64);
	dvy=(cs[0]-dcy)/qy;
	dcy=dcy+(dvy*qy);
	BTIC2F_StatValueDc(ctx, dvy);
	BTIC2F_StatBlockAc(ctx, cs, ctx->qtab_y);

	cs=ctx->blkbuf_u+(((y*ctx->bxs)+x)*64);
	dvu=(cs[0]-dcu)/qu;
	dcu=dcu+(dvu*qu);
	BTIC2F_StatValueDc(ctx, dvu);
	BTIC2F_StatBlockAc(ctx, cs, ctx->qtab_uv);

	cs=ctx->blkbuf_v+(((y*ctx->bxs)+x)*64);
	dvv=(cs[0]-dcv)/qv;
	dcv=dcv+(dvv*qv);
	BTIC2F_StatValueDc(ctx, dvv);
	BTIC2F_StatBlockAc(ctx, cs, ctx->qtab_uv);

	ctx->dcp_y=dcy;
	ctx->dcp_u=dcu;
	ctx->dcp_v=dcv;
}
#endif
void BTIC2F_TransBHT_8H(s16 *iblk, int *oblk)
{
	int ib0, ib1, ib2, ib3, ib4, ib5, ib6, ib7;
	int a0, a1, a2, a3, a4, a5, a6, a7;
	int b0, b1, b2, b3, b4, b5, b6, b7;
	int c0, c1, c2, c3, c4, c5, c6, c7;

	ib0=iblk[0];	ib1=iblk[1];	ib2=iblk[2];	ib3=iblk[3];
	ib4=iblk[4];	ib5=iblk[5];	ib6=iblk[6];	ib7=iblk[7];

	a0=ib0+ib1;			a1=ib2+ib3;
	a2=ib4+ib5;			a3=ib6+ib7;
	a4=(a0>>1)-ib1;		a5=(a1>>1)-ib3;
	a6=(a2>>1)-ib5;		a7=(a3>>1)-ib7;
	b0=a0+a1;			b1=a2+a3;
	b2=(b0>>1)-a1;		b3=(b1>>1)-a3;
	c0=b0+b1;			c1=(c0>>1)-b1;

	oblk[0]=c0;		oblk[1]=c1;	oblk[2]=b2;		oblk[3]=b3;
	oblk[4]=a4;		oblk[5]=a5;	oblk[6]=a6;		oblk[7]=a7;
}

void BTIC2F_TransBHT_8V(int *iblk, int *oblk,
	int istride, int ostride)
{
	int ib0, ib1, ib2, ib3, ib4, ib5, ib6, ib7;
	int a0, a1, a2, a3, a4, a5, a6, a7;
	int b0, b1, b2, b3, b4, b5, b6, b7;
	int c0, c1, c2, c3, c4, c5, c6, c7;

	ib0=iblk[8*0];		ib1=iblk[8*1];
	ib2=iblk[8*2];		ib3=iblk[8*3];
	ib4=iblk[8*4];		ib5=iblk[8*5];
	ib6=iblk[8*6];		ib7=iblk[8*7];
	a0=ib0+ib1;			a1=ib2+ib3;
	a2=ib4+ib5;			a3=ib6+ib7;
	a4=(a0>>1)-ib1;		a5=(a1>>1)-ib3;
	a6=(a2>>1)-ib5;		a7=(a3>>1)-ib7;
	b0=a0+a1;			b1=a2+a3;
	b2=(b0>>1)-a1;		b3=(b1>>1)-a3;
	c0=b0+b1;			c1=(c0>>1)-b1;
	oblk[8*0]=c0;		oblk[8*1]=c1;
	oblk[8*2]=b2;		oblk[8*3]=b3;
	oblk[8*4]=a4;		oblk[8*5]=a5;
	oblk[8*6]=a6;		oblk[8*7]=a7;
}

void BTIC2F_TransBHT8(s16 *iblk, int *oblk, int ystride)
{
	int s[64];

	BTIC2F_TransBHT_8H(iblk+ystride*0, s+8*0);
	BTIC2F_TransBHT_8H(iblk+ystride*1, s+8*1);
	BTIC2F_TransBHT_8H(iblk+ystride*2, s+8*2);
	BTIC2F_TransBHT_8H(iblk+ystride*3, s+8*3);
	BTIC2F_TransBHT_8H(iblk+ystride*4, s+8*4);
	BTIC2F_TransBHT_8H(iblk+ystride*5, s+8*5);
	BTIC2F_TransBHT_8H(iblk+ystride*6, s+8*6);
	BTIC2F_TransBHT_8H(iblk+ystride*7, s+8*7);

	BTIC2F_TransBHT_8V(s+0, oblk+0, 8, 8);
	BTIC2F_TransBHT_8V(s+1, oblk+1, 8, 8);
	BTIC2F_TransBHT_8V(s+2, oblk+2, 8, 8);
	BTIC2F_TransBHT_8V(s+3, oblk+3, 8, 8);
	BTIC2F_TransBHT_8V(s+4, oblk+4, 8, 8);
	BTIC2F_TransBHT_8V(s+5, oblk+5, 8, 8);
	BTIC2F_TransBHT_8V(s+6, oblk+6, 8, 8);
	BTIC2F_TransBHT_8V(s+7, oblk+7, 8, 8);
}

void BTIC2F_TransBHT8_SimQuantBlock(
	BTIC2F_EncState *ctx, int *s, int *s1, byte *qtab, int dcp)
{
	int t[64];
	int i, j, k;

//	s1[0]=(s[0]/qtab[0])*qtab[0];
	s1[0]=dcp+((s[0]-dcp)/qtab[0])*qtab[0];
//	t[0]=s[0];
	BTIC2F_EncodeQuantBlockAc(ctx, s, t, qtab);
	for(i=1; i<64; i++)
	{
		j=btic2f_inv_zigzag[i];
		s1[j]=t[i]*qtab[j];
	}
}

void BTIC2F_TransBHT8_QClamp(
	BTIC2F_EncState *ctx,
	s16 *iblk, int *oblk, int ystride,
	byte *qtab, int min, int max)
{
	s16 pix[64];
	int s[64];
	int t[64];
	int dc, n;
	int i, j, k;
	
	BTIC2F_TransBHT8(iblk, s, ystride);

#if 1
	n=8;
	while(n--)
	{
//		for(i=0; i<64; i++)
//			{ t[i]=(s[i]/qtab[i])*qtab[i]; }

		t[0]=(s[0]/qtab[0])*qtab[0];
//		t[0]=s[0];
		BTIC2F_EncodeQuantBlockAc(ctx, s, t, qtab);
		for(i=1; i<64; i++)
		{
			j=btic2f_inv_zigzag[i];
			s[j]=t[i]*qtab[j];
		}
		
		BTIC2F_TransIBHT8Stride(s, pix, 8);
		for(i=0; i<64; i++)
			if((pix[i]<min) || (pix[i]>max))
				break;
		if(i>=64)
			break;

		dc=s[0]/64;

		for(i=0; i<64; i++)
		{
			j=pix[i];
			if(j<min)	j+=2*(min-j);
			if(j>max)	j+=2*(max-j);

//			if(j<min)	j=dc;
//			if(j>max)	j=dc;

			pix[i]=j;
//			if((pix<min) || (pix>max))
//				break;
		}

		BTIC2F_TransBHT8(pix, s, 8);
	}
#endif

	memcpy(oblk, s, 64*sizeof(int));
}

int btic2f_clamp(int v, int m, int n)
{
	if(v<m)return(m);
	if(v>n)return(n);
	return(v);
}

void BTIC2F_EncodeTransImageBlockYuv420(
	s16 *pxy, s16 *pxu, s16 *pxv,
	s16 *iptr, int xstride, int ystride,
	int yeps)
{
	s16 *csy, *csu, *csv;
	s16 *ct0, *ct1;
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int cr3, cg3, cb3;
	int cy0, cy1, cy2, cy3;
	int cu0, cu1, cu2, cu3;
	int cv0, cv1, cv2, cv3;
	int cu, cv;
	int cr, cg, cb;
	int x, y;
	
	csy=pxy;	csu=pxu;	csv=pxv;
	for(y=0; y<8; y++)
	{
		csy=pxy+y*2*16;
		csu=pxu+y*8;
		csv=pxv+y*8;
		ct0=iptr+(y*2)*ystride;
		ct1=ct0+ystride;

		for(x=0; x<8; x++)
		{
			cr0=ct0[0];		cg0=ct0[1];		cb0=ct0[2];
			cr2=ct1[0];		cg2=ct1[1];		cb2=ct1[2];
			ct0+=xstride;	ct1+=xstride;
			cr1=ct0[0];		cg1=ct0[1];		cb1=ct0[2];
			cr3=ct1[0];		cg3=ct1[1];		cb3=ct1[2];
			ct0+=xstride;	ct1+=xstride;

			cr=(cr0+cr1+cr2+cr3)/4;
			cg=(cg0+cg1+cg2+cg3)/4;
			cb=(cb0+cb1+cb2+cb3)/4;
//			cu=cb-cg;
//			cv=cr-cg;

			cu0=cb-cg0;		cu1=cb-cg1;
			cu2=cb-cg2;		cu3=cb-cg3;
			cv0=cr-cg0;		cv1=cr-cg1;
			cv2=cr-cg2;		cv3=cr-cg3;
			cu=(cu0+cu1+cu2+cu3)/4;
			cv=(cv0+cv1+cv2+cv3)/4;

			cy0=cg0;	cy1=cg1;
			cy2=cg2;	cy3=cg3;

#if 1
//			cy0=btic2f_clamp(cy0, 16, 240);
//			cy1=btic2f_clamp(cy1, 16, 240);
//			cy2=btic2f_clamp(cy2, 16, 240);
//			cy3=btic2f_clamp(cy3, 16, 240);
			
			cy0=btic2f_clamp(cy0, yeps, 255-yeps);
			cy1=btic2f_clamp(cy1, yeps, 255-yeps);
			cy2=btic2f_clamp(cy2, yeps, 255-yeps);
			cy3=btic2f_clamp(cy3, yeps, 255-yeps);

//			cu=btic2f_clamp(cu, -224, 224);
//			cv=btic2f_clamp(cv, -224, 224);

			cu=btic2f_clamp(cu, -(255-yeps), 255-yeps);
			cv=btic2f_clamp(cv, -(255-yeps), 255-yeps);

//			cr=(cy0+cv)|(cy1+cv)|(cy2+cv)|(cy3+cv);
//			cb=(cy0+cu)|(cy1+cu)|(cy2+cu)|(cy3+cu);
//			if(cr>>8)cv=0;
//			if(cb>>8)cu=0;
#endif

			csy[0]=cy0;		csy[1]=cy1;
			csy[16]=cy2;	csy[17]=cy3;
			csu[0]=cu;		csv[0]=cv;
			csu++;			csv++;
			csy+=2;
		}
	}
}

void BTIC2F_EncodeDecTransBlockYuv420(
	s16 *pxy, s16 *pxu, s16 *pxv,
	s16 *iptr, int xstride, int ystride)
{
	s16 *csy, *csu, *csv;
	s16 *ct0, *ct1, *ct0e;
	int cy0, cy1, cy2, cy3, cu, cv;
	int cr, cg, cb;
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int cr3, cg3, cb3;
	int x, y;
	
//	csy=pxy;	csu=pxu;	csv=pxv;

	for(y=0; y<8; y++)
	{
		csy=pxy+y*2*16;
		csu=pxu+y*8;
		csv=pxv+y*8;

		ct0=iptr+(y*2)*ystride;
		ct1=ct0+ystride;
		ct0e=ct0+(xstride<<4);
		
		while(ct0<ct0e)
		{
			cy0=csy[0];		cy1=csy[1];
			cy2=csy[16];	cy3=csy[17];
			cu=*csu++;		cv=*csv++;		csy+=2;

			cr0=cy0+cv;	cg0=cy0;	cb0=cy0+cu;
			cr1=cy2+cv;	cg1=cy2;	cb1=cy2+cu;
			ct0[0]=cr0;	ct0[1]=cg0;	ct0[2]=cb0;
			ct1[0]=cr1;	ct1[1]=cg1;	ct1[2]=cb1;
			ct0+=xstride;
			ct1+=xstride;
			cr0=cy1+cv;	cg0=cy1;	cb0=cy1+cu;
			cr1=cy3+cv;	cg1=cy3;	cb1=cy3+cu;
			ct0[0]=cr0;	ct0[1]=cg0;	ct0[2]=cb0;
			ct1[0]=cr1;	ct1[1]=cg1;	ct1[2]=cb1;
			ct0+=xstride;
			ct1+=xstride;
		}
	}
}

void BTIC2F_EncodeTransImageBlock420(
	BTIC2F_EncState *ctx, int x, int y,
	byte *iptr, int xstride, int ystride,
	int xclip, int yclip)
{
	s16 pix[16*16*4];
	s16 pix1[16*16*4];
	int b_y0[64], b_y1[64], b_y2[64], b_y3[64];
	int b_u0[64], b_v0[64];

	int bq_y0[64], bq_y1[64], bq_y2[64], bq_y3[64];
	int bq_u0[64], bq_v0[64];

	int *ct;
	s16 pix_y[16*16];
	s16 pix_u[8*8];
	s16 pix_v[8*8];
	
	int dcpy, dcpu, dcpv;
	int cr, cg, cb;
	int cr1, cg1, cb1;
	int px, py, n, yeps, ymin, ymax;
	int i, j, k;

	for(py=0; py<yclip; py++)
	{
//		memcpy(pix+py*(16*4), iptr+py*ystride, xclip*xstride);
		
		for(px=0; px<xclip; px++)
		{
			pix[(py*16+px)*4+0]=iptr[py*ystride+px*xstride+0];
			pix[(py*16+px)*4+1]=iptr[py*ystride+px*xstride+1];
			pix[(py*16+px)*4+2]=iptr[py*ystride+px*xstride+2];
		}
	}
	
	yeps=ctx->yeps;
	ymin=yeps;
	ymax=255-yeps;
	BTIC2F_EncodeTransImageBlockYuv420(pix_y, pix_u, pix_v,
		pix, 4, 16*4, yeps);

#if 1
	BTIC2F_TransBHT8(pix_y+(0*16+0), b_y0, 16);
	BTIC2F_TransBHT8(pix_y+(0*16+8), b_y1, 16);
	BTIC2F_TransBHT8(pix_y+(8*16+0), b_y2, 16);
	BTIC2F_TransBHT8(pix_y+(8*16+8), b_y3, 16);
	BTIC2F_TransBHT8(pix_u, b_u0, 8);
	BTIC2F_TransBHT8(pix_v, b_v0, 8);
#endif

#if 0
	BTIC2F_TransBHT8_QClamp(ctx, pix_y+(0*16+0),
		b_y0, 16, ctx->qtab_y, 16, 240);
	BTIC2F_TransBHT8_QClamp(ctx, pix_y+(0*16+8),
		b_y1, 16, ctx->qtab_y, 16, 240);
	BTIC2F_TransBHT8_QClamp(ctx, pix_y+(8*16+0),
		b_y2, 16, ctx->qtab_y, 16, 240);
	BTIC2F_TransBHT8_QClamp(ctx, pix_y+(8*16+8),
		b_y3, 16, ctx->qtab_y, 16, 240);
	BTIC2F_TransBHT8_QClamp(ctx, pix_u,
		b_u0, 8, ctx->qtab_uv, -240, 240);
	BTIC2F_TransBHT8_QClamp(ctx, pix_v,
		b_v0, 8, ctx->qtab_uv, -240, 240);
#endif

	dcpy=ctx->dcp_y;
	dcpu=ctx->dcp_u;
	dcpv=ctx->dcp_v;

#if 1
	n=8;
	while(n--)
	{
		BTIC2F_TransBHT8_SimQuantBlock(ctx, b_y0, bq_y0, ctx->qtab_y, dcpy);
		BTIC2F_TransBHT8_SimQuantBlock(ctx, b_y2, bq_y2, ctx->qtab_y, bq_y0[0]);
		BTIC2F_TransBHT8_SimQuantBlock(ctx, b_y3, bq_y3, ctx->qtab_y, bq_y2[0]);
		BTIC2F_TransBHT8_SimQuantBlock(ctx, b_y1, bq_y1, ctx->qtab_y, bq_y3[0]);

		BTIC2F_TransBHT8_SimQuantBlock(ctx, b_u0, bq_u0, ctx->qtab_uv, dcpu);
		BTIC2F_TransBHT8_SimQuantBlock(ctx, b_v0, bq_v0, ctx->qtab_uv, dcpv);
	
		BTIC2F_TransIBHT8Stride(bq_y0, pix_y+(0*16+0), 16);
		BTIC2F_TransIBHT8Stride(bq_y1, pix_y+(0*16+8), 16);
		BTIC2F_TransIBHT8Stride(bq_y2, pix_y+(8*16+0), 16);
		BTIC2F_TransIBHT8Stride(bq_y3, pix_y+(8*16+8), 16);
		BTIC2F_TransIBHT8Stride(bq_u0, pix_u, 8);
		BTIC2F_TransIBHT8Stride(bq_v0, pix_v, 8);
		
		BTIC2F_EncodeDecTransBlockYuv420(pix_y, pix_u, pix_v,
			pix1, 4, 16*4);
			
		for(i=0; i<(16*16); i++)
		{
			if((pix1[i*4+0]<ymin) || (pix1[i*4+0]>ymax))
				break;
			if((pix1[i*4+1]<ymin) || (pix1[i*4+1]>ymax))
				break;
			if((pix1[i*4+2]<ymin) || (pix1[i*4+2]>ymax))
				break;
		}
		
		if(i>=(16*16))
			break;

		for(i=0; i<(16*16); i++)
		{
			cr=pix1[i*4+0];
			cg=pix1[i*4+1];
			cb=pix1[i*4+2];

			cr1=pix[i*4+0];
			cg1=pix[i*4+1];
			cb1=pix[i*4+2];

			cr1=cr;	cg1=cg;	cb1=cb;

			if(cr<ymin)	cr1=cr+1.414*(ymin-cr);
			if(cg<ymin)	cg1=cg+1.414*(ymin-cg);
			if(cb<ymin)	cb1=cb+1.414*(ymin-cb);
			if(cr>ymax)	cr1=cr+1.414*(ymax-cr);
			if(cg>ymax)	cg1=cg+1.414*(ymax-cg);
			if(cb>ymax)	cb1=cb+1.414*(ymax-cb);

//			k=(cr1+2*cg1+cb1)/4;
//			cr1=k;	cg1=k;	cb1=k;

//			if(cr1<16)	cr1=128;
//			if(cg1<16)	cg1=128;
//			if(cb1<16)	cb1=128;
//			if(cr1>240)	cr1=128;
//			if(cg1>240)	cg1=128;
//			if(cb1>240)	cb1=128;
			
			pix1[i*4+0]=cr1;
			pix1[i*4+1]=cg1;
			pix1[i*4+2]=cb1;
		}

		BTIC2F_EncodeTransImageBlockYuv420(pix_y, pix_u, pix_v,
			pix1, 4, 16*4, yeps);
		BTIC2F_TransBHT8(pix_y+(0*16+0), b_y0, 16);
		BTIC2F_TransBHT8(pix_y+(0*16+8), b_y1, 16);
		BTIC2F_TransBHT8(pix_y+(8*16+0), b_y2, 16);
		BTIC2F_TransBHT8(pix_y+(8*16+8), b_y3, 16);
		BTIC2F_TransBHT8(pix_u, b_u0, 8);
		BTIC2F_TransBHT8(pix_v, b_v0, 8);
	}
#endif

	ctx->dcp_y=bq_y3[0];
	ctx->dcp_u=bq_u0[0];
	ctx->dcp_v=bq_v0[0];

	ct=ctx->blkbuf_y+(((y*2+0)*(ctx->bxs*2))+(x*2+0))*64;
	memcpy(ct, b_y0, 64*sizeof(int));
	ct=ctx->blkbuf_y+(((y*2+0)*(ctx->bxs*2))+(x*2+1))*64;
	memcpy(ct, b_y1, 64*sizeof(int));
	ct=ctx->blkbuf_y+(((y*2+1)*(ctx->bxs*2))+(x*2+0))*64;
	memcpy(ct, b_y2, 64*sizeof(int));
	ct=ctx->blkbuf_y+(((y*2+1)*(ctx->bxs*2))+(x*2+1))*64;
	memcpy(ct, b_y3, 64*sizeof(int));
	
	ct=ctx->blkbuf_u+((y*ctx->bxs)+x)*64;
	memcpy(ct, b_u0, 64*sizeof(int));
	ct=ctx->blkbuf_v+((y*ctx->bxs)+x)*64;
	memcpy(ct, b_v0, 64*sizeof(int));

//	BTIC2F_DecodeTransBlock420(ctx, x, y, pix, xstride, 16*4);
	
}

#if 1
void BTIC2F_EncodeTransImageBlockYuv444(
	s16 *pxy, s16 *pxu, s16 *pxv,
	s16 *iptr, int xstride, int ystride,
	int yeps)
{
	s16 *csy, *csu, *csv;
	s16 *ct0, *ct1;
	int cy, cu, cv;
	int cr, cg, cb;
	int x, y;
	
	csy=pxy;	csu=pxu;	csv=pxv;
	for(y=0; y<8; y++)
	{
		csy=pxy+y*8;
		csu=pxu+y*8;
		csv=pxv+y*8;
		ct0=iptr+y*ystride;

		for(x=0; x<8; x++)
		{
			cr=ct0[0];		cg=ct0[1];		cb=ct0[2];
			ct0+=xstride;
			cu=cb-cg;		cv=cr-cg;		cy=cg;
			cy=btic2f_clamp(cy, yeps, 255-yeps);
			cu=btic2f_clamp(cu, -(256-yeps), 256-yeps);
			cv=btic2f_clamp(cv, -(256-yeps), 256-yeps);
			csy[0]=cy;		csu[0]=cu;		csv[0]=cv;
			csu++;			csv++;			csy++;
		}
	}
}

void BTIC2F_EncodeDecTransBlockYuv444(
	s16 *pxy, s16 *pxu, s16 *pxv,
	s16 *iptr, int xstride, int ystride)
{
	s16 *csy, *csu, *csv;
	s16 *ct0, *ct1, *ct0e;
	int cy, cu, cv;
	int cr, cg, cb;
	int x, y;
	
	for(y=0; y<8; y++)
	{
		csy=pxy+y*8;
		csu=pxu+y*8;
		csv=pxv+y*8;
		ct0=iptr+y*ystride;
		ct0e=ct0+(xstride<<3);
		while(ct0<ct0e)
		{
			cy=*csy++;	cu=*csu++;	cv=*csv++;
			cr=cy+cv;	cg=cy;	cb=cy+cu;
			ct0[0]=cr;	ct0[1]=cg;	ct0[2]=cb;
			ct0+=xstride;
		}
	}
}

void BTIC2F_EncodeTransImageBlock444(
	BTIC2F_EncState *ctx, int x, int y,
	byte *iptr, int xstride, int ystride,
	int xclip, int yclip)
{
	s16 pix[8*8*4];
	s16 pix1[8*8*4];
	int b_y0[64], b_u0[64], b_v0[64];
	int bq_y0[64], bq_u0[64], bq_v0[64];

	int *ct;
	s16 pix_y[8*8];
	s16 pix_u[8*8];
	s16 pix_v[8*8];
	s16 pix_y2[8*8];
	s16 pix_u2[8*8];
	s16 pix_v2[8*8];
	
	int dcpy, dcpu, dcpv;
	int cr, cg, cb;
	int cr1, cg1, cb1;
	int px, py, n, yeps, ymin, ymax;
	int i, j, k;

	memset(pix, 0, 8*8*4*2);
	memset(pix1, 0, 8*8*4*2);

	for(py=0; py<yclip; py++)
	{
//		memcpy(pix+py*(16*4), iptr+py*ystride, xclip*xstride);
		
		for(px=0; px<xclip; px++)
		{
			pix[(py*8+px)*4+0]=iptr[py*ystride+px*xstride+0];
			pix[(py*8+px)*4+1]=iptr[py*ystride+px*xstride+1];
			pix[(py*8+px)*4+2]=iptr[py*ystride+px*xstride+2];
		}
	}
	
	yeps=ctx->yeps;
	ymin=yeps;
	ymax=255-yeps;
	BTIC2F_EncodeTransImageBlockYuv444(pix_y, pix_u, pix_v,
		pix, 4, 8*4, yeps);

#if 1
	BTIC2F_TransBHT8(pix_y, b_y0, 8);
	BTIC2F_TransBHT8(pix_u, b_u0, 8);
	BTIC2F_TransBHT8(pix_v, b_v0, 8);
#endif

#if 1
	BTIC2F_TransIBHT8Stride(b_y0, pix_y2, 8);
	BTIC2F_TransIBHT8Stride(b_u0, pix_u2, 8);
	BTIC2F_TransIBHT8Stride(b_v0, pix_v2, 8);
	
	if(memcmp(pix_y, pix_y2, 64*2))
		{ printf("BHT: Y Mismatch\n"); }
	if(memcmp(pix_u, pix_u2, 64*2))
		{ printf("BHT: U Mismatch\n"); }
	if(memcmp(pix_v, pix_v2, 64*2))
		{ printf("BHT: V Mismatch\n"); }

	BTIC2F_EncodeDecTransBlockYuv444(pix_y, pix_u, pix_v,
		pix1, 4, 8*4);
		
	if(memcmp(pix, pix1, 8*8*4*2))
	{
		printf("BHT: Pixel Trans Mismatch\n");
	}
#endif

#if 1
	dcpy=ctx->dcp_y;
	dcpu=ctx->dcp_u;
	dcpv=ctx->dcp_v;

	n=8;
	while(n--)
	{
		BTIC2F_TransBHT8_SimQuantBlock(ctx, b_y0, bq_y0, ctx->qtab_y, dcpy);
		BTIC2F_TransBHT8_SimQuantBlock(ctx, b_u0, bq_u0, ctx->qtab_uv, dcpu);
		BTIC2F_TransBHT8_SimQuantBlock(ctx, b_v0, bq_v0, ctx->qtab_uv, dcpv);
		BTIC2F_TransIBHT8Stride(bq_y0, pix_y, 8);
		BTIC2F_TransIBHT8Stride(bq_u0, pix_u, 8);
		BTIC2F_TransIBHT8Stride(bq_v0, pix_v, 8);
		BTIC2F_EncodeDecTransBlockYuv444(pix_y, pix_u, pix_v,
			pix1, 4, 8*4);
			
		for(i=0; i<(8*8); i++)
		{
			if((pix1[i*4+0]<ymin) || (pix1[i*4+0]>ymax))
				break;
			if((pix1[i*4+1]<ymin) || (pix1[i*4+1]>ymax))
				break;
			if((pix1[i*4+2]<ymin) || (pix1[i*4+2]>ymax))
				break;
		}
		
		if(i>=(8*8))
			break;

		for(i=0; i<(8*8); i++)
		{
			cr=pix1[i*4+0];
			cg=pix1[i*4+1];
			cb=pix1[i*4+2];

			cr1=pix[i*4+0];
			cg1=pix[i*4+1];
			cb1=pix[i*4+2];

			cr1=cr;	cg1=cg;	cb1=cb;

			if(cr<ymin)	cr1=cr+1.414*(ymin-cr);
			if(cg<ymin)	cg1=cg+1.414*(ymin-cg);
			if(cb<ymin)	cb1=cb+1.414*(ymin-cb);
			if(cr>ymax)	cr1=cr+1.414*(ymax-cr);
			if(cg>ymax)	cg1=cg+1.414*(ymax-cg);
			if(cb>ymax)	cb1=cb+1.414*(ymax-cb);
			
			pix1[i*4+0]=cr1;
			pix1[i*4+1]=cg1;
			pix1[i*4+2]=cb1;
		}

		BTIC2F_EncodeTransImageBlockYuv444(pix_y, pix_u, pix_v,
			pix1, 4, 8*4, yeps);
		BTIC2F_TransBHT8(pix_y, b_y0, 8);
		BTIC2F_TransBHT8(pix_u, b_u0, 8);
		BTIC2F_TransBHT8(pix_v, b_v0, 8);
	}

	ctx->dcp_y=bq_y0[0];
	ctx->dcp_u=bq_u0[0];
	ctx->dcp_v=bq_v0[0];
#endif

	ct=ctx->blkbuf_y+((y*ctx->bxs)+x)*64;
	memcpy(ct, b_y0, 64*sizeof(int));	
	ct=ctx->blkbuf_u+((y*ctx->bxs)+x)*64;
	memcpy(ct, b_u0, 64*sizeof(int));
	ct=ctx->blkbuf_v+((y*ctx->bxs)+x)*64;
	memcpy(ct, b_v0, 64*sizeof(int));	
}

#endif

static const int pdjpg_ijg_qtab_y[64] = {
16, 11, 10, 16,  24,  40,  51,  61,
12, 12, 14, 19,  26,  58,  60,  55,
14, 13, 16, 24,  40,  57,  69,  56,
14, 17, 22, 29,  51,  87,  80,  62,
18, 22, 37, 56,  68, 109, 103,  77,
24, 35, 55, 64,  81, 104, 113,  92,
49, 64, 78, 87, 103, 121, 120, 101,
72, 92, 95, 98, 112, 100, 103,  99};

static const int pdjpg_ijg_qtab_uv[64] = {
99, 99, 99, 99, 99, 99, 99, 99,
99, 99, 99, 99, 99, 99, 99, 99,
99, 99, 99, 99, 99, 99, 99, 99,
99, 99, 99, 99, 99, 99, 99, 99,
47, 66, 99, 99, 99, 99, 99, 99,
24, 26, 56, 99, 99, 99, 99, 99,
18, 21, 26, 66, 99, 99, 99, 99,
17, 18, 24, 47, 99, 99, 99, 99};

void BTIC2F_MakeQuantTabFastIJG_Y(byte *tab, float qf)
{
	double s, q;
	int i, j;

	q=(qf*100);
	s=(q<50)?5000/q:(200-2*q);
	for(i=0; i<64; i++)
	{
		j=(int)((s * pdjpg_ijg_qtab_y[i]+50)/100);
		j=(j<1)?1:((j<256)?j:255);
		tab[i]=j;
	}
}

void BTIC2F_MakeQuantTabFastIJG_UV(byte *tab, float qf)
{
	double s, q;
	int i, j;

	q=(qf*100);
	s=(q<50)?5000/q:(200-2*q);
	for(i=0; i<64; i++)
	{
		j=(int)((s*pdjpg_ijg_qtab_uv[i]+50)/100);
		j=(j<1)?1:((j<256)?j:255);
		tab[i]=j;
	}
}

byte *BTIC2F_EncodeEmitTwoCC(byte *ict, int tcc, void *buf, int sz)
{
	byte *ct;
	int sz1;
	
	ct=ict;
	
	if((sz+4)<8192)
	{
		sz1=sz+4;
		*ct++=0x00|(sz1>>8);
		*ct++=sz1;
		*ct++=tcc;
		*ct++=tcc>>8;
		if(buf)
			{ memmove(ct, buf, sz); }
		else
			{ memset(ct, 0, sz); }
		ct+=sz;
		return(ct);
	}

	if((sz+5)<2097152)
	{
		sz1=sz+5;
		*ct++=0x80|(sz1>>16);
		*ct++=(sz1>>8);
		*ct++=sz1;
		*ct++=tcc;
		*ct++=tcc>>8;
		if(buf)
			{ memmove(ct, buf, sz); }
		else
			{ memset(ct, 0, sz); }
		ct+=sz;
		return(ct);
	}

	if((sz+6)<536870912)
	{
		sz1=sz+5;
		*ct++=0x40|(sz1>>24);
		*ct++=(sz1>>16);
		*ct++=(sz1>>8);
		*ct++=sz1;
		*ct++=tcc;
		*ct++=tcc>>8;
		if(buf)
			{ memmove(ct, buf, sz); }
		else
			{ memset(ct, 0, sz); }
		ct+=sz;
		return(ct);
	}
	
	return(NULL);
}

void BTIC2F_EncodeInitContext(BTIC2F_EncState *ctx,
	int xs, int ys, int qfl)
{
	ctx->xs=xs;
	ctx->ys=ys;
}

int BTIC2F_EncodeTransImage(BTIC2F_EncState *ctx,
	byte *obuf, int obsz,
	byte *img, int xstride, int ystride, int qfl)
{
	byte tbuf[4096];
	byte *ct, *ctt, *ct1;
	int xs, ys, bxs0, bys0, bxs1, bys1;
	int is444, bsz;
	int x, y;
	int sz;
	int i, j, k;

	xs=ctx->xs;
	ys=ctx->ys;
	
	is444=0;

	if((qfl&127)>=95)
	{
		is444=1;
	}
	
	if(is444)
	{
		bxs0=(xs+0)>>3;
		bys0=(ys+0)>>3;
		bxs1=(xs+7)>>3;
		bys1=(ys+7)>>3;
		bsz=8;
		ctx->EncodeTransImageBlock=BTIC2F_EncodeTransImageBlock444;
		ctx->StatTransBlock=BTIC2F_StatTransBlock444;
		ctx->EncodeTransBlock=BTIC2F_EncodeTransBlock444;
	}else
	{
		bxs0=(xs+0)>>4;
		bys0=(ys+0)>>4;
		bxs1=(xs+15)>>4;
		bys1=(ys+15)>>4;
		bsz=16;
		ctx->EncodeTransImageBlock=BTIC2F_EncodeTransImageBlock420;
		ctx->StatTransBlock=BTIC2F_StatTransBlock420;
		ctx->EncodeTransBlock=BTIC2F_EncodeTransBlock420;
	}
	
	ctx->bxs=bxs1;
	ctx->bys=bys1;
	ctx->is444=is444;
	
	if(!ctx->blkbuf_y)
	{
		ctx->blkbuf_y=malloc((bxs1*2)*(bys1*2)*64*sizeof(int));
		ctx->blkbuf_u=malloc((bxs1*1)*(bys1*1)*64*sizeof(int));
		ctx->blkbuf_v=malloc((bxs1*1)*(bys1*1)*64*sizeof(int));
		
		memset(ctx->blkbuf_y, 0, (bxs1*2)*(bys1*2)*64*sizeof(int));
		memset(ctx->blkbuf_u, 0, (bxs1*1)*(bys1*1)*64*sizeof(int));
		memset(ctx->blkbuf_v, 0, (bxs1*1)*(bys1*1)*64*sizeof(int));
	}
	
	if((qfl&127)>=100)
	{
		for(i=0; i<64; i++)
		{
			ctx->qtab_y[i]=1;
			ctx->qtab_uv[i]=1;
		}
	}else
	{
		BTIC2F_MakeQuantTabFastIJG_Y(ctx->qtab_y, (qfl&127)/100.0);
		BTIC2F_MakeQuantTabFastIJG_Y(ctx->qtab_uv, (qfl&127)/100.0);
	}
	
//	ctx->yeps=16;
//	ctx->yeps=(qfl&127)*(16.0/50.0);
//	ctx->yeps=(100-(qfl&127))*(16.0/50.0);
	ctx->yeps=4+(100-(qfl&127))*(12.0/50.0);
	
	if((qfl&127)>=100)
	{
		ctx->yeps=0;
	}
	
//	bxs0>>=1;		//BGB: Debug, half X

	ctx->dcp_y=0;
	ctx->dcp_u=0;
	ctx->dcp_v=0;

	for(y=0; y<bys0; y++)
	{
		for(x=0; x<bxs0; x++)
		{
			ctx->EncodeTransImageBlock(ctx, x, y,
				img+(y*bsz*ystride)+(x*bsz*xstride),
				xstride, ystride, bsz, bsz);
		}
		if(xs&(bsz-1))
		{
			ctx->EncodeTransImageBlock(ctx, x, y,
				img+(y*bsz*ystride)+(x*bsz*xstride),
				xstride, ystride, xs&(bsz-1), bsz);
		}
	}

	if(ys&(bsz-1))
	{
		for(x=0; x<bxs0; x++)
		{
			ctx->EncodeTransImageBlock(ctx, x, y,
				img+(y*bsz*ystride)+(x*bsz*xstride),
				xstride, ystride, bsz, ys&(bsz-1));
		}
		if(xs&(bsz-1))
		{
			ctx->EncodeTransImageBlock(ctx, x, y,
				img+(y*bsz*ystride)+(x*bsz*xstride),
				xstride, ystride, xs&(bsz-1), ys&(bsz-1));
		}
	}

	ctx->dcp_y=0;
	ctx->dcp_u=0;
	ctx->dcp_v=0;

	for(y=0; y<bys1; y++)
	{
		for(x=0; x<bxs1; x++)
		{
			ctx->StatTransBlock(ctx, x, y);
		}
	}


	ct=obuf;

	btic2f_setu16(tbuf+0, ctx->xs);
	btic2f_setu16(tbuf+2, ctx->ys);
	btic2f_setu16(tbuf+4, ctx->fl);
	tbuf[6]=0;
	tbuf[7]=0;
	if(is444)
		tbuf[7]=1;
	ct=BTIC2F_EncodeEmitTwoCC(ct, BTIC2F_TCC_HX, tbuf, 8);
	
	ct1=tbuf;
	*ct1++=1;	memcpy(ct1, ctx->qtab_y, 64);	ct1+=64;
	*ct1++=2;	memcpy(ct1, ctx->qtab_uv, 64);	ct1+=64;
	*ct1++=0;
	
	sz=ct1-tbuf;
	ct=BTIC2F_EncodeEmitTwoCC(ct, BTIC2F_TCC_QT, tbuf, sz);

	BTIC2F_BuildLengths(ctx, ctx->hstab_dc, ctx->hltab_dc);
	BTIC2F_BuildLengths(ctx, ctx->hstab_ac, ctx->hltab_ac);

	BTIC2F_SetupEncodeBuffer(ctx, tbuf, 4096);
	BTIC2F_WriteBits(ctx, 1, 4);
	BTIC2F_WritePackedLengths(ctx, ctx->hltab_dc);
	BTIC2F_WriteBits(ctx, 2, 4);
	BTIC2F_WritePackedLengths(ctx, ctx->hltab_ac);
	BTIC2F_WriteBits(ctx, 0, 4);
	BTIC2F_FlushWriteBits(ctx);
	sz=ctx->ct-tbuf;
	ct=BTIC2F_EncodeEmitTwoCC(ct, BTIC2F_TCC_HT, tbuf, sz);

	BTIC2F_SetupEncTableLengths(ctx, ctx->hetab_dc, ctx->hltab_dc);
	BTIC2F_SetupEncTableLengths(ctx, ctx->hetab_ac, ctx->hltab_ac);

	ctt=obuf+65536;
	BTIC2F_SetupEncodeBuffer(ctx, ctt, obsz-65536);

	ctx->dcp_y=0;
	ctx->dcp_u=0;
	ctx->dcp_v=0;

	for(y=0; y<bys1; y++)
	{
		for(x=0; x<bxs1; x++)
		{
//			BTIC2F_EncodeTransBlock420(ctx, x, y);
			ctx->EncodeTransBlock(ctx, x, y);
		}
	}

	BTIC2F_FlushWriteBits(ctx);
	sz=ctx->ct-ctt;
	ct=BTIC2F_EncodeEmitTwoCC(ct, BTIC2F_TCC_IX, ctt, sz);
	
	return(ct-obuf);
}

int BTIC2F_EncodeFrameImage(BTIC2F_EncState *ctx,
	byte *obuf, int obsz, byte *img, int qfl)
{
	int sz;

	sz=BTIC2F_EncodeTransImage(ctx,
		obuf, obsz, img, 4, ctx->xs*4, qfl);
	return(sz);
}
