void	tkmm_bigint_add(void *lva, void *lvb, void *lvc);
void	tkmm_bigint_sub(void *lva, void *lvb, void *lvc);

void	tkmm_bigint_and(void *lva, void *lvb, void *lvc);
void	tkmm_bigint_or (void *lva, void *lvb, void *lvc);
void	tkmm_bigint_xor(void *lva, void *lvb, void *lvc);

void *__alloca(int size);

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

#ifndef __BJX2__

void	tkmm_bigint_add(void *lva, void *lvb, void *lvc)
{
}

void	tkmm_bigint_sub(void *lva, void *lvb, void *lvc)
{
}

void	tkmm_bigint_and(void *lva, void *lvb, void *lvc)
{
}

void	tkmm_bigint_or (void *lva, void *lvb, void *lvc)
{
}

void	tkmm_bigint_xor(void *lva, void *lvb, void *lvc)
{
}

#endif

#if 1
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
#endif


#ifdef __BJX2__

void __xbi_add(void *va, void *vb, void *vc, int n);
void __xbi_sub(void *va, void *vb, void *vc, int n);
void __xbi_and(void *va, void *vb, void *vc, int n);
void __xbi_or(void *va, void *vb, void *vc, int n);
void __xbi_xor(void *va, void *vb, void *vc, int n);

__asm {
__xbi_add:
	MOV		0, R2
	.L0:
	CMPEQ	1, R2
	MOV.X	(R4, 0), R16
	MOV.X	(R5, 0), R18
	ADD		16, R4
	ADD		16, R5
	ADDC	R18, R16
	ADDC	R19, R17
	ADD		-1, R7
	MOVT	R2
	MOV.X	R16, (R6, 0)
	ADD		16, R6
	CMPEQ	0, R7
	BF		.L0
	RTS

__xbi_sub:
	MOV		0, R2
	.L0:
	CMPEQ	1, R2
	MOV.X	(R4, 0), R16
	MOV.X	(R5, 0), R18
	ADD		16, R4
	ADD		16, R5
	SUBC	R18, R16
	SUBC	R19, R17
	ADD		-1, R7
	MOVT	R2
	MOV.X	R16, (R6, 0)
	ADD		16, R6
	CMPEQ	0, R7
	BF		.L0
	RTS

__xbi_and:
	.L0:
	MOV.X	(R4, 0), R16
	MOV.X	(R5, 0), R18
	ADD		16, R4
	ADD		16, R5
	AND		R18, R16
	AND		R19, R17
	ADD		-1, R7
	MOV.X	R16, (R6, 0)
	ADD		16, R6
	CMPEQ	0, R7
	BF		.L0
	RTS

__xbi_or:
	.L0:
	MOV.X	(R4, 0), R16
	MOV.X	(R5, 0), R18
	ADD		16, R4
	ADD		16, R5
	OR		R18, R16
	OR		R19, R17
	ADD		-1, R7
	MOV.X	R16, (R6, 0)
	ADD		16, R6
	CMPEQ	0, R7
	BF		.L0
	RTS

__xbi_xor:
	.L0:
	MOV.X	(R4, 0), R16
	MOV.X	(R5, 0), R18
	ADD		16, R4
	ADD		16, R5
	XOR		R18, R16
	XOR		R19, R17
	ADD		-1, R7
	MOV.X	R16, (R6, 0)
	ADD		16, R6
	CMPEQ	0, R7
	BF		.L0
	RTS
};

void __xbi_shl(void *va, void *vb, void *vc, int n);
void __xbi_shll(void *va, void *vb, void *vc, int n);
void __xbi_shlr(void *va, void *vb, void *vc, int n);
void __xbi_shar(void *va, void *vb, void *vc, int n);

__asm {
__xbi_shl:
__xbi_shll:
	MOVU.W	(R5), R3
	ADD		R7, R7
	MOV		0, R1
	.L0:
	CMPGT	63, R3
	BF		.L1
	MOV.Q	R1, (R6)
	ADD		8, R6
	ADD		-64, R3
	ADD		-1, R7
	CMPEQ	0, R7
	BT		.Lend
	BRA		.L0

	.L1:
	MOV.X	(R4), R16
	SHLD.X	R16, R3, R18
	MOV.Q	R18, (R6)

	.L2:
	MOV.X	(R4), R16
	ADD		8, R4
	SHLD.X	R16, R3, R18
	ADD		-1, R7
	MOV.Q	R19, (R6, 8)
	ADD		8, R6
	CMPEQ	0, R7
	BF		.L2

	.Lend:
	RTS

__xbi_shlr:
	MOVU.W	(R5), R3
	ADD		R7, R7
	MOV		0, R1
	.L0:
	CMPGT	63, R3
	BF		.L1
	ADD		8, R4
	ADD		-64, R3
	ADD		-1, R7
	CMPEQ	0, R7
	BT		.Lend
	BRA		.L0

	.L1:
//	MOV.X	(R4), R16
//	SHLR.X	R16, R3, R18
//	MOV.Q	R18, (R6)

	.L2:
	MOV.X	(R4), R16
	ADD		8, R4
	SHLR.X	R16, R3, R18
	ADD		-1, R7
	MOV.Q	R18, (R6, 8)
	ADD		8, R6
	CMPEQ	0, R7
	BF		.L2

	.Lend:
	RTS


__xbi_shar:
	MOVU.W	(R5), R3
	ADD		R7, R7
	MOV		0, R1
	.L0:
	CMPGT	63, R3
	BF		.L1
	ADD		8, R4
	ADD		-64, R3
	ADD		-1, R7
	CMPEQ	0, R7
	BT		.Lend
	BRA		.L0

	.L1:
//	MOV.X	(R4), R16
//	SHLR.X	R16, R3, R18
//	MOV.Q	R18, (R6)

	.L2:
	MOV.X	(R4), R16
	ADD		8, R4
	SHAR.X	R16, R3, R18
	ADD		-1, R7
	MOV.Q	R18, (R6, 8)
	ADD		8, R6
	CMPEQ	0, R7
	BF		.L2

	.Lend:
	RTS
};


void __xbi_neg(void *va, void *vc, int n);
void __xbi_not(void *va, void *vc, int n);
int __xbi_isneg(void *va, int n);

__asm {
__xbi_neg:
	MOV		0, R1
	MOV		1, R2
	.L0:
	CMPEQ	1, R2
	MOV.X	(R4), R16
	ADD		16, R4
	NOT		R16, R16
	NOT		R17, R17
	ADDC	R1, R16
	ADDC	R1, R17
	ADD		-1, R6
	MOV.X	R16, (R5)
	ADD		16, R5
	MOVT	R2
	CMPEQ	0, R6
	BF		.L0
	
	RTS

__xbi_not:
	.L0:
	MOV.X	(R4), R16
	ADD		16, R4
	NOT		R16, R16
	NOT		R17, R17
	ADD		-1, R6
	MOV.X	R16, (R5)
	ADD		16, R5
	CMPEQ	0, R6
	BF		.L0
	RTS

__xbi_isneg:
	ADD		R5, R5, R7
	ADD		-1, R7
	MOV.Q	(R4, R7), R3
	SHLD.Q	R3, -63, R2
	RTS
};


void __xbi_cmp_eq(void *va, void *vb, int n);
void __xbi_cmp_ne(void *va, void *vb, int n);
void __xbi_cmp_gt(void *va, void *vb, int n);
void __xbi_cmp_ge(void *va, void *vb, int n);
void __xbi_cmp_hi(void *va, void *vb, int n);
void __xbi_cmp_he(void *va, void *vb, int n);
int __xbi_iszero(void *va, int n);

__asm {
__xbi_cmp_eq:
	MOV		R6, R7
	.L0:
	MOV.X	(R4, 0), R16
	MOV.X	(R5, 0), R18
	ADD		16, R4
	ADD		16, R5
	ADD		-1, R7
	CMPQEQ	R18, R16
	BF		.Lfalse
	CMPQEQ	R19, R17
	BF		.Lfalse
	CMPEQ	0, R7
	BF		.L0
	.Ltrue:
	MOV		1, R2
	RTS
	.Lfalse:
	MOV		0, R2
	RTS

__xbi_cmp_ne:
	MOV		R6, R7
	.L0:
	MOV.X	(R4, 0), R16
	MOV.X	(R5, 0), R18
	ADD		16, R4
	ADD		16, R5
	ADD		-1, R7
	CMPQEQ	R18, R16
	BF		.Ltrue
	CMPQEQ	R19, R17
	BF		.Ltrue
	CMPEQ	0, R7
	BF		.L0
	.Lfalse:
	MOV		0, R2
	RTS
	.Ltrue:
	MOV		1, R2
	RTS

__xbi_cmp_gt:
	SHLD	R6, 1, R3
	ADD		-2, R3
	LEA.Q	(R4, R3), R4
	LEA.Q	(R5, R3), R5
	MOV		R6, R7
	.L0:
	MOV.X	(R4, 0), R16
	MOV.X	(R5, 0), R18
	ADD		-16, R4
	ADD		-16, R5
	ADD		-1, R7
	CMPQEQ	R18, R16
	BF		.Lne1
	CMPQEQ	R19, R17
	BF		.Lne2
	CMPEQ	0, R7
	BF		.L0
	MOV		0, R2
	RTS	
	.Lne1:
	CMPQGT	R18, R16
	MOVT	R2
	RTS
	.Lne2:
	CMPQGT	R19, R17
	MOVT	R2
	RTS

__xbi_cmp_ge:
	SHLD	R6, 1, R3
	ADD		-2, R3
	LEA.Q	(R4, R3), R4
	LEA.Q	(R5, R3), R5
	MOV		R6, R7
	.L0:
	MOV.X	(R4, 0), R16
	MOV.X	(R5, 0), R18
	ADD		-16, R4
	ADD		-16, R5
	ADD		-1, R7
	CMPQEQ	R18, R16
	BF		.Lne1
	CMPQEQ	R19, R17
	BF		.Lne2
	CMPEQ	0, R7
	BF		.L0
	MOV		1, R2
	RTS	
	.Lne1:
	CMPQGT	R18, R16
	MOVT	R2
	RTS
	.Lne2:
	CMPQGT	R19, R17
	MOVT	R2
	RTS

__xbi_cmp_hi:
	SHLD	R6, 1, R3
	ADD		-2, R3
	LEA.Q	(R4, R3), R4
	LEA.Q	(R5, R3), R5
	MOV		R6, R7
	.L0:
	MOV.X	(R4, 0), R16
	MOV.X	(R5, 0), R18
	ADD		-16, R4
	ADD		-16, R5
	ADD		-1, R7
	CMPQEQ	R18, R16
	BF		.Lne1
	CMPQEQ	R19, R17
	BF		.Lne2
	CMPEQ	0, R7
	BF		.L0
	MOV		0, R2
	RTS	
	.Lne1:
	CMPQHI	R18, R16
	MOVT	R2
	RTS
	.Lne2:
	CMPQHI	R19, R17
	MOVT	R2
	RTS

__xbi_cmp_he:
	SHLD	R6, 1, R3
	ADD		-2, R3
	LEA.Q	(R4, R3), R4
	LEA.Q	(R5, R3), R5
	MOV		R6, R7
	.L0:
	MOV.X	(R4, 0), R16
	MOV.X	(R5, 0), R18
	ADD		-16, R4
	ADD		-16, R5
	ADD		-1, R7
	CMPQEQ	R18, R16
	BF		.Lne1
	CMPQEQ	R19, R17
	BF		.Lne2
	CMPEQ	0, R7
	BF		.L0
	MOV		1, R2
	RTS	
	.Lne1:
	CMPQHI	R16, R18
	MOVNT	R2
	RTS
	.Lne2:
	CMPQHI	R17, R19
	MOVNT	R2
	RTS

__xbi_iszero:
	.L0:
	MOV.X	(R4), R16
	ADD		16, R4
	ADD		-1, R5
	CMPQEQ	0, R16
	BF		.Lfalse
	CMPQEQ	0, R17
	BF		.Lfalse
	CMPEQ	0, R5
	BF		.L0
	.Ltrue:
	MOV		1, R2
	RTS
	.Lfalse:
	MOV		0, R2
	RTS
};


u128 __umuldq(u64 va, u64 vb);

void __xbi_umul(void *va, void *vb, void *vc, int n)
{
	u128 xi0, xi1, xi2;
	u64 i0, i1, i2, i3;
	int i, j, k, n1;

	memset(vc, 0, n*16);

	n1=n+n;
	for(i=0; i<n1; i++)
		for(j=0; j<n1; j++)
	{
		k=i+j;
		
		i0=((u64 *)va)[i];
		i1=((u64 *)vb)[j];
		xi0=*(u128 *)(((u64 *)vc)+k);
		xi1=__umuldq(i0, i1);
		xi2=xi0+xi1;
		if((k+3)<n1)
		{
			*(u128 *)(((u64 *)vc)+k)=xi2;
			if(xi2<(xi0|xi1))
			{
				/* Carry */
				*(u128 *)(((u64 *)vc)+(k+2)) =
					*(u128 *)(((u64 *)vc)+(k+2)) + 1;
			}
		}else
			if((k+1)<n1)
		{
			*(u128 *)(((u64 *)vc)+k)=xi2;
		}
	}
}

void __xbi_smul(void *va, void *vb, void *vc, int n)
{
	__xbi_umul(va, vb, vc, n);
}

extern int _fcn_clz64(u64 v);

int __xbi_clz(void *va, int n)
{
	u64 *csa;
	u64 v;
	int n1, c;

	n1=n*2-1; c=0;
	csa=((u64 *)va)+n1;
	while(n1--)
	{
		v=*csa--;
		if(!v)
			{ c+=64; continue; }
		c+=_fcn_clz64(v);
		break;
	}
	return(c);
}

void __xbi_udiv(void *vn, void *vd, void *vc, int n)
{
	u64		ttvbuf[4*(4*2)];
	void *vq, *vr, *vt0, *vc_1;
	int s, c, t0, t1;
//	int sr;
	byte sr;
	
	if(n>4)
	{
		vq  =__alloca(n*16);
		vr  =__alloca(n*16);
		vt0 =__alloca(n*16);
		vc_1=__alloca(n*16);
	}else
	{
		vq  =ttvbuf+ 0;
		vr  =ttvbuf+ 8;
		vt0 =ttvbuf+16;
		vc_1=ttvbuf+24;
	}
	
	memset(vc_1, 0, n*16);
	*(u64 *)vc_1=1;
	
	if(__xbi_iszero(vd, n) || __xbi_iszero(vn, n))
	{
		memset(vc, 0, n*16);
		return;
	}

	sr=__xbi_clz(vd, n)-__xbi_clz(vn, n);

	if(sr>=(n*128-1))
	{
		if(sr==(n*128-1))
		{
			memcpy(vc, vn, n*16);
			return;
		}
		memset(vc, 0, n*16);
		return;
	}

	sr++;
	c=0;
//	q=n<<(128-sr); r=n>>sr; c=0;
	t0=(n*128)-sr;
	t1=sr;
	__xbi_shll(vn, &t0, vq, n);
	__xbi_shlr(vn, &t1, vr, n);
	while(sr--)
	{
//		r=(r<<1)|(q>>127);

		t0=1;
		t1=n*128-1;
//		__xbi_shll(vr, vc_1, vr, n);
		__xbi_add (vr, vr, vr, n);
		__xbi_shlr(vq, &t1, vt0, n);
		__xbi_add(vr, vt0, vr, n);

//		q=(q<<1)|c;
//		__xbi_shll(vq, vc_1, vq, n);
		__xbi_add (vq, vq, vq, n);
		*(u64 *)vq=(*(u64 *)vq)|c;

//		s=((__int128)(d-r-1))>>127;
		__xbi_sub(vd, vr, vt0, n);
		__xbi_sub(vt0, vc_1, vt0, n);
		
		if(__xbi_isneg(vt0, n))
			{ __xbi_sub(vr, vd, vr, n); c=1; }
		else
			{ c=0; }
		
//		c=s&1;
//		r-=d&s;
	}

	__xbi_shll(vq, vc_1, vq, n);
	*(u64 *)vq=(*(u64 *)vq)|c;
	memcpy(vc, vq, n*16);

//	q=(q<<1)|c;
//	return(q);
}

void __xbi_sdiv(void *va, void *vb, void *vc, int n)
{
	u64		ttvbuf[3*(4*2)];
	void *tvan, *tvbn, *tvcn;
	int sg;
	
	tvan=va;
	tvbn=vb;
	tvcn=vc;
	sg=0;

	if(n<=4)
	{
		if(__xbi_isneg(va))
		{
			tvan=ttvbuf+0;
			__xbi_neg(va, tvan, n);
			sg=!sg;
		}

		if(__xbi_isneg(vb))
		{
			tvbn=ttvbuf+8;
			__xbi_neg(vb, tvbn, n);
			sg=!sg;
		}
		
		if(sg)
			{ tvcn=ttvbuf+16; }
	}else
	{
		if(__xbi_isneg(va))
		{
			tvan=__alloca(n*16);
			__xbi_neg(va, tvan, n);
			sg=!sg;
		}

		if(__xbi_isneg(vb))
		{
			tvbn=__alloca(n*16);
			__xbi_neg(vb, tvbn, n);
			sg=!sg;
		}
		
		if(sg)
			{ tvcn=__alloca(n*16); }
	}
	__xbi_udiv(tvan, tvbn, tvcn, n);
	if(sg)
		{ __xbi_neg(tvcn, vc, n); }
}

void __xbi_umod(void *va, void *vb, void *vc, int n)
{
	void *vq, *vr, *vt;
	vq=__alloca(n*16);
	vr=__alloca(n*16);
	vt=__alloca(n*16);
	__xbi_udiv(va, vb, vq, n);
	__xbi_umul(vq, vb, vt, n);
	__xbi_sub (va, vt, vc, n);
}

void __xbi_smod(void *va, void *vb, void *vc, int n)
{
	void *vq, *vr, *vt;
	vq=__alloca(n*16);
	vr=__alloca(n*16);
	vt=__alloca(n*16);
	__xbi_sdiv(va, vb, vq, n);
	__xbi_smul(vq, vb, vt, n);
	__xbi_sub (va, vt, vc, n);
}

#endif

