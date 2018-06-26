int BTIC1H_Tex_HalfSampleA(byte *src, int w, int h)
{
	int w2, h2, i2, j2, k2;
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
			af1=1024;
			af2=1024;
			af3=1024;
			af4=1024;
		}else
		{
			k2=262144/k;
			af1=(k2*a1+7)>>6;
			af2=(k2*a2+7)>>6;
			af3=(k2*a3+7)>>6;
			af4=(k2*a4+7)>>6;

//			af1=(4096*a1)/k;
//			af2=(4096*a2)/k;
//			af3=(4096*a3)/k;
//			af4=(4096*a4)/k;
		}
		
		src[((i*w2+j)<<2)+0]=
			(src[((i2*w+j2)<<2)+0]*af1+
			src[((i2*w+(j2+1))<<2)+0]*af2+
			src[(((i2+1)*w+j2)<<2)+0]*af3+
			src[(((i2+1)*w+(j2+1))<<2)+0]*af4)>>12;

		src[((i*w2+j)<<2)+1]=
			(src[((i2*w+j2)<<2)+1]*af1+
			src[((i2*w+(j2+1))<<2)+1]*af2+
			src[(((i2+1)*w+j2)<<2)+1]*af3+
			src[(((i2+1)*w+(j2+1))<<2)+1]*af4)>>12;

		src[((i*w2+j)<<2)+2]=
			(src[((i2*w+j2)<<2)+2]*af1+
			src[((i2*w+(j2+1))<<2)+2]*af2+
			src[(((i2+1)*w+j2)<<2)+2]*af3+
			src[(((i2+1)*w+(j2+1))<<2)+2]*af4)>>12;

		src[((i*w2+j)<<2)+3]=
			(src[((i2*w+j2)<<2)+3]+		src[((i2*w+(j2+1))<<2)+3]+
			src[(((i2+1)*w+j2)<<2)+3]+	src[(((i2+1)*w+(j2+1))<<2)+3])>>2;
	}
	return(0);
}

int BTIC1H_Tex_HalfSampleB(byte *src, int w, int h)
{
	int i, j, k, w2, h2, i2, j2;

	w2=w>>1;
	h2=h>>1;
	for(i=0;i<h2; i++)
		for(j=0; j<w2; j++)
	{
		i2=i<<1;
		j2=j<<1;

		k= (src[(((i2  )*w+j2)<<2)+0]+	src[(((i2  )*w+(j2+1))<<2)+0]+
			src[(((i2+1)*w+j2)<<2)+0]+	src[(((i2+1)*w+(j2+1))<<2)+0])>>2;
		src[((i*w2+j)<<2)+0]=k;

		k= (src[(((i2  )*w+j2)<<2)+1]+	src[(((i2  )*w+(j2+1))<<2)+1]+
			src[(((i2+1)*w+j2)<<2)+1]+	src[(((i2+1)*w+(j2+1))<<2)+1])>>2;
		src[((i*w2+j)<<2)+1]=k;

		k= (src[(((i2  )*w+j2)<<2)+2]+	src[(((i2  )*w+(j2+1))<<2)+2]+
			src[(((i2+1)*w+j2)<<2)+2]+	src[(((i2+1)*w+(j2+1))<<2)+2])>>2;
		src[((i*w2+j)<<2)+2]=k;

		k= (src[(((i2  )*w+j2)<<2)+3]+	src[(((i2  )*w+(j2+1))<<2)+3]+
			src[(((i2+1)*w+j2)<<2)+3]+	src[(((i2+1)*w+(j2+1))<<2)+3])>>2;
		src[((i*w2+j)<<2)+3]=k;
	}
	return(0);
}

int BTIC1H_Tex_HalfSample(byte *src, int w, int h, int clrs)
{
	if(	(clrs==BTIC1H_PXF_RGBA) ||
		(clrs==BTIC1H_PXF_BGRA))
	{
		return(BTIC1H_Tex_HalfSampleA(src, w, h));
	}

	if(	(clrs==BTIC1H_PXF_RGBX) ||
		(clrs==BTIC1H_PXF_BGRX))
	{
		return(BTIC1H_Tex_HalfSampleB(src, w, h));
	}
	
	return(-1);
}

int BTIC1H_Tex_HalfSample2(byte *src, int w, int h)
{
	int i, j, k, w2, h2, i2, j2;

	w2=w>>1;
	h2=h>>1;
	for(i=0;i<h2; i++)
		for(j=0; j<w2; j++)
	{
		i2=i<<1;
		j2=j<<1;

		k=	(src[((i2*w+j2)<<2)+0]+		src[((i2*w+(j2+1))<<2)+0]+
			src[(((i2+1)*w+j2)<<2)+0]+	src[(((i2+1)*w+(j2+1))<<2)+0]);
		if(k>255)k=255;
		src[((i*w2+j)<<2)+0]=k;

		k=	(src[((i2*w+j2)<<2)+1]+		src[((i2*w+(j2+1))<<2)+1]+
			src[(((i2+1)*w+j2)<<2)+1]+	src[(((i2+1)*w+(j2+1))<<2)+1]);
		if(k>255)k=255;
		src[((i*w2+j)<<2)+1]=k;

		k=	(src[((i2*w+j2)<<2)+2]+		src[((i2*w+(j2+1))<<2)+2]+
			src[(((i2+1)*w+j2)<<2)+2]+	src[(((i2+1)*w+(j2+1))<<2)+2]);
		if(k>255)k=255;
		src[((i*w2+j)<<2)+2]=k;

		k=	(src[((i2*w+j2)<<2)+3]+		src[((i2*w+(j2+1))<<2)+3]+
			src[(((i2+1)*w+j2)<<2)+3]+	src[(((i2+1)*w+(j2+1))<<2)+3]);
		if(k>255)k=255;
		src[((i*w2+j)<<2)+3]=k;
	}
	return(0);
}
