#define BJX2_UAXJFL_PREJMP		1	//jump was handled early
#define BJX2_UAXJFL_NOPREJMP		2	//disable pre-jump

BJX2_Opcode *BJX2_TryJitOpcode_CmpOp_GetBrOpA(UAX_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *tr, BJX2_Opcode *op)
{
	BJX2_Opcode *op1, *op2;
	
	if(tr->n_ops<3)
		return(NULL);
	
	op1=tr->ops[tr->n_ops-1];
	if(((op1->nmid==BJX2_NMID_BT) ||
		(op1->nmid==BJX2_NMID_BF)) &&
		(op1->fmid==BJX2_FMID_PCDISP))
			{ return(op1); }
//	op2=tr->ops[tr->n_ops-2];
//	if(((op2->nmid==BJX2_NMID_BTS) ||
//		(op2->nmid==BJX2_NMID_BFS)) &&
//		(op2->fmid==BJX2_FMID_ABS))
//			{ return(op2); }
	return(NULL);
}

int BJX2_TryJitOpcode_CmpOp_OpThrashSrT(UAX_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *tr, BJX2_Opcode *op)
{
	int rt;
	
//	return(1);
	
	switch(op->nmid)
	{
		case BJX2_NMID_BT:			case BJX2_NMID_BF:
//		case BJX2_NMID_BTS:			case BJX2_NMID_BFS:
		case BJX2_NMID_CLRT:		case BJX2_NMID_SETT:
		case BJX2_NMID_CLRS:		case BJX2_NMID_SETS:
		case BJX2_NMID_CMPHS:		case BJX2_NMID_CMPHI:
		case BJX2_NMID_CMPEQ:		case BJX2_NMID_CMPGE:
		case BJX2_NMID_CMPGT:		case BJX2_NMID_TST:
		case BJX2_NMID_CMPPL:		case BJX2_NMID_CMPPZ:

		case BJX2_NMID_CMPQHS:		case BJX2_NMID_CMPQHI:
		case BJX2_NMID_CMPQEQ:		case BJX2_NMID_CMPQGE:
		case BJX2_NMID_CMPQGT:		case BJX2_NMID_TSTQ:
//		case BJX2_NMID_CMPQPL:		case BJX2_NMID_CMPQPZ:

//		case BJX2_NMID_CMPSTR:
		case BJX2_NMID_MOVT:
//		case BJX2_NMID_SHAL:
		case BJX2_NMID_SHAR:
		case BJX2_NMID_SHLL:		case BJX2_NMID_SHLR:
		case BJX2_NMID_ROTL:		case BJX2_NMID_ROTCL:
		case BJX2_NMID_ROTR:		case BJX2_NMID_ROTCR:
//		case BJX2_NMID_DT:			case BJX2_NMID_DIV1:
//		case BJX2_NMID_DIV0S:		case BJX2_NMID_DIV0U:
//		case BJX2_NMID_ADDC:		case BJX2_NMID_ADDV:
//		case BJX2_NMID_SUBC:		case BJX2_NMID_SUBV:
		case BJX2_NMID_ADC:			case BJX2_NMID_SBB:
//		case BJX2_NMID_NEGC:		case BJX2_NMID_TASB:
//		case BJX2_NMID_LDC:			case BJX2_NMID_LDCL:
		case BJX2_NMID_FCMPEQ:		case BJX2_NMID_FCMPGT:
//		case BJX2_NMID_TSTB:
			rt=1; break;
		default:
			rt=0; break;
	}
	return(rt);
}

BJX2_Opcode *BJX2_TryJitOpcode_CmpOp_GetBrOpB(UAX_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *tr, BJX2_Opcode *op)
{
	BJX2_Opcode *op1, *op2;
	int i, j, k;

	if(jctx->jitfl&BJX2_UAXJFL_NOPREJMP)
		return(NULL);

	op1=BJX2_TryJitOpcode_CmpOp_GetBrOpA(jctx, cpu, tr, op);
	if(!op1)
		return(NULL);
	
	i=tr->n_ops-1;
	while((i>0) && (tr->ops[i]!=op1))i--;
	i--;
	while((i>0) && (tr->ops[i]!=op))
	{
		op2=tr->ops[i];
		if(BJX2_TryJitOpcode_CmpOp_OpThrashSrT(jctx, cpu, tr, op2))
			return(NULL);
		i--;
	}
	
	if(i<=0)
		return(NULL);
	
	return(op1);
}

int BJX2_TryJitOpcode_CmpOp_OpRR(UAX_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *tr, BJX2_Opcode *op,
	int setcc)
{
	BJX2_Opcode *op1;
	int jcc, jd;
	int l0, l1, l2;
	int r0, r1, r2;
	int r4, r5, r6;

	r0=BJX2_JitLoadReadSyncVMReg(jctx, BJX2_REG_SR);
	r1=BJX2_JitLoadReadSyncVMReg(jctx, op->rn);
	r2=BJX2_JitLoadReadSyncVMReg(jctx, op->rm);

#if 1
	op1=BJX2_TryJitOpcode_CmpOp_GetBrOpB(jctx, cpu, tr, op);
	jcc=-1;

//	op1=NULL;

	if(op1 &&
//		((op1->nmid==BJX2_NMID_BT) ||
//		 (op1->nmid==BJX2_NMID_BTS)))
		(op1->nmid==BJX2_NMID_BT))
	{
		jd=1;
		switch(setcc)
		{
			case UAX_OP_SETE: jcc=UAX_OP_JE; break;
			case UAX_OP_SETNE: jcc=UAX_OP_JNE; break;
			case UAX_OP_SETG: jcc=UAX_OP_JG; break;
			case UAX_OP_SETL: jcc=UAX_OP_JL; break;
			case UAX_OP_SETGE: jcc=UAX_OP_JGE; break;
			case UAX_OP_SETLE: jcc=UAX_OP_JLE; break;
			case UAX_OP_SETA: jcc=UAX_OP_JA; break;
			case UAX_OP_SETB: jcc=UAX_OP_JB; break;
			case UAX_OP_SETAE: jcc=UAX_OP_JAE; break;
			case UAX_OP_SETBE: jcc=UAX_OP_JBE; break;
			default: jcc=-1; break;
		}
	}

	if(op1 &&
//		((op1->nmid==BJX2_NMID_BF) ||
//		 (op1->nmid==BJX2_NMID_BFS)))
		(op1->nmid==BJX2_NMID_BF))
	{
		jd=0;
		switch(setcc)
		{
			case UAX_OP_SETE: jcc=UAX_OP_JNE; break;
			case UAX_OP_SETNE: jcc=UAX_OP_JE; break;
			case UAX_OP_SETG: jcc=UAX_OP_JLE; break;
			case UAX_OP_SETL: jcc=UAX_OP_JGE; break;
			case UAX_OP_SETGE: jcc=UAX_OP_JL; break;
			case UAX_OP_SETLE: jcc=UAX_OP_JG; break;
			case UAX_OP_SETA: jcc=UAX_OP_JBE; break;
			case UAX_OP_SETB: jcc=UAX_OP_JAE; break;
			case UAX_OP_SETAE: jcc=UAX_OP_JB; break;
			case UAX_OP_SETBE: jcc=UAX_OP_JA; break;
			default: jcc=-1; break;
		}
	}

	if(!(tr->jmpnext))
		jcc=-1;

	if(op1 && (jcc>0))
	{
		r1=UAX_Asm_RegAsDWord(r1);
		r2=UAX_Asm_RegAsDWord(r2);

		r4=r0;
		r5=r1;
		r6=r2;
		
		jctx->jitfl|=BJX2_UAXJFL_PREJMP;

		if(r4==UAX_REG_Z)	r4=UAX_REG_RDX;
		if(r5==UAX_REG_Z)	r5=UAX_REG_EAX;
		if(r6==UAX_REG_Z)	r6=UAX_REG_ECX;
	
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);

		if(r4!=r0)
			{ BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, r4); }

		if((r6==r2) && (r5==r1))
			{ UAX_AsmInsnRegReg(jctx, UAX_OP_CMP, r1, r2); }
		else if(r6==r2)
			{ BJX2_JitCmpVMReg(jctx, op->rn, r2); }
		else if(r5==r1)
			{ BJX2_JitInsnLdRegVMReg(jctx, UAX_OP_CMP, r1, op->rm); }
		else
		{
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
			BJX2_JitCmpVMReg(jctx, op->rn, UAX_REG_ECX);
		}

		UAX_AsmInsnLabel(jctx, jcc, l0|UAX_LBL_NEAR);

		if(jd==1)
			{ UAX_AsmAndRegImm(jctx, r4, (s32)(~1)); }
		else
			{ UAX_AsmOrRegImm(jctx, r4, 1); }

		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l1|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l0);		

		if(jd==1)
			{ UAX_AsmOrRegImm(jctx, r4, 1); }
		else
			{ UAX_AsmAndRegImm(jctx, r4, (s32)(~1)); }

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_R8Q, UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rjmp));
		BJX2_JitStoreVMRegImm(jctx, BJX2_REG_PC, tr->addr_jmp);
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt), UAX_REG_R8Q);

		UAX_EmitLabel(jctx, l1);
		if(r4!=r0)
			{ BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, r4); }
		return(1);
	}
#endif

//	return(0);

	r1=UAX_Asm_RegAsDWord(r1);
	r2=UAX_Asm_RegAsDWord(r2);

	if(r0!=UAX_REG_Z)
	{
		if(r1!=UAX_REG_Z)
		{
			if(r2!=UAX_REG_Z)
			{
				UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
				UAX_AsmAndRegImm(jctx, r0, (s32)(~1));
				UAX_AsmInsnRegReg(jctx, UAX_OP_CMP, r1, r2);
				UAX_AsmInsnReg(jctx, setcc, UAX_REG_AL);
				UAX_AsmOrRegReg(jctx, r0, UAX_REG_RAX);
				return(1);
			}

			UAX_AsmXorRegReg(jctx, UAX_REG_ECX, UAX_REG_ECX);
			UAX_AsmAndRegImm(jctx, r0, (s32)(~1));
			BJX2_JitInsnLdRegVMReg(jctx, UAX_OP_CMP, r1, op->rm);
			UAX_AsmInsnReg(jctx, setcc, UAX_REG_CL);
			UAX_AsmOrRegReg(jctx, r0, UAX_REG_RCX);
			return(1);
		}

		UAX_AsmXorRegReg(jctx, UAX_REG_EDX, UAX_REG_EDX);
		UAX_AsmAndRegImm(jctx, r0, (s32)(~1));
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
		BJX2_JitCmpVMReg(jctx, op->rn, UAX_REG_ECX);
		UAX_AsmInsnReg(jctx, setcc, UAX_REG_DL);
		UAX_AsmOrRegReg(jctx, r0, UAX_REG_RDX);
		return(1);
	}else
	{
		if(r1!=UAX_REG_Z)
		{
			if(r2!=UAX_REG_Z)
			{
				BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
				UAX_AsmXorRegReg(jctx, UAX_REG_ECX, UAX_REG_ECX);
				UAX_AsmAndRegImm(jctx, UAX_REG_RDX, (s32)(~1));
				UAX_AsmInsnRegReg(jctx, UAX_OP_CMP, r1, r2);
				UAX_AsmInsnReg(jctx, setcc, UAX_REG_CL);
				UAX_AsmOrRegReg(jctx, UAX_REG_RDX, UAX_REG_RCX);
				BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
				return(1);
			}

			BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
			UAX_AsmXorRegReg(jctx, UAX_REG_ECX, UAX_REG_ECX);
			UAX_AsmAndRegImm(jctx, UAX_REG_RDX, (s32)(~1));
			BJX2_JitInsnLdRegVMReg(jctx, UAX_OP_CMP, r1, op->rm);
			UAX_AsmInsnReg(jctx, setcc, UAX_REG_CL);
			UAX_AsmOrRegReg(jctx, UAX_REG_RDX, UAX_REG_RCX);
			BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
			return(1);
		}
	}

//	return(0);

	BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
	UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
	UAX_AsmAndRegImm(jctx, UAX_REG_RDX, (s32)(~1));
	BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
	BJX2_JitCmpVMReg(jctx, op->rn, UAX_REG_ECX);
	UAX_AsmInsnReg(jctx, setcc, UAX_REG_AL);
	UAX_AsmOrRegReg(jctx, UAX_REG_RDX, UAX_REG_RAX);
	BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
	return(1);
}

int BJX2_TryJitOpcode_CmpOp_OpRI(UAX_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *tr, BJX2_Opcode *op,
	int setcc, int idx, s64 imm)
{
	BJX2_Opcode *op1;
	int jcc, jd;
	int l0, l1, l2;
	int r0, r1, r2;
	int r4, r5, r6;

//	return(0);

	r0=BJX2_JitLoadReadSyncVMReg(jctx, BJX2_REG_SR);
	r1=BJX2_JitLoadReadSyncVMReg(jctx, idx);

	r1=UAX_Asm_RegAsDWord(r1);

#if 1
	op1=BJX2_TryJitOpcode_CmpOp_GetBrOpB(jctx, cpu, tr, op);
	jcc=-1;

//	op1=NULL;

	if(op1 &&
		(op1->nmid==BJX2_NMID_BT))
	{
		jd=1;
		switch(setcc)
		{
			case UAX_OP_SETE: jcc=UAX_OP_JE; break;
			case UAX_OP_SETNE: jcc=UAX_OP_JNE; break;
			case UAX_OP_SETG: jcc=UAX_OP_JG; break;
			case UAX_OP_SETL: jcc=UAX_OP_JL; break;
			case UAX_OP_SETGE: jcc=UAX_OP_JGE; break;
			case UAX_OP_SETLE: jcc=UAX_OP_JLE; break;
			case UAX_OP_SETA: jcc=UAX_OP_JA; break;
			case UAX_OP_SETB: jcc=UAX_OP_JB; break;
			case UAX_OP_SETAE: jcc=UAX_OP_JAE; break;
			case UAX_OP_SETBE: jcc=UAX_OP_JBE; break;
			default: jcc=-1; break;
		}
	}

	if(op1 && (op1->nmid==BJX2_NMID_BF))
	{
		jd=0;
		switch(setcc)
		{
			case UAX_OP_SETE: jcc=UAX_OP_JNE; break;
			case UAX_OP_SETNE: jcc=UAX_OP_JE; break;
			case UAX_OP_SETG: jcc=UAX_OP_JLE; break;
			case UAX_OP_SETL: jcc=UAX_OP_JGE; break;
			case UAX_OP_SETGE: jcc=UAX_OP_JL; break;
			case UAX_OP_SETLE: jcc=UAX_OP_JG; break;
			case UAX_OP_SETA: jcc=UAX_OP_JBE; break;
			case UAX_OP_SETB: jcc=UAX_OP_JAE; break;
			case UAX_OP_SETAE: jcc=UAX_OP_JB; break;
			case UAX_OP_SETBE: jcc=UAX_OP_JA; break;
			default: jcc=-1; break;
		}
	}

	if(!(tr->jmpnext))
		jcc=-1;

//	return(0);

	if(op1 && (jcc>0))
	{
		r4=r0;
		r5=r1;
		
		jctx->jitfl|=BJX2_UAXJFL_PREJMP;

		if(r4==UAX_REG_Z)	r4=UAX_REG_EDX;
		if(r5==UAX_REG_Z)	r5=UAX_REG_ECX;
	
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);

		if(r4!=r0)
			{ BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, r4); }

		if(r5==r1)
			{ UAX_AsmInsnRegImm(jctx, UAX_OP_CMP, r1, (s32)op->imm); }
		else
		{
			BJX2_JitLoadVMReg(jctx, idx, r5);
			UAX_AsmInsnRegImm(jctx, UAX_OP_CMP, r5, (s32)op->imm);
//			BJX2_JitInsnVMRegImm(jctx, UAX_OP_CMP, idx, (s32)op->imm);
		}

		UAX_AsmInsnLabel(jctx, jcc, l0|UAX_LBL_NEAR);

		if(jd==1)
			{ UAX_AsmAndRegImm(jctx, r4, (s32)(~1)); }
		else
			{ UAX_AsmOrRegImm(jctx, r4, 1); }

		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l1|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l0);		

		if(jd==1)
			{ UAX_AsmOrRegImm(jctx, r4, 1); }
		else
			{ UAX_AsmAndRegImm(jctx, r4, (s32)(~1)); }

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_R8Q, UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rjmp));
		BJX2_JitStoreVMRegImm(jctx, BJX2_REG_PC, tr->addr_jmp);
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt), UAX_REG_R8Q);

		UAX_EmitLabel(jctx, l1);
		if(r4!=r0)
			{ BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, r4); }
		return(1);
	}
#endif

//	return(0);

	if(r0!=UAX_REG_Z)
	{
		if(r1!=UAX_REG_Z)
		{
			UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
			UAX_AsmAndRegImm(jctx, r0, (s32)(~1));
			UAX_AsmCmpRegImm(jctx, r1, (s32)imm);
			UAX_AsmInsnReg(jctx, setcc, UAX_REG_AL);
			UAX_AsmOrRegReg(jctx, r0, UAX_REG_RAX);
			return(1);
		}

		UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
		UAX_AsmAndRegImm(jctx, r0, (s32)(~1));
		BJX2_JitLoadVMReg(jctx, idx, UAX_REG_ECX);
		UAX_AsmCmpRegImm(jctx, UAX_REG_ECX, (s32)imm);
		UAX_AsmInsnReg(jctx, setcc, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, r0, UAX_REG_RAX);
		return(1);
	}

//	return(0);

	if(r1!=UAX_REG_Z)
	{
		BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_RDX, (s32)(~1));
		UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
		UAX_AsmCmpRegImm(jctx, r1, (s32)imm);
		UAX_AsmInsnReg(jctx, setcc, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, UAX_REG_RDX, UAX_REG_RAX);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
		return(1);
	}

//	return(0);

	BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
	UAX_AsmAndRegImm(jctx, UAX_REG_RDX, (s32)(~1));
	BJX2_JitLoadVMReg(jctx, idx, UAX_REG_ECX);
	UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
	UAX_AsmCmpRegImm(jctx, UAX_REG_ECX, (s32)imm);
	UAX_AsmInsnReg(jctx, setcc, UAX_REG_AL);
	UAX_AsmOrRegReg(jctx, UAX_REG_RDX, UAX_REG_RAX);
	BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
	return(1);
}


#if 1
int BJX2_TryJitOpcode_CmpOpQ_OpRR(UAX_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *tr, BJX2_Opcode *op,
	int setcc)
{
	BJX2_Opcode *op1;
	int jcc, jd;
	int l0, l1, l2;
	int r0, r1, r2;
	int r4, r5, r6;

	r0=BJX2_JitLoadReadSyncVMReg(jctx, BJX2_REG_SR);
	r1=BJX2_JitLoadReadSyncVMReg(jctx, op->rn);
	r2=BJX2_JitLoadReadSyncVMReg(jctx, op->rm);

#if 1
	op1=BJX2_TryJitOpcode_CmpOp_GetBrOpB(jctx, cpu, tr, op);
	jcc=-1;

//	op1=NULL;

	if(op1 && (op1->nmid==BJX2_NMID_BT))
	{
		jd=1;
		switch(setcc)
		{
			case UAX_OP_SETE: jcc=UAX_OP_JE; break;
			case UAX_OP_SETNE: jcc=UAX_OP_JNE; break;
			case UAX_OP_SETG: jcc=UAX_OP_JG; break;
			case UAX_OP_SETL: jcc=UAX_OP_JL; break;
			case UAX_OP_SETGE: jcc=UAX_OP_JGE; break;
			case UAX_OP_SETLE: jcc=UAX_OP_JLE; break;
			case UAX_OP_SETA: jcc=UAX_OP_JA; break;
			case UAX_OP_SETB: jcc=UAX_OP_JB; break;
			case UAX_OP_SETAE: jcc=UAX_OP_JAE; break;
			case UAX_OP_SETBE: jcc=UAX_OP_JBE; break;
			default: jcc=-1; break;
		}
	}

	if(op1 && (op1->nmid==BJX2_NMID_BF))
	{
		jd=0;
		switch(setcc)
		{
			case UAX_OP_SETE: jcc=UAX_OP_JNE; break;
			case UAX_OP_SETNE: jcc=UAX_OP_JE; break;
			case UAX_OP_SETG: jcc=UAX_OP_JLE; break;
			case UAX_OP_SETL: jcc=UAX_OP_JGE; break;
			case UAX_OP_SETGE: jcc=UAX_OP_JL; break;
			case UAX_OP_SETLE: jcc=UAX_OP_JG; break;
			case UAX_OP_SETA: jcc=UAX_OP_JBE; break;
			case UAX_OP_SETB: jcc=UAX_OP_JAE; break;
			case UAX_OP_SETAE: jcc=UAX_OP_JB; break;
			case UAX_OP_SETBE: jcc=UAX_OP_JA; break;
			default: jcc=-1; break;
		}
	}

	if(!(tr->jmpnext))
		jcc=-1;

	if(op1 && (jcc>0))
	{
		r4=r0;
		r5=r1;
		r6=r2;
		
		jctx->jitfl|=BJX2_UAXJFL_PREJMP;

		if(r4==UAX_REG_Z)	r4=UAX_REG_RDX;
		if(r5==UAX_REG_Z)	r5=UAX_REG_RAX;
		if(r6==UAX_REG_Z)	r6=UAX_REG_RCX;
	
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);

		if(r4!=r0)
			{ BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, r4); }

		if((r6==r2) && (r5==r1))
			{ UAX_AsmInsnRegReg(jctx, UAX_OP_CMP, r1, r2); }
		else if(r6==r2)
			{ BJX2_JitCmpVMReg(jctx, op->rn, r2); }
		else if(r5==r1)
			{ BJX2_JitInsnLdRegVMReg(jctx, UAX_OP_CMP, r1, op->rm); }
		else
		{
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			BJX2_JitCmpVMReg(jctx, op->rn, UAX_REG_RCX);
		}

		UAX_AsmInsnLabel(jctx, jcc, l0|UAX_LBL_NEAR);

		if(jd==1)
			{ UAX_AsmAndRegImm(jctx, r4, (s32)(~1)); }
		else
			{ UAX_AsmOrRegImm(jctx, r4, 1); }

		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l1|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l0);		

		if(jd==1)
			{ UAX_AsmOrRegImm(jctx, r4, 1); }
		else
			{ UAX_AsmAndRegImm(jctx, r4, (s32)(~1)); }

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_R8Q, UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rjmp));
		BJX2_JitStoreVMRegImm(jctx, BJX2_REG_PC, tr->addr_jmp);
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt), UAX_REG_R8Q);

		UAX_EmitLabel(jctx, l1);
		if(r4!=r0)
			{ BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, r4); }
		return(1);
	}
#endif

//	return(0);

#if 1
	if(r0!=UAX_REG_Z)
//	if(0)
	{
		if(r1!=UAX_REG_Z)
		{
			if(r2!=UAX_REG_Z)
			{
				UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
				UAX_AsmAndRegImm(jctx, r0, (s32)(~1));
				UAX_AsmInsnRegReg(jctx, UAX_OP_CMP, r1, r2);
				UAX_AsmInsnReg(jctx, setcc, UAX_REG_AL);
				UAX_AsmOrRegReg(jctx, r0, UAX_REG_RAX);
				return(1);
			}

			UAX_AsmXorRegReg(jctx, UAX_REG_ECX, UAX_REG_ECX);
			UAX_AsmAndRegImm(jctx, r0, (s32)(~1));
			BJX2_JitInsnLdRegVMReg(jctx, UAX_OP_CMP, r1, op->rm);
			UAX_AsmInsnReg(jctx, setcc, UAX_REG_CL);
			UAX_AsmOrRegReg(jctx, r0, UAX_REG_RCX);
			return(1);
		}

		UAX_AsmXorRegReg(jctx, UAX_REG_EDX, UAX_REG_EDX);
		UAX_AsmAndRegImm(jctx, r0, (s32)(~1));
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
		BJX2_JitCmpVMReg(jctx, op->rn, UAX_REG_RCX);
		UAX_AsmInsnReg(jctx, setcc, UAX_REG_DL);
		UAX_AsmOrRegReg(jctx, r0, UAX_REG_RDX);
		return(1);
	}else
	{
		if(r1!=UAX_REG_Z)
		{
			if(r2!=UAX_REG_Z)
//			if(0)
			{
				BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
				UAX_AsmXorRegReg(jctx, UAX_REG_ECX, UAX_REG_ECX);
				UAX_AsmAndRegImm(jctx, UAX_REG_RDX, (s32)(~1));
				UAX_AsmInsnRegReg(jctx, UAX_OP_CMP, r1, r2);
				UAX_AsmInsnReg(jctx, setcc, UAX_REG_CL);
				UAX_AsmOrRegReg(jctx, UAX_REG_RDX, UAX_REG_RCX);
				BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
				return(1);
			}

			BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
			UAX_AsmXorRegReg(jctx, UAX_REG_ECX, UAX_REG_ECX);
			UAX_AsmAndRegImm(jctx, UAX_REG_RDX, (s32)(~1));
			BJX2_JitInsnLdRegVMReg(jctx, UAX_OP_CMP, r1, op->rm);
			UAX_AsmInsnReg(jctx, setcc, UAX_REG_CL);
			UAX_AsmOrRegReg(jctx, UAX_REG_RDX, UAX_REG_RCX);
			BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
			return(1);
		}
	}
#endif

//	return(0);

	BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
	UAX_AsmAndRegImm(jctx, UAX_REG_RDX, (s32)(~1));
	BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_R8Q);
	BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_R9Q);
//	BJX2_JitCmpVMReg(jctx, op->rn, UAX_REG_RCX);
	UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
	UAX_AsmInsnRegReg(jctx, UAX_OP_CMP, UAX_REG_R8Q, UAX_REG_R9Q);

	UAX_AsmInsnReg(jctx, setcc, UAX_REG_AL);
	UAX_AsmOrRegReg(jctx, UAX_REG_RDX, UAX_REG_RAX);
	BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
	return(1);
}
#endif


#if 1
int BJX2_TryJitOpcode_TestOp_OpRR(UAX_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *tr, BJX2_Opcode *op)
{
	BJX2_Opcode *op1;
	int isq;
	int jcc, jd;
	int l0, l1, l2;
	int r0, r1, r2;
	int r4, r5, r6;

	r0=BJX2_JitLoadReadSyncVMReg(jctx, BJX2_REG_SR);
	r1=BJX2_JitLoadReadSyncVMReg(jctx, op->rn);
	r2=BJX2_JitLoadReadSyncVMReg(jctx, op->rm);

	isq=0;
	if(op->nmid==BJX2_NMID_TSTQ)
		isq=1;

	if(!isq)
	{
		r1=UAX_Asm_RegAsDWord(r1);
		r2=UAX_Asm_RegAsDWord(r2);
	}

#if 1
	op1=BJX2_TryJitOpcode_CmpOp_GetBrOpB(jctx, cpu, tr, op);
	jcc=-1;

//	op1=NULL;

	if(op1 && (op1->nmid==BJX2_NMID_BT))
			{ jd=1; jcc=UAX_OP_JE; }
	if(op1 && (op1->nmid==BJX2_NMID_BF))
			{ jd=0; jcc=UAX_OP_JNE; }

	if(!(tr->jmpnext))
		jcc=-1;

	if(op1 && (jcc>0))
	{
		r4=r0;
		r5=r1;
		r6=r2;
		
		jctx->jitfl|=BJX2_UAXJFL_PREJMP;

		if(r4==UAX_REG_Z)	r4=UAX_REG_RDX;

		if(!isq)
		{
			if(r5==UAX_REG_Z)	r5=UAX_REG_EAX;
			if(r6==UAX_REG_Z)	r6=UAX_REG_ECX;
		}else
		{
			if(r5==UAX_REG_Z)	r5=UAX_REG_RAX;
			if(r6==UAX_REG_Z)	r6=UAX_REG_RCX;
		}
	
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);

		if(r4!=r0)
			{ BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, r4); }

		if((r6==r2) && (r5==r1))
			{ UAX_AsmInsnRegReg(jctx, UAX_OP_TEST, r1, r2); }
		else if(r6==r2)
			{ BJX2_JitTestVMReg(jctx, op->rn, r2); }
		else if(r5==r1)
		{
//			BJX2_JitInsnRegVMReg(jctx, UAX_OP_TEST, r1, op->rm);
			BJX2_JitInsnVMRegReg(jctx, UAX_OP_TEST, op->rm, r1);
		}
		else
		{
			if(!isq)
			{
				BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
				BJX2_JitTestVMReg(jctx, op->rn, UAX_REG_ECX);
			}else
			{
				BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
				BJX2_JitTestVMReg(jctx, op->rn, UAX_REG_RCX);
			}
		}

		UAX_AsmInsnLabel(jctx, jcc, l0|UAX_LBL_NEAR);

		if(jd==1)
			{ UAX_AsmAndRegImm(jctx, r4, (s32)(~1)); }
		else
			{ UAX_AsmOrRegImm(jctx, r4, 1); }

		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l1|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l0);		

		if(jd==1)
			{ UAX_AsmOrRegImm(jctx, r4, 1); }
		else
			{ UAX_AsmAndRegImm(jctx, r4, (s32)(~1)); }

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_R8Q, UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rjmp));
		BJX2_JitStoreVMRegImm(jctx, BJX2_REG_PC, tr->addr_jmp);
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt), UAX_REG_R8Q);

		UAX_EmitLabel(jctx, l1);
		if(r4!=r0)
			{ BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, r4); }
		return(1);
	}
#endif

//	return(0);

	if(r0!=UAX_REG_Z)
	{
		if(r1!=UAX_REG_Z)
		{
			if(r2!=UAX_REG_Z)
			{
				UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
				UAX_AsmAndRegImm(jctx, r0, (s32)(~1));
				UAX_AsmInsnRegReg(jctx, UAX_OP_TEST, r1, r2);
				UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
				UAX_AsmOrRegReg(jctx, r0, UAX_REG_RAX);
				return(1);
			}

			UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
			UAX_AsmAndRegImm(jctx, r0, (s32)(~1));
//			BJX2_JitInsnRegVMReg(jctx, UAX_OP_TEST, r1, op->rm);
			BJX2_JitInsnVMRegReg(jctx, UAX_OP_TEST, op->rm, r1);
			UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
			UAX_AsmOrRegReg(jctx, r0, UAX_REG_RAX);
			return(1);
		}

		UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
		UAX_AsmAndRegImm(jctx, r0, (s32)(~1));
		if(!isq)
		{
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
			BJX2_JitTestVMReg(jctx, op->rn, UAX_REG_ECX);
		}else
		{
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			BJX2_JitTestVMReg(jctx, op->rn, UAX_REG_RCX);
		}
		UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, r0, UAX_REG_RAX);
		return(1);
	}else
	{
		if(r1!=UAX_REG_Z)
		{
			if(r2!=UAX_REG_Z)
			{
				BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
				UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
				UAX_AsmAndRegImm(jctx, UAX_REG_RDX, (s32)(~1));
				UAX_AsmInsnRegReg(jctx, UAX_OP_TEST, r1, r2);
				UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
				UAX_AsmOrRegReg(jctx, UAX_REG_RDX, UAX_REG_RAX);
				BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
				return(1);
			}

			BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
			UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
			UAX_AsmAndRegImm(jctx, UAX_REG_RDX, (s32)(~1));
//			BJX2_JitInsnRegVMReg(jctx, UAX_OP_TEST, r1, op->rm);
			BJX2_JitInsnVMRegReg(jctx, UAX_OP_TEST, op->rm, r1);
			UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
			UAX_AsmOrRegReg(jctx, UAX_REG_RDX, UAX_REG_RAX);
			BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
			return(1);
		}
	}

	BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
	UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
	UAX_AsmAndRegImm(jctx, UAX_REG_RDX, (s32)(~1));
	if(!isq)
	{
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
		BJX2_JitTestVMReg(jctx, op->rn, UAX_REG_ECX);
	}else
	{
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
		BJX2_JitTestVMReg(jctx, op->rn, UAX_REG_RCX);
	}
	UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
	UAX_AsmOrRegReg(jctx, UAX_REG_RDX, UAX_REG_RAX);
	BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
	return(1);
}

int BJX2_TryJitOpcode_TestOp_OpRI(UAX_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *tr, BJX2_Opcode *op,
	int idx, s64 imm)
{
	BJX2_Opcode *op1;
	int jcc, jd;
	int l0, l1, l2;
	int r0, r1, r2;
	int r4, r5, r6;

	r0=BJX2_JitLoadReadSyncVMReg(jctx, BJX2_REG_SR);
	r1=BJX2_JitLoadReadSyncVMReg(jctx, idx);

	r1=UAX_Asm_RegAsDWord(r1);

#if 1
	op1=BJX2_TryJitOpcode_CmpOp_GetBrOpB(jctx, cpu, tr, op);
	jcc=-1;

//	op1=NULL;

	if(op1 && (op1->nmid==BJX2_NMID_BT))
			{ jd=1; jcc=UAX_OP_JE; }
	if(op1 && (op1->nmid==BJX2_NMID_BF))
			{ jd=0; jcc=UAX_OP_JNE; }

	if(!(tr->jmpnext))
		jcc=-1;

	if(op1 && (jcc>0))
	{
		r4=r0;
		r5=r1;
		
		jctx->jitfl|=BJX2_UAXJFL_PREJMP;

		if(r4==UAX_REG_Z)	r4=UAX_REG_RDX;
		if(r5==UAX_REG_Z)	r5=UAX_REG_ECX;
	
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);

		if(r4!=r0)
			{ BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, r4); }

		if(r5==r1)
			{ UAX_AsmInsnRegImm(jctx, UAX_OP_TEST, r1, (s32)op->imm); }
		else
			{ BJX2_JitInsnVMRegImm(jctx, UAX_OP_TEST, idx, (s32)op->imm); }

		UAX_AsmInsnLabel(jctx, jcc, l0|UAX_LBL_NEAR);

		if(jd==1)
			{ UAX_AsmAndRegImm(jctx, r4, (s32)(~1)); }
		else
			{ UAX_AsmOrRegImm(jctx, r4, 1); }

		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l1|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l0);		

		if(jd==1)
			{ UAX_AsmOrRegImm(jctx, r4, 1); }
		else
			{ UAX_AsmAndRegImm(jctx, r4, (s32)(~1)); }

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_R8Q, UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rjmp));
		BJX2_JitStoreVMRegImm(jctx, BJX2_REG_PC, tr->addr_jmp);
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt), UAX_REG_R8Q);

		UAX_EmitLabel(jctx, l1);
		if(r4!=r0)
			{ BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, r4); }
		return(1);
	}
#endif

//	return(0);

#if 1
	if(r0!=UAX_REG_Z)
	{
		if(r1!=UAX_REG_Z)
		{
			UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
			UAX_AsmAndRegImm(jctx, r0, (s32)(~1));
			UAX_AsmTestRegImm(jctx, r1, (s32)imm);
			UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
			UAX_AsmOrRegReg(jctx, r0, UAX_REG_RAX);
			return(1);
		}

		UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
		UAX_AsmAndRegImm(jctx, r0, (s32)(~1));
		BJX2_JitLoadVMReg(jctx, idx, UAX_REG_ECX);
		UAX_AsmTestRegImm(jctx, UAX_REG_ECX, (s32)imm);
		UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, r0, UAX_REG_RAX);
		return(1);
	}
#endif

//	return(0);

#if 1
	if(r1!=UAX_REG_Z)
	{
		BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
		UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
		UAX_AsmAndRegImm(jctx, UAX_REG_RDX, (s32)(~1));
		UAX_AsmTestRegImm(jctx, r1, (s32)imm);
		UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, UAX_REG_RDX, UAX_REG_RAX);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
		return(1);
	}
#endif

//	return(0);

#if 1
	BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
	UAX_AsmAndRegImm(jctx, UAX_REG_RDX, (s32)(~1));
	UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
	BJX2_JitLoadVMReg(jctx, idx, UAX_REG_ECX);
	UAX_AsmTestRegImm(jctx, UAX_REG_ECX, (s32)imm);
	UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
	UAX_AsmOrRegReg(jctx, UAX_REG_RDX, UAX_REG_RAX);
	BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
	return(1);
#endif
}
#endif


int BJX2_TryJitOpcode_CmpOp(UAX_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *tr, BJX2_Opcode *op)
{
	int r0, r1, r2;
	int l0, l1, l2;

//	return(0);

#if 1
	if((op->nmid==BJX2_NMID_CMPEQ) &&
		(op->fmid==BJX2_FMID_IMMREG))
	{
		return(BJX2_TryJitOpcode_CmpOp_OpRI(jctx, cpu, tr, op,
			UAX_OP_SETE, op->rn, (s32)op->imm));
//		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BJX2_NMID_CMPEQ) &&
		(op->fmid==BJX2_FMID_REGREG))
	{
		return(BJX2_TryJitOpcode_CmpOp_OpRR(
			jctx, cpu, tr, op, UAX_OP_SETE));
//		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BJX2_NMID_CMPGE) &&
		(op->fmid==BJX2_FMID_IMMREG))
	{
		return(BJX2_TryJitOpcode_CmpOp_OpRI(jctx, cpu, tr, op,
			UAX_OP_SETGE, op->rn, (s32)op->imm));
//		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_CMPGE) &&
		(op->fmid==BJX2_FMID_REGREG))
	{
		return(BJX2_TryJitOpcode_CmpOp_OpRR(
			jctx, cpu, tr, op, UAX_OP_SETGE));
//		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_CMPGT) &&
		(op->fmid==BJX2_FMID_IMMREG))
	{
		return(BJX2_TryJitOpcode_CmpOp_OpRI(jctx, cpu, tr, op,
			UAX_OP_SETG, op->rn, (s32)op->imm));
//		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_CMPGT) &&
		(op->fmid==BJX2_FMID_REGREG))
	{
		return(BJX2_TryJitOpcode_CmpOp_OpRR(
			jctx, cpu, tr, op, UAX_OP_SETG));
//		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BJX2_NMID_CMPHS) &&
		(op->fmid==BJX2_FMID_IMMREG))
	{
		return(BJX2_TryJitOpcode_CmpOp_OpRI(jctx, cpu, tr, op,
			UAX_OP_SETAE, op->rn, (s32)op->imm));
//		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_CMPHS) &&
		(op->fmid==BJX2_FMID_REGREG))
	{
		return(BJX2_TryJitOpcode_CmpOp_OpRR(
			jctx, cpu, tr, op, UAX_OP_SETAE));
//		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BJX2_NMID_CMPHI) &&
		(op->fmid==BJX2_FMID_IMMREG))
	{
		return(BJX2_TryJitOpcode_CmpOp_OpRI(jctx, cpu, tr, op,
			UAX_OP_SETA, op->rn, (s32)op->imm));
//		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_CMPHI) &&
		(op->fmid==BJX2_FMID_REGREG))
	{
		return(BJX2_TryJitOpcode_CmpOp_OpRR(
			jctx, cpu, tr, op, UAX_OP_SETA));
//		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BJX2_NMID_CMPQEQ) &&
		(op->fmid==BJX2_FMID_REGREG))
	{
		return(BJX2_TryJitOpcode_CmpOpQ_OpRR(
			jctx, cpu, tr, op, UAX_OP_SETE));
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_CMPQGT) &&
		(op->fmid==BJX2_FMID_REGREG))
	{
		return(BJX2_TryJitOpcode_CmpOpQ_OpRR(
			jctx, cpu, tr, op, UAX_OP_SETG));
	}

	if((op->nmid==BJX2_NMID_CMPQHI) &&
		(op->fmid==BJX2_FMID_REGREG))
	{
		return(BJX2_TryJitOpcode_CmpOpQ_OpRR(
			jctx, cpu, tr, op, UAX_OP_SETA));
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BJX2_NMID_TST) && (op->fmid==BJX2_FMID_REGREG))
	{
		return(BJX2_TryJitOpcode_TestOp_OpRR(
			jctx, cpu, tr, op));
//		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_TSTQ) && (op->fmid==BJX2_FMID_REGREG))
	{
		return(BJX2_TryJitOpcode_TestOp_OpRR(
			jctx, cpu, tr, op));
//		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BJX2_NMID_TST) && (op->fmid==BJX2_FMID_IMMREG))
	{
		return(BJX2_TryJitOpcode_TestOp_OpRI(jctx, cpu, tr, op,
			op->rn, (s32)op->imm));
//		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BJX2_NMID_CMPPL) &&
		(op->fmid==BJX2_FMID_REG))
	{
		return(BJX2_TryJitOpcode_CmpOp_OpRI(jctx, cpu, tr, op,
			UAX_OP_SETG, op->rn, 0));
//		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_CMPPZ) &&
		(op->fmid==BJX2_FMID_REG))
	{
		return(BJX2_TryJitOpcode_CmpOp_OpRI(jctx, cpu, tr, op,
			UAX_OP_SETGE, op->rn, 0));
//		return(1);
	}
#endif

//	return(0);

#if 0
	if((op->nmid==BJX2_NMID_DT) &&
		(op->fmid==BJX2_FMID_REGRN))
	{
		BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_RDX, (s32)(~1));
		BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_ECX);
		UAX_AsmInsnReg(jctx, UAX_OP_DEC, UAX_REG_ECX);
		UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, UAX_REG_DL, UAX_REG_AL);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_ECX);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
		return(1);
	}
#endif

#if 0
	if((op->nmid==BJX2_NMID_FCMPEQ) && (op->fmid==BJX2_FMID_FREGREG))
	{
		BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_EDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_EDX, (s32)(~1));
		BJX2_JitLoadVMDReg(jctx, op->rm, UAX_REG_RCX);
		BJX2_JitLoadVMDReg(jctx, op->rn, UAX_REG_RAX);
//		BJX2_JitTestDVMReg(jctx, op->rn, UAX_REG_RAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_CMP, UAX_REG_RCX, UAX_REG_RAX);
		UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, UAX_REG_DL, UAX_REG_AL);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, UAX_REG_EDX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_FCMPEQ) && (op->fmid==BJX2_FMID_FREGREG))
	{
		BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_RDX, (s32)(~1));
		BJX2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM1);
		BJX2_JitLoadVMDReg(jctx, op->rn, UAX_REG_XMM0);
		UAX_AsmInsnRegReg(jctx, UAX_OP_COMISD, UAX_REG_XMM0, UAX_REG_XMM1);
		UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, UAX_REG_DL, UAX_REG_AL);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_FCMPGT) && (op->fmid==BJX2_FMID_FREGREG))
	{
		BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_RDX, (s32)(~1));
		BJX2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM1);
		BJX2_JitLoadVMDReg(jctx, op->rn, UAX_REG_XMM0);
		UAX_AsmInsnRegReg(jctx, UAX_OP_COMISD, UAX_REG_XMM0, UAX_REG_XMM1);
		UAX_AsmInsnReg(jctx, UAX_OP_SETA, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, UAX_REG_DL, UAX_REG_AL);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
		return(1);
	}
#endif

	return(0);
}



int BJX2_TryJitOpcode_BranchOp(UAX_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *tr, BJX2_Opcode *op)
{
	BJX2_Trace *trj;
	int l0, l1, l2;
	int nm1, nm2, nm3;

#if 1
	if((op->nmid==BJX2_NMID_BF) &&
		(op->fmid==BJX2_FMID_PCDISP))
	{
		if(jctx->jitfl&BJX2_UAXJFL_PREJMP)
		{
//			__debugbreak();
			return(1);
		}
	
		if(tr->jmpnext)
//		if(0)
		{
			l0=UAX_GenLabelTemp(jctx);
			BJX2_JitTestVMRegImm(jctx, BJX2_REG_SR, 1);
			UAX_AsmInsnLabel(jctx, UAX_OP_JNE, l0|UAX_LBL_NEAR);

			UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
				UAX_REG_R8Q, UAX_REG_RCCTX,
				offsetof(BJX2_Context, tr_rjmp));

			BJX2_JitStoreVMRegImm(jctx, BJX2_REG_PC, (op->pc2)+(op->imm*2));
			UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
				UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt), UAX_REG_R8Q);
			UAX_EmitLabel(jctx, l0);
			return(1);
		}else
		{
			l0=UAX_GenLabelTemp(jctx);
			BJX2_JitTestVMRegImm(jctx, BJX2_REG_SR, 1);
			UAX_AsmInsnLabel(jctx, UAX_OP_JNE, l0|UAX_LBL_NEAR);
			BJX2_JitStoreVMRegImm(jctx, BJX2_REG_PC, (op->pc2)+(op->imm*2));
			BJX2_JitFlushJNext(jctx, cpu, tr);
			UAX_EmitLabel(jctx, l0);
			return(1);
		}

		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_BT) &&
		(op->fmid==BJX2_FMID_PCDISP))
	{
		if(jctx->jitfl&BJX2_UAXJFL_PREJMP)
		{
//			__debugbreak();
			return(1);
		}

		if(tr->jmpnext)
//		if(0)
		{
			l0=UAX_GenLabelTemp(jctx);
		
//			UAX_AsmInsnStRegDispImm(jctx, UAX_OP_TEST,
//				UAX_REG_RCCTX, offsetof(BJX2_Context, regs)+(BJX2_REG_SR*4),
//				1);
			BJX2_JitTestVMRegImm(jctx, BJX2_REG_SR, 1);
			UAX_AsmInsnLabel(jctx, UAX_OP_JE, l0|UAX_LBL_NEAR);

			UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
				UAX_REG_R8Q, UAX_REG_RCCTX,
				offsetof(BJX2_Context, tr_rjmp));

			BJX2_JitStoreVMRegImm(jctx, BJX2_REG_PC, (op->pc2)+(op->imm*2));
			UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
				UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt), UAX_REG_R8Q);
			UAX_EmitLabel(jctx, l0);
			return(1);
		}else
		{
			l0=UAX_GenLabelTemp(jctx);
//			UAX_AsmInsnStRegDispImm(jctx, UAX_OP_TEST,
//				UAX_REG_RCCTX, offsetof(BJX2_Context, regs)+(BJX2_REG_SR*4),
//				1);
			BJX2_JitTestVMRegImm(jctx, BJX2_REG_SR, 1);
			UAX_AsmInsnLabel(jctx, UAX_OP_JE, l0|UAX_LBL_NEAR);
			BJX2_JitStoreVMRegImm(jctx, BJX2_REG_PC, (op->pc2)+(op->imm*2));
			BJX2_JitFlushJNext(jctx, cpu, tr);
			UAX_EmitLabel(jctx, l0);
			return(1);
		}

		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_BRA) &&
		(op->fmid==BJX2_FMID_PCDISP))
	{
		if(((op->pc2)+(op->imm*2))==tr->addr_nxt)
			return(1);
	
		BJX2_JitStoreVMRegImm(jctx, BJX2_REG_PC, (op->pc2)+(op->imm*2));
		BJX2_JitFlushJNext(jctx, cpu, tr);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_BSR) &&
		(op->fmid==BJX2_FMID_PCDISP))
	{
//		if(op->imm!=tr->jmppc)
//			{ __debugbreak(); }

#if 1
		if(tr->lnknext)
		{
			l2=((tr->addr_nxt*65521)>>16)&63;
			UAX_AsmMovRegImm(jctx, UAX_REG_R8Q, (nlint)(tr->lnknext));
			UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
				UAX_REG_RCCTX, offsetof(BJX2_Context, rttr)+(l2*8),
				UAX_REG_R8Q);
			UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
				UAX_REG_RCCTX, offsetof(BJX2_Context, prttr),
				UAX_REG_R8Q);
		}
#endif

		BJX2_JitStoreVMRegImm(jctx, BJX2_REG_LR, tr->addr_nxt);
		BJX2_JitStoreVMRegImm(jctx, BJX2_REG_PC, (op->pc2)+(op->imm*2));

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_R8Q, UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rjmp));
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt), UAX_REG_R8Q);

		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_RTS) &&
//		(op->fmid==BJX2_FMID_NONE))
		(op->fmid==BJX2_FMID_Z))
	{
		BJX2_JitLoadVMReg(jctx, BJX2_REG_LR, UAX_REG_RAX);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_PC, UAX_REG_RAX);

//		l0=UAX_GenLabelTemp(jctx);
//		l1=UAX_GenLabelTemp(jctx);

#if 1
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_R8Q, UAX_REG_RCCTX, offsetof(BJX2_Context, prttr));
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_R9Q, UAX_REG_R8Q, offsetof(BJX2_Trace, addr));

		UAX_AsmInsnRegReg(jctx, UAX_OP_XOR, UAX_REG_RCX, UAX_REG_RCX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_CMP, UAX_REG_RAX, UAX_REG_R9Q);
		UAX_AsmInsnRegReg(jctx, UAX_OP_CMOVNE, UAX_REG_R8Q, UAX_REG_RCX);

		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt), UAX_REG_R8Q);
		return(1);
#endif

#if 0
//		UAX_AsmInsnRegReg(jctx, UAX_OP_XOR, UAX_REG_RAX, UAX_REG_RAX);
//		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
//			UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt), UAX_REG_RAX);
		BJX2_JitFlushJNext(jctx, cpu, tr);
		return(1);
#endif
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_RET) &&
//		(op->fmid==BJX2_FMID_NONE))
		(op->fmid==BJX2_FMID_Z))
	{
		BJX2_JitLoadVMReg(jctx, BJX2_REG_SP, UAX_REG_RDX);
		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDX);
		UAX_AsmAddRegImm(jctx, UAX_REG_RCX, 8);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_SP, UAX_REG_RCX);
		BJX2_JitGetAddrQWord(jctx, cpu);
//		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RAX);

//		BJX2_JitLoadVMReg(jctx, BJX2_REG_LR, UAX_REG_RAX);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_PC, UAX_REG_RAX);

#if 1
		l0=UAX_GenLabelTemp(jctx);
//		l1=UAX_GenLabelTemp(jctx);

		UAX_AsmMovRegReg(jctx, UAX_REG_ECX, UAX_REG_EAX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_IMUL, UAX_REG_ECX, 65521);
		UAX_AsmInsnRegImm(jctx, UAX_OP_SHR, UAX_REG_ECX, 16);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 63);

		UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_MOV,
			UAX_REG_R8Q,
			UAX_REG_RCCTX, 8, UAX_REG_RCX,
				offsetof(BJX2_Context, rttr));
//		if(l2<=0)
//			{ __debugbreak(); }

		UAX_AsmInsnRegReg(jctx, UAX_OP_TEST, UAX_REG_R8Q, UAX_REG_R8Q);
		UAX_AsmInsnLabel(jctx, UAX_OP_JE, l0|UAX_LBL_NEAR);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_R9Q, UAX_REG_R8Q, offsetof(BJX2_Trace, addr));

		UAX_AsmInsnRegReg(jctx, UAX_OP_XOR, UAX_REG_RCX, UAX_REG_RCX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_CMP, UAX_REG_RAX, UAX_REG_R9Q);
		UAX_AsmInsnRegReg(jctx, UAX_OP_CMOVNE, UAX_REG_R8Q, UAX_REG_RCX);

		UAX_EmitLabel(jctx, l0);
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt), UAX_REG_R8Q);
		return(1);
#endif

		UAX_AsmInsnRegReg(jctx, UAX_OP_XOR, UAX_REG_RAX, UAX_REG_RAX);
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt), UAX_REG_RAX);
		BJX2_JitFlushJNext(jctx, cpu, tr);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_BSR) &&
		(op->fmid==BJX2_FMID_REG))
	{
#if 1
		if(tr->lnknext)
		{
			l2=((tr->addr_nxt*65521)>>16)&63;
			UAX_AsmMovRegImm(jctx, UAX_REG_R8Q, (nlint)(tr->lnknext));
			UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
				UAX_REG_RCCTX, offsetof(BJX2_Context, rttr)+(l2*8),
				UAX_REG_R8Q);
			UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
				UAX_REG_RCCTX, offsetof(BJX2_Context, prttr),
				UAX_REG_R8Q);
		}
#endif

		BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RAX);
		BJX2_JitLoadVMReg(jctx, BJX2_REG_PC, UAX_REG_RCX);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_PC, UAX_REG_RAX);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_LR, UAX_REG_RCX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_XOR, UAX_REG_RAX, UAX_REG_RAX);
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt), UAX_REG_RAX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_BRA) &&
		(op->fmid==BJX2_FMID_REG))
	{
		BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RAX);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_PC, UAX_REG_RAX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_XOR, UAX_REG_RAX, UAX_REG_RAX);
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt), UAX_REG_RAX);
		return(1);
	}
#endif

#if 0
	if((op->nmid==BJX2_NMID_BSRF) &&
		(op->fmid==BJX2_FMID_REGRM))
	{
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_RDX, op->pc2);
		BJX2_JitLoadVMReg(jctx, BJX2_REG_PC, UAX_REG_RCX);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_PC, UAX_REG_RDX);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_PR, UAX_REG_RCX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_XOR, UAX_REG_RDX, UAX_REG_RDX);
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt), UAX_REG_RDX);
		return(1);
	}
#endif

#if 0
	if((op->nmid==BJX2_NMID_BRAF) &&
		(op->fmid==BJX2_FMID_REGRM))
	{
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_RCX, op->pc2);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_PC, UAX_REG_RCX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_XOR, UAX_REG_RCX, UAX_REG_RCX);
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt), UAX_REG_RCX);
		return(1);
	}
#endif


#if 0
	if((	(op->nmid==BJX2_NMID_BREQ) ||
			(op->nmid==BJX2_NMID_BRNE) ||
			(op->nmid==BJX2_NMID_BRGT) ||
			(op->nmid==BJX2_NMID_BRLT) ||
			(op->nmid==BJX2_NMID_BRGE) ||
			(op->nmid==BJX2_NMID_BRLE)) &&
		(op->fmid==BJX2_FMID_REG1ABS))
	{
		if(jctx->jitfl&BJX2_UAXJFL_PREJMP)
		{
			return(1);
		}
		
		if(op->nmid==BJX2_NMID_BREQ)
			nm1=UAX_OP_JNE;
		if(op->nmid==BJX2_NMID_BRNE)
			nm1=UAX_OP_JE;
		if(op->nmid==BJX2_NMID_BRGE)
			nm1=UAX_OP_JL;
		if(op->nmid==BJX2_NMID_BRLE)
			nm1=UAX_OP_JG;
		if(op->nmid==BJX2_NMID_BRGT)
			nm1=UAX_OP_JLE;
		if(op->nmid==BJX2_NMID_BRLT)
			nm1=UAX_OP_JGE;
	
		if(tr->jmpnext)
		{
			l0=UAX_GenLabelTemp(jctx);
			BJX2_JitCmpVMRegImm(jctx, op->rn, 0);
			UAX_AsmInsnLabel(jctx, nm1, l0|UAX_LBL_NEAR);
			UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
				UAX_REG_R8Q, UAX_REG_RCCTX,
				offsetof(BJX2_Context, tr_rjmp));
			BJX2_JitStoreVMRegImm(jctx, BJX2_REG_PC, (op->pc2)+(op->imm*2));
			UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
				UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt), UAX_REG_R8Q);
			UAX_EmitLabel(jctx, l0);
			return(1);
		}else
		{
			l0=UAX_GenLabelTemp(jctx);
			BJX2_JitCmpVMRegImm(jctx, op->rn, 0);
			UAX_AsmInsnLabel(jctx, nm1, l0|UAX_LBL_NEAR);
			BJX2_JitStoreVMRegImm(jctx, BJX2_REG_PC, (op->pc2)+(op->imm*2));
			BJX2_JitFlushJNext(jctx, cpu, tr);
			UAX_EmitLabel(jctx, l0);
			return(1);
		}

		return(1);
	}
#endif

#if 0
	if((	(op->nmid==BJX2_NMID_BREQ) ||
			(op->nmid==BJX2_NMID_BRNE) ||
			(op->nmid==BJX2_NMID_BRGT) ||
			(op->nmid==BJX2_NMID_BRLT) ||
			(op->nmid==BJX2_NMID_BRGE) ||
			(op->nmid==BJX2_NMID_BRLE)) &&
		(op->fmid==BJX2_FMID_REG2ABS))
	{
		if(jctx->jitfl&BJX2_UAXJFL_PREJMP)
		{
			return(1);
		}
		
		if(op->nmid==BJX2_NMID_BREQ)
			nm1=UAX_OP_JNE;
		if(op->nmid==BJX2_NMID_BRNE)
			nm1=UAX_OP_JE;
		if(op->nmid==BJX2_NMID_BRGE)
			nm1=UAX_OP_JL;
		if(op->nmid==BJX2_NMID_BRLE)
			nm1=UAX_OP_JG;
		if(op->nmid==BJX2_NMID_BRGT)
			nm1=UAX_OP_JLE;
		if(op->nmid==BJX2_NMID_BRLT)
			nm1=UAX_OP_JGE;
	
		if(tr->jmpnext)
		{
			l0=UAX_GenLabelTemp(jctx);

			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
			BJX2_JitCmpVMReg(jctx, op->rn, UAX_REG_EAX);

			UAX_AsmInsnLabel(jctx, nm1, l0|UAX_LBL_NEAR);
			UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
				UAX_REG_R8Q, UAX_REG_RCCTX,
				offsetof(BJX2_Context, tr_rjmp));
			BJX2_JitStoreVMRegImm(jctx, BJX2_REG_PC, (op->pc2)+(op->imm*2));
			UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
				UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt), UAX_REG_R8Q);
			UAX_EmitLabel(jctx, l0);
			return(1);
		}else
		{
			l0=UAX_GenLabelTemp(jctx);

			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
			BJX2_JitCmpVMReg(jctx, op->rn, UAX_REG_EAX);

//			BJX2_JitCmpVMRegImm(jctx, op->rn, 0);
			UAX_AsmInsnLabel(jctx, nm1, l0|UAX_LBL_NEAR);
			BJX2_JitStoreVMRegImm(jctx, BJX2_REG_PC, (op->pc2)+(op->imm*2));
			BJX2_JitFlushJNext(jctx, cpu, tr);
			UAX_EmitLabel(jctx, l0);
			return(1);
		}

		return(1);
	}
#endif

	return(0);
}
