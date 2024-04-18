// #define X(n)	_##n
#define X(n)	n

.section	.data

// loopcount	.long	0
// pixelcount	.long	0


.section	.text

/*
EAX -> R4
ECX -> R5
EDX -> R6
EBX -> R7

EBP -> R20
ESP -> R21
ESI -> R22
EDI -> R23
*/

.global X(R_DrawColumn)
X(R_DrawColumn):

	WEXMD

	MOV.L		X(dc_yh), R4
	MOV.L		X(dc_yl), R20
	ADD			#1, R4	
	SUB			R4, R20, R16

	CMP/GE		#0, R16
	BF			.done

	MOV			X(ylookup), R18
	MOV.L		X(dc_x), R7
	MOV			X(columnofs), R6
	MOV.L		X(dc_iscale), R5
	MOV.L		X(centery), R21
	MOV.L		X(dc_texturemid), R22

	SHLD.Q		R16, #-1, R17		| MOV.P		(R18, R20), R23
	SUB			R20, R21			| MOV.L		(R6, R7), R19

	MOV.L		X(dc_iscale), R7
	SHLD		R7, #9, R7			| MUL.L		R21, R5, R4
	SUB			R22, R4, R22		| LEA.W		(R23, R19), R23

// R4		aligned colormap
// R7		aligned colormap
// R5,R6	scratch
// R22		virtual source
// R23		moving destination pointer
// R20		frac
// R16		pixelcount
// R17		loopcount

	MOV		R7, R21			| SHLD	R22, #9, R18
	ADD		R18, R7, R19	| SHLD	R18, #-25, R5
	ADD		R19, R7, R20	| SHLD	R19, #-25, R6

	MOV.P	X(dc_colormap), R4
	MOV.P	X(dc_source), R22

	MOVU.B	(R22, R5), R18		// get first pixel
	MOVU.B	(R22, R6), R19		// get second pixel
	TEST	0xFFFFFFFE, R16
	MOVU.W	(R4, R18), R18		// color translate first pixel
	MOVU.W	(R4, R19), R19		// color translate second pixel
	
	BT		.checklast
	
#if 0
.doubleloop:
	MOV		R20, R5				// begin calculating third pixel

	ADD		R21, R20			// advance frac pointer
	MOV.W	R18, (R23)			// write first pixel
	SHLD	R5, #-25, R5		// finish calculation for third pixel
	MOV		R20, R6				// begin calculating fourth pixel

	ADD		R21, R20			// advance frac pointer
	MOV.W	R19, (R23, 640)		// write second pixel
	SHLD	R6, #-25, R6		// finish calculation for fourth pixel
	MOVU.B	(R22, R5), R18		// get third pixel
	ADD		#1280, R23			// advance to third pixel destination
	MOVU.B	(R22, R6), R19		// get fourth pixel
	ADD		#-1, R17
	MOVU.W	(R4, R18), R18		// color translate first pixel
	MOVU.W	(R4, R19), R19		// color translate second pixel
	TEST	R17, R17
	BF		.doubleloop
#endif

#if 1
.doubleloop:
.ifarch bjx2_wex3
	SHLD	R20, #-25, R5	|	MOV.W	R18, (R23)
	ADD		R21, R20		|	MOV.W	R19, (R23, 640)
.else
	SHLD	R20, #-25, R5	| ADD		R21, R20
								MOV.W	R18, (R23)
								MOV.W	R19, (R23, 640)
.endif
	SHLD	R20, #-25, R6	| MOVU.B	(R22, R5), R18
	ADD		R21, R20		| MOVU.B	(R22, R6), R19
	ADD		#-1, R17		| MOVU.W	(R4, R18), R18
	ADD		#1280, R23		| MOVU.W	(R4, R19), R19

.ifarch has_jcmpz
	BRNE.L	R17, .doubleloop
.else
	TEST	R17, R17
	BF		.doubleloop
.endif
#endif

// check for final pixel
.checklast:
	TEST	#1, R16
	MOV.W?F	R18, (R23)			// write final pixel
//	BT		.done
//	MOV.W	R18, (R23)			// write final pixel

.done:
	RTS
	

#if 1

//================
//
// R_DrawSpan
//
// Horizontal texture mapping
//
//================


.global X(R_DrawSpan)
X(R_DrawSpan):

//	RTS

	WEXMD

	MOV.L		X(ds_x2), R4
	ADD			#1, R4
	MOV.L		X(ds_x1), R3
	SUB			R3, R4					// pixel count
	MOV			R4, R16
	CMP/GE		#0, R4
	BF			.hdone

	SHLD		R4, #-1, R4						// double pixel count
	MOV			R4, R17

	MOV.L	X(ds_xfrac), R20
	SHLD	R20, #10, R20
	AND		#0xffff0000, R20
//	BREAK
	MOV.L	X(ds_yfrac), R4
	SHLD	R4, #-6, R4
	EXTU.W	R4
	OR		R4, R20

	MOV		X(ylookup), R3
	MOV.L	X(ds_y), R23
	MOV.P	X(ds_source), R22
	MOV.P	(R3, R23), R23
	MOV.L	X(ds_x1), R4
	MOV		X(columnofs), R3
	MOV.L	(R3, R4), R2
	LEA.W	(R23, R2), R23

	MOV.L	X(ds_xstep), R7
	SHLD	R7, #10, R7
	AND		#0xFFFF0000, R7
	MOV.L	X(ds_ystep), R4
	SHLD	R4, #-6, R4
	EXTU.W	R4
	OR		R4, R7

	MOV		R7, R21
	
// R4		aligned colormap
// R7		aligned colormap
// R5,R6	scratch
// R22		virtual source
// R23		moving destination pointer
// R20		frac
// R16		pixelcount
// R17		loopcount

	SHLD	R5, #22, R5		| SHLD	R20, #-10, R3
	OR		R3, R5
	SHLD	R5, #6, R5		| SHLD	R20, #-26, R3
	OR		R3, R5			| ADD	R7, R20
	AND		#4095, R5

	SHLD	R6, #22, R6		| SHLD	R20, #-10, R3
	OR		R3, R6
	SHLD	R6, #6, R6		| SHLD	R20, #-26, R3
	OR		R3, R6			| ADD R7, R20
	AND		#4095, R6

//	MOV.L	X(ds_colormap), R4
	MOV.P	X(ds_colormap), R4

	MOV		R4, R7

	MOVU.B	(R22, R5), R18			// get first pixel
	MOVU.B	(R22, R6), R19			// get second pixel
	TEST	0xFFFFFFFE, R16
	MOVU.W	(R4, R18), R18			// color translate first pixel
	MOVU.W	(R7, R19), R19			// color translate second pixel

	BT		.hchecklast

#if 0
.hdoubleloop:
//	shldl R5,R20,22				// begin calculating third pixel (y units)
//	shldl R5,R20,6				// begin calculating third pixel (x units)
	SHLD	R5, #22, R5
	SHLD	R20, #-10, R3
	OR		R3, R5
	SHLD	R5, #6, R5
	SHLD	R20, #-26, R3
	OR		R3, R5

	ADD		R21, R20			// advance frac pointer
	MOV.W	R18, (R23)			// write first pixel
	AND		#4095, R5			// finish calculation for third pixel

//	shldl R6,R20,22				// begin calculating fourth pixel (y units)
//	shldl R6,R20,6				// begin calculating fourth pixel (x units)
	SHLD	R6, #22, R6
	SHLD	R20, #-10, R3
	OR		R3, R6
	SHLD	R6, #6, R6
	SHLD	R20, #-26, R3
	OR		R3, R6

	ADD		R21, R20			// advance frac pointer
	MOV.W	R19, (R23, 2)		// write second pixel
	AND		#4095, R6			// finish calculation for fourth pixel
	MOVU.B	(R22, R5), R18		// get third pixel
	ADD		#4, R23				// advance to third pixel destination
	MOVU.B	(R22, R6), R19		// get fourth pixel
	MOVU.W	(R4, R18), R18		// color translate third pixel
	MOVU.W	(R7, R19), R19		// color translate fourth pixel
//	decl	[loopcount]			// done with loop?
	ADD		#-1, R17
//	jnz	hdoubleloop
	TEST	R17, R17
	BF		.hdoubleloop
#endif

#if 1
.hdoubleloop:
	SHLD	R20, #-10, R2		|	SHLD	R5, #22, R5
	SHLD	R20, #-26, R3		|	SHLD	R6, #22, R6
	ADD		R21, R20
	SHLD	R20, #-10, R0		|	SHLD	R20, #-26, R1
	OR		R2, R5				|	OR		R0, R6
	SHLD	R5, #6, R5			|	SHLD	R6, #6, R6
	OR		R3, R5				|	OR		R1, R6
.ifarch bjx2_wex3
	MOV		#4095, R3			|	MOV.W	R18, (R23)
	AND		R3, R5				|	MOV.W	R19, (R23, 2)	
.else
	MOV		#4095, R3
	MOV.W	R18, (R23)
	AND		R3, R5
	MOV.W	R19, (R23, 2)	
.endif
	AND		R3, R6				|	MOVU.B	(R22, R5), R18	
	ADD		R21, R20			|	MOVU.B	(R22, R6), R19	

	ADD		#-1, R17			|	MOVU.W	(R4, R18), R18
	ADD		#4, R23				|	MOVU.W	(R7, R19), R19
.ifarch has_jcmpz
	BRNE.L	R17, .hdoubleloop
.else
	TEST	R17, R17
	BF		.hdoubleloop
.endif
#endif

.hchecklast:
	TEST	#1, R16
	MOV.W?F		R18, (R23)				// write final pixel
	
.hdone:
	RTS

#endif


#if 1

.global X(R_DrawColumnLow)
X(R_DrawColumnLow):

	WEXMD

	MOV.L		X(dc_yh), R4
	MOV.L		X(dc_yl), R20
	ADD			#1, R4	
	SUB			R4, R20, R16

	CMP/GE		#0, R16
	BF			.done

	MOV			X(ylookup), R18
	MOV.L		X(dc_x), R7
	MOV			X(columnofs), R6
	MOV.L		X(dc_iscale), R5
	MOV.L		X(centery), R21
	MOV.L		X(dc_texturemid), R22
	
	ADD			R7, R7

	SHLD.Q		R16, #-1, R17		| MOV.P		(R18, R20), R23
	SUB			R20, R21			| MOV.L		(R6, R7), R19

	MOV.L		X(dc_iscale), R7
	SHLD		R7, #9, R7			| MUL.L		R21, R5, R4
	SUB			R22, R4, R22		| LEA.W		(R23, R19), R23

// R4		aligned colormap
// R7		aligned colormap
// R5,R6	scratch
// R22		virtual source
// R23		moving destination pointer
// R20		frac
// R16		pixelcount
// R17		loopcount

	MOV		R7, R21			| SHLD	R22, #9, R18
	ADD		R18, R7, R19	| SHLD	R18, #-25, R5
	ADD		R19, R7, R20	| SHLD	R19, #-25, R6

	MOV.P	X(dc_colormap), R4
	MOV.P	X(dc_source), R22

	MOVU.B	(R22, R5), R18		// get first pixel
	MOVU.B	(R22, R6), R19		// get second pixel
	MOVU.W	(R4, R18), R18		// color translate first pixel
	MOVU.W	(R4, R19), R19		// color translate second pixel
	
	TEST	0xFFFFFFFE, R16
	BT		.checklast
	
#if 0
.doubleloop:
	MOV		R20, R5				// begin calculating third pixel

	ADD		R21, R20			// advance frac pointer
	MOV.W	R18, (R23)			// write first pixel
	SHLD	R5, #-25, R5		// finish calculation for third pixel
	MOV		R20, R6				// begin calculating fourth pixel

	ADD		R21, R20			// advance frac pointer
	MOV.W	R19, (R23, 640)		// write second pixel
	SHLD	R6, #-25, R6		// finish calculation for fourth pixel
	MOVU.B	(R22, R5), R18		// get third pixel
	ADD		#1280, R23			// advance to third pixel destination
	MOVU.B	(R22, R6), R19		// get fourth pixel
	MOVU.W	(R4, R18), R18		// color translate first pixel
	MOVU.W	(R4, R19), R19		// color translate second pixel
	ADD		#-1, R17
	TEST	R17, R17
	BF		.doubleloop
#endif

#if 1
.doubleloop:
	SHLD	R20, #-25, R5
	MOV.W	R18, (R23)
	MOV.W	R18, (R23, 2)
	ADD		R21, R20		
	MOV.W	R19, (R23, 640)
	MOV.W	R19, (R23, 642)
	SHLD	R20, #-25, R6	| MOVU.B	(R22, R5), R18
	ADD		R21, R20		| MOVU.B	(R22, R6), R19
	ADD		#1280, R23		| MOVU.W	(R4, R18), R18
	ADD		#-1, R17		| MOVU.W	(R4, R19), R19
	TEST	R17, R17
	BF		.doubleloop
#endif

// check for final pixel
.checklast:
	TEST	#1, R16
	MOV.W?F	R18, (R23, 0)		// write final pixel
	MOV.W?F	R18, (R23, 2)		// write final pixel

.done:
	RTS
	


.global X(R_DrawSpanLow)
X(R_DrawSpanLow):
	WEXMD

	MOV.L		X(ds_x2), R4
	ADD			#1, R4
	MOV.L		X(ds_x1), R3
	SUB			R3, R4					// pixel count
//	SHLL1		R3
	
	MOV			R4, R16
	CMP/GE		#0, R4
	BF			.hdone

	SHLD		R4, #-1, R4						// double pixel count
	MOV			R4, R17

	MOV.L	X(ds_xfrac), R20
	SHLD	R20, #10, R20
	AND		#0xffff0000, R20
	MOV.L	X(ds_yfrac), R4
	SHLD	R4, #-6, R4
	EXTU.W	R4
	OR		R4, R20
	MOV.P	X(ds_source), R22
	MOV.L	X(ds_y), R23
	MOV		X(ylookup), R3
	MOV.P	(R3, R23), R23
	MOV.L	X(ds_x1), R4
	ADD		R4, R4
	MOV		X(columnofs), R3
	MOV.L	(R3, R4), R2
	LEA.W	(R23, R2), R23

	MOV.L	X(ds_xstep), R7
	SHLD	R7, #10, R7
	AND		#0xFFFF0000, R7
	MOV.L	X(ds_ystep), R4
	SHLD	R4, #-6, R4
	EXTU.W	R4
	OR		R4, R7

	MOV		R7, R21
	
// R4		aligned colormap
// R7		aligned colormap
// R5,R6	scratch
// R22		virtual source
// R23		moving destination pointer
// R20		frac
// R16		pixelcount
// R17		loopcount

	SHLD	R5, #22, R5
	SHLD	R20, #-10, R3
	OR		R3, R5
	SHLD	R5, #6, R5
	SHLD	R20, #-26, R3
	OR		R3, R5

	ADD		R7, R20					// advance frac pointer
	AND		#4095, R5				// finish calculation for third pixel

	SHLD	R6, #22, R6
	SHLD	R20, #-10, R3
	OR		R3, R6
	SHLD	R6, #6, R6
	SHLD	R20, #-26, R3
	OR		R3, R6

	ADD		R7, R20					// advance frac pointer
	AND		#4095, R6				// finish calculation for fourth pixel
	MOV.P	X(ds_colormap), R4
	MOV		R4, R7

	MOVU.B	(R22, R5), R18			// get first pixel
	MOVU.B	(R22, R6), R19			// get second pixel
	MOVU.W	(R4, R18), R18			// color translate first pixel
	MOVU.W	(R7, R19), R19			// color translate second pixel
	
	TEST	0xFFFFFFFE, R16
	BT		.hchecklast

#if 1
.hdoubleloop:
	SHLD	R20, #-10, R2		|	SHLD	R5, #22, R5
	SHLD	R20, #-26, R3		|	SHLD	R6, #22, R6
	ADD		R21, R20
	SHLD	R20, #-10, R0		|	SHLD	R20, #-26, R1
	OR		R2, R5				|	OR		R0, R6
	SHLD	R5, #6, R5			|	SHLD	R6, #6, R6
	OR		R3, R5				|	OR		R1, R6
	MOV		#4095, R3
	AND		R3, R5
	MOV.W	R18, (R23, 0)
	MOV.W	R18, (R23, 2)
	MOV.W	R19, (R23, 4)	
	MOV.W	R19, (R23, 6)	
	AND		R3, R6				|	MOVU.B	(R22, R5), R18	
	ADD		R21, R20			|	MOVU.B	(R22, R6), R19	
	TEST	R17, R17
	ADD		#-1, R17			|	MOVU.W	(R4, R18), R18
	ADD		#8, R23				|	MOVU.W	(R7, R19), R19
	BF		.hdoubleloop
#endif

.hchecklast:
	TEST	#1, R16
	MOV.W?F		R18, (R23, 0)				// write final pixel
	MOV.W?F		R18, (R23, 2)				// write final pixel
//	NOP

.hdone:
	RTS

#endif


#if 0
.global X(I_FinishUpdate_ScanCopyClear)

X(I_FinishUpdate_ScanCopyClear):

	WEXMD

	CMP/GT	#0, R6
	BF .done

	MOV		R20, R2
	MOV		R21, R3

#if 1
	SHLD	R6, -1, R6
	.loop:
							MOV.X	(R4,    0), R16
							MOV.X	(R4,  640), R18
							MOV.X	(R4, 1280), R20
							MOV.X	(R4, 1920), R22
							MOV.Q	R16, (R5,  0)
							MOV.Q	R17, (R5, 32)
	MOV		R2, R16		
	MOV.Q	R18, (R5,  8)
	MOV		R2, R17		
	MOV.Q	R19, (R5, 40)

							MOV.X	R16, (R4,    0)
							MOV.X	R16, (R4,  640)

	MOV		R3, R18		
	MOV.Q	R20, (R5, 16)
	MOV		R3, R19		
	MOV.Q	R22, (R5, 24)

							MOV.X	R18, (R4, 1280)
							MOV.X	R18, (R4, 1920)
	ADD		#-1, R6
	MOV.Q	R21, (R5, 48)
	ADD		#16, R4
	MOV.Q	R23, (R5, 56)
	ADD		#64, R5		|	TEST	R6, R6
	BF .loop
#endif

#if 0
	.loop:
	MOV.Q	(R4,    0), R16
	MOV.Q	(R4,  640), R17
	MOV.Q	(R4, 1280), R18
	MOV.Q	(R4, 1920), R19

	MOV.Q	R20, (R4,    0)
	MOV.Q	R20, (R4,  640)
	MOV.Q	R21, (R4, 1280)
	MOV.Q	R21, (R4, 1920)

	MOV.Q	R16, (R5,  0)
	MOV.Q	R17, (R5,  8)
	ADD		#-1, R6
	MOV.Q		R18, (R5, 16)
	ADD		#8, R4
	MOV.Q		R19, (R5, 24)
	ADD		#32, R5			| TEST		R6, R6
	BF .loop
#endif

	.done:
	RTS

#endif


#if 0
.global X(I_FinishUpdate_ScanCopy)

X(I_FinishUpdate_ScanCopy):

	WEXMD

	CMP/GT	#0, R6
	BF .done

#if 0
	.loop:
	MOV.Q	(R4,    0), R20
	MOV.Q	(R4,  640), R21
	MOV.Q	(R4, 1280), R22
	MOV.Q	(R4, 1920), R23
	MOV.Q	R20, (R5,  0)
	MOV.Q	R21, (R5,  8)
	ADD		#-1, R6
	MOV.Q	R22, (R5, 16)
	ADD		#8, R4
	MOV.Q	R23, (R5, 24)
	ADD		#32, R5			| TEST		R6, R6
	BF .loop
#endif

#if 1
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
	ADD		#-1, R6		
	MOV.Q	R21, (R5, 48)
	ADD		#16, R4
	MOV.Q	R23, (R5, 56)
	ADD		#64, R5		|	TEST	R6, R6
	BF .loop
#endif

	.done:
	RTS

#endif

#if 0
// (src, dst, cnt, vbl)
.global X(I_FinishUpdate_ScanCopyVbl)
X(I_FinishUpdate_ScanCopyVbl):
//	PUSH	LR
//	PUSH.X	R30
//	PUSH.X	R28
//	PUSH.X	R26
//	PUSH.X	R24

	MOV		LR, R16
	ADD		-80, SP
	MOV.X	R16, (SP, 64)
	MOV.X	R30, (SP, 48)
	MOV.X	R28, (SP, 32)
	MOV.X	R26, (SP, 16)
	MOV.X	R24, (SP,  0)


	CMP/GT	#0, R6
	BF .done

	// R2=0xFBDEFBDEFBDEFBDE
	// R3=blend_4x
	// R4=src
	// R5=dst
	// R6=cnt
	// R7=blend
	// R16=scratch
	// R17=scratch
	// R18=vbl_alpha
	// R19=scratch
	// R20=s_pix1
	// R21=s_pix2
	// R22=s_pix3
	// R23=s_pix4
	// R24=d_pix1
	// R25=d_pix2
	// R26=d_pix3
	// R27=d_pix4

	EXTU.W	R7, R3
	SHLD	R3, 16, R2
	OR		R2, R3
	SHLD.Q	R3, 32, R2
	OR		R2, R3
//	MOV		0xFBDEFBDEFBDEFBDE, R2
	MOV		0x7BDE7BDE7BDE7BDE, R2
	AND		R2, R3
	SHLD.Q	R3, #-1, R3
	
	SHLD	R7, #-16, R18

	.loop:
								MOV.Q	(R4,    0), R20
								MOV.Q	(R4,  640), R21
								MOV.Q	(R4, 1280), R22
								MOV.Q	(R4, 1920), R23

#if 1
	AND		R20, R2, R20	|	AND		R21, R2, R21
	AND		R22, R2, R22	|	AND		R23, R2, R23
	SHLD.Q	R20, #-1, R20	|	SHLD.Q	R21, #-1, R21
	SHLD.Q	R22, #-1, R22	|	SHLD.Q	R23, #-1, R23
	MOV		R3, R19			|	TEST	#0x4, R18

//	MOV?F	R19, R24
//	MOV?F	R19, R25
//	MOV?F	R19, R26
//	MOV?F	R19, R27
//	MOV?T	R20, R24
//	MOV?T	R21, R25
//	MOV?T	R22, R26
//	MOV?T	R23, R27

	CSELT	R20, R19, R24	|	CSELT	R21, R19, R25
	CSELT	R22, R19, R26	|	CSELT	R23, R19, R27

	AND		R20, R2, R20	|	AND		R21, R2, R21
	AND		R22, R2, R22	|	AND		R23, R2, R23
	AND		R19, R2, R19
	SHLD.Q	R20, #-1, R20	|	SHLD.Q	R21, #-1, R21
	SHLD.Q	R22, #-1, R22	|	SHLD.Q	R23, #-1, R23
	SHLD.Q	R19, #-1, R19	|	TEST	#0x2, R18

//	ADD?F	R19, R24
//	ADD?F	R19, R25
//	ADD?F	R19, R26
//	ADD?F	R19, R27
//	ADD?T	R20, R24
//	ADD?T	R21, R25
//	ADD?T	R22, R26
//	ADD?T	R23, R27

	ADD?F	R19, R24	|	ADD?F	R19, R25
	ADD?F	R19, R26	|	ADD?F	R19, R27
	ADD?T	R20, R24	|	ADD?T	R21, R25
	ADD?T	R22, R26	|	ADD?T	R23, R27

	AND		R20, R2, R20	|	AND		R21, R2, R21
	AND		R22, R2, R22	|	AND		R23, R2, R23
	AND		R19, R2, R19
	SHLD.Q	R20, #-1, R20	|	SHLD.Q	R21, #-1, R21
	SHLD.Q	R22, #-1, R22	|	SHLD.Q	R23, #-1, R23
	SHLD.Q	R19, #-1, R19	|	TEST	#0x1, R18
//	ADD?F	R19, R24
//	ADD?F	R19, R25
//	ADD?F	R19, R26
//	ADD?F	R19, R27
//	ADD?T	R20, R24
//	ADD?T	R21, R25
//	ADD?T	R22, R26
//	ADD?T	R23, R27

	ADD?F	R19, R24	|	ADD?F	R19, R25
	ADD?F	R19, R26	|	ADD?F	R19, R27
	ADD?T	R20, R24	|	ADD?T	R21, R25
	ADD?T	R22, R26	|	ADD?T	R23, R27
#endif

								MOV.Q	R24, (R5,  0)
								MOV.Q	R25, (R5,  8)
	ADD		#-1, R6
	MOV.Q	R26, (R5, 16)
	ADD		#8, R4
	MOV.Q	R27, (R5, 24)
	ADD		#32, R5			|	TEST	R6, R6
	BF .loop

	.done:
	MOV.X	(SP,  0), R24
	MOV.X	(SP, 16), R26
	MOV.X	(SP, 32), R28
	MOV.X	(SP, 48), R30
	MOV.X	(SP, 64), R16
	ADD		80, SP
	JMP		R16

//	POP.X	R24
//	POP.X	R26
//	POP.X	R28
//	POP.X	R30
//	POP		LR
//	RTS
#endif


#if 1
.global X(V_DrawPatchCmap_Col)
X(V_DrawPatchCmap_Col):

#if 0
	CMP/GE	#4, R7
	BF		.done1
	.loop1:
	MOVU.B		(R4, 0), R20
	MOVU.B		(R4, 1), R21
	MOVU.B		(R4, 2), R22
	MOVU.B		(R4, 3), R23
	MOVU.W		(R6, R20), R20
	MOVU.W		(R6, R21), R21
	MOVU.W		(R6, R22), R22
	MOVU.W		(R6, R23), R23
	MOV.W		R20, (R5, 0)
	MOV.W		R21, (R5, 640)
	MOV.W		R22, (R5, 1280)
	MOV.W		R23, (R5, 1920)
	ADD			#4, R4
	ADD			#2560, R5
	
	ADD		#-4, R7
	CMP/GE	#4, R7
	BT		.loop1
	.done1:
#endif

#if 1
	CMP/GE	#2, R7
	BF		.done1
	.loop1:
	MOVU.B		(R4, 0), R2
	MOVU.B		(R4, 1), R3
	ADD			#2, R4
	MOVU.W		(R6, R2), R2
	MOVU.W		(R6, R3), R3
	ADD			#-2, R7
	MOV.W		R2, (R5, 0)
	CMP/GE		#2, R7
	MOV.W		R3, (R5, 640)
	ADD			#1280, R5	
	BT		.loop1
	.done1:
#endif

#if 0
	TEST	R7, R7
	BT		.done2

	.loop2:
	MOVU.B		(R4), R2
	MOVU.W		(R6, R2), R3
	MOV.W		R3, (R5)
	ADD			#1, R4
	ADD			#640, R5
	ADD		#-1, R7
	TEST	R7, R7
	BF		.loop2
	.done2:
#endif

#if 1
	TEST	R7, R7
	BT		.done2
	MOVU.B		(R4), R2
	MOVU.W		(R6, R2), R3
	MOV.W		R3, (R5)
	.done2:
#endif

	RTS

#endif
