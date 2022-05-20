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

int BJX2_JitStoreVMRegImm(UAX_Context *jctx, int idx, s64 val);

int BJX2_TryJitOpcode_MovReg(UAX_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *tr, BJX2_Opcode *op)
{
	int r0, r1, r2;

	if(op->nmid==BJX2_NMID_MOV)
	{
		if(op->fmid==BJX2_FMID_REGREG)
		{
			BJX2_JitMovVMRegVMReg(jctx, op->rn, op->rm);
			return(1);
		}

#if 1
		if(op->fmid==BJX2_FMID_IMMREG)
		{
			BJX2_JitStoreVMRegImm(jctx, op->rn, (s32)(op->imm));
			return(1);
		}
#endif

#if 1
		if(op->fmid==BJX2_FMID_IMMZREG)
		{
			BJX2_JitStoreVMRegImm(jctx, op->rn,
				0LL|
				(u32)(op->imm));
			return(1);
		}

		if(op->fmid==BJX2_FMID_IMMNREG)
		{
			BJX2_JitStoreVMRegImm(jctx, op->rn,
//				(-1LL<<32)|
				(~((1LL<<32)-1))|
				(u32)(op->imm));
			return(1);
		}
#endif

#if 0
		if(op->fmid==BJX2_FMID_IMMXREG)
		{
			BJX2_JitStoreVMRegImm(jctx, op->rn,
				((~(s64)((s32)(op->imm)>>31))<<32)|
				(u32)(op->imm));
			return(1);
		}
#endif
		
		return(0);
	}

#if 1
	if(op->nmid==BJX2_NMID_LDI)
	{
		if(op->fmid==BJX2_FMID_IMMREG)
		{
			BJX2_JitStoreVMRegImm(jctx, op->rn, (s32)(op->imm));
			return(1);
		}
	}
#endif

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
			BJX2_JitStoreVMRegImm(jctx, 0, (u32)(op->imm));
			return(1);
		}
	}
#endif

#if 1
	if(op->nmid==BJX2_NMID_LDIN)
	{
		if(op->fmid==BJX2_FMID_IMM)
		{
//			BJX2_JitStoreVMRegImm(jctx, 0, ((-1LL)<<32)|(op->imm));
			BJX2_JitStoreVMRegImm(jctx, 0, (~((1LL<<32)-1))|(op->imm));
			return(1);
		}
	}
#endif

#if 1
	if(op->nmid==BJX2_NMID_LDISH8)
	{
		if(op->fmid==BJX2_FMID_IMM)
		{
			r1=BJX2_JitLoadReadSyncVMReg(jctx, 0);
			if(r1!=UAX_REG_Z)
			{
				UAX_AsmShlRegImm(jctx, r1, 8);
				UAX_AsmOrRegImm(jctx, r1, (op->imm&255));
				return(1);
			}

			BJX2_JitLoadVMReg(jctx, 0, UAX_REG_RDX);
			UAX_AsmShlRegImm(jctx, UAX_REG_RDX, 8);
			UAX_AsmOrRegImm(jctx, UAX_REG_RDX, (op->imm&255));
			BJX2_JitStoreVMReg(jctx, 0, UAX_REG_RDX);

//			BJX2_JitStoreVMRegImm(jctx, 0, (u32)(op->imm));
			return(1);
		}
	}
#endif

#if 1
	if(op->nmid==BJX2_NMID_LDISH16)
	{
		if(op->fmid==BJX2_FMID_IMM)
		{
			r1=BJX2_JitLoadReadSyncVMReg(jctx, 0);
			if(r1!=UAX_REG_Z)
			{
				UAX_AsmShlRegImm(jctx, r1, 16);
				UAX_AsmOrRegImm(jctx, r1, (op->imm&65535));
				return(1);
			}

			BJX2_JitLoadVMReg(jctx, 0, UAX_REG_RDX);
			UAX_AsmShlRegImm(jctx, UAX_REG_RDX, 16);
			UAX_AsmOrRegImm(jctx, UAX_REG_RDX, (op->imm&65535));
			BJX2_JitStoreVMReg(jctx, 0, UAX_REG_RDX);
			return(1);
		}

		if(op->fmid==BJX2_FMID_IMMREG)
		{
			r1=BJX2_JitLoadReadSyncVMReg(jctx, op->rn);
			if(r1!=UAX_REG_Z)
			{
				UAX_AsmShlRegImm(jctx, r1, 16);
				UAX_AsmOrRegImm(jctx, r1, (op->imm&65535));
				return(1);
			}

			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			UAX_AsmShlRegImm(jctx, UAX_REG_RDX, 16);
			UAX_AsmOrRegImm(jctx, UAX_REG_RDX, (op->imm&65535));
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
			return(1);
		}
	}
#endif

#if 1
	if(op->nmid==BJX2_NMID_LDISH32)
	{
		if(op->fmid==BJX2_FMID_IMM)
		{
			BJX2_JitLoadVMReg(jctx, 0, UAX_REG_RDX);
			UAX_AsmMovRegImm(jctx, UAX_REG_RAX, (u32)(op->imm));
			UAX_AsmShlRegImm(jctx, UAX_REG_RDX, 32);
//			UAX_AsmOrRegImm(jctx, UAX_REG_RDX, (u32)(op->imm));
			UAX_AsmOrRegReg(jctx, UAX_REG_RDX, UAX_REG_RAX);
			BJX2_JitStoreVMReg(jctx, 0, UAX_REG_RDX);
			return(1);
		}

		if(op->fmid==BJX2_FMID_IMMREG)
		{
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			UAX_AsmMovRegImm(jctx, UAX_REG_RAX, (u32)(op->imm));
			UAX_AsmShlRegImm(jctx, UAX_REG_RDX, 32);
//			UAX_AsmOrRegImm(jctx, UAX_REG_RDX, (u32)(op->imm));
			UAX_AsmOrRegReg(jctx, UAX_REG_RDX, UAX_REG_RAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
			return(1);
		}
	}
#endif

	if(op->nmid==BJX2_NMID_FMOV)
	{
		if(op->fmid==BJX2_FMID_FREGREG)
		{
			BJX2_JitLoadVMDReg(jctx, op->rm, UAX_REG_RCX);
			BJX2_JitStoreVMDReg(jctx, op->rn, UAX_REG_RCX);
//			BJX2_JitMovVMRegVMReg(jctx, op->rn, op->rm);
			return(1);
		}
	}

	if(op->nmid==BJX2_NMID_FLDCD)
	{
		if(op->fmid==BJX2_FMID_FREG)
		{
			BJX2_JitLoadVMReg(jctx, 0, UAX_REG_RCX);
			BJX2_JitStoreVMDReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BJX2_FMID_FREGREG)
		{
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			BJX2_JitStoreVMDReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}
	}

	if(op->nmid==BJX2_NMID_FSTCD)
	{
		if(op->fmid==BJX2_FMID_FREG)
		{
			BJX2_JitLoadVMDReg(jctx, op->rn, UAX_REG_RCX);
			BJX2_JitStoreVMReg(jctx, 0, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BJX2_FMID_FREGREG)
		{
			BJX2_JitLoadVMDReg(jctx, op->rm, UAX_REG_RCX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}
	}

#if 1
	if(op->nmid==BJX2_NMID_FLDCI)
	{
		if(op->fmid==BJX2_FMID_FREG)
		{
			BJX2_JitLoadVMReg(jctx, 0, UAX_REG_RCX);
			UAX_AsmInsnRegReg(jctx, UAX_OP_CVTSI2SD,
				UAX_REG_XMM0, UAX_REG_RCX);
			BJX2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM0);
			return(1);
		}
	}
#endif

#if 1
	if(op->nmid==BJX2_NMID_FSTCI)
	{
		if(op->fmid==BJX2_FMID_FREG)
		{
			BJX2_JitLoadVMDReg(jctx, op->rn, UAX_REG_XMM0);
			UAX_AsmInsnRegReg(jctx, UAX_OP_CVTTSD2SI,
				UAX_REG_RCX, UAX_REG_XMM0);
			BJX2_JitStoreVMReg(jctx, 0, UAX_REG_RCX);
			return(1);
		}
	}
#endif

#if 1
	if(op->nmid==BJX2_NMID_FLDCF)
	{
		if(op->fmid==BJX2_FMID_FREG)
		{
			BJX2_JitLoadVMReg(jctx, 0, UAX_REG_XMM1);
			UAX_AsmInsnRegReg(jctx, UAX_OP_CVTSS2SD,
				UAX_REG_XMM0, UAX_REG_XMM1);
			BJX2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM0);
			return(1);
		}
	}

	if(op->nmid==BJX2_NMID_FSTCF)
	{
		if(op->fmid==BJX2_FMID_FREG)
		{
			BJX2_JitLoadVMDReg(jctx, op->rn, UAX_REG_XMM0);
			UAX_AsmInsnRegReg(jctx, UAX_OP_CVTSD2SS,
				UAX_REG_XMM1, UAX_REG_XMM0);
			BJX2_JitStoreVMReg(jctx, 0, UAX_REG_XMM1);
			return(1);
		}
	}
#endif

	return(0);
}

int BJX2_TryJitOpcode_ArithReg(UAX_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *tr, BJX2_Opcode *op)
{
	int l0, l1, l2;
	int dreg, sreg;

	if((op->nmid==BJX2_NMID_ADD) && (op->fmid==BJX2_FMID_REGREG))
	{
		if(op->rm==op->rn)
		{
			dreg=BJX2_JitLoadReadSyncVMReg(jctx, op->rn);
			if(dreg!=UAX_REG_Z)
			{
//				UAX_AsmInsnRegImm(jctx, UAX_OP_SHL, dreg, 1);
				UAX_AsmInsnRegReg(jctx, UAX_OP_ADD, dreg, dreg);
				return(1);
			}

			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RAX);
//			UAX_AsmInsnRegImm(jctx, UAX_OP_SHL, UAX_REG_RAX, 1);
			UAX_AsmInsnRegReg(jctx, UAX_OP_ADD, UAX_REG_RAX, UAX_REG_RAX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RAX);
			return(1);
		}
	
		BJX2_JitInsnVMRegVMReg(jctx,
			UAX_OP_ADD, op->rn, op->rm);
		return(1);
	}

	if((op->nmid==BJX2_NMID_ADD) && (op->fmid==BJX2_FMID_IMMREG))
	{
		BJX2_JitAddVMRegImm(jctx, op->rn, (s32)(op->imm));
		return(1);
	}

	if((op->nmid==BJX2_NMID_SUB) && (op->fmid==BJX2_FMID_REGREG))
	{
		BJX2_JitInsnVMRegVMReg(jctx,
			UAX_OP_SUB, op->rn, op->rm);
		return(1);
	}

	if((op->nmid==BJX2_NMID_AND) && (op->fmid==BJX2_FMID_REGREG))
	{
		BJX2_JitInsnVMRegVMReg(jctx,
			UAX_OP_AND, op->rn, op->rm);
		return(1);
	}

	if((op->nmid==BJX2_NMID_AND) && (op->fmid==BJX2_FMID_IMMREG))
	{
		BJX2_JitAndVMRegImm(jctx, op->rn, (s32)(op->imm));
		return(1);
	}

	if((op->nmid==BJX2_NMID_OR) && (op->fmid==BJX2_FMID_REGREG))
	{
		BJX2_JitInsnVMRegVMReg(jctx,
			UAX_OP_OR, op->rn, op->rm);
		return(1);
	}

	if((op->nmid==BJX2_NMID_OR) && (op->fmid==BJX2_FMID_IMMREG))
	{
		BJX2_JitOrVMRegImm(jctx, op->rn, (s32)(op->imm));
		return(1);
	}

	if((op->nmid==BJX2_NMID_XOR) && (op->fmid==BJX2_FMID_REGREG))
	{
		BJX2_JitInsnVMRegVMReg(jctx,
			UAX_OP_XOR, op->rn, op->rm);
		return(1);
	}

	if((op->nmid==BJX2_NMID_XOR) && (op->fmid==BJX2_FMID_IMMREG))
	{
		BJX2_JitXorVMRegImm(jctx, op->rn, (s32)(op->imm));
		return(1);
	}

	if((op->nmid==BJX2_NMID_ADD) &&
		((op->fmid==BJX2_FMID_REGREGREG) ||
		(op->fmid==BJX2_FMID_REGDRREG)))
	{
		if(op->rn!=op->ro)
		{
//			BJX2_JitMovVMRegVMReg(jctx, op->ro, op->rn);
//			BJX2_JitInsnVMRegVMReg(jctx, UAX_OP_ADD, op->ro, op->rm);
			BJX2_JitMovVMRegVMReg(jctx, op->rn, op->rm);
			BJX2_JitInsnVMRegVMReg(jctx, UAX_OP_ADD, op->rn, op->ro);
			return(1);
		}

		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
		BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_ADD, UAX_REG_RDX, UAX_REG_RCX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}

#if 1
	if((op->nmid==BJX2_NMID_ADDSL) &&
		(op->fmid==BJX2_FMID_REGREGREG))
	{
		if(op->rn!=op->ro)
		{
			BJX2_JitMovVMRegVMReg(jctx, op->rn, op->rm);
			BJX2_JitInsnVMRegVMReg(jctx, UAX_OP_ADD, op->rn, op->ro);
	//		__debugbreak();
			BJX2_JitInsnVMRegVMReg(jctx, UAX_OP_MOVSXD, op->rn, op->rn);
			return(1);
		}

		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
		BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_ADD, UAX_REG_RDX, UAX_REG_RCX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXD, UAX_REG_RDX, UAX_REG_EDX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_SUBSL) &&
		(op->fmid==BJX2_FMID_REGREGREG))
	{
		if(op->rn!=op->ro)
		{
			BJX2_JitMovVMRegVMReg(jctx, op->rn, op->rm);
			BJX2_JitInsnVMRegVMReg(jctx, UAX_OP_SUB, op->rn, op->ro);
	//		__debugbreak();
			BJX2_JitInsnVMRegVMReg(jctx, UAX_OP_MOVSXD, op->rn, op->rn);
			return(1);
		}

		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
		BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SUB, UAX_REG_RDX, UAX_REG_RCX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXD, UAX_REG_RDX, UAX_REG_EDX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_ADDUL) &&
		(op->fmid==BJX2_FMID_REGREGREG))
	{
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
		BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_ADD, UAX_REG_EDX, UAX_REG_ECX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_SUBUL) &&
		(op->fmid==BJX2_FMID_REGREGREG))
	{
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
		BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SUB, UAX_REG_EDX, UAX_REG_ECX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BJX2_NMID_ADD) && (op->fmid==BJX2_FMID_REGIMMREG))
	{
		if(op->rm!=op->rn)
		{
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_RCX, (s32)(op->imm));
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
		}else
			{ BJX2_JitAddVMRegImm(jctx, op->rn, (s32)(op->imm)); }
		return(1);
	}

	if((op->nmid==BJX2_NMID_SUB) && (op->fmid==BJX2_FMID_REGIMMREG))
	{
		if(op->rm!=op->rn)
		{
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SUB, UAX_REG_RCX, (s32)(op->imm));
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
		}else
			{ BJX2_JitAddVMRegImm(jctx, op->rn, -(s32)(op->imm)); }
		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_ADDSL) && (op->fmid==BJX2_FMID_REGIMMREG))
	{
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_RCX, (s32)(op->imm));
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXD, UAX_REG_RCX, UAX_REG_ECX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
		return(1);
	}

	if((op->nmid==BJX2_NMID_SUBSL) && (op->fmid==BJX2_FMID_REGIMMREG))
	{
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_SUB, UAX_REG_RCX, (s32)(op->imm));
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXD, UAX_REG_RCX, UAX_REG_ECX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_ADDUL) && (op->fmid==BJX2_FMID_REGIMMREG))
	{
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_ECX, (s32)(op->imm));
//		UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_RCX, (s32)(op->imm));
//		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_ECX, UAX_REG_ECX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_AND) && (op->fmid==BJX2_FMID_REGIMMREG))
	{
		if(op->rm!=op->rn)
		{
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_RCX, (s32)(op->imm));
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
		}else
			{ BJX2_JitAndVMRegImm(jctx, op->rn, (s32)(op->imm)); }
		return(1);
	}

	if((op->nmid==BJX2_NMID_OR) && (op->fmid==BJX2_FMID_REGIMMREG))
	{
		if(op->rm!=op->rn)
		{
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_OR, UAX_REG_RCX, (s32)(op->imm));
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
		}else
			{ BJX2_JitOrVMRegImm(jctx, op->rn, (s32)(op->imm)); }
		return(1);
	}

	if((op->nmid==BJX2_NMID_XOR) && (op->fmid==BJX2_FMID_REGIMMREG))
	{
		if(op->rm!=op->rn)
		{
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_XOR, UAX_REG_RCX, (s32)(op->imm));
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
		}else
			{ BJX2_JitXorVMRegImm(jctx, op->rn, (s32)(op->imm)); }
		return(1);
	}
#endif

//	return(0);

#if 1

#if 1
//	if((op->nmid==BJX2_NMID_SUB) &&
//		((op->fmid==BJX2_FMID_REGREGREG) ||
//		(op->fmid==BJX2_FMID_REGDRREG)))
	if((op->nmid==BJX2_NMID_SUB) &&
		(op->fmid==BJX2_FMID_REGREGREG))
	{
		if(op->rn!=op->ro)
		{
//			BJX2_JitMovVMRegVMReg(jctx, op->ro, op->rn);
//			BJX2_JitInsnVMRegVMReg(jctx, UAX_OP_SUB, op->ro, op->rm);
			BJX2_JitMovVMRegVMReg(jctx, op->rn, op->rm);
			BJX2_JitInsnVMRegVMReg(jctx, UAX_OP_SUB, op->rn, op->ro);
			return(1);
		}

		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
		BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SUB, UAX_REG_RDX, UAX_REG_RCX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}
#endif

//	return(0);

	if((op->nmid==BJX2_NMID_AND) &&
		((op->fmid==BJX2_FMID_REGREGREG) ||
		(op->fmid==BJX2_FMID_REGDRREG)))
	{
		if(op->rn!=op->ro)
		{
//			BJX2_JitMovVMRegVMReg(jctx, op->ro, op->rn);
//			BJX2_JitInsnVMRegVMReg(jctx, UAX_OP_AND, op->ro, op->rm);
			BJX2_JitMovVMRegVMReg(jctx, op->rn, op->rm);
			BJX2_JitInsnVMRegVMReg(jctx, UAX_OP_AND, op->rn, op->ro);
			return(1);
		}

		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
		BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_AND, UAX_REG_RDX, UAX_REG_RCX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}

//	return(0);

	if((op->nmid==BJX2_NMID_OR) &&
		((op->fmid==BJX2_FMID_REGREGREG) ||
		(op->fmid==BJX2_FMID_REGDRREG)))
	{
		if(op->rn!=op->ro)
		{
//			BJX2_JitMovVMRegVMReg(jctx, op->ro, op->rn);
//			BJX2_JitInsnVMRegVMReg(jctx, UAX_OP_OR, op->ro, op->rm);
			BJX2_JitMovVMRegVMReg(jctx, op->rn, op->rm);
			BJX2_JitInsnVMRegVMReg(jctx, UAX_OP_OR, op->rn, op->ro);
			return(1);
		}

		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
		BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_OR, UAX_REG_RDX, UAX_REG_RCX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}

	if((op->nmid==BJX2_NMID_XOR) &&
		((op->fmid==BJX2_FMID_REGREGREG) ||
		(op->fmid==BJX2_FMID_REGDRREG)))
	{
		if(op->rn!=op->ro)
		{
//			BJX2_JitMovVMRegVMReg(jctx, op->ro, op->rn);
//			BJX2_JitInsnVMRegVMReg(jctx, UAX_OP_XOR, op->ro, op->rm);
			BJX2_JitMovVMRegVMReg(jctx, op->rn, op->rm);
			BJX2_JitInsnVMRegVMReg(jctx, UAX_OP_XOR, op->rn, op->ro);
			return(1);
		}

		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
		BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_XOR, UAX_REG_RDX, UAX_REG_RCX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}
#endif


//	return(0);

#if 0
	if((op->nmid==BJX2_NMID_SWAPW) && (op->fmid==BJX2_FMID_REGREG))
	{
		if(op->rm==op->rn)
		{
			BJX2_JitInsnVMRegImm(jctx, UAX_OP_ROL, op->rn, 16);
			return(1);
		}
		
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RAX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ROL, UAX_REG_RAX, 16);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RAX);
		return(1);
	}

	if((op->nmid==BJX2_NMID_SWAPB) && (op->fmid==BJX2_FMID_REGREG))
	{
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ROL, UAX_REG_AX, 8);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}
#endif

	if((op->nmid==BJX2_NMID_NEG) && (op->fmid==BJX2_FMID_REGREG))
	{
		if(op->rm==op->rn)
		{
			BJX2_JitInsnVMReg(jctx, UAX_OP_NEG, op->rn);
			return(1);
		}
		
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RAX);
		UAX_AsmInsnReg(jctx, UAX_OP_NEG, UAX_REG_RAX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RAX);
		return(1);
	}

	if((op->nmid==BJX2_NMID_NOT) && (op->fmid==BJX2_FMID_REGREG))
	{
		if(op->rm==op->rn)
		{
			BJX2_JitInsnVMReg(jctx, UAX_OP_NOT, op->rn);
			return(1);
		}
		
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RAX);
		UAX_AsmInsnReg(jctx, UAX_OP_NOT, UAX_REG_RAX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RAX);
		return(1);
	}

	if((op->nmid==BJX2_NMID_NEG) && (op->fmid==BJX2_FMID_REG))
	{
		BJX2_JitInsnVMReg(jctx, UAX_OP_NEG, op->rn);
		return(1);
	}

	if((op->nmid==BJX2_NMID_NOT) && (op->fmid==BJX2_FMID_REG))
	{
		BJX2_JitInsnVMReg(jctx, UAX_OP_NOT, op->rn);
		return(1);
	}

#if 1

#if 1
	if((op->nmid==BJX2_NMID_ADC) && (op->fmid==BJX2_FMID_REGREG))
	{
#if 1
		BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
		BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RCX);
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8Q);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_RAX, UAX_REG_RDX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_RDX, (s32)(~1));
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_RAX, 1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_ADD, UAX_REG_R8Q, UAX_REG_RAX);
		UAX_AsmInsnReg(jctx, UAX_OP_SETC, UAX_REG_AL);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_R9Q, UAX_REG_RAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_ADD, UAX_REG_RCX, UAX_REG_R8Q);
		UAX_AsmInsnReg(jctx, UAX_OP_SETC, UAX_REG_AL);
		UAX_AsmInsnRegReg(jctx, UAX_OP_OR, UAX_REG_RAX, UAX_REG_R9Q);
		
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_OR, UAX_REG_RDX, UAX_REG_RAX);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
		return(1);
#endif
	}
#endif

#if 0
	if((op->nmid==BJX2_NMID_SUBC) && (op->fmid==BJX2_FMID_REGREG))
	{
#if 1
		BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_EDX);
		BJX2_JitLoadVMRegZx(jctx, op->rn, UAX_REG_RCX);
		BJX2_JitLoadVMRegZx(jctx, op->rm, UAX_REG_RAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SUB, UAX_REG_RCX, UAX_REG_RAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_EAX, UAX_REG_EDX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_EDX, (s32)(~1));
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_RAX, 1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SUB, UAX_REG_RCX, UAX_REG_RAX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_ECX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_SHR, UAX_REG_RCX, 32);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_RCX, 1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_OR, UAX_REG_EDX, UAX_REG_ECX);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, UAX_REG_EDX);
		return(1);
#endif

#if 0
		BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_EDX);
		BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_ECX);
//		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
//		UAX_AsmInsnRegReg(jctx, UAX_OP_SUB, UAX_REG_ECX, UAX_REG_EAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_EAX, UAX_REG_EDX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_EDX, (s32)(~1));
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_EAX, 1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_ADD, UAX_REG_R8D, UAX_REG_EAX);
		UAX_AsmInsnReg(jctx, UAX_OP_SETC, UAX_REG_AL);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_R9D, UAX_REG_EAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SUB, UAX_REG_ECX, UAX_REG_R8D);
//		UAX_AsmInsnRegReg(jctx, UAX_OP_SUB, UAX_REG_ECX, UAX_REG_EAX);
		UAX_AsmInsnReg(jctx, UAX_OP_SETC, UAX_REG_AL);
		UAX_AsmInsnRegReg(jctx, UAX_OP_OR, UAX_REG_EAX, UAX_REG_R9D);
		
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_ECX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_OR, UAX_REG_EDX, UAX_REG_EAX);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, UAX_REG_EDX);
		return(1);
#endif
	}
#endif

#endif

	if((op->nmid==BJX2_NMID_CSELT) && (op->fmid==BJX2_FMID_REGREGREG))
	{
		BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_EDX);
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
		BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_RAX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_TEST, UAX_REG_EDX, 1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_CMOVE, UAX_REG_RCX, UAX_REG_RAX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
		return(1);
	}

//	return(0);

#if 1
	if((op->nmid==BJX2_NMID_SHLL) &&
		(op->fmid==BJX2_FMID_REG))
	{
		BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RCX);
		BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
		UAX_AsmMovRegReg(jctx, UAX_REG_RAX, UAX_REG_RCX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_RDX, (s32)(~1));
		UAX_AsmInsnRegImm(jctx, UAX_OP_SHL, UAX_REG_RAX, 1);
		UAX_AsmInsnRegImm(jctx, UAX_OP_SHR, UAX_REG_RCX, 63);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_OR, UAX_REG_RDX, UAX_REG_RCX);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_SHAR) && (op->fmid==BJX2_FMID_REG))
	{
		BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RCX);
		BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
		UAX_AsmMovRegReg(jctx, UAX_REG_RAX, UAX_REG_RCX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_RDX, (s32)(~1));
		UAX_AsmInsnRegImm(jctx, UAX_OP_SAR, UAX_REG_RAX, 1);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_RCX, 1);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_OR, UAX_REG_RDX, UAX_REG_RCX);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_SHLR) && (op->fmid==BJX2_FMID_REG))
	{
		BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_RCX);
		BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
		UAX_AsmMovRegReg(jctx, UAX_REG_RAX, UAX_REG_RCX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_RDX, (s32)(~1));
		UAX_AsmInsnRegImm(jctx, UAX_OP_SHR, UAX_REG_RAX, 1);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_RCX, 1);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_OR, UAX_REG_RDX, UAX_REG_RCX);
		BJX2_JitStoreVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
		return(1);
	}
#endif

//	return(0);

	if((op->nmid==BJX2_NMID_SHLL) && (op->fmid==BJX2_FMID_IMMREG))
	{
		BJX2_JitShlVMRegImm(jctx, op->rn, op->imm);
		return(1);
	}

	if((op->nmid==BJX2_NMID_SHLR) && (op->fmid==BJX2_FMID_IMMREG))
	{
		BJX2_JitShrVMRegImm(jctx, op->rn, op->imm);
		return(1);
	}

	if((op->nmid==BJX2_NMID_SHAR) && (op->fmid==BJX2_FMID_IMMREG))
	{
		BJX2_JitSarVMRegImm(jctx, op->rn, op->imm);
		return(1);
	}

	if((op->nmid==BJX2_NMID_MOVT) && (op->fmid==BJX2_FMID_REG))
	{
		BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_RDX, 1);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}

	if((op->nmid==BJX2_NMID_MOVNT) && (op->fmid==BJX2_FMID_REG))
	{
		BJX2_JitLoadVMReg(jctx, BJX2_REG_SR, UAX_REG_RDX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_RDX, 1);
		UAX_AsmInsnRegImm(jctx, UAX_OP_XOR, UAX_REG_RDX, 1);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}

#if 0
	if((op->nmid==BJX2_NMID_SHLL2) && (op->fmid==BJX2_FMID_REGRN))
	{
		BJX2_JitShlVMRegImm(jctx, op->rn, 2);
		return(1);
	}

	if((op->nmid==BJX2_NMID_SHLL8) && (op->fmid==BJX2_FMID_REGRN))
	{
		BJX2_JitShlVMRegImm(jctx, op->rn, 8);
		return(1);
	}

	if((op->nmid==BJX2_NMID_SHLL16) && (op->fmid==BJX2_FMID_REGRN))
	{
		BJX2_JitShlVMRegImm(jctx, op->rn, 16);
		return(1);
	}

	if((op->nmid==BJX2_NMID_SHLR2) && (op->fmid==BJX2_FMID_REGRN))
	{
		BJX2_JitShrVMRegImm(jctx, op->rn, 2);
		return(1);
	}

	if((op->nmid==BJX2_NMID_SHLR8) && (op->fmid==BJX2_FMID_REGRN))
	{
		BJX2_JitShrVMRegImm(jctx, op->rn, 8);
		return(1);
	}

	if((op->nmid==BJX2_NMID_SHLR16) && (op->fmid==BJX2_FMID_REGRN))
	{
		BJX2_JitShrVMRegImm(jctx, op->rn, 16);
		return(1);
	}
#endif


#if 1
	if((op->nmid==BJX2_NMID_SHAD) &&
		(op->fmid==BJX2_FMID_REGIMMREG))
	{
		if(((sbyte)op->imm)<0)
		{
			if(!(op->imm&31))
			{
				BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
				UAX_AsmInsnRegImm(jctx, UAX_OP_SAR, UAX_REG_ECX, 31);
				UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXD,
					UAX_REG_RCX, UAX_REG_ECX);
				BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
				return(1);
			}
		
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SAR, UAX_REG_ECX, -op->imm);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXD, UAX_REG_RCX, UAX_REG_ECX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}else if(((sbyte)op->imm)>0)
		{
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SHL, UAX_REG_ECX, op->imm);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXD, UAX_REG_RCX, UAX_REG_ECX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}else
		{
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXD, UAX_REG_RCX, UAX_REG_ECX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_SHLD) &&
		(op->fmid==BJX2_FMID_REGIMMREG))
	{
		if(((sbyte)op->imm)<0)
		{		
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SHR, UAX_REG_ECX, -op->imm);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}else if(((sbyte)op->imm)>0)
		{
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SHL, UAX_REG_ECX, op->imm);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}else
		{
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_SHADQ) &&
		(op->fmid==BJX2_FMID_REGIMMREG))
	{
		if(((sbyte)op->imm)<0)
		{
			if(!(op->imm&63))
			{
				BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
				UAX_AsmInsnRegImm(jctx, UAX_OP_SAR, UAX_REG_RCX, 63);
				BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
				return(1);
			}
		
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SAR, UAX_REG_RCX, -op->imm);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}else if(((sbyte)op->imm)>0)
		{
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SHL, UAX_REG_RCX, op->imm);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}else
		{
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_SHLDQ) &&
		(op->fmid==BJX2_FMID_REGIMMREG))
	{
		if(((sbyte)op->imm)<0)
		{		
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SHR, UAX_REG_RCX, -op->imm);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}else if(((sbyte)op->imm)>0)
		{
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SHL, UAX_REG_RCX, op->imm);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}else
		{
			BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_MULS) && (op->fmid==BJX2_FMID_REGREG))
	{
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_ECX);
		UAX_AsmInsnReg(jctx, UAX_OP_IMUL, UAX_REG_ECX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXD, UAX_REG_RDX, UAX_REG_EDX);
		BJX2_JitStoreVMReg(jctx, 0, UAX_REG_RAX);
		BJX2_JitStoreVMReg(jctx, 1, UAX_REG_RDX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_MULS) && (op->fmid==BJX2_FMID_REGREGREG))
	{
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_ECX);
		UAX_AsmInsnReg(jctx, UAX_OP_IMUL, UAX_REG_ECX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXD, UAX_REG_RDX, UAX_REG_EAX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_MULU) && (op->fmid==BJX2_FMID_REGREG))
	{
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_ECX);
		UAX_AsmInsnReg(jctx, UAX_OP_MUL, UAX_REG_ECX);
//		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXD, UAX_REG_RDX, UAX_REG_EDX);
		BJX2_JitStoreVMReg(jctx, 0, UAX_REG_RAX);
		BJX2_JitStoreVMReg(jctx, 1, UAX_REG_RDX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_MULU) && (op->fmid==BJX2_FMID_REGREGREG))
	{
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_ECX);
		UAX_AsmInsnReg(jctx, UAX_OP_MUL, UAX_REG_ECX);
//		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXD, UAX_REG_RDX, UAX_REG_EAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_EDX, UAX_REG_EAX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}
#endif

	return(0);
}


int BJX2_TryJitOpcode_SignExtOp(UAX_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *tr, BJX2_Opcode *op)
{
	int l0, l1, l2;

#if 1
	if((op->nmid==BJX2_NMID_EXTUB) &&
		(op->fmid==BJX2_FMID_REGREG))
	{
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
//		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVZXB, UAX_REG_EAX, UAX_REG_DL);
//		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);

		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_RDX, 255);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BJX2_NMID_EXTUW) &&
		(op->fmid==BJX2_FMID_REGREG))
	{
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
//		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVZXW, UAX_REG_EAX, UAX_REG_DX);
//		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);

		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_RDX, 65535);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_EXTUL) &&
		(op->fmid==BJX2_FMID_REGREG))
	{
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_EAX, UAX_REG_EDX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RAX);

//		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_RDX, 65535);
//		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BJX2_NMID_EXTSB) &&
		(op->fmid==BJX2_FMID_REGREG))
	{
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXB, UAX_REG_RAX, UAX_REG_DL);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RAX);
		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BJX2_NMID_EXTSW) &&
		(op->fmid==BJX2_FMID_REGREG))
	{
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXW, UAX_REG_EAX, UAX_REG_DX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXD, UAX_REG_RAX, UAX_REG_EAX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RAX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_EXTSL) &&
		(op->fmid==BJX2_FMID_REGREG))
	{
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXD, UAX_REG_RAX, UAX_REG_EDX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RAX);
		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BJX2_NMID_EXTUB) &&
		(op->fmid==BJX2_FMID_REG))
	{
		BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_RDX, 255);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BJX2_NMID_EXTUW) &&
		(op->fmid==BJX2_FMID_REG))
	{
		BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_RDX, 65535);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_EXTUL) &&
		(op->fmid==BJX2_FMID_REG))
	{
		BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_EAX, UAX_REG_EDX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RAX);
		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BJX2_NMID_EXTSB) &&
		(op->fmid==BJX2_FMID_REG))
	{
		BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXB, UAX_REG_RAX, UAX_REG_DL);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RAX);
		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BJX2_NMID_EXTSW) &&
		(op->fmid==BJX2_FMID_REG))
	{
		BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXW, UAX_REG_EAX, UAX_REG_DX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXD, UAX_REG_RAX, UAX_REG_EAX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RAX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_EXTSL) &&
		(op->fmid==BJX2_FMID_REG))
	{
		BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXD, UAX_REG_RAX, UAX_REG_EDX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RAX);
		return(1);
	}
#endif



	if((op->nmid==BJX2_NMID_NOP) &&
		(op->fmid==BJX2_FMID_NONE))
	{
		return(1);
	}

	if((op->nmid==BJX2_NMID_NOP) &&
		(op->fmid==BJX2_FMID_Z))
	{
		return(1);
	}

#if 0
	if((op->nmid==BJX2_NMID_SHAD) &&
		(op->fmid==BJX2_FMID_REGIMM))
	{
		if(((s32)op->imm)>=0)
		{
			BJX2_JitStoreVMRegImm(jctx, op->rm, (s32)op->imm);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SHL,
				UAX_REG_EDX, (op->imm)&31);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
			return(1);
		}else if(op->imm&31)
		{
			BJX2_JitStoreVMRegImm(jctx, op->rm, (s32)op->imm);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SAR,
				UAX_REG_EDX, 32-((op->imm)&31));
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
			return(1);
		}else
		{
			BJX2_JitStoreVMRegImm(jctx, op->rm, (s32)op->imm);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SAR, UAX_REG_EDX, 31);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
			return(1);
		}
	}

	if((op->nmid==BJX2_NMID_SHLD) &&
		(op->fmid==BJX2_FMID_REGIMM))
	{
		if(((s32)op->imm)>=0)
		{
			BJX2_JitStoreVMRegImm(jctx, op->rm, (s32)op->imm);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SHL,
				UAX_REG_EDX, (op->imm)&31);
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
			return(1);
		}else if(op->imm&31)
		{
			BJX2_JitStoreVMRegImm(jctx, op->rm, (s32)op->imm);
			BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SHR,
				UAX_REG_EDX, 32-((op->imm)&31));
			BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
			return(1);
		}else
		{
			BJX2_JitStoreVMRegImm(jctx, op->rm, (s32)op->imm);
			BJX2_JitStoreVMRegImm(jctx, op->rn, 0);
			return(1);
		}
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_SHAD) &&
		(op->fmid==BJX2_FMID_REGREG))
	{
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);
		l2=UAX_GenLabelTemp(jctx);
	
		BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_TEST,
			UAX_REG_ECX, UAX_REG_ECX);
		UAX_AsmInsnLabel(jctx, UAX_OP_JS, l0|UAX_LBL_NEAR);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 31);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SHL, UAX_REG_EDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l0);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 31);
		UAX_AsmInsnLabel(jctx, UAX_OP_JE, l1|UAX_LBL_NEAR);
		UAX_AsmInsnReg(jctx, UAX_OP_NEG, UAX_REG_ECX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_ECX, 32);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SAR, UAX_REG_EDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l1);
		UAX_AsmInsnRegImm(jctx, UAX_OP_SAR, UAX_REG_EDX, 31);

		UAX_EmitLabel(jctx, l2);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
		return(1);
	}

	if((op->nmid==BJX2_NMID_SHLD) &&
		(op->fmid==BJX2_FMID_REGREG))
	{
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);
		l2=UAX_GenLabelTemp(jctx);
	
		BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_TEST,
			UAX_REG_ECX, UAX_REG_ECX);
		UAX_AsmInsnLabel(jctx, UAX_OP_JS, l0|UAX_LBL_NEAR);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 31);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SHL, UAX_REG_EDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l0);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 31);
		UAX_AsmInsnLabel(jctx, UAX_OP_JE, l1|UAX_LBL_NEAR);
		UAX_AsmInsnReg(jctx, UAX_OP_NEG, UAX_REG_ECX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_ECX, 32);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SHR, UAX_REG_EDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_XOR, UAX_REG_EDX, UAX_REG_EDX);

		UAX_EmitLabel(jctx, l2);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BJX2_NMID_SHAD) &&
		(op->fmid==BJX2_FMID_REGREGREG))
	{
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);
		l2=UAX_GenLabelTemp(jctx);
	
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_ECX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_TEST,
			UAX_REG_ECX, UAX_REG_ECX);
		UAX_AsmInsnLabel(jctx, UAX_OP_JS, l0|UAX_LBL_NEAR);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 31);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SHL, UAX_REG_EDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l0);
//		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 31);
//		UAX_AsmInsnLabel(jctx, UAX_OP_JE, l1|UAX_LBL_NEAR);
		UAX_AsmInsnReg(jctx, UAX_OP_NEG, UAX_REG_ECX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_ECX, 32);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SAR, UAX_REG_EDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

//		UAX_EmitLabel(jctx, l1);
//		UAX_AsmInsnRegImm(jctx, UAX_OP_SAR, UAX_REG_EDX, 31);

		UAX_EmitLabel(jctx, l2);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXD, UAX_REG_RDX, UAX_REG_EDX);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}

	if((op->nmid==BJX2_NMID_SHLD) &&
		(op->fmid==BJX2_FMID_REGREGREG))
	{
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);
		l2=UAX_GenLabelTemp(jctx);
	
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_ECX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_TEST,
			UAX_REG_ECX, UAX_REG_ECX);
		UAX_AsmInsnLabel(jctx, UAX_OP_JS, l0|UAX_LBL_NEAR);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 31);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SHL, UAX_REG_EDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l0);
//		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 31);
//		UAX_AsmInsnLabel(jctx, UAX_OP_JE, l1|UAX_LBL_NEAR);
		UAX_AsmInsnReg(jctx, UAX_OP_NEG, UAX_REG_ECX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_ECX, 32);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SHR, UAX_REG_EDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

//		UAX_EmitLabel(jctx, l1);
//		UAX_AsmInsnRegReg(jctx, UAX_OP_XOR, UAX_REG_EDX, UAX_REG_EDX);

		UAX_EmitLabel(jctx, l2);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}
#endif

#if 0
	if((op->nmid==BJX2_NMID_SHAR) &&
		(op->fmid==BJX2_FMID_REGREGREG))
	{
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);
		l2=UAX_GenLabelTemp(jctx);
	
		BJX2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_TEST,
			UAX_REG_ECX, UAX_REG_ECX);
		UAX_AsmInsnLabel(jctx, UAX_OP_JS, l0|UAX_LBL_NEAR);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 31);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SAR, UAX_REG_EDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l0);
		UAX_AsmInsnReg(jctx, UAX_OP_NEG, UAX_REG_ECX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_ECX, 32);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SHL, UAX_REG_EDX, UAX_REG_CL);
//		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l2);
		BJX2_JitStoreVMReg(jctx, op->ro, UAX_REG_EDX);
		return(1);
	}
#endif


#if 1
	if((op->nmid==BJX2_NMID_SHADQ) &&
		(op->fmid==BJX2_FMID_REGREGREG))
	{
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);
		l2=UAX_GenLabelTemp(jctx);
	
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
		BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_ECX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_TEST,
			UAX_REG_ECX, UAX_REG_ECX);
		UAX_AsmInsnLabel(jctx, UAX_OP_JS, l0|UAX_LBL_NEAR);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 63);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SHL, UAX_REG_RDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l0);
		UAX_AsmInsnReg(jctx, UAX_OP_NEG, UAX_REG_ECX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_ECX, 64);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SAR, UAX_REG_RDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l2);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}

	if((op->nmid==BJX2_NMID_SHLDQ) &&
		(op->fmid==BJX2_FMID_REGREGREG))
	{
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);
		l2=UAX_GenLabelTemp(jctx);
	
		BJX2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
		BJX2_JitLoadVMReg(jctx, op->ro, UAX_REG_ECX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_TEST,
			UAX_REG_ECX, UAX_REG_ECX);
		UAX_AsmInsnLabel(jctx, UAX_OP_JS, l0|UAX_LBL_NEAR);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 63);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SHL, UAX_REG_RDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l0);
		UAX_AsmInsnReg(jctx, UAX_OP_NEG, UAX_REG_ECX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_ECX, 64);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SHR, UAX_REG_RDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l2);
		BJX2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}
#endif

//	return(0);

#if 1
	if(op->nmid==BJX2_NMID_FADD)
	{
		if(op->fmid==BJX2_FMID_FREGREG)
		{
			BJX2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM0);
			BJX2_JitLoadVMDReg(jctx, op->rn, UAX_REG_XMM1);
			UAX_AsmInsnRegReg(jctx, UAX_OP_ADDSD,
				UAX_REG_XMM1, UAX_REG_XMM0);
			BJX2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM1);
//			BJX2_JitMovVMRegVMReg(jctx, op->rn, op->rm);
			return(1);
		}

		if(op->fmid==BJX2_FMID_FREGREGREG)
		{
			BJX2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM0);
			BJX2_JitLoadVMDReg(jctx, op->ro, UAX_REG_XMM1);
			UAX_AsmInsnRegReg(jctx, UAX_OP_ADDSD,
				UAX_REG_XMM0, UAX_REG_XMM1);
			BJX2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM0);
			return(1);
		}
	}
#endif

#if 1
	if(op->nmid==BJX2_NMID_FSUB)
	{
		if(op->fmid==BJX2_FMID_FREGREG)
		{
			BJX2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM0);
			BJX2_JitLoadVMDReg(jctx, op->rn, UAX_REG_XMM1);
			UAX_AsmInsnRegReg(jctx, UAX_OP_SUBSD,
				UAX_REG_XMM1, UAX_REG_XMM0);
			BJX2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM1);
//			BJX2_JitMovVMRegVMReg(jctx, op->rn, op->rm);
			return(1);
		}

		if(op->fmid==BJX2_FMID_FREGREGREG)
		{
			BJX2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM0);
			BJX2_JitLoadVMDReg(jctx, op->ro, UAX_REG_XMM1);
			UAX_AsmInsnRegReg(jctx, UAX_OP_SUBSD,
				UAX_REG_XMM0, UAX_REG_XMM1);
			BJX2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM0);
			return(1);
		}
	}
#endif

#if 1
	if(op->nmid==BJX2_NMID_FMUL)
	{
		if(op->fmid==BJX2_FMID_FREGREG)
		{
			BJX2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM0);
			BJX2_JitLoadVMDReg(jctx, op->rn, UAX_REG_XMM1);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MULSD,
				UAX_REG_XMM1, UAX_REG_XMM0);
			BJX2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM1);
//			BJX2_JitMovVMRegVMReg(jctx, op->rn, op->rm);
			return(1);
		}

		if(op->fmid==BJX2_FMID_FREGREGREG)
		{
			BJX2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM0);
			BJX2_JitLoadVMDReg(jctx, op->ro, UAX_REG_XMM1);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MULSD,
				UAX_REG_XMM0, UAX_REG_XMM1);
			BJX2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM0);
			return(1);
		}
	}
#endif

#if 0
	if(op->nmid==BJX2_NMID_FDIV)
	{
		if(op->fmid==BJX2_FMID_FREGREG)
		{
			BJX2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM0);
			BJX2_JitLoadVMDReg(jctx, op->rn, UAX_REG_XMM1);
			UAX_AsmInsnRegReg(jctx, UAX_OP_DIVSD,
				UAX_REG_XMM1, UAX_REG_XMM0);
			BJX2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM1);
//			BJX2_JitMovVMRegVMReg(jctx, op->rn, op->rm);
			return(1);
		}
	}
#endif

	return(0);
}
