void BJX2_Op_MOV_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=op->imm;
}

void BJX2_Op_MOV_ImmxReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=((s32)op->imm)^0xFFFFFFFF00000000LL;
}

void BJX2_Op_MOV_Dr4Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[BJX2_REG_DR]*16+op->imm;
}

void BJX2_Op_ADD_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]+=op->imm;
}

void BJX2_Op_ADD_Dr4Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]+=ctx->regs[BJX2_REG_DR]*16+op->imm;
}

void BJX2_Op_ADD_RegDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+ctx->regs[BJX2_REG_DR];
}

void BJX2_Op_ADD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+ctx->regs[op->ro];
}

void BJX2_Op_ADD_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+op->imm;
}

void BJX2_Op_MOV_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm];
}

void BJX2_Op_ADD_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]+=ctx->regs[op->rm];
}

void BJX2_Op_SUB_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]-=ctx->regs[op->rm];
}

void BJX2_Op_SUB_RegDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]-ctx->regs[BJX2_REG_DR];
}

void BJX2_Op_SUB_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]-ctx->regs[op->ro];
}

void BJX2_Op_SUB_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]-op->imm;
}

void BJX2_Op_ADC_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;

	va=ctx->regs[op->rn];
	vb=ctx->regs[op->rm];
	vc=va+vb+(ctx->regs[BJX2_REG_SR]&1);
	ctx->regs[op->rn]=vc;
	if((vc>>32)&1)
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;

//	ctx->regs[op->rn]+=ctx->regs[op->rm]+(ctx->regs[BJX2_REG_SR]&1);
}

void BJX2_Op_SBB_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;

	va=ctx->regs[op->rn];
	vb=ctx->regs[op->rm];
	vc=va+(~vb)+(!(ctx->regs[BJX2_REG_SR]&1));
	ctx->regs[op->rn]=vc;
	if((vc>>32)&1)
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;

//	ctx->regs[op->rn]+=(~ctx->regs[op->rm])+(!(ctx->regs[BJX2_REG_SR]&1));
}

void BJX2_Op_TST_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if((u32)(ctx->regs[op->rn]&ctx->regs[op->rm]))
		ctx->regs[BJX2_REG_SR]&=~1;
	else
		ctx->regs[BJX2_REG_SR]|=1;
}

void BJX2_Op_TST_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if((u32)(ctx->regs[op->rn]&op->imm))
		ctx->regs[BJX2_REG_SR]&=~1;
	else
		ctx->regs[BJX2_REG_SR]|=1;
}

void BJX2_Op_TSTQ_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[op->rn]&ctx->regs[op->rm])
		ctx->regs[BJX2_REG_SR]&=~1;
	else
		ctx->regs[BJX2_REG_SR]|=1;
}

void BJX2_Op_TSTQ_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[op->rn]&op->imm)
		ctx->regs[BJX2_REG_SR]&=~1;
	else
		ctx->regs[BJX2_REG_SR]|=1;
}

void BJX2_Op_AND_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]&=ctx->regs[op->rm];
}

void BJX2_Op_AND_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]&=op->imm;
}

void BJX2_Op_AND_RegDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]&ctx->regs[BJX2_REG_DR];
}

void BJX2_Op_AND_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]&ctx->regs[op->ro];
}

void BJX2_Op_AND_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]&op->imm;
}

void BJX2_Op_OR_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]|=ctx->regs[op->rm];
}

void BJX2_Op_OR_RegDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]|ctx->regs[BJX2_REG_DR];
}

void BJX2_Op_OR_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]|ctx->regs[op->ro];
}

void BJX2_Op_OR_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]|op->imm;
}


void BJX2_Op_XOR_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]^=ctx->regs[op->rm];
}

void BJX2_Op_XOR_RegDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]^ctx->regs[BJX2_REG_DR];
}

void BJX2_Op_XOR_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]^ctx->regs[op->ro];
}

void BJX2_Op_XOR_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]^op->imm;
}

void BJX2_Op_MULS_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 va, vb, vc;
	va=(s32)(ctx->regs[op->rn]);
	vb=(s32)(ctx->regs[op->rm]);
	vc=va*vb;
	ctx->regs[BJX2_REG_DLR]=(u32)(vc    );
	ctx->regs[BJX2_REG_DHR]=(s32)(vc>>32);
}

void BJX2_Op_MULU_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;
	va=(u32)(ctx->regs[op->rn]);
	vb=(u32)(ctx->regs[op->rm]);
	vc=va*vb;
	ctx->regs[BJX2_REG_DLR]=(u32)(vc    );
	ctx->regs[BJX2_REG_DHR]=(u32)(vc>>32);
}

void BJX2_Op_MULS_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 va, vb, vc;
	va=(s32)(ctx->regs[op->ro]);
	vb=(s32)(ctx->regs[op->rm]);
	vc=va*vb;
	ctx->regs[op->rn]=vc;
}

void BJX2_Op_MULS_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]*op->imm;
}

void BJX2_Op_CMPEQ_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((u32)ctx->regs[op->rn])==((u32)ctx->regs[op->rm]))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPHI_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((u32)ctx->regs[op->rn])>((u32)ctx->regs[op->rm]))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPGT_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((s32)ctx->regs[op->rn])>((s32)ctx->regs[op->rm]))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPHS_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((u32)ctx->regs[op->rn])>=((u32)ctx->regs[op->rm]))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPGE_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((s32)ctx->regs[op->rn])>=((s32)ctx->regs[op->rm]))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPEQ_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((u32)ctx->regs[op->rn])==((u32)op->imm))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPGT_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((s32)ctx->regs[op->rn])>((s32)op->imm))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPGE_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((s32)ctx->regs[op->rn])>=((s32)op->imm))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPHI_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((u32)ctx->regs[op->rn])>((u32)op->imm))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}


void BJX2_Op_CMPQEQ_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[op->rn]==ctx->regs[op->rm])
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPQHI_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((u64)ctx->regs[op->rn])>((u64)ctx->regs[op->rm]))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPQGT_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((s64)ctx->regs[op->rn])>((s64)ctx->regs[op->rm]))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPQHS_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((u64)ctx->regs[op->rn])>=((u64)ctx->regs[op->rm]))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPQGE_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((s64)ctx->regs[op->rn])>=((s64)ctx->regs[op->rm]))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPQEQ_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((u64)ctx->regs[op->rn])==((u64)op->imm))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPQGT_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((s64)ctx->regs[op->rn])>((s64)op->imm))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPQGE_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((s64)ctx->regs[op->rn])>=((s64)op->imm))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPQHI_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((u64)ctx->regs[op->rn])>((u64)op->imm))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}


void BJX2_Op_LDIZ_Imm(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=op->imm&255;
}

void BJX2_Op_LDIN_Imm(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=op->imm|(~255);
}

void BJX2_Op_LDISH_Imm(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=(ctx->regs[BJX2_REG_DR]<<8)|(op->imm&255);
}

void BJX2_Op_LDISH16_Imm(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=(ctx->regs[BJX2_REG_DR]<<16)|(op->imm&65535);
}

void BJX2_Op_LDISH12_Imm(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=(ctx->regs[BJX2_REG_DR]<<12)|(op->imm&4095);
}

void BJX2_Op_LDISH20_Imm(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=(ctx->regs[BJX2_REG_DR]<<20)|(op->imm&0xFFFFF);
}

void BJX2_Op_LDISH32_Imm(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=
		(((u64)ctx->regs[BJX2_REG_DR])<<32)|
		((u32)(op->imm));
}


void BJX2_Op_LDISH16_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(ctx->regs[op->rn]<<16)|((u16)(op->imm));
}

void BJX2_Op_LDISH32_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(ctx->regs[op->rn]<<32)|((u32)(op->imm));
}


void BJX2_Op_LDIZ_Imm12(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=op->imm&4095;
}

void BJX2_Op_LDIN_Imm12(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=op->imm|(~4095);
}

void BJX2_Op_LDIZ_Imm24(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=op->imm&16777215;
}

void BJX2_Op_LDIN_Imm24(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=op->imm|(~16777215);
}

void BJX2_Op_LDIZ_Imm32(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=(u32)op->imm;
}

void BJX2_Op_LDIN_Imm32(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=((u32)op->imm)|(-1LL<<32);
}

void BJX2_Op_LEAB_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+(op->imm*1);
}

void BJX2_Op_LEAW_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+(op->imm*2);
}

void BJX2_Op_LEAD_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+(op->imm*4);
}

void BJX2_Op_LEAQ_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+(op->imm*8);
}

void BJX2_Op_LEAB_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+(ctx->regs[BJX2_REG_DR]*1);
}

void BJX2_Op_LEAW_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+(ctx->regs[BJX2_REG_DR]*2);
}

void BJX2_Op_LEAD_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+(ctx->regs[BJX2_REG_DR]*4);
}

void BJX2_Op_LEAQ_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+(ctx->regs[BJX2_REG_DR]*8);
}

void BJX2_Op_LEAB_LdDrPcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(op->pc+2)+(ctx->regs[BJX2_REG_DR]*1);
}

void BJX2_Op_LEAW_LdDrPcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(op->pc+2)+(ctx->regs[BJX2_REG_DR]*2);
}

void BJX2_Op_LEAD_LdDrPcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(op->pc+2)+(ctx->regs[BJX2_REG_DR]*4);
}

void BJX2_Op_LEAQ_LdDrPcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(op->pc+2)+(ctx->regs[BJX2_REG_DR]*8);
}


void BJX2_Op_LEAB_LdDr4PcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(op->pc+2)+(ctx->regs[BJX2_REG_DR]*16)+(op->imm);
}

void BJX2_Op_LEAW_LdDr4PcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(op->pc+2)+(ctx->regs[BJX2_REG_DR]*32)+(op->imm*2);
}


void BJX2_Op_LEAB_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+(ctx->regs[op->ro]*1);
}

void BJX2_Op_LEAW_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+(ctx->regs[op->ro]*2);
}

void BJX2_Op_LEAD_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+(ctx->regs[op->ro]*4);
}

void BJX2_Op_LEAQ_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+(ctx->regs[op->ro]*8);
}

void BJX2_Op_LEAB_LdPcDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(op->pc2)+op->imm;
}


void BJX2_Op_NOP_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
}

void BJX2_Op_CLRT_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_SETT_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_SR]|=1;
}

void BJX2_Op_CLRS_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_SR]&=~2;
}

void BJX2_Op_SETS_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_SR]|=2;
}

void BJX2_Op_NOTT_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_SR]^=1;
}

void BJX2_Op_NOTS_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_SR]^=2;
}


void BJX2_Op_CMPPL_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((s32)ctx->regs[op->rn])>0)
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPPZ_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((s32)ctx->regs[op->rn])>=0)
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_EXTUB_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(byte)ctx->regs[op->rn];
}

void BJX2_Op_EXTUW_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(u16)ctx->regs[op->rn];
}

void BJX2_Op_EXTSB_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(sbyte)ctx->regs[op->rn];
}

void BJX2_Op_EXTSW_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(s16)ctx->regs[op->rn];
}

void BJX2_Op_EXTUL_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(u32)ctx->regs[op->rn];
}

void BJX2_Op_EXTSL_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(s32)ctx->regs[op->rn];
}


void BJX2_Op_EXTUB_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(byte)ctx->regs[op->rm];
}

void BJX2_Op_EXTUW_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(u16)ctx->regs[op->rm];
}

void BJX2_Op_EXTSB_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(sbyte)ctx->regs[op->rm];
}

void BJX2_Op_EXTSW_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(s16)ctx->regs[op->rm];
}

void BJX2_Op_EXTUL_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(u32)ctx->regs[op->rm];
}

void BJX2_Op_EXTSL_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(s32)ctx->regs[op->rm];
}


void BJX2_Op_SHLL_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rn]<<op->imm;
}

void BJX2_Op_SHLR_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=((u64)ctx->regs[op->rn])>>op->imm;
}

void BJX2_Op_SHAR_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=((s64)ctx->regs[op->rn])>>op->imm;
}

void BJX2_Op_NOT_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=~ctx->regs[op->rn];
}

void BJX2_Op_NEG_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=-ctx->regs[op->rn];
}

//void BJX2_Op_NEG_RegDr(BJX2_Context *ctx, BJX2_Opcode *op)
//{
//	ctx->regs[BJX2_REG_DLR]=-ctx->regs[op->rn];
//}

void BJX2_Op_NEGC_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(~ctx->regs[op->rn])+
		(~(ctx->regs[BJX2_REG_SR]&1));
}

void BJX2_Op_NOT_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=~ctx->regs[op->rm];
}

void BJX2_Op_NEG_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=-ctx->regs[op->rm];
}

void BJX2_Op_ROTL_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		(ctx->regs[op->rn]<<1)|
		(ctx->regs[op->rn]>>31);
}

void BJX2_Op_ROTR_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		(ctx->regs[op->rn]>>1)|
		(ctx->regs[op->rn]<<31);
}

void BJX2_Op_ROTCL_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 sr;
	sr=ctx->regs[BJX2_REG_SR];
	ctx->regs[BJX2_REG_SR]=sr&(~1);
	ctx->regs[BJX2_REG_SR]|=((ctx->regs[op->rn]>>31)&1);
	ctx->regs[op->rn]=
		(ctx->regs[op->rn]<<1) | (sr&1);
}

void BJX2_Op_ROTCR_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 sr;
	sr=ctx->regs[BJX2_REG_SR];
	ctx->regs[BJX2_REG_SR]=sr&(~1);
	ctx->regs[BJX2_REG_SR]|=((ctx->regs[op->rn]   )&1);
	ctx->regs[op->rn]=
		(ctx->regs[op->rn]>>1) | ((sr&1)<<31);
}

void BJX2_Op_SHLL_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_SR]&=(~1);
	ctx->regs[BJX2_REG_SR]|=((ctx->regs[op->rn]>>31)&1);
	ctx->regs[op->rn]=ctx->regs[op->rn]<<1;
}

void BJX2_Op_SHLR_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_SR]&=(~1);
	ctx->regs[BJX2_REG_SR]|=((ctx->regs[op->rn]    )&1);
	ctx->regs[op->rn]=ctx->regs[op->rn]>>1;
}

void BJX2_Op_SHAR_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_SR]&=(~1);
	ctx->regs[BJX2_REG_SR]|=((ctx->regs[op->rn]>>31)&1);
	ctx->regs[op->rn]=((s32)ctx->regs[op->rn])>>1;
}

void BJX2_Op_MOVT_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(ctx->regs[BJX2_REG_SR]&1);
}

void BJX2_Op_MOVNT_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=!(ctx->regs[BJX2_REG_SR]&1);
}

void BJX2_Op_SWAPB_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		((ctx->regs[op->rn]&0xFF00FF00U)>>8) |
		((ctx->regs[op->rn]&0x00FF00FFU)<<8);
}

void BJX2_Op_SWAPW_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		((ctx->regs[op->rn]&0xFFFF0000U)>>16) |
		((ctx->regs[op->rn]&0x0000FFFFU)<<16);
}


void BJX2_Op_SHAD_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
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
			((s32)ctx->regs[op->rn])>>((-shl)&31);
	}
}

void BJX2_Op_SHLD_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
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
			((u32)ctx->regs[op->rn])>>((-shl)&31);
	}
}


void BJX2_Op_SHADQ_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[op->rm];
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			ctx->regs[op->rn]<<(shl&63);
	}else
	{
		ctx->regs[op->rn]=
			((s64)ctx->regs[op->rn])>>((-shl)&63);
	}
}

void BJX2_Op_SHLDQ_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[op->rm];
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			ctx->regs[op->rn]<<(shl&63);
	}else
	{
		ctx->regs[op->rn]=
			((u64)ctx->regs[op->rn])>>((-shl)&63);
	}
}

void BJX2_Op_CLZ_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 v;
	int n;
	
	v=(u32)ctx->regs[op->rm];

	if(!v)
	{
		ctx->regs[op->rn]=32;
		return;
	}

	n=0;
	if(!(v&0xFFFF0000U))
		{ v<<=16; n+=16; }
	if(!(v&0xFF000000U))
		{ v<<=8; n+=8; }
	if(!(v&0xF0000000U))
		{ v<<=4; n+=4; }
	if(!(v&0xC0000000U))
		{ v<<=2; n+=2; }
	if(!(v&0x80000000U))
		{ v<<=1; n+=1; }
	ctx->regs[op->rn]=n;
}

void BJX2_Op_CLZQ_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v;
	int n;
	
	v=(u64)ctx->regs[op->rm];

	if(!v)
	{
		ctx->regs[op->rn]=64;
		return;
	}

	n=0;
	if(!(v&0xFFFFFFFF00000000ULL))
		{ v<<=32; n+=32; }
	if(!(v&0xFFFF000000000000ULL))
		{ v<<=16; n+=16; }
	if(!(v&0xFF00000000000000ULL))
		{ v<<=8; n+=8; }
	if(!(v&0xF000000000000000ULL))
		{ v<<=4; n+=4; }
	if(!(v&0xC000000000000000ULL))
		{ v<<=2; n+=2; }
	if(!(v&0x8000000000000000ULL))
		{ v<<=1; n+=1; }
	ctx->regs[op->rn]=n;
}

void BJX2_Op_SHAD_RegDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[BJX2_REG_DLR];
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			ctx->regs[op->rm]<<(shl&31);
	}else
	{
		ctx->regs[op->rn]=
			((s32)ctx->regs[op->rm])>>((-shl)&31);
	}
}

void BJX2_Op_SHLD_RegDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[BJX2_REG_DLR];
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			ctx->regs[op->rm]<<(shl&31);
	}else
	{
		ctx->regs[op->rn]=
			((u32)ctx->regs[op->rm])>>((-shl)&31);
	}
}

void BJX2_Op_SHADQ_RegDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[BJX2_REG_DLR];
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			ctx->regs[op->rm]<<(shl&63);
	}else
	{
		ctx->regs[op->rn]=
			((s64)ctx->regs[op->rm])>>((-shl)&63);
	}
}

void BJX2_Op_SHLDQ_RegDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[BJX2_REG_DLR];
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			ctx->regs[op->rm]<<(shl&63);
	}else
	{
		ctx->regs[op->rn]=
			((u64)ctx->regs[op->rm])>>((-shl)&63);
	}
}

void BJX2_Op_SHAD_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)(op->imm);
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			((s32)ctx->regs[op->rm])<<(shl&31);
	}else
	{
		ctx->regs[op->rn]=
			((s32)ctx->regs[op->rm])>>((-shl)&31);
	}
}

void BJX2_Op_SHLD_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)(op->imm);
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			((u32)ctx->regs[op->rm])<<(shl&31);
	}else
	{
		ctx->regs[op->rn]=
			((u32)ctx->regs[op->rm])>>((-shl)&31);
	}
}

void BJX2_Op_SHADQ_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)(op->imm);
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			ctx->regs[op->rm]<<(shl&63);
	}else
	{
		ctx->regs[op->rn]=
			((s64)ctx->regs[op->rm])>>((-shl)&63);
	}
}

void BJX2_Op_SHLDQ_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)(op->imm);
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			ctx->regs[op->rm]<<(shl&63);
	}else
	{
		ctx->regs[op->rn]=
			((u64)ctx->regs[op->rm])>>((-shl)&63);
	}
}

void BJX2_Op_SHAD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[op->ro];
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			ctx->regs[op->rm]<<(shl&31);
	}else
	{
		ctx->regs[op->rn]=
			((s32)ctx->regs[op->rm])>>((-shl)&31);
	}
}

void BJX2_Op_SHLD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[op->ro];
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			ctx->regs[op->rm]<<(shl&31);
	}else
	{
		ctx->regs[op->rn]=
			((u32)ctx->regs[op->rm])>>((-shl)&31);
	}
}

void BJX2_Op_SHADQ_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[op->ro];
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			ctx->regs[op->rm]<<(shl&63);
	}else
	{
		ctx->regs[op->rn]=
			((s64)ctx->regs[op->rm])>>((-shl)&63);
	}
}

void BJX2_Op_SHLDQ_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[op->ro];
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			ctx->regs[op->rm]<<(shl&63);
	}else
	{
		ctx->regs[op->rn]=
			((u64)ctx->regs[op->rm])>>((-shl)&63);
	}
}


void BJX2_Op_CSELT_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		(ctx->regs[BJX2_REG_SR]&1)?
			ctx->regs[op->rm]:
			ctx->regs[op->ro];
}
