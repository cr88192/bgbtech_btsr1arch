int BJX2_TryJitOpcode_MovMem(UAX_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *tr, BJX2_Opcode *op)
{
	u32 v;
	int i;

#if 1
	if(op->nmid==BJX2_NMID_MOVL)
	{
		if(op->fmid==BJX2_FMID_LDREGREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BJX2_JitGetAddrDWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

#if 1
		if(op->fmid==BJX2_FMID_REGSTREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BJX2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}
#endif

#if 1
		if(op->fmid==BJX2_FMID_LDREGDISPREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm*4);
			BJX2_JitGetAddrDWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BJX2_FMID_REGSTREGDISP)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm*4);
			BJX2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}
#endif

#if 1
//		if((op->fmid==BJX2_FMID_LDREG2REG) &&
		if(((op->fmid==BJX2_FMID_LDREG2REG) ||
			(op->fmid==BJX2_FMID_LDDRREGREG)) &&
			(op->rm!=BJX2_REG_PC))
//			(op->rm!=BJX2_REG_PC) &&
//			(op->rm!=BJX2_REG_GBR))
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
//				UAX_REG_RDX, UAX_REG_RDX, 4, UAX_REG_RCX, 0);
				UAX_REG_RDX, UAX_REG_RDX,
				(op->rm==BJX2_REG_GBR)?1:4, UAX_REG_RCX, 0);
			BJX2_JitGetAddrDWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

//		if((op->fmid==BJX2_FMID_REGSTREG2) &&
		if(((op->fmid==BJX2_FMID_REGSTREG2) ||
			(op->fmid==BJX2_FMID_REGSTDRREG)) &&
			(op->rn!=BJX2_REG_PC))
//			(op->rn!=BJX2_REG_PC) &&
//			(op->rn!=BJX2_REG_GBR))
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
//				UAX_REG_RDX, UAX_REG_RDX, 4, UAX_REG_RCX, 0);
				UAX_REG_RDX, UAX_REG_RDX,
				(op->rn==BJX2_REG_GBR)?1:4, UAX_REG_RCX, 0);
			BJX2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}
#endif

#if 1
		if(op->fmid==BJX2_FMID_LDPCDISPREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			UAX_AsmMovRegImm(jctx, UAX_REG_RDX, (op->pc2)+(op->imm));
			BJX2_JitGetAddrDWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BJX2_FMID_REGSTPCDISP)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_R8D);
			UAX_AsmMovRegImm(jctx, UAX_REG_RDX, (op->pc2)+(op->imm));
			BJX2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}
#endif

#if 1
		if(op->fmid==BJX2_FMID_LDDRPCREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, 0, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->pc2);
			BJX2_JitGetAddrDWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BJX2_FMID_REGSTDRPC)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_R8D);
			BJX2_JitLoadVMReg(jctx, 0, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->pc2);
			BJX2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}
#endif

#if 0
		if(op->fmid==BJX2_FMID_REGLDABS)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			UAX_AsmMovRegImm(jctx, UAX_REG_RDX, (s32)(op->imm));
			BJX2_JitGetAddrDWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}
#endif

		return(0);
	}
#endif

#if 1
	if(op->nmid==BJX2_NMID_MOVQ)
	{
		if(op->fmid==BJX2_FMID_LDREGREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BJX2_JitGetAddrQWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_RCX, UAX_REG_RAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BJX2_FMID_REGSTREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8Q);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BJX2_JitSetAddrQWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BJX2_FMID_LDREGDISPREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm*8);
			BJX2_JitGetAddrQWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_RCX, UAX_REG_RAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

#if 1
		if(op->fmid==BJX2_FMID_REGSTREGDISP)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8Q);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm*8);
			BJX2_JitSetAddrQWord(jctx, cpu);
			return(1);
		}
#endif

#if 1
//		if((op->fmid==BJX2_FMID_LDREG2REG) &&
		if(((op->fmid==BJX2_FMID_LDREG2REG) ||
			(op->fmid==BJX2_FMID_LDDRREGREG)) &&
			(op->rm!=BJX2_REG_PC))
//			(op->rm!=BJX2_REG_PC) &&
//			(op->rm!=BJX2_REG_GBR))
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
//				UAX_REG_RDX, UAX_REG_RDX, 8, UAX_REG_RCX, 0);
				UAX_REG_RDX, UAX_REG_RDX,
				(op->rm==BJX2_REG_GBR)?1:8, UAX_REG_RCX, 0);
			BJX2_JitGetAddrQWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_RCX, UAX_REG_RAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

//		if((op->fmid==BJX2_FMID_REGSTREG2) &&
		if(((op->fmid==BJX2_FMID_REGSTREG2) ||
			(op->fmid==BJX2_FMID_REGSTDRREG)) &&
			(op->rn!=BJX2_REG_PC))
//			(op->rn!=BJX2_REG_PC) &&
//			(op->rn!=BJX2_REG_GBR))
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8Q);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
//				UAX_REG_RDX, UAX_REG_RDX, 8, UAX_REG_RCX, 0);
				UAX_REG_RDX, UAX_REG_RDX,
				(op->rn==BJX2_REG_GBR)?1:8, UAX_REG_RCX, 0);
			BJX2_JitSetAddrQWord(jctx, cpu);
			return(1);
		}
#endif

#if 1
		if(op->fmid==BJX2_FMID_LDPCDISPREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			UAX_AsmMovRegImm(jctx, UAX_REG_RDX, (op->pc2)+(op->imm));
			BJX2_JitGetAddrQWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_RCX, UAX_REG_RAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BJX2_FMID_REGSTPCDISP)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_R8Q);
			UAX_AsmMovRegImm(jctx, UAX_REG_RDX, (op->pc2)+(op->imm));
			BJX2_JitSetAddrQWord(jctx, cpu);
			return(1);
		}
#endif

#if 1
		if(op->fmid==BJX2_FMID_LDDRPCREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, 0, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->pc2);
			BJX2_JitGetAddrQWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_RCX, UAX_REG_RAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BJX2_FMID_REGSTDRPC)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_R8Q);
			BJX2_JitLoadVMReg(jctx, 0, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->pc2);
			BJX2_JitSetAddrQWord(jctx, cpu);
			return(1);
		}
#endif
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_PUSH) &&
		(op->fmid==BJX2_FMID_REG))
	{
		BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_R8Q);
		BJX2_JitLoadVMReg(jctx, BJX2_REG_SP, UAX_REG_RDX);
		UAX_AsmSubRegImm(jctx, UAX_REG_RDX, 8);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_SP, UAX_REG_RDX);
		BJX2_JitSetAddrQWord(jctx, cpu);
		return(1);
	}

	if((op->nmid==BJX2_NMID_POP) &&
		(op->fmid==BJX2_FMID_REG))
	{
		BJX2_JitLoadVMReg(jctx, BJX2_REG_SP, UAX_REG_RDX);
		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDX);
		UAX_AsmAddRegImm(jctx, UAX_REG_RCX, 8);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_SP, UAX_REG_RCX);
		BJX2_JitGetAddrQWord(jctx, cpu);
//		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RAX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RAX);
		return(1);
	}
#endif


#if 0
	if((op->nmid==BJX2_NMID_FPUSH) &&
		(op->fmid==BJX2_FMID_FREG))
	{
		BJX2_JitLoadVMDReg(jctx, op->rn, UAX_REG_R8Q);
		BJX2_JitLoadVMReg(jctx, BJX2_REG_SP, UAX_REG_RDX);
		UAX_AsmSubRegImm(jctx, UAX_REG_RDX, 8);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_SP, UAX_REG_RDX);
		BJX2_JitSetAddrQWord(jctx, cpu);
		return(1);
	}

	if((op->nmid==BJX2_NMID_FPOP) &&
		(op->fmid==BJX2_FMID_FREG))
	{
		BJX2_JitLoadVMReg(jctx, BJX2_REG_SP, UAX_REG_RDX);
		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDX);
		UAX_AsmAddRegImm(jctx, UAX_REG_RCX, 8);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_SP, UAX_REG_RCX);
		BJX2_JitGetAddrQWord(jctx, cpu);
		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RAX);
		BJX2_JitStoreVMDReg(jctx, op->rn, UAX_REG_RAX);
		return(1);
	}
#endif

//	return(0);

#if 1
	if(op->nmid==BJX2_NMID_MOVW)
	{
		if(op->fmid==BJX2_FMID_LDREGREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BJX2_JitGetAddrWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BJX2_FMID_REGSTREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BJX2_JitSetAddrWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BJX2_FMID_LDREGDISPREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm*2);
			BJX2_JitGetAddrWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BJX2_FMID_REGSTREGDISP)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm*2);
			BJX2_JitSetAddrWord(jctx, cpu);
			return(1);
		}

#if 1
//		if((op->fmid==BJX2_FMID_LDREG2REG) &&
		if(((op->fmid==BJX2_FMID_LDREG2REG) ||
			(op->fmid==BJX2_FMID_LDDRREGREG)) &&
			(op->rm!=BJX2_REG_PC) &&
			(op->rm!=BJX2_REG_GBR))
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, 2, UAX_REG_RCX, 0);
			BJX2_JitGetAddrWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

//		if((op->fmid==BJX2_FMID_REGSTREG2) &&
		if(((op->fmid==BJX2_FMID_REGSTREG2) ||
			(op->fmid==BJX2_FMID_REGSTDRREG)) &&
			(op->rn!=BJX2_REG_PC) &&
			(op->rn!=BJX2_REG_GBR))
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, 2, UAX_REG_RCX, 0);
			BJX2_JitSetAddrWord(jctx, cpu);
			return(1);
		}
#endif

#if 1
		if(op->fmid==BJX2_FMID_LDPCDISPREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			UAX_AsmMovRegImm(jctx, UAX_REG_RDX, (op->pc2)+(op->imm));
			BJX2_JitGetAddrWord(jctx, cpu);
//			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_EAX, UAX_REG_AX);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BJX2_FMID_REGSTPCDISP)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_R8D);
			UAX_AsmMovRegImm(jctx, UAX_REG_RDX, (op->pc2)+(op->imm));
			BJX2_JitSetAddrWord(jctx, cpu);
			return(1);
		}
#endif

#if 1
		if(op->fmid==BJX2_FMID_LDDRPCREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, 0, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->pc2);
			BJX2_JitGetAddrWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BJX2_FMID_REGSTDRPC)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_R8D);
			BJX2_JitLoadVMReg(jctx, 0, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->pc2);
			BJX2_JitSetAddrWord(jctx, cpu);
			return(1);
		}
#endif

#if 0
		if(op->fmid==BJX2_FMID_REGLDABS)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			UAX_AsmMovRegImm(jctx, UAX_REG_RDX, (s64)(op->imm));
			BJX2_JitGetAddrWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}
#endif

		return(0);
	}
#endif

#if 1
	if(op->nmid==BJX2_NMID_MOVB)
	{
		if(op->fmid==BJX2_FMID_LDREGREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BJX2_JitGetAddrByte(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BJX2_FMID_REGSTREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BJX2_JitSetAddrByte(jctx, cpu);
			return(1);
		}

		if(op->fmid==BJX2_FMID_LDREGDISPREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm);
			BJX2_JitGetAddrByte(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BJX2_FMID_REGSTREGDISP)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm);
			BJX2_JitSetAddrByte(jctx, cpu);
			return(1);
		}

#if 1
//		if((op->fmid==BJX2_FMID_LDREG2REG) &&
		if(((op->fmid==BJX2_FMID_LDREG2REG) ||
			(op->fmid==BJX2_FMID_LDDRREGREG)) &&
			(op->rm!=BJX2_REG_PC))
//			(op->rm!=BJX2_REG_PC) &&
//			(op->rm!=BJX2_REG_GBR))
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, 1, UAX_REG_RCX, 0);
			BJX2_JitGetAddrByte(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

//		if((op->fmid==BJX2_FMID_REGSTREG2) &&
		if(((op->fmid==BJX2_FMID_REGSTREG2) ||
			(op->fmid==BJX2_FMID_REGSTDRREG)) &&
			(op->rn!=BJX2_REG_PC))
//			(op->rn!=BJX2_REG_PC) &&
//			(op->rn!=BJX2_REG_GBR))
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, 1, UAX_REG_RCX, 0);
			BJX2_JitSetAddrByte(jctx, cpu);
			return(1);
		}
#endif

#if 1
		if(op->fmid==BJX2_FMID_LDPCDISPREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			UAX_AsmMovRegImm(jctx, UAX_REG_RDX, (op->pc2)+(op->imm));
			BJX2_JitGetAddrByte(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BJX2_FMID_REGSTPCDISP)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_R8D);
			UAX_AsmMovRegImm(jctx, UAX_REG_RDX, (op->pc2)+(op->imm));
			BJX2_JitSetAddrByte(jctx, cpu);
			return(1);
		}
#endif

#if 1
		if(op->fmid==BJX2_FMID_LDDRPCREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, 0, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->pc2);
			BJX2_JitGetAddrByte(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BJX2_FMID_REGSTDRPC)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_R8D);
			BJX2_JitLoadVMReg(jctx, 0, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->pc2);
			BJX2_JitSetAddrByte(jctx, cpu);
			return(1);
		}
#endif

		return(0);
	}
#endif

//	return(0);

#if 1
	if(op->nmid==BJX2_NMID_MOVUB)
	{
		if(op->fmid==BJX2_FMID_LDREGREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BJX2_JitGetAddrByte(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVZX, UAX_REG_RCX, UAX_REG_AL);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BJX2_FMID_LDREGDISPREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm);
			BJX2_JitGetAddrByte(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVZX, UAX_REG_RCX, UAX_REG_AL);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

#if 1
//		if((op->fmid==BJX2_FMID_LDREG2REG) &&
		if(((op->fmid==BJX2_FMID_LDREG2REG) ||
			(op->fmid==BJX2_FMID_LDDRREGREG)) &&
			(op->rm!=BJX2_REG_PC))
//			(op->rm!=BJX2_REG_PC) &&
//			(op->rm!=BJX2_REG_GBR))
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, 1, UAX_REG_RCX, 0);
			BJX2_JitGetAddrByte(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVZX, UAX_REG_RCX, UAX_REG_AL);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}
#endif

#if 1
		if(op->fmid==BJX2_FMID_LDDRPCREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, 0, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->pc2);
			BJX2_JitGetAddrByte(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVZX, UAX_REG_RCX, UAX_REG_AL);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}
#endif
	}
#endif

#if 1
	if(op->nmid==BJX2_NMID_MOVUW)
	{
		if(op->fmid==BJX2_FMID_LDREGREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BJX2_JitGetAddrWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVZX, UAX_REG_EAX, UAX_REG_AX);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_ECX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BJX2_FMID_LDREGDISPREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm*2);
			BJX2_JitGetAddrWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVZX, UAX_REG_EAX, UAX_REG_AX);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_ECX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

#if 1
//		if((op->fmid==BJX2_FMID_LDREG2REG) &&
		if(((op->fmid==BJX2_FMID_LDREG2REG) ||
			(op->fmid==BJX2_FMID_LDDRREGREG)) &&
			(op->rm!=BJX2_REG_PC) &&
			(op->rm!=BJX2_REG_GBR))
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, 2, UAX_REG_RCX, 0);
			BJX2_JitGetAddrWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVZX, UAX_REG_EAX, UAX_REG_AX);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_ECX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}
#endif

#if 1
		if(op->fmid==BJX2_FMID_LDDRPCREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, 0, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->pc2);
			BJX2_JitGetAddrWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVZX, UAX_REG_EAX, UAX_REG_AX);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_ECX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}
#endif
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BJX2_NMID_MOVUL) ||
		(op->nmid==BJX2_NMID_MOVDL))
	{
		if(op->fmid==BJX2_FMID_LDREGREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BJX2_JitGetAddrDWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_ECX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BJX2_FMID_LDREGDISPREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm*4);
			BJX2_JitGetAddrDWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_ECX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

#if 1
		if((op->fmid==BJX2_FMID_LDREG2REG) &&
//		if(((op->fmid==BJX2_FMID_LDREG2REG) ||
//			(op->fmid==BJX2_FMID_LDDRREGREG)) &&
			(op->rm!=BJX2_REG_PC))
//			(op->rm!=BJX2_REG_PC) &&
//			(op->rm!=BJX2_REG_GBR))
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
//				UAX_REG_RDX, UAX_REG_RDX, 4, UAX_REG_RCX, 0);
				UAX_REG_RDX, UAX_REG_RDX,
				(op->rm==BJX2_REG_GBR)?1:4, UAX_REG_RCX, 0);
			BJX2_JitGetAddrDWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_ECX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}
#endif

#if 1
		if((op->fmid==BJX2_FMID_LDDRREGREG) &&
			(op->rm!=BJX2_REG_PC))
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BJX2_JitLoadVMReg(jctx, BJX2_REG_R0, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
//				UAX_REG_RDX, UAX_REG_RDX, 4, UAX_REG_RCX, 0);
				UAX_REG_RDX, UAX_REG_RDX,
				(op->rm==BJX2_REG_GBR)?1:4, UAX_REG_RCX, 0);
			BJX2_JitGetAddrDWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_ECX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}
#endif

#if 1
		if(op->fmid==BJX2_FMID_LDDRPCREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, 0, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->pc2);
			BJX2_JitGetAddrDWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_ECX, UAX_REG_EAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}
#endif
	}
#endif

//	return(0);

#if 1
	if(op->nmid==BJX2_NMID_LEAB)
	{
		if(op->fmid==BJX2_FMID_LDREGREG)
		{
//			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
			return(1);
		}

		if(op->fmid==BJX2_FMID_LDREGDISPREG)
		{
//			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
			return(1);
		}

#if 1
//		if(((op->fmid==BJX2_FMID_LDREG2REG) ||
//			(op->fmid==BJX2_FMID_LDDRREGREG)) &&
		if((op->fmid==BJX2_FMID_LDREG2REG) &&
			(op->rm!=BJX2_REG_PC))
//			(op->rm!=BJX2_REG_PC) &&
//			(op->rm!=BJX2_REG_GBR))
		{
//			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmAddRegReg(jctx, UAX_REG_RDX, UAX_REG_RCX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
			return(1);
		}
#endif

#if 1
		if(op->fmid==BJX2_FMID_LDDRPCREG)
		{
//			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, 0, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->pc2);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
			return(1);
		}
#endif

#if 1
		if(op->fmid==BJX2_FMID_LDPCDISPREG)
		{
//			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
//			BJX2_JitLoadVMReg(jctx, 0, UAX_REG_RDX);
			UAX_AsmMovRegImm(jctx, UAX_REG_RDX, op->pc2+op->imm);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
			return(1);
		}
#endif
	}
#endif


#if 1
	if((op->nmid==BJX2_NMID_LEAB) ||
		(op->nmid==BJX2_NMID_LEAW) ||
		(op->nmid==BJX2_NMID_LEAD) ||
		(op->nmid==BJX2_NMID_LEAQ))
	{
		if((op->fmid==BJX2_FMID_LDREG2REG) &&
			(op->rm!=BJX2_REG_PC) &&
			(op->rm!=BJX2_REG_GBR))
		{
			i=1;
			if(op->nmid==BJX2_NMID_LEAB)i=1;
			if(op->nmid==BJX2_NMID_LEAW)i=2;
			if(op->nmid==BJX2_NMID_LEAD)i=4;
			if(op->nmid==BJX2_NMID_LEAQ)i=8;
//			if(op->ro==BJX2_REG_R0)i=1;
		
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, i, UAX_REG_RCX, 0);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
			return(1);
		}

#if 1
		if((op->fmid==BJX2_FMID_LDREGDISPREG) &&
			(op->rm!=BJX2_REG_PC) &&
			(op->rm!=BJX2_REG_GBR))
		{
			i=1;
			if(op->nmid==BJX2_NMID_LEAB)i=1;
			if(op->nmid==BJX2_NMID_LEAW)i=2;
			if(op->nmid==BJX2_NMID_LEAD)i=4;
			if(op->nmid==BJX2_NMID_LEAQ)i=8;
//			if(op->ro==BJX2_REG_R0)i=1;
		
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, (s64)(op->imm)*i);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
			return(1);
		}
#endif
	}
#endif
	

#if 0
	if(op->nmid==BJX2_NMID_FMOVS)
	{
		if(op->fmid==BJX2_FMID_LDREGREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BJX2_JitGetAddrDWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVQ, UAX_REG_XMM0, UAX_REG_RAX);
			UAX_AsmInsnRegReg(jctx, UAX_OP_CVTSS2SD, UAX_REG_XMM1, UAX_REG_XMM0);
			BJX2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM1);
			return(1);
		}

#if 1
		if(op->fmid==BJX2_FMID_REGSTREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM0);
			UAX_AsmInsnRegReg(jctx, UAX_OP_CVTSD2SS, UAX_REG_XMM1, UAX_REG_XMM0);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVQ, UAX_REG_R8Q, UAX_REG_XMM1);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BJX2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}
#endif

#if 1
		if(op->fmid==BJX2_FMID_LDREGDISPREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm*4);
			BJX2_JitGetAddrDWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVQ, UAX_REG_XMM0, UAX_REG_RAX);
			UAX_AsmInsnRegReg(jctx, UAX_OP_CVTSS2SD, UAX_REG_XMM1, UAX_REG_XMM0);
			BJX2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM1);
			return(1);
		}

		if(op->fmid==BJX2_FMID_REGSTREGDISP)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM0);
			UAX_AsmInsnRegReg(jctx, UAX_OP_CVTSD2SS, UAX_REG_XMM1, UAX_REG_XMM0);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVQ, UAX_REG_R8Q, UAX_REG_XMM1);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm*4);
			BJX2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}
#endif

#if 1
//		if((op->fmid==BJX2_FMID_LDREG2REG) &&
		if(((op->fmid==BJX2_FMID_LDREG2REG) ||
			(op->fmid==BJX2_FMID_LDDRREGREG)) &&
			(op->rm!=BJX2_REG_PC) &&
			(op->rm!=BJX2_REG_GBR))
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, 4, UAX_REG_RCX, 0);
			BJX2_JitGetAddrDWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVQ, UAX_REG_XMM0, UAX_REG_RAX);
			UAX_AsmInsnRegReg(jctx, UAX_OP_CVTSS2SD, UAX_REG_XMM1, UAX_REG_XMM0);
			BJX2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM1);
			return(1);
		}

//		if((op->fmid==BJX2_FMID_REGSTREG2) &&
		if(((op->fmid==BJX2_FMID_REGSTREG2) ||
			(op->fmid==BJX2_FMID_REGSTDRREG)) &&
			(op->rn!=BJX2_REG_PC) &&
			(op->rn!=BJX2_REG_GBR))
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM0);
			UAX_AsmInsnRegReg(jctx, UAX_OP_CVTSD2SS, UAX_REG_XMM1, UAX_REG_XMM0);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVQ, UAX_REG_R8Q, UAX_REG_XMM1);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, 4, UAX_REG_RCX, 0);
			BJX2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}
#endif

		if(((op->fmid==BJX2_FMID_LDREG2REG) ||
			(op->fmid==BJX2_FMID_LDDRREGREG)) &&
			(op->rm==BJX2_REG_PC))
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->pc2);
			BJX2_JitGetAddrDWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVQ, UAX_REG_XMM0, UAX_REG_RAX);
			UAX_AsmInsnRegReg(jctx, UAX_OP_CVTSS2SD,
				UAX_REG_XMM1, UAX_REG_XMM0);
			BJX2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM1);
			return(1);
		}
	}
#endif


#if 0
	if(op->nmid==BJX2_NMID_FMOVD)
	{
		if(op->fmid==BJX2_FMID_LDREGREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BJX2_JitGetAddrQWord(jctx, cpu);
//			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVQ, UAX_REG_XMM0, UAX_REG_RAX);
//			BJX2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM0);
			BJX2_JitStoreVMDReg(jctx, op->rn, UAX_REG_RAX);
			return(1);
		}

#if 1
		if(op->fmid==BJX2_FMID_REGSTREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMDReg(jctx, op->rm, UAX_REG_R8Q);
//			BJX2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM0);
//			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVQ, UAX_REG_R8Q, UAX_REG_XMM0);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BJX2_JitSetAddrQWord(jctx, cpu);
			return(1);
		}
#endif

#if 1
		if(op->fmid==BJX2_FMID_LDREGDISPREG)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm*8);
			BJX2_JitGetAddrQWord(jctx, cpu);
//			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVQ, UAX_REG_XMM0, UAX_REG_RAX);
//			BJX2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM0);
			BJX2_JitStoreVMDReg(jctx, op->rn, UAX_REG_RAX);
			return(1);
		}

		if(op->fmid==BJX2_FMID_REGSTREGDISP)
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMDReg(jctx, op->rm, UAX_REG_R8Q);
//			BJX2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM0);
//			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVQ, UAX_REG_R8Q, UAX_REG_XMM0);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm*8);
			BJX2_JitSetAddrQWord(jctx, cpu);
			return(1);
		}
#endif

#if 1
//		if((op->fmid==BJX2_FMID_LDREG2REG) &&
		if(((op->fmid==BJX2_FMID_LDREG2REG) ||
			(op->fmid==BJX2_FMID_LDDRREGREG)) &&
			(op->rm!=BJX2_REG_PC) &&
			(op->rm!=BJX2_REG_GBR))
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, 8, UAX_REG_RCX, 0);
			BJX2_JitGetAddrQWord(jctx, cpu);
//			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVQ, UAX_REG_XMM0, UAX_REG_RAX);
//			BJX2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM0);
			BJX2_JitStoreVMDReg(jctx, op->rn, UAX_REG_RAX);
			return(1);
		}

//		if((op->fmid==BJX2_FMID_REGSTREG2) &&
		if(((op->fmid==BJX2_FMID_REGSTREG2) ||
			(op->fmid==BJX2_FMID_REGSTDRREG)) &&
			(op->rn!=BJX2_REG_PC))
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMDReg(jctx, op->rm, UAX_REG_R8Q);
//			BJX2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM0);
//			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVQ, UAX_REG_R8Q, UAX_REG_XMM0);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, 8, UAX_REG_RCX, 0);
			BJX2_JitSetAddrQWord(jctx, cpu);
			return(1);
		}
#endif

		if(((op->fmid==BJX2_FMID_LDREG2REG) ||
			(op->fmid==BJX2_FMID_LDDRREGREG)) &&
			(op->rm==BJX2_REG_PC))
		{
			BJX2_JitSetupOpTrap(jctx, cpu, tr, op);
			BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->pc2);
			BJX2_JitGetAddrQWord(jctx, cpu);
			BJX2_JitStoreVMDReg(jctx, op->rn, UAX_REG_RAX);
			return(1);
		}
	}
#endif


	return(0);
}

int BJX2_TryJitOpcode_PredX(UAX_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *tr, BJX2_Opcode *op)
{
	BJX2_Opcode *op1;
	u32 v;
	int l0;
	int i;

	op1=op->data;

	if(op->nmid==BJX2_NMID_PRED_T)
	{
		l0=UAX_GenLabelTemp(jctx);
		BJX2_JitTestVMRegImm(jctx, BJX2_REG_SR, 1);
		UAX_AsmInsnLabel(jctx, UAX_OP_JZ, l0|UAX_LBL_NEAR);
		i=BJX2_TryJitOpcode(jctx, cpu, tr, op1);
		
		if(i<=0)
		{
			BJX2_JitSyncRegs(jctx, cpu, tr);
			UAX_AsmMovRegImm(jctx, UAX_REG_RDX, (nlint)(op1));
			UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RCCTX);
			BJX2_JitEmitCallFPtr(jctx, cpu, op1->Run);
		}
		
		UAX_EmitLabel(jctx, l0);
		return(1);
	}
	
	if(op->nmid==BJX2_NMID_PRED_F)
	{
		l0=UAX_GenLabelTemp(jctx);
		BJX2_JitTestVMRegImm(jctx, BJX2_REG_SR, 1);
		UAX_AsmInsnLabel(jctx, UAX_OP_JNZ, l0|UAX_LBL_NEAR);
		i=BJX2_TryJitOpcode(jctx, cpu, tr, op1);

		if(i<=0)
		{
			BJX2_JitSyncRegs(jctx, cpu, tr);
			UAX_AsmMovRegImm(jctx, UAX_REG_RDX, (nlint)(op1));
			UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RCCTX);
			BJX2_JitEmitCallFPtr(jctx, cpu, op1->Run);
		}

		UAX_EmitLabel(jctx, l0);
		return(1);
	}
	
	return(0);
}


int BJX2_TryJitOpcodeArr_PredX(UAX_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *tr, BJX2_Opcode **opa, int n_op)
{
	BJX2_Opcode *op, *op1;
	u32 v;
	int l0, tf;
	int i, j;

	if(n_op<=0)
		return(0);

	op=opa[0]; tf=0;
	if(op->nmid==BJX2_NMID_PRED_T)
		tf=1;

	l0=UAX_GenLabelTemp(jctx);

	BJX2_JitSyncRegs(jctx, cpu, tr);

	BJX2_JitTestVMRegImm(jctx, BJX2_REG_SR, 1);
//	UAX_AsmInsnLabel(jctx, tf?UAX_OP_JZ:UAX_OP_JNZ, l0|UAX_LBL_NEAR);
	UAX_AsmInsnLabel(jctx, tf?UAX_OP_JZ:UAX_OP_JNZ, l0);

	for(i=0; i<n_op; i++)
	{
		op=opa[i];
		op1=op->data;

		j=BJX2_TryJitOpcode(jctx, cpu, tr, op1);
		if(j<=0)
		{
			BJX2_JitSyncRegs(jctx, cpu, tr);
			UAX_AsmMovRegImm(jctx, UAX_REG_RDX, (nlint)(op1));
			UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RCCTX);
			BJX2_JitEmitCallFPtr(jctx, cpu, op1->Run);
		}
	}

	BJX2_JitSyncRegs(jctx, cpu, tr);

	UAX_EmitLabel(jctx, l0);
	return(1);
}