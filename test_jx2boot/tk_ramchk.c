#define TK_RAMCHK_BASE	0x01000000U
//#define TK_RAMCHK_LIM	0x08000000U
#define TK_RAMCHK_LIM	0x02000000U

#define TK_RAMCHK_LIM_HI	0x17000000U

int tk_ram_kib;

void TK_FlushCacheL1D(void *pptr);
void TK_FlushCacheL1D_INVDCA(void);
void TK_FlushCacheL1D_INVL2(void);
void TK_FlushCacheL1D_ReadBuf(void *ptr, int sz);

u64 tk_ramrng_sum1, tk_ramrng_sum2, tk_ramrng_sumx;

int TK_RamRngUpd()
{
	tk_ramrng_sum1+=TK_GetTimeUs();
	tk_ramrng_sum2+=tk_ramrng_sum1;
	tk_ramrng_sumx=tk_ramrng_sum1^tk_ramrng_sum2;
}

int TK_RamChk()
{
	u32 *ct, *cts, *cte, *ct1e;
	int i, j, k;

	TK_RamRngUpd();
	puts("RAM Check: Begin\n");
	TK_RamRngUpd();
	
	cts=(u32 *)TK_RAMCHK_BASE;
	cte=(u32 *)TK_RAMCHK_LIM;
	ct=cts;

//	__debugbreak();

#if 1
	while(ct<cte)
	{
//		printf("DRAM %d\r", (ct-cts)<<2);
//		ct1e=ct+16384;
		ct1e=ct+262144;
//		ct1e=ct+1024;
		while(ct<ct1e)
		{
			ct[0]=(u32)ct;
			ct[1]=0x5555AAAAU;
//			ct+=4;
			ct+=256;
//			__debugbreak();
		}
	}
#endif

	TK_RamRngUpd();

//	TK_FlushCacheL1D(cts);
	TK_FlushCacheL1D_INVDCA();
	TK_FlushCacheL1D_INVL2();

	TK_RamRngUpd();

//	TK_FlushCacheL1D_ReadBuf(0x02000000, 262144);
	TK_FlushCacheL1D_ReadBuf(0x02700000, 524288);

	TK_RamRngUpd();

//	putc('\n');

//	__debugbreak();

#if 0
	while(ct<cte)
	{
		ct[0]=(u32)ct;
		ct[1]=(u32)ct;
		ct[2]=(u32)ct;
		ct[3]=(u32)ct;
		ct+=4;
	}
#endif

#if 0
	ct=(u32 *)TK_RAMCHK_BASE;
	cte=(u32 *)TK_RAMCHK_LIM;
	
	while(ct<cte)
	{
		if(ct[0]!=((u32)ct))
			break;
		ct+=4;
	}
#endif

#if 1
	ct=(u32 *)TK_RAMCHK_BASE;
	cte=(u32 *)TK_RAMCHK_LIM;
	while(ct<cte)
	{
//		printf("DRAM %d\r", (ct-cts)<<2);
		ct1e=ct+262144;
		while(ct<ct1e)
		{
			if(ct[0]!=((u32)ct))
			{
//				__debugbreak();
				break;
			}
			ct+=256;
		}
		if(ct<ct1e)
			break;
	}
#endif

	if(ct<cte)
	{
		printf("RAM Check Fail, A=%p, expect=%X got=%X-%X",
			ct, ((u32)ct), ct[0], ct[1]);
		__debugbreak();
		return(-1);
	}

	TK_RamRngUpd();
	
#if 1
	cts=(u32 *)TK_RAMCHK_BASE;
	cte=(u32 *)TK_RAMCHK_LIM_HI;
	ct=cts;

	while(ct<cte)
	{
		ct[0]=0;
		ct=ct+262144;
	}

	ct=cts;
	
	ct[0]=0x55AA55AAU;
	ct[1]=0x55AA55AAU;
	ct[2]=0x12345678U;
	ct[3]=0xFEDCBA98U;
	
//	TK_FlushCacheL1D(NULL);
	TK_FlushCacheL1D_INVDCA();
	TK_FlushCacheL1D_INVL2();

//	TK_FlushCacheL1D_ReadBuf(0x02000000, 262144);
	TK_FlushCacheL1D_ReadBuf(0x02700000, 524288);

	ct[0]=0x55AA55AAU;
	ct[1]=0x55AA55AAU;
	ct[2]=0x12345678U;
	ct[3]=0xFEDCBA98U;

	TK_FlushCacheL1D_INVDCA();
	TK_FlushCacheL1D_INVL2();

//	TK_FlushCacheL1D_ReadBuf(0x02000000, 262144);
	TK_FlushCacheL1D_ReadBuf(0x02700000, 524288);

	TK_RamRngUpd();
	
//	k=ct[0];

//	TK_FlushCacheL1D_INVL2();
//	TK_FlushCacheL1D_INVDCA();
//	k=ct[0];
//	TK_FlushCacheL1D_INVL2();
//	TK_FlushCacheL1D_INVDCA();
//	k=ct[0];


	ct=ct+262144;
	while(ct<cte)
	{
		j=((ct-cts)<<2)>>10;
//		printf("DRAM %dK\r", j);
		if(	(ct[0]==0x55AA55AAU) &&
			(ct[1]==0x55AA55AAU) &&
			(ct[2]==0x12345678U) &&
			(ct[3]==0xFEDCBA98U))
				break;

//			(ct[2]==0x12345678U) )

//		if(	(ct[0]==0x55AA55AAU) &&
//			(ct[1]==0x55AA55AAU))
//		if(ct[0]==0x55AA55AAU)
//		{
//			printf("\n");
//			puts("\n");
//			break;
//		}
		ct=ct+262144;
	}
	
	j=((ct-cts)<<2)>>10;
	tk_ram_kib=j;

	printf("DRAM %dK\n", j);
#endif
	
	TK_RamRngUpd();
	
	puts("RAM Check: OK\n");
	
	TK_RamRngUpd();
	
	return(0);
}

#ifndef JX2UC

void memcpy_movx_test(void *dst, void *src, int sz);
void memset_movx_test(void *dst, int val, int sz);
void memcpy_fixed128_test(void *dst, void *src, int sz);
void memcpy_fixed512_test(void *dst, void *src, int sz);
void memset_fixed128_test(void *dst, int val, int sz);
void memset_fixed512_test(void *dst, int val, int sz);

void memload_movx_test(void *dst, int val, int sz);
void memload_fixed512_test(void *dst, int val, int sz);

#ifdef __BJX2__
__asm {
memcpy_movx_test:
#if 0
	CMP/GE	64, R6
	BF		.L1
	.L0:
	MOV.X	(R5,  0), R16
	MOV.X	(R5, 16), R18
	MOV.X	(R5, 32), R20
	MOV.X	(R5, 48), R22
	ADD		64, R5		|	ADD		-64, R6
	MOV.X	R16, (R4,  0)
	MOV.X	R18, (R4, 16)
	MOV.X	R20, (R4, 32)
	MOV.X	R22, (R4, 48)
	ADD		64, R4		|	CMP/GE	64, R6
	BT		.L0
	.L1:
	RTSU
#endif

#if 1
	CMP/GE	128, R6
	BF		.L1
	.L0:

	MOV.X	(R5,  0), R16
	MOV.X	(R5, 16), R18
	MOV.X	(R5, 32), R20
	MOV.X	(R5, 48), R22
	ADD		64, R5
	MOV.X	R16, (R4,  0)
	MOV.X	R18, (R4, 16)
	MOV.X	R20, (R4, 32)
	MOV.X	R22, (R4, 48)
	ADD		64, R4

	MOV.X	(R5,  0), R16
	MOV.X	(R5, 16), R18
	MOV.X	(R5, 32), R20
	MOV.X	(R5, 48), R22
	ADD		64, R5		|	ADD		-128, R6
	MOV.X	R16, (R4,  0)
	MOV.X	R18, (R4, 16)
	MOV.X	R20, (R4, 32)
	MOV.X	R22, (R4, 48)
	ADD		64, R4		|	CMP/GE	128, R6
	BT		.L0
	.L1:
	RTSU
#endif

#if 0
	CMP/GE	32, R6
	BF		.L1
	.L0:
	MOV.X	(R5,  0), R20
	MOV.X	(R5, 16), R22
	ADD		32, R5		|	ADD		-32, R6
	MOV.X	R20, (R4,  0)
	MOV.X	R22, (R4, 16)
	ADD		32, R4		|	CMP/GE	32, R6
	BT		.L0
	.L1:

	CMP/GE	8, R6
	BF		.L3
	.L2:
	ADD		-8, R6		|	MOV.Q	(R5,  0), R20
	ADD		8, R5		|	MOV.Q	R20, (R4,  0)
	ADD		8, R4		|	CMP/GE	8, R6
	BT		.L2
	.L3:

	RTSU
#endif

memcpy_fixed128_test:
	MOV.X	(R5,   0), R16
	MOV.X	(R5,  16), R18
	MOV.X	(R5,  32), R20
	MOV.X	(R5,  48), R22
	MOV.X	R16, (R4,   0)
	MOV.X	R18, (R4,  16)
	MOV.X	R20, (R4,  32)
	MOV.X	R22, (R4,  48)
	MOV.X	(R5,  64), R16
	MOV.X	(R5,  80), R18
	MOV.X	(R5,  96), R20
	MOV.X	(R5, 112), R22
	MOV.X	R16, (R4,  64)
	MOV.X	R18, (R4,  80)
	MOV.X	R20, (R4,  96)
	MOV.X	R22, (R4, 112)
	RTSU

memcpy_fixed512_test:
	MOV.X	(R5,   0), R16
	MOV.X	(R5,  16), R18
	MOV.X	(R5,  32), R20
	MOV.X	(R5,  48), R22
	MOV.X	R16, (R4,   0)
	MOV.X	R18, (R4,  16)
	MOV.X	R20, (R4,  32)
	MOV.X	R22, (R4,  48)
	MOV.X	(R5,  64), R16
	MOV.X	(R5,  80), R18
	MOV.X	(R5,  96), R20
	MOV.X	(R5, 112), R22
	MOV.X	R16, (R4,  64)
	MOV.X	R18, (R4,  80)
	MOV.X	R20, (R4,  96)
	MOV.X	R22, (R4, 112)
	ADD		128, R4			|	ADD		128, R5

	MOV.X	(R5,   0), R16
	MOV.X	(R5,  16), R18
	MOV.X	(R5,  32), R20
	MOV.X	(R5,  48), R22
	MOV.X	R16, (R4,   0)
	MOV.X	R18, (R4,  16)
	MOV.X	R20, (R4,  32)
	MOV.X	R22, (R4,  48)
	MOV.X	(R5,  64), R16
	MOV.X	(R5,  80), R18
	MOV.X	(R5,  96), R20
	MOV.X	(R5, 112), R22
	MOV.X	R16, (R4,  64)
	MOV.X	R18, (R4,  80)
	MOV.X	R20, (R4,  96)
	MOV.X	R22, (R4, 112)
	ADD		128, R4			|	ADD		128, R5

	MOV.X	(R5,   0), R16
	MOV.X	(R5,  16), R18
	MOV.X	(R5,  32), R20
	MOV.X	(R5,  48), R22
	MOV.X	R16, (R4,   0)
	MOV.X	R18, (R4,  16)
	MOV.X	R20, (R4,  32)
	MOV.X	R22, (R4,  48)
	MOV.X	(R5,  64), R16
	MOV.X	(R5,  80), R18
	MOV.X	(R5,  96), R20
	MOV.X	(R5, 112), R22
	MOV.X	R16, (R4,  64)
	MOV.X	R18, (R4,  80)
	MOV.X	R20, (R4,  96)
	MOV.X	R22, (R4, 112)
	ADD		128, R4			|	ADD		128, R5

	MOV.X	(R5,   0), R16
	MOV.X	(R5,  16), R18
	MOV.X	(R5,  32), R20
	MOV.X	(R5,  48), R22
	MOV.X	R16, (R4,   0)
	MOV.X	R18, (R4,  16)
	MOV.X	R20, (R4,  32)
	MOV.X	R22, (R4,  48)
	MOV.X	(R5,  64), R16
	MOV.X	(R5,  80), R18
	MOV.X	(R5,  96), R20
	MOV.X	(R5, 112), R22
	MOV.X	R16, (R4,  64)
	MOV.X	R18, (R4,  80)
	MOV.X	R20, (R4,  96)
	MOV.X	R22, (R4, 112)
//	ADD		128, R4			|	ADD		128, R5

	RTSU


memset_movx_test:
#if 0
	PSHUF.B	R5, 0x00, R16
	MOVLD	R16, R16, R18	|	MOVLD	R16, R16, R19
	CMP/GE	64, R6
	BF		.L1
	.L0:
	ADD		-64, R6
	MOV.X	R18, (R4,  0)
	MOV.X	R18, (R4, 16)
	MOV.X	R18, (R4, 32)
	MOV.X	R18, (R4, 48)
	ADD		64, R4		|	CMP/GE	64, R6
	BT		.L0
	.L1:
	RTSU
#endif

#if 1
	PSHUF.B	R5, 0x00, R16
	MOVLD	R16, R16, R18	|	MOVLD	R16, R16, R19
	CMP/GE	128, R6
	BF		.L1
	.L0:
	ADD		-128, R6
	MOV.X	R18, (R4,   0)
	MOV.X	R18, (R4,  16)
	MOV.X	R18, (R4,  32)
	MOV.X	R18, (R4,  48)
	MOV.X	R18, (R4,  64)
	MOV.X	R18, (R4,  80)
	MOV.X	R18, (R4,  96)
	MOV.X	R18, (R4, 112)
	ADD		128, R4		|	CMP/GE	128, R6
	BT		.L0
	.L1:
	RTSU
#endif

memset_fixed128_test:
	MOV		0, R18 			|	MOV		0, R19
//	PSHUF.B	R5, 0x00, R16
//	MOVLD	R16, R16, R18	|	MOVLD	R16, R16, R19
	MOV.X	R18, (R4,   0)
	MOV.X	R18, (R4,  16)
	MOV.X	R18, (R4,  32)
	MOV.X	R18, (R4,  48)
	MOV.X	R18, (R4,  64)
	MOV.X	R18, (R4,  80)
	MOV.X	R18, (R4,  96)
	MOV.X	R18, (R4, 112)
	RTSU

memset_fixed512_test:
	MOV		0, R18 			|	MOV		0, R19
//	PSHUF.B	R5, 0x00, R16
//	MOVLD	R16, R16, R18	|	MOVLD	R16, R16, R19
	MOV.X	R18, (R4,   0)
	MOV.X	R18, (R4,  16)
	MOV.X	R18, (R4,  32)
	MOV.X	R18, (R4,  48)
	MOV.X	R18, (R4,  64)
	MOV.X	R18, (R4,  80)
	MOV.X	R18, (R4,  96)
	MOV.X	R18, (R4, 112)
	ADD		128, R4
	MOV.X	R18, (R4,   0)
	MOV.X	R18, (R4,  16)
	MOV.X	R18, (R4,  32)
	MOV.X	R18, (R4,  48)
	MOV.X	R18, (R4,  64)
	MOV.X	R18, (R4,  80)
	MOV.X	R18, (R4,  96)
	MOV.X	R18, (R4, 112)
	ADD		128, R4
	MOV.X	R18, (R4,   0)
	MOV.X	R18, (R4,  16)
	MOV.X	R18, (R4,  32)
	MOV.X	R18, (R4,  48)
	MOV.X	R18, (R4,  64)
	MOV.X	R18, (R4,  80)
	MOV.X	R18, (R4,  96)
	MOV.X	R18, (R4, 112)
	ADD		128, R4
	MOV.X	R18, (R4,   0)
	MOV.X	R18, (R4,  16)
	MOV.X	R18, (R4,  32)
	MOV.X	R18, (R4,  48)
	MOV.X	R18, (R4,  64)
	MOV.X	R18, (R4,  80)
	MOV.X	R18, (R4,  96)
	MOV.X	R18, (R4, 112)
//	ADD		128, R4
	RTSU


memload_movx_test:
#if 1
	CMP/GE	128, R6
	BF		.L1
	.L0:
	ADD		-128, R6
	MOV.X	(R4,   0), R16
	MOV.X	(R4,  16), R18
	MOV.X	(R4,  32), R20
	MOV.X	(R4,  48), R22
	MOV.X	(R4,  64), R16
	MOV.X	(R4,  80), R18
	MOV.X	(R4,  96), R20
	MOV.X	(R4, 112), R22
	ADD		128, R4		|	CMP/GE	128, R6
	BT		.L0
	.L1:
	RTSU
#endif

memload_fixed512_test:
	MOV.X	(R4,   0), R16
	MOV.X	(R4,  16), R18
	MOV.X	(R4,  32), R20
	MOV.X	(R4,  48), R22
	MOV.X	(R4,  64), R16
	MOV.X	(R4,  80), R18
	MOV.X	(R4,  96), R20
	MOV.X	(R4, 112), R22
	ADD		128, R4
	MOV.X	(R4,   0), R16
	MOV.X	(R4,  16), R18
	MOV.X	(R4,  32), R20
	MOV.X	(R4,  48), R22
	MOV.X	(R4,  64), R16
	MOV.X	(R4,  80), R18
	MOV.X	(R4,  96), R20
	MOV.X	(R4, 112), R22
	ADD		128, R4
	MOV.X	(R4,   0), R16
	MOV.X	(R4,  16), R18
	MOV.X	(R4,  32), R20
	MOV.X	(R4,  48), R22
	MOV.X	(R4,  64), R16
	MOV.X	(R4,  80), R18
	MOV.X	(R4,  96), R20
	MOV.X	(R4, 112), R22
	ADD		128, R4
	MOV.X	(R4,   0), R16
	MOV.X	(R4,  16), R18
	MOV.X	(R4,  32), R20
	MOV.X	(R4,  48), R22
	MOV.X	(R4,  64), R16
	MOV.X	(R4,  80), R18
	MOV.X	(R4,  96), R20
	MOV.X	(R4, 112), R22
//	ADD		128, R4
	RTSU


memcpy_movb_test:
#if 0
	CMP/GE	64, R6
	BF		.L1
	.L0:
	MOV.B	(R5, 0), R16
	MOV.B	(R5, 1), R18
	MOV.B	(R5, 2), R20
	MOV.B	(R5, 3), R22
	ADD		4, R5		|	ADD		-4, R6
	MOV.B	R16, (R4, 0)
	MOV.B	R18, (R4, 1)
	MOV.B	R20, (R4, 2)
	MOV.B	R22, (R4, 3)
	ADD		4, R4		|	CMP/GE	4, R6
	BT		.L0
	.L1:
	RTSU
#endif
}
#endif

#ifdef __BJX2__
int TK_RamBench()
{
	byte *cs, *ct;
//	int ci, cj, ck;
	long long ci, cj;
	u32 csum1, csum2;
	int ck;
	int tf, th, tl;
	int i, j, k;

	puts("RAM Bench:\n");

	cs=(u32 *)TK_RAMCHK_BASE;
//	ct=cs+0x123450;
	ct=cs+0x123480;
//	ci = TK_GetTimeMs();
	ci = TK_GetTimeUs();

//	memcpy(ct, cs, 1<<20);
//	memcpy(ct, cs, 1<<17);
//	memcpy_movx_test(ct, cs, 1<<17);
	memcpy_movx_test(ct, cs, 1<<18);

//	cj = TK_GetTimeMs();
	cj = TK_GetTimeUs();
	ck = cj-ci;
	if(ck>0)
	{
//		printf("memcpy: %dms, %d MB/s\n", ck, 1000/ck);

//		tf=25600/ck;
//		tf=12800/ck;
//		tf=12800000/ck;
		tf=25600000/ck;
		th=tf/100;
		tl=tf-(th*100);
//		printf("memcpy (DRAM): %dus, %d.%02d MB/s\n", ck, th, tl);
		printf("%s: %dus, %d.%02d MB/s\n", "memcpy (DRAM)", ck, th, tl);
	}


//	ci = TK_GetTimeMs();
	ci = TK_GetTimeUs();

#if 0
//	memcpy(ct, cs, 1<<20);
//	for(i=0; i<8192; i++)
	for(i=0; i<4096; i++)
	{
//		memcpy(ct, cs, 128);
//		memcpy_fixed128_test(ct, cs, 128);
		memcpy_fixed512_test(ct, cs, 512);
	}
#endif

#if 1
	for(i=0; i<1024; i++)
	{
		memcpy_fixed512_test(ct, cs, 512);
		memcpy_fixed512_test(ct, cs, 512);
		memcpy_fixed512_test(ct, cs, 512);
		memcpy_fixed512_test(ct, cs, 512);
	}
#endif

//	cj = TK_GetTimeMs();
	cj = TK_GetTimeUs();
	ck = cj-ci;
	if(ck>0)
	{
//		tf=102400/ck;
//		tf=204800/ck;
		tf=204800000/ck;
//		tf=12800/ck;
		th=tf/100;
		tl=tf-(th*100);
//		printf("memcpy (L1): %dus, %d.%02d MB/s\n", ck, th, tl);
		printf("%s: %dus, %d.%02d MB/s\n", "memcpy (L1)", ck, th, tl);
	}


//	memcpy_movx_test(ct, cs, 16384);		//Pre-warm L2
	memcpy_movx_test(ct, cs, 49152);		//Pre-warm L2

//	ci = TK_GetTimeMs();
	ci = TK_GetTimeUs();

//	memcpy(ct, cs, 1<<20);
//	for(i=0; i<16; i++)
//	for(i=0; i<4; i++)
	for(i=0; i<8; i++)
	{
//		memcpy(ct, cs, 16384);
//		memcpy_movx_test(ct, cs, 16384);
//		memcpy_movx_test(ct, cs, 49152);
		memcpy_movx_test(ct, cs, 32768);
	}

//	cj = TK_GetTimeMs();
	cj = TK_GetTimeUs();
	ck = cj-ci;
	if(ck>0)
	{
//		tf=25600/ck;
		tf=25600000/ck;
//		tf=76800000/ck;
//		tf=19200000/ck;
//		tf=12800/ck;
		th=tf/100;
		tl=tf-(th*100);
//		printf("memcpy (L2): %dus, %d.%02d MB/s\n", ck, th, tl);
		printf("%s: %dus, %d.%02d MB/s\n", "memcpy (L2)", ck, th, tl);
	}


#if 1
//	ci = TK_GetTimeMs();
	ci = TK_GetTimeUs();

#if 0
//	memcpy(ct, cs, 1<<20);
//	for(i=0; i<8192; i++)
	for(i=0; i<4096; i++)
	{
//		memcpy(ct, cs, 128);
//		memcpy_fixed128_test(ct, cs, 128);
		memcpy_fixed512_test(ct+8, cs+8, 512);
	}
#endif

#if 1
	for(i=0; i<1024; i++)
	{
		memcpy_fixed512_test(ct+8, cs+8, 512);
		memcpy_fixed512_test(ct+8, cs+8, 512);
		memcpy_fixed512_test(ct+8, cs+8, 512);
		memcpy_fixed512_test(ct+8, cs+8, 512);
	}
#endif

//	cj = TK_GetTimeMs();
	cj = TK_GetTimeUs();
	ck = cj-ci;
	if(ck>0)
	{
//		tf=102400/ck;
//		tf=204800/ck;
		tf=204800000/ck;
//		tf=12800/ck;
		th=tf/100;
		tl=tf-(th*100);
//		printf("memcpy (L1, Misal): %dus, %d.%02d MB/s\n", ck, th, tl);
		printf("%s: %dus, %d.%02d MB/s\n", "memcpy (L1, Misal)", ck, th, tl);
	}
#endif


#if 1
//	ci = TK_GetTimeMs();
	ci = TK_GetTimeUs();

	memset_movx_test(ct, cs, 1<<18);

//	cj = TK_GetTimeMs();
	cj = TK_GetTimeUs();
	ck = cj-ci;
	if(ck>0)
	{
		tf=25600000/ck;
		th=tf/100;
		tl=tf-(th*100);
//		printf("memset (DRAM): %dus, %d.%02d MB/s\n", ck, th, tl);
		printf("%s: %dus, %d.%02d MB/s\n", "memset (DRAM)", ck, th, tl);
	}
#endif

//	ci = TK_GetTimeMs();
	ci = TK_GetTimeUs();

#if 0
//	memcpy(ct, cs, 1<<20);
	for(i=0; i<4096; i++)
//	for(i=0; i<8192; i++)
//	for(i=0; i<16384; i++)
	{
//		memcpy(ct, cs, 128);
//		memset_fixed128_test(ct, 0, 128);
		memset_fixed512_test(ct, 0, 512);
	}
#endif

#if 1
	for(i=0; i<1024; i++)
	{
		memset_fixed512_test(ct, 0, 512);
		memset_fixed512_test(ct, 0, 512);
		memset_fixed512_test(ct, 0, 512);
		memset_fixed512_test(ct, 0, 512);
	}
#endif

//	cj = TK_GetTimeMs();
	cj = TK_GetTimeUs();
	ck = cj-ci;
	if(ck>0)
	{
//		tf=51200/ck;
//		tf=102400/ck;
//		tf=204800/ck;
		tf=204800000/ck;
//		tf=12800/ck;
		th=tf/100;
		tl=tf-(th*100);
//		printf("memset (L1): %dus, %d.%02d MB/s\n", ck, th, tl);
		printf("%s: %dus, %d.%02d MB/s\n", "memset (L1)", ck, th, tl);
	}


//	memset_movx_test(ct, 0, 16384);		//Pre-warm L2
	memset_movx_test(ct, 0, 49152);		//Pre-warm L2

//	ci = TK_GetTimeMs();
	ci = TK_GetTimeUs();

//	memcpy(ct, cs, 1<<20);
	for(i=0; i<16; i++)
	{
//		memcpy(ct, cs, 16384);
//		memset_movx_test(ct, 0, 16384);
		memset_movx_test(ct, 0, 49152);
	}

//	cj = TK_GetTimeMs();
	cj = TK_GetTimeUs();
	ck = cj-ci;
	if(ck>0)
	{
//		tf=25600/ck;
//		tf=25600000/ck;
		tf=76800000/ck;
//		tf=12800/ck;
		th=tf/100;
		tl=tf-(th*100);
//		printf("memset (L2): %dus, %d.%02d MB/s\n", ck, th, tl);
		printf("%s: %dus, %d.%02d MB/s\n", "memset (L2)", ck, th, tl);
	}

#if 1


#if 1
//	ci = TK_GetTimeMs();
	ci = TK_GetTimeUs();

	memload_movx_test(ct, cs, 1<<18);
	
//	__debugbreak();

//	cj = TK_GetTimeMs();
	cj = TK_GetTimeUs();
	ck = cj-ci;
	if(ck>0)
	{
		tf=25600000/ck;
		th=tf/100;
		tl=tf-(th*100);
//		printf("memload (DRAM): %dus, %d.%02d MB/s\n", ck, th, tl);
		printf("%s: %dus, %d.%02d MB/s\n", "memload (DRAM)", ck, th, tl);
	}
#endif

//	ci = TK_GetTimeMs();
	ci = TK_GetTimeUs();

#if 0
//	memcpy(ct, cs, 1<<20);
	for(i=0; i<4096; i++)
//	for(i=0; i<8192; i++)
//	for(i=0; i<16384; i++)
	{
//		memcpy(ct, cs, 128);
//		memset_fixed128_test(ct, 0, 128);
		memload_fixed512_test(ct, 0, 512);
	}
#endif

#if 1
	for(i=0; i<1024; i++)
	{
		memload_fixed512_test(ct, 0, 512);
		memload_fixed512_test(ct, 0, 512);
		memload_fixed512_test(ct, 0, 512);
		memload_fixed512_test(ct, 0, 512);
	}
#endif

//	cj = TK_GetTimeMs();
	cj = TK_GetTimeUs();
	ck = cj-ci;
	if(ck>0)
	{
//		tf=51200/ck;
//		tf=102400/ck;
//		tf=204800/ck;
		tf=204800000/ck;
//		tf=12800/ck;
		th=tf/100;
		tl=tf-(th*100);
//		printf("memload (L1): %dus, %d.%02d MB/s\n", ck, th, tl);
		printf("%s: %dus, %d.%02d MB/s\n", "memload (L1)", ck, th, tl);
	}


//	memset_movx_test(ct, 0, 16384);		//Pre-warm L2
	memload_movx_test(ct, 0, 49152);	//Pre-warm L2

//	ci = TK_GetTimeMs();
	ci = TK_GetTimeUs();

//	memcpy(ct, cs, 1<<20);
	for(i=0; i<16; i++)
	{
//		memcpy(ct, cs, 16384);
//		memset_movx_test(ct, 0, 16384);
		memload_movx_test(ct, 0, 49152);
	}

//	cj = TK_GetTimeMs();
	cj = TK_GetTimeUs();
	ck = cj-ci;
	if(ck>0)
	{
//		tf=25600/ck;
//		tf=25600000/ck;
		tf=76800000/ck;
//		tf=12800/ck;
		th=tf/100;
		tl=tf-(th*100);
//		printf("memload (L2): %dus, %d.%02d MB/s\n", ck, th, tl);
		printf("%s: %dus, %d.%02d MB/s\n", "memload (L2)", ck, th, tl);
	}


#endif

//	__debugbreak();

#if 1
	memcpy_movx_test(ct, NULL, 32768);

	csum1=TKPE_CalculateImagePel4BChecksum(NULL, 32768);
	csum2=TKPE_CalculateImagePel4BChecksum(ct, 32768);
	if(csum1!=csum2)
	{
		printf("Memcpy Checksum Fail %X!=%X\n", csum2, csum1);
		__debugbreak();
	}
#endif

	return(0);
}
#endif

#ifndef __BJX2__
int TK_RamBench()
{
	return(0);
}
#endif

#endif

