void TKGDI_CopyPixelSpan_SetupPalArrayF(
	u16 *pal1, u32 *pal, int nclr)
{
	int i, j, k;

	for(i=0; i<256; i++)
	{
		j=pal[i];
		k=	((j>>9)&0x7C00)|
			((j>>6)&0x03E0)|
			((j>>3)&0x001F);
		pal1[i]=k;
	}
}

void TKGDI_CopyPixelSpan_SetupPalArray(
	u16 *pal1, byte *pal, int nclr)
{
	int cr, cg, cb, ca, v;
	int i;

	for(i=0; i<nclr; i++)
	{
		cb=pal[i*4+0];
		cg=pal[i*4+1];
		cr=pal[i*4+2];
		ca=pal[i*4+3];
		cb>>=3;	cg>>=3;	cr>>=3;
		v=(cr<<10)|(cg<<5)|cb;
		if(ca<240)
		{
			v&=0x7BDE;
			v|=0x8000;
			if(ca&0x80)		v|=0x0200;
			if(ca&0x40)		v|=0x0040;
			if(ca&0x20)		v|=0x0001;
		}
		pal1[i]=v;
	}
}

void TKGDI_CopyPixelSpan_CnvI8to15(
	u16 *dst, byte *src, int xs, u16 *pal)
{
	byte *cs;
	u16 *ct;
	u32 v0, v1, v2, v3;
	int xsa;
	int x;
	
//	if(!dst || !src)
//		return;
	
	xsa=xs&(~3);
	cs=src;
	ct=dst;
	for(x=0; x<xsa; x+=4)
	{
		v0=cs[0];		v1=cs[1];
		v2=cs[2];		v3=cs[3];
		v0=pal[v0];		v1=pal[v1];
		v2=pal[v2];		v3=pal[v3];
		ct[0]=v0;		ct[1]=v1;
		ct[2]=v2;		ct[3]=v3;
		cs+=4; ct+=4;
	}
	if(x<xs)
	{
		for(; x<xs; x+=4)
		{
			v0=cs[0];
			v0=pal[v0];
			ct[0]=v0;
			cs++; ct++;
		}
	}
}

void TKGDI_CopyPixelSpan_CnvI4to15(
	u16 *dst, byte *src, int xs, u16 *pal)
{
	byte *cs;
	u16 *ct;
	u32 v0, v1, v2, v3;
	int xsa;
	int x;
	
//	if(!dst || !src)
//		return;
	
	xsa=xs&(~3);
	cs=src;
	ct=dst;
	for(x=0; x<xsa; x+=2)
	{
		v0=cs[0];		v2=cs[1];
		v1=v0&15;		v3=v2&15;
		v0=v0>>4;		v2=v2>>4;
 		v0=pal[v0];		v1=pal[v1];
		v2=pal[v2];		v3=pal[v3];
		ct[0]=v0;		ct[1]=v1;
		ct[2]=v2;		ct[3]=v3;
		cs+=2; ct+=4;
	}
	if(x<xs)
	{
		xsa=xs&(~1);
		for(; x<xsa; x+=2)
		{
			v2=*cs++;
			v0=v2>>4;
			v1=v2&15;
			v0=pal[v0];
			v1=pal[v1];
			ct[0]=v0;
			ct[1]=v1;
			ct+=2;
		}
		
		if(x<xs)
		{
			v2=*cs;
			v0=v2>>4;
			v0=pal[v0];
			ct[0]=v0;
		}
	}
}

#ifdef __BJX2__
u16 TKGDI_CopyPixelSpan_GetRGB24(byte *src);
u64 TKGDI_CopyPixelSpan_GetRGB24x4(byte *src);
u64 TKGDI_CopyPixelSpan_GetRGB32x4(byte *src);

u16 TKGDI_CopyPixelSpan_GetRGB24H(byte *src, u32 scbi);
u64 TKGDI_CopyPixelSpan_GetRGB32x4H(byte *src, u32 scbi);
u64 TKGDI_CopyPixelSpan_GetRGB24x4H(byte *src, u32 scbi);

__asm {
TKGDI_CopyPixelSpan_GetRGB24:
	MOVU.L		(R4), R6
	MOV			0xFF000000, R7
	OR			R6, R7, R5
	RGB5PCK32	R5, R2
	RTS

TKGDI_CopyPixelSpan_GetRGB24x4:
	ADD			R4, 3, R5
	ADD			R4, 6, R6	|	MOVU.L		(R4), R20
	ADD			R4, 9, R7	|	MOVU.L		(R5), R21
	MOVU.L		(R6), R22
	MOVU.L		(R7), R23
	MOV			0xFF000000, R3
	OR			R20, R3, R20	|	OR			R21, R3, R21
	OR			R22, R3, R22	|	OR			R23, R3, R23
	RGB5PCK32	R20, R16		|	RGB5PCK32	R21, R17
	RGB5PCK32	R22, R18		|	RGB5PCK32	R23, R19
	SHLD.Q		R17, 16, R17	|	SHLD.Q		R18, 32, R18
	SHLD.Q		R19, 48, R19	|	OR			R16, R17, R20
	OR			R18, R19, R21
	OR			R20, R21, R2
	RTS

TKGDI_CopyPixelSpan_GetRGB32x4:
	MOVU.L		(R4,  0), R20
	MOVU.L		(R4,  4), R21
	MOVU.L		(R4,  8), R22
	MOVU.L		(R4, 12), R23
	RGB5PCK32	R20, R16		|	RGB5PCK32	R21, R17
	RGB5PCK32	R22, R18		|	RGB5PCK32	R23, R19
	SHLD.Q		R17, 16, R17	|	SHLD.Q		R18, 32, R18
	SHLD.Q		R19, 48, R19	|	OR			R16, R17, R20
	OR			R18, R19, R21
	OR			R20, R21, R2
	RTS

TKGDI_CopyPixelSpan_GetRGB24H:
	TST			R5, R5
	BT			.NoScale

	MOVU.L		(R4), R6
	PSHUF.W		R5, 0x00, R20
	PSHUF.W		R5, 0x55, R21

	PLDCM8UH	R6, R16
	MOV			0x3C003C003C003C00, R17
	PMUL.H		R16, R20, R16
	MOV			0x3FFF3FFF3FFF3FFF, R22
	PADD.H		R16, R21, R16

	PADD.H		R16, R17, R18
	
	PCMPGT.H	R22, R18
	PCSELT.W	R22, R18, R18

	PCMPGT.H	R18, R17
	PCSELT.W	R17, R18, R18


//	TSTQ		0x0000C000C000C000, R18
//	BF			.L1
//	.L0:

	MOV			0xFFFF000000000000, R7
	PCVTH2UW	R18, R19
	OR			R19, R7, R5
	RGB5PCK64	R5, R2

	RTS

.NoScale:
	MOVU.L		(R4), R6
	MOV			0x3C003C003C003C00, R17
	MOV			0x3FFF3FFF3FFF3FFF, R22

	PLDCM8UH	R6, R16

	PADD.H		R16, R17, R18
	
	PCMPGT.H	R22, R18
	PCSELT.W	R22, R18, R18

	PCMPGT.H	R18, R17
	PCSELT.W	R17, R18, R18

	MOV			0xFFFF000000000000, R7
	PCVTH2UW	R18, R19
	OR			R19, R7, R5
	RGB5PCK64	R5, R2

	RTS


#if 0
	.L1:

	TSTQ		0x00000000C000, R18
	AND?F		0xFFFFFFFF0000, R18
	OR?F		0x000000003BFF, R18

	TSTQ		0x0000C0000000, R18
	AND?F		0xFFFF0000FFFF, R18
	OR?F		0x00003BFF0000, R18

	TSTQ		0xC00000000000, R18
	AND?F		0x0000FFFFFFFF, R18
	OR?F		0x3BFF00000000, R18

	BRA			.L0
#endif


TKGDI_CopyPixelSpan_GetRGB32x4H:
	TST			R5, R5
	BT			TKGDI_CopyPixelSpan_GetRGB32x4H_NoScale

	MOVU.L		(R4,  0), R16
	MOVU.L		(R4,  4), R17
	MOVU.L		(R4,  8), R18
	MOVU.L		(R4, 12), R19

TKGDI_CopyPixelSpan_GetRGB32x4H_P1:

	PLDCM8UH	R16, R16
	PLDCM8UH	R17, R17
	PLDCM8UH	R18, R18
	PLDCM8UH	R19, R19

	PSHUF.W		R5, 0x00, R21
	PSHUF.W		R5, 0x55, R23

	MOV			0x3C003C003C003C00, R20
	MOV			0x3FFF3FFF3FFF3FFF, R22

	PMUL.H		R16, R21, R16
	PMUL.H		R17, R21, R17
	PMUL.H		R18, R21, R18
	PMUL.H		R19, R21, R19

	PADD.H		R16, R23, R16
	PADD.H		R17, R23, R17
	PADD.H		R18, R23, R18
	PADD.H		R19, R23, R19

	PADD.H		R16, R20, R16
	PADD.H		R17, R20, R17
	PADD.H		R18, R20, R18
	PADD.H		R19, R20, R19
	
	PCMPGT.H	R22, R16
	PCSELT.W	R22, R16, R16
	PCMPGT.H	R22, R17
	PCSELT.W	R22, R17, R17
	PCMPGT.H	R22, R18
	PCSELT.W	R22, R18, R18
	PCMPGT.H	R22, R19
	PCSELT.W	R22, R19, R19

	PCMPGT.H	R16, R20
	PCSELT.W	R20, R16, R16
	PCMPGT.H	R17, R20
	PCSELT.W	R20, R17, R17
	PCMPGT.H	R18, R20
	PCSELT.W	R20, R18, R18
	PCMPGT.H	R19, R20
	PCSELT.W	R20, R19, R19

	MOV			0xFFFF000000000000, R3
									PCVTH2UW	R16, R4
									PCVTH2UW	R17, R5
	OR			R4, R3, R4		|	PCVTH2UW	R18, R6
	OR			R5, R3, R5		|	PCVTH2UW	R19, R7
	OR			R6, R3, R6		|	RGB5PCK64	R4, R4
	OR			R7, R3, R7		|	RGB5PCK64	R5, R5
									RGB5PCK64	R6, R6
									RGB5PCK64	R7, R7
//	SHLD		R5, 16, R5
//	SHLD		R7, 16, R7
//	OR			R4, R5, R4
//	OR			R6, R7, R6
	MOVLLW		R5, R4, R4
	MOVLLW		R7, R6, R6

	MOVLLD		R6, R4, R2

	RTS

.balign		4
TKGDI_CopyPixelSpan_GetRGB32x4H_NoScale:
	MOVU.L		(R4,  0), R16
	MOVU.L		(R4,  4), R17
	MOVU.L		(R4,  8), R18
	MOVU.L		(R4, 12), R19

TKGDI_CopyPixelSpan_GetRGB32x4H_P1NS:
	MOV			0x3C003C003C003C00, R20
	MOV			0x3FFF3FFF3FFF3FFF, R22
	MOV			0xFFFF000000000000, R3

	PLDCM8UH	R16, R16
	PLDCM8UH	R17, R17
	PLDCM8UH	R18, R18
	PLDCM8UH	R19, R19

	PADD.H		R16, R20, R16
	PADD.H		R17, R20, R17
	PADD.H		R18, R20, R18
	PADD.H		R19, R20, R19
	
	PCMPGT.H	R22, R16
	PCSELT.W	R22, R16, R16
	PCMPGT.H	R22, R17
	PCSELT.W	R22, R17, R17
	PCMPGT.H	R22, R18
	PCSELT.W	R22, R18, R18
	PCMPGT.H	R22, R19
	PCSELT.W	R22, R19, R19

								PCVTH2UW	R16, R4
								PCVTH2UW	R17, R5
	OR			R4, R3, R4	|	PCVTH2UW	R18, R6
	OR			R5, R3, R5	|	PCVTH2UW	R19, R7
	OR			R6, R3, R6	|	RGB5PCK64	R4, R4
	OR			R7, R3, R7	|	RGB5PCK64	R5, R5
								RGB5PCK64	R6, R6
								RGB5PCK64	R7, R7

//	SHLD		R5, 16, R5
//	SHLD		R7, 16, R7
//	OR			R4, R5, R4
//	OR			R6, R7, R6

	MOVLLW		R5, R4, R4
	MOVLLW		R7, R6, R6

	MOVLLD		R6, R4, R2

	RTS

TKGDI_CopyPixelSpan_GetRGB24x4H:
	ADD			R4, 3, R21
	ADD			R4, 6, R22
	ADD			R4, 9, R23
	MOVU.L		(R4 ), R16
	MOVU.L		(R21), R17
	MOVU.L		(R22), R18
	MOVU.L		(R23), R19

	TST			R5, R5
	BT			TKGDI_CopyPixelSpan_GetRGB32x4H_P1NS
	BRA			TKGDI_CopyPixelSpan_GetRGB32x4H_P1
};

#else
u16 TKGDI_CopyPixelSpan_GetRGB24(byte *src)
{
	int cr, cg, cb, v;
	cr=src[2];
	cg=src[1];
	cb=src[0];
	cr=cr>>3;
	cg=cg>>3;
	cb=cb>>3;
	cr=cr<<10;
	cg=cg<<5;
	v=cr|cb;
	v=cg|v;
	return(v);
}

u16 TKGDI_CopyPixelSpan_GetRGB24H(byte *src, u32 scbi)
{
}
#endif

void TKGDI_CopyPixelSpan_Cnv24to15(
	u16 *dst, byte *src, int xs)
{
	byte *cs;
	u16 *ct;
	u32 v0, v1, v2, v3;
	int xsa;
	int x;

	xsa=xs&(~3);
	cs=src;
	ct=dst;
	for(x=0; x<xsa; x+=4)
	{
#ifdef __BJX2__
		*(u64 *)ct=TKGDI_CopyPixelSpan_GetRGB24x4(cs);
#else
		v0=TKGDI_CopyPixelSpan_GetRGB24(cs+0);
		v1=TKGDI_CopyPixelSpan_GetRGB24(cs+3);
		v2=TKGDI_CopyPixelSpan_GetRGB24(cs+6);
		v3=TKGDI_CopyPixelSpan_GetRGB24(cs+9);
		ct[0]=v0;		ct[1]=v1;
		ct[2]=v2;		ct[3]=v3;
#endif
		cs+=12; ct+=4;
	}
	if(x<xs)
	{
		for(; x<xs; x+=4)
		{
			v0=TKGDI_CopyPixelSpan_GetRGB24(cs);
			ct[0]=v0;
			cs+=3; ct++;
		}
	}
}

void TKGDI_CopyPixelSpan_Cnv32to15(
	u16 *dst, byte *src, int xs)
{
	byte *cs;
	u16 *ct;
	u32 v0, v1, v2, v3;
	int xsa;
	int x;

	xsa=xs&(~3);
	cs=src;
	ct=dst;
	for(x=0; x<xsa; x+=4)
	{
#ifdef __BJX2__
		*(u64 *)ct=TKGDI_CopyPixelSpan_GetRGB32x4(cs);
#else
		v0=TKGDI_CopyPixelSpan_GetRGB24(cs+ 0);
		v1=TKGDI_CopyPixelSpan_GetRGB24(cs+ 4);
		v2=TKGDI_CopyPixelSpan_GetRGB24(cs+ 8);
		v3=TKGDI_CopyPixelSpan_GetRGB24(cs+12);
		ct[0]=v0;		ct[1]=v1;
		ct[2]=v2;		ct[3]=v3;
#endif
		cs+=16; ct+=4;
	}
	if(x<xs)
	{
		for(; x<xs; x+=4)
		{
			v0=TKGDI_CopyPixelSpan_GetRGB24(cs);
			ct[0]=v0;
			cs+=4; ct++;
		}
	}
}


void TKGDI_CopyPixelSpan_Cnv24Hto15(
	u16 *dst, byte *src, int xs, u32 scbi)
{
	byte *cs;
	u16 *ct;
	u32 v0, v1, v2, v3;
	int xsa;
	int x;

//	if(!scbi)
//		scbi=0x3C00;

	xsa=xs&(~3);
	cs=src;
	ct=dst;
	for(x=0; x<xsa; x+=4)
	{
// #ifdef __BJX2__
#if 0
		*(u64 *)ct=TKGDI_CopyPixelSpan_GetRGB24x4(cs);
#else
		v0=TKGDI_CopyPixelSpan_GetRGB24H(cs+0, scbi);
		v1=TKGDI_CopyPixelSpan_GetRGB24H(cs+3, scbi);
		v2=TKGDI_CopyPixelSpan_GetRGB24H(cs+6, scbi);
		v3=TKGDI_CopyPixelSpan_GetRGB24H(cs+9, scbi);
		ct[0]=v0;		ct[1]=v1;
		ct[2]=v2;		ct[3]=v3;
#endif
		cs+=12; ct+=4;
	}
	if(x<xs)
	{
		for(; x<xs; x+=4)
		{
			v0=TKGDI_CopyPixelSpan_GetRGB24H(cs, scbi);
			ct[0]=v0;
			cs+=3; ct++;
		}
	}
}

void TKGDI_CopyPixelSpan_Cnv32Hto15(
	u16 *dst, byte *src, int xs, u32 scbi)
{
	byte *cs;
	u16 *ct;
	u32 v0, v1, v2, v3;
	int xsa;
	int x;

//	if(!scbi)
//		scbi=0x3C00;

	xsa=xs&(~3);
	cs=src;
	ct=dst;
	for(x=0; x<xsa; x+=4)
	{
#ifdef __BJX2__
// #if 0
		*(u64 *)ct=TKGDI_CopyPixelSpan_GetRGB32x4H(cs, scbi);
#else
		v0=TKGDI_CopyPixelSpan_GetRGB24H(cs+ 0, scbi);
		v1=TKGDI_CopyPixelSpan_GetRGB24H(cs+ 4, scbi);
		v2=TKGDI_CopyPixelSpan_GetRGB24H(cs+ 8, scbi);
		v3=TKGDI_CopyPixelSpan_GetRGB24H(cs+12, scbi);
		ct[0]=v0;		ct[1]=v1;
		ct[2]=v2;		ct[3]=v3;
#endif
		cs+=16; ct+=4;
	}
	if(x<xs)
	{
		for(; x<xs; x+=4)
		{
			v0=TKGDI_CopyPixelSpan_GetRGB24H(cs, scbi);
			ct[0]=v0;
			cs+=4; ct++;
		}
	}
}


/*
Restricted CRAM Subset:
Excludes all non-2-color blocks.
No skip blocks, no 8 color blocks.
Effectively, turning it into fixed color-cells.
 */

void TKGDI_CopyPixelSpan_RowCnvCRAM8to15(
	u16 *dst, byte *src, int xs, u16 *pal)
{
	byte *cs;
	u16 *ct, *ct0, *ct1, *ct2, *ct3;
	u16 px, ca, cb;
	int bx, xs4, p0, p1, p2, p3;

	xs4=xs>>2;
	cs=src;
	ct=dst;
	
	ct0=ct;
	ct1=ct+xs;
	ct2=ct+(xs*2);
	ct3=ct+(xs*3);
	
	for(bx=0; bx<xs4; bx++)
	{
		px=*((u16 *)cs);
		ca=cs[2];		cb=cs[3];
		ca=pal[ca];		cb=pal[cb];
		
		if(px&0x0001)	{ p0=ca; } else { p0=cb; }
		if(px&0x0002)	{ p1=ca; } else { p1=cb; }
		if(px&0x0004)	{ p2=ca; } else { p2=cb; }
		if(px&0x0008)	{ p3=ca; } else { p3=cb; }
		ct0[0]=p0;		ct0[1]=p1;
		ct0[2]=p2;		ct0[3]=p3;
		if(px&0x0010)	{ p0=ca; } else { p0=cb; }
		if(px&0x0020)	{ p1=ca; } else { p1=cb; }
		if(px&0x0040)	{ p2=ca; } else { p2=cb; }
		if(px&0x0080)	{ p3=ca; } else { p3=cb; }
		ct1[0]=p0;		ct1[1]=p1;
		ct1[2]=p2;		ct1[3]=p3;
		if(px&0x0100)	{ p0=ca; } else { p0=cb; }
		if(px&0x0200)	{ p1=ca; } else { p1=cb; }
		if(px&0x0400)	{ p2=ca; } else { p2=cb; }
		if(px&0x0800)	{ p3=ca; } else { p3=cb; }
		ct2[0]=p0;		ct2[1]=p1;
		ct2[2]=p2;		ct2[3]=p3;
		if(px&0x1000)	{ p0=ca; } else { p0=cb; }
		if(px&0x2000)	{ p1=ca; } else { p1=cb; }
		if(px&0x4000)	{ p2=ca; } else { p2=cb; }
		if(px&0x8000)	{ p3=ca; } else { p3=cb; }
		ct3[0]=p0;		ct3[1]=p1;
		ct3[2]=p2;		ct3[3]=p3;
		
		cs+=4;
		ct0+=4;		ct1+=4;
		ct2+=4;		ct3+=4;
	}
}

void TKGDI_CopyPixelSpan_RowCnvCRAM16to15(
	u16 *dst, byte *src, int xs)
{
	byte *cs;
	u16 *ct, *ct0, *ct1, *ct2, *ct3;
	u16 px, ca, cb;
	int bx, xs4, p0, p1, p2, p3;

	xs4=xs>>2;
	cs=src;
	ct=dst;
	
	ct0=ct;
	ct1=ct+xs;
	ct2=ct+(xs*2);
	ct3=ct+(xs*3);
	
	for(bx=0; bx<xs4; bx++)
	{
		px=((u16 *)cs)[0];
		ca=((u16 *)cs)[1];
		cb=((u16 *)cs)[2];

		ca=(ca&0x03E0)|((ca<<10)&0x7C00)|((ca>>10)&0x001F);
		cb=(cb&0x03E0)|((cb<<10)&0x7C00)|((cb>>10)&0x001F);

		if(px&0x0001)	{ p0=ca; } else { p0=cb; }
		if(px&0x0002)	{ p1=ca; } else { p1=cb; }
		if(px&0x0004)	{ p2=ca; } else { p2=cb; }
		if(px&0x0008)	{ p3=ca; } else { p3=cb; }
		ct0[0]=p0;		ct0[1]=p1;
		ct0[2]=p2;		ct0[3]=p3;
		if(px&0x0010)	{ p0=ca; } else { p0=cb; }
		if(px&0x0020)	{ p1=ca; } else { p1=cb; }
		if(px&0x0040)	{ p2=ca; } else { p2=cb; }
		if(px&0x0080)	{ p3=ca; } else { p3=cb; }
		ct1[0]=p0;		ct1[1]=p1;
		ct1[2]=p2;		ct1[3]=p3;
		if(px&0x0100)	{ p0=ca; } else { p0=cb; }
		if(px&0x0200)	{ p1=ca; } else { p1=cb; }
		if(px&0x0400)	{ p2=ca; } else { p2=cb; }
		if(px&0x0800)	{ p3=ca; } else { p3=cb; }
		ct2[0]=p0;		ct2[1]=p1;
		ct2[2]=p2;		ct2[3]=p3;
		if(px&0x1000)	{ p0=ca; } else { p0=cb; }
		if(px&0x2000)	{ p1=ca; } else { p1=cb; }
		if(px&0x4000)	{ p2=ca; } else { p2=cb; }
		if(px&0x8000)	{ p3=ca; } else { p3=cb; }
		ct3[0]=p0;		ct3[1]=p1;
		ct3[2]=p2;		ct3[3]=p3;
		
		cs+=6;
		ct0+=4;		ct1+=4;
		ct2+=4;		ct3+=4;
	}
}

void TKGDI_CopyPixelSpan_ImageCnvCRAM8to15(
	u16 *dst, byte *src, int xs, int ys, u16 *pal)
{
	byte *cs;
	u16 *ct;
	int xs4, ys4;
	int bx, by;
	
	xs4=xs>>2;
	ys4=ys>>2;
	cs=src;
	ct=dst;
	
	for(by=0; by<ys4; by++)
	{
		TKGDI_CopyPixelSpan_RowCnvCRAM8to15(ct, cs, xs, pal);
		cs+=xs4*4; ct+=xs*4;
	}
}

void TKGDI_CopyPixelSpan_ImageCnvCRAM16to15(
	u16 *dst, byte *src, int xs, int ys)
{
	byte *cs;
	u16 *ct;
	int xs4, ys4;
	int bx, by;
	
	xs4=xs>>2;
	ys4=ys>>2;
	cs=src;
	ct=dst;
	
	for(by=0; by<ys4; by++)
	{
		TKGDI_CopyPixelSpan_RowCnvCRAM16to15(ct, cs, xs);
		cs+=xs4*6; ct+=xs*4;
	}
}

#ifdef __BJX2__
u16 TKGDI_CopyPixelSpan_FixedBlendRGB15(u16 ca, u16 cb);

__asm {
TKGDI_CopyPixelSpan_FixedBlendRGB15:
	MOV			0x1FFF1FFF1FFF1FFF, R3
	RGB5UPCK64	R4, R16			|	RGB5UPCK64	R5, R17
	SHLD.Q		R16, -3, R16	|	SHLD.Q		R17, -3, R17
	AND			R16, R3, R16	|	AND			R17, R3, R17
	SHLD.Q		R16, 2, R18		|	SHLD.Q		R17, 1, R19
	ADD			R16, R18, R20	|	ADD			R17, R19, R21
	ADD			R20, R21, R6
	RGB5PCK64	R6, R2
	RTS
};

#else
/** 5/8 A + 3/8 B */
u16 TKGDI_CopyPixelSpan_FixedBlendRGB15(u16 ca, u16 cb)
{
	int cr0, cg0, cb0, cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int ca0, ca1, ca2;
	int v;

	if((ca|cb)&0x8000)
	{
		cr0=(ca>>10)&30;	cr1=(cb>>10)&30;
		cg0=(ca>> 5)&30;	cg1=(cb>> 5)&30;
		cb0=(ca>> 0)&30;	cb1=(cb>> 0)&30;
		ca0=(ca&1)|((ca>>4)&2)|((ca>>8)&4);
		ca1=(cb&1)|((cb>>4)&2)|((cb>>8)&4);
		if(!(ca&0x8000))	ca0=7;
		if(!(cb&0x8000))	ca1=7;
		cr2=5*cr0+3*cr1;
		cg2=5*cg0+3*cg1;
		cb2=5*cb0+3*cb1;
		ca2=5*ca0+3*ca1;
		cr2=cr2>>3;		cg2=cg2>>3;
		cb2=cb2>>3;		ca2=ca2>>3;
		v=0x8000|(cr2<<10)|(cg2<<5)|cb2;
		v=v&0x7BDE;
		if(ca2&4)	v|=0x0400;
		if(ca2&2)	v|=0x0020;
		if(ca2&1)	v|=0x0001;
	}else
	{
		cr0=(ca>>10)&31;	cr1=(cb>>10)&31;
		cg0=(ca>> 5)&31;	cg1=(cb>> 5)&31;
		cb0=(ca>> 0)&31;	cb1=(cb>> 0)&31;
		cr2=5*cr0+3*cr1;
		cg2=5*cg0+3*cg1;
		cb2=5*cb0+3*cb1;
		cr2=cr2>>3;
		cg2=cg2>>3;
		cb2=cb2>>3;
		v=(cr2<<10)|(cg2<<5)|cb2;
	}

	return(v);
}
#endif

void TKGDI_CopyPixelSpan_RowCnvUTX2to15(
	u16 *dst, byte *src, int xs)
{
	u16 ctab[4];
	byte *cs;
	u16 *ct, *ct0, *ct1, *ct2, *ct3;
	u16 ca, cb;
	u32 px;
	int bx, xs4, p0, p1, p2, p3;

	xs4=xs>>2;
	cs=src;
	ct=dst;
	
	ct0=ct;
	ct1=ct+xs;
	ct2=ct+(xs*2);
	ct3=ct+(xs*3);
	
	for(bx=0; bx<xs4; bx++)
	{
		ca=((u16 *)cs)[0];
		cb=((u16 *)cs)[1];
		px=((u32 *)cs)[1];
		
		ctab[0]=ca;
		ctab[1]=TKGDI_CopyPixelSpan_FixedBlendRGB15(ca, cb);
		ctab[2]=TKGDI_CopyPixelSpan_FixedBlendRGB15(cb, ca);
		ctab[3]=cb;
		
		ct0[0]=ctab[(px>> 0)&3];	ct0[1]=ctab[(px>> 2)&3];
		ct0[2]=ctab[(px>> 4)&3];	ct0[3]=ctab[(px>> 6)&3];
		ct1[0]=ctab[(px>> 8)&3];	ct1[1]=ctab[(px>>10)&3];
		ct1[2]=ctab[(px>>12)&3];	ct1[3]=ctab[(px>>14)&3];
		ct2[0]=ctab[(px>>16)&3];	ct2[1]=ctab[(px>>18)&3];
		ct2[2]=ctab[(px>>20)&3];	ct2[3]=ctab[(px>>22)&3];
		ct3[0]=ctab[(px>>24)&3];	ct3[1]=ctab[(px>>26)&3];
		ct3[2]=ctab[(px>>28)&3];	ct3[3]=ctab[(px>>30)&3];
		
		cs+=8;
		ct0+=4;		ct1+=4;
		ct2+=4;		ct3+=4;
	}
}

void TKGDI_CopyPixelSpan_ImageCnvUTX2to15(
	u16 *dst, byte *src, int xs, int ys)
{
	byte *cs;
	u16 *ct;
	int xs4, ys4;
	int bx, by;
	
	xs4=xs>>2;
	ys4=ys>>2;
	cs=src;
	ct=dst;
	
	for(by=0; by<ys4; by++)
	{
		TKGDI_CopyPixelSpan_RowCnvUTX2to15(ct, cs, xs);
		cs+=xs4<<3; ct+=xs*4;
	}
}


/*
Generate a 1bpp bitmask from an SDF image.
SDF is stored at 8 bits/pixel, where:
  The low 4 bits hold the X distance;
  The high 4 bits hold the Y distance.
 */

void TKGDI_InterpImageSdfBits(
	byte *dstbits, int dxs, int dys,
	byte *srcsdf, int sxs, int sys,
	int sxm, int sym, int sxn, int syn)
{
	int p0, p1, p2, p3;
	int px0, px1, px, py, p;
	int base_sx, base_sy;
	int step_sx, step_sy;
	int cur_sx, cur_sy;
	int dx, dy, di, sx, sy, frx, fry;
	
	if((dxs<1) || (dys<1))
		return;
	
	base_sx=sxm<<16;
	base_sy=sym<<16;
	step_sx=((sxn-sxm)<<16)/dxs;
	step_sy=((syn-sym)<<16)/dys;

	cur_sy=base_sy;
	for(dy=0; dy<dys; dy++)
	{
		cur_sx=base_sx;
		for(dx=0; dx<dxs; dx++)
		{
			sx=cur_sx>>16;
			sy=cur_sy>>16;
			frx=cur_sx&0xFFFF;
			fry=cur_sy&0xFFFF;
			
			p0=srcsdf[(sy+0)*sxs+(sx+0)];
			p1=srcsdf[(sy+0)*sxs+(sx+1)];
			p2=srcsdf[(sy+1)*sxs+(sx+0)];
			p3=srcsdf[(sy+1)*sxs+(sx+1)];
			di=dy*dxs+dx;
			
			px0=((p0 & 15)*(0xFFFF-frx))+((p1 & 15)*frx);
			px1=((p2 & 15)*(0xFFFF-frx))+((p3 & 15)*frx);
			px =((px0>>12)*(0xFFFF-fry))+((px1>>12)*fry);

			px0=((p0 >> 4)*(0xFFFF-frx))+((p1 >> 4)*frx);
			px1=((p2 >> 4)*(0xFFFF-frx))+((p3 >> 4)*frx);
			py =((px0>>12)*(0xFFFF-fry))+((px1>>12)*fry);
			
			p=(px+py)>>17;
			p=(p*17)>>4;

			if(p>=0x80)
				dstbits[di>>3]|=1<<(di&7);
			else
				dstbits[di>>3]&=~(1<<(di&7));
			
			cur_sx+=step_sx;
		}
		cur_sy+=step_sy;
	}
}

void TKGDI_InterpImageSdfBits2(
	byte *dstbits, int dxs, int dys,
	byte *srcsdf, byte *pal, int sxs, int sys,
	int sxm, int sym, int sxn, int syn)
{
	int p0, p1, p2, p3;
	int cx0, cx1, cx2, cx3, cx4;
	int px0, px1, px, py, pz, p;
	int base_sx, base_sy;
	int step_sx, step_sy;
	int cur_sx, cur_sy;
	int dx, dy, di, sx, sy, sx1, sy1, frx, fry, frz;
	
	if((dxs<1) || (dys<1))
		return;
	
	base_sx=sxm<<16;
	base_sy=sym<<16;
	step_sx=((sxn-sxm)<<16)/dxs;
	step_sy=((syn-sym)<<16)/dys;
	
	frz=(step_sx+step_sy)>>1;
	if(frz>=0x10000)
	{
		frz=frz-0x10000;
		if(frz>0xFFFF)
			frz=0xFFFF;
	}else
	{
		frz=0;
	}

	cur_sy=base_sy;
	for(dy=0; dy<dys; dy++)
	{
		cur_sx=base_sx;
		for(dx=0; dx<dxs; dx++)
		{
			sx=cur_sx>>16;
			sy=cur_sy>>16;
			frx=cur_sx&0xFFFF;
			fry=cur_sy&0xFFFF;

#if 1
			if(frz)
			{
				frx=((frx>>8)*(0xFFFF-frz))+(0x80*frz);
				fry=((fry>>8)*(0xFFFF-frz))+(0x80*frz);
				frx=frx>>8;
				fry=fry>>8;
			}
#endif

			sx1=sx+1;
			sy1=sy+1;
			if(sx1>=sxn)
				sx1=sx;
			if(sy1>=syn)
				sy1=sy;
			
			p0=srcsdf[(sy+0)*sxs+(sx+0)];
			p1=srcsdf[(sy+0)*sxs+(sx+1)];
			p2=srcsdf[(sy+1)*sxs+(sx+0)];
			p3=srcsdf[(sy+1)*sxs+(sx+1)];
			di=dy*dxs+dx;
			
			cx0=pal[p0*4+0];	cx1=pal[p1*4+0];
			cx2=pal[p2*4+0];	cx3=pal[p3*4+0];
			cx4=(cx0+cx1+cx2+cx3)>>2;
			px0=(cx0*(0xFFFF-frx))+(cx1*frx);
			px1=(cx2*(0xFFFF-frx))+(cx3*frx);
			px =((px0>>16)*(0xFFFF-fry))+((px1>>16)*fry);
//			if(frz)
//				px =((px>>16)*(0xFFFF-frz))+(cx4*frz);

			cx0=pal[p0*4+1];	cx1=pal[p1*4+1];
			cx2=pal[p2*4+1];	cx3=pal[p3*4+1];			
			cx4=(cx0+cx1+cx2+cx3)>>2;
			px0=(cx0*(0xFFFF-frx))+(cx1*frx);
			px1=(cx2*(0xFFFF-frx))+(cx3*frx);
			py =((px0>>16)*(0xFFFF-fry))+((px1>>16)*fry);
//			if(frz)
//				py =((py>>16)*(0xFFFF-frz))+(cx4*frz);

			cx0=pal[p0*4+2];	cx1=pal[p1*4+2];
			cx2=pal[p2*4+2];	cx3=pal[p3*4+2];			
			cx4=(cx0+cx1+cx2+cx3)>>2;
			px0=(cx0*(0xFFFF-frx))+(cx1*frx);
			px1=(cx2*(0xFFFF-frx))+(cx3*frx);
			pz =((px0>>16)*(0xFFFF-fry))+((px1>>16)*fry);
//			if(frz)
//				pz =((pz>>16)*(0xFFFF-frz))+(cx4*frz);
			
//			p=(px+py)>>17;

			if(px>py)
				{ p=px; px=py; py=p; }
			if(py>pz)
				{ p=py; py=pz; pz=p; }
			if(px>py)
				{ p=px; px=py; py=p; }
			p=py>>16;

//			p=(p*17)>>4;
			
			if(p>=0x80)
				dstbits[di>>3]|=1<<(di&7);
			else
				dstbits[di>>3]&=~(1<<(di&7));
			
			cur_sx+=step_sx;
		}
		cur_sy+=step_sy;
	}
}

TK_WadImage *tkgdi_unisdf_wad;
byte *tkgdi_unisdf_glyphcache_bits;
int *tkgdi_unisdf_glyphcache_idx;
int tkgdi_unisdf_init;

TK_WadImage *TK_Wad4_OpenImage(TK_FILE *fd);
void *TK_Wad4_CacheLumpPath(TK_WadImage *img, char *path, int *rsz);

void TKGDI_UnifontInit()
{
	TK_FILE *fd;

	if(tkgdi_unisdf_init)
		return;
	tkgdi_unisdf_init=1;
	
	fd=tk_fopen("/boot/tksys/unisdf.wad", "rb");
	if(fd)
	{
		tkgdi_unisdf_wad=TK_Wad4_OpenImage(fd);
		if(!tkgdi_unisdf_wad)
		{
			tk_dbg_printf("TKGDI_UnifontInit: WAD Open Fail\n");
		}
	}else
	{
		tk_dbg_printf("TKGDI_UnifontInit: No WAD\n");
	}
	
	tkgdi_unisdf_glyphcache_bits=tk_malloc(256*128);
	tkgdi_unisdf_glyphcache_idx=tk_malloc(256*4);
	memset(tkgdi_unisdf_glyphcache_idx, 0, 256*4);
}

void TKGDI_FetchUnifontSdfBits(
	byte *dstbits, int dxs, int dys,
	int fontid, int codepoint)
{
	char tb[64];
	byte *bmp, *dat, *pal;
	int kidx, kh, ksz;
	int bmpsz, pgn, ofs, px, py;
	
	TKGDI_UnifontInit();

	ksz=(dxs*dys+7)>>3;
	
	if(!tkgdi_unisdf_wad)
	{
//		tk_dbg_printf("TKGDI_FetchUnifontSdfBits: No WAD, ch=%04X\n", codepoint);
		memset(dstbits, 0, ksz);
		return;
	}
	
	kidx=(codepoint&0xFFFF)|(dxs<<16)|(dys<<24);
	kh=kidx^(kidx>>17);
	kh=kh^(kh>>7);
	kh=kh&255;
	
	if((dxs<=32) && (dys<=32) && (tkgdi_unisdf_glyphcache_idx[kh]==kidx))
	{
		memcpy(dstbits, tkgdi_unisdf_glyphcache_bits+kh*128, ksz);
		return;
	}
	
	pgn=(codepoint>>8)&255;
	sprintf(tb, "fixsdf%u.bmp", pgn);
	
	bmpsz=0;
	bmp=TK_Wad4_CacheLumpPath(tkgdi_unisdf_wad, tb, &bmpsz);
	if(!bmp)
	{
		if((dxs<=32) && (dys<=32))
		{
			tkgdi_unisdf_glyphcache_idx[kh]=kidx;
			memset(tkgdi_unisdf_glyphcache_bits+kh*128, 0, ksz);
		}
		tk_dbg_printf("TKGDI_FetchUnifontSdfBits: No BMP, ch=%04X\n", codepoint);
		memset(dstbits, 0, ksz);
		return;
	}

	if((bmp[0]!='B') || (bmp[1]!='M'))
	{
		tk_dbg_printf("TKGDI_FetchUnifontSdfBits: Not BMP, ch=%04X\n",
			codepoint);
		memset(dstbits, 0, ksz);
		return;
	}
	ofs=bmp[10]|(bmp[11]<<8);
	dat=bmp+ofs;

	pal=bmp+0x36;		//assume simple 8-bit indexed color...
	
	px=(codepoint>>0)&15;
	py=(codepoint>>4)&15;
	py=15-py;

//	TKGDI_InterpImageSdfBits(
//		dstbits, dxs, dys,
//		dat, 256, 256,
//		px*16, py*16, (px+1)*16, (py+1)*16);

	TKGDI_InterpImageSdfBits2(
		dstbits, dxs, dys,
		dat, pal, 256, 256,
		px*16, py*16, (px+1)*16, (py+1)*16);

	tk_dbg_printf("TKGDI_FetchUnifontSdfBits: OK, ch=%04X\n", codepoint);

	if((dxs<=32) && (dys<=32))
	{
		tkgdi_unisdf_glyphcache_idx[kh]=kidx;
		memcpy(tkgdi_unisdf_glyphcache_bits+kh*128, dstbits, ksz);
	}
}
