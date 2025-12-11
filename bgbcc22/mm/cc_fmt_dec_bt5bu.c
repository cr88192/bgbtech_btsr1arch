/*
 * Mini decoder for BTIC5B-U
 * Goal is to decode fairly directly to DXT1.
 *
 * BTIC5B-U will be a variant of BTIC5B limited to still images.
 * 
 * EndpointState
 *   (15: 0): ColorA
 *   (31:16): ColorB
 *   (47:32): ColorCenter
 *   (55:48): ColorDy
 */

#define BTPIC_TCC_PX	BGBCC_TWOCC('P', 'X')
#define BTPIC_TCC_IX	BGBCC_TWOCC('I', 'X')
#define BTPIC_TCC_HX	BGBCC_TWOCC('H', 'X')

#define BTPIC_TCC_Z3	BGBCC_TWOCC('Z', '3')
#define BTPIC_TCC_Z4	BGBCC_TWOCC('Z', '4')
#define BTPIC_TCC_Z5	BGBCC_TWOCC('Z', '5')
#define BTPIC_TCC_Z6	BGBCC_TWOCC('Z', '6')
#define BTPIC_TCC_Z7	BGBCC_TWOCC('Z', '7')

#define	BT5BU_MKRBG555(r,g,b)	((((r)&31)<<10)|(((g)&31)<<5)|(((b)&31)<<0))

#define btpic_getu16(cs)		(*(volatile u16 *)(cs))
#define btpic_getu32(cs)		(*(volatile u32 *)(cs))
#define btpic_getu64(cs)		(*(volatile u64 *)(cs))

u16 bt5bu_dystep[8]={
	BT5BU_MKRBG555( 0, 0, 0),	BT5BU_MKRBG555( 2, 2, 2),
	BT5BU_MKRBG555( 4, 4, 4),	BT5BU_MKRBG555( 6, 6, 6),
	BT5BU_MKRBG555( 8, 8, 8),	BT5BU_MKRBG555(10,10,10),
	BT5BU_MKRBG555(12,12,12),	BT5BU_MKRBG555(14,14,14)
	};

char bt5bu_pat6gen[8*4]={
	 2,  2,  2,  2,
	 4,  2, -2, -4,
	 4, -1,  1, -4,
	 4, -4,  4, -4,
	-2, -2, -2, -2,
	-4, -2,  2,  4,
	-4,  1, -1,  4,
	-4,  4, -4,  4
};

u16 bt5bu_delta5[32];
u16 bt5bu_delta9[512];

u16 bt5bu_pat6[64];
u32 bt5bu_pat6x2[64];

u32 bt5bu_pat_2x2x2[256];
u32 bt5bu_pat_2x2x1[16];
u32 bt5bu_pat_4x2x1[256];

byte bt5bu_conv4x2dxt[256];
byte bt5bu_conv4x2dxte[256];

byte bt5bu_conv4x2dxta[256];
byte bt5bu_conv4x2dxtea[256];

int BT5BU_InitTables()
{
	static byte remap4dxt[4]={0, 2, 3, 1};
	static byte remap4dxt_a[4]={0, 3, 2, 1};
	u32 pix;
	int i0, i1, i2, i3, px;
	int i, j, k;

	if(bt5bu_pat_2x2x1[15])
		return(0);

	for(i=0; i<32; i++)
	{
		i0=(i/1)%3;
		i1=(i/3)%3;
		i2=(i/9)%3;
		j=BT5BU_MKRBG555((i0-1)*2, (i1-1)*2, (i2-1)*2);
		bt5bu_delta5[i]=j;
	}

	for(i=0; i<512; i++)
	{
		i0=(i/ 1)%7;
		i1=(i/ 7)%7;
		i2=(i/49)%7;
		j=BT5BU_MKRBG555((i0-1)*2, (i1-1)*2, (i2-1)*2);
		bt5bu_delta9[i]=j;
	}

	for(i=0; i<64; i++)
	{
		pix=0; px=0;
		for(j=0; j<16; j++)
		{
			i0=(i>>0)&7; i1=(i>>3)&7;
			i2=bt5bu_pat6gen[(i0*4)+((j>>0)&3)];
			i3=bt5bu_pat6gen[(i1*4)+((j>>2)&3)];
			k=i2+i3;
			if(k>=0)px|=1<<j;
			if(k>=0)pix|=3<<(j*2);
		}
		bt5bu_pat6[i]=px;
		bt5bu_pat6x2[i]=pix;
	}

	for(i=0; i<16; i++)
	{
		pix=0;
		if(i&1)		{ pix|=0x00000F0FU; }
		if(i&2)		{ pix|=0x0000F0F0U; }
		if(i&4)		{ pix|=0x0F0F0000U; }
		if(i&8)		{ pix|=0xF0F00000U; }
		bt5bu_pat_2x2x1[i]=pix;
	}

	for(i=0; i<256; i++)
	{
		pix=0;
		if(i&0x01)	{ pix|=0x00000505U; }
		if(i&0x02)	{ pix|=0x00000A0AU; }
		if(i&0x04)	{ pix|=0x00005050U; }
		if(i&0x08)	{ pix|=0x0000A0A0U; }
		if(i&0x10)	{ pix|=0x05050000U; }
		if(i&0x20)	{ pix|=0x0A0A0000U; }
		if(i&0x40)	{ pix|=0x50500000U; }
		if(i&0x80)	{ pix|=0xA0A00000U; }
		bt5bu_pat_2x2x2[i]=pix;
		pix=0;
		if(i&0x01)	{ pix|=0x00000003U; }
		if(i&0x02)	{ pix|=0x0000000CU; }
		if(i&0x04)	{ pix|=0x00000030U; }
		if(i&0x08)	{ pix|=0x000000C0U; }
		if(i&0x10)	{ pix|=0x00000300U; }
		if(i&0x20)	{ pix|=0x00000C00U; }
		if(i&0x40)	{ pix|=0x00003000U; }
		if(i&0x80)	{ pix|=0x0000C000U; }
		bt5bu_pat_4x2x1[i]=pix;
	}

	for(i=0; i<256; i++)
	{
		k=	(remap4dxt[(i>>0)&3]<<0) |
			(remap4dxt[(i>>2)&3]<<2) |
			(remap4dxt[(i>>4)&3]<<4) |
			(remap4dxt[(i>>6)&3]<<6) ;
		bt5bu_conv4x2dxt[i]=k;
		bt5bu_conv4x2dxte[k]=i;

		j=	(remap4dxt_a[(i>>0)&3]<<0) |
			(remap4dxt_a[(i>>2)&3]<<2) |
			(remap4dxt_a[(i>>4)&3]<<4) |
			(remap4dxt_a[(i>>6)&3]<<6) ;
		bt5bu_conv4x2dxta[i]=j;
		bt5bu_conv4x2dxtea[j]=i;
	}

	return(0);
}

int BT5BU_CalcLumaForRgb5(u16 v)
{
	int cr, cg, cb, cy;
	cr=(v>>10)&31;
	cg=(v>>5)&31;
	cb=(v>>0)&31;
	cy=(2*cg+cr+cb)>>2;
	return(cy);
}

u64 BT5BU_DecodeEndpoint(byte **rcs, u64 eps0)
{
	u64 eps1;
	byte *cs;
	u32 v;
	int cdy, cdi, cdj, cdk, cdv, cdva, cdvb;
	u16 ca, cb, cc;

	cs=*rcs;
	v=btpic_getu32(cs);
	if((v&7)==7)
	{
		cs+=4;
		ca=(v>>17)&0x7FFF;
		cb=(v>> 2)&0x7FFE;
		eps1=(ca<<0)|(cb<<16)|(128ULL<<48);
//		if(BT5BU_CalcLumaForRgb5(ca)<BT5BU_CalcLumaForRgb5(cb))
		if(ca<cb)
//			eps1|=1ULL<<54;
			eps1|=1ULL<<31;
		*rcs=cs;
		return(eps1);
	}
	
	if(!(v&1))
	{
		cs+=2;
		cc=(v>>1)&0x7BDE;
		cdy=((v>>1)&1)|((v>>5)&2)|((v>>9)&4);
		ca=cc+bt5bu_dystep[cdy];
		cb=cc-bt5bu_dystep[cdy];

		eps1=(ca<<0)|(cb<<16)|(((u64)cc)<<32)|(((u64)cdy)<<48);
		*rcs=cs;
		return(eps1);
	}

	cdy=(eps0>>48)&255;
	cdi=(v>>3)&31;
	if(cdi>=0x1B)
	{
		cdv=0;
		cdva=0;
		cdvb=0;

		if(cdy&128)
		{
			if(cdi>=0x1E)
			{
				cs+=2;
				cdi=((v>>8)&255)|((cdi<<8)&1);
				cdv=bt5bu_delta9[cdi];
			}else if(cdi>=0x1C)
			{
				cs+=3;
				cdi=((v>>8)&65535)|((cdi<<16)&1);
				cdj=cdi%360;
				cdk=cdi/360;
				cdva=bt5bu_delta9[cdj];
				cdvb=bt5bu_delta9[cdk];
			}else
			{
				cs+=3;
				cdv=(v>>9)&0x7FFF;
				cdva=cdv;
				cdvb=cdv;
			}
		}
	}else
	{
		cs+=1;
		cdv=bt5bu_delta5[cdi];
		cdva=cdv;
		cdvb=cdv;
	}

	if(cdy&128)
	{
		ca=(u16)(eps0>> 0);
		cb=(u16)(eps0>>16);
		if(!(v&4))
		{
			ca+=cdva;
			ca&=0x7FFF;
		}
		if(!(v&2))
		{
			cb+=cdvb;
			cb&=0x7FFF;
		}
		
//		eps1=(ca<<0)|(cb<<16)|(128ULL<<48);
		eps1=(ca<<0)|(cb<<16)|((eps0>>48)<<48);
		*rcs=cs;
		return(eps1);
	}else
	{
		cdy+=((v>>1)&3)-1;
		cc=((u16)(eps0>>32))+cdv;
		ca=cc+bt5bu_dystep[cdy];
		cb=cc-bt5bu_dystep[cdy];

		eps1=(ca<<0)|(cb<<16)|(((u64)cc)<<32)|(((u64)cdy)<<48);
		*rcs=cs;
		return(eps1);
	}
}

int BT5BU_DecodeImageData(
	byte *ibuf, int szibuf, u64 *blks, int nblks)
{
	u64 *ct, *cte;
	byte *cs, *cse;
	u64 eps, tgv, blk;
	u32 pix;
	u16 ca, cb;
	int ix, n;
	int i, j, k;
	
	cs=ibuf;
	cse=ibuf+szibuf;
	ct=blks;
	cte=blks+nblks;
	
	eps=0; ca=0; cb=0;
	
	while((cs<cse) && (ct<cte))
	{
		tgv=btpic_getu64(cs);
		
		if(!(tgv&1))	//zzzz_zzz0
		{
			cs++;
			if(tgv&2)
				{ eps=BT5BU_DecodeEndpoint(&cs, eps); }
			ix=(tgv>>2)&63;
			pix=bt5bu_pat6x2[ix];
			blk=((u32)eps)|(((u64)pix)<<32);
			*ct++=blk;
			continue;
		}

		if(!(tgv&2))	//zzzz_zz01
		{
			cs+=8;
			cb=(tgv>>2)&0x7FFF;
			ca=(tgv>>17)&0x7FFF;
			eps=(ca<<0)|(cb<<16)|(128ULL<<48);
//			if(BT5BU_CalcLumaForRgb5(ca)<BT5BU_CalcLumaForRgb5(cb))
			if(ca<cb)
//				eps|=1ULL<<54;
				eps|=1ULL<<31;
			pix=tgv>>32;
//			blk=((u32)eps)|(((u64)pix)<<32)|(((eps>>54)&1)<<31);
			blk=((u32)eps)|(((u64)pix)<<32);
			*ct++=blk;
			continue;
		}

		if(!(tgv&4))	//zzzz_z011
		{
			cs++;
			if(tgv&8)
				{ eps=BT5BU_DecodeEndpoint(&cs, eps); }
			ix=(tgv>>4)&15;
			pix=bt5bu_pat_2x2x1[ix];
			blk=((u32)eps)|(((u64)pix)<<32);
			*ct++=blk;
			continue;
		}

		if(!(tgv&8))	//zzzz_0111
		{
			cs++;
			n=((tgv>>6)&3)+1;
			if(!(tgv&16))
			{
				if(tgv&32)
					{ eps=BT5BU_DecodeEndpoint(&cs, eps); }
				while(n--)
				{
					pix =bt5bu_pat_4x2x1[cs[0]]<<0;
					pix|=bt5bu_pat_4x2x1[cs[1]]<<8;
					cs+=2;
					blk=((u32)eps)|(((u64)pix)<<32);
					blk=blk;
					*ct++=blk;
				}
				continue;
			}

			if(tgv&32)
			{
				while(n--)
				{
					pix=bt5bu_pat_2x2x2[*cs++];
//					blk=((u32)eps)|(((u64)pix)<<32)|(((eps>>54)&1)<<31);
					blk=((u32)eps)|(((u64)pix)<<32);
					blk=blk;
					*ct++=blk;
				}
				continue;
			}
			while(n--)
			{
				ix=*cs++;
				pix=bt5bu_pat_2x2x1[ix&15];
				blk=((u32)eps)|(((u64)pix)<<32);
				*ct++=blk;
				pix=bt5bu_pat_2x2x1[(ix>>4)&15];
				blk=((u32)eps)|(((u64)pix)<<32);
				*ct++=blk;
			}
			continue;
		}

		if(!(tgv&16))	//zzz0_1111
		{
			cs++;
			n=((tgv>>6)&3)+1;
			pix=0x00000000U;
			if(tgv&32)
			{
				pix=0xFFFFFFFFU;
//				if((eps>>54)&1)
				if((eps>>31)&1)
					{ pix=0xAAAAAAAAU; }
			}
//			blk=((u32)eps)|(((u64)pix)<<32)|(((eps>>54)&1)<<31);
			blk=((u32)eps)|(((u64)pix)<<32);
			blk=blk;
			while(n--)
				{ *ct++=blk; }
			continue;
		}

		if(!(tgv&32))	//zz01_1111
		{
			cs++;
			n=((tgv>>6)&3)+1;

			while(n--)
			{
				eps=BT5BU_DecodeEndpoint(&cs, eps);
				blk=((u32)eps);
				*ct++=blk;
			}
			continue;
		}

		if(!(tgv&64))	//z011_1111
		{
			cs+=5;
			if(tgv&128)
				{ eps=BT5BU_DecodeEndpoint(&cs, eps); }
			pix=tgv>>8;
			blk=((u32)eps)|(((u64)pix)<<32);
			*ct++=blk;
			continue;
		}

		if(!(tgv&128))	//0111_1111
		{
			cs+=2;
			eps=BT5BU_DecodeEndpoint(&cs, eps);
			ix=(tgv>>8)&255;
			pix=bt5bu_pat_2x2x2[ix];

			blk=((u32)eps)|(((u64)pix)<<32);
			*ct++=blk;
			continue;
		}

		if(!(tgv&256))	//zzz0_1111_1111
			{ break; }
		if(!(tgv&512))	//zz01_1111_1111
			{ break; }

		if(!(tgv&1024))	//z011_1111_1111
		{
			cs+=2;
			n=((tgv>>12)&15)*4+8;
			i=(tgv>>11)&1;
			pix=0x00000000U;
			if(i)
			{
				pix=0xFFFFFFFFU;
//				if((eps>>54)&1)
				if((eps>>31)&1)
					{ pix=0xAAAAAAAAU; }
			}
//			blk=((u32)eps)|(((u64)pix)<<32)|(((eps>>54)&1)<<31);
			blk=((u32)eps)|(((u64)pix)<<32);
			while(n--)
				{ *ct++=blk; }
			continue;
		}
		
		break;
	}
	return(0);
}

int BT5BU_DecodeFrameBlks(
	byte *cdat, int csz, u64 *blks, int nblks)
{
	static byte *zfbuf;
	static int zfbsz;
	u64 *tblk;
	byte *cs, *cs0, *cs1, *cs2, *cse;
	byte *fdat;
	u32 fcc;
	int sz, dsz, fsz, ffl, cmp;
	int i, j, k, n;
	
	if(!csz)
		return(0);
	
	BT5BU_InitTables();
	
	fdat=NULL; ffl=0;
	cs=cdat; cse=cdat+csz;
	while(cs<cse)
	{
		i=cs[0];
		if((i>>5)==0)
		{
			sz=((i&31)<<8)|cs[1];
			fcc=btpic_getu16(cs+2);
			cs0=cs+4;	cs1=cs+sz;
		}else if((i>>5)==1)
		{
			sz=((i&31)<<8)|cs[1];
			fcc=btpic_getu32(cs+2);
			cs0=cs+6;	cs1=cs+sz;
		}else if((i>>5)==2)
		{
			sz=((i&31)<<24)|(cs[1]<<16)|(cs[2]<<8)|cs[3];
			fcc=btpic_getu16(cs+4);
			cs0=cs+6;	cs1=cs+sz;
		}else if((i>>5)==3)
		{
			sz=((i&31)<<24)|(cs[1]<<16)|(cs[2]<<8)|cs[3];
			fcc=btpic_getu32(cs+4);
			cs0=cs+8;	cs1=cs+sz;
		}else if((i>>5)==4)
		{
			sz=((i&31)<<16)|(cs[1]<<8)|cs[2];
			fcc=btpic_getu16(cs+3);
			cs0=cs+5;	cs1=cs+sz;
		}else if((i>>5)==5)
		{
			sz=((i&31)<<16)|(cs[1]<<8)|cs[2];
			fcc=btpic_getu32(cs+3);
			cs0=cs+7;	cs1=cs+sz;
		}

		if(!sz || !fcc)
			break;

// #if 1
#ifndef BGBCC_MTOOL
		if(	(fcc==BTPIC_TCC_Z3) || (fcc==BTPIC_TCC_Z4) ||
			(fcc==BTPIC_TCC_Z5) || (fcc==BTPIC_TCC_Z6) ||
			(fcc==BTPIC_TCC_Z7))
		{
			k=btpic_getu32(cs0+0);
			dsz=k&0xFFFFFF;
			cs2=cs0+4;
			
			if(!zfbuf)
			{
				i=65536;
				while(i<=(dsz+64))
					i=i+(i>>1);
				zfbuf=malloc(i);
				zfbsz=i;
			}
			
			if((dsz+64)>zfbsz)
			{
				i=zfbsz;
				while(i<=(dsz+64))
					i=i+(i>>1);
				zfbuf=realloc(zfbuf, i);
				zfbsz=i;
			}

			cmp=-1;
			if(fcc==BTPIC_TCC_Z3)	cmp=3;	//RP2
			if(fcc==BTPIC_TCC_Z4)	cmp=4;	//LZ4
			if(fcc==BTPIC_TCC_Z6)	cmp=6;	//LZ4LLB

			if(cmp>=0)
			{
//				BTPIC_DecodeBufferRP2(
//					cs2, ctx->zfbuf,
//					cs1-cs2, ctx->zfbsz);
				BGBCC_TKPE_UnpackBuffer(zfbuf, cs2, cs1-cs2, NULL, cmp);
				BT5BU_DecodeFrameBlks(zfbuf, dsz, blks, nblks);
			}
			cs=cs1;
			continue;
		}
		
		if(fcc==BTPIC_TCC_HX)
		{
//			ctx->xs=btpic_getu16(cs0+0);
//			ctx->ys=btpic_getu16(cs0+2);
//			ctx->fl=btpic_getu16(cs0+4);
//			ctx->bxs=(ctx->xs+3)>>2;
//			ctx->bys=(ctx->ys+3)>>2;			
			cs=cs1;
			continue;
		}
#endif

		if(	(fcc==BTPIC_TCC_IX) ||
			(fcc==BTPIC_TCC_PX)	)
		{
			fdat=cs0; fsz=cs1-cs0; ffl=0;
//			if(fcc==BTPIC_TCC_PX)
//				ffl|=BTPIC_QFL_PFRAME;
			cs=cs1;
			continue;
		}

		cs=cs1;
		continue;
	}
	
	if(fdat)
	{
		BT5BU_DecodeImageData(fdat, fsz, blks, nblks);
	}

	return(0);
}

u64 BT5BU_ConvBlockToDXT1(u64 blk)
{
	byte *cnvt;
	u64 tblk;
	u32 px0, px1;
	int ca, cb, cc, isal, flip;
	
	ca=(blk>> 0)&0x7FFF;
	cb=(blk>>16)&0x7FFF;
//	isal=(BT5BU_CalcLumaForRgb5(ca)<BT5BU_CalcLumaForRgb5(cb));
	isal=(blk>>31)&1;
	flip=0;

//	isal=0;

	px0=blk>>32;
	ca=(ca&0x001F)|((ca&0x7FE0)<<1)|((ca>>4)&0x0020);
	cb=(cb&0x001F)|((cb&0x7FE0)<<1)|((cb>>4)&0x0020);
	
	if(ca<cb)	flip=1;
	if(isal)	flip=!flip;

	cnvt=bt5bu_conv4x2dxt;
	if(isal && flip)
		cnvt=bt5bu_conv4x2dxta;
		
	px1=	(((u32)cnvt[((px0>> 0)&255)])<< 0) |
			(((u32)cnvt[((px0>> 8)&255)])<< 8) |
			(((u32)cnvt[((px0>>16)&255)])<<16) |
			(((u32)cnvt[((px0>>24)&255)])<<24) ;

	if(flip)
		{ cc=ca; ca=cb; cb=cc; px1^=0x55555555U; }

	if((ca<=cb) && !isal)
		px1&=0x55555555U;
	
	tblk=(((u64)ca)<<0)|(((u64)cb)<<16)|(((u64)px1)<<32);
	return(tblk);
}

int BT5BU_DecodeFrameBlksDXT1(
	byte *cdat, int csz, u64 *blks, int nblks)
{
	int i;
	BT5BU_DecodeFrameBlks(cdat, csz, blks, nblks);
	for(i=0; i<nblks; i++)
		{ blks[i]=BT5BU_ConvBlockToDXT1(blks[i]); }
	return(0);
}

int BT5BU_DecodeFrameRGBA(
	byte *cdat, int csz, byte *obuf, int xs, int ys)
{
	static u64 *blkbuf;
	static int sz_blkbuf;
	int xs2, ys2, n;
	int x, y;
	
	xs2=(xs+3)>>2;
	ys2=(ys+3)>>2;
	n=xs2*ys2;
	
	if(blkbuf && (n>sz_blkbuf))
		{ free(blkbuf); blkbuf=NULL; }
	if(!blkbuf)
		{ blkbuf=malloc(xs2*ys2*8); sz_blkbuf=n; }
	
	BT5BU_DecodeFrameBlksDXT1(cdat, csz, blkbuf, n);
	for(y=0; y<ys2; y++)
		for(x=0; x<xs2; x++)
	{
		BGBCC_Img_DecodeImageDDS_BlockDXT1(
			obuf+((y*4)*xs+x*4)*4,
			xs*4, blkbuf[y*xs2+x]);
	}
	return(0);
}
