void	*tkra_freelist[256];
int		tkra_totbrk=0;

#ifdef GLQUAKE
void *Q_MallocLLn(int sz, char *lfn, int lln);
void Q_FreeLLn(void *ptr, char *lfn, int lln);

void *TKRA_Brk(int sz)
{
	tkra_totbrk+=sz;
	return(Q_MallocLLn(sz, __FILE__, __LINE__));
}
#else
void *TKRA_Brk(int sz)
{
	tkra_totbrk+=sz;
	return(malloc(sz));
}
#endif

int TKRA_MallocIndexForSize(int sz)
{
	int e, i, j;
	
	e=0; i=sz;
	while(i>=16)
	{
		i=(i+1)>>1;
		e++;
	}
	j=(e<<3)|(i&7);
	return(j);
}

int TKRA_SizeForIndex(int ix)
{
	int e, f, sz;
	
	e=ix>>3;
	f=8|(ix&7);
	sz=f<<e;
	return(sz);
}

void *tkra_malloc(int sz)
{
	byte *ptr;
	int ix, sz1;
	
	ix=TKRA_MallocIndexForSize(sz+8);
	
	ptr=tkra_freelist[ix];
	if(ptr)
	{
		tkra_freelist[ix]=*(byte **)ptr;
		*(int *)ptr=ix;
		return(ptr+8);
	}
	
	sz1=TKRA_SizeForIndex(ix);
	ptr=TKRA_Brk(sz1);
	*(int *)ptr=ix;
	return(ptr+8);
}

void tkra_free(void *ptr)
{
	byte *ptr1;
	int ix;
	
	if(!ptr)
		return;

	ptr1=((byte *)ptr)-8;
	ix=*(int *)ptr1;
	*(byte **)ptr1=tkra_freelist[ix];
	tkra_freelist[ix]=ptr1;
}

void *tkra_malloc_gbl(int sz)
{
	void *ptr;
#ifdef __BGBCC__
	ptr=tk_malloc_cat(sz, TKMM_MCAT_GLOBAL);
#else
	ptr=malloc(sz);
#endif
	return(ptr);
}

void *tkra_malloc_phys(int sz)
{
	void *ptr;
	long li;
#ifdef __BGBCC__
	ptr=tk_malloc_cat(sz, TKMM_MCAT_PHYSDFL);
	li=(long)ptr;
	if(li>>28)
	{
		tk_printf("tkra_malloc_phys: Ptr is virtual %p\n", ptr);
	}else if(ptr&15)
	{
		tk_printf("tkra_malloc_phys: Ptr is misal %p\n", ptr);
	}
#else
	ptr=malloc(sz);
#endif
	return(ptr);
}

void *tkra_malloc_tex(int sz)
{
	return(tkra_malloc_phys(sz));
//	return(tk_malloc(sz));
}

void tkra_free_tex(void *ptr)
{
	tk_free(ptr);
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
	
	tmp=tkra_malloc_gbl(sizeof(TKRA_TexImage));
//	tmp=tkra_malloc_phys(sizeof(TKRA_TexImage));
	memset(tmp, 0, sizeof(TKRA_TexImage));
	
	tmp->next=ctx->svctx->tex_list;
	ctx->svctx->tex_list=tmp;
	
	return(tmp);
}

int TKRA_ResampleImage555(
	tkra_rastpixel *src, int sxs, int sys,
	tkra_rastpixel *dst, int dxs, int dys)
{
	tkra_rastpixel *ct, *cs0;
	int ofs_x[1024], ofs_y[1024];
	int x, y, qx, qy, sx, sy;
	
	sx=(sxs<<16)/dxs;
	sy=(sys<<16)/dys;

	qx=0; qy=0;
	for(x=0; x<dxs; x++)
		{ ofs_x[x]=qx>>16; qx+=sx; }
	for(y=0; y<dys; y++)
		{ ofs_y[y]=(qy>>16)*sxs; qy+=sy; }
	
	ct=dst;
	for(y=0; y<dys; y++)
		for(x=0; x<dxs; x++)
	{
		*ct++=src[ofs_y[y]+ofs_x[x]];
	}
	return(0);
}

int TKRA_ResampleImage32(
	tkra_rast2pixel *src, int sxs, int sys,
	tkra_rast2pixel *dst, int dxs, int dys)
{
	tkra_rast2pixel *ct, *cs0;
	int ofs_x[1024], ofs_y[1024];
	int x, y, qx, qy, sx, sy;
	
	sx=(sxs<<16)/dxs;
	sy=(sys<<16)/dys;

	qx=0; qy=0;
	for(x=0; x<dxs; x++)
		{ ofs_x[x]=qx>>16; qx+=sx; }
	for(y=0; y<dys; y++)
		{ ofs_y[y]=(qy>>16)*sxs; qy+=sy; }
	
	ct=dst;
	for(y=0; y<dys; y++)
		for(x=0; x<dxs; x++)
	{
		*ct++=src[ofs_y[y]+ofs_x[x]];
	}
	return(0);
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

int TKRA_GetPixel32Luma(int pix)
{
	int cr, cg, cb;
	int y;
	
	cr=(pix>>16)&255;
	cg=(pix>> 8)&255;
	cb=(pix>> 0)&255;

	y=(2*cg+cr+cb)>>2;
	return(y);
}

int TKRA_GetPixel32Alpha(int a)
{
	int av;
	return((a>>24)&255);
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

void TKRA_EncodeBlockUtx3(tkra_rast2pixel *pix, u32 *blk)
{
	byte pxy[16], pxa[16];
	u32 pxv, pxva;
	u64 argb, pvrgb, mrgb, nrgb, a2rgb;
	int mv, nv, my, ny, pv, py;
	int ma, na, pa, ky, ka;
	int acy, aca, isam;
	int i, j, k;
	
	argb=0;
	mv=0xFFFFFFFF;	nv=0xFF000000;
	my=256;		ny=0;
	ma=256;		na=0;
	acy=0;		aca=0;
	for(i=0; i<16; i++)
	{
		pv=pix[i];
		py=TKRA_GetPixel32Luma(pv);
		pa=TKRA_GetPixel32Alpha(pv);
		
		pvrgb=tkra_rgba32upck64(pv);
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
	mrgb =tkra_rgba32upck64(mv);
	nrgb =tkra_rgba32upck64(nv);

	mrgb =tkra_pmuluhw(mrgb, 0xAAAAAAAAAAAAAAAAULL);
	nrgb =tkra_pmuluhw(nrgb, 0xAAAAAAAAAAAAAAAAULL);
	a2rgb=tkra_pmuluhw(argb, 0x5555555555555555ULL);

	mrgb =mrgb+a2rgb;
	nrgb =nrgb+a2rgb;
	
	mrgb&=0x0000FFFFFFFFFFFFULL;
	nrgb&=0x0000FFFFFFFFFFFFULL;
	mrgb|=((u64)ma)<<56;
	nrgb|=((u64)na)<<56;
	
	mv=tkra_rgba32pck64(mrgb);
	nv=tkra_rgba32pck64(nrgb);

	my=TKRA_GetPixel32Luma(mv);
	ny=TKRA_GetPixel32Luma(nv);
#endif

	acy=(my+ny)/2;
	aca=(ma+na)/2;
	
	isam = ((ma&0xF0)!=(na&0xF0)) || ((ma&0xF0)!=0xF0);

	j=ny-acy;
	if(!j)j++;
	ky=384/j;

	j=na-aca;
	if(!j)j++;
	ka=384/j;

	pxv=0;
	pxva=0;
	for(i=0; i<16; i++)
	{
		py=pxy[i];
		pa=pxa[i];
		
		j=2+(((py-acy)*ky)>>8);
		if(j<0)j=0;
		if(j>3)j=3;
		pxv|=j<<(i*2);

		j=2+(((pa-aca)*ka)>>8);
		if(j<0)j=0;
		if(j>3)j=3;
		pxva|=j<<(i*2);
	}
		
	blk[0]=nv;
	blk[1]=mv;
	blk[2]=pxv;
	blk[3]=pxva;
}

void TKRA_EncodeImgMortUtx2(tkra_rastpixel *pix, u32 *blk,
	int xshl, int yshl)
{
	tkra_rastpixel *cs;
	u32 *ct;
	int i, n, shl, xs2, ys2, flip;
	
	flip=0;
	shl=xshl;
	if(yshl>xshl)
		{ shl=yshl; flip=1; }

	i=xshl+yshl-4;
	if(i<0)i=0;
	n=1<<i;

#if 0
	if(shl>=2)
	{
		xs2=xshl-2;
		ys2=yshl-2;
		if(xs2<0)xs2=0;
		if(ys2<0)ys2=0;
//		n=1<<((shl-2)*2);
		n=1<<(xs2+ys2);
	}else
	{
		n=1;
	}
#endif
	
	cs=pix; ct=blk;
	for(i=0; i<n; i++)
	{
		TKRA_EncodeBlockUtx2(cs, ct);
		cs+=16; ct+=2;
	}
}

void TKRA_EncodeImgMortUtx3(tkra_rast2pixel *pix, u32 *blk,
	int xshl, int yshl)
{
	tkra_rast2pixel *cs;
	u32 *ct;
	int i, n, shl, xs2, ys2, flip;
	
	flip=0;
	shl=xshl;
	if(yshl>xshl)
		{ shl=yshl; flip=1; }

	i=xshl+yshl-4;
	if(i<0)i=0;
	n=1<<i;

#if 0
	if(shl>=2)
	{
		xs2=xshl-2;
		ys2=yshl-2;
		if(xs2<0)xs2=0;
		if(ys2<0)ys2=0;
//		n=1<<((shl-2)*2);
		n=1<<(xs2+ys2);
	}else
	{
		n=1;
	}
#endif
	
	cs=pix; ct=blk;
	for(i=0; i<n; i++)
	{
		TKRA_EncodeBlockUtx3(cs, ct);
		cs+=16; ct+=4;
	}
}

void TKRA_UpdateTexImgI(
	TKRA_Context *ctx,
	TKRA_TexImage *img,
	tkra_rastpixel *srcbuf, int xs, int ys, int mip, int flag)
{
	tkra_rastpixel *txbuf;
	int xshl, yshl, xsh1, ysh1, txfl, pix;
	int x, y, m, bsz;
	int i, j, k;
	
	if(xs&(xs-1))
		return;
	if(ys&(ys-1))
		return;

//	flag&=~TKRA_TRFL_DOCMP;
//	flag|=TKRA_TRFL_DOCMP;

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
		if(mip>img->tex_nmip)
			img->tex_nmip=mip;

//		if((xshl==yshl) || ((xshl+1)==yshl) || (xshl==(yshl+1)))
		if(txfl&TKRA_TRFL_MORT)
		{
			txbuf=img->tex_img+img->tex_mipofs[mip];
			for(y=0; y<ys; y++)
				for(x=0; x<xs; x++)
			{
				j=(y<<xshl)+x;
				if(txfl&TKRA_TRFL_FLIPST)
					k=tkra_morton8(y, x);
				else
					k=tkra_morton8(x, y);

				if(txfl&TKRA_TRFL_PIXFMT_DW)
				{
					pix=((tkra_rast2pixel *)srcbuf)[j];
					((tkra_rast2pixel *)txbuf)[k]=pix;
					if((pix&0xFF000000)<0xF0000000)
						txfl|=TKRA_TRFL_ALPHA;
				}else
				{
					pix=srcbuf[j];
	//				img->tex_img[k]=pix;
					txbuf[k]=pix;
					if(pix&0x8000)
						txfl|=TKRA_TRFL_ALPHA;
				}
			}

			if(img->tex_img_bcn)
			{
				if(txfl&TKRA_TRFL_PIXFMT_UTX3)
				{
					TKRA_EncodeImgMortUtx3(
						(tkra_rast2pixel *)
							(img->tex_img+img->tex_mipofs[mip]),
						(tkra_rast2pixel *)
							(img->tex_img_bcn+img->tex_mipofs_bcn[mip]),
						xshl, yshl);
				}else
				{
					TKRA_EncodeImgMortUtx2(
						img->tex_img+img->tex_mipofs[mip],
						img->tex_img_bcn+img->tex_mipofs_bcn[mip],
						xshl, yshl);
				}
			}
		}else
		{
			k=(1<<xsh1)*(1<<ysh1);
			memcpy(img->tex_img+img->tex_mipofs[mip], srcbuf,
				k*sizeof(tkra_rastpixel));
		}

		img->tex_flag=txfl;

	}else if(mip<=0)
	{
		if((xshl!=img->tex_xshl) || (yshl!=img->tex_yshl))
		{
			txfl=0;
			
			if(flag&TKRA_TRFL_PIXFMT_DW)
				txfl|=TKRA_TRFL_PIXFMT_DW;
		
			if(img->tex_img)
			{
				if(img->tex_img != (tkra_rastpixel *)(img->tex_mipofs+4))
				{
					tkra_free_tex(img->tex_img);
				}
				img->tex_img=NULL;
			}
			if(img->tex_img_bcn)
			{
				if(img->tex_img_bcn != (u32 *)(img->tex_mipofs_bcn+4))
					tkra_free_tex(img->tex_img_bcn);
				img->tex_img_bcn=NULL;
			}

			i=0; j=0; xsh1=xshl; ysh1=yshl;
			while((xsh1>0) || (ysh1>0))
			{
				img->tex_mipofs[i]=j;
//				j+=(1<<xsh1)*(1<<ysh1);

				k=(1<<xsh1)*(1<<ysh1);
				if(txfl&TKRA_TRFL_PIXFMT_DW)
					k<<=1;
				if(txfl&TKRA_TRFL_PIXFMT_QW)
					k<<=2;

				k=(k+7)&(~7);
				j+=k;

				i++;
				if(xsh1>0)xsh1--;
				if(ysh1>0)ysh1--;
			}
			img->tex_mipofs[i]=j;
			j+=(1<<xsh1)*(1<<ysh1);

//			if(j<=24)
			if(0)
			{
				img->tex_img=(tkra_rastpixel *)(img->tex_mipofs+4);
			}
			else
			{
				img->tex_img=tkra_malloc_tex(j*sizeof(tkra_rastpixel));
			}
			img->tex_xshl=xshl;
			img->tex_yshl=yshl;
			
			k=i;
			if(k>xshl)k=xshl;
			if(k>yshl)k=yshl;
			img->tex_mmip=k;
			img->tex_nmip=0;
			img->tex_flag=txfl;

			if(txfl&TKRA_TRFL_PIXFMT_DW)
			{
				for(k=0; k<j; k++)
					img->tex_img[k]=0xFFFF;
			}else
			{
				for(k=0; k<j; k++)
					img->tex_img[k]=0x7FFF;
			}

#if 1
//			if((xshl==yshl) && (xshl>=3) && (yshl>=3))
			if((xshl==yshl) && (xshl>=3) && (yshl>=3) &&
				(flag&TKRA_TRFL_DOCMP))
			{
				txfl|=TKRA_TRFL_MORT;
				if(txfl&TKRA_TRFL_PIXFMT_DW)
					txfl|=TKRA_TRFL_PIXFMT_UTX3;

				bsz=1;
				if(txfl&TKRA_TRFL_PIXFMT_UTX3)
					bsz=2;

//				i=0; j=0; xsh1=xshl-2; ysh1=yshl-2;
				i=0; j=0; xsh1=xshl; ysh1=yshl;
				while((xsh1>0) || (ysh1>0))
				{
					img->tex_mipofs_bcn[i]=j;
					k=(xsh1+ysh1)-4;
					if(k<0)k=0;
					k=1<<(k+bsz);
					k=(k+3)&(~3);
					j+=k;

//					j+=1<<(xsh1+ysh1+bsz);

					i++;
					if(xsh1>0)xsh1--;
					if(ysh1>0)ysh1--;
				}
//				img->tex_mipofs_bcn[i+0]=j+(0<<bsz);	//4x4
//				img->tex_mipofs_bcn[i+1]=j+(1<<bsz);	//2x2
//				img->tex_mipofs_bcn[i+2]=j+(2<<bsz);	//1x1
//				i+=2; j+=(3<<bsz);

				img->tex_mipofs_bcn[i]=j;	//4x4
				j+=4;

//				if((j<=6) && (i<4))
				if(0)
				{
					img->tex_img_bcn=(u32 *)(img->tex_mipofs_bcn+4);
				}
				else
					img->tex_img_bcn=tkra_malloc_tex(j*sizeof(u32));
			}
#endif

			img->tex_flag=txfl;
		}

		txfl=img->tex_flag;
		txbuf=img->tex_img;

		if((xshl==yshl) || ((xshl+1)==yshl) || (xshl==(yshl+1)))
		{
			txfl|=TKRA_TRFL_MORT;
			if(xshl==(yshl+1))
				txfl|=TKRA_TRFL_FLIPST;

			for(y=0; y<ys; y++)
				for(x=0; x<xs; x++)
			{
				j=(y<<xshl)+x;
				if(txfl&TKRA_TRFL_FLIPST)
					k=tkra_morton8(y, x);
				else
					k=tkra_morton8(x, y);
				if(txfl&TKRA_TRFL_PIXFMT_DW)
				{
					pix=((tkra_rast2pixel *)srcbuf)[j];
					((tkra_rast2pixel *)txbuf)[k]=pix;
					if(((pix>>24)&0xFF)<0xF0)
						txfl|=TKRA_TRFL_ALPHA;
				}else
				{
					pix=srcbuf[j];
	//				img->tex_img[k]=pix;
					txbuf[k]=pix;
					if(pix&0x8000)
						txfl|=TKRA_TRFL_ALPHA;
				}
			}

#if 1
			if(img->tex_img_bcn)
			{
				if(txfl&TKRA_TRFL_PIXFMT_UTX3)
				{
					TKRA_EncodeImgMortUtx3(
//						img->tex_img+img->tex_mipofs[mip],
						txbuf,
//						img->tex_img_bcn+img->tex_mipofs_bcn[mip],
						img->tex_img_bcn,
						xshl, yshl);
				}else
				{
					TKRA_EncodeImgMortUtx2(
//						img->tex_img+img->tex_mipofs[mip],
						txbuf,
//						img->tex_img_bcn+img->tex_mipofs_bcn[mip],
						img->tex_img_bcn,
						xshl, yshl);
				}
			}
#endif
		}else 
		{
			k=(1<<xshl)*(1<<yshl);
			if(txfl&TKRA_TRFL_PIXFMT_DW)
				k<<=1;
			memcpy(img->tex_img, srcbuf, k*sizeof(tkra_rastpixel));
		}

		img->tex_flag=txfl;

	}
}

void TKRA_UpdateTexImgUtx2I(
	TKRA_Context *ctx,
	TKRA_TexImage *img,
	u64 *buf, int xs, int ys, int mip, int flag)
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

	if((xshl!=yshl) && ((xshl-1)!=yshl) && (xshl!=(yshl-1)))
		return;

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
		if(mip>img->tex_nmip)
			img->tex_nmip=mip;

		if(img->tex_img_bcn)
		{
			timg=(u64 *)(img->tex_img_bcn+img->tex_mipofs_bcn[mip]);
			j=(xshl+yshl)-4;
			if(j<0)j=1;
			n=1<<j;
//			n=xs1*ys1;
			if(txfl&TKRA_TRFL_PIXFMT_UTX3)
				n<<=1;
			for(i=0; i<n; i++)
			{
				blk=buf[i];
				if(txfl&TKRA_TRFL_PIXFMT_UTX3)
				{
					if(!i)
					{
						if(((blk>>24)&0xFF)<0xF0)
							txfl|=TKRA_TRFL_ALPHA;
						if(((blk>>56)&0xFF)<0xF0)
							txfl|=TKRA_TRFL_ALPHA;
					}
				}else
				{
					if(blk&0x80008000ULL)
						txfl|=TKRA_TRFL_ALPHA;
				}
				timg[i]=blk;
			}
		}

		img->tex_flag=txfl;

	}else if(mip<=0)
	{
		if((xshl!=img->tex_xshl) || (yshl!=img->tex_yshl))
		{
			txfl=0;

			if(flag&TKRA_TRFL_PIXFMT_UTX3)
				txfl|=TKRA_TRFL_PIXFMT_UTX3;
		
			if(img->tex_img)
			{
				if(img->tex_img != (tkra_rastpixel *)(img->tex_mipofs+4))
					tkra_free_tex(img->tex_img);
				img->tex_img=NULL;
			}
			if(img->tex_img_bcn)
			{
				if(img->tex_img_bcn != (u32 *)(img->tex_mipofs_bcn+4))
					tkra_free_tex(img->tex_img_bcn);
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
//				img->tex_img=tkra_malloc_tex(j*sizeof(tkra_rastpixel));
			img->tex_xshl=xshl;
			img->tex_yshl=yshl;
			
			k=i;
			if(k>xshl)k=xshl;
			if(k>yshl)k=yshl;
			img->tex_mmip=k;
			img->tex_nmip=0;
			img->tex_flag=txfl;

//			for(k=0; k<j; k++)
//				img->tex_img[k]=0x7FFF;
				
			bsz=1;
			if(txfl&TKRA_TRFL_PIXFMT_UTX3)
				bsz=2;

//			i=0; j=0; xsh1=xshl-2; ysh1=yshl-2;
			i=0; j=0; xsh1=xshl; ysh1=yshl;
			while((xsh1>0) || (ysh1>0))
			{
				k=(xsh1+ysh1)-4;
				if(k<0)k=0;
				img->tex_mipofs_bcn[i]=j;
//				j+=1<<(xsh1+ysh1+bsz);
				k=1<<(k+bsz);
				k=(k+3)&(~3);
				j+=k;
				i++;
				if(xsh1>0)xsh1--;
				if(ysh1>0)ysh1--;
			}

			img->tex_mipofs_bcn[i]=j;
			j+=4;

//			img->tex_mipofs_bcn[i+0]=j+(0<<bsz);	//4x4
//			img->tex_mipofs_bcn[i+1]=j+(1<<bsz);	//2x2
//			img->tex_mipofs_bcn[i+2]=j+(2<<bsz);	//1x1
//			i+=2; j+=(3<<bsz);

//			if((j<=6) && (i<4))
			if(0)
				img->tex_img_bcn=(u32 *)(img->tex_mipofs_bcn+4);
			else
				img->tex_img_bcn=tkra_malloc_tex(j*sizeof(u32));
		}

		txfl=img->tex_flag;
		txfl|=TKRA_TRFL_MORT;

		if(img->tex_img_bcn)
		{
			timg=(u64 *)(img->tex_img_bcn);
//			n=xs1*ys1;

			j=(xshl+yshl)-4;
			if(j<0)j=1;
			n=1<<j;

			if(txfl&TKRA_TRFL_PIXFMT_UTX3)
				n<<=1;

			for(i=0; i<n; i++)
			{
				blk=buf[i];
				if(txfl&TKRA_TRFL_PIXFMT_UTX3)
				{
					if(!i)
					{
						if(((blk>>24)&0xFF)<0xF0)
							txfl|=TKRA_TRFL_ALPHA;
						if(((blk>>56)&0xFF)<0xF0)
							txfl|=TKRA_TRFL_ALPHA;
					}
				}else
				{
					if(blk&0x80008000ULL)
						txfl|=TKRA_TRFL_ALPHA;
				}
				timg[i]=blk;
			}
		}

		img->tex_flag=txfl;
	}
}

u64 TKRA_ConvBlockDxt1ToUtx2A(u64 blk)
{
	u64 blk2;
	u32 pxa, pxb;
	int clra, clrb, clrc, clrd, alpha;
	int rek, ix;
	int x, y, z;
	int i, j, k;
	
//	clra=(blk>> 0)&65535;
//	clrb=(blk>>16)&65535;
	clra=(u16)(blk    );
	clrb=(u16)(blk>>16);
	
	clrc=((clra>>1)&0x7FE0)|(clra&0x001F);
	clrd=((clrb>>1)&0x7FE0)|(clrb&0x001F);
	
//	__debugbreak();
	
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

#if 0
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

#if 1
	y=0;
	z=tkra_morton8(0, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	z=tkra_morton8(1, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	z=tkra_morton8(2, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	z=tkra_morton8(3, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	y=1;
	z=tkra_morton8(0, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	z=tkra_morton8(1, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	z=tkra_morton8(2, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	z=tkra_morton8(3, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	y=2;
	z=tkra_morton8(0, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	z=tkra_morton8(1, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	z=tkra_morton8(2, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	z=tkra_morton8(3, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	y=3;
	z=tkra_morton8(0, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	z=tkra_morton8(1, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	z=tkra_morton8(2, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	z=tkra_morton8(3, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
#endif
	
	blk2=((u64)clrc) | (((u64)clrd)<<16) | (((u64)pxb)<<32);
//	__debugbreak();
	return(blk2);
}

u64 TKRA_ConvBlockDxt1ToUtx2B(u64 blk)
{
	u64 blk2;
	u32 pxa, pxb;
	int clra, clrb, clrc, clrd, alpha;
	int rek, ix;
	int x, y, z;
	int i, j, k;
	
//	clra=(blk>> 0)&65535;
//	clrb=(blk>>16)&65535;
	clra=(u16)(blk    );
	clrb=(u16)(blk>>16);
	
	clrc=((clra>>1)&0x7FE0)|(clra&0x001F);
	clrd=((clrb>>1)&0x7FE0)|(clrb&0x001F);
	rek=0x1203;
	alpha=0;
	
	pxa=(blk>>32); pxb=0; ix=0;

#if 0
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

#if 1
	y=0;
	z=tkra_morton8(0, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	z=tkra_morton8(1, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	z=tkra_morton8(2, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	z=tkra_morton8(3, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	y=1;
	z=tkra_morton8(0, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	z=tkra_morton8(1, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	z=tkra_morton8(2, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	z=tkra_morton8(3, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	y=2;
	z=tkra_morton8(0, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	z=tkra_morton8(1, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	z=tkra_morton8(2, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	z=tkra_morton8(3, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	y=3;
	z=tkra_morton8(0, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	z=tkra_morton8(1, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	z=tkra_morton8(2, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
	z=tkra_morton8(3, y);	i=(pxa>>ix)&3;	ix+=2;
	j=(rek>>(i*4))&3;		pxb|=j<<(z*2);
#endif
	
	blk2=((u64)clrc) | (((u64)clrd)<<16) | (((u64)pxb)<<32);
	return(blk2);
}

u64 TKRA_ConvBlockDxt5ToUtx2(u64 cblk, u64 ablk)
{
	u64 blk2;
	u32 pxa, pxb, pa0, pa1;
	int clra, clrb, clrc, clrd, alpha;
	int rek, ix, ixa;
	int x, y, z, ia;
	int i, j, k;
	
	clra=(cblk>> 0)&65535;
	clrb=(cblk>>16)&65535;
	pa0=(ablk>>0)&255;
	pa1=(ablk>>8)&255;
	
	clrc=((clra>>1)&0x7FE0)|(clra&0x001F);
	clrd=((clrb>>1)&0x7FE0)|(clrb&0x001F);
	
	rek=0x1203;
//	rek=0x0303;
	alpha=0;
	
	if((pa0<224) || (pa1<224))
		alpha=1;

	if(alpha)
	{
		clrc&=~0x0421;
		clrd&=~0x0421;
		clrd|= 0x8000;
		
		if(pa0&0x80)	clrc|=0x0400;
		if(pa0&0x40)	clrc|=0x0020;
		if(pa0&0x20)	clrc|=0x0001;

		if(pa1&0x80)	clrd|=0x0400;
		if(pa1&0x40)	clrd|=0x0020;
		if(pa1&0x20)	clrd|=0x0001;
		
		rek=0x0202;
	}
	
	pxa=(cblk>>32); pxb=0; ix=0; ixa=16;
#if 1
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		z=tkra_morton8(x, y);
		i=(pxa>>ix)&3;
		ia=(ablk>>ixa)&7;
		ix+=2; ixa+=3;
		j=(rek>>(i*4))&3;
		if(alpha && !(ia&1))
			j|=1;
		pxb|=j<<(z*2);
	}
#endif
	
	blk2=((u64)clrc) | (((u64)clrd)<<16) | (((u64)pxb)<<32);
	return(blk2);
}

int TKRA_ConvBlockUtx2ToUtx3(u64 *dblk, u64 sblk)
{
	u64 px;
	u32 pxa, pxb;
	u32 clra, clrb;

	px=tkra_rgbupck64((sblk>> 0)&0xFFFF);
	clra=tkra_rgba32pck64(px);
	px=tkra_rgbupck64((sblk>>16)&0xFFFF);
	clrb=tkra_rgba32pck64(px);

	if((sblk&0x80008000ULL)==0x00000000ULL)
	{
//		clra|=0xFF000000;
//		clrb|=0xFF000000;

		px=clra|(((u64)clrb)<<32);
		dblk[0]=px;

		pxa=sblk>>32;
		px=pxa|(((u64)pxa)<<32);
		dblk[1]=px;
		return(0);
	}

	if((sblk&0x80008000ULL)==0x80008000ULL)
	{
		px=clra|(((u64)clrb)<<32);
		dblk[0]=px;
		pxa=sblk>>32;
		px=pxa|(((u64)pxa)<<32);
		dblk[1]=px;
		return(0);
	}

//	if((sblk&0x80008000)==0x00008000)
	if(1)
	{
		px=tkra_rgbupck64(0x8000|((sblk>> 0)&0xFFFF));
		clra=tkra_rgba32pck64(px);
		px=tkra_rgbupck64(0x8000|((sblk>>16)&0xFFFF));
		clrb=tkra_rgba32pck64(px);

		px=clra|(((u64)clrb)<<32);
		dblk[0]=px;
		
		px=sblk>>32;
		pxa=px&0xAAAAAAAA;
		pxb=px&0x55555555;
		pxa|=pxa>>1;
		pxb|=pxb<<1;
		px=pxa|(((u64)pxb)<<32);
		dblk[1]=px;
		return(0);
	}

	return(0);
}

u32 tkra_teximg_repackrgba_ldr2hdr(u32 cv);

int TKRA_ConvBlockUtx2ToUtx3H(u64 *dblk, u64 sblk)
{
	u32 c0, c1;
	u64 cv;

	TKRA_ConvBlockUtx2ToUtx3(dblk, sblk);
	
	tkra_teximg_inithdr();
	
	cv=*dblk;
	c0=tkra_teximg_repackrgba_ldr2hdr((u32)(cv>> 0));
	c1=tkra_teximg_repackrgba_ldr2hdr((u32)(cv>>32));
	cv=(((u64)c1)<<32)|c0;
	*dblk=cv;
	return(0);
}

void TKRA_UpdateTexImgDxt1(
	TKRA_Context *ctx,
	TKRA_TexImage *img,
	u64 *src, int xs, int ys, int mip, int flag)
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
	
//	tk_printf("TKRA_UpdateTexImgDxt1: %s:%d\n", __FILE__, __LINE__);
	
	if(!tmpbuf)
	{
//		tmpbuf=tkra_malloc(128*128*8);
		tmpbuf=tk_malloc(128*128*16);
	}

//	if(flag&TKRA_TRFL_PIXFMT_DW)
	if(ctx->pixelfmt&TKRA_PIXFMT_DW)
	{
		flag|=TKRA_TRFL_PIXFMT_UTX3;
	}

	if(ctx->pixelfmt&TKRA_PIXFMT_HDR)
	{
		flag|=TKRA_TRFL_PIXFMT_HDR;
	}

//	tk_printf("TKRA_UpdateTexImgDxt1: %s:%d\n", __FILE__, __LINE__);

	if(flag&TKRA_TRFL_ALPHA)
	{
//		tk_printf("TKRA_UpdateTexImgDxt1: %s:%d\n", __FILE__, __LINE__);

		xs1=(xs+3)>>2;
		ys1=(ys+3)>>2;
		for(y=0; y<ys1; y++)
			for(x=0; x<xs1; x++)
		{
			sblk=src[y*xs1+x];		z=tkra_morton8(x, y);
			dblk=TKRA_ConvBlockDxt1ToUtx2A(sblk);
			if(flag&TKRA_TRFL_PIXFMT_UTX3)
			{
				if(flag&TKRA_TRFL_PIXFMT_HDR)
					TKRA_ConvBlockUtx2ToUtx3H(tmpbuf+z*2, dblk);
				else
					TKRA_ConvBlockUtx2ToUtx3(tmpbuf+z*2, dblk);
			}else
			{
				tmpbuf[z]=dblk;
			}
		}
	}else
	{
//		tk_printf("TKRA_UpdateTexImgDxt1: %s:%d\n", __FILE__, __LINE__);

		xs1=(xs+3)>>2;
		ys1=(ys+3)>>2;
		for(y=0; y<ys1; y++)
			for(x=0; x<xs1; x++)
		{
			sblk=src[y*xs1+x];		z=tkra_morton8(x, y);
			dblk=TKRA_ConvBlockDxt1ToUtx2B(sblk);
			if(flag&TKRA_TRFL_PIXFMT_UTX3)
			{
				if(flag&TKRA_TRFL_PIXFMT_HDR)
					TKRA_ConvBlockUtx2ToUtx3H(tmpbuf+z*2, dblk);
				else
					TKRA_ConvBlockUtx2ToUtx3(tmpbuf+z*2, dblk);
			}else
			{
				tmpbuf[z]=dblk;
			}
		}
	}

//	tk_printf("TKRA_UpdateTexImgDxt1: %s:%d\n", __FILE__, __LINE__);

	TKRA_UpdateTexImgUtx2(ctx, img, tmpbuf, xs, ys, mip, flag);

//	tk_printf("TKRA_UpdateTexImgDxt1: %s:%d\n", __FILE__, __LINE__);
}

void TKRA_UpdateTexImgDxt5(
	TKRA_Context *ctx,
	TKRA_TexImage *img,
	u64 *src, int xs, int ys, int mip, int flag)
{
	static u64 *tmpbuf=NULL;
	u64 sblk, ablk, dblk;
	int xs1, ys1, x, y, z;
	
	if(xs!=ys)
		return;
	if(xs<0)
		return;
	if(xs>256)
		return;
	
	if(!tmpbuf)
	{
//		tmpbuf=tkra_malloc(128*128*8);
		tmpbuf=tk_malloc(128*128*16);
	}
	
	if(ctx->pixelfmt&TKRA_PIXFMT_DW)
	{
		flag|=TKRA_TRFL_PIXFMT_UTX3;
	}

	if(ctx->pixelfmt&TKRA_PIXFMT_HDR)
	{
		flag|=TKRA_TRFL_PIXFMT_HDR;
	}

	xs1=(xs+3)>>2;
	ys1=(ys+3)>>2;
	for(y=0; y<ys1; y++)
		for(x=0; x<xs1; x++)
	{
		sblk=src[(y*xs1+x)*2+1];
		ablk=src[(y*xs1+x)*2+0];
		z=tkra_morton8(x, y);
		if(flag&TKRA_TRFL_PIXFMT_UTX3)
		{
			dblk=TKRA_ConvBlockDxt5ToUtx2(sblk, ablk);
			if(flag&TKRA_TRFL_PIXFMT_HDR)
				TKRA_ConvBlockUtx2ToUtx3H(tmpbuf+z*2, dblk);
			else
				TKRA_ConvBlockUtx2ToUtx3(tmpbuf+z*2, dblk);
		}else
		{
			dblk=TKRA_ConvBlockDxt5ToUtx2(sblk, ablk);
			tmpbuf[z]=dblk;
		}
	}

	TKRA_UpdateTexImgUtx2(ctx, img, tmpbuf, xs, ys, mip, flag);
}

TKRA_TexImage *TKRA_LookupTexImgI(TKRA_Context *ctx, int num)
{
	TKRA_TexImage *cur;
	int h;
	
	h=(num+(num>>8)+(num>>16))&255;
	cur=ctx->svctx->tex_hash[h];
	while(cur)
	{
		if(cur->tex_id==num)
			return(cur);
		cur=cur->chain;
	}
	return(NULL);
}

TKRA_TexImage *TKRA_GetTexImgI(TKRA_Context *ctx, int num)
{
	TKRA_TexImage *cur;
	int h;
	
	h=(num+(num>>8)+(num>>16))&255;
	cur=ctx->svctx->tex_hash[h];
	while(cur)
	{
		if(cur->tex_id==num)
			return(cur);
		cur=cur->chain;
	}
	
	cur=TKRA_AllocTexImg(ctx);
	cur->tex_id=num;
	cur->chain=ctx->svctx->tex_hash[h];
	ctx->svctx->tex_hash[h]=cur;
	
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
	
	id=ctx->svctx->tex_rov;
	ctx->svctx->tex_rov=(id+1);
	return(id);
}

int TKRA_BindTexImg2I(TKRA_Context *ctx, TKRA_TexImage *img, int unit)
{
	TKRA_SvContext *sctx;
	
	sctx=ctx->svctx;

	if(unit==0)
	{
		if(img)
		{
			sctx->tex_cur=img;
			sctx->tex_xshl=img->tex_xshl;
			sctx->tex_yshl=img->tex_yshl;
			sctx->tex_mmip=img->tex_mmip;
			sctx->tex_nmip=img->tex_nmip;
			sctx->tex_flag=img->tex_flag;
		}else
		{
			sctx->tex_cur=NULL;
			sctx->tex_xshl=0;
			sctx->tex_yshl=0;
			sctx->tex_mmip=1;
			sctx->tex_nmip=1;
			sctx->tex_flag=0;
		}

		sctx->tex_img=NULL;
		sctx->tex_img_bcn=NULL;

		sctx->span_trifl=-1;
		sctx->span_tex_cur=NULL;
	}

	if(unit==1)
	{
		if(img)
		{
			sctx->tex_cur2=img;
			sctx->tex_xshl2=img->tex_xshl;
			sctx->tex_yshl2=img->tex_yshl;
			sctx->tex_mmip2=img->tex_mmip;
			sctx->tex_nmip2=img->tex_nmip;
			sctx->tex_flag2=img->tex_flag;
		}else
		{
			sctx->tex_cur2=NULL;
			sctx->tex_xshl2=0;
			sctx->tex_yshl2=0;
			sctx->tex_mmip2=1;
			sctx->tex_nmip2=1;
			sctx->tex_flag2=0;
		}

		sctx->tex_img2=NULL;
		sctx->tex_img_bcn2=NULL;

//		sctx->span_trifl=-1;
//		sctx->span_tex_cur=NULL;
	}

	TKRA_SetupForState(ctx);
	
	return(0);
}

int TKRA_BindTexImgI(TKRA_Context *ctx, TKRA_TexImage *img)
{
	return(TKRA_BindTexImg2I(ctx, img, ctx->tex2d_active));
}

int TKRA_SetupForState(TKRA_Context *ctx)
{
	TKRA_SetupDrawEdgeForState(ctx);
	return(0);
}

int TKRA_CheckBoundTexImgIdx(TKRA_Context *ctx, int unit, int texid)
{
	TKRA_SvContext *sctx;
	TKRA_TexImage *img;
	
	sctx=ctx->svctx;

	if(unit==0)
	{
		img=sctx->tex_cur;
		if(!img || (img->tex_id!=texid) || !(ctx->bindsticky))
		{
			img=TKRA_GetTexImgI(ctx, texid);
			TKRA_BindTexImg2I(ctx, img, 0);
		}
	}

	if(unit==1)
	{
		img=sctx->tex_cur2;
		if(!img || (img->tex_id!=texid) || !(ctx->bindsticky))
		{
			if(texid>0)
			{
				img=TKRA_GetTexImgI(ctx, texid);
				TKRA_BindTexImg2I(ctx, img, 1);
			}else if(img)
			{
				TKRA_BindTexImg2I(ctx, NULL, 1);
			}
		}
	}

	return(0);
}

int TKRA_CheckBoundTexImg(TKRA_Context *ctx)
{
	if(ctx->tex2d_enamask&1)
	{
		TKRA_CheckBoundTexImgIdx(ctx, 0, ctx->boundtexid[0]);
	}else
	{
		TKRA_CheckBoundTexImgIdx(ctx, 0, 0);
	}

	if(ctx->tex2d_enamask&2)
	{
		TKRA_CheckBoundTexImgIdx(ctx, 1, ctx->boundtexid[1]);
	}else
	{
		TKRA_CheckBoundTexImgIdx(ctx, 1, 0);
	}

	ctx->bindsticky=1;

	return(0);
}
