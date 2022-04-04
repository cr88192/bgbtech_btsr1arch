/*
Virtual Memory

Each VM Page is 4K/16K/64K, and will map to a location in the pagefile.
Page indices will correspond to a logical pagefile entry.
 */

// #define		TK_VMEM_PAGESHL		12
#define		TK_VMEM_PAGESHL		TKMM_PAGEBITS
#define		TK_VMEM_BLOCKSHL	9
#define		TK_VMEM_PGBLKSHL	(TK_VMEM_PAGESHL-TK_VMEM_BLOCKSHL)

// #define		TK_VMEM_PTESHL		12
#define		TK_VMEM_PTESHL		TKMM_PAGEBITS

// #define		TK_VMEM_PTEMASK		16777215
#define		TK_VMEM_PTEMASK		(16777215ULL>>(TK_VMEM_PTESHL-12))

typedef struct TK_VMem_PageInfo_s TK_VMem_PageInfo;
struct TK_VMem_PageInfo_s {
int		pidx;		// 0, page index (pagefile)
byte	hi;			// 4, hash index
byte	pad0;		// 5, pad
u16		flags;		// 6, flags
s32		next;		// 8, next in LRU
s32		prev;		//12, prev in LRU
u32		vapidx;		//16, virtual address page index
s32		hnext;		//20, next in hash
s32		hprev;		//24, prev in hash
u32		pad3;		//28, pad
//32
};

byte *tk_vmem_pagecache=NULL;	//page cache memory
TK_VMem_PageInfo *tk_vmem_pageinf;
int tk_vmem_pagehash[256];		//page hash
int tk_vmem_npage;

int tk_vmem_lru_first;
int tk_vmem_lru_last;
int tk_vmem_lru_free;

s64	tk_vmem_swap_lba;
u32	tk_vmem_swap_sz;
u32	tk_vmem_swap_psz;
int	tk_vmem_swap_disable;


u64	*tk_vmem_pageroot=NULL;
u64	*tk_vmem_aclroot=NULL;

u16	*tk_vmem_usrexpage;		//User, Execute Only, Memory
u16	*tk_vmem_usrexonly;		//User, Execute Only, Ex-Only Addr
int	tk_vmem_usrexoffs;		//User, Execute Only, Offset
byte tk_vmem_useldekrr;

// byte tk_vmem_pagebmp[16384];	//virtual page
byte *tk_vmem_pagebmp;	//virtual page (allocated)
byte *tk_vmem_pagevnz;	//virtual page (valid / non-zero)
byte *tk_vmem_pagecmz;	//virtual page (compacted)
int tk_vmem_maxpage;
int tk_vmem_pagerov;

byte *tk_vmem_page_tcbuf;	//temporary compression buffer

int tk_vmem_varov_lo;
int tk_vmem_varov_hi;

#ifdef __BJX2__
// #if 0
extern volatile u64 __arch_ttb;
extern volatile u64 __arch_tea;
extern volatile u64 __arch_exsr;
extern volatile u64 __arch_mmcr;
extern volatile u64 __arch_sttb;
#endif

void TK_VMem_VaEvictPageIndex(int pidx);
void TK_VMem_VaFlushVaddr(s64 vaddr);

u64 *TK_VMem_PtrForPto(u64 pto)
{
	return((u64 *)(pto&(~((1ULL<<TKMM_PAGEBITS)-1))));
}

u64 TK_VMem_PtoForPtr(u64 *ptr)
{
	return((u64)ptr);
}

#include "tk_vmptavl.c"
#include "tk_vmabtree.c"

#include "tk_vmlz64a.c"

// #ifdef __BJX2__
#if 0
u64 TK_VMem_GetPageTableEntry(s64 vaddr);

__asm {
#if (TK_VMEM_PAGESHL==14)

TK_VMem_GetPageTableEntry:

	MOV		TK_VMEM_PTEMASK, R17
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
//	SHLD.Q	R7, -12,  R3
	SHLD.Q	R7, -TK_VMEM_PTESHL,  R3
	AND		R17, R3
	SHLD.Q	R3, 14, R3

	TEST	R3, R3
	BREAK?T

	MOV.Q	(R3, R21), R7

	NOP
	NOP

	TEST	1, R7
	BT		.L0
//	SHLD.Q	R7, -12,  R3
	SHLD.Q	R7, -TK_VMEM_PTESHL,  R3
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

//		vpn=(vaddr>>16)&TK_VMEM_PTEMASK;
//		vpt=vpn>>13;
//		vpn&=8191;
//		vpt&=8191;

	MOV		TK_VMEM_PTEMASK, R1
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
		
//		ptpn=(pde>>TK_VMEM_PTESHL)&TK_VMEM_PTEMASK;
//		ptp=(u64 *)(ptpn<<16);

//	SHLD.Q	R7, -12,  R3
	SHLD.Q	R7, -TK_VMEM_PTESHL,  R3
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

	MOV		TK_VMEM_PTEMASK, R1
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

	pde=__arch_ttb;
//	if((pde&15)==0x4)
	if((pde&0x13)==0x13)
	{
		if((pde&0x300)==0x000)
			{ pte=TK_VMem_GetAvlPageTableEntry(pde, vaddr, 0); }
		else if((pde&0x300)==0x100)
			{ pte=TK_VMem_GetBTreePageTableEntry(pde, vaddr, 0); }
		else if((pde&0x300)==0x200)
			{ pte=TK_VMem_GetBTreeHybPageTableEntry(pde, vaddr, 0); }
		return(pte);
	}
	
	ptp=(u64 *)(pde&(~4095ULL));

	if(TK_VMEM_PAGESHL==14)
	{
		/* Three-Level 16K page table. */
		vpn=(vaddr>>TK_VMEM_PAGESHL);
		vp0=(vpn>>22)&2047;
		vp1=(vpn>>11)&2047;
		vp2=(vpn    )&2047;
		
//		pde=tk_vmem_pageroot[vp0];
		pde=ptp[vp0];
		if(!(pde&1))
			{ return(0); }
		ptpn=(pde>>TK_VMEM_PTESHL)&TK_VMEM_PTEMASK;
		ptp=(u64 *)(ptpn<<TK_VMEM_PAGESHL);

		pde=ptp[vp1];
		if(!(pde&1))
			{ return(0); }
		ptpn=(pde>>TK_VMEM_PTESHL)&TK_VMEM_PTEMASK;
		ptp=(u64 *)(ptpn<<TK_VMEM_PAGESHL);

		pte=ptp[vp2];
		return(pte);
	}else
		if(TK_VMEM_PAGESHL==16)
	{
		/* Two-Level 64K page table. */
		vpn=(vaddr>>16)&TK_VMEM_PTEMASK;
		vpt=vpn>>13;
		vpn&=8191;
		vpt&=8191;
		
//		pde=tk_vmem_pageroot[vpt];
		pde=ptp[vp0];
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
		
		ptpn=(pde>>TK_VMEM_PTESHL)&TK_VMEM_PTEMASK;
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
		
//		pde=tk_vmem_pageroot[vp0];
		pde=ptp[vp0];
		if(!(pde&1))
			{ return(0); }
		ptpn=(pde>>TK_VMEM_PTESHL)&TK_VMEM_PTEMASK;
//		ptp=(u64 *)(ptpn<<12);
		ptp=(u64 *)(ptpn<<TK_VMEM_PAGESHL);

		pde=ptp[vp1];
		if(!(pde&1))
			{ return(0); }
		ptpn=(pde>>TK_VMEM_PTESHL)&TK_VMEM_PTEMASK;
		ptp=(u64 *)(ptpn<<TK_VMEM_PAGESHL);

		pde=ptp[vp2];
		if(!(pde&1))
			{ return(0); }
		ptpn=(pde>>TK_VMEM_PTESHL)&TK_VMEM_PTEMASK;
//		ptp=(u64 *)(ptpn<<12);
		ptp=(u64 *)(ptpn<<TK_VMEM_PAGESHL);

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

	pde=__arch_ttb;
//	if((pde&15)==0x4)
	if((pde&0x13)==0x13)
	{
//		TK_VMem_SetAvlPageTableEntry(pde, vaddr, 0, ptval);
		if((pde&0x300)==0x000)
			{ TK_VMem_SetAvlPageTableEntry(pde, vaddr, 0, ptval); }
		else if((pde&0x300)==0x100)
			{ TK_VMem_SetBTreePageTableEntry(pde, vaddr, 0, ptval); }
		else if((pde&0x300)==0x200)
			{ TK_VMem_SetBTreeHybPageTableEntry(pde, vaddr, 0, ptval); }
		return(0);
	}
	
	ptp=(u64 *)(pde&(~4095ULL));

	if(TK_VMEM_PAGESHL==14)
	{
//		__debugbreak();

		/* Three-Level 16K page table. */
		vpn=(vaddr>>14);
		vp0=(vpn>>22)&2047;
		vp1=(vpn>>11)&2047;
		vp2=(vpn    )&2047;
		
//		ptp=tk_vmem_pageroot;
		pde=ptp[vp0];
		if(!(pde&1))
		{
			if(!pde)
			{
//				ptpn=TKMM_AllocPages(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
				ptpn=TKMM_AllocPagesZeroed(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
				pde=(((u64)ptpn)<<TK_VMEM_PTESHL)|1;
				ptp[vp0]=pde;
			}else
			{
				__debugbreak();
			}
		}
		ptpn=(pde>>TK_VMEM_PTESHL)&TK_VMEM_PTEMASK;
		ptp=(u64 *)(ptpn<<14);

		pde=ptp[vp1];
		if(!(pde&1))
		{
			if(!pde)
			{
//				ptpn=TKMM_AllocPages(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
				ptpn=TKMM_AllocPagesZeroed(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
				pde=(((u64)ptpn)<<TK_VMEM_PTESHL)|1;
				ptp[vp1]=pde;
			}else
			{
				__debugbreak();
			}
		}
		ptpn=(pde>>TK_VMEM_PTESHL)&TK_VMEM_PTEMASK;
		ptp=(u64 *)(ptpn<<14);

		ptp[vp2]=ptval;
		return(0);
	}else
		if(TK_VMEM_PAGESHL==16)
	{
		vpn=(vaddr>>16)&TK_VMEM_PTEMASK;
		vpt=vpn>>13;
		vpn&=8191;
		vpt&=8191;
		
//		pde=tk_vmem_pageroot[vpt];
		pde=ptp[vpt];
		if(!(pde&1))
		{
			if(!pde)
			{
				/* No pages here yet. */
//				ptpn=TKMM_AllocPages(1);
//				ptpn=TKMM_AllocPages(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
				ptpn=TKMM_AllocPagesZeroed(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
//				tk_printf("TK_VMem_SetPageTableEntry: "
//					"AllocPage: %d %p\n",
//					ptpn, (void *)(ptpn<<16));
				pde=(((u64)ptpn)<<TK_VMEM_PTESHL)|1;
//				tk_vmem_pageroot[vpt]=pde;
				ptp[vpt]=pde;
			}else
			{
				/* Page table may not be paged out. */
				__debugbreak();
			}
		}
		
		ptpn=(pde>>TK_VMEM_PTESHL)&TK_VMEM_PTEMASK;
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
		
//		ptp=TK_VMem_FreeSwapPages;
		pde=ptp[vp0];
		if(!(pde&1))
		{
			if(!pde)
			{
//				ptpn=TKMM_AllocPages(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
				ptpn=TKMM_AllocPagesZeroed(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
				pde=(((u64)ptpn)<<TK_VMEM_PTESHL)|1;
				ptp[vp0]=pde;
			}else
			{
				__debugbreak();
			}
		}
		ptpn=(pde>>TK_VMEM_PTESHL)&TK_VMEM_PTEMASK;
		ptp=(u64 *)(ptpn<<TK_VMEM_PTESHL);

		pde=ptp[vp1];
		if(!(pde&1))
		{
			if(!pde)
			{
//				ptpn=TKMM_AllocPages(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
				ptpn=TKMM_AllocPagesZeroed(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
//				pde=(((u64)ptpn)<<12)|1;
				pde=(((u64)ptpn)<<TK_VMEM_PTESHL)|1;
				ptp[vp1]=pde;

//				ptp=(u64 *)(ptpn<<12);
//				memset(ptp, 0, 4096);
			}else
			{
				__debugbreak();
			}
		}
		ptpn=(pde>>TK_VMEM_PTESHL)&TK_VMEM_PTEMASK;
		ptp=(u64 *)(ptpn<<TK_VMEM_PTESHL);

		pde=ptp[vp2];
		if(!(pde&1))
		{
			if(!pde)
			{
//				ptpn=TKMM_AllocPages(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
				ptpn=TKMM_AllocPagesZeroed(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
				pde=(((u64)ptpn)<<TK_VMEM_PTESHL)|1;
				ptp[vp2]=pde;

//				ptp=(u64 *)(ptpn<<TK_VMEM_PTESHL);
//				memset(ptp, 0, 4096);
			}else
			{
				__debugbreak();
			}
		}
		ptpn=(pde>>TK_VMEM_PTESHL)&TK_VMEM_PTEMASK;
		ptp=(u64 *)(ptpn<<TK_VMEM_PTESHL);

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
void tk_vmem_do_ldtlb(u64 ptel, u64 pteh);
void tk_vmem_loadpte(u64 tva, u64 pte);
void tk_vmem_loadacl(u64 acle);

void tk_vmem_l1flush();
void tk_vmem_setsrjq(int fl);

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

tk_vmem_setsrjq:
	MOV		SR, R7
	
	TEST	R4, R4
	AND?T	0xFFFFFFFF7FFFFFFF, R7
	OR?F	0x0000000080000000, R7
	MOV		R7, SR

//	MOV		R4, R0
//	LDACL
	RTS

#if (TK_VMEM_PAGESHL==14)
tk_vmem_loadpte:
	MOV		0x0000000000000FFF, R16
	MOV		0x0000FFFFFFFFC000, R17
#if (TK_VMEM_PTESHL!=TK_VMEM_PAGESHL)
	MOV		0x0000000FFFFFF000, R18
#else
	MOV		0x0000000FFFFFC000, R18
#endif
	MOV		0xFFFF000000000000, R19
	AND		R5, R16, R2
	AND		R4, R17, R3
	AND		R5, R18, R6
	AND		R5, R19, R7
#if (TK_VMEM_PTESHL!=TK_VMEM_PAGESHL)
	SHLD.Q	R6, 2, R6
#endif
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
	char tblk1[512], tblk2[512];
	TK_VMem_PageInfo *cpi, *lpi, *npi;
	u32 *conbuft;
	s64 tva;
	u64 tpte, tmmcr;
	int np, b, lba;
	int i, j, k, l, n;

	if(!tk_iskernel())
	{
		__debugbreak();
		return(0);
	}

//	if(tk_vmem_pagecache || tk_vmem_pageroot)
	if(tk_vmem_pageroot)
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

		tk_vmem_swap_disable=1;

//		/* Don't set up virtual memory if no pagefile. */
//		return(0);
	}
	
	tk_printf("TK_VMem_Init\n");

	tk_vmem_swap_disable=0;

#if 0
	for(i=0; i<512; i++)
	{
		tblk1[i]=i;
		tblk2[i]=0;
	}
#endif

	TKSPI_ReadSectors(tblk2, lba, 1);

#if 1
	/* Feed bits into hardware RNG.
	 * Go through MMIO so RNG sees it.
	 */
	conbuft=(u32 *)(MMIO_BASE+0x000A0000ULL);
	for(l=0; l<8; l++)
	{
		for(i=0; i<512; i++)
		{
			j=tblk2[i];
			j=(j<<l)^(j>>l);
			k=(31744+(j&255));
			conbuft[k]=i-j;
		}
	}

	for(i=0; i<512; i++)
	{
		tblk1[i]=TK_GetRandom();
		tblk2[i]=0;
	}
	
	TK_SeedRandomASLR();
#endif

	lba=tk_vmem_swap_lba;
	TKSPI_WriteSectors(tblk1, lba, 1);
	TKSPI_ReadSectors(tblk2, lba, 1);

	for(i=0; i<512; i++)
	{
		if(tblk1[i]!=tblk2[i])
		{
			tk_printf("TK_VMem_Init: Swap IO Check Fail\n");
			tk_vmem_swap_disable=1;
			break;
		}
	}

	for(i=0; i<256; i++)
		tk_vmem_pagehash[i]=-1;

	tk_vmem_lru_first=-1;
	tk_vmem_lru_last=-1;

#if 0
	tk_vmem_lru_free=-1;
	for(i=0; i<np; i++)
	{
		cpi=tk_vmem_pageinf+i;
		cpi->next=tk_vmem_lru_free;
		tk_vmem_lru_free=i;
	}
#endif

#if 0
	tk_printf("TK_VMem_Init: A-0\n");

//	np=8192;
	np=(32768<<10)>>TK_VMEM_PAGESHL;
	tk_vmem_pagecache=TKMM_PageAlloc(np<<TK_VMEM_PAGESHL);
	tk_vmem_npage=np;
	
//	tk_vmem_pageinf=tk_malloc_cat(np*sizeof(TK_VMem_PageInfo), 2);
	tk_vmem_pageinf=TKMM_PageAlloc(np*sizeof(TK_VMem_PageInfo));
	memset(tk_vmem_pageinf, 0, np*sizeof(TK_VMem_PageInfo));
#endif

	tk_printf("TK_VMem_Init: A-1\n");

	tk_vmem_pageroot=TKMM_PageAlloc(1<<TK_VMEM_PAGESHL);
	memset(tk_vmem_pageroot, 0, 1<<TK_VMEM_PAGESHL);

	tk_vmem_aclroot=TKMM_PageAlloc(1<<TK_VMEM_PAGESHL);
	memset(tk_vmem_aclroot, 0, 1<<TK_VMEM_PAGESHL);

#ifdef __BJX2__
	tpte=(u64)tk_vmem_pageroot;

	if(TK_VMEM_PAGESHL==14)
	{
//		tpte|=0x0042;	//Three-Level, 16K pages
		tpte|=0x0153;	//B-Tree
//		tpte|=0x0253;	//B-Tree (Hybrid)
//		tmmcr=0x002D;
	}
	else
	if(TK_VMEM_PAGESHL==16)
	{
		tpte|=0x0021;	//Two-Level, 64K pages
//		tmmcr=0x001D;
	}
	else
	{
		tpte|=0x0003;	//Four-Level, 4K pages
//		tmmcr=0x000D;
	}
	__arch_ttb=tpte;
#endif

#if 1
	if(!tk_vmem_swap_disable)
	{
//		i=(tk_vmem_swap_psz+7)/8;
		i=((tk_vmem_swap_psz+63)/64)*8;
//		tk_vmem_pagebmp=tk_malloc_cat(i, 2);
//		tk_vmem_pagevnz=tk_malloc_cat(i, 2);
		tk_vmem_pagebmp=TKMM_PageAlloc(i);
		tk_vmem_pagevnz=TKMM_PageAlloc(i);
		tk_vmem_pagecmz=TKMM_PageAlloc(i);
		memset(tk_vmem_pagebmp, 0, i);
		memset(tk_vmem_pagevnz, 0, i);
		memset(tk_vmem_pagecmz, 0, i);

		tk_vmem_page_tcbuf=TKMM_PageAlloc(2<<TK_VMEM_PAGESHL);

		tk_vmem_pagebmp[0]|=1;

//		memset(tk_vmem_pagebmp, 0, 16384);
		tk_vmem_maxpage=tk_vmem_swap_psz;
		tk_vmem_pagerov=0;
	}
#endif

	tk_printf("TK_VMem_Init: A-2\n");

#if 0
	/* Identity Map RAM/etc */
//	n=(TKMM_PAGEEND>>TK_VMEM_PAGESHL);
	n=(TKMM_VAS_START_LO>>TK_VMEM_PAGESHL);
	for(i=0; i<n; i++)
	{
		tva=((u64)i)<<TK_VMEM_PAGESHL;
//		tpte=(2<<8)|(1<<10)|1;
		tpte=(0<<8)|(1<<10)|1;
		tpte|=i<<TK_VMEM_PTESHL;

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
		tpte|=(b+i)<<TK_VMEM_PTESHL;

		TK_VMem_SetPageTableEntry(tva, tpte);
	}
#endif

#if 1
	/* Setup Null Page */

	i=0x10000>>TK_VMEM_PAGESHL;
	TK_VMem_SetPageTableEntry(0x00000000, (i<<TK_VMEM_PTESHL)|1);
	TK_VMem_SetPageTableEntry(0x00004000, (i<<TK_VMEM_PTESHL)|1);
	TK_VMem_SetPageTableEntry(0x00008000, (i<<TK_VMEM_PTESHL)|1);
	TK_VMem_SetPageTableEntry(0x0000C000, (i<<TK_VMEM_PTESHL)|1);
//	TK_VMem_SetPageTableEntry(0, 0);

	i=0x20000>>TK_VMEM_PAGESHL;
	TK_VMem_SetPageTableEntry(0x00010000, (i<<TK_VMEM_PTESHL)|1);
	TK_VMem_SetPageTableEntry(0x00014000, (i<<TK_VMEM_PTESHL)|1);
	TK_VMem_SetPageTableEntry(0x00018000, (i<<TK_VMEM_PTESHL)|1);
	TK_VMem_SetPageTableEntry(0x0001C000, (i<<TK_VMEM_PTESHL)|1);
//	TK_VMem_SetPageTableEntry(0, 0);
#endif


	tk_printf("TK_VMem_Init: A-3\n");

	tk_vmem_varov_lo=TKMM_VAS_START_LO>>TK_VMEM_PAGESHL;
	tk_vmem_varov_hi=TKMM_VAS_START_HI>>TK_VMEM_PAGESHL;

	tk_vmem_usrexpage=TKMM_PageAlloc(1<<TK_VMEM_PAGESHL);
	tk_vmem_usrexonly=(void *)TKMM_EXOSTART;
	tk_vmem_usrexoffs=0;
	tk_vmem_useldekrr=0;

	tva=(u64)tk_vmem_usrexonly;
	i=((u64)tk_vmem_usrexpage)>>TK_VMEM_PAGESHL;
	TK_VMem_SetPageTableEntry(tva, (i<<TK_VMEM_PTESHL)|1);

#if 1
	tk_printf("TK_VMem_Init: A-4\n");

	if(!tk_vmem_swap_disable)
	{
	//	np=8192;
		np=(32768<<10)>>TK_VMEM_PAGESHL;
		tk_vmem_pagecache=TKMM_PageAlloc(np<<TK_VMEM_PAGESHL);
		tk_vmem_npage=np;
		
		k=(np+1)*sizeof(TK_VMem_PageInfo);
//		tk_printf("sizeof(TK_VMem_PageInfo)=%d, np=%d np*sz=%d\n",
//			sizeof(TK_VMem_PageInfo), np, k);
		
//		__debugbreak();
		
	//	tk_vmem_pageinf=tk_malloc(np*sizeof(TK_VMem_PageInfo));
		tk_vmem_pageinf=TKMM_PageAlloc(np*sizeof(TK_VMem_PageInfo));
	//	memset(tk_vmem_pageinf, 0, np*sizeof(TK_VMem_PageInfo));

#if 1
		tk_vmem_lru_free=-1;
//		for(i=0; i<np; i++)
//		for(i=1; i<(np-1); i++)
		for(i=np-1; i>0; i--)
		{
			cpi=tk_vmem_pageinf+i;
			cpi->next=tk_vmem_lru_free;
			tk_vmem_lru_free=i;
		}
#endif
	}

#endif

	tk_printf("TK_VMem_Init: A-5\n");

	/* Enable MMU */

#ifdef __BJX2__

#if 1
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
//	__arch_ttb=tpte;
#endif

#ifdef __ADDR_X48__
	tk_vmem_setsrjq(1);
#endif
	
//	__arch_ttb=tpte;
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

//	tk_printf("TK_VMem_MProtectPages: %p %X %X\n", addr, len, prot);

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
//		tpte|=(b+i)<<TK_VMEM_PTESHL;

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
	cpi->next=k;
	if(k>=0)
	{
		npi=tk_vmem_pageinf+k;
		npi->prev=i;
	}else
	{
		tk_vmem_lru_last=i;
	}
	tk_vmem_lru_first=i;
}

void TK_VMem_MovePageFrontLRU(int i)
{
	TK_VMem_UnlinkPageLRU(i);
	TK_VMem_AddPageFrontLRU(i);
}

void TK_VMem_SetPageVAddr(int cidx, s64 vaddr)
{
	TK_VMem_PageInfo *cpi;
	
	if(cidx<0)
		{ __debugbreak(); }
	if(cidx>=tk_vmem_npage)
		{ __debugbreak(); }

	cpi=tk_vmem_pageinf+cidx;
	cpi->vapidx=vaddr>>TKMM_PAGEBITS;
}

s64 TK_VMem_GetPageVAddr(int cidx)
{
	TK_VMem_PageInfo *cpi;
	s64 vaddr;

	if(cidx<0)
		{ __debugbreak(); }
	if(cidx>=tk_vmem_npage)
		{ __debugbreak(); }

	cpi=tk_vmem_pageinf+cidx;
	vaddr=((s64)(cpi->vapidx))<<TKMM_PAGEBITS;
	return(vaddr);
}

int TK_VMem_GetPagePidx(int cidx)
{
	TK_VMem_PageInfo *cpi;

	if(cidx<0)
		{ __debugbreak(); }
	if(cidx>=tk_vmem_npage)
		{ __debugbreak(); }

	cpi=tk_vmem_pageinf+cidx;
	return(cpi->pidx);
}

void TK_VMem_UnpackCompactedPage(byte *cpi, byte *ppi)
{
	u64 nzm;
	byte cm;
	byte *cs, *ct, *cmap;
	int i, j, k;

	cm=cpi[0];

	if(cm==1)
	{
#if 0
		nzm=*(u64 *)(cpi+8);
		memset(ppi, 0, 1<<TK_VMEM_PAGESHL);
		
		cs=cpi+16;
		ct=ppi;
		for(i=0; i<64; i++)
		{
			if((nzm>>i)&1)
			{
				memcpy(ct, cs, 1<<(TK_VMEM_PAGESHL-6));
				cs+=1<<(TK_VMEM_PAGESHL-6);
			}
			ct+=1<<(TK_VMEM_PAGESHL-6);
		}
#endif

#if 1
		memset(ppi, 0, 1<<TK_VMEM_PAGESHL);
		
		cmap=cpi+8;
		cs=cpi+8+(1<<(TK_VMEM_PAGESHL-7));
		ct=ppi;
		for(i=0; i<(1<<(TK_VMEM_PAGESHL-4)); i++)
		{
			if((cmap[i>>3]>>(i&7))&1)
				{ memcpy(ct, cs, 16); cs+=16; }
			ct+=16;
		}
#endif

		return;
	}
	
	if(cm==2)
	{
		TK_VMem_UnpackLz64Page(cpi, ppi);
		return;
	}

	__debugbreak();
}

void TK_VMem_PackCompactedPage(byte *cpi, byte *ppi)
{
	u64 nzm;
	byte cm;
	byte *cs, *ct, *cmap;
	int i, j, k;

	memset(cpi, 0, 512);

	cmap=cpi+8;
	ct=cpi+8+(1<<(TK_VMEM_PAGESHL-7));
	cs=ppi;

	for(i=0; i<(1<<(TK_VMEM_PAGESHL-4)); i++)
	{
		if(	(((u64 *)cs)[0]==0) &&
			(((u64 *)cs)[1]==0) )
		{
			cs+=16;
			continue;
		}

		cmap[i>>3]|=1<<(i&7);
		memcpy(ct, cs, 16);
		cs+=16;		ct+=16;
	}
	
	j=((ct-cpi)+511)>>9;
	cpi[0]=1;
	cpi[1]=j-1;

	if((j<<9)>=(1<<(TK_VMEM_PAGESHL-1)))
	{
		tk_printf("TK_VMem_PackCompactedPage: Pack64\n");
		TK_VMem_PackLz64Page(cpi, ppi);
		return;
	}

}

void TK_VMem_WritePageToDisk(int cidx)
{
	TK_VMem_PageInfo *cpi;
	u64 *ppq;
	byte *ppi, *cs, *ct, *tcbuf;
	u64 nzm;
	s64 lba;
	int i, j, k, l, cm, pidx;

	if(cidx<0)
		{ __debugbreak(); }
	if(cidx>=tk_vmem_npage)
		{ __debugbreak(); }

	cpi=tk_vmem_pageinf+cidx;
	ppi=tk_vmem_pagecache+(cidx<<TK_VMEM_PAGESHL);
	cm=0;
	pidx=cpi->pidx;

	tcbuf=tk_vmem_page_tcbuf;

//	if(!(tk_vmem_pagevnz[cidx>>3]&(1<<(cidx&7))))
	if(1)
	{
		/* If Zero or Non-Initialized, Check if Non-Zero. */
		ppq=(u64 *)ppi;
		nzm=0;
		l=(1<<(TK_VMEM_PAGESHL-3));
//		for(i=0; i<l; i++)
//			if(ppq[i])
//				break;

		for(i=0; i<l; i++)
		{
			if(ppq[i])
				nzm|=1ULL<<(i>>(TK_VMEM_PAGESHL-6-3));
		}

//		if(i>=l)
		if(!nzm)
		{
			/* Page is still Zero. */
			tk_printf("TK_VMem_WritePageToDisk: Page Is Zero\n");
			tk_vmem_pagevnz[pidx>>3]&=~(1<<(pidx&7));
			cpi->flags&=~1;
			return;
		}
		tk_vmem_pagevnz[pidx>>3]|=1<<(pidx&7);

		tk_printf("TK_VMem_WritePageToDisk: Page Nzm=%08X_%08X\n",
			(u32)(nzm>>32), (u32)(nzm>>0));
		
		k=0;
		for(i=0; i<64; i++)
			if(nzm&(1ULL<<i))
				k++;
//		if(k<=48)
		if(k<=56)
		{
			cs=ppi;
			cm=1;

			TK_VMem_PackCompactedPage(tcbuf, ppi);
			j=tcbuf[1]+1;

			if((j<<9)>=(1<<(TK_VMEM_PAGESHL-1)))
				cm=0;
			
			tk_printf("TK_VMem_WritePageToDisk: Page Compact, %dK\n",
				(j+2)>>1);

			if((j<<9)>=(1<<TK_VMEM_PAGESHL))
				{ __debugbreak(); }

#if 0
			ct=tk_vmem_page_tcbuf;
			j=16+(k*(1<<(TK_VMEM_PAGESHL-6)));
			j=((j+511)>>9)-1;
			ct[0]=1;
			ct[1]=j;
			*(u64 *)(ct+8)=nzm;
			ct+=16;

			tk_printf("TK_VMem_WritePageToDisk: Page Compact, %dK\n",
				(j+2)>>1);
			
			for(i=0; i<64; i++)
			{
				if((nzm>>i)&1)
				{
					memcpy(ct, cs, 1<<(TK_VMEM_PAGESHL-6));
					ct+=1<<(TK_VMEM_PAGESHL-6);
				}
				cs+=1<<(TK_VMEM_PAGESHL-6);
			}
#endif
		}else
		{
			cm=2;
			TK_VMem_PackLz64Page(tcbuf, ppi);
			j=tcbuf[1]+1;

			if((j<<9)>=(1<<(TK_VMEM_PAGESHL-1)))
				cm=0;
			
			if(cm==2)
			{
				tk_printf("TK_VMem_WritePageToDisk: Page LZ64A, %dK\n",
					(j+2)>>1);
			}

			cm=0;
//			tk_printf("TK_VMem_WritePageToDisk: Page Is Store\n");
		}
	}

	if(tk_vmem_swap_lba>0)
	{
		lba=tk_vmem_swap_lba+((cpi->pidx)<<TK_VMEM_PGBLKSHL);

		if(!cm)
		{
			TKSPI_WriteSectors(ppi, lba, 1<<TK_VMEM_PGBLKSHL);
			tk_vmem_pagecmz[pidx>>3]&=~(1<<(pidx&7));
		}else
		{
			TKSPI_WriteSectors(tcbuf, lba, tcbuf[1]+1);
			tk_vmem_pagecmz[pidx>>3]|=1<<(pidx&7);
			
		}

		cpi->flags&=~1;
	}
}

void TK_VMem_LoadPageFromDisk(int cidx)
{
	TK_VMem_PageInfo *cpi;
	u64 *ppq;
	byte *ppi, *tcb;
	s64 lba;
	int pidx;
	int i, l;

//	return;

	if(cidx<0)
		{ __debugbreak(); }
	if(cidx>=tk_vmem_npage)
		{ __debugbreak(); }

	cpi=tk_vmem_pageinf+cidx;
	ppi=tk_vmem_pagecache+(cidx<<TK_VMEM_PAGESHL);
	pidx=(cpi->pidx);
	
	if(!(tk_vmem_pagevnz[pidx>>3]&(1<<(pidx&7))))
	{
		/* Zero or Not Initialized Yet. */
//		ppq=(u64 *)ppi;
//		l=1<<(TK_VMEM_PAGESHL-3);
//		for(i=0; i<l; i++)
//			ppq[i]=0;

		memset(ppi, 0, 1<<TK_VMEM_PAGESHL);
		return;
	}

	if(tk_vmem_swap_lba>0)
	{
		lba=tk_vmem_swap_lba+(pidx<<TK_VMEM_PGBLKSHL);
		tcb=tk_vmem_page_tcbuf;

//		if(tk_vmem_pagecmz[cidx>>3]&(1<<(cidx&7)))
		if((tk_vmem_pagecmz[pidx>>3]>>(pidx&7))&1)
		{
			TKSPI_ReadSectors(tcb, lba, 1);
			TKSPI_ReadSectors(tcb+512, lba+1, tcb[1]);
			TK_VMem_UnpackCompactedPage(tcb, ppi);
		}else
		{
			TKSPI_ReadSectors(ppi, lba, 1<<TK_VMEM_PGBLKSHL);
		}
	}
}

int TK_VMem_MapVirtToCacheIdx(int pidx)
{
	TK_VMem_PageInfo *cpi, *lpi, *npi;
	int hi, lp;
	int i, j, k;
	
#if 1
//	hi=((pidx*65521)>>16)&63;
	hi=pidx&255;
	i=tk_vmem_pagehash[hi]; lpi=NULL;
	while(i>=0)
	{
		cpi=tk_vmem_pageinf+i;
		if(cpi->pidx==pidx)
		{
#if 1
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
#endif

			tk_printf("TK_VMem_MapVirtToCacheIdx: Reuse %d\n", i);

			return(i);
		}

		lpi=cpi;
		i=cpi->hnext;
	}
#endif

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
		cpi=tk_vmem_pageinf+i;

		tk_printf("TK_VMem_MapVirtToCacheIdx: Evict %d\n", i);

		if(cpi->flags&1)
		{
			TK_VMem_WritePageToDisk(i);
		}

		TK_VMem_VaEvictPageIndex(i);
		TK_VMem_UnlinkPageHash(i);
		TK_VMem_UnlinkPageLRU(i);
	}
		
//	tk_printf("TK_VMem_MapVirtToCacheIdx: Load Ci=%d Pi=%d\n", i, pidx);

	cpi->pidx=pidx;
	cpi->hi=hi;
	cpi->flags|=1;

	TK_VMem_LoadPageFromDisk(i);

	TK_VMem_AddPageFrontHash(i);
	TK_VMem_AddPageFrontLRU(i);
	return(i);
}

void *TK_VMem_MapCacheIdxToAddr(int cidx)
{
	byte *ppi;
	ppi=tk_vmem_pagecache+(cidx<<TK_VMEM_PAGESHL);
	return(ppi);
}

int TK_VMem_MapPhysIdxToPageNum(int cidx)
{
	int pn;	
	pn=((s64)tk_vmem_pagecache)>>TK_VMEM_PAGESHL;
	return(pn+cidx);
}

int TK_VMem_MapAddrToCacheIdx(void *addr)
{
	s64 ix;
	ix=((byte *)addr)-tk_vmem_pagecache;
	if(ix<0)
		return(-1);
	ix=ix>>TK_VMEM_PAGESHL;
	if(ix>=tk_vmem_npage)
		return(-1);
	return(ix);
}


int TK_VMem_FindFreeSwapPages(int n)
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
		i=TK_VMem_FindFreeSwapPages(n);
		return(i);
	}

	return(-1);
}

int TK_VMem_AllocSwapPage()
{
	int i0, i1;
	int i, j, m;
	
	i=tk_vmem_pagerov;
	m=tk_vmem_maxpage;
	while(i<m)
	{
		if(!(tk_vmem_pagebmp[i>>3]&(1<<(i&7))))
		{
			tk_vmem_pagebmp[i>>3]|=(1<<(i&7));
			tk_vmem_pagerov=i+1;
			return(i);
		}
		
		while((tk_vmem_pagebmp[i>>3]==0xFF) && (i<m))
		{
			i=(i+8)&(~7);
			continue;
		}
		while((tk_vmem_pagebmp[i>>3]&(1<<(i&7))) && (i<m))
			i++;
	}
	
	if(tk_vmem_pagerov!=0)
	{
		tk_vmem_pagerov=0;
		i=TK_VMem_AllocSwapPage();
		return(i);
	}

	return(-1);
}

int TK_VMem_AllocSwapPages(int n)
{
	int i, j, k;
	
	if(n==1)
	{
		i=TK_VMem_AllocSwapPage();
		if(i>=0)
			return(i);
	}

	i=TK_VMem_FindFreeSwapPages(n);
	if(i<0)
	{
		tk_printf("TK_VMem_AllocSwapPages: Out Of Memory, n=%d\n", n);
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

int TK_VMem_FreeSwapPages(int b, int n)
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
		tk_vmem_pagevnz[i>>3]&=~(1<<(i&7));
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

		if(pte&1)
			continue;

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
	int ptpn, pidx, ppn;
	int i, j, k;
	
	for(i=0; i<cnt; i++)
	{
		vtaddr=vaddr+(((s64)i)<<TKMM_PAGEBITS);
		pte=TK_VMem_GetPageTableEntry(vtaddr);

		if(pte&1)
			continue;

		if(!pte)
		{
//			if(TKMM_PAGEBITS==16)
//				{ pte=(2<<8)|(1<<10); }
//			else
//				{ pte=(0<<8)|(1<<10); }
			pte=(0<<8)|(1<<10);
//			TK_VMem_SetPageTableEntry(vtaddr, pte);
		}

		ptpn=(pte>>TK_VMEM_PTESHL)&TK_VMEM_PTEMASK;
		if(!ptpn)
		{
			ptpn=TK_VMem_AllocSwapPages(1);
#if 1
//			TK_VMem_SetPageVAddr(ptpn, vtaddr);
//			pte|=((u64)ptpn)<<12;
			pte|=((u64)ptpn)<<TK_VMEM_PTESHL;
			pte&=~1;
			TK_VMem_SetPageTableEntry(vtaddr, pte);
#endif
		}

#if 0
		pte&=~(TK_VMEM_PTEMASK<<TK_VMEM_PTESHL);
		pidx=TK_VMem_MapVirtToCacheIdx(ptpn);
		TK_VMem_SetPageVAddr(pidx, vtaddr);
		ppn=TK_VMem_MapPhysIdxToPageNum(pidx);
		pte|=((u64)ppn)<<TK_VMEM_PTESHL;
		pte|=1;
		TK_VMem_SetPageTableEntry(vtaddr, pte);
#endif
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
			paddr=((pte>>TK_VMEM_PTESHL)&0xFFFFFFFFFULL)<<TKMM_PAGEBITS;
			pidx=TK_VMem_MapAddrToCacheIdx((void *)paddr);
			if(pidx>=0)
			{
				TK_VMem_VaEvictPageIndex(pidx);

				pte=TK_VMem_GetPageTableEntry(vtaddr);
				if(!pte)
					continue;

				if(pte&1)
					__debugbreak();
			}else
			{
				/* Physical Remap */
				k=TKMM_PointerToPage((void *)paddr);
				TKMM_FreePages(k, 1);
				TK_VMem_SetPageTableEntry(vtaddr, 0);
				TK_VMem_VaFlushVaddr(vtaddr);
				continue;
			}
		}

		ptpn=(pte>>TK_VMEM_PTESHL)&TK_VMEM_PTEMASK;
		if(ptpn)
		{
			TK_VMem_FreeSwapPages(ptpn, 1);
			TK_VMem_SetPageTableEntry(vtaddr, 0);
		}

#if 0
		ptpn=(pte>>TK_VMEM_PTESHL)&TK_VMEM_PTEMASK;
		if(!ptpn)
		{
			ptpn=TK_VMem_AllocSwapPages(1);
//			TK_VMem_SetPageVAddr(ptpn, vtaddr);
//			pte|=((u64)ptpn)<<12;
			pte|=((u64)ptpn)<<TK_VMEM_PTESHL;
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
			pte=(((phaddr>>TKMM_PAGEBITS)+i)<<TK_VMEM_PTESHL)|
				(0<<8)|(1<<10)|1;

//			pte=(0<<8)|(1<<10);
			TK_VMem_SetPageTableEntry(vtaddr, pte);
			TK_VMem_VaFlushVaddr(vtaddr);
		}
	}
}

int TK_VMem_VaDoAllocRemapedPages(s64 vaddr, int cnt)
{
	void *ptr;
	u64 pte;
	s64 vtaddr;
	int phb, pg;
	int i, j, k;
	
	vtaddr=vaddr;
	for(i=0; i<cnt; i++)
	{
//		vtaddr=vaddr+(((s64)i)<<TKMM_PAGEBITS);
		pte=TK_VMem_GetPageTableEntry(vtaddr);
		if(!pte)
		{
//			pg=TKMM_AllocPages(1);
//			pg+=(TKMM_PAGEBASE>>TKMM_PAGEBITS);
			ptr=TKMM_PageAlloc(1<<TKMM_PAGEBITS);
			pg=((u64)ptr)>>TKMM_PAGEBITS;

			pte=(pg<<TK_VMEM_PTESHL)|(0<<8)|(1<<10)|1;
			
//			tk_printf("    %p %08X\n", (void *)vtaddr, pg<<14);

//			pte=(((phaddr>>TKMM_PAGEBITS)+i)<<TK_VMEM_PTESHL)|
//				(0<<8)|(1<<10)|1;

//			pte=(0<<8)|(1<<10);
			TK_VMem_SetPageTableEntry(vtaddr, pte);
			TK_VMem_VaFlushVaddr(vtaddr);
		}

		if(!(pte&1))
		{
			__debugbreak();
		}
		
		vtaddr+=1<<TKMM_PAGEBITS;
	}

#if 1
	pte=TK_VMem_GetPageTableEntry(vtaddr);
	if(!pte)
	{
		pg=0x10000>>TKMM_PAGEBITS;
		pte=(pg<<TK_VMEM_PTESHL)|(0<<8)|(1<<10)|1;
		TK_VMem_SetPageTableEntry(vtaddr, pte);
		TK_VMem_VaFlushVaddr(vtaddr);
	}
#endif
}

void TK_VMem_VaFlushVaddr(s64 vaddr)
{
	u64 pte;

	/* Attempt to flush page from TLB */
	pte=(0<<8)|(1<<10);
	tk_vmem_loadpte(vaddr, pte);
	tk_vmem_loadpte(vaddr, pte);
	tk_vmem_loadpte(vaddr, pte);
	tk_vmem_loadpte(vaddr, pte);

	tk_vmem_loadpte(vaddr, pte);
	tk_vmem_loadpte(vaddr, pte);
}

void TK_VMem_VaEvictPageIndex(int cidx)
{
	s64 vaddr;
	u64 pte;
	int ptpn;

	vaddr=TK_VMem_GetPageVAddr(cidx);
	ptpn=TK_VMem_GetPagePidx(cidx);

//	if(TKMM_PAGEBITS==16)
//		{ pte=(2<<8)|(1<<10); }
//	else
//		{ pte=(0<<8)|(1<<10); }
	pte=(0<<8)|(1<<10);
	pte|=((u64)ptpn)<<TK_VMEM_PTESHL;
	TK_VMem_SetPageTableEntry(vaddr, pte);
	
	/* Attempt to flush page from TLB */
//	tk_vmem_loadpte(vaddr, pte);
//	tk_vmem_loadpte(vaddr, pte);
//	tk_vmem_loadpte(vaddr, pte);
//	tk_vmem_loadpte(vaddr, pte);

	TK_VMem_VaFlushVaddr(vaddr);
}

void TK_VMem_VaPageInAddr(s64 vaddr)
{
	s64 vaddr1;
	u64 pte;
	int ptpn;
	int pidx;
	int ppn;

	vaddr1=vaddr&(~((1<<TKMM_PAGEBITS)-1));

	pte=TK_VMem_GetPageTableEntry(vaddr1);
	if(pte&1)
	{
		tk_vmem_loadpte(vaddr, pte);
		return;
	}

	ptpn=(pte>>TK_VMEM_PTESHL)&TK_VMEM_PTEMASK;
	if(!ptpn)
	{
#if 1
		tk_printf("TK_VMem_VaPageInAddr: Debug Pad, %p\n", vaddr1);
		ptpn=TK_VMem_AllocSwapPages(1);
		pte|=((u64)ptpn)<<TK_VMEM_PTESHL;
		pte&=~1;
		TK_VMem_SetPageTableEntry(vaddr, pte);
#endif

#if 0
//		pte=0x0F1;
//		pte=0x00010001;
		pte=0x000100F1;
		TK_VMem_SetPageTableEntry(vaddr, pte);

		tk_vmem_loadpte(vaddr, pte);
		return;
#endif
	}

//	pte&=~((16777215ULL)<<12);
	pte&=~(TK_VMEM_PTEMASK<<TK_VMEM_PTESHL);
	pidx=TK_VMem_MapVirtToCacheIdx(ptpn);
	TK_VMem_SetPageVAddr(pidx, vaddr1);
	ppn=TK_VMem_MapPhysIdxToPageNum(pidx);
	pte|=((u64)ppn)<<TK_VMEM_PTESHL;
	pte|=1;
	TK_VMem_SetPageTableEntry(vaddr1, pte);
	
//	tk_printf("TK_VMem_VaPageInAddr: %p %p\n", vaddr, pte);

	TK_VMem_VaFlushVaddr(vaddr1);

	tk_vmem_loadpte(vaddr1, pte);
}

s64 TK_VMem_VaFindFreePagesBasic(int cnt, int flag)
{
	u64 pte;
	s64 vta;
	int vpn;
	int i, j, k;
	
	if(!tk_vmem_pagecache)
		return(0);
	
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

s64 TK_VMem_VaFindFreePagesAslr(int cnt, int flag)
{
	s64 addr;
	int n, qfl;

	if(!tk_vmem_pagecache)
		return(0);

	n=256;
	while((n--)>0)
	{
//		addr=rand();
//		addr=(addr*4093)+rand();
//		addr=(addr*4093)+rand();
		addr=TK_GetRandom48ASLR();
		addr=(addr<<TKMM_PAGEBITS)&0x00003FFFFFFFFFFFLL;
		
		if(TK_VMem_CheckAddrIsVirtual(addr))
		{
			qfl=TK_VMem_VaQueryPages(addr, cnt+1);
			if(qfl==1)
			{
				return(addr);
			}
		}
	}
	return(0);
}

s64 TK_VMem_VaFindFreePages(int cnt, int flag)
{
//	return(TK_VMem_VaFindFreePagesBasic(cnt, flag));
	return(TK_VMem_VaFindFreePagesAslr(cnt, flag));
}

s64 TK_VMem_VaFindFreePagesLowBasic(int cnt, int flag)
{
	u64 pte;
	s64 vta;
	int vpn, vps, vpe;
	int i, j, k;
	
	if(cnt<=0)
		return(0);
	
//	if(!tk_vmem_pagecache)
//		return(0);
	
	vps=TKMM_VALOSTART>>TKMM_PAGEBITS;
	vpe=TKMM_VALOEND>>TKMM_PAGEBITS;
	
	vpn=tk_vmem_varov_lo;
	while(1)
	{
		vta=((s64)vpn)<<TKMM_PAGEBITS;
		pte=TK_VMem_GetPageTableEntry(vta);
		if(pte)
		{
			vpn++;
			continue;
		}
		
		if((vpn+cnt)>vpe)
		{
			vpn=0;
			continue;
		}
		
		if((vpn<tk_vmem_varov_lo) && ((vpn+cnt)>tk_vmem_varov_lo))
			return(0);

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

s64 TK_VMem_VaFindFreePagesLowAslr(int cnt, int flag)
{
	s64 addr;
	u32 vps, vpe, vpn;
	int n, qfl;

	if(!tk_vmem_pagecache)
		return(0);

	vps=TKMM_VALOSTART>>TKMM_PAGEBITS;
	vpe=TKMM_VALOEND>>TKMM_PAGEBITS;

	n=1024;
	while((n--)>0)
	{
//		vpn=rand();
//		vpn=(addr*251)+rand();
//		vpn=TK_GetRandom();

		addr=TK_GetRandom48ASLR();
		vpn=addr^(addr>>32);

		vpn&=(1<<(31-TKMM_PAGEBITS))-1;
		
		if(vpn<vps)
			vpn+=vps;
		if(vpn>vpe)
			vpn-=vps;
		
		if((vpn<vps) || ((vpn+cnt)>=vpe))
			continue;
		
		addr=((s64)vpn)<<TKMM_PAGEBITS;

		qfl=TK_VMem_VaQueryPages(addr, cnt+1);
		if(qfl==1)
		{
			return(addr);
		}

//		return(addr);
	}
	return(0);
}

s64 TK_VMem_VaFindFreePagesLow(int cnt, int flag)
{
//	return(TK_VMem_VaFindFreePagesLowBasic(cnt, flag));
	return(TK_VMem_VaFindFreePagesLowAslr(cnt, flag));
}

s64 TK_VMem_VaVirtualAlloc(s64 addr, s64 size, int flProt, int flMap)
{
	int vpn, vpb;
	int qfl;

//	__debugbreak();
//	return(0);

#if 1
	vpb=addr>>TKMM_PAGEBITS;
//	vpn=size>>TKMM_PAGEBITS;
//	vpn=(size+((1<<TKMM_PAGEBITS)-1))>>TKMM_PAGEBITS;
	vpn=((size+32)+((1<<TKMM_PAGEBITS)-1))>>TKMM_PAGEBITS;
//	vpn=(size+(1<<TKMM_PAGEBITS))>>TKMM_PAGEBITS;

	if(addr)
	{
		qfl=TK_VMem_VaQueryPages(addr, vpn);
		
		if(qfl==1)
		{
			if(TK_VMem_CheckAddrIsVirtual(addr))
			{
//				TK_VMem_VaReservePages(addr, vpn);
				TK_VMem_VaCommitPages(addr, vpn);
				return(addr);
			}else
			{
				TK_VMem_VaDoAllocRemapedPages(addr, vpn);
				return(addr);
			}
		}
	}

//	if(1)
//	if(0)
	if(tk_vmem_swap_disable || (flMap&TKMM_MAP_32BIT))
	{
//		tk_printf("TK_VMem_VaVirtualAlloc: A0\n");
		addr=TK_VMem_VaFindFreePagesLow(vpn, flMap);
//		tk_printf("TK_VMem_VaVirtualAlloc: A1\n");
		if(!addr)
			return(0);
//		tk_printf("TK_VMem_VaVirtualAlloc: A2 %p\n", (void *)addr);
		TK_VMem_VaDoAllocRemapedPages(addr, vpn);
		tk_vmem_varov_lo=(addr>>TKMM_PAGEBITS)+vpn;
//		tk_printf("TK_VMem_VaVirtualAlloc: A3\n");
		return(addr);
	}

	
	addr=TK_VMem_VaFindFreePages(vpn, flMap);
	if(!addr)
		return(0);
	
//	tk_printf("TK_VMem_VaVirtualAlloc: VA=%p..%p Vpn=%d\n",
//		addr, addr+(vpn<<TKMM_PAGEBITS), vpn);
	
	if(flMap&TKMM_MAP_DIRECT)
	{
		TK_VMem_VaDoAllocRemapedPages(addr, vpn);
	}else
	{
//		TK_VMem_VaReservePages(addr, vpn);
		TK_VMem_VaCommitPages(addr, vpn);
	}
	tk_vmem_varov_hi=(addr>>TKMM_PAGEBITS)+vpn;
	return(addr);
#endif
}

int TK_VMem_VaVirtualFree(s64 vaddr, s64 size)
{
	return(TK_VMem_VaFreePages(vaddr, size>>TKMM_PAGEBITS));
}

int TK_VMem_CheckAddrIsVirtual(s64 addr)
{
	u64 addr1;
	
	addr1=addr&0x0000FFFFFFFFFFFFULL;
	
//	if(((addr>>32)&0xFFFF)!=0x0000)
	if((addr1>=TKMM_VAS_START_HI) && (addr1<TKMM_VAS_END_HI))
		return(1);
	return(0);
}

int TK_VMem_CheckPtrIsVirtual(void *ptr)
{
	return(TK_VMem_CheckAddrIsVirtual((s64)ptr));
}

void tk_vmem_tlbmiss(u64 ttb, u64 tea)
{
	u64 pte, paddr;
	int cidx;

#if 0
//	if(!pte)
	if(1)
	{
		tea&=0x0000FFFFFFFFFFFFULL;
		pte=((tea>>TKMM_PAGEBITS)<<TK_VMEM_PTESHL)|1;
		tk_vmem_loadpte(tea, pte);
		return;
	}
#endif
	
//	__debugbreak();
	
	pte=TK_VMem_GetPageTableEntry(tea);
	if(pte&1)
	{
		if(TK_VMem_CheckAddrIsVirtual(tea))
		{
			paddr=((pte>>TK_VMEM_PTESHL)&0xFFFFFFFFFULL)<<TKMM_PAGEBITS;
			cidx=TK_VMem_MapAddrToCacheIdx((void *)paddr);
			if(cidx>=0)
			{
				TK_VMem_MovePageFrontLRU(cidx);
			}
		}

//		__debugbreak();
		tk_vmem_loadpte(tea, pte);
//		__debugbreak();
		return;
	}
	
#if 0
	if(!pte)
	{
		tea&=0x0000FFFFFFFFFFFFULL;
		pte=((tea>>TKMM_PAGEBITS)<<TK_VMEM_PTESHL)|1;

//		__debugbreak();
//		pte=((0x10000>>TKMM_PAGEBITS)<<TK_VMEM_PTESHL)|1;
		tk_vmem_loadpte(tea, pte);
		return;
	}
#endif
	
	if(TK_VMem_CheckAddrIsVirtual(tea))
	{
//		__debugbreak();
		TK_VMem_VaPageInAddr(tea);
	}else
	{
		if(tea<TKMM_VALOSTART)
		{
			/* Physical Mapped Range */

			tea&=0x0000FFFFFFFFFFFFULL;
			pte=((tea>>TKMM_PAGEBITS)<<TK_VMEM_PTESHL)|1;
			tk_vmem_loadpte(tea, pte);
			return;
		}else
		{
			__debugbreak();

			/* Direct Mapping Range */

			tea=0x10000ULL;
			pte=((tea>>TKMM_PAGEBITS)<<TK_VMEM_PTESHL)|1;
			tk_vmem_loadpte(tea, pte);
			return;
		}
	}
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
