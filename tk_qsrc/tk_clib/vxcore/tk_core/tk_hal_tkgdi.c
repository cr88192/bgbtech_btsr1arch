#include <tk_core.h>
#include <tkgdi/tkgdi.h>

int tkgdi_vid_frnum;
int tkgdi_vid_scrmode;
int tkgdi_vid_cellstride;
int tkgdi_vid_rowstride;

int tkgdi_vid_xsize;
int tkgdi_vid_ysize;

int tkgdi_vid_bmxsize;

u64 *tkgdi_vid_screenutx;		//screen UTX2 buffer
byte *tkgdi_vid_screendirty;		//screen UTX2 buffer


void TKGDI_BlitUpdate_ScanCopy(u16 *ics, u32 *ict, int blkn);
void TKGDI_BlitUpdate_ScanCopy_Flip(u16 *ics, u32 *ict, int blkn);

__asm {
TKGDI_BlitUpdate_ScanCopy:

	CMP/GT	0, R6
	BF .done

#if 0
	.loop:
	MOV.Q	(R4,    0), R20
	MOV.Q	(R4,  640), R21
	MOV.Q	(R4, 1280), R22
	MOV.Q	(R4, 1920), R23
	MOV.Q	R20, (R5,  0)
	MOV.Q	R21, (R5,  8)
	ADD		-1, R6			| MOV.Q		R22, (R5, 16)
	ADD		8, R4			| MOV.Q		R23, (R5, 24)
	ADD		32, R5			| TEST		R6, R6
	BF .loop
#endif

#if 0
	SHLD	R6, -1, R6
	.loop:
							MOV.X	(R4,    0), R16
							MOV.X	(R4,  640), R18
							MOV.X	(R4, 1280), R20
							MOV.X	(R4, 1920), R22
							MOV.Q	R16, (R5,  0)
							MOV.Q	R18, (R5,  8)
							MOV.Q	R20, (R5, 16)
							MOV.Q	R22, (R5, 24)
							MOV.Q	R17, (R5, 32)
							MOV.Q	R19, (R5, 40)
	ADD		-1, R6		|	MOV.Q	R21, (R5, 48)
	ADD		16, R4		|	MOV.Q	R23, (R5, 56)
	ADD		64, R5		|	TEST	R6, R6
	BF .loop
#endif

#if 1
	SHLD	R6, -2, R6
	.loop:
							MOV.X	(R4,    0), R16
							MOV.X	(R4,  640), R18
							MOV.X	(R4, 1280), R20
							MOV.X	(R4, 1920), R22
							MOV.Q	R16, (R5,  0)
							MOV.Q	R18, (R5,  8)
							MOV.Q	R20, (R5, 16)
							MOV.Q	R22, (R5, 24)
							MOV.Q	R17, (R5, 32)
							MOV.Q	R19, (R5, 40)
							MOV.Q	R21, (R5, 48)
							MOV.Q	R23, (R5, 56)

							MOV.X	(R4,   16), R16
							MOV.X	(R4,  656), R18
							MOV.X	(R4, 1296), R20
							MOV.X	(R4, 1936), R22
							MOV.Q	R16, (R5,  64)
							MOV.Q	R18, (R5,  72)
							MOV.Q	R20, (R5,  80)
							MOV.Q	R22, (R5,  88)
							MOV.Q	R17, (R5,  96)
							MOV.Q	R19, (R5, 104)
	ADD		-1, R6		|	MOV.Q	R21, (R5, 112)
	ADD		32, R4		|	MOV.Q	R23, (R5, 120)

	ADD		128, R5		|	TEST	R6, R6
	BF .loop
#endif

	.done:
	RTS

TKGDI_BlitUpdate_ScanCopy_Flip:

	CMP/GT	0, R6
	BF .done

	.loop:
	MOV.Q	(R4, 1920), R20
	MOV.Q	(R4, 1280), R21
	MOV.Q	(R4,  640), R22
	MOV.Q	(R4,    0), R23
	MOV.Q	R20, (R5,  0)
	MOV.Q	R21, (R5,  8)
	ADD		-1, R6			| MOV.Q		R22, (R5, 16)
	ADD		8, R4			| MOV.Q		R23, (R5, 24)
	ADD		32, R5			| TEST		R6, R6
	BF .loop

	.done:
	RTSU
};

void TKGDI_BlitUpdate_ScanCopyGen(u16 *ics, u32 *ict, int blkn, int sbxs)
{
	u64 *cs0, *cs1, *cs2, *cs3;
	u64 *ct;
	int i;
	
	cs0=(u64 *)(ics       );
	cs1=(u64 *)(ics+  sbxs);
	cs2=(u64 *)(ics+2*sbxs);
	cs3=(u64 *)(ics+3*sbxs);
	ct=(u64 *)ict;
	for(i=0; i<blkn; i++)
	{
		ct[0]=*cs0++;
		ct[1]=*cs1++;
		ct[2]=*cs2++;
		ct[3]=*cs3++;
		ct+=4;
	}
}

void TKGDI_BlitUpdate_EncodeCell8x8x1(u16 *ics, u32 *ict, int sbxs)
{
	byte clry[64];
	u16 clrp[64];
	u16 clrm, clrn, pix;
	u64 pxb;
	int x, y, cy, mcy, ncy, acy;
	int cr0, cg0, cb0, cr1, cg1, cb1;
	int cr, cg, cb, acr, acg, acb;
	int bal;
	int i, j;

#if 1
	mcy=256; ncy=-1;
	acy=0;
	for(y=0; y<8; y++)
	{
		for(x=0; x<8; x++)
		{
			pix=ics[x];
//			cy=(pix>>2)&255;
			cy=((pix>>3)&127)+((pix>>8)&127);
			clry[(y<<3)|x]=cy;
//			clrp[(y<<3)|x]=pix;
//			acy+=cy;
			
			if(cy<mcy)
				{ clrm=pix; mcy=cy; }
			if(cy>ncy)
				{ clrn=pix; ncy=cy; }
		}
		ics+=sbxs;
	}

//	acy=acy>>6;
	acy=(mcy+ncy)>>1;
#endif
	
#if 0
	mcy=256; ncy=-1;
	acr=0;	acy=0;	acb=0;
	acy=0;
	for(y=0; y<8; y++)
	{
		for(x=0; x<8; x++)
		{
			pix=ics[x];
//			cy=(pix>>2)&255;
			cr=(pix>>10)&31;
			cg=(pix>> 5)&31;
			cb=(pix>> 0)&31;

//			cy=((pix>>3)&127)+((pix>>8)&127);
			cy=2*cg+cr+cb;

			clry[(y<<3)|x]=cy;
			clrp[(y<<3)|x]=pix;
			acy+=cy;

			acr+=cr;	acg+=cg;	acb+=cb;
			
			if(cy<mcy)
				{ cr0=cr; cg0=cg; cb0=cb; mcy=cy; }
			if(cy>ncy)
				{ cr1=cr; cg1=cg; cb1=cb; ncy=cy; }
		}
		ics+=sbxs;
	}
	
	acr=acr>>6;
	acg=acg>>6;
	acb=acb>>6;
//	acy=acy>>6;

	cr0=(3*cr0+acr)>>2;
	cg0=(3*cg0+acg)>>2;
	cb0=(3*cb0+acb)>>2;

	cr1=(3*cr1+acr)>>2;
	cg1=(3*cg1+acg)>>2;
	cb1=(3*cb1+acb)>>2;

	mcy=2*cg0+cr0+cb0;
	ncy=2*cg1+cr1+cb1;

	acy=(mcy+ncy)>>1;
	
	clrm=(cr0<<10)|(cg0<<5)|cb0;
	clrn=(cr1<<10)|(cg1<<5)|cb1;
#endif

	pxb=0;
	for(i=0; i<64; i++)
	{
		cy=clry[i];
		pxb=(pxb<<1)|(cy>=acy);
	}
	
	clrm&=0x7FFF;
	clrn&=0x7FFF;
	
//	ict[0]=0x80000000U|(clrm<<15)|clrn;
//	ict[1]=0x0000;
//	ict[2]=pxb;
//	ict[3]=pxb>>32;

	((u64 *)ict)[0]=0x80000000ULL|(clrm<<15)|clrn;
	((u64 *)ict)[1]=pxb;
//	ict[2]=pxb;
//	ict[3]=pxb>>32;
}

static byte tkgdi_enc2b_rcptab[256];
static byte tkgdi_enc2b_rcptab_init;

void TKGDI_BlitUpdate_FillEncode2Tab()
{
	int i, j, k;
	
	if(tkgdi_enc2b_rcptab_init)
		return;
	tkgdi_enc2b_rcptab_init=1;
	
	for(i=0; i<256; i++)
	{
		j=896/(i+1);
		tkgdi_enc2b_rcptab[i]=j;
	}
}

void TKGDI_BlitUpdate_EncodeCell4x4x2(
	u16 *ics, u32 *ict0, u32 *ict1, int sbxs)
{
	byte clry[16];
	byte *pclry;
	u16 clrm, clrn, pix;
	u64 pxb;
	int x, y, cy, mcy, ncy, acy, acy0, acy1, cybi;
	int cy0, cy1, cy2, cy3, rcp, pxc;
	int i, j, k;
	
	mcy=256; ncy=-1;
	for(y=0; y<4; y++)
	{
#if 0
		for(x=0; x<4; x++)
		{
			pix=ics[x];
//			cy=(pix>>2)&255;
//			cy=((pix>>3)&127)+((pix>>9)&63)+(pix&31);

			acy0=pix>>3;	acy1=pix>>8;
			acy0&=127;		acy1&=127;
			cy=acy0+acy1;

//			cy=((pix>>3)&127)+((pix>>8)&127);
			clry[(y<<2)|x]=cy;
			
			if(cy<mcy)
				{ clrm=pix; mcy=cy; }
			if(cy>ncy)
				{ clrn=pix; ncy=cy; }
		}
#endif

#if 1
			pxb=*(u64 *)ics;
			pclry=clry+(y<<2);

			acy0=pxb>>3;	acy1=pxb>>8;
			acy0&=127;		acy1&=127;
			pix=pxb>>16;	cy0=acy0+acy1;
			acy0=pix>>3;	acy1=pix>>8;
			acy0&=127;		acy1&=127;
			pix=pxb>>32;	cy1=acy0+acy1;
			acy0=pix>>3;	acy1=pix>>8;
			acy0&=127;		acy1&=127;
			pix=pxb>>48;	cy2=acy0+acy1;
			acy0=pix>>3;	acy1=pix>>8;
			acy0&=127;		acy1&=127;
			cy3=acy0+acy1;

			pclry[0]=cy0;	pclry[1]=cy1;
			pclry[2]=cy2;	pclry[3]=cy3;
			
			if(cy0<mcy)
				{ clrm=pxb; mcy=cy0; }
			if(cy0>ncy)
				{ clrn=pxb; ncy=cy0; }

			if(cy1<mcy)
				{ clrm=pxb>>16; mcy=cy1; }
			else if(cy1>ncy)
				{ clrn=pxb>>16; ncy=cy1; }

			if(cy2<mcy)
				{ clrm=pxb>>32; mcy=cy2; }
			else if(cy2>ncy)
				{ clrn=pxb>>32; ncy=cy2; }

			if(cy3<mcy)
				{ clrm=pxb>>48; mcy=cy3; }
			else if(cy3>ncy)
				{ clrn=pxb>>48; ncy=cy3; }
#endif

		ics+=sbxs;
	}
	
	acy=(mcy+ncy)>>1;
//	acy0=(mcy+acy)>>1;
//	acy1=(acy+ncy)>>1;
//	cybi=(acy1-acy)>>1;

#if 0
	for(i=0; i<16; i++)
	{
		cy=clry[i];
//		if(i&1)	cy+=cybi;
//		else	cy-=cybi;
		j=(cy>=acy0);
		k=(cy>=acy1);
		if(cy>=acy)
			j=k+2;
//		j=(cy>=acy);
//		j=j|(j<<1);
		pxb=(pxb<<2)|j;
	}
#endif

#if 1
	pxc=0;
	rcp=tkgdi_enc2b_rcptab[ncy-mcy];
	pclry=clry;
	for(i=0; i<4; i++)
	{
		cy0=pclry[0];	cy1=pclry[1];
		cy2=pclry[2];	cy3=pclry[3];
		pclry+=4;
		
//		cy0+=cybi;	cy1-=cybi;
//		cy2+=cybi;	cy3-=cybi;
				
		cy0-=acy;	cy1-=acy;
		cy2-=acy;	cy3-=acy;
		cy0*=rcp;	cy1*=rcp;
		cy2*=rcp;	cy3*=rcp;

//		cybi=-cybi;

		cy0=(cy0>>8)+2;		cy1=(cy1>>8)+2;
		cy2=(cy2>>8)+2;		cy3=(cy3>>8)+2;

//		pxb=(pxb<<2)|cy0;	pxb=(pxb<<2)|cy1;
//		pxb=(pxb<<2)|cy2;	pxb=(pxb<<2)|cy3;

		pxc=(pxc<<2)|cy0;	pxc=(pxc<<2)|cy1;
		pxc=(pxc<<2)|cy2;	pxc=(pxc<<2)|cy3;
	}
#endif
	
	clrm&=0x7FFF;
	clrn&=0x7FFF;
	
	*ict0=0xC0000000U|(clrm<<15)|clrn;
//	*ict1=pxb;
	*ict1=pxc;
}

u64 TKGDI_BlitUpdate_EncodeCellUTX2(u16 *ics, int sbxs)
{
	byte clry[16];
	byte *pclry;
	u16 clrm, clrn, pix;
	u64 pxb;
	int x, y, cy, mcy, ncy, acy, acy0, acy1, cybi;
	int cy0, cy1, cy2, cy3, rcp, pxc;
	int i, j, k;
	
	mcy=256; ncy=-1;
	for(y=0; y<4; y++)
	{
		pxb=*(u64 *)ics;
		pclry=clry+(y<<2);

		acy0=pxb>>3;	acy1=pxb>>8;
		acy0&=127;		acy1&=127;
		pix=pxb>>16;	cy0=acy0+acy1;
		acy0=pix>>3;	acy1=pix>>8;
		acy0&=127;		acy1&=127;
		pix=pxb>>32;	cy1=acy0+acy1;
		acy0=pix>>3;	acy1=pix>>8;
		acy0&=127;		acy1&=127;
		pix=pxb>>48;	cy2=acy0+acy1;
		acy0=pix>>3;	acy1=pix>>8;
		acy0&=127;		acy1&=127;
		cy3=acy0+acy1;

		pclry[0]=cy0;	pclry[1]=cy1;
		pclry[2]=cy2;	pclry[3]=cy3;
		
		if(cy0<mcy)
			{ clrm=pxb; mcy=cy0; }
		if(cy0>ncy)
			{ clrn=pxb; ncy=cy0; }

		if(cy1<mcy)
			{ clrm=pxb>>16; mcy=cy1; }
		else if(cy1>ncy)
			{ clrn=pxb>>16; ncy=cy1; }

		if(cy2<mcy)
			{ clrm=pxb>>32; mcy=cy2; }
		else if(cy2>ncy)
			{ clrn=pxb>>32; ncy=cy2; }

		if(cy3<mcy)
			{ clrm=pxb>>48; mcy=cy3; }
		else if(cy3>ncy)
			{ clrn=pxb>>48; ncy=cy3; }

		ics+=sbxs;
	}
	
	acy=(mcy+ncy)>>1;

	pxc=0;
	rcp=tkgdi_enc2b_rcptab[ncy-mcy];
	pclry=clry;
	for(i=0; i<4; i++)
	{
		cy0=pclry[0];	cy1=pclry[1];
		cy2=pclry[2];	cy3=pclry[3];
		pclry+=4;				

		cy0-=acy;	cy1-=acy;
		cy2-=acy;	cy3-=acy;
		cy0*=rcp;	cy1*=rcp;
		cy2*=rcp;	cy3*=rcp;

		cy0=(cy0>>8)+2;		cy1=(cy1>>8)+2;
		cy2=(cy2>>8)+2;		cy3=(cy3>>8)+2;

		pxc=(pxc<<2)|cy0;	pxc=(pxc<<2)|cy1;
		pxc=(pxc<<2)|cy2;	pxc=(pxc<<2)|cy3;
	}
	
	clrm&=0x7FFF;
	clrn&=0x7FFF;

	pxb=(((u64)pxc)<<32)|(clrm<<16)|clrn;
	return(pxb);
}

void TKGDI_BlitUpdate_EncodeCell8x8x2(u16 *ics, u32 *ict, int sbxs)
{
	u32 tict[8];
	u64 li0, li1, li2, li3;
	
	TKGDI_BlitUpdate_EncodeCell4x4x2(ics+0, tict+0, tict+4, sbxs);
	TKGDI_BlitUpdate_EncodeCell4x4x2(ics+4, tict+1, tict+5, sbxs);
	ics+=4*sbxs;
	TKGDI_BlitUpdate_EncodeCell4x4x2(ics+0, tict+2, tict+6, sbxs);
	TKGDI_BlitUpdate_EncodeCell4x4x2(ics+4, tict+3, tict+7, sbxs);
	
//	li0=(((u64)tict[1])<<32)|tict[0];
//	li1=(((u64)tict[3])<<32)|tict[2];
//	li2=(((u64)tict[5])<<32)|tict[4];
//	li3=(((u64)tict[7])<<32)|tict[6];

//	li0=(((u64)tict[3])<<32)|tict[2];
//	li1=(((u64)tict[1])<<32)|tict[0];
//	li2=(((u64)tict[7])<<32)|tict[6];
//	li3=(((u64)tict[5])<<32)|tict[4];

	li0=(((u64)tict[2])<<32)|tict[3];
	li1=(((u64)tict[0])<<32)|tict[1];
	li2=(((u64)tict[6])<<32)|tict[7];
	li3=(((u64)tict[4])<<32)|tict[5];
	
	((u64 *)ict)[0]=li0;
	((u64 *)ict)[1]=li1;
	((u64 *)ict)[2]=li2;
	((u64 *)ict)[3]=li3;
}

void TKGDI_BlitUpdate_ScanCellEncode128(u16 *ics, u32 *ict,
	int blkn, int sbxs)
{
	int i, j, k;
	
	for(i=0; i<blkn; i++)
	{
		TKGDI_BlitUpdate_EncodeCell8x8x1(ics, ict, sbxs);
		ics+=8;
		ict+=4;
	}
}

void TKGDI_BlitUpdate_ScanCellEncode256(u16 *ics, u32 *ict,
	int blkn, int sbxs)
{
	int i, j, k;
	
	for(i=0; i<blkn; i++)
	{
//		TKGDI_BlitUpdate_EncodeCell8x8x1(ics, ict, sbxs);
		TKGDI_BlitUpdate_EncodeCell8x8x2(ics, ict, sbxs);
		ics+=8;
		ict+=8;
	}
}

u32 __int32_bswap(u32 b);

void TKGDI_BlitUpdate_ScanCellTransUTX2(u64 *ics, u32 *ict,
	int blkn, int sbxs)
{
	u64 *cs0, *cs1, *ct;
	u64 b0, b1, b2, b3;
	u64 c0, c1, c2, c3;
	u32 vi0, vi1, vi2, vi3, pi0, pi1, pi2, pi3;
	int i, j, k;
	
	cs0=ics;
	cs1=ics+((sbxs+3)>>2);
	ct=(u64 *)ict;
	
	for(i=0; i<blkn; i++)
	{
//		b0=cs0[0];	b1=cs0[1];
//		b2=cs1[0];	b3=cs1[1];

		b3=cs0[0];	b2=cs0[1];
		b1=cs1[0];	b0=cs1[1];
		cs0+=2;		cs1+=2;
		vi0=0xC0000000U|((b0>>1)&0x3FFF8000)|(b0&0x00007FFF);
		vi1=0xC0000000U|((b1>>1)&0x3FFF8000)|(b1&0x00007FFF);
		vi2=0xC0000000U|((b2>>1)&0x3FFF8000)|(b2&0x00007FFF);
		vi3=0xC0000000U|((b3>>1)&0x3FFF8000)|(b3&0x00007FFF);

		pi0=b0>>32;		pi1=b1>>32;
		pi2=b2>>32;		pi3=b3>>32;
		if(sbxs<0)
		{
			pi0=__int32_bswap(pi0);
			pi1=__int32_bswap(pi1);
			pi2=__int32_bswap(pi2);
			pi3=__int32_bswap(pi3);
		}

		c0=(((u64)vi1)<<32)|vi0;
		c1=(((u64)vi3)<<32)|vi2;
		c2=(((u64)pi1)<<32)|pi0;
		c3=(((u64)pi3)<<32)|pi2;
		ct[0]=c0;	ct[1]=c1;
		ct[2]=c2;	ct[3]=c3;
		ct+=4;
	}
}

void TKGDI_BlitUpdate_ScanCellTransUTX2_Mask(
	u64 *ics, u32 *ict, byte *bmcs,
	int blkn, int sbxs)
{
	u64 *cs0, *cs1, *ct;
	u64 b0, b1, b2, b3;
	u64 c0, c1, c2, c3;
	u32 vi0, vi1, vi2, vi3, pi0, pi1, pi2, pi3;
	int i, j, k;
	
	cs0=ics;
	cs1=ics+((sbxs+3)>>2);
	ct=(u64 *)ict;
	
	for(i=0; i<blkn; i++)
	{
		if(!(bmcs[i>>3]&(1<<(i&7))))
		{
			cs0+=2;		cs1+=2;
			ct+=4;
			continue;
		}

//		b0=cs0[0];	b1=cs0[1];
//		b2=cs1[0];	b3=cs1[1];

		b3=cs0[0];	b2=cs0[1];
		b1=cs1[0];	b0=cs1[1];
		cs0+=2;		cs1+=2;
		vi0=0xC0000000U|((b0>>1)&0x3FFF8000)|(b0&0x00007FFF);
		vi1=0xC0000000U|((b1>>1)&0x3FFF8000)|(b1&0x00007FFF);
		vi2=0xC0000000U|((b2>>1)&0x3FFF8000)|(b2&0x00007FFF);
		vi3=0xC0000000U|((b3>>1)&0x3FFF8000)|(b3&0x00007FFF);

		pi0=b0>>32;		pi1=b1>>32;
		pi2=b2>>32;		pi3=b3>>32;
		if(sbxs<0)
		{
			pi0=__int32_bswap(pi0);
			pi1=__int32_bswap(pi1);
			pi2=__int32_bswap(pi2);
			pi3=__int32_bswap(pi3);
		}

		c0=(((u64)vi1)<<32)|vi0;
		c1=(((u64)vi3)<<32)|vi2;
		c2=(((u64)pi1)<<32)|pi0;
		c3=(((u64)pi3)<<32)|pi2;
		ct[0]=c0;	ct[1]=c1;
		ct[2]=c2;	ct[3]=c3;
		ct+=4;
	}
}

/* For the 320x200 hi-color mode. */
int TKGDI_BlitUpdate_BlkRgb555(
	int dxo, int dyo, int dxs, int dys,
	u16 *sbuf,
	int sbxo, int sbyo,
	int sbxs, int sbys)
{
	u32 *conbufa, *conbufb, *conbufb2;
	u32 *ict;
	u16 *ics;
	int bym, byn, bxm, bxn;
	int bx, by, flip;

	flip=1;
	if(sbys<0)
	{
		sbys=-sbys;
		flip=!flip;
	}

	__hint_use_egpr();

//	conbufa=(u32 *)0xA00A0000;
	conbufa=(u32 *)0xFFFFF00A0000ULL;
//	conbufb=conbufa+(80*61);

//	conbufb=(u32 *)0x0000080A0000ULL;
	conbufb =(u32 *)0xC000200A0000ULL;		//RAM backed framebuffer
	conbufb2=(u32 *)0xD000200A0000ULL;		//Volatile / No Cache

//	((u32 *)0xFFFFF00BFF00ULL)[8]=tkgdi_vid_frnum;
//	tkgdi_vid_frnum++;

	conbufa[0]=tkgdi_vid_frnum;
	if(conbufb2[0]==tkgdi_vid_frnum)		//Detect if MMIO maps here.
		conbufa=conbufb;

	bxm=dxo>>2;
	bxn=(dxo+dxs)>>2;
	bym=dyo>>2;
	byn=(dyo+dys)>>2;

	ics=sbuf+(sbyo*sbxs)+sbxo;
	ict=conbufa+((bym*80+bxm)*8);
//	ict=conbufa;

	if(flip)
	{
//		ics+=((dys>>4)-1)*(4*sbxs);
//		ics+=(sbys-1)*sbxs;
		ics=sbuf+((sbys-sbyo-1)*sbxs)+sbxo;

		if(sbxs==320)
		{
			ics-=3*320;
		}
	}
	
	for(by=bym; by<byn; by++)
	{
		if(flip)
		{
			if(sbxs==320)
				TKGDI_BlitUpdate_ScanCopy_Flip(ics, ict, dxs>>2);
			else
				TKGDI_BlitUpdate_ScanCopyGen(ics, ict, dxs>>2, -sbxs);
			ics-=4*sbxs;
		}else
		{
			if(sbxs==320)
				TKGDI_BlitUpdate_ScanCopy(ics, ict, dxs>>2);
			else
				TKGDI_BlitUpdate_ScanCopyGen(ics, ict, dxs>>2, sbxs);
			ics+=4*sbxs;
		}
//		ict+=80*8;
		ict+=tkgdi_vid_rowstride;
	}

	((u32 *)0xFFFFF00BFF00ULL)[8]=tkgdi_vid_frnum;
	tkgdi_vid_frnum++;

	return(0);
}


/* For RGB555 to Color-Cell Mode. */
int TKGDI_BlitUpdate_BlkRgb555_CCE(
	int dxo, int dyo, int dxs, int dys,
	u16 *sbuf,
	int sbxo, int sbyo,
	int sbxs, int sbys)
{
	u32 *conbufa, *conbufb, *conbufb2;
	u32 *ict;
	u16 *ics;
	int bym, byn, bxm, bxn;
	int bx, by, flip;

	__hint_use_egpr();

	flip=1;
	if(sbys<0)
	{
		sbys=-sbys;
		flip=!flip;
	}
	
//	tk_printf("TKGDI_BlitUpdate_BlkRgb555_CCE: %d %d %d %d  %d %d %d %d\n",
//		dxo, dyo, dxs, dys,
//		sbxo, sbyo, sbxs, sbys);

	TKGDI_BlitUpdate_FillEncode2Tab();

	conbufa=(u32 *)0xFFFFF00A0000ULL;
	conbufb =(u32 *)0xC000200A0000ULL;		//RAM backed framebuffer
	conbufb2=(u32 *)0xD000200A0000ULL;		//Volatile / No Cache

//	((u32 *)0xFFFFF00BFF00ULL)[8]=tkgdi_vid_frnum;
//	tkgdi_vid_frnum++;

	conbufa[0]=tkgdi_vid_frnum;
	if(conbufb2[0]==tkgdi_vid_frnum)		//Detect if MMIO maps here.
		conbufa=conbufb;

	if(!(((long)conbufa)>>16))
		__debugbreak();

	bxm=dxo>>3;
	bxn=(dxo+dxs)>>3;
	bym=dyo>>3;
	byn=(dyo+dys)>>3;

//	tk_printf("  %d %d  %d %d\n", bxm, bxn, bym, byn);
//	tk_printf("  %p %p \n", conbufa, sbuf);

	ics=sbuf+(sbyo*sbxs)+sbxo;
//	ict=conbufa+((bym*80+bxm)*8);
	ict=conbufa+((bym*tkgdi_vid_rowstride)+(bxm*tkgdi_vid_cellstride));
	if(flip)
	{
//		ics+=((dys>>4)-1)*(4*sbxs);
//		ics+=(dys-1)*sbxs;
//		ics+=(sbys-1)*sbxs;
		ics=sbuf+((sbys-sbyo-1)*sbxs)+sbxo;
	}
	
	for(by=bym; by<byn; by++)
	{
		if(flip)
		{
			if(tkgdi_vid_cellstride>=8)
				TKGDI_BlitUpdate_ScanCellEncode256(ics, ict, dxs>>3, -sbxs);
			else
				TKGDI_BlitUpdate_ScanCellEncode128(ics, ict, dxs>>3, -sbxs);
			ics-=8*sbxs;
		}else
		{
			if(tkgdi_vid_cellstride>=8)
				TKGDI_BlitUpdate_ScanCellEncode256(ics, ict, dxs>>3, sbxs);
			else
				TKGDI_BlitUpdate_ScanCellEncode128(ics, ict, dxs>>3, sbxs);
			ics+=8*sbxs;
		}
//		ict+=80*8;
		ict+=tkgdi_vid_rowstride;
	}

	((u32 *)0xFFFFF00BFF00ULL)[8]=tkgdi_vid_frnum;
	tkgdi_vid_frnum++;
	
	return(0);
}

/* For UTX2 to Color-Cell Mode. */
int TKGDI_BlitUpdate_BlkUtx2_CCT(
	int dxo, int dyo, int dxs, int dys,
	u64 *sbuf,
	int sbxo, int sbyo,
	int sbxs, int sbys)
{
	u32 *conbufa, *conbufb, *conbufb2;
	u32 *ict;
	u64 *ics;
	int bym, byn, bxm, bxn;
	int bx, by, flip;

	flip=1;
	if(sbys<0)
	{
		sbys=-sbys;
		flip=!flip;
	}

	conbufa=(u32 *)0xFFFFF00A0000ULL;
	conbufb =(u32 *)0xC000200A0000ULL;		//RAM backed framebuffer
	conbufb2=(u32 *)0xD000200A0000ULL;		//Volatile / No Cache

	conbufa[0]=tkgdi_vid_frnum;
	if(conbufb2[0]==tkgdi_vid_frnum)		//Detect if MMIO maps here.
		conbufa=conbufb;

	bxm=dxo>>3;
	bxn=(dxo+dxs)>>3;
	bym=dyo>>3;
	byn=(dyo+dys)>>3;

	ics=sbuf+((sbyo>>2)*(sbxs>>2))+(sbxo>>2);
	ict=conbufa+((bym*80+bxm)*8);
	if(flip)
	{
//		ics+=((dys>>4)-1)*(sbxs>>2);
		ics+=((dys>>2)-1)*(sbxs>>2);
	}
	
	for(by=bym; by<byn; by++)
	{
		if(flip)
		{
			TKGDI_BlitUpdate_ScanCellTransUTX2(ics, ict, dxs>>3, -sbxs);
			ics-=2*(sbxs>>2);
		}else
		{
			TKGDI_BlitUpdate_ScanCellTransUTX2(ics, ict, dxs>>3, sbxs);
			ics+=2*(sbxs>>2);
		}
//		ict+=80*8;
		ict+=tkgdi_vid_rowstride;
	}
	
	((u32 *)0xFFFFF00BFF00ULL)[8]=tkgdi_vid_frnum;
	tkgdi_vid_frnum++;

	return(0);
}

/* For UTX2 to Color-Cell Mode. */
int TKGDI_BlitUpdate_BlkUtx2_CCT_Mask(
	int dxo, int dyo, int dxs, int dys,
	u64 *sbuf, byte *bmbuf,
	int sbxo, int sbyo,
	int sbxs, int sbys)
{
	u32 *conbufa, *conbufb, *conbufb2;
	u32 *ict;
	u64 *ics;
	byte *bmcs;
	int bym, byn, bxm, bxn, bmxs;
	int bx, by, flip;

	flip=1;
	if(sbys<0)
	{
		sbys=-sbys;
		flip=!flip;
	}

	conbufa=(u32 *)0xFFFFF00A0000ULL;
	conbufb =(u32 *)0xC000200A0000ULL;		//RAM backed framebuffer
	conbufb2=(u32 *)0xD000200A0000ULL;		//Volatile / No Cache

	conbufa[0]=tkgdi_vid_frnum;
	if(conbufb2[0]==tkgdi_vid_frnum)		//Detect if MMIO maps here.
		conbufa=conbufb;

	bxm=dxo>>3;
	bxn=(dxo+dxs)>>3;
	bym=dyo>>3;
	byn=(dyo+dys)>>3;
	
	bmxs=(((dxs+7)>>3)+7)>>3;

	ics=sbuf+((sbyo>>2)*(sbxs>>2))+(sbxo>>2);
	ict=conbufa+((bym*80+bxm)*8);
	bmcs=bmbuf;
	if(flip)
	{
//		ics+=((dys>>4)-1)*(sbxs>>2);
		ics+=((dys>>2)-1)*(sbxs>>2);
		bmcs+=((dys>>2)-1)*bmxs;
	}
	
	for(by=bym; by<byn; by++)
	{
		if(flip)
		{
			TKGDI_BlitUpdate_ScanCellTransUTX2_Mask(
				ics, ict, bmcs, dxs>>3, -sbxs);
			ics-=2*(sbxs>>2);
			bmcs-=bmxs;
		}else
		{
			TKGDI_BlitUpdate_ScanCellTransUTX2_Mask(
				ics, ict, bmcs, dxs>>3, sbxs);
			ics+=2*(sbxs>>2);
			bmcs+=bmxs;
		}
//		ict+=80*8;
		ict+=tkgdi_vid_rowstride;
	}
	
	((u32 *)0xFFFFF00BFF00ULL)[8]=tkgdi_vid_frnum;
	tkgdi_vid_frnum++;

	return(0);
}

/* Existing Windows */
_tkgdi_window_t		*tkgdi_windows[256];
int					tkgdi_n_windows;

/* Visible Windows */
_tkgdi_window_t		*tkgdi_window_vis[256];
int					tkgdi_n_window_vis;

int TKGDI_UpdateWindowCells(
	_tkgdi_window_t *wctx)
{
	u64 *ct, *ct1;
	u16 *cs, *cs1;
	byte *bm1, *bm2;
	int x, y, b, xs, ys, bxs, bys, bmxs;

//	tk_printf("TKGDI_UpdateWindowCells: A\n");

	if(!(wctx->dirty1))
		return(0);

	TKGDI_BlitUpdate_FillEncode2Tab();

//	tk_printf("TKGDI_UpdateWindowCells: B\n");

	xs=wctx->size_x;
	ys=wctx->size_y;
	bxs=wctx->size_bxs;
	bys=wctx->size_bys;
	bm1=wctx->buf_dirty1;
	bm2=wctx->buf_dirty2;
	bmxs=wctx->size_bmxs;
	
	cs=wctx->buf_data;
	ct=wctx->buf_utx2;
	
	for(y=0; y<bys; y++)
	{
		cs1=cs;
//		ct1=ct;
		for(x=0; x<bxs; x++)
		{
			b=bm1[x>>3];
			if(!b)
				{ x=((x+8)&(~7))-1; continue; }
			if(!(b&(1<<(x&7))))
				continue;
			ct[x]=TKGDI_BlitUpdate_EncodeCellUTX2(cs1, xs);
			bm1[x>>3]=b&(~(1<<(x&7)));
			bm2[x>>3]|= (1<<(x&7));
			cs1+=4;
		}
		cs+=xs*4;
		ct+=bxs;
		bm1+=bmxs;
		bm2+=bmxs;
	}
	
	wctx->dirty1=0;
	wctx->dirty2=1;
	return(1);
}

int TKGDI_ScreenMarkDirty(void)
{
	int xs, ys, bxs, bys, bxs2, bys2, bmsz;

	xs=tkgdi_vid_xsize;
	ys=tkgdi_vid_ysize;
	bxs2=(xs+7)>>3;
	bys2=(ys+7)>>3;
	bmsz=(bxs2*bys2+7)>>3;

	memset(tkgdi_vid_screendirty, 0xFF, bmsz);
	
	return(0);
}

int TKGDI_UpdateWindowStack(void)
{
	_tkgdi_window_t *wctx;
	u64 *wutx, *wucs, *wuct;
	u64 blkb;
	int xs, ys, bxs, bys, bxs2, bys2, bmsz, bmxs2;
	int x, y, z, z1, bx, by, wxs, wys, wbmxs;
	int sx, sy, z0a, z1a;
	int i, j, k;

	xs=tkgdi_vid_xsize;
	ys=tkgdi_vid_ysize;
	bxs2=(xs+7)>>3;
	bys2=(ys+7)>>3;

//	bmsz=(bxs2*bys2+7)>>3;

	tkgdi_vid_bmxsize=(bxs2+7)>>3;
	bmxs2=tkgdi_vid_bmxsize<<3;
	bmsz=tkgdi_vid_bmxsize*bys2;
	
	bxs=bxs2<<1;
	bys=bys2<<1;

	if(!tkgdi_vid_screenutx)
	{
		tkgdi_vid_screenutx=tk_malloc(bxs2*bys2*(4*8));
		tkgdi_vid_screendirty=tk_malloc(bmsz);
		memset(tkgdi_vid_screendirty, 0xFF, bmsz);
	}
	
//	blkb=0;
	blkb=0x55AA55AA5555AAAAULL;
	/* Clear any dirty blocks with background color. */
	for(y=0; y<bys2; y++)
		for(x=0; x<bxs2; x++)
	{
//		z=y*bxs2+x;
		z=y*bmxs2+x;
		if(!(tkgdi_vid_screendirty[z>>3]&(1<<(z&7))))
			continue;
		tkgdi_vid_screenutx[(((y<<1)+0)*bxs)+((x<<1)+0)]=blkb;
		tkgdi_vid_screenutx[(((y<<1)+0)*bxs)+((x<<1)+1)]=blkb;
		tkgdi_vid_screenutx[(((y<<1)+1)*bxs)+((x<<1)+0)]=blkb;
		tkgdi_vid_screenutx[(((y<<1)+1)*bxs)+((x<<1)+1)]=blkb;
	}
	
	for(i=0; i<tkgdi_n_window_vis; i++)
	{
		wctx=tkgdi_window_vis[i];
		
		bx=wctx->base_x>>2;
		by=wctx->base_y>>2;
		wxs=wctx->size_bxs;
		wys=wctx->size_bys;
		wbmxs=wctx->size_bmxs;
		
		wutx=wctx->buf_utx2;
		for(y=0; y<wys; y++)
		{
			sy=by+y;
			if(sy<0)
				continue;
			if(sy>=bxs)
				continue;

			wucs=wutx+(y*wxs);
			wuct=tkgdi_vid_screenutx+(sy*bxs+bx);
			z0a=(sy>>1)*bmxs2+(bx>>1);
			z1a=y*(wbmxs<<3);

			for(x=0; x<wxs; x++)
			{
				sx=bx+x;
//				if((sx<0) || (sy<0))
				if(sx<0)
					continue;
//				if((sx>=bxs) || (sy>=bxs))
				if(sx>=bxs)
					continue;

	//			z=(sy>>1)*bxs2+(sx>>1);
//				z=(sy>>1)*bmxs2+(sx>>1);
//				z1=y*(wbmxs<<3)+x;
				z=z0a+(x>>1);
				z1=z1a+x;
				
				if(!(tkgdi_vid_screendirty[z>>3]&(1<<(z&7))))
				{
					if(!(wctx->dirty2))
						continue;
					if(!(wctx->buf_dirty2[z1>>3]&(1<<(z1&7))))
						continue;
				}

//				blkb=wutx[y*wxs+x];
				blkb=wucs[x];
				
	//			blkb=0x55AA55AA7FFF0000ULL;
				
//				tkgdi_vid_screenutx[sy*bxs+sx]=blkb;
				wuct[x]=blkb;

				tkgdi_vid_screendirty[z>>3]|=(1<<(z&7));
			}
		}
		
		memset(wctx->buf_dirty2, 0, wctx->size_bmsz);
		wctx->dirty2=0;
	}
	
	TKGDI_BlitUpdate_BlkUtx2_CCT_Mask(
		0, 0, xs, ys,
		tkgdi_vid_screenutx, tkgdi_vid_screendirty,
		0, 0, xs, -ys);
	memset(tkgdi_vid_screendirty, 0, bmsz);
	
	return(1);
}

_tkgdi_window_t *TKGDI_AllocNewWindow(
	_tkgdi_context_t *ctx)
{
	_tkgdi_window_t *wctx;
	int i, j, k;

	for(i=2; i<tkgdi_n_windows; i++)
	{
		if(!tkgdi_windows[i])
			break;
	}
	
	if(i>=tkgdi_n_windows)
	{
		i=tkgdi_n_windows++;
		while(i<2)
			i=tkgdi_n_windows++;
		tkgdi_windows[i]=NULL;
	}
	
	wctx=tk_malloc(sizeof(_tkgdi_window_t));
	memset(wctx, 0, sizeof(_tkgdi_window_t));
	
	wctx->idx=i;
	tkgdi_windows[i]=wctx;
	
	wctx->buf_info=&(wctx->t_buf_info);
	
	return(wctx);
}

TKGSTATUS TKGDI_BlitSubImageNew(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGDI_RRECT *drect,
//	int xo_dev, int yo_dev,
	TKGDI_BITMAPINFOHEADER *info, void *data,
	TKGDI_RRECT *rect)
//	int xo_src, int yo_src, int xs_src, int ys_src)
{
	_tkgdi_window_t *wctx;
	byte *bmct;
	u16 *cs, *ct;
	int xo_dev, yo_dev;
	int xo_src, yo_src, xs_src, ys_src;
	int xs, ys, mxs, mys, bxs, bys, nx, ny;
	int i, j, k;

	if(dev<=0)
		return(-1);
	
	xo_dev=drect->xorg;
	yo_dev=drect->yorg;
	
	xo_src=rect->xorg;
	yo_src=rect->yorg;
	xs_src=rect->xsize;
	ys_src=rect->ysize;
	
	if(xs_src<0)
		xs_src=-xs_src;
	if(ys_src<0)
		ys_src=-ys_src;
	
//	return(-1);
		
	if(dev==1)
	{
		if(tkgdi_vid_scrmode==TKGDI_SCRMODE_320x200_RGB555)
		{
			xs=xs_src;
			ys=ys_src;
			
			mxs=320-xo_dev;
			mys=200-yo_dev;
			
			if(xs>mxs)
				xs=mxs;
			if(ys>mys)
				ys=mys;

			TKGDI_BlitUpdate_BlkRgb555(xo_dev, yo_dev, xs, ys,
				data, xo_src, yo_src, info->biWidth, info->biHeight);
		}

//		if(tkgdi_vid_scrmode==TKGDI_SCRMODE_640x400_CC)
		if(	(tkgdi_vid_scrmode==TKGDI_SCRMODE_TEXT)			||
			(tkgdi_vid_scrmode==TKGDI_SCRMODE_640x400_CC)	||
			(tkgdi_vid_scrmode==TKGDI_SCRMODE_800x600_CC)	)
		{
			xs=xs_src;
			ys=ys_src;
			
//			mxs=640-xo_dev;
			mxs=tkgdi_vid_xsize-xo_dev;
//			mys=400-yo_dev;
			mys=tkgdi_vid_ysize-yo_dev;
			if(xs>mxs)
				xs=mxs;
			if(ys>mys)
				ys=mys;

			if(info->biCompression==0)
			{
				TKGDI_BlitUpdate_BlkRgb555_CCE(xo_dev, yo_dev, xs, ys,
					data, xo_src, yo_src, info->biWidth, info->biHeight);
			}

			if(info->biCompression==TKGDI_FCC_UTX2)
			{
				TKGDI_BlitUpdate_BlkUtx2_CCT(xo_dev, yo_dev, xs, ys,
					data, xo_src, yo_src, info->biWidth, info->biHeight);
			}
		}

#if 0
		if(tkgdi_vid_scrmode==TKGDI_SCRMODE_TEXT)
		{
			xs=xs_src;
			ys=ys_src;
			
			mxs=640-xo_dev;
			mys=200-yo_dev;
			if(xs>mxs)
				xs=mxs;
			if(ys>mys)
				ys=mys;

			if(info->biCompression==0)
			{
				TKGDI_BlitUpdate_BlkRgb555_CCE(xo_dev, yo_dev, xs, ys,
					data, xo_src, yo_src, info->biWidth, info->biHeight);
			}

			if(info->biCompression==TKGDI_FCC_UTX2)
			{
				TKGDI_BlitUpdate_BlkUtx2_CCT(xo_dev, yo_dev, xs, ys,
					data, xo_src, yo_src, info->biWidth, info->biHeight);
			}
		}
#endif

		return(0);
	}
	
	if((dev>1) && (dev<tkgdi_n_windows))
	{
		wctx=tkgdi_windows[dev];
		
		if(!wctx || !wctx->buf_info)
		{
			tk_printf("TKGDI_BlitSubImage: Bad wctx=%p\n",
				wctx);
			return(-1);
		}

		xs=xs_src;
		ys=ys_src;
		
		bxs=wctx->buf_info->biWidth;
		bys=wctx->buf_info->biHeight;
		mxs=bxs-xo_dev;
		mys=bys-yo_dev;
		if(xs>mxs)
			xs=mxs;
		if(ys>mys)
			ys=mys;
			
		cs=data;
		ct=wctx->buf_data;
		
		ct+=yo_dev*bxs+xo_dev;
		for(i=0; i<ys; i++)
		{
			memcpy(ct, cs, xs*2);
			cs+=xs_src;
			ct+=bxs;
		}
		
		nx=(xo_dev+xs+3)>>2;
		ny=(yo_dev+ys+3)>>2;

		for(i=(yo_dev>>2); i<ny; i++)
		{
			bmct=wctx->buf_dirty1+(i*wctx->size_bmxs);
			for(j=(xo_dev>>2); j<nx; j++)
			{
				if(!(j&7) && ((j+8)<=nx))
				{
					bmct[j>>3]=0xFF;
					j+=7;
					continue;
				}
				bmct[j>>3]|=1<<(j&7);
			}
		}
		wctx->dirty1=1;

		TKGDI_UpdateWindowCells(wctx);
		TKGDI_UpdateWindowStack();

		return(0);
	}
	
	tk_printf("TKGDI_BlitSubImage: Bad hDc=%d max=%d\n",
		dev, tkgdi_n_windows);

	return(-1);
}

TKGSTATUS TKGDI_BlitSubImageOld(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	int xo_dev, int yo_dev,
	TKGDI_BITMAPINFOHEADER *info, void *data,
	int xo_src, int yo_src, int xs_src, int ys_src)
{
	TKGDI_RRECT t_rect, t_drect;
	TKGDI_RRECT *rect;
	TKGDI_RRECT *drect;
	
	rect=&t_rect;
	drect=&t_drect;
	rect->xorg=xo_src;
	rect->yorg=yo_src;
	rect->xsize=xs_src;
	rect->ysize=ys_src;

	drect->xorg=xo_dev;
	drect->yorg=yo_dev;
	drect->xsize=xs_src;
	drect->ysize=ys_src;
	
	return(ctx->vt->BlitSubImageNew(ctx,
		dev,
		drect,
		info, data,
		rect));
}

int TKGDI_ModeForInputFormat(TKGDI_BITMAPINFOHEADER *ifmt)
{
	int ofmt_mode;

	if(!ifmt)
	{
		tk_printf("TKGDI_ModeForInputFormat: Got NULL\n");
		return(-1);
	}

	ofmt_mode=-1;

	tk_printf("TKGDI_ModeForInputFormat: W=%d H=%d bpp=%d\n",
		ifmt->biWidth, ifmt->biHeight, ifmt->biBitCount);

	if((ifmt->biBitCount == 16) || (ifmt->biBitCount == 15))
	{

		if(	(ifmt->biWidth		== 320) &&
			(ifmt->biHeight		== 200) )
		{
			ofmt_mode=TKGDI_SCRMODE_320x200_RGB555;		//Use hi-color
		}

		if(	(ifmt->biWidth		== 320) &&
			(ifmt->biHeight		== 200) )
		{
			ofmt_mode=TKGDI_SCRMODE_320x200_RGB555;		//Use hi-color
		}

		if(	(ifmt->biWidth		== 640) &&
			(ifmt->biHeight		== 200) )
		{
			ofmt_mode=TKGDI_SCRMODE_TEXT;		//Use 80x25 color cell
		}

		if(	(ifmt->biWidth		== 640) &&
			(ifmt->biHeight		== 400) )
		{
			ofmt_mode=TKGDI_SCRMODE_640x400_CC;	//Use 80x50 color cell
		}
		
		if(	(ifmt->biWidth		== 800) &&
			(ifmt->biHeight		== 600) )
		{
			ofmt_mode=TKGDI_SCRMODE_800x600_CC;	//Use 100x75 color cell
		}
	}
	
	return(ofmt_mode);
}

TKGSTATUS TKGDI_QueryCreateDisplay(
	_tkgdi_context_t *ctx,
	TKGDI_BITMAPINFOHEADER *ifmt,
	TKGDI_BITMAPINFOHEADER *ofmt)
{
	int ofmt_mode;

	if(ifmt && ofmt)
	{
		return(-1);
	}

	ofmt_mode=tkgdi_vid_scrmode;

	if(ifmt)
	{
		ofmt_mode=TKGDI_ModeForInputFormat(ifmt);
	}

	if(ofmt)
	{
		ofmt->biSize=sizeof(TKGDI_BITMAPINFOHEADER);
		ofmt->biWidth=640;
		ofmt->biHeight=200;
		ofmt->biBitCount=16;
		ofmt->biPlanes=1;
		ofmt->biCompression=0;
		ofmt->biSizeImage=ofmt->biWidth*ofmt->biHeight*2;
		ofmt->biXPelsPerMeter=72;
		ofmt->biYPelsPerMeter=72;
		ofmt->biClrUsed=0;
		ofmt->biClrImportant=0;

		if(ofmt_mode<0)
			return(0);

		if(ofmt_mode==TKGDI_SCRMODE_TEXT)
		{
			ofmt->biWidth=640;
			ofmt->biHeight=200;
//			ofmt->biBitCount=16;
		}

		if(ofmt_mode==TKGDI_SCRMODE_640x400_CC)
		{
			ofmt->biWidth=640;
			ofmt->biHeight=400;
//			ofmt->biBitCount=16;
//			ofmt->biCompression=TKGDI_FCC_UTX2;
		}

		if(ofmt_mode==TKGDI_SCRMODE_800x600_CC)
		{
			ofmt->biWidth=640;
			ofmt->biHeight=400;
//			ofmt->biBitCount=16;
//			ofmt->biCompression=TKGDI_FCC_UTX2;
		}

		if(ofmt_mode==TKGDI_SCRMODE_320x200_RGB555)
		{
			ofmt->biWidth=320;
			ofmt->biHeight=200;
//			ofmt->biBitCount=16;
		}

		ofmt->biSizeImage=
			ofmt->biWidth*
			ofmt->biHeight*
			((ofmt->biBitCount+7)>>3);

		return(1);
	}
	
	return(0);
}

TKGSTATUS TKGDI_QueryDisplay(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGFOURCC parm,
	void *ifmt,
	void *ofmt)
{
	if(parm==TKGDI_FCC_crea)
	{
		return(TKGDI_QueryCreateDisplay(ctx, ifmt, ofmt));
	}
	
	return(-1);
}

TKGHDC TKGDI_CreateDisplay(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGFOURCC parm,
	TKGDI_BITMAPINFOHEADER *info)
{
	_tkgdi_window_t *wctx;
	int tgt_mode, domodeset;
	int xs, ys, bxs, bys;
	int i, j, k;

	tk_printf("TKGDI_CreateDisplay: A\n");

	if((tkgdi_vid_scrmode<0) || (tkgdi_vid_scrmode>16))
		tkgdi_vid_scrmode=0;

	tgt_mode=TKGDI_ModeForInputFormat(info);
	
	domodeset=0;
	if(tkgdi_vid_scrmode==TKGDI_SCRMODE_TEXT)
	{
		if(tgt_mode>0)
		{
			domodeset=1;
		}
	}
	
	if(tgt_mode==TKGDI_SCRMODE_TEXT)
	{
		if(tkgdi_vid_scrmode!=TKGDI_SCRMODE_TEXT)
		{
			domodeset=1;
		}
	}

	if((dev<=0) && domodeset)
	{
		/* Fullscreen modes. */
	

		if(tgt_mode==TKGDI_SCRMODE_TEXT)
		{
			((u32 *)0xFFFFF00BFF00UL)[0]=0x0081;	//80x25 color-cell
			tkgdi_vid_scrmode=tgt_mode;
			tkgdi_vid_cellstride=8;
			tkgdi_vid_rowstride=80*8;
			tkgdi_vid_xsize=640;
			tkgdi_vid_ysize=200;
			return(1);
		}

		if(tgt_mode==TKGDI_SCRMODE_640x400_CC)
		{
			((u32 *)0xFFFFF00BFF00UL)[0]=0x0085;	//80x50 color-cell
			tkgdi_vid_scrmode=tgt_mode;
			tkgdi_vid_cellstride=8;
			tkgdi_vid_rowstride=80*8;
			tkgdi_vid_xsize=640;
			tkgdi_vid_ysize=400;
			return(1);
		}

		if(tgt_mode==TKGDI_SCRMODE_800x600_CC)
		{
			((u32 *)0xFFFFF00BFF00UL)[0]=0x008F;	//100x75 color-cell
			tkgdi_vid_scrmode=tgt_mode;
			tkgdi_vid_cellstride=4;
			tkgdi_vid_rowstride=100*4;
			tkgdi_vid_xsize=800;
			tkgdi_vid_ysize=600;
			return(1);
		}

		if(tgt_mode==TKGDI_SCRMODE_320x200_RGB555)
		{
			((u32 *)0xFFFFF00BFF00UL)[0]=0x0095;	//320x200x16bpp, RGB555
			tkgdi_vid_scrmode=tgt_mode;
			tkgdi_vid_cellstride=8;
			tkgdi_vid_rowstride=80*8;
			tkgdi_vid_xsize=320;
			tkgdi_vid_ysize=200;
			return(1);
		}

		tk_printf("TKGDI_CreateDisplay: Invalid Mode\n");
		return(TKGHDC_NULL);
	}

	if(dev<=1)
	{
		wctx=TKGDI_AllocNewWindow(ctx);
		
		xs=info->biWidth;
		ys=info->biHeight;

		if(xs<0)
			xs=-xs;
		if(ys<0)
			ys=-ys;
		
		bxs=(xs+3)>>2;
		bys=(ys+3)>>2;
		
		wctx->buf_info->biWidth=xs;
		wctx->buf_info->biHeight=ys;
		wctx->buf_info->biBitCount=16;
		
		wctx->size_bxs=bxs;
		wctx->size_bys=bys;
		wctx->size_x=bxs<<2;
		wctx->size_y=bys<<2;
		
		tk_printf("  xs=%d ys=%d\n", xs, ys);
		tk_printf("  bxs=%d bys=%d\n", bxs, bys);
		
		wctx->size_bmxs=(bxs+7)>>3;
		wctx->size_bmsz=wctx->size_bmxs*bys;
		
		wctx->buf_data=tk_malloc(xs*ys*2);
		wctx->buf_utx2=tk_malloc(bxs*bys*8);
		wctx->buf_dirty1=tk_malloc(wctx->size_bmsz);
		wctx->buf_dirty2=tk_malloc(wctx->size_bmsz);
		memset(wctx->buf_data, 0x55, xs*ys*2);
		memset(wctx->buf_dirty1, 0xFF, wctx->size_bmsz);
		memset(wctx->buf_dirty2, 0xFF, wctx->size_bmsz);
		
		wctx->dirty1=1;
		wctx->dirty2=0;

		i=tkgdi_n_window_vis++;
		tkgdi_window_vis[i]=wctx;

		TKGDI_UpdateWindowCells(wctx);
		TKGDI_UpdateWindowStack();
		
		return(wctx->idx);
	}
}

static u16 tkgdi_rgbi2rgb555[16]={
	0x0000, /* 0, 0.000-00.00-000.0-0000 */
	0x0015, /* 1, 0.000-00.00-000.1-0101 */
	0x02A0, /* 2, 0.000-00.10-101.0-0000 */
	0x02B5, /* 3, 0.000-00.10-101.1-0101 */
	0x5400, /* 4, 0.101-01.00-000.0-0000 */
	0x5415, /* 5, 0.101-01.00-000.1-0101 */
	0x56A0, /* 6, 0.101-01.10-101.0-0000 */
	0x56B5, /* 7, 0.101-01.10-101.1-0101 */
	0x294A, /* 8, 0.010-10.01-010.0-1010 */
	0x295F, /* 9, 0.010-10.01-010.1-1111 */
	0x2BEA, /* A, 0.010-10.11-111.0-1010 */
	0x2BFF, /* B, 0.010-10.11-111.1-1111 */
	0x7D4A, /* C, 0.111-11.01-010.0-1010 */
	0x7D5F, /* D, 0.111-11.01-010.1-1111 */
	0x7FEA, /* E, 0.111-11.11-111.0-1010 */
	0x7FFF  /* F, 0.111-11.11-111.1-1111 */
};

int TKGDI_DrawCharBasic(int xo, int yo, int ch, long long mode)
{
	u32 *conbufa;
	u64 pix;
	u32 cxa;
	int xi, yi, ci;

	conbufa=(u32 *)0xFFFFF00A0000ULL;

	if(	(tkgdi_vid_scrmode==TKGDI_SCRMODE_TEXT		)	||
		(tkgdi_vid_scrmode==TKGDI_SCRMODE_640x400_CC	)	)
	{
		if(((mode&255)==8) && !(xo&7) && !(yo&7))
		{
			xi=xo>>3;
			yi=yo>>3;
			ci=(yi*80+xi)*8;
		
			pix=TK_Con_GlyphForCodepoint(ch);
			cxa=0x80000000ULL |
				(tkgdi_rgbi2rgb555[(mode>>12)&15]<<15) |
				(tkgdi_rgbi2rgb555[(mode>> 8)&15]<< 0) ;
			conbufa[ci+0]=cxa;
			conbufa[ci+1]=0;
			conbufa[ci+2]=pix;
			conbufa[ci+3]=pix>>32;
			
			return(0);
		}
	}

	return(-1);
}

TKGSTATUS TKGDI_DrawString(
	_tkgdi_context_t *ctx,
	TKGHDC dev, int xo_dev, int yo_dev,
	char *text, TKGHFONT font, long long mode)
{
	char *s;
	int cx, cy, sz, ch;
	
	cx=xo_dev;
	cy=yo_dev;
	sz=mode&255;
	s=text;
	
	while(*s)
	{
		ch=*s++;
		
		if(ch=='\r')
		{
			cx=xo_dev;
			continue;
		}
		if(ch=='\n')
		{
			cx=xo_dev;
			cy+=sz;
			continue;
		}
		
		TKGDI_DrawCharBasic(cx, cy, ch, mode);
	}
	return(0);
}

TKGSTATUS TKGDI_DestroyDisplay(
	_tkgdi_context_t *ctx,
	TKGHDC dev)
{
	return(0);
}

TKGSTATUS TKGDI_ModifyDisplay(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGFOURCC parm,
	void *ifmt,
	void *ofmt)
{
	_tkgdi_window_t *wctx;
	TKGDI_RECT *rect;

	wctx=tkgdi_windows[dev];
	if(!wctx)
	{
		return(-1);
	}

	if(parm==TKGDI_FCC_move)
	{
		TKGDI_ScreenMarkDirty();

		rect=ifmt;
		wctx->base_x=rect->left;
		wctx->base_y=rect->top;
	}

	return(0);
}


void	TKGDI_Snd_SetupOutputMode(int chan, int rate, int bits);
void	TKGDI_Snd_WriteModeSamples2(void *mixbuf, int nsamp, int nsamp2);
void	TKGDI_Snd_Submit(void);

TKGHSND TKGDI_CreateAudioDevice(
	_tkgdi_context_t *ctx,
	TKGHDC dev, TKGFOURCC clz, TKGDI_WAVEFORMATEX *info)
{
	TKGDI_Snd_SetupOutputMode(
		info->nChannels,
		info->nSamplesPerSec,
		info->wBitsPerSample);
	return(1);
}

TKGSTATUS TKGDI_DestroyAudioDevice(
	_tkgdi_context_t *ctx,
	TKGHSND dev)
{
	return(0);
}

TKGSTATUS TKGDI_ModifyAudioDevice(
	_tkgdi_context_t *ctx,
	TKGHSND dev,
	TKGFOURCC parm,
	void *ifmt,
	void *ofmt)
{
	TKGDI_MIDI_COMMAND *mcmd;
	int op, ch, d0, d1;

	if(parm==TKGDI_FCC_mcmd)
	{
		TK_Midi_Init();
	
		mcmd=ifmt;
		
		op=mcmd->op;
		ch=mcmd->ch;
		d0=mcmd->d0;
		d1=mcmd->d1;

		switch(mcmd->op)
		{
		case 0:
			TK_Midi_NoteOff(ch, d0, d1);
			break;
		case 1:
			TK_Midi_NoteOn(ch, d0, d1);
			break;
		case 2:
			TK_Midi_PitchBlend(ch, d0<<6);
			break;
		case 3:
			TK_Midi_Controller(ch,  d0, d1);
			break;
		case 4:
			TK_Midi_ProgramChange(ch,  d0);
			break;
			
		case 16:
			TK_Midi_SetFmRegisterData(d0, d1, mcmd->u0);
			break;
		case 17:
			if(d0==0)
			{
				TK_Midi_SilenceAll();
				break;
			}
			TK_Midi_SetMasterParam(d0, d1);
			break;

		default:
			break;
		}
	}

	return(0);
}

TKGSTATUS TKGDI_QueryAudioDevice(
	_tkgdi_context_t *ctx,
	TKGHSND dev,
	TKGFOURCC parm,
	void *info,
	void *rinfo)
{
	return(0);
}

TKGSTATUS TKGDI_WriteAudioSamples(
	_tkgdi_context_t *ctx,
	TKGHSND dev, void *buffer, int cnt, int ovcnt)
{
	if(dev==1)
	{
		if(ovcnt>0)
			TKGDI_Snd_WriteModeSamples2(buffer, cnt, ovcnt);
		TKGDI_Snd_Submit();
	}
	return(0);
}

const _tkgdi_context_vtable_t tkgdi_context_vtable_vt = {
NULL,
NULL,

(void *)0x12345678,

TKGDI_BlitSubImageOld,
TKGDI_QueryDisplay,
TKGDI_CreateDisplay,
TKGDI_DestroyDisplay,
TKGDI_ModifyDisplay,

TKGDI_DrawString,

(void *)0x12345678,

TKGDI_CreateAudioDevice,
TKGDI_DestroyAudioDevice,
TKGDI_ModifyAudioDevice,
TKGDI_QueryAudioDevice,
TKGDI_WriteAudioSamples,

(void *)0x12345678,

TKGDI_BlitSubImageNew,
NULL,	//1
NULL,	//2
NULL,	//3
NULL,	//4
NULL,	//5
NULL,	//6
NULL,	//7
NULL,	//8
NULL,	//9
NULL,	//10

(void *)0x12345678

};

// _tkgdi_context_t tkgdi_context_dfl = {
// &tkgdi_context_vtable_vt
// };

_tkgdi_context_t *tkgdi_context_ctx[256];
TKPE_TaskInfo *tkgdi_context_task[256];
int tkgdi_n_contexts;

void *TKGDI_GetHalContext(TKPE_TaskInfo *task,
	u64 apiname, u64 subname)
{
	_tkgdi_context_t *ctx;
	int i, j, k;
	
	tk_printf("TKGDI_GetHalContext:\n");
	
	for(i=0; i<tkgdi_n_contexts; i++)
	{
		if(tkgdi_context_task[i]==task)
		{
			return(tkgdi_context_ctx[i]);
		}
	}
	
	ctx=tk_malloc(sizeof(_tkgdi_context_t));
	memset(ctx, 0, sizeof(_tkgdi_context_t));
	ctx->vt=&tkgdi_context_vtable_vt;
	
	i=tkgdi_n_contexts++;
	tkgdi_context_task[i]=task;
	tkgdi_context_ctx[i]=ctx;
	
	return(ctx);
}

void tkgdi_comglue_dispatchfcn(void *obj, void *fcn, void *pret, void *args);

__asm {
tkgdi_comglue_dispatchfcn:
	ADD		-256, SP
	MOV		LR, R1
	MOV.Q	R1, (SP, 248)

	MOV.Q	R4, (SP, 192)
	MOV.Q	R5, (SP, 200)
	MOV.Q	R6, (SP, 208)
	MOV.Q	R7, (SP, 216)
	
	
	MOV		R7, R19
	MOV		R5, R3

	MOV.Q	(R19, 0), R5
	MOV.Q	(R19, 8), R6
	MOV.Q	(R19, 16), R7
	MOV.Q	(R19, 24), R20
	MOV.Q	(R19, 32), R21
	MOV.Q	(R19, 40), R22
	MOV.Q	(R19, 48), R23
	
	JSR		R3

	MOV.Q	(SP, 248), R1
	MOV.Q	(SP, 208), R18
	NOP
	MOV.Q	R2, (R18)
	NOP
	NOP
	ADD		256, SP
	JMP 	R1
};

void TKGDI_ComGlueDispatch(TKPE_TaskInfo *task,
	void *sObj, int idx, void *pret, void *args)
{
	void *obj1, **vt, *fn;
	
	obj1=((void **)sObj)[1];
	vt=(void **)(*(void **)obj1);
	fn=vt[idx];
	
	tkgdi_comglue_dispatchfcn(obj1, fn, pret, args);
}


void tkgdi_comglue_wrapcall3(void *obj);
void tkgdi_comglue_wrapcall4(void *obj);
void tkgdi_comglue_wrapcall5(void *obj);
void tkgdi_comglue_wrapcall6(void *obj);
void tkgdi_comglue_wrapcall7(void *obj);
void tkgdi_comglue_wrapcall8(void *obj);
void tkgdi_comglue_wrapcall9(void *obj);
void tkgdi_comglue_wrapcall10(void *obj);
void tkgdi_comglue_wrapcall11(void *obj);
void tkgdi_comglue_wrapcall12(void *obj);
void tkgdi_comglue_wrapcall13(void *obj);
void tkgdi_comglue_wrapcall14(void *obj);
void tkgdi_comglue_wrapcall15(void *obj);
void tkgdi_comglue_wrapcall16(void *obj);
void tkgdi_comglue_wrapcall17(void *obj);
void tkgdi_comglue_wrapcall18(void *obj);
void tkgdi_comglue_wrapcall19(void *obj);
void tkgdi_comglue_wrapcall20(void *obj);
void tkgdi_comglue_wrapcall21(void *obj);
void tkgdi_comglue_wrapcall22(void *obj);
void tkgdi_comglue_wrapcall23(void *obj);
void tkgdi_comglue_wrapcall24(void *obj);
void tkgdi_comglue_wrapcall25(void *obj);
void tkgdi_comglue_wrapcall26(void *obj);
void tkgdi_comglue_wrapcall27(void *obj);
void tkgdi_comglue_wrapcall28(void *obj);
void tkgdi_comglue_wrapcall29(void *obj);
void tkgdi_comglue_wrapcall30(void *obj);
void tkgdi_comglue_wrapcall31(void *obj);

void tkgdi_comglue_rv_wrapcall3(void *obj);
void tkgdi_comglue_rv_wrapcall4(void *obj);
void tkgdi_comglue_rv_wrapcall5(void *obj);
void tkgdi_comglue_rv_wrapcall6(void *obj);
void tkgdi_comglue_rv_wrapcall7(void *obj);
void tkgdi_comglue_rv_wrapcall8(void *obj);
void tkgdi_comglue_rv_wrapcall9(void *obj);
void tkgdi_comglue_rv_wrapcall10(void *obj);
void tkgdi_comglue_rv_wrapcall11(void *obj);
void tkgdi_comglue_rv_wrapcall12(void *obj);
void tkgdi_comglue_rv_wrapcall13(void *obj);
void tkgdi_comglue_rv_wrapcall14(void *obj);
void tkgdi_comglue_rv_wrapcall15(void *obj);
void tkgdi_comglue_rv_wrapcall16(void *obj);
void tkgdi_comglue_rv_wrapcall17(void *obj);
void tkgdi_comglue_rv_wrapcall18(void *obj);
void tkgdi_comglue_rv_wrapcall19(void *obj);
void tkgdi_comglue_rv_wrapcall20(void *obj);
void tkgdi_comglue_rv_wrapcall21(void *obj);
void tkgdi_comglue_rv_wrapcall22(void *obj);
void tkgdi_comglue_rv_wrapcall23(void *obj);
void tkgdi_comglue_rv_wrapcall24(void *obj);
void tkgdi_comglue_rv_wrapcall25(void *obj);
void tkgdi_comglue_rv_wrapcall26(void *obj);
void tkgdi_comglue_rv_wrapcall27(void *obj);
void tkgdi_comglue_rv_wrapcall28(void *obj);
void tkgdi_comglue_rv_wrapcall29(void *obj);
void tkgdi_comglue_rv_wrapcall30(void *obj);
void tkgdi_comglue_rv_wrapcall31(void *obj);

int tk_syscall_utxt(void *sobj, int umsg, void *pptr, void *args);
int tk_syscall_rv_utxt(void *sobj, int umsg, void *pptr, void *args);

__asm {
.section .utext

tk_syscall_utxt:
	nop
	nop
	nop
	nop
	syscall	0
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	rts
	nop
	nop
	nop
	nop
	brk
	brk

tkgdi_comglue_wrapcall_gen:
	ADD		-256, SP
	MOV		LR, R1
	MOV.Q	R1, (SP, 248)

	MOV.Q	R4, (SP, 128)
	MOV.Q	R5, (SP, 136)
	MOV.Q	R6, (SP, 144)
	MOV.Q	R7, (SP, 152)
	MOV.Q	R20, (SP, 160)
	MOV.Q	R21, (SP, 168)
	MOV.Q	R22, (SP, 176)
	MOV.Q	R23, (SP, 184)
	
	MOV		R3, R5
	LEA.B	(SP, 112), R6
//	LEA.B	(SP, 128), R7
	LEA.B	(SP, 136), R7
	BSR		tk_syscall_utxt

	MOV.Q	(SP, 248), R1
	MOV.Q	(SP, 112), R2
	MOV.Q	(SP, 120), R3

	ADD		256, SP
	JMP 	R1

tkgdi_comglue_wrapcall3:
	MOV		TK_UMSG_COMGLUE_VMT3, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall4:
	MOV		TK_UMSG_COMGLUE_VMT4, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall5:
	MOV		TK_UMSG_COMGLUE_VMT5, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall6:
	MOV		TK_UMSG_COMGLUE_VMT6, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall7:
	MOV		TK_UMSG_COMGLUE_VMT7, R3
	BRA		tkgdi_comglue_wrapcall_gen

tkgdi_comglue_wrapcall8:
	MOV		TK_UMSG_COMGLUE_VMT8, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall9:
	MOV		TK_UMSG_COMGLUE_VMT9, R3
	BRA		tkgdi_comglue_wrapcall_gen

tkgdi_comglue_wrapcall10:
	MOV		TK_UMSG_COMGLUE_VMT10, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall11:
	MOV		TK_UMSG_COMGLUE_VMT11, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall12:
	MOV		TK_UMSG_COMGLUE_VMT12, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall13:
	MOV		TK_UMSG_COMGLUE_VMT13, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall14:
	MOV		TK_UMSG_COMGLUE_VMT14, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall15:
	MOV		TK_UMSG_COMGLUE_VMT15, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall16:
	MOV		TK_UMSG_COMGLUE_VMT16, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall17:
	MOV		TK_UMSG_COMGLUE_VMT17, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall18:
	MOV		TK_UMSG_COMGLUE_VMT18, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall19:
	MOV		TK_UMSG_COMGLUE_VMT19, R3
	BRA		tkgdi_comglue_wrapcall_gen


#if 1
.riscv

tk_syscall_rv_utxt:
	nop
	nop
	nop
	nop
	syscall
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	rts
	nop
	nop
	nop
	nop

tkgdi_comglue_rv_wrapcall_gen:
	ADD		-256, SP
//	MOV		LR, R1
	MOV.Q	R1, (SP, 248)

	MOV.Q	R10, (SP, 128)
	MOV.Q	R11, (SP, 136)
	MOV.Q	R12, (SP, 144)
	MOV.Q	R13, (SP, 152)
	MOV.Q	R14, (SP, 160)
	MOV.Q	R15, (SP, 168)
	MOV.Q	R16, (SP, 176)
	MOV.Q	R17, (SP, 184)
	
	MOV		R5, R11
	ADD		SP, 112, R12
	ADD		SP, 136, R13
	BSR		tk_syscall_utxt

	MOV.Q	(SP, 248), R1
	MOV.Q	(SP, 112), R10
	MOV.Q	(SP, 120), R11

	ADD		256, SP
	JMP 	R1

tkgdi_comglue_rv_wrapcall3:
	MOV		TK_UMSG_COMGLUE_VMT3, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall4:
	MOV		TK_UMSG_COMGLUE_VMT4, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall5:
	MOV		TK_UMSG_COMGLUE_VMT5, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall6:
	MOV		TK_UMSG_COMGLUE_VMT6, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall7:
	MOV		TK_UMSG_COMGLUE_VMT7, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen

tkgdi_comglue_rv_wrapcall8:
	MOV		TK_UMSG_COMGLUE_VMT8, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall9:
	MOV		TK_UMSG_COMGLUE_VMT9, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen

tkgdi_comglue_rv_wrapcall10:
	MOV		TK_UMSG_COMGLUE_VMT10, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall11:
	MOV		TK_UMSG_COMGLUE_VMT11, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall12:
	MOV		TK_UMSG_COMGLUE_VMT12, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall13:
	MOV		TK_UMSG_COMGLUE_VMT13, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall14:
	MOV		TK_UMSG_COMGLUE_VMT14, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall15:
	MOV		TK_UMSG_COMGLUE_VMT15, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall16:
	MOV		TK_UMSG_COMGLUE_VMT16, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall17:
	MOV		TK_UMSG_COMGLUE_VMT17, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall18:
	MOV		TK_UMSG_COMGLUE_VMT18, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall19:
	MOV		TK_UMSG_COMGLUE_VMT19, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen

.endriscv
#endif
};


const _tkgdi_context_vtable_t tkgdi_context_vtable_gvt = {
NULL,						//0
NULL,						//1
(void *)0x12345678,			//2
tkgdi_comglue_wrapcall3,	//3
tkgdi_comglue_wrapcall4,
tkgdi_comglue_wrapcall5,
tkgdi_comglue_wrapcall6,
tkgdi_comglue_wrapcall7,
tkgdi_comglue_wrapcall8,
(void *)0x12345678,			//9
tkgdi_comglue_wrapcall10,	//10
tkgdi_comglue_wrapcall11,
tkgdi_comglue_wrapcall12,
tkgdi_comglue_wrapcall13,
tkgdi_comglue_wrapcall14,	//14
(void *)0x12345678,			//15
tkgdi_comglue_wrapcall16,	//16
NULL,	//1
NULL,	//2
NULL,	//3
NULL,	//4
NULL,	//5
NULL,	//6
NULL,	//7
NULL,	//8
NULL,	//9
NULL,	//10
(void *)0x12345678
};

const _tkgdi_context_vtable_t tkgdi_context_vtable_grvvt = {
NULL,						//0
NULL,						//1
(void *)0x12345678,			//2
tkgdi_comglue_rv_wrapcall3,	//3
tkgdi_comglue_rv_wrapcall4,
tkgdi_comglue_rv_wrapcall5,
tkgdi_comglue_rv_wrapcall6,
tkgdi_comglue_rv_wrapcall7,
tkgdi_comglue_rv_wrapcall8,
(void *)0x12345678,			//9
tkgdi_comglue_rv_wrapcall10,	//10
tkgdi_comglue_rv_wrapcall11,
tkgdi_comglue_rv_wrapcall12,
tkgdi_comglue_rv_wrapcall13,
tkgdi_comglue_rv_wrapcall14,	//14
(void *)0x12345678,			//15
tkgdi_comglue_rv_wrapcall16,	//16
NULL,	//1
NULL,	//2
NULL,	//3
NULL,	//4
NULL,	//5
NULL,	//6
NULL,	//7
NULL,	//8
NULL,	//9
NULL,	//10
(void *)0x12345678
};


_tkgdi_context_vtable_t *tkgdi_context_vtable_gvtc;
_tkgdi_context_vtable_t *tkgdi_context_vtable_grvvtc;

_tkgdi_context_t *tkgdi_gcontext_ctx[256];
TKPE_TaskInfo *tkgdi_gcontext_task[256];
int tkgdi_n_gcontexts;

void *TKGDI_GetHalContextComGlue(TKPE_TaskInfo *task,
	u64 apiname, u64 subname)
{
	void **ppv, *pv;
	_tkgdi_context_t *ctx;
	_tkgdi_context_t *ctx2;
	TKPE_TaskInfo *ctask;
	u64 lv;
	int i, j, k, n;
	
	tk_printf("TKGDI_GetHalContext:\n");
	
	for(i=0; i<tkgdi_n_gcontexts; i++)
	{
		if(tkgdi_gcontext_task[i]==task)
		{
			return(tkgdi_gcontext_ctx[i]);
		}
	}

	ctask=TK_GetCurrentTask();

	ctx2=TKGDI_GetHalContext(task, apiname, subname);
	
	if(ctask==task)
		return(ctx2);
	
	if(!tkgdi_context_vtable_gvtc)
	{
		tkgdi_context_vtable_gvtc=tk_malloc_usr(
			sizeof(_tkgdi_context_vtable_t));
		tkgdi_context_vtable_grvvtc=tk_malloc_usr(
			sizeof(_tkgdi_context_vtable_t));
		memcpy(tkgdi_context_vtable_gvtc,
			&tkgdi_context_vtable_gvt,
			sizeof(_tkgdi_context_vtable_t));
		memcpy(tkgdi_context_vtable_grvvtc,
			&tkgdi_context_vtable_grvvt,
			sizeof(_tkgdi_context_vtable_t));
			
		n=sizeof(_tkgdi_context_vtable_t)/sizeof(void *);
		ppv=(void **)tkgdi_context_vtable_gvtc;
		for(i=0; i<n; i++)
		{
			pv=ppv[i];
			lv=(u64)pv;
			if(!lv)
				continue;
			if(lv==0x12345678)
				continue;

			lv&=0x0000FFFFFFFFFFFEULL;
			lv|=0x0000000000000001ULL;
			ppv[i]=(void *)lv;
		}
	}
	
	ctx=tk_malloc_usr(sizeof(_tkgdi_context_t));
	memset(ctx, 0, sizeof(_tkgdi_context_t));
//	ctx->vt=&tkgdi_context_vtable_gvt;
	ctx->vt=tkgdi_context_vtable_gvtc;
	
	if(TK_Task_SyscallTaskIsRiscV(task))
	{
		ctx->vt=tkgdi_context_vtable_grvvtc;
	}
	
	ctx->sobj=TKMM_LVA_ExportSharedObjectPtr(ctx2);
	
	i=tkgdi_n_gcontexts++;
	tkgdi_gcontext_task[i]=task;
	tkgdi_gcontext_ctx[i]=ctx;
	
	return(ctx);
}
