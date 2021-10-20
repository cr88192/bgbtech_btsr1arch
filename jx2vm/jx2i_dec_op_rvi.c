
int BJX2_DecodeOpcode_DecRVI(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2, u32 jbits)
{
	int rn_dfl, rm_dfl, ro_dfl;
	s32 opw;
	int opw3, opw4;
	int disp5, eq, eo;
	int imm8u, imm8n;
	int imm12l, imm12s, imm12b;
	int imm20j, imm20u;
	int ret, fnm;

	op->fl|=BJX2_OPFL_TWOWORD;
	op->opn=opw1;
	op->opn2=opw2;
	op->pc=addr;
	ret=0;

	opw=(opw2<<16)|((u16)opw1);

	rn_dfl=(opw>> 7)&31;
	rm_dfl=(opw>>15)&31;
	ro_dfl=(opw>>20)&31;
	imm20u=(opw)>>12;
	imm12l=(opw)>>20;
	imm12s=(imm12l&(~31))|((opw>>7)&31);
	
	if(rn_dfl<6)
	{
		switch(rn_dfl)
		{
			case 0: rn_dfl=BJX2_REG_ZZR;	break;
			case 1: rn_dfl=BJX2_REG_LR;		break;
			case 2: rn_dfl=BJX2_REG_SP;		break;
			case 3: rn_dfl=BJX2_REG_GBR;	break;
			case 4: rn_dfl=BJX2_REG_TBR;	break;
			case 5: rn_dfl=BJX2_REG_DHR;	break;
		}
	}

	if(rm_dfl<6)
	{
		switch(rm_dfl)
		{
			case 0: rm_dfl=BJX2_REG_ZZR;	break;
			case 1: rm_dfl=BJX2_REG_LR;		break;
			case 2: rm_dfl=BJX2_REG_SP;		break;
			case 3: rm_dfl=BJX2_REG_GBR;	break;
			case 4: rm_dfl=BJX2_REG_TBR;	break;
			case 5: rm_dfl=BJX2_REG_DHR;	break;
		}
	}

	if(ro_dfl<6)
	{
		switch(ro_dfl)
		{
			case 0: ro_dfl=BJX2_REG_ZZR;	break;
			case 1: ro_dfl=BJX2_REG_LR;		break;
			case 2: ro_dfl=BJX2_REG_SP;		break;
			case 3: ro_dfl=BJX2_REG_GBR;	break;
			case 4: ro_dfl=BJX2_REG_TBR;	break;
			case 5: ro_dfl=BJX2_REG_DHR;	break;
		}
	}

	imm12b =
		((opw>> 8)&0x0000000F) |
		((opw>>21)&0x000003F0) |
		((opw<< 3)&0x00000400) |
		((opw>>20)&0xFFFFF800) ;

	imm20j=
		((opw>>20)&0x000007FE) |
		((opw>> 9)&0x00000800) |
		((opw>> 0)&0x000FF000) |
		((opw>>12)&0xFFF00000) ;

	switch((opw>>2)&0x1F)
	{
	case 0x00: /* 00-000 */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->imm=imm12l;

		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVB_LdRegDisp1Reg;
			break;
		case 1:
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVW_LdRegDisp1Reg;
			break;
		case 2:
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVL_LdRegDisp1Reg;
			break;
		case 3:
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVQ_LdRegDisp1Reg;
			break;
		case 4:
			op->nmid=BJX2_NMID_MOVUB;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVUB_LdRegDisp1Reg;
			break;
		case 5:
			op->nmid=BJX2_NMID_MOVUW;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVUW_LdRegDisp1Reg;
			break;
		case 6:
			op->nmid=BJX2_NMID_MOVUL;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVUL_LdRegDisp1Reg;
			break;
		case 7:
			op->nmid=BJX2_NMID_MOVX2;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVX2_LdRegDisp1Reg;
			break;
		}
		break;

	case 0x08: /* 01-000 */
		op->rn=rm_dfl;
		op->rm=ro_dfl;
		op->imm=imm12s;

		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_REGSTREGDISP1;
			op->Run=BJX2_Op_MOVB_RegStRegDisp1;
			break;
		case 1:
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_REGSTREGDISP1;
			op->Run=BJX2_Op_MOVW_RegStRegDisp1;
			break;
		case 2:
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREGDISP1;
			op->Run=BJX2_Op_MOVL_RegStRegDisp1;
			break;
		case 3:
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREGDISP1;
			op->Run=BJX2_Op_MOVQ_RegStRegDisp1;
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			op->nmid=BJX2_NMID_MOVX2;
			op->fmid=BJX2_FMID_REGSTREGDISP1;
			op->Run=BJX2_Op_MOVX2_RegStRegDisp1;
			break;
		}
		break;

	case 0x18: /* 11-000 */
		op->fl|=BJX2_OPFL_CTRLF;
		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEXSFX;

		op->rn=rm_dfl;
		op->rm=ro_dfl;
		op->imm=imm12b;
		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=BJX2_NMID_BREQ;
			op->fmid=BJX2_FMID_REGREGPCDISP;
			op->Run=BJX2_Op_BREQ_RegRegPcDisp;
			break;
		case 1:
			op->nmid=BJX2_NMID_BRNE;
			op->fmid=BJX2_FMID_REGREGPCDISP;
			op->Run=BJX2_Op_BRNE_RegRegPcDisp;
			break;

		case 4:
			op->nmid=BJX2_NMID_BRLT;
			op->fmid=BJX2_FMID_REGREGPCDISP;
			op->Run=BJX2_Op_BRLT_RegRegPcDisp;
			break;
		case 5:
			op->nmid=BJX2_NMID_BRGE;
			op->fmid=BJX2_FMID_REGREGPCDISP;
			op->Run=BJX2_Op_BRGE_RegRegPcDisp;
			break;
		case 6:
			op->nmid=BJX2_NMID_BRBI;
			op->fmid=BJX2_FMID_REGREGPCDISP;
			op->Run=BJX2_Op_BRBI_RegRegPcDisp;
			break;
		case 7:
			op->nmid=BJX2_NMID_BRHE;
			op->fmid=BJX2_FMID_REGREGPCDISP;
			op->Run=BJX2_Op_BRHE_RegRegPcDisp;
			break;
		}
		break;

	case 0x19: /* 11-001 */
		op->fl|=BJX2_OPFL_CTRLF;
		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEXSFX;

		if(rn_dfl==BJX2_REG_ZZR)
		{
			op->rn=rm_dfl;
			op->imm=imm12l;
			op->nmid=BJX2_NMID_BRA;
			op->fmid=BJX2_FMID_LDREGDISP1;
			op->Run=BJX2_Op_BRA_RegDisp1;
			break;
		}

		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->imm=imm12l;
		op->nmid=BJX2_NMID_BSR;
		op->fmid=BJX2_FMID_LDREGDISP1REG;
		op->Run=BJX2_Op_BSR_RegRegDisp1;
		break;

	case 0x1B: /* 11-011 */
		op->fl|=BJX2_OPFL_CTRLF;
		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEXSFX;

		if(rn_dfl==BJX2_REG_ZZR)
		{
			op->imm=imm20j>>1;
			op->nmid=BJX2_NMID_BRA;
			op->fmid=BJX2_FMID_PCDISP;
			op->Run=BJX2_Op_BRA_PcDisp;
			break;
		}

		op->rn=rn_dfl;
		op->imm=imm20j>>1;
		op->nmid=BJX2_NMID_BSR;
		op->fmid=BJX2_FMID_REGPCDISP;
		op->Run=BJX2_Op_BSR_RegPcDisp;
		break;

	case 0x04: /* 00-100 */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->imm=imm12l;

		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=BJX2_NMID_ADD;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_ADD_RegImmReg;
			break;

		case 1:
			op->nmid=BJX2_NMID_SHLDQ;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHLDQ_RegImmReg;
			break;

		case 4:
			op->nmid=BJX2_NMID_XOR;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_XOR_RegImmReg;
			break;

		case 5:
			op->imm=-(imm12l&63);
			if((opw>>30)&1)
			{
				op->nmid=BJX2_NMID_SHADQ;
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_SHADQ_RegImmReg;
			}else
			{
				op->nmid=BJX2_NMID_SHLDQ;
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_SHLDQ_RegImmReg;
			}
			break;

		case 6:
			op->nmid=BJX2_NMID_OR;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_OR_RegImmReg;
			break;
		case 7:
			op->nmid=BJX2_NMID_AND;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_AND_RegImmReg;
			break;
		}
		break;

	case 0x0C: /* 01-100 */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;

		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=BJX2_NMID_ADD;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_ADD_RegRegReg;
			if((opw>>30)&1)
			{
				op->nmid=BJX2_NMID_SUB;
				op->Run=BJX2_Op_SUB_RegRegReg;
			}
			break;

		case 1:
			op->nmid=BJX2_NMID_SHLDQ;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SHLDQ_RegRegReg;
			break;

		case 4:
			op->nmid=BJX2_NMID_XOR;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_XOR_RegRegReg;
			break;

		case 5:
			op->imm=-(imm12l&63);
			if((opw>>30)&1)
			{
				op->nmid=BJX2_NMID_SHARQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_SHARQ_RegRegReg;
			}else
			{
				op->nmid=BJX2_NMID_SHLRQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_SHLRQ_RegRegReg;
			}
			break;

		case 6:
			op->nmid=BJX2_NMID_OR;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_OR_RegRegReg;
			break;
		case 7:
			op->nmid=BJX2_NMID_AND;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_AND_RegRegReg;
			break;
		}
		break;

	case 0x05: /* 00-101 */
		op->rn=rn_dfl;
		op->imm=addr+(imm20u<<12)+4;
		op->nmid=BJX2_NMID_MOV;
		op->fmid=BJX2_FMID_IMMREG;
		op->Run=BJX2_Op_MOV_ImmReg;
		break;

	case 0x0D: /* 01-101 */
		op->rn=rn_dfl;
		op->imm=imm20u<<12;
		op->nmid=BJX2_NMID_MOV;
		op->fmid=BJX2_FMID_IMMREG;
		op->Run=BJX2_Op_MOV_ImmReg;
		break;


	case 0x06: /* 00-110 */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->imm=imm12l;

		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=BJX2_NMID_ADDSL;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_ADDSL_RegImmReg;
			break;

		case 1:
			op->nmid=BJX2_NMID_SHLD;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHLD_RegImmReg;
			break;

		case 5:
			op->imm=-(imm12l&63);
			if((opw>>30)&1)
			{
				op->nmid=BJX2_NMID_SHAD;
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_SHAD_RegImmReg;
			}else
			{
				op->nmid=BJX2_NMID_SHLD;
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_SHLD_RegImmReg;
			}
			break;
		}
		break;

	case 0x0E: /* 01-110 */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;

		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=BJX2_NMID_ADDSL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_ADDSL_RegRegReg;
			if((opw>>30)&1)
			{
				op->nmid=BJX2_NMID_SUBSL;
				op->Run=BJX2_Op_SUBSL_RegRegReg;
			}
			break;

		case 1:
			op->nmid=BJX2_NMID_SHLDQ;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SHLDQ_RegRegReg;
			break;

		case 4:
			op->nmid=BJX2_NMID_XOR;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_XOR_RegRegReg;
			break;

		case 5:
			op->imm=-(imm12l&63);
			if((opw>>30)&1)
			{
				op->nmid=BJX2_NMID_SHARQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_SHARQ_RegRegReg;
			}else
			{
				op->nmid=BJX2_NMID_SHLRQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_SHLRQ_RegRegReg;
			}
			break;

		case 6:
			op->nmid=BJX2_NMID_OR;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_OR_RegRegReg;
			break;
		case 7:
			op->nmid=BJX2_NMID_AND;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_AND_RegRegReg;
			break;
		}
		break;

	default:
		ret=-1;
		break;
	}

	return(ret);
}
