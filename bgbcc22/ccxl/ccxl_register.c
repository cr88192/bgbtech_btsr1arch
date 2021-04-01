#include <bgbccc.h>

BGBCC_CCXL_RegisterInfo *BGBCC_CCXL_AllocRegisterInfo(
	BGBCC_TransState *ctx)
{
	BGBCC_CCXL_RegisterInfo *rtmp;

	if(ctx->reginfo_free)
	{
		rtmp=ctx->reginfo_free;
		ctx->reginfo_free=rtmp->next;
		memset(rtmp, 0, sizeof(BGBCC_CCXL_RegisterInfo));
		return(rtmp);
	}
	
	rtmp=bgbcc_malloc(sizeof(BGBCC_CCXL_RegisterInfo));
	return(rtmp);
}

bool BGBCC_CCXL_FreeRegisterInfo(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *rinf)
{
	rinf->next=ctx->reginfo_free;
	ctx->reginfo_free=rinf;
	return(true);
}

BGBCC_CCXL_RegisterInfo *BGBCC_CCXL_CloneRegisterInfo(
	BGBCC_TransState *ctx, BGBCC_CCXL_RegisterInfo *rinf)
{
	BGBCC_CCXL_RegisterInfo *tmp;
	tmp=BGBCC_CCXL_AllocRegisterInfo(ctx);
	memcpy(tmp, rinf, sizeof(BGBCC_CCXL_RegisterInfo));
	return(tmp);
}

ccxl_status BGBCC_CCXL_RegisterAllocTemporary(
	BGBCC_TransState *ctx, ccxl_type bty,
	ccxl_register *rtreg)
{
	return(BGBCC_CCXL_RegisterAllocTemporaryLLn(ctx, bty, rtreg, NULL, 0));
}

	
ccxl_status BGBCC_CCXL_RegisterAllocTemporaryLLn(
	BGBCC_TransState *ctx, ccxl_type bty,
	ccxl_register *rtreg, char *fn, int ln)
{
	BGBCC_CCXL_RegisterInfo *ri;
	ccxl_register treg;
	int i, j, k, z, bi, rov, nr;

	BGBCC_CCXL_MarkTypeAccessed(ctx, bty);

	z=BGBCC_CCXL_GetTypeOperationBaseZ(ctx, bty);

	if(!ctx->cur_func->regs)
	{
		ctx->cur_func->regs=bgbcc_malloc(
			256*sizeof(BGBCC_CCXL_RegisterInfo *));
//		ctx->cur_func->regs_tyseq=bgbcc_malloc(256*sizeof(u32));
		ctx->cur_func->n_regs=0;
		ctx->cur_func->m_regs=256;
	}

#if 0
	if(ctx->cur_func->n_regs>=16)
	{
		printf("RegAllocDbg:\n");
		for(i=0; i<ctx->cur_func->n_regs; i++)
		{
			ri=ctx->cur_func->regs[i];
			if(ri)
			{
				printf("    R%d: %s:%d\n", i,
					ri->alc_fn, ri->alc_ln);
			}
		}
	}
#endif

	rov=ctx->regrov;
	ctx->regrov=rov+1;
	bi=-1;

	nr=ctx->cur_func->n_regs;
	for(i=0; i<nr; i++)
	{
		j=(i+rov)%nr;

		ri=ctx->cur_func->regs[j];
		if(!ri)
			continue;

		if(!ri->ucnt && (ri->type_zb==z))
		{
			if((BGBCC_CCXL_TypeValueObjectP(ctx, bty) ||
				BGBCC_CCXL_TypeValueObjectP(ctx, ri->type)) &&
				!BGBCC_CCXL_TypeEqualP(ctx, bty, ri->type))
					continue;

			bi=j;
			break;
		}
	}

	
//	for(i=0; i<256; i++)
	for(i=0; i<ctx->cur_func->m_regs; i++)
	{
//		j=ctx->cur_func->regs_tyseq[i];
//		if(j && (((j>>16)&15)!=z))
//			continue;

		ri=ctx->cur_func->regs[i];
//		if(!ctx->cur_func->regs[i])
		if(!ri)
		{
			if(i<ctx->cur_func->n_regs)
				BGBCC_DBGBREAK

//			if(!j)
//				{ ctx->cur_func->regs_tyseq[i]=(z<<16)|1; }
//			else
//				{ ctx->cur_func->regs_tyseq[i]=++j; }
		
			if(i>=ctx->cur_func->n_regs)
				ctx->cur_func->n_regs=i+1;
		
			ri=BGBCC_CCXL_AllocRegisterInfo(ctx);
			ri->alc_fn=fn;
			ri->alc_ln=ln;

			ri->ucnt=1;
			ri->cseq=1;
			ri->type_zb=z;

			ri->type=bty;
//			ri->regid=i|((j&4095)<<12);
			ri->regid=i|((ri->cseq&4095)<<12);

			ctx->cur_func->regs[i]=ri;
			treg.val=CCXL_REGTY_TEMP|
				(((u64)bty.val)<<CCXL_REGID_TYPESHIFT)|
				ri->regid;
			*rtreg=treg;
			return(i);
		}
		
		if(!ri->ucnt && (ri->type_zb==z))
		{
			if((BGBCC_CCXL_TypeValueObjectP(ctx, bty) ||
				BGBCC_CCXL_TypeValueObjectP(ctx, ri->type)) &&
				!BGBCC_CCXL_TypeEqualP(ctx, bty, ri->type))
					continue;

			bi=i;
			break;

#if 0
			if((BGBCC_CCXL_TypeValueObjectP(ctx, bty) ||
				BGBCC_CCXL_TypeValueObjectP(ctx, ri->type)) &&
				!BGBCC_CCXL_TypeEqualP(ctx, bty, ri->type))
					continue;
		
			ri->cseq++;
//			ri->type_zb=z;

			ri->type=bty;
			ri->regid=i|((ri->cseq&4095)<<12);
			ri->ucnt=1;

			treg.val=CCXL_REGTY_TEMP|
				(((u64)bty.val)<<CCXL_REGID_TYPESHIFT)|
				ri->regid;
			*rtreg=treg;
			return(i);
#endif
		}
	}
	
	if(bi>=0)
	{
		ri=ctx->cur_func->regs[bi];

//		if((BGBCC_CCXL_TypeValueObjectP(ctx, bty) ||
//			BGBCC_CCXL_TypeValueObjectP(ctx, ri->type)) &&
//			!BGBCC_CCXL_TypeEqualP(ctx, bty, ri->type))
//				continue;
	
		ri->cseq++;
//		ri->type_zb=z;

		ri->type=bty;
		ri->regid=bi|((ri->cseq&4095)<<12);
		ri->ucnt=1;

		treg.val=CCXL_REGTY_TEMP|
			(((u64)bty.val)<<CCXL_REGID_TYPESHIFT)|
			ri->regid;
		*rtreg=treg;
		return(bi);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_CANTACQUIRE));
	return(-1);
}

ccxl_status BGBCC_CCXL_RegisterAllocTemporaryInit(
	BGBCC_TransState *ctx, ccxl_type bty, ccxl_register *rtreg)
{
	BGBCC_CCXL_RegisterInfo *ri;
	BGBCC_CCXL_LiteralInfo *st;
	ccxl_status rt;
	int i, j, k;

	rt=BGBCC_CCXL_RegisterAllocTemporary(
		ctx, bty, rtreg);
	if(rt<0)return(rt);

	if(BGBCC_CCXL_TypeValueObjectP(ctx, bty))
	{
		st=BGBCC_CCXL_LookupStructureForType(ctx, bty);
		if(st)
			{ BGBCC_CCXL_EmitInitObj(ctx, bty, *rtreg, st); }
//		*rtreg.val|=CCXL_REGFL_INIT;

		i=rtreg->val&CCXL_REGID_BASEMASK;
		ri=ctx->cur_func->regs[i];
		ri->regflags|=BGBCC_REGFL_INITIALIZED;
	}

	return(rt);
}

ccxl_status BGBCC_CCXL_RegisterAllocTemporaryInt(
	BGBCC_TransState *ctx, ccxl_register *rtreg)
{
	return(BGBCC_CCXL_RegisterAllocTemporary(
		ctx, BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_I), rtreg));
}

ccxl_status BGBCC_CCXL_RegisterAllocTemporaryLong(
	BGBCC_TransState *ctx, ccxl_register *rtreg)
{
	return(BGBCC_CCXL_RegisterAllocTemporary(
		ctx, BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_L), rtreg));
}

ccxl_status BGBCC_CCXL_RegisterAllocTemporaryFloat(
	BGBCC_TransState *ctx, ccxl_register *rtreg)
{
	return(BGBCC_CCXL_RegisterAllocTemporary(
		ctx, BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_F), rtreg));
}

ccxl_status BGBCC_CCXL_RegisterAllocTemporaryDouble(
	BGBCC_TransState *ctx, ccxl_register *rtreg)
{
	return(BGBCC_CCXL_RegisterAllocTemporary(
		ctx, BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_D), rtreg));
}

ccxl_status BGBCC_CCXL_RegisterAllocTemporaryPointer(
	BGBCC_TransState *ctx, ccxl_register *rtreg)
{
	return(BGBCC_CCXL_RegisterAllocTemporary(
		ctx, BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_P), rtreg));
}

ccxl_status BGBCC_CCXL_RegisterAllocTemporaryMatch(
	BGBCC_TransState *ctx, ccxl_register reg, ccxl_register *rtreg)
{
	return(BGBCC_CCXL_RegisterAllocTemporary(
		ctx, BGBCC_CCXL_GetRegType(ctx, reg), rtreg));
}

ccxl_status BGBCC_CCXL_RegisterAllocTemporaryMatchInit(
	BGBCC_TransState *ctx, ccxl_register reg, ccxl_register *rtreg)
{
	return(BGBCC_CCXL_RegisterAllocTemporaryInit(
		ctx, BGBCC_CCXL_GetRegType(ctx, reg), rtreg));
}

ccxl_status BGBCC_CCXL_RegisterCheckRelease(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	BGBCC_CCXL_RegisterInfo *ri;
	BGBCC_CCXL_LiteralInfo *st;
	int i;

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
	{
		i=(reg.val&CCXL_REGID_BASEMASK);
		if((i<0) || (i>=256))
			return(CCXL_STATUS_ERR_BADVALUE);
		ri=ctx->cur_func->regs[i];
		if(ri->ucnt && (ri->ucnt<255))
			ri->ucnt--;
		if(ri->ucnt>0)
			return(CCXL_STATUS_NO);

		if(ri->regflags&BGBCC_REGFL_INITIALIZED)
		{
			if(BGBCC_CCXL_TypeValueObjectP(ctx, ri->type))
			{
				st=BGBCC_CCXL_LookupStructureForType(ctx, ri->type);
				if(st)
					{ BGBCC_CCXL_EmitDropObj(ctx, ri->type, reg, st); }
			}
		}

//		ctx->cur_func->regs[i]=NULL;
//		BGBCC_CCXL_FreeRegisterInfo(ctx, ri);
		return(CCXL_STATUS_YES);
	}

	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_LONG)
		{ return(CCXL_STATUS_NO); }
	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_DOUBLE)
		{ return(CCXL_STATUS_NO); }

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG)
		{ return(CCXL_STATUS_NO); }
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL)
		{ return(CCXL_STATUS_NO); }
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_GLOBAL)
		{ return(CCXL_STATUS_NO); }

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_THISIDX)
		{ return(CCXL_STATUS_NO); }

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT)
		{ return(CCXL_STATUS_NO); }
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG)
		{ return(CCXL_STATUS_NO); }
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_FLOAT)
		{ return(CCXL_STATUS_NO); }
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE)
		{ return(CCXL_STATUS_NO); }
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_STRING)
		{ return(CCXL_STATUS_NO); }

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG_LVT)
		{ return(CCXL_STATUS_NO); }
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE_LVT)
		{ return(CCXL_STATUS_NO); }
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_I128_LVT)
		{ return(CCXL_STATUS_NO); }
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_F128_LVT)
		{ return(CCXL_STATUS_NO); }

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_CANTRELEASE));
	return(CCXL_STATUS_ERR_CANTRELEASE);
}

ccxl_status BGBCC_CCXL_RegisterCheckAcquire(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	BGBCC_CCXL_RegisterInfo *ri;
	int i;

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
	{
		i=(reg.val&CCXL_REGID_BASEMASK);
		if((i<0) || (i>=256))
			return(CCXL_STATUS_ERR_BADVALUE);
		ri=ctx->cur_func->regs[i];
		if(ri->ucnt<255)
			ri->ucnt++;
		return(CCXL_STATUS_YES);
	}
	
	return(CCXL_STATUS_NO);
//	return(CCXL_STATUS_ERR_CANTACQUIRE);
}

ccxl_status BGBCC_CCXL_RegisterIdentEqualP(
	BGBCC_TransState *ctx, ccxl_register sreg, ccxl_register treg)
{
	u64 stxv;
	
	stxv=sreg.val^treg.val;
//	if((sreg.val&CCXL_REGTY_REGMASK)!=(treg.val&CCXL_REGTY_REGMASK))
//	if(((sreg.val^treg.val)&CCXL_REGTY_REGMASK)!=0)
	if((stxv&CCXL_REGTY_REGMASK)!=0)
		return(CCXL_STATUS_NO);
	
//	if(	((sreg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP) ||
//		((sreg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG) ||
//		((sreg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL))
	if((sreg.val&CCXL_REGTY_REGMASK)<=CCXL_REGTY_LOCAL)
	{
//		if((sreg.val&CCXL_REGID_BASEMASK)!=(treg.val&CCXL_REGID_BASEMASK))
//		if(((sreg.val^treg.val)&CCXL_REGID_BASEMASK)!=0)
		if((stxv&CCXL_REGID_BASEMASK)!=0)
			return(CCXL_STATUS_NO);
		return(CCXL_STATUS_YES);
	}
	
//	if(sreg.val!=treg.val)
	if(stxv!=0)
		return(CCXL_STATUS_NO);
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_RegisterSequenceEqualP(
	BGBCC_TransState *ctx, ccxl_register sreg, ccxl_register treg)
{
	if((sreg.val&CCXL_REGTY_REGMASK)!=(treg.val&CCXL_REGTY_REGMASK))
		return(CCXL_STATUS_NO);
	
	if(	((sreg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP) ||
		((sreg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG) ||
		((sreg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL))
	{
		if((sreg.val&CCXL_REGID_BASEMASK)!=(treg.val&CCXL_REGID_BASEMASK))
			return(CCXL_STATUS_NO);
		if((sreg.val&CCXL_REGID_SEQMASK)!=(treg.val&CCXL_REGID_SEQMASK))
			return(CCXL_STATUS_NO);
		return(CCXL_STATUS_YES);
	}
	
	if(sreg.val!=treg.val)
		return(CCXL_STATUS_NO);
	return(CCXL_STATUS_YES);
}
