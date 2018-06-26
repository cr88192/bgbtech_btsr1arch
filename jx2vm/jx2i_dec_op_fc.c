int BJX2_DecodeOpcode_DecFC(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2, int opw3)
{
	s32 imm32;
	int rn_i32;
	int ret;
	
	op->fl|=BJX2_OPFL_TRIWORD;
	op->opn=opw1;
	op->opn2=opw2;
	op->opn3=opw3;

	rn_i32=opw1&15;
	if(opw1&0x0100)
		rn_i32+=16;

	imm32=(opw3<<16)|((u16)opw2);

	ret=0;
	switch((opw1>>4)&15)
	{
	case 0x0:
		break;

	case 0x2:
		switch(opw1&15)
		{
		case 0x4:
			op->imm=imm32;
			op->nmid=BJX2_NMID_LDIZ;
			op->fmid=BJX2_FMID_IMM;
			op->Run=BJX2_Op_LDIZ_Imm32;
			break;
		case 0x5:
			op->imm=imm32;
			op->nmid=BJX2_NMID_LDIN;
			op->fmid=BJX2_FMID_IMM;
			op->Run=BJX2_Op_LDIN_Imm32;
			break;
		case 0x6:
			op->imm=imm32;
			op->nmid=BJX2_NMID_LDISH32;
			op->fmid=BJX2_FMID_IMM;
			op->Run=BJX2_Op_LDISH32_Imm;
			break;
		}
		break;

	case 0xC:
		op->imm=imm32;
		op->rn=rn_i32;
		op->nmid=BJX2_NMID_ADD;
		op->fmid=BJX2_FMID_IMMREG;
		op->Run=BJX2_Op_ADD_ImmReg;
		break;

	case 0xD:
		op->imm=imm32;
		op->rn=rn_i32;
		op->nmid=BJX2_NMID_LDISH32;
		op->fmid=BJX2_FMID_IMMREG;
		op->Run=BJX2_Op_LDISH32_ImmReg;
		break;

	case 0xE:
		op->imm=imm32;
		op->rn=rn_i32;
		op->nmid=BJX2_NMID_MOV;
		op->fmid=BJX2_FMID_IMMREG;
		op->Run=BJX2_Op_MOV_ImmReg;
		break;

	default:
		ret=-1;
		break;
	}
	return(ret);
}
