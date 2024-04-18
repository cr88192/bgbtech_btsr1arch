#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef unsigned char byte;

typedef unsigned short u16;
typedef signed short s16;

typedef unsigned int u32;
typedef signed int s32;

typedef struct TargaHeader_s {
	unsigned char 	id_length, ctype, itype;
	unsigned short	cindex, clength;
	unsigned char	csize;
	unsigned short	x_origin, y_origin, width, height;
	unsigned char	bpp, attributes;
} TargaHeader;

byte *BGBBTJ_Img_LoadTGA(FILE *fd, int *w, int *h)
{
	TargaHeader head;
	int pixlin, i, j;
	byte *dest, *src, *buf, *obuf, r, g, b, a;

	head.id_length=fgetc(fd);
	head.ctype=fgetc(fd);
	head.itype=fgetc(fd);
	head.cindex=fgetc(fd)+(fgetc(fd)<<8);
	head.clength=fgetc(fd)+(fgetc(fd)<<8);
	head.csize=fgetc(fd);
	head.x_origin=fgetc(fd)+(fgetc(fd)<<8);
	head.y_origin=fgetc(fd)+(fgetc(fd)<<8);
	head.width=fgetc(fd)+(fgetc(fd)<<8);
	head.height=fgetc(fd)+(fgetc(fd)<<8);
	head.bpp=fgetc(fd);
	head.attributes=fgetc(fd);

	if(((head.itype!=2) && (head.itype!=10)) ||
			((head.bpp!=32) && (head.bpp!=24)) || (head.ctype!=0))
	{
		printf("Img_LoadTGA: only RGB(24)/RGBA(32) uncompressed "
			"images supported.\n");
		printf("   bpp: %d, type: %d\n", head.bpp, head.ctype);
		return(NULL);
	}

	fseek(fd, head.id_length, 1); /* skip comment */

	pixlin=head.width*head.height;
	obuf=malloc(pixlin*4);
	dest=obuf;

	buf=malloc(2*pixlin*(head.bpp>>3));
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

	free(buf);

	if(head.attributes&32)
	{
		dest=malloc(pixlin*4);
		for(i=0; i<head.height; i++)
			memcpy(&dest[i*head.width*4],
				&obuf[(head.height-(i+1))*head.width*4],
				head.width*4);
		memcpy(obuf, dest, pixlin*4);
		free(dest);
	}

	*w=head.width;
	*h=head.height;
	return(obuf);
}

int BGBBTJ_Img_StoreTGA(FILE *fd, byte *buf, int w, int h)
{
	TargaHeader head;
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

//	vfwrite(buf, w*4, h, fd);

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

byte *BGBBTJ_TGA_LoadRaw(char *name, int *w, int *h)
{
	FILE *fd;
	byte *tmp;

	fd=fopen(name, "rb");
	if(!fd)
	{
		printf("PCX_LoadRaw: can't open '%s'\n", name);
		return(NULL);
	}

	tmp=BGBBTJ_Img_LoadTGA(fd, w, h);
	fclose(fd);

	return(tmp);
}

int BGBBTJ_TGA_StoreRaw(char *name, byte *buf, int w, int h)
{
	FILE *fd;

	fd=fopen(name, "wb");
	if(!fd)
	{
		printf("BGBBTJ_TGA_StoreRaw: can't open '%s'\n", name);
		return(-1);
	}

	BGBBTJ_Img_StoreTGA(fd, buf, w, h);
	fclose(fd);

	return(0);
}

struct TKGDI_BITMAPINFOHEADER_s {
	u32	biSize;
	s32	biWidth;
	s32	biHeight;
	u16	biPlanes;
	u16	biBitCount;
	u32	biCompression;
	u32	biSizeImage;
	s32	biXPelsPerMeter;
	s32	biYPelsPerMeter;
	u32	biClrUsed;
	u32	biClrImportant;
};

int BGBBTJ_Img_StoreBMP8(FILE *fd, byte *buf, int w, int h, byte *pal)
{
	byte tobuf[2048];
	struct TKGDI_BITMAPINFOHEADER_s *bmi;
	int i, sz, ofs_dat, ofs_pal;
	byte *s;
	
	bmi=(struct TKGDI_BITMAPINFOHEADER_s *)(tobuf+0xE);

	bmi->biSize=40;
	bmi->biWidth=w;
	bmi->biHeight=h;
	bmi->biPlanes=1;
	bmi->biBitCount=8;
	bmi->biCompression=0;
	bmi->biSizeImage=w*h;
	bmi->biXPelsPerMeter=2835;
	bmi->biYPelsPerMeter=2835;
	bmi->biClrUsed=256;
	bmi->biClrImportant=256;

	ofs_pal=0x0036;
	ofs_dat=0x0440;
	sz=ofs_dat+w*h;

	memcpy(tobuf, "BM", 2);
	tobuf[2]=sz;
	tobuf[3]=sz>>8;
	tobuf[4]=sz>>16;
	tobuf[5]=sz>>24;

	tobuf[6]=0;
	tobuf[7]=0;
	tobuf[8]=0;
	tobuf[9]=0;

	tobuf[10]=ofs_dat;
	tobuf[11]=ofs_dat>>8;
	tobuf[12]=ofs_dat>>16;
	tobuf[13]=ofs_dat>>24;

	for(i=0; i<256; i++)
	{
		tobuf[ofs_pal+i*4+0]=pal[i*3+2];
		tobuf[ofs_pal+i*4+1]=pal[i*3+1];
		tobuf[ofs_pal+i*4+2]=pal[i*3+0];
		tobuf[ofs_pal+i*4+3]=255;
	}

	fwrite(tobuf, 1, ofs_dat, fd);
	fwrite(buf, 1, w*h, fd);

	return(0);
}

int BGBBTJ_BMP8_StoreRaw(char *name, byte *buf, int w, int h, byte *pal)
{
	FILE *fd;

	fd=fopen(name, "wb");
	if(!fd)
	{
		printf("BGBBTJ_BMP8_StoreRaw: can't open '%s'\n", name);
		return(-1);
	}

	BGBBTJ_Img_StoreBMP8(fd, buf, w, h, pal);
	fclose(fd);

	return(0);
}

void gensdf_getcell(byte *ibuf, int xs, int ys,
	byte *cell, int xo, int yo, int cw, int ch)
{
	int cr, cg, cb, cy;
	int i, j, k;
	
	for(i=0; i<ch; i++)
		for(j=0; j<cw; j++)
	{
		k=(yo+i)*xs+xo+j;
		cr=ibuf[k*4+0];
		cg=ibuf[k*4+1];
		cb=ibuf[k*4+2];
		cy=(2*cg+cr+cb)>>2;
		cell[i*cw+j]=cy;
	}
}

int gensdf_dfcn0(int x0, int y0, int x1, int y1)
{
	int dy, dx, d;
	dx=x1-x0; dy=y1-y0;
	d=dx*dx+dy*dy;
	return(d);
}

int gensdf_dfcn1(int x0, int y0, int x1, int y1)
{
	int dy, dx, dx1, dy1, d0, d1, d;
	dx=x1-x0; dy=y1-y0;
//	dx=dx<<1; dy=dy>>1;
//	dy=0;
//	dy=dy>>1;
//	dy=(dy*3)>>2;
//	d=dx*dx+dy*dy;

//	dy1=(dy*3)>>2;
	dy1=dy>>2;
//	dy1=0;
	d0=dx*dx+dy*dy;
	d1=dx*dx+dy1*dy1;
//	d=2*d1-d0;
	d=2*d0-d1;
//	d=2*d-d1;

//	d=dx+dy; d=d*d;

	return(d);
}

int gensdf_dfcn2(int x0, int y0, int x1, int y1)
{
	int dy, dx, dx1, dy1, d0, d1, d;
	dx=x1-x0; dy=y1-y0;
//	dx=dx>>1; dy=dy<<1;
//	dx=0;
//	dx=dx>>1;
//	dx=(dx*3)>>2;
//	d=dx*dx+dy*dy;

//	dx1=(dx*3)>>2;
	dx1=dx>>2;
//	dx1=0;
	d0=dx*dx+dy*dy;
	d1=dx1*dx1+dy*dy;
//	d=2*d1-d0;
	d=2*d0-d1;
//	d=2*d-d1;

//	d=dx-dy; d=d*d;

	return(d);
}

void gensdf_sdfcell(byte *icell, byte *ocell,
	int cw, int ch, int axis)
{
	int (*dfcn)(int x0, int y0, int x1, int y1);
	int tcell[64*64];
	int cw2, ch2;
	int x, y, x1, y1, dx, dy, d, bd, mxd;
	float f, g, h;
	int i, j, k;
	
	dfcn=gensdf_dfcn0;
	if(axis==1)
		dfcn=gensdf_dfcn1;
	if(axis==2)
		dfcn=gensdf_dfcn2;
	
	mxd=1;
	for(y=0; y<ch; y++)
		for(x=0; x<cw; x++)
	{
		if(icell[y*cw+x]>128)
		{
			bd=999999999;

			for(x1=0; x1<cw; x1++)
			{
				if(icell[y*cw+x1]>128)
					continue;
				d=dfcn(x, y, x1, y);
				if(d<bd)
					{ bd=d; }
			}

			for(y1=0; y1<ch; y1++)
			{
				if(icell[y1*cw+x]>128)
					continue;
				d=dfcn(x, y, x, y1);
				if(d<bd)
					{ bd=d; }
			}

			for(y1=0; y1<ch; y1++)
			{
//				d=y-y1; d=d*d;
				d=dfcn(x, y, x, y1);
				if(bd<d)continue;
				for(x1=0; x1<cw; x1++)
				{
					if(icell[y1*cw+x1]>128)
						continue;
					d=dfcn(x, y, x1, y1);
					if(d<bd)
						{ bd=d; }
				}
			}
			tcell[y*cw+x]=bd;
			if(bd>mxd)mxd=bd;
		}else
		{
			bd=999999999;

			for(x1=0; x1<cw; x1++)
			{
				if(icell[y*cw+x1]<128)
					continue;
				d=dfcn(x, y, x1, y);
				if(d<bd)
					{ bd=d; }
			}

			for(y1=0; y1<ch; y1++)
			{
				if(icell[y1*cw+x]<128)
					continue;
				d=dfcn(x, y, x, y1);
				if(d<bd)
					{ bd=d; }
			}

			for(y1=0; y1<ch; y1++)
			{
//				d=y-y1; d=d*d;
				d=dfcn(x, y, x, y1);
				if(bd<d)continue;
				for(x1=0; x1<cw; x1++)
				{
					if(icell[y1*cw+x1]<128)
						continue;
					d=dfcn(x, y, x1, y1);
					if(d<bd)
						{ bd=d; }
				}
			}
			tcell[y*cw+x]=-bd;
			if(bd>mxd)mxd=bd;
		}
	}
	
	if(mxd==999999999)
	{
		mxd=2;
		for(y=0; y<ch; y++)
			for(x=0; x<cw; x++)
		{
			tcell[y*cw+x]=-1;
		}
	}

#if 0
	for(y=0; y<ch; y++)
	{
		i=tcell[y*cw+0];
		for(x=0; x<cw; x++)
		{
			j=tcell[y*cw+x];
//			j=(j+i)>>1;
//			j=(255*j+i)>>8;
			k=2*j-i;
			k=(j+k)>>1;
//			k=(3*j+k)>>2;
			tcell[y*cw+x]=k;
			i=j;
		}
		i=tcell[y*cw+(cw-1)];
		for(x=cw-1; x>=0; x--)
		{
			j=tcell[y*cw+x];
//			j=(j+i)>>1;
//			j=(255*j+i)>>8;
			k=2*j-i;
			k=(j+k)>>1;
//			k=(3*j+k)>>2;
			tcell[y*cw+x]=k;
			i=j;
		}
	}

#if 1
	for(x=0; x<cw; x++)
	{
		i=tcell[0*cw+x];
		for(y=0; y<ch; y++)
		{
			j=tcell[y*cw+x];
//			j=(j+k)>>1;
			k=2*j-i;
			k=(j+k)>>1;
//			k=(3*j+k)>>2;
			tcell[y*cw+x]=k;
			i=j;
		}
		i=tcell[(ch-1)*cw+x];
		for(y=ch-1; y>=0; y--)
		{
			j=tcell[y*cw+x];
//			j=(j+k)>>1;
			k=2*j-i;
			k=(j+k)>>1;
//			k=(3*j+k)>>2;
			tcell[y*cw+x]=k;
			i=j;
		}
	}
#endif
#endif

#if 0
	mxd=1;
	for(y=0; y<ch2; y++)
		for(x=0; x<cw2; x++)
	{
		d=(	tcell[(y*4+0)*cw+(x*4+0)]+tcell[(y*4+0)*cw+(x*4+1)]+
			tcell[(y*4+0)*cw+(x*4+2)]+tcell[(y*4+0)*cw+(x*4+3)]+
			tcell[(y*4+1)*cw+(x*4+0)]+tcell[(y*4+1)*cw+(x*4+1)]+
			tcell[(y*4+1)*cw+(x*4+2)]+tcell[(y*4+1)*cw+(x*4+3)]+
			tcell[(y*4+2)*cw+(x*4+0)]+tcell[(y*4+2)*cw+(x*4+1)]+
			tcell[(y*4+2)*cw+(x*4+2)]+tcell[(y*4+2)*cw+(x*4+3)]+
			tcell[(y*4+3)*cw+(x*4+0)]+tcell[(y*4+3)*cw+(x*4+1)]+
			tcell[(y*4+3)*cw+(x*4+2)]+tcell[(y*4+3)*cw+(x*4+3)])/16;
		if(d<0)d=-d;
		if(d>mxd)d=mxd;
	}
#endif
	
	f=128.0/mxd;
	f=128.0/sqrt(mxd);
	
	cw2=cw/4; ch2=ch/4;
	for(y=0; y<ch2; y++)
		for(x=0; x<cw2; x++)
	{
//		d=(tcell[(y*2+0)*cw+(x*2+0)]+
//			tcell[(y*2+0)*cw+(x*2+1)]+
//			tcell[(y*2+1)*cw+(x*2+0)]+
//			tcell[(y*2+1)*cw+(x*2+1)])/4;

//		d=(tcell[(y*4+0)*cw+(x*4+0)]+
//			tcell[(y*4+0)*cw+(x*4+2)]+
//			tcell[(y*4+2)*cw+(x*4+0)]+
//			tcell[(y*4+2)*cw+(x*4+2)])/4;

		d=(	tcell[(y*4+0)*cw+(x*4+0)]+tcell[(y*4+0)*cw+(x*4+1)]+
			tcell[(y*4+0)*cw+(x*4+2)]+tcell[(y*4+0)*cw+(x*4+3)]+
			tcell[(y*4+1)*cw+(x*4+0)]+tcell[(y*4+1)*cw+(x*4+1)]+
			tcell[(y*4+1)*cw+(x*4+2)]+tcell[(y*4+1)*cw+(x*4+3)]+
			tcell[(y*4+2)*cw+(x*4+0)]+tcell[(y*4+2)*cw+(x*4+1)]+
			tcell[(y*4+2)*cw+(x*4+2)]+tcell[(y*4+2)*cw+(x*4+3)]+
			tcell[(y*4+3)*cw+(x*4+0)]+tcell[(y*4+3)*cw+(x*4+1)]+
			tcell[(y*4+3)*cw+(x*4+2)]+tcell[(y*4+3)*cw+(x*4+3)]+
			0)/16;

//		d=sqrt(d);
		d=(d>=0)?sqrt(d):-sqrt(-d);
		d=128+d*f;
		if(d<0)d=0;
		if(d>255)d=255;
		ocell[y*cw2+x]=d;
	}
}

void gensdf_putcell(byte *ibuf, int xs, int ys,
	byte *cell, int xo, int yo, int cw, int ch)
{
	int cr, cg, cb, cy;
	int i, j, k;
	
	for(i=0; i<ch; i++)
		for(j=0; j<cw; j++)
	{
		k=(yo+i)*xs+xo+j;
		cy=cell[i*cw+j];
		
		cr=cy;
		cg=cy;
		cb=cy;

		ibuf[k*4+0]=cr;
		ibuf[k*4+1]=cg;
		ibuf[k*4+2]=cb;
		ibuf[k*4+3]=255;
	}
}

void gensdf_putcell2(byte *ibuf, int xs, int ys,
	byte *cell_r, byte *cell_g, byte *cell_b,
	int xo, int yo, int cw, int ch)
{
	int cr, cg, cb, cy;
	int i, j, k;
	
	for(i=0; i<ch; i++)
		for(j=0; j<cw; j++)
	{
		k=(yo+i)*xs+xo+j;
		cr=cell_r[i*cw+j];
		cg=cell_g[i*cw+j];
		cb=cell_b[i*cw+j];

		ibuf[k*4+0]=cr;
		ibuf[k*4+1]=cg;
		ibuf[k*4+2]=cb;
		ibuf[k*4+3]=255;
	}
}

void gensdf_putcell3b(byte *ibuf, int xs, int ys,
	byte *cell_x, byte *cell_y,
	int xo, int yo, int cw, int ch)
{
	int cr, cg, cb, cy;
	int i, j, k;
	
	for(i=0; i<ch; i++)
		for(j=0; j<cw; j++)
	{
		k=(yo+i)*xs+xo+j;
		cr=cell_x[i*cw+j];
		cg=cell_y[i*cw+j];

		cr=(cr-128)*1.55+128;
		cg=(cg-128)*1.55+128;
		if(cr<0) cr=0;
		if(cg<0) cg=0;
		if(cr>255) cr=255;
		if(cg>255) cg=255;

		cr=cr>>4;
		cg=cg>>4;
		
//		cr=(cr+7)>>4;
//		cg=(cg+7)>>4;
//		if(cr>15)
//			cr=15;
//		if(cg>15)
//			cg=15;

		ibuf[k]=(cg<<4)|cr;
	}
}

void gensdf_interp4(byte *ibuf, int xs, int ys,
	byte *clr, int x, int y)
{
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int cr3, cg3, cb3;
	int cr4, cg4, cb4;
	int cr5, cg5, cb5;
	int cr, cg, cb;
	int x0, y0, xf, yf, x1, y1;
	
	x0=x>>4;	y0=y>>4;
	xf=x&15;	yf=y&15;
	x1=x0+1;	y1=y0+1;
	if(x1>=xs)x1=xs-1;
	if(y1>=ys)y1=ys-1;
	
	cr0=ibuf[(y0*xs+x0)*4+0];	cg0=ibuf[(y0*xs+x0)*4+1];
	cb0=ibuf[(y0*xs+x0)*4+2];	cr1=ibuf[(y0*xs+x1)*4+0];
	cg1=ibuf[(y0*xs+x1)*4+1];	cb1=ibuf[(y0*xs+x1)*4+2];
	cr2=ibuf[(y1*xs+x0)*4+0];	cg2=ibuf[(y1*xs+x0)*4+1];
	cb2=ibuf[(y1*xs+x0)*4+2];	cr3=ibuf[(y1*xs+x1)*4+0];
	cg3=ibuf[(y1*xs+x1)*4+1];	cb3=ibuf[(y1*xs+x1)*4+2];
	
	cr4=(cr0*(16-xf))+(cr1*xf); cg4=(cg0*(16-xf))+(cg1*xf);
	cb4=(cb0*(16-xf))+(cb1*xf);	cr5=(cr2*(16-xf))+(cr3*xf);
	cg5=(cg2*(16-xf))+(cg3*xf);	cb5=(cb2*(16-xf))+(cb3*xf);
	cr=(cr4*(16-yf))+(cr5*yf);	cg=(cg4*(16-yf))+(cg5*yf);
	cb=(cb4*(16-yf))+(cb5*yf);
	
//	cr=cr>>8;
//	cg=cg>>8;
//	cb=cb>>8;

	cr=(cr+127)>>8;
	cg=(cg+127)>>8;
	cb=(cb+127)>>8;
	
	if(cr<0)cr=0;
	if(cg<0)cg=0;
	if(cb<0)cb=0;

	if(cr>255)cr=255;
	if(cg>255)cg=255;
	if(cb>255)cb=255;
	
	clr[0]=cr;
	clr[1]=cg;
	clr[2]=cb;
}

int median(int r, int g, int b) {
    return max(min(r, g), min(max(r, g), b));
}

int gensdf_clrmedian(byte *clr)
{
	int c0, c1, c2, c3;
	
	c0=clr[0];	c1=clr[1];	c2=clr[2];

	return(median(c0, c1, c2));

#if 0
	if(c1<c0)
		{ c3=c0; c0=c1; c1=c3; }
	if(c2<c0)
		{ c3=c0; c0=c2; c2=c3; }
	if(c2<c1)
		{ c3=c1; c1=c2; c2=c3; }
	return(c1);
#endif
}

void cmp_rmse(byte *ibuf0, byte *ibuf1, int xs, int ys)
{
	double er, eg, eb;
	int dr, dg, db;
	int i, j, k, n;
	
	n=xs*ys; er=0; eg=0; eb=0;
	for(i=0; i<n; i++)
	{
		dr=ibuf0[i*4+0]-ibuf1[i*4+0];
		dg=ibuf0[i*4+1]-ibuf1[i*4+1];
		db=ibuf0[i*4+2]-ibuf1[i*4+2];
		er=er+dr*dr;
		eg=eg+dg*dg;
		eb=eb+db*db;
	}
	
	printf("RMSE: r=%.3f g=%.3f b=%.3f avg=%.3f\n",
		sqrt(er/n),
		sqrt(eg/n),
		sqrt(eb/n),
		sqrt((er+eg+eb)/(3*n)));
}

int main(int argc, char *argv[])
{
	byte cbuf[64*64];
	byte cbuf2[16*16];
	byte cbuf2x[16*16];
	byte cbuf2y[16*16];
	byte pal[768];
	byte clr[4];
	byte *ibuf;
	byte *obuf;
	byte *obuf2;
	int xs, ys, xs1, ys1;
	int cr, cg, cy;
	int i, j, k;

	ibuf=BGBBTJ_TGA_LoadRaw("fixed_64px.tga", &xs, &ys);
	
	xs1=xs/4; ys1=ys/4;
	obuf=malloc(xs1*ys1*4);
	obuf2=malloc(xs*ys*4);
	
	for(i=0; i<16; i++)
		for(j=0; j<16; j++)
	{
		printf("%2dx%2d\r", i, j);
		gensdf_getcell(ibuf, xs, ys, cbuf, j*64, i*64, 64, 64);
		gensdf_sdfcell(cbuf, cbuf2, 64, 64, 0);
		gensdf_sdfcell(cbuf, cbuf2x, 64, 64, 1);
		gensdf_sdfcell(cbuf, cbuf2y, 64, 64, 2);
//		gensdf_putcell(obuf, xs1, ys1, cbuf2, j*16, i*16, 16, 16);
//		gensdf_putcell2(obuf, xs1, ys1,
//			cbuf2x, cbuf2, cbuf2y,
//			j*16, i*16, 16, 16);
		gensdf_putcell3b(
			obuf, xs1, ys1,
			cbuf2x, cbuf2y,
			j*16, i*16, 16, 16);
	
	}
	
	for(i=0; i<256; i++)
	{
		cr=(i&15)*17;
		cg=(i>>4)*17;
		cy=(cr+cg)>>1;
		pal[i*3+0]=cr;
		pal[i*3+1]=cg;
		pal[i*3+2]=cy;
	}
	
//	BGBBTJ_TGA_StoreRaw("fixsdf0.tga", obuf, xs1, ys1);
	BGBBTJ_BMP8_StoreRaw("fixsdf1.bmp", obuf, xs1, ys1, pal);

	for(i=0; i<xs1*ys1; i++)
	{
		obuf2[i*4+0]=pal[obuf[i]*3+0];
		obuf2[i*4+1]=pal[obuf[i]*3+2];
		obuf2[i*4+2]=pal[obuf[i]*3+1];
		obuf2[i*4+3]=255;
	}
	memcpy(obuf, obuf2, xs1*ys1*4);

#if 1
	for(i=0; i<ys; i++)
		for(j=0; j<xs; j++)
	{
		gensdf_interp4(obuf, xs1, ys1, clr, j<<2, i<<2);
		k=gensdf_clrmedian(clr);
//		k=clr[1];

		cy=(k>=128)?255:0;
		
		k=(i*xs+j)*4;
		obuf2[k+0]=cy;
		obuf2[k+1]=cy;
		obuf2[k+2]=cy;
		obuf2[k+3]=255;
	}

	BGBBTJ_TGA_StoreRaw("fixsdf1_rgn0.tga", obuf2, xs, ys);
#endif
	
	cmp_rmse(ibuf, obuf2, xs, ys);
}
