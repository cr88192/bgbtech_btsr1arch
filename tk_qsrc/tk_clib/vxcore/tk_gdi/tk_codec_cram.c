void TKGDI_ConvBGR2RGB(byte *sbuf, byte *tbuf, int sz);
void TKGDI_ConvBGRA2RGBA(byte *sbuf, byte *tbuf, int sz);

#if defined(ARCH_SH4LE) || defined(X86) || defined(X86_64)
// #if 0
#define CRAM_GETU16(cs)	(*(u16 *)(cs))
#else
#define CRAM_GETU16(cs)	((cs)[0]|((cs)[1]<<8))
#endif


int cram_decompress_inner8_rgb555(byte *ibuf, u16 *obuf,
	int isz, int osz, cram_ctxinfo *info, byte *pal)
{
	u16 pal16[256];
	u16 aclr[2], bclr[2], cclr[2], dclr[2];
	u16 *bbuf, *bb0, *bb1, *bb2, *bb3;
	u16 px;
	u16 clra, clrb;
	u16 clrc, clrd;
	byte *cs, *cse;
	int xs, ys;
	int n, bx, by, by2, bxs, bys;
	int i;
	
	xs=info->ohead->biWidth;
	ys=info->ohead->biHeight;
	if(ys<0)
		ys=-ys;
	bxs=xs>>2;
	bys=ys>>2;
	
	for(i=0; i<256; i++)
	{
		clra=pal[i*4+0];	clrb=pal[i*4+1];	clrc=pal[i*4+2];
		clra>>=3;			clrb>>=3;			clrc>>=3;
		clrd=(clrc<<10)|(clrb<<5)|clra;
		pal16[i]=clrd;
	}
	
	cs=ibuf; cse=ibuf+isz; bx=0; by=0;
	by2=bys-by-1;
	while(cs<cse)
	{
//		px=cs[0]|(cs[1]<<8);
		px=CRAM_GETU16(cs);
		if(!px && (by>=bys))
//		if(!px)
			break;
		if(px&0x8000)
		{
			cs+=2;

			if((px&0xFC00)==0x8400)
			{
				n=px&1023;
//				if(!n)n=1024;
				bx+=n;
//				bx+=n+1;
				while(bx>=bxs)
				{
					by++; bx-=bxs;
					by2=bys-by-1;
					if(by>=bys)
						break;
				}
				if(by>=bys)
					break;
				continue;
			}
			
			if(px>=0x9000)
			{
				aclr[1]=pal16[cs+0];
				aclr[0]=pal16[cs+1];
				bclr[1]=pal16[cs+2];
				bclr[0]=pal16[cs+3];
				cclr[1]=pal16[cs+4];
				cclr[0]=pal16[cs+5];
				dclr[1]=pal16[cs+6];
				dclr[0]=pal16[cs+7];
				cs+=8;

				bb0[0]=aclr[(px>> 0)&1];	bb0[1]=aclr[(px>> 1)&1];
				bb0[2]=bclr[(px>> 2)&1];	bb0[3]=bclr[(px>> 3)&1];
				bb1[0]=aclr[(px>> 4)&1];	bb1[1]=aclr[(px>> 5)&1];
				bb1[2]=bclr[(px>> 6)&1];	bb1[3]=bclr[(px>> 7)&1];
				bb2[0]=cclr[(px>> 8)&1];	bb2[1]=cclr[(px>> 9)&1];
				bb2[2]=dclr[(px>>10)&1];	bb2[3]=dclr[(px>>11)&1];
				bb3[0]=cclr[(px>>12)&1];	bb3[1]=cclr[(px>>13)&1];
				bb3[2]=dclr[(px>>14)&1];	bb3[3]=dclr[(px>>15)&1];

				bx++;
				if(bx>=bxs)
				{
					by++; bx-=bxs;
					by2=bys-by-1;
					if(by>=bys)
						break;
				}
				continue;
			}
			
			clrc=pal[px&255];
			
			bbuf=obuf+(by2*4*xs+bx*4);
			bb0=bbuf;
			bb1=bb0+xs;
			bb2=bb1+xs;
			bb3=bb2+xs;

			bb0[0]=clrc; bb0[1]=clrc; bb0[2]=clrc; bb0[3]=clrc;
			bb1[0]=clrc; bb1[1]=clrc; bb1[2]=clrc; bb1[3]=clrc;
			bb2[0]=clrc; bb2[1]=clrc; bb2[2]=clrc; bb2[3]=clrc;
			bb3[0]=clrc; bb3[1]=clrc; bb3[2]=clrc; bb3[3]=clrc;
			
			bx++;
			if(bx>=bxs)
			{
				by++; bx-=bxs;
				by2=bys-by-1;
				if(by>=bys)
					break;
			}
			continue;
		}

		clra=*(cs+2);
		clrb=*(cs+3);

		bbuf=obuf+(by2*4*xs+bx*4);
		bb3=bbuf;		bb2=bb3+xs;
		bb1=bb2+xs;		bb0=bb1+xs;

		cs+=4;
		aclr[1]=pal16[clra];
		aclr[0]=pal16[clrb];
		bb0[0]=aclr[(px>> 0)&1];	bb0[1]=aclr[(px>> 1)&1];
		bb0[2]=aclr[(px>> 2)&1];	bb0[3]=aclr[(px>> 3)&1];
		bb1[0]=aclr[(px>> 4)&1];	bb1[1]=aclr[(px>> 5)&1];
		bb1[2]=aclr[(px>> 6)&1];	bb1[3]=aclr[(px>> 7)&1];
		bb2[0]=aclr[(px>> 8)&1];	bb2[1]=aclr[(px>> 9)&1];
		bb2[2]=aclr[(px>>10)&1];	bb2[3]=aclr[(px>>11)&1];
		bb3[0]=aclr[(px>>12)&1];	bb3[1]=aclr[(px>>13)&1];
		bb3[2]=aclr[(px>>14)&1];	bb3[3]=aclr[(px>>15)&1];

		bx++;
		if(bx>=bxs)
		{
			by++; bx-=bxs;
			by2=bys-by-1;
			if(by>=bys)
				break;
		}
		continue;
	}
	
	return(0);
}

u16 cram_tclr_rgb555fl(u16 pxa)
{
	u16 pxc;
	pxc=(pxa&0x03E0)|
		((pxa>>10)&0x001F)|
		((pxa<<10)&0x7C00);
	return(pxc);
}

int cram_decompress_inner_rgb555(byte *ibuf, u16 *obuf,
	int isz, int osz, cram_ctxinfo *info)
{
	u16 aclr[2], bclr[2], cclr[2], dclr[2];
	u16 *bbuf, *bb0, *bb1, *bb2, *bb3;
	u16 px;
	u16 clra, clrb;
	u16 clrc, clrd;
	byte *cs, *cse;
	int xs, ys;
	int n, bx, by, by2, bxs, bys;
	
	xs=info->ohead->biWidth;
	ys=info->ohead->biHeight;
	if(ys<0)
		ys=-ys;
	bxs=xs>>2;
	bys=ys>>2;
	
	cs=ibuf; cse=ibuf+isz; bx=0; by=0;
	by2=bys-by-1;
	while(cs<cse)
	{
//		px=cs[0]|(cs[1]<<8);
		px=CRAM_GETU16(cs);
		if(!px && (by>=bys))
//		if(!px)
			break;
		if(px&0x8000)
		{
			cs+=2;

			if((px&0xFC00)==0x8400)
			{
				n=px&1023;
//				if(!n)n=1024;
				bx+=n;
//				bx+=n+1;
				while(bx>=bxs)
				{
					by++; bx-=bxs;
					by2=bys-by-1;
					if(by>=bys)
						break;
				}
				if(by>=bys)
					break;
				continue;
			}
			
			clrc=cram_tclr_rgb555fl(px);
			
			bbuf=obuf+(by2*4*xs+bx*4);
			bb0=bbuf;
			bb1=bb0+xs;
			bb2=bb1+xs;
			bb3=bb2+xs;

			bb0[0]=clrc; bb0[1]=clrc; bb0[2]=clrc; bb0[3]=clrc;
			bb1[0]=clrc; bb1[1]=clrc; bb1[2]=clrc; bb1[3]=clrc;
			bb2[0]=clrc; bb2[1]=clrc; bb2[2]=clrc; bb2[3]=clrc;
			bb3[0]=clrc; bb3[1]=clrc; bb3[2]=clrc; bb3[3]=clrc;
			
			bx++;
			if(bx>=bxs)
			{
				by++; bx-=bxs;
				by2=bys-by-1;
				if(by>=bys)
					break;
			}
			continue;
		}

//		clra=cs[2]|(cs[3]<<8);
//		clrb=cs[4]|(cs[5]<<8);
		clra=CRAM_GETU16(cs+2);
		clrb=CRAM_GETU16(cs+4);

		bbuf=obuf+(by2*4*xs+bx*4);
//		bb0=bbuf;		bb1=bb0+xs;
//		bb2=bb1+xs;		bb3=bb2+xs;

		bb3=bbuf;		bb2=bb3+xs;
		bb1=bb2+xs;		bb0=bb1+xs;

		if(clra&0x8000)
		{
			cs+=6;
			aclr[1]=cram_tclr_rgb555fl(clra);
			aclr[0]=cram_tclr_rgb555fl(clrb);

#if 1
			bclr[1]=cram_tclr_rgb555fl(CRAM_GETU16(cs+ 0));
			bclr[0]=cram_tclr_rgb555fl(CRAM_GETU16(cs+ 2));
			cclr[1]=cram_tclr_rgb555fl(CRAM_GETU16(cs+ 4));
			cclr[0]=cram_tclr_rgb555fl(CRAM_GETU16(cs+ 6));
			dclr[1]=cram_tclr_rgb555fl(CRAM_GETU16(cs+ 8));
			dclr[0]=cram_tclr_rgb555fl(CRAM_GETU16(cs+10));
#endif

			cs+=12;

#if 1
			bb0[0]=aclr[(px>> 0)&1];	bb0[1]=aclr[(px>> 1)&1];
			bb0[2]=bclr[(px>> 2)&1];	bb0[3]=bclr[(px>> 3)&1];
			bb1[0]=aclr[(px>> 4)&1];	bb1[1]=aclr[(px>> 5)&1];
			bb1[2]=bclr[(px>> 6)&1];	bb1[3]=bclr[(px>> 7)&1];
			bb2[0]=cclr[(px>> 8)&1];	bb2[1]=cclr[(px>> 9)&1];
			bb2[2]=dclr[(px>>10)&1];	bb2[3]=dclr[(px>>11)&1];
			bb3[0]=cclr[(px>>12)&1];	bb3[1]=cclr[(px>>13)&1];
			bb3[2]=dclr[(px>>14)&1];	bb3[3]=dclr[(px>>15)&1];
#endif

#if 0
			bb0[0]=aclr[(px>> 0)&1];	bb0[1]=aclr[(px>> 1)&1];
			bb0[2]=aclr[(px>> 2)&1];	bb0[3]=aclr[(px>> 3)&1];
			bb1[0]=aclr[(px>> 4)&1];	bb1[1]=aclr[(px>> 5)&1];
			bb1[2]=aclr[(px>> 6)&1];	bb1[3]=aclr[(px>> 7)&1];
			bb2[0]=aclr[(px>> 8)&1];	bb2[1]=aclr[(px>> 9)&1];
			bb2[2]=aclr[(px>>10)&1];	bb2[3]=aclr[(px>>11)&1];
			bb3[0]=aclr[(px>>12)&1];	bb3[1]=aclr[(px>>13)&1];
			bb3[2]=aclr[(px>>14)&1];	bb3[3]=aclr[(px>>15)&1];
#endif
		}else
		{
			cs+=6;
			aclr[1]=cram_tclr_rgb555fl(clra);
			aclr[0]=cram_tclr_rgb555fl(clrb);

#if 1
			bb0[0]=aclr[(px>> 0)&1];	bb0[1]=aclr[(px>> 1)&1];
			bb0[2]=aclr[(px>> 2)&1];	bb0[3]=aclr[(px>> 3)&1];
			bb1[0]=aclr[(px>> 4)&1];	bb1[1]=aclr[(px>> 5)&1];
			bb1[2]=aclr[(px>> 6)&1];	bb1[3]=aclr[(px>> 7)&1];
			bb2[0]=aclr[(px>> 8)&1];	bb2[1]=aclr[(px>> 9)&1];
			bb2[2]=aclr[(px>>10)&1];	bb2[3]=aclr[(px>>11)&1];
			bb3[0]=aclr[(px>>12)&1];	bb3[1]=aclr[(px>>13)&1];
			bb3[2]=aclr[(px>>14)&1];	bb3[3]=aclr[(px>>15)&1];
#endif
		}

		bx++;
		if(bx>=bxs)
		{
			by++; bx-=bxs;
			by2=bys-by-1;
			if(by>=bys)
				break;
		}
		continue;
	}
	
	return(0);
}

u32 cram_tclr_rgb555(u16 pxa)
{
	u32 pxc;
	pxc=0xFF000000|
		((pxa&0x7C00)<<9)|
		((pxa&0x03E0)<<6)|
		((pxa&0x001F)<<3);
	return(pxc);
}

int cram_decompress_inner_bgra(byte *ibuf, u32 *obuf,
	int isz, int osz, cram_ctxinfo *info)
{
	u32 aclr[2], bclr[2], cclr[2], dclr[2];
	u32 *bbuf, *bb0, *bb1, *bb2, *bb3;
	u16 px;
	u16 clra, clrb;
	u32 clrc, clrd;
	byte *cs, *cse;
	int xs, ys;
	int n, bx, by, by2, bxs, bys;
	
	xs=info->ohead->biWidth;
	ys=info->ohead->biHeight;
	if(ys<0)
		ys=-ys;
	bxs=xs>>2;
	bys=ys>>2;
	
	cs=ibuf; cse=ibuf+isz; bx=0; by=0;
	by2=bys-by-1;
	while(cs<cse)
	{
//		px=cs[0]|(cs[1]<<8);
		px=CRAM_GETU16(cs);
		if(!px && (by>=bys))
//		if(!px)
			break;
		if(px&0x8000)
		{
			cs+=2;

			if((px&0xFC00)==0x8400)
			{
				n=px&1023;
//				if(!n)n=1024;
				bx+=n;
//				bx+=n+1;
				while(bx>=bxs)
				{
					by++; bx-=bxs;
					by2=bys-by-1;
					if(by>=bys)
						break;
				}
				if(by>=bys)
					break;
				continue;
			}
			
			clrc=cram_tclr_rgb555(px);
			
			bbuf=obuf+(by2*4*xs+bx*4);
			bb0=bbuf;
			bb1=bb0+xs;
			bb2=bb1+xs;
			bb3=bb2+xs;

			bb0[0]=clrc; bb0[1]=clrc; bb0[2]=clrc; bb0[3]=clrc;
			bb1[0]=clrc; bb1[1]=clrc; bb1[2]=clrc; bb1[3]=clrc;
			bb2[0]=clrc; bb2[1]=clrc; bb2[2]=clrc; bb2[3]=clrc;
			bb3[0]=clrc; bb3[1]=clrc; bb3[2]=clrc; bb3[3]=clrc;
			
			bx++;
			if(bx>=bxs)
			{
				by++; bx-=bxs;
				by2=bys-by-1;
				if(by>=bys)
					break;
			}
			continue;
		}

//		clra=cs[2]|(cs[3]<<8);
//		clrb=cs[4]|(cs[5]<<8);
		clra=CRAM_GETU16(cs+2);
		clrb=CRAM_GETU16(cs+4);

		bbuf=obuf+(by2*4*xs+bx*4);
//		bb0=bbuf;		bb1=bb0+xs;
//		bb2=bb1+xs;		bb3=bb2+xs;

		bb3=bbuf;		bb2=bb3+xs;
		bb1=bb2+xs;		bb0=bb1+xs;

		if(clra&0x8000)
		{
			cs+=6;
			aclr[1]=cram_tclr_rgb555(clra);
			aclr[0]=cram_tclr_rgb555(clrb);

#if 1
			bclr[1]=cram_tclr_rgb555(CRAM_GETU16(cs+ 0));
			bclr[0]=cram_tclr_rgb555(CRAM_GETU16(cs+ 2));
			cclr[1]=cram_tclr_rgb555(CRAM_GETU16(cs+ 4));
			cclr[0]=cram_tclr_rgb555(CRAM_GETU16(cs+ 6));
			dclr[1]=cram_tclr_rgb555(CRAM_GETU16(cs+ 8));
			dclr[0]=cram_tclr_rgb555(CRAM_GETU16(cs+10));
#endif

			cs+=12;

#if 1
			bb0[0]=aclr[(px>> 0)&1];	bb0[1]=aclr[(px>> 1)&1];
			bb0[2]=bclr[(px>> 2)&1];	bb0[3]=bclr[(px>> 3)&1];
			bb1[0]=aclr[(px>> 4)&1];	bb1[1]=aclr[(px>> 5)&1];
			bb1[2]=bclr[(px>> 6)&1];	bb1[3]=bclr[(px>> 7)&1];
			bb2[0]=cclr[(px>> 8)&1];	bb2[1]=cclr[(px>> 9)&1];
			bb2[2]=dclr[(px>>10)&1];	bb2[3]=dclr[(px>>11)&1];
			bb3[0]=cclr[(px>>12)&1];	bb3[1]=cclr[(px>>13)&1];
			bb3[2]=dclr[(px>>14)&1];	bb3[3]=dclr[(px>>15)&1];
#endif

#if 0
			bb0[0]=aclr[(px>> 0)&1];	bb0[1]=aclr[(px>> 1)&1];
			bb0[2]=aclr[(px>> 2)&1];	bb0[3]=aclr[(px>> 3)&1];
			bb1[0]=aclr[(px>> 4)&1];	bb1[1]=aclr[(px>> 5)&1];
			bb1[2]=aclr[(px>> 6)&1];	bb1[3]=aclr[(px>> 7)&1];
			bb2[0]=aclr[(px>> 8)&1];	bb2[1]=aclr[(px>> 9)&1];
			bb2[2]=aclr[(px>>10)&1];	bb2[3]=aclr[(px>>11)&1];
			bb3[0]=aclr[(px>>12)&1];	bb3[1]=aclr[(px>>13)&1];
			bb3[2]=aclr[(px>>14)&1];	bb3[3]=aclr[(px>>15)&1];
#endif
		}else
		{
			cs+=6;
			aclr[1]=cram_tclr_rgb555(clra);
			aclr[0]=cram_tclr_rgb555(clrb);

#if 1
			bb0[0]=aclr[(px>> 0)&1];	bb0[1]=aclr[(px>> 1)&1];
			bb0[2]=aclr[(px>> 2)&1];	bb0[3]=aclr[(px>> 3)&1];
			bb1[0]=aclr[(px>> 4)&1];	bb1[1]=aclr[(px>> 5)&1];
			bb1[2]=aclr[(px>> 6)&1];	bb1[3]=aclr[(px>> 7)&1];
			bb2[0]=aclr[(px>> 8)&1];	bb2[1]=aclr[(px>> 9)&1];
			bb2[2]=aclr[(px>>10)&1];	bb2[3]=aclr[(px>>11)&1];
			bb3[0]=aclr[(px>>12)&1];	bb3[1]=aclr[(px>>13)&1];
			bb3[2]=aclr[(px>>14)&1];	bb3[3]=aclr[(px>>15)&1];
#endif
		}

		bx++;
		if(bx>=bxs)
		{
			by++; bx-=bxs;
			by2=bys-by-1;
			if(by>=bys)
				break;
		}
		continue;
	}
	
	return(0);
}

byte cram_fliptab4[16]={
	0x0, 0x8, 0x4, 0xC, 0x2, 0xA, 0x6, 0xE,
	0x1, 0x9, 0x5, 0xD, 0x3, 0xB, 0x7, 0xF};
byte cram_fliptab[256];
byte cram_fliptab_init=0;
u16 cram_frnum;
byte cram_hextab4[16]="0123456789ABCDEF";

int cram_decompress_inner_rcon(byte *ibuf, u32 *obuf,
	int isz, int osz, cram_ctxinfo *info)
{
	u32 aclr[2], bclr[2], cclr[2], dclr[2];
	u32 *bbuf, *bb0, *bb1, *bb2, *bb3;
	u32 *conbufa, *conbufb;
	u16 px, px1;
	u16 clra, clrb;
	u32 clrc, clrd;
	byte *cs, *cse;
	int xs, ys;
	int n, bx, by, by2, bxs, bys, rt;
	int i, j, k;
	
	xs=info->ohead->biWidth;
	ys=info->ohead->biHeight;
	bxs=xs>>2;
	bys=ys>>2;
	
	if(!cram_fliptab_init)
	{
		cram_fliptab_init=1;
		for(i=0; i<16; i++)
			for(j=0; j<16; j++)
		{
			k=cram_fliptab4[j]|(cram_fliptab4[i]<<4);
//			k=cram_fliptab4[i]|(cram_fliptab4[j]<<4);
			cram_fliptab[i*16+j]=k;
		}
	}
	
	conbufa=(u32 *)0xA0FF0000;
	conbufb=conbufa+(80*61);

	rt=0;
	cs=ibuf; cse=ibuf+isz; bx=0; by=0;
	by2=bys-by-1;
	while(cs<cse)
	{
		px=CRAM_GETU16(cs);
		if(!px && (by>=bys))
//		if(!px)
		{
			rt=1;
			break;
		}
		if(px&0x8000)
		{
			cs+=2;

			if((px&0xFC00)==0x8400)
			{
				n=px&1023;
				bx+=n;
				while(bx>=bxs)
				{
					by++; bx-=bxs;
					by2=bys-by-1;
					if(by>=bys)
						break;
				}
				if(by>=bys)
				{
					rt=2;
					break;
				}
				continue;
			}
			
			conbufa[by2*80+bx]=0x3055FFFF;
			conbufb[by2*80+bx]=px;
						
			bx++;
			if(bx>=bxs)
			{
				by++; bx-=bxs;
				by2=bys-by-1;
				if(by>=bys)
				{
					rt=3;
					break;
				}
			}
			continue;
		}

//		clra=cs[2]|(cs[3]<<8);
//		clrb=cs[4]|(cs[5]<<8);
		clra=CRAM_GETU16(cs+2);
		clrb=CRAM_GETU16(cs+4);

		bbuf=obuf+(by2*4*xs+bx*4);
//		bb0=bbuf;		bb1=bb0+xs;
//		bb2=bb1+xs;		bb3=bb2+xs;

		bb3=bbuf;		bb2=bb3+xs;
		bb1=bb2+xs;		bb0=bb1+xs;

		if(clra&0x8000)
		{
			cs+=6;
			aclr[1]=clra;
			aclr[0]=clrb;
			bclr[1]=CRAM_GETU16(cs+ 0);
			bclr[0]=CRAM_GETU16(cs+ 2);
			cclr[1]=CRAM_GETU16(cs+ 4);
			cclr[0]=CRAM_GETU16(cs+ 6);
			dclr[1]=CRAM_GETU16(cs+ 8);
			dclr[0]=CRAM_GETU16(cs+10);

			cs+=12;

#if 0
			if((aclr[0]&0x03E0)>(aclr[1]&0x03E0))
				{ px1=aclr[0]; aclr[0]=aclr[1]; aclr[1]=px1; px^=0x0033; }
			if((bclr[0]&0x03E0)>(bclr[1]&0x03E0))
				{ px1=aclr[0]; bclr[0]=bclr[1]; bclr[1]=px1; px^=0x00CC; }

			if((cclr[0]&0x03E0)>(cclr[1]&0x03E0))
				{ px1=cclr[0]; cclr[0]=cclr[1]; cclr[1]=px1; px^=0x3300; }
			if((dclr[0]&0x03E0)>(dclr[1]&0x03E0))
				{ px1=dclr[0]; dclr[0]=dclr[1]; dclr[1]=px1; px^=0xCC00; }

			clra=aclr[0]; clrb=aclr[1];
			if((bclr[0]&0x03E0)<(clra&0x03E0)) { clra=bclr[0]; }
			if((cclr[0]&0x03E0)<(clra&0x03E0)) { clra=cclr[0]; }
			if((dclr[0]&0x03E0)<(clra&0x03E0)) { clra=dclr[0]; }
			if((bclr[1]&0x03E0)>(clrb&0x03E0)) { clrb=bclr[1]; }
			if((cclr[1]&0x03E0)>(clrb&0x03E0)) { clrb=cclr[1]; }
			if((dclr[1]&0x03E0)>(clrb&0x03E0)) { clrb=dclr[1]; }

			px1=(cram_fliptab[(px   )&255]   )|
				(cram_fliptab[(px>>8)&255]<<8);
//			px1=(cram_fliptab[(px   )&255]<<8)|
//				(cram_fliptab[(px>>8)&255]   );

			conbufa[by2*80+bx]=0x30550000|px1;
//			conbufb[by2*80+bx]=(aclr[0]<<16)|aclr[1];
#endif

//			conbufb[by2*80+bx]=(clrb<<16)|clra;
//			conbufb[by2*80+bx]=(clra<<16)|clrb;
			conbufb[by2*80+bx]=1;
		}else
		{
			cs+=6;
//			aclr[1]=cram_tclr_rgb555(clra);
//			aclr[0]=cram_tclr_rgb555(clrb);

			px1=(cram_fliptab[(px   )&255]   )|
				(cram_fliptab[(px>>8)&255]<<8);
//			px1=(cram_fliptab[(px   )&255]<<8)|
//				(cram_fliptab[(px>>8)&255]   );

			conbufa[by2*80+bx]=0x30550000|px1;
			conbufb[by2*80+bx]=(clrb<<16)|clra;
		}

		bx++;
		if(bx>=bxs)
		{
			by++; bx-=bxs;
			by2=bys-by-1;
			if(by>=bys)
			{
				rt=3;
				break;
			}
		}
		continue;
	}

	cram_frnum++;

	conbufa[59*80+68]=0x10070000|'C';
	conbufa[59*80+69]=0x10070000|'R';
	conbufa[59*80+70]=0x10070000|'A';
	conbufa[59*80+71]=0x10070000|'M';
	conbufa[59*80+72]=0x10070000|('0'+rt);

	conbufa[59*80+74]=0x10070000|(cram_hextab4[(cram_frnum>>8)&15]);
	conbufa[59*80+75]=0x10070000|(cram_hextab4[(cram_frnum>>4)&15]);
	conbufa[59*80+76]=0x10070000|(cram_hextab4[(cram_frnum   )&15]);
		
	return(0);
}

static int cram_decompress_frame_clrs(TKGDI_VidCodecCTX *ctx,
	void *src, void *dst, int ssz, int dsz, int clrs)
{
	cram_ctxinfo *info;
	int err, i, j, k, ib, ob;
	byte *sbuf, *tbuf;

	info=ctx->data;

	info->ihead->biSizeImage=ssz;
	info->ohead->biSizeImage=dsz;

//	memset(dst, 255, info->ohead->biWidth*info->ohead->biHeight*4);

	if(dst && (clrs==TKGDI_JPG_BGRA))
	{
		cram_decompress_inner_bgra(src, dst, ssz, dsz, info);
	}else if(clrs==TKGDI_JPG_RAWCON)
	{
		cram_decompress_inner_rcon(src, info->buffer, ssz, dsz, info);
	}else
	{
		cram_decompress_inner_bgra(src, info->buffer, ssz, dsz, info);
	}

#if 0
	i=(!dst)?ICDECOMPRESS_PREROLL:0;	//frame skip
//	i=ICDECOMPRESS_NOTKEYFRAME;
//	err=ICDecompress(info->hicd, 0, info->ihead, src, info->ohead, dst);
	if(dst && (clrs==TKGDI_JPG_BGRA))
	{
		err=ICDecompress(info->hicd, i,
			(BITMAPINFOHEADER *)info->ihead, src,
			(BITMAPINFOHEADER *)info->ohead, dst);
	}else
	{
		err=ICDecompress(info->hicd, i,
			(BITMAPINFOHEADER *)info->ihead, src,
			(BITMAPINFOHEADER *)info->ohead, info->buffer);
	}
#endif

	if(dst && (clrs!=TKGDI_JPG_BGRA))
	{
		sbuf=info->buffer;
		tbuf=dst;

		j=info->ohead->biHeight;
		if(j<0)j=-j;
		j=info->ohead->biWidth*j;
		k=info->ohead->biBitCount/8;

		if(clrs==TKGDI_JPG_RGBA)
		{
			if(k==3)TKGDI_ConvBGR2RGB(sbuf, tbuf, j);
			if(k==4)TKGDI_ConvBGRA2RGBA(sbuf, tbuf, j);
		}
	}
	return(0);
}

static int cram_decompress_frame(TKGDI_VidCodecCTX *ctx,
	void *src, void *dst, int ssz, int dsz)
{
	return(cram_decompress_frame_clrs(ctx,
		src, dst, ssz, dsz, TKGDI_JPG_BGRA));
}

static TKGDI_VidCodecCTX *cram_begin_decompress(int fcc,
	TKGDI_BITMAPINFOHEADER *in, TKGDI_BITMAPINFOHEADER *out)
{
	TKGDI_VidCodecCTX *ctx;
	cram_ctxinfo *info;
	int err;

	if(in->biCompression!=TKGDI_FCC_CRAM)
		return(NULL);

	ctx=TKGDI_VidCodecCTX_New();
//	info=bgbrpi_malloc(sizeof(cram_ctxinfo));
	info=malloc(sizeof(cram_ctxinfo));
	ctx->data=info;

	info->ihead=malloc(sizeof(TKGDI_BITMAPINFOHEADER));
	memset(info->ihead, 0, sizeof(TKGDI_BITMAPINFOHEADER));
	info->ihead->biSize		= sizeof(TKGDI_BITMAPINFOHEADER);
	info->ihead->biWidth		= in->biWidth;
	info->ihead->biHeight		= in->biHeight;
	info->ihead->biPlanes		= in->biPlanes;
	info->ihead->biBitCount		= in->biBitCount;
	info->ihead->biCompression	= in->biCompression;
	info->ihead->biSizeImage	=
		in->biWidth*in->biHeight*in->biBitCount/8;

	info->ohead=malloc(sizeof(TKGDI_BITMAPINFOHEADER));
	memset(info->ohead, 0, sizeof(TKGDI_BITMAPINFOHEADER));
	info->ohead->biSize		= sizeof(TKGDI_BITMAPINFOHEADER);
	info->ohead->biWidth		= out->biWidth;
//	info->ohead->biHeight		= -out->biHeight;
	info->ohead->biHeight		= out->biHeight;
	info->ohead->biPlanes		= out->biPlanes;
	info->ohead->biBitCount		= out->biBitCount;
	info->ohead->biCompression	= out->biCompression;
	info->ihead->biSizeImage	=
		out->biWidth*out->biHeight*out->biBitCount/8;

	info->buffer=malloc(out->biWidth*out->biHeight*out->biBitCount/8);

	ctx->decompress_frame=&cram_decompress_frame;
	ctx->decompress_frame_clrs=&cram_decompress_frame_clrs;

	return(ctx);
}

static void cram_shutdown()
{
	int i;
}

int TKGDI_CodecCRAM_Init()
{
	TKGDI_VidCodec *codec;
	int i, j;
	short *sw;
	char buf[256];
	char *t;

	printf("CRAM: Init\n");

	codec=TKGDI_VidCodec_New();
	codec->begin_decompress=&cram_begin_decompress;

	return(0);
}
