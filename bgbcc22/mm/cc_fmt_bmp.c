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

byte *BGBBTJ_BufPNG_Decode(
	byte *csbuf, int cssz, int *w, int *h);

byte bgbcc_dfl_pal256[1024];
byte bgbcc_dfl_pal16[64];

int BGBCC_Img_DecodeBMP_CRAM8(byte *imgbuf,
	byte *imgdat, int xs, int ys, byte *pal)
{
	u32 clra, clrb;
	byte *cs;
	u32 *ct;
	int px, ca, cb;
	int bxs, bys;
	int x, y;
	
	bxs=(xs+3)>>2;
	bys=(ys+3)>>2;
	
	cs=imgdat;
	for(y=0; y<bys; y++)
	{
		ct=((u32 *)imgbuf)+(y*4*xs);
		for(x=0; x<bxs; x++)
		{
			px=cs[0]|(cs[1]<<8);
			ca=cs[2];
			cb=cs[3];
			
			if(!(px&0x8000))
			{
				cs+=4;
			}else
				if((px&0xFC00)==0x8000)
			{
				ca=px&255;
				cb=ca;
				cs+=2;
			}else
			{
			}
			
			clra=	(pal[ca*4+2]<< 0) |
					(pal[ca*4+1]<< 8) |
					(pal[ca*4+0]<<16) |
					(pal[ca*4+3]<<24) ;
			clrb=	(pal[cb*4+2]<< 0) |
					(pal[cb*4+1]<< 8) |
					(pal[cb*4+0]<<16) |
					(pal[cb*4+3]<<24) ;

			ct[(0*xs)+0]=(px&0x0001)?clra:clrb;
			ct[(0*xs)+1]=(px&0x0002)?clra:clrb;
			ct[(0*xs)+2]=(px&0x0004)?clra:clrb;
			ct[(0*xs)+3]=(px&0x0008)?clra:clrb;
			ct[(1*xs)+0]=(px&0x0010)?clra:clrb;
			ct[(1*xs)+1]=(px&0x0020)?clra:clrb;
			ct[(1*xs)+2]=(px&0x0040)?clra:clrb;
			ct[(1*xs)+3]=(px&0x0080)?clra:clrb;
			ct[(2*xs)+0]=(px&0x0100)?clra:clrb;
			ct[(2*xs)+1]=(px&0x0200)?clra:clrb;
			ct[(2*xs)+2]=(px&0x0400)?clra:clrb;
			ct[(2*xs)+3]=(px&0x0800)?clra:clrb;
			ct[(3*xs)+0]=(px&0x1000)?clra:clrb;
			ct[(3*xs)+1]=(px&0x2000)?clra:clrb;
			ct[(3*xs)+2]=(px&0x4000)?clra:clrb;
			ct[(3*xs)+3]=(px&0x8000)?clra:clrb;
			
			ct+=4;
		}
	}

	return(0);
}

int BGBCC_Img_DecodeBMP_CQ8(byte *imgbuf,
	byte *imgdat, int xs, int ys, byte *pal)
{
	u32 clra, clrb;
	byte *cs;
	u32 *ct, *ct1;
	u64 px;
	int ca, cb;
	int bxs, bys;
	int x, y;
	int i, j, k;
	
	bxs=(xs+7)>>3;
	bys=(ys+7)>>3;
	
	cs=imgdat;
	for(y=0; y<bys; y++)
	{
		ct=((u32 *)imgbuf)+(y*8*xs);
		for(x=0; x<bxs; x++)
		{
			px=cs[0]|(cs[1]<<8);
			ca=cs[2];
			cb=cs[3];
			
			if(!(px&0x8000))
			{
				clra=	(cs[0]<< 0)|(cs[1]<< 8) |
						(cs[2]<<16)|(cs[3]<<24) ;
				clrb=	(cs[4]<< 0)|(cs[5]<< 8) |
						(cs[6]<<16)|(cs[7]<<24) ;
				px=((u64)clra) | (((u64)clrb)<<32);
				ca=cs[8];
				cb=cs[9];
				cs+=10;
			}else
				if((px&0xFC00)==0x8000)
			{
				ca=px&255;
				cb=ca;
				cs+=2;
			}else
				if((px&0xFC00)>=0x9000)
			{
				if(px&0x0800)
				{
					clra=	(cs[0]<< 0)|(cs[1]<< 8) |
							(cs[2]<<16)|(cs[3]<<24) ;
					clrb=	(cs[4]<< 0)|(cs[5]<< 8) |
							(cs[6]<<16)|(cs[7]<<24) ;
					px=((u64)clra) | (((u64)clrb)<<32);
					ca=cs[8];
					cb=cs[9];
					cs+=16;
				}else
				{
					cs+=4;
				}
			}
			
			clra=	(pal[ca*4+2]<< 0) |
					(pal[ca*4+1]<< 8) |
					(pal[ca*4+0]<<16) |
					(pal[ca*4+3]<<24) ;
			clrb=	(pal[cb*4+2]<< 0) |
					(pal[cb*4+1]<< 8) |
					(pal[cb*4+0]<<16) |
					(pal[cb*4+3]<<24) ;

			ct1=ct;
			for(i=0; i<8; i++)
			{
//				j=(7-i)*8;
				j=i*8;
				k=(px>>j);
				ct1[0]=(k&0x80)?clra:clrb;
				ct1[1]=(k&0x40)?clra:clrb;
				ct1[2]=(k&0x20)?clra:clrb;
				ct1[3]=(k&0x10)?clra:clrb;
				ct1[4]=(k&0x08)?clra:clrb;
				ct1[5]=(k&0x04)?clra:clrb;
				ct1[6]=(k&0x02)?clra:clrb;
				ct1[7]=(k&0x01)?clra:clrb;
				ct1+=xs;
			}
			
			ct+=8;
		}
	}

	return(0);
}

byte *BGBCC_Img_DecodeBMP(byte *imgbuf, int *rw, int *rh)
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
		ofs_dat=exwad_getu32(imgbuf+0x0A);
	}

	if(	(imgbuf[0]==' ') && (imgbuf[1]=='B') &&
		(imgbuf[2]=='M') && (imgbuf[3]=='P') )
	{
		ofs_bmi=0x10;
		ofs_dat=exwad_getu32(imgbuf+0x0C);
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
		(bmi->biCompression!=BGBCC_FMT_CQL0))
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
	
	buf=bgbcc_malloc(xs*ys*4);
	
	if(bmi->biCompression==BGBCC_FMT_CRAM)
	{
		BGBCC_Img_DecodeBMP_CRAM8(buf, dat, xs, ys, pal);
	}
	
	if(bmi->biCompression==BGBCC_FMT_CQL0)
	{
		BGBCC_Img_DecodeBMP_CQ8(buf, dat, xs, ys, pal);
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


byte *BGBCC_Img_DecodeImage(byte *imgbuf, int *rw, int *rh)
{
	if(rw)
		*rw=0;
	if(rh)
		*rh=0;

	if(!imgbuf)
		return(NULL);

	if((imgbuf[0]=='B') && (imgbuf[1]=='M'))
	{
		return(BGBCC_Img_DecodeBMP(imgbuf, rw, rh));
	}

	if(	(imgbuf[0]==' ') && (imgbuf[1]=='B') &&
		(imgbuf[2]=='M') && (imgbuf[3]=='P') )
	{
		return(BGBCC_Img_DecodeBMP(imgbuf, rw, rh));
	}
	
	if(BGBCC_Img_DecodeProbeIsTGA(imgbuf, rw, rh))
	{
		return(BGBCC_Img_DecodeTGA(imgbuf, rw, rh));
	}

	if((imgbuf[0]=='q') && (imgbuf[1]=='o'))
	{
		return(QOILZ_DecImageBuffer(imgbuf, rw, rh));
	}
	
	if(	(imgbuf[0]=='l') && (imgbuf[1]=='c') &&
		(imgbuf[2]=='i') && (imgbuf[3]=='f'))
	{
		return(LCIF_DecImageTempBuffer(imgbuf, rw, rh));
	}
	
	if(	(imgbuf[0]=='u') && (imgbuf[1]=='p') &&
		(imgbuf[2]=='h') && (imgbuf[3]=='d'))
	{
		return(TKuPI_DecodeImageTempBuffer(imgbuf, rw, rh));
	}
	
	if(	(imgbuf[0]==0x89) && (imgbuf[1]=='P') &&
		(imgbuf[2]=='N') && (imgbuf[3]=='G'))
	{
		return(BGBBTJ_BufPNG_Decode(imgbuf, 1<<20, rw, rh));
	}
	
	return(NULL);
}

byte *bgbcc_img_bmppallookup;
byte *bgbcc_img_bmppallookupb;

static int bgbcc_img_bmppalhash;
static byte *bgbcc_img_bmppalmlookup;
static byte *bgbcc_img_bmppalmlookupb;
static int bgbcc_img_bmppalmhash[4];
static byte bgbcc_img_bmppalaki;
static byte bgbcc_img_bmppalmaki[4];

int BGBCC_Img_EncodeImageBmpSetupPal(byte *pal, int nclr)
{
	int cr0, cg0, cb0, cy0;
	int cr1, cg1, cb1, cy1;
	int dr, dg, db, dy, d, d0, d1, d2, d3;
	int bi, bd, aki, bi2, bd2;
	int i, j, k, h;

#if 0
	if(!bgbcc_img_bmppallookup)
	{
		bgbcc_img_bmppallookup=malloc(32768);
		bgbcc_img_bmppalhash=-1;
	}
#endif
	
#if 1
	if(!bgbcc_img_bmppalmlookup)
	{
		bgbcc_img_bmppalmlookup =malloc(4*32768);
		bgbcc_img_bmppalmlookupb=malloc(4*32768);
		for(i=0; i<4; i++)
			bgbcc_img_bmppalmhash[i]=-1;
	}
#endif
	
	h=1;
	for(i=0; i<(nclr*4); i++)
		h=h*251+pal[i];
	h=((h*251+1)>>8)&65535;

	bgbcc_img_bmppallookup =bgbcc_img_bmppalmlookup+((h&3)<<15);
	bgbcc_img_bmppallookupb=bgbcc_img_bmppalmlookupb+((h&3)<<15);
	bgbcc_img_bmppalhash=bgbcc_img_bmppalmhash[h&3];
	bgbcc_img_bmppalaki=bgbcc_img_bmppalmaki[h&3];

	if(bgbcc_img_bmppalhash==h)
		return(0);

	aki=0xFF;
	for(i=0; i<nclr; i++)
	{
		j=pal[i*4+3];
		if(j<16)
			aki=i;
	}
	
	bgbcc_img_bmppalhash=h;
	bgbcc_img_bmppalaki=h;
	bgbcc_img_bmppalmhash[h&3]=h;
	bgbcc_img_bmppalmaki[h&3]=aki;

	for(i=0; i<32768; i++)
	{
		cr0=(i>>10)&31;
		cg0=(i>> 5)&31;
		cb0=(i>> 0)&31;
		cr0=(cr0<<3)|(cr0>>2);
		cg0=(cg0<<3)|(cg0>>2);
		cb0=(cb0<<3)|(cb0>>2);
		cy0=(8*cg0+5*cr0+3*cb0)/16;
		
		bi=-1;
		bi2=-1;
		bd=999999999;
		bd2=999999999;
		
		for(j=0; j<nclr; j++)
		{
			cr1=pal[j*4+0];
			cg1=pal[j*4+1];
			cb1=pal[j*4+2];
			cy1=(8*cg1+5*cr1+3*cb1)/16;

			if(pal[j*4+3]<128)
				continue;

			dr=cr1-cr0;
			dg=cg1-cg0;
			db=cb1-cb0;
			dy=cy1-cy0;
//			d=(dr*dr)+(2*dg*dg)+(db*db);
//			d=(dr*dr)+(2*dg*dg)+(db*db)+(2*dy*dy);
			d=(dr*dr)+(dg*dg)+(db*db)+(2*dy*dy);

#if 0
			dr=dr^(dr>>31);
			dg=dg^(dg>>31);
			db=db^(db>>31);
			dy=dy^(dy>>31);
//			d=dr+dg+db;

			d0=dr;
			d1=dg;
			d2=db;
//			if(d1>d0) { k=d0; d0=d1; d1=k; }
//			if(d2>d1) { k=d1; d1=d2; d2=k; }
//			if(d1>d0) { k=d0; d0=d1; d1=k; }
//			d=d0+(d1>>1)+(d2>>2);

			d3=dy;
			if(d1>d0) { k=d0; d0=d1; d1=k; }
			if(d2>d1) { k=d1; d1=d2; d2=k; }
			if(d3>d2) { k=d2; d2=d3; d3=k; }
			if(d1>d0) { k=d0; d0=d1; d1=k; }
			if(d2>d1) { k=d1; d1=d2; d2=k; }
			if(d1>d0) { k=d0; d0=d1; d1=k; }
			d=d0+(d1>>1)+(d2>>2)+(d3>>3);
#endif

			if(d<bd)
			{
				if(bi2<0)
					{ bi2=j; bd2=d; }
				else
					{ bi2=bi; bd2=bd; }
				bi=j; bd=d;
			}else if((d<bd2) && (bi!=j))
			{
				bi2=j; bd2=d;
			}
		}
		bgbcc_img_bmppallookup[i]=bi;
		bgbcc_img_bmppallookupb[i]=bi2;
	}
	return(1);
}

int BGBCC_Img_EncodeImageBMP8I(byte *obuf, byte *ibuf,
	int xs, int ys, byte *pal, int fl)
{
	BGBCC_BITMAPINFOHEADER *bmi;
	byte *bdat, *bpal, *cs, *ct;
	int ofs_bmi;
	int ofs_pal;
	int ofs_dat;
	int cr, cg, cb, ca, ci, aki;
	int xstr, sz;
	int x, y;
	int i;

	xstr=(xs+3)&(~3);
//	sz=ofs_dat+ys*xstr;

	if(fl&1)
	{
		ofs_bmi=0x0010;
		ofs_pal=0x0038;
		ofs_dat=0x0440;
		sz=ofs_dat+ys*xstr;

		memcpy(obuf, " BMP", 4);
		exwad_setu32(obuf+0x04, sz);
		exwad_setu32(obuf+0x08, 0);
		exwad_setu32(obuf+0x0C, ofs_dat);
		bmi=(BGBCC_BITMAPINFOHEADER *)(obuf+0x10);
	}else
	{
		ofs_bmi=0x000E;
		ofs_pal=0x0036;
		ofs_dat=0x0440;
		sz=ofs_dat+ys*xstr;

		memcpy(obuf, "BM", 2);
		exwad_setu32(obuf+0x02, sz);
		exwad_setu32(obuf+0x06, 0);
		exwad_setu32(obuf+0x0A, ofs_dat);
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
	bmi->biClrUsed=256;
	bmi->biClrImportant=256;
	
	BGBCC_Img_EncodeImageBmpSetupPal(pal, 256);

	aki=0xFF;
	
	bpal=obuf+ofs_pal;
	bdat=obuf+ofs_dat;
	for(i=0; i<256; i++)
	{
		bpal[i*4+0]=pal[i*4+2];
		bpal[i*4+1]=pal[i*4+1];
		bpal[i*4+2]=pal[i*4+0];
		bpal[i*4+3]=pal[i*4+3];
		
		if(pal[i*4+3]<16)
			aki=i;
	}
	
	for(y=0; y<ys; y++)
	{
		cs=ibuf+(y*xs*4);
		ct=bdat+y*xstr;
		for(x=0; x<xs; x++)
		{
			cr=cs[x*4+0];
			cg=cs[x*4+1];
			cb=cs[x*4+2];
			ca=cs[x*4+3];
			cr=(cr>>3);
			cg=(cg>>3);
			cb=(cb>>3);
			ci=(cr<<10)|(cg<<5)|cb;
			ci=bgbcc_img_bmppallookup[ci];
			if(ca<128)
				ci=aki;
			ct[x]=ci;
		}
	}
	
	return(sz);
}

int BGBCC_Img_EncodeImageBMP8(byte *obuf, byte *ibuf,
	int xs, int ys, byte *pal)
{
	return(BGBCC_Img_EncodeImageBMP8I(obuf, ibuf, xs, ys, pal, 0));
}

int BGBCC_Img_EncodeImageBMP8A(byte *obuf, byte *ibuf,
	int xs, int ys, byte *pal)
{
	return(BGBCC_Img_EncodeImageBMP8I(obuf, ibuf, xs, ys, pal, 1));
}

int BGBCC_Img_EncodeImageBMP4I(byte *obuf, byte *ibuf,
	int xs, int ys, byte *pal, int fl)
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

	xstr=((xs+7)&(~7))>>1;

	if(fl&1)
	{
		ofs_bmi=0x0010;
		ofs_pal=0x0038;
		ofs_dat=0x0080;
		sz=ofs_dat+ys*xstr;

		memcpy(obuf, " BMP", 4);
		exwad_setu32(obuf+0x04, sz);
		exwad_setu32(obuf+0x08, 0);
		exwad_setu32(obuf+0x0C, ofs_dat);
		bmi=(BGBCC_BITMAPINFOHEADER *)(obuf+0x10);
	}else
	{
		ofs_bmi=0x000E;
		ofs_pal=0x0036;
		ofs_dat=0x0080;
		sz=ofs_dat+ys*xstr;

		memcpy(obuf, "BM", 2);
		exwad_setu32(obuf+0x02, sz);
		exwad_setu32(obuf+0x06, 0);
		exwad_setu32(obuf+0x0A, ofs_dat);
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
	
	BGBCC_Img_EncodeImageBmpSetupPal(pal, 16);
	
	aki=13;
	bpal=obuf+ofs_pal;
	bdat=obuf+ofs_dat;
	for(i=0; i<16; i++)
	{
		bpal[i*4+0]=pal[i*4+2];
		bpal[i*4+1]=pal[i*4+1];
		bpal[i*4+2]=pal[i*4+0];
		bpal[i*4+3]=pal[i*4+3];
		if(pal[i*4+3]<16)
			aki=i;
	}
	
	for(y=0; y<ys; y++)
	{
		cs=ibuf+(y*xs*4);
		ct=bdat+y*xstr;
		for(x=0; x<xs; x++)
		{
			cr=cs[x*4+0];	cg=cs[x*4+1];	cb=cs[x*4+2];
			cr=(cr>>3);		cg=(cg>>3);		cb=(cb>>3);
			ca=cs[x*4+3];
			ci=(cr<<10)|(cg<<5)|cb;
			ci=bgbcc_img_bmppallookup[ci];
			if(ca<128)
				ci=aki;
			if(x&1)
				{ ct[x>>1]=ct[x>>1]|ci; }
			else
				{ ct[x>>1]=ci<<4; }
		}
	}
	
	return(sz);
}

int BGBCC_Img_EncodeImageBMP4(byte *obuf, byte *ibuf,
	int xs, int ys, byte *pal)
{
	return(BGBCC_Img_EncodeImageBMP4I(obuf, ibuf, xs, ys, pal, 0));
}

int BGBCC_Img_EncodeImageBMP4A(byte *obuf, byte *ibuf,
	int xs, int ys, byte *pal)
{
	return(BGBCC_Img_EncodeImageBMP4I(obuf, ibuf, xs, ys, pal, 1));
}

int BGBCC_Img_EncodeImageBMP2I(byte *obuf, byte *ibuf,
	int xs, int ys, byte *pal, int fl)
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
//	sz=ofs_dat+ys*xstr;

	if(fl&1)
	{
		ofs_bmi=0x0010;
		ofs_pal=0x0038;
		ofs_dat=0x0050;
		sz=ofs_dat+ys*xstr;

		memcpy(obuf, " BMP", 4);
		exwad_setu32(obuf+0x04, sz);
		exwad_setu32(obuf+0x08, 0);
		exwad_setu32(obuf+0x0C, ofs_dat);
		bmi=(BGBCC_BITMAPINFOHEADER *)(obuf+0x10);
	}else
	{
		ofs_bmi=0x000E;
		ofs_pal=0x0036;
		ofs_dat=0x0050;
		sz=ofs_dat+ys*xstr;

		memcpy(obuf, "BM", 2);
		exwad_setu32(obuf+0x02, sz);
		exwad_setu32(obuf+0x06, 0);
		exwad_setu32(obuf+0x0A, ofs_dat);
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
	
	BGBCC_Img_EncodeImageBmpSetupPal(pal, 4);
	
	aki=3;
	bpal=obuf+ofs_pal;
	bdat=obuf+ofs_dat;
	for(i=0; i<4; i++)
	{
		bpal[i*4+0]=pal[i*4+2];
		bpal[i*4+1]=pal[i*4+1];
		bpal[i*4+2]=pal[i*4+0];
		bpal[i*4+3]=pal[i*4+3];
		if(pal[i*4+3]<16)
			aki=i;
	}
	
	for(y=0; y<ys; y++)
	{
		cs=ibuf+(y*xs*4);
		ct=bdat+y*xstr;
		for(x=0; x<xs; x++)
		{
			cr=cs[x*4+0];	cg=cs[x*4+1];	cb=cs[x*4+2];
			cr=(cr>>3);		cg=(cg>>3);		cb=(cb>>3);
			ca=cs[x*4+3];
			ci=(cr<<10)|(cg<<5)|cb;
			ci=bgbcc_img_bmppallookup[ci];
			if(ca<128)
				ci=aki;
			if(x&3)
				{ ct[x>>2]=ct[x>>2]|(ci<<((3-(x&3))*2)); }
			else
				{ ct[x>>2]=ci<<6; }
		}
	}
	
	return(sz);
}

int BGBCC_Img_EncodeImageBMP2(byte *obuf, byte *ibuf,
	int xs, int ys, byte *pal)
{
	return(BGBCC_Img_EncodeImageBMP2I(obuf, ibuf, xs, ys, pal, 0));
}

int BGBCC_Img_EncodeImageBMP2A(byte *obuf, byte *ibuf,
	int xs, int ys, byte *pal)
{
	return(BGBCC_Img_EncodeImageBMP2I(obuf, ibuf, xs, ys, pal, 1));
}

int BGBCC_Img_EncodeImageBMP1I(byte *obuf, byte *ibuf,
	int xs, int ys, byte *pal, int fl)
{
	BGBCC_BITMAPINFOHEADER *bmi;
	byte *bdat, *bpal, *cs, *ct;
	int ofs_bmi;
	int ofs_pal;
	int ofs_dat;
	int cr, cg, cb, ci;
	int xstr, sz;
	int x, y;
	int i;

	xstr=((xs+15)&(~15))>>2;
//	sz=ofs_dat+ys*xstr;

	if(fl&1)
	{
		ofs_bmi=0x0010;
		ofs_pal=0x0038;
		ofs_dat=0x0040;
		sz=ofs_dat+ys*xstr;

		memcpy(obuf, " BMP", 4);
		exwad_setu32(obuf+0x04, sz);
		exwad_setu32(obuf+0x08, 0);
		exwad_setu32(obuf+0x0C, ofs_dat);
		bmi=(BGBCC_BITMAPINFOHEADER *)(obuf+0x10);
	}else
	{
		ofs_bmi=0x000E;
		ofs_pal=0x0036;
		ofs_dat=0x0040;
		sz=ofs_dat+ys*xstr;

		memcpy(obuf, "BM", 2);
		exwad_setu32(obuf+0x02, sz);
		exwad_setu32(obuf+0x06, 0);
		exwad_setu32(obuf+0x0A, ofs_dat);
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
	bmi->biClrUsed=2;
	bmi->biClrImportant=2;
	
	BGBCC_Img_EncodeImageBmpSetupPal(pal, 2);
	
	bpal=obuf+ofs_pal;
	bdat=obuf+ofs_dat;
	for(i=0; i<2; i++)
	{
		bpal[i*4+0]=pal[i*4+2];
		bpal[i*4+1]=pal[i*4+1];
		bpal[i*4+2]=pal[i*4+0];
		bpal[i*4+3]=pal[i*4+3];
	}
	
	for(y=0; y<ys; y++)
	{
		cs=ibuf+(y*xs*4);
		ct=bdat+y*xstr;
		for(x=0; x<xs; x++)
		{
			cr=cs[x*4+0];	cg=cs[x*4+1];	cb=cs[x*4+2];
			cr=(cr>>3);		cg=(cg>>3);		cb=(cb>>3);
			ci=(cr<<10)|(cg<<5)|cb;
			ci=bgbcc_img_bmppallookup[ci];
			if(x&7)
				{ ct[x>>3]=ct[x>>3]|(ci<<(7-(x&7))); }
			else
				{ ct[x>>3]=ci<<7; }
		}
	}
	
	return(sz);
}

int BGBCC_Img_EncodeImageBMP1(byte *obuf, byte *ibuf,
	int xs, int ys, byte *pal)
{
	return(BGBCC_Img_EncodeImageBMP1I(obuf, ibuf, xs, ys, pal, 0));
}

int BGBCC_Img_EncodeImageBMP1A(byte *obuf, byte *ibuf,
	int xs, int ys, byte *pal)
{
	return(BGBCC_Img_EncodeImageBMP1I(obuf, ibuf, xs, ys, pal, 1));
}



int BGBCC_Img_EncodeImageBMP16I(byte *obuf, byte *ibuf,
	int xs, int ys, int fl)
{
//	BGBCC_BITMAPINFOHEADER *bmi;
	BGBCC_BITMAPV4HEADER *bmi;
	byte *bdat, *cs, *ct;
	u16 *bfi;
	int ofs_bmi;
	int ofs_dat;
	int cr, cg, cb, ca, ci;
	int xstr, sz;
	int x, y;
	int i;

	xstr=((2*xs)+3)&(~3);
//	sz=ofs_dat+ys*xstr;

	if(fl&1)
	{
		ofs_bmi=0x0010;
//		ofs_dat=0x0038;
		ofs_dat=0x0080;
		sz=ofs_dat+ys*xstr;

		memcpy(obuf, " BMP", 4);
		exwad_setu32(obuf+0x04, sz);
		exwad_setu32(obuf+0x08, 0);
		exwad_setu32(obuf+0x0C, ofs_dat);
//		bmi=(BGBCC_BITMAPINFOHEADER *)(obuf+0x10);
		bmi=(BGBCC_BITMAPV4HEADER *)(obuf+0x10);
	}else
	{
		ofs_bmi=0x000E;
//		ofs_dat=0x0038;
		ofs_dat=0x0080;
		sz=ofs_dat+ys*xstr;

		memcpy(obuf, "BM", 2);
		exwad_setu32(obuf+0x02, sz);
		exwad_setu32(obuf+0x06, 0);
		exwad_setu32(obuf+0x0A, ofs_dat);
//		bmi=(BGBCC_BITMAPINFOHEADER *)(obuf+0x0E);
		bmi=(BGBCC_BITMAPV4HEADER *)(obuf+0x0E);
	}
	
	memset(bmi, 0, 108);
	
//	bmi->biSize=40;
	bmi->biSize=108;
	bmi->biWidth=xs;
	bmi->biHeight=ys;
	bmi->biPlanes=1;
	bmi->biBitCount=16;
	bmi->biCompression=3;
//	bmi->biCompression=6;
	bmi->biSizeImage=ys*xstr;
	bmi->biXPelsPerMeter=2835;
	bmi->biYPelsPerMeter=2835;
	bmi->biClrUsed=0;
	bmi->biClrImportant=0;

#if 1
	bmi->biRedMask		=0x7C00;
	bmi->biGreenMask	=0x03E0;
	bmi->biBlueMask		=0x001F;
	bmi->biAlphaMask	=0x8000;
//	bmi->biCSType		=0x206E6957;
	bmi->biCSType		=0x57696E20;
#endif

//	bfi=(u16 *)(((byte *)bmi)+40);
//	bfi[0]=0x8000;
//	bfi[1]=0x001F;
//	bfi[2]=0x03E0;
//	bfi[3]=0x7C00;

	bdat=obuf+ofs_dat;	
	for(y=0; y<ys; y++)
	{
		cs=ibuf+(y*xs*4);
		ct=bdat+y*xstr;
		for(x=0; x<xs; x++)
		{
			cr=cs[x*4+0];
			cg=cs[x*4+1];
			cb=cs[x*4+2];
			ca=cs[x*4+3];
			cr=(cr>>3);
			cg=(cg>>3);
			cb=(cb>>3);

//			cr=((cr+4)>>3);
//			cg=((cg+4)>>3);
//			cb=((cb+4)>>3);
//			if(cr>31)	cr=31;
//			if(cg>31)	cg=31;
//			if(cb>31)	cb=31;
			ci=(cr<<10)|(cg<<5)|cb;
			if(ca<0x240)
			{
				ci&=0x7BDE;
				ci|=0x8000;
				if(ca&0x80)		ci|=0x0400;
				if(ca&0x40)		ci|=0x0020;
				if(ca&0x20)		ci|=0x0001;
			}
			ct[x*2+0]=(ci>>0);
			ct[x*2+1]=(ci>>8);
		}
	}
	
	return(sz);
}

int BGBCC_Img_EncodeImageBMP16(byte *obuf, byte *ibuf,
	int xs, int ys)
{
	return(BGBCC_Img_EncodeImageBMP16I(obuf, ibuf, xs, ys, 0));
}

int BGBCC_Img_EncodeImageBMP16A(byte *obuf, byte *ibuf,
	int xs, int ys)
{
	return(BGBCC_Img_EncodeImageBMP16I(obuf, ibuf, xs, ys, 1));
}

int BGBCC_Img_EncodeImageBMP32I(byte *obuf, byte *ibuf,
	int xs, int ys, int fl)
{
//	BGBCC_BITMAPINFOHEADER *bmi;
//	BGBCC_BITMAPV4HEADER *bmi;
	BGBCC_BITMAPV5HEADER *bmi;
	byte *bdat, *cs, *ct;
	u16 *bfi;
	int ofs_bmi;
	int ofs_dat;
	int cr, cg, cb, ca, ci;
	int xstr, sz;
	int x, y;
	int i;

//	xstr=((4*xs)+3)&(~3);
	xstr=xs*4;
//	sz=ofs_dat+ys*xstr;

	if(fl&1)
	{
		ofs_bmi=0x0010;
//		ofs_dat=0x0038;
		ofs_dat=0x0090;
		sz=ofs_dat+ys*xstr;

		memcpy(obuf, " BMP", 4);
		exwad_setu32(obuf+0x04, sz);
		exwad_setu32(obuf+0x08, 0);
		exwad_setu32(obuf+0x0C, ofs_dat);
//		bmi=(BGBCC_BITMAPINFOHEADER *)(obuf+0x10);
//		bmi=(BGBCC_BITMAPV4HEADER *)(obuf+0x10);
		bmi=(BGBCC_BITMAPV5HEADER *)(obuf+0x10);
	}else
	{
		ofs_bmi=0x000E;
//		ofs_dat=0x0038;
		ofs_dat=0x0090;
		sz=ofs_dat+ys*xstr;

		memcpy(obuf, "BM", 2);
		exwad_setu32(obuf+0x02, sz);
		exwad_setu32(obuf+0x06, 0);
		exwad_setu32(obuf+0x0A, ofs_dat);
//		bmi=(BGBCC_BITMAPINFOHEADER *)(obuf+0x0E);
//		bmi=(BGBCC_BITMAPV4HEADER *)(obuf+0x0E);
		bmi=(BGBCC_BITMAPV5HEADER *)(obuf+0x0E);
	}
	
	memset(bmi, 0, 124);
	
//	bmi->biSize=40;
	bmi->biSize=108;
//	bmi->biSize=124;
	bmi->biWidth=xs;
	bmi->biHeight=ys;
	bmi->biPlanes=1;
	bmi->biBitCount=32;
	bmi->biCompression=3;
//	bmi->biCompression=6;
	bmi->biSizeImage=ys*xstr;
	bmi->biXPelsPerMeter=2835;
	bmi->biYPelsPerMeter=2835;
	bmi->biClrUsed=0;
	bmi->biClrImportant=0;

#if 1
	bmi->biRedMask		=0x00FF0000;
	bmi->biGreenMask	=0x0000FF00;
	bmi->biBlueMask		=0x000000FF;
	bmi->biAlphaMask	=0xFF000000;
	bmi->biCSType		=0x57696E20;
#endif

	bmi->biIntent		=0;

	bdat=obuf+ofs_dat;	
	for(y=0; y<ys; y++)
	{
		cs=ibuf+(y*xs*4);
		ct=bdat+y*xstr;
		for(x=0; x<xs; x++)
		{
			cr=cs[x*4+0];	cg=cs[x*4+1];
			cb=cs[x*4+2];	ca=cs[x*4+3];
			ct[x*4+0]=cb;	ct[x*4+1]=cg;
			ct[x*4+2]=cr;	ct[x*4+3]=ca;
		}
	}
	
	return(sz);
}

int BGBCC_Img_EncodeImageBMP32(byte *obuf, byte *ibuf,
	int xs, int ys)
{
	return(BGBCC_Img_EncodeImageBMP32I(obuf, ibuf, xs, ys, 0));
}

int BGBCC_Img_EncodeImageBMP32A(byte *obuf, byte *ibuf,
	int xs, int ys)
{
	return(BGBCC_Img_EncodeImageBMP32I(obuf, ibuf, xs, ys, 1));
}


byte *BGBCC_Img_EncodeImageBMP_CellCRAM8I(byte *ct, byte *ibuf, int xstr)
{
	byte cya[16], caa[16];
	byte *cs;
	int cr, cg, cb, cy, ca, cu, cv;
	int mcr, mcg, mcb, mcy, mca, nca;
	int ncr, ncg, ncb, ncy, mcn, ncn;
	int acy, clra, clrb, px;
	int x, y, bal;
	int i, j, k;
	
	mcr=256; mcg=256; mcb=256;
	ncr=0; ncg=0; ncb=0;
	for(y=0; y<8; y++)
	{
		cs=ibuf+y*xstr;
		for(x=0; x<8; x++)
		{
			cr=cs[0];	cg=cs[1];	cb=cs[2];	ca=cs[3];
			cs+=4;
			cy=(8*cg+5*cr+3*cb)>>4;
			cu=(3*cg+5*cr+8*cb)>>4;
			cv=(5*cg+8*cr+3*cb)>>4;
			if(cy<mcg)	mcg=cy;
			if(cu<mcb)	mcb=cu;
			if(cv<mcr)	mcr=cv;
			if(cy>ncg)	ncg=cy;
			if(cu>ncb)	ncb=cu;
			if(cv>ncr)	ncr=cv;
		}
	}
	
	bal=0; k=ncg-mcg;
	j=ncr-mcr;		if(j>k)	{ bal=1; k=j; }
	j=ncb-mcb;		if(j>k)	{ bal=2; k=j; }
	
	mcr=0; mcg=0; mcb=0; mcy= 512; mca= 512;
	ncr=0; ncg=0; ncb=0; ncy=-512; nca=-512;
	for(y=0; y<4; y++)
	{
		cs=ibuf+y*xstr;
		for(x=0; x<4; x++)
		{
			cr=cs[0];	cg=cs[1];	cb=cs[2];	ca=cs[3];
			cs+=4;
			cy=(8*cg+5*cr+3*cb)>>4;
			cu=(3*cg+5*cr+8*cb)>>4;
			cv=(5*cg+8*cr+3*cb)>>4;
			if(bal==1)	cy=cu;
			if(bal==2)	cy=cv;

			cya[y*4+x]=cy;
			caa[y*4+x]=ca;
			
			if((cy<mcy) && (ca>=128))
				{ mcr=cr; mcg=cg; mcb=cb; mcy=cy; }
			if((cy>ncy) && (ca>=128))
				{ ncr=cr; ncg=cg; ncb=cb; ncy=cy; }
			if(ca<mca)
				{ mca=ca; }
			if(ca>nca)
				{ nca=ca; }
		}
	}
	acy=(mcy+ncy)>>1;

#if 1
	if((ncy-mcy)>4)
	{

		mcr=0; mcg=0; mcb=0; mcn=0;
		ncr=0; ncg=0; ncb=0; ncn=0;
		for(y=0; y<4; y++)
		{
			cs=ibuf+y*xstr;
			for(x=0; x<4; x++)
			{
				cr=cs[0];	cg=cs[1];	cb=cs[2];	ca=cs[3];
				cs+=4;
//				cy=(8*cg+5*cr+3*cb)>>4;
				cy=cya[y*4+x];
				
				if(cy<acy)
				{
					mcr+=cr;
					mcg+=cg;
					mcb+=cb;
					mcn++;
				}
				if(cy>acy)
				{
					ncr+=cr;
					ncg+=cg;
					ncb+=cb;
					ncn++;
				}
			}
		}
		mcr/=mcn;
		mcg/=mcn;
		mcb/=mcn;

		ncr/=ncn;
		ncg/=ncn;
		ncb/=ncn;
	}
#endif

	if(nca<=128)
	{
		/* Transparent */
		*ct++=bgbcc_img_bmppalaki;
		*ct++=0x80;
		return(ct);
	}

	cr=(ncr>>3);
	cg=(ncg>>3);
	cb=(ncb>>3);
	j=(cr<<10)|(cg<<5)|cb;
	clra=bgbcc_img_bmppallookup[j];

	cr=(mcr>>3);
	cg=(mcg>>3);
	cb=(mcb>>3);
	j=(cr<<10)|(cg<<5)|cb;
	clrb=bgbcc_img_bmppallookup[j];

	if(mca<128)
	{
		/* Alpha Mask Block */

		cr=(mcr+ncr)>>1;
		cg=(mcg+ncg)>>1;
		cb=(mcb+ncb)>>1;
		cr=(cr>>3);	cg=(cg>>3);	cb=(cb>>3);
		j=(cr<<10)|(cg<<5)|cb;
		clra=bgbcc_img_bmppallookup[j];
		
		clrb=bgbcc_img_bmppalaki;
		
		acy=128;
		
		for(i=0; i<16; i++)
			cya[i]=caa[i];
	}


	cu=(5*acy+3*mcy)>>3;
	cv=(5*acy+3*ncy)>>3;
	px=0;
	for(y=0; y<4; y++)
	{
		for(x=0; x<4; x++)
		{
			k=((x^y)&1)?cu:cv;
			cy=cya[y*4+x];
//			if(cy>acy)
			if(cy>k)
				px|=(1<<(y*4+x));
//				px|=(1<<(15-(y*4+x)));
		}
	}
	
	if(px&0x8000)
	{
		px=~px;
		k=clra; clra=clrb; clrb=k;
	}
	
	if(clra==clrb)
	{
		*ct++=clra;
		*ct++=0x80;
		return(ct);
	}
	
	*ct++=px;
	*ct++=px>>8;
	*ct++=clra;
	*ct++=clrb;
	return(ct);
}

int BGBCC_Img_EncodeImageBMP_CRAM8I(byte *obuf, byte *ibuf,
	int xs, int ys, byte *pal, int fl)
{
	BGBCC_BITMAPINFOHEADER *bmi;
	byte *bdat, *bpal, *cs, *ct;
	int ofs_bmi;
	int ofs_pal;
	int ofs_dat;
	int cr, cg, cb, ca, ci, aki;
	int xstr, sz, bxs, bys;
	int x, y;
	int i;

	xstr=(xs+3)&(~3);
//	sz=ofs_dat+ys*xstr;

	bxs=(xs+3)>>2;
	bys=(ys+3)>>2;

	if(fl&1)
	{
		ofs_bmi=0x0010;
		ofs_pal=0x0038;
		ofs_dat=0x0440;
		
		if(fl&2)
		{
			ofs_dat=0x0040;
		}
		
		sz=ofs_dat+ys*xstr;

		memcpy(obuf, " BMP", 4);
		exwad_setu32(obuf+0x04, sz);
		exwad_setu32(obuf+0x08, 0);
		exwad_setu32(obuf+0x0C, ofs_dat);
		bmi=(BGBCC_BITMAPINFOHEADER *)(obuf+0x10);
	}else
	{
		ofs_bmi=0x000E;
		ofs_pal=0x0036;
		ofs_dat=0x0440;

		if(fl&2)
		{
			ofs_dat=0x0040;
		}

		sz=ofs_dat+ys*xstr;

		memcpy(obuf, "BM", 2);
		exwad_setu32(obuf+0x02, sz);
		exwad_setu32(obuf+0x06, 0);
		exwad_setu32(obuf+0x0A, ofs_dat);
		bmi=(BGBCC_BITMAPINFOHEADER *)(obuf+0x0E);
	}
	
	bmi->biSize=40;
	bmi->biWidth=xs;
	bmi->biHeight=ys;
	bmi->biPlanes=1;
	bmi->biBitCount=8;
	bmi->biCompression=BGBCC_FMT_CRAM;
	bmi->biSizeImage=ys*xstr;
	bmi->biXPelsPerMeter=2835;
	bmi->biYPelsPerMeter=2835;
	bmi->biClrUsed=256;
	bmi->biClrImportant=256;

	if(fl&2)
	{
		bmi->biClrUsed=0;
	}

	BGBCC_Img_EncodeImageBmpSetupPal(pal, 256);

	aki=0xFF;
	
	bpal=obuf+ofs_pal;
	bdat=obuf+ofs_dat;
	if(!(fl&2))
	{
		for(i=0; i<256; i++)
		{
			bpal[i*4+0]=pal[i*4+2];
			bpal[i*4+1]=pal[i*4+1];
			bpal[i*4+2]=pal[i*4+0];
			bpal[i*4+3]=pal[i*4+3];
			
			if(pal[i*4+3]<16)
				aki=i;
		}
	}
	
	ct=bdat;
	for(y=0; y<bys; y++)
	{
		cs=ibuf+((y*4)*xs*4);
		for(x=0; x<bxs; x++)
		{
			ct=BGBCC_Img_EncodeImageBMP_CellCRAM8I(ct, cs, xs*4);
			cs+=4*4;
		}
	}

	bmi->biSizeImage=ct-bdat;
	
	sz=ct-obuf;
	
	if(fl&1)
		{ exwad_setu32(obuf+0x04, sz); }
	else
		{ exwad_setu32(obuf+0x02, sz); }

	return(sz);
}

int BGBCC_Img_EncodeImageBMP_CRAM8(byte *obuf, byte *ibuf,
	int xs, int ys, byte *pal)
{
	return(BGBCC_Img_EncodeImageBMP_CRAM8I(obuf, ibuf, xs, ys, pal, 0));
}

int BGBCC_Img_EncodeImageBMP_CRAM8A(byte *obuf, byte *ibuf,
	int xs, int ys, byte *pal)
{
	return(BGBCC_Img_EncodeImageBMP_CRAM8I(obuf, ibuf, xs, ys, pal, 1));
}

int BGBCC_Img_EncodeImageBMP_CRAM8NPA(byte *obuf, byte *ibuf,
	int xs, int ys, byte *pal)
{
	return(BGBCC_Img_EncodeImageBMP_CRAM8I(obuf, ibuf, xs, ys, pal, 3));
}

#ifndef TGVLZ_NOMAIN
extern int bgbcc_packlz_cblksz;
#endif

int BGBCC_Img_EncodeImageBMP_LZ8I(byte *obuf, byte *ibuf,
	int xs, int ys, byte *pal, int fl)
{
#ifdef TGVLZ_NOMAIN
	TgvLz_Context *tectx;
#endif
	BGBCC_BITMAPINFOHEADER *bmi;
	byte *bdat, *bpal, *cs, *ct;
	byte *tbuf;
	int ofs_bmi;
	int ofs_pal;
	int ofs_dat;
	int cr, cg, cb, ca, ci, aki;
	int xstr, sz, csz;
	int x, y;
	int i;

	BGBCC_Img_EncodeImageBMP8I(obuf, ibuf, xs, ys, pal, fl);

	xstr=(xs+3)&(~3);
//	sz=ofs_dat+ys*xstr;

	if(fl&1)
	{
		ofs_bmi=0x0010;
		ofs_pal=0x0038;
		ofs_dat=0x0440;
		sz=ofs_dat+ys*xstr;

		memcpy(obuf, " BMP", 4);
		exwad_setu32(obuf+0x04, sz);
		exwad_setu32(obuf+0x08, 0);
		exwad_setu32(obuf+0x0C, ofs_dat);
		bmi=(BGBCC_BITMAPINFOHEADER *)(obuf+0x10);
	}else
	{
		ofs_bmi=0x000E;
		ofs_pal=0x0036;
		ofs_dat=0x0440;
		sz=ofs_dat+ys*xstr;

		memcpy(obuf, "BM", 2);
		exwad_setu32(obuf+0x02, sz);
		exwad_setu32(obuf+0x06, 0);
		exwad_setu32(obuf+0x0A, ofs_dat);
		bmi=(BGBCC_BITMAPINFOHEADER *)(obuf+0x0E);
	}

	bmi->biCompression=BGBCC_FMT_RGL3;
	
	tbuf=malloc(2*ys*xstr);

#ifdef TGVLZ_NOMAIN
	tectx=TgvLz_CreateContext();
	csz=TgvLz_DoEncodeSafe(tectx, tbuf, obuf+ofs_dat, ys*xstr);
	printf("BGBCC_Img_EncodeImageBMP_LZ8I: RP2 %d -> %d\n", ys*xstr, csz);
#else
	if(1)
//	if(0)
	{
		BGBCC_JX2C_PackBlockLZ_Reset(NULL);
		BGBCC_JX2C_PackBlockRP2(NULL,
			obuf+ofs_dat, tbuf, ys*xstr, 2*ys*xstr);
		csz=bgbcc_packlz_cblksz;
		printf("BGBCC_Img_EncodeImageBMP_LZ8I: RP2 %d -> %d\n", ys*xstr, csz);
	}else
	{
		BGBCC_JX2C_PackBlockLZ_Reset(NULL);
		BGBCC_JX2C_PackBlockLZ4(NULL,
			tbuf, obuf+ofs_dat, 2*ys*xstr, ys*xstr);
		csz=bgbcc_packlz_cblksz;
		printf("BGBCC_Img_EncodeImageBMP_LZ8I: LZ4 %d -> %d\n", ys*xstr, csz);
	}
#endif

	memcpy(obuf+ofs_dat, tbuf, csz);
	bmi->biSizeImage=csz;

	sz=ofs_dat+csz;
	
	if(fl&1)
		{ exwad_setu32(obuf+0x04, sz); }
	else
		{ exwad_setu32(obuf+0x02, sz); }

	return(sz);
}

int BGBCC_Img_EncodeImageBMP_LZ8(byte *obuf, byte *ibuf,
	int xs, int ys, byte *pal)
{
	return(BGBCC_Img_EncodeImageBMP_LZ8I(obuf, ibuf, xs, ys, pal, 0));
}

int BGBCC_Img_EncodeImageBMP_LZ8A(byte *obuf, byte *ibuf,
	int xs, int ys, byte *pal)
{
	return(BGBCC_Img_EncodeImageBMP_LZ8I(obuf, ibuf, xs, ys, pal, 1));
}




byte *BGBCC_Img_EncodeImageBMP_CellCQ8I(byte *ct, byte *ibuf, int xstr)
{
	byte cya[64], caa[64];
	byte *cs;
	int cr, cg, cb, cy, ca, cu, cv;
	int mcr, mcg, mcb, mcy, mca, nca;
	int ncr, ncg, ncb, ncy, mcn, ncn;
	int acy, clra, clrb;
	u64 px;
	int x, y, bal;
	int i, j, k;

	mcr=256; mcg=256; mcb=256;
	ncr=0; ncg=0; ncb=0;
	for(y=0; y<8; y++)
	{
		cs=ibuf+y*xstr;
		for(x=0; x<8; x++)
		{
			cr=cs[0];	cg=cs[1];	cb=cs[2];	ca=cs[3];
			cs+=4;
			cy=(8*cg+5*cr+3*cb)>>4;
			cu=(3*cg+5*cr+8*cb)>>4;
			cv=(5*cg+8*cr+3*cb)>>4;
			if(cy<mcg)	mcg=cy;
			if(cu<mcb)	mcb=cu;
			if(cv<mcr)	mcr=cv;
			if(cy>ncg)	ncg=cy;
			if(cu>ncb)	ncb=cu;
			if(cv>ncr)	ncr=cv;
		}
	}
	
	bal=0; k=ncg-mcg;
	j=ncr-mcr;		if(j>k)	{ bal=1; k=j; }
	j=ncb-mcb;		if(j>k)	{ bal=2; k=j; }
	
	mcr=0; mcg=0; mcb=0; mcy= 512; mca= 512;
	ncr=0; ncg=0; ncb=0; ncy=-512; nca=-512;
	for(y=0; y<8; y++)
	{
		cs=ibuf+y*xstr;
		for(x=0; x<8; x++)
		{
			cr=cs[0];	cg=cs[1];	cb=cs[2];	ca=cs[3];
			cs+=4;
			cy=(8*cg+5*cr+3*cb)>>4;
			cu=(3*cg+5*cr+8*cb)>>4;
			cv=(5*cg+8*cr+3*cb)>>4;
			if(bal==1)	cy=cu;
			if(bal==2)	cy=cv;

			cya[y*8+x]=cy;
			caa[y*8+x]=ca;
			
			if((cy<mcy) && (ca>=128))
				{ mcr=cr; mcg=cg; mcb=cb; mcy=cy; }
			if((cy>ncy) && (ca>=128))
				{ ncr=cr; ncg=cg; ncb=cb; ncy=cy; }
			if(ca<mca)
				{ mca=ca; }
			if(ca>nca)
				{ nca=ca; }
		}
	}
	acy=(mcy+ncy)>>1;

#if 1
	if((ncy-mcy)>4)
	{

		mcr=0; mcg=0; mcb=0; mcn=0;
		ncr=0; ncg=0; ncb=0; ncn=0;
		for(y=0; y<8; y++)
		{
			cs=ibuf+y*xstr;
			for(x=0; x<8; x++)
			{
				cr=cs[0];	cg=cs[1];	cb=cs[2];	ca=cs[3];
				cs+=4;
//				cy=(8*cg+5*cr+3*cb)>>4;
				cy=cya[y*8+x];
				
				if(cy<acy)
				{
					mcr+=cr;
					mcg+=cg;
					mcb+=cb;
					mcn++;
				}
				if(cy>acy)
				{
					ncr+=cr;
					ncg+=cg;
					ncb+=cb;
					ncn++;
				}
			}
		}
		mcr/=mcn;
		mcg/=mcn;
		mcb/=mcn;

		ncr/=ncn;
		ncg/=ncn;
		ncb/=ncn;
	}
#endif

	if(nca<=128)
	{
		/* Transparent */
		*ct++=bgbcc_img_bmppalaki;
		*ct++=0x80;
		return(ct);
	}

	cr=(ncr>>3);
	cg=(ncg>>3);
	cb=(ncb>>3);
	j=(cr<<10)|(cg<<5)|cb;
	clra=bgbcc_img_bmppallookup[j];

	cr=(mcr>>3);
	cg=(mcg>>3);
	cb=(mcb>>3);
	j=(cr<<10)|(cg<<5)|cb;
	clrb=bgbcc_img_bmppallookup[j];

	if(mca<128)
	{
		/* Alpha Mask Block */

		cr=(mcr+ncr)>>1;
		cg=(mcg+ncg)>>1;
		cb=(mcb+ncb)>>1;
		cr=(cr>>3);	cg=(cg>>3);	cb=(cb>>3);
		j=(cr<<10)|(cg<<5)|cb;
		clra=bgbcc_img_bmppallookup[j];
		
		clrb=bgbcc_img_bmppalaki;
		
		acy=128;
		
		for(i=0; i<64; i++)
			cya[i]=caa[i];
	}


	cu=(5*acy+3*mcy)>>3;
	cv=(5*acy+3*ncy)>>3;
	px=0;
	for(y=0; y<8; y++)
	{
		for(x=0; x<8; x++)
		{
			k=((x^y)&1)?cv:cu;
//			cy=cya[y*8+x];
			cy=cya[(7-y)*8+x];
//			if(cy>acy)
			if(cy>k)
//				px|=(1ULL<<(y*8+x));
				px|=(1ULL<<(63-(y*8+x)));
		}
	}
	
	if(px&0x8000)
	{
		px=~px;
		k=clra; clra=clrb; clrb=k;
	}
	
	if(clra==clrb)
	{
		*ct++=clra;
		*ct++=0x80;
		return(ct);
	}
	
	*ct++=px>> 0;
	*ct++=px>> 8;
	*ct++=px>>16;
	*ct++=px>>24;
	*ct++=px>>32;
	*ct++=px>>40;
	*ct++=px>>48;
	*ct++=px>>56;
	*ct++=clra;
	*ct++=clrb;
	return(ct);
}

int BGBCC_Img_EncodeImageBMP_CQ8I(byte *obuf, byte *ibuf,
	int xs, int ys, byte *pal, int fl)
{
	BGBCC_BITMAPINFOHEADER *bmi;
	byte *bdat, *bpal, *cs, *ct;
	int ofs_bmi;
	int ofs_pal;
	int ofs_dat;
	int cr, cg, cb, ca, ci, aki;
	int xstr, sz, bxs, bys;
	int x, y;
	int i;

	xstr=(xs+7)&(~7);
//	sz=ofs_dat+ys*xstr;

	bxs=(xs+7)>>3;
	bys=(ys+7)>>3;

	if(fl&1)
	{
		ofs_bmi=0x0010;
		ofs_pal=0x0038;
		ofs_dat=0x0440;

		if(fl&2)
		{
			ofs_dat=0x0040;
		}

		sz=ofs_dat+ys*xstr;

		memcpy(obuf, " BMP", 4);
		exwad_setu32(obuf+0x04, sz);
		exwad_setu32(obuf+0x08, 0);
		exwad_setu32(obuf+0x0C, ofs_dat);
		bmi=(BGBCC_BITMAPINFOHEADER *)(obuf+0x10);
	}else
	{
		ofs_bmi=0x000E;
		ofs_pal=0x0036;
		ofs_dat=0x0440;

		if(fl&2)
		{
			ofs_dat=0x0040;
		}

		sz=ofs_dat+ys*xstr;

		memcpy(obuf, "BM", 2);
		exwad_setu32(obuf+0x02, sz);
		exwad_setu32(obuf+0x06, 0);
		exwad_setu32(obuf+0x0A, ofs_dat);
		bmi=(BGBCC_BITMAPINFOHEADER *)(obuf+0x0E);
	}
	
	bmi->biSize=40;
	bmi->biWidth=xs;
	bmi->biHeight=ys;
	bmi->biPlanes=1;
	bmi->biBitCount=8;
	bmi->biCompression=BGBCC_FMT_CQL0;
	bmi->biSizeImage=ys*xstr;
	bmi->biXPelsPerMeter=2835;
	bmi->biYPelsPerMeter=2835;
	bmi->biClrUsed=256;
	bmi->biClrImportant=256;

	if(fl&2)
	{
		bmi->biClrUsed=0;
	}
	
	BGBCC_Img_EncodeImageBmpSetupPal(pal, 256);

	aki=0xFF;

	bpal=obuf+ofs_pal;
	bdat=obuf+ofs_dat;
	
	if(!(fl&2))
	{
		for(i=0; i<256; i++)
		{
			bpal[i*4+0]=pal[i*4+2];
			bpal[i*4+1]=pal[i*4+1];
			bpal[i*4+2]=pal[i*4+0];
			bpal[i*4+3]=pal[i*4+3];
			
			if(pal[i*4+3]<16)
				aki=i;
		}
	}
	
	ct=bdat;
	for(y=0; y<bys; y++)
	{
		cs=ibuf+((y*8)*xs*4);
		for(x=0; x<bxs; x++)
		{
			ct=BGBCC_Img_EncodeImageBMP_CellCQ8I(ct, cs, xs*4);
			cs+=8*4;
		}
	}

	bmi->biSizeImage=ct-bdat;
	
	sz=ct-obuf;
	
	if(fl&1)
		{ exwad_setu32(obuf+0x04, sz); }
	else
		{ exwad_setu32(obuf+0x02, sz); }

	return(sz);
}

// int BGBCC_Img_EncodeImageBMP_CQ8(byte *obuf, byte *ibuf,
//	int xs, int ys, byte *pal)
//		{ return(BGBCC_Img_EncodeImageBMP_CQ8I(obuf, ibuf, xs, ys, pal, 0)); }

int BGBCC_Img_EncodeImageBMP_CQ8A(byte *obuf, byte *ibuf,
	int xs, int ys, byte *pal)
{
	return(BGBCC_Img_EncodeImageBMP_CQ8I(obuf, ibuf, xs, ys, pal, 1));
}

// int BGBCC_Img_EncodeImageBMP_CQ8NP(byte *obuf, byte *ibuf,
//	int xs, int ys, byte *pal)
//		{ return(BGBCC_Img_EncodeImageBMP_CQ8I(obuf, ibuf, xs, ys, pal, 2)); }

int BGBCC_Img_EncodeImageBMP_CQ8NPA(byte *obuf, byte *ibuf,
	int xs, int ys, byte *pal)
{
	return(BGBCC_Img_EncodeImageBMP_CQ8I(obuf, ibuf, xs, ys, pal, 3));
}

