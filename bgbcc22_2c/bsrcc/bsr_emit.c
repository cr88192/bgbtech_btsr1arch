int BGBCC_BSR_EmitCheckRegQuad(BGBCC_BSR_Context *ctx, int reg)
{
	return(0);
}

int BGBCC_BSR_EmitCheckRegExt4(BGBCC_BSR_Context *ctx, int reg)
{
	return(0);
}

int BGBCC_BSR_EmitCheckRegExtGPR(BGBCC_BSR_Context *ctx, int reg)
{
	return(BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg));
//	return(0);
}

int BGBCC_BSR_EmitCheckRegExtAddrGPR(BGBCC_BSR_Context *ctx, int reg)
{
	return(BGBCC_BSR_EmitCheckRegAddrGPR(ctx, reg));
//	return(0);
}

int BGBCC_BSR_EmitCheckRegBaseGPR(BGBCC_BSR_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
		return(1);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0)
	{
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
	{
		return(1);
	}

	return(0);
}

int BGBCC_BSR_EmitCheckRegAddrGPR(BGBCC_BSR_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
		return(1);

	return(0);
}

int BGBCC_BSR_EmitCheckRegNonBaseGPR(BGBCC_BSR_Context *ctx, int reg)
{
	return(0);
}

int BGBCC_BSR_EmitLoadDrImm(
	BGBCC_BSR_Context *ctx, s32 imm)
{
	int opw1, opw2, opw3;

#if 0
	if((imm&255)==imm)
	{
		opw1=0x2400|(imm&255);
		BGBCC_BSR_EmitWord(ctx, opw1);
		return(1);
	}

	if((imm|(~255))==imm)
	{
		opw1=0x2500|(imm&255);
		BGBCC_BSR_EmitWord(ctx, opw1);
		return(1);
	}
#endif

#if 1
	if(1)
	{
		if((imm&4095)==imm)
		{
			opw1=0xA000|(imm&4095);
			BGBCC_BSR_EmitWord(ctx, opw1);
			return(1);
		}

		if((imm|(~4095))==imm)
		{
			opw1=0xB000|(imm&4095);
			BGBCC_BSR_EmitWord(ctx, opw1);
			return(1);
		}
	}
#endif

#if 0
	if((imm&0xFFFFF)==0x00000)
	{
		opw1=0x30A2;
		BGBCC_BSR_EmitLoadDrImm(ctx, imm>>20);
		BGBCC_BSR_EmitWord(ctx, opw1);
		return(1);
	}

	if((imm&0xFFFFF)==0xFFFFF)
	{
		opw1=0x30B2;
		BGBCC_BSR_EmitLoadDrImm(ctx, imm>>20);
		BGBCC_BSR_EmitWord(ctx, opw1);
		return(1);
	}
#endif
	
	if((imm&0xFFFF)==0x0000)
	{
		opw1=0x30A0;
		BGBCC_BSR_EmitLoadDrImm(ctx, imm>>16);
		BGBCC_BSR_EmitWord(ctx, opw1);
		return(1);
	}

	if((imm&0xFFFF)==0xFFFF)
	{
		opw1=0x30B0;
		BGBCC_BSR_EmitLoadDrImm(ctx, imm>>16);
		BGBCC_BSR_EmitWord(ctx, opw1);
		return(1);
	}

#if 0
	if((imm&0xFFF)==0x000)
	{
		opw1=0x3082;
		BGBCC_BSR_EmitLoadDrImm(ctx, imm>>12);
		BGBCC_BSR_EmitWord(ctx, opw1);
		return(1);
	}

	if((imm&0xFFF)==0xFFF)
	{
		opw1=0x3092;
		BGBCC_BSR_EmitLoadDrImm(ctx, imm>>12);
		BGBCC_BSR_EmitWord(ctx, opw1);
		return(1);
	}
#endif

	opw1=0x2600|(imm&255);
	BGBCC_BSR_EmitLoadDrImm(ctx, imm>>8);
	BGBCC_BSR_EmitWord(ctx, opw1);
	return(1);
}

int BGBCC_BSR_EmitLoadRegImm(
	BGBCC_BSR_Context *ctx, int nmid, int reg, s32 imm)
{
	int opw1, opw2, opw3;

	if(BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))
	{
		if(((sbyte)imm)==imm)
		{
			opw1=0xE000|((reg&15)<<8)|(imm&255);
			BGBCC_BSR_EmitWord(ctx, opw1);
			return(1);
		}

//		opw1=0xD000|((reg&15)<<8)|(imm&255);
//		BGBCC_BSR_EmitLoadRegImm(ctx, nmid, reg, imm>>8);

		opw1=0x4800|((reg&15)<<4)|(imm&15);
		BGBCC_BSR_EmitLoadDrImm(ctx, imm>>4);

		BGBCC_BSR_EmitWord(ctx, opw1);
		return(1);
	}

	BGBCC_DBGBREAK
	return(0);
}

int BGBCC_BSR_EmitLoadRegImm64P(
	BGBCC_BSR_Context *ctx, int reg, s64 imm)
{
	s32 lo, hi;
	if(BGBCC_BSRC_EmitRegIsLpReg(NULL, ctx, reg))
	{
		lo=imm;		hi=imm>>32;
		return(BGBCC_BSR_EmitLoadRegImmPair32(ctx, reg, lo, hi));
	}

	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_BSR_EmitLoadRegImmPair32(
	BGBCC_BSR_Context *ctx, int reg, s32 imma, s32 immb)
{
	int reg2;

	if(BGBCC_BSRC_EmitRegIsLpReg(NULL, ctx, reg))
	{
		reg2=BGBCC_SH_REG_R0+(reg&15);
		BGBCC_BSR_EmitLoadRegImm(ctx, 0, reg2+0, imma);
		BGBCC_BSR_EmitLoadRegImm(ctx, 0, reg2+1, immb);
		return(1);
	}

	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_BSR_EmitCheckFlushIndexImmP(BGBCC_BSR_Context *ctx)
{
	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_BSR_EmitCheckFlushIndexImm(BGBCC_BSR_Context *ctx)
{
	return(0);
}

int BGBCC_BSR_EmitForceFlushIndexImm(BGBCC_BSR_Context *ctx)
{
	return(0);
}

int BGBCC_BSR_EmitFlushIndexImmBasic(BGBCC_BSR_Context *ctx)
{
	return(0);
}

int BGBCC_BSR_EmitLoadRegLabelRel(
	BGBCC_BSR_Context *ctx, int reg, int lbl, int ofs)
{
	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_BSR_EmitLoadRegLabelAbs(
	BGBCC_BSR_Context *ctx, int reg, int lbl)
{
	BGBCC_BSR_EmitOpLblReg(ctx, BGBCC_SH_NMID_MOV, lbl, reg);
//	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_BSR_EmitLoadRegLabelRel32(
	BGBCC_BSR_Context *ctx, int reg, int lbl, int ofs)
{
	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_BSR_EmitLoadRegLabelRel16(
	BGBCC_BSR_Context *ctx, int reg, int lbl, int ofs)
{
	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_BSR_EmitLoadRegLabelRel24(
	BGBCC_BSR_Context *ctx, int reg, int lbl)
{
	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_BSR_EmitLoadRegLabelVarRel24(
	BGBCC_BSR_Context *ctx, int nmid, int reg, int lbl)
{
	int opw1, opw2, opw3, opw4, opwf, rlty, prlty;

	if(BGBCC_BSR_EmitCheckAutoLabelNear8(ctx, lbl))
		{ prlty=BGBCC_SH_RLC_RELW8_BSR;}
	else if(BGBCC_BSR_ModelIsLabel16P(ctx))
	{
//		prlty=BGBCC_SH_RLC_RELW16C_BSR;
		prlty=BGBCC_SH_RLC_RELW16A_BSR;
		if((reg&15)==15)
			prlty=BGBCC_SH_RLC_RELW16C_BSR;
	}
	else
		{ prlty=BGBCC_SH_RLC_RELW24C_BSR; }

	opw1=-1; opw2=-1; opw3=-1; opw4=-1; rlty=-1;
	opwf=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVUB:
		if(prlty==BGBCC_SH_RLC_RELW16A_BSR)
			{ rlty=prlty; opwf=0x4300|((reg&15)<<4); break; }
		rlty=prlty;
		opwf=0x3007|((reg&15)<<4);
		break;
	case BGBCC_SH_NMID_MOVB:
		if(prlty==BGBCC_SH_RLC_RELW16A_BSR)
			{ rlty=prlty; opwf=0x4400|((reg&15)<<4); break; }
		rlty=prlty;
		opwf=0x300C|((reg&15)<<4);
		break;
	case BGBCC_SH_NMID_MOVW:
		if(prlty==BGBCC_SH_RLC_RELW16A_BSR)
			{ rlty=prlty; opwf=0x4500|((reg&15)<<4); break; }
		rlty=prlty;
		opwf=0x300D|((reg&15)<<4);
		break;
	case BGBCC_SH_NMID_MOVL:
		if(prlty==BGBCC_SH_RLC_RELW16A_BSR)
			{ rlty=prlty; opwf=0x4600|((reg&15)<<4); break; }
		rlty=prlty;
		opwf=0x300E|((reg&15)<<4);
		break;
	case BGBCC_SH_NMID_MOVUW:
		if(prlty==BGBCC_SH_RLC_RELW16A_BSR)
			{ rlty=prlty; opwf=0x4700|((reg&15)<<4); break; }
		rlty=prlty;
		opwf=0x300F|((reg&15)<<4);
		break;
	}
	
	if((rlty>=0) && (opw1<0) && (opwf>=0))
	{
//		if(rlty==BGBCC_SH_RLC_RELW8C_BSR)
//			{ opw1=0x25FE; opw2=opwf; }
		if(rlty==BGBCC_SH_RLC_RELW16C_BSR)
			{ opw1=0x25FF; opw2=0x26FE; opw3=opwf; }
		if(rlty==BGBCC_SH_RLC_RELW24C_BSR)
			{ opw1=0x25FF; opw2=0x26FF; opw3=0x26FE; opw4=opwf; }
		if(rlty==BGBCC_SH_RLC_RELW16A_BSR)
			{ opw1=0xA000; opw2=opwf; }
	}

	if(opw1>=0)
	{
		if(rlty>0)
			{ BGBCC_BSR_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_BSR_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_BSR_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_BSR_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_BSR_EmitWord(ctx, opw4);
		return(1);
	}

	BGBCC_DBGBREAK
	return(0);

//	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
//	return(0);
}

int BGBCC_BSR_EmitStoreRegLabelVarRel24(
	BGBCC_BSR_Context *ctx, int nmid, int reg, int lbl)
{
	int opw1, opw2, opw3, opw4, opwf, rlty, prlty;

	if(BGBCC_BSR_EmitCheckAutoLabelNear8(ctx, lbl))
		{ prlty=BGBCC_SH_RLC_RELW8_BSR;}
	else if(BGBCC_BSR_ModelIsLabel16P(ctx))
	{
//		prlty=BGBCC_SH_RLC_RELW16C_BSR;
		prlty=BGBCC_SH_RLC_RELW16A_BSR;
		if((reg&15)==15)
			prlty=BGBCC_SH_RLC_RELW16C_BSR;
	}
	else
		{ prlty=BGBCC_SH_RLC_RELW24C_BSR; }

	opw1=-1; opw2=-1; opw3=-1; opw4=-1; rlty=-1;
	opwf=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVUB:
	case BGBCC_SH_NMID_MOVB:
		if(prlty==BGBCC_SH_RLC_RELW16A_BSR)
			{ rlty=prlty; opwf=0x4000|((reg&15)<<4); break; }
		rlty=prlty;
		opwf=0x3004|((reg&15)<<4);
		break;
	case BGBCC_SH_NMID_MOVUW:
	case BGBCC_SH_NMID_MOVW:
		if(prlty==BGBCC_SH_RLC_RELW16A_BSR)
			{ rlty=prlty; opwf=0x4100|((reg&15)<<4); break; }
		rlty=prlty;
		opwf=0x3005|((reg&15)<<4);
		break;
	case BGBCC_SH_NMID_MOVL:
		if(prlty==BGBCC_SH_RLC_RELW16A_BSR)
			{ rlty=prlty; opwf=0x4200|((reg&15)<<4); break; }
		rlty=prlty;
		opwf=0x3006|((reg&15)<<4);
		break;
	}
	
	if((rlty>=0) && (opw1<0) && (opwf>=0))
	{
//		if(rlty==BGBCC_SH_RLC_RELW8C_BSR)
//			{ opw1=0x25FE; opw2=opwf; }
		if(rlty==BGBCC_SH_RLC_RELW16C_BSR)
			{ opw1=0x25FF; opw2=0x26FE; opw3=opwf; }
		if(rlty==BGBCC_SH_RLC_RELW24C_BSR)
			{ opw1=0x25FF; opw2=0x26FF; opw3=0x26FE; opw4=opwf; }
		if(rlty==BGBCC_SH_RLC_RELW16A_BSR)
			{ opw1=0xA000; opw2=opwf; }
	}

	if(opw1>=0)
	{
		if(rlty>0)
			{ BGBCC_BSR_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_BSR_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_BSR_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_BSR_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_BSR_EmitWord(ctx, opw4);
		return(1);
	}

	BGBCC_DBGBREAK
	return(0);
}


