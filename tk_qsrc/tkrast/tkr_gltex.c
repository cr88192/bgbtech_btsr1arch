int tkra_teximg_packrgb555(int cr, int cg, int cb, int ca)
{
	int v;
	
	cr>>=3;	cg>>=3;	cb>>=3;
	v=(cr<<10)|(cg<<5)|cb;
//	if(ca<128)
//		v|=0x8000;
	if((ca&0xF0)!=0xF0)
	{
		v&=0x7BDE;
		v|=0x8000;
		if(ca&0x80)
			v|=0x0400;
		if(ca&0x40)
			v|=0x0020;
		if(ca&0x20)
			v|=0x0001;
	}
	
	return(v);
}

u32 tkra_teximg_packrgba(int cr, int cg, int cb, int ca)
{
	int v;	
	v=(ca<<24)|(cr<<16)|(cg<<8)|cb;
	return(v);
}

void tkra_glTexImage2D(
	int target,
 	int level,
 	int internalformat,
 	int width,
 	int height,
 	int border,
 	int format,
 	int type,
 	const void *data)
{
	static u16 texlumatab[256];
	static tkra_rastpixel *txbuf;
	static tkra_rastpixel *txbuf2;
	static int sz_txbuf;
	static int sz_txbuf2;

	static tkra_rastpixel *ixbuf;

	TKRA_Context *ctx;
	TKRA_TexImage *img;
	int		width2, height2;
	int		ixw, ixh, flag;
	byte *cs;
	int i0, i1, i2, i3;
	int i, j, k;

	if(target!=TKRA_TEXTURE_2D)
		return;
	
//	if(width&(width-1))
//		return;
//	if(height&(height-1))
//		return;

	flag=0;
	if(	(internalformat==TKRA_GL_RGB_S3TC) ||
		(internalformat==TKRA_GL_RGBA_S3TC) ||
		(internalformat==TKRA_GL_CMPR_RGB_S3TC_DXT1) ||
		(internalformat==TKRA_GL_CMPR_RGBA_S3TC_DXT1))
	{
		flag|=TKRA_TRFL_DOCMP;
	}
	
	if(	(internalformat==TKRA_GL_RGB_S3TC) ||
		(internalformat==TKRA_GL_CMPR_RGB_S3TC_DXT1) ||
		(internalformat==GL_RGB8) ||
		(internalformat==GL_RGB5) ||
		(internalformat==3) ||
		(internalformat==1))
	{
		flag|=TKRA_TRFL_NOALPHA;
	}
	
//	if(internalformat==4)
//	if(format!=TKRA_LUMINANCE)
	if(1)
	{
		flag|=TKRA_TRFL_DOCMP;
	}

	if(!txbuf)
	{
		k=256*256*2;
		txbuf=tkra_malloc(k*2);
		sz_txbuf=k;
	}

	k=width*height*2;
	if(k>sz_txbuf)
	{
		if(txbuf)
			tkra_free(txbuf);
		txbuf=tkra_malloc(k*2);
		sz_txbuf=k;
	}
	
	if((type==TKRA_UBYTE) || (type==TKRA_BYTE))
	{
		cs=(byte *)data;
		if(format==TKRA_RGB)
		{
			k=width*height;
			for(i=0; i<k; i++)
			{
				txbuf[i]=tkra_teximg_packrgb555(cs[0], cs[1], cs[2], 255);
				cs+=3;
			}
		}else
			if(format==TKRA_RGBA)
		{
			if(flag&TKRA_TRFL_NOALPHA)
			{
				k=width*height;
				for(i=0; i<k; i++)
				{
					txbuf[i]=tkra_teximg_packrgb555(cs[0], cs[1], cs[2], 255);
					cs+=4;
				}
			}else
			{
				k=width*height;
				for(i=0; i<k; i++)
				{
					txbuf[i]=tkra_teximg_packrgb555(cs[0], cs[1], cs[2], cs[3]);
					cs+=4;
				}
			}
		}else
			if(format==TKRA_BGR)
		{
			k=width*height;
			for(i=0; i<k; i++)
			{
				txbuf[i]=tkra_teximg_packrgb555(cs[2], cs[1], cs[0], 255);
				cs+=3;
			}
		}else
			if(format==TKRA_BGRA)
		{
			if(flag&TKRA_TRFL_NOALPHA)
			{
				k=width*height;
				for(i=0; i<k; i++)
				{
					txbuf[i]=tkra_teximg_packrgb555(cs[2], cs[1], cs[0], 255);
					cs+=4;
				}
			}else
			{
				k=width*height;
				for(i=0; i<k; i++)
				{
					txbuf[i]=tkra_teximg_packrgb555(cs[2], cs[1], cs[0], cs[3]);
					cs+=4;
				}
			}
		}else
//			if((format==TKRA_LUMINANCE) || (format==TKRA_INTENSITY))
			if(format==TKRA_LUMINANCE)
		{
			if(!texlumatab[255])
			{
				for(i=0; i<256; i++)
					texlumatab[i]=tkra_teximg_packrgb555(i, i, i, 255);
			}
		
			flag|=TKRA_TRFL_NOALPHA;
			k=width*height;
#if 0
			for(i=0; i<k; i++)
			{
//				txbuf[i]=tkra_teximg_packrgb555(cs[0], cs[0], cs[0], 255);
				txbuf[i]=texlumatab[cs[0]];
				cs++;
			}
#endif

#if 1
			for(i=0; i<k; i+=4)
			{
				i0=cs[0];				i1=cs[1];
				i2=cs[2];				i3=cs[3];
				i0=texlumatab[i0];		i1=texlumatab[i1];
				i2=texlumatab[i2];		i3=texlumatab[i3];
				txbuf[i+0]=i0;			txbuf[i+1]=i1;
				txbuf[i+2]=i2;			txbuf[i+3]=i3;
				cs+=4;
			}
#endif
		}
		else
		{
			__debugbreak();
		}
	}else
	{
		__debugbreak();
	}

	ixbuf=txbuf;
	ixw=width;
	ixh=height;

	if(	(width&(width-1))	||
		(height&(height-1))	||
		(width!=height)		)
	{
		width2=1;
		height2=1;
		while(width2<width)
			width2<<=1;
		while(height2<height)
			height2<<=1;
		
		if(width2<height2)
			width2=height2;
		if(height2<width2)
			height2=width2;

		if(!txbuf2)
		{
			k=256*256*2;
			txbuf2=tkra_malloc(k*2);
			sz_txbuf2=k;
		}

		k=width2*height2*2;
		if(k>sz_txbuf2)
		{
			if(txbuf2)
				tkra_free(txbuf2);
			txbuf2=tkra_malloc(k*2);
			sz_txbuf2=k;
		}

		ixbuf=txbuf2;
		ixw=width2;
		ixh=height2;

		TKRA_ResampleImage555(
			txbuf, width, height,
			txbuf2, width2, height2);

	}

	ctx=TKRA_GetCurrentContext();
	img=ctx->tex_cur;

//	TKRA_UpdateTexImg(img, txbuf, width, height, level);
	TKRA_UpdateTexImg(img, ixbuf, ixw, ixh, level, flag);
	TKRA_BindTexImg(ctx, img);
}

void tkra_glTexSubImage2D(
	int target, int level,
	int xoffset, int yoffset,
	int width, int height,
	int format, int type,
	void *pixels )
{
	TKRA_Context *ctx;
	TKRA_TexImage *img;
	int ifmt;

	if(target!=TKRA_TEXTURE_2D)
		return;
	
//	if(width&(width-1))
//		return;
//	if(height&(height-1))
//		return;

	ctx=TKRA_GetCurrentContext();
	img=ctx->tex_cur;

	if((xoffset==0) && (yoffset==0) &&
		(width==(1<<img->tex_xshl)) &&
		(height==(1<<img->tex_yshl))	)
	{
		ifmt=(img->tex_flag&TKRA_TRFL_ALPHA)?4:3;
		if(format==TKRA_LUMINANCE)
			ifmt=1;

		tkra_glTexImage2D(
			target,	level, ifmt,
			width, height, 0,
			format, type, pixels);
		return;
	}
}

void tkra_glCompressedTexImage2D(int target,
	int level, int internalformat,
	int width, int height, int border,
	int imageSize, void * data)
{
	TKRA_Context *ctx;
	TKRA_TexImage *img;
	byte *cs;
	int i, j, k;

	if(target!=TKRA_TEXTURE_2D)
		return;
	
	if(width&(width-1))
		return;
	if(height&(height-1))
		return;


	ctx=TKRA_GetCurrentContext();
	img=ctx->tex_cur;

	if(internalformat==TKRA_GL_CMPR_RGB_S3TC_DXT1)
	{
		TKRA_UpdateTexImgDxt1(img, data, width, height, level,
			TKRA_TRFL_NOALPHA);
	}else
		if(internalformat==TKRA_GL_CMPR_RGBA_S3TC_DXT1)
	{
		TKRA_UpdateTexImgDxt1(img, data, width, height, level,
			TKRA_TRFL_ALPHA);
	}else
		if(internalformat==TKRA_GL_CMPR_RGBA_S3TC_DXT5)
	{
		TKRA_UpdateTexImgDxt5(img, data, width, height, level,
			TKRA_TRFL_ALPHA);
	}

	TKRA_BindTexImg(ctx, img);
	return;
}

void tkra_glBindTexture(int target, int texture)
{
	TKRA_Context *ctx;
	TKRA_TexImage *img;
	int i;

	if(target!=TKRA_TEXTURE_2D)
		return;

	ctx=TKRA_GetCurrentContext();

	img=TKRA_GetTexImg(ctx, texture);
	
	i=ctx->tex2d_active;
//	if(i)
	if(1)
	{
		ctx->tex_cur_mtx[i&7]=img;
		TKRA_BindTexImg(ctx, img);
	}else
	{
		ctx->tex_cur_mtx[0]=img;
		TKRA_BindTexImg(ctx, img);
	}


//	img=ctx->tex_cur;
//	TKRA_UpdateTexImg(img, txbuf, width, height, level);
}

void tkra_glTexParameteri(int target, int pname, int param);

void tkra_glTexParameterf(int target, int pname, float param)
{
	switch(pname)
	{
	case TKRA_GL_TEXTURE_MIN_FILTER:
	case TKRA_GL_TEXTURE_MAG_FILTER:
	case TKRA_GL_TEXTURE_WRAP_S:
	case TKRA_GL_TEXTURE_WRAP_T:
		tkra_glTexParameteri(target, pname, param);
		break;
	}
}

void tkra_glTexParameteri(int target, int pname, int param)
{
	TKRA_Context *ctx;
	TKRA_TexImage *img;
	int		txfl;

	ctx=TKRA_GetCurrentContext();
	img=ctx->tex_cur;
	if(!img)
		return;
	
	txfl=img->tex_flag;

	switch(pname)
	{
	case TKRA_GL_TEXTURE_MIN_FILTER:
		switch(param)
		{
		case TKRA_GL_NEAREST:
		case TKRA_GL_NEAREST_MIPMAP_NEAREST:
		case TKRA_GL_NEAREST_MIPMAP_LINEAR:
			txfl&=~TKRA_TRFL_MINBL;
			break;
		case TKRA_GL_LINEAR:
		case TKRA_GL_LINEAR_MIPMAP_NEAREST:
		case TKRA_GL_LINEAR_MIPMAP_LINEAR:
			txfl|=TKRA_TRFL_MINBL;
			break;
		}
		break;
	case TKRA_GL_TEXTURE_MAG_FILTER:
		switch(param)
		{
		case TKRA_GL_NEAREST:
		case TKRA_GL_NEAREST_MIPMAP_NEAREST:
		case TKRA_GL_NEAREST_MIPMAP_LINEAR:
			txfl&=~TKRA_TRFL_MAGBL;
			break;
		case TKRA_GL_LINEAR:
		case TKRA_GL_LINEAR_MIPMAP_NEAREST:
		case TKRA_GL_LINEAR_MIPMAP_LINEAR:
			txfl|=TKRA_TRFL_MAGBL;
			break;
		}
		break;
	case TKRA_GL_TEXTURE_WRAP_S:
		switch(param)
		{
		case TKRA_GL_REPEAT:
			txfl&=~TKRA_TRFL_CLAMPS;
			break;
		case TKRA_GL_CLAMP:
		case TKRA_GL_CLAMP_TO_EDGE:
			txfl|=TKRA_TRFL_CLAMPS;
			break;
		}
		break;
	case TKRA_GL_TEXTURE_WRAP_T:
		switch(param)
		{
		case TKRA_GL_REPEAT:
			txfl&=~TKRA_TRFL_CLAMPT;
			break;
		case TKRA_GL_CLAMP:
		case TKRA_GL_CLAMP_TO_EDGE:
			txfl|=TKRA_TRFL_CLAMPT;
			break;
		}
		break;
	}

	img->tex_flag=txfl;
	TKRA_BindTexImg(ctx, img);
}

void tkra_glTexParameterfv(int target, int pname, const float * params)
{
}

void tkra_glTexParameteriv(int target, int pname, const int * params)
{
}

void tkra_glTexEnvi(int target, int pname, int param)
{
	TKRA_Context *ctx;
	TKRA_TexImage *img;
	int i, ix;

	ctx=TKRA_GetCurrentContext();

	if(target!=GL_TEXTURE_ENV)
		return;

	if(pname==GL_TEXTURE_ENV_MODE)
	{
		if(param==GL_MODULATE)
		{
			ctx->stateflag1&=~TKRA_STFL1_TEX_REPLACE;
		}

		if(param==GL_REPLACE)
		{
			ctx->stateflag1|=TKRA_STFL1_TEX_REPLACE;
		}
	}
}

void tkra_glTexEnvf(int target, int pname, float param)
{
	TKRA_Context *ctx;
	TKRA_TexImage *img;
	int i, ix;

	ctx=TKRA_GetCurrentContext();

	if(target!=GL_TEXTURE_ENV)
		return;

	if(pname==GL_TEXTURE_ENV_MODE)
	{
		tkra_glTexEnvi(target, pname, param);
		return;
	}
}

void tkra_glShadeModel(int mode)
{
}

void tkra_glDeleteTextures(int cnt, int *list)
{
	TKRA_Context *ctx;
	TKRA_TexImage *img;
	u16 px;
	int i, ix;

//	if(target!=TKRA_TEXTURE_2D)
//		return;

	ctx=TKRA_GetCurrentContext();

	for(i=0; i<cnt; i++)
	{
		ix=list[i];
		img=TKRA_GetTexImg(ctx, ix);
		TKRA_BindTexImg(ctx, img);

		px=0x7FFF;
//		TKRA_UpdateTexImg(img, NULL, 0, 0, 0, 0);
		TKRA_UpdateTexImg(img, &px, 1, 1, 0, 0);
		
		if(img==ctx->tex_cur)
		{
			TKRA_BindTexImg(ctx, img);
		}
	}

//	img=ctx->tex_cur;
//	TKRA_UpdateTexImg(img, txbuf, width, height, level);
}

#if 0
void glTexParameterIiv(int target, int pname, const int * params);
void glTexParameterIuiv(int target, int pname, const unsigned int * params);

void glTextureParameterf(int texture, int pname, float param);
void glTextureParameteri(int texture, int pname, int param);
void glTextureParameterfv(int texture, int pname, const float *params);
void glTextureParameteriv(int texture, int pname, const int *params);
void glTextureParameterIiv(int texture, int pname, const int *params);
void glTextureParameterIuiv(int texture, int pname, const unsigned int *params);
#endif


void tkra_glActiveTexture(int texture)
{
	TKRA_Context *ctx;
	int ix;

	ctx=TKRA_GetCurrentContext();

	ix=texture-GL_TEXTURE0;
	if((ix<0) || (ix>=8))
	{
		ix=texture-0x835E;
		if((ix<0) || (ix>=8))
			return;
	}
	
	ctx->tex2d_active=ix&7;

//	ctx->blend_sfunc=ctx->blend_sfunc_mtx[ix];
//	ctx->blend_dfunc=ctx->blend_dfunc_mtx[ix];
//	TKRA_BindTexImg(ctx, ctx->tex_cur_mtx[ix]);
}
