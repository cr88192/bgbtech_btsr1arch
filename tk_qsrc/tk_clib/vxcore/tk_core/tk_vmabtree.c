/*
B-Tree intended for Virtual Address Translation.
Trying to make its speed passable...
 */

#if (TKMM_PAGEBITS==12)
// #define TK_VMABTREE_NODESZ		202
// #define TK_VMABTREE_NODEFULL		200
// #define TK_VMABTREE_NODEPAD2		24
#define TK_VMABTREE_NODESZ		203
#define TK_VMABTREE_NODEFULL	201
#define TK_VMABTREE_NODEPAD2	4
#endif

#if (TKMM_PAGEBITS==14)
// #define TK_VMABTREE_NODESZ		816
// #define TK_VMABTREE_NODEFULL		814
// #define TK_VMABTREE_NODEPAD2		32
#define TK_VMABTREE_NODESZ		817
#define TK_VMABTREE_NODEFULL	815
#define TK_VMABTREE_NODEPAD2	12
#endif

#if (TKMM_PAGEBITS==16)
// #define TK_VMABTREE_NODESZ		3274
// #define TK_VMABTREE_NODEFULL		3272
// #define TK_VMABTREE_NODEPAD2		24
#define TK_VMABTREE_NODESZ		3275
#define TK_VMABTREE_NODEFULL	3273
#define TK_VMABTREE_NODEPAD2	4
#endif

#define TK_VMABTREE_HYBSHR		(TKMM_PAGEBITS+(TKMM_PAGEBITS-3))

typedef struct TK_VmaBTreeNode_s TK_VmaBTreeNode;
struct TK_VmaBTreeNode_s {
u64		npt_next;	//0000
u64		npt_prev;	//0008
u64		npt_up;		//0010
u16		n_ecnt;		//0018, Entry Count
byte	level;		//001A
byte	pad1[5];	//001B
byte	pad2[TK_VMABTREE_NODEPAD2];		//0020..
u32		addr_hi[TK_VMABTREE_NODESZ];	//..
u64		addr_lo[TK_VMABTREE_NODESZ];	//..
u64		pte_val[TK_VMABTREE_NODESZ];	//..
};

u64 TK_VMem_GetBTreePageTableEntryI(s64 ttb, u64 va_lo, u32 va_hi)
{
	TK_VmaBTreeNode *head;
	u64 *addrlo;
	u32 *addrhi;
	u64 pva_lo;
	u32 pva_hi;
	u64 pte;
	int i, j, k, m, n;

	head=(TK_VmaBTreeNode *)TK_VMem_PtrForPto(ttb);
	
//	va_lo=(teal&0x0000FFFFFFFFFFFFULL)|(teah<<48);
//	va_hi=(teah>>16);
//	va_lo&=~((1ULL<<TKMM_PAGEBITS)-1);
	
	while(1)
	{
		addrhi=head->addr_hi;
		addrlo=head->addr_lo;
		pva_hi=-1;
		pva_lo=-1;

		/* Binary search for address. */
		m=0; n=head->n_ecnt;
		i=(m+n)>>1;
		while((n-m)>1)
//		while(n>m)
		{
			i=(m+n)>>1;
			pva_hi=addrhi[i];
			pva_lo=addrlo[i];

			if(va_hi<pva_hi)
				{ n=i; continue; }
			if(va_hi>pva_hi)
				{ m=i; continue; }

			if(va_lo<pva_lo)
				{ n=i; continue; }
			if(va_lo>pva_lo)
				{ m=i; continue; }

			if(!head->level)
			{
				pte=head->pte_val[i];
				return(pte);
			}

			break;
		}
//		i=(m+n)>>1;

#if 1
		if((va_lo!=pva_lo) || (va_hi!=pva_hi))
		{
			i=m;
			while(i<n)
			{
				pva_hi=addrhi[i+1];
				pva_lo=addrlo[i+1];
				if(pva_hi>va_hi)
					break;
				if(pva_lo>va_lo)
					break;
				i++;
			}
			pva_hi=addrhi[i];
			pva_lo=addrlo[i];
		}
#endif

		/* Break out of loop at bottom of tree. */
		if(!head->level)
			break;
		
		/* Next node down. */
		pte=head->pte_val[i];
		head=(TK_VmaBTreeNode *)TK_VMem_PtrForPto(pte);
	}

	if((va_hi==pva_hi) && (va_lo==pva_lo))
	{
		pte=head->pte_val[i];
		return(pte);
	}
	return(0);
}

int TK_VMem_SetBTreePageTableEntryI(s64 ttb, u64 va_lo, u32 va_hi, u64 ptval)
{
//	TK_VmaBTreeNode *headstk[16];
	TK_VmaBTreeNode *head, *head1, *head2;
	u64 *addrlo, *addrpt;
	u32 *addrhi;
	u64 pva_lo, pva1_lo;
	u32 pva_hi, pva1_hi;
	u64 pte, pte1, pte2, hptd;
	int i, j, k, m, n, hsp;

//	if(sizeof(TK_VmaBTreeNode)!=(1<<TKMM_PAGEBITS))
//	{
//		printf("Node Size Bad, %d\n", sizeof(TK_VmaBTreeNode));
//	}

	head=(TK_VmaBTreeNode *)TK_VMem_PtrForPto(ttb);
	hptd=ttb;

//	va_lo=(teal&0x0000FFFFFFFFFFFFULL)|(teah<<48);
//	va_hi=(teah>>16);
//	va_lo&=~((1ULL<<TKMM_PAGEBITS)-1);

	n=head->n_ecnt;	
	if(n>=TK_VMABTREE_NODEFULL)
	{
		/* Split Top-Level Node */
	
		head1=TKMM_PageAlloc(1<<TKMM_PAGEBITS);
		head2=TKMM_PageAlloc(1<<TKMM_PAGEBITS);
		pte1=TK_VMem_PtoForPtr((void *)head1)|1;
		pte2=TK_VMem_PtoForPtr((void *)head2)|1;

		memcpy(head1, head, 1<<TKMM_PAGEBITS);

		head1->npt_next=pte2;
		head2->npt_prev=pte1;
		head2->level=head1->level;

		j=head1->n_ecnt>>1;
		for(k=j; k<head1->n_ecnt; k++)
		{
			pva_hi=head1->addr_hi[k];
			pva_lo=head1->addr_lo[k];
			pte=head1->pte_val[k];
			head2->addr_hi[k-j]=pva_hi;
			head2->addr_lo[k-j]=pva_lo;
			head2->pte_val[k-j]=pte;
		}

		head2->n_ecnt=head1->n_ecnt-j;
		head1->n_ecnt=j;

		pva_hi=head1->addr_hi[0];
		pva_lo=head1->addr_lo[0];
		head->addr_hi[0]=pva_hi;
		head->addr_lo[0]=pva_lo;
		head->pte_val[0]=pte1;

		pva_hi=head2->addr_hi[0];
		pva_lo=head2->addr_lo[0];
		head->addr_hi[1]=pva_hi;
		head->addr_lo[1]=pva_lo;
		head->pte_val[1]=pte2;
		head->n_ecnt=2;
		head->level=head1->level+1;

		head->addr_hi[2]=-1;
		head->addr_lo[2]=-1;
	}

//	hsp=0;
	while(1)
	{
//		headstk[hsp++]=head;
	
		addrhi=head->addr_hi;
		addrlo=head->addr_lo;
		addrpt=head->pte_val;

		m=0; n=head->n_ecnt;
		while((n-m)>1)
		{
			i=(m+n)>>1;
//			pva_hi=head->addr_hi[i];
//			pva_lo=head->addr_lo[i];
			pva_hi=addrhi[i];
			pva_lo=addrlo[i];

			if(va_hi<pva_hi)
				{ n=i; continue; }
			if(va_hi>pva_hi)
				{ m=i; continue; }

			if(va_lo<pva_lo)
				{ n=i; continue; }
			if(va_lo>pva_lo)
				{ m=i; continue; }
			break;
		}

#if 1
		i=m;
		while(i<n)
		{
//			pva_hi=head->addr_hi[i+1];
//			pva_lo=head->addr_lo[i+1];
			pva_hi=addrhi[i+1];
			pva_lo=addrlo[i+1];
			if(pva_hi>va_hi)
				break;
			if(pva_lo>va_lo)
				break;
			i++;
		}
//		pva_hi=head->addr_hi[i];
//		pva_lo=head->addr_lo[i];
		pva_hi=addrhi[i];
		pva_lo=addrlo[i];
#endif

		if(!head->level)
			break;

		pte=head->pte_val[i];
		head1=(TK_VmaBTreeNode *)TK_VMem_PtrForPto(pte);
		if(head1->n_ecnt<TK_VMABTREE_NODEFULL)
		{
			/* Continue down tree if child node is not full. */
			hptd=pte;
			head=head1;
			continue;
		}

		/* If Node is almost full, Split Node */
		head2=TKMM_PageAlloc(1<<TKMM_PAGEBITS);
		pte1=TK_VMem_PtoForPtr((void *)head2)|1;

		head2->npt_next=head1->npt_next;
		head1->npt_next=pte1;
		head2->npt_prev=pte;
		head2->level=head1->level;

		k=head1->n_ecnt>>1;
		pva1_hi=head1->addr_hi[k];
		pva1_lo=head1->addr_lo[k];

		j=head->n_ecnt++;
//		while(j>i)
		while(j>0)
		{
			pva_hi=addrhi[j-1];
			pva_lo=addrlo[j-1];
			pte=addrpt[j-1];
//			if(pva_hi<va_hi)
//				break;
//			if(pva_lo<va_lo)
//				break;
			if(pva_hi<pva1_hi)
				break;
			if(pva_lo<pva1_lo)
				break;
			addrhi[j]=pva_hi;
			addrlo[j]=pva_lo;
			addrpt[j]=pte;
			j--;
		}

//		k=head1->n_ecnt>>1;
//		pva_hi=head1->addr_hi[k];
//		pva_lo=head1->addr_lo[k];
		head->addr_hi[j]=pva1_hi;
		head->addr_lo[j]=pva1_lo;
		head->pte_val[j]=pte1;

		j=head->n_ecnt;
		head->addr_hi[j]=-1;
		head->addr_lo[j]=-1;

		k=head1->n_ecnt;
		j=head1->n_ecnt>>1;

#if 1
		for(k=j; k<=head1->n_ecnt; k++)
		{
			pva_hi=head1->addr_hi[k];
			pva_lo=head1->addr_lo[k];
			pte=head1->pte_val[k];
			head2->addr_hi[k-j]=pva_hi;
			head2->addr_lo[k-j]=pva_lo;
			head2->pte_val[k-j]=pte;
		}
#endif

//		k=(k-j)+1;
//		memcpy(head2->addr_hi, head1->addr_hi+j, k*sizeof(u32));
//		memcpy(head2->addr_lo, head1->addr_lo+j, k*sizeof(u64));
//		memcpy(head2->pte_val, head1->pte_val+j, k*sizeof(u64));

		head2->n_ecnt=head1->n_ecnt-j;
		head1->n_ecnt=j;
		head1->addr_hi[j]=-1;
		head1->addr_lo[j]=-1;

//		j=head2->n_ecnt;
//		head2->addr_hi[j]=-1;
//		head2->addr_lo[j]=-1;

//		pva_hi=head2->addr_hi[0];
//		pva_lo=head2->addr_lo[0];
//		head->addr_hi[i+1]=pva_hi;
//		head->addr_lo[i+1]=pva_lo;
//		head->pte_val[i+1]=pte1;
	}

	if((va_hi==pva_hi) && (va_lo==pva_lo))
	{
		head->pte_val[i]=ptval;
		return(1);
	}

	addrhi=head->addr_hi;
	addrlo=head->addr_lo;
	addrpt=head->pte_val;

	j=head->n_ecnt++;
	addrhi[j+1]=-1;
	addrlo[j+1]=-1;

	while(j>i)
	{
		pva_hi=addrhi[j-1];
		pva_lo=addrlo[j-1];
		pte=addrpt[j-1];

		if(pva_hi<va_hi)
			break;
		if(pva_lo<va_lo)
			break;
		
		addrhi[j]=pva_hi;
		addrlo[j]=pva_lo;
		addrpt[j]=pte;
		j--;
	}

	addrhi[j]=va_hi;
	addrlo[j]=va_lo;
	addrpt[j]=ptval;
	return(1);
}

u64 TK_VMem_GetBTreePageTableEntry(s64 ttb, u64 teal, u64 teah)
{
	u64 va_lo;
	u32 va_hi;
	u64 pte;

//	head=(TK_VmaBTreeNode *)TK_VMem_PtrForPto(ttb);
	
	va_lo=(teal&0x0000FFFFFFFFFFFFULL)|(teah<<48);
	va_hi=(teah>>16);
	va_lo&=~((1ULL<<TKMM_PAGEBITS)-1);
	
	pte=TK_VMem_GetBTreePageTableEntryI(ttb, va_lo, va_hi);
	return(pte);
}

int TK_VMem_SetBTreePageTableEntry(s64 ttb, u64 teal, u64 teah, u64 ptval)
{
	u64 va_lo;
	u32 va_hi;
	u64 pte;

//	head=(TK_VmaBTreeNode *)TK_VMem_PtrForPto(ttb);
	
	va_lo=(teal&0x0000FFFFFFFFFFFFULL)|(teah<<48);
	va_hi=(teah>>16);
	va_lo&=~((1ULL<<TKMM_PAGEBITS)-1);
	
	pte=TK_VMem_SetBTreePageTableEntryI(ttb, va_lo, va_hi, ptval);
	return(pte);
}


u64 TK_VMem_GetBTreeHybPageTableEntry(s64 ttb, u64 teal, u64 teah)
{
	u64 *pt;
	u64 addr1l, pde, pte;
	u32 addr1h;
	int ix;

	addr1l=
		((teal&0x0000FFFFFFFFFFFFULL)>>TK_VMABTREE_HYBSHR)|
		(teah<<(48-TK_VMABTREE_HYBSHR));
	addr1h=((teah&0x0000FFFFFFFFFFFFULL)>>(16+TK_VMABTREE_HYBSHR));
	ix=(teal>>TKMM_PAGEBITS)&((1<<(TKMM_PAGEBITS-3))-1);

	pde=TK_VMem_GetBTreePageTableEntryI(ttb, addr1l, addr1h);

	if(!pde)
		return(0);
	pt=TK_VMem_PtrForPto(pde);
	pte=pt[ix];
	return(pte);
}

int TK_VMem_SetBTreeHybPageTableEntry(s64 ttb, u64 teal, u64 teah, u64 ptval)
{
	u64 *pt;
	u64 addr1l, pde, pte;
	u32 addr1h;
	int ix;

	addr1l=
		((teal&0x0000FFFFFFFFFFFFULL)>>TK_VMABTREE_HYBSHR)|
		(teah<<(48-TK_VMABTREE_HYBSHR));
	addr1h=((teah&0x0000FFFFFFFFFFFFULL)>>(16+TK_VMABTREE_HYBSHR));
	ix=(teal>>TKMM_PAGEBITS)&((1<<(TKMM_PAGEBITS-3))-1);

	pde=TK_VMem_GetBTreePageTableEntryI(ttb, addr1l, addr1h);

	if(!pde)
	{
		pt=TKMM_PageAlloc(1<<TKMM_PAGEBITS);
		pde=TK_VMem_PtoForPtr(pt)|1;
		TK_VMem_SetBTreePageTableEntryI(ttb, addr1l, addr1h, pde);
	}

	pt=TK_VMem_PtrForPto(pde);
	pt[ix]=ptval;
	return(1);
}
