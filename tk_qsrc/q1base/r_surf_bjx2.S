#define X(n)	n

.section .text

#if 1
.global X(R_DrawSurfaceBlock16_mip0A)
X(R_DrawSurfaceBlock16_mip0A):

//	PUSH.X	R26
//	PUSH.X	R24
//	PUSH.X	R12
//	PUSH.X	R10
//	PUSH.X	R8

	ADD		-80, SP
	MOV.X	R26, (SP, 64)
	MOV.X	R24, (SP, 48)
	MOV.X	R12, (SP, 32)
	MOV.X	R10, (SP, 16)
	MOV.X	R8,  (SP,  0)

	MOV.L	X(r_numvblocks), R16

	MOV.P	X(r_vid_colormap16), R18
	MOV.P	X(pbasesource), R19
	MOV.P	X(prowdestbase), R20

	MOV.P	X(r_lightptr), R22
	MOV.L	X(r_lightwidth), R23

	MOV.L	X(sourcetstep), R8
	MOV.L	X(surfrowbytes), R9
	MOV.L	X(r_sourcemax), R10
	MOV.L	X(r_stepback), R11

	MOV		#0xFF00, R13

// R2: scratch
// R3: scratch
// R4: lighttemp
// R5: lightstep
// R6: light
// R7: pix

// R8: sourcetstep
// R9: surfrowbytes
// R10: r_sourcemax
// R11: r_stepback
// R12: scratch
// R13: 0xFF00
// R14: -
// R15: SP
// R16: v
// R17: i
// R18: colormap
// R19: psource
// R20: prowdest
// R21: b
// R22: r_lightptr
// R23: r_lightwidth
// R24: lightleft
// R25: lightright
// R26: lightleftstep
// R27: lightrightstep
// R28: -
// R29: -
// R30: -

	.L0:

	SHAD	R23, #2, R2		| MOVU.L	(R22, 0), R24
	ADDS.L	R22, R2, R12	| MOVU.L	(R22, 4), R25
	MOV		R12, R22		| MOV.L		(R12, 0), R26
	SUBU.L	R26, R24, R26	| MOV.L		(R12, 4), R27
	SUBU.L	R27, R25, R27	| SHLD		R26, #-4, R26
	SHLD	R27, #-4, R27	| MOV		#16, R17

	.L1:
	SUBS.L	R24, R25, R4	| MOV		#16, R21
	SHAD	R4, #-4, R5		| MOV		R25, R6

#if 0
	.L2:
	ADD		#-1, R21
	AND		R6, R13, R2		| MOVU.B	(R19, R21), R7
	OR		R2, R7, R3
	ADD		R5, R6			| MOVU.W	(R18, R3), R2
	AND		R6, R13, R3		| MOV.W		R2, (R20, R21)
	ADD		#-1, R21
	MOVU.B	(R19, R21), R7
	OR		R3, R7, R3
	ADD		R5, R6			| MOVU.W	(R18, R3), R2
	MOV.W	R2, (R20, R21)
	CMP/GT	#0, R21
	BT		.L2
#endif

#if 1
	.L2:
	ADDS.L	R21, #-1, R12	| ADDS.L	R21, #-2, R21
	AND		R6, R13, R2		| MOVU.B	(R19, R12), R7
	OR		R2, R7, R3		| CMP/GT	#0, R21
	ADDS.L	R6, R5, R6		| MOVU.W	(R18, R3), R2
	AND		R6, R13, R3		| MOV.W		R2, (R20, R12)
	MOVU.B	(R19, R21), R7
	OR		R3, R7, R3	
	ADDS.L	R6, R5, R6		| MOVU.W	(R18, R3), R2
	MOV.W	R2, (R20, R21)
	BT		.L2
#endif

	ADD		#-1, R17	| ADD		R8, R19
	ADD		R27, R25	| ADD		R26, R24
	ADD		R9, R20		| CMP/GT	#0, R17
	BT	.L1

	ADD		#-1, R16	| CMP/GE	R10, R19
	SUB?T	R11, R19
	CMP/GT	#0, R16
	BT	.L0

	MOV.X	(SP,  0), R8
	MOV.X	(SP, 16), R10
	MOV.X	(SP, 32), R12
	MOV.X	(SP, 48), R24
	MOV.X	(SP, 64), R26
	ADD		80, SP

//	POP.X	R8
//	POP.X	R10
//	POP.X	R12
//	POP.X	R24
//	POP.X	R26
	RTSU

#endif



#if 1
.global X(R_DrawSurfaceBlock16_mip1A)
X(R_DrawSurfaceBlock16_mip1A):

//	PUSH.X	R26
//	PUSH.X	R24
//	PUSH.X	R12
//	PUSH.X	R10
//	PUSH.X	R8

	ADD		-80, SP
	MOV.X	R26, (SP, 64)
	MOV.X	R24, (SP, 48)
	MOV.X	R12, (SP, 32)
	MOV.X	R10, (SP, 16)
	MOV.X	R8,  (SP,  0)

	MOV.L	X(r_numvblocks), R16

	MOV.P	X(r_vid_colormap16), R18
	MOV.P	X(pbasesource), R19
	MOV.P	X(prowdestbase), R20

	MOV.P	X(r_lightptr), R22
	MOV.L	X(r_lightwidth), R23

	MOV.L	X(sourcetstep), R8
	MOV.L	X(surfrowbytes), R9
	MOV.L	X(r_sourcemax), R10
	MOV.L	X(r_stepback), R11

// R4: lighttemp
// R5: lightstep
// R6: light
// R7: pix

// R8: sourcetstep
// R9: surfrowbytes
// R10: r_sourcemax
// R11: r_stepback

// R16: v
// R17: i
// R18: colormap
// R19: psource
// R20: prowdest
// R21: b
// R22: r_lightptr
// R23: r_lightwidth
// R24: lightleft
// R25: lightright
// R26: lightleftstep
// R27: lightrightstep

	.L0:

	MOVU.L	(R22, 0), R24
	MOVU.L	(R22, 4), R25
	LEA.L	(R22, R23), R22

	MOV.L	(R22, 0), R26
	SUB		R24, R26		| MOV.L		(R22, 4), R27
	SUB		R25, R27		| SHLD		R26, #-3, R26
	SHLD	R27, #-3, R27	| MOV		#8, R17
//	MOV		#0xFF00, R1
	MOV		#0xFF00, R13

	.L1:

//	SUB		R24, R25, R4	| MOV		#7, R21
	SUB		R24, R25, R4	| MOV		#8, R21
	SHAD	R4, #-3, R5		| MOV		R25, R6

#if 1
	.L2:
	ADDS.L	R21, #-1, R12	| ADDS.L	R21, #-2, R21
	AND		R6, R13, R2		| MOVU.B	(R19, R12), R7
	OR		R2, R7, R3		| CMP/GT	#0, R21
	ADDS.L	R6, R5, R6		| MOVU.W	(R18, R3), R2
	AND		R6, R13, R3		| MOV.W		R2, (R20, R12)
	MOVU.B	(R19, R21), R7
	OR		R3, R7, R3
	ADDS.L	R6, R5, R6		| MOVU.W	(R18, R3), R2
	MOV.W	R2, (R20, R21)
	BT		.L2
#endif

#if 0
	.L2:
	ADD		#-1, R21
	AND		R6, R13, R2		| MOVU.B	(R19, R21), R7
	OR		R2, R7, R3
	ADD		R5, R6			| MOVU.W	(R18, R3), R2
	MOV.W		R2, (R20, R21)
	ADD		#-1, R21
	AND		R6, R13, R3		| MOVU.B	(R19, R21), R7
	OR		R3, R7, R3
	ADD		R5, R6			| MOVU.W	(R18, R3), R2
	MOV.W	R2, (R20, R21)

	CMP/GT	#0, R21
	BT		.L2
#endif

#if 0
	.L2:
	ADD		#-1, R21
	MOVU.B	(R19, R21), R7
	AND		R6, R13, R2
	OR		R2, R7, R3
	MOVU.W	(R18, R3), R2
	MOV.W	R2, (R20, R21)
	
	ADD		R5, R6
	CMP/GE	#0, R21
	BT		.L2
#endif

	ADD		R8, R19		| ADD		R27, R25
	ADD		R26, R24	| ADD		#-1, R17
	ADD		R9, R20		| CMP/GT	#0, R17
	BT	.L1

	CMP/GE	R10, R19
	SUB?T	R11, R19
	
	ADD		#-1, R16
	CMP/GT	#0, R16
	BT	.L0

	MOV.X	(SP,  0), R8
	MOV.X	(SP, 16), R10
	MOV.X	(SP, 32), R12
	MOV.X	(SP, 48), R24
	MOV.X	(SP, 64), R26
	ADD		80, SP

//	POP.X	R8
//	POP.X	R10
//	POP.X	R12
//	POP.X	R24
//	POP.X	R26
	RTSU

#endif

#if 1
.global X(R_DrawSurfaceBlock16_mip2A)
X(R_DrawSurfaceBlock16_mip2A):
//	PUSH.X	R26
//	PUSH.X	R24
//	PUSH.X	R10
//	PUSH.X	R8

	ADD		-80, SP
	MOV.X	R26, (SP, 64)
	MOV.X	R24, (SP, 48)
//	MOV.X	R12, (SP, 32)
	MOV.X	R10, (SP, 16)
	MOV.X	R8,  (SP,  0)


	MOV.L	X(r_numvblocks), R16

	MOV.P	X(r_vid_colormap16), R18
	MOV.P	X(pbasesource), R19
	MOV.P	X(prowdestbase), R20

	MOV.P	X(r_lightptr), R22
	MOV.L	X(r_lightwidth), R23

	MOV.L	X(sourcetstep), R8
	MOV.L	X(surfrowbytes), R9
	MOV.L	X(r_sourcemax), R10
	MOV.L	X(r_stepback), R11

	.L0:

	MOVU.L	(R22, 0), R24
	MOVU.L	(R22, 4), R25
	LEA.L	(R22, R23), R22

	MOV.L	(R22, 0), R26
	SUB		R24, R26		| MOV.L		(R22, 4), R27
	SUB		R25, R27		| SHLD		R26, #-2, R26
	SHLD	R27, #-2, R27	| MOV		#4, R17
	MOV		#0xFF00, R1

	.L1:

	SUB		R24, R25, R4	| MOV		#4, R21
	SHAD	R4, #-2, R5		| MOV		R25, R6

	.L2:
	ADD		#-1, R21
	MOVU.B	(R19, R21), R7
	MOV		#0xFF00, R1
	AND		R6, R1, R2
	OR		R2, R7, R3
	MOVU.W	(R18, R3), R2
	MOV.W	R2, (R20, R21)
	
	ADD		R5, R6
	CMP/GE	#0, R21
	BT		.L2

	ADD		R8, R19		| ADD		R27, R25
	ADD		R26, R24	| ADD		#-1, R17
	ADD		R9, R20		| CMP/GT	#0, R17
	BT	.L1

	CMP/GE	R10, R19
	SUB?T	R11, R19
	
	ADD		#-1, R16
	CMP/GT	#0, R16
	BT	.L0

	MOV.X	(SP,  0), R8
	MOV.X	(SP, 16), R10
//	MOV.X	(SP, 32), R12
	MOV.X	(SP, 48), R24
	MOV.X	(SP, 64), R26
	ADD		80, SP

//	POP.X	R8
//	POP.X	R10
//	POP.X	R24
//	POP.X	R26
	RTSU
#endif

#if 1
.global X(R_DrawSurfaceBlock16_mip3A)
X(R_DrawSurfaceBlock16_mip3A):
//	PUSH.X	R26
//	PUSH.X	R24
//	PUSH.X	R10
//	PUSH.X	R8

	ADD		-80, SP
	MOV.X	R26, (SP, 64)
	MOV.X	R24, (SP, 48)
//	MOV.X	R12, (SP, 32)
	MOV.X	R10, (SP, 16)
	MOV.X	R8,  (SP,  0)

	MOV.L	X(r_numvblocks), R16

	MOV.P	X(r_vid_colormap16), R18
	MOV.P	X(pbasesource), R19
	MOV.P	X(prowdestbase), R20

	MOV.P	X(r_lightptr), R22
	MOV.L	X(r_lightwidth), R23

	MOV.L	X(sourcetstep), R8
	MOV.L	X(surfrowbytes), R9
	MOV.L	X(r_sourcemax), R10
	MOV.L	X(r_stepback), R11

	.L0:

	MOVU.L	(R22, 0), R24
	MOVU.L	(R22, 4), R25
	LEA.L	(R22, R23), R22

	MOV.L	(R22, 0), R26
	SUB		R24, R26		| MOV.L		(R22, 4), R27
	SUB		R25, R27		| SHLD		R26, #-1, R26
	SHLD	R27, #-1, R27	| MOV		#2, R17
	MOV		#0xFF00, R1

	.L1:

	SUB		R24, R25, R4	| MOV		#2, R21
	SHAD	R4, #-1, R5		| MOV		R25, R6

	.L2:
	ADD		#-1, R21
	MOVU.B	(R19, R21), R7
	MOV		#0xFF00, R1
	AND		R6, R1, R2
	OR		R2, R7, R3
	MOVU.W	(R18, R3), R2
	MOV.W	R2, (R20, R21)
	
	ADD		R5, R6
	CMP/GE	#0, R21
	BT		.L2

	ADD		R8, R19		| ADD		R27, R25
	ADD		R26, R24	| ADD		#-1, R17
	ADD		R9, R20		| CMP/GT	#0, R17
	BT	.L1

	CMP/GE	R10, R19
	SUB?T	R11, R19
	
	ADD		#-1, R16
	CMP/GT	#0, R16
	BT	.L0

	MOV.X	(SP,  0), R8
	MOV.X	(SP, 16), R10
//	MOV.X	(SP, 32), R12
	MOV.X	(SP, 48), R24
	MOV.X	(SP, 64), R26
	ADD		80, SP

//	POP.X	R8
//	POP.X	R10
//	POP.X	R24
//	POP.X	R26
	RTS

#endif



#if 1
.global X(I_FinishUpdate_ScanCopy)

X(I_FinishUpdate_ScanCopy):

	CMP/GT	#0, R6
	BF .done

	.loop:
	MOV.Q	(R4,    0), R20
	MOV.Q	(R4,  640), R21
	MOV.Q	(R4, 1280), R22
	MOV.Q	(R4, 1920), R23
	MOV.Q	R20, (R5,  0)
	MOV.Q	R21, (R5,  8)
	ADD		#-1, R6			| MOV.Q		R22, (R5, 16)
	ADD		#8, R4			| MOV.Q		R23, (R5, 24)
	ADD		#32, R5			| TEST		R6, R6
	BF .loop

	.done:
	RTS

#endif


#if 0
.global X(D_DrawSpans16_InnerPx)

X(D_DrawSpans16_InnerPx):

	MOV.L	X(d_sstep2), R16
	MOV.L	X(d_tstep2), R17
	MOV.L	X(d_pbase2), R18
	MOV.L	X(d_pdest2), R19
	MOV.L	X(d_s2), R20
	MOV.L	X(d_t2), R21
	MOV.L	X(d_spancount2), R22
	MOV.L	X(cachewidth), R23

	CMP/GE	#2, R22
	BF .L1
	.L0:
	SHAD	R20, #-16, R4	| SHAD		R21, #-16, R5
	ADDS.L	R20, R16, R20	| MULS.W	R5, R23, R6
	ADDS.L	R21, R17, R21	| ADDS.L	R4, R6, R7
	SHAD	R20, #-16, R4	| MOVU.W	(R18, R7), R2
	SHAD	R21, #-16, R5	| MOV.W		R2, (R19, 0)
	ADDS.L	R20, R16, R20	| MULS.W	R5, R23, R6
	ADDS.L	R22, #-2, R22	| ADDS.L	R4, R6, R7
	ADDS.L	R21, R17, R21	| MOVU.W	(R18, R7), R2
	MOV.W	R2, (R19, 2)
	ADDS.L	#4, R19			| CMP/GE	#2, R22
	BT .L0
	.L1:
	
	TEST R22, R22
	BT .L2

	SHAD	R20, #-16, R4	| SHAD	R21, #-16, R5
	MULS.W	R5, R23, R6
	ADD		R4, R6, R7
	MOVU.W	(R18, R7), R2
	MOV.W	R2, (R19)
	ADD		#2, R19

	.L2:

//	MOV.L	R19, X(d_pdest2)
//	MOV		X(d_pdest2), R2
//	MOV.L	R19, (R2)

	RTS

#endif

#if 1
.global X(D_DrawSpans16_InnerPx2)

X(D_DrawSpans16_InnerPx2):
	CMP/GE	#2, R6
	BF .L1
	.L0:
	SHAD	R20, #-16, R2	| SHAD		R21, #-16, R3
	ADDS.L	R20, R22, R20	| MULS.W	R3, R7, R18
	ADDS.L	R21, R23, R21	| ADDS.L	R2, R18, R19
	SHAD	R20, #-16, R2	| MOVU.W	(R4, R19), R16
	SHAD	R21, #-16, R3	| MOV.W		R16, (R5, 0)
	ADDS.L	R20, R22, R20	| MULS.W	R3, R7, R18
	ADDS.L	R6, #-2, R6		| ADDS.L	R2, R18, R19
	ADDS.L	R21, R23, R21	| MOVU.W	(R4, R19), R16
	MOV.W	R16, (R5, 2)
	ADD		R5, #4, R5		| CMP/GE	#2, R6
	BT .L0
	.L1:
	TEST R6, R6
	BT .L2
	SHAD	R20, #-16, R2	| SHAD	R21, #-16, R3
	MULS.W	R3, R7, R18
	ADDS.L	R2, R18, R19
	MOVU.W	(R4, R19), R16
	MOV.W	R16, (R5)
	ADDS.L	R5, #2, R5
	.L2:
	RTS

#endif


#if 1
.global X(D_DrawSpans16_InnerPx2Lo)

X(D_DrawSpans16_InnerPx2Lo):
#if 1
	SHAD	R22, 1, R22
	SHAD	R23, 1, R23
	CMP/GE	#4, R6
	BF .L1
	.L0:
	SHAD	R20, -16, R2	|	SHAD	R21, -16, R3
	ADDS.L	R20, R22, R20	|	MULS.W	R3, R7, R18
	ADDS.L	R21, R23, R21	|	ADDS.L	R2, R18, R19
	SHAD	R20, #-16, R2	|	MOVU.W	(R4, R19), R16
	SHAD	R21, #-16, R3	|	PSHUF.W	R16, 0, R16
								MOV.L	R16, (R5, 0)
	ADDS.L	R20, R22, R20	|	MULS.W	R3, R7, R18
	ADDS.L	R6, -4, R6		|	ADDS.L	R2, R18, R19
	ADDS.L	R21, R23, R21	|	MOVU.W	(R4, R19), R16
	PSHUF.W	R16, 0, R16
								MOV.L	R16, (R5, 4)
	ADD		R5, 8, R5		|	CMP/GT	#4, R6
	BT		.L0
#endif

#if 0
	SHAD	R22, 2, R22
	SHAD	R23, 2, R23
	CMP/GE	#8, R6
	BF .L1
	.L0:
	SHAD	R20, -16, R2	|	SHAD	R21, -16, R3
	ADDS.L	R20, R22, R20	|	MULS.W	R3, R7, R18
	ADDS.L	R21, R23, R21	|	ADDS.L	R2, R18, R19
	SHAD	R20, #-16, R2	|	MOVU.W	(R4, R19), R16
	SHAD	R21, #-16, R3	|	PSHUF.W	R16, 0, R16
								MOV.Q	R16, (R5, 0)
	ADDS.L	R20, R22, R20	|	MULS.W	R3, R7, R18
	ADDS.L	R6, -8, R6		|	ADDS.L	R2, R18, R19
	ADDS.L	R21, R23, R21	|	MOVU.W	(R4, R19), R16
	PSHUF.W	R16, 0, R16
								MOV.Q	R16, (R5, 8)
	ADD		R5, 16, R5		|	CMP/GE	#8, R6
	BT		.L0
#endif

	.L1:
	TEST R6, R6
	BT .L2
	SHAD	R20, -16, R2	| SHAD	R21, -16, R3
	MULS.W	R3, R7, R18
	ADDS.L	R2, R18, R19
	MOVU.W	(R4, R19), R16
	.L3:
	ADDS.L	R6, -1, R6		|	MOV.W	R16, (R5)
	ADD		R5,  2, R5		|	TEST	R6, R6
	BF		.L3
	.L2:
	RTS

#endif


#if 1
.global X(D_DrawZSpans_Inner)
X(D_DrawZSpans_Inner):

#if 1
	MOV		#-65536, R16	| CMP/GE	#4, R7
	BF		.L3
	.L2:
	SHLD	R5, #-16, R2	| ADDU.L	R6, R5
	AND		R5, R16, R3		| ADDU.L	R6, R5
	OR		R3, R2
	SHLD	R5, #-16, R18	| ADDU.L	R6, R5
	AND		R5, R16, R17	| ADDU.L	R6, R5
	OR		R17, R18
	SHLD.Q	R18, #32, R3
	OR		R3, R2

	ADDS.L	#-4, R7			| MOV.Q		R2, (R4)
	ADD		#8, R4			| CMP/GE	#4, R7
	BT		.L2
	.L3:
#endif

	MOV		#-65536, R16	| CMP/GE	#2, R7
	BF		.L1
	.L0:
	SHLD	R5, #-16, R2	| ADD		R6, R5
	AND		R5, R16, R3		| ADD		R6, R5
	OR		R3, R2
	ADD		#-2, R7			| MOV.L		R2, (R4)
	ADD		#4, R4			| CMP/GE	#2, R7
	BT		.L0
	.L1:
	SHLD		R5, #-16, R2	| TEST		R7, R7
	MOV.W?F		R2, (R4)
	RTS

#endif


#if 1
.global X(D_DrawZSpans_InnerLo)
X(D_DrawZSpans_InnerLo):

#if 1
	MOV		#-65536, R16	| CMP/GE	#8, R7
	BF		.L3
	.L2:
	SHLD	R5, #-16, R2	| ADDU.L	R6, R5
	AND		R5, R16, R3
	OR		R3, R2
	SHLD.Q	R2, #32, R3		| ADDU.L	R6, R5
	OR		R3, R2
	ADDS.L	#-8, R7
	MOV.Q	R2, (R4, 0)
	MOV.Q	R2, (R4, 8)

	ADD		#16, R4			| CMP/GE	#8, R7
	BT		.L2
	.L3:
#endif

	MOV		#-65536, R16	| CMP/GE	#2, R7
	BF		.L1
	.L0:
	SHLD	R5, #-16, R2	| ADD		R6, R5
	AND		R5, R16, R3		| ADD		R6, R5
	OR		R3, R2
	ADD		#-2, R7			| MOV.L		R2, (R4)
	ADD		#4, R4			| CMP/GE	#2, R7
	BT		.L0
	.L1:
	SHLD		R5, #-16, R2	| TEST		R7, R7
	MOV.W?F		R2, (R4)
	RTS

#endif

#if 1
.global X(SND_PaintChannelFrom8_Inner)
X(SND_PaintChannelFrom8_Inner):

	CMP/GT #0, R20
	BF .L0
	.L0:
	MOV.B	(R4), R2
	MULS.W	R2, R6, R18		|	MOV.L	(R5, 0), R16
	MULS.W	R2, R7, R19		|	MOV.L	(R5, 4), R17
	ADD #1, R4				|	ADDS.L	R16, R18, R16
	ADDS.L	R17, R19, R17	|	MOV.L	R16, (R5, 0)
	ADD #-1, R20			|	MOV.L	R17, (R5, 4)
	ADD #8, R5				|	CMP/GT #0, R20
	BT .L0
	.L1:

	RTS
#endif


#if 1
D_SoftDivB_Dn:
	BRA D_SoftDiv
D_SoftDivB_Df:
	DMULS	R4, R6, R7
	SHAD.Q	R7, -32, R2

//	MULS	R4, R6
//	EXTS.L	R1, R2
	RTS

D_SoftDivB_D0:
D_SoftDivB_D1:
	MOV R4, R2
	RTS
D_SoftDivB_D2:
	SHAD R4, #-1, R2
	RTS
D_SoftDivB_D3:
	MOV #0x55555555, R6
	BRA	D_SoftDivB_Df
D_SoftDivB_D4:
	SHAD R4, #-2, R2
	RTS
D_SoftDivB_D5:
	MOV #0x33333333, R6
	BRA	D_SoftDivB_Df
D_SoftDivB_D6:
	MOV #0x2AAAAAAA, R6
	BRA	D_SoftDivB_Df
D_SoftDivB_D7:
	MOV #0x24924925, R6
	BRA	D_SoftDivB_Df
D_SoftDivB_D8:
	SHAD R4, #-3, R2
	RTS
D_SoftDivB_D9:
	MOV #0x1C71C71C, R6
	BRA	D_SoftDivB_Df
D_SoftDivB_D10:
	MOV #0x1999999A, R6
	BRA	D_SoftDivB_Df
D_SoftDivB_D11:
	MOV #0x1745D174, R6
	BRA	D_SoftDivB_Df
D_SoftDivB_D12:
	MOV #0x15555555, R6
	BRA	D_SoftDivB_Df
D_SoftDivB_D13:
	MOV #0x13B13B14, R6
	BRA	D_SoftDivB_Df
D_SoftDivB_D14:
	MOV #0x12492492, R6
	BRA	D_SoftDivB_Df
D_SoftDivB_D15:
	MOV #0x11111111, R6
	BRA	D_SoftDivB_Df
D_SoftDivB_D16:
	SHAD R4, #-4, R2
	RTS

.global X(D_SoftDivB)
X(D_SoftDivB):
	CMP/HI #16, R5
	BT		D_SoftDivB_Dn
//	BRA		D_SoftDivB_Dn
.ifarch bjx2_fix32
	ADD		R5, R5
	BRAF	R5
	BRA4B	D_SoftDivB_D0
	BRA4B	D_SoftDivB_D1
	BRA4B	D_SoftDivB_D2
	BRA4B	D_SoftDivB_D3
	BRA4B	D_SoftDivB_D4
	BRA4B	D_SoftDivB_D5
	BRA4B	D_SoftDivB_D6
	BRA4B	D_SoftDivB_D7
	BRA4B	D_SoftDivB_D8
	BRA4B	D_SoftDivB_D9
	BRA4B	D_SoftDivB_D10
	BRA4B	D_SoftDivB_D11
	BRA4B	D_SoftDivB_D12
	BRA4B	D_SoftDivB_D13
	BRA4B	D_SoftDivB_D14
	BRA4B	D_SoftDivB_D15
	BRA4B	D_SoftDivB_D16
.else
	BRAF	R5
	BRA2B	D_SoftDivB_D0
	BRA2B	D_SoftDivB_D1
	BRA2B	D_SoftDivB_D2
	BRA2B	D_SoftDivB_D3
	BRA2B	D_SoftDivB_D4
	BRA2B	D_SoftDivB_D5
	BRA2B	D_SoftDivB_D6
	BRA2B	D_SoftDivB_D7
	BRA2B	D_SoftDivB_D8
	BRA2B	D_SoftDivB_D9
	BRA2B	D_SoftDivB_D10
	BRA2B	D_SoftDivB_D11
	BRA2B	D_SoftDivB_D12
	BRA2B	D_SoftDivB_D13
	BRA2B	D_SoftDivB_D14
	BRA2B	D_SoftDivB_D15
	BRA2B	D_SoftDivB_D16
.endif

#endif

#if 1
.global X(VID_BlendEven16)
X(VID_BlendEven16):
//	MOV		#0xFBDF, R3
	MOV		#0x7BDE, R3
	AND		R4, R3, R6
	AND		R5, R3, R7
	ADD		R6, R7, R2
	SHAD	R2, #-1, R2
	RTS
#endif

#if 1
.global X(TransformVector)
X(TransformVector):

.ifarch has_fmovs
	FMOV.S	(R4, 0), R21
	FMOV.S	(R4, 4), R22
	FMOV.S	(R4, 8), R23
.else
	MOV.Q	(R4, 0), R2
	MOVD.L	(R4, 8), R3
	FLDCF	R2, R21
	FLDCFH	R2, R22
	FLDCF	R3, R23
.endif

	MOV		X(vright), R6
.ifarch has_fmovs
	FMOV.S	(R6, 0), R18
	FMOV.S	(R6, 4), R19
	FMOV.S	(R6, 8), R20
.else
	MOV.Q	(R6, 0), R16
	MOVD.L	(R6, 8), R17
	FLDCF	R16, R18
	FLDCFH	R16, R19
	FLDCF	R17, R20
.endif

	FMULA	R18, R21, R2
	FMULA	R19, R22, R3
	FMULA	R20, R23, R7
	FADDA	R2, R3, R2
	FADDA	R2, R7, R2
.ifarch has_fmovs
	FMOV.S	R2, (R5, 0)
.else
	FSTCF	R2, R3
	MOV.L	R3, (R5, 0)
.endif

	MOV		X(vup), R6

.ifarch has_fmovs
	FMOV.S	(R6, 0), R18
	FMOV.S	(R6, 4), R19
	FMOV.S	(R6, 8), R20
.else
	MOV.Q	(R6, 0), R16
	MOVD.L	(R6, 8), R17
	FLDCF	R16, R18
	FLDCFH	R16, R19
	FLDCF	R17, R20
.endif

	FMULA	R18, R21, R2
	FMULA	R19, R22, R3
	FMULA	R20, R23, R7
	FADDA	R2, R3, R2
	FADDA	R2, R7, R2
.ifarch has_fmovs
	FMOV.S	R2, (R5, 4)
.else
	FSTCF	R2, R3
	MOV.L	R3, (R5, 4)
.endif

	MOV		X(vpn), R6
.ifarch has_fmovs
	FMOV.S	(R6, 0), R18
	FMOV.S	(R6, 4), R19
	FMOV.S	(R6, 8), R20
.else
	MOV.Q	(R6, 0), R16
	MOVD.L	(R6, 8), R17
	FLDCF	R16, R18
	FLDCFH	R16, R19
	FLDCF	R17, R20
.endif

	FMULA	R18, R21, R2
	FMULA	R19, R22, R3
	FMULA	R20, R23, R7
	FADDA	R2, R3, R2
	FADDA	R2, R7, R2

.ifarch has_fmovs
	FMOV.S	R2, (R5, 8)
.else
	FSTCF	R2, R3
	MOV.L	R3, (R5, 8)
.endif
	
	RTSU
#endif
