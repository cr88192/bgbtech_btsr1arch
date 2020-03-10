#include <bgbccc.h>

void BGBCC_CCXL_DebugPrintStackLLn(
	BGBCC_TransState *ctx, char *op, char *file, int line)
{
	ccxl_register reg;
	int i;

	return;

	printf("%16s:%4d(%10s) (RP=%d): ",
		file, line, op, ctx->cur_func->n_regs);

	for(i=0; i<ctx->regstackpos; i++)
	{
		reg=ctx->regstack[i];
		
		if(BGBCC_CCXL_IsRegRegExtP(ctx, reg))
		{
			printf("R%d ", BGBCC_CCXL_GetRegID(ctx, reg));
			continue;
		}

		if(BGBCC_CCXL_IsRegArgExtP(ctx, reg))
		{
			printf("A%d ", BGBCC_CCXL_GetRegID(ctx, reg));
			continue;
		}

		if(BGBCC_CCXL_IsRegLocalExtP(ctx, reg))
		{
			printf("L%d ", BGBCC_CCXL_GetRegID(ctx, reg));
			continue;
		}

		if(BGBCC_CCXL_IsRegGlobalP(ctx, reg))
		{
			printf("G%d ", BGBCC_CCXL_GetRegID(ctx, reg));
			continue;
		}

		if(BGBCC_CCXL_IsRegImmIntP(ctx, reg))
		{
			printf("I:%d ", BGBCC_CCXL_GetRegImmIntValue(ctx, reg));
			continue;
		}

		if(BGBCC_CCXL_IsRegImmLongP(ctx, reg))
		{
			printf("L:%d ", (int)BGBCC_CCXL_GetRegImmLongValue(ctx, reg));
			continue;
		}

		if(BGBCC_CCXL_IsRegImmFloatP(ctx, reg))
		{
			printf("F:%f ", BGBCC_CCXL_GetRegImmFloatValue(ctx, reg));
			continue;
		}

		if(BGBCC_CCXL_IsRegImmDoubleP(ctx, reg))
		{
			printf("D:%f ", BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg));
			continue;
		}

		if(BGBCC_CCXL_IsRegImmStringP(ctx, reg))
		{
			printf("S:%d ", (int)(reg.val&CCXL_REGINT_MASK));
			continue;
		}
		
		printf("?(%08X%08X)", (u32)(reg.val>>32), (u32)reg.val);
	}
	
	printf("--\n");
}

ccxl_status BGBCC_CCXL_CheckExpandStack(BGBCC_TransState *ctx)
{
	int rt;
	int i;

	rt=CCXL_STATUS_NO;
	if(!ctx->markstack)
	{
		i=64;
		ctx->markstack=bgbcc_malloc(i*sizeof(int));
		ctx->markstackpos=0;
		rt=CCXL_STATUS_YES;
	}

	if(!ctx->regstack)
	{
		i=256;
		ctx->regstack=bgbcc_malloc(i*sizeof(ccxl_register));
		ctx->regstackpos=0;
		rt=CCXL_STATUS_YES;
	}

	if(!ctx->uregstack)
	{
		i=16;
		ctx->uregstack=bgbcc_malloc(i*sizeof(ccxl_register));
		ctx->uregstackpos=0;
		rt=CCXL_STATUS_YES;
	}
	
	if(ctx->regstackpos>=256)
	{
		BGBCC_DBGBREAK
	}

	if(ctx->markstackpos>=64)
	{
		BGBCC_DBGBREAK
	}

	if(ctx->uregstackpos>=16)
	{
		BGBCC_DBGBREAK
	}
	
	return(rt);
}

ccxl_status BGBCC_CCXL_PushRegister(BGBCC_TransState *ctx, ccxl_register reg)
{
	int i;

//	if(reg.val==0x000000036f001002)
//		__debugbreak();

	BGBCC_CCXL_CheckExpandStack(ctx);
	
	i=ctx->regstackpos++;
	ctx->regstack[i]=reg;
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_PopRegister(BGBCC_TransState *ctx,
	ccxl_register *rreg)
{
	int i;

	if(!ctx->regstack || (ctx->regstackpos<=0))
	{
		rreg->val=0;
		BGBCC_CCXL_TagError(ctx, CCXL_TERR_STACK_UNDERFLOW);
		return(CCXL_STATUS_NO);
	}

	i=--ctx->regstackpos;
	*rreg=ctx->regstack[i];
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_PeekRegister(BGBCC_TransState *ctx,
	ccxl_register *rreg)
{
	int i;

	if(!ctx->regstack || (ctx->regstackpos<=0))
	{
		rreg->val=0;
		BGBCC_CCXL_TagError(ctx, CCXL_TERR_STACK_UNDERFLOW);
		return(CCXL_STATUS_NO);
	}

	i=ctx->regstackpos-1;
	*rreg=ctx->regstack[i];
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_PeekRegisterIdx(BGBCC_TransState *ctx,
	ccxl_register *rreg, int idx)
{
	int i;

	if(!ctx->regstack || (ctx->regstackpos<=0))
	{
		rreg->val=0;
		BGBCC_CCXL_TagError(ctx, CCXL_TERR_STACK_UNDERFLOW);
		return(CCXL_STATUS_NO);
	}

	i=ctx->regstackpos-(idx+1);
	*rreg=ctx->regstack[i];
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_PeekRegisterMarkIdx(BGBCC_TransState *ctx,
	ccxl_register *rreg, int idx)
{
	int i, j, k;

	if(!ctx->regstack || (ctx->regstackpos<=0))
	{
		rreg->val=0;
		BGBCC_CCXL_TagError(ctx, CCXL_TERR_STACK_UNDERFLOW);
		return(CCXL_STATUS_NO);
	}

	if(!ctx->markstack || (ctx->markstackpos<=0))
	{
		rreg->val=0;
		BGBCC_CCXL_TagError(ctx, CCXL_TERR_STACK_UNDERFLOW);
		return(CCXL_STATUS_NO);
	}

	i=ctx->markstackpos-1;
	j=ctx->markstack[i];
	k=j-(idx+1);
	*rreg=ctx->regstack[k];
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_PushURegister(BGBCC_TransState *ctx,
	ccxl_register reg)
{
	int i;

	BGBCC_CCXL_CheckExpandStack(ctx);
	
	i=ctx->uregstackpos++;
	ctx->uregstack[i]=reg;
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_PopURegister(BGBCC_TransState *ctx,
	ccxl_register *rreg)
{
	int i;

	if(!ctx->uregstack || ctx->uregstackpos<=0)
	{
		rreg->val=0;
		BGBCC_CCXL_TagError(ctx, CCXL_TERR_STACK_UNDERFLOW);
		return(CCXL_STATUS_NO);
	}

	i=--ctx->uregstackpos;
	*rreg=ctx->uregstack[i];
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_PeekURegister(BGBCC_TransState *ctx,
	ccxl_register *rreg)
{
	int i;

	if(!ctx->uregstack || ctx->uregstackpos<=0)
	{
		BGBCC_CCXL_TagError(ctx, CCXL_TERR_STACK_UNDERFLOW);
		return(CCXL_STATUS_NO);
	}

	*rreg=ctx->uregstack[ctx->uregstackpos-1];
	return(CCXL_STATUS_YES);
}


ccxl_status BGBCC_CCXL_PushMark(BGBCC_TransState *ctx)
{
	int i;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_MARK);

	BGBCC_CCXL_CheckExpandStack(ctx);
	
	i=ctx->markstackpos++;
	ctx->markstack[i]=ctx->regstackpos;
	return(CCXL_STATUS_YES);
}

int BGBCC_CCXL_LookupArgIndex(BGBCC_TransState *ctx, char *name)
{
	int i;
	
	for(i=0; i<ctx->cur_func->n_args; i++)
		if(!strcmp(ctx->cur_func->args[i]->name, name))
			return(i);
	return(-1);
}

int BGBCC_CCXL_LookupLocalIndex(BGBCC_TransState *ctx, char *name)
{
	int i;
	
	for(i=0; i<ctx->cur_func->n_locals; i++)
	{
		if(!ctx->cur_func->locals[i])
			continue;
		if(!strcmp(ctx->cur_func->locals[i]->name, name))
			return(i);
	}
	return(-1);
}

int BGBCC_CCXL_LookupStaticIndex(BGBCC_TransState *ctx, char *name)
{
	int i;
	
	for(i=0; i<ctx->cur_func->n_statics; i++)
	{
		if(!ctx->cur_func->statics[i])
			continue;
		if(!strcmp(ctx->cur_func->statics[i]->name, name))
			return(i);
	}
	return(-1);
}

int BGBCC_CCXL_LookupGlobalIndex(BGBCC_TransState *ctx, char *name)
{
	char tb[256];
	BGBCC_CCXL_RegisterInfo *gbl;
	int i;

#if 0
	for(i=(ctx->n_imp-1); i>=0; i--)
	{
		sprintf(tb, "%s/%s", ctx->imp_ns[i], name);

		gbl=BGBCC_CCXL_LookupGlobal(ctx, tb);
		if(gbl)
		{
			return(gbl->gblid);
		}
	}
#endif

	gbl=BGBCC_CCXL_LookupGlobal(ctx, name);
	if(gbl)
		return(gbl->gblid);

//	gbl=BGBCC_CCXL_LookupGlobalFull(ctx, name);
//	if(gbl)
//		return(gbl->gblid);

	return(-1);

#if 0
	for(i=0; i<ctx->n_reg_globals; i++)
	{
		if(!ctx->reg_globals[i])
			continue;

		if(ctx->reg_globals[i]->defp)
		{
			if(ctx->reg_globals[i]->defp!=ctx->cur_func)
				continue;
		}
		
		if(!ctx->reg_globals[i]->name)
			continue;
		
		if(!strcmp(ctx->reg_globals[i]->name, name))
			return(i);
	}
	return(-1);
#endif
}

int BGBCC_CCXL_LookupGlobalIndexSig(
	BGBCC_TransState *ctx, char *name, char *sig)
{
	char tb[256];
	BGBCC_CCXL_RegisterInfo *gbl;
	int i;

	for(i=(ctx->n_imp-1); i>=0; i--)
	{
		sprintf(tb, "%s/%s", ctx->imp_ns[i], name);

		gbl=BGBCC_CCXL_LookupGlobalMatchSig(ctx, tb, sig);
		if(gbl)
		{
			return(gbl->gblid);
		}
	}

	gbl=BGBCC_CCXL_LookupGlobalMatchSig(ctx, name, sig);
	if(!gbl)
		return(-1);
	return(gbl->gblid);
}

int BGBCC_CCXL_LookupAsRegister(BGBCC_TransState *ctx,
	char *name, ccxl_register *rreg)
{
//	char *s1;
	int i;

	i=BGBCC_CCXL_TryLookupAsRegister(ctx, name, rreg, false);
	if(i>0)return(i);

	BGBCC_CCXL_Error(ctx, "Undeclared Variable '%s'\n", name);

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_NO));

	return(CCXL_STATUS_NO);
}

int BGBCC_CCXL_LookupAsRegisterSig(BGBCC_TransState *ctx,
	char *name, char *sig, ccxl_register *rreg)
{
//	char *s1;
	int i;

	i=BGBCC_CCXL_TryLookupAsRegisterSig(ctx, name, sig, rreg, false);
	if(i>0)return(i);

	BGBCC_CCXL_Error(ctx, "Undeclared Variable '%s'\n", name);

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_NO));

	return(CCXL_STATUS_NO);
}

int BGBCC_CCXL_LookupAsRegisterStore(BGBCC_TransState *ctx,
	char *name, ccxl_register *rreg)
{
	int i;

	i=BGBCC_CCXL_TryLookupAsRegister(ctx, name, rreg, true);
	if(i>0)return(i);

	BGBCC_CCXL_Error(ctx, "Undeclared Variable '%s'\n", name);

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_NO));

	return(CCXL_STATUS_NO);
}

int BGBCC_CCXL_TryLookupAsRegister(BGBCC_TransState *ctx,
	char *name, ccxl_register *rreg, bool store)
{
	return(BGBCC_CCXL_TryLookupAsRegisterSig(
		ctx, name, NULL, rreg, store));
}

BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_TryGetThisObj(BGBCC_TransState *ctx)
{
	char b1[256], b2[256];
	BGBCC_CCXL_LiteralInfo *obj;
	ccxl_register treg;
	ccxl_type ty;
	char *s1;
	int i0, i1, i2, i3;
	int i, j;

	obj=NULL;

	if(ctx->cur_func->flagsint&BGBCC_TYFL_VIRTUAL)
	{
		obj=BGBCC_CCXL_LookupStructureForType(
			ctx, ctx->cur_func->clz_type);
	}else if(ctx->cur_func->thisstr)
	{
		obj=BGBCC_CCXL_LookupStructure(ctx,
			ctx->cur_func->thisstr);
		if(obj)
		{
			ctx->cur_func->flagsint|=BGBCC_TYFL_VIRTUAL;
			ctx->cur_func->clz_type=
				BGBCC_CCXL_MakeTypeID(ctx, obj->litid+256);
		}
	}else
	{
		strcpy(b1, ctx->cur_func->name);
		s1=b1+strlen(b1);
		while((s1>b1) && (*s1!='/'))
			s1--;
		*s1=0;
		
		if(b1[0])
		{
			obj=BGBCC_CCXL_LookupStructure(ctx, b1);

			if(obj)
			{
				ctx->cur_func->flagsint|=BGBCC_TYFL_VIRTUAL;
				ctx->cur_func->clz_type=
					BGBCC_CCXL_MakeTypeID(ctx, obj->litid+256);
			}
		}
	}
	
	return(obj);
}

int BGBCC_CCXL_TryGetThisType(BGBCC_TransState *ctx,
	ccxl_type *rty)
{
	if(!(ctx->cur_func->flagsint&BGBCC_TYFL_VIRTUAL))
		BGBCC_CCXL_TryGetThisObj(ctx);

	if(ctx->cur_func->flagsint&BGBCC_TYFL_VIRTUAL)
	{
		*rty=ctx->cur_func->clz_type;
		return(CCXL_STATUS_YES);
	}

	rty->val=0;
	return(CCXL_STATUS_NO);
}

int BGBCC_CCXL_TryGetSuperType(BGBCC_TransState *ctx,
	ccxl_type *rty)
{
	BGBCC_CCXL_LiteralInfo *obj;
	ccxl_type tty;

	obj=BGBCC_CCXL_TryGetThisObj(ctx);
	if(obj)
	{
		obj=BGBCC_CCXL_GetStructSuperclass(ctx, obj);
		if(obj)
		{
			tty=BGBCC_CCXL_MakeTypeID(ctx, obj->litid+256);
			*rty=tty;
			return(CCXL_STATUS_YES);
		}
	}

	rty->val=0;
	return(CCXL_STATUS_NO);
}

int BGBCC_CCXL_TryLookupAsRegisterSig(BGBCC_TransState *ctx,
	char *name, char *sig, ccxl_register *rreg, bool store)
{
	char b1[256], b2[256];
	BGBCC_CCXL_LiteralInfo *obj;
	BGBCC_CCXL_RegisterInfo *gbl;
	ccxl_register treg;
	ccxl_type ty;
	char *s1;
	int i0, i1, i2, i3;
	int i, j, k;

	i=BGBCC_CCXL_LookupLocalIndex(ctx, name);
	if(i>=0)
	{
		if(store)
			ctx->cur_func->locals[i]->cseq++;
	
		ty=ctx->cur_func->locals[i]->type;
		j=i|((ctx->cur_func->locals[i]->cseq&4095)<<12);
		treg.val=CCXL_REGTY_LOCAL|
			(((s64)ty.val)<<CCXL_REGID_TYPESHIFT)|j;
		BGBCC_CCXL_RegisterCheckAcquire(ctx, treg);
		*rreg=treg;
		return(CCXL_STATUS_YES);
	}

	i=BGBCC_CCXL_LookupArgIndex(ctx, name);
	if(i>=0)
	{
		if(store)
			ctx->cur_func->args[i]->cseq++;

		ty=ctx->cur_func->args[i]->type;
		j=i|((ctx->cur_func->args[i]->cseq&4095)<<12);
		treg.val=CCXL_REGTY_ARG|
			(((s64)ty.val)<<CCXL_REGID_TYPESHIFT)|j;
		BGBCC_CCXL_RegisterCheckAcquire(ctx, treg);
		*rreg=treg;
		return(CCXL_STATUS_YES);
	}
	
	if(!(ctx->cur_func->flagsint&BGBCC_TYFL_STATIC))
	{
//		obj=NULL;

		obj=BGBCC_CCXL_TryGetThisObj(ctx);

		if(obj)
		{		
			i=BGBCC_CCXL_LookupStructSuperFieldType(
				ctx, obj, name,
				&i0, &i1, &ty);

			if(i>0)
			{
				treg.val=CCXL_REGTY_THISIDX|
					(((s64)ty.val)<<CCXL_REGID_TYPESHIFT)|
					((i1>> 4)&CCXL_REGTHIS_FIDTYMASK)|
					((i0<<12)&CCXL_REGTHIS_LVLMASK)|
					((i1    )&CCXL_REGTHIS_FIDBMASK);
				BGBCC_CCXL_RegisterCheckAcquire(ctx, treg);
				*rreg=treg;
				return(CCXL_STATUS_YES);
			}
		}
	}

	i=-1;

	if((i<0) && (ctx->cur_struct || ctx->cur_ns))
	{
		s1=BGBCC_CCXL_QualifyNameNS(ctx, name);
		if(sig)
			i=BGBCC_CCXL_LookupGlobalIndexSig(ctx, s1, sig);
		else
			i=BGBCC_CCXL_LookupGlobalIndex(ctx, s1);
	}

	if(i<0)
	{
		for(j=(ctx->n_imp-1); j>=0; j--)
		{
			sprintf(b1, "%s/%s", ctx->imp_ns[j], name);
			if(sig)
				k=BGBCC_CCXL_LookupGlobalIndexSig(ctx, b1, sig);
			else
				k=BGBCC_CCXL_LookupGlobalIndex(ctx, b1);
			if(k>0)
				{ i=k; break; }
		}
	}

	if(i<0)
	{
		if(sig)
			i=BGBCC_CCXL_LookupGlobalIndexSig(ctx, name, sig);
		else
			i=BGBCC_CCXL_LookupGlobalIndex(ctx, name);
	}

	if(i<0)
	{
		s1=BGBCC_CCXL_QualifyNameNSFl(ctx, name, BGBCC_TYFL_STATIC);
		if(sig)
			i=BGBCC_CCXL_LookupGlobalIndexSig(ctx, s1, sig);
		else
			i=BGBCC_CCXL_LookupGlobalIndex(ctx, s1);
	}

	if(i<0)
	{
		gbl=BGBCC_CCXL_LookupGlobalFull(ctx, name);
		if(gbl)
			i=gbl->gblid;
	}

	if(i<0)
	{
		s1=BGBCC_CCXL_QualifyNameNSFl(ctx, name, BGBCC_TYFL_STATIC);
		gbl=BGBCC_CCXL_LookupGlobalFull(ctx, s1);
		if(gbl)
			i=gbl->gblid;
	}

	if(i>=0)
	{
		ty=ctx->reg_globals[i]->type;

//		ctx->reg_globals[i]->regflags|=
//			BGBCC_REGFL_ACCESSED;
		treg.val=CCXL_REGTY_GLOBAL|
			(((s64)ty.val)<<CCXL_REGID_TYPESHIFT)|i;
		BGBCC_CCXL_RegisterCheckAcquire(ctx, treg);
		*rreg=treg;
		return(CCXL_STATUS_YES);
	}

	rreg->val=0;
//	BGBCC_DBGBREAK
	return(CCXL_STATUS_NO);
}

BGBCC_CCXL_RegisterInfo *BGBCC_CCXL_LookupLocalInfo(
	BGBCC_TransState *ctx, char *name)
{
	BGBCC_CCXL_RegisterInfo *def;
	int i;
	
	for(i=0; i<ctx->cur_func->n_locals; i++)
	{
		if(!ctx->cur_func->locals[i])
			continue;
		if(!strcmp(ctx->cur_func->locals[i]->name, name))
			return(ctx->cur_func->locals[i]);
	}
	return(NULL);
}

BGBCC_CCXL_RegisterInfo *BGBCC_CCXL_LookupArgInfo(
	BGBCC_TransState *ctx, char *name)
{
	BGBCC_CCXL_RegisterInfo *def;
	int i;
	
	for(i=0; i<ctx->cur_func->n_args; i++)
	{
		if(!ctx->cur_func->args[i])
			continue;
		if(!strcmp(ctx->cur_func->args[i]->name, name))
			return(ctx->cur_func->args[i]);
	}
	return(NULL);
}

BGBCC_CCXL_RegisterInfo *BGBCC_CCXL_LookupGlobalInfo(
	BGBCC_TransState *ctx, char *name)
{
	BGBCC_CCXL_RegisterInfo *def;
	int i;
	
	i=BGBCC_CCXL_LookupGlobalIndex(ctx, name);
	def=ctx->reg_globals[i];
	return(def);
}

ccxl_status BGBCC_CCXL_PushLoad(BGBCC_TransState *ctx, char *name)
{
	ccxl_register treg;
	int i;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "Load", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LOAD);
	BGBCC_CCXLR3_EmitArgString(ctx, name);

#if 0
	if((ctx->lang==BGBCC_LANG_CPP) ||
		(ctx->lang==BGBCC_LANG_BS2) ||
		(ctx->lang==BGBCC_LANG_CS))
	{
		if(!strcmp(name, "this"))
		{
			BGBCC_CCXL_PushThis(ctx);
			return(CCXL_STATUS_YES);
		}

		if(!strcmp(name, "null"))
		{
			treg.val=CCXL_REGSP_NULL;
			BGBCC_CCXL_PushRegister(ctx, treg);
			return(CCXL_STATUS_YES);
		}
	}

	if(	(ctx->lang==BGBCC_LANG_BS2) )
	{
		if(!strcmp(name, "super"))
		{
			BGBCC_CCXL_PushSuper(ctx);
			return(CCXL_STATUS_YES);
		}
	}
#endif
	
	if((name[0]=='_') && (name[1]=='_'))
	{
		if(!strcmp(name, "__object_this"))
		{
			BGBCC_CCXL_PushThis(ctx);
			return(CCXL_STATUS_YES);
		}

		if(!strcmp(name, "__object_super"))
		{
			BGBCC_CCXL_PushSuper(ctx);
			return(CCXL_STATUS_YES);
		}

		if(!strcmp(name, "__object_null"))
		{
			treg.val=CCXL_REGSP_NULL;
			BGBCC_CCXL_PushRegister(ctx, treg);
			return(CCXL_STATUS_YES);
		}
	}

	i=BGBCC_CCXL_LookupAsRegister(ctx, name, &treg);
	if(i<=0)
	{
		if(i==CCXL_STATUS_NO)
			return(CCXL_STATUS_ERR_LOOKUPFAIL);
		return(CCXL_STATUS_ERR_GENERIC);
	}
	
	BGBCC_CCXL_PushRegister(ctx, treg);
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_PopStore(BGBCC_TransState *ctx, char *name)
{
	ccxl_register sreg, treg, dreg;
	ccxl_type bty, dty, sty;
	int i, j, k;
	
	BGBCC_CCXL_DebugPrintStackLLn(ctx, "Store", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_STORE);
	BGBCC_CCXLR3_EmitArgString(ctx, name);

	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	j=BGBCC_CCXL_LookupAsRegisterStore(ctx, name, &dreg);
	if((i<0) || (j<=0))
	{
		if(!j)return(CCXL_STATUS_ERR_LOOKUPFAIL);
		if(i<0)return(i);
		if(j<0)return(j);
		return(CCXL_STATUS_ERR_GENERIC);
	}

	dty=BGBCC_CCXL_GetRegType(ctx, dreg);
	sty=BGBCC_CCXL_GetRegType(ctx, sreg);

//	if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty, sty))
	if(!BGBCC_CCXL_TypeCompatibleArchP(ctx, dty, sty))
	{
		BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &treg);
		BGBCC_CCXL_EmitConv(ctx, dty, sty, treg, sreg);
		k=BGBCC_CCXL_EmitMov(ctx, dty, dreg, treg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
		return(k);
	}

	k=BGBCC_CCXL_EmitMov(ctx, dty, dreg, sreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	return(k);
}

ccxl_status BGBCC_CCXL_PushThis(BGBCC_TransState *ctx)
{
	ccxl_register treg;
	ccxl_type bty;
	
	BGBCC_CCXL_TryGetThisType(ctx, &bty);
	
	treg.val=CCXL_REGSP_THIS |
		(((u64)(bty.val))<<CCXL_REGID_TYPESHIFT);
	BGBCC_CCXL_PushRegister(ctx, treg);
	return(CCXL_STATUS_YES);
//	return(BGBCC_CCXL_PushLoad(ctx, "this"));
}

ccxl_status BGBCC_CCXL_PushSuper(BGBCC_TransState *ctx)
{
	ccxl_register treg;
	ccxl_type bty;
	
	BGBCC_CCXL_TryGetSuperType(ctx, &bty);
	
	treg.val=CCXL_REGSP_SUPER |
		(((u64)(bty.val))<<CCXL_REGID_TYPESHIFT);
	BGBCC_CCXL_PushRegister(ctx, treg);
	return(CCXL_STATUS_YES);
//	return(BGBCC_CCXL_PushLoad(ctx, "this"));
}

ccxl_status BGBCC_CCXL_MovLoadStore(BGBCC_TransState *ctx,
	char *dname, char *sname)
{
	ccxl_register sreg, treg, dreg;
	ccxl_type bty, dty, sty;
	int i, j, k;
	
	BGBCC_CCXL_DebugPrintStackLLn(ctx, "MovLoadStore", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_MOVLDST);
	BGBCC_CCXLR3_EmitArgString(ctx, dname);
	BGBCC_CCXLR3_EmitArgString(ctx, sname);

	i=BGBCC_CCXL_LookupAsRegister(ctx, sname, &sreg);
	j=BGBCC_CCXL_LookupAsRegisterStore(ctx, dname, &dreg);
	if((i<=0) || (j<=0))
	{
		if(!i || !j)
			return(CCXL_STATUS_ERR_LOOKUPFAIL);
		if(i<0)return(i);
		if(j<0)return(j);
		return(CCXL_STATUS_ERR_GENERIC);
	}

	dty=BGBCC_CCXL_GetRegType(ctx, dreg);
	sty=BGBCC_CCXL_GetRegType(ctx, sreg);

//	if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty, sty))
	if(!BGBCC_CCXL_TypeCompatibleArchP(ctx, dty, sty))
	{
		BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &treg);
		BGBCC_CCXL_EmitConv(ctx, dty, sty, treg, sreg);
		k=BGBCC_CCXL_EmitMov(ctx, dty, dreg, treg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
		return(k);
	}

	k=BGBCC_CCXL_EmitMov(ctx, dty, dreg, sreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	return(k);
}

ccxl_status BGBCC_CCXL_StackTransforCallArgs(BGBCC_TransState *ctx)
{
	ccxl_register tarr[64];
	ccxl_register treg, dreg;
	int ms;
	int i, j, k, n, na;
	
	ms=ctx->markstack[ctx->markstackpos-1];
	n=ctx->regstackpos-ms;

	na=0;
	while(ctx->regstackpos>ms)
	{
		i=BGBCC_CCXL_PopRegister(ctx, &treg);
		BGBCC_CCXL_EmitCallArg(ctx, treg);
//		BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
		tarr[na++]=treg;
	}

	for(i=0; i<na; i++)
	{
		BGBCC_CCXL_RegisterCheckRelease(ctx, tarr[i]);
	}

	ctx->markstackpos--;
	return(CCXL_STATUS_YES);
}

int BGBCC_CCXL_StackGetCntCallArgs(BGBCC_TransState *ctx)
{
	ccxl_register treg, dreg;
	ccxl_type bty, dty, sty;
	int ms, ps, n, n2;

	ms=ctx->markstack[ctx->markstackpos-1];
	ps=ctx->regstackpos;
	n=ps-ms;

#if 1
	n2=0;
	while(ps>ms)
	{
		ps--;
		dreg=ctx->regstack[ps];
		sty=BGBCC_CCXL_GetRegType(ctx, dreg);

		if(BGBCC_CCXL_IsRegDoubleP(ctx, dreg) ||
			BGBCC_CCXL_IsRegSgLongP(ctx, dreg))
				{ n2+=2; }
		else
				{ n2+=1; }
	}
#endif

//	if(n>ctx->cur_func->n_cargs)
//		ctx->cur_func->n_cargs=n;
	if(n2>ctx->cur_func->n_cargs)
		ctx->cur_func->n_cargs=n2;

	return(n);
}

int BGBCC_CCXL_StackGetConvCallArgs(BGBCC_TransState *ctx,
	ccxl_register fnreg)
{
	BGBCC_CCXL_LiteralInfo *lobj;
	BGBCC_CCXL_RegisterInfo *rfn;
	ccxl_register treg, dreg;
	ccxl_type bty, dty, sty;
	int ms, ps;
	int i, j, k, n, an;
	
	rfn=NULL;
	
	if(BGBCC_CCXL_IsRegGlobalP(ctx, fnreg))
	{
		i=BGBCC_CCXL_GetRegID(ctx, fnreg);
		rfn=ctx->reg_globals[i];
		
//		if(rfn->regtype==CCXL_LITID_GLOBALVAR)
		if(	(rfn->regtype==CCXL_LITID_GLOBALVAR) ||
			(rfn->regtype==CCXL_LITID_STATICVAR) )
		{
			if(BGBCC_CCXL_TypeFunctionP(ctx, rfn->type))
			{
				i=BGBCC_CCXL_TypeObjectLiteralIndex(ctx, rfn->type);
				lobj=ctx->literals[i];
				rfn=lobj->decl;
			}
		}
	}

	if(!rfn)
	{
		bty=BGBCC_CCXL_GetRegType(ctx, fnreg);
		if(BGBCC_CCXL_TypeFunctionP(ctx, bty))
		{
			i=BGBCC_CCXL_TypeObjectLiteralIndex(ctx, bty);
			lobj=ctx->literals[i];
			rfn=lobj->decl;
		}
	}
	
	if((rfn->regtype!=CCXL_LITID_FUNCTION) &&
		(rfn->regtype!=CCXL_LITID_PROTOTYPE))
	{
		BGBCC_DBGBREAK
	}
	
	if(!rfn)
	{
		BGBCC_DBGBREAK
	}

	ms=ctx->markstack[ctx->markstackpos-1];
	ps=ctx->regstackpos;
	n=ps-ms;

	an=0;
	while(ps>ms)
	{
		ps--;
		dreg=ctx->regstack[ps];
		sty=BGBCC_CCXL_GetRegType(ctx, dreg);

		if(an>=rfn->n_args)
		{
			if(BGBCC_CCXL_IsRegFloatP(ctx, dreg) ||
				BGBCC_CCXL_IsRegFloat16P(ctx, dreg) ||
				BGBCC_CCXL_IsRegBFloat16P(ctx, dreg))
			{
				dty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
				BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &treg);
				BGBCC_CCXL_EmitConv(ctx, dty, sty, treg, dreg);
				ctx->regstack[ps]=treg;
			}
		}else
		{
			dty=rfn->args[an]->type;
			
			treg=dreg;
			k=BGBCC_JX2C_NormalizeImmVRegInt(ctx, NULL, dty, &treg);
			if(k)
			{
				ctx->regstack[ps]=treg;
			}
			else
			
//			if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty, sty))
			if(!BGBCC_CCXL_TypeCompatibleArchP(ctx, dty, sty))
			{
//				if(dty.val>0x3FFF)
//					{ BGBCC_DBGBREAK }
				if((dty.val==2) && (sty.val&0xF000))
					{ BGBCC_DBGBREAK }
				if((sty.val==2) && (dty.val&0xF000))
					{ BGBCC_DBGBREAK }
			
//				if(dty.val==0x1008)
//					{ BGBCC_DBGBREAK }
			
				BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &treg);
				BGBCC_CCXL_EmitConv(ctx, dty, sty, treg, dreg);
				ctx->regstack[ps]=treg;
			}
		}

		an++;
	}
	return(n);
}

char *BGBCC_CCXL_StackGetSigCallArgs(BGBCC_TransState *ctx)
{
	char tb[256];
	ccxl_register treg, dreg;
	ccxl_type bty, dty, sty;
	char *ct, *s0;
	int ms, ps, n, n2;

	ms=ctx->markstack[ctx->markstackpos-1];
	ps=ctx->regstackpos;
	n=ps-ms;

	ct=tb;
	*ct++='(';

	n2=0;
	while(ps>ms)
	{
		ps--;
		dreg=ctx->regstack[ps];
		sty=BGBCC_CCXL_GetRegType(ctx, dreg);
		
		s0=BGBCC_CCXL_TypeGetSig(ctx, sty);
		strcpy(ct, s0);
		ct+=strlen(ct);
	}

	*ct++=')';
	*ct++=0;

	return(bgbcc_strdup(tb));
}

char *BGBCC_CCXL_StackGetSigUnary(BGBCC_TransState *ctx,
	ccxl_type lty)
{
	char tb[256];
	char *s0, *ct;

	ct=tb;
	*ct++='(';

	s0=BGBCC_CCXL_TypeGetSig(ctx, lty);
	strcpy(ct, s0);
	ct+=strlen(ct);

	*ct++=')';
	*ct++=0;

	return(bgbcc_strdup(tb));
}

char *BGBCC_CCXL_StackGetSigBinary(BGBCC_TransState *ctx,
	ccxl_type lty, ccxl_type rty)
{
	char tb[256];
	char *s0, *ct;

	ct=tb;
	*ct++='(';

	s0=BGBCC_CCXL_TypeGetSig(ctx, lty);
	strcpy(ct, s0);
	ct+=strlen(ct);

	s0=BGBCC_CCXL_TypeGetSig(ctx, rty);
	strcpy(ct, s0);
	ct+=strlen(ct);

	*ct++=')';
	*ct++=0;

	return(bgbcc_strdup(tb));
}

char *BGBCC_CCXL_StackGetNameBinaryOverload(
	BGBCC_TransState *ctx,
	char *op)
{
	char *top;

	if(!strcmp(op, "+"))
		top="__operator_add";
	if(!strcmp(op, "-"))
		top="__operator_sub";
	if(!strcmp(op, "*"))
		top="__operator_mul";
	if(!strcmp(op, "/"))
		top="__operator_div";
	if(!strcmp(op, "%"))
		top="__operator_mod";

	if(!strcmp(op, "&"))
		top="__operator_and";
	if(!strcmp(op, "|"))
		top="__operator_or";
	if(!strcmp(op, "^"))
		top="__operator_xor";

	if(!strcmp(op, "=="))
		top="__operator_eq";
	if(!strcmp(op, "!="))
		top="__operator_ne";
	if(!strcmp(op, "<"))
		top="__operator_lt";
	if(!strcmp(op, ">"))
		top="__operator_gt";
	if(!strcmp(op, "<="))
		top="__operator_le";
	if(!strcmp(op, ">="))
		top="__operator_ge";
	
	return(top);
}

char *BGBCC_CCXL_StackGetMangleNameBinaryOverload(
	BGBCC_TransState *ctx,
	char *op,
	ccxl_type lty, ccxl_type rty)
{
	char tb[256];
	char *top;
	char *tsig;
	
//	top=NULL;
	top=BGBCC_CCXL_StackGetNameBinaryOverload(ctx, op);
	
	if(!top)
		return(NULL);
		
	tsig=BGBCC_CCXL_StackGetSigBinary(ctx, lty, rty);
	
	sprintf(tb, "%s%s", top, tsig);

	return(bgbcc_strdup(tb));
}

ccxl_status BGBCC_CCXL_StackCallName(BGBCC_TransState *ctx,
	char *name, int flag)
{
	return(BGBCC_CCXL_StackCallName2(ctx, name, NULL, flag));
}

int BGBCC_CCXL_CheckFuncNameInstrinsicP(BGBCC_TransState *ctx, char *name)
{
	if(!name)
		return(0);

	if(!strcmp(name, "__debugbreak")		||
		!strcmp(name, "__halt")				||
		!strcmp(name, "__hint_use_egpr")	)
		return(1);

	if(	!strcmp(name, "__int_min")			||
		!strcmp(name, "__int_max")			||
		!strcmp(name, "__int_clamp")		)
			return(1);

	return(0);
}

ccxl_status BGBCC_CCXL_StackCallName2(BGBCC_TransState *ctx,
	char *name, char *dname, int flag)
{
	ccxl_register treg, dreg, dreg2, oreg;
	ccxl_type bty, dty;
	BGBCC_CCXL_RegisterInfo *ri;
	char *tsig;
	int is_intrin;
	int i, j, k, n;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "CallName", __FILE__, __LINE__);

	if(flag&4)
	{
		if(dname)
		{
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_STOBJCALLN);
			BGBCC_CCXLR3_EmitArgString(ctx, name);
			BGBCC_CCXLR3_EmitArgString(ctx, dname);
		}else if(flag&1)
		{
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_OBJCALLNV);
			BGBCC_CCXLR3_EmitArgString(ctx, name);
		}
		else
		{
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_OBJCALLN);
			BGBCC_CCXLR3_EmitArgString(ctx, name);
		}
	}else
	{
		if(dname)
		{
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_STCALLN);
			BGBCC_CCXLR3_EmitArgString(ctx, name);
			BGBCC_CCXLR3_EmitArgString(ctx, dname);
		}else if(flag&1)
		{
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_CALLNV);
			BGBCC_CCXLR3_EmitArgString(ctx, name);
		}
		else
		{
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_CALLN);
			BGBCC_CCXLR3_EmitArgString(ctx, name);
		}
	}

	tsig=NULL;
	if((ctx->lang==BGBCC_LANG_CPP) ||
		(ctx->lang==BGBCC_LANG_BS2) ||
		(ctx->lang==BGBCC_LANG_CS))
	{
		tsig=BGBCC_CCXL_StackGetSigCallArgs(ctx);
	}

	is_intrin=BGBCC_CCXL_CheckFuncNameInstrinsicP(ctx, name);

	i=BGBCC_CCXL_LookupLocalIndex(ctx, name);
	if(i<0)
		{ i=BGBCC_CCXL_LookupArgIndex(ctx, name); }
	if(i<0)
	{
		if(tsig)
			i=BGBCC_CCXL_LookupGlobalIndexSig(ctx, name, tsig);
		else
			i=BGBCC_CCXL_LookupGlobalIndex(ctx, name);
	}
//	i=BGBCC_CCXL_LookupAsRegister(ctx, name, &treg);
	if(i<0)
	{

#if 0
		BGBCC_CCXL_Error(ctx, "Undeclared Function %s\n", name);
		BGBCC_CCXL_StackPushConstInt(ctx, 0);
//		ri=BGBCC_CCXL_GetGlobal(ctx, name);
//		ri->sig=bgbcc_strdup("(z)i");
		return(CCXL_STATUS_NO);
#endif

		BGBCC_CCXL_Warn(ctx, "Undeclared Function %s\n", name);
		i=BGBCC_CCXL_HandleMissingProto(ctx, name);
	}

	if(tsig)
		j=BGBCC_CCXL_LookupAsRegisterSig(ctx, name, tsig, &treg);
	else
		j=BGBCC_CCXL_LookupAsRegister(ctx, name, &treg);
	bty=BGBCC_CCXL_GetRegReturnType(ctx, treg);
	
	if(dname)
	{
		i=BGBCC_CCXL_LookupAsRegister(ctx, dname, &dreg);
		if(i<0)
		{
			BGBCC_CCXL_Error(ctx, "Undeclared Variable %s\n", dname);
			return(CCXL_STATUS_NO);
		}

		dty=BGBCC_CCXL_GetRegType(ctx, dreg);
//		if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty, bty) || (flag&2))
		if(!BGBCC_CCXL_TypeCompatibleArchP(ctx, dty, bty) || (flag&2))
		{
			dreg2=dreg;
			BGBCC_CCXL_RegisterAllocTemporary(ctx, bty, &dreg);
			flag|=2;
		}
	}else
		if(!(flag&1) || BGBCC_CCXL_TypeValueObjectP(ctx, bty))
	{
		BGBCC_CCXL_RegisterAllocTemporary(ctx, bty, &dreg);
	}else
	{
		dreg.val=CCXL_REGID_REG_Z;
	}
	
	oreg.val=CCXL_REGID_REG_Z;
	if(flag&4)
	{
		BGBCC_CCXL_PopRegister(ctx, &oreg);
	}

	n=BGBCC_CCXL_StackGetConvCallArgs(ctx, treg);
//	n=BGBCC_CCXL_StackGetCntCallArgs(ctx);

	if(is_intrin)
	{
		BGBCC_CCXL_EmitCallIntrinOp(ctx, bty, dreg, treg, n);
		BGBCC_CCXL_StackTransforCallArgs(ctx);
		if(dname)
		{
			if(flag&2)
			{
				BGBCC_CCXL_EmitConv(ctx, dty, bty, dreg2, dreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, dreg2);
			}
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
		}
		else if(!(flag&1))
		{
			BGBCC_CCXL_PushRegister(ctx, dreg);
		}else if(BGBCC_CCXL_TypeValueObjectP(ctx, bty))
		{
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
		}
		BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
		return(CCXL_STATUS_YES);
	}else
	{
		if(flag&4)
		{
			BGBCC_CCXL_EmitObjCallOp(ctx, bty, dreg, treg, oreg, n);
			BGBCC_CCXL_RegisterCheckRelease(ctx, oreg);
		}
		else
		{
			BGBCC_CCXL_EmitCallOp(ctx, bty, dreg, treg, n);
		}
		BGBCC_CCXL_StackTransforCallArgs(ctx);
		if(dname)
		{
			BGBCC_CCXL_EmitCallCsrvOp(ctx, bty, dreg, treg);
			if(flag&2)
			{
				BGBCC_CCXL_EmitConv(ctx, dty, bty, dreg2, dreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, dreg2);
			}
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
		}
		else if(!(flag&1))
		{
			BGBCC_CCXL_EmitCallCsrvOp(ctx, bty, dreg, treg);
			BGBCC_CCXL_PushRegister(ctx, dreg);
		}else if(BGBCC_CCXL_TypeValueObjectP(ctx, bty))
		{
			BGBCC_CCXL_EmitCallCsrvOp(ctx, bty, dreg, treg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
		}
		BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
		return(CCXL_STATUS_YES);
	}

	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_StackPopCall(BGBCC_TransState *ctx, int flag)
{
	return(BGBCC_CCXL_StackPopCall2(ctx, NULL, flag));
}

ccxl_status BGBCC_CCXL_StackPopCall2(BGBCC_TransState *ctx,
	char *dname, int flag)
{
	ccxl_register treg, dreg, oreg;
	ccxl_type bty;
	int i, j, k, n;
	
	BGBCC_CCXL_DebugPrintStackLLn(ctx, "PopCall", __FILE__, __LINE__);

	if(flag&4)
	{
		if(dname)
		{
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_STOBJCALLP);
		}else if(flag&1)
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_OBJCALLPV);
		else
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_OBJCALLP);
	}else
	{
		if(dname)
		{
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_STCALLP);
		}else if(flag&1)
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_CALLPV);
		else
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_CALLP);
	}

//	j=BGBCC_CCXL_LookupAsRegister(ctx, name, &treg);
	i=BGBCC_CCXL_PopRegister(ctx, &treg);
	bty=BGBCC_CCXL_GetRegReturnType(ctx, treg);

	if(flag&4)
	{
		BGBCC_CCXL_PopRegister(ctx, &oreg);
	}

	if(dname)
	{
		i=BGBCC_CCXL_LookupAsRegister(ctx, dname, &dreg);
		if(i<0)
		{
			BGBCC_CCXL_Error(ctx, "Undeclared Variable %s\n", dname);
			return(CCXL_STATUS_NO);
		}
	}else
		if(!(flag&1) || BGBCC_CCXL_TypeValueObjectP(ctx, bty))
	{
		BGBCC_CCXL_RegisterAllocTemporary(ctx, bty, &dreg);
	}else
	{
		dreg.val=CCXL_REGID_REG_Z;
	}

	n=BGBCC_CCXL_StackGetCntCallArgs(ctx);
	if(flag&4)
		BGBCC_CCXL_EmitObjCallOp(ctx, bty, dreg, treg, oreg, n);
	else
		BGBCC_CCXL_EmitCallOp(ctx, bty, dreg, treg, n);
	BGBCC_CCXL_StackTransforCallArgs(ctx);

	if(dname)
	{
		BGBCC_CCXL_EmitCallCsrvOp(ctx, bty, dreg, treg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
	}else if(!(flag&1))
	{
		BGBCC_CCXL_EmitCallCsrvOp(ctx, bty, dreg, treg);
		BGBCC_CCXL_PushRegister(ctx, dreg);
	}else if(BGBCC_CCXL_TypeValueObjectP(ctx, bty))
	{
		BGBCC_CCXL_EmitCallCsrvOp(ctx, bty, dreg, treg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
	}

	BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
	return(CCXL_STATUS_YES);
}


ccxl_status BGBCC_CCXL_StackLoadIndexConst(BGBCC_TransState *ctx, int idx)
{
	ccxl_register treg, dreg, sreg;
	ccxl_type bty, bty2;
	int i, j, k;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "LoadIndexC", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LDIXC);
	BGBCC_CCXLR3_EmitArgInt(ctx, idx);
	
	/* src -- src[idx] */
	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	bty=BGBCC_CCXL_GetRegDerefType(ctx, sreg);
	BGBCC_CCXL_TypeAutoPromoteType(ctx, bty, &bty2);
	BGBCC_CCXL_RegisterAllocTemporaryInit(ctx, bty2, &dreg);
	BGBCC_CCXL_EmitLoadIndexImm(ctx, bty, dreg, sreg, idx);
	BGBCC_CCXL_PushRegister(ctx, dreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_StackStoreIndexConst(BGBCC_TransState *ctx, int idx)
{
	ccxl_register treg, dreg, sreg;
	ccxl_type bty, sty;
	int i, j, k;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "StoreIndexC", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_STIXC);
	BGBCC_CCXLR3_EmitArgInt(ctx, idx);

	/* value dest -- */
	i=BGBCC_CCXL_PopRegister(ctx, &dreg);
	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
//	bty=BGBCC_CCXL_GetRegDerefType(ctx, sreg);
	bty=BGBCC_CCXL_GetRegDerefType(ctx, dreg);
//	bty=BGBCC_CCXL_GetRegType(ctx, sreg);

	sty=BGBCC_CCXL_GetRegType(ctx, sreg);
//	if(!BGBCC_CCXL_TypeCompatibleP(ctx, bty, sty))
//	if(!BGBCC_CCXL_TypeCompatibleArchP(ctx, bty, sty))
	if(!BGBCC_CCXL_TypeCompatibleFlP(ctx, bty, sty, 3))
	{
		BGBCC_CCXL_RegisterAllocTemporary(ctx, bty, &treg);
		BGBCC_CCXL_EmitConv(ctx, bty, sty, treg, sreg);

		BGBCC_CCXL_EmitStoreIndexImm(ctx, bty, dreg, treg, idx);
		BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
		return(CCXL_STATUS_YES);
	}

	BGBCC_CCXL_EmitStoreIndexImm(ctx, bty, dreg, sreg, idx);
	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_StackLoadIndexAddrConst(
	BGBCC_TransState *ctx, int idx)
{
	ccxl_register treg, dreg, sreg;
	ccxl_type bty, vty;
	int i, j, k;
	
	BGBCC_CCXL_DebugPrintStackLLn(ctx, "LoadIndexAC", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LDIXAC);
	BGBCC_CCXLR3_EmitArgInt(ctx, idx);
	
	/* src -- *src */
	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	vty=BGBCC_CCXL_GetRegDerefType(ctx, sreg);
	bty=BGBCC_CCXL_GetRegType(ctx, sreg);
	BGBCC_CCXL_RegisterAllocTemporary(ctx, bty, &dreg);
	BGBCC_CCXL_EmitLeaImm(ctx, vty, dreg, sreg, idx);
	BGBCC_CCXL_PushRegister(ctx, dreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_StackLoadIndex(BGBCC_TransState *ctx)
{
	ccxl_register dreg, sreg, treg;
	ccxl_type bty, bty2;
	int i, j, k;
	
	BGBCC_CCXL_DebugPrintStackLLn(ctx, "LoadIndex", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LDIX);
	
	/* src idx -- src[idx] */
	i=BGBCC_CCXL_PopRegister(ctx, &treg);
	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	bty=BGBCC_CCXL_GetRegDerefType(ctx, sreg);
	BGBCC_CCXL_TypeAutoPromoteType(ctx, bty, &bty2);
	BGBCC_CCXL_RegisterAllocTemporaryInit(ctx, bty2, &dreg);

//	bty=BGBCC_CCXL_GetRegDerefType(ctx, sreg);
//	bty=BGBCC_CCXL_GetRegType(ctx, sreg);
	BGBCC_CCXL_EmitLoadIndex(ctx, bty, dreg, sreg, treg);
	BGBCC_CCXL_PushRegister(ctx, dreg);

	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_StackStoreIndex(BGBCC_TransState *ctx)
{
	ccxl_register dreg, sreg, treg, treg2;
	ccxl_type bty, sty;
	int i, j, k;
	
	BGBCC_CCXL_DebugPrintStackLLn(ctx, "StoreIndex", __FILE__, __LINE__);
	
	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_STIX);

	/* value dest index -- */
	i=BGBCC_CCXL_PopRegister(ctx, &treg);
	i=BGBCC_CCXL_PopRegister(ctx, &dreg);
	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
//	bty=BGBCC_CCXL_GetRegDerefType(ctx, sreg);
	bty=BGBCC_CCXL_GetRegDerefType(ctx, dreg);
//	bty=BGBCC_CCXL_GetRegType(ctx, sreg);

	sty=BGBCC_CCXL_GetRegType(ctx, sreg);
//	if(!BGBCC_CCXL_TypeCompatibleP(ctx, bty, sty))
//	if(!BGBCC_CCXL_TypeCompatibleArchP(ctx, bty, sty))
	if(!BGBCC_CCXL_TypeCompatibleFlP(ctx, bty, sty, 3))
	{
		BGBCC_CCXL_RegisterAllocTemporary(ctx, bty, &treg2);
		BGBCC_CCXL_EmitConv(ctx, bty, sty, treg2, sreg);

		BGBCC_CCXL_EmitStoreIndex(ctx, bty, dreg, treg, treg2);

		BGBCC_CCXL_RegisterCheckRelease(ctx, treg2);
		BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
		return(CCXL_STATUS_YES);
	}

	BGBCC_CCXL_EmitStoreIndex(ctx, bty, dreg, treg, sreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_StackLoadIndexAddr(BGBCC_TransState *ctx)
{
	ccxl_register dreg, sreg, treg;
	ccxl_type bty, vty;
	int i, j, k;
	
	BGBCC_CCXL_DebugPrintStackLLn(ctx, "LoadIndexA", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LDIXA);

	/* src idx -- src[idx] */
	i=BGBCC_CCXL_PopRegister(ctx, &treg);
	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	vty=BGBCC_CCXL_GetRegDerefType(ctx, sreg);
	bty=BGBCC_CCXL_GetRegType(ctx, sreg);
	BGBCC_CCXL_RegisterAllocTemporary(ctx, bty, &dreg);

	bty=BGBCC_CCXL_GetRegDerefType(ctx, sreg);
//	bty=BGBCC_CCXL_GetRegType(ctx, sreg);
	BGBCC_CCXL_EmitLea(ctx, vty, dreg, sreg, treg);
	BGBCC_CCXL_PushRegister(ctx, dreg);

	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
	return(CCXL_STATUS_YES);
}

#if 0
ccxl_status BGBCC_CCXL_StackDup(BGBCC_TransState *ctx)
{
	ccxl_register sreg, trega, tregb;
	ccxl_type bty;
	int i, j, k;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "Dup", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_DUP);
	
	/* src -- src(1) src(2) */
//	i=BGBCC_CCXL_PopRegister(ctx, &treg);
//	BGBCC_CCXL_RegisterCheckAcquire(ctx, treg);
//	BGBCC_CCXL_PushRegister(ctx, treg);
//	BGBCC_CCXL_PushRegister(ctx, treg);
//	return(CCXL_STATUS_YES);

	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	bty=BGBCC_CCXL_GetRegType(ctx, sreg);

	BGBCC_CCXL_RegisterAllocTemporaryInit(ctx, bty, &trega);
	BGBCC_CCXL_RegisterAllocTemporaryInit(ctx, bty, &tregb);
	BGBCC_CCXL_EmitMov(ctx, bty, trega, sreg);
	BGBCC_CCXL_EmitMov(ctx, bty, tregb, sreg);
	BGBCC_CCXL_PushRegister(ctx, trega);
	BGBCC_CCXL_PushRegister(ctx, tregb);
	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	return(CCXL_STATUS_YES);
}
#endif

ccxl_status BGBCC_CCXL_StackDup(BGBCC_TransState *ctx)
{
	return(BGBCC_CCXL_StackDupB(ctx));
}

ccxl_status BGBCC_CCXL_StackDupB(BGBCC_TransState *ctx)
{
	ccxl_register sreg, treg;
	ccxl_type bty;
	int i, j, k;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "DupB", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_DUP);

	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	bty=BGBCC_CCXL_GetRegType(ctx, sreg);
	BGBCC_CCXL_RegisterAllocTemporaryInit(ctx, bty, &treg);
	BGBCC_CCXL_EmitMov(ctx, bty, treg, sreg);
	BGBCC_CCXL_PushRegister(ctx, treg);
	BGBCC_CCXL_PushRegister(ctx, sreg);
	return(CCXL_STATUS_YES);
}

/** More efficient dup, only safe for clean expressions. */
ccxl_status BGBCC_CCXL_StackDupClean(BGBCC_TransState *ctx)
{
	ccxl_register treg;
	ccxl_type bty;
	int i, j, k;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "DupClean", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_DUPCLEAN);
	
	/* src -- src src */
	i=BGBCC_CCXL_PopRegister(ctx, &treg);
	BGBCC_CCXL_RegisterCheckAcquire(ctx, treg);
	BGBCC_CCXL_PushRegister(ctx, treg);
	BGBCC_CCXL_PushRegister(ctx, treg);
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_StackExch(BGBCC_TransState *ctx)
{
	ccxl_register sreg, treg;
	ccxl_type bty;
	int i, j, k;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "Exch", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_EXCH);

	BGBCC_CCXL_PopRegister(ctx, &sreg);
	BGBCC_CCXL_PopRegister(ctx, &treg);
	BGBCC_CCXL_PushRegister(ctx, sreg);
	BGBCC_CCXL_PushRegister(ctx, treg);
	return(CCXL_STATUS_YES);
}


ccxl_status BGBCC_CCXL_StackDupIdx(BGBCC_TransState *ctx, int idx)
{
	ccxl_register sreg, treg;
	ccxl_type bty;
	int i, j, k;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "DupIx", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_DUPIX);
	BGBCC_CCXLR3_EmitArgInt(ctx, idx);

//	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	i=BGBCC_CCXL_PeekRegisterIdx(ctx, &sreg, idx);
	bty=BGBCC_CCXL_GetRegType(ctx, sreg);
	BGBCC_CCXL_RegisterAllocTemporaryInit(ctx, bty, &treg);
	BGBCC_CCXL_EmitMov(ctx, bty, treg, sreg);
	BGBCC_CCXL_PushRegister(ctx, treg);
//	BGBCC_CCXL_PushRegister(ctx, sreg);
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_StackDupMarkIdx(BGBCC_TransState *ctx, int idx)
{
	ccxl_register sreg, treg;
	ccxl_type bty;
	int i, j, k;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "DupMIx", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_DUPMIX);
	BGBCC_CCXLR3_EmitArgInt(ctx, idx);

//	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	i=BGBCC_CCXL_PeekRegisterMarkIdx(ctx, &sreg, idx);
	bty=BGBCC_CCXL_GetRegType(ctx, sreg);
	BGBCC_CCXL_RegisterAllocTemporaryInit(ctx, bty, &treg);
	BGBCC_CCXL_EmitMov(ctx, bty, treg, sreg);
	BGBCC_CCXL_PushRegister(ctx, treg);
//	BGBCC_CCXL_PushRegister(ctx, sreg);
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_StackRetV(BGBCC_TransState *ctx)
{
	BGBCC_CCXL_DebugPrintStackLLn(ctx, "RetV", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_RETV);

	BGBCC_CCXL_EmitCallRetV(ctx);
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_StackRet(BGBCC_TransState *ctx)
{
	ccxl_register treg, treg2;
	ccxl_type bty, dty;
	int i, j, k;
	
	BGBCC_CCXL_DebugPrintStackLLn(ctx, "Ret", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_RET);

	/* src -- */
	i=BGBCC_CCXL_PopRegister(ctx, &treg);
	bty=BGBCC_CCXL_GetRegType(ctx, treg);
	
	dty=BGBCC_CCXL_GetTypeReturnType(ctx, ctx->cur_func->type);

	if(BGBCC_CCXL_TypeVoidP(ctx, dty))
	{
		BGBCC_CCXL_TagWarn(ctx, CCXL_TERR_RETVOID);

		BGBCC_CCXL_EmitCallRetV(ctx);
		BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
		return(CCXL_STATUS_YES);
	}

//	if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty, bty))
	if(!BGBCC_CCXL_TypeCompatibleArchP(ctx, dty, bty))
	{
		BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &treg2);
		BGBCC_CCXL_EmitConv(ctx, dty, bty, treg2, treg);
		BGBCC_CCXL_EmitCallRetOp(ctx, dty, treg2);

		BGBCC_CCXL_RegisterCheckRelease(ctx, treg2);
		BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
		return(CCXL_STATUS_YES);
	}

	BGBCC_CCXL_EmitCallRetOp(ctx, bty, treg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_StackCastBool(BGBCC_TransState *ctx)
{
	BGBCC_CCXL_DebugPrintStackLLn(ctx, "CastBool", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_CASTBOOL);

	return(BGBCC_CCXL_StackCastSig(ctx, "b"));
//	BGBCC_CCXL_StubError(ctx);
}

ccxl_status BGBCC_CCXL_StackCastSig(BGBCC_TransState *ctx, char *sig)
{
	ccxl_register sreg, dreg;
	ccxl_type sty, dty;
	int i;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "CastSig", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_CASTSIG);
	BGBCC_CCXLR3_EmitArgString(ctx, sig);

	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	sty=BGBCC_CCXL_GetRegType(ctx, sreg);
	BGBCC_CCXL_TypeFromSig(ctx, &dty, sig);

#if 1
	if(BGBCC_CCXL_TypeCompatibleArchP(ctx, dty, sty) &&
//	if(BGBCC_CCXL_TypeCompatibleP(ctx, dty, sty) &&
		(BGBCC_CCXL_GetRegAsType(ctx, sreg, dty, &dreg)>0))
	{
		BGBCC_CCXL_PushRegister(ctx, dreg);
		return(CCXL_STATUS_YES);
	}
#endif

	BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &dreg);
	BGBCC_CCXL_EmitConv(ctx, dty, sty, dreg, sreg);
	BGBCC_CCXL_PushRegister(ctx, dreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	return(CCXL_STATUS_YES);

//	BGBCC_CCXL_StubError(ctx);
}

ccxl_status BGBCC_CCXL_StackBinaryOp(BGBCC_TransState *ctx, char *op)
{
	ccxl_register sreg, treg, dreg, sreg2, treg2, dreg2;
	ccxl_type sty, tty, dty, bty, pty;
	double f, g;
	int opr, is_shl;
	int i, j, k;

	opr=-1;
	if(!strcmp(op, "+"))
		opr=CCXL_BINOP_ADD;
	if(!strcmp(op, "-"))
		opr=CCXL_BINOP_SUB;
	if(!strcmp(op, "*"))
		opr=CCXL_BINOP_MUL;
	if(!strcmp(op, "/"))
		opr=CCXL_BINOP_DIV;
	if(!strcmp(op, "%"))
		opr=CCXL_BINOP_MOD;
	if(!strcmp(op, "&"))
		opr=CCXL_BINOP_AND;
	if(!strcmp(op, "|"))
		opr=CCXL_BINOP_OR;
	if(!strcmp(op, "^"))
		opr=CCXL_BINOP_XOR;
	if(!strcmp(op, "<<"))
		opr=CCXL_BINOP_SHL;
	if(!strcmp(op, ">>"))
		opr=CCXL_BINOP_SHR;
	if(!strcmp(op, ">>>"))
		opr=CCXL_BINOP_SHRR;

	if(opr>=0)
	{
		BGBCC_CCXL_DebugPrintStackLLn(ctx, "BinaryOp", __FILE__, __LINE__);

		switch(opr)
		{
		case CCXL_BINOP_ADD:
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_ADD); break;
		case CCXL_BINOP_SUB:
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_SUB); break;
		case CCXL_BINOP_MUL:
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_MUL); break;
		case CCXL_BINOP_AND:
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_AND); break;
		case CCXL_BINOP_OR:
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_OR); break;
		case CCXL_BINOP_XOR:
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_XOR); break;
		case CCXL_BINOP_SHL:
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_SHL); break;
		case CCXL_BINOP_SHR:
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_SAR); break;
		default:
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_BINOP);
			BGBCC_CCXLR3_EmitArgInt(ctx, opr);
			break;
		}

		j=BGBCC_CCXL_PopRegister(ctx, &treg);
		i=BGBCC_CCXL_PopRegister(ctx, &sreg);

#if 1
		if(BGBCC_CCXL_IsRegImmILFDP(ctx, sreg) &&
			((opr==CCXL_BINOP_ADD) ||
			 (opr==CCXL_BINOP_MUL) ||
			 (opr==CCXL_BINOP_AND) ||
			 (opr==CCXL_BINOP_OR) ||
			 (opr==CCXL_BINOP_XOR)))
		{
			dreg2=sreg;
			sreg=treg;
			treg=dreg2;
		}
#endif

		sty=BGBCC_CCXL_GetRegType(ctx, sreg);
		tty=BGBCC_CCXL_GetRegType(ctx, treg);
		BGBCC_CCXL_GetTypeBinaryDest(ctx, opr, sty, tty, &dty);

		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty) &&
			BGBCC_CCXL_TypeArrayOrPointerP(ctx, tty) &&
			(opr==CCXL_BINOP_SUB))
		{
			pty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I);
			BGBCC_CCXL_RegisterAllocTemporary(ctx, pty, &dreg);

			BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
			BGBCC_CCXL_EmitDiffPtr(ctx, bty, dreg, sreg, treg);

			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, treg);

			BGBCC_CCXL_PushRegister(ctx, dreg);
			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty) &&
			BGBCC_CCXL_TypeSmallIntP(ctx, tty))
		{
			if(BGBCC_CCXL_IsRegImmIntP(ctx, treg) &&
				((opr==CCXL_BINOP_ADD) ||
				 (opr==CCXL_BINOP_SUB)))
			{
				k=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
				if(opr==CCXL_BINOP_SUB)
					k=-k;
				if(k==0)
				{
					j=-1;
				}

				if(BGBCC_CCXL_TypeArrayP(ctx, sty))
					{ BGBCC_CCXL_TypePointerType(ctx, sty, &pty); }
				else
					{ pty=sty; }
				
				BGBCC_CCXL_RegisterAllocTemporary(ctx, pty, &dreg);
				BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
				BGBCC_CCXL_EmitLeaImm(ctx, bty, dreg, sreg, k);
				BGBCC_CCXL_PushRegister(ctx, dreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				return(CCXL_STATUS_YES);
			}
			
			if(opr==CCXL_BINOP_ADD)
			{
				if(BGBCC_CCXL_TypeArrayP(ctx, sty))
					{ BGBCC_CCXL_TypePointerType(ctx, sty, &pty); }
				else
					{ pty=sty; }

				BGBCC_CCXL_RegisterAllocTemporary(ctx, pty, &dreg);
				BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
				BGBCC_CCXL_EmitLea(ctx, bty, dreg, sreg, treg);
				BGBCC_CCXL_PushRegister(ctx, dreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				return(CCXL_STATUS_YES);
			}

			if(opr==CCXL_BINOP_SUB)
			{
				if(BGBCC_CCXL_TypeArrayP(ctx, sty))
					{ BGBCC_CCXL_TypePointerType(ctx, sty, &pty); }
				else
					{ pty=sty; }

				BGBCC_CCXL_RegisterAllocTemporary(ctx, tty, &treg2);
				BGBCC_CCXL_EmitUnaryOp(ctx, tty,
					CCXL_UNOP_NEG, treg2, treg);

				BGBCC_CCXL_RegisterAllocTemporary(ctx, pty, &dreg);
				BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
				BGBCC_CCXL_EmitLea(ctx, bty, dreg, sreg, treg2);
				BGBCC_CCXL_PushRegister(ctx, dreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg2);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				return(CCXL_STATUS_YES);
			}

			BGBCC_DBGBREAK
		}

		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty) &&
			BGBCC_CCXL_TypeSmallLongP(ctx, tty))
		{
			if(BGBCC_CCXL_IsRegImmIntP(ctx, treg) &&
				((opr==CCXL_BINOP_ADD) ||
				 (opr==CCXL_BINOP_SUB)))
			{
				k=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
				if(opr==CCXL_BINOP_SUB)
					k=-k;
				if(k==0)
				{
					j=-1;
				}

				if(BGBCC_CCXL_TypeArrayP(ctx, sty))
					{ BGBCC_CCXL_TypePointerType(ctx, sty, &pty); }
				else
					{ pty=sty; }
				
				BGBCC_CCXL_RegisterAllocTemporary(ctx, pty, &dreg);
				BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
				BGBCC_CCXL_EmitLeaImm(ctx, bty, dreg, sreg, k);
				BGBCC_CCXL_PushRegister(ctx, dreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				return(CCXL_STATUS_YES);
			}
			
			if(opr==CCXL_BINOP_ADD)
			{
				if(BGBCC_CCXL_TypeArrayP(ctx, sty))
					{ BGBCC_CCXL_TypePointerType(ctx, sty, &pty); }
				else
					{ pty=sty; }

				BGBCC_CCXL_RegisterAllocTemporary(ctx, pty, &dreg);
				BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
				BGBCC_CCXL_EmitLea(ctx, bty, dreg, sreg, treg);
				BGBCC_CCXL_PushRegister(ctx, dreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				return(CCXL_STATUS_YES);
			}

			if(opr==CCXL_BINOP_SUB)
			{
				if(BGBCC_CCXL_TypeArrayP(ctx, sty))
					{ BGBCC_CCXL_TypePointerType(ctx, sty, &pty); }
				else
					{ pty=sty; }

				BGBCC_CCXL_RegisterAllocTemporary(ctx, tty, &treg2);
				BGBCC_CCXL_EmitUnaryOp(ctx, tty,
					CCXL_UNOP_NEG, treg2, treg);

				BGBCC_CCXL_RegisterAllocTemporary(ctx, pty, &dreg);
				BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
				BGBCC_CCXL_EmitLea(ctx, bty, dreg, sreg, treg2);
				BGBCC_CCXL_PushRegister(ctx, dreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg2);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				return(CCXL_STATUS_YES);
			}
		}

		if(opr==CCXL_BINOP_DIV)
		{
			if(BGBCC_CCXL_IsRegImmFloatP(ctx, treg))
			{
				f=BGBCC_CCXL_GetRegImmFloatValue(ctx, treg);
				if((f!=0) && (f==f))
				{
					BGBCC_CCXL_GetRegForFloatValue(ctx, &treg, 1.0/f);
					opr=CCXL_BINOP_MUL;
				}
			}

			if(BGBCC_CCXL_IsRegImmDoubleP(ctx, treg))
			{
				f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, treg);
				if((f!=0) && (f==f))
				{
					BGBCC_CCXL_GetRegForDoubleValue(ctx, &treg, 1.0/f);
					opr=CCXL_BINOP_MUL;
				}
			}

			if(BGBCC_CCXL_IsRegImmIntP(ctx, treg) &&
				BGBCC_CCXL_TypeSmallLongP(ctx, dty))
			{
				j=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
				if((j>1) && !(j&(j-1)))
				{
					k=0;
					while(j>1)	{ j>>=1; k++; }
					BGBCC_CCXL_GetRegForIntValue(ctx, &treg, k);
					opr=CCXL_BINOP_SHR;
				}
			}
		}

#if 1
		if(opr==CCXL_BINOP_DIV)
		{
			if(	BGBCC_CCXL_TypeFloatP(ctx, dty) &&
				BGBCC_CCXL_IsRegImmILFDP(ctx, treg))
			{
				f=BGBCC_CCXL_GetRegImmFloatValue(ctx, treg);
				if((f!=0) && (f==f))
				{
					BGBCC_CCXL_GetRegForFloatValue(ctx, &treg, 1.0/f);
					opr=CCXL_BINOP_MUL;
				}
			}

			if(	BGBCC_CCXL_TypeDoubleP(ctx, dty) &&
				BGBCC_CCXL_IsRegImmILFDP(ctx, treg))
			{
				f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, treg);
				if((f!=0) && (f==f))
				{
					BGBCC_CCXL_GetRegForDoubleValue(ctx, &treg, 1.0/f);
					opr=CCXL_BINOP_MUL;
				}
			}
		}
#endif

#if 1
//		if(	!BGBCC_CCXL_TypeEqualP(ctx, dty, sty) ||
//			!BGBCC_CCXL_TypeEqualP(ctx, dty, tty))
		if(	!BGBCC_CCXL_TypeCompatibleP(ctx, dty, sty) ||
			!BGBCC_CCXL_TypeCompatibleP(ctx, dty, tty))
		{
//			if(BGBCC_CCXL_IsRegImmP(ctx, treg) &&
//				(BGBCC_CCXL_TypeDoubleP(ctx, dty) ||
//				 BGBCC_CCXL_TypeSmallLongP(ctx, dty)))

			if(BGBCC_CCXL_IsRegImmILFDP(ctx, treg) &&
//				BGBCC_CCXL_TypeSmallDoubleP(ctx, dty))
				BGBCC_CCXL_TypeSmallFloat128P(ctx, dty) &&
				BGBCC_CCXL_TypeSmallTypeP(ctx, sty, tty))
			{
				dty=sty; tty=sty;

				if((opr!=CCXL_BINOP_SHL) && (opr!=CCXL_BINOP_SHR) &&
					(opr!=CCXL_BINOP_SHRR))
				{
					BGBCC_CCXL_ConvImm(ctx, dty, tty, treg, &treg);
				}
			}
		}
#endif

		is_shl=0;
		if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
		{
			if(	(opr==CCXL_BINOP_SHL) ||
				(opr==CCXL_BINOP_SHR) ||
				(opr==CCXL_BINOP_SHRR))
					is_shl=1;
		}


//		if(BGBCC_CCXL_TypeEqualP(ctx, dty, sty) &&
//			BGBCC_CCXL_TypeEqualP(ctx, dty, tty))
		if(BGBCC_CCXL_TypeCompatibleP(ctx, dty, sty) &&
			(BGBCC_CCXL_TypeCompatibleP(ctx, dty, tty) || is_shl))
		{
			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &dreg);
			BGBCC_CCXL_EmitBinaryOp(ctx, dty, opr, dreg, sreg, treg);
			BGBCC_CCXL_PushRegister(ctx, dreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
			return(CCXL_STATUS_YES);
		}

		BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &dreg);
//		if((dty.val!=sty.val) && (dty.val!=tty.val))
//		if(1)
		if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty, sty) &&
			!(BGBCC_CCXL_TypeCompatibleP(ctx, dty, tty) || is_shl))
		{
			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &sreg2);
			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &treg2);
			BGBCC_CCXL_EmitConv(ctx, dty, sty, sreg2, sreg);
			if((opr!=CCXL_BINOP_SHL) && (opr!=CCXL_BINOP_SHR))
				{ BGBCC_CCXL_EmitConv(ctx, dty, tty, treg2, treg); }
			else
				{ BGBCC_CCXL_EmitMov(ctx, tty, treg2, treg); }
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
		}else
		{
//			if(dty.val!=sty.val)
//			if(1)
			if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty, sty))
			{
				BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &sreg2);
				BGBCC_CCXL_EmitConv(ctx, dty, sty, sreg2, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			}
			else
				{ sreg2=sreg; }

//			if(dty.val!=tty.val)
//			if(1)
			if(!(BGBCC_CCXL_TypeCompatibleP(ctx, dty, tty) || is_shl))
			{
//				BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &treg2);
				if((opr!=CCXL_BINOP_SHL) && (opr!=CCXL_BINOP_SHR))
				{
					BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &treg2);
					BGBCC_CCXL_EmitConv(ctx, dty, tty, treg2, treg);
				}
				else
				{
					BGBCC_CCXL_RegisterAllocTemporary(ctx, tty, &treg2);
					BGBCC_CCXL_EmitMov(ctx, tty, treg2, treg);
				}
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
			}
			else
				{ treg2=treg; }
		}

		BGBCC_CCXL_EmitBinaryOp(ctx, dty, opr, dreg, sreg2, treg2);
		BGBCC_CCXL_PushRegister(ctx, dreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg2);
		BGBCC_CCXL_RegisterCheckRelease(ctx, treg2);
		return(CCXL_STATUS_YES);
	}

	if(!strcmp(op, "=="))opr=CCXL_CMP_EQ;
	if(!strcmp(op, "!="))opr=CCXL_CMP_NE;
	if(!strcmp(op, "<"))opr=CCXL_CMP_LT;
	if(!strcmp(op, ">"))opr=CCXL_CMP_GT;
	if(!strcmp(op, "<="))opr=CCXL_CMP_LE;
	if(!strcmp(op, ">="))opr=CCXL_CMP_GE;

	if(opr>=0)
	{
		BGBCC_CCXL_DebugPrintStackLLn(ctx, "CompareOp", __FILE__, __LINE__);

		BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_CMPOP);
		BGBCC_CCXLR3_EmitArgInt(ctx, opr);

		j=BGBCC_CCXL_PopRegister(ctx, &treg);
		i=BGBCC_CCXL_PopRegister(ctx, &sreg);
		sty=BGBCC_CCXL_GetRegType(ctx, sreg);
		tty=BGBCC_CCXL_GetRegType(ctx, treg);
		BGBCC_CCXL_GetTypeCompareBinaryDest(ctx, opr, sty, tty, &dty);

//		if(BGBCC_CCXL_TypeEqualP(ctx, dty, sty) &&
//			BGBCC_CCXL_TypeEqualP(ctx, dty, tty))
		if(BGBCC_CCXL_TypeCompatibleP(ctx, dty, sty) &&
			BGBCC_CCXL_TypeCompatibleP(ctx, dty, tty))
		{
			BGBCC_CCXL_RegisterAllocTemporaryInt(ctx, &dreg);
			BGBCC_CCXL_EmitCompareOp(ctx, dty, opr, dreg, sreg, treg);
			BGBCC_CCXL_PushRegister(ctx, dreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
			return(CCXL_STATUS_YES);
		}

		BGBCC_CCXL_RegisterAllocTemporaryInt(ctx, &dreg);

#if 0
		BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &sreg2);
		BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &treg2);
		BGBCC_CCXL_EmitConv(ctx, dty, sty, sreg2, sreg);
		BGBCC_CCXL_EmitConv(ctx, dty, tty, treg2, treg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
#endif

#if 1
//		if((dty.val!=sty.val) && (dty.val!=tty.val))
//		if(1)
		if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty, sty) &&
			!BGBCC_CCXL_TypeCompatibleP(ctx, dty, tty))
		{
			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &sreg2);
			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &treg2);
			BGBCC_CCXL_EmitConv(ctx, dty, sty, sreg2, sreg);
			if((opr!=CCXL_BINOP_SHL) && (opr!=CCXL_BINOP_SHR))
				{ BGBCC_CCXL_EmitConv(ctx, dty, tty, treg2, treg); }
			else
				{ BGBCC_CCXL_EmitMov(ctx, tty, treg2, treg); }
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
		}else
		{
//			if(dty.val!=sty.val)
//			if(1)
			if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty, sty))
			{
				BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &sreg2);
				BGBCC_CCXL_EmitConv(ctx, dty, sty, sreg2, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			}
			else
				{ sreg2=sreg; }

//			if(dty.val!=tty.val)
//			if(1)
			if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty, tty))
			{
				BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &treg2);
				if((opr!=CCXL_BINOP_SHL) && (opr!=CCXL_BINOP_SHR))
					{ BGBCC_CCXL_EmitConv(ctx, dty, tty, treg2, treg); }
				else
					{ BGBCC_CCXL_EmitMov(ctx, tty, treg2, treg); }
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
			}
			else
				{ treg2=treg; }
		}
#endif

		BGBCC_CCXL_EmitCompareOp(ctx, dty, opr, dreg, sreg2, treg2);
		BGBCC_CCXL_PushRegister(ctx, dreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg2);
		BGBCC_CCXL_RegisterCheckRelease(ctx, treg2);
		return(CCXL_STATUS_YES);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_BADOPARGS));
	return(CCXL_STATUS_NO);
}

ccxl_status BGBCC_CCXL_StackUnaryOp(BGBCC_TransState *ctx, char *op)
{
	ccxl_register sreg, dreg;
	ccxl_type sty, dty, bty;
	int opr;
	int i, j, k;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "UnaryOp", __FILE__, __LINE__);

	opr=-1;
	if(!strcmp(op, "+"))opr=CCXL_UNOP_MOV;
	if(!strcmp(op, "-"))opr=CCXL_UNOP_NEG;
	if(!strcmp(op, "~"))opr=CCXL_UNOP_NOT;
	if(!strcmp(op, "!"))opr=CCXL_UNOP_LNOT;
	if(!strcmp(op, "++"))opr=CCXL_UNOP_INC;
	if(!strcmp(op, "--"))opr=CCXL_UNOP_DEC;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_UNOP);
	BGBCC_CCXLR3_EmitArgInt(ctx, opr);

	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	sty=BGBCC_CCXL_GetRegType(ctx, sreg);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty))
	{
		if((opr==CCXL_UNOP_INC) ||
			(opr==CCXL_UNOP_DEC))
		{
			BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
			k=(opr==CCXL_UNOP_INC)?(1):(-1);
			BGBCC_CCXL_RegisterAllocTemporary(ctx, sty, &dreg);
			
			if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
				__debugbreak();
			
			BGBCC_CCXL_EmitLeaImm(ctx, bty, dreg, sreg, k);
			BGBCC_CCXL_PushRegister(ctx, dreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			return(CCXL_STATUS_YES);
		}
	}

//	BGBCC_CCXL_TypeFromSig(ctx, &dty, sig);
	BGBCC_CCXL_RegisterAllocTemporary(ctx, sty, &dreg);
//	BGBCC_CCXL_EmitConv(ctx, dty, sty, dreg, sreg);

	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		__debugbreak();

	BGBCC_CCXL_EmitUnaryOp(ctx, sty, opr, dreg, sreg);
	BGBCC_CCXL_PushRegister(ctx, dreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	return(CCXL_STATUS_YES);
}

int BGBCC_CCXL_BinaryOpIdForName(BGBCC_TransState *ctx, char *op)
{
	int opr;

	if(!op)
		return(-1);

	opr=-1;
	if(!strcmp(op, "+"))opr=CCXL_BINOP_ADD;
	if(!strcmp(op, "-"))opr=CCXL_BINOP_SUB;
	if(!strcmp(op, "*"))opr=CCXL_BINOP_MUL;
	if(!strcmp(op, "/"))opr=CCXL_BINOP_DIV;
	if(!strcmp(op, "%"))opr=CCXL_BINOP_MOD;
	if(!strcmp(op, "&"))opr=CCXL_BINOP_AND;
	if(!strcmp(op, "|"))opr=CCXL_BINOP_OR;
	if(!strcmp(op, "^"))opr=CCXL_BINOP_XOR;
	if(!strcmp(op, "<<"))opr=CCXL_BINOP_SHL;
	if(!strcmp(op, ">>"))opr=CCXL_BINOP_SHR;
	if(!strcmp(op, ">>>"))opr=CCXL_BINOP_SHRR;

#if 0
	if(!strcmp(op, "=="))opr=CCXL_BINOP_EQ;
	if(!strcmp(op, "!="))opr=CCXL_BINOP_NE;
	if(!strcmp(op, "<"))opr=CCXL_BINOP_LT;
	if(!strcmp(op, ">"))opr=CCXL_BINOP_GT;
	if(!strcmp(op, "<="))opr=CCXL_BINOP_LE;
	if(!strcmp(op, ">="))opr=CCXL_BINOP_GE;
#endif

	return(opr);
}

#if 0
// int BGBCC_CCXL_BinaryCmpOpIdForName(BGBCC_TransState *ctx, char *op)
{
	int opr;

	if(!op)
		return(-1);

	opr=-1;
	if(!strcmp(op, "=="))opr=CCXL_BINOP_EQ;
	if(!strcmp(op, "!="))opr=CCXL_BINOP_NE;
	if(!strcmp(op, "<"))opr=CCXL_BINOP_LT;
	if(!strcmp(op, ">"))opr=CCXL_BINOP_GT;
	if(!strcmp(op, "<="))opr=CCXL_BINOP_LE;
	if(!strcmp(op, ">="))opr=CCXL_BINOP_GE;

	return(opr);
}
#endif

int BGBCC_CCXL_CompareOpIdForName(BGBCC_TransState *ctx, char *op)
{
	int opr;

	if(!op)
		return(-1);

	opr=-1;
	if(!strcmp(op, "=="))opr=CCXL_CMP_EQ;
	if(!strcmp(op, "!="))opr=CCXL_CMP_NE;
	if(!strcmp(op, "<"))opr=CCXL_CMP_LT;
	if(!strcmp(op, ">"))opr=CCXL_CMP_GT;
	if(!strcmp(op, "<="))opr=CCXL_CMP_LE;
	if(!strcmp(op, ">="))opr=CCXL_CMP_GE;

	return(opr);
}

ccxl_status BGBCC_CCXL_StackBinaryOpStore(BGBCC_TransState *ctx,
	char *op, char *name)
{
	ccxl_register sreg, treg, dreg, sreg2, treg2, dreg2;
	ccxl_type sty, tty, dty, dty2, bty;
	double f, g;
	int opr, is_shl;
	int i, j, k;

//	BGBCC_CCXL_DebugPrintStackLLn(ctx, "BinaryOpStore", __FILE__, __LINE__);

//	if(name && !strcmp(name, "dc_iscale"))
//		__debugbreak();

//	i=BGBCC_CCXL_PopRegister(ctx, &treg);
	j=BGBCC_CCXL_LookupAsRegister(ctx, name, &dreg);
	if(j<=0)
	{
		if(!j)return(CCXL_STATUS_ERR_LOOKUPFAIL);
//		if(i<0)return(i);
		if(j<0)return(j);
		return(CCXL_STATUS_ERR_GENERIC);
	}
	
	opr=-1;
	if(!strcmp(op, "+"))opr=CCXL_BINOP_ADD;
	if(!strcmp(op, "-"))opr=CCXL_BINOP_SUB;
	if(!strcmp(op, "*"))opr=CCXL_BINOP_MUL;
	if(!strcmp(op, "/"))opr=CCXL_BINOP_DIV;
	if(!strcmp(op, "%"))opr=CCXL_BINOP_MOD;
	if(!strcmp(op, "&"))opr=CCXL_BINOP_AND;
	if(!strcmp(op, "|"))opr=CCXL_BINOP_OR;
	if(!strcmp(op, "^"))opr=CCXL_BINOP_XOR;
	if(!strcmp(op, "<<"))opr=CCXL_BINOP_SHL;
	if(!strcmp(op, ">>"))opr=CCXL_BINOP_SHR;
	if(!strcmp(op, ">>>"))opr=CCXL_BINOP_SHRR;

	if(opr>=0)
	{
		BGBCC_CCXL_DebugPrintStackLLn(ctx,
			"BinaryOpStore", __FILE__, __LINE__);

		BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_STBINOP);
		BGBCC_CCXLR3_EmitArgInt(ctx, opr);
		BGBCC_CCXLR3_EmitArgString(ctx, name);

		j=BGBCC_CCXL_PopRegister(ctx, &treg);
		i=BGBCC_CCXL_PopRegister(ctx, &sreg);
		sty=BGBCC_CCXL_GetRegType(ctx, sreg);
		tty=BGBCC_CCXL_GetRegType(ctx, treg);

		if(	BGBCC_CCXL_TypeVecP(ctx, sty) &&
			BGBCC_CCXL_TypeVecP(ctx, tty))
		{
			k=-1;
		}

#if 1
		if(BGBCC_CCXL_IsRegImmILFDP(ctx, sreg) &&
//			BGBCC_CCXL_TypeSmallFloat128P(ctx, sty) &&
//			BGBCC_CCXL_TypeSmallFloat128P(ctx, tty) &&
			((opr==CCXL_BINOP_ADD) ||
			 (opr==CCXL_BINOP_MUL) ||
			 (opr==CCXL_BINOP_AND) ||
			 (opr==CCXL_BINOP_OR) ||
			 (opr==CCXL_BINOP_XOR)))
		{
			dreg2=sreg;
			sreg=treg;
			treg=dreg2;
		}
#endif

		sty=BGBCC_CCXL_GetRegType(ctx, sreg);
		tty=BGBCC_CCXL_GetRegType(ctx, treg);
		dty=BGBCC_CCXL_GetRegType(ctx, dreg);
		BGBCC_CCXL_GetTypeBinaryDest(ctx, opr, sty, tty, &dty2);

		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty) &&
			BGBCC_CCXL_TypeArrayOrPointerP(ctx, tty) &&
			(opr==CCXL_BINOP_SUB))
		{
//			pty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I);
//			BGBCC_CCXL_RegisterAllocTemporary(ctx, pty, &dreg);

			BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
			BGBCC_CCXL_EmitDiffPtr(ctx, bty, dreg, sreg, treg);

			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);

//			BGBCC_CCXL_PushRegister(ctx, dreg);
			return(CCXL_STATUS_YES);
		}

//		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty) &&
//			BGBCC_CCXL_TypeIntP(ctx, tty))
		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty) &&
			BGBCC_CCXL_TypeSmallIntP(ctx, tty))
		{
			if(BGBCC_CCXL_IsRegImmIntP(ctx, treg) &&
				((opr==CCXL_BINOP_ADD) ||
				 (opr==CCXL_BINOP_SUB)))
			{
				k=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
				if(opr==CCXL_BINOP_SUB)
					k=-k;
				
				BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
				BGBCC_CCXL_EmitLeaImm(ctx, bty, dreg, sreg, k);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
				return(CCXL_STATUS_YES);
			}
			
			if(opr==CCXL_BINOP_ADD)
			{
				BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
				BGBCC_CCXL_EmitLea(ctx, bty, dreg, sreg, treg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
				return(CCXL_STATUS_YES);
			}

			if(opr==CCXL_BINOP_SUB)
			{
				BGBCC_CCXL_RegisterAllocTemporary(ctx, tty, &treg2);
				BGBCC_CCXL_EmitUnaryOp(ctx, tty,
					CCXL_UNOP_NEG, treg2, treg);
				BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
				BGBCC_CCXL_EmitLea(ctx, bty, dreg, sreg, treg2);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg2);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
				return(CCXL_STATUS_YES);
			}
			
			BGBCC_DBGBREAK
		}

		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty) &&
			BGBCC_CCXL_TypeSmallLongP(ctx, tty))
		{
			if(opr==CCXL_BINOP_ADD)
			{
				BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
				BGBCC_CCXL_EmitLea(ctx, bty, dreg, sreg, treg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
				return(CCXL_STATUS_YES);
			}

			if(opr==CCXL_BINOP_SUB)
			{
				BGBCC_CCXL_RegisterAllocTemporary(ctx, tty, &treg2);
				BGBCC_CCXL_EmitUnaryOp(ctx, tty,
					CCXL_UNOP_NEG, treg2, treg);
				BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
				BGBCC_CCXL_EmitLea(ctx, bty, dreg, sreg, treg2);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg2);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
				return(CCXL_STATUS_YES);
			}
		}

		if(opr==CCXL_BINOP_DIV)
		{
			if(BGBCC_CCXL_IsRegImmFloatP(ctx, treg))
			{
				f=BGBCC_CCXL_GetRegImmFloatValue(ctx, treg);
				BGBCC_CCXL_GetRegForFloatValue(ctx, &treg, 1.0/f);
				opr=CCXL_BINOP_MUL;
			}

			if(BGBCC_CCXL_IsRegImmDoubleP(ctx, treg))
			{
				f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, treg);
				BGBCC_CCXL_GetRegForDoubleValue(ctx, &treg, 1.0/f);
				opr=CCXL_BINOP_MUL;
			}

			if(BGBCC_CCXL_IsRegImmIntP(ctx, treg) &&
				BGBCC_CCXL_TypeSmallLongP(ctx, dty))
			{
				j=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
				if((j>1) && !(j&(j-1)))
				{
					k=0;
					while(j>1)	{ j>>=1; k++; }
					BGBCC_CCXL_GetRegForIntValue(ctx, &treg, k);
					opr=CCXL_BINOP_SHR;
				}
			}
		}

#if 1
		if(opr==CCXL_BINOP_DIV)
		{
			if(	BGBCC_CCXL_TypeFloatP(ctx, dty2) &&
				BGBCC_CCXL_IsRegImmILFDP(ctx, treg))
			{
				f=BGBCC_CCXL_GetRegImmFloatValue(ctx, treg);
				if((f!=0) && (f==f))
				{
					BGBCC_CCXL_GetRegForFloatValue(ctx, &treg, 1.0/f);
					opr=CCXL_BINOP_MUL;
				}
			}

			if(	BGBCC_CCXL_TypeDoubleP(ctx, dty2) &&
				BGBCC_CCXL_IsRegImmILFDP(ctx, treg))
			{
				f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, treg);
				if((f!=0) && (f==f))
				{
					BGBCC_CCXL_GetRegForDoubleValue(ctx, &treg, 1.0/f);
					opr=CCXL_BINOP_MUL;
				}
			}
		}
#endif


#if 1
//		if(	!BGBCC_CCXL_TypeEqualP(ctx, dty2, sty) ||
//			!BGBCC_CCXL_TypeEqualP(ctx, dty2, tty))
		if(	!BGBCC_CCXL_TypeCompatibleP(ctx, dty2, sty) ||
			!BGBCC_CCXL_TypeCompatibleP(ctx, dty2, tty))
		{
			if(BGBCC_CCXL_IsRegImmILFDP(ctx, treg) &&
				BGBCC_CCXL_TypeSmallFloat128P(ctx, dty2) &&
				BGBCC_CCXL_TypeSmallTypeP(ctx, sty, tty))
			{
				dty2=sty; tty=sty;

				if(	(opr!=CCXL_BINOP_SHL) &&
					(opr!=CCXL_BINOP_SHR) &&
					(opr!=CCXL_BINOP_SHRR))
				{
					BGBCC_CCXL_ConvImm(ctx, dty2, tty, treg, &treg);
				}
			}
		}
#endif

		is_shl=0;
		if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
		{
			if(	(opr==CCXL_BINOP_SHL) ||
				(opr==CCXL_BINOP_SHR) ||
				(opr==CCXL_BINOP_SHRR))
			{
				is_shl=1;
			}
		}

		if(opr==CCXL_BINOP_SHR)
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, sty))
				opr=CCXL_BINOP_SHRR;
		}

		if(!BGBCC_CCXL_TypeSmallTypeP(ctx, dty, dty2) ||
			( BGBCC_CCXL_TypeRealP(ctx, dty) &&
			 !BGBCC_CCXL_TypeRealP(ctx, dty2)) ||
			(!BGBCC_CCXL_TypeRealP(ctx, dty) &&
			  BGBCC_CCXL_TypeRealP(ctx, dty2)))
		{
			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty2, &dreg2);
			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty2, &sreg2);
//			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty2, &treg2);
			BGBCC_CCXL_EmitConv(ctx, dty2, sty, sreg2, sreg);
//			BGBCC_CCXL_EmitConv(ctx, dty2, tty, treg2, treg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
//			BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
			
			if(!is_shl)
			{
				BGBCC_CCXL_RegisterAllocTemporary(ctx, dty2, &treg2);
				BGBCC_CCXL_EmitConv(ctx, dty2, tty, treg2, treg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
			}else
			{
				treg2=treg;
			}

			BGBCC_CCXL_EmitBinaryOp(ctx, dty2, opr, dreg2, sreg2, treg2);
			BGBCC_CCXL_EmitConv(ctx, dty, dty2, dreg, dreg2);

			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg2);
			BGBCC_CCXL_RegisterCheckRelease(ctx, treg2);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg2);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
			return(CCXL_STATUS_YES);
		}

//		if(BGBCC_CCXL_TypeEqualP(ctx, dty, sty) &&
//			BGBCC_CCXL_TypeEqualP(ctx, dty, tty))
		if(BGBCC_CCXL_TypeCompatibleP(ctx, dty, sty) &&
			BGBCC_CCXL_TypeCompatibleP(ctx, dty, tty) &&
			BGBCC_CCXL_TypeCompatibleP(ctx, dty, dty2) &&
			!BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
		{
//			BGBCC_CCXL_EmitBinaryOp(ctx, dty, opr, dreg, sreg, treg);
			BGBCC_CCXL_EmitBinaryOp(ctx, dty2, opr, dreg, sreg, treg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
			return(CCXL_STATUS_YES);
		}

//		BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &dreg);
		BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &sreg2);
//		BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &treg2);
		BGBCC_CCXL_EmitConv(ctx, dty, sty, sreg2, sreg);
//		BGBCC_CCXL_EmitConv(ctx, dty, tty, treg2, treg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
//		BGBCC_CCXL_RegisterCheckRelease(ctx, treg);

		if(!is_shl)
		{
			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &treg2);
			BGBCC_CCXL_EmitConv(ctx, dty, tty, treg2, treg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
		}else
		{
			treg2=treg;
		}

		BGBCC_CCXL_EmitBinaryOp(ctx, dty, opr, dreg, sreg2, treg2);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg2);
		BGBCC_CCXL_RegisterCheckRelease(ctx, treg2);
		BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
		return(CCXL_STATUS_YES);
	}

	if(!strcmp(op, "=="))opr=CCXL_CMP_EQ;
	if(!strcmp(op, "!="))opr=CCXL_CMP_NE;
	if(!strcmp(op, "<"))opr=CCXL_CMP_LT;
	if(!strcmp(op, ">"))opr=CCXL_CMP_GT;
	if(!strcmp(op, "<="))opr=CCXL_CMP_LE;
	if(!strcmp(op, ">="))opr=CCXL_CMP_GE;

	if(opr>=0)
	{
//		dty=BGBCC_CCXL_GetRegType(ctx, dreg);
		if(!BGBCC_CCXL_IsRegSmallIntP(ctx, dreg))
		{
			BGBCC_CCXL_StackBinaryOp(ctx, op);
			BGBCC_CCXL_PopStore(ctx, name);
			return(CCXL_STATUS_YES);
		}

		BGBCC_CCXL_DebugPrintStackLLn(ctx,
			"CompareOpStore", __FILE__, __LINE__);

		BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_STCMPOP);
		BGBCC_CCXLR3_EmitArgInt(ctx, opr);
		BGBCC_CCXLR3_EmitArgString(ctx, name);

		j=BGBCC_CCXL_PopRegister(ctx, &treg);
		i=BGBCC_CCXL_PopRegister(ctx, &sreg);
		sty=BGBCC_CCXL_GetRegType(ctx, sreg);
		tty=BGBCC_CCXL_GetRegType(ctx, treg);
//		dty=BGBCC_CCXL_GetRegType(ctx, dreg);
		BGBCC_CCXL_GetTypeBinaryDest(ctx, opr, sty, tty, &dty);

//		if(BGBCC_CCXL_TypeEqualP(ctx, dty, sty) &&
//			BGBCC_CCXL_TypeEqualP(ctx, dty, tty))
		if(BGBCC_CCXL_TypeCompatibleP(ctx, dty, sty) &&
			BGBCC_CCXL_TypeCompatibleP(ctx, dty, tty) &&
			!BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
		{
//			BGBCC_CCXL_RegisterAllocTemporaryInt(ctx, &dreg);
			BGBCC_CCXL_EmitCompareOp(ctx, dty, opr, dreg, sreg, treg);
//			BGBCC_CCXL_PushRegister(ctx, dreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
			return(CCXL_STATUS_YES);
		}

//		BGBCC_CCXL_RegisterAllocTemporaryInt(ctx, &dreg);
		BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &sreg2);
		BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &treg2);
		BGBCC_CCXL_EmitConv(ctx, dty, sty, sreg2, sreg);
		BGBCC_CCXL_EmitConv(ctx, dty, tty, treg2, treg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, treg);

		BGBCC_CCXL_EmitCompareOp(ctx, dty, opr, dreg, sreg2, treg2);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg2);
		BGBCC_CCXL_RegisterCheckRelease(ctx, treg2);
		BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
		return(CCXL_STATUS_YES);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_BADOPARGS));
	return(CCXL_STATUS_NO);
}

ccxl_status BGBCC_CCXL_StackUnaryOpName(BGBCC_TransState *ctx,
	char *op, char *name)
{
	ccxl_register sreg;
	ccxl_type sty, dty, bty;
	int opr;
	int i, j, k;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "UnaryOpName", __FILE__, __LINE__);

	j=BGBCC_CCXL_LookupAsRegister(ctx, name, &sreg);
	if(j<=0)
	{
		if(!j)return(CCXL_STATUS_ERR_LOOKUPFAIL);
//		if(i<0)return(i);
		if(j<0)return(j);
		return(CCXL_STATUS_ERR_GENERIC);
	}

	opr=-1;
	if(!strcmp(op, "+"))opr=CCXL_UNOP_MOV;
	if(!strcmp(op, "-"))opr=CCXL_UNOP_NEG;
	if(!strcmp(op, "~"))opr=CCXL_UNOP_NOT;
	if(!strcmp(op, "!"))opr=CCXL_UNOP_LNOT;
	if(!strcmp(op, "++"))opr=CCXL_UNOP_INC;
	if(!strcmp(op, "--"))opr=CCXL_UNOP_DEC;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LDUNOP);
	BGBCC_CCXLR3_EmitArgInt(ctx, opr);
	BGBCC_CCXLR3_EmitArgString(ctx, name);

	sty=BGBCC_CCXL_GetRegType(ctx, sreg);

	if(BGBCC_CCXL_TypePointerP(ctx, sty))
	{
		if((opr==CCXL_UNOP_INC) ||
			(opr==CCXL_UNOP_DEC))
		{
			BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
			k=(opr==CCXL_UNOP_INC)?(1):(-1);
//			BGBCC_CCXL_RegisterAllocTemporary(ctx, sty, &dreg);
//			BGBCC_CCXL_EmitLeaImm(ctx, bty, dreg, sreg, k);
//			BGBCC_CCXL_PushRegister(ctx, dreg);
			BGBCC_CCXL_EmitLeaImm(ctx, bty, sreg, sreg, k);
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			return(CCXL_STATUS_YES);
		}
	}

	BGBCC_CCXL_EmitUnaryOp(ctx, sty, opr, sreg, sreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	return(CCXL_STATUS_YES);

//	BGBCC_CCXL_TypeFromSig(ctx, &dty, sig);
//	BGBCC_CCXL_RegisterAllocTemporary(ctx, sty, &dreg);
//	BGBCC_CCXL_EmitConv(ctx, dty, sty, dreg, sreg);
//	BGBCC_CCXL_EmitUnaryOp(ctx, sty, opr, dreg, sreg);
//	BGBCC_CCXL_PushRegister(ctx, dreg);
//	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
//	return(CCXL_STATUS_YES);
}


ccxl_status BGBCC_CCXL_StackUnaryOpNameB(BGBCC_TransState *ctx,
	char *op, char *name, int mod)
{
	ccxl_register dreg, sreg;
	ccxl_type sty, dty, bty;
	int opr;
	int i, j, k;

	if(mod==0)
	{
		return(BGBCC_CCXL_StackUnaryOpName(ctx, op, name));
	}

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "UnaryOpNameB", __FILE__, __LINE__);

	j=BGBCC_CCXL_LookupAsRegister(ctx, name, &sreg);
	if(j<=0)
	{
		if(!j)return(CCXL_STATUS_ERR_LOOKUPFAIL);
//		if(i<0)return(i);
		if(j<0)return(j);
		return(CCXL_STATUS_ERR_GENERIC);
	}

	opr=-1;
	if(!strcmp(op, "+"))opr=CCXL_UNOP_MOV;
	if(!strcmp(op, "-"))opr=CCXL_UNOP_NEG;
	if(!strcmp(op, "~"))opr=CCXL_UNOP_NOT;
	if(!strcmp(op, "!"))opr=CCXL_UNOP_LNOT;
	if(!strcmp(op, "++"))opr=CCXL_UNOP_INC;
	if(!strcmp(op, "--"))opr=CCXL_UNOP_DEC;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LDUNOP);
	BGBCC_CCXLR3_EmitArgInt(ctx, opr+(mod*16));
	BGBCC_CCXLR3_EmitArgString(ctx, name);

	sty=BGBCC_CCXL_GetRegType(ctx, sreg);

	if(BGBCC_CCXL_TypePointerP(ctx, sty))
	{
		if((opr==CCXL_UNOP_INC) ||
			(opr==CCXL_UNOP_DEC))
		{
			BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
			k=(opr==CCXL_UNOP_INC)?(1):(-1);
//			BGBCC_CCXL_RegisterAllocTemporary(ctx, sty, &dreg);
//			BGBCC_CCXL_EmitLeaImm(ctx, bty, dreg, sreg, k);
//			BGBCC_CCXL_PushRegister(ctx, dreg);

			if(mod==0)
			{
				BGBCC_CCXL_EmitLeaImm(ctx, bty, sreg, sreg, k);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				return(CCXL_STATUS_YES);
			}

			if(mod==1)
			{
				BGBCC_CCXL_EmitLeaImm(ctx, bty, sreg, sreg, k);
//				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_PushRegister(ctx, sreg);
				return(CCXL_STATUS_YES);
			}

			if(mod==2)
			{
				BGBCC_CCXL_RegisterAllocTemporary(ctx, sty, &dreg);
				BGBCC_CCXL_EmitMov(ctx, sty, dreg, sreg);
				BGBCC_CCXL_EmitLeaImm(ctx, bty, sreg, sreg, k);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_PushRegister(ctx, dreg);
				return(CCXL_STATUS_YES);
			}

			BGBCC_DBGBREAK
			return(CCXL_STATUS_ERR_GENERIC);
		}

//		BGBCC_DBGBREAK
	}

	if(mod==0)
	{
		BGBCC_CCXL_EmitUnaryOp(ctx, sty, opr, sreg, sreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
		return(CCXL_STATUS_YES);
	}

	if(mod==1)
	{
		BGBCC_CCXL_EmitUnaryOp(ctx, sty, opr, sreg, sreg);
//		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
		BGBCC_CCXL_PushRegister(ctx, sreg);
		return(CCXL_STATUS_YES);
	}

	if(mod==2)
	{
		BGBCC_CCXL_RegisterAllocTemporary(ctx, sty, &dreg);
		BGBCC_CCXL_EmitMov(ctx, sty, dreg, sreg);
		BGBCC_CCXL_EmitUnaryOp(ctx, sty, opr, sreg, sreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
		BGBCC_CCXL_PushRegister(ctx, dreg);
		return(CCXL_STATUS_YES);
	}

	BGBCC_DBGBREAK
	return(CCXL_STATUS_ERR_GENERIC);

//	BGBCC_CCXL_TypeFromSig(ctx, &dty, sig);
//	BGBCC_CCXL_RegisterAllocTemporary(ctx, sty, &dreg);
//	BGBCC_CCXL_EmitConv(ctx, dty, sty, dreg, sreg);
//	BGBCC_CCXL_EmitUnaryOp(ctx, sty, opr, dreg, sreg);
//	BGBCC_CCXL_PushRegister(ctx, dreg);
//	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
//	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_StackUnaryOpStore(BGBCC_TransState *ctx,
	char *op, char *dname)
{
	ccxl_register sreg, dreg, dreg2;
	ccxl_type sty, dty, bty;
	int opr, cnv;
	int i, j, k;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "UnaryOpStore", __FILE__, __LINE__);

	opr=-1;
	if(!strcmp(op, "+"))opr=CCXL_UNOP_MOV;
	if(!strcmp(op, "-"))opr=CCXL_UNOP_NEG;
	if(!strcmp(op, "~"))opr=CCXL_UNOP_NOT;
	if(!strcmp(op, "!"))opr=CCXL_UNOP_LNOT;
	if(!strcmp(op, "++"))opr=CCXL_UNOP_INC;
	if(!strcmp(op, "--"))opr=CCXL_UNOP_DEC;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_STUNOP);
	BGBCC_CCXLR3_EmitArgInt(ctx, opr);
	BGBCC_CCXLR3_EmitArgString(ctx, dname);

	j=BGBCC_CCXL_LookupAsRegister(ctx, dname, &dreg);
	if(j<=0)
	{
		if(!j)return(CCXL_STATUS_ERR_LOOKUPFAIL);
//		if(i<0)return(i);
		if(j<0)return(j);
		return(CCXL_STATUS_ERR_GENERIC);
	}

	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	sty=BGBCC_CCXL_GetRegType(ctx, sreg);
	dty=BGBCC_CCXL_GetRegType(ctx, dreg);

//	if(!BGBCC_CCXL_TypeEqualP(ctx, dty, sty))
	if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty, sty))
	{
		dreg2=dreg; cnv=1;
		BGBCC_CCXL_RegisterAllocTemporary(ctx, sty, &dreg);
	}else
	{
		dreg2=dreg; cnv=0;
	}

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty))
	{
		if((opr==CCXL_UNOP_INC) ||
			(opr==CCXL_UNOP_DEC))
		{
			BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
			k=(opr==CCXL_UNOP_INC)?(1):(-1);
//			BGBCC_CCXL_RegisterAllocTemporary(ctx, sty, &dreg);
			
			if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
				__debugbreak();
			
			BGBCC_CCXL_EmitLeaImm(ctx, bty, dreg, sreg, k);
//			BGBCC_CCXL_PushRegister(ctx, dreg);

			if(cnv)
			{
				BGBCC_CCXL_EmitConv(ctx, dty, sty, dreg2, dreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, dreg2);
			}

			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
			return(CCXL_STATUS_YES);
		}
		
		if(opr==CCXL_UNOP_LNOT)
		{
			BGBCC_CCXL_EmitUnaryOp(ctx, sty, opr, dreg, sreg);

			if(cnv)
			{
				BGBCC_CCXL_EmitConv(ctx, dty, sty, dreg2, dreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, dreg2);
			}

			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
			return(CCXL_STATUS_YES);
		}
		
		BGBCC_DBGBREAK
	}

//	BGBCC_CCXL_TypeFromSig(ctx, &dty, sig);
//	BGBCC_CCXL_RegisterAllocTemporary(ctx, sty, &dreg);
//	BGBCC_CCXL_EmitConv(ctx, dty, sty, dreg, sreg);

	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{ BGBCC_DBGBREAK }

	BGBCC_CCXL_EmitUnaryOp(ctx, sty, opr, dreg, sreg);

	if(cnv)
	{
		BGBCC_CCXL_EmitConv(ctx, dty, sty, dreg2, dreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, dreg2);
	}

	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_StackLoadSlotStore(
	BGBCC_TransState *ctx, char *name,
	char *dname)
{
	BGBCC_CCXL_LiteralInfo *st;
	ccxl_register sreg, dreg, treg;
	ccxl_type bty, sty, bty2;
	int i, j;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_STLDSLOT);
	BGBCC_CCXLR3_EmitArgString(ctx, name);
	BGBCC_CCXLR3_EmitArgString(ctx, dname);

	return(BGBCC_CCXL_StackLoadSlotStoreI(ctx, name, dname));
}

ccxl_status BGBCC_CCXL_StackLoadSlotStoreI(
	BGBCC_TransState *ctx, char *name,
	char *dname)
{
	BGBCC_CCXL_LiteralInfo *st;
	ccxl_register sreg, dreg, treg;
	ccxl_type bty, sty, bty2;
	int i0, i1;
	int i, j;

	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	sty=BGBCC_CCXL_GetRegType(ctx, sreg);

	j=BGBCC_CCXL_LookupAsRegister(ctx, dname, &dreg);
	if(j<=0)
	{
		if(!j)return(CCXL_STATUS_ERR_LOOKUPFAIL);
//		if(i<0)return(i);
		if(j<0)return(j);
		return(CCXL_STATUS_ERR_GENERIC);
	}

	st=BGBCC_CCXL_LookupStructureForType(ctx, sty);

	if(BGBCC_CCXL_IsRegThisP(ctx, sreg))
	{
		i=BGBCC_CCXL_LookupStructSuperFieldType(
			ctx, st, name,
			&i0, &i1, &bty);

		if(i>0)
		{
			treg.val=CCXL_REGTY_THISIDX|
				(((s64)bty.val)<<CCXL_REGID_TYPESHIFT)|
				((i1>> 4)&CCXL_REGTHIS_FIDTYMASK)|
				((i0<<12)&CCXL_REGTHIS_LVLMASK)|
				((i1    )&CCXL_REGTHIS_FIDBMASK);
			BGBCC_CCXL_EmitMov(ctx, bty, dreg, treg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
			return(CCXL_STATUS_YES);
		}
	}

	if(BGBCC_CCXL_LookupStructFieldType(ctx, st, name, &bty)>0)
	{
		BGBCC_CCXL_EmitLoadSlot(ctx, bty, dreg, sreg, st, name, NULL);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
		return(CCXL_STATUS_YES);
	}
	
	j=BGBCC_CCXL_LookupStructContainsFieldID(ctx, st, name);
	if(j>=0)
	{
		BGBCC_CCXL_LookupStructFieldIdType(ctx, st, j, &bty);
		BGBCC_CCXL_TypePointerType(ctx, bty, &bty2);

		BGBCC_CCXL_EmitLoadSlotAddrID(ctx, bty2, dreg, sreg, st, j);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
		return(BGBCC_CCXL_StackLoadSlotStoreI(ctx, name, dname));
	}
	
	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_BADOPARGS));
	return(CCXL_STATUS_ERR_BADOPARGS);
}


ccxl_status BGBCC_CCXL_StackLoadSlotAddrStore(
	BGBCC_TransState *ctx, char *name,
	char *dname)
{
	BGBCC_CCXL_LiteralInfo *st;
	ccxl_register sreg, dreg, treg;
	ccxl_type bty, sty, bty2;
	int i, j;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_STLDSLOTA);
	BGBCC_CCXLR3_EmitArgString(ctx, name);
	BGBCC_CCXLR3_EmitArgString(ctx, dname);

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_BADOPARGS));
	return(CCXL_STATUS_ERR_BADOPARGS);
}


ccxl_status BGBCC_CCXL_StackLoadIndexConstStore(
	BGBCC_TransState *ctx, int idx, char *dname)
{
	ccxl_register treg, dreg, sreg;
	ccxl_type bty, bty2;
	int i, j, k;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "StLoadIndexC", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_STLDIXC);
	BGBCC_CCXLR3_EmitArgInt(ctx, idx);
	BGBCC_CCXLR3_EmitArgString(ctx, dname);

	j=BGBCC_CCXL_LookupAsRegister(ctx, dname, &dreg);
	if(j<=0)
	{
		if(!j)return(CCXL_STATUS_ERR_LOOKUPFAIL);
//		if(i<0)return(i);
		if(j<0)return(j);
		return(CCXL_STATUS_ERR_GENERIC);
	}

	/* src -- src[idx] */
	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	bty=BGBCC_CCXL_GetRegDerefType(ctx, sreg);
	BGBCC_CCXL_TypeAutoPromoteType(ctx, bty, &bty2);
//	BGBCC_CCXL_RegisterAllocTemporaryInit(ctx, bty2, &dreg);
	BGBCC_CCXL_EmitLoadIndexImm(ctx, bty, dreg, sreg, idx);
//	BGBCC_CCXL_PushRegister(ctx, dreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_StackLoadIndexStore(BGBCC_TransState *ctx, char *dname)
{
	ccxl_register dreg, sreg, treg;
	ccxl_type bty, bty2;
	int i, j, k;
	
	BGBCC_CCXL_DebugPrintStackLLn(ctx, "StLoadIndex", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_STLDIX);
	BGBCC_CCXLR3_EmitArgString(ctx, dname);

	j=BGBCC_CCXL_LookupAsRegister(ctx, dname, &dreg);
	if(j<=0)
	{
		if(!j)return(CCXL_STATUS_ERR_LOOKUPFAIL);
//		if(i<0)return(i);
		if(j<0)return(j);
		return(CCXL_STATUS_ERR_GENERIC);
	}

	/* src idx -- src[idx] */
	i=BGBCC_CCXL_PopRegister(ctx, &treg);
	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	bty=BGBCC_CCXL_GetRegDerefType(ctx, sreg);
	BGBCC_CCXL_TypeAutoPromoteType(ctx, bty, &bty2);
//	BGBCC_CCXL_RegisterAllocTemporaryInit(ctx, bty2, &dreg);

//	bty=BGBCC_CCXL_GetRegDerefType(ctx, sreg);
//	bty=BGBCC_CCXL_GetRegType(ctx, sreg);
	BGBCC_CCXL_EmitLoadIndex(ctx, bty, dreg, sreg, treg);
//	BGBCC_CCXL_PushRegister(ctx, dreg);

	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_StackCastSigStore(
	BGBCC_TransState *ctx, char *sig, char *dname)
{
	ccxl_register sreg, dreg;
	ccxl_type sty, dty;
	int i, j, k;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "StCastSig", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_STCASTSIG);
	BGBCC_CCXLR3_EmitArgString(ctx, sig);
	BGBCC_CCXLR3_EmitArgString(ctx, dname);

	j=BGBCC_CCXL_LookupAsRegister(ctx, dname, &dreg);
	if(j<=0)
	{
		if(!j)return(CCXL_STATUS_ERR_LOOKUPFAIL);
//		if(i<0)return(i);
		if(j<0)return(j);
		return(CCXL_STATUS_ERR_GENERIC);
	}

	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	sty=BGBCC_CCXL_GetRegType(ctx, sreg);
	BGBCC_CCXL_TypeFromSig(ctx, &dty, sig);
//	BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &dreg);
	BGBCC_CCXL_EmitConv(ctx, dty, sty, dreg, sreg);
//	BGBCC_CCXL_PushRegister(ctx, dreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
	return(CCXL_STATUS_YES);

//	BGBCC_CCXL_StubError(ctx);
}

ccxl_status BGBCC_CCXL_StackPushConstSmallInt(
	BGBCC_TransState *ctx, s32 val, int st)
{
	ccxl_register sreg;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "PushConstI", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LDCONSTI);
	BGBCC_CCXLR3_EmitArgInt(ctx, val);

	BGBCC_CCXL_GetRegForSmallIntValue(ctx, &sreg, val, st);
	BGBCC_CCXL_PushRegister(ctx, sreg);
	return(CCXL_STATUS_YES);
//	BGBCC_CCXL_StubError(ctx);
}

ccxl_status BGBCC_CCXL_StackPushConstInt(BGBCC_TransState *ctx, s32 val)
{
	ccxl_register sreg;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "PushConstI", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LDCONSTI);
	BGBCC_CCXLR3_EmitArgInt(ctx, val);

	BGBCC_CCXL_GetRegForIntValue(ctx, &sreg, val);
	BGBCC_CCXL_PushRegister(ctx, sreg);
	return(CCXL_STATUS_YES);
//	BGBCC_CCXL_StubError(ctx);
}

ccxl_status BGBCC_CCXL_StackPushConstUInt(BGBCC_TransState *ctx, u32 val)
{
	ccxl_register sreg;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "PushConstI", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LDCONSTI);
	BGBCC_CCXLR3_EmitArgInt(ctx, val);

	BGBCC_CCXL_GetRegForUIntValue(ctx, &sreg, val);
	BGBCC_CCXL_PushRegister(ctx, sreg);
	return(CCXL_STATUS_YES);
//	BGBCC_CCXL_StubError(ctx);
}

ccxl_status BGBCC_CCXL_StackPushConstLong(BGBCC_TransState *ctx, s64 val)
{
	ccxl_register sreg;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "PushConstL", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LDCONSTL);
	BGBCC_CCXLR3_EmitArgInt(ctx, val);

	BGBCC_CCXL_GetRegForLongValue(ctx, &sreg, val);
	BGBCC_CCXL_PushRegister(ctx, sreg);
	return(CCXL_STATUS_YES);
//	BGBCC_CCXL_StubError(ctx);
}

ccxl_status BGBCC_CCXL_StackPushConstInt128(
	BGBCC_TransState *ctx, s64 val_lo, s64 val_hi)
{
	ccxl_register sreg;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "PushConstI128", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LDCONSTXL);
	BGBCC_CCXLR3_EmitArgInt(ctx, val_lo);
	BGBCC_CCXLR3_EmitArgInt(ctx, val_hi);

	BGBCC_CCXL_GetRegForInt128Value(ctx, &sreg, val_lo, val_hi);
	BGBCC_CCXL_PushRegister(ctx, sreg);
	return(CCXL_STATUS_YES);
//	BGBCC_CCXL_StubError(ctx);
}

ccxl_status BGBCC_CCXL_StackPushConstFloat128(
	BGBCC_TransState *ctx, s64 val_lo, s64 val_hi)
{
	ccxl_register sreg;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "PushConstF128", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LDCONSTXF);
	BGBCC_CCXLR3_EmitArgInt(ctx, val_lo);
	BGBCC_CCXLR3_EmitArgInt(ctx, val_hi);

	BGBCC_CCXL_GetRegForFloat128Value(ctx, &sreg, val_lo, val_hi);
	BGBCC_CCXL_PushRegister(ctx, sreg);
	return(CCXL_STATUS_YES);
//	BGBCC_CCXL_StubError(ctx);
}

ccxl_status BGBCC_CCXL_StackPushConstFloat(
	BGBCC_TransState *ctx, double val)
{
	ccxl_register sreg;
	BGBCC_CCXL_DebugPrintStackLLn(ctx, "PushConstF", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LDCONSTF);
	BGBCC_CCXLR3_EmitArgFloat(ctx, val);

	BGBCC_CCXL_GetRegForFloatValue(ctx, &sreg, val);
	BGBCC_CCXL_PushRegister(ctx, sreg);
	return(CCXL_STATUS_YES);
//	BGBCC_CCXL_StubError(ctx);
}

ccxl_status BGBCC_CCXL_StackPushConstDouble(
	BGBCC_TransState *ctx, double val)
{
	ccxl_register sreg;
	BGBCC_CCXL_DebugPrintStackLLn(ctx, "PushConstD", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LDCONSTD);
	BGBCC_CCXLR3_EmitArgFloat(ctx, val);

	BGBCC_CCXL_GetRegForDoubleValue(ctx, &sreg, val);
	BGBCC_CCXL_PushRegister(ctx, sreg);
	return(CCXL_STATUS_YES);
//	BGBCC_CCXL_StubError(ctx);
}

ccxl_status BGBCC_CCXL_StackPushConstString(
	BGBCC_TransState *ctx, char *val)
{
	ccxl_register sreg;
	BGBCC_CCXL_DebugPrintStackLLn(ctx, "PushConstS", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LDCONSTS);
	BGBCC_CCXLR3_EmitArgString(ctx, val);

	BGBCC_CCXL_GetRegForStringValue(ctx, &sreg, val);
	BGBCC_CCXL_PushRegister(ctx, sreg);
	return(CCXL_STATUS_YES);
//	BGBCC_CCXL_StubError(ctx);
}

ccxl_status BGBCC_CCXL_StackPushConstWString(
	BGBCC_TransState *ctx, char *val)
{
	ccxl_register sreg;
	BGBCC_CCXL_DebugPrintStackLLn(ctx, "PushConstWS", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LDCONSTWS);
	BGBCC_CCXLR3_EmitArgString(ctx, val);

	BGBCC_CCXL_GetRegForWStringValue(ctx, &sreg, val);
	BGBCC_CCXL_PushRegister(ctx, sreg);
	return(CCXL_STATUS_YES);
//	BGBCC_CCXL_StubError(ctx);
}

/** Push 'void', only valid operation on Void is Pop */
ccxl_status BGBCC_CCXL_StackPushVoid(BGBCC_TransState *ctx)
{
	ccxl_register sreg;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "PushConstVoid", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LDCONSTV);

//	BGBCC_CCXL_GetRegForIntValue(ctx, &sreg, 0);
	sreg.val=CCXL_REGID_REG_Z;

	BGBCC_CCXL_PushRegister(ctx, sreg);
	return(CCXL_STATUS_YES);
}

/** Push 'void', only valid operation on Void is Pop */
ccxl_status BGBCC_CCXL_StackPushTempObj(BGBCC_TransState *ctx, char *sig)
{
	ccxl_register treg;
	ccxl_type bty, pty;
	int i;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_MKTMPOBJ);
	BGBCC_CCXLR3_EmitArgString(ctx, sig);

	BGBCC_CCXL_TypeFromSig(ctx, &bty, sig);
	BGBCC_CCXL_RegisterAllocTemporaryInit(ctx, bty, &treg);
	BGBCC_CCXL_PushRegister(ctx, treg);
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_StackPop(BGBCC_TransState *ctx)
{
	ccxl_register sreg;
	int i;
	BGBCC_CCXL_DebugPrintStackLLn(ctx, "Pop", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_POP);

	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_StackLoadAddr(BGBCC_TransState *ctx, char *name)
{
	ccxl_register sreg, dreg;
	ccxl_type bty, pty;
	int i;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "LoadA", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LDA);
	BGBCC_CCXLR3_EmitArgString(ctx, name);

//	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	i=BGBCC_CCXL_LookupAsRegister(ctx, name, &sreg);

//	bty=BGBCC_CCXL_GetRegPointerToType(ctx, sreg);
	bty=BGBCC_CCXL_GetRegType(ctx, sreg);
	BGBCC_CCXL_TypePointerType(ctx, bty, &pty);

	if(BGBCC_CCXL_TypeValueObjectP(ctx, bty) ||
		BGBCC_CCXL_TypeArrayP(ctx, bty))
	{
		BGBCC_CCXL_RegisterAllocTemporary(ctx, pty, &dreg);
		BGBCC_CCXL_EmitMov(ctx, pty, dreg, sreg);
		BGBCC_CCXL_PushRegister(ctx, dreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
		return(CCXL_STATUS_YES);
	}

	BGBCC_CCXL_RegisterAllocTemporary(ctx, pty, &dreg);
	BGBCC_CCXL_EmitLdaVar(ctx, pty, dreg, sreg);
	BGBCC_CCXL_PushRegister(ctx, dreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_StackSizeofSig(BGBCC_TransState *ctx, char *sig)
{
	ccxl_register sreg, dreg;
	ccxl_type bty;
	int sz;
	int i;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "SizeOfSig", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_SIZEOFSG);
	BGBCC_CCXLR3_EmitArgString(ctx, sig);

//	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
//	bty=BGBCC_CCXL_GetRegType(ctx, sreg);
	BGBCC_CCXL_TypeFromSig(ctx, &bty, sig);

	sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, bty);
	if(sz>0)
	{
		BGBCC_CCXL_StackPushConstInt(ctx, sz);
		return(CCXL_STATUS_YES);
	}

	BGBCC_CCXL_RegisterAllocTemporaryInt(ctx, &dreg);
	BGBCC_CCXL_EmitSizeofVar(ctx, bty, dreg);
	BGBCC_CCXL_PushRegister(ctx, dreg);

//	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	return(CCXL_STATUS_YES);

//	BGBCC_CCXL_StubError(ctx);
//	return(CCXL_STATUS_NO);
}

ccxl_status BGBCC_CCXL_StackSizeofVal(BGBCC_TransState *ctx)
{
	ccxl_register sreg, dreg;
	ccxl_type bty;
	int sz;
	int i;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "SizeOfVal", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_SIZEOFVAL);

	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	bty=BGBCC_CCXL_GetRegType(ctx, sreg);

	sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, bty);
	if(sz>0)
	{
		BGBCC_CCXL_StackPushConstInt(ctx, sz);
		return(CCXL_STATUS_YES);
	}

	BGBCC_CCXL_RegisterAllocTemporaryInt(ctx, &dreg);
	BGBCC_CCXL_EmitSizeofVar(ctx, bty, dreg);
	BGBCC_CCXL_PushRegister(ctx, dreg);

	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	return(CCXL_STATUS_YES);

//	BGBCC_CCXL_StubError(ctx);
//	return(CCXL_STATUS_NO);
}

ccxl_status BGBCC_CCXL_StackOffsetof(BGBCC_TransState *ctx,
	char *sig, char *name)
{
	BGBCC_CCXL_LiteralInfo *st;
	BGBCC_CCXL_RegisterInfo *fi;
	ccxl_register sreg, dreg;
	ccxl_type bty;
	int fn;
	int i;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_OFFSETOF);
	BGBCC_CCXLR3_EmitArgString(ctx, sig);
	BGBCC_CCXLR3_EmitArgString(ctx, name);

	bty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_I);
	st=BGBCC_CCXL_LookupStructureForSig(ctx, sig);
	if(st->decl && st->decl->fxmsize &&
		(st->decl->fxmsize==st->decl->fxnsize))
	{
		fn=BGBCC_CCXL_LookupStructFieldID(ctx, st, name);
		fi=st->decl->fields[fn];
		
		if(fi->fxmoffs==fi->fxnoffs)
		{
			BGBCC_CCXL_StackPushConstInt(ctx, fi->fxmoffs);
			return(CCXL_STATUS_YES);
		}
	}

	BGBCC_CCXL_RegisterAllocTemporary(ctx, bty, &dreg);
	BGBCC_CCXL_EmitOffsetOf(ctx, bty, dreg, st, name);
	BGBCC_CCXL_PushRegister(ctx, dreg);
	return(CCXL_STATUS_YES);
	
//	BGBCC_CCXL_StubError(ctx);
//	return(CCXL_STATUS_NO);
}

int BGBCC_CCXL_CheckForGetter(BGBCC_TransState *ctx, char *name)
{
	ccxl_register sreg, dreg, treg;
	ccxl_type bty, sty, bty2;

	BGBCC_CCXL_PeekRegister(ctx, &sreg);
	sty=BGBCC_CCXL_GetRegType(ctx, sreg);
	return(BGBCC_CCXL_CheckForTypeGetter(ctx, sty, name));
}

int BGBCC_CCXL_CheckForTypeGetter(BGBCC_TransState *ctx,
	ccxl_type sty, char *name)
{
	char tb1[256];
	BGBCC_CCXL_LiteralInfo *st;
//	ccxl_register sreg, dreg, treg;
	ccxl_type bty;
	int i, j;

	st=BGBCC_CCXL_LookupStructureForType(ctx, sty);

	sprintf(tb1, "get_%s", name);
	if(BGBCC_CCXL_LookupStructFieldType(ctx, st, tb1, &bty)>0)
	{
		return(1);
	}
	
	return(0);
}

int BGBCC_CCXL_CheckForTypeSetter(BGBCC_TransState *ctx,
	ccxl_type sty, char *name)
{
	char tb1[256];
	BGBCC_CCXL_LiteralInfo *st;
//	ccxl_register sreg, dreg, treg;
	ccxl_type bty;
	int i, j;

	st=BGBCC_CCXL_LookupStructureForType(ctx, sty);

	sprintf(tb1, "set_%s", name);
	if(BGBCC_CCXL_LookupStructFieldType(ctx, st, tb1, &bty)>0)
	{
		return(1);
	}
	
	return(0);
}

int BGBCC_CCXL_CheckForCNameGetter(BGBCC_TransState *ctx,
	char *cname, char *name)
{
	char tb1[256];
	BGBCC_CCXL_LiteralInfo *st;
//	ccxl_register sreg, dreg, treg;
	ccxl_type bty;
	int i, j;

//	st=BGBCC_CCXL_LookupStructureForType(ctx, sty);
	st=BGBCC_CCXL_LookupStructure(ctx, cname);

	sprintf(tb1, "get_%s", name);
	if(BGBCC_CCXL_LookupStructFieldType(ctx, st, tb1, &bty)>0)
	{
		return(1);
	}
	
	return(0);
}

int BGBCC_CCXL_CheckForCNameSetter(BGBCC_TransState *ctx,
	char *cname, char *name)
{
	char tb1[256];
	BGBCC_CCXL_LiteralInfo *st;
//	ccxl_register sreg, dreg, treg;
	ccxl_type bty;
	int i, j;

//	st=BGBCC_CCXL_LookupStructureForType(ctx, sty);
	st=BGBCC_CCXL_LookupStructure(ctx, cname);

	sprintf(tb1, "set_%s", name);
	if(BGBCC_CCXL_LookupStructFieldType(ctx, st, tb1, &bty)>0)
	{
		return(1);
	}
	
	return(0);
}


ccxl_status BGBCC_CCXL_StackLoadSlot(BGBCC_TransState *ctx, char *name)
{
	return(BGBCC_CCXL_StackLoadSlotSig(ctx, name, NULL));
}

ccxl_status BGBCC_CCXL_StackLoadSlotSig(BGBCC_TransState *ctx,
	char *name, char *sig)
{
	char tb1[256];
	BGBCC_CCXL_LiteralInfo *st;
	ccxl_register sreg, dreg, treg;
	ccxl_type bty, sty, bty2;
	int i0, i1;
	int i, j;

	i=BGBCC_CCXL_PeekRegister(ctx, &sreg);
	sty=BGBCC_CCXL_GetRegType(ctx, sreg);

	if(BGBCC_CCXL_TypeVecP(ctx, sty))
	{
		i=-1;
		
		if((i<0) && BGBCC_CCXL_TypeComplexP(ctx, sty))
		{
			if(!strcmp(name, "r"))	i=0;
			if(!strcmp(name, "i"))	i=1;

			if(!strcmp(name, "real"))	i=0;
			if(!strcmp(name, "imag"))	i=1;
		}
		
		if(i<0)
		{
			if(!strcmp(name, "x"))	i=0;
			if(!strcmp(name, "y"))	i=1;
			if(!strcmp(name, "z"))	i=2;
			if(!strcmp(name, "w"))	i=3;
		}

		if(i<0)
		{
			if(!strcmp(name, "b"))	i=0;
			if(!strcmp(name, "g"))	i=1;
			if(!strcmp(name, "r"))	i=2;
			if(!strcmp(name, "a"))	i=3;
		}

		if(i<0)
		{
			if(!strcmp(name, "s"))	i=0;
			if(!strcmp(name, "t"))	i=1;
			if(!strcmp(name, "u"))	i=2;
			if(!strcmp(name, "v"))	i=3;
		}
		
		if(i>=0)
		{
			return(BGBCC_CCXL_StackLoadIndexConst(ctx, i));
		}
	}

//	BGBCC_CCXL_LookupStructure()

	if(sig)
	{
		BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LDSLOTSIG);
		BGBCC_CCXLR3_EmitArgString(ctx, name);
		BGBCC_CCXLR3_EmitArgString(ctx, sig);
	}else
	{
		BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LOADSLOT);
		BGBCC_CCXLR3_EmitArgString(ctx, name);
	}

	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	sty=BGBCC_CCXL_GetRegType(ctx, sreg);

	st=BGBCC_CCXL_LookupStructureForType(ctx, sty);

	if(BGBCC_CCXL_IsRegThisP(ctx, sreg))
	{
		i=BGBCC_CCXL_LookupStructSuperFieldType(
			ctx, st, name,
			&i0, &i1, &bty);

		if(i>0)
		{
			treg.val=CCXL_REGTY_THISIDX|
				(((s64)bty.val)<<CCXL_REGID_TYPESHIFT)|
				((i1>> 4)&CCXL_REGTHIS_FIDTYMASK)|
				((i0<<12)&CCXL_REGTHIS_LVLMASK)|
				((i1    )&CCXL_REGTHIS_FIDBMASK);
			BGBCC_CCXL_PushRegister(ctx, treg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			return(CCXL_STATUS_YES);
		}
	}

//	st=BGBCC_CCXL_LookupStructureForSig(ctx, sig);
//	bty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_I);

//	if(BGBCC_CCXL_LookupStructFieldType(ctx, st, name, &bty)>0)
	if(BGBCC_CCXL_LookupStructFieldTypeSig(ctx, st, name, sig, &bty)>0)
	{
		BGBCC_CCXL_RegisterAllocTemporaryInit(ctx, bty, &dreg);
		BGBCC_CCXL_EmitLoadSlot(ctx, bty, dreg, sreg, st, name, sig);
		BGBCC_CCXL_PushRegister(ctx, dreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
		return(CCXL_STATUS_YES);
	}
	
	j=BGBCC_CCXL_LookupStructContainsFieldID(ctx, st, name);
	if(j>=0)
	{
		BGBCC_CCXL_LookupStructFieldIdType(ctx, st, j, &bty);
		BGBCC_CCXL_TypePointerType(ctx, bty, &bty2);

		BGBCC_CCXL_RegisterAllocTemporaryInit(ctx, bty2, &dreg);
		BGBCC_CCXL_EmitLoadSlotAddrID(ctx, bty2, dreg, sreg, st, j);
		BGBCC_CCXL_PushRegister(ctx, dreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);

		ctx->ril3_noril++;
		i=BGBCC_CCXL_StackLoadSlot(ctx, name);
		ctx->ril3_noril--;
		return(i);
	}
	
	
	BGBCC_CCXL_PushRegister(ctx, sreg);

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_BADOPARGS));
	return(CCXL_STATUS_ERR_BADOPARGS);
}

ccxl_status BGBCC_CCXL_StackStoreSlot(BGBCC_TransState *ctx, char *name)
{
	BGBCC_CCXL_LiteralInfo *st;
	ccxl_register sreg, dreg, treg;
	ccxl_type bty, sty, fty, bty2;
	int i0, i1;
	int i, j;

	//value obj --

//	BGBCC_CCXL_LookupStructure()

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_STORESLOT);
	BGBCC_CCXLR3_EmitArgString(ctx, name);

	BGBCC_CCXL_PopRegister(ctx, &dreg);
	BGBCC_CCXL_PopRegister(ctx, &sreg);
	sty=BGBCC_CCXL_GetRegType(ctx, dreg);
	bty=BGBCC_CCXL_GetRegType(ctx, sreg);

	st=BGBCC_CCXL_LookupStructureForType(ctx, sty);

	if(BGBCC_CCXL_IsRegThisP(ctx, dreg))
	{
		i=BGBCC_CCXL_LookupStructSuperFieldType(
			ctx, st, name,
			&i0, &i1, &bty);

		if(i>0)
		{
			treg.val=CCXL_REGTY_THISIDX|
				(((s64)bty.val)<<CCXL_REGID_TYPESHIFT)|
				((i1>> 4)&CCXL_REGTHIS_FIDTYMASK)|
				((i0<<12)&CCXL_REGTHIS_LVLMASK)|
				((i1    )&CCXL_REGTHIS_FIDBMASK);
			BGBCC_CCXL_EmitMov(ctx, bty, treg, sreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			return(CCXL_STATUS_YES);
		}
	}

//	st=BGBCC_CCXL_LookupStructureForSig(ctx, sig);
//	bty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_I);
	if(BGBCC_CCXL_LookupStructFieldType(ctx, st, name, &fty)>0)
	{
//		if(BGBCC_CCXL_TypeBaseSmallEqualP(ctx, fty, bty))
		if(BGBCC_CCXL_TypeCompatibleFlP(ctx, fty, bty, 3))
		{
//			BGBCC_CCXL_EmitStoreSlot(ctx, bty, dreg, sreg, st, name);
			BGBCC_CCXL_EmitStoreSlot(ctx, fty, dreg, sreg, st, name);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			return(CCXL_STATUS_YES);
		}

		BGBCC_CCXL_RegisterAllocTemporary(ctx, fty, &treg);
//		BGBCC_CCXL_EmitConv(ctx, fty, sty, treg, sreg);
		BGBCC_CCXL_EmitConv(ctx, fty, bty, treg, sreg);
		BGBCC_CCXL_EmitStoreSlot(ctx, fty, dreg, treg, st, name);
		BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
		return(CCXL_STATUS_YES);
	}

#if 1
	j=BGBCC_CCXL_LookupStructContainsFieldID(ctx, st, name);
	if(j>=0)
	{
		BGBCC_CCXL_LookupStructFieldIdType(ctx, st, j, &bty);
		BGBCC_CCXL_TypePointerType(ctx, bty, &bty2);

		BGBCC_CCXL_RegisterAllocTemporaryInit(ctx, bty2, &treg);
//		BGBCC_CCXL_EmitStoreSlotAddrID(ctx, bty2, dreg, sreg, st, j);
		BGBCC_CCXL_EmitLoadSlotAddrID(ctx, bty2, treg, sreg, st, j);

		BGBCC_CCXL_PushRegister(ctx, sreg);
		BGBCC_CCXL_PushRegister(ctx, treg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
		
		ctx->ril3_noril++;
		i=BGBCC_CCXL_StackStoreSlot(ctx, name);
		ctx->ril3_noril--;
		return(i);
	}
#endif
	
	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_BADOPARGS));
	return(CCXL_STATUS_ERR_BADOPARGS);
}

ccxl_status BGBCC_CCXL_StackLoadSlotAddr(BGBCC_TransState *ctx, char *name)
{
	BGBCC_CCXL_LiteralInfo *st;
	ccxl_register sreg, dreg, treg;
	ccxl_type bty, sty, bty2;
	int i, j;

//	BGBCC_CCXL_LookupStructure()

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LOADSLOTA);
	BGBCC_CCXLR3_EmitArgString(ctx, name);

	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	sty=BGBCC_CCXL_GetRegType(ctx, sreg);

	st=BGBCC_CCXL_LookupStructureForType(ctx, sty);

//	st=BGBCC_CCXL_LookupStructureForSig(ctx, sig);
//	bty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_I);

	if(BGBCC_CCXL_LookupStructFieldType(ctx, st, name, &bty)>0)
	{
		BGBCC_CCXL_TypePointerType(ctx, bty, &bty2);

		BGBCC_CCXL_RegisterAllocTemporaryInit(ctx, bty2, &dreg);
		BGBCC_CCXL_EmitLoadSlotAddr(ctx, bty, dreg, sreg, st, name);
		BGBCC_CCXL_PushRegister(ctx, dreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
		return(CCXL_STATUS_YES);
	}
	
	j=BGBCC_CCXL_LookupStructContainsFieldID(ctx, st, name);
	if(j>=0)
	{
		BGBCC_CCXL_LookupStructFieldIdType(ctx, st, j, &bty);
		BGBCC_CCXL_TypePointerType(ctx, bty, &bty2);

		BGBCC_CCXL_RegisterAllocTemporaryInit(ctx, bty2, &dreg);
		BGBCC_CCXL_EmitLoadSlotAddrID(ctx, bty2, dreg, sreg, st, j);
		BGBCC_CCXL_PushRegister(ctx, dreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);

		ctx->ril3_noril++;
		i=BGBCC_CCXL_StackLoadSlotAddr(ctx, name);
		ctx->ril3_noril--;
		return(i);
	}
	
	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_BADOPARGS));
	return(CCXL_STATUS_ERR_BADOPARGS);
}

#if 0
// ccxl_status BGBCC_CCXL_StackFn(BGBCC_TransState *ctx, char *fn)
{
	int i;
	
	i=BGBCC_CCXL_IndexString(ctx, fn);
	BGBCC_CCXL_EmitOpcode(ctx, CCXL_OP_LFN);
	BGBCC_CCXL_EmitWord24(ctx, i);

//	BGBCC_CCXL_StubNote(ctx);
//	BGBCC_CCXL_StubError(ctx);
	return(CCXL_STATUS_NO);
}

// ccxl_status BGBCC_CCXL_StackLn(BGBCC_TransState *ctx, int ln)
{
	BGBCC_CCXL_EmitOpcode(ctx, CCXL_OP_LLN);
	BGBCC_CCXL_EmitWord(ctx, ln);

//	BGBCC_CCXL_StubNote(ctx);
//	BGBCC_CCXL_StubError(ctx);
	return(CCXL_STATUS_NO);
}
#endif

ccxl_status BGBCC_CCXL_StackBeginU(BGBCC_TransState *ctx, char *sig)
{
	ccxl_register sreg, dreg;
	ccxl_type bty;
	int i;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "BeginU", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_BEGINU);
	BGBCC_CCXLR3_EmitArgString(ctx, sig);

	BGBCC_CCXL_TypeFromSig(ctx, &bty, sig);
	BGBCC_CCXL_RegisterAllocTemporary(ctx, bty, &dreg);
	BGBCC_CCXL_PushURegister(ctx, dreg);
	return(CCXL_STATUS_YES);

//	BGBCC_CCXL_StubError(ctx);
//	return(CCXL_STATUS_NO);
}

ccxl_status BGBCC_CCXL_StackEndU(BGBCC_TransState *ctx)
{
	ccxl_register reg;
	ccxl_type bty;
	int i;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "EndU", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_ENDU);

	i=BGBCC_CCXL_PopURegister(ctx, &reg);
	BGBCC_CCXL_PushRegister(ctx, reg);
	return(CCXL_STATUS_YES);

//	BGBCC_CCXL_StubError(ctx);
//	return(CCXL_STATUS_NO);
}

ccxl_status BGBCC_CCXL_StackSetU(BGBCC_TransState *ctx)
{
	ccxl_register sreg, dreg;
	ccxl_type dty, sty;
	int i;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "SetU", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_SETU);

	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	i=BGBCC_CCXL_PeekURegister(ctx, &dreg);
	sty=BGBCC_CCXL_GetRegType(ctx, sreg);
	dty=BGBCC_CCXL_GetRegType(ctx, dreg);

	BGBCC_CCXL_EmitConv(ctx, dty, sty, dreg, sreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);

	return(CCXL_STATUS_YES);

//	BGBCC_CCXL_StubError(ctx);
//	return(CCXL_STATUS_NO);
}

ccxl_status BGBCC_CCXL_StackInitVar(BGBCC_TransState *ctx, char *name)
{
	BGBCC_CCXL_RegisterInfo *def;
	BGBCC_CCXL_LiteralInfo *st;
	ccxl_register sreg, dreg;
	ccxl_type bty;
	char *s;
	int i, j, k, l;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_INITVAR);
	BGBCC_CCXLR3_EmitArgString(ctx, name);

	def=BGBCC_CCXL_LookupLocalInfo(ctx, name);
	if(def)
	{
		i=BGBCC_CCXL_LookupAsRegister(ctx, name, &dreg);

		st=BGBCC_CCXL_LookupStructureForSig2(ctx, def->sig);
		if(st)
		{
			bty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_P);
			BGBCC_CCXL_EmitInitObj(ctx, bty, dreg, st);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
			return(CCXL_STATUS_YES);
		}
		
		s=def->sig;
//		l=BGBCC_CCXL_GetArraySizeForSig2R(ctx, &s);
		l=BGBCC_CCXL_GetArraySizeForSig3R(ctx, &s);
		if(l>0)
		{
			st=BGBCC_CCXL_LookupStructureForSig2(ctx, s);
			if(st)
			{
				bty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_P);
//				BGBCC_CCXL_EmitInitArr(ctx, bty, dreg, l);
				BGBCC_CCXL_EmitInitObjArr(ctx, bty, dreg, st, l);
				BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
				return(CCXL_STATUS_YES);
			}

			BGBCC_CCXL_TypeFromSig(ctx, &bty, s);
			BGBCC_CCXL_EmitInitArr(ctx, bty, dreg, l);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
			return(CCXL_STATUS_YES);
		}
	}

	def=BGBCC_CCXL_LookupArgInfo(ctx, name);
	if(def)
	{
		i=BGBCC_CCXL_LookupAsRegister(ctx, name, &dreg);

		st=BGBCC_CCXL_LookupStructureForSig2(ctx, def->sig);
		if(st)
		{
			bty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_P);
			BGBCC_CCXL_EmitInitObj(ctx, bty, dreg, st);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
			return(CCXL_STATUS_YES);
		}
	}

//	BGBCC_CCXL_StubError(ctx);
	return(CCXL_STATUS_NO);

	BGBCC_CCXL_StubError(ctx);
	return(CCXL_STATUS_NO);
}

ccxl_status BGBCC_CCXL_StackInitVarValue(BGBCC_TransState *ctx, char *name)
{
	BGBCC_CCXL_RegisterInfo *def;
	BGBCC_CCXL_LiteralInfo *st;
	ccxl_register sreg, dreg;
	ccxl_type bty, dty, sty;
	char *s;
	int i, j, k, l;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_INITVARVAL);
	BGBCC_CCXLR3_EmitArgString(ctx, name);

	def=BGBCC_CCXL_LookupLocalInfo(ctx, name);
	if(def)
	{
		BGBCC_CCXL_PopRegister(ctx, &sreg);

		i=BGBCC_CCXL_LookupAsRegister(ctx, name, &dreg);

		st=BGBCC_CCXL_LookupStructureForSig2(ctx, def->sig);
		if(st)
		{
			dty=BGBCC_CCXL_GetRegType(ctx, dreg);
			BGBCC_CCXL_EmitMov(ctx, dty, dreg, sreg);

//			BGBCC_CCXL_StubError(ctx);

//			bty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_P);
//			BGBCC_CCXL_EmitInitObj(ctx, bty, dreg, st);

			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			return(CCXL_STATUS_YES);
		}
		
		s=def->sig;
//		l=BGBCC_CCXL_GetArraySizeForSig2R(ctx, &s);
		l=BGBCC_CCXL_GetArraySizeForSig3R(ctx, &s);
		if(l>0)
		{
//			BGBCC_CCXL_StubError(ctx);

			st=BGBCC_CCXL_LookupStructureForSig2(ctx, s);
			if(st)
			{
				bty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_P);
//				BGBCC_CCXL_EmitInitObjArr(ctx, bty, dreg, st, l);
				BGBCC_CCXL_EmitLoadInitObjArr(ctx,
					bty, dreg, st, def->value, l);

				BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				return(CCXL_STATUS_YES);
			}

			BGBCC_CCXL_TypeFromSig(ctx, &bty, s);
			BGBCC_CCXL_EmitLoadInitArr(ctx, bty, dreg, def->value, l);

			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			return(CCXL_STATUS_YES);
		}

		sty=BGBCC_CCXL_GetRegType(ctx, sreg);
		dty=BGBCC_CCXL_GetRegType(ctx, dreg);
		
		if(!BGBCC_CCXL_TypeCompatibleArchP(ctx, dty, sty) &&
			!(BGBCC_CCXL_TypePointerP(ctx, dty) &&
				BGBCC_CCXL_TypeValueObjectP(ctx, sty)))
		{
			BGBCC_CCXL_EmitConv(ctx,
				dty, sty,
				dreg, sreg);

//			__debugbreak();
		}else
		{
			BGBCC_CCXL_EmitMov(ctx,
				BGBCC_CCXL_GetRegType(ctx, dreg),
				dreg, sreg);
		}
		BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);

		return(CCXL_STATUS_YES);
	}

	def=BGBCC_CCXL_LookupGlobalInfo(ctx, name);
	if(def)
	{
//		BGBCC_CCXL_StubError(ctx);

		BGBCC_CCXL_PopRegister(ctx, &sreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
		return(CCXL_STATUS_NO);
	}

	BGBCC_CCXL_StubError(ctx);
	return(CCXL_STATUS_NO);
}

ccxl_status BGBCC_CCXL_StackLitTypeSig(BGBCC_TransState *ctx, char *sig)
{
	ccxl_register sreg, dreg;
	ccxl_type sty, dty;
	int i;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "LitTypeSig", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LITTYPESIG);
	BGBCC_CCXLR3_EmitArgString(ctx, sig);

	BGBCC_CCXL_TypeFromSig(ctx, &dty, sig);
//	BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &dreg);

	dreg.val=CCXL_REGID_REG_TYZ|
		(((u64)dty.val)<<CCXL_REGID_TYPESHIFT);

	BGBCC_CCXL_PushRegister(ctx, dreg);
//	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_StackVaStart(BGBCC_TransState *ctx)
{
	ccxl_register sreg, dreg;
	ccxl_type dty, sty;
	int i;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "VaStart", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_VA_START);

	BGBCC_CCXL_PopRegister(ctx, &dreg);
	BGBCC_CCXL_PopRegister(ctx, &sreg);
//	sty=BGBCC_CCXL_GetRegType(ctx, sreg);
//	dty=BGBCC_CCXL_GetRegType(ctx, dreg);

	BGBCC_CCXL_EmitVaStart(ctx, dreg, sreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);

	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_StackVaEnd(BGBCC_TransState *ctx)
{
	ccxl_register sreg, dreg;
	ccxl_type dty, sty;
	int i;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "VaEnd", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_VA_END);
	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	BGBCC_CCXL_EmitVaEnd(ctx, sreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);

	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_StackVaArg(BGBCC_TransState *ctx)
{
	ccxl_register sreg, treg, dreg;
	ccxl_type dty, sty;
	int i, j;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "VaArg", __FILE__, __LINE__);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_VA_ARG);

	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	j=BGBCC_CCXL_PopRegister(ctx, &treg);
//	sty=BGBCC_CCXL_GetRegType(ctx, sreg);
//	dty=BGBCC_CCXL_GetRegType(ctx, dreg);

	dty=BGBCC_CCXL_GetRegType(ctx, treg);
	BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &dreg);

	BGBCC_CCXL_EmitVaArg(ctx, dty, dreg, sreg);

	BGBCC_CCXL_PushRegister(ctx, dreg);

	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, treg);

	return(CCXL_STATUS_YES);
}
