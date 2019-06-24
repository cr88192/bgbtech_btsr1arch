/*
Regs (ARM)
R0: Scratch (Arg1 / Result)
R1: Scratch (Arg2)
R2: Scratch (Arg3)
R3: Scratch (Arg4)
R4: Var Cache
R5: Var Cache
R6: Var Cache
R7: Var Cache
R8: Var Cache
R9: Platform Register
R10: VM CPU Context
R11: Current Trace
R12: IP (Intra-Procedure Scratch)
R13: SP
R14: LR
R15: PC

  R1/R0: Scratch / Immed
  R3/R2: Cache Reg 1
  R5/R4: Cache Reg 2
  R7/R6: Cache Reg 3
  R8, Constant Table Pointer

D0..D7: Register Temp Cache
D8..D15: Register Cache

 */

#define UA32_REG_RCCTX	UA32_REG_R10
#define UA32_REG_RCTR	UA32_REG_R11
#define UA32_REG_RCCTAB	UA32_REG_R8

int BJX2_JitLoadVMRegGr2(UA32_Context *jctx, int idx, int reg)
{
	UA32T_EmitOpMemRegRegDisp(jctx, UA32_NMID_LDR,
		reg+0, UA32_REG_RCCTX, offsetof(BJX2_Context, regs)+(idx*8)+0);
	UA32T_EmitOpMemRegRegDisp(jctx, UA32_NMID_LDR,
		reg+1, UA32_REG_RCCTX, offsetof(BJX2_Context, regs)+(idx*8)+4);
	return(0);
}

int BJX2_JitStoreVMRegGr2(UA32_Context *jctx, int idx, int reg)
{
	UA32T_EmitOpMemRegRegDisp(jctx, UA32_NMID_STR,
		reg+0, UA32_REG_RCCTX, offsetof(BJX2_Context, regs)+(idx*8)+0);
	UA32T_EmitOpMemRegRegDisp(jctx, UA32_NMID_STR,
		reg+1, UA32_REG_RCCTX, offsetof(BJX2_Context, regs)+(idx*8)+4);
	return(0);
}

int BJX2_JitStoreVMRegImm(UA32_Context *jctx, int idx, s64 val)
{
//	UA32T_EmitLoadImm64(jctx, UA32_REG_R0, val);
	UA32T_EmitLoadCtImm64(jctx, UA32_REG_R0, UA32_REG_RCCTAB, val);
	BJX2_JitStoreVMRegGr2(jctx, idx, UA32_REG_R0);
	return(0);
}

int BJX2_JitSyncRegIdx(UA32_Context *jctx, int ix)
{
	int rd;

	if(jctx->reg_valid&(1<<ix))
	{
		if(jctx->reg_dirty&(1<<ix))
		{
			rd=UA32_REG_R2+(ix*2);
			BJX2_JitStoreVMRegGr2(jctx, jctx->reg_idx[ix], rd);
			jctx->reg_dirty&=~(1<<ix);
			return(1);
		}
	}
	return(0);
}

int BJX2_JitFlushRegIdx(UA32_Context *jctx, int ix)
{
	BJX2_JitSyncRegIdx(jctx, ix);
	jctx->reg_valid&=~(1<<ix);
	return(0);
}

int BJX2_JitSyncRegs(UA32_Context *jctx)
{
	int i;
	
	for(i=0; i<3; i++)
	{
//		BJX2_JitSyncRegIdx(jctx, i);
		BJX2_JitFlushRegIdx(jctx, i);
	}
	
	jctx->reg_live=0;
	jctx->reg_valid=0;
	jctx->reg_dirty=0;
	return(0);
}

int BJX2_JitGetVMRegRd(UA32_Context *jctx, int idx)
{
	int rd;
	int i;

	for(i=0; i<3; i++)
	{
		if(!(jctx->reg_valid&(1<<i)))
			continue;

		if(jctx->reg_idx[i]==idx)
		{
			jctx->reg_live|=(1<<i);
			return(UA32_REG_R2+(i*2));
		}
	}

	for(i=0; i<3; i++)
	{
		if(jctx->reg_live&(1<<i))
			continue;
		
		BJX2_JitSyncRegIdx(jctx, i);

		jctx->reg_valid|=(1<<i);
		jctx->reg_live|=(1<<i);
		jctx->reg_idx[i]=idx;
		rd=UA32_REG_R2+(i*2);
		BJX2_JitLoadVMRegGr2(jctx, idx, rd);
		return(rd);
	}
	
	return(-1);
}

int BJX2_JitGetVMRegWr(UA32_Context *jctx, int idx)
{
	int i;

	for(i=0; i<3; i++)
	{
		if(!(jctx->reg_valid&(1<<i)))
			continue;

		if(jctx->reg_idx[i]==idx)
		{
			jctx->reg_live|=(1<<i);
			jctx->reg_dirty|=(1<<i);
			return(UA32_REG_R2+(i*2));
		}
	}

	for(i=0; i<3; i++)
	{
		if(jctx->reg_live&(1<<i))
			continue;

		BJX2_JitSyncRegIdx(jctx, i);

		jctx->reg_valid|=(1<<i);
		jctx->reg_dirty|=(1<<i);
		jctx->reg_live|=(1<<i);
		jctx->reg_idx[i]=idx;
		return(UA32_REG_R2+(i*2));
	}
	
	return(-1);
}

int BJX2_JitGetRegIdx(UA32_Context *jctx, int rd)
{
	int i;
	
	if(rd<0)
		return(-1);
	i=(rd-2)>>1;
	if(i< 0)return(-1);
	if(i>=3)return(-1);
	return(i);
}

int BJX2_JitGetVMRegRdWr(UA32_Context *jctx, int idx)
{
	int rd;
	int i;
	
	rd=BJX2_JitGetVMRegRd(jctx, idx);
	i=BJX2_JitGetRegIdx(jctx, rd);
	
	if(i>=0)
		{ jctx->reg_dirty|=(1<<i); }
	return(rd);
}

int BJX2_JitReleaseReg(UA32_Context *jctx, int rd)
{
	int i;

	i=BJX2_JitGetRegIdx(jctx, rd);
	if(i>=0)
		{ jctx->reg_live&=~(1<<i); }
	return(1);
}

int BJX2_TryJitOpcode_MovReg(UA32_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *tr, BJX2_Opcode *op)
{
	int rd, rs, rt;
	if(op->nmid==BJX2_NMID_MOV)
	{
		if(op->fmid==BJX2_FMID_REGREG)
		{
#if 1
			rs=BJX2_JitGetVMRegRd(jctx, op->rm);
			rd=BJX2_JitGetVMRegWr(jctx, op->rn);
			UA32T_EmitOpRegReg(jctx, UA32_NMID_MOV, rd+0, rs+0);
			UA32T_EmitOpRegReg(jctx, UA32_NMID_MOV, rd+1, rs+1);
			BJX2_JitReleaseReg(jctx, rs);
			BJX2_JitReleaseReg(jctx, rd);
			return(1);
#endif
		}

#if 1
		if(op->fmid==BJX2_FMID_IMMREG)
		{
			rd=BJX2_JitGetVMRegWr(jctx, op->rn);
			UA32T_EmitLoadCtImm64(jctx, rd, UA32_REG_RCCTAB, (s32)(op->imm));
			BJX2_JitReleaseReg(jctx, rd);

//			BJX2_JitStoreVMRegImm(jctx, op->rn, (s32)(op->imm));
			return(1);
		}
#endif

#if 1
		if(op->fmid==BJX2_FMID_IMMZREG)
		{
			rd=BJX2_JitGetVMRegWr(jctx, op->rn);
			UA32T_EmitLoadCtImm64(jctx, rd, UA32_REG_RCCTAB, (u32)(op->imm));
			BJX2_JitReleaseReg(jctx, rd);
			return(1);
		}

		if(op->fmid==BJX2_FMID_IMMNREG)
		{
			rd=BJX2_JitGetVMRegWr(jctx, op->rn);
			UA32T_EmitLoadCtImm64(jctx, rd, UA32_REG_RCCTAB, 
				(~((1LL<<32)-1))|((u32)(op->imm)));
			BJX2_JitReleaseReg(jctx, rd);
			return(1);
		}
#endif
		
		return(0);
	}

//	return(0);

#if 1
	if(op->nmid==BJX2_NMID_LDI)
	{
		if(op->fmid==BJX2_FMID_IMMREG)
		{
			rd=BJX2_JitGetVMRegWr(jctx, op->rn);
			UA32T_EmitLoadCtImm64(jctx, rd, UA32_REG_RCCTAB, (s32)(op->imm));
			BJX2_JitReleaseReg(jctx, rd);
			return(1);
		}
	}
#endif

//	return(0);

#if 0
	if((op->nmid==BJX2_NMID_LDIZ) ||
		(op->nmid==BJX2_NMID_LDIN))
	{
		if(op->fmid==BJX2_FMID_IMM)
		{
			BJX2_JitStoreVMRegImm(jctx, 0, (s32)(op->imm));
			return(1);
		}
	}
#endif

#if 1
	if(op->nmid==BJX2_NMID_LDIZ)
	{
		if(op->fmid==BJX2_FMID_IMM)
		{
			rd=BJX2_JitGetVMRegWr(jctx, 0);
			UA32T_EmitLoadCtImm64(jctx, rd, UA32_REG_RCCTAB, (u32)(op->imm));
			BJX2_JitReleaseReg(jctx, rd);
			return(1);
		}

		if(op->fmid==BJX2_FMID_IMMREG)
		{
			rd=BJX2_JitGetVMRegWr(jctx, op->rn);
			UA32T_EmitLoadCtImm64(jctx, rd, UA32_REG_RCCTAB, (u32)(op->imm));
			BJX2_JitReleaseReg(jctx, rd);
			return(1);
		}
	}
#endif

//	return(0);

#if 1
	if(op->nmid==BJX2_NMID_LDIN)
	{
		if(op->fmid==BJX2_FMID_IMM)
		{
			rd=BJX2_JitGetVMRegWr(jctx, 0);
			UA32T_EmitLoadCtImm64(jctx, rd, UA32_REG_RCCTAB,
				(~((1LL<<32)-1))|((s32)(op->imm)));
			BJX2_JitReleaseReg(jctx, rd);
			return(1);
		}

		if(op->fmid==BJX2_FMID_IMMREG)
		{
			rd=BJX2_JitGetVMRegWr(jctx, op->rn);
			UA32T_EmitLoadCtImm64(jctx, rd, UA32_REG_RCCTAB,
				(~((1LL<<32)-1))|((s32)(op->imm)));
			BJX2_JitReleaseReg(jctx, rd);
			return(1);
		}
	}
#endif

//	return(0);

	if(op->nmid==BJX2_NMID_LDISH8)
	{
		if(op->fmid==BJX2_FMID_IMM)
		{
#if 1
			rd=BJX2_JitGetVMRegRdWr(jctx, 0);

			UA32T_EmitOpRegRegImm(jctx, UA32_NMID_LSR,
				UA32_REG_R1, rd+0, 24);
			UA32T_EmitOpRegImm(jctx, UA32_NMID_LSL, rd+1, 8);
			UA32T_EmitOpRegImm(jctx, UA32_NMID_LSL, rd+0, 8);
			UA32T_EmitOpRegReg(jctx, UA32_NMID_ADD,
				rd+1, UA32_REG_R1);
			UA32T_EmitOpRegImm(jctx, UA32_NMID_ADD,
				rd+0, op->imm);	
			BJX2_JitReleaseReg(jctx, rd);
			return(1);
#endif
		}
	}

//	return(0);

	if(op->nmid==BJX2_NMID_LDISH16)
	{
		if(op->fmid==BJX2_FMID_IMM)
		{
#if 1
			rd=BJX2_JitGetVMRegRdWr(jctx, 0);

			UA32T_EmitOpRegRegImm(jctx, UA32_NMID_LSR,
				UA32_REG_R1, rd+0, 16);
			UA32T_EmitOpRegImm(jctx, UA32_NMID_MOV,
				UA32_REG_R0, op->imm);

			UA32T_EmitOpRegImm(jctx, UA32_NMID_LSL, rd+1, 16);
			UA32T_EmitOpRegImm(jctx, UA32_NMID_LSL, rd+0, 16);
			UA32T_EmitOpRegReg(jctx, UA32_NMID_ADD,
				rd+1, UA32_REG_R1);
			UA32T_EmitOpRegReg(jctx, UA32_NMID_ADD,
				rd+0, UA32_REG_R0);
	
			BJX2_JitReleaseReg(jctx, rd);
			return(1);
#endif
		}

#if 1
		if(op->fmid==BJX2_FMID_IMMREG)
		{
#if 1
			rd=BJX2_JitGetVMRegRdWr(jctx, op->rn);

			UA32T_EmitOpRegRegImm(jctx, UA32_NMID_LSR,
				UA32_REG_R1, rd+0, 16);
			UA32T_EmitOpRegImm(jctx, UA32_NMID_MOV,
				UA32_REG_R0, op->imm);

			UA32T_EmitOpRegImm(jctx, UA32_NMID_LSL, rd+1, 16);
			UA32T_EmitOpRegImm(jctx, UA32_NMID_LSL, rd+0, 16);
			UA32T_EmitOpRegReg(jctx, UA32_NMID_ADD,
				rd+1, UA32_REG_R1);
			UA32T_EmitOpRegReg(jctx, UA32_NMID_ADD,
				rd+0, UA32_REG_R0);
	
			BJX2_JitReleaseReg(jctx, rd);
			return(1);
#endif
		}
#endif

	}

	return(0);
}

int BJX2_TryJitOpcode_ArithReg(UA32_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *tr, BJX2_Opcode *op)
{
	int rd, rs, rt;

	if(op->nmid==BJX2_NMID_ADD)
	{
		if(op->fmid==BJX2_FMID_REGREG)
		{
			rs=BJX2_JitGetVMRegRd(jctx, op->rm);
			rd=BJX2_JitGetVMRegRdWr(jctx, op->rn);
	
			UA32T_EmitOpRegRegReg(jctx, UA32_NMID_ADDS,
				rd+0, rd+0, rs+0);
			UA32T_EmitOpRegRegReg(jctx, UA32_NMID_ADC,
				rd+1, rd+1, rs+1);
	
			BJX2_JitReleaseReg(jctx, rs);
			BJX2_JitReleaseReg(jctx, rd);
			return(1);
		}
	
		if(op->fmid==BJX2_FMID_IMMREG)
		{
			UA32T_EmitLoadCtImm64(jctx, UA32_REG_R0,
				UA32_REG_RCCTAB, (s32)op->imm);
	
			rd=BJX2_JitGetVMRegRdWr(jctx, op->rn);
	
			UA32T_EmitOpRegRegReg(jctx, UA32_NMID_ADDS,
				rd+0, rd+0, UA32_REG_R0);
			UA32T_EmitOpRegRegReg(jctx, UA32_NMID_ADC,
				rd+1, rd+1, UA32_REG_R1);
	
			BJX2_JitReleaseReg(jctx, rd);
			return(1);
		}

		if(op->fmid==BJX2_FMID_REGREGREG)
		{
			rs=BJX2_JitGetVMRegRd(jctx, op->rm);
			rt=BJX2_JitGetVMRegRd(jctx, op->ro);
			rd=BJX2_JitGetVMRegWr(jctx, op->rn);
	
			UA32T_EmitOpRegRegReg(jctx, UA32_NMID_ADDS,
				rd+0, rs+0, rt+0);
			UA32T_EmitOpRegRegReg(jctx, UA32_NMID_ADC,
				rd+1, rs+1, rt+1);
	
			BJX2_JitReleaseReg(jctx, rs);
			BJX2_JitReleaseReg(jctx, rd);
			return(1);
		}

		return(0);
	}

	return(0);
}

int BJX2_TryJitOpcode(UA32_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *tr, BJX2_Opcode *op)
{
	if(BJX2_TryJitOpcode_MovReg(jctx, cpu, tr, op)>0)
		return(1);

	if(BJX2_TryJitOpcode_ArithReg(jctx, cpu, tr, op)>0)
		return(1);

	return(0);
}

int BJX2_TryJitTrace(BJX2_Context *cpu, BJX2_Trace *tr)
{
	UA32_Context *jctx;
	BJX2_Opcode *op;
	byte *ptr;
	int lbl_ctab;
	int sz;
	int i;

	jctx=UA32_AllocContext();
	if(!jctx)
		return(0);

	jctx->reg_resv=0;
	jctx->reg_live=0;
	jctx->reg_valid=0;
	jctx->reg_dirty=0;
	jctx->jitfl=0;
	jctx->isjq=0;

	lbl_ctab=UA32_GenLabelTemp(jctx);

	UA32_SetSection(jctx, UA32_CSEG_TEXT);

	UA32T_TryEmitOpImm(jctx, UA32_NMID_PUSH, 0x4FF0);

	UA32T_EmitOpRegReg(jctx, UA32_NMID_MOV, UA32_REG_RCCTX, UA32_REG_R0);
	UA32T_EmitOpRegReg(jctx, UA32_NMID_MOV, UA32_REG_RCTR, UA32_REG_R1);
	
	UA32T_EmitOpRegLabel(jctx, UA32_NMID_ADRW, UA32_REG_RCCTAB, lbl_ctab);

	UA32T_EmitOpMemRegRegDisp(jctx, UA32_NMID_LDR,
		UA32_REG_R3, UA32_REG_RCTR, offsetof(BJX2_Trace, lnknext));
	UA32T_EmitOpMemRegRegDisp(jctx, UA32_NMID_STR,
		UA32_REG_R3, UA32_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt));

	BJX2_JitStoreVMRegImm(jctx, BJX2_REG_PC, tr->addr_nxt);

	for(i=0; i<tr->n_ops; i++)
	{
		op=tr->ops[i];

		if(BJX2_TryJitOpcode(jctx, cpu, tr, op)>0)
			continue;

		BJX2_JitSyncRegs(jctx);

		UA32T_EmitOpRegReg(jctx, UA32_NMID_MOV, UA32_REG_R0, UA32_REG_R10);
//		UA32T_EmitLoadImm32(jctx, UA32_REG_R1, (nlint)(op));
		UA32T_EmitLoadCtImm32(jctx, UA32_REG_R1,
			UA32_REG_RCCTAB, (nlint)(op));

//		UA32T_EmitLoadImm32(jctx, UA32_REG_R3, (nlint)(op->Run));
		UA32T_EmitLoadCtImm32(jctx, UA32_REG_R3,
			UA32_REG_RCCTAB, (nlint)(op->Run));

		UA32T_EmitOpReg(jctx, UA32_NMID_BL, UA32_REG_R3);
	}

	BJX2_JitSyncRegs(jctx);

	UA32T_EmitOpMemRegRegDisp(jctx, UA32_NMID_LDR,
		UA32_REG_R0, UA32_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt));

//	UA32T_TryEmitOpImm(jctx, UA32_NMID_POP, 0x4FF0);
	UA32T_EmitOpImm(jctx, UA32_NMID_POP, 0x8FF0);
//	UA32T_TryEmitOpRegReg(jctx, UA32_NMID_MOV, UA32_REG_PC, UA32_REG_LR);

	UA32T_EmitOpImm(jctx, UA32_NMID_BKPT, 0);

	UA32_EmitBAlign(jctx, 4);

	UA32_EmitLabel(jctx, lbl_ctab);
	for(i=0; i<jctx->n_ctab; i++)
		{ UA32_EmitDWord(jctx, jctx->ctab[i]); }
	
	sz=UA32_EmitGetOffs(jctx);
	
	ptr=UA32_LinkContext(jctx);
	UA32_FreeContext(jctx);
	
	if(!ptr && (sz>0))
	{
		printf("BJX2_TryJitTrace: ExHeap Full\n");
//		cpu->jit_needflush=1;
		JX2_DBGBREAK
		return(-1);
	}
	
	if(!ptr)
		return(0);
//	UA32_DumpMemHex(ptr, ptr+sz);
//	UA32T_ThumbDisasm(ptr, ptr+sz);
	tr->Run=(void *)(ptr+1);
//	BJX2_JTraceAddTrace(cpu, tr);	
	
	if(!(tr->Run))
		{ JX2_DBGBREAK }
	
	return(1);
}

int BJX2_CheckJitTrace(BJX2_Context *ctx,
	BJX2_Trace *tr)
{
	if(!ctx->use_jit)
		return(0);

	BJX2_TryJitTrace(ctx, tr);
	return(0);
}
