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
	static tkra_rastpixel *txbuf;
	static int sz_txbuf;

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
			k=width*height;
			for(i=0; i<k; i++)
			{
				txbuf[i]=tkra_teximg_packrgb555(cs[0], cs[1], cs[2], cs[3]);
				cs+=4;
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
			k=width*height;
			for(i=0; i<k; i++)
			{
				txbuf[i]=tkra_teximg_packrgb555(cs[2], cs[1], cs[0], cs[3]);
				cs+=4;
			}
		}
	}else
	{
		__debugbreak();
	}

	ctx=TKRA_GetCurrentContext();
	img=ctx->tex_cur;

	TKRA_UpdateTexImg(img, txbuf, width, height, level);
	TKRA_BindTexImg(ctx, img);
}

void tkra_glTexSubImage2D(
	int target, int level,
	int xoffset, int yoffset,
	int width, int height,
	int format, int type,
	void *pixels )
{
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

	TKRA_UpdateTexImgDxt1(img, data, width, height, level);
	TKRA_BindTexImg(ctx, img);
}

void tkra_glBindTexture(int target, int texture)
{
	TKRA_Context *ctx;
	TKRA_TexImage *img;

	if(target!=TKRA_TEXTURE_2D)
		return;

	ctx=TKRA_GetCurrentContext();

	img=TKRA_GetTexImg(ctx, texture);
	TKRA_BindTexImg(ctx, img);

//	img=ctx->tex_cur;
//	TKRA_UpdateTexImg(img, txbuf, width, height, level);
}

void tkra_glTexParameterf(int target, int pname, float param)
{
}

void tkra_glTexParameteri(int target, int pname, int param)
{
}

void tkra_glTexParameterfv(int target, int pname, const float * params)
{
}

void tkra_glTexParameteriv(int target, int pname, const int * params)
{
}

void tkra_glTexEnvf(int target, int pname, float param)
{
}

void tkra_glShadeModel(int mode)
{
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
