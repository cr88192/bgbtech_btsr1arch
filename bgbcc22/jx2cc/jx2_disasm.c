struct {
int opn;	//opcode bits
int opm;	//opcode mask
int nmid;	//name index
int fmid;	//form index
}bgbcc_jx2_optab[]={
{0x0000, 0xFF00, BGBCC_SH_NMID_MOVB,	BGBCC_SH_FMID_REGST},
{0x0100, 0xFF00, BGBCC_SH_NMID_MOVW,	BGBCC_SH_FMID_REGST},
{0x0200, 0xFF00, BGBCC_SH_NMID_MOVL,	BGBCC_SH_FMID_REGST},
{0x0300, 0xFF00, BGBCC_SH_NMID_MOVUB,	BGBCC_SH_FMID_REGLD},
{0x0400, 0xFF00, BGBCC_SH_NMID_MOVB,	BGBCC_SH_FMID_REGSTR0N},
{0x0500, 0xFF00, BGBCC_SH_NMID_MOVW,	BGBCC_SH_FMID_REGSTR0N},
{0x0600, 0xFF00, BGBCC_SH_NMID_MOVL,	BGBCC_SH_FMID_REGSTR0N},
{0x0700, 0xFF00, BGBCC_SH_NMID_MOVUB,	BGBCC_SH_FMID_REGLDR0M},
{0x0800, 0xFF00, BGBCC_SH_NMID_MOVB,	BGBCC_SH_FMID_REGLD},
{0x0900, 0xFF00, BGBCC_SH_NMID_MOVW,	BGBCC_SH_FMID_REGLD},
{0x0A00, 0xFF00, BGBCC_SH_NMID_MOVL,	BGBCC_SH_FMID_REGLD},
{0x0B00, 0xFF00, BGBCC_SH_NMID_MOVUW,	BGBCC_SH_FMID_REGLD},
{0x0C00, 0xFF00, BGBCC_SH_NMID_MOVB,	BGBCC_SH_FMID_REGLDR0M},
{0x0D00, 0xFF00, BGBCC_SH_NMID_MOVW,	BGBCC_SH_FMID_REGLDR0M},
{0x0E00, 0xFF00, BGBCC_SH_NMID_MOVL,	BGBCC_SH_FMID_REGLDR0M},
{0x0F00, 0xFF00, BGBCC_SH_NMID_MOVUW,	BGBCC_SH_FMID_REGLDR0M},

{0x1000, 0xFF00, BGBCC_SH_NMID_ADD,		BGBCC_SH_FMID_REGREG},
{0x1100, 0xFF00, BGBCC_SH_NMID_SUB,		BGBCC_SH_FMID_REGREG},
{0x1200, 0xFF00, BGBCC_SH_NMID_ADDC,	BGBCC_SH_FMID_REGREG},
{0x1300, 0xFF00, BGBCC_SH_NMID_SUBC,	BGBCC_SH_FMID_REGREG},
{0x1400, 0xFF00, BGBCC_SH_NMID_TST,		BGBCC_SH_FMID_REGREG},
{0x1500, 0xFF00, BGBCC_SH_NMID_AND,		BGBCC_SH_FMID_REGREG},
{0x1600, 0xFF00, BGBCC_SH_NMID_OR,		BGBCC_SH_FMID_REGREG},
{0x1700, 0xFF00, BGBCC_SH_NMID_XOR,		BGBCC_SH_FMID_REGREG},
{0x1800, 0xFF00, BGBCC_SH_NMID_MOV,		BGBCC_SH_FMID_REGREG},
{0x1900, 0xFF00, BGBCC_SH_NMID_DMULS,	BGBCC_SH_FMID_REGREG},
{0x1A00, 0xFF00, BGBCC_SH_NMID_LDC,		BGBCC_SH_FMID_REGREG},
{0x1B00, 0xFF00, BGBCC_SH_NMID_STC,		BGBCC_SH_FMID_REGREG},
{0x1C00, 0xFF00, BGBCC_SH_NMID_CMPEQ,	BGBCC_SH_FMID_REGREG},
{0x1D00, 0xFF00, BGBCC_SH_NMID_CMPHI,	BGBCC_SH_FMID_REGREG},
{0x1E00, 0xFF00, BGBCC_SH_NMID_CMPGT,	BGBCC_SH_FMID_REGREG},
{0x1F00, 0xFF00, BGBCC_SH_NMID_DMULU,	BGBCC_SH_FMID_REGREG},

{0x2000, 0xFF00, BGBCC_SH_NMID_BRA,		BGBCC_SH_FMID_IMM},
{0x2100, 0xFF00, BGBCC_SH_NMID_BSR,		BGBCC_SH_FMID_IMM},
{0x2200, 0xFF00, BGBCC_SH_NMID_BT,		BGBCC_SH_FMID_IMM},
{0x2300, 0xFF00, BGBCC_SH_NMID_BF,		BGBCC_SH_FMID_IMM},
{0x2400, 0xFF00, BGBCC_SH_NMID_LDIZ,	BGBCC_SH_FMID_IMM},
{0x2500, 0xFF00, BGBCC_SH_NMID_LDIN,	BGBCC_SH_FMID_IMM},
{0x2600, 0xFF00, BGBCC_SH_NMID_LDISH,	BGBCC_SH_FMID_IMM},

{0x2800, 0xFF00, BGBCC_SH_NMID_MOVL,	BGBCC_SH_FMID_REGVIMM},
{0x2A00, 0xFF00, BGBCC_SH_NMID_MOVL,	BGBCC_SH_FMID_REGVIMM},

{0, 0, 0, 0}
};

int BGBCC_JX2_TryDisassembleOpcodeI1(
	BGBCC_JX2_Context *ctx,
	int opw,
	int *rnmid, int *rfmid,
	BGBCC_JX2_OpcodeArg *arg0,
	BGBCC_JX2_OpcodeArg *arg1,
	BGBCC_JX2_OpcodeArg *arg2)
{
	int nmid, fmid;
	int i;
	
	for(i=0; bgbcc_jx2_optab[i].opm!=0; i++)
	{
		if((opw&bgbcc_jx2_optab[i].opm)==bgbcc_jx2_optab[i].opn)
			break;
	}

	if(!bgbcc_jx2_optab[i].opm)
	{
		arg0->ty=BGBCC_SH_OPVTY_NONE;
		arg1->ty=BGBCC_SH_OPVTY_NONE;
		arg2->ty=BGBCC_SH_OPVTY_NONE;

		*rnmid=BGBCC_SH_NMID_UNK;
		*rfmid=0;
		return(-1);
	}

	nmid=bgbcc_jx2_optab[i].nmid;
	fmid=bgbcc_jx2_optab[i].fmid;
	*rnmid=nmid;
	*rfmid=fmid;

	arg0->ty=BGBCC_SH_OPVTY_NONE;
	arg1->ty=BGBCC_SH_OPVTY_NONE;
	arg2->ty=BGBCC_SH_OPVTY_NONE;
	
	switch(fmid)
	{
	case BGBCC_SH_FMID_NONE:
		break;
	case BGBCC_SH_FMID_REGREG:
		arg0->ty=BGBCC_SH_OPVTY_REG;
		arg1->ty=BGBCC_SH_OPVTY_REG;
		arg0->breg=((opw   )&15);
		arg1->breg=((opw>>4)&15);
		break;
	case BGBCC_SH_FMID_REGRN:
		arg0->ty=BGBCC_SH_OPVTY_REG;
		arg0->breg=((opw>>4)&15);
		break;
	case BGBCC_SH_FMID_REGVIMM:
		arg0->ty=BGBCC_SH_OPVTY_IMM;
		arg1->ty=BGBCC_SH_OPVTY_REG;
		arg0->imm=opw&15;
		arg1->breg=((opw>>4)&15);
		break;
	case BGBCC_SH_FMID_REGIMM:
		arg0->ty=BGBCC_SH_OPVTY_IMM;
		arg1->ty=BGBCC_SH_OPVTY_REG;
		arg0->imm=(sbyte)opw;
		arg1->breg=((opw>>8)&15);
		break;

	case BGBCC_SH_FMID_REGSTR0N:
		arg0->ty=BGBCC_SH_OPVTY_REG;
		arg1->ty=BGBCC_SH_OPVTY_RRMEM;
		arg0->breg=((opw   )&15);
		arg1->breg=((opw>>4)&15);
		arg1->ireg=BGBCC_SH_REG_MACL;
		break;
	case BGBCC_SH_FMID_DREGLDR0M:
		arg0->ty=BGBCC_SH_OPVTY_RRMEM;
		arg1->ty=BGBCC_SH_OPVTY_REG;
		arg0->breg=((opw   )&15);
		arg0->ireg=BGBCC_SH_REG_MACL;
		arg1->breg=((opw>>4)&15);
		break;
	default:
		break;
	}
}

char *BGBCC_JX2_NameForNmid(BGBCC_JX2_Context *ctx, int nmid)
{
	char *s0;

	switch(nmid)
	{
	case BGBCC_SH_NMID_UNK:		s0="?UNK"; break;
	case BGBCC_SH_NMID_MOV:		s0="MOV"; break;
	case BGBCC_SH_NMID_MOVB:	s0="MOV.B"; break;
	case BGBCC_SH_NMID_MOVW:	s0="MOV.W"; break;
	case BGBCC_SH_NMID_MOVL:	s0="MOV.L"; break;
	case BGBCC_SH_NMID_ADD:		s0="ADD"; break;
	case BGBCC_SH_NMID_ADDC:	s0="ADC"; break;
	case BGBCC_SH_NMID_SUB:		s0="SUB"; break;
	case BGBCC_SH_NMID_SUBC:	s0="SBB"; break;

	case BGBCC_SH_NMID_CMPHS:	s0="CMPHS"; break;
	case BGBCC_SH_NMID_CMPHI:	s0="CMPHI"; break;
	case BGBCC_SH_NMID_CMPEQ:	s0="CMPEQ"; break;
	case BGBCC_SH_NMID_CMPGE:	s0="CMPGE"; break;
	case BGBCC_SH_NMID_CMPGT:	s0="CMPGT"; break;

	case BGBCC_SH_NMID_JMP:		s0="JMP"; break;
	case BGBCC_SH_NMID_JSR:		s0="JSR"; break;
	case BGBCC_SH_NMID_BRA:		s0="BRA"; break;
	case BGBCC_SH_NMID_BSR:		s0="BSR"; break;
	case BGBCC_SH_NMID_BT:		s0="BT"; break;
	case BGBCC_SH_NMID_BF:		s0="BF"; break;

	case BGBCC_SH_NMID_DMULU:	s0="MULU"; break;
	case BGBCC_SH_NMID_DMULS:	s0="MULS"; break;

	case BGBCC_SH_NMID_TST:		s0="TST"; break;
	case BGBCC_SH_NMID_AND:		s0="AND"; break;
	case BGBCC_SH_NMID_XOR:		s0="XOR"; break;
	case BGBCC_SH_NMID_OR:		s0="OR"; break;

	case BGBCC_SH_NMID_CLRT:	s0="CLRT"; break;
	case BGBCC_SH_NMID_SETT:	s0="SETT"; break;
	case BGBCC_SH_NMID_NOP:		s0="NOP"; break;
	case BGBCC_SH_NMID_MOVT:	s0="MOVT"; break;
	case BGBCC_SH_NMID_RTS:		s0="RTS"; break;
	case BGBCC_SH_NMID_SLEEP:	s0="SLEEP"; break;
	case BGBCC_SH_NMID_RTE:		s0="RTE"; break;

	default:					s0="?BAD"; break;
	}
	return(s0);
}

int BGBCC_JX2_TryDisassembleOpcode_PrintArg(
	BGBCC_JX2_Context *ctx,
	BGBCC_JX2_OpcodeArg *arg)
{
	switch(arg->ty)
	{
	case BGBCC_SH_OPVTY_NONE:
		break;
	case BGBCC_SH_OPVTY_IMM:
		printf("#%d", op->imm);
		break;
	case BGBCC_SH_OPVTY_REG:
		printf("%s", BGBCC_JX2_NameForReg(ctx, op->breg));
		break;
	}
}

int BGBCC_JX2_TryDisassembleOpcodeI0(
	BGBCC_JX2_Context *ctx,
	int pc, int opw)
{
	BGBCC_JX2_OpcodeArg arg0;
	BGBCC_JX2_OpcodeArg arg1;
	BGBCC_JX2_OpcodeArg arg2;
	int nmid, fmid;
	
	BGBCC_JX2_TryDisassembleOpcodeI1(ctx, opw,
		&nmid, &fmid, &arg0, &arg1, &arg2);
		
	printf("%06X %04X %s", pc, opw,
		BGBCC_JX2_NameForNmid(ctx, nmid));
}

