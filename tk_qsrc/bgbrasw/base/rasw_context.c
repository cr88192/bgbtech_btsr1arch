#include <bgbrasw.h>

u32 bgbrasw_divtab_s32_u16[65536];

BGBRASW_API void *bgbrasw_malloc(size_t sz)
{
	byte *ptr, *ptr1;

	ptr=malloc(sz+23);
	memset(ptr, 0, sz+23);

	ptr1=ptr+sizeof(byte *);
	ptr1=ptr1+(16-(((int)ptr1)&15));
	((void **)ptr1)[-1]=ptr;

	return(ptr1);
}

BGBRASW_API void *bgbrasw_tmalloc(char *ty, size_t sz)
{
	void *ptr;
	ptr=bgbrasw_malloc(sz);
//	ptr=malloc(sz);
//	memset(ptr, 0, sz);
	return(ptr);
}

BGBRASW_API void bgbrasw_free(void *ptr)
{
	void *ptr1;
	
	ptr1=((void **)ptr)[-1];
	free(ptr1);
}

BGBRASW_API void *bgbrasw_realloc(void *ptr, size_t sz)
{
	byte *ptr1, *ptr2, *ptr3;
	int i, j;

	ptr1=((byte **)ptr)[-1];
	i=((byte *)ptr)-((byte *)ptr1);
	
	ptr2=realloc(ptr1, sz+23);
	ptr3=ptr2+i;
	if(!(((int)ptr3)&15))
	{
		((byte **)ptr3)[-1]=ptr2;
		return(ptr3);
	}

	ptr3=ptr2+sizeof(byte *);
	ptr3=ptr3+(16-(((int)ptr3)&15));
	((byte **)ptr3)[-1]=ptr2;
//	memcpy(ptr3, sz)

	return(ptr3);
}

BGBRASW_API int BGBRASW_InitCore()
{
	static int init=0;
	int i, j;
	
	if(init)return(0);
	init=1;
	
	bgbrasw_divtab_s32_u16[0]=0;
	bgbrasw_divtab_s32_u16[1]=4294967295UL;
	for(i=2; i<65536; i++)
	{
		bgbrasw_divtab_s32_u16[i]=(u32)(4294967296ULL/i);
	}

	return(1);
}

BGBRASW_API BGBRASW_Context *BGBRASW_AllocContext()
{
	BGBRASW_Context *tmp;
	
	tmp=bgbrasw_malloc(sizeof(BGBRASW_Context));
	return(tmp);
}

BGBRASW_API void BGBRASW_SetupRenderBasic(
	BGBRASW_Context *ctx, int xs, int ys, int flags)
{
#ifdef BGBRASW_SS_YUVA
	int xs1, ys1;

	if(!ctx->tgt_ssyuva)
	{
		xs1=(xs+1)>>1;			ys1=(ys+1)>>1;
		ctx->tgt_ssyuva=bgbrasw_malloc(xs1*ys1*sizeof(bgbrasw_yypixel));
		ctx->tgt_sszbuf=bgbrasw_malloc(xs1*ys1*sizeof(bgbrasw_zbuf));
		ctx->tgt_xs=xs1;		ctx->tgt_ys=ys1;
		ctx->tgt_lxs=xs;		ctx->tgt_lys=ys;
	}
#else
	if(!ctx->tgt_rgba)
	{
		ctx->tgt_rgba=bgbrasw_malloc(xs*ys*sizeof(bgbrasw_pixel));
		ctx->tgt_zbuf=bgbrasw_malloc(xs*ys*sizeof(bgbrasw_zbuf));
		ctx->tgt_xs=xs;			ctx->tgt_ys=ys;
		ctx->tgt_lxs=xs;		ctx->tgt_lys=ys;
	}
#endif
}

#ifdef BGBRASW_SS_YUVA
BGBRASW_API void BGBRASW_ClearScreenColor(
	BGBRASW_Context *ctx, bgbrasw_pixel clr)
{
	bgbrasw_yypixel *ct, *cte;
	bgbrasw_yypixel yclr;
	int i, n;
	
	yclr=bgbrasw_pixel2yypixel(clr);
	n=ctx->tgt_xs*ctx->tgt_ys;
	ct=ctx->tgt_ssyuva; cte=ct+n;

	while((ct+8)<=cte)
	{
		*ct++=clr;	*ct++=clr;	*ct++=clr;	*ct++=clr;
		*ct++=clr;	*ct++=clr;	*ct++=clr;	*ct++=clr;
	}

	while(ct<cte)
		{ *ct++=clr; }
}
#else
BGBRASW_API void BGBRASW_ClearScreenColor(
	BGBRASW_Context *ctx, bgbrasw_pixel clr)
{
	bgbrasw_pixel *ct, *cte;
	int i, n;
	
	n=ctx->tgt_xs*ctx->tgt_ys;
	ct=ctx->tgt_rgba; cte=ct+n;

	while((ct+8)<=cte)
	{
		*ct++=clr;	*ct++=clr;	*ct++=clr;	*ct++=clr;
		*ct++=clr;	*ct++=clr;	*ct++=clr;	*ct++=clr;
	}

	while(ct<cte)
		{ *ct++=clr; }
}
#endif

BGBRASW_API void BGBRASW_ClearDepthStencil(
	BGBRASW_Context *ctx, bgbrasw_zbuf clr)
{
	bgbrasw_pixel *ct, *cte;
	int i, n;
	
#ifdef BGBRASW_SS_YUVA
	n=ctx->tgt_xs*ctx->tgt_ys;
	ct=ctx->tgt_sszbuf; cte=ct+n;
#else
	n=ctx->tgt_xs*ctx->tgt_ys;
	ct=ctx->tgt_zbuf; cte=ct+n;
#endif

	while((ct+8)<=cte)
	{
		*ct++=clr;	*ct++=clr;	*ct++=clr;	*ct++=clr;
		*ct++=clr;	*ct++=clr;	*ct++=clr;	*ct++=clr;
	}

	while(ct<cte)
		{ *ct++=clr; }
}

BGBRASW_API void BGBRASW_ClearDepthOnly(
	BGBRASW_Context *ctx, bgbrasw_zbuf clr)
{
	bgbrasw_pixel *ct, *cte;
	int i, n;
	
#ifdef BGBRASW_SS_YUVA
	n=ctx->tgt_xs*ctx->tgt_ys;
	ct=ctx->tgt_sszbuf; cte=ct+n;
#else
	n=ctx->tgt_xs*ctx->tgt_ys;
	ct=ctx->tgt_zbuf; cte=ct+n;
#endif

	while((ct+8)<=cte)
	{
		*ct++=(clr&BGBRASW_MASK_DEPTH)|((*ct)&BGBRASW_MASK_STENCIL);
		*ct++=(clr&BGBRASW_MASK_DEPTH)|((*ct)&BGBRASW_MASK_STENCIL);
		*ct++=(clr&BGBRASW_MASK_DEPTH)|((*ct)&BGBRASW_MASK_STENCIL);
		*ct++=(clr&BGBRASW_MASK_DEPTH)|((*ct)&BGBRASW_MASK_STENCIL);
		*ct++=(clr&BGBRASW_MASK_DEPTH)|((*ct)&BGBRASW_MASK_STENCIL);
		*ct++=(clr&BGBRASW_MASK_DEPTH)|((*ct)&BGBRASW_MASK_STENCIL);
		*ct++=(clr&BGBRASW_MASK_DEPTH)|((*ct)&BGBRASW_MASK_STENCIL);
		*ct++=(clr&BGBRASW_MASK_DEPTH)|((*ct)&BGBRASW_MASK_STENCIL);
	}

	while(ct<cte)
	{
		*ct++=(clr&BGBRASW_MASK_DEPTH)|
			((*ct)&BGBRASW_MASK_STENCIL);
	}
}

BGBRASW_API void BGBRASW_ClearStencilOnly(
	BGBRASW_Context *ctx, bgbrasw_zbuf clr)
{
	bgbrasw_pixel *ct, *cte;
	int i, n;
	
#ifdef BGBRASW_SS_YUVA
	n=ctx->tgt_xs*ctx->tgt_ys;
	ct=ctx->tgt_sszbuf; cte=ct+n;
#else
	n=ctx->tgt_xs*ctx->tgt_ys;
	ct=ctx->tgt_zbuf; cte=ct+n;
#endif

	while((ct+8)<=cte)
	{
		*ct++=(clr&BGBRASW_MASK_STENCIL)|((*ct)&BGBRASW_MASK_DEPTH);
		*ct++=(clr&BGBRASW_MASK_STENCIL)|((*ct)&BGBRASW_MASK_DEPTH);
		*ct++=(clr&BGBRASW_MASK_STENCIL)|((*ct)&BGBRASW_MASK_DEPTH);
		*ct++=(clr&BGBRASW_MASK_STENCIL)|((*ct)&BGBRASW_MASK_DEPTH);
		*ct++=(clr&BGBRASW_MASK_STENCIL)|((*ct)&BGBRASW_MASK_DEPTH);
		*ct++=(clr&BGBRASW_MASK_STENCIL)|((*ct)&BGBRASW_MASK_DEPTH);
		*ct++=(clr&BGBRASW_MASK_STENCIL)|((*ct)&BGBRASW_MASK_DEPTH);
		*ct++=(clr&BGBRASW_MASK_STENCIL)|((*ct)&BGBRASW_MASK_DEPTH);
	}

	while(ct<cte)
	{
		*ct++=(clr&BGBRASW_MASK_STENCIL)|
			((*ct)&BGBRASW_MASK_DEPTH);
	}
}

BGBRASW_API void BGBRASW_FreeContext(BGBRASW_Context *ctx)
{
	BGBRASW_PrimitiveSlab *tscur, *tsnxt;
	
	tscur=ctx->prislab;
	while(tscur)
	{
		tsnxt=tscur->next;
		bgbrasw_free(tscur->first);
		bgbrasw_free(tscur);
		tscur=tsnxt;
	}

	bgbrasw_free(ctx);
}

BGBRASW_API BGBRASW_Primitive *BGBRASW_AllocPrimitive(BGBRASW_Context *ctx)
{
	BGBRASW_Primitive *tmp;
	BGBRASW_PrimitiveSlab *tslab;
	int i, j, k;
	
	if(ctx->prifree)
	{
		tmp=ctx->prifree;
		ctx->prifree=tmp->next;
//		tmp->next=NULL;
		return(tmp);
	}
	
	k=1024;
	tslab=bgbrasw_malloc(sizeof(BGBRASW_PrimitiveSlab));
	tslab->first=bgbrasw_malloc(k*sizeof(BGBRASW_Primitive));
	tslab->cnt=k;
	
	tslab->next=ctx->prislab;
	ctx->prislab=tslab;
	
	for(i=0; i<k; i++)
	{
		tmp=(tslab->first+i);
		tmp->ctx=ctx;
		tmp->next=ctx->prifree;
		ctx->prifree=tmp;

//		(tslab->first+i)->next=ctx->prifree;
//		ctx->prifree=(tslab->first+i);
	}

	tmp=ctx->prifree;
	ctx->prifree=tmp->next;
	return(tmp);
}

BGBRASW_API void BGBRASW_FreePrimitive(
	BGBRASW_Context *ctx, BGBRASW_Primitive *prim)
{
	prim->next=ctx->prifree;
	ctx->prifree=prim;
}

BGBRASW_API void BGBRASW_FreePrimitiveList(
	BGBRASW_Context *ctx, BGBRASW_Primitive *plst)
{
	BGBRASW_Primitive *pcur, *pnxt;

	pcur=plst;
	while(pcur)
	{
		pnxt=pcur->next;
		BGBRASW_FreePrimitive(ctx, pcur);
		pcur=pnxt;
	}

//	prim->next=ctx->prifree;
//	ctx->prifree=prim;
}

BGBRASW_API BGBRASW_Texture *BGBRASW_AllocTexture(BGBRASW_Context *ctx)
{
	BGBRASW_Texture *tmp;
	
	if(ctx->texfree)
	{
		tmp=ctx->texfree;
		ctx->texfree=tmp->next;
		return(tmp);
	}
	
	tmp=bgbrasw_malloc(sizeof(BGBRASW_Texture));
	return(tmp);
}

BGBRASW_API void BGBRASW_FreeTexture(
	BGBRASW_Context *ctx, BGBRASW_Texture *tex)
{
	tex->next=ctx->texfree;
	ctx->texfree=tex;
}

BGBRASW_API BGBRASW_TestBlendData *BGBRASW_AllocTestBlendData(
	BGBRASW_Context *ctx)
{
	BGBRASW_TestBlendData *tmp;
	
	if(ctx->tabsfree)
	{
		tmp=ctx->tabsfree;
		ctx->tabsfree=tmp->next;
		return(tmp);
	}
	
	tmp=bgbrasw_malloc(sizeof(BGBRASW_TestBlendData));
	return(tmp);
}

BGBRASW_API void BGBRASW_FreeTestBlendData(
	BGBRASW_Context *ctx, BGBRASW_TestBlendData *tabs)
{
	tabs->next=ctx->tabsfree;
	ctx->tabsfree=tabs;
}

BGBRASW_API void BGBRASW_SetTextureSize(
	BGBRASW_Context *ctx, BGBRASW_Texture *tex, int xs, int ys)
{
	int i, j, k;

	if(!(tex->rgba[0]))
	{
		tex->rgba[0]=bgbrasw_malloc(
			xs*ys*sizeof(bgbrasw_pixel));
		tex->xs=xs;
		tex->ys=ys;
	}
	
	if((tex->xs==xs) && (tex->ys==ys))
		return;
	
	for(i=0; i<16; i++)
	{
		if(tex->rgba[i])
		{
			bgbrasw_free(tex->rgba[i]);
			tex->rgba[i]=NULL;
		}
	}

	tex->rgba[0]=bgbrasw_malloc(
		xs*ys*sizeof(bgbrasw_pixel));
	tex->xs=xs;
	tex->ys=ys;
}

BGBRASW_API void BGBRASW_SetTextureAllocMip(
	BGBRASW_Context *ctx, BGBRASW_Texture *tex)
{
	int xs, ys, xs1, ys1;
	int i, j, k;

	xs=tex->xs; ys=tex->ys;
	for(i=1; i<16; i++)
	{
		if(tex->rgba[i])
			continue;

		xs1=xs>>i;
		ys1=ys>>i;
		if(!xs1 && !ys1)
			break;
		if(!xs1)xs1=1;
		if(!ys1)ys1=1;
		tex->rgba[1]=bgbrasw_malloc(
			xs1*ys1*sizeof(bgbrasw_pixel));
	}
}

#if 1
int Tex_HalfSampleB(byte *src, byte *dst, int w, int h)
{
	int w2, h2, i2, j2;
	int a1, a2, a3, a4;
	int af1, af2, af3, af4;
	int i, j, k;

	w2=w>>1;
	h2=h>>1;
	for(i=0;i<h2; i++)
		for(j=0; j<w2; j++)
	{
		i2=i<<1;
		j2=j<<1;

		a1=src[((i2*w+j2)<<2)+3];
		a2=src[((i2*w+(j2+1))<<2)+3];
		a3=src[(((i2+1)*w+j2)<<2)+3];
		a4=src[(((i2+1)*w+(j2+1))<<2)+3];

		k=a1+a2+a3+a4;
		if(!k || (k==1020))
		{
//			af1=1023;
//			af2=1023;
//			af3=1023;
//			af4=1023;

			af1=1024;
			af2=1024;
			af3=1024;
			af4=1024;
		}else
		{
//			af1=(4095*a1)/k;
//			af2=(4095*a2)/k;
//			af3=(4095*a3)/k;
//			af4=(4095*a4)/k;

			af1=(4096*a1)/k;
			af2=(4096*a2)/k;
			af3=(4096*a3)/k;
			af4=(4096*a4)/k;
		}
		
		dst[((i*w2+j)<<2)+0]=
			(src[((i2*w+j2)<<2)+0]*af1+
			src[((i2*w+(j2+1))<<2)+0]*af2+
			src[(((i2+1)*w+j2)<<2)+0]*af3+
			src[(((i2+1)*w+(j2+1))<<2)+0]*af4)>>12;

		dst[((i*w2+j)<<2)+1]=
			(src[((i2*w+j2)<<2)+1]*af1+
			src[((i2*w+(j2+1))<<2)+1]*af2+
			src[(((i2+1)*w+j2)<<2)+1]*af3+
			src[(((i2+1)*w+(j2+1))<<2)+1]*af4)>>12;

		dst[((i*w2+j)<<2)+2]=
			(src[((i2*w+j2)<<2)+2]*af1+
			src[((i2*w+(j2+1))<<2)+2]*af2+
			src[(((i2+1)*w+j2)<<2)+2]*af3+
			src[(((i2+1)*w+(j2+1))<<2)+2]*af4)>>12;

//		dst[((i*w2+j)<<2)+3]=
//			(src[((i2*w+j2)<<2)+3]*af1+
//			src[((i2*w+(j2+1))<<2)+3]*af2+
//			src[(((i2+1)*w+j2)<<2)+3]*af3+
//			src[(((i2+1)*w+(j2+1))<<2)+3]*af4)>>12;

		dst[((i*w2+j)<<2)+3]=
			(src[((i2*w+j2)<<2)+3]+		src[((i2*w+(j2+1))<<2)+3]+
			src[(((i2+1)*w+j2)<<2)+3]+	src[(((i2+1)*w+(j2+1))<<2)+3])>>2;
	}
	return(0);
}
#endif

BGBRASW_API void BGBRASW_SetTextureBuildMip(
	BGBRASW_Context *ctx, BGBRASW_Texture *tex)
{
	int xs, ys, xs1, ys1;
	int i, j, k;

	xs=tex->xs; ys=tex->ys;
	for(i=1; i<16; i++)
	{
		if(tex->rgba[i])
			continue;

		xs1=xs>>i;
		ys1=ys>>i;
		if(!xs1 && !ys1)
			break;
		if(!xs1)xs1=1;
		if(!ys1)ys1=1;
		tex->rgba[i]=bgbrasw_malloc(
			xs1*ys1*sizeof(bgbrasw_pixel));
		Tex_HalfSampleB((byte *)tex->rgba[i-1], (byte *)tex->rgba[i],
			xs>>(i-1), ys>>(i-1));
	}
}
