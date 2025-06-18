byte *x3vm_hostpagedat[1024];
u64 x3vm_hostpagemap[1024];
int x3vm_n_hostpage;

byte *x3vm_gblcelldat[1024];
u64 x3vm_gblcellmap[1024][64];
int x3vm_n_gblpage;

byte *x3vm_gblbulkdat[1024];
int x3vm_gblbulksz[1024];
int x3vm_n_gblbulk;

int X3VM_ValidatePointerGlobal(void *ptr)
{
	byte *ptrb;
	int i;
	
	ptrb=ptr;
	for(i=0; i<x3vm_n_gblbulk; i++)
	{
		if(	(ptrb>=x3vm_gblbulkdat[i]) &&
			(ptrb<(x3vm_gblbulkdat[i]+x3vm_gblbulksz[i])))
				return(1);
	}

	for(i=0; i<x3vm_n_gblpage; i++)
	{
		if(	(ptrb>=x3vm_gblcelldat[i]) &&
			(ptrb<(x3vm_gblcelldat[i]+(64<<14))))
				return(1);
	}
	
	return(0);
}

void *X3VM_AllocGlobalBulkData(X3VM_Context *ctx, int sz)
{
	byte *ptr;
	u64 bv;
	int csz;
	int i, j, k;
	
	csz=(sz+16383)>>14;
	
	if(csz>4096)
	{
		__debugbreak();
	}

	i=x3vm_n_gblbulk++;
	ptr=malloc((csz<<14)+16383);
	k=((long long)ptr)&16383;
	if(k)
		ptr+=16384-k;
	x3vm_gblbulkdat[i]=ptr;
	x3vm_gblbulksz[i]=sz;

	memset(ptr, 0, csz<<14);

	for(j=0; j<csz; j++)
	{
		bv=(u64)(ptr+(j<<14));
		X3VM_StorePteForAddr(ctx, bv, bv);
	}
	
	printf("X3VM_AllocGlobalBulkData: %p..%p\n", ptr, ptr+sz);
	
	return(ptr);
}

void *X3VM_AllocGlobalData(X3VM_Context *ctx, int sz)
{
	byte *ptr;
	u64 bv;
	int csz;
	int i, j, k;
	
	if(sz>=(1<<18))
	{
		return(X3VM_AllocGlobalBulkData(ctx, sz));
	}
	
	csz=(sz+255)>>8;
	if(!csz)
		return(NULL);

	for(i=0; i<x3vm_n_gblpage; i++)
	{
		k=-1;
		for(j=0; j<4096; j++)
		{
			bv=x3vm_gblcellmap[i][j>>6];
			if((bv>>(j&63))&1)
				{ k=j; continue; }
			if((j-k)>=csz)
				break;
		}
		if(j>=4096)
			continue;

		k++;
		ptr=x3vm_gblcelldat[i]+(k<<8);
		
		for(j=k; j<(k+csz); j++)
			{ x3vm_gblcellmap[i][j>>6]|=1ULL<<(j&63); }
		memset(ptr, 0, csz<<8);
		return(ptr);
	}
	
	i=x3vm_n_gblpage++;
	ptr=malloc(64*(1<<14)+16383);
	k=((long long)ptr)&16383;
	if(k)
		ptr+=16384-k;
	x3vm_gblcelldat[i]=ptr;
	memset(x3vm_gblcellmap[i], 0, 64*8);

	memset(ptr, 0, 64<<14);

	printf("X3VM_AllocGlobalData: %p..%p\n", ptr, ptr+(64<<14));

	for(j=0; j<64; j++)
	{
		bv=(u64)(ptr+(j<<14));
		X3VM_StorePteForAddr(ctx, bv, bv);
	}
	
	ptr=X3VM_AllocGlobalData(ctx, sz);
	return(ptr);
}

u16 X3VM_GetU16LE(void *ptr)
{
	return(*(u16 *)ptr);
}

u32 X3VM_GetU32LE(void *ptr)
{
	return(*(u32 *)ptr);
}

u64 X3VM_GetU64LE(void *ptr)
{
	return(*(u64 *)ptr);
}

void X3VM_SetU16LE(void *ptr, u16 val)
{
	*((u16 *)ptr)=val;
}

void X3VM_SetU32LE(void *ptr, u32 val)
{
	*((u32 *)ptr)=val;
}

void X3VM_SetU64LE(void *ptr, u64 val)
{
	*((u64 *)ptr)=val;
}

void *X3VM_AllocHostPage(X3VM_Context *ctx)
{
	byte *ptr;
	u64 bv;
	int i, j, k;
	
	for(i=0; i<x3vm_n_hostpage; i++)
	{
		bv=x3vm_hostpagemap[i];
		if(bv==(~0ULL))
			continue;
		for(j=0; j<64; j++)
			if(!((bv>>j)&1))
				break;
		if(j<64)
		{
			ptr=x3vm_hostpagedat[i]+(j<<14);
			bv|=1ULL<<j;
			x3vm_hostpagemap[i]=bv;
			memset(ptr, 0, 1<<14);
			return(ptr);
		}
	}
	
	i=x3vm_n_hostpage++;
	ptr=malloc(64*(1<<14)+255);
	k=((long long)ptr)&255;
	if(k)
		ptr+=256-k;
	x3vm_hostpagedat[i]=ptr;
	x3vm_hostpagemap[i]=0;
	ptr=X3VM_AllocHostPage(ctx);
	return(ptr);
}

u64 X3VM_LoadPteForAddr(X3VM_Context *ctx, u64 addr)
{
	u64 *pgp;
	byte *pgpb;
	u64 pgt;
	u32 pga, pba;
	int i0, i1, i2, h;

	pga=(u32)(addr>>14);
	pba=addr&16383;
	h=pga&4095;
	
	i2=(pga>> 0)&2047;
	i1=(pga>>11)&2047;
	i0=(pga>>22)&2047;
	
	pgt=ctx->pagetab;
	if(ctx->vas_parent)
		pgt=ctx->vas_parent->pagetab;

	if(!pgt)
		return(0);
	pgp=(u64 *)pgt;
	
	pgt=pgp[i0];
	if(!pgt)
		return(0);
	pgp=(u64 *)pgt;
	
	pgt=pgp[i1];
	if(!pgt)
		return(0);
	pgp=(u64 *)pgt;
	
	pgt=pgp[i2];
	if(!pgt)
		return(0);

	return(pgt);
}

int X3VM_StorePteForAddr(X3VM_Context *ctx, u64 addr, u64 pteval)
{
	u64 *pgp;
	byte *pgpb;
	u64 pgt;
	u32 pga, pba;
	int i0, i1, i2, h;

	pga=(u32)(addr>>14);
	pba=addr&16383;
	h=pga&4095;
	
	i2=(pga>> 0)&2047;
	i1=(pga>>11)&2047;
	i0=(pga>>22)&2047;
	
	pgt=ctx->pagetab;
	if(ctx->vas_parent)
		pgt=ctx->vas_parent->pagetab;

	if(!pgt)
	{
		pgpb=X3VM_AllocHostPage(ctx);
		pgt=(u64)pgpb;
		if(ctx->vas_parent)
			ctx->vas_parent->pagetab=pgt;
		else
			ctx->pagetab=pgt;
	}
	pgp=(u64 *)pgt;
	
	pgt=pgp[i0];
	if(!pgt)
	{
		pgpb=X3VM_AllocHostPage(ctx);
		pgt=(u64)pgpb;
		pgp[i0]=pgt;
	}
	pgp=(u64 *)pgt;
	
	pgt=pgp[i1];
	if(!pgt)
	{
		pgpb=X3VM_AllocHostPage(ctx);
		pgt=(u64)pgpb;
		pgp[i1]=pgt;
	}
	pgp=(u64 *)pgt;
	
	pgp[i2]=pteval;
	return(0);
}

int X3VM_QueryAddressRange(X3VM_Context *ctx, u64 base, u64 size)
{
	u32 spage, epage, pg;
	u64 pteval;
	int fl;

	spage=base>>14;
	epage=(base+size)>>14;
	fl=0;
	
	for(pg=spage; pg<=epage; pg++)
	{
		pteval=X3VM_LoadPteForAddr(ctx, ((u64)pg)<<14);
		if(pteval)
			fl|=1;
	}
	return(fl);
}

int X3VM_CommitAddressRange(X3VM_Context *ctx, u64 base, u64 size)
{
	void *pgpb;
	u32 spage, epage, pg;
	u64 pteval;
	int fl;

	spage=base>>14;
	epage=(base+size)>>14;
	fl=0;

	/* try to pre-alloc page table space if needed. */
	pteval=X3VM_LoadPteForAddr(ctx, ((u64)spage)<<14);
	X3VM_StorePteForAddr(ctx, ((u64)spage)<<14, pteval);
	pteval=X3VM_LoadPteForAddr(ctx, ((u64)epage)<<14);
	X3VM_StorePteForAddr(ctx, ((u64)epage)<<14, pteval);
	
	for(pg=spage; pg<=epage; pg++)
	{
		pteval=X3VM_LoadPteForAddr(ctx, ((u64)pg)<<14);
		if(!pteval)
		{
			pgpb=X3VM_AllocHostPage(ctx);
			pteval=(u64)pgpb;
			X3VM_StorePteForAddr(ctx, ((u64)pg)<<14, pteval);
		}
	}
	return(fl);
}

u64 X3VM_GenAslrAddr(X3VM_Context *ctx)
{
	u64 v1;
	u32 v0;

	v0=X3VM_RngRand32(ctx);
	v1=((u64)v0)<<14;
	return(v1);
}

u64 X3VM_GenAslrAddrSanitize(X3VM_Context *ctx, u64 sz)
{
	u64 base;
	int isok;

	base=X3VM_GenAslrAddr(ctx);
	if(ctx->aslr_useblock)
	{
		base=(base&((1ULL<<36)-1))|
			(((u64)(ctx->aslr_useblock))<<36);
	}
	
	while(1)
	{
		isok=1;

#ifdef _WIN32
		if(base<(1ULL<<40))
			isok=0;
#else
		if(base<(1ULL<<36))
			isok=0;
#endif

		if((sz<=(1ULL<<26)) && ((base>>32)!=((base+sz)>>32)))
			isok=0;

		if(isok)
			break;

		base=X3VM_GenAslrAddr(ctx);
		if(ctx->aslr_useblock)
		{
			base=(base&((1ULL<<36)-1))|
				(((u64)(ctx->aslr_useblock))<<36);
		}
	}
	return(base);
}

u64 X3VM_VirtAllocPages(X3VM_Context *ctx, u64 size)
{
	u64 base;
	int n;

	n=0;
//	base=X3VM_GenAslrAddr(ctx);
	base=X3VM_GenAslrAddrSanitize(ctx, size);
	while(X3VM_QueryAddressRange(ctx, base, size)&1)
	{
		if(n>=6)
			ctx->aslr_useblock=0;
		n++;
//		base=X3VM_GenAslrAddr(ctx);
		base=X3VM_GenAslrAddrSanitize(ctx, size);
	}
	
	ctx->aslr_useblock=base>>36;
	
	X3VM_CommitAddressRange(ctx, base, size);
	return(base);
}

static byte x3vm_mem_dummy[64];

void *X3VM_MemMapAddress(X3VM_Context *ctx, u64 addr, int fl)
{
	u64 *pgp;
	byte *pgpb;
	u64 pgt;
	u32 pga, pba;
	int i0, i1, i2, h;

	pga=(u32)(addr>>14);
	pba=addr&16383;
//	h=pga&4095;
//	h=pga&2047;
//	h=pga&1023;
	h=pga&(X3VM_MEMTLB_SIZE-1);

#if 0
	if(ctx->mem_spgp_pgn==pga)
	{
		pgpb=ctx->mem_spgp_ptr;
		return((void *)(pgpb+pba));
	}
#endif
	
	if(ctx->tlb_vpgaddr0[h]==pga)
	{
		pgpb=ctx->tlb_vpgptr0[h];
//		ctx->mem_spgp_pgn=pga;
//		ctx->mem_spgp_ptr=pgpb;
		return((void *)(pgpb+pba));
	}

	if(ctx->tlb_vpgaddr1[h]==pga)
	{
		pgpb=ctx->tlb_vpgptr1[h];
		ctx->tlb_vpgptr1[h]=ctx->tlb_vpgptr0[h];
		ctx->tlb_vpgaddr1[h]=ctx->tlb_vpgaddr0[h];
		ctx->tlb_vpgptr0[h]=pgpb;
		ctx->tlb_vpgaddr0[h]=pga;
//		ctx->mem_spgp_pgn=pga;
//		ctx->mem_spgp_ptr=pgpb;
		return((void *)(pgpb+pba));
	}

#if (X3VM_MEMTLB_WAY>=3)
	if(ctx->tlb_vpgaddr2[h]==pga)
	{
		pgpb=ctx->tlb_vpgptr2[h];
		ctx->tlb_vpgptr2[h]=ctx->tlb_vpgptr1[h];
		ctx->tlb_vpgaddr2[h]=ctx->tlb_vpgaddr1[h];
		ctx->tlb_vpgptr1[h]=pgpb;
		ctx->tlb_vpgaddr1[h]=pga;
		return((void *)(pgpb+pba));
	}
#endif

#if (X3VM_MEMTLB_WAY>=4)
	if(ctx->tlb_vpgaddr3[h]==pga)
	{
		pgpb=ctx->tlb_vpgptr3[h];
		ctx->tlb_vpgptr3[h]=ctx->tlb_vpgptr2[h];
		ctx->tlb_vpgaddr3[h]=ctx->tlb_vpgaddr2[h];
		ctx->tlb_vpgptr2[h]=pgpb;
		ctx->tlb_vpgaddr2[h]=pga;
		return((void *)(pgpb+pba));
	}
#endif

#if 1
	i2=(pga>> 0)&2047;
	i1=(pga>>11)&2047;
	i0=(pga>>22)&2047;
	
	pgt=ctx->pagetab;
	if(ctx->vas_parent)
		pgt=ctx->vas_parent->pagetab;
	if(!pgt)
		goto miss;
//		return(NULL);
	pgp=(u64 *)pgt;
	
	pgt=pgp[i0];
	if(!pgt)
		goto miss;
//		return(NULL);
	pgp=(u64 *)pgt;
	
	pgt=pgp[i1];
	if(!pgt)
		goto miss;
//		return(NULL);
	pgp=(u64 *)pgt;
	
	pgt=pgp[i2];
	if(!pgt)
		goto miss;
//		return(NULL);
#endif

#if 0
	pgt=X3VM_LoadPteForAddr(ctx, addr);
	if(!pgt)
		goto miss;
#endif

	pgpb=(byte *)pgt;

#if (X3VM_MEMTLB_WAY>=4)
	ctx->tlb_vpgptr3[h]=ctx->tlb_vpgptr2[h];
	ctx->tlb_vpgaddr3[h]=ctx->tlb_vpgaddr2[h];
#endif
#if (X3VM_MEMTLB_WAY>=3)
	ctx->tlb_vpgptr2[h]=ctx->tlb_vpgptr1[h];
	ctx->tlb_vpgaddr2[h]=ctx->tlb_vpgaddr1[h];
#endif
	ctx->tlb_vpgptr1[h]=ctx->tlb_vpgptr0[h];
	ctx->tlb_vpgaddr1[h]=ctx->tlb_vpgaddr0[h];
	ctx->tlb_vpgptr0[h]=pgpb;
	ctx->tlb_vpgaddr0[h]=pga;

	return((void *)(pgpb+pba));
	
	miss:

	if(fl&4)
	{
		X3VM_ThrowMemFault(ctx, X3VM_FAULT_MEMACC, addr);
		return(x3vm_mem_dummy);
	}

	return(NULL);
}

int X3VM_MemLoadS8(X3VM_Context *ctx, u64 addr)
{
	void *ptr;
	ptr=X3VM_MemMapAddress(ctx, addr, 4);
	return(*(sbyte *)ptr);
}

int X3VM_MemLoadU8(X3VM_Context *ctx, u64 addr)
{
	void *ptr;
	ptr=X3VM_MemMapAddress(ctx, addr, 4);
	return(*(byte *)ptr);
}

u16 X3VM_MemLoadU16(X3VM_Context *ctx, u64 addr)
{
	void *ptr;
	int i0, i1;

	if(addr&1)
	{
		if((addr>>14)!=((addr+1)>>14))
		{
			i0=X3VM_MemLoadU8(ctx, addr+0);
			i1=X3VM_MemLoadU8(ctx, addr+1);
			return(i0|(i1<<8));
		}
	}
	
	ptr=X3VM_MemMapAddress(ctx, addr, 4);
	return(*(u16 *)ptr);
}


int X3VM_MemLoadS16(X3VM_Context *ctx, u64 addr)
{
	return((s16)X3VM_MemLoadU16(ctx, addr));
}

u32 X3VM_MemLoadU32(X3VM_Context *ctx, u64 addr)
{
	void *ptr;
	u32 i0, i1;

	if(addr&3)
	{
		if((addr>>14)!=((addr+3)>>14))
		{
			i0=X3VM_MemLoadU16(ctx, addr+0);
			i1=X3VM_MemLoadU16(ctx, addr+2);
			return(i0|(i1<<16));
		}
	}
	
	ptr=X3VM_MemMapAddress(ctx, addr, 4);
	return(*(u32 *)ptr);
}


s32 X3VM_MemLoadS32(X3VM_Context *ctx, u64 addr)
{
	return((s32)X3VM_MemLoadU32(ctx, addr));
}

u64 X3VM_MemLoadU64(X3VM_Context *ctx, u64 addr)
{
	void *ptr;
	u64 i0, i1;

	if(addr&7)
	{
		if((addr>>14)!=((addr+7)>>14))
		{
			i0=X3VM_MemLoadU32(ctx, addr+0);
			i1=X3VM_MemLoadU32(ctx, addr+4);
			return(i0|(i1<<32));
		}
	}
	
	ptr=X3VM_MemMapAddress(ctx, addr, 4);
	return(*(u64 *)ptr);
}

s64 X3VM_MemLoadS64(X3VM_Context *ctx, u64 addr)
{
	return((s64)X3VM_MemLoadU64(ctx, addr));
}

int X3VM_MemLoad64P(X3VM_Context *ctx, u64 addr, u64 *rval)
{
	void *ptr;
	byte *pgpb;
	u32 pga, pba;
	u64 i0, i1;

	if((addr>>14)!=((addr+15)>>14))
	{
		rval[0]=X3VM_MemLoadU64(ctx, addr+0);
		rval[1]=X3VM_MemLoadU64(ctx, addr+8);
		return(0);
	}

#if 0
	pga=(u32)(addr>>14);
	pba=addr&16383;
	if(ctx->mem_spgp_pgn==pga)
	{
		pgpb=ctx->mem_spgp_ptr;
		ptr=(pgpb+pba);
		rval[0]=((u64 *)ptr)[0];
		rval[1]=((u64 *)ptr)[1];
		return(0);
	}
	pgpb=X3VM_MemMapAddress(ctx, ((u64)pga)<<14, 1);
#if 1
	if(!pgpb)
	{
//		ctx->reg[X3VM_REG_SPC]=ctx->reg[X3VM_REG_BPC];
//		ctx->reg[X3VM_REG_TEA]=addr;
//		X3VM_ThrowFault(ctx, X3VM_FAULT_MEMACC);
		X3VM_ThrowMemFault(ctx, X3VM_FAULT_MEMACC, addr);
		return(0);
	}
#endif
	ctx->mem_spgp_pgn=pga;
	ctx->mem_spgp_ptr=pgpb;
	ptr=(pgpb+pba);
	rval[0]=((u64 *)ptr)[0];
	rval[1]=((u64 *)ptr)[1];
	return(0);
#endif

#if 1
	ptr=X3VM_MemMapAddress(ctx, addr, 4);
	rval[0]=((u64 *)ptr)[0];
	rval[1]=((u64 *)ptr)[1];
	return(0);
#endif
}

int X3VM_MemStore64P(X3VM_Context *ctx, u64 addr, u64 *rval)
{
	void *ptr;
	byte *pgpb;
	u32 pga, pba;
	u64 i0, i1;

	if((addr>>14)!=((addr+15)>>14))
	{
		X3VM_MemStoreU64(ctx, addr+0, rval[0]);
		X3VM_MemStoreU64(ctx, addr+8, rval[1]);
		return(0);
	}

#if 0
	pga=(u32)(addr>>14);
	pba=addr&16383;
	if(ctx->mem_spgp_pgn==pga)
	{
		pgpb=ctx->mem_spgp_ptr;
		ptr=(pgpb+pba);
		((u64 *)ptr)[0]=rval[0];
		((u64 *)ptr)[1]=rval[1];
		return(0);
	}
	pgpb=X3VM_MemMapAddress(ctx, ((u64)pga)<<14, 1);
	if(!pgpb)
	{
//		ctx->reg[X3VM_REG_SPC]=ctx->reg[X3VM_REG_BPC];
//		ctx->reg[X3VM_REG_TEA]=addr;
//		X3VM_ThrowFault(ctx, X3VM_FAULT_MEMACC);
		X3VM_ThrowMemFault(ctx, X3VM_FAULT_MEMACC, addr);
		return(0);
	}
	ctx->mem_spgp_pgn=pga;
	ctx->mem_spgp_ptr=pgpb;
	ptr=(pgpb+pba);
	((u64 *)ptr)[0]=rval[0];
	((u64 *)ptr)[1]=rval[1];
	return(0);
#endif

#if 1
	ptr=X3VM_MemMapAddress(ctx, addr, 5);
	((u64 *)ptr)[0]=rval[0];
	((u64 *)ptr)[1]=rval[1];
	return(0);
#endif
}


int X3VM_MemStoreU8(X3VM_Context *ctx, u64 addr, byte val)
{
	void *ptr;
	ptr=X3VM_MemMapAddress(ctx, addr, 5);
	*(byte *)ptr=val;
	return(1);
}

int X3VM_MemStoreU16(X3VM_Context *ctx, u64 addr, u16 val)
{
	void *ptr;

	if(addr&1)
	{
		if((addr>>14)!=((addr+1)>>14))
		{
			X3VM_MemStoreU8(ctx, addr+0, val);
			X3VM_MemStoreU8(ctx, addr+1, val>>8);
			return(1);
		}
	}
	
	ptr=X3VM_MemMapAddress(ctx, addr, 5);
	*(u16 *)ptr=val;
	return(1);
}

int X3VM_MemStoreU32(X3VM_Context *ctx, u64 addr, u32 val)
{
	void *ptr;

	if(addr&3)
	{
		if((addr>>14)!=((addr+3)>>14))
		{
			X3VM_MemStoreU16(ctx, addr+0, val);
			X3VM_MemStoreU16(ctx, addr+2, val>>16);
			return(1);
		}
	}
	
	ptr=X3VM_MemMapAddress(ctx, addr, 5);
	*(u32 *)ptr=val;
	return(1);
}

int X3VM_MemStoreU64(X3VM_Context *ctx, u64 addr, u64 val)
{
	void *ptr;

	if(addr&7)
	{
		if((addr>>14)!=((addr+7)>>14))
		{
			X3VM_MemStoreU32(ctx, addr+0, val);
			X3VM_MemStoreU32(ctx, addr+4, val>>32);
			return(1);
		}
	}
	
	ptr=X3VM_MemMapAddress(ctx, addr, 5);
	*(u64 *)ptr=val;
	return(1);
}



int X3VM_VMemcpyIn(X3VM_Context *ctx, x3vm_addr dst, void *src, int sz)
{
	x3vm_addr ct;
	byte *cs;
	u64 v;
	int n;

	ct=dst;
	cs=src;
	n=sz;
	while(n>=8)
	{
		v=X3VM_GetU64LE(cs);
		X3VM_MemStoreU64(ctx, ct, v);
		cs+=8; ct+=8; n-=8;
	}
	while(n>=2)
	{
		v=X3VM_GetU16LE(cs);
		X3VM_MemStoreU16(ctx, ct, v);
		cs+=2; ct+=2; n-=2;
	}
	if(n>0)
	{
		X3VM_MemStoreU8(ctx, ct, *cs);
	}

	return(0);
}

int X3VM_VMemcpyOut(X3VM_Context *ctx, void *dst, x3vm_addr src, int sz)
{
	x3vm_addr cs;
	byte *ct;
	u64 v;
	int n;

	ct=dst;
	cs=src;
	n=sz;
	while(n>=8)
	{
		v=X3VM_MemLoadU64(ctx, cs);
		X3VM_SetU64LE(ct, v);
		cs+=8; ct+=8; n-=8;
	}
	while(n>=2)
	{
		v=X3VM_MemLoadU16(ctx, cs);
		X3VM_SetU16LE(ct, v);
		cs+=2; ct+=2; n-=2;
	}
	if(n>0)
	{
		*ct=X3VM_MemLoadU8(ctx, cs);
	}

	return(0);
}

int X3VM_VMemset(X3VM_Context *ctx, x3vm_addr dst, int val, int sz)
{
	x3vm_addr cs;
	x3vm_addr ct;
	u64 v;
	int n, d;

	ct=dst;
	n=sz;

	v=val&255;
	v=v|(v<<8);
	v=v|(v<<16);
	v=v|(v<<32);
	
	while(n>=8)
	{
		X3VM_MemStoreU64(ctx, ct, v);
		ct+=8; n-=8;
	}
	while(n>0)
	{
		X3VM_MemStoreU8(ctx, ct, v);
		ct++; n--;
	}
	return(0);
}

int X3VM_VMemcpy(X3VM_Context *ctx, x3vm_addr dst, x3vm_addr src, int sz)
{
	x3vm_addr cs;
	x3vm_addr ct;
	u64 v;
	int n, d;

	d=dst-src;
	if((d>=0) && (d<8) && (d<sz))
//	if((d>=0) && (d<8))
//	if(1)
	{
		if(d==0)
		{
			/* copy in place is no-op */
			return(0);
		}

		/* self-overlapping, assume LZ copy semantics. */
		ct=dst;
		cs=src;
		n=sz;

#if 0
		if(d==1)
		{
			/* but, 1 byte does a fill. */
			v=X3VM_MemLoadU8(ctx, cs);
			v=v|(v<<8);
			v=v|(v<<16);
			v=v|(v<<32);
			
			while(n>=8)
			{
				X3VM_MemStoreU64(ctx, ct, v);
				cs+=8; ct+=8; n-=8;
			}
			while(n>0)
			{
				X3VM_MemStoreU8(ctx, ct, v);
				cs++; ct++; n--;
			}
			return(0);
		}
#endif

		while(n>0)
		{
			v=X3VM_MemLoadU8(ctx, cs);
			X3VM_MemStoreU8(ctx, ct, v);
			cs++; ct++; n--;
		}
		return(0);
	}

	ct=dst;
	cs=src;
	n=sz;
	while(n>=8)
	{
		v=X3VM_MemLoadU64(ctx, cs);
		X3VM_MemStoreU64(ctx, ct, v);
		cs+=8; ct+=8; n-=8;
	}
#if 1
	while(n>=2)
	{
		v=X3VM_MemLoadU16(ctx, cs);
		X3VM_MemStoreU16(ctx, ct, v);
		cs+=2; ct+=2; n-=2;
	}
#endif
//	if(n>0)
	while(n>0)
	{
		v=X3VM_MemLoadU8(ctx, cs);
		X3VM_MemStoreU8(ctx, ct, v);
		cs++; ct++; n--;
	}

	return(0);
}
