char **x3vm_strdup_ptrs[256];
int *x3vm_strdup_chn[256];
int x3vm_strdup_hash[256];
int x3vm_strdup_last=1;

int x3vm_hashstring8b(char *str)
{
	char *cs;
	int h;
	
	cs=str; h=0;
	while(*cs)
		{ h=((h<<1)^(h>>7))+(*cs++); }
	return(h&255);
}

int x3vm_strdup_i(char *str)
{
	char *s0;
	int h, c;
	
	if(!str)
		return(0);

	h=x3vm_hashstring8b(str);
	c=x3vm_strdup_hash[h];
	while(c>0)
	{
		s0=x3vm_strdup_ptrs[c>>8][c&255];
		if(!strcmp(s0, str))
			return(c);
		c=x3vm_strdup_chn[c>>8][c&255];
	}
	
	c=x3vm_strdup_last++;
	if(!x3vm_strdup_ptrs[c>>8])
	{
		x3vm_strdup_ptrs[c>>8]=malloc(256*sizeof(char *));
		x3vm_strdup_chn[c>>8]=malloc(256*sizeof(int));
	}
	
	x3vm_strdup_ptrs[c>>8][c&255]=strdup(str);
	x3vm_strdup_chn[c>>8][c&255]=x3vm_strdup_hash[h];
	x3vm_strdup_hash[h]=c;
	
	return(c);
}

char *x3vm_strdup(char *str)
{
	int c;
	
	c=x3vm_strdup_i(str);
	if(!c)
		return(NULL);
	return(x3vm_strdup_ptrs[c>>8][c&255]);
}

struct {
char *name;
int nmid;
}x3vm_nmidnames[]={
{"INVOP",		X3VM_NMID_INVOP},

{"ADDW",		X3VM_NMID_ADDW},
{"SUBW",		X3VM_NMID_SUBW},
{"ADDWU",		X3VM_NMID_ADDWU},
{"SUBWU",		X3VM_NMID_SUBWU},
{"ADD",			X3VM_NMID_ADD},
{"SUB",			X3VM_NMID_SUB},
{"SLL",			X3VM_NMID_SLL},
{"SRL",			X3VM_NMID_SRL},
{"SRA",			X3VM_NMID_SRA},
{"AND",			X3VM_NMID_AND},
{"OR",			X3VM_NMID_OR},
{"XOR",			X3VM_NMID_XOR},
{"LB",			X3VM_NMID_LB},
{"LH",			X3VM_NMID_LH},
{"LW",			X3VM_NMID_LW},
{"LD",			X3VM_NMID_LD},
{"LBU",			X3VM_NMID_LBU},
{"LHU",			X3VM_NMID_LHU},
{"LWU",			X3VM_NMID_LWU},
{"LDP",			X3VM_NMID_LDP},
{"SB",			X3VM_NMID_SB},
{"SH",			X3VM_NMID_SH},
{"SW",			X3VM_NMID_SW},
{"SD",			X3VM_NMID_SD},
{"MULW",		X3VM_NMID_MULW},
{"MULWU",		X3VM_NMID_MULWU},
{"LEA",			X3VM_NMID_LEA},
{"SDP",			X3VM_NMID_SDP},
{"MIN",			X3VM_NMID_MIN},
{"MAX",			X3VM_NMID_MAX},
{"SEQ",			X3VM_NMID_SEQ},
{"SNE",			X3VM_NMID_SNE},
{"SLT",			X3VM_NMID_SLT},
{"SGE",			X3VM_NMID_SGE},
{"SLTU",		X3VM_NMID_SLTU},
{"SGEU",		X3VM_NMID_SGEU},
{"ADDX",		X3VM_NMID_ADDX},
{"SUBX",		X3VM_NMID_SUBX},
{"MULQ",		X3VM_NMID_MULQ},
{"MULQU",		X3VM_NMID_MULQU},
{"SLA",			X3VM_NMID_SLA},
{"ANDX",		X3VM_NMID_ANDX},
{"ORX",			X3VM_NMID_ORX},
{"XORX",		X3VM_NMID_XORX},
{"JAL",			X3VM_NMID_JAL},
{"JALR",		X3VM_NMID_JALR},
{"BEQ",			X3VM_NMID_BEQ},
{"BNE",			X3VM_NMID_BNE},
{"BLT",			X3VM_NMID_BLT},
{"BGE",			X3VM_NMID_BGE},
{"BLTU",		X3VM_NMID_BLTU},
{"BGEU",		X3VM_NMID_BGEU},
{"BTSTT",		X3VM_NMID_BTSTT},
{"BTSTF",		X3VM_NMID_BTSTF},

{"SLAX",		X3VM_NMID_SLAX},
{"SLLX",		X3VM_NMID_SLLX},
{"SLAW",		X3VM_NMID_SLAW},
{"SLLW",		X3VM_NMID_SLLW},
{"LI",			X3VM_NMID_LI},
{"SHORI",		X3VM_NMID_SHORI},

{"RSUB",		X3VM_NMID_RSUB},
{"FLIH",		X3VM_NMID_FLDCH},
{"SRLW",		X3VM_NMID_SRLW},
{"SRAW",		X3VM_NMID_SRAW},
{"FSD2W",		X3VM_NMID_FSD2W},
{"FSD2H",		X3VM_NMID_FSD2H},
{"FLW2D",		X3VM_NMID_FLW2D},
{"FLH2D",		X3VM_NMID_FLH2D},

{"PACK",		X3VM_NMID_PACK},
{"PACKU",		X3VM_NMID_PACKU},
{"PACKUL",		X3VM_NMID_PACKUL},
{"PACKLU",		X3VM_NMID_PACKLU},

{"DMULW",		X3VM_NMID_DMULW},
{"DMULWI",		X3VM_NMID_DMULWU},
{"DMULH",		X3VM_NMID_DMULH},
{"DMULHU",		X3VM_NMID_DMULHU},

{"FADD",		X3VM_NMID_FADD},
{"FSUB",		X3VM_NMID_FSUB},
{"FMUL",		X3VM_NMID_FMUL},
{"FDIV",		X3VM_NMID_FDIV},
{"FMAC",		X3VM_NMID_FMAC},
{"FLH",			X3VM_NMID_FLH},
{"FLW",			X3VM_NMID_FLW},
{"FLD",			X3VM_NMID_FLD},
{"LEAB",		X3VM_NMID_LEAB},
{"LEAH",		X3VM_NMID_LEAH},
{"LEAW",		X3VM_NMID_LEAW},
{"LEAD",		X3VM_NMID_LEAD},
{"SH1ADD",		X3VM_NMID_SH1ADD},
{"FSH",			X3VM_NMID_FSH},
{"FSW",			X3VM_NMID_FSW},
{"FSD",			X3VM_NMID_FSD},

{"LUI",			X3VM_NMID_LUI},
{"AUIPC",		X3VM_NMID_AUIPC},
{"SH2ADD",		X3VM_NMID_SH2ADD},
{"SH3ADD",		X3VM_NMID_SH3ADD},
{"DIVW",		X3VM_NMID_DIVW},
{"DIVUW",		X3VM_NMID_DIVUW},
{"REMW",		X3VM_NMID_REMW},
{"REMUW",		X3VM_NMID_REMUW},
{"DIV",			X3VM_NMID_DIV},
{"DIVU",		X3VM_NMID_DIVU},
{"REM",			X3VM_NMID_REM},
{"REMU",		X3VM_NMID_REMU},
{"EBREAK",		X3VM_NMID_EBREAK},
{"ECALL",		X3VM_NMID_ECALL},
{"ADDUW",		X3VM_NMID_ADDUW},
{"SHORI32",		X3VM_NMID_SHORI32},

{"SGT",			X3VM_NMID_SGT},
{"FCVT.DUS",	X3VM_NMID_FCVTDUS},
{"FMIN",		X3VM_NMID_FMIN},
{"FMAX",		X3VM_NMID_FMAX},
{"EXT.SB",		X3VM_NMID_EXTSB},
{"EXT.UB",		X3VM_NMID_EXTUB},
{"EXT.SH",		X3VM_NMID_EXTSH},
{"EXT.UH",		X3VM_NMID_EXTUH},
{"FCVT.SD",		X3VM_NMID_FCVTSD},
{"FCVT.DS",		X3VM_NMID_FCVTDS},
{"FCVT.HD",		X3VM_NMID_FCVTHD},
{"FCVT.DH",		X3VM_NMID_FCVTDH},
{"FCVT.ID",		X3VM_NMID_FCVTID},
{"FCVT.DI",		X3VM_NMID_FCVTDI},
{"FCVY.IS",		X3VM_NMID_FCVTIS},
{"FCVT.SI",		X3VM_NMID_FCVTSI},

{"PADDW",		X3VM_NMID_PADDW},
{"PADDL",		X3VM_NMID_PADDL},
{"PSUBW",		X3VM_NMID_PSUBW},
{"PSUBL",		X3VM_NMID_PSUBL},
{"PADD.H",		X3VM_NMID_PADDH},
{"PADD.S",		X3VM_NMID_PADDF},
{"PSUB.H",		X3VM_NMID_PSUBH},
{"PSUB.S",		X3VM_NMID_PSUBF},
{"PMUL.H",		X3VM_NMID_PMULH},
{"PMUL.S",		X3VM_NMID_PMULF},
{"PADDX.S",		X3VM_NMID_PADDXF},
{"PSUBX.S",		X3VM_NMID_PSUBXF},
{"PMULX.S",		X3VM_NMID_PMULXF},
{"PADDX.D",		X3VM_NMID_PADDXD},
{"PSUBX.D",		X3VM_NMID_PSUBXD},
{"PMULX.D",		X3VM_NMID_PMULXD},

{"PMULSW",		X3VM_NMID_PMULSW},
{"PMULUW",		X3VM_NMID_PMULUW},
{"FSQRT",		X3VM_NMID_FSQRT},
{"MULQH",		X3VM_NMID_MULQH},
{"PMULSLW",		X3VM_NMID_PMULSLW},
{"PMULULW",		X3VM_NMID_PMULULW},
{"PMULSHW",		X3VM_NMID_PMULSHW},
{"PMULUHW",		X3VM_NMID_PMULUHW},

{"STST",		X3VM_NMID_STST},
{"SNTST",		X3VM_NMID_SNTST},
{"MULQHSU",		X3VM_NMID_MULQHSU},
{"MULQHU",		X3VM_NMID_MULQHU},

{"FMADD.S",		X3VM_NMID_FMADDS},
{"FMSUB.S",		X3VM_NMID_FMSUBS},
{"FMNADD.S",	X3VM_NMID_FMNADDS},
{"FMNSUB.S",	X3VM_NMID_FMNSUBS},
{"FMADD.D",		X3VM_NMID_FMADDD},
{"FMSUB.D",		X3VM_NMID_FMSUBD},
{"FMNADD.D",	X3VM_NMID_FMNADDD},
{"FMNSUB.D",	X3VM_NMID_FMNSUBD},
{"FMADD.H",		X3VM_NMID_FMADDH},
{"FMSUB.H",		X3VM_NMID_FMSUBH},
{"FMNADD.H",	X3VM_NMID_FMNADDH},
{"FMNSUB.H",	X3VM_NMID_FMNSUBH},
{"PMIN.S",		X3VM_NMID_PMINF},
{"PMIN.H",		X3VM_NMID_PMINH},
{"PMAX.S",		X3VM_NMID_PMAXF},
{"PMAX.H",		X3VM_NMID_PMAXH},

{"PDIV.S",		X3VM_NMID_PDIVF},
{"PDIV.H",		X3VM_NMID_PDIVH},
{"FEQ.D",		X3VM_NMID_FEQ},
{"FLT.D",		X3VM_NMID_FLT},
{"FGE.D",		X3VM_NMID_FGE},
{"FEQ.S",		X3VM_NMID_FEQF},
{"FLT.S",		X3VM_NMID_FLTF},
{"FGE.S",		X3VM_NMID_FGEF},
{"FEQ.H",		X3VM_NMID_FEQH},
{"FLT.H",		X3VM_NMID_FLTH},
{"FGE.H",		X3VM_NMID_FGEH},
{"PCVT.SH",		X3VM_NMID_PCVTSH},
{"PCVT.HS",		X3VM_NMID_PCVTHS},
{"PSQRT.S",		X3VM_NMID_PSQRTF},
{"PSQRT.H",		X3VM_NMID_PSQRTH},

{"FSGNJ.D",		X3VM_NMID_FSGNJ},
{"FSGNJN.D",	X3VM_NMID_FSGNJN},
{"FSGNJX.D",	X3VM_NMID_FSGNJX},
{"FSGNJ.S",		X3VM_NMID_FSGNJF},
{"FSGNJN.S",	X3VM_NMID_FSGNJNF},
{"FSGNJX.S",	X3VM_NMID_FSGNJXF},
{"FSGNJ.H",		X3VM_NMID_FSGNJH},
{"FSGNJN.H",	X3VM_NMID_FSGNJNH},
{"FSGNJX.H",	X3VM_NMID_FSGNJXH},

{"CSRRW",		X3VM_NMID_CSRRW},
{"CSRRS",		X3VM_NMID_CSRRS},
{"CSRRC",		X3VM_NMID_CSRRC},
{"CSRRWI",		X3VM_NMID_CSRRWI},
{"CSRRSI",		X3VM_NMID_CSRRSI},
{"CSRRCI",		X3VM_NMID_CSRRCI},

{NULL,			0}
};

char *x3vm_regnames[128]={
"ZR" , "LR" , "SP" , "GP" ,		//00..03
"TP" , "X5" , "X6" , "X7" ,		//04..07
"X8" , "X9" , "X10", "X11",		//08..0B
"X12", "X13", "X14", "X15",		//0C..0F
"X16", "X17", "X18", "X19",		//10..13
"X20", "X21", "X22", "X23",		//14..17
"X24", "X25", "X26", "X27",		//18..1B
"X28", "X29", "X30", "X31",		//1C..1F
"F0" , "F1" , "F2" , "F3" ,		//20..23
"F4" , "F5" , "F6" , "F7" ,		//24..27
"F8" , "F9" , "F10", "f11",		//28..2B
"F12", "F13", "F14", "F15",		//2C..2F
"F16", "F17", "F18", "F19",		//30..33
"F20", "F21", "F22", "F23",		//34..37
"F24", "F25", "F26", "F27",		//38..3B
"F28", "F29", "F30", "F31",		//3C..3F

"PC" , "-"  , "SPC", "TEA",		//40..43
"-"  , "-"  , "-"  , "-"  ,		//44..47
"-"  , "-"  , "-"  , "-"  ,		//48..4B
"-"  , "-"  , "-"  , "-"  ,		//4C..4F
"-"  , "-"  , "-"  , "-"  ,		//50..53
"-"  , "-"  , "-"  , "-"  ,		//54..57
"-"  , "-"  , "-"  , "-"  ,		//58..5B
"-"  , "-"  , "-"  , "-"  ,		//5C..5F
"PC" , "-"  , "-"  , "-"  ,		//60..63
"-"  , "-"  , "-"  , "-"  ,		//64..67
"-"  , "-"  , "-"  , "-"  ,		//68..6B
"-"  , "-"  , "-"  , "-"  ,		//6C..6F
"-"  , "-"  , "-"  , "-"  ,		//70..73
"-"  , "-"  , "-"  , "-"  ,		//74..77
"-"  , "-"  , "-"  , "-"  ,		//78..7B
"-"  , "-"  , "-"  , "-"  		//7C..7F
};

char *X3VM_NameForNmid(X3VM_Context *ctx, int nmid)
{
	char tbuf[64];
	int i;
	for(i=0; x3vm_nmidnames[i].name; i++)
		if(x3vm_nmidnames[i].nmid==nmid)
			return(x3vm_nmidnames[i].name);

	sprintf(tbuf, "UNK%02X", nmid);
	return(x3vm_strdup(tbuf));
//	return(NULL);
}



char *X3VM_NameForReg(X3VM_Context *ctx, int reg)
{
	return(x3vm_regnames[reg]);
}

char *X3VM_SPrintOpcode(X3VM_Context *ctx, char *sbuf, X3VM_Opcode *op, int fl)
{
	char *ct;
	
	ct=sbuf;
	
	if(fl&4)
	{
		sprintf(ct, "%012llX ", op->addr);
		ct+=strlen(ct);
	}
	
	switch(fl&3)
	{
	case 0:
		break;
	case 1:
		if(op->opfl&X3VM_OPFL_OP16)
			sprintf(ct, "%04X      ", op->opw0);
		else
			sprintf(ct, "%08X  ", op->opw0);
		ct+=strlen(ct);
		break;
	case 2:
		if(op->opw1)
			sprintf(ct, "%08X.%08X  ", op->opw0, op->opw1);
		else
			sprintf(ct, "%08X           ", op->opw0);
		ct+=strlen(ct);
		break;
	case 3:
		sprintf(ct, "%08X.%08X.%08X  ", op->opw0, op->opw1, op->opw2);
		ct+=strlen(ct);
		break;
	}
	
	sprintf(ct, "%-8s ", X3VM_NameForNmid(ctx, op->nmid));
	ct+=strlen(ct);
	switch(op->fmid)
	{
	case X3VM_FMID_NONE:
		break;
	case X3VM_FMID_J2RI:
//		sprintf(ct, "%s, 0x%012llX",
//			X3VM_NameForReg(ctx, op->rn),
//			(op->addr+op->imm)&0x0000FFFFFFFFFFFELL);

		sprintf(ct, "%s, %lld (0x%012llX)",
			X3VM_NameForReg(ctx, op->rn),
			op->imm,
			(op->addr+op->imm)&0x0000FFFFFFFFFFFELL);
		break;
	case X3VM_FMID_J3RI:
		sprintf(ct, "%s, %s, %lld (0x%012llX)",
			X3VM_NameForReg(ctx, op->rs),
			X3VM_NameForReg(ctx, op->rt),
			op->imm,
			(op->addr+op->imm)&0x0000FFFFFFFFFFFELL);
		break;
	case X3VM_FMID_3R:
		sprintf(ct, "%s, %s, %s",
			X3VM_NameForReg(ctx, op->rn),
			X3VM_NameForReg(ctx, op->rs),
			X3VM_NameForReg(ctx, op->rt));
		break;
	case X3VM_FMID_3RI:
//		if((op->imm<=(1<<20)) || (op->imm>=(-(1<<20))))
		if((op->imm<=(1<<16)) || (op->imm>=(-(1<<20))))
		{
			sprintf(ct, "%s, %s, %lld",
				X3VM_NameForReg(ctx, op->rn),
				X3VM_NameForReg(ctx, op->rs),
				op->imm);
			break;
		}
//		if((op->imm<=(1<<20)) || (op->imm>=(-(1<<20))))
		if((op->imm<=(1LL<<32)) || (op->imm>=0))
		{
			sprintf(ct, "%s, %s, 0x%llX",
				X3VM_NameForReg(ctx, op->rn),
				X3VM_NameForReg(ctx, op->rs),
				op->imm);
			break;
		}
		sprintf(ct, "%s, %s, 0x%016llX",
			X3VM_NameForReg(ctx, op->rn),
			X3VM_NameForReg(ctx, op->rs),
			op->imm);
		break;
	case X3VM_FMID_2R:
		sprintf(ct, "%s, %s",
			X3VM_NameForReg(ctx, op->rn),
			X3VM_NameForReg(ctx, op->rs));
		break;
	case X3VM_FMID_2RI:
		if((op->imm<=(1<<16)) || (op->imm>=(-(1<<20))))
		{
			sprintf(ct, "%s, %lld",
				X3VM_NameForReg(ctx, op->rn),
				op->imm);
			break;
		}
//		if((op->imm<=(1<<20)) || (op->imm>=(-(1<<20))))
		if((op->imm<=(1LL<<32)) || (op->imm>=0))
		{
			sprintf(ct, "%s, 0x%llX",
				X3VM_NameForReg(ctx, op->rn),
				op->imm);
			break;
		}
		sprintf(ct, "%s, 0x%016llX",
			X3VM_NameForReg(ctx, op->rn),
			op->imm);
		break;
	case X3VM_FMID_LDRI:
	case X3VM_FMID_STRI:
//		if((op->imm<=(1<<20)) || (op->imm>=(-(1<<20))))
		if((op->imm<=(1<<16)) || (op->imm>=(-(1<<20))))
		{
			sprintf(ct, "%s, %lld(%s)",
				X3VM_NameForReg(ctx, op->rn),
				op->imm<<op->sc,
				X3VM_NameForReg(ctx, op->rs));
			break;
		}
		if((op->imm<=(1LL<<32)) || (op->imm>=0))
		{
			sprintf(ct, "%s, 0x%llX(%s)",
				X3VM_NameForReg(ctx, op->rn),
				op->imm<<op->sc,
				X3VM_NameForReg(ctx, op->rs));
			break;
		}

		sprintf(ct, "%s, 0x%llX(%s)",
			X3VM_NameForReg(ctx, op->rn),
			op->imm<<op->sc,
			X3VM_NameForReg(ctx, op->rs));
		break;
	case X3VM_FMID_LDRR:
	case X3VM_FMID_STRR:
		sprintf(ct, "%s, (%s, %s*%d)",
			X3VM_NameForReg(ctx, op->rn),
			X3VM_NameForReg(ctx, op->rs),
			X3VM_NameForReg(ctx, op->rt),
			1<<op->sc);
		break;
	}
	ct+=strlen(ct);
	
	strcat(ct, "\n");
	ct+=strlen(ct);
	
	return(ct);
}

int X3VM_PrintOpcode(X3VM_Context *ctx, X3VM_Opcode *op, int fl)
{
	char tbuf[256];
	
	X3VM_SPrintOpcode(ctx, tbuf, op, fl);
	fputs(tbuf, stdout);
	fflush(stdout);
	return(0);
}

int X3VM_DumpTrace(X3VM_Context *ctx, X3VM_Trace *tr, int fl)
{
	X3VM_Opcode *op;
	char *sym;
	int ofs;
	int i, j, k, ofl, mol;
	
	if(!tr)
	{
		return(0);
	}

	X3PE_LookupSymbolForAddr(ctx, tr->addr, &sym, &ofs, 0);

	if(sym)
	{
		printf("@ 0x%012llX  %s+%d\n", tr->addr, sym, ofs);
	}else
	{
		printf("@ 0x%012llX\n", tr->addr);
	}
	
	ofl=4; mol=1;
	for(i=0; i<tr->n_ops; i++)
	{
//		op=tr->ops[i];
		op=X3VM_GetTraceOpIx(ctx, tr, i);
		j=1;
		if(op->opw1)	j=2;
		if(op->opw2)	j=3;
		if(j>mol)		mol=j;
	}
	ofl|=mol;

	for(i=0; i<tr->n_ops; i++)
	{
//		op=tr->ops[i];
		op=X3VM_GetTraceOpIx(ctx, tr, i);

		sym=NULL; ofs=0;
		X3PE_LookupSymbolForAddr(ctx, op->addr, &sym, &ofs, 1);
		if(sym && !ofs)
		{
			printf("    %s\n", sym);
		}

		X3VM_PrintOpcode(ctx, op, ofl);
	}
	return(0);
}

int X3VM_DumpTraceLog(X3VM_Context *ctx)
{
	X3VM_Trace *tr;
	int i, j, k, max;
	
	max=64;
//	max=128;
	for(i=0; i<max; i++)
	{
//		j=(ctx->tr_logpos-i-1)&255;
		j=(ctx->tr_logpos-max+i)&255;
		tr=ctx->tr_log[j];
		X3VM_DumpTrace(ctx, tr, 0);
	}
	return(0);
}

int X3VM_DumpRegisters(X3VM_Context *ctx)
{
	int i;
	for(i=0; i<48; i++)
	{
		if((i>=32) && !ctx->reg[i*2+0] && !ctx->reg[i*2+1])
			continue;
		printf("%-4s=%016llX %-4s=%016llX\n",
			X3VM_NameForReg(ctx, i*2+0), ctx->reg[i*2+0],
			X3VM_NameForReg(ctx, i*2+1), ctx->reg[i*2+1]);
	}
	if(ctx->fault)
	{
		printf("Fault=%04X\n", ctx->fault);
	}
	return(0);
}

X3VM_Context *X3VM_AllocContext()
{
	X3VM_Context *ctx;
	
	ctx=malloc(sizeof(X3VM_Context));
	memset(ctx, 0, sizeof(X3VM_Context));
	return(ctx);
}

X3VM_Trace *X3VM_AllocTrace(X3VM_Context *ctx)
{
	X3VM_Trace *tmp;
	int i;
	
	tmp=ctx->free_tr;
	if(tmp)
	{
		ctx->free_tr=tmp->tr_next;
		memset(tmp, 0, sizeof(X3VM_Trace));
		return(tmp);
	}
	
	ctx->stat_alloc_traces+=64;
	tmp=malloc(64*sizeof(X3VM_Trace));
	for(i=0; i<63; i++)
	{
		tmp->tr_next=ctx->free_tr;
		ctx->free_tr=tmp;
		tmp++;
	}
	
	memset(tmp, 0, sizeof(X3VM_Trace));
	return(tmp);
}

X3VM_Opcode *X3VM_AllocOpcode(X3VM_Context *ctx)
{
	X3VM_Opcode *tmp;
	int i;
	
	tmp=ctx->free_opc;
	if(tmp)
	{
		ctx->free_opc=tmp->subop;
		memset(tmp, 0, sizeof(X3VM_Opcode));
		return(tmp);
	}
	
	ctx->stat_alloc_ops+=256;
	tmp=malloc(256*sizeof(X3VM_Opcode));
	for(i=0; i<255; i++)
	{
		tmp->subop=ctx->free_opc;
		ctx->free_opc=tmp;
		tmp++;
	}
	
	memset(tmp, 0, sizeof(X3VM_Opcode));
	return(tmp);
}

u64 x3vm_rng_seed1;
u64 x3vm_rng_seed2;
u64 x3vm_rng_seed3;
u64 x3vm_rng_seed4;
u32 x3vm_rng_tref;

int X3VM_RngGblMix()
{
	x3vm_rng_seed1=(x3vm_rng_seed1<<1)^(x3vm_rng_seed1>>5);
	x3vm_rng_seed2=(x3vm_rng_seed2<<1)^(x3vm_rng_seed2>>11);
	x3vm_rng_seed3=(x3vm_rng_seed3<<1)^(x3vm_rng_seed3>>17);
	x3vm_rng_seed4=(x3vm_rng_seed4<<1)^(~(x3vm_rng_seed4>>29));
	x3vm_rng_seed1^=x3vm_rng_seed2>>13;
	x3vm_rng_seed2^=x3vm_rng_seed3>>23;
	x3vm_rng_seed3^=x3vm_rng_seed4>>27;
	x3vm_rng_seed4^=x3vm_rng_seed1>>19;
	return(0);
}

int X3VM_RngGblAutoSeed()
{
	void *tptr;
	u64 sv1, sv2, sv3;
	u64 t0, t1;
	int i;

	tptr=malloc(231);
	free(tptr);

	sv1=(u64)tptr;
	sv2=1; sv3=1;
	t0=clock();
	t1=t0;
	while(t1==t0)
	{
		sv2+=sv1;
		sv1=(sv1<<1)^(sv1>>29);
		sv3+=sv1-sv2;
		t1=clock();
	}
	
	x3vm_rng_seed1=sv1;
	x3vm_rng_seed2=sv2;
	x3vm_rng_seed3=sv3;
	x3vm_rng_seed4=~(sv3^sv1);
	
	i=16+(sv1&15);
	while(i--)
	{
		X3VM_RngGblMix();
	}
	
	x3vm_rng_tref=t1;
	
	printf("X3VM_RngGblAutoSeed: %016llX %016llX %016llX\n",
		x3vm_rng_seed1, x3vm_rng_seed2, x3vm_rng_seed3);
	return(0);
}

int X3VM_RngGblRand16()
{
	u32 t0, t1, td;
	u16 val;
	if(!x3vm_rng_seed1)
		{ X3VM_RngGblAutoSeed(); }
	
	t0=x3vm_rng_tref;
	t1=clock();
	if(t1>t0)
		{ td=t1-t0;	}
	else
		{ td=t1^t0; }
	x3vm_rng_tref=t1;
	val=td;
	while(val--)
		X3VM_RngGblMix();
	
	X3VM_RngGblMix();
	val=x3vm_rng_seed1^(x3vm_rng_seed2>>3)^(x3vm_rng_seed3>>7);
	X3VM_RngGblMix();
	return(val);
}

u32 X3VM_RngGblRand32()
{
	u32 v0;
	v0=X3VM_RngGblRand16();
	v0=(v0<<8)^X3VM_RngGblRand16();
	v0=(v0<<8)^X3VM_RngGblRand16();
	return(v0);
}

u64 X3VM_RngGblRand64()
{
	u64 v0;
	v0=X3VM_RngGblRand16();
	v0=(v0<<8)^X3VM_RngGblRand16();
	v0=(v0<<8)^X3VM_RngGblRand16();

	v0=(v0<<8)^X3VM_RngGblRand16();
	v0=(v0<<8)^X3VM_RngGblRand16();
	v0=(v0<<8)^X3VM_RngGblRand16();
	v0=(v0<<8)^X3VM_RngGblRand16();
	return(v0);
}

int X3VM_RngGblInjectSeed(u64 sbits)
{
	if(!x3vm_rng_seed1)
		{ X3VM_RngGblAutoSeed(); }
	x3vm_rng_seed1+=sbits;
	X3VM_RngGblMix();
	X3VM_RngGblMix();
	X3VM_RngGblMix();
	return(0);
}


int X3VM_RngMix(X3VM_Context *ctx)
{
	ctx->rng_seed1=(ctx->rng_seed1<<1)^(ctx->rng_seed1>>5);
	ctx->rng_seed2=(ctx->rng_seed2<<1)^(ctx->rng_seed2>>11);
	ctx->rng_seed3=(ctx->rng_seed3<<1)^(ctx->rng_seed3>>17);
	ctx->rng_seed4=(ctx->rng_seed4<<1)^(~(ctx->rng_seed4>>29));
	ctx->rng_seed1^=ctx->rng_seed2>>13;
	ctx->rng_seed2^=ctx->rng_seed3>>23;
	ctx->rng_seed3^=ctx->rng_seed4>>27;
	ctx->rng_seed4^=ctx->rng_seed1>>19;
	return(0);
}

int X3VM_RngAutoSeed(X3VM_Context *ctx)
{
	u64 sv1, sv2, sv3;
	u64 t0, t1;
	int i;
	sv1=(u64)ctx;
	sv2=X3VM_RngGblRand32();
	sv3=X3VM_RngGblRand32();
	t0=clock();
	t1=t0;
	while(t1==t0)
	{
		sv2+=sv1;
		sv1=(sv1<<1)^(sv1>>29);
		sv3+=sv1-sv2;
		t1=clock();
	}
	
	ctx->rng_seed1=sv1;
	ctx->rng_seed2=sv2;
	ctx->rng_seed3=sv3;
	ctx->rng_seed4=~(sv3^sv1);
	
	i=16+(sv1&15);
	while(i--)
	{
		X3VM_RngMix(ctx);
	}
	
	printf("X3VM_RngAutoSeed: %016llX %016llX %016llX\n",
		ctx->rng_seed1, ctx->rng_seed2, ctx->rng_seed3);
	return(0);
}

int X3VM_RngRand16(X3VM_Context *ctx)
{
	u16 val;
	if(!ctx->rng_seed1)
		{ X3VM_RngAutoSeed(ctx); }

	if(!ctx->rng_mixto)
	{
		ctx->rng_seed1+=X3VM_RngGblRand32();
		ctx->rng_mixto=64+(ctx->rng_seed1&63);
		X3VM_RngMix(ctx);
		X3VM_RngMix(ctx);
	}else
	{
		ctx->rng_mixto--;
	}

	X3VM_RngMix(ctx);
	val=ctx->rng_seed1^(ctx->rng_seed2>>3)^(ctx->rng_seed3>>7);
	X3VM_RngMix(ctx);
	return(val);
}

u32 X3VM_RngRand32(X3VM_Context *ctx)
{
	u32 v0;
	v0=X3VM_RngRand16(ctx);
	v0=(v0<<8)^X3VM_RngRand16(ctx);
	v0=(v0<<8)^X3VM_RngRand16(ctx);
	return(v0);
}

u64 X3VM_RngRand64(X3VM_Context *ctx)
{
	u64 v0;
	v0=X3VM_RngRand16(ctx);
	v0=(v0<<8)^X3VM_RngRand16(ctx);
	v0=(v0<<8)^X3VM_RngRand16(ctx);

	v0=(v0<<8)^X3VM_RngRand16(ctx);
	v0=(v0<<8)^X3VM_RngRand16(ctx);
	v0=(v0<<8)^X3VM_RngRand16(ctx);
	v0=(v0<<8)^X3VM_RngRand16(ctx);
	return(v0);
}

int X3VM_ThrowFault(X3VM_Context *ctx, int fault)
{
	int i;

	if(ctx->fault)
		return(0);

	for(i=0; i<128; i++)
		ctx->ex_reg[i]=ctx->reg[i];

	ctx->fault=fault;
	ctx->tr_next=NULL;
	ctx->tr_bra=NULL;
	return(0);
}

int X3VM_ThrowMemFault(X3VM_Context *ctx, int fault, x3vm_addr addr)
{
	ctx->reg[X3VM_REG_SPC]=
		(((u64)(ctx->pc_hibits))<<48) |
		(ctx->reg[X3VM_REG_BPC]&0x0000FFFFFFFFFFFEULL) | 1;
	ctx->reg[X3VM_REG_TEA]=addr;
	X3VM_ThrowFault(ctx, fault);
	return(0);
}

int X3VM_DoRun(X3VM_Context *ctx, int limit)
{
	X3VM_Trace *tr;
	int i, j, k, n, v, rcyc;
	
	ctx->fault=0;
	n=limit;

	while((n>0) && !ctx->fault)
	{
		rcyc=0;
		tr=ctx->tr_cont;
		if(!tr)
		{
			tr=X3VM_TraceGetForAddr(ctx, ctx->reg[X3VM_REG_PC], 0);
			if(!tr)
				break;
		}
		while(tr && n--)
		{
			ctx->tr_log[ctx->tr_logpos++]=tr;
//			ctx->totcyc+=tr->n_ops;
			ctx->totcyc+=tr->n_orgops;
//			rcyc+=tr->n_ops;
			tr=tr->Run(ctx, tr);
		}
		ctx->tr_cont=tr;
//		ctx->totcyc+=rcyc;
		
		if(ctx->fault)
		{
			for(i=0; i<128; i++)
				ctx->reg[i]=ctx->ex_reg[i];
		}
		
		if(ctx->fault==X3VM_FAULT_ECALL)
		{
			ctx->reg[X3VM_REG_PC]=ctx->reg[X3VM_REG_SPC]+4;
			ctx->fault=0;
			v=X3VM_HandleSyscall(ctx);
			if(v<0)
				ctx->fault=X3VM_FAULT_BADCALL;
			ctx->reg[10]=v;
			continue;
		}
	}
	return(0);
}
