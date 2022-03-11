/*
Virtual Memory

Each VM Page is 4K|64K, and will map to a location in the pagefile.
Page indices will correspond to a logical pagefile entry.
 */

// #define		TK_VMEM_PAGESHL		12
#define		TK_VMEM_PAGESHL		TKMM_PAGEBITS
#define		TK_VMEM_BLOCKSHL	9
#define		TK_VMEM_PGBLKSHL	(TK_VMEM_PAGESHL-TK_VMEM_BLOCKSHL)

typedef struct TK_VMem_PageInfo_s TK_VMem_PageInfo;
struct TK_VMem_PageInfo_s {
int		pidx;		// 0, page index (pagefile)
byte	hi;			// 4, hash index
byte	pad0;		// 5, pad
short	flags;		// 6, flags
short	hnext;		// 8, next in hash
short	hprev;		//10, prev in hash
short	next;		//12, next in LRU
short	prev;		//14, prev in LRU
u32		vapidx;		//16, virtual address page index
u32		pad1;		//20, pad
u32		pad2;		//24, pad
u32		pad3;		//28, pad
//32
};

byte *tk_vmem_pagecache=NULL;	//page cache memory
TK_VMem_PageInfo *tk_vmem_pageinf;
int tk_vmem_pagehash[64];		//page hash
int tk_vmem_npage;

int tk_vmem_lru_first;
int tk_vmem_lru_last;
int tk_vmem_lru_free;

s64	tk_vmem_swap_lba;
u32	tk_vmem_swap_sz;
u32	tk_vmem_swap_psz;


u64	*tk_vmem_pageroot=NULL;
u64	*tk_vmem_aclroot=NULL;

u16	*tk_vmem_usrexpage;		//User, Execute Only, Memory
u16	*tk_vmem_usrexonly;		//User, Execute Only, Ex-Only Addr
int	tk_vmem_usrexoffs;		//User, Execute Only, Offset
byte tk_vmem_useldekrr;

// byte tk_vmem_pagebmp[16384];	//virtual page
byte *tk_vmem_pagebmp;	//virtual page
int tk_vmem_maxpage;
int tk_vmem_pagerov;

int tk_vmem_varov_lo;
int tk_vmem_varov_hi;

void TK_VMem_VaEvictPageIndex(int pidx);

#ifdef __BJX2__
// #if 0
u64 TK_VMem_GetPageTableEntry(s64 vaddr);

__asm {
#if (TK_VMEM_PAGESHL==14)

TK_VMem_GetPageTableEntry:

	MOV		16777215, R17
//	BREAK
	MOV		511, R2
	MOV		2047, R3
	SHLD.Q	R4, -14, R5

	SHLD.Q	R5, -22, R20
	SHLD.Q	R5, -11, R21
	SHLD.Q	R5,   0, R22
	AND		R3, R20
	AND		R3, R21
	AND		R3, R22

//		pde=tk_vmem_pageroot[vpt];
	
//	MOV.Q	tk_vmem_pageroot, R3
	MOV		0x0000FFFFFFFFC000, R16
	MOV		TTB, R3
	AND		R16, R3

	TEST	R3, R3
	BREAK?T
	
//	BREAK

	MOV.Q	(R3, R20), R7

	NOP
	NOP

	TEST	1, R7
	BT		.L0
	SHLD.Q	R7, -12,  R3
	AND		R17, R3
	SHLD.Q	R3, 14, R3

	TEST	R3, R3
	BREAK?T

	MOV.Q	(R3, R21), R7

	NOP
	NOP

	TEST	1, R7
	BT		.L0
	SHLD.Q	R7, -12,  R3
	AND		R17, R3
	SHLD.Q	R3, 14, R3

	TEST	R3, R3
	BREAK?T

	MOV.Q	(R3, R22), R2

	RTS

	.L0:
	
	TEST	R7, R7
	BREAK?F
	
	XOR		R2, R2
	RTS

#else

#if (TK_VMEM_PAGESHL==16)

TK_VMem_GetPageTableEntry:

//		vpn=(vaddr>>16)&16777215;
//		vpt=vpn>>13;
//		vpn&=8191;
//		vpt&=8191;

	MOV		16777215, R1
	MOV		8191, R2
	MOV		2047, R3
	SHLD.Q	R4, -16, R5
//	AND		R1, R5
	SHLD.Q	R5, -13, R6
	AND		R2, R5
	AND		R3, R6
//	AND		R2, R6

//		pde=tk_vmem_pageroot[vpt];
	
//	MOV.Q	tk_vmem_pageroot, R3
	MOV		0x0000FFFFFFFFC000, R16
	MOV		TTB, R3
	AND		R16, R3

	MOV.Q	(R3, R6), R7
	
//	XOR		R2, R2
	TEST	1, R7
	BT		.L0
//	RTS?T
		
//		ptpn=(pde>>12)&16777215;
//		ptp=(u64 *)(ptpn<<16);

	SHLD.Q	R7, -12,  R3
	AND		R1, R3
	SHLD.Q	R3, 16, R3

//		pte=ptp[vpn];
//		return(pte);

	MOV.Q	(R3, R5), R2
	RTS

	.L0:
	
	TEST	R7, R7
	BREAK?F
	
	XOR		R2, R2
	RTS

#else

TK_VMem_GetPageTableEntry:

	MOV		16777215, R1
	MOV		511, R2
	MOV		2047, R3
	SHLD.Q	R4, -12, R5

	SHLD.Q	R5, -27, R20
	SHLD.Q	R5, -18, R21
	SHLD.Q	R5,  -9, R22
	SHLD.Q	R5,   0, R23
	AND		R2, R20
	AND		R2, R21
	AND		R2, R22
	AND		R2, R23

//		pde=tk_vmem_pageroot[vpt];
	
//	MOV.Q	tk_vmem_pageroot, R3
	MOV		0x0000FFFFFFFFC000, R16
	MOV		TTB, R3
	AND		R16, R3

	MOV.Q	(R3, R20), R7

	TEST	1, R7
	BT		.L0
	SHLD.Q	R7, -12,  R3
	AND		R1, R3
	SHLD.Q	R3, 12, R3
	MOV.Q	(R3, R21), R7

	TEST	1, R7
	BT		.L0
	SHLD.Q	R7, -12,  R3
	AND		R1, R3
	SHLD.Q	R3, 12, R3
	MOV.Q	(R3, R22), R7

	TEST	1, R7
	BT		.L0
	SHLD.Q	R7, -12,  R3
	AND		R1, R3
	SHLD.Q	R3, 12, R3
	MOV.Q	(R3, R23), R2

	RTS

	.L0:
	
	TEST	R7, R7
	BREAK?F
	
	XOR		R2, R2
	RTS


#endif

#endif

};

#else
u64 TK_VMem_GetPageTableEntry(s64 vaddr)
{
	u64 *ptp;
	u64 pde, pte, vpn;
	int vp0, vp1, vp2, vp3;
	int vpt, ptpn;

	if(TK_VMEM_PAGESHL==14)
	{
		/* Three-Level 16K page table. */
		vpn=(vaddr>>14);
		vp0=(vpn>>22)&2047;
		vp1=(vpn>>11)&2047;
		vp2=(vpn    )&2047;
		
		pde=tk_vmem_pageroot[vp0];
		if(!(pde&1))
			{ return(0); }
		ptpn=(pde>>12)&16777215;
		ptp=(u64 *)(ptpn<<14);

		pde=ptp[vp1];
		if(!(pde&1))
			{ return(0); }
		ptpn=(pde>>12)&16777215;
		ptp=(u64 *)(ptpn<<14);

		pte=ptp[vp2];
		return(pte);
	}else
		if(TK_VMEM_PAGESHL==16)
	{
		/* Two-Level 64K page table. */
		vpn=(vaddr>>16)&16777215;
		vpt=vpn>>13;
		vpn&=8191;
		vpt&=8191;
		
		pde=tk_vmem_pageroot[vpt];
		if(!(pde&1))
		{
			if(!pde)
			{
				return(0);
			}else
			{
				/* Page table may not be paged out. */
				__debugbreak();
			}
		}
		
		ptpn=(pde>>12)&16777215;
		ptp=(u64 *)(ptpn<<16);
		
		pte=ptp[vpn];
		return(pte);
	}else
	{
//		__debugbreak();

		/* Four-Level 4K page table. */
		vpn=(vaddr>>12);
		vp0=(vpn>>27)&511;
		vp1=(vpn>>18)&511;
		vp2=(vpn>> 9)&511;
		vp3=(vpn    )&511;
		
		pde=tk_vmem_pageroot[vp0];
		if(!(pde&1))
			{ return(0); }
		ptpn=(pde>>12)&16777215;
		ptp=(u64 *)(ptpn<<12);

		pde=ptp[vp1];
		if(!(pde&1))
			{ return(0); }
		ptpn=(pde>>12)&16777215;
		ptp=(u64 *)(ptpn<<12);

		pde=ptp[vp2];
		if(!(pde&1))
			{ return(0); }
		ptpn=(pde>>12)&16777215;
		ptp=(u64 *)(ptpn<<12);

		pte=ptp[vp3];
		return(pte);
	}
}
#endif

int TK_VMem_SetPageTableEntry(s64 vaddr, u64 ptval)
{
	u64 *ptp;
	u64 pde, pte, vpn;
	int vp0, vp1, vp2, vp3;
	int vpt, ptpn;

	if(TK_VMEM_PAGESHL==14)
	{
//		__debugbreak();

		/* Three-Level 16K page table. */
		vpn=(vaddr>>14);
		vp0=(vpn>>22)&2047;
		vp1=(vpn>>11)&2047;
		vp2=(vpn    )&2047;
		
		ptp=tk_vmem_pageroot;
		pde=ptp[vp0];
		if(!(pde&1))
		{
			if(!pde)
			{
//				ptpn=TKMM_AllocPages(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
				ptpn=TKMM_AllocPagesZeroed(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
				pde=(((u64)ptpn)<<12)|1;
				ptp[vp0]=pde;

//				ptp=(u64 *)(ptpn<<14);
//				memset(ptp, 0, 16384);
			}else
			{
				__debugbreak();
			}
		}
		ptpn=(pde>>12)&16777215;
		ptp=(u64 *)(ptpn<<14);

		pde=ptp[vp1];
		if(!(pde&1))
		{
			if(!pde)
			{
//				ptpn=TKMM_AllocPages(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
				ptpn=TKMM_AllocPagesZeroed(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
				pde=(((u64)ptpn)<<12)|1;
				ptp[vp1]=pde;

//				ptp=(u64 *)(ptpn<<14);
//				memset(ptp, 0, 16384);
			}else
			{
				__debugbreak();
			}
		}
		ptpn=(pde>>12)&16777215;
		ptp=(u64 *)(ptpn<<14);

		ptp[vp2]=ptval;
		return(0);
	}else
		if(TK_VMEM_PAGESHL==16)
	{
		vpn=(vaddr>>16)&16777215;
		vpt=vpn>>13;
		vpn&=8191;
		vpt&=8191;
		
		pde=tk_vmem_pageroot[vpt];
		if(!(pde&1))
		{
			if(!pde)
			{
				/* No pages here yet. */
//				ptpn=TKMM_AllocPages(1);
//				ptpn=TKMM_AllocPages(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
				ptpn=TKMM_AllocPagesZeroed(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
				tk_printf("TK_VMem_SetPageTableEntry: "
					"AllocPage: %d %p\n",
					ptpn, (void *)(ptpn<<16));
				pde=(((u64)ptpn)<<12)|1;
				tk_vmem_pageroot[vpt]=pde;

//				ptp=(u64 *)(ptpn<<16);
//				memset(ptp, 0, 65536);
//				return(0);
			}else
			{
				/* Page table may not be paged out. */
				__debugbreak();
			}
		}
		
		ptpn=(pde>>12)&16777215;
		ptp=(u64 *)(ptpn<<16);
		
		ptp[vpn]=ptval;
		return(0);
	}else
	{
//		__debugbreak();

		/* Four-Level 4K page table. */
		vpn=(vaddr>>12);
		vp0=(vpn>>27)&511;
		vp1=(vpn>>18)&511;
		vp2=(vpn>> 9)&511;
		vp3=(vpn    )&511;
		
		ptp=tk_vmem_pageroot;
		pde=ptp[vp0];
		if(!(pde&1))
		{
			if(!pde)
			{
//				ptpn=TKMM_AllocPages(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
				ptpn=TKMM_AllocPagesZeroed(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
				pde=(((u64)ptpn)<<12)|1;
				ptp[vp0]=pde;

//				ptp=(u64 *)(ptpn<<12);
//				memset(ptp, 0, 4096);
			}else
			{
				__debugbreak();
			}
		}
		ptpn=(pde>>12)&16777215;
		ptp=(u64 *)(ptpn<<12);

		pde=ptp[vp1];
		if(!(pde&1))
		{
			if(!pde)
			{
//				ptpn=TKMM_AllocPages(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
				ptpn=TKMM_AllocPagesZeroed(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
				pde=(((u64)ptpn)<<12)|1;
				ptp[vp1]=pde;

//				ptp=(u64 *)(ptpn<<12);
//				memset(ptp, 0, 4096);
			}else
			{
				__debugbreak();
			}
		}
		ptpn=(pde>>12)&16777215;
		ptp=(u64 *)(ptpn<<12);

		pde=ptp[vp2];
		if(!(pde&1))
		{
			if(!pde)
			{
//				ptpn=TKMM_AllocPages(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
				ptpn=TKMM_AllocPagesZeroed(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
				pde=(((u64)ptpn)<<12)|1;
				ptp[vp2]=pde;

//				ptp=(u64 *)(ptpn<<12);
//				memset(ptp, 0, 4096);
			}else
			{
				__debugbreak();
			}
		}
		ptpn=(pde>>12)&16777215;
		ptp=(u64 *)(ptpn<<12);

		ptp[vp3]=ptval;
		return(0);
	}
}

int TK_VMem_Init();

int TK_VMem_AddSdSwap(s64 lba, u32 sz)
{
	if(!tk_iskernel())
	{
		return(0);
	}

	tk_vmem_swap_lba=lba;
	tk_vmem_swap_sz=sz;
	tk_vmem_swap_psz=sz>>TK_VMEM_PAGESHL;

	printf("TK_VMem_AddSdSwap: %X %dK %d(pages)\n", (int)lba, (int)(sz>>10),
		(int)(tk_vmem_swap_psz));
	
	TK_VMem_Init();

	return(0);
}

#ifdef __BJX2__
// #if 0
extern volatile u64 __arch_ttb;
extern volatile u64 __arch_tea;
extern volatile u64 __arch_exsr;
extern volatile u64 __arch_mmcr;
extern volatile u64 __arch_sttb;

void tk_vmem_do_ldtlb(u64 ptel, u64 pteh);
void tk_vmem_loadpte(u64 tva, u64 pte);
void tk_vmem_loadacl(u64 acle);

void tk_vmem_l1flush();

__asm {

tk_vmem_l1flush:

	//flush caches (initial)
//	XOR		R2, R2
	MOV		-1, R2
	INVDC	R2
	INVIC	R2

	MOV		0x10000, R4
	MOV		R4, R5

	.L0:
	MOV.Q	(R4), R3
	ADD		16, R4
	ADD		-16, R5
	CMPGT	0, R5
	BT		.L0

//	XOR		R2, R2
	MOV		-1, R2
	INVDC	R2
	INVIC	R2

	RTS


tk_vmem_do_ldtlb:
	MOV		R4, R0
	MOV		R5, R1

	NOP
	NOP
	NOP
	NOP

	LDTLB
	NOP
	NOP
	
//	XOR		R2, R2
	MOV		-1, R2
//	INVDC	R2
	INVIC	R2
//	INVDC	R5
//	INVIC	R5
	
	NOP
	NOP
	NOP
	
//	BREAK
	RTS
	NOP
	NOP

tk_vmem_loadacl:
	MOV		R4, R0
	LDACL
	RTS

#if (TK_VMEM_PAGESHL==14)
tk_vmem_loadpte:
	MOV		0x0000000000000FFF, R16
	MOV		0x0000FFFFFFFFC000, R17
	MOV		0x0000000FFFFFF000, R18
	MOV		0xFFFF000000000000, R19
	AND		R5, R16, R2
	AND		R4, R17, R3
	AND		R5, R18, R6
	AND		R5, R19, R7
	SHLD.Q	R6, 2, R6
	SHLD.Q	R5, -36, R20
	OR		R6, R2
	OR		R7, R3
	AND		R20, R16, R20
	OR		R20, R3

	MOV		R2, R0
	MOV		R3, R1

	NOP
	NOP
	NOP
	NOP

	LDTLB
//	BREAK
	NOP
	NOP

//	XOR		R2, R2
	MOV		-1, R2
//	INVDC	R2
//	INVIC	R2

//	INVDC	R4
//	INVIC	R4
	
	NOP
	NOP
	NOP

	RTS
	NOP
	NOP
#else

#if (TK_VMEM_PAGESHL==16)
tk_vmem_loadpte:
	MOV		0x0000000000000FFF, R16
	MOV		0x0000FFFFFFFF0000, R17
	MOV		0x0000000FFFFFF000, R18
	MOV		0xFFFF000000000000, R19
	AND		R5, R16, R2
	AND		R4, R17, R3
	AND		R5, R18, R6
	AND		R5, R19, R7
	SHLD.Q	R6, 4, R6
	SHLD.Q	R5, -36, R20
	OR		R6, R2
	OR		R7, R3
	AND		R20, R16, R20
	OR		R20, R3

	MOV		R2, R0
	MOV		R3, R1

	NOP
	NOP
	NOP
	NOP

	LDTLB
//	BREAK
	NOP
	NOP

//	XOR		R2, R2
	MOV		-1, R2
//	INVDC	R2
	INVIC	R2
//	INVDC	R4
//	INVIC	R4
	
	NOP
	NOP
	NOP

	RTS
	NOP
	NOP
#else
tk_vmem_loadpte:
	MOV		0x0000000000000FFF, R16
	MOV		0x0000FFFFFFFFF000, R17
	MOV		0x0000000FFFFFF000, R18
	MOV		0xFFFF000000000000, R19
	AND		R5, R16, R2
	AND		R4, R17, R3
	AND		R5, R18, R6
	AND		R5, R19, R7
	SHLD.Q	R5, -36, R20
	OR		R6, R2
	OR		R7, R3
	AND		R20, R16, R20
	OR		R20, R3

	MOV		R2, R0
	MOV		R3, R1

	NOP
	NOP
	NOP
	NOP

	LDTLB
//	BREAK
	NOP
	NOP

//	XOR		R2, R2
	MOV		-1, R2
//	INVDC	R2
	INVIC	R2

//	INVDC	R4
//	INVIC	R4
	
	NOP
	NOP
	NOP

	RTS
	NOP
	NOP
#endif

#endif
};

#if 0
void tk_vmem_loadpte(u64 tva, u64 pte)
{
	u64 ptel, pteh;
	
	if(TK_VMEM_PAGESHL==16)
	{
		ptel =	((pte&0x0000000000000FFFULL)    ) |
				((pte&0x0000000FFFFFF000ULL)<< 4) ;

		pteh =	((pte&0x0000FFF000000000ULL)>>36) |
				((pte&0xFFFF000000000000ULL)    ) |
				((tva&0x0000FFFFFFFF0000ULL)    ) ;
	}else
	{
		ptel =	((pte&0x0000000FFFFFFFFFULL)    ) ;

		pteh =	((pte&0x0000FFF000000000ULL)>>36) |
//		pteh =	((pte>>36)&0x000000000FFFULL    ) |
				((pte&0xFFFF000000000000ULL)    ) |
				((tva&0x0000FFFFFFFFF000ULL)    ) ;
	}

//	__debugbreak();

	tk_vmem_do_ldtlb(ptel, pteh);
}
#endif

#else

void tk_vmem_loadpte(u64 tva, u64 pte)
{
}

#endif

int TK_VMem_Init()
{
	TK_VMem_PageInfo *cpi, *lpi, *npi;
	s64 tva;
	u64 tpte, tmmcr;
	int np, b;
	int i, j, k, n;

	if(!tk_iskernel())
	{
		__debugbreak();
		return(0);
	}

	if(tk_vmem_pagecache)
		return(0);
	
	if(!tk_vmem_swap_psz)
	{
		if(!tk_vmem_usrexpage)
		{
			tk_vmem_usrexpage=TKMM_PageAlloc(1<<TK_VMEM_PAGESHL);
//			tk_vmem_usrexonly=(void *)TKMM_EXOSTART;
			tk_vmem_usrexonly=tk_vmem_usrexpage;
			tk_vmem_usrexoffs=0;
			tk_vmem_useldekrr=0;
		}

		/* Don't set up virtual memory if no pagefile. */
		return(0);
	}
	
	tk_printf("TK_VMem_Init\n");
		
	for(i=0; i<64; i++)
		tk_vmem_pagehash[i]=-1;

	tk_vmem_lru_first=-1;
	tk_vmem_lru_last=-1;

	tk_vmem_lru_free=-1;
	for(i=0; i<np; i++)
	{
		cpi=tk_vmem_pageinf+i;
		cpi->next=tk_vmem_lru_free;
		tk_vmem_lru_free=i;
	}

#if 0
	tk_printf("TK_VMem_Init: A-0\n");

//	np=8192;
	np=(32768<<10)>>TK_VMEM_PAGESHL;
	tk_vmem_pagecache=TKMM_PageAlloc(np<<TK_VMEM_PAGESHL);
	tk_vmem_npage=np;
	
	tk_vmem_pageinf=tk_malloc(np*sizeof(TK_VMem_PageInfo));
	memset(tk_vmem_pageinf, 0, np*sizeof(TK_VMem_PageInfo));
#endif

	tk_printf("TK_VMem_Init: A-1\n");

	tk_vmem_pageroot=TKMM_PageAlloc(1<<TK_VMEM_PAGESHL);
	memset(tk_vmem_pageroot, 0, 1<<TK_VMEM_PAGESHL);

	tk_vmem_aclroot=TKMM_PageAlloc(1<<TK_VMEM_PAGESHL);
	memset(tk_vmem_aclroot, 0, 1<<TK_VMEM_PAGESHL);

#if 1
//	i=(tk_vmem_swap_psz+7)/8;
	i=((tk_vmem_swap_psz+63)/64)*8;
	tk_vmem_pagebmp=tk_malloc(i);
	memset(tk_vmem_pagebmp, 0, i);

//	memset(tk_vmem_pagebmp, 0, 16384);
	tk_vmem_maxpage=tk_vmem_swap_psz;
	tk_vmem_pagerov=0;
#endif

	tk_printf("TK_VMem_Init: A-2\n");

	/* Identity Map RAM/etc */
//	n=(TKMM_PAGEEND>>TK_VMEM_PAGESHL);
	n=(TKMM_VAS_START_LO>>TK_VMEM_PAGESHL);
	for(i=0; i<n; i++)
	{
		tva=((u64)i)<<TK_VMEM_PAGESHL;
//		tpte=(2<<8)|(1<<10)|1;
		tpte=(0<<8)|(1<<10)|1;
		tpte|=i<<12;

		TK_VMem_SetPageTableEntry(tva, tpte);
//		tk_vmem_loadpte(tva, tpte);
	}
	
	/* Identity Map MMIO Range */
	b=(MMIO_BASE>>TK_VMEM_PAGESHL);
	n=((MMIO_END-MMIO_BASE)>>TK_VMEM_PAGESHL);
	for(i=0; i<n; i++)
	{
		tva=((u64)(b+i))<<TK_VMEM_PAGESHL;
//		tpte=(2<<8)|(1<<10)|1;
		tpte=(0<<8)|(1<<10)|1;
		tpte|=(b+i)<<12;

		TK_VMem_SetPageTableEntry(tva, tpte);
	}

	/* Setup Null Page */
	i=0x10000>>TK_VMEM_PAGESHL;
	TK_VMem_SetPageTableEntry(0, (i<<12)|1);
//	TK_VMem_SetPageTableEntry(0, 0);

	tk_printf("TK_VMem_Init: A-3\n");

	tk_vmem_varov_lo=TKMM_VAS_START_LO>>TK_VMEM_PAGESHL;
	tk_vmem_varov_hi=TKMM_VAS_START_HI>>TK_VMEM_PAGESHL;

	tk_vmem_usrexpage=TKMM_PageAlloc(1<<TK_VMEM_PAGESHL);
	tk_vmem_usrexonly=(void *)TKMM_EXOSTART;
	tk_vmem_usrexoffs=0;
	tk_vmem_useldekrr=0;

	tva=(u64)tk_vmem_usrexonly;
	i=((u64)tk_vmem_usrexpage)>>TK_VMEM_PAGESHL;
	TK_VMem_SetPageTableEntry(tva, (i<<12)|1);

#if 1
	tk_printf("TK_VMem_Init: A-4\n");

//	np=8192;
	np=(32768<<10)>>TK_VMEM_PAGESHL;
	tk_vmem_pagecache=TKMM_PageAlloc(np<<TK_VMEM_PAGESHL);
	tk_vmem_npage=np;
	
//	tk_vmem_pageinf=tk_malloc(np*sizeof(TK_VMem_PageInfo));
	tk_vmem_pageinf=TKMM_PageAlloc(np*sizeof(TK_VMem_PageInfo));
//	memset(tk_vmem_pageinf, 0, np*sizeof(TK_VMem_PageInfo));
#endif

	tk_printf("TK_VMem_Init: A-5\n");

	/* Enable MMU */

#ifdef __BJX2__
	tpte=(u64)tk_vmem_pageroot;

	if(TK_VMEM_PAGESHL==14)
	{
		tpte|=0x0042;	//Three-Level, 16K pages
		tmmcr=0x002D;
	}
	else
	if(TK_VMEM_PAGESHL==16)
	{
		tpte|=0x0021;	//Two-Level, 64K pages
		tmmcr=0x001D;
	}
	else
	{
		tpte|=0x0003;	//Four-Level, 4K pages
		tmmcr=0x000D;
	}
	__arch_ttb=tpte;
//	tk_printf("");
//	TK_VMem_GetPageTableEntry(0);
	
	__arch_mmcr=tmmcr;
	
//	__debugbreak();
#endif

	tk_printf("TK_VMem_Init: OK\n");

	return(1);
}

int TK_VMem_MProtectPages(u64 addr, size_t len, int prot)
{
	u64 tva, tpte;
	int b, n;
	int i, j;

//	return(0);

	if(!tk_vmem_pageroot)
		return(0);

	tk_printf("TK_VMem_MProtectPages: %p %X %X\n", addr, len, prot);

	b=(addr>>TK_VMEM_PAGESHL);
	n=(len>>TK_VMEM_PAGESHL);
	for(i=0; i<n; i++)
	{
		tva=((u64)(b+i))<<TK_VMEM_PAGESHL;

		tpte=TK_VMem_GetPageTableEntry(tva);

		tpte&=~0x00F8ULL;

		if(!(prot&0x01))
			tpte|=0x10ULL;
		if(!(prot&0x02))
			tpte|=0x20ULL;
		if(!(prot&0x04))
			tpte|=0x40ULL;

		if(prot&0x10)
			tpte|=0x08ULL;
		if(prot&0x20)
			tpte|=0x80ULL;

//		tpte=(2<<8)|(1<<10)|1;
//		tpte=(0<<8)|(1<<10)|1;
//		tpte|=(b+i)<<12;

		TK_VMem_SetPageTableEntry(tva, tpte);
	}

	return(0);
}

void TK_VMem_UnlinkPageHash(int i)
{
	TK_VMem_PageInfo *cpi, *lpi, *npi;
	int j, k, hi;

	cpi=tk_vmem_pageinf+i;
	j=cpi->hprev;
	if(j>=0)
	{
		/* Move to Front of Hash */
		k=cpi->hnext;
		lpi=tk_vmem_pageinf+j;
		lpi->hnext=k;
		if(k>=0)
		{
			npi=tk_vmem_pageinf+k;
			npi->hprev=j;
		}
	}
	else
	{
		hi=cpi->hi;
		k=cpi->hnext;
		if(k>=0)
		{
			npi=tk_vmem_pageinf+k;
			npi->hprev=-1;
			tk_vmem_pagehash[hi]=k;
		}else
		{
			tk_vmem_pagehash[hi]=-1;
		}
	}
		
	cpi->hnext=-1;
	cpi->hprev=-1;
}

void TK_VMem_UnlinkPageLRU(int i)
{
	TK_VMem_PageInfo *cpi, *lpi, *npi;
	int j, k, hi;

	cpi=tk_vmem_pageinf+i;
	j=cpi->prev;
	if(j>=0)
	{
		/* Move to Front of Hash */
		k=cpi->next;
		lpi=tk_vmem_pageinf+j;
		lpi->next=k;
		if(k>=0)
		{
			npi=tk_vmem_pageinf+k;
			npi->prev=j;
		}else
		{
			tk_vmem_lru_last=j;
		}
	}
	else
	{
		k=cpi->next;
		if(k>=0)
		{
			npi=tk_vmem_pageinf+k;
			npi->prev=-1;
			tk_vmem_lru_first=k;
		}else
		{
			tk_vmem_lru_first=-1;
			tk_vmem_lru_last=-1;
		}
	}
		
	cpi->next=-1;
	cpi->prev=-1;
}

void TK_VMem_AddPageFrontHash(int i)
{
	TK_VMem_PageInfo *cpi, *lpi, *npi;
	int k, hi;

	cpi=tk_vmem_pageinf+i;
	hi=cpi->hi;
	k=tk_vmem_pagehash[hi];
	cpi->hnext=k;
	cpi->hprev=-1;
	if(k>=0)
	{
		npi=tk_vmem_pageinf+k;
		npi->hprev=i;
	}
	tk_vmem_pagehash[hi]=i;
}

void TK_VMem_AddPageFrontLRU(int i)
{
	TK_VMem_PageInfo *cpi, *lpi, *npi;
	int k;

	cpi=tk_vmem_pageinf+i;
	k=tk_vmem_lru_first;
	npi=tk_vmem_pageinf+k;
	npi->prev=i;
	cpi->next=k;
	tk_vmem_lru_first=i;
}

void TK_VMem_SetPageVAddr(int idx, s64 vaddr)
{
	TK_VMem_PageInfo *cpi;
	cpi=tk_vmem_pageinf+idx;
	cpi->vapidx=vaddr>>TKMM_PAGEBITS;
}

s64 TK_VMem_GetPageVAddr(int idx)
{
	TK_VMem_PageInfo *cpi;
	s64 vaddr;
	cpi=tk_vmem_pageinf+idx;
	vaddr=((s64)(cpi->vapidx))<<TKMM_PAGEBITS;
	return(vaddr);
}

int TK_VMem_GetPagePidx(int idx)
{
	TK_VMem_PageInfo *cpi;
	cpi=tk_vmem_pageinf+idx;
	return(cpi->pidx);
}

void TK_VMem_WritePageToDisk(int idx)
{
	TK_VMem_PageInfo *cpi;
	byte *ppi;
	s64 lba;

	cpi=tk_vmem_pageinf+idx;
	ppi=tk_vmem_pagecache+(idx<<TK_VMEM_PAGESHL);
	
	if(tk_vmem_swap_lba>0)
	{
		lba=tk_vmem_swap_lba+((cpi->pidx)<<TK_VMEM_PGBLKSHL);
		TKSPI_WriteSectors(ppi, lba, 1<<TK_VMEM_PGBLKSHL);
		cpi->flags&=~1;
	}
}

void TK_VMem_LoadPageFromDisk(int idx)
{
	TK_VMem_PageInfo *cpi;
	byte *ppi;
	s64 lba;

	cpi=tk_vmem_pageinf+idx;
	ppi=tk_vmem_pagecache+(idx<<TK_VMEM_PAGESHL);
	
	if(tk_vmem_swap_lba>0)
	{
		lba=tk_vmem_swap_lba+((cpi->pidx)<<TK_VMEM_PGBLKSHL);
		TKSPI_ReadSectors(ppi, lba, 1<<TK_VMEM_PGBLKSHL);
	}
}

int TK_VMem_MapVirtToPhysIdx(int pidx)
{
	TK_VMem_PageInfo *cpi, *lpi, *npi;
	int hi, lp;
	int i, j, k;
	
	hi=((pidx*65521)>>16)&63;
	i=tk_vmem_pagehash[hi]; lpi=NULL;
	while(i>=0)
	{
		cpi=tk_vmem_pageinf+i;
		if(cpi->pidx==pidx)
		{
			j=cpi->hprev;
			if(j>=0)
			{
				/* Move to Front of Hash */
				TK_VMem_UnlinkPageHash(i);
				TK_VMem_AddPageFrontHash(i);
			}

			j=cpi->prev;
			if(j>=0)
			{
				/* Move to Front of list. */
				TK_VMem_UnlinkPageLRU(i);
				TK_VMem_AddPageFrontLRU(i);
			}
			return(i);
		}

		lpi=cpi;
		i=cpi->hnext;
	}
	
	if(tk_vmem_lru_free>=0)
	{
		/* Use page from free page list. */
		i=tk_vmem_lru_free;
		cpi=tk_vmem_pageinf+i;
		tk_vmem_lru_free=cpi->next;
		cpi->next=-1;	cpi->prev=-1;
		cpi->hnext=-1;	cpi->hprev=-1;
	}else
	{
		/* Evict old page. */
		i=tk_vmem_lru_last;
		TK_VMem_VaEvictPageIndex(i);
		TK_VMem_UnlinkPageHash(i);
		TK_VMem_UnlinkPageLRU(i);
	}
	
	if(cpi->flags&1)
	{
		TK_VMem_WritePageToDisk(i);
	}
	
	cpi->pidx=pidx;
	cpi->hi=hi;
	TK_VMem_LoadPageFromDisk(i);

	TK_VMem_AddPageFrontHash(i);
	TK_VMem_AddPageFrontLRU(i);
	return(i);
}

void *TK_VMem_MapPhysIdxToAddr(int idx)
{
	byte *ppi;
	ppi=tk_vmem_pagecache+(idx<<TK_VMEM_PAGESHL);
	return(ppi);
}

int TK_VMem_MapPhysIdxToPageNum(int idx)
{
	int pn;	
	pn=((s64)tk_vmem_pagecache)>>TK_VMEM_PAGESHL;
	return(pn+idx);
}

int TK_VMem_MapAddrToPhysIdx(void *addr)
{
	int ix;
	ix=((byte *)addr)-tk_vmem_pagecache;
	if(ix<0)
		return(-1);
	ix=ix>>TK_VMEM_PAGESHL;
	if(ix>=tk_vmem_npage)
		return(-1);
	return(ix);
}


int TK_VMem_FindFreeVirtPages(int n)
{
	int i0, i1;
	int i, j, m;
	
	if(n<=0)
		return(-1);
	
	i=tk_vmem_pagerov;
	m=tk_vmem_maxpage;
	while(i<m)
	{
		if(tk_vmem_pagebmp[i>>3]&(1<<(i&7)))
		{
			while((tk_vmem_pagebmp[i>>3]==0xFF) && (i<m))
			{
				i=(i+8)&(~7);
				continue;
			}
			while((tk_vmem_pagebmp[i>>3]&(1<<(i&7))) && (i<m))
				i++;
			continue;
		}
		i0=i; i1=i0+n;
		if(i1>m)break;
		while(!(tk_vmem_pagebmp[i>>3]&(1<<(i&7))) && (i<i1))
			i++;
		if(i>=i1)
		{
			tk_vmem_pagerov=i1;
			return(i0);
		}
	}
	
	if(tk_vmem_pagerov!=0)
	{
		tk_vmem_pagerov=0;
		i=TK_VMem_FindFreeVirtPages(n);
		return(i);
	}

	return(-1);
}

int TK_VMem_AllocVirtPages(int n)
{
	int i, j, k;
	
	i=TK_VMem_FindFreeVirtPages(n);
	if(i<0)
	{
		tk_printf("TK_VMem_AllocVirtPages: Out Of Memory, n=%d\n", n);
		return(-1);
	}

	if((i+n)>tk_vmem_maxpage)
	{
		__debugbreak();
	}
	
	j=i; k=j+n;
	while(j<k)
	{
		tk_vmem_pagebmp[j>>3]|=(1<<(j&7));
		j++;
	}
	return(i);
}

int TK_VMem_FreeVirtPages(int b, int n)
{
	int i;
	
	if((b<0) || (n<=0))
		return(-1);
		
	if((b+n)>tk_vmem_maxpage)
	{
		__debugbreak();
	}
	
	for(i=b; i<(b+n); i++)
	{
		tk_vmem_pagebmp[i>>3]&=~(1<<(i&7));
	}
	
	if(b<tk_vmem_pagerov)
		tk_vmem_pagerov=b;
	
	return(0);
}


u64 TK_VMem_GetPageTableEntry(s64 vaddr);
int TK_VMem_SetPageTableEntry(s64 vaddr, u64 ptval);

int TK_VMem_VaQueryPages(s64 vaddr, int cnt)
{
	u64 pte;
	s64 vtaddr;
	int i, j, k, qfl;
	
	qfl=0;
	for(i=0; i<cnt; i++)
	{
		vtaddr=vaddr+(((s64)i)<<TKMM_PAGEBITS);
		pte=TK_VMem_GetPageTableEntry(vtaddr);
		if(!pte)
		{
			qfl|=1;		//empty pages
		}else
		{
			qfl|=2;		//non-empty pages
		}
	}
	return(qfl);
}


int TK_VMem_VaReservePages(s64 vaddr, int cnt)
{
	u64 pte;
	s64 vtaddr;
	int i, j, k;
	
	for(i=0; i<cnt; i++)
	{
		vtaddr=vaddr+(((s64)i)<<TKMM_PAGEBITS);
		pte=TK_VMem_GetPageTableEntry(vtaddr);
		if(!pte)
		{
//			if(TKMM_PAGEBITS==16)
//				{ pte=(2<<8)|(1<<10); }
//			else
//				{ pte=(0<<8)|(1<<10); }
			pte=(0<<8)|(1<<10);
			TK_VMem_SetPageTableEntry(vtaddr, pte);
		}
	}
}

int TK_VMem_VaCommitPages(s64 vaddr, int cnt)
{
	u64 pte;
	s64 vtaddr;
	int ptpn;
	int i, j, k;
	
	for(i=0; i<cnt; i++)
	{
		vtaddr=vaddr+(((s64)i)<<TKMM_PAGEBITS);
		pte=TK_VMem_GetPageTableEntry(vtaddr);

		if(!pte)
		{
//			if(TKMM_PAGEBITS==16)
//				{ pte=(2<<8)|(1<<10); }
//			else
//				{ pte=(0<<8)|(1<<10); }
			pte=(0<<8)|(1<<10);
//			TK_VMem_SetPageTableEntry(vtaddr, pte);
		}

		ptpn=(pte>>12)&16777215;
		if(!ptpn)
		{
			ptpn=TK_VMem_AllocVirtPages(1);
//			TK_VMem_SetPageVAddr(ptpn, vtaddr);
			pte|=((u64)ptpn)<<12;
			pte&=~1;
			TK_VMem_SetPageTableEntry(vtaddr, pte);
		}
	}
}

int TK_VMem_VaFreePages(s64 vaddr, int cnt)
{
	u64 pte;
	s64 vtaddr, paddr;
	int ptpn, pidx;
	int i, j, k;
	
	for(i=0; i<cnt; i++)
	{
		vtaddr=vaddr+(((s64)i)<<TKMM_PAGEBITS);
		pte=TK_VMem_GetPageTableEntry(vtaddr);

		if(!pte)
			continue;

		if(pte&1)
		{
//			paddr=pte&0x0000FFFFFFFFF000ULL;
			paddr=((pte>>12)&0xFFFFFFFFFULL)<<TKMM_PAGEBITS;
			pidx=TK_VMem_MapAddrToPhysIdx((void *)paddr);
			if(pidx>=0)
			{
				TK_VMem_VaEvictPageIndex(pidx);

				pte=TK_VMem_GetPageTableEntry(vtaddr);
				if(!pte)
					continue;
			}else
			{
				/* Physical Remap */
				k=TKMM_PointerToPage((void *)paddr);
				TKMM_FreePages(k, 1);
				TK_VMem_SetPageTableEntry(vtaddr, 0);
				continue;
			}
		}

		ptpn=(pte>>12)&16777215;
		if(ptpn)
		{
			TK_VMem_FreeVirtPages(ptpn, 1);
			TK_VMem_SetPageTableEntry(vtaddr, 0);
		}

#if 0
		ptpn=(pte>>12)&16777215;
		if(!ptpn)
		{
			ptpn=TK_VMem_AllocVirtPages(1);
//			TK_VMem_SetPageVAddr(ptpn, vtaddr);
			pte|=((u64)ptpn)<<12;
			pte&=~1;
			TK_VMem_SetPageTableEntry(vtaddr, pte);
		}
#endif
	}
}


int TK_VMem_VaDoRemapPages(s64 vaddr, s64 phaddr, int cnt)
{
	u64 pte;
	s64 vtaddr;
	int phb;
	int i, j, k;
	
	for(i=0; i<cnt; i++)
	{
		vtaddr=vaddr+(((s64)i)<<TKMM_PAGEBITS);
		pte=TK_VMem_GetPageTableEntry(vtaddr);
		if(!pte)
		{
			pte=(((pdaddr>>TKMM_PAGEBITS)+i)<<12)|
				(0<<8)|(1<<10)|1;

//			pte=(0<<8)|(1<<10);
			TK_VMem_SetPageTableEntry(vtaddr, pte);
		}
	}
}


void TK_VMem_VaEvictPageIndex(int pidx)
{
	s64 vaddr;
	u64 pte;
	int ptpn;

	vaddr=TK_VMem_GetPageVAddr(pidx);
	ptpn=TK_VMem_GetPagePidx(pidx);

//	if(TKMM_PAGEBITS==16)
//		{ pte=(2<<8)|(1<<10); }
//	else
//		{ pte=(0<<8)|(1<<10); }
	pte=(0<<8)|(1<<10);
	pte|=((u64)ptpn)<<12;
	TK_VMem_SetPageTableEntry(vaddr, pte);
	
	/* Attempt to flush page from TLB */
	tk_vmem_loadpte(vaddr, pte);
	tk_vmem_loadpte(vaddr, pte);
	tk_vmem_loadpte(vaddr, pte);
	tk_vmem_loadpte(vaddr, pte);
}

void TK_VMem_VaPageInAddr(s64 vaddr)
{
	u64 pte;
	int ptpn;
	int pidx;
	int ppn;

	pte=TK_VMem_GetPageTableEntry(vaddr);
	if(pte&1)
		return;
	ptpn=(pte>>12)&16777215;
	if(!ptpn)
	{
		ptpn=TK_VMem_AllocVirtPages(1);
		pte|=((u64)ptpn)<<12;
		pte&=~1;
		TK_VMem_SetPageTableEntry(vaddr, pte);

//		return;
	}
	pte&=~((16777215ULL)<<12);
	pidx=TK_VMem_MapVirtToPhysIdx(ptpn);
	TK_VMem_SetPageVAddr(pidx, vaddr);
	ppn=TK_VMem_MapPhysIdxToPageNum(pidx);
	pte|=((u64)ppn)<<12;
	pte|=1;
	TK_VMem_SetPageTableEntry(vaddr, pte);

	tk_vmem_loadpte(vaddr, pte);
}

s64 TK_VMem_VaFindFreePages(int cnt, int flag)
{
	u64 pte;
	s64 vta;
	int vpn;
	int i, j, k;
	
	vpn=tk_vmem_varov_hi;
	while(1)
	{
		vta=((s64)vpn)<<TKMM_PAGEBITS;
		pte=TK_VMem_GetPageTableEntry(vta);
		if(pte)
		{
			vpn++;
			continue;
		}
		
		for(i=0; i<cnt; i++)
		{
			vta=((s64)(vpn+i))<<TKMM_PAGEBITS;
			pte=TK_VMem_GetPageTableEntry(vta);
			if(pte)
				break;
		}
		
		if(i>=cnt)
			break;
		vpn+=i;
	}

	vta=((s64)vpn)<<TKMM_PAGEBITS;
	return(vta);
}

s64 TK_VMem_VaVirtualAlloc(s64 addr, s64 size, int flProt, int flMap)
{
	int vpn, vpb;
	int qfl;

//	__debugbreak();
//	return(0);

#if 1
	vpb=addr>>TKMM_PAGEBITS;
	vpn=size>>TKMM_PAGEBITS;

	if(addr)
	{
		qfl=TK_VMem_VaQueryPages(addr, vpn);
		
		if(qfl==1)
		{
//			TK_VMem_VaReservePages(addr, vpn);
			TK_VMem_VaCommitPages(addr, vpn);
			return(addr);
		}
	}
	
	addr=TK_VMem_VaFindFreePages(vpn, flMap);
//	TK_VMem_VaReservePages(addr, vpn);
	TK_VMem_VaCommitPages(addr, vpn);
	return(addr);
#endif
}

int TK_VMem_CheckAddrIsVirtual(s64 addr)
{
	if(((addr>>32)&0xFFFF)!=0x0000)
		return(1);
	return(0);
}

int TK_VMem_CheckPtrIsVirtual(void *ptr)
{
	return(TK_VMem_CheckAddrIsVirtual((s64)ptr));
}

void tk_vmem_tlbmiss(u64 ttb, u64 tea)
{
	u64 pte;

#if 0
//	if(!pte)
	if(1)
	{
		tea&=0x0000FFFFFFFFFFFFULL;
		pte=((tea>>TKMM_PAGEBITS)<<12)|1;
		tk_vmem_loadpte(tea, pte);
		return;
	}
#endif
	
//	__debugbreak();
	
	pte=TK_VMem_GetPageTableEntry(tea);
	if(pte&1)
	{
//		__debugbreak();
		tk_vmem_loadpte(tea, pte);
//		__debugbreak();
		return;
	}
	
#if 0
	if(!pte)
	{
		tea&=0x0000FFFFFFFFFFFFULL;
		pte=((tea>>TKMM_PAGEBITS)<<12)|1;

//		__debugbreak();
//		pte=((0x10000>>TKMM_PAGEBITS)<<12)|1;
		tk_vmem_loadpte(tea, pte);
		return;
	}
#endif
	
//	__debugbreak();
	TK_VMem_VaPageInAddr(tea);
}

void tk_vmem_aclmiss(u64 ttb, u64 tea)
{
	u64 *acla;
	u64 acld, acle;
	int acid, acix;
	int apid;
	int i, k;
	
	acid=(tea    )&0xFFFF;
	apid=(tea>>16)&0xFFFF;
	
	acix=acid-0xF000;
	if((acix<0) || (acix>=2048))
	{
		/* Bad, No Access */
		acle=(u32)tea;	//No Access
		tk_vmem_loadacl(acle);
		return;
	}
	
	acld=tk_vmem_aclroot[acix];
	if((acld>>60)==1)
	{
		if(((u32)tea)==((u32)acld))
		{
			acle=acld&0x0000FFFFFFFFFFFFULL;
			tk_vmem_loadacl(acle);
			return;
		}

		acle=(u32)tea;	//No Access
		tk_vmem_loadacl(acle);
		return;
	}

	if((acld>>60)==0)
	{
		if(!acld)
		{
			/* NULL, No Access */
			acle=(u32)tea;	//No Access
			tk_vmem_loadacl(acle);
			return;
		}
	
		acla=(u64 *)acld;
		
		i=0;
		while(1)
		{
			acle=acla[i];
			if(!acle)
				break;

			/* Hit */
			if(((u32)tea)==((u32)acle))
			{
				tk_vmem_loadacl(acle);
				return;
			}
		}

		/* Miss, No Access */
		acle=(u32)tea;	//No Access
		tk_vmem_loadacl(acle);
		return;
	}
}

__interrupt void __isr_tlbfault(void)
// __declspec(dllexport) void isr_tlbfault_i(void)
{
	u64 ttb, tea, exc;
	u16 exsr;
	
	ttb=__arch_ttb;
	tea=__arch_tea;
	exc=__arch_exsr;
	exsr=(u16)(exc);
//	tea=exc>>16;

	if(exsr==0xA001)
	{
		tk_vmem_tlbmiss(ttb, tea);
		return;
	}

	if(exsr==0xA002)
	{
		tk_vmem_aclmiss(ttb, tea);
		return;
	}

	__debugbreak();

#if 0
	switch(exsr)
	{
	case 0xA001:
		tk_vmem_tlbmiss(ttb, tea);
		break;
	default:
		__debugbreak();
		break;
	}
#endif
}

#if 0
__asm {

ldtlb_dummy:
	MOV		TEA, R4
	MOV		0x0000FFFFFFFFC000, R1
	AND		R1, R4
	OR		R4, 1, R5
	MOV		R4, R1
	MOV		R5, R0
	LDTLB
	NOP
	RTS

__isr_tlbfault:
	ADD		-256, SP
	
	MOV.Q	R0, (SP, 0)
	MOV.Q	R1, (SP, 8)
	MOV.X	R2, (SP, 16)
	MOV.X	R4, (SP, 32)
	MOV.X	R6, (SP, 48)
	MOV.X	R16, (SP, 64)
	MOV.X	R18, (SP, 80)
	MOV.X	R20, (SP, 96)
	MOV.X	R22, (SP, 112)

	MOV		LR, R1
	MOV		GBR, R2
	MOV		TBR, R3
	MOV.Q	R1, (SP, 128)
	MOV.Q	R2, (SP, 136)
	MOV.Q	R3, (SP, 144)

//	BSR		isr_tlbfault_i
	BSR		ldtlb_dummy

	MOV.Q	(SP, 128), R1
	MOV.Q	(SP, 136), R2
	MOV.Q	(SP, 144), R3
	MOV		R1, LR
	MOV		R2, GBR
	MOV		R3, TBR

//	BREAK

	MOV.X	(SP, 16), R2
	MOV.X	(SP, 32), R4
	MOV.X	(SP, 48), R6
	MOV.X	(SP, 64), R16
	MOV.X	(SP, 80), R18
	MOV.X	(SP, 96), R20
	MOV.X	(SP, 112), R22
	MOV.Q	(SP, 0), R0
	MOV.Q	(SP, 8), R1
	
	ADD		256, SP
	RTE
};
#endif
