#define BTMGL_FOURCC(a, b, c, d)	((a)|((b)<<8)|((c)<<16)|((d)<<24))

#define TKUPI_FCC_qtab	BTMGL_FOURCC('q', 't', 'a', 'b')
#define TKUPI_FCC_rkpt	BTMGL_FOURCC('r', 'k', 'p', 't')
#define TKUPI_FCC_imgd	BTMGL_FOURCC('i', 'm', 'g', 'd')
#define TKUPI_FCC_uphd	BTMGL_FOURCC('u', 'p', 'h', 'd')

#define TKUPI_PIXFMT_BGRA32		1
#define TKUPI_PIXFMT_BGRA64		2
#define TKUPI_PIXFMT_BGRA32_F8	3

#define TKUPI_PIXFMT_BGRA24		5
#define TKUPI_PIXFMT_BGRA48		6
#define TKUPI_PIXFMT_BGRA24_F8	7

#define TKUPI_PIXFMT_RGBA32		9
#define TKUPI_PIXFMT_RGBA64		10
#define TKUPI_PIXFMT_RGBA32_F8	11

#define TKUPI_PIXFMT_RGBA24		13
#define TKUPI_PIXFMT_RGBA48		14
#define TKUPI_PIXFMT_RGBA24_F8	15

#define TKUPI_PIXFMT_BC1		16
#define TKUPI_PIXFMT_BC3		17
#define TKUPI_PIXFMT_BC1_MIP	18
#define TKUPI_PIXFMT_BC3_MIP	19

typedef struct TKuPI_DecState_s TKuPI_DecState;

struct TKuPI_DecState_s {
	byte *cs;
	u32 win;
	byte pos;
	byte status;
	
	byte pk_dc;
	byte pk_ac;
	
	byte mbform;	//macroblock format
	byte pixclrs;	//pixel format+colorspace
	byte qtsc;		//quantizer space
	byte ishdr;		//Is HDR/H variant
	
	u16 img_xs;
	u16 img_ys;

	u16 img_mbcx;
	u16 img_mbcy;
	
	s32 pdc_y;
	s32 pdc_u;
	s32 pdc_v;
	s32 pdc_a;
	
	s32 pdc_qtsc;
	s32 pdc_mvx;
	s32 pdc_mvy;

	byte cpt_dc[256];
	byte cpt_ac[256];
	u16 qtab_y[64];
	u16 qtab_uv[64];

//	void (*TransIBH)(s32 *oblk, s32 *iblk);
};

#if 0
int TKuPI_PeekBits(TKuPI_DecState *ctx, int bits)
{
	return((ctx->win>>ctx->pos)&((1<<bits)-1));
}

void TKuPI_SkipBits(TKuPI_DecState *ctx, int bits)
{
	ctx->pos+=bits;
	while(ctx->pos>=8)
		{ ctx->win=(ctx->win>>8)|((*ctx->cs++)<<24); ctx->pos-=8; }
}

void TKuPI_SetupReadBits(TKuPI_DecState *ctx, byte *buf)
{
	ctx->cs=buf;
	ctx->pos=32;
	TKuPI_SkipBits(ctx, 0);
}
#endif

#if 1
int TKuPI_PeekBits(TKuPI_DecState *ctx, int bits)
{
	u32 v;
	v=*(u32 *)(ctx->cs);
	return((v>>ctx->pos)&((1<<bits)-1));
}

void TKuPI_SkipBits(TKuPI_DecState *ctx, int bits)
{
	int p;

	p=ctx->pos+bits;
	ctx->cs+=p>>3;
	ctx->pos=p&7;
}

void TKuPI_SetupReadBits(TKuPI_DecState *ctx, byte *buf)
{
	ctx->cs=buf;
	ctx->pos=0;
}
#endif

#if 0
int TKuPI_ReadBits(TKuPI_DecState *ctx, int bits)
{
	int b;
	int p;
	u32 v;

	p=ctx->pos;
	v=*(u32 *)(ctx->cs);
	b=((v>>p)&((1<<bits)-1));

	p+=bits;
	ctx->cs+=p>>3;
	ctx->pos=p&7;
	return(b);
}
#endif

#if 1
int TKuPI_ReadBits(TKuPI_DecState *ctx, int bits)
{
	int b;
	b=TKuPI_PeekBits(ctx, bits);
	TKuPI_SkipBits(ctx, bits);
	return(b);
}
#endif

static const byte tkupi_riceqtab[256]={
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 7,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 8
};

static u16 tkupi_ricefasttab[8*256];

int TKuPI_InitRiceFastTab()
{
	int kf, k1, b, r, v, q, l;
	
	if(tkupi_ricefasttab[255]!=0)
		return(0);
	
	for(kf=0; kf<8; kf++)
	{
		for(b=0; b<256; b++)
		{
			q=tkupi_riceqtab[b];
			
			l=q+kf+1;
			if(l>8)
			{
				tkupi_ricefasttab[(kf<<8)|b]=0x000F;
				continue;
			}
			
			k1=kf;
			if((q==0) && (k1>0))k1--;
			if((q>1) && (k1<7))k1++;

			r=(b>>(q+1))&((1<<kf)-1);
			v=((q<<kf)|r);

			tkupi_ricefasttab[(kf<<8)|b]=l|(k1<<4)|(v<<8);
		}
	}
	return(0);
}

int TKuPI_ReadRice(TKuPI_DecState *ctx, int kf)
{
	int b, q, r;

	b=TKuPI_PeekBits(ctx, 16);
	q=tkupi_riceqtab[b&0xFF];
	if(q==8)
	{
		TKuPI_SkipBits(ctx, 16);
		return(b>>8);
	}
	r=(b>>(q+1))&((1<<kf)-1);
	TKuPI_SkipBits(ctx, q+kf+1);
	return((q<<kf)|r);
}

#if 0
int TKuPI_ReadAdRice(TKuPI_DecState *ctx, byte *rkf)
{
	int b, q, r, v, kf, skb;
	int p;

//	p=ctx->pos;
//	v=*(u32 *)(ctx->cs);
//	b=(v>>p);

	kf=*rkf;
	b=TKuPI_PeekBits(ctx, 16);
	q=tkupi_riceqtab[b&0xFF];
	if(q==8)
	{
		kf++;
		skb=16;
		v=(b>>8)&0xFF;
	}else
	{
		r=(b>>(q+1))&((1<<kf)-1);
		skb=q+kf+1;
		v=(q<<kf)|r;
		if(!q && kf)
			{ kf--; }
		else if(q>1)
			{ kf++; }
	}

	TKuPI_SkipBits(ctx, skb);
//	p+=skb;
//	ctx->cs+=p>>3;
//	ctx->pos=p&7;

	*rkf=kf;
	return(v);
}
#endif

#if 1
int TKuPI_ReadAdRice(TKuPI_DecState *ctx, byte *rkf)
{
	int b, q, r, v, kf;

	kf=*rkf;
	b=TKuPI_PeekBits(ctx, 16);

#if 1
	r=(kf<<8)|(b&255);
	r=tkupi_ricefasttab[r];
	q=(r&15);
	if(q<=8)
	{
		v=r>>8;
		*rkf=(r>>4)&15;
		TKuPI_SkipBits(ctx, q);
		return(v);
	}
#endif
	
	q=tkupi_riceqtab[b&0xFF];
	if(q==8)
	{
		kf++; *rkf=kf;
		TKuPI_SkipBits(ctx, 16);
		return(b>>8);
	}
	r=(b>>(q+1))&((1<<kf)-1);
	TKuPI_SkipBits(ctx, q+kf+1);
	v=(q<<kf)|r;
	if(!q && kf)
		{ kf--; *rkf=kf; }
	else if(q>1)
		{ kf++; *rkf=kf; }
	return(v);
}
#endif

int TKuPI_ReadAdRiceSTF(TKuPI_DecState *ctx, byte *rkf, byte *stftab)
{
	int i0, i1, v0, v1;
	
	i0=TKuPI_ReadAdRice(ctx, rkf);
	i1=(i0*15)>>4;
	v0=stftab[i0];	v1=stftab[i1];
	stftab[i0]=v1;	stftab[i1]=v0;
	return(v0);
}

int TKuPI_ReadCoeffB(TKuPI_DecState *ctx, int b)
{
	int h, f, v;

	if(b<4)
	{
		v=(b>>1)^(((s32)(b<<31))>>31);
		return(v);
	}
	
	if((b>=24) && (ctx->ishdr))
	{
		h=b-12;
		v=(1<<h)|TKuPI_ReadBits(ctx, h);
		v=(v>>1)^(((s32)(v<<31))>>31);
		return(v);
	}

	h=(b>>1)-1;
	v=(((2|b)&3)<<h)|TKuPI_ReadBits(ctx, h);

	v=(v>>1)^(((s32)(v<<31))>>31);
	return(v);
}

int TKuPI_ReadCoeffDc(TKuPI_DecState *ctx)
{
	int b, h, f, v;

	b=TKuPI_ReadAdRiceSTF(ctx, &(ctx->pk_dc), ctx->cpt_dc);
	v=TKuPI_ReadCoeffB(ctx, b&31);
	return(v);
}

int TKuPI_ReadCoeffAc(TKuPI_DecState *ctx, int *rv, int *rz)
{
	int b, h, f, v;

	b=TKuPI_ReadAdRiceSTF(ctx, &(ctx->pk_ac), ctx->cpt_ac);
	if(!b)
		return(1);
	v=TKuPI_ReadCoeffB(ctx, b&31);
	*rz=(b>>5)&7;
	*rv=v;
	return(0);
}

int TKuPI_ReadCoeffTag(TKuPI_DecState *ctx, int *rv, int *rz)
{
	int b, h, f, v;

	b=TKuPI_ReadAdRiceSTF(ctx, &(ctx->pk_dc), ctx->cpt_dc);
	v=TKuPI_ReadCoeffB(ctx, b&31);
	*rz=(b>>5)&7;
	*rv=v;
	return(0);
}

static const byte tkupi_izigzag[64]={
 0,  1,  8, 16,  9,  2,  3, 10,
17, 24, 32, 25, 18, 11,  4,  5,
12, 19, 26, 33, 40, 48, 41, 34,
27, 20, 13,  6,  7, 14, 21, 28,
35, 42, 49, 56, 57, 50, 43, 36,
29, 22, 15, 23, 30, 37, 44, 51,
58, 59, 52, 45, 38, 31, 39, 46,
53, 60, 61, 54, 47, 55, 62, 63
};

int TKuPI_ReadBlockCoeffs(TKuPI_DecState *ctx, s32 *blk, u16 *qtab, s32 *rpdc)
{
	int b, pdc;
	int i, j, k, z, v, z0;

	memset(blk, 0, 64*sizeof(s32));

	pdc=*rpdc;
	v=TKuPI_ReadCoeffDc(ctx);
	z=pdc+(v*qtab[0]);
	blk[0]=z;
	*rpdc=z;

	i=1;
	while(i<64)
	{
		if(TKuPI_ReadCoeffAc(ctx, &v, &z))
			break;
		i+=z;
		if(i>=64)
			__debugbreak();
		j=tkupi_izigzag[i];
		blk[j]=v*qtab[j];
		i++;
	}
	return(0);
}

void TKuPI_TransIBH(s32 *oblk, s32 *iblk)
{
	s32 tblk[64];
	s32 *cs, *ct;

	s32 i0, i1, i2, i3, i4, i5, i6, i7;
	s32 s0, s1;
	s32 t0, t1, t2, t3;
	s32 u0, u1, u2, u3, u4, u5, u6, u7;
	
	int i;
	
	cs=iblk; ct=tblk;
	for(i=0; i<8; i++)
	{
		i0=cs[ 0];	i1=cs[ 8];	i2=cs[16];	i3=cs[24];
		i4=cs[32];	i5=cs[40];	i6=cs[48];	i7=cs[56];
		s1=i0-(i1>>1);	s0=s1+i1;
		t1=s0-(i2>>1);	t0=t1+i2;	t3=s1-(i3>>1);	t2=t3+i3;
		u1=t0-(i4>>1);	u0=u1+i4;	u3=t1-(i5>>1);	u2=u3+i5;
		u5=t2-(i6>>1);	u4=u5+i6;	u7=t3-(i7>>1);	u6=u7+i7;
		ct[ 0]=u0;	ct[ 8]=u1;	ct[16]=u2;	ct[24]=u3;
		ct[32]=u4;	ct[40]=u5;	ct[48]=u6;	ct[56]=u7;
		cs++;	ct++;
	}

	cs=tblk; ct=oblk;
	for(i=0; i<8; i++)
	{
		i0=cs[0];	i1=cs[1];	i2=cs[2];	i3=cs[3];
		i4=cs[4];	i5=cs[5];	i6=cs[6];	i7=cs[7];
		s1=i0-(i1>>1);	s0=s1+i1;
		t1=s0-(i2>>1);	t0=t1+i2;	t3=s1-(i3>>1);	t2=t3+i3;
		u1=t0-(i4>>1);	u0=u1+i4;	u3=t1-(i5>>1);	u2=u3+i5;
		u5=t2-(i6>>1);	u4=u5+i6;	u7=t3-(i7>>1);	u6=u7+i7;
		ct[0]=u0;	ct[1]=u1;	ct[2]=u2;	ct[3]=u3;
		ct[4]=u4;	ct[5]=u5;	ct[6]=u6;	ct[7]=u7;
		cs+=8;	ct+=8;
	}
}

void TKuPI_TransICDF53(s32 *oblk, s32 *iblk)
{
	s32 tblk[64];
	s32 *cs, *ct;

	s32 i0, i1, i2, i3, i4, i5, i6, i7;
	s32 s0, s1;
	s32 t0, t1, t2, t3;
	s32 u0, u1, u2, u3, u4, u5, u6, u7;
	
	int i;
	
	cs=iblk; ct=tblk;
	for(i=0; i<8; i++)
	{
		i0=cs[ 0];	i1=cs[ 8];	i2=cs[16];	i3=cs[24];
		i4=cs[32];	i5=cs[40];	i6=cs[48];	i7=cs[56];
		s0=i0-(i1>>1);			s1=s0+i1;
		t0=s0;		t1=i2+i0;	t2=s1;		t3=i3+i0;
		u0=t0;		u1=i4+((t0+t1)>>1);		u2=t1;		u3=i5+((t1+t2)>>1);
		u4=t2;		u5=i6+((t2+t3)>>1);		u6=t3;		u7=i7+((t3+t0)>>1);
		ct[ 0]=u0;	ct[ 8]=u1;	ct[16]=u2;	ct[24]=u3;
		ct[32]=u4;	ct[40]=u5;	ct[48]=u6;	ct[56]=u7;
		cs++;	ct++;
	}

	cs=tblk; ct=oblk;
	for(i=0; i<8; i++)
	{
		i0=cs[0];	i1=cs[1];	i2=cs[2];	i3=cs[3];
		i4=cs[4];	i5=cs[5];	i6=cs[6];	i7=cs[7];
		s0=i0-(i1>>1);			s1=s0+i1;
		t0=s0;		t1=i2+i0;	t2=s1;		t3=i3+i0;
		u0=t0;		u1=i4+((t0+t1)>>1);		u2=t1;		u3=i5+((t1+t2)>>1);
		u4=t2;		u5=i6+((t2+t3)>>1);		u6=t3;		u7=i7+((t3+t0)>>1);
		ct[0]=u0;	ct[1]=u1;	ct[2]=u2;	ct[3]=u3;
		ct[4]=u4;	ct[5]=u5;	ct[6]=u6;	ct[7]=u7;
		cs+=8;	ct+=8;
	}
}

void TKuPI_TransIWHT_Stride8(int *iblk, int *oblk, int str)
{
#if 1
	s32 i0, i1, i2, i3, i4, i5, i6, i7;
	s32 s0, s1, s2, s3, s4, s5, s6, s7;
	s32 t0, t1, t2, t3, t4, t5, t6, t7;
	s32 u0, u1, u2, u3, u4, u5, u6, u7;
	
	i0=iblk[str*0];		i1=iblk[str*1];
	i2=iblk[str*2];		i3=iblk[str*3];
	i4=iblk[str*4];		i5=iblk[str*5];
	i6=iblk[str*6];		i7=iblk[str*7];
	s1=i0-(i1>>1);		s0=s1+i1;
	s3=i2-(i3>>1);		s2=s3+i3;
	s5=i4-(i5>>1);		s4=s5+i5;
	s7=i6-(i7>>1);		s6=s7+i7;

	t1=s0-(s2>>1);		t0=t1+s2;
	t3=s1-(s3>>1);		t2=t3+s3;
	t5=s4-(s6>>1);		t4=t5+s6;
	t7=s5-(s7>>1);		t6=t7+s7;

	u1=t0-(t4>>1);		u0=u1+t4;
	u3=t1-(t5>>1);		u2=u3+t5;
	u5=t2-(t6>>1);		u4=u5+t6;
	u7=t3-(t7>>1);		u6=u7+t7;
	oblk[str*0]=u0;		oblk[str*1]=u1;
	oblk[str*2]=u2;		oblk[str*3]=u3;
	oblk[str*4]=u4;		oblk[str*5]=u5;
	oblk[str*6]=u6;		oblk[str*7]=u7;
#endif
}

void TKuPI_TransIWHT(s32 *oblk, s32 *iblk)
{
	s32 tblk[64];
	s32 *cs, *ct;
	
	int i;
	
	cs=iblk; ct=tblk;
	for(i=0; i<8; i++)
		{ TKuPI_TransIWHT_Stride8(cs, ct, 8); cs++; ct++; }
	cs=tblk; ct=oblk;
	for(i=0; i<8; i++)
		{ TKuPI_TransIWHT_Stride8(cs, ct, 1); cs+=8; ct+=8; }
}

void TKuPI_TransIDCT_Horiz(s32 *iblk, s32 *oblk)
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

void TKuPI_TransIDCT_Vert2(s32 *iblk, s32 *oblk)
{
	int a, b, c, d, e, f;
	int i, j, k, l;
	int m, n, o, p;

	a=iblk[ 0]* 91;	b=iblk[32]*91;
	c=iblk[16]*118;	d=iblk[16]*49;
	e=iblk[48]*118;	f=iblk[48]*49;
	i=a+c+b+f;	j=a+d-b-e;	k=a-d-b+e;	l=a-c+b-f;
	m=iblk[ 8]*126+iblk[24]*106+iblk[40]* 71+iblk[56]* 25;
	n=iblk[ 8]*106-iblk[24]* 25-iblk[40]*126-iblk[56]* 71;
	o=iblk[ 8]* 71-iblk[24]*126+iblk[40]* 25+iblk[56]*106;
	p=iblk[ 8]* 25-iblk[24]* 71+iblk[40]*106-iblk[56]*126;
	oblk[ 0]=((i+m)>>16);	oblk[ 8]=((j+n)>>16);
	oblk[16]=((k+o)>>16);	oblk[24]=((l+p)>>16);
	oblk[32]=((l-p)>>16);	oblk[40]=((k-o)>>16);
	oblk[48]=((j-n)>>16);	oblk[56]=((i-m)>>16);
}

void TKuPI_TransIDCT(s32 *oblk, s32 *iblk)
{
	s32 s[64], t[64];
	s32 *ob, *obe;
	int i, j, k;

	TKuPI_TransIDCT_Horiz(iblk+0, s+0);
	TKuPI_TransIDCT_Horiz(iblk+8, s+8);
	TKuPI_TransIDCT_Horiz(iblk+16, s+16);
	TKuPI_TransIDCT_Horiz(iblk+24, s+24);
	TKuPI_TransIDCT_Horiz(iblk+32, s+32);
	TKuPI_TransIDCT_Horiz(iblk+40, s+40);
	TKuPI_TransIDCT_Horiz(iblk+48, s+48);
	TKuPI_TransIDCT_Horiz(iblk+56, s+56);

	TKuPI_TransIDCT_Vert2(s+0, oblk+0);
	TKuPI_TransIDCT_Vert2(s+1, oblk+1);
	TKuPI_TransIDCT_Vert2(s+2, oblk+2);
	TKuPI_TransIDCT_Vert2(s+3, oblk+3);
	TKuPI_TransIDCT_Vert2(s+4, oblk+4);
	TKuPI_TransIDCT_Vert2(s+5, oblk+5);
	TKuPI_TransIDCT_Vert2(s+6, oblk+6);
	TKuPI_TransIDCT_Vert2(s+7, oblk+7);
}


int tkupi_clamp255(int vi)
{
	int v;
	v=vi;
	if(vi<  0)	v=  0;
	if(vi>255)	v=255;
	return(v);
}

int tkupi_clamp64k_hdr(int vi)
{
	int v;
	v=vi;
	if(vi<  0)
		{ v=vi^((s32)0xFFFF7FFF); }
	if(vi>0xFFFF)	v=0xFFFF;
	return(v);
}

int (*tkupi_doclamp)(int x);
int tkupi_clampmask;

void (*tkupi_doclrtrans)(
	int cy, int cu, int cv,
	int *rcr, int *rcg, int *rcb);

void TKuPI_ClrTrans_ApxYUV(
	int cy, int cu, int cv,
	int *rcr, int *rcg, int *rcb)
{
	int cr, cg, cb;
	cb=cu+cy;
	cr=cv+cy;
	cg=cy-((3*cv+cu)>>2);
	*rcr=cr; *rcg=cg; *rcb=cb;
}

void TKuPI_ClrTrans_YCoCgR(
	int cy, int cu, int cv,
	int *rcr, int *rcg, int *rcb)
{
	int cr, cg, cb, t;
	t=cy-(cv>>1);
	cg=cv+t;
	cb=t-(cu>>1);
	cr=cb+cu;
	*rcr=cr; *rcg=cg; *rcb=cb;
}


void TKuPI_TransIYUV420(u16 *oimg, int ostr,
	s32 *iblky, s32 *iblku, s32 *iblkv,
	u16 *pimg)
{
	int (*clamp)(int x);
	void (*dotrans)(
		int cy, int cu, int cv,
		int *rcr, int *rcg, int *rcb);
	int clampmask;
	int cy0, cy1, cy2, cy3, cu, cv;
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int cr3, cg3, cb3;
	int t0, t1, t2, t3;
	int tcr, tcg, tcb;
	int x, y, z, czr, czg, czb;
	
	clamp=tkupi_doclamp;
	clampmask=tkupi_clampmask;
	dotrans=tkupi_doclrtrans;
	tcr=0; tcg=0; tcb=0;
	
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		z=(y*2)*8+(x*2);
		cy0=iblky[z+0];		cy1=iblky[z+1];
		cy2=iblky[z+8];		cy3=iblky[z+9];
		z=y*8+x;
		cu=iblku[z];	cv=iblkv[z];

		if(dotrans)
		{
			dotrans(cy0, cu, cv, &tcr, &tcg, &tcb);
			cr0=tcr;	cg0=tcg;	cb0=tcb;
			dotrans(cy1, cu, cv, &tcr, &tcg, &tcb);
			cr1=tcr;	cg1=tcg;	cb1=tcb;
			dotrans(cy2, cu, cv, &tcr, &tcg, &tcb);
			cr2=tcr;	cg2=tcg;	cb2=tcb;
			dotrans(cy3, cu, cv, &tcr, &tcg, &tcb);
			cr3=tcr;	cg3=tcg;	cb3=tcb;
		}else
		{
			z=((cu+cv)>>2);
			cg0=cy0-z;	cg1=cy1-z;
			cg2=cy2-z;	cg3=cy3-z;
			cr0=cv+cg0;	cr1=cv+cg1;
			cr2=cv+cg2;	cr3=cv+cg3;
			cb0=cu+cg0;	cb1=cu+cg1;
			cb2=cu+cg2;	cb3=cu+cg3;
		}

		if(pimg)
		{
			z=((y*2+0)*(16*4)+(x*2+0))*4;
			cb0+=pimg[z+0]; cg0+=pimg[z+1]; cr0+=pimg[z+2];
			cb1+=pimg[z+4]; cg1+=pimg[z+5]; cr1+=pimg[z+6];
			z=((y*2+1)*(16*4)+(x*2+0))*4;
			cb2+=pimg[z+0]; cg2+=pimg[z+1]; cr2+=pimg[z+2];
			cb3+=pimg[z+4]; cg3+=pimg[z+5]; cr3+=pimg[z+6];
		}

		t0=cr0|cr1; t1=cr2|cr3;
		t2=cg0|cg1; t3=cg2|cg3;
		czr=t0|t1;
		t0=cb0|cb1; t1=cb2|cb3;
		czg=t2|t3;	czb=t0|t1;

		if(czr&clampmask)
		{	cr0=clamp(cr0);	cr1=clamp(cr1);
			cr2=clamp(cr2);	cr3=clamp(cr3);	}
		if(czg&clampmask)
		{	cg0=clamp(cg0);	cg1=clamp(cg1);
			cg2=clamp(cg2);	cg3=clamp(cg3);	}
		if(czb&clampmask)
		{	cb0=clamp(cb0);	cb1=clamp(cb1);
			cb2=clamp(cb2);	cb3=clamp(cb3);	}
		
		z=((y*2+0)*ostr+(x*2+0))*4;
		oimg[z+0]=cb0;		oimg[z+1]=cg0;
		oimg[z+2]=cr0;		oimg[z+3]=255;
		oimg[z+4]=cb1;		oimg[z+5]=cg1;
		oimg[z+6]=cr1;		oimg[z+7]=255;
		z=((y*2+1)*ostr+(x*2+0))*4;
		oimg[z+0]=cb2;		oimg[z+1]=cg2;
		oimg[z+2]=cr2;		oimg[z+3]=255;
		oimg[z+4]=cb3;		oimg[z+5]=cg3;
		oimg[z+6]=cr3;		oimg[z+7]=255;
	}
}

void TKuPI_TransIY4(u16 *oimg, int ostr,
	s32 *iblky,
	u16 *pimg)
{
	int (*clamp)(int x);
	int clampmask;
	int cy0, cy1, cy2, cy3;
	int x, y, z;
	
	clamp=tkupi_doclamp;
	clampmask=tkupi_clampmask;
	
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		z=(y*2*8)+(x*2);
		cy0=iblky[z+0];	cy1=iblky[z+1];
		cy2=iblky[z+8];	cy3=iblky[z+9];

		if(pimg)
		{
			z=((y*2+0)*(16*4)+(x*2+0))*4;
			cy0+=pimg[z+1]; cy1+=pimg[z+5];
			z=((y*2+1)*(16*4)+(x*2+0))*4;
			cy2+=pimg[z+1]; cy3+=pimg[z+5];
		}

		if((cy0|cy1|cy2|cy3)&clampmask)
		{	cy0=clamp(cy0);	cy1=clamp(cy1);
			cy2=clamp(cy2);	cy3=clamp(cy3);	}
		z=((y*2+0)*ostr+(x*2+0))*4;
		oimg[z+0]=cy0;		oimg[z+1]=cy0;
		oimg[z+2]=cy0;		oimg[z+3]=255;
		oimg[z+4]=cy1;		oimg[z+5]=cy1;
		oimg[z+6]=cy1;		oimg[z+7]=255;
		z=((y*2+1)*ostr+(x*2+0))*4;
		oimg[z+0]=cy2;		oimg[z+1]=cy2;
		oimg[z+2]=cy2;		oimg[z+3]=255;
		oimg[z+4]=cy3;		oimg[z+5]=cy3;
		oimg[z+6]=cy3;		oimg[z+7]=255;
	}
}

void TKuPI_TransIA4(u16 *oimg, int ostr, s32 *iblky,
	u16 *pimg)
{
	int (*clamp)(int x);
	int clampmask;
	int cy0, cy1, cy2, cy3;
	int x, y, z;
	
	clamp=tkupi_doclamp;
	clampmask=tkupi_clampmask;
	
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		z=(y*2*8)+(x*2);
		cy0=iblky[z+0];	cy1=iblky[z+1];
		cy2=iblky[z+8];	cy3=iblky[z+9];
		if((cy0|cy1|cy2|cy3)&clampmask)
		{	cy0=clamp(cy0);	cy1=clamp(cy1);
			cy2=clamp(cy2);	cy3=clamp(cy3);	}

		if(pimg)
		{
			z=((y*2+0)*(16*4)+(x*2+0))*4;
			cy0+=pimg[z+3];	cy1+=pimg[z+7];
			z=((y*2+1)*(16*4)+(x*2+0))*4;
			cy2+=pimg[z+3]; cy3+=pimg[z+7];
		}

		z=((y*2+0)*ostr+(x*2+0))*4;
		oimg[z+3]=cy0;	oimg[z+7]=cy1;
		z=((y*2+1)*ostr+(x*2+0))*4;
		oimg[z+3]=cy2;	oimg[z+7]=cy3;
	}
}

void TKuPI_TransIYUV444(u16 *oimg, int ostr,
	s32 *iblky, s32 *iblku, s32 *iblkv,
	u16 *pimg)
{
	int (*clamp)(int x);
	void (*dotrans)(
		int cy, int cu, int cv,
		int *rcr, int *rcg, int *rcb);
	int clampmask;
	int cy0, cy1, cy2, cy3;
	int cu0, cu1, cu2, cu3;
	int cv0, cv1, cv2, cv3;
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int cr3, cg3, cb3;
	int t0, t1, t2, t3;
	int tcr, tcg, tcb;
	int czr, czg, czb;
	int x, y, z;
	
	clamp=tkupi_doclamp;
	clampmask=tkupi_clampmask;
	dotrans=tkupi_doclrtrans;
	tcr=0; tcg=0; tcb=0;
	
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		z=(y*2*8)+(x*2);
		cy0=iblky[z+0];	cy1=iblky[z+1];
		cy2=iblky[z+8];	cy3=iblky[z+9];
		cu0=iblku[z+0];	cu1=iblku[z+1];
		cu2=iblku[z+8];	cu3=iblku[z+9];
		cv0=iblkv[z+0];	cv1=iblkv[z+1];
		cv2=iblkv[z+8];	cv3=iblkv[z+9];

		if(dotrans)
		{
			dotrans(cy0, cu0, cv0, &tcr, &tcg, &tcb);
			cr0=tcr;	cg0=tcg;	cb0=tcb;
			dotrans(cy1, cu1, cv1, &tcr, &tcg, &tcb);
			cr1=tcr;	cg1=tcg;	cb1=tcb;
			dotrans(cy2, cu2, cv2, &tcr, &tcg, &tcb);
			cr2=tcr;	cg2=tcg;	cb2=tcb;
			dotrans(cy3, cu3, cv3, &tcr, &tcg, &tcb);
			cr3=tcr;	cg3=tcg;	cb3=tcb;
		}else
		{
			cg0=cy0-((cu0+cv0)>>2);
			cg1=cy1-((cu1+cv1)>>2);
			cg2=cy2-((cu2+cv2)>>2);
			cg3=cy3-((cu3+cv3)>>2);
			cr0=cv0+cg0;	cr1=cv1+cg1;
			cr2=cv2+cg2;	cr3=cv3+cg3;
			cb0=cu0+cg0;	cb1=cu1+cg1;
			cb2=cu2+cg2;	cb3=cu3+cg3;
		}

		if(pimg)
		{
			z=((y*2+0)*(16*4)+(x*2+0))*4;
			cb0+=pimg[z+0]; cg0+=pimg[z+1]; cr0+=pimg[z+2];
			cb1+=pimg[z+4]; cg1+=pimg[z+5]; cr1+=pimg[z+6];
			z=((y*2+1)*(16*4)+(x*2+0))*4;
			cb2+=pimg[z+0]; cg2+=pimg[z+1]; cr2+=pimg[z+2];
			cb3+=pimg[z+4]; cg3+=pimg[z+5]; cr3+=pimg[z+6];
		}

		t0=cr0|cr1; t1=cr2|cr3;
		t2=cg0|cg1; t3=cg2|cg3;
		czr=t0|t1;
		t0=cb0|cb1; t1=cb2|cb3;
		czg=t2|t3;	czb=t0|t1;

		if(czr&clampmask)
		{	cr0=clamp(cr0);	cr1=clamp(cr1);
			cr2=clamp(cr2);	cr3=clamp(cr3);	}
		if(czg&clampmask)
		{	cg0=clamp(cg0);	cg1=clamp(cg1);
			cg2=clamp(cg2);	cg3=clamp(cg3);	}
		if(czb&clampmask)
		{	cb0=clamp(cb0);	cb1=clamp(cb1);
			cb2=clamp(cb2);	cb3=clamp(cb3);	}
		
		z=((y*2+0)*ostr+(x*2+0))*4;
		oimg[z+0]=cb0;		oimg[z+1]=cg0;
		oimg[z+2]=cr0;		oimg[z+3]=255;
		oimg[z+4]=cb1;		oimg[z+5]=cg1;
		oimg[z+6]=cr1;		oimg[z+7]=255;
		z=((y*2+1)*ostr+(x*2+0))*4;
		oimg[z+0]=cb2;		oimg[z+1]=cg2;
		oimg[z+2]=cr2;		oimg[z+3]=255;
		oimg[z+4]=cb3;		oimg[z+5]=cg3;
		oimg[z+6]=cr3;		oimg[z+7]=255;
	}
}

void TKuPI_CopyTranslateMacroBlock(TKuPI_DecState *ctx,
	u16 *oimg, int ostr,
	byte *pimg, int pixs, int piys, int pixfmt, int mvx, int mvy)
{
	byte *pcs1, *pcs1_e, *pcs1_er;
	u16 *ocs, *pcs2, *pcs2_e;
	int ostr4, pixs4;
	int mvbx, mvby;
	int pixsz, pixcn, pixsw_b, pixsw_r, kmax;
	int i, j, k;
	
	if(!pimg)
		return;
	
	pixsz=(pixfmt&1)?1:2;
	pixcn=(pixfmt&4)?3:4;
	pixsw_b=(pixfmt&8)?2:0;
	pixsw_r=pixsw_b^2;

	mvbx=ctx->img_mbcx*16+mvx;
	mvby=ctx->img_mbcy*16+mvy;
	if(mvbx<0)mvbx=0;
	if(mvby<0)mvby=0;
	if(mvbx>(pixs-16))mvbx=pixs-16;
	if(mvby>(piys-16))mvby=piys-16;
	
	if(pixsz==1)
	{
		ostr4=ostr*4;
		pixs4=pixs*pixcn;
		ocs=oimg;
		pcs1=pimg+(mvbx*pixcn)+(mvby*pixs4);
		pcs1_e=pimg+piys*pixs4;
		kmax=(pixs-mvbx)*pixcn;
		for(i=0; i<16; i++)
		{
//			memcpy(ocs, pcs, 16*4*2);

			if(pixcn==4)
			{
				for(j=0, k=0; j<64; j+=4, k+=4)
				{
//					if(k>=kmax)
//						k=-4;
					ocs[j+0]=pcs1[k+pixsw_b];
					ocs[j+1]=pcs1[k+1];
					ocs[j+2]=pcs1[k+pixsw_r];
					ocs[j+3]=pcs1[k+3];
				}
			}else
				if(pixcn==3)
			{
				for(j=0, k=0; j<64; j+=4, k+=3)
				{
//					if(k>=kmax)
//						k=-3;
					ocs[j+0]=pcs1[k+pixsw_b];
					ocs[j+1]=pcs1[k+1];
					ocs[j+2]=pcs1[k+pixsw_r];
					ocs[j+3]=255;
				}
			}

			ocs+=ostr4; pcs1+=pixs4;
//			if(pcs1>=pcs1_e)
//				pcs1-=pixs4;
		}
		
		return;
	}
	
	ostr4=ostr*4;
	pixs4=pixs*pixcn;
	ocs=oimg;
	pcs2=(u16 *)(pimg+(((mvbx*4)+(mvby*pixs4))*2));
	pcs2_e=((u16 *)pimg)+piys*pixs4;
	kmax=(pixs-mvbx)*pixcn;
	for(i=0; i<16; i++)
	{
		if(pixcn==4)
		{
			for(j=0, k=0; j<64; j+=4, k+=4)
			{
//				if(k>=kmax)
//					k=-4;
				ocs[j+0]=pcs2[k+pixsw_b];
				ocs[j+1]=pcs2[k+1];
				ocs[j+2]=pcs2[k+pixsw_r];
				ocs[j+3]=pcs2[k+3];
			}
		}else
			if(pixcn==3)
		{
			for(j=0, k=0; j<64; j+=4, k+=3)
			{
//				if(k>=kmax)
//					k=-3;
				ocs[j+0]=pcs2[k+pixsw_b];
				ocs[j+1]=pcs2[k+1];
				ocs[j+2]=pcs2[k+pixsw_r];
				ocs[j+3]=0x3C00;
			}
		}
//		memcpy(ocs, pcs2, 16*4*2);
		ocs+=ostr4; pcs2+=pixs4;
//		if(pcs2>=pcs2_e)
//			pcs2-=pixs4;
	}
}

void TKuPI_DecodeMacroBlockI(TKuPI_DecState *ctx,
	u16 *oimg, int ostr,
	byte *pimg, int pixs, int piys, int pixfmt)
{
//	static byte t_pcsa[8*8*4];
	u16 pmvblk[8*8*4];
	s32 blk_y0[64], blk_y1[64];
	s32 blk_y2[64], blk_y3[64];
	s32 blk_u0[64], blk_v0[64];
	s32 blk_u1[64], blk_v1[64];
	s32 blk_u2[64], blk_v2[64];
	s32 blk_u3[64], blk_v3[64];
	s32 blk_a0[64], blk_a1[64];
	s32 blk_a2[64], blk_a3[64];
	void (*TransIBH)(s32 *oblk, s32 *iblk);
	byte mbf;
//	byte *pcs;
	u16 *pcs2, *pcs2_y0, *pcs2_y1, *pcs2_y2, *pcs2_y3;
	int vtag, ztag, qsc, qsgn;
	int mvx, mvy, tpixs, tpiys;
	int x, y;
	
	TransIBH=TKuPI_TransIBH;
//	if(ctx->mbform&32)
	if((ctx->mbform&0xE0)==0x20)
		{ TransIBH=TKuPI_TransIWHT; }
	if((ctx->mbform&0xE0)==0x40)
		{ TransIBH=TKuPI_TransICDF53; }
	if((ctx->mbform&0xE0)==0x60)
		{ TransIBH=TKuPI_TransIDCT; }
	
	
	tkupi_doclamp=tkupi_clamp255;
	tkupi_clampmask=~255;
	if(ctx->ishdr)
	{
		tkupi_doclamp=tkupi_clamp64k_hdr;
		tkupi_clampmask=~32767;
	}
	
	tkupi_doclrtrans=NULL;
	if(((ctx->pixclrs>>5)&7)==1)
		{ tkupi_doclrtrans=TKuPI_ClrTrans_YCoCgR; }
	if(((ctx->pixclrs>>5)&7)==2)
		{ tkupi_doclrtrans=TKuPI_ClrTrans_ApxYUV; }
	
	ctx->qtsc=32;
	if(pimg)
	{
		TKuPI_ReadCoeffTag(ctx, &vtag, &ztag);
		qsc=ctx->pdc_qtsc+vtag;
		ctx->pdc_qtsc=qsc;
		qsc=(qsc>>1)^(((s32)(qsc<<31))>>31);
		qsgn=0;
		if(qsc<0)
			{ qsc=-qsc; qsgn=1; }
		ctx->qtsc=qsc;
		
		if(ztag==2)
		{
			mvx=TKuPI_ReadCoeffDc(ctx);
			mvy=TKuPI_ReadCoeffDc(ctx);
			mvx+=ctx->pdc_mvx;	mvy+=ctx->pdc_mvy;
			ctx->pdc_mvx=mvx;	ctx->pdc_mvy=mvy;
			
			if(qsgn)
			{
				/* Skip block, Copy and done. */
				TKuPI_CopyTranslateMacroBlock(ctx,
					oimg, ostr, pimg, pixs, piys, pixfmt,
					mvx, mvy);
				return;
			}
		}

	}
	
//	mbf=ctx->mbform;
	mbf=ctx->mbform&15;
	
	TKuPI_ReadBlockCoeffs(ctx, blk_y0, ctx->qtab_y, &(ctx->pdc_y));
	TKuPI_ReadBlockCoeffs(ctx, blk_y2, ctx->qtab_y, &(ctx->pdc_y));
	TKuPI_ReadBlockCoeffs(ctx, blk_y3, ctx->qtab_y, &(ctx->pdc_y));
	TKuPI_ReadBlockCoeffs(ctx, blk_y1, ctx->qtab_y, &(ctx->pdc_y));

	TransIBH(blk_y0, blk_y0);
	TransIBH(blk_y1, blk_y1);
	TransIBH(blk_y2, blk_y2);
	TransIBH(blk_y3, blk_y3);
	
	if(mbf==1)
	{
		TKuPI_ReadBlockCoeffs(ctx, blk_u0, ctx->qtab_uv, &(ctx->pdc_u));
		TKuPI_ReadBlockCoeffs(ctx, blk_v0, ctx->qtab_uv, &(ctx->pdc_v));
		TransIBH(blk_u0, blk_u0);
		TransIBH(blk_v0, blk_v0);
	}
	
	if((mbf==2) || (mbf==3))
	{
		TKuPI_ReadBlockCoeffs(ctx, blk_u0, ctx->qtab_uv, &(ctx->pdc_u));
		TKuPI_ReadBlockCoeffs(ctx, blk_u2, ctx->qtab_uv, &(ctx->pdc_u));
		TKuPI_ReadBlockCoeffs(ctx, blk_u3, ctx->qtab_uv, &(ctx->pdc_u));
		TKuPI_ReadBlockCoeffs(ctx, blk_u1, ctx->qtab_uv, &(ctx->pdc_u));

		TKuPI_ReadBlockCoeffs(ctx, blk_v0, ctx->qtab_uv, &(ctx->pdc_v));
		TKuPI_ReadBlockCoeffs(ctx, blk_v2, ctx->qtab_uv, &(ctx->pdc_v));
		TKuPI_ReadBlockCoeffs(ctx, blk_v3, ctx->qtab_uv, &(ctx->pdc_v));
		TKuPI_ReadBlockCoeffs(ctx, blk_v1, ctx->qtab_uv, &(ctx->pdc_v));

		TransIBH(blk_u0, blk_u0);
		TransIBH(blk_u1, blk_u1);
		TransIBH(blk_u2, blk_u2);
		TransIBH(blk_u3, blk_u3);

		TransIBH(blk_v0, blk_v0);
		TransIBH(blk_v1, blk_v1);
		TransIBH(blk_v2, blk_v2);
		TransIBH(blk_v3, blk_v3);
	}
	
	if(mbf==3)
	{
		TKuPI_ReadBlockCoeffs(ctx, blk_a0, ctx->qtab_y, &(ctx->pdc_a));
		TKuPI_ReadBlockCoeffs(ctx, blk_a2, ctx->qtab_y, &(ctx->pdc_a));
		TKuPI_ReadBlockCoeffs(ctx, blk_a3, ctx->qtab_y, &(ctx->pdc_a));
		TKuPI_ReadBlockCoeffs(ctx, blk_a1, ctx->qtab_y, &(ctx->pdc_a));

		TransIBH(blk_a0, blk_a0);
		TransIBH(blk_a1, blk_a1);
		TransIBH(blk_a2, blk_a2);
		TransIBH(blk_a3, blk_a3);
	}
	
	if(pimg)
	{
		TKuPI_CopyTranslateMacroBlock(ctx,
			pmvblk, 16, pimg, pixs, piys, pixfmt,
			mvx, mvy);			
		pcs2=pmvblk;
		pcs2_y0=pmvblk;				pcs2_y1=pmvblk+8*4;
		pcs2_y2=pmvblk+(8*16*4);	pcs2_y3=pmvblk+(8*16*4)+8*4;
		tpixs=16;
		tpiys=16;
	}else
	{
//		pcs=NULL;
		pcs2=NULL;
		pcs2_y0=NULL; pcs2_y1=NULL;
		pcs2_y2=NULL; pcs2_y3=NULL;
		tpixs=0;
		tpiys=0;
	}

	if(mbf==0)
	{
		TKuPI_TransIY4(oimg+(0*ostr+0)*4, ostr, blk_y0, pcs2_y0);
		TKuPI_TransIY4(oimg+(0*ostr+8)*4, ostr, blk_y1, pcs2_y1);
		TKuPI_TransIY4(oimg+(8*ostr+0)*4, ostr, blk_y2, pcs2_y2);
		TKuPI_TransIY4(oimg+(8*ostr+8)*4, ostr, blk_y3, pcs2_y3);
	}
	
	if(mbf==1)
	{
		TKuPI_TransIYUV420(oimg+(0*ostr+0)*4, ostr,
			blk_y0, blk_u0+ 0, blk_v0+ 0, pcs2_y0);
		TKuPI_TransIYUV420(oimg+(0*ostr+8)*4, ostr,
			blk_y1, blk_u0+ 4, blk_v0+ 4, pcs2_y1);
		TKuPI_TransIYUV420(oimg+(8*ostr+0)*4, ostr,
			blk_y2, blk_u0+32, blk_v0+32, pcs2_y2);
		TKuPI_TransIYUV420(oimg+(8*ostr+8)*4, ostr,
			blk_y3, blk_u0+36, blk_v0+36, pcs2_y3);
	}

	if((mbf==2) || (mbf==3))
	{
		TKuPI_TransIYUV444(oimg+(0*ostr+0)*4, ostr,
			blk_y0, blk_u0, blk_v0, pcs2_y0);
		TKuPI_TransIYUV444(oimg+(0*ostr+8)*4, ostr,
			blk_y1, blk_u1, blk_v1, pcs2_y1);
		TKuPI_TransIYUV444(oimg+(8*ostr+0)*4, ostr,
			blk_y2, blk_u2, blk_v2, pcs2_y2);
		TKuPI_TransIYUV444(oimg+(8*ostr+8)*4, ostr,
			blk_y3, blk_u3, blk_v3, pcs2_y3);
	}

	if(mbf==3)
	{
		TKuPI_TransIA4(oimg+(0*ostr+0)*4, ostr, blk_a0, pcs2_y0);
		TKuPI_TransIA4(oimg+(0*ostr+8)*4, ostr, blk_a1, pcs2_y1);
		TKuPI_TransIA4(oimg+(8*ostr+0)*4, ostr, blk_a2, pcs2_y2);
		TKuPI_TransIA4(oimg+(8*ostr+8)*4, ostr, blk_a3, pcs2_y3);
	}
}

void TKuPI_DecodeMacroBlock(TKuPI_DecState *ctx,
	byte *oimg, int ostr,
	byte *pimg, int pixs, int piys, int pixfmt)
{
	u16 toblk[16*16*4];
	u16 tiblk[16*16*4];
	u16 *tics, *toct2;
	byte *toct;
	u32 tv;
	int pixsz, pixcn;
	int x, y, zi, zo;

	pixsz=(pixfmt&1)?1:2;
	pixcn=(pixfmt&4)?3:4;

	if((pixsz==2) && (pixcn==4))
	{
		TKuPI_DecodeMacroBlockI(ctx,
			(u16 *)oimg, ostr,
			pimg, pixs, piys, pixfmt);
		return;
	}

	TKuPI_DecodeMacroBlockI(ctx, toblk, 16, pimg, pixs, piys, pixfmt);

	if(pixsz==1)
	{
		for(y=0; y<16; y++)
		{
			tics=toblk+y*16*4;
			toct=oimg+y*ostr*pixcn;
			for(x=0; x<16; x++)
			{
				toct[0]=tics[0];
				toct[1]=tics[1];
				toct[2]=tics[2];
				toct[3]=tics[3];
				toct+=pixcn;
				tics+=4;
			}
		}
	}else
	{
		for(y=0; y<16; y++)
		{
			tics=toblk+y*16*4;
			toct2=((u16 *)oimg)+y*ostr*pixcn;
			for(x=0; x<16; x++)
			{
				toct2[0]=tics[0];
				toct2[1]=tics[1];
				toct2[2]=tics[2];
				if(pixcn==4)
					toct2[3]=tics[3];
				toct2+=pixcn;
				tics+=4;
			}
		}
	}
}

void TKuPI_DecodeMacroBlockEdge(TKuPI_DecState *ctx,
	byte *oimg, byte *pimg,
	int ostr, int frxs, int frys, int pixfmt)
{
	byte tblk[16*16*4*2];
	int pixsz, pixcn;
	int x, y, zi, zo;

	pixsz=(pixfmt&1)?1:2;
	pixcn=(pixfmt&4)?3:4;

	if(pixsz==2)
	{
		TKuPI_DecodeMacroBlock(ctx, tblk, 16,
			pimg, ctx->img_xs, ctx->img_ys, 2);
		for(y=0; y<frys; y++)
			for(x=0; x<frxs; x++)
		{
			zi=y*16+x;	zo=y*ostr+x;
			((u64 *)oimg)[zo]=((u64 *)tblk)[zi];
		}
	}else if(pixsz==1)
	{
		TKuPI_DecodeMacroBlock(ctx, tblk, 16,
			pimg, ctx->img_xs, ctx->img_ys, 1);
		for(y=0; y<frys; y++)
			for(x=0; x<frxs; x++)
		{
			zi=y*16+x;	zo=y*ostr+x;
			((u32 *)oimg)[zo]=((u32 *)tblk)[zi];
		}
	}
}

void TKuPI_DecodeMacroBlockBCN(TKuPI_DecState *ctx,
	u64 *oimg,
	int ostr, u64 *pimg, int frxs, int frys, int pixfmt)
{
#ifdef TKUPI_HASBCNENC
	byte tblk[16*16*4*2];
	int pixsz, pixcn;
	int x, y, zi, zo;

	pixsz=(pixfmt&1)?1:2;
	pixcn=(pixfmt&4)?3:4;

	TKuPI_DecodeMacroBlock(ctx, tblk, 16,
		NULL, ctx->img_xs, ctx->img_ys, 1);
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		zi=(y*4)*16+(x*4);	zo=y*ostr+x;
		if(	(pixfmt==TKUPI_PIXFMT_BC3) ||
			(pixfmt==TKUPI_PIXFMT_BC3_MIP))
		{
			oimg[zo*2+0]=TKuPI_EncodeImageDDS_BlockBC4A(tblk+zi*4, 16*4);
			oimg[zo*2+1]=TKuPI_EncodeImageDDS_BlockDXT1(tblk+zi*4, 16*4, 1);
		}else
		{
			oimg[zo]=TKuPI_EncodeImageDDS_BlockDXT1(tblk+zi*4, 16*4, 0);
		}
	}
#endif
}

/* rzrz-rzrr */
static const byte tkupi_initperm_sprz3[8]={
	0x00, 0x04, 0x10, 0x14, 0x40, 0x44, 0x50, 0x54 };
static const byte tkupi_initperm_sprr5[32]={
	0x00, 0x01, 0x02, 0x03, 0x08, 0x09, 0x0A, 0x0B,
	0x20, 0x21, 0x22, 0x23, 0x28, 0x29, 0x2A, 0x2B,
	0x80, 0x81, 0x82, 0x83, 0x88, 0x89, 0x8A, 0x8B,
	0xA0, 0xA1, 0xA2, 0xA3, 0xA8, 0xA9, 0xAA, 0xAB };

int TKuPI_DecodeImageBufferI(TKuPI_DecState *ctx,
	byte *ibuf, int szibuf,
	byte *oimg, byte *pimg, int oxs, int oys, int pixfmt)
{
	byte *cs, *cse, *cs0, *cs1, *ncs, *datap, *ppcs;
	u16 *ppqt;
	u32 tg, sz;
	int cxs, cys, cx, cy, ofs, pixsz, pixcn, isbcn;
	int i, j, k;
	
	for(i=0; i<256; i++)
		ctx->cpt_dc[i]=i;

	for(i=0; i<8; i++)
		for(j=0; j<32; j++)
	{
		k=tkupi_initperm_sprz3[i]|tkupi_initperm_sprr5[j];
		ctx->cpt_ac[k]=(i<<5)|j;
	}
	
	TKuPI_InitRiceFastTab();
	
	pixsz=(pixfmt&1)?1:2;
	pixcn=(pixfmt&4)?3:4;
	
	ctx->mbform=1;
	ctx->ishdr=0;
	
	datap=NULL;
	cs=ibuf;
	cse=ibuf+szibuf;
	while(cs<cse)
	{
		tg=((u32 *)cs)[0];
		sz=~((u32 *)cs)[1];

		if(!tg)
			break;
		if(tg&0x80808080U)
			break;
		if(sz<8)
			break;

		ncs=cs+sz;
		cs0=cs+8;
		if(ncs>cse)
			break;

		if(tg==TKUPI_FCC_imgd)
		{
			datap=cs0;
			cs=ncs;
			continue;
		}
		
		if(tg==TKUPI_FCC_uphd)
		{
			ctx->img_xs=*(u16 *)(cs0+0);
			ctx->img_ys=*(u16 *)(cs0+2);
			ctx->mbform=cs0[4];
			ctx->pixclrs=cs0[5];
			cs=ncs;
			continue;
		}
				
		if(tg==TKUPI_FCC_qtab)
		{
			k=cs0[0];
			ppqt=NULL;
			if(k==1)
				ppqt=ctx->qtab_y;
			if(k==2)
				ppqt=ctx->qtab_uv;

			if(ppqt)
			{
				if(sz==(64+1+8))
				{
					for(i=0; i<64; i++)
						ppqt[i]=cs0[i+1];
				}

				if(sz==(128+2+8))
				{
					ctx->ishdr=1;
					for(i=0; i<64; i++)
						ppqt[i]=cs0[2+i*2+0]|(cs0[2+i*2+1]<<8);
				}
			}

//			if(k==1)
//				memcpy(ctx->qtab_y, cs0+1, 64);
//			if(k==2)
//				memcpy(ctx->qtab_uv, cs0+1, 64);
			cs=ncs;
			continue;
		}

		cs=ncs;
		continue;
	}
	
	if(datap && oimg)
	{
//		ctx->cs=datap;
//		ctx->pos=32;
//		TKuPI_SkipBits(ctx, 0);
		TKuPI_SetupReadBits(ctx, datap);
		
		ctx->pdc_y=0;
		ctx->pdc_u=0;
		ctx->pdc_v=0;
		ctx->pdc_a=0;

		ctx->pk_dc=4;
		ctx->pk_ac=4;
		
		isbcn=0;
		if(	(pixfmt==TKUPI_PIXFMT_BC1) ||
			(pixfmt==TKUPI_PIXFMT_BC3) ||
			(pixfmt==TKUPI_PIXFMT_BC1_MIP) ||
			(pixfmt==TKUPI_PIXFMT_BC3_MIP))
				isbcn=1;

		if(isbcn)
		{
			pixcn=1;
			if(	(pixfmt==TKUPI_PIXFMT_BC3) ||
				(pixfmt==TKUPI_PIXFMT_BC3_MIP)	)
				pixcn=2;
		
			cxs=(oxs+15)>>4;
			cys=(oys+15)>>4;
			for(cy=0; cy<cys; cy++)
			{
				for(cx=0; cx<cxs; cx++)
				{
					ctx->img_mbcy=cy;	ctx->img_mbcx=cx;
					ofs=((cy*4*(oxs/4))+(cx*4))*pixcn;
					TKuPI_DecodeMacroBlockBCN(ctx,
						((u64 *)oimg)+ofs, oxs/4,
						NULL, oxs, oys, pixfmt);
				}
			}

			if(	(pixfmt==TKUPI_PIXFMT_BC1) ||
				(pixfmt==TKUPI_PIXFMT_BC1_MIP)	)
			{
				TKuPI_EncodeImageDDS_FlipImageVertBC1(
					((u64 *)oimg), oxs, oys);
			}
			
			if(pixfmt==TKUPI_PIXFMT_BC1_MIP)
			{
				TKuPI_EncodeImageDDS_BuildMipsBC1(
					((u64 *)oimg), oxs, oys);
			}
			if(pixfmt==TKUPI_PIXFMT_BC3_MIP)
			{
				TKuPI_EncodeImageDDS_BuildMipsBC3(
					((u64 *)oimg), oxs, oys);
			}
		}else
		{
			cxs=oxs>>4;
			cys=oys>>4;
			for(cy=0; cy<cys; cy++)
			{
				for(cx=0; cx<cxs; cx++)
				{
					ctx->img_mbcy=cy;	ctx->img_mbcx=cx;
					ofs=((cy*16*oxs)+(cx*16))*pixcn;
					ppcs=pimg;
					if(pimg)ppcs+=ofs*pixsz;
					TKuPI_DecodeMacroBlock(ctx,
						oimg+ofs*pixsz, oxs,
						ppcs, oxs, oys, pixfmt);
				}
				if(oxs&15)
				{
					ctx->img_mbcy=cy;	ctx->img_mbcx=cx;
					ofs=((cy*16*oxs)+(cx*16))*pixcn;
					ppcs=pimg;
					if(pimg)ppcs+=ofs*pixsz;
					TKuPI_DecodeMacroBlockEdge(ctx,
						oimg+ofs*pixsz, ppcs, oxs, oxs&15, 16, pixfmt);
				}
			}
			if(oys&15)
			{
				for(cx=0; cx<cxs; cx++)
				{
					ctx->img_mbcy=cy;	ctx->img_mbcx=cx;
					ofs=((cy*16*oxs)+(cx*16))*pixcn;
					ppcs=pimg;
					if(pimg)ppcs+=ofs*pixsz;
					TKuPI_DecodeMacroBlockEdge(ctx,
						oimg+ofs*pixsz, ppcs, oxs,
						16, oys&15, pixfmt);
				}
				if(oxs&15)
				{
					ctx->img_mbcy=cy;	ctx->img_mbcx=cx;
					ofs=((cy*16*oxs)+(cx*16))*pixcn;
					ppcs=pimg;
					if(pimg)ppcs+=ofs*pixsz;
					TKuPI_DecodeMacroBlockEdge(ctx,
						oimg+ofs*pixsz, ppcs, oxs, oxs&15, oys&15, pixfmt);
				}
			}
		}
	}
	
	return(0);
}

int TKuPI_DecodeImageBuffer(TKuPI_DecState *ctx,
	byte *ibuf, int szibuf,
	byte *oimg, int oxs, int oys)
{
	return(TKuPI_DecodeImageBufferI(ctx,
		ibuf, szibuf,
		oimg, NULL, oxs, oys, TKUPI_PIXFMT_BGRA32));
}

int TKuPI_DecodeImageBufferH(TKuPI_DecState *ctx,
	byte *ibuf, int szibuf,
	u16 *oimg, int oxs, int oys)
{
	return(TKuPI_DecodeImageBufferI(ctx,
		ibuf, szibuf,
		(byte *)oimg, NULL, oxs, oys, TKUPI_PIXFMT_BGRA64));
}

#if 1
byte *TKuPI_DecodeImageTempBuffer(byte *ibuf, int *rxs, int *rys)
{
#if 1
	TKuPI_DecState t_ctx;
	TKuPI_DecState *ctx;
	byte *buf;
	int xs, ys, n, p0, p1;
	int i;
	
	ctx=&t_ctx;
	memset(ctx, 0, sizeof(TKuPI_DecState));

	TKuPI_DecodeImageBuffer(ctx, ibuf, 1<<24, NULL, 0, 0);
	
	xs=ctx->img_xs;
	ys=ctx->img_ys;
	
	if((xs<=0) || (ys<=0))
		return(NULL);
	
	buf=malloc(xs*ys*4);
	TKuPI_DecodeImageBuffer(ctx, ibuf, 1<<24, buf, xs, ys);
	
	*rxs=xs;
	*rys=ys;
	
	n=xs*ys;
	for(i=0; i<xs*ys; i++)
	{
		p0=buf[i*4+0];	p1=buf[i*4+2];
		buf[i*4+0]=p1;	buf[i*4+2]=p0;
	}
	return(buf);
#endif
}
#endif

#if 1
byte *TKuPI_DecodeImageTempBuffer_BC3MIP(byte *ibuf, int *rxs, int *rys)
{
	TKuPI_DecState t_ctx;
	TKuPI_DecState *ctx;
	byte *buf;
	int xs, ys, bxs, bys, n, p0, p1;
	int i;
	ctx=&t_ctx;
	memset(ctx, 0, sizeof(TKuPI_DecState));
	TKuPI_DecodeImageBuffer(ctx, ibuf, 1<<24, NULL, 0, 0);
	xs=ctx->img_xs;		ys=ctx->img_ys;
	if((xs<=0) || (ys<=0))
		return(NULL);
	bxs=(xs+3)/4;	bys=(ys+3)/4;
	buf=malloc((bxs*bys*2+4)*16);
	TKuPI_DecodeImageBufferI(ctx, ibuf, 1<<24,
		buf, NULL, xs, ys, TKUPI_PIXFMT_BC3_MIP);
	*rxs=xs;
	*rys=ys;
	return(buf);
}

byte *TKuPI_DecodeImageTempBuffer_BC1MIP(byte *ibuf, int *rxs, int *rys)
{
	TKuPI_DecState t_ctx;
	TKuPI_DecState *ctx;
	byte *buf;
	int xs, ys, bxs, bys, n, p0, p1;
	int i;
	ctx=&t_ctx;
	memset(ctx, 0, sizeof(TKuPI_DecState));
	TKuPI_DecodeImageBuffer(ctx, ibuf, 1<<24, NULL, 0, 0);
	xs=ctx->img_xs;		ys=ctx->img_ys;
	if((xs<=0) || (ys<=0))
		return(NULL);
	bxs=(xs+3)/4;	bys=(ys+3)/4;
	buf=malloc((bxs*bys*2+4)*8);
	TKuPI_DecodeImageBufferI(ctx, ibuf, 1<<24,
		buf, NULL, xs, ys, TKUPI_PIXFMT_BC1_MIP);
	*rxs=xs;
	*rys=ys;
	return(buf);
}
#endif
