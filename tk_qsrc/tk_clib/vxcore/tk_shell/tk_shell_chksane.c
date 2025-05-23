// char tb_cwd[256];

void tk_shell_chksane_simd_asm();
void tk_shell_chksane_rgb5_asm();
void tk_shell_chksane_fmovs();
void tk_shell_chksane_srtmsk();
void tk_shell_chksane_movtt();

__vec4f		tk_shell_fv0_gbl;

__asm {
tk_shell_chksane_simd_asm:
//	PUSH	SR

	ADD		-8, SP
	MOV		SR, R1
	MOV.Q		R1, (SP)

	WEXMD	2

	MOV		0x123456789ABCDEF0, R4
	MOV		0x3456789ABCDEF012, R5
	MOV		0x468ACF12579ACF02, R6
	MOV		0xDDDEDDDEDDDEEEDE, R7
	
	PADD.W		R4, R5, R18		|	PSUB.W	R4, R5, R19
	CMPQEQ		R18, R6
	BREAK?F
	CMPQEQ		R19, R7
	BREAK?F

	.L0A0:

	MOV		0xAD7844303F08ACE0, R6
	MOV		0x03B828BC7228D110, R7

	PMULU.LW	R4, R5, R18		|	PMULU.HW	R4, R5, R19
	CMPQEQ		R18, R6
	BREAK?F
	CMPQEQ		R19, R7
	BREAK?F

	MOV		0xAD7844303F08ACE0, R6
	MOV		0x03B828BC1A8E020E, R7

	.L0A1:
	PMULS.LW	R4, R5, R18		|	PMULS.HW	R4, R5, R19
	CMPQEQ		R18, R6
	BREAK?F
	CMPQEQ		R19, R7
	BREAK?F

	MOV		0xDEF09ABC56781234, R6
	MOV		0xF012BCDE789A3456, R7

	.L0A2:
	PSHUF.W		R4, 0x1B, R18	|	PSHUF.W		R5, 0x1B, R19
	CMPQEQ		R18, R6
	BREAK?F
	CMPQEQ		R19, R7
	BREAK?F

	MOV		0x9ABCDEF0BCDEF012, R6
	MOV		0x123456783456789A, R7

	.L0A3:
	MOVLD	R4, R5, R18
	MOVHD	R4, R5, R19
	CMPQEQ		R18, R6
	BREAK?F
	CMPQEQ		R19, R7
	BREAK?F

	MOV		0x12345678BCDEF012, R6
	MOV		0x9ABCDEF03456789A, R7

	.L0A4:
	MOVHLD	R4, R5, R18
	MOVLHD	R4, R5, R19
	CMPQEQ		R18, R6
	BREAK?F
	CMPQEQ		R19, R7
	BREAK?F

	MOV		0x123456789ABCDEF0, R4
	MOV		0x3456789ABCDEF012, R5

	//12345678=4.03896783e-28
	//3456789A=1.99742e-07
	// + => 1.99742000000000000000403896783e-7‬  (0x345678aa)
	// - => -1.99741999999999999999596103217e-7‬ (0xb45678aa)
	// * => ‭8.0675151229986e-35‬ (0x0717153D)
	// 9ABCDEF0 => -7.81152e-23
	// BCDEF012 => -0.0272141
	// + => -0.0272141000000000000000781152 (0xbcdef01b)
	// - => ‭0.0272140999999999999999218848‬ (0x3cdef01b)
	// * => 2.12583486432e-24‬ (0x18247a72)

	MOV		0x3456789abcdef012, R6
	MOV		0xb456789a3cdef012, R7

	.L0A5:
	PADD.F		R4, R5, R18
	PSUB.F		R4, R5, R19
//	PMUL.F		R4, R5, R20
	CMPQEQ		R18, R6
	BREAK?F
	CMPQEQ		R19, R7
	BREAK?F

	.L0A6:
	MOV		0x0717153D18247A72, R6
	PMUL.F		R4, R5, R18
	CMPQEQ		R18, R6
	BREAK?F


	XOR		R18, R18
	XOR		R19, R19

	MOV		R4, R20
	MOV		R4, R21
	MOV		R5, R22
	MOV		R5, R23

	MOV		0x3456789abcdef012, R6
	MOV		0xb456789a3cdef012, R7

	.L0A7:
	PADDX.F		R20, R22, R18
	CMPQEQ		R18, R6
	BREAK?F
	CMPQEQ		R19, R6
	BREAK?F

	XOR		R18, R18
	XOR		R19, R19

	.L0A8:
	PSUBX.F		R20, R22, R18
	CMPQEQ		R18, R7
	BREAK?F
	CMPQEQ		R19, R7
	BREAK?F

	MOV		0x0717153D18247A72, R16

	.L0A9:
	PMULX.F		R20, R22, R18
	CMPQEQ		R18, R16
	BREAK?F
	CMPQEQ		R19, R16
	BREAK?F

	.L0A10:
	MOV.X		(SP, 0), R18
	PMULX.F		R20, R22, R18
	CMPQEQ		R18, R16
	BREAK?F
	CMPQEQ		R19, R16
	BREAK?F

	MOV			GBR, R16
	MOV			tk_shell_fv0_gbl, R17
//	ADD			-384, R17
	ADD			-6144, R17
	MOV			R17, GBR


	MOV.X		R4, tk_shell_fv0_gbl
	NOP
	MOV.X		tk_shell_fv0_gbl, R18

	.L0A11:
	CMPQEQ		R18, R4
	BREAK?F
	CMPQEQ		R19, R5
	BREAK?F

//	MOV.X		R6, (GBR, 384)
	MOV.X		R6, (GBR, 6144)
	NOP
//	MOV.X		(GBR, 384), R18
	MOV.X		(GBR, 6144), R18

	.L0A12:
	CMPQEQ		R18, R6
	BREAK?F
	CMPQEQ		R19, R7
	BREAK?F

#if 1
.ifarch has_jcmp
	MOV		0x3456789abcdef012, R6
	MOV		0xb456789a3cdef012, R7
	
	BREQ	R6, R6, .J0L0
	BREAK
	.J0L0:

	BRNE	R6, R7, .J0L1
	BREAK
	.J0L1:

	BRGT	R7, R6, .J0L2
	BREAK
	.J0L2:

	BRGE	R6, R6, .J0L3
	BREAK
	.J0L3:

.endif
#endif

	MOV			R16, GBR

//	BREAK

	MOV.Q	(SP), R1
	MOV		R1, SR
	ADD		8, SP

//	POP SR
	RTSU
	

tk_fcn_clz:
	clz		R4, R2
	RTS

tk_fcn_clzq:
	CLZQ	R4, R2
	RTS

tk_shell_chksane_rgb5_asm:
	/* Basic Opaque Color */

	MOV			0x1234, R16

	MOV			0xFF218CA5, R20
	MOV			0xFFFF21218C8CA5A5, R21

	
	RGB5UPCK32	R16, R4
	RGB5UPCK64	R16, R5
	
	.L0:
	CMPQEQ		R4, R20
	BREAK?F
	CMPQEQ		R5, R21
	BREAK?F

	RGB5PCK32	R4, R18
	RGB5PCK64	R5, R19

	CMPEQ		R18, R16
	BREAK?F
	CMPEQ		R19, R16
	BREAK?F


	/* Translucent Color */

	MOV			0xABCD, R16

	MOV			0x2052F76B, R20
	MOV			0x20205252F7F76B6B, R21

	RGB5UPCK32	R16, R4		|	RGB5UPCK64	R16, R5
	
	.L1:
	CMPQEQ		R4, R20
	BREAK?F
	CMPQEQ		R5, R21
	BREAK?F

	RGB5PCK32	R4, R18		|	RGB5PCK64	R5, R19

	CMPEQ		R18, R16
	BREAK?F
	CMPEQ		R19, R16
	BREAK?F

	/* RGB32 Pack / Unpack */

	MOV			0x2052F76B, R20
	MOV			0x20205252F7F76B6B, R21

	RGB32UPCK64	R20, R4
	RGB32PCK64	R21, R5

	CMPQEQ		R4, R21
	BREAK?F
	CMPEQ		R5, R20
	BREAK?F

	/* Test Word Shuffle */

	MOV			0x20205252F7F76B6B, R16
	MOV			0x2052F76B, R17

	MOV			0x2020202020202020, R20
	MOV			0x6B6BF7F752522020, R21

	MOV			0x20202020, R22
	MOV			0x6BF75220, R23

	PSHUF.W		R16, 0xFF, R4	|	PSHUF.W		R16, 0x1B, R5
	PSHUF.B		R17, 0xFF, R6	|	PSHUF.B		R17, 0x1B, R7

	.L2:
	CMPQEQ		R4, R20
	BREAK?F
	CMPQEQ		R5, R21
	BREAK?F

	CMPEQ		R6, R22
	BREAK?F
	CMPEQ		R7, R23
	BREAK?F

	/* PADD.W */

	MOV			0x4040A4A4EFEED6D6, R18
	MOV			0x04081A78F02E2D12, R19
	PADD.W		R16, R16, R4	|	PADD.W		R16, R16, R5
	PMULU.HW	R16, R16, R6	|	PMULU.HW	R16, R16, R7

	.L2B:
	CMPQEQ		R4, R18
	BREAK?F
	CMPQEQ		R5, R18
	BREAK?F

	CMPQEQ		R6, R19
	BREAK?F
	CMPQEQ		R7, R19
	BREAK?F

	/* Morton Shuffle */

	MOV			0xFFFF0000FF0000FF, R16
	MOV			0xFF0000FFFFFF0000, R17
	PMORT.L		R16, R4		|	PMORT.L		R17, R5
	PMORT.Q		R16, R6		|	PMORT.Q		R17, R7

	MOV			0xFFFFAAAA, R20
	MOV			0xFFFF5555, R21
	MOV			0xFFFFAAAA00005555, R22
	MOV			0xFFFF55550000AAAA, R23

	.L3:
	CMPEQ		R4, R20
	BREAK?F
	CMPEQ		R5, R21
	BREAK?F

	CMPQEQ		R6, R22
	BREAK?F
	CMPQEQ		R7, R23
	BREAK?F

	/* UTX2 */
#ifndef TK_IS_RBD
	MOV			0xFFAA550012342BCD, R16

	MOV			0xFFFF21218C8CA5A5, R20
	MOV			0xFFFF3131AFAF9191, R21
	MOV			0xFFFF4242D3D37E7E, R22
	MOV			0xFFFF5252F7F76B6B, R23

	MOV			0, R2
	MOV			15, R3
	BLKUTX2		R16, R2, R4
	BLKUTX2		R16, R3, R5

	.L4A0:
	CMPQEQ		R4, R20
	BREAK?F
	CMPQEQ		R5, R23
	BREAK?F

	MOV			6, R2
	MOV			10, R3
	BLKUTX2		R16, R2, R4
	BLKUTX2		R16, R3, R5

#if 0
	.L4A1:
	CMPQEQ		R4, R21
	BREAK?F
	CMPQEQ		R5, R22
	BREAK?F
#endif

	MOV			0xFFAA55009234ABCD, R16

	MOV			0x404021218C8CA5A5, R20
	MOV			0x20205252F7F76B6B, R23

	MOV			0, R2
	MOV			15, R3
	BLKUTX2		R16, R2, R4
	BLKUTX2		R16, R3, R5

#if 0
	.L4B:
	CMPQEQ		R4, R20
	BREAK?F
	CMPQEQ		R5, R23
	BREAK?F
#endif

#endif

	/* Test FLDCH Immed */
//	MOV			0x40159C0000000000, R20
//	MOV			0x40E2680000000000, R21

//	MOV			0xC0159C0000000000, R22
//	MOV			0xC0E2680000000000, R23

	MOV			0x40159C00, R20
	MOV			0x40E26800, R21

	MOV			0xC0159C00, R22
	MOV			0xC0E26800, R23
	
	SHLD.Q		R20, 32, R20
	SHLD.Q		R21, 32, R21
	SHLD.Q		R22, 32, R22
	SHLD.Q		R23, 32, R23

	FLDCH	0x4567, R4
	FLDCH	0x789A, R5

	CMPQEQ		R4, R20
	BREAK?F
	CMPQEQ		R5, R21
	BREAK?F

	FLDCH	0x4567, R6	|	FLDCH	0x789A, R7 | NOP

	CMPQEQ		R6, R20
	BREAK?F
	CMPQEQ		R7, R21
	BREAK?F

	FLDCH	0xC567, R6	|	FLDCH	0xF89A, R7 | NOP

	CMPQEQ		R6, R22
	BREAK?F
	CMPQEQ		R7, R23
	BREAK?F

// #if 1
#ifndef TK_IS_RBD
	PCVTSW2H	R16, R6
	MOV			0xC200C200C200C200, R3
	PADD.H		R6, R3, R7
	PCVTH2AL	R7, R2

	MOV			0x0075FBF5, R20
.L_PCVTH2AL_0:
	CMPEQ		R2, R20
	BREAK?F
#endif

#if 0
	RGB5MINMAX	R16, R6
	MOV			0, R7
	RGB5CCENC	R16, R6, R7

	MOV			0x5500FFAA, R20
//	MOV			0xC7000000, R21
	MOV			0x000000C7, R21
.L_RGB5MINMAX_0:
	CMPEQ		R6, R20
	BREAK?F

	CMPEQ		R7, R21
	BREAK?F
#endif

// #if 1
#ifndef TK_IS_RBD
	RGB5PCKI8	R3, R6
	CMPEQ		0, R6
	BREAK?T
#endif

	RTSU

tk_shell_chksane_fmovs_i:
	ADD		-64, SP

.L0:
	FLDCF	R4, R16
	FLDCF	R5, R17
	FSTCF	R16, R20
	FSTCF	R17, R21

	CMPQEQ		R4, R20
	BREAK?F
	CMPQEQ		R5, R21
	BREAK?F

.ifarch has_fmovs

.L1:
	FLDCF	R4, R16
	FLDCF	R5, R17
	FMOV.S	R16, (SP, 0)
	FMOV.S	R17, (SP, 4)
	FMOV.S	(SP, 0), R18
	FMOV.S	(SP, 4), R19
	FSTCF	R18, R20
	FSTCF	R19, R21

	CMPQEQ		R4, R20
	BREAK?F
	CMPQEQ		R5, R21
	BREAK?F


.L2:
	MOV		2, R22
	MOV		3, R23

	FLDCF	R4, R16
	FLDCF	R5, R17
	FMOV.S	R16, (SP, R22)
	FMOV.S	R17, (SP, R23)
	FMOV.S	(SP, R22), R18
	FMOV.S	(SP, R23), R19
	FSTCF	R18, R20
	FSTCF	R19, R21

	CMPQEQ		R4, R20
	BREAK?F
	CMPQEQ		R5, R21
	BREAK?F

.endif

.ifarch has_fmovh
	FLDCF	R4, R16
	FLDCF	R5, R17
	FSTCH	R16, R6
	FSTCH	R17, R7

.L4:
	FLDCH	R6, R16
	FLDCH	R7, R17
	FMOV.H	R16, (SP, 0)
	FMOV.H	R17, (SP, 4)
	FMOV.H	(SP, 0), R18
	FMOV.H	(SP, 4), R19
	FSTCH	R18, R20
	FSTCH	R19, R21

	CMPEQ		R6, R20
	BREAK?F
	CMPEQ		R7, R21
	BREAK?F

.L5:
	MOV		2, R22
	MOV		3, R23

	FLDCH	R6, R16
	FLDCH	R7, R17
	FMOV.H	R16, (SP, R22)
	FMOV.H	R17, (SP, R23)
	FMOV.H	(SP, R22), R18
	FMOV.H	(SP, R23), R19
	FSTCH	R18, R20
	FSTCH	R19, R21

	CMPEQ		R6, R20
	BREAK?F
	CMPEQ		R7, R21
	BREAK?F

.endif

	ADD		64, SP
	RTSU

tk_shell_chksane_fmovs:
	MOV		LR, R1
	.LA0:

	MOV		0x00123456, R4
	MOV		0x00123457, R5
	BSR		tk_shell_chksane_fmovs_i

	.LA1:

	MOV		0x80123456, R4
	MOV		0x80123457, R5
	BSR		tk_shell_chksane_fmovs_i

	.LA2:

	MOV		0x7F923456, R4
	MOV		0x7F923457, R5
	BSR		tk_shell_chksane_fmovs_i

	.LA3:

	MOV		0xFF923456, R4
	MOV		0xFF923457, R5
	BSR		tk_shell_chksane_fmovs_i


	MOV		0x3F923456, R4
	MOV		0x3F923457, R5
	BSR		tk_shell_chksane_fmovs_i


	MOV		0xBF923456, R4
	MOV		0xBF923457, R5
	BSR		tk_shell_chksane_fmovs_i

	JMP		R1
//	RTSU

tk_shell_chksane_padds_sf:
	PADDX.FA	R4, R6, R2
	RTS

tk_shell_chksane_pmuls_sf:
	PMULX.FA	R4, R6, R2
	RTS


tk_shell_chksane_srtmsk:
	CMPEQ	R4, R4
	NOP
	NOP
	CMPGT	R4, R4
	ADD		1, R4
	BREAK?T

	NOP
	CMPEQ	R4, R4
	MOVZT	R4, R4
	BREAK?F

	MOV		0xF123, R0
	CMPGT	R4, R4
	MOVTT	R4, R0, R4
	BREAK?T

	NOP
	CMPEQ		R4, R4
	BNDCHK.B	R4, R4
	BREAK?F

	RTS

tk_shell_chksane_movtt:
	MOV		0x0000456789ABCDEF, R4
	MOV		0x123A456789ABCDEF, R5

	MOVTT	R4, 0x123A, R6
	CMPQEQ	R5, R6
	BREAK?F

	MOVZT	R6, R7
	CMPQEQ	R4, R7
	BREAK?F

	RTS
};

__vec4f tk_shell_chksane_padds_sf(__vec4f va, __vec4f vb);
__vec4f tk_shell_chksane_pmuls_sf(__vec4f va, __vec4f vb);

int tk_fcn_clz(long long j);
int tk_fcn_clzq(long long j);

int tk_shell_chksane_clz()
{
	unsigned long long li, lj, lk;
	void *p;
	int i, j, k;
	
	p=tk_shell_chksane_clz;
	li=(long long)p;
	for(i=0; i<32; i++)
		li=li*251+1;
	
	for(i=0; i<32; i++)
	{
		lj=(1ULL<<(31-i));
		lk=lj|(li&(lj-1));
		k=tk_fcn_clz(lk);
		if(k!=i)
			__debugbreak();
		li=li*251+1;
	}

	for(i=0; i<64; i++)
	{
		lj=(1ULL<<(63-i));
		lk=lj|(li&(lj-1));
		k=tk_fcn_clzq(lk);
		if(k!=i)
			__debugbreak();
		li=li*251+1;
	}
}

__m128 __m128_float4(float f0, float f1, float f2, float f3);

int tk_shell_chksane_simd()
{
	__m128 mv0;
	__vec4f fv0, fv1, fv2;
	__quatf qv0, qv1, qv2, qv3;
	double	f0, f1, f2, f3;
	
	tk_shell_chksane_simd_asm();
	tk_shell_chksane_rgb5_asm();

//	__hint_cc_dbgbreak();
	
//	mv0=__m128_float4(1.0, 2.0, 3.0, 5.0);
	mv0=(__vec4f){1.0, 2.0, 3.0, 5.0};
	
//	__debugbreak();
	
	tk_shell_fv0_gbl = mv0;
	
//	__hint_cc_dbgbreak();
	
	fv0=mv0;
	
//	if(fv0.x!=1.0)
//		__debugbreak();
//	if(fv0.z!=3.0)
//		__debugbreak();
	
	fv1=fv0+fv0;
	fv2=fv1*fv0;

//	if(fv2.x!=2.0)
//		__debugbreak();
//	if(fv2.z!=18.0)
//		__debugbreak();
	
//	__debugbreak();

	f0=fv1.x;
	f1=fv1.y;
	f2=fv1.z;
	f3=fv1.w;

//	__debugbreak();
	
//	tk_printf("SIMD A0-0: %f %f %f %f\n", fv1.x, fv1.y, fv1.z, fv1.w);

	tk_printf("SIMD A0-0: %f %f %f %f\n", fv1.x, fv1.y, fv1.z, fv1.w);
	tk_printf("   Expect: %f %f %f %f\n", 2.0, 4.0, 6.0, 10.0);
	tk_printf("SIMD A0-1: %f %f %f %f\n", fv2.x, fv2.y, fv2.z, fv2.w);
	tk_printf("   Expect: %f %f %f %f\n", 2.0, 8.0, 18.0, 50.0);

	if(fv2.x!=2.0)
		__debugbreak();
	if(fv2.z!=18.0)
		__debugbreak();

	fv1=tk_shell_chksane_padds_sf(fv0, fv0);
	fv2=tk_shell_chksane_pmuls_sf(fv1, fv0);

	tk_printf("\n");

	tk_printf("SIMD A0-2: %f %f %f %f\n", fv1.x, fv1.y, fv1.z, fv1.w);
	tk_printf("   Expect: %f %f %f %f\n", 2.0, 4.0, 6.0, 10.0);
	tk_printf("SIMD A0-3: %f %f %f %f\n", fv2.x, fv2.y, fv2.z, fv2.w);
	tk_printf("   Expect: %f %f %f %f\n", 2.0, 8.0, 18.0, 50.0);

	tk_printf("\n");

//	__debugbreak();

	qv0 = (__quatf) { 1, 2, 3, 4 };
	qv1 = 1i + 2j + 3k + 4;
	qv2 = qv0*qv1;
	qv3 = (1i + 2j + 3k + 4) * (1i + 2j + 3k + 4);

	tk_printf("SIMD A1-0: %f %f %f %f\n", qv2.i, qv2.j, qv2.k, qv2.r);
	tk_printf("SIMD A1-1: %f %f %f %f\n", qv3.i, qv3.j, qv3.k, qv3.r);

//	__debugbreak();

}


void Sys_CheckSanityB(void)
{
	u32 t_arr[4]={0x12345678, 0xAB89EFCD, 0x00001234, 0x89ABCDEF};
	u64 t_arrl[4]={
		0x0000123412345678ULL, 0xAB89EFCD12345678ULL,
		0x00001234, 0x89ABCDEF};
	double fg;
	float ff;
	byte b_arr0[16];
	u32 ui, uj, uk;
	u64 uli, ulj, ulk;
	int i, j, k;

//	tk_printf("VA Tst: (%X %X %X %X) (%X %X %X %X)\n",
//		0x12345678, 0x89ABCDEF,	0x56781234, 0xCDEF89AB,
//		0x34127856, 0xAB89EFCD,	0x78563412, 0xEFCDAB89);

#if 1
	if((t_arr[0]>>4)!=0x01234567)
		__debugbreak();
	if((((s32)t_arr[1])>>4)!=((s32)0xFAB89EFC))
		__debugbreak();
	if((((u32)t_arr[1])>>4)!=((u32)0x0AB89EFC))
		__debugbreak();

	if((t_arr[0]>>11)!=0x0002468A)
		__debugbreak();
	if((((s32)t_arr[1])>>13)!=((s32)0xFFFD5C4F))
		__debugbreak();
	if((((u32)t_arr[1])>>13)!=((u32)0x00055C4F))
		__debugbreak();
	if((t_arr[2]<<17)!=0x24680000)
		__debugbreak();

	if(((t_arr[0]>>4)&15)!=7)
		__debugbreak();

//	if((((s64)(t_arr[0])>>4)&15)!=7)
//		__debugbreak();
//	if(((0x0123456789ABCDEFLL>>44)&15)!=5)
//		__debugbreak();

	tk_puts("Q Pt0 OK\n");
	
	((short *)(b_arr0))[0]=0x1234;
	((short *)(b_arr0))[1]=0xABCD;
	if(((short *)(b_arr0))[0]!=0x1234)
		__debugbreak();
	if(((short *)(b_arr0))[1]!=((int)0xFFFFABCD))
		__debugbreak();
	if(((unsigned short *)(b_arr0))[0]!=0x1234)
		__debugbreak();
	if(((unsigned short *)(b_arr0))[1]!=0xABCD)
		__debugbreak();

	tk_puts("Q Pt0 OK 1\n");

	((char *)(b_arr0))[0]=0x1234;
	((char *)(b_arr0))[1]=0xABCD;
	if(((signed char *)(b_arr0))[0]!=0x34)
		__debugbreak();
	if(((signed char *)(b_arr0))[1]!=((int)0xFFFFFFCD))
		__debugbreak();
	if(((unsigned char *)(b_arr0))[0]!=0x34)
		__debugbreak();
	if(((unsigned char *)(b_arr0))[1]!=0xCD)
		__debugbreak();

	tk_puts("Q Pt0 OK 2\n");

	i=t_arr[0]; j=i<<16; k=j>>16;
	if(k!=0x5678)
		__debugbreak();
	i=t_arr[3]; j=i<<16; k=j>>16;
	if(k!=0xFFFFCDEF)
		__debugbreak();
	i=t_arr[3]; j=i<<16; k=((unsigned int)j)>>16;
	if(k!=0xCDEF)
		__debugbreak();

	tk_puts("Q Pt0 OK 3\n");
	
	ui=t_arr[3];	uli=ui;
	if(uli!=0x0000000089ABCDEFULL)
		__debugbreak();
	i=t_arr[3];		uli=i;
	if(uli!=0xFFFFFFFF89ABCDEFULL)
		__debugbreak();

	uli=t_arr[3];	ulj=t_arr[0];	ulk=uli*ulj;
	if(ulk!=0x09CA39E0E242D208ULL)
		__debugbreak();
	uli=(s32)(t_arr[3]);	ulj=(s32)(t_arr[0]);	ulk=uli*ulj;
	if(ulk!=0xF795E368E242D208ULL)
		__debugbreak();

	tk_puts("Q Pt0 OK 4\n");

	uli=t_arrl[0];	ulj=t_arrl[1];
	ulk=t_arrl[0];
	if(uli==ulj)	__debugbreak();
	if(uli>ulj)		__debugbreak();
	if(uli>=ulj)	__debugbreak();
	if(ulj<uli)		__debugbreak();
	if(ulj<=uli)	__debugbreak();

	if(uli!=ulk)	__debugbreak();
	if(uli>ulk)		__debugbreak();
	if(uli<ulk)		__debugbreak();

	tk_puts("Q Pt0 OK 5\n");

	if((t_arr[0]/10)!=0x1D208A5)
		__debugbreak();
	if((t_arr[0]%10)!=6)
		__debugbreak();

	tk_puts("Q Pt0 OK 5-1\n");

#if 1
	*((int *)b_arr0)=10;
	i=*((int *)b_arr0);
	if((t_arr[0]/i)!=0x1D208A5)
		__debugbreak();

	tk_puts("Q Pt0 OK 5-2\n");

	if((t_arr[0]%i)!=6)
		__debugbreak();
#endif

#endif

	tk_puts("Q Pt0 OK 6\n");

	i=123456;
	ff=i;
	j=ff;
	if(i!=j)
		__debugbreak();
		
	i=123456789;
	fg=i;
	j=fg;
	if(i!=j)
		__debugbreak();


	i=-123456;
	ff=i;
	j=ff;
	if(i!=j)
		__debugbreak();
		
	i=-123456789;
	fg=i;
	j=fg;
	if(i!=j)
		__debugbreak();
	
	ff=(i==i);
	ff=!ff;
	j=ff;
	if(j)
		__debugbreak();
	if(ff)
		__debugbreak();
	ff=!ff;
	if(ff!=1.0)
		__debugbreak();
	if(!ff)
		__debugbreak();
		
	ff=123456;
	if(ff!=123456)
		__debugbreak();
		
	ff=123;
	*(short float *)b_arr0=ff;
	ff=*(short float *)b_arr0;
	j=*(short *)b_arr0;

	if(ff!=123)
		__debugbreak();

	ff=456;
	((short float *)b_arr0)[1]=ff;
	ff=((short float *)b_arr0)[1];

	if(ff!=456)
		__debugbreak();

	ff=0.09375;
	((short float *)b_arr0)[2]=ff;
	ff=((short float *)b_arr0)[2];
	j=((short *)b_arr0)[2];

	if(ff!=0.09375)
		__debugbreak();

	ff=-0.09375;
	((short float *)b_arr0)[3]=ff;
	ff=((short float *)b_arr0)[3];
	if(ff!=-0.09375)
		__debugbreak();

	ff=0.0000009375;
	((short float *)b_arr0)[3]=ff;
	ff=((short float *)b_arr0)[3];
	if(ff!=0.0)
		__debugbreak();

	j=2;
	ff=456;
	((short float *)b_arr0)[j]=ff;
	ff=((short float *)b_arr0)[j];

	if(ff!=456)
		__debugbreak();
		
#if 1	
	ff=16.0;
	((__float8u *)b_arr0)[0]=ff;
	ff=((__float8u *)b_arr0)[0];
	if(ff!=16.0)
		__debugbreak();

	ff=-16.0;
	((__float8s *)b_arr0)[1]=ff;
	ff=((__float8s *)b_arr0)[1];
	if(ff!=-16.0)
		__debugbreak();
#endif
}

long long __int32_dmuls(int a, int b);

void Sys_CheckSanityC(void)
{
	double f, g, h;
	long long la, lb, lc;
	int a, b, c;
	
	tk_printf("CS-C A0\n");

	f=0.0;
	h=3.14159;
	*(&f)=h;
	g=f*100;
	a=g;
	if(a!=314)
		__debugbreak();

	g=f*1000;
	a=g;
	if(a!=3141)
		__debugbreak();

	g=f*10000;
	a=g;
	if(a!=31415)
		__debugbreak();

	tk_printf("CS-C A1\n");

	g=f*100000;
	a=g;
	if(a!=314159)
		__debugbreak();

	g=f*(-10000);
	a=g;
	if(a!=(-31415))
		__debugbreak();

	g=f*0.1;
	a=g;
	if(a!=0)
		__debugbreak();
		
	g=f*0.31;
	a=g;
	if(a!=0)
		__debugbreak();

	tk_printf("CS-C A2\n");

	g=f*1.75;
	a=g*1000000;
//	if(g!=5.4977825)
	if(a!=5497782)
		__debugbreak();

	g=f*0.125;
	a=g*1000000;
//	if(g!=0.39269875)
	if(a!=392698)
		__debugbreak();

	g=f*0.15625;
	a=g*1000000;
//	if(g!=0.4908734375)
	if(a!=490873)
		__debugbreak();

	tk_printf("CS-C A3\n");

	if(g>0.5)
		__debugbreak();
	if(g>=0.5)
		__debugbreak();

	if(g<0.25)
		__debugbreak();
	if(g<=0.25)
		__debugbreak();

	tk_printf("CS-C A4\n");

	g=0.21875;
	a=0;
	if(g>0.21875)
		__debugbreak();
	if(g>=0.21875)
		a=1;
	if(!a)
		__debugbreak();

//	__debugbreak();

	tk_printf("CS-C A5\n");

	a=0;
	if(g<0.21875)
		__debugbreak();
	if(g<=0.21875)
		a=1;
	if(!a)
		__debugbreak();

	tk_printf("CS-C A6\n");

	g=sqrt(f);
	h=g*g;
	
	tk_printf("%f %f\n", f, h);
	
	tk_printf("CS-C B0\n");
	
	a=314159; b=271828;
	lc=__int32_dmuls(a, b);
	if(lc!=85397212652LL)
		__debugbreak();

	tk_printf("CS-C B1\n");

	a=314159; b=-271828;
	lc=__int32_dmuls(a, b);
	if(lc!=(-85397212652LL))
		__debugbreak();

//	__debugbreak();

	tk_printf("CS-C B2\n");

	a=-314159; b=271828;
	lc=__int32_dmuls(a, b);
	if(lc!=(-85397212652LL))
		__debugbreak();
	
	tk_printf("CS-C B3\n");
}


int tkra_clamp255(int v)
{
	if(v<0)v=0;
	if(v>255)v=255;
//	__debugbreak();
	return(v);
}

int tkra_clamp127s(int v)
{
	if(v<(-127))v=(-127);
	if(v>( 127))v=( 127);
	return(v);
}


struct sanity_dummyobj1_s {
u32 tkgl_begin_rgba;
};

struct sanity_dummyobj1_s sanity_tmpobj1;

struct sanity_dummyobj1_s *sanity_getCtx()
{
	return(&sanity_tmpobj1);
}

void (*sanity_glColor4f_pf)(float red, float green, float blue, float alpha);

void sanity_glColor4f(float red, float green, float blue, float alpha)
{
	struct sanity_dummyobj1_s *ctx;
//	TKRA_Context *ctx;
	int cr, cg, cb, ca;
	u32 px;

	tk_printf("%f %f %f %f\n", red, green, blue, alpha);

	tk_printf("%f %f %f %f  %f\n",
		red*255, green*255, blue*255, alpha*255, 255.0);

	cr=(int)(red*255);
	cg=(int)(green*255);
	cb=(int)(blue*255);
	ca=(int)(alpha*255);

	tk_printf("A0 %d %d %d %d\n", cr, cg, cb, ca);

	cr=tkra_clamp255(cr);
	cg=tkra_clamp255(cg);
	cb=tkra_clamp255(cb);
	ca=tkra_clamp255(ca);

	tk_printf("A1 %d %d %d %d\n", cr, cg, cb, ca);

	cr=tkra_clamp255((int)(red*255));
	cg=tkra_clamp255((int)(green*255));
	cb=tkra_clamp255((int)(blue*255));
	ca=tkra_clamp255((int)(alpha*255));
	px=(ca<<24)|(cr<<16)|(cg<<8)|cb;

//	__debugbreak();

	tk_printf("A2 %d %d %d %d\n", cr, cg, cb, ca);
	tk_printf("A3 %08X\n", px);

//	return(px);

	ctx=sanity_getCtx();
//	ctx=TKRA_GetCurrentContext();
	ctx->tkgl_begin_rgba=px;
//	tkra_glColor4ubI(red*255.0, green*255.0, blue*255.0, alpha*255.0);
}

#include "tk_shell_chksvar.c"
#include "tk_shell_chksbcd.c"

#include "tk_shell_chkocr.c"

int tk_shell_chksane_arith()
{
	int				*pi, *pj;
	long long		*pli, *plj;
	unsigned int	*pui, *puj;

	long long		li, lj, lk, ll;
	unsigned int	ui, uj, uk, ul;
	int				i, j, k, l;

	i=0; j=0; ui=0; uj=0;

	pi=&i;
	pj=&j;

	pui=&ui;
	puj=&uj;

	*pi=0x7FFFFFFDU;
	*pj=7;

	*pui=0xFFFFFFFBU;
	*puj=7;

	lk=i/j;
	ll=ui/uj;

	if(lk!=0x0000000012492491LL)
		__debugbreak();
	if(ll!=0x0000000024924923ULL)
		__debugbreak();

	lk=i/7;
	ll=ui/7;

	if(lk!=0x0000000012492491LL)
		__debugbreak();
	if(ll!=0x0000000024924923ULL)
		__debugbreak();

	if(lk==0xFFFFFFFF12492491LL)
		__debugbreak();

	*pi=0x80000003U;
	lk=i/7;

	if(lk!=0xFFFFFFFFEDB6DB6FLL)
		__debugbreak();
	if(lk==0xEDB6DB6FU)
		__debugbreak();

	*pi=123456;
	*pj=56789;
	*pui=123456;
	*puj=56789;

	lk=i*j;
	ll=ui*uj;

	if(lk!=0xFFFFFFFFA1E27F40LL)
		__debugbreak();
	if(ll!=0x0000000A1E27F40ULL)
		__debugbreak();

	lk=i*uj;
	ll=ui*j;
	if(ll!=0x0000000A1E27F40ULL)
		__debugbreak();
	if(ll!=0x0000000A1E27F40ULL)
		__debugbreak();

	lk=i*56789;
	ll=ui*56789;

	if(lk!=0xFFFFFFFFA1E27F40LL)
		__debugbreak();
	if(ll!=0x0000000A1E27F40ULL)
		__debugbreak();

	*pi=123456;
	*pj=6789;
	k=i*j;
	l=i*(-j);

	if(k!= 838142784)
		__debugbreak();
	if(l!=-838142784)
		__debugbreak();
		
	if(((byte)i)!=0x40)
		__debugbreak();
	if(((byte)j)!=0x85)
		__debugbreak();

	if(((s16)i)!=-7616)
		__debugbreak();
	if(((u16)i)!=0xE240)
		__debugbreak();
	
	*pi=123456;
	*pj=10;
	k=i/j;
	l=i%j;
	
	if(k!=12345)
		__debugbreak();
	if(l!=6)
		__debugbreak();

	*pj=1000;
	k=i/j;
	l=i%j;
	if(k!=123)
		__debugbreak();
	if(l!=456)
		__debugbreak();

	*pj=-1000;
	k=i/j;
	l=i%j;
	if(k!=-123)
		__debugbreak();
	if(l!=456)
		__debugbreak();

	*pi=-123456;
	*pj=1000;
	k=i/j;
	l=i%j;
	if(k!=-123)
		__debugbreak();
	if(l!=-456)
		__debugbreak();

	k=i/256;
	l=i%256;
	if(k!=-482)
		__debugbreak();
	if(l!=-64)
		__debugbreak();

 	*pi=-30873;
	k=i/251;
	l=i%251;
	if(k!=-123)
		__debugbreak();
	if(l!=0)
		__debugbreak();

 	*pi=-30880;
	k=i/251;
	l=i%251;
	if(k!=-123)
		__debugbreak();
	if(l!=-7)
		__debugbreak();

	li=0; lj=0;

	pli=&li;
	plj=&lj;


	*pli=123456;
	*plj=10;
	lk=li/lj;
	ll=li%lj;
	
	if(lk!=12345)
		__debugbreak();
	if(ll!=6)
		__debugbreak();

	*plj=1000;
	lk=li/lj;
	ll=li%lj;
	if(lk!=123)
		__debugbreak();
	if(ll!=456)
		__debugbreak();

	*plj=-1000;
	lk=li/lj;
	ll=li%lj;
	if(lk!=-123)
		__debugbreak();
	if(ll!=456)
		__debugbreak();

	*pli=-123456;
	*plj=1000;
	lk=li/lj;
	ll=li%lj;
	if(lk!=-123)
		__debugbreak();
	if(ll!=-456)
		__debugbreak();

	lk=li/256;
	ll=li%256;
	if(lk!=-482)
		__debugbreak();
	if(ll!=-64)
		__debugbreak();

 	*pli=-30873;
	lk=li/251;
	ll=li%251;
	if(lk!=-123)
		__debugbreak();
	if(ll!=0)
		__debugbreak();

 	*pli=-30880;
	lk=li/251;
	ll=li%251;
	if(lk!=-123)
		__debugbreak();
	if(ll!=-7)
		__debugbreak();


	*pli=0x123456789ABCDEFLL;
	*plj=0xDECAFLL;
	lk=li/lj;
	ll=li%lj;
	
	if(lk!=0x14EAF62751LL)
		__debugbreak();
	if(ll!=0xB4190LL)
		__debugbreak();

	ll=li*lj;
	if(ll!=0xD159E26AF36D1A61LL)
		__debugbreak();


	pui=&ui;
	puj=&uj;
	
	*pui=0xFE80DECAU;
	*puj=0x0000ABCDU;
	uk=ui/uj;
	ul=ui%uj;
	
	if(uk!=0x17B3CU)
		__debugbreak();
	if(ul!=0x1BBEU)
		__debugbreak();

	lk=ui*uj;
	ll=((long long)ui)*((long long)uj);
	if(lk!=0x00000000E20355C2LL)
		__debugbreak();
	if(ll!=0x0000AACBE20355C2LL)
		__debugbreak();

//	__debugbreak();
}

int tk_shell_chksane_memset()
{
	long long tba[32];
	long long tbb[32];
	char *tb, *ts, *tb1;
	int *pi;
	int			i, j, k, l;

	tk_printf("CS Memset 0\n");

	l=0;

#if 1
	tb=(char *)tba;
	ts=(char *)tbb;
	pi=&l;
	
	if(((int)tb)&7)
		__debugbreak();
	if(((int)ts)&7)
		__debugbreak();

	*pi=0x1234567;
	l&=~3;
	if(l!=0x1234564)
		__debugbreak();

	j=-251-l;
	if(j!=-19088991)
		__debugbreak();

	tk_printf("CS Memset 1\n");

	strcpy(tb, "0123456789ABCDEFGHIJKLMNOPQRSTUV");
	memcpy(ts, tb+16, 16);

	tk_printf("CS Memset 2\n");

	if(memcmp(tb, tb, 16))
		__debugbreak();

	tk_printf("CS Memset 2B\n");

	if(!memcmp(tb, ts, 16))
		__debugbreak();

	tk_printf("CS Memset 3\n");

	*pi=1;
	memcpy(tb, ts, *pi);
	if(memcmp(tb, "G1234567", 8))
		__debugbreak();
	*pi=2;
	memcpy(tb, ts, *pi);
	if(memcmp(tb, "GH234567", 8))
		__debugbreak();
	*pi=3;
	memcpy(tb, ts, *pi);
	if(memcmp(tb, "GHI34567", 8))
		__debugbreak();
	*pi=4;
	memcpy(tb, ts, *pi);
	if(memcmp(tb, "GHIJ4567", 8))
		__debugbreak();
	*pi=5;
	memcpy(tb, ts, *pi);
	if(memcmp(tb, "GHIJK567", 8))
		__debugbreak();
	*pi=6;
	memcpy(tb, ts, *pi);
	if(memcmp(tb, "GHIJKL67", 8))
		__debugbreak();
	*pi=7;
	memcpy(tb, ts, *pi);
	if(memcmp(tb, "GHIJKLM7", 8))
		__debugbreak();

	tk_printf("CS Memset 4\n");

	*pi=8;
	memcpy(tb, ts, *pi);
	if(memcmp(tb, "GHIJKLMN89ABCDEF", 16))
		__debugbreak();

	strcpy(tb, "0123456789ABCDEF");
	*pi=1;
	memset(tb, ' ', *pi);
	if(memcmp(tb, " 1234567", 8))
		__debugbreak();
	*pi=3;
	memset(tb, ' ', *pi);
	if(memcmp(tb, "   34567", 8))
		__debugbreak();
	*pi=5;
	memset(tb, ' ', *pi);
	if(memcmp(tb, "     567", 8))
		__debugbreak();
	*pi=7;
	memset(tb, ' ', *pi);
	if(memcmp(tb, "       7", 8))
		__debugbreak();

	tk_printf("CS Memset 5\n");

	for(i=0; i<192; i++)
		ts[i]=i;

	tk_printf("CS Memset 6\n");

	for(j=0; j<16; j++)
	{
		tb1=tb+j;
		for(i=0; i<128; i++)
		{
			memcpy(tb1, ts, 192);
			memcpy(tb1, ts+16, i);
			if(memcmp(tb1, ts+16, i))
				__debugbreak();
			if(memcmp(tb1+i, ts+i, 16))
				__debugbreak();
		}
	}

	tk_printf("CS Memset 7\n");

	for(j=0; j<16; j++)
	{
		tb1=tb+j;
		for(i=0; i<128; i++)
		{
			memcpy(tb1, ts, 192);
			memset(tb1, i, i);
//			if(memcmp(tb1, ts+16, i))
//				__debugbreak();
			for(k=0; k<i; k++)
				if(tb1[i]!=i)
					__debugbreak();
			if(memcmp(tb1+i, ts+i, 16))
				__debugbreak();
		}
	}

	tk_printf("CS Memset 8\n");
#endif
}

int tk_shell_chksane_addr()
{
	TK_WadZBlock	*pwz0, *pwz1;
	TK_Wad4Lump		*pwl0, *pwl1;
	int i, j, k;
	
	k=0;
	pwz0=&k;
	pwl0=&k;
	j=251;
	pwz1=pwz0+j;
	pwl1=pwl0+j;
	k=pwz1-pwz0;
	
	if(k!=j)
		__debugbreak();

	k=pwl1-pwl0;

	if(k!=j)
		__debugbreak();

#if 1
	j=65521;
	pwz1=pwz0+j;
	pwl1=pwl0+j;
	k=pwz1-pwz0;
	
	if(k!=j)
		__debugbreak();

	k=pwl1-pwl0;

	if(k!=j)
		__debugbreak();
#endif
}

int tk_shell_chksane_switch_i(int num)
{
	int k;
	
	switch(num)
	{
		case 1: k=2; break;
		case 2: k=4; break;
		case 3: k=6; break;
		case 4: k=8; break;
		case 5: k=10; break;
		case 6: k=12; break;
		case 7: k=14; break;
		case 8: k=16; break;
		case 9: k=18; break;
		case 10: k=20; break;
		case 11: k=22; break;
		case 12: k=24; break;
		case 13: k=26; break;
		case 14: k=28; break;
		case 15: k=30; break;
		case 16: k=32; break;
	}
	
	if((num+num)!=k)
		__debugbreak();
	return(k);
}

int tk_shell_chksane_arith2()
{
	int				*pi, *pj;
	long long		*pli, *plj;
	unsigned int	*pui, *puj;

	long long		li, lj, lk, ll;
	unsigned int	ui, uj, uk, ul;
	int				i, j, k, l;

	i=0; j=0;
	ui=0; uj=0;

	pi=&i;
	pj=&j;

	pui=&ui;
	puj=&uj;

	*pi=12345678;
	*pj=12345678;

	if(i<j)
		__debugbreak();
	if(i>j)
		__debugbreak();
	if(!(i<=j))
		__debugbreak();
	if(!(i>=j))
		__debugbreak();

	if(i<=j)
		{ k++; }
	else
		{ __debugbreak(); }
	if(i>=j)
		{ k++; }
	else
		{ __debugbreak(); }
	
	
	tk_shell_chksane_switch_i(2);
	tk_shell_chksane_switch_i(3);
	tk_shell_chksane_switch_i(4);
	tk_shell_chksane_switch_i(5);
	tk_shell_chksane_switch_i(6);
	tk_shell_chksane_switch_i(7);
	tk_shell_chksane_switch_i(8);
	tk_shell_chksane_switch_i(11);
	tk_shell_chksane_switch_i(13);
}

const int scopetst_done=1;

int tk_shell_chksane_scope1()
{
	int scopetst_done;
	int i, v;
	
	scopetst_done=0;
	if(scopetst_done)
		{ __debugbreak(); }
	
	v=0;
	for(i=0; i<4; i++)
	{
		int v;
		v=i+1;
	}

	if(v!=0)
		{ __debugbreak(); }
}

int tk_shell_chksane()
{
	unsigned int	ui;
	int			i, j, k, l;

	tk_printf("CS B0\n");

	tk_shell_chksane_arith();
	tk_shell_chksane_arith2();

	tk_printf("CS B1\n");

	tk_shell_chksane_memset();

	tk_printf("CS B1-1\n");

	tk_shell_chksane_fmovs();

	tk_printf("CS B1-2\n");

	tk_shell_chksane_movtt();

	tk_printf("CS B1-3\n");

	tk_shell_chksane_srtmsk();

	tk_printf("CS B1-4\n");

	Sys_CheckSanityB();

	tk_printf("CS B2: CLZ\n");

	tk_shell_chksane_clz();

	tk_printf("CS B3: SIMD\n");

	tk_shell_chksane_simd();

	tk_printf("CS B4\n");
	
	Sys_CheckSanityC();

	tk_shell_chksane_addr();

	tk_printf("CS B5\n");

	sanity_glColor4f_pf = sanity_glColor4f;
	sanity_glColor4f_pf(1.0, 0.5, 1.0, 0.5);
	ui=sanity_tmpobj1.tkgl_begin_rgba;
	if(ui!=0x7FFF7FFFU)
	{
		__debugbreak();
	}
	
	tk_printf("CS B6\n");

	tk_shell_chksane_int128();
	tk_shell_chksane_scope1();

	tk_printf("CS B7\n");

//	tk_shell_chksane_var();
	tk_shell_chksane_bcd();

	tk_printf("CS B8\n");
	
#ifndef TK_IS_RBD
	tk_chksane_chkocr();

	tk_printf("CS B9\n");

	tk_shell_chksane_qsort();

#endif

	tk_printf("CS B10\n");
}
