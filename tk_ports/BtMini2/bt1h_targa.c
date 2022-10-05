/*
Copyright (C) 2015 by Brendan G Bohannon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// #include <bgbrasw.h>

typedef struct PDGL_TGAHeader_s PDGL_TGAHeader;

struct PDGL_TGAHeader_s
{
byte 	id_length, ctype, itype;
u16		cindex, clength;
byte	csize;
u16		x_origin, y_origin, width, height;
byte	bpp, attributes;
};

void *btic1g_malloc(int sz)
{
	return(malloc(sz));
}

void btic1g_free(void *ptr)
{
	free(ptr);
}

byte *BTIC1H_Img_ReadTGA(FILE *fd, int *w, int *h)
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
	obuf=btic1g_malloc(pixlin*4);
	dest=obuf;

	buf=btic1g_malloc(2*pixlin*(head.bpp>>3));
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

	btic1g_free(buf);

//	if(!(head.attributes&32))
	if(head.attributes&32)
	{
		dest=btic1g_malloc(pixlin*4);
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

int BTIC1H_Img_StoreTGA(FILE *fd, byte *buf, int w, int h)
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

byte *BTIC1H_Img_LoadTGA(
	char *name, int *w, int *h)
{
	byte *buf;
	FILE *fd;
	
	fd=fopen(name, "rb");
	if(!fd)return(NULL);
	buf=BTIC1H_Img_ReadTGA(fd, w, h);
	return(buf);
}

int BTIC1H_Img_SaveTGA(
	char *name, byte *buf, int w, int h)
{
	FILE *fd;
	
	fd=fopen(name, "wb");
	if(!fd)return(-1);
	BTIC1H_Img_StoreTGA(fd, buf, w, h);
	return(0);
}



u16 *BTIC1H_Img_LoadTGA555(
	char *name, int *w, int *h)
{
	byte *buf;
	FILE *fd;
	int cr, cg, cb;
	int xs, ys, n;
	int i, j, k;
	
	fd=fopen(name, "rb");
	if(!fd)return(NULL);
	buf=BTIC1H_Img_ReadTGA(fd, &xs, &ys);
	fclose(fd);
	if(!buf)
		return(NULL);
	
	n=xs*ys;
	for(i=0; i<n; i++)
	{
		cr=buf[i*4+0];	cg=buf[i*4+1];	cb=buf[i*4+2];
		j=((cr>>3)<<10)|((cg>>3)<<5)|((cb>>3)<<0);
		((u16 *)buf)[i]=j;
	}
	
	*w=xs;
	*h=ys;
	
	return((u16 *)buf);
}

int BTIC1H_Img_SaveTGA555(
	char *name, u16 *ibuf, int xs, int ys)
{
	byte *tbuf;
	FILE *fd;
	int cr, cg, cb;
	int x, y, w, h, i0, i1;
	int i, j, k, n;
	
	w=xs; h=ys;
	if(h<0)h=-h;
	
	tbuf=malloc((w+4)*(h+4)*4);
	n=w*h;
//	for(i=0; i<n; i++)
	for(y=0; y<h; y++)
	{
		if(ys<0)
			i0=(h-y-1)*w;
		else
			i0=y*w;
		i1=y*w;

		for(x=0; x<w; x++)
		{
			i=i1+x;
			j=ibuf[i0+x];
			cr=(j>>10)&31;			cg=(j>> 5)&31;
			cb=(j>> 0)&31;			cr=(cr<<3)|(cr>>2);
			cg=(cg<<3)|(cg>>2);		cb=(cb<<3)|(cb>>2);
			tbuf[i*4+0]=cr;			tbuf[i*4+1]=cg;
			tbuf[i*4+2]=cb;			tbuf[i*4+3]=255;
		}
	}
	
	fd=fopen(name, "wb");
	if(!fd)return(-1);
	BTIC1H_Img_StoreTGA(fd, tbuf, w, h);
	fclose(fd);
	free(tbuf);
	return(0);
}

u16 *BTIC1H_Img_LoadTGA5551(
	char *name, int *w, int *h)
{
	byte *buf;
	FILE *fd;
	int cr, cg, cb, ca;
	int xs, ys, n;
	int i, j, k;
	
	fd=fopen(name, "rb");
	if(!fd)return(NULL);
	buf=BTIC1H_Img_ReadTGA(fd, &xs, &ys);
	fclose(fd);
	if(!buf)
		return(NULL);
	
	n=xs*ys;
	for(i=0; i<n; i++)
	{
		cr=buf[i*4+0];	cg=buf[i*4+1];	cb=buf[i*4+2];	ca=buf[i*4+3];
		j=((cr>>3)<<11)|((cg>>3)<<6)|((cb>>3)<<1)|(ca>>7);
		((u16 *)buf)[i]=j;
	}
	
	*w=xs;
	*h=ys;
	
	return((u16 *)buf);
}

#if 0
int BTIC1H_Img_StoreTGAPixel(
	FILE *fd, btic1g_pixel *buf, int w, int h)
{
	PDGL_TGAHeader head;
	int i;
	btic1g_pixel *s;

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

int BTIC1H_Img_SaveTGAPixel(
	char *name, btic1g_pixel *buf, int w, int h)
{
	FILE *fd;
	
	fd=fopen(name, "wb");
	if(!fd)return(-1);
	BTIC1H_Img_StoreTGAPixel(fd, buf, w, h);
	return(0);
}

btic1g_pixel *BTIC1H_Img_ReadTGAPixel(FILE *fd, int *w, int *h)
{
	byte *tbuf;
	btic1g_pixel *buf;
	int i, j, n;
	
	tbuf=BTIC1H_Img_ReadTGA(fd, w, h);
	buf=btic1g_malloc((*w)*(*h)*sizeof(btic1g_pixel));
	
	n=(*w)*(*h);
	for(i=0; i<n; i++)
	{
		buf[i]=BGBRASW_MAKEPIXEL(
			tbuf[i*4+0],tbuf[i*4+1],
			tbuf[i*4+2],tbuf[i*4+3]);
	}
	btic1g_free(tbuf);
	return(buf);
}

btic1g_pixel *BTIC1H_Img_LoadTGAPixel(
	char *name, int *w, int *h)
{
	btic1g_pixel *buf;
	FILE *fd;
	
	fd=fopen(name, "rb");
	if(!fd)return(NULL);
	buf=BTIC1H_Img_ReadTGAPixel(fd, w, h);
	return(buf);
}
#endif