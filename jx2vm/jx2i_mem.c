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
	int ra;
	ra=addr-sp->addr_base;
	BJX2_PtrSetByteOfsLe(sp->data, ra, val);
	return(0);
}

int BJX2_MemRamCb_SetWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int val)
{
	int ra;
	ra=addr-sp->addr_base;
	BJX2_PtrSetWordOfsLe(sp->data, ra, val);
	return(0);
}

int BJX2_MemRamCb_SetDWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s32 val)
{
	int ra;
	ra=addr-sp->addr_base;
	BJX2_PtrSetDWordOfsLe(sp->data, ra, val);
	return(0);
}

int BJX2_MemRamCb_SetQWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s64 val)
{
	int ra;
	ra=addr-sp->addr_base;
	BJX2_PtrSetQWordOfsLe(sp->data, ra, val);
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
	
	sp=BJX2_MemAllocSpan(ctx);
	
	sp->name=name;
	sp->addr_base=base;
	sp->addr_lim=lim;
	sp->data=malloc((lim-base)+8);
	
	sp->GetByte=BJX2_MemRamCb_GetByte;
	sp->GetWord=BJX2_MemRamCb_GetWord;
	sp->GetDWord=BJX2_MemRamCb_GetDWord;
	sp->GetQWord=BJX2_MemRamCb_GetQWord;
	
	sp->SetByte=BJX2_MemRamCb_SetByte;
	sp->SetWord=BJX2_MemRamCb_SetWord;
	sp->SetDWord=BJX2_MemRamCb_SetDWord;
	sp->SetQWord=BJX2_MemRamCb_SetQWord;
	
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
	sp->data=malloc((lim-base)+8);
	
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

BJX2_MemSpan *BJX2_MemSpanForAddr(BJX2_Context *ctx, bjx2_addr addr)
{
	BJX2_MemSpan *sp;
	int nc, nb, nr;

	sp=ctx->span_pr0;
	if(sp && (addr>=sp->addr_base) && (addr<=sp->addr_lim))
		return(sp);

	sp=ctx->span_pr1;
	if(sp && (addr>=sp->addr_base) && (addr<=sp->addr_lim))
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

int BJX2_MemGetByte(BJX2_Context *ctx, bjx2_addr addr)
{
	BJX2_MemSpan *sp;
	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		return(0);
	}
	return(sp->GetByte(ctx, sp, addr));
}

int BJX2_MemGetWord(BJX2_Context *ctx, bjx2_addr addr)
{
	BJX2_MemSpan *sp;
	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		return(0);
	}
	return(sp->GetWord(ctx, sp, addr));
}

s32 BJX2_MemGetDWord(BJX2_Context *ctx, bjx2_addr addr)
{
	BJX2_MemSpan *sp;
	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		return(0);
	}
	return(sp->GetDWord(ctx, sp, addr));
}

s64 BJX2_MemGetQWord(BJX2_Context *ctx, bjx2_addr addr)
{
	BJX2_MemSpan *sp;
	s64 t;
	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		return(0);
	}
	t=sp->GetQWord(ctx, sp, addr);
//	t=sp->GetDWord(ctx, sp, addr+4);
//	t=(t<<32)|((u32)(sp->GetDWord(ctx, sp, addr+0)));
	return(t);
}

int BJX2_MemSetByte(BJX2_Context *ctx, bjx2_addr addr, int val)
{
	BJX2_MemSpan *sp;
	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		return(0);
	}
	return(sp->SetByte(ctx, sp, addr, val));
}

int BJX2_MemSetWord(BJX2_Context *ctx, bjx2_addr addr, int val)
{
	BJX2_MemSpan *sp;
	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		return(0);
	}
	return(sp->SetWord(ctx, sp, addr, val));
}

int BJX2_MemSetDWord(BJX2_Context *ctx, bjx2_addr addr, s32 val)
{
	BJX2_MemSpan *sp;
	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		return(0);
	}
	return(sp->SetDWord(ctx, sp, addr, val));
}

int BJX2_MemSetQWord(BJX2_Context *ctx, bjx2_addr addr, s64 val)
{
	BJX2_MemSpan *sp;
	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		return(0);
	}
	sp->SetQWord(ctx, sp, addr, val);
//	sp->SetDWord(ctx, sp, addr+4, val>>32);
//	return(sp->SetDWord(ctx, sp, addr, val));
	return(0);
}
