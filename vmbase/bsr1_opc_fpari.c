void BTSR1_Op_FADD_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	double a, b, c;
	
	a=((double *)ctx->fpreg)[op->rn];
	b=((double *)ctx->fpreg)[op->rm];
	c=a+b;
	((double *)ctx->fpreg)[op->rn]=c;
}

void BTSR1_Op_FSUB_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	double a, b, c;
	
	a=((double *)ctx->fpreg)[op->rn];
	b=((double *)ctx->fpreg)[op->rm];
	c=a-b;
	((double *)ctx->fpreg)[op->rn]=c;
}

void BTSR1_Op_FMUL_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	double a, b, c;
	
	a=((double *)ctx->fpreg)[op->rn];
	b=((double *)ctx->fpreg)[op->rm];
	c=a*b;
	((double *)ctx->fpreg)[op->rn]=c;
}

void BTSR1_Op_FDIV_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	double a, b, c;
	
	a=((double *)ctx->fpreg)[op->rn];
	b=((double *)ctx->fpreg)[op->rm];
	c=a/b;
	((double *)ctx->fpreg)[op->rn]=c;
}

void BTSR1_Op_FCMPEQ_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	double a, b, c;
	
	a=((double *)ctx->fpreg)[op->rn];
	b=((double *)ctx->fpreg)[op->rm];

	if(a==b)
		ctx->regs[BTSR1_REG_SR]|=1;
	else
		ctx->regs[BTSR1_REG_SR]&=~1;
}

void BTSR1_Op_FCMPGT_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	double a, b, c;
	
	a=((double *)ctx->fpreg)[op->rn];
	b=((double *)ctx->fpreg)[op->rm];

	if(a>b)
		ctx->regs[BTSR1_REG_SR]|=1;
	else
		ctx->regs[BTSR1_REG_SR]&=~1;
}

void BTSR1_Op_FMOV_RegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->fpreg[op->rn]=ctx->fpreg[op->rm];
}

void BTSR1_Op_FNEG_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->fpreg[op->rn]=ctx->fpreg[op->rn]^0x8000000000000000ULL;
}

void BTSR1_Op_FABS_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->fpreg[op->rn]=ctx->fpreg[op->rn]^0x7FFFFFFFFFFFFFFFULL;
}


void BTSR1_Op_FMOVS_RegStReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	float sf;
	u32 v;
	sf=((double *)ctx->fpreg)[op->rm];
	v=*(u32 *)(&sf);
	BTSR1_MemSetDWord(ctx, ctx->regs[op->rn], v);
}

void BTSR1_Op_FMOVS_LdRegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	u32 v;
	v=BTSR1_MemGetDWord(ctx, ctx->regs[op->rm]);
	((double *)ctx->fpreg)[op->rn]=*(float *)(&v);
}

void BTSR1_Op_FMOVD_RegStReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	BTSR1_MemSetQWord(ctx, ctx->regs[op->rn], ctx->fpreg[op->rm]);
}

void BTSR1_Op_FMOVD_LdRegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->fpreg[op->rn]=(s32)BTSR1_MemGetQWord(ctx, ctx->regs[op->rm]);
}


void BTSR1_Op_FMOVS_RegStDrReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	float sf;
	u32 v;
	sf=((double *)ctx->fpreg)[op->rm];
	v=*(u32 *)(&sf);
	BTSR1_MemSetDWord(ctx, ctx->regs[op->rn]+(ctx->regs[BTSR1_REG_DR]*4), v);
}

void BTSR1_Op_FMOVS_LdDrRegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	u32 v;
	v=BTSR1_MemGetDWord(ctx, ctx->regs[op->rm]+(ctx->regs[BTSR1_REG_DR]*4));
	((double *)ctx->fpreg)[op->rn]=*(float *)(&v);
}

void BTSR1_Op_FMOVD_RegStDrReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	BTSR1_MemSetQWord(ctx, ctx->regs[op->rn]+
		(ctx->regs[BTSR1_REG_DR]*8), ctx->fpreg[op->rm]);
}

void BTSR1_Op_FMOVD_LdDrRegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->fpreg[op->rn]=BTSR1_MemGetQWord(ctx,
		ctx->regs[op->rm]+(ctx->regs[BTSR1_REG_DR]*8));
}


void BTSR1_Op_FLDCF_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	u32 v;
	v=ctx->regs[BTSR1_REG_DR];
	((double *)ctx->fpreg)[op->rn]=*(float *)(&v);
}

void BTSR1_Op_FSTCF_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	float sf;
	u32 v;
	sf=((double *)ctx->fpreg)[op->rn];
	v=*(u32 *)(&sf);
	ctx->regs[BTSR1_REG_DR]=v;
}

void BTSR1_Op_FLDCI_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	u32 v;
	v=ctx->regs[BTSR1_REG_DR];
	((double *)ctx->fpreg)[op->rn]=(s32)v;
}

void BTSR1_Op_FSTCI_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	double f;
	f=((double *)ctx->fpreg)[op->rn];
	ctx->regs[BTSR1_REG_DR]=(s32)f;
}

u32 BTSR1_CvtHalfToFloat(u16 v)
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

u16 BTSR1_CvtFloatToHalf(u32 v)
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

void BTSR1_Op_FLDCH_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	u32 v;
	v=ctx->regs[BTSR1_REG_DR];
	v=BTSR1_CvtHalfToFloat(v);
	((double *)ctx->fpreg)[op->rn]=*(float *)(&v);
}

void BTSR1_Op_FSTCH_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	float sf;
	u32 v;
	sf=((double *)ctx->fpreg)[op->rn];
	v=*(u32 *)(&sf);
	v=BTSR1_CvtFloatToHalf(v);
	ctx->regs[BTSR1_REG_DR]=v;
}


void BTSR1_Op_FLDCD_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	u64 v;
	u32 v0, v1;
	v0=ctx->regs[BTSR1_REG_DLR];
	v1=ctx->regs[BTSR1_REG_DHR];
	v=(((u64)v1)<<32)|v0;
	((double *)ctx->fpreg)[op->rn]=*(double *)(&v);
}

void BTSR1_Op_FSTCD_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	double f;
	u64 v;
	f=((double *)ctx->fpreg)[op->rn];
	v=*(u64 *)(&f);
	ctx->regs[BTSR1_REG_DLR]=v;
	ctx->regs[BTSR1_REG_DHR]=v>>32;
}
