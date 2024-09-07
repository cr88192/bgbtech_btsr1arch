/*
BGB Extensions:
APP11: BGBTech Tag
	FF,APP11,xx,xx,<ASCIZ TagName>, ...
	Tag-specific data until next marker.
	
	"AlphaColor":
		AlphaColor
		RGBA as string ("red green blue alpha").
		
		Specifies a "Alpha Color" extension, where any color sufficiently
		close to the given color (determined by 'alpha'), will be assumed
		to be a transparent pixel. These values are given in decimal.

	APP11 markers may indicate component layer:
		FF,APP11,xx,xx,"CompLayer\0", <layername:ASCIZ>
			"RGB": Base RGB
			"XYZ": Normal XYZ
			"SpRGB": Specular RGB
			"DASe": Depth, Alpha, Specular-Exponent
			"LuRGB": Luma RGB
			"Alpha": Mono alpha layer

	Component Layouts:
		3 component: (no marker, RGB)
		4 component: RGB+Alpha
		7 component: RGB+Alpha+LuRGB
		8 component: RGB+XYZ+DASe
		12 component: RGB+XYZ+SpRGB+DASe
		16 component: RGB+XYZ+SpRGB+DASe+LuRGB

	The APP11 tag for a layer will be followed by the layer
	(as a JPEG image), which may be placed within LDAT tags.
	
	Images lacking DHT and DQT will reuse those from the base image.

APP12: BGBTech Tag 2
	FF,APP12,xx,xx,tt,tt,tt,tt
		tt=FOURCC Tag.

	'LDAT': Layer Data.
	Gives an image/layer spread across a span of 'LDAT' tags.
	The encoded image will be split linearly across the tags.
*/

#if 0
#define DCTSZ	8
#define DCTSZ2	64

#define JPG_SOF0	0xC0
#define JPG_SOF1	0xC1
#define JPG_SOF2	0xC2
#define JPG_SOF3	0xC3
#define JPG_DHT		0xC4
#define JPG_SOF5	0xC5
#define JPG_SOF6	0xC6
#define JPG_SOF7	0xC7
#define JPG_JPG		0xC8
#define JPG_SOF9	0xC9
#define JPG_SOF10	0xCA
#define JPG_SOF11	0xCB
#define JPG_DAC		0xCC
#define JPG_SOF13	0xCD
#define JPG_SOF14	0xCE
#define JPG_SOF15	0xCF

#define JPG_RST0	0xD0
#define JPG_RST1	0xD1
#define JPG_RST2	0xD2
#define JPG_RST3	0xD3
#define JPG_RST4	0xD4
#define JPG_RST5	0xD5
#define JPG_RST6	0xD6
#define JPG_RST7	0xD7

#define JPG_SOI		0xD8
#define JPG_EOI		0xD9
#define JPG_SOS		0xDA
#define JPG_DQT		0xDB
#define JPG_DNL		0xDC
#define JPG_DRI		0xDD
#define JPG_DHP		0xDE
#define JPG_EXP		0xDF

#define JPG_APP0	0xE0
#define JPG_APP1	0xE1
#define JPG_APP2	0xE2
#define JPG_APP3	0xE3
#define JPG_APP4	0xE4
#define JPG_APP5	0xE5
#define JPG_APP6	0xE6
#define JPG_APP7	0xE7
#define JPG_APP8	0xE8
#define JPG_APP9	0xE9
#define JPG_APP10	0xEA
#define JPG_APP11	0xEB
#define JPG_APP12	0xEC
#define JPG_APP13	0xED
#define JPG_APP14	0xEE
#define JPG_APP15	0xEF

#define JPG_JPG0	0xF0
#define JPG_JPG1	0xF1
#define JPG_JPG2	0xF2
#define JPG_JPG3	0xF3
#define JPG_JPG4	0xF4
#define JPG_JPG5	0xF5
#define JPG_JPG6	0xF6
#define JPG_JPG7	0xF7
#define JPG_JPG8	0xF8
#define JPG_JPG9	0xF9
#define JPG_JPG10	0xFA
#define JPG_JPG11	0xFB
#define JPG_JPG12	0xFC
#define JPG_JPG13	0xFD
#define JPG_COM		0xFE

static const char *pdjpg_marker[]={
"SOF0", "SOF1", "SOF2", "SOF3", "DHT", "SOF5", "SOF6", "SOF7",
"JPG", "SOF9", "SOF10", "SOF11", "DAC", "SOF13", "SOF14", "SOF15",
"RST0", "RST1", "RST2", "RST3", "RST4", "RST5", "RST6", "RST7",
"SOI", "EOI", "SOS", "DQT", "DNL", "DRI", "DHP", "EXP",
"APP0", "APP1", "APP2", "APP3", "APP4", "APP5", "APP6", "APP7", 
"APP8", "APP9", "APP10", "APP11", "APP12", "APP13", "APP14", "APP15", 
"JPG0", "JPG1", "JPG2", "JPG3", "JPG4", "JPG5", "JPG6", "JPG7", 
"JPG8", "JPG9", "JPG10", "JPG11", "JPG12", "JPG13", "COM", ""
};

static const int pdjpg_zigzag[64]={
 0,  1,  5,  6, 14, 15, 27, 28,
 2,  4,  7, 13, 16, 26, 29, 42,
 3,  8, 12, 17, 25, 30, 41, 43,
 9, 11, 18, 24, 31, 40, 44, 53,
10, 19, 23, 32, 39, 45, 52, 54,
20, 22, 33, 38, 46, 51, 55, 60,
21, 34, 37, 47, 50, 56, 59, 61,
35, 36, 48, 49, 57, 58, 62, 63
};

static const int pdjpg_zigzag2[64]={
 0,  1,  8, 16,  9,  2,  3, 10,
17, 24, 32, 25, 18, 11,  4,  5,
12, 19, 26, 33, 40, 48, 41, 34,
27, 20, 13,  6,  7, 14, 21, 28,
35, 42, 49, 56, 57, 50, 43, 36,
29, 22, 15, 23, 30, 37, 44, 51,
58, 59, 52, 45, 38, 31, 39, 46,
53, 60, 61, 54, 47, 55, 62, 63
};
#endif


PDJPG_Context *PDJPG_AllocContext();
void PDJPG_FreeContext(PDJPG_Context *ctx);

#if 1
void PDJPG_TransDCT_Horiz(byte *iblk, int *oblk)
{
	int ib[8];

	ib[0]=iblk[0]-128;	ib[1]=iblk[1]-128;
	ib[2]=iblk[2]-128;	ib[3]=iblk[3]-128;
	ib[4]=iblk[4]-128;	ib[5]=iblk[5]-128;
	ib[6]=iblk[6]-128;	ib[7]=iblk[7]-128;

	oblk[0]=ib[0]*91  +ib[1]*91  +ib[2]*91  +ib[3]*91  
			+ib[4]*91  +ib[5]*91  +ib[6]*91  +ib[7]*91;
	oblk[1]=ib[0]*126 +ib[1]*106 +ib[2]*71  +ib[3]*25  
			-ib[4]*25  -ib[5]*71  -ib[6]*106 -ib[7]*126;
	oblk[2]=ib[0]*118 +ib[1]*49  -ib[2]*49  -ib[3]*118 
			-ib[4]*118 -ib[5]*49  +ib[6]*49  +ib[7]*118;
	oblk[3]=ib[0]*106 -ib[1]*25  -ib[2]*126 -ib[3]*71  
			+ib[4]*71  +ib[5]*126 +ib[6]*25  -ib[7]*106;
	oblk[4]=ib[0]*91  -ib[1]*91  -ib[2]*91  +ib[3]*91  
			+ib[4]*91  -ib[5]*91  -ib[6]*91  +ib[7]*91;
	oblk[5]=ib[0]*71  -ib[1]*126 +ib[2]*25  +ib[3]*106 
			-ib[4]*106 -ib[5]*25  +ib[6]*126 -ib[7]*71;
	oblk[6]=ib[0]*49  -ib[1]*118 +ib[2]*118 -ib[3]*49  
			-ib[4]*49  +ib[5]*118 -ib[6]*118 +ib[7]*49;
	oblk[7]=ib[0]*25  -ib[1]*71  +ib[2]*106 -ib[3]*126 
			+ib[4]*126 -ib[5]*106 +ib[6]*71  -ib[7]*25;
}

void PDJPG_TransDCT_Vert(int *iblk, int *oblk)
{
	oblk[ 0]=iblk[0]*91  +iblk[8]*91  
			+iblk[16]*91  +iblk[24]*91  
			+iblk[32]*91  +iblk[40]*91  
			+iblk[48]*91  +iblk[56]*91;
	oblk[ 8]=iblk[0]*126 +iblk[8]*106 
			+iblk[16]*71  +iblk[24]*25  
			-iblk[32]*25  -iblk[40]*71  
			-iblk[48]*106 -iblk[56]*126;
	oblk[16]=iblk[0]*118 +iblk[8]*49  
			-iblk[16]*49  -iblk[24]*118 
			-iblk[32]*118 -iblk[40]*49  
			+iblk[48]*49  +iblk[56]*118;
	oblk[24]=iblk[0]*106 -iblk[8]*25  
			-iblk[16]*126 -iblk[24]*71  
			+iblk[32]*71  +iblk[40]*126 
			+iblk[48]*25  -iblk[56]*106;
	oblk[32]=iblk[0]*91  -iblk[8]*91  
			-iblk[16]*91  +iblk[24]*91  
			+iblk[32]*91  -iblk[40]*91  
			-iblk[48]*91  +iblk[56]*91;
	oblk[40]=iblk[0]*71  -iblk[8]*126 
			+iblk[16]*25  +iblk[24]*106
			-iblk[32]*106 -iblk[40]*25  
			+iblk[48]*126 -iblk[56]*71;
	oblk[48]=iblk[0]*49  -iblk[8]*118
			+iblk[16]*118 -iblk[24]*49  
			-iblk[32]*49  +iblk[40]*118 
			-iblk[48]*118 +iblk[56]*49;
	oblk[56]=iblk[0]*25  -iblk[8]*71  
			+iblk[16]*106 -iblk[24]*126 
			+iblk[32]*126 -iblk[40]*106 
			+iblk[48]*71  -iblk[56]*25;
}

void PDJPG_TransDCT(byte *iblk, short *oblk)
{
	int s[DCTSZ2];
	int t[DCTSZ2];
	int i, j;

	PDJPG_TransDCT_Horiz(iblk+0, s+0);
	PDJPG_TransDCT_Horiz(iblk+8, s+8);
	PDJPG_TransDCT_Horiz(iblk+16, s+16);
	PDJPG_TransDCT_Horiz(iblk+24, s+24);
	PDJPG_TransDCT_Horiz(iblk+32, s+32);
	PDJPG_TransDCT_Horiz(iblk+40, s+40);
	PDJPG_TransDCT_Horiz(iblk+48, s+48);
	PDJPG_TransDCT_Horiz(iblk+56, s+56);

	PDJPG_TransDCT_Vert(s+0, t+0);
	PDJPG_TransDCT_Vert(s+1, t+1);
	PDJPG_TransDCT_Vert(s+2, t+2);
	PDJPG_TransDCT_Vert(s+3, t+3);
	PDJPG_TransDCT_Vert(s+4, t+4);
	PDJPG_TransDCT_Vert(s+5, t+5);
	PDJPG_TransDCT_Vert(s+6, t+6);
	PDJPG_TransDCT_Vert(s+7, t+7);

//	for(i=0; i<64; i++)
//		oblk[i]=t[i]>>16;

	for(i=0; i<64; i++)
		oblk[i]=(t[i]+32767)>>16;
}

#endif


int pdjpg_abslog2up(int v)
{
	int v1, i;

	v1=(v<<1)^(v>>31);
	i=0;
	while(v1>1)
	{
		i++;
		v1=(v1+1)>>1;
	}
	return(i);
}

void PDJHUFF_QuantBlock(
	PDJPG_Context *ctx, short *ibuf, short *obuf, int qid)
{
	int q0, q1, q2, q, lq, qb;
	int l0, l1;
	int i, j;
	
	if((ctx->qfl&127)>=95)
	{
		if((ctx->qfl&127)>=100)
		{
			for(i=0; i<64; i++)
				obuf[i]=ibuf[i];
			return;
		}

		for(i=0; i<64; i++)
			obuf[i]=(ibuf[i]*ctx->jpg_qtfp[qid][i]+2047)>>12;
		return;
	}
	
//	for(i=0; i<64; i++)
//		obuf[i]=ibuf[i]/ctx->jpg_qt[qid][i];

//	for(i=0; i<64; i++)
//		obuf[i]=(ibuf[i]*ctx->jpg_qtfp[qid][i]+2047)>>12;

#if 1
	qb=1; lq=0;
	for(i=0; i<64; i++)
	{
		q0=(ibuf[i]*ctx->jpg_qtfp[qid][i])>>12;
		q1=(ibuf[i]*ctx->jpg_qtfp[qid][i]+2047)>>12;

		q=q1;
		
		l0=pdjpg_abslog2up(q0);
		l1=pdjpg_abslog2up(q1);
		if(l0<l1)q=q0;
		
		if(!((q<(-1)) || (q>1)))
		{
			if(((i+1)<64))
			{
				q0=(ibuf[i]*ctx->jpg_qtfp[qid][i+1])>>12;
				q1=(ibuf[i]*ctx->jpg_qtfp[qid][i+1]+2047)>>12;

				q2=q1;
				l0=pdjpg_abslog2up(q0);
				l1=pdjpg_abslog2up(q1);
				if(l0<l1)q2=q0;
				if(!((q2<(-1)) || (q2>1)))
					q2=0;
			}else q2=0;

			if(!lq || !q2)
				q=0;
		}else
		{
//			if(i>0)
//				qb=0;

			if(i>0)
				qb=2;
			if(i>=16)
				qb=3;
			if(i>=32)
				qb=0;

		}
		
		lq=q;
		obuf[i]=q;
	}

	if(qb==1)
//	if(0)
	{
		for(i=1; i<64; i++)
			{ obuf[i]=0; }
	}else if(qb==2)
	{
		for(i=16; i<64; i++)
			{ obuf[i]=0; }
	}else if(qb==3)
	{
		for(i=32; i<64; i++)
			{ obuf[i]=0; }
	}
#endif
}

void PDJPG_SetupQuantTabDivFP(PDJPG_Context *ctx, int qid)
{
	int i;
	
	for(i=0; i<64; i++)
		ctx->jpg_qtfp[qid][i]=4096.0/ctx->jpg_qt[qid][i]+0.5;
}


void PDJHUFF_WriteBit(PDJPG_Context *ctx, int i)
{
	ctx->huff_win|=i<<(31-ctx->huff_pos);
	ctx->huff_pos++;
	if(ctx->huff_pos>=8)
	{
		i=(ctx->huff_win>>24)&0xFF;
		*ctx->huff_ct++=i;
		if(i==0xFF)*ctx->huff_ct++=0x00;
		ctx->huff_win<<=8;
		ctx->huff_pos-=8;
	}
}

void PDJHUFF_WriteNBits(PDJPG_Context *ctx, int v, int n)
{
	int i, j, k;

	v&=(1<<n)-1;

	j=ctx->huff_pos+n;
	k=ctx->huff_win|(v<<(32-j));
	while(j>=8)
	{
		i=(k>>24)&0xFF;
		*ctx->huff_ct++=i;
		if(i==0xFF)*ctx->huff_ct++=0x00;
		k<<=8;
		j-=8;
	}
	ctx->huff_pos=j;
	ctx->huff_win=k;
}

void PDJHUFF_FlushBits(PDJPG_Context *ctx)
{
	int i;
	while(ctx->huff_pos>0)
	{
		i=(ctx->huff_win>>24)&0xFF;
		*ctx->huff_ct++=i;
		if(i==0xFF)*ctx->huff_ct++=0x00;
		ctx->huff_win<<=8;
		ctx->huff_pos-=8;
	}
}

void PDJHUFF_WriteString(PDJPG_Context *ctx, char *str)
{
	char *s;

	PDJHUFF_FlushBits(ctx);	
	s=str;
	while(*s)
		*ctx->huff_ct++=*s++;
	*ctx->huff_ct++=0;
}

int PDJHUFF_BalanceTree_r(
	short *nodes, short *nlen, int root, int h, int ml)
{
	int h0, h1, h2, h3;
	int l0, l1, l2;

	if(root<0)return(0);

	h1=PDJHUFF_BalanceTree_r(nodes, nlen, nodes[root*2+0], h+1, ml);
	h2=PDJHUFF_BalanceTree_r(nodes, nlen, nodes[root*2+1], h+1, ml);
	h0=((h1>h2)?h1:h2)+1;
	nlen[root]=h0;

	if((h+h0)<=ml)	//depth limit not exceeded
		return(h0);

	//ok, so part of the tree is too deep
	if((h1+1)<h2)
	{
		l0=nodes[root*2+1];
		l1=nodes[l0*2+1];
		nodes[l0*2+1]=nodes[l0*2+0];
		nodes[l0*2+0]=nodes[root*2+0];
		nodes[root*2+0]=l0;
		nodes[root*2+1]=l1;
	}else if((h2+1)<h1)
	{
		l0=nodes[root*2+0];
		l1=nodes[l0*2+0];
		nodes[l0*2+0]=nodes[l0*2+1];
		nodes[l0*2+1]=nodes[root*2+1];
		nodes[root*2+0]=l1;
		nodes[root*2+1]=l0;
	}else
	{
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

	return(h0);
}

void PDJHUFF_CalcLengths_r(short *nodes, byte *cl, int root, int h)
{
	if(root<0)
	{
		if(root==-513)return;
		cl[(-root)-1]=h;
		return;
	}

	PDJHUFF_CalcLengths_r(nodes, cl, nodes[root*2+0], h+1);
	PDJHUFF_CalcLengths_r(nodes, cl, nodes[root*2+1], h+1);
}

int PDJHUFF_BuildLengths(int *stat, int nc, byte *cl, int ml)
{
	short nodes[1024], nlen[512];
	short roots[512], clen[512];
	int cnts[512];
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

	roots[nr]=-513;
	cnts[nr]=0;
	clen[nr]=0;
	nr++;


	for(i=0; i<nc; i++)cl[i]=0;
	if(!nr)return(-1);


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
		k=PDJHUFF_BalanceTree_r(nodes, nlen, l, 0, ml);
	if(k>ml)return(-2);	//failed to balance tree

	PDJHUFF_CalcLengths_r(nodes, cl, l, 0);
	return(0);
}

void PDJHUFF_BuildLengthsAdjust(int *stat, int nc, byte *cl, int ml)
{
	int i, j;

	while(1)
	{
		j=PDJHUFF_BuildLengths(stat, nc, cl, ml);
		if(j<0)
			printf("PDJHUFF_BuildLengthsAdjust: Huff Fail %d\n");

		for(i=0; i<nc; i++)
			if(stat[i] && !cl[i])
				break;
		if(i>=nc)break;

		printf("PDJHUFF_BuildLengthsAdjust: Fiddle Adjust\n");
		for(i=0; i<nc; i++)
			stat[i]++;
		continue;
	}
}

void PDJHUFF_EncodeSymbol(PDJPG_Context *ctx, int tab, int v)
{
	tab<<=8;
	PDJHUFF_WriteNBits(ctx, ctx->huff_code[tab|v], ctx->huff_len[tab|v]);
}

void PDJHUFF_EncodeVal(PDJPG_Context *ctx, int tab, int z, int v)
{
	int i, j, k;

	if(!v) { PDJHUFF_EncodeSymbol(ctx, tab, z<<4); return; }

	if(v>0)
	{
		i=1; while(v>=(1<<i))i++;
		PDJHUFF_EncodeSymbol(ctx, tab, (z<<4)|i);
		PDJHUFF_WriteNBits(ctx, v, i);
		return;
	}

	i=1; j=-v; while(j>=(1<<i))i++;
	PDJHUFF_EncodeSymbol(ctx, tab, (z<<4)|i);

	k=(1<<i)-(j+1);
	PDJHUFF_WriteNBits(ctx, k, i);
	return;
}

void PDJHUFF_StatVal(int *stat, int z, int v)
{
	int i, j, k;

	if(!v) { stat[z<<4]++; return; }

	if(v>0)
	{
		i=1; while(v>=(1<<i))i++;
		stat[(z<<4)|i]++;
		return;
	}

	i=1; j=-v; while(j>=(1<<i))i++;
	stat[(z<<4)|i]++;
}

#if 1
void PDJHUFF_EncodeBlock(PDJPG_Context *ctx,
	short *buf, int dctab, int actab)
{
	int i, j, k;

	PDJHUFF_EncodeVal(ctx, dctab, 0, buf[0]);

	for(i=1; i<64; i++)
	{
		if(buf[pdjpg_zigzag2[i]])
		{
			PDJHUFF_EncodeVal(ctx, actab, 0, buf[pdjpg_zigzag2[i]]);
			continue;
		}
		for(j=i; j<64; j++)
			if(buf[pdjpg_zigzag2[j]])break;
		if(j>=64)
		{
			PDJHUFF_EncodeSymbol(ctx, actab, 0);
			break;
		}

		j-=i; if(j>15)j=15;
		PDJHUFF_EncodeVal(ctx, actab, j, buf[pdjpg_zigzag2[i+j]]);
		i+=j;
	}
}

void PDJHUFF_StatBlock(short *buf, int *dcstat, int *acstat)
{
	int i, j, k;

	PDJHUFF_StatVal(dcstat, 0, buf[0]);
	for(i=1; i<64; i++)
	{
		if(buf[pdjpg_zigzag2[i]])
		{
			PDJHUFF_StatVal(acstat, 0, buf[pdjpg_zigzag2[i]]);
			continue;
		}
		for(j=i; j<64; j++)
			if(buf[pdjpg_zigzag2[j]])break;
		if(j>=64)
		{
			acstat[0]++;
			break;
		}

		j-=i; if(j>15)j=15;
		PDJHUFF_StatVal(acstat, j, buf[pdjpg_zigzag2[i+j]]);
		i+=j;
	}
}
#endif

#if defined(X86) || defined(X86_64)
#define memcpy8(dst, src)	(*(u64 *)(dst)=*(u64 *)(src))
#else
#define memcpy8(dst, src)	memcpy(dst, src, 8)
#endif

#if 1
void PDJPG_GetImgBlk(byte *blk, int xo, int yo, byte *img, int xs, int ys)
{
	byte *cs;
	byte *ct;
	int i, j, k;

	cs=img+(yo*xs+xo); ct=blk;
	memcpy8(ct, cs); ct+=8; cs+=xs;
	memcpy8(ct, cs); ct+=8; cs+=xs;
	memcpy8(ct, cs); ct+=8; cs+=xs;
	memcpy8(ct, cs); ct+=8; cs+=xs;
	memcpy8(ct, cs); ct+=8; cs+=xs;
	memcpy8(ct, cs); ct+=8; cs+=xs;
	memcpy8(ct, cs); ct+=8; cs+=xs;
	memcpy8(ct, cs); // ct+=8; cs+=xs;
}
#endif

void PDJPG_FilterImageDCT(byte *ibuf, short *obuf, int xs, int ys)
{
	short tblk[DCTSZ2], tblk2[DCTSZ2];
	byte blk[DCTSZ2];
	int i, j, k, l;

	k=0;
	for(i=0; i<(ys/DCTSZ); i++)
		for(j=0; j<(xs/DCTSZ); j++)
	{
		PDJPG_GetImgBlk(blk, j*DCTSZ, i*DCTSZ, ibuf, xs, ys);
		PDJPG_TransDCT(blk, obuf+k*DCTSZ2);
		k++;
	}
}

void PDJPG_EmitDQT(PDJPG_Context *ctx, int n)
{
	int i;

	*ctx->huff_ct++=0xFF;
	*ctx->huff_ct++=JPG_DQT;

	i=64+3;
	*ctx->huff_ct++=i>>8;
	*ctx->huff_ct++=i&0xFF;

	*ctx->huff_ct++=n;
	for(i=0; i<64; i++)
		*ctx->huff_ct++=ctx->jpg_qt[n][pdjpg_zigzag2[i]];
}

void PDJPG_EmitSOF(PDJPG_Context *ctx, int xs, int ys)
{
	int i;

	*ctx->huff_ct++=0xFF;
	*ctx->huff_ct++=JPG_SOF0;

	i=8+(ctx->jpg_mono?1:3)*3;
	*ctx->huff_ct++=i>>8;	//Lf
	*ctx->huff_ct++=i&0xFF;

	*ctx->huff_ct++=8; 	//P

	*ctx->huff_ct++=ys>>8;	//Y
	*ctx->huff_ct++=ys&0xFF;	//Y
	*ctx->huff_ct++=xs>>8;	//X
	*ctx->huff_ct++=xs&0xFF;	//X

	if(ctx->jpg_mono)
	{
		*ctx->huff_ct++=1;	//Nf

		*ctx->huff_ct++=1;	//Ci
		*ctx->huff_ct++=0x11;	//Hi Vi
		*ctx->huff_ct++=0;	//Tqi
	}else
	{
		*ctx->huff_ct++=3;	//Nf

		*ctx->huff_ct++=1;	//Ci
		*ctx->huff_ct++=0x22;	//Hi Vi
		*ctx->huff_ct++=0;	//Tqi
		*ctx->huff_ct++=2;	//Ci
		*ctx->huff_ct++=0x11;	//Hi Vi
		*ctx->huff_ct++=1;	//Tqi
		*ctx->huff_ct++=3;	//Ci
		*ctx->huff_ct++=0x11;	//Hi Vi
		*ctx->huff_ct++=1;	//Tqi
	}
}

void PDJPG_EmitSOS(PDJPG_Context *ctx)
{
	int i;

	*ctx->huff_ct++=0xFF;
	*ctx->huff_ct++=JPG_SOS;

	i=6+(ctx->jpg_mono?1:3)*2;
	*ctx->huff_ct++=i>>8;	//Lf
	*ctx->huff_ct++=i&0xFF;

	if(!ctx->jpg_mono)
	{
		*ctx->huff_ct++=3; 	//Ns

		*ctx->huff_ct++=1;	//Csi
		*ctx->huff_ct++=0x00;	//Tdi Tai
		*ctx->huff_ct++=2;	//Csi
		*ctx->huff_ct++=0x11;	//Tdi Tai
		*ctx->huff_ct++=3;	//Csi
		*ctx->huff_ct++=0x11;	//Tdi Tai
	}else
	{
		*ctx->huff_ct++=1; 	//Ns

		*ctx->huff_ct++=1;	//Csi
		*ctx->huff_ct++=0x00;	//Tdi Tai
	}

	*ctx->huff_ct++=0; 	//Ss
	*ctx->huff_ct++=63; 	//Se
	*ctx->huff_ct++=0x00; 	//Ah Al
}

void PDJPG_EmitDHT(PDJPG_Context *ctx, int tab)
{
	byte *p;
	int i, j, k;

	*ctx->huff_ct++=0xFF;
	*ctx->huff_ct++=JPG_DHT;

//	i=8+3*1;
	i=0;
	p=ctx->huff_ct;
	*ctx->huff_ct++=i>>8;	//Lf
	*ctx->huff_ct++=i&0xFF;

	i=(tab/2)|((tab&1)<<4);
	*ctx->huff_ct++=i; 	//Tc Th

	tab<<=8;
	for(i=1; i<=16; i++)
	{
		k=0;
		for(j=0; j<256; j++)
			if(ctx->huff_len[tab|j]==i)
				k++;
		*ctx->huff_ct++=k; 	//Li
	}

	k=0;
	for(i=1; i<=16; i++)
	{
		k<<=1;
		for(j=0; j<256; j++)
			if(ctx->huff_len[tab|j]==i)
		{
			*ctx->huff_ct++=j; 	//Vi
			ctx->huff_code[tab|j]=k++;
		}
	}

	if(k>=65536)
		printf("PDJPG_EmitDHT: Error %04X\n", k);

//	printf("DHT %04X\n", k);

	i=ctx->huff_ct-p;
	p[0]=i>>8;	//Lf
	p[1]=i&0xFF;
}

static int pdjpg_ijg_qtab_y[64] = {
16, 11, 10, 16,  24,  40,  51,  61,
12, 12, 14, 19,  26,  58,  60,  55,
14, 13, 16, 24,  40,  57,  69,  56,
14, 17, 22, 29,  51,  87,  80,  62,
18, 22, 37, 56,  68, 109, 103,  77,
24, 35, 55, 64,  81, 104, 113,  92,
49, 64, 78, 87, 103, 121, 120, 101,
72, 92, 95, 98, 112, 100, 103,  99};

static int pdjpg_ijg_qtab_uv[64] = {
99, 99, 99, 99, 99, 99, 99, 99,
99, 99, 99, 99, 99, 99, 99, 99,
99, 99, 99, 99, 99, 99, 99, 99,
99, 99, 99, 99, 99, 99, 99, 99,
47, 66, 99, 99, 99, 99, 99, 99,
24, 26, 56, 99, 99, 99, 99, 99,
18, 21, 26, 66, 99, 99, 99, 99,
17, 18, 24, 47, 99, 99, 99, 99};

void PDJPG_MakeQuantTabFastIJG_Y(
	short *inv, int cnt, byte *tab, float qf)
{
	double s, q;
	int i, j;

	q = (qf * 100);
	s = (q < 50) ? 5000 / q : (200 - 2 * q);
	for (i = 0; i < 64; i++)
	{
		j = (int)((s * pdjpg_ijg_qtab_y[i] + 50) / 100);
		j = (j < 1) ? 1 : ((j < 256) ? j : 255);
		tab[i] = j;
	}
}

void PDJPG_MakeQuantTabFastIJG_UV(
	short *inv, int cnt, byte *tab, float qf)
{
	double s, q;
	int i, j;

	q = (qf * 100);
	s = (q < 50) ? 5000 / q : (200 - 2 * q);
	for (i = 0; i < 64; i++)
	{
		j = (int)((s * pdjpg_ijg_qtab_uv[i] + 50) / 100);
		j = (j < 1) ? 1 : ((j < 256) ? j : 255);
		tab[i] = j;
	}
}

void PDJPG_MakeQuantTabInput(short *inv, int cnt, byte *tab, float q)
{
	PDJPG_MakeQuantTabInput_AA(inv, cnt, tab, q);
}

void PDJPG_MakeQuantTabInputY(short *inv, int cnt, byte *tab, float q)
{
	byte taba[64], tabb[64];
	int i;

	PDJPG_MakeQuantTabFastIJG_Y(inv, cnt, tab, q);

//	PDJPG_MakeQuantTabInput_AA(inv, cnt, taba, q);
//	PDJPG_MakeQuantTabFastIJG_Y(inv, cnt, tabb, q);
//	for (i = 0; i < 64; i++)
//		tab[i] = (taba[i] + tabb[i]) / 2;
}

void PDJPG_MakeQuantTabInputUV(short *inv, int cnt, byte *tab, float q)
{
	byte taba[64], tabb[64];
	int i;

//	PDJPG_MakeQuantTabFastIJG_UV(inv, cnt, tab, q);
	PDJPG_MakeQuantTabFastIJG_Y(inv, cnt, tab, q);

//	PDJPG_MakeQuantTabInput_AA(inv, cnt, taba, q);
//	PDJPG_MakeQuantTabFastIJG_UV(inv, cnt, tabb, q);
//	for (i = 0; i < 64; i++)
//		tab[i] = (taba[i] + tabb[i]) / 2;
}

void PDJPG_MakeQuantTabInput_AA(
	short *in, int cnt, byte *tab, float q)
{
	double deltas[DCTSZ2];
	double f, g, te;
	int i, j, k;

	for(j=0; j<DCTSZ2; j++)
		deltas[j]=0;

	for(i=0; i<cnt; i++)
		for(j=0; j<DCTSZ2; j++)
			deltas[j]+=fabs((double)(in[i*DCTSZ2+j]));
	for(j=0; j<DCTSZ2; j++)
		deltas[j]/=cnt;

	te=0;
	for(j=0; j<DCTSZ2; j++)
		te+=deltas[j];

	for(i=0; i<DCTSZ; i++)
	{
		for(j=0; j<DCTSZ; j++)
		{
			f=deltas[i*DCTSZ+j];

			f=f/te;
			if(f<0.00001)f=0.00001;
			g=(1.0-q);
			f=10*g*g*g/f;
			k=f;
			if(k<1)k=1;
			if(k>255)k=255;
			tab[i*DCTSZ+j]=k;
		}
	}

#if 1
	if(tab[DCTSZ2-3]>1)tab[DCTSZ2-3]*=0.75;
	if(tab[DCTSZ2-2]>1)tab[DCTSZ2-2]*=0.75;
	if(tab[DCTSZ2-1]>1)tab[DCTSZ2-1]*=0.5;
#endif
}

void PDJPG_MakeQuantTabInputFast(
	short *in, int cnt, byte *tab, float q)
{
	int i;
//	PDJPG_MakeQuantTabInput(in, cnt, tab, q);

	i=cnt/16;
	if(i<1)i=1;
	PDJPG_MakeQuantTabInput_AA(in, i, tab, q);
}

int PDJPG_EncodeCtx(
	PDJPG_Context *ctx, byte *ibuf, byte *obuf, int xs, int ys,
	int qf, int pf)
{
	int dcs[256], acs[256];
	int dcsuv[256], acsuv[256];
	short *tp;
	int xs2, ys2, xs3, ys3;
	int cr, cg, cb, cy, cu, cv;
	int i, j, k, l;

	if(ctx->jpg_mono)
	{
		xs2=((xs+7)/8)*8;
		ys2=((ys+7)/8)*8;
	}else
	{
		xs2=((xs+15)/16)*16;
		ys2=((ys+15)/16)*16;
	}

	xs3=((xs+15)/16)*8;
	ys3=((ys+15)/16)*8;

	ctx->jpg_mono=0;
//	ctx->jpg_tabcacheframe=0;
	ctx->xs=xs;
	ctx->ys=ys;
	ctx->qfl=qf;

	if(pf==BTIC1H_PXF_YYYA)
		ctx->jpg_mono=1;

	//full quality
	for(i=0; i<64; i++)ctx->jpg_qt[0][i]=1;
	for(i=0; i<64; i++)ctx->jpg_qt[1][i]=1;

	//dummy huffman tables
	for(i=0; i<256; i++)ctx->huff_len[0*256+i]=0;
	for(i=0; i<256; i++)ctx->huff_len[1*256+i]=0;
	for(i=0; i<256; i++)ctx->huff_len[2*256+i]=0;
	for(i=0; i<256; i++)ctx->huff_len[3*256+i]=0;

	for(i=0; i<16; i++)ctx->huff_len[0*256+i]=4;
	for(i=0; i<16; i++)ctx->huff_len[2*256+i]=4;
	for(i=0; i<255; i++)ctx->huff_len[1*256+i]=8;
	for(i=0; i<255; i++)ctx->huff_len[3*256+i]=8;

//	for(i=0; i<255; i++)ctx->huff_len[0*256+i]=8;
//	for(i=0; i<255; i++)ctx->huff_len[2*256+i]=8;


//	ctx->huff_len[1*256+0]=7;

//	printf("M0\n");

	if(!ctx->yb || (xs!=ctx->lxs) || (ys!=ctx->lys))
	{
		if(ctx->yb)
		{
			free(ctx->yb);
			free(ctx->ub);
			free(ctx->vb);
			free(ctx->ydb);
			free(ctx->udb);
			free(ctx->vdb);
		}

		ctx->yb=malloc(xs2*ys2);
		ctx->ub=malloc(xs2*ys2);
		ctx->vb=malloc(xs2*ys2);

		ctx->ydb=malloc((xs2+8)*(ys2+8)*sizeof(short));
		ctx->udb=malloc((xs3+8)*(ys3+8)*sizeof(short));
		ctx->vdb=malloc((xs3+8)*(ys3+8)*sizeof(short));

		ctx->lxs=xs;
		ctx->lys=ys;
	}


	memset(ctx->yb, 128, xs2*ys2);
	memset(ctx->ub, 128, xs2*ys2);
	memset(ctx->vb, 128, xs2*ys2);

	memset(ctx->ydb, 0, xs2*(ys2+8)*sizeof(short));
	memset(ctx->udb, 0, xs3*(ys3+8)*sizeof(short));
	memset(ctx->vdb, 0, xs3*(ys3+8)*sizeof(short));

	for(i=0; i<ys; i++)
		for(j=0; j<xs; j++)
	{
		k=ys-(i+1);
		cr=ibuf[(k*xs+j)*4+0];
		cg=ibuf[(k*xs+j)*4+1];
		cb=ibuf[(k*xs+j)*4+2];

		if(ctx->jpg_mono)
		{
			cy=(2*cg+cb+cr)>>2;
//			cy=cg;
			cu=128; cv=128;
		}else
		{
			cy=0.299*cr	+0.587*cg	+0.114*cb;
			cu=-0.1687*cr	-0.3313*cg	+0.5*cb 	+128;
			cv=0.5*cr	-0.4187*cg	-0.0813*cb	+128;
		}

//		cu=128; cv=128;

		cy=(cy<0)?0:((cy>255)?255:cy);
		cu=(cu<0)?0:((cu>255)?255:cu);
		cv=(cv<0)?0:((cv>255)?255:cv);

		ctx->yb[i*xs2+j]=cy;
		ctx->ub[i*xs2+j]=cu;
		ctx->vb[i*xs2+j]=cv;
	}

	for(i=0; i<ys3; i++)
		for(j=0; j<xs3; j++)
	{
		k=ctx->ub[(i*2)*xs2+j*2]+
			ctx->ub[(i*2)*xs2+j*2+1]+
			ctx->ub[(i*2+1)*xs2+j*2]+
			ctx->ub[(i*2+1)*xs2+j*2+1];
		ctx->ub[i*xs3+j]=k/4;

		k=ctx->vb[(i*2)*xs2+j*2]+
			ctx->vb[(i*2)*xs2+j*2+1]+
			ctx->vb[(i*2+1)*xs2+j*2]+
			ctx->vb[(i*2+1)*xs2+j*2+1];
		ctx->vb[i*xs3+j]=k/4;
	}

//	printf("M1\n");


	PDJPG_FilterImageDCT(ctx->yb, ctx->ydb, xs2, ys2);
	PDJPG_FilterImageDCT(ctx->ub, ctx->udb, xs3, ys3);
	PDJPG_FilterImageDCT(ctx->vb, ctx->vdb, xs3, ys3);

	j=(xs2/8)*(ys2/8);
	PDJPG_MakeQuantTabInputY(ctx->ydb, j,
		ctx->jpg_qt[0], (qf&127)/100.0);

	j=(xs3/8)*(ys3/8);
	PDJPG_MakeQuantTabInputUV(ctx->udb, j,
		ctx->jpg_qt[1], (qf&127)/100.0);
	PDJPG_MakeQuantTabInputUV(ctx->vdb, j,
		ctx->jpg_qt[2], (qf&127)/100.0);
	for(i=0; i<64; i++)
		ctx->jpg_qt[1][i]=(ctx->jpg_qt[1][i]+ctx->jpg_qt[2][i])/2;

	PDJPG_SetupQuantTabDivFP(ctx, 0);
	PDJPG_SetupQuantTabDivFP(ctx, 1);

	if(ctx->jpg_mono)
	{
		j=(xs2/8)*(ys2/8); k=0;
		for(i=0; i<j; i++)
		{
			PDJHUFF_QuantBlock(ctx, ctx->ydb+i*64, ctx->ydb+i*64, 0);
			ctx->ydb[i*64+0]-=k; k=ctx->ydb[i*64+0]+k;
		}
	}

	if(!ctx->jpg_mono)
	{
		l=0;
		for(i=0; i<(ys3/8); i++)
			for(j=0; j<(xs3/8); j++)
		{
			tp=ctx->ydb+((i*2+0)*(xs2/8)+j*2+0)*64;
			PDJHUFF_QuantBlock(ctx, tp, tp, 0);
			tp[0]-=l; l=tp[0]+l;

			tp=ctx->ydb+((i*2+0)*(xs2/8)+j*2+1)*64;
			PDJHUFF_QuantBlock(ctx, tp, tp, 0);
			tp[0]-=l; l=tp[0]+l;

			tp=ctx->ydb+((i*2+1)*(xs2/8)+j*2+0)*64;
			PDJHUFF_QuantBlock(ctx, tp, tp, 0);
			tp[0]-=l; l=tp[0]+l;

			tp=ctx->ydb+((i*2+1)*(xs2/8)+j*2+1)*64;
			PDJHUFF_QuantBlock(ctx, tp, tp, 0);
			tp[0]-=l; l=tp[0]+l;
		}
	}

	j=(xs3/8)*(ys3/8); k=0; l=0;
	for(i=0; i<j; i++)
	{
		PDJHUFF_QuantBlock(ctx, ctx->udb+i*64, ctx->udb+i*64, 1);
		PDJHUFF_QuantBlock(ctx, ctx->vdb+i*64, ctx->vdb+i*64, 1);
		ctx->udb[i*64+0]-=k; k=ctx->udb[i*64+0]+k;
		ctx->vdb[i*64+0]-=l; l=ctx->vdb[i*64+0]+l;
	}

//	printf("M2\n");

	for(i=0; i<256; i++)dcs[i]=0;
	for(i=0; i<256; i++)acs[i]=0;
	for(i=0; i<256; i++)dcsuv[i]=0;
	for(i=0; i<256; i++)acsuv[i]=0;

	j=(xs2/8)*(ys2/8);
	k=(xs3/8)*(ys3/8);
	for(i=0; i<j; i++)PDJHUFF_StatBlock(ctx->ydb+i*64, dcs, acs);
	for(i=0; i<k; i++)PDJHUFF_StatBlock(ctx->udb+i*64, dcsuv, acsuv);
	for(i=0; i<k; i++)PDJHUFF_StatBlock(ctx->vdb+i*64, dcsuv, acsuv);

	PDJHUFF_BuildLengthsAdjust(dcs, 256, ctx->huff_len+0*256, 16);
	PDJHUFF_BuildLengthsAdjust(acs, 256, ctx->huff_len+1*256, 16);
	PDJHUFF_BuildLengthsAdjust(dcsuv, 256, ctx->huff_len+2*256, 16);
	PDJHUFF_BuildLengthsAdjust(acsuv, 256, ctx->huff_len+3*256, 16);

	ctx->huff_ct=obuf;
	ctx->huff_win=0;
	ctx->huff_pos=0;

	*ctx->huff_ct++=0xFF;
	*ctx->huff_ct++=JPG_SOI;

	PDJPG_EmitDQT(ctx, 0);
	if(!ctx->jpg_mono)PDJPG_EmitDQT(ctx, 1);

	PDJPG_EmitSOF(ctx, xs, ys);

	PDJPG_EmitDHT(ctx, 0);
	PDJPG_EmitDHT(ctx, 1);
	if(!ctx->jpg_mono)
	{
		PDJPG_EmitDHT(ctx, 2);
		PDJPG_EmitDHT(ctx, 3);
	}

	PDJPG_EmitSOS(ctx);

	ctx->huff_win=0;
	ctx->huff_pos=0;

	if(ctx->jpg_mono)
	{
		j=(xs2/8)*(ys2/8);
		for(i=0; i<j; i++)PDJHUFF_EncodeBlock(ctx, ctx->ydb+i*64, 0, 1);
	}else
	{
		for(i=0; i<(ys3/8); i++)
			for(j=0; j<(xs3/8); j++)
		{
			PDJHUFF_EncodeBlock(ctx,
				ctx->ydb+((i*2+0)*(xs2/8)+j*2+0)*64, 0, 1);
			PDJHUFF_EncodeBlock(ctx,
				ctx->ydb+((i*2+0)*(xs2/8)+j*2+1)*64, 0, 1);
			PDJHUFF_EncodeBlock(ctx,
				ctx->ydb+((i*2+1)*(xs2/8)+j*2+0)*64, 0, 1);
			PDJHUFF_EncodeBlock(ctx,
				ctx->ydb+((i*2+1)*(xs2/8)+j*2+1)*64, 0, 1);

			k=i*(xs3/8)+j;
			PDJHUFF_EncodeBlock(ctx, ctx->udb+k*64, 2, 3);
			PDJHUFF_EncodeBlock(ctx, ctx->vdb+k*64, 2, 3);
		}
	}

	PDJHUFF_FlushBits(ctx);

	*ctx->huff_ct++=0xFF;
	*ctx->huff_ct++=JPG_EOI;

	i=ctx->huff_ct-obuf;

	return(i);
}

#if 0
PDJPG_Context *PDJPG_AllocContext()
{
	PDJPG_Context *ctx;
	
	ctx=malloc(sizeof(PDJPG_Context));
	memset(ctx, 0, sizeof(PDJPG_Context));
	return(ctx);
}

void PDJPG_FreeContext(PDJPG_Context *ctx)
{
	if(ctx->yb)
	{
		free(ctx->yb);
		free(ctx->ub);
		free(ctx->vb);
		free(ctx->ydb);
		free(ctx->udb);
		free(ctx->vdb);
	}

	if(ctx->jpg_imgbuf)
		free(ctx->jpg_imgbuf);

	free(ctx);
}
#endif

int PDJPG_Encode(
	byte *ibuf, byte *obuf, int xs, int ys, int qf)
{
	PDJPG_Context *ctx;
	int sz;
	
	ctx=PDJPG_AllocContext();
	sz=PDJPG_EncodeCtx(ctx, ibuf, obuf, xs, ys, qf, 0);
	PDJPG_FreeContext(ctx);
	return(sz);
}

void PDJPG_FlushEncodeFast(PDJPG_Context *ctx)
{
	if(!ctx)return;
//	ctx->jpg_tabcacheframe=0;	//force rebuild
//	ctx->oldAlphaClr=0;
}

#if 0
// void PDJPG_SetContextAlphaColor(PDJPG_Context *ctx,
	int cr, int cg, int cb, int ca)
{
	int cy, cu, cv;
	if(!ctx)return;

	if(ca<1)
	{
		ctx->alphaClr=0;
		return;
	}

	ctx->alphaClr=cr+(cg<<8)+(cb<<16)+(ca<<24);

	cy= 19595*cr +38470*cg + 7471*cb;
	cu=-11056*cr -21712*cg +32768*cb;
	cv= 32768*cr -27440*cg - 5328*cb;
	ctx->alphaClrY=cy>>16;
	ctx->alphaClrU=(cu>>16)+128;
	ctx->alphaClrV=(cv>>16)+128;
	ctx->alphaClrA=ca;
}
#endif

void PDJPG_ConvertImageYUV(PDJPG_Context *ctx,
	byte *ibuf, int xs, int ys, int pf,
	int xs2, int ys2, int xs3, int ys3)
{
//	void (*getPixel2)(byte *rgb,
//		int *ra, int *ga, int *ba,
//		int *rb, int *gb, int *bb);
	byte *cs, *cse, *cty, *ctu, *ctv;
	byte *cs1, *cs2, *cty1, *cty2;

	int cr, cg, cb, ca, cy, cu, cv;

	int cra, cga, cba, caa, cya, cua, cva;
	int crb, cgb, cbb, cab, cyb, cub, cvb;
	int crc, cgc, cbc, cac, cyc, cuc, cvc;
	int crd, cgd, cbd, cad, cyd, cud, cvd;
	int psz, psz2;

	int i, j, k, l, n;

//	if((pf==BTIC1H_PXF_RGBA) && !ctx->alphaClr)
	if(pf==BTIC1H_PXF_RGBA)
	{
		n=(ys+1)/2;
		for(i=0; i<n; i++)
		{
			k=((ys-(2*i+1))*xs)*4;
			cs1=ibuf+k; cse=cs1+xs*4;
			l=((ys-(2*i+2))*xs)*4;
			cs2=ibuf+l;

			cty1=ctx->yb+(2*i+0)*xs2;
			cty2=ctx->yb+(2*i+1)*xs2;

			l=i*xs3;
			ctu=ctx->ub+l;
			ctv=ctx->vb+l;

			while(cs1<cse)
			{
				cra=*(cs1+0); cga=*(cs1+1); cba=*(cs1+2);
				crb=*(cs1+4); cgb=*(cs1+5); cbb=*(cs1+6);
				crc=*(cs2+0); cgc=*(cs2+1); cbc=*(cs2+2);
				crd=*(cs2+4); cgd=*(cs2+5); cbd=*(cs2+6);

				cya=19595*cra + 38470*cga + 7471*cba;
				cyb=19595*crb + 38470*cgb + 7471*cbb;
				cyc=19595*crc + 38470*cgc + 7471*cbc;
				cyd=19595*crd + 38470*cgd + 7471*cbd;
				*cty1++=cya>>16;
				*cty1++=cyb>>16;
				*cty2++=cyc>>16;
				*cty2++=cyd>>16;

				cr=(cra+crb+crc+crd)>>2;
				cg=(cga+cgb+cgc+cgd)>>2;
				cb=(cba+cbb+cbc+cbd)>>2;

				cu=-11056*cr -21712*cg +32768*cb;
				cv= 32768*cr -27440*cg - 5328*cb;
				*ctu++=(cu>>16)+128;
				*ctv++=(cv>>16)+128;

				cs1+=8; cs2+=8;
			}
		}
	}else if(
		(pf==BTIC1H_PXF_RGBA) || (pf==BTIC1H_PXF_RGB) ||
		(pf==BTIC1H_PXF_BGRA) || (pf==BTIC1H_PXF_BGR))
	{
		switch(pf)
		{
		case BTIC1H_PXF_RGBA: case BTIC1H_PXF_BGRA:
			psz=4; break;
		case BTIC1H_PXF_RGB: case BTIC1H_PXF_BGR:
			psz=3; break;
		default: psz=4; break;
		}
		psz2=psz*2;

		n=(ys+1)/2;
		for(i=0; i<n; i++)
		{
			k=((ys-(2*i+1))*xs)*psz;
			cs1=ibuf+k; cse=cs1+xs*psz;
			l=((ys-(2*i+2))*xs)*psz;
			cs2=ibuf+l;

			cty1=ctx->yb+(2*i+0)*xs2;
			cty2=ctx->yb+(2*i+1)*xs2;

			l=i*xs3;
			ctu=ctx->ub+l;
			ctv=ctx->vb+l;

			while(cs1<cse)
			{
//				getPixel2(cs1, &cra, &cga, &cba, &crb, &cgb, &cbb);
//				getPixel2(cs2, &crc, &cgc, &cbc, &crd, &cgd, &cbd);

				switch(pf)
				{
				case BTIC1H_PXF_RGBA:
					cra=*(cs1+0); cga=*(cs1+1); cba=*(cs1+2); caa=*(cs1+3);
					crb=*(cs1+4); cgb=*(cs1+5); cbb=*(cs1+6); cab=*(cs1+7);
					crc=*(cs2+0); cgc=*(cs2+1); cbc=*(cs2+2); cac=*(cs2+3);
					crd=*(cs2+4); cgd=*(cs2+5); cbd=*(cs2+6); cad=*(cs2+7);
					psz=8; break;
				case BTIC1H_PXF_BGRA:
					cra=*(cs1+2); cga=*(cs1+1); cba=*(cs1+0); caa=*(cs1+3);
					crb=*(cs1+6); cgb=*(cs1+5); cbb=*(cs1+4); cab=*(cs1+7);
					crc=*(cs2+2); cgc=*(cs2+1); cbc=*(cs2+0); cac=*(cs2+3);
					crd=*(cs2+6); cgd=*(cs2+5); cbd=*(cs2+4); cad=*(cs2+7);
					psz=8; break;
				case BTIC1H_PXF_RGB:
					cra=*(cs1+0); cga=*(cs1+1); cba=*(cs1+2); caa=255;
					crb=*(cs1+3); cgb=*(cs1+4); cbb=*(cs1+5); cab=255;
					crc=*(cs2+0); cgc=*(cs2+1); cbc=*(cs2+2); cac=255;
					crd=*(cs2+3); cgd=*(cs2+4); cbd=*(cs2+5); cad=255;
					psz=6; break;
				case BTIC1H_PXF_BGR:
					cra=*(cs1+2); cga=*(cs1+1); cba=*(cs1+0); caa=255;
					crb=*(cs1+5); cgb=*(cs1+4); cbb=*(cs1+3); cab=255;
					crc=*(cs2+2); cgc=*(cs2+1); cbc=*(cs2+0); cac=255;
					crd=*(cs2+5); cgd=*(cs2+4); cbd=*(cs2+3); cad=255;
					psz=6; break;
				}

#if 0
				if(ctx->alphaClr)
				{
					ca=(caa+cab+cac+cad)>>2;
					if(ca<ctx->alphaClrA)
					{
						cy=ctx->alphaClrY;
						cu=ctx->alphaClrU;
						cv=ctx->alphaClrV;
						*cty1++=cy; *cty1++=cy;
						*cty2++=cy; *cty2++=cy;
						*ctu++=cu; *ctv++=cv;
						continue;
					}
				}
#endif

				cya=19595*cra + 38470*cga + 7471*cba;
				cyb=19595*crb + 38470*cgb + 7471*cbb;
				cyc=19595*crc + 38470*cgc + 7471*cbc;
				cyd=19595*crd + 38470*cgd + 7471*cbd;
				*cty1++=cya>>16;
				*cty1++=cyb>>16;
				*cty2++=cyc>>16;
				*cty2++=cyd>>16;

				cr=(cra+crb+crc+crd)>>2;
				cg=(cga+cgb+cgc+cgd)>>2;
				cb=(cba+cbb+cbc+cbd)>>2;

				cu=-11056*cr -21712*cg +32768*cb;
				cv= 32768*cr -27440*cg - 5328*cb;
				*ctu++=(cu>>16)+128;
				*ctv++=(cv>>16)+128;

				cs1+=psz2; cs2+=psz2;
			}
		}
	}else
	{
		switch(pf)
		{
		case BTIC1H_PXF_YUVA: psz=4; psz2=8; break;
		case BTIC1H_PXF_YUV: psz=3; psz2=6; break;
		case BTIC1H_PXF_YUV422: psz=2; psz2=4; break;
		case BTIC1H_PXF_YUV420: psz=3; psz2=3; break;
		case BTIC1H_PXF_YA: psz=2; psz2=4; break;
		case BTIC1H_PXF_Y: psz=1; psz2=2; break;
		case BTIC1H_PXF_YYYA: psz=4; psz2=8; break;
		default: psz=4; psz2=8; break;
		}

		n=(ys+1)/2;
		for(i=0; i<n; i++)
		{
			if(pf==BTIC1H_PXF_YUV420)
			{
				k=(((ys-(2*i+1))*xs2)*psz2)>>1;
				cs1=ibuf+k; cse=cs1+((xs*psz2)>>1);
				l=(((ys-(2*i+2))*xs2)*psz2)>>1;
				cs2=ibuf+l;
			}else
			{
				k=((ys-(2*i+1))*xs)*psz;
				cs1=ibuf+k; cse=cs1+xs*psz;
				l=((ys-(2*i+2))*xs)*psz;
				cs2=ibuf+l;
			}

			cty1=ctx->yb+(2*i+0)*xs2;
			cty2=ctx->yb+(2*i+1)*xs2;

			l=i*xs3;
			ctu=ctx->ub+l;
			ctv=ctx->vb+l;

			while(cs1<cse)
			{
//				getPixel2(cs1, &cra, &cga, &cba, &crb, &cgb, &cbb);
//				getPixel2(cs2, &crc, &cgc, &cbc, &crd, &cgd, &cbd);

				switch(pf)
				{
				case BTIC1H_PXF_YUVA:
					cya=*(cs1+0); cua=*(cs1+1); cva=*(cs1+2);
					cyb=*(cs1+4); cub=*(cs1+5); cvb=*(cs1+6);
					cyc=*(cs2+0); cuc=*(cs2+1); cvc=*(cs2+2);
					cyd=*(cs2+4); cud=*(cs2+5); cvd=*(cs2+6);
					cu=(cua+cub+cuc+cud)>>2;
					cv=(cva+cvb+cvc+cvd)>>2;
					break;
				case BTIC1H_PXF_YUV:
					cya=*(cs1+0); cua=*(cs1+1); cva=*(cs1+2);
					cyb=*(cs1+3); cub=*(cs1+4); cvb=*(cs1+5);
					cyc=*(cs2+0); cuc=*(cs2+1); cvc=*(cs2+2);
					cyd=*(cs2+3); cud=*(cs2+4); cvd=*(cs2+5);
					cu=(cua+cub+cuc+cud)>>2;
					cv=(cva+cvb+cvc+cvd)>>2;
					break;
				case BTIC1H_PXF_YUV422:
					cya=*(cs1+0); cua=*(cs1+1); cyb=*(cs1+2); cva=*(cs1+3);
					cyc=*(cs2+0); cuc=*(cs2+1); cyd=*(cs2+2); cvc=*(cs2+3);
					cu=(cua+cuc)>>1; cv=(cva+cvc)>>1;
					break;
				case BTIC1H_PXF_YUV420:
					cya=*(cs1+0); cyb=*(cs1+1); cu=*(cs1+2);
					cyc=*(cs2+0); cyd=*(cs2+1); cv=*(cs2+2);
					break;
				case BTIC1H_PXF_YA:
					cya=*(cs1+0); cyb=*(cs1+2);
					cyc=*(cs2+0); cyd=*(cs2+2);
					cu=0; cv=0;
					break;
				case BTIC1H_PXF_Y:
					cya=*(cs1+0); cyb=*(cs1+1);
					cyc=*(cs2+0); cyd=*(cs2+1);
					cu=0; cv=0;
					break;

				case BTIC1H_PXF_YYYA:
					cya=*(cs1+1); cyb=*(cs1+5);
					cyc=*(cs2+1); cyd=*(cs2+5);
					cu=0; cv=0;
					break;
				}

				*cty1++=cya; *cty1++=cyb;
				*cty2++=cyc; *cty2++=cyd;
				*ctu++=cu; *ctv++=cv;

				cs1+=psz2; cs2+=psz2;
			}
		}
	}
}

int PDJPG_EncodeFastCtx(PDJPG_Context *ctx,
	byte *ibuf, byte *obuf, int xs, int ys, int qf, int pf)
{
	char tb[256];
	short *tp;
//	byte *cs, *cse, *cty, *ctu, *ctv;
	int xs2, ys2, xs3, ys3;
//	int cr, cg, cb, cy, cu, cv;

//	int cra, cga, cba, cya;
//	int crb, cgb, cbb, cyb;
//	int crc, cgc, cbc, cyc;
//	int crd, cgd, cbd, cyd;
//	byte *cs1, *cs2, *cty1, *cty2;
	byte *ctt;

	int i, j, k, l, n;

	xs2=((xs+7)/8)*8;
	ys2=((ys+7)/8)*8;
	xs3=((xs+15)/16)*8;
	ys3=((ys+15)/16)*8;

	ctx->jpg_mono=0;

//	if(!ctx->jpg_tabcacheframe)
	if(1)
	{
		//full quality
		for(i=0; i<64; i++)ctx->jpg_qt[0][i]=1;
		for(i=0; i<64; i++)ctx->jpg_qt[1][i]=1;
	}

	if(!ctx->yb || (xs!=ctx->lxs) || (ys!=ctx->lys))
	{
		if(ctx->yb)
		{
			free(ctx->yb);
			free(ctx->ub);
			free(ctx->vb);
			free(ctx->ydb);
			free(ctx->udb);
			free(ctx->vdb);
		}

		ctx->yb=malloc(xs2*ys2);
		ctx->ub=malloc(xs2*ys2);
		ctx->vb=malloc(xs2*ys2);

		ctx->ydb=malloc((xs2+8)*(ys2+16)*sizeof(short));
		ctx->udb=malloc((xs3+8)*(ys3+8)*sizeof(short));
		ctx->vdb=malloc((xs3+8)*(ys3+8)*sizeof(short));

		ctx->lxs=xs;
		ctx->lys=ys;

		memset(ctx->yb, 128, xs2*ys2);
		memset(ctx->ub, 128, xs2*ys2);
		memset(ctx->vb, 128, xs2*ys2);

		memset(ctx->ydb, 0, xs2*(ys2+8)*sizeof(short));
		memset(ctx->udb, 0, xs3*(ys3+8)*sizeof(short));
		memset(ctx->vdb, 0, xs3*(ys3+8)*sizeof(short));

//		ctx->jpg_tabcacheframe=0;	//force rebuild
	}

	PDJPG_ConvertImageYUV(ctx,
		ibuf, xs, ys, pf,
		xs2, ys2, xs3, ys3);

	PDJPG_FilterImageDCT(ctx->yb, ctx->ydb, xs2, ys2);
	PDJPG_FilterImageDCT(ctx->ub, ctx->udb, xs3, ys3);
	PDJPG_FilterImageDCT(ctx->vb, ctx->vdb, xs3, ys3);

//	if(ctx->jpg_tabcacheframe<=0)
	if(1)
	{
		j=(xs2/8)*(ys2/8);
//		PDJPG_MakeQuantTabInputFast(ctx->ydb, j, ctx->jpg_qt[0], qf/100.0);
		PDJPG_MakeQuantTabFastIJG_Y(
			ctx->ydb, j, ctx->jpg_qt[0], qf/100.0);

		j=(xs3/8)*(ys3/8);
//		PDJPG_MakeQuantTabInputFast(ctx->udb, j, ctx->jpg_qt[1], qf/100.0);
//		PDJPG_MakeQuantTabInputFast(ctx->vdb, j, ctx->jpg_qt[2], qf/100.0);
		PDJPG_MakeQuantTabFastIJG_UV(
			ctx->udb, j, ctx->jpg_qt[1], qf/100.0);
		PDJPG_MakeQuantTabFastIJG_UV(
			ctx->vdb, j, ctx->jpg_qt[2], qf/100.0);
		for(i=0; i<64; i++)
			ctx->jpg_qt[1][i]=(ctx->jpg_qt[1][i]+ctx->jpg_qt[2][i])/2;

		PDJPG_SetupQuantTabDivFP(ctx, 0);
		PDJPG_SetupQuantTabDivFP(ctx, 1);
	}

	if(ctx->jpg_mono)
	{
		j=(xs2/8)*(ys2/8); k=0;
		for(i=0; i<j; i++)
		{
			PDJHUFF_QuantBlock(ctx, ctx->ydb+i*64, ctx->ydb+i*64, 0);
			ctx->ydb[i*64+0]-=k;
			k=ctx->ydb[i*64+0]+k;
		}
	}

	if(!ctx->jpg_mono)
	{
		l=0;
		for(i=0; i<=(ys3/8); i++)
			for(j=0; j<(xs3/8); j++)
		{
			tp=ctx->ydb+((i*2+0)*(xs2/8)+j*2+0)*64;
			PDJHUFF_QuantBlock(ctx, tp, tp, 0);
			tp[0]-=l; l=tp[0]+l;

			tp=ctx->ydb+((i*2+0)*(xs2/8)+j*2+1)*64;
			PDJHUFF_QuantBlock(ctx, tp, tp, 0);
			tp[0]-=l; l=tp[0]+l;

			tp=ctx->ydb+((i*2+1)*(xs2/8)+j*2+0)*64;
			PDJHUFF_QuantBlock(ctx, tp, tp, 0);
			tp[0]-=l; l=tp[0]+l;

			tp=ctx->ydb+((i*2+1)*(xs2/8)+j*2+1)*64;
			PDJHUFF_QuantBlock(ctx, tp, tp, 0);
			tp[0]-=l; l=tp[0]+l;
		}
	}

	j=(xs3/8)*(ys3/8); k=0; l=0;
	for(i=0; i<j; i++)
	{
		PDJHUFF_QuantBlock(ctx, ctx->udb+i*64, ctx->udb+i*64, 1);
		PDJHUFF_QuantBlock(ctx, ctx->vdb+i*64, ctx->vdb+i*64, 1);
		ctx->udb[i*64+0]-=k; k=ctx->udb[i*64+0]+k;
		ctx->vdb[i*64+0]-=l; l=ctx->vdb[i*64+0]+l;
	}

//	printf("M2\n");

//	if(ctx->jpg_tabcacheframe<=0)
	if(1)
	{
		for(i=0; i<256; i++)ctx->dcs[i]=1;
		for(i=0; i<256; i++)ctx->acs[i]=1;
		for(i=0; i<256; i++)ctx->dcsuv[i]=1;
		for(i=0; i<256; i++)ctx->acsuv[i]=1;

		j=(xs2/8)*(ys2/8);
		k=(xs3/8)*(ys3/8);
		for(i=0; i<j; i++)
			PDJHUFF_StatBlock(ctx->ydb+i*64, ctx->dcs, ctx->acs);
		for(i=0; i<k; i++)
			PDJHUFF_StatBlock(ctx->udb+i*64, ctx->dcsuv, ctx->acsuv);
		for(i=0; i<k; i++)
			PDJHUFF_StatBlock(ctx->vdb+i*64, ctx->dcsuv, ctx->acsuv);

		PDJHUFF_BuildLengthsAdjust(
			ctx->dcs, 256, ctx->huff_len+0*256, 16);
		PDJHUFF_BuildLengthsAdjust(
			ctx->acs, 256, ctx->huff_len+1*256, 16);
		PDJHUFF_BuildLengthsAdjust(
			ctx->dcsuv, 256, ctx->huff_len+2*256, 16);
		PDJHUFF_BuildLengthsAdjust(
			ctx->acsuv, 256, ctx->huff_len+3*256, 16);
	}

	ctx->huff_ct=obuf;
	ctx->huff_win=0;
	ctx->huff_pos=0;

#if 0
	if(ctx->alphaClr && (ctx->alphaClr!=ctx->oldAlphaClr))
	{
		sprintf(tb, "%d %d %d %d",
			(ctx->alphaClr&0xFF),
			((ctx->alphaClr>>8)&0xFF),
			((ctx->alphaClr>>16)&0xFF),
			((ctx->alphaClr>>24)&0xFF));
	
		*ctx->huff_ct++=0xFF;
		*ctx->huff_ct++=JPG_APP11;
		ctt=ctx->huff_ct;
		*ctx->huff_ct++=0x00;
		*ctx->huff_ct++=0x00;
		PDJHUFF_WriteString(ctx, "AlphaColor");
		PDJHUFF_WriteString(ctx, tb);
		i=(ctx->huff_ct-ctt);
		ctt[0]=(i>>8)&0xFF; ctt[1]=i&0xFF;
	}
#endif

	*ctx->huff_ct++=0xFF;
	*ctx->huff_ct++=JPG_SOI;

//	if(ctx->jpg_tabcacheframe<=0)
	if(1)
	{
		PDJPG_EmitDQT(ctx, 0);
		if(!ctx->jpg_mono)PDJPG_EmitDQT(ctx, 1);
	}

	PDJPG_EmitSOF(ctx, xs, ys);

//	if(ctx->jpg_tabcacheframe<=0)
	if(1)
	{
		PDJPG_EmitDHT(ctx, 0);
		PDJPG_EmitDHT(ctx, 1);
		if(!ctx->jpg_mono)
		{
			PDJPG_EmitDHT(ctx, 2);
			PDJPG_EmitDHT(ctx, 3);
		}
	}

	PDJPG_EmitSOS(ctx);

	ctx->huff_win=0;
	ctx->huff_pos=0;

	if(ctx->jpg_mono)
	{
		j=(xs2/8)*(ys2/8);
		for(i=0; i<j; i++)PDJHUFF_EncodeBlock(ctx, ctx->ydb+i*64, 0, 1);
	}else
	{
		for(i=0; i<=((ys3)/8); i++)
			for(j=0; j<(xs3/8); j++)
		{
			PDJHUFF_EncodeBlock(ctx,
				ctx->ydb+((i*2+0)*(xs2/8)+j*2+0)*64, 0, 1);
			PDJHUFF_EncodeBlock(ctx,
				ctx->ydb+((i*2+0)*(xs2/8)+j*2+1)*64, 0, 1);
			PDJHUFF_EncodeBlock(ctx,
				ctx->ydb+((i*2+1)*(xs2/8)+j*2+0)*64, 0, 1);
			PDJHUFF_EncodeBlock(ctx,
				ctx->ydb+((i*2+1)*(xs2/8)+j*2+1)*64, 0, 1);

			k=i*(xs3/8)+j;
			PDJHUFF_EncodeBlock(ctx, ctx->udb+k*64, 2, 3);
			PDJHUFF_EncodeBlock(ctx, ctx->vdb+k*64, 2, 3);
		}
	}

	PDJHUFF_FlushBits(ctx);

	*ctx->huff_ct++=0xFF;
	*ctx->huff_ct++=JPG_EOI;

#if 0
	if(ctx->jpg_tabcacheframe<=0)
	{
//		ctx->jpg_tabcacheframe=16;
		ctx->jpg_tabcacheframe=8;
	}else
	{
		ctx->jpg_tabcacheframe--;
	}
#endif

//	printf("M3\n");

	i=ctx->huff_ct-obuf;

	return(i);
}

int PDJPG_EncodeFast(
	byte *ibuf, byte *obuf, int xs, int ys, int qf)
{
	static PDJPG_Context *ctx=NULL;
	int sz;
	
	if(!ctx)ctx=PDJPG_AllocContext();
	sz=PDJPG_EncodeFastCtx(ctx, ibuf, obuf, xs, ys, qf, 0);
//	PDJPG_FreeContext(ctx);
	return(sz);
}

byte *PDJPG_EmitComponentLayer(PDJPG_Context *ctx,
	byte *ct, char *name)
{
	byte *ctt;
	int i;

	ctx->huff_ct=ct; ctx->huff_win=0; ctx->huff_pos=0;
	*ctx->huff_ct++=0xFF;
	*ctx->huff_ct++=JPG_APP11;
	ctt=ctx->huff_ct;
	*ctx->huff_ct++=0x00;
	*ctx->huff_ct++=0x00;
	PDJHUFF_WriteString(ctx, "CompLayer");
	PDJHUFF_WriteString(ctx, name);
	i=ctx->huff_ct-ctt;
	ctt[0]=(i>>8)&0xFF; ctt[1]=i&0xFF;
	return(ctx->huff_ct);
}

int PDJPG_EscapeEncodeBuffer(byte *ibuf, int isz,
	byte *obuf, int osz)
{
	byte *cs, *ct, *cse, *cte;
	
	cs=ibuf; cse=ibuf+isz;
	ct=obuf; cte=obuf+osz;
	
	while((cs<cse) && (ct<cte))
	{
		if(*cs==0xFF)
		{
			cs++;
			*ct++=0xFF;
			*ct++=0x00;
			continue;
		}
		
		*ct++=*cs++;
	}
	
	if(ct>=cte)return(-1);
	return(ct-obuf);
}

int PDJPG_EscapeEncodeSingleBuffer(byte *buf, int sz)
{
	byte *tbuf;
	int i;
	
	tbuf=malloc(sz*2);
	i=PDJPG_EscapeEncodeBuffer(buf, sz, tbuf, sz*2);
	if(i<0)return(i);
	memcpy(buf, tbuf, i);
	free(tbuf);
	return(i);
}

int PDJPG_EncodeLDatCtx(
	PDJPG_Context *ctx, byte *ibuf, byte *obuf,
	int xs, int ys, int qf, int pf)
{
	byte *tbuf, *cts;
	int i, j, sz, tsz;
	
//	if(!ctx)ctx=PDJPG_AllocContext();

	tbuf=malloc(1<<20);
	sz=PDJPG_EncodeCtx(ctx, ibuf, tbuf, xs, ys, qf, pf);

	sz=PDJPG_EscapeEncodeSingleBuffer(tbuf, sz);

	cts=tbuf; tsz=sz;
	ctx->huff_ct=obuf; ctx->huff_win=0; ctx->huff_pos=0;
	while(tsz>=65528)
	{
		i=65529;
		if(cts[i-1]==0xFF)i--;
		j=i+6;
		*ctx->huff_ct++=0xFF;
		*ctx->huff_ct++=JPG_APP12;
		*ctx->huff_ct++=(j>>8)&0xFF;
		*ctx->huff_ct++=j&0xFF;

		*ctx->huff_ct++='L';
		*ctx->huff_ct++='D';
		*ctx->huff_ct++='A';
		*ctx->huff_ct++='T';
		
		memcpy(ctx->huff_ct, cts, i);
		cts+=i; ctx->huff_ct+=i;
	}
	
	if((tsz>0) && (tsz<65528))
	{
		i=sz+6;
		*ctx->huff_ct++=0xFF;
		*ctx->huff_ct++=JPG_APP12;
		*ctx->huff_ct++=(i>>8)&0xFF;
		*ctx->huff_ct++=i&0xFF;

		*ctx->huff_ct++='L';
		*ctx->huff_ct++='D';
		*ctx->huff_ct++='A';
		*ctx->huff_ct++='T';
		
		memcpy(ctx->huff_ct, cts, tsz);
		ctx->huff_ct+=sz;
	}

	sz=ctx->huff_ct-obuf;
	free(tbuf);
	
	return(sz);
}

int PDJPG_EncodeComponentCtx(
	PDJPG_Context *ctx,
	byte *rgba, byte *norm, byte *spec, byte *luma,
	byte *obuf, int xs, int ys, int qf)
{
	byte *tbuf;
	byte *ct;
	int i, j, k, n;

	if(!rgba)return(-1);

	n=xs*ys;
	tbuf=malloc(xs*ys*4);

	ct=obuf;
//	ct=PDJPG_EmitComponentLayer(ctx, ct, "RGB");
	i=PDJPG_EncodeCtx(ctx, rgba, ct, xs, ys, qf, 0);
	if(i<0) { free(tbuf); return(i); }
	ct+=i;
	
	if(norm)
	{
		for(i=0; i<n; i++)
		{
			tbuf[i*4+0]=norm[i*4+0];
			tbuf[i*4+1]=norm[i*4+2];
			tbuf[i*4+2]=norm[i*4+1];
			tbuf[i*4+3]=255;
		}

		ct=PDJPG_EmitComponentLayer(ctx, ct, "XYZ");
		i=PDJPG_EncodeLDatCtx(ctx, tbuf, ct, xs, ys, qf, 0);
		if(i<0) { free(tbuf); return(i); }
		ct+=i;
	}

	if(spec)
	{
		ct=PDJPG_EmitComponentLayer(ctx, ct, "SpRGB");
		i=PDJPG_EncodeLDatCtx(ctx, spec, ct, xs, ys, qf, 0);
		if(i<0) { free(tbuf); return(i); }
		ct+=i;
	}

	if(norm || spec)
	{
		for(i=0; i<n; i++)
		{
			j=rgba[i*4+3];
			tbuf[i*4+0]=norm?norm[i*4+3]:j;
			tbuf[i*4+1]=rgba[i*4+3];
			tbuf[i*4+2]=spec?spec[i*4+3]:j;
			tbuf[i*4+3]=255;
		}

		ct=PDJPG_EmitComponentLayer(ctx, ct, "DASe");
		i=PDJPG_EncodeLDatCtx(ctx, tbuf, ct, xs, ys, qf, 0);
		if(i<0) { free(tbuf); return(i); }
		ct+=i;
	}else
	{
		k=0;
		for(i=0; i<n; i++)
		{
			j=rgba[i*4+3];
			if(j!=255)k=1;
			tbuf[i*4+0]=j;
			tbuf[i*4+1]=j;
			tbuf[i*4+2]=j;
			tbuf[i*4+3]=255;
		}

		if(k)
		{
			ct=PDJPG_EmitComponentLayer(ctx, ct, "Alpha");
			i=PDJPG_EncodeLDatCtx(ctx,
				tbuf, ct, xs, ys, qf, BTIC1H_PXF_YYYA);
			if(i<0) { free(tbuf); return(i); }
			ct+=i;
		}
	}

	if(luma)
	{
		ct=PDJPG_EmitComponentLayer(ctx, ct, "LuRGB");
		i=PDJPG_EncodeLDatCtx(ctx, luma, ct, xs, ys, qf, 0);
		if(i<0) { free(tbuf); return(i); }
		ct+=i;
	}

	free(tbuf);
	return(ct-obuf);
}

int PDJPG_EncodeComponent(
	byte *rgba, byte *norm, byte *spec, byte *luma,
	byte *obuf, int xs, int ys, int qf)
{
	PDJPG_Context *ctx;
	int sz;
	
	ctx=PDJPG_AllocContext();
	sz=PDJPG_EncodeComponentCtx(ctx,
		rgba, norm, spec, luma,
		obuf, xs, ys, qf);
	PDJPG_FreeContext(ctx);
	return(sz);
}

BTEIFGL_API int PDJPG_EncodeRgbaCtx(
	PDJPG_Context *ctx,
	byte *rgba, byte *obuf, int xs, int ys, int qf)
{
	return(PDJPG_EncodeClrsCtx(ctx, rgba,
		obuf, xs, ys, qf, BTIC1H_PXF_RGBA));
}

BTEIFGL_API int PDJPG_EncodeClrsCtx(
	PDJPG_Context *ctx,
	byte *rgba, byte *obuf, int xs, int ys, int qf, int pf)
{
	byte *tbuf;
	byte *ct;
	int i, j, k, n, pf2;

	if(!rgba)return(-1);

	n=xs*ys;
	tbuf=malloc(xs*ys*4);

	pf2=pf;
	if(pf==BTIC1H_PXF_RGB8E8)
		pf2=BTIC1H_PXF_RGBA;

	ct=obuf;
	i=PDJPG_EncodeCtx(ctx, rgba, ct, xs, ys, qf, pf2);
	if(i<0) { free(tbuf); return(i); }
	ct+=i;

	if((pf==BTIC1H_PXF_RGBA) ||
		(pf==BTIC1H_PXF_BGRA))
	{
		k=0;
		for(i=0; i<n; i++)
		{
			j=rgba[i*4+3];
//			if(j!=255)k=1;
			if(j<240)k=1;
			tbuf[i*4+0]=j;
			tbuf[i*4+1]=j;
			tbuf[i*4+2]=j;
			tbuf[i*4+3]=255;
		}

		if(k)
		{
			ct=PDJPG_EmitComponentLayer(ctx, ct, "Alpha");
			i=PDJPG_EncodeLDatCtx(ctx,
				tbuf, ct, xs, ys, qf, BTIC1H_PXF_YYYA);
			if(i<0) { free(tbuf); return(i); }
			ct+=i;
		}
	}

	if(pf==BTIC1H_PXF_RGB8E8)
	{
		k=0;
		for(i=0; i<n; i++)
		{
			j=rgba[i*4+3];
			
			j=((j-128)*8+4)+128;
			
			tbuf[i*4+0]=j;
			tbuf[i*4+1]=j;
			tbuf[i*4+2]=j;
			tbuf[i*4+3]=255;
		}

		ct=PDJPG_EmitComponentLayer(ctx, ct, "AlExp");
		i=PDJPG_EncodeLDatCtx(ctx,
			tbuf, ct, xs, ys, 100, BTIC1H_PXF_YYYA);
		if(i<0) { free(tbuf); return(i); }
		ct+=i;
	}

	free(tbuf);
	return(ct-obuf);
}

BTEIFGL_API int PDJPG_EncodeRgba(
	byte *rgba, byte *obuf, int xs, int ys, int qf)
{
	PDJPG_Context *ctx;
	int sz;
	
	ctx=PDJPG_AllocContext();
	sz=PDJPG_EncodeRgbaCtx(ctx,
		rgba, obuf, xs, ys, qf);
	PDJPG_FreeContext(ctx);
	return(sz);
}

BTEIFGL_API int PDJPG_EncodeClrs(
	byte *rgba, byte *obuf, int xs, int ys, int qf, int pf)
{
	PDJPG_Context *ctx;
	int sz;
	
	ctx=PDJPG_AllocContext();
	sz=PDJPG_EncodeClrsCtx(ctx,
		rgba, obuf, xs, ys, qf, pf);
	PDJPG_FreeContext(ctx);
	return(sz);
}
