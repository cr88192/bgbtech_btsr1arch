int BJX2_DecodeOpcode_DecF0(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2);
int BJX2_DecodeOpcode_DecF1(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2);
int BJX2_DecodeOpcode_DecF2(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2);
int BJX2_DecodeOpcode_DecF8(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2);
int BJX2_DecodeOpcode_DecFC(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2, int opw3);

int BJX2_DecodeOpcode_DecD4(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2);
int BJX2_DecodeOpcode_DecD5(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2);
int BJX2_DecodeOpcode_DecD6(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2);
int BJX2_DecodeOpcode_DecD8(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2);
int BJX2_DecodeOpcode_DecDC(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2, int opw3);


int BJX2_DecodeOpcode_CheckTwoWord(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw)
{
	int opw2, opw3;

	return(0);

//	opw=BJX2_MemGetWord(ctx, addr);
	opw2=BJX2_MemGetWord(ctx, addr+2);
//	opw3=BJX2_MemGetWord(ctx, addr+4);

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
			op->fl|=BJX2_OPFL_TWOWORD;
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVB_RegStRegDisp;
			break;
		case 0x5:
			op->fl|=BJX2_OPFL_TWOWORD;
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVW_RegStRegDisp;
			break;
		case 0x6:
			op->fl|=BJX2_OPFL_TWOWORD;
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVL_RegStRegDisp;
			break;
//		case 0x7:
//			op->fl|=BJX2_OPFL_TWOWORD;
//			op->nmid=BJX2_NMID_MOVUB;
//			op->fmid=BJX2_FMID_LDREGDISPREG;
//			op->Run=BJX2_Op_MOVUB_LdRegDispReg;
//			break;

		case 0xC:
			op->fl|=BJX2_OPFL_TWOWORD;
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVB_LdRegDispReg;
			break;
		case 0xD:
			op->fl|=BJX2_OPFL_TWOWORD;
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVW_LdRegDispReg;
			break;
		case 0xE:
			op->fl|=BJX2_OPFL_TWOWORD;
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVL_LdRegDispReg;
			break;
//		case 0xF:
//			op->fl|=BJX2_OPFL_TWOWORD;
//			op->nmid=BJX2_NMID_MOVUW;
//			op->fmid=BJX2_FMID_LDREGDISPREG;
//			op->Run=BJX2_Op_MOVUW_LdRegDispReg;
//			break;
		}
		break;

	case 0x3:
		switch((opw2>>8)&15)
		{
		case 0x1:
			switch(opw2&15)
			{
			case 0x4:
				op->fl|=BJX2_OPFL_TWOWORD;
				op->rn=(opw2>>4)&15;
				op->nmid=BJX2_NMID_TST;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_TST_ImmReg;
				break;
			case 0x5:
				op->fl|=BJX2_OPFL_TWOWORD;
				op->rn=(opw2>>4)&15;
				op->nmid=BJX2_NMID_AND;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_AND_ImmReg;
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
				op->fl|=BJX2_OPFL_TWOWORD|BJX2_OPFL_CTRLF;
				op->imm=(op->imm<<4)|(opw2&15);
//				op->imm+=1;
				op->nmid=BJX2_NMID_BRA;
				op->fmid=BJX2_FMID_PCDISP;
				op->Run=BJX2_Op_BRA_PcDisp;
#endif
				break;
			}
			op->fl|=BJX2_OPFL_TWOWORD;
			op->imm=(op->imm<<4)|(opw2&15);
//			op->imm+=2;
			op->rn=(opw2>>4)&15;
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_REGSTPCDISP;
			op->Run=BJX2_Op_MOVB_RegStPcDisp;
			break;
#if 1
		case 0x1:
			if(((opw2>>4)&15)==15)
			{
#if 1
				op->fl|=BJX2_OPFL_TWOWORD|BJX2_OPFL_CTRLF;
				op->imm=(op->imm<<4)|(opw2&15);
//				op->imm+=1;
				op->nmid=BJX2_NMID_BSR;
				op->fmid=BJX2_FMID_PCDISP;
				op->Run=BJX2_Op_BSR_PcDisp2;
#endif
				break;
			}
			op->fl|=BJX2_OPFL_TWOWORD;
			op->imm=(op->imm<<4)|(opw2&15);
			op->rn=(opw2>>4)&15;
//			op->imm+=2;
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_REGSTPCDISP;
			op->Run=BJX2_Op_MOVW_RegStPcDisp;
			break;
		case 0x2:
			if(((opw2>>4)&15)==15)
				break;
			op->fl|=BJX2_OPFL_TWOWORD;
			op->imm=(op->imm<<4)|(opw2&15);
			op->rn=(opw2>>4)&15;
//			op->imm+=2;
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTPCDISP;
			op->Run=BJX2_Op_MOVL_RegStPcDisp;
			break;
/*		case 0x3:
			if(((opw2>>4)&15)==15)
				break;
			op->fl|=BJX2_OPFL_TWOWORD;
			op->imm=(op->imm<<4)|(opw2&15);
			op->rn=(opw2>>4)&15;
			op->imm+=2;
			op->nmid=BJX2_NMID_MOVUB;
			op->fmid=BJX2_FMID_LDPCDISPREG;
			op->Run=BJX2_Op_MOVUB_LdPcDispReg;
			break;
			*/
#endif
		case 0x4:
			if(((opw2>>4)&15)==15)
			{
#if 1
				op->fl|=BJX2_OPFL_TWOWORD|BJX2_OPFL_CTRLF;
				op->imm=(op->imm<<4)|(opw2&15);
//				op->imm+=1;
				op->nmid=BJX2_NMID_BT;
				op->fmid=BJX2_FMID_PCDISP;
				op->Run=BJX2_Op_BT_PcDisp;
#endif
				break;
			}
			op->fl|=BJX2_OPFL_TWOWORD;
			op->imm=(op->imm<<4)|(opw2&15);
			op->rn=(opw2>>4)&15;
//			op->imm+=2;
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_LDPCDISPREG;
			op->Run=BJX2_Op_MOVB_LdPcDispReg;
			break;
		case 0x5:
			if(((opw2>>4)&15)==15)
			{
#if 1
				op->fl|=BJX2_OPFL_TWOWORD|BJX2_OPFL_CTRLF;
				op->imm=(op->imm<<4)|(opw2&15);
//				op->imm+=1;
				op->nmid=BJX2_NMID_BF;
				op->fmid=BJX2_FMID_PCDISP;
				op->Run=BJX2_Op_BF_PcDisp;
#endif
				break;
			}
			op->fl|=BJX2_OPFL_TWOWORD;
			op->imm=(op->imm<<4)|(opw2&15);
			op->rn=(opw2>>4)&15;
//			op->imm+=2;
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_LDPCDISPREG;
			op->Run=BJX2_Op_MOVW_LdPcDispReg;
			break;
		case 0x6:
			if(((opw2>>4)&15)==15)
				break;
			op->fl|=BJX2_OPFL_TWOWORD;
			op->imm=(op->imm<<4)|(opw2&15);
			op->rn=(opw2>>4)&15;
//			op->imm+=2;
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDPCDISPREG;
			op->Run=BJX2_Op_MOVL_LdPcDispReg;
			break;
/*		case 0x7:
			if(((opw2>>4)&15)==15)
				break;
			op->fl|=BJX2_OPFL_TWOWORD;
			op->imm=(op->imm<<4)|(opw2&15);
			op->rn=(opw2>>4)&15;
			op->imm+=2;
			op->nmid=BJX2_NMID_MOVUW;
			op->fmid=BJX2_FMID_LDPCDISPREG;
			op->Run=BJX2_Op_MOVUW_LdPcDispReg;
			break; */

		case 0x8:
			op->fl|=BJX2_OPFL_TWOWORD;
			op->imm=(op->imm<<4)|(opw2&15);
			op->rn=(opw2>>4)&15;
//				op->rm=opw&15;
			op->nmid=BJX2_NMID_MOV;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_MOV_ImmReg;
			break;
		}
		break;
	default:
		break;
	}
#endif
	return(0);
}

int BJX2_DecodeOpcodeForAddr(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr)
{
	int ret, fnm;
	int opw, opw2, opw3;

	opw =BJX2_MemGetWord(ctx, addr+0);
	opw2=BJX2_MemGetWord(ctx, addr+2);
	opw3=BJX2_MemGetWord(ctx, addr+4);
	
	op->opn=opw;
	op->pc=addr;
	op->cyc=-1;
	op->rn=0;
	op->rm=0;
	op->ro=0;
	op->imm=0;
	
#ifdef BJX2_FLIPSTNM
	fnm=0;
	if((opw&0xF800)==0x0000)
		fnm=1;
	if(((opw&0xF800)==0x8000) &&
		((opw&0xFF00)!=0x8000))
			fnm=1;
	if((opw&0xFC00)==0x9000)
		fnm=1;
	
	if(fnm)
	{
		opw=(opw&0xFF00)|
			((opw<<4)&0x00F0)|
			((opw>>4)&0x000F);
	}
#endif
	
	ret=0;
	switch((opw>>12)&15)
	{
	case 0x0:	/* 0zzz */
		op->rn=(opw>>4)&15;
		op->rm=opw&15;
		op->ro=0;
		switch((opw>>8)&15)
		{
		case 0x0:	/* 00zz */
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_REGSTREG;
			op->Run=BJX2_Op_MOVB_RegStReg;
			break;
		case 0x1:	/* 01zz */
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_REGSTREG;
			op->Run=BJX2_Op_MOVW_RegStReg;
			break;
		case 0x2:	/* 02zz */
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREG;
			op->Run=BJX2_Op_MOVL_RegStReg;
			break;
		case 0x3:	/* 03zz */
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREG;
			op->Run=BJX2_Op_MOVQ_RegStReg;
			break;
//		case 0x3:	/* 03zz */
//			op->nmid=BJX2_NMID_MOVUB;
//			op->fmid=BJX2_FMID_LDREGREG;
//			op->Run=BJX2_Op_MOVUB_LdRegReg;
//			break;

		case 0x4:	/* 04zz */
			if(op->rn==0)
			{
				op->rn=op->rm;
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_REGSTDRPC;
				op->Run=BJX2_Op_MOVB_RegStDrPc;
				break;
			}
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_REGSTDRREG;
			op->Run=BJX2_Op_MOVB_RegStDrReg;
			if(op->rn==1)
			{
				op->rn=BJX2_REG_GBR;
				op->ro=BJX2_REG_DLR;
				op->Run=BJX2_Op_MOVB_RegStReg2B;
			}
			break;
		case 0x5:	/* 05zz */
			if(op->rn==0)
			{
				op->rn=op->rm;
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_REGSTDRPC;
				op->Run=BJX2_Op_MOVW_RegStDrPc;
				break;
			}
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_REGSTDRREG;
			op->Run=BJX2_Op_MOVW_RegStDrReg;
			if(op->rn==1)
			{
				op->rn=BJX2_REG_GBR;
				op->ro=BJX2_REG_DLR;
				op->Run=BJX2_Op_MOVW_RegStReg2B;
			}
			break;
		case 0x6:	/* 06zz */
			if(op->rn==0)
			{
				op->rn=op->rm;
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_REGSTDRPC;
				op->Run=BJX2_Op_MOVL_RegStDrPc;
				break;
			}
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTDRREG;
			op->Run=BJX2_Op_MOVL_RegStDrReg;
			if(op->rn==1)
			{
				op->rn=BJX2_REG_GBR;
				op->ro=BJX2_REG_DLR;
				op->Run=BJX2_Op_MOVL_RegStReg2B;
			}
			break;
		case 0x7:	/* 07zz */
			if(op->rn==0)
			{
				op->rn=op->rm;
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_REGSTDRPC;
				op->Run=BJX2_Op_MOVQ_RegStDrPc;
				break;
			}
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTDRREG;
			op->Run=BJX2_Op_MOVQ_RegStDrReg;
			if(op->rn==1)
			{
				op->rn=BJX2_REG_GBR;
				op->ro=BJX2_REG_DLR;
				op->Run=BJX2_Op_MOVQ_RegStReg2B;
			}
			break;
//		case 0x7:	/* 07zz */
//			op->nmid=BJX2_NMID_MOVUB;
//			op->fmid=BJX2_FMID_LDDRREGREG;
//			op->Run=BJX2_Op_MOVUB_LdDrRegReg;
//			break;
		case 0x8:	/* 08zz */
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_LDREGREG;
			op->Run=BJX2_Op_MOVB_LdRegReg;
			break;
		case 0x9:	/* 09zz */
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_LDREGREG;
			op->Run=BJX2_Op_MOVW_LdRegReg;
			break;
		case 0xA:	/* 0Azz */
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREGREG;
			op->Run=BJX2_Op_MOVL_LdRegReg;
			break;
		case 0xB:	/* 0Bzz */
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREGREG;
			op->Run=BJX2_Op_MOVQ_LdRegReg;
			break;
//		case 0xB:	/* 0Bzz */
//			op->nmid=BJX2_NMID_MOVUW;
//			op->fmid=BJX2_FMID_LDREGREG;
//			op->Run=BJX2_Op_MOVUW_LdRegReg;
//			break;
		case 0xC:	/* 0Czz */
			if(op->rm==0)
			{
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_LDDRPCREG;
				op->Run=BJX2_Op_MOVB_LdDrPcReg;
				break;
			}
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_LDDRREGREG;
			op->Run=BJX2_Op_MOVB_LdDrRegReg;
			if(op->rm==1)
			{
				op->rm=BJX2_REG_GBR;
				op->ro=BJX2_REG_DLR;
				op->Run=BJX2_Op_MOVB_LdReg2RegB;
			}
			break;
		case 0xD:	/* 0Dzz */
			if(op->rm==0)
			{
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_LDDRPCREG;
				op->Run=BJX2_Op_MOVW_LdDrPcReg;
				break;
			}
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_LDDRREGREG;
			op->Run=BJX2_Op_MOVW_LdDrRegReg;
			if(op->rm==1)
			{
				op->rm=BJX2_REG_GBR;
				op->ro=BJX2_REG_DLR;
				op->Run=BJX2_Op_MOVW_LdReg2RegB;
			}
			break;
		case 0xE:	/* 0Ezz */
			if(op->rm==0)
			{
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_LDDRPCREG;
				op->Run=BJX2_Op_MOVL_LdDrPcReg;
				break;
			}
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDDRREGREG;
			op->Run=BJX2_Op_MOVL_LdDrRegReg;
			if(op->rm==1)
			{
				op->rm=BJX2_REG_GBR;
				op->ro=BJX2_REG_DLR;
				op->Run=BJX2_Op_MOVL_LdReg2RegB;
			}
			break;
		case 0xF:	/* 0Fzz */
			if(op->rm==0)
			{
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_LDDRPCREG;
				op->Run=BJX2_Op_MOVQ_LdDrPcReg;
				break;
			}
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDDRREGREG;
			op->Run=BJX2_Op_MOVQ_LdDrRegReg;
			if(op->rm==1)
			{
				op->rm=BJX2_REG_GBR;
				op->ro=BJX2_REG_DLR;
				op->Run=BJX2_Op_MOVQ_LdReg2RegB;
			}
			break;
//		case 0xF:	/* 0Fzz */
//			op->nmid=BJX2_NMID_MOVUB;
//			op->fmid=BJX2_FMID_LDDRREGREG;
//			op->Run=BJX2_Op_MOVUB_LdDrRegReg;
//			break;
		}
		break;

	case 0x1:	/* 1zzz */
		op->rn=(opw>>4)&15;
		op->rm=opw&15;
		switch((opw>>8)&15)
		{
		case 0x0:	/* 10zz */
			op->nmid=BJX2_NMID_ADD;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_ADD_RegReg;
			break;
		case 0x1:	/* 11zz */
			op->nmid=BJX2_NMID_SUB;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_SUB_RegReg;
			break;
		case 0x2:	/* 12zz */
			op->nmid=BJX2_NMID_ADC;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_ADC_RegReg;
			break;
		case 0x3:	/* 13zz */
			op->nmid=BJX2_NMID_SBB;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_SBB_RegReg;
			break;
		case 0x4:	/* 14zz */
			op->nmid=BJX2_NMID_TST;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_TST_RegReg;
			break;
		case 0x5:	/* 15zz */
			op->nmid=BJX2_NMID_AND;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_AND_RegReg;
			break;
		case 0x6:	/* 16zz */
			op->nmid=BJX2_NMID_OR;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_OR_RegReg;
			break;
		case 0x7:	/* 17zz */
			op->nmid=BJX2_NMID_XOR;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_XOR_RegReg;
			break;
		case 0x8:	/* 18zz */
			op->nmid=BJX2_NMID_MOV;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_MOV_RegReg;
			break;
#if 1
		case 0x9:	/* 19nm */
		case 0xA:	/* 1Anm */
		case 0xB:	/* 1Bnm */
			op->rn=((opw>>4)&15)+((opw&0x0200)?16:0);
			op->rm=((opw   )&15)+((opw&0x0100)?16:0);
			op->nmid=BJX2_NMID_MOV;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_MOV_RegReg;
			break;
#endif

#if 0
		case 0x9:	/* 19zz */
			op->nmid=BJX2_NMID_MULS;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_MULS_RegReg;
			break;
		case 0xA:	/* 1Azz */
			op->rn+=BJX2_REG_PC;
			op->nmid=BJX2_NMID_MOV;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_MOV_RegReg;
			break;
		case 0xB:	/* 1Bzz */
			op->rm+=BJX2_REG_PC;
			op->nmid=BJX2_NMID_MOV;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_MOV_RegReg;
			break;
		case 0xF:	/* 1Fzz */
			op->nmid=BJX2_NMID_MULU;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_MULU_RegReg;
			break;
#endif
		case 0xC:	/* 1Czz */
			op->nmid=BJX2_NMID_CMPEQ;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_CMPEQ_RegReg;
			break;
		case 0xD:	/* 1Dzz */
			op->nmid=BJX2_NMID_CMPHI;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_CMPHI_RegReg;
			break;
		case 0xE:	/* 1Ezz */
			op->nmid=BJX2_NMID_CMPGT;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_CMPGT_RegReg;
			break;

#if 1
		case 0xF:	/* 1Fzz */
			op->nmid=BJX2_NMID_CMPGE;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_CMPGE_RegReg;
			break;
#endif

#if 0
		case 0xF:	/* 1Fzz */
			op->nmid=BJX2_NMID_ADDSL;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_ADDSL_RegReg;
			break;
#endif

#if 0
		case 0xF:	/* 1Fzz */
			op->nmid=BJX2_NMID_MULU;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_MULU_RegReg;
			break;
#endif
		}
		break;

	case 0x2:	/* 2zzz */
		op->rn=(opw>>4)&15;
		op->imm=(sbyte)opw;
		switch((opw>>8)&15)
		{
		case 0x0:	/* 20zz */
			op->nmid=BJX2_NMID_BRA;
			op->fmid=BJX2_FMID_PCDISP;
			op->Run=BJX2_Op_BRA_PcDisp;
			op->fl|=BJX2_OPFL_CTRLF;
			break;
		case 0x1:	/* 21zz */
			op->nmid=BJX2_NMID_BSR;
			op->fmid=BJX2_FMID_PCDISP;
			op->Run=BJX2_Op_BSR_PcDisp;
			op->fl|=BJX2_OPFL_CTRLF;
			break;
		case 0x2:	/* 22zz */
			op->nmid=BJX2_NMID_BT;
			op->fmid=BJX2_FMID_PCDISP;
			op->Run=BJX2_Op_BT_PcDisp;
			op->fl|=BJX2_OPFL_CTRLF;
			break;
		case 0x3:	/* 23zz */
			op->nmid=BJX2_NMID_BF;
			op->fmid=BJX2_FMID_PCDISP;
			op->Run=BJX2_Op_BF_PcDisp;
			op->fl|=BJX2_OPFL_CTRLF;
			break;
#if 1
		case 0x4:	/* 24zz */
			op->imm=opw&255;
			op->nmid=BJX2_NMID_LDIZ;
			op->fmid=BJX2_FMID_IMM;
			op->Run=BJX2_Op_LDIZ_Imm;
//			BJX2_DecodeOpcode_CheckTwoWord(ctx, op, addr, opw);
			break;
		case 0x5:	/* 25zz */
			op->imm=opw|(~255);
			op->nmid=BJX2_NMID_LDIN;
			op->fmid=BJX2_FMID_IMM;
			op->Run=BJX2_Op_LDIN_Imm;
//			BJX2_DecodeOpcode_CheckTwoWord(ctx, op, addr, opw);
			break;
#endif
		case 0x6:	/* 26zz */
			op->imm=opw&255;
			op->nmid=BJX2_NMID_LDISH8;
			op->fmid=BJX2_FMID_IMM;
			op->Run=BJX2_Op_LDISH_Imm;
			break;
//		case 0x7:	/* 27zz */
//			op->rn=BJX2_REG_SP;
//			op->rm=BJX2_REG_SP;
//			op->nmid=BJX2_NMID_LEAD;
//			op->fmid=BJX2_FMID_LDREGDISPREG;
//			op->Run=BJX2_Op_LEAD_LdRegDispReg;
//			break;

#if 1
		case 0x8:	/* 28zz */
			op->imm=opw&15;
			op->rm=BJX2_REG_SP;
			op->nmid=BJX2_NMID_MOVUL;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVUL_LdRegDispReg;
			break;

		case 0xA:	/* 2Azz */
			op->imm=opw&15;
			op->rm=BJX2_REG_SP;
			op->rn+=16;
			op->nmid=BJX2_NMID_MOVUL;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVUL_LdRegDispReg;
			break;
#endif

#if 0
//		case 0x9:	/* 29zz */
		case 0x7:	/* 27zz */
			op->imm=opw&15;
//			op->rm=BJX2_REG_SP;
			op->rn+=16;
			op->nmid=BJX2_NMID_ADD;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_ADD_ImmReg;
			break;
//		case 0xB:	/* 2Bzz */
		case 0x9:	/* 29zz */
			op->imm=opw&15;
//			op->rm=BJX2_REG_SP;
			op->rn+=16;
			op->nmid=BJX2_NMID_LDIZ;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_MOV_ImmReg;
			break;
		case 0xB:	/* 2Bzz */
			op->imm=(opw&15)|(~15);
//			op->rm=BJX2_REG_SP;
			op->rn+=16;
			op->nmid=BJX2_NMID_LDIN;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_MOV_ImmReg;
			break;
#endif

#if 0
		case 0x8:	/* 28zz */
			op->imm=opw&15;
			op->rm=op->rn;
			op->rn=BJX2_REG_SP;
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVL_RegStRegDisp;
			break;
		case 0x9:	/* 29zz */
#if 1
			op->imm=opw&15;
			op->rm=op->rn;
			op->rn=BJX2_REG_SP;
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVQ_RegStRegDisp;
#endif
			break;
		case 0xA:	/* 2Azz */
			op->imm=opw&15;
			op->rm=BJX2_REG_SP;
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVL_LdRegDispReg;
			break;
		case 0xB:	/* 2Bzz */
#if 1
			op->imm=opw&15;
			op->rm=BJX2_REG_SP;
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVQ_LdRegDispReg;
#endif
			break;
#endif

		case 0xC:	/* 2Czz */
			op->imm=opw&15;
			op->nmid=BJX2_NMID_CMPEQ;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_CMPEQ_ImmReg;
			break;
		case 0xD:	/* 2Dzz */
			op->imm=opw|(~15);
			op->nmid=BJX2_NMID_CMPEQ;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_CMPEQ_ImmReg;
			break;
		case 0xE:	/* 2Ezz */
			op->imm=opw&15;
			op->nmid=BJX2_NMID_CMPGT;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_CMPGT_ImmReg;
			break;
		case 0xF:	/* 2Fzz */
			op->imm=opw&15;
			op->nmid=BJX2_NMID_CMPGE;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_CMPGE_ImmReg;
			break;
		}
		break;

	case 0x3:	/* 3zzz */
//		op->rn=(opw>>4)&15;
		op->rn=((opw>>4)&15)|((opw>>7)&16);
//		switch((opw>>8)&15)
		switch((opw>>8)&7)
		{
		case 0x0:	/* 30zz */
			switch(opw&15)
			{
			case 0x0:	/* 30z0 */
				op->fmid=BJX2_FMID_Z;
				
				switch((opw>>4)&15)
				{
				case 0x0:
					op->nmid=BJX2_NMID_NOP;
					op->Run=BJX2_Op_NOP_None;
					break;
				case 0x1:
					op->nmid=BJX2_NMID_RTS;
					op->Run=BJX2_Op_RTS_None;
					op->fl|=BJX2_OPFL_CTRLF;
					break;

				case 0x2:
					op->nmid=BJX2_NMID_SLEEP;
					op->Run=BJX2_Op_SLEEP_None;
					op->fl|=BJX2_OPFL_CTRLF;
					break;
				case 0x3:
					op->nmid=BJX2_NMID_BREAK;
					op->Run=BJX2_Op_BREAK_None;
					op->fl|=BJX2_OPFL_CTRLF;
					break;

				case 0x4:
					op->nmid=BJX2_NMID_CLRT;
					op->Run=BJX2_Op_CLRT_None;
					break;
				case 0x5:
					op->nmid=BJX2_NMID_SETT;
					op->Run=BJX2_Op_SETT_None;
					break;
				case 0x6:
					op->nmid=BJX2_NMID_CLRS;
					op->Run=BJX2_Op_CLRS_None;
					break;
				case 0x7:
					op->nmid=BJX2_NMID_SETS;
					op->Run=BJX2_Op_SETS_None;
					break;
				case 0x8:
					op->nmid=BJX2_NMID_NOTT;
					op->Run=BJX2_Op_NOTT_None;
					break;
				case 0x9:
					op->nmid=BJX2_NMID_NOTS;
					op->Run=BJX2_Op_NOTS_None;
					break;

				case 0xA:
					op->fmid=BJX2_FMID_IMM;
					op->imm=0x0000;
					op->nmid=BJX2_NMID_LDISH16;
					op->Run=BJX2_Op_LDISH16_Imm;
					break;
				case 0xB:
					op->fmid=BJX2_FMID_IMM;
					op->imm=0xFFFF;
					op->nmid=BJX2_NMID_LDISH16;
					op->Run=BJX2_Op_LDISH16_Imm;
					break;
				case 0xC:
					op->nmid=BJX2_NMID_RTE;
					op->Run=BJX2_Op_RTE_None;
					op->fl|=BJX2_OPFL_CTRLF;
					break;
				}
				break;

			case 0x1:	/* 30z1 */
				break;

			case 0x2:	/* 30z2 */
				switch((opw>>4)&15)
				{
#if 0
				case 0x0:
					op->nmid=BJX2_NMID_BRA;
					op->fmid=BJX2_FMID_PCDR;
					op->Run=BJX2_Op_BRA_PcDr;
					op->fl|=BJX2_OPFL_CTRLF;
					break;
				case 0x1:
					op->nmid=BJX2_NMID_BSR;
					op->fmid=BJX2_FMID_PCDR;
					op->Run=BJX2_Op_BSR_PcDr;
					op->fl|=BJX2_OPFL_CTRLF;
					break;
				case 0x2:
					op->nmid=BJX2_NMID_BT;
					op->fmid=BJX2_FMID_PCDR;
					op->Run=BJX2_Op_BT_PcDr;
					op->fl|=BJX2_OPFL_CTRLF;
					break;
				case 0x3:
					op->nmid=BJX2_NMID_BF;
					op->fmid=BJX2_FMID_PCDR;
					op->Run=BJX2_Op_BF_PcDr;
					op->fl|=BJX2_OPFL_CTRLF;
					break;
#endif

				case 0x0:
					op->nmid=BJX2_NMID_NOP;
					op->Run=BJX2_Op_NOP_None;
					break;
				case 0x1:
					op->nmid=BJX2_NMID_RTSU;
					op->Run=BJX2_Op_RTS_None;
					op->fl|=BJX2_OPFL_CTRLF;
					break;


//				case 0x7:
//					op->rn=BJX2_REG_SP;
//					op->rm=BJX2_REG_SP;
//					op->nmid=BJX2_NMID_LEAD;
//					op->fmid=BJX2_FMID_LDDRREGREG;
//					op->Run=BJX2_Op_LEAD_LdDrRegReg;
//					break;

				case 0x8:
					op->fmid=BJX2_FMID_IMM;
					op->imm=0x000;
					op->nmid=BJX2_NMID_LDISH12;
					op->Run=BJX2_Op_LDISH12_Imm;
					break;
				case 0x9:
					op->fmid=BJX2_FMID_IMM;
					op->imm=0xFFF;
					op->nmid=BJX2_NMID_LDISH12;
					op->Run=BJX2_Op_LDISH12_Imm;
					break;
				case 0xA:
					op->fmid=BJX2_FMID_IMM;
					op->imm=0x00000;
					op->nmid=BJX2_NMID_LDISH20;
					op->Run=BJX2_Op_LDISH20_Imm;
					break;
				case 0xB:
					op->fmid=BJX2_FMID_IMM;
					op->imm=0xFFFFF;
					op->nmid=BJX2_NMID_LDISH20;
					op->Run=BJX2_Op_LDISH20_Imm;
					break;
				}
				break;

			case 0x3:	/* 30z3 */
				break;

#if 0
			case 0x4:	/* 30z4 */
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_REGSTDRPC;
				op->Run=BJX2_Op_MOVB_RegStDrPc;
				break;
			case 0x5:	/* 30z5 */
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_REGSTDRPC;
				op->Run=BJX2_Op_MOVW_RegStDrPc;
				break;
			case 0x6:	/* 30z6 */
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_REGSTDRPC;
				op->Run=BJX2_Op_MOVL_RegStDrPc;
				break;
			case 0x7:	/* 30z7 */
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_REGSTDRPC;
				op->Run=BJX2_Op_MOVQ_RegStDrPc;
				break;
//			case 0x7:	/* 30z7 */
//				op->nmid=BJX2_NMID_MOVUB;
//				op->fmid=BJX2_FMID_LDDRPCREG;
//				op->Run=BJX2_Op_MOVUB_LdDrPcReg;
//				break;
#endif

#if 0
			case 0x8:	/* 30z8 */
				op->nmid=BJX2_NMID_PUSH;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_PUSH_Reg;
				break;
			case 0x9:	/* 30z9 */
				op->rn+=BJX2_REG_PC;
				op->nmid=BJX2_NMID_PUSH;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_PUSH_Reg;
				break;
			case 0xA:	/* 30zA */
				op->nmid=BJX2_NMID_POP;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_POP_Reg;
				break;
			case 0xB:	/* 30zB */
				if((opw&0x00F0)==0x0000)
				{
					op->nmid=BJX2_NMID_RET;
					op->fmid=BJX2_FMID_Z;
					op->Run=BJX2_Op_RET_None;
					op->fl|=BJX2_OPFL_CTRLF;
					break;
				}
			
				op->rn+=BJX2_REG_PC;
				op->nmid=BJX2_NMID_POP;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_POP_Reg;
				break;
#endif

#if 0
			case 0xC:	/* 30zC */
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_LDDRPCREG;
				op->Run=BJX2_Op_MOVB_LdDrPcReg;
				break;
			case 0xD:	/* 30zD */
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_LDDRPCREG;
				op->Run=BJX2_Op_MOVW_LdDrPcReg;
				break;
			case 0xE:	/* 30zE */
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_LDDRPCREG;
				op->Run=BJX2_Op_MOVL_LdDrPcReg;
				break;
			case 0xF:	/* 30zF */
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_LDDRPCREG;
				op->Run=BJX2_Op_MOVQ_LdDrPcReg;
				break;
//			case 0xF:	/* 30zF */
//				op->nmid=BJX2_NMID_MOVUW;
//				op->fmid=BJX2_FMID_LDDRPCREG;
//				op->Run=BJX2_Op_MOVUW_LdDrPcReg;
//				break;
#endif

			}
			break;

		case 0x1:	/* 31zz */
//			op->rn=(opw>>4)&15;
			op->rm=BJX2_REG_DR;
			switch(opw&15)
			{
#if 1
			case 0x0:	/* 31z0 */
//				op->rn+=16;
				op->nmid=BJX2_NMID_BRA;
				op->fmid=BJX2_FMID_PCREG;
				op->Run=BJX2_Op_BRA_PcReg;
				op->fl|=BJX2_OPFL_CTRLF;
				break;
			case 0x1:	/* 31z1 */
//				op->rn+=16;
				op->nmid=BJX2_NMID_BSR;
				op->fmid=BJX2_FMID_PCREG;
				op->Run=BJX2_Op_BSR_PcReg;
				op->fl|=BJX2_OPFL_CTRLF;
				break;
			case 0x2:	/* 31z2 */
//				op->rn+=16;
				op->nmid=BJX2_NMID_BT;
				op->fmid=BJX2_FMID_PCREG;
				op->Run=BJX2_Op_BT_PcReg;
				op->fl|=BJX2_OPFL_CTRLF;
				break;
			case 0x3:	/* 31z3 */
//				op->rn+=16;
				op->nmid=BJX2_NMID_BF;
				op->fmid=BJX2_FMID_PCREG;
				op->Run=BJX2_Op_BF_PcReg;
				op->fl|=BJX2_OPFL_CTRLF;
				break;
#endif

#if 0
			case 0x0:	/* 31z0 */
				op->nmid=BJX2_NMID_ADD;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_ADD_RegReg;
				break;
			case 0x1:	/* 31z1 */
				op->nmid=BJX2_NMID_SUB;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SUB_RegReg;
				break;
			case 0x2:	/* 31z2 */
				op->nmid=BJX2_NMID_ADC;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_ADC_RegReg;
				break;
			case 0x3:	/* 31z3 */
				op->nmid=BJX2_NMID_SBB;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SBB_RegReg;
				break;
			case 0x4:	/* 31z4 */
				op->nmid=BJX2_NMID_TST;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_TST_RegReg;
				break;
			case 0x5:	/* 31z5 */
				op->nmid=BJX2_NMID_AND;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_AND_RegReg;
				break;
			case 0x6:	/* 31z6 */
				op->nmid=BJX2_NMID_OR;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_OR_RegReg;
				break;
			case 0x7:	/* 31z7 */
				op->nmid=BJX2_NMID_XOR;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_XOR_RegReg;
				break;
#endif

#if 0
			case 0x8:	/* 31z8 */
				op->nmid=BJX2_NMID_CMPPL;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_CMPPL_Reg;
				break;
			case 0x9:	/* 31z9 */
				op->nmid=BJX2_NMID_CMPPZ;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_CMPPZ_Reg;
				break;
#endif

#if 1
			case 0x4:	/* 31n4 */
				op->rm=op->rn;
				op->nmid=BJX2_NMID_TST;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_TST_RegReg;
				break;
			case 0x5:	/* 31n5 */
				op->rm=op->rn;
				op->nmid=BJX2_NMID_TSTQ;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_TSTQ_RegReg;
				break;
#endif


#if 1
			case 0x6:	/* 31z6 */
				op->rm=BJX2_REG_DLR;
//				op->rn=(opw>>4)&15;
				op->nmid=BJX2_NMID_SHADQ;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SHADQ_RegReg;
				break;
			case 0x7:	/* 31z7 */
				op->rm=BJX2_REG_DLR;
//				op->rn=(opw>>4)&15;
				op->nmid=BJX2_NMID_SHLDQ;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SHLDQ_RegReg;
				break;
#endif

#if 1
			case 0x8:	/* 31z8 */
				op->nmid=BJX2_NMID_PUSH;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_PUSH_Reg;
				break;
			case 0x9:	/* 31z9 */
				op->rn+=BJX2_REG_PC;
				op->nmid=BJX2_NMID_PUSH;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_PUSH_Reg;
				break;
			case 0xA:	/* 31zA */
				op->nmid=BJX2_NMID_POP;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_POP_Reg;
				break;
			case 0xB:	/* 31zB */
				if((opw&0x00F0)==0x0000)
				{
					op->nmid=BJX2_NMID_RET;
					op->fmid=BJX2_FMID_Z;
					op->Run=BJX2_Op_RET_None;
					op->fl|=BJX2_OPFL_CTRLF;
					break;
				}
				op->rn+=BJX2_REG_PC;
				op->nmid=BJX2_NMID_POP;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_POP_Reg;
				break;
#endif

#if 0
			case 0xC:	/* 31zC */
				op->nmid=BJX2_NMID_CMPPL;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_CMPPL_Reg;
				break;
			case 0xD:	/* 31zD */
				op->nmid=BJX2_NMID_CMPPZ;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_CMPPZ_Reg;
				break;
#endif

#if 1
			case 0xC:	/* 31zC */
				op->nmid=BJX2_NMID_INVIC;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_INVIC_Reg;
				op->fl|=BJX2_OPFL_CTRLF;
				break;
			case 0xD:	/* 31zD */
				op->nmid=BJX2_NMID_INVDC;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_INVDC_Reg;
				op->fl|=BJX2_OPFL_CTRLF;
				break;
#endif

#if 1
			case 0xE:	/* 31zE */
				op->nmid=BJX2_NMID_FPUSH;
				op->fmid=BJX2_FMID_FREG;
				op->Run=BJX2_Op_PUSH_FpReg;
				break;
			case 0xF:	/* 31zF */
				op->nmid=BJX2_NMID_FPOP;
				op->fmid=BJX2_FMID_FREG;
				op->Run=BJX2_Op_POP_FpReg;
				break;
#endif

#if 0
			case 0xA:	/* 31zA */
				op->nmid=BJX2_NMID_LEAW;
				op->fmid=BJX2_FMID_LDDRPCREG;
				op->Run=BJX2_Op_LEAW_LdDrPcReg;
				break;
			case 0xB:	/* 31zB */
				op->nmid=BJX2_NMID_LEAD;
				op->fmid=BJX2_FMID_LDDRPCREG;
				op->Run=BJX2_Op_LEAD_LdDrPcReg;
				break;
#endif

#if 0
			case 0xC:	/* 31zC */
				op->nmid=BJX2_NMID_CMPEQ;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CMPEQ_RegReg;
				break;
			case 0xD:	/* 31zD */
				op->nmid=BJX2_NMID_CMPHI;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CMPHI_RegReg;
				break;
			case 0xE:	/* 31zE */
				op->nmid=BJX2_NMID_CMPGT;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CMPGT_RegReg;
				break;
#endif

#if 0
			case 0xF:	/* 31zF */
				op->nmid=BJX2_NMID_LEAB;
				op->fmid=BJX2_FMID_LDDRPCREG;
				op->Run=BJX2_Op_LEAB_LdDrPcReg;
				break;
#endif
			}
			break;

		case 0x2:	/* 32zz */
//			op->rn=(opw>>4)&15;
			switch(opw&15)
			{
			case 0x0:	/* 32z0 */
				op->nmid=BJX2_NMID_BRA;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_BRA_Reg;
				op->fl|=BJX2_OPFL_CTRLF;
				break;
			case 0x1:	/* 32z1 */
				op->nmid=BJX2_NMID_BSR;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_BSR_Reg;
				op->fl|=BJX2_OPFL_CTRLF;
				break;
			case 0x2:	/* 32z2 */
				op->nmid=BJX2_NMID_BT;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_BT_Reg;
				op->fl|=BJX2_OPFL_CTRLF;
				break;
			case 0x3:	/* 32z3 */
				op->nmid=BJX2_NMID_BF;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_BF_Reg;
				op->fl|=BJX2_OPFL_CTRLF;
				break;

			case 0x4:	/* 32z4 */
				op->nmid=BJX2_NMID_EXTUB;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_EXTUB_Reg;
				break;
			case 0x5:	/* 32z5 */
				op->nmid=BJX2_NMID_EXTUW;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_EXTUW_Reg;
				break;
			case 0x6:	/* 32z6 */
				op->nmid=BJX2_NMID_EXTSB;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_EXTSB_Reg;
				break;
			case 0x7:	/* 32z7 */
				op->nmid=BJX2_NMID_EXTSW;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_EXTSW_Reg;
				break;

#if 0
			case 0x8:	/* 32z8 */
				op->rn+=BJX2_REG_R16;
				op->nmid=BJX2_NMID_PUSH;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_PUSH_Reg;
				break;
			case 0x9:	/* 32z9 */
				op->rn+=BJX2_REG_PC+16;
				op->nmid=BJX2_NMID_PUSH;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_PUSH_Reg;
				break;
			case 0xA:	/* 32zA */
				op->rn+=BJX2_REG_R16;
				op->nmid=BJX2_NMID_POP;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_POP_Reg;
				break;
			case 0xB:	/* 32zB */
				op->rn+=BJX2_REG_PC+16;
				op->nmid=BJX2_NMID_POP;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_POP_Reg;
				break;
#endif

#if 0
			case 0xC:	/* 32zC */
				op->rm=((opw>>4)&15)+16;
				op->rn=BJX2_REG_DLR;
				op->nmid=BJX2_NMID_MOV;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MOV_RegReg;
				break;
			case 0xD:	/* 32zD */
				op->rm=BJX2_REG_DLR;
				op->rn=((opw>>4)&15)+16;
				op->nmid=BJX2_NMID_MOV;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MOV_RegReg;
				break;
			case 0xE:	/* 32zE */
				op->rm=((opw>>4)&15)+BJX2_REG_PC+16;
				op->rn=BJX2_REG_DLR;
				op->nmid=BJX2_NMID_MOV;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MOV_RegReg;
				break;
			case 0xF:	/* 32zF */
				op->rm=BJX2_REG_DLR;
				op->rn=((opw>>4)&15)+BJX2_REG_PC+16;
				op->nmid=BJX2_NMID_MOV;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MOV_RegReg;
				break;
#endif

			}
			break;

		case 0x3:	/* 33zz */
//			op->rn=(opw>>4)&15;
			switch(opw&15)
			{
			case 0x0:	/* 33z0 */
				op->nmid=BJX2_NMID_NOT;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_NOT_Reg;
				break;
			case 0x1:	/* 33z1 */
				op->nmid=BJX2_NMID_NEG;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_NEG_Reg;
				break;
			case 0x2:	/* 33z2 */
				op->nmid=BJX2_NMID_NEGC;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_NEGC_Reg;
				break;
			case 0x3:	/* 33z3 */
//				op->rn=(opw>>4)&15;
				op->nmid=BJX2_NMID_MOVNT;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_MOVNT_Reg;
				break;
			case 0x4:	/* 33z4 */
				op->nmid=BJX2_NMID_ROTL;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_ROTL_Reg;
				break;
			case 0x5:	/* 33z5 */
				op->nmid=BJX2_NMID_ROTR;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_ROTR_Reg;
				break;
			case 0x6:	/* 33z6 */
				op->nmid=BJX2_NMID_ROTCL;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_ROTCL_Reg;
				break;
			case 0x7:	/* 33z7 */
				op->nmid=BJX2_NMID_ROTCR;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_ROTCR_Reg;
				break;

			case 0x8:	/* 33z8 */
				op->nmid=BJX2_NMID_SHLL;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SHLL_Reg;
				break;
			case 0x9:	/* 33z9 */
				op->nmid=BJX2_NMID_SHLR;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SHLR_Reg;
				break;
			case 0xA:	/* 33zA */
				op->nmid=BJX2_NMID_SHAR;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SHAR_Reg;
				break;

			case 0xC:	/* 33zC */
//				op->rm=(opw>>4)&15;
				op->rm=((opw>>4)&15)|((opw>>7)&16);
				op->rn=BJX2_REG_DLR;
				op->nmid=BJX2_NMID_NEG;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_NEG_RegReg;
				break;
#if 0
			case 0xD:	/* 33zD */
//				op->rm=((opw>>4)&15)+16;
				op->rm=((opw>>4)&15)|((opw>>7)&16);
				op->rn=BJX2_REG_DLR;
				op->nmid=BJX2_NMID_NEG;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_NEG_RegReg;
				break;
#endif
			}
			break;

		case 0x4:	/* 34zz */
//			op->rn=(opw>>4)&15;
			switch(opw&15)
			{
			case 0x0:	/* 34z0 */
				op->imm=1;
				op->nmid=BJX2_NMID_SHLL;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_SHLL_ImmReg;
				break;
			case 0x1:	/* 34z1 */
				op->imm=2;
				op->nmid=BJX2_NMID_SHLL;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_SHLL_ImmReg;
				break;
			case 0x2:	/* 34z2 */
				op->imm=4;
				op->nmid=BJX2_NMID_SHLL;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_SHLL_ImmReg;
				break;
			case 0x3:	/* 34z3 */
				op->imm=8;
				op->nmid=BJX2_NMID_SHLL;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_SHLL_ImmReg;
				break;
			case 0x4:	/* 34z4 */
				op->imm=1;
				op->nmid=BJX2_NMID_SHLR;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_SHLR_ImmReg;
				break;
			case 0x5:	/* 34z5 */
				op->imm=2;
				op->nmid=BJX2_NMID_SHLR;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_SHLR_ImmReg;
				break;
			case 0x6:	/* 34z6 */
				op->imm=4;
				op->nmid=BJX2_NMID_SHLR;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_SHLR_ImmReg;
				break;
			case 0x7:	/* 34z7 */
				op->imm=8;
				op->nmid=BJX2_NMID_SHLR;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_SHLR_ImmReg;
				break;
			case 0x8:	/* 34z8 */
				op->imm=1;
				op->nmid=BJX2_NMID_SHAR;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_SHAR_ImmReg;
				break;
			case 0x9:	/* 34z9 */
				op->imm=2;
				op->nmid=BJX2_NMID_SHAR;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_SHAR_ImmReg;
				break;
			case 0xA:	/* 34zA */
				op->imm=4;
				op->nmid=BJX2_NMID_SHAR;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_SHAR_ImmReg;
				break;
			case 0xB:	/* 34zB */
				op->imm=8;
				op->nmid=BJX2_NMID_SHAR;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_SHAR_ImmReg;
				break;
			case 0xC:	/* 34zC */
				op->imm=16;
				op->nmid=BJX2_NMID_SHLL;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_SHLL_ImmReg;
				break;
			case 0xD:	/* 34zD */
				op->imm=16;
				op->nmid=BJX2_NMID_SHLR;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_SHLR_ImmReg;
				break;
			case 0xE:	/* 34zE */
				op->imm=16;
				op->nmid=BJX2_NMID_SHAR;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_SHAR_ImmReg;
				break;
			case 0xF:	/* 34zF */
				op->imm=31;
				op->nmid=BJX2_NMID_SHAR;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_SHAR_ImmReg;
				break;
			}
			break;

		case 0x5:	/* 35zz */
//			op->rn=(opw>>4)&15;
			switch(opw&15)
			{
#if 0
			case 0x0:	/* 35z0 */
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_REGSTDRABS;
				op->Run=BJX2_Op_MOVB_RegStDrAbs;
				break;
			case 0x1:	/* 35z1 */
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_REGSTDRABS;
				op->Run=BJX2_Op_MOVW_RegStDrAbs;
				break;
			case 0x2:	/* 35z2 */
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_REGSTDRABS;
				op->Run=BJX2_Op_MOVL_RegStDrAbs;
				break;
			case 0x3:	/* 35z3 */
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_REGSTDRABS;
				op->Run=BJX2_Op_MOVQ_RegStDrAbs;
				break;
//			case 0x3:	/* 35z3 */
//				op->nmid=BJX2_NMID_MOVUB;
//				op->fmid=BJX2_FMID_REGSTDRABS;
//				op->Run=BJX2_Op_MOVUB_LdDrAbsReg;
//				break;
#endif

#if 0
			case 0x4:	/* 35z4 */
				op->nmid=BJX2_NMID_MOVUB;
				op->fmid=BJX2_FMID_LDDRPCREG;
				op->Run=BJX2_Op_MOVUB_LdDrPcReg;
				break;
			case 0x5:	/* 35z5 */
				op->nmid=BJX2_NMID_MOVUW;
				op->fmid=BJX2_FMID_LDDRPCREG;
				op->Run=BJX2_Op_MOVUW_LdDrPcReg;
				break;
			case 0x6:	/* 35z6 */
				op->nmid=BJX2_NMID_MOVUL;
				op->fmid=BJX2_FMID_LDDRPCREG;
				op->Run=BJX2_Op_MOVUL_LdDrPcReg;
				break;
//			case 0x7:	/* 35z7 */
//				op->nmid=BJX2_NMID_MOVQ;
//				op->fmid=BJX2_FMID_LDDRPCREG;
//				op->Run=BJX2_Op_MOVQ_LdDrPcReg;
//				break;
#endif

#if 0
			case 0x4:	/* 35z4 */
				op->rm=(opw>>4)&15;
				op->rn=BJX2_REG_GBR;
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_REGSTDRREG;
				op->Run=BJX2_Op_MOVB_RegStDrReg;
				break;
			case 0x5:	/* 35z5 */
				op->rm=(opw>>4)&15;
				op->rn=BJX2_REG_GBR;
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_REGSTDRREG;
				op->Run=BJX2_Op_MOVW_RegStDrReg;
				break;
			case 0x6:	/* 35z6 */
				op->rm=(opw>>4)&15;
				op->rn=BJX2_REG_GBR;
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_REGSTDRREG;
				op->Run=BJX2_Op_MOVL_RegStDrReg;
				break;
//			case 0x7:	/* 35z7 */
//				op->rm=BJX2_REG_GBR;
//				op->rn=(opw>>4)&15;
//				op->nmid=BJX2_NMID_MOVUB;
//				op->fmid=BJX2_FMID_LDDRREGREG;
//				op->Run=BJX2_Op_MOVUB_LdDrRegReg;
//				break;
#endif

#if 0
			case 0x8:	/* 35z8 */
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_LDDRABSREG;
				op->Run=BJX2_Op_MOVB_LdDrAbsReg;
				break;
			case 0x9:	/* 35z9 */
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_LDDRABSREG;
				op->Run=BJX2_Op_MOVW_LdDrAbsReg;
				break;
			case 0xA:	/* 35zA */
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_LDDRABSREG;
				op->Run=BJX2_Op_MOVL_LdDrAbsReg;
				break;
			case 0xB:	/* 35zB */
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_LDDRABSREG;
				op->Run=BJX2_Op_MOVQ_LdDrAbsReg;
				break;
//			case 0xB:	/* 35zB */
//				op->nmid=BJX2_NMID_MOVUW;
//				op->fmid=BJX2_FMID_LDDRABSREG;
//				op->Run=BJX2_Op_MOVUW_LdDrAbsReg;
//				break;

			case 0xC:	/* 35zC */
				op->nmid=BJX2_NMID_MOVUB;
				op->fmid=BJX2_FMID_LDDRABSREG;
				op->Run=BJX2_Op_MOVUB_LdDrAbsReg;
				break;
			case 0xD:	/* 35zD */
				op->nmid=BJX2_NMID_MOVUW;
				op->fmid=BJX2_FMID_LDDRABSREG;
				op->Run=BJX2_Op_MOVUW_LdDrAbsReg;
				break;
			case 0xE:	/* 35zE */
				op->nmid=BJX2_NMID_MOVUL;
				op->fmid=BJX2_FMID_LDDRABSREG;
				op->Run=BJX2_Op_MOVUL_LdDrAbsReg;
				break;
#endif

#if 0
			case 0xC:	/* 35zC */
				op->rm=BJX2_REG_GBR;
				op->rn=(opw>>4)&15;
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_LDDRREGREG;
				op->Run=BJX2_Op_MOVB_LdDrRegReg;
				break;
			case 0xD:	/* 35zD */
				op->rm=BJX2_REG_GBR;
				op->rn=(opw>>4)&15;
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_LDDRREGREG;
				op->Run=BJX2_Op_MOVW_LdDrRegReg;
				break;
			case 0xE:	/* 35zE */
				op->rm=BJX2_REG_GBR;
				op->rn=(opw>>4)&15;
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_LDDRREGREG;
				op->Run=BJX2_Op_MOVL_LdDrRegReg;
				break;
//			case 0xF:	/* 35zF */
//				op->rm=BJX2_REG_GBR;
//				op->rn=(opw>>4)&15;
//				op->nmid=BJX2_NMID_MOVUW;
//				op->fmid=BJX2_FMID_LDDRREGREG;
//				op->Run=BJX2_Op_MOVUW_LdDrRegReg;
//				break;
#endif

			default:
				ret=-1;
				break;
			}
			break;
			
		case 0x6:	/* 36zz */
//			op->rn=(opw>>4)&15;
			switch(opw&15)
			{
			case 0x0:	/* 36z0 */
				op->imm=32;
				op->nmid=BJX2_NMID_SHLL;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_SHLL_ImmReg;
				break;
			case 0x1:	/* 36z1 */
				op->imm=32;
				op->nmid=BJX2_NMID_SHLR;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_SHLR_ImmReg;
				break;
			case 0x2:	/* 36z2 */
				op->imm=32;
				op->nmid=BJX2_NMID_SHAR;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_SHAR_ImmReg;
				break;

			case 0x3:	/* 36z3 */
				op->nmid=BJX2_NMID_TRAP;
				op->fmid=BJX2_FMID_IMM;
				op->Run=BJX2_Op_TRAP_Imm;
				break;

			case 0x4:	/* 36z4 */
				op->nmid=BJX2_NMID_EXTUL;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_EXTUL_Reg;
				break;
			case 0x5:	/* 36z5 */
				op->nmid=BJX2_NMID_EXTSL;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_EXTSL_Reg;
				break;

			case 0x6:	/* 36z6 */
				op->rm=BJX2_REG_DLR;
//				op->rn=(opw>>4)&15;
				op->nmid=BJX2_NMID_SHAD;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SHAD_RegReg;
				break;
			case 0x7:	/* 36z7 */
				op->rm=BJX2_REG_DLR;
//				op->rn=(opw>>4)&15;
				op->nmid=BJX2_NMID_SHLD;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SHLD_RegReg;
				break;

			case 0x8:	/* 36z8 */
				op->nmid=BJX2_NMID_TRAP;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_TRAP_Reg;
				break;

			case 0x9:	/* 36z9 */
				op->nmid=BJX2_NMID_WEXMD;
				op->fmid=BJX2_FMID_IMM;
				op->Run=BJX2_Op_WEXMD_Imm;
				op->imm=op->rn;
				ctx->v_wexmd=op->imm;
				break;

#if 0
			case 0x8:	/* 36z8 */
				op->rm=BJX2_REG_DLR;
//				op->rn=(opw>>4)&15;
				op->nmid=BJX2_NMID_BRA;
				op->fmid=BJX2_FMID_PCREG;
				op->Run=BJX2_Op_BRA_PcReg;
				op->fl|=BJX2_OPFL_CTRLF;
				break;
			case 0x9:	/* 36z9 */
				op->rm=BJX2_REG_DLR;
//				op->rn=(opw>>4)&15;
				op->nmid=BJX2_NMID_BSR;
				op->fmid=BJX2_FMID_PCREG;
				op->Run=BJX2_Op_BSR_PcReg;
				op->fl|=BJX2_OPFL_CTRLF;
				break;
#endif

			case 0xA:	/* 36zA */
				op->rm=BJX2_REG_DLR;
//				op->rn=(opw>>4)&15;
				op->nmid=BJX2_NMID_SWAPB;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SWAPB_Reg;
				break;
			case 0xB:	/* 36zB */
				op->rm=BJX2_REG_DLR;
//				op->rn=(opw>>4)&15;
				op->nmid=BJX2_NMID_SWAPW;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SWAPW_Reg;
				break;

#if 0
			case 0xC:	/* 36zC */
				op->rn=BJX2_REG_DLR;
				op->rm=(opw>>4)&15;
				op->nmid=BJX2_NMID_ADD;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_ADD_RegReg;
				break;
#endif

#if 1
			case 0xD:	/* 36zD */
//				op->rn=(opw>>4)&15;
				op->rm=BJX2_REG_DLR;
				op->nmid=BJX2_NMID_CMPHS;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CMPHS_RegReg;
				break;
			case 0xE:	/* 36zE */
//				op->rn=(opw>>4)&15;
				op->rm=BJX2_REG_DLR;
				op->nmid=BJX2_NMID_CMPGE;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CMPGE_RegReg;
				break;
#endif

			case 0xF:	/* 36zF */
//				op->rn=(opw>>4)&15;
				op->nmid=BJX2_NMID_MOVT;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_MOVT_Reg;
				break;
			}
			break;

		case 0x7:	/* 37zz */
//			op->rn=(opw>>4)&15;
			switch(opw&15)
			{
			case 0x0:	/* 37z0 */
//				op->rm=BJX2_REG_DLR;
//				op->rn=(opw>>4)&15;
				op->rm=op->rn;
				op->nmid=BJX2_NMID_SWAPB;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SWAPB_Reg;
				break;
			case 0x1:	/* 37z1 */
//				op->rm=BJX2_REG_DLR;
//				op->rn=(opw>>4)&15;
				op->rm=op->rn;
				op->nmid=BJX2_NMID_SWAPW;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SWAPW_Reg;
				break;
			case 0x2:	/* 37z2 */
//				op->rm=BJX2_REG_DLR;
//				op->rn=(opw>>4)&15;
				op->rm=op->rn;
				op->nmid=BJX2_NMID_SWAP8B;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SWAP8B_Reg;
				break;
			case 0x3:	/* 37z3 */
//				op->rm=BJX2_REG_DLR;
//				op->rn=(opw>>4)&15;
				op->rm=op->rn;
				op->nmid=BJX2_NMID_SWAPL;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SWAPL_Reg;
				break;
			case 0x4:	/* 37z4 */
//				op->rm=BJX2_REG_DLR;
//				op->rn=(opw>>4)&15;
				op->rm=op->rn;
				op->nmid=BJX2_NMID_SWAPLB;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SWAPLB_Reg;
				break;
			case 0x5:	/* 37z5 */
//				op->rm=BJX2_REG_DLR;
//				op->rn=(opw>>4)&15;
				op->rm=op->rn;
				op->nmid=BJX2_NMID_SWAPLW;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SWAPLW_Reg;
				break;
			case 0x6:	/* 37z6 */
//				op->rm=BJX2_REG_DLR;
//				op->rn=(opw>>4)&15;
				op->rm=op->rn;
				op->nmid=BJX2_NMID_SWCPLB;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SWCPLB_Reg;
				break;
			case 0x7:	/* 37z7 */
//				op->rm=BJX2_REG_DLR;
//				op->rn=(opw>>4)&15;
				op->rm=op->rn;
				op->nmid=BJX2_NMID_SWCPLW;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SWCPLW_Reg;
				break;
			case 0x8:	/* 37z8 */
//				op->rm=BJX2_REG_DLR;
//				op->rn=(opw>>4)&15;
				op->rm=op->rn;
				op->nmid=BJX2_NMID_SWAPMB;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SWAPMB_Reg;
				break;
			case 0x9:	/* 37z9 */
//				op->rm=BJX2_REG_DLR;
//				op->rn=(opw>>4)&15;
				op->rm=op->rn;
				op->nmid=BJX2_NMID_SWAPMW;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SWAPMW_Reg;
				break;
			case 0xA:	/* 37zA */
//				op->rm=BJX2_REG_DLR;
//				op->rn=(opw>>4)&15;
				op->rm=op->rn;
				op->nmid=BJX2_NMID_SWCPMB;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SWCPMB_Reg;
				break;
			case 0xB:	/* 37zB */
//				op->rm=BJX2_REG_DLR;
//				op->rn=(opw>>4)&15;
				op->rm=op->rn;
				op->nmid=BJX2_NMID_SWCPMW;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SWCPMW_Reg;
				break;
			case 0xC:	/* 37zC */
//				op->rm=BJX2_REG_DLR;
//				op->rn=(opw>>4)&15;
				op->rm=op->rn;
				op->nmid=BJX2_NMID_SWAPHB;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SWAPHB_Reg;
				break;
			case 0xD:	/* 37zD */
//				op->rm=BJX2_REG_DLR;
//				op->rn=(opw>>4)&15;
				op->rm=op->rn;
				op->nmid=BJX2_NMID_SWAPHW;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SWAPHW_Reg;
				break;
			case 0xE:	/* 37zE */
//				op->rm=BJX2_REG_DLR;
//				op->rn=(opw>>4)&15;
				op->rm=op->rn;
				op->nmid=BJX2_NMID_SWCPHB;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SWCPHB_Reg;
				break;
			case 0xF:	/* 37zF */
//				op->rm=BJX2_REG_DLR;
//				op->rn=(opw>>4)&15;
				op->rm=op->rn;
				op->nmid=BJX2_NMID_SWCPHW;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SWCPHW_Reg;
				break;
			}
			break;

#if 0
		case 0x8:	/* 38zz */
			op->rn=(opw>>4)&15;
			switch(opw&15)
			{
//			case 0x0:	/* 38z0 */
//				break;

			case 0x2:	/* 38z2 */
				op->nmid=BJX2_NMID_FPUSH;
				op->fmid=BJX2_FMID_FREG;
				op->Run=BJX2_Op_PUSH_FpReg;
				break;
			case 0x3:	/* 38z3 */
				op->nmid=BJX2_NMID_FPOP;
				op->fmid=BJX2_FMID_FREG;
				op->Run=BJX2_Op_POP_FpReg;
				break;

			case 0x6:	/* 38z6 */
				op->rm=BJX2_REG_DLR;
				op->rn=(opw>>4)&15;
				op->nmid=BJX2_NMID_SHADQ;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SHADQ_RegReg;
				break;
			case 0x7:	/* 38z7 */
				op->rm=BJX2_REG_DLR;
				op->rn=(opw>>4)&15;
				op->nmid=BJX2_NMID_SHLDQ;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SHLDQ_RegReg;
				break;
			}
			break;
#endif

#if 0
		case 0xF:	/* 3Fnj */
			op->rn=(opw>>4)&15;
			op->imm=opw&15;
			op->nmid=BJX2_NMID_LEAB;
			op->fmid=BJX2_FMID_LDDR4PCREG;
			op->Run=BJX2_Op_LEAB_LdDr4PcReg;
			break;
#endif

#if 0
		case 0xC:	/* 3Czz */
			op->imm=opw&15;
			op->rm=((opw>>4)&15)|16;
			op->rn=BJX2_REG_SP;
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVL_RegStRegDisp;
			break;
		case 0xD:	/* 3Dzz */
			op->imm=opw&15;
			op->rm=((opw>>4)&15)|16;
			op->rn=BJX2_REG_SP;
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVQ_RegStRegDisp;
			break;
		case 0xE:	/* 3Ezz */
			op->imm=opw&15;
			op->rn=((opw>>4)&15)|16;
			op->rm=BJX2_REG_SP;
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVL_LdRegDispReg;
			break;
		case 0xF:	/* 3Fzz */
			op->imm=opw&15;
			op->rn=((opw>>4)&15)|16;
			op->rm=BJX2_REG_SP;
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVQ_LdRegDispReg;
			break;
#endif
	
		default:
			ret=-1;
			break;
		}
		break;

	case 0x4:	/* 4zzz */
		op->rn=(opw>>4)&15;
		op->rm=opw&15;
		op->imm=opw&15;
		switch((opw>>8)&15)
		{
#if 1
		case 0x0:	/* 40zz */
			op->imm=opw&15;
			op->rm=op->rn;
			op->rn=BJX2_REG_SP;
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVL_RegStRegDisp;
			break;
		case 0x1:	/* 41zz */
			op->imm=opw&15;
			op->rm=op->rn;
			op->rn=BJX2_REG_SP;
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVQ_RegStRegDisp;
			break;
		case 0x2:	/* 42zz */
			op->imm=opw&15;
			op->rm=op->rn|16;
			op->rn=BJX2_REG_SP;
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVL_RegStRegDisp;
			break;
		case 0x3:	/* 43zz */
			op->imm=opw&15;
			op->rm=op->rn|16;
			op->rn=BJX2_REG_SP;
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVQ_RegStRegDisp;
			break;
		case 0x4:	/* 44zz */
			op->imm=opw&15;
			op->rm=BJX2_REG_SP;
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVL_LdRegDispReg;
			break;
		case 0x5:	/* 45zz */
			op->imm=opw&15;
			op->rm=BJX2_REG_SP;
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVQ_LdRegDispReg;
			break;
		case 0x6:	/* 46zz */
			op->imm=opw&15;
			op->rm=BJX2_REG_SP;
			op->rn=op->rn|16;
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVL_LdRegDispReg;
			break;
		case 0x7:	/* 47zz */
			op->imm=opw&15;
			op->rm=BJX2_REG_SP;
			op->rn=op->rn|16;
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVQ_LdRegDispReg;
			break;
#endif

#if 0
		case 0x0:	/* 40zz */
			if(op->rn==15)
			{
				op->nmid=BJX2_NMID_BRA;
				op->fmid=BJX2_FMID_PCDR4;
				op->Run=BJX2_Op_BRA_PcDr4;
				op->fl|=BJX2_OPFL_CTRLF;
				break;
			}
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_REGSTDR4PC;
			op->Run=BJX2_Op_MOVB_RegStDr4Pc;
			break;
		case 0x1:	/* 41zz */
			if(op->rn==15)
			{
				op->nmid=BJX2_NMID_BSR;
				op->fmid=BJX2_FMID_PCDR4;
				op->Run=BJX2_Op_BSR_PcDr4;
				op->fl|=BJX2_OPFL_CTRLF;
				break;
			}
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_REGSTDR4PC;
			op->Run=BJX2_Op_MOVW_RegStDr4Pc;
			break;
		case 0x2:	/* 42zz */
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTDR4PC;
			op->Run=BJX2_Op_MOVL_RegStDr4Pc;
			break;
		case 0x3:	/* 43zz */
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTDR4PC;
			op->Run=BJX2_Op_MOVQ_RegStDr4Pc;
			break;

//		case 0x3:	/* 43zz */
//			op->nmid=BJX2_NMID_MOVUB;
//			op->fmid=BJX2_FMID_LDDR4PCREG;
//			op->Run=BJX2_Op_MOVUB_LdDr4PcReg;
//			break;
		case 0x4:	/* 44zz */
			if(op->rn==15)
			{
				op->nmid=BJX2_NMID_BT;
				op->fmid=BJX2_FMID_PCDR4;
				op->Run=BJX2_Op_BT_PcDr4;
				op->fl|=BJX2_OPFL_CTRLF;
				break;
			}
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_LDDR4PCREG;
			op->Run=BJX2_Op_MOVB_LdDr4PcReg;
			break;
		case 0x5:	/* 45zz */
			if(op->rn==15)
			{
				op->nmid=BJX2_NMID_BF;
				op->fmid=BJX2_FMID_PCDR4;
				op->Run=BJX2_Op_BF_PcDr4;
				op->fl|=BJX2_OPFL_CTRLF;
				break;
			}
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_LDDR4PCREG;
			op->Run=BJX2_Op_MOVW_LdDr4PcReg;
			break;
		case 0x6:	/* 46zz */
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDDR4PCREG;
			op->Run=BJX2_Op_MOVL_LdDr4PcReg;
			break;
		case 0x7:	/* 47zz */
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDDR4PCREG;
			op->Run=BJX2_Op_MOVQ_LdDr4PcReg;
			break;
//		case 0x7:	/* 47zz */
//			op->nmid=BJX2_NMID_MOVUW;
///			op->fmid=BJX2_FMID_LDDR4PCREG;
//			op->Run=BJX2_Op_MOVUW_LdDr4PcReg;
//			break;
#endif

#if 0
		case 0x8:	/* 48zz */
			op->nmid=BJX2_NMID_MOV;
			op->fmid=BJX2_FMID_DR4REG;
			op->Run=BJX2_Op_MOV_Dr4Reg;
			break;
		case 0x9:	/* 49zz */
			op->nmid=BJX2_NMID_ADD;
			op->fmid=BJX2_FMID_DR4REG;
			op->Run=BJX2_Op_ADD_Dr4Reg;
			break;
#endif

#if 1
		case 0x8:	/* 48zz */
			op->rn+=BJX2_REG_PC;
			op->nmid=BJX2_NMID_MOV;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_MOV_RegReg;
			break;
		case 0x9:	/* 49zz */
			op->rm+=BJX2_REG_PC;
			op->nmid=BJX2_NMID_MOV;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_MOV_RegReg;
			break;
#endif

		case 0xA:	/* 4Azz */
			op->rn=(opw>>4)&15;
			op->rm=opw&15;
			op->rn+=BJX2_REG_R0B;
			op->nmid=BJX2_NMID_MOV;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_MOV_RegReg;
			break;
		case 0xB:	/* 4Bzz */
			op->rn=(opw>>4)&15;
			op->rm=opw&15;
			op->rm+=BJX2_REG_R0B;
			op->nmid=BJX2_NMID_MOV;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_MOV_RegReg;
			break;

		case 0xC:	/* 4Czz */
			op->rn=(opw>>4)&15;
			op->rm=opw&15;
			op->imm=0;
			if(op->rm==0)
			{
				op->nmid=BJX2_NMID_LEAB;
				op->fmid=BJX2_FMID_LDDRPCREG;
				op->Run=BJX2_Op_LEAB_LdDrPcReg;
				break;
			}
			if(op->rm==1)
				{ op->rm=BJX2_REG_GBR; }
			op->nmid=BJX2_NMID_LEAB;
			op->fmid=BJX2_FMID_LDDRREGREG;
			op->Run=BJX2_Op_LEAB_LdDrRegReg;
			break;
		case 0xD:	/* 4Dzz */
			op->rn=(opw>>4)&15;
			op->rm=opw&15;
			op->imm=0;
			if(op->rm==0)
			{
				op->nmid=BJX2_NMID_LEAW;
				op->fmid=BJX2_FMID_LDDRPCREG;
				op->Run=BJX2_Op_LEAW_LdDrPcReg;
				break;
			}
			if(op->rm==1)
				{ break; }
			op->nmid=BJX2_NMID_LEAW;
			op->fmid=BJX2_FMID_LDDRREGREG;
			op->Run=BJX2_Op_LEAW_LdDrRegReg;
			break;
		case 0xE:	/* 4Ezz */
			op->rn=(opw>>4)&15;
			op->rm=opw&15;
			op->imm=0;
			if(op->rm==0)
			{
				op->nmid=BJX2_NMID_LEAD;
				op->fmid=BJX2_FMID_LDDRPCREG;
				op->Run=BJX2_Op_LEAD_LdDrPcReg;
				break;
			}
			if(op->rm==1)
				{ break; }
			op->nmid=BJX2_NMID_LEAD;
			op->fmid=BJX2_FMID_LDDRREGREG;
			op->Run=BJX2_Op_LEAD_LdDrRegReg;
			break;
		case 0xF:	/* 4Fzz */
			op->rn=(opw>>4)&15;
			op->rm=opw&15;
			op->imm=0;
			if(op->rm==0)
			{
				op->nmid=BJX2_NMID_LEAQ;
				op->fmid=BJX2_FMID_LDDRPCREG;
				op->Run=BJX2_Op_LEAQ_LdDrPcReg;
				break;
			}
			if(op->rm==1)
				{ break; }
			op->nmid=BJX2_NMID_LEAQ;
			op->fmid=BJX2_FMID_LDDRREGREG;
			op->Run=BJX2_Op_LEAQ_LdDrRegReg;
			break;
		}
		break;

	case 0x5:	/* 5zzz */
		op->rn=(opw>>4)&15;
		op->rm=opw&15;
		op->imm=opw&15;
		switch((opw>>8)&15)
		{
		case 0x0:	/* 50zz */
			op->nmid=BJX2_NMID_MOVUB;
			op->fmid=BJX2_FMID_LDREGREG;
			op->Run=BJX2_Op_MOVUB_LdRegReg;
			break;
		case 0x1:	/* 51zz */
			op->nmid=BJX2_NMID_MOVUW;
			op->fmid=BJX2_FMID_LDREGREG;
			op->Run=BJX2_Op_MOVUW_LdRegReg;
			break;
		case 0x2:	/* 52zz */
			if(op->rm==0)
			{
				op->nmid=BJX2_NMID_MOVUB;
				op->fmid=BJX2_FMID_LDDRPCREG;
				op->Run=BJX2_Op_MOVUB_LdDrPcReg;
				break;
			}
			op->nmid=BJX2_NMID_MOVUB;
			op->fmid=BJX2_FMID_LDDRREGREG;
			op->Run=BJX2_Op_MOVUB_LdDrRegReg;
			if(op->rm==1)
			{
				op->rm=BJX2_REG_GBR;
				op->ro=BJX2_REG_DLR;
				op->Run=BJX2_Op_MOVUB_LdReg2RegB;
			}
			break;
		case 0x3:	/* 53zz */
			if(op->rm==0)
			{
				op->nmid=BJX2_NMID_MOVUW;
				op->fmid=BJX2_FMID_LDDRPCREG;
				op->Run=BJX2_Op_MOVUW_LdDrPcReg;
				break;
			}
			op->nmid=BJX2_NMID_MOVUW;
			op->fmid=BJX2_FMID_LDDRREGREG;
			op->Run=BJX2_Op_MOVUW_LdDrRegReg;
			if(op->rm==1)
			{
				op->rm=BJX2_REG_GBR;
				op->ro=BJX2_REG_DLR;
				op->Run=BJX2_Op_MOVUW_LdReg2RegB;
			}
			break;

		case 0x4:	/* 54nm */
			op->rn=(opw>>4)&15;
			op->rm=(opw   )&15;
			op->nmid=BJX2_NMID_TSTQ;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_TSTQ_RegReg;
			break;

#if 0
		case 0x5:	/* 55nm */
		case 0x6:	/* 56nm */
		case 0x7:	/* 57nm */
			op->rn=((opw>>4)&15)+((opw&0x0200)?16:0);
			op->rm=((opw   )&15)+((opw&0x0100)?16:0);
			op->nmid=BJX2_NMID_MOV;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_MOV_RegReg;
			break;
#endif

#if 1
		case 0x5:	/* 55zz */
			op->nmid=BJX2_NMID_CMPQEQ;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_CMPQEQ_RegReg;
			break;
#endif

#if 1
		case 0x6:	/* 56zz */
			op->rn=(opw>>4)&15;
			op->rm=(opw   )&15;
			op->nmid=BJX2_NMID_MULS;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_MULS_RegReg;
			break;
		case 0x7:	/* 57zz */
			op->rn=(opw>>4)&15;
			op->rm=(opw   )&15;
			op->nmid=BJX2_NMID_MULU;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_MULU_RegReg;
			break;
#endif

#if 1
		case 0x8:	/* 58nm */
			op->rn=(opw>>4)&15;
			op->rm=(opw   )&15;
			op->ro=0;
			op->nmid=BJX2_NMID_ADD;
			op->fmid=BJX2_FMID_REGDRREG;
			op->Run=BJX2_Op_ADD_RegDrReg;
			break;
		case 0x9:	/* 59nm */
			op->rn=(opw>>4)&15;
			op->rm=(opw   )&15;
			op->ro=0;
			op->nmid=BJX2_NMID_SUB;
			op->fmid=BJX2_FMID_REGDRREG;
			op->Run=BJX2_Op_SUB_RegDrReg;
			break;
#endif

#if 0
		case 0xA:	/* 5Anm */
			op->rn=(opw>>4)&15;
			op->rm=(opw   )&15;
			op->ro=0;
			op->nmid=BJX2_NMID_AND;
			op->fmid=BJX2_FMID_REGDRREG;
			op->Run=BJX2_Op_AND_RegDrReg;
			break;
		case 0xB:	/* 5Bnm */
			op->rn=(opw>>4)&15;
			op->rm=(opw   )&15;
			op->ro=0;
			op->nmid=BJX2_NMID_OR;
			op->fmid=BJX2_FMID_REGDRREG;
			op->Run=BJX2_Op_OR_RegDrReg;
			break;
		case 0xC:	/* 5Cnm */
			op->rn=(opw>>4)&15;
			op->rm=(opw   )&15;
			op->ro=0;
			op->nmid=BJX2_NMID_XOR;
			op->fmid=BJX2_FMID_REGDRREG;
			op->Run=BJX2_Op_XOR_RegDrReg;
			break;
#endif

#if 0
		case 0xD:	/* 5Dzz */
			op->nmid=BJX2_NMID_CMPQHI;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_CMPQHI_RegReg;
			break;
		case 0xE:	/* 5Ezz */
			op->nmid=BJX2_NMID_CMPQGT;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_CMPQGT_RegReg;
			break;
		case 0xF:	/* 5Fzz */
			op->nmid=BJX2_NMID_CMPQEQ;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_CMPQEQ_RegReg;
			break;
#endif

#if 1
		case 0xA:	/* 5Anm */
			op->rn=(opw>>4)&15;
			op->rm=(opw   )&15;
			op->ro=0;
			op->nmid=BJX2_NMID_MULS;
			op->fmid=BJX2_FMID_REGDRREG;
			op->Run=BJX2_Op_MULS_RegRegReg;
			break;
#endif

#if 1
		case 0xB:	/* 5Bzz */
			op->nmid=BJX2_NMID_CMPQHI;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_CMPQHI_RegReg;
			break;
		case 0xC:	/* 5Czz */
			op->nmid=BJX2_NMID_CMPQGT;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_CMPQGT_RegReg;
			break;
#endif

#if 1
		case 0xD:	/* 5Dnm */
			op->rn=(opw>>4)&15;
			op->rm=(opw   )&15;
			op->ro=0;
			op->nmid=BJX2_NMID_AND;
			op->fmid=BJX2_FMID_REGDRREG;
			op->Run=BJX2_Op_AND_RegDrReg;
			break;
		case 0xE:	/* 5Enm */
			op->rn=(opw>>4)&15;
			op->rm=(opw   )&15;
			op->ro=0;
			op->nmid=BJX2_NMID_OR;
			op->fmid=BJX2_FMID_REGDRREG;
			op->Run=BJX2_Op_OR_RegDrReg;
			break;
		case 0xF:	/* 5Fnm */
			op->rn=(opw>>4)&15;
			op->rm=(opw   )&15;
			op->ro=0;
			op->nmid=BJX2_NMID_XOR;
			op->fmid=BJX2_FMID_REGDRREG;
			op->Run=BJX2_Op_XOR_RegDrReg;
			break;
#endif

		}
		break;

	case 0x6:	/* 6zzz */
		op->rn=(opw>>4)&15;
		op->rm=opw&15;
		switch((opw>>8)&15)
		{

		case 0x0:	/* 60nm */
			op->ro=op->rm;
			op->rm=op->rn;
			op->nmid=BJX2_NMID_FADD;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FADDD_RegRegReg;
			break;
		case 0x1:	/* 61nm */
			op->ro=op->rm;
			op->rm=op->rn;
			op->nmid=BJX2_NMID_FSUB;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FSUBD_RegRegReg;
			break;
		case 0x2:	/* 62nm */
			op->ro=op->rm;
			op->rm=op->rn;
			op->nmid=BJX2_NMID_FMUL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FMULD_RegRegReg;
			break;
		case 0x3:	/* 63nm */
			op->nmid=BJX2_NMID_FLDCF;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_FLDCF_GRegReg;
			break;
		case 0x4:	/* 64nm */
			op->nmid=BJX2_NMID_FCMPEQ;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_FCMPEQ_GRegReg;
			break;
		case 0x5:	/* 65nm */
			op->nmid=BJX2_NMID_FCMPGT;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_FCMPGT_GRegReg;
			break;
		case 0x6:	/* 66nm */
			op->nmid=BJX2_NMID_FSTCF;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_FSTCF_GRegReg;
			break;

		case 0x7:	/* 67nz */
			switch(opw&15)
			{
			default:
				break;
			}
			break;

#if 1
		case 0x8:	/* 68zz */
			op->imm=opw&15;
			op->rn+=16;
			op->nmid=BJX2_NMID_ADD;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_ADD_ImmReg;
			break;
		case 0x9:	/* 69zz */
			op->imm=(opw&15)|(~15);
			op->rn+=16;
			op->nmid=BJX2_NMID_ADD;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_ADD_ImmReg;
			break;
		case 0xA:	/* 6Azz */
			op->imm=opw&15;
			op->rn+=16;
			op->nmid=BJX2_NMID_LDIZ;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_MOV_ImmReg;
			break;
		case 0xB:	/* 6Bzz */
			op->imm=(opw&15)|(~15);
			op->rn+=16;
			op->nmid=BJX2_NMID_LDIN;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_MOV_ImmReg;
			break;
#endif

#if 1
		case 0xC:	/* 2Czz */
			op->rn+=16;
			op->imm=opw&15;
			op->nmid=BJX2_NMID_CMPEQ;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_CMPEQ_ImmReg;
			break;
		case 0xD:	/* 2Dzz */
			op->rn+=16;
			op->imm=opw|(~15);
			op->nmid=BJX2_NMID_CMPEQ;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_CMPEQ_ImmReg;
			break;
		case 0xE:	/* 2Ezz */
			op->rn+=16;
			op->imm=opw&15;
			op->nmid=BJX2_NMID_CMPGT;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_CMPGT_ImmReg;
			break;
		case 0xF:	/* 2Fzz */
			op->rn+=16;
			op->imm=opw&15;
			op->nmid=BJX2_NMID_CMPGE;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_CMPGE_ImmReg;
			break;
#endif

		default:
			break;
		}
		break;

	case 0x8:	/* 8zzz */
		op->rn=(opw>>4)&15;
		op->rm=opw&15;
		switch((opw>>8)&15)
		{
		case 0x0:	/* 80zz */
			op->nmid=BJX2_NMID_MOVUL;
			op->fmid=BJX2_FMID_LDREGREG;
			op->Run=BJX2_Op_MOVUL_LdRegReg;
			break;
		
		case 0x1:	/* 81zz */
		case 0x2:	/* 82zz */
		case 0x3:	/* 83zz */
		case 0x4:	/* 84zz */
		case 0x5:	/* 85zz */
		case 0x6:	/* 86zz */
		case 0x7:	/* 87zz */
			op->imm=(opw>>8)&7;
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVL_RegStRegDisp;
			break;

		case 0x8:	/* 88zz */
			if(op->rm==0)
			{
				op->nmid=BJX2_NMID_MOVUL;
				op->fmid=BJX2_FMID_LDDRPCREG;
				op->Run=BJX2_Op_MOVUL_LdDrPcReg;
				break;
			}
			op->nmid=BJX2_NMID_MOVUL;
			op->fmid=BJX2_FMID_LDDRREGREG;
			op->Run=BJX2_Op_MOVUL_LdDrRegReg;
			if(op->rm==1)
			{
				op->rm=BJX2_REG_GBR;
				op->ro=BJX2_REG_DLR;
				op->Run=BJX2_Op_MOVUL_LdReg2RegB;
			}
			break;

		case 0x9:	/* 89zz */
		case 0xA:	/* 8Azz */
		case 0xB:	/* 8Bzz */
		case 0xC:	/* 8Czz */
		case 0xD:	/* 8Dzz */
		case 0xE:	/* 8Ezz */
		case 0xF:	/* 8Fzz */
			op->imm=(opw>>8)&7;
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVL_LdRegDispReg;
			break;
		}
		break;

	case 0x9:	/* 9zzz */
		op->rn=(opw>>4)&15;
		op->rm=opw&15;
		switch((opw>>8)&15)
		{
		case 0x0:	/* 90zz */
//			op->imm=(opw>>8)&7;
			op->nmid=BJX2_NMID_FMOVS;
			op->fmid=BJX2_FMID_REGSTREG;
			op->Run=BJX2_Op_FMOVS_RegStReg;
			break;
		case 0x1:	/* 91zz */
//			op->imm=(opw>>8)&7;
			op->nmid=BJX2_NMID_FMOVD;
			op->fmid=BJX2_FMID_REGSTREG;
			op->Run=BJX2_Op_FMOVD_RegStReg;
			break;
		case 0x2:	/* 92zz */
//			op->imm=(opw>>8)&7;
			if(op->rn==0)
			{
				op->rn=BJX2_REG_PC;
				op->ro=0;
				op->nmid=BJX2_NMID_FMOVS;
				op->fmid=BJX2_FMID_REGSTDRREG;
				op->Run=BJX2_Op_FMOVS_RegStPcReg;
				break;
			}
			if(op->rn==1)
				op->rn=BJX2_REG_GBR;
			op->nmid=BJX2_NMID_FMOVS;
			op->fmid=BJX2_FMID_REGSTDRREG;
			op->Run=BJX2_Op_FMOVS_RegStDrReg;
			break;
		case 0x3:	/* 93zz */
//			op->imm=(opw>>8)&7;
			if(op->rn==0)
			{
				op->rn=BJX2_REG_PC;
				op->ro=0;
				op->nmid=BJX2_NMID_FMOVD;
				op->fmid=BJX2_FMID_REGSTDRREG;
				op->Run=BJX2_Op_FMOVD_RegStPcReg;
				break;
			}
			if(op->rn==1)
				op->rn=BJX2_REG_GBR;
			op->nmid=BJX2_NMID_FMOVD;
			op->fmid=BJX2_FMID_REGSTDRREG;
			op->Run=BJX2_Op_FMOVD_RegStDrReg;
			break;


		case 0x4:	/* 94zz */
//			op->imm=(opw>>8)&7;
			op->nmid=BJX2_NMID_FMOVS;
			op->fmid=BJX2_FMID_LDREGREG;
			op->Run=BJX2_Op_FMOVS_LdRegReg;
			break;
		case 0x5:	/* 95zz */
//			op->imm=(opw>>8)&7;
			op->nmid=BJX2_NMID_FMOVD;
			op->fmid=BJX2_FMID_LDREGREG;
			op->Run=BJX2_Op_FMOVD_LdRegReg;
			break;
		case 0x6:	/* 96zz */
//			op->imm=(opw>>8)&7;
			if(op->rm==0)
			{
				op->rm=BJX2_REG_PC;
				op->ro=0;
				op->nmid=BJX2_NMID_FMOVS;
				op->fmid=BJX2_FMID_LDDRREGREG;
				op->Run=BJX2_Op_FMOVS_LdPcRegReg;
				break;
			}
			if(op->rm==1)
				op->rm=BJX2_REG_GBR;
			op->nmid=BJX2_NMID_FMOVS;
			op->fmid=BJX2_FMID_LDDRREGREG;
			op->Run=BJX2_Op_FMOVS_LdDrRegReg;
			break;
		case 0x7:	/* 97zz */
//			op->imm=(opw>>8)&7;
			if(op->rm==0)
			{
				op->rm=BJX2_REG_PC;
				op->ro=0;
				op->nmid=BJX2_NMID_FMOVD;
				op->fmid=BJX2_FMID_LDDRREGREG;
				op->Run=BJX2_Op_FMOVD_LdPcRegReg;
				break;
			}
			if(op->rm==1)
				op->rm=BJX2_REG_GBR;
			op->nmid=BJX2_NMID_FMOVD;
			op->fmid=BJX2_FMID_LDDRREGREG;
			op->Run=BJX2_Op_FMOVD_LdDrRegReg;
			break;

		case 0x8:	/* 98zz */
			op->nmid=BJX2_NMID_FADD;
			op->fmid=BJX2_FMID_FREGREG;
			op->Run=BJX2_Op_FADD_RegReg;
			break;
		case 0x9:	/* 99zz */
			op->nmid=BJX2_NMID_FSUB;
			op->fmid=BJX2_FMID_FREGREG;
			op->Run=BJX2_Op_FSUB_RegReg;
			break;
		case 0xA:	/* 9Azz */
			op->nmid=BJX2_NMID_FMUL;
			op->fmid=BJX2_FMID_FREGREG;
			op->Run=BJX2_Op_FMUL_RegReg;
			break;
		case 0xB:	/* 9Bzz */
			op->nmid=BJX2_NMID_FDIV;
			op->fmid=BJX2_FMID_FREGREG;
			op->Run=BJX2_Op_FDIV_RegReg;
			break;

		case 0xC:	/* 9Czz */
			op->nmid=BJX2_NMID_FCMPEQ;
			op->fmid=BJX2_FMID_FREGREG;
			op->Run=BJX2_Op_FCMPEQ_RegReg;
			break;
		case 0xD:	/* 9Dzz */
			op->nmid=BJX2_NMID_FCMPGT;
			op->fmid=BJX2_FMID_FREGREG;
			op->Run=BJX2_Op_FCMPGT_RegReg;
			break;
		case 0xE:	/* 9Ezz */
			op->nmid=BJX2_NMID_FMOV;
			op->fmid=BJX2_FMID_FREGREG;
			op->Run=BJX2_Op_FMOV_RegReg;
			break;

		case 0xF:	/* 9Fzz */
			switch(opw&15)
			{
			case 0x0:	/* 9Fz0 */
				op->nmid=BJX2_NMID_FLDCF;
				op->fmid=BJX2_FMID_FREG;
				op->Run=BJX2_Op_FLDCF_Reg;
				break;
			case 0x1:	/* 9Fz1 */
				op->nmid=BJX2_NMID_FLDCD;
				op->fmid=BJX2_FMID_FREG;
				op->Run=BJX2_Op_FLDCD_Reg;
				break;
			case 0x2:	/* 9Fz2 */
				op->nmid=BJX2_NMID_FLDCI;
				op->fmid=BJX2_FMID_FREG;
				op->Run=BJX2_Op_FLDCI_Reg;
				break;
			case 0x3:	/* 9Fz3 */
				op->nmid=BJX2_NMID_FLDCH;
				op->fmid=BJX2_FMID_FREG;
				op->Run=BJX2_Op_FLDCH_Reg;
				break;
			case 0x4:	/* 9Fz4 */
				op->nmid=BJX2_NMID_FSTCF;
				op->fmid=BJX2_FMID_FREG;
				op->Run=BJX2_Op_FSTCF_Reg;
				break;
			case 0x5:	/* 9Fz5 */
				op->nmid=BJX2_NMID_FSTCD;
				op->fmid=BJX2_FMID_FREG;
				op->Run=BJX2_Op_FSTCD_Reg;
				break;
			case 0x6:	/* 9Fz6 */
				op->nmid=BJX2_NMID_FSTCI;
				op->fmid=BJX2_FMID_FREG;
				op->Run=BJX2_Op_FSTCI_Reg;
				break;
			case 0x7:	/* 9Fz7 */
				op->nmid=BJX2_NMID_FSTCH;
				op->fmid=BJX2_FMID_FREG;
				op->Run=BJX2_Op_FSTCH_Reg;
				break;
			case 0x8:	/* 9Fz8 */
				op->nmid=BJX2_NMID_FNEG;
				op->fmid=BJX2_FMID_FREG;
				op->Run=BJX2_Op_FNEG_Reg;
				break;
			case 0x9:	/* 9Fz9 */
				op->nmid=BJX2_NMID_FABS;
				op->fmid=BJX2_FMID_FREG;
				op->Run=BJX2_Op_FABS_Reg;
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
			op->nmid=BJX2_NMID_LDIN;
			op->fmid=BJX2_FMID_IMM;
			op->Run=BJX2_Op_LDIN_Imm12;
		}else
		{
			op->imm=opw&4095;
			op->nmid=BJX2_NMID_LDIZ;
			op->fmid=BJX2_FMID_IMM;
			op->Run=BJX2_Op_LDIZ_Imm12;
		}

		BJX2_DecodeOpcode_CheckTwoWord(ctx, op, addr, opw);
		break;
	case 0xC:	/* Cnii */
		op->imm=(sbyte)opw;
		op->rn=(opw>>8)&15;
		op->nmid=BJX2_NMID_ADD;
		op->fmid=BJX2_FMID_IMMREG;
		op->Run=BJX2_Op_ADD_ImmReg;
		
#ifdef BJX2_ADDR32
		if(op->rn==15)
		{
			op->rm=op->rn;
			op->nmid=BJX2_NMID_ADDUL;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_ADDUL_RegImmReg;
		}
#endif
		
		break;

//	case 0xE:	/* Enii */
	case 0xD:	/* Dnii */
		op->imm=(sbyte)opw;
		op->rn=(opw>>8)&15;
//		op->nmid=BJX2_NMID_MOV;
		op->nmid=BJX2_NMID_LDI;
		op->fmid=BJX2_FMID_IMMREG;
		op->Run=BJX2_Op_MOV_ImmReg;
		break;

//	case 0xD:	/* Dzzz */
	case 0xE:	/* Ezzz */
		switch((opw>>8)&15)
		{
		case 0x0:	/* E0zz */
		case 0x4:	/* E4zz */
			ret=BJX2_DecodeOpcode_DecD4(ctx, op, addr, opw, opw2);
			break;
		case 0x1:	/* E1zz */
		case 0x5:	/* E5zz */
			ret=BJX2_DecodeOpcode_DecD5(ctx, op, addr, opw, opw2);
			break;
		case 0x2:	/* E2zz */
		case 0x6:	/* E6zz */
			ret=BJX2_DecodeOpcode_DecD6(ctx, op, addr, opw, opw2);
			break;

		case 0x8:	/* E8zz */
		case 0x9:	/* E9zz */
//		case 0xA:	/* EAzz */
			ret=BJX2_DecodeOpcode_DecD8(ctx, op, addr, opw, opw2);
			break;

		case 0xA:	/* EAzz */
		case 0xB:	/* EBzz */
			if(opw2&0x8000)
				ret=BJX2_DecodeOpcode_DecD6(ctx, op, addr, opw, opw2);
			else
				ret=BJX2_DecodeOpcode_DecD4(ctx, op, addr, opw, opw2);
			op->fl|=BJX2_OPFL_WEX;
			break;

		case 0xC:	/* ECzz */
		case 0xD:	/* EDzz */
		case 0xE:	/* EEzz */
		case 0xF:	/* EFzz */
			ret=BJX2_DecodeOpcode_DecFC(ctx, op, addr, opw, opw2, opw3);
			break;
		}
		break;

	case 0xF:	/* Fzzz */
		switch((opw>>8)&15)
		{
		case 0x0:	/* F0zz */
			ret=BJX2_DecodeOpcode_DecF0(ctx, op, addr, opw, opw2);
			break;
		case 0x1:	/* F1zz */
			ret=BJX2_DecodeOpcode_DecF1(ctx, op, addr, opw, opw2);
			break;
		case 0x2:	/* F2zz */
			ret=BJX2_DecodeOpcode_DecF2(ctx, op, addr, opw, opw2);
			break;

#if 0
		case 0x4:	/* F4zz */
			ret=BJX2_DecodeOpcode_DecF4(ctx, op, addr, opw, opw2);
			break;
		case 0x5:	/* F5zz */
			ret=BJX2_DecodeOpcode_DecF5(ctx, op, addr, opw, opw2);
			break;
		case 0x6:	/* F6zz */
			ret=BJX2_DecodeOpcode_DecF6(ctx, op, addr, opw, opw2);
			break;
#endif

#if 1
		case 0x4:	/* F4zz */
			ret=BJX2_DecodeOpcode_DecF0(ctx, op, addr, opw, opw2);
			op->fl|=BJX2_OPFL_WEX;
			break;
		case 0x5:	/* F5zz */
			ret=BJX2_DecodeOpcode_DecF1(ctx, op, addr, opw, opw2);
			op->fl|=BJX2_OPFL_WEX;
			break;
		case 0x6:	/* F6zz */
			ret=BJX2_DecodeOpcode_DecF2(ctx, op, addr, opw, opw2);
			op->fl|=BJX2_OPFL_WEX;
			break;
#endif

		case 0x8:	/* F8zz */
			ret=BJX2_DecodeOpcode_DecF8(ctx, op, addr, opw, opw2);
			break;

		case 0x9:	/* F9zz */
			ret=BJX2_DecodeOpcode_DecF8(ctx, op, addr, opw, opw2);
			op->fl|=BJX2_OPFL_WEX;
			break;

		case 0xA:	/* FAzz */
		case 0xB:	/* FBzz */
			op->fl|=BJX2_OPFL_TWOWORD;
			op->opn=opw;
			op->opn2=opw2;
			if(opw&0x0100)
			{
				op->imm=((opw|(~255))<<16)|((u16)opw2);
				op->nmid=BJX2_NMID_LDIN;
				op->fmid=BJX2_FMID_IMM;
				op->Run=BJX2_Op_LDIN_Imm24;
			}else
			{
				op->imm=((opw&255)<<16)|((u16)opw2);
				op->nmid=BJX2_NMID_LDIZ;
				op->fmid=BJX2_FMID_IMM;
				op->Run=BJX2_Op_LDIZ_Imm24;
			}
			break;

		case 0xC:	/* FCzz */
		case 0xD:	/* FDzz */
		case 0xE:	/* FEzz */
		case 0xF:	/* FFzz */
			ret=BJX2_DecodeOpcode_DecFC(ctx, op, addr, opw, opw2, opw3);
			break;
		}
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
		case BJX2_NMID_MOVB:
		case BJX2_NMID_MOVW:
		case BJX2_NMID_MOVL:
		case BJX2_NMID_MOVQ:
		case BJX2_NMID_MOVUB:
		case BJX2_NMID_MOVUW:
		case BJX2_NMID_MOVUL:
		case BJX2_NMID_FMOVS:
		case BJX2_NMID_FMOVD:
		case BJX2_NMID_PUSH:
		case BJX2_NMID_POP:
		case BJX2_NMID_FPUSH:
		case BJX2_NMID_FPOP:
			op->cyc=2;
//			op->cyc=3;
//			op->cyc=4;
//			op->cyc=6;
			break;

		case BJX2_NMID_ADD:
		case BJX2_NMID_SUB:
		case BJX2_NMID_ADC:
		case BJX2_NMID_SBB:
//			op->cyc=2;
			op->cyc=1;
			break;

		case BJX2_NMID_CMPQEQ:
		case BJX2_NMID_CMPQGT:
		case BJX2_NMID_CMPQGE:
		case BJX2_NMID_CMPQHI:
		case BJX2_NMID_CMPQHS:
			op->cyc=2;
			break;

		case BJX2_NMID_BRA:
		case BJX2_NMID_BSR:
			op->cyc=2;
			break;

		case BJX2_NMID_BT:
		case BJX2_NMID_BF:
			op->cyc=3;
//			op->cyc=2;
			break;
		case BJX2_NMID_RTS:
			op->cyc=5;
//			op->cyc=3;
//			op->cyc=2;
			break;
		case BJX2_NMID_RTSU:
//			op->cyc=3;
			op->cyc=2;
			break;

		case BJX2_NMID_FADD:
		case BJX2_NMID_FSUB:
		case BJX2_NMID_FMUL:
			op->cyc=6;
			break;
		case BJX2_NMID_FDIV:
			op->cyc=80;
			break;

		case BJX2_NMID_MULU:
		case BJX2_NMID_MULS:
			op->cyc=4;
			break;
		
		default:
			op->cyc=1;
			break;
		}
	}
	
//	if(!op->Run)
	if(!op->Run || !op->opn)
	{
		if(1)
		{
			op->nmid=BJX2_NMID_INV;
			op->fmid=BJX2_FMID_Z;
			op->Run=BJX2_Op_INVOP_None;
			op->fl|=BJX2_OPFL_CTRLF;
		}
//		__debugbreak();
	}
	
	return(ret);
}
