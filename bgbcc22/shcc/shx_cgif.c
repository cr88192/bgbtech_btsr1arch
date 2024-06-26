BGBCC_CCXL_BackendFuncs_vt bgbgc_shx_vt;
static int bgbgc_shx_init=0;

	
int BGBCC_SHXC_InitIface()
{
	if(bgbgc_shx_init)
		return(0);
	bgbgc_shx_init=1;

	bgbgc_shx_vt.tgtname="SHx";
	
	bgbgc_shx_vt.SetupContextForArch=	BGBCC_SHXC_SetupContextForArch;
	bgbgc_shx_vt.FlattenImage=			BGBCC_SHXC_FlattenImage;
	bgbgc_shx_vt.EndFunction=			BGBCC_SHXC_EndFunction;
	bgbgc_shx_vt.LoadBufferDLL=			BGBCC_SHXC_LoadBufferDLL;

	BGBCC_CCXL_RegisterBackend(&bgbgc_shx_vt);

	return(1);
}

ccxl_status BGBCC_SHXC_LoadBufferDLL(BGBCC_TransState *ctx,
	byte *buf, int sz)
{
	BGBCC_SHX_Context *sctx;
	BGBCC_CCXL_RegisterInfo *obj;
	BGBCC_CCXL_LiteralInfo *litobj;
	int l0;
	u32 addr;
	int i, j, k;

	sctx=ctx->uctx;

	if((buf[0]=='M') && (buf[1]=='Z'))
	{
		i=BGBCC_SHXC_CoffLoadBufferDLL(ctx, sctx, buf, sz);
		return(i);
	}
	
	return(-1);
}

ccxl_status BGBCC_SHXC_SetupContextForArch(BGBCC_TransState *ctx)
{
	BGBCC_SHX_Context *shctx;

	if(ctx->arch!=BGBCC_ARCH_SH)
		return(CCXL_STATUS_ERR_UNSUPPORTED);

	shctx=BGBCC_SHX_AllocContext();

	ctx->arch_sizeof_long=4;
	ctx->arch_sizeof_ptr=4;
	ctx->arch_sizeof_valist=64;
	ctx->arch_align_max=4;
	ctx->arch_align_objmin=4;
	ctx->uctx=shctx;

	shctx->tctx=ctx;
	shctx->is_le=0;
	shctx->is_addr64=0;
	shctx->has_bjx1egpr=0;
	shctx->is_betav=0;
	shctx->has_bjx1r3mov=0;

	switch(ctx->sub_arch)
	{
	case BGBCC_ARCH_SH_BX6B:
	case BGBCC_ARCH_SH_BX6L:
	case BGBCC_ARCH_SH_BX6C:
		ctx->arch_align_max=8;
		ctx->arch_sizeof_long=8;
		ctx->arch_sizeof_ptr=8;
		ctx->arch_sizeof_valist=128;
		shctx->is_addr64=1;
		shctx->has_bjx1egpr=1;
		shctx->has_bjx1r3mov=1;
		break;

	case BGBCC_ARCH_SH_BX6U:
	case BGBCC_ARCH_SH_BX6V:
	case BGBCC_ARCH_SH_BX6M:
		ctx->arch_align_max=8;
		ctx->arch_sizeof_long=8;
		ctx->arch_sizeof_ptr=8;
		ctx->arch_sizeof_valist=128;
		shctx->is_addr64=1;
		shctx->has_bjx1r3mov=1;
//		shctx->has_bjx1egpr=0;
		break;
	default:
		break;
	}

	switch(ctx->sub_arch)
	{
	case BGBCC_ARCH_SH_SH2:
	case BGBCC_ARCH_SH_SH2B:
	case BGBCC_ARCH_SH_SH4B:
	case BGBCC_ARCH_SH_BX1B:
	case BGBCC_ARCH_SH_BX6B:
//	case BGBCC_ARCH_SH_BX6V:
		shctx->is_le=0;
		break;
	case BGBCC_ARCH_SH_SH4:
	case BGBCC_ARCH_SH_SH2L:
	case BGBCC_ARCH_SH_SH4L:
	case BGBCC_ARCH_SH_BX1L:
	case BGBCC_ARCH_SH_BX6L:
	case BGBCC_ARCH_SH_BX6U:
	case BGBCC_ARCH_SH_BX6M:
	case BGBCC_ARCH_SH_BX6C:
	case BGBCC_ARCH_SH_BX6V:
		shctx->is_le=1;
		break;
	default:
		break;
	}

	shctx->has_shad=0;
	shctx->has_movi20=0;
	shctx->has_bjx1jmp=0;
	shctx->has_movi20=0;
	shctx->has_misalgn=0;
	shctx->has_bjx1mov=0;
	shctx->has_bjx1ari=0;
	shctx->has_bjx1breq=0;
	
	shctx->no_fpu=0;
	shctx->no_ext32=0;
	shctx->fpu_soft=0;
	shctx->fpu_lite=0;

	switch(ctx->sub_arch)
	{
	case BGBCC_ARCH_SH_SH2:
	case BGBCC_ARCH_SH_SH2L:
	case BGBCC_ARCH_SH_SH2B:
		break;
	case BGBCC_ARCH_SH_SH4:
	case BGBCC_ARCH_SH_SH4L:
	case BGBCC_ARCH_SH_SH4B:
		shctx->has_shad=1;
		break;

	case BGBCC_ARCH_SH_BX6U:
	case BGBCC_ARCH_SH_BX6V:
		shctx->has_shad=1;
		shctx->has_misalgn=1;

//		shctx->has_bjx1mov=1;
//		shctx->has_bjx1jmp=1;
//		shctx->has_bjx1ari=1;
//		shctx->has_bjx1egpr=1;
//		shctx->no_ext32=1;

//		shctx->fpu_lite=1;
		shctx->fpu_soft=1;

		break;

	case BGBCC_ARCH_SH_BX1L:
	case BGBCC_ARCH_SH_BX1B:
	case BGBCC_ARCH_SH_BX6L:
	case BGBCC_ARCH_SH_BX6B:
	case BGBCC_ARCH_SH_BX6M:
	case BGBCC_ARCH_SH_BX6C:
		shctx->has_shad=1;
//		shctx->has_movi20=0;
//		shctx->has_bjx1jmp=0;
//		shctx->has_movi20=1;
		shctx->has_misalgn=1;
		shctx->has_bjx1mov=1;
		shctx->has_bjx1jmp=1;
		shctx->has_bjx1ari=1;
//		shctx->has_bjx1breq=1;

//		shctx->has_bjx1ari=0;

//		shctx->use_onlyimm=1;
//		shctx->use_onlyimm=3;
		break;


	default:
		break;
	}
	
	if((ctx->sub_arch==BGBCC_ARCH_SH_BX6C) ||
		(ctx->sub_arch==BGBCC_ARCH_SH_BX6V))
	{
		shctx->is_betav=1;
		shctx->is_mergece=1;
		shctx->has_bjx1r3mov=0;
	}
	
	if(shctx->fpu_soft)
		shctx->no_fpu=1;
	

	BGBPP_AddStaticDefine(NULL, "__superh__", "");
	BGBPP_AddStaticDefine(NULL, "__sh__", "");

	BGBPP_AddStaticDefine(NULL, "__BGBCC__", "");
	BGBPP_AddStaticDefine(NULL, "__BGBCC_SH__", "");
	
	if(shctx->has_bjx1mov)
		BGBPP_AddStaticDefine(NULL, "__BJX1__", "");

	if(shctx->no_fpu)
		BGBPP_AddStaticDefine(NULL, "__BJX1_NOFPU__", "");

	if(shctx->no_ext32)
		BGBPP_AddStaticDefine(NULL, "__BJX1_ONLY16__", "");

	if(ctx->arch_sizeof_ptr==8)
	{
		BGBPP_AddStaticDefine(NULL, "__BJX1_64__", "");

		if(shctx->has_bjx1egpr)
		{
			BGBPP_AddStaticDefine(NULL, "__BJX1_64E__", "");
			BGBPP_AddStaticDefine(NULL, "__BJX1_64A__", "");
		}
	}

	return(0);
}

bool BGBCC_SHXC_TypeIntRegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_SHX_Context *sctx;

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
		if(BGBCC_CCXL_TypeFloatP(ctx, ty))
			return(true);
		if(BGBCC_CCXL_TypeFloat16P(ctx, ty))
			return(true);

//		if(ctx->arch_sizeof_ptr==8)
//		{
//			if(BGBCC_CCXL_TypeDoubleP(ctx, ty))
//				return(true);
//		}
	}

	return(false);
}

bool BGBCC_SHXC_TypeInt2RegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_SHX_Context *sctx;

	sctx=ctx->uctx;
	
	if(ctx->arch_sizeof_ptr==8)
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

//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(true);
	return(false);
}

bool BGBCC_SHXC_TypeFloatRegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_SHX_Context *sctx;

	sctx=ctx->uctx;
	
	if(sctx->no_fpu || sctx->fpu_soft)
		return(0);

	if(BGBCC_CCXL_TypeFloatP(ctx, ty))
		return(true);
	if(BGBCC_CCXL_TypeFloat16P(ctx, ty))
		return(true);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(true);
	return(false);
}

bool BGBCC_SHXC_TypeFloat2RegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_SHX_Context *sctx;

	sctx=ctx->uctx;
	
	if(sctx->no_fpu || sctx->fpu_soft)
		return(0);
	
	if(BGBCC_CCXL_TypeDoubleP(ctx, ty))
		return(true);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(true);
	return(false);
}

int BGBCC_SHXC_TypeGetRegClassP(BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_SHX_Context *sctx;
	int sz;

	sctx=ctx->uctx;

	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
	{
		return(BGBCC_SH_REGCLS_AR_REF);
	}

	if(BGBCC_SHXC_TypeIntRegP(ctx, ty))
		return(BGBCC_SH_REGCLS_GR);
	if(BGBCC_SHXC_TypeInt2RegP(ctx, ty))
		return(BGBCC_SH_REGCLS_GR2);
	if(BGBCC_SHXC_TypeFloatRegP(ctx, ty))
		return(BGBCC_SH_REGCLS_FR);
	if(BGBCC_SHXC_TypeFloat2RegP(ctx, ty))
		return(BGBCC_SH_REGCLS_FR2);

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
		}
	}
		
	if(BGBCC_CCXL_TypeValueObjectP(ctx, ty))
	{
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, ty);
		if(sz<=4)
			return(BGBCC_SH_REGCLS_VO_GR);
		if(sz<=8)
		{
			if(ctx->arch_sizeof_ptr==8)
				return(BGBCC_SH_REGCLS_VO_QGR);
			return(BGBCC_SH_REGCLS_VO_GR2);
		}
		if(sz<=16)
		{
			return(BGBCC_SH_REGCLS_VO_QGR2);
		}

		return(BGBCC_SH_REGCLS_VO_REF);
	}

	return(BGBCC_SH_REGCLS_NONE);
}


int BGBCC_SHXC_EmitVaArg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type,
	ccxl_register dreg,
	ccxl_register sreg)
{
	int i, j, k;

//	sctx->sreg_live|=0x0010;
//	sctx->sfreg_live|=0x0000;


	if(sctx->is_addr64)
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ4);
		BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_RQ4);
	}
	else
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
	}

//	BGBCC_SHXC_EmitCallName(ctx, sctx, "__va_arg_i");
//	BGBCC_SHXC_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);

	j=BGBCC_SHXC_TypeGetRegClassP(ctx, type);
	switch(j)
	{
	case BGBCC_SH_REGCLS_GR:
	case BGBCC_SH_REGCLS_VO_GR:
	case BGBCC_SH_REGCLS_VO_REF:
		if(sctx->is_addr64)
			BGBCC_SHXC_EmitCallName(ctx, sctx, "__va64_arg_i");
		else
			BGBCC_SHXC_EmitCallName(ctx, sctx, "__va_arg_i");
//		BGBCC_SHXC_EmitMovRegReg(ctx, sctx,
//			BGBCC_SH_REG_R0, BGBCC_SH_REG_R3);
//		BGBCC_SHXC_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R3);

		BGBCC_SHXC_ResetModeDqUnknown(ctx, sctx);

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);

		break;

	case BGBCC_SH_REGCLS_VO_QGR:
	case BGBCC_SH_REGCLS_QGR:
		if(sctx->is_addr64)
			BGBCC_SHXC_EmitCallName(ctx, sctx, "__va64_arg_l");
		else
			BGBCC_SHXC_EmitCallName(ctx, sctx, "__va_arg_l");

		BGBCC_SHXC_ResetModeDqUnknown(ctx, sctx);

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);

		break;

	case BGBCC_SH_REGCLS_GR2:
	case BGBCC_SH_REGCLS_VO_GR2:
		if(sctx->is_addr64)
			BGBCC_SHXC_EmitCallName(ctx, sctx, "__va64_arg_x");
		else
			BGBCC_SHXC_EmitCallName(ctx, sctx, "__va_arg_x");

		BGBCC_SHXC_ResetModeDqUnknown(ctx, sctx);

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);

		break;

	case BGBCC_SH_REGCLS_VO_QGR2:
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__va64_arg_x");
		BGBCC_SHXC_ResetModeDqUnknown(ctx, sctx);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);
		break;

	case BGBCC_SH_REGCLS_FR:
		if(sctx->is_addr64)
			BGBCC_SHXC_EmitCallName(ctx, sctx, "__va64_arg_f");
		else
			BGBCC_SHXC_EmitCallName(ctx, sctx, "__va_arg_f");

		BGBCC_SHXC_ResetModeDqUnknown(ctx, sctx);

		BGBCC_SHXC_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_FR0);
		break;
	case BGBCC_SH_REGCLS_FR2:
		if(sctx->is_addr64)
			BGBCC_SHXC_EmitCallName(ctx, sctx, "__va64_arg_d");
		else
			BGBCC_SHXC_EmitCallName(ctx, sctx, "__va_arg_d");

		BGBCC_SHXC_ResetModeDqUnknown(ctx, sctx);

		BGBCC_SHXC_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR0);
		break;
	default:
		break;
	}

	BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);

	return(1);
}

int BGBCC_SHXC_EmitVaEnd(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register sreg)
{
	return(0);
}

int BGBCC_SHXC_EmitVaStart(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
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

char *BGBCC_SHXC_DebugRegToStr(BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
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

ccxl_status BGBCC_SHXC_PrintVirtOp(BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
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
			case CCXL_VOP_JMPTAB:			s0="JMPTAB"; break;
			case CCXL_VOP_OBJCALL:			s0="OBJCALL"; break;
			case CCXL_VOP_PREDCMP:			s0="PREDCMP"; break;
			case CCXL_VOP_PREDCMP_Z:		s0="PREDCMP_Z"; break;
			case CCXL_VOP_PREDSYNC:			s0="PREDSYNC"; break;
			case CCXL_VOP_CALL_INTRIN:		s0="CALL_INTRIN"; break;
			case CCXL_VOP_ASMINLINE:		s0="ASMINLINE"; break;
			case CCXL_VOP_TEMP_PHI:			s0="TEMP_PHI"; break;
			case CCXL_VOP_TRINARY:			s0="TRINARY"; break;
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
				BGBCC_SHXC_DebugRegToStr(ctx, sctx, op->type, op->dst));
		}
		if(op->srca.val)
		{
//			fprintf(sctx->cgen_log, " srca=%016llX", op->srca.val);
			fprintf(sctx->cgen_log, " srca=%s", 
				BGBCC_SHXC_DebugRegToStr(ctx, sctx, op->type, op->srca));
		}
		if(op->srcb.val)
		{
//			fprintf(sctx->cgen_log, " srcb=%016llX", op->srcb.val);
			fprintf(sctx->cgen_log, " srcb=%s", 
				BGBCC_SHXC_DebugRegToStr(ctx, sctx, op->type, op->srcb));
		}

		if(op->srcc.val)
		{
			fprintf(sctx->cgen_log, " srcc=%s", 
				BGBCC_SHXC_DebugRegToStr(ctx, sctx, op->type, op->srcc));
		}

		if(op->srcd.val)
		{
			fprintf(sctx->cgen_log, " srcd=%s", 
				BGBCC_SHXC_DebugRegToStr(ctx, sctx, op->type, op->srcd));
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
		
		if(op->opn==CCXL_VOP_CALL)
		{
			na=op->imm.call.na;
			ca=op->imm.call.args;
			for(i=0; i<na; i++)
			{
				fprintf(sctx->cgen_log, "\t\tA%d=%s\n",  i,
					BGBCC_SHXC_DebugRegToStr(ctx, sctx, op->type, ca[i]));
			}
			fflush(sctx->cgen_log);
		}
	}
#endif

	return(0);
}

ccxl_status BGBCC_SHXC_CompileVirtOp(BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, BGBCC_CCXL_VirtOp *op)
{
	char *s0;

//	BGBCC_SHXC_CompilePrintVirtOp(ctx, sctx, obj, op);

	sctx->sreg_live=sctx->sreg_held;
	sctx->sfreg_live=sctx->sfreg_held;
	BGBCC_SHX_EmitCheckFlushIndexImm(sctx);

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
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);
//		BGBCC_SHX_EmitForceFlushIndexImm(sctx);
		BGBCC_SHX_EmitLabel(sctx,
			op->imm.si);
		break;
	case CCXL_VOP_JMP:
//		BGBCC_SHX_EmitOpLabel(sctx, BGBCC_SH_NMID_BRA,
//			op->imm.si);
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);
//		BGBCC_SHXC_ResetFpscrUnknown(ctx, sctx);

//		BGBCC_SHX_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BRA,
//			op->imm.si);
//		BGBCC_SHX_EmitOpNone(sctx,
//			BGBCC_SH_NMID_NOP);

		BGBCC_SHX_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BRAN,
			op->imm.si);

//		BGBCC_SHX_EmitFlushIndexImm16(sctx);
//		BGBCC_SHX_EmitFlushIndexImm32(sctx);
		BGBCC_SHX_EmitFlushIndexImmBasic(sctx);
		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
		break;
	case CCXL_VOP_MOV:
		BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, op->type,
			op->dst, op->srca);
		break;
	case CCXL_VOP_JCMP_ZERO:
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);
//		BGBCC_SHXC_ResetFpscrUnknown(ctx, sctx);
//		BGBCC_SHX_EmitForceFlushIndexImm(sctx);
		BGBCC_SHXC_EmitJCmpVRegZero(ctx, sctx, op->type,
			op->srca, op->opr, op->imm.si);
		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
		break;
	case CCXL_VOP_JCMP:
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);
//		BGBCC_SHXC_ResetFpscrUnknown(ctx, sctx);
//		BGBCC_SHX_EmitForceFlushIndexImm(sctx);
		BGBCC_SHXC_EmitJCmpVRegVReg(ctx, sctx, op->type,
			op->srca, op->srcb, op->opr, op->imm.si);
		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
		break;
	case CCXL_VOP_CALL:
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
//		BGBCC_SHXC_ResetFpscrDefaults(ctx, sctx);
//		BGBCC_SHX_EmitForceFlushIndexImm(sctx);

//		sctx->sreg_live|=0x00F4;
//		sctx->sreg_live|=0x00F0;
//		sctx->sreg_live|=0x00F000F0;
//		sctx->sfreg_live|=0x0FF0;
		BGBCC_SHXC_EmitCallVReg(ctx, sctx,
			op->type, op->dst, op->srca,
			op->imm.call.na, op->imm.call.args);

//		sctx->sreg_live&=~0x00F0;
//		sctx->sreg_live&=~0x00F000F0;
//		sctx->sfreg_live&=~0x0FF0;

//		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
//		BGBCC_SHXC_EmitLabelFlushFpRegisters(ctx, sctx);
		break;
	case CCXL_VOP_CSRV:
//		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
//		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
		BGBCC_SHXC_EmitCsrvVReg(ctx, sctx,
			op->type, op->dst);
		break;
	case CCXL_VOP_RETDFL:
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);
		BGBCC_SHXC_EmitReturnVoid(ctx, sctx);
		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
		BGBCC_SHX_EmitFlushIndexImmBasic(sctx);
		break;
	case CCXL_VOP_RETV:
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);
		BGBCC_SHXC_EmitReturnVoid(ctx, sctx);
		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
		BGBCC_SHX_EmitFlushIndexImmBasic(sctx);
		break;
	case CCXL_VOP_RET:
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);
		BGBCC_SHXC_EmitReturnVReg(ctx, sctx,
			op->type, op->srca);
		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
		BGBCC_SHX_EmitFlushIndexImmBasic(sctx);
		break;
	case CCXL_VOP_CONV:
		BGBCC_SHXC_EmitConvVRegVReg(ctx, sctx,
			op->type, op->stype,
			op->dst, op->srca);
		break;
	case CCXL_VOP_UNARY:
		BGBCC_SHXC_EmitUnaryVRegVReg(ctx, sctx, op->type,
			op->dst, op->opr, op->srca);
		break;
	case CCXL_VOP_BINARY:
		BGBCC_SHXC_EmitBinaryVRegVRegVReg(ctx, sctx, op->type,
			op->dst, op->opr, op->srca, op->srcb);
		break;
	case CCXL_VOP_COMPARE:
		BGBCC_SHXC_EmitCompareVRegVRegVReg(ctx, sctx, op->type,
			op->dst, op->opr, op->srca, op->srcb);
		break;
	case CCXL_VOP_LDIXIMM:
		BGBCC_SHXC_EmitLdixVRegVRegImm(ctx, sctx, op->type,
			op->dst, op->srca, op->imm.si);
		break;
	case CCXL_VOP_STIXIMM:
		BGBCC_SHXC_EmitStixVRegVRegImm(ctx, sctx, op->type,
			op->dst, op->srca, op->imm.si);
		break;
	case CCXL_VOP_LDIX:
		BGBCC_SHXC_EmitLdixVRegVRegVReg(ctx, sctx, op->type,
			op->dst, op->srca, op->srcb);
		break;
	case CCXL_VOP_STIX:
		BGBCC_SHXC_EmitStixVRegVRegVReg(ctx, sctx, op->type,
			op->dst, op->srca, op->srcb);
		break;
	case CCXL_VOP_LEAIMM:
		BGBCC_SHXC_EmitLeaVRegVRegImm(ctx, sctx, op->type,
			op->dst, op->srca, op->imm.si);
		break;
	case CCXL_VOP_LEA:
		BGBCC_SHXC_EmitLeaVRegVRegVReg(ctx, sctx, op->type,
			op->dst, op->srca, op->srcb);
		break;
	case CCXL_VOP_LDAVAR:
		BGBCC_SHXC_EmitLdaVRegVReg(ctx, sctx, op->type,
			op->dst, op->srca);
		break;

	case CCXL_VOP_DIFFPTR:
		BGBCC_SHXC_EmitDiffPtrVRegVRegVReg(ctx, sctx, op->type,
			op->dst, op->srca, op->srcb);
		break;

	case CCXL_VOP_LOADSLOT:
		BGBCC_SHXC_EmitLoadSlotVRegVRegImm(ctx, sctx, op->type,
			op->dst, op->srca, op->imm.obj.gid, op->imm.obj.fid);
		break;
	case CCXL_VOP_STORESLOT:
		BGBCC_SHXC_EmitStoreSlotVRegVRegImm(ctx, sctx, op->type,
			op->dst, op->srca, op->imm.obj.gid, op->imm.obj.fid);
		break;
	case CCXL_VOP_LOADSLOTADDR:
		BGBCC_SHXC_EmitLoadSlotAddrVRegVRegImm(ctx, sctx, op->type,
			op->dst, op->srca, op->imm.obj.gid, op->imm.obj.fid);
		break;
	
	case CCXL_VOP_INITOBJ:
		break;
	case CCXL_VOP_DROPOBJ:
		break;
	case CCXL_VOP_INITARR:
		break;
	case CCXL_VOP_INITOBJARR:
		break;

	case CCXL_VOP_VA_START:
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_EmitVaStart(ctx, sctx, op->dst, op->srca);
		break;
	case CCXL_VOP_VA_END:
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_EmitVaEnd(ctx, sctx, op->srca);
		break;
	case CCXL_VOP_VA_ARG:
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_EmitVaArg(ctx, sctx, op->type, op->dst, op->srca);
		break;

	default:
		BGBCC_CCXL_StubError(ctx);
		break;
	}

	BGBCC_SHX_EmitCheckFlushIndexImm(sctx);
	
	if(sctx->regalc_live || sctx->fregalc_live)
	{
		BGBCC_DBGBREAK
	}

	if(sctx->sreg_live || sctx->sfreg_live)
	{
		BGBCC_DBGBREAK
	}

#if 1
//	BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
//	BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);

//	BGBCC_SHXC_EmitSyncFpRegisters(ctx, sctx);
//	BGBCC_SHXC_EmitLabelFlushFpRegisters(ctx, sctx);
#endif
	
	return(1);
}

ccxl_status BGBCC_SHXC_PrintVirtTr(BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
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
		BGBCC_SHXC_PrintVirtOp(ctx, sctx, obj, obj->vop[tr->b_ops+i]);
	}

	return(0);
}

ccxl_status BGBCC_SHXC_CompileVirtTr(BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, BGBCC_CCXL_VirtTr *tr, int idx)
{
	BGBCC_CCXL_VirtOp *vop, *vop1, *vop2;
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

	for(i=0; i<tr->n_ops; i++)
	{
		sctx->tr_opnum=tr->b_ops+i;
		vop=obj->vop[tr->b_ops+i];
		vop1=NULL;
		if((i+1)<tr->n_ops)
			vop1=obj->vop[tr->b_ops+i+1];

#if 1
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
				BGBCC_SHXC_EmitLdixVRegVRegVRegImm(
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
				BGBCC_SHXC_EmitStixVRegVRegVRegImm(
					ctx, sctx, vop1->type,
					vop1->dst, vop->srca, vop1->srcb, j);
				i++;
				continue;
			}
		}
#endif
#endif
		
		BGBCC_SHXC_CompileVirtOp(ctx, sctx, obj, vop);
	}

	BGBCC_SHX_EmitCheckFlushIndexImm(sctx);

	return(0);
}

ccxl_status BGBCC_SHXC_EndFunction(BGBCC_TransState *ctx,
	BGBCC_CCXL_LiteralInfo *obj)
{
	return(0);
}

ccxl_status BGBCC_SHXC_BuildFunctionBody(
	BGBCC_TransState *ctx, BGBCC_SHX_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, int fcnlbl)
{
	int bo, co, bo1, co1;
	int i, j, k;

	BGBCC_SHXC_EmitFrameProlog(ctx, sctx, obj, fcnlbl);
	
	bo=BGBCC_SHX_EmitGetOffs(sctx);

	for(i=0; i<obj->n_vtr; i++)
	{
		bo1=BGBCC_SHX_EmitGetOffs(sctx);
		sctx->tr_trnum=i;
		BGBCC_SHXC_CompileVirtTr(ctx, sctx, obj, obj->vtr[i], i);
		co1=BGBCC_SHX_EmitGetOffs(sctx);
		
		if(i<16)
		{
			sctx->fnsz_lvtr[i]=sctx->fnsz_vtr[i];
			sctx->fnsz_vtr[i]=co1-bo1;
			sctx->fnsz_dvtr[i]=sctx->fnsz_vtr[i]-sctx->fnsz_lvtr[i];
		}
	}

	co=BGBCC_SHX_EmitGetOffs(sctx);
	sctx->fnsz_bod=co-bo;

	BGBCC_SHXC_EmitFrameEpilog(ctx, sctx, obj);
	return(0);
}

ccxl_status BGBCC_SHXC_BuildFunction(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	BGBCC_SHX_Context *sctx;
	int fnsz[8];
	int fnsz_pro[8];
	int fnsz_epi[8];
	int fnsz_bod[8];
	int otrov, issta;
	int l0, l1, np, co, lo, bo, bo1, co1, sz, sz1, msz, nsz;
	int i, j, k;

	sctx=ctx->uctx;

	if(obj->regflags&BGBCC_REGFL_CULL)
	{
		fprintf(sctx->cgen_log, "BGBCC_SHXC_BuildFunction: Culled %s\n",
			obj->name);
		fflush(sctx->cgen_log);

		return(0);
	}

	if((obj->flagsint&BGBCC_TYFL_DLLIMPORT) && (obj->n_vtr<=0))
	{
		l0=obj->fxoffs;
		if(l0<=0)
		{
			l0=BGBCC_SHX_GetNamedLabel(sctx, obj->name);
			obj->fxoffs=l0;
		}

		l1=obj->fxnoffs;
		if(l1<=0)
		{
			l1=BGBCC_SHX_GetNamedLabel(sctx, obj->name);
			obj->fxnoffs=l1;
		}

		BGBCC_SHX_SetSectionName(sctx, ".text");
		BGBCC_SHX_EmitBAlign(sctx, 4);
//		BGBCC_SHX_EmitBAlign(sctx, 16);

//		bo=BGBCC_SHX_EmitGetOffs(sctx);

		BGBCC_SHX_EmitLabel(sctx, l0);
		BGBCC_SHX_EmitWord(sctx, 0xD301);
		BGBCC_SHX_EmitWord(sctx, 0x6332);
		BGBCC_SHX_EmitWord(sctx, 0x432B);
		BGBCC_SHX_EmitWord(sctx, 0x0009);
		BGBCC_SHX_EmitLabel(sctx, l1);
		BGBCC_SHX_EmitDWord(sctx, 0);

		return(0);
	}

	if(sctx->cgen_log)
	{
		fprintf(sctx->cgen_log, "BGBCC_SHXC_BuildFunction: Begin %s\n",
			obj->name);
		fflush(sctx->cgen_log);

		for(i=0; i<obj->n_vtr; i++)
		{
			BGBCC_SHXC_PrintVirtTr(ctx, sctx, obj, obj->vtr[i], i);
		}
	}

	
	l0=obj->fxoffs;
	if(l0<=0)
	{
//		l0=BGBCC_SHX_GenLabel(sctx);
		l0=BGBCC_SHX_GetNamedLabel(sctx, obj->name);
		obj->fxoffs=l0;
	}
	
#if 1
	if(sctx->cgen_log)
	{
		fprintf(sctx->cgen_log, "BGBCC_SHXC_BuildFunction: BegSim %s\n",
			obj->name);
		fflush(sctx->cgen_log);
	}

	sctx->cnt_set_fp32=0;
	sctx->cnt_set_fp64=0;

	sctx->cnt_set_dq0=0;
	sctx->cnt_set_dq1=0;
	sctx->dfl_dq=0;

	otrov=sctx->lbltrov;
	sctx->reg_vsave=0;
	sctx->freg_vsave=0;
	lo=-1;
//	bo=BGBCC_SHX_EmitGetOffs(sctx);
	sctx->simfnsz=9999999;
	nsz=0;
	msz=999999999;

	sctx->need_farjmp=1;
	sctx->need_f16jmp=1;
	sctx->need_n12jmp=1;
	sctx->is_leaf=2;

//	sctx->dfl_dq=2;
//	sctx->dfl_dq=3;
	sctx->dfl_dq=4;

	if(sctx->is_addr64 && !sctx->has_bjx1egpr &&
			!sctx->is_betav)
	{
		sctx->dfl_dq=2;
	}

	for(np=0; np<6; np++)
	{
		sctx->lbltrov=otrov;

		sctx->is_stable=1;		//pretend stable
		if(np==0)
			sctx->is_stable=0;

		BGBCC_SHX_SetBeginSimPass(sctx);
		sctx->is_simpass=np+1;

		BGBCC_SHXC_SetupFrameLayout(ctx, sctx, obj);

		BGBCC_SHX_SetSectionName(sctx, ".text");
		BGBCC_SHX_EmitBAlign(sctx, 4);
//		BGBCC_SHX_EmitBAlign(sctx, 16);

		bo=BGBCC_SHX_EmitGetOffs(sctx);

//		BGBCC_SHX_EmitLabel(sctx, l0);
		BGBCC_SHXC_BuildFunctionBody(ctx, sctx, obj, l0);

#if 0
		BGBCC_SHXC_EmitFrameProlog(ctx, sctx, obj);
		
		for(i=0; i<obj->n_vtr; i++)
		{
			sctx->tr_trnum=i;
			BGBCC_SHXC_CompileVirtTr(ctx, sctx, obj, obj->vtr[i], i);
		}
		
		BGBCC_SHXC_EmitFrameEpilog(ctx, sctx, obj);
#endif

		co=BGBCC_SHX_EmitGetOffs(sctx);

		BGBCC_SHX_SetEndSimPass(sctx);

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

//		co=BGBCC_SHX_EmitGetOffs(sctx);
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
		fprintf(sctx->cgen_log, "BGBCC_SHXC_BuildFunction: EndSim %s np=%d\n",
			obj->name, np);
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
	
	BGBCC_SHXC_SetupFrameLayout(ctx, sctx, obj);

	BGBCC_SHX_SetSectionName(sctx, ".text");
	BGBCC_SHX_EmitBAlign(sctx, 4);
//	BGBCC_SHX_EmitBAlign(sctx, 16);

	bo1=BGBCC_SHX_EmitGetOffs(sctx);

//	BGBCC_SHX_EmitLabel(sctx, l0);
	BGBCC_SHXC_BuildFunctionBody(ctx, sctx, obj, l0);

#if 0
	BGBCC_SHXC_EmitFrameProlog(ctx, sctx, obj);
	
	for(i=0; i<obj->n_vtr; i++)
	{
		sctx->tr_trnum=i;
		BGBCC_SHXC_CompileVirtTr(ctx, sctx, obj, obj->vtr[i], i);
	}
	
	BGBCC_SHXC_EmitFrameEpilog(ctx, sctx, obj);
#endif

	co1=BGBCC_SHX_EmitGetOffs(sctx);
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
		fprintf(sctx->cgen_log, "BGBCC_SHXC_BuildFunction: End %s\n",
			obj->name);
		fflush(sctx->cgen_log);
	}

	return(0);
}

ccxl_status BGBCC_SHXC_BuildGlobal_EmitLitAsType(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
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
	int n, sz, asz, bty;
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

			n=litobj->decl->n_listdata;
			if((asz>0) && (asz<n))
			{
				n=asz;
			}
				
			for(i=0; i<n; i++)
			{
				BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, litobj->decl->listdata[i]);
			}

			BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
			for(i=n; i<asz; i++)
			{
				BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value) &&
			BGBCC_CCXL_IsRegImmZeroP(ctx, value))
		{
//			tty=litobj->decl->type;
			asz=BGBCC_CCXL_TypeGetArraySize(ctx, type);

			BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
			for(i=0; i<asz; i++)
			{
				BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
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
				BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}

			BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
			for(; i<asz; i++)
			{
				BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}
			if(asz<=0)
			{
				BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
	{
		if(sctx->is_addr64)
			BGBCC_SHX_EmitBAlign(sctx, 8);
		else
			BGBCC_SHX_EmitBAlign(sctx, 4);

//		BGBCC_SHX_EmitBAlign(sctx, 4);

		if(BGBCC_CCXL_IsRegImmStringP(ctx, value))
		{
			s0=BGBCC_CCXL_GetRegImmStringValue(ctx, value);

			if(type.val==CCXL_VTY_PCHAR)
				k=BGBCC_SHX_EmitGetStrtabLabelUTF2ASCII(sctx, s0);
			if(type.val==CCXL_VTY_PWCHAR)
				k=BGBCC_SHX_EmitGetStrtabLabelUTF2UCS2(sctx, s0);				
			else
				k=BGBCC_SHX_EmitGetStrtabLabel(sctx, s0);

			if(sctx->is_addr64)
			{
				BGBCC_SHX_EmitRelocTy(sctx, k, BGBCC_SH_RLC_ABS64);
				BGBCC_SHX_EmitQWord(sctx, 0);
			}else
			{
				BGBCC_SHX_EmitRelocTy(sctx, k, BGBCC_SH_RLC_ABS32);
				BGBCC_SHX_EmitDWord(sctx, 0);
			}
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, value);
			if(sctx->is_addr64)
				{ BGBCC_SHX_EmitQWord(sctx, i); }
			else
				{ BGBCC_SHX_EmitDWord(sctx, i); }
			return(1);
		}
		
		if(BGBCC_CCXL_IsRegImmLongP(ctx, value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, value);
			if(sctx->is_addr64)
				{ BGBCC_SHX_EmitQWord(sctx, li); }
			else
				{ BGBCC_SHX_EmitDWord(sctx, li); }
			return(1);
		}
		
		if(BGBCC_CCXL_IsRegImmGlobalAddrP(ctx, value))
		{
			i=value.val&CCXL_REGINT_MASK;
//			gblobj=ctx->reg_globals[i];
			k=BGBCC_SHXC_GetGblIndexLabel(ctx, sctx, i);
			if(sctx->is_addr64)
			{
				BGBCC_SHX_EmitRelocTy(sctx, k, BGBCC_SH_RLC_ABS64);
				BGBCC_SHX_EmitQWord(sctx, 0);
			}else
			{
				BGBCC_SHX_EmitRelocTy(sctx, k, BGBCC_SH_RLC_ABS32);
				BGBCC_SHX_EmitDWord(sctx, 0);
			}
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((bty==CCXL_TY_I) || (bty==CCXL_TY_UI) ||
		(((bty==CCXL_TY_NL) || (bty==CCXL_TY_UNL)) && !sctx->is_addr64))
	{
		BGBCC_SHX_EmitBAlign(sctx, 4);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, value);
			BGBCC_SHX_EmitDWord(sctx, i);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((bty==CCXL_TY_L) || (bty==CCXL_TY_UL) ||
		(((bty==CCXL_TY_NL) || (bty==CCXL_TY_UNL)) && sctx->is_addr64))
	{
		if(sctx->is_addr64)
			BGBCC_SHX_EmitBAlign(sctx, 8);
		else
			BGBCC_SHX_EmitBAlign(sctx, 4);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, value);
			BGBCC_SHX_EmitQWord(sctx, li);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(bty==CCXL_TY_F)
	{
		BGBCC_SHX_EmitBAlign(sctx, 4);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, value);
			(*(float *)(&j))=li;
			BGBCC_SHX_EmitDWord(sctx, j);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmFloatP(ctx, value) ||
			BGBCC_CCXL_IsRegImmDoubleP(ctx, value))
		{
			f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, value);
			(*(float *)(&j))=f;
			BGBCC_SHX_EmitDWord(sctx, j);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(bty==CCXL_TY_D)
	{
		if(sctx->is_addr64)
			BGBCC_SHX_EmitBAlign(sctx, 8);
		else
			BGBCC_SHX_EmitBAlign(sctx, 4);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, value);
			(*(double *)(&lj))=li;
			BGBCC_SHX_EmitQWord(sctx, lj);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmFloatP(ctx, value) ||
			BGBCC_CCXL_IsRegImmDoubleP(ctx, value))
		{
			f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, value);
			(*(double *)(&lj))=f;
			BGBCC_SHX_EmitQWord(sctx, lj);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((bty==CCXL_TY_SB) || (bty==CCXL_TY_UB))
	{
//		BGBCC_SHX_EmitBAlign(sctx, 4);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, value);
			BGBCC_SHX_EmitByte(sctx, i);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((bty==CCXL_TY_SS) || (bty==CCXL_TY_US))
	{
		BGBCC_SHX_EmitBAlign(sctx, 2);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, value);
			BGBCC_SHX_EmitWord(sctx, i);
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
			BGBCC_SHX_EmitQWord(sctx, li);
			BGBCC_SHX_EmitQWord(sctx, lj);
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
			BGBCC_SHX_EmitQWord(sctx, li);
			BGBCC_SHX_EmitQWord(sctx, lj);
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
			tty=litobj->decl->type;

			littyobj=NULL;
			i=BGBCC_CCXL_GetTypeBaseType(ctx, tty);
			if(i>=256)
				{ littyobj=ctx->literals[i-256]; }

			if(littyobj && (littyobj->littype==CCXL_LITID_STRUCT))
			{
				n=littyobj->decl->n_fields;
				if(litobj->decl->n_listdata<n)
					n=litobj->decl->n_listdata;
				
				for(i=0; i<n; i++)
				{
					BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
						littyobj->decl->fields[i]->type,
						litobj->decl->listdata[i]);
				}

				BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
				for(i=n; i<littyobj->decl->n_fields; i++)
				{
					BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
						littyobj->decl->fields[i]->type, treg);
				}
				return(1);
			}

			BGBCC_CCXL_StubError(ctx);
			return(0);
		}

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value) &&
			BGBCC_CCXL_IsRegImmZeroP(ctx, value))
		{
			sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
			BGBCC_SHX_EmitRawBytes(sctx, NULL, sz);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

ccxl_status BGBCC_SHXC_BuildGlobal(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	BGBCC_SHX_Context *sctx;
	BGBCC_CCXL_LiteralInfo *litobj;
	BGBCC_CCXL_LiteralInfo *littyobj;
	ccxl_register treg;
	ccxl_type tty;
	char *s0;
	int l0, sz, al, asz, iskv;
	s64 li;
	int n;
	int i, j, k;

	sctx=ctx->uctx;
	
	if(obj->regflags&BGBCC_REGFL_CULL)
		return(0);
	
	l0=obj->fxoffs;
	if(l0<=0)
	{
//		lbl=BGBCC_SHX_LookupNamedLabel(ctx, name);
//		l0=BGBCC_SHX_GenLabel(sctx);
		l0=BGBCC_SHX_GetNamedLabel(sctx, obj->name);
		obj->fxoffs=l0;
	}
	
	if(!obj->value.val)
	{
//		if(!strcmp(obj->name, "snd_scaletable"))
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

		BGBCC_SHX_EmitCommSym(sctx, l0, sz);

//		BGBCC_SHX_SetSectionName(sctx, ".bss");
//		BGBCC_SHX_EmitBAlign(sctx, 4);
//		BGBCC_SHX_EmitLabel(sctx, l0);
//		BGBCC_SHX_EmitRawBytes(sctx, NULL, sz);
		return(1);
	}
	
	sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->type);
	al=BGBCC_CCXL_TypeGetLogicalAlign(ctx, obj->type);

	if(al<1)al=1;
	if(sctx->is_addr64)
		{ if(al>8)al=8; }
	else
		{ if(al>4)al=4; }
	if(al&(al-1))al=4;

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
		BGBCC_SHX_SetSectionName(sctx, ".rodata");
	}else
	{
		BGBCC_SHX_SetSectionName(sctx, ".data");
	}

//	BGBCC_SHX_EmitBAlign(sctx, 4);
	BGBCC_SHX_EmitBAlign(sctx, al);
	BGBCC_SHX_EmitLabel(sctx, l0);
	
	if(BGBCC_CCXL_TypeSmallIntP(ctx, obj->type))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, obj->value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, obj->value);
			BGBCC_SHX_EmitDWord(sctx, i);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmLongP(ctx, obj->value))
		{
			i=BGBCC_CCXL_GetRegImmLongValue(ctx, obj->value);
			BGBCC_SHX_EmitDWord(sctx, i);
			return(1);
		}
	}

	if(BGBCC_CCXL_TypeSgLongP(ctx, obj->type) ||
		(BGBCC_CCXL_TypeSgNLongP(ctx, obj->type) && sctx->is_addr64))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, obj->value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, obj->value);
			BGBCC_SHX_EmitQWord(sctx, i);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmLongP(ctx, obj->value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, obj->value);
			BGBCC_SHX_EmitQWord(sctx, li);
			return(1);
		}
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, obj->type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, obj->type))
	{
		BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
			obj->type, obj->value);
		return(1);
	}

	if(BGBCC_CCXL_TypePointerP(ctx, obj->type))
	{
		if(BGBCC_CCXL_IsRegImmStringP(ctx, obj->value))
		{
			s0=BGBCC_CCXL_GetRegImmStringValue(ctx, obj->value);
			if(obj->type.val==CCXL_VTY_PCHAR)
				k=BGBCC_SHX_EmitGetStrtabLabelUTF2ASCII(sctx, s0);
			if(obj->type.val==CCXL_VTY_PWCHAR)
				k=BGBCC_SHX_EmitGetStrtabLabelUTF2UCS2(sctx, s0);				
			else
				k=BGBCC_SHX_EmitGetStrtabLabel(sctx, s0);
			if(sctx->is_addr64)
			{
				BGBCC_SHX_EmitRelocTy(sctx, k, BGBCC_SH_RLC_ABS64);
				BGBCC_SHX_EmitQWord(sctx, 0);
			}else
			{
				BGBCC_SHX_EmitRelocTy(sctx, k, BGBCC_SH_RLC_ABS32);
				BGBCC_SHX_EmitDWord(sctx, 0);
			}
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmGlobalAddrP(ctx, obj->value))
		{
			i=obj->value.val&CCXL_REGINT_MASK;
			k=BGBCC_SHXC_GetGblIndexLabel(ctx, sctx, i);
			if(sctx->is_addr64)
			{
				BGBCC_SHX_EmitRelocTy(sctx, k, BGBCC_SH_RLC_ABS64);
				BGBCC_SHX_EmitQWord(sctx, 0);
			}else
			{
				BGBCC_SHX_EmitRelocTy(sctx, k, BGBCC_SH_RLC_ABS32);
				BGBCC_SHX_EmitDWord(sctx, 0);
			}
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmIntP(ctx, obj->value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, obj->value);
			if(sctx->is_addr64)
				{ BGBCC_SHX_EmitQWord(sctx, i); }
			else
				{ BGBCC_SHX_EmitDWord(sctx, i); }
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmLongP(ctx, obj->value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, obj->value);
			if(sctx->is_addr64)
				{ BGBCC_SHX_EmitQWord(sctx, li); }
			else
				{ BGBCC_SHX_EmitDWord(sctx, li); }
			return(1);
		}
	}

	if(BGBCC_CCXL_IsRegImmLiteralP(ctx, obj->value))
	{
//		if(!strcmp(obj->name, "keynames"))
//		{
//			k=-1;
//		}
	
		BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
			obj->type, obj->value);
		return(1);
	}

	if(BGBCC_CCXL_TypeArrayP(ctx, obj->type))
	{
		BGBCC_CCXL_TypeDerefType(ctx, obj->type, &tty);

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
				BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}
			
			BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
			for(; i<asz; i++)
			{
				BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}

			if(asz<=0)
			{
				BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}
			return(1);
		}
	}
	
	BGBCC_CCXL_StubError(ctx);
	BGBCC_SHX_EmitRawBytes(sctx, NULL, sz);
	return(1);
}

ccxl_status BGBCC_SHXC_BuildAsmBlob(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	BGBCC_SHX_Context *sctx;
	int l0, sz;
	int i, j, k;

	sctx=ctx->uctx;
	
	BGBCC_SHXC_AssembleBuffer(ctx, sctx, (char *)(obj->text));
	return(1);
}

int BGBCC_SHXC_LookupLabelIndex(
	BGBCC_TransState *ctx, BGBCC_SHX_Context *sctx,
	int lblid)
{

	return(BGBCC_SHX_LookupLabelIndex(sctx, lblid));

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

int BGBCC_SHXC_LookupLabelImgOffs(
	BGBCC_TransState *ctx, BGBCC_SHX_Context *sctx,
	int lblid)
{
	int i, j, k;
	
	j=BGBCC_SHXC_LookupLabelIndex(ctx, sctx, lblid);
	k=sctx->sec_rva[sctx->lbl_sec[j]]+sctx->lbl_ofs[j];
	return(k);
}

int BGBCC_SHXC_LookupLabelImgVA(
	BGBCC_TransState *ctx, BGBCC_SHX_Context *sctx,
	int lblid)
{
	int i, j, k;
	
	j=BGBCC_SHXC_LookupLabelIndex(ctx, sctx, lblid);
	k=sctx->sec_lva[sctx->lbl_sec[j]]+sctx->lbl_ofs[j];
	return(k);
}

extern char *bgbcc_shx_srcidx[256];
extern int bgbcc_shx_nsrcidx;

ccxl_status BGBCC_SHXC_ApplyImageRelocs(
	BGBCC_TransState *ctx, BGBCC_SHX_Context *sctx,
	byte *imgbase)
{
	byte *ctl, *ctr;
	char *s0;
	int en;
	s64 val, var;
	s64 b, d, b1, d1, w0, w1;
	int i, j, k;

	en=(sctx->is_le==0);
	sctx->stat_ovlbl8=0;

	for(i=0; i<sctx->nrlc; i++)
	{
		j=BGBCC_SHXC_LookupLabelIndex(ctx, sctx, sctx->rlc_id[i]);
		if(j<0)
		{
			k=sctx->rlc_id[i];
			s0=BGBCC_SHX_LookupNameForLabel(sctx, k);
			if(s0)
			{
				printf("BGBCC_SHXC_ApplyImageRelocs: "
					"Missing Label N=%s\n", s0);
			}else
			{
				printf("BGBCC_SHXC_ApplyImageRelocs: "
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
			d1=b+d;
			if(((sbyte)d1)!=d1)
				__debugbreak();
			bgbcc_sets8en(ctr, en, d1);
			break;
		case BGBCC_SH_RLC_REL16:
			b=bgbcc_gets16en(ctr, en);
			d1=b+d;
			if(((s16)d1)!=d1)
				__debugbreak();
			bgbcc_sets16en(ctr, en, d1);
			break;
		case BGBCC_SH_RLC_REL32:
			b=bgbcc_gets32en(ctr, en);
			bgbcc_sets32en(ctr, en, b+d);
			break;
		case BGBCC_SH_RLC_REL32B:
			b=bgbcc_gets32en(ctr, en);
			bgbcc_sets32en(ctr, en, b-d);
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
			bgbcc_setu16en(ctr, en, (b&0xF000)|(d1&0x0FFF));
			break;
		case BGBCC_SH_RLC_RELW8:
			b=bgbcc_getu16en(ctr, en);
			b1=((s32)(b<<24))>>24;
//			d1=b1+(d-4);
			d1=b1+((d-4)>>1);
			if((((s32)(d1<<24))>>24)!=d1)
				__debugbreak();
			bgbcc_setu16en(ctr, en, (b&0xFF00)|(d1&0x00FF));
			break;

		case BGBCC_SH_RLC_RELW12L:
			b=bgbcc_getu16en(ctr, en);
			b1=((s32)(b<<20))>>20;
//			d1=b1+(d-4);
			d1=b1+((d-2)>>2);
//			if(((d1<<20)>>20)!=d1)
			if((d1&4095)!=d1)
				__debugbreak();
			bgbcc_setu16en(ctr, en, (b&0xF000)|(d1&0x0FFF));
			break;
		case BGBCC_SH_RLC_RELW8L:
			b=bgbcc_getu16en(ctr, en);
			b1=((s32)(b<<24))>>24;
//			d1=b1+(d-4);
			d1=b1+((d-2)>>2);
//			if(((d1<<24)>>24)!=d1)
			if((d1&255)!=d1)
				__debugbreak();
			bgbcc_setu16en(ctr, en, (b&0xFF00)|(d1&0x00FF));
			break;

		case BGBCC_SH_RLC_RELW16_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			
//			b=(w0&255)|((w1&255)<<8);
			b=((w0&255)<<8)|(w1&255);
			b1=(b<<16)>>16;

			d1=b1+((d-6)>>1);
			if((((s32)(d1<<16))>>16)!=d1)
				__debugbreak();
				
			if(((((s32)(d1<<24))>>24)==d1) &&
				((w1&0xFF00)!=0x8300))
					sctx->stat_ovlbl8++;
				
			w0=(w0&0xFF00)|((d1>>8)&0x00FF);
			w1=(w1&0xFF00)|((d1   )&0x00FF);
			bgbcc_setu16en(ctr+0, en, w0);
			bgbcc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_RELW20_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			
//			b=(w0&255)|((w1&255)<<8);
//			b=((w0&255)<<8)|(w1&255);
			b=((w0&255)<<12)|(w1&4095);
			b1=((s32)(b<<12))>>12;

			d1=b1+((d-6)>>1);
			if((((s32)(d1<<12))>>12)!=d1)
				__debugbreak();

			if((((s32)(d1<<24))>>24)==d1)
				sctx->stat_ovlbl8++;

			w0=(w0&0xFF00)|((d1>>12)&0x00FF);
			w1=(w1&0xF000)|((d1    )&0x0FFF);
			bgbcc_setu16en(ctr+0, en, w0);
			bgbcc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_REL24_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			b=((w0&255)<<16)|(w1&65535);
			b1=((s32)(b<<8))>>8;
			d1=b1+d;
			if((((s32)(d1<<8))>>8)!=d1)
				__debugbreak();
			w0=(w0&0xFF00)|((d1>>16)&0x00FF);
			w1=d1&0xFFFF;
			bgbcc_setu16en(ctr+0, en, w0);
			bgbcc_setu16en(ctr+2, en, w1);
			break;
		case BGBCC_SH_RLC_REL24B_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			b=((w0&255)<<16)|(w1&65535);
			b1=((s32)(b<<8))>>8;
			d1=b1-d;
			if((((s32)(d1<<8))>>8)!=d1)
				__debugbreak();
			w0=(w0&0xFF00)|((d1>>16)&0x00FF);
			w1=d1&0xFFFF;
			bgbcc_setu16en(ctr+0, en, w0);
			bgbcc_setu16en(ctr+2, en, w1);
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

ccxl_status BGBCC_SHXC_FlattenImage(BGBCC_TransState *ctx,
	byte *obuf, int *rosz, fourcc imgfmt)
{
	BGBCC_SHX_Context *sctx;
	BGBCC_CCXL_RegisterInfo *obj;
	BGBCC_CCXL_LiteralInfo *litobj;
	int l0;
	u32 addr;
	int i, j, k;

	sctx=ctx->uctx;
	
	sctx->stat_tot_dq0=0;
	sctx->stat_tot_dq1=0;
	sctx->stat_tot_dqi=0;

	if(!sctx->cgen_log)
		sctx->cgen_log=fopen("bgbcc_shxlog.txt", "wt");

	sctx->is_rom=0;
	if(imgfmt==BGBCC_IMGFMT_ROM)
	{
		sctx->is_rom=1;
		sctx->lbl_rom_data_strt=
			BGBCC_SHX_GetNamedLabel(sctx, "__rom_data_start");
		sctx->lbl_rom_data_end=
			BGBCC_SHX_GetNamedLabel(sctx, "__rom_data_end");
	}

	BGBCC_SHX_SetSectionName(sctx, ".text");
	BGBCC_SHX_EmitNamedLabel(sctx, "__text_start");

	BGBCC_SHX_SetSectionName(sctx, ".data");
	BGBCC_SHX_EmitNamedLabel(sctx, "__data_start");

	BGBCC_SHX_SetSectionName(sctx, ".bss");
	BGBCC_SHX_EmitNamedLabel(sctx, "__bss_start");
	
	sctx->lbl_got=BGBCC_SHX_GenLabel(sctx);

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

	for(i=0; i<ctx->n_reg_globals; i++)
	{
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
				
				BGBCC_CCXL_GlobalMarkReachable(ctx, obj);
				continue;
			}

			if(!strcmp(obj->name, "main"))
			{
				BGBCC_CCXL_GlobalMarkReachable(ctx, obj);
				continue;
			}
		}
	}

	for(i=0; i<ctx->n_reg_globals; i++)
	{
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

	for(i=0; i<ctx->n_reg_globals; i++)
	{
		obj=ctx->reg_globals[i];
		if(!obj)
			continue;

		if((obj->regtype==CCXL_LITID_FUNCTION) && (obj->vtr))
		{
			BGBCC_SHXC_BuildFunction(ctx, obj);
			continue;
		}

		if(obj->regtype==CCXL_LITID_GLOBALVAR)
		{
			BGBCC_SHXC_BuildGlobal(ctx, obj);
			continue;
		}

		if(obj->regtype==CCXL_LITID_STATICVAR)
		{
			BGBCC_SHXC_BuildGlobal(ctx, obj);
			continue;
		}

		if(obj->regtype==CCXL_LITID_ASMBLOB)
		{
			BGBCC_SHXC_BuildAsmBlob(ctx, obj);
			continue;
		}
	}

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
		printf("16=%.2f%% 8A=%.2f%% 8E=%.2f%% CE=%.2f%% "
				"CC0=%.2f%% CC3=%.2f%%\n",
			(100.0*sctx->stat_opc_base16)/k,
			(100.0*sctx->stat_opc_ext8a)/k,
			(100.0*sctx->stat_opc_ext8e)/k,
			(100.0*sctx->stat_opc_extCe)/k,
			(100.0*sctx->stat_opc_extCC0)/k,
			(100.0*sctx->stat_opc_extCC3)/k
			);
	}

	if(sctx->lvt16_n_idx>0)
	{
		BGBCC_SHX_SetSectionName(sctx, ".data");
		BGBCC_SHX_EmitBAlign(sctx, 16);
		for(i=0; i<sctx->lvt16_n_idx; i++)
		{
			BGBCC_SHX_EmitLabel(sctx, sctx->lvt16_lbl[i]);
			if(sctx->is_le)
			{
				BGBCC_SHX_EmitQWord(sctx, sctx->lvt16_val[i*2+0]);
				BGBCC_SHX_EmitQWord(sctx, sctx->lvt16_val[i*2+1]);
			}else
			{
				BGBCC_SHX_EmitQWord(sctx, sctx->lvt16_val[i*2+1]);
				BGBCC_SHX_EmitQWord(sctx, sctx->lvt16_val[i*2+0]);
			}
		}
	}

	if(sctx->got_n_gblidx>0)
	{
		BGBCC_SHX_SetSectionName(sctx, ".got");
		BGBCC_SHX_EmitBAlign(sctx, 4);
		BGBCC_SHX_EmitLabel(sctx, sctx->lbl_got);
		
		addr=0;
		for(i=0; i<sctx->got_n_gblidx; i++)
		{
			j=sctx->got_gblidx[i];
			l0=ctx->reg_globals[j]->fxoffs;
			BGBCC_SHX_EmitRelocAbs32(sctx, l0);
			BGBCC_SHX_EmitDWord(sctx, 0);
		}
	}

	BGBCC_SHX_SetSectionName(sctx, ".text");
	BGBCC_SHX_EmitNamedLabel(sctx, "__text_end");

	BGBCC_SHX_SetSectionName(sctx, ".data");
	BGBCC_SHX_EmitNamedLabel(sctx, "__data_end");

	BGBCC_SHX_SetSectionName(sctx, ".bss");
	BGBCC_SHX_EmitNamedLabel(sctx, "_end");
	BGBCC_SHX_EmitNamedLabel(sctx, "__bss_end");

	if((imgfmt==BGBCC_IMGFMT_EXE) ||
		(imgfmt==BGBCC_IMGFMT_DLL))
	{
		BGBCC_SHXC_FlattenImagePECOFF(ctx, obuf, rosz, imgfmt);
		return(1);
	}

	if((imgfmt==BGBCC_IMGFMT_ELXE) ||
		(imgfmt==BGBCC_IMGFMT_ELSO))
	{
		BGBCC_SHXC_FlattenImageELF(ctx, obuf, rosz, imgfmt);
		return(1);
	}

	if(imgfmt==BGBCC_IMGFMT_ROM)
	{
		BGBCC_SHXC_FlattenImageROM(ctx, obuf, rosz, imgfmt);
		return(1);
	}

	return(0);
}
