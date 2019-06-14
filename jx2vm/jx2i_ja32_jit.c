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

 */

int BJX2_TryJitTrace(BJX2_Context *cpu, BJX2_Trace *tr)
{
	UA32_Context *jctx;

	jctx=UA32_AllocContext();

	jctx->reg_resv=0;
	jctx->reg_live=0;
	jctx->reg_save=0;
	jctx->jitfl=0;
	jctx->isjq=0;

	UA32_TryEmitOpReg(jctx, UA32_NMID_PUSH, UA32_REG_LR);

	UA32_TryEmitOpReg(jctx, UA32_NMID_POP, UA32_REG_R11);
	UA32_TryEmitOpReg(jctx, UA32_NMID_POP, UA32_REG_R10);
	UA32_TryEmitOpReg(jctx, UA32_NMID_POP, UA32_REG_R8);
	UA32_TryEmitOpReg(jctx, UA32_NMID_POP, UA32_REG_R7);
	UA32_TryEmitOpReg(jctx, UA32_NMID_POP, UA32_REG_R6);
	UA32_TryEmitOpReg(jctx, UA32_NMID_POP, UA32_REG_R5);
	UA32_TryEmitOpReg(jctx, UA32_NMID_POP, UA32_REG_R4);

	UA32_TryEmitOpRegReg(jctx, UA32_NMID_MOV, UA32_REG_R10, UA32_REG_R0);
	UA32_TryEmitOpRegReg(jctx, UA32_NMID_MOV, UA32_REG_R11, UA32_REG_R1);
	
	for(i=0; i<tr->n_ops; i++)
	{
		op=tr->ops[i];
		UA32_TryEmitOpRegReg(jctx, UA32_NMID_MOV, UA32_REG_R0, UA32_REG_R10);
		UA32_EmitLoadImm32(jctx, UA32_REG_R1, (nlint)(op));

		UA32_EmitLoadImm32(jctx, UA32_REG_R3, (nlint)(op->Run));
		UA32_TryEmitOpReg(jctx, UA32_NMID_BL, UA32_REG_R3);
	}


//	if(tr->csfl&BJX2_CSFL_SRJQ)
//		jctx->isjq=1;

	UA32_TryEmitOpReg(jctx, UA32_NMID_POP, UA32_REG_R4);
	UA32_TryEmitOpReg(jctx, UA32_NMID_POP, UA32_REG_R5);
	UA32_TryEmitOpReg(jctx, UA32_NMID_POP, UA32_REG_R6);
	UA32_TryEmitOpReg(jctx, UA32_NMID_POP, UA32_REG_R7);
	UA32_TryEmitOpReg(jctx, UA32_NMID_POP, UA32_REG_R8);
	UA32_TryEmitOpReg(jctx, UA32_NMID_POP, UA32_REG_R10);
	UA32_TryEmitOpReg(jctx, UA32_NMID_POP, UA32_REG_R11);

	UA32_TryEmitOpReg(jctx, UA32_NMID_POP, UA32_REG_LR);
	UA32_TryEmitOpRegReg(jctx, UA32_NMID_MOV, UA32_REG_PC, UA32_REG_LR);
	
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
//	UAX_DumpMemHex(ptr, ptr+sz);
	tr->Run=(void *)ptr;
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
