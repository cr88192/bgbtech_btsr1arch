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



int BGBCC_JX2C_EmitFrameEpilog_TinyLeaf(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	int i;

	if(sctx->is_leaftiny&8)
	{
#if 0
//		if(ctx->cur_func->regflags&BGBCC_REGFL_LEAFTINY)
		if(!(ctx->cur_func->regflags&BGBCC_REGFL_NOTLEAFTINY))
			printf("Reject LeafTiny, Epilog&8 %s\n",
				ctx->cur_func->qname);
#endif
		obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;
	}

	if(sctx->frm_offs_thisptr)
	{
		obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;
	}

//	bo=BGBCC_JX2_EmitGetOffs(sctx);

	for(i=0; i<obj->n_locals; i++)
	{
		if(obj->locals[i]->regflags&BGBCC_REGFL_ACCESSED)
			{ obj->locals[i]->regflags&=~BGBCC_REGFL_CULL; }
		else
			{ obj->locals[i]->regflags|=BGBCC_REGFL_CULL; }
	}

	for(i=0; i<obj->n_args; i++)
	{
		if(obj->args[i]->regflags&BGBCC_REGFL_ACCESSED)
			{ obj->args[i]->regflags&=~BGBCC_REGFL_CULL; }
		else
			{ obj->args[i]->regflags|=BGBCC_REGFL_CULL; }
	}

	for(i=0; i<obj->n_regs; i++)
	{
		if(obj->regs[i]->regflags&BGBCC_REGFL_ACCESSED)
			{ obj->regs[i]->regflags&=~BGBCC_REGFL_CULL; }
		else
			{ obj->regs[i]->regflags|=BGBCC_REGFL_CULL; }
	}

	BGBCC_JX2_EmitCheckFlushIndexImm(sctx);
//	BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);

	if(sctx->lbl_ret_zero)
	{
//		BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BRAN,
//			sctx->lbl_ret);
		BGBCC_JX2_EmitLabel(sctx, sctx->lbl_ret_zero);
		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV,
//			0, BGBCC_SH_REG_R2);
			0, BGBCC_JX2CC_PSREG_RQRET);
	}

	if(sctx->lbl_ret)
	{
		if(sctx->is_align_wexj&2)
			{ BGBCC_JX2_EmitPad32AlignLastOp(sctx); }
		BGBCC_JX2_EmitLabel(sctx, sctx->lbl_ret);
	}

	sctx->is_leaftiny=3;

	BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);

	BGBCC_JX2C_EmitSyncEpilog(ctx, sctx);

	BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
	BGBCC_JX2C_EmitEpilogFlushRegisters(ctx, sctx);

	BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);
//	BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);

	return(0);
}

int BGBCC_JX2C_EmitFrameEpilogUnwind(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	if(	(ctx->imgfmt==BGBCC_IMGFMT_ROM) ||
		(ctx->imgfmt==BGBCC_IMGFMT_SYS)	)
			return(0);
	
	if(sctx->abi_noexcept)
		return(0);

	if(sctx->is_leaf&1)
	{
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_PR, BGBCC_JX2CC_PSREG_TS0);
		BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
			BGBCC_JX2CC_PSREG_TS0, BGBCC_SH_REG_TBR, 0x40);
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_JX2CC_PSREG_ARG(0), BGBCC_SH_REG_PR);
	}else
	{
		BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
			BGBCC_SH_REG_SP, sctx->frm_size-8,
			BGBCC_JX2CC_PSREG_TS0);
		BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
			BGBCC_JX2CC_PSREG_TS0, BGBCC_SH_REG_TBR, 0x40);
		BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
			BGBCC_JX2CC_PSREG_ARG(0),
			BGBCC_SH_REG_SP, sctx->frm_size-8);
	}

	if(sctx->lbl_ret)
	{
		BGBCC_JX2_EmitOpAutoLabel(sctx,
			BGBCC_SH_NMID_BRAN, sctx->lbl_ret);
	}else
	{
		BGBCC_DBGBREAK
	}

//	sctx->lbl_ret
	return(0);
}

int BGBCC_JX2C_EmitFrameEpilog(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	char tb[256];
	ccxl_register reg, treg;
	ccxl_type tty;
	u64 uli, epik;
	int epix, epilbl;
	int bo, co;
	int ob, ov;
	int tr0, tr1;
	int p0, tsz;
	int i, j, k, l;

	if(sctx->is_leaftiny&8)
		obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;

	if(obj->regflags&BGBCC_REGFL_LEAFTINY)
	{
		i=BGBCC_JX2C_EmitFrameEpilog_TinyLeaf(ctx, sctx, obj);
		return(i);
	}

	bo=BGBCC_JX2_EmitGetOffs(sctx);

	for(i=0; i<obj->n_locals; i++)
	{
		if(obj->locals[i]->regflags&BGBCC_REGFL_ACCESSED)
			{ obj->locals[i]->regflags&=~BGBCC_REGFL_CULL; }
		else
			{ obj->locals[i]->regflags|=BGBCC_REGFL_CULL; }
	}

	BGBCC_JX2_EmitCheckFlushIndexImm(sctx);
//	BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);

	if(sctx->lbl_ret_zero)
	{
		BGBCC_JX2_EmitLabel(sctx, sctx->lbl_ret_zero);
		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV,
//			0, BGBCC_SH_REG_R2);
			0, BGBCC_JX2CC_PSREG_RQRET);
	}

	if(sctx->lbl_ret)
	{
		if(sctx->is_align_wexj&2)
			{ BGBCC_JX2_EmitPad32AlignLastOp(sctx); }

//		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
		BGBCC_JX2_EmitLabel(sctx, sctx->lbl_ret);
	}

	for(i=0; i<obj->n_locals; i++)
	{
		if(obj->locals[i]->flagsint&BGBCC_TYFL_DYNAMIC)
		{
			j=BGBCC_CCXL_LookupGlobalIndex(ctx, obj->locals[i]->name);
			if(j<=0)
				continue;
			tty=ctx->reg_globals[j]->type;
			reg.val=CCXL_REGTY_GLOBAL|
				(((s64)tty.val)<<CCXL_REGID_TYPESHIFT)|j;
			treg.val=CCXL_REGTY_LOCAL|
				(((s64)tty.val)<<CCXL_REGID_TYPESHIFT)|i;
			BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, tty, reg, treg);
			obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;
		}
	}

	BGBCC_JX2C_EmitSyncEpilog(ctx, sctx);
	BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);

	sctx->sreg_held=0x0003;
	sctx->sfreg_held=0x0003;

	tty=BGBCC_CCXL_GetTypeReturnType(ctx, obj->type);

	if(BGBCC_CCXL_TypeVoidP(ctx, tty))
		{ sctx->sreg_held=0x0000; sctx->sfreg_held=0x0000; }
	if(BGBCC_CCXL_TypeSmallIntP(ctx, tty) ||
		BGBCC_CCXL_TypePointerP(ctx, tty) ||
		BGBCC_CCXL_TypeValueObjectP(ctx, tty))
		{ sctx->sreg_held=0x0001; sctx->sfreg_held=0x0000; }
	if(BGBCC_CCXL_TypeSgLongP(ctx, tty) ||
		BGBCC_CCXL_TypeVariantP(ctx, tty))
		{ sctx->sreg_held=0x0003; sctx->sfreg_held=0x0000; }
	if(BGBCC_CCXL_TypeFloatP(ctx, tty))
		{ sctx->sreg_held=0x0000; sctx->sfreg_held=0x0001; }
	if(BGBCC_CCXL_TypeDoubleP(ctx, tty))
		{ sctx->sreg_held=0x0000; sctx->sfreg_held=0x0003; }

	sctx->sreg_live=sctx->sreg_held;
	sctx->sfreg_live=sctx->sfreg_held;

	k=sctx->frm_offs_retstr;
	if(k)
	{
		obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;

		tsz=BGBCC_CCXL_TypeGetLogicalSize(ctx, tty);

		if(sctx->has_xgpr&2)
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx,
				BGBCC_SH_REG_RQ2);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx,
				BGBCC_SH_REG_RQ18);
			BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx,
				0, k, BGBCC_SH_REG_RQ18);
			BGBCC_JX2C_EmitValueCopyRegRegSz(ctx, sctx,
				BGBCC_SH_REG_RQ18, BGBCC_SH_REG_RQ2, tsz, 8);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx,
				BGBCC_SH_REG_RQ18);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx,
				BGBCC_SH_REG_RQ2);
		}else
			if(sctx->is_addr64)
		{
#if 0
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ2);
			BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, 0, k, BGBCC_SH_REG_RQ2);
			BGBCC_JX2C_EmitValueCopyRegRegSz(ctx, sctx,
				BGBCC_SH_REG_RQ2, BGBCC_SH_REG_RQ0, tsz, 8);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ2);
#endif

#if 0
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ2);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ3);
			BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, 0, k, BGBCC_SH_REG_RQ3);
			BGBCC_JX2C_EmitValueCopyRegRegSz(ctx, sctx,
				BGBCC_SH_REG_RQ3, BGBCC_SH_REG_RQ2, tsz, 8);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ3);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ2);
#endif

#if 1
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_RQRET);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_RQTHIS);
			BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, 0, k, 
				BGBCC_JX2CC_PSREG_RQTHIS);
			BGBCC_JX2C_EmitValueCopyRegRegSz(ctx, sctx,
				BGBCC_JX2CC_PSREG_RQTHIS, BGBCC_JX2CC_PSREG_RQRET, tsz, 8);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_RQTHIS);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_RQRET);
#endif

		}else
		{
#if 0
//			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R2);
//			BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, 0, k, BGBCC_SH_REG_R2);
//			BGBCC_JX2C_EmitValueCopyRegRegSz(ctx, sctx,
//				BGBCC_SH_REG_R2, BGBCC_SH_REG_R0, tsz, 4);
//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R2);
#endif
		}
	}

	k=sctx->frm_offs_allocamrk;
	if(k)
	{
		if(BGBCC_JX2C_CheckFrameSavedLpReg(ctx, sctx, BGBCC_SH_REG_LR12) &&
			!(sctx->emit_riscv&0x33))
		{
			BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
				BGBCC_SH_REG_LR2, BGBCC_SH_REG_LR12);
			BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, 0, k, 
				BGBCC_JX2CC_PSREG_ARG(0));
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__alloca_end");
			BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
				BGBCC_SH_REG_LR12, BGBCC_SH_REG_LR2);
		}else
		{
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, 0,
				BGBCC_JX2CC_PSREG_LRRET);
			BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, 0, k,
				BGBCC_JX2CC_PSREG_ARG(0));
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__alloca_end");
			BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, 0, 0,
				BGBCC_JX2CC_PSREG_LRRET);
		}
	}

	k=sctx->frm_offs_sectoken;
	if(k)
	{
		BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, 0, k, BGBCC_JX2CC_PSREG_TS0);
		BGBCC_JX2_EmitRelocTy(sctx, obj->fxoffs, BGBCC_SH_RLC_SECTOKEN);
		BGBCC_JX2_EmitLoadRegImm64P(sctx, BGBCC_JX2CC_PSREG_TS1,
//			0x1234567890ABCDEFLL);
			sctx->frm_val_sectoken);

		if(sctx->emit_riscv&0x11)
		{
			/* RISC-V */
			p0=BGBCC_JX2_GenLabelTemp(sctx);
			BGBCC_JX2_EmitOpRegRegLbl(sctx, BGBCC_SH_NMID_BREQ,
				BGBCC_JX2CC_PSREG_TS1, BGBCC_JX2CC_PSREG_TS0, p0);
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_BRK);
			BGBCC_JX2_EmitLabel(sctx, p0);
		}else
			if(sctx->abi_spillpad&2)
		{
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_VSKC,
				BGBCC_JX2CC_PSREG_TS1, BGBCC_JX2CC_PSREG_TS0);
		}else
		{
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_CMPQEQ,
				BGBCC_JX2CC_PSREG_TS0, BGBCC_JX2CC_PSREG_TS1);
			sctx->op_is_wex2=5;
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_BRK);
			sctx->op_is_wex2=0;
		}
	}

//	BGBCC_JX2C_ResetFpscrDefaults(ctx, sctx);

	BGBCC_JX2C_CalcFrameEpiKey(ctx, sctx, obj, 2, &epik, &epix);
//	epix=(((((u64)epik)+1)*2147483647LL)>>32)&1023;
	epilbl=sctx->epihash_lbl[epix];

#if 0
	if(sctx->frm_offs_sectoken)
	{
		epik=0;
	}
#endif

#if 1
//	BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);

	k=sctx->frm_offs_save;
	p0=BGBCC_JX2_TryEmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
		k, BGBCC_SH_REG_SP);
	if(p0<=0)
	{
		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, tr0, k);
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, tr0, BGBCC_SH_REG_SP);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
	}

//	j=BGBCC_JX2_EmitCheckAutoLabelNear16B(sctx, epilbl, 
	j=BGBCC_JX2_EmitCheckAutoLabelNear20B(sctx, epilbl, 
		BGBCC_SH_NMID_BSR);
	if(epik && (sctx->epihash_key[epix]==epik) && (epilbl>0) &&
		!sctx->is_simpass && j)
	{
//		if((j&3)==3)
		if((j&15)==1)
		{
			BGBCC_JX2_EmitOpLabel(sctx,
				BGBCC_SH_NMID_BRAN, epilbl);
		}else
		{
//			BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BRAN, epilbl);
//			BGBCC_JX2_EmitOpFar16Label(sctx,
			BGBCC_JX2_EmitOpFar20Label(sctx,
				BGBCC_SH_NMID_BRAN, epilbl);
		}

		BGBCC_JX2_EmitFlushIndexImmBasic(sctx);
		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);

		if(sctx->is_leaf==2)
			sctx->is_leaf=1;

		co=BGBCC_JX2_EmitGetOffs(sctx);
		sctx->fnsz_epi=co-bo;

		return(0);
	}

#if 1
	if(!sctx->is_simpass && epik)
	{
		if(sctx->is_align_wexj&2)
			{ BGBCC_JX2_EmitPad32AlignLastOp(sctx); }

		l=sctx->epihash_rov++;
//		sprintf(tb, "__epilog.%04X.%016llX", l, epik);
		sprintf(tb, "__epilog_%04X_%012llX", l, epik);

//		epilbl=BGBCC_JX2_GenLabel(sctx);
		epilbl=BGBCC_JX2_GetNamedLabel(sctx, tb);
		BGBCC_JX2_EmitLabel(sctx, epilbl);

		sctx->epihash_key[epix]=epik;
		sctx->epihash_lbl[epix]=epilbl;
	}
#endif

//	BGBCC_JX2_EmitWord(sctx, 0x0848);	//Debug, Clear Token

#if 0
//	for(i=0; i<16; i++)
//	for(i=0; i<32; i++)
	for(i=0; i<64; i++)
	{
		if(sctx->freg_save&(1ULL<<i))
		{
			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP,
				BGBCC_SH_REG_FR0+i);
			k+=8;

//			BGBCC_JX2_EmitOpLdIncRegReg(sctx, BGBCC_SH_NMID_FMOVS,
//				BGBCC_SH_REG_SP, BGBCC_SH_REG_FR0+i);
//			k+=4;
		}
	}

	if(sctx->use_fpr && !sctx->no_fpu && !sctx->fpu_gfp)
	{
//		BGBCC_JX2_EmitOpLdIncRegReg(sctx, BGBCC_SH_NMID_LDSL,
//			BGBCC_SH_REG_SP, BGBCC_SH_REG_FPSCR);
//		k+=sctx->is_addr64?8:4;
	}
#endif

//	if(	(ctx->optmode==BGBCC_OPT_SPEED) ||
//		(ctx->optmode==BGBCC_OPT_SPEED2) )
	if(1)
	{
		j=k;

#if 1
		if(	!(obj->flagsint&BGBCC_TYFL_INTERRUPT) &&
			!(obj->flagsint&BGBCC_TYFL_SYSCALL) &&
			!(sctx->is_leaf&1))
		{
			for(i=0; i<64; i++)
			{
#if 0
				if(!(sctx->has_xgpr&1) && (i>=32))
					continue;
				if(!(sctx->has_bjx1egpr) && (i>=16))
					continue;
#endif
				if(i==15)
					continue;

				if(sctx->reg_save&(1ULL<<i))
				{
					k+=8;
				}
			}

			if(sctx->is_pbo && !(sctx->is_pbo&2) &&
					(obj->regflags&BGBCC_REGFL_ALIASPTR))
				k+=8;

			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_SP, (k-j),
				BGBCC_JX2CC_PSREG_RTSLR);

			k=j;

		}
#endif

		j=k;
	
	//	for(i=0; i<15; i++)
//		for(i=0; i<32; i++)
		for(i=0; i<64; i++)
		{
#if 0
			if(!(sctx->has_xgpr&1) && (i>=32))
				continue;
			if(!(sctx->has_bjx1egpr) && (i>=16))
				continue;
#endif
			if(i==15)
				continue;

#if 1
			if(sctx->has_pushx2 && !(i&1))
//			if(0)
			{
				if(((sctx->reg_save&(3ULL<<i))==(3ULL<<i)) &&
					(i!=14) && (i!=31) && (i!=63))
				{
//					BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POPX2,
//						BGBCC_SH_REG_R0+i);
					BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVX2,
						BGBCC_SH_REG_SP, (k-j),
						BGBCC_SH_REG_R0+i);
					k+=16;
					i++;
					continue;
				}
			}
#endif

			if(sctx->reg_save&(1ULL<<i))
			{
//				BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP,
//					BGBCC_SH_REG_RQ0+i);
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_SP, (k-j),
					BGBCC_SH_REG_R0+i);
				k+=8;
			}
		}

#if 0
		if((sctx->frm_offs_sectoken) && !epik)
		{
			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_REGCHKC,
				BGBCC_SH_REG_R16);
		}
#endif

//		if(	!(obj->flagsint&BGBCC_TYFL_INTERRUPT) &&
//			!(obj->flagsint&BGBCC_TYFL_SYSCALL) &&
//			!(sctx->is_leaf&1)	)
//		if(0)
		if(	!(obj->flagsint&BGBCC_TYFL_INTERRUPT) &&
			!(obj->flagsint&BGBCC_TYFL_SYSCALL))
		{
#if 1
			if(sctx->is_leaf&1)
			{
				if(sctx->is_pbo && !(sctx->is_pbo&2) &&
					(obj->regflags&BGBCC_REGFL_ALIASPTR))
				{
					if(((k+8)!=sctx->frm_size) && !sctx->is_simpass)
						{ BGBCC_DBGBREAK; }

					if(sctx->has_fmovc)
					{
						BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVC,
							BGBCC_SH_REG_SP, (k-j),
							BGBCC_SH_REG_GBR);
					}else
					{
						BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
							BGBCC_SH_REG_SP, (k-j),
							BGBCC_JX2CC_PSREG_TS0);
						BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
							BGBCC_JX2CC_PSREG_TS0, BGBCC_SH_REG_GBR);
					}
					k+=8;

					BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
						k-j, BGBCC_SH_REG_SP);
					if(BGBCC_JX2_CheckPipelineMin(sctx, 6))
						{ BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTSU); }
					else
						{ BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS); }
				}else
				{
					BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
						k-j, BGBCC_SH_REG_SP);
					if(BGBCC_JX2_CheckPipelineMin(sctx, 6))
						{ BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTSU); }
					else
						{ BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS); }
//					BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
				}
			}
			else
#endif
				if(sctx->is_pbo && !(sctx->is_pbo&2) &&
					(obj->regflags&BGBCC_REGFL_ALIASPTR))
			{
				if(((k+16)!=sctx->frm_size) && !sctx->is_simpass)
					{ BGBCC_DBGBREAK; }

//				if(sctx->has_pushx2)
				if(0)
				{
//					BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVX2,
//						BGBCC_SH_REG_SP, (k-j),
//						BGBCC_SH_REG_R18);
				}else
				{
					if(sctx->has_fmovc)
					{
#if 0
						BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
							BGBCC_SH_REG_SP, (k-j)+8,
							BGBCC_SH_REG_R1);
#endif
						BGBCC_JX2_EmitOpLdRegDispReg(sctx,
							BGBCC_SH_NMID_MOVC,
							BGBCC_SH_REG_SP, (k-j)+0,
							BGBCC_SH_REG_GBR);
//						BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVC,
//							BGBCC_SH_REG_SP, (k-j)+8,
//							BGBCC_SH_REG_PR);
					}else
					{
#if 0
						BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
							BGBCC_SH_REG_SP, (k-j)+8,
	//						BGBCC_SH_REG_R19);
							BGBCC_SH_REG_R1);
#endif
						BGBCC_JX2_EmitOpLdRegDispReg(sctx,
							BGBCC_SH_NMID_MOVQ,
							BGBCC_SH_REG_SP, (k-j)+0,
							BGBCC_JX2CC_PSREG_TS0);
					}
				}
				k+=16;

				BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
					k-j, BGBCC_SH_REG_SP);

				if(sctx->has_fmovc)
				{
//					BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);
					BGBCC_JX2_EmitOpReg(sctx,
//						BGBCC_SH_NMID_JMP, BGBCC_SH_REG_R1);
						BGBCC_SH_NMID_JMP, BGBCC_JX2CC_PSREG_RTSLR);
//					BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
				}else
				{
					BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
						BGBCC_JX2CC_PSREG_TS0, BGBCC_SH_REG_GBR);
//					BGBCC_JX2_EmitOpReg(sctx,
//						BGBCC_SH_NMID_JMP, BGBCC_SH_REG_R19);
					BGBCC_JX2_EmitOpReg(sctx,
//						BGBCC_SH_NMID_JMP, BGBCC_SH_REG_R1);
						BGBCC_SH_NMID_JMP, BGBCC_JX2CC_PSREG_RTSLR);
//					BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
				}
			}else
			{
				if(((k+8)!=sctx->frm_size) && !sctx->is_simpass)
					{ BGBCC_DBGBREAK; }

//				if(sctx->has_fmovc)
				if(0)
				{
					BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVC,
						BGBCC_SH_REG_SP, (k-j),
						BGBCC_SH_REG_PR);
					k+=8;

					BGBCC_JX2_EmitOpImmReg(sctx,
						BGBCC_SH_NMID_ADD,
						k-j, BGBCC_SH_REG_SP);

					BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);
//					BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
				}else
				{
#if 0
					BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
						BGBCC_SH_REG_SP, (k-j),
						BGBCC_SH_REG_R1);
#endif
					k+=8;

					BGBCC_JX2_EmitOpImmReg(sctx,
						BGBCC_SH_NMID_ADD,
						k-j, BGBCC_SH_REG_SP);

					BGBCC_JX2_EmitOpReg(sctx,
						BGBCC_SH_NMID_JMP, BGBCC_JX2CC_PSREG_RTSLR);
//					BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
				}
			}


//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP, BGBCC_SH_REG_PR);
//			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTE);

			BGBCC_JX2_EmitFlushIndexImmBasic(sctx);
			BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);

			if(sctx->is_leaf==2)
				sctx->is_leaf=1;

			co=BGBCC_JX2_EmitGetOffs(sctx);
			sctx->fnsz_epi=co-bo;

			return(0);

		}
		else
		{
//			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
			BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
				k-j, BGBCC_SH_REG_SP);
		}
	}else
	{

		BGBCC_DBGBREAK

#if 0
	//	for(i=0; i<15; i++)
//		for(i=0; i<32; i++)
		for(i=0; i<64; i++)
		{
			if(!(sctx->has_xgpr&1) && (i>=32))
				continue;
			if(!(sctx->has_bjx1egpr) && (i>=16))
				continue;
			if(i==15)
				continue;

	#if 1
			if(sctx->has_pushx2 && !(i&1))
	//		if(0)
			{
				if(((sctx->reg_save&(3ULL<<i))==(3ULL<<i)) &&
					(i!=14) && (i!=31) && (i!=63))
				{
					BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POPX2,
						BGBCC_SH_REG_R0+i);
					k+=sctx->is_addr64?16:8;
					i++;
					continue;
				}
			}
	#endif

			if(sctx->reg_save&(1ULL<<i))
			{
				if(sctx->is_addr64)
	//			if(1)
				{
					BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP,
						BGBCC_SH_REG_RQ0+i);
					k+=8;
				}else
				{
	//				BGBCC_JX2_EmitOpLdIncRegReg(sctx, BGBCC_SH_NMID_MOVL,
	//					BGBCC_SH_REG_SP, BGBCC_SH_REG_R0+i);
					BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP,
						BGBCC_SH_REG_R0+i);
					k+=4;
				}
			}
		}
#endif

	}

	sctx->sreg_live		= 0;
	sctx->sfreg_live	= 0;

	if(	sctx->regalc_live || sctx->fregalc_live ||
		sctx->sreg_live || sctx->sfreg_live)
//	if(	sctx->regalc_live || sctx->fregalc_live)
			{ BGBCC_DBGBREAK }

	j=k;




#if 1
//	if(obj->flagsint&BGBCC_TYFL_INTERRUPT)
		if(	(obj->flagsint&BGBCC_TYFL_INTERRUPT)	&&
			(obj->flagsint&BGBCC_TYFL_TBRSAVE)		)
	{
		BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
			BGBCC_SH_REG_TBR, 0x20, BGBCC_SH_REG_R2);


#if 1
		if(sctx->has_pushx2)
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVX2,
				BGBCC_SH_REG_R2, 0x40*8, BGBCC_SH_REG_LR4);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVX2,
				BGBCC_SH_REG_R2, 0x42*8, BGBCC_SH_REG_LR6);

			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVX2,
				BGBCC_SH_REG_LR4, BGBCC_SH_REG_SP, ((k+4*8)-j));
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVX2,
				BGBCC_SH_REG_LR6, BGBCC_SH_REG_SP, ((k+6*8)-j));

			for(i=4; i<32; i+=2)
			{
				if(i==14)
				{
					continue;
				}
			
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVX2,
					BGBCC_SH_REG_R2, i*8, BGBCC_SH_REG_LR0+i);
			}

			if(sctx->has_xgpr&1)
			{
				for(i=32; i<64; i+=2)
				{
					BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVX2,
						BGBCC_SH_REG_R2, i*8, BGBCC_SH_REG_LR32+(i-32));
				}
			}

			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R2, 14*8, BGBCC_SH_REG_R14);

			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R2, 15*8, BGBCC_SH_REG_R1);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R1, BGBCC_SH_REG_SGR);

			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R2, 0*8, BGBCC_SH_REG_R0);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R2, 1*8, BGBCC_SH_REG_R1);


			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVX2,
				BGBCC_SH_REG_R2, 2*8, BGBCC_SH_REG_LR0+2);

			k+=4*8;
		}else
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R2, 0x40*8, BGBCC_SH_REG_R4);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R2, 0x41*8, BGBCC_SH_REG_R5);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R2, 0x42*8, BGBCC_SH_REG_R6);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R2, 0x43*8, BGBCC_SH_REG_R7);

			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R4, BGBCC_SH_REG_SP, ((k+4*8)-j));
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R5, BGBCC_SH_REG_SP, ((k+5*8)-j));
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R6, BGBCC_SH_REG_SP, ((k+6*8)-j));
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R7, BGBCC_SH_REG_SP, ((k+7*8)-j));

			for(i=4; i<32; i++)
			{
//				if(i==14)
//					continue;
				if(i==15)
					continue;
			
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R2, i*8,
					BGBCC_SH_REG_RQ0+i);
			}

			if(sctx->has_xgpr&1)
			{
				for(i=32; i<64; i++)
				{
					BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
						BGBCC_SH_REG_R2, i*8,
						BGBCC_SH_REG_RQ0+i);
				}
			}

//			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
//				BGBCC_SH_REG_R2, 14*8, BGBCC_SH_REG_R14);

			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R2, 15*8, BGBCC_SH_REG_R1);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R1, BGBCC_SH_REG_SGR);

			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R2, 0*8, BGBCC_SH_REG_R0);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R2, 1*8, BGBCC_SH_REG_R1);

			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R2, 3*8, BGBCC_SH_REG_R3);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R2, 2*8, BGBCC_SH_REG_R2);

			k+=4*8;
		}
#endif

	}
#endif





//	if(obj->flagsint&BGBCC_TYFL_INTERRUPT)
		if(	 (obj->flagsint&BGBCC_TYFL_INTERRUPT)	&&
			!(obj->flagsint&BGBCC_TYFL_TBRSAVE)		)
	{
//		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_TRAPA,
//			2, BGBCC_SH_REG_SP);

#if 1
		if(sctx->has_pushx2)
		{
#if 1
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_SP, ((k+14*8)-j),
				BGBCC_SH_REG_R14);

			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_SP, ((k+15*8)-j),
				BGBCC_SH_REG_R1);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R1, BGBCC_SH_REG_SGR);

			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
//				BGBCC_SH_REG_SP, (k-j),
				BGBCC_SH_REG_SP, ((k+0*8)-j),
				BGBCC_SH_REG_R0);
//			k+=8;
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
//				BGBCC_SH_REG_SP, (k-j),
				BGBCC_SH_REG_SP, ((k+1*8)-j),
				BGBCC_SH_REG_R1);

//			k+=8;
			k+=16;
#endif

#if 1
			for(i=2; i<32; i+=2)
			{
				if(i==14)
				{
					k+=16;
					continue;
				}
			
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVX2,
					BGBCC_SH_REG_SP, (k-j),
//					BGBCC_SH_REG_RQ0+i);
					BGBCC_SH_REG_LR0+i);
				k+=16;
			}

			if(sctx->has_xgpr&1)
			{
				for(i=32; i<64; i+=2)
				{
					BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVX2,
						BGBCC_SH_REG_SP, (k-j),
//						BGBCC_SH_REG_RQ0+i);
						BGBCC_SH_REG_LR32+(i-32));
					k+=16;
				}
			}

#endif

#if 0
//			for(i=0; i<8; i+=2)
			for(i=2; i<8; i+=2)
			{
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVX2,
					BGBCC_SH_REG_SP, (k-j),
					BGBCC_SH_REG_RQ0+i);
				k+=16;
			}

			for(i=16; i<24; i+=2)
			{
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVX2,
					BGBCC_SH_REG_SP, (k-j),
					BGBCC_SH_REG_RQ0+i);
				k+=16;
			}
#endif
		}else
		{
#if 1
			for(i=0; i<32; i++)
			{
				if(i==15)
				{
					k+=8;
					continue;
				}
			
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_SP, (k-j),
					BGBCC_SH_REG_RQ0+i);
				k+=8;
			}
#endif

#if 1
			if(sctx->has_xgpr&1)
			{
				for(i=32; i<64; i++)
				{
					BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
						BGBCC_SH_REG_SP, (k-j),
						BGBCC_SH_REG_RQ0+i);
					k+=8;
				}
			}
#endif

#if 0
			for(i=0; i<8; i++)
			{
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_SP, (k-j),
					BGBCC_SH_REG_R0+i);
				k+=8;
			}

			if(sctx->has_bjx1egpr)
			{
				for(i=16; i<24; i++)
				{
					BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
						BGBCC_SH_REG_SP, (k-j),
						BGBCC_SH_REG_R0+i);
					k+=8;
				}
			}
#endif
		}

//		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
//			k-j, BGBCC_SH_REG_SP);
#endif


#if 0
		for(i=0; i<8; i++)
		{
			if(sctx->is_addr64)
			{
				BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP,
					BGBCC_SH_REG_RQ0+i);
				k+=8;
			}else
			{
				BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP,
					BGBCC_SH_REG_R0+i);
				k+=4;
			}
		}

//		if(sctx->has_bjx1egpr)
		if(1)
		{
			for(i=16; i<24; i++)
			{
				if(sctx->is_addr64)
				{
					BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP,
						BGBCC_SH_REG_RQ0+i);
					k+=8;
				}else
				{
					BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP,
						BGBCC_SH_REG_R0+i);
					k+=4;
				}
			}
		}
#endif
	}

#if 1
	if(obj->flagsint&BGBCC_TYFL_INTERRUPT)
	{
		if(!sctx->has_fmovc)
		{
	//		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
	//			BGBCC_SH_REG_R1, BGBCC_SH_REG_TEA);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R1,
				BGBCC_SH_REG_SP, 0);
		}
	}
#endif

//	if(sctx->is_pbo)
//	if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
	if((sctx->is_pbo && !(sctx->is_pbo&2) &&
		(obj->regflags&BGBCC_REGFL_ALIASPTR)) ||
		(obj->flagsint&BGBCC_TYFL_INTERRUPT))
	{
		if(sctx->has_fmovc)
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVC,
				BGBCC_SH_REG_SP, (k-j),
				BGBCC_SH_REG_GBR);
		}
		else
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_SP, (k-j),
				BGBCC_JX2CC_PSREG_ALTLR);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_JX2CC_PSREG_ALTLR, BGBCC_SH_REG_GBR);
		}
		k+=8;

//		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP, BGBCC_SH_REG_GBR);
//		k+=sctx->is_addr64?8:4;
	}

//	BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
//		k-j, BGBCC_SH_REG_SP);

	if(	sctx->regalc_live || sctx->fregalc_live ||
		sctx->sreg_live || sctx->sfreg_live)
			{ BGBCC_DBGBREAK }

//	if(!(sctx->is_leaf&1) && !(obj->flagsint&BGBCC_TYFL_INTERRUPT))
	if(!(sctx->is_leaf&1) || (obj->flagsint&BGBCC_TYFL_INTERRUPT) ||
		(obj->flagsint&BGBCC_TYFL_SYSCALL))
//	if(!(sctx->is_leaf&1) || (obj->flagsint&BGBCC_TYFL_INTERRUPT) ||
//		(obj->flagsint&BGBCC_TYFL_SYSCALL) ||
//		(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR)))
	{
		if(sctx->has_fmovc)
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVC,
				BGBCC_SH_REG_SP, (k-j),
				BGBCC_SH_REG_PR);
		}else
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_SP, (k-j),
				BGBCC_JX2CC_PSREG_RTSLR);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_JX2CC_PSREG_RTSLR, BGBCC_SH_REG_PR);
		}

		k+=8;

//		if(sctx->is_addr64)
//			k+=8;
//		else
//			k+=4;
	}

#if 1
	if(obj->flagsint&BGBCC_TYFL_INTERRUPT)
	{
		BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
			BGBCC_SH_REG_SP, (k-j),
			BGBCC_SH_REG_R1);
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R1, BGBCC_SH_REG_SPC);
		k+=8;

		BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
			BGBCC_SH_REG_SP, (k-j),
			BGBCC_SH_REG_R1);
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R1, BGBCC_SH_REG_EXSR);
		k+=8;
	}
#endif

#if 1
	if(obj->flagsint&BGBCC_TYFL_INTERRUPT)
	{
		if(!sctx->has_fmovc)
		{
	//		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
	//			BGBCC_SH_REG_TEA, BGBCC_SH_REG_R1);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_SP, 0,
				BGBCC_SH_REG_R1);
		}
	}
#endif

//	BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);

	BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
		k-j, BGBCC_SH_REG_SP);

	if((k!=sctx->frm_size) && !sctx->is_simpass)
		{ BGBCC_DBGBREAK; }
#endif

	if((obj->flagsint&BGBCC_TYFL_INTERRUPT) ||
		(obj->flagsint&BGBCC_TYFL_SYSCALL))
	{
//		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP, BGBCC_SH_REG_PR);
//		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//			BGBCC_SH_REG_R1, BGBCC_SH_REG_PR);

		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);

		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTE);

		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
	}
	else
		if(!(sctx->is_leaf&1))
	{

		BGBCC_DBGBREAK

//		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP, BGBCC_SH_REG_PC);
//		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RET);
		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_JMP, BGBCC_SH_REG_R1);

	}else
	{
//		if(BGBCC_JX2_CheckPipelineMin(sctx, 4))
		if(BGBCC_JX2_CheckPipelineMin(sctx, 6))
		{
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTSU);
		}else
		{
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);
		}
//		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
	}

	if(	sctx->regalc_live || sctx->fregalc_live ||
		sctx->sreg_live || sctx->sfreg_live)
			{ BGBCC_DBGBREAK }

#if 0
	ob=BGBCC_JX2_EmitGetOffs(ctx);
	ov=BGBCC_JX2_EmitGetOffsWord(ctx, ob);
	BGBCC_JX2_EmitSetOffsWord(ctx, ob, ov);
#endif

	BGBCC_JX2_EmitFlushIndexImmBasic(sctx);
//	BGBCC_JX2_EmitFlushIndexImm16(sctx);
//	BGBCC_JX2_EmitFlushIndexImm32(sctx);
	BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);

	BGBCC_JX2C_EmitEpilogFlushRegisters(ctx, sctx);

	if(sctx->is_leaf==2)
		sctx->is_leaf=1;
//	sctx->is_leaf=0;

	co=BGBCC_JX2_EmitGetOffs(sctx);
	sctx->fnsz_epi=co-bo;

	return(0);
}
