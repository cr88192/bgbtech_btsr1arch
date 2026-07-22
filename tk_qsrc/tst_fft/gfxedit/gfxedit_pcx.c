typedef struct GfxEdit_PcxHeader_s GfxEdit_PcxHeader;

struct GfxEdit_PcxHeader_s {
    sbyte	manufacturer;
    sbyte	version;
    sbyte	encoding;
    sbyte	bits_per_pixel;
    u16		xmin,ymin,xmax,ymax;
    u16		hres,vres;
    byte	palette[48];
    sbyte	reserved;
    sbyte	color_planes;
    u16		bytes_per_line;
    u16		palette_type;
    sbyte	filler[58];
};

byte *GfxEdit_DecodePCX(byte *imgbuf, u32 *rpal, int *rw, int *rh, int flag)
{
	GfxEdit_PcxHeader *head;
	byte *cs, *ct, *cte, *data, *obuf, *pal;
	int xs, ys, c, j, v;
	
	head=(GfxEdit_PcxHeader *)imgbuf;
	data=(byte *)(head+1);
	
	if(	(head->manufacturer != 0x0A) ||
		(head->version != 5) ||
		(head->encoding != 1) ||
		(head->bits_per_pixel != 8))
	{
		return(NULL);
	}
	
	xs=head->xmax+1;	ys=head->ymax+1;
	obuf=malloc(xs*ys);
	
	cs=data; ct=obuf;
	cte=obuf+xs*ys;
	while(ct<cte)
	{
		c=*cs++;
		if(c>=0xC0)
		{
			j=c&63;
			v=*cs++;
			while(j--)
				*ct++=v;
		}else
		{
			*ct++=c;
		}
	}
	
	if(*cs==0x0C)
	{
		pal=(byte *)rpal;
		cs++;
		for(j=0; j<256; j++)
		{
			pal[j*4+0]=cs[2];
			pal[j*4+1]=cs[1];
			pal[j*4+2]=cs[0];
			pal[j*4+3]=255;
			cs+=3;
		}

		for(j=255; j>=2; j--)
		{
			if(rpal[j]!=0xFF000000)
				break;
			rpal[j]=0;
		}
	}
	
	*rw=xs;
	*rh=ys;
	return(obuf);
}

int GfxEdit_EncodeImagePCX8(byte *obuf, byte *ibuf,
	int xs, int ys, u32 *rpal, int fl)
{
	GfxEdit_PcxHeader *head;
	byte *cs, *ct, *cte, *data, *pal;
	int c, j, v, y, x;
	
	head=(GfxEdit_PcxHeader *)obuf;
	data=(byte *)(head+1);
	
	memset(head, 0, sizeof(GfxEdit_PcxHeader));
	head->manufacturer=0x0A;
	head->version=5;
	head->encoding=1;
	head->bits_per_pixel=8;
	head->xmin=0;
	head->ymin=0;
	head->xmax=xs-1;
	head->ymax=ys-1;
	head->hres=72;
	head->vres=72;
	head->color_planes=1;
	head->bytes_per_line=xs;
	head->palette_type=1;
	
	ct=data; cs=ibuf;
	for(y=0; y<ys; y++)
	{
		for(x=0; x<xs; x++)
		{
			c=*cs;
			for(j=1; ((x+j)<xs) && (cs[j]==c); j++);
			if(j>63)
				j=63;
			if(j>1)
			{
				*ct++=0xC1+(j-1);
				*ct++=c;
				cs+=j;
				x+=(j-1);
				continue;
			}
		
			c=*cs++;
			if(c<0xC0)
			{
				*ct++=c;
			}else
			{
				*ct++=0xC1;
				*ct++=c;
			}
		}
	}
	
	pal=(byte *)rpal;
	*ct++=0x0C;
	for(j=0; j<256; j++)
	{
		*ct++=pal[j*4+2];
		*ct++=pal[j*4+1];
		*ct++=pal[j*4+0];
	}
	
	return(ct-obuf);
}
