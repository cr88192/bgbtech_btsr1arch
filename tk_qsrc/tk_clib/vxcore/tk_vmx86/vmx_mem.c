int VMX86_Mem_GetByte(VMX86_Context *ctx, u64 addr)
{
	byte *natram;
	
	natram=ctx->nat_ram;
	if(natram && !(addr>>ctx->nat_ramsz))
	{
		return(*(byte *)(natram+addr));
	}
	
	__debugbreak();
}

void VMX86_Mem_SetByte(VMX86_Context *ctx, u64 addr, int val)
{
	byte *natram;
	
	natram=ctx->nat_ram;
	if(natram && !(addr>>ctx->nat_ramsz))
	{
		*(byte *)(natram+addr)=val;
		return;
	}
	
	__debugbreak();
}

int VMX86_Mem_GetWord(VMX86_Context *ctx, u64 addr)
{
	byte *natram;
	
	natram=ctx->nat_ram;
	if(natram && !(addr>>ctx->nat_ramsz))
	{
		return(*(u16 *)(natram+addr));
	}
	
	__debugbreak();
}

void VMX86_Mem_SetWord(VMX86_Context *ctx, u64 addr, int val)
{
	byte *natram;
	
	natram=ctx->nat_ram;
	if(natram && !(addr>>ctx->nat_ramsz))
	{
		*(u16 *)(natram+addr)=val;
		return;
	}
	
	__debugbreak();
}

u32 VMX86_Mem_GetDWord(VMX86_Context *ctx, u64 addr)
{
	byte *natram;
	
	natram=ctx->nat_ram;
	if(natram && !(addr>>ctx->nat_ramsz))
	{
		return(*(u32 *)(natram+addr));
	}
	
	__debugbreak();
}

void VMX86_Mem_SetDWord(VMX86_Context *ctx, u64 addr, u32 val)
{
	byte *natram;
	
	natram=ctx->nat_ram;
	if(natram && !(addr>>ctx->nat_ramsz))
	{
		*(u32 *)(natram+addr)=val;
		return;
	}
	
	__debugbreak();
}

u64 VMX86_Mem_GetQWord(VMX86_Context *ctx, u64 addr)
{
	byte *natram;
	
	natram=ctx->nat_ram;
	if(natram && !(addr>>ctx->nat_ramsz))
	{
		return(*(u64 *)(natram+addr));
	}
	
	__debugbreak();
}

void VMX86_Mem_SetQWord(VMX86_Context *ctx, u64 addr, u64 val)
{
	byte *natram;
	
	natram=ctx->nat_ram;
	if(natram && !(addr>>ctx->nat_ramsz))
	{
		*(u64 *)(natram+addr)=val;
		return;
	}
	
	__debugbreak();
}

/*
 * Get value based on decoded R/M field for an instruction (Byte).
 */
u64 VMX86_Mem_GetMemValueGenByte(VMX86_Context *ctx, VMX86_Opcode *op)
{
	u64 vb, vi, va;

	if(op->rb==VMX86_REGS_ZZR)
	{
		if(op->rex)
		{
			return((byte)(ctx->regs[op->ri&15]));
		}else
		{
			if(op->ri&4)
			{
				return((byte)(ctx->regs[op->ri&3]>>8));
			}else
			{
				return((byte)(ctx->regs[op->ri&3]));
			}
		}
	}

	if(op->rb==VMX86_REGS_RIP)
	{
		va=op->rip+op->sz+op->disp;
		ctx->cached_agu=va;
		return(VMX86_Mem_GetByte(ctx, va));
	}
	
	vb=(ctx->regs[op->rb&15]);
	if(op->ri==VMX86_REGS_ZZR)
		{ vi=0; }
	else
		{ vi=(ctx->regs[op->ri&15])<<(op->sc&3); }

	va=vb+vi+op->disp;
	ctx->cached_agu=va;
	return(VMX86_Mem_GetByte(ctx, va));
}

u64 VMX86_Mem_GetMemValueGenWord(VMX86_Context *ctx, VMX86_Opcode *op)
{
	u64 vb, vi, va;

	if(op->rb==VMX86_REGS_ZZR)
	{
		return((u16)(ctx->regs[op->ri&15]));
	}

	if(op->rb==VMX86_REGS_RIP)
	{
		va=op->rip+op->sz+op->disp;
		ctx->cached_agu=va;
		return(VMX86_Mem_GetWord(ctx, va));
	}
	
	vb=(ctx->regs[op->rb&15]);
	if(op->ri==VMX86_REGS_ZZR)
		{ vi=0; }
	else
		{ vi=(ctx->regs[op->ri&15])<<(op->sc&3); }

	va=vb+vi+op->disp;
	ctx->cached_agu=va;
	return(VMX86_Mem_GetWord(ctx, va));
}

u64 VMX86_Mem_GetMemValueGenDWord(VMX86_Context *ctx, VMX86_Opcode *op)
{
	u64 vb, vi, va;

	if(op->rb==VMX86_REGS_ZZR)
	{
		return((u32)(ctx->regs[op->ri&15]));
	}

	if(op->rb==VMX86_REGS_RIP)
	{
		va=op->rip+op->sz+op->disp;
		ctx->cached_agu=va;
		return(VMX86_Mem_GetDWord(ctx, va));
	}
	
	vb=(ctx->regs[op->rb&15]);
	if(op->ri==VMX86_REGS_ZZR)
		{ vi=0; }
	else
		{ vi=(ctx->regs[op->ri&15])<<(op->sc&3); }

	va=vb+vi+op->disp;
	ctx->cached_agu=va;
	return(VMX86_Mem_GetDWord(ctx, va));
}

u64 VMX86_Mem_GetMemValueGenQWord(VMX86_Context *ctx, VMX86_Opcode *op)
{
	u64 vb, vi, va;

	if(op->rb==VMX86_REGS_ZZR)
	{
		return((u64)(ctx->regs[op->ri&15]));
	}

	if(op->rb==VMX86_REGS_RIP)
	{
		va=op->rip+op->sz+op->disp;
		ctx->cached_agu=va;
		return(VMX86_Mem_GetQWord(ctx, va));
	}
	
	vb=(ctx->regs[op->rb&15]);
	if(op->ri==VMX86_REGS_ZZR)
		{ vi=0; }
	else
		{ vi=(ctx->regs[op->ri&15])<<(op->sc&3); }

	va=vb+vi+op->disp;
	ctx->cached_agu=va;
	return(VMX86_Mem_GetQWord(ctx, va));
}

u64 VMX86_Mem_GetMemValueRegDWord(VMX86_Context *ctx, VMX86_Opcode *op)
{
	return((u32)(ctx->regs[op->ri&15]));
}

u64 VMX86_Mem_GetMemValueRegQWord(VMX86_Context *ctx, VMX86_Opcode *op)
{
	return(ctx->regs[op->ri&15]);
}


void VMX86_Mem_SetMemValueGenByte(VMX86_Context *ctx,
	VMX86_Opcode *op, u64 val)
{
	u64 vb, vi, va;

	if(op->rb==VMX86_REGS_ZZR)
	{
		if(op->rex)
		{
			ctx->regs[op->ri&15]=
				((ctx->regs[op->ri&15])&(~255)) |
				(val&255);
			return;
		}else
		{
			if(op->ri&4)
			{
				ctx->regs[op->ri&15]=
					((ctx->regs[op->ri&15])&(~0xFF00)) |
					((val&255)<<8);
				return;
			}else
			{
				ctx->regs[op->ri&3]=
					((ctx->regs[op->ri&3])&(~255)) |
					(val&255);
				return;
			}
		}
	}

	if(op->rb==VMX86_REGS_RIP)
	{
		return(VMX86_Mem_GetByte(ctx, op->rip+op->sz+op->disp));
	}
	
	vb=(ctx->regs[op->rb&15]);
	if(op->ri==VMX86_REGS_ZZR)
		{ vi=0; }
	else
		{ vi=(ctx->regs[op->ri&15])<<(op->sc&3); }

	return(VMX86_Mem_GetByte(ctx, vb+vi+op->disp));
}

void VMX86_Mem_SetMemValueGenWord(VMX86_Context *ctx,
	VMX86_Opcode *op, u64 val)
{
	u64 vb, vi, va;

	if(op->rb==VMX86_REGS_ZZR)
	{
		ctx->regs[op->ri&15]=
			((ctx->regs[op->ri&15])&(~65535)) |
			(val&65535);
		return;
	}

	if(op->rb==VMX86_REGS_RIP)
	{
		VMX86_Mem_SetWord(ctx, op->rip+op->sz+op->disp, val);
		return;
	}
	
	vb=(ctx->regs[op->rb&15]);
	if(op->ri==VMX86_REGS_ZZR)
		{ vi=0; }
	else
		{ vi=(ctx->regs[op->ri&15])<<(op->sc&3); }

	VMX86_Mem_SetWord(ctx, vb+vi+op->disp, val);
}

void VMX86_Mem_SetMemValueGenDWord(VMX86_Context *ctx,
	VMX86_Opcode *op, u64 val)
{
	u64 vb, vi, va;

	if(op->rb==VMX86_REGS_ZZR)
	{
		ctx->regs[op->ri&15]=(u32)val;
		return;
	}

	if(op->rb==VMX86_REGS_RIP)
	{
		VMX86_Mem_SetDWord(ctx, op->rip+op->sz+op->disp, val);
		return;
	}
	
	vb=(ctx->regs[op->rb&15]);
	if(op->ri==VMX86_REGS_ZZR)
		{ vi=0; }
	else
		{ vi=(ctx->regs[op->ri&15])<<(op->sc&3); }

	VMX86_Mem_SetDWord(ctx, vb+vi+op->disp, val);
}

void VMX86_Mem_SetMemValueGenQWord(VMX86_Context *ctx,
	VMX86_Opcode *op, u64 val)
{
	u64 vb, vi, va;

	if(op->rb==VMX86_REGS_ZZR)
	{
		ctx->regs[op->ri&15]=val;
		return;
	}

	if(op->rb==VMX86_REGS_RIP)
	{
		VMX86_Mem_SetQWord(ctx, op->rip+op->sz+op->disp, val);
		return;
	}
	
	vb=(ctx->regs[op->rb&15]);
	if(op->ri==VMX86_REGS_ZZR)
		{ vi=0; }
	else
		{ vi=(ctx->regs[op->ri&15])<<(op->sc&3); }

	VMX86_Mem_SetQWord(ctx, vb+vi+op->disp, val);
}

void VMX86_Mem_SetMemValueCachedDWord(VMX86_Context *ctx,
	VMX86_Opcode *op, u64 val)
{
	VMX86_Mem_SetDWord(ctx, ctx->cached_agu, val);
}

void VMX86_Mem_SetMemValueCachedQWord(VMX86_Context *ctx,
	VMX86_Opcode *op, u64 val)
{
	VMX86_Mem_SetQWord(ctx, ctx->cached_agu, val);
}

void VMX86_Mem_SetMemValueRegDWord(VMX86_Context *ctx,
	VMX86_Opcode *op, u64 val)
{
	ctx->regs[op->ri&15]=(u32)val;
}

void VMX86_Mem_SetMemValueRegQWord(VMX86_Context *ctx,
	VMX86_Opcode *op, u64 val)
{
	ctx->regs[op->ri&15]=val;
}

/*
Get value based on decoded R/M field for an instruction.
 */
u64 VMX86_Mem_GetMemValueGeneric(VMX86_Context *ctx, VMX86_Opcode *op)
{
}
