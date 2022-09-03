void VMX86_Op_MovRegMemByte(VMX86_Context *ctx, VMX86_Opcode *op)
{
	u64 vb;
	vb=op->GetMem(ctx, op);

	if(op->rex)
	{
		ctx->regs[op->rn]=
			(ctx->regs[op->rn]&0x00000000FFFFFF00ULL) |
			((byte)vb);
	}else
	{
		if(op->rn&4)
		{
			ctx->regs[op->rn]=
				(ctx->regs[op->rn]&0x00000000FFFF00FFULL) |
				(((byte)vb)<<8);
		}else
		{
			ctx->regs[op->rn]=
				(ctx->regs[op->rn]&0x00000000FFFFFF00ULL) |
				((byte)vb);
		}
	}
}

void VMX86_Op_MovRegMemWord(VMX86_Context *ctx, VMX86_Opcode *op)
{
	u64 vb;
	vb=op->GetMem(ctx, op);
	ctx->regs[op->rn]=
		(ctx->regs[op->rn]&0x00000000FFFF0000ULL) |
		((u16)vb);
}

void VMX86_Op_MovRegMemDWord(VMX86_Context *ctx, VMX86_Opcode *op)
{
	u64 vb;
	vb=op->GetMem(ctx, op);
	ctx->regs[op->rn]=(u32)vb;
}

void VMX86_Op_MovRegMemQWord(VMX86_Context *ctx, VMX86_Opcode *op)
{
	u64 vb;
	vb=op->GetMem(ctx, op);
	ctx->regs[op->rn]=vb;
}

void VMX86_Op_AluRegMemDWord(VMX86_Context *ctx, VMX86_Opcode *op)
{
	u64 va, vb, vc, vd, fl;
	
	va=ctx->regs[op->rn&15];
	vb=op->GetMem(ctx, op);
	
	fl=ctx->regs[VMX86_REGS_FLAGS];
	
	switch(op->nmid)
	{
	case VMX86_NMID_ADD:	vc=va+vb;	vd=vc;	break;
	case VMX86_NMID_OR:		vc=va|vb;	vd=vc;	break;

	case VMX86_NMID_ADC:
		vc=va+vb+(fl&VMX86_FLAGS_C);
		vd=vc;
		break;
	case VMX86_NMID_SBB:
		vc=va-vb-(fl&VMX86_FLAGS_C);
		vd=vc;
		break;

	case VMX86_NMID_AND:	vc=va&vb;	vd=vc;	break;
	case VMX86_NMID_SUB:	vc=va-vb;	vd=vc;	break;
	case VMX86_NMID_XOR:	vc=va^vb;	vd=vc;	break;
	case VMX86_NMID_CMP:	vc=va-vb;	vd=va;	break;
	case VMX86_NMID_MOV:	vc=vb;		vd=vb;	break;
	}
	
	fl&=~(
		VMX86_FLAGS_C|VMX86_FLAGS_P|
		VMX86_FLAGS_A|VMX86_FLAGS_Z|
		VMX86_FLAGS_S|VMX86_FLAGS_O);

	if((vc>>32)&1)
		fl|=VMX86_FLAGS_C;
	if(!((u32)vc))
		fl|=VMX86_FLAGS_Z;
	if((vc>>31)&1)
		fl|=VMX86_FLAGS_S;
	if(((vc>>31)&3)==3)
		fl|=VMX86_FLAGS_O;
	
	ctx->regs[op->rn&15]=(u32)vd;
}

