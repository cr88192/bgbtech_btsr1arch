int BGBCC_BSR_EmitOpNone(BGBCC_BSR_Context *ctx, int nmid)
{
	if(!BGBCC_BSR_TryEmitOpNone(ctx, nmid))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_BSR_ProbeEmitOpNone(BGBCC_BSR_Context *ctx, int nmid)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_BSR_TryEmitOpNone(ctx, nmid);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_BSR_TryEmitOpNone(BGBCC_BSR_Context *ctx, int nmid)
{
	int opw1, opw2;

	opw1=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_NOP:		opw1=0x3000; break;
	case BGBCC_SH_NMID_RTS:		opw1=0x3010; break;
	case BGBCC_SH_NMID_RTSN:	opw1=0x3010; break;
	case BGBCC_SH_NMID_SLEEP:	opw1=0x3020; break;
	case BGBCC_SH_NMID_BRK:		opw1=0x3030; break;
	case BGBCC_SH_NMID_CLRT:	opw1=0x3040; break;
	case BGBCC_SH_NMID_SETT:	opw1=0x3050; break;
	case BGBCC_SH_NMID_CLRS:	opw1=0x3060; break;
	case BGBCC_SH_NMID_SETS:	opw1=0x3070; break;
	case BGBCC_SH_NMID_NOTT:	opw1=0x3080; break;
	case BGBCC_SH_NMID_NOTS:	opw1=0x3090; break;

	case BGBCC_SH_NMID_RTE:		opw1=0x30C0; break;
	case BGBCC_SH_NMID_DIV0U:	opw1=0x30D0; break;
	case BGBCC_SH_NMID_DIV1:	opw1=0x30E0; break;

//	case BGBCC_SH_NMID_CLRMAC:	opw1=0x0028; break;
//	case BGBCC_SH_NMID_LDTLB:	opw1=0x0038; break;

	default:
		break;
	}
	
	if(opw1>=0)
	{
		BGBCC_BSR_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_BSR_EmitWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_BSR_EmitOpReg(BGBCC_BSR_Context *ctx, int nmid, int reg)
{
	if(!BGBCC_BSR_TryEmitOpReg(ctx, nmid, reg))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_BSR_ProbeEmitOpReg(BGBCC_BSR_Context *ctx, int nmid, int reg)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_BSR_TryEmitOpReg(ctx, nmid, reg);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_BSR_TryEmitOpReg(BGBCC_BSR_Context *ctx, int nmid, int reg)
{
	int opw1, opw2, reg2;

	opw1=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_BSRF:
		if(!BGBCC_BSR_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw1=0x3609|((reg&15)<<4); break;
	case BGBCC_SH_NMID_BRAF:
		if(!BGBCC_BSR_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw1=0x3608|((reg&15)<<4); break;

	case BGBCC_SH_NMID_MOVT:
		if(!BGBCC_BSR_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw1=0x360F|((reg&15)<<4); break;

//	case BGBCC_SH_NMID_MOVRT:
//		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))		break;
//		opw=0x0039|((reg&15)<<8); break;

	case BGBCC_SH_NMID_CMPEQ:	/* pseudo-op */
	case BGBCC_SH_NMID_TST:		/* pseudo-op */
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))		break;
		opw1=0x1400|((reg&15)<<4)|((reg&15)<<0); break;

	case BGBCC_SH_NMID_CMPQEQ:	/* pseudo-op */
	case BGBCC_SH_NMID_TSTQ:	/* pseudo-op */
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))		break;
		opw1=0x1400|((reg&15)<<4)|((reg&15)<<0); break;

	case BGBCC_SH_NMID_CMPPZ:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))		break;
		opw1=0x3109|((reg&15)<<4); break;
	case BGBCC_SH_NMID_CMPPL:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))		break;
		opw1=0x3108|((reg&15)<<4); break;

//	case BGBCC_SH_NMID_SHLL:
//		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))		break;
//		opw=0x4000|((reg&15)<<8); break;
//	case BGBCC_SH_NMID_DT:
//		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))		break;
//		opw=0x4010|((reg&15)<<8); break;
//	case BGBCC_SH_NMID_SHAL:
//		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))		break;
//		opw=0x4020|((reg&15)<<8); break;

//	case BGBCC_SH_NMID_SHLR:
//		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
//		opw=0x4001|((reg&15)<<8); break;
//	case BGBCC_SH_NMID_SHAR:
//		if(!BGBCC_BSR_EmitCheckRegGPR(ctx, reg))		break;
//		opw=0x4021|((reg&15)<<8); break;

	case BGBCC_SH_NMID_ROTL:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3304|((reg&15)<<4);	break;
	case BGBCC_SH_NMID_ROTR:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3305|((reg&15)<<4);	break;
	case BGBCC_SH_NMID_ROTCL:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3306|((reg&15)<<4);	break;
	case BGBCC_SH_NMID_ROTCR:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3307|((reg&15)<<4);	break;

	case BGBCC_SH_NMID_SHLL1:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3400|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHLL2:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3401|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHLL4:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3402|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHLL8:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3403|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHLL16:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x340C|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHLL32:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3600|((reg&15)<<4); break;

	case BGBCC_SH_NMID_SHLR1:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3404|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHLR2:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3405|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHLR4:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3406|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHLR8:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3407|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHLR16:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x340D|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHLR32:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3602|((reg&15)<<4); break;

	case BGBCC_SH_NMID_SHAR1:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3408|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHAR2:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3409|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHAR4:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x340A|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHAR8:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x340B|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHAR16:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x340E|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHAR32:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3603|((reg&15)<<4); break;

	case BGBCC_SH_NMID_JSR:
		if(!BGBCC_BSR_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw1=0x3201|((reg&15)<<4); break;
	case BGBCC_SH_NMID_JMP:
		if(!BGBCC_BSR_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw1=0x3200|((reg&15)<<4); break;

	case BGBCC_SH_NMID_PUSH:
		if(BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))
			{ opw1=0x3008|((reg&15)<<4); break; }
		reg2=BGBCC_BSR_TryMapRegCn(ctx, reg);
		if(reg2>=0)
			{ opw1=0x3009|((reg2&15)<<4); break; }
		break;
	case BGBCC_SH_NMID_POP:
		if(BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))
			{ opw1=0x300A|((reg&15)<<4); break; }
		reg2=BGBCC_BSR_TryMapRegCn(ctx, reg);
		if(reg2>=0)
			{ opw1=0x300B|((reg2&15)<<4); break; }
		break;

	case BGBCC_SH_NMID_EXTUB:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3204|((reg&15)<<4); break;
	case BGBCC_SH_NMID_EXTUW:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3205|((reg&15)<<4); break;
	case BGBCC_SH_NMID_EXTSB:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3206|((reg&15)<<4); break;
	case BGBCC_SH_NMID_EXTSW:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3207|((reg&15)<<4); break;

	case BGBCC_SH_NMID_EXTUL:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3604|((reg&15)<<4); break;
	case BGBCC_SH_NMID_EXTSL:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3605|((reg&15)<<4); break;

	case BGBCC_SH_NMID_SWAPB:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x360A|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SWAPW:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x360B|((reg&15)<<4); break;

	case BGBCC_SH_NMID_NOT:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3300|((reg&15)<<4); break;
	case BGBCC_SH_NMID_NEG:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3301|((reg&15)<<4); break;
	case BGBCC_SH_NMID_NEGC:
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3302|((reg&15)<<4); break;

	default:
		break;
	}

	if(opw1>=0)
	{
		BGBCC_BSR_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_BSR_EmitWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_BSR_EmitOpImm(BGBCC_BSR_Context *ctx, int nmid, int imm)
{
	if(!BGBCC_BSR_TryEmitOpImm(ctx, nmid, imm))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_BSR_ProbeEmitOpImm(BGBCC_BSR_Context *ctx, int nmid, int imm)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_BSR_TryEmitOpImm(ctx, nmid, imm);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_BSR_TryEmitOpImm(BGBCC_BSR_Context *ctx, int nmid, int imm)
{
	int opw1, opw2;

	opw1=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_TRAPA:
//		if(((byte)imm)!=imm)	break;
		if((imm&15)!=imm)		break;
		opw1=0x3603|(imm<<4);	break;

	default:
		break;
	}
	
	if(opw1>=0)
	{
		BGBCC_BSR_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_BSR_EmitWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_BSR_EmitOpMReg(BGBCC_BSR_Context *ctx, int nmid, int reg)
{
	if(!BGBCC_BSR_TryEmitOpMReg(ctx, nmid, reg))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_BSR_ProbeEmitOpMReg(BGBCC_BSR_Context *ctx, int nmid, int reg)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_BSR_TryEmitOpMReg(ctx, nmid, reg);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_BSR_TryEmitOpMReg(BGBCC_BSR_Context *ctx, int nmid, int reg)
{
	int opw1, opw2;

	opw1=-1; opw2=-1;
	switch(nmid)
	{
//	case BGBCC_SH_NMID_PREF:
//		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
//		opw=0x0083|((reg&15)<<8); break;
//	case BGBCC_SH_NMID_OCBI:
//		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
//		opw=0x0093|((reg&15)<<8); break;
//	case BGBCC_SH_NMID_OCBP:
//		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
//		opw=0x00A3|((reg&15)<<8); break;
//	case BGBCC_SH_NMID_OCBWB:
//		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
//		opw=0x00B3|((reg&15)<<8); break;
//	case BGBCC_SH_NMID_ICBI:
//		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
//		opw=0x00E3|((reg&15)<<8); break;

//	case BGBCC_SH_NMID_TASB:
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
//		opw=0x401B|((reg&15)<<8); break;

	case BGBCC_SH_NMID_JSR:
		if(!BGBCC_BSR_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw1=0x3200|((reg&15)<<4); break;
	case BGBCC_SH_NMID_JMP:
		if(!BGBCC_BSR_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw1=0x3201|((reg&15)<<4); break;

	default:
		break;
	}

	if(opw1>=0)
	{
		BGBCC_BSR_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_BSR_EmitWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_BSR_EmitOpRegReg(BGBCC_BSR_Context *ctx, int nmid, int rm, int rn)
{
	if(!BGBCC_BSR_TryEmitOpRegReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_BSR_ProbeEmitOpRegReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_BSR_TryEmitOpRegReg(ctx, nmid, rm, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_BSR_TryMapRegCn(BGBCC_BSR_Context *ctx, int reg)
{
	int reg1;
	
	reg1=-1;
	switch(reg)
	{
	case BGBCC_SH_REG_PC:		reg1=0x10; break;
	case BGBCC_SH_REG_PR:		reg1=0x11; break;
	case BGBCC_SH_REG_SR:		reg1=0x12; break;
	case BGBCC_SH_REG_VBR:		reg1=0x13; break;
	case BGBCC_SH_REG_MACL:		reg1=0x14; break;
	case BGBCC_SH_REG_MACH:		reg1=0x15; break;
	case BGBCC_SH_REG_GBR:		reg1=0x16; break;
	}
	return(reg1);
}

int BGBCC_BSR_TryMapRegSn(BGBCC_BSR_Context *ctx, int reg)
{
	int reg1;
	
	reg1=-1;
	switch(reg)
	{
	case BGBCC_SH_REG_SPC:		reg1=0x28; break;
	case BGBCC_SH_REG_SSR:		reg1=0x2A; break;
	case BGBCC_SH_REG_SGR:		reg1=0x2B; break;

//	case BGBCC_SH_REG_PR:		reg1=0x11; break;
//	case BGBCC_SH_REG_VBR:		reg1=0x13; break;
//	case BGBCC_SH_REG_MACL:		reg1=0x14; break;
//	case BGBCC_SH_REG_MACH:		reg1=0x15; break;
//	case BGBCC_SH_REG_GBR:		reg1=0x16; break;
	}
	return(reg1);
}

int BGBCC_BSR_TryEmitOpRegReg(BGBCC_BSR_Context *ctx, int nmid, int rm, int rn)
{
	int opw1, opw2;
	int rm3, rn3;

	if(rm==rn)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_NOT:
		case BGBCC_SH_NMID_NEG:
		case BGBCC_SH_NMID_NEGC:
		case BGBCC_SH_NMID_SWAPB:
		case BGBCC_SH_NMID_SWAPW:
		case BGBCC_SH_NMID_EXTUB:
		case BGBCC_SH_NMID_EXTUW:
		case BGBCC_SH_NMID_EXTSB:
		case BGBCC_SH_NMID_EXTSW:
			return(BGBCC_BSR_TryEmitOpReg(ctx, nmid, rn));

		case BGBCC_SH_NMID_MOV:
			return(1);

		default:
			break;
		}
	}

	opw1=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_STC:
	case BGBCC_SH_NMID_STS:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		rm3=BGBCC_BSR_TryMapRegCn(ctx, rm);
		if(rm3<0)										break;
		opw1=0x1B00|((rn&15)<<4)|((rm3&15)<<0);			break;
		break;

	case BGBCC_SH_NMID_LDC:
	case BGBCC_SH_NMID_LDS:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		rn3=BGBCC_BSR_TryMapRegCn(ctx, rn);
		if(rn3<0)										break;
		opw1=0x1A00|((rn3&15)<<4)|((rm&15)<<0);			break;
		break;

	case BGBCC_SH_NMID_ADD:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1000|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_SUB:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1100|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_ADDC:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1200|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_SUBC:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1300|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_TST:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1400|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_AND:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1500|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_OR:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1600|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_XOR:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1700|((rn&15)<<4)|((rm&15)<<0);			break;

	case BGBCC_SH_NMID_MOV:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))
		{
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;

			rm3=BGBCC_BSR_TryMapRegCn(ctx, rm);
			if(rm3>0)
				{ opw1=0x1B00|((rn&15)<<4)|((rm3&15)<<0);	break; }

			rm3=BGBCC_BSR_TryMapRegSn(ctx, rm);
			if(rm3>0)
				{ opw1=0x4B00|((rn&15)<<4)|((rm3&15)<<0);	break; }
			break;
		}
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))
		{
			rn3=BGBCC_BSR_TryMapRegCn(ctx, rn);
			if(rn3>0)
				{ opw1=0x1A00|((rn3&15)<<4)|((rm&15)<<0);	break; }

			rn3=BGBCC_BSR_TryMapRegSn(ctx, rn);
			if(rn3>0)
				{ opw1=0x4A00|((rn3&15)<<4)|((rm&15)<<0);	break; }

			break;
		}
		opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
		break;

	case BGBCC_SH_NMID_MULL:
	case BGBCC_SH_NMID_DMULS:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;

	case BGBCC_SH_NMID_CMPEQ:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_CMPHI:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1D00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_CMPGT:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1E00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_DMULU:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1F00|((rn&15)<<4)|((rm&15)<<0);			break;

	case BGBCC_SH_NMID_DIV0S:
	case BGBCC_SH_NMID_XTRCT:
	case BGBCC_SH_NMID_MULUW:
	case BGBCC_SH_NMID_MULSW:
	case BGBCC_SH_NMID_DIV1:
		break;

	case BGBCC_SH_NMID_CMPQEQ:
	case BGBCC_SH_NMID_CMPQHS:
	case BGBCC_SH_NMID_CMPQGE:
	case BGBCC_SH_NMID_CMPQHI:
	case BGBCC_SH_NMID_CMPQGT:
	case BGBCC_SH_NMID_ADDV:
	case BGBCC_SH_NMID_SUBV:
	case BGBCC_SH_NMID_SHADQ:
	case BGBCC_SH_NMID_SHLDQ:
	case BGBCC_SH_NMID_MACW:
		break;


	case BGBCC_SH_NMID_SHAD:
//		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
//		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//		opw=0x400C|((rn&15)<<8)|((rm&15)<<4);			break;
	case BGBCC_SH_NMID_SHLD:
//		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
//		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//		opw=0x400D|((rn&15)<<8)|((rm&15)<<4);			break;
		break;

	case BGBCC_SH_NMID_NOT:
		opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
		opw2=0x3301|((rn&15)<<4);
		break;
	case BGBCC_SH_NMID_NEG:
		opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
		opw2=0x3301|((rn&15)<<4);
		break;
	case BGBCC_SH_NMID_SWAPB:
		opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
		opw2=0x360A|((rn&15)<<4);
		break;
	case BGBCC_SH_NMID_SWAPW:
		opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
		opw2=0x360B|((rn&15)<<4);
		break;

	case BGBCC_SH_NMID_NEGC:
		break;

	case BGBCC_SH_NMID_EXTUB:
		opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
		opw2=0x3204|((rn&15)<<4);
		break;
	case BGBCC_SH_NMID_EXTUW:
		opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
		opw2=0x3205|((rn&15)<<4);
		break;
	case BGBCC_SH_NMID_EXTSB:
		opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
		opw2=0x3206|((rn&15)<<4);
		break;
	case BGBCC_SH_NMID_EXTSW:
		opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
		opw2=0x3207|((rn&15)<<4);
		break;

	default:
		break;
	}

	if(opw1>=0)
	{
		BGBCC_BSR_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_BSR_EmitWord(ctx, opw2);
		return(1);
	}

	return(0);
}


int BGBCC_BSR_EmitOpRegRegReg(BGBCC_BSR_Context *ctx,
	int nmid, int rs, int rt, int rn)
{
	if(!BGBCC_BSR_TryEmitOpRegRegReg(ctx, nmid, rs, rt, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_BSR_ProbeEmitOpRegRegReg(BGBCC_BSR_Context *ctx,
	int nmid, int rs, int rt, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_BSR_TryEmitOpRegRegReg(ctx, nmid, rs, rt, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_BSR_TryEmitOpRegRegReg(
	BGBCC_BSR_Context *ctx, int nmid, int rs, int rt, int rn)
{
	int opw, opw1, opw2;
	int i, e;

	return(0);
}

int BGBCC_BSR_EmitOpImmReg(BGBCC_BSR_Context *ctx,
	int nmid, int imm, int reg)
{
	if(!BGBCC_BSR_TryEmitOpImmReg(ctx, nmid, imm, reg))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_BSR_ProbeEmitOpImmReg(BGBCC_BSR_Context *ctx,
	int nmid, int imm, int reg)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_BSR_TryEmitOpImmReg(ctx, nmid, imm, reg);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_BSR_TryEmitOpImmReg(BGBCC_BSR_Context *ctx,
	int nmid, int imm, int reg)
{
	s32 imm1, imm1n;
	int opw1, opw2, odr;

	imm1n=-imm;
	opw1=-1; opw2=-1; odr=0;
	switch(nmid)
	{
#if 0
	case BGBCC_SH_NMID_LDSH8:	
		if(((byte)imm)!=imm)
			break;
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0xD000|((reg&15)<<8)|(imm&255);			break;
#endif

	case BGBCC_SH_NMID_ADD:	
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(((sbyte)imm)==imm)
			{ opw1=0xC000|((reg&15)<<8)|(imm&255); break; }
		if((((s16)imm)==imm) || (((u16)imm)==imm))
		{
			opw1=0xA000|((imm>>4)&0x1FFF);
			opw2=0x4900|((reg&15)<<4)|(imm&15);
			break;
		}
		odr=1; opw1=0x3100|((reg&15)<<4);				break;

	case BGBCC_SH_NMID_SUB:	
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(((sbyte)imm1n)==imm1n)
			{ opw1=0xC000|((reg&15)<<8)|(imm1n&255); break; }
		if((((s16)imm)==imm1n) || (((u16)imm1n)==imm1n))
		{
			opw1=0xA000|((imm1n>>4)&0x1FFF);
			opw2=0x4900|((reg&15)<<4)|(imm1n&15);
			break;
		}
		odr=1; opw1=0x3101|((reg&15)<<4);				break;

	case BGBCC_SH_NMID_ADDC:	
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		odr=1; opw1=0x3102|((reg&15)<<4);				break;
	case BGBCC_SH_NMID_SUBC:	
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		odr=1; opw1=0x3103|((reg&15)<<4);				break;
	case BGBCC_SH_NMID_TST:	
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		odr=1; opw1=0x3104|((reg&15)<<4);				break;
	case BGBCC_SH_NMID_AND:	
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		odr=1; opw1=0x3105|((reg&15)<<4);				break;
	case BGBCC_SH_NMID_OR:	
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		odr=1; opw1=0x3106|((reg&15)<<4);				break;
	case BGBCC_SH_NMID_XOR:	
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		odr=1; opw1=0x3107|((reg&15)<<4);				break;

	case BGBCC_SH_NMID_MOV:	
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(((sbyte)imm)==imm)
			{ opw1=0xE000|((reg&15)<<8)|(imm&255); break; }
		if((((s16)imm)==imm) || (((u16)imm)==imm))
		{
			opw1=0xA000|((imm>>4)&0x1FFF);
			opw2=0x4800|((reg&15)<<4)|(imm&15);
			break;
		}
		odr=1; opw1=0x1B03|((reg&15)<<4);				break;

	case BGBCC_SH_NMID_CMPEQ:	
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(((imm&15)==imm) || ((imm|(~15))==imm))
			{ opw1=0x2C00|((reg&15)<<4)|(imm&0x010F); break; }
		
		odr=1;	opw1=0x310C|((reg&15)<<4);				break;
	case BGBCC_SH_NMID_CMPHI:	
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		odr=1;		opw1=0x310D|((reg&15)<<4);			break;
	case BGBCC_SH_NMID_CMPGT:	
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		if((imm&15)==imm)
			{ opw1=0x2E00|((reg&15)<<4)|(imm&15); break; }
		odr=1;		opw1=0x310E|((reg&15)<<4);			break;

	case BGBCC_SH_NMID_CMPHS:	
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		odr=1;		opw1=0x360D|((reg&15)<<4);			break;
	case BGBCC_SH_NMID_CMPGE:	
		if(!BGBCC_BSR_EmitCheckRegBaseGPR(ctx, reg))	break;
		if((imm&15)==imm)
			{ opw1=0x2F00|((reg&15)<<4)|(imm&15); break; }
		odr=1;		opw1=0x360E|((reg&15)<<4);			break;
	}

	if(opw1>=0)
	{
		if(odr)
			BGBCC_BSR_EmitLoadDrImm(ctx, imm);
		BGBCC_BSR_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_BSR_EmitWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_BSR_EmitOpRegImmReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int imm, int rn)
{
	if(!BGBCC_BSR_TryEmitOpRegImmReg(ctx, nmid, rm, imm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_BSR_ProbeEmitOpRegImmReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int imm, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_BSR_TryEmitOpRegImmReg(ctx, nmid, rm, imm, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_BSR_TryEmitOpRegImmReg(
	BGBCC_BSR_Context *ctx, int nmid, int rm, int imm, int rn)
{
	int opw, opw1, opw2;
	int imm1;
	int i;

	return(0);
}
