void X3VM_Opc_LDU8_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=X3VM_MemLoadU8(ctx,
		ctx->reg[op->rs]+(op->imm<<op->sc));
}

void X3VM_Opc_LDS8_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=X3VM_MemLoadS8(ctx,
		ctx->reg[op->rs]+(op->imm<<op->sc));
}

void X3VM_Opc_LDU16_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=X3VM_MemLoadU16(ctx,
		ctx->reg[op->rs]+(op->imm<<op->sc));
}

void X3VM_Opc_LDS16_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=X3VM_MemLoadS16(ctx,
		ctx->reg[op->rs]+(op->imm<<op->sc));
}

void X3VM_Opc_LDU32_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=X3VM_MemLoadU32(ctx,
		ctx->reg[op->rs]+(op->imm<<op->sc));
}

void X3VM_Opc_LDS32_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=X3VM_MemLoadS32(ctx,
		ctx->reg[op->rs]+(op->imm<<op->sc));
}

void X3VM_Opc_LDU64_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=X3VM_MemLoadU64(ctx,
		ctx->reg[op->rs]+(op->imm<<op->sc));
}

void X3VM_Opc_LDS64_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=X3VM_MemLoadU64(ctx,
		ctx->reg[op->rs]+(op->imm<<op->sc));
}

void X3VM_Opc_ST8_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	X3VM_MemStoreU8(ctx,
		ctx->reg[op->rs]+(op->imm<<op->sc),
		ctx->reg[op->rn]);
}

void X3VM_Opc_ST16_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	X3VM_MemStoreU16(ctx,
		ctx->reg[op->rs]+(op->imm<<op->sc),
		ctx->reg[op->rn]);
}

void X3VM_Opc_ST32_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	X3VM_MemStoreU32(ctx,
		ctx->reg[op->rs]+(op->imm<<op->sc),
		ctx->reg[op->rn]);
}

void X3VM_Opc_ST64_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	X3VM_MemStoreU64(ctx,
		ctx->reg[op->rs]+(op->imm<<op->sc),
		ctx->reg[op->rn]);
}


void X3VM_Opc_LD64P_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;

#if 0
	if((ctx->reg[op->rs]+(op->imm<<op->sc))&7)
	{
		X3VM_ThrowFault(ctx, X3VM_FAULT_MEMACC);
	}
#endif

//	ctx->reg[op->rn+0]=X3VM_MemLoadU64(ctx,
//		ctx->reg[op->rs]+(op->imm<<op->sc)+0);
//	ctx->reg[op->rn+1]=X3VM_MemLoadU64(ctx,
//		ctx->reg[op->rs]+(op->imm<<op->sc)+8);
	X3VM_MemLoad64P(ctx, ctx->reg[op->rs]+(op->imm<<op->sc), ctx->reg+op->rn);
}

void X3VM_Opc_ST64P_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;

#if 0
	if((ctx->reg[op->rs]+(op->imm<<op->sc))&7)
	{
		X3VM_ThrowFault(ctx, X3VM_FAULT_MEMACC);
	}
#endif

//	X3VM_MemStoreU64(ctx,
//		ctx->reg[op->rs]+(op->imm<<op->sc)+0,
//		ctx->reg[op->rn+0]);
//	X3VM_MemStoreU64(ctx,
//		ctx->reg[op->rs]+(op->imm<<op->sc)+8,
//		ctx->reg[op->rn+1]);
	X3VM_MemStore64P(ctx, ctx->reg[op->rs]+(op->imm<<op->sc), ctx->reg+op->rn);
}


void X3VM_Opc_LDS32_3RI_FSP(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s32 *ptr;
	ptr=(s32 *)(ctx->reg[op->rs]+(op->imm<<op->sc));
	ctx->reg[op->rn]=ptr[0];
}

void X3VM_Opc_LDU32_3RI_FSP(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u32 *ptr;
	ptr=(u32 *)(ctx->reg[op->rs]+(op->imm<<op->sc));
	ctx->reg[op->rn]=ptr[0];
}

void X3VM_Opc_ST32_3RI_FSP(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u32 *ptr;
	ptr=(u32 *)(ctx->reg[op->rs]+(op->imm<<op->sc));
	ptr[0]=ctx->reg[op->rn];
}

void X3VM_Opc_LD64_3RI_FSP(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 *ptr;
//	ptr=(u64 *)(ctx->reg[op->rs]+(op->imm<<op->sc));
	ptr=(u64 *)(ctx->reg[op->rs]+op->imm);
	ctx->reg[op->rn]=ptr[0];
}

void X3VM_Opc_ST64_3RI_FSP(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 *ptr;
//	ptr=(u64 *)(ctx->reg[op->rs]+(op->imm<<op->sc));
	ptr=(u64 *)(ctx->reg[op->rs]+op->imm);
	ptr[0]=ctx->reg[op->rn];
}

void X3VM_Opc_LD64P_3RI_FSP(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 *ptr, *reg;
	int rs, rn;
	reg=ctx->reg; rs=op->rs; rn=op->rn;
//	ptr=(u64 *)(ctx->reg[op->rs]+(op->imm<<op->sc));
	ptr=(u64 *)(reg[rs]+op->imm);
	reg[rn+0]=ptr[0];
	reg[rn+1]=ptr[1];
}

void X3VM_Opc_ST64P_3RI_FSP(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 *ptr, *reg;
	int rs, rn;
	reg=ctx->reg; rs=op->rs; rn=op->rn;
//	ptr=(u64 *)(ctx->reg[op->rs]+(op->imm<<op->sc));
	ptr=(u64 *)(reg[rs]+op->imm);
	ptr[0]=reg[rn+0];
	ptr[1]=reg[rn+1];
}


void X3VM_Opc_LD64Q_3RI_FSP(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 *ptr, *reg;
	int rs, rn, ru;
	reg=ctx->reg; rs=op->rs; rn=op->rn; ru=op->ru;
//	ptr=(u64 *)(ctx->reg[op->rs]+(op->imm<<op->sc));
	ptr=(u64 *)(reg[rs]+(u32)(op->imm));
	reg[rn+0]=ptr[0];
	reg[rn+1]=ptr[1];
	ptr=(u64 *)(reg[rs]+(u32)(op->imm>>32));
	reg[ru+0]=ptr[0];
	reg[ru+1]=ptr[1];
}

void X3VM_Opc_ST64Q_3RI_FSP(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 *ptr, *reg;
	int rs, rn, ru;
	reg=ctx->reg; rs=op->rs; rn=op->rn; ru=op->ru;
//	ptr=(u64 *)(ctx->reg[op->rs]+(op->imm<<op->sc));
	ptr=(u64 *)(reg[rs]+(u32)(op->imm));
	ptr[0]=reg[rn+0];
	ptr[1]=reg[rn+1];
	ptr=(u64 *)(reg[rs]+(u32)(op->imm>>32));
	ptr[0]=reg[ru+0];
	ptr[1]=reg[ru+1];
}

void X3VM_Opc_FLD32TO64_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vn;
	u32 vc;
	
	ctx->reg[X3VM_REG_BPC]=op->addr;
	vc=X3VM_MemLoadU32(ctx, ctx->reg[op->rs]+(op->imm<<op->sc));
	*((double *)(&vn))=*((float *)(&vc));
	ctx->reg[op->rn]=vn;

//	ctx->reg[op->rn]=X3VM_ConvFp32ToFp64(
//		X3VM_MemLoadU32(ctx,
//			ctx->reg[op->rs]+(op->imm<<op->sc)));
}

void X3VM_Opc_FLD16TO64_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=X3VM_ConvFp16ToFp64(
		X3VM_MemLoadU16(ctx,
			ctx->reg[op->rs]+(op->imm<<op->sc)));
}

void X3VM_Opc_FST64TO32_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 va;
	u32 vc;

	ctx->reg[X3VM_REG_BPC]=op->addr;

	va=ctx->reg[op->rn];
	*((float *)(&vc))=*((double *)(&va));
	X3VM_MemStoreU32(ctx,
		ctx->reg[op->rs]+(op->imm<<op->sc), vc);

//	X3VM_MemStoreU32(ctx,
//		ctx->reg[op->rs]+(op->imm<<op->sc),
//		X3VM_ConvFp64ToFp32(ctx->reg[op->rn]));
}

void X3VM_Opc_FST64TO16_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	X3VM_MemStoreU16(ctx,
		ctx->reg[op->rs]+(op->imm<<op->sc),
		X3VM_ConvFp64ToFp16(ctx->reg[op->rn]));
}


void X3VM_Opc_LDU8_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=X3VM_MemLoadU8(ctx,
		ctx->reg[op->rs]+(ctx->reg[op->rt]<<op->sc));
}

void X3VM_Opc_LDS8_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=X3VM_MemLoadS8(ctx,
		ctx->reg[op->rs]+(ctx->reg[op->rt]<<op->sc));
}

void X3VM_Opc_LDU16_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=X3VM_MemLoadU16(ctx,
		ctx->reg[op->rs]+(ctx->reg[op->rt]<<op->sc));
}

void X3VM_Opc_LDS16_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=X3VM_MemLoadS16(ctx,
		ctx->reg[op->rs]+(ctx->reg[op->rt]<<op->sc));
}

void X3VM_Opc_LDU32_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=X3VM_MemLoadU32(ctx,
		ctx->reg[op->rs]+(ctx->reg[op->rt]<<op->sc));
}

void X3VM_Opc_LDS32_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=X3VM_MemLoadS32(ctx,
		ctx->reg[op->rs]+(ctx->reg[op->rt]<<op->sc));
}

void X3VM_Opc_LDU64_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=X3VM_MemLoadU64(ctx,
		ctx->reg[op->rs]+(ctx->reg[op->rt]<<op->sc));
}

void X3VM_Opc_LDS64_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=X3VM_MemLoadS64(ctx,
		ctx->reg[op->rs]+(ctx->reg[op->rt]<<op->sc));
}

void X3VM_Opc_ST8_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	X3VM_MemStoreU8(ctx,
		ctx->reg[op->rs]+(ctx->reg[op->rt]<<op->sc),
		ctx->reg[op->rn]);
}

void X3VM_Opc_ST16_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	X3VM_MemStoreU16(ctx,
		ctx->reg[op->rs]+(ctx->reg[op->rt]<<op->sc),
		ctx->reg[op->rn]);
}

void X3VM_Opc_ST32_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	X3VM_MemStoreU32(ctx,
		ctx->reg[op->rs]+(ctx->reg[op->rt]<<op->sc),
		ctx->reg[op->rn]);
}

void X3VM_Opc_ST64_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	X3VM_MemStoreU64(ctx,
		ctx->reg[op->rs]+(ctx->reg[op->rt]<<op->sc),
		ctx->reg[op->rn]);
}


void X3VM_Opc_LD64P_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;

	if((ctx->reg[op->rs]+(ctx->reg[op->rt]<<op->sc))&7)
	{
		X3VM_ThrowFault(ctx, X3VM_FAULT_MEMACC);
	}

	ctx->reg[op->rn+0]=X3VM_MemLoadU64(ctx,
		ctx->reg[op->rs]+(ctx->reg[op->rt]<<op->sc)+0);
	ctx->reg[op->rn+1]=X3VM_MemLoadU64(ctx,
		ctx->reg[op->rs]+(ctx->reg[op->rt]<<op->sc)+8);
}

void X3VM_Opc_ST64P_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;

	if((ctx->reg[op->rs]+(ctx->reg[op->rt]<<op->sc))&7)
	{
		X3VM_ThrowFault(ctx, X3VM_FAULT_MEMACC);
	}

	X3VM_MemStoreU64(ctx,
		ctx->reg[op->rs]+(ctx->reg[op->rt]<<op->sc)+0,
		ctx->reg[op->rn+0]);
	X3VM_MemStoreU64(ctx,
		ctx->reg[op->rs]+(ctx->reg[op->rt]<<op->sc)+8,
		ctx->reg[op->rn+1]);
}


void X3VM_Opc_FLD32TO64_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=X3VM_ConvFp32ToFp64(
		X3VM_MemLoadU32(ctx,
			ctx->reg[op->rs]+(ctx->reg[op->rt]<<op->sc)));
}

void X3VM_Opc_FLD16TO64_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=X3VM_ConvFp16ToFp64(
		X3VM_MemLoadU16(ctx,
			ctx->reg[op->rs]+(ctx->reg[op->rt]<<op->sc)));
}

void X3VM_Opc_FST64TO32_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	X3VM_MemStoreU32(ctx,
		ctx->reg[op->rs]+(ctx->reg[op->rt]<<op->sc),
		X3VM_ConvFp64ToFp32(ctx->reg[op->rn]));
}

void X3VM_Opc_FST64TO16_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	X3VM_MemStoreU16(ctx,
		ctx->reg[op->rs]+(ctx->reg[op->rt]<<op->sc),
		X3VM_ConvFp64ToFp16(ctx->reg[op->rn]));
}


void X3VM_Opc_LEA_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=
		(ctx->reg[op->rs]+(ctx->reg[op->rt]<<op->sc))&
		0x0000FFFFFFFFFFFFULL;
}

void X3VM_Opc_LEA_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=
		(ctx->reg[op->rs]+(op->imm<<op->sc))&
		0x0000FFFFFFFFFFFFULL;
}

void X3VM_Opc_SHNADD_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=
		ctx->reg[op->rt]+(ctx->reg[op->rs]<<op->sc);
}

void X3VM_Opc_SHNADDUW_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=
		ctx->reg[op->rt]+(((u32)ctx->reg[op->rs])<<op->sc);
}

void X3VM_Opc_LEAT_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vn;

	if(op->rt==X3VM_REG_BPC)
	{
		vn=(op->addr+(ctx->reg[op->rt]<<op->sc))&0x0000FFFFFFFFFFFFULL;
		vn=vn|((u64)(ctx->pc_hibits)<<48)|1;
		ctx->reg[op->rn]=vn;
		return;
	}
	
	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=
		(ctx->reg[op->rs]+(ctx->reg[op->rt]<<op->sc))&
		0x0000FFFFFFFFFFFFULL;
}

void X3VM_Opc_LEAT_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vn;

	if(op->rt==X3VM_REG_BPC)
	{
		vn=(op->addr+(op->imm<<op->sc))&0x0000FFFFFFFFFFFFULL;
		vn=vn|((u64)(ctx->pc_hibits)<<48)|1;
		ctx->reg[op->rn]=vn;
		return;
	}
	
//	ctx->reg[X3VM_REG_BPC]=op->addr;
	ctx->reg[op->rn]=
		(ctx->reg[op->rs]+(op->imm<<op->sc))&
		0x0000FFFFFFFFFFFFULL;
}

void X3VM_Opc_INVOP_NONE(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_BPC]=op->addr;
	X3VM_ThrowFault(ctx, X3VM_FAULT_INVOP);
}

void X3VM_Opc_EBREAK_NONE(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_SPC]=op->addr;
	ctx->reg[X3VM_REG_TEA]=op->imm;

	ctx->reg[X3VM_REG_BPC]=op->addr;
	X3VM_ThrowFault(ctx, X3VM_FAULT_BREAK);
}

void X3VM_Opc_ECALL_NONE(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_SPC]=op->addr;
	ctx->reg[X3VM_REG_TEA]=op->imm;

	ctx->reg[X3VM_REG_BPC]=op->addr;
	X3VM_ThrowFault(ctx, X3VM_FAULT_ECALL);
}

void X3VM_Opc_BTSTT_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	if((vs&vt)==0)
	{
		ctx->reg[X3VM_REG_PC]=
//			(ctx->reg[X3VM_REG_PC]&0xFFFF000000000000ULL) |
			((op->addr+op->imm)&0x0000FFFFFFFFFFFEULL) ;
		ctx->tr_next=ctx->tr_bra;
	}
}

void X3VM_Opc_BTSTF_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	if((vs&vt)!=0)
	{
		ctx->reg[X3VM_REG_PC]=
//			(ctx->reg[X3VM_REG_PC]&0xFFFF000000000000ULL) |
			((op->addr+op->imm)&0x0000FFFFFFFFFFFEULL) ;
		ctx->tr_next=ctx->tr_bra;
	}
}

void X3VM_Opc_BLT_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	if(vs<vt)
	{
		ctx->reg[X3VM_REG_PC]=
//			(ctx->reg[X3VM_REG_PC]&0xFFFF000000000000ULL) |
			((op->addr+op->imm)&0x0000FFFFFFFFFFFEULL) ;
		ctx->tr_next=ctx->tr_bra;
	}
}

void X3VM_Opc_BGE_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	if(vs>=vt)
	{
		ctx->reg[X3VM_REG_PC]=
//			(ctx->reg[X3VM_REG_PC]&0xFFFF000000000000ULL) |
			((op->addr+op->imm)&0x0000FFFFFFFFFFFEULL) ;
		ctx->tr_next=ctx->tr_bra;
	}
}

void X3VM_Opc_BLTU_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	if(vs<vt)
	{
		ctx->reg[X3VM_REG_PC]=
//			(ctx->reg[X3VM_REG_PC]&0xFFFF000000000000ULL) |
			((op->addr+op->imm)&0x0000FFFFFFFFFFFEULL) ;
		ctx->tr_next=ctx->tr_bra;
	}
}

void X3VM_Opc_BGEU_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	if(vs>=vt)
	{
		ctx->reg[X3VM_REG_PC]=
//			(ctx->reg[X3VM_REG_PC]&0xFFFF000000000000ULL) |
			((op->addr+op->imm)&0x0000FFFFFFFFFFFEULL) ;
		ctx->tr_next=ctx->tr_bra;
	}
}

void X3VM_Opc_BEQ_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	if(vs==vt)
	{
		ctx->reg[X3VM_REG_PC]=
//			(ctx->reg[X3VM_REG_PC]&0xFFFF000000000000ULL) |
			((op->addr+op->imm)&0x0000FFFFFFFFFFFEULL) ;
		ctx->tr_next=ctx->tr_bra;
	}
}

void X3VM_Opc_BNE_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	if(vs!=vt)
	{
		ctx->reg[X3VM_REG_PC]=
//			(ctx->reg[X3VM_REG_PC]&0xFFFF000000000000ULL) |
			((op->addr+op->imm)&0x0000FFFFFFFFFFFEULL) ;
		ctx->tr_next=ctx->tr_bra;
	}
}

void X3VM_Opc_JAL_2RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	if(op->rn!=X3VM_REG_ZR)
	{
		ctx->reg[op->rn]=
//			(ctx->reg[X3VM_REG_PC]&0xFFFF000000000000ULL) |
			(((u64)ctx->pc_hibits)<<48) |
			((op->addr+4)&0x0000FFFFFFFFFFFEULL) |
			1;
	}

	ctx->reg[X3VM_REG_PC]=
//		(ctx->reg[X3VM_REG_PC]&0xFFFF000000000000ULL) |
		((op->addr+op->imm)&0x0000FFFFFFFFFFFEULL) ;
	ctx->tr_next=ctx->tr_bra;
}

void X3VM_Opc_BRA_2RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_PC]=
//		(ctx->reg[X3VM_REG_PC]&0xFFFF000000000000ULL) |
		((op->addr+op->imm)&0x0000FFFFFFFFFFFEULL) ;
	ctx->tr_next=ctx->tr_bra;
}

void X3VM_Opc_BSR_2RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[X3VM_REG_LR]=
//		(ctx->reg[X3VM_REG_PC]&0xFFFF000000000000ULL) |
		(((u64)ctx->pc_hibits)<<48) |
		((op->addr+4)&0x0000FFFFFFFFFFFEULL) | 1;
	ctx->reg[X3VM_REG_PC]=
//		(ctx->reg[X3VM_REG_PC]&0xFFFF000000000000ULL) |
		((op->addr+op->imm)&0x0000FFFFFFFFFFFEULL) ;

	ctx->tr_pred_lr2=ctx->tr_pred_lr1;
	ctx->tr_pred_lr1=ctx->tr_pred_lr0;
	ctx->tr_pred_lr0=ctx->tr_next;

	ctx->tr_next=ctx->tr_bra;
}

void X3VM_Opc_BRANB_2RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
}

void X3VM_Opc_BSRNB_2RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	X3VM_Trace *tr_nb;

	ctx->reg[X3VM_REG_LR]=
//		(ctx->reg[X3VM_REG_PC]&0xFFFF000000000000ULL) |
		(((u64)ctx->pc_hibits)<<48) |
		((op->addr+4)&0x0000FFFFFFFFFFFEULL) | 1;

	tr_nb=ctx->tr_cur->tr_nobra;
	if(tr_nb)
	{
		ctx->tr_pred_lr2=ctx->tr_pred_lr1;
		ctx->tr_pred_lr1=ctx->tr_pred_lr0;
		ctx->tr_pred_lr0=tr_nb;
	}
}

void X3VM_Opc_JALR_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	X3VM_Trace *tr_p;
	u64 pc1, vs;

	if(op->rn!=X3VM_REG_ZR)
	{
		ctx->reg[op->rn]=
//			(ctx->reg[X3VM_REG_PC]&0xFFFF000000000000ULL) |
			(((u64)ctx->pc_hibits)<<48) |
			((op->addr+4)&0x0000FFFFFFFFFFFEULL) | 1;
	}

	vs=ctx->reg[op->rs];
	pc1=
//		(ctx->reg[X3VM_REG_PC]&0xFFFF000000000000ULL) |
		((vs+op->imm)&0x0000FFFFFFFFFFFEULL) ;
	ctx->reg[X3VM_REG_PC]=pc1;
	if(vs&1)
		ctx->pc_hibits=vs>>48;
	ctx->tr_next=NULL;
	
	tr_p=ctx->tr_pred_lr0;
	if(tr_p && !ctx->fault)
	{
		if((pc1&0x0000FFFFFFFFFFFE)==tr_p->addr)
		{
			ctx->tr_next=tr_p;
			ctx->tr_pred_lr0=ctx->tr_pred_lr1;
			ctx->tr_pred_lr1=ctx->tr_pred_lr2;
			ctx->tr_pred_lr2=NULL;
		}
	}
}

u64 X3VM_GetVirtTimeUsec(X3VM_Context *ctx)
{
	s64 cyc, relcyc, refusec, usec, lusec;

	cyc=ctx->totcyc;
	refusec=ctx->timeref_usec;
	lusec=ctx->timeref_lastusec;
	relcyc=cyc-ctx->timeref_cyc;
	usec=refusec+((relcyc*ctx->timeref_scale)>>16);
	
	if(usec<lusec)
		usec=lusec;
	
	ctx->timeref_lastusec=usec;
	
	return(usec);
}

u64 X3VM_GetCSR(X3VM_Context *ctx, int id)
{
	if(id==0xFDC)
	{
		return(X3VM_GetVirtTimeUsec(ctx));
	}
	if(id==0xFDE)
	{
		return(ctx->totcyc);
	}
	if(id==0xFDF)
	{
		return(X3VM_RngRand64(ctx));
	}

	return(0);
}

void X3VM_SetCSR(X3VM_Context *ctx, int id, u64 val)
{
}

void X3VM_Opc_CSRRW_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=X3VM_GetCSR(ctx, (op->imm)&0xFFF);
	X3VM_SetCSR(ctx, (op->imm)&0xFFF, vs);
	if(op->rn!=0)
		ctx->reg[op->rn]=vt;
}

void X3VM_Opc_CSRRS_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=X3VM_GetCSR(ctx, (op->imm)&0xFFF);
	X3VM_SetCSR(ctx, (op->imm)&0xFFF, vt|vs);
	if(op->rn!=0)
		ctx->reg[op->rn]=vt;
}

void X3VM_Opc_CSRRC_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=X3VM_GetCSR(ctx, (op->imm)&0xFFF);
	X3VM_SetCSR(ctx, (op->imm)&0xFFF, vt&(~vs));
	if(op->rn!=0)
		ctx->reg[op->rn]=vt;
}


void X3VM_Opc_CSRRWI_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=op->rs;
	vt=X3VM_GetCSR(ctx, (op->imm)&0xFFF);
	X3VM_SetCSR(ctx, (op->imm)&0xFFF, vs);
	if(op->rn!=0)
		ctx->reg[op->rn]=vt;
}

void X3VM_Opc_CSRRSI_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=op->rs;
	vt=X3VM_GetCSR(ctx, (op->imm)&0xFFF);
	X3VM_SetCSR(ctx, (op->imm)&0xFFF, vt|vs);
	if(op->rn!=0)
		ctx->reg[op->rn]=vt;
}

void X3VM_Opc_CSRRCI_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=op->rs;
	vt=X3VM_GetCSR(ctx, (op->imm)&0xFFF);
	X3VM_SetCSR(ctx, (op->imm)&0xFFF, vt&(~vs));
	if(op->rn!=0)
		ctx->reg[op->rn]=vt;
}
