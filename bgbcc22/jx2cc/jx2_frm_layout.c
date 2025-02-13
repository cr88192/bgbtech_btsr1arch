/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

int BGBCC_JX2C_MergeGlobalSetMask(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *dobj,
	BGBCC_CCXL_RegisterInfo *sobj)
{
	int i, j, k, n;
	if(!sobj)
		return(0);
	if(!sobj->gblsetmask)
		return(0);

	if(!dobj)
		return(0);
	if(!dobj->gblsetmask)
		return(0);

	if(!(sctx->is_simpass&64))
		return(0);

	n=((ctx->n_reg_globals+63)/64);
	
	for(i=0; i<n; i++)
		{ dobj->gblsetmask[i]|=sobj->gblsetmask[i]; }

	return(1);
}

int BGBCC_JX2C_InvalidateGlobalSetMask(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *dobj)
{
	int n;

	if(!dobj)
		return(0);
	if(!dobj->gblsetmask)
		return(0);

	if(dobj->gblsetmask[0]&1)
		return(0);

	if(!(sctx->is_simpass&64))
		return(0);

	n=((ctx->n_reg_globals+63)/64);
	memset(dobj->gblsetmask, 0xFF, n*8);

	return(1);
}

int BGBCC_JX2C_MarkGlobalSetMask(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj,
	ccxl_register reg)
{
	int i, n;

	if(!obj)
		return(0);
	if(!obj->gblsetmask)
		return(0);

	if(BGBCC_CCXL_IsRegGlobalP(ctx, reg))
	{
		i=BGBCC_CCXL_GetRegID(ctx, reg);
		
		if(i>0)
		{
			obj->gblsetmask[i>>6]|=(1ULL<<(i&63));
		}
	}
	return(0);
}

int BGBCC_JX2C_CheckGlobalSetMask(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx, ccxl_register reg)
{
	int i, n;

	if(BGBCC_CCXL_IsRegGlobalP(ctx, reg))
	{
		i=BGBCC_CCXL_GetRegID(ctx, reg);
		
		if(i>0)
		{
			if(ctx->cur_func->gblsetmask[i>>6]&(1ULL<<(i&63)))
				return(1);
		}
	}
	return(0);
}

int BGBCC_JX2C_SetupFrameLayout(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	BGBCC_CCXL_VirtOp *vop, *vop1;
	BGBCC_CCXL_VirtTr *vtr;
	u32 localmap[16];
	char *fname;
	ccxl_register reg, reg1;
	ccxl_type tty;
	s64 regfl, regfl2;
	int trn;
	int ni, nf, rcls, noarg, sz, tsz, msz;
	int i, j, k, ka, kf, nogblstore, ftgblstore;

	sctx->vspan_num=0;

	ctx->cur_func=obj;
	sctx->use_bp=0;
	sctx->use_fpr=0;
	sctx->use_dbr=0;
	sctx->is_vararg=0;
	sctx->frm_offs_retstr=0;
	sctx->frm_offs_thisptr=0;
	sctx->frm_offs_isrsaves=0;

	memset(localmap, 0, 16*sizeof(u32));

	for(i=0; i<256; i++)
		bgbcc_jx2cc_framevreghash[i]=-1;

	for(i=0; i<obj->n_vop; i++)
	{
		vop=obj->vop[i];
		vop->tgt_mult=0;
	}

	kf=0;
	for(i=0; i<obj->n_vop; i++)
	{
		vop=obj->vop[i];

		if(vop->opn==CCXL_VOP_DBGFN)
		{
			ctx->lfn=vop->imm.str;
		}

		if(vop->opn==CCXL_VOP_DBGLN)
		{
			if(!kf)
				ctx->lln=vop->imm.si;
			kf++;
		}

		if(	(vop->opn==CCXL_VOP_CALL) ||
			(vop->opn==CCXL_VOP_OBJCALL) )
		{
			j=vop->imm.call.na*2;
			if(j>obj->n_cargs)
				obj->n_cargs=j;
		}

		if(vop->opn==CCXL_VOP_LDAVAR)
		{
			reg=vop->srca;
			j=reg.val&4095;
			if(j==4095)
				continue;

			if(BGBCC_CCXL_IsRegTempP(ctx, reg))
			{
				obj->regs[j]->regflags|=BGBCC_REGFL_ALIASPTR;
				obj->regs[j]->regflags|=BGBCC_REGFL_INITIALIZED;
			}
			if(BGBCC_CCXL_IsRegArgP(ctx, reg))
				{ obj->args[j]->regflags|=BGBCC_REGFL_ALIASPTR; }
			if(BGBCC_CCXL_IsRegLocalP(ctx, reg))
				{ obj->locals[j]->regflags|=BGBCC_REGFL_ALIASPTR; }
		}

		if(	(vop->opn==CCXL_VOP_STIX) ||
			(vop->opn==CCXL_VOP_STIXIMM))
		{
			reg=vop->dst;
			j=reg.val&4095;
			if(j==4095)
				continue;

			if(BGBCC_CCXL_IsRegTempP(ctx, reg))
			{
//				obj->regs[j]->regflags|=BGBCC_REGFL_ALIASPTR;
				obj->regs[j]->regflags|=BGBCC_REGFL_INITIALIZED;
			}
//			if(BGBCC_CCXL_IsRegArgP(ctx, reg))
//				{ obj->args[j]->regflags|=BGBCC_REGFL_ALIASPTR; }
//			if(BGBCC_CCXL_IsRegLocalP(ctx, reg))
//				{ obj->locals[j]->regflags|=BGBCC_REGFL_ALIASPTR; }

			obj->regflags|=BGBCC_REGFL_HASSTIX;
		}
		
		if(vop->opn==CCXL_VOP_STORESLOT)
		{
			obj->regflags|=BGBCC_REGFL_HASSTIX;
		}

		if((vop->opn==CCXL_VOP_INITOBJ) ||
			(vop->opn==CCXL_VOP_DROPOBJ) ||
			(vop->opn==CCXL_VOP_INITARR) ||
			(vop->opn==CCXL_VOP_INITOBJARR) ||
			(vop->opn==CCXL_VOP_CALL) ||
			(vop->opn==CCXL_VOP_OBJCALL) ||
			(vop->opn==CCXL_VOP_CSRV))
		{
			reg=vop->dst;
			j=reg.val&4095;
			if(j==4095)
				continue;

			if(BGBCC_CCXL_IsRegTempP(ctx, reg))
				{ obj->regs[j]->regflags|=BGBCC_REGFL_INITIALIZED; }
		}

		if((vop->opn==CCXL_VOP_JMP) ||
			(vop->opn==CCXL_VOP_JCMP) ||
			(vop->opn==CCXL_VOP_JCMP_ZERO))
		{
			for(j=0; j<obj->n_vop; j++)
			{
				vop1=obj->vop[j];
				if(vop1->opn!=CCXL_VOP_LABEL)
					continue;
				if(vop->imm.ui!=vop1->imm.ui)
					continue;
				vop1->tgt_mult++;
				break;
			}
		}
	}
	
	if(!(sctx->is_simpass))
	{
		sctx->stat_funarg_exp[obj->n_cargs/2]++;
		sctx->stat_funarg_tot++;
	}

	k=0;
	for(i=0; i<obj->n_vop; i++)
	{
		vop=obj->vop[i];
		if(vop->opn==CCXL_VOP_LABEL)
		{
//			k=vop->tgt_mult;
			j=(vop->tgt_mult>>3);
			if(j<0)		j=0;
			if(j>3)		j=3;
//			k=vop->llvl;
			k=vop->llvl + j;
		}else
		{
			vop->tgt_mult=k;
		}
	}

	for(i=0; i<obj->n_regs; i++)
	{
		if((sctx->is_simpass&15)==1)
			obj->regs[i]->regflags&=~BGBCC_REGFL_CULL;
		obj->regs[i]->fxoffs=-1;
		obj->regs[i]->fxmoffs=-1;
	}

	for(i=0; i<obj->n_args; i++)
	{
		if((sctx->is_simpass&15)==1)
			obj->args[i]->regflags&=~BGBCC_REGFL_CULL;
		obj->args[i]->fxoffs=-1;
		obj->args[i]->fxmoffs=-1;
	}

	for(i=0; i<obj->n_locals; i++)
	{
		if((sctx->is_simpass&15)==1)
			obj->locals[i]->regflags&=~BGBCC_REGFL_CULL;
		obj->locals[i]->fxoffs=-1;
		obj->locals[i]->fxmoffs=-1;

		if(obj->locals[i]->regflags&BGBCC_REGFL_ACCESSED)
			{ obj->locals[i]->regflags&=~BGBCC_REGFL_CULL; }

//		if(obj->locals[i]->regflags&BGBCC_REGFL_ACCESSED)
//			{ obj->locals[i]->regflags&=~BGBCC_REGFL_CULL; }
//		else
//			{ obj->locals[i]->regflags|=BGBCC_REGFL_CULL; }
	}

#if 1

	obj->regflags&=~BGBCC_REGFL_HASLCLALIAS;

	tsz=0;
	for(i=0; i<obj->n_locals; i++)
	{
		tty=obj->locals[i]->type;

		if(	BGBCC_CCXL_TypeArrayP(ctx, tty) ||
			BGBCC_CCXL_TypeValueObjectP(ctx, tty))
		{
			k=BGBCC_CCXL_TypeGetLogicalSize(ctx, tty);
			tsz+=k;
		}
	}
	
	msz=16384;
	if(tsz>=16384)
	{
		k=tsz/12288;
		msz=16384/k;
	}

	for(i=0; i<obj->n_locals; i++)
	{
		if(obj->locals[i]->regflags&BGBCC_REGFL_CULL)
			continue;
		if(obj->locals[i]->regflags&BGBCC_REGFL_ALIASPTR)
		{
			tty=obj->locals[i]->type;

			if(BGBCC_CCXL_TypeArrayP(ctx, tty))
			{
				obj->regflags|=BGBCC_REGFL_HASARRAY;

				k=BGBCC_CCXL_TypeGetLogicalSize(ctx, tty);
//				al=BGBCC_CCXL_TypeGetLogicalAlign(ctx, tty);

//				if(k>=16384)
				if(k>=msz)
				{
					BGBCC_CCXL_TagWarn(ctx, CCXL_TERR_AUTOALLOCA);
					obj->regflags|=BGBCC_REGFL_ALLOCA;
					obj->locals[i]->regflags|=BGBCC_REGFL_ALLOCA;
				}
			}

			if(BGBCC_CCXL_TypeValueObjectP(ctx, tty))
			{
				k=BGBCC_CCXL_TypeGetLogicalSize(ctx, tty);
				if(k>=msz)
				{
					BGBCC_CCXL_TagWarn(ctx, CCXL_TERR_AUTOALLOCA);
					obj->regflags|=BGBCC_REGFL_ALLOCA;
					obj->locals[i]->regflags|=BGBCC_REGFL_ALLOCA;
				}
			}

			obj->regflags|=BGBCC_REGFL_HASLCLALIAS;

			continue;
		}
			
		tty=obj->locals[i]->type;

		if(BGBCC_CCXL_TypeFloat16P(ctx, tty) ||
			BGBCC_CCXL_TypeBFloat16P(ctx, tty))
		{
			tty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);
			obj->locals[i]->type=tty;
		}
		
		if(BGBCC_CCXL_TypeFloatP(ctx, tty) && sctx->fpu_gfp)
		{
			tty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
			obj->locals[i]->type=tty;
		}

		if(BGBCC_CCXL_TypeArrayP(ctx, tty))
		{
			obj->regflags|=BGBCC_REGFL_HASARRAY;

			k=BGBCC_CCXL_TypeGetLogicalSize(ctx, tty);
//			al=BGBCC_CCXL_TypeGetLogicalAlign(ctx, type);

//			if(k>=16384)
			if(k>=msz)
			{
				BGBCC_CCXL_TagWarn(ctx, CCXL_TERR_AUTOALLOCA);
				obj->regflags|=BGBCC_REGFL_ALLOCA;
				obj->locals[i]->regflags|=BGBCC_REGFL_ALLOCA;
			}
		}

		if(BGBCC_CCXL_TypeValueObjectP(ctx, tty))
		{
			k=BGBCC_CCXL_TypeGetLogicalSize(ctx, tty);
			if(k>=msz)
			{
				BGBCC_CCXL_TagWarn(ctx, CCXL_TERR_AUTOALLOCA);
				obj->regflags|=BGBCC_REGFL_ALLOCA;
				obj->locals[i]->regflags|=BGBCC_REGFL_ALLOCA;
			}
		}

		if(BGBCC_CCXL_TypeVec128P(ctx, tty))
		{
			obj->regflags|=BGBCC_REGFL_HAS128;
			sctx->use_egpr|=1;
		}

		if(obj->locals[i]->flagsint&BGBCC_TYFL_REGISTER)
		{
			sctx->use_egpr|=1;
			obj->regflags|=BGBCC_REGFL_GOFAST;
		}
	}

#if 1
	for(i=0; i<obj->n_regs; i++)
	{
		if(obj->regs[i]->regflags&BGBCC_REGFL_CULL)
			continue;
		if(obj->regs[i]->regflags&BGBCC_REGFL_ALIASPTR)
		{
			obj->regflags|=BGBCC_REGFL_HASLCLALIAS;
			continue;
		}
			
		tty=obj->regs[i]->type;

		if(BGBCC_CCXL_TypeFloat16P(ctx, tty) ||
			BGBCC_CCXL_TypeBFloat16P(ctx, tty))
		{
			tty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);
			obj->regs[i]->type=tty;
		}
		
#if 1
		if(BGBCC_CCXL_TypeFloatP(ctx, tty) && sctx->fpu_gfp)
		{
			tty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
			obj->regs[i]->type=tty;
		}
#endif

//		if(BGBCC_CCXL_TypeArrayP(ctx, tty))
//		{
//			obj->regflags|=BGBCC_REGFL_HASARRAY;
//		}
	}
#endif

#if 1
	ni=0;
	for(i=0; i<obj->n_args; i++)
	{
		if(obj->args[i]->regflags&BGBCC_REGFL_CULL)
			continue;
		if(obj->args[i]->regflags&BGBCC_REGFL_ALIASPTR)
		{
			obj->regflags|=BGBCC_REGFL_HASLCLALIAS;
			continue;
		}
			
		tty=obj->args[i]->type;

		if(BGBCC_CCXL_TypeFloat16P(ctx, tty) ||
			BGBCC_CCXL_TypeBFloat16P(ctx, tty))
		{
			tty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);
			obj->args[i]->type=tty;
		}
		
		if(BGBCC_CCXL_TypeFloatP(ctx, tty) && sctx->fpu_gfp)
		{
			tty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
			obj->args[i]->type=tty;
		}
		
		rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->args[i]->type);
		switch(rcls)
		{
			case BGBCC_SH_REGCLS_VO_QGR2:
			case BGBCC_SH_REGCLS_QGR2:
			case BGBCC_SH_REGCLS_VO_REF2:
			case BGBCC_SH_REGCLS_AR_REF2:
				if(ni&1)ni++;
				if(ni>=8)
				{
					/* Argument comes from stack. */
					obj->args[i]->regflags|=BGBCC_REGFL_TEMPLOAD;
				}
				ni+=2;
				break;

			default:
				if(ni>=8)
				{
					/* Argument comes from stack. */
					obj->args[i]->regflags|=BGBCC_REGFL_TEMPLOAD;
				}
				ni++;
				break;
		}
	}
	if(ni>8)
	{
		/* Function may access stack. */
		obj->regflags|=BGBCC_REGFL_TEMPLOAD;

		/* Disallow "tiny leaf" special case. */
		obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;
	}
#endif

#endif

	obj->regflags&=~BGBCC_REGFL_HASGBLALIAS;

	sctx->vsp_tcnt=0;
	trn=0;
	BGBCC_JX2C_BeginSetupFrameVRegSpan(ctx, sctx);
	nogblstore=1;
	ftgblstore=1;

	for(i=0; i<obj->n_vop; i++)
	{
		vop=obj->vop[i];

		if(vop->opn==CCXL_VOP_OBJCALL)
		{
			BGBCC_JX2C_InvalidateGlobalSetMask(ctx, sctx, obj);
			obj->regflags|=BGBCC_REGFL_NOTLEAF;
			obj->regflags|=BGBCC_REGFL_REC_GBLSTORE;
			nogblstore=0;
			ftgblstore=3;
		}

		if(vop->opn==CCXL_VOP_CALL)
		{
			ctx->cur_func->regflags|=BGBCC_REGFL_NOTLEAF;
			
			if(vop->imm.call.na>8)
			{
				ctx->cur_func->regflags|=BGBCC_REGFL_CALLVARARG;
			}
			
			if(BGBCC_CCXL_IsRegGlobalFunctionVarArgP(ctx, vop->srca))
			{
				ctx->cur_func->regflags|=BGBCC_REGFL_CALLVARARG;
			}
			
			if(BGBCC_CCXL_IsRegGlobalFunctionP(ctx, vop->srca))
			{
				if(BGBCC_CCXL_GetRegRegisterInfo(ctx, vop->srca)!=obj)
//				if(1)
				{
					regfl=BGBCC_CCXL_GetRegFlags(ctx, reg);
					if(	(regfl&BGBCC_REGFL_GBLSTORE) ||
						(regfl&BGBCC_REGFL_HASGBLALIAS) ||
						(regfl&BGBCC_REGFL_REC_GBLSTORE))
					{
						ctx->cur_func->regflags|=BGBCC_REGFL_REC_GBLSTORE;
						nogblstore=0;
						
						BGBCC_JX2C_MergeGlobalSetMask(ctx, sctx, obj,
							BGBCC_CCXL_GetRegRegisterInfo(ctx, vop->srca));
					}else if(!(regfl&BGBCC_REGFL_REC_NOGBLSTORE))
					{
						nogblstore=0;
					}

					if(!(regfl&(
						BGBCC_REGFL_REC_FTGBLSTORE|
						BGBCC_REGFL_REC_NOGBLSTORE)))
					{
						if(!(ftgblstore&2))
							ftgblstore=0;
					}
				}
			}else
			{
				/* Be cautious. */
				BGBCC_JX2C_InvalidateGlobalSetMask(ctx, sctx, obj);
				ctx->cur_func->regflags|=BGBCC_REGFL_REC_GBLSTORE;
				nogblstore=0;

				BGBCC_JX2C_InvalidateGlobalSetMask(ctx, sctx, obj);
				ftgblstore=3;
			}
		}
		
		if(BGBCC_CCXL_IsRegGlobalP(ctx, vop->dst))
		{
			ctx->cur_func->regflags|=BGBCC_REGFL_GBLSTORE;
			ctx->cur_func->regflags|=BGBCC_REGFL_REC_GBLSTORE;
			nogblstore=0;

			BGBCC_JX2C_MarkGlobalSetMask(ctx, sctx, obj, vop->dst);

			if(BGBCC_CCXL_IsRegAliasedP(ctx, vop->dst))
				obj->regflags|=BGBCC_REGFL_HASGBLALIAS;
		}

		if(BGBCC_CCXL_IsRegGlobalP(ctx, vop->srca))
		{
			if(BGBCC_CCXL_IsRegAliasedP(ctx, vop->srca))
				obj->regflags|=BGBCC_REGFL_HASGBLALIAS;
		}

		if(BGBCC_CCXL_IsRegGlobalP(ctx, vop->srcb))
		{
			if(BGBCC_CCXL_IsRegAliasedP(ctx, vop->srcb))
				obj->regflags|=BGBCC_REGFL_HASGBLALIAS;
		}

		if(vop->opn==CCXL_VOP_DBGFN)
			{ ctx->lfn=vop->imm.str; }
		if(vop->opn==CCXL_VOP_DBGLN)
			{ ctx->lln=vop->imm.si; }

		if(!sctx->is_simpass)
		{
			if(BGBCC_CCXL_IsRegLocalP(ctx, vop->dst))
			{
				j=vop->dst.val&4095;
				localmap[j>>5]|=1U<<(j&31);
			}

			if(BGBCC_CCXL_IsRegLocalP(ctx, vop->srca))
			{
				j=vop->srca.val&4095;
				if(!(localmap[j>>5]&(1U<<(j&31))))
				{
					if(!BGBCC_CCXL_TypeArrayP(ctx,
						ctx->cur_func->locals[j]->type))
					{
						printf("%s:%d uninitialized local %s\n",
							ctx->lfn, ctx->lln, ctx->cur_func->locals[j]->name);
					}
					localmap[j>>5]|=1U<<(j&31);
				}
			}

			if(BGBCC_CCXL_IsRegLocalP(ctx, vop->srcb))
			{
				j=vop->srcb.val&4095;
				if(!(localmap[j>>5]&(1U<<(j&31))))
				{
					if(!BGBCC_CCXL_TypeArrayP(ctx,
						ctx->cur_func->locals[j]->type))
					{
						printf("%s:%d uninitialized local %s\n",
							ctx->lfn, ctx->lln, ctx->cur_func->locals[j]->name);
					}
					localmap[j>>5]|=1U<<(j&31);
				}
			}
		}
	}
	
	regfl=ctx->cur_func->regflags;

	if(!(regfl&BGBCC_REGFL_NOTLEAF))
		nogblstore=1;

	if(	(regfl&BGBCC_REGFL_GBLSTORE) ||
		(regfl&BGBCC_REGFL_HASGBLALIAS) ||
		(regfl&BGBCC_REGFL_ALLOCA) ||
//		(regfl&BGBCC_REGFL_HASSTIX) ||
		(regfl&BGBCC_REGFL_REC_GBLSTORE))
	{
		nogblstore=0;
		ctx->cur_func->regflags|=BGBCC_REGFL_REC_GBLSTORE;
	}

	if(nogblstore)
	{
		/* Entire call graph is known clean WRT global variables. */
		ctx->cur_func->regflags|=BGBCC_REGFL_REC_NOGBLSTORE;
	}

	if(ftgblstore)
	{
		/* This part of the graph is fully resolved. */
		ctx->cur_func->regflags|=BGBCC_REGFL_REC_FTGBLSTORE;
	}
	
	if(obj->qname[0]=='m')
	{
		if(	!strcmp(obj->qname, "memcpy") ||
			!strcmp(obj->qname, "memset") ||
			!strcmp(obj->qname, "memmove"))
		{
//			ctx->cur_func->regflags|=BGBCC_REGFL_GBLSTORE;
//			ctx->cur_func->regflags|=BGBCC_REGFL_REC_GBLSTORE;

			/* pretend these don't touch anything */
			ctx->cur_func->regflags&=~BGBCC_REGFL_REC_GBLSTORE;
			ctx->cur_func->regflags|=BGBCC_REGFL_REC_NOGBLSTORE;
		}
	}

	if(obj->qname[0]=='t')
	{
		if(	!strcmp(obj->qname, "tk_syscall") ||
			!strcmp(obj->qname, "tk_sprintf") ||
			!strcmp(obj->qname, "tk_printf"))
		{
			/* pretend these don't touch anything */
			ctx->cur_func->regflags&=~BGBCC_REGFL_REC_GBLSTORE;
			ctx->cur_func->regflags|=BGBCC_REGFL_REC_NOGBLSTORE;
		}
	}
	
#if 0
	regfl=ctx->cur_func->regflags;

	if(!sctx->is_simpass)
	{
		if(!(regfl&(BGBCC_REGFL_REC_GBLSTORE|BGBCC_REGFL_REC_NOGBLSTORE)))
		{
			printf("DBG: Not Rec GBLSTORE or NOGBLSTORE %s\n", obj->qname);
		}else if(regfl&BGBCC_REGFL_REC_GBLSTORE)
		{
			if(regfl&BGBCC_REGFL_REC_GBLSTORE)
			{
				printf("DBG: FTGBLSTORE  %s\n", obj->qname);
			}else
			{
				printf("DBG: GBLSTORE    %s\n", obj->qname);
			}
		}else if(regfl&BGBCC_REGFL_REC_NOGBLSTORE)
		{
			printf("DBG: NOGBLSTORE  %s\n", obj->qname);
		}
	}
#endif
	

	for(i=0; i<obj->n_vop; i++)
	{
		vtr=obj->vtr[trn];
		vop=obj->vop[i];

		while(i>=(vtr->b_ops+vtr->n_ops) && (trn<obj->n_vtr))
		{
			trn++;
			vtr=obj->vtr[trn];
		}
		
		sctx->tr_trnum=trn;
		sctx->tr_opnum=i;
		
		BGBCC_JX2C_SetupFrameVRegSpan(ctx, sctx, vop->dst, 1, vop->tgt_mult);

		if(vop->opn==CCXL_VOP_MOV)
		{
			reg1=vop->srca;
			BGBCC_JX2C_NormalizeImmVRegInt(ctx, sctx, vop->type, &reg1);
			if(reg1.val!=vop->srca.val)
			{
				BGBCC_JX2C_SetupFrameVRegSpan(ctx, sctx,
					reg1, 0, vop->tgt_mult);
			}
		}

		if(	(vop->opn==CCXL_VOP_BINARY) ||
			(vop->opn==CCXL_VOP_COMPARE) ||
			(vop->opn==CCXL_VOP_JCMP))
		{
			reg1=vop->srcb;
			BGBCC_JX2C_NormalizeImmVRegInt(ctx, sctx, vop->type, &reg1);
			if(reg1.val!=vop->srcb.val)
			{
				BGBCC_JX2C_SetupFrameVRegSpan(ctx, sctx,
					reg1, 0, vop->tgt_mult);
			}
		}

		if(	(vop->opn!=CCXL_VOP_CALL) &&
			(vop->opn!=CCXL_VOP_CALL_INTRIN))
//		if(1)
		{
			BGBCC_JX2C_SetupFrameVRegSpan(ctx, sctx,
				vop->srca, 0, vop->tgt_mult);
		}else
		{
			if(!BGBCC_CCXL_IsRegGlobalFunctionP(ctx, vop->srca))
			{
				BGBCC_JX2C_SetupFrameVRegSpan(ctx, sctx,
					vop->srca, 0, vop->tgt_mult);
			}
		}

		BGBCC_JX2C_SetupFrameVRegSpan(ctx, sctx, vop->srcb, 0, vop->tgt_mult);

		if(vop->opn==CCXL_VOP_CALL)
		{
			fname=NULL;
			noarg=0;
			if((vop->srca.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_GLOBAL)
			{
				j=(int)(vop->srca.val&CCXL_REGID_REGMASK);
				fname=ctx->reg_globals[j]->name;
				
				if(fname && !strcmp(fname, "__alloca"))
					{ obj->regflags|=BGBCC_REGFL_ALLOCA; }
					
				noarg=BGBCC_JX2C_CheckCallPossibleBuiltin(ctx, sctx, fname);
			}
			
			for(j=0; j<vop->imm.call.na; j++)
			{
				reg1=vop->imm.call.args[j];
				tty=BGBCC_CCXL_GetRegType(ctx, reg1);
				
				if(	BGBCC_CCXL_TypeVecP(ctx, tty) ||
					BGBCC_CCXL_TypeVec128P(ctx, tty) ||
					BGBCC_CCXL_TypeQuadPointerP(ctx, tty) ||
					BGBCC_CCXL_TypeValueObjectP(ctx, tty))
					noarg=1;
			}

			for(j=0; j<vop->imm.call.na; j++)
			{
				k=2|(j<<16);
				k|=(((i*31)+i)&0x3F)<<(16+6);	//key to 3AC op
				if(noarg)
					k=0;
				BGBCC_JX2C_SetupFrameVRegSpan(ctx, sctx,
					vop->imm.call.args[j], k, vop->tgt_mult);
			}
		}
	}
	BGBCC_JX2C_EndSetupFrameVRegSpan(ctx, sctx);

	if(!strcmp(obj->name, "TKRA_ProjectVertex"))
	{
//		sctx->frm_offs_retstr=k-8;
		k=-1;
//		BGBCC_DBGBREAK
	}

	
	if(!(sctx->use_egpr&4))
		sctx->use_egpr=0;
	if(sctx->has_bjx1egpr)
	{
		k=64;

		if(ctx->optmode==BGBCC_OPT_SPEED2)
			k=16;

		if(ctx->optmode==BGBCC_OPT_SPEED)
//			k=32;
//			k=24;
			k=20;
//			k=16;
//			k=4;
		if(ctx->optmode==BGBCC_OPT_SIZE)
			k=96;

#if 1
//		if(sctx->vspan_num>32)
//		if(sctx->vspan_num>48)
//		if(sctx->vspan_num>56)
//		if(sctx->vspan_num>64)
//		if(sctx->vspan_num>68)

//		if(sctx->vspan_num>72)

//		if(sctx->vspan_num>80)
//		if(sctx->vspan_num>88)
//		if(sctx->vspan_num>96)
//		if(sctx->vspan_num>128)

		/* High register pressure. */
		if(sctx->vspan_num>k)
		{
			sctx->use_egpr|=1;
		}

		if(sctx->vspan_num>(2*k))
//		if(sctx->vspan_num>(3*k))
		{
			sctx->use_egpr|=2;
		}

#if 1
		if(ctx->optmode==BGBCC_OPT_SPEED2)
		{
//			if(sctx->vspan_num>=24)
//			if(sctx->vspan_num>=36)
			if(sctx->vspan_num>=k)
				sctx->use_egpr|=2;
		}
#endif

		/* Try for full static assignment. */
		if(sctx->vspan_num<12)
			sctx->use_egpr|=1;
		
//		if(sctx->is_mergece)
//			sctx->use_egpr=0;
#endif

//		sctx->use_egpr=1;
	}

	if(!sctx->is_simpass)
	{
		i=sctx->vspan_num>>2;
		if(i>63)i=63;
		sctx->stat_varcount_exp[i]++;
		sctx->stat_varcount_tot++;

		if(sctx->vspan_num>64)
		{
//			printf("large-var-count: %s: %d\n", obj->name, sctx->vspan_num);
		}
	}

	if(ctx->optmode==BGBCC_OPT_SPEED2)
		sctx->use_egpr|=1;

	if(sctx->is_fixed32)
		sctx->use_egpr|=1;
		
	if(sctx->has_xgpr&2)
		sctx->use_egpr|=1;

//	if((sctx->vspan_num>12) && (sctx->vspan_num<24))
//		sctx->use_egpr|=2;

//	sctx->use_egpr|=3;

	if(!sctx->use_egpr)
	{
		for(i=0; i<obj->n_locals; i++)
		{
			if(obj->locals[i]->regflags&BGBCC_REGFL_CULL)
				continue;

			if(obj->locals[i]->flagsint&BGBCC_TYFL_REGISTER)
			{
				sctx->use_egpr|=1;
//				sctx->use_egpr|=3;
				obj->regflags|=BGBCC_REGFL_GOFAST;
//				continue;
			}
		
			rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->locals[i]->type);
			switch(rcls)
			{
			case BGBCC_SH_REGCLS_VO_QGR:
			case BGBCC_SH_REGCLS_VO_QGR2:
			case BGBCC_SH_REGCLS_QGR:
			case BGBCC_SH_REGCLS_QGR2:
			case BGBCC_SH_REGCLS_WGR:
			case BGBCC_SH_REGCLS_VO_REF2:
			case BGBCC_SH_REGCLS_AR_REF2:
				sctx->use_egpr|=1;
				break;
			}
		}
		
		for(i=0; i<obj->n_args; i++)
		{
			rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->args[i]->type);
			switch(rcls)
			{
			case BGBCC_SH_REGCLS_VO_QGR:
			case BGBCC_SH_REGCLS_VO_QGR2:
			case BGBCC_SH_REGCLS_QGR:
			case BGBCC_SH_REGCLS_QGR2:
			case BGBCC_SH_REGCLS_WGR:
			case BGBCC_SH_REGCLS_VO_REF2:
			case BGBCC_SH_REGCLS_AR_REF2:
				sctx->use_egpr|=1;
				break;
			}
		}
		
		for(i=0; i<obj->n_regs; i++)
		{
			rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->regs[i]->type);
			switch(rcls)
			{
			case BGBCC_SH_REGCLS_VO_QGR:
			case BGBCC_SH_REGCLS_VO_QGR2:
			case BGBCC_SH_REGCLS_QGR:
			case BGBCC_SH_REGCLS_QGR2:
			case BGBCC_SH_REGCLS_WGR:
			case BGBCC_SH_REGCLS_VO_REF2:
			case BGBCC_SH_REGCLS_AR_REF2:
				sctx->use_egpr|=1;
				break;
			}
		}
	}
	
	if(obj->regflags&BGBCC_REGFL_GOFAST)
		sctx->use_egpr|=2;

//	sctx->use_egpr|=2;

	if(!(sctx->has_xgpr&1))
		sctx->use_egpr&=~2;

	if(!(sctx->has_xgpr&6))
		sctx->use_egpr&=~2;

//	sctx->use_egpr&=~2;

	if(!strcmp(obj->name, "TKRA_MatrixIdentify"))
	{
		sctx->frm_offs_retstr=k-8;
//		BGBCC_DBGBREAK
	}

//	sctx->use_egpr=0;

	ni=0; nf=0;
	k=0; ka=0; kf=0;
	k-=2*8;		//saved PR, R14
//	k-=6*4;		//saved R8/9/10/11/13, R2
	k-=8*8;		//saved R8/9/10/11/13, R2

//	if(sctx->has_bjx1egpr)
	if(sctx->has_bjx1egpr && sctx->use_egpr)
		k-=8*8;		//saved R24..R31

//	if(sctx->is_pbo)
	if((sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR)) ||
		(obj->flagsint&BGBCC_TYFL_INTERRUPT))
		k-=8;		//saved GBR

//	if(sctx->has_xgpr&2)
//	if((sctx->has_xgpr&2) ||
//		((sctx->has_xgpr&1) && (sctx->use_egpr&2)))
//	if((sctx->has_xgpr&2) || (sctx->use_egpr&2))
//	if((sctx->has_xgpr&1) || (sctx->use_egpr&2))
	if((sctx->has_xgpr&6) || (sctx->use_egpr&2))
		k-=16*8;		//saved R40..R47, R56..R63

	if(obj->flagsint&BGBCC_TYFL_INTERRUPT)
	{
		k-=2*8;		//saved SPC, EXSR

#if 0
		k-=8*8;		//saved R0..R7
		if(sctx->has_bjx1egpr)
			k-=8*8;		//saved R16..R23

		if(sctx->has_xgpr&2)
			k-=16*8;		//saved R32..R39,R48..R55
#endif

#if 1
		k-=16*8;		//saved R0..R15
		if(sctx->has_bjx1egpr)
			k-=16*8;		//saved R16..R31

//		if(sctx->has_xgpr&2)
		if(sctx->has_xgpr&1)
			k-=32*8;		//saved R32..R64
#endif
	}

//	if(sctx->is_addr64)
//		k*=2;
	
	sctx->frm_offs_save_rsv=k;
	
	if((obj->flagsint&BGBCC_TYFL_VIRTUAL) ||
		obj->thisstr)
	{
		if(sctx->has_xgpr&2)
			{ k-=16; k&=~15; }
		else
			{ k-=8; k&=~7; }

//		else if(sctx->is_addr64)
//			{ k-=8; k&=~7; }
//		else
//			{ k-=4; k&=~3; }
		sctx->frm_offs_thisptr=k;
	}
	
//	if(obj->regflags&BGBCC_REGFL_HASARRAY)
//	if((obj->regflags&BGBCC_REGFL_HASARRAY) &&
//		!(obj->regflags&BGBCC_REGFL_GOFAST))
	if(	(obj->regflags&BGBCC_REGFL_HASARRAY) ||
		(sctx->abi_spillpad&8))
//	if(1)
	{
		i=(nlint)obj;
		i=(u16)((i*65521)>>16);
//		k-=8; k&=~7;
		k-=16; k&=~15;
		sctx->frm_offs_sectoken=k;
		sctx->frm_val_sectoken=i;
		k-=16; k&=~15;
	}else
	{
		sctx->frm_offs_sectoken=0;
		k-=16; k&=~15;
	}

	if(sctx->abi_spillpad&64)
	{
		sctx->frm_offs_sectoken=0;
		sctx->frm_val_sectoken=0;
	}

	if((obj->regflags&BGBCC_REGFL_ALLOCA))
	{
//		if(sctx->has_xgpr&2)
		if(ctx->arch_sizeof_ptr==16)
			{ k-=16; k&=~15; }
		else
			{ k-=8; k&=~7; }
		sctx->frm_offs_allocamrk=k;
	}else
	{
		sctx->frm_offs_allocamrk=0;
	}

	sctx->frm_offs_datahi=k;

//	if(obj->regflags&BGBCC_REGFL_HASARRAY)
//		k-=16;

	for(i=0; i<obj->n_locals; i++)
	{
		if(obj->locals[i]->regflags&BGBCC_REGFL_CULL)
			continue;
	
		rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->locals[i]->type);
		
		obj->locals[i]->regcls=rcls;
		switch(rcls)
		{
//		case BGBCC_SH_REGCLS_FR:	case BGBCC_SH_REGCLS_FR2:
//		case BGBCC_SH_REGCLS_DR:
//			sctx->use_fpr=1;	break;
		case BGBCC_SH_REGCLS_VO_REF:
		case BGBCC_SH_REGCLS_AR_REF:
		case BGBCC_SH_REGCLS_VO_REF2:
		case BGBCC_SH_REGCLS_AR_REF2:

			/* Don't reserve stack space if folded to alloca... */
			if(obj->locals[i]->regflags&BGBCC_REGFL_ALLOCA)
				break;

			kf=(kf+15)&(~15);

//			obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;

//			obj->locals[i]->fxmoffs=kf;
			j=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->locals[i]->type);

			obj->locals[i]->fxmoffs=
				BGBCC_JX2C_CheckPadOffsetToBndTag16(
					ctx, sctx, kf, j);

			j=BGBCC_JX2C_CheckPadToBndTag16(ctx, sctx, j);
			if(!j)
				{ BGBCC_DBGBREAK }

			if(j>BGBCC_MAXSTACKOBJ)
				{ BGBCC_DBGBREAK }

//			j=(j+15)&(~15);
			j=(j+23)&(~15);
//			j=(j+31)&(~15);

//			if(sctx->has_xgpr&2)
//				{ j=(j+15)&(~15); }
//			else if(sctx->is_addr64)
//				{ j=(j+7)&(~7); }
//			else
//				{ j=(j+3)&(~3); }
			kf+=j;
			break;

		case BGBCC_SH_REGCLS_GR:
		case BGBCC_SH_REGCLS_GR2:
		case BGBCC_SH_REGCLS_VO_GR:
		case BGBCC_SH_REGCLS_VO_GR2:
		case BGBCC_SH_REGCLS_VO_QGR:
		case BGBCC_SH_REGCLS_VO_QGR2:
		case BGBCC_SH_REGCLS_QGR:
		case BGBCC_SH_REGCLS_QGR2:
		case BGBCC_SH_REGCLS_WGR:
			break;

		default:
			BGBCC_DBGBREAK
			break;
		}
	}

	for(i=0; i<obj->n_regs; i++)
	{
		rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->regs[i]->type);
//		rcls=BGBCC_SHXC_TypeGetRegClassP(ctx, 
//			BGBCC_CCXL_TypeWrapBasicType(obj->regs[i]->type_zb));
		obj->regs[i]->regcls=rcls;

		switch(rcls)
		{
		case BGBCC_SH_REGCLS_FR:	case BGBCC_SH_REGCLS_FR2:
		case BGBCC_SH_REGCLS_DR:
			sctx->use_fpr=1;	break;
		case BGBCC_SH_REGCLS_VO_REF:
		case BGBCC_SH_REGCLS_AR_REF:
		case BGBCC_SH_REGCLS_VO_REF2:
		case BGBCC_SH_REGCLS_AR_REF2:

			if(!(obj->regs[i]->regflags&BGBCC_REGFL_INITIALIZED))
				break;

			kf=(kf+15)&(~15);

			obj->regs[i]->fxmoffs=kf;
			j=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->regs[i]->type);

//			j=(j+15)&(~15);
			j=(j+23)&(~15);

//			if(sctx->has_xgpr&2)
//				{ j=(j+15)&(~15); }
//			else if(sctx->is_addr64)
//				{ j=(j+7)&(~7); }
//			else
//				{ j=(j+3)&(~3); }

			if(j>BGBCC_MAXSTACKOBJ)
				{ BGBCC_DBGBREAK }

			kf+=j;
			break;

//		case BGBCC_SH_REGCLS_AR_REF:
//			break;

		case BGBCC_SH_REGCLS_GR:
		case BGBCC_SH_REGCLS_GR2:
		case BGBCC_SH_REGCLS_VO_GR:
		case BGBCC_SH_REGCLS_VO_GR2:
		case BGBCC_SH_REGCLS_VO_QGR:
		case BGBCC_SH_REGCLS_VO_QGR2:
		case BGBCC_SH_REGCLS_QGR:
		case BGBCC_SH_REGCLS_QGR2:
		case BGBCC_SH_REGCLS_WGR:
			break;

		default:
			BGBCC_DBGBREAK
			break;
		}

#if 0
		if(BGBCC_CCXL_TypeArrayP(ctx, obj->regs[i]->type))
		{
//			k-=4; obj->regs[i]->fxoffs=k;
			obj->regs[i]->fxmoffs=kf;
			j=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->regs[i]->type);
			j=(j+3)&(~3);
			kf+=j;
		}

		if(	(rcls==BGBCC_SH_REGCLS_VO_REF) ||
			(rcls==BGBCC_SH_REGCLS_VO_REF2))
		{
//			k-=4; obj->regs[i]->fxoffs=k;
			obj->regs[i]->fxmoffs=kf;
			j=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->regs[i]->type);

			if(j>1024)
				{ BGBCC_DBGBREAK }

			j=(j+3)&(~3);
			kf+=j;
		}
#endif
	}

	for(i=0; i<obj->n_args; i++)
	{
		if(BGBCC_CCXL_TypeVarArgsP(ctx, obj->args[i]->type))
			{ sctx->is_vararg=1; }

		rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->args[i]->type);
		if(rcls==BGBCC_SH_REGCLS_WGR)
			rcls=BGBCC_SH_REGCLS_GR;

		obj->args[i]->regcls=rcls;

		switch(rcls)
		{
		case BGBCC_SH_REGCLS_FR:
		case BGBCC_SH_REGCLS_FR2:
		case BGBCC_SH_REGCLS_DR:
			sctx->use_fpr=1;	break;
		case BGBCC_SH_REGCLS_GR:
		case BGBCC_SH_REGCLS_GR2:
		case BGBCC_SH_REGCLS_VO_GR:
		case BGBCC_SH_REGCLS_VO_GR2:
		case BGBCC_SH_REGCLS_VO_QGR2:
		case BGBCC_SH_REGCLS_WGR:
			break;
		case BGBCC_SH_REGCLS_VO_REF:
		case BGBCC_SH_REGCLS_AR_REF:
		case BGBCC_SH_REGCLS_VO_QGR:
		case BGBCC_SH_REGCLS_QGR:
		case BGBCC_SH_REGCLS_QGR2:
		case BGBCC_SH_REGCLS_VO_REF2:
		case BGBCC_SH_REGCLS_AR_REF2:
			break;
		default:
			BGBCC_DBGBREAK
			break;
		}

//		if(BGBCC_CCXL_TypeValueObjectP(ctx, obj->args[i]->type) &&
//			(rcls==BGBCC_SH_REGCLS_VO_REF))
		if(	(rcls==BGBCC_SH_REGCLS_VO_REF) ||
			(rcls==BGBCC_SH_REGCLS_VO_REF2))
		{
			kf=(kf+15)&(~15);

//			k-=4; obj->args[i]->fxoffs=k;
			obj->args[i]->fxmoffs=kf;
			j=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->args[i]->type);
			
			if(j>BGBCC_MAXSTACKOBJ)
				{ BGBCC_DBGBREAK }
			
//			j=(j+3)&(~3);
//			if(sctx->has_xgpr&2)
			if(ctx->arch_sizeof_ptr==16)
				{ j=(j+15)&(~15); }
			else 
				{ j=(j+7)&(~7); }
			kf+=j;
		}
	}

//	if(sctx->use_fpr)
//		{ k-=4*8; }		//saved FR12/13/14/15
	if(sctx->use_fpr)
		{ k-=8*8; }		//saved FR12/13/14/15

//	if(!strcmp(obj->name, "GetWavinfo"))
//	if(!strcmp(obj->name, "TKRA_MatrixIdentify"))
//	if(!strcmp(obj->qname, "TKRA_MatrixIdentify"))
//	{
//		sctx->frm_offs_retstr=k-8;
//		BGBCC_DBGBREAK
//	}

	k=k&(~15);
	kf=(kf+15)&(~15);

	k-=kf;
	sctx->frm_offs_fix=k;

	sctx->frm_offs_datalo=k;

	tty=BGBCC_CCXL_GetTypeReturnType(ctx, obj->type);
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, tty);

//	if(BGBCC_CCXL_TypeValueObjectP(ctx, tty))
	if(rcls==BGBCC_SH_REGCLS_VO_REF)
	{
//		k-=4;
		k-=8;
		sctx->frm_offs_retstr=k;
		obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;
	}

	if(rcls==BGBCC_SH_REGCLS_VO_REF2)
	{
		k-=16;
		sctx->frm_offs_retstr=k;
		obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;
	}

	if((-k)>=BGBCC_MAXSTACKFRAME)
		{ BGBCC_DBGBREAK }

#if 1
	if(sctx->abi_spillpad&1)
	{
		if(sctx->has_xgpr&2)
			{ ka+=16*8; }
		else
			{ ka+=8*8; }
	}

	for(i=0; i<obj->n_args; i++)
	{
		rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->args[i]->type);
		if(rcls==BGBCC_SH_REGCLS_WGR)
			rcls=BGBCC_SH_REGCLS_GR;

		switch(rcls)
		{
		case BGBCC_SH_REGCLS_GR:
		case BGBCC_SH_REGCLS_VO_GR:
		case BGBCC_SH_REGCLS_WGR:
			if(sctx->has_xgpr&2)
			{
				if(ni<16)
				{
					if(sctx->abi_spillpad&1)
					{
						obj->args[i]->fxoffs=ni*8; ni++;
					}else
					{
						k&=~3; k-=4; obj->args[i]->fxoffs=k; ni++;
					}
				}
				else
				{
					if(sctx->abi_spillpad&1)
					{
						ka=(ka+7)&(~7);
						obj->args[i]->fxoffs=ka;
	//					ka+=4;
						ka+=8;
					}else
					{
						obj->args[i]->fxoffs=ka;
						ka+=4;
					}
				}
				break;
			}

//			if(sctx->is_addr64)
//			if(sctx->has_bjx1egpr)
			if(1)
			{
				if(ni<8)
				{
					if(sctx->abi_spillpad&1)
					{
						obj->args[i]->fxoffs=ni*8; ni++;
					}else
					{
						k&=~3; k-=4; obj->args[i]->fxoffs=k; ni++;
					}
				}
				else
				{
					if(sctx->abi_spillpad&1)
					{
						ka=(ka+7)&(~7);
						obj->args[i]->fxoffs=ka;
	//					ka+=4;
						ka+=8;
					}else
					{
						if(sctx->emit_riscv&3)
						{
							ka=(ka+7)&(~7);
							obj->args[i]->fxoffs=ka;
							ka+=8;
						}
						else
						{
							obj->args[i]->fxoffs=ka;
							ka+=4;
						}
					}
				}
				break;
			}
		
//			if(ni<4)	{ k&=~3; k-=4; obj->args[i]->fxoffs=k; ni++; }
//			else		{ obj->args[i]->fxoffs=ka; ka+=4; }
			break;

		case BGBCC_SH_REGCLS_VO_REF:
		case BGBCC_SH_REGCLS_AR_REF:
		case BGBCC_SH_REGCLS_VO_QGR:
		case BGBCC_SH_REGCLS_QGR:
//			if(sctx->is_addr64)
			if(1)
			{
				if(	(ni<4) || ((ni<8) && sctx->has_bjx1egpr) ||
					((ni<16) && (sctx->has_xgpr&2)))
				{
					if(sctx->abi_spillpad&1)
					{
						obj->args[i]->fxoffs=ni*8; ni++;
					}else
					{
						k&=(~7); k-=8;
						obj->args[i]->fxoffs=k; ni++;
					}
				}
				else
				{
					ka=(ka+7)&(~7);
					obj->args[i]->fxoffs=ka;
					ka+=8;
				}
				break;
			}

//			if(ni<4)	{ k&=~3; k-=4; obj->args[i]->fxoffs=k; ni++; }
//			else		{ obj->args[i]->fxoffs=ka; ka+=4; }
			break;

		case BGBCC_SH_REGCLS_GR2:
		case BGBCC_SH_REGCLS_QGR2:
		case BGBCC_SH_REGCLS_VO_GR2:
		case BGBCC_SH_REGCLS_VO_QGR2:
		case BGBCC_SH_REGCLS_VO_REF2:
		case BGBCC_SH_REGCLS_AR_REF2:
			if(sctx->abi_evenonly)
				ni=(ni+1)&(~1);
		
//			if(sctx->is_addr64)
			if(1)
			{
//				if(ni<7)
				if((ni<3) || ((ni<7) && sctx->has_bjx1egpr) ||
					((ni<15) && (sctx->has_xgpr&2)))
				{
//					k&=~7; k-=16;
					if(sctx->abi_spillpad&1)
					{
						obj->args[i]->fxoffs=ni*8; ni+=2;
					}else
					{
						k&=~15; k-=16;
						obj->args[i]->fxoffs=k; ni+=2;
					}
				}
				else
				{
//					ka=(ka+7)&(~7);
					ka=(ka+15)&(~15);
					obj->args[i]->fxoffs=ka; ka+=16;
				}
				break;
			}
//			if(ni<3)	{ k&=~3; k-=8; obj->args[i]->fxoffs=k; ni+=2; }
//			else		{ obj->args[i]->fxoffs=ka; ka+=8; }
			break;

#if 0
		case BGBCC_SH_REGCLS_FR:
			if(nf<4)	{ k&=~3; k-=4; obj->args[i]->fxoffs=k; nf++; }
			else		{ obj->args[i]->fxoffs=ka; ka+=4; }
			sctx->use_fpr=1;
			break;
		case BGBCC_SH_REGCLS_DR:
			if(nf<4)	{ k&=~7; k-=8; obj->args[i]->fxoffs=k; nf++; }
			else		{ ka=(ka+7)&(~7); obj->args[i]->fxoffs=ka; ka+=8; }
			sctx->use_fpr=1;
			break;
#endif

#if 0
		case BGBCC_SH_REGCLS_FR2:
			if(sctx->is_addr64)
			{
				nf+=nf&1;
				if(nf<7)
				{
					k&=~7; k-=8;
					obj->args[i]->fxoffs=k; nf+=2;
				}
				else
				{
					ka=(ka+7)&(~7);
					obj->args[i]->fxoffs=ka; ka+=8;
				}
				sctx->use_fpr=1;
				sctx->use_dbr=1;
				break;
			}

			nf+=nf&1;
			if(nf<7)	{ k&=~3; k-=8; obj->args[i]->fxoffs=k; nf+=2; }
			else		{ obj->args[i]->fxoffs=ka; ka+=8; }
			sctx->use_fpr=1;
			sctx->use_dbr=1;
			break;
#endif

		default:
			BGBCC_DBGBREAK
			break;
		}
	}
#endif

	if((-k)>=BGBCC_MAXSTACKFRAME)
		{ BGBCC_DBGBREAK }

#if 0
	for(i=0; i<obj->n_locals; i++)
		{ obj->locals[i]->fxoffs=0; }
	for(i=0; i<obj->n_regs; i++)
		{ obj->regs[i]->fxoffs=0; }

	for(i=sctx->vspan_num-1; i>=0; i--)
	{
		reg=sctx->vspan[i]->reg;

//		tty=BGBCC_CCXL_GetRegType(ctx, reg);

		if(	!BGBCC_CCXL_IsRegTempP(ctx, reg) &&
			!BGBCC_CCXL_IsRegLocalP(ctx, reg))
				continue;

		tty=BGBCC_CCXL_GetRegType(ctx, reg);
		rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, tty);
		switch(rcls)
		{
		case BGBCC_SH_REGCLS_GR:
		case BGBCC_SH_REGCLS_VO_GR:
			k&=~3; k-=4;	break;
		case BGBCC_SH_REGCLS_GR:
			k&=~1; k-=2;	break;
		case BGBCC_SH_REGCLS_VO_REF:
		case BGBCC_SH_REGCLS_AR_REF:
		case BGBCC_SH_REGCLS_VO_QGR:
		case BGBCC_SH_REGCLS_QGR:
			if(sctx->is_addr64)
				{ k&=~7; k-=8;	break; }
			k&=~3; k-=4;	break;
		case BGBCC_SH_REGCLS_GR2:
		case BGBCC_SH_REGCLS_QGR2:
		case BGBCC_SH_REGCLS_VO_GR2:
		case BGBCC_SH_REGCLS_VO_QGR2:
		case BGBCC_SH_REGCLS_VO_REF2:
		case BGBCC_SH_REGCLS_AR_REF2:
			if(sctx->is_addr64)
				{ k&=~7; k-=16;	break; }
			k&=~3; k-=8;	break;
		case BGBCC_SH_REGCLS_FR:
			sctx->use_fpr=1;
			k&=~3; k-=4;	break;
		case BGBCC_SH_REGCLS_DR:
			sctx->use_fpr=1;
			sctx->use_dbr=1;
			k&=~7; k-=8;	break;
		case BGBCC_SH_REGCLS_FR2:
			sctx->use_fpr=1;
			sctx->use_dbr=1;
			k&=~3; k-=8;	break;
		default:
			BGBCC_DBGBREAK
			break;
		}
		
		j=reg.val&4095;
		if(BGBCC_CCXL_IsRegTempP(ctx, reg))
			{ obj->regs[j]->fxoffs=k; }
		if(BGBCC_CCXL_IsRegLocalP(ctx, reg))
			{ obj->locals[j]->fxoffs=k; }
	}

	sctx->frm_offs_lcl=k;
	sctx->frm_offs_tmp=k;
#endif

	if((-k)>=BGBCC_MAXSTACKFRAME)
		{ BGBCC_DBGBREAK }

#if 1
	for(i=0; i<obj->n_locals; i++)
	{
		if(obj->locals[i]->regflags&BGBCC_REGFL_CULL)
//		if(0)
		{
			obj->locals[i]->fxoffs=k;
			continue;
		}

		j=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->locals[i]->type);
		switch(j)
		{
		case BGBCC_SH_REGCLS_GR:
		case BGBCC_SH_REGCLS_VO_GR:
			k&=~3; k-=4; obj->locals[i]->fxoffs=k;
			break;

		case BGBCC_SH_REGCLS_WGR:
			k&=~1; k-=2; obj->locals[i]->fxoffs=k;
			break;

		case BGBCC_SH_REGCLS_VO_REF:
		case BGBCC_SH_REGCLS_AR_REF:
		case BGBCC_SH_REGCLS_VO_QGR:
		case BGBCC_SH_REGCLS_QGR:
//			if(sctx->is_addr64)
//				{ k&=~7; k-=8; obj->locals[i]->fxoffs=k; break; }
//			k&=~3; k-=4; obj->locals[i]->fxoffs=k;
			k&=~7; k-=8;
			obj->locals[i]->fxoffs=k;
			break;
		case BGBCC_SH_REGCLS_GR2:
		case BGBCC_SH_REGCLS_QGR2:
		case BGBCC_SH_REGCLS_VO_GR2:
		case BGBCC_SH_REGCLS_VO_QGR2:
		case BGBCC_SH_REGCLS_VO_REF2:
		case BGBCC_SH_REGCLS_AR_REF2:
//			if(sctx->is_addr64)
//				{ k&=~7; k-=16; obj->locals[i]->fxoffs=k; break; }
//				{ k&=~15; k-=16; obj->locals[i]->fxoffs=k; break; }
//			k&=~3; k-=8; obj->locals[i]->fxoffs=k;
			k&=~15; k-=16;
			obj->locals[i]->fxoffs=k;
			break;

#if 0
		case BGBCC_SH_REGCLS_FR:
			sctx->use_fpr=1;
			k&=~3; k-=4; obj->locals[i]->fxoffs=k;
			break;
		case BGBCC_SH_REGCLS_FR2:
			sctx->use_fpr=1;
			sctx->use_dbr=1;
			if(sctx->is_addr64)
				{ k&=~7; }
			k&=~3; k-=8; obj->locals[i]->fxoffs=k;
			break;
		case BGBCC_SH_REGCLS_DR:
			sctx->use_fpr=1;
			sctx->use_dbr=1;
			k&=~7; k-=8; obj->locals[i]->fxoffs=k;
			break;
#endif

		default:
			BGBCC_DBGBREAK
			break;
		}
	}
	sctx->frm_offs_lcl=k;

	if((-k)>=BGBCC_MAXSTACKFRAME)
		{ BGBCC_DBGBREAK }

	for(i=0; i<obj->n_regs; i++)
	{
		rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->regs[i]->type);
//		rcls=BGBCC_SHXC_TypeGetRegClassP(ctx, 
//			BGBCC_CCXL_TypeWrapBasicType(obj->regs[i]->type_zb));
		switch(rcls)
		{
		case BGBCC_SH_REGCLS_GR:
		case BGBCC_SH_REGCLS_VO_GR:
			k&=~3; k-=4; obj->regs[i]->fxoffs=k;
			break;

		case BGBCC_SH_REGCLS_WGR:
			k&=~1; k-=2; obj->regs[i]->fxoffs=k;
			break;

		case BGBCC_SH_REGCLS_VO_REF:
		case BGBCC_SH_REGCLS_AR_REF:
		case BGBCC_SH_REGCLS_VO_QGR:
		case BGBCC_SH_REGCLS_QGR:
//			if(sctx->is_addr64)
//				{ k&=~7; k-=8; obj->regs[i]->fxoffs=k; break; }
//			k&=~3; k-=4; obj->regs[i]->fxoffs=k;
			k&=~7; k-=8;
			obj->regs[i]->fxoffs=k;
			break;

		case BGBCC_SH_REGCLS_GR2:
		case BGBCC_SH_REGCLS_QGR2:
		case BGBCC_SH_REGCLS_VO_GR2:
		case BGBCC_SH_REGCLS_VO_QGR2:
		case BGBCC_SH_REGCLS_VO_REF2:
		case BGBCC_SH_REGCLS_AR_REF2:
//			if(sctx->is_addr64)
//				{ k&=~7; k-=16; obj->regs[i]->fxoffs=k; break; }
//				{ k&=~15; k-=16; obj->regs[i]->fxoffs=k; break; }
//			k&=~3; k-=8; obj->regs[i]->fxoffs=k;
			k&=~15; k-=16;
			obj->regs[i]->fxoffs=k;
			break;

#if 0
		case BGBCC_SH_REGCLS_FR:
			sctx->use_fpr=1;
			k&=~3; k-=4; obj->regs[i]->fxoffs=k;
			break;
		case BGBCC_SH_REGCLS_FR2:
			sctx->use_fpr=1;
			sctx->use_dbr=1;
			if(sctx->is_addr64)
				{ k&=~7; }
			k&=~3; k-=8; obj->regs[i]->fxoffs=k;
			break;
		case BGBCC_SH_REGCLS_DR:
			sctx->use_fpr=1;
			k&=~7; k-=8; obj->regs[i]->fxoffs=k;
			break;
#endif

		default:
			BGBCC_DBGBREAK
			break;
		}
	}
	sctx->frm_offs_tmp=k;
#endif

	i=obj->n_cargs;
	
//	i=24;
	
	if(ctx->cur_func->regflags&BGBCC_REGFL_NOTLEAF)
	{
//		i=(i+3)&(~3);
		i=(i+1)&(~1);
		if(i<8)
			i=8;

		if((sctx->has_xgpr&2) &&
			(ctx->cur_func->regflags&BGBCC_REGFL_CALLVARARG))
		{
			if(i<16)
				i=16;
		}
	}

//	k&=~7;
	k&=~15;
	k-=i*8;

//	if(sctx->is_addr64)
//		{ k&=~7; k-=obj->n_cargs*8; }
//	else
//		k-=obj->n_cargs*4;

	if((-k)>=BGBCC_MAXSTACKFRAME)
		{ BGBCC_DBGBREAK }

//	k-=obj->n_cargs*4;
//	k-=obj->n_cargs*8;
//	k&=~15;
//	k&=~3;

//	if(sctx->is_addr64)
	if(1)
		{ k&=~15; }

	if((-k)>=60)
	{
		j=-1;
	}
	
#if 0
	if(sctx->has_movi20)
	{
		if((-k)>=3840)
			{ k&=~255; sctx->use_bp=1; }
	}else
	{
//		if((-k)>=960)
		if((-k)>=1012)
			{ k&=~255; sctx->use_bp=1; }
	}
#else
	if((-k)>=1012)
		{ k&=~63; }
#endif
	
	if((-k)>=BGBCC_MAXSTACKFRAME)
		{ BGBCC_DBGBREAK }
	
	sctx->frm_size=-k;

	if(ctx->cur_func->regflags&BGBCC_REGFL_NOTLEAF)
	{
		sctx->stat_tot_frm_size+=sctx->frm_size;
		sctx->stat_cnt_frm_size++;
	}

	return(0);
}
