.global	__sfp_fmul_f32

/*
 * R4: Arg0
 * R5: Arg1
 * R6: Sg
 
 * R18: e0
 * R19: e1
 * R20: e2
 * R21: m2
 * R22: m0
 * R23: m1
 */
__sfp_fmul_f32:
	XOR R4, R5, R6
	AND #0x80000000, R6
	TST R4, R4
	BT .rtz
	TST R5, R5
	BT .rtz

//	e0=(f0>>23)&255;
//	e1=(f1>>23)&255;
//	e2=(e0+e1)-127;
//	m0=0x00800000|(f0&0x00FFFFFF);
//	m1=0x00800000|(f1&0x00FFFFFF);

	SHLD	R4, #-23, R18
	SHLD	R5, #-23, R19
	EXTU.B	R18
	EXTU.B	R19
	ADD		R18, R19, R20
	ADD		#-127, R20

//	m0=0x00800000|(f0&0x00FFFFFF);
//	m1=0x00800000|(f1&0x00FFFFFF);
//	m2=__int32_dmulu(m0, m1)>>18;

	AND		R4, #0x00FFFFFF, R22
	AND		R5, #0x00FFFFFF, R23
	OR		#0x00800000, R22
	OR		#0x00800000, R23
	DMULU	R22, R23
	SHLD.Q	R1, #32, R21
	ADD		R0, R21
	SHLD.Q	R21, #-18, R21

//	m2=(m2+15)>>5;

	ADD		#15, R21
	SHAD	#-5, R21

//	if((m2&0xFF000000))
//	{
//		m2=m2>>1;
//		e2++;
//	}

	TST		#0xFF000000, R21
	SHLD?F	R21, #-1, R21
	ADD?F	#1, R20

//	if(e2>=255)
//	{
//		e2=255;
//		m2=0;
//	}else if(e2<=0)
//	{
//		return(0);
//	}

	CMPGT	#254, R20
	MOV?T	#255, R20
	MOV?T	#0, R21

	CMPGT	#0, R20
	MOV?F	#0, R20
	MOV?F	#0, R21
	MOV?F	#0, R6

//	f2=sg|(e2<<23)|(m2&0x007FFFFF);

	AND		R21, #0x007FFFFF, R2
	SHAD	R20, #23, R7
	OR		R6, R7, R3
	OR		R3, R2

//	BREAK

	RTS

.rtz:
	MOV #0, R2
	RTS
