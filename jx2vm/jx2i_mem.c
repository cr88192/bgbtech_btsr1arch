int BJX2_MemRamCb_GetByte(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	int ra;
	ra=addr-sp->addr_base;
	return(BJX2_PtrGetSByteOfsLe(sp->data, ra));
}

int BJX2_MemRamCb_GetWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	int ra;
	ra=addr-sp->addr_base;
	return(BJX2_PtrGetSWordOfsLe(sp->data, ra));
}

s32 BJX2_MemRamCb_GetDWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	int ra;
	ra=addr-sp->addr_base;
	return(BJX2_PtrGetSDWordOfsLe(sp->data, ra));
}

s64 BJX2_MemRamCb_GetQWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	int ra;
	ra=addr-sp->addr_base;
	return(BJX2_PtrGetSQWordOfsLe(sp->data, ra));
}


int BJX2_MemRamCb_SetByte(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int val)
{
	int ra, ra4;
	ra=addr-sp->addr_base;
	BJX2_PtrSetByteOfsLe(sp->data, ra, val);

	ra4=(ra>>4);
	if(sp->tripwire[ra4>>3]&(1<<(ra4&7)))
		{ BJX2_MemRamCb_SetFault(ctx, sp, addr, val); }
	return(0);
}

int BJX2_MemRamCb_SetWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int val)
{
	int ra, ra4;
	ra=addr-sp->addr_base;
	BJX2_PtrSetWordOfsLe(sp->data, ra, val);

	ra4=(ra>>4);
	if(sp->tripwire[ra4>>3]&(1<<(ra4&7)))
		{ BJX2_MemRamCb_SetFault(ctx, sp, addr, val); }
	return(0);
}

int BJX2_MemRamCb_SetDWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s32 val)
{
	int ra, ra4;
	ra=addr-sp->addr_base;
	BJX2_PtrSetDWordOfsLe(sp->data, ra, val);

	ra4=(ra>>4);
	if(sp->tripwire[ra4>>3]&(1<<(ra4&7)))
		{ BJX2_MemRamCb_SetFault(ctx, sp, addr, val); }
	return(0);
}

int BJX2_MemRamCb_SetQWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s64 val)
{
	int ra, ra4;
	ra=addr-sp->addr_base;
	BJX2_PtrSetQWordOfsLe(sp->data, ra, val);

	ra4=(ra>>4);
	if(sp->tripwire[ra4>>3]&(1<<(ra4&7)))
		{ BJX2_MemRamCb_SetFault(ctx, sp, addr, val); }
	return(0);
}

int BJX2_MemRamCb_SetFault(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s32 val)
{
	ctx->regs[BJX2_REG_PC]=ctx->trapc;
	ctx->regs[BJX2_REG_TEA]=addr;
	BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INV_MWR);
	return(0);
}

int BJX2_MemRamCb_SetFaultQW(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s64 val)
{
	ctx->regs[BJX2_REG_PC]=ctx->trapc;
	ctx->regs[BJX2_REG_TEA]=addr;
	BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INV_MWR);
	return(0);
}

int BJX2_MemRamCb_SetTripwire(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int mode)
{
	int ra, ra4;
	ra=addr-sp->addr_base;
//	BJX2_PtrSetQWordOfsLe(sp->data, ra, val);

	ra4=(ra>>4);
//	if(sp->tripwire[ra4>>3]&(1<<(ra4&7)))
//		{ BJX2_MemRamCb_SetFault(ctx, sp, addr, val); }

	if(mode==3)
	{
		sp->tripwire[ra4>>3]|=(1<<(ra4&7));
	}else if(mode==2)
	{
		sp->tripwire[ra4>>3]&=~(1<<(ra4&7));
	}

	return(0);
}

BJX2_MemSpan *BJX2_MemAllocSpan(BJX2_Context *ctx)
{
	BJX2_MemSpan *sp;
	
	sp=malloc(sizeof(BJX2_MemSpan));
	memset(sp, 0, sizeof(BJX2_MemSpan));
	return(sp);
}

int BJX2_MemDefineRAM(BJX2_Context *ctx,
	char *name, bjx2_addr base, bjx2_addr lim)
{
	BJX2_MemSpan *sp;
	int nblk;
	
	nblk=((lim-base)+15)/16;
	
	sp=BJX2_MemAllocSpan(ctx);
	
	sp->name=name;
	sp->addr_base=base;
	sp->addr_lim=lim;
	sp->addr_sz=lim-base;
//	sp->data=malloc((lim-base)+8);
	sp->data=malloc(nblk*16+8);
	sp->tripwire=malloc((nblk+31)/8);
	sp->simple_mem=3;
	
	memset(sp->tripwire, 0, nblk/8);
	
	sp->GetByte=BJX2_MemRamCb_GetByte;
	sp->GetWord=BJX2_MemRamCb_GetWord;
	sp->GetDWord=BJX2_MemRamCb_GetDWord;
	sp->GetQWord=BJX2_MemRamCb_GetQWord;
	
	sp->SetByte=BJX2_MemRamCb_SetByte;
	sp->SetWord=BJX2_MemRamCb_SetWord;
	sp->SetDWord=BJX2_MemRamCb_SetDWord;
	sp->SetQWord=BJX2_MemRamCb_SetQWord;

	sp->SetTripwire=BJX2_MemRamCb_SetTripwire;
	
	BJX2_MemAddSpan(ctx, sp);
	return(0);
}

int BJX2_MemDefineROM(BJX2_Context *ctx,
	char *name, bjx2_addr base, bjx2_addr lim)
{
	BJX2_MemSpan *sp;
	
	sp=BJX2_MemAllocSpan(ctx);
	
	sp->name=name;
	sp->addr_base=base;
	sp->addr_lim=lim;
	sp->addr_sz=lim-base;
	sp->data=malloc((lim-base)+8);
	sp->simple_mem=1;
	
	sp->GetByte=BJX2_MemRamCb_GetByte;
	sp->GetWord=BJX2_MemRamCb_GetWord;
	sp->GetDWord=BJX2_MemRamCb_GetDWord;
	sp->GetQWord=BJX2_MemRamCb_GetQWord;
	
	sp->SetByte=BJX2_MemRamCb_SetFault;
	sp->SetWord=BJX2_MemRamCb_SetFault;
	sp->SetDWord=BJX2_MemRamCb_SetFault;
	sp->SetQWord=BJX2_MemRamCb_SetFaultQW;
	
	BJX2_MemAddSpan(ctx, sp);
	return(0);
}

int BJX2_MemAddSpan(BJX2_Context *ctx, BJX2_MemSpan *sp)
{
	BJX2_MemSpan *sp1;
	int i, j;

	i=ctx->n_span++;
	while(i>0)
	{
		sp1=ctx->span[i-1];
		ctx->span[i]=sp1;
		if(sp1->addr_base<=sp->addr_base)
			break;
		i--;
	}
	ctx->span[i]=sp;
	return(0);
}

BJX2_MemSpan *BJX2_MemSpanForName(BJX2_Context *ctx, char *name)
{
	BJX2_MemSpan *sp;
	int nc, nb, nr;

	nb=0; nr=ctx->n_span;
	
	for(nc=0; nc<nr; nc++)
	{
		sp=ctx->span[nc];
		if(sp->name && !strcmp(sp->name, name))
			return(sp);
	}
	return(NULL);
}

BJX2_MemSpan *BJX2_MemSpanForAddr64(BJX2_Context *ctx, bjx2_addr addr)
{
	BJX2_MemSpan *sp;
	int nc, nb, nr;

	sp=ctx->span_pr0;
//	if(sp && (addr>=sp->addr_base) && (addr<=sp->addr_lim))
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
		return(sp);

	sp=ctx->span_pr1;
//	if(sp && (addr>=sp->addr_base) && (addr<=sp->addr_lim))
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		ctx->span_pr1=ctx->span_pr0;
		ctx->span_pr0=sp;
		return(sp);
	}

	nb=0; nr=ctx->n_span;
	
	if(nr<8)
	{
		for(nc=0; nc<nr; nc++)
		{
			sp=ctx->span[nc];
			if(addr<=sp->addr_lim)
			{
				if(addr<sp->addr_base)
					break;

				ctx->span_pr1=ctx->span_pr0;
				ctx->span_pr0=sp;
				return(sp);
			}
		}
		return(NULL);
	}
	
	while(nr>1)
	{
		nc=nb+(nr>>1);
		sp=ctx->span[nc];
		if(addr>=sp->addr_base)
		{
			nb=nc;
			nr-=(nr>>1);
		}else
		{
			nr=nr>>1;
		}
	}

	sp=ctx->span[nb];
	if((addr>=sp->addr_base) && (addr<=sp->addr_lim))
	{
		ctx->span_pr1=ctx->span_pr0;
		ctx->span_pr0=sp;
		return(sp);
	}
	
	return(NULL);
}

BJX2_MemSpan *BJX2_MemSpanForAddr32(BJX2_Context *ctx, bjx2_addr addr)
{
	return(BJX2_MemSpanForAddr64(ctx, (u32)addr));
}

BJX2_MemSpan *BJX2_MemSpanForAddr(BJX2_Context *ctx, bjx2_addr addr)
{
	return(ctx->MemSpanForAddr(ctx, addr));

//	if(ctx->MemSpanForAddr)
//		return(ctx->MemSpanForAddr(ctx, addr));
	
//	if(ctx->regs[BJX2_REG_SR]&0x80000000)
//		ctx->MemSpanForAddr=BJX2_MemSpanForAddr64;
//	else
//		ctx->MemSpanForAddr=BJX2_MemSpanForAddr32;
//	return(ctx->MemSpanForAddr(ctx, addr));
}

int BJX2_MemSimAddrL1I(BJX2_Context *ctx, bjx2_addr addr)
{
#if 1
	bjx2_addr t0, t1;
	int h, p1, p2;
	
	if(ctx->use_jit)
		return(0);
	
	if(((addr>>28)&0xF)==0xF)
		return(0);
	
//	h=(addr>>4)&63;
//	h=(addr>>5)&63;
	h=(addr>>5)&255;
//	h=(addr>>5)&511;

//	ctx->mem_cyc+=3;
//	ctx->mem_cyc+=2;
	ctx->mem_cnt_l1i++;

//	if((addr>>4)==ctx->mem_l1ih4k[(h<<2)|0])
//		return(0);
	if((addr>>5)==ctx->mem_l1ih4k[(h<<2)|0])
		return(0);

	p1=5;
	p2=24;

	ctx->miss_cyc+=p1;
	ctx->miss_cyc_l1+=p1;
	ctx->miss_cnt_l1i++;

	if(ctx->tr_cur)
		ctx->tr_cur->acc_pencyc+=p1;

//	ctx->mem_l1h4k[(h<<2)|3]=ctx->mem_l1h4k[(h<<2)|2];
//	ctx->mem_l1h4k[(h<<2)|2]=ctx->mem_l1h4k[(h<<2)|1];
//	ctx->mem_l1h4k[(h<<2)|1]=ctx->mem_l1h4k[(h<<2)|0];

//	ctx->mem_l1ih4k[(h<<2)|0]=addr>>4;
	ctx->mem_l1ih4k[(h<<2)|0]=addr>>5;


	/* L2 Cache */

	ctx->mem_cnt_l2++;

//	h=(addr>>4)&1023;
//	h=(addr>>4)&2047;
	h=(addr>>4)&4095;
//	h=(addr>>4)&8191;

	if((addr>>4)==ctx->mem_l2h32k[(h<<1)|0])
		return(0);

#if 0
	if((addr>>4)==ctx->mem_l2h32k[(h<<1)|1])
	{
		t0=ctx->mem_l2h32k[(h<<1)|0];
		t1=ctx->mem_l2h32k[(h<<1)|1];
		ctx->mem_l2h32k[(h<<1)|0]=t1;
		ctx->mem_l2h32k[(h<<1)|1]=t0;
		return(0);
	}
#endif

	ctx->mem_l2h32k[(h<<1)|1]=ctx->mem_l2h32k[(h<<1)|0];
	ctx->mem_l2h32k[(h<<1)|0]=addr>>4;

	/* Main RAM */

	ctx->miss_cyc+=p2;
	ctx->miss_cyc_l2+=p2;
	ctx->miss_cnt_l2++;
	ctx->mem_cnt_dri++;

	if(ctx->tr_cur)
		ctx->tr_cur->acc_pencyc+=p2;
#endif

	return(1);
}


int BJX2_MemSimAddrL1(BJX2_Context *ctx, bjx2_addr addr)
{
#if 1
	bjx2_addr t0, t1;
	int h, p1, p2;
	
	if(ctx->use_jit)
		return(0);
	
	if(((addr>>28)&0xF)==0xF)
	{
		p1=7+15;
		p2=15;

		ctx->mem_cyc+=2;

		ctx->miss_cyc+=p1;
		ctx->mem_cyc_mmio+=p1;
	
//		ctx->miss_cyc+=p1;
//		ctx->miss_cyc_l1+=p1;
//		ctx->miss_cnt_l1++;

//		ctx->miss_cyc+=p2;
//		ctx->miss_cyc_l2+=p2;
//		ctx->miss_cnt_l2++;
//		ctx->mem_cnt_drd++;

		if(ctx->tr_cur)
			ctx->tr_cur->acc_pencyc+=p1;

		return(0);
	}
	
//	h=(addr>>4)&63;
//	h=(addr>>5)&63;
//	h=(addr>>5)&255;
	h=(addr>>5)&511;

//	ctx->mem_cyc+=3;
	ctx->mem_cyc+=2;
	ctx->mem_cnt_l1++;

#if 0
	if((addr>>4)==ctx->mem_l1h4k[(h<<2)|0])
		return(0);
	if((addr>>4)==ctx->mem_l1h4k[(h<<2)|1])
		return(0);
	if((addr>>4)==ctx->mem_l1h4k[(h<<2)|2])
		return(0);
	if((addr>>4)==ctx->mem_l1h4k[(h<<2)|3])
		return(0);
#endif

//	if((addr>>4)==ctx->mem_l1h4k[(h<<2)|0])
//		return(0);

	if((addr>>5)==ctx->mem_l1h4k[(h<<2)|0])
		return(0);

#if 0
	if((addr>>4)==ctx->mem_l1h4k[(h<<2)|1])
	{
		t0=ctx->mem_l1h4k[(h<<2)|0];
		t1=ctx->mem_l1h4k[(h<<2)|1];
		ctx->mem_l1h4k[(h<<2)|0]=t1;
		ctx->mem_l1h4k[(h<<2)|1]=t0;
		return(0);
	}
#endif
	
#if 0
	if((addr>>4)==ctx->mem_l1h4k[(h<<2)|2])
	{
		t0=ctx->mem_l1h4k[(h<<2)|1];
		t1=ctx->mem_l1h4k[(h<<2)|2];
		ctx->mem_l1h4k[(h<<2)|1]=t1;
		ctx->mem_l1h4k[(h<<2)|2]=t0;
		return(0);
	}
	if((addr>>4)==ctx->mem_l1h4k[(h<<2)|3])
	{
		t0=ctx->mem_l1h4k[(h<<2)|2];
		t1=ctx->mem_l1h4k[(h<<2)|3];
		ctx->mem_l1h4k[(h<<2)|2]=t1;
		ctx->mem_l1h4k[(h<<2)|3]=t0;
		return(0);
	}
#endif

//	p1=5;
//	p2=24;

//	p1=7;
//	p2=36;

	p1=9;
	p2=45;

	ctx->miss_cyc+=p1;
	ctx->miss_cyc_l1+=p1;
	ctx->miss_cnt_l1++;

	if(ctx->tr_cur)
		ctx->tr_cur->acc_pencyc+=p1;

//	ctx->mem_l1h4k[(h<<2)|3]=ctx->mem_l1h4k[(h<<2)|2];
//	ctx->mem_l1h4k[(h<<2)|2]=ctx->mem_l1h4k[(h<<2)|1];
//	ctx->mem_l1h4k[(h<<2)|1]=ctx->mem_l1h4k[(h<<2)|0];
//	ctx->mem_l1h4k[(h<<2)|0]=addr>>4;

	ctx->mem_l1h4k[(h<<2)|0]=addr>>5;

	/* L2 Cache */

	ctx->mem_cnt_l2++;

//	h=(addr>>4)&1023;
//	h=(addr>>4)&2047;
	h=(addr>>4)&4095;
//	h=(addr>>4)&8191;

	if((addr>>4)==ctx->mem_l2h32k[(h<<1)|0])
		return(0);

#if 0
	if((addr>>4)==ctx->mem_l2h32k[(h<<1)|1])
	{
		t0=ctx->mem_l2h32k[(h<<1)|0];
		t1=ctx->mem_l2h32k[(h<<1)|1];
		ctx->mem_l2h32k[(h<<1)|0]=t1;
		ctx->mem_l2h32k[(h<<1)|1]=t0;
		return(0);
	}
#endif

	ctx->mem_l2h32k[(h<<1)|1]=ctx->mem_l2h32k[(h<<1)|0];
	ctx->mem_l2h32k[(h<<1)|0]=addr>>4;

	/* Main RAM */

	ctx->miss_cyc+=p2;
	ctx->miss_cyc_l2+=p2;
	ctx->miss_cnt_l2++;
	ctx->mem_cnt_drd++;

	if(ctx->tr_cur)
		ctx->tr_cur->acc_pencyc+=p2;

#endif

#if 0

#if 0
	if((addr>>3)==ctx->mem_l1addr1)
		return(0);
	if((addr>>3)==ctx->mem_l1addr2)
		return(0);
	if((addr>>3)==ctx->mem_l1addr3)
		return(0);
	if((addr>>3)==ctx->mem_l1addr4)
		return(0);
#endif

#if 1
	if((addr>>4)==ctx->mem_l1addr1)
		return(0);
	if((addr>>4)==ctx->mem_l1addr2)
		return(0);
	if((addr>>4)==ctx->mem_l1addr3)
		return(0);
	if((addr>>4)==ctx->mem_l1addr4)
		return(0);
#endif

	ctx->miss_cyc+=3;
	if(ctx->tr_cur)
		ctx->tr_cur->acc_pencyc+=3;

	ctx->mem_l1addr4=ctx->mem_l1addr3;
	ctx->mem_l1addr3=ctx->mem_l1addr2;
	ctx->mem_l1addr2=ctx->mem_l1addr1;
//	ctx->mem_l1addr1=addr>>3;
	ctx->mem_l1addr1=addr>>4;
#endif

	return(1);
}

bjx2_addr BJX2_MemTranslateTlb(BJX2_Context *ctx, bjx2_addr addr)
{
	bjx2_addr addr1;
	u64 tlbhi2, tlblo2;
	u64 tlbhi0, tlblo0;
	u64 tlbhi1, tlblo1;
	int i, j, k;

	if(!(ctx->regs[BJX2_REG_MMCR]&1))
		return(addr);

	
	k=(addr>>28)&0xFFFFFFFE;
	if((k>=0x00008) && (k<=0x0000F))
	{
		if(k==0x8)
		{
			return(BJX2_MemTranslateTlb(ctx,
//				(-1LL<<32)|(addr&0x1FFFFFFF)));
				(~((1LL<<32)-1))|(addr&0x1FFFFFFF)));
		}
		if(k==0xA)
			return(addr);
		if(k==0xC)
			return(addr&0x1FFFFFFF);
		if(k==0xE)
			return(addr);
	}

	tlbhi0=ctx->mem_tlb_pr0_hi;
	tlblo0=ctx->mem_tlb_pr0_lo;
	if(	(tlbhi0&0x0000FFFFFFFFF000ULL) ==
		(addr  &0x0000FFFFFFFFF000ULL))
	{
		addr1=(tlblo0&0x0000FFFFFFFFF000ULL)|(addr&0x0FFFULL);
		return(addr1);
	}

	tlbhi1=ctx->mem_tlb_pr1_hi;
	tlblo1=ctx->mem_tlb_pr1_lo;
	if(	(tlbhi1&0x0000FFFFFFFFF000ULL) ==
		(addr  &0x0000FFFFFFFFF000ULL))
	{
		ctx->mem_tlb_pr0_hi=tlbhi1;
		ctx->mem_tlb_pr0_lo=tlblo1;
		ctx->mem_tlb_pr1_hi=tlbhi0;
		ctx->mem_tlb_pr1_lo=tlblo0;
		addr1=(tlblo1&0x0000FFFFFFFFF000ULL)|(addr&0x0FFFULL);
		return(addr1);
	}

	for(i=0; i<64; i++)
	{
		tlbhi2=ctx->mem_tlb_hi[i];
		tlblo2=ctx->mem_tlb_lo[i];

		if(	(tlbhi2&0x0000FFFFFFFFF000ULL) ==
			(addr  &0x0000FFFFFFFFF000ULL))
		{
			ctx->mem_tlb_pr0_hi=tlbhi2;
			ctx->mem_tlb_pr0_lo=tlblo2;
			ctx->mem_tlb_pr1_hi=tlbhi0;
			ctx->mem_tlb_pr1_lo=tlblo0;
			addr1=(tlblo2&0x0000FFFFFFFFF000ULL)|(addr&0x0FFFULL);
			return(addr1);
		}
	}

	ctx->regs[BJX2_REG_PC]=ctx->trapc;
	ctx->regs[BJX2_REG_TEA]=addr;
	BJX2_ThrowFaultStatus(ctx, BJX2_FLT_TLBMISS);
	return(0);

//	return(addr);
}

int BJX2_MemGetByte_Dfl(BJX2_Context *ctx, bjx2_addr addr0)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra;

	addr=BJX2_MemTranslateTlb(ctx, addr0);

	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		BJX2_MemSimAddrL1(ctx, addr);

		if(sp->simple_mem)
		{
			ra=addr-sp->addr_base;
			return(BJX2_PtrGetSByteOfsLe(sp->data, ra));
		}
		return(sp->GetByte(ctx, sp, addr));
	}

	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		return(0);
	}
	
	BJX2_MemSimAddrL1(ctx, addr);

#if 1
	if(sp->simple_mem)
	{
		if(!(ctx->regs[BJX2_REG_MMCR]&1))
		{
			ctx->psp_pbase=sp->addr_base;
			ctx->psp_prng3=sp->addr_sz-3;
			ctx->psp_pdata=sp->data;
		}

		ra=addr-sp->addr_base;
		return(BJX2_PtrGetSByteOfsLe(sp->data, ra));
	}
#endif

	return(sp->GetByte(ctx, sp, addr));
}

int BJX2_MemGetWord_Dfl(BJX2_Context *ctx, bjx2_addr addr0)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra;

	addr=BJX2_MemTranslateTlb(ctx, addr0);

	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		BJX2_MemSimAddrL1(ctx, addr);

		if(sp->simple_mem)
		{
			ra=addr-sp->addr_base;
			return(BJX2_PtrGetSWordOfsLe(sp->data, ra));
		}
		return(sp->GetWord(ctx, sp, addr));
	}

	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		return(0);
	}

	BJX2_MemSimAddrL1(ctx, addr);

#if 1
	if(sp->simple_mem)
	{
		if(!(ctx->regs[BJX2_REG_MMCR]&1))
		{
			ctx->psp_pbase=sp->addr_base;
			ctx->psp_prng3=sp->addr_sz-3;
			ctx->psp_pdata=sp->data;
		}

		ra=addr-sp->addr_base;
		return(BJX2_PtrGetSWordOfsLe(sp->data, ra));
	}
#endif

	return(sp->GetWord(ctx, sp, addr));
}

s32 BJX2_MemGetDWord_Dfl(BJX2_Context *ctx, bjx2_addr addr0)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra;

	addr=BJX2_MemTranslateTlb(ctx, addr0);

	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		BJX2_MemSimAddrL1(ctx, addr);

		if(sp->simple_mem)
		{
			ra=addr-sp->addr_base;
			return(BJX2_PtrGetSDWordOfsLe(sp->data, ra));
		}
		return(sp->GetDWord(ctx, sp, addr));
	}

	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		return(0);
	}

	BJX2_MemSimAddrL1(ctx, addr);

#if 1
	if(sp->simple_mem)
	{
		if(!(ctx->regs[BJX2_REG_MMCR]&1))
		{
			ctx->psp_pbase=sp->addr_base;
			ctx->psp_prng3=sp->addr_sz-3;
			ctx->psp_pdata=sp->data;
		}

		ra=addr-sp->addr_base;
		return(BJX2_PtrGetSDWordOfsLe(sp->data, ra));
	}
#endif

	return(sp->GetDWord(ctx, sp, addr));
}

s64 BJX2_MemGetQWord_Dfl(BJX2_Context *ctx, bjx2_addr addr0)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	s64 t;
	int ra;

	addr=BJX2_MemTranslateTlb(ctx, addr0);

	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		BJX2_MemSimAddrL1(ctx, addr);

		if(sp->simple_mem)
		{
			ra=addr-sp->addr_base;
			return(BJX2_PtrGetSQWordOfsLe(sp->data, ra));
		}
		return(sp->GetDWord(ctx, sp, addr));
	}

	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		return(0);
	}

	BJX2_MemSimAddrL1(ctx, addr);

#if 1
	if(sp->simple_mem)
	{
		if(!(ctx->regs[BJX2_REG_MMCR]&1))
		{
			ctx->psp_pbase=sp->addr_base;
			ctx->psp_prng3=sp->addr_sz-3;
			ctx->psp_pdata=sp->data;
		}

		ra=addr-sp->addr_base;
		return(BJX2_PtrGetSQWordOfsLe(sp->data, ra));
	}
#endif

	t=sp->GetQWord(ctx, sp, addr);
//	t=sp->GetDWord(ctx, sp, addr+4);
//	t=(t<<32)|((u32)(sp->GetDWord(ctx, sp, addr+0)));
	return(t);
}


int BJX2_MemGetByte_NoAT(BJX2_Context *ctx, bjx2_addr addr0)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra;

	addr=(u32)addr0;
	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		if(sp->simple_mem)
		{
			ra=addr-sp->addr_base;
			return(BJX2_PtrGetSByteOfsLe(sp->data, ra));
		}
		return(sp->GetByte(ctx, sp, addr));
	}

//	sp=BJX2_MemSpanForAddr(ctx, addr);
	sp=ctx->MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		return(0);
	}
	
	if(sp->simple_mem)
	{
//		if(!(ctx->regs[BJX2_REG_MMCR]&1))
		if(1)
		{
			ctx->psp_pbase=sp->addr_base;
			ctx->psp_prng3=sp->addr_sz-3;
			ctx->psp_pdata=sp->data;
		}

		ra=addr-sp->addr_base;
		return(BJX2_PtrGetSByteOfsLe(sp->data, ra));
	}

	return(sp->GetByte(ctx, sp, addr));
}

int BJX2_MemGetWord_NoAT(BJX2_Context *ctx, bjx2_addr addr0)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra;

	addr=(u32)addr0;
	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		if(sp->simple_mem)
		{
			ra=addr-sp->addr_base;
			return(BJX2_PtrGetSWordOfsLe(sp->data, ra));
		}
		return(sp->GetWord(ctx, sp, addr));
	}

//	sp=BJX2_MemSpanForAddr(ctx, addr);
	sp=ctx->MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		return(0);
	}

	if(sp->simple_mem)
	{
//		if(!(ctx->regs[BJX2_REG_MMCR]&1))
		if(1)
		{
			ctx->psp_pbase=sp->addr_base;
			ctx->psp_prng3=sp->addr_sz-3;
			ctx->psp_pdata=sp->data;
		}

		ra=addr-sp->addr_base;
		return(BJX2_PtrGetSWordOfsLe(sp->data, ra));
	}

	return(sp->GetWord(ctx, sp, addr));
}

s32 BJX2_MemGetDWord_NoAT(BJX2_Context *ctx, bjx2_addr addr0)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra;

	addr=(u32)addr0;
	sp=ctx->span_pr0;
	if(sp && (((u32)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		if(sp->simple_mem)
		{
			ra=addr-sp->addr_base;
			return(BJX2_PtrGetSDWordOfsLe(sp->data, ra));
		}
		return(sp->GetDWord(ctx, sp, addr));
	}

//	sp=BJX2_MemSpanForAddr(ctx, addr);
	sp=ctx->MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		return(0);
	}

	if(sp->simple_mem)
	{
//		if(!(ctx->regs[BJX2_REG_MMCR]&1))
		if(1)
		{
			ctx->psp_pbase=sp->addr_base;
			ctx->psp_prng3=sp->addr_sz-3;
			ctx->psp_pdata=sp->data;
		}

		ra=addr-sp->addr_base;
		return(BJX2_PtrGetSDWordOfsLe(sp->data, ra));
	}

	return(sp->GetDWord(ctx, sp, addr));
}

s64 BJX2_MemGetQWord_NoAT(BJX2_Context *ctx, bjx2_addr addr0)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	s64 t;
	int ra;

	addr=(u32)addr0;

	sp=ctx->span_pr0;
	if(sp && (((u32)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		if(sp->simple_mem)
		{
			ra=addr-sp->addr_base;
			return(BJX2_PtrGetSQWordOfsLe(sp->data, ra));
		}
		return(sp->GetDWord(ctx, sp, addr));
	}

//	sp=BJX2_MemSpanForAddr(ctx, addr);
	sp=ctx->MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		return(0);
	}

#if 1
	if(sp->simple_mem)
	{
//		if(!(ctx->regs[BJX2_REG_MMCR]&1))
		if(1)
		{
			ctx->psp_pbase=sp->addr_base;
			ctx->psp_prng3=sp->addr_sz-3;
			ctx->psp_pdata=sp->data;
		}

		ra=addr-sp->addr_base;
		return(BJX2_PtrGetSQWordOfsLe(sp->data, ra));
	}
#endif

	t=sp->GetQWord(ctx, sp, addr);
//	t=sp->GetDWord(ctx, sp, addr+4);
//	t=(t<<32)|((u32)(sp->GetDWord(ctx, sp, addr+0)));
	return(t);
}


int BJX2_MemSetByte_Dfl(BJX2_Context *ctx, bjx2_addr addr0, int val)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra, ra4;

	addr=BJX2_MemTranslateTlb(ctx, addr0);

	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		BJX2_MemSimAddrL1(ctx, addr);

		if(sp->simple_mem&2)
		{
			ra=addr-sp->addr_base;
			BJX2_PtrSetByteOfsLe(sp->data, ra, val);

			ra4=(ra>>4);
			if(sp->tripwire[ra4>>3]&(1<<(ra4&7)))
				{ BJX2_MemRamCb_SetFault(ctx, sp, addr, val); }
			return(0);
		}
		return(sp->SetByte(ctx, sp, addr, val));
	}

	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		return(0);
	}

	BJX2_MemSimAddrL1(ctx, addr);

	return(sp->SetByte(ctx, sp, addr, val));
}

int BJX2_MemSetWord_Dfl(BJX2_Context *ctx, bjx2_addr addr0, int val)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra, ra4;

	addr=BJX2_MemTranslateTlb(ctx, addr0);

	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		BJX2_MemSimAddrL1(ctx, addr);

		if(sp->simple_mem&2)
		{
			ra=addr-sp->addr_base;
			BJX2_PtrSetWordOfsLe(sp->data, ra, val);

			ra4=(ra>>4);
			if(sp->tripwire[ra4>>3]&(1<<(ra4&7)))
				{ BJX2_MemRamCb_SetFault(ctx, sp, addr, val); }
			return(0);
		}
		return(sp->SetWord(ctx, sp, addr, val));
	}

	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		return(0);
	}

	BJX2_MemSimAddrL1(ctx, addr);

#if 1
	if(sp->simple_mem&2)
	{
		ra=addr-sp->addr_base;
		BJX2_PtrSetWordOfsLe(sp->data, ra, val);
		return(0);
	}
#endif

	return(sp->SetWord(ctx, sp, addr, val));
}

int BJX2_MemSetDWord_Dfl(BJX2_Context *ctx, bjx2_addr addr0, s32 val)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra, ra4;

	addr=BJX2_MemTranslateTlb(ctx, addr0);

	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		BJX2_MemSimAddrL1(ctx, addr);

		if(sp->simple_mem&2)
		{
			ra=addr-sp->addr_base;
			BJX2_PtrSetDWordOfsLe(sp->data, ra, val);

			ra4=(ra>>4);
			if(sp->tripwire[ra4>>3]&(1<<(ra4&7)))
				{ BJX2_MemRamCb_SetFault(ctx, sp, addr, val); }
			return(0);
		}
		return(sp->SetDWord(ctx, sp, addr, val));
	}

	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		return(0);
	}

	BJX2_MemSimAddrL1(ctx, addr);

#if 1
	if(sp->simple_mem&2)
	{
		ra=addr-sp->addr_base;
		BJX2_PtrSetDWordOfsLe(sp->data, ra, val);
		return(0);
	}
#endif

	return(sp->SetDWord(ctx, sp, addr, val));
}

int BJX2_MemSetQWord_Dfl(BJX2_Context *ctx, bjx2_addr addr0, s64 val)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra, ra4;

	addr=BJX2_MemTranslateTlb(ctx, addr0);

	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		BJX2_MemSimAddrL1(ctx, addr);

		if(sp->simple_mem&2)
		{
			ra=addr-sp->addr_base;
			BJX2_PtrSetQWordOfsLe(sp->data, ra, val);

			ra4=(ra>>4);
			if(sp->tripwire[ra4>>3]&(1<<(ra4&7)))
				{ BJX2_MemRamCb_SetFault(ctx, sp, addr, val); }
			return(0);
		}
		return(sp->SetQWord(ctx, sp, addr, val));
	}

	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		return(0);
	}

	BJX2_MemSimAddrL1(ctx, addr);

#if 1
	if(sp->simple_mem&2)
	{
		ra=addr-sp->addr_base;
		BJX2_PtrSetQWordOfsLe(sp->data, ra, val);
		return(0);
	}
#endif

	sp->SetQWord(ctx, sp, addr, val);
//	sp->SetDWord(ctx, sp, addr+4, val>>32);
//	return(sp->SetDWord(ctx, sp, addr, val));
	return(0);
}


int BJX2_MemSetDWord_NoAT(BJX2_Context *ctx, bjx2_addr addr0, s32 val)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra;

	addr=(u32)addr0;
	sp=ctx->span_pr0;
	if(sp && (((u32)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		if(sp->simple_mem&2)
		{
			ra=addr-sp->addr_base;
			BJX2_PtrSetDWordOfsLe(sp->data, ra, val);
			return(0);
		}
		return(sp->SetDWord(ctx, sp, addr, val));
	}

//	sp=BJX2_MemSpanForAddr(ctx, addr);
	sp=ctx->MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		return(0);
	}

	if(sp->simple_mem&2)
	{
		ra=addr-sp->addr_base;
		BJX2_PtrSetDWordOfsLe(sp->data, ra, val);
		return(0);
	}

	return(sp->SetDWord(ctx, sp, addr, val));
}

int BJX2_MemSetQWord_NoAT(BJX2_Context *ctx, bjx2_addr addr0, s64 val)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra;

	addr=(u32)addr0;

	sp=ctx->span_pr0;
	if(sp && (((u32)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		if(sp->simple_mem&2)
		{
			ra=addr-sp->addr_base;
			BJX2_PtrSetQWordOfsLe(sp->data, ra, val);
			return(0);
		}
		return(sp->SetQWord(ctx, sp, addr, val));
	}

//	sp=BJX2_MemSpanForAddr(ctx, addr);
	sp=ctx->MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		return(0);
	}

#if 1
	if(sp->simple_mem&2)
	{
		ra=addr-sp->addr_base;
		BJX2_PtrSetQWordOfsLe(sp->data, ra, val);
		return(0);
	}
#endif

	return(sp->SetQWord(ctx, sp, addr, val));
}


int BJX2_MemSetTripwire_Dfl(BJX2_Context *ctx, bjx2_addr addr0, int val)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra;

	addr=BJX2_MemTranslateTlb(ctx, addr0);

	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		BJX2_MemSimAddrL1(ctx, addr);

//		if(sp->simple_mem&2)
//		{
//			ra=addr-sp->addr_base;
//			BJX2_PtrSetByteOfsLe(sp->data, ra, val);
//			return(0);
//		}
		
		if(!sp->SetTripwire)
			return(0);
		return(sp->SetTripwire(ctx, sp, addr, val));
	}

	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		return(0);
	}

	BJX2_MemSimAddrL1(ctx, addr);

	if(!sp->SetTripwire)
		return(0);
	return(sp->SetTripwire(ctx, sp, addr, val));
}


int BJX2_MemSetupState(BJX2_Context *ctx)
{
	if(ctx->MemGetByte)
		return(0);

	ctx->MemGetByte=BJX2_MemGetByte_Dfl;
	ctx->MemGetWord=BJX2_MemGetWord_Dfl;
	ctx->MemGetDWord=BJX2_MemGetDWord_Dfl;
	ctx->MemGetQWord=BJX2_MemGetQWord_Dfl;
	ctx->MemSetByte=BJX2_MemSetByte_Dfl;
	ctx->MemSetWord=BJX2_MemSetWord_Dfl;
	ctx->MemSetDWord=BJX2_MemSetDWord_Dfl;
	ctx->MemSetQWord=BJX2_MemSetQWord_Dfl;
//	ctx->MemSpanForAddr=BJX2_MemSpanForAddr64;

	ctx->MemSetTripwire=BJX2_MemSetTripwire_Dfl;

	if(ctx->regs[BJX2_REG_SR]&0x80000000)
		ctx->MemSpanForAddr=BJX2_MemSpanForAddr64;
	else
		ctx->MemSpanForAddr=BJX2_MemSpanForAddr32;

	if(!(ctx->regs[BJX2_REG_MMCR]&1) && ctx->use_jit)
	{
		if(!(ctx->regs[BJX2_REG_SR]&0x80000000))
		{
			ctx->MemGetByte=BJX2_MemGetByte_NoAT;
			ctx->MemGetWord=BJX2_MemGetWord_NoAT;
			ctx->MemGetDWord=BJX2_MemGetDWord_NoAT;
			ctx->MemGetQWord=BJX2_MemGetQWord_NoAT;
			ctx->MemSetDWord=BJX2_MemSetDWord_NoAT;
			ctx->MemSetQWord=BJX2_MemSetQWord_NoAT;
			ctx->MemSpanForAddr=BJX2_MemSpanForAddr32;
		}
	}

	return(1);
}

int BJX2_MemGetByte(BJX2_Context *ctx, bjx2_addr addr0)
{
//	if(ctx->MemGetByte)
//		return(ctx->MemGetByte(ctx, addr0));
//	ctx->MemGetByte=BJX2_MemGetByte_Dfl;
	return(ctx->MemGetByte(ctx, addr0));
}

int BJX2_MemGetWord(BJX2_Context *ctx, bjx2_addr addr0)
{
//	if(ctx->MemGetWord)
//		return(ctx->MemGetWord(ctx, addr0));
//	ctx->MemGetWord=BJX2_MemGetWord_Dfl;
	return(ctx->MemGetWord(ctx, addr0));
}

s32 BJX2_MemGetDWord(BJX2_Context *ctx, bjx2_addr addr0)
{
//	if(ctx->MemGetDWord)
//		return(ctx->MemGetDWord(ctx, addr0));
//	ctx->MemGetDWord=BJX2_MemGetDWord_Dfl;
	return(ctx->MemGetDWord(ctx, addr0));
}

s64 BJX2_MemGetQWord(BJX2_Context *ctx, bjx2_addr addr0)
{
//	if(ctx->MemGetQWord)
//		return(ctx->MemGetQWord(ctx, addr0));
//	ctx->MemGetQWord=BJX2_MemGetQWord_Dfl;
	return(ctx->MemGetQWord(ctx, addr0));
}

int BJX2_MemSetByte(BJX2_Context *ctx, bjx2_addr addr0, int val)
{
//	if(ctx->MemSetByte)
//		return(ctx->MemSetByte(ctx, addr0, val));
//	ctx->MemSetByte=BJX2_MemSetByte_Dfl;
	return(ctx->MemSetByte(ctx, addr0, val));
}

int BJX2_MemSetWord(BJX2_Context *ctx, bjx2_addr addr0, int val)
{
//	if(ctx->MemSetWord)
//		return(ctx->MemSetWord(ctx, addr0, val));
//	ctx->MemSetWord=BJX2_MemSetWord_Dfl;
	return(ctx->MemSetWord(ctx, addr0, val));
}

int BJX2_MemSetDWord(BJX2_Context *ctx, bjx2_addr addr0, s32 val)
{
//	if(ctx->MemSetDWord)
//		return(ctx->MemSetDWord(ctx, addr0, val));
//	ctx->MemSetDWord=BJX2_MemSetDWord_Dfl;
	return(ctx->MemSetDWord(ctx, addr0, val));
}

int BJX2_MemSetQWord(BJX2_Context *ctx, bjx2_addr addr0, s64 val)
{
//	if(ctx->MemSetQWord)
//		return(ctx->MemSetQWord(ctx, addr0, val));
//	ctx->MemSetQWord=BJX2_MemSetQWord_Dfl;
	return(ctx->MemSetQWord(ctx, addr0, val));
}

int BJX2_MemSetTrip(BJX2_Context *ctx, bjx2_addr addr0, int val)
{
	return(ctx->MemSetTripwire(ctx, addr0, val));
}
