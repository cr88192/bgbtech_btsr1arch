#define TKUPI_HASBCNENC

u64 TKuPI_EncodeImageDDS_BlockDXT1(byte *ibuf, int xstr, int flag)
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
		cb=ibuf[y*xstr+x*4+0];
		cg=ibuf[y*xstr+x*4+1];
		cr=ibuf[y*xstr+x*4+2];
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
		cb=ibuf[y*xstr+x*4+0];
		cg=ibuf[y*xstr+x*4+1];
		cr=ibuf[y*xstr+x*4+2];
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
	
	if((pxa==pxb) && !isal)
	{
		pix=0;
		tblk=(pxa<<0)|(((u64)pxb)<<16)|(((u64)pix)<<32);
		return(tblk);
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
		ca=abuf[z];
		
		cy+=((((bayer4[z]+1)*15-128)*bysc)>>8);

		if((nca-mca)<=128)
			ca+=((bayer4[15-z]+1)*15-128)/2;
		
		z=(cy<acy)?((cy<acy_l)?1:3):((cy<acy_h)?2:0);
		if(isal)
		{
			if(z==(flip?2:3))
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

u64 TKuPI_EncodeImageDDS_BlockBC4A(byte *ibuf, int xstr)
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


int TKuPI_DecodeImageDDS_BlockDXT1(byte *ibuf, int xstr, u64 blk)
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
	{
		clrtab[2*4+0]=(clrtab[0*4+0]+clrtab[1*4+0])/2;
		clrtab[2*4+1]=(clrtab[0*4+1]+clrtab[1*4+1])/2;
		clrtab[2*4+2]=(clrtab[0*4+2]+clrtab[1*4+2])/2;
		clrtab[3*4+3]=0;
	}
	
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		z=(pix>>((y*4+x)*2))&3;
		ibuf[y*xstr+x*4+0]=clrtab[z*4+2];
		ibuf[y*xstr+x*4+1]=clrtab[z*4+1];
		ibuf[y*xstr+x*4+2]=clrtab[z*4+0];
		ibuf[y*xstr+x*4+3]=clrtab[z*4+3];
	}
	return(0);
}

int TKuPI_DecodeImageDDS_BlockBC4A(byte *ibuf, int xstr, u64 blk)
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

int TKuPI_EncodeImageDDS_MipBlockBC1(
	u64 *oblk,
	u64 blk0, u64 blk1, u64 blk2, u64 blk3)
{
	byte tbuf[8*8*4], tbuf1[4*4*4];
	int x, y, z;
	TKuPI_DecodeImageDDS_BlockDXT1(tbuf+(0*8+0)*4, 8*4, blk0);
	TKuPI_DecodeImageDDS_BlockDXT1(tbuf+(0*8+4)*4, 8*4, blk1);
	TKuPI_DecodeImageDDS_BlockDXT1(tbuf+(4*8+0)*4, 8*4, blk2);
	TKuPI_DecodeImageDDS_BlockDXT1(tbuf+(4*8+4)*4, 8*4, blk3);
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
			for(z=0; z<4; z++)
	{
		tbuf1[(y*4+x)*4+z]=
			(tbuf[((y*2+0)*8+(x*2+0))*4+z]+
			 tbuf[((y*2+0)*8+(x*2+1))*4+z]+
			 tbuf[((y*2+1)*8+(x*2+0))*4+z]+
			 tbuf[((y*2+1)*8+(x*2+1))*4+z])/4;
	}
	*oblk=TKuPI_EncodeImageDDS_BlockDXT1(tbuf1, 4*4, 0);
	return(0);
}

int TKuPI_EncodeImageDDS_MipBlockBC3(
	u64 *oblk,
	u64 *blk0, u64 *blk1, u64 *blk2, u64 *blk3)
{
	byte tbuf[8*8*4], tbuf1[4*4*4];
	int x, y, z;

	TKuPI_DecodeImageDDS_BlockDXT1(tbuf+(0*8+0)*4, 8*4, blk0[1]);
	TKuPI_DecodeImageDDS_BlockBC4A(tbuf+(0*8+0)*4, 8*4, blk0[0]);
	TKuPI_DecodeImageDDS_BlockDXT1(tbuf+(0*8+4)*4, 8*4, blk1[1]);
	TKuPI_DecodeImageDDS_BlockBC4A(tbuf+(0*8+4)*4, 8*4, blk1[0]);
	TKuPI_DecodeImageDDS_BlockDXT1(tbuf+(4*8+0)*4, 8*4, blk2[1]);
	TKuPI_DecodeImageDDS_BlockBC4A(tbuf+(4*8+0)*4, 8*4, blk2[0]);
	TKuPI_DecodeImageDDS_BlockDXT1(tbuf+(4*8+4)*4, 8*4, blk3[1]);
	TKuPI_DecodeImageDDS_BlockBC4A(tbuf+(4*8+4)*4, 8*4, blk3[0]);

	for(y=0; y<4; y++)
		for(x=0; x<4; y++)
			for(z=0; z<4; z++)
	{
		tbuf1[(y*4+x)*4+z]=
			(tbuf[((y*2+0)*8+(x*2+0))*4+z]+
			 tbuf[((y*2+0)*8+(x*2+1))*4+z]+
			 tbuf[((y*2+1)*8+(x*2+0))*4+z]+
			 tbuf[((y*2+1)*8+(x*2+1))*4+z])/4;
	}
	oblk[0]=TKuPI_EncodeImageDDS_BlockBC4A(tbuf1, 4*4);
	oblk[1]=TKuPI_EncodeImageDDS_BlockDXT1(tbuf1, 4*4, 0);
	return(0);
}

u64 TKuPI_EncodeImageDDS_FlipBlockVertBC1(u64 blk)
{
	u64 tv, px0, px1;
	px0=blk>>32;
	px1=(((px0>> 0)&255)<<24) |
		(((px0>> 8)&255)<<16) |
		(((px0>>16)&255)<< 8) |
		(((px0>>24)&255)<< 0) ;
	tv=((u32)blk)|(px1<<32);
	return(tv);
}

u64 TKuPI_EncodeImageDDS_FlipImageVertBC1(u64 *ibuf, int ixs, int iys)
{
	u64 *cs, *ct;
	u64 v0, v1;
	int xs, ys, x, y, y1, bxs, bys;
	
	xs=ixs;		ys=iys;
	bxs=ixs/4;	bys=iys/4;
	
	for(y=0; y<(bys/2); y++)
	{
		y1=bys-y-1;
		for(x=0; x<bxs; x++)
		{
			v0=ibuf[y *bxs+x];
			v1=ibuf[y1*bxs+x];
			v0=TKuPI_EncodeImageDDS_FlipBlockVertBC1(v0);
			v1=TKuPI_EncodeImageDDS_FlipBlockVertBC1(v1);
			ibuf[y *bxs+x]=v1;
			ibuf[y1*bxs+x]=v0;
		}
	}

	return(0);
}

u64 TKuPI_EncodeImageDDS_BuildMipsBC1(u64 *ibuf, int ixs, int iys)
{
	u64 *cs, *ct;
	int xs, ys, x, y, bxs, bys;
	
	xs=ixs;		ys=iys;
	bxs=(ixs+3)/4;	bys=(iys+3)/4;
	
	cs=ibuf;
	ct=ibuf+bxs*bys;
	while((xs>=8) && (ys>=8))
	{
		bxs=(xs+7)/8;
		bys=(ys+7)/8;
		
		for(y=0; y<bys; y++)
			for(x=0; x<bxs; x++)
		{
			TKuPI_EncodeImageDDS_MipBlockBC1(
				ct+y*bxs+x,
				cs[(y*2+0)*(bxs*2)+(x*2+0)],
				cs[(y*2+0)*(bxs*2)+(x*2+1)],
				cs[(y*2+1)*(bxs*2)+(x*2+0)],
				cs[(y*2+1)*(bxs*2)+(x*2+1)]);
		}
		cs=ct;
		ct=cs+bxs*bys;
		xs/=2;	ys/=2;
	}

	while((xs>1) || (ys>1))
	{
		bxs=(xs+7)/8;
		bys=(ys+7)/8;
		
		for(y=0; y<bys; y++)
			for(x=0; x<bxs; x++)
		{
			TKuPI_EncodeImageDDS_MipBlockBC1(
				ct+y*bxs+x, cs[0], cs[0], cs[0], cs[0]);
		}
		cs=ct;
		ct=cs+bxs*bys;
		xs/=2;	ys/=2;
	}
	return(0);
}

u64 TKuPI_EncodeImageDDS_BuildMipsBC3(u64 *ibuf, int ixs, int iys)
{
	u64 *cs, *ct;
	int xs, ys, x, y, bxs, bys;
	
	xs=ixs;		ys=iys;
	bxs=ixs/4;	bys=iys/4;
	
	cs=ibuf;
	ct=ibuf+bxs*bys*2;
	while((xs>=8) || (ys>=8))
	{
		bxs=xs/8;
		bys=ys/8;
		
		for(y=0; y<bys; y++)
			for(x=0; x<bxs; x++)
		{
			TKuPI_EncodeImageDDS_MipBlockBC3(
				ct+(y*bxs+x)*2,
				cs+((y*2+0)*(bxs*2)+(x*2+0))*2,
				cs+((y*2+0)*(bxs*2)+(x*2+1))*2,
				cs+((y*2+1)*(bxs*2)+(x*2+0))*2,
				cs+((y*2+1)*(bxs*2)+(x*2+1))*2);
		}
		cs=ct;
		ct=cs+bxs*bys*2;
		xs/=2;	ys/=2;
	}

	while((xs>1) || (ys>1))
	{
		bxs=(xs+7)/8;
		bys=(ys+7)/8;
		
		for(y=0; y<bys; y++)
			for(x=0; x<bxs; x++)
		{
			TKuPI_EncodeImageDDS_MipBlockBC3(
				ct+(y*bxs+x)*2, cs, cs, cs, cs);
		}
		cs=ct;
		ct=cs+bxs*bys*2;
		xs/=2;	ys/=2;
	}
	return(0);
}
