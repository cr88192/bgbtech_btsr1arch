void BTSR1_Op_MOV_ImmReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=op->imm;
}

void BTSR1_Op_MOV_Dr4Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[BTSR1_REG_DR]*16+op->imm;
}

void BTSR1_Op_ADD_ImmReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]+=op->imm;
}

void BTSR1_Op_ADD_Dr4Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]+=ctx->regs[BTSR1_REG_DR]*16+op->imm;
}

void BTSR1_Op_ADD_RegDrReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+ctx->regs[BTSR1_REG_DR];
}

void BTSR1_Op_MOV_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm];
}

void BTSR1_Op_ADD_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]+=ctx->regs[op->rm];
}

void BTSR1_Op_SUB_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]-=ctx->regs[op->rm];
}

void BTSR1_Op_SUB_RegDrReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]-ctx->regs[BTSR1_REG_DR];
}

void BTSR1_Op_ADC_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	u64 va, vb, vc;

	va=ctx->regs[op->rn];
	vb=ctx->regs[op->rm];
	vc=va+vb+(ctx->regs[BTSR1_REG_SR]&1);
	ctx->regs[op->rn]=vc;
	if((vc>>32)&1)
		ctx->regs[BTSR1_REG_SR]|=1;
	else
		ctx->regs[BTSR1_REG_SR]&=~1;

//	ctx->regs[op->rn]+=ctx->regs[op->rm]+(ctx->regs[BTSR1_REG_SR]&1);
}

void BTSR1_Op_SBB_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	u64 va, vb, vc;

	va=ctx->regs[op->rn];
	vb=ctx->regs[op->rm];
	vc=va+(~vb)+(!(ctx->regs[BTSR1_REG_SR]&1));
	ctx->regs[op->rn]=vc;
	if((vc>>32)&1)
		ctx->regs[BTSR1_REG_SR]|=1;
	else
		ctx->regs[BTSR1_REG_SR]&=~1;

//	ctx->regs[op->rn]+=(~ctx->regs[op->rm])+(!(ctx->regs[BTSR1_REG_SR]&1));
}

void BTSR1_Op_TST_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	if(ctx->regs[op->rn]&ctx->regs[op->rm])
		ctx->regs[BTSR1_REG_SR]&=~1;
	else
		ctx->regs[BTSR1_REG_SR]|=1;
}

void BTSR1_Op_TST_ImmReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	if(ctx->regs[op->rn]&op->imm)
		ctx->regs[BTSR1_REG_SR]&=~1;
	else
		ctx->regs[BTSR1_REG_SR]|=1;
}

void BTSR1_Op_AND_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]&=ctx->regs[op->rm];
}

void BTSR1_Op_AND_ImmReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]&=op->imm;
}

void BTSR1_Op_AND_RegDrReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]&ctx->regs[BTSR1_REG_DR];
}

void BTSR1_Op_OR_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]|=ctx->regs[op->rm];
}

void BTSR1_Op_OR_RegDrReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]|ctx->regs[BTSR1_REG_DR];
}


void BTSR1_Op_XOR_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]^=ctx->regs[op->rm];
}

void BTSR1_Op_XOR_RegDrReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]^ctx->regs[BTSR1_REG_DR];
}

void BTSR1_Op_MULS_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	s64 va, vb, vc;
	va=(s32)(ctx->regs[op->rn]);
	vb=(s32)(ctx->regs[op->rm]);
	vc=va*vb;
	ctx->regs[BTSR1_REG_DLR]=vc;
	ctx->regs[BTSR1_REG_DHR]=(vc>>32);
}

void BTSR1_Op_MULU_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	u64 va, vb, vc;
	va=(u32)(ctx->regs[op->rn]);
	vb=(u32)(ctx->regs[op->rm]);
	vc=va*vb;
	ctx->regs[BTSR1_REG_DLR]=vc;
	ctx->regs[BTSR1_REG_DHR]=(vc>>32);
}

void BTSR1_Op_CMPEQ_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	if(ctx->regs[op->rn]==ctx->regs[op->rm])
		ctx->regs[BTSR1_REG_SR]|=1;
	else
		ctx->regs[BTSR1_REG_SR]&=~1;
}

void BTSR1_Op_CMPHI_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	if(((u32)ctx->regs[op->rn])>((u32)ctx->regs[op->rm]))
		ctx->regs[BTSR1_REG_SR]|=1;
	else
		ctx->regs[BTSR1_REG_SR]&=~1;
}

void BTSR1_Op_CMPGT_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	if(((s32)ctx->regs[op->rn])>((s32)ctx->regs[op->rm]))
		ctx->regs[BTSR1_REG_SR]|=1;
	else
		ctx->regs[BTSR1_REG_SR]&=~1;
}

void BTSR1_Op_CMPHS_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	if(((u32)ctx->regs[op->rn])>=((u32)ctx->regs[op->rm]))
		ctx->regs[BTSR1_REG_SR]|=1;
	else
		ctx->regs[BTSR1_REG_SR]&=~1;
}

void BTSR1_Op_CMPGE_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	if(((s32)ctx->regs[op->rn])>=((s32)ctx->regs[op->rm]))
		ctx->regs[BTSR1_REG_SR]|=1;
	else
		ctx->regs[BTSR1_REG_SR]&=~1;
}

void BTSR1_Op_CMPEQ_ImmReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	if(ctx->regs[op->rn]==op->imm)
		ctx->regs[BTSR1_REG_SR]|=1;
	else
		ctx->regs[BTSR1_REG_SR]&=~1;
}

void BTSR1_Op_CMPGT_ImmReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	if(((s32)ctx->regs[op->rn])>op->imm)
		ctx->regs[BTSR1_REG_SR]|=1;
	else
		ctx->regs[BTSR1_REG_SR]&=~1;
}

void BTSR1_Op_CMPGE_ImmReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	if(((s32)ctx->regs[op->rn])>=op->imm)
		ctx->regs[BTSR1_REG_SR]|=1;
	else
		ctx->regs[BTSR1_REG_SR]&=~1;
}


void BTSR1_Op_LDIZ_Imm(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_DR]=op->imm&255;
}

void BTSR1_Op_LDIN_Imm(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_DR]=op->imm|(~255);
}

void BTSR1_Op_LDISH_Imm(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_DR]=(ctx->regs[BTSR1_REG_DR]<<8)|(op->imm&255);
}

void BTSR1_Op_LDISH16_Imm(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_DR]=(ctx->regs[BTSR1_REG_DR]<<16)|(op->imm&65535);
}

void BTSR1_Op_LDISH12_Imm(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_DR]=(ctx->regs[BTSR1_REG_DR]<<12)|(op->imm&4095);
}

void BTSR1_Op_LDISH20_Imm(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_DR]=(ctx->regs[BTSR1_REG_DR]<<20)|(op->imm&0xFFFFF);
}


void BTSR1_Op_LDIZ_Imm12(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_DR]=op->imm&4095;
}

void BTSR1_Op_LDIN_Imm12(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_DR]=op->imm|(~4095);
}

void BTSR1_Op_LEAB_LdRegDispReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+(op->imm*1);
}

void BTSR1_Op_LEAW_LdRegDispReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+(op->imm*2);
}

void BTSR1_Op_LEAD_LdRegDispReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+(op->imm*4);
}

void BTSR1_Op_LEAB_LdDrRegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+(ctx->regs[BTSR1_REG_DR]*1);
}

void BTSR1_Op_LEAW_LdDrRegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+(ctx->regs[BTSR1_REG_DR]*2);
}

void BTSR1_Op_LEAD_LdDrRegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+(ctx->regs[BTSR1_REG_DR]*4);
}

void BTSR1_Op_LEAQ_LdDrRegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+(ctx->regs[BTSR1_REG_DR]*8);
}

void BTSR1_Op_LEAB_LdDrPcReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*1);
}

void BTSR1_Op_LEAW_LdDrPcReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*2);
}

void BTSR1_Op_LEAD_LdDrPcReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*4);
}


void BTSR1_Op_LEAB_LdDr4PcReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*16)+(op->imm);
}

void BTSR1_Op_LEAW_LdDr4PcReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*32)+(op->imm*2);
}


void BTSR1_Op_NOP_None(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
}

void BTSR1_Op_CLRT_None(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_SR]&=~1;
}

void BTSR1_Op_SETT_None(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_SR]|=1;
}

void BTSR1_Op_CLRS_None(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_SR]&=~2;
}

void BTSR1_Op_SETS_None(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_SR]|=2;
}

void BTSR1_Op_NOTT_None(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_SR]^=1;
}

void BTSR1_Op_NOTS_None(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_SR]^=2;
}


void BTSR1_Op_CMPPL_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	if(((s32)ctx->regs[op->rn])>0)
		ctx->regs[BTSR1_REG_SR]|=1;
	else
		ctx->regs[BTSR1_REG_SR]&=~1;
}

void BTSR1_Op_CMPPZ_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	if(((s32)ctx->regs[op->rn])>=0)
		ctx->regs[BTSR1_REG_SR]|=1;
	else
		ctx->regs[BTSR1_REG_SR]&=~1;
}

void BTSR1_Op_EXTUB_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(byte)ctx->regs[op->rn];
}

void BTSR1_Op_EXTUW_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(u16)ctx->regs[op->rn];
}

void BTSR1_Op_EXTSB_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(sbyte)ctx->regs[op->rn];
}

void BTSR1_Op_EXTSW_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(s16)ctx->regs[op->rn];
}


void BTSR1_Op_SHLL_ImmReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rn]<<op->imm;
}

void BTSR1_Op_SHLR_ImmReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=((u32)ctx->regs[op->rn])>>op->imm;
}

void BTSR1_Op_SHAR_ImmReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=((s32)ctx->regs[op->rn])>>op->imm;
}

void BTSR1_Op_NOT_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=~ctx->regs[op->rn];
}

void BTSR1_Op_NEG_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=-ctx->regs[op->rn];
}

void BTSR1_Op_NEGC_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(~ctx->regs[op->rn])+
		(~(ctx->regs[BTSR1_REG_SR]&1));
}

void BTSR1_Op_ROTL_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=
		(ctx->regs[op->rn]<<1)|
		(ctx->regs[op->rn]>>31);
}

void BTSR1_Op_ROTR_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=
		(ctx->regs[op->rn]>>1)|
		(ctx->regs[op->rn]<<31);
}

void BTSR1_Op_ROTCL_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	u32 sr;
	sr=ctx->regs[BTSR1_REG_SR];
	ctx->regs[BTSR1_REG_SR]=sr&(~1);
	ctx->regs[BTSR1_REG_SR]|=((ctx->regs[op->rn]>>31)&1);
	ctx->regs[op->rn]=
		(ctx->regs[op->rn]<<1) | (sr&1);
}

void BTSR1_Op_ROTCR_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	u32 sr;
	sr=ctx->regs[BTSR1_REG_SR];
	ctx->regs[BTSR1_REG_SR]=sr&(~1);
	ctx->regs[BTSR1_REG_SR]|=((ctx->regs[op->rn]   )&1);
	ctx->regs[op->rn]=
		(ctx->regs[op->rn]>>1) | ((sr&1)<<31);
}

void BTSR1_Op_SHLL_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_SR]&=(~1);
	ctx->regs[BTSR1_REG_SR]|=((ctx->regs[op->rn]>>31)&1);
	ctx->regs[op->rn]=ctx->regs[op->rn]<<1;
}

void BTSR1_Op_SHLR_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_SR]&=(~1);
	ctx->regs[BTSR1_REG_SR]|=((ctx->regs[op->rn]    )&1);
	ctx->regs[op->rn]=ctx->regs[op->rn]>>1;
}

void BTSR1_Op_SHAR_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_SR]&=(~1);
	ctx->regs[BTSR1_REG_SR]|=((ctx->regs[op->rn]>>31)&1);
	ctx->regs[op->rn]=((s32)ctx->regs[op->rn])>>1;
}

void BTSR1_Op_MOVT_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(ctx->regs[BTSR1_REG_SR]&1);
}

void BTSR1_Op_SWAPB_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=
		((ctx->regs[op->rn]&0xFF00FF00U)>>8) |
		((ctx->regs[op->rn]&0x00FF00FFU)<<8);
}

void BTSR1_Op_SWAPW_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=
		((ctx->regs[op->rn]&0xFFFF0000U)>>16) |
		((ctx->regs[op->rn]&0x0000FFFFU)<<16);
}


void BTSR1_Op_SHAD_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[op->rm];
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			ctx->regs[op->rn]<<(shl&31);
	}else
	{
		ctx->regs[op->rn]=
			((s32)ctx->regs[op->rn])>>(shl&31);
	}
}

void BTSR1_Op_SHLD_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[op->rm];
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			ctx->regs[op->rn]<<(shl&31);
	}else
	{
		ctx->regs[op->rn]=
			((u32)ctx->regs[op->rn])>>(shl&31);
	}
}

