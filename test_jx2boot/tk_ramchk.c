#define TK_RAMCHK_BASE	0x01000000U
//#define TK_RAMCHK_LIM	0x08000000U
#define TK_RAMCHK_LIM	0x02000000U

int TK_RamChk()
{
	u32 *ct, *cts, *cte, *ct1e;

	puts("RAM Check: Begin\n");
	
	cts=(u32 *)TK_RAMCHK_BASE;
	cte=(u32 *)TK_RAMCHK_LIM;
	ct=cts;

#if 1
	while(ct<cte)
	{
		printf("DRAM %d\r", (ct-cts)<<2);
//		ct1e=ct+16384;
		ct1e=ct+262144;
//		ct1e=ct+1024;
		while(ct<ct1e)
		{
			ct[0]=(u32)ct;
//			ct+=4;
			ct+=256;
		}
	}
#endif

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
		printf("DRAM %d\r", (ct-cts)<<2);
		ct1e=ct+262144;
		while(ct<ct1e)
		{
			if(ct[0]!=((u32)ct))
				break;
			ct+=256;
		}
		if(ct<ct1e)
			break;
	}
#endif

	if(ct<cte)
	{
		printf("RAM Check Fail, A=%p, expect=%X got=%X",
			ct, ((u32)ct), ct[0]);
		__debugbreak();
		return(-1);
	}
	
	puts("RAM Check: OK\n");
	
	return(0);
}
