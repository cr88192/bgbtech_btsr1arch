#ifdef __BJX2__

#define tkra_morton8(x, y)		__int_pmortq(y, x)
#define tkra_morton16(x, y)		__int_pmortq(y, x)

// #define tkra_paddhw(x, y)		__int64_paddw(y, x)
// #define tkra_pmuluhw(x, y)		__int64_pmulhw(y, x)

// int tkra_morton8(int x, int y);
// int tkra_morton16(int x, int y);
u64 tkra_paddhw(u64 a, u64 b);
u64 tkra_pmuluhw(u64 a, u64 b);
u64 tkra_rgbupck64(u16 a);
u16 tkra_rgbpck64(u64 a);

u64 TKRA_CachedBlkUtx2(void *src, int ix);
u64 TKRA_CachedBlkUtx3(void *src, int ix);

u64 TKRA_InterpBilinear(
	u16 px0, u16 px1, u16 px2, u16 px3,
	u16 xfrac, u16 yfrac);
u64 TKRA_InterpBilinear64(
	u64 px0, u64 px1, u64 px2, u64 px3,
	u16 xfrac, u16 yfrac);

u64 TKRA_InterpBilinear3Pt_64(
	u64 px0, u64 px1, u64 px2,
	u16 xfrac, u16 yfrac);

__asm {
.global tkra_morton8
.global tkra_morton16
.global tkra_paddhw
.global tkra_pmuluhw
.global tkra_rgbupck64
.global tkra_rgbpck64
.global tkra_padduhw_sat
.global TKRA_CachedBlkUtx2
.global TKRA_CachedBlkUtx3
.global TKRA_InterpBilinear
.global TKRA_InterpBilinear64
.global TKRA_InterpBilinear3Pt_64

tkra_morton8:
tkra_morton16:
	MOVLD	R5, R4, R6
	PMORT.Q R6, R2
	RTS

tkra_paddhw:
	PADD.W	R4, R5, R2
	RTS

tkra_pmuluhw:
	PMULU.HW	R4, R5, R2
	RTS

tkra_rgbupck64:
	RGB5UPCK64	R4, R2
	RTS

tkra_rgbpck64:
	RGB5PCK64	R4, R2
	RTS

tkra_padduhw_sat:
	MOV		0x7FFE7FFE7FFE7FFE, R6
	SHLD.Q	R4, -1, R4		|	SHLD.Q	R5, -1, R5
	AND		R4, R6, R4		|	AND		R5, R6, R5
	ADD		R4, R5, R2

	TSTQ	0x8000000000000000, R2
	OR?F	0xFFFF000000000000, R2
	TSTQ	0x0000800000000000, R2
	OR?F	0x0000FFFF00000000, R2
	TSTQ	0x0000000080000000, R2
	OR?F	0x00000000FFFF0000, R2
	TSTQ	0x0000000000008000, R2
	OR?F	0x000000000000FFFF, R2

	ADD		R2, R2
	RTSU

TKRA_CachedBlkUtx2:
	SHAD		R5, -4, R6
	MOV.Q		(R4, R6), R7
	BLKUTX2		R7, R5, R2
	RTS

TKRA_CachedBlkUtx3:
	SHAD		R5, -4, R6
	ADD			R6, R6, R7
	MOV			R5, R18
	MOV.X		(R4, R7), R16
	BLKUTX3		R16, R18, R2
	RTS

TKRA_InterpBilinear:
	RGB5UPCK64	R4, R4
	RGB5UPCK64	R5, R5
	RGB5UPCK64	R6, R6
	RGB5UPCK64	R7, R7

TKRA_InterpBilinear64:
	PSHUF.W		R20, 0, R22
	PSHUF.W		R21, 0, R20
	NOT			R22, R23		|	NOT			R20, R21
	PMULU.HW	R4, R23, R16	|	PMULU.HW	R5, R22, R17
	PMULU.HW	R6, R23, R18	|	PMULU.HW	R7, R22, R19
	PADD.W		R16, R17, R2	|	PADD.W		R18, R19, R3
	PMULU.HW	R2, R21, R16	|	PMULU.HW	R3, R20, R17
	PADD.W		R16, R17, R2
	RTS

TKRA_InterpBilinear3Pt_64:
	MOV			0x7FFF, R3		|
	SHLD		R7, -1, R7		|	SHLD		R20, -1, R20
	AND			R3, R7			|	AND			R3, R20
	PSHUF.W		R7, 0, R22		|	PSHUF.W		R20, 0, R20
	PADD.W		R20, R22, R23
	NOT			R23, R23		|	PMULU.HW	R6, R20, R18
	PMULU.HW	R4, R23, R16	|	PMULU.HW	R5, R22, R17
	PADD.W		R16, R17, R3
	PADD.W		R18, R3, R2
	RTS

};


void TKRA_DrawSpan_DirClr(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);

__asm {
//R4=Parm
//R5=dstc
//R6=dstz
//R7=cnt
TKRA_DrawSpan_DirClr:
	MOV.Q	(R4, TKRA_DS_CPOS*8), R22
	MOV.Q	(R4, TKRA_DS_CSTEP*8), R23
	LEA.W	(R5, R7), R20
	.L0:
	RGB5PCK64	R22, R2
	ADD			R23, R22
	MOV.W		R2, (R5)
	ADD			2, R5
	CMPQGT		R5, R20
	BT			.L0
	RTSU
};

// #if 0

void TKRA_DrawSpan_DirTex(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);

__asm {
TKRA_DrawSpan_DirTex:
	ADD		-64, SP
	MOV.X	R30, (SP, 48)
	MOV.X	R28, (SP, 32)
//	MOV.X	R26, (SP, 16)
//	MOV.X	R24, (SP,  0)

	MOV.Q	(R4, TKRA_DS_TEXIMG*8), R20
	MOV.Q	(R4, TKRA_DS_TPOS*8), R28
	MOV.Q	(R4, TKRA_DS_TSTEP*8), R29
	MOV.Q	(R4, TKRA_DS_XMASK*8), R30
	MOV.Q	(R4, TKRA_DS_YMASK*8), R31
	LEA.W	(R5, R7), R21

	.L0:	
	SHLD.Q	R28, -16, R3	|	SHLD.Q		R28, -32, R2
	AND		R3, R30, R3		|	AND			R2, R31, R2
	ADD		R29, R28		|	OR			R2, R3
	MOV.W	(R20, R3), R2

	MOV.W	R2, (R5)
	ADD		2, R5
	CMPQGT	R5, R21
	BT		.L0

//	MOV.X	(SP,  0), R24
//	MOV.X	(SP, 16), R26
	MOV.X	(SP, 32), R28
	MOV.X	(SP, 48), R30
	ADD		64, SP
	RTSU
};

void TKRA_DrawSpan_ModTex(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_ModTexMort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_AlphaModTexMort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_AtestModTexMort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);

void TKRA_DrawSpan_ModBlTexMort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_AlphaModBlTexMort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_AtestModBlTexMort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);

void TKRA_DrawSpan_ModUtx2Mort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_AlphaModUtx2Mort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_AtestModUtx2Mort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);

void TKRA_DrawSpan_ModBlUtx2Mort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_AlphaModBlUtx2Mort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_AtestModBlUtx2Mort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);

__asm {
TKRA_DrawSpan_ModTex:
	ADD		-64, SP
	MOV.X	R30, (SP, 48)
	MOV.X	R28, (SP, 32)
	MOV.X	R26, (SP, 16)
	MOV.X	R24, (SP,  0)

	MOV.Q	(R4, TKRA_DS_TEXIMG*8), R20

	MOV.Q	(R4, TKRA_DS_CPOS*8), R26
	MOV.Q	(R4, TKRA_DS_CSTEP*8), R27
	MOV.Q	(R4, TKRA_DS_TPOS*8), R28
	MOV.Q	(R4, TKRA_DS_TSTEP*8), R29
	MOV.Q	(R4, TKRA_DS_XMASK*8), R30
	MOV.Q	(R4, TKRA_DS_YMASK*8), R31
	LEA.W	(R5, R7), R21

	.L0:	
	SHLD.Q		R28, -16, R3	|	SHLD.Q		R28, -32, R2
	AND			R3, R30, R3		|	AND			R2, R31, R2
	ADD			R29, R28		|	OR			R2, R3
	MOV.W		(R20, R3), R2
	RGB5UPCK64	R2, R2
	PMULU.HW	R2, R26, R2
	ADD			R27, R26		| RGB5PCK64		R2, R2
	MOV.W		R2, (R5)
	ADD			2, R5
	CMPQGT		R5, R21
	BT			.L0

	MOV.X	(SP,  0), R24
	MOV.X	(SP, 16), R26
	MOV.X	(SP, 32), R28
	MOV.X	(SP, 48), R30
	ADD		64, SP
	RTSU

TKRA_DrawSpan_ModTexMort:
	ADD		-64, SP
	MOV.X	R30, (SP, 48)
	MOV.X	R28, (SP, 32)
	MOV.X	R26, (SP, 16)
	MOV.X	R24, (SP,  0)

	MOV.Q	(R4, TKRA_DS_TEXIMG*8), R20

	MOV.Q	(R4, TKRA_DS_CPOS *8), R26
	MOV.Q	(R4, TKRA_DS_CSTEP*8), R27
	MOV.Q	(R4, TKRA_DS_TPOS *8), R28
	MOV.Q	(R4, TKRA_DS_TSTEP*8), R29
	MOV.Q	(R4, TKRA_DS_XMASK*8), R30
	MOV.Q	(R4, TKRA_DS_YMASK*8), R31
	LEA.W	(R5, R7), R21

	.L0:	
//	SHLD.Q		R28, -16, R3	|	SHLD.Q		R28, -32, R2
//	AND			R3, R30, R3		|	AND			R2, R31, R2
	PMORT.L		R28, R2
	ADD			R29, R28		|	AND			R2, R31, R3
	MOV.W		(R20, R3), R2
	RGB5UPCK64	R2, R2
	PMULU.HW	R2, R26, R2
	ADD			R27, R26		| RGB5PCK64		R2, R2
	MOV.W		R2, (R5)
	ADD			2, R5
	CMPQGT		R5, R21
	BT			.L0

	MOV.X	(SP,  0), R24
	MOV.X	(SP, 16), R26
	MOV.X	(SP, 32), R28
	MOV.X	(SP, 48), R30
	ADD		64, SP
	RTSU


TKRA_DrawSpan_AlphaModTexMort:
	ADD		-64, SP
	MOV.X	R30, (SP, 48)
	MOV.X	R28, (SP, 32)
	MOV.X	R26, (SP, 16)
	MOV.X	R24, (SP,  0)
	
	MOV.Q	(R4, TKRA_DS_TEXIMG*8), R20

//	MOV.Q	(R4, TKRA_DS_ZPOS *8), R24
//	MOV.Q	(R4, TKRA_DS_ZSTEP*8), R25
	MOV.Q	(R4, TKRA_DS_CPOS *8), R26
	MOV.Q	(R4, TKRA_DS_CSTEP*8), R27
	MOV.Q	(R4, TKRA_DS_TPOS *8), R28
	MOV.Q	(R4, TKRA_DS_TSTEP*8), R29
//	MOV.Q	(R4, TKRA_DS_XMASK*8), R30
	MOV.Q	(R4, TKRA_DS_YMASK*8), R31
	LEA.W	(R5, R7), R21

/*
R2: Scratch
R3: Scratch
R4: Span State
R5: Dest Pixel
R6: Dest ZBuf
R7: Count
R8: Scratch
R9: Scratch
R10: Scratch
R11: Scratch
R12: Scratch
R13: Scratch
R14: Scratch
R15: SP
R16: Scratch
R17: Scratch / ZPos
R18: Pixel A / TexPos A
R19: Pixel B / TexPos B
R20: Texture Pixels
R21: Span End Address
R22: Scratch / Next Pointer
R23: Scratch
R24: ZPos
R25: ZStep
R26: CPos
R27: CStep
R28: TPos
R29: TStep
R30: -
R31: X/Y Mask
 */

	.L1:
//	CMPQGT		R5, R21
//	BF			.L2

	PMORT.L		R28, R18		|	MOVU.W		(R5), R19
	ADD			R29, R28		|	AND			R18, R31, R3
									MOV.W		(R20, R3), R18
//	RGB5UPCK64	R18, R18		|	PSHUF.W		R26, 0xFF, R22
//	RGB5UPCK64	R19, R19		|	NOT			R22, R23
	RGB5UPCK64	R18, R18		|	RGB5UPCK64	R19, R19
	PMULU.HW	R18, R26, R18
	PSHUF.W		R18, 0xFF, R22
	NOT			R22, R23
	PMULU.HW	R18, R22, R2	|	PMULU.HW	R19, R23, R3
//	SHAD.Q		R24, -16, R17	|	
	PADD.W		R2, R3, R18
	PADD.W		R26, R27, R26	|	RGB5PCK64	R18, R18
//	ADD			R25, R24		|	MOVU.W		(R6), R16
//									CMPHI		R16, R17
//									MOV.W?F		R18, (R5)
									MOV.W		R18, (R5)

	ADD			2, R5
	TEST		2, R5
	ADD?T		2, R6

	CMPQGT		R5, R21
	BT			.L1

	.L2:

	MOV.X	(SP,  0), R24
	MOV.X	(SP, 16), R26
	MOV.X	(SP, 32), R28
	MOV.X	(SP, 48), R30
	ADD		64, SP
	RTSU

TKRA_DrawSpan_AtestModTexMort:
	ADD		-64, SP
	MOV.X	R30, (SP, 48)
	MOV.X	R28, (SP, 32)
	MOV.X	R26, (SP, 16)
	MOV.X	R24, (SP,  0)

	MOV.Q	(R4, TKRA_DS_TEXIMG*8), R20

	MOV.X	(R4, TKRA_DS_CPOS *8), R26
	MOV.X	(R4, TKRA_DS_TPOS *8), R28
//	MOV.Q	(R4, TKRA_DS_XMASK*8), R30
	MOV.Q	(R4, TKRA_DS_YMASK*8), R31
	LEA.W	(R5, R7), R21

	.L0:	
//	SHLD.Q		R28, -16, R3	|	SHLD.Q		R28, -32, R2
//	AND			R3, R30, R3		|	AND			R2, R31, R2
	PMORT.L		R28, R2
	ADD			R29, R28		|	AND			R2, R31, R3
	MOV.W		(R20, R3), R2
	RGB5UPCK64	R2, R2
	PMULU.HW	R2, R26, R2
	SHLD.Q		R2, -63, R3
	ADD			R27, R26		| RGB5PCK64		R2, R2
	TST			1, R3
	MOV.W?F		R2, (R5)
	ADD			2, R5
	CMPQGT		R5, R21
	BT			.L0

	MOV.X	(SP,  0), R24
	MOV.X	(SP, 16), R26
	MOV.X	(SP, 32), R28
	MOV.X	(SP, 48), R30
	ADD		64, SP
	RTSU


TKRA_DrawSpan_ModBlTexMort:
	ADD		-128, SP
	MOV.X	R30, (SP, 112)
	MOV.X	R28, (SP,  96)
	MOV.X	R26, (SP,  80)
	MOV.X	R24, (SP,  64)
	MOV.Q	R14, (SP,  48)
	MOV.X	R12, (SP,  32)
	MOV.X	R10, (SP,  16)
	MOV.X	R8 , (SP,   0)

	MOV.Q	(R4, TKRA_DS_TEXIMG*8), R20
	MOV.X	(R4, TKRA_DS_ZPOS *8), R24
	MOV.X	(R4, TKRA_DS_CPOS *8), R26
	MOV.X	(R4, TKRA_DS_TPOS *8), R28
	MOV.Q	(R4, TKRA_DS_YMASK*8), R31
	LEA.W	(R5, R7), R21

	.L1:
	MOV			0x00000000FFFF, R8
	MOV			0xFFFF00000000, R9
	ADD			R28, R8, R17	|	PMORT.L		R28, R18
	AND			R18, R31, R2	|	PMORT.L		R17, R19
	AND			R19, R31, R3	|	MOV.W		(R20, R2), R18
	ADD			R28, R9, R16	|	MOV.W		(R20, R3), R19
	ADD			R16, R8, R17	|	PMORT.L		R16, R22
	AND			R22, R31, R22	|	PMORT.L		R17, R23
	AND			R23, R31, R23	|	MOV.W		(R20, R22), R10
	MOV			R28, R16		|	MOV.W		(R20, R23), R11
	ADD			R29, R28		|	PSHUF.W		R16, 0, R22
	RGB5UPCK64	R18, R8			|	RGB5UPCK64	R19, R9
	NOT			R22, R23		|	PSHUF.W		R16, 0xAA, R18
	NOT			R18, R19		|
	RGB5UPCK64	R10, R10		|	RGB5UPCK64	R11, R11
	PMULU.HW	R8, R23, R12	|	PMULU.HW	R9, R22, R13
	PMULU.HW	R10, R23, R14	|	PMULU.HW	R11, R22, R30
	PADD.W		R12, R13, R8	|	PADD.W		R14, R30, R9
	PMULU.HW	R8, R19, R12	|	PMULU.HW	R9, R18, R13
									PADD.W		R12, R13, R18
									PMULU.HW	R18, R26, R18
	ADD			R27, R26		|	RGB5PCK64	R18, R18
									MOV.W		R18, (R5)
	ADD			2, R5

	CMPQGT		R5, R21
	BT			.L1

	.L2:

	MOV.X	(SP,   0), R8
	MOV.X	(SP,  16), R10
	MOV.X	(SP,  32), R12
	MOV.Q	(SP,  48), R14
	MOV.X	(SP,  64), R24
	MOV.X	(SP,  80), R26
	MOV.X	(SP,  96), R28
	MOV.X	(SP, 112), R30
	ADD		128, SP
	RTSU

TKRA_DrawSpan_AlphaModBlTexMort:
	ADD		-128, SP
	MOV.X	R30, (SP, 112)
	MOV.X	R28, (SP,  96)
	MOV.X	R26, (SP,  80)
	MOV.X	R24, (SP,  64)
	MOV.Q	R14, (SP,  48)
	MOV.X	R12, (SP,  32)
	MOV.X	R10, (SP,  16)
	MOV.X	R8 , (SP,   0)

	MOV.Q	(R4, TKRA_DS_TEXIMG*8), R20
	MOV.X	(R4, TKRA_DS_ZPOS *8), R24
	MOV.X	(R4, TKRA_DS_CPOS *8), R26
	MOV.X	(R4, TKRA_DS_TPOS *8), R28
	MOV.Q	(R4, TKRA_DS_YMASK*8), R31
	LEA.W	(R5, R7), R21

	.L1:
	MOV			0x00000000FFFF, R8
	MOV			0xFFFF00000000, R9
	ADD			R28, R8, R17	|	PMORT.L		R28, R18
	AND			R18, R31, R2	|	PMORT.L		R17, R19
	AND			R19, R31, R3	|	MOV.W		(R20, R2), R18
	ADD			R28, R9, R16	|	MOV.W		(R20, R3), R19
	ADD			R16, R8, R17	|	PMORT.L		R16, R22
	AND			R22, R31, R22	|	PMORT.L		R17, R23
	AND			R23, R31, R23	|	MOV.W		(R20, R22), R10
	MOV			R28, R16		|	MOV.W		(R20, R23), R11
	ADD			R29, R28		|	PSHUF.W		R16, 0, R22
	RGB5UPCK64	R18, R8			|	RGB5UPCK64	R19, R9
	NOT			R22, R23		|	PSHUF.W		R16, 0xAA, R18
	NOT			R18, R19		|
	RGB5UPCK64	R10, R10		|	RGB5UPCK64	R11, R11
	PMULU.HW	R8, R23, R12	|	PMULU.HW	R9, R22, R13
	PMULU.HW	R10, R23, R14	|	PMULU.HW	R11, R22, R30
	PADD.W		R12, R13, R8	|	PADD.W		R14, R30, R9
	PMULU.HW	R8, R19, R12	|	PMULU.HW	R9, R18, R13
	PADD.W		R12, R13, R18	|	MOV.W		(R5), R19
									PMULU.HW	R18, R26, R18
									PSHUF.W		R18, 0xFF, R22
	RGB5UPCK64	R19, R19		|	NOT			R22, R23
	PMULU.HW	R18, R22, R12	|	PMULU.HW	R19, R23, R13
	ADD			R27, R26		|	PADD.W		R12, R13, R18
									RGB5PCK64	R18, R18
									MOV.W		R18, (R5)
	ADD			2, R5
	CMPQGT		R5, R21
	BT			.L1

	.L2:

	MOV.X	(SP,   0), R8
	MOV.X	(SP,  16), R10
	MOV.X	(SP,  32), R12
	MOV.Q	(SP,  48), R14
	MOV.X	(SP,  64), R24
	MOV.X	(SP,  80), R26
	MOV.X	(SP,  96), R28
	MOV.X	(SP, 112), R30
	ADD		128, SP
	RTSU

TKRA_DrawSpan_AtestModBlTexMort:
	ADD		-128, SP
	MOV.X	R30, (SP, 112)
	MOV.X	R28, (SP,  96)
	MOV.X	R26, (SP,  80)
	MOV.X	R24, (SP,  64)
	MOV.Q	R14, (SP,  48)
	MOV.X	R12, (SP,  32)
	MOV.X	R10, (SP,  16)
	MOV.X	R8 , (SP,   0)

	MOV.Q	(R4, TKRA_DS_TEXIMG*8), R20
	MOV.X	(R4, TKRA_DS_ZPOS *8), R24
	MOV.X	(R4, TKRA_DS_CPOS *8), R26
	MOV.X	(R4, TKRA_DS_TPOS *8), R28
	MOV.Q	(R4, TKRA_DS_YMASK*8), R31
	LEA.W	(R5, R7), R21

	.L1:
	MOV			0x00000000FFFF, R8
	MOV			0xFFFF00000000, R9
	ADD			R28, R8, R17	|	PMORT.L		R28, R18
	AND			R18, R31, R2	|	PMORT.L		R17, R19
	AND			R19, R31, R3	|	MOV.W		(R20, R2), R18
	ADD			R28, R9, R16	|	MOV.W		(R20, R3), R19
	ADD			R16, R8, R17	|	PMORT.L		R16, R22
	AND			R22, R31, R22	|	PMORT.L		R17, R23
	AND			R23, R31, R23	|	MOV.W		(R20, R22), R10
	MOV			R28, R16		|	MOV.W		(R20, R23), R11
	ADD			R29, R28		|	PSHUF.W		R16, 0, R22
	RGB5UPCK64	R18, R8			|	RGB5UPCK64	R19, R9
	NOT			R22, R23		|	PSHUF.W		R16, 0xAA, R18
	NOT			R18, R19		|
	RGB5UPCK64	R10, R10		|	RGB5UPCK64	R11, R11
	PMULU.HW	R8, R23, R12	|	PMULU.HW	R9, R22, R13
	PMULU.HW	R10, R23, R14	|	PMULU.HW	R11, R22, R30
	PADD.W		R12, R13, R8	|	PADD.W		R14, R30, R9
	PMULU.HW	R8, R19, R12	|	PMULU.HW	R9, R18, R13
									PADD.W		R12, R13, R18
									PMULU.HW	R18, R26, R18
									SHLD.Q		R18, -63, R3
	ADD			R27, R26		|	RGB5PCK64	R18, R18
									TST			1, R3
									MOV.W?F		R18, (R5)
	ADD			2, R5

	CMPQGT		R5, R21
	BT			.L1

	.L2:

	MOV.X	(SP,   0), R8
	MOV.X	(SP,  16), R10
	MOV.X	(SP,  32), R12
	MOV.Q	(SP,  48), R14
	MOV.X	(SP,  64), R24
	MOV.X	(SP,  80), R26
	MOV.X	(SP,  96), R28
	MOV.X	(SP, 112), R30
	ADD		128, SP
	RTSU



TKRA_DrawSpan_ModUtx2Mort:
	ADD		-64, SP
	MOV.X	R30, (SP, 48)
	MOV.X	R28, (SP, 32)
	MOV.X	R26, (SP, 16)
	MOV.X	R24, (SP,  0)

	MOV.Q	(R4, TKRA_DS_TEXBCN*8), R20

	MOV.X	(R4, TKRA_DS_CPOS *8), R26
	MOV.X	(R4, TKRA_DS_TPOS *8), R28

//	MOV.Q	(R4, TKRA_DS_CPOS *8), R26
//	MOV.Q	(R4, TKRA_DS_CSTEP*8), R27
//	MOV.Q	(R4, TKRA_DS_TPOS *8), R28
//	MOV.Q	(R4, TKRA_DS_TSTEP*8), R29
//	MOV.Q	(R4, TKRA_DS_XMASK*8), R30
	MOV.Q	(R4, TKRA_DS_YMASK*8), R31
	LEA.W	(R5, R7), R21

	.L0:	
	PMORT.L		R28, R2
	ADD			R29, R28		|	AND			R2, R31, R3
//	MOV.W		(R20, R3), R2
//	RGB5UPCK64	R2, R2

	SHAD		R3, -4, R2
	MOV.Q		(R20, R2), R2
	BLKUTX2		R2, R3, R2

	PMULU.HW	R2, R26, R2
	ADD			R27, R26		| RGB5PCK64		R2, R2
	MOV.W		R2, (R5)
	ADD			2, R5
	CMPQGT		R5, R21
	BT			.L0

	MOV.X	(SP,  0), R24
	MOV.X	(SP, 16), R26
	MOV.X	(SP, 32), R28
	MOV.X	(SP, 48), R30
	ADD		64, SP
	RTSU


TKRA_DrawSpan_AlphaModUtx2Mort:
	ADD		-64, SP
	MOV.X	R30, (SP, 48)
	MOV.X	R28, (SP, 32)
	MOV.X	R26, (SP, 16)
	MOV.X	R24, (SP,  0)
	
	MOV.Q	(R4, TKRA_DS_TEXBCN*8), R20

	MOV.X	(R4, TKRA_DS_CPOS *8), R26
	MOV.X	(R4, TKRA_DS_TPOS *8), R28

//	MOV.Q	(R4, TKRA_DS_ZPOS *8), R24
//	MOV.Q	(R4, TKRA_DS_ZSTEP*8), R25
//	MOV.Q	(R4, TKRA_DS_CPOS *8), R26
//	MOV.Q	(R4, TKRA_DS_CSTEP*8), R27
//	MOV.Q	(R4, TKRA_DS_TPOS *8), R28
//	MOV.Q	(R4, TKRA_DS_TSTEP*8), R29
//	MOV.Q	(R4, TKRA_DS_XMASK*8), R30
	MOV.Q	(R4, TKRA_DS_YMASK*8), R31
	LEA.W	(R5, R7), R21

/*
R2: Scratch
R3: Scratch
R4: Span State
R5: Dest Pixel
R6: Dest ZBuf
R7: Count
R8: Scratch
R9: Scratch
R10: Scratch
R11: Scratch
R12: Scratch
R13: Scratch
R14: Scratch
R15: SP
R16: Scratch
R17: Scratch / ZPos
R18: Pixel A / TexPos A
R19: Pixel B / TexPos B
R20: Texture Pixels
R21: Span End Address
R22: Scratch / Next Pointer
R23: Scratch
R24: ZPos
R25: ZStep
R26: CPos
R27: CStep
R28: TPos
R29: TStep
R30: -
R31: X/Y Mask
 */

	.L1:
//	CMPQGT		R5, R21
//	BF			.L2

	PMORT.L		R28, R18		|	MOVU.W		(R5), R19
	ADD			R29, R28		|	AND			R18, R31, R3
//									MOV.W		(R20, R3), R18
//	RGB5UPCK64	R18, R18		|	
	SHAD		R3, -4, R18
	MOV.Q		(R20, R18), R18
	BLKUTX2		R18, R3, R18

	RGB5UPCK64	R19, R19
	PMULU.HW	R18, R26, R18
	PSHUF.W		R18, 0xFF, R22
	NOT			R22, R23
	PMULU.HW	R18, R22, R2	|	PMULU.HW	R19, R23, R3
//	SHAD.Q		R24, -16, R17	|	
	PADD.W		R2, R3, R18
	PADD.W		R26, R27, R26	|	RGB5PCK64	R18, R18
//	ADD			R25, R24		|	MOVU.W		(R6), R16
//									CMPHI		R16, R17
//									MOV.W?F		R18, (R5)
									MOV.W		R18, (R5)

	ADD			2, R5
	TEST		2, R5
	ADD?T		2, R6

	CMPQGT		R5, R21
	BT			.L1

	.L2:

	MOV.X	(SP,  0), R24
	MOV.X	(SP, 16), R26
	MOV.X	(SP, 32), R28
	MOV.X	(SP, 48), R30
	ADD		64, SP
	RTSU

#if 0
TKRA_DrawSpan_AtestModUtx2Mort:
	ADD		-64, SP
	MOV.X	R30, (SP, 48)
	MOV.X	R28, (SP, 32)
	MOV.X	R26, (SP, 16)
	MOV.X	R24, (SP,  0)

	MOV.Q	(R4, TKRA_DS_TEXBCN*8), R20
	MOV.X	(R4, TKRA_DS_CPOS *8), R26
	MOV.X	(R4, TKRA_DS_TPOS *8), R28
	MOV.Q	(R4, TKRA_DS_YMASK*8), R31
	LEA.W	(R5, R7), R21

	.L0:	
	PMORT.L		R28, R2
	ADD			R29, R28		|	AND			R2, R31, R3
	SHAD		R3, -4, R2
	MOV.Q		(R20, R2), R2
	BLKUTX2		R2, R3, R2

	PMULU.HW	R2, R26, R2
	SHLD.Q		R2, -63, R3
	ADD			R27, R26		| RGB5PCK64		R2, R2
	TST			1, R3
	MOV.W?F		R2, (R5)
	ADD			2, R5
	CMPQGT		R5, R21
	BT			.L0

	MOV.X	(SP,  0), R24
	MOV.X	(SP, 16), R26
	MOV.X	(SP, 32), R28
	MOV.X	(SP, 48), R30
	ADD		64, SP
	RTSU
#endif

#if 1
TKRA_DrawSpan_AtestModUtx2Mort:
	MOV.X	(R4, TKRA_DS_CPOS *8), R16
	MOV.X	(R4, TKRA_DS_TPOS *8), R18
	MOV.Q	(R4, TKRA_DS_TEXBCN*8), R20
	MOV.Q	(R4, TKRA_DS_YMASK*8), R22
	LEA.W	(R5, R7), R21

#if 1
	LDTEX		(R20, R18), R2
#endif

	.L0:
.ifarch has_ldtex
#if 1
	PMULU.HW	R2, R16, R2
	ADD			R19, R18		|	SHLD.Q		R2, -63, R3
	ADD			R17, R16		|	RGB5PCK64	R2, R2
	TST			1, R3
	MOV.W?F		R2, (R5)
	ADD			2, R5			|	LDTEX		(R20, R18), R2
#endif
.else
#if 1
	PMORT.L		R18, R2
	ADD			R19, R18		|	AND			R2, R22, R3
	SHAD		R3, -4, R2
	MOV.Q		(R20, R2), R2
	BLKUTX2		R2, R3, R2

	PMULU.HW	R2, R16, R2
	SHLD.Q		R2, -63, R3
	ADD			R17, R16		| RGB5PCK64		R2, R2
	TST			1, R3
	MOV.W?F		R2, (R5)
	ADD			2, R5
#endif
.endif

	CMPQGT		R5, R21
	BT			.L0

	RTSU
#endif

#if 1
TKRA_DrawSpan_ModBlUtx2Mort:
	ADD		-128, SP
	MOV.X	R30, (SP, 112)
	MOV.X	R28, (SP,  96)
	MOV.X	R26, (SP,  80)
	MOV.X	R24, (SP,  64)
	MOV.Q	R14, (SP,  48)
	MOV.X	R12, (SP,  32)
	MOV.X	R10, (SP,  16)
	MOV.X	R8 , (SP,   0)

	MOV.Q	(R4, TKRA_DS_TEXBCN*8), R20
	MOV.X	(R4, TKRA_DS_ZPOS *8), R24
	MOV.X	(R4, TKRA_DS_CPOS *8), R26
	MOV.X	(R4, TKRA_DS_TPOS *8), R28
	MOV.Q	(R4, TKRA_DS_YMASK*8), R31
	LEA.W	(R5, R7), R21

	.L1:
	MOV			0x00000000FFFF, R8
	MOV			0xFFFF00000000, R9
	ADD			R28, R8, R17	|	PMORT.L		R28, R18
	AND			R18, R31, R2	|	PMORT.L		R17, R19
	AND			R19, R31, R3	|	ADD			R28, R9, R16
	SHAD		R2, -4, R18		|	SHAD		R3, -4, R19
	ADD			R16, R8, R17	|	MOV.Q		(R20, R18), R18
	PMORT.L		R16, R22		|	MOV.Q		(R20, R19), R19
	AND			R22, R31, R22	|	BLKUTX2		R18, R2, R8
	PMORT.L		R17, R23		|	BLKUTX2		R19, R3, R9
	AND			R23, R31, R23	|	MOV			R28, R16
	SHAD		R22, -4, R10	|	SHAD		R23, -4, R11
									MOV.Q		(R20, R10), R10
									MOV.Q		(R20, R11), R11
									BLKUTX2		R10, R22, R10
									BLKUTX2		R11, R23, R11
	ADD			R29, R28		|	PSHUF.W		R16, 0, R22
	NOT			R22, R23		|	PSHUF.W		R16, 0xAA, R18
	NOT			R18, R19		|
	PMULU.HW	R8, R23, R12	|	PMULU.HW	R9, R22, R13
	PMULU.HW	R10, R23, R14	|	PMULU.HW	R11, R22, R30
	PADD.W		R12, R13, R8	|	PADD.W		R14, R30, R9
	PMULU.HW	R8, R19, R12	|	PMULU.HW	R9, R18, R13
									PADD.W		R12, R13, R18
									PMULU.HW	R18, R26, R18
	ADD			R27, R26		|	RGB5PCK64	R18, R18
									MOV.W		R18, (R5)
	ADD			2, R5
	CMPQGT		R5, R21
	BT			.L1

	.L2:
	MOV.X	(SP,   0), R8
	MOV.X	(SP,  16), R10
	MOV.X	(SP,  32), R12
	MOV.Q	(SP,  48), R14
	MOV.X	(SP,  64), R24
	MOV.X	(SP,  80), R26
	MOV.X	(SP,  96), R28
	MOV.X	(SP, 112), R30
	ADD		128, SP
	RTSU

TKRA_DrawSpan_AlphaModBlUtx2Mort:
	ADD		-128, SP
	MOV.X	R30, (SP, 112)
	MOV.X	R28, (SP,  96)
	MOV.X	R26, (SP,  80)
	MOV.X	R24, (SP,  64)
	MOV.Q	R14, (SP,  48)
	MOV.X	R12, (SP,  32)
	MOV.X	R10, (SP,  16)
	MOV.X	R8 , (SP,   0)

	MOV.Q	(R4, TKRA_DS_TEXBCN*8), R20
	MOV.X	(R4, TKRA_DS_ZPOS *8), R24
	MOV.X	(R4, TKRA_DS_CPOS *8), R26
	MOV.X	(R4, TKRA_DS_TPOS *8), R28
	MOV.Q	(R4, TKRA_DS_YMASK*8), R31
	LEA.W	(R5, R7), R21

	.L1:
	MOV			0x00000000FFFF, R8
	MOV			0xFFFF00000000, R9
	ADD			R28, R8, R17	|	PMORT.L		R28, R18
	AND			R18, R31, R2	|	PMORT.L		R17, R19
	AND			R19, R31, R3	|	ADD			R28, R9, R16

	SHAD		R2, -4, R18		|	SHAD		R3, -4, R19
	ADD			R16, R8, R17	|	MOV.Q		(R20, R18), R18
	PMORT.L		R16, R22		|	MOV.Q		(R20, R19), R19
	AND			R22, R31, R22	|	BLKUTX2		R18, R2, R8
	PMORT.L		R17, R23		|	BLKUTX2		R19, R3, R9

	AND			R23, R31, R23	|	MOV			R28, R16		
	SHAD		R22, -4, R10	|	SHAD		R23, -4, R11
									MOV.Q		(R20, R10), R10
									MOV.Q		(R20, R11), R11
									BLKUTX2		R10, R22, R10
									BLKUTX2		R11, R23, R11

	ADD			R29, R28		|	PSHUF.W		R16, 0, R22
	NOT			R22, R23		|	PSHUF.W		R16, 0xAA, R18
	NOT			R18, R19		|
	PMULU.HW	R8, R23, R12	|	PMULU.HW	R9, R22, R13
	PMULU.HW	R10, R23, R14	|	PMULU.HW	R11, R22, R30
	PADD.W		R12, R13, R8	|	PADD.W		R14, R30, R9
	PMULU.HW	R8, R19, R12	|	PMULU.HW	R9, R18, R13
	PADD.W		R12, R13, R18	|	MOV.W		(R5), R19
									PMULU.HW	R18, R26, R18
									PSHUF.W		R18, 0xFF, R22
	RGB5UPCK64	R19, R19		|	NOT			R22, R23
	PMULU.HW	R18, R22, R12	|	PMULU.HW	R19, R23, R13
	ADD			R27, R26		|	PADD.W		R12, R13, R18
									RGB5PCK64	R18, R18
									MOV.W		R18, (R5)
	ADD			2, R5
	CMPQGT		R5, R21
	BT			.L1

	.L2:
	MOV.X	(SP,   0), R8
	MOV.X	(SP,  16), R10
	MOV.X	(SP,  32), R12
	MOV.Q	(SP,  48), R14
	MOV.X	(SP,  64), R24
	MOV.X	(SP,  80), R26
	MOV.X	(SP,  96), R28
	MOV.X	(SP, 112), R30
	ADD		128, SP
	RTSU

TKRA_DrawSpan_AtestModBlUtx2Mort:
	ADD		-128, SP
	MOV.X	R30, (SP, 112)
	MOV.X	R28, (SP,  96)
	MOV.X	R26, (SP,  80)
	MOV.X	R24, (SP,  64)
	MOV.Q	R14, (SP,  48)
	MOV.X	R12, (SP,  32)
	MOV.X	R10, (SP,  16)
	MOV.X	R8 , (SP,   0)

	MOV.Q	(R4, TKRA_DS_TEXBCN*8), R20
	MOV.X	(R4, TKRA_DS_ZPOS *8), R24
	MOV.X	(R4, TKRA_DS_CPOS *8), R26
	MOV.X	(R4, TKRA_DS_TPOS *8), R28
	MOV.Q	(R4, TKRA_DS_YMASK*8), R31
	LEA.W	(R5, R7), R21

	.L1:
	MOV			0x00000000FFFF, R8
	MOV			0xFFFF00000000, R9
	ADD			R28, R8, R17	|	PMORT.L		R28, R18
	AND			R18, R31, R2	|	PMORT.L		R17, R19
	AND			R19, R31, R3	|	ADD			R28, R9, R16
	SHAD		R2, -4, R18		|	SHAD		R3, -4, R19
	ADD			R16, R8, R17	|	MOV.Q		(R20, R18), R18
	PMORT.L		R16, R22		|	MOV.Q		(R20, R19), R19
	AND			R22, R31, R22	|	BLKUTX2		R18, R2, R8
	PMORT.L		R17, R23		|	BLKUTX2		R19, R3, R9
	AND			R23, R31, R23	|	MOV			R28, R16
	SHAD		R22, -4, R10	|	SHAD		R23, -4, R11
									MOV.Q		(R20, R10), R10
									MOV.Q		(R20, R11), R11
									BLKUTX2		R10, R22, R10
									BLKUTX2		R11, R23, R11
	ADD			R29, R28		|	PSHUF.W		R16, 0, R22
	NOT			R22, R23		|	PSHUF.W		R16, 0xAA, R18
	NOT			R18, R19		|
	PMULU.HW	R8, R23, R12	|	PMULU.HW	R9, R22, R13
	PMULU.HW	R10, R23, R14	|	PMULU.HW	R11, R22, R30
	PADD.W		R12, R13, R8	|	PADD.W		R14, R30, R9
	PMULU.HW	R8, R19, R12	|	PMULU.HW	R9, R18, R13
									PADD.W		R12, R13, R18
									PMULU.HW	R18, R26, R18
									SHLD.Q		R18, -63, R3
	ADD			R27, R26		|	RGB5PCK64	R18, R18
									TST			1, R3
									MOV.W?F		R18, (R5)
	ADD			2, R5
	CMPQGT		R5, R21
	BT			.L1

	.L2:
	MOV.X	(SP,   0), R8
	MOV.X	(SP,  16), R10
	MOV.X	(SP,  32), R12
	MOV.Q	(SP,  48), R14
	MOV.X	(SP,  64), R24
	MOV.X	(SP,  80), R26
	MOV.X	(SP,  96), R28
	MOV.X	(SP, 112), R30
	ADD		128, SP
	RTSU
#endif

};

#endif
