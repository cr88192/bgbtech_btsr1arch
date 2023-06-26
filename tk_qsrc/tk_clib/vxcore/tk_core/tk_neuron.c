/*
 * Basic SIMD Neurons
 *
 * void TK_NN_ProcessVector4w(
 *	u64 *vvec, u64 *wvec,
 *	u64 *dvec,
 *	int n);
 *
 * vvec: Values (4x Binary16)
 * wvec: Weights
 * n: Multiple of 4 inputs, or 16 weights.
 * After n*16 weights, will be a 4 element bias vector.
 * The low 2 bits of the bias vector will give the activation function.
 * * 00: None
 * * 01: ReLU
 * * 10: SSQRT
 * * 11: USQRT
 *
 * The 8w case will be similar to the 4w case.
 * But, will operate in groups of 8 inputs and 32 weights.
 * The bias vector will operate on 2 groups of 4 elements.
 */

#ifdef __BJX2__

void TK_NN_ProcessVector4w(
	u64 *vvec, u64 *wvec,
	u64 *dvec,
	int n);

void TK_NN_ProcessVector8w(
	u64 *vvec, u64 *wvec,
	u64 *dvec,
	int n);

__asm {
TK_NN_ProcessVector4w:
	ADD			-128, SP
	MOV.X		R30, (SP, 112)
	MOV.X		R28, (SP,  96)
	MOV.X		R26, (SP,  80)
	MOV.X		R24, (SP,  64)

	MOV.X		R12, (SP,  32)
	MOV.X		R10, (SP,  16)
	MOV.X		R8 , (SP,   0)

	MOV			0, R2
	MOV			R7, R3

	.L0:
	MOV.Q		(R4,  0), R7
	MOV.X		(R5,  0), R16
	MOV.X		(R5, 16), R18
	PSHUF.W		R7, 0x00, R20
	PSHUF.W		R7, 0x55, R21
	PMUL.H		R16, R20, R24
	PMUL.H		R17, R21, R25
	PSHUF.W		R7, 0xAA, R22
	PADD.H		R24, R2
	PMUL.H		R18, R22, R26
	PSHUF.W		R7, 0xFF, R23
	PADD.H		R25, R2
	ADD			32, R5
	PMUL.H		R19, R23, R27
	PADD.H		R26, R2
	ADD			8, R4
	ADD			-1, R3
	PADD.H		R27, R2
	CMPGT		0, R3
	BT			.L0

	MOV.Q		(R5,  0), R7
	PADD.H		R2, R7, R2
	AND			R7, 3, R3
	
	CMPEQ		1, R3
	PRELU.H?T	R2, R2
	CMPEQ		2, R3
	PSQRTA.H?T	R2, R2
	CMPEQ		3, R3
	PSQRTUA.H?T	R2, R2
	
	MOV.Q		R2, (R6)

	MOV.X		(SP, 112), R30
	MOV.X		(SP,  96), R28
	MOV.X		(SP,  80), R26
	MOV.X		(SP,  64), R24

	MOV.X		(SP,  32), R12
	MOV.X		(SP,  16), R10
	MOV.X		(SP,   0), R8
	ADD			128, SP

	RTS


TK_NN_ProcessVector8w:
	ADD			-128, SP
	MOV.X		R30, (SP, 112)
	MOV.X		R28, (SP,  96)
	MOV.X		R26, (SP,  80)
	MOV.X		R24, (SP,  64)

	MOV.Q		R14, (SP,  48)
	MOV.X		R12, (SP,  32)
	MOV.X		R10, (SP,  16)
	MOV.X		R8 , (SP,   0)

	MOV			0, R2
	MOV			0, R3
	MOV			R7, R14

	.L0:
	MOV.Q		(R4,  0), R7

	MOV.X		(R5,  0), R16
	MOV.X		(R5, 16), R18
	PSHUF.W		R7, 0x00, R20
	PSHUF.W		R7, 0x55, R21
	PMUL.H		R16, R20, R24
	PMUL.H		R17, R21, R25
	PSHUF.W		R7, 0xAA, R22
	PADD.H		R24, R2
	PMUL.H		R18, R22, R26
	PSHUF.W		R7, 0xFF, R23
	PADD.H		R25, R2
	PMUL.H		R19, R23, R27

	MOV.X		(R5, 32), R16
	PADD.H		R26, R2

	MOV.X		(R5, 48), R18
	PSHUF.W		R7, 0x00, R20
	PSHUF.W		R7, 0x55, R21
	PADD.H		R27, R2

	PMUL.H		R16, R20, R24
	PMUL.H		R17, R21, R25
	PSHUF.W		R7, 0xAA, R22

	PADD.H		R24, R3
	PMUL.H		R18, R22, R26
	PSHUF.W		R7, 0xFF, R23
	PADD.H		R25, R3
	PMUL.H		R19, R23, R27
	PADD.H		R26, R3

	ADD			-1, R14
	ADD			64, R5
	PADD.H		R27, R3
	ADD			8, R4

	CMPGT		0, R14
	BT			.L0

	MOV.X		(R5,  0), R16

	PADD.H		R2, R16, R2
	PADD.H		R3, R17, R3

	AND			R16, 3, R18
	AND			R17, 3, R19
	
	CMPEQ		1, R18
	PRELU.H?T	R2, R2
	CMPEQ		2, R18
	PSQRTA.H?T	R2, R2
	CMPEQ		3, R18
	PSQRTUA.H?T	R2, R2

	CMPEQ		1, R19
	PRELU.H?T	R3, R3
	CMPEQ		2, R19
	PSQRTA.H?T	R3, R3
	CMPEQ		3, R19
	PSQRTUA.H?T	R3, R3
	
	MOV.X		R2, (R6)

	MOV.X		(SP, 112), R30
	MOV.X		(SP,  96), R28
	MOV.X		(SP,  80), R26
	MOV.X		(SP,  64), R24

	MOV.Q		(SP,  48), R14
	MOV.X		(SP,  32), R12
	MOV.X		(SP,  16), R10
	MOV.X		(SP,   0), R8
	ADD			128, SP

	RTS
};

#else

u64 tk_nn_shuf4_0(u64 v0)
{
	u64 v1;
	v1=(u16)(v0>> 0);
	v1=v1|(v1<<16);
	v1=v1|(v1<<32);
	return(v1);
}

u64 tk_nn_shuf4_1(u64 v0)
{
	u64 v1;
	v1=(u16)(v0>>16);
	v1=v1|(v1<<16);
	v1=v1|(v1<<32);
	return(v1);
}

u64 tk_nn_shuf4_2(u64 v0)
{
	u64 v1;
	v1=(u16)(v0>>32);
	v1=v1|(v1<<16);
	v1=v1|(v1<<32);
	return(v1);
}

u64 tk_nn_shuf4_3(u64 v0)
{
	u64 v1;
	v1=(u16)(v0>>48);
	v1=v1|(v1<<16);
	v1=v1|(v1<<32);
	return(v1);
}

u16 tk_nn_fmulh(u16 va, u16 vb)
{
	int ma, mb, mc, ea, eb, ec, sc, vc;
	
	ea=(va>>10)&31;
	eb=(vb>>10)&31;
	sc=(va^vb)&0x8000;

	if(!ea || !eb)
		return(0);
	
	ma=1024|(va&1023);
	mb=1024|(vb&1023);
	mc=(ma*mb)>>10;

	ec=(ea+eb)-15;
	
	if(mc>=2048)
	{
		mc=mc>>1;
		ec++;
	}
	
	if(ec<=0)
		return(0);
	if(ec>=31)
		return(sc|0x7C00);
	
	vc=sc|(ec<<10)|(mc&1023);
	return(vc);
}

u16 tk_nn_faddh(u16 va, u16 vb)
{
	int ma, mb, mc, ea, eb, ec, sc, vc;
	
	ea=(va>>10)&31;
	eb=(vb>>10)&31;
//	sc=(va^vb)&0x8000;

	if(!ea)
		return(vb);
	if(!eb)
		return(va);
	
	ma=1024|(va&1023);
	mb=1024|(vb&1023);

	if(va&0x8000)
		ma=-ma;
	if(vb&0x8000)
		mb=-mb;

	if(ea>=eb)
	{
		ec=ea;
		mc=ma+(mb>>(ea-eb));
	}else
	{
		ec=eb;
		mc=mb+(ma>>(eb-ea));
	}
	
	sc=0;
	if(mc<0)
	{
		mc=-mc;
		sc=0x8000;
	}
	
	if(mc>=2048)
	{
		mc=mc>>1;
		ec++;
		if(ec>=31)
			{ ec=31; mc=0; }
	}else if(mc)
	{	
		while(!(mc&1024))
		{
			mc=mc<<1;
			ec--;
		}
		if(ec<=0)
			{ ec=0; mc=0; }
	}else
	{
		ec=0;
	}
		
	vc=sc|(ec<<10)|(mc&1023);
	return(vc);
}

u16 tk_nn_freluh(u16 va)
{
	if(va&0x8000)
		return(0);
	return(va);
}

u16 tk_nn_fsqrtsh(u16 va)
{
	u16 vc;
	if(!va)
		return(0);
	vc=(va&0x8000)+(((va&0x7FFF)>>1)+0x1E00);
	return(vc);
}

u16 tk_nn_fsqrtuh(u16 va)
{
	u16 vc;
	if(va&0x8000)
		return(0);
	if(!va)
		return(0);
	vc=(va&0x8000)+(((va&0x7FFF)>>1)+0x1E00);
	return(vc);
}

u64 tk_nn_pmulh(u64 va, u64 vb)
{
	u64 vc;
	vc=         tk_nn_fmulh(va>>48, vb>>48);
	vc=(vc<<16)|tk_nn_fmulh(va>>32, vb>>32);
	vc=(vc<<16)|tk_nn_fmulh(va>>16, vb>>16);
	vc=(vc<<16)|tk_nn_fmulh(va>> 0, vb>> 0);
	return(vc);
}

u64 tk_nn_paddh(u64 va, u64 vb)
{
	u64 vc;
	vc=         tk_nn_faddh(va>>48, vb>>48);
	vc=(vc<<16)|tk_nn_faddh(va>>32, vb>>32);
	vc=(vc<<16)|tk_nn_faddh(va>>16, vb>>16);
	vc=(vc<<16)|tk_nn_faddh(va>> 0, vb>> 0);
	return(vc);
}

u64 tk_nn_psubh(u64 va, u64 vb)
{
	u64 vc, vb1;
	
	vb1=vb^0x8000800080008000ULL;
	vc=         tk_nn_faddh(va>>48, vb1>>48);
	vc=(vc<<16)|tk_nn_faddh(va>>32, vb1>>32);
	vc=(vc<<16)|tk_nn_faddh(va>>16, vb1>>16);
	vc=(vc<<16)|tk_nn_faddh(va>> 0, vb1>> 0);
	return(vc);
}

u64 tk_nn_preluh(u64 va)
{
	u64 vc;
	vc=         tk_nn_freluh(va>>48);
	vc=(vc<<16)|tk_nn_freluh(va>>32);
	vc=(vc<<16)|tk_nn_freluh(va>>16);
	vc=(vc<<16)|tk_nn_freluh(va>> 0);
	return(vc);
}

u64 tk_nn_psqrtsh(u64 va)
{
	u64 vc;
	vc=         tk_nn_fsqrtsh(va>>48);
	vc=(vc<<16)|tk_nn_fsqrtsh(va>>32);
	vc=(vc<<16)|tk_nn_fsqrtsh(va>>16);
	vc=(vc<<16)|tk_nn_fsqrtsh(va>> 0);
	return(vc);
}

u64 tk_nn_psqrtuh(u64 va)
{
	u64 vc;
	vc=         tk_nn_fsqrtuh(va>>48);
	vc=(vc<<16)|tk_nn_fsqrtuh(va>>32);
	vc=(vc<<16)|tk_nn_fsqrtuh(va>>16);
	vc=(vc<<16)|tk_nn_fsqrtuh(va>> 0);
	return(vc);
}

void TK_NN_ProcessVector4w(
	u64 *vvec, u64 *wvec,
	u64 *dvec,
	int n)
{
	u64 *csv, *csw;
	u64 v0, v2, w0, w1, w2, w3;
	u64 t0, t1, t2, t3;
	int i;
	
	v2=0;
	csv=vvec;
	csw=wvec;
	for(i=0; i<n; i++)
	{
		v0=csv[0];
		w0=csw[0];	w1=csw[1];
		w2=csw[2];	w3=csw[3];
		t0=tk_nn_shuf4_0(v0);
		t1=tk_nn_shuf4_1(v0);
		t2=tk_nn_shuf4_2(v0);
		t3=tk_nn_shuf4_3(v0);
		t0=tk_nn_pmulh(t0, w0);
		t1=tk_nn_pmulh(t1, w1);
		t2=tk_nn_pmulh(t2, w2);
		t3=tk_nn_pmulh(t3, w3);
		v2=tk_nn_paddh(v2, t0);
		v2=tk_nn_paddh(v2, t1);
		v2=tk_nn_paddh(v2, t2);
		v2=tk_nn_paddh(v2, t3);
		csv++;
		csw+=4;
	}

	v0=csw[0];
	v2=tk_nn_paddh(v2, v0);
	
	if((v0&3)==1)
		{ v2=tk_nn_preluh(v2); }
	if((v0&3)==2)
		{ v2=tk_nn_psqrtsh(v2); }
	if((v0&3)==3)
		{ v2=tk_nn_psqrtuh(v2); }
	*dvec=v2;
}


void TK_NN_ProcessVector8w(
	u64 *vvec, u64 *wvec,
	u64 *dvec,
	int n)
{
	u64 *csv, *csw;
	u64 v0, v1, v2, v3, w0, w1, w2, w3;
	u64 t0, t1, t2, t3;
	int i;
	
	v2=0;
	v3=0;
	csv=vvec;
	csw=wvec;
	for(i=0; i<n; i++)
	{
		v0=csv[0];

		w0=csw[0];	w1=csw[1];
		w2=csw[2];	w3=csw[3];
		t0=tk_nn_shuf4_0(v0);
		t1=tk_nn_shuf4_1(v0);
		t2=tk_nn_shuf4_2(v0);
		t3=tk_nn_shuf4_3(v0);
		t0=tk_nn_pmulh(t0, w0);
		t1=tk_nn_pmulh(t1, w1);
		t2=tk_nn_pmulh(t2, w2);
		t3=tk_nn_pmulh(t3, w3);
		v2=tk_nn_paddh(v2, t0);
		v2=tk_nn_paddh(v2, t1);
		v2=tk_nn_paddh(v2, t2);
		v2=tk_nn_paddh(v2, t3);

		w0=csw[4];	w1=csw[5];
		w2=csw[6];	w3=csw[7];
		t0=tk_nn_shuf4_0(v0);
		t1=tk_nn_shuf4_1(v0);
		t2=tk_nn_shuf4_2(v0);
		t3=tk_nn_shuf4_3(v0);
		t0=tk_nn_pmulh(t0, w0);
		t1=tk_nn_pmulh(t1, w1);
		t2=tk_nn_pmulh(t2, w2);
		t3=tk_nn_pmulh(t3, w3);
		v3=tk_nn_paddh(v3, t0);
		v3=tk_nn_paddh(v3, t1);
		v3=tk_nn_paddh(v3, t2);
		v3=tk_nn_paddh(v3, t3);

		csv++;
		csw+=8;
	}

	v0=csw[0];
	v1=csw[1];
	v2=tk_nn_paddh(v2, v0);
	v3=tk_nn_paddh(v3, v1);
	
	if((v0&3)==1)
		{ v2=tk_nn_preluh(v2); }
	if((v0&3)==2)
		{ v2=tk_nn_psqrtsh(v2); }
	if((v0&3)==3)
		{ v2=tk_nn_psqrtuh(v2); }

	if((v1&3)==1)
		{ v3=tk_nn_preluh(v3); }
	if((v1&3)==2)
		{ v3=tk_nn_psqrtsh(v3); }
	if((v1&3)==3)
		{ v3=tk_nn_psqrtuh(v3); }

	dvec[0]=v2;
	dvec[1]=v3;
}

#endif

void TK_NN_ProcessNet4w(
	u64 *vvec, u64 *wvec,
	u64 *dvec,
	int n, int m)
{
	u64 *csw, *ct;
	int i;
	
	csw=wvec;
	ct=dvec;
	for(i=0; i<m; i++)
	{
		TK_NN_ProcessVector4w(vvec, csw, ct, n);
		csw+=n+1;
		ct++;
	}
}

void TK_NN_ProcessNet8w(
	u64 *vvec, u64 *wvec,
	u64 *dvec,
	int n, int m)
{
	u64 *csw, *ct;
	int i;
	
	csw=wvec;
	ct=dvec;
	for(i=0; i<m; i++)
	{
		TK_NN_ProcessVector4w(vvec, csw, ct, n);
		csw+=n+n+2;
		ct+=2;
	}
}

