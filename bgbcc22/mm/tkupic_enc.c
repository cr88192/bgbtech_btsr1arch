// typedef struct TKuPI_EncState_s TKuPI_EncState;

struct TKuPI_EncState_s {
	byte *ct;
	u32 win;
	byte pos;
	byte status;
	
	byte pk_dc;
	byte pk_ac;
	byte mbform;
	byte pixsw;
	
	s16 pdc_y;
	s16 pdc_u;
	s16 pdc_v;
	s16 pdc_a;

	byte cpt_dc[256];
	byte cpt_ac[256];
	byte cpi_dc[256];
	byte cpi_ac[256];
	byte qtab_y[64];
	byte qtab_uv[64];
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
	s16 *blk, byte *qtab, s16 *rpdc)
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

void TKuPI_TransBH_Horz(s16 *oblk, s16 *iblk)
{
	s16 i0, i1, i2, i3, i4, i5, i6, i7;
	s16 s0, s1;
	s16 t0, t1, t2, t3;
	s16 u0, u1, u2, u3, u4, u5, u6, u7;
	
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

void TKuPI_TransBH_Vert(s16 *oblk, s16 *iblk)
{
	s16 i0, i1, i2, i3, i4, i5, i6, i7;
	s16 s0, s1;
	s16 t0, t1, t2, t3;
	s16 u0, u1, u2, u3, u4, u5, u6, u7;
	
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

void TKuPI_TransBH(s16 *oblk, s16 *iblk)
{
	s16 tblk[64];

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

void TKuPI_TransYUV420(byte *img, int str,
	s16 *blky, s16 *blku, s16 *blkv)
{
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

//	sw=2;
	sw=0;
	
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		z=((y*2+0)*str+(x*2+0))*4;
		cb0=img[z+(0^sw)];		cg0=img[z+1];
		cb1=img[z+(4^sw)];		cg1=img[z+5];
		cr0=img[z+(2^sw)];		cr1=img[z+(6^sw)];
		z=((y*2+1)*str+(x*2+0))*4;
		cb2=img[z+(0^sw)];		cg2=img[z+1];
		cb3=img[z+(4^sw)];		cg3=img[z+5];
		cr2=img[z+(2^sw)];		cr3=img[z+(6^sw)];

#if 0
		cu0=cr0-cb0;		cu1=cr1-cb1;
		cu2=cr2-cb2;		cu3=cr3-cb3;
		t0=(cr0+cb0)>>1;	t1=(cr1+cb1)>>1;
		t2=(cr2+cb2)>>1;	t3=(cr3+cb3)>>1;
		cv0=cg0-t0;			cv1=cg1-t1;
		cv2=cg2-t2;			cv3=cg3-t3;
		cy0=(cg0+t0)>>1;	cy1=(cg1+t1)>>1;
		cy2=(cg2+t2)>>1;	cy3=(cg3+t3)>>1;
		cu=(cu0+cu1+cu2+cu3)/4;
		cv=(cv0+cv1+cv2+cv3)/4;
#endif

#if 0
		cu0=cr0-cb0;		cu1=cr1-cb1;
		cu2=cr2-cb2;		cu3=cr3-cb3;
		t0=cb0+(cu0>>1);	t1=cb1+(cu1>>1);
		t2=cb2+(cu2>>1);	t3=cb3+(cu3>>1);
		cv0=cg0-t0;			cv1=cg1-t1;
		cv2=cg2-t2;			cv3=cg3-t3;
		cy0=t0+(cg0>>1);	cy1=t1+(cg1>>1);
		cy2=t2+(cg2>>1);	cy3=t3+(cg3>>1);
		cu=(cu0+cu1+cu2+cu3)/4;
		cv=(cv0+cv1+cv2+cv3)/4;
#endif

#if 0
		cy0=cg0+((cr0-cb0)>>2);
		cy1=cg1+((cr1-cb1)>>2);
		cy2=cg2+((cr2-cb2)>>2);
		cy3=cg3+((cr3-cb3)>>2);
		
		cy=(cy0+cy1+cy2+cy3)/4;
		cu=(cb0+cb1+cb2+cb3-4*cy)/4;
		cv=(cr0+cr1+cr2+cr3-4*cy)/4;
#endif

#if 0
		cy0=(4*cg0+3*cr0+cb0)/8;
		cy1=(4*cg1+3*cr1+cb1)/8;
		cy2=(4*cg2+3*cr2+cb2)/8;
		cy3=(4*cg3+3*cr3+cb3)/8;
		
		cy=(cy0+cy1+cy2+cy3)/4;
		cu=(cb0+cb1+cb2+cb3-4*cy)/4;
		cv=(cr0+cr1+cr2+cr3-4*cy)/4;
#endif

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

void TKuPI_TransYUV444(byte *img, int str,
	s16 *blky, s16 *blku, s16 *blkv)
{
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

//	sw=2;
	sw=0;
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		z=((y*2+0)*str+(x*2+0))*4;
		cb0=img[z+(0^sw)];		cg0=img[z+1];
		cb1=img[z+(4^sw)];		cg1=img[z+5];
		cr0=img[z+(2^sw)];		cr1=img[z+(6^sw)];
		z=((y*2+1)*str+(x*2+0))*4;
		cb2=img[z+(0^sw)];		cg2=img[z+1];
		cb3=img[z+(4^sw)];		cg3=img[z+5];
		cr2=img[z+(2^sw)];		cr3=img[z+(6^sw)];

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

void TKuPI_TransY4(byte *img, int str, s16 *blky)
{
	int cy0, cy1, cy2, cy3;
	int cy, cu, cv, sw;
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int cr3, cg3, cb3;
	int x, y, z;

//	sw=2;
	sw=0;
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		z=((y*2+0)*str+(x*2+0))*4;
		cb0=img[z+(0^sw)];		cg0=img[z+1];
		cb1=img[z+(4^sw)];		cg1=img[z+5];
		cr0=img[z+(2^sw)];		cr1=img[z+(6^sw)];
		z=((y*2+1)*str+(x*2+0))*4;
		cb2=img[z+(0^sw)];		cg2=img[z+1];
		cb3=img[z+(4^sw)];		cg3=img[z+5];
		cr2=img[z+(2^sw)];		cr3=img[z+(6^sw)];

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

void TKuPI_TransA4(byte *img, int str, s16 *blky)
{
	int cy0, cy1, cy2, cy3;
	int x, y, z;

	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		z=((y*2+0)*str+(x*2+0))*4;
		cy0=img[z+3];		cy1=img[z+7];
		z=((y*2+1)*str+(x*2+0))*4;
		cy2=img[z+3];		cy3=img[z+7];
		z=(y*2*8)+(x*2);
		blky[z+0]=cy0;	blky[z+1]=cy1;
		blky[z+8]=cy2;	blky[z+9]=cy3;
	}
}


void TKuPI_EncodeMacroBlock(TKuPI_EncState *ctx, byte *img, int str)
{
	s16 blk_y0[64], blk_y1[64];
	s16 blk_y2[64], blk_y3[64];
	s16 blk_u0[64], blk_v0[64];	
	s16 blk_u1[64], blk_v1[64];	
	s16 blk_u2[64], blk_v2[64];	
	s16 blk_u3[64], blk_v3[64];	
	s16 blk_a0[64], blk_a1[64];
	s16 blk_a2[64], blk_a3[64];
	int mbf;
	
	mbf=ctx->mbform;

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

	TKuPI_TransBH(blk_y0, blk_y0);
	TKuPI_TransBH(blk_y1, blk_y1);
	TKuPI_TransBH(blk_y2, blk_y2);
	TKuPI_TransBH(blk_y3, blk_y3);

	if(mbf==1)
	{
		TKuPI_TransBH(blk_u0, blk_u0);
		TKuPI_TransBH(blk_v0, blk_v0);
	}

	if((mbf==2) || (mbf==3))
	{
		TKuPI_TransBH(blk_u0, blk_u0);
		TKuPI_TransBH(blk_u1, blk_u1);
		TKuPI_TransBH(blk_u2, blk_u2);
		TKuPI_TransBH(blk_u3, blk_u3);

		TKuPI_TransBH(blk_v0, blk_v0);
		TKuPI_TransBH(blk_v1, blk_v1);
		TKuPI_TransBH(blk_v2, blk_v2);
		TKuPI_TransBH(blk_v3, blk_v3);
	}
	
	if(mbf==3)
	{
		TKuPI_TransBH(blk_a0, blk_a0);
		TKuPI_TransBH(blk_a1, blk_a1);
		TKuPI_TransBH(blk_a2, blk_a2);
		TKuPI_TransBH(blk_a3, blk_a3);
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

void TKuPI_EncodeMacroBlockEdge(TKuPI_EncState *ctx, byte *img,
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


int TKuPI_EncodeImageBuffer(TKuPI_EncState *ctx,
	byte *ibuf, int szibuf,
	byte *oimg, int oxs, int oys, int qfl)
{
	byte *ct, *ct0;
	u32 tg, sz;
	float qsc;
	int cxs, cys, cx, cy;
	int i, j, k, l, am;

	if((qfl&127)>=100)
	{
		am=0; l=oxs*oys;
		for(i=0; i<l; i++)
			if(oimg[i*4+3]!=255)
				am=1;

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

		am=0; l=oxs*oys;
		for(i=0; i<l; i++)
			if(oimg[i*4+3]<(255-qsc))
				am=1;

		for(cy=0; cy<8; cy++)
		{
			for(cx=0; cx<8; cx++)
			{
//				k=1.0+((cx+1)/4.0)*((cy+1)/4.0)*13;
//				k=1.0+sqrt(((cx+1)/4.0)*((cy+1)/4.0))*qsc;
				k=1.0+pow(((cx+1)/4.0)*((cy+1)/4.0), 0.667)*qsc;
				if(k>255)
					k=255;
				i=cy*8+cx;
				ctx->qtab_y[i]=k;
				ctx->qtab_uv[i]=k;
				printf("%3d ", k);
			}
			printf("\n");
		}
		
		ctx->mbform=1;
	//	ctx->mbform=2;
	//	ctx->mbform=0;
		if(am)
			ctx->mbform=3;
	}
	
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

	i=32;
	((u32 *)ct)[0]=TKUPI_FCC_uphd;
	((u32 *)ct)[1]=~i;
	memset(ct+8, 0, 24);
	*(u16 *)(ct+ 8)=oxs;
	*(u16 *)(ct+10)=oys;
	*(byte *)(ct+12)=ctx->mbform;
	ct+=i;


	i=8+1+64;
	((u32 *)ct)[0]=TKUPI_FCC_qtab;
	((u32 *)ct)[1]=~i;
	ct[8]=1;
	memcpy(ct+9, ctx->qtab_y, 64);
	ct+=i;

	i=8+1+64;
	((u32 *)ct)[0]=TKUPI_FCC_qtab;
	((u32 *)ct)[1]=~i;
	ct[8]=2;
	memcpy(ct+9, ctx->qtab_uv, 64);
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
				oimg+((cy*16*oxs)+(cx*16))*4, oxs);
		}
		if(oxs&15)
		{
			TKuPI_EncodeMacroBlockEdge(ctx,
				oimg+((cy*16*oxs)+(cx*16))*4, oxs, oxs&15, 16);
		}
	}
	if(oys&15)
	{
		for(cx=0; cx<cxs; cx++)
		{
			TKuPI_EncodeMacroBlockEdge(ctx,
				oimg+((cy*16*oxs)+(cx*16))*4, oxs,
				16, oys&15);
		}
		if(oxs&15)
		{
			TKuPI_EncodeMacroBlockEdge(ctx,
				oimg+((cy*16*oxs)+(cx*16))*4, oxs, oxs&15, oys&15);
		}
	}
	
	TKuPI_WriteBits(ctx, 0, 32);
	
	ct=ctx->ct;
	i=ct-ct0;
	((u32 *)ct0)[1]=~i;
	
	return(ct-ibuf);
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