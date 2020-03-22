mat4f TK_M4F_Add(mat4f A, mat4f B);
mat4f TK_M4F_Sub(mat4f A, mat4f B);
mat4f TK_M4F_Trans(mat4f A);
mat4f TK_M4F_Mult(mat4f A, mat4f B);


__asm {
TK_M4F_Add:		//mat4f TK_M4F_Add(mat4f A, mat4f B);
	MOV.X		(R4,  0), R16
	MOV.X		(R5,  0), R18
	PADDX.F		R16, R18, R6
	MOV.X		R6, (R2,  0)
	MOV.X		(R4, 16), R16
	MOV.X		(R5, 16), R18
	PADDX.F		R16, R18, R6
	MOV.X		R6, (R2, 16)
	MOV.X		(R4, 32), R16
	MOV.X		(R5, 32), R18
	PADDX.F		R16, R18, R6
	MOV.X		R6, (R2, 32)
	MOV.X		(R4, 48), R16
	MOV.X		(R5, 48), R18
	PADDX.F		R16, R18, R6
	MOV.X		R6, (R2, 48)
	RTSU

TK_M4F_Sub:		//mat4f TK_M4F_Sub(mat4f A, mat4f B);
	MOV.X		(R4,  0), R16
	MOV.X		(R5,  0), R18
	PSUBX.F		R16, R18, R6
	MOV.X		R6, (R2,  0)
	MOV.X		(R4, 16), R16
	MOV.X		(R5, 16), R18
	PSUBX.F		R16, R18, R6
	MOV.X		R6, (R2, 16)
	MOV.X		(R4, 32), R16
	MOV.X		(R5, 32), R18
	PSUBX.F		R16, R18, R6
	MOV.X		R6, (R2, 32)
	MOV.X		(R4, 48), R16
	MOV.X		(R5, 48), R18
	PSUBX.F		R16, R18, R6
	MOV.X		R6, (R2, 48)
	RTSU


TK_M4F_Trans:	//mat4f TK_M4F_Trans(mat4f A);
	//Load
	MOV.X		(R4,  0), R16
	MOV.X		(R4, 16), R18
	MOV.X		(R4, 32), R20
	MOV.X		(R4, 48), R22
	//Transpose
	MOVLD		R18, R16, R6	|	MOVLD		R22, R20, R7
	MOV.X		R6, (R2,  0)
	MOVHD		R18, R16, R6	|	MOVHD		R22, R20, R7
	MOV.X		R6, (R2, 16)
	MOVLD		R19, R17, R6	|	MOVLD		R23, R21, R7
	MOV.X		R6, (R2, 32)
	MOVHD		R19, R17, R6	|	MOVHD		R23, R21, R7
	MOV.X		R6, (R2, 48)
	RTSU

TK_M4F_Mult:	//mat4f TK_M4F_Mult(mat4f A, mat4f B);
	PUSH.X		R30
	PUSH.X		R28
	PUSH.X		R26
	PUSH.X		R24
	PUSH		R14
	PUSH.X		R12
	PUSH.X		R10
	PUSH.X		R8

	//R4 = *A
	//R5 = *B
	//R6 = *C
	//R8..R13, R2/3: T
	//R16..R23: T
	//R24..R31: B^T

	//Load B (~ 12c)
	MOV.X		(R5,  0), R16
	MOV.X		(R5, 16), R18
	MOV.X		(R5, 32), R20
	MOV.X		(R5, 48), R22
	//Transpose B (4c)
	MOVLD		R18, R16, R24	|	MOVLD		R22, R20, R25
	MOVHD		R18, R16, R26	|	MOVHD		R22, R20, R27
	MOVLD		R19, R17, R28	|	MOVLD		R23, R21, R29
	MOVHD		R19, R17, R30	|	MOVHD		R23, R21, R31

	MOV			6, R3

	.L0:
	//Load/Multiply Row (~ 43c)
	MOV.X		(R4, R3), R2
	PMULX.F		R2, R24, R16
	PMULX.F		R2, R26, R18
	PMULX.F		R2, R28, R20
	PMULX.F		R2, R30, R22
	//Transpose Again (4c)
	MOVLD		R18, R16, R8	|	MOVLD		R22, R20, R9
	MOVHD		R18, R16, R10	|	MOVHD		R22, R20, R11
	MOVLD		R19, R17, R12	|	MOVLD		R23, R21, R13
	MOVHD		R19, R17, R6	|	MOVHD		R23, R21, R7
	//Add Rows, Store Result (~ 34c)
	PADDX.F		R8, R10, R10
	PADDX.F		R6, R12, R12
	PADDX.F		R10, R12, R6
	MOV.X		R6, (R2, R3)
	
	CMP/GT		0, R3
	ADD			-2, R3
	BT			.L0

	//Epilog
	POP.X		R8
	POP.X		R10
	POP.X		R12
	POP			R14
	POP.X		R24
	POP.X		R26
	POP.X		R28
	POP.X		R30
	RTSU 
};
