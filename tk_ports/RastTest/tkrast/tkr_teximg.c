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

void TKRA_UpdateTexImg(TKRA_TexImage *img,
	tkra_rastpixel *buf, int xs, int ys, int mip)
{
	int xshl, yshl, xsh1, ysh1;
	int x, y, m;
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
		if(xsh1<1)xsh1=1;
		if(ysh1<1)ysh1=1;
		
		if((xshl!=xsh1) || (yshl!=ysh1))
			return;
		k=(1<<xsh1)*(1<<ysh1);
		memcpy(img->tex_img+img->tex_mipofs[mip], buf,
			k*sizeof(tkra_rastpixel));
	}else if(mip==0)
	{
		if((xshl!=img->tex_xshl) || (yshl!=img->tex_yshl))
		{
			if(img->tex_img)
			{
				tkra_free(img->tex_img);
				img->tex_img=NULL;
			}

			i=0; j=0; xsh1=xshl; ysh1=yshl;
			while((xsh1>1) || (ysh1>1))
			{
				img->tex_mipofs[i]=j;
				j+=(1<<xsh1)*(1<<ysh1);
				i++;
				if(xsh1>1)xsh1--;
				if(ysh1>1)ysh1--;
			}

			img->tex_img=tkra_malloc(j*sizeof(tkra_rastpixel));
			img->tex_xshl=xshl;
			img->tex_yshl=yshl;
			
			for(k=0; k<j; k++)
				img->tex_img[k]=0x7FFF;
		}

		k=(1<<xshl)*(1<<yshl);
		memcpy(img->tex_img, buf, k*sizeof(tkra_rastpixel));
	}else
	{
		if((xshl!=img->tex_xshl) || (yshl!=img->tex_yshl))
		{
			if(img->tex_img)
			{
				tkra_free(img->tex_img);
				img->tex_img=NULL;
			}

			img->tex_mipofs[0]=0;
			j=(1<<xshl)*(1<<yshl);
			img->tex_img=tkra_malloc(j*sizeof(tkra_rastpixel));
			img->tex_xshl=xshl;
			img->tex_yshl=yshl;
			
			for(k=0; k<j; k++)
				img->tex_img[k]=0x7FFF;
		}

		if(xshl==yshl)
		{
			for(y=0; y<ys; y++)
				for(x=0; x<xs; x++)
			{
				j=(y<<xshl)+x;
				k=tkra_morton8(x, y);
				img->tex_img[k]=buf[j];
			}
		}else
		{
			k=(1<<xshl)*(1<<yshl);
			memcpy(img->tex_img, buf, k*sizeof(tkra_rastpixel));
		}
	}
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
	ctx->tex_xshl=img->tex_xshl;
	ctx->tex_yshl=img->tex_yshl;
	return(0);
}

int TKRA_SetupForState(TKRA_Context *ctx)
{
	TKRA_SetupDrawEdgeForState(ctx);
	return(0);
}
