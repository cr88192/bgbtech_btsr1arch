int BGBCC_JX2RV_NormalizeReg(BGBCC_JX2_Context *ctx, int reg)
{
	if(reg==BGBCC_SH_REG_ZZR)
		return(BGBCC_SH_REG_R0);
	if(reg==BGBCC_SH_REG_LR)
		return(BGBCC_SH_REG_R1);
	if(reg==BGBCC_SH_REG_SP)
		return(BGBCC_SH_REG_R2);
	if(reg==BGBCC_SH_REG_GBR)
		return(BGBCC_SH_REG_R3);
	if(reg==BGBCC_SH_REG_TBR)
		return(BGBCC_SH_REG_R4);
	if(reg==BGBCC_SH_REG_DHR)
		return(BGBCC_SH_REG_R5);
	return(reg);
}

int BGBCC_JX2RV_TryEmitOpRegRegReg(
	BGBCC_JX2_Context *ctx, int nmid, int rs, int rt, int rn)
{
	s64 opw1, opw2, opw3, opw4;
	int i, ex, exw;

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;

	rs=BGBCC_JX2RV_NormalizeReg(ctx, rs);
	rt=BGBCC_JX2RV_NormalizeReg(ctx, rt);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	switch(nmid)
	{
	case BGBCC_SH_NMID_ADD:
		opw1=0x00000033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;
	case BGBCC_SH_NMID_SUB:
		opw1=0x40000033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;
	case BGBCC_SH_NMID_SHADQ:
	case BGBCC_SH_NMID_SHLDQ:
		opw1=0x00001033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_XOR:
		opw1=0x00004033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;
	case BGBCC_SH_NMID_SHARQ:
		opw1=0x40005033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
	case BGBCC_SH_NMID_SHLRQ:
		opw1=0x00005033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;
	case BGBCC_SH_NMID_OR:
		opw1=0x00006033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;
	case BGBCC_SH_NMID_AND:
		opw1=0x00007033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;


	case BGBCC_SH_NMID_ADDSL:
		opw1=0x0000003B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;
	case BGBCC_SH_NMID_SUBSL:
		opw1=0x4000003B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;
	case BGBCC_SH_NMID_SHAD:
	case BGBCC_SH_NMID_SHLD:
		opw1=0x0000103B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_SHAR:
		opw1=0x4000503B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
	case BGBCC_SH_NMID_SHLR:
		opw1=0x0000503B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;
	}

	if(opw1>0)
	{
		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_TryEmitOpRegImmReg(
	BGBCC_JX2_Context *ctx, int nmid, int rm, s64 imm, int rn)
{
	s64 opw1, opw2, opw3, opw4;
	int i, ex, exw, nm1;

	nm1=-1;
	if(nmid==BGBCC_SH_NMID_SUB)
		nm1=BGBCC_SH_NMID_ADD;
	if(nmid==BGBCC_SH_NMID_SUBSL)
		nm1=BGBCC_SH_NMID_ADDSL;

	if((nmid==BGBCC_SH_NMID_SHAD) && (imm<0))
		nm1=BGBCC_SH_NMID_SHAR;
	if((nmid==BGBCC_SH_NMID_SHADQ) && (imm<0))
		nm1=BGBCC_SH_NMID_SHARQ;

	if(nm1>0)
	{
		return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			nm1, rm, -imm, rn));
	}

	rm=BGBCC_JX2RV_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;

	switch(nmid)
	{
	case BGBCC_SH_NMID_ADD:
		opw1=0x00000013|((rn&31)<<7)|((rm&31)<<15)|(imm<<20);
		break;
	case BGBCC_SH_NMID_SHADQ:
	case BGBCC_SH_NMID_SHLDQ:
		opw1=0x00001013|((rn&31)<<7)|((rm&31)<<15)|(imm<<20);
		break;

	case BGBCC_SH_NMID_XOR:
		opw1=0x00004013|((rn&31)<<7)|((rm&31)<<15)|(imm<<20);
		break;
	case BGBCC_SH_NMID_SHARQ:
		opw1=0x40005013|((rn&31)<<7)|((rm&31)<<15)|(imm<<20);
	case BGBCC_SH_NMID_SHLRQ:
		opw1=0x00005013|((rn&31)<<7)|((rm&31)<<15)|(imm<<20);
		break;
	case BGBCC_SH_NMID_OR:
		opw1=0x00006013|((rn&31)<<7)|((rm&31)<<15)|(imm<<20);
		break;
	case BGBCC_SH_NMID_AND:
		opw1=0x00007013|((rn&31)<<7)|((rm&31)<<15)|(imm<<20);
		break;


	case BGBCC_SH_NMID_ADDSL:
		opw1=0x0000001B|((rn&31)<<7)|((rm&31)<<15)|(imm<<20);
		break;
	case BGBCC_SH_NMID_SHAD:
	case BGBCC_SH_NMID_SHLD:
		opw1=0x0000101B|((rn&31)<<7)|((rm&31)<<15)|(imm<<20);
		break;

	case BGBCC_SH_NMID_SHAR:
		opw1=0x4000501B|((rn&31)<<7)|((rm&31)<<15)|(imm<<20);
	case BGBCC_SH_NMID_SHLR:
		opw1=0x0000501B|((rn&31)<<7)|((rm&31)<<15)|(imm<<20);
		break;

	case BGBCC_SH_NMID_JSR:
		opw1=0x00000067|((rn&31)<<7)|((rm&31)<<15)|(imm<<20);
		break;
	}

	if(opw1>0)
	{
		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_TryEmitOpRegStRegDisp(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int rn, int disp)
{
	s64 opw1, opw2, opw3, opw4;
	u32 dispm;
	int odr, ex, ex2, nowxi, exw;
	int i, j, k;

	rm=BGBCC_JX2RV_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;

	dispm=((disp&0xFE0)<<20)|((disp&31)<<7);

	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
	case BGBCC_SH_NMID_MOVUB:
		opw1=0x00000023|((rn&31)<<15)|((rm&31)<<20)|dispm;
		break;
	case BGBCC_SH_NMID_MOVW:
	case BGBCC_SH_NMID_MOVUW:
		opw1=0x00001023|((rn&31)<<15)|((rm&31)<<20)|dispm;
		break;
	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVUL:
		opw1=0x00002023|((rn&31)<<15)|((rm&31)<<20)|dispm;
		break;
	case BGBCC_SH_NMID_MOVQ:
		opw1=0x00003023|((rn&31)<<15)|((rm&31)<<20)|dispm;
		break;
	case BGBCC_SH_NMID_MOVX2:
		opw1=0x00007023|((rn&31)<<15)|((rm&31)<<20)|dispm;
		break;
	}



	if(opw1>0)
	{
		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_TryEmitOpLdRegDispReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int disp, int rn)
{
	s64 opw1, opw2, opw3, opw4;
	int odr, ex, ex2, nowxi, exw;
	int i, j, k;

	rm=BGBCC_JX2RV_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;

	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		opw1=0x00000003|((rn&31)<<7)|((rm&31)<<15)|(disp<<20);
		break;
	case BGBCC_SH_NMID_MOVW:
		opw1=0x00001003|((rn&31)<<7)|((rm&31)<<15)|(disp<<20);
		break;
	case BGBCC_SH_NMID_MOVL:
		opw1=0x00002003|((rn&31)<<7)|((rm&31)<<15)|(disp<<20);
		break;
	case BGBCC_SH_NMID_MOVQ:
		opw1=0x00003003|((rn&31)<<7)|((rm&31)<<15)|(disp<<20);
		break;
	case BGBCC_SH_NMID_MOVUB:
		opw1=0x00004003|((rn&31)<<7)|((rm&31)<<15)|(disp<<20);
		break;
	case BGBCC_SH_NMID_MOVUW:
		opw1=0x00005003|((rn&31)<<7)|((rm&31)<<15)|(disp<<20);
		break;
	case BGBCC_SH_NMID_MOVUL:
		opw1=0x00006003|((rn&31)<<7)|((rm&31)<<15)|(disp<<20);
		break;
	case BGBCC_SH_NMID_MOVX2:
		opw1=0x00007003|((rn&31)<<7)|((rm&31)<<15)|(disp<<20);
		break;
	}

	if(opw1>0)
	{
		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_TryEmitOpRegStReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	return(BGBCC_JX2RV_TryEmitOpRegStRegDisp(ctx, nmid, rm, rn, 0));
}

int BGBCC_JX2RV_TryEmitOpLdRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	return(BGBCC_JX2RV_TryEmitOpLdRegDispReg(ctx, nmid, rm, 0, rn));
}

int BGBCC_JX2RV_TryEmitOpLblReg(BGBCC_JX2_Context *ctx,
	int nmid, int lbl, int reg)
{
	s64 opw1, opw2, opw3, opw4;
	int odr, ex, ex2, nowxi, exw, rlty;
	int i, j, k;

	reg=BGBCC_JX2RV_NormalizeReg(ctx, reg);

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;
	rlty=-1;

	switch(nmid)
	{
	case BGBCC_SH_NMID_BSR:
//		opw1=0x00000003|((reg&31)<<7)|((rm&31)<<15)|(disp<<20);
//		opw1=0xFFDFF06F|((reg&31)<<7);
		opw1=0x0000006F|((reg&31)<<7);
		rlty=BGBCC_SH_RLC_RELW20_RVI;
		break;
	}

	if(opw1>0)
	{
		if(rlty>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty); }

		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_TryEmitOpRegLbl(BGBCC_JX2_Context *ctx,
	int nmid, int lbl, int reg)
{
	return(0);
}

int BGBCC_JX2RV_TryEmitOpRegRegLbl(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn, int lbl)
{
	s64 opw1, opw2, opw3, opw4;
	int odr, ex, ex2, nowxi, exw, rlty;
	int i, j, k;

	rm=BGBCC_JX2RV_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;
	rlty=-1;

	switch(nmid)
	{
	case BGBCC_SH_NMID_BREQ:
	case BGBCC_SH_NMID_BREQL:
//		opw1=0x00000063|((rn&31)<<15)|((rm&31)<<20);
		opw1=0x00000063|((rn&31)<<15)|((rm&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;
	case BGBCC_SH_NMID_BRNE:
	case BGBCC_SH_NMID_BRNEL:
//		opw1=0xFE001EE3|((rn&31)<<15)|((rm&31)<<20);
		opw1=0x00001063|((rn&31)<<15)|((rm&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;

	case BGBCC_SH_NMID_BRLT:
	case BGBCC_SH_NMID_BRLTL:
		opw1=0x00004063|((rn&31)<<15)|((rm&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;
	case BGBCC_SH_NMID_BRGE:
	case BGBCC_SH_NMID_BRGEL:
		opw1=0x00005063|((rn&31)<<15)|((rm&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;

	case BGBCC_SH_NMID_BRGT:
	case BGBCC_SH_NMID_BRGTL:
		opw1=0x00004063|((rm&31)<<15)|((rn&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;
	case BGBCC_SH_NMID_BRLE:
	case BGBCC_SH_NMID_BRLEL:
		opw1=0x00005063|((rm&31)<<15)|((rn&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;

	case BGBCC_SH_NMID_BRLTU:
		opw1=0x00006063|((rn&31)<<15)|((rm&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;
	case BGBCC_SH_NMID_BRGEU:
		opw1=0x00007063|((rn&31)<<15)|((rm&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;

	case BGBCC_SH_NMID_BRGTU:
		opw1=0x00006063|((rm&31)<<15)|((rn&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;
	case BGBCC_SH_NMID_BRLEU:
		opw1=0x00007063|((rm&31)<<15)|((rn&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;
	}

	if(opw1>0)
	{
		if(rlty>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty); }

		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_TryEmitOpLabel(BGBCC_JX2_Context *ctx, int nmid, int lbl)
{
	if(nmid==BGBCC_SH_NMID_BRA)
		return(BGBCC_JX2RV_TryEmitOpLblReg(ctx,
			BGBCC_SH_NMID_BSR, lbl, BGBCC_SH_REG_ZZR));

	if(nmid==BGBCC_SH_NMID_BSR)
		return(BGBCC_JX2RV_TryEmitOpLblReg(ctx,
			BGBCC_SH_NMID_BSR, lbl, BGBCC_SH_REG_LR));

	return(0);
}

int BGBCC_JX2RV_TryEmitOpNone(BGBCC_JX2_Context *ctx, int nmid)
{
	if(nmid==BGBCC_SH_NMID_RTS)
		return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_JSR, BGBCC_SH_REG_LR, 0, BGBCC_SH_REG_ZZR));

	if(nmid==BGBCC_SH_NMID_NOP)
		return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_ADD, BGBCC_SH_REG_ZZR, 0, BGBCC_SH_REG_ZZR));

	return(0);
}

int BGBCC_JX2RV_TryEmitOpReg(BGBCC_JX2_Context *ctx, int nmid, int reg)
{
	int opw1, opw2, opw3, reg2, ex, ex2, exw;
	int i;

	if(nmid==BGBCC_SH_NMID_JSR)
		return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_JSR, reg, 0, BGBCC_SH_REG_LR));
	if(nmid==BGBCC_SH_NMID_JMP)
		return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_JSR, reg, 0, BGBCC_SH_REG_ZZR));

	return(0);
}

int BGBCC_JX2RV_TryEmitOpImm(BGBCC_JX2_Context *ctx, int nmid, int imm)
{
	int opw1, opw2, odr;

	return(0);
}

int BGBCC_JX2RV_TryEmitOpRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw1, opw2, ex, ex3r, exw;
	int rm3, rn3;

	if(
		(nmid==BGBCC_SH_NMID_ADD)	||
		(nmid==BGBCC_SH_NMID_SUB)	||
		(nmid==BGBCC_SH_NMID_ADDSL)	||
		(nmid==BGBCC_SH_NMID_SUBSL)	||
		(nmid==BGBCC_SH_NMID_AND)	||
		(nmid==BGBCC_SH_NMID_OR)	||
		(nmid==BGBCC_SH_NMID_XOR)	)
	{
		return(BGBCC_JX2RV_TryEmitOpRegRegReg(ctx, nmid, rn, rm, rn));
	}

	return(0);
}

int BGBCC_JX2RV_TryEmitOpImmReg(BGBCC_JX2_Context *ctx,
	int nmid, s64 imm, int reg)
{
	if(
		(nmid==BGBCC_SH_NMID_ADD)	||
		(nmid==BGBCC_SH_NMID_SUB)	||
		(nmid==BGBCC_SH_NMID_ADDSL)	||
		(nmid==BGBCC_SH_NMID_SUBSL)	||
		(nmid==BGBCC_SH_NMID_AND)	||
		(nmid==BGBCC_SH_NMID_OR)	||
		(nmid==BGBCC_SH_NMID_XOR)	)
	{
		return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx, nmid, reg, imm, reg));
	}

	if(nmid==BGBCC_SH_NMID_JSR)
		return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_JSR, reg, imm, BGBCC_SH_REG_LR));
	if(nmid==BGBCC_SH_NMID_JMP)
		return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_JSR, reg, imm, BGBCC_SH_REG_ZZR));

	return(0);
}

int BGBCC_JX2RV_TryEmitOpRegRegRegReg(
	BGBCC_JX2_Context *ctx, int nmid, int rs, int rt, int rp, int rn)
{
	int opw1, opw2, opw3, opw4;
	int i, ex, exw;

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;

	return(0);
}

int BGBCC_JX2RV_TryEmitOpRegRegImmReg(
	BGBCC_JX2_Context *ctx, int nmid, int rs, int rt, int imm, int rn)
{
	return(0);
}

int BGBCC_JX2RV_TryEmitOpRegStReg2(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int ro, int rn)
{
	int opw1, opw2, opw3, opw4, ex, exw;
	return(0);
}

int BGBCC_JX2RV_TryEmitOpLdReg2Reg(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int ro, int rn)
{
	int opw1, opw2, opw3, opw4, ex, exw;
	return(0);
}


