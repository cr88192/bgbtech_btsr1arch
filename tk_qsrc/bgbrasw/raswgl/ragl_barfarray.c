#include <bgbrasw.h>

RASWGL_BARF_RenderArray *RASWGL_BARF_AllocRenderArray()
{
	RASWGL_BARF_RenderArray *tmp;
	
	tmp=bgbrasw_tmalloc("raswgl_barf_renderarray_t",
		sizeof(RASWGL_BARF_RenderArray));
	return(tmp);
}

RASWGL_BARF_RenderContext *RASWGL_BARF_AllocRenderContext()
{
	RASWGL_BARF_RenderContext *tmp;
	
	tmp=bgbrasw_tmalloc("raswgl_barf_rendercontext_t",
		sizeof(RASWGL_BARF_RenderContext));
	return(tmp);
}

void RASWGL_BARF_CheckExpandContextArrays(RASWGL_BARF_RenderContext *ctx)
{
	int i, j, k;

	if(!ctx->array)
	{
		i=256;
		ctx->array=bgbrasw_malloc(i*sizeof(RASWGL_BARF_RenderArray *));
		ctx->n_array=0;
		ctx->m_array=i;
	}
	
	if((ctx->n_array+1)>=ctx->m_array)
	{
		i=ctx->m_array;
		i=i+(i>>1);
		ctx->array=bgbrasw_realloc(ctx->array,
			i*sizeof(RASWGL_BARF_RenderArray *));
		ctx->m_array=i;
	}
}

RASWGL_BARF_RenderArray *RASWGL_BARF_GetRenderArrayForTexture(
	RASWGL_BARF_RenderContext *ctx, int texnum)
{
	int i, j, k;
	
	for(i=0; i<ctx->n_array; i++)
		if(ctx->array[i]->tex==texnum)
			return(ctx->array[i]);

	RASWGL_BARF_CheckExpandContextArrays(ctx);

	i=ctx->n_array++;
	ctx->array[i]=RASWGL_BARF_AllocRenderArray();
	ctx->array[i]->tex=texnum;
//	ctx->array[i]->texflags=RASWGL_Texture_GetImageFlags(texnum);
//	ctx->array[i]->texflags2=RASWGL_Texture_GetImageFlags2(texnum);
	return(ctx->array[i]);
}

void RASWGL_BARF_ResetContextArrays(
	RASWGL_BARF_RenderContext *ctx)
{
	int i, j, k;
	
	for(i=0; i<ctx->n_array; i++)
	{
//		if(ctx->array[i]->tex==texnum)
//			return(ctx->array[i]);
		ctx->array[i]->n_tris=0;
	}
}

void RASWGL_BARF_CheckExpandArrayTriangles(
	RASWGL_BARF_RenderArray *arr, int ntris)
{
	int i, j, k;
	
	if(!arr->xyz)
	{
//		i=256;
		i=4096;
		while(i<=ntris)
			i=i+(i>>1);
		arr->xyz=bgbrasw_malloc(3*i*3*sizeof(raswgl_barf_xyzfloat));
		arr->st=bgbrasw_malloc(3*i*2*sizeof(raswgl_barf_stfloat));
//		arr->norm=bgbrasw_malloc(3*i*3*sizeof(raswgl_barf_nvfloat));
//		arr->xnorm=bgbrasw_malloc(3*i*3*sizeof(raswgl_barf_nvfloat));
//		arr->ynorm=bgbrasw_malloc(3*i*3*sizeof(raswgl_barf_nvfloat));
		arr->norm=bgbrasw_malloc(3*i*sizeof(raswgl_barf_nvfloat));
//		arr->xnorm=bgbrasw_malloc(3*i*sizeof(raswgl_barf_nvfloat));
//		arr->ynorm=bgbrasw_malloc(3*i*sizeof(raswgl_barf_nvfloat));
		arr->rgba=bgbrasw_malloc(3*i*4*sizeof(raswgl_barf_clrfloat));
//		arr->srgba=bgbrasw_malloc(3*i*4*sizeof(raswgl_barf_clrfloat));
//		arr->vrgba=bgbrasw_malloc(3*i*4*sizeof(raswgl_barf_clrfloat));
//		arr->trgba=bgbrasw_malloc(3*i*4*sizeof(raswgl_barf_clrfloat));
		arr->n_tris=0;
		arr->m_tris=i;
	}
	
	if((arr->n_tris+ntris+256)>=arr->m_tris)
	{
		i=arr->m_tris;
		j=arr->n_tris+ntris+256;
		while(i<=j)
			i=i+(i>>1);
		arr->xyz=bgbrasw_realloc(arr->xyz,
			3*i*3*sizeof(raswgl_barf_xyzfloat));
		arr->st=bgbrasw_realloc(arr->st,
			3*i*2*sizeof(raswgl_barf_stfloat));
		arr->norm=bgbrasw_realloc(arr->norm,
			3*i*sizeof(raswgl_barf_nvfloat));
//		arr->xnorm=bgbrasw_realloc(arr->xnorm,
//			3*i*sizeof(raswgl_barf_nvfloat));
//		arr->ynorm=bgbrasw_realloc(arr->ynorm,
//			3*i*sizeof(raswgl_barf_nvfloat));
		arr->rgba=bgbrasw_realloc(arr->rgba,
			3*i*4*sizeof(raswgl_barf_clrfloat));
//		arr->srgba=bgbrasw_realloc(arr->srgba,
//			3*i*4*sizeof(raswgl_barf_clrfloat));
//		arr->vrgba=bgbrasw_realloc(arr->vrgba,
//			3*i*4*sizeof(raswgl_barf_clrfloat));
//		arr->trgba=bgbrasw_realloc(arr->trgba,
//			3*i*4*sizeof(raswgl_barf_clrfloat));
		arr->m_tris=i;
	}
}

#if 0
void RASWGL_BARF_UpdateArrayVBO(
	RASWGL_BARF_RenderArray *arr)
{
	byte *buf, *ct;
	int i, n, id, sz;

	n=arr->n_tris*3;
	if(n<=0)return;

	sz= n*3*sizeof(float) +	//XYZ
		n*2*sizeof(float) +	//ST
		n*4 +	n*4 +	n*4 +	//Norm, xNorm, yNorm
		n*4 +	n*4 +	n*4;	//RGBA, sRGBA, vRGBA

	i=256;
	while(i<sz)
		i=i+(i>>1);
	sz=i;

	if(arr->vbo<=0)
	{
		pdglGenBuffers(1, &(arr->vbo));
		arr->sz_vbo=0;
	}

//	if(arr->sz_vbo>sz)
//		sz=arr->sz_vbo;

	pdglBindBuffer(GL_ARRAY_BUFFER, arr->vbo);
	
	if(sz>arr->sz_vbo)
	{
		pdglBufferData(GL_ARRAY_BUFFER, sz, NULL, GL_DYNAMIC_DRAW);
		arr->sz_vbo=sz;
	}
//	chk->mesh->vbo_aface=id;

	buf=pdglMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	ct=buf;

	arr->vbo_xyz=ct-buf;
	memcpy(ct, arr->xyz, n*3*sizeof(float));
	ct+=n*3*sizeof(float);

	arr->vbo_st=ct-buf;
	memcpy(ct, arr->st, n*2*sizeof(float));
	ct+=n*2*sizeof(float);

	arr->vbo_norm=ct-buf;
	memcpy(ct, arr->norm, n*4);
	ct+=n*4;

	arr->vbo_xnorm=ct-buf;
	memcpy(ct, arr->xnorm, n*4);
	ct+=n*4;

	arr->vbo_ynorm=ct-buf;
	memcpy(ct, arr->ynorm, n*4);
	ct+=n*4;

	arr->vbo_rgba=ct-buf;
	memcpy(ct, arr->rgba, n*4);
	ct+=n*4;

	arr->vbo_srgba=ct-buf;
	memcpy(ct, arr->srgba, n*4);
	ct+=n*4;

	arr->vbo_vrgba=ct-buf;
	memcpy(ct, arr->vrgba, n*4);
	ct+=n*4;

	pdglUnmapBuffer(GL_ARRAY_BUFFER);
//	free(buf);
	pdglBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RASWGL_BARF_UpdateContextArrayVBOs(
	RASWGL_BARF_RenderContext *ctx)
{
	int tsz, msz, ttr, mtr;
	int i, j, k;
	
	tsz=0; msz=0; ttr=0; mtr=0;
	for(i=0; i<ctx->n_array; i++)
	{
		RASWGL_BARF_UpdateArrayVBO(ctx->array[i]);

		ttr+=ctx->array[i]->n_tris;
		tsz+=ctx->array[i]->sz_vbo;
		if(ctx->array[i]->n_tris>mtr)
			mtr=ctx->array[i]->n_tris;
		if(ctx->array[i]->sz_vbo>msz)
			msz=ctx->array[i]->sz_vbo;
	}
	
	if(btCvarGeti("vox_dumpstats")>0)
	{
		printf("BARF: %d arrays, %d tris, avg: %d tris/array, "
				"max: %d tris/array\n",
			ctx->n_array, ttr, ttr/ctx->n_array, mtr);
		printf("\t total: %d bytes, avg: %d bytes/array, "
				"max: %d bytes/array\n",
			tsz, tsz/ctx->n_array, msz);
	}
}
#endif

void raswgl_barf_copyArrayXYZ(raswgl_barf_xyzfloat *dst, int nvec,
	int vsize, int vtype, int vstep, void *src)
{
	raswgl_barf_xyzfloat *ct;
	byte *csb;
	float *csf;
	int i, j, k;
	
	if(vtype==GL_FLOAT)
	{
		csb=src; ct=dst;
		for(i=0; i<nvec; i++)
		{
			csf=(float *)csb;
			RASWGL_Shader_TransformPoint(csf, ct);
			ct+=3; csb+=vstep;
		}
		return;
	}
}

void raswgl_barf_copyArrayST(raswgl_barf_stfloat *dst, int nvec,
	int vsize, int vtype, int vstep, void *src)
{
	raswgl_barf_stfloat *ct;
	byte *csb;
	float *csf;
	int i, j, k;
	
	if(!src)
	{
		ct=dst;
		for(i=0; i<nvec; i++)
			{ ct[0]=0; ct[1]=0; ct+=2; }
		return;
	}
	
	if(vtype==GL_FLOAT)
	{
		csb=src; ct=dst;
		for(i=0; i<nvec; i++)
		{
			csf=(float *)csb;
			ct[0]=csf[0];
			ct[1]=csf[1];
			ct+=2; csb+=vstep;
		}
		return;
	}
}

u32 raswgl_barf_Float3ToNvFloat(float x, float y, float z)
{
	int i0, i1, i2, i3;
	
	i0=x*511;
	i1=y*511;
	i2=z*511;
	i0=(i0>=(-512))?((i0<=511)?i0:511):(-512);
	i1=(i1>=(-512))?((i1<=511)?i1:511):(-512);
	i2=(i2>=(-512))?((i2<=511)?i2:511):(-512);
	i3=(i0&1023)|((i1&1023)<<10)|((i2&1023)<<20);
	return(i3);
}

u32 raswgl_barf_UByte3ToNvFloat(int x, int y, int z)
{
	int i0, i1, i2, i3;
	
	i0=(x<<1)|(x>>7);
	i1=(y<<1)|(y>>7);
	i2=(z<<1)|(z>>7);
	i3=(i0&1023)|((i1&1023)<<10)|((i2&1023)<<20);
	return(i3);
}

u32 raswgl_barf_SByte3ToNvFloat(int x, int y, int z)
{
	int i0, i1, i2, i3;
	i0=(x<<2)|(x>>6);
	i1=(y<<2)|(y>>6);
	i2=(z<<2)|(z>>6);
	i3=(i0&1023)|((i1&1023)<<10)|((i2&1023)<<20);
	return(i3);
}

void raswgl_barf_copyArrayNorm(raswgl_barf_nvfloat *dst, int nvec,
	int vsize, int vtype, int vstep, void *src)
{
	raswgl_barf_nvfloat *ct;
	byte *csb;
	float *csf;
	signed char *cssb;
	u16 *cshf;
	int i0, i1, i2, i3;
	int i, j, k;
	
	if(!src)
	{
		ct=dst;
		for(i=0; i<nvec; i++)
			{ *ct++=0; }
//		for(i=0; i<nvec; i++)
//			{ ct[0]=0; ct[1]=0; ct[2]=0; ct+=3; }
		return;
	}
	
	if(vtype==GL_FLOAT)
	{
		csb=src; ct=dst;
		for(i=0; i<nvec; i++)
		{
			csf=(float *)csb;
//			ct[0]=csf[0];
//			ct[1]=csf[1];
//			ct[2]=csf[2];
//			ct[0]=BGBBTJ_EncodeFloat16(csf[0]);
//			ct[1]=BGBBTJ_EncodeFloat16(csf[1]);
//			ct[2]=BGBBTJ_EncodeFloat16(csf[2]);
//			ct+=3; csb+=vstep;

			ct[0]=raswgl_barf_Float3ToNvFloat(csf[0], csf[1], csf[2]);
			ct++; csb+=vstep;
		}
		return;
	}

	if(vtype==GL_BYTE)
	{
		cssb=src; ct=dst;
		for(i=0; i<nvec; i++)
		{
			i0=cssb[0]; i1=cssb[1]; i2=cssb[2];
			i0=(i0<<2)|(i0>>6);
			i1=(i1<<2)|(i1>>6);
			i2=(i2<<2)|(i2>>6);
			ct[0]=(i0&1023)|((i1&1023)<<10)|((i2&1023)<<20);

//			ct[0]=raswgl_barf_SByte3ToNvFloat(cssb[0], cssb[1], cssb[2]);
			ct++; cssb+=vstep;
		}
		return;
	}

	if(vtype==GL_HALF_FLOAT)
	{
		csb=src; ct=dst;
		for(i=0; i<nvec; i++)
		{
			cshf=(u16 *)csb;
			ct[0]=raswgl_barf_Float3ToNvFloat(
				BGBBTJ_DecodeFloat16(cshf[0]),
				BGBBTJ_DecodeFloat16(cshf[1]),
				BGBBTJ_DecodeFloat16(cshf[2]));
			ct++; csb+=vstep;
		}
		return;
	}

	if(vtype==GL_UNSIGNED_BYTE)
	{
		csb=src; ct=dst;
		for(i=0; i<nvec; i++)
		{
			ct[0]=raswgl_barf_UByte3ToNvFloat(csb[0], csb[1], csb[2]);
			ct++; csb+=vstep;
		}
		return;
	}
}

raswgl_barf_clrfloat raswgl_barf_float2clrfloat(float val)
{
	int i;
	i=val*255.0;
	i=(i>=0)?((i<=255)?i:255):0;
	return(i);
}

raswgl_barf_clrfloat raswgl_barf_halffloat2clrfloat(u16 val)
{
	int i;
	i=BGBBTJ_DecodeFloat16(val)*255.0;
	i=(i>=0)?((i<=255)?i:255):0;
	return(i);
}

raswgl_barf_clrfloat raswgl_barf_ubyte2clrfloat(int val)
{
	return(val);
}

raswgl_barf_clrfloat raswgl_barf_sbyte2clrfloat(int val)
{
	int i;
	i=val*2;
	i=(i>=0)?((i<=255)?i:255):0;
	return(i);
}

void raswgl_barf_copyArrayRGBA(raswgl_barf_clrfloat *dst, int nvec,
	int vsize, int vtype, int vstep, void *src,
	int vsize2, int vtype2, int vstep2, void *src2)
{
	raswgl_barf_clrfloat *ct;
	int *csi, *cti, *csie;
	byte *csb;
	signed char *cssb;
	float *csf;
	u16 *cshf;
	int i, j, k;
	
	if(!src)
	{
		if(!src2)
		{
			ct=dst;
//			j=BGBBTJ_EncodeFloat16(1.0);
			j=255;
			for(i=0; i<nvec; i++)
				{ ct[0]=j; ct[1]=j; ct[2]=j; ct[3]=j; ct+=4; }
			return;
		}
		
		vsize=vsize2;
		vtype=vtype2;
		vstep=vstep2;
		src=src2;
	}
	
	if(vtype==GL_FLOAT)
	{
		csb=src; ct=dst;
		for(i=0; i<nvec; i++)
		{
			csf=(float *)csb;
			ct[0]=raswgl_barf_float2clrfloat(csf[0]);
			ct[1]=raswgl_barf_float2clrfloat(csf[1]);
			ct[2]=raswgl_barf_float2clrfloat(csf[2]);
			ct[3]=raswgl_barf_float2clrfloat(csf[3]);
			ct+=4; csb+=vstep;
		}
		return;
	}

	if(vtype==GL_UNSIGNED_BYTE)
	{
#if defined(X86) || defined(X86_64)
		if(vstep==4)
		{
			csi=(int *)src; cti=(int *)dst;
			csie=csi+nvec;
			while(csi<csie)
				{ *cti++=*csi++; }
			return;
		}
#endif

		csb=src; ct=dst;
		for(i=0; i<nvec; i++)
		{
//			*(int *)ct=*(int *)csb;
			ct[0]=raswgl_barf_ubyte2clrfloat(csb[0]);
			ct[1]=raswgl_barf_ubyte2clrfloat(csb[1]);
			ct[2]=raswgl_barf_ubyte2clrfloat(csb[2]);
			ct[3]=raswgl_barf_ubyte2clrfloat(csb[3]);
			ct+=4; csb+=vstep;
		}
		return;
	}

	if(vtype==GL_BYTE)
	{
		cssb=src; ct=dst;
		for(i=0; i<nvec; i++)
		{
			ct[0]=raswgl_barf_sbyte2clrfloat(cssb[0]);
			ct[1]=raswgl_barf_sbyte2clrfloat(cssb[1]);
			ct[2]=raswgl_barf_sbyte2clrfloat(cssb[2]);
			ct[3]=raswgl_barf_sbyte2clrfloat(cssb[3]);
			ct+=4; cssb+=vstep;
		}
		return;
	}

	if(vtype==GL_HALF_FLOAT)
	{
		csb=src; ct=dst;
		for(i=0; i<nvec; i++)
		{
			cshf=(u16 *)csb;
			ct[0]=raswgl_barf_halffloat2clrfloat(cshf[0]);
			ct[1]=raswgl_barf_halffloat2clrfloat(cshf[1]);
			ct[2]=raswgl_barf_halffloat2clrfloat(cshf[2]);
			ct[3]=raswgl_barf_halffloat2clrfloat(cshf[3]);
			ct+=4; csb+=vstep;
		}
		return;
	}
}

void RASWGL_BARF_ArraysEmitTriangles(
	RASWGL_BARF_RenderArray *arr, int ntris,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int stsize, int sttype, int ststep, void *st, 
	int normsize, int normtype, int normstep, void *norm,
	int xnormsize, int xnormtype, int xnormstep, void *xnorm,
	int ynormsize, int ynormtype, int ynormstep, void *ynorm,
	int rgbsize, int rgbtype, int rgbstep, void *rgb,
	int srgbsize, int srgbtype, int srgbstep, void *srgb,
	int vrgbsize, int vrgbtype, int vrgbstep, void *vrgb,
	int trgbsize, int trgbtype, int trgbstep, void *trgb)
{
	BGBBTJ_Float16_Init();
	RASWGL_BARF_CheckExpandArrayTriangles(arr, ntris);
	
	raswgl_barf_copyArrayXYZ(arr->xyz+(arr->n_tris*3*3), ntris*3,
		xyzsize, xyztype, xyzstep, xyz);
	raswgl_barf_copyArrayST(arr->st+(arr->n_tris*3*2), ntris*3,
		stsize, sttype, ststep, st);

	raswgl_barf_copyArrayNorm(arr->norm+(arr->n_tris*3), ntris*3,
		normsize, normtype, normstep, norm);
	raswgl_barf_copyArrayNorm(arr->xnorm+(arr->n_tris*3), ntris*3,
		xnormsize, xnormtype, xnormstep, xnorm);
	raswgl_barf_copyArrayNorm(arr->ynorm+(arr->n_tris*3), ntris*3,
		ynormsize, ynormtype, ynormstep, ynorm);

	raswgl_barf_copyArrayRGBA(arr->rgba+(arr->n_tris*3*4), ntris*3,
		rgbsize, rgbtype, rgbstep, rgb,
		0, 0, 0, NULL);
	raswgl_barf_copyArrayRGBA(arr->srgba+(arr->n_tris*3*4), ntris*3,
		srgbsize, srgbtype, srgbstep, srgb,
		rgbsize, rgbtype, rgbstep, rgb);
	raswgl_barf_copyArrayRGBA(arr->vrgba+(arr->n_tris*3*4), ntris*3,
		vrgbsize, vrgbtype, vrgbstep, vrgb,
		rgbsize, rgbtype, rgbstep, rgb);
	raswgl_barf_copyArrayRGBA(arr->trgba+(arr->n_tris*3*4), ntris*3,
		trgbsize, trgbtype, trgbstep, trgb,
		rgbsize, rgbtype, rgbstep, rgb);
	
	arr->n_tris+=ntris;
}

void *raswgl_barf_arrayIndexOffset(int base,
	int vsize, int vtype, int *rvstep, void *vec)
{
	int isz, vstep;
	if(!vec)return(vec);

	vstep=*rvstep;
	if(!vstep)
	{
		switch(vtype)
		{
		case GL_BYTE:				isz=sizeof(byte); break;
		case GL_UNSIGNED_BYTE:		isz=sizeof(byte); break;
		case GL_SHORT:				isz=sizeof(short); break;
		case GL_UNSIGNED_SHORT:		isz=sizeof(short); break;
		case GL_INT:				isz=sizeof(int); break;
		case GL_UNSIGNED_INT:		isz=sizeof(int); break;
		case GL_FLOAT:				isz=sizeof(float); break;
		case GL_DOUBLE:				isz=sizeof(double); break;
		case GL_HALF_FLOAT:			isz=sizeof(u16); break;
		default:					isz=sizeof(float); break;
		}
		vstep=vsize*isz;
		*rvstep=vstep;
	}
	vec=(void *)(((byte *)vec)+(base*vstep));
	return(vec);
}

void RASWGL_BARF_ArraysEmitVertices(
	RASWGL_BARF_RenderArray *arr, 
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int stsize, int sttype, int ststep, void *st, 
	int normsize, int normtype, int normstep, void *norm,
	int xnormsize, int xnormtype, int xnormstep, void *xnorm,
	int ynormsize, int ynormtype, int ynormstep, void *ynorm,
	int rgbsize, int rgbtype, int rgbstep, void *rgb,
	int srgbsize, int srgbtype, int srgbstep, void *srgb,
	int vrgbsize, int vrgbtype, int vrgbstep, void *vrgb,
	int trgbsize, int trgbtype, int trgbstep, void *trgb)
{
	if(base)
	{
		xyz=raswgl_barf_arrayIndexOffset(base,
			xyzsize, xyztype, &xyzstep, xyz);
		st=raswgl_barf_arrayIndexOffset(base,
			stsize, sttype, &ststep, st);
		norm=raswgl_barf_arrayIndexOffset(base,
			normsize, normtype, &normstep, norm);
		xnorm=raswgl_barf_arrayIndexOffset(base,
			xnormsize, xnormtype, &xnormstep, xnorm);
		ynorm=raswgl_barf_arrayIndexOffset(base,
			ynormsize, ynormtype, &ynormstep, ynorm);
		rgb=raswgl_barf_arrayIndexOffset(base,
			rgbsize, rgbtype, &rgbstep, rgb);
		srgb=raswgl_barf_arrayIndexOffset(base,
			srgbsize, srgbtype, &srgbstep, srgb);
		vrgb=raswgl_barf_arrayIndexOffset(base,
			vrgbsize, vrgbtype, &vrgbstep, vrgb);
		trgb=raswgl_barf_arrayIndexOffset(base,
			trgbsize, trgbtype, &trgbstep, trgb);
	}

	if(prim==GL_TRIANGLES)
	{
		RASWGL_BARF_ArraysEmitTriangles(arr, nxyz/3,
			xyzsize, xyztype, xyzstep, xyz,			//XYZ
			stsize, sttype, ststep, st,				//ST
			normsize, normtype, normstep, norm,		//Norm
			xnormsize, xnormtype, xnormstep, xnorm, //xNorm
			ynormsize, ynormtype, ynormstep, ynorm,	//yNorm
			rgbsize, rgbtype, rgbstep, rgb,			//RGBA
			srgbsize, srgbtype, srgbstep, srgb,		//sRGBA
			vrgbsize, vrgbtype, vrgbstep, vrgb,		//vRGBA
			trgbsize, trgbtype, trgbstep, trgb		//tRGBA
		);
		return;
	}
}

void RASWGL_BARF_DrawArraysSimpleFlat(
	RASWGL_BARF_RenderArray *arr, 
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz)
{
	RASWGL_BARF_ArraysEmitVertices(arr, prim, base, nxyz,
		xyzsize, xyztype, xyzstep, xyz,		//XYZ
		0, 0, 0, NULL,						//ST
		0, 0, 0, NULL,						//Norm
		0, 0, 0, NULL,						//xNorm
		0, 0, 0, NULL,						//yNorm
		0, 0, 0, NULL,						//RGBA
		0, 0, 0, NULL,						//sRGBA
		0, 0, 0, NULL,						//vRGBA
		0, 0, 0, NULL						//tRGBA
	);
}

void RASWGL_BARF_DrawArraysNormalFlat(
	RASWGL_BARF_RenderArray *arr, 
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int normsize, int normtype, int normstep, void *norm)
{
	RASWGL_BARF_ArraysEmitVertices(arr, prim, base, nxyz,
		xyzsize, xyztype, xyzstep, xyz,		//XYZ
		0, 0, 0, NULL,						//ST
		normsize, normtype, normstep, norm,	//Norm
		0, 0, 0, NULL,						//xNorm
		0, 0, 0, NULL,						//yNorm
		0, 0, 0, NULL,						//RGBA
		0, 0, 0, NULL,						//sRGBA
		0, 0, 0, NULL,						//vRGBA
		0, 0, 0, NULL						//tRGBA
	);
}

void RASWGL_BARF_DrawArraysNormalFlatRGB(
	RASWGL_BARF_RenderArray *arr, 
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int normsize, int normtype, int normstep, void *norm,
	int rgbsize, int rgbtype, int rgbstep, void *rgb)
{
	RASWGL_BARF_ArraysEmitVertices(arr, prim, base, nxyz,
		xyzsize, xyztype, xyzstep, xyz,		//XYZ
		0, 0, 0, NULL,						//ST
		normsize, normtype, normstep, norm,	//Norm
		0, 0, 0, NULL,						//xNorm
		0, 0, 0, NULL,						//yNorm
		rgbsize, rgbtype, rgbstep, rgb,		//RGBA
		0, 0, 0, NULL,						//sRGBA
		0, 0, 0, NULL,						//vRGBA
		0, 0, 0, NULL						//tRGBA
	);
}

void RASWGL_BARF_DrawArraysNormalTex(
	RASWGL_BARF_RenderArray *arr, 
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int stsize, int sttype, int ststep, void *st, 
	int normsize, int normtype, int normstep, void *norm)
{
	RASWGL_BARF_ArraysEmitVertices(arr, prim, base, nxyz,
		xyzsize, xyztype, xyzstep, xyz,		//XYZ
		stsize, sttype, ststep, st,			//ST
		normsize, normtype, normstep, norm,	//Norm
		0, 0, 0, NULL,						//xNorm
		0, 0, 0, NULL,						//yNorm
		0, 0, 0, NULL,						//RGBA
		0, 0, 0, NULL,						//sRGBA
		0, 0, 0, NULL,						//vRGBA
		0, 0, 0, NULL						//tRGBA
	);
}

void RASWGL_BARF_DrawArraysNormalTexRGB(
	RASWGL_BARF_RenderArray *arr, 
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int stsize, int sttype, int ststep, void *st, 
	int normsize, int normtype, int normstep, void *norm,
	int rgbsize, int rgbtype, int rgbstep, void *rgb)
{
	RASWGL_BARF_ArraysEmitVertices(arr, prim, base, nxyz,
		xyzsize, xyztype, xyzstep, xyz,		//XYZ
		stsize, sttype, ststep, st,			//ST
		normsize, normtype, normstep, norm,	//Norm
		0, 0, 0, NULL,						//xNorm
		0, 0, 0, NULL,						//yNorm
		rgbsize, rgbtype, rgbstep, rgb,		//RGBA
		0, 0, 0, NULL,						//sRGBA
		0, 0, 0, NULL,						//vRGBA
		0, 0, 0, NULL						//tRGBA
	);
}

void RASWGL_BARF_DrawArraysTexRGB(
	RASWGL_BARF_RenderArray *arr, 
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int stsize, int sttype, int ststep, void *st, 
	int rgbsize, int rgbtype, int rgbstep, void *rgb)
{
	RASWGL_BARF_ArraysEmitVertices(arr, prim, base, nxyz,
		xyzsize, xyztype, xyzstep, xyz,		//XYZ
		stsize, sttype, ststep, st,			//ST
		0, 0, 0, NULL,						//Norm
		0, 0, 0, NULL,						//xNorm
		0, 0, 0, NULL,						//yNorm
		rgbsize, rgbtype, rgbstep, rgb,		//RGBA
		0, 0, 0, NULL,						//sRGBA
		0, 0, 0, NULL,						//vRGBA
		0, 0, 0, NULL						//tRGBA
	);
}
