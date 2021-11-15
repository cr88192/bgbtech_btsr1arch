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
			BGBCC_CCXL_LoadslotCacheFlushReg(ctx, treg);
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
			BGBCC_CCXL_LoadslotCacheFlushReg(ctx, treg);
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
		BGBCC_CCXL_LoadslotCacheFlushReg(ctx, treg);
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

//	if(BGBCC_CCXL_TypeValueObjectP(ctx, bty))
	if(0)
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
//		BGBCC_CCXL_LoadslotCacheFlushReg(ctx, reg);
	
		i=(reg.val&CCXL_REGID_BASEMASK);
		if((i<0) || (i>=256))
		{
			if(i==CCXL_REGID_BASEMASK)
				return(CCXL_STATUS_NO);

			BGBCC_CCXL_TagError(ctx,
				CCXL_TERR_STATUS(CCXL_STATUS_ERR_CANTACQUIRE));
			return(CCXL_STATUS_ERR_BADVALUE);
		}
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

//		BGBCC_CCXL_LoadslotCacheFlushReg(ctx, reg);

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

ccxl_status BGBCC_CCXL_RegisterIsTempFree(
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
//		if(ri->ucnt<255)
//			ri->ucnt++;
		if(ri->ucnt<=0)
			return(CCXL_STATUS_YES);
		return(CCXL_STATUS_NO);
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

ccxl_status BGBCC_CCXL_LoadslotCacheFlush(
	BGBCC_TransState *ctx)
{
	ctx->loadslot_cache_srov=ctx->loadslot_cache_erov;
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_LoadslotCacheFlushStorePtr(
	BGBCC_TransState *ctx, ccxl_register dreg)
{
	ccxl_register ireg;
	ccxl_type dty, bty, ssty;
	int sr, er;
	int i;

	dty=BGBCC_CCXL_GetRegType(ctx, dreg);
	
//	if(ctx->opt_ptrcache<2)
	if((ctx->opt_ptrcache<2) &&
		!BGBCC_CCXL_TypeRestrictPointerP(ctx, dty))
	{
		BGBCC_CCXL_LoadslotCacheFlush(ctx);
		return(CCXL_STATUS_YES);
	}

	sr=ctx->loadslot_cache_srov;
	er=ctx->loadslot_cache_erov;
	ireg.val=CCXL_REGID_REG_Z;

	if(sr==er)
		return(CCXL_STATUS_NO);

	dty=BGBCC_CCXL_GetRegType(ctx, dreg);
	if(BGBCC_CCXL_TypeRestrictPointerP(ctx, dty))
		return(CCXL_STATUS_NO);

//	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, dty))
		BGBCC_CCXL_TypeDerefType(ctx, dty, &dty);

	i=sr;
	while(i!=er)
	{
		ssty=BGBCC_CCXL_GetRegType(ctx, ctx->loadslot_cache_sreg[i]);
		if(BGBCC_CCXL_TypeRestrictPointerP(ctx, ssty))
			continue;

		bty=BGBCC_CCXL_GetRegType(ctx, ctx->loadslot_cache_dreg[i]);
//		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, bty))
//			BGBCC_CCXL_TypeDerefType(ctx, bty, &bty);

		if(BGBCC_CCXL_TypeCompatibleP(ctx, dty, bty))
		{
			ctx->loadslot_cache_st[i]=NULL;
			ctx->loadslot_cache_ireg[i]=ireg;
		}
		
		i=(i+1)&255;
	}

	return(CCXL_STATUS_NO);
//	BGBCC_CCXL_LoadslotCacheFlush(ctx);
//	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_LoadslotCacheFlushStoreSlot(
	BGBCC_TransState *ctx, ccxl_register dreg,
	BGBCC_CCXL_LiteralInfo *st, char *name,
	ccxl_register sreg)
{
	char *sn;
	ccxl_register ireg;
	ccxl_type dty, bty, ssty;
	int sr, er;
	int i;

	sr=ctx->loadslot_cache_srov;
	er=ctx->loadslot_cache_erov;
	
	if(sr==er)
		return(CCXL_STATUS_NO);

	dty=BGBCC_CCXL_GetRegType(ctx, dreg);

//	if(ctx->opt_ptrcache<2)
	if((ctx->opt_ptrcache<2) &&
		!BGBCC_CCXL_TypeRestrictPointerP(ctx, dty))
	{
		BGBCC_CCXL_LoadslotCacheFlush(ctx);
		return(CCXL_STATUS_YES);
	}

	ireg.val=CCXL_REGID_REG_Z;

	i=sr;
	while(i!=er)
	{
		ssty=BGBCC_CCXL_GetRegType(ctx, ctx->loadslot_cache_sreg[i]);
		if(BGBCC_CCXL_TypeRestrictPointerP(ctx, ssty))
			continue;

		sn=ctx->loadslot_cache_name[i];

		if(
//			BGBCC_CCXL_RegisterIdentEqualP(ctx,
//				ctx->loadslot_cache_sreg[i], dreg) &&
			(ctx->loadslot_cache_st[i]==st) &&
			sn && !strcmp(sn, name)	)
		{
			ctx->loadslot_cache_st[i]=NULL;
			ctx->loadslot_cache_ireg[i]=ireg;
		}
		
		i=(i+1)&255;
	}

	return(CCXL_STATUS_NO);

//	BGBCC_CCXL_LoadslotCacheFlushReg(ctx, dreg);
}

ccxl_status BGBCC_CCXL_LoadslotCacheFlushReg(
	BGBCC_TransState *ctx, ccxl_register sreg)
{
	ccxl_register ireg;
	int sr, er;
	int i;

	if(sreg.val==CCXL_REGID_REG_Z)
		return(CCXL_STATUS_NO);

	sr=ctx->loadslot_cache_srov;
	er=ctx->loadslot_cache_erov;
	
	if(sr==er)
		return(CCXL_STATUS_YES);

	ireg.val=CCXL_REGID_REG_Z;

	i=sr;
	while(i!=er)
	{
//		if(	(ctx->loadslot_cache_sreg[i]==sreg) ||
//			(ctx->loadslot_cache_dreg[i]==sreg)	)
		if(	BGBCC_CCXL_RegisterIdentEqualP(ctx,
				ctx->loadslot_cache_sreg[i], sreg) ||
			BGBCC_CCXL_RegisterIdentEqualP(ctx,
				ctx->loadslot_cache_dreg[i], sreg) ||
			BGBCC_CCXL_RegisterIdentEqualP(ctx,
				ctx->loadslot_cache_ireg[i], sreg)	)
		{
			ctx->loadslot_cache_ireg[i]=ireg;
			ctx->loadslot_cache_st[i]=NULL;
		}
		
		i=(i+1)&255;
	}

//	ctx->loadslot_cache_srov=ctx->loadslot_cache_erov;
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_LoadslotCacheAdd(
	BGBCC_TransState *ctx,
	ccxl_register dreg, ccxl_register sreg,
	BGBCC_CCXL_LiteralInfo *st, char *name)
{
	ccxl_register ireg;
	byte erov;

	if(ctx->opt_ptrcache==0)
		return(CCXL_STATUS_NO);
	
	if(BGBCC_CCXL_IsRegVolatileP(ctx, sreg))
		return(CCXL_STATUS_NO);
	if(BGBCC_CCXL_IsRegVolatileP(ctx, dreg))
		return(CCXL_STATUS_NO);

	ireg.val=CCXL_REGID_REG_Z;

	erov=ctx->loadslot_cache_erov++;
	ctx->loadslot_cache_dreg[erov]=dreg;
	ctx->loadslot_cache_sreg[erov]=sreg;
	ctx->loadslot_cache_ireg[erov]=ireg;
	ctx->loadslot_cache_st	[erov]=st;
	ctx->loadslot_cache_name[erov]=name;
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_LoadslotCacheCheck(
	BGBCC_TransState *ctx,
	ccxl_register sreg,
	BGBCC_CCXL_LiteralInfo *st, char *name,
	ccxl_register *rdreg2)
{
	char *sn;
	int sr, er;
	int i;

//	return(CCXL_STATUS_NO);

	sr=ctx->loadslot_cache_srov;
	er=ctx->loadslot_cache_erov;
	
	if(sr==er)
		return(CCXL_STATUS_NO);

	if(ctx->opt_ptrcache==0)
		return(CCXL_STATUS_NO);

	i=sr;
	while(i!=er)
	{
		sn=ctx->loadslot_cache_name[i];
		if(	(ctx->loadslot_cache_sreg[i].val==sreg.val) &&
			(ctx->loadslot_cache_st  [i]==st  ) &&
			sn && !strcmp(sn, name)	)
		{
			*rdreg2=ctx->loadslot_cache_dreg[i];
			return(CCXL_STATUS_YES);
		}
		
		i=(i+1)&255;
	}

	return(CCXL_STATUS_NO);
}

ccxl_status BGBCC_CCXL_LoadIndexCacheAdd(
	BGBCC_TransState *ctx,
	ccxl_register dreg, ccxl_register sreg, ccxl_register ireg)
{
	byte erov;

	if(ctx->opt_ptrcache==0)
		return(CCXL_STATUS_NO);

	if(BGBCC_CCXL_IsRegVolatileP(ctx, sreg))
		return(CCXL_STATUS_NO);
	if(BGBCC_CCXL_IsRegVolatileP(ctx, dreg))
		return(CCXL_STATUS_NO);
	if(BGBCC_CCXL_IsRegVolatileP(ctx, ireg))
		return(CCXL_STATUS_NO);

	erov=ctx->loadslot_cache_erov++;
	ctx->loadslot_cache_dreg[erov]=dreg;
	ctx->loadslot_cache_sreg[erov]=sreg;
	ctx->loadslot_cache_ireg[erov]=ireg;
	ctx->loadslot_cache_st	[erov]=NULL;
	ctx->loadslot_cache_name[erov]=NULL;
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_LoadIndexCacheCheck(
	BGBCC_TransState *ctx,
	ccxl_register sreg,
	ccxl_register ireg,
	ccxl_register *rdreg2)
{
	int sr, er;
	int i;

//	return(CCXL_STATUS_NO);

	sr=ctx->loadslot_cache_srov;
	er=ctx->loadslot_cache_erov;
	
	if(sr==er)
		return(CCXL_STATUS_NO);

	if(ctx->opt_ptrcache==0)
		return(CCXL_STATUS_NO);

	if(BGBCC_CCXL_IsRegVolatileP(ctx, sreg))
		return(CCXL_STATUS_NO);

	i=sr;
	while(i!=er)
	{
		if(	(ctx->loadslot_cache_sreg[i].val==sreg.val) &&
			(ctx->loadslot_cache_ireg[i].val==ireg.val) )
		{
			*rdreg2=ctx->loadslot_cache_dreg[i];
			return(CCXL_STATUS_YES);
		}
		
		i=(i+1)&255;
	}

	return(CCXL_STATUS_NO);
}

ccxl_status BGBCC_CCXL_LoadIndexConstCacheAdd(
	BGBCC_TransState *ctx,
	ccxl_register dreg, ccxl_register sreg, int idx)
{
	ccxl_register ireg;
	byte erov;
	BGBCC_CCXL_GetRegForIntValue(ctx, &ireg, idx);
	return(BGBCC_CCXL_LoadIndexCacheAdd(ctx, dreg, sreg, ireg));
}

ccxl_status BGBCC_CCXL_LoadIndexConstCacheCheck(
	BGBCC_TransState *ctx,
	ccxl_register sreg, int idx,
	ccxl_register *rdreg2)
{
	ccxl_register ireg;
	byte erov;
	BGBCC_CCXL_GetRegForIntValue(ctx, &ireg, idx);
	return(BGBCC_CCXL_LoadIndexCacheCheck(ctx, sreg, ireg, rdreg2));
}
