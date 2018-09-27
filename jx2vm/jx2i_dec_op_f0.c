int BJX2_DecodeOpcode_DecF0(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2)
{
	int rn_dfl, rm_dfl, ro_dfl;
	int disp5, eq, eo;
	int imm8u, imm8n;
	int imm10u, imm10n;
	int ret;

	op->fl|=BJX2_OPFL_TWOWORD;
	op->opn=opw1;
	op->opn2=opw2;

	rn_dfl=(opw2>>4)&15;
	rm_dfl=(opw2   )&15;
	ro_dfl=(opw1   )&15;
	if(opw1&0x0040)rn_dfl+=16;
	if(opw1&0x0020)rm_dfl+=16;
	if(opw1&0x0010)ro_dfl+=16;

	disp5=(opw1   )&31;
	eq=(opw1&0x0080)?1:0;
	eo=(opw1&0x0010)?1:0;

	imm8u=opw2&255;
	imm8n=opw2|(~255);
	imm10u=(opw2&255)|((opw1&0x0060)<<3);
	imm10n=(opw2&255)|((opw1&0x0060)<<3)|(~1023);

	ret=0;
	switch((opw2>>12)&15)
	{
	case 0x0:	/* F0ez_0zzz */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;
		op->imm=disp5;
		switch((opw2>>8)&15)
		{
		case 0x0:	/* F0ez_00zz */
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVB_RegStRegDisp;
			break;
		case 0x1:	/* F0ez_01zz */
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVW_RegStRegDisp;
			break;
		case 0x2:	/* F0ez_02zz */
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVL_RegStRegDisp;
			break;
		case 0x3:	/* F0ez_03zz */
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVQ_RegStRegDisp;
			break;

		case 0x4:	/* F0ez_04zz */
			if(op->rn==0)
			{
				op->rn=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVB_RegStPcIdx;
				break;
			}
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_MOVB_RegStReg2;
			break;
		case 0x5:	/* F0ez_05zz */
			if(op->rn==0)
			{
				op->rn=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVW_RegStPcIdx;
				break;
			}
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_MOVW_RegStReg2;
			break;
		case 0x6:	/* F0ez_06zz */
			if(op->rn==0)
			{
				op->rn=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVL_RegStPcIdx;
				break;
			}
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_MOVL_RegStReg2;
			break;
		case 0x7:	/* F0ez_07zz */
			if(op->rn==0)
			{
				op->rn=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVQ_RegStPcIdx;
				break;
			}
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_MOVQ_RegStReg2;
			break;
		case 0x8:	/* F0ez_08zz */
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVB_LdRegDispReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUB;
				op->Run=BJX2_Op_MOVUB_LdRegDispReg;
			}
			break;
		case 0x9:	/* F0ez_09zz */
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVW_LdRegDispReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUW;
				op->Run=BJX2_Op_MOVUW_LdRegDispReg;
			}
			break;
		case 0xA:	/* F0ez_0Azz */
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVL_LdRegDispReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUL;
				op->Run=BJX2_Op_MOVUL_LdRegDispReg;
			}
			break;
		case 0xB:	/* F0ez_0Bzz */
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVQ_LdRegDispReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUL;
				op->Run=BJX2_Op_MOVUL_LdRegDispReg;
			}
			break;
		case 0xC:	/* F0ez_0Czz */
			if(op->rm==0)
			{
				op->rm=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVB_LdPcIdxReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_MOVUB;
					op->Run=BJX2_Op_MOVUB_LdPcIdxReg;
				}
				break;
			}
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_MOVB_LdReg2Reg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUB;
				op->Run=BJX2_Op_MOVUB_LdReg2Reg;
			}
			break;
		case 0xD:	/* F0ez_0Dzz */
			if(op->rm==0)
			{
				op->rm=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVW_LdPcIdxReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_MOVUW;
					op->Run=BJX2_Op_MOVUW_LdPcIdxReg;
				}
				break;
			}
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_MOVW_LdReg2Reg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUW;
				op->Run=BJX2_Op_MOVUW_LdReg2Reg;
			}
			break;
		case 0xE:	/* F0ez_0Ezz */
			if(op->rm==0)
			{
				op->rm=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVL_LdPcIdxReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_MOVUL;
					op->Run=BJX2_Op_MOVUL_LdPcIdxReg;
				}
				break;
			}
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_MOVL_LdReg2Reg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUL;
				op->Run=BJX2_Op_MOVUL_LdReg2Reg;
			}
			break;
		case 0xF:	/* F0ez_0Fzz */
			if(op->rm==0)
			{
				op->rm=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVQ_LdPcIdxReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_MOVUL;
					op->Run=BJX2_Op_MOVUL_LdPcIdxReg;
				}
				break;
			}
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_MOVQ_LdReg2Reg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUL;
				op->Run=BJX2_Op_MOVUL_LdReg2Reg;
			}
			break;
		}
		break;

	case 0x1:	/* F0ez_1zzz */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;
		op->imm=disp5;
		switch((opw2>>8)&15)
		{
		case 0x0:	/* F0ez_10zz */
			op->nmid=BJX2_NMID_ADD;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_ADD_RegRegReg;
			if(eq)
			{
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_ADD_RegImmReg;
			}
			break;
		case 0x1:	/* F0ez_11zz */
			op->nmid=BJX2_NMID_SUB;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SUB_RegRegReg;
			if(eq)
			{
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_SUB_RegImmReg;
			}
			break;
		case 0x2:	/* F0ez_12zz */
			op->nmid=BJX2_NMID_MULS;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_MULS_RegRegReg;
			if(eq)
			{
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_MULS_RegImmReg;
			}
			break;

		case 0x5:	/* F0ez_15zz */
			op->nmid=BJX2_NMID_AND;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_AND_RegRegReg;
			if(eq)
			{
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_AND_RegImmReg;
			}
			break;
		case 0x6:	/* F0ez_16zz */
			op->nmid=BJX2_NMID_OR;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_OR_RegRegReg;
			if(eq)
			{
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_OR_RegImmReg;
			}
			break;
		case 0x7:	/* F0ez_17zz */
			op->nmid=BJX2_NMID_XOR;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_XOR_RegRegReg;
			if(eq)
			{
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_XOR_RegImmReg;
			}
			break;

		case 0x8:	/* F0ez_18zz */
			switch(opw1&15)
			{
#if 0
			case 0x0:	/* F0e0_18zz */
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_REGSTREG;
				op->Run=BJX2_Op_MOVB_RegStReg;
				break;
			case 0x1:	/* F0e1_18zz */
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_REGSTREG;
				op->Run=BJX2_Op_MOVW_RegStReg;
				break;
			case 0x2:	/* F0e2_18zz */
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_REGSTREG;
				op->Run=BJX2_Op_MOVL_RegStReg;
				break;
			case 0x3:	/* F0e3_18zz */
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_REGSTREG;
				op->Run=BJX2_Op_MOVQ_RegStReg;
				break;
			case 0x4:	/* F0e4_18zz */
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_REGSTDRREG;
				op->Run=BJX2_Op_MOVB_RegStDrReg;
				break;
			case 0x5:	/* F0e5_18zz */
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_REGSTDRREG;
				op->Run=BJX2_Op_MOVW_RegStDrReg;
				break;
			case 0x6:	/* F0e6_18zz */
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_REGSTDRREG;
				op->Run=BJX2_Op_MOVL_RegStDrReg;
				break;
			case 0x7:	/* F0e7_18zz */
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_REGSTDRREG;
				op->Run=BJX2_Op_MOVQ_RegStDrReg;
				break;
			case 0x8:	/* F0e8_18zz */
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_LDREGREG;
				op->Run=BJX2_Op_MOVB_LdRegReg;
				if(eq)
				{	op->nmid=BJX2_NMID_MOVUB;
					op->Run=BJX2_Op_MOVUB_LdRegReg;		}
				break;
			case 0x9:	/* F0e9_18zz */
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_LDREGREG;
				op->Run=BJX2_Op_MOVW_LdRegReg;
				if(eq)
				{	op->nmid=BJX2_NMID_MOVUW;
					op->Run=BJX2_Op_MOVUW_LdRegReg;		}
				break;
			case 0xA:	/* F0eA_18zz */
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_LDREGREG;
				op->Run=BJX2_Op_MOVL_LdRegReg;
				if(eq)
				{	op->nmid=BJX2_NMID_MOVUL;
					op->Run=BJX2_Op_MOVUL_LdRegReg;		}
				break;
			case 0xB:	/* F0eB_18zz */
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_LDREGREG;
				op->Run=BJX2_Op_MOVQ_LdRegReg;
				break;
			case 0xC:	/* F0eC_18zz */
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_LDDRREGREG;
				op->Run=BJX2_Op_MOVB_LdDrRegReg;
				if(eq)
				{	op->nmid=BJX2_NMID_MOVUB;
					op->Run=BJX2_Op_MOVUB_LdDrRegReg;		}
				break;
			case 0xD:	/* F0eD_18zz */
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_LDDRREGREG;
				op->Run=BJX2_Op_MOVW_LdDrRegReg;
				if(eq)
				{	op->nmid=BJX2_NMID_MOVUW;
					op->Run=BJX2_Op_MOVUW_LdDrRegReg;		}
				break;
			case 0xE:	/* F0eE_18zz */
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_LDDRREGREG;
				op->Run=BJX2_Op_MOVL_LdDrRegReg;
				if(eq)
				{	op->nmid=BJX2_NMID_MOVUL;
					op->Run=BJX2_Op_MOVUL_LdDrRegReg;		}
				break;
			case 0xF:	/* F0eF_18zz */
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_LDDRREGREG;
				op->Run=BJX2_Op_MOVQ_LdDrRegReg;
				break;
#endif
			}
			break;
		case 0x9:	/* F0ez_19zz */
			switch(opw1&15)
			{
			case 0x0:	/* F0e0_19nm */
				op->nmid=BJX2_NMID_ADD;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_ADD_RegReg;
				break;
			case 0x1:	/* F0e1_19nm */
				op->nmid=BJX2_NMID_SUB;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SUB_RegReg;
				break;
			case 0x2:	/* F0e2_19nm */
				op->nmid=BJX2_NMID_ADC;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_ADC_RegReg;
				break;
			case 0x3:	/* F0e3_19nm */
				op->nmid=BJX2_NMID_SBB;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SBB_RegReg;
				break;
			case 0x4:	/* F0e4_19nm */
				op->nmid=BJX2_NMID_TST;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_TST_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_TSTQ;
					op->Run=BJX2_Op_TSTQ_RegReg;
				}
				break;
			case 0x5:	/* F0e5_19nm */
				op->nmid=BJX2_NMID_AND;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_AND_RegReg;
				break;
			case 0x6:	/* F0e6_19nm */
				op->nmid=BJX2_NMID_OR;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_OR_RegReg;
				break;
			case 0x7:	/* F0e7_19nm */
				op->nmid=BJX2_NMID_XOR;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_XOR_RegReg;
				break;
			case 0x8:	/* F0e8_19nm */
				op->nmid=BJX2_NMID_MOV;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MOV_RegReg;
				break;
			case 0x9:	/* F0e9_19nm */
				op->nmid=BJX2_NMID_MULS;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MULS_RegReg;
				break;
			case 0xA:	/* F0eA_19nm */
				op->rn+=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOV;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MOV_RegReg;
				break;
			case 0xB:	/* F0eB_19nm */
				op->rm+=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOV;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MOV_RegReg;
				break;
			case 0xC:	/* F0eC_19nm */
				op->nmid=BJX2_NMID_CMPEQ;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CMPEQ_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_CMPQEQ;
					op->Run=BJX2_Op_CMPQEQ_RegReg;
				}
				break;
			case 0xD:	/* F0eD_19nm */
				op->nmid=BJX2_NMID_CMPHI;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CMPHI_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_CMPQHI;
					op->Run=BJX2_Op_CMPQHI_RegReg;
				}
				break;
			case 0xE:	/* F0eE_19nm */
				op->nmid=BJX2_NMID_CMPGT;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CMPGT_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_CMPQGT;
					op->Run=BJX2_Op_CMPQGT_RegReg;
				}
				break;
			case 0xF:	/* F0eF_19nm */
				op->nmid=BJX2_NMID_MULU;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MULU_RegReg;
				break;
			}
			break;
		case 0xA:	/* F0ez_1Azz */
			switch(opw1&15)
			{
			case 0x0:	/* F0e0_1Anm */
				break;
			}
			break;
		case 0xB:	/* F0ez_1Bzz */
			switch(opw1&15)
			{
			case 0x0:	/* F0e0_1Bnm */
				break;
			}
			break;
		case 0xC:	/* F0ez_1Czz */
			switch(opw1&15)
			{
			case 0x0:	/* F0e0_1Cnm */
				op->nmid=BJX2_NMID_NOT;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_NOT_RegReg;
				break;
			case 0x1:	/* F0e1_1Cnm */
				op->nmid=BJX2_NMID_NEG;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_NEG_RegReg;
				break;

			case 0x2:	/* F0e2_1Cnm */
				op->nmid=BJX2_NMID_CLZ;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CLZ_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_CLZQ;
					op->Run=BJX2_Op_CLZQ_RegReg;
				}
				break;

			case 0x6:	/* F0e6_1Cnm */
				op->nmid=BJX2_NMID_SHAD;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SHAD_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_SHADQ;
					op->Run=BJX2_Op_SHADQ_RegReg;
				}
				break;
			case 0x7:	/* F0e7_1Cnm */
				op->nmid=BJX2_NMID_SHLD;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SHLD_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_SHLDQ;
					op->Run=BJX2_Op_SHLDQ_RegReg;
				}
				break;

			case 0x8:	/* F0e8_1Cnm */
				op->nmid=BJX2_NMID_EXTSB;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_EXTSB_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_EXTUB;
					op->Run=BJX2_Op_EXTUB_RegReg;
				}
				break;
			case 0x9:	/* F0e9_1Cnm */
				op->nmid=BJX2_NMID_EXTSW;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_EXTSW_RegReg;
				if(eo)
				{
					op->nmid=BJX2_NMID_EXTSL;
					op->Run=BJX2_Op_EXTSL_RegReg;
				}
				if(eq)
				{
					op->nmid=BJX2_NMID_EXTUW;
					op->Run=BJX2_Op_EXTUW_RegReg;

					if(eo)
					{
						op->nmid=BJX2_NMID_EXTUL;
						op->Run=BJX2_Op_EXTUL_RegReg;
					}
				}
				break;

			case 0xA:	/* F0eA_1Cnm */
				op->rn+=BJX2_REG_R0B;
				op->nmid=BJX2_NMID_MOV;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MOV_RegReg;
				break;
			case 0xB:	/* F0eB_1Cnm */
				op->rm+=BJX2_REG_R0B;
				op->nmid=BJX2_NMID_MOV;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MOV_RegReg;
				break;
			}
			break;
		case 0xD:	/* F0ez_1Dzz */
			switch(opw1&15)
			{
			case 0x0:	/* F0e0_1Dnm */
				break;

			case 0x6:	/* F0e6_1Dnm */
				op->nmid=BJX2_NMID_SHAD;
				op->fmid=BJX2_FMID_REGDRREG;
				op->Run=BJX2_Op_SHAD_RegDrReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_SHADQ;
					op->Run=BJX2_Op_SHADQ_RegDrReg;
				}
				break;
			case 0x7:	/* F0e7_1Dnm */
				op->nmid=BJX2_NMID_SHLD;
				op->fmid=BJX2_FMID_REGDRREG;
				op->Run=BJX2_Op_SHLD_RegDrReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_SHLDQ;
					op->Run=BJX2_Op_SHLDQ_RegDrReg;
				}
				break;

#if 1
			case 0x8:	/* F0e8_1Dnm */
				op->nmid=BJX2_NMID_ADD;
				op->fmid=BJX2_FMID_REGDRREG;
				op->Run=BJX2_Op_ADD_RegDrReg;
				break;
			case 0x9:	/* F0e9_1Dnm */
				op->nmid=BJX2_NMID_SUB;
				op->fmid=BJX2_FMID_REGDRREG;
				op->Run=BJX2_Op_SUB_RegDrReg;
				break;
			case 0xA:	/* F0eA_1Dnm */
				op->nmid=BJX2_NMID_AND;
				op->fmid=BJX2_FMID_REGDRREG;
				op->Run=BJX2_Op_AND_RegDrReg;
				break;
			case 0xB:	/* F0eB_1Dnm */
				op->nmid=BJX2_NMID_OR;
				op->fmid=BJX2_FMID_REGDRREG;
				op->Run=BJX2_Op_OR_RegDrReg;
				break;
			case 0xC:	/* F0eC_1Dnm */
				op->nmid=BJX2_NMID_XOR;
				op->fmid=BJX2_FMID_REGDRREG;
				op->Run=BJX2_Op_XOR_RegDrReg;
				break;
#endif

			case 0xD:	/* F0eD_1Dnm */
				op->nmid=BJX2_NMID_CMPHS;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CMPHS_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_CMPQHS;
					op->Run=BJX2_Op_CMPQHS_RegReg;
				}
				break;
			case 0xE:	/* F0eE_1Dnm */
				op->nmid=BJX2_NMID_CMPGE;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CMPGE_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_CMPQGE;
					op->Run=BJX2_Op_CMPQGE_RegReg;
				}
				break;
			}
			break;
		case 0xE:	/* F0ez_1Ezz */
			switch(opw1&15)
			{
			case 0x0:	/* F0e0_1Enm */
				break;
			}
			break;
		case 0xF:	/* F0ez_1Fzz */
			switch(opw1&15)
			{
			case 0x0:	/* F0e0_1Fnm */
				break;
			}
			break;
		}
		break;

	case 0x2:	/* F0zz_2zzz */
		op->rn=ro_dfl;

//		op->rn=rn_dfl;
//		op->rm=rm_dfl;
//		op->ro=ro_dfl;
//		op->imm=disp5;
		switch((opw2>>8)&15)
		{

		case 0x0:	/* 20zz */
			op->imm=(s16)(((opw1&255)<<8)|(opw2&255));
			op->nmid=BJX2_NMID_BRA;
			op->fmid=BJX2_FMID_PCDISP;
			op->Run=BJX2_Op_BRA_PcDisp;
			op->fl|=BJX2_OPFL_CTRLF;
			break;
		case 0x1:	/* 21zz */
			op->imm=(s16)(((opw1&255)<<8)|(opw2&255));
			op->nmid=BJX2_NMID_BSR;
			op->fmid=BJX2_FMID_PCDISP;
			op->Run=BJX2_Op_BSR_PcDisp;
			op->fl|=BJX2_OPFL_CTRLF;
			break;
		case 0x2:	/* 22zz */
			op->imm=(s16)(((opw1&255)<<8)|(opw2&255));
			op->nmid=BJX2_NMID_BT;
			op->fmid=BJX2_FMID_PCDISP;
			op->Run=BJX2_Op_BT_PcDisp;
			op->fl|=BJX2_OPFL_CTRLF;
			break;
		case 0x3:	/* 23zz */
			op->imm=(s16)(((opw1&255)<<8)|(opw2&255));
			op->nmid=BJX2_NMID_BF;
			op->fmid=BJX2_FMID_PCDISP;
			op->Run=BJX2_Op_BF_PcDisp;
			op->fl|=BJX2_OPFL_CTRLF;
			break;

		case 0x4:
		case 0x5:
			if(opw2&0x0100)
			{
				op->imm=((opw1&255)<<8)|(opw2&255)|(~65535);
				op->nmid=BJX2_NMID_LDIN;
				op->fmid=BJX2_FMID_IMM;
				op->Run=BJX2_Op_LDIN_Imm12;
			}else
			{
				op->imm=((opw1&255)<<8)|(opw2&255);
				op->nmid=BJX2_NMID_LDIZ;
				op->fmid=BJX2_FMID_IMM;
				op->Run=BJX2_Op_LDIZ_Imm12;
			}
			break;

		case 0x6:	/* F0zz_26zz */
			op->imm=((opw1&255)<<8)|(opw2&255);
			op->nmid=BJX2_NMID_LDISH16;
			op->fmid=BJX2_FMID_IMM;
			op->Run=BJX2_Op_LDISH16_Imm;
			break;

		case 0x8:	/* 28zz */
			op->imm=imm10u;
			op->rm=op->rn;
			op->rn=BJX2_REG_SP;
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVL_RegStRegDisp;
			break;
		case 0x9:	/* 29zz */
			op->imm=imm10u;
			op->rm=op->rn;
			op->rn=BJX2_REG_SP;
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVQ_RegStRegDisp;
			break;
		case 0xA:	/* 2Azz */
			op->imm=imm10u;
			op->rm=BJX2_REG_SP;
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVL_LdRegDispReg;
			break;
		case 0xB:	/* 2Bzz */
			op->imm=imm10u;
			op->rm=BJX2_REG_SP;
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVQ_LdRegDispReg;
			break;

		case 0xC:
//			op->imm=imm8u;
			op->imm=imm10u;
			op->nmid=BJX2_NMID_CMPEQ;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_CMPEQ_ImmReg;
			break;
		case 0xD:
//			op->imm=imm8n;
			op->imm=imm10n;
			op->nmid=BJX2_NMID_CMPEQ;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_CMPEQ_ImmReg;
			break;
		case 0xE:
//			op->imm=imm8u;
			op->imm=imm10u;
			op->nmid=BJX2_NMID_CMPGT;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_CMPGT_ImmReg;
			break;
		case 0xF:
//			op->imm=imm8u;
			op->imm=imm10u;
			op->nmid=BJX2_NMID_CMPGE;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_CMPGE_ImmReg;
			break;
		}
		break;

	case 0x3:	/* F0ez_3zzz */
		op->rn=ro_dfl;
		op->imm=ro_dfl;
		switch((opw2>>8)&15)
		{
		case 0x0:	/* F0ez_30zz */
			switch(opw2&255)
			{
			case 0x00:	/* F0ez_3000 */
				op->fmid=BJX2_FMID_Z;
				switch(opw1&15)
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

/*
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
*/

				case 0xC:
					op->nmid=BJX2_NMID_RTE;
					op->Run=BJX2_Op_RTE_None;
					op->fl|=BJX2_OPFL_CTRLF;
					break;
				}
				break;

			case 0x02:	/* F0ez_3002 */
				op->fmid=BJX2_FMID_Z;
				switch(opw1&15)
				{
				case 0x0:	/* F0e0_3002 */
					op->nmid=BJX2_NMID_BRA;
					op->fmid=BJX2_FMID_PCDR;
					op->Run=BJX2_Op_BRA_PcDr;
					op->fl|=BJX2_OPFL_CTRLF;
					break;
				case 0x1:	/* F0e1_3002 */
					op->nmid=BJX2_NMID_BSR;
					op->fmid=BJX2_FMID_PCDR;
					op->Run=BJX2_Op_BSR_PcDr;
					op->fl|=BJX2_OPFL_CTRLF;
					break;
				case 0x2:	/* F0e2_3002 */
					op->nmid=BJX2_NMID_BT;
					op->fmid=BJX2_FMID_PCDR;
					op->Run=BJX2_Op_BT_PcDr;
					op->fl|=BJX2_OPFL_CTRLF;
					break;
				case 0x3:	/* F0e3_3002 */
					op->nmid=BJX2_NMID_BF;
					op->fmid=BJX2_FMID_PCDR;
					op->Run=BJX2_Op_BF_PcDr;
					op->fl|=BJX2_OPFL_CTRLF;
					break;
				}
				break;

			case 0x18:	/* F0ez_3018 */
				op->nmid=BJX2_NMID_PUSH;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_PUSH_Reg;
				break;
			case 0x19:	/* F0ez_3019 */
				op->rn+=BJX2_REG_PC;
				op->nmid=BJX2_NMID_PUSH;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_PUSH_Reg;
				break;
			case 0x1A:	/* F0ez_301A */
				op->nmid=BJX2_NMID_POP;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_POP_Reg;
				break;
			case 0x1B:	/* F0ez_301B */
				if((opw1&0x00F0)==0x0000)
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
			case 0x1C:	/* F0ez_301C */
				op->nmid=BJX2_NMID_CMPPL;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_CMPPL_Reg;
				break;
			case 0x1D:	/* F0ez_301D */
				op->nmid=BJX2_NMID_CMPPZ;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_CMPPZ_Reg;
				break;

			case 0x20:	/* F0ez_3020 */
				op->nmid=BJX2_NMID_BRA;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_BRA_Reg;
				op->fl|=BJX2_OPFL_CTRLF;
				break;
			case 0x21:	/* F0ez_3021 */
				op->nmid=BJX2_NMID_BSR;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_BSR_Reg;
				op->fl|=BJX2_OPFL_CTRLF;
				break;
			case 0x22:	/* F0ez_3022 */
				op->nmid=BJX2_NMID_BT;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_BT_Reg;
				op->fl|=BJX2_OPFL_CTRLF;
				break;
			case 0x23:	/* F0ez_3023 */
				op->nmid=BJX2_NMID_BF;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_BF_Reg;
				op->fl|=BJX2_OPFL_CTRLF;
				break;


			case 0x33:	/* 33z3 */
				op->nmid=BJX2_NMID_MOVNT;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_MOVNT_Reg;
				break;
			case 0x34:	/* 33z4 */
				op->nmid=BJX2_NMID_ROTL;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_ROTL_Reg;
				break;
			case 0x35:	/* 33z5 */
				op->nmid=BJX2_NMID_ROTR;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_ROTR_Reg;
				break;
			case 0x36:	/* 33z6 */
				op->nmid=BJX2_NMID_ROTCL;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_ROTCL_Reg;
				break;
			case 0x37:	/* 33z7 */
				op->nmid=BJX2_NMID_ROTCR;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_ROTCR_Reg;
				break;

			case 0x38:	/* 33z8 */
				op->nmid=BJX2_NMID_SHLL;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SHLL_Reg;
				break;
			case 0x39:	/* 33z9 */
				op->nmid=BJX2_NMID_SHLR;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SHLR_Reg;
				break;
			case 0x3A:	/* 33zA */
				op->nmid=BJX2_NMID_SHAR;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SHAR_Reg;
				break;


			case 0x68:	/* 36z8 */
				op->rm=BJX2_REG_DLR;
				op->nmid=BJX2_NMID_BRA;
				op->fmid=BJX2_FMID_PCREG;
				op->Run=BJX2_Op_BRA_PcReg;
				break;
			case 0x69:	/* 36z9 */
				op->rm=BJX2_REG_DLR;
				op->nmid=BJX2_NMID_BSR;
				op->fmid=BJX2_FMID_PCREG;
				op->Run=BJX2_Op_BSR_PcReg;
				break;
			case 0x6A:	/* 36zA */
				op->rm=BJX2_REG_DLR;
				op->nmid=BJX2_NMID_SWAPB;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SWAPB_Reg;
				break;
			case 0x6B:	/* 36zB */
				op->rm=BJX2_REG_DLR;
				op->nmid=BJX2_NMID_SWAPW;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SWAPW_Reg;
				break;

			case 0x6F:	/* 36zF */
				op->nmid=BJX2_NMID_MOVT;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_MOVT_Reg;
				break;

			case 0x82:	/* 38z2 */
				op->nmid=BJX2_NMID_FPUSH;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_PUSH_FpReg;
				break;
			case 0x83:	/* 38z3 */
				op->nmid=BJX2_NMID_FPOP;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_POP_FpReg;
				break;
			}
			break;
		default:
			break;
		}
		break;

	case 0x4:	/* F0ez_4zzz */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;
		op->imm=disp5;
		switch((opw2>>8)&15)
		{
		case 0xC:	/* F0eo_4Cnm */
			if(op->rm==0)
			{
				op->rm=BJX2_REG_PC;
				op->nmid=BJX2_NMID_LEAB;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_LEAB_LdPcIdxReg;
				if(eq)
				{
					op->fmid=BJX2_FMID_LDPCDISPREG;
					op->Run=BJX2_Op_LEAB_LdPcDispReg;
				}
				break;
			}

			op->nmid=BJX2_NMID_LEAB;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_LEAB_LdReg2Reg;
			if(eq)
			{
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_LEAB_LdRegDispReg;
			}
			break;
		case 0xD:	/* F0eo_4Dnm */
			if(op->rm==0)
			{
				break;
			}

			op->nmid=BJX2_NMID_LEAW;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_LEAW_LdReg2Reg;
			if(eq)
			{
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_LEAW_LdRegDispReg;
			}
			break;
		case 0xE:	/* F0eo_4Enm */
			if(op->rm==0)
			{
				break;
			}

			op->nmid=BJX2_NMID_LEAD;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_LEAD_LdReg2Reg;
			if(eq)
			{
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_LEAD_LdRegDispReg;
			}
			break;
		case 0xF:	/* F0eo_4Fnm */
			if(op->rm==0)
			{
				break;
			}

			op->nmid=BJX2_NMID_LEAQ;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_LEAQ_LdReg2Reg;
			if(eq)
			{
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_LEAQ_LdRegDispReg;
			}
			break;
		}
		break;

	case 0x5:	/* F0ez_5zzz */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;
		op->imm=disp5;
		switch((opw2>>8)&15)
		{
		case 0x0:	/* F0eo_50nm */
			op->nmid=BJX2_NMID_CSELT;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_CSELT_RegRegReg;
			break;


		case 0x4:	/* F0eo_54nm */
			op->nmid=BJX2_NMID_SHAD;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SHAD_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_SHADQ;
				op->Run=BJX2_Op_SHADQ_RegRegReg;
			}
			break;
		case 0x5:	/* F0eo_55nm */
			op->nmid=BJX2_NMID_SHLD;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SHLD_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_SHLDQ;
				op->Run=BJX2_Op_SHLDQ_RegRegReg;
			}
			break;

		case 0x6:	/* F0eo_56nm */
			op->nmid=BJX2_NMID_SHAD;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHAD_RegImmReg;
			if(eq)op->imm|=~31;
			break;
		case 0x7:	/* F0eo_57nm */
			op->nmid=BJX2_NMID_SHLD;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHLD_RegImmReg;
			if(eq)op->imm|=~31;
			break;

		case 0x8:	/* F0eo_58nm */
			op->nmid=BJX2_NMID_SHADQ;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHADQ_RegImmReg;
			if(eq)op->imm|=~31;
			break;
		case 0x9:	/* F0eo_59nm */
			op->nmid=BJX2_NMID_SHLDQ;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHLDQ_RegImmReg;
			if(eq)op->imm|=~31;
			break;
		case 0xA:	/* F0eo_5Anm */
			op->nmid=BJX2_NMID_SHADQ;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHADQ_RegImmReg;
			if(eq)
				op->imm|=~63;
			else
				op->imm|= 32;
			break;
		case 0xB:	/* F0eo_5Bnm */
			op->nmid=BJX2_NMID_SHLDQ;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHLDQ_RegImmReg;
			if(eq)
				op->imm|=~63;
			else
				op->imm|= 32;
			break;
		}
		break;

	case 0x9:	/* F0ez_9zzz */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;
		op->imm=disp5;
		switch((opw2>>8)&15)
		{
		case 0x0:	/* F0eo_90nm */
			op->nmid=BJX2_NMID_FMOVS;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_FMOVS_RegStRegDisp;
			break;
		case 0x1:	/* F0eo_91nm */
			op->nmid=BJX2_NMID_FMOVD;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_FMOVD_RegStRegDisp;
			break;
		case 0x2:	/* F0eo_92nm */
			op->nmid=BJX2_NMID_FMOVS;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_FMOVS_RegStReg2;
			break;
		case 0x3:	/* F0eo_93nm */
			op->nmid=BJX2_NMID_FMOVD;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_FMOVD_RegStReg2;
			break;
		case 0x4:	/* F0eo_94nm */
			op->nmid=BJX2_NMID_FMOVS;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_FMOVS_LdRegDispReg;
			break;
		case 0x5:	/* F0eo_95nm */
			op->nmid=BJX2_NMID_FMOVD;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_FMOVD_LdRegDispReg;
			break;
		case 0x6:	/* F0eo_96nm */
			op->nmid=BJX2_NMID_FMOVS;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_FMOVS_LdReg2Reg;
			break;
		case 0x7:	/* F0eo_97nm */
			op->nmid=BJX2_NMID_FMOVD;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_FMOVD_LdReg2Reg;
			break;

		case 0x8:	/* 98zz */
			op->nmid=BJX2_NMID_FADD;
			op->fmid=BJX2_FMID_FREGREGREG;
			op->Run=BJX2_Op_FADD_RegRegReg;
			break;
		case 0x9:	/* 99zz */
			op->nmid=BJX2_NMID_FSUB;
			op->fmid=BJX2_FMID_FREGREGREG;
			op->Run=BJX2_Op_FSUB_RegRegReg;
			break;
		case 0xA:	/* 9Azz */
			op->nmid=BJX2_NMID_FMUL;
			op->fmid=BJX2_FMID_FREGREGREG;
			op->Run=BJX2_Op_FMUL_RegRegReg;
			break;
		case 0xB:	/* 9Bzz */
			op->nmid=BJX2_NMID_FDIV;
			op->fmid=BJX2_FMID_FREGREGREG;
			op->Run=BJX2_Op_FDIV_RegRegReg;
			break;

		case 0xC:	/* F0ez_9Cnm */
			switch(opw1&15)
			{
#if 0
			case 0x0:	/* F0e0_9Cnm */
				op->nmid=BJX2_NMID_FMOVS;
				op->fmid=BJX2_FMID_REGSTREG;
				op->Run=BJX2_Op_FMOVS_RegStReg;
				break;
			case 0x1:	/* F0e1_9Cnm */
				op->nmid=BJX2_NMID_FMOVD;
				op->fmid=BJX2_FMID_REGSTREG;
				op->Run=BJX2_Op_FMOVD_RegStReg;
				break;
			case 0x2:	/* F0e2_9Cnm */
				op->nmid=BJX2_NMID_FMOVS;
				op->fmid=BJX2_FMID_REGSTDRREG;
				op->Run=BJX2_Op_FMOVS_RegStDrReg;
				break;
			case 0x3:	/* F0e3_9Cnm */
				op->nmid=BJX2_NMID_FMOVD;
				op->fmid=BJX2_FMID_REGSTDRREG;
				op->Run=BJX2_Op_FMOVD_RegStDrReg;
				break;
			case 0x4:	/* F0e4_9Cnm */
				op->nmid=BJX2_NMID_FMOVS;
				op->fmid=BJX2_FMID_LDREGREG;
				op->Run=BJX2_Op_FMOVS_LdRegReg;
				break;
			case 0x5:	/* F0e5_9Cnm */
				op->nmid=BJX2_NMID_FMOVD;
				op->fmid=BJX2_FMID_LDREGREG;
				op->Run=BJX2_Op_FMOVD_LdRegReg;
				break;
			case 0x6:	/* F0e6_9Cnm */
				op->nmid=BJX2_NMID_FMOVS;
				op->fmid=BJX2_FMID_LDDRREGREG;
				op->Run=BJX2_Op_FMOVS_LdDrRegReg;
				break;
			case 0x7:	/* F0e7_9Cnm */
				op->nmid=BJX2_NMID_FMOVD;
				op->fmid=BJX2_FMID_LDDRREGREG;
				op->Run=BJX2_Op_FMOVD_LdDrRegReg;
				break;
#endif

			case 0xC:	/* F0eC_9Czz */
				op->nmid=BJX2_NMID_FCMPEQ;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FCMPEQ_RegReg;
				break;
			case 0xD:	/* F0eD_9Czz */
				op->nmid=BJX2_NMID_FCMPGT;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FCMPGT_RegReg;
				break;
			case 0xE:	/* F0eE_9Czz */
				op->nmid=BJX2_NMID_FMOV;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FMOV_RegReg;
				break;
			}
			break;

		case 0xD:	/* F0ez_9Dnm */
			switch(opw1&15)
			{
			default:
				break;
			}
			break;
		case 0xE:	/* F0ez_9Enm */
			switch(opw1&15)
			{
			default:
				break;
			}
			break;
		case 0xF:	/* F0ez_9Fnm */
			switch(opw1&15)
			{
			case 0x0:	/* 9Fz0 */
				op->nmid=BJX2_NMID_FLDCF;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FLDCF_RegReg;
				break;
			case 0x1:	/* 9Fz1 */
				op->nmid=BJX2_NMID_FLDCD;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FLDCD_RegReg;
				break;
			case 0x2:	/* 9Fz2 */
				op->nmid=BJX2_NMID_FLDCI;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FLDCI_RegReg;
				break;
			case 0x3:	/* 9Fz3 */
				op->nmid=BJX2_NMID_FLDCH;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FLDCH_RegReg;
				break;
			case 0x4:	/* 9Fz4 */
				op->nmid=BJX2_NMID_FSTCF;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FSTCF_RegReg;
				break;
			case 0x5:	/* 9Fz5 */
				op->nmid=BJX2_NMID_FSTCD;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FSTCD_RegReg;
				break;
			case 0x6:	/* 9Fz6 */
				op->nmid=BJX2_NMID_FSTCI;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FSTCI_RegReg;
				break;
			case 0x7:	/* 9Fz7 */
				op->nmid=BJX2_NMID_FSTCH;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FSTCH_RegReg;
				break;
			case 0x8:	/* 9Fz8 */
				op->nmid=BJX2_NMID_FNEG;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FNEG_RegReg;
				break;
			case 0x9:	/* 9Fz9 */
				op->nmid=BJX2_NMID_FABS;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FABS_RegReg;
				break;

			default:
				break;
			}
			break;
			
		}
		break;

	default:
		ret=-1;
		break;
	}
	
	return(ret);
}
