void BJX2_Op_FADD_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double a, b, c;
	
	a=((double *)ctx->fpreg)[op->rn];
	b=((double *)ctx->fpreg)[op->rm];
	c=a+b;
	((double *)ctx->fpreg)[op->rn]=c;
}

void BJX2_Op_FSUB_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double a, b, c;
	
	a=((double *)ctx->fpreg)[op->rn];
	b=((double *)ctx->fpreg)[op->rm];
	c=a-b;
	((double *)ctx->fpreg)[op->rn]=c;
}

void BJX2_Op_FMUL_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double a, b, c;
	
	a=((double *)ctx->fpreg)[op->rn];
	b=((double *)ctx->fpreg)[op->rm];
	c=a*b;
	((double *)ctx->fpreg)[op->rn]=c;
}

void BJX2_Op_FDIV_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double a, b, c;
	
	a=((double *)ctx->fpreg)[op->rn];
	b=((double *)ctx->fpreg)[op->rm];
	c=a/b;
	((double *)ctx->fpreg)[op->rn]=c;
}

void BJX2_Op_FCMPEQ_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double a, b, c;
	
	a=((double *)ctx->fpreg)[op->rn];
	b=((double *)ctx->fpreg)[op->rm];

	if(a==b)
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_FCMPGT_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double a, b, c;
	
	a=((double *)ctx->fpreg)[op->rn];
	b=((double *)ctx->fpreg)[op->rm];

	if(a>b)
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_FMOV_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->fpreg[op->rn]=ctx->fpreg[op->rm];
}

void BJX2_Op_FMOV_FRegGReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->fpreg[op->rm];
}

void BJX2_Op_FMOV_GRegFReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->fpreg[op->rn]=ctx->regs[op->rm];
}


void BJX2_Op_FNEG_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->fpreg[op->rn]=ctx->fpreg[op->rn]^0x8000000000000000ULL;
}

void BJX2_Op_FABS_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->fpreg[op->rn]=ctx->fpreg[op->rn]&0x7FFFFFFFFFFFFFFFULL;
}

void BJX2_Op_FNEG_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->fpreg[op->rn]=ctx->fpreg[op->rm]^0x8000000000000000ULL;
}

void BJX2_Op_FABS_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->fpreg[op->rn]=ctx->fpreg[op->rm]&0x7FFFFFFFFFFFFFFFULL;
}


void BJX2_Op_FADD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double a, b, c;
	
	a=((double *)ctx->fpreg)[op->rm];
	b=((double *)ctx->fpreg)[op->ro];
	c=a+b;
	((double *)ctx->fpreg)[op->rn]=c;
}

void BJX2_Op_FSUB_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double a, b, c;
	
	a=((double *)ctx->fpreg)[op->rm];
	b=((double *)ctx->fpreg)[op->ro];
	c=a-b;
	((double *)ctx->fpreg)[op->rn]=c;
}

void BJX2_Op_FMUL_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double a, b, c;
	
	a=((double *)ctx->fpreg)[op->rm];
	b=((double *)ctx->fpreg)[op->ro];
	c=a*b;
	((double *)ctx->fpreg)[op->rn]=c;
}

void BJX2_Op_FDIV_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double a, b, c;
	
	a=((double *)ctx->fpreg)[op->rm];
	b=((double *)ctx->fpreg)[op->ro];
	c=a/b;
	((double *)ctx->fpreg)[op->rn]=c;
}

#if 1
void BJX2_Op_FADDD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double a, b, c;
	
	a=((double *)ctx->regs)[op->rm];
	b=((double *)ctx->regs)[op->ro];
	c=a+b;
	((double *)ctx->regs)[op->rn]=c;
}

void BJX2_Op_FSUBD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double a, b, c;
	
	a=((double *)ctx->regs)[op->rm];
	b=((double *)ctx->regs)[op->ro];
	c=a-b;
	((double *)ctx->regs)[op->rn]=c;
}

void BJX2_Op_FMULD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double a, b, c;
	
	a=((double *)ctx->regs)[op->rm];
	b=((double *)ctx->regs)[op->ro];
	c=a*b;
	((double *)ctx->regs)[op->rn]=c;
}
#endif

void BJX2_Op_FMOVS_RegStReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float sf;
	u32 v;
	sf=((double *)ctx->fpreg)[op->rm];
	v=*(u32 *)(&sf);
	BJX2_MemSetDWord(ctx, ctx->regs[op->rn], v);
}

void BJX2_Op_FMOVS_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 v;
	v=BJX2_MemGetDWord(ctx, ctx->regs[op->rm]);
	((double *)ctx->fpreg)[op->rn]=*(float *)(&v);
}

void BJX2_Op_FMOVD_RegStReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	BJX2_MemSetQWord(ctx, ctx->regs[op->rn], ctx->fpreg[op->rm]);
}

void BJX2_Op_FMOVD_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->fpreg[op->rn]=BJX2_MemGetQWord(ctx, ctx->regs[op->rm]);
}


void BJX2_Op_FMOVS_RegStDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float sf;
	u32 v;
	sf=((double *)ctx->fpreg)[op->rm];
	v=*(u32 *)(&sf);
	BJX2_MemSetDWord(ctx, ctx->regs[op->rn]+(ctx->regs[BJX2_REG_DR]*4), v);
}

void BJX2_Op_FMOVS_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 v;
	v=BJX2_MemGetDWord(ctx, ctx->regs[op->rm]+(ctx->regs[BJX2_REG_DR]*4));
	((double *)ctx->fpreg)[op->rn]=*(float *)(&v);
}

void BJX2_Op_FMOVD_RegStDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	BJX2_MemSetQWord(ctx, ctx->regs[op->rn]+
		(ctx->regs[BJX2_REG_DR]*8), ctx->fpreg[op->rm]);
}

void BJX2_Op_FMOVD_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->fpreg[op->rn]=BJX2_MemGetQWord(ctx,
		ctx->regs[op->rm]+(ctx->regs[BJX2_REG_DR]*8));
}


void BJX2_Op_FMOVS_RegStRegDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float sf;
	u32 v;
	sf=((double *)ctx->fpreg)[op->rm];
	v=*(u32 *)(&sf);
	BJX2_MemSetDWord(ctx, ctx->regs[op->rn]+(op->imm*4), v);
}

void BJX2_Op_FMOVS_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 v;
	v=BJX2_MemGetDWord(ctx, ctx->regs[op->rm]+(op->imm*4));
	((double *)ctx->fpreg)[op->rn]=*(float *)(&v);
}

void BJX2_Op_FMOVD_RegStRegDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	BJX2_MemSetQWord(ctx, ctx->regs[op->rn]+
		(op->imm*8), ctx->fpreg[op->rm]);
}

void BJX2_Op_FMOVD_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->fpreg[op->rn]=BJX2_MemGetQWord(ctx,
		ctx->regs[op->rm]+(op->imm*8));
}


void BJX2_Op_FMOVS_RegStReg2(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float sf;
	u32 v;
	sf=((double *)ctx->fpreg)[op->rm];
	v=*(u32 *)(&sf);
	BJX2_MemSetDWord(ctx, ctx->regs[op->rn]+(ctx->regs[op->ro]*4), v);
}

void BJX2_Op_FMOVS_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 v;
	v=BJX2_MemGetDWord(ctx, ctx->regs[op->rm]+(ctx->regs[op->ro]*4));
	((double *)ctx->fpreg)[op->rn]=*(float *)(&v);
}

void BJX2_Op_FMOVD_RegStReg2(BJX2_Context *ctx, BJX2_Opcode *op)
{
	BJX2_MemSetQWord(ctx, ctx->regs[op->rn]+
		(ctx->regs[op->ro]*8), ctx->fpreg[op->rm]);
}

void BJX2_Op_FMOVD_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->fpreg[op->rn]=BJX2_MemGetQWord(ctx,
		ctx->regs[op->rm]+(ctx->regs[op->ro]*8));
}


void BJX2_Op_FMOVS_RegStPcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float sf;
	u32 v;
	sf=((double *)ctx->fpreg)[op->rm];
	v=*(u32 *)(&sf);
	BJX2_MemSetDWord(ctx, (op->pc2)+(ctx->regs[op->ro]), v);
}

void BJX2_Op_FMOVS_LdPcRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 v;
	v=BJX2_MemGetDWord(ctx, (op->pc2)+(ctx->regs[op->ro]));
	((double *)ctx->fpreg)[op->rn]=*(float *)(&v);
}

void BJX2_Op_FMOVD_RegStPcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	BJX2_MemSetQWord(ctx, (op->pc2)+
		(ctx->regs[op->ro]), ctx->fpreg[op->rm]);
}

void BJX2_Op_FMOVD_LdPcRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->fpreg[op->rn]=BJX2_MemGetQWord(ctx,
		(op->pc2)+(ctx->regs[op->ro]));
}


void BJX2_Op_FLDCF_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 v;
	v=ctx->regs[BJX2_REG_DR];
	((double *)ctx->fpreg)[op->rn]=*(float *)(&v);
}

void BJX2_Op_FSTCF_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float sf;
	u32 v;
	sf=((double *)ctx->fpreg)[op->rn];
	v=*(u32 *)(&sf);
	ctx->regs[BJX2_REG_DR]=v;
}

void BJX2_Op_FLDCI_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v;
	v=ctx->regs[BJX2_REG_DR];
	((double *)ctx->fpreg)[op->rn]=(s64)v;
}

void BJX2_Op_FSTCI_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double f;
	f=((double *)ctx->fpreg)[op->rn];
	ctx->regs[BJX2_REG_DR]=(s64)f;
}

u32 BJX2_CvtHalfToFloat(u16 v)
{
	u32 v1;
	int ex, fr, sg;
	
	if(!v)
		return(0);

	sg=(v>>15)&1;
	ex=(v>>10)&31;
	fr=v&1023;
	ex=(ex-15)+127;
	fr=fr<<13;
	v1=(sg<<31)|(ex<<23)|fr;
	return(v1);
}

u16 BJX2_CvtFloatToHalf(u32 v)
{
	u32 v1;
	int ex, fr, sg;

	if(!v)
		return(0);

	sg=(v>>31)&1;
	ex=(v>>23)&255;
	fr=v&0x7FFFFF;
	ex=(ex-127)+15;
	fr=fr>>13;
	
	if(ex<0)
		return(0);
	if(ex>=15)
		return(0x7C00|(sg<<15));
	v1=(sg<<15)|(ex<<10)|fr;
	return(v1);
}

void BJX2_Op_FLDCH_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 v;
	v=ctx->regs[BJX2_REG_DR];
	v=BJX2_CvtHalfToFloat(v);
	((double *)ctx->fpreg)[op->rn]=*(float *)(&v);
}

void BJX2_Op_FSTCH_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float sf;
	u32 v;
	sf=((double *)ctx->fpreg)[op->rn];
	v=*(u32 *)(&sf);
	v=BJX2_CvtFloatToHalf(v);
	ctx->regs[BJX2_REG_DR]=v;
}


void BJX2_Op_FLDCD_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v;
//	u32 v0, v1;
//	v0=ctx->regs[BJX2_REG_DLR];
//	v1=ctx->regs[BJX2_REG_DHR];
//	v=(((u64)v1)<<32)|v0;
	v=ctx->regs[BJX2_REG_DLR];
	ctx->fpreg[op->rn]=v;
//	((double *)ctx->fpreg)[op->rn]=*(double *)(&v);
}

void BJX2_Op_FSTCD_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	double f;
	u64 v;
//	f=((double *)ctx->fpreg)[op->rn];
//	v=*(u64 *)(&f);
	v=ctx->fpreg[op->rn];
	ctx->regs[BJX2_REG_DLR]=v;
//	ctx->regs[BJX2_REG_DHR]=v>>32;
}


void BJX2_Op_FLDCF_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 v;
	v=ctx->regs[op->rm];
	((double *)ctx->fpreg)[op->rn]=*(float *)(&v);
}

void BJX2_Op_FSTCF_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float sf;
	u32 v;
	sf=((double *)ctx->fpreg)[op->rm];
	v=*(u32 *)(&sf);
	ctx->regs[op->rn]=v;
}

void BJX2_Op_FLDCI_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v;
	v=ctx->regs[op->rm];
	((double *)ctx->fpreg)[op->rn]=(s64)v;
}

void BJX2_Op_FSTCI_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double f;
	f=((double *)ctx->fpreg)[op->rm];
	ctx->regs[op->rn]=(s64)f;
}

void BJX2_Op_FLDCH_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 v;
	v=ctx->regs[op->rm];
	v=BJX2_CvtHalfToFloat(v);
	((double *)ctx->fpreg)[op->rn]=*(float *)(&v);
}

void BJX2_Op_FSTCH_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float sf;
	u32 v;
	sf=((double *)ctx->fpreg)[op->rm];
	v=*(u32 *)(&sf);
	v=BJX2_CvtFloatToHalf(v);
	ctx->regs[op->rn]=v;
}


void BJX2_Op_FLDCD_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v;
	v=ctx->regs[op->rm];
	ctx->fpreg[op->rn]=v;
}

void BJX2_Op_FSTCD_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v;
	v=ctx->fpreg[op->rm];
	ctx->regs[op->rn]=v;
}

#if 1
void BJX2_Op_FLDCF_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 v;
	v=ctx->regs[op->rm];
	((double *)ctx->regs)[op->rn]=*(float *)(&v);
}

void BJX2_Op_FSTCF_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float sf;
	u32 v;
	sf=((double *)ctx->regs)[op->rm];
	v=*(u32 *)(&sf);
	ctx->regs[op->rn]=v;
}

void BJX2_Op_FLDCI_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v;
	v=ctx->regs[op->rm];
	((double *)ctx->regs)[op->rn]=(s64)v;
}

void BJX2_Op_FSTCI_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double f;
	f=((double *)ctx->regs)[op->rm];
	ctx->regs[op->rn]=(s64)f;
}

void BJX2_Op_FLDCH_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 v;
	v=ctx->regs[op->rm];
	v=BJX2_CvtHalfToFloat(v);
	((double *)ctx->regs)[op->rn]=*(float *)(&v);
}

void BJX2_Op_FSTCH_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float sf;
	u32 v;
	sf=((double *)ctx->regs)[op->rm];
	v=*(u32 *)(&sf);
	v=BJX2_CvtFloatToHalf(v);
	ctx->regs[op->rn]=v;
}

void BJX2_Op_FNEG_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]^0x8000000000000000ULL;
}

void BJX2_Op_FABS_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]&0x7FFFFFFFFFFFFFFFULL;
}

void BJX2_Op_FCMPEQ_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double a, b, c;
	
	a=((double *)ctx->regs)[op->rn];
	b=((double *)ctx->regs)[op->rm];

	if(a==b)
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_FCMPGT_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double a, b, c;
	
	a=((double *)ctx->regs)[op->rn];
	b=((double *)ctx->regs)[op->rm];

	if(a>b)
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}
#endif
