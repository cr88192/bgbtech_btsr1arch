static byte *bt5bu_pat6_elut;
byte bt5bu_pat8_xbitnz[256];

int BT5BU_InitEncTables()
{
	int i, j, k;
	if(bt5bu_pat6_elut)
		return(0);

	BT5BU_InitTables();
	
	bt5bu_pat6_elut=malloc(1<<16);
	memset(bt5bu_pat6_elut, 255, 1<<16);
	
	for(i=0; i<256; i++)
	{
		k=0;
		for(j=0; j<8; j++)
			if(i&(1<<j))
				k++;
		bt5bu_pat8_xbitnz[i]=k;
	}
	
	return(1);
}

int BT5BU_QuantPixReducePat6(u32 pix)
{
	int ix, jx;
	byte p0, bd;
	int i, j, k;

//	return(-1);

#if 1
	ix=	(((pix>> 1)&1)<< 0) | (((pix>> 3)&1)<< 1) |
		(((pix>> 5)&1)<< 2) | (((pix>> 7)&1)<< 3) |
		(((pix>> 9)&1)<< 4) | (((pix>>11)&1)<< 5) |
		(((pix>>13)&1)<< 6) | (((pix>>15)&1)<< 7) |
		(((pix>>17)&1)<< 8) | (((pix>>19)&1)<< 9) |
		(((pix>>21)&1)<<10) | (((pix>>23)&1)<<11) |
		(((pix>>25)&1)<<12) | (((pix>>27)&1)<<13) |
		(((pix>>29)&1)<<14) | (((pix>>31)&1)<<15) ;
	p0=bt5bu_pat6_elut[ix];

	if(p0<64)
		return(p0);
	
	p0=0; bd=255;
	for(i=0; i<64; i++)
	{
		jx=bt5bu_pat6[i];
		j=(ix^jx);
		j=bt5bu_pat8_xbitnz[j&255]+bt5bu_pat8_xbitnz[(j>>8)&255];
		if(j<bd)
			{ p0=i; bd=j; }
	}
	bt5bu_pat6_elut[ix]=p0;
	return(p0);
#endif
}

int BT5BU_CalcRgb5Distance(u16 clr1, u16 clr2)
{
	int dr, dg, db, da;
	
	da=0;
	if((clr1^clr2)&0x8000)
		da=255;
	dr=((clr1>>10)&31)-((clr2>>10)&31);
	dg=((clr1>> 5)&31)-((clr2>> 5)&31);
	db=((clr1>> 0)&31)-((clr2>> 0)&31);
	dr^=dr>>31;
	dg^=dg>>31;
	db^=db>>31;
	return((dr+dg+db+da)*3);
}

u16 BT5BU_CalcRgb5_Interp(u16 clra, u16 clrb)
{
	int cra, cga, cba;
	int crb, cgb, cbb;
	int crc, cgc, cbc;
	int cv;
	
	cra=(clra>>10)&31;	cga=(clra>> 5)&31;	cba=(clra>> 0)&31;
	crb=(clrb>>10)&31;	cgb=(clrb>> 5)&31;	cbb=(clrb>> 0)&31;
	crc=((5*cra)+(3*crb))/8;
	cgc=((5*cga)+(3*cgb))/8;
	cbc=((5*cba)+(3*cbb))/8;
	cv=(crc<<10)|(cgc<<5)|cbc;
	return(cv);
}

int BT5BU_CalcBlockDistance(u64 blk1, u64 blk2)
{
	u16 ctab1[4], ctab2[4];
	int ix1, ix2;
	int i, j, k, e;
	
	ctab1[0]=(blk1>> 0)&0x7FFF;
	ctab1[3]=(blk1>>16)&0x7FFF;
	ctab2[0]=(blk2>> 0)&0x7FFF;
	ctab2[3]=(blk2>>16)&0x7FFF;
	ctab1[1]=BT5BU_CalcRgb5_Interp(ctab1[0], ctab1[3]);
	ctab1[2]=BT5BU_CalcRgb5_Interp(ctab1[3], ctab1[0]);
	ctab2[1]=BT5BU_CalcRgb5_Interp(ctab2[0], ctab2[3]);
	ctab2[2]=BT5BU_CalcRgb5_Interp(ctab2[3], ctab2[0]);

//	if(BT5BU_CalcLumaForRgb5(ctab1[0])<BT5BU_CalcLumaForRgb5(ctab1[3]))
	if((blk1>>31)&1)
	{
		ctab1[1]=BT5BU_CalcRgb5_Interp(ctab1[2], ctab1[1]);
		ctab1[2]=0x8000;
	}
//	if(BT5BU_CalcLumaForRgb5(ctab2[0])<BT5BU_CalcLumaForRgb5(ctab2[3]))
	if((blk2>>31)&1)
	{
		ctab2[1]=BT5BU_CalcRgb5_Interp(ctab2[2], ctab2[1]);
		ctab2[2]=0x8000;
	}
	
	e=0;
	for(i=0; i<16; i++)
	{
		ix1=(blk1>>(i*2+32))&3;
		ix2=(blk2>>(i*2+32))&3;
		e+=BT5BU_CalcRgb5Distance(ctab1[ix1], ctab2[ix2]);
	}

	if(((blk1^blk2)>>31)&1)
		e+=1024;

	return(e/16);
}

u64 BT5BU_QuantBlockReduce1bpp(u64 blk1)
{
	u64 tblk;
	u32 px1, px2;
	
	px1=blk1>>32;
	px2=px1&0xAAAAAAAAU;
	px2=px2|(px2>>1);
	tblk=((u32)blk1)|(((u64)px2)<<32);
	return(tblk);
}

byte BT5BU_QuantPixReduce2x2Avg(u32 pix)
{
	int pxc[4];
	int x, y;
	int i, j, k;

	for(i=0; i<4; i++)
		pxc[i]=0;
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		i=(pix>>((y*4+x)*2))&3;
		j=(y>>1)*2+(x>>1);
		pxc[j]+=i;
	}	

	k=0;
	for(i=0; i<4; i++)
	{
		j=pxc[i];
		j/=4;
		k|=j<<(i*2);
	}
	return(k);
}

u64 BT5BU_QuantBlockReduce2x2(u64 blk1)
{
	u64 tblk;
	u32 px1, px2;
	byte pxr;
	
	px1=blk1>>32;
	pxr=BT5BU_QuantPixReduce2x2Avg(px1);
	px2=bt5bu_pat_2x2x2[pxr];

	tblk=((u32)blk1)|(((u64)px2)<<32);
	return(tblk);
}

int BT5BU_QuantGetBlockIndex2x2x2(u64 blk1)
{
	u32 pix;
	byte ix;
	pix=blk1>>32;
	ix=	(((pix>> 0)&3)<<0) |
		(((pix>> 4)&3)<<2) |
		(((pix>>16)&3)<<4) |
		(((pix>>20)&3)<<6) ;
	if(bt5bu_pat_2x2x2[ix]==pix)
		return(ix);
	return(-1);
}

int BT5BU_QuantGetBlockIndex2x2x1(u64 blk1)
{
	u32 pix;
	byte ix;
	pix=blk1>>32;
	ix=	(((pix>> 1)&1)<<0) |
		(((pix>> 5)&1)<<1) |
		(((pix>>17)&1)<<2) |
		(((pix>>21)&1)<<3) ;
	if(bt5bu_pat_2x2x1[ix]==pix)
		return(ix);
	return(-1);
}

int BT5BU_QuantGetBlockIndex4x4x1(u64 blk1)
{
	u32 pix;
	u16 ix;

	pix=blk1>>32;
	if((pix&0x55555555U)!=((pix>>1)&0x55555555U))
		return(-1);
	ix=
		(((pix>> 1)&1)<< 0) | (((pix>> 3)&1)<< 1) |
		(((pix>> 5)&1)<< 2) | (((pix>> 7)&1)<< 3) |
		(((pix>> 9)&1)<< 4) | (((pix>>11)&1)<< 5) |
		(((pix>>13)&1)<< 6) | (((pix>>15)&1)<< 7) |
		(((pix>>17)&1)<< 8) | (((pix>>19)&1)<< 9) |
		(((pix>>21)&1)<<10) | (((pix>>23)&1)<<11) |
		(((pix>>25)&1)<<12) | (((pix>>27)&1)<<13) |
		(((pix>>29)&1)<<14) | (((pix>>31)&1)<<15) ;
	return(ix);
}

int BT5BU_QuantGetBlockIndexPat6(u64 blk1)
{
	u32 pix;
	byte ix;
	pix=blk1>>32;
	ix=BT5BU_QuantPixReducePat6(pix);
	if(bt5bu_pat6x2[ix]==pix)
		return(ix);
	return(-1);
}

u64 BT5BU_QuantBlockReducePat6(u64 blk1)
{
	u64 tblk;
	u32 px1, px2;
	byte pxr;
	
	px1=blk1>>32;
	pxr=BT5BU_QuantPixReducePat6(px1);
	px2=bt5bu_pat6x2[pxr];

	tblk=((u32)blk1)|(((u64)px2)<<32);
	return(tblk);
}

u64 BT5BU_QuantizeBlockLastColors(u64 blk1, u64 lblk1)
{
	u64 blk2;
	blk2=(blk1&(~0xFFFFFFFFULL))|(lblk1&0xFFFFFFFFULL);
	return(blk2);
}

u64 BT5BU_QuantBlockConjColors(u64 blk1)
{
	u64 tblk;
	u32 pix;
	u16 clra, clrb;
	int cra, cga, cba, cya;
	int crb, cgb, cbb, cyb;
	int crc, cgc, cbc, cyc;
	int cv, dy;

	pix=blk1>>32;

	clra=(blk1>> 0)&0x7FFF;
	clrb=(blk1>>16)&0x7FFF;
	cra=(clra>>10)&31;	cga=(clra>> 5)&31;	cba=(clra>> 0)&31;
	crb=(clrb>>10)&31;	cgb=(clrb>> 5)&31;	cbb=(clrb>> 0)&31;
	crc=(cra+crb)/2;
	cgc=(cga+cgb)/2;
	cbc=(cba+cbb)/2;
	cya=(2*cga+cra+cba)/4;
	cyb=(2*cgb+crb+cbb)/4;
	cyc=(2*cgc+crc+cbc)/4;
	dy=((cya-cyb)+2)/4;
	if((dy<0) || (dy>7))
		return(0);
	
	crc&=~1;
	cgc&=~1;
	cbc&=~1;
	
	cra=crc+(dy*2);
	cga=cgc+(dy*2);
	cba=cbc+(dy*2);
	
	crb=crc-(dy*2);
	cgb=cgc-(dy*2);
	cbb=cbc-(dy*2);
	
	if(cra>31)	cra=31;
	if(cga>31)	cga=31;
	if(cba>31)	cba=31;
	if(crb<0)	crb=0;
	if(cgb<0)	cgb=0;
	if(cbb<0)	cbb=0;
	
	clra=(cra<<10)|(cga<<5)|cba;
	clrb=(crb<<10)|(cgb<<5)|cbb;
	
	tblk=(((u64)pix)<<32)|(((u64)clra)<<0)|(((u64)clrb)<<16);
	return(tblk);
}

int BT5BU_QCLD5(int v)
{
	int v1;
	v1=v/2;
	if(v1<(-1))		v1=-1;
	if(v1>( 1))		v1= 1;
	return(v1*2);
}

u64 BT5BU_QuantizeBlockDeltaPairColors(u64 blk1, u64 lblk1)
{
	u64 tblk;
	u32 pix;
	u16 clr1a, clr1b;
	u16 clr0a, clr0b;
	u16 clr2a, clr2b;
	u16 cdva, cdvb;
	int cr1a, cg1a, cb1a, cy1a;
	int cr1b, cg1b, cb1b, cy1b;
	int cr0a, cg0a, cb0a, cy0a;
	int cr0b, cg0b, cb0b, cy0b;
	int cr2a, cg2a, cb2a, cy2a;
	int cr2b, cg2b, cb2b, cy2b;
	int crc, cgc, cbc, cyc;
	int dra, dga, dba;
	int drb, dgb, dbb;
	int drc, dgc, dbc;
	int cv, dy;

	tblk=BT5BU_QuantizeBlockLastColors(blk1, lblk1);
	if(tblk==blk1)
		return(tblk);

	pix=blk1>>32;

	clr1a=(blk1 >> 0)&0x7FFF;
	clr1b=(blk1 >>16)&0x7FFF;
	clr0a=(lblk1>> 0)&0x7FFF;
	clr0b=(lblk1>>16)&0x7FFF;
	cr1a=(clr1a>>10)&31;	cg1a=(clr1a>> 5)&31;	cb1a=(clr1a>> 0)&31;
	cr1b=(clr1b>>10)&31;	cg1b=(clr1b>> 5)&31;	cb1b=(clr1b>> 0)&31;
	cr0a=(clr0a>>10)&31;	cg0a=(clr0a>> 5)&31;	cb0a=(clr0a>> 0)&31;
	cr0b=(clr0b>>10)&31;	cg0b=(clr0b>> 5)&31;	cb0b=(clr0b>> 0)&31;
	dra=cr1a-cr0a;			dga=cg1a-cg0a;			dba=cb1a-cb0a;
	drb=cr1b-cr0b;			dgb=cg1b-cg0b;			dbb=cb1b-cb0b;
	drc=(dra+drb)/2;		dgc=(dga+dgb)/2;		dbc=(dba+dbb)/2;
	dra=BT5BU_QCLD5(dra);	dga=BT5BU_QCLD5(dga);	dba=BT5BU_QCLD5(dba);
	drb=BT5BU_QCLD5(drb);	dgb=BT5BU_QCLD5(dgb);	dbb=BT5BU_QCLD5(dbb);
	drc=BT5BU_QCLD5(drc);	dgc=BT5BU_QCLD5(dgc);	dbc=BT5BU_QCLD5(dbc);
	
	cdva=0;
	cdvb=0;
	if((dra|dga|dba) && (drb|dgb|dbb))
		{ cdva=BT5BU_MKRBG555(drc, dgc, dbc); cdvb=cdva; }
	else if(dra|dga|dba)
		{ cdva=BT5BU_MKRBG555(dra, dga, dba); }
	else
		{ cdvb=BT5BU_MKRBG555(drb, dgb, dbb); }
	
	clr2a=clr0a+cdva;
	clr2b=clr0b+cdvb;
	
	tblk=(((u64)clr2a)<<0)|(((u64)clr2b)<<16)|(((u64)pix)<<32);
	return(tblk);
}

u64 BT5BU_QuantizeBlockDeltaColors(u64 blk1, u64 lblk1)
{
	return(BT5BU_QuantizeBlockDeltaPairColors(blk1, lblk1));
}

int BT5BU_QuantizeFrameBlks(
	u64 *blks, int nblks, int qfl)
{
	int mxe, tqf;
	u64 blk0, blk1, blk2, lblk1;
	int i, j, k;
	
	mxe=100-(qfl&127);
	mxe/=5;
	if(mxe<=0)
		return(0);
	
	lblk1=0;
	for(i=0; i<nblks; i++)
	{
		blk0=blks[i];
		blk1=blk0;
		tqf=0;

		blk2=BT5BU_QuantBlockReduce2x2(blk1);
		if(BT5BU_CalcBlockDistance(blk0, blk2)<mxe)
			{ blk1=blk2; tqf|=1; }

		blk2=BT5BU_QuantBlockReduce1bpp(blk1);
		if(BT5BU_CalcBlockDistance(blk0, blk2)<mxe)
			{ blk1=blk2; tqf|=2; }
		
		if(tqf!=3)
		{
			blk2=BT5BU_QuantBlockReducePat6(blk0);
			if(BT5BU_CalcBlockDistance(blk0, blk2)<mxe)
				blk1=blk2;
		}
		
		blk2=(u32)blk1;
		if(BT5BU_CalcBlockDistance(blk0, blk2)<mxe)
			blk1=blk2;

		blk2=BT5BU_QuantizeBlockDeltaColors(blk1, lblk1);
		if(BT5BU_CalcBlockDistance(blk0, blk2)<mxe)
			{ blk1=blk2; tqf|=8; }
		
		if(!(tqf&8))
		{
			blk2=BT5BU_QuantBlockConjColors(blk1);
			if(BT5BU_CalcBlockDistance(blk0, blk2)<mxe)
				blk1=blk2;
		}

		blk2=BT5BU_QuantizeBlockLastColors(blk1, lblk1);
		if(BT5BU_CalcBlockDistance(blk0, blk2)<mxe)
			blk1=blk2;

		blks[i]=blk1;
		lblk1=blk1;
	}
	return(0);
}

u64 BT5BU_DummyDecodeEndpoint(byte *cs, u64 eps0)
{
	byte *cs1;
	u64 eps1;

	cs1=cs;
	eps1=BT5BU_DecodeEndpoint(&cs1, eps0);
	return(eps1);
}

byte *BT5BU_EncodeEndpoint_ConjBasic(byte *ct, u64 blk)
{
	u16 clra, clrb;
	int cra, cga, cba, cya;
	int crb, cgb, cbb, cyb;
	int crc, cgc, cbc, cyc;
	int cv;
	
	clra=(blk>> 0)&0x7FFF;
	clrb=(blk>>16)&0x7FFF;
	
	cra=(clra>>10)&31;	cga=(clra>> 5)&31;	cba=(clra>> 0)&31;
	crb=(clrb>>10)&31;	cgb=(clrb>> 5)&31;	cbb=(clrb>> 0)&31;
	cya=(2*cga+cra+cba)/4;
	cyb=(2*cgb+crb+cbb)/4;
	
	crc=(cra+crb)/2;
	cgc=(cga+cgb)/2;
	cbc=(cba+cbb)/2;
//	cyc=(cya-cyb)/2;
	cyc=((cya-cyb)+2)/4;
	
	if((cyc<0) || (cyc>7))
		return(ct);
	
	crc/=2;
	cgc/=2;
	cbc/=2;
	
	cv=(cbc<<2)|(cgc<<7)|(cbc<<12)|
		(((cyc>>0)&1)<< 1)|(((cyc>>1)&1)<< 6)|(((cyc>>2)&1)<<11);
	*ct++=(cv>>0)&255;
	*ct++=(cv>>8)&255;
	return(ct);
}

byte *BT5BU_EncodeEndpoint_PairBasic(byte *ct, u64 blk)
{
	u16 clra, clrb;
	int cra, cga, cba, cya;
	int crb, cgb, cbb, cyb;
	int crc, cgc, cbc, cyc;
	int cv;
	
	clra=(blk>> 0)&0x7FFF;
	clrb=(blk>>16)&0x7FFF;
	
	if((blk>>31)&1)
	{
		if(clra>=clrb)
		{
			if(clra<0xFFFF)
				{ clrb=clra+2; }
			else
				{ clra=clrb-2; }
		}
	}
	
	cra=(clra>>10)&31;	cga=(clra>> 5)&31;	cba=(clra>> 0)&31;
	crb=(clrb>>10)&31;	cgb=(clrb>> 5)&31;	cbb=(clrb>> 0)&31;
	cv=7|((cbb>>1)<<3)|(cgb<<7)|(crb<<12)|
		(cba<<17)|(cga<<22)|(cra<<27);
	*ct++=(cv>> 0)&255;	*ct++=(cv>> 8)&255;
	*ct++=(cv>>16)&255;	*ct++=(cv>>24)&255;
	return(ct);
}

byte *BT5BU_EncodeEndpoint_PairDelta(byte *ct0, u64 eps0, u64 blk)
{
	byte *ct;
	u16 clra, clrb;
	u16 clr0a, clr0b;
	int cra, cga, cba, cya;
	int crb, cgb, cbb, cyb;
	int crc, cgc, cbc, cyc;

	int cr0a, cg0a, cb0a, cy0a;
	int cr0b, cg0b, cb0b, cy0b;

	int dra, dga, dba, dya;
	int drb, dgb, dbb, dyb;

	int cv, i3a, i3b;
	
	ct=ct0;
	
	clra=(blk>> 0)&0x7FFF;
	clrb=(blk>>16)&0x7FFF;
	
	clr0a=(eps0>> 0)&0x7FFF;
	clr0b=(eps0>>16)&0x7FFF;
	
	cra=(clra>>10)&31;	cga=(clra>> 5)&31;	cba=(clra>> 0)&31;
	crb=(clrb>>10)&31;	cgb=(clrb>> 5)&31;	cbb=(clrb>> 0)&31;
	cya=(2*cga+cra+cba)/4;
	cyb=(2*cgb+crb+cbb)/4;

	cr0a=(clr0a>>10)&31;	cg0a=(clr0a>> 5)&31;	cb0a=(clr0a>> 0)&31;
	cr0b=(clr0b>>10)&31;	cg0b=(clr0b>> 5)&31;	cb0b=(clr0b>> 0)&31;
	cy0a=(2*cg0a+cr0a+cb0a)/4;
	cy0b=(2*cg0b+cr0b+cb0b)/4;

	dra=(cra-cr0a)/2;
	dga=(cga-cg0a)/2;
	dba=(cba-cb0a)/2;

	drb=(crb-cr0b)/2;
	dgb=(cgb-cg0b)/2;
	dbb=(cbb-cb0b)/2;
	
	i3a=-1;
	if(	(dra>=-1) && (dra<= 1) &&
		(dga>=-1) && (dga<= 1) &&
		(dba>=-1) && (dba<= 1) )
			{ i3a=(dra+1)*9+(dga+1)*3+(dba+1); }
	i3b=-1;
	if(	(drb>=-1) && (drb<= 1) &&
		(dgb>=-1) && (dgb<= 1) &&
		(dbb>=-1) && (dbb<= 1) )
			{ i3b=(drb+1)*9+(dgb+1)*3+(dbb+1); }

	if((i3a>0) && (i3a==i3b))
	{
		*ct++=0x01|(i3a<<3);
		return(ct);
	}
	if((i3a>0) && (i3b==13))
	{
		*ct++=0x03|(i3a<<3);
		return(ct);
	}
	if((i3b>0) && (i3a==13))
	{
		*ct++=0x05|(i3b<<3);
		return(ct);
	}

	return(ct0);
}

byte *BT5BU_EncodeEndpoint_ConjDelta(byte *ct0, u64 eps0, u64 blk)
{
	byte *ct;
	u16 clra, clrb;
	u16 clr0a, clr0b;
	int cra, cga, cba, cya;
	int crb, cgb, cbb, cyb;
	int crc, cgc, cbc, cyc;

	int cr0a, cg0a, cb0a, cy0a;
	int cr0b, cg0b, cb0b, cy0b;

	int dra, dga, dba, dya;
	int drb, dgb, dbb, dyb;

	int cv, i3a, i3b;
	
	ct=ct0;
	
	clra=(blk>> 0)&0x7FFF;
	clrb=(blk>>16)&0x7FFF;
	
	clr0a=(eps0>>32)&0x7FFF;
	
	cra=(clra>>10)&31;	cga=(clra>> 5)&31;	cba=(clra>> 0)&31;
	crb=(clrb>>10)&31;	cgb=(clrb>> 5)&31;	cbb=(clrb>> 0)&31;
	cya=(2*cga+cra+cba)/4;
	cyb=(2*cgb+crb+cbb)/4;

	crc=(cra+crb)/2;
	cgc=(cga+cgb)/2;
	cbc=(cba+cbb)/2;
//	cyc=(2*cgc+crc+cbc)/4;
	cya=((cya-cyb)+2)/4;

	cr0a=(clr0a>>10)&31;	cg0a=(clr0a>> 5)&31;	cb0a=(clr0a>> 0)&31;
	cy0a=(2*cg0a+cr0a+cb0a)/4;
	cy0b=(eps0>>48)&255;

	dra=(crc-cr0a)/2;
	dga=(cgc-cg0a)/2;
	dba=(cbc-cb0a)/2;
	dya=cya-cy0b;

	i3a=-1;
	if(	(dra>=-1) && (dra<= 1) &&
		(dga>=-1) && (dga<= 1) &&
		(dba>=-1) && (dba<= 1) )
			{ i3a=(dra+1)*9+(dga+1)*3+(dba+1); }

	if((i3a>=0) &&
		(dya>=-1) && (dya<= 1))
	{
		*ct++=0x01|(i3a<<3)|((dya+1)<<1);
		return(ct);
	}
	return(ct0);
}

byte *BT5BU_EncodeEndpoint_Delta(byte *ct0, u64 eps0, u64 blk)
{
	if((eps0>>55)&1)
		{ return(BT5BU_EncodeEndpoint_PairDelta(ct0, eps0, blk)); }
	return(BT5BU_EncodeEndpoint_ConjDelta(ct0, eps0, blk));

	return(ct0);
}

int BT5BU_EncodeEndpoint(byte **rct, u64 *reps, u64 blk)
{
	byte *ct, *ct0;
	u64 eps0, eps1;

	ct=*rct;
	ct0=ct;
	eps0=*reps;
	
	ct=BT5BU_EncodeEndpoint_Delta(ct0, eps0, blk);
	if(ct!=ct0)
	{
		eps0=*reps;
		eps1=BT5BU_DummyDecodeEndpoint(ct0, eps0);
		if(((u32)eps1)==((u32)blk))
		{
			*reps=eps1;
			*rct=ct;
			return(ct-ct0);
		}
	}

	ct=BT5BU_EncodeEndpoint_ConjBasic(ct0, blk);
	if(ct!=ct0)
	{
		eps0=*reps;
		eps1=BT5BU_DummyDecodeEndpoint(ct0, eps0);
		if(((u32)eps1)==((u32)blk))
		{
			*reps=eps1;
			*rct=ct;
			return(ct-ct0);
		}
	}
	
	ct=BT5BU_EncodeEndpoint_PairBasic(ct0, blk);
	if(ct!=ct0)
	{
		eps0=*reps;
		eps1=BT5BU_DummyDecodeEndpoint(ct0, eps0);
		*reps=eps1;
		*rct=ct;
		return(ct-ct0);
	}

	return(0);
}

int BT5BU_EncodeBlockFull(byte **rct, u64 *reps, u64 blk)
{
	byte *ct, *ct0, *ct1;
	u64 tgv, eps0, eps1;
	u32 pix;
	int ca, cb, ix;
	
	ct=*rct;
	ct0=ct;
	eps0=*reps;
	eps1=eps0;
	
	ca=(blk>> 0)&0x7FFF;
	cb=(blk>>16)&0x7FFF;
	pix=(u32)(blk>>32);

	if(((u32)eps0)==((u32)blk))
	{
		*ct++=0x3F;
		*(u32 *)ct=pix;
		ct+=4;
		*rct=ct;
		return(ct-ct0);
	}
	
	ct1=ct;
	ix=BT5BU_EncodeEndpoint(&ct1, &eps1, blk);
	if((ix>0) && (ix<3))
	{
		eps1=eps0;
	
		*ct++=0xBF;
		*(u32 *)ct=pix;
		ct+=4;
		BT5BU_EncodeEndpoint(&ct, &eps1, blk);
		*rct=ct;
		*reps=eps1;
		return(ct-ct0);
	}
	
	if((blk>>31)&1)
	{
		if(ca==cb)
		{
			if(cb<0x7FFF)
				{ cb++; }
			else
				{ ca--; }
		}
	}
	
	tgv=1|(((u64)cb)<<2)|(((u64)ca)<<17)|(((u64)pix)<<32);
	*(u64 *)ct=tgv;
	ct+=8;
	*rct=ct;
	*reps=((u32)blk)|((255ULL)<<48);
	return(ct-ct0);
}

int BT5BU_EncodeBlock(byte **rct, u64 *reps, u64 blk)
{
	byte *ct, *ct0;
	u64 eps0, eps1;
	int ix;

	ct=*rct;
	ct0=ct;
	eps0=*reps;
	eps1=eps0;

	ix=BT5BU_QuantGetBlockIndex2x2x1(blk);
	if(ix>=0)
	{
		if(((u32)blk)==((u32)eps0))
		{
			*ct++=0x03|(ix<<4);
		}else
		{
			*ct++=0x0B|(ix<<4);
			BT5BU_EncodeEndpoint(&ct, &eps1, blk);
		}
		*rct=ct;
		*reps=eps1;
		return(0);
	}

	ix=BT5BU_QuantGetBlockIndexPat6(blk);
	if(ix>=0)
	{
		if(((u32)blk)==((u32)eps0))
		{
			*ct++=0x00|(ix<<2);
		}else
		{
			*ct++=0x02|(ix<<2);
			BT5BU_EncodeEndpoint(&ct, &eps1, blk);
		}
		*rct=ct;
		*reps=eps1;
		return(0);
	}

	ix=BT5BU_QuantGetBlockIndex2x2x2(blk);
	if(ix>=0)
	{
		if(((u32)blk)==((u32)eps0))
		{
			*ct++=0x37;
			*ct++=ix;
		}else
		{
			*ct++=0x7F;
			*ct++=ix;
			BT5BU_EncodeEndpoint(&ct, &eps1, blk);
		}
		*rct=ct;
		*reps=eps1;
		return(0);
	}

	ix=BT5BU_QuantGetBlockIndex4x4x1(blk);
	if(ix>=0)
	{
		if(((u32)blk)==((u32)eps0))
		{
			*ct++=0x07;
			*ct++=(ix>>0)&255;
			*ct++=(ix>>8)&255;
		}else
		{
			*ct++=0x27;
			BT5BU_EncodeEndpoint(&ct, &eps1, blk);
			*ct++=(ix>>0)&255;
			*ct++=(ix>>8)&255;
		}
		*rct=ct;
		*reps=eps1;
		return(0);
	}

	BT5BU_EncodeBlockFull(&ct, &eps1, blk);
	
	*rct=ct;
	*reps=eps1;
	return(0);
}

int BT5BU_EncodeFrameBlks(
	byte *codat, int cosz, u64 *blks, int nblks)
{
	byte *ct;
	u64 *cs, *cse;
	u64 eps, blk, blk1, blk2, tgv;
	u32 pix;
	u16 ca, cb;
	int nce, ncf0, ncf1, nc_221, nc_222, nc_441, n, n2, n4, n8;
	int nl, nl4, nl8, nl64, ix, ix1, ix2, ix_p6;
	int i, j, k;
	
	BT5BU_InitEncTables();
	
	ct=codat;
	cs=blks;
	cse=blks+nblks;
	eps=0;
	
	while(cs<cse)
	{
		nl=cse-cs;
		nl4=min(nl, 4);
		nl8=min(nl, 8);
		nl64=min(nl, 64);
		
		if(eps)
		{
			ix_p6=BT5BU_QuantGetBlockIndexPat6(blk);

			for(nce=0; nce<nl64; nce++)
			{
				blk1=cs[nce];
				if((((u32)blk1)!=((u32)eps)))
					break;
			}
			
			for(ncf0=0; ncf0<nl64; ncf0++)
			{	blk1=cs[ncf0];	blk2=(u32)blk1;
				if(blk2!=blk1)	break;		}

			for(ncf1=0; ncf1<nl64; ncf1++)
			{
				blk1=cs[ncf1];
				blk2=blk1|(~0xFFFFFFFFULL);
				if((eps>>31)&1)
					{ blk2=((u32)blk1)|(0xAAAAAAAAULL<<32); }
				if(blk2!=blk1)	break;
			}

			for(nc_221=0; nc_221<nl8; nc_221++)
			{
				blk1=cs[nc_221];
				ix=BT5BU_QuantGetBlockIndex2x2x1(blk1);
				if(ix<0)	break;
			}

			for(nc_222=0; nc_222<nl4; nc_222++)
			{
				blk1=cs[nc_222];
				ix=BT5BU_QuantGetBlockIndex2x2x2(blk1);
				if(ix<0)	break;
			}

			for(nc_441=0; nc_441<nl4; nc_441++)
			{
				blk1=cs[nc_441];
				ix=BT5BU_QuantGetBlockIndex4x4x1(blk1);
				if(ix<0)	break;
			}


			n=min(nce, ncf0);
			n4=n/4;
			if(n4>=2)
			{
				*ct++=0xFF;
				*ct++=0x03|((n4-2)<<4);
				cs+=n4*4;
				continue;
			}
			
			if(n>4)	n=4;
			if(n>=1)
			{
				*ct++=0x0F|((n-1)<<6);
				cs+=n;
				continue;
			}

			n=min(nce, ncf1);
			n4=n/4;
			if(n4>=2)
			{
				*ct++=0xFF;
				*ct++=0x0B|((n4-2)<<4);
				cs+=n4*4;
				continue;
			}

			if(n>4)	n=4;
			if(n>=1)
			{
				*ct++=0x2F|((n-1)<<6);
				cs+=n;
				continue;
			}

			n=min(nl4, ncf0);
			if(n>2)
			{
				*ct++=0x1F|((n-1)<<6);
				for(i=0; i<n; i++)
					{ BT5BU_EncodeEndpoint(&ct, &eps, cs[i]); }
				cs+=n;
				continue;
			}

			n=min(nce, nc_221);
			n2=n/2;
			if(n2>=1)
			{
				*ct++=0x17|((n2-1)<<6);
				for(i=0; i<n2; i++)
				{
					blk1=cs[i*2+0];
					blk2=cs[i*2+1];
					ix1=BT5BU_QuantGetBlockIndex2x2x1(blk1);
					ix2=BT5BU_QuantGetBlockIndex2x2x1(blk2);
					*ct++=ix1|(ix2<<4);
				}
				cs+=(n2*2);
				continue;
			}

			n=min(nce, nc_222);
			if((n>1) && (ix_p6<0))
			{
				*ct++=0x37|((n-1)<<6);
				for(i=0; i<n; i++)
					{ *ct++=BT5BU_QuantGetBlockIndex2x2x2(cs[i]); }
				cs+=n;
				continue;
			}

			n=min(nce, nc_441);
			if((n>1) && (ix_p6<0))
			{
				*ct++=0x07|((n-1)<<6);
				for(i=0; i<n; i++)
				{
					ix=BT5BU_QuantGetBlockIndex4x4x1(cs[i]);
					*ct++=(ix>>0)&255;
					*ct++=(ix>>8)&255;
				}
				cs+=n;
				continue;
			}
		}
		
		blk=*cs++;
		BT5BU_EncodeBlock(&ct, &eps, blk);
	}
	return(ct-codat);
}

u64 BT5BU_ConvBlockFromDXT1(u64 blk)
{
	byte *cnvt;
	u64 tblk;
	u32 px0, px1;
	int ca, cb, cc, isal, flip;
	
	ca=(blk>> 0)&0xFFFF;
	cb=(blk>>16)&0xFFFF;
	px0=blk>>32;
	
	isal=(ca<=cb);
	flip=0;
	
//	isal=0;
	
	ca=(ca&0x001F)|((ca>>1)&0x7FE0);
	cb=(cb&0x001F)|((cb>>1)&0x7FE0);
	
//	if(BT5BU_CalcLumaForRgb5(ca)<BT5BU_CalcLumaForRgb5(cb))
	if(ca<cb)
		flip=1;
	if(isal)
		flip=!flip;
	
	cnvt=bt5bu_conv4x2dxte;
	if(isal && flip)
		cnvt=bt5bu_conv4x2dxtea;

	px1=	(((u32)cnvt[((px0>> 0)&255)])<< 0) |
			(((u32)cnvt[((px0>> 8)&255)])<< 8) |
			(((u32)cnvt[((px0>>16)&255)])<<16) |
			(((u32)cnvt[((px0>>24)&255)])<<24) ;
	
	if(flip)
		{ cc=ca; ca=cb; cb=cc; px1=~px1; }
	tblk=(((u64)ca)<<0)|(((u64)cb)<<16)|(((u64)px1)<<32);
	if(isal)
		tblk|=1ULL<<31;
	return(tblk);
}

int BT5BU_EncodeFrameRGBA(
	byte *cobuf, int cosz, byte *ibuf, int xs, int ys, int qfl)
{
	static u64 *blkbuf;
	static int sz_blkbuf;
	static byte *enc1buf;
	static byte *enc2buf;

	byte *ct;
	u64 blk;
	int xs2, ys2, n, sz1, sz2;
	int x, y;
	
	BT5BU_InitEncTables();
	
	xs2=(xs+3)>>2;
	ys2=(ys+3)>>2;
	n=xs2*ys2;
	
	if(blkbuf && (n>sz_blkbuf))
	{
		free(blkbuf);
		free(enc1buf);
		free(enc2buf);
		blkbuf=NULL;
		enc1buf=NULL;
		enc2buf=NULL;
	}
	if(!blkbuf)
	{
		blkbuf=malloc(xs2*ys2*8);
		enc1buf=malloc(xs2*ys2*12);
		enc2buf=malloc(xs2*ys2*12);
		sz_blkbuf=n;
	}
	
	for(y=0; y<ys2; y++)
		for(x=0; x<xs2; x++)
	{
		blk=BGBCC_Img_EncodeImageDDS_BlockDXT1(
			ibuf+((y*4)*(xs2*4)+x*4)*4,
			xs2*4*4, 0);
		blk=BT5BU_ConvBlockFromDXT1(blk);
		blkbuf[y*xs2+x]=blk;
	}
	
	BT5BU_QuantizeFrameBlks(blkbuf, n, qfl);
	sz1=BT5BU_EncodeFrameBlks(enc1buf, n*12, blkbuf, n);
	
	ct=cobuf;
	
	sz2=sz1+6;
	*ct++=0x40|(sz2>>24);
	*ct++=(sz2>>16)&255;
	*ct++=(sz2>> 8)&255;
	*ct++=(sz2>> 0)&255;
	*ct++='I';
	*ct++='X';
	memcpy(ct, enc1buf, sz1);
	ct+=sz1;
	
	*ct++=0;
	*ct++=0;
	*ct++=0;
	*ct++=0;
	
	return(ct-cobuf);
}
