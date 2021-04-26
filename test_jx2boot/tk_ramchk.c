#define TK_RAMCHK_BASE	0x01000000U
//#define TK_RAMCHK_LIM	0x08000000U
#define TK_RAMCHK_LIM	0x02000000U

#define TK_RAMCHK_LIM_HI	0x17000000U

int tk_ram_kib;

void TK_FlushCacheL1D(void *pptr);
void TK_FlushCacheL1D_INVDCA(void);
void TK_FlushCacheL1D_INVL2(void);
void TK_FlushCacheL1D_ReadBuf(void *ptr, int sz);

int TK_RamChk()
{
	u32 *ct, *cts, *cte, *ct1e;
	int i, j, k;

	puts("RAM Check: Begin\n");
	
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

//	TK_FlushCacheL1D(cts);
	TK_FlushCacheL1D_INVDCA();
	TK_FlushCacheL1D_INVL2();

	TK_FlushCacheL1D_ReadBuf(0x02000000, 262144);

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
				__debugbreak();
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

	TK_FlushCacheL1D_ReadBuf(0x02000000, 262144);

	ct[0]=0x55AA55AAU;
	ct[1]=0x55AA55AAU;
	ct[2]=0x12345678U;
	ct[3]=0xFEDCBA98U;

	TK_FlushCacheL1D_INVDCA();
	TK_FlushCacheL1D_INVL2();

	TK_FlushCacheL1D_ReadBuf(0x02000000, 262144);

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
	
	puts("RAM Check: OK\n");
	
	return(0);
}

#ifndef JX2UC

void memcpy_movx_test(void *dst, void *src, int sz);
void memcpy_fixed128_test(void *dst, void *src, int sz);

__asm {
memcpy_movx_test:
#if 1
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
}

int TK_RamBench()
{
	byte *cs, *ct;
	int ci, cj, ck;
	int tf, th, tl;
	int i, j, k;

	puts("RAM Bench:\n");

	cs=(u32 *)TK_RAMCHK_BASE;
	ct=cs+0x123450;
	ci = TK_GetTimeMs();

//	memcpy(ct, cs, 1<<20);
//	memcpy(ct, cs, 1<<17);
	memcpy_movx_test(ct, cs, 1<<17);

	cj = TK_GetTimeMs();
	ck = cj-ci;
	if(ck>0)
	{
//		printf("memcpy: %dms, %d MB/s\n", ck, 1000/ck);

//		tf=25600/ck;
		tf=12800/ck;
		th=tf/100;
		tl=tf-(th*100);
		printf("memcpy (DRAM): %dms, %d.%02d MB/s\n", ck, th, tl);
	}


	ci = TK_GetTimeMs();

//	memcpy(ct, cs, 1<<20);
	for(i=0; i<8192; i++)
	{
//		memcpy(ct, cs, 128);
		memcpy_fixed128_test(ct, cs, 128);
	}

	cj = TK_GetTimeMs();
	ck = cj-ci;
	if(ck>0)
	{
		tf=102400/ck;
//		tf=12800/ck;
		th=tf/100;
		tl=tf-(th*100);
		printf("memcpy (L1): %dms, %d.%02d MB/s\n", ck, th, tl);
	}


	ci = TK_GetTimeMs();

//	memcpy(ct, cs, 1<<20);
	for(i=0; i<16; i++)
	{
//		memcpy(ct, cs, 16384);
		memcpy_movx_test(ct, cs, 16384);
	}

	cj = TK_GetTimeMs();
	ck = cj-ci;
	if(ck>0)
	{
		tf=25600/ck;
//		tf=12800/ck;
		th=tf/100;
		tl=tf-(th*100);
		printf("memcpy (L2): %dms, %d.%02d MB/s\n", ck, th, tl);
	}

	return(0);
}

#endif

