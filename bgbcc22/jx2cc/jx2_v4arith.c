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
	int nm1, nm2;
	int i, j, k;
	
	s0=NULL;

	switch(type.val)
	{
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
