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

int BGBCC_JX2C_CalcBitCount(u64 val)
{
	static const byte bitcnt[16]={
		0, 1, 1, 2,  1, 2, 2, 3,
		1, 2, 2, 3,  2, 3, 3, 4	};
	int c;

	c=	bitcnt[(val>> 0)&15] +
		bitcnt[(val>> 4)&15] +
		bitcnt[(val>> 8)&15] +
		bitcnt[(val>>12)&15] +
		bitcnt[(val>>16)&15] +
		bitcnt[(val>>20)&15] +
		bitcnt[(val>>24)&15] +
		bitcnt[(val>>28)&15] +
		bitcnt[(val>>32)&15] +
		bitcnt[(val>>36)&15] +
		bitcnt[(val>>40)&15] +
		bitcnt[(val>>44)&15] +
		bitcnt[(val>>48)&15] +
		bitcnt[(val>>52)&15] +
		bitcnt[(val>>56)&15] +
		bitcnt[(val>>60)&15] ;
	return(c);
}

int BGBCC_JX2C_CalcFrameEpiKey(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj,
	int rqt, u64 *repik, int *repix)
{
	u64 uli;
	u64 epik, epix, epilbl;
	int bc, bcm;

	*repik=0;
	*repix=0;

	if(sctx->is_simpass)
		return(0);

//	if(!sctx->has_bjx1jmp)
//		return(0);

	if(	(obj->flagsint&BGBCC_TYFL_INTERRUPT) ||
		(obj->flagsint&BGBCC_TYFL_SYSCALL))
			return(0);

	/* Don't compress prologs/epilogs for hot-path functions. */
	if(obj->regflags&BGBCC_REGFL_GOFAST)
//	if(	(obj->regflags&BGBCC_REGFL_GOFAST) &&
//		(ctx->optmode!=BGBCC_OPT_SIZE))
		return(0);


//	if(ctx->optmode==BGBCC_OPT_SPEED)
//		return(0);
//	if(ctx->optmode!=BGBCC_OPT_SIZE)
//		return(0);

	if((sctx->is_leaf&1) && (rqt&1))
//	if(1)
	{
		return(0);
	}
	
	if(rqt&1)
	{
		if(sctx->is_leaf)
//		if(sctx->is_leaf&1)
			return(0);
			
//		return(0);
	}

//	epik=	((sctx->reg_save >> 8)&0x000000FF)|
//			((sctx->reg_save >>16)&0x0000FF00)|
//			((sctx->freg_save<< 8)&0x00FF0000);

	epik=	((sctx->reg_save >> 8)&0x00000000000000FFULL)|
			((sctx->reg_save >>16)&0x000000000000FF00ULL)|
			((sctx->freg_save<< 8)&0x0000000000FF0000ULL)|
			((sctx->reg_save >> 8)&0x000000FF00000000ULL)|
			((sctx->reg_save >>16)&0x0000FF0000000000ULL);

//	if(sctx->use_fpr)
//		epik|=0x01000000;
	if(!(sctx->is_leaf&1))
		epik|=0x02000000ULL;

	if((obj->regflags&BGBCC_REGFL_ALIASPTR) && sctx->is_pbo &&
			!(sctx->is_pbo&2))
		epik|=0x04000000ULL;
		
	uli=epik;
//	uli=(uli+1)*65521;
//	uli=(uli+1)*65521;
//	uli=(uli+1)*251;

	uli=(uli+(uli>>31)+1)*251;
	uli=(uli+(uli>>31)+1)*251;
	uli=(uli+(uli>>31)+1)*251;
	uli=(uli+(uli>>31)+1)*251;
	uli=(uli+(uli>>31)+1)*251;
	uli=(uli+(uli>>31)+1)*251;

	epix=(uli>>32)&1023;

	/* Unusual Registers Saved */
	if(sctx->reg_save&0x00FF00FF00FF00FFULL)
		{ epilbl=0; epik=0; epix=0; }
	if(sctx->freg_save&0xFFFFFFFFFFFF00FFULL)
		{ epilbl=0; epik=0; epix=0; }

	/* Not enough registers saved */
//	if((epik&0x60)!=0x60)
//	if((epik&0x70)!=0x70)
//	if((epik&0x7C)!=0x7C)
//		{ epilbl=0; epik=0; epix=0; }
//	if((epik&0x60)!=0x60)
//		{ epilbl=0; epik=0; epix=0; }

	bcm=4;
	if(	(ctx->optmode==BGBCC_OPT_SPEED) ||
			(ctx->optmode==BGBCC_OPT_SPEED2) )
				bcm=5;
	if(ctx->optmode==BGBCC_OPT_SIZE)
		bcm=4;

	bc=BGBCC_JX2C_CalcBitCount(epik);
//	if(bc<6)
//	if(bc<5)
//	if(bc<4)
	if(bc<bcm)
		{ epilbl=0; epik=0; epix=0; }

	if(rqt&1)
	{
//		if((epik&0x78)!=0x78)
//			{ epilbl=0; epik=0; epix=0; }

//		if((epik&0x7C)!=0x7C)
//		if(epik && ((epik&0x7C)!=0x7C))
//		{
//			epilbl=0;
//			epik=0; epix=0;
//		}
	}

	*repik=epik;
	*repix=epix;
	return(0);
}

/* Estimate permutation RAW/WAW cost.
 * This cost may happens if a subsequent store overlaps with the same
 * Cache lines as a preceding store.
 */
int BGBCC_JX2C_EmitFrameProlog_EstPermRawCost(int *rja, int *perm, int n)
{
	int ix0, ix1, ix0l, ix1l;
	int pen;
	int i, j, k;
	
	ix0l=-1;
	ix1l=-1;
	pen=0;
	
	for(i=0; i<n; i++)
	{
		j=rja[perm[i]];
		k=512-j*4;
		
		if(k&16)
		{
			ix0=(k>>4)+1;
			ix1=(k>>4)+0;
		}else
		{
			ix0=(k>>4)+0;
			ix1=(k>>4)+1;
		}
		
		if((ix0==ix0l) || (ix1==ix1l))
		{
			pen++;
		}
		
		ix0l=ix0;
		ix1l=ix1;
	}
	return(pen);
}

/* Try to find the permutation that would give the fewest RAW stalls. */
int BGBCC_JX2C_EmitFrameProlog_GenBestPerm(int *rja, int *perm)
{
	int pta[64], pba[64];
	int d, db, seed;
	int i0, i1;
	int i, j, k, n;
	
	n=0; seed=0;
	for(i=63; i>=0; i--)
	{
		j=i^2;
		k=rja[j];
		if(k<0)
			continue;
		pba[n++]=j;
		seed=seed*251+k+1;
	}
	
	if(n<2)
	{
		if(n)
			memcpy(perm, pba, n*sizeof(int));
		return(n);
	}

	db=BGBCC_JX2C_EmitFrameProlog_EstPermRawCost(rja, pba, n);
	if(db==0)
	{
		memcpy(perm, pba, n*sizeof(int));
		return(n);
	}
	
	memcpy(pta, pba, n*sizeof(int));
	
	for(i=0; i<10; i++)
	{
		for(j=0; j<n; j++)
		{
			k=((seed>>16)&255)%n;
			seed=seed*65521+1;
			i0=pta[j];	i1=pta[k];
			pta[j]=i1;	pta[k]=i0;
		}

		d=BGBCC_JX2C_EmitFrameProlog_EstPermRawCost(rja, pta, n);
		if(d<db)
		{
			memcpy(pba, pta, n*sizeof(int));
			db=d;
		}
	}

	memcpy(perm, pba, n*sizeof(int));
	return(n);
}

/*
 fl&1: LR | GBR passed in ALTLR
 fl&2: GBR/LR in R6/R7 or R18/R19
 fl&8: Save LR directly from R1 (RV / XG2RV / XG3RV)
 */
int BGBCC_JX2C_EmitFrameProlog_PushRegs(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx, int fl, int *rfl2)
{
	int rja[64], perm[64];
	int i0, i1, pn;
	int i, j, k, n, fl2;

//	if(	(ctx->optmode==BGBCC_OPT_SPEED) ||
//		(ctx->optmode==BGBCC_OPT_SPEED2))
//	if(0)
	if(1)
	{
		k=0;
		n=0;
		fl2=0;

		if(fl&1)
			k+=2;
		if(fl&8)
			k+=2;
		if(fl&2)
			k+=4;

		for(i=0; i<64; i++)
			rja[i]=-1;

//		for(i=31; i>0; i--)
		for(i=63; i>0; i--)
		{
#if 0
			if(!(sctx->has_xgpr&1) && (i>=32))
				continue;
			if(!(sctx->has_bjx1egpr) && (i>=16))
				continue;
#endif
			if(i==15)
				continue;

			if(!(sctx->reg_vsave&((2ULL<<i)-1)))
				break;

			if(sctx->has_pushx2 && (i&1))
//			if(0)
			{
				if((sctx->reg_vsave&(3ULL<<(i-1)))==(3ULL<<(i-1)))
				{
					sctx->reg_save|=(3ULL<<(i-1));
					k+=4;
					rja[i]=k;
					i--;
//					rja[i]=k;
					continue;
				}
			}

			if(sctx->reg_vsave&(1ULL<<i))
			{
				sctx->reg_save|=(1ULL<<i);
				k+=2;
				rja[i]=k;
			}
		}

//		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);

		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
			-k*4, BGBCC_SH_REG_SP);
		n++;
			
		j=0;

		if(fl&8)
		{
			if(sctx->emit_riscv&0x33)
			{
				j+=2;
				/* Save LR via RQ1 Direct, RV/XG3 */
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_RQ1, BGBCC_SH_REG_SP, (k-j)*4);
				n++;
			}else
			{
				BGBCC_DBGBREAK
			}
		}

		if(fl&1)
		{
			j+=2;
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_JX2CC_PSREG_ALTLR, BGBCC_SH_REG_SP, (k-j)*4);
			n++;
		}

		if(fl&2)
		{
			j+=4;
			if(sctx->emit_riscv&0x11)
			{
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R7, BGBCC_SH_REG_SP, ((k-j)+2)*4);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R6, BGBCC_SH_REG_SP, ((k-j)+0)*4);
				n+=2;
			}else
				if(sctx->has_pushx2)
			{
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVX2,
					BGBCC_SH_REG_R18, BGBCC_SH_REG_SP, (k-j)*4);
				n++;
			}else
			{
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R19, BGBCC_SH_REG_SP, ((k-j)+2)*4);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R18, BGBCC_SH_REG_SP, ((k-j)+0)*4);
				n+=2;
			}
		}

		pn=BGBCC_JX2C_EmitFrameProlog_GenBestPerm(rja, perm);
		
#if 1
//		for(i=31; i>0; i--)
//		for(i=63; i>0; i--)
//		for(i0=63; i0>0; i0--)
		for(i0=0; i0<pn; i0++)
		{
//			if(!(sctx->has_xgpr&1) && (i>=32))
//				continue;
//			if(!(sctx->has_bjx1egpr) && (i>=16))
//				continue;
//			if(i==15)
//				continue;

//			if(!(sctx->reg_vsave&((2ULL<<i)-1)))
//				break;

//			i=i0^2;
			i=perm[i0];

			if(rja[i]<0)
				continue;

			if(sctx->has_pushx2 && (i&1))
//			if(0)
			{
				if((sctx->reg_vsave&(3ULL<<(i-1)))==(3ULL<<(i-1)))
				{
					sctx->reg_save|=(3ULL<<(i-1));
					j=rja[i];
					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVX2,
						BGBCC_SH_REG_R0+(i-1), BGBCC_SH_REG_SP, (k-j)*4);
					n++;
					i--;
					continue;
				}
			}

			if(sctx->reg_vsave&(1ULL<<i))
			{
				j=rja[i];
				sctx->reg_save|=(1ULL<<i);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R0+i, BGBCC_SH_REG_SP, (k-j)*4);
				n++;
			}
		}
#endif
		
		if(n>=6)
			fl2|=1;

		if(rfl2)
			*rfl2=fl2;

		return(k);
	}

	BGBCC_DBGBREAK

	return(k);
}

ccxl_status BGBCC_JX2C_TinyLeafProlog_ReserveReg(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, ccxl_register reg)
{
	int csreg;

	if(!reg.val)
		return(0);
	
	if(reg.val==CCXL_REGID_REG_Z)
		return(0);
	if(reg.val==CCXL_REGID_REG_DZ)
		return(0);

	csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, reg);
	
	if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
	{
		obj->regflags|=BGBCC_REGFL_HYBLEAFTINY;
	}else
	{
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, reg);
	}
	
	return(0);
}

ccxl_status BGBCC_JX2C_TinyLeafProlog_ReserveVopRegs(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, BGBCC_CCXL_VirtOp *op)
{
	BGBCC_JX2C_TinyLeafProlog_ReserveReg(ctx, sctx, obj, op->srca);
	BGBCC_JX2C_TinyLeafProlog_ReserveReg(ctx, sctx, obj, op->srcb);
	BGBCC_JX2C_TinyLeafProlog_ReserveReg(ctx, sctx, obj, op->srcc);
	BGBCC_JX2C_TinyLeafProlog_ReserveReg(ctx, sctx, obj, op->srcd);
	BGBCC_JX2C_TinyLeafProlog_ReserveReg(ctx, sctx, obj, op->dst);
	return(0);
}

ccxl_status BGBCC_JX2C_Prolog_VopSetupReturnLabel(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, BGBCC_CCXL_VirtOp *op)
{
//	sctx->lbl_ret=BGBCC_JX2_GenLabelTemp(sctx);
//	sctx->lbl_ret_zero=0;

	if(
		(op->opn==CCXL_VOP_JMP) ||
		(op->opn==CCXL_VOP_JCMP_ZERO) ||
		(op->opn==CCXL_VOP_JCMP) )
	{
		if(op->imm.si==CCXL_LBL_RETURN_ZERO)
		{
			if(!sctx->lbl_ret_zero)
			{
//				sctx->lbl_ret_zero=BGBCC_JX2_GenLabelTemp(sctx);
				sctx->lbl_ret_zero=BGBCC_JX2_GenLabel(sctx);
				obj->t_lbl_ret_z=sctx->lbl_ret_zero;
			}
		}
	}
	return(0);
}

int BGBCC_JX2C_EmitFrameProlog_TinyLeaf(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, int fcnlbl)
{
	BGBCC_CCXL_VirtOp	*vop;
	ccxl_register reg, treg;
	ccxl_type tty;
	int tr0, tr1, ref0, ref1;
	int i, j, k, bo, ni, nf, vaix, rcls;

	sctx->is_leaftiny=3;

	BGBCC_JX2_EmitLabel(sctx, fcnlbl);

	sctx->cur_fcnbase=BGBCC_JX2_EmitGetOffs(sctx);

	ref0=BGBCC_JX2_EmitGetOffs(sctx);

	BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);

	sctx->cur_fpscr=0;
	sctx->dfl_fpscr=0;

	sctx->reg_save=0;
	sctx->freg_save=0;
	
	sctx->sreg_held=0x00F000F000FC00F4ULL;
	sctx->sfreg_held=0x00000FF0ULL;

	sctx->sreg_live=sctx->sreg_held;
	sctx->sfreg_live=sctx->sfreg_held;

	bo=BGBCC_JX2_EmitGetOffs(sctx);

	sctx->regalc_save=0;
	sctx->regalc_live=0;
	sctx->regalc_dirty=0;

	sctx->fregalc_save=0;
	sctx->fregalc_live=0;
	sctx->fregalc_dirty=0;

	sctx->sreg_held=0x0000;
	sctx->sfreg_held=0x0000;
	sctx->sreg_live=sctx->sreg_held;
	sctx->sfreg_live=sctx->sfreg_held;

	sctx->vsp_rsv=0;

	sctx->lbl_ret=obj->t_lbl_ret;
	sctx->lbl_ret_zero=obj->t_lbl_ret_z;

	if(sctx->lbl_ret<=0)
	{
//		sctx->lbl_ret=BGBCC_JX2_GenLabelTemp(sctx);
		sctx->lbl_ret=BGBCC_JX2_GenLabel(sctx);
		obj->t_lbl_ret=sctx->lbl_ret;
//		sctx->lbl_ret_zero=0;
	}

	for(i=0; i<obj->n_vop; i++)
	{
		vop=obj->vop[i];
		BGBCC_JX2C_Prolog_VopSetupReturnLabel(
			ctx, sctx, obj, vop);
	}


	for(i=0; i<obj->n_locals; i++)
	{
		if(obj->locals[i]->regflags&BGBCC_REGFL_CULL)
			continue;

		if(BGBCC_CCXL_TypeValueObjectP(ctx, obj->locals[i]->type))
		{
//			obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;
		}

		if(obj->locals[i]->regflags&BGBCC_REGFL_ALIASPTR)
		{
			obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;
		}
		
		if(BGBCC_CCXL_TypeArrayP(ctx, obj->locals[i]->type))
		{
			obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;
		}
	}

	ni=0; nf=0; vaix=-1;
	for(i=0; i<obj->n_args; i++)
	{
		if(BGBCC_CCXL_TypeVarArgsP(ctx, obj->args[i]->type))
			{ vaix=i; continue; }

		tty=obj->args[i]->type;
		treg.val=CCXL_REGTY_ARG|
			(((u64)tty.val)<<CCXL_REGID_TYPESHIFT)|i;
//		pr0=BGBCC_JX2C_GetVRegPriority(ctx, sctx, treg);
	
		rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->args[i]->type);
		switch(rcls)
		{
		case BGBCC_SH_REGCLS_GR:
			if((ni>=4) && !sctx->has_bjx1egpr)
				break;
//			if(ni>=8)
			if((ni>=8) && !(sctx->has_xgpr&2))
				break;
			if(ni>=16)
				break;

			k=obj->args[i]->fxoffs;

#if 1
			tr0=BGBCC_JX2C_GetRdRegForArgumentIndex(ctx, sctx, ni);
			if(tr0>0)
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, tr0);
			}
#endif

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);

//			if(	sctx->regalc_live || sctx->fregalc_live ||
//				sctx->sreg_live || sctx->sfreg_live)
//					{ BGBCC_DBGBREAK }

			ni++;
			break;

		case BGBCC_SH_REGCLS_VO_GR:
			obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;

			if((ni>=4) && !sctx->has_bjx1egpr)
				break;
//			if(ni>=8)
//				break;
			if((ni>=8) && !(sctx->has_xgpr&2))
				break;
			if(ni>=16)
				break;
			k=obj->args[i]->fxoffs;

#if 1
			tr0=BGBCC_JX2C_GetRdRegForArgumentIndex(ctx, sctx, ni);
			if(tr0>0)
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, tr0);
			}
#endif

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);

			ni++;
			break;

		case BGBCC_SH_REGCLS_VO_REF:
		case BGBCC_SH_REGCLS_AR_REF:
//			obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;

		case BGBCC_SH_REGCLS_QGR:
			if((ni>=4) && !sctx->has_bjx1egpr)
				break;
//			if(ni>=8)
//				break;
			if((ni>=8) && !(sctx->has_xgpr&2))
				break;
			if(ni>=16)
				break;

			k=obj->args[i]->fxoffs;

#if 1
			tr0=BGBCC_JX2C_GetRqRegForArgumentIndex(ctx, sctx, ni);
			if(tr0>0)
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, tr0);
			}
#endif

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);

			ni++;
			break;

		case BGBCC_SH_REGCLS_VO_QGR:
			obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;

			if((ni>=4) && !sctx->has_bjx1egpr)
				break;
//			if(ni>=8)
//				break;
			if((ni>=8) && !(sctx->has_xgpr&2))
				break;
			if(ni>=16)
				break;

			k=obj->args[i]->fxoffs;

#if 1
			tr0=BGBCC_JX2C_GetRqRegForArgumentIndex(ctx, sctx, ni);
			if(tr0>0)
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, tr0);
			}
#endif

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);

			ni++;
			break;

		case BGBCC_SH_REGCLS_GR2:
		case BGBCC_SH_REGCLS_QGR2:
		case BGBCC_SH_REGCLS_VO_GR2:
		case BGBCC_SH_REGCLS_VO_QGR2:
		case BGBCC_SH_REGCLS_VO_REF2:
		case BGBCC_SH_REGCLS_AR_REF2:
			obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;
			break;

		default:
			BGBCC_DBGBREAK
			break;
		}
	}

	tty=BGBCC_CCXL_GetTypeReturnType(ctx, obj->type);
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, tty);

	if(	(rcls==BGBCC_SH_REGCLS_VO_REF)	||
		(rcls==BGBCC_SH_REGCLS_VO_REF2)	||
		(rcls==BGBCC_SH_REGCLS_VO_QGR2)	||
		(rcls==BGBCC_SH_REGCLS_VO_GR2)	||
		(rcls==BGBCC_SH_REGCLS_GR2)		||
		(rcls==BGBCC_SH_REGCLS_QGR2)	)
		{ obj->regflags|=BGBCC_REGFL_NOTLEAFTINY; }

	if(sctx->frm_offs_retstr)
		{ obj->regflags|=BGBCC_REGFL_NOTLEAFTINY; }
	if(sctx->frm_offs_thisptr)
		{ obj->regflags|=BGBCC_REGFL_NOTLEAFTINY; }

	sctx->is_leaftiny=1;

//	obj->regflags|=BGBCC_REGFL_HYBLEAFTINY;

	if(!(obj->regflags&BGBCC_REGFL_HYBLEAFTINY))
	{
		for(i=0; i<obj->n_vop; i++)
		{
			vop=obj->vop[i];
			BGBCC_JX2C_TinyLeafProlog_ReserveVopRegs(
				ctx, sctx, obj, vop);
		}
		
		if(obj->regflags&BGBCC_REGFL_HYBLEAFTINY)
		{
//			sctx->is_leaftiny=0;
//			BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);

//			sctx->is_leaftiny=3;
			BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
		}
	}

//	if(	sctx->regalc_live || sctx->fregalc_live ||
//		sctx->sreg_live || sctx->sfreg_live)
//			{ BGBCC_DBGBREAK }

	BGBCC_JX2C_EmitSyncProlog(ctx, sctx);

	sctx->is_leaftiny=1;

	ref1=BGBCC_JX2_EmitGetOffs(sctx);
//	if(!sctx->is_simpass)
//		{ sctx->stat_size_prolog+=ref1-ref0; }
	
	sctx->fnsz_pro=ref1-ref0;

	return(0);
}


int BGBCC_JX2C_EmitFrameProlog(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, int fcnlbl)
{
	char tb[256];
	ccxl_register reg, treg;
	ccxl_register reg1;
	ccxl_type tty;
	BGBCC_CCXL_VirtOp *vop;
	BGBCC_JX2_VarSpan *vsp, *vsp1;
	BGBCC_JX2_VarSpan2 *vspb;
	int bo, co, pr0;
	int p0, vaix, cnt, maxrsv, ismaxrsv, maxvalidrsv;
	int ni, nf, rcls, ob, ov;
	u64 epik;
	int epix, epilbl, epij;
	int tr0, tr1, ref0, ref1, l0, l1, sz;
	int i, j, k, l, fl, fl2;

	ctx->cur_func=obj;
	ctx->cur_vtr=NULL;

	reg1=sctx->regalc_map[0];
	if(((s64)reg1.val)<0)
	{
		BGBCC_DBGBREAK
	}

	if(sctx->frm_offs_thisptr)
	{
		obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;
	}

//	if(obj->n_args>8)
	if((obj->n_args>8) || (obj->regflags&BGBCC_REGFL_TEMPLOAD))
	{
		obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;
	}

	if(obj->regflags&BGBCC_REGFL_NOTLEAFTINY)
		obj->regflags&=~BGBCC_REGFL_LEAFTINY;
	sctx->is_leaftiny=0;

	if(obj->regflags&BGBCC_REGFL_LEAFTINY)
	{
		i=BGBCC_JX2C_EmitFrameProlog_TinyLeaf(ctx, sctx, obj, fcnlbl);
		return(i);
	}

	ref0=BGBCC_JX2_EmitGetOffs(sctx);

#if 1
	sctx->reg_save=sctx->reg_vsave;
	sctx->freg_save=sctx->freg_vsave;

	//	if((sctx->is_leaf&1) && ((sctx->reg_save&0x7E00)==0x7E00))
	//		sctx->is_leaf=0;

	if(sctx->is_leaf&1)
	{
		if((sctx->reg_save&0x7E00)==0x7E00)
			sctx->is_leaf=0;

		if(((sctx->reg_save&0x7800)==0x7800) &&
			((sctx->freg_save&0xC000)==0xC000))
				sctx->is_leaf=0;
		if(((sctx->reg_save&0x7000)==0x7000) &&
			((sctx->freg_save&0xE000)==0xE000))
				sctx->is_leaf=0;
		if(((sctx->reg_save&0x6000)==0x6000) &&
			((sctx->freg_save&0xF000)==0xF000))
				sctx->is_leaf=0;
	}

	BGBCC_JX2C_CalcFrameEpiKey(ctx, sctx, obj, 1, &epik, &epix);
//	epix=(((((u64)epik)+1)*2147483647LL)>>32)&1023;
	epilbl=sctx->eprhash_lbl[epix];
	
	if(sctx->eprhash_key[epix]!=epik)
		epilbl=0;
	
	epij=0;
	if(epilbl>0)
	{
//		epij=BGBCC_JX2_EmitCheckAutoLabelNear16B(sctx, epilbl, 
		epij=BGBCC_JX2_EmitCheckAutoLabelNear20B(sctx, epilbl, 
			BGBCC_SH_NMID_BSR);
		if(epij<=0)epilbl=0;
	}

	if(!sctx->is_simpass && epik && (epilbl<=0))
	{
		epij=3;

		l=sctx->eprhash_rov++;
//		sprintf(tb, "__prolog.%04X.%016llX", l, epik);
		sprintf(tb, "__prolog_%04X_%012llX", l, epik);

//		epilbl=BGBCC_JX2_GenLabel(sctx);
		epilbl=BGBCC_JX2_GetNamedLabel(sctx, tb);
		sctx->eprhash_key[epix]=epik;
		sctx->eprhash_lbl[epix]=epilbl;

		if(sctx->is_align_wexj&2)
			{ BGBCC_JX2_EmitPad32AlignLastOp(sctx); }

		BGBCC_JX2_EmitLabel(sctx, epilbl);

		fl=1;
		if(sctx->is_pbo && !(sctx->is_pbo&2) &&
			(obj->regflags&BGBCC_REGFL_ALIASPTR))
		{
//			fl=0;
			fl=2;
		}

#if 1
		fl2=0;
		k=BGBCC_JX2C_EmitFrameProlog_PushRegs(ctx, sctx, fl, &fl2);

//		if(1)
		if(fl2&1)
		{
//			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTSN);
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTSU);
//			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
		}else
		{
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);
//			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
		}
#endif
	}
#endif

	if(sctx->is_align_wexj&2)
		{ BGBCC_JX2_EmitPad32AlignLastOp(sctx); }

	BGBCC_JX2_EmitLabel(sctx, fcnlbl);

	sctx->cur_fcnbase=BGBCC_JX2_EmitGetOffs(sctx);

	BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);

	sctx->cur_fpscr=0;
	sctx->dfl_fpscr=0;

	sctx->reg_save=0;
	sctx->freg_save=0;
	
	sctx->sreg_held=0x00F000F000FC00F4ULL;
	sctx->sfreg_held=0x00000FF0ULL;

	sctx->sreg_live=sctx->sreg_held;
	sctx->sfreg_live=sctx->sfreg_held;

	bo=BGBCC_JX2_EmitGetOffs(sctx);

#if 1
	sctx->regalc_save=0;
	sctx->regalc_live=0;
	sctx->regalc_dirty=0;

	sctx->fregalc_save=0;
	sctx->fregalc_live=0;
	sctx->fregalc_dirty=0;

	sctx->sreg_held=0x0000;
	sctx->sfreg_held=0x0000;
	sctx->sreg_live=sctx->sreg_held;
	sctx->sfreg_live=sctx->sfreg_held;

	sctx->lbl_ret=obj->t_lbl_ret;
	sctx->lbl_ret_zero=obj->t_lbl_ret_z;

	if(sctx->lbl_ret<=0)
	{
//		sctx->lbl_ret=BGBCC_JX2_GenLabelTemp(sctx);
		sctx->lbl_ret=BGBCC_JX2_GenLabel(sctx);
//		sctx->lbl_ret_zero=0;
	}

	for(i=0; i<obj->n_vop; i++)
	{
		vop=obj->vop[i];
		BGBCC_JX2C_Prolog_VopSetupReturnLabel(
			ctx, sctx, obj, vop);
	}

	if(sctx->cnt_set_fp64>sctx->cnt_set_fp32)
//	if(0)
	{
		sctx->dfl_fpscr=BGBCC_SH_FPSCR_PR;
	}else
	{
		sctx->dfl_fpscr=0;
	}

	reg1=sctx->regalc_map[0];
	if(((s64)reg1.val)<0)
	{
		BGBCC_DBGBREAK
	}

//	sctx->cur_fpscr=0x1000;	
//	BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);

//	sctx->cur_fpscr|=0x1000;	
//	sctx->cur_fpscr=1;
//	BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
	
//	sctx->vsp_rsv=0;
//	sctx->vsp_rsv=2;
//	sctx->vsp_rsv=3;
	sctx->vsp_rsv=4;
	
//	if(sctx->use_egpr)
	if(sctx->use_egpr&3)
	{
//		sctx->vsp_rsv+=3;
//		sctx->vsp_rsv+=4;
		sctx->vsp_rsv+=5;
		
		if(ctx->cur_func->regflags&BGBCC_REGFL_HAS128)
			sctx->vsp_rsv-=2;
	}

//	if(sctx->has_xgpr&2)
//		sctx->vsp_rsv+=11;
	
	if(!(sctx->is_simpass&64))
	{
		if(	!(ctx->cur_func->regflags&BGBCC_REGFL_NOSCRATCHDYN) &&
			!(ctx->cur_func->regflags&BGBCC_REGFL_NOTLEAF))
		{
			if(sctx->emit_riscv&0x11)
			{
				sctx->vsp_rsv+=20;
			}else
			{
	//			if(sctx->use_egpr)
				if(sctx->use_egpr&3)
					{ sctx->vsp_rsv+=8; }
				else
					{ sctx->vsp_rsv+=3; }

				if((sctx->has_xgpr&2) || (sctx->use_egpr&2))
					{ sctx->vsp_rsv+=12; }
			}
		}
	}
	
#if 1
	if(!(sctx->has_xgpr&2))
	{
		if(sctx->use_egpr&3)
		{
			if(sctx->vsp_rsv>8)
			{
				sctx->vsp_rsv=8;
			}
		}else
		{
			if(sctx->vsp_rsv>4)
			{
				sctx->vsp_rsv=4;
			}
		}
	}
#endif

	if(sctx->emit_riscv&0x33)
	{
		/* RISC-V and XG2RV */
		sctx->vsp_rsv=8;

		if(ctx->cur_func->regflags&BGBCC_REGFL_HAS128)
			sctx->vsp_rsv-=2;

		if((sctx->has_xgpr&2) || (sctx->use_egpr&2))
			{ sctx->vsp_rsv+=12; }
	}


#if 1
	maxvalidrsv=sctx->vspan_num;
	for(i=0; i<sctx->vspan_num; i++)
	{
		reg=sctx->vspan[i]->reg;
		k=BGBCC_JX2C_GetFrameVRegFlags(ctx, sctx, reg);
		if(k&BGBCC_REGFL_ALIASPTR)
			{ maxvalidrsv=i; break; }
//		if(k&BGBCC_REGFL_TEMPLOAD)
		if(BGBCC_CCXL_IsRegArgP(ctx, reg) && (k&BGBCC_REGFL_TEMPLOAD))
			{ maxvalidrsv=i; break; }

		if(sctx->vspan[i]->flag&BGBCC_RSPFL_NONBASIC)
			{ maxvalidrsv=i; break; }
		if(sctx->vspan[i]->flag&BGBCC_RSPFL_ALIASPTR)
			{ maxvalidrsv=i; break; }

		if(sctx->vspan[i]->flag&BGBCC_RSPFL_NONLOCAL)
		{
			if(BGBCC_CCXL_IsRegImmP(ctx, reg))
//			if(BGBCC_CCXL_IsRegImmSmallLongP(ctx, reg))
			{
				continue;
			}
		
#if 1
			if(BGBCC_CCXL_IsRegGlobalP(ctx, reg) &&
				(sctx->is_leaf&1) &&
//				!(sctx->vspan[i]->flag&BGBCC_RSPFL_GBLSTORE) &&
				!(ctx->cur_func->regflags&BGBCC_REGFL_NOTLEAF))
			{
				continue;
			}
#endif

#if 1
			if(BGBCC_CCXL_IsRegGlobalP(ctx, reg) &&
				!BGBCC_CCXL_IsRegGlobalFunctionP(ctx, reg) &&
				!!BGBCC_CCXL_IsRegAliasedP(ctx, reg) &&
				(ctx->cur_func->regflags&BGBCC_REGFL_REC_NOGBLSTORE))
			{
				continue;
			}
#endif

//			if(BGBCC_CCXL_IsRegImmP(ctx, reg))
//			{
//				continue;
//			}

			maxvalidrsv=i; break;
		}
	}
#endif

	/*
		If number of registers needed is less than what is available,
		Try to static assign everything.
	 */
//	maxrsv=7;
	maxrsv=4;
	if(sctx->use_bp)maxrsv--;
	if(sctx->is_pic)maxrsv--;
//	if(sctx->use_egpr)
	if(sctx->use_egpr&3)
		maxrsv+=8;

//	if(sctx->has_xgpr&2)
//	if((sctx->has_xgpr&2) ||
//		((sctx->has_xgpr&1) && (sctx->use_egpr&2)))
	if((sctx->has_xgpr&2) || (sctx->use_egpr&2))
	{
		maxrsv+=14;
	}

	if(sctx->emit_riscv&0x33)
	{
		/* RISC-V and XG2RV */
//		maxrsv=8;
		maxrsv=10;

//		if(ctx->cur_func->regflags&BGBCC_REGFL_HAS128)
//			maxrsv-=2;

		if((sctx->has_xgpr&2) || (sctx->use_egpr&2))
			{ maxrsv+=12; }
	}

	ismaxrsv=0;

	if(	(sctx->vspan_num<=maxrsv) &&
		(sctx->vspan_num==maxvalidrsv))
	{
		sctx->vsp_rsv=sctx->vspan_num;
		ismaxrsv=1;
	}else if(sctx->vspan_num<=maxrsv)
	{
		ismaxrsv=0;

#if 1
		for(i=maxvalidrsv; i<sctx->vspan_num; i++)
		{
			reg=sctx->vspan[i]->reg;
//			if(!BGBCC_CCXL_IsRegImmP(ctx, reg))
//				break;

			if(BGBCC_CCXL_IsRegImmP(ctx, reg))
				continue;

#if 1
			if(BGBCC_CCXL_IsRegGlobalP(ctx, reg) &&
				!BGBCC_CCXL_IsRegAliasedP(ctx, reg) &&
				(ctx->cur_func->regflags&BGBCC_REGFL_REC_NOGBLSTORE))
					continue;
#endif

			if(BGBCC_CCXL_IsRegGlobalP(ctx, reg) &&
				(sctx->is_leaf&1) &&
				!(ctx->cur_func->regflags&BGBCC_REGFL_NOTLEAF))
					continue;

			break;

//			if(ctx->cur_func->regflags&BGBCC_REGFL_IMMLOAD)
//				break;
		}

		ismaxrsv=0;
		
		if(i==sctx->vspan_num)
		{
//			sctx->vsp_rsv=sctx->vspan_num;
			ismaxrsv=1;
		}
#endif
	}else
	{
		ismaxrsv=0;
	}
	
	if(sctx->use_bp)sctx->vsp_rsv--;
	if(sctx->is_pic)sctx->vsp_rsv--;
	if(sctx->vsp_rsv<0)
		sctx->vsp_rsv=0;

	if(sctx->vspan_num<sctx->vsp_rsv)
		sctx->vsp_rsv=sctx->vspan_num;

	if(maxvalidrsv<sctx->vsp_rsv)
		sctx->vsp_rsv=maxvalidrsv;

	reg1=sctx->regalc_map[0];
	if(((s64)reg1.val)<0)
	{
		BGBCC_DBGBREAK
	}

	cnt=0;
	for(i=0; i<sctx->vsp_rsv; i++)
	{
		cnt+=sctx->vspan[i]->cnt;
		reg=sctx->vspan[i]->reg;

#if 1
		vsp=sctx->vspan[i];
		for(j=0; j<vsp->nseq; j++)
		{
			vspb=vsp->seq+j;
			if(vspb->flag&BGBCC_RSPFL_ISCALLARG)
			{
				vspb->flag&=~BGBCC_RSPFL_ISCALLARG;
				vspb->flag|=BGBCC_RSPFL_ISSOURCE;
			}
		}
#endif

#if 1
//		if((cnt<(sctx->vsp_tcnt/8)) && !(ismaxrsv>0))
		if((cnt>((sctx->vsp_tcnt-cnt)*32)) && !(ismaxrsv>0))
//		if((cnt<(sctx->vsp_tcnt/32)) && !(ismaxrsv>0))
//		if((cnt<((sctx->vsp_tcnt*8)>>8)) && !(ismaxrsv>0))
//		if((cnt<((sctx->vsp_tcnt*6)>>8)) && !(ismaxrsv>0))
			{ sctx->vsp_rsv=i; break; }
#endif

		k=BGBCC_JX2C_GetFrameVRegFlags(ctx, sctx, reg);

//		if(BGBCC_CCXL_IsRegArgP(ctx, reg) && (obj->n_args>8))
		if(BGBCC_CCXL_IsRegArgP(ctx, reg) && (k&BGBCC_REGFL_TEMPLOAD))
			{ sctx->vsp_rsv=i; break; }
	}
#endif

//	sctx->vsp_rsv=0;

	if(sctx->vspan_num!=sctx->vsp_rsv)
	{
		if(ctx->cur_func->regflags&BGBCC_REGFL_HAS128)
		{
			if(sctx->vsp_rsv>(maxrsv-6))
				sctx->vsp_rsv=(maxrsv-6);
		}else
		{
			if(sctx->vsp_rsv>(maxrsv-3))
				sctx->vsp_rsv=(maxrsv-3);
		}
	}

	if(!sctx->is_simpass)
	{
		if(sctx->vspan_num==sctx->vsp_rsv)
		{
			sctx->stat_func_maxrsv++;
		}else
		{
			sctx->stat_func_partrsv++;
			sctx->stat_func_acc_vsprsv+=sctx->vsp_rsv;
			sctx->stat_func_acc_vspan +=sctx->vspan_num;
			sctx->stat_func_acc_vspval+=maxvalidrsv;
			sctx->stat_func_acc_vspmax+=maxrsv;
		}
	}

	k=0;

	reg1=sctx->regalc_map[0];
	if(((s64)reg1.val)<0)
	{
		BGBCC_DBGBREAK
	}

//	j=BGBCC_JX2_EmitCheckAutoLabelNear16B(sctx, epilbl);
//	epij=BGBCC_JX2_EmitCheckAutoLabelNear16B(sctx, epilbl);
	if(epik && (epilbl>0) && (epij>0))
	{
#if 1
//		BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_STSL,
//			BGBCC_SH_REG_PR, BGBCC_SH_REG_SP);
		if(sctx->is_pbo && !(sctx->is_pbo&2) &&
			(obj->regflags&BGBCC_REGFL_ALIASPTR))
		{
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_PR);
			k+=4;

			if(sctx->emit_riscv&0x11)
			{
				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_PR, BGBCC_SH_REG_R7);
				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_GBR, BGBCC_SH_REG_R6);
			}else
			{
				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_PR, BGBCC_SH_REG_R19);
				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_GBR, BGBCC_SH_REG_R18);
			}

		}else
		{
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//				BGBCC_SH_REG_PR, BGBCC_SH_REG_R16);
//				BGBCC_SH_REG_PR, BGBCC_SH_REG_R1);
				BGBCC_SH_REG_PR, BGBCC_JX2CC_PSREG_ALTLR);
//			k+=sctx->is_addr64?2:1;
			k+=2;
		}
#endif

//		if((j&3)==3)
		if(0)
//		if((epij&3)>1)
		{
			BGBCC_JX2_EmitOpLabel(sctx,
				BGBCC_SH_NMID_BSR, epilbl);
//			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);

		}else
		{
//			BGBCC_JX2_EmitOpFar16Label(sctx,
			BGBCC_JX2_EmitOpFar20Label(sctx,
				BGBCC_SH_NMID_BSRN, epilbl);
		}

#if 1
//		if(sctx->is_pbo)
		if(sctx->is_pbo && !(sctx->is_pbo&2) &&
			(obj->regflags&BGBCC_REGFL_ALIASPTR))
		{
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_GBR);
//			k+=sctx->is_addr64?2:1;

			j=BGBCC_JX2_GetNamedLabel(sctx, "__global_ptr");
//			BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
//				BGBCC_SH_NMID_MOVQ,
//				BGBCC_SH_REG_R1, j);
//			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//				BGBCC_SH_REG_R1, BGBCC_SH_REG_GBR);

//			BGBCC_JX2_EmitRelocTy(sctx, j, BGBCC_SH_RLC_TBR12_BJX);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx,
				BGBCC_SH_NMID_MOVQ,
//				BGBCC_SH_REG_GBR, 0, BGBCC_SH_REG_R18);
				BGBCC_SH_REG_GBR, 0, BGBCC_JX2CC_PSREG_TS0);

#if 1
			if(sctx->use_wexmd)
			{
				BGBCC_JX2_EmitOpImm(sctx,
					BGBCC_SH_NMID_WEXMD, sctx->use_wexmd);
			}
#endif

			if(sctx->has_fmovc)
			{
				if(sctx->is_exe&1)
//				if(0)
				{
					BGBCC_JX2_EmitOpLdRegDispReg(sctx,
						BGBCC_SH_NMID_MOVC,
//						BGBCC_SH_REG_R18, 0, BGBCC_SH_REG_GBR);
						BGBCC_JX2CC_PSREG_TS0, 0, BGBCC_SH_REG_GBR);
				}else
				{
					BGBCC_JX2_EmitRelocTy(sctx, j, BGBCC_SH_RLC_TBR12_BJX);
					BGBCC_JX2_EmitOpLdRegDispReg(sctx,
						BGBCC_SH_NMID_MOVC_DISP24,
//						BGBCC_SH_REG_R18, 0, BGBCC_SH_REG_GBR);
						BGBCC_JX2CC_PSREG_TS0, 0, BGBCC_SH_REG_GBR);
				}
			}else
			{
				if(sctx->is_exe&1)
//				if(0)
				{
					BGBCC_JX2_EmitOpLdRegDispReg(sctx,
						BGBCC_SH_NMID_MOVQ,
//						BGBCC_SH_REG_R18, 0, BGBCC_SH_REG_R18);
						BGBCC_JX2CC_PSREG_TS0, 0, BGBCC_JX2CC_PSREG_TS0);
				}else
				{
					BGBCC_JX2_EmitRelocTy(sctx, j, BGBCC_SH_RLC_TBR12_BJX);
					BGBCC_JX2_EmitOpLdRegDispReg(sctx,
						BGBCC_SH_NMID_MOVQ_DISP24,
//						BGBCC_SH_REG_R18, 0, BGBCC_SH_REG_R18);
						BGBCC_JX2CC_PSREG_TS0, 0, BGBCC_JX2CC_PSREG_TS0);
				}

				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//					BGBCC_SH_REG_R18, BGBCC_SH_REG_GBR);
					BGBCC_JX2CC_PSREG_TS0, BGBCC_SH_REG_GBR);
			}
		}else
		{
#if 1
			if(obj->regflags&BGBCC_REGFL_ALIASPTR)
			{
				if(sctx->use_wexmd)
				{
					BGBCC_JX2_EmitOpImm(sctx,
						BGBCC_SH_NMID_WEXMD, sctx->use_wexmd);
				}
			}
#endif
		}
#endif

//		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);

#if 1
	//	for(i=14; i>0; i--)
//		for(i=31; i>0; i--)
		for(i=63; i>0; i--)
		{
#if 0
			if(!(sctx->has_xgpr&1) && (i>=32))
				continue;
			if(!(sctx->has_bjx1egpr) && (i>=16))
				continue;
#endif
			if(i==15)
				continue;
			if(!(sctx->reg_vsave&((2ULL<<i)-1)))
				break;
			if(sctx->reg_vsave&(1ULL<<i))
				{ sctx->reg_save|=(1ULL<<i); k+=sctx->is_addr64?2:1; }
		}
//		if(sctx->use_fpr)
//			{ k+=sctx->is_addr64?2:1; }
//		for(i=15; i>0; i--)
		for(i=63; i>0; i--)
		{
//			if(!(sctx->has_xgpr&1) && (i>=32))
//				continue;
			if(sctx->freg_vsave&(1ULL<<i))
				{ sctx->freg_save|=(1ULL<<i); k+=2; }
		}
#endif
	}
	else
		if(	!(obj->flagsint&BGBCC_TYFL_INTERRUPT) &&
			!(obj->flagsint&BGBCC_TYFL_SYSCALL))
//		if(0)
	{
		fl=0;

#if 1
		if(sctx->is_leaf&1)
		{
			if(sctx->is_pbo && !(sctx->is_pbo&2) &&
				(obj->regflags&BGBCC_REGFL_ALIASPTR))
			{
				fl=1;
				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_GBR, BGBCC_JX2CC_PSREG_ALTLR);
			}else
			{
				fl=0;
			}
		}else
			if(sctx->is_pbo && !(sctx->is_pbo&2) &&
				(obj->regflags&BGBCC_REGFL_ALIASPTR))
		{
			fl=2;
			if(sctx->emit_riscv&0x11)
			{
				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_PR, BGBCC_SH_REG_R7);
				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_GBR, BGBCC_SH_REG_R6);
			}else
			{
				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_PR, BGBCC_SH_REG_R19);
				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_GBR, BGBCC_SH_REG_R18);
			}
		}else
		{
			/* ALTLR Ref 3 */
			if(sctx->emit_riscv&0x33)
//			if(0)
			{
				fl=8;
			}else
			{
				fl=1;
				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_PR, BGBCC_JX2CC_PSREG_ALTLR);
			}
		}
#endif

		if((obj->regflags&BGBCC_REGFL_ALIASPTR) && !(sctx->is_pbo&2))
		{
			if(sctx->use_wexmd)
			{
				BGBCC_JX2_EmitOpImm(sctx,
					BGBCC_SH_NMID_WEXMD, sctx->use_wexmd);
			}
		}

		fl2=0;
		k+=BGBCC_JX2C_EmitFrameProlog_PushRegs(ctx, sctx, fl, &fl2);

//		if(sctx->is_pbo)
		if(sctx->is_pbo && !(sctx->is_pbo&2) &&
			(obj->regflags&BGBCC_REGFL_ALIASPTR))
		{
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_GBR);
//			k+=sctx->is_addr64?2:1;

			j=BGBCC_JX2_GetNamedLabel(sctx, "__global_ptr");
			if(sctx->has_fmovc&1)
			{
				BGBCC_JX2_EmitOpLdRegDispReg(sctx,
					BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_GBR, 0, BGBCC_JX2CC_PSREG_TS0);

				if(sctx->is_exe&1)
				{
					BGBCC_JX2_EmitOpLdRegDispReg(sctx,
						BGBCC_SH_NMID_MOVC,
						BGBCC_JX2CC_PSREG_TS0, 0, BGBCC_SH_REG_GBR);
				}else
				{
					BGBCC_JX2_EmitRelocTy(sctx, j, BGBCC_SH_RLC_TBR12_BJX);
					BGBCC_JX2_EmitOpLdRegDispReg(sctx,
						BGBCC_SH_NMID_MOVC_DISP24,
						BGBCC_JX2CC_PSREG_TS0, 0, BGBCC_SH_REG_GBR);
				}
			}else
			{
				if(sctx->is_exe&1)
				{
					BGBCC_JX2_EmitOpLdRegDispReg(sctx,
						BGBCC_SH_NMID_MOVQ,
						BGBCC_SH_REG_GBR, 0, BGBCC_JX2CC_PSREG_TS0);
					BGBCC_JX2_EmitOpLdRegDispReg(sctx,
						BGBCC_SH_NMID_MOVQ,
						BGBCC_JX2CC_PSREG_TS0, 0, BGBCC_JX2CC_PSREG_TS0);
					BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
						BGBCC_JX2CC_PSREG_TS0, BGBCC_SH_REG_GBR);
				}else
				{
					BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
						BGBCC_SH_NMID_MOVQ,
						BGBCC_JX2CC_PSREG_ALTLR, j);
					BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
						BGBCC_JX2CC_PSREG_ALTLR, BGBCC_SH_REG_GBR);
				}
			}
		}
	}
	else
	{
#if 1
		if(obj->flagsint&BGBCC_TYFL_INTERRUPT)
		{
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_PR);
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_GBR);
			k+=8;

			if(sctx->has_fmovc)
			{
				BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP4B);
				BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP4B);

				BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
					-32, BGBCC_SH_REG_SP);

				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVC,
					BGBCC_SH_REG_EXSR, BGBCC_SH_REG_SP, 24);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVC,
					BGBCC_SH_REG_SPC, BGBCC_SH_REG_SP, 16);

				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVC,
					BGBCC_SH_REG_PR, BGBCC_SH_REG_SP, 8);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVC,
					BGBCC_SH_REG_GBR, BGBCC_SH_REG_SP, 0);
			}
			else
			{
				BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP4B);

				BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
					-40, BGBCC_SH_REG_SP);

				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
//					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 0);
					BGBCC_JX2CC_PSREG_ALTLR, BGBCC_SH_REG_SP, 0);

				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_EXSR, BGBCC_SH_REG_R1);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
//					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 32);
					BGBCC_JX2CC_PSREG_ALTLR, BGBCC_SH_REG_SP, 32);

//				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_SPC, BGBCC_JX2CC_PSREG_ALTLR);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
//					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 24);
					BGBCC_JX2CC_PSREG_ALTLR, BGBCC_SH_REG_SP, 24);


				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//					BGBCC_SH_REG_PR, BGBCC_SH_REG_R1);
					BGBCC_SH_REG_PR, BGBCC_JX2CC_PSREG_ALTLR);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
//					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 16);
					BGBCC_JX2CC_PSREG_ALTLR, BGBCC_SH_REG_SP, 16);

				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//					BGBCC_SH_REG_GBR, BGBCC_SH_REG_R1);
					BGBCC_SH_REG_GBR, BGBCC_JX2CC_PSREG_ALTLR);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
//					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 8);
					BGBCC_JX2CC_PSREG_ALTLR, BGBCC_SH_REG_SP, 8);


				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_SP, 0, BGBCC_SH_REG_R1);
				BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
					8, BGBCC_SH_REG_SP);
			}
		}
		else
			if(sctx->is_pbo && !(sctx->is_pbo&2) &&
				(obj->regflags&BGBCC_REGFL_ALIASPTR))
		{
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_PR);
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_GBR);
			k+=4;

			if(sctx->has_fmovc)
			{
				BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
					-16, BGBCC_SH_REG_SP);

				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVC,
					BGBCC_SH_REG_PR, BGBCC_SH_REG_SP, 8);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVC,
					BGBCC_SH_REG_GBR, BGBCC_SH_REG_SP, 0);
			}

#if 1
			else
			{
				BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
					-24, BGBCC_SH_REG_SP);

				/* ALTLR Ref 1 */
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
//					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 0);
					BGBCC_JX2CC_PSREG_ALTLR, BGBCC_SH_REG_SP, 0);

				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//					BGBCC_SH_REG_PR, BGBCC_SH_REG_R1);
					BGBCC_SH_REG_PR, BGBCC_JX2CC_PSREG_ALTLR);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
//					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 16);
					BGBCC_JX2CC_PSREG_ALTLR, BGBCC_SH_REG_SP, 16);

				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//					BGBCC_SH_REG_GBR, BGBCC_SH_REG_R1);
					BGBCC_SH_REG_GBR, BGBCC_JX2CC_PSREG_ALTLR);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
//					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 8);
					BGBCC_JX2CC_PSREG_ALTLR, BGBCC_SH_REG_SP, 8);

				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
//					BGBCC_SH_REG_SP, 0, BGBCC_SH_REG_R1);
					BGBCC_SH_REG_SP, 0, BGBCC_JX2CC_PSREG_ALTLR);
				BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
					8, BGBCC_SH_REG_SP);

	//			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
	//				BGBCC_SH_REG_PR, BGBCC_SH_REG_R1);
	//			BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
	//				-8, BGBCC_SH_REG_SP);
	//			BGBCC_JX2_EmitOpRegStReg(sctx, BGBCC_SH_NMID_MOVQ,
	//				BGBCC_SH_REG_R1, BGBCC_SH_REG_SP);
			}
#endif

//			k+=sctx->is_addr64?2:1;
		}else
		{
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_PR);
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_GBR);
			k+=2;

			if(sctx->has_fmovc)
			{
				BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
					-8, BGBCC_SH_REG_SP);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVC,
					BGBCC_SH_REG_PR, BGBCC_SH_REG_SP, 0);
			}else
			{
				/* ALTLR Ref 2 */

				BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
					-16, BGBCC_SH_REG_SP);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
//					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 0);
					BGBCC_JX2CC_PSREG_ALTLR, BGBCC_SH_REG_SP, 0);

				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//					BGBCC_SH_REG_PR, BGBCC_SH_REG_R1);
					BGBCC_SH_REG_PR, BGBCC_JX2CC_PSREG_ALTLR);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
//					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 8);
					BGBCC_JX2CC_PSREG_ALTLR, BGBCC_SH_REG_SP, 8);

				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
//					BGBCC_SH_REG_SP, 0, BGBCC_SH_REG_R1);
					BGBCC_SH_REG_SP, 0, BGBCC_JX2CC_PSREG_ALTLR);

				BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
					8, BGBCC_SH_REG_SP);
			}
		}
#endif


#if 1
		if(	(obj->flagsint&BGBCC_TYFL_INTERRUPT)	&&
			(obj->flagsint&BGBCC_TYFL_TBRSAVE)		)
		{
			j=k;

//			if(sctx->has_xgpr&1)
//				k+=64;
//			if(sctx->has_bjx1egpr)
//				k+=32;
//			k+=32;

			k+=8;

			BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
				-(k-j)*4, BGBCC_SH_REG_SP);

			if(sctx->has_pushx2)
			{
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVX2,
					BGBCC_SH_REG_LR2, BGBCC_SH_REG_SP, 2*8);
			}else
			{
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R0+3, BGBCC_SH_REG_SP, 3*8);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R0+2, BGBCC_SH_REG_SP, 2*8);
			}

			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R0+1, BGBCC_SH_REG_SP, 1*8);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R0+0, BGBCC_SH_REG_SP, 0*8);

			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_TBR, 0x20, BGBCC_SH_REG_R2);

			if(sctx->has_pushx2)
			{
				if(sctx->has_xgpr&1)
				{
					for(i=62; i>=32; i-=2)
					{
						BGBCC_JX2_EmitOpRegStRegDisp(sctx, 
							BGBCC_SH_NMID_MOVX2,
							BGBCC_SH_REG_LR32+(i-32), BGBCC_SH_REG_R2, i*8);
					}
				}

				for(i=30; i>=4; i-=2)
				{
					if(i==14)
						continue;
					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVX2,
						BGBCC_SH_REG_LR0+i, BGBCC_SH_REG_R2, i*8);
				}

				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVX2,
					BGBCC_SH_REG_SP, 0*8, BGBCC_SH_REG_R4);
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVX2,
					BGBCC_SH_REG_SP, 2*8, BGBCC_SH_REG_R6);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVX2,
					BGBCC_SH_REG_R4, BGBCC_SH_REG_R2, 0*8);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVX2,
					BGBCC_SH_REG_R6, BGBCC_SH_REG_R2, 2*8);

				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVX2,
					BGBCC_SH_REG_SP, 4*8, BGBCC_SH_REG_R4);
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVX2,
					BGBCC_SH_REG_SP, 6*8, BGBCC_SH_REG_R6);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVX2,
					BGBCC_SH_REG_R4, BGBCC_SH_REG_R2, 0x40*8);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVX2,
					BGBCC_SH_REG_R6, BGBCC_SH_REG_R2, 0x42*8);

				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R14, BGBCC_SH_REG_R2, 14*8);

				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//					BGBCC_SH_REG_SGR, BGBCC_SH_REG_R1);
					BGBCC_SH_REG_SGR, BGBCC_JX2CC_PSREG_ALTLR);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
//					BGBCC_SH_REG_R1, BGBCC_SH_REG_R2, 15*8);
					BGBCC_JX2CC_PSREG_ALTLR, BGBCC_SH_REG_R2, 15*8);
			}else
			{
				if(sctx->has_xgpr&1)
				{
					for(i=63; i>=32; i--)
					{
						BGBCC_JX2_EmitOpRegStRegDisp(sctx, 
							BGBCC_SH_NMID_MOVQ,
							BGBCC_SH_REG_RQ0+i, BGBCC_SH_REG_R2, i*8);
					}
				}

				for(i=31; i>=4; i--)
				{
					if(i==15)
						continue;
					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
						BGBCC_SH_REG_RQ0+i, BGBCC_SH_REG_R2, i*8);
				}

				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_SP, 0*8, BGBCC_SH_REG_R4);
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_SP, 1*8, BGBCC_SH_REG_R5);
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_SP, 2*8, BGBCC_SH_REG_R6);
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_SP, 3*8, BGBCC_SH_REG_R7);

				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R4, BGBCC_SH_REG_R2, 0*8);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R5, BGBCC_SH_REG_R2, 1*8);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R6, BGBCC_SH_REG_R2, 2*8);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R7, BGBCC_SH_REG_R2, 3*8);

				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_SP, 4*8, BGBCC_SH_REG_R4);
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_SP, 5*8, BGBCC_SH_REG_R5);
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_SP, 6*8, BGBCC_SH_REG_R6);
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_SP, 7*8, BGBCC_SH_REG_R7);

				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R4, BGBCC_SH_REG_R2, 0x40*8);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R5, BGBCC_SH_REG_R2, 0x41*8);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R6, BGBCC_SH_REG_R2, 0x42*8);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R7, BGBCC_SH_REG_R2, 0x43*8);

				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_SGR, BGBCC_SH_REG_RQ1);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_RQ1, BGBCC_SH_REG_R2, 15*8);
			}
		}
#endif

//		if(obj->flagsint&BGBCC_TYFL_INTERRUPT)
		if(	 (obj->flagsint&BGBCC_TYFL_INTERRUPT)	&&
			!(obj->flagsint&BGBCC_TYFL_TBRSAVE)		)
		{
#if 1
			j=k;
//			if(sctx->has_bjx1egpr)
//				k+=16;
//			k+=16;

			if(sctx->has_xgpr&1)
				k+=64;
			if(sctx->has_bjx1egpr)
				k+=32;
			k+=32;

			BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
				-(k-j)*4, BGBCC_SH_REG_SP);

#if 1
			if(sctx->has_pushx2)
			{
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R0+0, BGBCC_SH_REG_SP, 0*8);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R0+1, BGBCC_SH_REG_SP, 1*8);

				if(sctx->has_xgpr&1)
				{
					for(i=62; i>=32; i-=2)
					{
						BGBCC_JX2_EmitOpRegStRegDisp(sctx, 
							BGBCC_SH_NMID_MOVX2,
//							BGBCC_SH_REG_RQ0+i, BGBCC_SH_REG_SP, i*8);
							BGBCC_SH_REG_LR32+(i-32), BGBCC_SH_REG_SP, i*8);
					}
				}

				for(i=30; i>=2; i-=2)
				{
					if(i==14)
						continue;
					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVX2,
//						BGBCC_SH_REG_RQ0+i, BGBCC_SH_REG_SP, i*8);
						BGBCC_SH_REG_LR0+i, BGBCC_SH_REG_SP, i*8);
				}

#if 1
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R14, BGBCC_SH_REG_SP, 14*8);

				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//					BGBCC_SH_REG_SGR, BGBCC_SH_REG_R1);
					BGBCC_SH_REG_SGR, BGBCC_JX2CC_PSREG_ALTLR);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
//					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 15*8);
					BGBCC_JX2CC_PSREG_ALTLR, BGBCC_SH_REG_SP, 15*8);
#endif
			}else
			{
				if(sctx->has_xgpr&1)
				{
					for(i=63; i>=32; i--)
					{
						BGBCC_JX2_EmitOpRegStRegDisp(sctx, 
							BGBCC_SH_NMID_MOVQ,
							BGBCC_SH_REG_RQ0+i, BGBCC_SH_REG_SP, i*8);
					}
				}

				for(i=31; i>=0; i--)
				{
					if(i==15)
						continue;
					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
						BGBCC_SH_REG_RQ0+i, BGBCC_SH_REG_SP, i*8);
				}

				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_SGR, BGBCC_SH_REG_RQ1);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_RQ1, BGBCC_SH_REG_SP, 15*8);
			}
#endif

#endif
		}

		sctx->frm_offs_isrsaves=sctx->frm_size-(k*4);

		fl2=0;
		k+=BGBCC_JX2C_EmitFrameProlog_PushRegs(ctx, sctx, 0, &fl2);

		if((obj->regflags&BGBCC_REGFL_ALIASPTR) && !(sctx->is_pbo&2))
		{
			if(sctx->use_wexmd)
			{
				BGBCC_JX2_EmitOpImm(sctx,
					BGBCC_SH_NMID_WEXMD, sctx->use_wexmd);
			}
		}

		if(sctx->is_pbo && !(sctx->is_pbo&2) &&
			(obj->regflags&BGBCC_REGFL_ALIASPTR))
		{
			j=BGBCC_JX2_GetNamedLabel(sctx, "__global_ptr");
			if(sctx->has_fmovc&1)
			{
				BGBCC_JX2_EmitOpLdRegDispReg(sctx,
					BGBCC_SH_NMID_MOVQ,
//					BGBCC_SH_REG_GBR, 0, BGBCC_SH_REG_R18);
					BGBCC_SH_REG_GBR, 0, BGBCC_JX2CC_PSREG_TS0);
				if(sctx->is_exe&1)
//				if(0)
				{
					BGBCC_JX2_EmitOpLdRegDispReg(sctx,
						BGBCC_SH_NMID_MOVC,
//						BGBCC_SH_REG_R18, 0, BGBCC_SH_REG_GBR);
						BGBCC_JX2CC_PSREG_TS0, 0, BGBCC_SH_REG_GBR);
				}else
				{
					BGBCC_JX2_EmitRelocTy(sctx, j, BGBCC_SH_RLC_TBR12_BJX);
					BGBCC_JX2_EmitOpLdRegDispReg(sctx,
						BGBCC_SH_NMID_MOVC_DISP24,
//						BGBCC_SH_REG_R18, 0, BGBCC_SH_REG_GBR);
						BGBCC_JX2CC_PSREG_TS0, 0, BGBCC_SH_REG_GBR);
				}

			}else
			{
				if(sctx->is_exe&1)
//				if(0)
				{
					BGBCC_JX2_EmitOpLdRegDispReg(sctx,
						BGBCC_SH_NMID_MOVQ,
//						BGBCC_SH_REG_GBR, 0, BGBCC_SH_REG_R18);
						BGBCC_SH_REG_GBR, 0, BGBCC_JX2CC_PSREG_TS0);
					BGBCC_JX2_EmitOpLdRegDispReg(sctx,
						BGBCC_SH_NMID_MOVQ,
//						BGBCC_SH_REG_R18, 0, BGBCC_SH_REG_R18);
						BGBCC_JX2CC_PSREG_TS0, 0, BGBCC_JX2CC_PSREG_TS0);
					BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//						BGBCC_SH_REG_R18, BGBCC_SH_REG_GBR);
						BGBCC_JX2CC_PSREG_TS0, BGBCC_SH_REG_GBR);
				}else
				{
					BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
						BGBCC_SH_NMID_MOVQ,
//						BGBCC_SH_REG_R1, j);
						BGBCC_JX2CC_PSREG_ALTLR, j);
					BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//						BGBCC_SH_REG_R1, BGBCC_SH_REG_GBR);
						BGBCC_JX2CC_PSREG_ALTLR, BGBCC_SH_REG_GBR);
				}
			}
		}
	}


	BGBCC_JX2C_ResetModeDqUnknown(ctx, sctx);
//	BGBCC_JX2C_SetModeDqSet(ctx, sctx);
//	BGBCC_JX2C_ResetModeDqLocal(ctx, sctx);

//	BGBCC_JX2_EmitWord(sctx, 0x0858);	//Debug, Set Token
	
	sctx->frm_offs_save=sctx->frm_size-(k*4);
	
	if(sctx->frm_offs_save<(sctx->frm_size+sctx->frm_offs_save_rsv))
	{
		printf("Sanity: offs_save is below offs_save_rsv\n");
		BGBCC_CCXL_StubError(ctx);
	}

//	BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);

	BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
		-sctx->frm_offs_save, BGBCC_SH_REG_SP);

	k=sctx->frm_offs_retstr;
	if(k)
	{
		obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;

		if(ctx->arch_sizeof_ptr==16)
		{
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
				k, BGBCC_JX2CC_PSREG_LRRET);
		}else
			if(sctx->is_addr64)
		{
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, 
				BGBCC_JX2CC_PSREG_STRUCTRET);
		}
		else
		{
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k,
				BGBCC_JX2CC_PSREG_STRUCTRET);
		}

//		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_BRK);
	}
	
	k=sctx->frm_offs_thisptr;
	if(k)
	{
		obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;

//		if(sctx->has_xgpr&2)
		if(ctx->arch_sizeof_ptr==16)
		{
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//				k, BGBCC_SH_REG_LR18);
				k, BGBCC_JX2CC_PSREG_LRTHIS);
		}else
			if(sctx->is_addr64)
		{
//			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_RQ3);
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k,
				BGBCC_JX2CC_PSREG_RQTHIS);
		}
		else
		{
//			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_R3);
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, 
				BGBCC_JX2CC_PSREG_RQTHIS);
		}
	}

	
	k=sctx->frm_offs_sectoken;
	if(k)
	{
//		BGBCC_JX2_EmitLoadRegImm64P(sctx, BGBCC_SH_REG_RQ16,
		BGBCC_JX2_EmitLoadRegImm64P(sctx,
			BGBCC_JX2CC_PSREG_ARGTMP(0),
//			0x1234567890ABCDEFLL);
			sctx->frm_val_sectoken);
			
		if(sctx->abi_spillpad&2)
		{
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_VSKG,
//				BGBCC_SH_REG_R16, BGBCC_SH_REG_R16);
				BGBCC_JX2CC_PSREG_ARGTMP(0),
				BGBCC_JX2CC_PSREG_ARGTMP(0));
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_REGCHKG,
//				BGBCC_SH_REG_R16);
		}
			
//		BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_RQ16);
		BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k,
			BGBCC_JX2CC_PSREG_ARGTMP(0));
	}

	if(	sctx->regalc_live || sctx->fregalc_live ||
		sctx->sreg_live || sctx->sfreg_live)
			{ BGBCC_DBGBREAK }

//	sctx->sreg_live|=0x00F000F0ULL;
	sctx->sreg_live|=BGBCC_JX2C_GetFrameArgRegsMask(ctx, sctx);

	ni=0; nf=0; vaix=-1;
	for(i=0; i<obj->n_args; i++)
	{
		if(BGBCC_CCXL_TypeVarArgsP(ctx, obj->args[i]->type))
			{ vaix=i; continue; }

		tty=obj->args[i]->type;
		treg.val=CCXL_REGTY_ARG|
			(((u64)tty.val)<<CCXL_REGID_TYPESHIFT)|i;
		pr0=BGBCC_JX2C_GetVRegPriority(ctx, sctx, treg);
	
		rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->args[i]->type);
		switch(rcls)
		{
		case BGBCC_SH_REGCLS_GR:
//		case BGBCC_SH_REGCLS_VO_GR:
//			if(sctx->is_addr64)
			if(1)
			{
				if((ni>=4) && !sctx->has_bjx1egpr)
					break;
//				if(ni>=8)
//					break;
				if((ni>=8) && !(sctx->has_xgpr&2))
					break;
				if(ni>=16)
					break;

				k=obj->args[i]->fxoffs;

#if 1
				tr0=BGBCC_JX2C_GetRdRegForArgumentIndex(ctx, sctx, ni);
				if(tr0>0)
				{
					BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
						treg, tr0);
					ni++;
					break;
				}
#endif

				break;
			}
			break;

		case BGBCC_SH_REGCLS_VO_GR:
//			if(sctx->is_addr64)
			if(1)
			{
				if((ni>=4) && !sctx->has_bjx1egpr)
					break;
//				if(ni>=8)
//					break;
				if((ni>=8) && !(sctx->has_xgpr&2))
					break;
				if(ni>=16)
					break;

				k=obj->args[i]->fxoffs;

#if 1
				tr0=BGBCC_JX2C_GetRdRegForArgumentIndex(ctx, sctx, ni);
				if(tr0>0)
				{
					BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
						treg, tr0);
					ni++;
					break;
				}
#endif
				break;
			}
			break;

		case BGBCC_SH_REGCLS_VO_REF:
		case BGBCC_SH_REGCLS_AR_REF:
//		case BGBCC_SH_REGCLS_VO_QGR:
		case BGBCC_SH_REGCLS_QGR:
//			if(sctx->is_addr64)
			if(1)
			{
				if((ni>=4) && !sctx->has_bjx1egpr)
					break;
//				if(ni>=8)
//					break;
				if((ni>=8) && !(sctx->has_xgpr&2))
					break;
				if(ni>=16)
					break;

				k=obj->args[i]->fxoffs;

#if 1
				tr0=BGBCC_JX2C_GetRqRegForArgumentIndex(ctx, sctx, ni);
				if(tr0>0)
				{
					BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
						treg, tr0);
					ni++;
					break;
				}
#endif
				break;
			}
			break;

		case BGBCC_SH_REGCLS_VO_QGR:
//		case BGBCC_SH_REGCLS_QGR:
//			if(sctx->is_addr64)
			if(1)
			{
				if((ni>=4) && !sctx->has_bjx1egpr)
					break;
//				if(ni>=8)
//					break;
				if((ni>=8) && !(sctx->has_xgpr&2))
					break;
				if(ni>=16)
					break;

				k=obj->args[i]->fxoffs;

#if 1
				tr0=BGBCC_JX2C_GetRqRegForArgumentIndex(ctx, sctx, ni);
				if(tr0>0)
				{
					BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
						treg, tr0);
					ni++;
					break;
				}
#endif
				break;
			}
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
				if((ni>=3) && !sctx->has_bjx1egpr)
					break;
//				if(ni>=7)
//					break;
				if((ni>=7) && !(sctx->has_xgpr&2))
					break;
				if(ni>=15)
					break;

				k=obj->args[i]->fxoffs;

#if 1
//				if(sctx->has_xgpr&2)
				if(!(ni&1))
				{
					tr0=BGBCC_JX2C_GetLrRegForArgumentIndex(ctx, sctx, ni);
					if(tr0>0)
					{
						BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
							treg, tr0);
						ni+=2;
						break;
					}
				}else
				{
					tr0=BGBCC_JX2C_GetRqRegForArgumentIndex(ctx, sctx, ni+0);
					tr1=BGBCC_JX2C_GetRqRegForArgumentIndex(ctx, sctx, ni+1);
					if((tr0>0) && (tr1>0))
					{
						BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
							k+0, tr0);
						BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
							k+8, tr1);
						ni+=2;
						break;
					}
				}
#endif
				break;
			}
			break;

		default:
			BGBCC_DBGBREAK
			break;
		}
	}
	if(vaix<0)
		vaix=obj->n_args;

	if(!strcmp(obj->name, "TKRA_MatrixIdentify"))
	{
		i=-1;
//		sctx->frm_offs_retstr=k-8;
//		BGBCC_DBGBREAK
	}

	if(	sctx->regalc_live || sctx->fregalc_live)
			{ BGBCC_DBGBREAK }

	for(i=0; i<obj->n_locals; i++)
	{
		if(obj->locals[i]->regflags&BGBCC_REGFL_CULL)
			continue;

		rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->locals[i]->type);

		if(obj->locals[i]->regflags&BGBCC_REGFL_ALIASPTR)
		{
			obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;
		}
		
		if(BGBCC_CCXL_TypeArrayP(ctx, obj->locals[i]->type))
		{
			obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;
		}

		if((rcls==BGBCC_SH_REGCLS_VO_REF) ||
			(rcls==BGBCC_SH_REGCLS_AR_REF) ||
			(rcls==BGBCC_SH_REGCLS_VO_REF2) ||
			(rcls==BGBCC_SH_REGCLS_AR_REF2))
		{
			j=obj->locals[i]->fxmoffs+(sctx->frm_offs_fix);
			k=obj->locals[i]->fxoffs;

			obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;

//			if(sctx->has_xgpr&2)
			if(ctx->arch_sizeof_ptr==16)
			{
				BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR16);
				if(obj->locals[i]->fxmoffs<0)
				{
					/* Dynamically allocate with alloca... */
					sz=BGBCC_CCXL_TypeGetLogicalSize(ctx,
						obj->locals[i]->type);
					BGBCC_JX2C_EmitOpImmReg(ctx, sctx,
						BGBCC_SH_NMID_MOV, sz, BGBCC_SH_REG_RQ4);
					BGBCC_JX2C_EmitCallName(ctx, sctx, "__alloca");
					BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
						BGBCC_SH_NMID_XMOV,
						BGBCC_SH_REG_LR2, BGBCC_SH_REG_LR16);
				}else
				{
					BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx,
						j, BGBCC_SH_REG_LR16);
				}
				BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
					k, BGBCC_SH_REG_LR16);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR16);

				if(BGBCC_CCXL_TypeVaListP(ctx, obj->locals[i]->type) &&
					(vaix>0))
				{
					BGBCC_JX2_EmitOpRegReg(sctx,
						BGBCC_SH_NMID_MOV,
						BGBCC_SH_REG_RQ16, BGBCC_SH_REG_RQ3);
				}

			}else
			{
//				BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R3);
				BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx,
					BGBCC_JX2CC_PSREG_TS0);
//				if(sctx->is_addr64)
				if(1)
				{
					if(obj->locals[i]->fxmoffs<0)
					{
						/* Dynamically allocate with alloca... */
						sz=BGBCC_CCXL_TypeGetLogicalSize(ctx,
							obj->locals[i]->type);
						BGBCC_JX2C_EmitOpImmReg(ctx, sctx,
//							BGBCC_SH_NMID_MOV, sz, BGBCC_SH_REG_RQ4);
							BGBCC_SH_NMID_MOV, sz, BGBCC_JX2CC_PSREG_ARG(0));
						BGBCC_JX2C_EmitCallName(ctx, sctx, "__alloca");
						BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
							BGBCC_SH_NMID_MOV,
//							BGBCC_SH_REG_RQ2, BGBCC_SH_REG_RQ3);
							BGBCC_JX2CC_PSREG_RQRET, BGBCC_JX2CC_PSREG_TS0);
					}else
					{
						BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx,
//							j, BGBCC_SH_REG_RQ3);
							j, BGBCC_JX2CC_PSREG_TS0);
					}
					BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//						k, BGBCC_SH_REG_RQ3);
						k, BGBCC_JX2CC_PSREG_TS0);
				}else
				{
					BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx,
//						j, BGBCC_SH_REG_R3);
						j, BGBCC_JX2CC_PSREG_TS0);
					BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//						k, BGBCC_SH_REG_R3);
						k, BGBCC_JX2CC_PSREG_TS0);
				}
//				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R3);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_TS0);
			}
		}
		
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
			BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, tty, treg, reg);
		}

		if(BGBCC_CCXL_TypeVaListP(ctx, obj->locals[i]->type) && (vaix>0))
		{

#if 1
			if(sctx->emit_riscv&0x33)
			{
				if(ni<=8)
				{
					BGBCC_JX2C_EmitOpImmReg(ctx, sctx,
						BGBCC_SH_NMID_MOV, ni*8, BGBCC_JX2CC_PSREG_TS1);
				}else
				{
					BGBCC_JX2C_EmitOpImmReg(ctx, sctx,
						BGBCC_SH_NMID_MOV, 8*8, BGBCC_JX2CC_PSREG_TS1);
				}
				BGBCC_JX2C_EmitCallName(ctx, sctx, "__va64_saveargs");
			}else
#endif

#if 1
			if(sctx->is_addr64)
			{
//				BGBCC_JX2C_EmitCallName(ctx, sctx, "__va64_saveargs");

#if 1
				if(sctx->has_pushx2)
				{
					for(j=0; j<4; j+=2)
					{
						BGBCC_JX2_EmitOpRegStRegDisp(sctx,
							BGBCC_SH_NMID_MOVX2,
							BGBCC_SH_REG_RQ4+j,
//							BGBCC_SH_REG_RQ3, (j+0)*8);
							BGBCC_JX2CC_PSREG_TS0, (j+0)*8);
						BGBCC_JX2_EmitOpRegStRegDisp(sctx,
							BGBCC_SH_NMID_MOVX2,
							BGBCC_SH_REG_RQ20+j,
//							BGBCC_SH_REG_RQ3, (j+4)*8);
							BGBCC_JX2CC_PSREG_TS0, (j+4)*8);
					}
				}else
				{
					for(j=0; j<4; j++)
					{
						BGBCC_JX2_EmitOpRegStRegDisp(sctx,
							BGBCC_SH_NMID_MOVQ,
							BGBCC_SH_REG_RQ4+j,
//							BGBCC_SH_REG_RQ3, (j+0)*8);
							BGBCC_JX2CC_PSREG_TS0, (j+0)*8);
						BGBCC_JX2_EmitOpRegStRegDisp(sctx,
							BGBCC_SH_NMID_MOVQ,
							BGBCC_SH_REG_RQ20+j,
//							BGBCC_SH_REG_RQ3, (j+4)*8);
							BGBCC_JX2CC_PSREG_TS0, (j+4)*8);
					}
				}

				if(sctx->has_xgpr&2)
				{
					if(sctx->has_pushx2)
					{
						for(j=0; j<4; j+=2)
						{
							BGBCC_JX2_EmitOpRegStRegDisp(sctx,
								BGBCC_SH_NMID_MOVX2,
								BGBCC_SH_REG_RQ36+j,
//								BGBCC_SH_REG_RQ3, (j+8)*8);
								BGBCC_JX2CC_PSREG_TS0, (j+8)*8);
							BGBCC_JX2_EmitOpRegStRegDisp(sctx,
								BGBCC_SH_NMID_MOVX2,
								BGBCC_SH_REG_RQ52+j,
//								BGBCC_SH_REG_RQ3, (j+12)*8);
								BGBCC_JX2CC_PSREG_TS0, (j+12)*8);
						}
					}else
					{
						for(j=0; j<4; j++)
						{
							BGBCC_JX2_EmitOpRegStRegDisp(sctx,
								BGBCC_SH_NMID_MOVQ,
								BGBCC_SH_REG_RQ36+j,
//								BGBCC_SH_REG_RQ3, (j+8)*8);
								BGBCC_JX2CC_PSREG_TS0, (j+8)*8);
							BGBCC_JX2_EmitOpRegStRegDisp(sctx,
								BGBCC_SH_NMID_MOVQ,
								BGBCC_SH_REG_RQ52+j,
//								BGBCC_SH_REG_RQ3, (j+12)*8);
								BGBCC_JX2CC_PSREG_TS0, (j+12)*8);
						}
					}
				}
#endif

//				if(sctx->has_xgpr&2)
				if(1)
				{
					BGBCC_JX2_EmitLoadRegImm(sctx,
//						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1, ni*8);
						BGBCC_SH_NMID_MOVL, BGBCC_JX2CC_PSREG_ALTLR, ni*8);
					BGBCC_JX2_EmitOpRegStRegDisp(sctx,
//						BGBCC_SH_NMID_MOVQ, BGBCC_SH_REG_R1,
						BGBCC_SH_NMID_MOVQ, BGBCC_JX2CC_PSREG_ALTLR,
//						BGBCC_SH_REG_R3, 128);
						BGBCC_JX2CC_PSREG_TS0, 128);

					j=0;
					if(sctx->abi_spillpad&1)
					{
						j=8*8;
						if(sctx->has_xgpr&2)
							j=16*8;
					}

//					BGBCC_JX2_EmitLoadRegImm(sctx,
//						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1,
//						sctx->frm_size+j);

//					BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
//						BGBCC_SH_REG_SP, BGBCC_SH_REG_R1);

//					BGBCC_JX2_EmitOpRegImmReg(sctx, BGBCC_SH_NMID_ADD,
//						BGBCC_SH_REG_SP, sctx->frm_size+j, BGBCC_SH_REG_R1);

					BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_LEAB,
//						BGBCC_SH_REG_SP, sctx->frm_size+j, BGBCC_SH_REG_R1);
						BGBCC_SH_REG_SP, sctx->frm_size+j, 
							BGBCC_JX2CC_PSREG_TS1);

					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
//						BGBCC_SH_REG_R1, BGBCC_SH_REG_R3, 144);
						BGBCC_JX2CC_PSREG_TS1, BGBCC_JX2CC_PSREG_TS0, 144);
				}else
				{
					BGBCC_DBGBREAK
				}

//				BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_BRK);
			}else
			{
				BGBCC_DBGBREAK
			}
#endif
		}
	}

	k=sctx->frm_offs_allocamrk;
	if(k)
	{
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__alloca_begin");
		if(sctx->has_xgpr&2)
		{
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k,
//				BGBCC_SH_REG_LR2);
				BGBCC_JX2CC_PSREG_LRRET);
		}
		else
		{
//			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_RQ2);
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, 
				BGBCC_JX2CC_PSREG_RQRET);
		}
	}

//	sctx->sreg_live&=~0x00FF00FF00F000F0ULL;
	sctx->sreg_live&=~sctx->regs_args;

	if(	sctx->regalc_live || sctx->fregalc_live ||
		sctx->sreg_live || sctx->sfreg_live)
			{ BGBCC_DBGBREAK }

	BGBCC_JX2C_EmitSyncProlog(ctx, sctx);

	if(	sctx->regalc_live || sctx->fregalc_live ||
		sctx->sreg_live || sctx->sfreg_live)
			{ BGBCC_DBGBREAK }

#if 1
	for(i=0; i<sctx->vsp_rsv; i++)
	{
		reg=sctx->vspan[i]->reg;

		if(	BGBCC_CCXL_IsRegImmP(ctx, reg) ||
			BGBCC_CCXL_IsRegGlobalP(ctx, reg))
		{
			BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, reg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, reg);
		}
	}
#endif

	co=BGBCC_JX2_EmitGetOffs(sctx);
	sctx->fnsz_pro=co-bo;

	return(0);
}
