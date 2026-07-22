typedef struct BGBCC_BITMAPINFOHEADER_s BGBCC_BITMAPINFOHEADER;
typedef struct BGBCC_BITMAPV4HEADER_s BGBCC_BITMAPV4HEADER;
typedef struct BGBCC_BITMAPV5HEADER_s BGBCC_BITMAPV5HEADER;

struct BGBCC_BITMAPINFOHEADER_s {
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

struct BGBCC_BITMAPV4HEADER_s {
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

	u32	biRedMask;
	u32	biGreenMask;
	u32	biBlueMask;
	u32	biAlphaMask;
	u32	biCSType;
//	CIEXYZTRIPLE bV4Endpoints;

	u32	biCieRedX;
	u32	biCieRedY;
	u32	biCieRedZ;
	u32	biCieGreenX;
	u32	biCieGreenY;
	u32	biCieGreenZ;
	u32	biCieBlueX;
	u32	biCieBlueY;
	u32	biCieBlueZ;

	u32	biGammaRed;
	u32	biGammaGreen;
	u32	biGammaBlue;
};

struct BGBCC_BITMAPV5HEADER_s {
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

	u32	biRedMask;
	u32	biGreenMask;
	u32	biBlueMask;
	u32	biAlphaMask;
	u32	biCSType;
//	CIEXYZTRIPLE bV4Endpoints;

	u32	biCieRedX;
	u32	biCieRedY;
	u32	biCieRedZ;
	u32	biCieGreenX;
	u32	biCieGreenY;
	u32	biCieGreenZ;
	u32	biCieBlueX;
	u32	biCieBlueY;
	u32	biCieBlueZ;

	u32	biGammaRed;
	u32	biGammaGreen;
	u32	biGammaBlue;

	u32 biIntent;
	u32 biProfileData;
	u32 biProfileSize;
	u32 biReserved;
};

u32 gfxedit_getu32(byte *ptr)
{
	return(*(u32 *)ptr);
}

void gfxedit_setu32(byte *ptr, u32 val)
{
	*(u32 *)ptr=val;
}

#if 0
byte *BGBCC_Img_DecodeBMP32(byte *imgbuf, int *rw, int *rh)
{
	BGBCC_BITMAPINFOHEADER *bmi;
	byte t_pal[1024];
	byte *dat, *cs, *pal;
	byte *buf, *ct;
	int ofs_bmi;
	int ofs_dat;
	int xstr, bc, bc1, xs, ys, flip;
	int p0, p1, p2, p3;
	int cr, cg, cb, ca;
	int x, y;
	int i;

	ofs_bmi=0;

	if((imgbuf[0]=='B') && (imgbuf[1]=='M'))
	{
		ofs_bmi=0x0E;
		ofs_dat=gfxedit_getu32(imgbuf+0x0A);
	}

	if(	(imgbuf[0]==' ') && (imgbuf[1]=='B') &&
		(imgbuf[2]=='M') && (imgbuf[3]=='P') )
	{
		ofs_bmi=0x10;
		ofs_dat=gfxedit_getu32(imgbuf+0x0C);
	}
	
	if(!ofs_bmi)
		return(NULL);
	
	bmi=(BGBCC_BITMAPINFOHEADER *)(imgbuf+ofs_bmi);
	dat=(byte *)(imgbuf+ofs_dat);
	pal=(byte *)(imgbuf+ofs_bmi+bmi->biSize);
	
	if(	(bmi->biCompression!=0) &&
		(bmi->biCompression!=3) &&
		(bmi->biCompression!=4) &&
		(bmi->biCompression!=BGBCC_FMT_CRAM) &&
		(bmi->biCompression!=BGBCC_FMT_CQL0) &&
		(bmi->biCompression!=BGBCC_FMT_bt5b))
		return(NULL);

	bc=bmi->biBitCount;
	bc1=bc;
	if(bc1==15)
		bc1=16;
	xstr=(((bmi->biWidth*bc1)+31)&(~31))>>3;
	
	if((bc==8) && (bmi->biClrUsed==0) && (bmi->biClrImportant==256))
	{
		pal=t_pal;
//		pal=bgbcc_dfl_pal256;
		for(i=0; i<256; i++)
		{
			pal[i*4+0]=bgbcc_dfl_pal256[i*4+2];
			pal[i*4+1]=bgbcc_dfl_pal256[i*4+1];
			pal[i*4+2]=bgbcc_dfl_pal256[i*4+0];
			pal[i*4+3]=bgbcc_dfl_pal256[i*4+3];
		}
	}
	
	if((bc==4) && (bmi->biClrUsed==0) && (bmi->biClrImportant==16))
	{
		pal=t_pal;
//		pal=bgbcc_dfl_pal16;
		for(i=0; i<16; i++)
		{
			pal[i*4+0]=bgbcc_dfl_pal16[i*4+2];
			pal[i*4+1]=bgbcc_dfl_pal16[i*4+1];
			pal[i*4+2]=bgbcc_dfl_pal16[i*4+0];
			pal[i*4+3]=bgbcc_dfl_pal16[i*4+3];
		}
	}

	xs=bmi->biWidth;
	ys=bmi->biHeight;
	flip=0;
	
	if(ys<0)
	{
		ys=-ys;
		flip=!flip;
	}
	
	buf=bgbcc_malloc_bmp(xs*ys*4);
	
	if(bmi->biCompression==BGBCC_FMT_CRAM)
	{
		BGBCC_Img_DecodeBMP_CRAM8(buf, dat, xs, ys, pal);
	}
	
	if(bmi->biCompression==BGBCC_FMT_CQL0)
	{
		BGBCC_Img_DecodeBMP_CQ8(buf, dat, xs, ys, pal);
	}
	
	if(bmi->biCompression==BGBCC_FMT_bt5b)
	{
		BT5BU_DecodeFrameRGBA(dat, 1<<20, buf, xs, ys);
	}
	
	if(	(bmi->biCompression==0) ||
		(bmi->biCompression==3) ||
		(bmi->biCompression==4))
	{
		for(y=0; y<ys; y++)
		{
			ct=buf+(y*xs*4);
			cs=dat+(y*xstr);
			if(flip)
				cs=buf+((ys-y-1)*xstr);
			for(x=0; x<xs; x++)
			{
				if(bc==32)
				{
					ct[0]=cs[2];
					ct[1]=cs[1];
					ct[2]=cs[0];
					ct[3]=cs[3];
					cs+=4; ct+=4;
				}else
					if(bc==24)
				{
					ct[0]=cs[2];
					ct[1]=cs[1];
					ct[2]=cs[0];
					ct[3]=255;
					cs+=3; ct+=4;
				}else
	#if 0
					if(bc==16)
				{
					p0=cs[0]|(cs[1]<<8);
					cr=(p0>>11)&31;
					cg=(p0>> 5)&63;
					cb=(p0>> 0)&31;
					cr=(cr<<3)|(cr>>2);
					cg=(cg<<2)|(cg>>4);
					cb=(cb<<3)|(cb>>2);
					ct[0]=cr;
					ct[1]=cg;
					ct[2]=cb;
					ct[3]=255;
					cs+=2; ct+=4;
				}else
	#endif
					if((bc==16) || (bc==15))
				{
					p0=cs[0]|(cs[1]<<8);
					cr=(p0>>10)&31;
					cg=(p0>> 5)&31;
					cb=(p0>> 0)&31;
					cr=(cr<<3)|(cr>>2);
					cg=(cg<<3)|(cg>>2);
					cb=(cb<<3)|(cb>>2);
					ca=255;
					if(p0&0x8000)
					{
						cr=(p0>>11)&15;
						cg=(p0>> 6)&15;
						cb=(p0>> 1)&15;
						cr=(cr<<4)|cr;
						cg=(cg<<4)|cg;
						cb=(cb<<4)|cb;
						ca=0x00;
						if(p0&0x0400)	ca|=0x80;
						if(p0&0x0020)	ca|=0x40;
						if(p0&0x0001)	ca|=0x20;
					}
					ct[0]=cr;	ct[1]=cg;
					ct[2]=cb;	ct[3]=ca;
					cs+=2;		ct+=4;
				}else
					if(bc==8)
				{
					p0=*cs;
					cr=pal[p0*4+2];		cg=pal[p0*4+1];
					cb=pal[p0*4+0];		ca=pal[p0*4+3];
					ct[0]=cr;			ct[1]=cg;
					ct[2]=cb;			ct[3]=ca;
					cs++;				ct+=4;
				}else
					if(bc==4)
				{
					p0=*cs;
					if(x&1)
						p0<<=4;
					p0=(p0>>4)&15;
					cr=pal[p0*4+2];		cg=pal[p0*4+1];
					cb=pal[p0*4+0];		ca=pal[p0*4+3];
					ct[0]=cr;			ct[1]=cg;
					ct[2]=cb;			ct[3]=ca;
					if(x&1)
						cs++;
					ct+=4;
				}else
					if(bc==2)
				{
					p0=*cs;
					if(x&2)		p0<<=4;
					if(x&1)		p0<<=2;
					p0=(p0>>6)&3;
					cr=pal[p0*4+2];		cg=pal[p0*4+1];
					cb=pal[p0*4+0];		ca=pal[p0*4+3];
					ct[0]=cr;			ct[1]=cg;
					ct[2]=cb;			ct[3]=ca;
					if((x&3)==3)
						cs++;
					ct+=4;
				}else
					if(bc==1)
				{
					p0=*cs;
					if(x&4)		p0<<=4;
					if(x&2)		p0<<=2;
					if(x&1)		p0<<=1;
					p0=(p0>>7)&1;
					cr=pal[p0*4+2];		cg=pal[p0*4+1];
					cb=pal[p0*4+0];		ca=pal[p0*4+3];
					ct[0]=cr;			ct[1]=cg;
					ct[2]=cb;			ct[3]=ca;
					if((x&7)==7)
						cs++;
					ct+=4;
				}
			}
		}
	}
	
	if(rw)
		*rw=xs;
	if(rh)
		*rh=ys;

	return(buf);
}
#endif

void GfxEdit_BmpFillPalGradient(byte *rpal)
{
	int cr, cg, cb, lo, hi, mi, lo1, lo2;
	int i, j, k;
	
	for(i=0; i<16; i++)
	{
		for(j=0; j<16; j++)
		{
			hi=(0xFF*(j+1))>>4;
			mi=(0xAA*(j+1))>>4;
			lo1=(0x33*(j+1))>>4;
			lo2=(0x55*(j+1))>>4;
			lo=lo1;
			if(i&8)
				lo=mi;
			
			cb=(i&1)?hi:lo;
			cg=(i&2)?hi:lo;
			cr=(i&4)?hi:lo;
			
			if(i==0)
				{ cr=hi; cg=hi; cb=hi; }
			if(i==7)
				{ cr=hi; cg=mi; cb=lo2; }
			if(i==8)
				{ cr=lo2; cg=mi; cb=hi; }
			if(i==15)
				{ cr=mi; cg=hi; cb=lo2; }

			k=i*16+j;
			rpal[k*4+0]=cb;
			rpal[k*4+1]=cg;
			rpal[k*4+2]=cr;
			rpal[k*4+3]=255;
		}
		
		hi=0xAA; lo=0x00;
		if(i&8)
			{ hi=0xFF; lo=0x55; }
		cr=(i&4)?hi:lo;
		cg=(i&2)?hi:lo;
		cb=(i&1)?hi:lo;

		k=i*16+0;
		rpal[k*4+0]=cb;
		rpal[k*4+1]=cg;
		rpal[k*4+2]=cr;
		rpal[k*4+3]=255;
	}

	for(i=0; i<15; i++)
	{
//		j=11+(i%5);
//		j=7+(i%5)*2;
		j=19+(i%5)*3;
		k=i/5;

		hi=(0xFF*(j+1))>>5;
		mi=(0xDD*(j+1))>>5;
		
		switch(k)
		{
		case 0:		cr=mi; cg=hi; cb=hi; break;
		case 1:		cr=hi; cg=mi; cb=hi; break;
		case 2:		cr=hi; cg=hi; cb=mi; break;
		}
	
		k=(i+1)*16+1;
		rpal[k*4+0]=cb;
		rpal[k*4+1]=cg;
		rpal[k*4+2]=cr;
		rpal[k*4+3]=255;
	}

	k=0xF0;
	rpal[k*4+3]=0;
}

void GfxEdit_BmpFillPalLookup555(byte *tlut, byte *rpal)
{
	u32 *pal;
	u32 cref;
	int bi, bd, bi2, bd2, d;
	int i, j, k;
	
	pal=(u32 *)rpal;
	
	for(i=0; i<65536; i++)
	{
		cref=GfxEdit_RGB555to32(i);
		bi=0; bi2=0; bd=99999999; bd2=bd;
		for(j=0; j<256; j++)
		{
			d=GfxEdit_ColorDistanceRgb32(cref, pal[j]);
			if(d<bd)
				{ bi2=bi; bd2=bd; bi=j; bd=d; }
			else if(d<bd2)
				{ bi2=j; bd2=d; }
		}
		tlut[i]=bi;
		
		if(bd2>(bd*1.5))
			bi2=bi;
		
		tlut[65536+i]=bi2;
	}
}

static byte gfxedit_t_pal[1024];
static byte *gfxedit_t_lut_rgb555;

byte *GfxEdit_DecodeBMP8(byte *imgbuf, u32 *rpal, int *rw, int *rh, int flag)
{
	BGBCC_BITMAPINFOHEADER *bmi;
	byte *dat, *cs, *pal;
	byte *buf, *ct;
	int ofs_bmi;
	int ofs_dat;
	int xstr, bc, bc1, xs, ys, flip, nclr;
	int p0, p1, p2, p3;
	int cr, cg, cb, ca;
	int x, y;
	int i;

	ofs_bmi=0;

	if((imgbuf[0]=='B') && (imgbuf[1]=='M'))
	{
		ofs_bmi=0x0E;
		ofs_dat=gfxedit_getu32(imgbuf+0x0A);
	}

	if(	(imgbuf[0]==' ') && (imgbuf[1]=='B') &&
		(imgbuf[2]=='M') && (imgbuf[3]=='P') )
	{
		ofs_bmi=0x10;
		ofs_dat=gfxedit_getu32(imgbuf+0x0C);
	}
	
	if(!ofs_bmi)
		return(NULL);
	
	bmi=(BGBCC_BITMAPINFOHEADER *)(imgbuf+ofs_bmi);
	dat=(byte *)(imgbuf+ofs_dat);
	pal=(byte *)(imgbuf+ofs_bmi+bmi->biSize);
	
	if(	(bmi->biCompression!=0) &&
		(bmi->biCompression!=3) &&
		(bmi->biCompression!=4) )
			return(NULL);

	bc=bmi->biBitCount;
	bc1=bc;
	if(bc1==15)
		bc1=16;
	xstr=(((bmi->biWidth*bc1)+31)&(~31))>>3;

	if(bc1>8)
	{
		pal=gfxedit_t_pal;
		if(!gfxedit_t_lut_rgb555)
		{
			gfxedit_t_lut_rgb555=malloc(65536*2);
			GfxEdit_BmpFillPalGradient(gfxedit_t_pal);
			GfxEdit_BmpFillPalLookup555(gfxedit_t_lut_rgb555, pal);
		}
//		return(NULL);
	}

	xs=bmi->biWidth;
	ys=bmi->biHeight;
	flip=0;
	
	if(ys<0)
	{
		ys=-ys;
		flip=!flip;
	}
	
	if(flag&8)
		flip=!flip;
	
	buf=malloc(xs*ys);
	
	if(	(bmi->biCompression==0) ||
		(bmi->biCompression==3) ||
		(bmi->biCompression==4))
	{
		for(y=0; y<ys; y++)
		{
			ct=buf+(y*xs);
			cs=dat+(y*xstr);
			if(flip)
				cs=dat+((ys-y-1)*xstr);
			for(x=0; x<xs; x++)
			{
				if(bc==32)
				{
					p0=cs[0]|(cs[1]<<8)|(cs[2]<<16)|(cs[3]<<24);
					p0=GfxEdit_RGBA32to555(p0);
					if((x^y)&1)
						p0|=0x10000;
					p0=gfxedit_t_lut_rgb555[p0];
					*ct=p0;
					cs+=4;		ct++;
				}else
					if(bc==24)
				{
					p0=cs[0]|(cs[1]<<8)|(cs[2]<<16);
					p0=GfxEdit_RGB24to555(p0);
					if((x^y)&1)
						p0|=0x10000;
					p0=gfxedit_t_lut_rgb555[p0];
					*ct=p0;
					cs+=3;		ct++;
				}else
					if((bc==16) || (bc==15))
				{
					p0=cs[0]|(cs[1]<<8);
					if((x^y)&1)
						p0|=0x10000;
					p0=gfxedit_t_lut_rgb555[p0];
					*ct=p0;
					cs+=2;		ct++;
				}else
					if(bc==8)
				{
					p0=*cs;		*ct=p0;
					cs++;		ct++;
				}else
					if(bc==4)
				{
					p0=*cs;
					if(x&1)
						p0<<=4;
					p0=(p0>>4)&15;
					*ct=p0;
					if(x&1)
						cs++;
					ct++;
				}else
					if(bc==2)
				{
					p0=*cs;
					if(x&2)		p0<<=4;
					if(x&1)		p0<<=2;
					p0=(p0>>6)&3;
					*ct=p0;
					if((x&3)==3)
						cs++;
					ct++;
				}else
					if(bc==1)
				{
					p0=*cs;
					if(x&4)		p0<<=4;
					if(x&2)		p0<<=2;
					if(x&1)		p0<<=1;
					p0=(p0>>7)&1;
					*ct=p0;
					if((x&7)==7)
						cs++;
					ct++;
				}
			}
		}
	}
	
	if(rw)
		*rw=xs;
	if(rh)
		*rh=ys;

	if(bc<=8)
	{
		nclr=bmi->biClrUsed;
		if(!nclr || (nclr>(1<<bc)))
			nclr=1<<bc;
		
		memset(rpal, 0, 256*4);
//		memcpy(rpal, pal, (1<<bc)*4);
		memcpy(rpal, pal, nclr*4);
	}
	else
		{ memcpy(rpal, pal, 256*4); }

	return(buf);
}

int GfxEdit_EncodeImageBMP8I(byte *obuf, byte *ibuf,
	int xs, int ys, u32 *pal, int fl)
{
	BGBCC_BITMAPINFOHEADER *bmi;
	byte *bdat, *bpal, *cs, *ct;
	int ofs_bmi;
	int ofs_pal;
	int ofs_dat;
	int cr, cg, cb, ca, ci, cj, ck, aki;
	int xstr, sz, nclr, ncbpp;
	int x, y;
	int i;

	xstr=(xs+3)&(~3);
//	sz=ofs_dat+ys*xstr;

	for(nclr=256; (nclr>=1) && !pal[nclr-1]; nclr--);
	ncbpp=gfxedit_log2up(nclr);
	nclr=1<<ncbpp;
	if(nclr>256)
		nclr=256;

	if(fl&1)
	{
		ofs_bmi=0x0010;
		ofs_pal=0x0038;
//		ofs_dat=0x0440;
		ofs_dat=ofs_pal+(nclr*4);
		ofs_dat=(ofs_dat+15)&(~15);
		
		sz=ofs_dat+ys*xstr;

		memcpy(obuf, " BMP", 4);
		gfxedit_setu32(obuf+0x04, sz);
		gfxedit_setu32(obuf+0x08, 0);
		gfxedit_setu32(obuf+0x0C, ofs_dat);
		bmi=(BGBCC_BITMAPINFOHEADER *)(obuf+0x10);
	}else
	{
		ofs_bmi=0x000E;
		ofs_pal=0x0036;
//		ofs_dat=0x0440;
		ofs_dat=ofs_pal+(nclr*4);
		ofs_dat=(ofs_dat+15)&(~15);

		sz=ofs_dat+ys*xstr;

		memcpy(obuf, "BM", 2);
		gfxedit_setu32(obuf+0x02, sz);
		gfxedit_setu32(obuf+0x06, 0);
		gfxedit_setu32(obuf+0x0A, ofs_dat);
		bmi=(BGBCC_BITMAPINFOHEADER *)(obuf+0x0E);
	}
	
	bmi->biSize=40;
	bmi->biWidth=xs;
	bmi->biHeight=ys;
	bmi->biPlanes=1;
	bmi->biBitCount=8;
	bmi->biCompression=0;
	bmi->biSizeImage=ys*xstr;
	bmi->biXPelsPerMeter=2835;
	bmi->biYPelsPerMeter=2835;
	bmi->biClrUsed=nclr;
	bmi->biClrImportant=nclr;
	
	aki=0xFF;
	
	bpal=obuf+ofs_pal;
	bdat=obuf+ofs_dat;
	for(i=0; i<nclr; i++)
	{
		bpal[i*4+0]=(pal[i]>> 0)&255;
		bpal[i*4+1]=(pal[i]>> 8)&255;
		bpal[i*4+2]=(pal[i]>>16)&255;
		bpal[i*4+3]=(pal[i]>>24)&255;
	}
	
	for(y=0; y<ys; y++)
	{
		cs=ibuf+(y*xs);
		ct=bdat+y*xstr;
		if(fl&8)
			cs=ibuf+((ys-y-1)*xs);
		for(x=0; x<xs; x++)
		{
			cj=cs[x];
			ct[x]=cj;
		}
	}
	
	return(sz);
}


int GfxEdit_EncodeImageBMP4I(byte *obuf, byte *ibuf,
	int xs, int ys, u32 *pal, int fl)
{
	BGBCC_BITMAPINFOHEADER *bmi;
	byte *bdat, *bpal, *cs, *ct;
	int ofs_bmi;
	int ofs_pal;
	int ofs_dat;
	int cr, cg, cb, ca, ci, cj, ck;
	int xstr, sz, aki;
	int x, y;
	int i;

	xstr=((xs+7)&(~7))>>1;

	if(fl&1)
	{
		ofs_bmi=0x0010;
		ofs_pal=0x0038;
		ofs_dat=0x0080;
		sz=ofs_dat+ys*xstr;

		memcpy(obuf, " BMP", 4);
		gfxedit_setu32(obuf+0x04, sz);
		gfxedit_setu32(obuf+0x08, 0);
		gfxedit_setu32(obuf+0x0C, ofs_dat);
		bmi=(BGBCC_BITMAPINFOHEADER *)(obuf+0x10);
	}else
	{
		ofs_bmi=0x000E;
		ofs_pal=0x0036;
		ofs_dat=0x0080;
		sz=ofs_dat+ys*xstr;

		memcpy(obuf, "BM", 2);
		gfxedit_setu32(obuf+0x02, sz);
		gfxedit_setu32(obuf+0x06, 0);
		gfxedit_setu32(obuf+0x0A, ofs_dat);
		bmi=(BGBCC_BITMAPINFOHEADER *)(obuf+0x0E);
	}
	
	bmi->biSize=40;
	bmi->biWidth=xs;
	bmi->biHeight=ys;
	bmi->biPlanes=1;
	bmi->biBitCount=4;
	bmi->biCompression=0;
	bmi->biSizeImage=ys*xstr;
	bmi->biXPelsPerMeter=2835;
	bmi->biYPelsPerMeter=2835;
	bmi->biClrUsed=16;
	bmi->biClrImportant=16;
	
	bpal=obuf+ofs_pal;
	bdat=obuf+ofs_dat;
	for(i=0; i<16; i++)
	{
		bpal[i*4+0]=(pal[i]>> 0)&255;
		bpal[i*4+1]=(pal[i]>> 8)&255;
		bpal[i*4+2]=(pal[i]>>16)&255;
		bpal[i*4+3]=(pal[i]>>24)&255;
	}
	
	for(y=0; y<ys; y++)
	{
		cs=ibuf+(y*xs);
		ct=bdat+y*xstr;
		if(fl&8)
			cs=ibuf+((ys-y-1)*xs);
		for(x=0; x<xs; x++)
		{
			cj=cs[x];
			if(x&1)		{ ct[x>>1]=ct[x>>1]|cj; }
			else		{ ct[x>>1]=cj<<4; }
		}
	}
	
	return(sz);
}

int GfxEdit_EncodeImageBMP2I(byte *obuf, byte *ibuf,
	int xs, int ys, u32 *pal, int fl)
{
	BGBCC_BITMAPINFOHEADER *bmi;
	byte *bdat, *bpal, *cs, *ct;
	int ofs_bmi;
	int ofs_pal;
	int ofs_dat;
	int cr, cg, cb, ca, ci;
	int xstr, sz, aki;
	int x, y;
	int i;

	xstr=((xs+15)&(~15))>>2;

	if(fl&1)
	{
		ofs_bmi=0x0010;
		ofs_pal=0x0038;
		ofs_dat=0x0050;
		sz=ofs_dat+ys*xstr;

		memcpy(obuf, " BMP", 4);
		gfxedit_setu32(obuf+0x04, sz);
		gfxedit_setu32(obuf+0x08, 0);
		gfxedit_setu32(obuf+0x0C, ofs_dat);
		bmi=(BGBCC_BITMAPINFOHEADER *)(obuf+0x10);
	}else
	{
		ofs_bmi=0x000E;
		ofs_pal=0x0036;
		ofs_dat=0x0050;
		sz=ofs_dat+ys*xstr;

		memcpy(obuf, "BM", 2);
		gfxedit_setu32(obuf+0x02, sz);
		gfxedit_setu32(obuf+0x06, 0);
		gfxedit_setu32(obuf+0x0A, ofs_dat);
		bmi=(BGBCC_BITMAPINFOHEADER *)(obuf+0x0E);
	}
	
	bmi->biSize=40;
	bmi->biWidth=xs;
	bmi->biHeight=ys;
	bmi->biPlanes=1;
	bmi->biBitCount=2;
	bmi->biCompression=0;
	bmi->biSizeImage=ys*xstr;
	bmi->biXPelsPerMeter=2835;
	bmi->biYPelsPerMeter=2835;
	bmi->biClrUsed=4;
	bmi->biClrImportant=4;
	
	aki=3;
	bpal=obuf+ofs_pal;
	bdat=obuf+ofs_dat;
	for(i=0; i<4; i++)
	{
		bpal[i*4+0]=(pal[i]>> 0)&255;
		bpal[i*4+1]=(pal[i]>> 8)&255;
		bpal[i*4+2]=(pal[i]>>16)&255;
		bpal[i*4+3]=(pal[i]>>24)&255;
		if(pal[i*4+3]<16)
			aki=i;
	}
	
	for(y=0; y<ys; y++)
	{
		cs=ibuf+(y*xs);
		ct=bdat+y*xstr;
		if(fl&8)
			cs=ibuf+((ys-y-1)*xs);
		for(x=0; x<xs; x++)
		{
			ci=cs[x];
			if(x&3)		{ ct[x>>2]=ct[x>>2]|(ci<<((3-(x&3))*2)); }
			else		{ ct[x>>2]=ci<<6; }
		}
	}
	
	return(sz);
}

int GfxEdit_EncodeImageBMP1I(byte *obuf, byte *ibuf,
	int xs, int ys, u32 *pal, int fl)
{
	BGBCC_BITMAPINFOHEADER *bmi;
	byte *bdat, *bpal, *cs, *ct;
	int ofs_bmi;
	int ofs_pal;
	int ofs_dat;
	int cr, cg, cb, ca, ci;
	int xstr, sz, aki;
	int x, y;
	int i;

	xstr=((xs+31)&(~31))>>3;

	if(fl&1)
	{
		ofs_bmi=0x0010;
		ofs_pal=0x0038;
		ofs_dat=0x0050;
		sz=ofs_dat+ys*xstr;

		memcpy(obuf, " BMP", 4);
		gfxedit_setu32(obuf+0x04, sz);
		gfxedit_setu32(obuf+0x08, 0);
		gfxedit_setu32(obuf+0x0C, ofs_dat);
		bmi=(BGBCC_BITMAPINFOHEADER *)(obuf+0x10);
	}else
	{
		ofs_bmi=0x000E;
		ofs_pal=0x0036;
		ofs_dat=0x0050;
		sz=ofs_dat+ys*xstr;

		memcpy(obuf, "BM", 2);
		gfxedit_setu32(obuf+0x02, sz);
		gfxedit_setu32(obuf+0x06, 0);
		gfxedit_setu32(obuf+0x0A, ofs_dat);
		bmi=(BGBCC_BITMAPINFOHEADER *)(obuf+0x0E);
	}
	
	bmi->biSize=40;
	bmi->biWidth=xs;
	bmi->biHeight=ys;
	bmi->biPlanes=1;
	bmi->biBitCount=1;
	bmi->biCompression=0;
	bmi->biSizeImage=ys*xstr;
	bmi->biXPelsPerMeter=2835;
	bmi->biYPelsPerMeter=2835;
	bmi->biClrUsed=4;
	bmi->biClrImportant=4;
	
	bpal=obuf+ofs_pal;
	bdat=obuf+ofs_dat;
	for(i=0; i<2; i++)
	{
		bpal[i*4+0]=(pal[i]>> 0)&255;
		bpal[i*4+1]=(pal[i]>> 8)&255;
		bpal[i*4+2]=(pal[i]>>16)&255;
		bpal[i*4+3]=(pal[i]>>24)&255;
	}
	
	for(y=0; y<ys; y++)
	{
		cs=ibuf+(y*xs);
		ct=bdat+y*xstr;
		if(fl&8)
			cs=ibuf+((ys-y-1)*xs);
		for(x=0; x<xs; x++)
		{
			ci=cs[x];
			if(x&7)		{ ct[x>>3]=ct[x>>3]|(ci<<(7-(x&7))); }
			else		{ ct[x>>3]=ci<<7; }
		}
	}
	
	return(sz);
}
