// char tb_cwd[256];

void tk_shell_chksane_simd_asm();
void tk_shell_chksane_rgb5_asm();

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

	MOV		0xAD7844303F08ACE0, R6
	MOV		0x03B828BC7228D110, R7

	PMULU.LW	R4, R5, R18		|	PMULU.HW	R4, R5, R19
	CMPQEQ		R18, R6
	BREAK?F
	CMPQEQ		R19, R7
	BREAK?F

	MOV		0xAD7844303F08ACE0, R6
	MOV		0x03B828BC1A8E020E, R7

	PMULS.LW	R4, R5, R18		|	PMULS.HW	R4, R5, R19
	CMPQEQ		R18, R6
	BREAK?F
	CMPQEQ		R19, R7
	BREAK?F

	MOV		0xDEF09ABC56781234, R6
	MOV		0xF012BCDE789A3456, R7

	PSHUF.W		R4, 0x1B, R18	|	PSHUF.W		R5, 0x1B, R19
	CMPQEQ		R18, R6
	BREAK?F
	CMPQEQ		R19, R7
	BREAK?F

	MOV		0x9ABCDEF0BCDEF012, R6
	MOV		0x123456783456789A, R7

	MOVLD	R4, R5, R18
	MOVHD	R4, R5, R19
	CMPQEQ		R18, R6
	BREAK?F
	CMPQEQ		R19, R7
	BREAK?F

	MOV		0x12345678BCDEF012, R6
	MOV		0x9ABCDEF03456789A, R7

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

	PADD.F		R4, R5, R18
	PSUB.F		R4, R5, R19
//	PMUL.F		R4, R5, R20
	CMPQEQ		R18, R6
	BREAK?F
	CMPQEQ		R19, R7
	BREAK?F

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

	PADDX.F		R20, R22, R18
	CMPQEQ		R18, R6
	BREAK?F
	CMPQEQ		R19, R6
	BREAK?F

	XOR		R18, R18
	XOR		R19, R19

	PSUBX.F		R20, R22, R18
	CMPQEQ		R18, R7
	BREAK?F
	CMPQEQ		R19, R7
	BREAK?F

	MOV		0x0717153D18247A72, R16

	PMULX.F		R20, R22, R18
	CMPQEQ		R18, R16
	BREAK?F
	CMPQEQ		R19, R16
	BREAK?F

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

	CMPQEQ		R18, R4
	BREAK?F
	CMPQEQ		R19, R5
	BREAK?F

//	MOV.X		R6, (GBR, 384)
	MOV.X		R6, (GBR, 6144)
	NOP
//	MOV.X		(GBR, 384), R18
	MOV.X		(GBR, 6144), R18

	CMPQEQ		R18, R6
	BREAK?F
	CMPQEQ		R19, R7
	BREAK?F


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

	.L4A1:
	CMPQEQ		R4, R21
	BREAK?F
	CMPQEQ		R5, R22
	BREAK?F


	MOV			0xFFAA55009234ABCD, R16

	MOV			0x404021218C8CA5A5, R20
	MOV			0x20205252F7F76B6B, R23

	MOV			0, R2
	MOV			15, R3
	BLKUTX2		R16, R2, R4
	BLKUTX2		R16, R3, R5

	.L4B:
	CMPQEQ		R4, R20
	BREAK?F
	CMPQEQ		R5, R23
	BREAK?F


	/* Test FLDCH Immed */
	MOV			0x40159C0000000000, R20
	MOV			0x40E2680000000000, R21

	FLDCH	0x4567, R4
	FLDCH	0x789A, R5

	CMPQEQ		R4, R20
	BREAK?F
	CMPQEQ		R5, R21
	BREAK?F


	RTSU

};

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
	
//	mv0=__m128_float4(1.0, 2.0, 3.0, 5.0);
	mv0=(__vec4f){1.0, 2.0, 3.0, 5.0};
	
	tk_shell_fv0_gbl = mv0;
	
//	__hint_cc_dbgbreak();
	
	fv0=mv0;
	fv1=fv0+fv0;
	fv2=fv1*fv0;
	
//	__debugbreak();

	f0=fv1.x;
	f1=fv1.y;
	f2=fv1.z;
	f3=fv1.w;

//	__debugbreak();
	
//	tk_printf("SIMD A0: %f %f %f %f\n", fv1.x, fv1.y, fv1.z, fv1.w);
	tk_printf("SIMD A0: %f %f\n", fv1.x, fv1.y);
	tk_printf("SIMD A0: %f %f\n", fv1.z, fv1.w);

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
}

long long __int32_dmuls(int a, int b);

void Sys_CheckSanityC(void)
{
	double f, g, h;
	long long la, lb, lc;
	int a, b, c;
	
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

//	cr=(int)(red*255);
//	cg=(int)(green*255);
//	cb=(int)(blue*255);
//	ca=(int)(alpha*255);
//	cr=tkra_clamp255(cr);
//	cg=tkra_clamp255(cg);
//	cb=tkra_clamp255(cb);
//	ca=tkra_clamp255(ca);

	cr=tkra_clamp255((int)(red*255));
	cg=tkra_clamp255((int)(green*255));
	cb=tkra_clamp255((int)(blue*255));
	ca=tkra_clamp255((int)(alpha*255));
	px=(ca<<24)|(cr<<16)|(cg<<8)|cb;

//	return(px);

	ctx=sanity_getCtx();
//	ctx=TKRA_GetCurrentContext();
	ctx->tkgl_begin_rgba=px;
//	tkra_glColor4ubI(red*255.0, green*255.0, blue*255.0, alpha*255.0);
}

#include "tk_shell_chksvar.c"

int tk_shell_chksane()
{
	long long tba[32];
	long long tbb[32];
	char *tb, *ts, *tb1;

	int				*pi, *pj;
	long long		*pli, *plj;
	unsigned int	*pui, *puj;

	long long		li, lj, lk, ll;
	unsigned int	ui, uj, uk, ul;
	int				i, j, k, l;
	
	pi=&i;
	pj=&j;
	
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

	pli=&li;
	plj=&lj;
	
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

#if 0
	tb=(char *)tba;
	ts=(char *)tbb;
	
	if(((int)tb)&7)
		__debugbreak();
	if(((int)ts)&7)
		__debugbreak();
		
	strcpy(tb, "0123456789ABCDEFGHIJKLMNOPQRSTUV");
	memcpy(ts, tb+16, 16);

	if(memcmp(tb, tb, 16))
		__debugbreak();
	if(!memcmp(tb, ts, 16))
		__debugbreak();

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

	for(i=0; i<192; i++)
		ts[i]=i;

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
#endif

	Sys_CheckSanityB();

	tk_shell_chksane_clz();
	tk_shell_chksane_simd();
	
	Sys_CheckSanityC();

	tk_printf("CS B5\n");

	sanity_glColor4f_pf = sanity_glColor4f;
	sanity_glColor4f_pf(1.0, 0.5, 1.0, 0.5);
	ui=sanity_tmpobj1.tkgl_begin_rgba;
	if(ui!=0x7FFF7FFFU)
	{
		__debugbreak();
	}
	
	tk_printf("CS B6\n");

	tk_shell_chksane_var();

	tk_printf("CS B7\n");

	tk_shell_chksane_int128();

	tk_printf("CS B8\n");
}
