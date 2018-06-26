#include <bgbrasw.h>

char *RASWGL_GetString(RASWGL_Context *ctx, int name)
{
	char *str;
	switch(name)
	{
	case GL_VENDOR:			str="BGBTech"; break;
	case GL_RENDERER:		str="BGBRASW"; break;
	case GL_VERSION:		str="1.2"; break;
	case GL_EXTENSIONS:		str=""; break;
	default:				str=NULL; break;
	}
	
	return(str);
//	return(NULL);
}

void RASWGL_TexEnvf(RASWGL_Context *ctx, 
		int target, int pname, float param)
{
//	RaGlSetErrorNopStub();
}

void RASWGL_TexEnvi(RASWGL_Context *ctx, 
		int target, int pname, int param)
{
//	RaGlSetErrorNopStub();
}

void RASWGL_TexEnvfv(RASWGL_Context *ctx, 
		int target, int pname, const float *params)
	{ RaGlSetErrorNopStub(); }

void RASWGL_TexEnviv(RASWGL_Context *ctx, 
		int target, int pname, const int *params)
	{ RaGlSetErrorNopStub(); }

void RASWGL_GetTexEnvfv(RASWGL_Context *ctx, 
		int target, int pname, float *params)
	{ RaGlSetErrorNopStub(); }

void RASWGL_GetTexEnviv(RASWGL_Context *ctx, 
		int target, int pname, int *params)
	{ RaGlSetErrorNopStub(); }

void RASWGL_TexParameterf(RASWGL_Context *ctx, 
		int target, int pname, float param)
{
	BGBRASW_Texture *tex;

	if(target!=GL_TEXTURE_2D)
		{ ctx->glerror=GL_INVALID_ENUM; return; }

	tex=BGBRASW_GetTexture(ctx->ractx, ctx->texid);

	switch(pname)
	{
	case GL_TEXTURE_MIN_FILTER:
		tex->tex_min_filter=param;
		break;
	case GL_TEXTURE_MAG_FILTER:
		tex->tex_mag_filter=param;
		break;
	default:
		ctx->glerror=GL_INVALID_ENUM;
		break;
	}
}

void RASWGL_TexParameteri(RASWGL_Context *ctx, 
		int target, int pname, int param)
{
	BGBRASW_Texture *tex;

	if(target!=GL_TEXTURE_2D)
		{ ctx->glerror=GL_INVALID_ENUM; return; }

	tex=BGBRASW_GetTexture(ctx->ractx, ctx->texid);

	switch(pname)
	{
	case GL_TEXTURE_MIN_FILTER:
		tex->tex_min_filter=param;
		break;
	case GL_TEXTURE_MAG_FILTER:
		tex->tex_mag_filter=param;
		break;
	default:
		ctx->glerror=GL_INVALID_ENUM;
		break;
	}
//	RaGlSetErrorNopStub();
}

void RASWGL_TexParameterfv(RASWGL_Context *ctx, 
		int target, int pname, const float *params)
{
	switch(pname)
	{
	case GL_TEXTURE_BORDER_COLOR:
		break;
	default:
		ctx->glerror=GL_INVALID_ENUM;
		break;
	}
//	RaGlSetErrorNopStub();
}

void RASWGL_TexParameteriv(RASWGL_Context *ctx, 
		int target, int pname, const int *params)
	{ RaGlSetErrorNopStub(); }

void RASWGL_GetTexParameterfv(RASWGL_Context *ctx, 
		int target, int pname, float *params)
	{ RaGlSetErrorNopStub(); }
	
void RASWGL_GetTexParameteriv(RASWGL_Context *ctx, 
		int target, int pname, int *params)
	{ RaGlSetErrorNopStub(); }

void RASWGL_GetTexLevelParameterfv(RASWGL_Context *ctx, 
		int target, int level, int pname, float *params)
	{ RaGlSetErrorNopStub(); }

void RASWGL_GetTexLevelParameteriv(RASWGL_Context *ctx, 
		int target, int level, int pname, int *params)
	{ RaGlSetErrorNopStub(); }

void RASWGL_TexImage1D(RASWGL_Context *ctx, 
		int target, int level, int internalFormat,
		int width, int border, int format, int type,
		const void *pixels)
{
	RASWGL_TexImage2D(ctx,
		target, level, internalFormat,
		width, 1, border, format, type, pixels);
}

int RASWGL_TexImage_ConvPixelsFromSrc(
	int width, int height,
	int format, int type,
	const void *pixels, bgbrasw_pixel *opix)
{
	byte *bpix;
	int cr, cg, cb, ca;
	int i, j, k, n, rt;

	n=width*height;

	if(!pixels)
	{
		memset(opix, 0, n*sizeof(bgbrasw_pixel));
		return(0);
	}

	rt=1;
	switch(format)
	{
	case GL_RGB:
		switch(type)
		{
		case GL_UNSIGNED_BYTE:
			bpix=(byte *)pixels;
			for(i=0; i<n; i++)
			{
				cr=bpix[i*3+0];	cg=bpix[i*3+1];
				cb=bpix[i*3+2];	ca=255;
				opix[i]=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
			}
			break;
		}
		break;
	case GL_RGBA:
		switch(type)
		{
		case GL_UNSIGNED_BYTE:
			bpix=(byte *)pixels;
			for(i=0; i<n; i++)
			{
				cr=bpix[i*4+0];	cg=bpix[i*4+1];
				cb=bpix[i*4+2];	ca=bpix[i*4+3];
				if(ca<240)
					rt&=~1;
				opix[i]=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
			}
			break;
		}
		break;
	case GL_BGR:
		switch(type)
		{
		case GL_UNSIGNED_BYTE:
			bpix=(byte *)pixels;
			for(i=0; i<n; i++)
			{
				cr=bpix[i*3+2];	cg=bpix[i*3+1];
				cb=bpix[i*3+0];	ca=255;
				opix[i]=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
			}
			break;
		}
		break;
	case GL_BGRA:
		switch(type)
		{
		case GL_UNSIGNED_BYTE:
			bpix=(byte *)pixels;
			for(i=0; i<n; i++)
			{
				cr=bpix[i*4+2];	cg=bpix[i*4+1];
				cb=bpix[i*4+0];	ca=bpix[i*4+3];
				if(ca<240)
					rt&=~1;
				opix[i]=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
			}
			break;
		}
		break;

	case GL_LUMINANCE:
		switch(type)
		{
		case GL_UNSIGNED_BYTE:
			bpix=(byte *)pixels;
			for(i=0; i<n; i++)
			{
				cg=bpix[i]; cr=cg; cb=cg; ca=255;
				opix[i]=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
			}
			break;
		}
		break;
	case GL_LUMINANCE_ALPHA:
		switch(type)
		{
		case GL_UNSIGNED_BYTE:
			bpix=(byte *)pixels;
			for(i=0; i<n; i++)
			{
				cg=bpix[i*2+0]; cr=cg; cb=cg; ca=bpix[i*2+1];
				if(ca<240)
					rt&=~1;
				opix[i]=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
			}
			break;
		}
		break;
	}
#ifdef BGBRASW_SS_YUVA
	for(i=0; i<n; i++)
	{
		opix[i]=bgbrasw_pixel2yypixel(opix[i]);
	}
#endif
	return(rt);
}

int RASWGL_TexImage_ConvSubImagePixelsFromSrc(
	int xoffs, int yoffs, int width, int height,
	int format, int type,
	const void *pixels, bgbrasw_pixel *opix,
	int tgtxs, int tgtys)
{
	byte *bpix;
	int cr, cg, cb, ca;
	int i, j, k, l, n, rt;

	if(!pixels)
		{ return(0); }

	rt=1;
	switch(format)
	{
	case GL_RGB:
		switch(type)
		{
		case GL_UNSIGNED_BYTE:
			bpix=(byte *)pixels;
			for(i=0; i<height; i++)
				for(j=0; j<width; j++)
			{
				k=i*width+j;
				l=(i+yoffs)*tgtxs+j;
				cr=bpix[k*3+0];	cg=bpix[k*3+1];
				cb=bpix[k*3+2];	ca=255;
				opix[l]=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
			}
			break;
		}
		break;
	case GL_RGBA:
		switch(type)
		{
		case GL_UNSIGNED_BYTE:
			bpix=(byte *)pixels;
			for(i=0; i<height; i++)
				for(j=0; j<width; j++)
			{
				k=i*width+j;
				l=(i+yoffs)*tgtxs+j;
				cr=bpix[k*4+0];	cg=bpix[k*4+1];
				cb=bpix[k*4+2];	ca=bpix[k*4+3];
				if(ca<240)
					rt&=~1;
				opix[l]=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
			}
			break;
		}
		break;
	case GL_BGR:
		switch(type)
		{
		case GL_UNSIGNED_BYTE:
			bpix=(byte *)pixels;
			for(i=0; i<height; i++)
				for(j=0; j<width; j++)
			{
				k=i*width+j;
				l=(i+yoffs)*tgtxs+j;
				cr=bpix[k*3+2];	cg=bpix[k*3+1];
				cb=bpix[k*3+0];	ca=255;
				opix[l]=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
			}
			break;
		}
		break;
	case GL_BGRA:
		switch(type)
		{
		case GL_UNSIGNED_BYTE:
			bpix=(byte *)pixels;
			for(i=0; i<height; i++)
				for(j=0; j<width; j++)
			{
				k=i*width+j;
				l=(i+yoffs)*tgtxs+j;
				cr=bpix[k*4+2];	cg=bpix[k*4+1];
				cb=bpix[k*4+0];	ca=bpix[k*4+3];
				if(ca<240)
					rt&=~1;
				opix[l]=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
			}
			break;
		}
		break;

	case GL_LUMINANCE:
		switch(type)
		{
		case GL_UNSIGNED_BYTE:
			bpix=(byte *)pixels;
			for(i=0; i<height; i++)
				for(j=0; j<width; j++)
			{
				k=i*width+j;
				l=(i+yoffs)*tgtxs+j;
				cg=bpix[k]; cr=cg; cb=cg; ca=255;
				opix[l]=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
			}
			break;
		}
		break;
	case GL_LUMINANCE_ALPHA:
		switch(type)
		{
		case GL_UNSIGNED_BYTE:
			bpix=(byte *)pixels;
			for(i=0; i<height; i++)
				for(j=0; j<width; j++)
			{
				k=i*width+j;
				l=(i+yoffs)*tgtxs+j;
				cg=bpix[k*2+0]; cr=cg; cb=cg; ca=bpix[k*2+1];
				if(ca<240)
					rt&=~1;
				opix[l]=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
			}
			break;
		}
		break;
	}
#ifdef BGBRASW_SS_YUVA
	for(i=0; i<height; i++)
		for(j=0; j<width; j++)
	{
		k=i*width+j;
		l=(i+yoffs)*tgtxs+j;
		opix[l]=bgbrasw_pixel2yypixel(opix[l]);
	}
#endif
	return(rt);
}

void RASWGL_TexImage2D(RASWGL_Context *ctx, 
		int target, int level, int internalFormat,
		int width, int height, int border, int format,
		int type, const void *pixels)
{
	BGBRASW_Texture *tex;
	bgbrasw_pixel *pixb;
	int i, j, h;
	
	if(target!=GL_TEXTURE_2D)
		{ ctx->glerror=GL_INVALID_ENUM; return; }
	if((level<0) || (level>=16))
		{ ctx->glerror=GL_INVALID_VALUE; return; }
	
	tex=BGBRASW_GetTexture(ctx->ractx, ctx->texid);
	BGBRASW_SetTextureSizeMip(ctx->ractx, tex,
		width, height, level);
	i=RASWGL_TexImage_ConvPixelsFromSrc(
		width, height, format, type,
		pixels, tex->rgba[level]);

	if(i&1)
		{ tex->flags|=BGBRASW_TEXFLAG_OPAQUE; }
	else
		{ tex->flags&=~BGBRASW_TEXFLAG_OPAQUE; }
	
	ctx->texture=tex;
//	BGBRASW_SetTexturePixelsSize(ctx->ractx, tex,
//		pixb, width, height, level);
}

void RASWGL_GetTexImage(RASWGL_Context *ctx, 
		int target, int level, int format, int type,
		void *pixels)
{
//	RaGlSetErrorNopStub();
}


void RASWGL_TexSubImage1D(RASWGL_Context *ctx, 
		int target, int level, int xoffset, int width,
		int format, int type, const void *pixels)
{
	RaGlSetErrorNopStub();
}

void RASWGL_TexSubImage2D(RASWGL_Context *ctx, 
		int target, int level, int xoffset, int yoffset,
		int width, int height, int format, int type,
		const void *pixels)
{
	BGBRASW_Texture *tex;
	bgbrasw_pixel *pixb;
	int i, j, h;
	
	if(target!=GL_TEXTURE_2D)
		{ ctx->glerror=GL_INVALID_ENUM; return; }
	if((level<0) || (level>=16))
		{ ctx->glerror=GL_INVALID_VALUE; return; }
	
	tex=BGBRASW_LookupTexture(ctx->ractx, ctx->texid);

	i=RASWGL_TexImage_ConvSubImagePixelsFromSrc(
		xoffset, yoffset, width, height, format, type,
		pixels, tex->rgba[level], tex->xs, tex->ys);

	if(!(i&1))
		{ tex->flags&=~BGBRASW_TEXFLAG_OPAQUE; }
	
	ctx->texture=tex;
}

void RASWGL_CopyTexImage1D(RASWGL_Context *ctx, 
		int target, int level, int internalformat,
		int x, int y, int width, int border)
	{ RaGlSetErrorNopStub(); }
void RASWGL_CopyTexImage2D(RASWGL_Context *ctx, 
		int target, int level, int internalformat,
		int x, int y, int width, int height, int border)
	{ RaGlSetErrorNopStub(); }
void RASWGL_CopyTexSubImage1D(RASWGL_Context *ctx, 
		int target, int level, int xoffset, int x, int y,
		int width)
	{ RaGlSetErrorNopStub(); }
void RASWGL_CopyTexSubImage2D(RASWGL_Context *ctx, 
		int target, int level, int xoffset, int yoffset,
		int x, int y, int width, int height)
	{ RaGlSetErrorNopStub(); }

void RASWGL_CompressedTexImage2D(RASWGL_Context *ctx, 
		int target, int level, int internalFormat,
		int width, int height, int border, int imgSize,
		const void *pixels)
{
	BGBRASW_Texture *tex;
	bgbrasw_pixel *pixb;
	int i, j, h;

	RaGlSetErrorNopStub();
	
	if(target!=GL_TEXTURE_2D)
		{ ctx->glerror=GL_INVALID_ENUM; return; }
	if((level<0) || (level>=16))
		{ ctx->glerror=GL_INVALID_VALUE; return; }
	
	tex=BGBRASW_GetTexture(ctx->ractx, ctx->texid);
	BGBRASW_SetTextureSizeMip(ctx->ractx, tex,
		width, height, level);

//	i=RASWGL_TexImage_ConvPixelsFromSrc(
//		width, height, format, type,
//		pixels, tex->rgba[level]);
	i=1;

	if(i&1)
		{ tex->flags|=BGBRASW_TEXFLAG_OPAQUE; }
	else
		{ tex->flags&=~BGBRASW_TEXFLAG_OPAQUE; }
	
	ctx->texture=tex;
//	BGBRASW_SetTexturePixelsSize(ctx->ractx, tex,
//		pixb, width, height, level);
}

void RASWGL_ReadPixels(
	RASWGL_Context *ctx, 
	int x, int y, int width, int height,
	int format, int type, byte *pixels)
{
	bgbrasw_pixel pix;
	bgbrasw_pixel *fb;
	byte *ct;
	int cx, cy, fbxs, fbys, xstr, ystr, pxf;
	int i, j, k, l;
	
	xstr=4; pxf=0;
	if((format==GL_BGRA) || (format==GL_BGR))
		pxf|=1;
	if((format==GL_RGB) || (format==GL_BGR))
		xstr=3;

	ystr=xstr*width;

	fb=ctx->ractx->tgt_rgba;
	fbxs=ctx->ractx->tgt_xs;
	fbys=ctx->ractx->tgt_ys;
	
	for(i=0; i<height; i++)
		for(j=0; j<width; j++)
	{
		cx=x+j; cy=y+i;
		if((cx<0) || (cx>=fbxs))
			continue;
		if((cy<0) || (cy>=fbys))
			continue;
		
		pix=fb[cy*fbxs+cx];
		ct=pixels+i*ystr+j*xstr;
		if(pxf&1)
			{ ct[0]=pix; ct[1]=pix>>8; ct[2]=pix>>16; }
		else
			{ ct[2]=pix; ct[1]=pix>>8; ct[0]=pix>>16; }
		if(xstr==4)
			ct[3]=pix>>24;
	}
}
