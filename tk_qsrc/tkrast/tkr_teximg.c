void *tkra_malloc(int sz)
{
	return(malloc(sz));
}

void tkra_free(void *ptr)
{
	free(ptr);
}

int tkra_log2dn(int val)
{
	int v, i;
	
	v=val; i=0;
	while(v>1)
		{ v=v>>1; i++; }
	return(i);
}

TKRA_TexImage *TKRA_AllocTexImg(TKRA_Context *ctx)
{
	TKRA_TexImage *tmp;
	
	tmp=tkra_malloc(sizeof(TKRA_TexImage));
	memset(tmp, 0, sizeof(TKRA_TexImage));
	
	tmp->next=ctx->tex_list;
	ctx->tex_list=tmp;
	
	return(tmp);
}

int TKRA_GetPixel555Luma(int pix)
{
//	int cr, cg, cb;
	int y;
	
//	cr=(pix>>10)&31;
//	cg=(pix>>10)&31;
//	cb=(pix>>10)&31;

	y=	(((pix>> 5)&31)<<2)+
		(((pix>>10)&31)<<1)+
		(((pix>> 0)&31)<<1);
	return(y);
}

int TKRA_GetPixel555Alpha(int a)
{
	int av;
	av=(a&0x0001)|((a&0x0020)>>4)|((a&0x0400)>>8);
	if(a&0x8000)
	{
		av=(av<<5);
		return(av);
	}
	return(255);
}

int TKRA_GetPixel444A3_Alpha(int a)
{
	int av;
	av=(a&0x0001)|((a&0x0020)>>4)|((a&0x0400)>>8);
	av=(av<<5);
	return(av);
}

void TKRA_EncodeBlockUtx2(tkra_rastpixel *pix, u32 *blk)
{
	byte pxy[16], pxa[16];
	u32 pxv;
	u64 argb, pvrgb, mrgb, nrgb, a2rgb;
	int mv, nv, my, ny, pv, py;
	int ma, na, pa;
	int acy, aca, isam;
	int i, j, k;
	
	argb=0;
	mv=0x7FFF;	nv=0x0000;
	my=256;		ny=0;
	ma=256;		na=0;
	acy=0;		aca=0;
	for(i=0; i<16; i++)
	{
		pv=pix[i];
		py=TKRA_GetPixel555Luma(pv);
		pa=TKRA_GetPixel555Alpha(pv);
		
		pvrgb=tkra_rgbupck64(pv);
		pvrgb=tkra_pmuluhw(pvrgb, 0x0FFF0FFF0FFF0FFFULL);
		argb+=pvrgb;
		
		if(py<my)
			{ mv=pv; my=py; }
		if(py>ny)
			{ nv=pv; ny=py; }
		if(pa<ma)	{ ma=pa; }
		if(pa>na)	{ na=pa; }
		pxy[i]=py;
		pxa[i]=pa;
		acy+=py;
		aca+=pa;
	}
	acy=acy/16;
	aca=aca/16;

#if 1
	mrgb =tkra_rgbupck64(mv);
	nrgb =tkra_rgbupck64(nv);
//	mrgb =tkra_pmuluhw(mrgb, 0x7FFF7FFF7FFF7FFFULL);
//	nrgb =tkra_pmuluhw(nrgb, 0x7FFF7FFF7FFF7FFFULL);
//	a2rgb=tkra_pmuluhw(argb, 0x7FFF7FFF7FFF7FFFULL);

	mrgb =tkra_pmuluhw(mrgb, 0xAAAAAAAAAAAAAAAAULL);
	nrgb =tkra_pmuluhw(nrgb, 0xAAAAAAAAAAAAAAAAULL);
	a2rgb=tkra_pmuluhw(argb, 0x5555555555555555ULL);

	mrgb =mrgb+a2rgb;
	nrgb =nrgb+a2rgb;
	mv=tkra_rgbpck64(mrgb);
	nv=tkra_rgbpck64(nrgb);

	my=TKRA_GetPixel555Luma(mv);
	ny=TKRA_GetPixel555Luma(nv);
#endif

	acy=(my+ny)/2;
	
	isam = ((ma&0xF0)!=(na&0xF0)) || ((ma&0xF0)!=0xF0);

	j=ny-acy;
	if(!j)j++;
	k=384/j;

	pxv=0;
	for(i=0; i<16; i++)
	{
		py=pxy[i];
		pa=pxa[i];
		
		if(isam)
		{
			if(py>acy)
				pxv|=1<<(i*2+1);
			if(pa>aca)
				pxv|=1<<(i*2+0);
		}else
		{
			j=2+(((py-acy)*k)>>8);
			if(j<0)j=0;
			if(j>3)j=3;
			pxv|=j<<(i*2);
		
//			if(py>acy)
//				pxv|=3<<(i*2);
		}
	}
	
//	if(((ma&0xF0)!=(na&0xF0)) || ((ma&0xF0)!=0xF0))
	if(isam)
	{
		nv&=0x7BDE;
		mv&=0x7BDE;

		if(na&0x80)nv|=0x0400;
		if(na&0x40)nv|=0x0020;
		if(na&0x20)nv|=0x0001;

		if(ma&0x80)mv|=0x0400;
		if(ma&0x40)mv|=0x0020;
		if(ma&0x20)mv|=0x0001;
		mv|=0x8000;
	}else
	{
		mv&=0x7FFF;
		nv&=0x7FFF;
	}
	
	blk[1]=pxv;
	blk[0]=(nv<<0)|(mv<<16);
}

void TKRA_EncodeImgMortUtx2(tkra_rastpixel *pix, u32 *blk, int shl)
{
	tkra_rastpixel *cs;
	u32 *ct;
	int i, n;
	
	if(shl>=2)
	{
		n=1<<((shl-2)*2);
	}else
	{
		n=1;
	}
	
	cs=pix; ct=blk;
	for(i=0; i<n; i++)
	{
		TKRA_EncodeBlockUtx2(cs, ct);
		cs+=16; ct+=2;
	}
}

void TKRA_UpdateTexImg(TKRA_TexImage *img,
	tkra_rastpixel *buf, int xs, int ys, int mip)
{
	int xshl, yshl, xsh1, ysh1, txfl, pix;
	int x, y, m, bsz;
	int i, j, k;
	
	if(xs&(xs-1))
		return;
	if(ys&(ys-1))
		return;

	xshl=tkra_log2dn(xs);
	yshl=tkra_log2dn(ys);

	if(mip>0)
	{
		xsh1=img->tex_xshl-mip;
		ysh1=img->tex_yshl-mip;
		if(xsh1<0)xsh1=0;
		if(ysh1<0)ysh1=0;
		
		if((xshl!=xsh1) || (yshl!=ysh1))
			return;

		txfl=img->tex_flag;

		txfl|=TKRA_TRFL_HASMIP;

		if(xshl==yshl)
		{
			for(y=0; y<ys; y++)
				for(x=0; x<xs; x++)
			{
				j=(y<<xshl)+x;
//				k=tkra_morton8(x, y);
				k=tkra_morton8(x, y)+img->tex_mipofs[mip];
				pix=buf[j];
				img->tex_img[k]=pix;
				if(pix&0x8000)
					txfl|=TKRA_TRFL_ALPHA;
			}

			if(img->tex_img_bcn)
			{
				TKRA_EncodeImgMortUtx2(
					img->tex_img+img->tex_mipofs[mip],
					img->tex_img_bcn+img->tex_mipofs_bcn[mip],
					xshl);
			}
		}else
		{
			k=(1<<xsh1)*(1<<ysh1);
			memcpy(img->tex_img+img->tex_mipofs[mip], buf,
				k*sizeof(tkra_rastpixel));
		}

		img->tex_flag=txfl;

	}else if(mip<=0)
	{
		if((xshl!=img->tex_xshl) || (yshl!=img->tex_yshl))
		{
			if(img->tex_img)
			{
				if(img->tex_img != (tkra_rastpixel *)(img->tex_mipofs+4))
					tkra_free(img->tex_img);
				img->tex_img=NULL;
			}
			if(img->tex_img_bcn)
			{
				if(img->tex_img_bcn != (u32 *)(img->tex_mipofs_bcn+4))
					tkra_free(img->tex_img_bcn);
				img->tex_img_bcn=NULL;
			}

			i=0; j=0; xsh1=xshl; ysh1=yshl;
			while((xsh1>0) || (ysh1>0))
			{
				img->tex_mipofs[i]=j;
				j+=(1<<xsh1)*(1<<ysh1);
				i++;
				if(xsh1>0)xsh1--;
				if(ysh1>0)ysh1--;
			}
			img->tex_mipofs[i]=j;
			j+=(1<<xsh1)*(1<<ysh1);

			if(j<=24)
				img->tex_img=(tkra_rastpixel *)(img->tex_mipofs+4);
			else
				img->tex_img=tkra_malloc(j*sizeof(tkra_rastpixel));
			img->tex_xshl=xshl;
			img->tex_yshl=yshl;
			
			k=i;
			if(k>xshl)k=xshl;
			if(k>yshl)k=yshl;
			img->tex_mmip=k;
			img->tex_flag=0;

			for(k=0; k<j; k++)
				img->tex_img[k]=0x7FFF;

#if 0
			if((xshl==yshl) && (xshl>=3) && (yshl>=3))
			{
				bsz=1;
				i=0; j=0; xsh1=xshl-2; ysh1=yshl-2;
				while((xsh1>0) || (ysh1>0))
				{
					img->tex_mipofs_bcn[i]=j;
					j+=1<<(xsh1+ysh1+bsz);
					i++;
					if(xsh1>0)xsh1--;
					if(ysh1>0)ysh1--;
				}
				img->tex_mipofs_bcn[i+0]=j+(0<<bsz);	//4x4
				img->tex_mipofs_bcn[i+1]=j+(1<<bsz);	//2x2
				img->tex_mipofs_bcn[i+2]=j+(2<<bsz);	//1x1
				i+=2; j+=(3<<bsz);

				if((j<=6) && (i<4))
					img->tex_img_bcn=(u32 *)(img->tex_mipofs_bcn+4);
				else
					img->tex_img_bcn=tkra_malloc(j*sizeof(u32));
			}
#endif
		}

		txfl=img->tex_flag;

		if(xshl==yshl)
		{
			for(y=0; y<ys; y++)
				for(x=0; x<xs; x++)
			{
				j=(y<<xshl)+x;
				k=tkra_morton8(x, y);
				pix=buf[j];
				img->tex_img[k]=pix;
				if(pix&0x8000)
					txfl|=TKRA_TRFL_ALPHA;
			}

#if 1
			if(img->tex_img_bcn)
			{
				TKRA_EncodeImgMortUtx2(
					img->tex_img+img->tex_mipofs[mip],
					img->tex_img_bcn+img->tex_mipofs_bcn[mip],
					xshl);
			}
#endif
		}else
		{
			k=(1<<xshl)*(1<<yshl);
			memcpy(img->tex_img, buf, k*sizeof(tkra_rastpixel));
		}

		img->tex_flag=txfl;

	}
}


void TKRA_UpdateTexImgUtx2(TKRA_TexImage *img,
	u64 *buf, int xs, int ys, int mip)
{
	u64 *timg;
	u64 blk;
	int xshl, yshl, xsh1, ysh1, txfl, pix;
	int x, y, m, n, xs1, ys1, bsz;
	int i, j, k;
	
	if(xs&(xs-1))
		return;
	if(ys&(ys-1))
		return;
	if(xs!=ys)
		return;

	xs1=(xs+3)>>2;
	ys1=(ys+3)>>2;

	xshl=tkra_log2dn(xs);
	yshl=tkra_log2dn(ys);

	if(mip>0)
	{
		xsh1=img->tex_xshl-mip;
		ysh1=img->tex_yshl-mip;
		if(xsh1<0)xsh1=0;
		if(ysh1<0)ysh1=0;
		
		if((xshl!=xsh1) || (yshl!=ysh1))
			return;

		txfl=img->tex_flag;

		if(img->tex_img_bcn)
		{
			timg=(u64 *)(img->tex_img_bcn+img->tex_mipofs_bcn[mip]);
			n=xs1*ys1;
			for(i=0; i<n; i++)
			{
				blk=buf[i];
				if(blk&0x80008000ULL)
					txfl|=TKRA_TRFL_ALPHA;
				timg[i]=blk;
			}
		}

		img->tex_flag=txfl;

	}else if(mip<=0)
	{
		if((xshl!=img->tex_xshl) || (yshl!=img->tex_yshl))
		{
			if(img->tex_img)
			{
				if(img->tex_img != (tkra_rastpixel *)(img->tex_mipofs+4))
					tkra_free(img->tex_img);
				img->tex_img=NULL;
			}
			if(img->tex_img_bcn)
			{
				if(img->tex_img_bcn != (u32 *)(img->tex_mipofs_bcn+4))
					tkra_free(img->tex_img_bcn);
				img->tex_img_bcn=NULL;
			}

			i=0; j=0; xsh1=xshl; ysh1=yshl;
			while((xsh1>0) || (ysh1>0))
			{
				img->tex_mipofs[i]=j;
				j+=(1<<xsh1)*(1<<ysh1);
				i++;
				if(xsh1>0)xsh1--;
				if(ysh1>0)ysh1--;
			}
			img->tex_mipofs[i]=j;
			j+=(1<<xsh1)*(1<<ysh1);

//			if(j<=24)
//				img->tex_img=(tkra_rastpixel *)(img->tex_mipofs+4);
//			else
//				img->tex_img=tkra_malloc(j*sizeof(tkra_rastpixel));
			img->tex_xshl=xshl;
			img->tex_yshl=yshl;
			
			k=i;
			if(k>xshl)k=xshl;
			if(k>yshl)k=yshl;
			img->tex_mmip=k;
			img->tex_flag=0;

//			for(k=0; k<j; k++)
//				img->tex_img[k]=0x7FFF;
				
			bsz=1;
			i=0; j=0; xsh1=xshl-2; ysh1=yshl-2;
			while((xsh1>0) || (ysh1>0))
			{
				img->tex_mipofs_bcn[i]=j;
				j+=1<<(xsh1+ysh1+bsz);
				i++;
				if(xsh1>0)xsh1--;
				if(ysh1>0)ysh1--;
			}
			img->tex_mipofs_bcn[i+0]=j+(0<<bsz);	//4x4
			img->tex_mipofs_bcn[i+1]=j+(1<<bsz);	//2x2
			img->tex_mipofs_bcn[i+2]=j+(2<<bsz);	//1x1
			i+=2; j+=(3<<bsz);

			if((j<=6) && (i<4))
				img->tex_img_bcn=(u32 *)(img->tex_mipofs_bcn+4);
			else
				img->tex_img_bcn=tkra_malloc(j*sizeof(u32));
		}

		txfl=img->tex_flag;

		if(img->tex_img_bcn)
		{
			timg=(u64 *)(img->tex_img_bcn);
			n=xs1*ys1;
			for(i=0; i<n; i++)
			{
				blk=buf[i];
				if(blk&0x80008000ULL)
					txfl|=TKRA_TRFL_ALPHA;
				timg[i]=blk;
			}
		}

		img->tex_flag=txfl;
	}
}

u64 TKRA_ConvBlockDxt1ToUtx2(u64 blk)
{
	u64 blk2;
	u32 pxa, pxb;
	int clra, clrb, clrc, clrd, alpha;
	int rek, ix;
	int x, y, z;
	int i, j, k;
	
	clra=(blk>> 0)&65535;
	clrb=(blk>>16)&65535;
	
	clrc=((clra>>1)&0x7FE0)|(clra&0x001F);
	clrd=((clrb>>1)&0x7FE0)|(clrb&0x001F);
	
//	rek=0x2130;
//	rek=0x1230;
	rek=0x1203;
//	rek=0x0303;
	alpha=0;
	if(clra<=clrb)
	{
		clrd|= 0x0421;
		clrc&=~0x0421;

		clrd|= 0x8000;
		alpha=1;
		rek=0x1002;
	}
	
	pxa=(blk>>32); pxb=0; ix=0;

#if 0
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		z=tkra_morton8(x, y);
//		i=(pxa>>((y*4+x)*2))&3;
		i=(pxa>>ix)&3;	ix+=2;
//		k=rek|(((x^y)&alpha)<<9);
//		k=rek;
//		j=(k>>(i*4))&3;
		j=(rek>>(i*4))&3;
		pxb|=j<<(z*2);
	}
#endif

#if 1
	for(y=0; y<4; y++)
	{
		z=tkra_morton8(0, y);
		i=(pxa>>ix)&3;	ix+=2;
		j=(rek>>(i*4))&3;
		pxb|=j<<(z*2);

		z=tkra_morton8(1, y);
		i=(pxa>>ix)&3;	ix+=2;
		j=(rek>>(i*4))&3;
		pxb|=j<<(z*2);

		z=tkra_morton8(2, y);
		i=(pxa>>ix)&3;	ix+=2;
		j=(rek>>(i*4))&3;
		pxb|=j<<(z*2);

		z=tkra_morton8(3, y);
		i=(pxa>>ix)&3;	ix+=2;
		j=(rek>>(i*4))&3;
		pxb|=j<<(z*2);
	}
#endif
	
	blk2=((u64)clrc) | (((u64)clrd)<<16) | (((u64)pxb)<<32);
	return(blk2);
}

void TKRA_UpdateTexImgDxt1(TKRA_TexImage *img,
	u64 *src, int xs, int ys, int mip)
{
	static u64 *tmpbuf=NULL;
	u64 sblk, dblk;
	int xs1, ys1, x, y, z;
	
	if(xs!=ys)
		return;
	if(xs<0)
		return;
	if(xs>256)
		return;
	
	if(!tmpbuf)
	{
		tmpbuf=malloc(64*64*8);
	}
	
	xs1=(xs+3)>>2;
	ys1=(ys+3)>>2;
	for(y=0; y<ys1; y++)
		for(x=0; x<xs1; x++)
	{
		z=tkra_morton8(x, y);
		sblk=src[y*xs1+x];
		dblk=TKRA_ConvBlockDxt1ToUtx2(sblk);
		tmpbuf[z]=dblk;
	}

	TKRA_UpdateTexImgUtx2(img, tmpbuf, xs, ys, mip);
}

TKRA_TexImage *TKRA_LookupTexImg(TKRA_Context *ctx, int num)
{
	TKRA_TexImage *cur;
	int h;
	
	h=(num+(num>>8)+(num>>16))&255;
	cur=ctx->tex_hash[h];
	while(cur)
	{
		if(cur->tex_id==num)
			return(cur);
		cur=cur->chain;
	}
	return(NULL);
}

TKRA_TexImage *TKRA_GetTexImg(TKRA_Context *ctx, int num)
{
	TKRA_TexImage *cur;
	int h;
	
	h=(num+(num>>8)+(num>>16))&255;
	cur=ctx->tex_hash[h];
	while(cur)
	{
		if(cur->tex_id==num)
			return(cur);
		cur=cur->chain;
	}
	
	cur=TKRA_AllocTexImg(ctx);
	cur->tex_id=num;
	cur->chain=ctx->tex_hash[h];
	ctx->tex_hash[h]=cur;
	
	cur->tex_xshl=0;
	cur->tex_yshl=0;
	cur->tex_mmip=0;
	cur->tex_img=(tkra_rastpixel *)(cur->tex_mipofs+4);
	cur->tex_img[0]=0x7FFF;
	cur->tex_img_bcn=NULL;
	
	return(cur);
}

int TKRA_AllocTexnum(TKRA_Context *ctx)
{
	int id;
	
	id=ctx->tex_rov;
	ctx->tex_rov=(id+1);
	return(id);
}

int TKRA_BindTexImg(TKRA_Context *ctx, TKRA_TexImage *img)
{
	ctx->tex_cur=img;
	ctx->tex_img=img->tex_img;
	ctx->tex_img_bcn=img->tex_img_bcn;
	ctx->tex_xshl=img->tex_xshl;
	ctx->tex_yshl=img->tex_yshl;
	ctx->tex_mmip=img->tex_mmip;
	ctx->tex_flag=img->tex_flag;
	
	TKRA_SetupForState(ctx);
	
	return(0);
}

int TKRA_SetupForState(TKRA_Context *ctx)
{
	TKRA_SetupDrawEdgeForState(ctx);
	return(0);
}
