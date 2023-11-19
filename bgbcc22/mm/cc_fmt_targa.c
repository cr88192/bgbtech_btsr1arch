typedef struct PDGL_TGAHeader_s PDGL_TGAHeader;

struct PDGL_TGAHeader_s
{
byte 	id_length, ctype, itype;
u16		cindex, clength;
byte	csize;
u16		x_origin, y_origin, width, height;
byte	bpp, attributes;
};

int BGBCC_Img_DecodeProbeIsTGA(byte *imgbuf, int *w, int *h)
{
	PDGL_TGAHeader head;
	byte *cs;

	cs=imgbuf;

	head.id_length=*cs++;
	head.ctype=*cs++;
	head.itype=*cs++;
	head.cindex=*cs++;
	head.cindex+=(*cs++<<8);
	head.clength=*cs++;
	head.clength+=(*cs++<<8);
	head.csize=*cs++;
	head.x_origin=*cs++;
	head.x_origin+=(*cs++<<8);
	head.y_origin=*cs++;
	head.y_origin+=(*cs++<<8);
	head.width=*cs++;
	head.width+=(*cs++<<8);
	head.height=*cs++;
	head.height+=(*cs++<<8);
	head.bpp=*cs++;
	head.attributes=*cs++;

	if(((head.itype!=2) && (head.itype!=3) &&
		(head.itype!=10) && (head.itype!=11)) ||
			((head.bpp!=32) && (head.bpp!=24) && (head.bpp!=8)) ||
			(head.ctype!=0))
	{
		return(0);
	}
	
	if(w)	*w=head.width;
	if(h)	*h=head.height;
	return(1);
}


byte *BGBCC_Img_DecodeTGA(byte *imgbuf, int *w, int *h)
{
	PDGL_TGAHeader head;
	int pixlin, i, j;
	byte *dest, *src, *buf, *obuf, r, g, b, a;
	byte *cs;

	cs=imgbuf;

	head.id_length=*cs++;
	head.ctype=*cs++;
	head.itype=*cs++;
	head.cindex=*cs++;
	head.cindex+=(*cs++<<8);
	head.clength=*cs++;
	head.clength+=(*cs++<<8);
	head.csize=*cs++;
	head.x_origin=*cs++;
	head.x_origin+=(*cs++<<8);
	head.y_origin=*cs++;
	head.y_origin+=(*cs++<<8);
	head.width=*cs++;
	head.width+=(*cs++<<8);
	head.height=*cs++;
	head.height+=(*cs++<<8);
	head.bpp=*cs++;
	head.attributes=*cs++;

	if(((head.itype!=2) && (head.itype!=3) &&
		(head.itype!=10) && (head.itype!=11)) ||
			((head.bpp!=32) && (head.bpp!=24) && (head.bpp!=8)) ||
			(head.ctype!=0))
	{
//		printf("Img_LoadTGA: Unsupported Image Type\n");
//		printf("   bpp: %d, ctype: %d, itype: %d\n",
//			head.bpp, head.ctype, head.itype);
		return(NULL);
	}

	cs+=head.id_length;

	pixlin=head.width*head.height;
	obuf=bgbcc_malloc(pixlin*4);
	dest=obuf;

//	buf=bgbcc_malloc(2*pixlin*(head.bpp>>3));
//	fread(buf, 1, 2*pixlin*(head.bpp>>3), fd);
//	memcpy(buf, cs, 2*pixlin*(head.bpp>>3));
	src=cs;

	if(head.itype==2)
	{
		for(i=0; i<pixlin; i++)
		{
			b=*src++;
			g=*src++;
			r=*src++;
			if(head.bpp==32)
				a=*src++;
				else a=255;
			*dest++=r;
			*dest++=g;
			*dest++=b;
			*dest++=a;
		}
	}

	if(head.itype==3)
	{
		for(i=0; i<pixlin; i++)
		{
			g=*src++;
			*dest++=g;
			*dest++=g;
			*dest++=g;
			*dest++=255;
		}
	}

	if(head.itype==10)
	{
		i=0;
		while(i<pixlin)
		{
			j=*src++;
			if(j<128)
			{
				j++;
				while(j)
				{
					b=*src++;
					g=*src++;
					r=*src++;
					if(head.bpp==32)
						a=*src++;
						else a=255;
					*dest++=r;
					*dest++=g;
					*dest++=b;
					*dest++=a;

					j--;
					i++;
				}
			}else
			{
				b=*src++;
				g=*src++;
				r=*src++;
				if(head.bpp==32)
					a=*src++;
					else a=255;

				j-=127;
				while(j)
				{
					*dest++=r;
					*dest++=g;
					*dest++=b;
					*dest++=a;

					j--;
					i++;
				}
			}
		}
	}

	if(head.itype==11)
	{
		i=0;
		while(i<pixlin)
		{
			j=*src++;
			if(j<128)
			{
				j++;
				while(j)
				{
					g=*src++;
					b=g; r=g;
					a=255;
					*dest++=r;
					*dest++=g;
					*dest++=b;
					*dest++=a;

					j--;
					i++;
				}
			}else
			{
				g=*src++;
				b=g; r=g;
				a=255;

				j-=127;
				while(j)
				{
					*dest++=r;
					*dest++=g;
					*dest++=b;
					*dest++=a;

					j--;
					i++;
				}
			}
		}
	}

//	btic1g_free(buf);

//	if(!(head.attributes&32))
	if(head.attributes&32)
	{
		dest=bgbcc_malloc(pixlin*4);
		for(i=0; i<head.height; i++)
			memcpy(&dest[i*head.width*4],
				&obuf[(head.height-(i+1))*head.width*4], head.width*4);
		memcpy(obuf, dest, pixlin*4);
		bgbcc_free(dest);
	}

	*w=head.width;
	*h=head.height;
	return(obuf);
}
