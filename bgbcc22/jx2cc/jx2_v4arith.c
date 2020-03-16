int BGBCC_JX2C_LoadVectorField64(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, int ofs,
	int csreg, int cdreg)
{
	int sreg2, dreg2;
	int ty, i;
	
	ty=type.val;

	if(ty==CCXL_TY_F)
	{
		i=0;
		switch(ofs)
		{
		case 0:
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_FLDCF, csreg, cdreg);
			i=1;
			break;
		case 4:
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_FLDCFH, csreg, cdreg);
			i=1;
			break;
		}
		if(i)return(i);
	}
	
	switch(ofs)
	{
	case 0:
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
		break;
				case 1:		case 2:		case 3:
	case 4:		case 5:		case 6:		case 7:
		BGBCC_JX2_EmitOpRegImmReg(sctx,
			BGBCC_SH_NMID_SHLDQ, csreg, -(ofs&7)*8, cdreg);
		break;
	default:
		break;
	}

	switch(ty)
	{
	case CCXL_TY_I:
		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_EXTSL, cdreg);
		break;
	case CCXL_TY_UI:
		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_EXTUL, cdreg);
		break;
	case CCXL_TY_L:
	case CCXL_TY_UL:
//		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_EXTSL, cdreg);
		break;

	case CCXL_TY_F:
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_FLDCF, cdreg, cdreg);
		break;

	case CCXL_TY_SB:
		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_EXTSB, cdreg);
		break;
	case CCXL_TY_UB:
		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_EXTSB, cdreg);
		break;
	case CCXL_TY_SS:
		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_EXTSW, cdreg);
		break;
	case CCXL_TY_US:
		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_EXTUW, cdreg);
		break;
	}
	
	return(1);
}

int BGBCC_JX2C_LoadVectorField128(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, int ofs,
	int csreg, int cdreg)
{
	int sreg2, dreg2;
	int ty;
	int i, j;
	
	if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, cdreg))
	{
		if(!ofs)
		{
			sreg2=BGBCC_SH_REG_RQ0+(csreg&31);
			dreg2=BGBCC_SH_REG_RQ0+(cdreg&31);

			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, sreg2+0, dreg2+0);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, sreg2+1, dreg2+1);
			return(1);
		}
		
		BGBCC_DBGBREAK
	}
	
	sreg2=BGBCC_SH_REG_RQ0+(csreg&31);

	ty=type.val;

	if(ty==CCXL_TY_F)
	{
		i=0;
		switch(ofs)
		{
		case 0:
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_FLDCF, sreg2+0, cdreg);
			i=1;
			break;
		case 4:
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_FLDCFH, csreg+0, cdreg);
			i=1;
			break;
		case 8:
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_FLDCF, sreg2+1, cdreg);
			i=1;
			break;
		case 12:
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_FLDCFH, csreg+1, cdreg);
			i=1;
			break;
		}
		if(i>0)return(i);
	}
	
	switch(ofs)
	{
	case 0:
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, sreg2+0, cdreg);
		break;
				case 1:		case 2:		case 3:
	case 4:		case 5:		case 6:		case 7:
		BGBCC_JX2_EmitOpRegImmReg(sctx,
			BGBCC_SH_NMID_SHLDQ, sreg2+0, -(ofs&7)*8, cdreg);
		break;
	case 8:
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, sreg2+1, cdreg);
		break;
				case 9:		case 10:	case 11:
	case 12:	case 13:	case 14:	case 15:
		BGBCC_JX2_EmitOpRegImmReg(sctx,
			BGBCC_SH_NMID_SHLDQ, sreg2+1, -(ofs&7)*8, cdreg);
		break;
	default:
		break;
	}

	ty=type.val;

	switch(ty)
	{
	case CCXL_TY_I:
		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_EXTSL, cdreg);
		break;
	case CCXL_TY_UI:
		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_EXTUL, cdreg);
		break;
	case CCXL_TY_L:
	case CCXL_TY_UL:
//		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_EXTSL, cdreg);
		break;

	case CCXL_TY_F:
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_FLDCF, cdreg, cdreg);
		break;

	case CCXL_TY_SB:
		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_EXTSB, cdreg);
		break;
	case CCXL_TY_UB:
		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_EXTSB, cdreg);
		break;
	case CCXL_TY_SS:
		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_EXTSW, cdreg);
		break;
	case CCXL_TY_US:
		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_EXTUW, cdreg);
		break;
	}
	
	return(1);
}


int BGBCC_JX2C_EmitBinaryVRegVRegVReg_Vec64F(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, flip;
	int i, j, k;
	
	nm1=-1; nm2=-1;
	flip=0;

	if(type.val==CCXL_TY_VEC4SW)
	{
		switch(opr)
		{
		case CCXL_BINOP_ADD:
			nm1=BGBCC_SH_NMID_PADDW;
			break;
		case CCXL_BINOP_SUB:
			nm1=BGBCC_SH_NMID_PSUBW;
			break;
		case CCXL_BINOP_MUL:
			nm1=BGBCC_SH_NMID_PMULSHW;
			break;
		case CCXL_BINOP_MOD:
			nm1=BGBCC_SH_NMID_PMULSLW;
			break;
		}
	}

	if(type.val==CCXL_TY_VEC4UW)
	{
		switch(opr)
		{
		case CCXL_BINOP_ADD:
			nm1=BGBCC_SH_NMID_PADDW;
			break;
		case CCXL_BINOP_SUB:
			nm1=BGBCC_SH_NMID_PSUBW;
			break;
		case CCXL_BINOP_MUL:
			nm1=BGBCC_SH_NMID_PMULUHW;
			break;
		case CCXL_BINOP_MOD:
			nm1=BGBCC_SH_NMID_PMULULW;
			break;
		}
	}

	if(type.val==CCXL_TY_VEC2F)
	{
		switch(opr)
		{
		case CCXL_BINOP_ADD:
			nm1=BGBCC_SH_NMID_PADDF;
			break;
		case CCXL_BINOP_SUB:
			nm1=BGBCC_SH_NMID_PSUBF;
			break;
		case CCXL_BINOP_MUL:
			nm1=BGBCC_SH_NMID_PMULF;
			break;
		}
	}

	if(type.val==CCXL_TY_FCOMPLEX)
	{
		switch(opr)
		{
		case CCXL_BINOP_ADD:
			nm1=BGBCC_SH_NMID_PADDF;
			break;
		case CCXL_BINOP_SUB:
			nm1=BGBCC_SH_NMID_PSUBF;
			break;
//		case CCXL_BINOP_MUL:
//			nm1=BGBCC_SH_NMID_PMULF;
//			break;
		}
	}

	if(nm1>=0)
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

		if(BGBCC_JX2_ProbeEmitOpRegRegReg(sctx,
			nm1, csreg, ctreg, cdreg)>0)
		{
			if(flip)
				BGBCC_JX2C_EmitOpRegRegReg(ctx, sctx,
					nm1, ctreg, csreg, cdreg);
			else
				BGBCC_JX2C_EmitOpRegRegReg(ctx, sctx,
					nm1, csreg, ctreg, cdreg);
		}else
		{
			if(flip)
			{
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_MOV, ctreg, cdreg);
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg, cdreg);
			}
			else
			{
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_MOV, csreg, cdreg);
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, cdreg);
			}
		}

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}
	
	s0=NULL;

	switch(type.val)
	{
	case CCXL_TY_VEC2F:
		switch(opr)
		{
			case CCXL_BINOP_ADD:	s0="__vnf_v2f_add"; break;
			case CCXL_BINOP_SUB:	s0="__vnf_v2f_sub"; break;
			case CCXL_BINOP_MUL:	s0="__vnf_v2f_mul"; break;
			case CCXL_BINOP_DIV:	s0="__vnf_v2f_div"; break;
			case CCXL_BINOP_MOD:	s0="__vnf_v2f_cross"; break;
			case CCXL_BINOP_XOR:	s0="__vnf_v2f_dot"; break;
		}
		break;
	case CCXL_TY_VEC4SW:
		switch(opr)
		{
			case CCXL_BINOP_ADD:	s0="__vnf_v4w_add"; break;
			case CCXL_BINOP_SUB:	s0="__vnf_v4w_sub"; break;
			case CCXL_BINOP_MUL:	s0="__vnf_v4w_mulsh"; break;
			case CCXL_BINOP_MOD:	s0="__vnf_v4w_mulsl"; break;
		}
		break;
	case CCXL_TY_VEC4UW:
		switch(opr)
		{
			case CCXL_BINOP_ADD:	s0="__vnf_v4w_add"; break;
			case CCXL_BINOP_SUB:	s0="__vnf_v4w_sub"; break;
			case CCXL_BINOP_MUL:	s0="__vnf_v4w_muluh"; break;
			case CCXL_BINOP_MOD:	s0="__vnf_v4w_mulul"; break;
		}
		break;
	case CCXL_TY_FCOMPLEX:
		switch(opr)
		{
			case CCXL_BINOP_ADD:	s0="__vnf_c2f_add"; break;
			case CCXL_BINOP_SUB:	s0="__vnf_c2f_sub"; break;
			case CCXL_BINOP_MUL:	s0="__vnf_c2f_mul"; break;
			case CCXL_BINOP_DIV:	s0="__vnf_c2f_div"; break;
//			case CCXL_BINOP_MOD:	s0="__vnf_c2f_cross"; break;
//			case CCXL_BINOP_XOR:	s0="__vnf_c2f_dot"; break;
		}
		break;
	}

#if 0
	switch(opr)
	{
	case CCXL_BINOP_ADD:	s0="__xli_add"; break;
	case CCXL_BINOP_SUB:	s0="__xli_sub"; break;
	case CCXL_BINOP_MUL:	s0="__xli_smul"; break;
	case CCXL_BINOP_DIV:	s0="__xli_sdiv"; break;
	case CCXL_BINOP_AND:	s0="__xli_and"; break;
	case CCXL_BINOP_OR:		s0="__xli_or"; break;
	case CCXL_BINOP_XOR:	s0="__xli_xor"; break;
	case CCXL_BINOP_SHL:	s0="__xli_shl"; break;
	case CCXL_BINOP_SHR:	s0="__xli_shr"; break;
	}
#endif

	if(s0)
	{
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_RQ5);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ4);
		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
			dreg, BGBCC_SH_REG_RQ2);
		return(1);
	}
	
	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_JX2C_EmitBinaryVRegVRegVReg_Vec128F(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, flip;
	int i, j, k;
	
	s0=NULL;

	nm1=-1; nm2=-1;
	flip=0;

	if(sctx->has_simdx2)
	{
		if(	(type.val==CCXL_TY_VEC3F)	||
			(type.val==CCXL_TY_VEC4F)	)
		{
			switch(opr)
			{
			case CCXL_BINOP_ADD:
				nm1=BGBCC_SH_NMID_PADDFX;
				break;
			case CCXL_BINOP_SUB:
				nm1=BGBCC_SH_NMID_PSUBFX;
				break;
			case CCXL_BINOP_MUL:
				nm1=BGBCC_SH_NMID_PMULFX;
				break;
			}
		}

		if(type.val==CCXL_TY_QUATF)
		{
			switch(opr)
			{
			case CCXL_BINOP_ADD:
				nm1=BGBCC_SH_NMID_PADDFX;
				break;
			case CCXL_BINOP_SUB:
				nm1=BGBCC_SH_NMID_PSUBFX;
				break;
//			case CCXL_BINOP_MUL:
//				nm1=BGBCC_SH_NMID_PMULFX;
//				break;
			}
		}
	}
	

	if(nm1>=0)
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

		if(BGBCC_JX2_ProbeEmitOpRegRegReg(sctx,
			nm1, csreg, ctreg, cdreg)>0)
		{
			if(flip)
				BGBCC_JX2C_EmitOpRegRegReg(ctx, sctx,
					nm1, ctreg, csreg, cdreg);
			else
				BGBCC_JX2C_EmitOpRegRegReg(ctx, sctx,
					nm1, csreg, ctreg, cdreg);
		}else
		{
			if(flip)
			{
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_MOV, ctreg, cdreg);
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg, cdreg);
			}
			else
			{
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_MOV, csreg, cdreg);
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, cdreg);
			}
		}

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	switch(type.val)
	{
	case CCXL_TY_VEC2D:
		switch(opr)
		{
			case CCXL_BINOP_ADD:	s0="__vnf_v2d_add"; break;
			case CCXL_BINOP_SUB:	s0="__vnf_v2d_sub"; break;
			case CCXL_BINOP_MUL:	s0="__vnf_v2d_mul"; break;
			case CCXL_BINOP_DIV:	s0="__vnf_v2d_div"; break;
			case CCXL_BINOP_MOD:	s0="__vnf_v2d_cross"; break;
			case CCXL_BINOP_XOR:	s0="__vnf_v2d_dot"; break;
		}
		break;

	case CCXL_TY_VEC3F:
		switch(opr)
		{
			case CCXL_BINOP_ADD:	s0="__vnf_v3f_add"; break;
			case CCXL_BINOP_SUB:	s0="__vnf_v3f_sub"; break;
			case CCXL_BINOP_MUL:	s0="__vnf_v3f_mul"; break;
			case CCXL_BINOP_DIV:	s0="__vnf_v3f_div"; break;
			case CCXL_BINOP_MOD:	s0="__vnf_v3f_cross"; break;
			case CCXL_BINOP_XOR:	s0="__vnf_v3f_dot"; break;
		}
		break;
	case CCXL_TY_VEC4F:
		switch(opr)
		{
			case CCXL_BINOP_ADD:	s0="__vnf_v4f_add"; break;
			case CCXL_BINOP_SUB:	s0="__vnf_v4f_sub"; break;
			case CCXL_BINOP_MUL:	s0="__vnf_v4f_mul"; break;
			case CCXL_BINOP_DIV:	s0="__vnf_v4f_div"; break;
			case CCXL_BINOP_MOD:	s0="__vnf_v4f_cross"; break;
			case CCXL_BINOP_XOR:	s0="__vnf_v4f_dot"; break;
		}
		break;

	case CCXL_TY_QUATF:
		switch(opr)
		{
			case CCXL_BINOP_ADD:	s0="__vnf_vqf_add"; break;
			case CCXL_BINOP_SUB:	s0="__vnf_vqf_sub"; break;
			case CCXL_BINOP_MUL:	s0="__vnf_vqf_mul"; break;
			case CCXL_BINOP_DIV:	s0="__vnf_vqf_div"; break;
			case CCXL_BINOP_MOD:	s0="__vnf_v4f_cross"; break;
			case CCXL_BINOP_XOR:	s0="__vnf_v4f_dot"; break;
		}
		break;
	case CCXL_TY_DCOMPLEX:
		switch(opr)
		{
			case CCXL_BINOP_ADD:	s0="__vnf_c2d_add"; break;
			case CCXL_BINOP_SUB:	s0="__vnf_c2d_sub"; break;
			case CCXL_BINOP_MUL:	s0="__vnf_c2d_mul"; break;
			case CCXL_BINOP_DIV:	s0="__vnf_c2d_div"; break;
//			case CCXL_BINOP_MOD:	s0="__vnf_v4f_cross"; break;
//			case CCXL_BINOP_XOR:	s0="__vnf_v4f_dot"; break;
		}
		break;

	case CCXL_TY_VEC4SI:
		switch(opr)
		{
			case CCXL_BINOP_ADD:	s0="__vnf_v4i_add"; break;
			case CCXL_BINOP_SUB:	s0="__vnf_v4i_sub"; break;
			case CCXL_BINOP_MUL:	s0="__vnf_v4i_mulsh"; break;
			case CCXL_BINOP_DIV:	s0="__vnf_v4i_div"; break;
			case CCXL_BINOP_MOD:	s0="__vnf_v4i_mulsl"; break;
			case CCXL_BINOP_AND:	s0="__vnf_v4i_and"; break;
			case CCXL_BINOP_OR:		s0="__vnf_v4i_or"; break;
			case CCXL_BINOP_XOR:	s0="__vnf_v4i_xor"; break;
		}
		break;
	case CCXL_TY_VEC4UI:
		switch(opr)
		{
			case CCXL_BINOP_ADD:	s0="__vnf_v4i_add"; break;
			case CCXL_BINOP_SUB:	s0="__vnf_v4i_sub"; break;
			case CCXL_BINOP_MUL:	s0="__vnf_v4i_muluh"; break;
			case CCXL_BINOP_DIV:	s0="__vnf_v4i_div"; break;
			case CCXL_BINOP_MOD:	s0="__vnf_v4i_mulul"; break;
			case CCXL_BINOP_AND:	s0="__vnf_v4i_and"; break;
			case CCXL_BINOP_OR:		s0="__vnf_v4i_or"; break;
			case CCXL_BINOP_XOR:	s0="__vnf_v4i_xor"; break;
		}
		break;
	}

#if 0
	switch(opr)
	{
	case CCXL_BINOP_ADD:	s0="__xli_add"; break;
	case CCXL_BINOP_SUB:	s0="__xli_sub"; break;
	case CCXL_BINOP_MUL:	s0="__xli_smul"; break;
	case CCXL_BINOP_DIV:	s0="__xli_sdiv"; break;
	case CCXL_BINOP_AND:	s0="__xli_and"; break;
	case CCXL_BINOP_OR:		s0="__xli_or"; break;
	case CCXL_BINOP_XOR:	s0="__xli_xor"; break;
	case CCXL_BINOP_SHL:	s0="__xli_shl"; break;
	case CCXL_BINOP_SHR:	s0="__xli_shr"; break;
	}
#endif

	if(s0)
	{
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_LR6);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
			dreg, BGBCC_SH_REG_LR2);
		return(1);
	}
	
	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_JX2C_EmitBinaryVRegVRegVReg_Vec(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	char *fcn;
	int i;
	
	if(BGBCC_CCXL_TypeVec64P(ctx, type))
	{
		i=BGBCC_JX2C_EmitBinaryVRegVRegVReg_Vec64F(ctx, sctx, type,
			dreg, opr, sreg, treg);
		return(i);
	}
	
	if(BGBCC_CCXL_TypeVec128P(ctx, type))
	{
		i=BGBCC_JX2C_EmitBinaryVRegVRegVReg_Vec128F(ctx, sctx, type,
			dreg, opr, sreg, treg);
		return(i);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
