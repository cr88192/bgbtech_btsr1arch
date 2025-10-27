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
int		pidx;		//00, page index (pagefile)
byte	hi;			//04, hash index
byte	pad0;		//05, pad
u16		flags;		//06, flags
s32		next;		//08, next in LRU
s32		prev;		//0C, prev in LRU
u32		vapidx;		//10, virtual address page index
s32		hnext;		//14, next in hash
s32		hprev;		//18, prev in hash
u16		pgchk;		//1C, page checksum
u16		pad3;		//1E, pad
//20
u64		vaphi;		//20, high bits of address
u64		pad4;		//28
u64		pad5;		//30
u64		pad6;		//38
//40
};

#ifdef __BJX2__

extern volatile u64 __arch_ttb;
extern volatile u64 __arch_tea;
extern volatile u64 __arch_exsr;
extern volatile u64 __arch_mmcr;
extern volatile u64 __arch_sttb;
extern volatile u64 __arch_krr;
extern volatile u64 __arch_tbr;

extern volatile u64 __arch_pch;
extern volatile u64 __arch_gbh;

extern volatile void *__arch_isrsave;		/* Pseudo */

#endif

int tk_syscall_utxt(void *sobj, int umsg, void *pptr, void *args);
int tk_syscall_rv_utxt(void *sobj, int umsg, void *pptr, void *args);


/* Model TLB so we can check if a page is likely already evicted.
 * The need for this is not an ideal turn of events.
 * This is needed to avoid unneeded TLB VADDR flushes, which
 * carry a risk of getting stuck in infinite TLB miss loops.
 */
u32 tk_vmem_faketlb_vpn[256*4];
u32 tk_vmem_faketlb_ppn[256*4];

byte tk_vmem_faketlb_pix[256*8];

u64 tk_vmem_tlbmisshist[256];
byte tk_vmem_tlbmisshrov;
byte tk_vmem_tlbflushinhibit;



byte *tk_vmem_pagecache=NULL;	//page cache memory
byte *tk_vmem_pagecacheub=NULL;	//page cache memory
TK_VMem_PageInfo *tk_vmem_pageinf;
TK_VMem_PageInfo *tk_vmem_pageinfub;
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

u64	tk_vmem_pageglobal=0;
u64	tk_vmem_aclglobal=0;

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
byte *tk_vmem_page_tdbuf;	//temporary compression buffer

int tk_vmem_varov_lo;
int tk_vmem_varov_hi;

byte tk_vm_lazyflush=0;

#ifdef __BJX2__
// #if 0
extern volatile u64 __arch_ttb;
extern volatile u64 __arch_tea;
extern volatile u64 __arch_teah;
extern volatile u64 __arch_exsr;
extern volatile u64 __arch_mmcr;
extern volatile u64 __arch_sttb;
extern volatile u64 __arch_spc;

extern volatile u64 __arch_gbh;
extern volatile u64 __arch_pch;
#endif

void TK_VMem_VaEvictPageIndex(int pidx);
void TK_VMem_VaDiscardPageIndex(int cidx);
void TK_VMem_VaFlushVaddr(s64 vaddr);
void TK_VMem_VaFlushVaddr2(s64 vaddr, s64 vaddrh);
int TK_VMem_CheckAddrIsVirtual2(s64 addr, s64 addrh);

void *tk_ptrsetbound1(void *ptr, int size);
void *tk_ptrsetbound2(void *ptr, int lobnd, int hibnd);

void tk_vmem_flush_faketlb(void);
void tk_vmem_loadpte_faketlb(u64 tva, u64 pte);
u64 TK_VMem_QueryTlbVaForVirtPage(u64 vaddr);

void tk_vmem_loadacl(u64 acle);


u64 *TK_VMem_PtrForPto(u64 pto)
{
	if(!(pto&1))
		{ __debugbreak(); }
	return((u64 *)(pto&(~((1ULL<<TKMM_PAGEBITS)-1))));
}

u64 TK_VMem_PtoForPtr(u64 *ptr)
{
	return((u64)ptr);
}

#include "tk_vmptavl.c"
#include "tk_vmabtree.c"
#include "tk_vmahy2bt.c"

#include "tk_vmlz64a.c"
#include "tk_vmfelz32.c"

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
u64 TK_VMem_GetPageTableEntry2(s64 vaddr, s64 vaddrh)
{
	u64 *ptp;
	u64 pde, pte, vpn;
	int vp0, vp1, vp2, vp3;
	int vpt, ptpn;

#ifdef __BJX2__
	pde=__arch_ttb;
//	if((pde&15)==0x4)
	if((pde&0x13)==0x13)
	{
		if((pde&0x300)==0x000)
			{ pte=TK_VMem_GetAvlPageTableEntry(pde, vaddr, vaddrh); }
		else if((pde&0x300)==0x100)
			{ pte=TK_VMem_GetBTreePageTableEntry(pde, vaddr, vaddrh); }
		else if((pde&0x300)==0x200)
			{ pte=TK_VMem_GetBTreeHybPageTableEntry(pde, vaddr, vaddrh); }
		else if((pde&0x300)==0x300)
			{ pte=TK_VMem_GetHyb2PageTableEntry(pde, vaddr, vaddrh); }
		return(pte);
	}

//	ptp=(u64 *)(pde&(~4095ULL));
	ptp=(u64 *)(0xC00000000000ULL|(pde&(~((1<<TK_VMEM_PAGESHL)-1))));

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
		{
			if(pde)
				{ __debugbreak(); }
			return(0);
		}
		ptpn=(pde>>TK_VMEM_PTESHL)&TK_VMEM_PTEMASK;
//		ptp=(u64 *)(ptpn<<TK_VMEM_PAGESHL);
		ptp=(u64 *)(0xC00000000000ULL|(ptpn<<TK_VMEM_PAGESHL));

		pde=ptp[vp1];
		if(!(pde&1))
		{
			if(pde)
				{ __debugbreak(); }
			return(0);
		}
		ptpn=(pde>>TK_VMEM_PTESHL)&TK_VMEM_PTEMASK;
//		ptp=(u64 *)(ptpn<<TK_VMEM_PAGESHL);
		ptp=(u64 *)(0xC00000000000ULL|(ptpn<<TK_VMEM_PAGESHL));

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
#endif
}

u64 TK_VMem_GetPageTableEntry(s64 vaddr)
{
	return(TK_VMem_GetPageTableEntry2(vaddr, 0));
}

#endif

u64 TK_VMem_EncCheckPageTableEntry(u64 pteval)
{
	int h;
	u64 pte1;

	if(!pteval)
		return(pteval);

	pte1=pteval&(~(7ULL<<11));
	h=pte1^(pte1>>32);
	h=h^(h>>16);
	h=h^(h>>8);
	h=h^(h>>4);
	pte1|=(h&7)<<11;

	return(pte1);
}

void TK_VMem_ValidatePageTableEntry(u64 pteval)
{
	int h;
	u64 pte1;

	if(!pteval)
		return;

	pte1=pteval&(~(7ULL<<11));
	h=pte1^(pte1>>32);
	h=h^(h>>16);
	h=h^(h>>8);
	h=h^(h>>4);
	pte1|=(h&7)<<11;

	if(pte1!=pteval)
	{
		__debugbreak();
	}
}

int TK_VMem_SetPageTableEntry2(s64 vaddr, s64 vaddrh, u64 ptval)
{
	u64 *ptp;
	u64 pde, pte, vpn;
	int vp0, vp1, vp2, vp3;
	int vpt, ptpn;

#ifdef __BJX2__
	pde=__arch_ttb;
//	if((pde&15)==0x4)
	if((pde&0x13)==0x13)
	{
//		TK_VMem_SetAvlPageTableEntry(pde, vaddr, 0, ptval);
		if((pde&0x300)==0x000)
			{ TK_VMem_SetAvlPageTableEntry(pde, vaddr, vaddrh, ptval); }
		else if((pde&0x300)==0x100)
			{ TK_VMem_SetBTreePageTableEntry(pde, vaddr, vaddrh, ptval); }
		else if((pde&0x300)==0x200)
			{ TK_VMem_SetBTreeHybPageTableEntry(pde, vaddr, vaddrh, ptval); }
		else if((pde&0x300)==0x300)
			{ TK_VMem_SetHyb2PageTableEntry(pde, vaddr, vaddrh, ptval); }
		return(0);
	}

//	ptp=(u64 *)(pde&(~4095ULL));
//	ptp=(u64 *)(pde&(~((1<<TK_VMEM_PAGESHL)-1)));
	ptp=(u64 *)(0xC00000000000ULL|(pde&(~((1<<TK_VMEM_PAGESHL)-1))));

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
//				ptpn=TKMM_AllocPagesZeroed(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
				ptpn=TKMM_AllocPagesZeroedApn(1);
				pde=(((u64)ptpn)<<TK_VMEM_PTESHL)|1;

				if((pde>>28)!=0)
					{ __debugbreak(); }

				if(!ptp[vp0])
				{
					ptp[vp0]=pde;
				}else
				{
					pde=ptp[vp0];
					TKMM_FreePagesApn(ptpn, 1);
				}
			}else
			{
				__debugbreak();
			}
		}
		ptpn=(pde>>TK_VMEM_PTESHL)&TK_VMEM_PTEMASK;
//		ptp=(u64 *)(ptpn<<14);
		ptp=(u64 *)(0xC00000000000ULL|(ptpn<<TK_VMEM_PAGESHL));

		pde=ptp[vp1];
		if(!(pde&1))
		{
			if(!pde)
			{
//				ptpn=TKMM_AllocPages(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
//				ptpn=TKMM_AllocPagesZeroed(1)+(TKMM_PAGEBASE>>TKMM_PAGEBITS);
				ptpn=TKMM_AllocPagesZeroedApn(1);
				pde=(((u64)ptpn)<<TK_VMEM_PTESHL)|1;

				if((pde>>28)!=0)
					{ __debugbreak(); }

				if(!ptp[vp1])
				{
					ptp[vp1]=pde;
				}else
				{
					pde=ptp[vp1];
					TKMM_FreePagesApn(ptpn, 1);
				}
			}else
			{
				__debugbreak();
			}
		}

		ptpn=(pde>>TK_VMEM_PTESHL)&TK_VMEM_PTEMASK;
//		ptp=(u64 *)(ptpn<<14);
		ptp=(u64 *)(0xC00000000000ULL|(ptpn<<TK_VMEM_PAGESHL));

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
//				tk_dbg_printf("TK_VMem_SetPageTableEntry: "
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
//		ptp=(u64 *)(0xC00000000000ULL|(ptpn<<14));

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
#endif
}

int TK_VMem_SetPageTableEntry(s64 vaddr, u64 ptval)
{
	return(TK_VMem_SetPageTableEntry2(vaddr, 0, ptval));
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
void tk_vmem_loadpte8x(u64 tva, u64 pte);
void tk_vmem_loadptehi(u64 tva, u64 pte);
void tk_vmem_loadacl(u64 acle);
void tk_vmem_loadptehi(u64 tva, u64 pte);

void tk_vmem_l1flush();
void tk_vmem_tlbflush();
void tk_vmem_setsrjq(int fl);

__asm {

tk_vmem_tlbflush:
	INVTLB
	RTS

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

	INVDC	R2
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

	MOV		TTB, R6
	AND		R6, R19, R6
	OR		R6, R2

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


tk_vmem_loadpte8x:
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

	MOV		TTB, R6
	AND		R6, R19, R6
	OR		R6, R2

	MOV		R2, R0
	MOV		R3, R1

	NOP
	NOP
	NOP
	NOP

	LDTLB
	NOP
	LDTLB
	NOP
	LDTLB
	NOP
	LDTLB
	NOP
	LDTLB
	NOP
	LDTLB
	NOP
	LDTLB
	NOP
	LDTLB
	NOP

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

	MOV		TTB, R6
	AND		R6, R19, R6
	OR		R6, R2

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


tk_vmem_loadpte8x:
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

	MOV		TTB, R6
	AND		R6, R19, R6
	OR		R6, R2

	MOV		R2, R0
	MOV		R3, R1

	NOP
	NOP
	NOP
	NOP

	LDTLB
	NOP
	LDTLB
	NOP
	LDTLB
	NOP
	LDTLB
	NOP
	LDTLB
	NOP
	LDTLB
	NOP
	LDTLB
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

	MOV		TTB, R6
	AND		R6, R19, R6
	OR		R6, R2

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

tk_vmem_loadpte8x:
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

	MOV		TTB, R6
	AND		R6, R19, R6
	OR		R6, R2

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

	LDTLB
	NOP
	LDTLB
	NOP
	LDTLB
	NOP
	LDTLB
	NOP
	LDTLB
	NOP
	LDTLB
	NOP
	LDTLB
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

#if 1
tk_vmem_loadptehi:
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

void tk_vmem_flush_faketlb(void)
{
	u32 *vpn, *ppn;
	int i;
	
	vpn=tk_vmem_faketlb_vpn;
	ppn=tk_vmem_faketlb_ppn;
	for(i=0; i<(256*4); i++)
		{ vpn[i]=0; ppn[i]=0; }

//	memset(tk_vmem_faketlb_vpn, 256*4*sizeof(u32));
//	memset(tk_vmem_faketlb_ppn, 256*4*sizeof(u32));
}

void tk_vmem_loadpte_faketlb(u64 tva, u64 pte)
{
	u32 *vpn, *ppn, *avpn, *appn;
	byte *pix, *apix;
	u32 pva, ppa;
	u32 i0, i1, i2, i3, i4, i5, i6, i7;
	int tlbix, psix, ttix;

	pva=tva>>TK_VMEM_PAGESHL;
	ppa=pte>>TK_VMEM_PAGESHL;
	tlbix=pva&255;
	psix=(ppa&255);

	vpn=tk_vmem_faketlb_vpn;
	ppn=tk_vmem_faketlb_ppn;
	pix=tk_vmem_faketlb_pix;

	ttix=tlbix<<2;
	avpn=vpn+ttix;
	appn=ppn+ttix;

	apix=pix+(psix<<3);

	if(!(pte&3))
	{
		avpn[3]=0;	avpn[2]=0;
		avpn[1]=0;	avpn[0]=0;
		appn[3]=0;	appn[2]=0;
		appn[1]=0;	appn[0]=0;
		return;
	}

//	i3=avpn[3];
	i2=avpn[2];
	i1=avpn[1];	i0=avpn[0];
	avpn[3]=i2;	avpn[2]=i1;
	avpn[1]=i0;	avpn[0]=pva;

//	i3=appn[3];
	i2=appn[2];
	i1=appn[1];	i0=appn[0];
	appn[3]=i2;	appn[2]=i1;
	appn[1]=i0;	appn[0]=ppa;

//	i7=apix[7];
	i6=apix[6];
	i5=apix[5];	i4=apix[4];
	apix[7]=i6;	i3=apix[3];
	apix[6]=i5;	i2=apix[2];
	apix[5]=i4;	i1=apix[1];
	apix[4]=i3;	i0=apix[0];
	apix[3]=i2;	apix[2]=i1;
	apix[1]=i0;	apix[0]=tlbix;
}

/* Query if a physical page is still in the model of the TLB.
 * If found, return the virtual address associated with the page.
 */
u64 TK_VMem_QueryTlbVaForPhysPage(u64 pte)
{
	u64 va;
	int ppa, pva, psix, ttix, tlbix;
	int i, j, k;

	ppa=pte>>14;
	psix=(ppa&255);
	ttix=psix<<3;

	for(i=0; i<8; i++)
	{
		tlbix=tk_vmem_faketlb_pix[ttix+i];
		k=tlbix<<2;
		for(j=0; j<4; j++)
		{
			if(tk_vmem_faketlb_ppn[k+j]==ppa)
			{
				pva=tk_vmem_faketlb_vpn[k+j];
				va=((u64)pva)<<14;
				return(va);
			}
		}
	}

	return(0);
}

/* Query if virtual address is in the model of the TLB.
 * Return the virtual address if true, NULL otherwise.
 */
u64 TK_VMem_QueryTlbVaForVirtPage(u64 vaddr)
{
//	return(1);

#if 1
	u32 pva, ppa;
	u32 i0, i1, i2, i3, i4, i5, i6, i7;
	int tlbix, psix, ttix;

	pva=vaddr>>14;
//	ppa=pte>>14;
	tlbix=pva&255;

	ttix=tlbix<<2;
	i3=tk_vmem_faketlb_vpn[ttix+3];
	i2=tk_vmem_faketlb_vpn[ttix+2];
	i1=tk_vmem_faketlb_vpn[ttix+1];
	i0=tk_vmem_faketlb_vpn[ttix+0];

	if(i0==pva)
		return(vaddr);
	if(i1==pva)
		return(vaddr);
	if(i2==pva)
		return(vaddr);
	if(i3==pva)
		return(vaddr);
	return(0);
#endif
}


void tk_vmem_loadpte2(u64 tval, u64 tvah, u64 pte)
{
	u64 pteh;

	if(!tvah)
	{
		tk_vmem_loadpte_faketlb(tval, pte);
		tk_vmem_loadpte(tval, pte);
		return;
	}

	pteh=0x0000000000000002ULL;
	tk_vmem_loadptehi(tvah, pteh);

	tk_vmem_loadpte(tval, pte);
	return;
}


#else

void tk_vmem_loadpte(u64 tva, u64 pte)
{
}

void tk_vmem_loadpte2(u64 tva, u64 tvah, u64 pte)
{
}

void tk_vmem_loadpte8x(u64 tva, u64 pte)
{
}

u64 TK_VMem_QueryTlbVaForPhysPage(u64 pte)
{
	return(0);
}

#endif

void *TK_VMem_PointerAsPhysical(void *ptr)
{
	void *ptr1;
	long lv;
	lv=(long)ptr;
	lv|=0xC00000000000ULL;
	ptr1=(void *)lv;
	return(ptr1);
}

#ifdef __BJX2__
extern byte __utext_start;
extern byte __utext_end;
#endif

int TK_VMem_Init()
{
	char tblk1[512], tblk2[512];
	TK_VMem_PageInfo *cpi, *lpi, *npi;
	u32 *conbuft;
	void *tp0;
	s64 tva;
	u64 tpte, tpte1, tmmcr;
	int np, b, lba, pg;
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
			tk_vmem_usrexpage=TKMM_PageAllocL(1<<TK_VMEM_PAGESHL);
//			tk_vmem_usrexonly=(void *)TKMM_EXOSTART;
			tk_vmem_usrexonly=tk_vmem_usrexpage;
			tk_vmem_usrexoffs=0;
			tk_vmem_useldekrr=0;
		}

		tk_vmem_swap_disable=1;

//		/* Don't set up virtual memory if no pagefile. */
//		return(0);
	}

	tk_dbg_printf("TK_VMem_Init\n");

	tk_vmem_swap_disable=0;

#if 0
	for(i=0; i<512; i++)
	{
		tblk1[i]=i;
		tblk2[i]=0;
	}
#endif

	lba=tk_vmem_swap_lba;
	if(lba>0)
	{
		TKSPI_ReadSectors(tblk2, lba, 1);
	}

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

//	TK_SeedRandomASLR();
#endif

	lba=tk_vmem_swap_lba;
	if(lba>0)
	{
		TKSPI_WriteSectors(tblk1, lba, 1);
		TKSPI_ReadSectors(tblk2, lba, 1);

		for(i=0; i<512; i++)
		{
			if(tblk1[i]!=tblk2[i])
			{
				tk_dbg_printf("TK_VMem_Init: Swap IO Check Fail\n");
				tk_vmem_swap_disable=1;
				break;
			}
		}
	}else
	{
		tk_dbg_printf("TK_VMem_Init: VMem with No Swap\n");
		tk_vmem_swap_disable=1;
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
	tk_dbg_printf("TK_VMem_Init: A-0\n");

//	np=8192;
	np=(32768<<10)>>TK_VMEM_PAGESHL;
	tk_vmem_pagecache=TKMM_PageAllocL(np<<TK_VMEM_PAGESHL);
	tk_vmem_npage=np;

//	tk_vmem_pageinf=tk_malloc_cat(np*sizeof(TK_VMem_PageInfo), 2);
	tk_vmem_pageinf=TKMM_PageAllocL(np*sizeof(TK_VMem_PageInfo));
	tk_vmem_pageinf=tk_ptrsetbound1(tk_vmem_pageinf,
		np*sizeof(TK_VMem_PageInfo));

	memset(tk_vmem_pageinf, 0, np*sizeof(TK_VMem_PageInfo));
#endif

	tk_dbg_printf("TK_VMem_Init: A-1\n");

	pg=TKMM_AllocPagesZeroedApn(1);
	tk_vmem_aclroot=(u64 *)(((u64)pg)<<TK_VMEM_PAGESHL);

	pg=TKMM_AllocPagesZeroedApn(1);
	tk_vmem_pageroot=(u64 *)(((u64)pg)<<TK_VMEM_PAGESHL);

//	tk_vmem_pageroot=TKMM_PageAllocL(1<<TK_VMEM_PAGESHL);
//	memset(tk_vmem_pageroot, 0, 1<<TK_VMEM_PAGESHL);

//	tk_vmem_aclroot=TKMM_PageAllocL(1<<TK_VMEM_PAGESHL);
//	memset(tk_vmem_aclroot, 0, 1<<TK_VMEM_PAGESHL);

#ifdef __BJX2__
	tpte=(u64)tk_vmem_pageroot;

	if(TK_VMEM_PAGESHL==14)
	{
		tpte|=0x0042;	//Three-Level, 16K pages
//		tpte|=0x0153;	//B-Tree
//		tpte|=0x0253;	//B-Tree (Hybrid)
//		tpte|=0x0353;	//B-Tree (Hybrid-2)
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

	tk_vmem_pageglobal=tpte;
	__arch_ttb=tpte;
#endif

#if 1
	if(!tk_vmem_swap_disable)
	{
//		i=(tk_vmem_swap_psz+7)/8;
		i=((tk_vmem_swap_psz+63)/64)*8;
//		tk_vmem_pagebmp=tk_malloc_cat(i, 2);
//		tk_vmem_pagevnz=tk_malloc_cat(i, 2);
		tk_vmem_pagebmp=TKMM_PageAllocL(i);
		tk_vmem_pagevnz=TKMM_PageAllocL(i);
		tk_vmem_pagecmz=TKMM_PageAllocL(i);

		tk_vmem_pagebmp=TK_VMem_PointerAsPhysical(tk_vmem_pagebmp);
		tk_vmem_pagevnz=TK_VMem_PointerAsPhysical(tk_vmem_pagevnz);
		tk_vmem_pagecmz=TK_VMem_PointerAsPhysical(tk_vmem_pagecmz);

		tk_vmem_pagebmp=tk_ptrsetbound1(tk_vmem_pagebmp, i);
		tk_vmem_pagevnz=tk_ptrsetbound1(tk_vmem_pagevnz, i);
		tk_vmem_pagecmz=tk_ptrsetbound1(tk_vmem_pagecmz, i);

		memset(tk_vmem_pagebmp, 0, i);
		memset(tk_vmem_pagevnz, 0, i);
		memset(tk_vmem_pagecmz, 0, i);

		tk_vmem_page_tcbuf=TKMM_PageAllocL(2<<TK_VMEM_PAGESHL);
		tk_vmem_page_tdbuf=TKMM_PageAllocL(2<<TK_VMEM_PAGESHL);

		tk_vmem_page_tcbuf=TK_VMem_PointerAsPhysical(tk_vmem_page_tcbuf);
		tk_vmem_page_tdbuf=TK_VMem_PointerAsPhysical(tk_vmem_page_tdbuf);

		tk_vmem_pagebmp[0]|=1;

//		memset(tk_vmem_pagebmp, 0, 16384);
		tk_vmem_maxpage=tk_vmem_swap_psz;
		tk_vmem_pagerov=0;
	}
#endif

	tk_dbg_printf("TK_VMem_Init: A-2\n");

#if 1
	/* Identity Map RAM/etc */
//	n=(TKMM_PAGEEND>>TK_VMEM_PAGESHL);
	n=(TKMM_VAS_START_LO>>TK_VMEM_PAGESHL);
	for(i=256; i<n; i++)
	{
		tva=((u64)i)<<TK_VMEM_PAGESHL;
//		tpte=(2<<8)|(1<<10)|1;
		tpte=(0<<8)|(1<<10)|1;
		tpte|=i<<TK_VMEM_PTESHL;

		if(tva<TKMM_PAGEBASE)
			tpte|=(1<<7);	//No User

		tpte=TK_VMem_EncCheckPageTableEntry(tpte);

		TK_VMem_SetPageTableEntry(tva, tpte);
//		tk_vmem_loadpte(tva, tpte);
	}
#endif

#if 1
	/* Validate Identity Map RAM/etc */
//	n=(TKMM_PAGEEND>>TK_VMEM_PAGESHL);
	n=(TKMM_VAS_START_LO>>TK_VMEM_PAGESHL);
	for(i=256; i<n; i++)
	{
		tva=((u64)i)<<TK_VMEM_PAGESHL;
//		tpte=(2<<8)|(1<<10)|1;
		tpte=(0<<8)|(1<<10)|1;
		tpte|=i<<TK_VMEM_PTESHL;

		if(tva<TKMM_PAGEBASE)
			tpte|=(1<<7);	//No User

		tpte=TK_VMem_EncCheckPageTableEntry(tpte);

		tpte1=TK_VMem_GetPageTableEntry(tva);
		if(tpte!=tpte1)
			__debugbreak();
	}
#endif

#if 1
	tp0=tk_syscall_utxt;
	tva=(u64)tp0;
	tpte=TK_VMem_GetPageTableEntry(tva);
	tpte&=~(1<<7);
	tpte=TK_VMem_EncCheckPageTableEntry(tpte);
	TK_VMem_SetPageTableEntry(tva, tpte);

	tp0=tk_syscall_rv_utxt;
	tva=(u64)tp0;
	tpte=TK_VMem_GetPageTableEntry(tva);
	tpte&=~(1<<7);
	tpte=TK_VMem_EncCheckPageTableEntry(tpte);
	TK_VMem_SetPageTableEntry(tva, tpte);
#endif

#if 0
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


	tk_dbg_printf("TK_VMem_Init: A-3\n");

	tk_vmem_varov_lo=TKMM_VAS_START_LO>>TK_VMEM_PAGESHL;
	tk_vmem_varov_hi=TKMM_VAS_START_HI>>TK_VMEM_PAGESHL;

	tk_vmem_usrexpage=TKMM_PageAllocL(1<<TK_VMEM_PAGESHL);
	tk_vmem_usrexonly=(void *)TKMM_EXOSTART;
	tk_vmem_usrexoffs=0;
	tk_vmem_useldekrr=0;

	tva=(u64)tk_vmem_usrexonly;
	i=((u64)tk_vmem_usrexpage)>>TK_VMEM_PAGESHL;
	TK_VMem_SetPageTableEntry(tva, (i<<TK_VMEM_PTESHL)|1);

#if 1
	tk_dbg_printf("TK_VMem_Init: A-4\n");

	if(!tk_vmem_swap_disable)
	{
		i=sizeof(TK_VMem_PageInfo);
		if(i&(i-1))
		{
			tk_dbg_printf("TK_VMem_Init: sizeof(TK_VMem_PageInfo)"
				" not power of 2\n");
			__debugbreak();
		}
	
	//	np=8192;
//		np=(32768<<10)>>TK_VMEM_PAGESHL;
		np=(tkmm_maxpage<<(TKMM_PAGEBITS-2))>>TK_VMEM_PAGESHL;
//		np=(tkmm_maxpage<<(TKMM_PAGEBITS-1))>>TK_VMEM_PAGESHL;
		tk_vmem_pagecache=TKMM_PageAllocL(np<<TK_VMEM_PAGESHL);
		tk_vmem_npage=np;

		tk_vmem_pagecacheub=tk_vmem_pagecache+(np<<TK_VMEM_PAGESHL);

//		tk_vmem_pagecache=
//			(void *)(((long)tk_vmem_pagecache)|0xC00000000000ULL);
		tk_vmem_pagecache=TK_VMem_PointerAsPhysical(tk_vmem_pagecache);
		tk_vmem_pagecacheub=TK_VMem_PointerAsPhysical(tk_vmem_pagecacheub);

		k=(np+1)*sizeof(TK_VMem_PageInfo);
//		tk_dbg_printf("sizeof(TK_VMem_PageInfo)=%d, np=%d np*sz=%d\n",
//			sizeof(TK_VMem_PageInfo), np, k);

//		__debugbreak();

	//	tk_vmem_pageinf=tk_malloc(np*sizeof(TK_VMem_PageInfo));
		tk_vmem_pageinf=TKMM_PageAllocL(np*sizeof(TK_VMem_PageInfo));
	//	memset(tk_vmem_pageinf, 0, np*sizeof(TK_VMem_PageInfo));

		tk_vmem_pageinfub=tk_vmem_pageinf+np;

//		tk_vmem_pageinf=
//			(void *)(((long)tk_vmem_pageinf)|0xC00000000000ULL);
		tk_vmem_pageinf=TK_VMem_PointerAsPhysical(tk_vmem_pageinf);
		tk_vmem_pageinfub=TK_VMem_PointerAsPhysical(tk_vmem_pageinfub);

//		tk_vmem_pageinf=tk_ptrsetbound1(tk_vmem_pageinf,
//			np*sizeof(TK_VMem_PageInfo));

#if 1
		tk_vmem_lru_free=-1;
//		for(i=0; i<np; i++)
//		for(i=1; i<(np-1); i++)
		for(i=np-1; i>0; i--)
		{
			cpi=tk_vmem_pageinf+i;
			if(cpi>=tk_vmem_pageinfub)
				{ __debugbreak(); }
			cpi->next=tk_vmem_lru_free;
			tk_vmem_lru_free=i;
		}
#endif
	}

#endif

#ifdef __BJX2__
	TK_VMem_MProtectPages(
		&__utext_start,
//		8192,
		&__utext_end-&__utext_start,
		TKMM_PROT_RWX);


	TK_VMem_MProtectPages(
		tk_vmem_pageroot, 1<<TK_VMEM_PAGESHL,
		TKMM_PROT_RW|TKMM_PROT_NOUSER);

	for(i=0; i<(1<<(TK_VMEM_PAGESHL-3)); i++)
	{
		tpte=tk_vmem_pageroot[i];
		if(!(tpte&1))
			continue;
		TK_VMem_MProtectPages(
			(void *)tpte, 1<<TK_VMEM_PAGESHL,
			TKMM_PROT_RW|TKMM_PROT_NOUSER);
	}

#endif

	tk_dbg_printf("TK_VMem_Init: A-5\n");

	/* Enable MMU */

#ifdef __BJX2__

#if 1
//	tpte=(u64)tk_vmem_pageroot;

	if(TK_VMEM_PAGESHL==14)
	{
//		tpte|=0x0042;	//Three-Level, 16K pages
		tmmcr=0x002D;
	}
	else
	if(TK_VMEM_PAGESHL==16)
	{
//		tpte|=0x0021;	//Two-Level, 64K pages
		tmmcr=0x001D;
	}
	else
	{
//		tpte|=0x0003;	//Four-Level, 4K pages
		tmmcr=0x000D;
	}
//	__arch_ttb=tpte;
#endif

#ifdef __ADDR_X48__
	tk_vmem_setsrjq(1);
#endif

//	__arch_ttb=tpte;
//	tk_dbg_printf("");
//	TK_VMem_GetPageTableEntry(0);

	__arch_mmcr=tmmcr;

//	for(i=0; i<256; i++)
//		j++;

	i=tblk1[0]+tblk2[0];

//	__debugbreak();
#endif

//	tk_dbg_puts("TK_VMem_Init: A-6\n");

	tk_dbg_printf("TK_VMem_Init: OK\n");

	return(1);
}

int TK_VMem_MProtectPages2(u64 addr, u64 addrh, size_t len, int prot)
{
	u64 tva, tpte, pte1;
	u64 b, n;
	int i, j;

//	return(0);

	if(!tk_vmem_pageroot)
		return(0);

#if 0
	if(addr<TKMM_VALOSTART)
	{
		/* Skip physical-map range. */
		return(0);
	}
#endif

//	tk_dbg_printf("TK_VMem_MProtectPages: %p..%p sz=%d %X\n",
//		addr, addr+len, len, prot);

	b=(addr>>TK_VMEM_PAGESHL);
//	n=(len>>TK_VMEM_PAGESHL);
	n=(len+((1<<TKMM_PAGEBITS)-1))>>TKMM_PAGEBITS;

	for(i=0; i<n; i++)
	{
		tva=((u64)(b+i))<<TK_VMEM_PAGESHL;

//		tpte=TK_VMem_GetPageTableEntry(tva);
		tpte=TK_VMem_GetPageTableEntry2(tva, addrh);
		TK_VMem_ValidatePageTableEntry(tpte);

		if(!tpte)
		{
			tk_dbg_printf("TK_VMem_MProtectPages: No page at %p\n",
				tva);
			__debugbreak();
		}

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

		tpte=TK_VMem_EncCheckPageTableEntry(tpte);

//		tpte=(2<<8)|(1<<10)|1;
//		tpte=(0<<8)|(1<<10)|1;
//		tpte|=(b+i)<<TK_VMEM_PTESHL;

//		TK_VMem_SetPageTableEntry(tva, tpte);
		TK_VMem_SetPageTableEntry2(tva, addrh, tpte);
		tk_vm_lazyflush=1;

		pte1=TK_VMem_GetPageTableEntry2(tva, addrh);
		if(pte1!=tpte)
			{ __debugbreak(); }
	}

	return(0);
}

int TK_VMem_MProtectPages(u64 addr, size_t len, int prot)
{
	return(TK_VMem_MProtectPages2(addr, 0, len, prot));
}

void TK_VMem_UnlinkPageHash(int i)
{
	TK_VMem_PageInfo *cpi, *lpi, *npi;
	int j, k, hi;

	if(i<0)
		__debugbreak();
	if(i>=tk_vmem_npage)
		__debugbreak();

	cpi=tk_vmem_pageinf+i;
	if(cpi>=tk_vmem_pageinfub)
		{ __debugbreak(); }
	j=cpi->hprev;
	if(j>=0)
	{
		/* Move to Front of Hash */
		k=cpi->hnext;
		lpi=tk_vmem_pageinf+j;
		if(lpi>=tk_vmem_pageinfub)
			{ __debugbreak(); }
		lpi->hnext=k;
		if(k>=0)
		{
			npi=tk_vmem_pageinf+k;
			if(npi>=tk_vmem_pageinfub)
				{ __debugbreak(); }
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
			if(npi>=tk_vmem_pageinfub)
				{ __debugbreak(); }
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

	if(i<0)
		__debugbreak();
	if(i>=tk_vmem_npage)
		__debugbreak();

	cpi=tk_vmem_pageinf+i;
	if(cpi>=tk_vmem_pageinfub)
		{ __debugbreak(); }
	j=cpi->prev;
	if(j>=0)
	{
		if(j>=tk_vmem_npage)
			__debugbreak();

		/* Move to Front of Hash */
		k=cpi->next;
		lpi=tk_vmem_pageinf+j;
		if(lpi>=tk_vmem_pageinfub)
			{ __debugbreak(); }
		lpi->next=k;
		if(k>=0)
		{
			if(k>=tk_vmem_npage)
				__debugbreak();

			npi=tk_vmem_pageinf+k;
			if(npi>=tk_vmem_pageinfub)
				{ __debugbreak(); }
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
			if(k>=tk_vmem_npage)
				__debugbreak();

			npi=tk_vmem_pageinf+k;
			if(npi>=tk_vmem_pageinfub)
				{ __debugbreak(); }
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

	if(i<0)
		__debugbreak();
	if(i>=tk_vmem_npage)
		__debugbreak();

	cpi=tk_vmem_pageinf+i;
	if(cpi>=tk_vmem_pageinfub)
		{ __debugbreak(); }
	hi=cpi->hi;
	k=tk_vmem_pagehash[hi];
	cpi->hnext=k;
	cpi->hprev=-1;
	if(k>=0)
	{
		npi=tk_vmem_pageinf+k;
		if(npi>=tk_vmem_pageinfub)
			{ __debugbreak(); }
		npi->hprev=i;
	}
	tk_vmem_pagehash[hi]=i;
}

void TK_VMem_AddPageFrontLRU(int i)
{
	TK_VMem_PageInfo *cpi, *lpi, *npi;
	int k;

	if(i<0)
		__debugbreak();
	if(i>=tk_vmem_npage)
		__debugbreak();

	cpi=tk_vmem_pageinf+i;
	if(cpi>=tk_vmem_pageinfub)
		{ __debugbreak(); }
	k=tk_vmem_lru_first;
	cpi->next=k;
	if(k>=0)
	{
		if(k>=tk_vmem_npage)
			__debugbreak();
		npi=tk_vmem_pageinf+k;
		if(npi>=tk_vmem_pageinfub)
			{ __debugbreak(); }
		npi->prev=i;
	}else
	{
		tk_vmem_lru_last=i;
	}
	tk_vmem_lru_first=i;
}

void TK_VMem_MovePageFrontLRU(int i)
{
	if(i==tk_vmem_lru_first)
		return;
	TK_VMem_UnlinkPageLRU(i);
	TK_VMem_AddPageFrontLRU(i);
}

void TK_VMem_AddPageFreeLRU(int i)
{
	TK_VMem_PageInfo *cpi, *lpi, *npi;
	int k;

	if(i<0)
		__debugbreak();
	if(i>=tk_vmem_npage)
		__debugbreak();

	cpi=tk_vmem_pageinf+i;
	if(cpi>=tk_vmem_pageinfub)
		{ __debugbreak(); }
	k=tk_vmem_lru_free;
	cpi->next=k;
	if(k>=0)
	{
		if(k>=tk_vmem_npage)
			__debugbreak();
		npi=tk_vmem_pageinf+k;
		if(npi>=tk_vmem_pageinfub)
			{ __debugbreak(); }
		npi->prev=i;
	}else
	{
//		tk_vmem_lru_last=i;
	}
	tk_vmem_lru_free=i;
}

void TK_VMem_SetPageVAddr2(int cidx, s64 vaddr, s64 vaddrh)
{
	TK_VMem_PageInfo *cpi;

	if(cidx<0)
		{ __debugbreak(); }
	if(cidx>=tk_vmem_npage)
		{ __debugbreak(); }

	cpi=tk_vmem_pageinf+cidx;
	if(cpi>=tk_vmem_pageinfub)
		{ __debugbreak(); }
	cpi->vapidx=vaddr>>TKMM_PAGEBITS;
	cpi->vaphi=vaddrh;
}

void TK_VMem_SetPageVAddr(int cidx, s64 vaddr)
{
	TK_VMem_SetPageVAddr2(cidx, vaddr, 0);
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
	if(cpi>=tk_vmem_pageinfub)
		{ __debugbreak(); }
	vaddr=((s64)(cpi->vapidx))<<TKMM_PAGEBITS;
	return(vaddr);
}

s64 TK_VMem_GetPageVAddrHi(int cidx)
{
	TK_VMem_PageInfo *cpi;
	s64 vaddr;

	if(cidx<0)
		{ __debugbreak(); }
	if(cidx>=tk_vmem_npage)
		{ __debugbreak(); }

	cpi=tk_vmem_pageinf+cidx;
	vaddr=cpi->vaphi;
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

int TK_VMem_SetPagePidx(int cidx, int pidx)
{
	TK_VMem_PageInfo *cpi;

	if(cidx<0)
		{ __debugbreak(); }
	if(cidx>=tk_vmem_npage)
		{ __debugbreak(); }

	cpi=tk_vmem_pageinf+cidx;
	cpi->pidx=pidx;
	return(0);
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

	if(cm==7)
	{
		TK_VMem_UnpackFeLz32Page(cpi, ppi);
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

#if 1
	if((j<<9)>=(1<<(TK_VMEM_PAGESHL-1)))
	{
		tk_dbg_printf("TK_VMem_PackCompactedPage: Pack64\n");
		TK_VMem_PackLz64Page(cpi, ppi);
		return;
	}
#endif

#if 0
	if((j<<9)>=(1<<(TK_VMEM_PAGESHL-1)))
	{
		tk_dbg_printf("TK_VMem_PackCompactedPage: FeLz32\n");
		TK_VMem_PackFeLz32Page(cpi, ppi);
		return;
	}
#endif
}

void TK_VMem_WritePageToDisk(int cidx)
{
	TK_VMem_PageInfo *cpi;
	u64 *ppq;
	byte *ppi, *cs, *ct, *tcbuf, *tdbuf;
	u64 nzm, v, sum, ch0, ch1, csum;
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

	if(ppi>=tk_vmem_pagecacheub)
		{ __debugbreak(); }

	tcbuf=tk_vmem_page_tcbuf;
	tdbuf=tk_vmem_page_tdbuf;

#if 1
	if(!(tk_vmem_pagebmp[pidx>>3]&(1<<(pidx&7))))
	{
		/* If page is freed, don't bother with storing to swap. */
		tk_dbg_printf("TK_VMem_WritePageToDisk: Skip Freed Page "
			"pidx=%d\n", pidx);
		tk_vmem_pagevnz[pidx>>3]&=~(1<<(pidx&7));
		tk_vmem_pagecmz[pidx>>3]&=~(1<<(pidx&7));
		cpi->flags&=~1;
		return;
	}
#endif

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

		sum=0; ch0=1; ch1=1;
		for(i=0; i<l; i++)
		{
			v=ppq[i];
			sum+=v;
			ch0+=v;
			ch1+=ch0;
			if(v)
			{
				nzm|=1ULL<<(i>>(TK_VMEM_PAGESHL-6-3));
			}
		}

		csum=ch0^ch1;
		csum=csum+(csum>>32);
		csum=csum+(csum>>16);
		csum=(u16)csum;

		if((csum==cpi->pgchk) && csum)
		{
			cpi->flags&=~1;
			cpi->pgchk=csum;
			return;
		}


//		if(i>=l)
		if(!nzm)
		{
			if(nzm|(nzm>>32))
				{ __debugbreak(); }
			if(sum)
				{ __debugbreak(); }

			/* Page is still Zero. */
//			tk_dbg_printf("TK_VMem_WritePageToDisk: Page Is Zero\n");
			tk_vmem_pagevnz[pidx>>3]&=~(1<<(pidx&7));
			cpi->flags&=~1;
			return;
		}

		tk_vmem_pagevnz[pidx>>3]|=1<<(pidx&7);

//		tk_dbg_printf("TK_VMem_WritePageToDisk: Page Nzm=%08X_%08X\n",
//			(u32)(nzm>>32), (u32)(nzm>>0));

//		k=0;
//		for(i=0; i<64; i++)
//			if(nzm&(1ULL<<i))
//				k++;

		if(0)
//		if(k<=32)
//		if(k<=48)
//		if(k<=56)
		{
			cs=ppi;
			cm=1;

			TK_VMem_PackCompactedPage(tcbuf, ppi);
			j=tcbuf[1]+1;

			if((j<<9)>=(1<<(TK_VMEM_PAGESHL-1)))
				cm=0;

			tk_dbg_printf("TK_VMem_WritePageToDisk: Page Compact, %dK\n",
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

			tk_dbg_printf("TK_VMem_WritePageToDisk: Page Compact, %dK\n",
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
//			TK_VMem_PackFeLz32Page(tcbuf, ppi);
			j=tcbuf[1]+1;

#if 1
			TK_VMem_UnpackLz64Page(tcbuf, tdbuf);
			if(memcmp(ppi, tdbuf, 1<<TK_VMEM_PAGESHL))
			{
				tk_dbg_printf("TK_VMem_WritePageToDisk: Page LZ Corrupt\n");
				cm=0;
				__debugbreak();
			}
#endif

//			if((j<<9)>=(1<<(TK_VMEM_PAGESHL-1)))
//				cm=0;
			if((j<<9)>=(1<<TK_VMEM_PAGESHL))
				cm=0;

			if(cm!=0)
			{
//				tk_dbg_printf("TK_VMem_WritePageToDisk: Page LZ(%d), %dK\n",
//					tcbuf[0], (j+2)>>1);
			}

			cm=0;
//			tk_dbg_printf("TK_VMem_WritePageToDisk: Page Is Store\n");
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
		cpi->pgchk=csum;
	}
}

void TK_VMem_LoadPageFromDisk(int cidx)
{
	TK_VMem_PageInfo *cpi;
	u64 *ppq;
	byte *ppi, *tcb;
	u64 v, ch0, ch1, csum;
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

	if(ppi>=tk_vmem_pagecacheub)
		{ __debugbreak(); }

	if(!(tk_vmem_pagevnz[pidx>>3]&(1<<(pidx&7))))
	{
		/* Zero or Not Initialized Yet. */
//		ppq=(u64 *)ppi;
//		l=1<<(TK_VMEM_PAGESHL-3);
//		for(i=0; i<l; i++)
//			ppq[i]=0;

//		tk_dbg_printf("TK_VMem_LoadPageFromDisk: Page Was Zero %d\n", pidx);

		memset(ppi, 0, 1<<TK_VMEM_PAGESHL);
		cpi->pgchk=0;
		return;
	}

	if(tk_vmem_swap_lba>0)
	{
		lba=tk_vmem_swap_lba+(pidx<<TK_VMEM_PGBLKSHL);
		tcb=tk_vmem_page_tcbuf;

//		if(tk_vmem_pagecmz[cidx>>3]&(1<<(cidx&7)))
		if((tk_vmem_pagecmz[pidx>>3]>>(pidx&7))&1)
		{
//			tk_dbg_printf("TK_VMem_LoadPageFromDisk: Page Was Compressed %d\n", 
//				pidx);

			TKSPI_ReadSectors(tcb, lba, 1);
			l=tcb[1];
			if((l+1)>(1<<TK_VMEM_PGBLKSHL))
				{ __debugbreak(); }
			TKSPI_ReadSectors(tcb+512, lba+1, l);
			TK_VMem_UnpackCompactedPage(tcb, ppi);
		}else
		{
//			tk_dbg_printf("TK_VMem_LoadPageFromDisk: Page Was Raw %d\n", pidx);
			TKSPI_ReadSectors(ppi, lba, 1<<TK_VMEM_PGBLKSHL);
		}

		cpi->pgchk=0;

#if 1
		ppq=(u64 *)ppi;
		l=1<<(TK_VMEM_PAGESHL-3);
		ch0=1; ch1=1;
		for(i=0; i<l; i++)
		{
			v=ppq[i];
			ch0+=v;
			ch1+=ch0;
		}
		csum=ch0^ch1;
		csum=csum+(csum>>32);
		csum=csum+(csum>>16);
		csum=(u16)csum;

		cpi->pgchk=csum;
#endif

		return;
	}

	__debugbreak();
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

			tk_dbg_printf("TK_VMem_MapVirtToCacheIdx: Reuse %d\n", i);

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

		TK_FlushCacheL1D();

//		tk_dbg_printf("TK_VMem_MapVirtToCacheIdx: Evict %d / %d\n",
//			i, cpi->pidx);

		if(cpi->flags&1)
		{
			TK_VMem_WritePageToDisk(i);
		}

		TK_VMem_VaEvictPageIndex(i);
		TK_VMem_UnlinkPageHash(i);
		TK_VMem_UnlinkPageLRU(i);
	}

//	tk_dbg_printf("TK_VMem_MapVirtToCacheIdx: Load Ci=%d Pi=%d\n", i, pidx);

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
	if(TK_VMEM_PAGESHL==14)
		pn&=(1<<30)-1;
	if(TK_VMEM_PAGESHL==16)
		pn&=(1<<26)-1;
	return(pn+cidx);
}

int TK_VMem_MapAddrToCacheIdx(void *addr)
{
	s64 ix;
	ix=((byte *)addr)-tk_vmem_pagecache;
	ix=(ix<<20)>>20;
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
	{
		tk_dbg_printf("TK_VMem_FindFreeSwapPages: n=%d\n", n);
		return(-1);
	}

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

	tk_dbg_printf("TK_VMem_FindFreeSwapPages: Failed to find, n=%d\n", n);
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
		j=tk_vmem_pagebmp[i>>3];
		if(j==0xFF)
		{
			i=(i+8)&(~7);
			continue;
		}
		if(!(j&(1<<(i&7))))
			break;
		i++;

//		while((tk_vmem_pagebmp[i>>3]&(1<<(i&7))) && (i<m))
//			i++;
	}

	if(i<m)
	{
		j=tk_vmem_pagebmp[i>>3];
		if(!(j&(1<<(i&7))))
		{
	//			tk_vmem_pagebmp[i>>3]|=(1<<(i&7));
			tk_vmem_pagebmp[i>>3]=j|(1<<(i&7));
			tk_vmem_pagerov=i+1;
			return(i);
		}
	}

	if(tk_vmem_pagerov!=0)
	{
		tk_vmem_pagerov=0;
		i=TK_VMem_AllocSwapPage();
		return(i);
	}

	tk_dbg_printf("TK_VMem_AllocSwapPage: Failed to alloc\n");
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
		tk_dbg_printf("TK_VMem_AllocSwapPages: Out Of Memory, n=%d\n", n);
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
		tk_vmem_pagecmz[i>>3]&=~(1<<(i&7));
	}

	if(b<tk_vmem_pagerov)
		tk_vmem_pagerov=b;

	return(0);
}


u64 TK_VMem_GetPageTableEntry(s64 vaddr);
int TK_VMem_SetPageTableEntry(s64 vaddr, u64 ptval);

int TK_VMem_VaQueryPages2(s64 vaddr, u64 vaddrh, int cnt)
{
	u64 pte;
	s64 vtaddr;
	int i, j, k, qfl;

	qfl=0;
	for(i=0; i<cnt; i++)
	{
		vtaddr=vaddr+(((s64)i)<<TKMM_PAGEBITS);
//		pte=TK_VMem_GetPageTableEntry(vtaddr);
		pte=TK_VMem_GetPageTableEntry2(vtaddr, vaddrh);
		TK_VMem_ValidatePageTableEntry(pte);
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

int TK_VMem_VaQueryPages(s64 vaddr, int cnt)
{
	return(TK_VMem_VaQueryPages2(vaddr, 0, cnt));
}

int TK_VMem_VaReservePages2(s64 vaddr, s64 vaddrh, int cnt)
{
	u64 pte;
	s64 vtaddr;
	int i, j, k;

	for(i=0; i<cnt; i++)
	{
		vtaddr=vaddr+(((s64)i)<<TKMM_PAGEBITS);
//		pte=TK_VMem_GetPageTableEntry(vtaddr);
		pte=TK_VMem_GetPageTableEntry2(vtaddr, vaddrh);
		TK_VMem_ValidatePageTableEntry(pte);

		if(pte&1)
			continue;

		if(!pte)
		{
//			if(TKMM_PAGEBITS==16)
//				{ pte=(2<<8)|(1<<10); }
//			else
//				{ pte=(0<<8)|(1<<10); }
			pte=(0<<8)|(1<<10);

			pte=TK_VMem_EncCheckPageTableEntry(pte);

//			TK_VMem_SetPageTableEntry(vtaddr, pte);
			TK_VMem_SetPageTableEntry2(vtaddr, vaddrh, pte);
//			tk_vm_lazyflush=1;
		}
	}
}

int TK_VMem_VaReservePages(s64 vaddr, int cnt)
{
	return(TK_VMem_VaReservePages2(vaddr, 0, cnt));
}

int TK_VMem_VaCommitPages2(s64 vaddr, s64 vaddrh, int cnt)
{
	u64 pte, pte1;
	s64 vtaddr;
	int ptpn, pidx, ppn;
	int i, j, k;

	for(i=0; i<cnt; i++)
	{
		vtaddr=vaddr+(((s64)i)<<TKMM_PAGEBITS);
//		pte=TK_VMem_GetPageTableEntry(vtaddr);
		pte=TK_VMem_GetPageTableEntry2(vtaddr, vaddrh);
		TK_VMem_ValidatePageTableEntry(pte);

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

			pte=TK_VMem_EncCheckPageTableEntry(pte);

//			TK_VMem_SetPageTableEntry(vtaddr, pte);
			TK_VMem_SetPageTableEntry2(vtaddr, vaddrh, pte);
//			tk_vm_lazyflush=1;
			TK_VMem_VaFlushVaddr2(vtaddr, vaddrh);
#endif

//			pte1=TK_VMem_GetPageTableEntry(vtaddr);
			pte1=TK_VMem_GetPageTableEntry2(vtaddr, vaddrh);
			if(pte1!=pte)
				{ __debugbreak(); }
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

int TK_VMem_VaCommitPages(s64 vaddr, int cnt)
{
	return(TK_VMem_VaCommitPages2(vaddr, 0, cnt));
}

int TK_VMem_VaFreePages2(s64 vaddr, s64 vaddrh, int cnt)
{
	u64 pte;
	s64 vtaddr, paddr;
	int ptpn, pidx;
	int i, j, k;

	for(i=0; i<cnt; i++)
	{
		vtaddr=vaddr+(((s64)i)<<TKMM_PAGEBITS);
//		pte=TK_VMem_GetPageTableEntry(vtaddr);
		pte=TK_VMem_GetPageTableEntry2(vtaddr, vaddrh);
		TK_VMem_ValidatePageTableEntry(pte);

		if(!pte)
			continue;

		if(pte&1)
		{
//			paddr=pte&0x0000FFFFFFFFF000ULL;
			paddr=((pte>>TK_VMEM_PTESHL)&0xFFFFFFFFFULL)<<TKMM_PAGEBITS;
			pidx=TK_VMem_MapAddrToCacheIdx((void *)paddr);
			if(pidx>=0)
			{
//				TK_VMem_VaEvictPageIndex(pidx);
				TK_VMem_VaDiscardPageIndex(pidx);

//				pte=TK_VMem_GetPageTableEntry(vtaddr);
				pte=TK_VMem_GetPageTableEntry2(vtaddr, vaddrh);
				TK_VMem_ValidatePageTableEntry(pte);
				if(!pte)
					continue;

				if(pte&1)
					__debugbreak();
			}else
			{
				/* Physical Remap */
				k=TKMM_PointerToPage((void *)paddr);
				TKMM_FreePages(k, 1);
//				TK_VMem_SetPageTableEntry(vtaddr, 0);
				TK_VMem_SetPageTableEntry2(vtaddr, vaddrh, 0);
//				TK_VMem_VaFlushVaddr(vtaddr);
//				TK_VMem_VaFlushVaddr2(vtaddr, vaddrh);
				tk_vm_lazyflush=1;
				continue;
			}
		}

		ptpn=(pte>>TK_VMEM_PTESHL)&TK_VMEM_PTEMASK;
//		if(ptpn)
		if(ptpn && (pte&(1<<10)))
		{
			TK_VMem_FreeSwapPages(ptpn, 1);
//			TK_VMem_SetPageTableEntry(vtaddr, 0);
			TK_VMem_SetPageTableEntry2(vtaddr, vaddrh, 0);
			tk_vm_lazyflush=1;
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

			pte=TK_VMem_EncCheckPageTableEntry(pte);

//			TK_VMem_SetPageTableEntry(vtaddr, pte);
			TK_VMem_SetPageTableEntry2(vtaddr, vaddrh, pte);
		}
#endif
	}
}

int TK_VMem_VaFreePages(s64 vaddr, int cnt)
{
	return(TK_VMem_VaFreePages2(vaddr, 0, cnt));
}

int TK_VMem_VaDoRemapPages2(s64 vaddr, s64 vaddrh, s64 phaddr, int cnt)
{
	u64 pte, pte1;
	s64 vtaddr;
	int phb;
	int i, j, k;

	for(i=0; i<cnt; i++)
	{
		vtaddr=vaddr+(((s64)i)<<TKMM_PAGEBITS);
//		pte=TK_VMem_GetPageTableEntry(vtaddr);
		pte=TK_VMem_GetPageTableEntry2(vtaddr, vaddrh);
		TK_VMem_ValidatePageTableEntry(pte);

		if(!pte)
		{
			pte=(((phaddr>>TKMM_PAGEBITS)+i)<<TK_VMEM_PTESHL)|
				(0<<8)|(1<<10)|1;

			pte=TK_VMem_EncCheckPageTableEntry(pte);

//			pte=(0<<8)|(1<<10);
//			TK_VMem_SetPageTableEntry(vtaddr, pte);
			TK_VMem_SetPageTableEntry2(vtaddr, vaddrh, pte);
//			TK_VMem_VaFlushVaddr(vtaddr);
			TK_VMem_VaFlushVaddr2(vtaddr, vaddrh);
//			tk_vm_lazyflush=1;

//			pte1=TK_VMem_GetPageTableEntry(vtaddr);
			pte1=TK_VMem_GetPageTableEntry2(vtaddr, vaddrh);
			if(pte1!=pte)
				{ __debugbreak(); }
		}
	}
}

int TK_VMem_VaDoRemapPages(s64 vaddr, s64 phaddr, int cnt)
{
	return(TK_VMem_VaDoRemapPages2(vaddr, 0, phaddr, cnt));
}

int TK_VMem_VaDoAllocRemapedPages2(s64 vaddr, s64 vaddrh, int cnt)
{
	void *ptr;
	u64 pte, pte1;
	s64 vtaddr;
	int phb, pg;
	int i, j, k;

	if(vaddr&((1<<TKMM_PAGEBITS)-1))
		{ __debugbreak(); }

	vtaddr=vaddr;
	for(i=0; i<cnt; i++)
	{
//		vtaddr=vaddr+(((s64)i)<<TKMM_PAGEBITS);
//		pte=TK_VMem_GetPageTableEntry(vtaddr);
		pte=TK_VMem_GetPageTableEntry2(vtaddr, vaddrh);
		TK_VMem_ValidatePageTableEntry(pte);

		if(!pte)
		{
			pte=(0<<8)|(1<<10)|0;

#if 0
//			pg=TKMM_AllocPages(1);
			pg=TKMM_AllocPagesApn(1);
//			pg=TKMM_AllocPage();
//			pg+=(TKMM_PAGEBASE>>TKMM_PAGEBITS);

//			ptr=TKMM_PageAlloc(1<<TKMM_PAGEBITS);
//			pg=((u64)ptr)>>TKMM_PAGEBITS;

			pte=(pg<<TK_VMEM_PTESHL)|(0<<8)|(1<<10)|1;

//			tk_dbg_printf("    %p %08X\n", (void *)vtaddr, pg<<14);

//			pte=(((phaddr>>TKMM_PAGEBITS)+i)<<TK_VMEM_PTESHL)|
//				(0<<8)|(1<<10)|1;

#endif

			pte=TK_VMem_EncCheckPageTableEntry(pte);

//			pte=(0<<8)|(1<<10);
//			TK_VMem_SetPageTableEntry(vtaddr, pte);
			TK_VMem_SetPageTableEntry2(vtaddr, vaddrh, pte);
//			TK_VMem_VaFlushVaddr(vtaddr);
			TK_VMem_VaFlushVaddr2(vtaddr, vaddrh);
//			tk_vm_lazyflush=1;

//			pte1=TK_VMem_GetPageTableEntry(vtaddr);
			pte1=TK_VMem_GetPageTableEntry2(vtaddr, vaddrh);
			if(pte1!=pte)
				{ __debugbreak(); }
		}

#if 0
		if(!(pte&1))
		{
			__debugbreak();
		}
#endif

		vtaddr+=1<<TKMM_PAGEBITS;
	}

#if 0
	pte=TK_VMem_GetPageTableEntry(vtaddr);
	if(!pte)
	{
		pg=0x10000>>TKMM_PAGEBITS;
		pte=(pg<<TK_VMEM_PTESHL)|(0<<8)|(1<<10)|1;
		TK_VMem_SetPageTableEntry(vtaddr, pte);
//		TK_VMem_VaFlushVaddr(vtaddr);
		TK_VMem_VaFlushVaddr2(vtaddr, vaddrh);
	}
#endif
}

int TK_VMem_VaDoAllocRemapedPages(s64 vaddr, int cnt)
{
	return(TK_VMem_VaDoAllocRemapedPages2(vaddr, 0, cnt));
}

void TK_VMem_CacheKnock()
{
	u64 *dummy;
	int i, j;

	dummy=(u64 *)tk_vmem_page_tcbuf;
	if(!dummy)
		dummy=(u64 *)(&TK_VMem_CacheKnock);
	j=0;
	for(i=0; i<8192; i++)
		j+=dummy[i];
}

void TK_VMem_VaFlushVaddr(s64 vaddr)
{
	u64 pte;

	/* Attempt to flush page from TLB */
	pte=(0<<8)|(1<<10);

	tk_vmem_loadpte_faketlb(vaddr, pte);
	tk_vmem_loadpte8x(vaddr, pte);
//	TK_VMem_CacheKnock();

#if 0
	tk_vmem_loadpte(vaddr, pte);
	tk_vmem_loadpte(vaddr, pte);
	tk_vmem_loadpte(vaddr, pte);
	tk_vmem_loadpte(vaddr, pte);

	tk_vmem_loadpte(vaddr, pte);
	tk_vmem_loadpte(vaddr, pte);
	tk_vmem_loadpte(vaddr, pte);
	tk_vmem_loadpte(vaddr, pte);
#endif
}

void TK_VMem_VaFlushVaddr2I(s64 vaddr, s64 vaddrh)
{
	TK_VMem_VaFlushVaddr(vaddr);
}

void TK_VMem_VaFlushVaddr2(s64 vaddr, s64 vaddrh)
{
//	TK_VMem_VaFlushVaddr(vaddr);

#if 1
	u64 ava;

	if(tk_vmem_tlbflushinhibit)
		return;

	ava=TK_VMem_QueryTlbVaForVirtPage(vaddr);
	if(ava)
	{
		TK_VMem_VaFlushVaddr(vaddr);
	}
#endif
}

void TK_VMem_VaEvictPageIndex(int cidx)
{
	s64 vaddr, vaddrh;
	s64 ava;
	u64 pte;
	int ptpn, needknock;

	vaddr=TK_VMem_GetPageVAddr(cidx);
	vaddrh=TK_VMem_GetPageVAddrHi(cidx);
	ptpn=TK_VMem_GetPagePidx(cidx);

//	tk_dbg_printf("TK_VMem_VaEvictPageIndex: cidx=%d pidx=%d\n", cidx, ptpn);

//	if(TKMM_PAGEBITS==16)
//		{ pte=(2<<8)|(1<<10); }
//	else
//		{ pte=(0<<8)|(1<<10); }
	pte=(0<<8)|(1<<10);
	pte|=((u64)ptpn)<<TK_VMEM_PTESHL;

	pte=TK_VMem_EncCheckPageTableEntry(pte);

//	TK_VMem_SetPageTableEntry(vaddr, pte);
	TK_VMem_SetPageTableEntry2(vaddr, vaddrh, pte);

	/* Attempt to flush page from TLB */
//	tk_vmem_loadpte(vaddr, pte);
//	tk_vmem_loadpte(vaddr, pte);
//	tk_vmem_loadpte(vaddr, pte);
//	tk_vmem_loadpte(vaddr, pte);

	needknock=0;
	ava=TK_VMem_QueryTlbVaForVirtPage(vaddr);
	if(ava)
	{
		TK_VMem_VaFlushVaddr2(vaddr, vaddrh);
		needknock=1;
	}

	ava=TK_VMem_QueryTlbVaForPhysPage(pte);
	if(ava)
	{
//		TK_VMem_VaFlushVaddr(vaddr);
//		TK_VMem_VaFlushVaddr2(vaddr, vaddrh);
		TK_VMem_VaFlushVaddr2(ava, vaddrh);
//		TK_VMem_CacheKnock();
		needknock=1;
	}

	if(tk_vmem_tlbflushinhibit)
		needknock=0;

	if(needknock)
	{
		TK_VMem_CacheKnock();
//		tk_vmem_l1flush();
	}
}

void TK_VMem_VaDiscardPageIndex(int cidx)
{
	s64 vaddr, vaddrh;
	s64 ava;
	u64 pte;
	int ptpn, needknock;

	vaddr=TK_VMem_GetPageVAddr(cidx);
	vaddrh=TK_VMem_GetPageVAddrHi(cidx);
	ptpn=TK_VMem_GetPagePidx(cidx);

//	tk_dbg_printf("TK_VMem_VaDiscardPageIndex: cidx=%d pidx=%d A=%p\n",
//		cidx, ptpn, vaddr);

//	pte=(0<<8)|(1<<10);
//	pte|=((u64)ptpn)<<TK_VMEM_PTESHL;
	pte=0;
	TK_VMem_SetPageTableEntry2(vaddr, vaddrh, pte);

	TK_VMem_FreeSwapPages(ptpn, 1);

	TK_VMem_SetPageVAddr(cidx, 0);
	TK_VMem_SetPagePidx(cidx, 0);
	TK_VMem_UnlinkPageHash(cidx);
	TK_VMem_UnlinkPageLRU(cidx);
	TK_VMem_AddPageFreeLRU(cidx);

//	TK_VMem_VaFlushVaddr2(vaddr, vaddrh);
	tk_vm_lazyflush=1;
}

void TK_VMem_VaPageInAddr2(s64 vaddr, s64 vaddrh)
{
	s64 vaddr1;
	u64 pte;
	int ptpn;
	int cidx;
	int ppn;

	vaddr1=vaddr&(~((1<<TKMM_PAGEBITS)-1));

//	pte=TK_VMem_GetPageTableEntry(vaddr1);
	pte=TK_VMem_GetPageTableEntry2(vaddr1, vaddrh);
	TK_VMem_ValidatePageTableEntry(pte);

	if(pte&1)
	{
//		tk_vmem_loadpte(vaddr, pte);
		tk_vmem_loadpte2(vaddr, vaddrh, pte);
		return;
	}

	ptpn=(pte>>TK_VMEM_PTESHL)&TK_VMEM_PTEMASK;
	if(!ptpn)
	{
		if(!(pte&(1<<10)))
			__debugbreak();

#if 1
		tk_dbg_printf("TK_VMem_VaPageInAddr: Debug Pad, %p\n", vaddr1);
		ptpn=TK_VMem_AllocSwapPages(1);
		pte|=((u64)ptpn)<<TK_VMEM_PTESHL;
		pte&=~1;

		pte=TK_VMem_EncCheckPageTableEntry(pte);

//		TK_VMem_SetPageTableEntry(vaddr, pte);
		TK_VMem_SetPageTableEntry2(vaddr, vaddrh, pte);
		TK_VMem_VaFlushVaddr2(vaddr, vaddrh);
//		tk_vm_lazyflush=1;
		return;
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

//	TK_FlushCacheL1D();

//	pte&=~((16777215ULL)<<12);
	pte&=~(TK_VMEM_PTEMASK<<TK_VMEM_PTESHL);
	cidx=TK_VMem_MapVirtToCacheIdx(ptpn);
//	TK_VMem_SetPageVAddr(cidx, vaddr1);
	TK_VMem_SetPageVAddr2(cidx, vaddr1, vaddrh);
	ppn=TK_VMem_MapPhysIdxToPageNum(cidx);
	pte|=((u64)ppn)<<TK_VMEM_PTESHL;
	pte|=1;

	pte=TK_VMem_EncCheckPageTableEntry(pte);

//	TK_VMem_SetPageTableEntry(vaddr1, pte);
	TK_VMem_SetPageTableEntry2(vaddr1, vaddrh, pte);

//	tk_dbg_printf("TK_VMem_VaPageInAddr: %p %p\n", vaddr, pte);

//	TK_VMem_VaFlushVaddr(vaddr1);
	TK_VMem_VaFlushVaddr2(vaddr1, vaddrh);

//	TK_VMem_CacheKnock();

//	tk_vmem_loadpte(vaddr1, pte);
	tk_vmem_loadpte2(vaddr1, vaddrh, pte);
}

void TK_VMem_VaPageInAddr(s64 vaddr)
{
	TK_VMem_VaPageInAddr2(vaddr, 0);
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
		TK_VMem_ValidatePageTableEntry(pte);

		if(pte)
		{
			vpn++;
			continue;
		}

		for(i=0; i<cnt; i++)
		{
			vta=((s64)(vpn+i))<<TKMM_PAGEBITS;
			pte=TK_VMem_GetPageTableEntry(vta);
			TK_VMem_ValidatePageTableEntry(pte);

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

s64 TK_VMem_VaFindFreePagesAslr2(int cnt, int flag, s64 vaddrh)
{
	s64 addr, addr1;
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
//		addr=(addr<<TKMM_PAGEBITS)&0x00003FFFFFFFFFFFLL;
		addr=(addr<<TKMM_PAGEBITS)&0x00001FFFFFFFFFFFLL;

		if(cnt<=65536)
		{
			addr1=addr+(cnt<<TKMM_PAGEBITS);
			if((addr>>32)!=(addr1>>32))
				continue;
		}

//		if(TK_VMem_CheckAddrIsVirtual(addr))
		if(TK_VMem_CheckAddrIsVirtual2(addr, vaddrh))
		{
//			qfl=TK_VMem_VaQueryPages(addr, cnt+1);
			qfl=TK_VMem_VaQueryPages2(addr, vaddrh, cnt+1);
			if(qfl==1)
			{
				return(addr);
			}
		}
	}
	return(0);
}

s64 TK_VMem_VaFindFreePagesAslr(int cnt, int flag)
{
	return(TK_VMem_VaFindFreePagesAslr2(cnt, flag, 0));
}

s64 TK_VMem_VaFindFreePages(int cnt, int flag)
{
//	return(TK_VMem_VaFindFreePagesBasic(cnt, flag));
	return(TK_VMem_VaFindFreePagesAslr(cnt, flag));
}

s64 TK_VMem_VaFindFreePages2(int cnt, int flag, s64 vaddrh)
{
//	return(TK_VMem_VaFindFreePagesBasic(cnt, flag));
	return(TK_VMem_VaFindFreePagesAslr2(cnt, flag, vaddrh));
}

s64 TK_VMem_VaFindFreePagesLowBasic(int cnt, int flag)
{
	u64 pte;
	s64 vta, vta0;
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
		TK_VMem_ValidatePageTableEntry(pte);

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

		vta0=((s64)vpn)<<TKMM_PAGEBITS;
		for(i=0; i<cnt; i++)
		{
			vta=((s64)(vpn+i))<<TKMM_PAGEBITS;
			pte=TK_VMem_GetPageTableEntry(vta);
			TK_VMem_ValidatePageTableEntry(pte);

			if(pte)
				break;
				
			if(((vta>>24)!=(vta0>>24)) && (cnt<256))
				break;
		}

		if(i>=cnt)
			break;
		vpn+=i;
	}

	vta=((s64)vpn)<<TKMM_PAGEBITS;
	return(vta);
}

s64 TK_VMem_VaFindFreePagesLowAslr2(int cnt, int flag, s64 vaddrh)
{
	s64 addr;
	u32 vps, vpe, vpn, vpne;
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

		vpne=vpn+cnt;

//		if((vpn<vps) || ((vpn+cnt)>=vpe))
		if((vpn<vps) || (vpne>=vpe))
			continue;

		if(cnt<(1<<(22-TKMM_PAGEBITS)))
		{
			if((vpn>>(24-TKMM_PAGEBITS))!=(vpne>>(24-TKMM_PAGEBITS)))
				continue;
		}else
			if(cnt<(1<<(30-TKMM_PAGEBITS)))
		{
			if((vpn>>(32-TKMM_PAGEBITS))!=(vpne>>(32-TKMM_PAGEBITS)))
				continue;
		}

		addr=((s64)vpn)<<TKMM_PAGEBITS;

//		qfl=TK_VMem_VaQueryPages(addr, cnt+1);
		qfl=TK_VMem_VaQueryPages2(addr, vaddrh, cnt+1);
		if(qfl==1)
		{
			return(addr);
		}

//		return(addr);
	}
	return(0);
}

s64 TK_VMem_VaFindFreePagesLowAslr(int cnt, int flag)
{
	return(TK_VMem_VaFindFreePagesLowAslr2(cnt, flag, 0));
}

s64 TK_VMem_VaFindFreePagesLow(int cnt, int flag)
{
//	return(TK_VMem_VaFindFreePagesLowBasic(cnt, flag));
	return(TK_VMem_VaFindFreePagesLowAslr(cnt, flag));
}

s64 TK_VMem_VaVirtualAlloc2(s64 addr, s64 addrh, s64 size,
	int flProt, int flMap)
{
	int vpn, vpb;
	int qfl;

//	__debugbreak();
//	return(0);

//	tk_dbg_printf("TK_VMem_VaVirtualAlloc2: "
//		"addr=%lXX size=%d flProt=%X flMap=%X\n", addr, size, flProt, flMap);

#if 1
	vpb=addr>>TKMM_PAGEBITS;
//	vpn=size>>TKMM_PAGEBITS;
	vpn=(size+((1<<TKMM_PAGEBITS)-1))>>TKMM_PAGEBITS;
//	vpn=((size+32)+((1<<TKMM_PAGEBITS)-1))>>TKMM_PAGEBITS;
//	vpn=(size+(1<<TKMM_PAGEBITS))>>TKMM_PAGEBITS;

//	tk_dbg_printf("TK_VMem_VaVirtualAlloc: size=%d vpn=%d\n", size, vpn);

	if(addr)
	{
//		qfl=TK_VMem_VaQueryPages(addr, vpn);
		qfl=TK_VMem_VaQueryPages2(addr, addrh, vpn);

		if(qfl==1)
		{
//			if(TK_VMem_CheckAddrIsVirtual(addr))
			if(TK_VMem_CheckAddrIsVirtual2(addr, addrh))
			{
//				TK_VMem_VaReservePages(addr, vpn);
//				TK_VMem_VaCommitPages(addr, vpn);
				TK_VMem_VaCommitPages2(addr, addrh, vpn);
				return(addr);
			}else
			{
//				TK_VMem_VaDoAllocRemapedPages(addr, vpn);
				TK_VMem_VaDoAllocRemapedPages2(addr, addrh, vpn);
				return(addr);
			}
		}
	}

	if(flMap&TKMM_MAP_PHYSICAL)
	{
		addr=(s64)(TKMM_PageAlloc(size));
		return(addr);
	}

//	if(1)
//	if(0)
	if(tk_vmem_swap_disable || (flMap&TKMM_MAP_32BIT))
	{
//		tk_dbg_printf("TK_VMem_VaVirtualAlloc: A0\n");
		addr=TK_VMem_VaFindFreePagesLow(vpn, flMap);
//		tk_dbg_printf("TK_VMem_VaVirtualAlloc: A1\n");
		if(!addr)
			return(0);
//		tk_dbg_printf("TK_VMem_VaVirtualAlloc: A2 %p\n", (void *)addr);
//		TK_VMem_VaDoAllocRemapedPages(addr, vpn);
		TK_VMem_VaDoAllocRemapedPages2(addr, addrh, vpn);
		tk_vmem_varov_lo=(addr>>TKMM_PAGEBITS)+vpn;
//		tk_dbg_printf("TK_VMem_VaVirtualAlloc: A3\n");
		return(addr);
	}


//	addr=TK_VMem_VaFindFreePages(vpn, flMap);
	addr=TK_VMem_VaFindFreePages2(vpn, flMap, addrh);
	if(!addr)
		return(0);

//	tk_dbg_printf("TK_VMem_VaVirtualAlloc: VA=%p..%p Vpn=%d\n",
//		addr, addr+(vpn<<TKMM_PAGEBITS), vpn);

	if(flMap&TKMM_MAP_DIRECT)
	{
//		TK_VMem_VaDoAllocRemapedPages(addr, vpn);
		TK_VMem_VaDoAllocRemapedPages2(addr, addrh, vpn);
	}else
	{
//		TK_VMem_VaReservePages(addr, vpn);
//		TK_VMem_VaCommitPages(addr, vpn);
		TK_VMem_VaCommitPages2(addr, addrh, vpn);
	}
	tk_vmem_varov_hi=(addr>>TKMM_PAGEBITS)+vpn;
	return(addr);
#endif
}

s64 TK_VMem_VaVirtualAlloc(s64 addr, s64 size, int flProt, int flMap)
{
	return(TK_VMem_VaVirtualAlloc2(addr, 0, size, flProt, flMap));
}

int TK_VMem_VaVirtualFree(s64 vaddr, s64 size)
{
	return(TK_VMem_VaFreePages(vaddr, size>>TKMM_PAGEBITS));
}

int TK_VMem_VaVirtualFree2(s64 vaddr, s64 vaddrh, s64 size)
{
	return(TK_VMem_VaFreePages2(vaddr, vaddrh, size>>TKMM_PAGEBITS));
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

int TK_VMem_CheckAddrIsVirtual2(s64 addr, s64 addrh)
{
	u64 addr1;

	if((addrh&0x0000FFFFFFFFFFFFULL)!=0)
	{
		if((addr>>47)&1)
			return(0);
		return(1);
	}

	addr1=addr&0x0000FFFFFFFFFFFFULL;

//	if(((addr>>32)&0xFFFF)!=0x0000)
	if((addr1>=TKMM_VAS_START_HI) && (addr1<TKMM_VAS_END_HI))
		return(1);
	return(0);
}

int TK_VMem_CheckAddrIsLowVirtual(s64 addr)
{
	u64 addr1;

	addr1=addr&0x0000FFFFFFFFFFFFULL;

//	if(((addr>>32)&0xFFFF)!=0x0000)
	if((addr1>=TKMM_VAS_START_LO) && (addr1<TKMM_VAS_END_LO))
		return(1);
	return(0);
}

int TK_VMem_CheckAddrIsPhysPage(s64 addr)
{
	u64 addr1;

	addr1=addr&0x0000FFFFFFFFFFFFULL;

	if(((addr1>>44)&0xE)==0xC)
		return(1);

//	if(((addr>>32)&0xFFFF)!=0x0000)
	if((addr1>=TKMM_PAGEBASE) && (addr1<TKMM_PAGEEND))
		return(1);
	return(0);
}

int TK_VMem_CheckPtrIsVirtual(void *ptr)
{
	return(TK_VMem_CheckAddrIsVirtual((s64)ptr));
}

void tk_vmem_tlbmiss(u64 ttb, u64 tea, u64 teah)
{
	TKPE_TaskInfo *task;
	u64 pte, paddr;
	int cidx, pidx, pid;
	int i, j, k;

//	tk_dbg_printf("tk_vmem_tlbmiss: %p\n", tea);

	task=(TKPE_TaskInfo *)TK_GET_TBR;
	if(task)
	{
		pid=task->pid;
	}else
	{
		pid=-1;
	}

#if 0
	if(tk_vm_lazyflush)
	{
		tk_vmem_flush_faketlb();
		tk_vmem_tlbflush();
//		tk_vmem_l1flush();
		TK_VMem_CacheKnock();
		tk_vm_lazyflush=0;
	}
#endif

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

	cidx=tk_vmem_tlbmisshrov;
	tk_vmem_tlbmisshrov=(cidx+1)&255;
	tk_vmem_tlbmisshist[cidx]=tea;

	if(tk_vmem_tlbflushinhibit>0)
		tk_vmem_tlbflushinhibit--;

	if(((tea>>47)&1) && !teah)
	{
		if(((tea>>44)&0xE)>=0xC)
		{
			/* Ideally shouldn't happen, but who knows?... */
			__debugbreak();
			tea&=0x00000FFFFFFFFFFFULL;
			pte=((tea>>TKMM_PAGEBITS)<<TK_VMEM_PTESHL)|1;
			tk_vmem_loadpte2(tea, teah, pte);
			return;
		}
	}

//	__debugbreak();

//	pte=TK_VMem_GetPageTableEntry(tea);
	pte=TK_VMem_GetPageTableEntry2(tea, teah);
	TK_VMem_ValidatePageTableEntry(pte);

	if(pte&1)
	{
		cidx=-1;
		pidx=-1;

//		if(TK_VMem_CheckAddrIsVirtual(tea))
		if(TK_VMem_CheckAddrIsVirtual2(tea, teah))
		{
			paddr=((pte>>TK_VMEM_PTESHL)&0xFFFFFFFFFULL)<<TKMM_PAGEBITS;
			cidx=TK_VMem_MapAddrToCacheIdx((void *)paddr);
			if(cidx>=0)
			{
				TK_VMem_MovePageFrontLRU(cidx);
				pidx=TK_VMem_GetPagePidx(cidx);
			}
		}else
		{
//			if((tea>=0x100000) && (tea<TKMM_VALOSTART))
			if((tea>=TKMM_RAMBASE) && (tea<TKMM_VALOSTART))
			{
				if((pte>>TKMM_PAGEBITS)!=(tea>>TKMM_PAGEBITS))
					{ __debugbreak(); }
			}else if((tea>=TKMM_VALOSTART) && (tea<TKMM_VALOEND))
			{
				if((pte>=TKMM_RAMBASE) && (pte<TKMM_PAGEBASE))
					{ __debugbreak(); }

//				if(pte>TKMM_PAGEEND)
				if(pte>tkmm_pageend)
					{ __debugbreak(); }
				if(pte<TKMM_RAMBASE)
//				if((pte>>TK_VMEM_PTESHL) && (pte<TKMM_RAMBASE))
					{ __debugbreak(); }
			}else if(tea<0x10000)
			{
				/* NULL, Special. */
//				pte=(0x10000>>TKMM_PAGEBITS)<<TK_VMEM_PTESHL;
//				pte|=1;
			}else
			{
				tk_dbg_printf("tk_vmem_tlbmiss: Unhandled VA Range %08X\n",
					tea);
				__debugbreak();
			}
		}

//		tk_dbg_printf("tk_vmem_tlbmiss: page-hit %p->%08X "
//			"(cidx=%04X pidx=%04X, pid=%d)\n",
//			tea, pte, cidx, pidx, pid);

//		__debugbreak();
//		tk_vmem_loadpte(tea, pte);
		tk_vmem_loadpte2(tea, teah, pte);
//		__debugbreak();
		return;
	}

//	tk_dbg_printf("tk_vmem_tlbmiss: page-miss %p pid=%d\n", tea, pid);

#if 0
	if((tk_vmem_tlbmisshist[(cidx-1)&255]>>TKMM_PAGEBITS)==
		(tea>>TKMM_PAGEBITS))
			{ tk_vmem_tlbflushinhibit=8; }
	if((tk_vmem_tlbmisshist[(cidx-2)&255]>>TKMM_PAGEBITS)==
		(tea>>TKMM_PAGEBITS))
			{ tk_vmem_tlbflushinhibit=8; }
	if((tk_vmem_tlbmisshist[(cidx-3)&255]>>TKMM_PAGEBITS)==
		(tea>>TKMM_PAGEBITS))
			{ tk_vmem_tlbflushinhibit=8; }
	if((tk_vmem_tlbmisshist[(cidx-4)&255]>>TKMM_PAGEBITS)==
		(tea>>TKMM_PAGEBITS))
			{ tk_vmem_tlbflushinhibit=8; }
#endif

	for(i=1; i<12; i++)
	{
		if((tk_vmem_tlbmisshist[(cidx-i)&255]>>TKMM_PAGEBITS)==
			(tea>>TKMM_PAGEBITS))
				{ tk_vmem_tlbflushinhibit=16; }
	}

#if 0
	if(!pte)
	{
		tea&=0x0000FFFFFFFFFFFFULL;
		pte=((tea>>TKMM_PAGEBITS)<<TK_VMEM_PTESHL)|1;

//		__debugbreak();
//		pte=((0x10000>>TKMM_PAGEBITS)<<TK_VMEM_PTESHL)|1;
//		tk_vmem_loadpte(tea, pte);
		tk_vmem_loadpte2(tea, teah, pte);
		return;
	}
#endif

//	if(TK_VMem_CheckAddrIsVirtual(tea))
	if(TK_VMem_CheckAddrIsVirtual2(tea, teah))
	{
//		__debugbreak();
//		TK_VMem_VaPageInAddr(tea);
		TK_VMem_VaPageInAddr2(tea, teah);
	}else
	{
		if(tea<TKMM_VALOSTART)
		{
			/* Physical Mapped Range */

			if(pte)
				{ __debugbreak(); }

			tea&=0x0000FFFFFFFFFFFFULL;
			pte=((tea>>TKMM_PAGEBITS)<<TK_VMEM_PTESHL)|1;

			pte=TK_VMem_EncCheckPageTableEntry(pte);

//			TK_VMem_SetPageTableEntry(tea, pte);
			TK_VMem_SetPageTableEntry2(tea, teah, pte);

//			tk_vmem_loadpte(tea, pte);
			tk_vmem_loadpte2(tea, teah, pte);
			return;
		}else
		{
			if(pte&(1<<10))
			{
				k=TKMM_AllocPagesZeroedApn(1);
				pte=(((u64)k)<<TK_VMEM_PTESHL)|(1<<10)|1;

				pte=TK_VMem_EncCheckPageTableEntry(pte);

	//			TK_VMem_SetPageTableEntry(tea, pte);
				TK_VMem_SetPageTableEntry2(tea, teah, pte);

	//			tk_vmem_loadpte(tea, pte);
				tk_vmem_loadpte2(tea, teah, pte);
				return;
			}

			__debugbreak();

			/* Direct Mapping Range */

			tea=0x10000ULL;
			pte=((tea>>TKMM_PAGEBITS)<<TK_VMEM_PTESHL)|1;
//			tk_vmem_loadpte(tea, pte);
			tk_vmem_loadpte2(tea, teah, pte);
			return;
		}
	}
}

#ifdef __BJX2__
void *tk_vmem_virttophys(u64 vaddr)
{
	u64 pte, vaddr1;

	u64 mask_lo, mask_hi;
	
	pte=__arch_ttb;
	if(!(pte&1))
		return((void *)vaddr);
	
	mask_lo=(1<<TKMM_PAGEBITS)-1;
	mask_hi=~mask_lo;
	
	pte=TK_VMem_GetPageTableEntry2(vaddr, 0);
	TK_VMem_ValidatePageTableEntry(pte);
	
	vaddr1=(pte&mask_hi)|(vaddr&mask_lo);
	return((void *)vaddr1);
}
#else
void *tk_vmem_virttophys(u64 vaddr)
{
	return((void *)vaddr);
}
#endif

void tk_vmem_aclmiss(u64 ttb, u64 tea, u64 teah)
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

u64 __sfp_fdiv_f64(u64 f0, u64 f1);
u64 __sfp_pdiv_f32(u64 f0, u64 f1);

void tk_vmem_emurq_rv(u64 spc, u64 exc, u64 *regs, u32 *opp)
{
	static byte rvremap[32]={
		TKPE_REGSAVE_R0 , TKPE_REGSAVE_LR , TKPE_REGSAVE_SSP, TKPE_REGSAVE_GBR,
		TKPE_REGSAVE_R4 , TKPE_REGSAVE_R5 , TKPE_REGSAVE_R6 , TKPE_REGSAVE_R7 ,
		TKPE_REGSAVE_R8 , TKPE_REGSAVE_R9 , TKPE_REGSAVE_R10, TKPE_REGSAVE_R11,
		TKPE_REGSAVE_R12, TKPE_REGSAVE_R13, TKPE_REGSAVE_R2 , TKPE_REGSAVE_R3 ,
		TKPE_REGSAVE_R16, TKPE_REGSAVE_R17, TKPE_REGSAVE_R18, TKPE_REGSAVE_R19,
		TKPE_REGSAVE_R20, TKPE_REGSAVE_R21, TKPE_REGSAVE_R22, TKPE_REGSAVE_R23,
		TKPE_REGSAVE_R24, TKPE_REGSAVE_R25, TKPE_REGSAVE_R26, TKPE_REGSAVE_R27,
		TKPE_REGSAVE_R28, TKPE_REGSAVE_R29, TKPE_REGSAVE_R30, TKPE_REGSAVE_R31
	};
	u32 opw;
	int rn, rm, ro, rmf, rnf, rof, isok, fpsr;
	int rmxf, rnxf, roxf;

	opw=*opp;

	rn=(opw>> 7);	rm=(opw>>15);	ro=(opw>>20);
	rn&=31;			rm&=31;			ro&=31;
	rnf=rn+32;		rmf=rm+32;		rof=ro+32;
	isok=0;

	rm=rvremap[rm];
	rn=rvremap[rn];
	ro=rvremap[ro];
	
	rmxf=rmf;
	rnxf=rnf;
	roxf=rof;
	if(rmf&1)	rmxf=rm&(~1);
	if(rnf&1)	rnxf=rn&(~1);
	if(rof&1)	roxf=ro&(~1);
	
	fpsr=regs[TKPE_REGSAVE_SSP]>>48;

	if((opw&0x7F)==0x53)
	{
		switch(opw>>25)
		{
		case 0x00:
			/* FADD.S */
			regs[rnf]=__sfp_padd_f32(regs[rmf], regs[rof]);
			isok=1;
			break;
		case 0x01:
			/* FADD.D */
			regs[rnf]=__sfp_fadd_f64(regs[rmf], regs[rof]);
			isok=1;
			break;
		case 0x02:
			/* FADD.H */
			regs[rnf]=__sfp_padd_f16(regs[rmf], regs[rof]);
			isok=1;
			break;
		case 0x03:
			/* FADD.Q */
			__sqfp_fadd_f128fv(regs+rmxf, regs+roxf, regs+rnxf);
			isok=1;
			break;

		case 0x04:
			/* FSUB.S */
			regs[rnf]=__sfp_psub_f32(regs[rmf], regs[rof]);
			isok=1;
			break;
		case 0x05:
			/* FSUB.D */
			regs[rnf]=__sfp_fsub_f64(regs[rmf], regs[rof]);
			isok=1;
			break;
		case 0x06:
			/* FSUB.H */
			regs[rnf]=__sfp_psub_f16(regs[rmf], regs[rof]);
			isok=1;
			break;
		case 0x07:
			/* FSUB.Q */
			__sqfp_fsub_f128fv(regs+rmxf, regs+roxf, regs+rnxf);
			isok=1;
			break;

		case 0x08:
			/* FMUL.S */
			regs[rnf]=__sfp_pmul_f32(regs[rmf], regs[rof]);
			isok=1;
			break;
		case 0x09:
			/* FMUL.D */
			regs[rnf]=__sfp_fmul_f64(regs[rmf], regs[rof]);
			isok=1;
			break;
		case 0x0A:
			/* FMUL.S */
			regs[rnf]=__sfp_pmul_f16(regs[rmf], regs[rof]);
			isok=1;
			break;
		case 0x0B:
			/* FMUL.Q */
			__sqfp_fmul_f128fv(regs+rmxf, regs+roxf, regs+rnxf);
			isok=1;
			break;

		case 0x0C:
			/* FDIV.S */
			regs[rnf]=__sfp_pdiv_f32(regs[rmf], regs[rof]);
			isok=1;
			break;
		case 0x0D:
			/* FDIV.D */
			regs[rnf]=__sfp_fdiv_f64(regs[rmf], regs[rof]);
			isok=1;
			break;
		case 0x0E:
			/* FDIV.S */
			regs[rnf]=__sfp_pdiv_f16(regs[rmf], regs[rof]);
			isok=1;
			break;
		case 0x0F:
			/* FDIV.Q */
			__sqfp_fdiv_f128fv(regs+rmxf, regs+roxf, regs+rnxf);
			isok=1;
			break;

		case 0x21:
			switch((opw>>20)&31)
			{
			case 0:
				/* FCVT.D.S */
				regs[rnf]=__sfp_fcnvsd(regs[rmf]);
				isok=1;
				break;
			case 1:
				/* FCVT.D.D */
				regs[rnf]=regs[rmf];
				isok=1;
				break;

			case 3:
				/* FCVT.D.Q */
				__sqfp_fcvt_q2d(regs+rmxf, regs+rnf);
				isok=1;
				break;
			}
			break;

		case 0x23:
			switch((opw>>20)&31)
			{
			case 0:
				/* FCVT.Q.S */
				__sqfp_fcvt_s2q(regs+rmf, regs+rnxf);
				isok=1;
				break;
			case 1:
				/* FCVT.Q.D */
				__sqfp_fcvt_d2q(regs+rmf, regs+rnxf);
				isok=1;
				break;
			case 3:
				/* FCVT.Q.Q */
				regs[rnxf+0]=regs[rmxf+0];
				regs[rnxf+1]=regs[rmxf+1];
				isok=1;
				break;
			}
			break;

		case 0x2C:
			/* FSQRT.S */
			regs[rnf]=__sfp_psqrt_f32(regs[rmf]);
			isok=1;
			break;
		case 0x2D:
			/* FSQRT.D */
			regs[rnf]=__sfp_fsqrt_f64(regs[rmf]);
			isok=1;
			break;
		case 0x2E:
			/* FSQRT.H */
			regs[rnf]=__sfp_psqrt_f16(regs[rmf]);
			isok=1;
			break;
		case 0x2F:
			/* FSQRT.Q */
			__sqfp_fsqrt_f128fv(regs+rmxf, regs+rnxf);
			isok=1;
			break;

		case 0x60:
			/* FCVT.L.S */
			regs[rnf]=__sfp_float_f32(regs[rmf]);
			isok=1;
			break;
		case 0x61:
			/* FCVT.L.D */
			regs[rnf]=__sfp_float_f64(regs[rmf]);
			isok=1;
			break;

		case 0x63:
			/* FCVT.L.Q */
			__sqfp_fcvt_i2q(regs+rm, regs+rnxf);
			isok=1;
			break;

		case 0x6B:
			/* FCVT.Q.L */
			__sqfp_fcvt_q2i(regs+rmxf, regs+rn);
			isok=1;
			break;

		default:
			break;
		}
	}
	
	if(isok)
	{
		/* instruction was emulated. */
		regs[TKPE_REGSAVE_SPC]=regs[TKPE_REGSAVE_SPC]+4;
		return;
	}

	tk_dbg_printf("tk_vmem_emurq_rv: EMU REQ: %08X @ %p, FPSR=%04X\n",
		opw, spc, fpsr);
	__debugbreak();
}

void tk_vmem_emurq_xg3(u64 spc, u64 exc, u64 *regs, u32 *opp)
{
	u32 opw;
	int rn, rm, ro, isok, id;

	opw=*opp;

	if((opw&3)==3)
	{
		tk_vmem_emurq_rv(spc, exc, regs, opp);
		return;
	}

	rn=(opw>> 6);	rm=(opw>>16);	ro=(opw>>22);
	rn&=63;			rm&=63;			ro&=63;
	isok=0;

	
	id=((opw>>2)&0x0F)|((opw>>8)&0xF0);
	switch(id)
	{
	case 0x20:
		switch((opw>>28)&15)
		{
		case 0x5:
			/* FADD.S */
			regs[rn]=__sfp_padd_f32(regs[rm], regs[ro]);
			isok=1;
			break;
		case 0x6:
			/* FSUB.S */
			regs[rn]=__sfp_psub_f32(regs[rm], regs[ro]);
			isok=1;
			break;
		case 0x7:
			/* FMUL.S */
			regs[rn]=__sfp_pmul_f32(regs[rm], regs[ro]);
			isok=1;
			break;
		}
		break;

	case 0x50:
		switch((opw>>28)&15)
		{
		case 0x8:
			/* FADD.D */
			regs[rn]=__sfp_fadd_f64(regs[rm], regs[ro]);
			isok=1;
			break;
		case 0x9:
			/* FSUB.D */
			regs[rn]=__sfp_fsub_f64(regs[rm], regs[ro]);
			isok=1;
			break;
		case 0xA:
			/* FMUL.D */
			regs[rn]=__sfp_fmul_f64(regs[rm], regs[ro]);
			isok=1;
			break;
		}
		break;
	}
}

void tk_vmem_emurq(u64 spc, u64 exc, u64 *regs)
{
	u32 ssr, opw;
	u32 *opp;
	
	opp=tk_vmem_virttophys(spc);
	
	ssr=exc>>32;
	if(((ssr>>26)&1) && !((ssr>>23)&1))
	{
		if((ssr>>27)&1)
		{
			tk_vmem_emurq_xg3(spc, exc, regs, opp);
			return;
		}

		tk_vmem_emurq_rv(spc, exc, regs, opp);
		return;
	}
	
	opw=*opp;
}

#ifdef __BJX2__
__interrupt void __isr_tlbfault(void)
// __interrupt_tbrsave void __isr_tlbfault(void)
// __declspec(dllexport) void isr_tlbfault_i(void)
{
	TKPE_TaskInfo *task, *task2, *task3;
	TKPE_TaskInfoKern *taskern, *taskern2;
	u32 *isrsave;

	u64 ttb, tea, teah, exc, spc;
	u16 exsr;

	ttb=__arch_ttb;
	tea=__arch_tea;
	teah=__arch_teah;
	exc=__arch_exsr;
	spc=__arch_spc;
	exsr=(u16)(exc);
//	tea=exc>>16;

	if(((tea>>40)&255)==0x7F)
	{
		__debugbreak();
	}

	task=(TKPE_TaskInfo *)__arch_tbr;
	if(!task)
		__debugbreak();
	if(task->magic0!=TKPE_TASK_MAGIC)
		__debugbreak();

	taskern=(TKPE_TaskInfoKern *)task->krnlptr;
	if(taskern->magic0!=TKPE_TASK_MAGIC)
		__debugbreak();

	task2=task;

	if(exsr==0xA001)
	{
		tk_vmem_tlbmiss(ttb, tea, teah);
	}else
		if(exsr==0xA002)
	{
		tk_vmem_aclmiss(ttb, tea, teah);
		return;
	}else
		if(exsr==0xA003)
	{
		isrsave=__arch_isrsave;
		tk_vmem_emurq(spc, exc, isrsave);
	}else
	{
		__debugbreak();
	}

#if 0
	if(task2!=task)
	{
		if(task2->magic0!=TKPE_TASK_MAGIC)
			__debugbreak();

		taskern2=(TKPE_TaskInfoKern *)task2->krnlptr;
		if(taskern2->magic0!=TKPE_TASK_MAGIC)
			__debugbreak();

		if(!taskern2->ctx_regsave[TKPE_REGSAVE_SPC])
			__debugbreak();

		taskern->ctx_regsave[TKPE_REGSAVE_KRR]=__arch_krr;
		taskern->ctx_regsave[TKPE_REGSAVE_TTB]=__arch_ttb;

		__ifarch(has_xmov)
		{
			taskern->ctx_regsave[TKPE_REGSAVE_PCH]=__arch_pch;
			taskern->ctx_regsave[TKPE_REGSAVE_GBH]=__arch_gbh;
		}

		ttb=taskern2->ctx_regsave[TKPE_REGSAVE_TTB];
		if(!ttb)
			{ ttb=tk_vmem_pageglobal; }

		__arch_krr=taskern2->ctx_regsave[TKPE_REGSAVE_KRR];
		__arch_ttb=ttb;

//		if(__arch_ifarch__has_xmov)
		__ifarch(has_xmov)
		{
			__arch_pch=taskern2->ctx_regsave[TKPE_REGSAVE_PCH];
			__arch_gbh=taskern2->ctx_regsave[TKPE_REGSAVE_GBH];
		}

		__arch_tbr=(u64 *)task2;
	}
#endif

}
#endif

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
