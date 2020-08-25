/*
 Copyright (c) 2018-2020 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

void BJX2_Op_CMOVTB_RegStReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->trapc=op->pc;
		BJX2_MemSetByte(ctx, ctx->regs[op->rn], ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVTB_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=(sbyte)BJX2_MemGetByte(ctx, ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVTW_RegStReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->trapc=op->pc;
		BJX2_MemSetWord(ctx, ctx->regs[op->rn], ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVTW_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=(s16)BJX2_MemGetWord(ctx, ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVTL_RegStReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->trapc=op->pc;
		BJX2_MemSetDWord(ctx, ctx->regs[op->rn], ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVTL_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=(s32)BJX2_MemGetDWord(ctx, ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVTQ_RegStReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->trapc=op->pc;
		BJX2_MemSetQWord(ctx, ctx->regs[op->rn], ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVTQ_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=BJX2_MemGetQWord(ctx, ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVUTB_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=(byte)BJX2_MemGetByte(ctx, ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVUTW_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=(u16)BJX2_MemGetWord(ctx, ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVUTL_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=(u32)BJX2_MemGetDWord(ctx, ctx->regs[op->rm]);
	}
}


void BJX2_Op_CMOVTB_RegStDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->trapc=op->pc;
		BJX2_MemSetByte(ctx,
			ctx->regs[op->rn]+(ctx->regs[BJX2_REG_DR]*1),
			ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVTB_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=(sbyte)BJX2_MemGetByte(ctx,
			ctx->regs[op->rm]+(ctx->regs[BJX2_REG_DR]*1));
	}
}

void BJX2_Op_CMOVTW_RegStDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->trapc=op->pc;
		BJX2_MemSetWord(ctx,
			ctx->regs[op->rn]+(ctx->regs[BJX2_REG_DR]*2),
			ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVTW_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=(s16)BJX2_MemGetWord(ctx,
			ctx->regs[op->rm]+(ctx->regs[BJX2_REG_DR]*2));
	}
}

void BJX2_Op_CMOVTL_RegStDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->trapc=op->pc;
		BJX2_MemSetDWord(ctx,
			ctx->regs[op->rn]+(ctx->regs[BJX2_REG_DR]*4),
			ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVTL_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=(s32)BJX2_MemGetDWord(ctx,
			ctx->regs[op->rm]+(ctx->regs[BJX2_REG_DR]*4));
	}
}

void BJX2_Op_CMOVTQ_RegStDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->trapc=op->pc;
		BJX2_MemSetQWord(ctx,
			ctx->regs[op->rn]+(ctx->regs[BJX2_REG_DR]*8),
			ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVTQ_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=BJX2_MemGetQWord(ctx,
			ctx->regs[op->rm]+(ctx->regs[BJX2_REG_DR]*8));
	}
}

void BJX2_Op_CMOVUTB_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=(byte)BJX2_MemGetByte(ctx,
			ctx->regs[op->rm]+(ctx->regs[BJX2_REG_DR]*1));
	}
}

void BJX2_Op_CMOVUTW_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=(u16)BJX2_MemGetWord(ctx,
			ctx->regs[op->rm]+(ctx->regs[BJX2_REG_DR]*2));
	}
}

void BJX2_Op_CMOVUTL_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=(u32)BJX2_MemGetDWord(ctx,
			ctx->regs[op->rm]+(ctx->regs[BJX2_REG_DR]*4));
	}
}



void BJX2_Op_CMOVFB_RegStReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->trapc=op->pc;
		BJX2_MemSetByte(ctx, ctx->regs[op->rn], ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVFB_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=(sbyte)BJX2_MemGetByte(ctx, ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVFW_RegStReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->trapc=op->pc;
		BJX2_MemSetWord(ctx, ctx->regs[op->rn], ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVFW_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=(s16)BJX2_MemGetWord(ctx, ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVFL_RegStReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->trapc=op->pc;
		BJX2_MemSetDWord(ctx, ctx->regs[op->rn], ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVFL_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=(s32)BJX2_MemGetDWord(ctx, ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVFQ_RegStReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->trapc=op->pc;
		BJX2_MemSetQWord(ctx, ctx->regs[op->rn], ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVFQ_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=BJX2_MemGetQWord(ctx, ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVUFB_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=(byte)BJX2_MemGetByte(ctx, ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVUFW_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=(u16)BJX2_MemGetWord(ctx, ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVUFL_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=(u32)BJX2_MemGetDWord(ctx, ctx->regs[op->rm]);
	}
}


void BJX2_Op_CMOVFB_RegStDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->trapc=op->pc;
		BJX2_MemSetByte(ctx,
			ctx->regs[op->rn]+(ctx->regs[BJX2_REG_DR]*1),
			ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVFB_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=(sbyte)BJX2_MemGetByte(ctx,
			ctx->regs[op->rm]+(ctx->regs[BJX2_REG_DR]*1));
	}
}

void BJX2_Op_CMOVFW_RegStDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->trapc=op->pc;
		BJX2_MemSetWord(ctx,
			ctx->regs[op->rn]+(ctx->regs[BJX2_REG_DR]*2),
			ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVFW_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=(s16)BJX2_MemGetWord(ctx,
			ctx->regs[op->rm]+(ctx->regs[BJX2_REG_DR]*2));
	}
}

void BJX2_Op_CMOVFL_RegStDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->trapc=op->pc;
		BJX2_MemSetDWord(ctx,
			ctx->regs[op->rn]+(ctx->regs[BJX2_REG_DR]*4),
			ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVFL_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=(s32)BJX2_MemGetDWord(ctx,
			ctx->regs[op->rm]+(ctx->regs[BJX2_REG_DR]*4));
	}
}

void BJX2_Op_CMOVFQ_RegStDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->trapc=op->pc;
		BJX2_MemSetQWord(ctx,
			ctx->regs[op->rn]+(ctx->regs[BJX2_REG_DR]*8),
			ctx->regs[op->rm]);
	}
}

void BJX2_Op_CMOVFQ_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=BJX2_MemGetQWord(ctx,
			ctx->regs[op->rm]+(ctx->regs[BJX2_REG_DR]*8));
	}
}

void BJX2_Op_CMOVUFB_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=(byte)BJX2_MemGetByte(ctx,
			ctx->regs[op->rm]+(ctx->regs[BJX2_REG_DR]*1));
	}
}

void BJX2_Op_CMOVUFW_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=(u16)BJX2_MemGetWord(ctx,
			ctx->regs[op->rm]+(ctx->regs[BJX2_REG_DR]*2));
	}
}

void BJX2_Op_CMOVUFL_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->trapc=op->pc;
		ctx->regs[op->rn]=(u32)BJX2_MemGetDWord(ctx,
			ctx->regs[op->rm]+(ctx->regs[BJX2_REG_DR]*4));
	}
}

