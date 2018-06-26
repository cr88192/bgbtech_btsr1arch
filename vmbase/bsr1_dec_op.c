int BTSR1_DecodeOpcode_CheckTwoWord(BTSR1_Context *ctx,
	BTSR1_Opcode *op, btsr1_addr addr, int opw)
{
	int opw2;

//	opw=BTSR1_MemGetWord(ctx, addr);
	opw2=BTSR1_MemGetWord(ctx, addr+2);

#if 1
	op->opn2=opw2;
	switch((opw2>>12)&15)
	{
	case 0x0:
		op->rn=(opw2>>4)&15;
		op->rm=(opw2   )&15;
		switch((opw2>>8)&15)
		{
		case 0x4:
			op->fl|=BTSR1_OPFL_TWOWORD;
			op->nmid=BTSR1_NMID_MOVB;
			op->fmid=BTSR1_FMID_REGSTREGDISP;
			op->Run=BTSR1_Op_MOVB_RegStRegDisp;
			break;
		case 0x5:
			op->fl|=BTSR1_OPFL_TWOWORD;
			op->nmid=BTSR1_NMID_MOVW;
			op->fmid=BTSR1_FMID_REGSTREGDISP;
			op->Run=BTSR1_Op_MOVW_RegStRegDisp;
			break;
		case 0x6:
			op->fl|=BTSR1_OPFL_TWOWORD;
			op->nmid=BTSR1_NMID_MOVD;
			op->fmid=BTSR1_FMID_REGSTREGDISP;
			op->Run=BTSR1_Op_MOVD_RegStRegDisp;
			break;
		case 0x7:
			op->fl|=BTSR1_OPFL_TWOWORD;
			op->nmid=BTSR1_NMID_MOVUB;
			op->fmid=BTSR1_FMID_LDREGDISPREG;
			op->Run=BTSR1_Op_MOVUB_LdRegDispReg;
			break;

		case 0xC:
			op->fl|=BTSR1_OPFL_TWOWORD;
			op->nmid=BTSR1_NMID_MOVB;
			op->fmid=BTSR1_FMID_LDREGDISPREG;
			op->Run=BTSR1_Op_MOVB_LdRegDispReg;
			break;
		case 0xD:
			op->fl|=BTSR1_OPFL_TWOWORD;
			op->nmid=BTSR1_NMID_MOVW;
			op->fmid=BTSR1_FMID_LDREGDISPREG;
			op->Run=BTSR1_Op_MOVW_LdRegDispReg;
			break;
		case 0xE:
			op->fl|=BTSR1_OPFL_TWOWORD;
			op->nmid=BTSR1_NMID_MOVD;
			op->fmid=BTSR1_FMID_LDREGDISPREG;
			op->Run=BTSR1_Op_MOVD_LdRegDispReg;
			break;
		case 0xF:
			op->fl|=BTSR1_OPFL_TWOWORD;
			op->nmid=BTSR1_NMID_MOVUW;
			op->fmid=BTSR1_FMID_LDREGDISPREG;
			op->Run=BTSR1_Op_MOVUW_LdRegDispReg;
			break;
		}
		break;

	case 0x3:
		switch((opw2>>8)&15)
		{
		case 0x1:
			switch(opw2&15)
			{
			case 0x4:
				op->fl|=BTSR1_OPFL_TWOWORD;
				op->rn=(opw2>>4)&15;
				op->nmid=BTSR1_NMID_TST;
				op->fmid=BTSR1_FMID_IMMREG;
				op->Run=BTSR1_Op_TST_ImmReg;
				break;
			case 0x5:
				op->fl|=BTSR1_OPFL_TWOWORD;
				op->rn=(opw2>>4)&15;
				op->nmid=BTSR1_NMID_AND;
				op->fmid=BTSR1_FMID_IMMREG;
				op->Run=BTSR1_Op_AND_ImmReg;
				break;
			}
			break;
			
		}
		break;

	case 0x4:
		switch((opw2>>8)&15)
		{
		case 0x0:
			if(((opw2>>4)&15)==15)
			{
#if 1
				op->fl|=BTSR1_OPFL_TWOWORD|BTSR1_OPFL_CTRLF;
				op->imm=(op->imm<<4)|(opw2&15);
				op->imm+=1;
				op->nmid=BTSR1_NMID_BRA;
				op->fmid=BTSR1_FMID_PCDISP;
				op->Run=BTSR1_Op_BRA_PcDisp;
#endif
				break;
			}
			op->fl|=BTSR1_OPFL_TWOWORD;
			op->imm=(op->imm<<4)|(opw2&15);
			op->imm+=2;
			op->rn=(opw2>>4)&15;
			op->nmid=BTSR1_NMID_MOVB;
			op->fmid=BTSR1_FMID_REGSTPCDISP;
			op->Run=BTSR1_Op_MOVB_RegStPcDisp;
			break;
#if 1
		case 0x1:
			if(((opw2>>4)&15)==15)
			{
#if 1
				op->fl|=BTSR1_OPFL_TWOWORD|BTSR1_OPFL_CTRLF;
				op->imm=(op->imm<<4)|(opw2&15);
				op->imm+=1;
				op->nmid=BTSR1_NMID_BSR;
				op->fmid=BTSR1_FMID_PCDISP;
				op->Run=BTSR1_Op_BSR_PcDisp2;
#endif
				break;
			}
			op->fl|=BTSR1_OPFL_TWOWORD;
			op->imm=(op->imm<<4)|(opw2&15);
			op->rn=(opw2>>4)&15;
			op->imm+=2;
			op->nmid=BTSR1_NMID_MOVW;
			op->fmid=BTSR1_FMID_REGSTPCDISP;
			op->Run=BTSR1_Op_MOVW_RegStPcDisp;
			break;
		case 0x2:
			if(((opw2>>4)&15)==15)
				break;
			op->fl|=BTSR1_OPFL_TWOWORD;
			op->imm=(op->imm<<4)|(opw2&15);
			op->rn=(opw2>>4)&15;
			op->imm+=2;
			op->nmid=BTSR1_NMID_MOVD;
			op->fmid=BTSR1_FMID_REGSTPCDISP;
			op->Run=BTSR1_Op_MOVD_RegStPcDisp;
			break;
		case 0x3:
			if(((opw2>>4)&15)==15)
				break;
			op->fl|=BTSR1_OPFL_TWOWORD;
			op->imm=(op->imm<<4)|(opw2&15);
			op->rn=(opw2>>4)&15;
			op->imm+=2;
			op->nmid=BTSR1_NMID_MOVUB;
			op->fmid=BTSR1_FMID_LDPCDISPREG;
			op->Run=BTSR1_Op_MOVUB_LdPcDispReg;
			break;
#endif
		case 0x4:
			if(((opw2>>4)&15)==15)
			{
#if 1
				op->fl|=BTSR1_OPFL_TWOWORD|BTSR1_OPFL_CTRLF;
				op->imm=(op->imm<<4)|(opw2&15);
				op->imm+=1;
				op->nmid=BTSR1_NMID_BT;
				op->fmid=BTSR1_FMID_PCDISP;
				op->Run=BTSR1_Op_BT_PcDisp;
#endif
				break;
			}
			op->fl|=BTSR1_OPFL_TWOWORD;
			op->imm=(op->imm<<4)|(opw2&15);
			op->rn=(opw2>>4)&15;
			op->imm+=2;
			op->nmid=BTSR1_NMID_MOVB;
			op->fmid=BTSR1_FMID_LDPCDISPREG;
			op->Run=BTSR1_Op_MOVB_LdPcDispReg;
			break;
		case 0x5:
			if(((opw2>>4)&15)==15)
			{
#if 1
				op->fl|=BTSR1_OPFL_TWOWORD|BTSR1_OPFL_CTRLF;
				op->imm=(op->imm<<4)|(opw2&15);
				op->imm+=1;
				op->nmid=BTSR1_NMID_BF;
				op->fmid=BTSR1_FMID_PCDISP;
				op->Run=BTSR1_Op_BF_PcDisp;
#endif
				break;
			}
			op->fl|=BTSR1_OPFL_TWOWORD;
			op->imm=(op->imm<<4)|(opw2&15);
			op->rn=(opw2>>4)&15;
			op->imm+=2;
			op->nmid=BTSR1_NMID_MOVW;
			op->fmid=BTSR1_FMID_LDPCDISPREG;
			op->Run=BTSR1_Op_MOVW_LdPcDispReg;
			break;
		case 0x6:
			if(((opw2>>4)&15)==15)
				break;
			op->fl|=BTSR1_OPFL_TWOWORD;
			op->imm=(op->imm<<4)|(opw2&15);
			op->rn=(opw2>>4)&15;
			op->imm+=2;
			op->nmid=BTSR1_NMID_MOVD;
			op->fmid=BTSR1_FMID_LDPCDISPREG;
			op->Run=BTSR1_Op_MOVD_LdPcDispReg;
			break;
		case 0x7:
			if(((opw2>>4)&15)==15)
				break;
			op->fl|=BTSR1_OPFL_TWOWORD;
			op->imm=(op->imm<<4)|(opw2&15);
			op->rn=(opw2>>4)&15;
			op->imm+=2;
			op->nmid=BTSR1_NMID_MOVUW;
			op->fmid=BTSR1_FMID_LDPCDISPREG;
			op->Run=BTSR1_Op_MOVUW_LdPcDispReg;
			break;

		case 0x8:
			op->fl|=BTSR1_OPFL_TWOWORD;
			op->imm=(op->imm<<4)|(opw2&15);
			op->rn=(opw2>>4)&15;
//				op->rm=opw&15;
			op->nmid=BTSR1_NMID_MOV;
			op->fmid=BTSR1_FMID_IMMREG;
			op->Run=BTSR1_Op_MOV_ImmReg;
			break;
		}
		break;
	default:
		break;
	}
#endif
	return(0);
}

int BTSR1_DecodeOpcodeForAddr(BTSR1_Context *ctx,
	BTSR1_Opcode *op, btsr1_addr addr)
{
	int ret;
	int opw, opw2;

	opw=BTSR1_MemGetWord(ctx, addr);
	opw2=BTSR1_MemGetWord(ctx, addr+2);
	
	op->opn=opw;
	op->pc=addr;
	op->cyc=-1;
	
	ret=0;
	switch((opw>>12)&15)
	{
	case 0x0:	/* 0zzz */
		op->rn=(opw>>4)&15;
		op->rm=opw&15;
		switch((opw>>8)&15)
		{
		case 0x0:	/* 00zz */
			op->nmid=BTSR1_NMID_MOVB;
			op->fmid=BTSR1_FMID_REGSTREG;
			op->Run=BTSR1_Op_MOVB_RegStReg;
			break;
		case 0x1:	/* 01zz */
			op->nmid=BTSR1_NMID_MOVW;
			op->fmid=BTSR1_FMID_REGSTREG;
			op->Run=BTSR1_Op_MOVW_RegStReg;
			break;
		case 0x2:	/* 02zz */
			op->nmid=BTSR1_NMID_MOVD;
			op->fmid=BTSR1_FMID_REGSTREG;
			op->Run=BTSR1_Op_MOVD_RegStReg;
			break;
		case 0x3:	/* 03zz */
			op->nmid=BTSR1_NMID_MOVUB;
			op->fmid=BTSR1_FMID_LDREGREG;
			op->Run=BTSR1_Op_MOVUB_LdRegReg;
			break;
		case 0x4:	/* 04zz */
			op->nmid=BTSR1_NMID_MOVB;
			op->fmid=BTSR1_FMID_REGSTDRREG;
			op->Run=BTSR1_Op_MOVB_RegStDrReg;
			break;
		case 0x5:	/* 05zz */
			op->nmid=BTSR1_NMID_MOVW;
			op->fmid=BTSR1_FMID_REGSTDRREG;
			op->Run=BTSR1_Op_MOVW_RegStDrReg;
			break;
		case 0x6:	/* 06zz */
			op->nmid=BTSR1_NMID_MOVD;
			op->fmid=BTSR1_FMID_REGSTDRREG;
			op->Run=BTSR1_Op_MOVD_RegStDrReg;
			break;
		case 0x7:	/* 07zz */
			op->nmid=BTSR1_NMID_MOVUB;
			op->fmid=BTSR1_FMID_LDDRREGREG;
			op->Run=BTSR1_Op_MOVUB_LdDrRegReg;
			break;
		case 0x8:	/* 08zz */
			op->nmid=BTSR1_NMID_MOVB;
			op->fmid=BTSR1_FMID_LDREGREG;
			op->Run=BTSR1_Op_MOVB_LdRegReg;
			break;
		case 0x9:	/* 09zz */
			op->nmid=BTSR1_NMID_MOVW;
			op->fmid=BTSR1_FMID_LDREGREG;
			op->Run=BTSR1_Op_MOVW_LdRegReg;
			break;
		case 0xA:	/* 0Azz */
			op->nmid=BTSR1_NMID_MOVD;
			op->fmid=BTSR1_FMID_LDREGREG;
			op->Run=BTSR1_Op_MOVD_LdRegReg;
			break;
		case 0xB:	/* 0Bzz */
			op->nmid=BTSR1_NMID_MOVUW;
			op->fmid=BTSR1_FMID_LDREGREG;
			op->Run=BTSR1_Op_MOVUW_LdRegReg;
			break;
		case 0xC:	/* 0Czz */
			op->nmid=BTSR1_NMID_MOVB;
			op->fmid=BTSR1_FMID_LDDRREGREG;
			op->Run=BTSR1_Op_MOVB_LdDrRegReg;
			break;
		case 0xD:	/* 0Dzz */
			op->nmid=BTSR1_NMID_MOVW;
			op->fmid=BTSR1_FMID_LDDRREGREG;
			op->Run=BTSR1_Op_MOVW_LdDrRegReg;
			break;
		case 0xE:	/* 0Ezz */
			op->nmid=BTSR1_NMID_MOVD;
			op->fmid=BTSR1_FMID_LDDRREGREG;
			op->Run=BTSR1_Op_MOVD_LdDrRegReg;
			break;
		case 0xF:	/* 0Fzz */
			op->nmid=BTSR1_NMID_MOVUB;
			op->fmid=BTSR1_FMID_LDDRREGREG;
			op->Run=BTSR1_Op_MOVUB_LdDrRegReg;
			break;
		}
		break;

	case 0x1:	/* 1zzz */
		op->rn=(opw>>4)&15;
		op->rm=opw&15;
		switch((opw>>8)&15)
		{
		case 0x0:	/* 10zz */
			op->nmid=BTSR1_NMID_ADD;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_ADD_RegReg;
			break;
		case 0x1:	/* 11zz */
			op->nmid=BTSR1_NMID_SUB;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_SUB_RegReg;
			break;
		case 0x2:	/* 12zz */
			op->nmid=BTSR1_NMID_ADC;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_ADC_RegReg;
			break;
		case 0x3:	/* 13zz */
			op->nmid=BTSR1_NMID_SBB;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_SBB_RegReg;
			break;
		case 0x4:	/* 14zz */
			op->nmid=BTSR1_NMID_TST;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_TST_RegReg;
			break;
		case 0x5:	/* 15zz */
			op->nmid=BTSR1_NMID_AND;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_AND_RegReg;
			break;
		case 0x6:	/* 16zz */
			op->nmid=BTSR1_NMID_OR;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_OR_RegReg;
			break;
		case 0x7:	/* 17zz */
			op->nmid=BTSR1_NMID_XOR;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_XOR_RegReg;
			break;
		case 0x8:	/* 18zz */
			op->nmid=BTSR1_NMID_MOV;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_MOV_RegReg;
			break;
		case 0x9:	/* 19zz */
			op->nmid=BTSR1_NMID_MULS;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_MULS_RegReg;
			break;
		case 0xA:	/* 1Azz */
			op->rn|=16;
			op->nmid=BTSR1_NMID_MOV;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_MOV_RegReg;
			break;
		case 0xB:	/* 1Bzz */
			op->rm|=16;
			op->nmid=BTSR1_NMID_MOV;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_MOV_RegReg;
			break;
		case 0xC:	/* 1Czz */
			op->nmid=BTSR1_NMID_CMPEQ;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_CMPEQ_RegReg;
			break;
		case 0xD:	/* 1Dzz */
			op->nmid=BTSR1_NMID_CMPHI;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_CMPHI_RegReg;
			break;
		case 0xE:	/* 1Ezz */
			op->nmid=BTSR1_NMID_CMPGT;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_CMPGT_RegReg;
			break;
		case 0xF:	/* 1Fzz */
			op->nmid=BTSR1_NMID_MULU;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_MULU_RegReg;
			break;
		}
		break;

	case 0x2:	/* 2zzz */
		op->rn=(opw>>4)&15;
		op->imm=(sbyte)opw;
		switch((opw>>8)&15)
		{
		case 0x0:	/* 20zz */
			op->nmid=BTSR1_NMID_BRA;
			op->fmid=BTSR1_FMID_PCDISP;
			op->Run=BTSR1_Op_BRA_PcDisp;
			op->fl|=BTSR1_OPFL_CTRLF;
			break;
		case 0x1:	/* 21zz */
			op->nmid=BTSR1_NMID_BSR;
			op->fmid=BTSR1_FMID_PCDISP;
			op->Run=BTSR1_Op_BSR_PcDisp;
			op->fl|=BTSR1_OPFL_CTRLF;
			break;
		case 0x2:	/* 22zz */
			op->nmid=BTSR1_NMID_BT;
			op->fmid=BTSR1_FMID_PCDISP;
			op->Run=BTSR1_Op_BT_PcDisp;
			op->fl|=BTSR1_OPFL_CTRLF;
			break;
		case 0x3:	/* 23zz */
			op->nmid=BTSR1_NMID_BF;
			op->fmid=BTSR1_FMID_PCDISP;
			op->Run=BTSR1_Op_BF_PcDisp;
			op->fl|=BTSR1_OPFL_CTRLF;
			break;
		case 0x4:	/* 24zz */
			op->nmid=BTSR1_NMID_LDIZ;
			op->fmid=BTSR1_FMID_IMM;
			op->Run=BTSR1_Op_LDIZ_Imm;
			BTSR1_DecodeOpcode_CheckTwoWord(ctx, op, addr, opw);
			break;
		case 0x5:	/* 25zz */
			op->nmid=BTSR1_NMID_LDIN;
			op->fmid=BTSR1_FMID_IMM;
			op->Run=BTSR1_Op_LDIN_Imm;
			BTSR1_DecodeOpcode_CheckTwoWord(ctx, op, addr, opw);
			break;
		case 0x6:	/* 26zz */
			op->nmid=BTSR1_NMID_LDISH;
			op->fmid=BTSR1_FMID_IMM;
			op->Run=BTSR1_Op_LDISH_Imm;
			break;
//		case 0x7:	/* 27zz */
//			op->rn=BTSR1_REG_SP;
//			op->rm=BTSR1_REG_SP;
//			op->nmid=BTSR1_NMID_LEAD;
//			op->fmid=BTSR1_FMID_LDREGDISPREG;
//			op->Run=BTSR1_Op_LEAD_LdRegDispReg;
//			break;
		case 0x8:	/* 28zz */
			op->imm=opw&15;
			op->rm=op->rn;
			op->rn=BTSR1_REG_SP;
			op->nmid=BTSR1_NMID_MOVD;
			op->fmid=BTSR1_FMID_REGSTREGDISP;
			op->Run=BTSR1_Op_MOVD_RegStRegDisp;
			break;
		case 0x9:	/* 29zz */
#if 0
			op->imm=opw&15;
			op->rm=op->rn;
			op->rn=BTSR1_REG_SP;
			op->nmid=BTSR1_NMID_MOVW;
			op->fmid=BTSR1_FMID_REGSTREGDISP;
			op->Run=BTSR1_Op_MOVW_RegStRegDisp;
#endif
#if 1
			op->imm=16+(opw&15);
			op->rm=op->rn;
			op->rn=BTSR1_REG_SP;
			op->nmid=BTSR1_NMID_MOVD;
			op->fmid=BTSR1_FMID_REGSTREGDISP;
			op->Run=BTSR1_Op_MOVD_RegStRegDisp;
#endif
			break;
		case 0xA:	/* 2Azz */
			op->imm=opw&15;
			op->rm=BTSR1_REG_SP;
			op->nmid=BTSR1_NMID_MOVD;
			op->fmid=BTSR1_FMID_LDREGDISPREG;
			op->Run=BTSR1_Op_MOVD_LdRegDispReg;
			break;
		case 0xB:	/* 2Bzz */
#if 0
			op->imm=opw&15;
			op->rm=BTSR1_REG_SP;
			op->nmid=BTSR1_NMID_MOVW;
			op->fmid=BTSR1_FMID_LDREGDISPREG;
			op->Run=BTSR1_Op_MOVW_LdRegDispReg;
#endif
#if 1
			op->imm=16+(opw&15);
			op->rm=BTSR1_REG_SP;
			op->nmid=BTSR1_NMID_MOVD;
			op->fmid=BTSR1_FMID_LDREGDISPREG;
			op->Run=BTSR1_Op_MOVD_LdRegDispReg;
#endif
			break;

		case 0xC:	/* 2Czz */
			op->imm=opw&15;
			op->nmid=BTSR1_NMID_CMPEQ;
			op->fmid=BTSR1_FMID_IMMREG;
			op->Run=BTSR1_Op_CMPEQ_ImmReg;
			break;
		case 0xD:	/* 2Dzz */
			op->imm=opw|(~15);
			op->nmid=BTSR1_NMID_CMPEQ;
			op->fmid=BTSR1_FMID_IMMREG;
			op->Run=BTSR1_Op_CMPEQ_ImmReg;
			break;
		case 0xE:	/* 2Ezz */
			op->imm=opw|(~15);
			op->nmid=BTSR1_NMID_CMPGT;
			op->fmid=BTSR1_FMID_IMMREG;
			op->Run=BTSR1_Op_CMPGT_ImmReg;
			break;
		case 0xF:	/* 2Fzz */
			op->imm=opw|(~15);
			op->nmid=BTSR1_NMID_CMPGE;
			op->fmid=BTSR1_FMID_IMMREG;
			op->Run=BTSR1_Op_CMPGE_ImmReg;
			break;
		}
		break;

	case 0x3:	/* 3zzz */
		op->rn=(opw>>4)&15;
		switch((opw>>8)&15)
		{
		case 0x0:	/* 30zz */
			switch(opw&15)
			{
			case 0x0:	/* 30z0 */
				op->fmid=BTSR1_FMID_Z;
				
				switch((opw>>4)&15)
				{
				case 0x0:
					op->nmid=BTSR1_NMID_NOP;
					op->Run=BTSR1_Op_NOP_None;
					break;
				case 0x1:
					op->nmid=BTSR1_NMID_RTS;
					op->Run=BTSR1_Op_RTS_None;
					op->fl|=BTSR1_OPFL_CTRLF;
					break;

				case 0x2:
					op->nmid=BTSR1_NMID_SLEEP;
					op->Run=BTSR1_Op_SLEEP_None;
					op->fl|=BTSR1_OPFL_CTRLF;
					break;
				case 0x3:
					op->nmid=BTSR1_NMID_BREAK;
					op->Run=BTSR1_Op_BREAK_None;
					op->fl|=BTSR1_OPFL_CTRLF;
					break;

				case 0x4:
					op->nmid=BTSR1_NMID_CLRT;
					op->Run=BTSR1_Op_CLRT_None;
					break;
				case 0x5:
					op->nmid=BTSR1_NMID_SETT;
					op->Run=BTSR1_Op_SETT_None;
					break;
				case 0x6:
					op->nmid=BTSR1_NMID_CLRS;
					op->Run=BTSR1_Op_CLRS_None;
					break;
				case 0x7:
					op->nmid=BTSR1_NMID_SETS;
					op->Run=BTSR1_Op_SETS_None;
					break;
				case 0x8:
					op->nmid=BTSR1_NMID_NOTT;
					op->Run=BTSR1_Op_NOTT_None;
					break;
				case 0x9:
					op->nmid=BTSR1_NMID_NOTS;
					op->Run=BTSR1_Op_NOTS_None;
					break;

				case 0xA:
					op->fmid=BTSR1_FMID_IMM;
					op->imm=0x0000;
					op->nmid=BTSR1_NMID_LDISH16;
					op->Run=BTSR1_Op_LDISH16_Imm;
					break;
				case 0xB:
					op->fmid=BTSR1_FMID_IMM;
					op->imm=0xFFFF;
					op->nmid=BTSR1_NMID_LDISH16;
					op->Run=BTSR1_Op_LDISH16_Imm;
					break;
				case 0xC:
					op->nmid=BTSR1_NMID_RTE;
					op->Run=BTSR1_Op_RTE_None;
					op->fl|=BTSR1_OPFL_CTRLF;
					break;
				}
				break;

			case 0x1:	/* 30z1 */
				break;

			case 0x2:	/* 30z2 */
				switch((opw>>4)&15)
				{
				case 0x0:
					op->nmid=BTSR1_NMID_BRA;
					op->fmid=BTSR1_FMID_PCDR;
					op->Run=BTSR1_Op_BRA_PcDr;
					op->fl|=BTSR1_OPFL_CTRLF;
					break;
				case 0x1:
					op->nmid=BTSR1_NMID_BSR;
					op->fmid=BTSR1_FMID_PCDR;
					op->Run=BTSR1_Op_BSR_PcDr;
					op->fl|=BTSR1_OPFL_CTRLF;
					break;
				case 0x2:
					op->nmid=BTSR1_NMID_BT;
					op->fmid=BTSR1_FMID_PCDR;
					op->Run=BTSR1_Op_BT_PcDr;
					op->fl|=BTSR1_OPFL_CTRLF;
					break;
				case 0x3:
					op->nmid=BTSR1_NMID_BF;
					op->fmid=BTSR1_FMID_PCDR;
					op->Run=BTSR1_Op_BF_PcDr;
					op->fl|=BTSR1_OPFL_CTRLF;
					break;

//				case 0x7:
//					op->rn=BTSR1_REG_SP;
//					op->rm=BTSR1_REG_SP;
//					op->nmid=BTSR1_NMID_LEAD;
//					op->fmid=BTSR1_FMID_LDDRREGREG;
//					op->Run=BTSR1_Op_LEAD_LdDrRegReg;
//					break;

				case 0x8:
					op->fmid=BTSR1_FMID_IMM;
					op->imm=0x000;
					op->nmid=BTSR1_NMID_LDISH12;
					op->Run=BTSR1_Op_LDISH12_Imm;
					break;
				case 0x9:
					op->fmid=BTSR1_FMID_IMM;
					op->imm=0xFFF;
					op->nmid=BTSR1_NMID_LDISH12;
					op->Run=BTSR1_Op_LDISH12_Imm;
					break;
				case 0xA:
					op->fmid=BTSR1_FMID_IMM;
					op->imm=0x00000;
					op->nmid=BTSR1_NMID_LDISH20;
					op->Run=BTSR1_Op_LDISH20_Imm;
					break;
				case 0xB:
					op->fmid=BTSR1_FMID_IMM;
					op->imm=0xFFFFF;
					op->nmid=BTSR1_NMID_LDISH20;
					op->Run=BTSR1_Op_LDISH20_Imm;
					break;
				}
				break;

			case 0x3:	/* 30z3 */
				break;

			case 0x4:	/* 30z4 */
				op->nmid=BTSR1_NMID_MOVB;
				op->fmid=BTSR1_FMID_REGSTDRPC;
				op->Run=BTSR1_Op_MOVB_RegStDrPc;
				break;
			case 0x5:	/* 30z5 */
				op->nmid=BTSR1_NMID_MOVW;
				op->fmid=BTSR1_FMID_REGSTDRPC;
				op->Run=BTSR1_Op_MOVW_RegStDrPc;
				break;
			case 0x6:	/* 30z6 */
				op->nmid=BTSR1_NMID_MOVD;
				op->fmid=BTSR1_FMID_REGSTDRPC;
				op->Run=BTSR1_Op_MOVD_RegStDrPc;
				break;
			case 0x7:	/* 30z7 */
				op->nmid=BTSR1_NMID_MOVUB;
				op->fmid=BTSR1_FMID_LDDRPCREG;
				op->Run=BTSR1_Op_MOVUB_LdDrPcReg;
				break;
			case 0x8:	/* 30z8 */
				op->nmid=BTSR1_NMID_PUSH;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_PUSH_Reg;
				break;
			case 0x9:	/* 30z9 */
				op->rn+=16;
				op->nmid=BTSR1_NMID_PUSH;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_PUSH_Reg;
				break;
			case 0xA:	/* 30zA */
				op->nmid=BTSR1_NMID_POP;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_POP_Reg;
				break;
			case 0xB:	/* 30zB */
				if((opw&0x00F0)==0x0000)
				{
					op->nmid=BTSR1_NMID_RET;
					op->fmid=BTSR1_FMID_Z;
					op->Run=BTSR1_Op_RET_None;
					op->fl|=BTSR1_OPFL_CTRLF;
					break;
				}
			
				op->rn+=16;
				op->nmid=BTSR1_NMID_POP;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_POP_Reg;
				break;
			case 0xC:	/* 30zC */
				op->nmid=BTSR1_NMID_MOVB;
				op->fmid=BTSR1_FMID_LDDRPCREG;
				op->Run=BTSR1_Op_MOVB_LdDrPcReg;
				break;
			case 0xD:	/* 30zD */
				op->nmid=BTSR1_NMID_MOVW;
				op->fmid=BTSR1_FMID_LDDRPCREG;
				op->Run=BTSR1_Op_MOVW_LdDrPcReg;
				break;
			case 0xE:	/* 30zE */
				op->nmid=BTSR1_NMID_MOVD;
				op->fmid=BTSR1_FMID_LDDRPCREG;
				op->Run=BTSR1_Op_MOVD_LdDrPcReg;
				break;
			case 0xF:	/* 30zF */
				op->nmid=BTSR1_NMID_MOVUW;
				op->fmid=BTSR1_FMID_LDDRPCREG;
				op->Run=BTSR1_Op_MOVUW_LdDrPcReg;
				break;
			}
			break;

		case 0x1:	/* 31zz */
			op->rn=(opw>>4)&15;
			op->rm=BTSR1_REG_DR;
			switch(opw&15)
			{
			case 0x0:	/* 31z0 */
				op->nmid=BTSR1_NMID_ADD;
				op->fmid=BTSR1_FMID_REGREG;
				op->Run=BTSR1_Op_ADD_RegReg;
				break;
			case 0x1:	/* 31z1 */
				op->nmid=BTSR1_NMID_SUB;
				op->fmid=BTSR1_FMID_REGREG;
				op->Run=BTSR1_Op_SUB_RegReg;
				break;
			case 0x2:	/* 31z2 */
				op->nmid=BTSR1_NMID_ADC;
				op->fmid=BTSR1_FMID_REGREG;
				op->Run=BTSR1_Op_ADC_RegReg;
				break;
			case 0x3:	/* 31z3 */
				op->nmid=BTSR1_NMID_SBB;
				op->fmid=BTSR1_FMID_REGREG;
				op->Run=BTSR1_Op_SBB_RegReg;
				break;
			case 0x4:	/* 31z4 */
				op->nmid=BTSR1_NMID_TST;
				op->fmid=BTSR1_FMID_REGREG;
				op->Run=BTSR1_Op_TST_RegReg;
				break;
			case 0x5:	/* 31z5 */
				op->nmid=BTSR1_NMID_AND;
				op->fmid=BTSR1_FMID_REGREG;
				op->Run=BTSR1_Op_AND_RegReg;
				break;
			case 0x6:	/* 31z6 */
				op->nmid=BTSR1_NMID_OR;
				op->fmid=BTSR1_FMID_REGREG;
				op->Run=BTSR1_Op_OR_RegReg;
				break;
			case 0x7:	/* 31z7 */
				op->nmid=BTSR1_NMID_XOR;
				op->fmid=BTSR1_FMID_REGREG;
				op->Run=BTSR1_Op_XOR_RegReg;
				break;
			case 0x8:	/* 31z8 */
				op->nmid=BTSR1_NMID_CMPPL;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_CMPPL_Reg;
				break;
			case 0x9:	/* 31z9 */
				op->nmid=BTSR1_NMID_CMPPZ;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_CMPPZ_Reg;
				break;
			case 0xA:	/* 31zA */
				op->nmid=BTSR1_NMID_LEAW;
				op->fmid=BTSR1_FMID_LDDRPCREG;
				op->Run=BTSR1_Op_LEAW_LdDrPcReg;
				break;
			case 0xB:	/* 31zB */
				op->nmid=BTSR1_NMID_LEAD;
				op->fmid=BTSR1_FMID_LDDRPCREG;
				op->Run=BTSR1_Op_LEAD_LdDrPcReg;
				break;
			case 0xC:	/* 31zC */
				op->nmid=BTSR1_NMID_CMPEQ;
				op->fmid=BTSR1_FMID_REGREG;
				op->Run=BTSR1_Op_CMPEQ_RegReg;
				break;
			case 0xD:	/* 31zD */
				op->nmid=BTSR1_NMID_CMPHI;
				op->fmid=BTSR1_FMID_REGREG;
				op->Run=BTSR1_Op_CMPHI_RegReg;
				break;
			case 0xE:	/* 31zE */
				op->nmid=BTSR1_NMID_CMPGT;
				op->fmid=BTSR1_FMID_REGREG;
				op->Run=BTSR1_Op_CMPGT_RegReg;
				break;
			case 0xF:	/* 31zF */
				op->nmid=BTSR1_NMID_LEAB;
				op->fmid=BTSR1_FMID_LDDRPCREG;
				op->Run=BTSR1_Op_LEAB_LdDrPcReg;
				break;
			}
			break;

		case 0x2:	/* 32zz */
			op->rn=(opw>>4)&15;
			switch(opw&15)
			{
			case 0x0:	/* 32z0 */
				op->nmid=BTSR1_NMID_BRA;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_BRA_Reg;
				op->fl|=BTSR1_OPFL_CTRLF;
				break;
			case 0x1:	/* 32z1 */
				op->nmid=BTSR1_NMID_BSR;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_BSR_Reg;
				op->fl|=BTSR1_OPFL_CTRLF;
				break;
			case 0x2:	/* 32z2 */
				op->nmid=BTSR1_NMID_BT;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_BT_Reg;
				op->fl|=BTSR1_OPFL_CTRLF;
				break;
			case 0x3:	/* 32z3 */
				op->nmid=BTSR1_NMID_BF;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_BF_Reg;
				op->fl|=BTSR1_OPFL_CTRLF;
				break;

			case 0x4:	/* 32z4 */
				op->nmid=BTSR1_NMID_EXTUB;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_EXTUB_Reg;
				break;
			case 0x5:	/* 32z5 */
				op->nmid=BTSR1_NMID_EXTUW;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_EXTUW_Reg;
				break;
			case 0x6:	/* 32z6 */
				op->nmid=BTSR1_NMID_EXTSB;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_EXTSB_Reg;
				break;
			case 0x7:	/* 32z7 */
				op->nmid=BTSR1_NMID_EXTSW;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_EXTSW_Reg;
				break;

#if 0
			case 0x8:	/* 32z8 */
				op->imm=opw&15;
				op->rm=op->rn;
				op->rn=BTSR1_REG_TBR;
				op->nmid=BTSR1_NMID_MOVD;
				op->fmid=BTSR1_FMID_REGSTDRREG;
				op->Run=BTSR1_Op_MOVD_RegStDrReg;
				break;

			case 0xA:	/* 32zA */
				op->imm=opw&15;
				op->rm=BTSR1_REG_TBR;
				op->nmid=BTSR1_NMID_MOVD;
				op->fmid=BTSR1_FMID_LDDRREGREG;
				op->Run=BTSR1_Op_MOVD_LdDrRegReg;
				break;

			case 0xC:	/* 32zC */
				op->rm=BTSR1_REG_GBR;
				op->nmid=BTSR1_NMID_LEAB;
				op->fmid=BTSR1_FMID_LDDRREGREG;
				op->Run=BTSR1_Op_LEAB_LdDrRegReg;
				break;
			case 0xD:	/* 32zD */
				op->rm=BTSR1_REG_GBR;
				op->nmid=BTSR1_NMID_LEAW;
				op->fmid=BTSR1_FMID_LDDRREGREG;
				op->Run=BTSR1_Op_LEAW_LdDrRegReg;
				break;
			case 0xE:	/* 32zE */
				op->rm=BTSR1_REG_GBR;
				op->nmid=BTSR1_NMID_LEAD;
				op->fmid=BTSR1_FMID_LDDRREGREG;
				op->Run=BTSR1_Op_LEAD_LdDrRegReg;
				break;
#endif
			}
			break;

		case 0x3:	/* 33zz */
			op->rn=(opw>>4)&15;
			switch(opw&15)
			{
			case 0x0:	/* 33z0 */
				op->nmid=BTSR1_NMID_NOT;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_NOT_Reg;
				break;
			case 0x1:	/* 33z1 */
				op->nmid=BTSR1_NMID_NEG;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_NEG_Reg;
				break;
			case 0x2:	/* 33z2 */
				op->nmid=BTSR1_NMID_NEGC;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_NEGC_Reg;
				break;

			case 0x4:	/* 33z4 */
				op->nmid=BTSR1_NMID_ROTL;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_ROTL_Reg;
				break;
			case 0x5:	/* 33z5 */
				op->nmid=BTSR1_NMID_ROTR;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_ROTR_Reg;
				break;
			case 0x6:	/* 33z6 */
				op->nmid=BTSR1_NMID_ROTCL;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_ROTCL_Reg;
				break;
			case 0x7:	/* 33z7 */
				op->nmid=BTSR1_NMID_ROTCR;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_ROTCR_Reg;
				break;

			case 0x8:	/* 33z8 */
				op->nmid=BTSR1_NMID_SHLL;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_SHLL_Reg;
				break;
			case 0x9:	/* 33z9 */
				op->nmid=BTSR1_NMID_SHLR;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_SHLR_Reg;
				break;
			case 0xA:	/* 33zA */
				op->nmid=BTSR1_NMID_SHAR;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_SHAR_Reg;
				break;
			}
			break;

		case 0x4:	/* 34zz */
			op->rn=(opw>>4)&15;
			switch(opw&15)
			{
			case 0x0:	/* 34z0 */
				op->imm=1;
				op->nmid=BTSR1_NMID_SHLL;
				op->fmid=BTSR1_FMID_IMMREG;
				op->Run=BTSR1_Op_SHLL_ImmReg;
				break;
			case 0x1:	/* 34z1 */
				op->imm=2;
				op->nmid=BTSR1_NMID_SHLL;
				op->fmid=BTSR1_FMID_IMMREG;
				op->Run=BTSR1_Op_SHLL_ImmReg;
				break;
			case 0x2:	/* 34z2 */
				op->imm=4;
				op->nmid=BTSR1_NMID_SHLL;
				op->fmid=BTSR1_FMID_IMMREG;
				op->Run=BTSR1_Op_SHLL_ImmReg;
				break;
			case 0x3:	/* 34z3 */
				op->imm=8;
				op->nmid=BTSR1_NMID_SHLL;
				op->fmid=BTSR1_FMID_IMMREG;
				op->Run=BTSR1_Op_SHLL_ImmReg;
				break;
			case 0x4:	/* 34z4 */
				op->imm=1;
				op->nmid=BTSR1_NMID_SHLR;
				op->fmid=BTSR1_FMID_IMMREG;
				op->Run=BTSR1_Op_SHLR_ImmReg;
				break;
			case 0x5:	/* 34z5 */
				op->imm=2;
				op->nmid=BTSR1_NMID_SHLR;
				op->fmid=BTSR1_FMID_IMMREG;
				op->Run=BTSR1_Op_SHLR_ImmReg;
				break;
			case 0x6:	/* 34z6 */
				op->imm=4;
				op->nmid=BTSR1_NMID_SHLR;
				op->fmid=BTSR1_FMID_IMMREG;
				op->Run=BTSR1_Op_SHLR_ImmReg;
				break;
			case 0x7:	/* 34z7 */
				op->imm=8;
				op->nmid=BTSR1_NMID_SHLR;
				op->fmid=BTSR1_FMID_IMMREG;
				op->Run=BTSR1_Op_SHLR_ImmReg;
				break;
			case 0x8:	/* 34z8 */
				op->imm=1;
				op->nmid=BTSR1_NMID_SHAR;
				op->fmid=BTSR1_FMID_IMMREG;
				op->Run=BTSR1_Op_SHAR_ImmReg;
				break;
			case 0x9:	/* 34z9 */
				op->imm=2;
				op->nmid=BTSR1_NMID_SHAR;
				op->fmid=BTSR1_FMID_IMMREG;
				op->Run=BTSR1_Op_SHAR_ImmReg;
				break;
			case 0xA:	/* 34zA */
				op->imm=4;
				op->nmid=BTSR1_NMID_SHAR;
				op->fmid=BTSR1_FMID_IMMREG;
				op->Run=BTSR1_Op_SHAR_ImmReg;
				break;
			case 0xB:	/* 34zB */
				op->imm=8;
				op->nmid=BTSR1_NMID_SHAR;
				op->fmid=BTSR1_FMID_IMMREG;
				op->Run=BTSR1_Op_SHAR_ImmReg;
				break;
			case 0xC:	/* 34zC */
				op->imm=16;
				op->nmid=BTSR1_NMID_SHLL;
				op->fmid=BTSR1_FMID_IMMREG;
				op->Run=BTSR1_Op_SHLL_ImmReg;
				break;
			case 0xD:	/* 34zD */
				op->imm=16;
				op->nmid=BTSR1_NMID_SHLR;
				op->fmid=BTSR1_FMID_IMMREG;
				op->Run=BTSR1_Op_SHLR_ImmReg;
				break;
			case 0xE:	/* 34zE */
				op->imm=16;
				op->nmid=BTSR1_NMID_SHAR;
				op->fmid=BTSR1_FMID_IMMREG;
				op->Run=BTSR1_Op_SHAR_ImmReg;
				break;
			case 0xF:	/* 34zF */
				op->imm=31;
				op->nmid=BTSR1_NMID_SHAR;
				op->fmid=BTSR1_FMID_IMMREG;
				op->Run=BTSR1_Op_SHAR_ImmReg;
				break;
			}
			break;

		case 0x5:	/* 35zz */
			op->rn=(opw>>4)&15;
			switch(opw&15)
			{
			case 0x0:	/* 35z0 */
				op->nmid=BTSR1_NMID_MOVB;
				op->fmid=BTSR1_FMID_REGSTDRABS;
				op->Run=BTSR1_Op_MOVB_RegStDrAbs;
				break;
			case 0x1:	/* 35z1 */
				op->nmid=BTSR1_NMID_MOVW;
				op->fmid=BTSR1_FMID_REGSTDRABS;
				op->Run=BTSR1_Op_MOVW_RegStDrAbs;
				break;
			case 0x2:	/* 35z2 */
				op->nmid=BTSR1_NMID_MOVD;
				op->fmid=BTSR1_FMID_REGSTDRABS;
				op->Run=BTSR1_Op_MOVD_RegStDrAbs;
				break;
			case 0x3:	/* 35z3 */
				op->nmid=BTSR1_NMID_MOVUB;
				op->fmid=BTSR1_FMID_REGSTDRABS;
				op->Run=BTSR1_Op_MOVUB_LdDrAbsReg;
				break;

#if 0
			case 0x4:	/* 35z4 */
				op->rm=(opw>>4)&15;
				op->rn=BTSR1_REG_GBR;
				op->nmid=BTSR1_NMID_MOVB;
				op->fmid=BTSR1_FMID_REGSTDRREG;
				op->Run=BTSR1_Op_MOVB_RegStDrReg;
				break;
			case 0x5:	/* 35z5 */
				op->rm=(opw>>4)&15;
				op->rn=BTSR1_REG_GBR;
				op->nmid=BTSR1_NMID_MOVW;
				op->fmid=BTSR1_FMID_REGSTDRREG;
				op->Run=BTSR1_Op_MOVW_RegStDrReg;
				break;
			case 0x6:	/* 35z6 */
				op->rm=(opw>>4)&15;
				op->rn=BTSR1_REG_GBR;
				op->nmid=BTSR1_NMID_MOVD;
				op->fmid=BTSR1_FMID_REGSTDRREG;
				op->Run=BTSR1_Op_MOVD_RegStDrReg;
				break;
			case 0x7:	/* 35z7 */
				op->rm=BTSR1_REG_GBR;
				op->rn=(opw>>4)&15;
				op->nmid=BTSR1_NMID_MOVUB;
				op->fmid=BTSR1_FMID_LDDRREGREG;
				op->Run=BTSR1_Op_MOVUB_LdDrRegReg;
				break;
#endif

			case 0x8:	/* 35z8 */
				op->nmid=BTSR1_NMID_MOVB;
				op->fmid=BTSR1_FMID_LDDRABSREG;
				op->Run=BTSR1_Op_MOVB_LdDrAbsReg;
				break;
			case 0x9:	/* 35z9 */
				op->nmid=BTSR1_NMID_MOVW;
				op->fmid=BTSR1_FMID_LDDRABSREG;
				op->Run=BTSR1_Op_MOVW_LdDrAbsReg;
				break;
			case 0xA:	/* 35zA */
				op->nmid=BTSR1_NMID_MOVD;
				op->fmid=BTSR1_FMID_LDDRABSREG;
				op->Run=BTSR1_Op_MOVD_LdDrAbsReg;
				break;
			case 0xB:	/* 35zB */
				op->nmid=BTSR1_NMID_MOVUW;
				op->fmid=BTSR1_FMID_LDDRABSREG;
				op->Run=BTSR1_Op_MOVUW_LdDrAbsReg;
				break;

#if 0
			case 0xC:	/* 35zC */
				op->rm=BTSR1_REG_GBR;
				op->rn=(opw>>4)&15;
				op->nmid=BTSR1_NMID_MOVB;
				op->fmid=BTSR1_FMID_LDDRREGREG;
				op->Run=BTSR1_Op_MOVB_LdDrRegReg;
				break;
			case 0xD:	/* 35zD */
				op->rm=BTSR1_REG_GBR;
				op->rn=(opw>>4)&15;
				op->nmid=BTSR1_NMID_MOVW;
				op->fmid=BTSR1_FMID_LDDRREGREG;
				op->Run=BTSR1_Op_MOVW_LdDrRegReg;
				break;
			case 0xE:	/* 35zE */
				op->rm=BTSR1_REG_GBR;
				op->rn=(opw>>4)&15;
				op->nmid=BTSR1_NMID_MOVD;
				op->fmid=BTSR1_FMID_LDDRREGREG;
				op->Run=BTSR1_Op_MOVD_LdDrRegReg;
				break;
			case 0xF:	/* 35zF */
				op->rm=BTSR1_REG_GBR;
				op->rn=(opw>>4)&15;
				op->nmid=BTSR1_NMID_MOVUW;
				op->fmid=BTSR1_FMID_LDDRREGREG;
				op->Run=BTSR1_Op_MOVUW_LdDrRegReg;
				break;
#endif

			default:
				ret=-1;
				break;
			}
			break;
			
		case 0x6:	/* 36zz */
			op->rn=(opw>>4)&15;
			switch(opw&15)
			{
//			case 0x0:	/* 36z0 */
//				break;

			case 0x6:	/* 36z6 */
				op->rm=BTSR1_REG_DLR;
				op->rn=(opw>>4)&15;
				op->nmid=BTSR1_NMID_SHAD;
				op->fmid=BTSR1_FMID_REGREG;
				op->Run=BTSR1_Op_SHAD_RegReg;
				break;
			case 0x7:	/* 36z7 */
				op->rm=BTSR1_REG_DLR;
				op->rn=(opw>>4)&15;
				op->nmid=BTSR1_NMID_SHLD;
				op->fmid=BTSR1_FMID_REGREG;
				op->Run=BTSR1_Op_SHLD_RegReg;
				break;
			case 0x8:	/* 36z8 */
				op->rm=BTSR1_REG_DLR;
				op->rn=(opw>>4)&15;
				op->nmid=BTSR1_NMID_BRA;
				op->fmid=BTSR1_FMID_PCREG;
				op->Run=BTSR1_Op_BRA_PcReg;
				break;
			case 0x9:	/* 36z9 */
				op->rm=BTSR1_REG_DLR;
				op->rn=(opw>>4)&15;
				op->nmid=BTSR1_NMID_BSR;
				op->fmid=BTSR1_FMID_PCREG;
				op->Run=BTSR1_Op_BSR_PcReg;
				break;
			case 0xA:	/* 36zA */
				op->rm=BTSR1_REG_DLR;
				op->rn=(opw>>4)&15;
				op->nmid=BTSR1_NMID_SWAPB;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_SWAPB_Reg;
				break;
			case 0xB:	/* 36zB */
				op->rm=BTSR1_REG_DLR;
				op->rn=(opw>>4)&15;
				op->nmid=BTSR1_NMID_SWAPW;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_SWAPW_Reg;
				break;
			case 0xC:	/* 36zC */
				op->rn=BTSR1_REG_DLR;
				op->rm=(opw>>4)&15;
				op->nmid=BTSR1_NMID_ADD;
				op->fmid=BTSR1_FMID_REGREG;
				op->Run=BTSR1_Op_ADD_RegReg;
				break;
			case 0xD:	/* 36zD */
				op->rn=(opw>>4)&15;
				op->rm=BTSR1_REG_DLR;
				op->nmid=BTSR1_NMID_CMPHS;
				op->fmid=BTSR1_FMID_REGREG;
				op->Run=BTSR1_Op_CMPHS_RegReg;
				break;
			case 0xE:	/* 36zE */
				op->rn=(opw>>4)&15;
				op->rm=BTSR1_REG_DLR;
				op->nmid=BTSR1_NMID_CMPGE;
				op->fmid=BTSR1_FMID_REGREG;
				op->Run=BTSR1_Op_CMPGE_RegReg;
				break;
			case 0xF:	/* 36zF */
				op->rn=(opw>>4)&15;
				op->nmid=BTSR1_NMID_MOVT;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_MOVT_Reg;
				break;
			}
			break;


		case 0xF:	/* 3Fnj */
			op->rn=(opw>>4)&15;
			op->imm=opw&15;
			op->nmid=BTSR1_NMID_LEAB;
			op->fmid=BTSR1_FMID_LDDR4PCREG;
			op->Run=BTSR1_Op_LEAB_LdDr4PcReg;
			break;
			
		default:
			ret=-1;
			break;
		}
		break;

	case 0x4:	/* 4zzz */
		op->rn=(opw>>4)&15;
//		op->rm=opw&15;
		op->imm=opw&15;
		switch((opw>>8)&15)
		{
		case 0x0:	/* 40zz */
			if(op->rn==15)
			{
				op->nmid=BTSR1_NMID_BRA;
				op->fmid=BTSR1_FMID_PCDR4;
				op->Run=BTSR1_Op_BRA_PcDr4;
				op->fl|=BTSR1_OPFL_CTRLF;
				break;
			}
			op->nmid=BTSR1_NMID_MOVB;
			op->fmid=BTSR1_FMID_REGSTDR4PC;
			op->Run=BTSR1_Op_MOVB_RegStDr4Pc;
			break;
		case 0x1:	/* 41zz */
			if(op->rn==15)
			{
				op->nmid=BTSR1_NMID_BSR;
				op->fmid=BTSR1_FMID_PCDR4;
				op->Run=BTSR1_Op_BSR_PcDr4;
				op->fl|=BTSR1_OPFL_CTRLF;
				break;
			}
			op->nmid=BTSR1_NMID_MOVW;
			op->fmid=BTSR1_FMID_REGSTDR4PC;
			op->Run=BTSR1_Op_MOVW_RegStDr4Pc;
			break;
		case 0x2:	/* 42zz */
			op->nmid=BTSR1_NMID_MOVD;
			op->fmid=BTSR1_FMID_REGSTDR4PC;
			op->Run=BTSR1_Op_MOVD_RegStDr4Pc;
			break;

		case 0x3:	/* 43zz */
			op->nmid=BTSR1_NMID_MOVUB;
			op->fmid=BTSR1_FMID_LDDR4PCREG;
			op->Run=BTSR1_Op_MOVUB_LdDr4PcReg;
			break;
		case 0x4:	/* 44zz */
			if(op->rn==15)
			{
				op->nmid=BTSR1_NMID_BT;
				op->fmid=BTSR1_FMID_PCDR4;
				op->Run=BTSR1_Op_BT_PcDr4;
				op->fl|=BTSR1_OPFL_CTRLF;
				break;
			}
			op->nmid=BTSR1_NMID_MOVB;
			op->fmid=BTSR1_FMID_LDDR4PCREG;
			op->Run=BTSR1_Op_MOVB_LdDr4PcReg;
			break;
		case 0x5:	/* 45zz */
			if(op->rn==15)
			{
				op->nmid=BTSR1_NMID_BF;
				op->fmid=BTSR1_FMID_PCDR4;
				op->Run=BTSR1_Op_BF_PcDr4;
				op->fl|=BTSR1_OPFL_CTRLF;
				break;
			}
			op->nmid=BTSR1_NMID_MOVW;
			op->fmid=BTSR1_FMID_LDDR4PCREG;
			op->Run=BTSR1_Op_MOVW_LdDr4PcReg;
			break;
		case 0x6:	/* 46zz */
			op->nmid=BTSR1_NMID_MOVD;
			op->fmid=BTSR1_FMID_LDDR4PCREG;
			op->Run=BTSR1_Op_MOVD_LdDr4PcReg;
			break;
		case 0x7:	/* 47zz */
			op->nmid=BTSR1_NMID_MOVUW;
			op->fmid=BTSR1_FMID_LDDR4PCREG;
			op->Run=BTSR1_Op_MOVUW_LdDr4PcReg;
			break;

		case 0x8:	/* 48zz */
			op->nmid=BTSR1_NMID_MOV;
			op->fmid=BTSR1_FMID_DR4REG;
			op->Run=BTSR1_Op_MOV_Dr4Reg;
			break;
		case 0x9:	/* 49zz */
			op->nmid=BTSR1_NMID_ADD;
			op->fmid=BTSR1_FMID_DR4REG;
			op->Run=BTSR1_Op_ADD_Dr4Reg;
			break;

		case 0xA:	/* 4Azz */
			op->rn=(opw>>4)&15;
			op->rm=opw&15;
			op->rn|=32;
			op->nmid=BTSR1_NMID_MOV;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_MOV_RegReg;
			break;
		case 0xB:	/* 4Bzz */
			op->rn=(opw>>4)&15;
			op->rm=opw&15;
			op->rm|=32;
			op->nmid=BTSR1_NMID_MOV;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_MOV_RegReg;
			break;

		case 0xC:	/* 4Czz */
			op->rn=(opw>>4)&15;
			op->rm=opw&15;
			op->imm=0;
			op->nmid=BTSR1_NMID_LEAB;
			op->fmid=BTSR1_FMID_LDDRREGREG;
			op->Run=BTSR1_Op_LEAB_LdDrRegReg;
			break;
		case 0xD:	/* 4Dzz */
			op->rn=(opw>>4)&15;
			op->rm=opw&15;
			op->imm=0;
			op->nmid=BTSR1_NMID_LEAW;
			op->fmid=BTSR1_FMID_LDDRREGREG;
			op->Run=BTSR1_Op_LEAW_LdDrRegReg;
			break;
		case 0xE:	/* 4Ezz */
			op->rn=(opw>>4)&15;
			op->rm=opw&15;
			op->imm=0;
			op->nmid=BTSR1_NMID_LEAD;
			op->fmid=BTSR1_FMID_LDDRREGREG;
			op->Run=BTSR1_Op_LEAD_LdDrRegReg;
			break;
		case 0xF:	/* 4Fzz */
			op->rn=(opw>>4)&15;
			op->rm=opw&15;
			op->imm=0;
			op->nmid=BTSR1_NMID_LEAQ;
			op->fmid=BTSR1_FMID_LDDRREGREG;
			op->Run=BTSR1_Op_LEAQ_LdDrRegReg;
			break;
		}
		break;

	case 0x5:	/* 5zzz */
		op->rn=(opw>>4)&15;
//		op->rm=opw&15;
		op->imm=opw&15;
		switch((opw>>8)&15)
		{
#if 1
		case 0x8:	/* 58nm */
			op->rn=(opw>>4)&15;
			op->rm=(opw   )&15;
			op->nmid=BTSR1_NMID_ADD;
			op->fmid=BTSR1_FMID_REGDRREG;
			op->Run=BTSR1_Op_ADD_RegDrReg;
			break;
		case 0x9:	/* 59nm */
			op->rn=(opw>>4)&15;
			op->rm=(opw   )&15;
			op->nmid=BTSR1_NMID_SUB;
			op->fmid=BTSR1_FMID_REGDRREG;
			op->Run=BTSR1_Op_SUB_RegDrReg;
			break;
		case 0xA:	/* 5Anm */
			op->rn=(opw>>4)&15;
			op->rm=(opw   )&15;
			op->nmid=BTSR1_NMID_AND;
			op->fmid=BTSR1_FMID_REGDRREG;
			op->Run=BTSR1_Op_AND_RegDrReg;
			break;
		case 0xB:	/* 5Bnm */
			op->rn=(opw>>4)&15;
			op->rm=(opw   )&15;
			op->nmid=BTSR1_NMID_OR;
			op->fmid=BTSR1_FMID_REGDRREG;
			op->Run=BTSR1_Op_OR_RegDrReg;
			break;
		case 0xC:	/* 5Cnm */
			op->rn=(opw>>4)&15;
			op->rm=(opw   )&15;
			op->nmid=BTSR1_NMID_XOR;
			op->fmid=BTSR1_FMID_REGDRREG;
			op->Run=BTSR1_Op_XOR_RegDrReg;
			break;
#endif
		}
		break;

	case 0x8:	/* 8zzz */
		op->rn=(opw>>4)&15;
		op->rm=opw&15;
		switch((opw>>8)&15)
		{
		case 0x1:	/* 81zz */
		case 0x2:	/* 82zz */
		case 0x3:	/* 83zz */
		case 0x4:	/* 84zz */
		case 0x5:	/* 85zz */
		case 0x6:	/* 86zz */
		case 0x7:	/* 87zz */
			op->imm=(opw>>8)&7;
			op->nmid=BTSR1_NMID_MOVD;
			op->fmid=BTSR1_FMID_REGSTREGDISP;
			op->Run=BTSR1_Op_MOVD_RegStRegDisp;
			break;
		case 0x9:	/* 89zz */
		case 0xA:	/* 8Azz */
		case 0xB:	/* 8Bzz */
		case 0xC:	/* 8Czz */
		case 0xD:	/* 8Dzz */
		case 0xE:	/* 8Ezz */
		case 0xF:	/* 8Fzz */
			op->imm=(opw>>8)&7;
			op->nmid=BTSR1_NMID_MOVD;
			op->fmid=BTSR1_FMID_LDREGDISPREG;
			op->Run=BTSR1_Op_MOVD_LdRegDispReg;
			break;
		}
		break;

	case 0x9:	/* 9zzz */
		op->rn=(opw>>4)&15;
		op->rm=opw&15;
		switch((opw>>8)&15)
		{
		case 0x0:	/* 90zz */
			op->imm=(opw>>8)&7;
			op->nmid=BTSR1_NMID_FMOVS;
			op->fmid=BTSR1_FMID_REGSTREG;
			op->Run=BTSR1_Op_FMOVS_RegStReg;
			break;
		case 0x1:	/* 91zz */
			op->imm=(opw>>8)&7;
			op->nmid=BTSR1_NMID_FMOVD;
			op->fmid=BTSR1_FMID_REGSTREG;
			op->Run=BTSR1_Op_FMOVS_RegStReg;
			break;
		case 0x2:	/* 92zz */
			op->imm=(opw>>8)&7;
			op->nmid=BTSR1_NMID_FMOVS;
			op->fmid=BTSR1_FMID_REGSTDRREG;
			op->Run=BTSR1_Op_FMOVS_RegStDrReg;
			break;
		case 0x3:	/* 93zz */
			op->imm=(opw>>8)&7;
			op->nmid=BTSR1_NMID_FMOVD;
			op->fmid=BTSR1_FMID_REGSTDRREG;
			op->Run=BTSR1_Op_FMOVS_RegStDrReg;
			break;


		case 0x4:	/* 94zz */
			op->imm=(opw>>8)&7;
			op->nmid=BTSR1_NMID_FMOVS;
			op->fmid=BTSR1_FMID_LDREGREG;
			op->Run=BTSR1_Op_FMOVS_LdRegReg;
			break;
		case 0x5:	/* 95zz */
			op->imm=(opw>>8)&7;
			op->nmid=BTSR1_NMID_FMOVD;
			op->fmid=BTSR1_FMID_LDREGREG;
			op->Run=BTSR1_Op_FMOVS_LdRegReg;
			break;
		case 0x6:	/* 96zz */
			op->imm=(opw>>8)&7;
			op->nmid=BTSR1_NMID_FMOVS;
			op->fmid=BTSR1_FMID_LDDRREGREG;
			op->Run=BTSR1_Op_FMOVS_LdDrRegReg;
			break;
		case 0x7:	/* 97zz */
			op->imm=(opw>>8)&7;
			op->nmid=BTSR1_NMID_FMOVD;
			op->fmid=BTSR1_FMID_LDDRREGREG;
			op->Run=BTSR1_Op_FMOVS_LdDrRegReg;
			break;

		case 0x8:	/* 98zz */
			op->nmid=BTSR1_NMID_FADD;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_FADD_RegReg;
			break;
		case 0x9:	/* 99zz */
			op->nmid=BTSR1_NMID_FSUB;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_FSUB_RegReg;
			break;
		case 0xA:	/* 9Azz */
			op->nmid=BTSR1_NMID_FMUL;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_FMUL_RegReg;
			break;
		case 0xB:	/* 9Bzz */
			op->nmid=BTSR1_NMID_FDIV;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_FDIV_RegReg;
			break;

		case 0xC:	/* 9Czz */
			op->nmid=BTSR1_NMID_FCMPEQ;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_FCMPEQ_RegReg;
			break;
		case 0xD:	/* 9Dzz */
			op->nmid=BTSR1_NMID_FCMPGT;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_FCMPGT_RegReg;
			break;
		case 0xE:	/* 9Ezz */
			op->nmid=BTSR1_NMID_FMOV;
			op->fmid=BTSR1_FMID_REGREG;
			op->Run=BTSR1_Op_FMOV_RegReg;
			break;

		case 0xF:	/* 9Fzz */
			switch(opw&15)
			{
			case 0x0:	/* 9Fz0 */
				op->nmid=BTSR1_NMID_FLDCF;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_FLDCF_Reg;
				break;
			case 0x1:	/* 9Fz1 */
				op->nmid=BTSR1_NMID_FLDCD;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_FLDCD_Reg;
				break;
			case 0x2:	/* 9Fz2 */
				op->nmid=BTSR1_NMID_FLDCI;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_FLDCI_Reg;
				break;
			case 0x3:	/* 9Fz3 */
				op->nmid=BTSR1_NMID_FLDCH;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_FLDCH_Reg;
				break;
			case 0x4:	/* 9Fz4 */
				op->nmid=BTSR1_NMID_FSTCF;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_FSTCF_Reg;
				break;
			case 0x5:	/* 9Fz5 */
				op->nmid=BTSR1_NMID_FSTCD;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_FSTCD_Reg;
				break;
			case 0x6:	/* 9Fz6 */
				op->nmid=BTSR1_NMID_FSTCI;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_FSTCI_Reg;
				break;
			case 0x7:	/* 9Fz7 */
				op->nmid=BTSR1_NMID_FSTCH;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_FSTCH_Reg;
				break;
			case 0x8:	/* 9Fz8 */
				op->nmid=BTSR1_NMID_FNEG;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_FNEG_Reg;
				break;
			case 0x9:	/* 9Fz9 */
				op->nmid=BTSR1_NMID_FABS;
				op->fmid=BTSR1_FMID_REG;
				op->Run=BTSR1_Op_FABS_Reg;
				break;
			}
			break;
		}
		break;

	case 0xA:	/* Azzz */
	case 0xB:	/* Bzzz */
		if(opw&0x1000)
		{
			op->imm=opw|(~4095);
			op->nmid=BTSR1_NMID_LDIN;
			op->fmid=BTSR1_FMID_IMM;
			op->Run=BTSR1_Op_LDIN_Imm12;
		}else
		{
			op->imm=opw&4095;
			op->nmid=BTSR1_NMID_LDIZ;
			op->fmid=BTSR1_FMID_IMM;
			op->Run=BTSR1_Op_LDIZ_Imm12;
		}

		BTSR1_DecodeOpcode_CheckTwoWord(ctx, op, addr, opw);
		break;
	case 0xC:	/* Cnii */
		op->imm=(sbyte)opw;
		op->rn=(opw>>8)&15;
		op->nmid=BTSR1_NMID_ADD;
		op->fmid=BTSR1_FMID_IMMREG;
		op->Run=BTSR1_Op_ADD_ImmReg;
		break;

	case 0xE:	/* Enii */
		op->imm=(sbyte)opw;
		op->rn=(opw>>8)&15;
		op->nmid=BTSR1_NMID_MOV;
		op->fmid=BTSR1_FMID_IMMREG;
		op->Run=BTSR1_Op_MOV_ImmReg;
		break;

	default:
		ret=-1;
		break;
	}
	
	if(op->cyc==-1)
	{
//		op->cyc==1;
		switch(op->nmid)
		{
		case BTSR1_NMID_MOVB:
		case BTSR1_NMID_MOVW:
		case BTSR1_NMID_MOVD:
		case BTSR1_NMID_MOVUB:
		case BTSR1_NMID_MOVUW:
			op->cyc=3;
			break;

		case BTSR1_NMID_BRA:
		case BTSR1_NMID_BSR:
		case BTSR1_NMID_BT:
		case BTSR1_NMID_BF:
			op->cyc=3;
			break;

		case BTSR1_NMID_MULU:
		case BTSR1_NMID_MULS:
			op->cyc=3;
			break;
		
		default:
			op->cyc=1;
			break;
		}
	}
	
	if(!op->Run)
	{
		__debugbreak();
	}
	
	return(ret);
}
