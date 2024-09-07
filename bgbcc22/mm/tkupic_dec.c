#define BTMGL_FOURCC(a, b, c, d)	((a)|((b)<<8)|((c)<<16)|((d)<<24))

#define TKUPI_FCC_qtab	BTMGL_FOURCC('q', 't', 'a', 'b')
#define TKUPI_FCC_rkpt	BTMGL_FOURCC('r', 'k', 'p', 't')
#define TKUPI_FCC_imgd	BTMGL_FOURCC('i', 'm', 'g', 'd')
#define TKUPI_FCC_uphd	BTMGL_FOURCC('u', 'p', 'h', 'd')


// typedef struct TKuPI_DecState_s TKuPI_DecState;

struct TKuPI_DecState_s {
	byte *cs;
	u32 win;
	byte pos;
	byte status;
	
	byte pk_dc;
	byte pk_ac;
	
	byte mbform;	//macroblock format
	
	u16 img_xs;
	u16 img_ys;
	
	s16 pdc_y;
	s16 pdc_u;
	s16 pdc_v;
	s16 pdc_a;

	byte cpt_dc[256];
	byte cpt_ac[256];
	byte qtab_y[64];
	byte qtab_uv[64];
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

int TKuPI_ReadBlockCoeffs(TKuPI_DecState *ctx, s16 *blk, byte *qtab, s16 *rpdc)
{
	int b, pdc;
	int i, j, k, z, v, z0;

	memset(blk, 0, 64*sizeof(s16));

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

void TKuPI_TransIBH(s16 *oblk, s16 *iblk)
{
	s16 tblk[64];
	s16 *cs, *ct;

	s16 i0, i1, i2, i3, i4, i5, i6, i7;
	s16 s0, s1;
	s16 t0, t1, t2, t3;
	s16 u0, u1, u2, u3, u4, u5, u6, u7;
	
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

int tkupi_clamp255(int vi)
{
	int v;
	v=vi;
	if(vi<  0)	v=  0;
	if(vi>255)	v=255;
	return(v);
}

void TKuPI_TransIYUV420(byte *oimg, int ostr,
	s16 *iblky, s16 *iblku, s16 *iblkv)
{
	int cy0, cy1, cy2, cy3, cu, cv;
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int cr3, cg3, cb3;
	int t0, t1, t2, t3;
	int x, y, z, czr, czg, czb;
	
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		z=(y*2)*8+(x*2);
		cy0=iblky[z+0];		cy1=iblky[z+1];
		cy2=iblky[z+8];		cy3=iblky[z+9];
		z=y*8+x;
		cu=iblku[z];	cv=iblkv[z];

		z=((cu+cv)>>2);
		cg0=cy0-z;	cg1=cy1-z;
		cg2=cy2-z;	cg3=cy3-z;
		cr0=cv+cg0;	cr1=cv+cg1;
		cr2=cv+cg2;	cr3=cv+cg3;
		cb0=cu+cg0;	cb1=cu+cg1;
		cb2=cu+cg2;	cb3=cu+cg3;

		t0=cr0|cr1; t1=cr2|cr3;
		t2=cg0|cg1; t3=cg2|cg3;
		czr=t0|t1;
		t0=cb0|cb1; t1=cb2|cb3;
		czg=t2|t3;	czb=t0|t1;

		if(czr>>8)
		{	cr0=tkupi_clamp255(cr0);
			cr1=tkupi_clamp255(cr1);
			cr2=tkupi_clamp255(cr2);
			cr3=tkupi_clamp255(cr3);	}
		if(czg>>8)
		{	cg0=tkupi_clamp255(cg0);
			cg1=tkupi_clamp255(cg1);
			cg2=tkupi_clamp255(cg2);
			cg3=tkupi_clamp255(cg3);	}
		if(czb>>8)
		{	cb0=tkupi_clamp255(cb0);
			cb1=tkupi_clamp255(cb1);
			cb2=tkupi_clamp255(cb2);
			cb3=tkupi_clamp255(cb3);	}
		
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

void TKuPI_TransIY4(byte *oimg, int ostr,
	s16 *iblky)
{
	int cy0, cy1, cy2, cy3;
	int x, y, z;
	
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		z=(y*2*8)+(x*2);
		cy0=iblky[z+0];	cy1=iblky[z+1];
		cy2=iblky[z+8];	cy3=iblky[z+9];
		if((cy0|cy1|cy2|cy3)>>8)
		{	cy0=tkupi_clamp255(cy0);
			cy1=tkupi_clamp255(cy1);
			cy2=tkupi_clamp255(cy2);
			cy3=tkupi_clamp255(cy3);	}
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

void TKuPI_TransIA4(byte *oimg, int ostr, s16 *iblky)
{
	int cy0, cy1, cy2, cy3;
	int x, y, z;
	
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		z=(y*2*8)+(x*2);
		cy0=iblky[z+0];	cy1=iblky[z+1];
		cy2=iblky[z+8];	cy3=iblky[z+9];
		if((cy0|cy1|cy2|cy3)>>8)
		{	cy0=tkupi_clamp255(cy0);
			cy1=tkupi_clamp255(cy1);
			cy2=tkupi_clamp255(cy2);
			cy3=tkupi_clamp255(cy3);	}
		z=((y*2+0)*ostr+(x*2+0))*4;
		oimg[z+3]=cy0;	oimg[z+7]=cy1;
		z=((y*2+1)*ostr+(x*2+0))*4;
		oimg[z+3]=cy2;	oimg[z+7]=cy3;
	}
}

void TKuPI_TransIYUV444(byte *oimg, int ostr,
	s16 *iblky, s16 *iblku, s16 *iblkv)
{
	int cy0, cy1, cy2, cy3;
	int cu0, cu1, cu2, cu3;
	int cv0, cv1, cv2, cv3;
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int cr3, cg3, cb3;
	int t0, t1, t2, t3;
	int czr, czg, czb;
	int x, y, z;
	
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

		cg0=cy0-((cu0+cv0)>>2);
		cg1=cy1-((cu1+cv1)>>2);
		cg2=cy2-((cu2+cv2)>>2);
		cg3=cy3-((cu3+cv3)>>2);
		cr0=cv0+cg0;	cr1=cv1+cg1;
		cr2=cv2+cg2;	cr3=cv3+cg3;
		cb0=cu0+cg0;	cb1=cu1+cg1;
		cb2=cu2+cg2;	cb3=cu3+cg3;

		t0=cr0|cr1; t1=cr2|cr3;
		t2=cg0|cg1; t3=cg2|cg3;
		czr=t0|t1;
		t0=cb0|cb1; t1=cb2|cb3;
		czg=t2|t3;	czb=t0|t1;

		if(czr>>8)
		{	cr0=tkupi_clamp255(cr0);
			cr1=tkupi_clamp255(cr1);
			cr2=tkupi_clamp255(cr2);
			cr3=tkupi_clamp255(cr3);	}
		if(czg>>8)
		{	cg0=tkupi_clamp255(cg0);
			cg1=tkupi_clamp255(cg1);
			cg2=tkupi_clamp255(cg2);
			cg3=tkupi_clamp255(cg3);	}
		if(czb>>8)
		{	cb0=tkupi_clamp255(cb0);
			cb1=tkupi_clamp255(cb1);
			cb2=tkupi_clamp255(cb2);
			cb3=tkupi_clamp255(cb3);	}
		
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

void TKuPI_DecodeMacroBlock(TKuPI_DecState *ctx, byte *oimg, int ostr)
{
	s16 blk_y0[64], blk_y1[64];
	s16 blk_y2[64], blk_y3[64];
	s16 blk_u0[64], blk_v0[64];
	s16 blk_u1[64], blk_v1[64];
	s16 blk_u2[64], blk_v2[64];
	s16 blk_u3[64], blk_v3[64];
	s16 blk_a0[64], blk_a1[64];
	s16 blk_a2[64], blk_a3[64];
	byte mbf;
	
	mbf=ctx->mbform;
	
	TKuPI_ReadBlockCoeffs(ctx, blk_y0, ctx->qtab_y, &(ctx->pdc_y));
	TKuPI_ReadBlockCoeffs(ctx, blk_y2, ctx->qtab_y, &(ctx->pdc_y));
	TKuPI_ReadBlockCoeffs(ctx, blk_y3, ctx->qtab_y, &(ctx->pdc_y));
	TKuPI_ReadBlockCoeffs(ctx, blk_y1, ctx->qtab_y, &(ctx->pdc_y));

	TKuPI_TransIBH(blk_y0, blk_y0);
	TKuPI_TransIBH(blk_y1, blk_y1);
	TKuPI_TransIBH(blk_y2, blk_y2);
	TKuPI_TransIBH(blk_y3, blk_y3);
	
	if(mbf==1)
	{
		TKuPI_ReadBlockCoeffs(ctx, blk_u0, ctx->qtab_uv, &(ctx->pdc_u));
		TKuPI_ReadBlockCoeffs(ctx, blk_v0, ctx->qtab_uv, &(ctx->pdc_v));
		TKuPI_TransIBH(blk_u0, blk_u0);
		TKuPI_TransIBH(blk_v0, blk_v0);
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

		TKuPI_TransIBH(blk_u0, blk_u0);
		TKuPI_TransIBH(blk_u1, blk_u1);
		TKuPI_TransIBH(blk_u2, blk_u2);
		TKuPI_TransIBH(blk_u3, blk_u3);

		TKuPI_TransIBH(blk_v0, blk_v0);
		TKuPI_TransIBH(blk_v1, blk_v1);
		TKuPI_TransIBH(blk_v2, blk_v2);
		TKuPI_TransIBH(blk_v3, blk_v3);
	}
	
	if(mbf==3)
	{
		TKuPI_ReadBlockCoeffs(ctx, blk_a0, ctx->qtab_y, &(ctx->pdc_a));
		TKuPI_ReadBlockCoeffs(ctx, blk_a2, ctx->qtab_y, &(ctx->pdc_a));
		TKuPI_ReadBlockCoeffs(ctx, blk_a3, ctx->qtab_y, &(ctx->pdc_a));
		TKuPI_ReadBlockCoeffs(ctx, blk_a1, ctx->qtab_y, &(ctx->pdc_a));

		TKuPI_TransIBH(blk_a0, blk_a0);
		TKuPI_TransIBH(blk_a1, blk_a1);
		TKuPI_TransIBH(blk_a2, blk_a2);
		TKuPI_TransIBH(blk_a3, blk_a3);
	}
	
	if(mbf==0)
	{
		TKuPI_TransIY4(oimg+(0*ostr+0)*4, ostr, blk_y0);
		TKuPI_TransIY4(oimg+(0*ostr+8)*4, ostr, blk_y1);
		TKuPI_TransIY4(oimg+(8*ostr+0)*4, ostr, blk_y2);
		TKuPI_TransIY4(oimg+(8*ostr+8)*4, ostr, blk_y3);
	}
	
	if(mbf==1)
	{
		TKuPI_TransIYUV420(oimg+(0*ostr+0)*4, ostr,
			blk_y0, blk_u0+ 0, blk_v0+ 0);
		TKuPI_TransIYUV420(oimg+(0*ostr+8)*4, ostr,
			blk_y1, blk_u0+ 4, blk_v0+ 4);
		TKuPI_TransIYUV420(oimg+(8*ostr+0)*4, ostr,
			blk_y2, blk_u0+32, blk_v0+32);
		TKuPI_TransIYUV420(oimg+(8*ostr+8)*4, ostr,
			blk_y3, blk_u0+36, blk_v0+36);
	}

	if((mbf==2) || (mbf==3))
	{
		TKuPI_TransIYUV444(oimg+(0*ostr+0)*4, ostr,
			blk_y0, blk_u0, blk_v0);
		TKuPI_TransIYUV444(oimg+(0*ostr+8)*4, ostr,
			blk_y1, blk_u1, blk_v1);
		TKuPI_TransIYUV444(oimg+(8*ostr+0)*4, ostr,
			blk_y2, blk_u2, blk_v2);
		TKuPI_TransIYUV444(oimg+(8*ostr+8)*4, ostr,
			blk_y3, blk_u3, blk_v3);
	}

	if(mbf==3)
	{
		TKuPI_TransIA4(oimg+(0*ostr+0)*4, ostr, blk_a0);
		TKuPI_TransIA4(oimg+(0*ostr+8)*4, ostr, blk_a1);
		TKuPI_TransIA4(oimg+(8*ostr+0)*4, ostr, blk_a2);
		TKuPI_TransIA4(oimg+(8*ostr+8)*4, ostr, blk_a3);
	}
}

void TKuPI_DecodeMacroBlockEdge(TKuPI_DecState *ctx, byte *oimg,
	int ostr, int frxs, int frys)
{
	byte tblk[16*16*4];
	int x, y, zi, zo;

	TKuPI_DecodeMacroBlock(ctx, tblk, 16);
	for(y=0; y<frys; y++)
		for(x=0; x<frxs; x++)
	{
		zi=y*16+x;	zo=y*ostr+x;
		((u32 *)oimg)[zo]=((u32 *)tblk)[zi];
	}
}

/* rzrz-rzrr */
static const byte tkupi_initperm_sprz3[8]={
	0x00, 0x04, 0x10, 0x14, 0x40, 0x44, 0x50, 0x54 };
static const byte tkupi_initperm_sprr5[32]={
	0x00, 0x01, 0x02, 0x03, 0x08, 0x09, 0x0A, 0x0B,
	0x20, 0x21, 0x22, 0x23, 0x28, 0x29, 0x2A, 0x2B,
	0x80, 0x81, 0x82, 0x83, 0x88, 0x89, 0x8A, 0x8B,
	0xA0, 0xA1, 0xA2, 0xA3, 0xA8, 0xA9, 0xAA, 0xAB };

int TKuPI_DecodeImageBuffer(TKuPI_DecState *ctx,
	byte *ibuf, int szibuf,
	byte *oimg, int oxs, int oys)
{
	byte *cs, *cse, *cs0, *cs1, *ncs, *datap;
	u32 tg, sz;
	int cxs, cys, cx, cy;
	int i, j, k;
	
	for(i=0; i<256; i++)
		ctx->cpt_dc[i]=i;

	for(i=0; i<8; i++)
		for(j=0; j<32; j++)
	{
		k=tkupi_initperm_sprz3[i]|tkupi_initperm_sprr5[j];
		ctx->cpt_ac[k]=(i<<5)|j;
	}
	
	ctx->mbform=1;
	
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
			cs=ncs;
			continue;
		}
				
		if(tg==TKUPI_FCC_qtab)
		{
			k=cs0[0];
			if(k==1)
				memcpy(ctx->qtab_y, cs0+1, 64);
			if(k==2)
				memcpy(ctx->qtab_uv, cs0+1, 64);
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
		
		cxs=oxs>>4;
		cys=oys>>4;
		for(cy=0; cy<cys; cy++)
		{
			for(cx=0; cx<cxs; cx++)
			{
				TKuPI_DecodeMacroBlock(ctx,
					oimg+((cy*16*oxs)+(cx*16))*4, oxs);
			}
			if(oxs&15)
			{
				TKuPI_DecodeMacroBlockEdge(ctx,
					oimg+((cy*16*oxs)+(cx*16))*4, oxs, oxs&15, 16);
			}
		}
		if(oys&15)
		{
			for(cx=0; cx<cxs; cx++)
			{
				TKuPI_DecodeMacroBlockEdge(ctx,
					oimg+((cy*16*oxs)+(cx*16))*4, oxs,
					16, oys&15);
			}
			if(oxs&15)
			{
				TKuPI_DecodeMacroBlockEdge(ctx,
					oimg+((cy*16*oxs)+(cx*16))*4, oxs, oxs&15, oys&15);
			}
		}
	}
	
	return(0);
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
