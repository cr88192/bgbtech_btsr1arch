#define TKULZ_STATUS_NONE	0
#define TKULZ_STATUS_END	1
#define TKULZ_STATUS_BADTAG	2
#define TKULZ_STATUS_BADESC	3
#define TKULZ_STATUS_BADHT	4

#ifndef TKULZ_BYTE
#define TKULZ_BYTE
typedef unsigned char		byte;
typedef unsigned short	u16;
typedef unsigned int		u32;

typedef signed char		sbyte;
typedef signed short		s16;
typedef signed int		s32;
#endif

// #define TKULZ_LEN13

#define TKULZ_HTABSZ 8192
#define TKULZ_HTABNB 13

#ifndef TKSVC_DBGBREAK
#ifdef _MSC_VER
#define TKSVC_DBGBREAK	__debugbreak();
#else
#define TKSVC_DBGBREAK
#endif
#endif

#if defined(_M_IX86) || defined(_M_X64)
#define TKSVC_UNALACC_LE
#endif

typedef struct TKSVC0D_DecState_s TKSVC0D_DecState;

struct TKSVC0D_DecState_s {
	byte *cs;
	u32 win;
	byte pos;
	byte status;

	byte is_pf;					/* Is P-Frame */
	byte is_444;				/* Is 4:4:4 */
	int xs, ys;

	u16 htab[8][TKULZ_HTABSZ];	/* Huffman Tables */
	int qtab[4][64];			/* Quantization Tables */
	byte use_dct[4];			/* Plane Uses DCT */
	
	/* DC Planes */
	int dcxs[4];
	int dcys[4];
	int *dcp[4];				/* DC Planes */
	int *mvxp[4];				/* Motion Vector Planes */
	int *mvyp[4];				/* Motion Vecroe Planes */
	
	byte *yuvp[4];				/* YUV Planes */
	byte *lyuvp[4];				/* Last Frame YUV Planes */

};

/* Linear->ZigZag */
static const char tksvc0_zigzag[64]={
 0,  1,  5,  6, 14, 15, 27, 28,
 2,  4,  7, 13, 16, 26, 29, 42,
 3,  8, 12, 17, 25, 30, 41, 43,
 9, 11, 18, 24, 31, 40, 44, 53,
10, 19, 23, 32, 39, 45, 52, 54,
20, 22, 33, 38, 46, 51, 55, 60,
21, 34, 37, 47, 50, 56, 59, 61,
35, 36, 48, 49, 57, 58, 62, 63
};

/* ZigZag->Linear */
static const char tksvc0_zigzag2[64]={
 0,  1,  8, 16,  9,  2,  3, 10,
17, 24, 32, 25, 18, 11,  4,  5,
12, 19, 26, 33, 40, 48, 41, 34,
27, 20, 13,  6,  7, 14, 21, 28,
35, 42, 49, 56, 57, 50, 43, 36,
29, 22, 15, 23, 30, 37, 44, 51,
58, 59, 52, 45, 38, 31, 39, 46,
53, 60, 61, 54, 47, 55, 62, 63
};

int TKSVC0D_PeekBits(TKSVC0D_DecState *ctx, int bits)
	{ return((ctx->win>>ctx->pos)&((1<<bits)-1)); }

void TKSVC0D_SkipBits(TKSVC0D_DecState *ctx, int bits)
{
	ctx->pos+=bits;
#ifdef TKSVC_UNALACC_LE
// #if 0
	ctx->cs+=(ctx->pos>>3);
//	ctx->win=*(u32 *)(ctx->cs-4);
	ctx->win=*(u32 *)ctx->cs;
	ctx->pos&=7;
#else
	while(ctx->pos>=8)
		{ ctx->win=(ctx->win>>8)|((*ctx->cs++)<<24); ctx->pos-=8; }
#endif
}

int TKSVC0D_ReadBits(TKSVC0D_DecState *ctx, int bits)
{
	int b;
	b=TKSVC0D_PeekBits(ctx, bits);
	TKSVC0D_SkipBits(ctx, bits);
	return(b);
}

void TKSVC0D_ReadPackedLengths(TKSVC0D_DecState *ctx, byte *cls)
{
	byte *t, *te;
	int i, j, c, nz, zc, lc;
	
	t=cls; te=cls+256; lc=0;
	while(t<te)
	{
		c=TKSVC0D_ReadBits(ctx, 4); zc=0; nz=0;
		if(c<14)
			{ *t++=c; lc=c; }
		else if(c==14)
			{ nz=TKSVC0D_ReadBits(ctx, 4)+3; }
		else if(c==15)
		{
			j=TKSVC0D_ReadBits(ctx, 2);
			if(j==0)
				{ nz=TKSVC0D_ReadBits(ctx, 6)+19; }
			else if(j==1)
			{
				nz=TKSVC0D_ReadBits(ctx, 6)+3; zc=lc;
				if(nz==3)
					{ nz=te-t; zc=0; }
			}
			else
				{ ctx->status=TKULZ_STATUS_BADHT; }
		}
		while(nz>0)
			{ *t++=zc; nz--; }
	}
	
	if(t>te)
		{ ctx->status=TKULZ_STATUS_BADHT; }
}

byte TKSVC0D_TransposeByte(byte v)
{
	static const byte trans4[16]={
		0x0,0x8,0x4,0xC, 0x2,0xA,0x6,0xE,
		0x1,0x9,0x5,0xD, 0x3,0xB,0x7,0xF};
	return((trans4[v&15]<<4)|trans4[(v>>4)&15]);
}

u16 TKSVC0D_TransposeWord(u16 v)
	{ return((TKSVC0D_TransposeByte(v&255)<<8)|TKSVC0D_TransposeByte(v>>8)); }

void TKSVC0D_SetupTableLengths(TKSVC0D_DecState *ctx, u16 *htab, byte *cls)
{
	int c, l, tc, ntc;
	int i, j, k;

	for(i=0; i<TKULZ_HTABSZ; i++)
		htab[i]=0;
	
	c=0;
	for(l=1; l<(TKULZ_HTABNB+1); l++)
	{
		for(i=0; i<256; i++)
			if(cls[i]==l)
		{
			ntc=1<<(TKULZ_HTABNB-l);
			tc=c<<(TKULZ_HTABNB-l);
			for(j=0; j<ntc; j++)
			{
				k=TKSVC0D_TransposeWord(tc+j)>>(16-TKULZ_HTABNB);
				htab[k]=(l<<8)|i;
			}
			c++;
		}
		c=c<<1;
	}
}

int TKSVC0D_DecodeHuffSym(TKSVC0D_DecState *ctx, u16 *htab)
{
	int b, c;
	b=(ctx->win>>ctx->pos)&(TKULZ_HTABSZ-1);
	c=htab[b];
//	if(!((c>>8)&15))
//		{ TKSVC_DBGBREAK }
	TKSVC0D_SkipBits(ctx, (c>>8)&15);
	return(c&255);
}

int TKSVC0D_DecodeSymbol(TKSVC0D_DecState *ctx, int htn)
{
	return(TKSVC0D_DecodeHuffSym(ctx, ctx->htab[htn]));
}

void TKSVC0D_DecodeCoeff(TKSVC0D_DecState *ctx, int htn, int *rz, int *rv)
{
	int pfx, eb, d, z;

	pfx=TKSVC0D_DecodeSymbol(ctx, htn);
	if(pfx&0x80)
	{
		z=(pfx>>4)&7;

//		eb=(pfx>>2)&3; d=pfx&3;
//		if(eb) { eb--; d=((4+d)<<eb)|TKSVC0D_ReadBits(ctx, eb); }
		eb=(pfx>>1)&7; d=pfx&1;
		if(eb) { eb--; d=((2+d)<<eb)|TKSVC0D_ReadBits(ctx, eb); }

		if(z)
		{
			d=(d>>1)^((d<<31)>>31);
			*rz=z; *rv=d;
		}
		else
			{ *rz=d; *rv=0; }
	}else if(pfx)
	{
		eb=pfx>>2; d=pfx&3;
		if(eb) { eb--; d=((4+d)<<eb)|TKSVC0D_ReadBits(ctx, eb); }
		d=(d>>1)^((d<<31)>>31);
		*rz=0;	*rv=d;
	}else
	{
		*rz=-1;	*rv=0;
	}
}

void TKSVC0D_DecodeCoeffDc(TKSVC0D_DecState *ctx, int htn, int *rz, int *rv)
{
	int z, v;
	TKSVC0D_DecodeCoeff(ctx, htn, &z, &v);
	if(z<0)z=0;
	*rz=z; *rv=v;
}

void TKSVC0D_DecodeBlockAC(TKSVC0D_DecState *ctx, int htn,
	int *blk, int *rqt, int *rskp)
{
	int z, v, qv;
	int i, j, k;
	
	memset(blk+1, 0, 63*sizeof(int));
	
	i=1;
	while(i<64)
	{
		TKSVC0D_DecodeCoeff(ctx, htn, &z, &v);
		if(z<0)
			break;
		if(z>=64)
		{
			*rskp=z-63;
			break;
		}

//		j=z;
//		while(j--)
//			{ blk[tksvc0_zigzag2[i++]]=0; }
		i+=z;

		qv=rqt[i];
		k=tksvc0_zigzag2[i++];
		blk[k]=v*qv;
//		blk[k]=v;
	}

#if 0
	while((i+4)<64)
	{
		blk[tksvc0_zigzag2[i+0]]=0;
		blk[tksvc0_zigzag2[i+1]]=0;
		blk[tksvc0_zigzag2[i+2]]=0;
		blk[tksvc0_zigzag2[i+3]]=0;
		i+=4;
	}

	while(i<64)
		{ blk[tksvc0_zigzag2[i++]]=0; }
#endif
		
	if(i>64)
	{
		TKSVC_DBGBREAK
	}
}

int TKSVC0D_Paeth(int a, int b, int c)
{
	int p, pa, pb, pc;

	p=a+b-c;
	pa=(p>a)?(p-a):(a-p);
	pb=(p>b)?(p-b):(b-p);
	pc=(p>c)?(p-c):(c-p);

	p=(pa<=pb)?((pa<=pc)?a:c):((pb<=pc)?b:c);
	return(p);
}

void TKSVC0D_DecodeDcPlaneData(TKSVC0D_DecState *ctx, int htn, int pn)
{
	static const char qn2pn4[4]={0,1,2,0};
	int xs, ys;
	int *dcp;
	int a0, a1, a2, ap, qv;
	int x, y, px, z, v;
	
	xs=ctx->dcxs[pn&3];
	ys=ctx->dcys[pn&3];
	dcp=ctx->dcp[pn&3];
	
	if(pn&0xC)
	{
		xs=ctx->dcxs[qn2pn4[pn&3]];
		ys=ctx->dcys[qn2pn4[pn&3]];
		if(pn&0x4)
			{ dcp=ctx->mvxp[pn&3]; }
		if(pn&0x8)
			{ dcp=ctx->mvyp[pn&3]; }
	}
	
	z=0; v=0;
	TKSVC0D_DecodeCoeffDc(ctx, htn, &z, &v);
	for(y=0; y<ys; y++)
		for(x=0; x<xs; x++)
	{
		px=y*xs+x;

		if(y)
		{
			if(x)
			{
				a0=dcp[px-xs-1];
				a1=dcp[px-xs-0];
				a2=dcp[px   -1];
				ap=TKSVC0D_Paeth(a2, a1, a0);
			}else
				{ ap=dcp[px-xs]; }
		}else
		{
			if(x)
				{ ap=dcp[px-1]; }
			else
				{ ap=0; }
		}
	
		if(z)
		{
			dcp[px]=ap;
			z--;
			continue;
		}else
		{
			dcp[px]=ap+v;
			TKSVC0D_DecodeCoeffDc(ctx, htn, &z, &v);
		}
	}
}

#if 1
void TKSVC0D_TransIDCT_Horiz(s32 *iblk, s32 *oblk)
{
	int a, b, c, d, e, f;
	int i, j, k, l;
	int m, n, o, p;

	a=iblk[0]* 91;	b=iblk[4]* 91;
	c=iblk[2]*118;	d=iblk[2]* 49;
	e=iblk[6]*118;	f=iblk[6]* 49;
	i=a+c+b+f;	j=a+d-b-e;	k=a-d-b+e;	l=a-c+b-f;
	m=iblk[1]*126+iblk[3]*106+iblk[5]* 71+iblk[7]* 25;
	n=iblk[1]*106-iblk[3]* 25-iblk[5]*126-iblk[7]* 71;
	o=iblk[1]* 71-iblk[3]*126+iblk[5]* 25+iblk[7]*106;
	p=iblk[1]* 25-iblk[3]* 71+iblk[5]*106-iblk[7]*126;
	oblk[0]=i+m;	oblk[1]=j+n;	oblk[2]=k+o;	oblk[3]=l+p;
	oblk[4]=l-p;	oblk[5]=k-o;	oblk[6]=j-n;	oblk[7]=i-m;
}

void TKSVC0D_TransIDCT_Vert(s32 *iblk, s32 *oblk)
{
	int a, b, c, d, e, f;
	int i, j, k, l;
	int m, n, o, p;

	a=iblk[ 0]* 91;	b=iblk[32]*91;
	c=iblk[16]*118;	d=iblk[16]*49;
	e=iblk[48]*118;	f=iblk[48]*49;
	i=a+c+b+f;	j=a+d-b-e;	k=a-d-b+e;	l=a-c+b-f;
	m=iblk[8]*126+iblk[24]*106+iblk[40]* 71+iblk[56]* 25;
	n=iblk[8]*106-iblk[24]* 25-iblk[40]*126-iblk[56]* 71;
	o=iblk[8]* 71-iblk[24]*126+iblk[40]* 25+iblk[56]*106;
	p=iblk[8]* 25-iblk[24]* 71+iblk[40]*106-iblk[56]*126;
	oblk[ 0]=i+m;	oblk[ 8]=j+n;	oblk[16]=k+o;	oblk[24]=l+p;
	oblk[32]=l-p;	oblk[40]=k-o;	oblk[48]=j-n;	oblk[56]=i-m;
}

void TKSVC0D_TransIDCT(s32 *iblk, s32 *oblk, byte isflat)
{
	int s[64];
	int t[64];
	int v;
	int i, j;

	if(isflat)
	{
		v=(iblk[0]*8281+32768)>>16;
		for(i=0; i<64; i++)
			oblk[i]=v;
		return;
	}

	TKSVC0D_TransIDCT_Horiz(iblk+0, s+0);
	TKSVC0D_TransIDCT_Horiz(iblk+8, s+8);
	TKSVC0D_TransIDCT_Horiz(iblk+16, s+16);
	TKSVC0D_TransIDCT_Horiz(iblk+24, s+24);
	TKSVC0D_TransIDCT_Horiz(iblk+32, s+32);
	TKSVC0D_TransIDCT_Horiz(iblk+40, s+40);
	TKSVC0D_TransIDCT_Horiz(iblk+48, s+48);
	TKSVC0D_TransIDCT_Horiz(iblk+56, s+56);

	TKSVC0D_TransIDCT_Vert(s+0, t+0);
	TKSVC0D_TransIDCT_Vert(s+1, t+1);
	TKSVC0D_TransIDCT_Vert(s+2, t+2);
	TKSVC0D_TransIDCT_Vert(s+3, t+3);
	TKSVC0D_TransIDCT_Vert(s+4, t+4);
	TKSVC0D_TransIDCT_Vert(s+5, t+5);
	TKSVC0D_TransIDCT_Vert(s+6, t+6);
	TKSVC0D_TransIDCT_Vert(s+7, t+7);

	for(i=0; i<64; i++)
	{
		j=((t[i]+32768)>>16);
		oblk[i]=j;
	}
}
#endif

void TKSVC0D_TransIWHT_Horiz(s32 *iblk, s32 *oblk)
{
	int ob0, ob1, ob2, ob3, ob4, ob5, ob6, ob7;
	int a0, a1, a2, a3, a4, a5, a6, a7;
	int b0, b1, b2, b3, b4, b5, b6, b7;
	int c0, c1, c2, c3, c4, c5, c6, c7;

	c0=iblk[0];		c4=iblk[1];
	c6=iblk[2];		c2=iblk[3];
	c3=iblk[4];		c7=iblk[5];
	c5=iblk[6];		c1=iblk[7];

//	c0=iblk[0];		c1=iblk[1];
//	c2=iblk[2];		c3=iblk[3];
//	c4=iblk[4];		c5=iblk[5];
//	c6=iblk[6];		c7=iblk[7];

	b1=(c0>>1)-c1;	b3=(c2>>1)-c3;
	b5=(c4>>1)-c5;	b7=(c6>>1)-c7;
	b0=c0-b1;		b2=c2-b3;
	b4=c4-b5;		b6=c6-b7;

	a2=(b0>>1)-b2;	a3=(b1>>1)-b3;
	a6=(b4>>1)-b6;	a7=(b5>>1)-b7;
	a0=b0-a2;		a1=b1-a3;
	a4=b4-a6;		a5=b5-a7;

	ob4=(a0>>1)-a4;	ob5=(a1>>1)-a5;
	ob6=(a2>>1)-a6;	ob7=(a3>>1)-a7;
	ob0=a0-ob4;		ob1=a1-ob5;
	ob2=a2-ob6;		ob3=a3-ob7;

	oblk[0]=ob0;	oblk[1]=ob1;
	oblk[2]=ob2;	oblk[3]=ob3;
	oblk[4]=ob4;	oblk[5]=ob5;
	oblk[6]=ob6;	oblk[7]=ob7;
}

void TKSVC0D_TransIWHT_Vert(s32 *iblk, s32 *oblk)
{
	int ob0, ob1, ob2, ob3, ob4, ob5, ob6, ob7;
	int a0, a1, a2, a3, a4, a5, a6, a7;
	int b0, b1, b2, b3, b4, b5, b6, b7;
	int c0, c1, c2, c3, c4, c5, c6, c7;

	c0=iblk[8*0];	c4=iblk[8*1];
	c6=iblk[8*2];	c2=iblk[8*3];
	c3=iblk[8*4];	c7=iblk[8*5];
	c5=iblk[8*6];	c1=iblk[8*7];

//	c0=iblk[8*0];	c1=iblk[8*1];
//	c2=iblk[8*2];	c3=iblk[8*3];
//	c4=iblk[8*4];	c5=iblk[8*5];
//	c6=iblk[8*6];	c7=iblk[8*7];

	b1=(c0>>1)-c1;	b3=(c2>>1)-c3;
	b5=(c4>>1)-c5;	b7=(c6>>1)-c7;
	b0=c0-b1;		b2=c2-b3;
	b4=c4-b5;		b6=c6-b7;

	a2=(b0>>1)-b2;	a3=(b1>>1)-b3;
	a6=(b4>>1)-b6;	a7=(b5>>1)-b7;
	a0=b0-a2;		a1=b1-a3;
	a4=b4-a6;		a5=b5-a7;

	ob4=(a0>>1)-a4;	ob5=(a1>>1)-a5;
	ob6=(a2>>1)-a6;	ob7=(a3>>1)-a7;
	ob0=a0-ob4;		ob1=a1-ob5;
	ob2=a2-ob6;		ob3=a3-ob7;

	oblk[8*0]=ob0;	oblk[8*1]=ob1;
	oblk[8*2]=ob2;	oblk[8*3]=ob3;
	oblk[8*4]=ob4;	oblk[8*5]=ob5;
	oblk[8*6]=ob6;	oblk[8*7]=ob7;
}

void TKSVC0D_TransIWHT(s32 *iblk, s32 *oblk, byte isflat)
{
	s32 s[64];
	int v, i;

	if(isflat)
	{
		v=iblk[0]>>6;
		for(i=0; i<64; i++)
			oblk[i]=v;
		return;
	}

	TKSVC0D_TransIWHT_Vert(iblk+0, s+0);
	TKSVC0D_TransIWHT_Vert(iblk+1, s+1);
	TKSVC0D_TransIWHT_Vert(iblk+2, s+2);
	TKSVC0D_TransIWHT_Vert(iblk+3, s+3);
	TKSVC0D_TransIWHT_Vert(iblk+4, s+4);
	TKSVC0D_TransIWHT_Vert(iblk+5, s+5);
	TKSVC0D_TransIWHT_Vert(iblk+6, s+6);
	TKSVC0D_TransIWHT_Vert(iblk+7, s+7);

	TKSVC0D_TransIWHT_Horiz(s+0, oblk+0);
	TKSVC0D_TransIWHT_Horiz(s+8, oblk+8);
	TKSVC0D_TransIWHT_Horiz(s+16, oblk+16);
	TKSVC0D_TransIWHT_Horiz(s+24, oblk+24);
	TKSVC0D_TransIWHT_Horiz(s+32, oblk+32);
	TKSVC0D_TransIWHT_Horiz(s+40, oblk+40);
	TKSVC0D_TransIWHT_Horiz(s+48, oblk+48);
	TKSVC0D_TransIWHT_Horiz(s+56, oblk+56);
}

int TKSVC0D_Clamp65535(int v)
{
	if(v>>16)
		return((v>0)?65535:0);
	return(v);
}

int TKSVC0D_Clamp255(int v)
{
	int v1, vp1, vn1;

//	vp1=(~(((v>>8)-1)>>31))&255;
//	vn1=~(v>>31);
//	v1=(v|vp1)&vn1;
//	return(v1);

	if(v>>8)
		return((v>0)?255:0);
	return(v);
}

int TKSVC0D_Clamp32768S(int v)
{
	if((v+32768)>>16)
		return((v>0)?32767:(-32768));
	return(v);
}

void TKSVC0D_DecodeAcPlaneData(TKSVC0D_DecState *ctx, int htn, int pn, int qn)
{
	byte pblk[64];
	int blk[64];
	int *qt;
	
	int xs, ys, xsp, ysp;
	int *dcp, *mvxp, *mvyp;
	byte *yvp, *lyvp;
	int a0, a1, a2, ap, qv, mvy, mvx;
	int x, y, x1, y1, px, z, v, bpx, lpx, lz;
	int i0, i1, i2, i3;

	if(pn>=4)
	{
		ctx->status=TKULZ_STATUS_BADTAG;
		return;
	}
	
	xs=ctx->dcxs[pn];
	ys=ctx->dcys[pn];
	dcp=ctx->dcp[pn];

	qt=ctx->qtab[qn];
	qv=qt[0];
	
	yvp=ctx->yuvp[pn];
	lyvp=ctx->lyuvp[pn];
	xsp=xs*8;
	ysp=ys*8;
	
	mvxp=ctx->mvxp[qn];
	mvyp=ctx->mvyp[qn];
	
	z=0;
	for(y=0; y<ys; y++)
		for(x=0; x<xs; x++)
	{
		px=y*xs+x;
		blk[0]=dcp[px]*qv;

		if(!z)
			{ TKSVC0D_DecodeBlockAC(ctx, htn, blk, qt, &z); lz=z; if(z)z--; }
		else
			{ lz=z--; }

		if(!ctx->use_dct[qn])
			{ TKSVC0D_TransIWHT(blk, blk, lz); }
		else
			{ TKSVC0D_TransIDCT(blk, blk, lz); }
		
		mvx=mvxp[px];
		mvy=mvyp[px];
		
		if(ctx->is_pf)
		{
			if(lz)
			{
				for(y1=0; y1<8; y1++)
				{
					bpx=y1*8;
					px=((y*8+y1)*xsp)+(x*8);
					lpx=((y*8+y1+mvy)*xsp)+(x*8+mvx);
					for(x1=0; x1<8; x1++)
						{ yvp[px+x1]=lyvp[lpx+x1]; }
				}
			}else
			{
				for(y1=0; y1<8; y1++)
				{
					bpx=y1*8;
					px=((y*8+y1)*xsp)+(x*8);
					lpx=((y*8+y1+mvy)*xsp)+(x*8+mvx);
					for(x1=0; x1<8; x1++)
						{ yvp[px++]=TKSVC0D_Clamp255(lyvp[lpx++]+blk[bpx++]); }
				}
			}
		}else
		{
#if 1
//			for(y1=0; y1<64; y1++)
//			{
//				pblk[y1]=TKSVC0D_Clamp255(blk[y1]+128);
//			}

			for(y1=0; y1<64; y1+=4)
			{
				i0=blk[y1+0]+128;	i1=blk[y1+1]+128;
				i2=blk[y1+2]+128;	i3=blk[y1+3]+128;
				if((i0|i1|i2|i3)>>8)
				{
					i0=TKSVC0D_Clamp255(i0);
					i1=TKSVC0D_Clamp255(i1);
					i2=TKSVC0D_Clamp255(i2);
					i3=TKSVC0D_Clamp255(i3);
				}
				pblk[y1+0]=i0;	pblk[y1+1]=i1;
				pblk[y1+2]=i2;	pblk[y1+3]=i3;
			}

			bpx=0;
			px=((y*8)*xsp)+(x*8);
			for(y1=0; y1<8; y1++)
			{
//				bpx=y1*8;
//				px=((y*8+y1)*xsp)+(x*8);
				memcpy(yvp+px, pblk+bpx, 8);
				bpx+=8;		px+=xsp;
			}
#endif

#if 0
			for(y1=0; y1<8; y1++)
			{
				bpx=y1*8;
				px=((y*8+y1)*xsp)+(x*8);

#if 0
				i0=blk[bpx+0]+128;		i1=blk[bpx+1]+128;
				i2=blk[bpx+2]+128;		i3=blk[bpx+3]+128;
				if((i0|i1|i2|i3)>>8)
				{
					i0=TKSVC0D_Clamp255(i0);
					i1=TKSVC0D_Clamp255(i1);
					i2=TKSVC0D_Clamp255(i2);
					i3=TKSVC0D_Clamp255(i3);
				}
				yvp[px+0]=i0;
				yvp[px+1]=i1;
				yvp[px+2]=i2;
				yvp[px+3]=i3;

				i0=blk[bpx+4]+128;		i1=blk[bpx+5]+128;
				i2=blk[bpx+6]+128;		i3=blk[bpx+7]+128;
				if((i0|i1|i2|i3)>>8)
				{
					i0=TKSVC0D_Clamp255(i0);
					i1=TKSVC0D_Clamp255(i1);
					i2=TKSVC0D_Clamp255(i2);
					i3=TKSVC0D_Clamp255(i3);
				}
				yvp[px+4]=i0;
				yvp[px+5]=i1;
				yvp[px+6]=i2;
				yvp[px+7]=i3;
#endif

//				yvp[px+0]=TKSVC0D_Clamp255(blk[bpx+0]+128);
//				yvp[px+1]=TKSVC0D_Clamp255(blk[bpx+1]+128);
//				yvp[px+2]=TKSVC0D_Clamp255(blk[bpx+2]+128);
//				yvp[px+3]=TKSVC0D_Clamp255(blk[bpx+3]+128);
//				yvp[px+4]=TKSVC0D_Clamp255(blk[bpx+4]+128);
//				yvp[px+5]=TKSVC0D_Clamp255(blk[bpx+5]+128);
//				yvp[px+6]=TKSVC0D_Clamp255(blk[bpx+6]+128);
//				yvp[px+7]=TKSVC0D_Clamp255(blk[bpx+7]+128);

//				for(x1=0; x1<8; x1++)
//				{
//					yvp[px++]=TKSVC0D_Clamp255(blk[bpx++]);
//					yvp[px++]=TKSVC0D_Clamp255(blk[bpx++]+128);
//				}

				for(x1=0; x1<8; x1++)
				{
					yvp[px+x1]=TKSVC0D_Clamp255(blk[bpx+x1]+128);
				}
			}
#endif
		}
	}
}


void TKSVC0D_DecodeHuffmanTable(TKSVC0D_DecState *ctx)
{
	byte cl[256];
	int tn;
	
	ctx->is_pf=0;

	tn=TKSVC0D_ReadBits(ctx, 4);
	if(tn>=8)
	{
		ctx->status=TKULZ_STATUS_BADTAG;
		return;
	}

	TKSVC0D_ReadPackedLengths(ctx, cl);
	TKSVC0D_SetupTableLengths(ctx, ctx->htab[tn], cl);
}

void TKSVC0D_DecodeQuantizationTable(TKSVC0D_DecState *ctx)
{
	int *qt;
	int tn, st, sv, v, lv, lz;
	int i;
	
	ctx->is_pf=0;

	tn=TKSVC0D_ReadBits(ctx, 4);
//	if(tn>=4)
//	{
//		ctx->status=TKULZ_STATUS_BADTAG;
//		return;
//	}

	qt=ctx->qtab[tn&3];
	
	if(tn&8)
	{
		st=TKSVC0D_ReadBits(ctx, 4);
		for(i=0; i<64; i++)
		{
			sv=TKSVC0D_ReadBits(ctx, st);
			qt[tksvc0_zigzag2[i]]=sv;
		}
	}else
	{
		lv=0; lz=0;
		for(i=0; i<64; i++)
		{
			if(lz)
			{
				lz--;
				qt[tksvc0_zigzag2[i]]=lv;
				continue;
			}

			st=TKSVC0D_ReadBits(ctx, 2);
			sv=TKSVC0D_ReadBits(ctx, 2+(st*3));
			if((st==2) && (sv<16))
			{
				lz=sv;
				qt[tksvc0_zigzag2[i]]=lv;
				continue;
			}
			v=(sv>>1)^((sv<<31)>>31);
			lv+=v;
			qt[tksvc0_zigzag2[i]]=lv;
		}
	}
	
	ctx->use_dct[tn&3]=0;
	if(tn&4)
		ctx->use_dct[tn&3]=1;

#if 0
	printf("%d: ", tn);
	for(i=0; i<64; i++)
	{
		printf("%d ", qt[i]);
	}
	printf("\n");
#endif
}

void TKSVC0D_DecodeDcPlane(TKSVC0D_DecState *ctx)
{
	static const byte htpn[16]={
		0, 1, 1, 2, 0, 1, 1, 2,
		3, 3, 3, 3, 3, 3, 3, 3};
	int pn;
	int htn;

	pn=TKSVC0D_ReadBits(ctx, 4);
	htn=htpn[pn];
	TKSVC0D_DecodeDcPlaneData(ctx, htn, pn);
}

void TKSVC0D_DecodeAcPlane(TKSVC0D_DecState *ctx)
{
	static const byte htpn[16]={
		4, 5, 5, 6, 0, 0, 0, 0,
		3, 3, 3, 3, 3, 3, 3, 3};
	static const byte htqn[16]={
		0, 1, 1, 2, 0, 0, 0, 0,
		3, 3, 3, 3, 3, 3, 3, 3};
	int pn;
	int htn, hqn;

	pn=TKSVC0D_ReadBits(ctx, 4);
	if(pn>=4)
	{
		ctx->status=TKULZ_STATUS_BADTAG;
		return;
	}
	
	htn=htpn[pn];
	hqn=htqn[pn];
	TKSVC0D_DecodeAcPlaneData(ctx, htn, pn, hqn);
}

void TKSVC0D_DecodeUserSegment(TKSVC0D_DecState *ctx)
{
	int st;
	int len;
	
	st=TKSVC0D_ReadBits(ctx, 4);

	TKSVC0D_ReadBits(ctx, 7);
	ctx->pos=0;
	
	switch(st&3)
	{
		case 0:		len=TKSVC0D_ReadBits(ctx, 8);	break;
		case 1:		len=TKSVC0D_ReadBits(ctx, 16);	break;
		case 2:
			len=TKSVC0D_ReadBits(ctx, 16);
			len=TKSVC0D_ReadBits(ctx, 8)<<16;
			break;
		case 3:
			len=TKSVC0D_ReadBits(ctx, 16);
			len=TKSVC0D_ReadBits(ctx, 16)<<16;
			break;
	}
	
	TKSVC0D_SkipBits(ctx, len*8);
}

void TKSVC0D_DecodeTagBlock(TKSVC0D_DecState *ctx)
{
	int tag;
	
	tag=TKSVC0D_ReadBits(ctx, 4);
	
//	printf("TagBlk %d\n", tag);
	
	if(tag==0)
		{ ctx->status=TKULZ_STATUS_END; return; }
	if(tag==1)
		{ TKSVC0D_DecodeHuffmanTable(ctx); return; }
	if(tag==2)
		{ TKSVC0D_DecodeQuantizationTable(ctx); return; }
	if(tag==3)
		{ TKSVC0D_DecodeUserSegment(ctx); return; }
	if(tag==4)
		{ TKSVC0D_DecodeDcPlane(ctx); return; }
	if(tag==5)
		{ TKSVC0D_DecodeAcPlane(ctx); return; }
	ctx->status=TKULZ_STATUS_BADTAG;
}

void TKSVC0D_SetupDecodeBuffers(TKSVC0D_DecState *ctx,
	byte *src, int ssz)
{
	ctx->cs=src;
	ctx->pos=0;		ctx->status=0;
#ifndef TKSVC_UNALACC_LE
	TKSVC0D_SkipBits(ctx, 16);
	TKSVC0D_SkipBits(ctx, 16);
#else
	TKSVC0D_SkipBits(ctx, 0);
#endif
}

void TKSVC0D_SetupForResolution(TKSVC0D_DecState *ctx)
{
	int xs, ys, xs_y, ys_y, xs_uv, ys_uv;
	int i;
	
	xs=ctx->xs;
	ys=ctx->ys;

	if(ctx->is_444)
	{
		xs_y=((xs+7)/8);
		ys_y=((ys+7)/8);
		xs_uv=xs_y;
		ys_uv=ys_y;
	}else
	{
		xs_y=((xs+15)/16)*2;
		ys_y=((ys+15)/16)*2;
		xs_uv=xs_y/2;
		ys_uv=ys_y/2;
	}

	ctx->dcxs[0]=xs_y;
	ctx->dcxs[1]=xs_uv;
	ctx->dcxs[2]=xs_uv;
	ctx->dcxs[3]=xs_y;

	ctx->dcys[0]=ys_y;
	ctx->dcys[1]=ys_uv;
	ctx->dcys[2]=ys_uv;
	ctx->dcys[3]=ys_y;
	
	if(!ctx->dcp[0])
	{
		for(i=0; i<4; i++)
		{
			ctx->dcp[i]=malloc(ctx->dcxs[i]*ctx->dcys[i]*sizeof(int));
			ctx->yuvp[i]=malloc(ctx->dcxs[i]*ctx->dcys[i]*64);
			ctx->lyuvp[i]=malloc(ctx->dcxs[i]*ctx->dcys[i]*64);
		}

		ctx->mvxp[0]=malloc(ctx->dcxs[0]*ctx->dcys[0]*sizeof(int));
		ctx->mvyp[0]=malloc(ctx->dcxs[0]*ctx->dcys[0]*sizeof(int));
		ctx->mvxp[1]=malloc(ctx->dcxs[1]*ctx->dcys[1]*sizeof(int));
		ctx->mvyp[1]=malloc(ctx->dcxs[1]*ctx->dcys[1]*sizeof(int));
		ctx->mvxp[2]=malloc(ctx->dcxs[3]*ctx->dcys[3]*sizeof(int));
		ctx->mvyp[2]=malloc(ctx->dcxs[3]*ctx->dcys[3]*sizeof(int));
	}
}

void TKSVC0D_TeardownBuffers(TKSVC0D_DecState *ctx)
{
	int i, j, k;

	if(ctx->dcp[0])
	{
		for(i=0; i<4; i++)
		{
			if(ctx->dcp[i])
				free(ctx->dcp[i]);
			if(ctx->yuvp[i])
				free(ctx->yuvp[i]);
			if(ctx->lyuvp[i])
				free(ctx->lyuvp[i]);
			if(ctx->mvxp[i])
				free(ctx->mvxp[i]);
			if(ctx->mvyp[i])
				free(ctx->mvyp[i]);

			ctx->dcp[i]=NULL;
			ctx->yuvp[i]=NULL;
			ctx->lyuvp[i]=NULL;
			ctx->mvxp[i]=NULL;
			ctx->mvyp[i]=NULL;
		}
	}
}

void TKSVC0D_SetFrameSize(TKSVC0D_DecState *ctx,
	int xs, int ys, int is444)
{
	if((xs!=ctx->xs) || (ys!=ctx->ys) || (ctx->is_444!=is444))
	{
		TKSVC0D_TeardownBuffers(ctx);
		ctx->xs=xs;
		ctx->ys=ys;
		ctx->is_444=is444;
		TKSVC0D_SetupForResolution(ctx);
	}
}

void TKSVC0D_SetupSwapFrames(TKSVC0D_DecState *ctx)
{
	byte *p1, *p2;
	int i;

	ctx->is_pf=1;	/* Assume P-Frame for now */

	for(i=0; i<4; i++)
	{
		p1=ctx->yuvp[i];		p2=ctx->lyuvp[i];
		ctx->yuvp[i]=p2;		ctx->lyuvp[i]=p1;
	}
}

int TKSVC0D_DecodeBuffer(TKSVC0D_DecState *ctx,
	byte *src, int ssz)
{
	TKSVC0D_SetupForResolution(ctx);
	TKSVC0D_SetupSwapFrames(ctx);
	TKSVC0D_SetupDecodeBuffers(ctx, src, ssz);
	while(!(ctx->status))
		{ TKSVC0D_DecodeTagBlock(ctx); }
	if(ctx->status && (ctx->status!=1))
		printf("Error %d\n", ctx->status);
	return(1);
}

int TKSVC0D_GetImageRGBA(TKSVC0D_DecState *ctx,
	byte *ibuf)
{
	byte *yvp, *uvp, *vvp, *avp;
	int xs_y, ys_y, xsp_y, ysp_y;
	int xs_uv, ys_uv, xsp_uv, ysp_uv;
	int xs, ys, xsh, ysh;
	int x, y;
	
	u32 px0, px1, px2, px3;
	int spx0, spx1, spx2;
	int dpx0, dpx1, dpx2, dpx3;
	int cy0, cy1, cy2, cy3;
	int ca0, ca1, ca2, ca3;

	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int cm0, cm1, cm2;

	int cu, cv, cu1, cv1;

	yvp=ctx->yuvp[0];
	uvp=ctx->yuvp[1];
	vvp=ctx->yuvp[2];
	avp=ctx->yuvp[3];

	xs_y=ctx->dcxs[0];
	ys_y=ctx->dcys[0];
	xsp_y=xs_y*8;
	ysp_y=ys_y*8;

	xs_uv=ctx->dcxs[1];
	ys_uv=ctx->dcys[1];
	xsp_uv=xs_uv*8;
	ysp_uv=ys_uv*8;
	
	xs=ctx->xs;
	ys=ctx->ys;
	xsh=xs/2;
	ysh=ys/2;
	
//	if((xs_y==xs_uv) && (ys_y==ys_uv))
	if(ctx->is_444)
	{
		for(y=0; y<ys; y++)
			for(x=0; x<xs; x++)
		{
			dpx0=(y*xs+x)*4;
			spx0=(y*xsp_y)+x;
			ca0=avp[spx0];	cg0=yvp[spx0];
			cb0=uvp[spx0];	cr0=vvp[spx0];
			ibuf[dpx0+0]=cb0;		ibuf[dpx0+1]=cg0;
			ibuf[dpx0+2]=cr0;		ibuf[dpx0+3]=ca0;
		}
		return(0);
	}
	
	for(y=0; y<ysh; y++)
		for(x=0; x<xsh; x++)
	{
		dpx0=((y*2+0)*xs+(x*2))*4;
//		dpx1=dpx0+4;
		dpx2=((y*2+1)*xs+(x*2))*4;
//		dpx3=dpx2+4;
		
		spx0=((y*2+0)*xsp_y)+(x*2);
		spx1=((y*2+1)*xsp_y)+(x*2);
		spx2=(y*xsp_uv)+x;
	
		ca0=avp[spx0+0];	ca1=avp[spx0+1];
		ca2=avp[spx1+0];	ca3=avp[spx1+1];

		cy0=yvp[spx0+0];	cy1=yvp[spx0+1];
		cy2=yvp[spx1+0];	cy3=yvp[spx1+1];
		cu=uvp[spx2];		cv=vvp[spx2];
		
		cu1=(cu-128)*2;
		cv1=(cv-128)*2;
		
		cr0=cy0+cv1;	cr1=cy1+cv1;
		cr2=cy2+cv1;	cr3=cy3+cv1;
		cb0=cy0+cu1;	cb1=cy1+cu1;
		cb2=cy2+cu1;	cb3=cy3+cu1;

		cm0=cr0|cr1|cr2|cr3;
		cm2=cb0|cb1|cb2|cb3;

//		cg0=(8*cy0-3*cr0-cb0)/4;
//		cg1=(8*cy1-3*cr1-cb1)/4;
//		cg2=(8*cy2-3*cr2-cb2)/4;
//		cg3=(8*cy3-3*cr3-cb3)/4;
		cg0=(4*cy0-cr0-cb0)/2;
		cg1=(4*cy1-cr1-cb1)/2;
		cg2=(4*cy2-cr2-cb2)/2;
		cg3=(4*cy3-cr3-cb3)/2;

		cm1=cg0|cg1|cg2|cg3;
		
		px0=(ca0<<24)|(cr0<<16)|(cg0<<8)|(cb0<<0);
		px1=(ca1<<24)|(cr1<<16)|(cg1<<8)|(cb1<<0);
		px2=(ca2<<24)|(cr2<<16)|(cg2<<8)|(cb2<<0);
		px3=(ca3<<24)|(cr3<<16)|(cg3<<8)|(cb3<<0);

#ifdef TKSVC_UNALACC_LE
		if(!((cm0|cm1|cm2)&(~255)))
		{
			((u32 *)(ibuf+dpx0))[0]=px0;
			((u32 *)(ibuf+dpx0))[1]=px1;
			((u32 *)(ibuf+dpx2))[0]=px2;
			((u32 *)(ibuf+dpx2))[1]=px3;
			continue;
		}
#endif

//		if((cm0|cm1|cm2)&(~255))
		if(1)
		{
			if(cm0&(~255))
			{
				cr0=TKSVC0D_Clamp255(cr0);
				cr1=TKSVC0D_Clamp255(cr1);
				cr2=TKSVC0D_Clamp255(cr2);
				cr3=TKSVC0D_Clamp255(cr3);
			}

			if(cm1&(~255))
			{
				cg0=TKSVC0D_Clamp255(cg0);
				cg1=TKSVC0D_Clamp255(cg1);
				cg2=TKSVC0D_Clamp255(cg2);
				cg3=TKSVC0D_Clamp255(cg3);
			}

			if(cm2&(~255))
			{
				cb0=TKSVC0D_Clamp255(cb0);
				cb1=TKSVC0D_Clamp255(cb1);
				cb2=TKSVC0D_Clamp255(cb2);
				cb3=TKSVC0D_Clamp255(cb3);
			}
		}

		ibuf[dpx0+0]=cb0;	ibuf[dpx0+4]=cb1;
		ibuf[dpx2+0]=cb2;	ibuf[dpx2+4]=cb3;
		ibuf[dpx0+1]=cg0;	ibuf[dpx0+5]=cg1;
		ibuf[dpx2+1]=cg2;	ibuf[dpx2+5]=cg3;
		ibuf[dpx0+2]=cr0;	ibuf[dpx0+6]=cr1;
		ibuf[dpx2+2]=cr2;	ibuf[dpx2+6]=cr3;
		ibuf[dpx0+3]=ca0;	ibuf[dpx0+7]=ca1;
		ibuf[dpx2+3]=ca2;	ibuf[dpx2+7]=ca3;
	}

	return(0);
}

TKSVC0D_DecState *TKSVC0D_CreateContext(void)
{
	TKSVC0D_DecState *ctx;
	
	ctx=malloc(sizeof(TKSVC0D_DecState));
	memset(ctx, 0, sizeof(TKSVC0D_DecState));
	return(ctx);
}
