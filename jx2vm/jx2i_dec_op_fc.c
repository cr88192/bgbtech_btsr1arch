int BJX2_DecodeOpcode_DecFC(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2, int opw3)
{
	int rn_dfl, rm_dfl;
	s32 imm32, imm24;
	s32 disp17s, disp17u;
	int rn_i32;
	int rn_i24;
	int eq;
	int ret;
	
	op->fl|=BJX2_OPFL_TRIWORD;
	op->opn=opw1;
	op->opn2=opw2;
	op->opn3=opw3;

	rn_i32=opw1&15;
	if(opw1&0x0100)
		rn_i32+=16;

	rn_i24=(opw2>>8)&15;
	if(opw1&0x0100)
		rn_i24+=16;

	imm32=(opw3<<16)|((u16)opw2);
	imm24=(((sbyte)opw2)<<16)|((u16)opw3);

	rn_dfl=(opw2>>4)&15;
	rm_dfl=(opw2   )&15;
	if(opw1&0x0004)rn_dfl+=16;
	if(opw1&0x0002)rm_dfl+=16;

	disp17s=(u16)opw3;
	disp17u=(u16)opw3;
	if(opw1&0x0001)
	{
		disp17s|=(-1)<<16;
		disp17u|=( 1)<<16;
	}
	
//	eq=(opw1&0x0008)?1:0;
	eq=(opw1>>3)&1;

	ret=0;
	switch((opw1>>4)&15)
	{
	case 0x0:
		switch((opw2>>12)&15)
		{
		case 0x0:	/* FC0e_0zzz_dddd */
			op->rn=rn_dfl;
			op->rm=rm_dfl;
			op->imm=disp17s;
			switch((opw2>>8)&15)
			{
			case 0x0:	/* FC0e_00zz_dddd */
				if(eq)
				{
					op->nmid=BJX2_NMID_LEAB;
					op->fmid=BJX2_FMID_LDREGDISPREG;
					op->Run=BJX2_Op_LEAB_LdRegDispReg;
					break;
				}
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_REGSTREGDISP;
				op->Run=BJX2_Op_MOVB_RegStRegDisp;
				break;
			case 0x1:	/* FC0e_01zz_dddd */
				if(eq)
				{
					op->nmid=BJX2_NMID_LEAW;
					op->fmid=BJX2_FMID_LDREGDISPREG;
					op->Run=BJX2_Op_LEAW_LdRegDispReg;
					break;
				}
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_REGSTREGDISP;
				op->Run=BJX2_Op_MOVW_RegStRegDisp;
				break;
			case 0x2:	/* FC0e_02zz_dddd */
				if(eq)
				{
					op->nmid=BJX2_NMID_LEAD;
					op->fmid=BJX2_FMID_LDREGDISPREG;
					op->Run=BJX2_Op_LEAD_LdRegDispReg;
					break;
				}
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_REGSTREGDISP;
				op->Run=BJX2_Op_MOVL_RegStRegDisp;
				break;
			case 0x3:	/* FC0e_03zz_dddd */
				if(eq)
				{
					op->nmid=BJX2_NMID_LEAQ;
					op->fmid=BJX2_FMID_LDREGDISPREG;
					op->Run=BJX2_Op_LEAQ_LdRegDispReg;
					break;
				}
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_REGSTREGDISP;
				op->Run=BJX2_Op_MOVQ_RegStRegDisp;
				break;

			case 0x8:	/* FC0e_08zz_dddd */
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_MOVB_LdRegDispReg;
				if(eq)
				{	op->nmid=BJX2_NMID_MOVUB;
					op->Run=BJX2_Op_MOVUB_LdRegDispReg;		}
				break;
			case 0x9:	/* FC0e_09zz_dddd */
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_MOVW_LdRegDispReg;
				if(eq)
				{	op->nmid=BJX2_NMID_MOVUW;
					op->Run=BJX2_Op_MOVUW_LdRegDispReg;		}
				break;
			case 0xA:	/* FC0e_0Azz_dddd */
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_MOVL_LdRegDispReg;
				if(eq)
				{	op->nmid=BJX2_NMID_MOVUL;
					op->Run=BJX2_Op_MOVUL_LdRegDispReg;		}
				break;
			case 0xB:	/* FC0e_0Bzz_dddd */
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_MOVQ_LdRegDispReg;
				if(eq)
				{	op->nmid=BJX2_NMID_MOVDL;
					op->Run=BJX2_Op_MOVUL_LdRegDispReg;		}
				break;

			}
			break;

		case 0x1:	/* FC0e_1zzz_iiii */
			op->rn=rn_dfl;
			op->rm=rm_dfl;
			op->imm=disp17s;
			switch((opw2>>8)&15)
			{
			case 0x0:	/* FC0e_10zz_iiii */
				op->imm=disp17u;
				op->nmid=BJX2_NMID_ADD;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_ADD_RegRegReg;
				if(eq)
				{	op->fmid=BJX2_FMID_REGIMMREG;
					op->Run=BJX2_Op_ADD_RegImmReg;	}
				break;
			case 0x1:	/* FC0e_11zz_iiii */
				op->imm=disp17u;
				op->nmid=BJX2_NMID_SUB;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_SUB_RegRegReg;
				if(eq)
				{	op->fmid=BJX2_FMID_REGIMMREG;
					op->Run=BJX2_Op_SUB_RegImmReg;	}
				break;
			case 0x2:	/* FC0e_12zz_iiii */
				op->nmid=BJX2_NMID_MULS;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_MULS_RegRegReg;
				if(eq)
				{	op->fmid=BJX2_FMID_REGIMMREG;
					op->Run=BJX2_Op_MULS_RegImmReg;	}
				break;

			case 0x5:	/* FC0e_15zz_iiii */
				op->nmid=BJX2_NMID_AND;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_AND_RegRegReg;
				if(eq)
				{	op->fmid=BJX2_FMID_REGIMMREG;
					op->Run=BJX2_Op_AND_RegImmReg;	}
				break;
			case 0x6:	/* FC0e_16zz_iiii */
				op->nmid=BJX2_NMID_OR;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_OR_RegRegReg;
				if(eq)
				{	op->fmid=BJX2_FMID_REGIMMREG;
					op->Run=BJX2_Op_OR_RegImmReg;	}
				break;
			case 0x7:	/* FC0e_17zz_iiii */
				op->nmid=BJX2_NMID_XOR;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_XOR_RegRegReg;
				if(eq)
				{	op->fmid=BJX2_FMID_REGIMMREG;
					op->Run=BJX2_Op_XOR_RegImmReg;	}
				break;

			case 0x8:	/* FC0e_18zz_iiii */
				break;
			case 0x9:	/* FC0e_19zz_iiii */
				switch((opw2>>0)&15)
				{
				case 0x4:	/* FC0e_19n4_iiii */
					op->nmid=BJX2_NMID_TST;
					op->fmid=BJX2_FMID_IMMREG;
					op->Run=BJX2_Op_TST_ImmReg;
					if(eq)
					{	op->nmid=BJX2_NMID_TSTQ;
						op->Run=BJX2_Op_TSTQ_ImmReg;	}
					break;

				case 0xC:	/* FC0e_19nC_iiii */
					op->nmid=BJX2_NMID_CMPEQ;
					op->fmid=BJX2_FMID_IMMREG;
					op->Run=BJX2_Op_CMPEQ_ImmReg;
					if(eq)
					{	op->nmid=BJX2_NMID_CMPQEQ;
						op->Run=BJX2_Op_CMPQEQ_ImmReg;	}
					break;
				case 0xD:	/* FC0e_19nD_iiii */
					op->nmid=BJX2_NMID_CMPHI;
					op->fmid=BJX2_FMID_IMMREG;
					op->Run=BJX2_Op_CMPHI_ImmReg;
					if(eq)
					{	op->nmid=BJX2_NMID_CMPQHI;
						op->Run=BJX2_Op_CMPQHI_ImmReg;	}
					break;
				case 0xE:	/* FC0e_19nE_iiii */
					op->nmid=BJX2_NMID_CMPGT;
					op->fmid=BJX2_FMID_IMMREG;
					op->Run=BJX2_Op_CMPGT_ImmReg;
					if(eq)
					{	op->nmid=BJX2_NMID_CMPQGT;
						op->Run=BJX2_Op_CMPQGT_ImmReg;	}
					break;
				}
				break;
			}
			break;

		}
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

		case 0x7:
			switch((opw2>>12)&15)
			{
			case 0x0:
				op->rn=rn_i24;
				op->rm=BJX2_REG_PC;
				op->imm=imm24;
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_REGSTPCDISP;
				op->Run=BJX2_Op_MOVB_RegStPcDisp;
				break;
			case 0x1:
				op->rn=rn_i24;
				op->rm=BJX2_REG_PC;
				op->imm=imm24;
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_REGSTPCDISP;
				op->Run=BJX2_Op_MOVW_RegStPcDisp;
				break;
			case 0x2:
				op->rn=rn_i24;
				op->rm=BJX2_REG_PC;
				op->imm=imm24;
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_REGSTPCDISP;
				op->Run=BJX2_Op_MOVL_RegStPcDisp;
				break;
			case 0x3:
				op->rn=rn_i24;
				op->rm=BJX2_REG_PC;
				op->imm=imm24;
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_REGSTPCDISP;
				op->Run=BJX2_Op_MOVQ_RegStPcDisp;
				break;

			case 0x8:
				op->rn=rn_i24;
				op->rm=BJX2_REG_PC;
				op->imm=imm24;
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_LDPCDISPREG;
				op->Run=BJX2_Op_MOVB_LdPcDispReg;
				break;
			case 0x9:
				op->rn=rn_i24;
				op->rm=BJX2_REG_PC;
				op->imm=imm24;
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_LDPCDISPREG;
				op->Run=BJX2_Op_MOVW_LdPcDispReg;
				break;
			case 0xA:
				op->rn=rn_i24;
				op->rm=BJX2_REG_PC;
				op->imm=imm24;
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_LDPCDISPREG;
				op->Run=BJX2_Op_MOVL_LdPcDispReg;
				break;
			case 0xB:
				op->rn=rn_i24;
				op->rm=BJX2_REG_PC;
				op->imm=imm24;
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_LDPCDISPREG;
				op->Run=BJX2_Op_MOVQ_LdPcDispReg;
				break;

			case 0xC:
				op->rn=rn_i24;
				op->rm=BJX2_REG_PC;
				op->imm=imm24;
				op->nmid=BJX2_NMID_MOVUB;
				op->fmid=BJX2_FMID_LDPCDISPREG;
				op->Run=BJX2_Op_MOVUB_LdPcDispReg;
				break;
			case 0xD:
				op->rn=rn_i24;
				op->rm=BJX2_REG_PC;
				op->imm=imm24;
				op->nmid=BJX2_NMID_MOVUW;
				op->fmid=BJX2_FMID_LDPCDISPREG;
				op->Run=BJX2_Op_MOVUW_LdPcDispReg;
				break;
			case 0xE:
				op->rn=rn_i24;
				op->rm=BJX2_REG_PC;
				op->imm=imm24;
				op->nmid=BJX2_NMID_MOVUL;
				op->fmid=BJX2_FMID_LDPCDISPREG;
				op->Run=BJX2_Op_MOVUL_LdPcDispReg;
				break;
			case 0xF:
				op->rn=rn_i24;
				op->rm=BJX2_REG_PC;
				op->imm=imm24;
				op->nmid=BJX2_NMID_LEAB;
				op->fmid=BJX2_FMID_LDPCDISPREG;
				op->Run=BJX2_Op_LEAB_LdPcDispReg;
				break;
			}
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
	case 0xF:
		op->imm=imm32;
		op->rn=rn_i32;
		op->nmid=BJX2_NMID_MOV;
		op->fmid=BJX2_FMID_IMMXREG;
		op->Run=BJX2_Op_MOV_ImmxReg;
		break;

	default:
		ret=-1;
		break;
	}
	return(ret);
}
