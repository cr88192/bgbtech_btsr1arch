/*
Encode XG3 Ops.
XG3 will be considered a sub-mode of RISC-V Mode, and so will primarily be intended for cases where XG3 can express something more efficiently than a corresponding RISC-V op.
 */

int BGBCC_JX2X3_CheckRepack1(
	BGBCC_JX2_Context *ctx, s64 *ropw1)
{
	s64 opw1;
	
	opw1=*ropw1;
	if(opw1<0)
		return(0);
	
	if(ctx->op_is_wex2&4)
	{
		if(ctx->op_is_wex2&1)
		{
			opw1=opw1&(~3);
			opw1=opw1|1;
		}else
		{
			opw1=opw1&(~3);
			opw1=opw1|0;
		}
	}
	
	*ropw1=opw1;
	return(0);
}

int BGBCC_JX2X3_CheckRepack3(
	BGBCC_JX2_Context *ctx, s64 *ropw1, s64 *ropw2, s64 *ropw3)
{
	BGBCC_JX2X3_CheckRepack1(ctx, ropw1);
	BGBCC_JX2X3_CheckRepack1(ctx, ropw2);
	BGBCC_JX2X3_CheckRepack1(ctx, ropw3);
	return(0);
}

int BGBCC_JX2X3_CheckEncodeRIRJ_Imm10I(
	BGBCC_JX2_Context *ctx,
	s64 opwb, int rm, s64 imm, int rn,
	s64 *ropw1, s64 *ropw2)
{
	s64 opw1, opw2;
	
//	return(0);
		
	if((((s32)imm)==imm) || (((u32)imm)==imm))
	{
		*ropw1=0x0000001AU |
			(((imm>> 8)&0xFFFF)<<16) |
			(((imm>>24)&0x00FF)<< 8) ;
		*ropw2=opwb|
			((rm&63)<<16)|
			((rn&63)<<6)|
			((imm&255)<<22) |
			(((imm>>32)&1)<<30);
		return(1);
	}
	
	return(0);
}

int BGBCC_JX2X3_CheckEncodeRIRJ_Imm10s(
	BGBCC_JX2_Context *ctx,
	s64 opwb, int rm, s64 imm, int rn,
	s64 *ropw1, s64 *ropw2)
{
	s64 opw1, opw2;
	
//	return(0);
	
	if((imm>=(-512)) && (imm<=( 511)))
	{
		*ropw1=opwb|((rm&63)<<16)|((rn&63)<<6)|((imm&1023)<<22);
		return(1);
	}
	
	return(BGBCC_JX2X3_CheckEncodeRIRJ_Imm10I(ctx,
		opwb, rm, imm, rn, ropw1, ropw2));
}

int BGBCC_JX2X3_CheckEncodeRIRJ_Imm10u(
	BGBCC_JX2_Context *ctx,
	s64 opwb, int rm, s64 imm, int rn,
	s64 *ropw1, s64 *ropw2)
{
	s64 opw1, opw2;
	
//	return(0);
	
	if((imm>=0) && (imm<=1023))
	{
		*ropw1=opwb|((rm&63)<<16)|((rn&63)<<6)|((imm&1023)<<22);
		return(1);
	}
	
	return(BGBCC_JX2X3_CheckEncodeRIRJ_Imm10I(ctx,
		opwb, rm, imm, rn, ropw1, ropw2));
}

int BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(
	BGBCC_JX2_Context *ctx,
	s64 opwb, int rm, s64 imm, int shr, int rn,
	s64 *ropw1, s64 *ropw2)
{
	s64 opw1, opw2;
	s64 immsc;
	int immfr;
	
//	return(0);
	
	immsc=imm>>shr;
	immfr=imm&((1<<shr)-1);
	
	if((immsc>=(-512)) && (immsc<=( 511)) && !immfr)
	{
		*ropw1=opwb|((rm&63)<<16)|((rn&63)<<6)|((immsc&1023)<<22);
		return(1);
	}
	
	if(!immfr)
	{
		return(BGBCC_JX2X3_CheckEncodeRIRJ_Imm10I(ctx,
			opwb, rm, immsc, rn, ropw1, ropw2));
	}
	
	return(0);
}


int BGBCC_JX2X3_CheckEncodeRIRJ_Imm6I(
	BGBCC_JX2_Context *ctx,
	s64 opwb, int rm, s64 imm, int rn,
	s64 *ropw1, s64 *ropw2)
{
	s64 opw1, opw2;
	int isimm29s;
	
//	return(0);
	
	isimm29s=0;
	if((((s32)(imm<<3))>>3)==imm)
		isimm29s=1;
	
//	if((((s32)imm)==imm) || (((u32)imm)==imm))
	if(isimm29s)
	{
		*ropw1=0x0000001AU |
			(((imm>> 4)&0xFFFF)<<16) |
			(((imm>>20)&0x00FF)<< 8) |
			(((imm>>29)&0x0007)<< 5) ;
		*ropw2=opwb|
			((rm&63)<<16)|
			((rn&63)<<6)|
			((imm&15)<<22) |
			(((imm>>28)&1)<<26) |
			(((imm>>32)&1)<<27);
		return(1);
	}
	
	return(0);
}

int BGBCC_JX2X3_CheckEncodeRIRJ_Imm6s(
	BGBCC_JX2_Context *ctx,
	s64 opwb, int rm, s64 imm, int rn,
	s64 *ropw1, s64 *ropw2)
{
	s64 opw1, opw2;
	
//	return(0);
	
	if((imm>=(-32)) && (imm<=( 31)))
	{
		*ropw1=opwb|((rm&63)<<16)|((rn&63)<<6)|((imm&63)<<22);
		return(1);
	}
	
	return(BGBCC_JX2X3_CheckEncodeRIRJ_Imm6I(ctx,
		opwb, rm, imm, rn, ropw1, ropw2));
}



int BGBCC_JX2X3_TryEmitOpRegRegReg(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int ro, int rn)
{
	s64 opw1, opw2, opw3;

	if(!(ctx->emit_riscv&0x11) || !(ctx->emit_riscv&0x22))
		return(0);

//	return(0);

	rm=BGBCC_JX2RV_NormalizeReg(ctx, rm);
	ro=BGBCC_JX2RV_NormalizeReg(ctx, ro);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	if(	!BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) ||
		!BGBCC_JX2_EmitCheckRegExtGPR(ctx, ro) ||
		!BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
			return(0);

#if 0
	if(	BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
		BGBCC_JX2RV_CheckRegIsGPR(ctx, ro) &&
		BGBCC_JX2RV_CheckRegIsGPR(ctx, rn) &&
		!(ctx->op_is_wex2&4))
	{
		/* Check if we should reject and fall back to RV op */
		if(	(nmid==BGBCC_SH_NMID_ADD) ||
			(nmid==BGBCC_SH_NMID_SUB) ||
			(nmid==BGBCC_SH_NMID_AND) ||
			(nmid==BGBCC_SH_NMID_OR) ||
			(nmid==BGBCC_SH_NMID_XOR) ||
			(nmid==BGBCC_SH_NMID_ADDSL) ||
			(nmid==BGBCC_SH_NMID_SUBSL) ||

			(nmid==BGBCC_SH_NMID_SHAD) ||
			(nmid==BGBCC_SH_NMID_SHLD) ||
			(nmid==BGBCC_SH_NMID_SHADQ) ||
			(nmid==BGBCC_SH_NMID_SHLDQ) ||
			(nmid==BGBCC_SH_NMID_SHAR) ||
			(nmid==BGBCC_SH_NMID_SHLR) ||
			(nmid==BGBCC_SH_NMID_SHARQ) ||
			(nmid==BGBCC_SH_NMID_SHLRQ) ||

			0)
		{
			return(0);
		}
	}
#endif

	opw1=-1;
	opw2=-1;
	opw3=-1;

	switch(nmid)
	{
	case BGBCC_SH_NMID_ADD:
		opw1=0x00001002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_ADDX:
		opw1=0x00001022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_SUB:
		opw1=0x10001002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_SUBX:
		opw1=0x10001002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_DMULS:
	case BGBCC_SH_NMID_MULL:
		opw1=0x20001002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_MULSQ:
		opw1=0x20001022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_DMULU:
		opw1=0x30001002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_MULUQ:
		opw1=0x30001022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;

#if 1
	case BGBCC_SH_NMID_MIN:
		opw1=0x40001002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_MAX:
		opw1=0x40001022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;

	case BGBCC_SH_NMID_AND:
		opw1=0x50001002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_ANDX:
		opw1=0x50001022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_OR:
		opw1=0x60001002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_ORX:
		opw1=0x60001002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_XOR:
		opw1=0x70001002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_XORX:
		opw1=0x70001002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
#endif

#if 1
	case BGBCC_SH_NMID_PADDW:
		opw1=0x00002002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_PADDL:
		opw1=0x00002022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_PSUBW:
		opw1=0x10002002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_PSUBL:
		opw1=0x10002022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;

	case BGBCC_SH_NMID_SHAR:
		opw1=0x20002002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_SHARQ:
		opw1=0x20002022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_SHLR:
		opw1=0x30002002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_SHLRQ:
		opw1=0x30002022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;

	case BGBCC_SH_NMID_PCSELTW:
		opw1=0x40002002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_PCSELTL:
		opw1=0x40002022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;

	case BGBCC_SH_NMID_PADDF:
		opw1=0x50002002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_PADDFX:
		opw1=0x50002022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_PSUBF:
		opw1=0x60002002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_PSUBFX:
		opw1=0x60002022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_PMULF:
		opw1=0x70002002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_PMULFX:
		opw1=0x70002022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
#endif

#if 1
	case BGBCC_SH_NMID_MOVHD:
		opw1=0x80002002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_MOVLD:
		opw1=0x80002022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_MOVHLD:
		opw1=0x90002002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_MOVLHD:
		opw1=0x90002022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;

	case BGBCC_SH_NMID_PSCHEQW:
		opw1=0xA0002002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_PSCHEQB:
		opw1=0xA0002022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_PSCHNEW:
		opw1=0xB0002002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_PSCHNEB:
		opw1=0xB0002022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;

	case BGBCC_SH_NMID_BLKUTX1:
		opw1=0xC0002002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_BLKUTX2:
		opw1=0xC0002022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;

	case BGBCC_SH_NMID_PADDH:
		opw1=0xD0002002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_PADDXD:
		opw1=0xD0002022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_PSUBH:
		opw1=0xE0002002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_PSUBXD:
		opw1=0xE0002022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_PMULH:
		opw1=0xF0002002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_PMULXD:
		opw1=0xF0002022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;

	case BGBCC_SH_NMID_MOVTT:
		opw1=0x10003002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
//	case BGBCC_SH_NMID_PADDXD:
//		opw1=0x10003022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
//		break;

	case BGBCC_SH_NMID_ROTL:
		opw1=0x20003002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_SHARX:
		opw1=0x20003022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_ROTR:
		opw1=0x30003002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_SHLRX:
		opw1=0x30003022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;

	case BGBCC_SH_NMID_MULHSQ:
		opw1=0x40003002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_SHADX:
		opw1=0x40003022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_MULHUQ:
		opw1=0x50003002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_SHLDX:
		opw1=0x50003022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;

//	case BGBCC_SH_NMID_ROTLL:
//		opw1=0x60003002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
//		break;
	case BGBCC_SH_NMID_ROTLX:
		opw1=0x60003022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;

	case BGBCC_SH_NMID_CSELT:
		opw1=0x00005002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
//	case BGBCC_SH_NMID_PADDXD:
//		opw1=0x00005022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
//		break;
	case BGBCC_SH_NMID_PMULSW:
		opw1=0x10005002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_PMULUW:
		opw1=0x10005022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;

	case BGBCC_SH_NMID_MULSL:
		opw1=0x20005002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
//	case BGBCC_SH_NMID_DMULSQ:
//		opw1=0x20005022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
//		break;
	case BGBCC_SH_NMID_MULUL:
		opw1=0x30005002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
//	case BGBCC_SH_NMID_DMULUQ:
//		opw1=0x30005022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
//		break;

	case BGBCC_SH_NMID_SHAD:
		opw1=0x40005002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_SHADQ:
		opw1=0x40005022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;

	case BGBCC_SH_NMID_SHLD:
		opw1=0x50005002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_SHLDQ:
		opw1=0x50005022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;

	case BGBCC_SH_NMID_PMULSLW:
		opw1=0x60005002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_PMULULW:
		opw1=0x60005022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_PMULSHW:
		opw1=0x70005002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_PMULUHW:
		opw1=0x70005022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;

	case BGBCC_SH_NMID_FADD:
		opw1=0x80005002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_FADDX:
		opw1=0x80005022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_FSUB:
		opw1=0x90005002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_FSUBX:
		opw1=0x90005022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_FMUL:
		opw1=0xA0005002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_FMULX:
		opw1=0xA0005022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_FMAC:
		opw1=0xB0005002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_FMACX:
		opw1=0xB0005022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
#endif

#if 1
	case BGBCC_SH_NMID_ADDSL:
		opw1=0xC0005002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_ADDUL:
		opw1=0xC0005022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_SUBSL:
		opw1=0xD0005002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_SUBUL:
		opw1=0xD0005022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;

	case BGBCC_SH_NMID_MULSW:
		opw1=0xE0005002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
//	case BGBCC_SH_NMID_MULSW:
//		opw1=0xE0005022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
//		break;
	case BGBCC_SH_NMID_MULUW:
		opw1=0xF0005002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
//	case BGBCC_SH_NMID_MULUW:
//		opw1=0xF0005022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
//		break;


	case BGBCC_SH_NMID_MACSL:
		opw1=0x00006002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_MACUL:
		opw1=0x10006002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_DMACSL:
		opw1=0x20006002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_DMACUL:
		opw1=0x30006002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;

	case BGBCC_SH_NMID_DIVSQ:
		opw1=0x40006002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_DIVUQ:
		opw1=0x40006022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_MODSQ:
		opw1=0x50006002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_MODUQ:
		opw1=0x50006022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_FDIV:
		opw1=0x60006002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_FDIVX:
		opw1=0x60006022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
//	case BGBCC_SH_NMID_FDIVA:
//		opw1=0x70006002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
//		break;
//	case BGBCC_SH_NMID_FDIVXA:
//		opw1=0x70006022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
//		break;

//	case BGBCC_SH_NMID_BLKUTX3H:
//		opw1=0x80006002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
//		break;
	case BGBCC_SH_NMID_BLKUTX3H:
		opw1=0x80006022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
//	case BGBCC_SH_NMID_BLERP:
//		opw1=0x90006002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
//		break;
	case BGBCC_SH_NMID_BLKUTX3L:
		opw1=0x90006022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
//	case BGBCC_SH_NMID_BLINTA:
//		opw1=0xA0006002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
//		break;
//	case BGBCC_SH_NMID_BLINT:
//		opw1=0xA0006022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
//		break;
	case BGBCC_SH_NMID_BITSEL:
		opw1=0xB0006002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_BITSELX:
		opw1=0xB0006022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_BLKUAB1:
		opw1=0xC0006002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_BLKUAB2:
		opw1=0xC0006022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;

	case BGBCC_SH_NMID_FADDG:
		opw1=0xD0006002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
//	case BGBCC_SH_NMID_FADDI:
//		opw1=0xD0006022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
//		break;
	case BGBCC_SH_NMID_FSUBG:
		opw1=0xE0006002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
//	case BGBCC_SH_NMID_FSUBI:
//		opw1=0xE0006022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
//		break;
	case BGBCC_SH_NMID_FMULG:
		opw1=0xF0006002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
//	case BGBCC_SH_NMID_FMULI:
//		opw1=0xD0006022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
//		break;
#endif

#if 1
	case BGBCC_SH_NMID_FADDA:
		opw1=0x00007002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_FMIN:
		opw1=0x00007022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_FSUBA:
		opw1=0x10007002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_FMAX:
		opw1=0x10007022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_FMULA:
		opw1=0x20007002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_BITNN:
		opw1=0x20007022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_MOVHW:
		opw1=0x30007002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_MOVLW:
		opw1=0x30007022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_DIVSL:
		opw1=0x40007002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_DIVUL:
		opw1=0x40007022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_MODSL:
		opw1=0x50007002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_MODUL:
		opw1=0x50007022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_RGB5CCENC1:
		opw1=0x60007002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_PMULF8H:
		opw1=0x60007022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;

	case BGBCC_SH_NMID_TST:
	case BGBCC_SH_NMID_TSTQ:
//		opw1=0x10008022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		opw1=0x20008022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_NTST:
//		opw1=0x20008022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		opw1=0x10008022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;


	case BGBCC_SH_NMID_CMPQEQ:
	case BGBCC_SH_NMID_CMPEQ:
		opw1=0x40009022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_CMPQGT:
	case BGBCC_SH_NMID_CMPGT:
		opw1=0x50009022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_CMPQNE:
//	case BGBCC_SH_NMID_CMPNE:
		opw1=0x60009022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_CMPQGE:
	case BGBCC_SH_NMID_CMPGE:
		opw1=0x70009022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;

	case BGBCC_SH_NMID_CMPQLT:
//	case BGBCC_SH_NMID_CMPLT:
		opw1=0x50009022U|((rm&63)<<22)|((ro&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_CMPQLE:
//	case BGBCC_SH_NMID_CMPLE:
		opw1=0x70009022U|((rm&63)<<22)|((ro&63)<<16)|((rn&63)<<6);
		break;
#endif
	}

	if(opw1>0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpRegRegReg(ctx, nmid, rm, ro, rn);
		
		BGBCC_JX2X3_CheckRepack3(ctx, &opw1, &opw2, &opw3);

		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		if(opw3>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw3);
		return(1);
	}

	return(0);
}

int BGBCC_JX2X3_TryEmitOpRegReg(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int rn)
{
	s64 opw1, opw2, opw3;
	int nm1;

	if(!(ctx->emit_riscv&0x11) || !(ctx->emit_riscv&0x22))
		return(0);

	rm=BGBCC_JX2RV_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	if(	(nmid==BGBCC_SH_NMID_CMPQEQ) ||
		(nmid==BGBCC_SH_NMID_CMPQGT) ||
		(nmid==BGBCC_SH_NMID_CMPQNE) ||
		(nmid==BGBCC_SH_NMID_CMPQGE))
	{
		if(!(ctx->has_jcmp&16))
			return(0);
		return(BGBCC_JX2X3_TryEmitOpRegRegReg(
			ctx, nmid, rn, rm, BGBCC_SH_REG_RQ0));
	}

	if(	(nmid==BGBCC_SH_NMID_CMPEQ) ||
		(nmid==BGBCC_SH_NMID_CMPGT) ||
		(nmid==BGBCC_SH_NMID_CMPGE))
	{
		if(!(ctx->has_jcmp&16))
			return(0);
		return(BGBCC_JX2X3_TryEmitOpRegRegReg(
			ctx, nmid, rn, rm, BGBCC_SH_REG_RQ0));
	}
	
	nm1=-1;
	if(nmid==BGBCC_SH_NMID_MOV)
		nm1=BGBCC_SH_NMID_ADD;
	if(nmid==BGBCC_SH_NMID_EXTSL)
		nm1=BGBCC_SH_NMID_ADDSL;
	if(nmid==BGBCC_SH_NMID_EXTUL)
		nm1=BGBCC_SH_NMID_ADDUL;
	
	if(nm1>0)
	{
		return(BGBCC_JX2X3_TryEmitOpRegRegReg(
			ctx, nm1, rm, BGBCC_SH_REG_RQ0, rn));
	}

	if(	!BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) ||
		!BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
			return(0);

	opw1=-1;
	opw2=-1;
	opw3=-1;

	switch(nmid)
	{
	case BGBCC_SH_NMID_EXTSB:
		opw1=0xC2001002U|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_EXTUB:
		opw1=0xC2001022U|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_EXTSW:
		opw1=0xC2401002U|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_EXTUW:
		opw1=0xC2401022U|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_EXTSL:
		opw1=0xC1401002U|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_EXTUL:
		opw1=0xC1401022U|((rm&63)<<16)|((rn&63)<<6);
		break;
	}

	if(opw1>0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpRegReg(ctx, nmid, rm, rn);
		
		BGBCC_JX2X3_CheckRepack3(ctx, &opw1, &opw2, &opw3);

		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		if(opw3>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw3);
		return(1);
	}

	return(0);
}

int BGBCC_JX2X3_UpdateStat2RI(
	BGBCC_JX2_Context *ctx, int nmid, s64 imm)
{
	if(!ctx->is_simpass)
	{
#if 1
		ctx->stat_imm2ri_immtot++;
		if((imm&63)==imm)
			ctx->stat_imm2ri_imm6u++;
		if((imm|(~63))==imm)
			ctx->stat_imm2ri_imm6n++;
		if((imm&1023)==imm)
			ctx->stat_imm2ri_imm10u++;
		if((imm|(~1023))==imm)
			ctx->stat_imm2ri_imm10n++;
		if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			ctx->stat_imm2ri_imm10un++;
#endif
	}

	return(0);
}

int BGBCC_JX2X3_UpdateStat3RI(
	BGBCC_JX2_Context *ctx, int nmid, s64 imm)
{
	if(!ctx->is_simpass)
	{
#if 1
		ctx->stat_imm3ri_immtot++;

		if((imm&31)==imm)
			ctx->stat_imm3ri_imm5u++;
		if((imm|(~31))==imm)
			ctx->stat_imm3ri_imm5n++;

		if((imm&511)==imm)
			ctx->stat_imm3ri_imm9u++;
		if((imm|(~511))==imm)
			ctx->stat_imm3ri_imm9n++;
		if(((imm&511)==imm) || ((imm|(~511))==imm))
			ctx->stat_imm3ri_imm9un++;

		if((imm&1023)==imm)
			ctx->stat_imm3ri_imm10u++;
		if((imm|(~1023))==imm)
			ctx->stat_imm3ri_imm10n++;
		if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			ctx->stat_imm3ri_imm10un++;
#endif
	}

	return(0);
}

int BGBCC_JX2X3_UpdateStat2RIM(
	BGBCC_JX2_Context *ctx, int nmid, s64 imm)
{
	if(ctx->is_simpass)
		return(0);

	s64 immhi, imm1;
	u32 imm_f32, imm1_f32, imm2_f32;
	u16 imm_f16, imm1_f16, imm2_f16, imm3_f16, imm4_f16;
	int rt1, rt2;

	rt1=BGBCC_JX2_ConstConvDoubleToFloat(imm, &imm_f32);
	rt2=BGBCC_JX2_ConstConvFloatToHalf(imm_f32, &imm_f16);

	ctx->stat_const_masktot++;

	if((((imm>>12)&1023)<<12)==imm)
		ctx->stat_mask_imm10u_sh12++;
	if((((imm>>16)&1023)<<16)==imm)
		ctx->stat_mask_imm10u_sh16++;

	if((rt1>0) && imm)
	{
		ctx->stat_fp16_isfpa++;
		if(rt2>0)
		{
			ctx->stat_fp16_tot++;
		}
	}

	return(0);
}

int BGBCC_JX2X3_TryEncodeLoadImmReg(
	BGBCC_JX2_Context *ctx, s64 imm, int rn,
	s64 *ropw1, s64 *ropw2, s64 *ropw3)
{
	s64 opw1, opw2, opw3;
	s64 immhi, imm1;
	u32 imm_f32, imm1_f32, imm2_f32, imm_2xf16, imm_4xf8u;
	u16 imm_f16, imm1_f16, imm2_f16, imm3_f16, imm4_f16;
	int rt1, rt2, rt3, rt4;

	BGBCC_JX2X3_UpdateStat2RIM(ctx, 0, imm);

	rt1=BGBCC_JX2_ConstConvDoubleToFloat(imm, &imm_f32);
	rt2=BGBCC_JX2_ConstConvFloatToHalf(imm_f32, &imm_f16);
	rt3=BGBCC_JX2_ConstConvPackedFloatToHalf2x(imm, &imm_2xf16);
	rt4=BGBCC_JX2_ConstConvV4HToV4FP8U(imm, &imm_4xf8u);

	opw1=-1;
	opw2=-1;
	opw3=-1;

	if((opw1<0) && ((imm&0xFFFF)==imm))
	{
		opw1=0x00000012U|((rn&63)<<6)|((imm&0xFFFF)<<16);

		if(!ctx->is_simpass)
		{
			ctx->stat_const_imm16++;
			ctx->stat_const_imm16u++;
		}
	}

	if((opw1<0) && ((imm|(~0xFFFF))==imm))
	{
		opw1=0x00001012U|((rn&63)<<6)|((imm&0xFFFF)<<16);

		if(!ctx->is_simpass)
		{
			ctx->stat_const_imm16++;
			ctx->stat_const_imm16n++;
		}
	}
	
#if 1
	if((opw1<0) && (rt1>0) && (rt2>0))
	{
		opw1=0x00004012U|((rn&63)<<6)|((imm_f16&0xFFFFU)<<16);

		if(!ctx->is_simpass)
		{
			ctx->stat_const_imm16++;
			ctx->stat_const_imm16h++;
		}
	}
#endif
	
	if((opw1<0) && BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))
	{
		immhi=(((s32)imm)>>12);
		imm1=immhi&0xFFFFF;
		if((immhi<<12)==imm)
		{
			opw1=0x00000037|((rn&31)<<7)|(imm1<<12);

			if(!ctx->is_simpass)
			{
//				ctx->stat_const_imm16++;
				ctx->stat_const_imm20lui++;
			}
		}
	}

	if(opw1<0)
	{
		BGBCC_JX2X3_CheckEncodeRIRJ_Imm10u(ctx, 0x0000000AU,
			BGBCC_SH_REG_RQ0, imm, rn, &opw1, &opw2);

		if(!ctx->is_simpass && (opw1>=0))
		{
			ctx->stat_const_jumbo64++;
			ctx->stat_const_jumbo64_imm33l++;
		}
	}

#if 1
	immhi=(imm>>32);
	if((opw1<0) && ((immhi<<32)==imm))
	{
		opw1=0x0000001EU|(immhi&0xFFFF0000U);
		opw2=0x00003032U|((rn&63)<<6)|((immhi&0xFFFF)<<16);

		if(!ctx->is_simpass && (opw1>=0))
		{
			ctx->stat_const_jumbo64++;
			ctx->stat_const_jumbo64_imm32h++;
		}
	}
#endif

#if 1
	immhi=imm_f32;
	if((opw1<0) && (rt1>0))
	{
		opw1=0x0000001EU|(immhi&0xFFFF0000U);
		opw2=0x00004012U|((rn&63)<<6)|((immhi&0xFFFF)<<16);

		if(!ctx->is_simpass && (opw1>=0))
		{
			ctx->stat_const_jumbo64++;
			ctx->stat_const_jumbo64_f32++;
		}
	}
#endif

#if 1
	immhi=imm_2xf16;
	if((opw1<0) && (rt3>0))
	{
		opw1=0x0000001EU|(immhi&0xFFFF0000U);
		opw2=0x00004032U|((rn&63)<<6)|((immhi&0xFFFF)<<16);

		if(!ctx->is_simpass && (opw1>=0))
		{
			ctx->stat_const_jumbo64++;
			ctx->stat_const_jumbo64_2xf16++;
		}
	}
#endif

#if 1
	immhi=imm_4xf8u;
	if((opw1<0) && (rt4>0))
	{
		opw1=0x0000001EU|(immhi&0xFFFF0000U);
		opw2=0x0000C032U|((rn&63)<<6)|((immhi&0xFFFF)<<16);

		if(!ctx->is_simpass && (opw1>=0))
		{
			ctx->stat_const_jumbo64++;
			ctx->stat_const_jumbo64_4xf8++;
		}
	}
#endif

	if((opw1<0) && (ctx->has_jumbo&4))
	{
		opw1=0x0000001AU|
			(((imm>>40)&0xFFFF)<<16)|
			(((imm>>56)&0x00FF)<< 8);
		opw2=0x0000001AU|(((imm>>16)&0xFFFF)<<16)|
			(((imm>>32)&0x00FF)<< 8);
		opw3=0x00000012U|
			((rn&63)<<6)|
			((imm&0xFFFF)<<16);

		if(!ctx->is_simpass && (opw1>=0))
		{
			ctx->stat_const_jumbo96++;
			ctx->stat_const_jumbo96ph++;
		}
	}
	

	*ropw1=opw1;
	*ropw2=opw2;
	*ropw3=opw3;
	return(opw1>0);
}

int BGBCC_JX2X3_TryEmitOpImmReg(
	BGBCC_JX2_Context *ctx, int nmid, s64 imm, int rn)
{
	s64 opw1, opw2, opw3;

	if(!(ctx->emit_riscv&0x11) || !(ctx->emit_riscv&0x22))
		return(0);

	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	if(	(nmid==BGBCC_SH_NMID_CMPQEQ) ||
		(nmid==BGBCC_SH_NMID_CMPQGT) ||
		(nmid==BGBCC_SH_NMID_CMPQNE) ||
		(nmid==BGBCC_SH_NMID_CMPQGE))
	{
		if(!(ctx->has_jcmp&16))
			return(0);
		return(BGBCC_JX2X3_TryEmitOpRegImmReg(
			ctx, nmid, rn, imm, BGBCC_SH_REG_RQ0));
	}

	if(	(nmid==BGBCC_SH_NMID_CMPEQ) ||
		(nmid==BGBCC_SH_NMID_CMPGT) ||
		(nmid==BGBCC_SH_NMID_CMPGE))
	{
		if(!(ctx->has_jcmp&16))
			return(0);
		return(BGBCC_JX2X3_TryEmitOpRegImmReg(
			ctx, nmid, rn, imm, BGBCC_SH_REG_RQ0));
	}
	
	opw1=-1;
	opw2=-1;
	opw3=-1;

	if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
		return(0);

//	if(BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))
//		return(0);

	switch(nmid)
	{
	case BGBCC_SH_NMID_MOV:
	
		BGBCC_JX2X3_TryEncodeLoadImmReg(ctx, imm, rn, &opw1, &opw2, &opw3);
		break;

#if 0
		if((imm&0xFFFF)==imm)
		{
			opw1=0x00000012U|((rn&63)<<6)|((imm&0xFFFF)<<16);
			break;
		}
		if((imm|(~0xFFFF))==imm)
		{
			opw1=0x00001012U|((rn&63)<<6)|((imm&0xFFFF)<<16);
			break;
		}
	
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Imm10u(ctx, 0x0000000AU,
			BGBCC_SH_REG_RQ0, imm, rn, &opw1, &opw2)>0)
				break;
//		opw1=0x0000001EU|(imm&0xFFFF0000U);
//		opw2=0x00003012U|((rn&63)<<6)|((imm&0xFFFF)<<16);
		break;
#endif
	case BGBCC_SH_NMID_LDSH16:
		if(((u16)imm)==imm)
		{
			opw1=0x00003012U|((rn&63)<<6)|((imm&0xFFFF)<<16);
			break;
		}
		break;
	case BGBCC_SH_NMID_LDSH32:
//		break;

		if(((u32)imm)==imm)
		{
			opw1=0x0000001EU|(imm&0xFFFF0000U);
			opw2=0x00003012U|((rn&63)<<6)|((imm&0xFFFF)<<16);
			break;
		}
		break;
	}

	if(opw1>0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2X3_UpdateStat2RI(ctx, nmid, imm);

		BGBCC_JX2DA_EmitOpImmReg(ctx, nmid, imm, rn);

		BGBCC_JX2X3_CheckRepack3(ctx, &opw1, &opw2, &opw3);

		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		if(opw3>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw3);
		return(1);
	}

	return(0);
}

int BGBCC_JX2X3_TryEmitOpRegImmReg(
	BGBCC_JX2_Context *ctx, int nmid, int rm, s64 imm, int rn)
{
	s64 opw1, opw2, opw3;
	int isimm10u, isimm10n, isimm10s, isimm8s, isimm6u, isimm6s;
	int isimm16s, isimm17s;
	int isimm12rv, isimm9u;

	if(!(ctx->emit_riscv&0x11) || !(ctx->emit_riscv&0x22))
		return(0);
	
	rm=BGBCC_JX2RV_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	opw1=-1;
	opw2=-1;
	opw3=-1;

	if(	!BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) ||
		!BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
			return(0);

	if(rm==rn)
	{
		if(	(nmid==BGBCC_SH_NMID_LDSH16) ||
			(nmid==BGBCC_SH_NMID_LDSH32))
		{
			return(BGBCC_JX2X3_TryEmitOpImmReg(ctx, nmid, imm, rn));
		}
	}

	isimm9u=((imm&511)==imm);

#if 1
	isimm12rv=(((s32)(imm<<20))>>20)==imm;
	if(	BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
		BGBCC_JX2RV_CheckRegIsGPR(ctx, rn) &&
		isimm12rv && !isimm9u &&
		!(ctx->op_is_wex2&4))
	{
		/* Check if we should reject and fall back to RV op */
		if(	(nmid==BGBCC_SH_NMID_ADD) ||
			(nmid==BGBCC_SH_NMID_SUB) ||
			(nmid==BGBCC_SH_NMID_AND) ||
			(nmid==BGBCC_SH_NMID_OR) ||
			(nmid==BGBCC_SH_NMID_XOR) ||
			(nmid==BGBCC_SH_NMID_ADDSL) ||
			(nmid==BGBCC_SH_NMID_SUBSL) ||
			0)
		{
			return(0);
		}
	}
#endif

	if(nmid==BGBCC_SH_NMID_SUB)
		{ imm=-imm; nmid=BGBCC_SH_NMID_ADD; }
	if(nmid==BGBCC_SH_NMID_SUBSL)
		{ imm=-imm; nmid=BGBCC_SH_NMID_ADDSL; }
	if(nmid==BGBCC_SH_NMID_SUBUL)
		{ imm=-imm; nmid=BGBCC_SH_NMID_ADDUL; }

	if(nmid==BGBCC_SH_NMID_SHAR)
		{ imm=-imm; nmid=BGBCC_SH_NMID_SHAD; }
	if(nmid==BGBCC_SH_NMID_SHARQ)
		{ imm=-imm; nmid=BGBCC_SH_NMID_SHADQ; }

	if(nmid==BGBCC_SH_NMID_SHLR)
		{ imm=-imm; nmid=BGBCC_SH_NMID_SHLD; }
	if(nmid==BGBCC_SH_NMID_SHLRQ)
		{ imm=-imm; nmid=BGBCC_SH_NMID_SHLDQ; }

	if((nmid==BGBCC_SH_NMID_CMPQGE) || (nmid==BGBCC_SH_NMID_CMPGE))
		{ imm=imm-1; nmid=BGBCC_SH_NMID_CMPQGT; }
	if(nmid==BGBCC_SH_NMID_CMPQLE)
		{ imm=imm+1; nmid=BGBCC_SH_NMID_CMPQLT; }

	if((nmid==BGBCC_SH_NMID_CMPQHS) || (nmid==BGBCC_SH_NMID_CMPHS))
		{ imm=imm-1; nmid=BGBCC_SH_NMID_CMPQHI; }

	isimm10u=0;
	isimm10n=0;
	isimm10s=0;

	isimm8s=0;
	isimm6u=0;
	isimm6s=0;
	isimm16s=0;
	isimm17s=0;
	
	if((imm&1023)==imm)		isimm10u=1;
	if((imm|(~1023))==imm)	isimm10n=1;
	if((((s32)(imm<<22))>>22)==imm)
		isimm10s=1;

	if((((s32)(imm<<24))>>24)==imm)
		isimm8s=1;

	if((imm&63)==imm)		isimm6u=1;
	if((((s32)(imm<<26))>>26)==imm)
		isimm6s=1;

	if(((s16)imm)==imm)
		isimm16s=1;
	if((((s32)(imm<<15))>>15)==imm)
		isimm17s=1;


	switch(nmid)
	{
	case BGBCC_SH_NMID_ADD:
		if(isimm10u)
		{
			opw1=0x0000000AU|((imm&1023)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(isimm10n)
		{
			opw1=0x0000100AU|((imm&1023)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}

		if(rm==rn)
		{
			if(isimm16s)
			{
				opw1=0x00002012U|((rn&63)<<6)|((imm&0xFFFF)<<16);
				break;
			}
		}
		
		if((rm&63)==0)
		{
			if(isimm16s)
			{
				if(imm>=0)
				{
					opw1=0x00000012U|((rn&63)<<6)|((imm&0xFFFF)<<16);
					break;
				}else
				{
					opw1=0x00001012U|((rn&63)<<6)|((imm&0xFFFF)<<16);
					break;
				}
			}
		}

		if(BGBCC_JX2X3_CheckEncodeRIRJ_Imm10u(ctx, 0x0000000AU,
			rm, imm, rn, &opw1, &opw2)>0)
				break;
		break;

	case BGBCC_SH_NMID_DMULS:
	case BGBCC_SH_NMID_MULL:
		opw1=0x0000200AU|((imm&1023)<<22)|((rm&63)<<16)|((rn&63)<<6);
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Imm10u(ctx, 0x0000200AU,
			rm, imm, rn, &opw1, &opw2)>0)
				break;
		break;
	case BGBCC_SH_NMID_DMULU:
		opw1=0x0000300AU|((imm&1023)<<22)|((rm&63)<<16)|((rn&63)<<6);
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Imm10u(ctx, 0x0000300AU,
			rm, imm, rn, &opw1, &opw2)>0)
				break;
		break;

	case BGBCC_SH_NMID_ADDSL:
		if(isimm10u)
		{
			opw1=0x0000300AU|((imm&1023)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(isimm10n)
		{
			opw1=0x0000400AU|((imm&1023)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Imm10u(ctx,
			0x0000300AU, rm, imm, rn, &opw1, &opw2)>0)
				break;
		break;

	case BGBCC_SH_NMID_ADDUL:
		if(isimm10u)
		{
			opw1=0x0000302AU|((imm&1023)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(isimm10n)
		{
			opw1=0x0000402AU|((imm&1023)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Imm10u(ctx,
			0x0000302AU, rm, imm, rn, &opw1, &opw2)>0)
				break;
		break;

	case BGBCC_SH_NMID_AND:
		if(imm==0xFFFF)
		{
			opw1=0xC2401022U|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
	
		if(isimm10s)
		{
			opw1=0x0000500AU|((imm&1023)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Imm10s(ctx,
			0x0000500AU, rm, imm, rn, &opw1, &opw2)>0)
				break;
		break;
	case BGBCC_SH_NMID_RSUB:
		if(isimm10s)
		{
			opw1=0x0000502AU|((imm&1023)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Imm10s(ctx,
			0x0000502AU, rm, imm, rn, &opw1, &opw2)>0)
				break;
		break;
	case BGBCC_SH_NMID_OR:
		if(isimm10u)
		{
			opw1=0x0000600AU|((imm&1023)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Imm10u(ctx,
			0x0000600AU, rm, imm, rn, &opw1, &opw2)>0)
				break;
		break;
	case BGBCC_SH_NMID_SHADX:
		if(isimm8s)
		{
			opw1=0x0000602AU|((imm&255)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		break;
	case BGBCC_SH_NMID_XOR:
		if(isimm10u)
		{
			opw1=0x0000700AU|((imm&1023)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Imm10u(ctx,
			0x0000700AU, rm, imm, rn, &opw1, &opw2)>0)
				break;
		break;
	case BGBCC_SH_NMID_SHLDX:
		if(isimm8s)
		{
			opw1=0x0000702AU|((imm&255)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		break;

	case BGBCC_SH_NMID_SHAD:
		if(isimm8s)
		{
			opw1=0x0000800AU|((imm&255)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		break;
	case BGBCC_SH_NMID_SHADQ:
		if(isimm8s)
		{
			opw1=0x0000802AU|((imm&255)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		break;

	case BGBCC_SH_NMID_PSHUFB:
		if(isimm8s)
		{
			opw1=0x4000800AU|((imm&255)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		break;
	case BGBCC_SH_NMID_PSHUFW:
		if(isimm8s)
		{
			opw1=0x4000802AU|((imm&255)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		break;

	case BGBCC_SH_NMID_SHLD:
		if(isimm8s)
		{
			opw1=0x0000900AU|((imm&255)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		break;
	case BGBCC_SH_NMID_SHLDQ:
		if(isimm8s)
		{
			opw1=0x0000902AU|((imm&255)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		break;

	case BGBCC_SH_NMID_MOVTT:
		if(isimm6u)
		{
			opw1=0x10003022U|((imm&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		break;
	case BGBCC_SH_NMID_MULSW:
		if(isimm6u)
		{
			opw1=0xE0005022U|((imm&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		break;
	case BGBCC_SH_NMID_MULUW:
		if(isimm6u)
		{
			opw1=0xF0005022U|((imm&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		break;
	case BGBCC_SH_NMID_MACSL:
		if(isimm6u)
		{
			opw1=0x00006022U|((imm&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		break;
	case BGBCC_SH_NMID_MACUL:
		if(isimm6u)
		{
			opw1=0x10006022U|((imm&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		break;
	case BGBCC_SH_NMID_DMACSL:
		if(isimm6u)
		{
			opw1=0x20006022U|((imm&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		break;
	case BGBCC_SH_NMID_DMACUL:
		if(isimm6u)
		{
			opw1=0x30006022U|((imm&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		break;

	case BGBCC_SH_NMID_CMPQEQ:
	case BGBCC_SH_NMID_CMPEQ:
		if(isimm6s)
		{
			opw1=0x00009022U|((imm&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Imm6s(ctx,
			0x00009022U, rm, imm, rn, &opw1, &opw2)>0)
				break;
		break;
	case BGBCC_SH_NMID_CMPQGT:
	case BGBCC_SH_NMID_CMPGT:
		if(isimm6s)
		{
			opw1=0x10009022U|((imm&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Imm6s(ctx,
			0x10009022U, rm, imm, rn, &opw1, &opw2)>0)
				break;
		break;
	case BGBCC_SH_NMID_CMPQNE:
		if(isimm6s)
		{
			opw1=0x20009022U|((imm&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Imm6s(ctx,
			0x20009022U, rm, imm, rn, &opw1, &opw2)>0)
				break;
		break;
	case BGBCC_SH_NMID_CMPQLT:
		if(isimm6s)
		{
			opw1=0x30009022U|((imm&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Imm6s(ctx,
			0x30009022U, rm, imm, rn, &opw1, &opw2)>0)
				break;
		break;
	}

	if(opw1>0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2X3_UpdateStat3RI(ctx, nmid, imm);

		BGBCC_JX2DA_EmitOpRegImmReg(ctx, nmid, rm, imm, rn);

		BGBCC_JX2X3_CheckRepack3(ctx, &opw1, &opw2, &opw3);

		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		if(opw3>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw3);
		return(1);
	}

	return(0);
}

int BGBCC_JX2X3_TryEmitOpLdReg2Reg(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int ro, int rn)
{
	s64 opw1, opw2, opw3;

	if(!(ctx->emit_riscv&0x11) || !(ctx->emit_riscv&0x22))
		return(0);

	rm=BGBCC_JX2RV_NormalizeReg(ctx, rm);
	ro=BGBCC_JX2RV_NormalizeReg(ctx, ro);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	opw1=-1;
	opw2=-1;
	opw3=-1;

	if(	!BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) ||
		!BGBCC_JX2_EmitCheckRegExtGPR(ctx, ro) ||
		!BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
			return(0);

	switch(nmid)
	{
	case BGBCC_SH_NMID_LEAB:
		opw1=0x40000022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_LEAW:
		opw1=0x50000022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_LEAL:
		opw1=0x60000022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_LEAQ:
		opw1=0x70000022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;

	case BGBCC_SH_NMID_MOVB:
		opw1=0xC0000002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_MOVUB:
		opw1=0xC0000022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_MOVW:
		opw1=0xD0000002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_MOVUW:
		opw1=0xD0000022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_MOVL:
		opw1=0xE0000002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_MOVUL:
		opw1=0xE0000022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	case BGBCC_SH_NMID_MOVQ:
		opw1=0xF0000002U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;

	case BGBCC_SH_NMID_LDTEX:
		opw1=0xB0000022U|((ro&63)<<22)|((rm&63)<<16)|((rn&63)<<6);
		break;
	}

	if(opw1>0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpLdReg2Reg(ctx, nmid, rm, ro, rn);

		BGBCC_JX2X3_CheckRepack3(ctx, &opw1, &opw2, &opw3);

		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		if(opw3>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw3);
		return(1);
	}

	return(0);
}

int BGBCC_JX2X3_TryEmitOpRegStReg2(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int ro, int rn)
{
	s64 opw1, opw2, opw3;

	if(!(ctx->emit_riscv&0x11) || !(ctx->emit_riscv&0x22))
		return(0);

	rm=BGBCC_JX2RV_NormalizeReg(ctx, rm);
	ro=BGBCC_JX2RV_NormalizeReg(ctx, ro);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	opw1=-1;
	opw2=-1;
	opw3=-1;

	if(	!BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) ||
		!BGBCC_JX2_EmitCheckRegExtGPR(ctx, ro) ||
		!BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
			return(0);

	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
	case BGBCC_SH_NMID_MOVUB:
		opw1=0x40000002U|((ro&63)<<22)|((rn&63)<<16)|((rm&63)<<6);
		break;

	case BGBCC_SH_NMID_MOVW:
	case BGBCC_SH_NMID_MOVUW:
		opw1=0x50000002U|((ro&63)<<22)|((rn&63)<<16)|((rm&63)<<6);
		break;

	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVUL:
		opw1=0x60000002U|((ro&63)<<22)|((rn&63)<<16)|((rm&63)<<6);
		break;

	case BGBCC_SH_NMID_MOVQ:
		opw1=0x70000002U|((ro&63)<<22)|((rn&63)<<16)|((rm&63)<<6);
		break;
	}

	if(opw1>0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpRegStReg2(ctx, nmid, rm, ro, rn);

		BGBCC_JX2X3_CheckRepack3(ctx, &opw1, &opw2, &opw3);

		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		if(opw3>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw3);
		return(1);
	}

	return(0);
}

int BGBCC_JX2X3_TryEmitOpRegStRegDisp(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int rn, s64 disp)
{
	s64 opw1, opw2, opw3;
	int disp10b, disp10w, disp10l, disp10q, isdisp12rv, isdisp9u;

	if(!(ctx->emit_riscv&0x11) || !(ctx->emit_riscv&0x22))
		return(0);

	rm=BGBCC_JX2RV_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	opw1=-1;
	opw2=-1;
	opw3=-1;

	if(	!BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) ||
		!BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
			return(0);

	isdisp9u=(disp&0x1F8)==disp;

#if 1
	isdisp12rv=(((s32)(disp<<20))>>20)==disp;
	if(	BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
		BGBCC_JX2RV_CheckRegIsGPR(ctx, rn) &&
		isdisp12rv && !isdisp9u &&
		!(ctx->op_is_wex2&4))
	{
		/* Check if we should reject and fall back to RV op */
		return(0);
	}
#endif

	disp10b=((s32)((disp>>0)<<22))>>22;
	disp10w=((s32)((disp>>1)<<22))>>22;
	disp10l=((s32)((disp>>2)<<22))>>22;
	disp10q=((s32)((disp>>3)<<22))>>22;

	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
	case BGBCC_SH_NMID_MOVUB:
		if((disp10b<<0)==disp)
		{
			opw1=0x00000006U|((disp10b&1023)<<22)|((rn&63)<<16)|((rm&63)<<6);
			break;
		}
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx,
			0x00000006U, rn, disp, 0, rm, &opw1, &opw2)>0)
				break;
		break;

	case BGBCC_SH_NMID_MOVW:
	case BGBCC_SH_NMID_MOVUW:
		if((disp10w<<1)==disp)
		{
			opw1=0x00001006U|((disp10w&1023)<<22)|((rn&63)<<16)|((rm&63)<<6);
			break;
		}
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx,
			0x00001006U, rn, disp, 1, rm, &opw1, &opw2)>0)
				break;
		break;

	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVUL:
		if((disp10l<<2)==disp)
		{
			opw1=0x00002006U|((disp10l&1023)<<22)|((rn&63)<<16)|((rm&63)<<6);
			break;
		}
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx,
			0x00002006U, rn, disp, 2, rm, &opw1, &opw2)>0)
				break;
		break;

	case BGBCC_SH_NMID_MOVQ:
		if((disp10q<<3)==disp)
		{
			opw1=0x00003006U|((disp10q&1023)<<22)|((rn&63)<<16)|((rm&63)<<6);
			break;
		}
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx,
			0x00003006U, rn, disp, 3, rm, &opw1, &opw2)>0)
				break;
		break;

	case BGBCC_SH_NMID_MOVX2:
		if(rm&1)
			break;
		if((disp10q<<3)==disp)
		{
			opw1=0x00005026U|((disp10q&1023)<<22)|((rn&63)<<16)|((rm&63)<<6);
			break;
		}
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx,
			0x00005026U, rn, disp, 3, rm, &opw1, &opw2)>0)
				break;
		break;
	}

	if(opw1>0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpRegStRegDisp(ctx, nmid, rm, rn, disp);

		BGBCC_JX2X3_CheckRepack3(ctx, &opw1, &opw2, &opw3);

		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		if(opw3>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw3);
		return(1);
	}

	return(0);
}

int BGBCC_JX2X3_TryEmitOpLdRegDispReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, s64 disp, int rn)
{
	s64 opw1, opw2, opw3;
	int disp10b, disp10w, disp10l, disp10q, isdisp12rv, isdisp9u;

	if(!(ctx->emit_riscv&0x11) || !(ctx->emit_riscv&0x22))
		return(0);

	rm=BGBCC_JX2RV_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	opw1=-1;
	opw2=-1;
	opw3=-1;

	if(	!BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) ||
		!BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
			return(0);

	isdisp9u=(disp&0x1F8)==disp;

#if 1
	isdisp12rv=(((s32)(disp<<20))>>20)==disp;
	if(	BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
		BGBCC_JX2RV_CheckRegIsGPR(ctx, rn) &&
		isdisp12rv && !isdisp9u &&
		!(ctx->op_is_wex2&4))
	{
		/* Check if we should reject and fall back to RV op */
		return(0);
	}
#endif

	disp10b=((s32)((disp>>0)<<22))>>22;
	disp10w=((s32)((disp>>1)<<22))>>22;
	disp10l=((s32)((disp>>2)<<22))>>22;
	disp10q=((s32)((disp>>3)<<22))>>22;

	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if((disp10b<<0)==disp)
		{
			opw1=0x00008006U|((disp10b&1023)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx,
			0x00008006U, rm, disp, 0, rn, &opw1, &opw2)>0)
				break;
		break;
	case BGBCC_SH_NMID_MOVUB:
		if((disp10b<<0)==disp)
		{
			opw1=0x00008026U|((disp10b&1023)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx,
			0x00008026U, rm, disp, 0, rn, &opw1, &opw2)>0)
				break;
		break;
	case BGBCC_SH_NMID_MOVW:
		if((disp10w<<1)==disp)
		{
			opw1=0x00009006U|((disp10w&1023)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx,
			0x00009006U, rm, disp, 1, rn, &opw1, &opw2)>0)
				break;
		break;
	case BGBCC_SH_NMID_MOVUW:
		if((disp10w<<1)==disp)
		{
			opw1=0x00009026U|((disp10w&1023)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx,
			0x00009026U, rm, disp, 1, rn, &opw1, &opw2)>0)
				break;
		break;
	case BGBCC_SH_NMID_MOVL:
		if((disp10l<<2)==disp)
		{
			opw1=0x0000A006U|((disp10l&1023)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx,
			0x0000A006U, rm, disp, 2, rn, &opw1, &opw2)>0)
				break;
		break;
	case BGBCC_SH_NMID_MOVUL:
		if((disp10l<<2)==disp)
		{
			opw1=0x0000A026U|((disp10l&1023)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx,
			0x0000A026U, rm, disp, 2, rn, &opw1, &opw2)>0)
				break;
		break;
	case BGBCC_SH_NMID_MOVQ:
		if((disp10q<<3)==disp)
		{
			opw1=0x0000B006U|((disp10q&1023)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx,
			0x0000B006U, rm, disp, 3, rn, &opw1, &opw2)>0)
				break;
		break;

	case BGBCC_SH_NMID_MOVX2:
		if(rn&1)
			break;
		if((disp10q<<3)==disp)
		{
			opw1=0x00007026U|((disp10q&1023)<<22)|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx,
			0x00007026U, rm, disp, 3, rn, &opw1, &opw2)>0)
				break;
		break;
	}

	if(opw1>0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpLdRegDispReg(ctx, nmid, rm, disp, rn);

		BGBCC_JX2X3_CheckRepack3(ctx, &opw1, &opw2, &opw3);

		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		if(opw3>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw3);
		return(1);
	}

	return(0);
}

int BGBCC_JX2X3_TryEmitOpRegRegLbl(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn, int lbl)
{
	s64 opw1, opw2, opw3, opw4;
	int odr, ex, ex2, nowxi, exw, rlty, rlty2, rlty3, rt, isrvr;
	int i, j, k;

	if(!(ctx->emit_riscv&0x11) || !(ctx->emit_riscv&0x22))
		return(0);

//	return(0);

	rm=BGBCC_JX2RV_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	opw1=-1;	opw2=-1;	opw3=-1;	opw4=-1;
	rlty=-1;	rlty2=-1;	rlty3=-1;

	if(	!BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) ||
		!BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
			return(0);

	isrvr=0;
	if(	BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
		BGBCC_JX2RV_CheckRegIsGPR(ctx, rn) )
			isrvr=1;

	switch(nmid)
	{
	case BGBCC_SH_NMID_BREQ:
	case BGBCC_SH_NMID_BREQL:
		if(isrvr && BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
			opw1=0x00000063U|((rn&31)<<15)|((rm&31)<<20);
			rlty=BGBCC_SH_RLC_RELW12_RVI;
			break;
		}
		if(BGBCC_JX2_EmitCheckAutoLabelNear10(ctx, lbl, nmid))
		{
			rlty=BGBCC_SH_RLC_RELW10_XG3;
			opw1=0x0000F006U|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(1)
		{
			opw1=0x0080F026U|((rm&63)<<16)|((rn&63)<<6);
			opw2=0x0000006FU;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}
		break;
	case BGBCC_SH_NMID_BRNE:
	case BGBCC_SH_NMID_BRNEL:
		if(isrvr && BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
			opw1=0x00001063U|((rn&31)<<15)|((rm&31)<<20);
			rlty=BGBCC_SH_RLC_RELW12_RVI;
			break;
		}
		if(BGBCC_JX2_EmitCheckAutoLabelNear10(ctx, lbl, nmid))
		{
			rlty=BGBCC_SH_RLC_RELW10_XG3;
			opw1=0x0000F026U|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(1)
		{
			opw1=0x0080F006U|((rm&63)<<16)|((rn&63)<<6);
			opw2=0x0000006FU;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}
		break;
	case BGBCC_SH_NMID_BRLT:
	case BGBCC_SH_NMID_BRLTL:
		if(isrvr && BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
			opw1=0x00004063U|((rn&31)<<15)|((rm&31)<<20);
			rlty=BGBCC_SH_RLC_RELW12_RVI;
			break;
		}
		if(BGBCC_JX2_EmitCheckAutoLabelNear10(ctx, lbl, nmid))
		{
			rlty=BGBCC_SH_RLC_RELW10_XG3;
			opw1=0x0000D006U|((rn&63)<<16)|((rm&63)<<6);
			break;
		}
		if(1)
		{
			opw1=0x0080D026U|((rn&63)<<16)|((rm&63)<<6);
			opw2=0x0000006FU;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}
		break;
	case BGBCC_SH_NMID_BRGT:
	case BGBCC_SH_NMID_BRGTL:
		if(isrvr && BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
			opw1=0x00004063U|((rm&31)<<15)|((rn&31)<<20);
			rlty=BGBCC_SH_RLC_RELW12_RVI;
			break;
		}
		if(BGBCC_JX2_EmitCheckAutoLabelNear10(ctx, lbl, nmid))
		{
			rlty=BGBCC_SH_RLC_RELW10_XG3;
			opw1=0x0000D006U|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(1)
		{
			opw1=0x0080D026U|((rm&63)<<16)|((rn&63)<<6);
			opw2=0x0000006FU;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}
		break;

	case BGBCC_SH_NMID_BRLE:
	case BGBCC_SH_NMID_BRLEL:
		if(isrvr && BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
			opw1=0x00005063U|((rm&31)<<15)|((rn&31)<<20);
			rlty=BGBCC_SH_RLC_RELW12_RVI;
			break;
		}
		if(BGBCC_JX2_EmitCheckAutoLabelNear10(ctx, lbl, nmid))
		{
			rlty=BGBCC_SH_RLC_RELW10_XG3;
			opw1=0x0000D026U|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(1)
		{
			opw1=0x0080D006U|((rm&63)<<16)|((rn&63)<<6);
			opw2=0x0000006FU;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}
		break;
	case BGBCC_SH_NMID_BRGE:
	case BGBCC_SH_NMID_BRGEL:
		if(isrvr && BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
			opw1=0x00005063U|((rn&31)<<15)|((rm&31)<<20);
			rlty=BGBCC_SH_RLC_RELW12_RVI;
			break;
		}
		if(BGBCC_JX2_EmitCheckAutoLabelNear10(ctx, lbl, nmid))
		{
			rlty=BGBCC_SH_RLC_RELW10_XG3;
			opw1=0x0000D026U|((rn&63)<<16)|((rm&63)<<6);
			break;
		}
		if(1)
		{
			opw1=0x0080D006U|((rn&63)<<16)|((rm&63)<<6);
			opw2=0x0000006FU;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}
		break;


	case BGBCC_SH_NMID_BRLTU:
	case BGBCC_SH_NMID_BRLTUL:
		if(isrvr && BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
			opw1=0x00006063U|((rn&31)<<15)|((rm&31)<<20);
			rlty=BGBCC_SH_RLC_RELW12_RVI;
			break;
		}
		if(BGBCC_JX2_EmitCheckAutoLabelNear10(ctx, lbl, nmid))
		{
			rlty=BGBCC_SH_RLC_RELW10_XG3;
			opw1=0x0000E006U|((rn&63)<<16)|((rm&63)<<6);
			break;
		}
		if(1)
		{
			opw1=0x0080E026U|((rn&63)<<16)|((rm&63)<<6);
			opw2=0x0000006FU;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}
		break;
	case BGBCC_SH_NMID_BRGTU:
	case BGBCC_SH_NMID_BRGTUL:
		if(isrvr && BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
			opw1=0x00006063U|((rm&31)<<15)|((rn&31)<<20);
			rlty=BGBCC_SH_RLC_RELW12_RVI;
			break;
		}
		if(BGBCC_JX2_EmitCheckAutoLabelNear10(ctx, lbl, nmid))
		{
			rlty=BGBCC_SH_RLC_RELW10_XG3;
			opw1=0x0000E006U|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(1)
		{
			opw1=0x0080E026U|((rm&63)<<16)|((rn&63)<<6);
			opw2=0x0000006FU;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}
		break;
	case BGBCC_SH_NMID_BRLEU:
	case BGBCC_SH_NMID_BRLEUL:
		if(isrvr && BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
			opw1=0x00007063U|((rm&31)<<15)|((rn&31)<<20);
			rlty=BGBCC_SH_RLC_RELW12_RVI;
			break;
		}
		if(BGBCC_JX2_EmitCheckAutoLabelNear10(ctx, lbl, nmid))
		{
			rlty=BGBCC_SH_RLC_RELW10_XG3;
			opw1=0x0000E026U|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(1)
		{
			opw1=0x0080E006U|((rm&63)<<16)|((rn&63)<<6);
			opw2=0x0000006FU;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}
		break;
	case BGBCC_SH_NMID_BRGEU:
	case BGBCC_SH_NMID_BRGEUL:
		if(isrvr && BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
			opw1=0x00007063U|((rn&31)<<15)|((rm&31)<<20);
			rlty=BGBCC_SH_RLC_RELW12_RVI;
			break;
		}
		if(BGBCC_JX2_EmitCheckAutoLabelNear10(ctx, lbl, nmid))
		{
			rlty=BGBCC_SH_RLC_RELW10_XG3;
			opw1=0x0000E026U|((rn&63)<<16)|((rm&63)<<6);
			break;
		}
		if(1)
		{
			opw1=0x0080E006U|((rn&63)<<16)|((rm&63)<<6);
			opw2=0x0000006FU;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}
		break;

	case BGBCC_SH_NMID_BRTSTQ:
	case BGBCC_SH_NMID_BRTSTL:
//	case BGBCC_SH_NMID_BRTSTNQ:
//	case BGBCC_SH_NMID_BRTSTNL:
		if(BGBCC_JX2_EmitCheckAutoLabelNear10(ctx, lbl, nmid))
		{
			rlty=BGBCC_SH_RLC_RELW10_XG3;
			opw1=0x0000C006U|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(1)
		{
			opw1=0x0080C026U|((rm&63)<<16)|((rn&63)<<6);
			opw2=0x0000006FU;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}
		break;
	case BGBCC_SH_NMID_BRTSTNQ:
	case BGBCC_SH_NMID_BRTSTNL:
//	case BGBCC_SH_NMID_BRTSTQ:
//	case BGBCC_SH_NMID_BRTSTL:
		if(BGBCC_JX2_EmitCheckAutoLabelNear10(ctx, lbl, nmid))
		{
			rlty=BGBCC_SH_RLC_RELW10_XG3;
			opw1=0x0000C026U|((rm&63)<<16)|((rn&63)<<6);
			break;
		}
		if(1)
		{
			opw1=0x0080C006U|((rm&63)<<16)|((rn&63)<<6);
			opw2=0x0000006FU;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}
		break;
	}

	if(opw1>0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpRegRegLbl(ctx, nmid, rm, rn, lbl);

		if(rlty>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty); }

		BGBCC_JX2_EmitOpDWord(ctx, opw1);

		if(rlty2>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty2); }
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);

		if(rlty3>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty3); }
		if(opw3>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw3);

		if(opw4>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw4);
		return(1);
	}

	return(0);
}

int BGBCC_JX2X3_TryEmitOpImmRegLbl(BGBCC_JX2_Context *ctx,
	int nmid, s64 imm, int rn, int lbl)
{
	return(0);
}
