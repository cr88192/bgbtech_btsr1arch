#define TKUPI_ENCFL_ENCAUTO		0x8000	//auto-select options

#define TKUPI_ENCFL_USEWHT		0x0400
#define TKUPI_ENCFL_USELGT53	0x0800
#define TKUPI_ENCFL_USEDCT		0x0C00
#define TKUPI_ENCFL_USEBHT		0x0200

#define TKUPI_ENCFL_USEYCOCG	0x1000
#define TKUPI_ENCFL_USEAPXYUV	0x2000
#define TKUPI_ENCFL_USERCT		0x4000

#define TKUPI_ENCFL_USETNS_MSK	0x0E00
#define TKUPI_ENCFL_USECLR_MSK	0x7000

typedef struct TKuPI_EncState_s TKuPI_EncState;

struct TKuPI_EncState_s {
	byte *ct;
	u32 win;
	byte pos;
	byte status;
	
	byte pk_dc;
	byte pk_ac;
	byte mbform;
	byte pixclrs;	//pixel format+colorspace
	byte ishdr;
	
	s32 pdc_y;
	s32 pdc_u;
	s32 pdc_v;
	s32 pdc_a;

	byte cpt_dc[256];
	byte cpt_ac[256];
	byte cpi_dc[256];
	byte cpi_ac[256];
	u16 qtab_y[64];
	u16 qtab_uv[64];
};

void TKuPI_WriteBits(TKuPI_EncState *ctx, int val, int bits)
{
	ctx->win|=(val&((1U<<bits)-1))<<ctx->pos;
	ctx->pos+=bits;
	while(ctx->pos>=8)
	{
		*ctx->ct++=ctx->win;
		ctx->win>>=8;
		ctx->pos-=8;
	}
}

void TKuPI_WriteRice(TKuPI_EncState *ctx, int val, int kf)
{
	int q;
	
	q=val>>kf;
	if(q>=8)
	{
		TKuPI_WriteBits(ctx, 0xFF, 8);
		TKuPI_WriteBits(ctx, val, 8);
		return;
	}

	TKuPI_WriteBits(ctx, (1<<q)-1, q+1);
	TKuPI_WriteBits(ctx, val&((1<<kf)-1), kf);
}

void TKuPI_WriteAdRice(TKuPI_EncState *ctx, int val, byte *rkf)
{
	int q, kf;
	
	kf=*rkf;
	q=val>>kf;

	if(q>=8)
	{
		kf++; *rkf=kf;
		TKuPI_WriteBits(ctx, 0xFF, 8);
		TKuPI_WriteBits(ctx, val, 8);
		return;
	}

	TKuPI_WriteBits(ctx, (1<<q)-1, q+1);
	TKuPI_WriteBits(ctx, val&((1<<kf)-1), kf);
	if(!q && kf)
		{ kf--; *rkf=kf; }
	else if(q>1)
		{ kf++; *rkf=kf; }
}

void TKuPI_WriteAdRiceSTF(TKuPI_EncState *ctx, int val,
	byte *rkf, byte *stfvtab, byte *stfitab)
{
	int i0, i1, v0, v1;
	
	i0=stfitab[val];
//	i1=(i0*7)>>3;
	i1=(i0*15)>>4;
	v0=stfvtab[i0];	v1=stfvtab[i1];
	stfvtab[i0]=v1;	stfvtab[i1]=v0;
	stfitab[v1]=i0;	stfitab[v0]=i1;
	
	TKuPI_WriteAdRice(ctx, i0, rkf);
}

void TKuPI_WriteCoeff(TKuPI_EncState *ctx,
	int v, int z,
	byte *rkf, byte *stfvtab, byte *stfitab)
{
	int av, tz, p, b, b1, h;
	
	tz=z;
	while(tz>=8)
	{
		TKuPI_WriteAdRiceSTF(ctx, 0xE0, rkf, stfvtab, stfitab);
		tz-=8;
	}
	
	av=(v<<1)^(v>>31);
	
	if(av<4)
	{
		p=(tz<<5)|av;
		TKuPI_WriteAdRiceSTF(ctx, p, rkf, stfvtab, stfitab);
		return;
	}
	
	if((av>=4096) && ctx->ishdr)
	{
		b=12;
		b1=b+1;
		p=1<<b1;
		while(p<=av)
		{
			b++;
			b1=b+1;
			p=1<<b1;
		}

		h=b;
		p=(tz<<5)|(b+12);
		TKuPI_WriteAdRiceSTF(ctx, p, rkf, stfvtab, stfitab);
		TKuPI_WriteBits(ctx, av&((1<<h)-1), h);
		return;
	}
	
	b=4;
	b1=b+1;
	h=(b1>>1)-1;
	p=(((2|b1)&3)<<h);
	while(p<=av)
	{
		b++;
		b1=b+1;
		h=(b1>>1)-1;
		p=(((2|b1)&3)<<h);
	}

	h=(b>>1)-1;
	p=(tz<<5)|b;
	TKuPI_WriteAdRiceSTF(ctx, p, rkf, stfvtab, stfitab);
	TKuPI_WriteBits(ctx, av&((1<<h)-1), h);
}

void TKuPI_WriteCoeffDc(TKuPI_EncState *ctx,
	int val)
{
	TKuPI_WriteCoeff(ctx, val, 0,
		&(ctx->pk_dc), ctx->cpt_dc, ctx->cpi_dc);
}

void TKuPI_WriteCoeffAc(TKuPI_EncState *ctx,
	int val, int zp)
{
	TKuPI_WriteCoeff(ctx, val, zp,
		&(ctx->pk_ac), ctx->cpt_ac, ctx->cpi_ac);
}

static const byte tkupi_zigzag[64]={
 0,  1,  5,  6, 14, 15, 27, 28,
 2,  4,  7, 13, 16, 26, 29, 42,
 3,  8, 12, 17, 25, 30, 41, 43,
 9, 11, 18, 24, 31, 40, 44, 53,
10, 19, 23, 32, 39, 45, 52, 54,
20, 22, 33, 38, 46, 51, 55, 60,
21, 34, 37, 47, 50, 56, 59, 61,
35, 36, 48, 49, 57, 58, 62, 63
};

int TKuPI_CalcQuantTrel(int dv, int q)
{
	int v0, v1, v2, v3;
	
	if(q==1)
		return(dv);
	
	v0=dv/q;
	v1=(dv+(q>>1))/q;

	if((v0==0) || (v0==1) || (v0==-1))
		return(0);
	if(v0==v1)
		return(v0);
	
	v2=(v0<<1)^(v0>>31);
	v3=(v1<<1)^(v1>>31);
	while((v2&v3)>=2)
		{ v2>>=1; v3>>=1; }
	if(v2==v3)
		return(v1);
	return(v0);
}

void TKuPI_WriteBlockCoeffs(TKuPI_EncState *ctx,
	s32 *blk, u16 *qtab, s32 *rpdc)
{
	int pdc, v, z;
	int i, j, k;
	
	pdc=*rpdc;
	v=blk[0];
//	v=(v-pdc)/qtab[0];
	v=TKuPI_CalcQuantTrel(v-pdc, qtab[0]);
	TKuPI_WriteCoeffDc(ctx, v);
	*rpdc=pdc+(v*qtab[0]);
	
//	printf("TKuPI_WriteBlockCoeffs: pdc=%d v=%d\n", pdc, v);
	
	z=0;
	for(i=1; i<64; i++)
	{
//		j=tkupi_zigzag[i];
		j=tkupi_izigzag[i];
//		v=blk[j]/qtab[j];
		v=TKuPI_CalcQuantTrel(blk[j], qtab[j]);
		
//		if(((v==1) || (v==-1)) && (qtab[j]>=5))
//			v=0;
		
		if(!v)
		{
			z++;
			continue;
		}
		TKuPI_WriteCoeffAc(ctx, v, z);
		z=0;
	}
	
	if(z)
	{
//		TKuPI_WriteCoeffAc(ctx, 0, z);
		TKuPI_WriteCoeffAc(ctx, 0, 0);
	}
}

void TKuPI_TransBH_Horz(s32 *oblk, s32 *iblk)
{
	s32 i0, i1, i2, i3, i4, i5, i6, i7;
	s32 s0, s1;
	s32 t0, t1, t2, t3;
	s32 u0, u1, u2, u3, u4, u5, u6, u7;
	
	i0=iblk[0];		i1=iblk[1];		i2=iblk[2];		i3=iblk[3];
	i4=iblk[4];		i5=iblk[5];		i6=iblk[6];		i7=iblk[7];

	u0=(i0+i1)>>1;
	u1=(i2+i3)>>1;
	u2=(i4+i5)>>1;
	u3=(i6+i7)>>1;
	u4=i0-i1;
	u5=i2-i3;
	u6=i4-i5;
	u7=i6-i7;

	t0=(u0+u1)>>1;
	t1=(u2+u3)>>1;
	t2=u0-u1;
	t3=u2-u3;
	
	s0=(t0+t1)>>1;
	s1=t0-t1;
	
	oblk[0]=s0;		oblk[1]=s1;		oblk[2]=t2;		oblk[3]=t3;
	oblk[4]=u4;		oblk[5]=u5;		oblk[6]=u6;		oblk[7]=u7;
}

void TKuPI_TransBH_Vert(s32 *oblk, s32 *iblk)
{
	s32 i0, i1, i2, i3, i4, i5, i6, i7;
	s32 s0, s1;
	s32 t0, t1, t2, t3;
	s32 u0, u1, u2, u3, u4, u5, u6, u7;
	
	i0=iblk[ 0];	i1=iblk[ 8];	i2=iblk[16];	i3=iblk[24];
	i4=iblk[32];	i5=iblk[40];	i6=iblk[48];	i7=iblk[56];

	u0=(i0+i1)>>1;
	u1=(i2+i3)>>1;
	u2=(i4+i5)>>1;
	u3=(i6+i7)>>1;
	u4=i0-i1;
	u5=i2-i3;
	u6=i4-i5;
	u7=i6-i7;

	t0=(u0+u1)>>1;
	t1=(u2+u3)>>1;
	t2=u0-u1;
	t3=u2-u3;
	
	s0=(t0+t1)>>1;
	s1=t0-t1;
	
	oblk[ 0]=s0;	oblk[ 8]=s1;	oblk[16]=t2;	oblk[24]=t3;
	oblk[32]=u4;	oblk[40]=u5;	oblk[48]=u6;	oblk[56]=u7;
}

void TKuPI_TransBH(s32 *oblk, s32 *iblk)
{
	s32 tblk[64];

	TKuPI_TransBH_Horz(tblk+ 0, iblk+ 0);
	TKuPI_TransBH_Horz(tblk+ 8, iblk+ 8);
	TKuPI_TransBH_Horz(tblk+16, iblk+16);
	TKuPI_TransBH_Horz(tblk+24, iblk+24);
	TKuPI_TransBH_Horz(tblk+32, iblk+32);
	TKuPI_TransBH_Horz(tblk+40, iblk+40);
	TKuPI_TransBH_Horz(tblk+48, iblk+48);
	TKuPI_TransBH_Horz(tblk+56, iblk+56);

	TKuPI_TransBH_Vert(oblk+ 0, tblk+ 0);
	TKuPI_TransBH_Vert(oblk+ 1, tblk+ 1);
	TKuPI_TransBH_Vert(oblk+ 2, tblk+ 2);
	TKuPI_TransBH_Vert(oblk+ 3, tblk+ 3);
	TKuPI_TransBH_Vert(oblk+ 4, tblk+ 4);
	TKuPI_TransBH_Vert(oblk+ 5, tblk+ 5);
	TKuPI_TransBH_Vert(oblk+ 6, tblk+ 6);
	TKuPI_TransBH_Vert(oblk+ 7, tblk+ 7);
}

void TKuPI_TransXBH_Horz(s32 *oblk, s32 *iblk)
{
	s32 i0, i1, i2, i3, i4, i5, i6, i7;
//	s32 s0, s1, s2, s3, s4, s5, s6, s7;
	s32 t0, t1, t2, t3, t4, t5, t6, t7;
	s32 u0, u1, u2, u3, u4, u5, u6, u7;
	
	i0=iblk[0];			i1=iblk[1];
	i2=iblk[2];			i3=iblk[3];
	i4=iblk[4];			i5=iblk[5];
	i6=iblk[6];			i7=iblk[7];

#if 1
	u0=i0+i2+i4+i6;		u1=i0+i2-i4-i6;
	u2=i0-i2-i4+i6;		u3=i0-i2+i4-i6;
	u4=i1+i3+i5+i7;		u5=i1+i3-i5-i7;
	u6=i1-i3-i5+i7;		u7=i1-i3+i5-i7;
	t0=(u0+u4)>>1;		t1=(u1+u5)>>1;
	t2=(u2+u6)>>1;		t3=(u3+u7)>>1;
	t4=u0-u4;			t5=u1-u5;
	t6=u2-u6;			t7=u3-u7;
	oblk[0]=t0;			oblk[1]=t1;
	oblk[2]=t2;			oblk[3]=t3;
	oblk[4]=t4;			oblk[5]=t5;
	oblk[6]=t6;			oblk[7]=t7;
#endif
}

void TKuPI_TransXBH_Vert(s32 *oblk, s32 *iblk)
{
	s32 i0, i1, i2, i3, i4, i5, i6, i7;
//	s32 s0, s1, s2, s3, s4, s5, s6, s7;
	s32 t0, t1, t2, t3, t4, t5, t6, t7;
	s32 u0, u1, u2, u3, u4, u5, u6, u7;
	
	i0=iblk[ 0];		i1=iblk[ 8];
	i2=iblk[16];		i3=iblk[24];
	i4=iblk[32];		i5=iblk[40];
	i6=iblk[48];		i7=iblk[56];

#if 1
	u0=i0+i2+i4+i6;		u1=i0+i2-i4-i6;
	u2=i0-i2-i4+i6;		u3=i0-i2+i4-i6;
	u4=i1+i3+i5+i7;		u5=i1+i3-i5-i7;
	u6=i1-i3-i5+i7;		u7=i1-i3+i5-i7;
	t0=(u0+u4)>>1;		t1=(u1+u5)>>1;
	t2=(u2+u6)>>1;		t3=(u3+u7)>>1;
	t4=u0-u4;			t5=u1-u5;
	t6=u2-u6;			t7=u3-u7;
	oblk[ 0]=t0;		oblk[ 8]=t1;
	oblk[16]=t2;		oblk[24]=t3;
	oblk[32]=t4;		oblk[40]=t5;
	oblk[48]=t6;		oblk[56]=t7;
#endif
}

void TKuPI_TransXBH(s32 *oblk, s32 *iblk)
{
	s32 tblk[64];

	TKuPI_TransXBH_Horz(tblk+ 0, iblk+ 0);
	TKuPI_TransXBH_Horz(tblk+ 8, iblk+ 8);
	TKuPI_TransXBH_Horz(tblk+16, iblk+16);
	TKuPI_TransXBH_Horz(tblk+24, iblk+24);
	TKuPI_TransXBH_Horz(tblk+32, iblk+32);
	TKuPI_TransXBH_Horz(tblk+40, iblk+40);
	TKuPI_TransXBH_Horz(tblk+48, iblk+48);
	TKuPI_TransXBH_Horz(tblk+56, iblk+56);

	TKuPI_TransXBH_Vert(oblk+ 0, tblk+ 0);
	TKuPI_TransXBH_Vert(oblk+ 1, tblk+ 1);
	TKuPI_TransXBH_Vert(oblk+ 2, tblk+ 2);
	TKuPI_TransXBH_Vert(oblk+ 3, tblk+ 3);
	TKuPI_TransXBH_Vert(oblk+ 4, tblk+ 4);
	TKuPI_TransXBH_Vert(oblk+ 5, tblk+ 5);
	TKuPI_TransXBH_Vert(oblk+ 6, tblk+ 6);
	TKuPI_TransXBH_Vert(oblk+ 7, tblk+ 7);
}


void TKuPI_TransWHT_Stride8(s32 *oblk, s32 *iblk, int str)
{
	s32 i0, i1, i2, i3, i4, i5, i6, i7;
	s32 s0, s1, s2, s3, s4, s5, s6, s7;
	s32 t0, t1, t2, t3, t4, t5, t6, t7;
	s32 u0, u1, u2, u3, u4, u5, u6, u7;
	
	i0=iblk[str*0];		i1=iblk[str*1];
	i2=iblk[str*2];		i3=iblk[str*3];
	i4=iblk[str*4];		i5=iblk[str*5];
	i6=iblk[str*6];		i7=iblk[str*7];

#if 1
	u0=(i0+i1)>>1;		u1=(i2+i3)>>1;
	u2=(i4+i5)>>1;		u3=(i6+i7)>>1;
	u4=i0-i1;			u5=i2-i3;
	u6=i4-i5;			u7=i6-i7;

	t0=(u0+u1)>>1;		t1=(u2+u3)>>1;
	t2=u0-u1;			t3=u2-u3;
	t4=(u4+u5)>>1;		t5=(u6+u7)>>1;
	t6=u4-u5;			t7=u6-u7;
	
	s0=(t0+t1)>>1;		s1=t0-t1;
	s2=(t2+t3)>>1;		s3=t2-t3;
	s4=(t4+t5)>>1;		s5=t4-t5;
	s6=(t6+t7)>>1;		s7=t6-t7;

	oblk[str*0]=s0;		oblk[str*1]=s1;
	oblk[str*2]=s2;		oblk[str*3]=s3;
	oblk[str*4]=s4;		oblk[str*5]=s5;
	oblk[str*6]=s6;		oblk[str*7]=s7;
#endif
}

void TKuPI_TransWHT_Horz(s32 *oblk, s32 *iblk)
	{ TKuPI_TransWHT_Stride8(oblk, iblk, 1); }
void TKuPI_TransWHT_Vert(s32 *oblk, s32 *iblk)
	{ TKuPI_TransWHT_Stride8(oblk, iblk, 8); }

void TKuPI_TransWHT(s32 *oblk, s32 *iblk)
{
	s32 tblk[64];

	TKuPI_TransWHT_Horz(tblk+ 0, iblk+ 0);
	TKuPI_TransWHT_Horz(tblk+ 8, iblk+ 8);
	TKuPI_TransWHT_Horz(tblk+16, iblk+16);
	TKuPI_TransWHT_Horz(tblk+24, iblk+24);
	TKuPI_TransWHT_Horz(tblk+32, iblk+32);
	TKuPI_TransWHT_Horz(tblk+40, iblk+40);
	TKuPI_TransWHT_Horz(tblk+48, iblk+48);
	TKuPI_TransWHT_Horz(tblk+56, iblk+56);

	TKuPI_TransWHT_Vert(oblk+ 0, tblk+ 0);
	TKuPI_TransWHT_Vert(oblk+ 1, tblk+ 1);
	TKuPI_TransWHT_Vert(oblk+ 2, tblk+ 2);
	TKuPI_TransWHT_Vert(oblk+ 3, tblk+ 3);
	TKuPI_TransWHT_Vert(oblk+ 4, tblk+ 4);
	TKuPI_TransWHT_Vert(oblk+ 5, tblk+ 5);
	TKuPI_TransWHT_Vert(oblk+ 6, tblk+ 6);
	TKuPI_TransWHT_Vert(oblk+ 7, tblk+ 7);
}

void TKuPI_TransCDF53_Stride8(s32 *oblk, s32 *iblk, int str)
{
	s32 i0, i1, i2, i3, i4, i5, i6, i7;
	s32 s0, s1, s2, s3, s4, s5, s6, s7;
	s32 t0, t1, t2, t3, t4, t5, t6, t7;
	s32 u0, u1, u2, u3, u4, u5, u6, u7;
	
	i0=iblk[str*0];			i1=iblk[str*1];
	i2=iblk[str*2];			i3=iblk[str*3];
	i4=iblk[str*4];			i5=iblk[str*5];
	i6=iblk[str*6];			i7=iblk[str*7];
	u0=i0;	u1=i2;			u2=i4;	u3=i6;
	u4=i1-((i0+i2)>>1);		u5=i3-((i2+i4)>>1);
	u6=i5-((i4+i6)>>1);		u7=i7-((i6+i0)>>1);
	t0=u0;					t1=u2;
	t2=u1-((u0+u2)>>1);		t3=u3-((u2+u0)>>1);
	s0=(t0+t1)>>1;			s1=(t1-t0);
	oblk[str*0]=s0;			oblk[str*1]=s1;
	oblk[str*2]=t2;			oblk[str*3]=t3;
	oblk[str*4]=u4;			oblk[str*5]=u5;
	oblk[str*6]=u6;			oblk[str*7]=u7;
}

void TKuPI_TransCDF53_Horz(s32 *oblk, s32 *iblk)
	{ TKuPI_TransCDF53_Stride8(oblk, iblk, 1); }
void TKuPI_TransCDF53_Vert(s32 *oblk, s32 *iblk)
	{ TKuPI_TransCDF53_Stride8(oblk, iblk, 8); }

void TKuPI_TransCDF53(s32 *oblk, s32 *iblk)
{
	s32 tblk[64];

	TKuPI_TransCDF53_Horz(tblk+ 0, iblk+ 0);
	TKuPI_TransCDF53_Horz(tblk+ 8, iblk+ 8);
	TKuPI_TransCDF53_Horz(tblk+16, iblk+16);
	TKuPI_TransCDF53_Horz(tblk+24, iblk+24);
	TKuPI_TransCDF53_Horz(tblk+32, iblk+32);
	TKuPI_TransCDF53_Horz(tblk+40, iblk+40);
	TKuPI_TransCDF53_Horz(tblk+48, iblk+48);
	TKuPI_TransCDF53_Horz(tblk+56, iblk+56);

	TKuPI_TransCDF53_Vert(oblk+ 0, tblk+ 0);
	TKuPI_TransCDF53_Vert(oblk+ 1, tblk+ 1);
	TKuPI_TransCDF53_Vert(oblk+ 2, tblk+ 2);
	TKuPI_TransCDF53_Vert(oblk+ 3, tblk+ 3);
	TKuPI_TransCDF53_Vert(oblk+ 4, tblk+ 4);
	TKuPI_TransCDF53_Vert(oblk+ 5, tblk+ 5);
	TKuPI_TransCDF53_Vert(oblk+ 6, tblk+ 6);
	TKuPI_TransCDF53_Vert(oblk+ 7, tblk+ 7);
}

void TKuPI_TransDCT_Stride(int *oblk, int *iblk, int str)
{
	int ib0, ib1, ib2, ib3, ib4, ib5, ib6, ib7;
	int ib0p7, ib1p6, ib2p5, ib3p4;
	int ib0n7, ib1n6, ib2n5, ib3n4;
	int ib0p7n3n4, ib1p6n2n5;
	ib0=iblk[str*0];	ib1=iblk[str*1];
	ib2=iblk[str*2];	ib3=iblk[str*3];
	ib4=iblk[str*4];	ib5=iblk[str*5];
	ib6=iblk[str*6];	ib7=iblk[str*7];
	ib0p7=ib0+ib7;	ib1p6=ib1+ib6;
	ib2p5=ib2+ib5;	ib3p4=ib3+ib4;
	ib0n7=ib0-ib7;	ib1n6=ib1-ib6;
	ib2n5=ib2-ib5;	ib3n4=ib3-ib4;
	oblk[str*0]=(ib0p7+ib1p6+ib2p5+ib3p4)*91;
	oblk[str*4]=(ib0p7-ib1p6-ib2p5+ib3p4)*91;
	ib0p7n3n4=ib0p7-ib3p4;
	ib1p6n2n5=ib1p6-ib2p5;
	oblk[str*2]=ib0p7n3n4*118 +ib1p6n2n5* 49;
	oblk[str*6]=ib0p7n3n4* 49 -ib1p6n2n5*118;
	oblk[str*1]=ib0n7*126 +ib1n6*106 +ib2n5* 71 +ib3n4* 25;
	oblk[str*3]=ib0n7*106 -ib1n6* 25 -ib2n5*126 -ib3n4* 71;
	oblk[str*5]=ib0n7* 71 -ib1n6*126 +ib2n5* 25 +ib3n4*106;
	oblk[str*7]=ib0n7* 25 -ib1n6* 71 +ib2n5*106 -ib3n4*126;
}
void TKuPI_TransDCT(s32 *oblk, s32 *iblk)
{
	s32 tblk[64], t2blk[64];
	int i;
	for(i=0; i<8; i++)
		TKuPI_TransDCT_Stride(tblk+i*8, iblk+i*8, 1);
	for(i=0; i<8; i++)
		TKuPI_TransDCT_Stride(t2blk+i, tblk+i, 8);
	for(i=0; i<64; i++)
	{
//		oblk[i]=t2blk[i]>>16;
		oblk[i]=(t2blk[i]+32768)>>16;
	}
}


int tkupi_infold_basic(int v)
	{ return(v); }

int tkupi_infold_fp16(int v)
{
	int v1;
	
	v1=v;
	if(v1>=0x8000)
		{ v1=(s16)(v^0x7FFF); }
	return(v1);
}

int (*tkupi_infold)(int val);

void (*tkupi_eclrtrans)(
	int cr, int cg, int cb,
	int *rcy, int *rcu, int *rcv);

void TKuPI_EClrTrans_ApxYUV(
	int cr, int cg, int cb,
	int *rcy, int *rcu, int *rcv)
{
	int cy, cu, cv, t;
	cy=(4*cg+3*cr+cb)>>3;
	cu=cb-cy;	cv=cr-cy;

	cy=cg+((3*cv+cu)>>2);
	cu=cb-cy;	cv=cr-cy;

	cy=cg+((3*cv+cu)>>2);
	cu=cb-cy;	cv=cr-cy;

	*rcy=cy;
	*rcu=cu;
	*rcv=cv;
}

void TKuPI_EClrTrans_YCoCgR(
	int cr, int cg, int cb,
	int *rcy, int *rcu, int *rcv)
{
	int cy, cu, cv, t;
	cu=cr-cb;
	t=cb+(cu>>1);
	cv=cg-t;
	cy=t+(cv>>1);
	*rcy=cy;
	*rcu=cu;
	*rcv=cv;
}

void TKuPI_TransYUV420(u16 *img, int str,
	s32 *blky, s32 *blku, s32 *blkv)
{
	int (*fold)(int val);
	void (*clrtrans)(
		int cr, int cg, int cb,
		int *rcy, int *rcu, int *rcv);
	int cy0, cy1, cy2, cy3;
	int cy, cu, cv, sw;
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int cr3, cg3, cb3;
	int cu0, cu1, cu2, cu3;
	int cv0, cv1, cv2, cv3;
	int t0, t1, t2, t3;
	int x, y, z;

	fold=tkupi_infold;
	clrtrans=tkupi_eclrtrans;

//	sw=2;
	sw=0;
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		z=((y*2+0)*str+(x*2+0))*4;
		cb0=fold(img[z+(0^sw)]);	cg0=fold(img[z+1]);
		cb1=fold(img[z+(4^sw)]);	cg1=fold(img[z+5]);
		cr0=fold(img[z+(2^sw)]);	cr1=fold(img[z+(6^sw)]);
		z=((y*2+1)*str+(x*2+0))*4;
		cb2=fold(img[z+(0^sw)]);	cg2=fold(img[z+1]);
		cb3=fold(img[z+(4^sw)]);	cg3=fold(img[z+5]);
		cr2=fold(img[z+(2^sw)]);	cr3=fold(img[z+(6^sw)]);

		if(clrtrans)
		{
			clrtrans(cr0, cg0, cb0, &cy0, &cu0, &cv0);
			clrtrans(cr1, cg1, cb1, &cy1, &cu1, &cv1);
			clrtrans(cr2, cg2, cb2, &cy2, &cu2, &cv2);
			clrtrans(cr3, cg3, cb3, &cy3, &cu3, &cv3);
			cu=(cu0+cu1+cu2+cu3)/4;
			cv=(cv0+cv1+cv2+cv3)/4;
		}else
		{
#if 1
			cy0=(2*cg0+cr0+cb0)/4;
			cy1=(2*cg1+cr1+cb1)/4;
			cy2=(2*cg2+cr2+cb2)/4;
			cy3=(2*cg3+cr3+cb3)/4;

			cy=(cg0+cg1+cg2+cg3);
	//		cy=(cy0+cy1+cy2+cy3);
			cu=(cb0+cb1+cb2+cb3-cy)/4;
			cv=(cr0+cr1+cr2+cr3-cy)/4;
#endif
		}
		
//		cu=128;
//		cu=0;
//		cv=0;

		blky[(y*2+0)*8+(x*2+0)]=cy0;
		blky[(y*2+0)*8+(x*2+1)]=cy1;
		blky[(y*2+1)*8+(x*2+0)]=cy2;
		blky[(y*2+1)*8+(x*2+1)]=cy3;
		blku[y*8+x]=cu;
		blkv[y*8+x]=cv;
	}
}

void TKuPI_TransYUV444(u16 *img, int str,
	s32 *blky, s32 *blku, s32 *blkv)
{
	int (*fold)(int val);
	void (*clrtrans)(
		int cr, int cg, int cb,
		int *rcy, int *rcu, int *rcv);
	int cy0, cy1, cy2, cy3;
	int cy, cu, cv, sw;
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int cr3, cg3, cb3;
	int cu0, cu1, cu2, cu3;
	int cv0, cv1, cv2, cv3;
	int t0, t1, t2, t3;
	int x, y, z;

	fold=tkupi_infold;
	clrtrans=tkupi_eclrtrans;

//	sw=2;
	sw=0;
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		z=((y*2+0)*str+(x*2+0))*4;
		cb0=fold(img[z+(0^sw)]);	cg0=fold(img[z+1]);
		cb1=fold(img[z+(4^sw)]);	cg1=fold(img[z+5]);
		cr0=fold(img[z+(2^sw)]);	cr1=fold(img[z+(6^sw)]);
		z=((y*2+1)*str+(x*2+0))*4;
		cb2=fold(img[z+(0^sw)]);	cg2=fold(img[z+1]);
		cb3=fold(img[z+(4^sw)]);	cg3=fold(img[z+5]);
		cr2=fold(img[z+(2^sw)]);	cr3=fold(img[z+(6^sw)]);

		if(clrtrans)
		{
			clrtrans(cr0, cg0, cb0, &cy0, &cu0, &cv0);
			clrtrans(cr1, cg1, cb1, &cy1, &cu1, &cv1);
			clrtrans(cr2, cg2, cb2, &cy2, &cu2, &cv2);
			clrtrans(cr3, cg3, cb3, &cy3, &cu3, &cv3);
		}else
		{
#if 1
			cy0=(2*cg0+cr0+cb0)/4;
			cy1=(2*cg1+cr1+cb1)/4;
			cy2=(2*cg2+cr2+cb2)/4;
			cy3=(2*cg3+cr3+cb3)/4;
			cu0=cb0-cg0;	cv0=cr0-cg0;
			cu1=cb1-cg1;	cv1=cr1-cg1;
			cu2=cb2-cg2;	cv2=cr2-cg2;
			cu3=cb3-cg3;	cv3=cr3-cg3;
#endif
		}

		blky[(y*2+0)*8+(x*2+0)]=cy0;
		blky[(y*2+0)*8+(x*2+1)]=cy1;
		blky[(y*2+1)*8+(x*2+0)]=cy2;
		blky[(y*2+1)*8+(x*2+1)]=cy3;

		blku[(y*2+0)*8+(x*2+0)]=cu0;
		blku[(y*2+0)*8+(x*2+1)]=cu1;
		blku[(y*2+1)*8+(x*2+0)]=cu2;
		blku[(y*2+1)*8+(x*2+1)]=cu3;

		blkv[(y*2+0)*8+(x*2+0)]=cv0;
		blkv[(y*2+0)*8+(x*2+1)]=cv1;
		blkv[(y*2+1)*8+(x*2+0)]=cv2;
		blkv[(y*2+1)*8+(x*2+1)]=cv3;
	}
}

void TKuPI_TransY4(u16 *img, int str, s32 *blky)
{
	int (*fold)(int val);
	int cy0, cy1, cy2, cy3;
	int cy, cu, cv, sw;
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int cr3, cg3, cb3;
	int x, y, z;

	fold=tkupi_infold;

//	sw=2;
	sw=0;
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		z=((y*2+0)*str+(x*2+0))*4;
		cb0=fold(img[z+(0^sw)]);	cg0=fold(img[z+1]);
		cb1=fold(img[z+(4^sw)]);	cg1=fold(img[z+5]);
		cr0=fold(img[z+(2^sw)]);	cr1=fold(img[z+(6^sw)]);
		z=((y*2+1)*str+(x*2+0))*4;
		cb2=fold(img[z+(0^sw)]);	cg2=fold(img[z+1]);
		cb3=fold(img[z+(4^sw)]);	cg3=fold(img[z+5]);
		cr2=fold(img[z+(2^sw)]);	cr3=fold(img[z+(6^sw)]);

		cy0=(2*cg0+cr0+cb0)/4;
		cy1=(2*cg1+cr1+cb1)/4;
		cy2=(2*cg2+cr2+cb2)/4;
		cy3=(2*cg3+cr3+cb3)/4;

		blky[(y*2+0)*8+(x*2+0)]=cy0;
		blky[(y*2+0)*8+(x*2+1)]=cy1;
		blky[(y*2+1)*8+(x*2+0)]=cy2;
		blky[(y*2+1)*8+(x*2+1)]=cy3;
	}
}

void TKuPI_TransA4(u16 *img, int str, s32 *blky)
{
	int (*fold)(int val);
	int cy0, cy1, cy2, cy3;
	int x, y, z;

	fold=tkupi_infold;

	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		z=((y*2+0)*str+(x*2+0))*4;
		cy0=fold(img[z+3]);		cy1=fold(img[z+7]);
		z=((y*2+1)*str+(x*2+0))*4;
		cy2=fold(img[z+3]);		cy3=fold(img[z+7]);
		z=(y*2*8)+(x*2);
		blky[z+0]=cy0;	blky[z+1]=cy1;
		blky[z+8]=cy2;	blky[z+9]=cy3;
	}
}


void TKuPI_EncodeMacroBlockI(TKuPI_EncState *ctx, u16 *img, int str)
{
	s32 blk_y0[64], blk_y1[64];
	s32 blk_y2[64], blk_y3[64];
	s32 blk_u0[64], blk_v0[64];	
	s32 blk_u1[64], blk_v1[64];	
	s32 blk_u2[64], blk_v2[64];	
	s32 blk_u3[64], blk_v3[64];	
	s32 blk_a0[64], blk_a1[64];
	s32 blk_a2[64], blk_a3[64];
	void (*TransBH)(s32 *oblk, s32 *iblk);

	int mbf;
	
	mbf=ctx->mbform&15;

	TransBH=TKuPI_TransBH;
	if((ctx->mbform&0x60)==0x20)
	{
//		TransBH=TKuPI_TransXBH;
		TransBH=TKuPI_TransWHT;
//		TransBH=TKuPI_TransCDF53;
	}

	if((ctx->mbform&0x60)==0x40)
		{ TransBH=TKuPI_TransCDF53; }
	if((ctx->mbform&0x60)==0x60)
		{ TransBH=TKuPI_TransDCT; }
	
	if(mbf==0)
	{
		TKuPI_TransY4(img+(0*str+0)*4, str, blk_y0);
		TKuPI_TransY4(img+(0*str+8)*4, str, blk_y1);
		TKuPI_TransY4(img+(8*str+0)*4, str, blk_y2);
		TKuPI_TransY4(img+(8*str+8)*4, str, blk_y3);
	}

	if(mbf==1)
	{
		TKuPI_TransYUV420(img+(0*str+0)*4, str, blk_y0, blk_u0+ 0, blk_v0+ 0);
		TKuPI_TransYUV420(img+(0*str+8)*4, str, blk_y1, blk_u0+ 4, blk_v0+ 4);
		TKuPI_TransYUV420(img+(8*str+0)*4, str, blk_y2, blk_u0+32, blk_v0+32);
		TKuPI_TransYUV420(img+(8*str+8)*4, str, blk_y3, blk_u0+36, blk_v0+36);
	}

	if((mbf==2) || (mbf==3))
	{
		TKuPI_TransYUV444(img+(0*str+0)*4, str, blk_y0, blk_u0, blk_v0);
		TKuPI_TransYUV444(img+(0*str+8)*4, str, blk_y1, blk_u1, blk_v1);
		TKuPI_TransYUV444(img+(8*str+0)*4, str, blk_y2, blk_u2, blk_v2);
		TKuPI_TransYUV444(img+(8*str+8)*4, str, blk_y3, blk_u3, blk_v3);
	}

	if(mbf==3)
	{
		TKuPI_TransA4(img+(0*str+0)*4, str, blk_a0);
		TKuPI_TransA4(img+(0*str+8)*4, str, blk_a1);
		TKuPI_TransA4(img+(8*str+0)*4, str, blk_a2);
		TKuPI_TransA4(img+(8*str+8)*4, str, blk_a3);
	}

	TransBH(blk_y0, blk_y0);
	TransBH(blk_y1, blk_y1);
	TransBH(blk_y2, blk_y2);
	TransBH(blk_y3, blk_y3);

	if(mbf==1)
	{
		TransBH(blk_u0, blk_u0);
		TransBH(blk_v0, blk_v0);
	}

	if((mbf==2) || (mbf==3))
	{
		TransBH(blk_u0, blk_u0);
		TransBH(blk_u1, blk_u1);
		TransBH(blk_u2, blk_u2);
		TransBH(blk_u3, blk_u3);

		TransBH(blk_v0, blk_v0);
		TransBH(blk_v1, blk_v1);
		TransBH(blk_v2, blk_v2);
		TransBH(blk_v3, blk_v3);
	}
	
	if(mbf==3)
	{
		TransBH(blk_a0, blk_a0);
		TransBH(blk_a1, blk_a1);
		TransBH(blk_a2, blk_a2);
		TransBH(blk_a3, blk_a3);
	}

	TKuPI_WriteBlockCoeffs(ctx, blk_y0, ctx->qtab_y, &(ctx->pdc_y));
	TKuPI_WriteBlockCoeffs(ctx, blk_y2, ctx->qtab_y, &(ctx->pdc_y));
	TKuPI_WriteBlockCoeffs(ctx, blk_y3, ctx->qtab_y, &(ctx->pdc_y));
	TKuPI_WriteBlockCoeffs(ctx, blk_y1, ctx->qtab_y, &(ctx->pdc_y));

	if(mbf==1)
	{
		TKuPI_WriteBlockCoeffs(ctx, blk_u0, ctx->qtab_uv, &(ctx->pdc_u));
		TKuPI_WriteBlockCoeffs(ctx, blk_v0, ctx->qtab_uv, &(ctx->pdc_v));
	}

	if((mbf==2) || (mbf==3))
	{
		TKuPI_WriteBlockCoeffs(ctx, blk_u0, ctx->qtab_uv, &(ctx->pdc_u));
		TKuPI_WriteBlockCoeffs(ctx, blk_u2, ctx->qtab_uv, &(ctx->pdc_u));
		TKuPI_WriteBlockCoeffs(ctx, blk_u3, ctx->qtab_uv, &(ctx->pdc_u));
		TKuPI_WriteBlockCoeffs(ctx, blk_u1, ctx->qtab_uv, &(ctx->pdc_u));

		TKuPI_WriteBlockCoeffs(ctx, blk_v0, ctx->qtab_uv, &(ctx->pdc_v));
		TKuPI_WriteBlockCoeffs(ctx, blk_v2, ctx->qtab_uv, &(ctx->pdc_v));
		TKuPI_WriteBlockCoeffs(ctx, blk_v3, ctx->qtab_uv, &(ctx->pdc_v));
		TKuPI_WriteBlockCoeffs(ctx, blk_v1, ctx->qtab_uv, &(ctx->pdc_v));
	}
	
	if(mbf==3)
	{
		TKuPI_WriteBlockCoeffs(ctx, blk_a0, ctx->qtab_y, &(ctx->pdc_a));
		TKuPI_WriteBlockCoeffs(ctx, blk_a2, ctx->qtab_y, &(ctx->pdc_a));
		TKuPI_WriteBlockCoeffs(ctx, blk_a3, ctx->qtab_y, &(ctx->pdc_a));
		TKuPI_WriteBlockCoeffs(ctx, blk_a1, ctx->qtab_y, &(ctx->pdc_a));
	}
}

void TKuPI_EncodeMacroBlockEdge(TKuPI_EncState *ctx,
	byte *img, int str, int frxs, int frys, int pixfmt)
{
	u16 tblk[16*16*4];
	byte *pcs1;
	u16 *pct, *pcs2;
	int pixsz, pixcn;
	int y, x, y1, x0, x1, x1l;

	pixsz=(pixfmt&1)?1:2;
	pixcn=(pixfmt&4)?3:4;

	tkupi_infold=tkupi_infold_basic;
	if(pixsz==2)
		tkupi_infold=tkupi_infold_fp16;

	tkupi_eclrtrans=NULL;
	if(((ctx->pixclrs>>5)&7)==1)
		{ tkupi_eclrtrans=TKuPI_EClrTrans_YCoCgR; }
	if(((ctx->pixclrs>>5)&7)==2)
		{ tkupi_eclrtrans=TKuPI_EClrTrans_ApxYUV; }

	x1l=frxs*pixcn;
	
	if(pixsz==1)
	{
		for(y=0; y<16; y++)
		{
			y1=y;
			if(y1>=frys)
				y1=frys-1;
		
			pcs1=img+(y1*str*4);
			pct=tblk+y*16*4;
			if(pixcn==4)
			{
				for(x0=0, x1=0; x0<64; x0+=4, x1+=4)
				{
					if(x1>=x1l)
						x1-=4;
					pct[x0+0]=pcs1[x1+0];
					pct[x0+1]=pcs1[x1+1];
					pct[x0+2]=pcs1[x1+2];
					pct[x0+3]=pcs1[x1+3];
				}
			}else
				if(pixcn==3)
			{
				for(x0=0, x1=0; x0<64; x0+=4, x1+=3)
				{
					if(x1>=x1l)
						x1-=3;
					pct[x0+0]=pcs1[x1+0];
					pct[x0+1]=pcs1[x1+1];
					pct[x0+2]=pcs1[x1+2];
					pct[x0+3]=255;
				}
			}
		}
	}else
		if(pixsz==2)
	{
		for(y=0; y<16; y++)
		{
			y1=y;
			if(y1>=frys)
				y1=frys-1;
		
			pcs2=((u16 *)img)+(y1*str*4);
			pct=tblk+y*16*4;
			if(pixcn==4)
			{
				for(x0=0, x1=0; x0<64; x0+=4, x1+=4)
				{
					if(x1>=x1l)
						x1-=4;
					pct[x0+0]=pcs2[x1+0];
					pct[x0+1]=pcs2[x1+1];
					pct[x0+2]=pcs2[x1+2];
					pct[x0+3]=pcs2[x1+3];
				}
			}else
				if(pixcn==3)
			{
				for(x0=0, x1=0; x0<64; x0+=4, x1+=3)
				{
					if(x1>=x1l)
						x1-=3;
					pct[x0+0]=pcs2[x1+0];
					pct[x0+1]=pcs2[x1+1];
					pct[x0+2]=pcs2[x1+2];
					pct[x0+3]=0x3C00;
				}
			}
		}
	}
	
	TKuPI_EncodeMacroBlockI(ctx, tblk, 16);
}

void TKuPI_EncodeMacroBlock(TKuPI_EncState *ctx,
	byte *img, int str, int pixfmt)
{
	TKuPI_EncodeMacroBlockEdge(ctx, img, str, 16, 16, pixfmt);
}

#if 0
// void TKuPI_EncodeMacroBlockEdge(TKuPI_EncState *ctx, byte *img,
	int str, int frxs, int frys)
{
	byte tblk[16*16*4];
	u32 px;
	int x, y, zi, zo;

	for(y=0; y<frys; y++)
	{
		for(x=0; x<frxs; x++)
		{
			zi=y*16+x;	zo=y*str+x;
			px=((u32 *)img)[zo];
			((u32 *)tblk)[zi]=px;
		}
		for(x=frxs; x<16; x++)
		{
			zi=y*16+x;	
			((u32 *)tblk)[zi]=px;
		}
	}
	for(y=frys; y<16; y++)
	{
		for(x=0; x<16; x++)
		{
			zi=y*16+x;	
			((u32 *)tblk)[zi]=px;
		}
	}
	TKuPI_EncodeMacroBlock(ctx, tblk, 16);
}
#endif


int TKuPI_EncodeImageBuffer1I(TKuPI_EncState *ctx,
	byte *ibuf, int szibuf,
	byte *oimg, int oxs, int oys, int qfl, int pixfmt)
{
	static byte qtab_xbh[8] = {16, 12, 10, 16, 24, 40, 48, 64};
	static byte qtab_bht[8] = {4, 8, 16, 16, 26, 26, 26, 26};
	byte *ct, *ct0;
	u32 tg, sz;
	float qsc;
	int pixsz, pixcn, pixstr, qfmax, usexbh, usecdf, usedct;
	int cxs, cys, cx, cy;
	int i, j, k, l, am;

	pixsz=(pixfmt&1)?1:2;
	pixcn=(pixfmt&4)?3:4;
	pixstr=pixsz*pixcn;
	usexbh=0;
//	usexbh=1;
	usedct=0;

//	usexbh=(qfl&1);
	usexbh=(qfl&1024);
	usecdf=(qfl&2048);

	if(usexbh && usecdf)
	{
		usexbh=0;
		usecdf=0;
		usedct=1;
	}
	
	ctx->pixclrs=0;
	if((qfl&0x3000)==0x1000)
		ctx->pixclrs=0x20;
	if((qfl&0x3000)==0x2000)
		ctx->pixclrs=0x40;

	if((qfl&127)>=100)
	{
		am=0; l=oxs*oys;
		for(i=0; i<l; i++)
			if(oimg[i*4+3]!=255)
		{
			am=1;
			break;
		}

		for(i=0; i<64; i++)
		{
			ctx->qtab_y[i]=1;
			ctx->qtab_uv[i]=1;
		}

		ctx->mbform=2;

		if(am)
			ctx->mbform=3;
	}else
	{
		k=100-(qfl&127);
		qsc=k*0.25;
//		qsc=k*0.33;
		qfmax=255;
		
		qsc=pow(qsc, 1.25);

		if(pixsz==2)
		{
			qsc*=8;
			qfmax=4095;
		}

		am=0; l=oxs*oys;
		for(i=0; i<l; i++)
			if(oimg[i*4+3]<(255-qsc))
				am=1;

		if(usecdf)
			qsc*=0.75;
		if(usexbh)
			qsc*=0.87;

		for(cy=0; cy<8; cy++)
		{
			for(cx=0; cx<8; cx++)
			{
//				k=1.0+((cx+1)/4.0)*((cy+1)/4.0)*13;
//				k=1.0+sqrt(((cx+1)/4.0)*((cy+1)/4.0))*qsc;
				k=1.0+pow(((cx+1)/4.0)*((cy+1)/4.0), 0.667)*qsc;

				k=1.0+pow((qtab_bht[cx]/16.0)*(qtab_bht[cy]/16.0), 0.667)*qsc;

//				if(usexbh)
//				if(usexbh || usedct)
				if(usedct)
				{
//					k=1.0+pow(((cx+1)/4.0)*((cy+1)/4.0), 0.500)*4.5*qsc;
//					k=1.0+pow(((cx+1)/4.0)*((cy+1)/4.0), 0.375)*4.0*qsc;
//					k=1.0+pow(((cx+1)/4.0)*((cy+1)/4.0), 0.25)*4.0*qsc;
//					k=1.0+3.5*qsc;
//					k=1.0+
//						pow((qtab_xbh[cx]/14.0)*(qtab_xbh[cy]/14.0), 0.500)*
//							2.5*qsc;

//					k=1.0+
//						pow((qtab_xbh[cx]/14.0)*(qtab_xbh[cy]/14.0), 0.667)*
//							2.25*qsc;
//					k=1.0+
//						pow((qtab_xbh[cx]/13.0)*(qtab_xbh[cy]/13.0), 0.584)*
//							2.15*qsc;

					k=1.0+
						pow((qtab_xbh[cx]/13.0)*(qtab_xbh[cy]/13.0), 0.667)*
							0.50*qsc;
				}
				
				if(k>qfmax)
					k=qfmax;
				i=cy*8+cx;
				ctx->qtab_y[i]=k;
				ctx->qtab_uv[i]=k;
				if(!(qfl&0x100))
					printf("%3d ", k);
			}
			if(!(qfl&0x100))
				printf("\n");
		}
		
		ctx->mbform=1;
		if((qfl&127)>95)
			ctx->mbform=2;

	//	ctx->mbform=2;
	//	ctx->mbform=0;
		if(am)
			ctx->mbform=3;
	}
	
	if(usexbh)
		ctx->mbform|=0x20;
	if(usecdf)
		ctx->mbform|=0x40;
	if(usedct)
		ctx->mbform|=0x60;

	for(i=0; i<256; i++)
	{
		ctx->cpt_dc[i]=i;
		ctx->cpi_dc[i]=i;

		ctx->cpt_ac[i]=i;
		ctx->cpi_ac[i]=i;
	}

#if 1
	for(i=0; i<8; i++)
		for(j=0; j<32; j++)
	{
		k=tkupi_initperm_sprz3[i]|tkupi_initperm_sprr5[j];
		l=(i<<5)|j;
		ctx->cpt_ac[k]=l;
		ctx->cpi_ac[l]=k;
	}
#endif

	ctx->pk_dc=4;
	ctx->pk_ac=4;

	ct=ibuf;

	if(pixfmt==TKUPI_PIXFMT_BGRA32_F8)
		ctx->pixclrs=(ctx->pixclrs&(~15))|1;
	if(pixsz==2)
		ctx->pixclrs=(ctx->pixclrs&(~15))|2;

	i=32;
	((u32 *)ct)[0]=TKUPI_FCC_uphd;
	((u32 *)ct)[1]=~i;
	memset(ct+8, 0, 24);
	*(u16 *)(ct+ 8)=oxs;
	*(u16 *)(ct+10)=oys;
	*(byte *)(ct+12)=ctx->mbform;
	*(byte *)(ct+13)=ctx->pixclrs;
//	if(pixfmt==TKUPI_PIXFMT_BGRA32_F8)
//		*(byte *)(ct+13)=1;
//	if(pixsz==2)
//		*(byte *)(ct+13)=2;
	ct+=i;


	i=8+1+64;
	if(pixsz==2)
		i=8+2+64*2;
	((u32 *)ct)[0]=TKUPI_FCC_qtab;
	((u32 *)ct)[1]=~i;
	ct[8]=1;
//	memcpy(ct+9, ctx->qtab_y, 64);
	if(pixsz==2)
	{
		for(j=0; j<64; j++)
		{
			ct[10+j*2+0]=ctx->qtab_y[j];
			ct[10+j*2+1]=ctx->qtab_y[j]>>8;
		}
	}else
	{
		for(j=0; j<64; j++)
			ct[9+j]=ctx->qtab_y[j];
	}
	ct+=i;

	i=8+1+64;
	if(pixsz==2)
		i=8+2+64*2;
	((u32 *)ct)[0]=TKUPI_FCC_qtab;
	((u32 *)ct)[1]=~i;
	ct[8]=2;
//	memcpy(ct+9, ctx->qtab_uv, 64);
	if(pixsz==2)
	{
		for(j=0; j<64; j++)
		{
			ct[10+j*2+0]=ctx->qtab_uv[j];
			ct[10+j*2+1]=ctx->qtab_uv[j]>>8;
		}
	}else
	{
		for(j=0; j<64; j++)
			ct[9+j]=ctx->qtab_uv[j];
	}
	ct+=i;


	ct0=ct;
	((u32 *)ct)[0]=TKUPI_FCC_imgd;
	ct+=8;
	
	ctx->ct=ct;
	ctx->pos=0;
	ctx->win=0;
		
	ctx->pdc_y=0;
	ctx->pdc_u=0;
	ctx->pdc_v=0;
	ctx->pdc_a=0;
	
	cxs=oxs>>4;
	cys=oys>>4;
	for(cy=0; cy<cys; cy++)
	{
		for(cx=0; cx<cxs; cx++)
		{
			TKuPI_EncodeMacroBlock(ctx,
				oimg+((cy*16*oxs)+(cx*16))*pixstr, oxs, pixfmt);
		}
		if(oxs&15)
		{
			TKuPI_EncodeMacroBlockEdge(ctx,
				oimg+((cy*16*oxs)+(cx*16))*pixstr, oxs, oxs&15, 16, pixfmt);
		}
	}
	if(oys&15)
	{
		for(cx=0; cx<cxs; cx++)
		{
			TKuPI_EncodeMacroBlockEdge(ctx,
				oimg+((cy*16*oxs)+(cx*16))*pixstr, oxs,
				16, oys&15, pixfmt);
		}
		if(oxs&15)
		{
			TKuPI_EncodeMacroBlockEdge(ctx,
				oimg+((cy*16*oxs)+(cx*16))*pixstr, oxs, oxs&15, oys&15, pixfmt);
		}
	}
	
	TKuPI_WriteBits(ctx, 0, 32);
	
	ct=ctx->ct;
	i=ct-ct0;
	((u32 *)ct0)[1]=~i;
	
	((u32 *)ct)[0]=0;
	((u32 *)ct)[1]=0;
	
	return(ct-ibuf);
}

double TKuPI_EncCalcMseQbpp(byte *i1buf, byte *i2buf, int xs, int ys, int sz)
{
	double ce, bpp, ebpp;
	int dr, dg, db, da;
	int i, n;

	n=xs*ys;
	ce=0;
	for(i=0; i<n; i++)
	{
		db=i1buf[i*4+0]-i2buf[i*4+0];
		dg=i1buf[i*4+1]-i2buf[i*4+1];
		dr=i1buf[i*4+2]-i2buf[i*4+2];
		da=i1buf[i*4+3]-i2buf[i*4+3];
		ce+=dr*dr+dg*dg+db*db+da*da;
	}
//	ce/=n;
	ce=sqrt(ce/n);
	bpp=((8.0*sz)/n);
	printf("TKuPI_EncCalcMseQbpp: MSE=%f Bpp=%f\n", ce, bpp);
	ebpp=(ce+1.0)*bpp;
	return(ebpp);
}

int TKuPI_EncodeImageBufferI(TKuPI_EncState *ctx,
	byte *ibuf, int szibuf,
	byte *oimg, int oxs, int oys, int qfl, int pixfmt)
{
	static byte *tb_cbuf, *tb_obuf;
	static int sz_obuf;
	TKuPI_DecState t_dctx;
	TKuPI_DecState *dctx;
	char *sn_btns, *sn_clrs;
	double bqe, qe;
	int sz, sz1, sz2, sz3, n, npsz, ncsz, pixsz, qfl1, bqfl;
	int i, j, k;

	if(!(qfl&TKUPI_ENCFL_ENCAUTO))
	{
		sz=TKuPI_EncodeImageBuffer1I(ctx,
			ibuf, szibuf, oimg, oxs, oys, qfl, pixfmt);
		return(sz);
	}

	dctx=&t_dctx;
	memset(dctx, 0, sizeof(TKuPI_DecState));
	
	pixsz=(pixfmt&1)?1:2;
	n=oxs*oys*pixsz;
	npsz=n*4;
	ncsz=1024+n*6;
	
	if(tb_obuf && (n>sz_obuf))
	{
		free(tb_cbuf);
		free(tb_obuf);
		tb_cbuf=NULL;
		tb_obuf=NULL;
	}
	
	if(!tb_obuf)
	{
		tb_cbuf=malloc(ncsz);
		tb_obuf=malloc(npsz);
		sz_obuf=n;
	}
	
	sz=ncsz;
	bqe=999999.0;
	bqfl=0;
	
	for(i=0; i<3; i++)
		for(j=0; j<4; j++)
	{
		qfl1=qfl;
		
		if(i && (qfl&TKUPI_ENCFL_USECLR_MSK))
			continue;
		if(j && (qfl&TKUPI_ENCFL_USETNS_MSK))
			continue;
		if((qfl&127)>=97)
		{
			if(i==2)	continue;
			if(j==3)	continue;
		}

		sn_clrs="RCT";
		sn_btns="BHT";
		
		if(i==1)
			{ qfl1|=TKUPI_ENCFL_USEYCOCG; sn_clrs="YCoCg"; }
		if(i==2)
			{ qfl1|=TKUPI_ENCFL_USEAPXYUV; sn_clrs="ApxYUV"; }

		if(j==1)
			{ qfl1|=TKUPI_ENCFL_USELGT53; sn_btns="LGT53"; }
		if(j==2)
			{ qfl1|=TKUPI_ENCFL_USEWHT; sn_btns="WHT"; }
		if(j==3)
			{ qfl1|=TKUPI_ENCFL_USEDCT; sn_btns="DCT"; }
		
		sz1=TKuPI_EncodeImageBuffer1I(ctx,
			tb_cbuf, szibuf, oimg, oxs, oys, qfl1, pixfmt);
		if(sz1<=0)
			continue;
		TKuPI_DecodeImageBufferI(dctx,
			tb_cbuf, sz1,
			tb_obuf, NULL, oxs, oys, pixfmt);
		qe=TKuPI_EncCalcMseQbpp(oimg, tb_obuf, oxs, oys, sz1);

		printf("TKuPI_EncodeImageBufferI: ev qfl=%04X %s-%s sz=%d %f\n",
			qfl1, sn_btns, sn_clrs, sz1, qe);

		if(qe<bqe)
		{
			memcpy(ibuf, tb_cbuf, sz1);
			sz=sz1;
			bqe=qe;
			bqfl=qfl1;
		}
	}

	sn_clrs="RCT";
	if((bqfl&TKUPI_ENCFL_USECLR_MSK)==TKUPI_ENCFL_USEYCOCG)
		sn_clrs="YCoCg";
	if((bqfl&TKUPI_ENCFL_USECLR_MSK)==TKUPI_ENCFL_USEAPXYUV)
		sn_clrs="ApxYUV";

	sn_btns="BHT";
	if((bqfl&TKUPI_ENCFL_USETNS_MSK)==TKUPI_ENCFL_USEWHT)
		sn_btns="WHT";
	if((bqfl&TKUPI_ENCFL_USETNS_MSK)==TKUPI_ENCFL_USELGT53)
		sn_btns="LGT53";
	if((bqfl&TKUPI_ENCFL_USETNS_MSK)==TKUPI_ENCFL_USEDCT)
		sn_btns="DCT";
	
//	char *sn_btns, *sn_clrs;

	printf("TKuPI_EncodeImageBufferI: bqfl=%04X %s-%s\n",
		bqfl, sn_btns, sn_clrs);
	
	return(sz);
}

int TKuPI_EncodeImageBuffer(TKuPI_EncState *ctx,
	byte *ibuf, int szibuf,
	byte *oimg, int oxs, int oys, int qfl)
{
	return(TKuPI_EncodeImageBufferI(ctx, ibuf, szibuf,
		oimg, oxs, oys, qfl, TKUPI_PIXFMT_BGRA32));
}

int TKuPI_EncodeImageBufferH(TKuPI_EncState *ctx,
	byte *ibuf, int szibuf,
	byte *oimg, int oxs, int oys, int qfl)
{
	return(TKuPI_EncodeImageBufferI(ctx, ibuf, szibuf,
		oimg, oxs, oys, qfl, TKUPI_PIXFMT_BGRA64));
}

int TKuPI_EncodeImageBufferTemp(
	byte *ibuf,
	byte *oimg, int oxs, int oys, int qfl)
{
	TKuPI_EncState t_ctx;
	TKuPI_EncState *ctx;
	byte *tibuf;
	int sz, n;
	int i;
	
	tibuf=oimg;
	if(qfl&256)
	{
		n=oxs*oys;
		tibuf=malloc(n*4);
		for(i=0; i<n; i++)
		{
			tibuf[i*4+0]=oimg[i*4+2];
			tibuf[i*4+1]=oimg[i*4+1];
			tibuf[i*4+2]=oimg[i*4+0];
			tibuf[i*4+3]=oimg[i*4+3];
		}
	}
	
	ctx=&t_ctx;
	memset(ctx, 0, sizeof(TKuPI_EncState));
	
	sz=TKuPI_EncodeImageBuffer(ctx, ibuf, oxs*oys*6, tibuf, oxs, oys, qfl);
	
	if(tibuf!=oimg)
		free(tibuf);

	return(sz);
}
