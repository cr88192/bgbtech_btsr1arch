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
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_MOVB_RegStReg2;
			break;
		case 0x5:	/* F0ez_05zz */
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_MOVW_RegStReg2;
			break;
		case 0x6:	/* F0ez_06zz */
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_MOVL_RegStReg2;
			break;
		case 0x7:	/* F0ez_07zz */
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
			break;
		case 0xC:	/* F0ez_0Czz */
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
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_MOVQ_LdReg2Reg;
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

	case 0x4:	/* F0ez_4zzz */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;
		op->imm=disp5;
		switch((opw2>>8)&15)
		{
		case 0xC:	/* F0eo_4Cnm */
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
