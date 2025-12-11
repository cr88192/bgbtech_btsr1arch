
typedef struct  {
	u32 dwSize;
	u32 dwFlags;
	u32 dwFourCC;
	u32 dwRGBBitCount;
	u32 dwRBitMask;
	u32 dwGBitMask;
	u32 dwBBitMask;
	u32 dwABitMask;
} BTMGL_DDS_PIXFMT;

typedef struct {
	u32           dwSize;
	u32           dwFlags;
	u32           dwHeight;
	u32           dwWidth;
	u32           dwPitchOrLinearSize;
	u32           dwDepth;
	u32           dwMipMapCount;
	u32           dwReserved1[11];
	BTMGL_DDS_PIXFMT ddspf;
	u32           dwCaps;
	u32           dwCaps2;
	u32           dwCaps3;
	u32           dwCaps4;
	u32           dwReserved2;
} BTMGL_DDS_HEADER;

typedef struct {
	u32 dwMagic;
	BTMGL_DDS_HEADER head;
} BTMGL_DDS_FILEHEADER;


u64 BGBCC_Img_EncodeImageDDS_BlockDXT1(byte *ibuf, int xstr, int flag)
{
	static const byte bayer4[16]={
		 0,  8,  2, 10,
		12,  4, 14,  6,
		 3, 11,  1,  9,
		15,  7, 13,  5
	};

	byte ybuf[16];
	byte abuf[16];
	int ax_my[8], ax_ny[8];
	u64 tblk;
	u32 pix;
	int cr, cg, cb, ca, cy, ax, isal, flip;
	int mcr, mcg, mcb, mcy;
	int ncr, ncg, ncb, ncy;
	int pxa, pxb, acy, acy_l, acy_h, bysc;
	int mca, nca, aca;
	int x, y, z;

	for(z=0; z<4; z++)
		{ ax_my[z]=256; ax_ny[z]=-1; }
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		cr=ibuf[y*xstr+x*4+0];
		cg=ibuf[y*xstr+x*4+1];
		cb=ibuf[y*xstr+x*4+2];
		ca=ibuf[y*xstr+x*4+3];

		if(ca>=96)
		{
			for(z=0; z<4; z++)
			{
				switch(z)
				{
					case 0: cy=(2*cg+1*cr+1*cb)/ 4;		break;
	//				case 1: cy=(8*cg+7*cb+1*cr)/16;		break;
	//				case 2: cy=(8*cb+7*cr+1*cg)/16;		break;
	//				case 3: cy=(8*cr+7*cg+1*cb)/16;		break;
					case 1: cy=(4*cg+3*cb+1*cr)/8;		break;
					case 2: cy=(4*cb+3*cr+1*cg)/8;		break;
					case 3: cy=(4*cr+3*cg+1*cb)/8;		break;
					case 4: cy=128+((cr-cg)/2);			break;
					case 5: cy=128+((cg-cb)/2);			break;
					case 6: cy=128+((cr-cb)/2);			break;
				}
				
				if(cy<ax_my[z])
					ax_my[z]=cy;
				if(cy>ax_ny[z])
					ax_ny[z]=cy;
			}
		}
	}
	
	ax=0; ca=0;
	for(z=0; z<4; z++)
	{
		cy=ax_ny[z]-ax_my[z];
		if(cy>ca)
			{ ax=z; ca=cy; }
	}
	
//	ax=3;
	
	isal=0;		mca=255;	nca=0;		aca=0;
	mcr=255;	mcg=255;	mcb=255;	mcy=255;
	ncr=  0;	ncg=  0;	ncb=  0;	ncy=  0;
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		cr=ibuf[y*xstr+x*4+0];
		cg=ibuf[y*xstr+x*4+1];
		cb=ibuf[y*xstr+x*4+2];
		ca=ibuf[y*xstr+x*4+3];
//		ca=255;

		switch(ax)
		{
			case 0: cy=(2*cg+1*cr+1*cb)/ 4;		break;
//			case 1: cy=(8*cg+7*cb+1*cr)/16;		break;
//			case 2: cy=(8*cb+7*cr+1*cg)/16;		break;
//			case 3: cy=(8*cr+7*cg+1*cb)/16;		break;
			case 1: cy=(4*cg+3*cb+1*cr)/8;		break;
			case 2: cy=(4*cb+3*cr+1*cg)/8;		break;
			case 3: cy=(4*cr+3*cg+1*cb)/8;		break;
			case 4: cy=128+((cr-cg)/2);			break;
			case 5: cy=128+((cg-cb)/2);			break;
			case 6: cy=128+((cr-cb)/2);			break;
		}
		ybuf[y*4+x]=cy;
		abuf[y*4+x]=ca;
		
		if(ca>=96)
		{
			if(cy<mcy)
				{ mcr=cr; mcg=cg; mcb=cb; mcy=cy; }
			if(cy>ncy)
				{ ncr=cr; ncg=cg; ncb=cb; ncy=cy; }
		}
//		if(ca<224)
		if(ca<192)
			isal=1;
		
		if(ca<mca)	mca=ca;
		if(ca>nca)	nca=ca;
		aca+=ca;
	}
	
	aca/=16;
	if((aca>224) && (mca>=128))
		isal=0;
	
	if(flag&1)
		isal=0;
	
	pxa=((ncr>>3)<<11)|((ncg>>2)<<5)|((ncb>>3)<<0);
	pxb=((mcr>>3)<<11)|((mcg>>2)<<5)|((mcb>>3)<<0);

	flip=(pxa<pxb);

	if(isal)
		flip=!flip;
	

	acy=(mcy+ncy)/2;
	acy_l=(mcy+acy)/2;
	acy_h=(ncy+acy)/2;

	bysc=(ncy-mcy)/4;

	pix=0;
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		z=y*4+x;
		cy=ybuf[z];
		ca=abuf[z];
		
		cy+=((((bayer4[z]+1)*15-128)*bysc)>>8);
		ca+=((bayer4[15-z]+1)*15-128);
		
		z=(cy<acy)?((cy<acy_l)?1:3):((cy<acy_h)?2:0);
		if(isal)
		{
			if(z==(flip?3:2))
				z^=1;
			if(ca<128)
				z=flip?2:3;
		}
		pix|=z<<((y*4+x)*2);
	}
	
	if(flip)
	{
//		return(0);
	
		z=pxa; pxa=pxb; pxb=z;
		pix^=0x55555555;
	}
	
	tblk=(pxa<<0)|(((u64)pxb)<<16)|(((u64)pix)<<32);
//	tblk=(pxb<<0)|(pxa<<16)|(((u64)pix)<<32);
	return(tblk);
}

u64 BGBCC_Img_EncodeImageDDS_BlockBC4A(byte *ibuf, int xstr)
{
	static const byte bayer4[16]={
		 0,  8,  2, 10,
		12,  4, 14,  6,
		 3, 11,  1,  9,
		15,  7, 13,  5
	};

	byte ybuf[16];
	int ax_my[4], ax_ny[4];
	u64 tblk;
	u64 pix;
	int cr, cg, cb, ca, cy, ax, isal;
	int mcr, mcg, mcb, mcy;
	int ncr, ncg, ncb, ncy;
	int pxa, pxb, acy, acy_l, acy_h, bysc;
	int x, y, z;
	
	mcy=256;
	ncy=-1;
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		cy=ibuf[y*xstr+x*4+3];
		ybuf[y*4+x]=cy;
		
		if(cy<mcy)
			{ mcy=cy; }
		if(cy>ncy)
			{ ncy=cy; }
	}
	
	acy=(mcy+ncy)/2;
	acy_l=(mcy+acy)/2;
	acy_h=(ncy+acy)/2;

	bysc=(ncy-mcy)/4;

	pix=0;
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		z=y*4+x;
		cy=ybuf[z];
		cy+=((((bayer4[z]+1)*15-128)*bysc)>>8);
		
		z=(cy<acy)?((cy<acy_l)?0:3):((cy<acy_h)?6:1);
		pix|=((u64)z)<<((y*4+x)*3);
	}
	
	tblk=(ncy<<0)|(mcy<<8)|(pix<<16);
	return(tblk);
}

int BGBCC_Img_EncodeImageDDS_DXTn(byte *obuf, byte *ibuf,
	int xs, int ys, int bcnfl)
{
	static byte *resampbuf;
	static int sz_resampbuf;
	BTMGL_DDS_FILEHEADER *fhead;
	BTMGL_DDS_HEADER *tdds;
	u64 *blks, *bct;
	int xs_l2, ys_l2, domip, l2xs, l2ys, szlog2, isz;
	int xs2, ys2, x, y, bcn;
	int i, j, k;
	
	bcn=bcnfl&7;
	domip=1;
	if(bcnfl&16)
		domip=0;
	
	xs_l2=1; ys_l2=1;
	l2xs=0; l2ys=0;
	while(xs_l2<xs)
		{ xs_l2=xs_l2<<1; l2xs++; }
	while(ys_l2<ys)
		{ ys_l2=ys_l2<<1; l2ys++; }
	if((xs_l2<<1)<ys_l2)
		{ xs_l2=ys_l2>>1; }
	if((ys_l2<<1)<xs_l2)
		{ ys_l2=xs_l2>>1; }
	
	k=(xs_l2+4)*(ys_l2+4);
	if(resampbuf && (k>sz_resampbuf))
		{ free(resampbuf); resampbuf=NULL; }
	if(!resampbuf)
	{
		resampbuf=malloc(k*4);
		sz_resampbuf=k;
	}

	BGBCC_ImgUtil_ResampleImage(
		ibuf, xs, ys,
		resampbuf, xs_l2, ys_l2);
	
	szlog2=(l2xs>=l2ys)?l2xs:l2ys;

	j=szlog2-2;
	if(j<0)j=0;
	isz=1<<(j+j+3);

	if(bcn!=1)
		isz=isz<<1;


	fhead=(BTMGL_DDS_FILEHEADER *)obuf;
	tdds=&(fhead->head);
	
	fhead->dwMagic=BGBCC_FMT_DDS;

	tdds->dwSize=124;
	tdds->dwFlags=0;
	tdds->dwHeight=ys_l2;
	tdds->dwWidth=xs_l2;
	tdds->dwPitchOrLinearSize=isz;
	tdds->dwDepth=0;
	tdds->dwMipMapCount=domip?szlog2:0;
	tdds->dwCaps=domip?0x401008:0x001008;
	tdds->dwCaps2=0;
	tdds->dwCaps3=0;
	tdds->dwCaps4=0;
	tdds->dwReserved2=0;
	
	tdds->ddspf.dwSize=32;
	tdds->ddspf.dwFlags=0x0004;
	tdds->ddspf.dwRGBBitCount=16;
	tdds->ddspf.dwRBitMask=0xF800;
	tdds->ddspf.dwGBitMask=0x07E0;
	tdds->ddspf.dwBBitMask=0x001F;
	tdds->ddspf.dwABitMask=0x0000;

	tdds->ddspf.dwFourCC=BGBCC_FMT_DXT1;
	if(bcn==3)
		tdds->ddspf.dwFourCC=BGBCC_FMT_DXT5;

	blks=(u64 *)(obuf+128);
	
	bct=blks;
	
	while((l2xs>=0) || (l2ys>=0))
	{
		xs2=1<<(l2xs-2);
		ys2=1<<(l2ys-2);
		if(l2xs<2)	xs2=1;
		if(l2ys<2)	ys2=1;
		for(y=0; y<ys2; y++)
			for(x=0; x<xs2; x++)
		{
			if(bcn==1)
			{
				bct[y*xs2+x]=
					BGBCC_Img_EncodeImageDDS_BlockDXT1(
						resampbuf+(((ys2*4)-(y*4)-1)*(xs2*4)+x*4)*4,
						-xs2*4*4, 0);
			}

			if(bcn==3)
			{
				bct[(y*xs2+x)*2+1]=
					BGBCC_Img_EncodeImageDDS_BlockDXT1(
						resampbuf+(((ys2*4)-(y*4)-1)*(xs2*4)+x*4)*4,
						-xs2*4*4, 1);
				bct[(y*xs2+x)*2+0]=
					BGBCC_Img_EncodeImageDDS_BlockBC4A(
						resampbuf+(((ys2*4)-(y*4)-1)*(xs2*4)+x*4)*4,
						-xs2*4*4);
			}
		}
		
		if(!domip)
			break;
	
		BGBCC_ImgUtil_ResampleImage(
			resampbuf, xs2*4, ys2*4,
			resampbuf, xs2*2, ys2*2);
		bct+=xs2*ys2;
		if(bcn!=1)
			bct+=xs2*ys2;
		l2xs--; l2ys--;
	}

	k=((byte *)(bct))-obuf;
	return(k);
}

int BGBCC_Img_DecodeImageDDS_BlockDXT1(byte *ibuf, int xstr, u64 blk)
{
	byte clrtab[4*4];
	u32 pix;
	int pxa, pxb;
	int cr, cg, cb, ca;
	int x, y, z;
	
	pxa=(blk>> 0)&0xFFFF;
	pxb=(blk>>16)&0xFFFF;
	pix=blk>>32;

	ca=255;

	cr=(pxa>>11)&31; cg=(pxa>> 5)&63; cb=(pxa>> 0)&31;
	cr=(cr<<3)|(cr>>2); cg=(cg<<2)|(cg>>4); cb=(cb<<3)|(cb>>2);
	clrtab[0*4+0]=cr;	clrtab[0*4+1]=cg;
	clrtab[0*4+2]=cb;	clrtab[0*4+3]=ca;
	cr=(pxb>>11)&31; cg=(pxb>> 5)&63; cb=(pxb>> 0)&31;
	cr=(cr<<3)|(cr>>2); cg=(cg<<2)|(cg>>4); cb=(cb<<3)|(cb>>2);
	clrtab[1*4+0]=cr;	clrtab[1*4+1]=cg;
	clrtab[1*4+2]=cb;	clrtab[1*4+3]=ca;
	clrtab[2*4+0]=((5*clrtab[0*4+0])+(3*clrtab[1*4+0]))/8;
	clrtab[2*4+1]=((5*clrtab[0*4+1])+(3*clrtab[1*4+1]))/8;
	clrtab[2*4+2]=((5*clrtab[0*4+2])+(3*clrtab[1*4+1]))/8;
	clrtab[2*4+3]=ca;
	clrtab[3*4+0]=((5*clrtab[1*4+0])+(3*clrtab[0*4+0]))/8;
	clrtab[3*4+1]=((5*clrtab[1*4+1])+(3*clrtab[0*4+1]))/8;
	clrtab[3*4+2]=((5*clrtab[1*4+2])+(3*clrtab[0*4+1]))/8;
	clrtab[3*4+3]=ca;
	if(pxa<=pxb)
		{ clrtab[3*4+3]=0; }
	
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		z=(pix>>((y*4+x)*2))&3;
		ibuf[y*xstr+x*4+0]=clrtab[z*4+0];
		ibuf[y*xstr+x*4+1]=clrtab[z*4+1];
		ibuf[y*xstr+x*4+2]=clrtab[z*4+2];
		ibuf[y*xstr+x*4+3]=clrtab[z*4+3];
	}
	return(0);
}

int BGBCC_Img_DecodeImageDDS_BlockBC4A(byte *ibuf, int xstr, u64 blk)
{
	byte clrtab[8];
	u64 pix;
	int pxa, pxb;
	int cr, cg, cb, ca;
	int x, y, z;
	
	pxa=(blk>>0)&0xFF;
	pxb=(blk>>8)&0xFF;
	pix=blk>>16;

	ca=255;

	clrtab[0]=pxa;
	clrtab[1]=pxb;

	if(pxa>pxb)
	{
		clrtab[2]=((13*clrtab[0])+( 3*clrtab[1]))/16;
		clrtab[3]=((11*clrtab[0])+( 5*clrtab[1]))/16;
		clrtab[4]=(( 9*clrtab[0])+( 7*clrtab[1]))/16;
		clrtab[5]=(( 7*clrtab[0])+( 9*clrtab[1]))/16;
		clrtab[6]=(( 5*clrtab[0])+(11*clrtab[1]))/16;
		clrtab[7]=(( 3*clrtab[0])+(13*clrtab[1]))/16;
	}else
	{
		clrtab[2]=((13*clrtab[0])+( 3*clrtab[1]))/16;
		clrtab[3]=((10*clrtab[0])+( 6*clrtab[1]))/16;
		clrtab[4]=(( 6*clrtab[0])+(10*clrtab[1]))/16;
		clrtab[5]=(( 3*clrtab[0])+(13*clrtab[1]))/16;
		clrtab[6]=  0;
		clrtab[7]=255;
	}
	
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		z=(pix>>((y*4+x)*3))&7;
		ibuf[y*xstr+x*4+3]=clrtab[z];
	}
	return(0);
}

byte *BGBCC_Img_DecodeDDS(byte *imgbuf, int *rw, int *rh)
{
	BTMGL_DDS_FILEHEADER *fhead;
	BTMGL_DDS_HEADER *tdds;
	u64 *blks, *bct;
	byte *obuf;
	int xs, ys, xs2, ys2, bcn;
	int x, y;

//	return(NULL);

	fhead=(BTMGL_DDS_FILEHEADER *)imgbuf;
	tdds=&(fhead->head);
	blks=(u64 *)(imgbuf+128);

	if(fhead->dwMagic!=BGBCC_FMT_DDS)
		return(NULL);

	xs=tdds->dwWidth;
	ys=tdds->dwHeight;
	
	if((xs&3) || (ys&3) || (xs<4) || (ys<4))
		return(NULL);
	
	bcn=1;
	if(tdds->ddspf.dwFourCC==BGBCC_FMT_DXT1)
		bcn=1;
	if(tdds->ddspf.dwFourCC==BGBCC_FMT_DXT5)
		bcn=3;

	obuf=malloc((xs+1)*(ys+1)*4);

	xs2=xs>>2;
	ys2=ys>>2;
	for(y=0; y<ys2; y++)
		for(x=0; x<xs2; x++)
	{
		if(bcn==1)
		{
			BGBCC_Img_DecodeImageDDS_BlockDXT1(
				obuf+((ys-(y*4)-1)*xs+x*4)*4,
				-xs*4, blks[y*xs2+x]);
		}
		if(bcn==3)
		{
			BGBCC_Img_DecodeImageDDS_BlockDXT1(
				obuf+((ys-(y*4)-1)*xs+x*4)*4,
				-xs*4, blks[(y*xs2+x)*2+1]);
			BGBCC_Img_DecodeImageDDS_BlockBC4A(
				obuf+((ys-(y*4)-1)*xs+x*4)*4,
				-xs*4, blks[(y*xs2+x)*2+0]);
		}
	}

	*rw=xs;
	*rh=ys;
	return(obuf);
}
