int BGBCC_JX2_EmitOpNone(BGBCC_JX2_Context *ctx, int nmid)
{
	if(!BGBCC_JX2_TryEmitOpNone(ctx, nmid))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpNone(BGBCC_JX2_Context *ctx, int nmid)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpNone(ctx, nmid);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpNone(BGBCC_JX2_Context *ctx, int nmid)
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
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpReg(BGBCC_JX2_Context *ctx, int nmid, int reg)
{
	if(!BGBCC_JX2_TryEmitOpReg(ctx, nmid, reg))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpReg(BGBCC_JX2_Context *ctx, int nmid, int reg)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpReg(ctx, nmid, reg);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpReg(BGBCC_JX2_Context *ctx, int nmid, int reg)
{
	int opw1, opw2, reg2, ex;

	ex=0;
	if(BGBCC_JX2_EmitCheckRegExt4(ctx, reg)) ex|=0x0040;
	if(BGBCC_JX2_EmitCheckRegExt4(ctx, reg)) ex|=0x0020;

	opw1=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_BSRF:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw1=0x3609|((reg&15)<<4); break;
	case BGBCC_SH_NMID_BRAF:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw1=0x3608|((reg&15)<<4); break;

	case BGBCC_SH_NMID_MOVT:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x360F|((reg&15)<<4); break;

	case BGBCC_SH_NMID_MOVNT:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3303|((reg&15)<<4); break;

//	case BGBCC_SH_NMID_MOVRT:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))		break;
//		opw=0x0039|((reg&15)<<8); break;

	case BGBCC_SH_NMID_CMPEQ:	/* pseudo-op */
	case BGBCC_SH_NMID_TST:		/* pseudo-op */
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))		break;
		opw1=0x1400|((reg&15)<<4)|((reg&15)<<0); break;

	case BGBCC_SH_NMID_CMPQEQ:	/* pseudo-op */
	case BGBCC_SH_NMID_TSTQ:	/* pseudo-op */
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))		break;
		opw1=0x1400|((reg&15)<<4)|((reg&15)<<0); break;

	case BGBCC_SH_NMID_CMPPZ:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))		break;
		opw1=0x3109|((reg&15)<<4); break;
	case BGBCC_SH_NMID_CMPPL:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))		break;
		opw1=0x3108|((reg&15)<<4); break;

//	case BGBCC_SH_NMID_SHLL:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))		break;
//		opw=0x4000|((reg&15)<<8); break;
//	case BGBCC_SH_NMID_DT:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))		break;
//		opw=0x4010|((reg&15)<<8); break;
//	case BGBCC_SH_NMID_SHAL:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))		break;
//		opw=0x4020|((reg&15)<<8); break;

//	case BGBCC_SH_NMID_SHLR:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
//		opw=0x4001|((reg&15)<<8); break;
//	case BGBCC_SH_NMID_SHAR:
//		if(!BGBCC_JX2_EmitCheckRegGPR(ctx, reg))		break;
//		opw=0x4021|((reg&15)<<8); break;

	case BGBCC_SH_NMID_ROTL:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3304|((reg&15)<<4);	break;
	case BGBCC_SH_NMID_ROTR:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3305|((reg&15)<<4);	break;
	case BGBCC_SH_NMID_ROTCL:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3306|((reg&15)<<4);	break;
	case BGBCC_SH_NMID_ROTCR:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3307|((reg&15)<<4);	break;

	case BGBCC_SH_NMID_SHLL1:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3400|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHLL2:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3401|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHLL4:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3402|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHLL8:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3403|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHLL16:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x340C|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHLL32:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3600|((reg&15)<<4); break;

	case BGBCC_SH_NMID_SHLR1:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3404|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHLR2:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3405|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHLR4:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3406|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHLR8:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3407|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHLR16:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x340D|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHLR32:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3601|((reg&15)<<4); break;

	case BGBCC_SH_NMID_SHAR1:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3408|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHAR2:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3409|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHAR4:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x340A|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHAR8:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x340B|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHAR16:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x340E|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SHAR32:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3602|((reg&15)<<4); break;

	case BGBCC_SH_NMID_SHARX:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x340F|((reg&15)<<4); break;

	case BGBCC_SH_NMID_JSR:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw1=0x3201|((reg&15)<<4); break;
	case BGBCC_SH_NMID_JMP:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw1=0x3200|((reg&15)<<4); break;

	case BGBCC_SH_NMID_PUSH:
		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
			{ opw1=0x3008|((reg&15)<<4); break; }

		if(BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))
			{ opw1=0x3802|((reg&15)<<4); break; }

		reg2=BGBCC_JX2_TryMapRegCn(ctx, reg);
		if(reg2>=0)
			{ opw1=0x3009|((reg2&15)<<4); break; }
		if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{ opw1=0x3208|((reg&15)<<4); break; }
		break;
	case BGBCC_SH_NMID_POP:
		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
			{ opw1=0x300A|((reg&15)<<4); break; }

		if(BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))
			{ opw1=0x3803|((reg&15)<<4); break; }

		reg2=BGBCC_JX2_TryMapRegCn(ctx, reg);
		if(reg2>=0)
			{ opw1=0x300B|((reg2&15)<<4); break; }
		if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{ opw1=0x320A|((reg&15)<<4); break; }
		break;

	case BGBCC_SH_NMID_PUSHX2:
		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
			{ opw1=0x320C|((reg&15)<<4); break; }
		if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{ opw1=0x320D|((reg&15)<<4); break; }
		break;
	case BGBCC_SH_NMID_POPX2:
		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
			{ opw1=0x320E|((reg&15)<<4); break; }
		if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{ opw1=0x320F|((reg&15)<<4); break; }
		break;

	case BGBCC_SH_NMID_EXTUB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3204|((reg&15)<<4); break;
	case BGBCC_SH_NMID_EXTUW:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3205|((reg&15)<<4); break;
	case BGBCC_SH_NMID_EXTSB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3206|((reg&15)<<4); break;
	case BGBCC_SH_NMID_EXTSW:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3207|((reg&15)<<4); break;

	case BGBCC_SH_NMID_EXTUL:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3604|((reg&15)<<4); break;
	case BGBCC_SH_NMID_EXTSL:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3605|((reg&15)<<4); break;

	case BGBCC_SH_NMID_SWAPB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x360A|((reg&15)<<4); break;
	case BGBCC_SH_NMID_SWAPW:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x360B|((reg&15)<<4); break;

	case BGBCC_SH_NMID_NOT:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3300|((reg&15)<<4); break;
	case BGBCC_SH_NMID_NEG:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3301|((reg&15)<<4); break;
	case BGBCC_SH_NMID_NEGC:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0x3302|((reg&15)<<4); break;

	case BGBCC_SH_NMID_FLDCF:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F00|((reg&15)<<4); break;
	case BGBCC_SH_NMID_FLDCD:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F01|((reg&15)<<4); break;
	case BGBCC_SH_NMID_FLDCI:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F02|((reg&15)<<4); break;
	case BGBCC_SH_NMID_FLDCH:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F03|((reg&15)<<4); break;
	case BGBCC_SH_NMID_FSTCF:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F04|((reg&15)<<4); break;
	case BGBCC_SH_NMID_FSTCD:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F05|((reg&15)<<4); break;
	case BGBCC_SH_NMID_FSTCI:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F06|((reg&15)<<4); break;
	case BGBCC_SH_NMID_FSTCH:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F07|((reg&15)<<4); break;

	case BGBCC_SH_NMID_FNEG:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F08|((reg&15)<<4); break;
	case BGBCC_SH_NMID_FABS:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F09|((reg&15)<<4); break;
	case BGBCC_SH_NMID_FRCPA:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F0A|((reg&15)<<4); break;
	case BGBCC_SH_NMID_FSQRTA:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F0B|((reg&15)<<4); break;
	case BGBCC_SH_NMID_FRCP:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F0C|((reg&15)<<4); break;
	case BGBCC_SH_NMID_FSQRT:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F0D|((reg&15)<<4); break;

	default:
		break;
	}

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
	{
		switch(nmid)
		{
//		case BGBCC_SH_NMID_MOVT:
//			opw1=0xF000|ex;
//			opw2=0x1B00|((reg&15)<<4)|((reg&15)<<0);	break;

		case BGBCC_SH_NMID_NOT:
			opw1=0xF000|ex;
			opw2=0x1C00|((reg&15)<<4)|((reg&15)<<0);	break;
		case BGBCC_SH_NMID_NEG:
			opw1=0xF001|ex;
			opw2=0x1C00|((reg&15)<<4)|((reg&15)<<0);	break;

		case BGBCC_SH_NMID_EXTSB:
			opw1=0xF008|ex;
			opw2=0x1C00|((reg&15)<<4)|((reg&15)<<0);	break;
		case BGBCC_SH_NMID_EXTUB:
			opw1=0xF088|ex;
			opw2=0x1C00|((reg&15)<<4)|((reg&15)<<0);	break;
		case BGBCC_SH_NMID_EXTSW:
			opw1=0xF009|ex;
			opw2=0x1C00|((reg&15)<<4)|((reg&15)<<0);	break;
		case BGBCC_SH_NMID_EXTUW:
			opw1=0xF089|ex;
			opw2=0x1C00|((reg&15)<<4)|((reg&15)<<0);	break;
		case BGBCC_SH_NMID_EXTSL:
			opw1=0xF019|ex;
			opw2=0x1C00|((reg&15)<<4)|((reg&15)<<0);	break;
		case BGBCC_SH_NMID_EXTUL:
			opw1=0xF099|ex;
			opw2=0x1C00|((reg&15)<<4)|((reg&15)<<0);	break;

		case BGBCC_SH_NMID_SHLL1:
			opw1=0xF000|ex;
			opw2=0x1900|((reg&15)<<4)|((reg&15)<<0);	break;
		}
	}

	if(opw1>=0)
	{
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpImm(BGBCC_JX2_Context *ctx, int nmid, int imm)
{
	if(!BGBCC_JX2_TryEmitOpImm(ctx, nmid, imm))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpImm(BGBCC_JX2_Context *ctx, int nmid, int imm)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpImm(ctx, nmid, imm);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpImm(BGBCC_JX2_Context *ctx, int nmid, int imm)
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
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpMReg(BGBCC_JX2_Context *ctx, int nmid, int reg)
{
	if(!BGBCC_JX2_TryEmitOpMReg(ctx, nmid, reg))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpMReg(BGBCC_JX2_Context *ctx, int nmid, int reg)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpMReg(ctx, nmid, reg);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpMReg(BGBCC_JX2_Context *ctx, int nmid, int reg)
{
	int opw1, opw2;

	opw1=-1; opw2=-1;
	switch(nmid)
	{
//	case BGBCC_SH_NMID_PREF:
//		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, reg))	break;
//		opw=0x0083|((reg&15)<<8); break;
//	case BGBCC_SH_NMID_OCBI:
//		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, reg))	break;
//		opw=0x0093|((reg&15)<<8); break;
//	case BGBCC_SH_NMID_OCBP:
//		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, reg))	break;
//		opw=0x00A3|((reg&15)<<8); break;
//	case BGBCC_SH_NMID_OCBWB:
//		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, reg))	break;
//		opw=0x00B3|((reg&15)<<8); break;
//	case BGBCC_SH_NMID_ICBI:
//		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, reg))	break;
//		opw=0x00E3|((reg&15)<<8); break;

//	case BGBCC_SH_NMID_TASB:
//		if(!BGBCC_JX2_EmitCheckRegGPR(ctx, reg))		break;
//		opw=0x401B|((reg&15)<<8); break;

	case BGBCC_SH_NMID_JSR:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw1=0x3200|((reg&15)<<4); break;
	case BGBCC_SH_NMID_JMP:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw1=0x3201|((reg&15)<<4); break;

	default:
		break;
	}

	if(opw1>=0)
	{
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpRegReg(BGBCC_JX2_Context *ctx, int nmid, int rm, int rn)
{
	if(!BGBCC_JX2_TryEmitOpRegReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpRegReg(ctx, nmid, rm, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryMapRegCn(BGBCC_JX2_Context *ctx, int reg)
{
	int reg1;
	
	reg1=-1;
	switch(reg)
	{
	case BGBCC_SH_REG_PC:		reg1=0x10; break;
	case BGBCC_SH_REG_PR:		reg1=0x11; break;
	case BGBCC_SH_REG_SR:		reg1=0x12; break;
	case BGBCC_SH_REG_VBR:		reg1=0x13; break;
//	case BGBCC_SH_REG_MACL:		reg1=0x14; break;
//	case BGBCC_SH_REG_MACH:		reg1=0x15; break;
	case BGBCC_SH_REG_GBR:		reg1=0x16; break;

	case BGBCC_SH_REG_TTB:		reg1=0x18; break;
	case BGBCC_SH_REG_TEA:		reg1=0x19; break;
	case BGBCC_SH_REG_MMCR:		reg1=0x1A; break;
	case BGBCC_SH_REG_EXSR:		reg1=0x1B; break;
	}
	return(reg1);
}

int BGBCC_JX2_TryMapRegSn(BGBCC_JX2_Context *ctx, int reg)
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

int BGBCC_JX2_EmitCheckRegBsrCn(BGBCC_JX2_Context *ctx, int reg)
{
	int i;
	i=BGBCC_JX2_TryMapRegCn(ctx, reg);
	return(i>=0);
}

int BGBCC_JX2_EmitCheckRegBsrSn(BGBCC_JX2_Context *ctx, int reg)
{
	int i;
	i=BGBCC_JX2_TryMapRegSn(ctx, reg);
	return(i>=0);
}

int BGBCC_JX2_TryEmitOpRegReg(BGBCC_JX2_Context *ctx, int nmid, int rm, int rn)
{
	int opw1, opw2, ex;
	int rm3, rn3;

	if(rm==BGBCC_SH_REG_MACL)		rm=BGBCC_SH_REG_R0;
	if(rn==BGBCC_SH_REG_MACL)		rn=BGBCC_SH_REG_R0;
	if(rm==BGBCC_SH_REG_MACH)		rm=BGBCC_SH_REG_R1;
	if(rn==BGBCC_SH_REG_MACH)		rn=BGBCC_SH_REG_R1;

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
		case BGBCC_SH_NMID_EXTUL:
		case BGBCC_SH_NMID_EXTSL:
			return(BGBCC_JX2_TryEmitOpReg(ctx, nmid, rn));

		case BGBCC_SH_NMID_MOV:
			return(1);

		default:
			break;
		}
	}


	ex=0;
	if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
	if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;

	opw1=-1; opw2=-1;

	if(	(nmid==BGBCC_SH_NMID_MOV) ||
		(nmid==BGBCC_SH_NMID_STS) ||
		(nmid==BGBCC_SH_NMID_STC) ||
		(nmid==BGBCC_SH_NMID_LDS) ||
		(nmid==BGBCC_SH_NMID_LDC) )
	{
		if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm))
		{
			if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
			{
				if(ex)
				{
//					opw1=0xF008|ex;
//					opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);
//					opw1=0x5400|(ex<<3)|
					opw1=0x1800|(ex<<3)|
						((rn&15)<<4)|((rm&15)<<0);
				}else
				{
					opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
				}
			}else if(BGBCC_JX2_EmitCheckRegBsrCn(ctx, rn))
			{
				rn3=BGBCC_JX2_TryMapRegCn(ctx, rn);
				if(ex)
				{
					opw1=0xF00A|ex;
					opw2=0x1900|((rn3&15)<<4)|((rm&15)<<0);
				}else
				{
//					opw1=0x1A00|((rn3&15)<<4)|((rm&15)<<0);
					opw1=0x4800|((rn3&15)<<4)|((rm&15)<<0);
				}
			}else if(BGBCC_JX2_EmitCheckRegBsrSn(ctx, rn))
			{
				rn3=BGBCC_JX2_TryMapRegSn(ctx, rn);
				if(ex)
				{
					opw1=0xF00A|ex;
					opw2=0x1C00|((rn3&15)<<4)|((rm&15)<<0);
				}else
				{
					opw1=0x4A00|((rn3&15)<<4)|((rm&15)<<0);
				}
			}
		}else if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
		{
			if(BGBCC_JX2_EmitCheckRegBsrCn(ctx, rm))
			{
				rm3=BGBCC_JX2_TryMapRegCn(ctx, rm);
				if(ex)
				{
					opw1=0xF00B|ex;
					opw2=0x1900|((rn&15)<<4)|((rm3&15)<<0);
				}else
				{
//					opw1=0x1B00|((rn&15)<<4)|((rm3&15)<<0);
					opw1=0x4900|((rn&15)<<4)|((rm3&15)<<0);
				}
			}else if(BGBCC_JX2_EmitCheckRegBsrSn(ctx, rm))
			{
				rm3=BGBCC_JX2_TryMapRegSn(ctx, rm);
				if(ex)
				{
					opw1=0xF00B|ex;
					opw2=0x1C00|((rn&15)<<4)|((rm3&15)<<0);
				}else
				{
					opw1=0x4B00|((rn&15)<<4)|((rm3&15)<<0);
				}
			}
		}
	}

	if(opw1<0)
		switch(nmid)
	{
#if 1
	case BGBCC_SH_NMID_STC:
	case BGBCC_SH_NMID_STS:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		rm3=BGBCC_JX2_TryMapRegCn(ctx, rm);
		if(rm3<0)										break;
		opw1=0x1B00|((rn&15)<<4)|((rm3&15)<<0);			break;
		break;

	case BGBCC_SH_NMID_LDC:
	case BGBCC_SH_NMID_LDS:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		rn3=BGBCC_JX2_TryMapRegCn(ctx, rn);
		if(rn3<0)										break;
		opw1=0x1A00|((rn3&15)<<4)|((rm&15)<<0);			break;
		break;
#endif

	case BGBCC_SH_NMID_ADD:
#if 0
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))
		{
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(rm==BGBCC_SH_REG_DLR)
			{
				opw1=0x3100|((rn&15)<<4);
				break;
			}
			break;
		}
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))
		{
			if(rn==BGBCC_SH_REG_DLR)
			{
				opw1=0x360C|((rm&15)<<4);
				break;
			}
			break;
		}
#endif

		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;

		opw1=0x1000|((rn&15)<<4)|((rm&15)<<0);			break;

#if 1
	case BGBCC_SH_NMID_SUB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1100|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_ADDC:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1200|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_SUBC:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1300|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_TST:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1400|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_AND:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1500|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_OR:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1600|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_XOR:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1700|((rn&15)<<4)|((rm&15)<<0);			break;
#endif

	case BGBCC_SH_NMID_MOV:

#if 0
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))
		{
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;

			rm3=BGBCC_JX2_TryMapRegCn(ctx, rm);
			if(rm3>0)
				{ opw1=0x1B00|((rn&15)<<4)|((rm3&15)<<0);	break; }

			rm3=BGBCC_JX2_TryMapRegSn(ctx, rm);
			if(rm3>0)
				{ opw1=0x4B00|((rn&15)<<4)|((rm3&15)<<0);	break; }
			break;
		}
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))
		{
			rn3=BGBCC_JX2_TryMapRegCn(ctx, rn);
			if(rn3>0)
				{ opw1=0x1A00|((rn3&15)<<4)|((rm&15)<<0);	break; }

			rn3=BGBCC_JX2_TryMapRegSn(ctx, rn);
			if(rn3>0)
				{ opw1=0x4A00|((rn3&15)<<4)|((rm&15)<<0);	break; }

			break;
		}
#endif
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
		break;

#if 1
	case BGBCC_SH_NMID_MULL:
	case BGBCC_SH_NMID_DMULS:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
//		opw1=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;
		opw1=0x5600|((rn&15)<<4)|((rm&15)<<0);			break;

	case BGBCC_SH_NMID_CMPEQ:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_CMPHI:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1D00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_CMPGT:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1E00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_DMULU:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
//		opw1=0x1F00|((rn&15)<<4)|((rm&15)<<0);			break;
		opw1=0x5700|((rn&15)<<4)|((rm&15)<<0);			break;
#endif

	case BGBCC_SH_NMID_DIV0S:
	case BGBCC_SH_NMID_XTRCT:
	case BGBCC_SH_NMID_MULUW:
	case BGBCC_SH_NMID_MULSW:
	case BGBCC_SH_NMID_DIV1:
		break;

//	case BGBCC_SH_NMID_CMPQEQ:
	case BGBCC_SH_NMID_CMPQHS:
	case BGBCC_SH_NMID_CMPQGE:
//	case BGBCC_SH_NMID_CMPQHI:
//	case BGBCC_SH_NMID_CMPQGT:
	case BGBCC_SH_NMID_ADDV:
	case BGBCC_SH_NMID_SUBV:
//	case BGBCC_SH_NMID_SHADQ:
//	case BGBCC_SH_NMID_SHLDQ:
	case BGBCC_SH_NMID_MACW:
		break;

#if 1
	case BGBCC_SH_NMID_TSTQ:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x5400|((rn&15)<<4)|((rm&15)<<0);			break;

	case BGBCC_SH_NMID_CMPQHI:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x5D00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_CMPQGT:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x5E00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_CMPQEQ:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x5F00|((rn&15)<<4)|((rm&15)<<0);			break;
#endif

#if 0
	case BGBCC_SH_NMID_SHAD:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1A40|(rm&15);
		opw2=0x3606|((rn&15)<<4);			break;
	case BGBCC_SH_NMID_SHLD:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1A40|(rm&15);
		opw2=0x3607|((rn&15)<<4);			break;
		break;

	case BGBCC_SH_NMID_SHADQ:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1A40|(rm&15);
		opw2=0x3806|((rn&15)<<4);			break;
	case BGBCC_SH_NMID_SHLDQ:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1A40|(rm&15);
		opw2=0x3807|((rn&15)<<4);			break;
		break;
#endif

#if 0
	case BGBCC_SH_NMID_NOT:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0xF000;
		opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;
//		opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
//		opw2=0x3301|((rn&15)<<4);
//		break;
	case BGBCC_SH_NMID_NEG:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0xF001;
		opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;
//		opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
//		opw2=0x3301|((rn&15)<<4);
		break;
#endif

	case BGBCC_SH_NMID_SWAPB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
		opw2=0x360A|((rn&15)<<4);
		break;
	case BGBCC_SH_NMID_SWAPW:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
		opw2=0x360B|((rn&15)<<4);
		break;

	case BGBCC_SH_NMID_NEGC:
		break;

#if 0
	case BGBCC_SH_NMID_EXTUB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
		opw2=0x3204|((rn&15)<<4);
		break;
	case BGBCC_SH_NMID_EXTUW:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
		opw2=0x3205|((rn&15)<<4);
		break;
	case BGBCC_SH_NMID_EXTSB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
		opw2=0x3206|((rn&15)<<4);
		break;
	case BGBCC_SH_NMID_EXTSW:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
		opw2=0x3207|((rn&15)<<4);
		break;

	case BGBCC_SH_NMID_EXTUL:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
		opw2=0x3604|((rn&15)<<4); break;
	case BGBCC_SH_NMID_EXTSL:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
		opw2=0x3605|((rn&15)<<4); break;
#endif

	case BGBCC_SH_NMID_FADD:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rn))		break;
		opw1=0x9800|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_FSUB:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rn))		break;
		opw1=0x9900|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_FMUL:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rn))		break;
		opw1=0x9A00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_FDIV:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rn))		break;
		opw1=0x9B00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_FCMPEQ:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rn))		break;
		opw1=0x9C00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_FCMPGT:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rn))		break;
		opw1=0x9D00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_FMOV:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rn))		break;
		opw1=0x9E00|((rn&15)<<4)|((rm&15)<<0);			break;

	default:
		break;
	}
	
//	opw1=-1;

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_ADD:
			opw1=0xF000|ex;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_SUB:
			opw1=0xF001|ex;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_ADDC:
			opw1=0xF002|ex;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_SUBC:
			opw1=0xF003|ex;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_TST:
			opw1=0xF004|ex;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_AND:
			opw1=0xF005|ex;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_OR:
			opw1=0xF006|ex;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_XOR:
			opw1=0xF007|ex;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOV:
			opw1=0xF008|ex;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_MULL:
		case BGBCC_SH_NMID_DMULS:
			opw1=0xF009|ex;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;
		
		case BGBCC_SH_NMID_CMPEQ:
			opw1=0xF00C|ex;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_CMPHI:
			opw1=0xF00D|ex;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_CMPGT:
			opw1=0xF00E|ex;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_DMULU:
			opw1=0xF00F|ex;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_TSTQ:
			opw1=0xF084|ex;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_CMPQEQ:
			opw1=0xF00C|ex|0x0080;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_CMPQHI:
			opw1=0xF00D|ex|0x0080;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_CMPQGT:
			opw1=0xF00E|ex|0x0080;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_CMPHS:
			opw1=0xF00D|ex;
			opw2=0x1D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_CMPGE:
			opw1=0xF00E|ex;
			opw2=0x1D00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_CMPQHS:
			opw1=0xF08D|ex;
			opw2=0x1D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_CMPQGE:
			opw1=0xF08E|ex;
			opw2=0x1D00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_NOT:
			opw1=0xF000|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_NEG:
			opw1=0xF001|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_SHAD:
			opw1=0xF006|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_SHLD:
			opw1=0xF007|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_SHADQ:
			opw1=0xF086|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_SHLDQ:
			opw1=0xF087|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_EXTSB:
			opw1=0xF008|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_EXTUB:
			opw1=0xF088|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_EXTSW:
			opw1=0xF009|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_EXTUW:
			opw1=0xF089|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_EXTSL:
			opw1=0xF019|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_EXTUL:
			opw1=0xF099|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}

	if(opw1>=0)
	{
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		return(1);
	}

	return(0);
}


int BGBCC_JX2_EmitOpRegRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rs, int rt, int rn)
{
	if(!BGBCC_JX2_TryEmitOpRegRegReg(ctx, nmid, rs, rt, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpRegRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rs, int rt, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpRegRegReg(ctx, nmid, rs, rt, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpRegRegReg(
	BGBCC_JX2_Context *ctx, int nmid, int rs, int rt, int rn)
{
	int opw1, opw2, opw3;
	int i, ex;

	opw1=-1; opw2=-1; opw3=-1;

	ex=0;
	if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
	if(BGBCC_JX2_EmitCheckRegExt4(ctx, rs)) ex|=0x0020;
	if(BGBCC_JX2_EmitCheckRegExt4(ctx, rt)) ex|=0x0010;


#if 0
	switch(nmid)
	{
	case BGBCC_SH_NMID_ADD:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rs))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rt))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1A40|(rt&15);
		opw2=0x5800|((rn&15)<<4)|((rs&15)<<0);			break;
		break;
	case BGBCC_SH_NMID_SUB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rs))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rt))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1A40|(rt&15);
		opw2=0x5900|((rn&15)<<4)|((rs&15)<<0);			break;
		break;
	case BGBCC_SH_NMID_AND:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rs))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rt))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1A40|(rt&15);
		opw2=0x5A00|((rn&15)<<4)|((rs&15)<<0);			break;
		break;
	case BGBCC_SH_NMID_OR:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rs))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rt))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1A40|(rt&15);
		opw2=0x5B00|((rn&15)<<4)|((rs&15)<<0);			break;
		break;
	case BGBCC_SH_NMID_XOR:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rs))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rt))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1A40|(rt&15);
		opw2=0x5C00|((rn&15)<<4)|((rs&15)<<0);			break;
		break;
	default:
		break;
	}
#endif

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rs) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rt))
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_ADD:
			opw1=0xF000|ex|(rt&15);
			opw2=0x1000|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_SUB:
			opw1=0xF000|ex|(rt&15);
			opw2=0x1100|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_MULL:
			opw1=0xF000|ex|(rt&15);
			opw2=0x1200|((rn&15)<<4)|((rs&15)<<0);
			break;

		case BGBCC_SH_NMID_AND:
			opw1=0xF000|ex|(rt&15);
			opw2=0x1500|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_OR:
			opw1=0xF000|ex|(rt&15);
			opw2=0x1600|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_XOR:
			opw1=0xF000|ex|(rt&15);
			opw2=0x1700|((rn&15)<<4)|((rs&15)<<0);
			break;

/*
		case BGBCC_SH_NMID_SHAD:
			opw1=0xF000|ex|(rt&15);
			opw2=0x1300|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_SHLD:
			opw1=0xF000|ex|(rt&15);
			opw2=0x1400|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_SHADQ:
			opw1=0xF080|ex|(rt&15);
			opw2=0x1300|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_SHLDQ:
			opw1=0xF080|ex|(rt&15);
			opw2=0x1400|((rn&15)<<4)|((rs&15)<<0);
			break;
*/

		case BGBCC_SH_NMID_SHAD:
//			opw1=(rt&16)?(0x320C|((rt&15)<<4)):(0x1A40|(rt&15));
//			opw1=(rt&16)?(0x5500|(rt&15)):(0x1800|(rt&15));
			opw1=(rt&16)?(0x1900|(rt&15)):(0x1800|(rt&15));
			opw2=0xF006|ex;
			opw3=0x1D00|((rn&15)<<4)|((rs&15)<<0);			break;
		case BGBCC_SH_NMID_SHLD:
//			opw1=0x330C|((rt&15)<<4)|((rt>>4)&1);
//			opw1=(rt&16)?(0x320C|((rt&15)<<4)):(0x1A40|(rt&15));
//			opw1=(rt&16)?(0x5500|(rt&15)):(0x1800|(rt&15));
			opw1=(rt&16)?(0x1900|(rt&15)):(0x1800|(rt&15));
			opw2=0xF007|ex;
			opw3=0x1D00|((rn&15)<<4)|((rs&15)<<0);			break;
		case BGBCC_SH_NMID_SHADQ:
//			opw1=0x330C|((rt&15)<<4)|((rt>>4)&1);
//			opw1=(rt&16)?(0x320C|((rt&15)<<4)):(0x1A40|(rt&15));
//			opw1=(rt&16)?(0x5500|(rt&15)):(0x1800|(rt&15));
			opw1=(rt&16)?(0x1900|(rt&15)):(0x1800|(rt&15));
			opw2=0xF086|ex;
			opw3=0x1D00|((rn&15)<<4)|((rs&15)<<0);			break;
		case BGBCC_SH_NMID_SHLDQ:
//			opw1=0x330C|((rt&15)<<4)|((rt>>4)&1);
//			opw1=(rt&16)?(0x320C|((rt&15)<<4)):(0x1A40|(rt&15));
//			opw1=(rt&16)?(0x5500|(rt&15)):(0x1800|(rt&15));
			opw1=(rt&16)?(0x1900|(rt&15)):(0x1800|(rt&15));
			opw2=0xF087|ex;
			opw3=0x1D00|((rn&15)<<4)|((rs&15)<<0);			break;

		case BGBCC_SH_NMID_SHAR:
			opw1=0x330C|((rt&15)<<4)|((rt>>4)&1);
			opw2=0xF006|ex;
			opw3=0x1D00|((rn&15)<<4)|((rs&15)<<0);			break;
		case BGBCC_SH_NMID_SHLR:
			opw1=0x330C|((rt&15)<<4)|((rt>>4)&1);
			opw2=0xF007|ex;
			opw3=0x1D00|((rn&15)<<4)|((rs&15)<<0);			break;
		case BGBCC_SH_NMID_SHARQ:
			opw1=0x330C|((rt&15)<<4)|((rt>>4)&1);
			opw2=0xF086|ex;
			opw3=0x1D00|((rn&15)<<4)|((rs&15)<<0);			break;
		case BGBCC_SH_NMID_SHLRQ:
			opw1=0x330C|((rt&15)<<4)|((rt>>4)&1);
			opw2=0xF087|ex;
			opw3=0x1D00|((rn&15)<<4)|((rs&15)<<0);			break;

		case BGBCC_SH_NMID_CSELT:
			opw1=0xF000|ex|(rt&15);
			opw2=0x5000|((rn&15)<<4)|((rs&15)<<0);
			break;
		}
	}

#if 1
	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtFPR(ctx, rn) &&
		BGBCC_JX2_EmitCheckRegExtFPR(ctx, rs) &&
		BGBCC_JX2_EmitCheckRegExtFPR(ctx, rt))
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_FADD:
			opw1=0xF000|ex|(rt&15);
			opw2=0x9800|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_FSUB:
			opw1=0xF000|ex|(rt&15);
			opw2=0x9900|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_FMUL:
			opw1=0xF000|ex|(rt&15);
			opw2=0x9A00|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_FDIV:
			opw1=0xF000|ex|(rt&15);
			opw2=0x9B00|((rn&15)<<4)|((rs&15)<<0);
			break;
		}
	}
#endif

	if(opw1>=0)
	{
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpImmReg(BGBCC_JX2_Context *ctx,
	int nmid, int imm, int reg)
{
	if(!BGBCC_JX2_TryEmitOpImmReg(ctx, nmid, imm, reg))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpImmReg(BGBCC_JX2_Context *ctx,
	int nmid, int imm, int reg)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpImmReg(ctx, nmid, imm, reg);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpImmReg(BGBCC_JX2_Context *ctx,
	int nmid, int imm, int reg)
{
	s32 imm1, imm1n;
	int opw1, opw2, opw3, odr, ex;

	imm1n=-imm;
	opw1=-1; opw2=-1; opw3=-1; odr=0;
	switch(nmid)
	{
#if 0
	case BGBCC_SH_NMID_LDSH8:	
		if(((byte)imm)!=imm)
			break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
//		opw1=0xD000|((reg&15)<<8)|(imm&255);			break;
		break;
#endif

#if 1
	case BGBCC_SH_NMID_ADD:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(((sbyte)imm)==imm)
			{ opw1=0xC000|((reg&15)<<8)|(imm&255); break; }
		break;
		if((((s16)imm)==imm) || (((u16)imm)==imm))
		{
//			opw1=0xA000|((imm>>4)&0x1FFF);
//			opw2=0x4900|((reg&15)<<4)|(imm&15);
			break;
		}
//		odr=1; opw1=0x3100|((reg&15)<<4);				break;
		odr=1; opw1=0x1000|((reg&15)<<4);				break;

	case BGBCC_SH_NMID_SUB:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(((sbyte)imm1n)==imm1n)
			{ opw1=0xC000|((reg&15)<<8)|(imm1n&255); break; }
		break;
		if((((s16)imm)==imm1n) || (((u16)imm1n)==imm1n))
		{
//			opw1=0xA000|((imm1n>>4)&0x1FFF);
//			opw2=0x4900|((reg&15)<<4)|(imm1n&15);
			break;
		}
//		odr=1; opw1=0x3101|((reg&15)<<4);				break;
		odr=1; opw1=0x1100|((reg&15)<<4);				break;

	case BGBCC_SH_NMID_ADDC:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
//		odr=1; opw1=0x3102|((reg&15)<<4);				break;
		odr=1; opw1=0x1200|((reg&15)<<4);				break;
	case BGBCC_SH_NMID_SUBC:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
//		odr=1; opw1=0x3103|((reg&15)<<4);				break;
		odr=1; opw1=0x1300|((reg&15)<<4);				break;
	case BGBCC_SH_NMID_TST:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
//		odr=1; opw1=0x3104|((reg&15)<<4);				break;
		odr=1; opw1=0x1400|((reg&15)<<4);				break;
	case BGBCC_SH_NMID_AND:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		
		if(imm==255)
			{ opw1=0x3204|((reg&15)<<4);	break;	}
		if(imm==65535)
			{ opw1=0x3205|((reg&15)<<4);	break;	}
		
//		odr=1; opw1=0x3105|((reg&15)<<4);				break;
		odr=1; opw1=0x1500|((reg&15)<<4);				break;
	case BGBCC_SH_NMID_OR:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
//		odr=1; opw1=0x3106|((reg&15)<<4);				break;
		odr=1; opw1=0x1600|((reg&15)<<4);				break;
	case BGBCC_SH_NMID_XOR:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
//		odr=1; opw1=0x3107|((reg&15)<<4);				break;
		odr=1; opw1=0x1000|((reg&15)<<4);				break;

	case BGBCC_SH_NMID_MOV:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(((sbyte)imm)==imm)
			{ opw1=0xE000|((reg&15)<<8)|(imm&255); break; }
		break;
//		if((((s16)imm)==imm) || (((u16)imm)==imm))
//		{
//			opw1=0xA000|((imm>>4)&0x1FFF);
//			opw2=0x4800|((reg&15)<<4)|(imm&15);
//			break;
//		}
//		odr=1; opw1=0x1B03|((reg&15)<<4);				break;
#endif

#if 1
	case BGBCC_SH_NMID_CMPEQ:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(((imm&15)==imm) || ((imm|(~15))==imm))
			{ opw1=0x2C00|((reg&15)<<4)|(imm&0x010F); break; }
//		if(((imm&255)==imm) || ((imm|(~255))==imm))
//			{ opw1=0xF000|(reg&15); opw2=0x2C00|(imm&0x01FF); break; }		
		if(((imm&1023)==imm) || ((imm|(~1023))==imm))
		{	opw1=0xF000|(reg&31)|((imm>>3)&0x0060);
			opw2=0x2C00|(imm&0x00FF)|((imm>>10)&0x0100);
			break;		}
//		odr=1;	opw1=0x310C|((reg&15)<<4);				break;
		odr=1;	opw1=0x1C00|((reg&15)<<4);				break;
	case BGBCC_SH_NMID_CMPHI:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
//		odr=1;		opw1=0x310D|((reg&15)<<4);			break;
		odr=1;	opw1=0x1D00|((reg&15)<<4);				break;
	case BGBCC_SH_NMID_CMPGT:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if((imm&15)==imm)
			{ opw1=0x2E00|((reg&15)<<4)|(imm&15); break; }
//		if((imm&255)==imm)
//			{ opw1=0xF000|(reg&15); opw2=0x2E00|(imm&0x00FF); break; }		
		if((imm&1023)==imm)
		{	opw1=0xF000|(reg&31)|((imm>>3)&0x0060);
			opw2=0x2E00|(imm&0x00FF);
			break;		}		
//		odr=1;		opw1=0x310E|((reg&15)<<4);			break;
		odr=1;	opw1=0x1E00|((reg&15)<<4);				break;

	case BGBCC_SH_NMID_CMPHS:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		odr=1;		opw1=0x360D|((reg&15)<<4);			break;
	case BGBCC_SH_NMID_CMPGE:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if((imm&15)==imm)
			{ opw1=0x2F00|((reg&15)<<4)|(imm&15); break; }
//		if((imm&255)==imm)
//			{ opw1=0xF000|(reg&15); opw2=0x2F00|(imm&0x00FF); break; }		
		if((imm&1023)==imm)
		{	opw1=0xF000|(reg&31)|((imm>>3)&0x0060);
			opw2=0x2F00|(imm&0x00FF);
			break;		}		
		odr=1;		opw1=0x360E|((reg&15)<<4);			break;
#endif

#if 1

	case BGBCC_SH_NMID_SHAD:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
#if 1
		if(imm==1)		{ opw1=0x3400|((reg&15)<<4);	break; }
		if(imm==2)		{ opw1=0x3401|((reg&15)<<4);	break; }
		if(imm==4)		{ opw1=0x3402|((reg&15)<<4);	break; }
		if(imm==8)		{ opw1=0x3403|((reg&15)<<4);	break; }
		if(imm==16)		{ opw1=0x340C|((reg&15)<<4);	break; }
		if(imm==32)		{ opw1=0x3600|((reg&15)<<4);	break; }
//		if(imm==-1)		{ opw1=0x3408|((reg&15)<<4);	break; }
//		if(imm==-2)		{ opw1=0x3409|((reg&15)<<4);	break; }
//		if(imm==-4)		{ opw1=0x340A|((reg&15)<<4);	break; }
//		if(imm==-8)		{ opw1=0x340B|((reg&15)<<4);	break; }
//		if(imm==-16)	{ opw1=0x340E|((reg&15)<<4);	break; }
//		if(imm==-32)	{ opw1=0x3602|((reg&15)<<4);	break; }
#endif

		break;

		odr=1; opw1=0x3606|((reg&15)<<4);				break;
	case BGBCC_SH_NMID_SHLD:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
#if 1
		if(imm==1)		{ opw1=0x3400|((reg&15)<<4);	break; }
		if(imm==2)		{ opw1=0x3401|((reg&15)<<4);	break; }
		if(imm==4)		{ opw1=0x3402|((reg&15)<<4);	break; }
		if(imm==8)		{ opw1=0x3403|((reg&15)<<4);	break; }
		if(imm==16)		{ opw1=0x340C|((reg&15)<<4);	break; }
		if(imm==32)		{ opw1=0x3600|((reg&15)<<4);	break; }
//		if(imm==-1)		{ opw1=0x3404|((reg&15)<<4);	break; }
//		if(imm==-2)		{ opw1=0x3405|((reg&15)<<4);	break; }
//		if(imm==-4)		{ opw1=0x3406|((reg&15)<<4);	break; }
//		if(imm==-8)		{ opw1=0x3407|((reg&15)<<4);	break; }
//		if(imm==-16)	{ opw1=0x340D|((reg&15)<<4);	break; }
//		if(imm==-32)	{ opw1=0x3601|((reg&15)<<4);	break; }
#endif

		break;

		odr=1; opw1=0x3607|((reg&15)<<4);				break;


	case BGBCC_SH_NMID_SHADQ:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
#if 1
		if(imm==1)		{ opw1=0x3400|((reg&15)<<4);	break; }
		if(imm==2)		{ opw1=0x3401|((reg&15)<<4);	break; }
		if(imm==4)		{ opw1=0x3402|((reg&15)<<4);	break; }
		if(imm==8)		{ opw1=0x3403|((reg&15)<<4);	break; }
		if(imm==16)		{ opw1=0x340C|((reg&15)<<4);	break; }
		if(imm==32)		{ opw1=0x3600|((reg&15)<<4);	break; }
		if(imm==-1)		{ opw1=0x3408|((reg&15)<<4);	break; }
		if(imm==-2)		{ opw1=0x3409|((reg&15)<<4);	break; }
		if(imm==-4)		{ opw1=0x340A|((reg&15)<<4);	break; }
		if(imm==-8)		{ opw1=0x340B|((reg&15)<<4);	break; }
		if(imm==-16)	{ opw1=0x340E|((reg&15)<<4);	break; }
		if(imm==-32)	{ opw1=0x3602|((reg&15)<<4);	break; }
#endif
		odr=1; opw1=0x3806|((reg&15)<<4);				break;
	case BGBCC_SH_NMID_SHLDQ:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
#if 1
		if(imm==1)		{ opw1=0x3400|((reg&15)<<4);	break; }
		if(imm==2)		{ opw1=0x3401|((reg&15)<<4);	break; }
		if(imm==4)		{ opw1=0x3402|((reg&15)<<4);	break; }
		if(imm==8)		{ opw1=0x3403|((reg&15)<<4);	break; }
		if(imm==16)		{ opw1=0x340C|((reg&15)<<4);	break; }
		if(imm==32)		{ opw1=0x3600|((reg&15)<<4);	break; }
		if(imm==-1)		{ opw1=0x3404|((reg&15)<<4);	break; }
		if(imm==-2)		{ opw1=0x3405|((reg&15)<<4);	break; }
		if(imm==-4)		{ opw1=0x3406|((reg&15)<<4);	break; }
		if(imm==-8)		{ opw1=0x3407|((reg&15)<<4);	break; }
		if(imm==-16)	{ opw1=0x340D|((reg&15)<<4);	break; }
		if(imm==-32)	{ opw1=0x3601|((reg&15)<<4);	break; }
#endif
		odr=1; opw1=0x3807|((reg&15)<<4);				break;

#endif
	}

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, reg)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, reg)) ex|=0x0020;

		switch(nmid)
		{
		case BGBCC_SH_NMID_ADD:
			if(((s16)imm)==imm)
			{
				opw1=0xF840|(reg&31);
				opw2=(u16)imm;
				break;
			}

			if(((s32)imm)==imm)
			{
				opw1=0xFCC0|(reg&15)|((reg&0x10)<<4);
				opw2=(u16)(imm    );
				opw3=(u16)(imm>>16);
				break;
			}

			break;
		case BGBCC_SH_NMID_SUB:
			if(((s16)imm1n)==imm1n)
			{
				opw1=0xF840|(reg&31);
				opw2=(u16)imm1n;
				break;
			}

			if(((s32)imm1n)==imm1n)
			{
				opw1=0xFCC0|(reg&15)|((reg&0x10)<<4);
				opw2=(u16)(imm1n    );
				opw3=(u16)(imm1n>>16);
				break;
			}
			break;

		case BGBCC_SH_NMID_MOV:
//			if(((s16)imm)==imm)
			if((((s16)imm)==imm) || (((u16)imm)==imm))
			{
				opw1=0xF800|(reg&31)|((imm>>16)&0x0020);
				opw2=(u16)imm;
				break;
			}

			if(((s32)imm)==imm)
			{
				opw1=0xFCE0|(reg&15)|((reg&0x10)<<4);
				opw2=(u16)(imm    );
				opw3=(u16)(imm>>16);
				break;
			}
			break;

		case BGBCC_SH_NMID_SHAD:
			if((imm&31)==imm)
			{	opw1=0xF000|ex|(imm&31);
				opw2=0x5600|((reg&15)<<4)|((reg&15)<<0);
				break;			}
			if((imm|(~31))==imm)
			{	opw1=0xF080|ex|(imm&31);
				opw2=0x5600|((reg&15)<<4)|((reg&15)<<0);
				break;			}

			opw1=0xF006|ex; odr=1;
			opw2=0x1D00|((reg&15)<<4)|((reg&15)<<0);		break;
		case BGBCC_SH_NMID_SHLD:
			if((imm&31)==imm)
			{	opw1=0xF000|ex|(imm&31);
				opw2=0x5700|((reg&15)<<4)|((reg&15)<<0);
				break;			}
			if((imm|(~31))==imm)
			{	opw1=0xF080|ex|(imm&31);
				opw2=0x5700|((reg&15)<<4)|((reg&15)<<0);
				break;			}

			opw1=0xF007|ex; odr=1;
			opw2=0x1D00|((reg&15)<<4)|((reg&15)<<0);		break;

		case BGBCC_SH_NMID_SHADQ:
			opw1=0xF086|ex; odr=1;
			opw2=0x1D00|((reg&15)<<4)|((reg&15)<<0);		break;
		case BGBCC_SH_NMID_SHLDQ:
			opw1=0xF087|ex; odr=1;
			opw2=0x1D00|((reg&15)<<4)|((reg&15)<<0);		break;

		case BGBCC_SH_NMID_CMPEQ:	
			if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			{	opw1=0xF000|(reg&31)|((imm>>3)&0x0060);
				opw2=0x2C00|(imm&0x00FF)|((imm>>10)&0x0100);
				break;		}
			opw1=0xF00C|(ex&0x0040); odr=1;
			opw2=0x1900|((reg&15)<<4);
			break;
		case BGBCC_SH_NMID_CMPHI:	
			opw1=0xF00D|(ex&0x0040); odr=1;
			opw2=0x1900|((reg&15)<<4);
			break;
		case BGBCC_SH_NMID_CMPGT:	
			if((imm&1023)==imm)
			{	opw1=0xF000|(reg&31)|((imm>>3)&0x0060);
				opw2=0x2E00|(imm&0x00FF);
				break;		}		
			opw1=0xF00E|(ex&0x0040); odr=1;
			opw2=0x1900|((reg&15)<<4);
			break;
		}
	}
	

	if(opw1>=0)
	{
		if(odr)
			BGBCC_JX2_EmitLoadDrImm(ctx, imm);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpRegImmReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int imm, int rn)
{
	if(!BGBCC_JX2_TryEmitOpRegImmReg(ctx, nmid, rm, imm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpRegImmReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int imm, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpRegImmReg(ctx, nmid, rm, imm, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpRegImmReg(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int imm, int rn)
{
	int opw1, opw2, opw3, ex;
	int imm1, odr;
	int i;

	if(nmid==BGBCC_SH_NMID_AND)
	{
		imm1=-1;

		if(imm==255)
			{ imm1=BGBCC_SH_NMID_EXTUB; }
		if(imm==65535)
			{ imm1=BGBCC_SH_NMID_EXTUW; }
		if(imm1>0)
			return(BGBCC_JX2_TryEmitOpRegReg(ctx, imm1, rm, rn));
	}

	opw1=-1; opw2=-1; opw3=-1;
	odr=0;

#if 1
	switch(nmid)
	{
	case BGBCC_SH_NMID_ADD:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		
//		if((imm>=0) && (imm<=31))	break;
		if((imm>=-31) && (imm<=31))	break;
		if((imm>=-65535) && (imm<=65535))	break;
		
//		odr=1; opw1=0x3800|((rn&15)<<4)|((rm&15)<<0);			break;
		odr=1; opw1=0x5800|((rn&15)<<4)|((rm&15)<<0);			break;
		break;
	case BGBCC_SH_NMID_SUB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;

//		if((imm>=0) && (imm<=31))	break;
		if((imm>=-31) && (imm<=31))	break;
		if((imm>=-65535) && (imm<=65535))	break;
		
//		odr=1; opw1=0x3900|((rn&15)<<4)|((rm&15)<<0);			break;
		odr=1; opw1=0x5900|((rn&15)<<4)|((rm&15)<<0);			break;
		break;
	case BGBCC_SH_NMID_AND:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;

		if((imm>=0) && (imm<=31))	break;
		if((imm>=0) && (imm<=65535))	break;

#if 0
		if(imm==255)
		{	opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
			opw2=0x3204|((rn&15)<<4);	break;	}
		if(imm==65535)
		{	opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
			opw1=0x3205|((rn&15)<<4);	break;	}
#endif

//		odr=1; opw1=0x3A00|((rn&15)<<4)|((rm&15)<<0);			break;
		odr=1; opw1=0x5A00|((rn&15)<<4)|((rm&15)<<0);			break;
		break;
	case BGBCC_SH_NMID_OR:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;

		if((imm>=0) && (imm<=31))	break;
		if((imm>=0) && (imm<=65535))	break;

//		odr=1; opw1=0x3B00|((rn&15)<<4)|((rm&15)<<0);			break;
		odr=1; opw1=0x5B00|((rn&15)<<4)|((rm&15)<<0);			break;
		break;
	case BGBCC_SH_NMID_XOR:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;

		if((imm>=0) && (imm<=31))	break;
		if((imm>=0) && (imm<=65535))	break;

//		odr=1; opw1=0x3C00|((rn&15)<<4)|((rm&15)<<0);			break;
		odr=1; opw1=0x5C00|((rn&15)<<4)|((rm&15)<<0);			break;
		break;
	default:
		break;
	}
#endif

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;

		switch(nmid)
		{
		case BGBCC_SH_NMID_ADD:
			if((imm>=0) && (imm<=31))
			{	opw1=0xF080|ex|(imm&31);
				opw2=0x1000|((rn&15)<<4)|((rm&15)<<0);		break;	}
			if((imm>=-31) && (imm<=0))
			{	opw1=0xF080|ex|((-imm)&31);
				opw2=0x1100|((rn&15)<<4)|((rm&15)<<0);		break;	}

#if 1
			if((imm>=0) && (imm<=65535))
			{	opw1=0xFC08|(ex>>4)|((imm>>16)&1);
				opw2=0x1000|((rn&15)<<4)|((rm&15)<<0);
				opw3=(u16)(imm);
				break;
			}
#endif
#if 1
			if((imm>=-65535) && (imm<=0))
			{	opw1=0xFC08|(ex>>4);
				opw2=0x1100|((rn&15)<<4)|((rm&15)<<0);
				opw3=(u16)(-imm);
				break;
			}
#endif

			opw1=0xF008|ex; odr=1;
			opw2=0x1D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_SUB:
			if((imm>=0) && (imm<=31))
			{	opw1=0xF080|ex|(imm&31);
				opw2=0x1100|((rn&15)<<4)|((rm&15)<<0);		break;	}
			if((imm>=-31) && (imm<=0))
			{	opw1=0xF080|ex|((-imm)&31);
				opw2=0x1000|((rn&15)<<4)|((rm&15)<<0);		break;	}

#if 1
			if((imm>=0) && (imm<=65535))
			{	opw1=0xFC08|(ex>>4)|((imm>>16)&1);
				opw2=0x1100|((rn&15)<<4)|((rm&15)<<0);
				opw3=(u16)(imm);
				break;
			}
			if((imm>=-65535) && (imm<=0))
			{	opw1=0xFC08|(ex>>4);
				opw2=0x1000|((rn&15)<<4)|((rm&15)<<0);
				opw3=(u16)(-imm);
				break;
			}
#endif

			opw1=0xF009|ex; odr=1;
			opw2=0x1D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_AND:
			if((imm>=0) && (imm<=31))
			{	opw1=0xF080|ex|(imm&31);
				opw2=0x1500|((rn&15)<<4)|((rm&15)<<0);		break;	}
#if 1
			if((imm>=0) && (imm<=65535))
			{	opw1=0xFC08|(ex>>4);
				opw2=0x1500|((rn&15)<<4)|((rm&15)<<0);
				opw3=(u16)(imm);	break;		}
#endif
			opw1=0xF00A|ex; odr=1;
			opw2=0x1D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_OR:
			if((imm>=0) && (imm<=31))
			{	opw1=0xF080|ex|(imm&31);
				opw2=0x1600|((rn&15)<<4)|((rm&15)<<0);		break;	}
#if 1
			if((imm>=0) && (imm<=65535))
			{	opw1=0xFC08|(ex>>4);
				opw2=0x1600|((rn&15)<<4)|((rm&15)<<0);
				opw3=(u16)(imm);	break;		}
#endif
			opw1=0xF00B|ex; odr=1;
			opw2=0x1D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_XOR:
			if((imm>=0) && (imm<=31))
			{	opw1=0xF080|ex|(imm&31);
				opw2=0x1700|((rn&15)<<4)|((rm&15)<<0);		break;	}
#if 1
			if((imm>=0) && (imm<=65535))
			{	opw1=0xFC08|(ex>>4);
				opw2=0x1700|((rn&15)<<4)|((rm&15)<<0);
				opw3=(u16)(imm);	break;		}
#endif
			opw1=0xF00C|ex; odr=1;
			opw2=0x1D00|((rn&15)<<4)|((rm&15)<<0);			break;

#if 0
		case BGBCC_SH_NMID_SHAD:
			opw1=0xF006|ex; odr=1;
			opw2=0x1D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_SHLD:
			opw1=0xF007|ex; odr=1;
			opw2=0x1D00|((rn&15)<<4)|((rm&15)<<0);			break;
#endif

#if 1
		case BGBCC_SH_NMID_SHAD:
			if((imm&31)==imm)
			{	opw1=0xF000|ex|(imm&31);
				opw2=0x5600|((rn&15)<<4)|((rm&15)<<0);
				break;			}
			if((imm|(~31))==imm)
			{	opw1=0xF080|ex|(imm&31);
				opw2=0x5600|((rn&15)<<4)|((rm&15)<<0);
				break;			}
			opw1=0xF006|ex; odr=1;
			opw2=0x1D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_SHLD:
			if((imm&31)==imm)
			{	opw1=0xF000|ex|(imm&31);
				opw2=0x5700|((rn&15)<<4)|((rm&15)<<0);
				break;			}
			if((imm|(~31))==imm)
			{	opw1=0xF080|ex|(imm&31);
				opw2=0x5700|((rn&15)<<4)|((rm&15)<<0);
				break;			}
			opw1=0xF000|ex|(imm&31);
			opw2=0x5700|((rn&15)<<4)|((rm&15)<<0);			break;
#endif

		case BGBCC_SH_NMID_SHADQ:
			if((imm&31)==imm)
			{	opw1=0xF000|ex|(imm&31);
				opw2=0x5800|((rn&15)<<4)|((rm&15)<<0);
				break;			}
			if((imm|(~31))==imm)
			{	opw1=0xF080|ex|(imm&31);
				opw2=0x5800|((rn&15)<<4)|((rm&15)<<0);
				break;			}
			if(((imm&31)|32)==imm)
			{	opw1=0xF000|ex|(imm&31);
				opw2=0x5A00|((rn&15)<<4)|((rm&15)<<0);
				break;			}
			if(((imm|(~63))&(~32))==imm)
			{	opw1=0xF080|ex|(imm&31);
				opw2=0x5A00|((rn&15)<<4)|((rm&15)<<0);
				break;			}
			opw1=0xF086|ex; odr=1;
			opw2=0x1D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_SHLDQ:
			if((imm&31)==imm)
			{	opw1=0xF000|ex|(imm&31);
				opw2=0x5900|((rn&15)<<4)|((rm&15)<<0);
				break;			}
			if((imm|(~31))==imm)
			{	opw1=0xF080|ex|(imm&31);
				opw2=0x5900|((rn&15)<<4)|((rm&15)<<0);
				break;			}
			if(((imm&31)|32)==imm)
			{	opw1=0xF000|ex|(imm&31);
				opw2=0x5B00|((rn&15)<<4)|((rm&15)<<0);
				break;			}
			if(((imm|(~63))&(~32))==imm)
			{	opw1=0xF080|ex|(imm&31);
				opw2=0x5B00|((rn&15)<<4)|((rm&15)<<0);
				break;			}
			opw1=0xF087|ex; odr=1;
			opw2=0x1D00|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}

	if(opw1>=0)
	{
		if(odr)
			BGBCC_JX2_EmitLoadDrImm(ctx, imm);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		return(1);
	}

	return(0);
}
