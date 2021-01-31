void TKRA_DrawSpan_ModTexZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_ModTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_AlphaModTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);

void TKRA_DrawSpan_ModUtx2MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_ModUtx2MortZb(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);

void TKRA_DrawSpan_AlphaModUtx2MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_AlphaModUtx2MortZb(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);

void TKRA_DrawSpan_Zbuf(tkra_zbufpixel *dstz, int cnt, u64 zpos, u64 zstep);
void TKRA_DrawSpan_ZbNul(tkra_zbufpixel *dstz, int cnt, u64 zpos, u64 zstep);

void TKRA_DrawSpan_ModTexHZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_ModTexMortHZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);

void TKRA_DrawSpan_AlphaModTexHZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_AlphaModTexMortHZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);

void TKRA_DrawSpan_ModBlTexMortHZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);
void TKRA_DrawSpan_AlphaModBlTexMortHZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt);


#ifdef __BJX2__
// #if 0

__asm {
TKRA_DrawSpan_ModTexZt:
	ADD		-64, SP
	MOV.X	R30, (SP, 48)
	MOV.X	R28, (SP, 32)
	MOV.X	R26, (SP, 16)
	MOV.X	R24, (SP,  0)

	MOV.Q	(R4, TKRA_DS_TEXIMG*8), R20

	MOV.Q	(R4, TKRA_DS_ZPOS *8), R24
	MOV.Q	(R4, TKRA_DS_ZSTEP*8), R25
	MOV.Q	(R4, TKRA_DS_CPOS *8), R26
	MOV.Q	(R4, TKRA_DS_CSTEP*8), R27
	MOV.Q	(R4, TKRA_DS_TPOS *8), R28
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
	SHAD		R24, -16, R17	| MOVU.W		(R6), R16
	ADD			R25, R24		| CMPHI			R17, R16
	MOV.W?T		R2, (R5)
//	MOV.W?T		R17, (R6)
	ADD			2, R5			| ADD			2, R6
	CMPGT		R5, R21
	BT			.L0

	MOV.X	(SP,  0), R24
	MOV.X	(SP, 16), R26
	MOV.X	(SP, 32), R28
	MOV.X	(SP, 48), R30
	ADD		64, SP
	RTSU

TKRA_DrawSpan_ModTexMortZt:
	ADD		-64, SP
	MOV.X	R30, (SP, 48)
	MOV.X	R28, (SP, 32)
	MOV.X	R26, (SP, 16)
	MOV.X	R24, (SP,  0)

	MOV.Q	(R4, TKRA_DS_TEXIMG*8), R20

	MOV.Q	(R4, TKRA_DS_ZPOS *8), R24
	MOV.Q	(R4, TKRA_DS_ZSTEP*8), R25
	MOV.Q	(R4, TKRA_DS_CPOS *8), R26
	MOV.Q	(R4, TKRA_DS_CSTEP*8), R27
	MOV.Q	(R4, TKRA_DS_TPOS *8), R28
	MOV.Q	(R4, TKRA_DS_TSTEP*8), R29
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
	ADD			R27, R26		| RGB5PCK64		R2, R2
	SHAD		R24, -16, R17	| MOVU.W		(R6), R16
	ADD			R25, R24

//	CMPHI			R17, R16
//	MOV.W?T		R2, (R5)
//	MOV.W?T		R17, (R6)

	CMPHI		R16, R17
	MOV.W?F		R2, (R5)
//	MOV.W?F		R17, (R6)

	ADD			2, R5			| ADD			2, R6
	CMPGT		R5, R21
	BT			.L0

	MOV.X	(SP,  0), R24
	MOV.X	(SP, 16), R26
	MOV.X	(SP, 32), R28
	MOV.X	(SP, 48), R30
	ADD		64, SP
	RTSU


TKRA_DrawSpan_ModTexMortZb:
	ADD		-64, SP
	MOV.X	R30, (SP, 48)
	MOV.X	R28, (SP, 32)
	MOV.X	R26, (SP, 16)
	MOV.X	R24, (SP,  0)

	MOV.Q	(R4, TKRA_DS_TEXIMG*8), R20

	MOV.Q	(R4, TKRA_DS_ZPOS *8), R24
	MOV.Q	(R4, TKRA_DS_ZSTEP*8), R25
	MOV.Q	(R4, TKRA_DS_CPOS *8), R26
	MOV.Q	(R4, TKRA_DS_CSTEP*8), R27
	MOV.Q	(R4, TKRA_DS_TPOS *8), R28
	MOV.Q	(R4, TKRA_DS_TSTEP*8), R29
//	MOV.Q	(R4, TKRA_DS_XMASK*8), R30
	MOV.Q	(R4, TKRA_DS_YMASK*8), R31
	LEA.W	(R5, R7), R21

	.L0:	
	PMORT.L		R28, R2
	ADD			R29, R28		|	AND			R2, R31, R3
	MOV.W		(R20, R3), R2
	RGB5UPCK64	R2, R2
	PMULU.HW	R2, R26, R2
	ADD			R27, R26		| RGB5PCK64		R2, R2
	SHAD		R24, -16, R17	| MOVU.W		(R6), R16
	ADD			R25, R24

	CMPHI		R16, R17
	MOV.W?F		R2, (R5)
	MOV.W?F		R17, (R6)

	ADD			2, R5			| ADD			2, R6
	CMPGT		R5, R21
	BT			.L0

	MOV.X	(SP,  0), R24
	MOV.X	(SP, 16), R26
	MOV.X	(SP, 32), R28
	MOV.X	(SP, 48), R30
	ADD		64, SP
	RTSU

TKRA_DrawSpan_AlphaModTexMortZt:
	ADD		-64, SP
	MOV.X	R30, (SP, 48)
	MOV.X	R28, (SP, 32)
	MOV.X	R26, (SP, 16)
	MOV.X	R24, (SP,  0)
	
	MOV.Q	(R4, TKRA_DS_TEXIMG*8), R20

	MOV.Q	(R4, TKRA_DS_ZPOS *8), R24
	MOV.Q	(R4, TKRA_DS_ZSTEP*8), R25
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
	SHAD.Q		R24, -16, R17	|	PADD.W		R2, R3, R18
	ADD			R27, R26		|	RGB5PCK64	R18, R18
	ADD			R25, R24		|	MOVU.W		(R6), R16
									CMPHI		R16, R17
									MOV.W?F		R18, (R5)
	ADD			2, R5			|	ADD			2, R6
	CMPGT		R5, R21
	BT			.L1

	.L2:

	MOV.X	(SP,  0), R24
	MOV.X	(SP, 16), R26
	MOV.X	(SP, 32), R28
	MOV.X	(SP, 48), R30
	ADD		64, SP
	RTSU



TKRA_DrawSpan_ModUtx2MortZt:
	ADD		-64, SP
	MOV.X	R30, (SP, 48)
	MOV.X	R28, (SP, 32)
	MOV.X	R26, (SP, 16)
	MOV.X	R24, (SP,  0)

	MOV.Q	(R4, TKRA_DS_TEXBCN*8), R20

	MOV.X	(R4, TKRA_DS_ZPOS *8), R24
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

	.L0:	
									PMORT.L		R28, R2
	ADD			R29, R28		|	AND			R2, R31, R3
	SHAD		R3, -4, R2		|	MOVU.W		(R6), R16
	SHAD		R24, -16, R17	|	MOV.Q		(R20, R2), R2
									BLKUTX2		R2, R3, R2
									PMULU.HW	R2, R26, R2
	ADD			R27, R26		|	RGB5PCK64	R2, R2
	ADD			R25, R24		|	CMPHI		R16, R17
									MOV.W?F		R2, (R5)
	ADD			2, R5			|	ADD			2, R6

	CMPGT		R5, R21
	BT			.L0

	MOV.X	(SP,  0), R24
	MOV.X	(SP, 16), R26
	MOV.X	(SP, 32), R28
	MOV.X	(SP, 48), R30
	ADD		64, SP
	RTSU

TKRA_DrawSpan_ModUtx2MortZb:
	ADD		-64, SP
	MOV.X	R30, (SP, 48)
	MOV.X	R28, (SP, 32)
	MOV.X	R26, (SP, 16)
	MOV.X	R24, (SP,  0)

	MOV.Q	(R4, TKRA_DS_TEXBCN*8), R20

	MOV.X	(R4, TKRA_DS_ZPOS *8), R24
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

	.L0:	
									PMORT.L		R28, R2
	ADD			R29, R28		|	AND			R2, R31, R3
	SHAD		R3, -4, R2		|	MOVU.W		(R6), R16
	SHAD		R24, -16, R17	|	MOV.Q		(R20, R2), R2
									BLKUTX2		R2, R3, R2
									PMULU.HW	R2, R26, R2
	ADD			R27, R26		|	RGB5PCK64	R2, R2
	ADD			R25, R24		|	CMPHI		R16, R17
									MOV.W?F		R2, (R5)
									MOV.W?F		R17, (R6)
	ADD			2, R5			|	ADD			2, R6

	CMPGT		R5, R21
	BT			.L0

	MOV.X	(SP,  0), R24
	MOV.X	(SP, 16), R26
	MOV.X	(SP, 32), R28
	MOV.X	(SP, 48), R30
	ADD		64, SP
	RTSU

TKRA_DrawSpan_AlphaModUtx2MortZt:
	ADD		-64, SP
	MOV.X	R30, (SP, 48)
	MOV.X	R28, (SP, 32)
	MOV.X	R26, (SP, 16)
	MOV.X	R24, (SP,  0)
	
	MOV.Q	(R4, TKRA_DS_TEXBCN*8), R20

	MOV.X	(R4, TKRA_DS_ZPOS *8), R24
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

	.L1:
	PMORT.L		R28, R18		|	MOVU.W		(R5), R19
	ADD			R29, R28		|	AND			R18, R31, R3
									SHAD		R3, -4, R18
									MOV.Q		(R20, R18), R18
									BLKUTX2		R18, R3, R18
	RGB5UPCK64	R19, R19		|	PMULU.HW	R18, R26, R18
									PSHUF.W		R18, 0xFF, R22
									NOT			R22, R23
	PMULU.HW	R18, R22, R2	|	PMULU.HW	R19, R23, R3
	SHAD.Q		R24, -16, R17	|	PADD.W		R2, R3, R18
	ADD			R27, R26		|	RGB5PCK64	R18, R18
	ADD			R25, R24		|	MOVU.W		(R6), R16
									CMPHI		R16, R17
									MOV.W?F		R18, (R5)
	ADD			2, R5			|	ADD			2, R6
	CMPGT		R5, R21
	BT			.L1

	.L2:

	MOV.X	(SP,  0), R24
	MOV.X	(SP, 16), R26
	MOV.X	(SP, 32), R28
	MOV.X	(SP, 48), R30
	ADD		64, SP
	RTSU

TKRA_DrawSpan_AlphaModUtx2MortZb:
	ADD		-64, SP
	MOV.X	R30, (SP, 48)
	MOV.X	R28, (SP, 32)
	MOV.X	R26, (SP, 16)
	MOV.X	R24, (SP,  0)
	
	MOV.Q	(R4, TKRA_DS_TEXBCN*8), R20

	MOV.X	(R4, TKRA_DS_ZPOS *8), R24
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

	.L1:
//	CMPQGT		R5, R21
//	BF			.L2

	PMORT.L		R28, R18		|	MOVU.W		(R5), R19
	ADD			R29, R28		|	AND			R18, R31, R3
									SHAD		R3, -4, R18
									MOV.Q		(R20, R18), R18
									BLKUTX2		R18, R3, R18
									RGB5UPCK64	R19, R19
									PMULU.HW	R18, R26, R18
									PSHUF.W		R18, 0xFF, R22
									NOT			R22, R23
	PMULU.HW	R18, R22, R2	|	PMULU.HW	R19, R23, R3
	SHAD.Q		R24, -16, R17	|	PADD.W		R2, R3, R18
	ADD			R27, R26		|	RGB5PCK64	R18, R18
	ADD			R25, R24		|	MOVU.W		(R6), R16
									CMPHI		R16, R17
									MOV.W?F		R18, (R5)
									MOV.W?F		R17, (R6)
	ADD			2, R5			|	ADD			2, R6
	CMPGT		R5, R21
	BT			.L1

	.L2:

	MOV.X	(SP,  0), R24
	MOV.X	(SP, 16), R26
	MOV.X	(SP, 32), R28
	MOV.X	(SP, 48), R30
	ADD		64, SP
	RTSU



TKRA_DrawSpan_ModTexMortHZt:
	ADD		-64, SP
	MOV.X	R30, (SP, 48)
	MOV.X	R28, (SP, 32)
	MOV.X	R26, (SP, 16)
	MOV.X	R24, (SP,  0)
	
	MOV.Q	(R4, TKRA_DS_TEXIMG*8), R20

	MOV.Q	(R4, TKRA_DS_ZPOS *8), R24
	MOV.Q	(R4, TKRA_DS_ZSTEP*8), R25
	MOV.Q	(R4, TKRA_DS_CPOS *8), R26
	MOV.Q	(R4, TKRA_DS_CSTEP*8), R27
	MOV.Q	(R4, TKRA_DS_TPOS *8), R28
	MOV.Q	(R4, TKRA_DS_TSTEP*8), R29
//	MOV.Q	(R4, TKRA_DS_XMASK*8), R30
	MOV.Q	(R4, TKRA_DS_YMASK*8), R31

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

	ADD		R5, 4, R22	|	LEA.W	(R5, R7), R21
	ADD		R25, R25	|	CMPGT		R22, R21
	BF			.L1

	.L0:
	ADD			R28, R29, R16	|	PMORT.L		R28, R18
	ADD			R16, R29, R28	|	PMORT.L		R16, R19
	AND			R18, R31, R2	|	AND			R19, R31, R3
	SHAD.Q		R24, -16, R17	|	MOV.W		(R20, R2), R18
	ADD			R25, R24		|	MOV.W		(R20, R3), R19
	RGB5UPCK64	R18, R18		|	RGB5UPCK64	R19, R19
	ADD			R26, R27, R16	|	PMULU.HW	R18, R26, R18
	ADD			R16, R27, R26	|	PMULU.HW	R19, R16, R19
	RGB5PCK64	R18, R18		|	RGB5PCK64	R19, R19
	ADD			R5, 8, R22		|	MOVU.W		(R6), R16
	ADD			2, R6			|	CMPHI		R16, R17
									MOV.W?F		R18, (R5, 0)
									MOV.W?F		R19, (R5, 2)
	ADD			R5, 4, R5		|	CMPQGT		R22, R21
	BT			.L0
//	BRA			.L1B

	.L1:
	CMPQGT		R5, R21
	BF			.L2

	PMORT.L		R28, R18
	ADD			R29, R28		|	AND			R18, R31, R3
	MOV.W		(R20, R3), R18
	RGB5UPCK64	R18, R18
	PMULU.HW	R18, R26, R18
	ADD			R27, R26		| RGB5PCK64		R18, R18
	SHAD.Q		R24, -16, R17	| MOVU.W		(R6), R16
	ADD			R25, R24
	CMPHI		R16, R17
	MOV.W?F		R18, (R5)

	ADD			2, R5
	TEST		2, R5
	ADD?T		2, R6

	CMPGT		R5, R21
	BT			.L1

	.L2:

	MOV.X	(SP,  0), R24
	MOV.X	(SP, 16), R26
	MOV.X	(SP, 32), R28
	MOV.X	(SP, 48), R30
	ADD		64, SP
	RTSU


TKRA_DrawSpan_AlphaModTexMortHZt:
	ADD		-64, SP
	MOV.X	R30, (SP, 48)
	MOV.X	R28, (SP, 32)
	MOV.X	R26, (SP, 16)
	MOV.X	R24, (SP,  0)
	
	MOV.Q	(R4, TKRA_DS_TEXIMG*8), R20

	MOV.Q	(R4, TKRA_DS_ZPOS *8), R24
	MOV.Q	(R4, TKRA_DS_ZSTEP*8), R25
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
	SHAD.Q		R24, -16, R17	|	PADD.W		R2, R3, R18
	ADD			R27, R26		|	RGB5PCK64	R18, R18
	ADD			R25, R24		|	MOVU.W		(R6), R16
									CMPHI		R16, R17
									MOV.W?F		R18, (R5)

	ADD			2, R5
	TEST		2, R5
	ADD?T		2, R6

	CMPGT		R5, R21
	BT			.L1

	.L2:

	MOV.X	(SP,  0), R24
	MOV.X	(SP, 16), R26
	MOV.X	(SP, 32), R28
	MOV.X	(SP, 48), R30
	ADD		64, SP
	RTSU

TKRA_DrawSpan_ZbNul:
	RTS

TKRA_DrawSpan_Zbuf:
	LEA.W	(R4, R5), R21
	.L0:	
	SHAD		R6, -16, R17	| MOVU.W	(R4), R16
	ADD			R7, R6			| CMPHI		R16, R17
	MOV.W?F		R17, (R4)
	ADD			2, R4
	CMPGT		R4, R21
	BT			.L0
	RTSU


TKRA_DrawSpan_ModBlTexMortHZt:
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

	MOV.Q	(R4, TKRA_DS_ZPOS *8), R24
	MOV.Q	(R4, TKRA_DS_ZSTEP*8), R25
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
	SHAD.Q		R24, -16, R17	|	MOVU.W		(R6), R16
	ADD			R25, R24		|	CMPHI		R16, R17
									MOV.W?F		R18, (R5)
	ADD			2, R5
	TEST		2, R5
	ADD?T		2, R6

	CMPGT		R5, R21
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

TKRA_DrawSpan_AlphaModBlTexMortHZt:
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

	MOV.Q	(R4, TKRA_DS_ZPOS *8), R24
	MOV.Q	(R4, TKRA_DS_ZSTEP*8), R25
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
//									PSHUF.W		R26, 0xFF, R22
									PSHUF.W		R18, 0xFF, R22
	RGB5UPCK64	R19, R19		|	NOT			R22, R23
	PMULU.HW	R18, R22, R12	|	PMULU.HW	R19, R23, R13
	ADD			R27, R26		|	PADD.W		R12, R13, R18
									RGB5PCK64	R18, R18
	SHAD.Q		R24, -16, R17	|	MOVU.W		(R6), R16
	ADD			R25, R24		|	CMPHI		R16, R17
									MOV.W?F		R18, (R5)
	ADD			2, R5
	TEST		2, R5
	ADD?T		2, R6
	CMPGT		R5, R21
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

};
#else

void TKRA_DrawSpan_ModTexZbuf(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	pos, step;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval;
	u32 xmask, ymask;
	u32 z;
	int pix, clr, idx;

	pos=parm[TKRA_DS_TPOS];
	step=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXIMG]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
		pix=src[idx];

		cval=tkra_rgbupck64(pix);
		cval=tkra_pmuluhw(cval, cpos);
		pix=tkra_rgbpck64(cval);

		z=zpos>>16;
		if(z<=(*ctz))
		{
			*ct=pix;
//			*ctz=z;
		}
		ct++;
		ctz++;
		pos+=step;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpan_ModTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval;
	u32 xmask, ymask;
	u32 z;
	int pix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXIMG]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
//		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
//		idx=tkra_morton8(((tpos>>16)&xmask), ((tpos>>48)&xmask));
//		idx=tkra_morton8(tpos>>16, tpos>>48)&ymask;
		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;
		pix=src[idx];

		cval=tkra_rgbupck64(pix);
		cval=tkra_pmuluhw(cval, cpos);
		pix=tkra_rgbpck64(cval);

		z=zpos>>16;
		if(z<=(*ctz))
		{
			*ct=pix;
//			*ctz=z;
		}
		ct++;
		ctz++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpan_AlphaModTexMortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, amod, anmod;
	u32 xmask, ymask;
	u32 z;
	int pix, dpix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXIMG]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
//		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
//		idx=tkra_morton8(((tpos>>16)&xmask), ((tpos>>48)&xmask));
//		idx=tkra_morton8(tpos>>16, tpos>>48)&ymask;
		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;
		pix=src[idx];
		dpix=*ct;

		cval=tkra_rgbupck64(pix);
		dval=tkra_rgbupck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
		
		amod=(cval>>48)&65535;
		amod|=amod<<16;
		amod|=amod<<32;
		anmod=~amod;

		cval=tkra_pmuluhw(cval, amod);
		dval=tkra_pmuluhw(dval, anmod);
		cval+=dval;
		
		pix=tkra_rgbpck64(cval);

		z=zpos>>16;
		if(z<=(*ctz))
			{ *ct=pix; }
		ctz++;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

#if 1
void TKRA_DrawSpan_ModUtx2MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	u64 *src;
	tkra_rastpixel *ct, *cte;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, blk;
	u32 xmask, ymask;
	u32 z;
	int pix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(u64 *)(parm[TKRA_DS_TEXBCN]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
//		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
//		idx=tkra_morton8(((tpos>>16)&xmask), ((tpos>>48)&xmask));
//		idx=tkra_morton8(tpos>>16, tpos>>48)&ymask;
		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;
//		pix=src[idx];
//		blk=src[idx>>4];

//		cval=tkra_rgbupck64(pix);
//		cval=tkra_blkutx2(blk, idx&15);
		cval=TKRA_CachedBlkUtx2(src, idx);
		cval=tkra_pmuluhw(cval, cpos);
		pix=tkra_rgbpck64(cval);

		z=zpos>>16;
		if(z<=(*ctz))
		{
			*ct=pix;
//			*ctz=z;
		}
		ct++;
		ctz++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpan_AlphaModUtx2MortZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	u64 *src;
	tkra_rastpixel *ct, *cte;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, amod, anmod;
	u64 blk;
	u32 xmask, ymask;
	u32 z;
	int pix, dpix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(u64 *)(parm[TKRA_DS_TEXBCN]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
//		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
//		idx=tkra_morton8(((tpos>>16)&xmask), ((tpos>>48)&xmask));
//		idx=tkra_morton8(tpos>>16, tpos>>48)&ymask;
		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;
//		pix=src[idx];
//		blk=src[idx>>4];
		dpix=*ct;

//		cval=tkra_rgbupck64(pix);
//		cval=tkra_blkutx2(blk, idx&15);
		cval=TKRA_CachedBlkUtx2(src, idx);
		dval=tkra_rgbupck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
		
		amod=(cval>>48)&65535;
		amod|=amod<<16;
		amod|=amod<<32;
		anmod=~amod;

		cval=tkra_pmuluhw(cval, amod);
		dval=tkra_pmuluhw(dval, anmod);
		cval+=dval;
		
		pix=tkra_rgbpck64(cval);

		z=zpos>>16;
		if(z<=(*ctz))
			{ *ct=pix; }
		ctz++;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}
#endif

void TKRA_DrawSpan_ZbNul(
	tkra_zbufpixel *dstz, int cnt,
	u64 izpos, u64 izstep)
{
}

void TKRA_DrawSpan_Zbuf(
	tkra_zbufpixel *dstz, int cnt,
	u64 izpos, u64 izstep)
{
	tkra_zbufpixel *ct, *cte;
	u64	zpos, zstep;
	int pix, idx;

	zpos=izpos; zstep=izstep;
	ct=dstz; cte=ct+cnt;
	while(ct<cte)
	{
		pix=zpos>>16;
		if(pix<(*ct))
			*ct=pix;
		ct++;
		zpos+=zstep;
	}
}

void TKRA_DrawSpan_ModUtx2MortZb(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_Zbuf(dstz, cnt,
		parm[TKRA_DS_ZPOS], parm[TKRA_DS_ZSTEP]);
	TKRA_DrawSpan_ModUtx2MortZt(parm, dstc, dstz, cnt);
}

void TKRA_DrawSpan_AlphaModUtx2MortZb(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	TKRA_DrawSpan_Zbuf(dstz, cnt,
		parm[TKRA_DS_ZPOS], parm[TKRA_DS_ZSTEP]);
	TKRA_DrawSpan_AlphaModUtx2MortZt(parm, dstc, dstz, cnt);
}

void TKRA_DrawSpan_ModTexMortHZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval;
	u32 xmask, ymask;
	u32 z;
	int pix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXIMG]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
//		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
//		idx=tkra_morton8(((tpos>>16)&xmask), ((tpos>>48)&xmask));
//		idx=tkra_morton8(tpos>>16, tpos>>48)&ymask;
		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;
		pix=src[idx];

		cval=tkra_rgbupck64(pix);
		cval=tkra_pmuluhw(cval, cpos);
		pix=tkra_rgbpck64(cval);

		z=zpos>>16;
		if(z<=(*ctz))
			{ *ct=pix; }
		if((ct-dstc)&1)
			ctz++;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpan_AlphaModTexMortHZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, amod, anmod;
	u32 xmask, ymask;
	u32 z;
	int pix, dpix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXIMG]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
//		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
//		idx=tkra_morton8(((tpos>>16)&xmask), ((tpos>>48)&xmask));
//		idx=tkra_morton8(tpos>>16, tpos>>48)&ymask;
		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;
		pix=src[idx];
		dpix=*ct;

		cval=tkra_rgbupck64(pix);
		dval=tkra_rgbupck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
		
		amod=(cval>>48)&65535;
		amod|=amod<<16;
		amod|=amod<<32;
		anmod=~amod;

		cval=tkra_pmuluhw(cval, amod);
		dval=tkra_pmuluhw(dval, anmod);
		cval+=dval;
		
		pix=tkra_rgbpck64(cval);

		z=zpos>>16;
		if(z<=(*ctz))
			{ *ct=pix; }
		if((ct-dstc)&1)
			ctz++;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

u64 TKRA_InterpBilinear(
	u16 px0, u16 px1, u16 px2, u16 px3,
	u16 xfrac, u16 yfrac)
{
	u64 cv0, cv1, cv2, cv3, cv4, cv5, cv6;
	u64 cxf, cyf, cxnf, cynf;

	cv0=tkra_rgbupck64(px0);
	cv1=tkra_rgbupck64(px1);
	cv2=tkra_rgbupck64(px2);
	cv3=tkra_rgbupck64(px3);

	cxf=xfrac;	cxf|=cxf<<16;	cxf|=cxf<<32;
	cyf=yfrac;	cyf|=cyf<<16;	cyf|=cyf<<32;
	cxnf=~cxf;
	cynf=~cyf;

	cv4=tkra_pmuluhw(cv0, cxnf)+tkra_pmuluhw(cv1, cxf);
	cv5=tkra_pmuluhw(cv2, cxnf)+tkra_pmuluhw(cv3, cxf);
	cv6=tkra_pmuluhw(cv4, cynf)+tkra_pmuluhw(cv5, cyf);
	return(cv6);
}

void TKRA_DrawSpan_ModBlTexMortHZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval;
	u32 xmask, ymask;
	u32 z;
	int pix0, pix1, pix2, pix3;
	int pix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXIMG]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
//		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
//		idx=tkra_morton8(((tpos>>16)&xmask), ((tpos>>48)&xmask));
//		idx=tkra_morton8(tpos>>16, tpos>>48)&ymask;
//		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;
//		pix=src[idx];

		pix0=src[tkra_morton16((tpos>>16)+0, (tpos>>48)+0)&ymask];
		pix1=src[tkra_morton16((tpos>>16)+1, (tpos>>48)+0)&ymask];
		pix2=src[tkra_morton16((tpos>>16)+0, (tpos>>48)+1)&ymask];
		pix3=src[tkra_morton16((tpos>>16)+1, (tpos>>48)+1)&ymask];
		cval=TKRA_InterpBilinear(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));

//		cval=tkra_rgbupck64(pix);
		cval=tkra_pmuluhw(cval, cpos);
		pix=tkra_rgbpck64(cval);

		z=zpos>>16;
		if(z<=(*ctz))
			{ *ct=pix; }
		if((ct-dstc)&1)
			ctz++;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

void TKRA_DrawSpan_AlphaModBlTexMortHZt(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, amod, anmod;
	u32 xmask, ymask;
	u32 z;
	int pix0, pix1, pix2, pix3;
	int pix, dpix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXIMG]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
//		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
//		idx=tkra_morton8(((tpos>>16)&xmask), ((tpos>>48)&xmask));
//		idx=tkra_morton8(tpos>>16, tpos>>48)&ymask;
//		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;
//		pix=src[idx];

		pix0=src[tkra_morton16((tpos>>16)+0, (tpos>>48)+0)&ymask];
		pix1=src[tkra_morton16((tpos>>16)+1, (tpos>>48)+0)&ymask];
		pix2=src[tkra_morton16((tpos>>16)+0, (tpos>>48)+1)&ymask];
		pix3=src[tkra_morton16((tpos>>16)+1, (tpos>>48)+1)&ymask];
		cval=TKRA_InterpBilinear(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));

		dpix=*ct;

//		cval=tkra_rgbupck64(pix);
		dval=tkra_rgbupck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
		
		amod=(cval>>48)&65535;
		amod|=amod<<16;
		amod|=amod<<32;
		anmod=~amod;

		cval=tkra_pmuluhw(cval, amod);
		dval=tkra_pmuluhw(dval, anmod);
		cval+=dval;
		
		pix=tkra_rgbpck64(cval);

		z=zpos>>16;
		if(z<=(*ctz))
			{ *ct=pix; }
		if((ct-dstc)&1)
			ctz++;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
		zpos+=zstep;
	}
}

#endif
