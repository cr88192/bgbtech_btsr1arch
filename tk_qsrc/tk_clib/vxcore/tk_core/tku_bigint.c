void	tkmm_bigint_add(void *lva, void *lvb, void *lvc);
void	tkmm_bigint_sub(void *lva, void *lvb, void *lvc);

void	tkmm_bigint_and(void *lva, void *lvb, void *lvc);
void	tkmm_bigint_or (void *lva, void *lvb, void *lvc);
void	tkmm_bigint_xor(void *lva, void *lvb, void *lvc);

#ifdef __BJX2__

__asm {
tkmm_bigint_add:
	MOV.X	(R4,  0), R16
	MOV.X	(R4, 16), R18

	MOV.X	(R5,  0), R20
	MOV.X	(R5, 16), R22

//	MOV.X	(R4, 32), R20
	CLRT
	ADDC	R20, R16
	ADDC	R21, R17
	ADDC	R22, R18
	ADDC	R23, R19

	MOV.X	R16, (R6,  0)

	MOV.X	(R4, 32), R16
	MOV.X	(R5, 32), R20

	MOV.X	R18, (R6, 16)

	ADDC	R20, R16
	ADDC	R21, R17

	MOV.X	R16, (R6, 32)

	RTS

tkmm_bigint_sub:
	MOV.X	(R4,  0), R16
	MOV.X	(R4, 16), R18

	MOV.X	(R5,  0), R20
	MOV.X	(R5, 16), R22

//	MOV.X	(R4, 32), R20
	CLRT
	SUBC	R20, R16
	SUBC	R21, R17
	SUBC	R22, R18
	SUBC	R23, R19

	MOV.X	R16, (R6,  0)

	MOV.X	(R4, 32), R16
	MOV.X	(R5, 32), R20

	MOV.X	R18, (R6, 16)

	SUBC	R20, R16
	SUBC	R21, R17

	MOV.X	R16, (R6, 32)

	RTS

tkmm_bigint_and:
	MOV.X	(R4,  0), R16
	MOV.X	(R5,  0), R20

	MOV.X	(R4, 16), R18
	MOV.X	(R5, 16), R22
	
	AND		R20, R16
	AND		R21, R17
	MOV.X	R16, (R6,  0)

	MOV.X	(R4, 32), R16
	MOV.X	(R5, 32), R20
	
	AND		R22, R18
	AND		R23, R19
	MOV.X	R18, (R6,  0)

	AND		R20, R16
	AND		R21, R17
	MOV.X	R16, (R6, 32)
	
	RTS

tkmm_bigint_or:
	MOV.X	(R4,  0), R16
	MOV.X	(R5,  0), R20

	MOV.X	(R4, 16), R18
	MOV.X	(R5, 16), R22
	
	OR		R20, R16
	OR		R21, R17
	MOV.X	R16, (R6,  0)

	MOV.X	(R4, 32), R16
	MOV.X	(R5, 32), R20
	
	OR		R22, R18
	OR		R23, R19
	MOV.X	R18, (R6,  0)

	OR		R20, R16
	OR		R21, R17
	MOV.X	R16, (R6, 32)
	
	RTS

tkmm_bigint_xor:
	MOV.X	(R4,  0), R16
	MOV.X	(R5,  0), R20

	MOV.X	(R4, 16), R18
	MOV.X	(R5, 16), R22
	
	XOR		R20, R16
	XOR		R21, R17
	MOV.X	R16, (R6,  0)

	MOV.X	(R4, 32), R16
	MOV.X	(R5, 32), R20
	
	XOR		R22, R18
	XOR		R23, R19
	MOV.X	R18, (R6,  0)

	XOR		R20, R16
	XOR		R21, R17
	MOV.X	R16, (R6, 32)
	
	RTS

};

#endif

TK_BIGINT tk_bigint_add(TK_BIGINT va, TK_BIGINT vb)
{
	TK_BIGINT vc;
	tkmm_bigint_add(&va, &vb, &vc);
	return(vc);
}

TK_BIGINT tk_bigint_sub(TK_BIGINT va, TK_BIGINT vb)
{
	TK_BIGINT vc;
	tkmm_bigint_sub(&va, &vb, &vc);
	return(vc);
}

TK_BIGINT tk_bigint_and(TK_BIGINT va, TK_BIGINT vb)
{
	TK_BIGINT vc;
	tkmm_bigint_and(&va, &vb, &vc);
	return(vc);
}

TK_BIGINT tk_bigint_or(TK_BIGINT va, TK_BIGINT vb)
{
	TK_BIGINT vc;
	tkmm_bigint_or(&va, &vb, &vc);
	return(vc);
}

TK_BIGINT tk_bigint_xor(TK_BIGINT va, TK_BIGINT vb)
{
	TK_BIGINT vc;
	tkmm_bigint_xor(&va, &vb, &vc);
	return(vc);
}
