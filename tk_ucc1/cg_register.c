int TKUCC_CgenNmidForType(TKUCC_MainContext *ctx, tkucc_valtype sty)
{
	int nmid;

	if(sty.val<=TKUCC_VALTYPE_TY_UL)
	{
		nmid=-1;
		switch(sty.val)
		{
		case TKUCC_VALTYPE_TY_SB:	nmid=TKUCC_NMID_MOVB;	break;
		case TKUCC_VALTYPE_TY_UB:	nmid=TKUCC_NMID_MOVUB;	break;
		case TKUCC_VALTYPE_TY_SS:	nmid=TKUCC_NMID_MOVW;	break;
		case TKUCC_VALTYPE_TY_US:	nmid=TKUCC_NMID_MOVUW;	break;
		case TKUCC_VALTYPE_TY_SI:	nmid=TKUCC_NMID_MOVL;	break;
		case TKUCC_VALTYPE_TY_UI:	nmid=TKUCC_NMID_MOVUL;	break;
		case TKUCC_VALTYPE_TY_SNL:	nmid=TKUCC_NMID_MOVQ;	break;
		case TKUCC_VALTYPE_TY_UNL:	nmid=TKUCC_NMID_MOVQ;	break;
		case TKUCC_VALTYPE_TY_SL:	nmid=TKUCC_NMID_MOVQ;	break;
		case TKUCC_VALTYPE_TY_UL:	nmid=TKUCC_NMID_MOVQ;	break;
		}
		return(nmid);
	}
	
	if(TKUCC_TypePointerP(ctx, sty))
	{
		return(TKUCC_NMID_MOVQ);
	}
	
	return(-1);
}

int TKUCC_CgenLoadFrameReg(TKUCC_MainContext *ctx,
	tkucc_valreg val, int reg)
{
	TKUCC_IRDECL *decl;
	tkucc_valtype ty;
	u64 tv;
	int nmid;

	if(TKUCC_VregIsLocalP(ctx, val))
	{
		decl=TKUCC_CgenLookupLocalIndex(ctx, val.val&0xFFF);
		nmid=TKUCC_CgenNmidForType(ctx, decl->ty);
		if(nmid>0)
		{
			TKUCC_CgenEmitOp_LdDispReg(ctx, nmid, TKUCC_REG_SP, decl->ofs, reg);
			return(1);
		}
	}

	if(TKUCC_VregIsGlobalP(ctx, val))
	{
		decl=TKUCC_CgenLookupGlobalIndex(ctx, val.val&0xFFFFFF);
		TKUCC_CgenEmitOp_LblReg(ctx, TKUCC_NMID_MOV, decl->lbl, reg);
		return(1);
	}

	if(TKUCC_VregIsSmallintP(ctx, val))
	{
		ty=TKUCC_VregGetType(ctx, val);
		
		if(TKUCC_TypeSmallLongP(ctx, ty))
		{
			if(TKUCC_TypeUnsignedP(ctx, ty))
				{ tv=(u32)val.val; }
			else
				{ tv=(s32)val.val; }
			TKUCC_CgenEmitOp_ImmReg(ctx, TKUCC_NMID_MOV, tv, reg);
			return(1);
		}
	}

	return(0);
}

int TKUCC_CgenStoreFrameReg(TKUCC_MainContext *ctx,
	tkucc_valreg val, int reg)
{
}

int TKUCC_CgenGetValueAsRegister(TKUCC_MainContext *ctx,
	tkucc_valreg val, int accfl)
{
	int i, j, k;
	
	for(i=0; i<64; i++)
	{
		if(TKUCC_VregCheckIdentP(ctx, ctx->relalc_val[i], val))
		{
			ctx->regalc_lcnt[i]++;
			return(i);
		}
	}

	k=ctx->regalc_rov;
	for(i=0; i<64; i++)
	{
		if(!((TKUCC_REGMASK_PRESERVE>>k)&1))
		{
			k=(k+1)&63;
			continue;
		}
		if(!((ctx->regalc_regsaved>>k)&1))
		{
			k=(k+1)&63;
			continue;
		}
		if(ctx->regalc_lcnt[k])
		{
			k=(k+1)&63;
			continue;
		}
		
		if((ctx->regalc_regdirty>>k)&1)
		{
			TKUCC_CgenStoreFrameReg(ctx, ctx->relalc_val[k], k);
			ctx->regalc_regdirty&=~(1ULL<<k);
		}
		
		TKUCC_CgenLoadFrameReg(ctx, val, k);
		ctx->relalc_val[k]=val;
		ctx->regalc_lcnt[k]=1;
		ctx->regalc_rov=(k+1)&63;
		return(k);
	}

	k=ctx->regalc_rov;
	for(i=0; i<64; i++)
	{
		if(!((TKUCC_REGMASK_PRESERVE>>k)&1))
		{
			k=(k+1)&63;
			continue;
		}
		if(!((ctx->regalc_regcansave>>k)&1))
		{
			k=(k+1)&63;
			continue;
		}
		
		ctx->regalc_regsaved|=1ULL<<k;
		
		TKUCC_CgenLoadFrameReg(ctx, val, k);
		ctx->relalc_val[k]=val;
		ctx->regalc_lcnt[k]=1;
		ctx->regalc_rov=(k+1)&63;
		return(k);
	}
}

int TKUCC_CgenGetValueAsRegisterSrc(TKUCC_MainContext *ctx,
	tkucc_valreg val)
{
	return(TKUCC_CgenGetValueAsRegister(ctx, val, 1));
}

int TKUCC_CgenGetValueAsRegisterDst(TKUCC_MainContext *ctx,
	tkucc_valreg val)
{
	return(TKUCC_CgenGetValueAsRegister(ctx, val, 2));
}

int TKUCC_CgenGetValueAsRegisterDirty(TKUCC_MainContext *ctx,
	tkucc_valreg val)
{
	return(TKUCC_CgenGetValueAsRegister(ctx, val, 3));
}

int TKUCC_CgenReleaseRegister(TKUCC_MainContext *ctx, int reg)
{
	if(ctx->regalc_lcnt[reg]>0)
		ctx->regalc_lcnt[reg]--;
}
