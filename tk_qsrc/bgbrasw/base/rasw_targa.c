#include <bgbrasw.h>

typedef struct PDGL_TGAHeader_s PDGL_TGAHeader;

struct PDGL_TGAHeader_s
{
byte 	id_length, ctype, itype;
u16		cindex, clength;
byte	csize;
u16		x_origin, y_origin, width, height;
byte	bpp, attributes;
};

BGBRASW_API byte *BGBRASW_Img_ReadTGA(FILE *fd, int *w, int *h)
{
	PDGL_TGAHeader head;
	int pixlin, i, j;
	byte *dest, *src, *buf, *obuf, r, g, b, a;

	head.id_length=fgetc(fd);
	head.ctype=fgetc(fd);
	head.itype=fgetc(fd);
	head.cindex=fgetc(fd);
	head.cindex+=(fgetc(fd)<<8);
	head.clength=fgetc(fd);
	head.clength+=(fgetc(fd)<<8);
	head.csize=fgetc(fd);
	head.x_origin=fgetc(fd);
	head.x_origin+=(fgetc(fd)<<8);
	head.y_origin=fgetc(fd);
	head.y_origin+=(fgetc(fd)<<8);
	head.width=fgetc(fd);
	head.width+=(fgetc(fd)<<8);
	head.height=fgetc(fd);
	head.height+=(fgetc(fd)<<8);
	head.bpp=fgetc(fd);
	head.attributes=fgetc(fd);

	if(((head.itype!=2) && (head.itype!=3) &&
		(head.itype!=10) && (head.itype!=11)) ||
			((head.bpp!=32) && (head.bpp!=24) && (head.bpp!=8)) ||
			(head.ctype!=0))
	{
		printf("Img_LoadTGA: Unsupported Image Type\n");
		printf("   bpp: %d, ctype: %d, itype: %d\n",
			head.bpp, head.ctype, head.itype);
		return(NULL);
	}

	fseek(fd, head.id_length, 1); /* skip comment */

	pixlin=head.width*head.height;
	obuf=bgbrasw_malloc(pixlin*4);
	dest=obuf;

	buf=bgbrasw_malloc(2*pixlin*(head.bpp>>3));
	fread(buf, 1, 2*pixlin*(head.bpp>>3), fd);
	src=buf;

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

	bgbrasw_free(buf);

//	if(!(head.attributes&32))
	if(head.attributes&32)
	{
		dest=bgbrasw_malloc(pixlin*4);
		for(i=0; i<head.height; i++)
			memcpy(&dest[i*head.width*4],
				&obuf[(head.height-(i+1))*head.width*4], head.width*4);
		memcpy(obuf, dest, pixlin*4);
		free(dest);
	}

	*w=head.width;
	*h=head.height;
	return(obuf);
}

BGBRASW_API int BGBRASW_Img_StoreTGA(FILE *fd, byte *buf, int w, int h)
{
	PDGL_TGAHeader head;
	int i;
	byte *s;

	head.id_length=0;
	head.ctype=0;
	head.itype=2;
	head.cindex=0;
	head.clength=0;
	head.csize=0;
	head.x_origin=0;
	head.y_origin=0;
	head.width=w;
	head.height=h;
	head.bpp=24;
	head.attributes=0;

	fputc(head.id_length, fd);
	fputc(head.ctype, fd);
	fputc(head.itype, fd);

	fputc(head.cindex, fd);
	fputc(head.cindex>>8, fd);
	fputc(head.clength, fd);
	fputc(head.clength>>8, fd);

	fputc(head.csize, fd);

	fputc(head.x_origin, fd);
	fputc(head.x_origin>>8, fd);
	fputc(head.y_origin, fd);
	fputc(head.y_origin>>8, fd);

	fputc(head.width, fd);
	fputc(head.width>>8, fd);
	fputc(head.height, fd);
	fputc(head.height>>8, fd);

	fputc(head.bpp, fd);
	fputc(head.attributes, fd);

//	fwrite(buf, w*4, h, fd);

	s=buf;
	for(i=0; i<w*h; i++)
	{
		fputc(s[2], fd);
		fputc(s[1], fd);
		fputc(s[0], fd);
		s+=4;
	}

	return(0);
}


BGBRASW_API int BGBRASW_Img_StoreTGAPixel(
	FILE *fd, bgbrasw_pixel *buf, int w, int h)
{
	PDGL_TGAHeader head;
	int i;
	bgbrasw_pixel *s;

	head.id_length=0;
	head.ctype=0;
	head.itype=2;
	head.cindex=0;
	head.clength=0;
	head.csize=0;
	head.x_origin=0;
	head.y_origin=0;
	head.width=w;
	head.height=h;
	head.bpp=24;
	head.attributes=0;

	fputc(head.id_length, fd);
	fputc(head.ctype, fd);
	fputc(head.itype, fd);

	fputc(head.cindex, fd);
	fputc(head.cindex>>8, fd);
	fputc(head.clength, fd);
	fputc(head.clength>>8, fd);

	fputc(head.csize, fd);

	fputc(head.x_origin, fd);
	fputc(head.x_origin>>8, fd);
	fputc(head.y_origin, fd);
	fputc(head.y_origin>>8, fd);

	fputc(head.width, fd);
	fputc(head.width>>8, fd);
	fputc(head.height, fd);
	fputc(head.height>>8, fd);

	fputc(head.bpp, fd);
	fputc(head.attributes, fd);

//	fwrite(buf, w*4, h, fd);

	s=buf;
	for(i=0; i<w*h; i++)
	{
		fputc(BGBRASW_PIXEL_B(*s), fd);
		fputc(BGBRASW_PIXEL_G(*s), fd);
		fputc(BGBRASW_PIXEL_R(*s), fd);
		s++;
	}

	return(0);
}

BGBRASW_API int BGBRASW_Img_SaveTGAPixel(
	char *name, bgbrasw_pixel *buf, int w, int h)
{
	FILE *fd;
	
	fd=fopen(name, "wb");
	if(!fd)return(-1);
	BGBRASW_Img_StoreTGAPixel(fd, buf, w, h);
	return(0);
}

BGBRASW_API bgbrasw_pixel *BGBRASW_Img_ReadTGAPixel(FILE *fd, int *w, int *h)
{
	byte *tbuf;
	bgbrasw_pixel *buf;
	int i, j, n;
	
	tbuf=BGBRASW_Img_ReadTGA(fd, w, h);
	buf=bgbrasw_malloc((*w)*(*h)*sizeof(bgbrasw_pixel));
	
	n=(*w)*(*h);
	for(i=0; i<n; i++)
	{
		buf[i]=BGBRASW_MAKEPIXEL(
			tbuf[i*4+0],tbuf[i*4+1],
			tbuf[i*4+2],tbuf[i*4+3]);
	}
	bgbrasw_free(tbuf);
	return(buf);
}

BGBRASW_API bgbrasw_pixel *BGBRASW_Img_LoadTGAPixel(
	char *name, int *w, int *h)
{
	bgbrasw_pixel *buf;
	FILE *fd;
	
	fd=fopen(name, "rb");
	if(!fd)return(NULL);
	buf=BGBRASW_Img_ReadTGAPixel(fd, w, h);
	return(buf);
}
