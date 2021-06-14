/*
Page Table Walk
 */

void __debugbreak();

extern u64 __arch_ttb;
extern u64 __arch_tea;
extern u64 __arch_exsr;
extern u64 __arch_mmcr;
extern u64 __arch_sttb;

void __do_ldtlb(u64 ptel, u64 pteh);
void __do_rethrow_fault(int exsr);

u16 ptetlb_rethow_exc;

void ptetlb_tlbmiss_tl32(u64 ttb, u64 tea)
{
	u32 *ptd, *pte;
	u32 ptdv, ptev;
	u64 ptel, pteh;
	int ptdix, pteix;
	
	ptdix=(tea>>22)&1023;
	pteix=(tea>>12)&1023;
	
	ptd=(u32 *)(nlint)(NOMMU_BASE+(ttb&0x1FFFF000));
	ptdv=ptd[ptdix];
	if(!(ptdv&1))
		{ ptetlb_rethow_exc=0x8007; return; }
	pte=(u32 *)(nlint)(NOMMU_BASE+(ptdv&0x1FFFF000));
	ptev=pte[pteix];
	if(!(ptev&1))
		{ ptetlb_rethow_exc=0x8007; return; }
	
	pteh=tea&0x0000FFFFF000ULL;
	ptel=ptev;
	__do_ldtlb(ptel, pteh);
}

void ptetlb_tlbmiss_tl64a(u64 ttb, u64 tea)
{
	u32 *ptd0, *ptd1, *pte;
	u32 ptd0v, ptd1v, ptev;
	u64 ptel, pteh;
	int ptd0ix, ptd1ix, pteix;
	
	ptd0ix=(tea>>30)&511;
	ptd1ix=(tea>>21)&511;
	pteix=(tea>>12)&511;
	
	ptd0=(u32 *)(nlint)(NOMMU_BASE+(ttb&0x1FFFF000));
	ptd0v=ptd0[ptd0ix];
	if(!(ptd0v&1))
		{ ptetlb_rethow_exc=0x8007; return; }

	ptd1=(u32 *)(nlint)(NOMMU_BASE+(ptd0v&0x1FFFF000));
	ptd1v=ptd1[ptd1ix];
	if(!(ptd1v&1))
		{ ptetlb_rethow_exc=0x8007; return; }

	pte=(u32 *)(nlint)(NOMMU_BASE+(ptd1v&0x1FFFF000));
	ptev=pte[pteix];
	if(!(ptev&1))
		{ ptetlb_rethow_exc=0x8007; return; }
	
	pteh=tea&0x007FFFFFF000ULL;
	ptel=ptev;
	__do_ldtlb(ptel, pteh);
}

void ptetlb_tlbmiss_tl64b(u64 ttb, u64 tea)
{
	u32 *ptd0, *ptd1, *ptd2, *pte;
	u32 ptd0v, ptd1v, ptd2v, ptev;
	u64 ptel, pteh;
	int ptd0ix, ptd1ix, ptd2ix, pteix;
	
	ptd0ix=(tea>>39)&511;
	ptd1ix=(tea>>30)&511;
	ptd2ix=(tea>>21)&511;
	pteix =(tea>>12)&511;
	
	ptd0=(u32 *)(nlint)(NOMMU_BASE+(ttb&0x1FFFF000));
	ptd0v=ptd0[ptd0ix];
	if(!(ptd0v&1))
		{ ptetlb_rethow_exc=0x8007; return; }

	ptd1=(u32 *)(nlint)(NOMMU_BASE+(ptd0v&0x1FFFF000));
	ptd1v=ptd1[ptd1ix];
	if(!(ptd1v&1))
		{ ptetlb_rethow_exc=0x8007; return; }

	ptd2=(u32 *)(nlint)(NOMMU_BASE+(ptd1v&0x1FFFF000));
	ptd2v=ptd2[ptd2ix];
	if(!(ptd2v&1))
		{ ptetlb_rethow_exc=0x8007; return; }

	pte=(u32 *)(nlint)(NOMMU_BASE+(ptd2v&0x1FFFF000));
	ptev=pte[pteix];
	if(!(ptev&1))
		{ ptetlb_rethow_exc=0x8007; return; }
	
	pteh=tea&0xFFFFFFFFF000ULL;
	ptel=ptev;
	__do_ldtlb(ptel, pteh);
}

void ptetlb_tlbmiss(u64 ttb, u64 tea)
{
	u64 sttb;
	switch(ttb&15)
	{
	case 0:		__debugbreak();						break;
	case 1:		ptetlb_tlbmiss_tl32(ttb, tea);		break;
	case 2:		ptetlb_tlbmiss_tl64a(ttb, tea);		break;
	case 3:		ptetlb_tlbmiss_tl64b(ttb, tea);		break;

	case 5:
		if(tea&0x80000000)
		{
			sttb=__arch_sttb;
			ptetlb_tlbmiss_tl32(sttb, tea);
			break;
		}
		ptetlb_tlbmiss_tl32(ttb, tea);
		break;

	default:	__debugbreak();						break;
	}
}

__interrupt void __isr_mmu(void)
{
// #ifndef JX2UC
#if 0
	u64 ttb, tea;
	u16 exsr;
	
	ttb=__arch_ttb;
	tea=__arch_tea;
	exsr=(u16)(__arch_exsr);
	ptetlb_rethow_exc=0;
	
	switch(exsr)
	{
	case 0xA001:
		ptetlb_tlbmiss(ttb, tea);
		break;
	default:
		__debugbreak();
		break;
	}
	
	if(ptetlb_rethow_exc)
	{
		__arch_exsr=ptetlb_rethow_exc;
		__do_rethrow_fault();
	}
#endif
}
