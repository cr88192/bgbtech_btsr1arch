byte *BTIC2D_DecodeBlockSplitZV(
	byte *cs, int *rz, int *rv)
{
	int z, v;
	int i, j, k;

	i=*cs++;
	if(!(i&0x80))
	{
		j=i&15; j=(j>>1)^((j<<31)>>31);
		*rv=j; *rz=(i>>4);
		return(cs);
	}
	
	switch((i>>5)&7)
	{
	case 0:	case 1:	case 2:	case 3:
		j=i&15; j=(j>>1)^((j<<31)>>31);
		v=j; z=(i>>4);

//		j=i&31; j=(j>>1)^((j<<31)>>31);
//		v=j; z=(i>>5);
		break;
	case 4:
//		j=(i&63)+16; j=(j>>1)^((j<<31)>>31);
//		v=j; z=0;
//		j=(i&1)*2-1;
//		v=j; z=((i>>1)&15)+8;

		j=(i&3)+1; j=(j>>1)^((j<<31)>>31);
		v=j; z=((i>>2)&7)+8;
		break;
	case 5:
		v=0;
		z=(i&31)+1;
		break;

#if 1
	case 6:
		j=(i&31)+16; j=(j>>1)^((j<<31)>>31);
		v=j; z=0;
		break;

	case 7:
		switch((i>>2)&7)
		{
		case 0: case 1:
		case 2: case 3:
			j=((i&3)<<8)|(*cs++);
			j=(j>>1)^((j<<31)>>31);
			v=j; z=(i>>2)&3;
			break;
		case 4:
//			j=((i&3)<<8)|(*cs++);
			j=*(u16 *)cs; cs+=2;
//			j=*cs++;
//			j=(j<<8)|(*cs++);
			j=(j>>1)^((j<<31)>>31);
//			v=j; z=0;
			v=j; z=(i>>2)&3;
			break;
		case 5:
//			j=((i&3)<<8)|(*cs++);
			j=(*(u32 *)cs)&16777215; cs+=3;

//			j=*cs++;
//			j=(j<<8)|(*cs++);
//			j=(j<<8)|(*cs++);
			j=(j>>1)^((j<<31)>>31);
//			v=j; z=0;
			v=j; z=(i>>2)&3;
			break;
		case 6:
//			j=*cs++;
//			j=(j<<8)|(*cs++);
//			j=(j<<8)|(*cs++);
//			j=(j<<8)|(*cs++);
			j=*(u32 *)cs; cs+=4;
			j=(j>>1)^((j<<31)>>31);
			v=j; z=0;
			break;
		}
		break;
#endif

#if 0
	case 6:
		j=((i&7)<<8)|(*cs++);
		j=(j>>1)^((j<<31)>>31);
		v=j; z=(i>>3)&3;
		break;
	case 7:
		j=((i&15)<<8)|(*cs++);
		j=(j<<8)|(*cs++);
		if(i&16)
		{
			j=(j<<8)|(*cs++);
		}
		
		j=(j>>1)^((j<<31)>>31);
		v=j; z=0;
		break;
#endif
	}
	
	*rz=z;
	*rv=v;
	return(cs);
}

int BTIC2D_DecodeBlockByte8Y(BTIC1H_Context *ctx, int *blk)
{
	byte *lcs[64];
	byte *cs, *ics0, *ics1;
	int *ct, *cte;
	int z, v, lcr;
	int i, j, k;
	
	cs=ctx->bs_cs;
	ct=blk; cte=blk+64;

	lcr=0;
	ics0=cs;
	if(*cs==0xAF)
		{ cs++; }
	else
	{
		k=-1;
	}
	cs=BTIC2D_DecodeBlockSplitZV(cs, &z, &v);
	if(z==1)
	{
		*ct++=v;
		while(ct<cte)
			*ct++=0;
		ctx->bs_cs=cs;
		return(1);
	}else if(z)
	{
		return(-1);
	}
	
	*ct++=v;

	ics1=cs;
	while(ct<cte)
	{
		ics0=cs;
		lcs[lcr++]=cs; lcr&=63;
		cs=BTIC2D_DecodeBlockSplitZV(cs, &z, &v);
		if(!v)
		{
			if(!z)
			{
				while(ct<cte)
					*ct++=0;
				break;
			}

			if(((*ics0)>>5)!=5)
				k=-1;
		
			i=z;
			while(i--)
			{
				if(ct>=cte)
					return(-1);
				*ct++=0;
			}
			continue;
		}
		i=z;
		while(i--)
		{
			if(ct>=cte)
				return(-1);
			*ct++=0;
		}
		*ct++=v;
	}

	if(*cs!=0xAF)
		k=-1;

	ctx->bs_cs=cs;
	return(0);
}

void BTIC2D_DecodeBlockByte8(BTIC1H_Context *ctx, int *blk)
{
	BTIC2D_DecodeBlockByte8Y(ctx, blk+  0);
	BTIC2D_DecodeBlockByte8Y(ctx, blk+ 64);
	BTIC2D_DecodeBlockByte8Y(ctx, blk+128);
	BTIC2D_DecodeBlockByte8Y(ctx, blk+192);
	BTIC2D_DecodeBlockByte8Y(ctx, blk+256);
	BTIC2D_DecodeBlockByte8Y(ctx, blk+320);

	ctx->cy+=blk[  0];	blk[  0]=ctx->cy;
	ctx->cy+=blk[ 64];	blk[ 64]=ctx->cy;
	ctx->cy+=blk[128];	blk[128]=ctx->cy;
	ctx->cy+=blk[192];	blk[192]=ctx->cy;

	ctx->cu+=blk[256];	blk[256]=ctx->cu;
	ctx->cv+=blk[320];	blk[320]=ctx->cv;
}

int BTIC2D_DecodeUVLI(BTIC1H_Context *ctx)
{
	int i;
	
	i=*ctx->bs_cs++;
	if(i<0x80)
	{
	}else if(i<0xC0)
	{
		i=((i&0x3F)<<8)|(*ctx->bs_cs++);
	}else if(i<0xE0)
	{
		i=((i&0x1F)<<8)|(*ctx->bs_cs++);
		i=(i<<8)|(*ctx->bs_cs++);
	}else if(i<0xF0)
	{
		i=((i&0x0F)<<8)|(*ctx->bs_cs++);
		i=(i<<8)|(*ctx->bs_cs++);
		i=(i<<8)|(*ctx->bs_cs++);
	}else
	{
		i=((i&0x07)<<8)|(*ctx->bs_cs++);
		i=(i<<8)|(*ctx->bs_cs++);
		i=(i<<8)|(*ctx->bs_cs++);
		i=(i<<8)|(*ctx->bs_cs++);
	}
	return(i);
}

int BTIC2D_DecodeSVLI(BTIC1H_Context *ctx)
{
	int i;

	i=BTIC2D_DecodeUVLI(ctx);
	i=(i>>1)^((i<<31)>>31);
	return(i);
}

int BTIC2D_DecodeQTab(BTIC1H_Context *ctx, int *tab)
{
	int *ct, *cte;
	int i, j, k, l;
	
	ct=tab; cte=tab+64; l=1;
	while(ct<cte)
	{
		i=BTIC2D_DecodeSVLI(ctx);
		if(i<=0)
		{
			if(!i)
			{
				while(ct<cte)
					*ct++=l;
				break;
			}
			
			j=-i;
			while((ct<cte) && (j--))
				*ct++=l;
			continue;
		}else
		{
			*ct++=i;
		}
	}
	return(0);
}

void BTIC2D_DequantReorder(int *iblk, int *oblk, int *qtab)
{
	int i, j;
	
	for(i=0; i<8; i++)
	{
		j=i*8;
		oblk[pdjpg_zigzag8_2[j+0]]=iblk[j+0]*qtab[j+0];
		oblk[pdjpg_zigzag8_2[j+1]]=iblk[j+1]*qtab[j+1];
		oblk[pdjpg_zigzag8_2[j+2]]=iblk[j+2]*qtab[j+2];
		oblk[pdjpg_zigzag8_2[j+3]]=iblk[j+3]*qtab[j+3];
		oblk[pdjpg_zigzag8_2[j+4]]=iblk[j+4]*qtab[j+4];
		oblk[pdjpg_zigzag8_2[j+5]]=iblk[j+5]*qtab[j+5];
		oblk[pdjpg_zigzag8_2[j+6]]=iblk[j+6]*qtab[j+6];
		oblk[pdjpg_zigzag8_2[j+7]]=iblk[j+7]*qtab[j+7];
	}
}

void BTIC2D_DecodeBlockYUV(int *iblk,
	int *oblky, int *oblku, int *oblkv,
	int *yqtab, int *uvqtab,
	int yxstride, int yystride,
	int uvxstride, int uvystride)
{
	int tblk[512];

	BTIC2D_DequantReorder(iblk+  0, tblk+  0, yqtab);
	BTIC2D_DequantReorder(iblk+ 64, tblk+ 64, yqtab);
	BTIC2D_DequantReorder(iblk+128, tblk+128, yqtab);
	BTIC2D_DequantReorder(iblk+192, tblk+192, yqtab);
	
	BTIC2D_DequantReorder(iblk+256, tblk+256, uvqtab);
	BTIC2D_DequantReorder(iblk+320, tblk+320, uvqtab);
	
#if 0
	BTIC2D_TransIWHT8Stride(tblk+  0, oblky+0*yystride+0*yxstride,
		1, 8, yxstride, yystride);
	BTIC2D_TransIWHT8Stride(tblk+ 64, oblky+0*yystride+8*yxstride,
		1, 8, yxstride, yystride);
	BTIC2D_TransIWHT8Stride(tblk+128, oblky+8*yystride+0*yxstride,
		1, 8, yxstride, yystride);
	BTIC2D_TransIWHT8Stride(tblk+192, oblky+8*yystride+8*yxstride,
		1, 8, yxstride, yystride);

	BTIC2D_TransIWHT8Stride(tblk+256, oblku, 1, 8, uvxstride, uvystride);
	BTIC2D_TransIWHT8Stride(tblk+320, oblkv, 1, 8, uvxstride, uvystride);
#endif

#if 1
	BTIC2D_TransIBHT8Stride(tblk+  0, oblky+0*yystride+0*yxstride,
		1, 8, yxstride, yystride);
	BTIC2D_TransIBHT8Stride(tblk+ 64, oblky+0*yystride+8*yxstride,
		1, 8, yxstride, yystride);
	BTIC2D_TransIBHT8Stride(tblk+128, oblky+8*yystride+0*yxstride,
		1, 8, yxstride, yystride);
	BTIC2D_TransIBHT8Stride(tblk+192, oblky+8*yystride+8*yxstride,
		1, 8, yxstride, yystride);

	BTIC2D_TransIBHT8Stride(tblk+256, oblku, 1, 8, uvxstride, uvystride);
	BTIC2D_TransIBHT8Stride(tblk+320, oblkv, 1, 8, uvxstride, uvystride);
#endif
}

void BTIC2D_DecodeFillImgBlockRGBA(
	byte *imgblk, int xstride, int ystride,
	int *ybuf, int *ubuf, int *vbuf,
	int yystride, int uvystride)
{
	int *csy0, *csy1, *csu, *csv;
	byte *ct0, *ct1;
	int cy0, cy1, cy2, cy3, cu, cv, cu1, cv1;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int i, j, k;
	
	for(i=0; i<8; i++)
	{
		ct0=imgblk+(i*2+0)*ystride;
		ct1=imgblk+(i*2+1)*ystride;
		csy0=ybuf+(i*2+0)*yystride;
		csy1=ybuf+(i*2+1)*yystride;
		csu=ubuf+i*uvystride;
		csv=vbuf+i*uvystride;

		for(j=0; j<8; j++)
		{
			cy0=csy0[0]; cy1=csy0[1];
			cy2=csy1[0]; cy3=csy1[1];
			csy0+=2; csy1+=2;
			cu=*csu++; cv=*csv++;
			
//			cu=128;
//			cv=128;
			
			cu1=(cu-128)*2;
			cv1=(cv-128)*2;
			
			cb0=cy0+cu1;	cb1=cy1+cu1;
			cb2=cy2+cu1;	cb3=cy3+cu1;
			cr0=cy0+cv1;	cr1=cy1+cv1;
			cr2=cy2+cv1;	cr3=cy3+cv1;
			cg0=(8*cy0-3*cr0-cb0)/4;
			cg1=(8*cy1-3*cr1-cb1)/4;
			cg2=(8*cy2-3*cr2-cb2)/4;
			cg3=(8*cy3-3*cr3-cb3)/4;
			
			if((cr0|cr1|cr2|cr3)>>8)
			{	cr0=clamp255(cr0);	cr1=clamp255(cr1);
				cr2=clamp255(cr2);	cr3=clamp255(cr3);	}
			if((cg0|cg1|cg2|cg3)>>8)
			{	cg0=clamp255(cg0);	cg1=clamp255(cg1);
				cg2=clamp255(cg2);	cg3=clamp255(cg3);	}
			if((cb0|cb1|cb2|cb3)>>8)
			{	cb0=clamp255(cb0);	cb1=clamp255(cb1);
				cb2=clamp255(cb2);	cb3=clamp255(cb3);	}
			
			ct0[0]=cr0;	ct0[1]=cg0;	ct0[2]=cb0; ct0+=xstride;
			ct0[0]=cr1;	ct0[1]=cg1;	ct0[2]=cb1; ct0+=xstride;
			ct1[0]=cr2;	ct1[1]=cg2;	ct1[2]=cb2; ct1+=xstride;
			ct1[0]=cr3;	ct1[1]=cg3;	ct1[2]=cb3; ct1+=xstride;
		}
	}
}

int BTIC2D_DecodeCtx(
	BTIC1H_Context *ctx,
	byte *cbuf,
	byte *imgbuf, int cbsz, int ibsz,
	int *rxs, int *rys, int pxf)
{
	int qftab[128];
	int tblk[512];
	int yblk[16*16], ublk[8*8], vblk[8*8];
	byte *cs0, *cs0e;
	byte *ct;
	int xstr, ystr;
	int xs1, ys1;
	int i, j, k;

	ctx->bs_cs=cbuf;
	ctx->bs_cse=cbuf+cbsz;

	for(i=0; i<128; i++)
		qftab[i]=1;

	i=BTIC2D_DecodeSVLI(ctx);
	while(i!=0)
	{
		if(i==-1)
		{
			ctx->xs=BTIC2D_DecodeUVLI(ctx);
			ctx->ys=BTIC2D_DecodeUVLI(ctx);
			i=BTIC2D_DecodeSVLI(ctx);
			continue;
		}
		
		if(i==-4)
		{
			BTIC2D_DecodeQTab(ctx, qftab);
			i=BTIC2D_DecodeSVLI(ctx);
			continue;
		}

		if(i==-5)
		{
			BTIC2D_DecodeQTab(ctx, qftab+64);
			i=BTIC2D_DecodeSVLI(ctx);
			continue;
		}
	}
	
	if(!imgbuf)
	{
		if(ctx->xs && ctx->ys)
		{
			if(rxs)*rxs=ctx->xs;
			if(rys)*rys=ctx->ys;
		}

		return(0);
	}

	xstr=4;
	ystr=ctx->xs*4;

	xs1=ctx->xs>>4;
	ys1=ctx->ys>>4;

	for(i=0; i<ys1; i++)
	{
		k=BTIC2D_DecodeUVLI(ctx);
		cs0=ctx->bs_cs;
		cs0e=cs0+k;
		ct=imgbuf+i*16*ystr;

		ctx->cy=0;
		ctx->cu=0;
		ctx->cv=0;

		for(j=0; j<xs1; j++)
		{
			BTIC2D_DecodeBlockByte8(ctx, tblk);
			BTIC2D_DecodeBlockYUV(tblk, yblk, ublk, vblk,
				qftab, qftab+64, 1, 16, 1, 8);
			BTIC2D_DecodeFillImgBlockRGBA(ct, xstr, ystr,
				yblk, ublk, vblk, 16, 8);
			ct+=16*xstr;
		}
		
		ctx->bs_cs=cs0e;
	}

	if(rxs)*rxs=ctx->xs;
	if(rys)*rys=ctx->ys;
	return(0);
}
