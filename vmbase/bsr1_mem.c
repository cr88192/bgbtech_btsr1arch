int BTSR1_MemRamCb_GetByte(BTSR1_Context *ctx,
	BTSR1_MemSpan *sp, btsr1_addr addr)
{
	int ra;
	ra=addr-sp->addr_base;
	return(BTSR1_PtrGetSByteOfsLe(sp->data, ra));
}

int BTSR1_MemRamCb_GetWord(BTSR1_Context *ctx,
	BTSR1_MemSpan *sp, btsr1_addr addr)
{
	int ra;
	ra=addr-sp->addr_base;
	return(BTSR1_PtrGetSWordOfsLe(sp->data, ra));
}

s32 BTSR1_MemRamCb_GetDWord(BTSR1_Context *ctx,
	BTSR1_MemSpan *sp, btsr1_addr addr)
{
	int ra;
	ra=addr-sp->addr_base;
	return(BTSR1_PtrGetSDWordOfsLe(sp->data, ra));
}


int BTSR1_MemRamCb_SetByte(BTSR1_Context *ctx,
	BTSR1_MemSpan *sp, btsr1_addr addr, int val)
{
	int ra;
	ra=addr-sp->addr_base;
	BTSR1_PtrSetByteOfsLe(sp->data, ra, val);
	return(0);
}

int BTSR1_MemRamCb_SetWord(BTSR1_Context *ctx,
	BTSR1_MemSpan *sp, btsr1_addr addr, int val)
{
	int ra;
	ra=addr-sp->addr_base;
	BTSR1_PtrSetWordOfsLe(sp->data, ra, val);
	return(0);
}

int BTSR1_MemRamCb_SetDWord(BTSR1_Context *ctx,
	BTSR1_MemSpan *sp, btsr1_addr addr, s32 val)
{
	int ra;
	ra=addr-sp->addr_base;
	BTSR1_PtrSetDWordOfsLe(sp->data, ra, val);
	return(0);
}

int BTSR1_MemRamCb_SetFault(BTSR1_Context *ctx,
	BTSR1_MemSpan *sp, btsr1_addr addr, s32 val)
{
	ctx->regs[BTSR1_REG_TEA]=addr;
	BTSR1_ThrowFaultStatus(ctx, BTSR1_FLT_INV_MWR);
	return(0);
}

BTSR1_MemSpan *BTSR1_MemAllocSpan(BTSR1_Context *ctx)
{
	BTSR1_MemSpan *sp;
	
	sp=malloc(sizeof(BTSR1_MemSpan));
	memset(sp, 0, sizeof(BTSR1_MemSpan));
	return(sp);
}

int BTSR1_MemDefineRAM(BTSR1_Context *ctx,
	char *name, btsr1_addr base, btsr1_addr lim)
{
	BTSR1_MemSpan *sp;
	
	sp=BTSR1_MemAllocSpan(ctx);
	
	sp->name=name;
	sp->addr_base=base;
	sp->addr_lim=lim;
	sp->data=malloc((lim-base)+8);
	
	sp->GetByte=BTSR1_MemRamCb_GetByte;
	sp->GetWord=BTSR1_MemRamCb_GetWord;
	sp->GetDWord=BTSR1_MemRamCb_GetDWord;
	
	sp->SetByte=BTSR1_MemRamCb_SetByte;
	sp->SetWord=BTSR1_MemRamCb_SetWord;
	sp->SetDWord=BTSR1_MemRamCb_SetDWord;
	
	BTSR1_MemAddSpan(ctx, sp);
	return(0);
}

int BTSR1_MemDefineROM(BTSR1_Context *ctx,
	char *name, btsr1_addr base, btsr1_addr lim)
{
	BTSR1_MemSpan *sp;
	
	sp=BTSR1_MemAllocSpan(ctx);
	
	sp->name=name;
	sp->addr_base=base;
	sp->addr_lim=lim;
	sp->data=malloc((lim-base)+8);
	
	sp->GetByte=BTSR1_MemRamCb_GetByte;
	sp->GetWord=BTSR1_MemRamCb_GetWord;
	sp->GetDWord=BTSR1_MemRamCb_GetDWord;
	
	sp->SetByte=BTSR1_MemRamCb_SetFault;
	sp->SetWord=BTSR1_MemRamCb_SetFault;
	sp->SetDWord=BTSR1_MemRamCb_SetFault;
	
	BTSR1_MemAddSpan(ctx, sp);
	return(0);
}

int BTSR1_MemAddSpan(BTSR1_Context *ctx, BTSR1_MemSpan *sp)
{
	BTSR1_MemSpan *sp1;
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

BTSR1_MemSpan *BTSR1_MemSpanForName(BTSR1_Context *ctx, char *name)
{
	BTSR1_MemSpan *sp;
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

BTSR1_MemSpan *BTSR1_MemSpanForAddr(BTSR1_Context *ctx, btsr1_addr addr)
{
	BTSR1_MemSpan *sp;
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

int BTSR1_MemGetByte(BTSR1_Context *ctx, btsr1_addr addr)
{
	BTSR1_MemSpan *sp;
	sp=BTSR1_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BTSR1_REG_TEA]=addr;
		BTSR1_ThrowFaultStatus(ctx, BTSR1_FLT_INVADDR);
		return(0);
	}
	return(sp->GetByte(ctx, sp, addr));
}

int BTSR1_MemGetWord(BTSR1_Context *ctx, btsr1_addr addr)
{
	BTSR1_MemSpan *sp;
	sp=BTSR1_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BTSR1_REG_TEA]=addr;
		BTSR1_ThrowFaultStatus(ctx, BTSR1_FLT_INVADDR);
		return(0);
	}
	return(sp->GetWord(ctx, sp, addr));
}

s32 BTSR1_MemGetDWord(BTSR1_Context *ctx, btsr1_addr addr)
{
	BTSR1_MemSpan *sp;
	sp=BTSR1_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BTSR1_REG_TEA]=addr;
		BTSR1_ThrowFaultStatus(ctx, BTSR1_FLT_INVADDR);
		return(0);
	}
	return(sp->GetDWord(ctx, sp, addr));
}

s64 BTSR1_MemGetQWord(BTSR1_Context *ctx, btsr1_addr addr)
{
	BTSR1_MemSpan *sp;
	s64 t;
	sp=BTSR1_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BTSR1_REG_TEA]=addr;
		BTSR1_ThrowFaultStatus(ctx, BTSR1_FLT_INVADDR);
		return(0);
	}
	t=sp->GetDWord(ctx, sp, addr+4);
	t=(t<<32)|((u32)(sp->GetDWord(ctx, sp, addr+0)));
	return(t);
}

int BTSR1_MemSetByte(BTSR1_Context *ctx, btsr1_addr addr, int val)
{
	BTSR1_MemSpan *sp;
	sp=BTSR1_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BTSR1_REG_TEA]=addr;
		BTSR1_ThrowFaultStatus(ctx, BTSR1_FLT_INVADDR);
		return(0);
	}
	return(sp->SetByte(ctx, sp, addr, val));
}

int BTSR1_MemSetWord(BTSR1_Context *ctx, btsr1_addr addr, int val)
{
	BTSR1_MemSpan *sp;
	sp=BTSR1_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BTSR1_REG_TEA]=addr;
		BTSR1_ThrowFaultStatus(ctx, BTSR1_FLT_INVADDR);
		return(0);
	}
	return(sp->SetWord(ctx, sp, addr, val));
}

int BTSR1_MemSetDWord(BTSR1_Context *ctx, btsr1_addr addr, s32 val)
{
	BTSR1_MemSpan *sp;
	sp=BTSR1_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BTSR1_REG_TEA]=addr;
		BTSR1_ThrowFaultStatus(ctx, BTSR1_FLT_INVADDR);
		return(0);
	}
	return(sp->SetDWord(ctx, sp, addr, val));
}

int BTSR1_MemSetQWord(BTSR1_Context *ctx, btsr1_addr addr, s64 val)
{
	BTSR1_MemSpan *sp;
	sp=BTSR1_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		ctx->regs[BTSR1_REG_TEA]=addr;
		BTSR1_ThrowFaultStatus(ctx, BTSR1_FLT_INVADDR);
		return(0);
	}
	sp->SetDWord(ctx, sp, addr+4, val>>32);
	return(sp->SetDWord(ctx, sp, addr, val));
}
