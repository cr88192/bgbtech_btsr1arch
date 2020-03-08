int tkgl_log2dn(int val)
{
	int v, i;
	
	v=val; i=0;
	while(v>1)
		{ v=v>>1; i++; }
	return(i);
}

TKGL_RastTexImage *TKGL_AllocTexImg()
{
	TKGL_RastTexImage *tmp;
	
	tmp=tkgl_malloc(sizeof(TKGL_RastTexImage));
	memset(tmp, 0, sizeof(TKGL_RastTexImage));
	return(tmp);
}

void TKGL_UpdateTexImg(TKGL_RastTexImage *img,
	tkgl_rastpixel *buf, int xs, int ys, int mip)
{
	int xshl, yshl, xsh1, ysh1;
	int i, j, k;
	
	if(xs&(xs-1))
		return;
	if(ys&(ys-1))
		return;

	xshl=tkgl_log2dn(xs);
	yshl=tkgl_log2dn(ys);

	if(mip)
	{
		xsh1=img->tex_xshl-mip;
		ysh1=img->tex_yshl-mip;
		if(xsh1<1)xsh1=1;
		if(ysh1<1)ysh1=1;
		
		if((xshl!=xsh1) || (yshl!=ysh1))
			return;
		k=(1<<xsh1)*(1<<ysh1);
		memcpy(img->tex_img+img->tex_mipofs[mip], buf, k*sizeof(tkgl_rastpixel));
	}else
	{
		if((xshl!=img->tex_xshl) || (yshl!=img->tex_yshl))
		{
			if(img->tex_img)
			{
				tkgl_free(img->tex_img);
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

			img->tex_img=tkgl_malloc(j*sizeof(tkgl_rastpixel));
			
			for(k=0; k<j; k++)
				img->tex_img[k]=0x7FFF;
		}

		k=(1<<xshl)*(1<<yshl);
		memcpy(img->tex_img, buf, k*sizeof(tkgl_rastpixel));
	}
}
