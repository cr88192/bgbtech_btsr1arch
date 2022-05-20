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

/*
RAX( 0): Scratch
RCX( 1): Scratch
RDX( 2): Scratch
RBX( 3): Var Cache (W64) / CPU Context (AMD64)
RSP( 4): OS Stack
RBP( 5): Frame / Cache
RSI( 6): Var Cache (W64) / Scratch (AMD64)
RDI( 7): CPU Context (W64) / Scratch (AMD64)
R8 ( 8): Scratch
R9 ( 9): Scratch
R10(10): Scratch
R11(11): Scratch
R12(12): Var Cache
R13(13): Var Cache
R14(14): Var Cache
R15(15): Var Cache

[RBP+24]: Trace
[RBP+16]: CPU
[RBP+ 8]/[RSP+504]: Return RIP
[RBP+ 0]/[RSP+496]: Saved RBP
[RBP- 8]/[RSP+488]: Saved RSI
[RBP-16]/[RSP+480]: Saved RDI
[RBP-24]/[RSP+472]: Saved RBX
[RBP-32]/[RSP+464]: Saved R12
[RBP-40]/[RSP+456]: Saved R13
[RBP-48]/[RSP+448]: Saved R14
[RBP-56]/[RSP+440]: Saved R15

Frame 

Args:
	 RCX,  RDX,   R8,   R9
	XMM0, XMM1, XMM2, XMM3

[RSP+8]: Opcode
[RSP+0]: CPU

Locals and stack are placed end-to-end in the frame.
The stack is placed after the locals.

Possible Naive Reg Allocator:
  First Pass: Stat most-used vars in a trace.
  Assign top N vars to cache registers.
  Operations loading/storing this var directed to register.
  Write back when trace ends.

 */

//#ifdef _M_X64
#if defined(UAX_WINX64) || defined(UAX_SYSVAMD64)

#define BTEJ_X64_FRAMESZ	512
#define BTEJ_X64_FRAMEADJ	(BTEJ_X64_FRAMESZ-8)

#define BTEJ_X64_FRIDX_TRACE	1
#define BTEJ_X64_FRIDX_CPU		0
#define BTEJ_X64_FRIDX_RIP		-1
#define BTEJ_X64_FRIDX_RBP		-2
#define BTEJ_X64_FRIDX_RSI		-3
#define BTEJ_X64_FRIDX_RDI		-4
#define BTEJ_X64_FRIDX_RBX		-5
#define BTEJ_X64_FRIDX_R12		-6
#define BTEJ_X64_FRIDX_R13		-7
#define BTEJ_X64_FRIDX_R14		-8
#define BTEJ_X64_FRIDX_R15		-9


int BJX2_JitLoadFrame(UAX_Context *jctx, int idx, int reg)
{
//	if(idx!=(idx&127))
//		{ JX2_DBGBREAK }

	UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
		reg, UAX_REG_RSP, BTEJ_X64_FRAMESZ+(idx*8));
	return(0);
}

int BJX2_JitStoreFrame(UAX_Context *jctx, int idx, int reg)
{
//	if(idx!=(idx&127))
//		{ JX2_DBGBREAK }

	UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
		UAX_REG_RSP, BTEJ_X64_FRAMESZ+(idx*8), reg);
	return(0);
}

int BJX2_JitLoadCtxVMReg(UAX_Context *jctx, int idx, int reg)
{
	int reg1;

	if(idx!=(idx&127))
		{ JX2_DBGBREAK }

	if(UAX_Asm_RegIsXmmP(reg))
	{
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOVSD,
			reg, UAX_REG_RCCTX, offsetof(BJX2_Context, regs)+(idx*8));
	}else
	{
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			reg, UAX_REG_RCCTX, offsetof(BJX2_Context, regs)+(idx*8));
	}
	return(0);
}

int BJX2_JitStoreCtxVMReg(UAX_Context *jctx, int idx, int reg)
{
	int reg1;

	if(idx!=(idx&127))
		{ JX2_DBGBREAK }

	if(UAX_Asm_RegIsXmmP(reg))
	{
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOVSD,
			UAX_REG_RCCTX, offsetof(BJX2_Context, regs)+(idx*8), reg);
	}else
	{
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BJX2_Context, regs)+(idx*8), reg);
	}
	return(0);
}

int BJX2_JitLoadReadSyncVMReg(UAX_Context *jctx, int idx)
{
	int reg;
	int i, j, k;

	if(idx!=(idx&127))
		{ JX2_DBGBREAK }

	for(i=0; i<UAX_MAX_CACHEVAR; i++)
		if((jctx->reg_resv&(1<<i)) && (jctx->reg_idx[i]==idx))
	{
		reg=jctx->reg_reg[i];
		if(jctx->reg_live&(1<<i))
			return(reg);

		BJX2_JitLoadCtxVMReg(jctx, idx, reg);

		jctx->reg_live|=(1<<i);
		return(reg);
	}
	
	return(UAX_REG_Z);
}

int BJX2_JitLoadWriteSyncVMReg(UAX_Context *jctx, int idx)
{
	int reg;
	int i, j, k;

	if(idx!=(idx&127))
		{ JX2_DBGBREAK }

	for(i=0; i<UAX_MAX_CACHEVAR; i++)
		if((jctx->reg_resv&(1<<i)) && (jctx->reg_idx[i]==idx))
	{
		reg=jctx->reg_reg[i];
		if(jctx->reg_live&(1<<i))
			return(reg);
		jctx->reg_live|=(1<<i);
		return(reg);
	}
	
	return(UAX_REG_Z);
}

int BJX2_JitLoadVMReg(UAX_Context *jctx, int idx, int reg)
{
	int reg0, reg1;

	if(idx!=(idx&127))
		{ JX2_DBGBREAK }

	reg1=BJX2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		if(UAX_Asm_RegIsXmmP(reg))
		{
			reg1=UAX_Asm_RegAsDWord(reg1);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVD, reg, reg1);
		}
		else if(UAX_Asm_RegTypeEquivP(reg, reg1))
			{ UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, reg, reg1); }
		else
		{
			reg0=UAX_Asm_RegAsDWord(reg);
			reg1=UAX_Asm_RegAsDWord(reg1);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, reg0, reg1);
		}
		return(0);
	}

	BJX2_JitLoadCtxVMReg(jctx, idx, reg);
	return(0);
}

int BJX2_JitStoreVMReg(UAX_Context *jctx, int idx, int reg)
{
	int reg0, reg1;

	if(idx!=(idx&127))
		{ JX2_DBGBREAK }

	reg1=BJX2_JitLoadWriteSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		if(UAX_Asm_RegIsXmmP(reg))
		{
			reg1=UAX_Asm_RegAsDWord(reg1);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVD, reg1, reg);
		}
		else if(UAX_Asm_RegTypeEquivP(reg, reg1))
			{ UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, reg1, reg); }
		else
		{
			reg0=UAX_Asm_RegAsDWord(reg);
			reg1=UAX_Asm_RegAsDWord(reg1);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, reg1, reg0);
		}
		return(0);
	}

	BJX2_JitStoreCtxVMReg(jctx, idx, reg);
	return(0);
}

int BJX2_JitStoreVMRegImm(UAX_Context *jctx, int idx, s64 val)
{
	int reg1;

	if(idx!=(idx&127))
		{ JX2_DBGBREAK }

	reg1=BJX2_JitLoadWriteSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
//		UAX_AsmInsnRegImm(jctx, UAX_OP_MOV, UAX_REG_RAX, val);
//		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, reg1, UAX_REG_RAX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_MOV, reg1, val);
		return(0);
	}

//	if(jctx->isjq)
	if(1)
	{
		UAX_AsmInsnRegImm(jctx, UAX_OP_MOV, UAX_REG_RAX, val);
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BJX2_Context, regs)+(idx*8),
			UAX_REG_RAX);

//		UAX_AsmInsnStRegDispImm32(jctx, UAX_OP_MOV,
//			UAX_REG_RCCTX, offsetof(BJX2_Context, regs)+(idx*8+0),
//			(s32)val);
//		UAX_AsmInsnStRegDispImm32(jctx, UAX_OP_MOV,
//			UAX_REG_RCCTX, offsetof(BJX2_Context, regs)+(idx*8+4),
//			(s32)(val>>32));
	}else
	{
		UAX_AsmInsnStRegDispImm32(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BJX2_Context, regs)+(idx*8),
			(s32)val);
	}
	return(0);
}

int BJX2_JitLoadVMRegZx(UAX_Context *jctx, int idx, int reg)
{
	int reg1, reg1b;

	if(idx!=(idx&127))
		{ JX2_DBGBREAK }

	reg1=BJX2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		reg1b=UAX_REG_RAX|(reg1&15);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, reg, reg1b);
		return(0);
	}

	UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOVZXD,
		reg, UAX_REG_RCCTX, offsetof(BJX2_Context, regs)+(idx*8));
	return(0);
}

int BJX2_JitInsnVMRegReg(UAX_Context *jctx, int op, int idx, int reg)
{
	int reg1;

	if(idx!=(idx&127))
		{ JX2_DBGBREAK }

	reg1=BJX2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnRegReg(jctx, op, reg1, reg);
		return(0);
	}

	UAX_AsmInsnStRegDispReg(jctx, op,
		UAX_REG_RCCTX, offsetof(BJX2_Context, regs)+(idx*8), reg);
	return(0);
}

int BJX2_JitInsnVMRegLdReg(UAX_Context *jctx, int op, int idx, int reg)
{
	int reg1;

	if(idx!=(idx&127))
		{ JX2_DBGBREAK }

	reg1=BJX2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnRegReg(jctx, op, reg1, reg);
		return(0);
	}

	UAX_AsmInsnStRegDispReg(jctx, op,
		UAX_REG_RCCTX, offsetof(BJX2_Context, regs)+(idx*8), reg);
	return(0);
}

int BJX2_JitInsnVMRegImm(UAX_Context *jctx, int op, int idx, s32 val)
{
	int reg1;

	if(idx!=(idx&127))
		{ JX2_DBGBREAK }

	reg1=BJX2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnRegImm(jctx, op, reg1, val);
		return(0);
	}

//	if(UAX_Asm_RegIsQWordP(reg))
//	if(jctx->isjq)
	if(1)
	{
		BJX2_JitLoadVMReg(jctx, idx, UAX_REG_RCX);
		UAX_AsmInsnRegImm(jctx, op, UAX_REG_RCX, val);
		BJX2_JitStoreVMReg(jctx, idx, UAX_REG_RCX);
	}else
	{
		UAX_AsmInsnStRegDispImm32(jctx, op,
			UAX_REG_RCCTX, offsetof(BJX2_Context, regs)+(idx*8), val);
	}
	return(0);
}

int BJX2_JitInsnVMRegLdImm(UAX_Context *jctx, int op, int idx, s32 val)
{
	int reg1;

	if(idx!=(idx&127))
		{ JX2_DBGBREAK }

	reg1=BJX2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnRegImm(jctx, op, reg1, val);
		return(0);
	}

//	if(UAX_Asm_RegIsQWordP(reg))
//	if(jctx->isjq)
	if(1)
	{
		BJX2_JitLoadVMReg(jctx, idx, UAX_REG_RCX);
		UAX_AsmInsnRegImm(jctx, op, UAX_REG_RCX, val);
	}else
	{
		UAX_AsmInsnStRegDispImm32(jctx, op,
			UAX_REG_RCCTX, offsetof(BJX2_Context, regs)+(idx*8), val);
	}
	return(0);
}

int BJX2_JitInsnVMReg(UAX_Context *jctx, int op, int idx)
{
	int reg1;

	if(idx!=(idx&127))
		{ JX2_DBGBREAK }

	reg1=BJX2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnReg(jctx, op, reg1);
		return(0);
	}

	UAX_AsmInsnStRegDisp32(jctx, op,
		UAX_REG_RCCTX, offsetof(BJX2_Context, regs)+(idx*8));
	return(0);
}

int BJX2_JitInsnRegVMReg(UAX_Context *jctx, int op, int reg, int idx)
{
	int reg1;

	if(idx!=(idx&127))
		{ JX2_DBGBREAK }

	reg1=BJX2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnRegReg(jctx, op, reg, reg1);
		return(0);
	}

	UAX_AsmInsnRegLdRegDisp(jctx, op,
		reg, UAX_REG_RCCTX, offsetof(BJX2_Context, regs)+(idx*8));
	return(0);
}

int BJX2_JitInsnLdRegVMReg(UAX_Context *jctx, int op, int reg, int idx)
{
	int reg1;

	if(idx!=(idx&127))
		{ JX2_DBGBREAK }

	reg1=BJX2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnRegReg(jctx, op, reg, reg1);
		return(0);
	}

	UAX_AsmInsnRegLdRegDisp(jctx, op,
		reg, UAX_REG_RCCTX, offsetof(BJX2_Context, regs)+(idx*8));
	return(0);
}


#if 1
int BJX2_JitAddVMReg(UAX_Context *jctx, int idx, int reg)
	{ return(BJX2_JitInsnVMRegReg(jctx, UAX_OP_ADD, idx, reg)); }
int BJX2_JitSubVMReg(UAX_Context *jctx, int idx, int reg)
	{ return(BJX2_JitInsnVMRegReg(jctx, UAX_OP_SUB, idx, reg)); }
int BJX2_JitAndVMReg(UAX_Context *jctx, int idx, int reg)
	{ return(BJX2_JitInsnVMRegReg(jctx, UAX_OP_AND, idx, reg)); }
int BJX2_JitOrVMReg(UAX_Context *jctx, int idx, int reg)
	{ return(BJX2_JitInsnVMRegReg(jctx, UAX_OP_OR, idx, reg)); }
int BJX2_JitXorVMReg(UAX_Context *jctx, int idx, int reg)
	{ return(BJX2_JitInsnVMRegReg(jctx, UAX_OP_XOR, idx, reg)); }
int BJX2_JitCmpVMReg(UAX_Context *jctx, int idx, int reg)
	{ return(BJX2_JitInsnVMRegLdReg(jctx, UAX_OP_CMP, idx, reg)); }
int BJX2_JitTestVMReg(UAX_Context *jctx, int idx, int reg)
	{ return(BJX2_JitInsnVMRegLdReg(jctx, UAX_OP_TEST, idx, reg)); }

int BJX2_JitAddVMRegImm(UAX_Context *jctx, int idx, s32 val)
	{ return(BJX2_JitInsnVMRegImm(jctx, UAX_OP_ADD, idx, val)); }
int BJX2_JitAndVMRegImm(UAX_Context *jctx, int idx, s32 val)
	{ return(BJX2_JitInsnVMRegImm(jctx, UAX_OP_AND, idx, val)); }
int BJX2_JitOrVMRegImm(UAX_Context *jctx, int idx, s32 val)
	{ return(BJX2_JitInsnVMRegImm(jctx, UAX_OP_OR, idx, val)); }
int BJX2_JitXorVMRegImm(UAX_Context *jctx, int idx, s32 val)
	{ return(BJX2_JitInsnVMRegImm(jctx, UAX_OP_XOR, idx, val)); }
int BJX2_JitShlVMRegImm(UAX_Context *jctx, int idx, s32 val)
	{ return(BJX2_JitInsnVMRegImm(jctx, UAX_OP_SHL, idx, val)); }
int BJX2_JitShrVMRegImm(UAX_Context *jctx, int idx, s32 val)
	{ return(BJX2_JitInsnVMRegImm(jctx, UAX_OP_SHR, idx, val)); }
int BJX2_JitSarVMRegImm(UAX_Context *jctx, int idx, s32 val)
	{ return(BJX2_JitInsnVMRegImm(jctx, UAX_OP_SAR, idx, val)); }

int BJX2_JitCmpVMRegImm(UAX_Context *jctx, int idx, s32 val)
	{ return(BJX2_JitInsnVMRegImm(jctx, UAX_OP_CMP, idx, val)); }
int BJX2_JitTestVMRegImm(UAX_Context *jctx, int idx, s32 val)
	{ return(BJX2_JitInsnVMRegLdImm(jctx, UAX_OP_TEST, idx, val)); }
#endif

int BJX2_JitMovVMRegVMReg(UAX_Context *jctx, int didx, int sidx)
{
	int reg1, reg2;

	if(didx!=(didx&127))
		{ JX2_DBGBREAK }
	if(sidx!=(sidx&127))
		{ JX2_DBGBREAK }

	reg1=BJX2_JitLoadReadSyncVMReg(jctx, sidx);
	reg2=BJX2_JitLoadWriteSyncVMReg(jctx, didx);
	if(reg1!=UAX_REG_Z)
	{
		if(reg2!=UAX_REG_Z)
		{
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, reg2, reg1);
			return(1);
		}

		BJX2_JitStoreVMReg(jctx, didx, reg1);
		return(1);
	}

	if(reg2!=UAX_REG_Z)
	{
		BJX2_JitLoadVMReg(jctx, sidx, reg2);
		return(1);
	}

//	if(jctx->isjq)
	if(1)
	{
#if 1
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RCX, UAX_REG_RCCTX,
			offsetof(BJX2_Context, regs)+(sidx*8));
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BJX2_Context, regs)+
				(didx*8), UAX_REG_RCX);
#endif
	}else
	{
		BJX2_JitLoadVMReg(jctx, sidx, UAX_REG_EAX);
		BJX2_JitStoreVMReg(jctx, didx, UAX_REG_EAX);
	}
	return(1);
}

int BJX2_JitInsnVMRegVMReg(UAX_Context *jctx, int op, int didx, int sidx)
{
	int dreg, sreg;

	if(didx!=(didx&127))
		{ JX2_DBGBREAK }
	if(sidx!=(sidx&127))
		{ JX2_DBGBREAK }

	if(sidx==didx)
	{
		dreg=BJX2_JitLoadReadSyncVMReg(jctx, didx);
		if(dreg!=UAX_REG_Z)
		{
			UAX_AsmInsnRegReg(jctx, op, dreg, dreg);
			return(1);
		}

		BJX2_JitLoadVMReg(jctx, sidx, UAX_REG_RDX);
		UAX_AsmInsnRegReg(jctx, op, UAX_REG_RDX, UAX_REG_RDX);
		BJX2_JitStoreVMReg(jctx, didx, UAX_REG_RDX);
//		BJX2_JitInsnVMRegReg(jctx, op, didx, UAX_REG_EAX);
		return(1);
	}

	dreg=BJX2_JitLoadReadSyncVMReg(jctx, didx);
	sreg=BJX2_JitLoadReadSyncVMReg(jctx, sidx);
	if(sreg!=UAX_REG_Z)
	{
		if(dreg!=UAX_REG_Z)
		{
			UAX_AsmInsnRegReg(jctx, op, dreg, sreg);
			return(1);
		}

		BJX2_JitInsnVMRegReg(jctx, op, didx, sreg);
		return(1);
	}

	if(dreg!=UAX_REG_Z)
	{
		BJX2_JitInsnRegVMReg(jctx, op, dreg, sidx);
		return(1);
	}

//	if(jctx->isjq)
	if(1)
	{
		BJX2_JitLoadVMReg(jctx, sidx, UAX_REG_RDX);
		BJX2_JitInsnVMRegReg(jctx, op, didx, UAX_REG_RDX);
	}else
	{
		BJX2_JitLoadVMReg(jctx, sidx, UAX_REG_EAX);
		BJX2_JitInsnVMRegReg(jctx, op, didx, UAX_REG_EAX);
	}
	return(1);
}


int BJX2_JitLoadVMFReg(UAX_Context *jctx, int idx, int reg)
{
	if(idx!=(idx&127))
		{ JX2_DBGBREAK }

	if(UAX_Asm_RegIsXmmP(reg))
	{
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOVSD,
			reg, UAX_REG_RCCTX, offsetof(BJX2_Context, fpreg)+(idx*8));
	}else
	{
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			reg, UAX_REG_RCCTX, offsetof(BJX2_Context, fpreg)+(idx*8));
	}
	return(0);
}

int BJX2_JitStoreVMFReg(UAX_Context *jctx, int idx, int reg)
{
	if(idx!=(idx&127))
		{ JX2_DBGBREAK }

	if(UAX_Asm_RegIsXmmP(reg))
	{
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOVSD,
			UAX_REG_RCCTX, offsetof(BJX2_Context, fpreg)+(idx*8), reg);
	}else
	{
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BJX2_Context, fpreg)+(idx*8), reg);
	}
	return(0);
}


int BJX2_JitLoadVMDReg(UAX_Context *jctx, int idx, int reg)
{
	if(idx!=(idx&127))
		{ JX2_DBGBREAK }

	if(idx&(~15))
		{ JX2_DBGBREAK }

	if(UAX_Asm_RegIsXmmP(reg))
	{
#if 0
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RAX, UAX_REG_RCCTX,
			offsetof(BJX2_Context, fpreg)+(idx*8));
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVQ, reg, UAX_REG_RAX);
#endif

#if 1
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOVSD,
//		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOVQ,
			reg, UAX_REG_RCCTX,
			offsetof(BJX2_Context, fpreg)+(idx*8));
#endif
	}else
	{
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			reg, UAX_REG_RCCTX, offsetof(BJX2_Context, fpreg)+(idx*8));
//		UAX_AsmInsnRegImm(jctx, UAX_OP_ROL, reg, 32);
	}
	return(0);
}

int BJX2_JitStoreVMDReg(UAX_Context *jctx, int idx, int reg)
{
	if(idx!=(idx&127))
		{ JX2_DBGBREAK }

	if(idx&(~15))
		{ JX2_DBGBREAK }

	if(UAX_Asm_RegIsXmmP(reg))
	{
#if 0
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVQ, UAX_REG_RAX, reg);
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BJX2_Context, fpreg)+(idx*8),
			UAX_REG_RAX);
#endif

#if 1
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOVSD,
//		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOVQ,
			UAX_REG_RCCTX, offsetof(BJX2_Context, fpreg)+(idx*8),
			reg);
#endif
	}else
	{
//		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVQ, UAX_REG_RAX, reg);
//		UAX_AsmInsnRegImm(jctx, UAX_OP_ROL, UAX_REG_RAX, 32);
//		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
//			UAX_REG_RCCTX, offsetof(BJX2_Context, fregs)+(idx*4),
//			UAX_REG_RAX);

		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BJX2_Context, fpreg)+(idx*8),
			reg);
	}
	return(0);
}

int BJX2_JitFlushJNext(UAX_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *tr)
{
	UAX_AsmXorRegReg(jctx, UAX_REG_R8Q, UAX_REG_R8Q);
	UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
		UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt), UAX_REG_R8Q);
	return(0);
}

int BJX2_JitEmitCallFPtr(
	UAX_Context *jctx, BJX2_Context *cpu,
	void *addr)
{
	int i;

	if(UAX_CheckAddrExHeapRel32(addr))
	{
		i=UAX_LookupGlobalAddress(addr);
		if(i<=0)
		{
			i=UAX_RegisterGlobal(NULL, addr);
		}
	}else
	{
		i=0;
	}

#ifdef UAX_SYSVAMD64
	UAX_AsmMovRegReg(jctx, UAX_REG_RDI, UAX_REG_RCX);
	UAX_AsmMovRegReg(jctx, UAX_REG_RSI, UAX_REG_RDX);
	UAX_AsmMovRegReg(jctx, UAX_REG_RDX, UAX_REG_R8Q);
	UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_R9Q);
#endif

	if(i>0)
	{
		UAX_AsmInsnLabel(jctx, UAX_OP_CALL, i);
	}else
	{
		UAX_AsmMovRegImm(jctx, UAX_REG_RAX, (nlint)addr);
		UAX_AsmInsnReg(jctx, UAX_OP_CALL, UAX_REG_RAX);
	}
	return(0);
}

int BJX2_JitEmitCallFPtrReg(
	UAX_Context *jctx, BJX2_Context *cpu, int reg)
{
#ifdef UAX_SYSVAMD64
	UAX_AsmMovRegReg(jctx, UAX_REG_RDI, UAX_REG_RCX);
	UAX_AsmMovRegReg(jctx, UAX_REG_RSI, UAX_REG_RDX);
	UAX_AsmMovRegReg(jctx, UAX_REG_RDX, UAX_REG_R8Q);
	UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_R9Q);
#endif

	UAX_AsmInsnReg(jctx, UAX_OP_CALL, reg);
	return(0);
}

/* EDX -> EAX */
int BJX2_JitGetAddrDWord(UAX_Context *jctx, BJX2_Context *cpu)
{
	int l0, l1, l2;
	int i;

#if 0
//	if(cpu->GetAddrDWord==BJX2_GetAddrDWordFMMU_NoAT)
	if(1)
	{
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);

//		UAX_AsmMovRegReg(jctx, UAX_REG_ECX, UAX_REG_EDX);
		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDX);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_SUB,
			UAX_REG_RCX, UAX_REG_RCCTX, offsetof(BJX2_Context, psp_pbase));
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_CMP,
			UAX_REG_RCX, UAX_REG_RCCTX, offsetof(BJX2_Context, psp_prng3));

		UAX_AsmInsnLabel(jctx, UAX_OP_JA, l0);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RDX, UAX_REG_RCCTX, offsetof(BJX2_Context, psp_pdata));

		UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_MOV, UAX_REG_EAX,
			UAX_REG_RDX, 1, UAX_REG_RCX, 0);

//		UAX_AsmInsnNone(jctx, UAX_OP_RET);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l1);

		UAX_EmitLabel(jctx, l0);

		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RCCTX);
//		BJX2_JitEmitCallFPtr(jctx, cpu, cpu->GetAddrDWord);
//		BJX2_JitEmitCallFPtr(jctx, cpu, BJX2_MemGetDWord);
//		BJX2_JitEmitCallFPtr(jctx, cpu, cpu->MemGetDWord);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RAX, UAX_REG_RCCTX, offsetof(BJX2_Context, MemGetDWord));
		BJX2_JitEmitCallFPtrReg(jctx, cpu, UAX_REG_RAX);

		UAX_EmitLabel(jctx, l1);
		return(0);
	}
#endif

	UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RCCTX);

//	UAX_AsmMovRegImm(jctx, UAX_REG_RAX, (nlint)(cpu->GetAddrDWord));
//	UAX_AsmInsnReg(jctx, UAX_OP_CALL, UAX_REG_RAX);

//	BJX2_JitEmitCallFPtr(jctx, cpu, cpu->GetAddrDWord);
	BJX2_JitEmitCallFPtr(jctx, cpu, BJX2_MemGetDWord);
	return(0);
}

int BJX2_JitGetAddrWord(UAX_Context *jctx, BJX2_Context *cpu)
{
	int l0, l1, l2;
	int i;

#if 0
//	if(cpu->GetAddrDWord==BJX2_GetAddrDWordFMMU_NoAT)
	if(1)
	{
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);

		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDX);
//		UAX_AsmAndRegImm(jctx, UAX_REG_RCX, 0x1FFFFFFF);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_SUB,
			UAX_REG_RCX, UAX_REG_RCCTX, offsetof(BJX2_Context, psp_pbase));
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_CMP,
			UAX_REG_RCX, UAX_REG_RCCTX, offsetof(BJX2_Context, psp_prng3));

		UAX_AsmInsnLabel(jctx, UAX_OP_JA, l0);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RDX, UAX_REG_RCCTX, offsetof(BJX2_Context, psp_pdata));

		UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_MOVSXW, UAX_REG_EAX,
			UAX_REG_RDX, 1, UAX_REG_RCX, 0);

//		UAX_AsmInsnNone(jctx, UAX_OP_RET);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l1);

		UAX_EmitLabel(jctx, l0);

		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RCCTX);
//		BJX2_JitEmitCallFPtr(jctx, cpu, cpu->GetAddrWord);
//		BJX2_JitEmitCallFPtr(jctx, cpu, BJX2_MemGetWord);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RAX, UAX_REG_RCCTX, offsetof(BJX2_Context, MemGetWord));
		BJX2_JitEmitCallFPtrReg(jctx, cpu, UAX_REG_RAX);

		UAX_EmitLabel(jctx, l1);
		return(0);
	}
#endif

	UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RCCTX);

//	UAX_AsmMovRegImm(jctx, UAX_REG_RAX, (nlint)(cpu->GetAddrWord));
//	UAX_AsmInsnReg(jctx, UAX_OP_CALL, UAX_REG_RAX);
//	BJX2_JitEmitCallFPtr(jctx, cpu, cpu->GetAddrWord);

	BJX2_JitEmitCallFPtr(jctx, cpu, BJX2_MemGetWord);

	return(0);
}

int BJX2_JitGetAddrByte(UAX_Context *jctx, BJX2_Context *cpu)
{
	int l0, l1, l2;
	int i;

#if 0
//	if(cpu->GetAddrDWord==BJX2_GetAddrDWordFMMU_NoAT)
	if(1)
	{
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);

		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDX);
//		UAX_AsmAndRegImm(jctx, UAX_REG_RCX, 0x1FFFFFFF);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_SUB,
			UAX_REG_RCX, UAX_REG_RCCTX, offsetof(BJX2_Context, psp_pbase));
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_CMP,
			UAX_REG_RCX, UAX_REG_RCCTX, offsetof(BJX2_Context, psp_prng3));

		UAX_AsmInsnLabel(jctx, UAX_OP_JA, l0);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RDX, UAX_REG_RCCTX, offsetof(BJX2_Context, psp_pdata));

		UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_MOVSXB, UAX_REG_EAX,
			UAX_REG_RDX, 1, UAX_REG_RCX, 0);

//		UAX_AsmInsnNone(jctx, UAX_OP_RET);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l1);

		UAX_EmitLabel(jctx, l0);

		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RCCTX);
//		BJX2_JitEmitCallFPtr(jctx, cpu, cpu->GetAddrByte);
//		BJX2_JitEmitCallFPtr(jctx, cpu, BJX2_MemGetByte);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RAX, UAX_REG_RCCTX, offsetof(BJX2_Context, MemGetByte));
		BJX2_JitEmitCallFPtrReg(jctx, cpu, UAX_REG_RAX);

		UAX_EmitLabel(jctx, l1);
		return(0);
	}
#endif

	UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RCCTX);

//	UAX_AsmMovRegImm(jctx, UAX_REG_RAX, (nlint)(cpu->GetAddrByte));
//	UAX_AsmInsnReg(jctx, UAX_OP_CALL, UAX_REG_RAX);
//	BJX2_JitEmitCallFPtr(jctx, cpu, cpu->GetAddrByte);

	BJX2_JitEmitCallFPtr(jctx, cpu, BJX2_MemGetByte);

	return(0);
}

int BJX2_JitGetAddrQWord(UAX_Context *jctx, BJX2_Context *cpu)
{
	int l0, l1, l2;
	int i;

#if 0
//	if(cpu->GetAddrDWord==BJX2_GetAddrDWordFMMU_NoAT)
	if(1)
	{
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);

		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDX);
//		UAX_AsmAndRegImm(jctx, UAX_REG_RCX, 0x1FFFFFFF);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_SUB,
			UAX_REG_RCX, UAX_REG_RCCTX, offsetof(BJX2_Context, psp_pbase));
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_CMP,
			UAX_REG_RCX, UAX_REG_RCCTX, offsetof(BJX2_Context, psp_prng3));

		UAX_AsmInsnLabel(jctx, UAX_OP_JA, l0);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RDX, UAX_REG_RCCTX, offsetof(BJX2_Context, psp_pdata));

		UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_MOV, UAX_REG_RAX,
			UAX_REG_RDX, 1, UAX_REG_RCX, 0);

//		UAX_AsmInsnNone(jctx, UAX_OP_RET);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l1);

		UAX_EmitLabel(jctx, l0);

		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RCCTX);
//		BJX2_JitEmitCallFPtr(jctx, cpu, cpu->GetAddrQWord);
//		BJX2_JitEmitCallFPtr(jctx, cpu, BJX2_MemGetQWord);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RAX, UAX_REG_RCCTX, offsetof(BJX2_Context, MemGetQWord));
		BJX2_JitEmitCallFPtrReg(jctx, cpu, UAX_REG_RAX);

		UAX_EmitLabel(jctx, l1);
		return(0);
	}
#endif

	UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RCCTX);

//	UAX_AsmMovRegImm(jctx, UAX_REG_RAX, (nlint)(cpu->GetAddrDWord));
//	UAX_AsmInsnReg(jctx, UAX_OP_CALL, UAX_REG_RAX);

//	BJX2_JitEmitCallFPtr(jctx, cpu, cpu->GetAddrQWord);

	BJX2_JitEmitCallFPtr(jctx, cpu, BJX2_MemGetQWord);

	return(0);
}

int BJX2_JitSetAddrQWord(UAX_Context *jctx, BJX2_Context *cpu)
{
	int l0, l1, l2;
	int i;

#if 0
//	if(cpu->GetAddrDWord==BJX2_GetAddrDWordFMMU_NoAT)
	if(1)
	{
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);

		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDX);
//		UAX_AsmAndRegImm(jctx, UAX_REG_RCX, 0x1FFFFFFF);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_SUB,
			UAX_REG_RCX, UAX_REG_RCCTX, offsetof(BJX2_Context, psp_pbase));
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_CMP,
			UAX_REG_RCX, UAX_REG_RCCTX, offsetof(BJX2_Context, psp_prng3));

		UAX_AsmInsnLabel(jctx, UAX_OP_JA, l0);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RDX, UAX_REG_RCCTX, offsetof(BJX2_Context, psp_pdata));

		UAX_AsmInsnStRegIxDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RDX, 1, UAX_REG_RCX, 0, UAX_REG_R8Q);

		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l1);

		UAX_EmitLabel(jctx, l0);

		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RCCTX);
//		BJX2_JitEmitCallFPtr(jctx, cpu, cpu->SetAddrQWord);
//		BJX2_JitEmitCallFPtr(jctx, cpu, BJX2_MemSetQWord);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RAX, UAX_REG_RCCTX, offsetof(BJX2_Context, MemSetQWord));
		BJX2_JitEmitCallFPtrReg(jctx, cpu, UAX_REG_RAX);

		UAX_EmitLabel(jctx, l1);
		return(0);
	}
#endif

	UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RCCTX);

//	UAX_AsmMovRegImm(jctx, UAX_REG_RAX, (nlint)(cpu->SetAddrDWord));
//	UAX_AsmInsnReg(jctx, UAX_OP_CALL, UAX_REG_RAX);
//	BJX2_JitEmitCallFPtr(jctx, cpu, cpu->SetAddrQWord);

	BJX2_JitEmitCallFPtr(jctx, cpu, BJX2_MemSetQWord);

	return(0);
}

/* EDX,R8 -> */
int BJX2_JitSetAddrDWord(UAX_Context *jctx, BJX2_Context *cpu)
{
	int l0, l1, l2;
	int i;

#if 0
//	if(cpu->GetAddrDWord==BJX2_GetAddrDWordFMMU_NoAT)
	if(1)
	{
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);

		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDX);
//		UAX_AsmAndRegImm(jctx, UAX_REG_RCX, 0x1FFFFFFF);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_SUB,
			UAX_REG_RCX, UAX_REG_RCCTX, offsetof(BJX2_Context, psp_pbase));
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_CMP,
			UAX_REG_RCX, UAX_REG_RCCTX, offsetof(BJX2_Context, psp_prng3));

		UAX_AsmInsnLabel(jctx, UAX_OP_JA, l0);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RDX, UAX_REG_RCCTX, offsetof(BJX2_Context, psp_pdata));

		UAX_AsmInsnStRegIxDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RDX, 1, UAX_REG_RCX, 0, UAX_REG_R8D);

		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l1);

		UAX_EmitLabel(jctx, l0);

		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RCCTX);
//		BJX2_JitEmitCallFPtr(jctx, cpu, cpu->SetAddrDWord);
//		BJX2_JitEmitCallFPtr(jctx, cpu, BJX2_MemSetDWord);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RAX, UAX_REG_RCCTX, offsetof(BJX2_Context, MemSetDWord));
		BJX2_JitEmitCallFPtrReg(jctx, cpu, UAX_REG_RAX);

		UAX_EmitLabel(jctx, l1);
		return(0);
	}
#endif

	UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RCCTX);

//	UAX_AsmMovRegImm(jctx, UAX_REG_RAX, (nlint)(cpu->SetAddrDWord));
//	UAX_AsmInsnReg(jctx, UAX_OP_CALL, UAX_REG_RAX);
//	BJX2_JitEmitCallFPtr(jctx, cpu, cpu->SetAddrDWord);

	BJX2_JitEmitCallFPtr(jctx, cpu, BJX2_MemSetDWord);

	return(0);
}

int BJX2_JitSetAddrWord(UAX_Context *jctx, BJX2_Context *cpu)
{
	int l0, l1, l2;
	int i;

#if 0
//	if(cpu->GetAddrDWord==BJX2_GetAddrDWordFMMU_NoAT)
	if(1)
	{
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);

		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDX);
//		UAX_AsmAndRegImm(jctx, UAX_REG_RCX, 0x1FFFFFFF);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_SUB,
			UAX_REG_RCX, UAX_REG_RCCTX, offsetof(BJX2_Context, psp_pbase));
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_CMP,
			UAX_REG_RCX, UAX_REG_RCCTX, offsetof(BJX2_Context, psp_prng3));

		UAX_AsmInsnLabel(jctx, UAX_OP_JA, l0);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RDX, UAX_REG_RCCTX, offsetof(BJX2_Context, psp_pdata));

		UAX_AsmInsnStRegIxDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RDX, 1, UAX_REG_RCX, 0, UAX_REG_R8W);

		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l1);

		UAX_EmitLabel(jctx, l0);

		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RCCTX);
//		BJX2_JitEmitCallFPtr(jctx, cpu, cpu->SetAddrWord);
//		BJX2_JitEmitCallFPtr(jctx, cpu, BJX2_MemSetWord);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RAX, UAX_REG_RCCTX, offsetof(BJX2_Context, MemSetWord));
		BJX2_JitEmitCallFPtrReg(jctx, cpu, UAX_REG_RAX);

		UAX_EmitLabel(jctx, l1);
		return(0);
	}
#endif

	UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RCCTX);

//	UAX_AsmMovRegImm(jctx, UAX_REG_RAX, (nlint)(cpu->SetAddrWord));
//	UAX_AsmInsnReg(jctx, UAX_OP_CALL, UAX_REG_RAX);
//	BJX2_JitEmitCallFPtr(jctx, cpu, cpu->SetAddrWord);

	BJX2_JitEmitCallFPtr(jctx, cpu, BJX2_MemSetWord);

	return(0);
}

int BJX2_JitSetAddrByte(UAX_Context *jctx, BJX2_Context *cpu)
{
	int l0, l1, l2;
	int i;

#if 0
//	if(cpu->GetAddrDWord==BJX2_GetAddrDWordFMMU_NoAT)
	if(1)
	{
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);

		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDX);
//		UAX_AsmAndRegImm(jctx, UAX_REG_RCX, 0x1FFFFFFF);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_SUB,
			UAX_REG_RCX, UAX_REG_RCCTX, offsetof(BJX2_Context, psp_pbase));
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_CMP,
			UAX_REG_RCX, UAX_REG_RCCTX, offsetof(BJX2_Context, psp_prng3));

		UAX_AsmInsnLabel(jctx, UAX_OP_JA, l0);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RDX, UAX_REG_RCCTX, offsetof(BJX2_Context, psp_pdata));

		UAX_AsmInsnStRegIxDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RDX, 1, UAX_REG_RCX, 0, UAX_REG_R8B);

		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l1);

		UAX_EmitLabel(jctx, l0);

		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RCCTX);
//		BJX2_JitEmitCallFPtr(jctx, cpu, cpu->SetAddrByte);
//		BJX2_JitEmitCallFPtr(jctx, cpu, BJX2_MemSetByte);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RAX, UAX_REG_RCCTX, offsetof(BJX2_Context, MemSetByte));
		BJX2_JitEmitCallFPtrReg(jctx, cpu, UAX_REG_RAX);

		UAX_EmitLabel(jctx, l1);
		return(0);
	}
#endif

	UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RCCTX);

//	UAX_AsmMovRegImm(jctx, UAX_REG_RAX, (nlint)(cpu->SetAddrByte));
//	UAX_AsmInsnReg(jctx, UAX_OP_CALL, UAX_REG_RAX);
//	BJX2_JitEmitCallFPtr(jctx, cpu, cpu->SetAddrByte);

	BJX2_JitEmitCallFPtr(jctx, cpu, BJX2_MemSetByte);

	return(0);
}

int BJX2_JitSetupOpTrap(UAX_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *tr, BJX2_Opcode *op)
{
//	BJX2_JitStoreVMRegImm(jctx, op->rn, (s32)(op->imm));

	UAX_AsmMovRegImm(jctx, UAX_REG_RAX, op->pc);
	UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
		UAX_REG_RCCTX, offsetof(BJX2_Context, trapc), UAX_REG_RAX);
	return(0);
}

int BJX2_TryJitOpcode(UAX_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *tr, BJX2_Opcode *op)
{
//	return(0);

	if(BJX2_TryJitOpcode_MovReg(jctx, cpu, tr, op)>0)
		return(1);

//	return(0);

	if(BJX2_TryJitOpcode_ArithReg(jctx, cpu, tr, op)>0)
		return(1);

//	return(0);

	if(BJX2_TryJitOpcode_SignExtOp(jctx, cpu, tr, op)>0)
		return(1);

//	return(0);

	if(BJX2_TryJitOpcode_MovMem(jctx, cpu, tr, op)>0)
		return(1);

	return(0);

	if(BJX2_TryJitOpcode_CmpOp(jctx, cpu, tr, op)>0)
		return(1);
	if(BJX2_TryJitOpcode_BranchOp(jctx, cpu, tr, op)>0)
		return(1);

	if(BJX2_TryJitOpcode_PredX(jctx, cpu, tr, op)>0)
		return(1);

#if 0
	if(BJX2_TryJitOpcode_MovReg(jctx, cpu, tr, op)>0)
		return(1);
	if(BJX2_TryJitOpcode_ArithReg(jctx, cpu, tr, op)>0)
		return(1);
	if(BJX2_TryJitOpcode_MovMem(jctx, cpu, tr, op)>0)
		return(1);

//	return(0);

#if 1
	if(BJX2_TryJitOpcode_FpuOp(jctx, cpu, tr, op)>0)
		return(1);
	if(BJX2_TryJitOpcode_BranchOp(jctx, cpu, tr, op)>0)
		return(1);
#endif

//	return(0);

	if(BJX2_TryJitOpcode_CmpOp(jctx, cpu, tr, op)>0)
		return(1);
	if(BJX2_TryJitOpcode_SignExtOp(jctx, cpu, tr, op)>0)
		return(1);
#endif

	return(0);
}

int BJX2_JitSyncRegs(UAX_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *trj)
{
	int idx, reg;
	int i, j, k;
	
	if(!jctx->reg_live)
		return(0);
	
	for(i=0; i<16; i++)
		if(jctx->reg_live&(1<<i))
	{
		idx=jctx->reg_idx[i];
		reg=jctx->reg_reg[i];
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BJX2_Context, regs)+(idx*8), reg);
		jctx->reg_live&=~(1<<i);
	}
	return(0);
}

int BJX2_JitSyncSaveReg(UAX_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *trj,
	int reg)
{
	if(reg==UAX_REG_Z)
		return(0);

	if((reg&15)==3)
	{	BJX2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_RBX, UAX_REG_RBX);
		jctx->reg_save|=0x0008;	}
	if((reg&15)==5)
	{	BJX2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_RBP, UAX_REG_RBP);
		jctx->reg_save|=0x0020;	}
#ifdef UAX_WINX64
	if((reg&15)==6)
	{	BJX2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_RSI, UAX_REG_RSI);
		jctx->reg_save|=0x0040;	}
	if((reg&15)==7)
	{	BJX2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_RDI, UAX_REG_RDI);
		jctx->reg_save|=0x0080;	}
#endif

	if((reg&15)==12)
	{	BJX2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_R12, UAX_REG_R12Q);
		jctx->reg_save|=0x1000;	}
	if((reg&15)==13)
	{	BJX2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_R13, UAX_REG_R13Q);
		jctx->reg_save|=0x2000;	}
	if((reg&15)==14)
	{	BJX2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_R14, UAX_REG_R14Q);
		jctx->reg_save|=0x4000;	}
	if((reg&15)==15)
	{	BJX2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_R15, UAX_REG_R15Q);
		jctx->reg_save|=0x8000;	}
	return(0);
}

int BJX2_JitSyncRestoreRegs(UAX_Context *jctx,
	BJX2_Context *cpu, BJX2_Trace *trj)
{
	if(jctx->reg_save&0x0008)
		BJX2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_RBX, UAX_REG_RBX);
	if(jctx->reg_save&0x0020)
		BJX2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_RBP, UAX_REG_RBP);

#ifdef UAX_WINX64
	if(jctx->reg_save&0x0040)
		BJX2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_RSI, UAX_REG_RSI);
	if(jctx->reg_save&0x0080)
		BJX2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_RDI, UAX_REG_RDI);
#endif

	if(jctx->reg_save&0x1000)
		BJX2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_R12, UAX_REG_R12Q);
	if(jctx->reg_save&0x2000)
		BJX2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_R13, UAX_REG_R13Q);
	if(jctx->reg_save&0x4000)
		BJX2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_R14, UAX_REG_R14Q);
	if(jctx->reg_save&0x8000)
		BJX2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_R15, UAX_REG_R15Q);
	return(0);
}

int btesh2_nmid_jitmiss[256*64];

int BJX2_TryJitTrace(BJX2_Context *cpu, BJX2_Trace *tr)
{
	byte uax_cachereg[6]={
		UAX_REG_R12D, UAX_REG_R13D, UAX_REG_R14D,
		UAX_REG_R15D,  UAX_REG_EBX,  UAX_REG_ESI};
	byte uax_cacheregq[6]={
		UAX_REG_R12Q, UAX_REG_R13Q, UAX_REG_R14Q,
		UAX_REG_R15Q,  UAX_REG_RBX,  UAX_REG_RSI};

	byte regidx[128];
	byte regcnt[128];
	BJX2_Opcode *op_prt[64];
	BJX2_Opcode *op_prf[64];
	UAX_Context *jctx;
	BJX2_Trace *trj;
	BJX2_Opcode *op;
	byte nolink;
	byte *ptr;
	int n_prt, n_prf;
	int l0, l1, l2;
	int i0, i1, i2;
	int i, j, k, sz;

	if(uax_fault)
		return(0);

	if(tr->n_ops<=0)
		return(0);

//	if(cpu->archfl&BJX2_ARFL_NOJIT)
//		return(0);

#ifdef UAX_SYSVAMD64
//	return(0);
#endif

	nolink=0;
//	if(cpu->arch==BJX2_ARCH_SH2)
//		nolink=1;
//	if(BJX2_CpuNolink(cpu))
//		nolink=1;

#ifdef UAX_SYSVAMD64
	nolink=1;
#endif

#if 1
	for(i=0; i<128; i++)
	{
		regcnt[i]=0;
		regidx[i]=i;
	}
#endif

#if 1
	for(i=0; i<tr->n_ops; i++)
	{
		op=tr->ops[i];
		
		if((op->nmid==BJX2_NMID_PRED_T) ||
			(op->nmid==BJX2_NMID_PRED_F))
		{
			regcnt[BJX2_REG_SR]++;
			op=op->data;
		}
		
		i0=op->rm;
		i1=op->rn;
		i2=op->ro;
		if((i0<0) || (i0>=BJX2_REG_ZZR))i0=BJX2_REG_ZZR;
		if((i1<0) || (i1>=BJX2_REG_ZZR))i1=BJX2_REG_ZZR;
		if((i2<0) || (i2>=BJX2_REG_ZZR))i2=BJX2_REG_ZZR;
		
		switch(op->fmid)
		{
		case BJX2_FMID_REGREG:
		case BJX2_FMID_REGSTREG:
		case BJX2_FMID_LDREGREG:
		case BJX2_FMID_REGSTREGDISP:
		case BJX2_FMID_LDREGDISPREG:
			regcnt[i0]++;	regcnt[i1]++;	break;
//		case BJX2_FMID_REGRM:
//		case BJX2_FMID_LDREG:
//			regcnt[i0]++;	break;
		case BJX2_FMID_REG:
		case BJX2_FMID_IMMREG:
			regcnt[i1]++;	break;
//		case BJX2_FMID_REGDECST:
//			regcnt[i0]++;	regcnt[i1]+=2;	break;
//		case BJX2_FMID_REGINCLD:
//			regcnt[i0]+=2;	regcnt[i1]++;	break;
//		case BJX2_FMID_REGSTR0N:
//		case BJX2_FMID_REGLDR0M:
		case BJX2_FMID_REGSTREG2:
		case BJX2_FMID_LDREG2REG:
			regcnt[i0]++;
			regcnt[i1]++;
			regcnt[i2]++;
			break;
		case BJX2_FMID_REGREGREG:
		case BJX2_FMID_REGDRREG:
			regcnt[i0]++;
			regcnt[i1]++;
			regcnt[i2]++;
			break;
//		case BJX2_FMID_ABS:
		case BJX2_FMID_NONE:
		case BJX2_FMID_Z:
		case BJX2_FMID_IMM:
		case BJX2_FMID_PCDISP:
			break;
		case BJX2_FMID_FREGREG:
			break;
		case BJX2_FMID_FREG:
			break;
		default:
			regcnt[i0]++;
			regcnt[i1]++;
			regcnt[i2]++;
			break;
		}

		switch(op->nmid)
		{
		case BJX2_NMID_BT:		case BJX2_NMID_BF:
//		case BJX2_NMID_BTS:		case BJX2_NMID_BFS:
		case BJX2_NMID_CLRT:		case BJX2_NMID_SETT:
		case BJX2_NMID_CLRS:		case BJX2_NMID_SETS:
			regcnt[BJX2_REG_SR]++;
			break;
		case BJX2_NMID_CMPHS:		case BJX2_NMID_CMPHI:
		case BJX2_NMID_CMPEQ:		case BJX2_NMID_CMPGE:
		case BJX2_NMID_CMPGT:		case BJX2_NMID_TST:
		case BJX2_NMID_CMPPL:		case BJX2_NMID_CMPPZ:
//		case BJX2_NMID_SHAL:		case BJX2_NMID_SHAR:
//		case BJX2_NMID_SHLL:		case BJX2_NMID_SHLR:
//			regcnt[BJX2_REG_SR]+=2;
			break;
		default:
			break;
		}
	}
	
	regcnt[BJX2_REG_ZZR]=0;

	for(i=0; i<128; i++)
		for(j=i+1; j<128; j++)
	{
		if(regcnt[j]>regcnt[i])
		{	k=regcnt[i]; regcnt[i]=regcnt[j]; regcnt[j]=k;
			k=regidx[i]; regidx[i]=regidx[j]; regidx[j]=k;	}
	}

	for(i=0; i<128; i++)
	{
		if(regcnt[i]<4)regcnt[i]=0;
	}
#endif

	
#if 0
	if(!tr->lnknext && !nolink)
	{
		trj=BJX2_GetTraceForAddr(cpu, tr->addr_nxt);
		if(trj)
		{
//			BJX2_JitLinkTrace(cpu, trj);
			tr->lnknext=trj;
		}
	}
#endif

#if 0
	if(!tr->jmpnext && tr->addr_jmp && !nolink)
	{
		trj=BJX2_GetTraceForAddr(cpu, tr->addr_jmp);
		if(trj)
		{
			if(tr->addr_jmp != trj->addr)
				{ JX2_DBGBREAK }
		
//			BJX2_JitLinkTrace(cpu, trj);
			tr->jmpnext=trj;
//			BJX2_ValidateTrace(cpu, trj);
		}
	}
#endif

	jctx=UAX_AllocContext();

	jctx->reg_resv=0;
	jctx->reg_live=0;
	jctx->reg_save=0;
	jctx->jitfl=0;
	jctx->isjq=0;
	
//	if(tr->csfl&BJX2_CSFL_SRJQ)
//		jctx->isjq=1;
	
	if(nolink)
	{
		jctx->jitfl|=BJX2_UAXJFL_NOPREJMP;
	}
	
#if 1
	for(i=0; i<UAX_MAX_CACHEVAR; i++)
	{
		if(regcnt[i]<=0)
		{
			jctx->reg_reg[i]=UAX_REG_Z;
			continue;
		}
		jctx->reg_idx[i]=regidx[i];
//		jctx->reg_reg[i]=UAX_REG_R12D+i;
//		if(tr->csfl&BJX2_CSFL_SRJQ)
			jctx->reg_reg[i]=uax_cacheregq[i];
//		else
//			jctx->reg_reg[i]=uax_cachereg[i];
		jctx->reg_resv|=1<<i;
	}
#endif

	
	UAX_SetSection(jctx, UAX_CSEG_TEXT);
	
	UAX_AsmInsnRegImm(jctx, UAX_OP_SUB, UAX_REG_RSP, BTEJ_X64_FRAMEADJ);
//	BJX2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_CPU, UAX_REG_RCX);
//	BJX2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_TRACE, UAX_REG_RDX);

	BJX2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_RCCTX, UAX_REG_RCCTX);
//	BJX2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_RSI, UAX_REG_RSI);

	for(i=0; i<UAX_MAX_CACHEVAR; i++)
	{
		if(!(jctx->reg_resv&(1<<i)))
			continue;
		BJX2_JitSyncSaveReg(jctx, cpu, tr,
			jctx->reg_reg[i]);

//		UAX_AsmXorRegReg(jctx, jctx->reg_reg[i], jctx->reg_reg[i]);
	}

#ifdef UAX_SYSVAMD64
	UAX_AsmMovRegReg(jctx, UAX_REG_RCCTX, UAX_REG_RDI);
	UAX_AsmMovRegReg(jctx, UAX_REG_RDX, UAX_REG_RSI);
	UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
		UAX_REG_RAX, UAX_REG_RSI, offsetof(BJX2_Trace, lnknext));
	UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
		UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt), UAX_REG_RAX);
#else
	UAX_AsmMovRegReg(jctx, UAX_REG_RCCTX, UAX_REG_RCX);
	UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
		UAX_REG_RAX, UAX_REG_RDX, offsetof(BJX2_Trace, lnknext));
//	UAX_AsmInsnRegImm(jctx, UAX_OP_MOV, UAX_REG_RAX, (s64)(tr->trnext));
	UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
		UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt), UAX_REG_RAX);
#endif

	UAX_AsmMovRegImm(jctx, UAX_REG_RAX, (nlint)(tr));
	UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
		UAX_REG_RCCTX, offsetof(BJX2_Context, tr_cur), UAX_REG_RAX);

	if(tr->addr_nxt)
	{
//		BJX2_JitStoreVMRegImm(jctx, BJX2_REG_PC, (s32)(tr->addr_nxt));
		BJX2_JitStoreVMRegImm(jctx, BJX2_REG_PC, tr->addr_nxt);
	}else
	{
//		BJX2_JitStoreVMRegImm(jctx, BJX2_REG_PC, (s32)(tr->addr_nxt));
		BJX2_JitStoreVMRegImm(jctx, BJX2_REG_PC, -1);

		UAX_AsmInsnRegReg(jctx, UAX_OP_XOR, UAX_REG_RAX, UAX_REG_RAX);
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt), UAX_REG_RAX);
	}


#if 1
//	if(tr->jmppc)
	if(1)
//	if(tr->trjmpnext)
	{
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RAX, UAX_REG_RDX, offsetof(BJX2_Trace, jmpnext));
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rjmp), UAX_REG_RAX);
	}else
//		if(tr->jmppc)
	{
		UAX_AsmInsnRegReg(jctx, UAX_OP_XOR, UAX_REG_RAX, UAX_REG_RAX);
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rjmp), UAX_REG_RAX);
	}
#endif

	for(i=0; i<tr->n_ops; i++)
	{
		op=tr->ops[i];

#if 0
		if((op->nmid==BJX2_NMID_PRED_T) ||
			(op->nmid==BJX2_NMID_PRED_F))
		{
			n_prt=0; n_prf=0;
			
			for(j=i; j<tr->n_ops; j++)
			{
				op=tr->ops[j];
				if(op->nmid==BJX2_NMID_PRED_T)
					{ op_prt[n_prt++]=op; continue; }
				if(op->nmid==BJX2_NMID_PRED_F)
					{ op_prf[n_prf++]=op; continue; }
				break;
			}
			
			BJX2_TryJitOpcodeArr_PredX(jctx, cpu, tr, op_prt, n_prt);
			BJX2_TryJitOpcodeArr_PredX(jctx, cpu, tr, op_prf, n_prf);
			
			i=j-1;
			continue;
		}
#endif
	
#if 0
		if(BJX2_TryJitOpcode(jctx, cpu, tr, tr->ops[i])>0)
			continue;
#endif

		BJX2_JitSyncRegs(jctx, cpu, tr);

		UAX_AsmMovRegImm(jctx, UAX_REG_RDX, (nlint)(tr->ops[i]));
		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RCCTX);
		BJX2_JitEmitCallFPtr(jctx, cpu, tr->ops[i]->Run);
		
		j=tr->ops[i]->nmid;
		k=tr->ops[i]->fmid;
		btesh2_nmid_jitmiss[j*64+k]++;
	}

	BJX2_JitSyncRegs(jctx, cpu, tr);

//	BJX2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_RCCTX, UAX_REG_RCCTX);
	UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
		UAX_REG_RAX, UAX_REG_RCCTX, offsetof(BJX2_Context, tr_rnxt));

	BJX2_JitSyncRestoreRegs(jctx, cpu, tr);

#if 0
	if(!nolink)
//	if(0)
	{
		l0=UAX_GenLabelTemp(jctx);
	//	l1=UAX_GenLabelTemp(jctx);

		/* validate we have next trace */
		UAX_AsmAndRegReg(jctx, UAX_REG_RAX, UAX_REG_RAX);
		UAX_AsmInsnLabel(jctx, UAX_OP_JE, l0|UAX_LBL_NEAR);

		UAX_AsmMovRegReg(jctx, UAX_REG_RDX, UAX_REG_RAX);

		/* validate we run-limit hasn't expired */
		UAX_AsmInsnStRegDisp32(jctx, UAX_OP_DEC,
			UAX_REG_RCCTX, offsetof(BJX2_Context, tr_runlim));
		UAX_AsmInsnLabel(jctx, UAX_OP_JBE, l0|UAX_LBL_NEAR);

		UAX_AsmMovRegImm(jctx, UAX_REG_EAX, tr->nwops);
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_ADD,
			UAX_REG_RCCTX, offsetof(BJX2_Context, tr_tops), UAX_REG_RAX);

		/* tail call into next trace */
		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RCCTX);
		BJX2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_RCCTX, UAX_REG_RCCTX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_RSP, BTEJ_X64_FRAMEADJ);
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RAX, UAX_REG_RDX, offsetof(BJX2_Trace, Run));
		UAX_AsmInsnReg(jctx, UAX_OP_JMP, UAX_REG_RAX);

		UAX_EmitLabel(jctx, l0);		/* normal return */
	}
#endif

	BJX2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_RCCTX, UAX_REG_RCCTX);
//	BJX2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_RSI, UAX_REG_RSI);

	UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_RSP, BTEJ_X64_FRAMEADJ);
	UAX_AsmInsnNone(jctx, UAX_OP_RET);
	
	sz=UAX_EmitGetOffs(jctx);
	
	ptr=UAX_LinkContext(jctx);
	UAX_FreeContext(jctx);
	
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


#endif
