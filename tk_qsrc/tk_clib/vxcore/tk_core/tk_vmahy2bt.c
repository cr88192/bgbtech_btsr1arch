/*
Hybrid-2 B-Tree

The upper bits of the address will be managed by a B-Tree, with the final levels handled using a conventional page-table structure.

For 4K pages:
  Low 39 bits will be handled with a 3-level page table.

For 16K pages:
  Low 36 bits will be handled with a 2-level page table.

For 64K pages:
  Low 42 bits will be handled with a 2-level page table.

 */


#if (TKMM_PAGEBITS==12)
#define TK_VMAHYB2_NODESZ		254
#define TK_VMAHYB2_NODEFULL		252
#endif

#if (TKMM_PAGEBITS==14)
#define TK_VMAHYB2_NODESZ		1022
#define TK_VMAHYB2_NODEFULL		1020
#endif

#if (TKMM_PAGEBITS==16)
#define TK_VMAHYB2_NODESZ		4094
#define TK_VMAHYB2_NODEFULL		4092
#endif

typedef struct TK_VmaHyb2Node_s TK_VmaHyb2Node;
struct TK_VmaHyb2Node_s {
u64		npt_next;	//0000
u64		npt_prev;	//0008
u64		npt_up;		//0010
u16		n_ecnt;		//0018, Entry Count
byte	level;		//001A
byte	pad1[5];	//001B
u64		addr_hi[TK_VMAHYB2_NODESZ];	//..
u64		pte_val[TK_VMAHYB2_NODESZ];	//..
};

u64 tk_vmembt2_hash_ttb[256];
u64 tk_vmembt2_hash_tea[256];
u64 tk_vmembt2_hash_pte[256];

int TK_VMem_GetBt2TableHash(s64 ttb, u64 tea)
{
	u64 th;
	int h;

//	th=tea*65521+ttb+1;
	th=tea+ttb+1;
	th=th*65521+(th>>48)+1;
	th=th*65521+(th>>48)+1;
	th=th*65521+(th>>48)+1;
	h=(th>>48)&255;
	
	return(h);
}

u64 TK_VMem_GetBt2PageTableEntry(s64 ttb, u64 tea)
{
	TK_VmaHyb2Node *head;
	u64 *addr;
	u64 pva, pte;
	u64 th;
	int i, m, n, h;

	h=TK_VMem_GetBt2TableHash(ttb, tea);
	th=tk_vmembt2_hash_ttb[h];
	pva=tk_vmembt2_hash_tea[h];
	pte=tk_vmembt2_hash_pte[h];
	if((th==ttb) && (pva==tea))
	{
		return(pte);
	}

	head=(TK_VmaHyb2Node *)TK_VMem_PtrForPto(ttb);

#if 1
	while(1)
	{
		addr=head->addr_hi;
		pva=-1;
		m=0;	n=head->n_ecnt;

#if 1
		while((n-m)>3)
//		while((n-m)>0)
//		while(m<=n)
		{
			i=(m+n)>>1;
			pva=addr[i];

			if(tea<pva)
				{ n=i; continue; }
			if(tea>pva)
				{ m=i+1; continue; }

			if(!head->level)
			{
				pte=head->pte_val[i];

				tk_vmembt2_hash_ttb[h]=ttb;
				tk_vmembt2_hash_tea[h]=tea;
				tk_vmembt2_hash_pte[h]=pte;

				return(pte);
			}

			break;
		}
#endif

#if 1
		addr=head->addr_hi;
//		n=head->n_ecnt;
//		for(i=0; i<n; i++)
		for(i=m; i<n; i++)
		{
			pva=addr[i];
			if(pva>=tea)
				break;
		}
#endif

		if(!head->level)
			break;

		if((pva>tea) && (i>0))
			i--;

		pte=head->pte_val[i];
		head=(TK_VmaHyb2Node *)TK_VMem_PtrForPto(pte);
	}
#endif

	
	if(tea==pva)
	{
		pte=head->pte_val[i];

		tk_vmembt2_hash_ttb[h]=ttb;
		tk_vmembt2_hash_tea[h]=tea;
		tk_vmembt2_hash_pte[h]=pte;

		return(pte);
	}
	
	return(0);
}

int TK_VMem_SetBt2PageTableEntry(s64 ttb, u64 tea, u64 pteval)
{
	TK_VmaHyb2Node *head;
	u64 *addr, *val;
	u64 pva, pte;
	int i, j, k, m, n, h;

	h=TK_VMem_GetBt2TableHash(ttb, tea);
	tk_vmembt2_hash_ttb[h]=ttb;
	tk_vmembt2_hash_tea[h]=tea;
	tk_vmembt2_hash_pte[h]=pteval;

	head=(TK_VmaHyb2Node *)TK_VMem_PtrForPto(ttb);
	
	while(1)
	{
		addr=head->addr_hi;
		pva=-1;
		m=0;	n=head->n_ecnt;

#if 1
		while((n-m)>5)
//		while(m<=n)
		{
			i=(m+n)>>1;
			pva=addr[i];

			if(tea<pva)
				{ n=i; continue; }
			if(tea>pva)
				{ m=i+1; continue; }

			if(!head->level)
			{
				head->pte_val[i]=pteval;
				return(1);
			}

			break;
		}
#endif

#if 1
		addr=head->addr_hi;
//		n=head->n_ecnt;
//		for(i=0; i<n; i++)
		for(i=m; i<n; i++)
		{
			pva=addr[i];
			if(pva>=tea)
				break;
		}
#endif

		if(!head->level)
			break;

		if((pva>tea) && (i>0))
			i--;

		pte=head->pte_val[i];
		head=(TK_VmaHyb2Node *)TK_VMem_PtrForPto(pte);
	}
	
	if(tea==pva)
	{
		head->pte_val[i]=pteval;
		return(1);
	}
	
	if(head->n_ecnt<TK_VMAHYB2_NODEFULL)
	{
		addr=head->addr_hi;
		val=head->pte_val;

		j=head->n_ecnt;
		head->n_ecnt++;
		
		while(j>i)
		{
			addr[j]=addr[j-1];
			val[j]=val[j-1];
			j--;
		}
		addr[i]=tea;
		val[i]=pteval;
		return(1);
	}
	
	/* TODO: Add node split. */
	__debugbreak();
	
	return(0);
}


u64 TK_VMem_GetTab2PageTableEntry(s64 ttb, u64 vaddr)
{
	u64 *ptp;
	u64 pte, pde;
	int vp0, vp1, vp2, vpn;

	ptp=(u64 *)(ttb&(~((1<<TK_VMEM_PAGESHL)-1)));

#if (TKMM_PAGEBITS==14)
	vpn=(vaddr>>14);
	vp1=(vpn>>11)&2047;
	vp0=(vpn    )&2047;
	
	pde=ptp[vp1];
	if(!(pde&1))
		return(0);
//	ptp=(u64 *)(pde&(~((1<<TK_VMEM_PAGESHL)-1)));
	ptp=TK_VMem_PtrForPto(pde);

	pte=ptp[vp0];
	return(pte);
#endif

#if (TKMM_PAGEBITS==16)
	vpn=(vaddr>>16);
	vp1=(vpn>>13)&8191;
	vp0=(vpn    )&8191;
	
	pde=ptp[vp1];
	if(!(pde&1))
		return(0);
//	ptp=(u64 *)(pde&(~((1<<TK_VMEM_PAGESHL)-1)));
	ptp=TK_VMem_PtrForPto(pde);

	pte=ptp[vp0];
	return(pte);
#endif

#if (TKMM_PAGEBITS==12)
	vpn=(vaddr>>12);
	vp2=(vpn>>18)&511;
	vp1=(vpn>> 9)&511;
	vp0=(vpn    )&511;
	
	pde=ptp[vp2];
	if(!(pde&1))
		return(0);
//	ptp=(u64 *)(pde&(~((1<<TK_VMEM_PAGESHL)-1)));
	ptp=TK_VMem_PtrForPto(pde);

	pde=ptp[vp1];
	if(!(pde&1))
		return(0);
//	ptp=(u64 *)(pde&(~((1<<TK_VMEM_PAGESHL)-1)));
	ptp=TK_VMem_PtrForPto(pde);

	pte=ptp[vp0];
	return(pte);
#endif
}

u64 TK_VMem_SetTab2PageTableEntry(s64 ttb, u64 vaddr, u64 pte)
{
	u64 *ptp;
	u64 pde;
	int vp0, vp1, vp2, vpn, ptpn;

	ptp=(u64 *)(ttb&(~((1<<TK_VMEM_PAGESHL)-1)));

#if (TKMM_PAGEBITS==14)
	vpn=(vaddr>>14);
	vp1=(vpn>>11)&2047;
	vp0=(vpn    )&2047;
	
	pde=ptp[vp1];
	if(!(pde&1))
	{
		ptpn=TKMM_AllocPagesZeroedApn(1);
		pde=(((u64)ptpn)<<TK_VMEM_PTESHL)|1;
		ptp[vp1]=pde;
	}
//	ptp=(u64 *)(pde&(~((1<<TK_VMEM_PAGESHL)-1)));
	ptp=TK_VMem_PtrForPto(pde);

	ptp[vp0]=pte;
	return(pte);
#endif

#if (TKMM_PAGEBITS==16)
	vpn=(vaddr>>16);
	vp1=(vpn>>13)&8191;
	vp0=(vpn    )&8191;
	
	pde=ptp[vp1];
	if(!(pde&1))
	{
		ptpn=TKMM_AllocPagesZeroedApn(1);
		pde=(((u64)ptpn)<<TK_VMEM_PTESHL)|1;
		ptp[vp1]=pde;
	}

//	ptp=(u64 *)(pde&(~((1<<TK_VMEM_PAGESHL)-1)));
	ptp=TK_VMem_PtrForPto(pde);

	ptp[vp0]=pte;
	return(pte);
#endif

#if (TKMM_PAGEBITS==12)
	vpn=(vaddr>>12);
	vp2=(vpn>>18)&511;
	vp1=(vpn>> 9)&511;
	vp0=(vpn    )&511;
	
	pde=ptp[vp2];
	if(!(pde&1))
	{
		ptpn=TKMM_AllocPagesZeroedApn(1);
		pde=(((u64)ptpn)<<TK_VMEM_PTESHL)|1;
		ptp[vp2]=pde;
	}

//	ptp=(u64 *)(pde&(~((1<<TK_VMEM_PAGESHL)-1)));
	ptp=TK_VMem_PtrForPto(pde);

	pde=ptp[vp1];
	if(!(pde&1))
	{
		ptpn=TKMM_AllocPagesZeroedApn(1);
		pde=(((u64)ptpn)<<TK_VMEM_PTESHL)|1;
		ptp[vp1]=pde;
	}

//	ptp=(u64 *)(pde&(~((1<<TK_VMEM_PAGESHL)-1)));
	ptp=TK_VMem_PtrForPto(pde);

	ptp[vp0]=pte;
	return(pte);
#endif
}


u64 TK_VMem_GetHyb2PageTableEntry(s64 ttb, u64 teal, u64 teah)
{
	u64 addrh, pteh, pte;

#if (TKMM_PAGEBITS==14)
	addrh=(teah<<12)|((teal>>36)&0x0FFF);
	pteh=TK_VMem_GetBt2PageTableEntry(ttb, addrh);
	if(!(pteh&1))
		return(0);
	pte=TK_VMem_GetTab2PageTableEntry(pteh, teal);
	return(pte);
#endif

#if (TKMM_PAGEBITS==12)
	addrh=(teah<<9)|((teal>>39)&0x01FF);
	pteh=TK_VMem_GetBt2PageTableEntry(ttb, addrh);
	if(!(pteh&1))
		return(0);
	pte=TK_VMem_GetTab2PageTableEntry(pteh, teal);
	return(pte);
#endif

#if (TKMM_PAGEBITS==16)
	addrh=(teah<<6)|((teal>>42)&0x003F);
	pteh=TK_VMem_GetBt2PageTableEntry(ttb, addrh);
	if(!(pteh&1))
		return(0);
	pte=TK_VMem_GetTab2PageTableEntry(pteh, teal);
	return(pte);
#endif
}

int TK_VMem_SetHyb2PageTableEntry(s64 ttb, u64 teal, u64 teah, u64 ptval)
{
	u64 addrh, pteh, pte;
	int ptpn;

#if (TKMM_PAGEBITS==14)
	addrh=(teah<<12)|((teal>>36)&0x0FFF);
	pteh=TK_VMem_GetBt2PageTableEntry(ttb, addrh);
	if(!(pteh&1))
	{
		ptpn=TKMM_AllocPagesZeroedApn(1);
		pteh=(((u64)ptpn)<<TK_VMEM_PTESHL)|1;
		TK_VMem_SetBt2PageTableEntry(ttb, addrh, pteh);
	}
	pte=TK_VMem_SetTab2PageTableEntry(pteh, teal, ptval);
	return(pte);
#endif

#if (TKMM_PAGEBITS==12)
	addrh=(teah<<9)|((teal>>39)&0x01FF);
	pteh=TK_VMem_GetBt2PageTableEntry(ttb, addrh);
	if(!(pteh&1))
	{
		ptpn=TKMM_AllocPagesZeroedApn(1);
		pteh=(((u64)ptpn)<<TK_VMEM_PTESHL)|1;
		TK_VMem_SetBt2PageTableEntry(ttb, addrh, pteh);
	}
	pte=TK_VMem_SetTab2PageTableEntry(pteh, teal, ptval);
	return(pte);
#endif

#if (TKMM_PAGEBITS==16)
	addrh=(teah<<6)|((teal>>42)&0x003F);
	pteh=TK_VMem_GetBt2PageTableEntry(ttb, addrh);
	if(!(pteh&1))
	{
		ptpn=TKMM_AllocPagesZeroedApn(1);
		pteh=(((u64)ptpn)<<TK_VMEM_PTESHL)|1;
		TK_VMem_SetBt2PageTableEntry(ttb, addrh, pteh);
	}
	pte=TK_VMem_SetTab2PageTableEntry(pteh, teal, ptval);
	return(pte);
#endif
}
