#define X(n)	n

.section .text

#if 1
.global X(R_DrawSurfaceBlock16_mip0)
X(R_DrawSurfaceBlock16_mip0):

	PUSH.X	R26
	PUSH.X	R24
	PUSH.X	R10
	PUSH.X	R8

	MOV.L	X(r_numvblocks), R16

	MOV.L	X(r_vid_colormap16), R18
	MOV.L	X(pbasesource), R19
	MOV.L	X(prowdestbase), R20

	MOV.L	X(r_lightptr), R22
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
	SUB		R25, R27		| SHLD	R26, #-4, R26
	SHLD	R27, #-4, R27	| MOV #16, R17
	MOV		#0xFF00, R1

	.L1:
	SUB		R24, R25, R4	| MOV		#16, R21
	SHAD	R4, #-4, R5		| MOV		R25, R6
	.L2:
	ADD		#-1, R21
	AND		R6, R1, R2		| MOVU.B	(R19, R21), R7
	OR		R2, R7, R3
	ADD		R5, R6			| MOVU.W	(R18, R3), R2
	AND		R6, R1, R3		| MOV.W		R2, (R20, R21)
	ADD		#-1, R21
	OR		R3, R7, R3		| MOVU.B	(R19, R21), R7
	ADD		R5, R6			| MOVU.W	(R18, R3), R2
	MOV.W	R2, (R20, R21)
	CMP/GT	#0, R21
	BT		.L2

	ADD		#-1, R17	| ADD		R8, R19
	ADD		R27, R25	| ADD		R26, R24
	ADD		R9, R20		| CMP/GT	#0, R17
	BT	.L1

	ADD		#-1, R16	| CMP/GE	R10, R19
	SUB?T	R11, R19
	CMP/GT	#0, R16
	BT	.L0

	POP.X	R8
	POP.X	R10
	POP.X	R24
	POP.X	R26
	RTSU

#endif



#if 1
.global X(R_DrawSurfaceBlock16_mip1)
X(R_DrawSurfaceBlock16_mip1):

	PUSH.X	R26
	PUSH.X	R24
	PUSH.X	R10
	PUSH.X	R8

	MOV.L	X(r_numvblocks), R16

	MOV.L	X(r_vid_colormap16), R18
	MOV.L	X(pbasesource), R19
	MOV.L	X(prowdestbase), R20

	MOV.L	X(r_lightptr), R22
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
	MOV		#0xFF00, R1

	.L1:

//	SUB		R24, R25, R4	| MOV		#7, R21
	SUB		R24, R25, R4	| MOV		#8, R21
	SHAD	R4, #-3, R5		| MOV		R25, R6

#if 1
	.L2:
	ADD		#-1, R21
	AND		R6, R1, R2		| MOVU.B	(R19, R21), R7
	OR		R2, R7, R3
	ADD		R5, R6			| MOVU.W	(R18, R3), R2
	AND		R6, R1, R3		| MOV.W		R2, (R20, R21)
	ADD		#-1, R21
	OR		R3, R7, R3		| MOVU.B	(R19, R21), R7
	ADD		R5, R6			| MOVU.W	(R18, R3), R2
	MOV.W	R2, (R20, R21)
	CMP/GT	#0, R21
	BT		.L2
#endif

#if 0
	.L2:
	MOVU.B	(R19, R21), R7
	MOV		#0xFF00, R1
	AND		R6, R1, R2
	OR		R2, R7, R3
	MOVU.W	(R18, R3), R2
	MOV.W	R2, (R20, R21)
	
	ADD		R5, R6
	ADD		#-1, R21
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

	POP.X	R8
	POP.X	R10
	POP.X	R24
	POP.X	R26
	RTSU

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
	RTSU

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
	ADD		R16, R20		| MULS.W	R5, R23, R6
	ADD		R17, R21		| ADD		R4, R6, R7
	SHAD	R20, #-16, R4	| MOVU.W	(R18, R7), R2
	SHAD	R21, #-16, R5	| MOV.W		R2, (R19, 0)
	ADD		R16, R20		| MULS.W	R5, R23, R6
	ADD		#-2, R22		| ADD		R4, R6, R7
	ADD		R17, R21		| MOVU.W	(R18, R7), R2
	MOV.W	R2, (R19, 2)
	ADD		#4, R19			| CMP/GE	#2, R22
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

	RTSU

#endif

#if 1
.global X(D_DrawSpans16_InnerPx2)

X(D_DrawSpans16_InnerPx2):

	CMP/GE	#2, R6
	BF .L1
	.L0:
	SHAD	R20, #-16, R2	| SHAD		R21, #-16, R3
	ADD		R22, R20		| MULS.W	R3, R7, R18
	ADD		R23, R21		| ADD		R2, R18, R19
	SHAD	R20, #-16, R2	| MOVU.W	(R4, R19), R16
	SHAD	R21, #-16, R3	| MOV.W		R16, (R5, 0)
	ADD		R22, R20		| MULS.W	R3, R7, R18
	ADD		#-2, R6			| ADD		R2, R18, R19
	ADD		R23, R21		| MOVU.W	(R4, R19), R16
	MOV.W	R16, (R5, 2)
	ADD		#4, R5			| CMP/GE	#2, R6
	BT .L0
	.L1:
	
	TEST R6, R6
	BT .L2

	SHAD	R20, #-16, R2	| SHAD	R21, #-16, R3
	MULS.W	R3, R7, R18
	ADD		R2, R18, R19
	MOVU.W	(R4, R19), R16
	MOV.W	R16, (R5)
	ADD		#2, R5

	.L2:
	RTSU

#endif


#if 1
.global X(D_DrawZSpans_Inner)
X(D_DrawZSpans_Inner):

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
	RTSU

#endif
