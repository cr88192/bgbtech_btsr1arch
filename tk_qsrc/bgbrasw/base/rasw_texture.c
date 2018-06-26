#include <bgbrasw.h>

BGBRASW_Texture *BGBRASW_LookupTexture(
	BGBRASW_Context *ctx, int texid)
{
	BGBRASW_Texture *cur;
	int h;
	
	h=((texid*4093)>>12)&1023;
	cur=ctx->texhash[h];
	while(cur)
	{
		if(cur->texid==texid)
			return(cur);
		cur=cur->hnext;
	}
	return(NULL);
}

BGBRASW_Texture *BGBRASW_GetTexture(
	BGBRASW_Context *ctx, int texid)
{
	BGBRASW_Texture *cur;
	int i, j, h;
	
	cur=BGBRASW_LookupTexture(ctx, texid);
	if(cur)
		return(cur);
		
	cur=BGBRASW_AllocTexture(ctx);
	cur->texid=texid;
	
	cur->next=ctx->texlist;
	ctx->texlist=cur;

	h=((texid*4093)>>12)&1023;
	cur->next=ctx->texhash[h];
	ctx->texhash[h]=cur;
	
	if((texid>0) || (texid<32768))
	{
		i=(texid>>5); j=texid&31;
		ctx->tex_alloc[i]|=(1UL<<j);
	}
	
	return(cur);
}

int BGBRASW_DeleteTexture(
	BGBRASW_Context *ctx, int texid)
{
	BGBRASW_Texture *cur;
	int i, j, h;
	
	BGBRASW_Finish(ctx);
	
	cur=BGBRASW_LookupTexture(ctx, texid);
	if(!cur)return(0);
		
	if((texid>0) || (texid<32768))
	{
		i=(texid>>5); j=texid&31;
		ctx->tex_alloc[i]&=~(1UL<<j);
	}

	for(i=0; i<16; i++)
	{
		if(cur->rgba[i])
		{
			bgbrasw_free(cur->rgba[i]);
			cur->rgba[i]=NULL;
		}
	}

	return(0);
}

int BGBRASW_AllocTextureID(BGBRASW_Context *ctx)
{
	int i, j, k;

	ctx->tex_alloc[0]|=1;

	i=ctx->tex_alcrov;
	for(j=0; j<32; j++)
		if(!(ctx->tex_alloc[i]&(1UL<<j)))
	{
		ctx->tex_alloc[i]|=(1UL<<j);
		return(i*32+j);
	}

	for(i=0; i<1024; i++)
	{
		if(ctx->tex_alloc[i]==0xFFFFFFFFUL)
			continue;
		for(j=0; j<32; j++)
			if(!(ctx->tex_alloc[i]&(1UL<<j)))
		{
			ctx->tex_alcrov=i;
			ctx->tex_alloc[i]|=(1UL<<j);
			return(i*32+j);
		}
	}
	return(-1);
}


BGBRASW_API void BGBRASW_SetTexturePixelsSize(
	BGBRASW_Context *ctx, BGBRASW_Texture *tex,
		bgbrasw_pixel *pix, int xs, int ys, int lvl)
{
	int i, j, k;

	BGBRASW_Finish(ctx);

	if(!(tex->rgba[lvl]))
	{
		tex->rgba[lvl]=bgbrasw_malloc(
			xs*ys*sizeof(bgbrasw_pixel));
		if(!lvl)
		{
			tex->xs=xs;
			tex->ys=ys;
		}
	}
	
	if(tex->rgba[lvl] && (tex->xs==(xs<<lvl)) && (tex->ys==(ys<<lvl)))
	{
		if(pix)memcpy(tex->rgba[lvl], pix, xs*ys*sizeof(bgbrasw_pixel));
		return;
	}
	
	if(tex->rgba[lvl])
	{
		bgbrasw_free(tex->rgba[lvl]);
		tex->rgba[lvl]=NULL;
	}

	tex->rgba[lvl]=bgbrasw_malloc(
		xs*ys*sizeof(bgbrasw_pixel));
	if(pix)memcpy(tex->rgba[lvl], pix, xs*ys*sizeof(bgbrasw_pixel));
	if(!lvl)
	{
		tex->xs=xs;
		tex->ys=ys;
	}
}

BGBRASW_API void BGBRASW_SetTextureSizeMip(
	BGBRASW_Context *ctx, BGBRASW_Texture *tex, int xs, int ys, int lvl)
{
	int i, j, k;

//	if(lvl)
//	{
//		if((tex->xs!=(xs<<lvl)) && (tex->ys==(ys<<lvl)))
//			return;
//	}

	BGBRASW_Finish(ctx);

	if(!(tex->rgba[lvl]))
	{
		tex->rgba[lvl]=bgbrasw_malloc(
			xs*ys*sizeof(bgbrasw_pixel));
		if(!lvl)
		{
			tex->xs=xs;
			tex->ys=ys;
		}
		return;
	}
	
	if(lvl)return;

	if((tex->xs==xs) && (tex->ys==ys))
		return;

#if 1
	for(i=0; i<16; i++)
	{
		if(tex->rgba[i])
		{
			bgbrasw_free(tex->rgba[i]);
			tex->rgba[i]=NULL;
		}
	}
#endif

	tex->rgba[lvl]=bgbrasw_malloc(
		xs*ys*sizeof(bgbrasw_pixel));
	tex->xs=xs;
	tex->ys=ys;
}
