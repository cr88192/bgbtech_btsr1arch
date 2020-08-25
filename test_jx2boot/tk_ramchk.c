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
