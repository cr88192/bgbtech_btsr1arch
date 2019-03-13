int BJX2_DecodeOpcode_DecF8(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2)
{
	s32 imm16u, imm16n, imm16s;
	int rn_i16;
	int ret;
	
	op->fl|=BJX2_OPFL_TWOWORD;
	op->opn=opw1;
	op->opn2=opw2;

//	rn_i16=opw1&15;
//	if(opw1&0x0100)
//		rn_i16+=16;
	rn_i16=opw1&31;

	imm16u=(u16)opw2;
	imm16n=opw2|(~65535);
	imm16s=(s16)opw2;

	ret=0;
	switch((opw1>>4)&15)
	{
	case 0x0:	case 0x1:
		op->imm=imm16u;
		op->rn=rn_i16;
//		op->nmid=BJX2_NMID_MOV;
		op->nmid=BJX2_NMID_LDI;
		op->fmid=BJX2_FMID_IMMREG;
		op->Run=BJX2_Op_MOV_ImmReg;
		break;
	case 0x2:	case 0x3:
		op->imm=imm16n;
		op->rn=rn_i16;
//		op->nmid=BJX2_NMID_MOV;
		op->nmid=BJX2_NMID_LDI;
		op->fmid=BJX2_FMID_IMMREG;
		op->Run=BJX2_Op_MOV_ImmReg;
		break;
	case 0x4:	case 0x5:
		op->imm=imm16s;
		op->rn=rn_i16;
		op->nmid=BJX2_NMID_ADD;
		op->fmid=BJX2_FMID_IMMREG;
		op->Run=BJX2_Op_ADD_ImmReg;
		break;
	case 0x6:	case 0x7:
		op->imm=imm16u;
		op->rn=rn_i16;
		op->nmid=BJX2_NMID_LDISH16;
		op->fmid=BJX2_FMID_IMMREG;
		op->Run=BJX2_Op_LDISH16_ImmReg;
		break;

	default:
		ret=-1;
		break;
	}
	return(ret);
}

int BJX2_DecodeOpcode_DecD8(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2, int opw3)
{
	BJX2_Opcode *op1;
	int ret;

	op->fl|=BJX2_OPFL_TRIWORD;
	op->opn=opw1;
	op->opn2=opw2;
	op->opn3=opw3;
	
	op1=BJX2_ContextAllocOpcode(ctx);

	ret=BJX2_DecodeOpcode_DecF8(ctx, op1, addr, opw1, opw2, opw3);

	if(opw1&0x0200)
	{
		op->nmid=BJX2_NMID_PRED_F;
		op->fmid=BJX2_FMID_CHAIN;
		op->Run=BJX2_Op_PREDF_Chn;
		op->data=op1;
	}else
	{
		op->nmid=BJX2_NMID_PRED_T;
		op->fmid=BJX2_FMID_CHAIN;
		op->Run=BJX2_Op_PREDT_Chn;
		op->data=op1;
	}

	return(ret);
}
