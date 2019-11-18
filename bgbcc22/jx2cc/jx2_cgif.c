BGBCC_CCXL_BackendFuncs_vt bgbgc_jx2_vt;
static int bgbgc_jx2_init=0;

	
int BGBCC_JX2C_InitIface()
{
	if(bgbgc_jx2_init)
		return(0);
	bgbgc_jx2_init=1;

	bgbgc_jx2_vt.tgtname="JX2";
	
	bgbgc_jx2_vt.SetupContextForArch=	BGBCC_JX2C_SetupContextForArch;
	bgbgc_jx2_vt.FlattenImage=			BGBCC_JX2C_FlattenImage;
	bgbgc_jx2_vt.EndFunction=			BGBCC_JX2C_EndFunction;
	bgbgc_jx2_vt.LoadBufferDLL=			BGBCC_JX2C_LoadBufferDLL;

	BGBCC_CCXL_RegisterBackend(&bgbgc_jx2_vt);

	return(1);
}

ccxl_status BGBCC_JX2C_LoadBufferDLL(BGBCC_TransState *ctx,
	byte *buf, int sz)
{
	BGBCC_JX2_Context *sctx;
	BGBCC_CCXL_RegisterInfo *obj;
	BGBCC_CCXL_LiteralInfo *litobj;
	int l0;
	u32 addr;
	int i, j, k;

	sctx=ctx->uctx;

	if((buf[0]=='M') && (buf[1]=='Z'))
	{
		i=BGBCC_JX2C_CoffLoadBufferDLL(ctx, sctx, buf, sz);
		return(i);
	}
	
	return(-1);
}

ccxl_status BGBCC_JX2C_SetupContextForArch(BGBCC_TransState *ctx)
{
	BGBCC_JX2_Context *shctx;

	if(ctx->arch!=BGBCC_ARCH_BJX2)
		return(CCXL_STATUS_ERR_UNSUPPORTED);

	shctx=BGBCC_JX2_AllocContext();

	ctx->arch_sizeof_long=8;
	ctx->arch_sizeof_ptr=8;
	ctx->arch_sizeof_valist=128;
	ctx->arch_align_max=8;
//	ctx->arch_align_objmin=4;
	ctx->arch_align_objmin=1;
	ctx->uctx=shctx;

	shctx->tctx=ctx;
	shctx->is_le=1;
	shctx->is_addr64=1;
	shctx->has_bjx1egpr=1;
	shctx->is_betav=1;
	shctx->has_bjx1r3mov=0;

//	shctx->use_memmdl=BGBCC_MEMMDL_TINY16;
//	shctx->use_memmdl=BGBCC_MEMMDL_MEDIUM24;

	shctx->has_shad=1;
	shctx->has_movi20=0;
	shctx->has_bjx1jmp=0;
	shctx->has_movi20=0;
	shctx->has_misalgn=1;
	shctx->has_bjx1mov=1;
	shctx->has_bjx1ari=1;
//	shctx->has_bjx1breq=0;
	shctx->has_bjx1breq=1;
	shctx->has_addsl=1;
	
//	shctx->no_fpu=1;
	shctx->no_ext32=0;
//	shctx->fpu_soft=1;
	shctx->fpu_lite=0;
	shctx->fpu_gfp=0;
	shctx->is_addr_x32=0;
	shctx->use_padcross=0;
	shctx->no_ops48=0;

	shctx->is_pbo=0;
//	shctx->use_wexmd=1;
	shctx->use_wexmd=0;

//	shctx->fpu_gfp=1;
//	shctx->is_pbo=1;

	if(ctx->optmode==BGBCC_OPT_SIZE)
		shctx->use_wexmd=0;

	if(BGBCC_CCXL_CheckForOptStr(ctx, "nowex"))
		shctx->use_wexmd=0;
	if(BGBCC_CCXL_CheckForOptStr(ctx, "wex2w"))
		shctx->use_wexmd=1;
	if(BGBCC_CCXL_CheckForOptStr(ctx, "wex3w"))
	{
		shctx->use_wexmd=2;
//		shctx->has_jumbo=1;
	}

	if(BGBCC_CCXL_CheckForOptStr(ctx, "wexj"))
	{
//		shctx->use_wexmd=2;
		shctx->has_jumbo=1;
	}

//	ctx->arch_has_predops=0;
	ctx->arch_has_predops=1;

	shctx->optmode=ctx->optmode;

	if(ctx->sub_arch==BGBCC_ARCH_BJX2_JX2A)
	{
		shctx->fpu_gfp=1;
		shctx->is_pbo=1;
	}

	if(ctx->sub_arch==BGBCC_ARCH_BJX2_JX2B)
	{
		shctx->is_fixed32=1;

		shctx->fpu_gfp=1;
		shctx->is_pbo=1;
	}

	if(ctx->sub_arch==BGBCC_ARCH_BJX2_JX2C)
	{
		ctx->arch_sizeof_long=4;
		ctx->arch_sizeof_ptr=4;
		shctx->is_addr_x32=1;
//		shctx->no_ops48=1;
		shctx->no_ops48=0;

		shctx->fpu_gfp=1;
		shctx->is_pbo=1;
	}

	if(ctx->sub_arch==BGBCC_ARCH_BJX2_JX2D)
	{
		ctx->arch_sizeof_long=4;
		ctx->arch_sizeof_ptr=4;
		shctx->is_fixed32=1;
		shctx->is_addr_x32=1;

		shctx->fpu_gfp=1;
		shctx->is_pbo=1;
	}

	if(ctx->sub_arch==BGBCC_ARCH_BJX2_JX2E)
	{
		ctx->arch_sizeof_long=4;
		ctx->arch_sizeof_ptr=4;
		shctx->is_addr_x32=1;
		shctx->no_ops48=1;

		shctx->no_fpu=1;
		shctx->fpu_soft=1;
		shctx->is_pbo=1;
	}

	if(ctx->sub_arch==BGBCC_ARCH_BJX2_JX2F)
	{
		ctx->arch_sizeof_long=4;
		ctx->arch_sizeof_ptr=4;
		shctx->is_addr_x32=1;
		shctx->no_ops48=1;

		shctx->fpu_gfp=1;
		shctx->is_pbo=1;
	}

	if(ctx->imgfmt==BGBCC_IMGFMT_SYS)
	{
		shctx->is_pbo=0;
	}

	BGBPP_AddStaticDefine(NULL, "__jx2__", "");

	BGBPP_AddStaticDefine(NULL, "__BGBCC__", "");
	BGBPP_AddStaticDefine(NULL, "__BGBCC_JX2__", "");

	BGBPP_AddStaticDefine(NULL, "__STDC__", "");
	BGBPP_AddStaticDefine(NULL, "__BJX2__", "");

	if(ctx->arch_sizeof_ptr==4)
	{
		BGBPP_AddStaticDefine(NULL, "__ADDR_32__", "");
	}

	if(ctx->arch_sizeof_ptr==8)
	{
		BGBPP_AddStaticDefine(NULL, "__ADDR_64__", "");
		BGBPP_AddStaticDefine(NULL, "__ADDR_X48__", "");
	}

	if(shctx->is_addr_x32)
	{
		BGBPP_AddStaticDefine(NULL, "__ADDR_X32__", "");
	}
	
	if(shctx->no_fpu)
		BGBPP_AddStaticDefine(NULL, "__NOFPU__", "");

	return(0);
}

bool BGBCC_JX2C_TypeShortRegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_JX2_Context *sctx;

	sctx=ctx->uctx;
	if(sctx->is_addr64)
		return(false);
	
//	if(BGBCC_CCXL_TypeSgByteP(ctx, ty))
//		return(true);
//	if(BGBCC_CCXL_TypeSignedShortP(ctx, ty))
//		return(true);

	return(false);
}

bool BGBCC_JX2C_TypeIntRegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_JX2_Context *sctx;

	sctx=ctx->uctx;
	
	if(BGBCC_CCXL_TypeSmallIntP(ctx, ty))
		return(true);
		
	if(ctx->arch_sizeof_ptr==4)
	{
		if(BGBCC_CCXL_TypePointerP(ctx, ty))
			return(true);
		if(BGBCC_CCXL_TypeFunctionP(ctx, ty))
			return(true);
	}

	if(sctx->no_fpu || sctx->fpu_soft)
	{
		if(sctx->fpu_gfp)
		{
			if(BGBCC_CCXL_TypeFloatP(ctx, ty))
				return(false);
			if(BGBCC_CCXL_TypeFloat16P(ctx, ty))
				return(false);
		}

		if(BGBCC_CCXL_TypeFloatP(ctx, ty))
			return(true);
		if(BGBCC_CCXL_TypeFloat16P(ctx, ty))
			return(true);
//		if(BGBCC_CCXL_TypeBFloat16P(ctx, ty))
//			return(true);
	}

	if(ty.val==0x000F)
		return(true);

	return(false);
}

bool BGBCC_JX2C_TypeInt2RegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_JX2_Context *sctx;

	sctx=ctx->uctx;
	
	if(ctx->arch_sizeof_ptr==8)
	{
		return(false);
	}

	if(sctx->is_addr64)
	{
		return(false);
	}

	if(ctx->arch_sizeof_ptr==4)
	{
		if(BGBCC_CCXL_TypeSmallLongP(ctx, ty))
			return(true);

		if(BGBCC_CCXL_TypeVoidP(ctx, ty))
			return(true);
		if(BGBCC_CCXL_TypeVariantP(ctx, ty))
			return(true);

		if(sctx->no_fpu || sctx->fpu_soft)
		{
			if(BGBCC_CCXL_TypeDoubleP(ctx, ty))
				return(true);
		}
	}

	return(false);
}

bool BGBCC_JX2C_TypeFloatRegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_JX2_Context *sctx;

	sctx=ctx->uctx;
	
	if(sctx->no_fpu || sctx->fpu_soft || sctx->fpu_gfp)
		return(0);

	if(BGBCC_CCXL_TypeFloatP(ctx, ty))
		return(true);
	if(BGBCC_CCXL_TypeFloat16P(ctx, ty))
		return(true);
	if(BGBCC_CCXL_TypeBFloat16P(ctx, ty))
		return(true);

//	if(BGBCC_CCXL_TypeDoubleP(ctx, ty))
//		return(true);

	return(false);
}

bool BGBCC_JX2C_TypeFloat2RegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_JX2_Context *sctx;

	sctx=ctx->uctx;
	
	if(sctx->no_fpu || sctx->fpu_soft || sctx->fpu_gfp)
		return(0);
	
//	if(BGBCC_CCXL_TypeDoubleP(ctx, ty))
//		return(true);
	return(false);
}

bool BGBCC_JX2C_TypeDoubleRegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_JX2_Context *sctx;

	sctx=ctx->uctx;
	
	if(sctx->no_fpu || sctx->fpu_soft || sctx->fpu_gfp)
		return(0);
	
	if(BGBCC_CCXL_TypeDoubleP(ctx, ty))
		return(true);
	return(false);
}

int BGBCC_JX2C_TypeGetRegClassP(BGBCC_TransState *ctx, ccxl_type ty)
{
	static byte rcls_lut[65536];
	static BGBCC_TransState *p_ctx=NULL;
	int rc;
	int i;

	if(p_ctx!=ctx)
	{
		p_ctx=ctx;
//		for(i=0; i<65536; i++)
		for(i=0; i<4096; i++)
			rcls_lut[i]=BGBCC_SH_REGCLS_NONE;
	}

//	if(ty.val<65536)
	if(ty.val<4096)
	{
		rc=rcls_lut[ty.val];
		if(rc!=BGBCC_SH_REGCLS_NONE)
		{
			return(rc);
		}
		
		rc=BGBCC_JX2C_TypeGetRegClassPI(ctx, ty);
		rcls_lut[ty.val]=rc;
		return(rc);
	}

	rc=BGBCC_JX2C_TypeGetRegClassPI(ctx, ty);
	return(rc);
}

int BGBCC_JX2C_TypeGetRegClassPI(BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_JX2_Context *sctx;
	int sz;

	sctx=ctx->uctx;

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
	{
		if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		{
			return(BGBCC_SH_REGCLS_AR_REF);
		}

		if(BGBCC_CCXL_TypeNearPointerP(ctx, ty))
			{ return(BGBCC_SH_REGCLS_GR); }
		if(BGBCC_CCXL_TypeFarPointerP(ctx, ty))
			{ return(BGBCC_SH_REGCLS_QGR); }

//		if(BGBCC_CCXL_TypePointerP(ctx, ty))
		if(1)
		{
			if(ctx->arch_sizeof_ptr==8)
				return(BGBCC_SH_REGCLS_QGR);
			return(BGBCC_SH_REGCLS_GR);
		}
	}

	if(BGBCC_JX2C_TypeShortRegP(ctx, ty))
		return(BGBCC_SH_REGCLS_WGR);

	if(BGBCC_JX2C_TypeIntRegP(ctx, ty))
		return(BGBCC_SH_REGCLS_GR);
	if(BGBCC_JX2C_TypeInt2RegP(ctx, ty))
		return(BGBCC_SH_REGCLS_GR2);

	if(BGBCC_JX2C_TypeFloatRegP(ctx, ty))
		return(BGBCC_SH_REGCLS_FR);
	if(BGBCC_JX2C_TypeFloat2RegP(ctx, ty))
		return(BGBCC_SH_REGCLS_FR2);
	if(BGBCC_JX2C_TypeDoubleRegP(ctx, ty))
		return(BGBCC_SH_REGCLS_DR);

	if(BGBCC_CCXL_TypeValueObjectP(ctx, ty))
	{
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, ty);
		if(sz<=4)
			return(BGBCC_SH_REGCLS_VO_GR);
		if(sz<=8)
		{
//			if(ctx->arch_sizeof_ptr==8)
			if(sctx->is_addr64)
				return(BGBCC_SH_REGCLS_VO_QGR);
			return(BGBCC_SH_REGCLS_VO_GR2);
		}
		if(sz<=16)
		{
			return(BGBCC_SH_REGCLS_VO_QGR2);
		}

		return(BGBCC_SH_REGCLS_VO_REF);
	}

	if(ctx->arch_sizeof_ptr==8)
	{
		if(BGBCC_CCXL_TypeSmallLongP(ctx, ty))
			return(BGBCC_SH_REGCLS_QGR);
		if(BGBCC_CCXL_TypeVoidP(ctx, ty))
			return(BGBCC_SH_REGCLS_QGR);
		if(BGBCC_CCXL_TypeVariantP(ctx, ty))
			return(BGBCC_SH_REGCLS_QGR);

		if(BGBCC_CCXL_TypePointerP(ctx, ty))
			return(BGBCC_SH_REGCLS_QGR);
		if(BGBCC_CCXL_TypeFunctionP(ctx, ty))
			return(BGBCC_SH_REGCLS_QGR);

		if(sctx->fpu_soft)
		{
			if(BGBCC_CCXL_TypeDoubleP(ctx, ty))
				return(BGBCC_SH_REGCLS_QGR);

			if(BGBCC_CCXL_TypeFloatP(ctx, ty))
				return(BGBCC_SH_REGCLS_GR);
			if(BGBCC_CCXL_TypeFloat16P(ctx, ty))
				return(BGBCC_SH_REGCLS_GR);
		}

		if(sctx->fpu_gfp)
		{
			if(BGBCC_CCXL_TypeDoubleP(ctx, ty))
				return(BGBCC_SH_REGCLS_QGR);
			if(BGBCC_CCXL_TypeFloatP(ctx, ty))
				return(BGBCC_SH_REGCLS_QGR);
			if(BGBCC_CCXL_TypeFloat16P(ctx, ty))
				return(BGBCC_SH_REGCLS_QGR);
		}
	}

	if(sctx->is_addr64)
	{
		if(BGBCC_CCXL_TypeSmallLongP(ctx, ty))
			return(BGBCC_SH_REGCLS_QGR);
		if(BGBCC_CCXL_TypeVoidP(ctx, ty))
			return(BGBCC_SH_REGCLS_QGR);
		if(BGBCC_CCXL_TypeVariantP(ctx, ty))
			return(BGBCC_SH_REGCLS_QGR);

		if(BGBCC_CCXL_TypePointerP(ctx, ty))
			return(BGBCC_SH_REGCLS_GR);
		if(BGBCC_CCXL_TypeFunctionP(ctx, ty))
			return(BGBCC_SH_REGCLS_GR);

		if(sctx->fpu_gfp)
		{
			if(BGBCC_CCXL_TypeDoubleP(ctx, ty))
				return(BGBCC_SH_REGCLS_QGR);
			if(BGBCC_CCXL_TypeFloatP(ctx, ty))
				return(BGBCC_SH_REGCLS_QGR);
			if(BGBCC_CCXL_TypeFloat16P(ctx, ty))
				return(BGBCC_SH_REGCLS_QGR);
		}

		if(sctx->fpu_soft)
		{
			if(BGBCC_CCXL_TypeDoubleP(ctx, ty))
				return(BGBCC_SH_REGCLS_QGR);

			if(BGBCC_CCXL_TypeFloatP(ctx, ty))
				return(BGBCC_SH_REGCLS_GR);
			if(BGBCC_CCXL_TypeFloat16P(ctx, ty))
				return(BGBCC_SH_REGCLS_GR);
		}
	}

	if(BGBCC_CCXL_TypeFatPointerP(ctx, ty))
	{
		return(BGBCC_SH_REGCLS_VO_QGR2);
	}

	return(BGBCC_SH_REGCLS_NONE);
}


int BGBCC_JX2C_EmitVaArg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register dreg,
	ccxl_register sreg)
{
	int i, j, k;

//	if(sctx->is_addr64)
	if(ctx->arch_sizeof_ptr==8)
	{
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ4);
		BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_RQ4);
	}
	else
	{
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
	}

	j=BGBCC_JX2C_TypeGetRegClassP(ctx, type);
	switch(j)
	{
	case BGBCC_SH_REGCLS_GR:
	case BGBCC_SH_REGCLS_VO_GR:
	case BGBCC_SH_REGCLS_VO_REF:
	case BGBCC_SH_REGCLS_WGR:
		if(sctx->is_addr64)
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__va64_arg_i");
		else
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__va_arg_i");

		BGBCC_JX2C_ResetModeDqUnknown(ctx, sctx);

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R2);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R2);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R2);

		break;

	case BGBCC_SH_REGCLS_VO_QGR:
	case BGBCC_SH_REGCLS_QGR:
		if(sctx->is_addr64)
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__va64_arg_l");
		else
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__va_arg_l");

		BGBCC_JX2C_ResetModeDqUnknown(ctx, sctx);

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R2);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R2);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R2);

		break;

	case BGBCC_SH_REGCLS_GR2:
	case BGBCC_SH_REGCLS_VO_GR2:
		if(sctx->is_addr64)
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__va64_arg_x");
		else
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__va_arg_x");

		BGBCC_JX2C_ResetModeDqUnknown(ctx, sctx);

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR2);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR2);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR2);

		break;

	case BGBCC_SH_REGCLS_VO_QGR2:
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__va64_arg_x");
		BGBCC_JX2C_ResetModeDqUnknown(ctx, sctx);

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR2);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR2);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR2);
		break;

	case BGBCC_SH_REGCLS_FR:
		if(sctx->is_addr64)
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__va64_arg_f");
		else
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__va_arg_f");

		BGBCC_JX2C_ResetModeDqUnknown(ctx, sctx);

//		BGBCC_JX2C_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_FR0);
		BGBCC_JX2C_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_FR2);
		break;
	case BGBCC_SH_REGCLS_FR2:
		if(sctx->is_addr64)
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__va64_arg_d");
		else
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__va_arg_d");

		BGBCC_JX2C_ResetModeDqUnknown(ctx, sctx);

//		BGBCC_JX2C_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR0);
		BGBCC_JX2C_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR2);
		break;

	case BGBCC_SH_REGCLS_DR:
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__va64_arg_d");
		BGBCC_JX2C_ResetModeDqUnknown(ctx, sctx);
//		BGBCC_JX2C_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR0);
		BGBCC_JX2C_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR2);
		break;

	default:
		BGBCC_DBGBREAK
		break;
	}

	BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);

	return(1);
}

int BGBCC_JX2C_EmitVaEnd(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register sreg)
{
	return(0);
}

int BGBCC_JX2C_EmitVaStart(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register sreg,
	ccxl_register treg)
{
	BGBCC_CCXL_RegisterInfo *obj;
	int ni, nf, sofs;
	int ix, kx;
	int i, j, k;
	
	ix=treg.val&4095;
	sofs=999999;
	
	obj=ctx->cur_func;

	return(0);
}

char *BGBCC_JX2C_DebugRegToStr(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register reg)
{
	char tb[256];
	char *s0;
	s64 li;
	int i;
	
	if(reg.val==CCXL_REGID_REG_Z)
	{
		sprintf(tb, "ZZ");
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_LONG)
	{
		li=reg.val<<3; li=li>>3;
		sprintf(tb, "I64(%lld)", li);
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_DOUBLE)
	{
		li=reg.val<<3;
		sprintf(tb, "F64(%g)", *(double *)(&li));
		return(bgbcc_rstrdup(tb));
	}
	
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
	{
		sprintf(tb, "T%d.%d",
			(int)(reg.val&CCXL_REGID_BASEMASK),
			(int)((reg.val&CCXL_REGID_SEQMASK)>>12));
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG)
	{
		sprintf(tb, "A%d.%d",
			(int)(reg.val&CCXL_REGID_BASEMASK),
			(int)((reg.val&CCXL_REGID_SEQMASK)>>12));
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL)
	{
		sprintf(tb, "L%d.%d",
			(int)(reg.val&CCXL_REGID_BASEMASK),
			(int)((reg.val&CCXL_REGID_SEQMASK)>>12));
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_GLOBAL)
	{
		i=(int)(reg.val&CCXL_REGID_REGMASK);
//		i=(int)(reg.val);
		s0=ctx->reg_globals[i]->name;
		if(!s0)s0="?";
		sprintf(tb, "G%d(%s)", i, s0);
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT)
	{
		sprintf(tb, "I32(%d)", (int)(reg.val));
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG)
	{
		li=reg.val<<8; li=li>>8;
		sprintf(tb, "I64(%lld)", li);
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_FLOAT)
	{
		i=(int)(reg.val);
		sprintf(tb, "F32(%g)", *(float *)(&i));
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE)
	{
		li=reg.val<<8;
		sprintf(tb, "F64(%g)", *(double *)(&li));
		return(bgbcc_rstrdup(tb));
	}

	sprintf(tb, "%016llX", reg.val);
	return(bgbcc_rstrdup(tb));
}

ccxl_status BGBCC_JX2C_PrintVirtOp(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, BGBCC_CCXL_VirtOp *op)
{
	ccxl_register *ca;
	char *s0;
	int na;
	int i, j, k;

#if 1
	if(sctx->cgen_log)
	{
//		fprintf(sctx->cgen_log,
//			"\t%04X ty=%08X",
//			op->opn, op->type.val);

		s0=NULL;
		switch(op->opn)
		{
			case CCXL_VOP_NONE:				s0="NONE"; break;
			case CCXL_VOP_DBGFN:			s0="DBGFN"; break;
			case CCXL_VOP_DBGLN:			s0="DBGLN"; break;
			case CCXL_VOP_LABEL:			s0="LABEL"; break;
			case CCXL_VOP_JMP:				s0="JMP"; break;
			case CCXL_VOP_MOV:				s0="MOV"; break;
			case CCXL_VOP_JCMP_ZERO:		s0="JCMP_Z"; break;
			case CCXL_VOP_JCMP:				s0="JCMP"; break;
			case CCXL_VOP_CALL:				s0="CALL"; break;
			case CCXL_VOP_CSRV:				s0="CSRV"; break;
			case CCXL_VOP_RETDFL:			s0="RETDFL"; break;
			case CCXL_VOP_RETV:				s0="RETV"; break;
			case CCXL_VOP_RET:				s0="RET"; break;
			case CCXL_VOP_CONV:				s0="CONV"; break;
			case CCXL_VOP_UNARY:			s0="UNARY"; break;
			case CCXL_VOP_BINARY:			s0="BINARY"; break;
			case CCXL_VOP_COMPARE:			s0="COMPARE"; break;
			case CCXL_VOP_LDIXIMM:			s0="LDIXIMM"; break;
			case CCXL_VOP_STIXIMM:			s0="STIXIMM"; break;
			case CCXL_VOP_LDIX:				s0="LDIX"; break;
			case CCXL_VOP_STIX:				s0="STIX"; break;
			case CCXL_VOP_LEAIMM:			s0="LEAIMM"; break;
			case CCXL_VOP_LEA:				s0="LEA"; break;
			case CCXL_VOP_LDAVAR:			s0="LDAVAR"; break;
			case CCXL_VOP_SIZEOFVAR:		s0="SIZEOFVAR"; break;
			case CCXL_VOP_DIFFPTR:			s0="DIFFPTR"; break;
			case CCXL_VOP_OFFSETOF:			s0="OFFSETOF"; break;
			case CCXL_VOP_LOADSLOT:			s0="LOADSLOT"; break;
			case CCXL_VOP_STORESLOT:		s0="STORESLOT"; break;
			case CCXL_VOP_LOADSLOTADDR:		s0="LOADSLOTA"; break;
			case CCXL_VOP_LOADSLOTADDRID:	s0="LOADSLOTA"; break;
			case CCXL_VOP_INITOBJ:			s0="INITOBJ"; break;
			case CCXL_VOP_DROPOBJ:			s0="DROPOBJ"; break;
			case CCXL_VOP_INITARR:			s0="INITARR"; break;
			case CCXL_VOP_INITOBJARR:		s0="INITOBJARR"; break;
			case CCXL_VOP_LOADINITARR:		s0="LDINITARR"; break;
			case CCXL_VOP_LOADINITOBJARR:	s0="LDINITOBJARR"; break;
			case CCXL_VOP_VA_START:			s0="VA_START"; break;
			case CCXL_VOP_VA_END:			s0="VA_END"; break;
			case CCXL_VOP_VA_ARG:			s0="VA_ARG"; break;
			case CCXL_VOP_CSELCMP:			s0="CSELCMP"; break;
			case CCXL_VOP_CSELCMP_Z:		s0="CSELCMP_Z"; break;
			case CCXL_VOP_OBJCALL:			s0="OBJCALL"; break;
			case CCXL_VOP_PREDCMP:			s0="PREDCMP"; break;
			case CCXL_VOP_CALL_INTRIN:		s0="CALL_INTRIN"; break;
		}

		if(s0)
		{
			fprintf(sctx->cgen_log,
				"\t%s",
				s0);
		}else
		{
			fprintf(sctx->cgen_log,
				"\t%04X",
				op->opn);
		}

		if(op->type.val<256)
		{
			fprintf(sctx->cgen_log,
				" ty=%02X", op->type.val);
		}else if(op->type.val<65536)
		{
			fprintf(sctx->cgen_log,
				" ty=%04X", op->type.val);
		}else
		{
			fprintf(sctx->cgen_log,
				" ty=%08X", op->type.val);
		}
		
		if(op->stype.val)
		{
			if(op->stype.val<256)
			{
				fprintf(sctx->cgen_log,
					" sty=%02X", op->stype.val);
			}else if(op->stype.val<65536)
			{
				fprintf(sctx->cgen_log,
					" sty=%04X", op->stype.val);
			}else
			{
				fprintf(sctx->cgen_log,
					" sty=%08X", op->stype.val);
			}
		}

		if(op->opr)
		{
			fprintf(sctx->cgen_log,
				" opr=%02X", op->opr);
		}

		if(op->dst.val)
		{
//			fprintf(sctx->cgen_log, " dst=%016llX", op->dst.val);
			fprintf(sctx->cgen_log, " dst=%s", 
				BGBCC_JX2C_DebugRegToStr(ctx, sctx, op->type, op->dst));
		}
		if(op->srca.val)
		{
//			fprintf(sctx->cgen_log, " srca=%016llX", op->srca.val);
			fprintf(sctx->cgen_log, " srca=%s", 
				BGBCC_JX2C_DebugRegToStr(ctx, sctx, op->type, op->srca));
		}
		if(op->srcb.val)
		{
//			fprintf(sctx->cgen_log, " srcb=%016llX", op->srcb.val);
			fprintf(sctx->cgen_log, " srcb=%s", 
				BGBCC_JX2C_DebugRegToStr(ctx, sctx, op->type, op->srcb));
		}

		if(op->srcc.val)
		{
			fprintf(sctx->cgen_log, " srcc=%s", 
				BGBCC_JX2C_DebugRegToStr(ctx, sctx, op->type, op->srcc));
		}

		if(op->srcd.val)
		{
			fprintf(sctx->cgen_log, " srcd=%s", 
				BGBCC_JX2C_DebugRegToStr(ctx, sctx, op->type, op->srcd));
		}

		if(op->imm.ul)
		{
			if((op->opn==CCXL_VOP_LOADSLOT) ||
				(op->opn==CCXL_VOP_STORESLOT) ||
				(op->opn==CCXL_VOP_LOADSLOTADDR))
			{
				fprintf(sctx->cgen_log, " obj=%03X.%02X",
					op->imm.obj.gid, op->imm.obj.fid);
			}else if(op->imm.ul==op->imm.ui)
				{ fprintf(sctx->cgen_log, " imm=%08X", op->imm.ui); }
			else 
				{ fprintf(sctx->cgen_log, " imm=%016llX", op->imm.ul); }
		}

		fprintf(sctx->cgen_log, "\n");
		fflush(sctx->cgen_log);
		
//		if(op->opn==CCXL_VOP_CALL)
		if((op->opn==CCXL_VOP_CALL) ||
			(op->opn==CCXL_VOP_OBJCALL) ||
			(op->opn==CCXL_VOP_CALL_INTRIN))
		{
			na=op->imm.call.na;
			ca=op->imm.call.args;
			for(i=0; i<na; i++)
			{
				fprintf(sctx->cgen_log, "\t\tA%d=%s\n",  i,
					BGBCC_JX2C_DebugRegToStr(ctx, sctx, op->type, ca[i]));
			}
			fflush(sctx->cgen_log);
		}
	}
#endif

	return(0);
}

ccxl_status BGBCC_JX2C_CompileVirtOp(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, BGBCC_CCXL_VirtOp *op)
{
	char *s0;

//	BGBCC_JX2C_CompilePrintVirtOp(ctx, sctx, obj, op);

	sctx->test_lclalign=0;

//	sctx->test_lclalign=1;
//	sctx->test_lclalign=3;

	sctx->sreg_live=sctx->sreg_held;
	sctx->sfreg_live=sctx->sfreg_held;
	BGBCC_JX2_EmitCheckFlushIndexImm(sctx);

	switch(op->opn)
	{
	case CCXL_VOP_NONE:
		break;
	case CCXL_VOP_DBGFN:
		ctx->lfn=op->imm.str;
		break;
	case CCXL_VOP_DBGLN:
		ctx->lln=op->imm.si;
		break;
	case CCXL_VOP_LABEL:
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
//		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
//		BGBCC_JX2_EmitForceFlushIndexImm(sctx);
		BGBCC_JX2_EmitLabel(sctx,
			op->imm.si);
		break;
	case CCXL_VOP_JMP:
//		BGBCC_JX2_EmitOpLabel(sctx, BGBCC_SH_NMID_BRA,
//			op->imm.si);
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
//		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
//		BGBCC_JX2C_ResetFpscrUnknown(ctx, sctx);

//		BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BRA,
//			op->imm.si);
//		BGBCC_JX2_EmitOpNone(sctx,
//			BGBCC_SH_NMID_NOP);

		BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BRAN,
			op->imm.si);

//		BGBCC_JX2_EmitFlushIndexImm16(sctx);
//		BGBCC_JX2_EmitFlushIndexImm32(sctx);
		BGBCC_JX2_EmitFlushIndexImmBasic(sctx);
		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
		break;
	case CCXL_VOP_MOV:
		BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, op->type,
			op->dst, op->srca);
		break;
	case CCXL_VOP_JCMP_ZERO:
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
//		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
//		BGBCC_JX2C_ResetFpscrUnknown(ctx, sctx);
//		BGBCC_JX2_EmitForceFlushIndexImm(sctx);
		BGBCC_JX2C_EmitJCmpVRegZero(ctx, sctx, op->type,
			op->srca, op->opr, op->imm.si);
		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
		break;
	case CCXL_VOP_JCMP:
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
//		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
//		BGBCC_JX2C_ResetFpscrUnknown(ctx, sctx);
//		BGBCC_JX2_EmitForceFlushIndexImm(sctx);
		BGBCC_JX2C_EmitJCmpVRegVReg(ctx, sctx, op->type,
			op->srca, op->srcb, op->opr, op->imm.si);
		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
		break;
	case CCXL_VOP_CALL:
	case CCXL_VOP_OBJCALL:
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
//		BGBCC_JX2C_ResetFpscrDefaults(ctx, sctx);
//		BGBCC_JX2_EmitForceFlushIndexImm(sctx);

//		sctx->sreg_live|=0x00F4;
//		sctx->sreg_live|=0x00F0;
//		sctx->sreg_live|=0x00F000F0;
//		sctx->sfreg_live|=0x0FF0;
		BGBCC_JX2C_EmitCallVReg(ctx, sctx,
			op->type, op->dst, op->srca, op->srcb,
			op->imm.call.na, op->imm.call.args);

//		sctx->sreg_live&=~0x00F0;
//		sctx->sreg_live&=~0x00F000F0;
//		sctx->sfreg_live&=~0x0FF0;

//		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
//		BGBCC_JX2C_EmitLabelFlushFpRegisters(ctx, sctx);
		break;
	case CCXL_VOP_CSRV:
//		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
//		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
		BGBCC_JX2C_EmitCsrvVReg(ctx, sctx,
			op->type, op->dst);
		break;

	case CCXL_VOP_CALL_INTRIN:
		BGBCC_JX2C_EmitCallIntrinVReg(ctx, sctx,
			op->type, op->dst, op->srca, op->srcb,
			op->imm.call.na, op->imm.call.args);
		break;

	case CCXL_VOP_RETDFL:
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
//		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
		BGBCC_JX2C_EmitReturnVoid(ctx, sctx);
		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
		BGBCC_JX2_EmitFlushIndexImmBasic(sctx);
		break;
	case CCXL_VOP_RETV:
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
//		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
		BGBCC_JX2C_EmitReturnVoid(ctx, sctx);
		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
		BGBCC_JX2_EmitFlushIndexImmBasic(sctx);
		break;
	case CCXL_VOP_RET:
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
//		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
		BGBCC_JX2C_EmitReturnVReg(ctx, sctx,
			op->type, op->srca);
		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
		BGBCC_JX2_EmitFlushIndexImmBasic(sctx);
		break;
	case CCXL_VOP_CONV:
		BGBCC_JX2C_EmitConvVRegVReg(ctx, sctx,
			op->type, op->stype,
			op->dst, op->srca);
		break;
	case CCXL_VOP_UNARY:
		BGBCC_JX2C_EmitUnaryVRegVReg(ctx, sctx, op->type,
			op->dst, op->opr, op->srca);
		break;
	case CCXL_VOP_BINARY:
		BGBCC_JX2C_EmitBinaryVRegVRegVReg(ctx, sctx, op->type,
			op->dst, op->opr, op->srca, op->srcb);
		break;
	case CCXL_VOP_COMPARE:
		BGBCC_JX2C_EmitCompareVRegVRegVReg(ctx, sctx, op->type,
			op->dst, op->opr, op->srca, op->srcb);
		break;
	case CCXL_VOP_LDIXIMM:
		BGBCC_JX2C_EmitLdixVRegVRegImm(ctx, sctx, op->type,
			op->dst, op->srca, op->imm.si);
		break;
	case CCXL_VOP_STIXIMM:
		BGBCC_JX2C_EmitStixVRegVRegImm(ctx, sctx, op->type,
			op->dst, op->srca, op->imm.si);
		break;
	case CCXL_VOP_LDIX:
		BGBCC_JX2C_EmitLdixVRegVRegVReg(ctx, sctx, op->type,
			op->dst, op->srca, op->srcb);
		break;
	case CCXL_VOP_STIX:
		BGBCC_JX2C_EmitStixVRegVRegVReg(ctx, sctx, op->type,
			op->dst, op->srca, op->srcb);
		break;
	case CCXL_VOP_LEAIMM:
		BGBCC_JX2C_EmitLeaVRegVRegImm(ctx, sctx, op->type,
			op->dst, op->srca, op->imm.si);
		break;
	case CCXL_VOP_LEA:
		BGBCC_JX2C_EmitLeaVRegVRegVReg(ctx, sctx, op->type,
			op->dst, op->srca, op->srcb);
		break;
	case CCXL_VOP_LDAVAR:
		BGBCC_JX2C_EmitLdaVRegVReg(ctx, sctx, op->type,
			op->dst, op->srca);
		break;

	case CCXL_VOP_SIZEOFVAR:
		BGBCC_JX2C_SizeofVar(ctx, sctx, op->type, op->dst);
//		BGBCC_CCXL_StubError(ctx);
		break;

	case CCXL_VOP_DIFFPTR:
		BGBCC_JX2C_EmitDiffPtrVRegVRegVReg(ctx, sctx, op->type,
			op->dst, op->srca, op->srcb);
		break;

	case CCXL_VOP_LOADSLOT:
		BGBCC_JX2C_EmitLoadSlotVRegVRegImm(ctx, sctx, op->type,
			op->dst, op->srca, op->imm.obj.gid, op->imm.obj.fid);
		break;
	case CCXL_VOP_STORESLOT:
		BGBCC_JX2C_EmitStoreSlotVRegVRegImm(ctx, sctx, op->type,
			op->dst, op->srca, op->imm.obj.gid, op->imm.obj.fid);
		break;
	case CCXL_VOP_LOADSLOTADDR:
		BGBCC_JX2C_EmitLoadSlotAddrVRegVRegImm(ctx, sctx, op->type,
			op->dst, op->srca, op->imm.obj.gid, op->imm.obj.fid);
		break;
	
	case CCXL_VOP_INITOBJ:
		if(BGBCC_CCXL_IsRegLocalP(ctx, op->dst) ||
			BGBCC_CCXL_IsRegArgP(ctx, op->dst))
				break;
		if(BGBCC_CCXL_IsRegTempP(ctx, op->dst))
		{
			BGBCC_JX2C_EmitInitObj(ctx, sctx, op->type, op->dst);
			break;
		}
		BGBCC_CCXL_StubError(ctx);
		break;
	case CCXL_VOP_DROPOBJ:
		if(BGBCC_CCXL_IsRegLocalP(ctx, op->dst) ||
			BGBCC_CCXL_IsRegArgP(ctx, op->dst))
				break;
		if(BGBCC_CCXL_IsRegTempP(ctx, op->dst))
			break;
		BGBCC_CCXL_StubError(ctx);
		break;
	case CCXL_VOP_INITARR:
		if(BGBCC_CCXL_IsRegLocalP(ctx, op->dst) ||
			BGBCC_CCXL_IsRegArgP(ctx, op->dst))
				break;
		BGBCC_CCXL_StubError(ctx);
		break;
	case CCXL_VOP_INITOBJARR:
		if(BGBCC_CCXL_IsRegLocalP(ctx, op->dst) ||
			BGBCC_CCXL_IsRegArgP(ctx, op->dst))
				break;
		BGBCC_CCXL_StubError(ctx);
		break;

	case CCXL_VOP_VA_START:
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
		BGBCC_JX2C_EmitVaStart(ctx, sctx, op->dst, op->srca);
		break;
	case CCXL_VOP_VA_END:
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
		BGBCC_JX2C_EmitVaEnd(ctx, sctx, op->srca);
		break;
	case CCXL_VOP_VA_ARG:
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
		BGBCC_JX2C_EmitVaArg(ctx, sctx, op->type, op->dst, op->srca);
		break;

	case CCXL_VOP_JMPTAB:
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
		BGBCC_JX2C_EmitJmpTab(ctx, sctx,
			op->type, op->srca,
			op->imm.jmptab.vmin, op->imm.jmptab.nlbl, op->imm.jmptab.lbls);
		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
		break;

	case CCXL_VOP_PREDCMP:
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
		BGBCC_JX2C_EmitPredCmpVRegVReg(ctx, sctx, op->type,
			op->srca, op->srcb, op->opr);
		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
		break;

	case CCXL_VOP_PREDSYNC:
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
		break;

	default:
		BGBCC_CCXL_StubError(ctx);
		break;
	}

	BGBCC_JX2_EmitCheckFlushIndexImm(sctx);
	
	if(sctx->regalc_live || sctx->fregalc_live)
	{
		BGBCC_DBGBREAK
	}

	if(sctx->sreg_live || sctx->sfreg_live)
	{
		BGBCC_DBGBREAK
	}

#if 1
//	BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
//	BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);

//	BGBCC_JX2C_EmitSyncFpRegisters(ctx, sctx);
//	BGBCC_JX2C_EmitLabelFlushFpRegisters(ctx, sctx);
#endif
	
	return(1);
}

ccxl_status BGBCC_JX2C_PrintVirtTr(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, BGBCC_CCXL_VirtTr *tr, int idx)
{
	int i, j, k;

	if(sctx->cgen_log)
	{
		fprintf(sctx->cgen_log,
			"   Tr: idx=%d base=%d num=%d\n",
			idx, tr->b_ops, tr->n_ops);
		fflush(sctx->cgen_log);
	}

	for(i=0; i<tr->n_ops; i++)
	{
		BGBCC_JX2C_PrintVirtOp(ctx, sctx, obj, obj->vop[tr->b_ops+i]);
	}

	return(0);
}

ccxl_status BGBCC_JX2C_CompileVirtTr(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, BGBCC_CCXL_VirtTr *tr, int idx)
{
	BGBCC_CCXL_VirtOp *vop, *vop1, *vop2;
	int ps0, ps1, usewex;
	int i, j, k;

#if 0
	if(sctx->cgen_log)
	{
		fprintf(sctx->cgen_log,
			"   Tr: idx=%d base=%d num=%d\n",
			idx, tr->b_ops, tr->n_ops);
		fflush(sctx->cgen_log);
	}
#endif

	ctx->cur_vtr=tr;

	sctx->is_leaf&=~4;
	sctx->is_fixed32&=(~16);

//	if(sctx->is_simpass==1)
	if((sctx->is_simpass==1) || (sctx->is_simpass&64))
	{
		sctx->is_leaf|=4;
	}

	sctx->is_tr_leaf=0;
//	if(tr->trfl&1)
	if(tr->trfl&1)
		sctx->is_tr_leaf=1;

	usewex=0;

//	if(tr->n_ops>10)
	if(tr->n_ops>5)
//	if(tr->n_ops>4)
//	if(tr->n_ops>3)
	{
		if(sctx->use_egpr)
//		if(sctx->use_egpr && sctx->is_tr_leaf)
//		if(sctx->is_tr_leaf)
		{
//			sctx->is_fixed32|=16;
			usewex=1;
		}
	}

	if(tr->n_ops>2)
	{
#if 1
//		if((ctx->optmode==BGBCC_OPT_SPEED) && sctx->is_tr_leaf)
		if(ctx->optmode==BGBCC_OPT_SPEED2)
		{
//			sctx->is_fixed32|=16;
			usewex=1;
		}
#endif
	}

	if(usewex)
		{ BGBCC_JX2_BeginWex(sctx); }

#if 0
	ps0=BGBCC_JX2_EmitGetOffs(sctx);
	if(sctx->is_fixed32&16)
		sctx->wex_ofs_begin=ps0;
#endif

	sctx->op_is_wex2=0;

	for(i=0; i<tr->n_ops; i++)
	{
		sctx->tr_opnum=tr->b_ops+i;
		vop=obj->vop[tr->b_ops+i];
		vop1=NULL;
		if((i+1)<tr->n_ops)
			vop1=obj->vop[tr->b_ops+i+1];

		ctx->cur_vop=vop;

		j=0;
//		if(vop->prd==2)j=(sctx->pred_tfmd)?5:4;
//		if(vop->prd==3)j=(sctx->pred_tfmd)?4:5;
		if(vop->prd==3)j=(sctx->pred_tfmd)?5:4;
		if(vop->prd==2)j=(sctx->pred_tfmd)?4:5;
		sctx->op_is_wex2=j;

#if 0
		/* Hacky Special Case Opt: arr[expr+imm] */
		if(	vop && vop1 && sctx->has_bjx1mov &&
			(vop->opn==CCXL_VOP_BINARY) &&
			(vop->opr==CCXL_BINOP_ADD) &&
			(vop1->opn==CCXL_VOP_LDIX) &&
			BGBCC_CCXL_RegisterSequenceEqualP(ctx,
				vop->dst, vop1->srcb) &&
			BGBCC_CCXL_TypeSmallIntP(ctx, vop1->type) &&
			BGBCC_CCXL_IsRegTempP(ctx, vop->dst) &&
			BGBCC_CCXL_IsRegImmIntP(ctx, vop->srcb)	)
		{
			j=BGBCC_CCXL_GetRegImmIntValue(ctx, vop->srcb);
			if((j>=0) && (j<16))
			{
				BGBCC_JX2C_EmitLdixVRegVRegVRegImm(
					ctx, sctx, vop1->type,
					vop1->dst, vop1->srca, vop->srca, j);
				i++;
				continue;
			}
		}

#if 1
		/* Hacky Special Case Opt: arr[expr+imm]=val */
		if(	vop && vop1 && sctx->has_bjx1mov &&
			(vop->opn==CCXL_VOP_BINARY) &&
			(vop->opr==CCXL_BINOP_ADD) &&
			(vop1->opn==CCXL_VOP_STIX) &&
			BGBCC_CCXL_RegisterSequenceEqualP(ctx,
				vop->dst, vop1->srca) &&
			BGBCC_CCXL_TypeSmallIntP(ctx, vop1->type) &&
			BGBCC_CCXL_IsRegTempP(ctx, vop->dst) &&
			BGBCC_CCXL_IsRegImmIntP(ctx, vop->srcb)	)
		{
			j=BGBCC_CCXL_GetRegImmIntValue(ctx, vop->srcb);
			if((j>=0) && (j<16))
			{
				BGBCC_JX2C_EmitStixVRegVRegVRegImm(
					ctx, sctx, vop1->type,
					vop1->dst, vop->srca, vop1->srcb, j);
				i++;
				continue;
			}
		}
#endif
#endif
		
		BGBCC_JX2C_CompileVirtOp(ctx, sctx, obj, vop);
	}

	sctx->op_is_wex2=0;

#if 0
	ps1=BGBCC_JX2_EmitGetOffs(sctx);

	if(sctx->is_fixed32&16)
	{
		ps0=sctx->wex_ofs_begin;
		BGBCC_JX2C_CheckWexify(ctx, sctx, ps0, ps1);
	}
	sctx->is_fixed32&=(~16);
#endif

	BGBCC_JX2_EndWex(sctx);

	if(sctx->use_egpr)
	{
		if(!BGBCC_JX2_CheckPadAlign32(sctx))
		{
			BGBCC_JX2_EmitPadCheckExpandLastOp(sctx);
		}
	}

	BGBCC_JX2_EmitCheckFlushIndexImm(sctx);

//	if(sctx->is_simpass==1)
	if((sctx->is_simpass==1) || (sctx->is_simpass&64))
	{
		if(sctx->is_leaf&4)
			tr->trfl|=1;
		else
			tr->trfl&=~1;
		sctx->is_leaf&=(~4);
	}

	return(0);
}

ccxl_status BGBCC_JX2C_EndFunction(BGBCC_TransState *ctx,
	BGBCC_CCXL_LiteralInfo *obj)
{
	return(0);
}

int BGBCC_JX2C_BuildFunctionLastTraceP(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx)
{
	BGBCC_CCXL_VirtTr *tr;
	BGBCC_CCXL_VirtOp *vop;

	if((sctx->tr_trnum+1)==ctx->cur_func->n_vtr)
		return(1);

	if((sctx->tr_trnum+2)==ctx->cur_func->n_vtr)
	{
		tr=ctx->cur_func->vtr[sctx->tr_trnum+1];
		if(tr->n_ops==1)
		{
			vop=ctx->cur_func->vop[tr->b_ops+0];
			if(vop->opn==CCXL_VOP_RETDFL)
				return(1);
		}
	}

	return(0);
}

ccxl_status BGBCC_JX2C_BuildFunctionBody(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, int fcnlbl)
{
	int bs, bt, bo, co, ce, bo1, co1;
	int plsz, fnsz;
	int i, j, k;

	ctx->cur_func=obj;
	sctx->is_tr_leaf=0;

	bs=BGBCC_JX2_EmitGetOffs(sctx);

	BGBCC_JX2C_EmitFrameProlog(ctx, sctx, obj, fcnlbl);
	
	bt=sctx->cur_fcnbase;
	
	bo=BGBCC_JX2_EmitGetOffs(sctx);

	for(i=0; i<obj->n_vtr; i++)
	{
		bo1=BGBCC_JX2_EmitGetOffs(sctx);
		sctx->tr_trnum=i;
		BGBCC_JX2C_CompileVirtTr(ctx, sctx, obj, obj->vtr[i], i);
		co1=BGBCC_JX2_EmitGetOffs(sctx);
		
		if(i<16)
		{
			sctx->fnsz_lvtr[i]=sctx->fnsz_vtr[i];
			sctx->fnsz_vtr[i]=co1-bo1;
			sctx->fnsz_dvtr[i]=sctx->fnsz_vtr[i]-sctx->fnsz_lvtr[i];
		}
	}

	co=BGBCC_JX2_EmitGetOffs(sctx);
	sctx->fnsz_bod=co-bo;

	sctx->is_tr_leaf=0;

	BGBCC_JX2C_EmitFrameEpilog(ctx, sctx, obj);

	ce=BGBCC_JX2_EmitGetOffs(sctx);

	plsz=(bo-bt)/2;
	fnsz=(ce-bt)/2;

//	if(!sctx->is_simpass)
	if(!sctx->is_rom)
	{
		BGBCC_JX2_SetSectionName(sctx, ".pdata");

		BGBCC_JX2_EmitRelocTy(sctx, fcnlbl, BGBCC_SH_RLC_RVA32);
		BGBCC_JX2_EmitDWord(sctx, 0);	//start of function

		k=plsz | (fnsz<<8);
		BGBCC_JX2_EmitDWord(sctx, k);	//function size / etc.

		BGBCC_JX2_SetSectionName(sctx, ".text");
	}

	return(0);
}

ccxl_status BGBCC_JX2C_BuildFunction(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	BGBCC_JX2_Context *sctx;
	int fnsz[8];
	int fnsz_pro[8];
	int fnsz_epi[8];
	int fnsz_bod[8];
	int otrov, issta;
	int l0, l1, np, co, lo, bo, bo1, co1, sz, sz1, msz, nsz;
	byte simonly;
	int i, j, k;

	sctx=ctx->uctx;
	
	if(!obj->qname)
		obj->qname=obj->name;

	if(obj->regflags&BGBCC_REGFL_CULL)
	{
		if(sctx->cgen_log)
		{
			fprintf(sctx->cgen_log, "BGBCC_JX2C_BuildFunction: Culled %s\n",
				obj->qname);
			fflush(sctx->cgen_log);
		}

		return(0);
	}

#if 1
	if((obj->flagsint&BGBCC_TYFL_DLLIMPORT) && (obj->n_vtr<=0))
	{
		l0=obj->fxoffs;
		if(l0<=0)
		{
			l0=BGBCC_JX2_GetNamedLabel(sctx, obj->qname);
			obj->fxoffs=l0;
		}

		l1=obj->fxnoffs;
		if(l1<=0)
		{
			l1=BGBCC_JX2_GetNamedLabel(sctx, obj->qname);
			obj->fxnoffs=l1;
		}

		BGBCC_JX2_SetSectionName(sctx, ".text");
		BGBCC_JX2_EmitBAlign(sctx, 4);
//		BGBCC_JX2_EmitBAlign(sctx, 16);

//		bo=BGBCC_JX2_EmitGetOffs(sctx);

		BGBCC_JX2_EmitLabel(sctx, l0);
//		BGBCC_JX2_EmitWord(sctx, 0xD301);
//		BGBCC_JX2_EmitWord(sctx, 0x6332);
//		BGBCC_JX2_EmitWord(sctx, 0x432B);
//		BGBCC_JX2_EmitWord(sctx, 0x0009);

//		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);
		if(ctx->arch_sizeof_ptr==8)
		{
			BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
				BGBCC_SH_NMID_MOVQ, BGBCC_SH_REG_R1, l1);
			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_BRA, BGBCC_SH_REG_R1);
		}else
		{
			BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
				BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1, l1);
			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_BRA, BGBCC_SH_REG_R1);
		}

//		if(sctx->is_addr64)
		if(ctx->arch_sizeof_ptr==8)
		{
			BGBCC_JX2_EmitBAlign(sctx, 8);
			BGBCC_JX2_EmitLabel(sctx, l1);
			BGBCC_JX2_EmitQWord(sctx, 0);
		}else
		{
			BGBCC_JX2_EmitBAlign(sctx, 4);
			BGBCC_JX2_EmitLabel(sctx, l1);
			BGBCC_JX2_EmitDWord(sctx, 0);
		}

		return(0);
	}
#endif

	simonly=(sctx->is_simpass&64)!=0;

	if(sctx->cgen_log)
	{
		fprintf(sctx->cgen_log, "BGBCC_JX2C_BuildFunction: Begin %s\n",
			obj->qname);
		fflush(sctx->cgen_log);

		for(i=0; i<obj->n_vtr; i++)
		{
			BGBCC_JX2C_PrintVirtTr(ctx, sctx, obj, obj->vtr[i], i);
		}
	}

	
	l0=obj->fxoffs;
	if(l0<=0)
	{
//		l0=BGBCC_JX2_GenLabel(sctx);
		l0=BGBCC_JX2_GetNamedLabel(sctx, obj->qname);
		obj->fxoffs=l0;
	}
	
#if 1
	if(sctx->cgen_log)
	{
		fprintf(sctx->cgen_log, "BGBCC_JX2C_BuildFunction: BegSim %s\n",
			obj->qname);
		fflush(sctx->cgen_log);
	}

	sctx->cnt_set_fp32=0;
	sctx->cnt_set_fp64=0;

	sctx->cnt_set_dq0=0;
	sctx->cnt_set_dq1=0;
	sctx->dfl_dq=0;

	sctx->use_egpr=0;
	sctx->use_efpr=0;

	otrov=sctx->lbltrov;
	sctx->reg_vsave=0;
	sctx->freg_vsave=0;
	lo=-1;
//	bo=BGBCC_JX2_EmitGetOffs(sctx);
	sctx->simfnsz=9999999;
	nsz=0;
	msz=999999999;

	sctx->need_farjmp=1;
	sctx->need_f16jmp=1;
	sctx->need_n12jmp=1;
	sctx->is_leaf=2;
	
//	sctx->need_n16bsr=(sctx->sim_txtsz>=(65536-4096));
//	sctx->need_n20bsr=(sctx->sim_txtsz>=(1048576-65536));
//	sctx->need_n24bsr=(sctx->sim_txtsz>=(16777216-262144));

//	sctx->dfl_dq=2;
//	sctx->dfl_dq=3;
	sctx->dfl_dq=4;

	if(sctx->is_addr64 && !sctx->has_bjx1egpr &&
			!sctx->is_betav)
	{
		sctx->dfl_dq=2;
	}

//	__debugbreak();

//	for(np=0; np<6; np++)
	for(np=0; np<4; np++)
	{
		sctx->lbltrov=otrov;

		sctx->is_stable=1;		//pretend stable
		if(np==0)
			sctx->is_stable=0;

		if(!simonly)
		{
			BGBCC_JX2_SetBeginSimPass(sctx);
			sctx->is_simpass=np+1;
		}

		BGBCC_JX2C_SetupFrameLayout(ctx, sctx, obj);

		BGBCC_JX2_SetSectionName(sctx, ".text");
		BGBCC_JX2_EmitBAlign(sctx, 4);
//		BGBCC_JX2_EmitBAlign(sctx, 16);

		bo=BGBCC_JX2_EmitGetOffs(sctx);

//		BGBCC_JX2_EmitLabel(sctx, l0);
		BGBCC_JX2C_BuildFunctionBody(ctx, sctx, obj, l0);

#if 0
		BGBCC_JX2C_EmitFrameProlog(ctx, sctx, obj);
		
		for(i=0; i<obj->n_vtr; i++)
		{
			sctx->tr_trnum=i;
			BGBCC_JX2C_CompileVirtTr(ctx, sctx, obj, obj->vtr[i], i);
		}
		
		BGBCC_JX2C_EmitFrameEpilog(ctx, sctx, obj);
#endif

		co=BGBCC_JX2_EmitGetOffs(sctx);

		if(!simonly)
		{
			BGBCC_JX2_SetEndSimPass(sctx);
		}
		
		if(simonly)
			break;

//		sctx->dfl_dq=2;

#if 1
		if(!(sctx->dfl_dq&4))
		{
//			sctx->dfl_dq=0;
//			if(sctx->cnt_set_dq0>sctx->cnt_set_dq1)
			if(sctx->cnt_set_dq0>(sctx->cnt_set_dq1+10))
				sctx->dfl_dq=2;
//			if(sctx->cnt_set_dq1>sctx->cnt_set_dq0)
			if(sctx->cnt_set_dq1>(sctx->cnt_set_dq0+10))
				sctx->dfl_dq=3;
		}
#endif

//		co=BGBCC_JX2_EmitGetOffs(sctx);
		sz=co-bo;
		if(sz<msz)msz=sz;
		if(sz>nsz)nsz=sz;
		sctx->simfnsz=sz;
		sctx->simfnmsz=msz;
		sctx->simfnnsz=nsz;
		
		fnsz[np]=sz;
		fnsz_pro[np]=sctx->fnsz_pro;
		fnsz_epi[np]=sctx->fnsz_epi;
		fnsz_bod[np]=sctx->fnsz_bod;
		
		issta=1;
		if(sctx->reg_vsave!=sctx->reg_save)
			issta=0;
		if(sctx->freg_vsave!=sctx->freg_save)
			issta=0;
		if(co!=lo)
			issta=0;
		if(sctx->is_stable&2)
			issta=0;
		
		sctx->reg_vsave=sctx->reg_save;
		sctx->freg_vsave=sctx->freg_save;
		lo=co;

//		if(issta)
		if(issta && (np>2))
			break;

		sctx->need_farjmp=0;
		sctx->need_f16jmp=0;
		sctx->need_n12jmp=0;
		if(sctx->simfnsz>244)
			sctx->need_n12jmp=1;
		if(sctx->simfnnsz>3840)
			sctx->need_f16jmp=1;
		if(sctx->simfnnsz>30720)
			sctx->need_farjmp=1;
	}

	if(simonly)
		return(0);

	if(sctx->dfl_dq)
	{
		if(sctx->dfl_dq&1)
			sctx->stat_tot_dq1++;
		else
			sctx->stat_tot_dq0++;
	}else
	{
		sctx->stat_tot_dqi++;
	}
	
	sctx->is_stable=issta;
	sctx->simfnsz=sz;
	sctx->simfnmsz=msz;
	sctx->simfnnsz=nsz;

	if(sctx->cgen_log)
	{
		fprintf(sctx->cgen_log, "BGBCC_JX2C_BuildFunction: EndSim %s np=%d\n",
			obj->qname, np);
		fflush(sctx->cgen_log);
	}
#endif

	sctx->lbltrov=otrov;

	sz1=sctx->simfnnsz+((sctx->simfnnsz-sctx->simfnmsz)*2);
	sctx->simfnsz=sz1;
	
	sctx->need_farjmp=0;
	sctx->need_f16jmp=0;
	sctx->need_n12jmp=0;
//	if((obj->srctok*4)>=128)
//	if((obj->n_vop*6)>=128)
	if(sctx->simfnsz>244)
//	if(sctx->simfnnsz>120)
//	if(sctx->simfnnsz>60)
		sctx->need_n12jmp=1;
//	if((obj->srctok*4)>=2048)
//	if((obj->n_vop*6)>=2048)
	if(sctx->simfnsz>3840)
		sctx->need_f16jmp=1;
	if(sctx->simfnsz>30720)
		sctx->need_farjmp=1;
	
	BGBCC_JX2C_SetupFrameLayout(ctx, sctx, obj);

	BGBCC_JX2_SetSectionName(sctx, ".text");
	BGBCC_JX2_EmitBAlign(sctx, 4);
//	BGBCC_JX2_EmitBAlign(sctx, 16);

	bo1=BGBCC_JX2_EmitGetOffs(sctx);

//	BGBCC_JX2_EmitLabel(sctx, l0);
	BGBCC_JX2C_BuildFunctionBody(ctx, sctx, obj, l0);

#if 0
	BGBCC_JX2C_EmitFrameProlog(ctx, sctx, obj);
	
	for(i=0; i<obj->n_vtr; i++)
	{
		sctx->tr_trnum=i;
		BGBCC_JX2C_CompileVirtTr(ctx, sctx, obj, obj->vtr[i], i);
	}
	
	BGBCC_JX2C_EmitFrameEpilog(ctx, sctx, obj);
#endif

	co1=BGBCC_JX2_EmitGetOffs(sctx);
	sz1=co1-bo1;

	fnsz[np]=sz1;
	fnsz_pro[np]=sctx->fnsz_pro;
	fnsz_epi[np]=sctx->fnsz_epi;
	fnsz_bod[np]=sctx->fnsz_bod;

	if(sctx->reg_vsave!=sctx->reg_save)
		{ BGBCC_DBGBREAK }
	if(sctx->freg_vsave!=sctx->freg_save)
		{ BGBCC_DBGBREAK }

//	if(sctx->is_stable&2)
//		{ BGBCC_DBGBREAK }

	if(bo1!=bo)
		{ BGBCC_DBGBREAK }
		
	k=sz1-sz;
	if(k<0)k=-k;
//	if(k>64)
//	if(sz1<msz)
//		{ BGBCC_DBGBREAK }
//	if(sz1>nsz)

//	if(sz1>sctx->simfnsz)
//		{ BGBCC_DBGBREAK }

	if(sctx->cgen_log)
	{
		fprintf(sctx->cgen_log, "BGBCC_JX2C_BuildFunction: End %s\n",
			obj->qname);
		fflush(sctx->cgen_log);
	}

	return(0);
}

ccxl_status BGBCC_JX2C_BuildGlobal_EmitLitAsType(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register value)
{
	BGBCC_CCXL_LiteralInfo *litobj;
	BGBCC_CCXL_LiteralInfo *littyobj;
	BGBCC_CCXL_RegisterInfo *gblobj;
	ccxl_register treg;
	ccxl_type tty;
	s64 li, lj;
	double f, g;
	char *s0;
	int n, sz, sz1, al, asz, bty;
	int i, j, k;

	bty=BGBCC_CCXL_GetTypeBaseType(ctx, type);
	
	if(BGBCC_CCXL_TypeArrayP(ctx, type))
	{
		BGBCC_CCXL_TypeDerefType(ctx, type, &tty);

		if(BGBCC_CCXL_IsRegImmLiteralP(ctx, value))
		{
			i=value.val&CCXL_REGINT_MASK;
			litobj=ctx->literals[i];
//			tty=litobj->decl->type;
//			asz=BGBCC_CCXL_TypeGetArraySize(ctx, type);
			asz=BGBCC_CCXL_TypeGetArrayDimSize(ctx, type);

			al=BGBCC_CCXL_TypeGetLogicalAlign(ctx, tty);
			BGBCC_JX2_EmitBAlign(sctx, al);

			n=litobj->decl->n_listdata;
			if((asz>0) && (asz<n))
			{
				n=asz;
			}
				
			for(i=0; i<n; i++)
			{
				BGBCC_JX2_EmitBAlign(sctx, al);
				BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, litobj->decl->listdata[i]);
			}

			BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
			for(i=n; i<asz; i++)
			{
				BGBCC_JX2_EmitBAlign(sctx, al);
				BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value) &&
			BGBCC_CCXL_IsRegImmZeroP(ctx, value))
		{
//			tty=litobj->decl->type;
			asz=BGBCC_CCXL_TypeGetArraySize(ctx, type);
			al=BGBCC_CCXL_TypeGetLogicalAlign(ctx, tty);

			BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
			for(i=0; i<asz; i++)
			{
				BGBCC_JX2_EmitBAlign(sctx, al);
				BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmStringP(ctx, value))
		{
			s0=BGBCC_CCXL_GetRegImmStringValue(ctx, value);
			asz=BGBCC_CCXL_TypeGetArraySize(ctx, type);

//			j=strlen(s0);
			j=BGBCP_StrlenUTF8(s0);
			
			if(asz<j)j=asz;
			for(i=0; i<j; i++)
			{
				k=BGBCP_ParseChar(&s0);
				BGBCC_CCXL_GetRegForIntValue(ctx, &treg, k);
//				BGBCC_CCXL_GetRegForIntValue(ctx, &treg, s0[i]);
				BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}

			BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
			for(; i<asz; i++)
			{
				BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}
			if(asz<=0)
			{
				BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
	{
//		if(sctx->is_addr64)
		if(ctx->arch_sizeof_ptr==8)
			BGBCC_JX2_EmitBAlign(sctx, 8);
		else
			BGBCC_JX2_EmitBAlign(sctx, 4);

//		BGBCC_JX2_EmitBAlign(sctx, 4);

		if(BGBCC_CCXL_IsRegImmStringP(ctx, value))
		{
			s0=BGBCC_CCXL_GetRegImmStringValue(ctx, value);

			if(type.val==CCXL_VTY_PCHAR)
				k=BGBCC_JX2_EmitGetStrtabLabelUTF2ASCII(sctx, s0);
			if(type.val==CCXL_VTY_PWCHAR)
				k=BGBCC_JX2_EmitGetStrtabLabelUTF2UCS2(sctx, s0);				
			else
				k=BGBCC_JX2_EmitGetStrtabLabel(sctx, s0);

//			if(sctx->is_addr64)
			if(ctx->arch_sizeof_ptr==8)
			{
//				BGBCC_JX2_EmitRelocTy(sctx, k, BGBCC_SH_RLC_ABS64);
//				BGBCC_JX2_EmitQWord(sctx, 0);
				BGBCC_JX2_EmitQWordAbs64(sctx, k);
			}else
			{
//				BGBCC_JX2_EmitRelocTy(sctx, k, BGBCC_SH_RLC_ABS32);
//				BGBCC_JX2_EmitDWord(sctx, 0);
				BGBCC_JX2_EmitDWordAbs32(sctx, k);
			}
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, value);
//			if(sctx->is_addr64)
			if(ctx->arch_sizeof_ptr==8)
				{ BGBCC_JX2_EmitQWord(sctx, i); }
			else
				{ BGBCC_JX2_EmitDWord(sctx, i); }
			return(1);
		}
		
		if(BGBCC_CCXL_IsRegImmLongP(ctx, value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, value);
//			if(sctx->is_addr64)
			if(ctx->arch_sizeof_ptr==8)
				{ BGBCC_JX2_EmitQWord(sctx, li); }
			else
				{ BGBCC_JX2_EmitDWord(sctx, li); }
			return(1);
		}
		
		if(BGBCC_CCXL_IsRegImmGlobalAddrP(ctx, value))
		{
			i=value.val&CCXL_REGINT_MASK;
			j=(value.val>>32)&0xFFFFF;
			j=((s32)(j<<12))>>12;
//			gblobj=ctx->reg_globals[i];
			k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, i);
			if(ctx->arch_sizeof_ptr==8)
			{
//				BGBCC_JX2_EmitQWordAbs64(sctx, k);
				BGBCC_JX2_EmitQWordAbs64Disp(sctx, k, j);
			}else
			{
//				BGBCC_JX2_EmitDWordAbs32(sctx, k);
				BGBCC_JX2_EmitDWordAbs32Disp(sctx, k, j);
			}
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((bty==CCXL_TY_I) || (bty==CCXL_TY_UI) ||
		(((bty==CCXL_TY_NL) || (bty==CCXL_TY_UNL)) && !sctx->is_addr64))
	{
		BGBCC_JX2_EmitBAlign(sctx, 4);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, value);
			BGBCC_JX2_EmitDWord(sctx, i);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmFloatP(ctx, value) ||
			BGBCC_CCXL_IsRegImmDoubleP(ctx, value))
		{
//			i=BGBCC_CCXL_GetRegImmIntValue(ctx, value);
			i=BGBCC_CCXL_GetRegImmDoubleValue(ctx, value);
			BGBCC_JX2_EmitDWord(sctx, i);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((bty==CCXL_TY_L) || (bty==CCXL_TY_UL) ||
		(((bty==CCXL_TY_NL) || (bty==CCXL_TY_UNL)) && sctx->is_addr64))
	{
		if(sctx->is_addr64)
			BGBCC_JX2_EmitBAlign(sctx, 8);
		else
			BGBCC_JX2_EmitBAlign(sctx, 4);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, value);
			BGBCC_JX2_EmitQWord(sctx, li);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(bty==CCXL_TY_F)
	{
		BGBCC_JX2_EmitBAlign(sctx, 4);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, value);
			(*(float *)(&j))=li;
			BGBCC_JX2_EmitDWord(sctx, j);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmFloatP(ctx, value) ||
			BGBCC_CCXL_IsRegImmDoubleP(ctx, value))
		{
			f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, value);
			(*(float *)(&j))=f;
			BGBCC_JX2_EmitDWord(sctx, j);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(bty==CCXL_TY_D)
	{
		if(sctx->is_addr64)
			BGBCC_JX2_EmitBAlign(sctx, 8);
		else
			BGBCC_JX2_EmitBAlign(sctx, 4);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, value);
			(*(double *)(&lj))=li;
			BGBCC_JX2_EmitQWord(sctx, lj);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmFloatP(ctx, value) ||
			BGBCC_CCXL_IsRegImmDoubleP(ctx, value))
		{
			f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, value);
			(*(double *)(&lj))=f;
			BGBCC_JX2_EmitQWord(sctx, lj);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((bty==CCXL_TY_SB) || (bty==CCXL_TY_UB))
	{
//		BGBCC_JX2_EmitBAlign(sctx, 4);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, value);
			BGBCC_JX2_EmitByte(sctx, i);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((bty==CCXL_TY_SS) || (bty==CCXL_TY_US))
	{
		BGBCC_JX2_EmitBAlign(sctx, 2);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, value);
			BGBCC_JX2_EmitWord(sctx, i);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(BGBCC_CCXL_TypeSgInt128P(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmInt128P(ctx, value) ||
			BGBCC_CCXL_IsRegImmILFDP(ctx, value))
		{
			BGBCC_CCXL_GetRegImmInt128Value(ctx, value, &li, &lj);
			BGBCC_JX2_EmitQWord(sctx, li);
			BGBCC_JX2_EmitQWord(sctx, lj);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(BGBCC_CCXL_TypeFloat128P(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmFloat128P(ctx, value) ||
			BGBCC_CCXL_IsRegImmILFDP(ctx, value))
		{
			BGBCC_CCXL_GetRegImmFloat128Value(ctx, value, &li, &lj);
			BGBCC_JX2_EmitQWord(sctx, li);
			BGBCC_JX2_EmitQWord(sctx, lj);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmLiteralP(ctx, value))
		{
			i=value.val&CCXL_REGINT_MASK;
			litobj=ctx->literals[i];
//			tty=litobj->decl->type;
			tty=type;

			sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, tty);
			al=BGBCC_CCXL_TypeGetLogicalAlign(ctx, tty);
			BGBCC_JX2_EmitBAlign(sctx, al);

			littyobj=NULL;
			i=BGBCC_CCXL_GetTypeBaseType(ctx, tty);
			if(i>=256)
				{ littyobj=ctx->literals[i-256]; }

			if(littyobj && (littyobj->littype==CCXL_LITID_STRUCT))
//			if(littyobj &&
//				((littyobj->littype==CCXL_LITID_STRUCT) ||
//				 (littyobj->littype==CCXL_LITID_UNION)) )
			{
//				BGBCC_JX2_EmitBAlign(sctx, al);

				n=littyobj->decl->n_fields;
				if(litobj->decl->n_listdata<n)
					n=litobj->decl->n_listdata;
				
				for(i=0; i<n; i++)
				{
					BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
						littyobj->decl->fields[i]->type,
						litobj->decl->listdata[i]);
				}

				BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
				for(i=n; i<littyobj->decl->n_fields; i++)
				{
					BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
						littyobj->decl->fields[i]->type, treg);
				}

//				BGBCC_JX2_EmitBAlign(sctx, al);

				return(1);
			}

			if(littyobj && (littyobj->littype==CCXL_LITID_UNION) )
			{
//				BGBCC_JX2_EmitBAlign(sctx, al);

				n=littyobj->decl->n_fields;
				if(litobj->decl->n_listdata<n)
					n=litobj->decl->n_listdata;

				i=0;
				
//				for(i=0; i<n; i++)
				if(n)
				{
					BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
						littyobj->decl->fields[i]->type,
						litobj->decl->listdata[i]);
				}else
				{
					BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
					BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
						littyobj->decl->fields[i]->type, treg);
				}

				sz1=BGBCC_CCXL_TypeGetLogicalSize(ctx,
					littyobj->decl->fields[i]->type);
				if(sz>sz1)
				{
					BGBCC_JX2_EmitRawBytes(sctx, NULL, sz-sz1);
				}

//				BGBCC_JX2_EmitBAlign(sctx, al);

				return(1);
			}

			BGBCC_CCXL_StubError(ctx);
			return(0);
		}

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value) &&
			BGBCC_CCXL_IsRegImmZeroP(ctx, value))
		{
//			sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
			sz=BGBCC_CCXL_TypeGetLogicalPadSize(ctx, type);
			BGBCC_JX2_EmitRawBytes(sctx, NULL, sz);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

ccxl_status BGBCC_JX2C_BuildGlobal(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	BGBCC_JX2_Context *sctx;
	BGBCC_CCXL_LiteralInfo *litobj;
	BGBCC_CCXL_LiteralInfo *littyobj;
	ccxl_register treg;
	ccxl_type tty;
	char *s0;
	int l0, sz, al, al1, asz, iskv;
	s64 li;
	int n, hasval;
	int i, j, k;

	sctx=ctx->uctx;
	
	if(obj->regflags&BGBCC_REGFL_CULL)
		return(0);

	if(!obj->qname)
		obj->qname=obj->name;

	l0=obj->fxoffs;
	if(l0<=0)
	{
//		lbl=BGBCC_JX2_LookupNamedLabel(ctx, name);
//		l0=BGBCC_JX2_GenLabel(sctx);
		l0=BGBCC_JX2_GetNamedLabel(sctx, obj->qname);
		obj->fxoffs=l0;
	}
	
	if((l0>=BGBCC_SH_LBL_ARCHSTRT) && (l0<BGBCC_SH_LBL_ARCHEND))
	{
		/* This variable is a system register. */
		return(0);
	}
	
	hasval=1;

	if(!obj->value.val)
		hasval=0;
	if(obj->value.val==CCXL_REGTY_IMM_INT)
		hasval=0;
	if(obj->value.val==CCXL_REGTY_IMM_LONG)
		hasval=0;
	if(obj->value.val==CCXL_REGTY_IMM_FLOAT)
		hasval=0;
	if(obj->value.val==CCXL_REGTY_IMM_DOUBLE)
		hasval=0;
	
//	if(!obj->value.val)
	if(!hasval)
	{
//		if(!strcmp(obj->qname, "snd_scaletable"))
//			i=-1;
		
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->type);
		al=BGBCC_CCXL_TypeGetLogicalAlign(ctx, obj->type);

		if(al<1)al=1;
		if(sctx->is_addr64)
			{ if(al>8)al=8; }
		else
			{ if(al>4)al=4; }
		if(al&(al-1))al=4;

		if(sz<1)sz=1;
//		sz=(sz+3)&(~3);
		sz=(sz+(al-1))&(~(al-1));

		BGBCC_JX2_EmitCommSym(sctx, l0, sz);

//		BGBCC_JX2_SetSectionName(sctx, ".bss");
//		BGBCC_JX2_EmitBAlign(sctx, 4);
//		BGBCC_JX2_EmitLabel(sctx, l0);
//		BGBCC_JX2_EmitRawBytes(sctx, NULL, sz);
		return(1);
	}
	
	sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->type);
	al=BGBCC_CCXL_TypeGetLogicalAlign(ctx, obj->type);

	if(BGBCC_CCXL_TypeArrayP(ctx, obj->type))
	{
		BGBCC_CCXL_TypeDerefType(ctx, obj->type, &tty);
		al=BGBCC_CCXL_TypeGetLogicalAlign(ctx, tty);
	}

	if(al<1)al=1;
	if(sctx->is_addr64)
		{ if(al>8)al=8; }
	else
		{ if(al>4)al=4; }
//	if(al&(al-1))al=4;
	if(al&(al-1))al=8;

	iskv=0;

	if((obj->flagsint&BGBCC_TYFL_CONST) &&
		!BGBCC_CCXL_TypePointerP(ctx, obj->type))
			iskv=1;
	if((obj->flagsint&BGBCC_TYFL_CONST) &&
		BGBCC_CCXL_TypeArrayP(ctx, obj->type))
			iskv=1;
	if(obj->flagsint&BGBCC_TYFL_CONST2)
		iskv=1;

	if(iskv)
	{
		BGBCC_JX2_SetSectionName(sctx, ".rodata");
	}else
	{
		BGBCC_JX2_SetSectionName(sctx, ".data");
	}

//	BGBCC_JX2_EmitBAlign(sctx, 4);
	BGBCC_JX2_EmitBAlign(sctx, al);
	BGBCC_JX2_EmitLabel(sctx, l0);
	
	if(BGBCC_CCXL_TypeSmallIntP(ctx, obj->type))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, obj->value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, obj->value);
			BGBCC_JX2_EmitDWord(sctx, i);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmLongP(ctx, obj->value))
		{
			i=BGBCC_CCXL_GetRegImmLongValue(ctx, obj->value);
			BGBCC_JX2_EmitDWord(sctx, i);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmFloatP(ctx, obj->value) ||
			BGBCC_CCXL_IsRegImmDoubleP(ctx, obj->value))
		{
			i=BGBCC_CCXL_GetRegImmDoubleValue(ctx, obj->value);
			BGBCC_JX2_EmitDWord(sctx, i);
			return(1);
		}
	}

	if(BGBCC_CCXL_TypeSgLongP(ctx, obj->type) ||
		(BGBCC_CCXL_TypeSgNLongP(ctx, obj->type) && sctx->is_addr64))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, obj->value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, obj->value);
			BGBCC_JX2_EmitQWord(sctx, i);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmLongP(ctx, obj->value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, obj->value);
			BGBCC_JX2_EmitQWord(sctx, li);
			return(1);
		}
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, obj->type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, obj->type))
	{
		BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
			obj->type, obj->value);
		return(1);
	}

	if(BGBCC_CCXL_TypePointerP(ctx, obj->type))
	{
		if(BGBCC_CCXL_IsRegImmStringP(ctx, obj->value))
		{
			s0=BGBCC_CCXL_GetRegImmStringValue(ctx, obj->value);
			if(obj->type.val==CCXL_VTY_PCHAR)
				k=BGBCC_JX2_EmitGetStrtabLabelUTF2ASCII(sctx, s0);
			if(obj->type.val==CCXL_VTY_PWCHAR)
				k=BGBCC_JX2_EmitGetStrtabLabelUTF2UCS2(sctx, s0);				
			else
				k=BGBCC_JX2_EmitGetStrtabLabel(sctx, s0);
//			if(sctx->is_addr64)
			if(ctx->arch_sizeof_ptr==8)
			{
//				BGBCC_JX2_EmitRelocTy(sctx, k, BGBCC_SH_RLC_ABS64);
//				BGBCC_JX2_EmitQWord(sctx, 0);
				BGBCC_JX2_EmitQWordAbs64(sctx, k);
			}else
			{
//				BGBCC_JX2_EmitRelocTy(sctx, k, BGBCC_SH_RLC_ABS32);
//				BGBCC_JX2_EmitDWord(sctx, 0);
				BGBCC_JX2_EmitDWordAbs32(sctx, k);
			}
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmGlobalAddrP(ctx, obj->value))
		{
			i=obj->value.val&CCXL_REGINT_MASK;
			j=(obj->value.val>>32)&0xFFFFF;
			j=((s32)(j<<12))>>12;

			k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, i);

			if(ctx->arch_sizeof_ptr==8)
			{
//				BGBCC_JX2_EmitQWordAbs64(sctx, k);
				BGBCC_JX2_EmitQWordAbs64Disp(sctx, k, j);
			}else
			{
//				BGBCC_JX2_EmitDWordAbs32(sctx, k);
				BGBCC_JX2_EmitDWordAbs32Disp(sctx, k, j);
			}
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmIntP(ctx, obj->value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, obj->value);
//			if(sctx->is_addr64)
			if(ctx->arch_sizeof_ptr==8)
				{ BGBCC_JX2_EmitQWord(sctx, i); }
			else
				{ BGBCC_JX2_EmitDWord(sctx, i); }
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmLongP(ctx, obj->value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, obj->value);
//			if(sctx->is_addr64)
			if(ctx->arch_sizeof_ptr==8)
				{ BGBCC_JX2_EmitQWord(sctx, li); }
			else
				{ BGBCC_JX2_EmitDWord(sctx, li); }
			return(1);
		}
	}

	if(BGBCC_CCXL_IsRegImmLiteralP(ctx, obj->value))
	{
//		if(!strcmp(obj->qname, "keynames"))
//		{
//			k=-1;
//		}
	
		BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
			obj->type, obj->value);
		return(1);
	}

	if(BGBCC_CCXL_TypeArrayP(ctx, obj->type))
	{
		BGBCC_CCXL_TypeDerefType(ctx, obj->type, &tty);

//		al1=BGBCC_CCXL_TypeGetLogicalAlign(ctx, tty);

		if(BGBCC_CCXL_IsRegImmStringP(ctx, obj->value))
		{
			s0=BGBCC_CCXL_GetRegImmStringValue(ctx, obj->value);
			asz=BGBCC_CCXL_TypeGetArraySize(ctx, obj->type);
			
			if(asz<=0)
			{
				asz=strlen(s0)+1;
				asz=(asz+3)&(~3);
			}

//			j=strlen(s0);
			j=BGBCP_StrlenUTF8(s0);
			if((asz>0) && (asz<j))j=asz;
			for(i=0; i<j; i++)
			{
				k=BGBCP_ParseChar(&s0);
				BGBCC_CCXL_GetRegForIntValue(ctx, &treg, k);
				BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}
			
			BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
			for(; i<asz; i++)
			{
				BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}

			if(asz<=0)
			{
				BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}
			return(1);
		}
	}
	
	BGBCC_CCXL_StubError(ctx);
	BGBCC_JX2_EmitRawBytes(sctx, NULL, sz);
	return(1);
}

ccxl_status BGBCC_JX2C_BuildAsmBlob(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	BGBCC_JX2_Context *sctx;
	int l0, sz;
	int i, j, k;

	sctx=ctx->uctx;
	
	BGBCC_JX2C_AssembleBuffer(ctx, sctx, (char *)(obj->text));
	return(1);
}

ccxl_status BGBCC_JX2C_BuildPrestartInit(BGBCC_TransState *ctx)
{
	char tb[256];
	BGBCC_JX2_Context *sctx;
	char *s;
	int l0, l1, l2, l3, nm1;
	int i;

	sctx=ctx->uctx;

//	l0=BGBCC_JX2_GetNamedLabel(sctx, "__prestart_init");
	l0=BGBCC_JX2_GetNamedLabel(sctx, "__start_init");

	/* Object Init Stub */
	BGBCC_JX2_SetSectionName(sctx, ".text");
	BGBCC_JX2_EmitBAlign(sctx, 8);
	BGBCC_JX2_EmitLabel(sctx, l0);

	BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_PR);
	
	for(i=0; i<1024; i++)
	{
		sprintf(tb, "__prestart_init!%d", i);
		s=bgbcc_strdup(tb);

		l1=BGBCC_JX2_LookupNamedLabel(sctx, s);
		if(l1<=0)
			break;
		
		nm1=BGBCC_SH_NMID_BSRN;
		BGBCC_JX2_EmitOpAutoLabel(sctx, nm1, l1);

#if 0
		if(BGBCC_JX2_EmitCheckAutoLabelNear20B(sctx, l1, nm1))
		{
			BGBCC_JX2_EmitOpFar20Label(sctx, nm1, l1);
			continue;
		}
		if(BGBCC_JX2_EmitCheckAutoLabelNear24B(sctx, l1, nm1))
		{
			BGBCC_JX2_EmitOpFar24Label(sctx, nm1, l1);
			continue;
		}
#endif
	}
	
//	BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);
	BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RET);
	
	return(0);
}

ccxl_status BGBCC_JX2C_BuildStruct(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	char tb[256];
	int vt_lbl[1024];
	BGBCC_CCXL_RegisterInfo *vt_vmi[1024];

	BGBCC_CCXL_RegisterInfo *aclz[64];
	int tlbl[1024];

	int vif_lbl[256];
	BGBCC_CCXL_RegisterInfo *vif_vmi[256];

	BGBCC_JX2_Context *sctx;
	BGBCC_CCXL_LiteralInfo *li_sclz;
	BGBCC_CCXL_RegisterInfo *sclz, *cclz, *cif, *fi, *fj;
	int l0, l1, l2, l3;
	int l4, l5, l6, l7;
	int nvtix, naclz, nif;
	int i, j, k, l;

	sctx=ctx->uctx;

	if(!(obj->flagsint&BGBCC_TYFL_NONPOD))
	{
		return(1);
	}

	if(!obj->qname)
		obj->qname=obj->name;

	sclz=NULL;
	if(obj->n_args>0)
	{
		li_sclz=BGBCC_CCXL_LookupStructureForSig(ctx,
			obj->args[0]->sig);
		sclz=li_sclz->decl;
	}

#if 0
	l0=obj->fxoffs;
	if(l0<=0)
	{
//		lbl=BGBCC_JX2_LookupNamedLabel(ctx, name);
//		l0=BGBCC_JX2_GenLabel(sctx);
		l0=BGBCC_JX2_GetNamedLabel(sctx, obj->qname);
		obj->fxoffs=l0;
	}
#endif

	sprintf(tb, "%s/%s", obj->qname, "__vtable");
	l0=BGBCC_JX2_GetNamedLabel(sctx, tb);

	sprintf(tb, "%s/%s", obj->qname, "__init");
	l1=BGBCC_JX2_GetNamedLabel(sctx, tb);

	sprintf(tb, "%s/%s", obj->qname, "__cinfo");
	l2=BGBCC_JX2_GetNamedLabel(sctx, tb);

	l3=0;
	if(sclz)
	{
		sprintf(tb, "%s/%s", sclz->qname, "__cinfo");
		l3=BGBCC_JX2_GetNamedLabel(sctx, tb);
	}

	nvtix=obj->n_vargs;
	for(i=0; i<nvtix; i++)
	{
		vt_lbl[i]=0;
		vt_vmi[i]=NULL;
	}
	
	if(obj->regflags&BGBCC_REGFL_VARCONV)
	{
		vt_lbl[0]=l2;
	}

	naclz=0;
	cclz=obj;
	while(cclz)
	{
		aclz[naclz++]=cclz;

		for(i=0; i<cclz->n_regs; i++)
		{
			fi=cclz->regs[i];
			j=fi->fxmoffs;
			if(j<=0)
				continue;
			
			sprintf(tb, "%s/%s", cclz->qname, fi->name);
			k=BGBCC_JX2_GetNamedLabel(sctx, tb);
			if(vt_lbl[j]<=0)
				{ vt_lbl[j]=k; vt_vmi[j]=fi; }
		}

		if(cclz->n_args>0)
		{
			li_sclz=BGBCC_CCXL_LookupStructureForSig(ctx,
				cclz->args[0]->sig);
			cclz=li_sclz->decl;
		}else
		{
			cclz=NULL;
		}
	}


	BGBCC_JX2_SetSectionName(sctx, ".rodata");
//	BGBCC_JX2_EmitBAlign(sctx, 4);
	BGBCC_JX2_EmitBAlign(sctx, 8);

	/* Main VTable */

	BGBCC_JX2_EmitLabel(sctx, l0);
	for(i=0; i<nvtix; i++)
		{ BGBCC_JX2_EmitPtrWordAbs(sctx, vt_lbl[i]); }
	BGBCC_JX2_EmitPtrWordAbs(sctx, 0);



	if(obj->regflags&BGBCC_REGFL_VARCONV)
	{
		/* Class Info */

		l4=BGBCC_JX2_GenLabel(sctx);
		l5=BGBCC_JX2_GenLabel(sctx);
		l6=BGBCC_JX2_GenLabel(sctx);

		BGBCC_JX2_EmitLabel(sctx, l2);	
		k=BGBCC_JX2_EmitGetStrtabLabel(sctx, obj->qname);
		BGBCC_JX2_EmitDWordRva32(sctx, l2);		/* Self Pointer */
		BGBCC_JX2_EmitDWordRva32(sctx, k);		/* QName */
		BGBCC_JX2_EmitDWordRva32(sctx, l3);		/* Super */
		BGBCC_JX2_EmitDWordRva32(sctx, l4);		/* Fields */
		BGBCC_JX2_EmitDWordRva32(sctx, l5);		/* Methods */
		BGBCC_JX2_EmitDWordRva32(sctx, l6);		/* IFace */

		/* Build Fields List */
		for(i=0; i<obj->n_fields; i++)
		{
			k=BGBCC_JX2_GenLabel(sctx);
			tlbl[i]=k;

			BGBCC_JX2_EmitBAlign(sctx, 8);
			BGBCC_JX2_EmitLabel(sctx, k);

			fi=obj->fields[i];
			j=BGBCC_JX2_EmitGetStrtabLabel(sctx, fi->name);
			k=BGBCC_JX2_EmitGetStrtabLabel(sctx, fi->sig);
			BGBCC_JX2_EmitDWordRva32(sctx, j);		/* Name */
			BGBCC_JX2_EmitDWordRva32(sctx, k);		/* Sig */
	//		BGBCC_JX2_EmitQWord(sctx, fi->flagsint);
			BGBCC_JX2_EmitDWord(sctx, fi->flagsint);
			BGBCC_JX2_EmitDWord(sctx, fi->fxoffs);
	//		BGBCC_JX2_EmitDWord(sctx, 0);
		}

		BGBCC_JX2_EmitBAlign(sctx, 8);
		BGBCC_JX2_EmitLabel(sctx, l4);
		for(i=0; i<obj->n_fields; i++)
			{ BGBCC_JX2_EmitPtrWordAbs(sctx, tlbl[i]); }
		BGBCC_JX2_EmitPtrWordAbs(sctx, 0);


		/* Build Methods List */
		for(i=0; i<obj->n_regs; i++)
		{
			k=BGBCC_JX2_GenLabel(sctx);
			tlbl[i]=k;

			BGBCC_JX2_EmitBAlign(sctx, 8);
			BGBCC_JX2_EmitLabel(sctx, k);

			fi=obj->regs[i];
			j=BGBCC_JX2_EmitGetStrtabLabel(sctx, fi->name);
			k=BGBCC_JX2_EmitGetStrtabLabel(sctx, fi->sig);
			BGBCC_JX2_EmitDWordRva32(sctx, j);		/* Name */
			BGBCC_JX2_EmitDWordRva32(sctx, k);		/* Sig */
	//		BGBCC_JX2_EmitQWord(sctx, fi->flagsint);
			BGBCC_JX2_EmitDWord(sctx, fi->flagsint);
			BGBCC_JX2_EmitDWord(sctx, fi->fxoffs);
	//		BGBCC_JX2_EmitDWord(sctx, 0);
		}

		BGBCC_JX2_EmitBAlign(sctx, 8);
		BGBCC_JX2_EmitLabel(sctx, l5);
		for(i=0; i<obj->n_regs; i++)
			{ BGBCC_JX2_EmitDWordRva32(sctx, tlbl[i]); }
		BGBCC_JX2_EmitDWordRva32(sctx, 0);


		/* Build Interface List */
		for(i=1; i<obj->n_args; i++)
		{
			k=BGBCC_JX2_GenLabel(sctx);
			tlbl[i]=k;

			BGBCC_JX2_EmitBAlign(sctx, 8);
			BGBCC_JX2_EmitLabel(sctx, k);

			fi=obj->args[i];
			li_sclz=BGBCC_CCXL_LookupStructureForSig(ctx, fi->sig);
			cclz=li_sclz->decl;

			j=BGBCC_JX2_EmitGetStrtabLabel(sctx, cclz->qname);
	//		k=BGBCC_JX2_EmitGetStrtabLabel(sctx, fi->sig);

			sprintf(tb, "%s/%s", cclz->qname, "__cinfo");
			k=BGBCC_JX2_GetNamedLabel(sctx, tb);

			BGBCC_JX2_EmitDWordRva32(sctx, j);		/* Name */
			BGBCC_JX2_EmitDWordRva32(sctx, k);		/* Sig */
	//		BGBCC_JX2_EmitQWord(sctx, fi->flagsint);
			BGBCC_JX2_EmitDWord(sctx, fi->flagsint);
			BGBCC_JX2_EmitDWord(sctx, fi->fxoffs);
	//		BGBCC_JX2_EmitDWord(sctx, 0);
		}

		BGBCC_JX2_EmitBAlign(sctx, 8);
		BGBCC_JX2_EmitLabel(sctx, l6);
		for(i=1; i<obj->n_args; i++)
			{ BGBCC_JX2_EmitDWordRva32(sctx, tlbl[i]); }
		BGBCC_JX2_EmitDWordRva32(sctx, 0);
	}


	/* Build Per-Interface VTables */
	
//	viflbl
	nif=0;

	for(l=0; l<naclz; l++)
	{
		for(i=1; i<aclz[l]->n_args; i++)
		{
			cif=aclz[l]->args[i];
			li_sclz=BGBCC_CCXL_LookupStructureForSig(ctx, cif->sig);
			cclz=li_sclz->decl;

			k=nif++;
			l7=BGBCC_JX2_GenLabel(sctx);
			vif_lbl[k]=l7;
			vif_vmi[k]=cif;

			for(j=0; j<cclz->n_vargs; j++)
			{
				tlbl[j]=0;
			}
			
			for(j=0; j<cclz->n_regs; j++)
			{
				fi=cclz->regs[j];
				for(k=0; k<nvtix; k++)
				{
					fj=vt_vmi[k];
					if(!fj)continue;
					if(!strcmp(fi->name, fj->name))
						break;
				}
				
				if(k<nvtix)
					{ tlbl[fi->fxmoffs]=vt_lbl[k]; }
			}

			sprintf(tb, "%s/%s", cclz->qname, "__cinfo");
			k=BGBCC_JX2_GetNamedLabel(sctx, tb);
//			j=(-(cif->fxoffs))/ctx->arch_sizeof_ptr;
			if(ctx->arch_sizeof_ptr==8)
				j=(-(cif->fxoffs))>>3;
			else
				j=(-(cif->fxoffs))>>2;
			tlbl[0]=k;
//			tlbl[1]=CCXL_LBL_ABS16N|((-(cif->fxoffs))&0xFFFF);
//			tlbl[1]=CCXL_LBL_ABS16SP|(j&0xFFFF);
			tlbl[1]=CCXL_LBL_ABS16NP|(j&0xFFFF);

			BGBCC_JX2_EmitBAlign(sctx, 8);
			BGBCC_JX2_EmitLabel(sctx, l7);

			for(j=0; j<cclz->n_vargs; j++)
				{ BGBCC_JX2_EmitPtrWordAbs(sctx, tlbl[i]); }
			BGBCC_JX2_EmitPtrWordAbs(sctx, 0);
		}
	}


	/* Object Init Stub */
	BGBCC_JX2_SetSectionName(sctx, ".text");
//	BGBCC_JX2_EmitBAlign(sctx, 4);
	BGBCC_JX2_EmitBAlign(sctx, 8);
	BGBCC_JX2_EmitLabel(sctx, l1);
	
	if(ctx->arch_sizeof_ptr==8)
	{
		BGBCC_JX2_EmitLoadRegLabelRel24(sctx, BGBCC_SH_REG_R7, l0);
		BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
			BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R4, 0,
				BGBCC_SH_REG_R7);

		BGBCC_JX2C_EmitOpImmReg(ctx, sctx,
			BGBCC_SH_NMID_MOV, 0, BGBCC_SH_REG_R7);
		BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
			BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R4, 8,
				BGBCC_SH_REG_R7);
				
		for(i=0; i<nif; i++)
		{
			k=vif_lbl[k];
			fi=vif_vmi[k];

			BGBCC_JX2_EmitLoadRegLabelRel24(sctx, BGBCC_SH_REG_R7, k);
			BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
				BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R4, fi->fxoffs,
					BGBCC_SH_REG_R7);
		}
	}else
	{
		BGBCC_JX2_EmitLoadRegLabelRel24(sctx, BGBCC_SH_REG_R7, l0);
		BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
			BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R4, 0,
				BGBCC_SH_REG_R7);

		BGBCC_JX2C_EmitOpImmReg(ctx, sctx,
			BGBCC_SH_NMID_MOV, 0, BGBCC_SH_REG_R7);
		BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
			BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R4, 4,
				BGBCC_SH_REG_R7);

		for(i=0; i<nif; i++)
		{
			k=vif_lbl[k];
			fi=vif_vmi[k];

			BGBCC_JX2_EmitLoadRegLabelRel24(sctx, BGBCC_SH_REG_R7, k);
			BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
				BGBCC_SH_NMID_MOVL,
					BGBCC_SH_REG_R4, fi->fxoffs,
					BGBCC_SH_REG_R7);
		}
	}
	BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);
	
	return(0);
}


int BGBCC_JX2C_LookupLabelIndex(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int lblid)
{

	return(BGBCC_JX2_LookupLabelIndex(sctx, lblid));

#if 0
	int i, j, k;
	
	for(i=0; i<sctx->nlbl; i++)
	{
		if(sctx->lbl_id[i]==lblid)
			return(i);
	}
	return(-1);
#endif
}

int BGBCC_JX2C_LookupLabelImgOffs(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int lblid)
{
	int i, j, k;
	
	j=BGBCC_JX2C_LookupLabelIndex(ctx, sctx, lblid);
	k=sctx->sec_rva[sctx->lbl_sec[j]]+sctx->lbl_ofs[j];
	return(k);
}

int BGBCC_JX2C_LookupLabelImgVA(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int lblid)
{
	int i, j, k;
	
	
	j=BGBCC_JX2C_LookupLabelIndex(ctx, sctx, lblid);

	if(sctx->lbl_sec[j]==BGBCC_SH_CSEG_ABS)
		return(sctx->lbl_ofs[j]);

	k=sctx->sec_lva[sctx->lbl_sec[j]]+sctx->lbl_ofs[j];
	return(k);
}

extern char *bgbcc_shx_srcidx[256];
extern int bgbcc_shx_nsrcidx;

void bgbcc_jx2cc_setu16en(byte *ct, int en, u16 v)
{
//	if(((v>>12)&15)==0xD)
//	{
//		BGBCC_DBGBREAK
//	}
	bgbcc_setu16en(ct, en, v);
}

ccxl_status BGBCC_JX2C_ApplyImageRelocs(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	byte *imgbase)
{
	byte *ctl, *ctr, *gbr_base;
	char *s0;
	int en;
	s64 val, var;
	s64 b, d, b1, d1;
	int w0, w1, w2, w3;
	int i, j, k;

	en=(sctx->is_le==0);
	sctx->stat_ovlbl8=0;
	
	gbr_base=imgbase+sctx->gbr_rva;

	for(i=0; i<sctx->nrlc; i++)
	{
		j=BGBCC_JX2C_LookupLabelIndex(ctx, sctx, sctx->rlc_id[i]);
		if(j<0)
		{
			k=sctx->rlc_id[i];
			s0=BGBCC_JX2_LookupNameForLabel(sctx, k);
			if(s0)
			{
				printf("BGBCC_JX2C_ApplyImageRelocs: "
					"Missing Label N=%s\n", s0);
			}else
			{
				printf("BGBCC_JX2C_ApplyImageRelocs: "
					"Missing Label ID=%08X\n", k);
				if((k&(~65535))==CCXL_LBL_GENSYM2BASE)
				{
					b=k&65535;
					b1=sctx->genlabel_srcpos[b];
					printf("\t%s:%d\n",
						bgbcc_shx_srcidx[b1>>16], (u16)b1);
				}
			}
//			__debugbreak();
			continue;
		}

		if(sctx->lbl_sec[j]==BGBCC_SH_CSEG_ABS)
		{
			ctl=NULL;
			val=sctx->lbl_ofs[j];
		}else
		{
			ctl=imgbase+sctx->sec_rva[sctx->lbl_sec[j]]+sctx->lbl_ofs[j];
			val=sctx->sec_lva[sctx->lbl_sec[j]]+sctx->lbl_ofs[j];

			if((ctl<=imgbase) || (ctl>(imgbase+0x1000000)))
				__debugbreak();
			if(
//					(sctx->lbl_ofs[j]<0) ||
					(sctx->lbl_ofs[j] > sctx->sec_lsz[sctx->lbl_sec[j]]))
				__debugbreak();
		}

		ctr=imgbase+sctx->sec_rva[sctx->rlc_sec[i]]+sctx->rlc_ofs[i];
		var=sctx->sec_lva[sctx->rlc_sec[i]]+sctx->rlc_ofs[i];

		if((ctr<=imgbase) || (ctr>(imgbase+0x1000000)))
			__debugbreak();

//		d=ctl-ctr;
		d=val-var;
		switch(sctx->rlc_ty[i])
		{
		case BGBCC_SH_RLC_REL8:
			b=bgbcc_gets8en(ctr, en);
			d1=b+(d-1);
			if(((sbyte)d1)!=d1)
				__debugbreak();
			bgbcc_sets8en(ctr, en, d1);
			break;
		case BGBCC_SH_RLC_REL16:
			b=bgbcc_gets16en(ctr, en);
			d1=b+(d-2);
			if(((s16)d1)!=d1)
				__debugbreak();
			bgbcc_sets16en(ctr, en, d1);
			break;
		case BGBCC_SH_RLC_REL32:
			b=bgbcc_gets32en(ctr, en);
			bgbcc_sets32en(ctr, en, b+(d-4));
			break;
		case BGBCC_SH_RLC_REL32B:
			b=bgbcc_gets32en(ctr, en);
			bgbcc_sets32en(ctr, en, b-(d-4));
			break;

		case BGBCC_SH_RLC_ABS32:
//			d1=0x0C000000+(ctl-imgbase);
			d1=sctx->image_base+(ctl-imgbase);
			b=bgbcc_gets32en(ctr, en);
			bgbcc_sets32en(ctr, en, b+d1);
			break;

		case BGBCC_SH_RLC_ABS64:
//			d1=0x0C000000+(ctl-imgbase);
			d1=sctx->image_base+(ctl-imgbase);
			b=bgbcc_gets64en(ctr, en);
			bgbcc_sets64en(ctr, en, b+d1);
			break;

		case BGBCC_SH_RLC_RVA32:
			d1=(ctl-imgbase);
			b=bgbcc_gets32en(ctr, en);
			bgbcc_sets32en(ctr, en, b+d1);
			break;

		case BGBCC_SH_RLC_RELW12:
			b=bgbcc_getu16en(ctr, en);
			b1=((s32)(b<<20))>>20;
//			d1=b1+(d-4);
			d1=b1+((d-4)>>1);
			if((((s32)(d1<<20))>>20)!=d1)
				__debugbreak();
			bgbcc_jx2cc_setu16en(ctr, en, (b&0xF000)|(d1&0x0FFF));
			break;
		case BGBCC_SH_RLC_RELW8:
			b=bgbcc_getu16en(ctr, en);
			b1=((s32)(b<<24))>>24;
//			d1=b1+(d-4);
			d1=b1+((d-4)>>1);
			if((((s32)(d1<<24))>>24)!=d1)
				__debugbreak();
			bgbcc_jx2cc_setu16en(ctr, en, (b&0xFF00)|(d1&0x00FF));
			break;
		case BGBCC_SH_RLC_RELW12L:
			b=bgbcc_getu16en(ctr, en);
			b1=((s32)(b<<20))>>20;
//			d1=b1+(d-4);
			d1=b1+((d-2)>>2);
//			if(((d1<<20)>>20)!=d1)
			if((d1&4095)!=d1)
				__debugbreak();
			bgbcc_jx2cc_setu16en(ctr, en, (b&0xF000)|(d1&0x0FFF));
			break;
		case BGBCC_SH_RLC_RELW8L:
			b=bgbcc_getu16en(ctr, en);
			b1=((s32)(b<<24))>>24;
//			d1=b1+(d-4);
			d1=b1+((d-2)>>2);
//			if(((d1<<24)>>24)!=d1)
			if((d1&255)!=d1)
				__debugbreak();
			bgbcc_jx2cc_setu16en(ctr, en, (b&0xFF00)|(d1&0x00FF));
			break;

		case BGBCC_SH_RLC_RELW16_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			
//			b=(w0&255)|((w1&255)<<8);
			b=((w0&255)<<8)|(w1&255);
			b1=(b<<16)>>16;

			d1=b1+((d-4)>>1);
			if((((s32)(d1<<16))>>16)!=d1)
				__debugbreak();
				
			if(((((s32)(d1<<24))>>24)==d1) &&
				((w1&0xFF00)!=0x8300))
					sctx->stat_ovlbl8++;
				
			w0=(w0&0xFF00)|((d1>>8)&0x00FF);
			w1=(w1&0xFF00)|((d1   )&0x00FF);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_RELW20_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			
//			b=(w0&255)|((w1&255)<<8);
//			b=((w0&255)<<8)|(w1&255);
			b=((w0&255)<<12)|(w1&4095);
			b1=((s32)(b<<12))>>12;

			d1=b1+((d-4)>>1);
			if((((s32)(d1<<12))>>12)!=d1)
				__debugbreak();

			if((((s32)(d1<<24))>>24)==d1)
				sctx->stat_ovlbl8++;

			w0=(w0&0xFF00)|((d1>>12)&0x00FF);
			w1=(w1&0xF000)|((d1    )&0x0FFF);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_REL24_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			b=((w0&255)<<16)|(w1&65535);
			b1=((s32)(b<<8))>>8;
			d1=b1+(d-4);

			if((((s32)(d1<<24))>>24)==d1)
					sctx->stat_ovlbl8+=2;

			if((w0&0xFE00)==0xFA00)
			{
				if((((s32)(d1<<7))>>7)!=d1)
					__debugbreak();
				w0=(w0&0xFE00)|((d1>>16)&0x01FF);
			}else
			{
				if((((s32)(d1<<8))>>8)!=d1)
					__debugbreak();
				w0=(w0&0xFF00)|((d1>>16)&0x00FF);
			}
			w1=d1&0xFFFF;
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;
		case BGBCC_SH_RLC_REL24B_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			b=((w0&255)<<16)|(w1&65535);
			b1=((s32)(b<<8))>>8;
			d1=b1-(d-4);

			if((((s32)(d1<<24))>>24)==d1)
					sctx->stat_ovlbl8+=2;

			if((w0&0xFE00)==0xFA00)
			{
				if((((s32)(d1<<7))>>7)!=d1)
					__debugbreak();
				w0=(w0&0xFE00)|((d1>>16)&0x01FF);
			}else
			{
				if((((s32)(d1<<8))>>8)!=d1)
					__debugbreak();
				w0=(w0&0xFF00)|((d1>>16)&0x00FF);
			}
//			if((((s32)(d1<<8))>>8)!=d1)
//				__debugbreak();
//			w0=(w0&0xFF00)|((d1>>16)&0x00FF);
			w1=d1&0xFFFF;
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_RELW24_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			b=((w0&255)<<16)|(w1&65535);
			b1=((s32)(b<<8))>>8;
			d1=b1+((d-4)>>1);

			if((((s32)(d1<<24))>>24)==d1)
					sctx->stat_ovlbl8+=2;

			if((w0&0xFE00)==0xFA00)
			{
				if((((s32)(d1<<7))>>7)!=d1)
					__debugbreak();
				w0=(w0&0xFE00)|((d1>>16)&0x01FF);
			}else
			{
				if((((s32)(d1<<8))>>8)!=d1)
					__debugbreak();
				w0=(w0&0xFF00)|((d1>>16)&0x00FF);
			}
			w1=d1&0xFFFF;
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_PBO24_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			b=((w0&255)<<16)|(w1&65535);
			b1=((s32)(b<<8))>>8;
			d1=b1+(ctl-gbr_base);

			if(d1<0)
				{ BGBCC_DBGBREAK }
			

//			if(b1!=0)
//				{ BGBCC_DBGBREAK }

			if((w0&0xFE00)==0xFA00)
			{
				if((((s32)(d1<<7))>>7)!=d1)
					{ BGBCC_DBGBREAK }
				w0=(w0&0xFE00)|((d1>>16)&0x01FF);
			}else
			{
				BGBCC_DBGBREAK

				if((((s32)(d1<<8))>>8)!=d1)
					{ BGBCC_DBGBREAK }
				w0=(w0&0xFF00)|((d1>>16)&0x00FF);
			}
			w1=d1&0xFFFF;
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_TBR24_BJX:
			break;
		case BGBCC_SH_RLC_TBR12_BJX:
			break;

		case BGBCC_SH_RLC_RELW8_BSR:
			w0=bgbcc_getu16en(ctr, en);
			b1=((s32)(w0<<24))>>24;
//			d1=b1+(d-4);
			d1=b1+((d-2)>>1);
			if((((s32)(d1<<24))>>24)!=d1)
				__debugbreak();
			if((w0&0xFE00)==0x2400)
				w0=(w0&0xFE00)|(d1&0x01FF);
			else
				w0=(w0&0xFF00)|(d1&0x00FF);
//			if(((w0&0xFF00)==0x2400) && (d1&0x100))w0^=0x0100;
			bgbcc_jx2cc_setu16en(ctr, en, w0);
			break;

		case BGBCC_SH_RLC_RELW12_BSR:
			b=bgbcc_getu16en(ctr, en);
			b1=((s32)(b<<20))>>20;
			d1=b1+((d-2)>>1);
			if((((s32)(d1<<20))>>20)!=d1)
				__debugbreak();
			bgbcc_jx2cc_setu16en(ctr, en, (b&0xF000)|(d1&0x0FFF));
			break;

		case BGBCC_SH_RLC_RELW16_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			b=((w0&255)<<8)|(w1&255);
			b1=((s32)(b<<16))>>16;
			d1=b1+((d-4)>>1);
//			if((((s32)(d1<<16))>>16)!=d1)
//				__debugbreak();
			if((((s32)(d1<<24))>>24)==d1)
					sctx->stat_ovlbl8++;
			if((w0&0xFE00)==0x2400)
			{
				if((((s32)(d1<<15))>>15)!=d1)
					__debugbreak();
				w0=(w0&0xFE00)|((d1>>8)&0x01FF);
				w1=(w1&0xFF00)|((d1   )&0x00FF);
			}
			else if((w1&0xFE00)==0x2400)
			{
				if((((s32)(d1<<15))>>15)!=d1)
					__debugbreak();
				w0=(w0&0xFF00)|((d1>>8)&0x00FF);
				w1=(w1&0xFE00)|((d1   )&0x00FF)|
					((d1>>16)&0x0100);
			}
			else
			{
				if((((s32)(d1<<16))>>16)!=d1)
					__debugbreak();
				w0=(w0&0xFF00)|((d1>>8)&0x00FF);
				w1=(w1&0xFF00)|((d1   )&0x00FF);
			}
//			if(((w0&0xFF00)==0x2400) && (d1&0x10000))w0^=0x0100;
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_RELW16C_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			b=((w0&255)<<8)|(w1&255);
			b1=((s32)(b<<16))>>16;
			d1=b1+(d-4);
//			if((((s32)(d1<<16))>>16)!=d1)
//				__debugbreak();
			if((w0&0xFE00)==0x2400)
			{
				if((((s32)(d1<<15))>>15)!=d1)
					__debugbreak();
				w0=(w0&0xFE00)|((d1>>8)&0x01FF);
				w1=(w1&0xFF00)|((d1   )&0x00FF);
			}
			else
				if((w1&0xFE00)==0x2400)
			{
				if((((s32)(d1<<15))>>15)!=d1)
					__debugbreak();
				w0=(w0&0xFF00)|((d1>>8)&0x00FF);
				w1=(w1&0xFE00)|((d1   )&0x00FF)|
					((d1>>16)&0x0100);
			}
			else
			{
				if((((s32)(d1<<16))>>16)!=d1)
					__debugbreak();
				w0=(w0&0xFF00)|((d1>>8)&0x00FF);
				w1=(w1&0xFF00)|((d1   )&0x00FF);
			}
//			if(((w0&0xFF00)==0x2400) && (d1&0x10000))w0^=0x0100;
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_RELW16D_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			b=((w0&4095)<<4)|(w1&15);
			b1=((s32)(b<<16))>>16;
			d1=b1+((d-4)>>1);
			if((((s32)(d1<<16))>>16)!=d1)
				__debugbreak();
			if((((s32)(d1<<24))>>24)==d1)
					sctx->stat_ovlbl8++;
			if((w0&0xE000)==0xA000)
				w0=(w0&0xE000)|((d1>>4)&0x1FFF);
			else
				w0=(w0&0xF000)|((d1>>4)&0x0FFF);
			w1=(w1&0xFFF0)|((d1   )&0x000F);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_RELW16A_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			b=((w0&4095)<<4)|(w1&15);
			b1=((s32)(b<<16))>>16;
			d1=b1+(d-4);
			if((((s32)(d1<<15))>>15)!=d1)
				__debugbreak();
			if((((s32)(d1<<24))>>24)==d1)
					sctx->stat_ovlbl8++;
			w0=(w0&0xE000)|((d1>>4)&0x1FFF);
			w1=(w1&0xFFF0)|((d1   )&0x000F);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_RELW24_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			b=((w0&255)<<16)|((w1&255)<<8)|(w2&255);
			b1=((s32)(b<<8))>>8;
			d1=b1+((d-6)>>1);
			if((((s32)(d1<<8))>>8)!=d1)
				__debugbreak();
			if((((s32)(d1<<24))>>24)==d1)
					sctx->stat_ovlbl8++;
			if((w0&0xFE00)==0x2400)
				w0=(w0&0xFE00)|((d1>>16)&0x01FF);
			else
				w0=(w0&0xFF00)|((d1>>16)&0x00FF);
			w1=(w1&0xFF00)|((d1>> 8)&0x00FF);
			w2=(w2&0xFF00)|((d1    )&0x00FF);
//			if(((w0&0xFF00)==0x2400) && (d1&0x1000000))w0^=0x0100;
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			break;

		case BGBCC_SH_RLC_RELW24C_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			b=((w0&255)<<16)|((w1&255)<<8)|(w2&255);
			b1=((s32)(b<<8))>>8;
			d1=b1+(d-6);
			if((((s32)(d1<<8))>>8)!=d1)
				__debugbreak();
			if((w0&0xFE00)==0x2400)
				w0=(w0&0xFE00)|((d1>>16)&0x01FF);
			else
				w0=(w0&0xFF00)|((d1>>16)&0x00FF);
			w1=(w1&0xFF00)|((d1>> 8)&0x00FF);
			w2=(w2&0xFF00)|((d1    )&0x00FF);
//			if(((w0&0xFF00)==0x2400) && (d1&0x1000000))w0^=0x0100;
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			break;
		case BGBCC_SH_RLC_RELW32C_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			w3=bgbcc_getu16en(ctr+6, en);
			b=((w0&255)<<24)|((w1&255)<<16)|((w2&255)<<8)|(w3&255);
			b1=(s32)b;
			d1=b1+(d-8);
			if(((s32)d1)!=d1)
				__debugbreak();
			if((w0&0xFE00)==0x2400)
				w0=(w0&0xFE00)|((d1>>32)&0x01FF);
			else
				w0=(w0&0xFF00)|((d1>>32)&0x00FF);
			w1=(w1&0xFF00)|((d1>>16)&0x00FF);
			w2=(w2&0xFF00)|((d1>> 8)&0x00FF);
			w3=(w3&0xFF00)|((d1    )&0x00FF);
//			if(((w0&0xFF00)==0x2400) && (d1&0x1000000))w0^=0x0100;
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			bgbcc_jx2cc_setu16en(ctr+6, en, w3);
			break;

		case BGBCC_SH_RLC_RELW24D_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			b=((w0&4095)<<12)|((w1&255)<<4)|(w2&15);
			b1=((s32)(b<<8))>>8;
			d1=b1+((d-6)>>1);
			if((((s32)(d1<<8))>>8)!=d1)
				__debugbreak();
			if((((s32)(d1<<24))>>24)==d1)
					sctx->stat_ovlbl8++;
			w0=(w0&0xE000)|((d1>>12)&0x1FFF);
			w1=(w1&0xFF00)|((d1>> 4)&0x00FF);
			w2=(w2&0xFFF0)|((d1    )&0x000F);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			break;
		case BGBCC_SH_RLC_RELW32D_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			w3=bgbcc_getu16en(ctr+6, en);
			b=((w0&4095)<<20)|((w1&255)<<12)|((w2&255)<<4)|(w3&15);
//			b1=((s32)(b<<8))>>8;
			b1=(s32)b;
			d1=b1+((d-8)>>1);
//			if((((s32)(d1<<8))>>8)!=d1)
//				__debugbreak();
			if((((s32)(d1<<24))>>24)==d1)
					sctx->stat_ovlbl8++;
			w0=(w0&0xE000)|((d1>>20)&0x1FFF);
			w1=(w1&0xFF00)|((d1>>12)&0x00FF);
			w2=(w2&0xFF00)|((d1>> 4)&0x00FF);
			w3=(w3&0xFFF0)|((d1    )&0x000F);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			bgbcc_jx2cc_setu16en(ctr+6, en, w3);
			break;

		case BGBCC_SH_RLC_RELW24A_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			b=((w0&4095)<<12)|((w1&255)<<4)|(w2&15);
			b1=((s32)(b<<8))>>8;
			d1=b1+(d-6);
			if((((s32)(d1<<8))>>8)!=d1)
				__debugbreak();
			if((((s32)(d1<<24))>>24)==d1)
					sctx->stat_ovlbl8++;
			w0=(w0&0xE000)|((d1>>12)&0x1FFF);
			w1=(w1&0xFF00)|((d1>> 4)&0x00FF);
			w2=(w2&0xFFF0)|((d1    )&0x000F);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			break;
		case BGBCC_SH_RLC_RELW32A_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			w3=bgbcc_getu16en(ctr+6, en);
			b=((w0&4095)<<20)|((w1&255)<<12)|((w2&255)<<4)|(w3&15);
//			b1=((s32)(b<<8))>>8;
			b1=(s32)b;
			d1=b1+(d-8);
//			if((((s32)(d1<<8))>>8)!=d1)
//				__debugbreak();
			if((((s32)(d1<<24))>>24)==d1)
					sctx->stat_ovlbl8++;
			w0=(w0&0xE000)|((d1>>20)&0x1FFF);
			w1=(w1&0xFF00)|((d1>>12)&0x00FF);
			w1=(w1&0xFF00)|((d1>> 4)&0x00FF);
			w2=(w2&0xFFF0)|((d1    )&0x000F);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			bgbcc_jx2cc_setu16en(ctr+6, en, w2);
			break;

		case BGBCC_SH_RLC_ABSW16A_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			b=((w0&255)<<8)|(w1&255);
//			b1=sctx->image_base+(ctl-imgbase);
			b1=val;
			d1=b+b1;
			if((d1&0xFFFF)!=d1)
				__debugbreak();
			w0=(w0&0xFF00)|((d1>>8)&0x00FF);
			w1=(w1&0xFF00)|((d1   )&0x00FF);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_ABSW24A_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			b=((w0&255)<<16)|((w1&255)<<8)|(w2&255);
//			b1=sctx->image_base+(ctl-imgbase);
			b1=val;
			d1=b+b1;
			if((d1&0xFFFFFF)!=d1)
				__debugbreak();
			w0=(w0&0xFF00)|((d1>>16)&0x00FF);
			w1=(w1&0xFF00)|((d1>> 8)&0x00FF);
			w2=(w2&0xFF00)|((d1    )&0x00FF);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			break;

		case BGBCC_SH_RLC_ABSW32A_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			w3=bgbcc_getu16en(ctr+6, en);
			b=((w0&255)<<24)|((w1&255)<<16)|((w2&255)<<8)|(w3&255);
//			b1=sctx->image_base+(ctl-imgbase);
			b1=val;
			d1=b+b1;
			w0=(w0&0xFF00)|((d1>>24)&0x00FF);
			w1=(w1&0xFF00)|((d1>>16)&0x00FF);
			w2=(w2&0xFF00)|((d1>> 8)&0x00FF);
			w3=(w3&0xFF00)|((d1    )&0x00FF);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			bgbcc_jx2cc_setu16en(ctr+6, en, w3);
			break;

		case BGBCC_SH_RLC_ABSW16B_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			b=((w0&4095)<<4)|(w1&15);
//			b1=sctx->image_base+(ctl-imgbase);
			b1=val;
			d1=b+b1;
			if((d1&0xFFFF)!=d1)
				__debugbreak();
			w0=(w0&0xF000)|((d1>>4)&0x0FFF);
			w1=(w1&0xFFF0)|((d1   )&0x000F);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;
		case BGBCC_SH_RLC_ABSW24B_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			b=((w0&4095)<<12)|((w1&255)<<4)|(w2&15);
//			b1=sctx->image_base+(ctl-imgbase);
			b1=val;
			d1=b+b1;
			if((d1&0xFFFFFF)!=d1)
				__debugbreak();
			w0=(w0&0xF000)|((d1>>12)&0x0FFF);
			w1=(w1&0xFF00)|((d1>> 4)&0x00FF);
			w2=(w2&0xFFF0)|((d1    )&0x000F);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			break;
		case BGBCC_SH_RLC_ABSW32B_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			w3=bgbcc_getu16en(ctr+6, en);
			b=((w0&4095)<<20)|((w1&255)<<12)|((w2&255)<<4)|(w3&15);
//			b1=sctx->image_base+(ctl-imgbase);
			b1=val;
			d1=b+b1;
//			if((d1&0xFFFFFF)!=d1)
//				__debugbreak();
			w0=(w0&0xF000)|((d1>>20)&0x0FFF);
			w1=(w1&0xFF00)|((d1>>12)&0x00FF);
			w2=(w2&0xFF00)|((d1>> 4)&0x00FF);
			w3=(w3&0xFFF0)|((d1    )&0x000F);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			bgbcc_jx2cc_setu16en(ctr+6, en, w3);
			break;

		default:
			__debugbreak();
		}
	}
	
	if(sctx->stat_ovlbl8>0)
	{
		printf("Overlong Branches %d, EstCost=%dB\n",
			sctx->stat_ovlbl8, sctx->stat_ovlbl8*2);
	}
	
	return(0);
}

ccxl_status BGBCC_JX2C_FlattenImage(BGBCC_TransState *ctx,
	byte *obuf, int *rosz, fourcc imgfmt)
{
	BGBCC_JX2_Context *sctx;
	BGBCC_CCXL_RegisterInfo *obj;
	BGBCC_CCXL_LiteralInfo *litobj;
	int *shufarr;
	int t0, t1, t2, t3;
	int l0;
	int shufgbl;
	u64 h;
	u32 addr;
	int i, j, k, l, n;

	sctx=ctx->uctx;
	
	sctx->stat_tot_dq0=0;
	sctx->stat_tot_dq1=0;
	sctx->stat_tot_dqi=0;

//	if(!sctx->cgen_log)
//		sctx->cgen_log=fopen("bgbcc_shxlog.txt", "wt");

	sctx->do_asm=0;
	if(imgfmt==BGBCC_IMGFMT_ASM)
	{
		sctx->do_asm=1;
//		sctx->is_pbo=0;
	}


	sctx->is_rom=0;
	if(imgfmt==BGBCC_IMGFMT_ROM)
	{
		sctx->is_pbo=0;		//PBO is N/A for ROM images.
		sctx->is_rom=1;
		sctx->lbl_rom_data_strt=
			BGBCC_JX2_GetNamedLabel(sctx, "__rom_data_start");
		sctx->lbl_rom_data_end=
			BGBCC_JX2_GetNamedLabel(sctx, "__rom_data_end");

		if(!sctx->use_memmdl)
		{
			sctx->use_memmdl=BGBCC_MEMMDL_TINY16;
//			sctx->use_memmdl=BGBCC_MEMMDL_MEDIUM24;
		}
	}else
	{
		if(!sctx->use_memmdl)
		{
			sctx->use_memmdl=BGBCC_MEMMDL_MEDIUM32;
//			sctx->use_memmdl=BGBCC_MEMMDL_MEDIUM24;
		}
	}

	BGBCC_JX2_SetSectionName(sctx, ".text");
	BGBCC_JX2_EmitNamedLabel(sctx, "__text_start");

	BGBCC_JX2_SetSectionName(sctx, ".data");
	BGBCC_JX2_EmitNamedLabel(sctx, "__data_start");

	if(sctx->is_pbo)
	{
		/*
		 * Emit pointer to self at the start of ".data"
		 * This self-pointer needs to be at the start of the PBO data area.
		 */
	
		BGBCC_JX2_EmitNamedLabel(sctx, "__global_ptr");
		j=BGBCC_JX2_GetNamedLabel(sctx, "__global_ptr");
		BGBCC_JX2_EmitQWordAbs64(sctx, j);
//		BGBCC_JX2_EmitQWord(sctx, 0);

		sctx->lbl_gbl_ptr=j;

//		/* Reserve space for pointers to other images. */
//		for(i=0; i<127; i++)
//		{
//			BGBCC_JX2_EmitQWord(sctx, 0);
//		}

		/* Change: Pointers table will have negative offsets relative to GBR. */

	}

	BGBCC_JX2_SetSectionName(sctx, ".bss");
	BGBCC_JX2_EmitNamedLabel(sctx, "__bss_start");
	
	sctx->lbl_got=BGBCC_JX2_GenLabel(sctx);

	for(i=0; i<ctx->n_literals; i++)
	{
		litobj=ctx->literals[i];
		BGBCC_CCXL_FixupObjSize(ctx, litobj, 0);
	}

	for(i=0; i<ctx->n_literals; i++)
	{
		litobj=ctx->literals[i];
		BGBCC_CCXL_FixupObjSize(ctx, litobj, 1);
	}

	for(i=0; i<ctx->n_literals; i++)
	{
		litobj=ctx->literals[i];
		BGBCC_CCXL_SanityObjSize(ctx, litobj, 1);
	}

	h=clock();
	
	t0=clock();
	t1=t0;
	while(t1<(t0+(0.1*CLOCKS_PER_SEC)))
	{
		t2=clock();
		t3=t2-t1;
		t1=t2;
		h=(h*65521)+t1;
	}

	for(i=0; i<ctx->n_reg_globals; i++)
	{
//		j=shufarr[i];
//		obj=ctx->reg_globals[j];

		obj=ctx->reg_globals[i];
		if(!obj)
			continue;
		
		if(obj->flagsint&BGBCC_TYFL_DLLEXPORT)
		{
			BGBCC_CCXL_GlobalMarkReachable(ctx, obj);
			continue;
		}

//		if(obj->regtype==CCXL_LITID_FUNCTION)
		if(1)
		{
			if(obj->regflags&BGBCC_REGFL_ACCESSED)
				BGBCC_CCXL_GlobalMarkReachable(ctx, obj);
//			continue;
		}

		if(obj->name)
		{
			if(!strncmp(obj->name, "__", 2))
			{
				if(!(ctx->ccxl_tyc_seen&BGBCC_TYCSEEN_VARIANT))
				{
					if(!strncmp(obj->name, "__lva_", 6))
						continue;
				}

				if(!(ctx->ccxl_tyc_seen&BGBCC_TYCSEEN_INT128))
				{
					if(!strncmp(obj->name, "__xli_", 6))
						continue;
				}

				if(!(ctx->ccxl_tyc_seen&BGBCC_TYCSEEN_FLOAT128))
				{
					if(!strncmp(obj->name, "__xlf_", 6))
						continue;
				}

				if(!(ctx->ccxl_tyc_seen&BGBCC_TYCSEEN_FLOAT_FPU))
				{
					if(!strncmp(obj->name, "__sfp_", 6))
						continue;
					if(!strncmp(obj->name, "__lfp_", 6))
						continue;
				}
				
				if(!(sctx->fpu_soft) && !(sctx->fpu_lite))
				{
					if(!strncmp(obj->name, "__sfp_", 6))
						continue;
				}

				if(!(sctx->fpu_lite))
				{
					if(!strncmp(obj->name, "__lfp_", 6))
						continue;
				}

				if(!(ctx->ccxl_tyc_seen&BGBCC_TYCSEEN_IDIV_VAR))
				{
					if(!strcmp(obj->name, "__udivsi3"))
						continue;
					if(!strcmp(obj->name, "__sdivsi3"))
						continue;
				}
				
				if(!(ctx->ccxl_tyc_seen&BGBCC_TYCSEEN_IMOD_VAR))
				{
					if(!strcmp(obj->name, "__umodsi3"))
						continue;
					if(!strcmp(obj->name, "__smodsi3"))
						continue;
				}

				if(1)
				{
					/* called by frontend, no free pass. */
					if(!strncmp(obj->name, "__lvo_", 6))
						continue;
				}

				if(!strcmp(obj->name, "__start"))
				{
					obj->regflags|=BGBCC_REGFL_ALIASPTR;
				}
				
				BGBCC_CCXL_GlobalMarkReachable(ctx, obj);
				continue;
			}

			if(!strcmp(obj->name, "main"))
			{
				obj->regflags|=BGBCC_REGFL_ALIASPTR;
				BGBCC_CCXL_GlobalMarkReachable(ctx, obj);
				continue;
			}
		}
	}

	shufgbl=ctx->n_reg_globals;
	shufarr=malloc(shufgbl*sizeof(int));
	for(i=0; i<shufgbl; i++)
		shufarr[i]=i;

#if 0
	for(i=0; i<shufgbl; i++)
	{
		j=(((int)(h>>32))&0x0FFFFFFF)%(shufgbl);
		h=h*65521+13;
		k=shufarr[i];
		shufarr[i]=shufarr[j];
		shufarr[j]=k;
	}
#endif

	for(i=0; i<ctx->n_reg_globals; i++)
	{
//		j=shufarr[i];
//		obj=ctx->reg_globals[j];

		obj=ctx->reg_globals[i];
		if(!obj)
			continue;

		if(obj->regtype==CCXL_LITID_FUNCTION)
		{
			if(!(obj->regflags&BGBCC_REGFL_ACCESSED))
				{ obj->regflags|=BGBCC_REGFL_CULL; }
			continue;
		}

		if(obj->regtype==CCXL_LITID_GLOBALVAR)
		{
			if(!(obj->regflags&BGBCC_REGFL_ACCESSED))
				{ obj->regflags|=BGBCC_REGFL_CULL; }
			continue;
		}

		if(obj->regtype==CCXL_LITID_STATICVAR)
		{
			if(!(obj->regflags&BGBCC_REGFL_ACCESSED))
				{ obj->regflags|=BGBCC_REGFL_CULL; }
			continue;
		}
	}

	sctx->need_n16bsr=1;
	sctx->need_n20bsr=1;
	sctx->need_n24bsr=0;

	sctx->need_n16dat=1;
	sctx->need_n20dat=1;
	sctx->need_n24dat=0;

	BGBCC_JX2_SetBeginSimPass(sctx);
	sctx->is_simpass=64;

	for(i=0; i<ctx->n_reg_globals; i++)
	{
		if(i<shufgbl)
			j=shufarr[i];
		else
			j=i;
		obj=ctx->reg_globals[j];

//		obj=ctx->reg_globals[i];
		if(!obj)
			continue;

		if((obj->regtype==CCXL_LITID_FUNCTION) && (obj->vtr))
		{
			BGBCC_JX2C_BuildFunction(ctx, obj);
			continue;
		}

		if(obj->regtype==CCXL_LITID_GLOBALVAR)
		{
			BGBCC_JX2C_BuildGlobal(ctx, obj);
			continue;
		}

		if(obj->regtype==CCXL_LITID_STATICVAR)
		{
			BGBCC_JX2C_BuildGlobal(ctx, obj);
			continue;
		}

#if 0
		if(obj->regtype==CCXL_LITID_ASMBLOB)
		{
			BGBCC_JX2C_BuildAsmBlob(ctx, obj);
			continue;
		}

		if((obj->regtype==CCXL_LITID_STRUCT) ||
			(obj->regtype==CCXL_LITID_CLASS))
		{
			BGBCC_JX2C_BuildStruct(ctx, obj);
			continue;
		}
#endif
	}
	
	BGBCC_JX2_SetSectionName(sctx, ".text");
	k=BGBCC_JX2_EmitGetOffs(sctx);
	j=k;
	sctx->sim_txtsz=k;

	BGBCC_JX2_SetSectionName(sctx, ".data");
	k=BGBCC_JX2_EmitGetOffs(sctx);
	j+=k;
	sctx->sim_datsz=k;

	BGBCC_JX2_SetSectionName(sctx, ".bss");
	k=BGBCC_JX2_EmitGetOffs(sctx);
	j+=k;
	sctx->sim_bsssz=k;

	BGBCC_JX2_SetSectionName(sctx, ".rodata");
	k=BGBCC_JX2_EmitGetOffs(sctx);
	j+=k;
	sctx->sim_datsz+=k;

	sctx->simimgsz=j;

	BGBCC_JX2_SetEndSimPass(sctx);
//	sctx->nlbl=0;
//	sctx->nvlbl=0;
	sctx->nvlbl=sctx->nlbl;

	printf("Sim .text %d\n", sctx->sim_txtsz);
	printf("Sim .data %d\n", sctx->sim_datsz);
	printf("Sim .bss %d\n", sctx->sim_bsssz);

	sctx->need_n16bsr=(sctx->sim_txtsz>=(65536-4096));
	sctx->need_n20bsr=(sctx->sim_txtsz>=(1048576-65536));
	sctx->need_n24bsr=(sctx->sim_txtsz>=(16777216-262144));

	sctx->need_n16dat=(sctx->simimgsz>=(65536-4096));
	sctx->need_n20dat=(sctx->simimgsz>=(1048576-65536));
	sctx->need_n24dat=(sctx->simimgsz>=(16777216-262144));

#if 0
#if 1
	for(i=0; i<ctx->n_reg_globals; i++)
	{
		if(i<shufgbl)
			j=shufarr[i];
		else
			j=i;
		obj=ctx->reg_globals[j];

//		obj=ctx->reg_globals[i];
		if(!obj)
			continue;

		if(obj->regtype==CCXL_LITID_GLOBALVAR)
		{
			BGBCC_JX2C_BuildGlobal(ctx, obj);
			continue;
		}

		if(obj->regtype==CCXL_LITID_STATICVAR)
		{
			BGBCC_JX2C_BuildGlobal(ctx, obj);
			continue;
		}

		if((obj->regtype==CCXL_LITID_STRUCT) ||
			(obj->regtype==CCXL_LITID_CLASS))
		{
			BGBCC_JX2C_BuildStruct(ctx, obj);
			continue;
		}
	}
#endif

	for(i=0; i<ctx->n_reg_globals; i++)
	{
		if(i<shufgbl)
			j=shufarr[i];
		else
			j=i;
		obj=ctx->reg_globals[j];

//		obj=ctx->reg_globals[i];
		if(!obj)
			continue;

		if((obj->regtype==CCXL_LITID_FUNCTION) && (obj->vtr))
		{
			BGBCC_JX2C_BuildFunction(ctx, obj);
			continue;
		}

		if(obj->regtype==CCXL_LITID_ASMBLOB)
		{
			BGBCC_JX2C_BuildAsmBlob(ctx, obj);
			continue;
		}

#if 0
		if(obj->regtype==CCXL_LITID_GLOBALVAR)
		{
			BGBCC_JX2C_BuildGlobal(ctx, obj);
			continue;
		}

		if(obj->regtype==CCXL_LITID_STATICVAR)
		{
			BGBCC_JX2C_BuildGlobal(ctx, obj);
			continue;
		}

		if((obj->regtype==CCXL_LITID_STRUCT) ||
			(obj->regtype==CCXL_LITID_CLASS))
		{
			BGBCC_JX2C_BuildStruct(ctx, obj);
			continue;
		}
#endif
	}
#endif


#if 1
	n=0;
	while(n<ctx->n_reg_globals)
	{
		l=ctx->n_reg_globals;
		for(i=n; i<l; i++)
		{
			if(i<shufgbl)
				j=shufarr[i];
			else
				j=i;
			obj=ctx->reg_globals[j];

	//		obj=ctx->reg_globals[i];
			if(!obj)
				continue;

			if(obj->regtype==CCXL_LITID_GLOBALVAR)
			{
				BGBCC_JX2C_BuildGlobal(ctx, obj);
				continue;
			}

			if(obj->regtype==CCXL_LITID_STATICVAR)
			{
				BGBCC_JX2C_BuildGlobal(ctx, obj);
				continue;
			}

			if((obj->regtype==CCXL_LITID_STRUCT) ||
				(obj->regtype==CCXL_LITID_CLASS))
			{
				BGBCC_JX2C_BuildStruct(ctx, obj);
				continue;
			}
		}

		for(i=n; i<l; i++)
		{
			if(i<shufgbl)
				j=shufarr[i];
			else
				j=i;
			obj=ctx->reg_globals[j];

			if(!obj)
				continue;

			if((obj->regtype==CCXL_LITID_FUNCTION) && (obj->vtr))
			{
				BGBCC_JX2C_BuildFunction(ctx, obj);
				continue;
			}
			if(obj->regtype==CCXL_LITID_ASMBLOB)
			{
				BGBCC_JX2C_BuildAsmBlob(ctx, obj);
				continue;
			}
		}
		
		n=l;
	}
#endif
	
	BGBCC_JX2C_BuildPrestartInit(ctx);


	k=sctx->stat_tot_dq0+sctx->stat_tot_dq1+sctx->stat_tot_dqi;
	if(k>0)
	{
		printf("DQ0/DQ1/DQi=%d/%d/%d %.2f%% %.2f%% %.2f%%\n",
			sctx->stat_tot_dq0,
			sctx->stat_tot_dq1,
			sctx->stat_tot_dqi,
			(100.0*sctx->stat_tot_dq0)/k,
			(100.0*sctx->stat_tot_dq1)/k,
			(100.0*sctx->stat_tot_dqi)/k
			);
	}

	if(sctx->stat_tot_imm>0)
	{
		printf("tot imm %d\n", sctx->stat_tot_imm);
		printf("tot imm8 %d\n", sctx->stat_tot_imm8);
		printf("tot imm16 %d\n", sctx->stat_tot_imm16);
		printf("tot imm8r %d\n", sctx->stat_tot_imm8r);
		printf("tot imm32 %d\n", sctx->stat_tot_imm32);
	}
	
	if(sctx->stat_opc_tot>0)
	{
		k=sctx->stat_opc_tot;
//		printf("16=%.2f%% 8A=%.2f%% 8E=%.2f%% CE=%.2f%% "
//				"CC0=%.2f%% CC3=%.2f%%\n",
		printf("16=%.2f%% Fz=%.2f%% FC=%.2f%% FD=%.2f%% "
				"CC0=%.2f%% CC3=%.2f%%\n",
			(100.0*sctx->stat_opc_base16)/k,
			(100.0*sctx->stat_opc_ext8a)/k,
			(100.0*sctx->stat_opc_ext8e)/k,
			(100.0*sctx->stat_opc_extCe)/k,
			(100.0*sctx->stat_opc_extCC0)/k,
			(100.0*sctx->stat_opc_extCC3)/k
			);

		printf("16=%.2fkB Fz=%.2fkB FC=%.2fkB FD=%.2fkB\n",
			(2.0*sctx->stat_opc_base16)/1024.0,
			(4.0*sctx->stat_opc_ext8a)/1024.0,
			(6.0*sctx->stat_opc_ext8e)/1024.0,
			(6.0*sctx->stat_opc_extCe)/1024.0
			);
	}
	
#if 1
	printf("High 8 op use map:\n");
	printf("A    ");
	for(j=0; j<8; j++)
			printf("    x%1X", j);
	printf("\n");

	printf("B     ");
	for(j=8; j<16; j++)
			printf("    x%1X", j);
	printf("\n");
	
	for(i=0; i<16; i++)
	{
		if((i==0xA) || (i==0xB) || (i==0xC) || (i==0xD))
		{
			k=0;
			for(j=0; j<16; j++)
				{ k+=sctx->opcnt_hi8[i*16+j]; }
			printf("  %1Xx %d\n", i, k);
			continue;
		}

		k=0;
		for(j=0; j<16; j++)
			{ k+=sctx->opcnt_hi8[i*16+j]; }
		if(!k)continue;
	
		printf("A %1Xx ", i);
		for(j=0; j<8; j++)
		{
			printf(" %5d", sctx->opcnt_hi8[i*16+j]);
		}
		printf("\n");

		printf("B %1Xx  ", i);
		for(j=8; j<16; j++)
		{
			printf(" %5d", sctx->opcnt_hi8[i*16+j]);
		}
		printf("\n");
	}
	printf("\n");
#endif

#if 1
	printf("High 3xzx op use map:\n");
//	printf("   ");
//	for(j=0; j<16; j++)
//			printf("  x%1X", j);
//	printf("\n");

	printf("A    ");
	for(j=0; j<8; j++)
			printf("    x%1X", j);
	printf("\n");

	printf("B     ");
	for(j=8; j<16; j++)
			printf("    x%1X", j);
	printf("\n");

//	for(i=0; i<16; i++)
	for(i=0; i<8; i++)
	{
		k=0;
		for(j=0; j<16; j++)
			{ k+=sctx->opcnt_3xx[i*16+j]; }
		if(!k)continue;

		printf("A %1Xx ", i);
		for(j=0; j<8; j++)
		{
			printf(" %5d", sctx->opcnt_3xx[i*16+j]);
		}
		printf("\n");

		printf("B %1Xx  ", i);
		for(j=8; j<16; j++)
		{
			printf(" %5d", sctx->opcnt_3xx[i*16+j]);
		}
		printf("\n");
	}
	printf("\n");
#endif

#if 1
	printf("High F0xx op use map:\n");
//	printf("   ");
//	for(j=0; j<16; j++)
//			printf("  x%1X", j);
//	printf("\n");

	printf("A    ");
	for(j=0; j<8; j++)
			printf("    x%1X", j);
	printf("\n");

	printf("B     ");
	for(j=8; j<16; j++)
			printf("    x%1X", j);
	printf("\n");

	for(i=0; i<16; i++)
	{
		k=0;
		for(j=0; j<16; j++)
			{ k+=sctx->opcnt_f0xx[i*16+j]; }
		if(!k)continue;
	
		if(i>=0xC)
		{
			k=0;
			for(j=0; j<16; j++)
				{ k+=sctx->opcnt_f0xx[i*16+j]; }
			printf("  %1Xx %d\n", i, k);
			continue;
		}

		printf("A %1Xx ", i);
		for(j=0; j<8; j++)
		{
			printf(" %5d", sctx->opcnt_f0xx[i*16+j]);
		}
		printf("\n");

		printf("B %1Xx  ", i);
		for(j=8; j<16; j++)
		{
			printf(" %5d", sctx->opcnt_f0xx[i*16+j]);
		}
		printf("\n");
	}
	printf("\n");
#endif

#if 1
	printf("High F1xx op use map:\n");
	for(i=0; i<8; i++)
	{
		printf(" %5d", sctx->opcnt_f1xx[i]);
	}
	printf("\n");
	for(i=8; i<16; i++)
	{
		printf(" %5d", sctx->opcnt_f1xx[i]);
	}
	printf("\n");
#endif

#if 1
	printf("High F2xx op use map:\n");

	printf("A    ");
	for(j=0; j<8; j++)
			printf("    x%1X", j);
	printf("\n");

	printf("B     ");
	for(j=8; j<16; j++)
			printf("    x%1X", j);
	printf("\n");

	printf("0x..7x ");
	for(i=0; i<8; i++)
	{
		k=0;
		for(j=0; j<16; j++)
			{ k+=sctx->opcnt_f2xx[i*16+j]; }
		printf(" %5d", k);
	}
	printf("\n");
	printf("8x..Bx ");
	for(i=8; i<12; i++)
	{
		k=0;
		for(j=0; j<16; j++)
			{ k+=sctx->opcnt_f2xx[i*16+j]; }
		printf(" %5d", k);
	}
	printf("\n");

	for(i=12; i<16; i++)
	{
		k=0;
		for(j=0; j<16; j++)
			{ k+=sctx->opcnt_f2xx[i*16+j]; }
		if(!k)continue;
	
		printf("A %1Xx ", i);
		for(j=0; j<8; j++)
		{
			printf(" %5d", sctx->opcnt_f2xx[i*16+j]);
		}
		printf("\n");

		printf("B %1Xx  ", i);
		for(j=8; j<16; j++)
		{
			printf(" %5d", sctx->opcnt_f2xx[i*16+j]);
		}
		printf("\n");
	}
	printf("\n");
#endif

#if 1
	printf("High F8xx op use map:\n");
	for(i=0; i<8; i++)
	{
		printf(" %5d", sctx->opcnt_f8xx[i]);
	}
	printf("\n");
	for(i=8; i<16; i++)
	{
		printf(" %5d", sctx->opcnt_f8xx[i]);
	}
	printf("\n");
#endif

	if(sctx->lvt16_n_idx>0)
	{
		BGBCC_JX2_SetSectionName(sctx, ".data");
		BGBCC_JX2_EmitBAlign(sctx, 16);
		for(i=0; i<sctx->lvt16_n_idx; i++)
		{
			BGBCC_JX2_EmitLabel(sctx, sctx->lvt16_lbl[i]);
			if(sctx->is_le)
			{
				BGBCC_JX2_EmitQWord(sctx, sctx->lvt16_val[i*2+0]);
				BGBCC_JX2_EmitQWord(sctx, sctx->lvt16_val[i*2+1]);
			}else
			{
				BGBCC_JX2_EmitQWord(sctx, sctx->lvt16_val[i*2+1]);
				BGBCC_JX2_EmitQWord(sctx, sctx->lvt16_val[i*2+0]);
			}
		}
	}

	if(sctx->got_n_gblidx>0)
	{
		BGBCC_JX2_SetSectionName(sctx, ".got");
		BGBCC_JX2_EmitBAlign(sctx, 4);
		BGBCC_JX2_EmitLabel(sctx, sctx->lbl_got);
		
		addr=0;
		for(i=0; i<sctx->got_n_gblidx; i++)
		{
			j=sctx->got_gblidx[i];
			l0=ctx->reg_globals[j]->fxoffs;
//			BGBCC_JX2_EmitRelocAbs32(sctx, l0);
//			BGBCC_JX2_EmitDWord(sctx, 0);
			BGBCC_JX2_EmitDWordAbs32(sctx, k);
		}
	}

	BGBCC_JX2_SetSectionName(sctx, ".text");
	BGBCC_JX2_EmitNamedLabel(sctx, "__text_end");
	BGBCC_JX2_EmitBAlign(sctx, 16);

	BGBCC_JX2_SetSectionName(sctx, ".data");
	BGBCC_JX2_EmitNamedLabel(sctx, "__data_end");

	BGBCC_JX2_SetSectionName(sctx, ".bss");
	BGBCC_JX2_EmitNamedLabel(sctx, "_end");
	BGBCC_JX2_EmitNamedLabel(sctx, "__bss_end");

	if(ctx->n_error)
	{
		printf("Errors Seen (CGEN)\n");
		return(-1);
	}

	if((imgfmt==BGBCC_IMGFMT_EXE) ||
		(imgfmt==BGBCC_IMGFMT_DLL) ||
		(imgfmt==BGBCC_IMGFMT_SYS))
	{
		BGBCC_JX2C_FlattenImagePECOFF(ctx, obuf, rosz, imgfmt);

		if(ctx->n_error)
		{
			printf("Errors Seen (IMG)\n");
			return(-1);
		}

		return(1);
	}

//	if((imgfmt==BGBCC_IMGFMT_ELXE) ||
//		(imgfmt==BGBCC_IMGFMT_ELSO))
//	{
//		BGBCC_JX2C_FlattenImageELF(ctx, obuf, rosz, imgfmt);
//		return(1);
//	}

	if(imgfmt==BGBCC_IMGFMT_ROM)
	{
		BGBCC_JX2C_FlattenImageROM(ctx, obuf, rosz, imgfmt);

		if(ctx->n_error)
		{
			printf("Errors Seen (IMG)\n");
			return(-1);
		}

		return(1);
	}

	if(imgfmt==BGBCC_IMGFMT_ASM)
	{
		BGBCC_JX2C_FlattenImageASM(ctx, obuf, rosz, imgfmt);
		return(1);
	}

	return(-1);
}
