int BJX2_DecodeOpcode_DecF1(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2)
{
	int rn_dfl, rm_dfl, ro_dfl;
	int disp5, eq, eo;
//	int imm8u, imm8n;
//	int imm10u, imm10n;
	int imm9u;
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

//	imm8u=opw2&255;
//	imm8n=opw2|(~255);
//	imm10u=(opw2&255)|((opw1&0x0060)<<3);
//	imm10n=(opw2&255)|((opw1&0x0060)<<3)|(~1023);

	imm9u=((opw1&31)<<4)|((opw2>>8)&15);

	op->rn=rn_dfl;
	op->rm=rm_dfl;
	op->imm=imm9u;

	ret=0;
	switch((opw2>>12)&15)
	{
	case 0x0:	/* F1ed_0dzz */
		op->nmid=BJX2_NMID_MOVB;
		op->fmid=BJX2_FMID_REGSTREGDISP;
		op->Run=BJX2_Op_MOVB_RegStRegDisp;
		break;
	case 0x1:	/* F1ed_1dzz */
		op->nmid=BJX2_NMID_MOVW;
		op->fmid=BJX2_FMID_REGSTREGDISP;
		op->Run=BJX2_Op_MOVW_RegStRegDisp;
		break;
	case 0x2:	/* F1ed_2dzz */
		op->nmid=BJX2_NMID_MOVL;
		op->fmid=BJX2_FMID_REGSTREGDISP;
		op->Run=BJX2_Op_MOVL_RegStRegDisp;
		break;
	case 0x3:	/* F1ed_3dzz */
		op->nmid=BJX2_NMID_MOVQ;
		op->fmid=BJX2_FMID_REGSTREGDISP;
		op->Run=BJX2_Op_MOVQ_RegStRegDisp;
		break;

	case 0x8:	/* F1ed_8dzz */
		op->nmid=BJX2_NMID_MOVB;
		op->fmid=BJX2_FMID_LDREGDISPREG;
		op->Run=BJX2_Op_MOVB_LdRegDispReg;
		if(eq)
		{
			op->nmid=BJX2_NMID_MOVUB;
			op->Run=BJX2_Op_MOVUB_LdRegDispReg;
		}
		break;
	case 0x9:	/* F1ed_9dzz */
		op->nmid=BJX2_NMID_MOVW;
		op->fmid=BJX2_FMID_LDREGDISPREG;
		op->Run=BJX2_Op_MOVW_LdRegDispReg;
		if(eq)
		{
			op->nmid=BJX2_NMID_MOVUW;
			op->Run=BJX2_Op_MOVUW_LdRegDispReg;
		}
		break;
	case 0xA:	/* F1ed_Adzz */
		op->nmid=BJX2_NMID_MOVL;
		op->fmid=BJX2_FMID_LDREGDISPREG;
		op->Run=BJX2_Op_MOVL_LdRegDispReg;
		if(eq)
		{
			op->nmid=BJX2_NMID_MOVUL;
			op->Run=BJX2_Op_MOVUL_LdRegDispReg;
		}
		break;
	case 0xB:	/* F1ed_Bdzz */
		op->nmid=BJX2_NMID_MOVQ;
		op->fmid=BJX2_FMID_LDREGDISPREG;
		op->Run=BJX2_Op_MOVQ_LdRegDispReg;
		break;
	default:
		ret=-1;
		break;
	}
	
	return(ret);
}
