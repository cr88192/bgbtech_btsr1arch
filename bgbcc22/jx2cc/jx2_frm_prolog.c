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

int BGBCC_JX2C_CalcFrameEpiKey(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj,
	int rqt, u64 *repik, int *repix)
{
	u64 uli;
	u64 epik, epix, epilbl;

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
			((sctx->reg_save >>16)&0x000000FF00000000ULL)|
			((sctx->reg_save >>24)&0x0000FF0000000000ULL);

	if(sctx->use_fpr)
		epik|=0x01000000;
	if(!(sctx->is_leaf&1))
		epik|=0x02000000;

	if((obj->regflags&BGBCC_REGFL_ALIASPTR) && sctx->is_pbo)
		epik|=0x04000000;
		
	uli=epik;
	uli=(uli+1)*65521;
	uli=(uli+1)*65521;
	uli=(uli+1)*251;
	epix=(uli>>32)&1023;

	/* Unusual Registers Saved */
	if(sctx->reg_save&0x00FF00FF00FF00FFULL)
		{ epilbl=0; epik=0; epix=0; }
	if(sctx->freg_save&0xFFFFFFFFFFFF00FFULL)
		{ epilbl=0; epik=0; epix=0; }

	/* Not enough registers saved */
	if((epik&0x60)!=0x60)
//	if((epik&0x70)!=0x70)
//	if((epik&0x7C)!=0x7C)
		{ epilbl=0; epik=0; epix=0; }
//	if((epik&0x60)!=0x60)
//		{ epilbl=0; epik=0; epix=0; }

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

int BGBCC_JX2C_EmitFrameProlog_PushRegs(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx, int fl, int *rfl2)
{
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
		if(fl&2)
			k+=4;

//		for(i=31; i>0; i--)
		for(i=63; i>0; i--)
		{
			if(!(sctx->has_xgpr&1) && (i>=32))
				continue;
			if(!(sctx->has_bjx1egpr) && (i>=16))
				continue;
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
					i--;
					continue;
				}
			}

			if(sctx->reg_vsave&(1ULL<<i))
			{
				sctx->reg_save|=(1ULL<<i);
				k+=2;
			}
		}

		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
			-k*4, BGBCC_SH_REG_SP);
		n++;
			
		j=0;

		if(fl&1)
		{
			j+=2;
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
//				BGBCC_SH_REG_R16, BGBCC_SH_REG_SP, (k-j)*4);
				BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, (k-j)*4);
			n++;
		}

		if(fl&2)
		{
			j+=4;
			if(sctx->has_pushx2)
//			if(0)
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

//		for(i=31; i>0; i--)
		for(i=63; i>0; i--)
		{
			if(!(sctx->has_xgpr&1) && (i>=32))
				continue;
			if(!(sctx->has_bjx1egpr) && (i>=16))
				continue;
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
					j+=4;

					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVX2,
						BGBCC_SH_REG_R0+(i-1), BGBCC_SH_REG_SP, (k-j)*4);
					n++;
					i--;
					continue;
				}
			}

			if(sctx->reg_vsave&(1ULL<<i))
			{
				j+=2;

				sctx->reg_save|=(1ULL<<i);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R0+i, BGBCC_SH_REG_SP, (k-j)*4);
				n++;
			}
		}
		
		if(n>=6)
			fl2|=1;

		if(rfl2)
			*rfl2=fl2;

		return(k);
	}

	BGBCC_DBGBREAK

#if 0
	k=0;

	if(fl&1)
	{
		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH,
//			BGBCC_SH_REG_R16);
			BGBCC_SH_REG_R1);
		k+=2;
	}

//	for(i=31; i>0; i--)
	for(i=63; i>0; i--)
	{
		if(!(sctx->has_xgpr&1) && (i>=32))
			continue;
		if(!(sctx->has_bjx1egpr) && (i>=16))
			continue;
		if(i==15)
			continue;

		if(!(sctx->reg_vsave&((2LL<<i)-1)))
			break;

#if 1
		if(sctx->has_pushx2 && (i&1))
		{
			if((sctx->reg_vsave&(3ULL<<(i-1)))==(3ULL<<(i-1)))
			{
				sctx->reg_save|=(3ULL<<(i-1));
				BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSHX2,
					BGBCC_SH_REG_R0+(i-1));
				k+=sctx->is_addr64?4:2;
				i--;
				continue;
			}
		}
#endif

		if(sctx->reg_vsave&(1ULL<<i))
		{
			sctx->reg_save|=(1ULL<<i);
			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH,
				BGBCC_SH_REG_R0+i);
			k+=2;
		}
	}

#if 0
//	for(i=31; i>0; i--)
	for(i=63; i>0; i--)
	{
		if(sctx->freg_vsave&(1ULL<<i))
		{
			sctx->freg_save|=(1ULL<<i);
			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH,
				BGBCC_SH_REG_FR0+i);
			k+=2;
		}
	}
#endif

#endif

	return(k);
}

ccxl_status BGBCC_JX2C_TinyLeafProlog_ReserveReg(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, ccxl_register reg)
{
	int csreg;

	if(!reg.val)
		return(0);

	csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, reg);
	
	if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
	{
#if 0
		if(!(obj->regflags&BGBCC_REGFL_HYBLEAFTINY))
		{
			printf("BGBCC_JX2C_TinyLeafProlog_ReserveReg: "
				"Reject Alloc Fail %s %016llX\n",
				obj->qname, reg.val);
		}
#endif
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

int BGBCC_JX2C_EmitFrameProlog_TinyLeaf(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, int fcnlbl)
{
	BGBCC_CCXL_VirtOp	*vop;
	ccxl_register reg, treg;
	ccxl_type tty;
	int i, j, k, bo, ni, nf, vaix, rcls;

	sctx->is_leaftiny=3;

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

	sctx->lbl_ret=BGBCC_JX2_GenLabelTemp(sctx);
	

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
			if(ni>=12)
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_RD52+(ni-12));
			}else
				if(ni>=8)
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_RD36+(ni-8));
			}else
				if(ni>=4)
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_RD20+(ni-4));
			}else
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_RD4+ni);
			}

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
			if(ni>=12)
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_RD52+(ni-12));
			}
			if(ni>=8)
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_RD36+(ni-8));
			}
			if(ni>=4)
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_RD20+(ni-4));
			}else
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_RD4+ni);
			}

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);

//			if(	sctx->regalc_live || sctx->fregalc_live ||
//				sctx->sreg_live || sctx->sfreg_live)
//					{ BGBCC_DBGBREAK }

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
			if(ni>=12)
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_RQ52+(ni-12));
			}else
				if(ni>=8)
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_RQ36+(ni-8));
			}else
				if(ni>=4)
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_RQ20+(ni-4));
			}else
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_RQ4+ni);
			}

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);

//			if(	sctx->regalc_live || sctx->fregalc_live ||
//				sctx->sreg_live || sctx->sfreg_live)
//					{ BGBCC_DBGBREAK }

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
			if(ni>=12)
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_RQ52+(ni-12));
			}else
				if(ni>=8)
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_RQ36+(ni-8));
			}else
				if(ni>=4)
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_RQ20+(ni-4));
			}else
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_RQ4+ni);
			}

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);

//			if(	sctx->regalc_live || sctx->fregalc_live ||
//				sctx->sreg_live || sctx->sfreg_live)
//					{ BGBCC_DBGBREAK }

			ni++;
			break;

		case BGBCC_SH_REGCLS_GR2:
		case BGBCC_SH_REGCLS_QGR2:
		case BGBCC_SH_REGCLS_VO_GR2:
		case BGBCC_SH_REGCLS_VO_QGR2:
		case BGBCC_SH_REGCLS_VO_REF2:
		case BGBCC_SH_REGCLS_AR_REF2:
			obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;

#if 0
			ni=(ni+1)&(~1);

			if((ni>=4) && !sctx->has_bjx1egpr)
				break;
			if((ni>=8) && !(sctx->has_xgpr&2))
				break;
			if(ni>=16)
				break;

			k=obj->args[i]->fxoffs;
			if(ni>=12)
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_LR52+(ni-12));
				ni+=2;
				break;
			}
			if(ni>=8)
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_LR36+(ni-8));
				ni+=2;
				break;
			}
			if(ni>=4)
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_LR20+(ni-4));
				ni+=2;
				break;
			}
			BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
				treg, BGBCC_SH_REG_LR4+ni);
			ni+=2;
#endif
			break;

		default:
			BGBCC_DBGBREAK
			break;
		}
	}

//	if(	sctx->regalc_live || sctx->fregalc_live ||
//		sctx->sreg_live || sctx->sfreg_live)
//			{ BGBCC_DBGBREAK }

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

	sctx->is_leaftiny=1;
	
	return(0);
}


int BGBCC_JX2C_EmitFrameProlog(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, int fcnlbl)
{
	ccxl_register reg, treg;
	ccxl_type tty;
	int bo, co, pr0;
	int p0, vaix, cnt, maxrsv, ismaxrsv, maxvalidrsv;
	int ni, nf, rcls, ob, ov;
	u64 epik;
	int epix, epilbl, epij;
	int i, j, k, fl, fl2;

	ctx->cur_func=obj;
	ctx->cur_vtr=NULL;

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

		epilbl=BGBCC_JX2_GenLabel(sctx);
		sctx->eprhash_key[epix]=epik;
		sctx->eprhash_lbl[epix]=epilbl;

		if(sctx->is_align_wexj&2)
			{ BGBCC_JX2_EmitPad32AlignLastOp(sctx); }

		BGBCC_JX2_EmitLabel(sctx, epilbl);

		fl=1;
		if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
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
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
		}else
		{
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
		}
#endif

#if 0
		k=0;

	//	for(i=14; i>0; i--)
//		for(i=31; i>0; i--)
		for(i=63; i>0; i--)
		{
			if(!(sctx->has_bjx1egpr) && (i>=16))
				continue;
			if(i==15)
				continue;

			if(!(sctx->reg_vsave&((2LL<<i)-1)))
				break;
	//		if(!(sctx->reg_vsave&((2<<i)-1)))
	//			break;

#if 1
			if(sctx->has_pushx2 && (i&1))
			{
				if((sctx->reg_vsave&(3ULL<<(i-1)))==(3ULL<<(i-1)))
				{
					sctx->reg_save|=(3ULL<<(i-1));
					BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSHX2,
						BGBCC_SH_REG_R0+(i-1));
					k+=sctx->is_addr64?4:2;
					i--;
					continue;
				}
			}
#endif

			if(sctx->reg_vsave&(1ULL<<i))
			{
				sctx->reg_save|=(1ULL<<i);
				if(sctx->is_addr64)
				{
					BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH,
						BGBCC_SH_REG_R0+i);
				}else
				{
					BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_MOVL,
						BGBCC_SH_REG_R0+i, BGBCC_SH_REG_SP);
				}
	//			k++;
				k+=sctx->is_addr64?2:1;
			}
		}

		if(sctx->use_fpr && !sctx->no_fpu && !sctx->fpu_gfp)
		{
//			BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_STSL,
//				BGBCC_SH_REG_FPSCR, BGBCC_SH_REG_SP);
//			k+=sctx->is_addr64?2:1;
		}

//		for(i=15; i>0; i--)
//		for(i=31; i>0; i--)
		for(i=63; i>0; i--)
		{
			if(sctx->freg_vsave&(1ULL<<i))
			{
				sctx->freg_save|=(1ULL<<i);
//				BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_FMOVS,
//					BGBCC_SH_REG_FR0+i, BGBCC_SH_REG_SP);
//				k++;
				BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH,
					BGBCC_SH_REG_FR0+i);
				k+=2;
			}
		}

//		if(sctx->reg_save || sctx->freg_save)
//		{
//			/* Hack prior op into RTS delay slot */
//			ob=BGBCC_JX2_EmitGetOffs(sctx);
//			ov=BGBCC_JX2_EmitGetOffsWord(sctx, ob-2);
//			BGBCC_JX2_EmitSetOffsWord(sctx, ob-2, 0x000B);
//			BGBCC_JX2_EmitWord(sctx, ov);
//		}else
//		if(sctx->has_bjx1jmp)
		if(1)
		{
//			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTSN);
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTSU);
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

	sctx->lbl_ret=BGBCC_JX2_GenLabelTemp(sctx);
	
	if(sctx->cnt_set_fp64>sctx->cnt_set_fp32)
//	if(0)
	{
		sctx->dfl_fpscr=BGBCC_SH_FPSCR_PR;
	}else
	{
		sctx->dfl_fpscr=0;
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
	
	if(sctx->use_egpr)
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
			if(sctx->use_egpr)
				{ sctx->vsp_rsv+=8; }
			else
				{ sctx->vsp_rsv+=3; }
		}
	}


#if 1
	maxvalidrsv=sctx->vspan_num;
	for(i=0; i<sctx->vspan_num; i++)
	{
		reg=sctx->vspan[i]->reg;
		k=BGBCC_JX2C_GetFrameVRegFlags(ctx, sctx, reg);
		if(k&BGBCC_REGFL_ALIASPTR)
			{ maxvalidrsv=i; break; }
		if(k&BGBCC_REGFL_TEMPLOAD)
			{ maxvalidrsv=i; break; }

		if(
			!BGBCC_CCXL_IsRegTempP(ctx, reg) &&
			!BGBCC_CCXL_IsRegArgP(ctx, reg) &&
			!BGBCC_CCXL_IsRegLocalP(ctx, reg))
				{ maxvalidrsv=i; break; }

		if(BGBCC_CCXL_IsRegVolatileP(ctx, reg))
			{ maxvalidrsv=i; break; }

		tty=BGBCC_CCXL_GetRegType(ctx, reg);
		if(BGBCC_CCXL_TypeValueObjectP(ctx, tty) ||
			BGBCC_CCXL_TypeArrayP(ctx, tty) ||
//			BGBCC_CCXL_TypeSgLongP(ctx, tty) ||
			BGBCC_CCXL_TypeVoidP(ctx, tty) ||
//			BGBCC_CCXL_TypeRealP(ctx, tty) ||
			BGBCC_CCXL_TypeVec128P(ctx, tty) ||
			BGBCC_CCXL_TypeQuadPointerP(ctx, tty) ||
			BGBCC_CCXL_TypeQuadVariantP(ctx, tty) ||
			BGBCC_CCXL_TypeQuadVarObjP(ctx, tty) ||
			BGBCC_CCXL_TypeQuadVarRefP(ctx, tty))
				{ maxvalidrsv=i; break; }
	}
#endif

	/*
		If number of registers needed is less than what is available,
		Try to static assign everything.
	 */
	maxrsv=7;
	if(sctx->use_bp)maxrsv--;
	if(sctx->is_pic)maxrsv--;
	if(sctx->use_egpr)
		maxrsv+=8;

//	if(sctx->has_xgpr&2)
//		maxrsv+=12;

	ismaxrsv=0;

	if(	(sctx->vspan_num<=maxrsv) &&
		(sctx->vspan_num==maxvalidrsv))
	{
		sctx->vsp_rsv=sctx->vspan_num;
		ismaxrsv=1;
	}
	
	if(sctx->use_bp)sctx->vsp_rsv--;
	if(sctx->is_pic)sctx->vsp_rsv--;
	if(sctx->vsp_rsv<0)
		sctx->vsp_rsv=0;

	if(sctx->vspan_num<sctx->vsp_rsv)
		sctx->vsp_rsv=sctx->vspan_num;

	if(maxvalidrsv<sctx->vsp_rsv)
		sctx->vsp_rsv=maxvalidrsv;

	cnt=0;
	for(i=0; i<sctx->vsp_rsv; i++)
	{
		cnt+=sctx->vspan[i]->cnt;
//		if((cnt<(sctx->vsp_tcnt/8)) && !(ismaxrsv>0))
		if((cnt<(sctx->vsp_tcnt/16)) && !(ismaxrsv>0))
//		if((cnt<(sctx->vsp_tcnt/32)) && !(ismaxrsv>0))
//		if((cnt<((sctx->vsp_tcnt*8)>>8)) && !(ismaxrsv>0))
//		if((cnt<((sctx->vsp_tcnt*6)>>8)) && !(ismaxrsv>0))
			{ sctx->vsp_rsv=i; break; }

		if(BGBCC_CCXL_IsRegArgP(ctx, reg) && (obj->n_args>8))
			{ sctx->vsp_rsv=i; break; }

#if 0
		reg=sctx->vspan[i]->reg;
		k=BGBCC_JX2C_GetFrameVRegFlags(ctx, sctx, reg);
//		if(BGBCC_CCXL_IsRegGlobalP(ctx, reg) ||
//			BGBCC_CCXL_IsRegArgP(ctx, reg) ||
//				(k&BGBCC_REGFL_ALIASPTR))
		if(k&BGBCC_REGFL_ALIASPTR)
			{ sctx->vsp_rsv=i; break; }

		if(
			!BGBCC_CCXL_IsRegTempP(ctx, reg) &&
			!BGBCC_CCXL_IsRegArgP(ctx, reg) &&
			!BGBCC_CCXL_IsRegLocalP(ctx, reg))
				{ sctx->vsp_rsv=i; break; }

		tty=BGBCC_CCXL_GetRegType(ctx, reg);
		if(BGBCC_CCXL_TypeValueObjectP(ctx, tty) ||
			BGBCC_CCXL_TypeArrayP(ctx, tty) ||
			BGBCC_CCXL_TypeSgLongP(ctx, tty) ||
			BGBCC_CCXL_TypeVoidP(ctx, tty) ||
			BGBCC_CCXL_TypeRealP(ctx, tty) ||
			BGBCC_CCXL_TypeVec128P(ctx, tty) ||
			BGBCC_CCXL_TypeQuadPointerP(ctx, tty) ||
			BGBCC_CCXL_TypeQuadVariantP(ctx, tty) ||
			BGBCC_CCXL_TypeQuadVarObjP(ctx, tty) ||
			BGBCC_CCXL_TypeQuadVarRefP(ctx, tty))
			{ sctx->vsp_rsv=i; break; }

//		if(BGBCC_CCXL_IsRegSgLongP(ctx, reg))
//			{ sctx->vsp_rsv=i; break; }
//		if(BGBCC_CCXL_IsRegFloatP(ctx, reg))
//			{ sctx->vsp_rsv=i; break; }
//		if(BGBCC_CCXL_IsRegDoubleP(ctx, reg))
//			{ sctx->vsp_rsv=i; break; }
#endif
	}
#endif

	k=0;

#if 0
	if(!(sctx->is_leaf&1))
	{
		BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_STSL,
			BGBCC_SH_REG_PR, BGBCC_SH_REG_SP);
//		k++;
		k+=sctx->is_addr64?2:1;
	}
#endif

//	j=BGBCC_JX2_EmitCheckAutoLabelNear16B(sctx, epilbl);
//	epij=BGBCC_JX2_EmitCheckAutoLabelNear16B(sctx, epilbl);
	if(epik && (epilbl>0) && (epij>0))
	{
#if 1
//		BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_STSL,
//			BGBCC_SH_REG_PR, BGBCC_SH_REG_SP);
		if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
		{
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_PR);
			k+=4;

			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_PR, BGBCC_SH_REG_R19);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_GBR, BGBCC_SH_REG_R18);

		}else
		{
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//				BGBCC_SH_REG_PR, BGBCC_SH_REG_R16);
				BGBCC_SH_REG_PR, BGBCC_SH_REG_R1);
//			k+=sctx->is_addr64?2:1;
			k+=2;
		}
#endif

#if 0
		if(obj->regflags&BGBCC_REGFL_ALIASPTR)
		{
			if(sctx->use_wexmd)
			{
				BGBCC_JX2_EmitOpImm(sctx,
					BGBCC_SH_NMID_WEXMD, sctx->use_wexmd);
			}
		}

//		if(sctx->is_pbo)
		if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
		{
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_GBR);
			k+=sctx->is_addr64?2:1;

			j=BGBCC_JX2_GetNamedLabel(sctx, "__global_ptr");
			BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
				BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R1, j);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R1, BGBCC_SH_REG_GBR);
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
		if(obj->regflags&BGBCC_REGFL_ALIASPTR)
		{
			if(sctx->use_wexmd)
			{
				BGBCC_JX2_EmitOpImm(sctx,
					BGBCC_SH_NMID_WEXMD, sctx->use_wexmd);
			}
		}

//		if(sctx->is_pbo)
		if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
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
				BGBCC_SH_REG_GBR, 0, BGBCC_SH_REG_R18);

			if(sctx->has_fmovc)
			{
				BGBCC_JX2_EmitRelocTy(sctx, j, BGBCC_SH_RLC_TBR12_BJX);
				BGBCC_JX2_EmitOpLdRegDispReg(sctx,
					BGBCC_SH_NMID_MOVC_DISP24,
					BGBCC_SH_REG_R18, 0, BGBCC_SH_REG_GBR);
			}else
			{
				BGBCC_JX2_EmitRelocTy(sctx, j, BGBCC_SH_RLC_TBR12_BJX);
				BGBCC_JX2_EmitOpLdRegDispReg(sctx,
					BGBCC_SH_NMID_MOVQ_DISP24,
					BGBCC_SH_REG_R18, 0, BGBCC_SH_REG_R18);

				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_R18, BGBCC_SH_REG_GBR);
			}
		}
#endif

//		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);

#if 1
	//	for(i=14; i>0; i--)
//		for(i=31; i>0; i--)
		for(i=63; i>0; i--)
		{
			if(!(sctx->has_xgpr&1) && (i>=32))
				continue;
			if(!(sctx->has_bjx1egpr) && (i>=16))
				continue;
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
			if(!(sctx->has_xgpr&1) && (i>=32))
				continue;
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
#if 0
//		if(!(sctx->is_leaf&1) && !(obj->flagsint&BGBCC_TYFL_INTERRUPT))
		if(!(sctx->is_leaf&1) || (obj->flagsint&BGBCC_TYFL_INTERRUPT) ||
			(obj->flagsint&BGBCC_TYFL_SYSCALL))
//		if(!(sctx->is_leaf&1) || (obj->flagsint&BGBCC_TYFL_INTERRUPT) ||
//			(obj->flagsint&BGBCC_TYFL_SYSCALL) ||
//			(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR)))
		{
//			BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_STSL,
//				BGBCC_SH_REG_PR, BGBCC_SH_REG_SP);
			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_PR);
			k+=sctx->is_addr64?2:1;
		}

		if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
		{
			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_GBR);
			k+=sctx->is_addr64?2:1;
		}
#endif

		fl=0;

#if 1
		if(sctx->is_leaf&1)
		{
			if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
			{
				fl=1;
				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_GBR, BGBCC_SH_REG_R1);
			}else
			{
				fl=0;
			}
		}else
			if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
		{
			fl=2;
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_PR, BGBCC_SH_REG_R19);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_GBR, BGBCC_SH_REG_R18);
		}else
		{
			fl=1;
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_PR, BGBCC_SH_REG_R1);
		}
#endif

		if(obj->regflags&BGBCC_REGFL_ALIASPTR)
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
		if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
		{
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_GBR);
//			k+=sctx->is_addr64?2:1;

			j=BGBCC_JX2_GetNamedLabel(sctx, "__global_ptr");
			BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
				BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R1, j);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R1, BGBCC_SH_REG_GBR);
		}

	}
	else
	{
#if 0
//		if(!(sctx->is_leaf&1) && !(obj->flagsint&BGBCC_TYFL_INTERRUPT))
//		if(!(sctx->is_leaf&1) || (obj->flagsint&BGBCC_TYFL_INTERRUPT) ||
//			(obj->flagsint&BGBCC_TYFL_SYSCALL))
		if(1)
		{
//			BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_STSL,
//				BGBCC_SH_REG_PR, BGBCC_SH_REG_SP);

			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_PR);
			k+=sctx->is_addr64?2:1;
		}
#endif

#if 0
//		if(sctx->is_pbo)
		if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
		{
			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_GBR);
			k+=sctx->is_addr64?2:1;

//			j=BGBCC_JX2_GetNamedLabel(sctx, "__global_ptr");
//			BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
//				BGBCC_SH_NMID_MOVQ,
//				BGBCC_SH_REG_R1, j);
//			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//				BGBCC_SH_REG_R1, BGBCC_SH_REG_GBR);
		}
#endif

#if 1
		if(obj->flagsint&BGBCC_TYFL_INTERRUPT)
		{
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_PR);
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_GBR);
			k+=8;

			if(sctx->has_fmovc)
			{
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
				BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
					-40, BGBCC_SH_REG_SP);

				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 0);

				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_EXSR, BGBCC_SH_REG_R1);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 32);

				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_SPC, BGBCC_SH_REG_R1);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 24);


				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_PR, BGBCC_SH_REG_R1);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 16);

				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_GBR, BGBCC_SH_REG_R1);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 8);

				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_SP, 0, BGBCC_SH_REG_R1);
				BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
					8, BGBCC_SH_REG_SP);
			}
		}
		else
			if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
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

				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 0);

				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_PR, BGBCC_SH_REG_R1);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 16);

				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_GBR, BGBCC_SH_REG_R1);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 8);

				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_SP, 0, BGBCC_SH_REG_R1);
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
				BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
					-16, BGBCC_SH_REG_SP);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 0);

				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_PR, BGBCC_SH_REG_R1);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 8);

				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_SP, 0, BGBCC_SH_REG_R1);

				BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
					8, BGBCC_SH_REG_SP);
			}
		}
#endif

		if(obj->flagsint&BGBCC_TYFL_INTERRUPT)
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
					BGBCC_SH_REG_R0+1, BGBCC_SH_REG_SP, 1*8);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R0+0, BGBCC_SH_REG_SP, 0*8);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R14, BGBCC_SH_REG_SP, 14*8);

				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_SGR, BGBCC_SH_REG_R1);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 15*8);

				if(sctx->has_xgpr&1)
				{
					for(i=62; i>=32; i-=2)
					{
						BGBCC_JX2_EmitOpRegStRegDisp(sctx, 
							BGBCC_SH_NMID_MOVX2,
							BGBCC_SH_REG_R0+i, BGBCC_SH_REG_SP, i*8);
					}
				}

				for(i=30; i>=2; i-=2)
				{
					if(i==14)
						continue;
					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVX2,
						BGBCC_SH_REG_R0+i, BGBCC_SH_REG_SP, i*8);
				}
			}else
			{
				for(i=31; i>=0; i--)
				{
					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
						BGBCC_SH_REG_R0+i, BGBCC_SH_REG_SP, i*8);
				}
			}
#endif

#if 0
			if(sctx->has_pushx2)
			{
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R0+1, BGBCC_SH_REG_SP, 1*8);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R0+0, BGBCC_SH_REG_SP, 0*8);

				for(i=22; i>=16; i-=2)
				{
					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVX2,
						BGBCC_SH_REG_R0+i, BGBCC_SH_REG_SP, ((i-16)+8)*8);
				}

				for(i=6; i>=2; i-=2)
				{
					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVX2,
						BGBCC_SH_REG_R0+i, BGBCC_SH_REG_SP, i*8);
				}
			}else
			{
				if(sctx->has_bjx1egpr)
				{
					for(i=23; i>=16; i--)
					{
						BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
							BGBCC_SH_REG_R0+i, BGBCC_SH_REG_SP, ((i-16)+8)*8);
					}
				}

				for(i=7; i>=0; i--)
				{
					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
						BGBCC_SH_REG_R0+i, BGBCC_SH_REG_SP, i*8);
				}
			}
#endif

#endif

#if 0
			if(sctx->has_bjx1egpr)
			{
				for(i=23; i>=16; i--)
				{
					BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH,
						BGBCC_SH_REG_R0+i);
					k+=sctx->is_addr64?2:1;
				}
			}

			for(i=7; i>=0; i--)
			{
				BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH,
					BGBCC_SH_REG_R0+i);
				k+=sctx->is_addr64?2:1;
			}
#endif

//			BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_TRAPA,
//				3, BGBCC_SH_REG_SP);
		}

		sctx->frm_offs_isrsaves=sctx->frm_size-(k*4);

		fl2=0;
		k+=BGBCC_JX2C_EmitFrameProlog_PushRegs(ctx, sctx, 0, &fl2);

		if(obj->regflags&BGBCC_REGFL_ALIASPTR)
		{
			if(sctx->use_wexmd)
			{
				BGBCC_JX2_EmitOpImm(sctx,
					BGBCC_SH_NMID_WEXMD, sctx->use_wexmd);
			}
		}

		if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
		{
			j=BGBCC_JX2_GetNamedLabel(sctx, "__global_ptr");
			BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
				BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R1, j);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R1, BGBCC_SH_REG_GBR);
		}
	}


	BGBCC_JX2C_ResetModeDqUnknown(ctx, sctx);
//	BGBCC_JX2C_SetModeDqSet(ctx, sctx);
//	BGBCC_JX2C_ResetModeDqLocal(ctx, sctx);

//	BGBCC_JX2_EmitWord(sctx, 0x0858);	//Debug, Set Token
	
	sctx->frm_offs_save=sctx->frm_size-(k*4);

	BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
		-sctx->frm_offs_save, BGBCC_SH_REG_SP);

#if 0
	p0=BGBCC_JX2_TryEmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
		-sctx->frm_offs_save, BGBCC_SH_REG_SP);
	if(p0<=0)
	{
#if 0
//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
//			BGBCC_SH_REG_R0, sctx->frm_offs_save);
//		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_SUB,
//			BGBCC_SH_REG_R0, BGBCC_SH_REG_SP);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
#endif

#if 1
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R2);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R2, sctx->frm_offs_save);
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_SUB,
			BGBCC_SH_REG_R2, BGBCC_SH_REG_SP);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R2);
#endif

	}
#endif

#if 0
	if(sctx->is_pic)
	{
//		BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, -16, BGBCC_SH_REG_R12);
		BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, BGBCC_SH_REG_R12);

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitWord(sctx, 0xC700);
		BGBCC_JX2_EmitLoadRegLabelRel(sctx, BGBCC_SH_REG_R12, sctx->lbl_got, 0);
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_R0, BGBCC_SH_REG_R12);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
	}
#endif

//	BGBCC_JX2C_ResetFpscrUnknown(ctx, sctx);

	k=sctx->frm_offs_retstr;
	if(k)
	{
		obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;

		if(sctx->has_xgpr&2)
		{
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
				k, BGBCC_SH_REG_LR2);
		}else
			if(sctx->is_addr64)
		{
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_RQ2);
		}
		else
		{
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_R2);
		}

//		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_BRK);
	}
	
	k=sctx->frm_offs_thisptr;
	if(k)
	{
		obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;

		if(sctx->has_xgpr&2)
		{
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
				k, BGBCC_SH_REG_LR18);
		}else
			if(sctx->is_addr64)
		{
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_RQ3);
		}
		else
		{
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_R3);
		}
	}

	
	k=sctx->frm_offs_sectoken;
	if(k)
	{
		BGBCC_JX2_EmitLoadRegImm64P(sctx, BGBCC_SH_REG_RQ0,
//			0x1234567890ABCDEFLL);
			sctx->frm_val_sectoken);
		BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_RQ0);
	}

	if(	sctx->regalc_live || sctx->fregalc_live ||
		sctx->sreg_live || sctx->sfreg_live)
			{ BGBCC_DBGBREAK }

	sctx->sreg_live|=0x00F000F0ULL;

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
			if(sctx->is_addr64)
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
				if(ni>=12)
				{
					BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
						treg, BGBCC_SH_REG_RD52+(ni-12));
					ni++;
					break;
				}
				if(ni>=8)
				{
					BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
						treg, BGBCC_SH_REG_RD36+(ni-8));
					ni++;
					break;
				}
				if(ni>=4)
				{
//					if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
//					if(1)
						BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
							treg, BGBCC_SH_REG_RD20+(ni-4));
//					else
//						BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//							k, BGBCC_SH_REG_RD20+(ni-4));
					ni++;
					break;
				}
//				if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
//				if(1)
					BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
						treg, BGBCC_SH_REG_RD4+ni);
//				else
//					BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//						k, BGBCC_SH_REG_RD4+ni);
				ni++;
				break;
			}

			if(ni>=4)
				break;
			k=obj->args[i]->fxoffs;
//			if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
//			if(1)
				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_R4+ni);
//			else
//				BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//					k, BGBCC_SH_REG_R4+ni);
			ni++;
			break;

		case BGBCC_SH_REGCLS_VO_GR:
			if(sctx->is_addr64)
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

				if(ni>=12)
				{
					BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
						treg, BGBCC_SH_REG_RD52+(ni-12));
					ni++;
					break;
				}
				if(ni>=8)
				{
					BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
						treg, BGBCC_SH_REG_RD36+(ni-8));
					ni++;
					break;
				}

				if(ni>=4)
				{
//					if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
//					if(1)
						BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
							treg, BGBCC_SH_REG_RD20+(ni-4));
//					else
//						BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//							k, BGBCC_SH_REG_RD20+(ni-4));
					ni++;
					break;
				}
//				if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
//				if(1)
					BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
						treg, BGBCC_SH_REG_RD4+ni);
//				else
//					BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//						k, BGBCC_SH_REG_RD4+ni);
				ni++;
				break;
			}

			if(ni>=4)
				break;
			k=obj->args[i]->fxoffs;
//			if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
//			if(1)
				BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
					treg, BGBCC_SH_REG_R4+ni);
//			else
//				BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//					k, BGBCC_SH_REG_R4+ni);
			ni++;
			break;

		case BGBCC_SH_REGCLS_VO_REF:
		case BGBCC_SH_REGCLS_AR_REF:
//		case BGBCC_SH_REGCLS_VO_QGR:
		case BGBCC_SH_REGCLS_QGR:
			if(sctx->is_addr64)
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

				if(ni>=12)
				{
					BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
						treg, BGBCC_SH_REG_RQ52+(ni-12));
					ni++;
					break;
				}
				if(ni>=8)
				{
					BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
						treg, BGBCC_SH_REG_RQ36+(ni-8));
					ni++;
					break;
				}

				if(ni>=4)
				{
//					if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
//					if(1)
						BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
							treg, BGBCC_SH_REG_RQ20+(ni-4));
//					else
//						BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//							k, BGBCC_SH_REG_RQ20+(ni-4));
					ni++;
					break;
				}
//				if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
//				if(1)
					BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
						treg, BGBCC_SH_REG_RQ4+ni);
//				else
//					BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//						k, BGBCC_SH_REG_RQ4+ni);
				ni++;
				break;
			}

			if(ni>=4)
				break;
			k=obj->args[i]->fxoffs;
//			if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
//			if(1)
				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_R4+ni);
//			else
//				BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//					k, BGBCC_SH_REG_R4+ni);
			ni++;
			break;

		case BGBCC_SH_REGCLS_VO_QGR:
//		case BGBCC_SH_REGCLS_QGR:
			if(sctx->is_addr64)
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

				if(ni>=12)
				{
					BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
						treg, BGBCC_SH_REG_RQ52+(ni-12));
					ni++;
					break;
				}
				if(ni>=8)
				{
					BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
						treg, BGBCC_SH_REG_RQ36+(ni-8));
					ni++;
					break;
				}

				if(ni>=4)
				{
//					if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
//					if(1)
						BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
							treg, BGBCC_SH_REG_RQ20+(ni-4));
//					else
//						BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//							k, BGBCC_SH_REG_RQ20+(ni-4));
					ni++;
					break;
				}
//				if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
//				if(1)
					BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
						treg, BGBCC_SH_REG_RQ4+ni);
//				else
//					BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//						k, BGBCC_SH_REG_RQ4+ni);
				ni++;
				break;
			}

			if(ni>=4)
				break;
			k=obj->args[i]->fxoffs;
//			if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
//			if(1)
				BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
					treg, BGBCC_SH_REG_R4+ni);
//			else
//				BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//					k, BGBCC_SH_REG_R4+ni);
			ni++;
			break;

		case BGBCC_SH_REGCLS_GR2:
		case BGBCC_SH_REGCLS_QGR2:
		case BGBCC_SH_REGCLS_VO_GR2:
		case BGBCC_SH_REGCLS_VO_QGR2:
		case BGBCC_SH_REGCLS_VO_REF2:
		case BGBCC_SH_REGCLS_AR_REF2:
			if(sctx->abi_evenonly)
				ni=(ni+1)&(~1);
		
			if(sctx->is_addr64)
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

				if(ni>=12)
				{
					BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
						treg, BGBCC_SH_REG_LR52+(ni-12));
					ni+=2;
					break;
				}
				if(ni>=8)
				{
					BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
						treg, BGBCC_SH_REG_LR36+(ni-8));
					ni+=2;
					break;
				}
				
				if(sctx->has_xgpr&2)
				{
					if(ni>=4)
					{
						BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
							treg, BGBCC_SH_REG_LR20+(ni-4));
						ni+=2;
						break;
					}

					BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
						treg, BGBCC_SH_REG_LR4+(ni-0));
					ni+=2;
					break;
				}

				if(ni>=4)
				{
					BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
						k+0, BGBCC_SH_REG_RQ20+(ni-4)+0);
					BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
						k+8, BGBCC_SH_REG_RQ20+(ni-4)+1);
					ni+=2;
					break;
				}
				if(ni==3)
				{
					BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
						k+0, BGBCC_SH_REG_RQ7);
					BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
						k+8, BGBCC_SH_REG_RQ20);
					ni+=2;
					break;
				}
				BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
					k+0, BGBCC_SH_REG_RQ4+ni+0);
				BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
					k+8, BGBCC_SH_REG_RQ4+ni+1);
				ni+=2;
				break;
			}

			if(ni>=3)
				break;
			k=obj->args[i]->fxoffs;
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
				k, BGBCC_SH_REG_R4+ni);
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
				k+4, BGBCC_SH_REG_R4+ni+1);
			ni+=2;
			break;
#if 0
		case BGBCC_SH_REGCLS_FR:
			if(nf>=4)
				break;
			k=obj->args[i]->fxoffs;
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_FR4+nf);
			nf++;
			break;
		case BGBCC_SH_REGCLS_FR2:
			nf+=nf&1;
			if(nf>=7)
				break;
			k=obj->args[i]->fxoffs;
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
				k+4, BGBCC_SH_REG_FR4+nf+0);
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
				k+0, BGBCC_SH_REG_FR4+nf+1);
			nf+=2;
			break;
		case BGBCC_SH_REGCLS_DR:
			if(nf>=4)
				break;
			k=obj->args[i]->fxoffs;
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_DR4+nf);
			nf++;
			break;
#endif
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

		if((rcls==BGBCC_SH_REGCLS_VO_REF) ||
			(rcls==BGBCC_SH_REGCLS_AR_REF) ||
			(rcls==BGBCC_SH_REGCLS_VO_REF2) ||
			(rcls==BGBCC_SH_REGCLS_AR_REF2))
		{
			j=obj->locals[i]->fxmoffs+(sctx->frm_offs_fix);
			k=obj->locals[i]->fxoffs;

//			if(sctx->has_xgpr&2)
			if(ctx->arch_sizeof_ptr==16)
			{
				BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR16);
				BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx,
					j, BGBCC_SH_REG_LR16);
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
				BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R3);
				if(sctx->is_addr64)
				{
					BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx,
						j, BGBCC_SH_REG_RQ3);
					BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
						k, BGBCC_SH_REG_RQ3);
				}else
				{
					BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx,
						j, BGBCC_SH_REG_R3);
					BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
						k, BGBCC_SH_REG_R3);
				}
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R3);
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
			if(sctx->is_addr64)
			{
				for(j=0; j<4; j++)
				{
					BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
//					if(sctx->has_bjx1mov && !sctx->no_ext32)
//					if(0)
					if(1)
					{
						BGBCC_JX2_EmitOpRegStRegDisp(sctx,
							BGBCC_SH_NMID_MOVQ,
							BGBCC_SH_REG_RQ4+j, BGBCC_SH_REG_RQ3, (j+0)*8);
					}else
					{
#if 0
//						BGBCC_JX2C_ScratchSafeStompReg(
//							ctx, sctx, BGBCC_SH_REG_R0);
//						BGBCC_JX2_EmitLoadRegImm(sctx,
//							BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, j*8);
//						BGBCC_JX2_EmitOpRegStReg2(sctx,
//							BGBCC_SH_NMID_MOVQ, BGBCC_SH_REG_RQ4+j,
//							BGBCC_SH_REG_RQ0, BGBCC_SH_REG_RQ3);
//						BGBCC_JX2C_ScratchReleaseReg(
//							ctx, sctx, BGBCC_SH_REG_R0);
#endif
					}
				}

				if(sctx->has_bjx1egpr)
				{
					for(j=0; j<4; j++)
					{
						BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
//						if(sctx->has_bjx1mov && !sctx->no_ext32)
//						if(0)
						if(1)
						{
							BGBCC_JX2_EmitOpRegStRegDisp(sctx,
								BGBCC_SH_NMID_MOVQ,
								BGBCC_SH_REG_RQ20+j,
								BGBCC_SH_REG_RQ3, (j+4)*8);
						}else
						{
#if 0
//							BGBCC_JX2C_ScratchSafeStompReg(
//								ctx, sctx, BGBCC_SH_REG_R0);
//							BGBCC_JX2_EmitLoadRegImm(sctx,
//								BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, (j+4)*8);
//							BGBCC_JX2_EmitOpRegStReg2(sctx,
//								BGBCC_SH_NMID_MOVQ, BGBCC_SH_REG_RQ20+j,
//								BGBCC_SH_REG_RQ0, BGBCC_SH_REG_RQ3);
//							BGBCC_JX2C_ScratchReleaseReg(
//								ctx, sctx, BGBCC_SH_REG_R0);
#endif
						}
					}
				}else
				{
#if 0
					BGBCC_JX2_EmitLoadRegImm(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1, 0x55AA55AA);
					for(j=0; j<4; j++)
					{
//						BGBCC_JX2_EmitLoadRegImm(sctx,
//							BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, (j+4)*8);
//						BGBCC_JX2_EmitOpRegStReg2(sctx,
//							BGBCC_SH_NMID_MOVQ, BGBCC_SH_REG_RQ1,
//							BGBCC_SH_REG_RQ0, BGBCC_SH_REG_RQ3);
					}
#endif
				}

				if(sctx->has_xgpr&2)
				{
					for(j=0; j<4; j++)
					{
						BGBCC_JX2_EmitOpRegStRegDisp(sctx,
							BGBCC_SH_NMID_MOVQ,
							BGBCC_SH_REG_RQ36+j,
							BGBCC_SH_REG_RQ3, (j+8)*8);
						BGBCC_JX2_EmitOpRegStRegDisp(sctx,
							BGBCC_SH_NMID_MOVQ,
							BGBCC_SH_REG_RQ52+j,
							BGBCC_SH_REG_RQ3, (j+12)*8);
					}
				}

#if 0
				if(!sctx->no_fpu && !sctx->fpu_gfp)
				{
					for(j=0; j<4; j++)
					{
						BGBCC_JX2_EmitOpRegStRegDisp(sctx,
							BGBCC_SH_NMID_FMOVD,
							BGBCC_SH_REG_DR4+j, BGBCC_SH_REG_RQ3, 64+(j*8));
					}
					
					
//					BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_BRK);

#if 0
					for(j=0; j<8; j++)
					{
						if(sctx->has_bjx1mov && !sctx->no_ext32)
	//					if(0)
						{
							BGBCC_JX2_EmitOpRegStRegDisp(sctx,
								BGBCC_SH_NMID_FMOVS,
								BGBCC_SH_REG_FR4+j, BGBCC_SH_REG_RQ3, 64+(j*4));
						}else
						{
//							BGBCC_JX2C_ScratchSafeStompReg(
//								ctx, sctx, BGBCC_SH_REG_R0);
//							BGBCC_JX2_EmitLoadRegImm(sctx,
//								BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, 64+(j*4));
//							BGBCC_JX2_EmitOpRegStReg2(sctx,
//								BGBCC_SH_NMID_FMOVS, BGBCC_SH_REG_FR4+j,
//								BGBCC_SH_REG_RQ0, BGBCC_SH_REG_RQ3);
//							BGBCC_JX2C_ScratchReleaseReg(
//								ctx, sctx, BGBCC_SH_REG_R0);
						}
					}
#endif
				}
#endif

//				BGBCC_JX2_EmitLoadRegImm(sctx,
//					BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, 96);
//				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
//					BGBCC_SH_REG_R3, BGBCC_SH_REG_R0);

//				for(j=7; j>=0; j--)
//				{
//					BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_FMOVS,
//						BGBCC_SH_REG_FR4+j, BGBCC_SH_REG_R0);
//				}

//				if(sctx->has_xgpr&2)
				if(1)
				{
					BGBCC_JX2_EmitLoadRegImm(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1, ni*8);
					BGBCC_JX2_EmitOpRegStRegDisp(sctx,
						BGBCC_SH_NMID_MOVQ, BGBCC_SH_REG_R1,
						BGBCC_SH_REG_R3, 128);

					BGBCC_JX2_EmitLoadRegImm(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1, sctx->frm_size);
					BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
						BGBCC_SH_REG_SP, BGBCC_SH_REG_R1);

					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
						BGBCC_SH_REG_R1, BGBCC_SH_REG_R3, 144);
				}else
				{
					BGBCC_JX2_EmitLoadRegImm(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1, ni*8);
					BGBCC_JX2_EmitOpRegStRegDisp(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1,
						BGBCC_SH_REG_R3, 96);

					BGBCC_JX2_EmitLoadRegImm(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1, 64+nf*8);
					BGBCC_JX2_EmitOpRegStRegDisp(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1,
						BGBCC_SH_REG_R3, 104);

					BGBCC_JX2_EmitLoadRegImm(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1, sctx->frm_size);
					BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
						BGBCC_SH_REG_SP, BGBCC_SH_REG_R1);

					BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
						BGBCC_SH_REG_R1, BGBCC_SH_REG_R3, 112);
				}

//				BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_BRK);
			}else
			{
			
				BGBCC_DBGBREAK

#if 0
				for(j=0; j<4; j++)
				{
					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
						BGBCC_SH_REG_R4+j, BGBCC_SH_REG_R3, j*4);
				}

				if(!sctx->no_fpu && !sctx->fpu_gfp)
				{
//					BGBCC_JX2_EmitLoadRegImm(sctx,
//						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, 48);
//					BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
//						BGBCC_SH_REG_R3, BGBCC_SH_REG_R0);

//					for(j=7; j>=0; j--)
					for(j=3; j>=0; j--)
					{
//						BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_FMOVD,
//							BGBCC_SH_REG_FR4+j, BGBCC_SH_REG_R0);
					}
				}

//				BGBCC_JX2_EmitLoadRegImm(sctx,
//					BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, ni*4);
//				BGBCC_JX2_EmitOpRegStRegDisp(sctx,
//					BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, BGBCC_SH_REG_R3, 48);

//				BGBCC_JX2_EmitLoadRegImm(sctx,
//					BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, 16+nf*4);
//				BGBCC_JX2_EmitOpRegStRegDisp(sctx,
//					BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, BGBCC_SH_REG_R3, 52);

#if 1
				BGBCC_JX2_EmitLoadRegImm(sctx,
					BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1, sctx->frm_size);
				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
					BGBCC_SH_REG_SP, BGBCC_SH_REG_R1);
#endif

				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
					BGBCC_SH_REG_R1, BGBCC_SH_REG_R3, 56);
#endif
			}
		}
	}

	k=sctx->frm_offs_allocamrk;
	if(k)
	{
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__alloca_begin");
		if(sctx->has_xgpr&2)
		{
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k,
				BGBCC_SH_REG_LR2);
		}
		else
		{
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_RQ2);
		}
	}

	sctx->sreg_live&=~0x00FF00FF00F000F0ULL;

	if(	sctx->regalc_live || sctx->fregalc_live ||
		sctx->sreg_live || sctx->sfreg_live)
			{ BGBCC_DBGBREAK }

#if 0
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

	sctx->lbl_ret=BGBCC_JX2_GenLabelTemp(sctx);
	
	if(sctx->cnt_set_fp64>sctx->cnt_set_fp32)
//	if(0)
	{
		sctx->dfl_fpscr=BGBCC_SH_FPSCR_PR;
	}else
	{
		sctx->dfl_fpscr=0;
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
	if(sctx->use_bp)sctx->vsp_rsv--;
	if(sctx->is_pic)sctx->vsp_rsv--;
	if(sctx->vsp_rsv<0)
		sctx->vsp_rsv=0;

	if(sctx->vspan_num<sctx->vsp_rsv)
		sctx->vsp_rsv=sctx->vspan_num;

	cnt=0;
	for(i=0; i<sctx->vsp_rsv; i++)
	{
		cnt+=sctx->vspan[i]->cnt;
		if(cnt<(sctx->vsp_tcnt/16))
			{ sctx->vsp_rsv=i; break; }
		
		reg=sctx->vspan[i]->reg;
		k=BGBCC_JX2C_GetFrameVRegFlags(ctx, sctx, reg);
//		if(BGBCC_CCXL_IsRegGlobalP(ctx, reg) ||
//			BGBCC_CCXL_IsRegArgP(ctx, reg) ||
//				(k&BGBCC_REGFL_ALIASPTR))
		if(k&BGBCC_REGFL_ALIASPTR)
			{ sctx->vsp_rsv=i; break; }

		if(
			!BGBCC_CCXL_IsRegTempP(ctx, reg) &&
			!BGBCC_CCXL_IsRegLocalP(ctx, reg))
				{ sctx->vsp_rsv=i; break; }

		tty=BGBCC_CCXL_GetRegType(ctx, reg);
		if(BGBCC_CCXL_TypeValueObjectP(ctx, tty) ||
			BGBCC_CCXL_TypeArrayP(ctx, tty) ||
			BGBCC_CCXL_TypeSgLongP(ctx, tty) ||
			BGBCC_CCXL_TypeVoidP(ctx, tty) ||
			BGBCC_CCXL_TypeRealP(ctx, tty))
			{ sctx->vsp_rsv=i; break; }

//		if(BGBCC_CCXL_IsRegSgLongP(ctx, reg))
//			{ sctx->vsp_rsv=i; break; }
//		if(BGBCC_CCXL_IsRegFloatP(ctx, reg))
//			{ sctx->vsp_rsv=i; break; }
//		if(BGBCC_CCXL_IsRegDoubleP(ctx, reg))
//			{ sctx->vsp_rsv=i; break; }
	}
#endif

	if(	sctx->regalc_live || sctx->fregalc_live ||
		sctx->sreg_live || sctx->sfreg_live)
			{ BGBCC_DBGBREAK }

#if 0
	for(i=0; i<sctx->vsp_rsv; i++)
	{
		reg=sctx->vspan[i].reg;
		BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, reg);
//		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, reg);
	}
#endif

	co=BGBCC_JX2_EmitGetOffs(sctx);
	sctx->fnsz_pro=co-bo;
	
	return(0);
}
