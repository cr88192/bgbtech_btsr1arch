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
	int opw1, opw2, opw3, opw4;

	opw1=-1; opw2=-1;
	opw3=-1; opw4=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_NOP:		opw1=0x3000; break;
	case BGBCC_SH_NMID_RTS:		opw1=0x3010; break;
	case BGBCC_SH_NMID_RTSN:	opw1=0x3010; break;
	case BGBCC_SH_NMID_RTSU:	opw1=0x3012; break;
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
	case BGBCC_SH_NMID_LDTLB:	opw1=0x30F0; break;
	case BGBCC_SH_NMID_INVTLB:	opw1=0x30F2; break;

	case BGBCC_SH_NMID_SYSCALL:	opw1=0x3022; break;

//	case BGBCC_SH_NMID_CLRMAC:	opw1=0x0028; break;

//	case BGBCC_SH_NMID_RET:		opw1=0x300B; break;
//	case BGBCC_SH_NMID_RET:		opw1=0x310B; break;
	case BGBCC_SH_NMID_RET:
//		opw1=0x310B;
		opw1=0x311B;
		opw2=0x3010;
		break;

	default:
		break;
	}

//	if(ctx->is_fixed32)
	if(ctx->is_fixed32 || ctx->op_is_wex2)
	{
//		if(opw1>=0)
//		if((opw1&0xF000)==0x3000)
		if(((opw1&0xF000)==0x3000) && ((opw2&0xF000)==0x3000))
		{
			opw4=0x3000|((opw2>>4)&0x00F0)|(opw2&0x000F);
			opw3=0xF000|((opw2>>4)&0x000F);

			opw2=0x3000|((opw1>>4)&0x00F0)|(opw1&0x000F);
			opw1=0xF000|((opw1>>4)&0x000F);
		}else
			if(((opw1&0xF000)==0x3000) && (opw2<0))
		{
//			opw1=-1; odr=0;
			opw2=0x3000|((opw1>>4)&0x00F0)|(opw1&0x000F);
			opw1=0xF000|((opw1>>4)&0x000F);
		}else
		{
			opw1=-1;
		}
	}
	
	if(opw1>=0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpNone(ctx, nmid);

//		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
		BGBCC_JX2_EmitOpCheckRepack4(ctx, &opw1, &opw2, &opw3, &opw4);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
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
	int opw1, opw2, opw3, reg2, ex, ex2;
	int i;

#if 0
	if(!(ctx->has_pushx2))
	{
		if(nmid==BGBCC_SH_NMID_PUSHX2)
		{
			i=BGBCC_JX2_TryEmitOpReg(ctx, BGBCC_SH_NMID_PUSH, reg+1);
			if(i<=0)return(i);
			i=BGBCC_JX2_TryEmitOpReg(ctx, BGBCC_SH_NMID_PUSH, reg+0);
			return(i);
		}

		if(nmid==BGBCC_SH_NMID_POPX2)
		{
			i=BGBCC_JX2_TryEmitOpReg(ctx, BGBCC_SH_NMID_POP, reg+0);
			if(i<=0)return(i);
			i=BGBCC_JX2_TryEmitOpReg(ctx, BGBCC_SH_NMID_POP, reg+1);
			return(i);
		}
	}
#endif

	ex=0;
	if(BGBCC_JX2_EmitCheckRegExt4(ctx, reg)) ex|=0x0040;
	if(BGBCC_JX2_EmitCheckRegExt4(ctx, reg)) ex|=0x0020;
	ex2=ex<<4;

	opw1=-1; opw2=-1; opw3=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_BSRF:
//		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtAddrGPR(ctx, reg))	break;
//		opw1=0x3609|((reg&15)<<4); break;
//		opw1=0x3101|((reg&15)<<4); break;
		opw1=0x3101|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_BRAF:
//		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtAddrGPR(ctx, reg))	break;
//		opw1=0x3608|((reg&15)<<4); break;
//		opw1=0x3100|((reg&15)<<4); break;
		opw1=0x3100|((reg&15)<<4)|((reg&16)<<7); break;

	case BGBCC_SH_NMID_BSR2F:
//		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtAddrGPR(ctx, reg))	break;
		if(BGBCC_JX2_CheckPadAlign32(ctx))				break;
//		opw1=0x3609|((reg&15)<<4); break;
//		opw1=0x3101|((reg&15)<<4); break;
		opw1=0x3101|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_BRA2F:
//		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtAddrGPR(ctx, reg))	break;
		if(BGBCC_JX2_CheckPadAlign32(ctx))				break;
//		opw1=0x3608|((reg&15)<<4); break;
//		opw1=0x3100|((reg&15)<<4); break;
		opw1=0x3100|((reg&15)<<4)|((reg&16)<<7); break;

	case BGBCC_SH_NMID_INVIC:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x310C|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_INVDC:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x310D|((reg&15)<<4)|((reg&16)<<7); break;

	case BGBCC_SH_NMID_MOVT:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
//		opw1=0x360F|((reg&15)<<4); break;
		opw1=0x360F|((reg&15)<<4)|((reg&16)<<7); break;

	case BGBCC_SH_NMID_MOVNT:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
//		opw1=0x3303|((reg&15)<<4); break;
		opw1=0x3303|((reg&15)<<4)|((reg&16)<<7); break;

//	case BGBCC_SH_NMID_MOVRT:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))		break;
//		opw=0x0039|((reg&15)<<8); break;

	case BGBCC_SH_NMID_CMPEQ:	/* pseudo-op */
	case BGBCC_SH_NMID_TST:		/* pseudo-op */
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))		break;
//		opw1=0x3104|((reg&15)<<4)|((reg&16)<<7); break;
		opw1=0x2C00|((reg&15)<<4)|((reg&16)<<10); break;

//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))		break;
//		opw1=0x1400|((reg&15)<<4)|((reg&15)<<0); break;

	case BGBCC_SH_NMID_CMPQEQ:	/* pseudo-op */
	case BGBCC_SH_NMID_TSTQ:	/* pseudo-op */
//		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))		break;
//		opw1=0x3105|((reg&15)<<4)|((reg&16)<<7); break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))		break;
		opw1=0x5400|((reg&15)<<4)|((reg&15)<<0); break;

	case BGBCC_SH_NMID_CMPPL:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))		break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))		break;
//		opw1=0x3108|((reg&15)<<4); break;
//		opw1=0x310C|((reg&15)<<4); break;
//		opw1=0x310C|((reg&15)<<4)|((reg&16)<<7); break;
		opw1=0x2E00|((reg&15)<<4)|((reg&16)<<10); break;
	case BGBCC_SH_NMID_CMPPZ:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))		break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))		break;
//		opw1=0x3109|((reg&15)<<4); break;
//		opw1=0x310D|((reg&15)<<4); break;
//		opw1=0x310D|((reg&15)<<4)|((reg&16)<<7); break;
		opw1=0x2F00|((reg&15)<<4)|((reg&16)<<10); break;

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
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
//		opw1=0x3304|((reg&15)<<4);	break;
		opw1=0x3304|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_ROTR:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
//		opw1=0x3305|((reg&15)<<4);	break;
		opw1=0x3305|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_ROTCL:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
//		opw1=0x3306|((reg&15)<<4);	break;
		opw1=0x3306|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_ROTCR:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
//		opw1=0x3307|((reg&15)<<4);	break;
		opw1=0x3307|((reg&15)<<4)|((reg&16)<<7); break;

#if 0
	case BGBCC_SH_NMID_SHLL1:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
//		opw1=0x3400|((reg&15)<<4); break;
		opw1=0x3400|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SHLL2:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
//		opw1=0x3401|((reg&15)<<4); break;
		opw1=0x3401|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SHLL4:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
//		opw1=0x3402|((reg&15)<<4); break;
		opw1=0x3402|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SHLL8:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
//		opw1=0x3403|((reg&15)<<4); break;
		opw1=0x3403|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SHLL16:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
//		opw1=0x340C|((reg&15)<<4); break;
		opw1=0x340C|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SHLL32:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
//		opw1=0x3600|((reg&15)<<4); break;
		opw1=0x3600|((reg&15)<<4)|((reg&16)<<7); break;
#endif

#if 0
	case BGBCC_SH_NMID_SHLR1:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
//		opw1=0x3404|((reg&15)<<4); break;
		opw1=0x3404|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SHLR2:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
//		opw1=0x3405|((reg&15)<<4); break;
		opw1=0x3405|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SHLR4:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
//		opw1=0x3406|((reg&15)<<4); break;
		opw1=0x3406|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SHLR8:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x3407|((reg&15)<<4)|((reg&16)<<7); break;
#endif

#if 0
	case BGBCC_SH_NMID_SHLR16:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x340D|((reg&15)<<4)|((reg&16)<<7); break;
#endif
#if 0
	case BGBCC_SH_NMID_SHLR32:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x3601|((reg&15)<<4)|((reg&16)<<7); break;
#endif

#if 0
	case BGBCC_SH_NMID_SHAR1:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x3408|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SHAR2:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x3409|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SHAR4:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x340A|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SHAR8:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x340B|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SHAR16:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x340E|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SHAR32:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x3602|((reg&15)<<4)|((reg&16)<<7); break;

	case BGBCC_SH_NMID_SHARX:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x340F|((reg&15)<<4)|((reg&16)<<7); break;
#endif

	case BGBCC_SH_NMID_JSR:
	case BGBCC_SH_NMID_BSR:
//		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, reg))	break;
//		if(!BGBCC_JX2_EmitCheckRegExtAddrGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x3201|((reg&15)<<4)|((reg&16)<<7); break;
//		if(BGBCC_JX2_EmitCheckRegAddrGPR(ctx, reg))
//			{ opw1=0x3201|((reg&15)<<4); break; }
//		if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
//			{ opw1=0x3101|((reg&15)<<4); break; }
		break;

	case BGBCC_SH_NMID_JMP:
	case BGBCC_SH_NMID_BRA:
//		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, reg))	break;
//		if(!BGBCC_JX2_EmitCheckRegExtAddrGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x3200|((reg&15)<<4)|((reg&16)<<7); break;
//		if(BGBCC_JX2_EmitCheckRegAddrGPR(ctx, reg))
//			{ opw1=0x3200|((reg&15)<<4); break; }
//		if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
//			{ opw1=0x3100|((reg&15)<<4); break; }

	case BGBCC_SH_NMID_PUSH:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;

		if(ctx->is_fixed32 || ctx->op_is_wex2)
		{
			if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
				opw1=0xF000|(reg&31);
				opw2=0x3018;
				break;
			}
		}

		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
//			{ opw1=0x3008|((reg&15)<<4); break; }
			{ opw1=0x3108|((reg&15)<<4); break; }

//		if(BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))
//			{ opw1=0x3802|((reg&15)<<4); break; }
		if(BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))
			{ opw1=0x310E|((reg&15)<<4); break; }

		reg2=BGBCC_JX2_TryMapRegCn(ctx, reg);
		if(reg2>=0)
//			{ opw1=0x3009|((reg2&15)<<4); break; }
			{ opw1=0x3109|((reg2&15)<<4); break; }
//		if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
//			{ opw1=0x3208|((reg&15)<<4); break; }
//		if(BGBCC_JX2_EmitCheckRegExtFPR(ctx, reg))
//			{ opw1=0x380A|((reg&15)<<4); break; }

		if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{ opw1=0x3908|((reg&15)<<4); break; }
		if(BGBCC_JX2_EmitCheckRegExtFPR(ctx, reg))
			{ opw1=0x390E|((reg&15)<<4); break; }
		break;
	case BGBCC_SH_NMID_POP:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;

		if(ctx->is_fixed32 || ctx->op_is_wex2)
		{
			if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
				opw1=0xF000|(reg&31);
				opw2=0x301A;
				break;
			}
		}

		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
//			{ opw1=0x300A|((reg&15)<<4); break; }
			{ opw1=0x310A|((reg&15)<<4); break; }

//		if(BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))
//			{ opw1=0x3803|((reg&15)<<4); break; }
		if(BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))
			{ opw1=0x310F|((reg&15)<<4); break; }

		reg2=BGBCC_JX2_TryMapRegCn(ctx, reg);
		if(reg2>=0)
//			{ opw1=0x300B|((reg2&15)<<4); break; }
			{ opw1=0x310B|((reg2&15)<<4); break; }
//		if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
//			{ opw1=0x320A|((reg&15)<<4); break; }
//		if(BGBCC_JX2_EmitCheckRegExtFPR(ctx, reg))
//			{ opw1=0x380B|((reg&15)<<4); break; }

		if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{ opw1=0x390A|((reg&15)<<4); break; }
		if(BGBCC_JX2_EmitCheckRegExtFPR(ctx, reg))
			{ opw1=0x390F|((reg&15)<<4); break; }
		break;

#if 1
	case BGBCC_SH_NMID_PUSHX2:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
		if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{ opw1=0x320C|((reg&15)<<4)|((reg&16)<<7); break; }
		break;
	case BGBCC_SH_NMID_POPX2:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
		if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{ opw1=0x320E|((reg&15)<<4)|((reg&16)<<7); break; }
		break;
#endif

	case BGBCC_SH_NMID_EXTUB:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x3204|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_EXTUW:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x3205|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_EXTSB:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x3206|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_EXTSW:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x3207|((reg&15)<<4)|((reg&16)<<7); break;

	case BGBCC_SH_NMID_NOT:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x3300|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_NEG:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x3301|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_NEGC:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x3302|((reg&15)<<4)|((reg&16)<<7); break;


	case BGBCC_SH_NMID_EXTUL:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x3604|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_EXTSL:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x3605|((reg&15)<<4)|((reg&16)<<7); break;

#if 0
	case BGBCC_SH_NMID_SWAPB:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
//		opw1=0x360A|((reg&15)<<4)|((reg&16)<<7); break;
		opw1=0x3700|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SWAPW:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
//		opw1=0x360B|((reg&15)<<4)|((reg&16)<<7); break;
		opw1=0x3701|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SWAP8B:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x3702|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SWAPL:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x3703|((reg&15)<<4)|((reg&16)<<7); break;

	case BGBCC_SH_NMID_SWAPLB:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x3704|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SWAPLW:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x3705|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SWCPLB:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x3706|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SWCPLW:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x3707|((reg&15)<<4)|((reg&16)<<7); break;

	case BGBCC_SH_NMID_SWAPMB:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x3708|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SWAPMW:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x3709|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SWCPMB:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x370A|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SWCPMW:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x370B|((reg&15)<<4)|((reg&16)<<7); break;

	case BGBCC_SH_NMID_SWAPHB:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x370C|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SWAPHW:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x370D|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SWCPHB:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x370E|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_SWCPHW:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))	break;
		opw1=0x370F|((reg&15)<<4)|((reg&16)<<7); break;
#endif

#if 0
	case BGBCC_SH_NMID_FLDCF:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F00|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_FLDCD:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F01|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_FLDCI:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F02|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_FLDCH:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F03|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_FSTCF:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F04|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_FSTCD:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F05|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_FSTCI:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F06|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_FSTCH:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F07|((reg&15)<<4)|((reg&16)<<7); break;

	case BGBCC_SH_NMID_FNEG:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F08|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_FABS:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F09|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_FRCPA:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F0A|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_FSQRTA:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F0B|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_FRCP:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F0C|((reg&15)<<4)|((reg&16)<<7); break;
	case BGBCC_SH_NMID_FSQRT:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0x9F0D|((reg&15)<<4)|((reg&16)<<7); break;
#endif

	default:
		break;
	}

	if(ctx->is_fixed32 || ctx->op_is_wex2)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_TST:
		case BGBCC_SH_NMID_TSTQ:
		case BGBCC_SH_NMID_CMPEQ:
		case BGBCC_SH_NMID_CMPQEQ:
			opw1=-1;
			break;
		}
	
		if((opw1&0xF000)==0x3000)
		{
//			opw2=0x3000|((opw1>>4)&0x00F0)|(opw1&0x000F);
			opw2=0x3000|((opw1>>4)&0x0070)|(opw1&0x000F);
//			opw1=0xF000|((opw1>>4)&0x000F);
			opw1=0xF000|((opw1>>4)&0x000F)|
//				((opw1>>5)&0x0040);
				((opw1>>7)&0x0010);
		}else
			if((opw1&0xF000)!=0xF000)
		{
			opw1=-1;
		}
	}

	if(opw1<0)
		switch(nmid)
	{
	case BGBCC_SH_NMID_PUSH:
		if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
		{
			opw1=0xF000|(reg&31);
			opw2=0x3018;
			break;
		}

		if(BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))
		{
			opw1=0xF000|(reg&31);
			opw2=0x3082;
			break;
		}

		reg2=BGBCC_JX2_TryMapRegCn(ctx, reg);
		if(reg2>=0)
		{
//			opw1=0xF000|(reg2&31);
			opw1=0xF000|(reg2&15);
			opw2=0x3019;
			break;
		}
		break;
	case BGBCC_SH_NMID_POP:
		if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
		{
			opw1=0xF000|(reg&31);
			opw2=0x301A;
			break;
		}

		if(BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))
		{
			opw1=0xF000|(reg&31);
			opw2=0x3083;
			break;
		}

		reg2=BGBCC_JX2_TryMapRegCn(ctx, reg);
		if(reg2>=0)
		{
//			opw1=0xF000|(reg2&31);
			opw1=0xF000|(reg2&15);
			opw2=0x301B;
			break;
		}
		break;

#if 0
	case BGBCC_SH_NMID_FLDCF:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0xF000;
		opw2=0x9F00|((reg&15)<<4); break;
	case BGBCC_SH_NMID_FLDCD:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0xF001;
		opw2=0x9F00|((reg&15)<<4); break;
	case BGBCC_SH_NMID_FLDCI:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0xF002;
		opw2=0x9F00|((reg&15)<<4); break;
	case BGBCC_SH_NMID_FLDCH:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0xF003;
		opw2=0x9F00|((reg&15)<<4); break;

	case BGBCC_SH_NMID_FSTCF:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0xF004;
		opw2=0x9F00|((reg&15)<<0); break;
	case BGBCC_SH_NMID_FSTCD:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0xF005;
		opw2=0x9F00|((reg&15)<<0); break;
	case BGBCC_SH_NMID_FSTCI:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0xF006;
		opw2=0x9F00|((reg&15)<<0); break;
	case BGBCC_SH_NMID_FSTCH:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0xF007;
		opw2=0x9F00|((reg&15)<<0); break;

	case BGBCC_SH_NMID_FNEG:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0xF008;
		opw2=0x9F00|((reg&15)<<4)|((reg&15)<<0); break;
	case BGBCC_SH_NMID_FABS:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0xF009;
		opw2=0x9F00|((reg&15)<<4)|((reg&15)<<0); break;
	case BGBCC_SH_NMID_FRCP:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0xF00C;
		opw2=0x9F00|((reg&15)<<4)|((reg&15)<<0); break;
	case BGBCC_SH_NMID_FSQRT:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, reg))	break;
		opw1=0xF00D;
		opw2=0x9F00|((reg&15)<<4)|((reg&15)<<0); break;
#endif

	default:
		break;
	}

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_JSR:
			opw1=0xF000|(reg&31);
			opw2=0x3021; break;
		case BGBCC_SH_NMID_JMP:
			opw1=0xF000|(reg&31);
			opw2=0x3020; break;

		case BGBCC_SH_NMID_BSRF:
			opw1=0xF000|(reg&31);
//			opw2=0x3069; break;
			opw2=0x3011; break;
		case BGBCC_SH_NMID_BRAF:
			opw1=0xF000|(reg&31);
//			opw2=0x3068; break;
			opw2=0x3010; break;

		case BGBCC_SH_NMID_BSR2F:
			if(BGBCC_JX2_CheckPadAlign32(ctx))
			{
				opw1=0xF000|(reg&31);
//				opw2=0x3069;
				opw2=0x3011;
			}else
			{
				if(ctx->is_fixed32 || ctx->op_is_wex2)
					break;

				opw1=0x3000;
				opw2=0xF000|(reg&31);
//				opw3=0x3069;
				opw3=0x3011;
			}
			break;
		case BGBCC_SH_NMID_BRA2F:
			if(BGBCC_JX2_CheckPadAlign32(ctx))
			{
				opw1=0x6F000|(reg&31);
//				opw2=0x63068; break;
				opw2=0x63010; break;
			}else
			{
				if(ctx->is_fixed32 || ctx->op_is_wex2)
					break;

				opw1=0x63000;
				opw2=0x6F000|(reg&31);
//				opw3=0x63068;
				opw3=0x63010;
			}
			break;

		case BGBCC_SH_NMID_MOVNT:
			opw1=0xF000|(reg&31);
			opw2=0x3033; break;

		case BGBCC_SH_NMID_TRAPA:
			opw1=0xF000|(reg&31);
			opw2=0x3068; break;

		case BGBCC_SH_NMID_MOVT:
			opw1=0xF000|(reg&31);
			opw2=0x306F; break;

		case BGBCC_SH_NMID_SWAPB:
			opw1=0xF000|ex;
			opw2=0x3700|((reg&15)<<4)|(reg&15); break;
		case BGBCC_SH_NMID_SWAPW:
			opw1=0xF001|ex;
			opw2=0x3700|((reg&15)<<4)|(reg&15); break;
		case BGBCC_SH_NMID_SWAP8B:
			opw1=0xF002|ex;
			opw2=0x3700|((reg&15)<<4)|(reg&15); break;
		case BGBCC_SH_NMID_SWAPL:
			opw1=0xF003|ex;
			opw2=0x3700|((reg&15)<<4)|(reg&15); break;

		case BGBCC_SH_NMID_SWAPLB:
			opw1=0xF004|ex;
			opw2=0x3700|((reg&15)<<4)|(reg&15); break;
		case BGBCC_SH_NMID_SWAPLW:
			opw1=0xF005|ex;
			opw2=0x3700|((reg&15)<<4)|(reg&15); break;
		case BGBCC_SH_NMID_SWCPLB:
			opw1=0xF006|ex;
			opw2=0x3700|((reg&15)<<4)|(reg&15); break;
		case BGBCC_SH_NMID_SWCPLW:
			opw1=0xF007|ex;
			opw2=0x3700|((reg&15)<<4)|(reg&15); break;

		case BGBCC_SH_NMID_SWAPMB:
			opw1=0xF008|ex;
			opw2=0x3700|((reg&15)<<4)|(reg&15); break;
		case BGBCC_SH_NMID_SWAPMW:
			opw1=0xF009|ex;
			opw2=0x3700|((reg&15)<<4)|(reg&15); break;
		case BGBCC_SH_NMID_SWCPMB:
			opw1=0xF00A|ex;
			opw2=0x3700|((reg&15)<<4)|(reg&15); break;
		case BGBCC_SH_NMID_SWCPMW:
			opw1=0xF00B|ex;
			opw2=0x3700|((reg&15)<<4)|(reg&15); break;

		case BGBCC_SH_NMID_SWAPHB:
			opw1=0xF00C|ex;
			opw2=0x3700|((reg&15)<<4)|(reg&15); break;
		case BGBCC_SH_NMID_SWAPHW:
			opw1=0xF00D|ex;
			opw2=0x3700|((reg&15)<<4)|(reg&15); break;
		case BGBCC_SH_NMID_SWCPHB:
			opw1=0xF00E|ex;
			opw2=0x3700|((reg&15)<<4)|(reg&15); break;
		case BGBCC_SH_NMID_SWCPHW:
			opw1=0xF00F|ex;
			opw2=0x3700|((reg&15)<<4)|(reg&15); break;

//		case BGBCC_SH_NMID_MOVT:
//			opw1=0xF000|ex;
//			opw2=0x1B00|((reg&15)<<4)|((reg&15)<<0);	break;

		case BGBCC_SH_NMID_CMPEQ:	/* pseudo-op */
		case BGBCC_SH_NMID_TST:		/* pseudo-op */
			opw1=0xF004|ex;
			opw2=0x1900|((reg&15)<<4)|((reg&15)<<0); break;

		case BGBCC_SH_NMID_CMPQEQ:	/* pseudo-op */
		case BGBCC_SH_NMID_TSTQ:	/* pseudo-op */
			opw2=0xF084|ex;
			opw1=0x1900|((reg&15)<<4)|((reg&15)<<0); break;


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
//			opw1=0xF019|ex;
//			opw2=0x1C00|((reg&15)<<4)|((reg&15)<<0);	break;
			opw1=0xF005|ex;
			opw2=0x1C00|((reg&15)<<4)|((reg&15)<<0);	break;
		case BGBCC_SH_NMID_EXTUL:
//			opw1=0xF099|ex;
//			opw2=0x1C00|((reg&15)<<4)|((reg&15)<<0);	break;
			opw1=0xF085|ex;
			opw2=0x1C00|((reg&15)<<4)|((reg&15)<<0);	break;

#if 0
//		case BGBCC_SH_NMID_SHLL1:
//			opw1=0xF000|ex;
//			opw2=0x1900|((reg&15)<<4)|((reg&15)<<0);	break;

		case BGBCC_SH_NMID_SHLL1:
			opw1=0xF001|ex;
			opw2=0x5900|((reg&15)<<4)|((reg&15)<<0);	break;
		case BGBCC_SH_NMID_SHLL2:
			opw1=0xF002|ex;
			opw2=0x5900|((reg&15)<<4)|((reg&15)<<0);	break;
		case BGBCC_SH_NMID_SHLL4:
			opw1=0xF004|ex;
			opw2=0x5900|((reg&15)<<4)|((reg&15)<<0);	break;
		case BGBCC_SH_NMID_SHLL8:
			opw1=0xF008|ex;
			opw2=0x5900|((reg&15)<<4)|((reg&15)<<0);	break;
		case BGBCC_SH_NMID_SHLL16:
			opw1=0xF010|ex;
			opw2=0x5900|((reg&15)<<4)|((reg&15)<<0);	break;
		case BGBCC_SH_NMID_SHLL32:
			opw1=0xF000|ex;
			opw2=0x5B00|((reg&15)<<4)|((reg&15)<<0);	break;

		case BGBCC_SH_NMID_SHLR1:
			opw1=0xF09F|ex;
			opw2=0x5900|((reg&15)<<4)|((reg&15)<<0);	break;
		case BGBCC_SH_NMID_SHLR2:
			opw1=0xF09E|ex;
			opw2=0x5900|((reg&15)<<4)|((reg&15)<<0);	break;
		case BGBCC_SH_NMID_SHLR4:
			opw1=0xF09C|ex;
			opw2=0x5900|((reg&15)<<4)|((reg&15)<<0);	break;
		case BGBCC_SH_NMID_SHLR8:
			opw1=0xF098|ex;
			opw2=0x5900|((reg&15)<<4)|((reg&15)<<0);	break;
		case BGBCC_SH_NMID_SHLR16:
			opw1=0xF090|ex;
			opw2=0x5900|((reg&15)<<4)|((reg&15)<<0);	break;
		case BGBCC_SH_NMID_SHLR32:
			opw1=0xF080|ex;
			opw2=0x5900|((reg&15)<<4)|((reg&15)<<0);	break;

		case BGBCC_SH_NMID_SHAR1:
			opw1=0xF09F|ex;
			opw2=0x5800|((reg&15)<<4)|((reg&15)<<0);	break;
		case BGBCC_SH_NMID_SHAR2:
			opw1=0xF09E|ex;
			opw2=0x5800|((reg&15)<<4)|((reg&15)<<0);	break;
		case BGBCC_SH_NMID_SHAR4:
			opw1=0xF09C|ex;
			opw2=0x5800|((reg&15)<<4)|((reg&15)<<0);	break;
		case BGBCC_SH_NMID_SHAR8:
			opw1=0xF098|ex;
			opw2=0x5800|((reg&15)<<4)|((reg&15)<<0);	break;
		case BGBCC_SH_NMID_SHAR16:
			opw1=0xF090|ex;
			opw2=0x5800|((reg&15)<<4)|((reg&15)<<0);	break;
		case BGBCC_SH_NMID_SHAR32:
			opw1=0xF080|ex;
			opw2=0x5800|((reg&15)<<4)|((reg&15)<<0);	break;
#endif

		case BGBCC_SH_NMID_FNEG:
			opw1=0xF008|ex;
			opw2=0x1D00|((reg&15)<<4)|(reg&15); break;
			break;
		case BGBCC_SH_NMID_FABS:
			opw1=0xF009|ex;
			opw2=0x1D00|((reg&15)<<4)|(reg&15); break;
			break;

#if 1
		case BGBCC_SH_NMID_SHLL1:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x9801|ex2;
			break;
		case BGBCC_SH_NMID_SHLL2:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x9802|ex2;
			break;
		case BGBCC_SH_NMID_SHLL4:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x9804|ex2;
			break;
		case BGBCC_SH_NMID_SHLL8:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x9808|ex2;
			break;
		case BGBCC_SH_NMID_SHLL16:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x9810|ex2;
			break;
		case BGBCC_SH_NMID_SHLL32:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x9820|ex2;
			break;
#endif

#if 0
		case BGBCC_SH_NMID_SHLR1:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x9881|ex2;
			break;
		case BGBCC_SH_NMID_SHLR2:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x9882|ex2;
			break;
		case BGBCC_SH_NMID_SHLR4:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x9884|ex2;
			break;
		case BGBCC_SH_NMID_SHLR8:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x9888|ex2;
			break;
		case BGBCC_SH_NMID_SHLR16:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x9890|ex2;
			break;
		case BGBCC_SH_NMID_SHLR32:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x98A0|ex2;
			break;

		case BGBCC_SH_NMID_SHAR1:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x8881|ex2;
			break;
		case BGBCC_SH_NMID_SHAR2:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x8882|ex2;
			break;
		case BGBCC_SH_NMID_SHAR4:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x8884|ex2;
			break;
		case BGBCC_SH_NMID_SHAR8:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x8888|ex2;
			break;
		case BGBCC_SH_NMID_SHAR16:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x8890|ex2;
			break;
		case BGBCC_SH_NMID_SHAR32:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x88A0|ex2;
			break;
#endif

#if 1
		case BGBCC_SH_NMID_SHLR1:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x98FF|ex2;
			break;
		case BGBCC_SH_NMID_SHLR2:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x98FE|ex2;
			break;
		case BGBCC_SH_NMID_SHLR4:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x98FC|ex2;
			break;
		case BGBCC_SH_NMID_SHLR8:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x98F8|ex2;
			break;
		case BGBCC_SH_NMID_SHLR16:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x98F0|ex2;
			break;
		case BGBCC_SH_NMID_SHLR32:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x98E0|ex2;
			break;

		case BGBCC_SH_NMID_SHAR1:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x88FF|ex2;
			break;
		case BGBCC_SH_NMID_SHAR2:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x88FE|ex2;
			break;
		case BGBCC_SH_NMID_SHAR4:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x88FC|ex2;
			break;
		case BGBCC_SH_NMID_SHAR8:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x88F8|ex2;
			break;
		case BGBCC_SH_NMID_SHAR16:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x88F0|ex2;
			break;
		case BGBCC_SH_NMID_SHAR32:
			opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
			opw2=0x88E0|ex2;
			break;
#endif

		case BGBCC_SH_NMID_CMPPL:
			opw1=0xF20E|((reg&15)<<4);
			opw2=0xC000|(ex2&0x0400);
			break;
		case BGBCC_SH_NMID_CMPPZ:
			opw1=0xF20A|((reg&15)<<4);
			opw2=0xC000|(ex2&0x0400);
			break;

		default:
			opw1=opw1;
			break;
		}
	}

	if(opw1>=0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpReg(ctx, nmid, reg);

//		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
		BGBCC_JX2_EmitOpCheckRepack3(ctx, &opw1, &opw2, &opw3);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
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
	int opw1, opw2, odr;

	opw1=-1; opw2=-1; odr=0;
	switch(nmid)
	{
	case BGBCC_SH_NMID_TRAPA:
//		if(((byte)imm)!=imm)	break;
		if((imm&15)==imm)
		{
			opw1=0x3603|(imm<<4);
			break;
		}
		opw1=0x3608; odr=1;
		break;

	case BGBCC_SH_NMID_WEXMD:
		if((imm&15)==imm)
			{ opw1=0x3609|((imm&15)<<4); break; }
		if((imm&31)==imm)
			{ opw1=0x3E09|((imm&15)<<4); break; }
		break;
	case BGBCC_SH_NMID_CPUID:
		if((imm&15)==imm)
			{ opw1=0x360A|((imm&15)<<4); break; }
		if((imm&31)==imm)
			{ opw1=0x3E0A|((imm&15)<<4); break; }
		break;

	case BGBCC_SH_NMID_SYSCALL:
		odr=1; opw1=0x3022;
		break;

	case BGBCC_SH_NMID_INVIC:
		odr=1; opw1=0x310C;
		break;
	case BGBCC_SH_NMID_INVDC:
		odr=1; opw1=0x310D;
		break;

#if 1
	case BGBCC_SH_NMID_LDSH8:	
		if(((byte)imm)!=imm)
			break;
		opw1=0x2600|(imm&255);
		break;
#endif

	default:
		break;
	}

	if(ctx->is_fixed32 || ctx->op_is_wex2)
	{
		if((opw1&0xF000)==0x3000)
		{
			opw2=0x3000|((opw1>>4)&0x00F0)|(opw1&0x000F);
			opw1=0xF000|((opw1>>4)&0x000F);
		}else
		{
			opw1=-1;
		}
	}
	
	if(opw1>=0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpImm(ctx, nmid, imm);

		if(odr)
		{
			if(imm%odr)
				{ BGBCC_DBGBREAK }
			BGBCC_JX2_EmitLoadDrImm(ctx, imm/odr);
		}

		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2, 0);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
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

	if(ctx->is_fixed32 || ctx->op_is_wex2)
	{
		if((opw1&0xF000)==0x3000)
		{
			opw2=0x3000|((opw1>>4)&0x00F0)|(opw1&0x000F);
			opw1=0xF000|((opw1>>4)&0x000F);
		}else
		{
			opw1=-1;
		}
	}

	if(opw1>=0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpMReg(ctx, nmid, reg);

		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2, 0);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
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
	case BGBCC_SH_REG_PC:		reg1=0x20; break;
	case BGBCC_SH_REG_PR:		reg1=0x21; break;
	case BGBCC_SH_REG_SR:		reg1=0x22; break;
	case BGBCC_SH_REG_VBR:		reg1=0x23; break;

//	case BGBCC_SH_REG_MACL:		reg1=0x24; break;
//	case BGBCC_SH_REG_MACH:		reg1=0x25; break;
	case BGBCC_SH_REG_SPC:		reg1=0x24; break;
	case BGBCC_SH_REG_SGR:		reg1=0x25; break;

	case BGBCC_SH_REG_GBR:		reg1=0x26; break;
	case BGBCC_SH_REG_TBR:		reg1=0x27; break;

	case BGBCC_SH_REG_TTB:		reg1=0x28; break;
	case BGBCC_SH_REG_TEA:		reg1=0x29; break;
	case BGBCC_SH_REG_MMCR:		reg1=0x2A; break;
	case BGBCC_SH_REG_EXSR:		reg1=0x2B; break;
	case BGBCC_SH_REG_STTB:		reg1=0x2C; break;
	case BGBCC_SH_REG_KRR:		reg1=0x2D; break;
	}
	return(reg1);
}

int BGBCC_JX2_TryMapRegSn(BGBCC_JX2_Context *ctx, int reg)
{
	int reg1;
	
	reg1=-1;
	switch(reg)
	{
//	case BGBCC_SH_REG_SPC:		reg1=0x48; break;
//	case BGBCC_SH_REG_SSR:		reg1=0x4A; break;
//	case BGBCC_SH_REG_SGR:		reg1=0x4B; break;

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
	int opw1, opw2, ex, ex3r;
	int rm3, rn3;

	if(
		(nmid==BGBCC_SH_NMID_ADDSL)	||
		(nmid==BGBCC_SH_NMID_ADDUL)	||
		(nmid==BGBCC_SH_NMID_SUBSL)	||
		(nmid==BGBCC_SH_NMID_SUBUL)	||
		(nmid==BGBCC_SH_NMID_SHAR)	||
		(nmid==BGBCC_SH_NMID_SHARQ) ||
		(nmid==BGBCC_SH_NMID_SHLR)	||
		(nmid==BGBCC_SH_NMID_SHLRQ) )
	{
		return(BGBCC_JX2_TryEmitOpRegRegReg(ctx, nmid, rn, rm, rn));
	}

	if(
		(nmid==BGBCC_SH_NMID_PADDF) ||
		(nmid==BGBCC_SH_NMID_PADDH) ||
		(nmid==BGBCC_SH_NMID_PADDFX) ||
		(nmid==BGBCC_SH_NMID_PADDXD) ||
		(nmid==BGBCC_SH_NMID_PSUBF) ||
		(nmid==BGBCC_SH_NMID_PSUBH) ||
		(nmid==BGBCC_SH_NMID_PSUBFX) ||
		(nmid==BGBCC_SH_NMID_PSUBXD) ||
		(nmid==BGBCC_SH_NMID_PMULF) ||
		(nmid==BGBCC_SH_NMID_PMULH) ||
		(nmid==BGBCC_SH_NMID_PMULFX) ||
		(nmid==BGBCC_SH_NMID_PMULXD) )
	{
		return(BGBCC_JX2_TryEmitOpRegRegReg(ctx, nmid, rn, rm, rn));
	}

	if(	(nmid==BGBCC_SH_NMID_PADDW) ||
		(nmid==BGBCC_SH_NMID_PADDL) ||
		(nmid==BGBCC_SH_NMID_PSUBW) ||
		(nmid==BGBCC_SH_NMID_PSUBL) ||
		(nmid==BGBCC_SH_NMID_PMULUW) ||
		(nmid==BGBCC_SH_NMID_PMULSW) ||
		(nmid==BGBCC_SH_NMID_PMULULW) ||
		(nmid==BGBCC_SH_NMID_PMULSLW) ||
		(nmid==BGBCC_SH_NMID_PMULUHW) ||
		(nmid==BGBCC_SH_NMID_PMULSHW) )
	{
		return(BGBCC_JX2_TryEmitOpRegRegReg(ctx, nmid, rn, rm, rn));
	}

	if(
		(nmid==BGBCC_SH_NMID_FADDX)	||
		(nmid==BGBCC_SH_NMID_FSUBX)	||
		(nmid==BGBCC_SH_NMID_FMULX)	||
		(nmid==BGBCC_SH_NMID_ADDX)	||
		(nmid==BGBCC_SH_NMID_SUBX)	||
		(nmid==BGBCC_SH_NMID_ADCX)	||
		(nmid==BGBCC_SH_NMID_SBBX)	||
		(nmid==BGBCC_SH_NMID_ANDX)	||
		(nmid==BGBCC_SH_NMID_ORX)	||
		(nmid==BGBCC_SH_NMID_XORX)  ||
		(nmid==BGBCC_SH_NMID_SHADX)	||
		(nmid==BGBCC_SH_NMID_SHLDX)	||
		(nmid==BGBCC_SH_NMID_SHARX)	||
		(nmid==BGBCC_SH_NMID_SHLRX)	||
		(nmid==BGBCC_SH_NMID_ROTLX)	)
	{
		return(BGBCC_JX2_TryEmitOpRegRegReg(ctx, nmid, rn, rm, rn));
	}

	if(rm==BGBCC_SH_REG_MACL)		rm=BGBCC_SH_REG_R0;
	if(rn==BGBCC_SH_REG_MACL)		rn=BGBCC_SH_REG_R0;
	if(rm==BGBCC_SH_REG_MACH)		rm=BGBCC_SH_REG_R1;
	if(rn==BGBCC_SH_REG_MACH)		rn=BGBCC_SH_REG_R1;

	if((rm==rn) && !(ctx->is_fixed32) && !(ctx->op_is_wex2))
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

	if(rm==rn)
	{
		if(nmid==BGBCC_SH_NMID_MOV)
			return(1);
	}


	ex=0;
	if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
	if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;

	ex3r=0;
	if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex3r|=0x0060;
	if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex3r|=0x0010;

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
				if(ctx->is_fixed32 || ctx->op_is_wex2)
				{
					opw1=0xF008|ex;
					opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);
				}else
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
				if(ctx->is_fixed32 || ctx->op_is_wex2)
				{
					opw1=0xF00A|ex;
					opw2=0x1900|((rn3&15)<<4)|((rm&15)<<0);
				}else
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
				if(ctx->is_fixed32 || ctx->op_is_wex2)
				{
					opw1=0xF00A|ex;
					opw2=0x1C00|((rn3&15)<<4)|((rm&15)<<0);
				}else
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
				if(ctx->is_fixed32 || ctx->op_is_wex2)
				{
					opw1=0xF00B|ex;
					opw2=0x1900|((rn&15)<<4)|((rm3&15)<<0);
				}else
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
				if(ctx->is_fixed32 || ctx->op_is_wex2)
				{
					opw1=0xF00B|ex;
					opw2=0x1C00|((rn&15)<<4)|((rm3&15)<<0);
				}else
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

	case BGBCC_SH_NMID_ADDSL:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1F00|((rn&15)<<4)|((rm&15)<<0);			break;

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

#if 1
	case BGBCC_SH_NMID_MULL:
	case BGBCC_SH_NMID_DMULS:
	case BGBCC_SH_NMID_MULSL:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
//		opw1=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;
		opw1=0x5600|((rn&15)<<4)|((rm&15)<<0);			break;
#endif

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
	case BGBCC_SH_NMID_CMPGE:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1F00|((rn&15)<<4)|((rm&15)<<0);			break;

#if 1
	case BGBCC_SH_NMID_MULUL:
	case BGBCC_SH_NMID_DMULU:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
//		opw1=0x1F00|((rn&15)<<4)|((rm&15)<<0);			break;
		opw1=0x5700|((rn&15)<<4)|((rm&15)<<0);			break;
#endif

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
//		opw1=0x5D00|((rn&15)<<4)|((rm&15)<<0);			break;
		opw1=0x5B00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_CMPQGT:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
//		opw1=0x5E00|((rn&15)<<4)|((rm&15)<<0);			break;
		opw1=0x5C00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_CMPQEQ:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
//		opw1=0x5F00|((rn&15)<<4)|((rm&15)<<0);			break;
		opw1=0x5500|((rn&15)<<4)|((rm&15)<<0);			break;
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
//		opw2=0x3806|((rn&15)<<4);			break;
		opw2=0x3106|((rn&15)<<4);			break;
	case BGBCC_SH_NMID_SHLDQ:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x1A40|(rm&15);
//		opw2=0x3807|((rn&15)<<4);			break;
		opw2=0x3107|((rn&15)<<4);			break;
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

#if 0
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
#endif

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

#if 0
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
#endif

	default:
		break;
	}

	if(opw1<0)
		switch(nmid)
	{
#if 1
	case BGBCC_SH_NMID_FADD:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x6000|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_FSUB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x6100|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_FMUL:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x6200|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_FLDCF:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x6300|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_FCMPEQ:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x6400|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_FCMPGT:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x6500|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_FSTCF:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x6600|((rn&15)<<4)|((rm&15)<<0);			break;
#endif

	default:
		break;
	}

	if(ctx->is_fixed32 || ctx->op_is_wex2)
	{
		if((opw1>=0) && ((opw1&0xF000)!=0xF000))
		{
			opw1=-1;
		}
	}

#if 0
	if(opw1<0)
		switch(nmid)
	{
	case BGBCC_SH_NMID_FADD:
		if(!BGBCC_JX2_EmitCheckRegExtFPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegExtFPR(ctx, rn))		break;
		opw1=0xF000|((rm&15)<<0)|(ex|((ex>>1)&0x0010));
		opw2=0x9800|((rn&15)<<4)|((rn&15)<<0);			break;
	case BGBCC_SH_NMID_FSUB:
		if(!BGBCC_JX2_EmitCheckRegExtFPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegExtFPR(ctx, rn))		break;
		opw1=0xF000|((rm&15)<<0)|(ex|((ex>>1)&0x0010));
		opw2=0x9900|((rn&15)<<4)|((rn&15)<<0);			break;
	case BGBCC_SH_NMID_FMUL:
		if(!BGBCC_JX2_EmitCheckRegExtFPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegExtFPR(ctx, rn))		break;
		opw1=0xF000|((rm&15)<<0)|(ex|((ex>>1)&0x0010));
		opw2=0x9A00|((rn&15)<<4)|((rn&15)<<0);			break;
	case BGBCC_SH_NMID_FDIV:
		if(!BGBCC_JX2_EmitCheckRegExtFPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegExtFPR(ctx, rn))		break;
		opw1=0xF000|((rm&15)<<0)|(ex|((ex>>1)&0x0010));
		opw2=0x9B00|((rn&15)<<4)|((rn&15)<<0);			break;

	case BGBCC_SH_NMID_FCMPEQ:
		if(!BGBCC_JX2_EmitCheckRegExtFPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegExtFPR(ctx, rn))		break;
		opw1=0xF00C|ex;
		opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_FCMPGT:
		if(!BGBCC_JX2_EmitCheckRegExtFPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegExtFPR(ctx, rn))		break;
		opw1=0xF00D|ex;
		opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);			break;

	case BGBCC_SH_NMID_FMOV:
//		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rm))		break;
//		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rn))		break;
//		opw1=0xF00E;
//		opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);			break;

		if(BGBCC_JX2_EmitCheckRegExtFPR(ctx, rm))
		{
			if(BGBCC_JX2_EmitCheckRegExtFPR(ctx, rn))
			{
				opw1=0xF00E|ex;
				opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);
			}else if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
			{
//				opw1=0xF01E|ex;
//				opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF005|ex;
				opw2=0x9F00|((rn&15)<<4)|((rm&15)<<0);
			}
		}else if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm))
		{
			if(BGBCC_JX2_EmitCheckRegExtFPR(ctx, rn))
			{
//				opw1=0xF09E|ex;
//				opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF001|ex;
				opw2=0x9F00|((rn&15)<<4)|((rm&15)<<0);
			}else if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
			{
				opw1=0xF008|ex;
				opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);
			}
		}
		break;

	case BGBCC_SH_NMID_FLDCF:
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegExtFPR(ctx, rn))		break;
		opw1=0xF000|ex;
		opw2=0x9F00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_FSTCF:
		if(!BGBCC_JX2_EmitCheckRegExtFPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))		break;
		opw1=0xF004|ex;
		opw2=0x9F00|((rn&15)<<4)|((rm&15)<<0);			break;

	case BGBCC_SH_NMID_FLDCI:
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegExtFPR(ctx, rn))		break;
		opw1=0xF002|ex;
		opw2=0x9F00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_FSTCI:
		if(!BGBCC_JX2_EmitCheckRegExtFPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))		break;
		opw1=0xF006|ex;
		opw2=0x9F00|((rn&15)<<4)|((rm&15)<<0);			break;

	case BGBCC_SH_NMID_FLDCH:
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegExtFPR(ctx, rn))		break;
		opw1=0xF003|ex;
		opw2=0x9F00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_FSTCH:
		if(!BGBCC_JX2_EmitCheckRegExtFPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))		break;
		opw1=0xF007|ex;
		opw2=0x9F00|((rn&15)<<4)|((rm&15)<<0);			break;

	default:
		break;
	}
#endif
	
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

		case BGBCC_SH_NMID_MOVD:
			opw1=0xF085|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;

//		case BGBCC_SH_NMID_MOVD:
		case BGBCC_SH_NMID_MOVX2:
			opw1=0xF088|ex;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_TRAPA:
			opw1=0xF08A|ex;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_ADDL:
			opw1=0xF080|ex;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_SUBL:
			opw1=0xF081|ex;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;


		case BGBCC_SH_NMID_ADDSL:
			ex|=(ex>>2)&0x0010;
			opw1=0xF000|ex|((rn&15)<<0);
			opw2=0x5C00|((rn&15)<<4)|((rm&15)<<0);			break;


		case BGBCC_SH_NMID_MOVHD:
			opw1=0xF008|ex;
			opw2=0x1A00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVLD:
			opw1=0xF088|ex;
			opw2=0x1A00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_MOVHLD:
			opw1=0xF00A|ex;
			opw2=0x1A00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVLHD:
			opw1=0xF08A|ex;
			opw2=0x1A00|((rn&15)<<4)|((rm&15)<<0);			break;

#if 0
		case BGBCC_SH_NMID_PADDW:
			opw1=0xF000|ex;
			opw2=0x1A00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_PSUBW:
			opw1=0xF001|ex;
			opw2=0x1A00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_PADDL:
			opw1=0xF080|ex;
			opw2=0x1A00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_PSUBL:
			opw1=0xF081|ex;
			opw2=0x1A00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_PMULUW:
			opw1=0xF00F|ex;
			opw2=0x1A00|((rn&15)<<4)|((rm&15)<<0);			break;
#endif

#if 0
		case BGBCC_SH_NMID_PADDH:
			opw1=0xF005|ex;
			opw2=0x1A00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_PSUBH:
			opw1=0xF006|ex;
			opw2=0x1A00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_PMULH:
			opw1=0xF007|ex;
			opw2=0x1A00|((rn&15)<<4)|((rm&15)<<0);			break;
#endif

#if 1
		case BGBCC_SH_NMID_MULL:
		case BGBCC_SH_NMID_DMULS:
		case BGBCC_SH_NMID_MULSL:
			opw1=0xF009|ex;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;
#endif
		
		case BGBCC_SH_NMID_CMPEQ:
			opw1=0xF00C|ex;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_CMPHI:
			opw1=0xF00D|ex;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_CMPGT:
			opw1=0xF00E|ex;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;

#if 1
		case BGBCC_SH_NMID_DMULU:
		case BGBCC_SH_NMID_MULUL:
			opw1=0xF00F|ex;
			opw2=0x1900|((rn&15)<<4)|((rm&15)<<0);			break;
#endif

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

		case BGBCC_SH_NMID_CMPXEQ:
			opw1=0xF00C|ex|0x0080;
			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_CMPXHI:
			opw1=0xF00D|ex|0x0080;
			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_CMPXGT:
			opw1=0xF00E|ex|0x0080;
			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;

#if 0
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
#endif

		case BGBCC_SH_NMID_NOT:
			opw1=0xF000|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_NEG:
			opw1=0xF001|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_CLZ:
			opw1=0xF002|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_CLZQ:
			opw1=0xF082|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_CTZ:
			opw1=0xF003|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_CTZQ:
			opw1=0xF083|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_BTRNS:
			opw1=0xF004|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_BTRNSQ:
			opw1=0xF084|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_PMORTL:
			opw1=0xF001|ex;
			opw2=0x1E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_PMORTQ:
			opw1=0xF081|ex;
			opw2=0x1E00|((rn&15)<<4)|((rm&15)<<0);			break;

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
//		case BGBCC_SH_NMID_EXTSL:
//			opw1=0xF019|ex;
//			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;
//		case BGBCC_SH_NMID_EXTUL:
//			opw1=0xF099|ex;
//			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_EXTSL:
			opw1=0xF005|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_EXTUL:
			opw1=0xF085|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;

#if 1
		case BGBCC_SH_NMID_CMPHS:
			opw1=0xF00F|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_CMPGE:
			opw1=0xF00E|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_CMPQHS:
			opw1=0xF08F|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_CMPQGE:
			opw1=0xF08E|ex;
			opw2=0x1C00|((rn&15)<<4)|((rm&15)<<0);			break;
#endif

#if 1
		case BGBCC_SH_NMID_PCMPEQH:
			opw1=0xF00A|ex;
			opw2=0x1A00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_PCMPEQF:
			opw1=0xF08A|ex;
			opw2=0x1A00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_PCMPGTH:
			opw1=0xF00B|ex;
			opw2=0x1A00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_PCMPGTF:
			opw1=0xF08B|ex;
			opw2=0x1A00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_PCMPEQW:
			opw1=0xF00C|ex;
			opw2=0x1A00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_PCMPEQL:
			opw1=0xF08C|ex;
			opw2=0x1A00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_PCMPHIW:
			opw1=0xF00D|ex;
			opw2=0x1A00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_PCMPHIL:
			opw1=0xF08D|ex;
			opw2=0x1A00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_PCMPGTW:
			opw1=0xF00E|ex;
			opw2=0x1A00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_PCMPGTL:
			opw1=0xF08E|ex;
			opw2=0x1A00|((rn&15)<<4)|((rm&15)<<0);			break;
#endif

		case BGBCC_SH_NMID_RGB5PCK32:
			opw1=0xF002|ex;
			opw2=0x1E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_RGB5PCK64:
			opw1=0xF082|ex;
			opw2=0x1E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_RGB5UPCK32:
			opw1=0xF003|ex;
			opw2=0x1E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_RGB5UPCK64:
			opw1=0xF083|ex;
			opw2=0x1E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_RGB32PCK64:
			opw1=0xF084|ex;
			opw2=0x1E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_RGB32UPCK64:
			opw1=0xF085|ex;
			opw2=0x1E00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_SWAPB:
			opw1=0xF000|ex;
			opw2=0x3700|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_SWAPW:
			opw1=0xF001|ex;
			opw2=0x3700|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_SWAP8B:
			opw1=0xF002|ex;
			opw2=0x3700|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_SWAPL:
			opw1=0xF003|ex;
			opw2=0x3700|((rn&15)<<4)|(rm&15); break;

		case BGBCC_SH_NMID_SWAPLB:
			opw1=0xF004|ex;
			opw2=0x3700|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_SWAPLW:
			opw1=0xF005|ex;
			opw2=0x3700|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_SWCPLB:
			opw1=0xF006|ex;
			opw2=0x3700|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_SWCPLW:
			opw1=0xF007|ex;
			opw2=0x3700|((rn&15)<<4)|(rm&15); break;

		case BGBCC_SH_NMID_SWAPMB:
			opw1=0xF008|ex;
			opw2=0x3700|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_SWAPMW:
			opw1=0xF009|ex;
			opw2=0x3700|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_SWCPMB:
			opw1=0xF00A|ex;
			opw2=0x3700|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_SWCPMW:
			opw1=0xF00B|ex;
			opw2=0x3700|((rn&15)<<4)|(rm&15); break;

		case BGBCC_SH_NMID_SWAPHB:
			opw1=0xF00C|ex;
			opw2=0x3700|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_SWAPHW:
			opw1=0xF00D|ex;
			opw2=0x3700|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_SWCPHB:
			opw1=0xF00E|ex;
			opw2=0x3700|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_SWCPHW:
			opw1=0xF00F|ex;
			opw2=0x3700|((rn&15)<<4)|(rm&15); break;


		case BGBCC_SH_NMID_FADD:
			opw1=0xF000|ex3r|((rm&15)<<0);
			opw2=0x5800|((rn&15)<<4)|((rn&15)<<0);			break;
		case BGBCC_SH_NMID_FSUB:
			opw1=0xF000|ex3r|((rm&15)<<0);
			opw2=0x5900|((rn&15)<<4)|((rn&15)<<0);			break;
		case BGBCC_SH_NMID_FMUL:
			opw1=0xF000|ex3r|((rm&15)<<0);
			opw2=0x5A00|((rn&15)<<4)|((rn&15)<<0);			break;

		case BGBCC_SH_NMID_FLDCF:
			opw1=0xF000|ex;
			opw2=0x1D00|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_FLDCFH:
			opw1=0xF001|ex;
			opw2=0x1D00|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_FLDCI:
			opw1=0xF002|ex;
			opw2=0x1D00|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_FLDCH:
			opw1=0xF003|ex;
			opw2=0x1D00|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_FSTCF:
			opw1=0xF004|ex;
			opw2=0x1D00|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_FSTCFH:
			opw1=0xF005|ex;
			opw2=0x1D00|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_FSTCI:
			opw1=0xF006|ex;
			opw2=0x1D00|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_FSTCH:
			opw1=0xF007|ex;
			opw2=0x1D00|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_FNEG:
			opw1=0xF008|ex;
			opw2=0x1D00|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_FABS:
			opw1=0xF009|ex;
			opw2=0x1D00|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_FCMPEQ:
			opw1=0xF00A|ex;
			opw2=0x1D00|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_FCMPGT:
			opw1=0xF00B|ex;
			opw2=0x1D00|((rn&15)<<4)|(rm&15); break;

		case BGBCC_SH_NMID_FCMPXEQ:
			opw1=0xF08A|ex;
			opw2=0x1D00|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_FCMPXGT:
			opw1=0xF08B|ex;
			opw2=0x1D00|((rn&15)<<4)|(rm&15); break;

		case BGBCC_SH_NMID_FLDCDX:
			opw1=0xF080|ex;
			opw2=0x1D00|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_FLDCIX:
			opw1=0xF082|ex;
			opw2=0x1D00|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_FSTCDX:
			opw1=0xF084|ex;
			opw2=0x1D00|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_FSTCIX:
			opw1=0xF086|ex;
			opw2=0x1D00|((rn&15)<<4)|(rm&15); break;

		case BGBCC_SH_NMID_PLDCH:
			opw1=0xF00C|ex;
			opw2=0x1E00|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_PLDCHH:
			opw1=0xF08C|ex;
			opw2=0x1E00|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_PSTCH:
			opw1=0xF00E|ex;
			opw2=0x1E00|((rn&15)<<4)|(rm&15); break;

		case BGBCC_SH_NMID_PLDCM8SH:
			opw1=0xF008|ex;
			opw2=0x1E00|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_PLDCM8UH:
			opw1=0xF088|ex;
			opw2=0x1E00|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_PSTCM8SH:
			opw1=0xF00A|ex;
			opw2=0x1E00|((rn&15)<<4)|(rm&15); break;
		case BGBCC_SH_NMID_PSTCM8UH:
			opw1=0xF08A|ex;
			opw2=0x1E00|((rn&15)<<4)|(rm&15); break;
		}
	}

	if(opw1>=0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpRegReg(ctx, nmid, rm, rn);

		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2, 0);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
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

int BGBCC_JX2_TryNormalizeXReg(
	BGBCC_JX2_Context *ctx, int nmid, int rs)
{
	if((rs&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_LR0)
	{
		rs&=31;
	}
	else if((rs&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RQ0)
	{
		rs&=31;
		if(rs&1)
			return(-1);
	}
	else if((rs&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_R0)
	{
		rs&=31; 
		if(rs&1)
			return(-1);
	}
	else
	{
		return(-1);
	}
	
	if(rs==0)
		return(-1);
	if(rs==14)
		return(-1);
	
	return(rs);
}

int BGBCC_JX2_TryEmitOpRegRegReg(
	BGBCC_JX2_Context *ctx, int nmid, int rs, int rt, int rn)
{
	int opw1, opw2, opw3, opw4;
	int i, ex;

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;

	switch(nmid)
	{
	case BGBCC_SH_NMID_PADDFX:
	case BGBCC_SH_NMID_PSUBFX:
	case BGBCC_SH_NMID_PMULFX:

	case BGBCC_SH_NMID_PADDXD:
	case BGBCC_SH_NMID_PSUBXD:
	case BGBCC_SH_NMID_PMULXD:

	case BGBCC_SH_NMID_FADDX:
	case BGBCC_SH_NMID_FSUBX:
	case BGBCC_SH_NMID_FMULX:
	case BGBCC_SH_NMID_FMACX:

	case BGBCC_SH_NMID_ADDX:
	case BGBCC_SH_NMID_SUBX:
	case BGBCC_SH_NMID_ADCX:
	case BGBCC_SH_NMID_SBBX:

	case BGBCC_SH_NMID_ANDX:
	case BGBCC_SH_NMID_ORX:
	case BGBCC_SH_NMID_XORX:

	case BGBCC_SH_NMID_BLINTW:

		rs=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rs);
		rt=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rt);
		rn=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rn);
		if((rs<0) || (rt<0) || (rn<0))
			return(0);
		break;

	case BGBCC_SH_NMID_SHADX:
	case BGBCC_SH_NMID_SHLDX:
	case BGBCC_SH_NMID_SHARX:
	case BGBCC_SH_NMID_SHLRX:
	case BGBCC_SH_NMID_ROTLX:
		rs=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rs);
		rn=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rn);
		if((rs<0) || (rn<0))
			return(0);
		break;
	}

	ex=0;
	if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
	if(BGBCC_JX2_EmitCheckRegExt4(ctx, rs)) ex|=0x0020;
	if(BGBCC_JX2_EmitCheckRegExt4(ctx, rt)) ex|=0x0010;

	if(rs==rn)
	{
		if(
			(nmid==BGBCC_SH_NMID_ADD) ||
//			(nmid==BGBCC_SH_NMID_ADDSL) ||
			(nmid==BGBCC_SH_NMID_SUB) ||
			(nmid==BGBCC_SH_NMID_AND) ||
			(nmid==BGBCC_SH_NMID_OR) ||
			(nmid==BGBCC_SH_NMID_XOR)
		)
		{
			return(BGBCC_JX2_TryEmitOpRegReg(ctx, nmid, rt, rn));
		}
	}

	if(rt==rn)
	{
		if(
			(nmid==BGBCC_SH_NMID_ADD) ||
//			(nmid==BGBCC_SH_NMID_ADDSL) ||
//			(nmid==BGBCC_SH_NMID_SUB) ||
			(nmid==BGBCC_SH_NMID_AND) ||
			(nmid==BGBCC_SH_NMID_OR) ||
			(nmid==BGBCC_SH_NMID_XOR)
		)
		{
			return(BGBCC_JX2_TryEmitOpRegReg(ctx, nmid, rs, rn));
		}
	}


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

	if(BGBCC_JX2_CheckPadCross32(ctx))
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_ADD:
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rs))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rt))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x1800|(rt&15);
			opw2=0x5800|((rn&15)<<4)|((rs&15)<<0);			break;
			break;
		case BGBCC_SH_NMID_SUB:
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rs))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rt))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x1800|(rt&15);
			opw2=0x5900|((rn&15)<<4)|((rs&15)<<0);			break;
			break;
		case BGBCC_SH_NMID_AND:
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rs))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rt))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x1800|(rt&15);
//			opw2=0x5A00|((rn&15)<<4)|((rs&15)<<0);			break;
			opw2=0x5D00|((rn&15)<<4)|((rs&15)<<0);			break;
			break;
		case BGBCC_SH_NMID_OR:
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rs))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rt))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x1800|(rt&15);
//			opw2=0x5B00|((rn&15)<<4)|((rs&15)<<0);			break;
			opw2=0x5E00|((rn&15)<<4)|((rs&15)<<0);			break;
			break;
		case BGBCC_SH_NMID_XOR:
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rs))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rt))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x1800|(rt&15);
//			opw2=0x5C00|((rn&15)<<4)|((rs&15)<<0);			break;
			opw2=0x5F00|((rn&15)<<4)|((rs&15)<<0);			break;
			break;
		default:
			break;
		}
	}

	if(ctx->is_fixed32 || ctx->op_is_wex2)
	{
		if((opw1>=0) && ((opw1&0xF000)!=0xF000))
		{
			opw1=-1;
		}
	}

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

		case BGBCC_SH_NMID_ADDX:
			opw1=0xF080|ex|(rt&15);
			opw2=0x1000|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_SUBX:
			opw1=0xF080|ex|(rt&15);
			opw2=0x1100|((rn&15)<<4)|((rs&15)<<0);
			break;

		case BGBCC_SH_NMID_MULL:
		case BGBCC_SH_NMID_DMULS:
			opw1=0xF000|ex|(rt&15);
			opw2=0x1200|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_DMULU:
//			opw1=0xF080|ex|(rt&15);
//			opw2=0x1200|((rn&15)<<4)|((rs&15)<<0);
			opw1=0xF000|ex|(rt&15);
			opw2=0x1300|((rn&15)<<4)|((rs&15)<<0);
			break;

		case BGBCC_SH_NMID_MULSL:
			opw1=0xF000|ex|(rt&15);
			opw2=0x5200|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_MULUL:
			opw1=0xF000|ex|(rt&15);
			opw2=0x5300|((rn&15)<<4)|((rs&15)<<0);
			break;

		case BGBCC_SH_NMID_MULSW:
			opw1=0xF000|ex|(rt&15);
			opw2=0x5E00|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_MULUW:
			opw1=0xF000|ex|(rt&15);
			opw2=0x5F00|((rn&15)<<4)|((rs&15)<<0);
			break;

		case BGBCC_SH_NMID_PMULSW:
			opw1=0xF000|ex|(rt&15);
			opw2=0x5100|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_PMULUW:
			opw1=0xF080|ex|(rt&15);
			opw2=0x5100|((rn&15)<<4)|((rs&15)<<0);
			break;

		case BGBCC_SH_NMID_PMULSLW:
			opw1=0xF000|ex|(rt&15);
			opw2=0x5600|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_PMULULW:
			opw1=0xF080|ex|(rt&15);
			opw2=0x5600|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_PMULSHW:
			opw1=0xF000|ex|(rt&15);
			opw2=0x5700|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_PMULUHW:
			opw1=0xF080|ex|(rt&15);
			opw2=0x5700|((rn&15)<<4)|((rs&15)<<0);
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

		case BGBCC_SH_NMID_ANDX:
			opw1=0xF080|ex|(rt&15);
			opw2=0x1500|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_ORX:
			opw1=0xF080|ex|(rt&15);
			opw2=0x1600|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_XORX:
			opw1=0xF080|ex|(rt&15);
			opw2=0x1700|((rn&15)<<4)|((rs&15)<<0);
			break;

		case BGBCC_SH_NMID_ADDSL:
			opw1=0xF000|ex|(rt&15);
			opw2=0x5C00|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_ADDUL:
			opw1=0xF080|ex|(rt&15);
			opw2=0x5C00|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_SUBSL:
			opw1=0xF000|ex|(rt&15);
			opw2=0x5D00|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_SUBUL:
			opw1=0xF080|ex|(rt&15);
			opw2=0x5D00|((rn&15)<<4)|((rs&15)<<0);
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

#if 0
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
#endif

#if 1
		case BGBCC_SH_NMID_SHAD:
			opw1=0xF000|ex|(rt&15);
			opw2=0x5400|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_SHLD:
			opw1=0xF000|ex|(rt&15);
			opw2=0x5500|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_SHADQ:
			opw1=0xF080|ex|(rt&15);
			opw2=0x5400|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_SHLDQ:
			opw1=0xF080|ex|(rt&15);
			opw2=0x5500|((rn&15)<<4)|((rs&15)<<0);
			break;
#endif

#if 1
		case BGBCC_SH_NMID_SHAR:
			opw1=0xF000|ex|(rt&15);
			opw2=0x2200|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_SHLR:
			opw1=0xF000|ex|(rt&15);
			opw2=0x2300|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_SHARQ:
			opw1=0xF080|ex|(rt&15);
			opw2=0x2200|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_SHLRQ:
			opw1=0xF080|ex|(rt&15);
			opw2=0x2300|((rn&15)<<4)|((rs&15)<<0);
			break;
#endif

#if 0
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
#endif

#if 0
		case BGBCC_SH_NMID_SHAR:
			if(ctx->is_fixed32 || ctx->op_is_wex2)
			{
				opw1=0xF001|(((rt>>4)&1)<<5);
				opw2=0x1C00|((rt&15)<<0);
				opw3=0xF000|(ex&0x00E0);
				opw4=0x5400|((rn&15)<<4)|((rs&15)<<0);
				break;
			}
//			opw1=0x330C|((rt&15)<<4)|((rt>>4)&1);
			opw1=0x330C|((rt&15)<<4)|(((rt>>4)&1)<<11);
			opw2=0xF000|(ex&0x00E0);
			opw3=0x5400|((rn&15)<<4)|((rs&15)<<0);			break;
		case BGBCC_SH_NMID_SHLR:
			if(ctx->is_fixed32 || ctx->op_is_wex2)
			{
				opw1=0xF001|(((rt>>4)&1)<<5);
				opw2=0x1C00|((rt&15)<<0);
				opw3=0xF000|(ex&0x00E0);
				opw4=0x5500|((rn&15)<<4)|((rs&15)<<0);
				break;
			}
//			opw1=0x330C|((rt&15)<<4)|((rt>>4)&1);
			opw1=0x330C|((rt&15)<<4)|(((rt>>4)&1)<<11);
			opw2=0xF000|(ex&0x00E0);
			opw3=0x5500|((rn&15)<<4)|((rs&15)<<0);			break;
		case BGBCC_SH_NMID_SHARQ:
			if(ctx->is_fixed32 || ctx->op_is_wex2)
			{
				opw1=0xF001|(((rt>>4)&1)<<5);
				opw2=0x1C00|((rt&15)<<0);
				opw3=0xF080|(ex&0x00E0);
				opw4=0x5400|((rn&15)<<4)|((rs&15)<<0);
				break;
			}
//			opw1=0x330C|((rt&15)<<4)|((rt>>4)&1);
			opw1=0x330C|((rt&15)<<4)|(((rt>>4)&1)<<11);
			opw2=0xF080|(ex&0x00E0);
			opw3=0x5400|((rn&15)<<4)|((rs&15)<<0);			break;
		case BGBCC_SH_NMID_SHLRQ:
			if(ctx->is_fixed32 || ctx->op_is_wex2)
			{
				opw1=0xF001|(((rt>>4)&1)<<5);
				opw2=0x1C00|((rt&15)<<0);
				opw3=0xF080|(ex&0x00E0);
				opw4=0x5500|((rn&15)<<4)|((rs&15)<<0);
				break;
			}
//			opw1=0x330C|((rt&15)<<4)|((rt>>4)&1);
			opw1=0x330C|((rt&15)<<4)|(((rt>>4)&1)<<11);
			opw2=0xF080|(ex&0x00E0);
			opw3=0x5500|((rn&15)<<4)|((rs&15)<<0);			break;
#endif

#if 1
		case BGBCC_SH_NMID_SHADX:
			opw1=0xF080|ex|(rt&15);
			opw2=0x3400|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_SHLDX:
			opw1=0xF080|ex|(rt&15);
			opw2=0x3500|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_SHARX:
			opw1=0xF080|ex|(rt&15);
			opw2=0x3200|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_SHLRX:
			opw1=0xF080|ex|(rt&15);
			opw2=0x3300|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_ROTLX:
			opw1=0xF080|ex|(rt&15);
			opw2=0x3600|((rn&15)<<4)|((rs&15)<<0);
			break;

		case BGBCC_SH_NMID_ROTL:
			opw1=0xF000|ex|(rt&15);
			opw2=0x3200|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_ROTR:
			opw1=0xF000|ex|(rt&15);
			opw2=0x3300|((rn&15)<<4)|((rs&15)<<0);
			break;
#endif

		case BGBCC_SH_NMID_CSELT:
			opw1=0xF000|ex|(rt&15);
			opw2=0x5000|((rn&15)<<4)|((rs&15)<<0);
			break;

		case BGBCC_SH_NMID_PCSELTW:
			opw1=0xF000|ex|(rt&15);
			opw2=0x2400|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_PCSELTL:
			opw1=0xF080|ex|(rt&15);
			opw2=0x2400|((rn&15)<<4)|((rs&15)<<0);
			break;

		case BGBCC_SH_NMID_FADD:
			opw1=0xF000|ex|(rt&15);
			opw2=0x5800|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_FSUB:
			opw1=0xF000|ex|(rt&15);
			opw2=0x5900|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_FMUL:
			opw1=0xF000|ex|(rt&15);
			opw2=0x5A00|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_FMAC:
			opw1=0xF000|ex|(rt&15);
			opw2=0x5B00|((rn&15)<<4)|((rs&15)<<0);
			break;

		case BGBCC_SH_NMID_FADDX:
			opw1=0xF080|ex|(rt&15);
			opw2=0x5800|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_FSUBX:
			opw1=0xF080|ex|(rt&15);
			opw2=0x5900|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_FMULX:
			opw1=0xF080|ex|(rt&15);
			opw2=0x5A00|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_FMACX:
			opw1=0xF080|ex|(rt&15);
			opw2=0x5B00|((rn&15)<<4)|((rs&15)<<0);
			break;

		case BGBCC_SH_NMID_PADDW:
			opw1=0xF000|ex|(rt&15);
			opw2=0x2000|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_PSUBW:
			opw1=0xF000|ex|(rt&15);
			opw2=0x2100|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_PADDL:
			opw1=0xF080|ex|(rt&15);
			opw2=0x2000|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_PSUBL:
			opw1=0xF080|ex|(rt&15);
			opw2=0x2100|((rn&15)<<4)|((rs&15)<<0);
			break;


		case BGBCC_SH_NMID_PADDF:
			opw1=0xF000|ex|(rt&15);
			opw2=0x2500|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_PSUBF:
			opw1=0xF000|ex|(rt&15);
			opw2=0x2600|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_PMULF:
			opw1=0xF000|ex|(rt&15);
			opw2=0x2700|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_PADDFX:
			opw1=0xF080|ex|(rt&15);
			opw2=0x2500|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_PSUBFX:
			opw1=0xF080|ex|(rt&15);
			opw2=0x2600|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_PMULFX:
			opw1=0xF080|ex|(rt&15);
			opw2=0x2700|((rn&15)<<4)|((rs&15)<<0);
			break;

		case BGBCC_SH_NMID_PADDH:
			opw1=0xF000|ex|(rt&15);
			opw2=0x2D00|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_PSUBH:
			opw1=0xF000|ex|(rt&15);
			opw2=0x2E00|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_PMULH:
			opw1=0xF000|ex|(rt&15);
			opw2=0x2F00|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_PADDXD:
			opw1=0xF080|ex|(rt&15);
			opw2=0x2D00|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_PSUBXD:
			opw1=0xF080|ex|(rt&15);
			opw2=0x2E00|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_PMULXD:
			opw1=0xF080|ex|(rt&15);
			opw2=0x2F00|((rn&15)<<4)|((rs&15)<<0);
			break;

		case BGBCC_SH_NMID_MOVHD:
			opw1=0xF000|ex|(rt&15);
			opw2=0x2800|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_MOVLD:
			opw1=0xF080|ex|(rt&15);
			opw2=0x2800|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_MOVHLD:
			opw1=0xF000|ex|(rt&15);
			opw2=0x2900|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_MOVLHD:
			opw1=0xF080|ex|(rt&15);
			opw2=0x2900|((rn&15)<<4)|((rs&15)<<0);
			break;

		case BGBCC_SH_NMID_NOP:
			opw1=0xF000|ex|(rt&15);
			opw2=0x5000|((rn&15)<<4)|((rs&15)<<0);
			break;

		case BGBCC_SH_NMID_BLKUTX1:
			opw1=0xF000|ex|(rt&15);
			opw2=0x2C00|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_BLKUTX2:
			opw1=0xF080|ex|(rt&15);
			opw2=0x2C00|((rn&15)<<4)|((rs&15)<<0);
//			opw2=0x6800|((rn&15)<<4)|((rs&15)<<0);
			break;

		case BGBCC_SH_NMID_BLERPW:
			opw1=0xF000|ex|(rt&15);
			opw2=0x6900|((rn&15)<<4)|((rs&15)<<0);
			break;

		case BGBCC_SH_NMID_BLKUTX3H:
			opw1=0xF080|ex|(rt&15);
			opw2=0x6800|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_BLKUTX3L:
			opw1=0xF080|ex|(rt&15);
			opw2=0x6900|((rn&15)<<4)|((rs&15)<<0);
			break;

		case BGBCC_SH_NMID_BLINTW:
			opw1=0xF080|ex|(rt&15);
			opw2=0x6A00|((rn&15)<<4)|((rs&15)<<0);
			break;
		case BGBCC_SH_NMID_BLINTAW:
			opw1=0xF000|ex|(rt&15);
			opw2=0x6A00|((rn&15)<<4)|((rs&15)<<0);
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

#if 0
	if(ctx->op_is_wex2&4)
	{
		if((opw1&0xFF00)==0xF000)
		{
			if(opw1&0x0010)
				{ opw1=-1; }
		}
	}
#endif

	if(opw1>=0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpRegRegReg(ctx, nmid, rs, rt, rn);

//		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
		BGBCC_JX2_EmitOpCheckRepack4(ctx, &opw1, &opw2, &opw3, &opw4);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		
		if((opw1&0xF000)!=0xF000)
		{
//			BGBCC_JX2_EmitOpCheckRepack(ctx, &opw2, &opw3);
			BGBCC_JX2_EmitPadForOpWord(ctx, opw2);
		}
		
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpImmReg(BGBCC_JX2_Context *ctx,
	int nmid, s64 imm, int reg)
{
	if(!BGBCC_JX2_TryEmitOpImmReg(ctx, nmid, imm, reg))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpImmReg(BGBCC_JX2_Context *ctx,
	int nmid, s64 imm, int reg)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpImmReg(ctx, nmid, imm, reg);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpImmReg(BGBCC_JX2_Context *ctx,
	int nmid, s64 imm, int reg)
{
	s32 imm1, imm1n;
	int opw1, opw2, opw3, opw4, opw5, opw6, odr, ex, ex2;
	int i, j, k;

	if(	(nmid==BGBCC_SH_NMID_ADDSL) ||
		(nmid==BGBCC_SH_NMID_ADDUL) )
	{
		return(BGBCC_JX2_TryEmitOpRegImmReg(ctx, nmid, reg, imm, reg));
	}

	if(	(nmid==BGBCC_SH_NMID_CMPEQ) ||
		(nmid==BGBCC_SH_NMID_CMPGT) ||
		(nmid==BGBCC_SH_NMID_CMPHI) ||
		(nmid==BGBCC_SH_NMID_CMPGE) ||
		(nmid==BGBCC_SH_NMID_CMPHS) ||
		(nmid==BGBCC_SH_NMID_TST) )
	{
		imm=(s32)imm;
	}

	if(nmid==BGBCC_SH_NMID_SHAR)
	{	return(BGBCC_JX2_TryEmitOpImmReg(ctx,
			BGBCC_SH_NMID_SHAD, -imm, reg));	}
	if(nmid==BGBCC_SH_NMID_SHLR)
	{	return(BGBCC_JX2_TryEmitOpImmReg(ctx,
			BGBCC_SH_NMID_SHLD, -imm, reg));	}
	if(nmid==BGBCC_SH_NMID_SHARQ)
	{	return(BGBCC_JX2_TryEmitOpImmReg(ctx,
			BGBCC_SH_NMID_SHADQ, -imm, reg));	}
	if(nmid==BGBCC_SH_NMID_SHLRQ)
	{	return(BGBCC_JX2_TryEmitOpImmReg(ctx,
			BGBCC_SH_NMID_SHLDQ, -imm, reg));	}

	imm1n=-imm;
	opw1=-1; opw2=-1; opw3=-1;
	opw4=-1; opw5=-1; opw6=-1;
	odr=0;

	switch(nmid)
	{
#if 1
	case BGBCC_SH_NMID_LDSH8:	
		if(((byte)imm)!=imm)
			break;
		if((reg!=BGBCC_SH_REG_R0) &&
			(reg!=BGBCC_SH_REG_RD0) &&
			(reg!=BGBCC_SH_REG_RQ0) &&
			(reg!=BGBCC_SH_REG_DLR))
				break;
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
//		opw1=0xD000|((reg&15)<<8)|(imm&255);			break;
		opw1=0x2600|(imm&255);			break;
		break;
#endif

#if 1
	case BGBCC_SH_NMID_LDSH16:	
		if(((u16)imm)!=imm)
			break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw1=0xF860|(reg&15);
		opw2=imm&65535;
		break;
#endif

#if 1
	case BGBCC_SH_NMID_ADD:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
#if 0
				if((imm&15)==imm)
				{
//					opw1=0x2900|((reg&15)<<4)|(imm&15);
//					opw1=0x2700|((reg&15)<<4)|(imm&15);
					opw1=0x6800|((reg&15)<<4)|(imm&15);
					break;
				}
#endif

#if 1
				if((((s32)(imm<<27))>>27)==imm)
				{
//					opw1=0xE000|((reg&15)<<8)|(imm&255);
//					opw1=0x7600|((reg&15)<<4)|(imm&0x010F);
					opw1=0x6800|((reg&15)<<4)|(imm&0x010F);
					break;
				}
#endif
			}
			break;
		}
		if(((sbyte)imm)==imm)
			{ opw1=0xC000|((reg&15)<<8)|(imm&255); break; }

//		break;

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((((s16)imm)==imm) || (((u16)imm)==imm))
			{
	//			opw1=0xA000|((imm>>4)&0x1FFF);
	//			opw2=0x4900|((reg&15)<<4)|(imm&15);
				break;
			}
		}

		if(ctx->has_jumbo && !ctx->op_is_wex2)
			break;

//		odr=1; opw1=0x3100|((reg&15)<<4);				break;
		odr=1; opw1=0x1000|((reg&15)<<4);				break;

	case BGBCC_SH_NMID_SUB:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
#if 1
			if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
				if((((s32)(imm1n<<27))>>27)==imm1n)
				{
	//				opw1=0xE000|((reg&15)<<8)|(imm&255);
//					opw1=0x7600|((reg&15)<<4)|(imm1n&0x010F);
					opw1=0x6800|((reg&15)<<4)|(imm1n&0x010F);
					break;
				}
			}
#endif
			break;
		}

		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(((sbyte)imm1n)==imm1n)
			{ opw1=0xC000|((reg&15)<<8)|(imm1n&255); break; }

//		break;

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((((s16)imm)==imm1n) || (((u16)imm1n)==imm1n))
			{
	//			opw1=0xA000|((imm1n>>4)&0x1FFF);
	//			opw2=0x4900|((reg&15)<<4)|(imm1n&15);
				break;
			}
		}

		if(ctx->has_jumbo && !ctx->op_is_wex2)
			break;

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

		if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			break;
		if(ctx->has_jumbo && !ctx->op_is_wex2)
			break;

		odr=1; opw1=0x1400|((reg&15)<<4);				break;
	case BGBCC_SH_NMID_AND:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		
		if(imm==255)
			{ opw1=0x3204|((reg&15)<<4);	break;	}
		if(imm==65535)
			{ opw1=0x3205|((reg&15)<<4);	break;	}

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((imm&511)==imm)
				break;
		}

		if(!BGBCC_JX2_CheckPadCross48(ctx) &&
			(ctx->optmode!=BGBCC_OPT_SIZE))
		{
			if((((s16)imm)==imm) || (((u16)imm)==imm))
				break;
		}

		if(ctx->has_jumbo && !ctx->op_is_wex2)
			break;
		
//		odr=1; opw1=0x3105|((reg&15)<<4);				break;
		odr=1; opw1=0x1500|((reg&15)<<4);				break;
	case BGBCC_SH_NMID_OR:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((imm&511)==imm)
				break;
		}
		
		if(!BGBCC_JX2_CheckPadCross48(ctx) &&
			(ctx->optmode!=BGBCC_OPT_SIZE))
		{
			if((((s16)imm)==imm) || (((u16)imm)==imm))
				break;
		}

		if(ctx->has_jumbo && !ctx->op_is_wex2)
			break;
		
//		odr=1; opw1=0x3106|((reg&15)<<4);				break;
		odr=1; opw1=0x1600|((reg&15)<<4);				break;
	case BGBCC_SH_NMID_XOR:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((imm&511)==imm)
				break;
		}
		
		if(!BGBCC_JX2_CheckPadCross48(ctx) &&
			(ctx->optmode!=BGBCC_OPT_SIZE))
		{
			if((((s16)imm)==imm) || (((u16)imm)==imm))
				break;
		}

		if(ctx->has_jumbo && !ctx->op_is_wex2)
			break;

//		odr=1; opw1=0x3107|((reg&15)<<4);				break;
		odr=1; opw1=0x1700|((reg&15)<<4);				break;

	case BGBCC_SH_NMID_MOV:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(((sbyte)imm)==imm)
//			{ opw1=0xE000|((reg&15)<<8)|(imm&255); break; }
			{ opw1=0xD000|((reg&15)<<8)|(imm&255); break; }
//		break;
		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((((s16)imm)==imm) || (((u16)imm)==imm))
			{
	//			opw1=0xA000|((imm>>4)&0x1FFF);
	//			opw2=0x4800|((reg&15)<<4)|(imm&15);
				break;
			}
		}

		if(ctx->has_jumbo && !ctx->op_is_wex2)
			break;

		odr=1; opw1=0x1800|((reg&15)<<4);				break;
//		odr=1; opw1=0x1B03|((reg&15)<<4);				break;
#endif

#if 1
	case BGBCC_SH_NMID_CMPEQ:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
				if(((imm&15)==imm) || ((imm|(~15))==imm))
					{ opw1=0x6C00|((reg&15)<<4)|(imm&0x010F); break; }
			}

			break;
		}
		if(((imm&15)==imm) || ((imm|(~15))==imm))
			{ opw1=0x2C00|((reg&15)<<4)|(imm&0x010F); break; }
//		if(((imm&255)==imm) || ((imm|(~255))==imm))
//			{ opw1=0xF000|(reg&15); opw2=0x2C00|(imm&0x01FF); break; }		

#if 0
		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			{	opw1=0xF000|(reg&31)|((imm>>3)&0x0060);
				opw2=0x2C00|(imm&0x00FF)|((imm>>10)&0x0100);
				break;		}
		}
#endif

		if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			break;

		if(!BGBCC_JX2_CheckPadCross48(ctx))
		{
			if((((s16)imm)==imm) || (((u16)imm)==imm))
				break;
		}

		if(ctx->has_jumbo && !ctx->op_is_wex2)
			break;

		odr=1;	opw1=0x1C00|((reg&15)<<4);				break;
	case BGBCC_SH_NMID_CMPHI:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;

		if(!BGBCC_JX2_CheckPadCross48(ctx))
		{
			if((((s16)imm)==imm) || (((u16)imm)==imm))
				break;
		}

		if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			break;

		if(ctx->has_jumbo && !ctx->op_is_wex2)
			break;

//		odr=1;		opw1=0x310D|((reg&15)<<4);			break;
		odr=1;	opw1=0x1D00|((reg&15)<<4);				break;
	case BGBCC_SH_NMID_CMPGT:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
				if((imm&15)==imm)
					{ opw1=0x6E00|((reg&15)<<4)|(imm&15); break; }
			}

			break;
		}

//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if((imm&15)==imm)
			{ opw1=0x2E00|((reg&15)<<4)|(imm&15); break; }
//		if((imm&255)==imm)
//			{ opw1=0xF000|(reg&15); opw2=0x2E00|(imm&0x00FF); break; }		

#if 0
		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((imm&1023)==imm)
			{	opw1=0xF000|(reg&31)|((imm>>3)&0x0060);
				opw2=0x2E00|(imm&0x00FF);
				break;		}
		}
#endif

		if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			break;

		if(!BGBCC_JX2_CheckPadCross48(ctx))
		{
			if((((s16)imm)==imm) || (((u16)imm)==imm))
				break;
		}

		if(ctx->has_jumbo && !ctx->op_is_wex2)
			break;

//		odr=1;		opw1=0x310E|((reg&15)<<4);			break;
		odr=1;	opw1=0x1E00|((reg&15)<<4);				break;

	case BGBCC_SH_NMID_CMPHS:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		odr=1;		opw1=0x360D|((reg&15)<<4);			break;
	case BGBCC_SH_NMID_CMPGE:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
				if((imm&15)==imm)
					{ opw1=0x6F00|((reg&15)<<4)|(imm&15); break; }
			}

			break;
		}
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
		if((imm&15)==imm)
			{ opw1=0x2F00|((reg&15)<<4)|(imm&15); break; }
//		if((imm&255)==imm)
//			{ opw1=0xF000|(reg&15); opw2=0x2F00|(imm&0x00FF); break; }		

#if 0
		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((imm&1023)==imm)
			{	opw1=0xF000|(reg&31)|((imm>>3)&0x0060);
				opw2=0x2F00|(imm&0x00FF);
				break;		}
		}
#endif

		if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			break;

//		odr=1;		opw1=0x360E|((reg&15)<<4);			break;
		odr=1;		opw1=0x1F00|((reg&15)<<4);			break;
#endif

#if 1
	case BGBCC_SH_NMID_CMPQEQ:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
//			{
//				if(((imm&15)==imm) || ((imm|(~15))==imm))
//					{ opw1=0x6C00|((reg&15)<<4)|(imm&0x010F); break; }
//			}

			break;
		}
//		if(((imm&15)==imm) || ((imm|(~15))==imm))
//			{ opw1=0x2C00|((reg&15)<<4)|(imm&0x010F); break; }

		if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			break;

		if(!BGBCC_JX2_CheckPadCross48(ctx))
		{
			if((((s16)imm)==imm) || (((u16)imm)==imm))
				break;
		}

		if(ctx->has_jumbo && !ctx->op_is_wex2)
			break;

		odr=1;	opw1=0x5500|((reg&15)<<4);
		break;
#endif

#if 0

	case BGBCC_SH_NMID_SHAD:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
#if 0
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

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			break;
		}

		odr=1; opw1=0x3606|((reg&15)<<4);				break;
	case BGBCC_SH_NMID_SHLD:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
#if 0
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

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			break;
		}

		odr=1; opw1=0x3607|((reg&15)<<4);				break;


	case BGBCC_SH_NMID_SHADQ:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
#if 0
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
//		odr=1; opw1=0x3806|((reg&15)<<4);				break;
		odr=1; opw1=0x3106|((reg&15)<<4);				break;
	case BGBCC_SH_NMID_SHLDQ:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))	break;
#if 0
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
//		odr=1; opw1=0x3807|((reg&15)<<4);				break;
		odr=1; opw1=0x3107|((reg&15)<<4);				break;

#endif
	}

	if(ctx->is_fixed32 || ctx->op_is_wex2)
	{
		if((opw1>=0) && ((opw1&0xF000)!=0xF000))
		{
			opw1=-1; odr=0;
		}
	}

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, reg)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, reg)) ex|=0x0020;
		ex2=ex<<4;

		switch(nmid)
		{
		case BGBCC_SH_NMID_ADD:

#if 1
			if(((s16)imm)==imm)
			{
				opw1=0xF840|(reg&31);
				opw2=(u16)imm;
				break;
			}

#if 0
			if(!BGBCC_JX2_CheckPadCross48(ctx) &&
				!ctx->is_fixed32 && !ctx->op_is_wex2)
			{
				if(((s32)imm)==imm)
				{
					opw1=0xFCC0|(reg&15)|((reg&0x10)<<4);
					opw2=(u16)(imm    );
					opw3=(u16)(imm>>16);
					break;
				}
			}
#endif

#endif

#if 1
			if(ctx->has_jumbo && !ctx->op_is_wex2)
			{
				i=BGBCC_JX2_ComposeJumboImmRegF2B(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF200, 0xD000, imm, reg);
				if(i>0)break;
			}
#endif

			odr=1;
			opw1=0xF000|(ex&(~0x0020));
			opw2=0x1900|((reg&15)<<4);
			break;

			break;
		case BGBCC_SH_NMID_SUB:
#if 1
			if(((s16)imm1n)==imm1n)
			{
				opw1=0xF840|(reg&31);
				opw2=(u16)imm1n;
				break;
			}

			if(!BGBCC_JX2_CheckPadCross48(ctx) &&
				!ctx->is_fixed32 && !ctx->op_is_wex2)
			{
				if(((s32)imm1n)==imm1n)
				{
					opw1=0xFCC0|(reg&15)|((reg&0x10)<<4);
					opw2=(u16)(imm1n    );
					opw3=(u16)(imm1n>>16);
					break;
				}
			}
#endif

#if 1
			if(ctx->has_jumbo && !ctx->op_is_wex2)
			{
				i=BGBCC_JX2_ComposeJumboImmRegF2B(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF200, 0xD000, imm1n, reg);
				if(i>0)break;
			}
#endif

			odr=1;
			opw1=0xF001|(ex&(~0x0020));
			opw2=0x1900|((reg&15)<<4);
			break;

		case BGBCC_SH_NMID_MOV:
#if 1
//			if(((s16)imm)==imm)
			if((((s16)imm)==imm) || (((u16)imm)==imm))
			{
				opw1=0xF800|(reg&31)|((imm>>16)&0x0020);
				opw2=(u16)imm;
				break;
			}

			if(!BGBCC_JX2_CheckPadCross48(ctx) &&
				!ctx->is_fixed32 && !ctx->op_is_wex2)
			{
				if(((s32)imm)==imm)
				{
					opw1=0xFCE0|(reg&15)|((reg&0x10)<<4);
					opw2=(u16)(imm    );
					opw3=(u16)(imm>>16);
					break;
				}
			}
#endif

			if(ctx->is_fixed32 || ctx->op_is_wex2)
			{
				odr=1;
				opw1=0xF008|(ex&(~0x0020));
				opw2=0x1900|((reg&15)<<4);
				break;
			}

			odr=1;
//			opw1=0xF008|(ex&(~0x0020));
//			opw2=0x1900|((reg&15)<<4);
			opw1=0x1800|((reg&15)<<4)|((reg&16)<<5);
			break;
		
		case BGBCC_SH_NMID_FLDCH:
			opw1=0xF880|(reg&31);
			opw2=(u16)imm;
			break;

		case BGBCC_SH_NMID_SHAD:
#if 0
			if((imm&31)==imm)
			{	opw1=0xF000|ex|(imm&31);
				opw2=0x5600|((reg&15)<<4)|((reg&15)<<0);
				break;			}
			if((imm|(~31))==imm)
			{	opw1=0xF080|ex|(imm&31);
				opw2=0x5600|((reg&15)<<4)|((reg&15)<<0);
				break;			}
#endif

			if(1)
			{
				opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
//				opw2=0x8000|ex2|(imm&511);
				opw2=0x8000|ex2|(imm&255);
				break;
			}

//			opw1=0xF006|ex; odr=1;
//			opw2=0x1D00|((reg&15)<<4)|((reg&15)<<0);		break;
			opw1=0xF000|ex; odr=1;
			opw2=0x5400|((reg&15)<<4)|((reg&15)<<0);		break;
		case BGBCC_SH_NMID_SHLD:
#if 0
			if((imm&31)==imm)
			{	opw1=0xF000|ex|(imm&31);
				opw2=0x5700|((reg&15)<<4)|((reg&15)<<0);
				break;			}
			if((imm|(~31))==imm)
			{	opw1=0xF080|ex|(imm&31);
				opw2=0x5700|((reg&15)<<4)|((reg&15)<<0);
				break;			}
#endif

			if(1)
			{
				opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
//				opw2=0x9000|ex2|(imm&511);
				opw2=0x9000|ex2|(imm&255);
				break;
			}

//			opw1=0xF007|ex; odr=1;
//			opw2=0x1D00|((reg&15)<<4)|((reg&15)<<0);		break;
			opw1=0xF000|ex; odr=1;
			opw2=0x5500|((reg&15)<<4)|((reg&15)<<0);		break;

		case BGBCC_SH_NMID_SHADQ:
//			opw1=0xF086|ex; odr=1;
//			opw2=0x1D00|((reg&15)<<4)|((reg&15)<<0);		break;
			if(1)
			{
				opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
//				opw2=0x8800|ex2|(imm&511);
				opw2=0x8800|ex2|(imm&255);
				break;
			}

			opw1=0xF080|ex; odr=1;
			opw2=0x5400|((reg&15)<<4)|((reg&15)<<0);		break;
		case BGBCC_SH_NMID_SHLDQ:
//			opw1=0xF087|ex; odr=1;
//			opw2=0x1D00|((reg&15)<<4)|((reg&15)<<0);		break;

			if(1)
			{
				opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
//				opw2=0x9800|ex2|(imm&511);
				opw2=0x9800|ex2|(imm&255);
				break;
			}

			opw1=0xF080|ex; odr=1;
			opw2=0x5500|((reg&15)<<4)|((reg&15)<<0);		break;

		case BGBCC_SH_NMID_CMPEQ:
#if 0
			if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			{	opw1=0xF000|(reg&31)|((imm>>3)&0x0060);
				opw2=0x2C00|(imm&0x00FF)|((imm>>10)&0x0100);
				break;		}
#endif

#if 1
			if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			{
				opw1=0xF20C|((reg&15)<<4)|((imm>>31)&1);
				opw2=0xC000|(imm&0x03FF)|(ex2&0x0C00);
				break;
			}
#endif

#if 0
			if(!BGBCC_JX2_CheckPadCross48(ctx) &&
				!ctx->is_fixed32 && !ctx->op_is_wex2)
			{
				if((((s16)imm)==imm) || (((u16)imm)==imm))
//				if(((s16)imm)==imm)
				{
					opw1=0xFC00|((ex>>4)&4)|((imm>>16)&1);
					opw2=0x190C|((reg&15)<<4);
					opw3=(u16)(imm);
					break;
				}
			}
#endif

#if 1
			if(ctx->has_jumbo && !ctx->op_is_wex2)
			{
				i=BGBCC_JX2_ComposeJumboImmRegF2B(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF20C, 0xC000, imm, reg);
				if(i>0)break;
			}
#endif

			opw1=0xF00C|(ex&0x0040); odr=1;
			opw2=0x1900|((reg&15)<<4);
			break;
		case BGBCC_SH_NMID_CMPHI:
#if 0
			if(!BGBCC_JX2_CheckPadCross48(ctx) &&
				!ctx->is_fixed32 && !ctx->op_is_wex2)
			{
				if((((s16)imm)==imm) || (((u16)imm)==imm))
				{
					opw1=0xFC00|((reg&0x10)>>2)|((imm>>16)&1);
					opw2=0x190D|((reg&15)<<4);
					opw3=(u16)(imm);
					break;
				}
			}
#endif

#if 1
			if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			{
				opw1=0xF208|((reg&15)<<4)|((imm>>31)&1);
				opw2=0xC000|(imm&0x03FF)|(ex2&0x0C00);
				break;
			}
#endif

#if 1
			if(ctx->has_jumbo && !ctx->op_is_wex2)
			{
				i=BGBCC_JX2_ComposeJumboImmRegF2B(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF208, 0xC000, imm, reg);
				if(i>0)break;
			}
#endif

			opw1=0xF00D|(ex&0x0040); odr=1;
			opw2=0x1900|((reg&15)<<4);
			break;
		case BGBCC_SH_NMID_CMPGT:
#if 0
			if((imm&1023)==imm)
			{	opw1=0xF000|(reg&31)|((imm>>3)&0x0060);
				opw2=0x2E00|(imm&0x00FF);
				break;		}
#endif		

#if 1
			if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			{
				opw1=0xF20E|((reg&15)<<4)|((imm>>31)&1);
				opw2=0xC000|(imm&0x03FF)|(ex2&0x0C00);
				break;
			}
#endif

#if 0
			if(!BGBCC_JX2_CheckPadCross48(ctx) &&
				!ctx->is_fixed32 && !ctx->op_is_wex2)
			{
				if((((s16)imm)==imm) || (((u16)imm)==imm))
				{
					opw1=0xFC00|((reg&0x10)>>2)|((imm>>16)&1);
					opw2=0x190E|((reg&15)<<4);
					opw3=(u16)(imm);
					break;
				}
			}
#endif

#if 1
			if(ctx->has_jumbo && !ctx->op_is_wex2)
			{
				i=BGBCC_JX2_ComposeJumboImmRegF2B(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF20E, 0xC000, imm, reg);
				if(i>0)break;
			}
#endif

			opw1=0xF00E|(ex&0x0040); odr=1;
			opw2=0x1900|((reg&15)<<4);
			break;

#if 1
		case BGBCC_SH_NMID_CMPQEQ:	
#if 0
			if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			{	opw1=0xF080|(reg&31)|((imm>>3)&0x0060);
				opw2=0x2C00|(imm&0x00FF)|((imm>>10)&0x0100);
				break;		}
#endif

#if 1
			if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			{
				opw1=0xF20C|((reg&15)<<4)|((imm>>31)&1);
				opw2=0xC800|(imm&0x03FF)|(ex2&0x0C00);
				break;
			}
#endif

#if 0
			if(!BGBCC_JX2_CheckPadCross48(ctx) &&
				!ctx->is_fixed32 && !ctx->op_is_wex2)
			{
				if((((s16)imm)==imm) || (((u16)imm)==imm))
				{
					opw1=0xFC08|((reg&0x10)>>2)|((imm>>16)&1);
					opw2=0x190C|((reg&15)<<4);
					opw3=(u16)(imm);
					break;
				}
			}
#endif

#if 1
			if(ctx->has_jumbo && !ctx->op_is_wex2)
			{
				i=BGBCC_JX2_ComposeJumboImmRegF2B(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF20C, 0xC800, imm, reg);
				if(i>0)break;
			}
#endif

			opw1=0xF08C|(ex&0x0040); odr=1;
			opw2=0x1900|((reg&15)<<4);
			break;
		case BGBCC_SH_NMID_CMPQHI:	
#if 1
			if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			{
				opw1=0xF208|((reg&15)<<4)|((imm>>31)&1);
				opw2=0xC800|(imm&0x03FF)|(ex2&0x0C00);
				break;
			}
#endif

#if 0
			if(!BGBCC_JX2_CheckPadCross48(ctx) &&
				!ctx->is_fixed32 && !ctx->op_is_wex2)
			{
				if((((s16)imm)==imm) || (((u16)imm)==imm))
				{
					opw1=0xFC08|((reg&0x10)>>2)|((imm>>16)&1);
					opw2=0x190D|((reg&15)<<4);
					opw3=(u16)(imm);
					break;
				}
			}
#endif

#if 1
			if(ctx->has_jumbo && !ctx->op_is_wex2)
			{
				i=BGBCC_JX2_ComposeJumboImmRegF2B(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF208, 0xC800, imm, reg);
				if(i>0)break;
			}
#endif

			opw1=0xF08D|(ex&0x0040); odr=1;
			opw2=0x1900|((reg&15)<<4);
			break;
		case BGBCC_SH_NMID_CMPQGT:
#if 0
			if((imm&1023)==imm)
			{	opw1=0xF080|(reg&31)|((imm>>3)&0x0060);
				opw2=0x2E00|(imm&0x00FF);
				break;		}
#endif	

#if 1
			if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			{
				opw1=0xF20E|((reg&15)<<4)|((imm>>31)&1);
				opw2=0xC800|(imm&0x03FF)|(ex2&0x0C00);
				break;
			}
#endif

#if 0
			if(!BGBCC_JX2_CheckPadCross48(ctx) &&
				!ctx->is_fixed32 && !ctx->op_is_wex2)
			{
				if((((s16)imm)==imm) || (((u16)imm)==imm))
				{
					opw1=0xFC08|((reg&0x10)>>2)|((imm>>16)&1);
					opw2=0x190E|((reg&15)<<4);
					opw3=(u16)(imm);
					break;
				}
			}
#endif

#if 1
			if(ctx->has_jumbo && !ctx->op_is_wex2)
			{
				i=BGBCC_JX2_ComposeJumboImmRegF2B(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF20E, 0xC800, imm, reg);
				if(i>0)break;
			}
#endif

			opw1=0xF08E|(ex&0x0040); odr=1;
			opw2=0x1900|((reg&15)<<4);
			break;
#endif

#if 1
		case BGBCC_SH_NMID_CMPHS:
			if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			{
				opw1=0xF206|((reg&15)<<4)|((imm>>31)&1);
				opw2=0xC000|(imm&0x03FF)|(ex2&0x0C00);
				break;
			}
//			opw1=0xF08E|(ex&0x0040); odr=1;
//			opw2=0x1900|((reg&15)<<4);
			break;

		case BGBCC_SH_NMID_CMPGE:
			if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			{
				opw1=0xF20A|((reg&15)<<4)|((imm>>31)&1);
				opw2=0xC000|(imm&0x03FF)|(ex2&0x0C00);
				break;
			}
//			opw1=0xF08E|(ex&0x0040); odr=1;
//			opw2=0x1900|((reg&15)<<4);
			break;

		case BGBCC_SH_NMID_CMPQHS:
			if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			{
				opw1=0xF206|((reg&15)<<4)|((imm>>31)&1);
				opw2=0xC800|(imm&0x03FF)|(ex2&0x0C00);
				break;
			}
//			opw1=0xF08E|(ex&0x0040); odr=1;
//			opw2=0x1900|((reg&15)<<4);
			break;

		case BGBCC_SH_NMID_CMPQGE:
			if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			{
				opw1=0xF20A|((reg&15)<<4)|((imm>>31)&1);
				opw2=0xC800|(imm&0x03FF)|(ex2&0x0C00);
				break;
			}
//			opw1=0xF08E|(ex&0x0040); odr=1;
//			opw2=0x1900|((reg&15)<<4);
			break;
#endif

		case BGBCC_SH_NMID_ADDC:	
			opw1=0xF002|(ex&0x0040); odr=1;
			opw2=0x1900|((reg&15)<<4);
			break;
		case BGBCC_SH_NMID_SUBC:	
			opw1=0xF003|(ex&0x0040); odr=1;
			opw2=0x1900|((reg&15)<<4);
			break;

		case BGBCC_SH_NMID_TST:	
			if(!BGBCC_JX2_CheckPadCross48(ctx) &&
				!ctx->is_fixed32 && !ctx->op_is_wex2)
			{
				if((((s16)imm)==imm) || (((u16)imm)==imm))
				{
					opw1=0xFC00|((reg&0x10)>>2)|((imm>>16)&1);
					opw2=0x1904|((reg&15)<<4);
					opw3=(u16)(imm);
					break;
				}
			}

			if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			{
				opw1=0xF204|((reg&15)<<4)|((imm>>31)&1);
				opw2=0xC000|(imm&0x03FF)|(ex2&0x0C00);
				break;
			}

#if 1
			if(ctx->has_jumbo && !ctx->op_is_wex2)
			{
				i=BGBCC_JX2_ComposeJumboImmRegF2B(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF204, 0xC000, imm, reg);
				if(i>0)break;
			}
#endif

			opw1=0xF004|(ex&0x0040); odr=1;
			opw2=0x1900|((reg&15)<<4);
			break;
		case BGBCC_SH_NMID_TSTQ:	
			if(!BGBCC_JX2_CheckPadCross48(ctx) &&
				!ctx->is_fixed32 && !ctx->op_is_wex2)
			{
				if((((s16)imm)==imm) || (((u16)imm)==imm))
				{
					opw1=0xFC08|((reg&0x10)>>2)|((imm>>16)&1);
					opw2=0x1904|((reg&15)<<4);
					opw3=(u16)(imm);
					break;
				}
			}

			if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			{
				opw1=0xF204|((reg&15)<<4)|((imm>>31)&1);
				opw2=0xC800|(imm&0x03FF)|(ex2&0x0C00);
				break;
			}

#if 1
			if(ctx->has_jumbo && !ctx->op_is_wex2)
			{
				i=BGBCC_JX2_ComposeJumboImmRegF2B(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF204, 0xC800, imm, reg);
				if(i>0)break;
			}
#endif

			opw1=0xF084|(ex&0x0040); odr=1;
			opw2=0x1900|((reg&15)<<4);
			break;

		case BGBCC_SH_NMID_AND:
			if((imm&511)==imm)
			{
				opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
				opw2=0x5000|ex2|(imm&511);
				break;
			}
			if(!BGBCC_JX2_CheckPadCross48(ctx) &&
				!ctx->is_fixed32 && !ctx->op_is_wex2)
			{
				if((imm&65535)==imm)
				{
					opw1=0xFC08|(ex>>4);
					opw2=0x1500|((reg&15)<<4)|((reg&15)<<0);
					opw3=(u16)imm;
					break;
				}
			}
			opw1=0xF005|(ex&0x0040); odr=1;
			opw2=0x1900|((reg&15)<<4);
			break;
		case BGBCC_SH_NMID_OR:	
			if((imm&511)==imm)
			{
				opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
				opw2=0x6000|ex2|(imm&511);
				break;
			}
			if(!BGBCC_JX2_CheckPadCross48(ctx) &&
				!ctx->is_fixed32 && !ctx->op_is_wex2)
			{
				if((imm&65535)==imm)
				{
					opw1=0xFC08|(ex>>4);
					opw2=0x1600|((reg&15)<<4)|((reg&15)<<0);
					opw3=(u16)imm;
					break;
				}
			}
			opw1=0xF006|(ex&0x0040); odr=1;
			opw2=0x1900|((reg&15)<<4);
			break;
		case BGBCC_SH_NMID_XOR:	
			if((imm&511)==imm)
			{
				opw1=0xF200|((reg&15)<<4)|((reg&15)<<0);
				opw2=0x7000|ex2|(imm&511);
				break;
			}
			if(!BGBCC_JX2_CheckPadCross48(ctx) &&
				!ctx->is_fixed32 && !ctx->op_is_wex2)
			{
				if((imm&65535)==imm)
				{
					opw1=0xFC08|(ex>>4);
					opw2=0x1700|((reg&15)<<4)|((reg&15)<<0);
					opw3=(u16)imm;
					break;
				}
			}
			opw1=0xF007|(ex&0x0040); odr=1;
			opw2=0x1900|((reg&15)<<4);
			break;

		case BGBCC_SH_NMID_JLDI:
#if 0
//			opw1=0xF400|((imm>>56)&  255);
//			opw2=0xC000|((imm>>44)& 4095);
//			opw3=0xF400|((imm>>36)&  255);
//			opw4=0xC000|((imm>>24)& 4095);
//			opw5=0xFA00|((imm>>16)&  255);
//			opw6=0x0000|((imm>> 0)&65535);
//			opw2|=((reg>>2)&3)<<12;
//			opw4|=((reg>>0)&3)<<12;
//			opw5|=((reg>>4)&1)<< 8;
#endif

#if 1
			opw1=0xFE00|((imm>>56)&  255);
			opw2=0x0000|((imm>>40)&65535);
			opw3=0xFE00|((imm>>32)&  255);
			opw4=0x0000|((imm>>16)&65535);
			opw5=0xF800|(reg&31);
			opw6=0x0000|((imm>> 0)&65535);
#endif

			break;
		}
	}
	

	if(opw1>=0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpImmReg(ctx, nmid, imm, reg);

		if(odr)
			BGBCC_JX2_EmitLoadDrImm(ctx, imm);

//		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
//		BGBCC_JX2_EmitOpCheckRepack3(ctx, &opw1, &opw2, &opw3);
		BGBCC_JX2_EmitOpCheckRepack6(ctx,
			&opw1, &opw2, &opw3,
			&opw4, &opw5, &opw6);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);

		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
		if(opw5>=0)
			BGBCC_JX2_EmitWord(ctx, opw5);
		if(opw6>=0)
			BGBCC_JX2_EmitWord(ctx, opw6);

		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpRegImmReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, s64 imm, int rn)
{
	if(!BGBCC_JX2_TryEmitOpRegImmReg(ctx, nmid, rm, imm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpRegImmReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, s64 imm, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpRegImmReg(ctx, nmid, rm, imm, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpRegImmReg(
	BGBCC_JX2_Context *ctx, int nmid, int rm, s64 imm, int rn)
{
	int opw1, opw2, opw3, opw4, ex, ex2;
	int rm1, rn1;
	s64 imm1;
	int odr, imm_is12s;
	int i;

	if(nmid==BGBCC_SH_NMID_AND)
	{
		imm1=-1;

		if(imm==255)
			{ imm1=BGBCC_SH_NMID_EXTUB; }
		if(imm==65535)
			{ imm1=BGBCC_SH_NMID_EXTUW; }
		if(imm==((1LL<<32)-1))
			{ imm1=BGBCC_SH_NMID_EXTUL; }
		if(imm1>0)
			return(BGBCC_JX2_TryEmitOpRegReg(ctx, imm1, rm, rn));
	}

	if(nmid==BGBCC_SH_NMID_PSHUFL)
	{
		rm1=BGBCC_SH_REG_RQ0+(rm&31);
		rn1=BGBCC_SH_REG_RQ0+(rn&31);
		return(BGBCC_JX2_EmitShufDWordRegReg(ctx, rm1, imm, rn1));
	}

	if(nmid==BGBCC_SH_NMID_PSHUFXL)
	{
		rm1=BGBCC_SH_REG_LR0+(rm&31);
		rn1=BGBCC_SH_REG_LR0+(rn&31);
		return(BGBCC_JX2_EmitShufDWordRegReg(ctx, rm1, imm, rn1));
	}

	if(nmid==BGBCC_SH_NMID_SHAR)
	{	return(BGBCC_JX2_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHAD, rm, -imm, rn));		}
	if(nmid==BGBCC_SH_NMID_SHLR)
	{	return(BGBCC_JX2_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHLD, rm, -imm, rn));		}
	if(nmid==BGBCC_SH_NMID_SHARQ)
	{	return(BGBCC_JX2_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHADQ, rm, -imm, rn));	}
	if(nmid==BGBCC_SH_NMID_SHLRQ)
	{	return(BGBCC_JX2_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHLDQ, rm, -imm, rn));	}
	

#if 1
	if(rm==rn)
	{
		if(
			(nmid==BGBCC_SH_NMID_ADD) ||
			(nmid==BGBCC_SH_NMID_SUB) ||
			(nmid==BGBCC_SH_NMID_AND) ||
			(nmid==BGBCC_SH_NMID_OR) ||
			(nmid==BGBCC_SH_NMID_XOR) ||
			(nmid==BGBCC_SH_NMID_SHLD) ||
			(nmid==BGBCC_SH_NMID_SHAD) ||
			(nmid==BGBCC_SH_NMID_SHLDQ) ||
			(nmid==BGBCC_SH_NMID_SHADQ)
		)
		{
			return(BGBCC_JX2_TryEmitOpImmReg(ctx, nmid, imm, rn));
		}
	}
#endif

	ex=0;	ex2=0;

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;
	odr=0;

	imm_is12s=((imm>=-4095) && (imm<=4095));

#if 1
	switch(nmid)
	{
	case BGBCC_SH_NMID_ADD:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		
		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
//			if((imm>=0) && (imm<=31))	break;
			if((imm>=-31) && (imm<=31))	break;
//			if(!BGBCC_JX2_CheckPadCross48(ctx))
			if(!BGBCC_JX2_CheckPadCross48(ctx) && !imm_is12s)
				if((imm>=-65535) && (imm<=65535))	break;

//			if((imm>0) && (imm<=511))	break;
//			if((imm>0) && !(imm&1) && (imm<1024))	break;
//			if((imm>0) && !(imm&3) && (imm<2048))	break;
//			if((imm>0) && !(imm&7) && (imm<4096))	break;

			if((imm>=-512) && (imm<=511))	break;
		}
		
//		odr=1; opw1=0x3800|((rn&15)<<4)|((rm&15)<<0);			break;
		odr=1; opw1=0x5800|((rn&15)<<4)|((rm&15)<<0);			break;
		break;
	case BGBCC_SH_NMID_SUB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
//			if((imm>=0) && (imm<=31))	break;
			if((imm>=-31) && (imm<=31))			break;
//			if(!BGBCC_JX2_CheckPadCross48(ctx))
			if(!BGBCC_JX2_CheckPadCross48(ctx) && !imm_is12s)
				if((imm>=-65535) && (imm<=65535))	break;
		}
		
//		odr=1; opw1=0x3900|((rn&15)<<4)|((rm&15)<<0);			break;
		odr=1; opw1=0x5900|((rn&15)<<4)|((rm&15)<<0);			break;
		break;
	case BGBCC_SH_NMID_AND:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((imm>=0) && (imm<=31))		break;
//			if(!BGBCC_JX2_CheckPadCross48(ctx))
			if(!BGBCC_JX2_CheckPadCross48(ctx) && !imm_is12s)
				if((imm>=0) && (imm<=65535))	break;
		}

#if 0
		if(imm==255)
		{	opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
			opw2=0x3204|((rn&15)<<4);	break;	}
		if(imm==65535)
		{	opw1=0x1800|((rn&15)<<4)|((rm&15)<<0);
			opw1=0x3205|((rn&15)<<4);	break;	}
#endif

//		odr=1; opw1=0x3A00|((rn&15)<<4)|((rm&15)<<0);			break;
//		odr=1; opw1=0x5A00|((rn&15)<<4)|((rm&15)<<0);			break;
		odr=1; opw1=0x5D00|((rn&15)<<4)|((rm&15)<<0);			break;
		break;
	case BGBCC_SH_NMID_OR:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((imm>=0) && (imm<=31))		break;
//			if(!BGBCC_JX2_CheckPadCross48(ctx))
			if(!BGBCC_JX2_CheckPadCross48(ctx) && !imm_is12s)
				if((imm>=0) && (imm<=65535))	break;
		}

//		odr=1; opw1=0x3B00|((rn&15)<<4)|((rm&15)<<0);			break;
//		odr=1; opw1=0x5B00|((rn&15)<<4)|((rm&15)<<0);			break;
		odr=1; opw1=0x5E00|((rn&15)<<4)|((rm&15)<<0);			break;
		break;
	case BGBCC_SH_NMID_XOR:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((imm>=0) && (imm<=31))		break;
//			if(!BGBCC_JX2_CheckPadCross48(ctx))
			if(!BGBCC_JX2_CheckPadCross48(ctx) && !imm_is12s)
				if((imm>=0) && (imm<=65535))	break;
		}

//		odr=1; opw1=0x3C00|((rn&15)<<4)|((rm&15)<<0);			break;
//		odr=1; opw1=0x5C00|((rn&15)<<4)|((rm&15)<<0);			break;
		odr=1; opw1=0x5F00|((rn&15)<<4)|((rm&15)<<0);			break;
		break;

#if 1
	case BGBCC_SH_NMID_MULL:
	case BGBCC_SH_NMID_DMULS:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((imm>=0) && (imm<=511))		break;
		}
		odr=1; opw1=0x5A00|((rn&15)<<4)|((rm&15)<<0);
		break;
#endif
#if 0
	case BGBCC_SH_NMID_DMULU:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((imm>=0) && (imm<=511))		break;
		}
		odr=1; opw1=0x5700|((rn&15)<<4)|((rm&15)<<0);
		break;
#endif

	default:
		break;
	}
#endif

	if(ctx->is_fixed32 || ctx->op_is_wex2)
	{
		if((opw1>=0) && ((opw1&0xF000)!=0xF000))
		{
			opw1=-1; odr=0;
		}

		if((opw1>=0) && ((opw1&0xFE00)==0xFC00))
		{
			opw1=-1; odr=0;
		}
	}

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
		ex2=ex<<4;

		switch(nmid)
		{
		case BGBCC_SH_NMID_ADD:
//			if(!BGBCC_JX2_CheckPadCross32(ctx))
			if(1)
			{
#if 0
				if((imm>=0) && (imm<=31))
				{	opw1=0xF080|ex|(imm&31);
					opw2=0x1000|((rn&15)<<4)|((rm&15)<<0);		break;	}
				if((imm>=-31) && (imm<=0))
				{	opw1=0xF080|ex|((-imm)&31);
					opw2=0x1100|((rn&15)<<4)|((rm&15)<<0);		break;	}
#endif

#if 0
				if((imm>=0) && (imm<=511))
				{	opw1=0xF100|ex|((imm>>4)&31);
					opw2=0x4000|((imm&15)<<8)|
						((rn&15)<<4)|((rm&15)<<0);
					break;	}
				if((imm>=-512) && (imm<0))
				{	opw1=0xF100|ex|((imm>>4)&31);
					opw2=0x5000|((imm&15)<<8)|
						((rn&15)<<4)|((rm&15)<<0);
					break;	}
#endif

#if 1
				if((imm>=0) && (imm<=511))
				{
					opw1=0xF200|((rn&15)<<4)|((rm&15)<<0);
					opw2=0x0000|ex2|(imm&511);
					break;	}
				if((imm>=-512) && (imm<0))
				{
					opw1=0xF200|((rn&15)<<4)|((rm&15)<<0);
					opw2=0x1000|ex2|(imm&511);
					break;	}
#endif
			}

			if(!BGBCC_JX2_CheckPadCross48(ctx) &&
				!ctx->is_fixed32 && !ctx->op_is_wex2)
			{
#if 0
				if((imm>=0) && (imm<=65535))
				{	opw1=0xFC08|(ex>>4)|((imm>>16)&1);
					opw2=0x1000|((rn&15)<<4)|((rm&15)<<0);
					opw3=(u16)(imm);
					break;
				}
#endif
#if 0
				if((imm>=-65535) && (imm<0))
				{	opw1=0xFC08|(ex>>4);
					opw2=0x1100|((rn&15)<<4)|((rm&15)<<0);
					opw3=(u16)(-imm);
					break;
				}
#endif
			}

			if(ctx->has_jumbo && !ctx->op_is_wex2)
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2B(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF200, 0x0000,
					rm, imm, rn);
				if(i>0)break;
			}

//			opw1=0xF008|ex; odr=1;
//			opw2=0x1D00|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex; odr=1;
			opw2=0x1000|((rn&15)<<4)|((rm&15)<<0);		break;
		case BGBCC_SH_NMID_SUB:
//			if(!BGBCC_JX2_CheckPadCross32(ctx))
			if(0)
			{
				if((imm>=0) && (imm<=31))
				{	opw1=0xF080|ex|(imm&31);
					opw2=0x1100|((rn&15)<<4)|((rm&15)<<0);		break;	}
				if((imm>=-31) && (imm<=0))
				{	opw1=0xF080|ex|((-imm)&31);
					opw2=0x1000|((rn&15)<<4)|((rm&15)<<0);		break;	}
			}
			
			if(1)
			{
#if 1
				imm1=-imm;
				if((imm1>=0) && (imm1<=511))
				{
					opw1=0xF200|((rn&15)<<4)|((rm&15)<<0);
					opw2=0x0000|ex2|(imm1&511);
					break;	}
				if((imm1>=-512) && (imm1<0))
				{
					opw1=0xF200|((rn&15)<<4)|((rm&15)<<0);
					opw2=0x1000|ex2|(imm1&511);
					break;	}
#endif
			}

			if(!BGBCC_JX2_CheckPadCross48(ctx) &&
				!ctx->is_fixed32 && !ctx->op_is_wex2)
			{
#if 0
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
			}


			if(ctx->has_jumbo && !ctx->op_is_wex2)
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2B(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF200, 0x0000,
					rm, -imm, rn);
				if(i>0)break;
			}

//			opw1=0xF009|ex; odr=1;
//			opw2=0x1D00|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex; odr=1;
			opw2=0x1100|((rn&15)<<4)|((rm&15)<<0);		break;
		case BGBCC_SH_NMID_AND:
#if 0
			if((imm>=0) && (imm<=31))
			{	opw1=0xF080|ex|(imm&31);
				opw2=0x1500|((rn&15)<<4)|((rm&15)<<0);		break;	}
#endif

#if 1
			if((imm>=0) && (imm<=511))
			{
				opw1=0xF200|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x5000|ex2|(imm&511);
				break;
			}
#endif

#if 0
			if(!BGBCC_JX2_CheckPadCross48(ctx) &&
				!ctx->is_fixed32 && !ctx->op_is_wex2)
			{
				if((imm>=0) && (imm<=65535))
				{	opw1=0xFC08|(ex>>4);
					opw2=0x1500|((rn&15)<<4)|((rm&15)<<0);
					opw3=(u16)(imm);	break;		}
			}
#endif

			if(ctx->has_jumbo && !ctx->op_is_wex2)
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2U(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF200, 0x5000,
					rm, imm, rn);
				if(i>0)break;
			}

//			opw1=0xF00A|ex; odr=1;
//			opw2=0x1D00|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex; odr=1;
			opw2=0x1500|((rn&15)<<4)|((rm&15)<<0);		break;
		case BGBCC_SH_NMID_OR:
#if 0
			if((imm>=0) && (imm<=31))
			{	opw1=0xF080|ex|(imm&31);
				opw2=0x1600|((rn&15)<<4)|((rm&15)<<0);		break;	}
#endif

#if 1
			if((imm>=0) && (imm<=511))
			{
				opw1=0xF200|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x6000|ex2|(imm&511);
				break;
			}
#endif

#if 0
			if(!BGBCC_JX2_CheckPadCross48(ctx) &&
				!ctx->is_fixed32 && !ctx->op_is_wex2)
			{
				if((imm>=0) && (imm<=65535))
				{	opw1=0xFC08|(ex>>4);
					opw2=0x1600|((rn&15)<<4)|((rm&15)<<0);
					opw3=(u16)(imm);	break;		}
			}
#endif

			if(ctx->has_jumbo && !ctx->op_is_wex2)
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2U(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF200, 0x6000,
					rm, imm, rn);
				if(i>0)break;
			}

//			opw1=0xF00B|ex; odr=1;
//			opw2=0x1D00|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex; odr=1;
			opw2=0x1600|((rn&15)<<4)|((rm&15)<<0);		break;
		case BGBCC_SH_NMID_XOR:
#if 0
			if((imm>=0) && (imm<=31))
			{	opw1=0xF080|ex|(imm&31);
				opw2=0x1700|((rn&15)<<4)|((rm&15)<<0);		break;	}
#endif

#if 1
			if((imm>=0) && (imm<=511))
			{
				opw1=0xF200|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x7000|ex2|(imm&511);
				break;
			}
#endif

#if 0
			if(!BGBCC_JX2_CheckPadCross48(ctx) &&
				!ctx->is_fixed32 && !ctx->op_is_wex2)
			{
				if((imm>=0) && (imm<=65535))
				{	opw1=0xFC08|(ex>>4);
					opw2=0x1700|((rn&15)<<4)|((rm&15)<<0);
					opw3=(u16)(imm);	break;		}
			}
#endif

			if(ctx->has_jumbo && !ctx->op_is_wex2)
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2U(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF200, 0x7000,
					rm, imm, rn);
				if(i>0)break;
			}

//			opw1=0xF00C|ex; odr=1;
//			opw2=0x1D00|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex; odr=1;
			opw2=0x1700|((rn&15)<<4)|((rm&15)<<0);		break;

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
#if 0
			if((imm&31)==imm)
			{	opw1=0xF000|ex|(imm&31);
				opw2=0x5600|((rn&15)<<4)|((rm&15)<<0);
				break;			}
			if((imm|(~31))==imm)
			{	opw1=0xF080|ex|(imm&31);
				opw2=0x5600|((rn&15)<<4)|((rm&15)<<0);
				break;			}
#endif

			if(1)
			{
				opw1=0xF200|((rn&15)<<4)|((rm&15)<<0);
//				opw2=0x8000|ex2|(imm&511);
				opw2=0x8000|ex2|(imm&255);
				break;
			}

//			opw1=0xF006|ex; odr=1;
//			opw2=0x1D00|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex; odr=1;
			opw2=0x5400|((rn&15)<<4)|((rm&15)<<0);		break;
		case BGBCC_SH_NMID_SHLD:
#if 0
			if((imm&31)==imm)
			{	opw1=0xF000|ex|(imm&31);
				opw2=0x5700|((rn&15)<<4)|((rm&15)<<0);
				break;			}
			if((imm|(~31))==imm)
			{	opw1=0xF080|ex|(imm&31);
				opw2=0x5700|((rn&15)<<4)|((rm&15)<<0);
				break;			}
#endif
			if(1)
			{
				opw1=0xF200|((rn&15)<<4)|((rm&15)<<0);
//				opw2=0x9000|ex2|(imm&511);
				opw2=0x9000|ex2|(imm&255);
				break;
			}

//			opw1=0xF000|ex|(imm&31);
//			opw2=0x5700|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex; odr=1;
			opw2=0x5500|((rn&15)<<4)|((rm&15)<<0);		break;
#endif

		case BGBCC_SH_NMID_SHADQ:
#if 0
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
#endif

			if(1)
			{
				opw1=0xF200|((rn&15)<<4)|((rm&15)<<0);
//				opw2=0x8800|ex2|(imm&511);
				opw2=0x8800|ex2|(imm&255);
				break;
			}

//			opw1=0xF086|ex; odr=1;
//			opw2=0x1D00|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF080|ex; odr=1;
			opw2=0x5400|((rn&15)<<4)|((rm&15)<<0);		break;
		case BGBCC_SH_NMID_SHLDQ:
#if 0
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
#endif

			if(1)
			{
				opw1=0xF200|((rn&15)<<4)|((rm&15)<<0);
//				opw2=0x9800|ex2|(imm&511);
				opw2=0x9800|ex2|(imm&255);
				break;
			}

//			opw1=0xF087|ex; odr=1;
//			opw2=0x1D00|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF080|ex; odr=1;
			opw2=0x5500|((rn&15)<<4)|((rm&15)<<0);		break;


		case BGBCC_SH_NMID_ADDSL:
		case BGBCC_SH_NMID_ADDUL:
			if(nmid==BGBCC_SH_NMID_ADDUL)
			{
				ex |=0x0080;
				ex2|=0x0800;
			}
			if(0)
//			if(1)
			{
				if((imm>=0) && (imm<=511))
				{	opw1=0xF100|ex|((imm>>4)&31);
					opw2=0xC000|((imm&15)<<8)|
						((rn&15)<<4)|((rm&15)<<0);
					break;	}
				if((imm>=-512) && (imm<0))
				{	opw1=0xF100|ex|((imm>>4)&31);
					opw2=0xD000|((imm&15)<<8)|
						((rn&15)<<4)|((rm&15)<<0);
					break;	}
			}

			if(1)
			{
#if 1
				if((imm>=0) && (imm<=511))
				{
					opw1=0xF200|((rn&15)<<4)|((rm&15)<<0);
					opw2=0x3000|ex2|(imm&511);
					break;	}
				if((imm>=-512) && (imm<0))
				{
					opw1=0xF200|((rn&15)<<4)|((rm&15)<<0);
					opw2=0x4000|ex2|(imm&511);
					break;	}
#endif
			}

			if(!BGBCC_JX2_CheckPadCross48(ctx) &&
				!ctx->is_fixed32 && !ctx->op_is_wex2)
			{
#if 0
				if((imm>=0) && (imm<=65535))
				{	opw1=0xFC08|(ex>>4)|((imm>>16)&1);
					opw2=0x1000|((rn&15)<<4)|((rm&15)<<0);
					opw3=(u16)(imm);
					break;
				}
#endif
#if 0
				if((imm>=-65535) && (imm<0))
				{	opw1=0xFC08|(ex>>4);
					opw2=0x1100|((rn&15)<<4)|((rm&15)<<0);
					opw3=(u16)(-imm);
					break;
				}
#endif
			}

			if(ctx->has_jumbo && !ctx->op_is_wex2)
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2B(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF200, 0x3000|ex2,
					rm, imm, rn);
				if(i>0)break;
			}

			opw1=0xF000|ex; odr=1;
			opw2=0x5C00|((rn&15)<<4)|((rm&15)<<0);
			break;

		case BGBCC_SH_NMID_PSHUFB:
			if((imm>=0) && (imm<=255))
			{
				opw1=0xF200|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x8100|ex2|(imm&255);
				break;
			}
			break;
		case BGBCC_SH_NMID_PSHUFW:
			if((imm>=0) && (imm<=255))
			{
				opw1=0xF200|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x8900|ex2|(imm&255);
				break;
			}
			break;

		case BGBCC_SH_NMID_LEAB:
			if((imm>=0) && (imm<=511))
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x0800|ex2|(imm&511);
				break;
			}
			break;
		case BGBCC_SH_NMID_LEAW:
			if((imm>=0) && (imm<=511))
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x1800|ex2|(imm&511);
				break;
			}
			break;
		case BGBCC_SH_NMID_LEAL:
			if((imm>=0) && (imm<=511))
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x2800|ex2|(imm&511);
				break;
			}
			break;
		case BGBCC_SH_NMID_LEAQ:
			if((imm>=0) && (imm<=511))
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x3800|ex2|(imm&511);
				break;
			}
			break;

		case BGBCC_SH_NMID_MULL:
		case BGBCC_SH_NMID_DMULS:
			if((imm>=0) && (imm<=511))
			{
				opw1=0xF200|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x2000|ex2|(imm&511);
				break;
			}

			if(ctx->has_jumbo && !ctx->op_is_wex2)
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2U(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF200, 0x2000,
					rm, imm, rn);
				if(i>0)break;
			}

			opw1=0xF000|ex; odr=1;
			opw2=0x1200|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_DMULU:
			if((imm>=0) && (imm<=511))
			{
				opw1=0xF200|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x2800|ex2|(imm&511);
				break;
			}

			if(ctx->has_jumbo && !ctx->op_is_wex2)
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2U(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF200, 0x2800,
					rm, imm, rn);
				if(i>0)break;
			}

			opw1=0xF080|ex; odr=1;
			opw2=0x1200|((rn&15)<<4)|((rm&15)<<0);
			break;

		case BGBCC_SH_NMID_MULSL:
			opw1=0xF000|ex; odr=1;
			opw2=0x5200|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_MULUL:
			opw1=0xF000|ex; odr=1;
			opw2=0x5300|((rn&15)<<4)|((rm&15)<<0);
			break;


		case BGBCC_SH_NMID_MULSW:
#if 0
			if((imm>=0) && (imm<=511))
			{
				opw1=0xF200|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x2000|ex2|(imm&511);
				break;
			}
#endif
#if 1
			if((imm>=0) && (imm<=31))
			{
				opw1=0xF080|ex|(imm&31);
				opw2=0x5E00|((rn&15)<<4)|((rm&15)<<0);
				break;
			}
#endif
			opw1=0xF000|ex; odr=1;
			opw2=0x5E00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_MULUW:
#if 0
			if((imm>=0) && (imm<=511))
			{
				opw1=0xF200|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x2800|ex2|(imm&511);
				break;
			}
#endif
#if 1
			if((imm>=0) && (imm<=31))
			{
				opw1=0xF080|ex|(imm&31);
				opw2=0x5F00|((rn&15)<<4)|((rm&15)<<0);
				break;
			}
#endif
			opw1=0xF000|ex; odr=1;
			opw2=0x5F00|((rn&15)<<4)|((rm&15)<<0);
			break;
		}
	}

	if(opw1>=0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpRegImmReg(ctx, nmid, rm, imm, rn);
	
		if(odr)
			BGBCC_JX2_EmitLoadDrImm(ctx, imm);

//		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
		BGBCC_JX2_EmitOpCheckRepack4(ctx, &opw1, &opw2, &opw3, &opw4);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
		return(1);
	}

	return(0);
}
