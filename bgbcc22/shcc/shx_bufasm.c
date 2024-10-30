char *BGBCC_SHXA_EatWhite(char *cs)
{
	return(BGBCP_EatWhite(cs));
}

char *BGBCC_SHXA_EatWhiteNoLinebreak(char *cs)
{
	return(BGBCP_EatWhiteNLb(cs));
}

char *BGBCC_SHXA_EatWhiteOnlyNoLinebreak(char *cs)
{
	return(BGBCP_EatWhiteOnly2(cs));
}

char *BGBCC_SHXA_ParseTokenBasic(char *cs, char *tb)
{
	char *cs1;
	int ty;

	cs1=BGBCP_Token(cs, tb+1, &ty);

	switch(ty)
	{
	case BTK_NUMBER:
		tb[0]='|'; break;
	case BTK_NAME:
		tb[0]='I'; break;
	case BTK_STRING:
		tb[0]='S'; break;
	case BTK_CHARSTRING:
		tb[0]='C'; break;
	case BTK_MISC:
	case BTK_SEPERATOR:
	case BTK_BRACE:
	case BTK_OPERATOR:
		tb[0]='X'; break;
	default:
		tb[0]='X'; break;
	}

	return(cs1);
}

char *BGBCC_SHXA_ParseTokenBasicAltn(char *s, char *tb)
{
	char *t, *te;
	int i, j, k;

	t=tb; te=tb+252;

	s=BGBCC_SHXA_EatWhite(s);
	if(!(*s))
	{
		*t++=0; *t++=0;
		return(s);
	}

	if((*s=='_') || (*s=='$') ||
		(*s=='.') || (*s=='/') || 
		((*s>='A') && (*s<='Z')) ||
		((*s>='a') && (*s<='z')))
	{
		*t++='I';
		while(*s && (t<te) &&
			((*s=='_') || (*s=='$') ||
			 (*s=='.') || (*s=='/') ||
			((*s>='A') && (*s<='Z')) ||
			((*s>='a') && (*s<='z')) ||
			((*s>='0') && (*s<='9'))))
				*t++=*s++;
		*t++=0;
		return(s);
	}

	if((*s=='-') &&
		((s[1]>='0') && (s[1]<='9')))
	{
		*t++='|';
		*t++=*s++;
		while(*s && (t<te) && ((*s=='_') || ((*s>='0') && (*s<='9'))))
			*t++=*s++;
		*t++=0;
		return(s);
	}

	s=BGBCC_SHXA_ParseTokenBasic(s, tb);
	return(s);
}

char *BGBCC_SHXA_ParseToken(char *cs, char **rstr)
{
	char tb[256];
	cs=BGBCC_SHXA_ParseTokenBasic(cs, tb);
	*rstr=bgbcc_rstrdup(tb);
	return(cs);
}

char *BGBCC_SHXA_ParseTokenAlt(char *cs, char **rstr)
{
	char tb[256];
	cs=BGBCC_SHXA_ParseTokenBasicAltn(cs, tb);
	*rstr=bgbcc_rstrdup(tb);
	return(cs);
}

#if 0
char *BGBCC_SHXA_ParseToken(char *cs, char **rtok)
{
	static char tkb[16][256];
	static byte tkrov;
	char *tk, *cs1;
	int ty;

	tk=tkb[(tkrov++)&15];
	cs1=BGBCP_Token(cs, tk+1, &ty);

	*rtok=tk;

	switch(ty)
	{
	case BTK_NUMBER:
		tk[0]='|'; break;
	case BTK_NAME:
		tk[0]='I'; break;
	case BTK_STRING:
		tk[0]='S'; break;
	case BTK_CHARSTRING:
		tk[0]='C'; break;
	case BTK_MISC:
	case BTK_SEPERATOR:
	case BTK_BRACE:
	case BTK_OPERATOR:
		tk[0]='X'; break;
	default:
		tk[0]='X'; break;
	}

	return(cs1);
}

char *BGBCC_SHXA_ParseTokenAlt(char *cs, char **rtok)
{
	return(BGBCC_SHXA_ParseToken(cs, rtok));
}
#endif

int BGBCC_SHXA_GetRegId(char *str)
{
	int i, t, t1, isdr;

	switch(str[0])
	{
	case 'F':	case 'f':
	case 'D':	case 'd':
		if((str[1]=='R') || (str[1]=='r'))
		{
			t=BGBCC_SH_REG_FR0; isdr=0;
			if((str[0]=='D') || (str[0]=='d'))
				{ t=BGBCC_SH_REG_DR0; isdr=1; }
			if(str[2]=='0')
			{
				if(!str[3])
					return(t);

				if((str[3]>='0') && (str[3]<='9') && !str[4])
				{
					t1=t+(str[3]-'0');
//					if(isdr)
//						t1=BGBCC_SH_REG_DR0+(((t1&15)>>1)|((t1&1)<<3));
					return(t1);
				}
			}else if(str[2]=='1')
			{
				if(!str[3])
					return(t+1);
				if((str[3]>='0') && (str[3]<='5') && !str[4])
				{
					t1=t+10+(str[3]-'0');
//					if(isdr)
//						t1=BGBCC_SH_REG_DR0+(((t1&15)>>1)|((t1&1)<<3));
					return(t1);
				}
			}else if((str[2]>='0') && (str[2]<='9') && !str[3])
			{
				t1=t+(str[2]-'0');
//				if(isdr)
//					t1=BGBCC_SH_REG_DR0+(((t1&15)>>1)|((t1&1)<<3));
				return(t1);
			}
		}

		if(!bgbcc_stricmp(str, "fpscr"))
			return(BGBCC_SH_REG_FPSCR);
		if(!bgbcc_stricmp(str, "fpul"))
			return(BGBCC_SH_REG_FPUL);
		break;

	case 'R':	case 'r':
		if((str[1]=='D') || (str[1]=='d') ||
			(str[1]=='Q') || (str[1]=='q'))
		{
			t=BGBCC_SH_REG_R0;
			if((str[1]=='D') || (str[1]=='d'))
				t=BGBCC_SH_REG_RD0;
			if((str[1]=='Q') || (str[1]=='q'))
				t=BGBCC_SH_REG_RQ0;

			if(!str[3])
			{
				if((str[2]>='0') && (str[2]<='9'))
					{ return(t+(str[2]-'0')); }
				break;
			}

			if(!str[4])
			{
				if((str[2]>='0') && (str[2]<='9') &&
					(str[3]>='0') && (str[3]<='9'))
						{ return(t+(str[2]-'0')*10+(str[3]-'0')); }
			}
			break;
		}

		t=BGBCC_SH_REG_R0;

		if(!str[2])
		{
			if((str[1]>='0') && (str[1]<='9'))
				{ return(t+(str[1]-'0')); }
			break;
		}

		if(!str[3])
		{
			if((str[1]>='0') && (str[1]<='9') &&
				(str[2]>='0') && (str[2]<='9'))
					{ return(t+(str[1]-'0')*10+(str[2]-'0')); }
		}
		break;

	case 'M':	case 'm':
		if(!bgbcc_stricmp(str, "mach"))
			return(BGBCC_SH_REG_MACH);
		if(!bgbcc_stricmp(str, "macl"))
			return(BGBCC_SH_REG_MACL);
		break;
	case 'P':	case 'p':
		if(!bgbcc_stricmp(str, "pr"))
			return(BGBCC_SH_REG_PR);
		if(!bgbcc_stricmp(str, "pc"))
			return(BGBCC_SH_REG_PC);
		break;

	case 'X':	case 'x':
		if((str[1]=='F') || (str[1]=='f') ||
			(str[1]=='D') || (str[1]=='d'))
		{
			t=BGBCC_SH_REG_FR0; isdr=0;
			if((str[1]=='D') || (str[1]=='d'))
				{ t=BGBCC_SH_REG_DR0; isdr=1; }
			if(str[2]=='0')
			{
				if(!str[3])
					return(t^1);

				if((str[3]>='0') && (str[3]<='9') && !str[4])
				{
					t1=t+(str[3]-'0')^1;
					return(t1);
				}
			}else if(str[2]=='1')
			{
				if(!str[3])
					return((t+1)^1);
				if((str[3]>='0') && (str[3]<='5') && !str[4])
				{
					t1=t+10+(str[3]-'0')^1;
					return(t1);
				}
			}else if((str[2]>='0') && (str[2]<='9') && !str[3])
			{
				t1=t+(str[2]-'0')^1;
				return(t1);
			}
		}
		break;

	default:
		if(!bgbcc_stricmp(str, "sp"))
			return(BGBCC_SH_REG_SP);

		if(!bgbcc_stricmp(str, "sr"))
			return(BGBCC_SH_REG_SR);
		if(!bgbcc_stricmp(str, "gbr"))
			return(BGBCC_SH_REG_GBR);
		if(!bgbcc_stricmp(str, "vbr"))
			return(BGBCC_SH_REG_VBR);
//		if(!bgbcc_stricmp(str, "tbr"))
//			return(BGBCC_SH_REG_TBR);
		break;
	}

	return(-1);
}

int BGBCC_SHXA_ParseOperand(char **rcs, BGBCC_SHX_OpcodeArg *opv)
{
	char *tk0, *tk1;
	char *cs, *cs1, *cs2;
	int i;

	cs=*rcs;
	cs=BGBCC_SHXA_EatWhiteNoLinebreak(cs);
	if((*cs=='\r') || (*cs=='\n'))
	{
		cs=BGBCC_SHXA_EatWhite(cs);
		*rcs=cs;
		return(0);
	}

	if((*cs==';') || (*cs=='|'))
	{
		cs++;
		cs=BGBCC_SHXA_EatWhite(cs);
		*rcs=cs;
		return(0);
	}

	if(*cs=='#')
	{
		cs++;
		
		if((*cs=='-') || ((cs[0]==' ') && (cs[1]=='-')))
		{
			if(*cs==' ')cs++;
			cs++;
			cs1=BGBCC_SHXA_ParseTokenAlt(cs, &tk0);

			opv->ty=BGBCC_SH_OPVTY_IMM;
			opv->disp=-bgbcc_atoi(tk0+1);
			*rcs=cs1;
			return(1);
		}
		
		cs1=BGBCC_SHXA_ParseTokenAlt(cs, &tk0);

		opv->ty=BGBCC_SH_OPVTY_IMM;
		opv->disp=bgbcc_atoi(tk0+1);
		*rcs=cs1;
		return(1);
	}

	if(*cs=='@')
	{
		cs++;
		while(*cs==' ')cs++;

		if(*cs=='-')
		{
			cs++;

			cs1=BGBCC_SHXA_ParseToken(cs, &tk0);

			if(*tk0=='I')
			{
				opv->ty=BGBCC_SH_OPVTY_DRREGD;
				opv->breg=BGBCC_SHXA_GetRegId(tk0+1);
				*rcs=cs1;
				return(1);
			}
			return(-1);
		}

		if(*cs=='(')
		{
			cs++;
			cs=BGBCC_SHXA_ParseToken(cs, &tk0);
			tk1=NULL;
			cs=BGBCC_SHXA_EatWhiteNoLinebreak(cs);
			if(*cs==',')
			{
				cs++;
				cs=BGBCC_SHXA_ParseToken(cs, &tk1);
			}
			cs=BGBCC_SHXA_EatWhiteNoLinebreak(cs);
			if(*cs==')')
				cs++;

			if(tk1)
			{
				if((*tk0=='|') && (*tk1=='I'))
				{
					opv->ty=BGBCC_SH_OPVTY_RDMEM;
					opv->breg=BGBCC_SHXA_GetRegId(tk1+1);
					opv->disp=bgbcc_atoi(tk0+1);
					*rcs=cs;
					return(1);
				}

				if(*tk1=='I')
				{
					opv->ty=BGBCC_SH_OPVTY_RRMEM;
//					opv->rb=BGBCC_SHXA_GetRegId(tk0+1);
//					opv->ri=BGBCC_SHXA_GetRegId(tk1+1);

					opv->ireg=BGBCC_SHXA_GetRegId(tk0+1);
					opv->breg=BGBCC_SHXA_GetRegId(tk1+1);
					*rcs=cs;
					return(1);
				}
				if(*tk1=='|')
				{
					opv->ty=BGBCC_SH_OPVTY_RDMEM;
					opv->breg=BGBCC_SHXA_GetRegId(tk0+1);
					opv->disp=bgbcc_atoi(tk1+1);
					*rcs=cs;
					return(1);
				}
				return(-1);
			}

			opv->ty=BGBCC_SH_OPVTY_DRREG;
			opv->breg=BGBCC_SHXA_GetRegId(tk0+1);
			if(*cs=='+')
			{
				cs++;
				if(*cs==')')
					cs++;
				opv->ty=BGBCC_SH_OPVTY_DRREGI;
			}
			*rcs=cs;
			return(1);
		}

		cs1=BGBCC_SHXA_ParseToken(cs, &tk0);

		if(*tk0=='I')
		{
			opv->ty=BGBCC_SH_OPVTY_DRREG;
			opv->breg=BGBCC_SHXA_GetRegId(tk0+1);
			if(*cs1=='+')
			{
				cs1++;
				opv->ty=BGBCC_SH_OPVTY_DRREGI;
			}
			*rcs=cs1;
			return(1);
		}

		opv->ty=BGBCC_SH_OPVTY_IMM;
		opv->disp=bgbcc_atoi(tk0+1);
		*rcs=cs1;
		return(1);
	}

	cs1=BGBCC_SHXA_ParseTokenAlt(cs, &tk0);
	cs2=BGBCC_SHXA_ParseToken(cs1, &tk1);

	if(*tk0=='|')
	{
		opv->ty=BGBCC_SH_OPVTY_IMM;
		opv->disp=bgbcc_atoi(tk0+1);
		*rcs=cs1;
		return(1);
	}

	if(*tk0=='I')
	{
		i=BGBCC_SHXA_GetRegId(tk0+1);
		if(i>=0)
		{
			opv->ty=BGBCC_SH_OPVTY_REG;
			opv->breg=i;
			*rcs=cs1;
			return(1);
		}

		opv->ty=BGBCC_SH_OPVTY_NAME;
		if(strlen(tk0+1)<8)
		{
			opv->name=(char *)(&opv->disp);
			strncpy(opv->name, tk0+1, 7);
		}else
		{
			opv->name=bgbcc_strdup(tk0+1);
		}
		*rcs=cs1;
		return(1);
	}

	return(0);
}

struct {
char *name;
int nmid;
}bgbcc_shxa_nmidtab[]={
//{"",		BGBCC_SH_NMID_UNK},
{"mov",		BGBCC_SH_NMID_MOV},
{"mov.b",	BGBCC_SH_NMID_MOVB},
{"mov.w",	BGBCC_SH_NMID_MOVW},
{"mov.l",	BGBCC_SH_NMID_MOVL},
{"mov.q",	BGBCC_SH_NMID_MOVQ},
{"add",		BGBCC_SH_NMID_ADD},
{"addc",	BGBCC_SH_NMID_ADDC},
{"addv",	BGBCC_SH_NMID_ADDV},
{"sub",		BGBCC_SH_NMID_SUB},
{"subc",	BGBCC_SH_NMID_SUBC},
{"subv",	BGBCC_SH_NMID_SUBV},
{"cmp/hs",	BGBCC_SH_NMID_CMPHS},
{"cmp/hi",	BGBCC_SH_NMID_CMPHI},
{"cmp/eq",	BGBCC_SH_NMID_CMPEQ},
{"cmp/ge",	BGBCC_SH_NMID_CMPGE},
{"cmp/gt",	BGBCC_SH_NMID_CMPGT},
{"cmpq/hs",	BGBCC_SH_NMID_CMPQHS},
{"cmpq/hi",	BGBCC_SH_NMID_CMPQHI},
{"cmpq/eq",	BGBCC_SH_NMID_CMPQEQ},
{"cmpq/ge",	BGBCC_SH_NMID_CMPQGE},
{"cmpq/gt",	BGBCC_SH_NMID_CMPQGT},
{"jmp",		BGBCC_SH_NMID_JMP},
{"jsr",		BGBCC_SH_NMID_JSR},
{"bra",		BGBCC_SH_NMID_BRA},
{"bsr",		BGBCC_SH_NMID_BSR},
{"bt",		BGBCC_SH_NMID_BT},
{"bf",		BGBCC_SH_NMID_BF},
{"bt/s",	BGBCC_SH_NMID_BTS},
{"bf/s",	BGBCC_SH_NMID_BFS},
{"div1",	BGBCC_SH_NMID_DIV1},
{"dmulu",	BGBCC_SH_NMID_DMULU},
{"dmuls",	BGBCC_SH_NMID_DMULS},
{"dmulu.l",	BGBCC_SH_NMID_DMULU},
{"dmuls.l",	BGBCC_SH_NMID_DMULS},
{"tst",		BGBCC_SH_NMID_TST},
{"and",		BGBCC_SH_NMID_AND},
{"xor",		BGBCC_SH_NMID_XOR},
{"or",		BGBCC_SH_NMID_OR},
{"div0s",	BGBCC_SH_NMID_DIV0S},
{"cmp/str",	BGBCC_SH_NMID_CMPSTR},
{"mulu.w",	BGBCC_SH_NMID_MULUW},
{"muls.w",	BGBCC_SH_NMID_MULSW},
{"mac.l",	BGBCC_SH_NMID_MACL},
{"bsrf",	BGBCC_SH_NMID_BSRF},
{"braf",	BGBCC_SH_NMID_BRAF},
{"mul.l",	BGBCC_SH_NMID_MULL},
{"clrt",	BGBCC_SH_NMID_CLRT},
{"sett",	BGBCC_SH_NMID_SETT},
{"clrmac",	BGBCC_SH_NMID_CLRMAC},
{"nop",		BGBCC_SH_NMID_NOP},
{"movt",	BGBCC_SH_NMID_MOVT},
{"rts",		BGBCC_SH_NMID_RTS},
{"sleep",	BGBCC_SH_NMID_SLEEP},
{"rte",		BGBCC_SH_NMID_RTE},
{"shll",	BGBCC_SH_NMID_SHLL},
{"dt",		BGBCC_SH_NMID_DT},
{"shal",	BGBCC_SH_NMID_SHAL},
{"shlr",	BGBCC_SH_NMID_SHLR},
{"cmp/pz",	BGBCC_SH_NMID_CMPPZ},
{"shar",	BGBCC_SH_NMID_SHAR},
{"rotl",	BGBCC_SH_NMID_ROTL},
{"rotcl",	BGBCC_SH_NMID_ROTCL},
{"rott",	BGBCC_SH_NMID_ROTT},
{"rotcr",	BGBCC_SH_NMID_ROTCR},
{"shll2",	BGBCC_SH_NMID_SHLL2},
{"rotr",	BGBCC_SH_NMID_ROTR},
{"cmp/pl",	BGBCC_SH_NMID_CMPPL},
{"shll8",	BGBCC_SH_NMID_SHLL8},
{"shll16",	BGBCC_SH_NMID_SHLL16},
{"shlr2",	BGBCC_SH_NMID_SHLR2},
{"shlr8",	BGBCC_SH_NMID_SHLR8},
{"shlr16",	BGBCC_SH_NMID_SHLR16},
{"shll4",	BGBCC_SH_NMID_SHLL4},
{"shlr4",	BGBCC_SH_NMID_SHLR4},
{"shll32",	BGBCC_SH_NMID_SHLL32},
{"shlr32",	BGBCC_SH_NMID_SHLR32},
{"mac.w",	BGBCC_SH_NMID_MACW},
{"not",		BGBCC_SH_NMID_NOT},
{"swap.b",	BGBCC_SH_NMID_SWAPB},
{"swap.w",	BGBCC_SH_NMID_SWAPW},
{"negc",	BGBCC_SH_NMID_NEGC},
{"neg",		BGBCC_SH_NMID_NEG},
{"extu.b",	BGBCC_SH_NMID_EXTUB},
{"extu.w",	BGBCC_SH_NMID_EXTUW},
{"extu.l",	BGBCC_SH_NMID_EXTUL},
{"exts.b",	BGBCC_SH_NMID_EXTSB},
{"exts.w",	BGBCC_SH_NMID_EXTSW},
{"exts.l",	BGBCC_SH_NMID_EXTSL},
{"trapa",	BGBCC_SH_NMID_TRAPA},
{"mova",	BGBCC_SH_NMID_MOVA},
{"tst.b",	BGBCC_SH_NMID_TSTB},
{"div0u",	BGBCC_SH_NMID_DIV0U},
{"ldc",		BGBCC_SH_NMID_LDC},
{"ldc.l",	BGBCC_SH_NMID_LDCL},
{"lds",		BGBCC_SH_NMID_LDS},
{"lds.l",	BGBCC_SH_NMID_LDSL},
{"stc",		BGBCC_SH_NMID_STC},
{"stc.l",	BGBCC_SH_NMID_STCL},
{"sts",		BGBCC_SH_NMID_STS},
{"sts.l",	BGBCC_SH_NMID_STSL},
{"cas.l",	BGBCC_SH_NMID_CASL},
{"tas.b",	BGBCC_SH_NMID_TASB},
{"xtrct",	BGBCC_SH_NMID_XTRCT},
{"and.b",	BGBCC_SH_NMID_ANDB},
{"xor.b",	BGBCC_SH_NMID_XORB},
{"or.b",	BGBCC_SH_NMID_ORB},
{"shad",	BGBCC_SH_NMID_SHAD},
{"shld",	BGBCC_SH_NMID_SHLD},
{"clrs",	BGBCC_SH_NMID_CLRS},
{"sets",	BGBCC_SH_NMID_SETS},
{"nott",	BGBCC_SH_NMID_NOTT},
{"ldtlb",	BGBCC_SH_NMID_LDTLB},
{"brk",		BGBCC_SH_NMID_BRK},
{"movrt",	BGBCC_SH_NMID_MOVRT},
{"movca.l",	BGBCC_SH_NMID_MOVCAL},
{"lea.b",	BGBCC_SH_NMID_LEAB},
{"lea.w",	BGBCC_SH_NMID_LEAW},
{"lea.l",	BGBCC_SH_NMID_LEAL},
{"lea.q",	BGBCC_SH_NMID_LEAQ},
{"shad.q",	BGBCC_SH_NMID_SHADQ},
{"shld.q",	BGBCC_SH_NMID_SHLDQ},

{"push",	BGBCC_SH_NMID_PUSH},
{"pop",		BGBCC_SH_NMID_POP},

{"fabs",	BGBCC_SH_NMID_FABS},
{"fadd",	BGBCC_SH_NMID_FADD},
{"fcmp/eq",	BGBCC_SH_NMID_FCMPEQ},
{"fcmp/gt",	BGBCC_SH_NMID_FCMPGT},
{"fcnvds",	BGBCC_SH_NMID_FCNVDS},
{"fcnvsd",	BGBCC_SH_NMID_FCNVSD},
{"fdiv",	BGBCC_SH_NMID_FDIV},
{"fldi0",	BGBCC_SH_NMID_FLDI0},
{"fldi1",	BGBCC_SH_NMID_FLDI1},
{"flds",	BGBCC_SH_NMID_FLDS},
{"float",	BGBCC_SH_NMID_FLOAT},
{"fmac",	BGBCC_SH_NMID_FMAC},
{"fmov",	BGBCC_SH_NMID_FMOV},
{"fmov.s",	BGBCC_SH_NMID_FMOVS},
{"fmov.d",	BGBCC_SH_NMID_FMOVD},
{"fmul",	BGBCC_SH_NMID_FMUL},
{"fneg",	BGBCC_SH_NMID_FNEG},
{"fschg",	BGBCC_SH_NMID_FSCHG},
{"fsqrt",	BGBCC_SH_NMID_FSQRT},
{"fsts",	BGBCC_SH_NMID_FSTS},
{"fsub",	BGBCC_SH_NMID_FSUB},
{"ftrc",	BGBCC_SH_NMID_FTRC},
{"fsrra",	BGBCC_SH_NMID_FSRRA},
{"pref",	BGBCC_SH_NMID_PREF},
{"ocbi",	BGBCC_SH_NMID_OCBI},
{"ocbp",	BGBCC_SH_NMID_OCBP},
{"ocbwb",	BGBCC_SH_NMID_OCBWB},
{"icbi",	BGBCC_SH_NMID_ICBI},

{"iclrmd.dq",	BGBCC_SH_NMID_ICLRMD_DQ},
{"isetmd.dq",	BGBCC_SH_NMID_ISETMD_DQ},

{NULL, 0}
};

int bgbcc_shxa_nmidtab_chn[256];
int bgbcc_shxa_nmidtab_hash[256];
int bgbcc_shxa_init=0;

int BGBCC_SHXA_Init()
{
	int i, j, k;
	
	if(bgbcc_shxa_init)
		return(0);
	bgbcc_shxa_init=1;
	
	for(i=0; i<256; i++)
		bgbcc_shxa_nmidtab_hash[i]=-1;
	
	for(i=0; bgbcc_shxa_nmidtab[i].name; i++)
	{
		j=BGBCC_CCXL_HashNameCase(bgbcc_shxa_nmidtab[i].name);
		j&=255;
		
		bgbcc_shxa_nmidtab_chn[i]=bgbcc_shxa_nmidtab_hash[j];
		bgbcc_shxa_nmidtab_hash[j]=i;
		
//		if(!bgbcc_stricmp(bgbcc_shxa_nmidtab[i].name, name))
//			return(bgbcc_shxa_nmidtab[i].nmid);
	}
	return(-1);
}

int BGBCC_SHXA_LookupOpcodeNmid(char *name)
{
	int h;
	int i;

	h=BGBCC_CCXL_HashNameCase(name);
	h&=255;

	i=bgbcc_shxa_nmidtab_hash[h];
	while(i>=0)
	{
		if(!bgbcc_stricmp(bgbcc_shxa_nmidtab[i].name, name))
			return(bgbcc_shxa_nmidtab[i].nmid);
		i=bgbcc_shxa_nmidtab_chn[i];
	}
	return(-1);

//	for(i=0; bgbcc_shxa_nmidtab[i].name; i++)
//		if(!bgbcc_stricmp(bgbcc_shxa_nmidtab[i].name, name))
//			return(bgbcc_shxa_nmidtab[i].nmid);
//	return(-1);
}

int BGBCC_SHXA_LookupOpcodeFmid(
	BGBCC_SHX_OpcodeArg *arg0,
	BGBCC_SHX_OpcodeArg *arg1,
	BGBCC_SHX_OpcodeArg *arg2)
{
	int fm;

	if(!arg0->ty)
		return(BGBCC_SH_FMID_NONE);

	if(!arg1->ty)
	{
		if(arg0->ty==BGBCC_SH_OPVTY_IMM)
			return(BGBCC_SH_FMID_IMM);
		if(arg0->ty==BGBCC_SH_OPVTY_REG)
			return(BGBCC_SH_FMID_REGRN);
		if(arg0->ty==BGBCC_SH_OPVTY_NAME)
			return(BGBCC_SH_FMID_LABEL);

		if(arg0->ty==BGBCC_SH_OPVTY_DRREG)
			return(BGBCC_SH_FMID_LDREG);

//		if(arg0->ty==BGBCC_SH_OPVTY_DRREGI)
//			return(BGBCC_SH_FMID_LDREG);

		return(0);
	}

	if(!arg2->ty)
	{
		fm=0;
		switch(arg0->ty)
		{
		case BGBCC_SH_OPVTY_REG:
			switch(arg1->ty)
			{
			case BGBCC_SH_OPVTY_REG:
				fm=BGBCC_SH_FMID_REGREG; break;
			case BGBCC_SH_OPVTY_DRREG:
				fm=BGBCC_SH_FMID_REGST; break;
			case BGBCC_SH_OPVTY_DRREGD:
				fm=BGBCC_SH_FMID_REGDECST; break;
			case BGBCC_SH_OPVTY_RRMEM:
				fm=BGBCC_SH_FMID_REGSTR0N; break;
			case BGBCC_SH_OPVTY_RDMEM:
				fm=BGBCC_SH_FMID_REGSTDISP; break;
			default: fm=0; break;
			}
			break;
		case BGBCC_SH_OPVTY_IMM:
			switch(arg1->ty)
			{
			case BGBCC_SH_OPVTY_REG:
				fm=BGBCC_SH_FMID_IMMREG; break;
			case BGBCC_SH_OPVTY_RRMEM:
				fm=BGBCC_SH_FMID_IMMSTRMN; break;
			default: fm=0; break;
			}
			break;
		case BGBCC_SH_OPVTY_DRREG:
			switch(arg1->ty)
			{
			case BGBCC_SH_OPVTY_REG:
				fm=BGBCC_SH_FMID_REGLD; break;
			default: fm=0; break;
			}
			break;
		case BGBCC_SH_OPVTY_DRREGI:
			switch(arg1->ty)
			{
			case BGBCC_SH_OPVTY_REG:
				fm=BGBCC_SH_FMID_REGINCLD; break;
			default: fm=0; break;
			}
			break;
		case BGBCC_SH_OPVTY_RRMEM:
			switch(arg1->ty)
			{
			case BGBCC_SH_OPVTY_REG:
				fm=BGBCC_SH_FMID_REGLDR0M; break;
			default: fm=0; break;
			}
			break;
		case BGBCC_SH_OPVTY_RDMEM:
			switch(arg1->ty)
			{
			case BGBCC_SH_OPVTY_REG:
				fm=BGBCC_SH_FMID_REGLDDISP; break;
			default: fm=0; break;
			}
			break;
		case BGBCC_SH_OPVTY_NAME:
			switch(arg1->ty)
			{
			case BGBCC_SH_OPVTY_REG:
				fm=BGBCC_SH_FMID_LBLREG; break;
			default: fm=0; break;
			}
			break;
		default:
			fm=0; break;
		}
		
		return(fm);
	}

	if(1)
	{
		fm=0;
		switch(arg0->ty)
		{
		case BGBCC_SH_OPVTY_REG:
			switch(arg1->ty)
			{
			case BGBCC_SH_OPVTY_REG:
				switch(arg2->ty)
				{
				case BGBCC_SH_OPVTY_REG:
					fm=BGBCC_SH_FMID_REGREGREG; break;
				}
				break;
			case BGBCC_SH_OPVTY_IMM:
				switch(arg2->ty)
				{
				case BGBCC_SH_OPVTY_REG:
					fm=BGBCC_SH_FMID_REGIMMREG; break;
				}
				break;
			default: fm=0; break;
			}
			break;
		}
	}

	return(0);
}

int BGBCC_SHXA_TryAssembleOpcode(
	BGBCC_SHX_Context *ctx,
	char *name,
	BGBCC_SHX_OpcodeArg *arg0,
	BGBCC_SHX_OpcodeArg *arg1,
	BGBCC_SHX_OpcodeArg *arg2)
{
	int nmid, fmid, rt, lbl;
	
	if(name[0]=='.')
		return(0);
	
	nmid=BGBCC_SHXA_LookupOpcodeNmid(name);
	if(nmid<=0)
		return(0);

	if(nmid==BGBCC_SH_NMID_ISETMD_DQ)
	{
		BGBCC_SHXC_SetModeDqSet(NULL, ctx);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_ICLRMD_DQ)
	{
		BGBCC_SHXC_SetModeDqClear(NULL, ctx);
		return(1);
	}

	fmid=BGBCC_SHXA_LookupOpcodeFmid(arg0, arg1, arg2);
	if(fmid<=0)
		return(0);

	if(ctx->iflvl_f>0)
		return(1);

	switch(fmid)
	{
	case BGBCC_SH_FMID_REGREG:
		rt=BGBCC_SHX_TryEmitOpRegReg(ctx,
			nmid, arg0->breg, arg1->breg);
		break;
	case BGBCC_SH_FMID_IMMREG:
		rt=BGBCC_SHX_TryEmitOpRegImm(ctx,
			nmid, arg1->breg, arg0->disp);
		break;
	case BGBCC_SH_FMID_REGST:
		rt=BGBCC_SHX_TryEmitOpRegStReg(ctx,
			nmid, arg0->breg, arg1->breg);
		break;
	case BGBCC_SH_FMID_REGLD:
		rt=BGBCC_SHX_TryEmitOpLdRegReg(ctx,
			nmid, arg0->breg, arg1->breg);
		break;
	case BGBCC_SH_FMID_REGDECST:
		rt=BGBCC_SHX_TryEmitOpRegStDecReg(ctx,
			nmid, arg0->breg, arg1->breg);
		break;
	case BGBCC_SH_FMID_REGINCLD:
		rt=BGBCC_SHX_TryEmitOpLdIncRegReg(ctx,
			nmid, arg0->breg, arg1->breg);
		break;
	case BGBCC_SH_FMID_REGSTR0N:
		rt=BGBCC_SHX_TryEmitOpRegStReg2(ctx,
			nmid, arg0->breg, arg1->ireg, arg1->breg);
		break;
	case BGBCC_SH_FMID_REGLDR0M:
		rt=BGBCC_SHX_TryEmitOpLdReg2Reg(ctx,
			nmid, arg0->breg, arg0->ireg, arg1->breg);
		break;
	case BGBCC_SH_FMID_REGSTDISP:
		rt=BGBCC_SHX_TryEmitOpRegStRegDisp(ctx,
			nmid, arg0->breg, arg1->breg, arg1->disp);
		break;
	case BGBCC_SH_FMID_REGLDDISP:
		rt=BGBCC_SHX_TryEmitOpLdRegDispReg(ctx,
			nmid, arg0->breg, arg0->disp, arg1->breg);
		break;
	
	case BGBCC_SH_FMID_NONE:
		rt=BGBCC_SHX_TryEmitOpNone(ctx, nmid);
		break;
	
	case BGBCC_SH_FMID_REGRM:
	case BGBCC_SH_FMID_REGRN:
		rt=BGBCC_SHX_TryEmitOpReg(ctx, nmid, arg0->breg);
		break;
	case BGBCC_SH_FMID_IMM:
		rt=BGBCC_SHX_TryEmitOpImm(ctx, nmid, arg0->disp);
		break;
	case BGBCC_SH_FMID_LDREG:
	case BGBCC_SH_FMID_STREG:
		rt=BGBCC_SHX_TryEmitOpMReg(ctx, nmid, arg0->breg);
		break;

	case BGBCC_SH_FMID_LABEL:
		lbl=BGBCC_SHX_GetNamedLabel(ctx, arg0->name);
		rt=BGBCC_SHX_TryEmitOpLabel(ctx, nmid, lbl);
		break;
	case BGBCC_SH_FMID_LBLREG:
		lbl=BGBCC_SHX_GetNamedLabel(ctx, arg0->name);
		rt=BGBCC_SHX_TryEmitOpLblReg(ctx, nmid, lbl, arg1->breg);
		break;

	default:
		__debugbreak();
		rt=0;
		break;
	}
	return(rt);
}

int BGBCC_SHXA_ParseCheckFeature(BGBCC_SHX_Context *ctx, char *sym)
{
	if(!bgbcc_stricmp(sym, "bjx1_egpr"))
		return(ctx->has_bjx1egpr);
	if(!bgbcc_stricmp(sym, "bjx1_betav"))
		return(ctx->is_betav);
	if(!bgbcc_stricmp(sym, "bjx1_64"))
		return(ctx->is_addr64);

	if(!bgbcc_stricmp(sym, "bjx1_nofpu"))
		return(ctx->no_fpu);
	if(!bgbcc_stricmp(sym, "bjx1_noext32"))
		return(ctx->no_ext32);

	return(0);
}

int BGBCC_SHXA_ParseOpcode(BGBCC_SHX_Context *ctx, char **rcs)
{
	BGBCC_SHX_OpcodeArg arg[4];
//	BTSHAS_ListingOp *op;
	char *tk0, *tk1;
	char *cs, *cs1, *cs2;
	s64 li;
	f32 ff;
	f64 fd;
	int i, j, k;

	cs=*rcs;
	cs1=BGBCC_SHXA_ParseTokenAlt(cs, &tk0);

	if(*tk0=='I')
	{
		cs1=BGBCC_SHXA_EatWhiteOnlyNoLinebreak(cs1);
		if(*cs1==':')
		{
			if(ctx->iflvl_f>0)
			{
				*rcs=cs1+1;
				return(1);
			}
			
			BGBCC_SHX_EmitNamedLabel(ctx, tk0+1);
			BGBCC_SHXC_ResetModeDqUnknown(NULL, ctx);
			*rcs=cs1+1;
			return(1);
		}

		cs2=cs1;
		arg[0].ty=0; arg[1].ty=0; arg[2].ty=0;
		for(i=0; i<3; i++)
		{
			if(BGBCC_SHXA_ParseOperand(&cs2, &arg[i])<=0)
				break;
			cs2=BGBCC_SHXA_EatWhiteNoLinebreak(cs2);
			if(*cs2==',')
				cs2++;
		}
//		if(BGBCC_SHXA_ParseOperand(&cs2, arg[0])>0)
//			if(BGBCC_SHXA_ParseOperand(&cs2, arg[1])>0)
//				if(BGBCC_SHXA_ParseOperand(&cs2, arg[2])>0)
//		{
//		}

#if 0
		op=BTSHAS_SHxOpLookup(tk0+1, &arg[0], &arg[1], &arg[2]);
		if(op)
		{
			BGBCC_SHX_EmitListOp(ctx, op, &arg[0], &arg[1], &arg[2]);
			*rcs=cs2;
			return(1);
		}
#endif

		k=BGBCC_SHXA_TryAssembleOpcode(ctx, tk0+1, &arg[0], &arg[1], &arg[2]);
		if(k>0)
		{
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.align"))
		{
			cs2=cs1;
			cs2=BGBCC_SHXA_ParseToken(cs2, &tk0);
			BGBCC_SHX_EmitBAlign(ctx, bgbcc_atoi(tk0+1)*2);
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.balign"))
		{
			cs2=cs1;
			cs2=BGBCC_SHXA_ParseToken(cs2, &tk0);
			BGBCC_SHX_EmitBAlign(ctx, bgbcc_atoi(tk0+1));
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.rept"))
		{
			cs2=cs1;
			cs2=BGBCC_SHXA_ParseToken(cs2, &tk0);
			ctx->cnrept=bgbcc_atoi(tk0+1);
			ctx->csrept=cs2;
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.endr"))
		{
			cs2=cs1;
			ctx->cnrept--;
			if(ctx->cnrept>0)
				cs2=ctx->csrept;
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.long") ||
			!strcmp(tk0, "I.int"))
		{
			cs2=cs1;
			cs2=BGBCC_SHXA_ParseTokenAlt(cs2, &tk0);
			cs2=BGBCC_SHXA_EatWhiteNoLinebreak(cs2);

			if(*tk0=='I')
			{
				if(*cs2=='@')
				{
					while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
						cs2++;
				}

				k=0;
				if(*cs2=='-')
				{
					cs2++;
					cs2=BGBCC_SHXA_ParseTokenAlt(cs2, &tk1);
					if(tk1[0]=='I')
					{
						BGBCC_SHX_EmitNamedReloc(ctx, tk0+1,
							BGBCC_SH_RLC_REL32);
						BGBCC_SHX_EmitNamedReloc(ctx, tk1+1,
							BGBCC_SH_RLC_REL32B);
					}else if(tk1[0]=='|')
					{
						BGBCC_SHX_EmitNamedReloc(ctx, tk0+1,
							BGBCC_SH_RLC_ABS32);
						k=-bgbcc_atoi(tk1+1);
					}
				}else if(*cs2=='+')
				{
					cs2++;
					cs2=BGBCC_SHXA_ParseTokenAlt(cs2, &tk1);
					if(tk1[0]=='I')
					{
						BGBCC_SHX_EmitNamedReloc(ctx, tk0+1,
							BGBCC_SH_RLC_REL32);
						BGBCC_SHX_EmitNamedReloc(ctx, tk1+1,
							BGBCC_SH_RLC_REL32);
					}else if(tk1[0]=='|')
					{
						BGBCC_SHX_EmitNamedReloc(ctx, tk0+1,
							BGBCC_SH_RLC_ABS32);
						k=bgbcc_atoi(tk1+1);
					}
				}else
				{
					BGBCC_SHX_EmitNamedReloc(ctx, tk0+1,
						BGBCC_SH_RLC_ABS32);
				}

				BGBCC_SHX_EmitDWord(ctx, k);
//				*rcs=cs2;
//				return(1);
			}else if((*tk0=='|') || (*tk0=='C'))

			{
	//			BGBCC_SHX_EmitDWord(ctx, bgbcc_atoi(tk0+1));

				if(*tk0=='|')
					i=bgbcc_atoi(tk0+1);
				if(*tk0=='C')
					i=(ctx->is_le)?
						(tk0[1]|(tk0[2]<<8)|(tk0[3]<<16)|(tk0[4]<<24)):
						(tk0[4]|(tk0[3]<<8)|(tk0[2]<<16)|(tk0[1]<<24));
				BGBCC_SHX_EmitDWord(ctx, i);
			}

			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_SHXA_ParseTokenAlt(cs2, &tk0);
//				BGBCC_SHX_EmitDWord(ctx, bgbcc_atoi(tk0+1));

				if(*tk0=='I')
				{
					BGBCC_SHX_EmitNamedReloc(ctx, tk0+1,
						BGBCC_SH_RLC_ABS32);
					BGBCC_SHX_EmitDWord(ctx, 0);
					continue;
				}

				if(*tk0=='|')
					i=bgbcc_atoi(tk0+1);
				if(*tk0=='C')
					i=(ctx->is_le)?
						(tk0[1]|(tk0[2]<<8)|(tk0[3]<<16)|(tk0[4]<<24)):
						(tk0[4]|(tk0[3]<<8)|(tk0[2]<<16)|(tk0[1]<<24));
				BGBCC_SHX_EmitDWord(ctx, i);
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.word") ||
			!strcmp(tk0, "I.short") ||
			!strcmp(tk0, "I.hword"))
		{
			cs2=cs1;
			cs2=BGBCC_SHXA_ParseTokenAlt(cs2, &tk0);
//			BGBCC_SHX_EmitWord(ctx, bgbcc_atoi(tk0+1));
			if(*tk0=='|')
				i=bgbcc_atoi(tk0+1);
			if(*tk0=='C')
				i=(ctx->is_le)?(tk0[1]|(tk0[2]<<8)):(tk0[2]|(tk0[1]<<8));
			BGBCC_SHX_EmitWord(ctx, i);

			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_SHXA_ParseTokenAlt(cs2, &tk0);
//				BGBCC_SHX_EmitWord(ctx, bgbcc_atoi(tk0+1));

				if(*tk0=='|')
					i=bgbcc_atoi(tk0+1);
				if(*tk0=='C')
					i=(ctx->is_le)?(tk0[1]|(tk0[2]<<8)):(tk0[2]|(tk0[1]<<8));
				BGBCC_SHX_EmitWord(ctx, i);
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.byte"))
		{
			cs2=cs1;
			cs2=BGBCC_SHXA_ParseToken(cs2, &tk0);
			if(*tk0=='|')
				i=bgbcc_atoi(tk0+1);
			if(*tk0=='C')
				i=tk0[1];
			BGBCC_SHX_EmitByte(ctx, i);
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_SHXA_ParseToken(cs2, &tk0);
				if(*tk0=='|')
					i=bgbcc_atoi(tk0+1);
				if(*tk0=='C')
					i=tk0[1];
				BGBCC_SHX_EmitByte(ctx, i);
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.quad"))
		{
			cs2=cs1;
			cs2=BGBCC_SHXA_ParseTokenAlt(cs2, &tk0);
			BGBCC_SHX_EmitQWord(ctx, bgbcc_atoi(tk0+1));
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_SHXA_ParseTokenAlt(cs2, &tk0);
				BGBCC_SHX_EmitQWord(ctx, bgbcc_atoi(tk0+1));
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.float") || !strcmp(tk0, "I.single"))
		{
			cs2=cs1;
			cs2=BGBCC_SHXA_ParseTokenAlt(cs2, &tk0);
			ff=bgbcc_atof(tk0+1);
			BGBCC_SHX_EmitDWord(ctx, *(u32 *)(&ff));
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_SHXA_ParseTokenAlt(cs2, &tk0);
				ff=bgbcc_atof(tk0+1);
				BGBCC_SHX_EmitDWord(ctx, *(u32 *)(&ff));
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.double"))
		{
			cs2=cs1;
			cs2=BGBCC_SHXA_ParseTokenAlt(cs2, &tk0);
			fd=bgbcc_atof(tk0+1);
			BGBCC_SHX_EmitQWord(ctx, *(u64 *)(&fd));
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_SHXA_ParseTokenAlt(cs2, &tk0);
				fd=bgbcc_atof(tk0+1);
				BGBCC_SHX_EmitQWord(ctx, *(u64 *)(&fd));
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.string"))
		{
			cs2=cs1;
			cs2=BGBCC_SHXA_ParseToken(cs2, &tk0);
			BGBCC_SHX_EmitString(ctx, tk0+1);
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_SHXA_ParseToken(cs2, &tk0);
				BGBCC_SHX_EmitString(ctx, tk0+1);
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.section"))
		{
			cs2=cs1;
			cs2=BGBCC_SHXA_ParseTokenAlt(cs2, &tk0);
			BGBCC_SHX_SetSectionName(ctx, tk0+1);

			if(*cs2==',')
			{
				while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
					cs2++;
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.text") ||
			!strcmp(tk0, "I.data") ||
			!strcmp(tk0, "I.bss"))
		{
			BGBCC_SHX_SetSectionName(ctx, tk0+1);
			*rcs=cs1;
			return(1);
		}

		if(!strcmp(tk0, "I.global") ||
			!strcmp(tk0, "I.globl") ||
			!strcmp(tk0, "I.extern") ||
			!strcmp(tk0, "I.weak"))
		{
			cs2=cs1;
			cs2=BGBCC_SHXA_ParseTokenAlt(cs2, &tk0);
			BGBCC_SHX_EmitNamedGlobal(ctx, tk0+1);
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.local"))
		{
			cs2=cs1;
			cs2=BGBCC_SHXA_ParseTokenAlt(cs2, &tk0);
			BGBCC_SHX_EmitNamedGlobal(ctx, tk0+1);
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.rel32"))
		{
			cs2=cs1;
			cs2=BGBCC_SHXA_ParseTokenAlt(cs2, &tk0);

			BGBCC_SHX_EmitNamedReloc(ctx, tk0+1,
				BGBCC_SH_RLC_REL32);

			if(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_SHXA_ParseTokenAlt(cs2, &tk0);
				BGBCC_SHX_EmitNamedReloc(ctx, tk0+1,
					BGBCC_SH_RLC_REL32B);
			}

			BGBCC_SHX_EmitDWord(ctx, 0);
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.comm"))
		{
			cs2=cs1;
			cs2=BGBCC_SHXA_ParseTokenAlt(cs2, &tk0);

			j=1; k=1;
			if(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_SHXA_ParseTokenAlt(cs2, &tk1);
				j=atoi(tk1+1);
			}
			if(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_SHXA_ParseTokenAlt(cs2, &tk1);
				k=atoi(tk1+1);
			}

			j=j*k;

			BGBCC_SHX_EmitNamedCommSym(ctx, tk0+1, j);
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.skip") || !strcmp(tk0, "I.space"))
		{
			j=1; k=0;

			cs2=cs1;
			cs2=BGBCC_SHXA_ParseTokenAlt(cs2, &tk0);
			j=bgbcc_atoi(tk0+1);

			if(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_SHXA_ParseTokenAlt(cs2, &tk1);
				k=bgbcc_atoi(tk1+1);
			}
			
			while((j--)>0)
				BGBCC_SHX_EmitByte(ctx, k);

			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.type") ||
			!strcmp(tk0, "I.size") ||
			!strcmp(tk0, "I.file") ||
			!strcmp(tk0, "I.end") ||
			!strcmp(tk0, "I.ident"))
		{
			cs2=cs1;
			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.endif"))
		{
			if(ctx->iflvl_f>0)
			{
				ctx->iflvl_f--;
			}else if(ctx->iflvl_t>0)
			{
				ctx->iflvl_t--;
			}else
			{
				printf("unexpected .endif '%s'\n", tk0+1);
			}

			cs2=cs1;
			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.else"))
		{
			if(ctx->iflvl_f>0)
			{
				if(ctx->iflvl_f==1)
				{
					ctx->iflvl_f--;
					ctx->iflvl_t++;
				}
			}else if(ctx->iflvl_t>0)
			{
				ctx->iflvl_t--;
				ctx->iflvl_f++;
			}else
			{
				printf("unexpected .else '%s'\n", tk0+1);
			}

			cs2=cs1;
			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.ifarch"))
		{
			cs2=cs1;
			cs2=BGBCC_SHXA_ParseTokenAlt(cs2, &tk0);

			i=BGBCC_SHXA_ParseCheckFeature(ctx, tk0+1);
			if(i>0)
				{ ctx->iflvl_t++; }
			else
				{ ctx->iflvl_f++; }

	//		cs2=cs1;
			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.ifnarch"))
		{
			cs2=cs1;
			cs2=BGBCC_SHXA_ParseTokenAlt(cs2, &tk0);

			i=BGBCC_SHXA_ParseCheckFeature(ctx, tk0+1);
			if(i>0)
				{ ctx->iflvl_f++; }
			else
				{ ctx->iflvl_t++; }

	//		cs2=cs1;
			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}

		printf("unexpected token '%s'\n", tk0+1);
		*rcs=cs;
		return(-1);
	}

	printf("unexpected token '%s'\n", tk0+1);
	return(-1);
}

int BGBCC_SHXA_ParseBuffer(BGBCC_SHX_Context *ctx, char **rcs)
{
	char tb[256];
	char *cs, *t;
	int i;

	BGBCC_SHXA_Init();

	cs=*rcs;
	while(*cs)
	{
		cs=BGBCC_SHXA_EatWhite(cs);
		if(!(*cs))
			break;
		i=BGBCC_SHXA_ParseOpcode(ctx, (char **)(&cs));
		if(i<0)
		{
			t=tb;
			while(*cs && (*cs!='\r') && (*cs!='\n'))
				*t++=*cs++;
			*t++=0;
			printf("skip line \"%s\"\n", tb);
		}
	}

	*rcs=cs;
	return(0);
}

int BGBCC_SHXC_AssembleBuffer(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	char *text)
{
	char *cs;

	sctx->iflvl_f=0;
	sctx->iflvl_t=0;

	sctx->is_rawasm=1;
	cs=text;
	BGBCC_SHXA_ParseBuffer(sctx, &cs);
	sctx->is_rawasm=0;
	return(0);
}
