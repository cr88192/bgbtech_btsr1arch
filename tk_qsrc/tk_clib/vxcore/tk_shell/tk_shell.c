int TKSH_InitCmds(void);

// char tb_cwd[256];

void tk_shell_chksane_simd_asm();

__asm {
tk_shell_chksane_simd_asm:
	PUSH	SR

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

	POP SR
	RTSU
};

__m128 __m128_float4(float f0, float f1, float f2, float f3);

int tk_shell_chksane_simd()
{
	__m128 mv0;
	__vec4f fv0, fv1, fv2;
	__quatf qv0, qv1, qv2, qv3;
	double	f0, f1, f2, f3;
	
	tk_shell_chksane_simd_asm();
	
//	mv0=__m128_float4(1.0, 2.0, 3.0, 5.0);
	mv0=(__vec4f){1.0, 2.0, 3.0, 5.0};
	
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

int tk_shell_chksane()
{
	char tb[64];

	int *pi, *pj;
	int i, j, k, l;
	
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
		
	strcpy(tb, "0123456789ABCDEF");
	*pi=1;
	memcpy(tb, "GHIJKLMN", *pi);
	if(memcmp(tb, "G1234567", 8))
		__debugbreak();
	*pi=3;
	memcpy(tb, "GHIJKLMN", *pi);
	if(memcmp(tb, "GHI34567", 8))
		__debugbreak();
	*pi=5;
	memcpy(tb, "GHIJKLMN", *pi);
	if(memcmp(tb, "GHIJK567", 8))
		__debugbreak();
	*pi=7;
	memcpy(tb, "GHIJKLMN", *pi);
	if(memcmp(tb, "GHIJKLM7", 8))
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

	tk_shell_chksane_simd();
}

int main(int argc, char *argv[])
{
	TKPE_TaskInfo *task;
	char tb_cwd[256];
	char tbuf[256];

	tk_shell_chksane();

	tk_con_reset();

	task=TK_AllocNewTask();
	TK_SetCurrentTask(task);
	
	TKSH_InitCmds();

	strcpy(tb_cwd, "/");
	TK_Env_SetCwd(tb_cwd);

	TKSH_TryLoad("autoexec.pf", NULL);

	while(1)
	{
		TK_Env_GetCwd(tb_cwd, 256);
//		tk_con_chkreset();
		tk_puts(tb_cwd);
		tk_puts("$ ");
//		tk_puts("$  \b");
		tk_gets(tbuf);
		TKSH_ExecCmd(tbuf);
	}

	puts("Loop Terminate\n");
}
