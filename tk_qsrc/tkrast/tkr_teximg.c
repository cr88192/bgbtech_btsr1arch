void	*tkra_freelist[256];
void	*tkra_freelist_phys[256];
int		tkra_totbrk=0;

#ifdef GLQUAKE
void *Q_MallocLLn(int sz, char *lfn, int lln);
void Q_FreeLLn(void *ptr, char *lfn, int lln);

void *TKRA_Brk(int sz)
{
	byte *ptr;
	tkra_totbrk+=sz;
	
	ptr=Q_MallocLLn(sz+16, __FILE__, __LINE__);
	while(((long)ptr)&15)
		ptr++;
	return(ptr);
}
#else
void *TKRA_Brk(int sz)
{
	byte *ptr;
	tkra_totbrk+=sz;
	ptr=malloc(sz);
	while(((long)ptr)&15)
		ptr++;
	return(ptr);
}
#endif

void *TKRA_BrkPhys(int sz)
{
	byte *ptr;
	tkra_totbrk+=sz;
#ifdef __BJX2__
	ptr=tk_malloc_cat(sz+16, TKMM_MCAT_PHYSDFL);
	while(((long)ptr)&15)
		ptr++;
	return(ptr);
#else
	return(TKRA_Brk(sz));
#endif
}

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
	
	ix=TKRA_MallocIndexForSize(sz+16);
	
	ptr=tkra_freelist[ix];
	if(ptr)
	{
		tkra_freelist[ix]=*(byte **)ptr;
		((int *)ptr)[0]=ix;
		((int *)ptr)[1]=0x6789;
		return(ptr+16);
	}
	
	sz1=TKRA_SizeForIndex(ix);
	ptr=TKRA_Brk(sz1);
	((int *)ptr)[0]=ix;
	((int *)ptr)[1]=0x6789;
	return(ptr+16);
}

void tkra_free(void *ptr)
{
	byte *ptr1;
	int ix, tag;
	
	if(!ptr)
		return;

	ptr1=((byte *)ptr)-16;
	ix=((int *)ptr1)[0];
	tag=((int *)ptr1)[1];
	
	if(tag==0x6789)
	{
		*(byte **)ptr1=tkra_freelist[ix];
		tkra_freelist[ix]=ptr1;
	}else
		if(tag==0x1234)
	{
		*(byte **)ptr1=tkra_freelist_phys[ix];
		tkra_freelist_phys[ix]=ptr1;
	}else
	{
		printf("tkra_free: Bad Tag %08X\n", tag);
	}
}

void *tkra_malloc_phys(int sz)
{
	byte *ptr;
	int ix, sz1;
	
	ix=TKRA_MallocIndexForSize(sz+16);
	
	ptr=tkra_freelist_phys[ix];
	if(ptr)
	{
		tkra_freelist_phys[ix]=*(byte **)ptr;
		((int *)ptr)[0]=ix;
		((int *)ptr)[1]=0x1234;
		return(ptr+16);
	}
	
	sz1=TKRA_SizeForIndex(ix);
	ptr=TKRA_BrkPhys(sz1);
	((int *)ptr)[0]=ix;
	((int *)ptr)[1]=0x1234;
	return(ptr+16);
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
	tkra_rastpixel *buf, int xs, int ys, int mip, int flag)
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
		if(mip>img->tex_nmip)
			img->tex_nmip=mip;

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
//				if(img->tex_img != (tkra_rastpixel *)(img->tex_mipofs+4))
				if(img->tex_img_org &&
					(img->tex_img_org != (tkra_rastpixel *)(img->tex_mipofs+4)))
				{
					tkra_free(img->tex_img_org);
				}
				img->tex_img=NULL;
				img->tex_img_org=NULL;
			}
			if(img->tex_img_bcn)
			{
//				if(img->tex_img_bcn != (u32 *)(img->tex_mipofs_bcn+4))
				if(img->tex_img_bcn_org &&
					(img->tex_img_bcn_org != (u32 *)(img->tex_mipofs_bcn+4)))
				{
					tkra_free(img->tex_img_bcn_org);
				}
				img->tex_img_bcn=NULL;
				img->tex_img_bcn_org=NULL;
			}

			i=0; j=0; xsh1=xshl; ysh1=yshl;
			while((xsh1>0) || (ysh1>0))
			{
				img->tex_mipofs[i]=j;
				j+=(1<<xsh1)*(1<<ysh1);
				j=(j+7)&(~7);
				i++;
				if(xsh1>0)xsh1--;
				if(ysh1>0)ysh1--;
			}
			img->tex_mipofs[i]=j;
			j+=(1<<xsh1)*(1<<ysh1);
			j=(j+7)&(~7);

//			if(j<=24)
//				img->tex_img=(tkra_rastpixel *)(img->tex_mipofs+4);
//			else
//				img->tex_img=tkra_malloc(j*sizeof(tkra_rastpixel));
			img->tex_xshl=xshl;
			img->tex_yshl=yshl;

			img->tex_img=tkra_malloc_phys(j*sizeof(tkra_rastpixel));
			img->tex_img_org=img->tex_img;
//			while(((int)(img->tex_img))&15)
//				img->tex_img++;

			k=i;
			if(k>xshl)k=xshl;
			if(k>yshl)k=yshl;
			img->tex_mmip=k;
			img->tex_nmip=0;
			img->tex_flag=0;

			for(k=0; k<j; k++)
				img->tex_img[k]=0x7FFF;

#if 1
//			if((xshl==yshl) && (xshl>=3) && (yshl>=3))
//			if((xshl==yshl) && (xshl>=3) && (yshl>=3) &&
//				(flag&TKRA_TRFL_DOCMP))
			if((xshl==yshl) && (xshl>=2) && (yshl>=2) &&
				(flag&TKRA_TRFL_DOCMP))
			{
				bsz=1;
				i=0; j=0; xsh1=xshl-2; ysh1=yshl-2;
				while((xsh1>0) || (ysh1>0))
				{
					img->tex_mipofs_bcn[i]=j;
					j+=1<<(xsh1+ysh1+bsz);
					j=(j+3)&(~3);
					i++;
					if(xsh1>0)xsh1--;
					if(ysh1>0)ysh1--;
				}
//				img->tex_mipofs_bcn[i+0]=j+(0<<bsz);	//4x4
//				img->tex_mipofs_bcn[i+1]=j+(1<<bsz);	//2x2
//				img->tex_mipofs_bcn[i+2]=j+(2<<bsz);	//1x1

				img->tex_mipofs_bcn[i+0]=j;	//4x4
				j+=1<<bsz;	j=(j+3)&(~3);
				img->tex_mipofs_bcn[i+1]=j;	//2x2
				j+=1<<bsz;	j=(j+3)&(~3);
				img->tex_mipofs_bcn[i+2]=j;	//1x1
				j+=1<<bsz;	j=(j+3)&(~3);

				i+=2;
				//j+=(3<<bsz);

#if 0
				if((j<=6) && (i<4))
					img->tex_img_bcn=(u32 *)(img->tex_mipofs_bcn+4);
				else
//					img->tex_img_bcn=tkra_malloc(j*sizeof(u32));
					img->tex_img_bcn=tkra_malloc_phys(j*sizeof(u32));
#endif

				img->tex_img_bcn=tkra_malloc_phys(j*sizeof(u32));
				img->tex_img_bcn_org=img->tex_img_bcn;
//				while(((int)(img->tex_img_bcn))&15)
//					img->tex_img_bcn++;
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
		txfl|=TKRA_TRFL_HASMIP;
		if(mip>img->tex_nmip)
			img->tex_nmip=mip;

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
				if(img->tex_img_org &&
					(img->tex_img_org != (tkra_rastpixel *)(img->tex_mipofs+4)))
				{
					tkra_free(img->tex_img_org);
				}
				img->tex_img=NULL;
				img->tex_img_org=NULL;
			}
			if(img->tex_img_bcn)
			{
				if(img->tex_img_bcn_org &&
					(img->tex_img_bcn_org != (u32 *)(img->tex_mipofs_bcn+4)))
				{
					tkra_free(img->tex_img_bcn_org);
				}
				img->tex_img_bcn=NULL;
				img->tex_img_bcn_org=NULL;
			}

			i=0; j=0; xsh1=xshl; ysh1=yshl;
			while((xsh1>0) || (ysh1>0))
			{
				img->tex_mipofs[i]=j;
				j+=(1<<xsh1)*(1<<ysh1);
				j=(j+7)&(~7);
				i++;
				if(xsh1>0)xsh1--;
				if(ysh1>0)ysh1--;
			}
			img->tex_mipofs[i]=j;
			j+=(1<<xsh1)*(1<<ysh1);
			j=(j+7)&(~7);

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
			img->tex_nmip=0;
			img->tex_flag=0;

//			for(k=0; k<j; k++)
//				img->tex_img[k]=0x7FFF;
				
			bsz=1;
			i=0; j=0; xsh1=xshl-2; ysh1=yshl-2;
			while((xsh1>0) || (ysh1>0))
			{
				img->tex_mipofs_bcn[i]=j;
				j+=1<<(xsh1+ysh1+bsz);
				j=(j+3)&(~3);
				i++;
				if(xsh1>0)xsh1--;
				if(ysh1>0)ysh1--;
			}
//			img->tex_mipofs_bcn[i+0]=j+(0<<bsz);	//4x4
//			img->tex_mipofs_bcn[i+1]=j+(1<<bsz);	//2x2
//			img->tex_mipofs_bcn[i+2]=j+(2<<bsz);	//1x1

			img->tex_mipofs_bcn[i+0]=j;	//4x4
			j+=1<<bsz;	j=(j+3)&(~3);
			img->tex_mipofs_bcn[i+1]=j;	//2x2
			j+=1<<bsz;	j=(j+3)&(~3);
			img->tex_mipofs_bcn[i+2]=j;	//1x1
			j+=1<<bsz;	j=(j+3)&(~3);

			i+=2;
//			j+=(3<<bsz);

//			if((j<=6) && (i<4))
//				img->tex_img_bcn=(u32 *)(img->tex_mipofs_bcn+4);
//			else
//				img->tex_img_bcn=tkra_malloc(j*sizeof(u32));

			img->tex_img_bcn=tkra_malloc_phys(j*sizeof(u32));
			img->tex_img_bcn_org=img->tex_img_bcn;
			
//			while(((int)(img->tex_img_bcn))&15)
//				img->tex_img_bcn++;
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

void TKRA_UpdateTexImgDxt1(TKRA_TexImage *img,
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
	
	if(!tmpbuf)
	{
		tmpbuf=tkra_malloc(64*64*8);
	}
	
	if(flag&TKRA_TRFL_ALPHA)
	{
		xs1=(xs+3)>>2;
		ys1=(ys+3)>>2;
		for(y=0; y<ys1; y++)
			for(x=0; x<xs1; x++)
		{
			sblk=src[y*xs1+x];		z=tkra_morton8(x, y);
			dblk=TKRA_ConvBlockDxt1ToUtx2A(sblk);
			tmpbuf[z]=dblk;
		}
	}else
	{
		xs1=(xs+3)>>2;
		ys1=(ys+3)>>2;
		for(y=0; y<ys1; y++)
			for(x=0; x<xs1; x++)
		{
			sblk=src[y*xs1+x];		z=tkra_morton8(x, y);
			dblk=TKRA_ConvBlockDxt1ToUtx2B(sblk);
			tmpbuf[z]=dblk;
		}
	}

	TKRA_UpdateTexImgUtx2(img, tmpbuf, xs, ys, mip);
}

void TKRA_UpdateTexImgDxt5(TKRA_TexImage *img,
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
		tmpbuf=tkra_malloc(64*64*8);
	}
	
	xs1=(xs+3)>>2;
	ys1=(ys+3)>>2;
	for(y=0; y<ys1; y++)
		for(x=0; x<xs1; x++)
	{
		sblk=src[(y*xs1+x)*2+1];
		ablk=src[(y*xs1+x)*2+0];
		z=tkra_morton8(x, y);
		dblk=TKRA_ConvBlockDxt5ToUtx2(sblk, ablk);
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
	cur->tex_img_org=NULL;
	cur->tex_img[0]=0x7FFF;
	cur->tex_img_bcn=NULL;
	cur->tex_img_bcn_org=NULL;
	
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
//	ctx->tex_img=img->tex_img;
//	ctx->tex_img_bcn=img->tex_img_bcn;
	ctx->tex_xshl=img->tex_xshl;
	ctx->tex_yshl=img->tex_yshl;
	ctx->tex_mmip=img->tex_mmip;
	ctx->tex_nmip=img->tex_nmip;
	ctx->tex_flag=img->tex_flag;

	ctx->tex_img=NULL;
	ctx->tex_img_bcn=NULL;
//	ctx->tex_img_org=NULL;
//	ctx->tex_img_bcn_org=NULL;

	ctx->span_trifl=-1;
	ctx->span_tex_cur=NULL;

	TKRA_SetupForState(ctx);
	
	return(0);
}

int TKRA_SetupForState(TKRA_Context *ctx)
{
	TKRA_SetupDrawEdgeForState(ctx);
	return(0);
}
