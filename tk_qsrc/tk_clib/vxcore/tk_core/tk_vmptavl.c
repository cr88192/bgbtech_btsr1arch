/*
Virtual Memory as AVL Trees.

AVL Node:
  (255:172): Virtual Address
  (171:160): ?
  (159:152): Node Depth (0=Leaf)
  (151:128): ?
  (127: 64): Page Table Entry
  ( 63: 32): Left Node
  ( 31:  0): Right Node

Page Table Entry:
  (63:48): VUGID / ACLID
  (47:36): KR Access Mode
  (35:12): Physical Address
  (11: 0): Control Bits

 */

u32 tk_vmemavl_freelist=0;
u64 *tk_vmemavl_refbase=NULL;

#ifdef __BJX2__

#define TK_VMemAvl_PtrForTno(x)		((u64 *)(((u64)(x))<<4))
#define TK_VMemAvl_TnoForPtr(x)		(((u64)(x))>>4)

#else

u64 *TK_VMemAvl_PtrForTno(u32 tno)
{
	return(tk_vmemavl_refbase+(tno<<2));
}

u32 TK_VMemAvl_TnoForPtr(u64 *ptr)
{
	return((ptr-tk_vmemavl_refbase)>>2);
}
#endif

u32 TK_VMemAvl_AllocNode()
{
	u64 *pv, *pva;
	u32 tno, pg;
	int i, j, k;
	
	tno=tk_vmemavl_freelist;
	if(tno)
	{
		pv=TK_VMemAvl_PtrForTno(tno);
		tk_vmemavl_freelist=pv[0];
		memset(pv, 0, 4*8);
		return(tno);
	}
	
	pva=TKMM_PageAlloc(1<<TKMM_PAGEBITS);
	if(!tk_vmemavl_refbase)
		tk_vmemavl_refbase=pva;
	
	pv=pva;
	for(i=0; i<(1<<(TKMM_PAGEBITS-5)); i++)
	{
		tno=TK_VMemAvl_TnoForPtr(pv);
		pv[0]=tk_vmemavl_freelist;
		tk_vmemavl_freelist=tno;
		pv+=4;
	}
	
	tno=tk_vmemavl_freelist;
	pv=TK_VMemAvl_PtrForTno(tno);
	tk_vmemavl_freelist=pv[0];
	memset(pv, 0, 4*8);
	
	if(!tno)
		{ __debugbreak(); }
	
	return(tno);
}

u64 TK_VMem_GetAvlPageTableEntry(s64 ttb, u64 teal, u64 teah)
{
//	static const u128 pvamask=0xFFFFFFFF_FFFFFFFF_FFFFC000_00000000UI128;
//	u128 vladdr, pvaddr;
	u64 vladdr_h, vladdr_l, pvaddr_h, pvaddr_l;
	u64 *pv;
	s64 lna, rna;
	u64 pte;
	int i, isge;
	
//	vladdr=(vaddr<<32)&pvamask;
//	vladdr=((((u128)teah)<<80)|(((u128)teal)<<32))&pvamask;
	vladdr_h=(teah<<16)|((teal>>32)&0xFFFF);
	vladdr_l=(teal<<32)&0xFFFFC00000000000ULL;
//	pv=(u64 *)(ttb&(~31));
	pv=TK_VMemAvl_PtrForTno(ttb>>12);
	
	while(1)
	{
//		pvaddr=(u128 *)(pv+2);
//		pvaddr&=pvamask;

		pvaddr_l=pv[2];
		pvaddr_h=pv[3];
		pvaddr_l&=0xFFFFC00000000000ULL;

		if(	(pvaddr_h==vladdr_h) &&
			(pvaddr_l==vladdr_l))
		{
			pte=pv[1];
			return(pte);
		}
		
		isge=0;
		if(vladdr_h>pvaddr_h)
			{ isge=1; }
		else if((vladdr_h==pvaddr_h) && (vladdr_l>pvaddr_l))
			{ isge=1; }
		
		lna=(u32)(pv[0]>>32);
		rna=(u32)(pv[0]    );
		if(isge)
		{
			if(!rna)
				break;
			pv=TK_VMemAvl_PtrForTno(rna);
		}
		else
		{
			if(!lna)
				break;
			pv=TK_VMemAvl_PtrForTno(lna);
		}
	}
	
	return(0);
}

int TK_VMem_SetAvlPageTableEntry(s64 ttb, u64 teal, u64 teah, u64 ptval)
{
//	static const u128 pvamask=0xFFFFFFFF_FFFFFFFF_FFFFC000_00000000UI128;
	u64 *pvstack[128];
	u64 pvt[4];
//	u128 vladdr, pvaddr;
	u64 vladdr_h, vladdr_l, pvaddr_h, pvaddr_l;
	u64 *pv, *pvl, *pvr;
	s64 lna, rna, lna2, rna2;
	u64 pte;
	u32 ix, ixl, ixr;
	int i, pvspos, isge, nl;
	
//	vladdr=(vaddr<<32)&pvamask;
//	vladdr=((((u128)teah)<<80)|(((u128)teal)<<32))&pvamask;
	vladdr_h=(teah<<16)|((teal>>32)&0xFFFF);
	vladdr_l=(teal<<32)&0xFFFFC00000000000ULL;
//	pv=(u64 *)(ttb&(~31));
	pv=TK_VMemAvl_PtrForTno(ttb>>12);
	
	if(pv[0]==0)
	{
		if(pv[1]==0)
		{
			pv[0]=0;
			pv[1]=ptval;
			pv[2]=vladdr_l;
			pv[3]=vladdr_h;
			return(0);
		}
	}

	pvspos=0;
	while(1)
	{
//		pvaddr=(u128 *)(pv+2);
//		pvaddr&=pvamask;

		pvaddr_l=pv[2];
		pvaddr_h=pv[3];
		pvaddr_l&=0xFFFFC00000000000ULL;
		
		if(	(pvaddr_h==vladdr_h) &&
			(pvaddr_l==vladdr_l))
		{
			pv[1]=ptval;
			return(0);
		}
		
//		ix=(pv[2]>>24)&255;
//		if(!ix)
//			break;
		
		isge=0;
		if(vladdr_h>pvaddr_h)
			{ isge=1; }
		else if((vladdr_h==pvaddr_h) && (vladdr_l>pvaddr_l))
			{ isge=1; }
		
		lna=(u32)(pv[0]>>32);
		rna=(u32)(pv[0]    );
		
		if(pvspos<128)
			pvstack[pvspos++]=pv;

//		if(pvaddr>=vladdr)
		if(isge)
		{
			if(rna)
			{
				pv=TK_VMemAvl_PtrForTno(rna);
				continue;
			}
			
			rna=TK_VMemAvl_AllocNode();
			pv[0]=pv[0]|rna;

			pvr=TK_VMemAvl_PtrForTno(rna);
			pvr[0]=0;
			pvr[1]=ptval;
			pvr[2]=vladdr_l;
			pvr[3]=vladdr_h;
			
			break;
		}
		else
		{
			if(lna)
			{
				pv=TK_VMemAvl_PtrForTno(lna);
				continue;
			}

			lna=TK_VMemAvl_AllocNode();
			pv[0]=pv[0]|(lna<<32);
			
			pvl=TK_VMemAvl_PtrForTno(lna);
			pvl[0]=0;
			pvl[1]=ptval;
			pvl[2]=vladdr_l;
			pvl[3]=vladdr_h;
			
			break;
		}
	}

#if 1
	nl=256;
	i=pvspos-1;
	while(i>=0)
	{
		pv=pvstack[i];

		lna=(u32)(pv[0]>>32);
		rna=(u32)(pv[0]    );

		ixl=0;		ixr=0;
		pvl=NULL;	pvr=NULL;

		if(lna)
		{
			pvl=TK_VMemAvl_PtrForTno(lna);
			ixl=(pvl[2]>>24)&255;
			
			if(pvl[3]>pv[3])
				__debugbreak();
			if((pvl[3]==pv[3]) && (pvl[2]>=pv[2]))
				__debugbreak();
		}

		if(rna)
		{
			pvr=TK_VMemAvl_PtrForTno(rna);
			ixr=(pvr[2]>>24)&255;

			if(pvr[3]<pv[3])
				__debugbreak();
			if((pvr[3]==pv[3]) && (pvr[2]<=pv[2]))
				__debugbreak();
		}

		if(nl>0)
		{
#if 1
			if(ixl>(ixr+1))
	//		if(ixl>(ixr+8))
	//		if(ixl>ixr)
	//		if((ixl>(ixr+4)) && lna && rna)
			{
				/* Rotate Right */
				lna2=(u32)(pvl[0]>>32);
				rna2=(u32)(pvl[0]    );
				
				memcpy(pvt, pv, 4*8);
				memcpy(pv, pvl, 4*8);
				memcpy(pvl, pvt, 4*8);

				pv[0]=(lna2<<32)|lna;
				pvl[0]=(rna2<<32)|rna;
				
				i++;
				pvstack[i]=pvl;
				nl--;
				continue;
			}
#endif

#if 1
	//		if(ixr>(ixl+2))
			if(ixr>(ixl+1))
			{
				/* Rotate Left */
				lna2=(u32)(pvr[0]>>32);
				rna2=(u32)(pvr[0]    );
				
				memcpy(pvt, pv, 4*8);
				memcpy(pv, pvr, 4*8);
				memcpy(pvr, pvt, 4*8);

				pv[0]=(rna<<32)|rna2;
				pvr[0]=(lna<<32)|lna2;
				
				i++;
				pvstack[i]=pvr;
				nl--;
				continue;
			}
#endif
		}
		
		if(lna || rna)
			{ ix=((ixl>ixr)?ixl:ixr)+1; }
		else
			{ ix=0; }

		pv[2]=(pv[2]&(~0x00FF000000ULL))|(ix<<24);
		i--;
	}
#endif
	
	return(0);
}

int TK_VMem_SetHybAvlPageTableEntry(s64 ttb, u64 teal, u64 teah, u64 ptval)
{
	u64 *ptp, *ptp1;
	u64 ptd, teal2;
	int ix;
	
	teal2=teal&0xFFFFFFFFFE000000ULL;
	ix=(teal>>TKMM_PAGEBITS)&((1<<(TKMM_PAGEBITS-3))-1);
	
	ptd=TK_VMem_GetAvlPageTableEntry(ttb, teal2, teah);
	if(!ptd)
	{
//		ptp1=TKMM_PageAllocZero(1<<TKMM_PAGEBITS);
		ptp1=TKMM_PageAlloc(1<<TKMM_PAGEBITS);
		ptd=TK_VMem_PtoForPtr(ptp1)|1;
		TK_VMem_SetAvlPageTableEntry(ttb, teal2, teah, ptd);
	}

	ptp=TK_VMem_PtrForPto(ptd);
	ptp[ix]=ptval;
	return(0);
}

u64 TK_VMem_GetHybAvlPageTableEntry(s64 ttb, u64 teal, u64 teah)
{
	u64 *ptp, *ptp1;
	u64 pte, ptd, teal2;
	int ix;
	
	teal2=teal&0xFFFFFFFFFE000000ULL;
	ix=(teal>>TKMM_PAGEBITS)&((1<<(TKMM_PAGEBITS-3))-1);
	
	ptd=TK_VMem_GetAvlPageTableEntry(ttb, teal2, teah);
	if(!ptd)
		return(0);

	ptp=TK_VMem_PtrForPto(ptd);
	pte=ptp[ix];
	return(pte);
}
