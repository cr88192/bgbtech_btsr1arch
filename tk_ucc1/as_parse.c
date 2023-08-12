char *TKUCC_AsmJx2_EatWhite(char *cs)
{
	return(TKUCC_TokEatWhite(cs));
}

char *TKUCC_AsmJx2_EatWhiteNoLinebreak(char *cs)
{
	return(TKUCC_TokEatWhiteNlb(cs));
}

char *TKUCC_AsmJx2_EatWhiteOnlyNoLinebreak(char *cs)
{
	return(TKUCC_TokEatWhiteOnlyNlb(cs));
}

char *TKUCC_AsmJx2_ParseTokenBasic(char *cs, char *tb)
{
	char *cs1;
	int ty;

	cs1=TKUCC_TokReadToken(cs, tb+1, &ty);

	switch(ty)
	{
	case TKUCC_TOK_NUMBER:
		tb[0]='|'; break;
	case TKUCC_TOK_NAME:
		tb[0]='I'; break;
	case TKUCC_TOK_STRING:
		tb[0]='S'; break;
	case TKUCC_TOK_CHARSTRING:
		tb[0]='C'; break;
	case TKUCC_TOK_OPERATOR:
		tb[0]='X'; break;
	default:
		tb[0]='X'; break;
	}

	return(cs1);
}

char *TKUCC_AsmJx2_ParseTokenBasicAltn(char *s, char *tb)
{
	char *t, *te;
	int i, j, k;

	t=tb; te=tb+252;

	s=TKUCC_AsmJx2_EatWhite(s);
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

	s=TKUCC_AsmJx2_ParseTokenBasic(s, tb);
	return(s);
}

char *TKUCC_AsmJx2_ParseToken(char *cs, char **rstr)
{
	char tb[256];
	cs=TKUCC_AsmJx2_ParseTokenBasic(cs, tb);
	*rstr=tkucc_rstrdup(tb);
	return(cs);
}

char *TKUCC_AsmJx2_ParseTokenAlt(char *cs, char **rstr)
{
	char tb[256];
	cs=TKUCC_AsmJx2_ParseTokenBasicAltn(cs, tb);
	*rstr=tkucc_rstrdup(tb);
	return(cs);
}

int TKUCC_AsmJx2_GetRegId(TKUCC_MainContext *ctx, char *str)
{
	int i, t, t1, isdr;

	switch(str[0])
	{
	case 'A':	case 'a':
		if(ctx->asm_modefl&TKUCC_ASMMODE_RVREGS)
		{
			t=TKUCC_REG_R10;

			if(!str[2])
			{
				if((str[1]>='0') && (str[1]<='7'))
					{ return(t+(str[1]-'0')); }
				break;
			}
			
			if(!stricmp(str, "ar"))
				return(TKUCC_REG_R10);
			if(!stricmp(str, "ar0"))
				return(TKUCC_REG_R10);
			if(!stricmp(str, "ar1"))
				return(TKUCC_REG_R11);
			
		}else
		{
			i=-1;
			if(!str[2])
			{
				if((str[1]>='0') && (str[1]<='9'))
					{ i=(str[1]-'0'); }
			}else
			{
				if((str[1]>='0') && (str[1]<='9'))
					{ i=(str[1]-'0'); }
				if((str[2]>='0') && (str[2]<='9'))
					{ i=(i*10)+(str[1]-'0'); }
			}

			if((i>=0) && (i<4))
				return(TKUCC_REG_R4+(i-0));
			if((i>=4) && (i<8))
				return(TKUCC_REG_R20+(i-4));
			if((i>=8) && (i<12))
				return(TKUCC_REG_R36+(i-8));
			if((i>=12) && (i<16))
				return(TKUCC_REG_R52+(i-12));

			if(!stricmp(str, "ar"))
				return(TKUCC_REG_R2);
			if(!stricmp(str, "ar0"))
				return(TKUCC_REG_R2);
			if(!stricmp(str, "ar1"))
				return(TKUCC_REG_R3);
		}
		break;

	case 'F':	case 'f':
	case 'D':	case 'd':
#if 0
		if((str[1]=='R') || (str[1]=='r'))
		{
			t=TKUCC_REG_FR0; isdr=0;
			if((str[0]=='D') || (str[0]=='d'))
				{ t=TKUCC_REG_DR0; isdr=1; }
			if(str[2]=='0')
			{
				if(!str[3])
					return(t);

				if((str[3]>='0') && (str[3]<='9') && !str[4])
				{
					t1=t+(str[3]-'0');
					return(t1);
				}
			}else if(str[2]=='1')
			{
				if(!str[3])
					return(t+1);
				if((str[3]>='0') && (str[3]<='5') && !str[4])
				{
					t1=t+10+(str[3]-'0');
					return(t1);
				}
			}else if((str[2]>='0') && (str[2]<='9') && !str[3])
			{
				t1=t+(str[2]-'0');
				return(t1);
			}
		}
#endif

//		if(!tkucc_stricmp(str, "fpscr"))
//			return(TKUCC_REG_FPSCR);
//		if(!tkucc_stricmp(str, "fpul"))
//			return(TKUCC_REG_FPUL);

		if(!tkucc_stricmp(str, "dlr"))
			return(TKUCC_REG_DLR);
		if(!tkucc_stricmp(str, "dhr"))
			return(TKUCC_REG_DHR);
		break;

	case 'M':	case 'm':
//		if(!tkucc_stricmp(str, "mach"))
//			return(TKUCC_REG_MACH);
//		if(!tkucc_stricmp(str, "macl"))
//			return(TKUCC_REG_MACL);
		if(!tkucc_stricmp(str, "mmcr"))
			return(TKUCC_REG_MMCR);
		break;
	case 'P':	case 'p':
		if(!tkucc_stricmp(str, "pr"))
			return(TKUCC_REG_LR);
		if(!tkucc_stricmp(str, "pc"))
			return(TKUCC_REG_PC);
		if(!tkucc_stricmp(str, "pch"))
			return(TKUCC_REG_PCH);
		break;

	case 'R':	case 'r':
		if((str[1]=='D') || (str[1]=='d') ||
			(str[1]=='Q') || (str[1]=='q'))
		{
			t=TKUCC_REG_R0;
//			if((str[1]=='D') || (str[1]=='d'))
//				t=TKUCC_REG_RD0;
//			if((str[1]=='Q') || (str[1]=='q'))
//				t=TKUCC_REG_RQ0;

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
				{
					i=((str[2]-'0')*10)+(str[3]-'0');
					
					if((i>=0) && (i<=31))
						return(t+i);
					if((i>=32) && (i<=63))
						return(t+i);
				}
			}
			break;
		}

		t=TKUCC_REG_R0;

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
			{
				i=((str[1]-'0')*10)+(str[2]-'0');
				
				if((i>=0) && (i<=31))
					return(TKUCC_REG_R0+i);
				if((i>=32) && (i<=63))
					return(TKUCC_REG_R0+i);
			}
		}

		if(!tkucc_stricmp(str, "r0u"))
			return(TKUCC_REG_R0U);
		if(!tkucc_stricmp(str, "r1u"))
			return(TKUCC_REG_R1U);
		if(!tkucc_stricmp(str, "r15u"))
			return(TKUCC_REG_R15U);

		break;


	case 'X':	case 'x':
		if(ctx->asm_modefl&TKUCC_ASMMODE_RVREGS)
		{
			t=TKUCC_REG_R0;

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
				{
					i=((str[1]-'0')*10)+(str[2]-'0');
					
					if((i>=0) && (i<=31))
						return(TKUCC_REG_R0+i);
					if((i>=32) && (i<=63))
						return(TKUCC_REG_R32+(i-32));
				}
			}
		}

#if 0
		if((str[1]=='F') || (str[1]=='f') ||
			(str[1]=='D') || (str[1]=='d'))
		{
			t=TKUCC_REG_FR0; isdr=0;
			if((str[1]=='D') || (str[1]=='d'))
				{ t=TKUCC_REG_DR0; isdr=1; }
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
#endif

		t=TKUCC_REG_LR0;

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
			{
				i=(str[1]-'0')*10+(str[2]-'0');
				if(i>=64)
					break;
				if((i>=32) && (i&1))
					break;

				if(i>=32)
					i=(i&0x1E)|1;
				return(t+i);
			}
		}
		break;

	case 'S':	case 's':
		if(ctx->asm_modefl&TKUCC_ASMMODE_RVREGS)
		{
			i=-1;

			if((str[1]>='0') && (str[1]<='9'))
				i=str[1]-0;
			if((str[1]>='0') && (str[1]<='9') &&
				(str[2]>='0') && (str[2]<='9'))
			{
				i=(str[1]-'0')*10+(str[2]-'0');
			}
			
			if((i>=0) && (i<=11))
			{
				if(i<2)
					return(TKUCC_REG_R8+i);
				return(TKUCC_REG_R18+(i-2));
			}
		}else
		{
			i=-1;

			if((str[1]>='0') && (str[1]<='9'))
				i=str[1]-0;
			if((str[1]>='0') && (str[1]<='9') &&
				(str[2]>='0') && (str[2]<='9'))
			{
				i=(str[1]-'0')*10+(str[2]-'0');
			}
			
			if((i>=0) && (i<=11))
			{
				if(i<6)
					return(TKUCC_REG_R8+i);
				return(TKUCC_REG_R24+(i-6));
			}
		}

		if(!tkucc_stricmp(str, "sp"))
			return(TKUCC_REG_SP);

		if(!tkucc_stricmp(str, "sr"))
			return(TKUCC_REG_SR);

		if(!tkucc_stricmp(str, "ssp"))
			return(TKUCC_REG_SSP);
//		if(!tkucc_stricmp(str, "ssr"))
//			return(TKUCC_REG_SSR);
		if(!tkucc_stricmp(str, "spc"))
			return(TKUCC_REG_SPC);

		if(!tkucc_stricmp(str, "sttb"))
			return(TKUCC_REG_STTB);

		break;

	case 'T':	case 't':
		if(ctx->asm_modefl&TKUCC_ASMMODE_RVREGS)
		{
			i=-1;

			if((str[1]>='0') && (str[1]<='9'))
				i=str[1]-0;
			if((str[1]>='0') && (str[1]<='9') &&
				(str[2]>='0') && (str[2]<='9'))
			{
				i=(str[1]-'0')*10+(str[2]-'0');
			}
			
			if((i>=0) && (i<=6))
			{
				if(i<3)
					return(TKUCC_REG_R5+i);
				return(TKUCC_REG_R28+(i-3));
			}

			if(!tkucc_stricmp(str, "tp"))
				return(TKUCC_REG_R4);
		}else
		{
			i=-1;

			if((str[1]>='0') && (str[1]<='9'))
				i=str[1]-0;
			if((str[1]>='0') && (str[1]<='9') &&
				(str[2]>='0') && (str[2]<='9'))
			{
				i=(str[1]-'0')*10+(str[2]-'0');
			}
			
			if((i>=0) && (i<=6))
			{
				if(i<3)
					return(TKUCC_REG_R1+i);
				return(TKUCC_REG_R16+(i-3));
			}
		}

		if(!tkucc_stricmp(str, "tbr"))
			return(TKUCC_REG_TBR);

		if(!tkucc_stricmp(str, "ttb"))
			return(TKUCC_REG_TTB);
		if(!tkucc_stricmp(str, "tea"))
			return(TKUCC_REG_TEA);
		if(!tkucc_stricmp(str, "teah"))
			return(TKUCC_REG_TEAH);

		break;


	default:
		if(ctx->asm_modefl&TKUCC_ASMMODE_RVREGS)
		{
			if(!tkucc_stricmp(str, "zero"))
				return(TKUCC_REG_R0);

			if(!tkucc_stricmp(str, "ra"))
				return(TKUCC_REG_R1);

			if(!tkucc_stricmp(str, "gp"))
				return(TKUCC_REG_R3);
		}

		if(!tkucc_stricmp(str, "gbr"))
			return(TKUCC_REG_GBR);
		if(!tkucc_stricmp(str, "vbr"))
			return(TKUCC_REG_VBR);

		if(!tkucc_stricmp(str, "lr"))
			return(TKUCC_REG_LR);

		if(!tkucc_stricmp(str, "exsr"))
			return(TKUCC_REG_EXSR);

		if(!tkucc_stricmp(str, "krr"))
			return(TKUCC_REG_KRR);

		if(!tkucc_stricmp(str, "isrsave"))
			return(TKUCC_REG_ISRSAVE);

		if(!tkucc_stricmp(str, "gbh"))
			return(TKUCC_REG_GBH);
		if(!tkucc_stricmp(str, "pch"))
			return(TKUCC_REG_PCH);

		if(!tkucc_stricmp(str, "vipt"))
			return(TKUCC_REG_VIPT);

		break;
	}

	return(-1);
}

static char *bgbcc_jx2a_lastlbl;

int TKUCC_AsmJx2_ParseOperand_OffsetOf(TKUCC_MainContext *ctx,
	char *objn, char *fldn)
{
#if 0
	BGBCC_CCXL_LiteralInfo *st;
	BGBCC_CCXL_RegisterInfo *fi;
	ccxl_register sreg, dreg;
	ccxl_type bty;
	int fn;
	int i;

	st=BGBCC_CCXL_LookupStructure(ctx->tctx, objn);
	if(!st)
	{
		BGBCC_DBGBREAK
		return(0);
	}
	
	fn=BGBCC_CCXL_LookupStructFieldID(ctx->tctx, st, fldn);
	fi=st->decl->fields[fn];

	if(fi->fxmoffs==fi->fxnoffs)
	{
		return(fi->fxmoffs);
	}
	
	BGBCC_DBGBREAK
#endif

	return(0);
}

int TKUCC_AsmJx2_ParseOperand(
	TKUCC_MainContext *ctx, char **rcs,
	TKUCC_AsmOpArg *opv)
{
	char tb0[256];
	char *tk0, *tk1, *tk2, *tk3;
	char *cs, *cs1, *cs2;
	int i, scale;

	memset(opv, 0, sizeof(TKUCC_AsmOpArg));

	cs=*rcs;
	cs=TKUCC_AsmJx2_EatWhiteNoLinebreak(cs);
	if((*cs=='\r') || (*cs=='\n'))
	{
		cs=TKUCC_AsmJx2_EatWhite(cs);
		*rcs=cs;
		return(0);
	}

	if(*cs=='|')
	{
		*rcs=cs;
		return(0);
	}

//	if((*cs==';') || (*cs=='|'))
	if(*cs==';')
	{
		cs++;
		cs=TKUCC_AsmJx2_EatWhite(cs);
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
			cs1=TKUCC_AsmJx2_ParseTokenAlt(cs, &tk0);

			opv->aty=TKUCC_ARGTY_IMM;
			opv->imm=-tkucc_atoll(tk0+1);
			*rcs=cs1;
			return(1);
		}
		
		cs1=TKUCC_AsmJx2_ParseTokenAlt(cs, &tk0);

		opv->aty=TKUCC_ARGTY_IMM;
		opv->imm=tkucc_atoll(tk0+1);
		*rcs=cs1;
		return(1);
	}

	if((*cs=='@') || (*cs=='('))
	{
		if(*cs=='@')
			cs++;
		while(*cs==' ')cs++;

#if 0
		if(*cs=='-')
		{
			cs++;

			cs1=TKUCC_AsmJx2_ParseToken(cs, &tk0);

			if(*tk0=='I')
			{
				opv->aty=TKUCC_ARGTY_MEM_RD;
				opv->rb=TKUCC_AsmJx2_GetRegId(ctx, tk0+1);
				*rcs=cs1;
				return(1);
			}
			return(-1);
		}
#endif

		if(*cs=='(')
		{
			cs++;
			cs=TKUCC_AsmJx2_ParseToken(cs, &tk0);
			tk1=NULL;
			tk2=NULL;
			tk3=NULL;
			scale=0;

			cs=TKUCC_AsmJx2_EatWhiteNoLinebreak(cs);
			if(*cs=='*')
			{
				cs++;
				cs=TKUCC_AsmJx2_ParseTokenAlt(cs, &cs2);
				scale=atoi(cs2+1);
			}

			cs=TKUCC_AsmJx2_EatWhiteNoLinebreak(cs);
			if(*cs==',')
			{
				cs++;
				cs=TKUCC_AsmJx2_ParseToken(cs, &tk1);
			}

			if(tk1 && !strcmp(tk1, "Ioffsetof"))
			{
				cs=TKUCC_AsmJx2_ParseTokenAlt(cs, &tk2);
				cs=TKUCC_AsmJx2_ParseTokenAlt(cs, &tk3);
			}
			
			cs=TKUCC_AsmJx2_EatWhiteNoLinebreak(cs);
			if(*cs=='*')
			{
				cs++;
				cs=TKUCC_AsmJx2_ParseTokenAlt(cs, &cs2);
				scale=atoi(cs2+1);
			}

			cs=TKUCC_AsmJx2_EatWhiteNoLinebreak(cs);
			if(*cs==')')
				cs++;

			if(tk1)
			{
				if((*tk0=='|') && (*tk1=='I'))
				{
					opv->aty=TKUCC_ARGTY_MEM_RD;
					opv->rb=TKUCC_AsmJx2_GetRegId(ctx, tk1+1);
					opv->imm=tkucc_atoll(tk0+1);
					if(scale>0)
						opv->imm*=scale;
					*rcs=cs;
					return(1);
				}
				
				if(*tk1=='I')
				{
					if(!strcmp(tk1, "Ioffsetof"))
					{
						opv->aty=TKUCC_ARGTY_MEM_RD;
						opv->rb=TKUCC_AsmJx2_GetRegId(ctx, tk0+1);
						opv->imm=TKUCC_AsmJx2_ParseOperand_OffsetOf(
							ctx, tk2+1, tk3+1);
						*rcs=cs;
						return(1);
					}

					opv->aty=TKUCC_ARGTY_MEM_RI;
					opv->rb=TKUCC_AsmJx2_GetRegId(ctx, tk0+1);
					opv->ri=TKUCC_AsmJx2_GetRegId(ctx, tk1+1);
					opv->sc=scale;

					*rcs=cs;
					return(1);
				}
				if(*tk1=='|')
				{
					opv->aty=TKUCC_ARGTY_MEM_RD;
					opv->rb=TKUCC_AsmJx2_GetRegId(ctx, tk0+1);
					opv->imm=tkucc_atoll(tk1+1);
					if(scale>0)
						opv->imm*=scale;
					*rcs=cs;
					return(1);
				}
				return(-1);
			}

			opv->aty=TKUCC_ARGTY_MEM_R;
			opv->rb=TKUCC_AsmJx2_GetRegId(ctx, tk0+1);

#if 0
			if(*cs=='+')
			{
				cs++;
				if(*cs==')')
					cs++;
				opv->aty=TKUCC_ARGTY_MEM_RI;
			}
#endif

			*rcs=cs;
			return(1);
		}

		cs1=TKUCC_AsmJx2_ParseToken(cs, &tk0);

		if(*tk0=='I')
		{
			opv->aty=TKUCC_ARGTY_MEM_R;
			opv->rb=TKUCC_AsmJx2_GetRegId(ctx, tk0+1);
			if(*cs1=='+')
			{
				cs1++;
				opv->aty=TKUCC_ARGTY_MEM_RI;
			}
			*rcs=cs1;
			return(1);
		}

		opv->aty=TKUCC_ARGTY_IMM;
		opv->imm=tkucc_atoll(tk0+1);
		*rcs=cs1;
		return(1);
	}

	cs1=TKUCC_AsmJx2_ParseTokenAlt(cs, &tk0);
	cs2=TKUCC_AsmJx2_ParseToken(cs1, &tk1);

	if(*tk0=='|')
	{
		opv->aty=TKUCC_ARGTY_IMM;
		opv->imm=tkucc_atoll(tk0+1);
		*rcs=cs1;
		return(1);
	}

	if(*tk0=='I')
	{
		i=TKUCC_AsmJx2_GetRegId(ctx, tk0+1);
		if(i>=0)
		{
			opv->aty=TKUCC_ARGTY_REG;
			opv->rb=i;
			*rcs=cs1;
			return(1);
		}

		if((tk0[1]=='.') && bgbcc_jx2a_lastlbl)
		{
			sprintf(tb0, "%s%s", bgbcc_jx2a_lastlbl, tk0+1);

			opv->aty=TKUCC_ARGTY_LABEL;
			if(strlen(tb0)<8)
			{
				opv->name=(char *)(&opv->imm);
				strncpy(opv->name, tb0, 7);
			}else
			{
				opv->name=TKUCC_StrdupInternAsm(ctx, tb0);
			}
		}else
		{
			opv->aty=TKUCC_ARGTY_LABEL;
			if(strlen(tk0+1)<8)
			{
				opv->imm=0;
				opv->name=(char *)(&opv->imm);
				strncpy(opv->name, tk0+1, 8);
			}else
			{
				opv->name=TKUCC_StrdupInternAsm(ctx, tk0+1);
			}
		}
		*rcs=cs1;
		return(1);
	}

	return(0);
}

struct {
char *name;
int nmid;
int mfl;
}bgbcc_jx2a_nmidtab[]={
{"mov",		TKUCC_NMID_MOV},
{"mov.b",	TKUCC_NMID_MOVB},
{"mov.w",	TKUCC_NMID_MOVW},
{"mov.l",	TKUCC_NMID_MOVL},
{"mov.q",	TKUCC_NMID_MOVQ},
{"mov.p",	TKUCC_NMID_MOVP},

{"movu.b",	TKUCC_NMID_MOVUB},
{"movu.w",	TKUCC_NMID_MOVUW},
{"movu.l",	TKUCC_NMID_MOVUL},

{"lea.b",	TKUCC_NMID_LEAB},
{"lea.w",	TKUCC_NMID_LEAW},
{"lea.l",	TKUCC_NMID_LEAL},
{"lea.q",	TKUCC_NMID_LEAQ},
{"lea.p",	TKUCC_NMID_LEAP},

{"xmov.b",	TKUCC_NMID_XMOVB},
{"xmov.w",	TKUCC_NMID_XMOVW},
{"xmov.l",	TKUCC_NMID_XMOVL},
{"xmov.q",	TKUCC_NMID_XMOVQ},
{"xmov.p",	TKUCC_NMID_XMOVP},

{"xmovu.b",	TKUCC_NMID_XMOVUB},
{"xmovu.w",	TKUCC_NMID_XMOVUW},
{"xmovu.l",	TKUCC_NMID_XMOVUL},
{"xmov.x",	TKUCC_NMID_XMOVX},

{"xlea.b",	TKUCC_NMID_XLEAB},
{"xlea.w",	TKUCC_NMID_XLEAW},
{"xlea.l",	TKUCC_NMID_XLEAL},
{"xlea.q",	TKUCC_NMID_XLEAQ},

{"leat.b",	TKUCC_NMID_LEATB},
{"leat.w",	TKUCC_NMID_LEATW},
{"leat.l",	TKUCC_NMID_LEATL},
{"leat.q",	TKUCC_NMID_LEATQ},

{"add",		TKUCC_NMID_ADD},
{"addc",	TKUCC_NMID_ADDC},
//{"addv",	TKUCC_NMID_ADDV},
{"sub",		TKUCC_NMID_SUB},
{"subc",	TKUCC_NMID_SUBC},
//{"subv",	TKUCC_NMID_SUBV},
{"adc",		TKUCC_NMID_ADDC},
{"sbb",		TKUCC_NMID_SUBC},
{"cmp/hs",	TKUCC_NMID_CMPHS},
{"cmp/hi",	TKUCC_NMID_CMPHI},
{"cmp/eq",	TKUCC_NMID_CMPEQ},
{"cmp/ge",	TKUCC_NMID_CMPGE},
{"cmp/gt",	TKUCC_NMID_CMPGT},
{"cmpq/hs",	TKUCC_NMID_CMPQHS},
{"cmpq/hi",	TKUCC_NMID_CMPQHI},
{"cmpq/eq",	TKUCC_NMID_CMPQEQ},
{"cmpq/ge",	TKUCC_NMID_CMPQGE},
{"cmpq/gt",	TKUCC_NMID_CMPQGT},

{"cmphs",	TKUCC_NMID_CMPHS},
{"cmphi",	TKUCC_NMID_CMPHI},
{"cmpeq",	TKUCC_NMID_CMPEQ},
{"cmpge",	TKUCC_NMID_CMPGE},
{"cmpgt",	TKUCC_NMID_CMPGT},
{"cmpqhs",	TKUCC_NMID_CMPQHS},
{"cmpqhi",	TKUCC_NMID_CMPQHI},
{"cmpqeq",	TKUCC_NMID_CMPQEQ},
{"cmpqge",	TKUCC_NMID_CMPQGE},
{"cmpqgt",	TKUCC_NMID_CMPQGT},

{"cmphs.l",	TKUCC_NMID_CMPHS},
{"cmphi.l",	TKUCC_NMID_CMPHI},
{"cmpeq.l",	TKUCC_NMID_CMPEQ},
{"cmpge.l",	TKUCC_NMID_CMPGE},
{"cmpgt.l",	TKUCC_NMID_CMPGT},
{"cmphs.q",	TKUCC_NMID_CMPQHS},
{"cmphi.q",	TKUCC_NMID_CMPQHI},
{"cmpeq.q",	TKUCC_NMID_CMPQEQ},
{"cmpge.q",	TKUCC_NMID_CMPQGE},
{"cmpgt.q",	TKUCC_NMID_CMPQGT},

{"cmppeq",	TKUCC_NMID_CMPPEQ},
{"cmppge",	TKUCC_NMID_CMPPGE},
{"cmppgt",	TKUCC_NMID_CMPPGT},

{"cmpeq.p",	TKUCC_NMID_CMPPEQ},
{"cmpge.p",	TKUCC_NMID_CMPPGE},
{"cmpgt.p",	TKUCC_NMID_CMPPGT},

{"cmtteq",	TKUCC_NMID_CMPTTEQ},
{"cmtaeq",	TKUCC_NMID_CMPTAEQ},
{"cmtahi",	TKUCC_NMID_CMPTAHI},
{"cmtahs",	TKUCC_NMID_CMPTAHS},

{"cmppeqx",	TKUCC_NMID_CMPPEQX},
{"cmppgtx",	TKUCC_NMID_CMPPGTX},

{"jmp",		TKUCC_NMID_JMP},
{"jsr",		TKUCC_NMID_JSR},
{"bra",		TKUCC_NMID_BRA},
{"bsr",		TKUCC_NMID_BSR},
{"bt",		TKUCC_NMID_BT},
{"bf",		TKUCC_NMID_BF},
{"bt/s",	TKUCC_NMID_BTS},
{"bf/s",	TKUCC_NMID_BFS},
// {"div1",	TKUCC_NMID_DIV1},

//{"bra.b",	TKUCC_NMID_BRAB},
//{"bsr.b",	TKUCC_NMID_BSRB},
//{"bt.b",	TKUCC_NMID_BTB},
//{"bf.b",	TKUCC_NMID_BFB},

//{"bra.l",	TKUCC_NMID_BRAL},
//{"bsr.l",	TKUCC_NMID_BSRL},
//{"bt.l",	TKUCC_NMID_BTL},
//{"bf.l",	TKUCC_NMID_BFL},

{"vskg",	TKUCC_NMID_VSKG},
{"vskc",	TKUCC_NMID_VSKC},
{"bndchk",	TKUCC_NMID_BNDCHK},
{"bndcmp",	TKUCC_NMID_BNDCMP},

{"bndchk.b",	TKUCC_NMID_BNDCHKB},
{"bndchk.w",	TKUCC_NMID_BNDCHKW},
{"bndchk.l",	TKUCC_NMID_BNDCHKL},
{"bndchk.q",	TKUCC_NMID_BNDCHKQ},

{"bndcmp.b",	TKUCC_NMID_BNDCMPB},
{"bndcmp.q",	TKUCC_NMID_BNDCMPW},
{"bndcmp.l",	TKUCC_NMID_BNDCMPL},
{"bndcmp.q",	TKUCC_NMID_BNDCMPQ},

{"breq",	TKUCC_NMID_BREQ},
{"brne",	TKUCC_NMID_BRNE},

{"brlt",	TKUCC_NMID_BRLT},
{"brge",	TKUCC_NMID_BRGE},
{"brgt",	TKUCC_NMID_BRGT},
{"brle",	TKUCC_NMID_BRLE},

{"brltu",	TKUCC_NMID_BRLTU},
{"brgeu",	TKUCC_NMID_BRGEU},
{"brgtu",	TKUCC_NMID_BRGTU},
{"brleu",	TKUCC_NMID_BRLEU},

{"brz",		TKUCC_NMID_BREQ},
{"brnz",	TKUCC_NMID_BRNE},

{"breq.q",	TKUCC_NMID_BREQ},
{"brne.q",	TKUCC_NMID_BRNE},

{"brlt.q",	TKUCC_NMID_BRLT},
{"brge.q",	TKUCC_NMID_BRGE},
{"brgt.q",	TKUCC_NMID_BRGT},
{"brle.q",	TKUCC_NMID_BRLE},

{"breq.l",	TKUCC_NMID_BREQL},
{"brne.l",	TKUCC_NMID_BRNEL},

{"brlt.l",	TKUCC_NMID_BRLTL},
{"brge.l",	TKUCC_NMID_BRGEL},
{"brgt.l",	TKUCC_NMID_BRGTL},
{"brle.l",	TKUCC_NMID_BRLEL},

{"divs.q",	TKUCC_NMID_DIVSQ},
{"divu.q",	TKUCC_NMID_DIVUQ},
{"mods.q",	TKUCC_NMID_MODSQ},
{"modu.q",	TKUCC_NMID_MODUQ},

{"divs.l",	TKUCC_NMID_DIVSL},
{"divu.l",	TKUCC_NMID_DIVUL},
{"mods.l",	TKUCC_NMID_MODSL},
{"modu.l",	TKUCC_NMID_MODUL},

{"bcdadc",	TKUCC_NMID_BCDADC},
{"bcdsbb",	TKUCC_NMID_BCDSBB},

//{"bcdadd",	TKUCC_NMID_BCDADD},
//{"bcdsub",	TKUCC_NMID_BCDSUB},
//{"bcdaddx",	TKUCC_NMID_BCDADDX},
//{"bcdsubx",	TKUCC_NMID_BCDSUBX},

{"dmulu",	TKUCC_NMID_MULUL},
{"dmuls",	TKUCC_NMID_MULSL},
{"dmulu.l",	TKUCC_NMID_MULUL},
{"dmuls.l",	TKUCC_NMID_MULSL},
{"mulu",	TKUCC_NMID_DMULU},
{"muls",	TKUCC_NMID_DMULS},
{"mulu.l",	TKUCC_NMID_DMULU},
{"muls.l",	TKUCC_NMID_DMULS},

{"mulu.w",	TKUCC_NMID_MULUW},
{"muls.w",	TKUCC_NMID_MULSW},
{"dmulu.w",	TKUCC_NMID_MULUW},
{"dmuls.w",	TKUCC_NMID_MULSW},

{"tst",		TKUCC_NMID_TST},
{"test",	TKUCC_NMID_TST},
{"tstq",	TKUCC_NMID_TSTQ},
{"test.q",	TKUCC_NMID_TSTQ},
{"and",		TKUCC_NMID_AND},
{"xor",		TKUCC_NMID_XOR},
{"or",		TKUCC_NMID_OR},
// {"div0s",	TKUCC_NMID_DIV0S},
// {"cmp/str",	TKUCC_NMID_CMPSTR},
// {"mac.l",	TKUCC_NMID_MACL},
{"bsrf",	TKUCC_NMID_BSRF},
{"braf",	TKUCC_NMID_BRAF},
{"mul.l",	TKUCC_NMID_MULSL},
{"clrt",	TKUCC_NMID_CLRT},
{"sett",	TKUCC_NMID_SETT},
// {"clrmac",	TKUCC_NMID_CLRMAC},
{"nop",		TKUCC_NMID_NOP},
{"movt",	TKUCC_NMID_MOVT},
{"rts",		TKUCC_NMID_RTS},
{"sleep",	TKUCC_NMID_SLEEP},
{"break",	TKUCC_NMID_BRK},
{"rte",		TKUCC_NMID_RTE},
// {"shll",	TKUCC_NMID_SHLL},
// {"dt",		TKUCC_NMID_DT},
// {"shal",	TKUCC_NMID_SHAL},
{"shlr",	TKUCC_NMID_SHLR},
//{"cmp/pz",	TKUCC_NMID_CMPPZ},
{"shar",	TKUCC_NMID_SHAR},
{"rotl",	TKUCC_NMID_ROTL},
{"rotcl",	TKUCC_NMID_ROTCL},
// {"rott",	TKUCC_NMID_ROTT},
{"rotcr",	TKUCC_NMID_ROTCR},
{"rotr",	TKUCC_NMID_ROTR},

{"rotcl.l",	TKUCC_NMID_ROTCL},
{"rotcr.l",	TKUCC_NMID_ROTCR},
//{"rotcl.q",	TKUCC_NMID_ROTCLQ},
//{"rotcr.q",	TKUCC_NMID_ROTCRQ},

//{"cmp/pl",	TKUCC_NMID_CMPPL},

#if 0
{"shll1",	TKUCC_NMID_SHLL1},
{"shlr1",	TKUCC_NMID_SHLR1},
{"shar1",	TKUCC_NMID_SHAR1},
{"shll2",	TKUCC_NMID_SHLL2},
{"shlr2",	TKUCC_NMID_SHLR2},
{"shar2",	TKUCC_NMID_SHAR2},
{"shll4",	TKUCC_NMID_SHLL4},
{"shlr4",	TKUCC_NMID_SHLR4},
{"shar4",	TKUCC_NMID_SHAR4},
{"shll8",	TKUCC_NMID_SHLL8},
{"shlr8",	TKUCC_NMID_SHLR8},
{"shar8",	TKUCC_NMID_SHAR8},
{"shll16",	TKUCC_NMID_SHLL16},
{"shlr16",	TKUCC_NMID_SHLR16},
{"shar16",	TKUCC_NMID_SHAR16},
{"shll32",	TKUCC_NMID_SHLL32},
{"shlr32",	TKUCC_NMID_SHLR32},
{"shar32",	TKUCC_NMID_SHAR32},
#endif

{"movnt",	TKUCC_NMID_MOVNT},
{"cselt",	TKUCC_NMID_CSELT},
{"clz",		TKUCC_NMID_CLZ},
{"clzq",	TKUCC_NMID_CLZQ},
{"ctz",		TKUCC_NMID_CTZ},
{"ctzq",	TKUCC_NMID_CTZQ},
{"btrns",	TKUCC_NMID_BTRNS},
{"btrnsq",	TKUCC_NMID_BTRNSQ},

{"pmortl",	TKUCC_NMID_PMORTL},
{"pmortq",	TKUCC_NMID_PMORTQ},
{"pmort.l",	TKUCC_NMID_PMORTL},
{"pmort.q",	TKUCC_NMID_PMORTQ},

// {"mac.w",	TKUCC_NMID_MACW},
{"not",		TKUCC_NMID_NOT},
//{"swap.b",	TKUCC_NMID_SWAPB},
//{"swap.w",	TKUCC_NMID_SWAPW},
{"negc",	TKUCC_NMID_NEGC},
{"neg",		TKUCC_NMID_NEG},
{"extu.b",	TKUCC_NMID_EXTUB},
{"extu.w",	TKUCC_NMID_EXTUW},
{"extu.l",	TKUCC_NMID_EXTUL},
{"exts.b",	TKUCC_NMID_EXTSB},
{"exts.w",	TKUCC_NMID_EXTSW},
{"exts.l",	TKUCC_NMID_EXTSL},
{"trapa",	TKUCC_NMID_TRAPA},
// {"mova",	TKUCC_NMID_MOVA},
// {"tst.b",	TKUCC_NMID_TSTB},
// {"div0u",	TKUCC_NMID_DIV0U},
// {"div0",	TKUCC_NMID_DIV0U},
{"ldc",		TKUCC_NMID_LDC},
//{"ldc.l",	TKUCC_NMID_LDCL},
{"lds",		TKUCC_NMID_LDS},
//{"lds.l",	TKUCC_NMID_LDSL},
{"stc",		TKUCC_NMID_STC},
//{"stc.l",	TKUCC_NMID_STCL},
{"sts",		TKUCC_NMID_STS},
//{"sts.l",	TKUCC_NMID_STSL},
//{"cas.l",	TKUCC_NMID_CASL},
//{"tas.b",	TKUCC_NMID_TASB},
//{"xtrct",	TKUCC_NMID_XTRCT},

#if 0
{"and.b",	TKUCC_NMID_ANDB},
{"xor.b",	TKUCC_NMID_XORB},
{"or.b",	TKUCC_NMID_ORB},
{"and.w",	TKUCC_NMID_ANDW},
{"xor.w",	TKUCC_NMID_XORW},
{"or.w",	TKUCC_NMID_ORW},
{"and.l",	TKUCC_NMID_ANDL},
{"xor.l",	TKUCC_NMID_XORL},
{"or.l",	TKUCC_NMID_ORL},
{"and.q",	TKUCC_NMID_AND},
{"xor.q",	TKUCC_NMID_XOR},
{"or.q",	TKUCC_NMID_OR},
#endif

#if 0
{"add.b",	TKUCC_NMID_ADDSB},
{"adds.b",	TKUCC_NMID_ADDSB},
{"addu.b",	TKUCC_NMID_ADDUB},
{"sub.b",	TKUCC_NMID_SUBSB},
{"subs.b",	TKUCC_NMID_SUBSB},
{"subu.b",	TKUCC_NMID_SUBUB},
{"rsub.b",	TKUCC_NMID_RSUBSB},
{"rsubs.b",	TKUCC_NMID_RSUBSB},
{"rsubu.b",	TKUCC_NMID_RSUBUB},

{"add.w",	TKUCC_NMID_ADDSW},
{"adds.w",	TKUCC_NMID_ADDSW},
{"addu.w",	TKUCC_NMID_ADDUW},
{"sub.w",	TKUCC_NMID_SUBSW},
{"subs.w",	TKUCC_NMID_SUBSW},
{"subu.w",	TKUCC_NMID_SUBUW},
{"rsub.w",	TKUCC_NMID_RSUBSW},
{"rsubs.w",	TKUCC_NMID_RSUBSW},
{"rsubu.w",	TKUCC_NMID_RSUBUW},

{"add.l",	TKUCC_NMID_ADDSL},
{"adds.l",	TKUCC_NMID_ADDSL},
{"addu.l",	TKUCC_NMID_ADDUL},
{"sub.l",	TKUCC_NMID_SUBSL},
{"subs.l",	TKUCC_NMID_SUBSL},
{"subu.l",	TKUCC_NMID_SUBUL},
{"rsub.l",	TKUCC_NMID_RSUBSL},
{"rsubs.l",	TKUCC_NMID_RSUBSL},
{"rsubu.l",	TKUCC_NMID_RSUBUL},
#endif

{"add.q",	TKUCC_NMID_ADD},
{"sub.q",	TKUCC_NMID_SUB},
{"rsub.q",	TKUCC_NMID_RSUB},

{"sub.p",	TKUCC_NMID_SUBP},
{"subx.p",	TKUCC_NMID_SUBXP},

{"shad",	TKUCC_NMID_SHAD},
{"shld",	TKUCC_NMID_SHLD},
{"clrs",	TKUCC_NMID_CLRS},
{"sets",	TKUCC_NMID_SETS},
{"nott",	TKUCC_NMID_NOTT},
{"brk",		TKUCC_NMID_BRK},
// {"movrt",	TKUCC_NMID_MOVRT},
// {"movca.l",	TKUCC_NMID_MOVCAL},

{"shad.l",	TKUCC_NMID_SHAD},
{"shld.l",	TKUCC_NMID_SHLD},
{"shad.q",	TKUCC_NMID_SHADQ},
{"shld.q",	TKUCC_NMID_SHLDQ},

// {"push",	TKUCC_NMID_PUSH},
// {"pop",		TKUCC_NMID_POP},
{"bra2b",	TKUCC_NMID_BRA2B},
{"bra4b",	TKUCC_NMID_BRA4B},
{"bra8b",	TKUCC_NMID_BRA8B},
// {"ret",		TKUCC_NMID_RET},
{"rtsu",	TKUCC_NMID_RTSU},
{"syscall",	TKUCC_NMID_SYSCALL},

// {"push.x",	TKUCC_NMID_PUSHX2},
// {"pop.x",	TKUCC_NMID_POPX2},
{"mov.x",	TKUCC_NMID_MOVX},
{"movx",	TKUCC_NMID_XMOV},

{"movc",	TKUCC_NMID_MOVC},
{"mov.c",	TKUCC_NMID_MOVC},

{"ldtlb",	TKUCC_NMID_LDTLB},
{"invic",	TKUCC_NMID_INVIC},
{"invdc",	TKUCC_NMID_INVDC},
{"invtlb",	TKUCC_NMID_INVTLB},
{"ldacl",	TKUCC_NMID_LDACL},

{"ldekrr",	TKUCC_NMID_LDEKRR},
{"svekrr",	TKUCC_NMID_SVEKRR},
{"ldekey",	TKUCC_NMID_LDEKEY},
{"ldeenc",	TKUCC_NMID_LDEENC},

{"wexmd",	TKUCC_NMID_WEXMD},
{"cpuid",	TKUCC_NMID_CPUID},

{"ldiz",	TKUCC_NMID_LDIZ},
{"ldin",	TKUCC_NMID_LDIN},
{"jldi",	TKUCC_NMID_JLDI},

{"ldish8",	TKUCC_NMID_LDSH8},
{"ldish16",	TKUCC_NMID_LDSH16},
{"ldsh8",	TKUCC_NMID_LDSH8},
{"ldsh16",	TKUCC_NMID_LDSH16},
{"shori",	TKUCC_NMID_LDSH16},

{"fabs",	TKUCC_NMID_FABS},
{"fadd",	TKUCC_NMID_FADD},
{"fcmp/eq",	TKUCC_NMID_FCMPEQ},
{"fcmp/gt",	TKUCC_NMID_FCMPGT},
{"fcmpeq",	TKUCC_NMID_FCMPEQ},
{"fcmpgt",	TKUCC_NMID_FCMPGT},
// {"fcnvds",	TKUCC_NMID_FCNVDS},
// {"fcnvsd",	TKUCC_NMID_FCNVSD},
{"fdiv",	TKUCC_NMID_FDIV},
{"fdivx",	TKUCC_NMID_FDIVX},
// {"fldi0",	TKUCC_NMID_FLDI0},
// {"fldi1",	TKUCC_NMID_FLDI1},
// {"flds",	TKUCC_NMID_FLDS},
// {"float",	TKUCC_NMID_FLOAT},
{"fmac",	TKUCC_NMID_FMAC},
// {"fmov",	TKUCC_NMID_FMOV},
{"fmov.s",	TKUCC_NMID_FMOVS},
{"fmov.f",	TKUCC_NMID_FMOVS},
// {"fmov.d",	TKUCC_NMID_FMOVD},
{"fmov.h",	TKUCC_NMID_FMOVH},
{"fmul",	TKUCC_NMID_FMUL},
{"fneg",	TKUCC_NMID_FNEG},
// {"fschg",	TKUCC_NMID_FSCHG},
{"fsqrt",	TKUCC_NMID_FSQRT},
// {"fsts",	TKUCC_NMID_FSTS},
{"fsub",	TKUCC_NMID_FSUB},
// {"ftrc",	TKUCC_NMID_FTRC},
// {"fsrra",	TKUCC_NMID_FSRRA},
// {"pref",	TKUCC_NMID_PREF},
// {"ocbi",	TKUCC_NMID_OCBI},
// {"ocbp",	TKUCC_NMID_OCBP},
// {"ocbwb",	TKUCC_NMID_OCBWB},
// {"icbi",	TKUCC_NMID_ICBI},

{"faddx",	TKUCC_NMID_FADDX},
{"fsubx",	TKUCC_NMID_FSUBX},
{"fmulx",	TKUCC_NMID_FMULX},
{"fmacx",	TKUCC_NMID_FMACX},
{"fcmpxeq",	TKUCC_NMID_FCMPXEQ},
{"fcmpxgt",	TKUCC_NMID_FCMPXGT},

{"faddg",	TKUCC_NMID_FADDG},
{"fsubg",	TKUCC_NMID_FSUBG},
{"fmulg",	TKUCC_NMID_FMULG},

{"fadd.g",	TKUCC_NMID_FADDG},
{"fsub.g",	TKUCC_NMID_FSUBG},
{"fmul.g",	TKUCC_NMID_FMULG},

{"cmpxeq",	TKUCC_NMID_CMPXEQ},
{"cmpxgt",	TKUCC_NMID_CMPXGT},
{"addx",	TKUCC_NMID_ADDX},
{"subx",	TKUCC_NMID_SUBX},
// {"adcx",	TKUCC_NMID_ADCX},
// {"sbbx",	TKUCC_NMID_SBBX},

{"rotlx",	TKUCC_NMID_ROTLX},
{"shadx",	TKUCC_NMID_SHADX},
{"shldx",	TKUCC_NMID_SHLDX},
{"sharx",	TKUCC_NMID_SHARX},
{"shlrx",	TKUCC_NMID_SHLRX},

{"shad.x",	TKUCC_NMID_SHADX},
{"shld.x",	TKUCC_NMID_SHLDX},
{"shar.x",	TKUCC_NMID_SHARX},
{"shlr.x",	TKUCC_NMID_SHLRX},

{"blkutx1",		TKUCC_NMID_BLKUTX1},
{"blkutx2",		TKUCC_NMID_BLKUTX2},
{"blkutx3h",	TKUCC_NMID_BLKUTX3H},
{"blkutx3l",	TKUCC_NMID_BLKUTX3L},
{"blint.w",		TKUCC_NMID_BLINTW},
// {"blerp.w",		TKUCC_NMID_BLERPW},
{"blinta.w",	TKUCC_NMID_BLINTAW},

{"blkuab1",		TKUCC_NMID_BLKUAB1},
{"blkuab2",		TKUCC_NMID_BLKUAB2},

{"ldtex",		TKUCC_NMID_LDTEX},

{"convfxi",		TKUCC_NMID_CONVFXI},
{"convfli",		TKUCC_NMID_CONVFLI},
{"snipe.dc",	TKUCC_NMID_SNIPEDC},
{"snipe.ic",	TKUCC_NMID_SNIPEIC},

{"movtt",		TKUCC_NMID_MOVTT},
{"xmovtt",		TKUCC_NMID_XMOVTT},
{"movzt",		TKUCC_NMID_MOVZT},
{"xmovzt",		TKUCC_NMID_XMOVZT},
{"movst",		TKUCC_NMID_MOVST},
{"xmovst",		TKUCC_NMID_XMOVST},


{"pscheq.w",	TKUCC_NMID_PSCHEQW},
{"pscheq.b",	TKUCC_NMID_PSCHEQB},
{"pschne.w",	TKUCC_NMID_PSCHNEW},
{"pschne.b",	TKUCC_NMID_PSCHNEB},

// {"frcp",	TKUCC_NMID_FRCP},
{"fsqrta",	TKUCC_NMID_FSQRTA},
// {"frcpa",	TKUCC_NMID_FRCPA},

{"fldcf",	TKUCC_NMID_FLDCF},
// {"fldcd",	TKUCC_NMID_FLDCD},
{"fldci",	TKUCC_NMID_FLDCI},
{"fldch",	TKUCC_NMID_FLDCH},
{"fstcf",	TKUCC_NMID_FSTCF},
// {"fstcd",	TKUCC_NMID_FSTCD},
{"fstci",	TKUCC_NMID_FSTCI},
{"fstch",	TKUCC_NMID_FSTCH},

{"fldcfh",	TKUCC_NMID_FLDCFH},
{"fstcfh",	TKUCC_NMID_FSTCFH},

// {"fldcdx",	TKUCC_NMID_FLDCDX},
// {"fstcdx",	TKUCC_NMID_FSTCDX},
// {"fldcix",	TKUCC_NMID_FLDCIX},
// {"fstcix",	TKUCC_NMID_FSTCIX},

{"pldch",	TKUCC_NMID_PLDCH},
{"pldchl",	TKUCC_NMID_PLDCH},
{"pstch",	TKUCC_NMID_PSTCH},
{"pldchh",	TKUCC_NMID_PLDCHH},

{"pldcehl",	TKUCC_NMID_PLDCEHL},
{"pldcehh",	TKUCC_NMID_PLDCEHH},

// {"pldcxh",	TKUCC_NMID_PLDCXH},
// {"pstcxh",	TKUCC_NMID_PSTCXH},

// {"keybrk_xg2",	TKUCC_NMID_KEYBRK_XG2},
// {"keybrk_base",	TKUCC_NMID_KEYBRK_BASE},


// {"movd.l",	TKUCC_NMID_MOVDL},
// {"movd",	TKUCC_NMID_MOVD},

{"movhd",	TKUCC_NMID_MOVHD},
{"movld",	TKUCC_NMID_MOVLD},
{"movhhd",	TKUCC_NMID_MOVHD},
{"movlld",	TKUCC_NMID_MOVLD},
{"movhld",	TKUCC_NMID_MOVHLD},
{"movlhd",	TKUCC_NMID_MOVLHD},

#if 0
{"cmovt.b",		TKUCC_NMID_CMOVTB},
{"cmovt.w",		TKUCC_NMID_CMOVTW},
{"cmovt.l",		TKUCC_NMID_CMOVTL},
{"cmovt.q",		TKUCC_NMID_CMOVTQ},
{"cmovf.b",		TKUCC_NMID_CMOVFB},
{"cmovf.w",		TKUCC_NMID_CMOVFW},
{"cmovf.l",		TKUCC_NMID_CMOVFL},
{"cmovf.q",		TKUCC_NMID_CMOVFQ},
{"cmovut.b",	TKUCC_NMID_CMOVUTB},
{"cmovut.w",	TKUCC_NMID_CMOVUTW},
{"cmovut.l",	TKUCC_NMID_CMOVUTL},
{"cmovdt.l",	TKUCC_NMID_CMOVDTL},
{"cmovuf.b",	TKUCC_NMID_CMOVUFB},
{"cmovuf.w",	TKUCC_NMID_CMOVUFW},
{"cmovuf.l",	TKUCC_NMID_CMOVUFL},
{"cmovdf.l",	TKUCC_NMID_CMOVDFL},
#endif

{"padd.w",		TKUCC_NMID_PADDW},
{"padd.l",		TKUCC_NMID_PADDL},
{"psub.w",		TKUCC_NMID_PSUBW},
{"psub.l",		TKUCC_NMID_PSUBL},
{"padd.h",		TKUCC_NMID_PADDH},
{"psub.h",		TKUCC_NMID_PSUBH},
{"pmul.h",		TKUCC_NMID_PMULH},

{"padd.f",		TKUCC_NMID_PADDF},
{"psub.f",		TKUCC_NMID_PSUBF},
{"pmul.f",		TKUCC_NMID_PMULF},

{"paddx.f",		TKUCC_NMID_PADDFX},
{"psubx.f",		TKUCC_NMID_PSUBFX},
{"pmulx.f",		TKUCC_NMID_PMULFX},
{"paddx.d",		TKUCC_NMID_PADDXD},
{"psubx.d",		TKUCC_NMID_PSUBXD},
{"pmulx.d",		TKUCC_NMID_PMULXD},

{"padd.fa",		TKUCC_NMID_PADDFA},
{"psub.fa",		TKUCC_NMID_PSUBFA},
{"pmul.fa",		TKUCC_NMID_PMULFA},
{"paddx.fa",	TKUCC_NMID_PADDFAX},
{"psubx.fa",	TKUCC_NMID_PSUBFAX},
{"pmulx.fa",	TKUCC_NMID_PMULFAX},

{"fadda",		TKUCC_NMID_FADDA},
{"fsuba",		TKUCC_NMID_FSUBA},
{"fmula",		TKUCC_NMID_FMULA},
{"paddx.da",	TKUCC_NMID_PADDXDA},
{"psubx.da",	TKUCC_NMID_PSUBXDA},
{"pmulx.da",	TKUCC_NMID_PMULXDA},

{"pshuf.b",		TKUCC_NMID_PSHUFB},
{"pshuf.w",		TKUCC_NMID_PSHUFW},
{"pshuf.l",		TKUCC_NMID_PSHUFL},
{"pshufx.l",	TKUCC_NMID_PSHUFXL},

{"pmulu.w",		TKUCC_NMID_PMULUW},
{"pmuls.w",		TKUCC_NMID_PMULSW},
{"pmulu.lw",	TKUCC_NMID_PMULULW},
{"pmuls.lw",	TKUCC_NMID_PMULSLW},
{"pmulu.hw",	TKUCC_NMID_PMULUHW},
{"pmuls.hw",	TKUCC_NMID_PMULSHW},

{"pcselt.w",	TKUCC_NMID_PCSELTW},
{"pcselt.l",	TKUCC_NMID_PCSELTL},
{"pcmpeq.w",	TKUCC_NMID_PCMPEQW},
{"pcmpeq.l",	TKUCC_NMID_PCMPEQL},
{"pcmphi.w",	TKUCC_NMID_PCMPHIW},
{"pcmphi.l",	TKUCC_NMID_PCMPHIL},
{"pcmpgt.w",	TKUCC_NMID_PCMPGTW},
{"pcmpgt.l",	TKUCC_NMID_PCMPGTL},

{"pcmpeq.h",	TKUCC_NMID_PCMPEQH},
{"pcmpeq.f",	TKUCC_NMID_PCMPEQF},
{"pcmpgt.h",	TKUCC_NMID_PCMPGTH},
{"pcmpgt.f",	TKUCC_NMID_PCMPGTF},

{"rgb5shr1",	TKUCC_NMID_RGB5SHR1},
{"rgb5minmax",	TKUCC_NMID_RGB5MINMAX},
{"rgb5ccenc",	TKUCC_NMID_RGB5CCENC1},
{"rgb5ccenc1",	TKUCC_NMID_RGB5CCENC1},
{"rgb5ccenc2",	TKUCC_NMID_RGB5CCENC2},

{"rgb5pck32",	TKUCC_NMID_RGB5PCK32},
{"rgb5pck64",	TKUCC_NMID_RGB5PCK64},
{"rgb5upck32",	TKUCC_NMID_RGB5UPCK32},
{"rgb5upck64",	TKUCC_NMID_RGB5UPCK64},
{"rgb32pck64",	TKUCC_NMID_RGB32PCK64},
{"rgb32upck64",	TKUCC_NMID_RGB32UPCK64},

{"pcvtsb2h",	TKUCC_NMID_PCVTSB2HL},
{"pcvtub2h",	TKUCC_NMID_PCVTUB2HL},
{"pcvtsb2hl",	TKUCC_NMID_PCVTSB2HL},
{"pcvtub2hl",	TKUCC_NMID_PCVTUB2HL},
{"pcvtsb2hh",	TKUCC_NMID_PCVTSB2HH},
{"pcvtub2hh",	TKUCC_NMID_PCVTUB2HH},

{"pcvtsw2f",	TKUCC_NMID_PCVTSW2FL},
{"pcvtuw2f",	TKUCC_NMID_PCVTUW2FL},
{"pcvtsw2fl",	TKUCC_NMID_PCVTSW2FL},
{"pcvtuw2fl",	TKUCC_NMID_PCVTUW2FL},
{"pcvtsw2fh",	TKUCC_NMID_PCVTSW2FH},
{"pcvtuw2fh",	TKUCC_NMID_PCVTUW2FH},

{"pcvth2sb",	TKUCC_NMID_PCVTH2SB},
{"pcvth2ub",	TKUCC_NMID_PCVTH2UB},
{"pcvtsw2h",	TKUCC_NMID_PCVTSW2H},
{"pcvtuw2h",	TKUCC_NMID_PCVTUW2H},

{"pcvtf2sw",	TKUCC_NMID_PCVTF2SW},
{"pcvtf2uw",	TKUCC_NMID_PCVTF2UW},
{"pcvth2sw",	TKUCC_NMID_PCVTH2SW},
{"pcvth2uw",	TKUCC_NMID_PCVTH2UW},

{"pcvth2al",	TKUCC_NMID_PCVTH2AL},
{"pcvtal2h",	TKUCC_NMID_PCVTAL2H},


{"psqrta.h",	TKUCC_NMID_PSQRTAH},
{"psqrtua.h",	TKUCC_NMID_PSQRTUAH},
{"psqrta.f",	TKUCC_NMID_PSQRTAF},
{"psqrtua.h",	TKUCC_NMID_PSQRTUAF},

{"prcpa.h",		TKUCC_NMID_PRCPAH},
{"prelu.h",		TKUCC_NMID_PRELUH},
{"prcpa.f",		TKUCC_NMID_PRCPAF},
{"prelu.f",		TKUCC_NMID_PRELUF},

{"mv",			TKUCC_NMID_MOV,		2},
{"li",			TKUCC_NMID_MOV,		2},
{"call",		TKUCC_NMID_BSR,		2},
{"addi",		TKUCC_NMID_ADD,		2},
{"addw",		TKUCC_NMID_ADDSL,	2},

{NULL, 0}
};

int bgbcc_jx2a_nmidtab_chn[1024];
int bgbcc_jx2a_nmidtab_hash[256];
int bgbcc_jx2a_init=0;

char *bgbcc_jx2a_namefornmid[1024];

int TKUCC_AsmJx2_HashCiName(char *str)
{
	char *s;
	int h;
	
	s=str; h=0;
	while(*s)
		{ h=h*251+tkucc_tolower(*s++)+31; }
	h=h*251+31;
	h=h*251+31;
	return(h>>8);
}

char *TKUCC_AsmNameForNmid(int nmid)
{
	return(bgbcc_jx2a_namefornmid[nmid]);
}

int TKUCC_AsmJx2_Init()
{
	int i, j, k;
	
	if(bgbcc_jx2a_init)
		return(0);
	bgbcc_jx2a_init=1;
	
	for(i=0; i<256; i++)
		bgbcc_jx2a_nmidtab_hash[i]=-1;	

	for(i=0; bgbcc_jx2a_nmidtab[i].name; i++)
	{
		j=TKUCC_AsmJx2_HashCiName(bgbcc_jx2a_nmidtab[i].name);
		j&=255;
		
		bgbcc_jx2a_nmidtab_chn[i]=bgbcc_jx2a_nmidtab_hash[j];
		bgbcc_jx2a_nmidtab_hash[j]=i;
		
		k=bgbcc_jx2a_nmidtab[i].nmid;
		if(!bgbcc_jx2a_namefornmid[k])
			bgbcc_jx2a_namefornmid[k]=bgbcc_jx2a_nmidtab[i].name;
	}
	return(-1);
}

int TKUCC_AsmJx2_LookupOpcodeNmid(
	TKUCC_MainContext *ctx,
	char *name)
{
	int h, skp;
	int i, j;

	h=TKUCC_AsmJx2_HashCiName(name);
	h&=255;

	i=bgbcc_jx2a_nmidtab_hash[h];
	while(i>=0)
	{
		if(!tkucc_stricmp(bgbcc_jx2a_nmidtab[i].name, name))
		{
			skp=0;
			j=bgbcc_jx2a_nmidtab[i].mfl;
			if((j&1) && (ctx->asm_modefl&TKUCC_ASMMODE_RVOPS))
				skp=1;
			if((j&2) && !(ctx->asm_modefl&TKUCC_ASMMODE_RVOPS))
				skp=1;

			if(!skp)
				return(bgbcc_jx2a_nmidtab[i].nmid);
		}
		i=bgbcc_jx2a_nmidtab_chn[i];
	}
	return(-1);
}

#if 0
int TKUCC_AsmJx2_LookupOpcodeFmid(
	TKUCC_AsmOpArg *arg0,
	TKUCC_AsmOpArg *arg1,
	TKUCC_AsmOpArg *arg2,
	TKUCC_AsmOpArg *arg3)
{
	int fm;

	if(!arg0->aty)
		return(TKUCC_FMID_NONE);

	if(!arg1->aty)
	{
		if(arg0->aty==TKUCC_ARGTY_IMM)
			return(TKUCC_FMID_IMM);
		if(arg0->aty==TKUCC_ARGTY_REG)
			return(TKUCC_FMID_REG);
		if(arg0->aty==TKUCC_ARGTY_LABEL)
			return(TKUCC_FMID_LABEL);

		if(arg0->aty==TKUCC_ARGTY_MEM_R)
			return(TKUCC_FMID_REGLD4);

		return(0);
	}

	if(!arg2->aty)
	{
		fm=0;
		switch(arg0->aty)
		{
		case TKUCC_ARGTY_REG:
			switch(arg1->aty)
			{
			case TKUCC_ARGTY_REG:
				fm=TKUCC_FMID_REGREGB; break;
			case TKUCC_ARGTY_MEM_R:
				fm=TKUCC_FMID_REGST4; break;
//			case TKUCC_ARGTY_MEM_RD:
//				fm=BGBCC_SH_FMID_REGDECST; break;
			case TKUCC_ARGTY_MEM_RI:
				fm=TKUCC_FMID_REGSTR0N; break;
			case TKUCC_ARGTY_MEM_RD:
				fm=TKUCC_FMID_REGSTDISP; break;
			case TKUCC_ARGTY_LABEL:
				fm=TKUCC_FMID_REGLBL; break;
			default: fm=0; break;
			}
			break;
		case TKUCC_ARGTY_IMM:
			switch(arg1->aty)
			{
			case TKUCC_ARGTY_REG:
				fm=TKUCC_FMID_REGIMM_10; break;
//			case TKUCC_ARGTY_MEM_RI:
//				fm=TKUCC_FMID_IMMSTRMN; break;
			default: fm=0; break;
			}
			break;
		case TKUCC_ARGTY_MEM_R:
			switch(arg1->aty)
			{
			case TKUCC_ARGTY_REG:
				fm=BGBCC_SH_FMID_REGLD; break;
			default: fm=0; break;
			}
			break;
		case TKUCC_ARGTY_MEM_RI:
			switch(arg1->aty)
			{
			case TKUCC_ARGTY_REG:
				fm=BGBCC_SH_FMID_REGINCLD; break;
			default: fm=0; break;
			}
			break;
		case TKUCC_ARGTY_MEM_RI:
			switch(arg1->aty)
			{
			case TKUCC_ARGTY_REG:
				fm=BGBCC_SH_FMID_REGLDR0M; break;
			default: fm=0; break;
			}
			break;
		case TKUCC_ARGTY_MEM_RD:
			switch(arg1->aty)
			{
			case TKUCC_ARGTY_REG:
				fm=BGBCC_SH_FMID_REGLDDISP; break;
			default: fm=0; break;
			}
			break;
		case TKUCC_ARGTY_LABEL:
			switch(arg1->aty)
			{
			case TKUCC_ARGTY_REG:
				fm=BGBCC_SH_FMID_LBLREG; break;
			default: fm=0; break;
			}
			break;
		default:
			fm=0; break;
		}
		
		return(fm);
	}

	if(!arg3->aty)
	{
		fm=0;
		switch(arg0->aty)
		{
		case TKUCC_ARGTY_REG:
			switch(arg1->aty)
			{
			case TKUCC_ARGTY_REG:
				switch(arg2->aty)
				{
				case TKUCC_ARGTY_REG:
					fm=BGBCC_SH_FMID_REGREGREG; break;
				case TKUCC_ARGTY_LABEL:
					fm=BGBCC_SH_FMID_REGREGLBL; break;
				}
				break;
			case TKUCC_ARGTY_IMM:
				switch(arg2->aty)
				{
				case TKUCC_ARGTY_REG:
					fm=TKUCC_FMID_REGIMM_10REG; break;
				}
				break;
			default: fm=0; break;
			}
			break;
		}
		return(fm);
	}

	if(1)
	{
		fm=0;
		switch(arg0->aty)
		{
		case TKUCC_ARGTY_REG:
			switch(arg1->aty)
			{
			case TKUCC_ARGTY_REG:
				switch(arg2->aty)
				{
				case TKUCC_ARGTY_REG:
					switch(arg3->aty)
					{
					case TKUCC_ARGTY_REG:
						fm=BGBCC_SH_FMID_REGREGREGREG; break;
					case TKUCC_ARGTY_IMM:
						fm=BGBCC_SH_FMID_REGREGREGIMM; break;
					default: fm=0; break;
					}
					break;
				case TKUCC_ARGTY_IMM:
					switch(arg3->aty)
					{
					case TKUCC_ARGTY_REG:
						fm=BGBCC_SH_FMID_REGREGIMMREG; break;
					default: fm=0; break;
					}
					break;
				default: fm=0; break;
				}
				break;
#if 1
			case TKUCC_ARGTY_IMM:
				switch(arg2->aty)
				{
				case TKUCC_ARGTY_REG:
					switch(arg3->aty)
					{
					case TKUCC_ARGTY_REG:
						fm=TKUCC_FMID_REGIMM_10REGREG; break;
					default: fm=0; break;
					}
					break;
				default: fm=0; break;
				}
				break;
#endif
			default: fm=0; break;
			}
			break;
		}
		return(fm);
	}

	return(0);
}
#endif


int TKUCC_AsmJx2_TryAssembleOpcode(
	TKUCC_MainContext *ctx,
	char *name,
	TKUCC_AsmOpArg *arg0,
	TKUCC_AsmOpArg *arg1,
	TKUCC_AsmOpArg *arg2,
	TKUCC_AsmOpArg *arg3)
{
	TKUCC_AsmOp t_op;
	TKUCC_AsmOp *op;
	int nmid, fmid, rt, lbl;
	
	if(name[0]=='.')
		return(0);
	
	nmid=TKUCC_AsmJx2_LookupOpcodeNmid(ctx, name);
	if(nmid<=0)
	{
		printf("Unknown Instruction %s\n", name);
		return(0);
	}

//	fmid=TKUCC_AsmJx2_LookupOpcodeFmid(arg0, arg1, arg2, arg3);
//	if(fmid<=0)
//		return(0);

	op=&t_op;
	op->nmid=nmid;
	op->fmid=0;
	op->n_arg=0;
	op->pred=ctx->op_is_wex2;
	
	if(arg0->aty==TKUCC_ARGTY_LABEL)
	{
		if(arg0->name && !arg0->lbl)
			arg0->lbl=TKUCC_AsmGetNamedLabel(ctx, arg0->name);
	}
	
	if(arg1->aty==TKUCC_ARGTY_LABEL)
	{
		if(arg1->name && !arg1->lbl)
			arg1->lbl=TKUCC_AsmGetNamedLabel(ctx, arg1->name);
	}
	
	if(arg0 && arg0->aty)
	{
		op->arg[0]=arg0;
		op->n_arg=1;
		if(arg1 && arg1->aty)
		{
			op->arg[1]=arg1;
			op->n_arg=2;
			if(arg2 && arg2->aty)
			{
				op->arg[2]=arg2;
				op->n_arg=3;
				if(arg3 && arg3->aty)
				{
					op->arg[3]=arg3;
					op->n_arg=4;
				}
			}
		}
	}

	if(ctx->iflvl_f>0)
		return(1);
//	rt=0;

	rt=TKUCC_AsmEncodeOp(ctx, op);

#if 0
	switch(fmid)
	{
	case BGBCC_SH_FMID_REGREG:
		rt=BGBCC_JX2_TryEmitOpRegReg(ctx,
			nmid, arg0->rb, arg1->rb);
		break;
	case TKUCC_FMID_REGIMM_10:
		if((nmid==TKUCC_NMID_MOV) && !(ctx->emit_riscv&0x33))
		{
			if(	(arg1->rb==TKUCC_REG_R0) ||
				(arg1->rb==TKUCC_REG_DLR))
			{
				BGBCC_JX2_EmitLoadDrImm(ctx, arg0->imm);
				rt=1;
				break;
			}
		
			BGBCC_JX2_EmitLoadRegImm64P(ctx,
				arg1->rb, arg0->imm);
			rt=1;
			break;
		}
	
		rt=BGBCC_JX2_TryEmitOpImmReg(ctx,
			nmid, arg0->imm, arg1->rb);
		break;

	case BGBCC_SH_FMID_REGREGREG:
		rt=BGBCC_JX2_TryEmitOpRegRegReg(ctx,
			nmid, arg0->rb, arg1->rb, arg2->rb);
		break;
	case TKUCC_FMID_REGIMM_10REG:
		rt=BGBCC_JX2_TryEmitOpRegImmReg(ctx,
			nmid, arg0->rb, arg1->imm, arg2->rb);
		break;

	case BGBCC_SH_FMID_REGREGREGREG:
		rt=BGBCC_JX2_TryEmitOpRegRegRegReg(ctx,
			nmid, arg0->rb, arg1->rb, arg2->rb, arg3->rb);
		break;
	case BGBCC_SH_FMID_REGREGIMMREG:
		rt=BGBCC_JX2_TryEmitOpRegRegImmReg(ctx,
			nmid, arg0->rb, arg1->rb, arg2->imm, arg3->rb);
		break;
	case BGBCC_SH_FMID_REGREGREGIMM:
		rt=BGBCC_JX2_TryEmitOpRegRegImmReg(ctx,
			nmid, arg0->rb, arg1->rb, arg3->imm, arg2->rb);
		break;

	case TKUCC_FMID_REGIMM_10REGREG:
		rt=BGBCC_JX2_TryEmitOpRegImmRegReg(ctx,
			nmid, arg0->rb, arg1->imm, arg2->rb, arg3->rb);
		break;

	case BGBCC_SH_FMID_REGST:
		rt=BGBCC_JX2_TryEmitOpRegStReg(ctx,
			nmid, arg0->rb, arg1->rb);
		break;
	case BGBCC_SH_FMID_REGLD:
		rt=BGBCC_JX2_TryEmitOpLdRegReg(ctx,
			nmid, arg0->rb, arg1->rb);
		break;
	case BGBCC_SH_FMID_REGDECST:
		rt=BGBCC_JX2_TryEmitOpRegStDecReg(ctx,
			nmid, arg0->rb, arg1->rb);
		break;
	case BGBCC_SH_FMID_REGINCLD:
		rt=BGBCC_JX2_TryEmitOpLdIncRegReg(ctx,
			nmid, arg0->rb, arg1->rb);
		break;
	case BGBCC_SH_FMID_REGSTR0N:
		rt=BGBCC_JX2_TryEmitOpRegStReg2(ctx,
			nmid, arg0->rb, arg1->ri, arg1->rb);
		break;
	case BGBCC_SH_FMID_REGLDR0M:
		rt=BGBCC_JX2_TryEmitOpLdReg2Reg(ctx,
			nmid, arg0->rb, arg0->ri, arg1->rb);
		break;
	case BGBCC_SH_FMID_REGSTDISP:
		rt=BGBCC_JX2_TryEmitOpRegStRegDisp(ctx,
			nmid, arg0->rb, arg1->rb, arg1->imm);
		break;
	case BGBCC_SH_FMID_REGLDDISP:
		rt=BGBCC_JX2_TryEmitOpLdRegDispReg(ctx,
			nmid, arg0->rb, arg0->imm, arg1->rb);
		break;
	
	case TKUCC_FMID_NONE:
		rt=BGBCC_JX2_TryEmitOpNone(ctx, nmid);
		break;
	
	case BGBCC_SH_FMID_REGRM:
	case TKUCC_FMID_REG:
		rt=BGBCC_JX2_TryEmitOpReg(ctx, nmid, arg0->rb);
		break;
	case TKUCC_FMID_IMM:
		rt=BGBCC_JX2_TryEmitOpImm(ctx, nmid, arg0->imm);
		break;
	case TKUCC_FMID_REGLD4:
	case BGBCC_SH_FMID_STREG:
		rt=BGBCC_JX2_TryEmitOpMReg(ctx, nmid, arg0->rb);
		break;

	case TKUCC_FMID_LABEL:
		lbl=BGBCC_JX2_GetNamedLabel(ctx, arg0->name);
		rt=BGBCC_JX2_TryEmitOpAutoLabel(ctx, nmid, lbl);
		break;
	case BGBCC_SH_FMID_LBLREG:
		if((nmid==TKUCC_NMID_MOV) && !(ctx->emit_riscv&0x21))
		{
			rt=BGBCC_JX2C_CheckEmitLoadFrameNameReg(
				ctx->tctx, ctx, arg0->name, arg1->rb);
			if(rt>0)
				break;
		}

		lbl=BGBCC_JX2_GetNamedLabel(ctx, arg0->name);
		rt=BGBCC_JX2_TryEmitOpLblReg(ctx, nmid, lbl, arg1->rb);
		break;

	case BGBCC_SH_FMID_REGLBL:
		if((nmid==TKUCC_NMID_MOV) && !(ctx->emit_riscv&0x21))
		{
			rt=BGBCC_JX2C_CheckEmitStoreFrameNameReg(
				ctx->tctx, ctx, arg1->name, arg0->rb);
			if(rt>0)
				break;
		}

		lbl=BGBCC_JX2_GetNamedLabel(ctx, arg1->name);
		rt=BGBCC_JX2_TryEmitOpRegLbl(ctx, nmid, lbl, arg0->rb);
		break;

	case BGBCC_SH_FMID_REGREGLBL:
		lbl=BGBCC_JX2_GetNamedLabel(ctx, arg2->name);
		rt=BGBCC_JX2_TryEmitOpRegRegLbl(ctx, nmid,
			arg0->rb, arg1->rb, lbl);
		break;

	default:
		__debugbreak();
		rt=0;
		break;
	}
#endif

	return(rt);
}

int TKUCC_AsmJx2_ParseCheckFeatureList(TKUCC_MainContext *ctx, char *sym)
{
	char tb[256];
	char *cs, *ct;
	int dfl, ni;

	dfl=0;
	cs=sym;
	ct=tb;
	while(*cs)
	{
		if(*cs=='(')
		{
			cs++; ni=1;
			while(*cs)
			{
				if(*cs=='(')
				{
					*ct++=*cs++;
					ni++;
					continue;
				}
				if(*cs==')')
				{
					ni--;
					if(!ni)
						break;
					*ct++=*cs++;
					continue;
				}
				*ct++=*cs++;
			}
			
			ni=TKUCC_AsmJx2_ParseCheckFeatureList(ctx, tb);
			sprintf(tb, "%d", ni);
			ct=tb+strlen(tb);
			continue;
		}

		if((*cs==',') || (*cs=='|'))
		{
			while(*cs==',')cs++;
			while(*cs=='|')cs++;
			*ct++=0;
			if(tb[0])
			{
				if(TKUCC_AsmJx2_ParseCheckFeature(ctx, tb)!=0)
					return(1);
				ct=tb;
				continue;
			}
			continue;
		}

		if(*cs=='&')
		{
			while(*cs=='&')cs++;
			*ct++=0;
			if(tb[0])
			{
				if(TKUCC_AsmJx2_ParseCheckFeature(ctx, tb)==0)
					return(0);
				ct=tb;
				continue;
			}
			continue;
		}
		
		*ct++=*cs++;
	}

	*ct++=0;
	if(tb[0])
	{
		if(TKUCC_AsmJx2_ParseCheckFeature(ctx, tb)!=0)
			return(1);
	}
	return(0);
}

int TKUCC_AsmJx2_ParseCheckFeature(TKUCC_MainContext *ctx, char *sym)
{
#if 0
	BGBCC_TransState *tctx;
	int i;

	if(!tkucc_stricmp(sym, "bjx2"))
		return(1);
	
	if((*sym>='0') && *sym<='9')
	{
		i=atoi(sym);
		return(i!=0);
	}
	
	if(*sym=='!')
	{
		return(!TKUCC_AsmJx2_ParseCheckFeature(ctx, sym+1));
	}

	tctx=ctx->tctx;

	if(!tkucc_stricmp(sym, "bjx2_fix32"))
		return((ctx->is_fixed32)!=0);
	if(!tkucc_stricmp(sym, "bjx2_xg2"))
		return((ctx->is_fixed32&2)!=0);

	if(!tkucc_stricmp(sym, "bjx1_egpr"))
		return(ctx->has_bjx1egpr);
	if(!tkucc_stricmp(sym, "bjx1_betav"))
		return(ctx->is_betav);
	if(!tkucc_stricmp(sym, "bjx1_64"))
		return(ctx->is_addr64);

	if(	!tkucc_stricmp(sym, "bjx1_nofpu") ||
		!tkucc_stricmp(sym, "bjx2_nofpu"))
		return(ctx->no_fpu);
	if(	!tkucc_stricmp(sym, "bjx1_noext32") ||
		!tkucc_stricmp(sym, "bjx2_noext32"))
		return(ctx->no_ext32);

	if(	!tkucc_stricmp(sym, "bjx1_fpu_gfp") ||
		!tkucc_stricmp(sym, "bjx2_fpu_gfp"))
		return(ctx->fpu_gfp);

	if(!tkucc_stricmp(sym, "bjx2_ptr32"))
		return(ctx->is_addr_x32);
	if(!tkucc_stricmp(sym, "bjx2_ptr64"))
		return(!(ctx->is_addr_x32));


	if(!tkucc_stricmp(sym, "bjx2_wex"))
		return(ctx->use_wexmd!=0);
	if(!tkucc_stricmp(sym, "bjx2_wex2"))
		return(ctx->use_wexmd==1);
	if(!tkucc_stricmp(sym, "bjx2_wex3"))
		return(ctx->use_wexmd==2);
	if(!tkucc_stricmp(sym, "bjx2_jumbo"))
		return(ctx->has_jumbo);

	if(!tkucc_stricmp(sym, "bjx2_wex3p"))
		return(ctx->use_wexmd>=2);

	if(!tkucc_stricmp(sym, "bjx2_movx"))
		return(ctx->has_pushx2);

	if(!tkucc_stricmp(sym, "has_movx"))
		return(ctx->has_pushx2);
	if(!tkucc_stricmp(sym, "has_simdx2"))
		return(ctx->has_simdx2);

	if(!tkucc_stricmp(sym, "has_alux"))
		return(ctx->has_alux);
	if(!tkucc_stricmp(sym, "has_fpux"))
		return(ctx->has_fpux);

	if(!tkucc_stricmp(sym, "has_bra48"))
		return(ctx->has_bra48);
	if(!tkucc_stricmp(sym, "has_xgpr"))
		return((ctx->has_xgpr&1)!=0);

	if(!tkucc_stricmp(sym, "has_aluptr"))
		return((ctx->has_qmul&16)!=0);

	if(!tkucc_stricmp(sym, "abi_is_xgpr"))
	{
		if(tctx->arch_sizeof_ptr==8)
			return(0);
		return((ctx->has_xgpr&2)!=0);
	}

	if(!tkucc_stricmp(sym, "has_dmacl"))
		return((ctx->has_dmacl&1)!=0);
	if(!tkucc_stricmp(sym, "has_qmul"))
		return((ctx->has_qmul&1)!=0);

	if(!tkucc_stricmp(sym, "has_fdiv"))
		return((ctx->has_qmul&2)!=0);
	if(!tkucc_stricmp(sym, "has_bcd"))
		return((ctx->has_qmul&4)!=0);

	if(!tkucc_stricmp(sym, "has_fmovs"))
		return((ctx->has_fmovs&1)!=0);
	if(!tkucc_stricmp(sym, "has_fmovh"))
		return((ctx->has_fmovs&2)!=0);
	if(!tkucc_stricmp(sym, "has_ldtex"))
		return((ctx->has_fmovs&4)!=0);
	if(!tkucc_stricmp(sym, "has_ldgbr"))
		return((ctx->has_fmovs&8)!=0);
	if(!tkucc_stricmp(sym, "has_xmov"))
		return((ctx->has_fmovs&16)!=0);

	if(!tkucc_stricmp(sym, "has_jcmpz"))
		return((ctx->has_jcmp&1)!=0);
	if(!tkucc_stricmp(sym, "has_jcmpr"))
		return((ctx->has_jcmp&2)!=0);

	if(!tkucc_stricmp(sym, "has_jcmp"))
		return((ctx->has_jcmp&3)==3);

	if(!tkucc_stricmp(sym, "has_fpvsf"))
		return((ctx->has_fpvsf&1)!=0);
	if(!tkucc_stricmp(sym, "has_fpvsf_sp"))
		return((ctx->has_fpvsf&2)!=0);

	if(!tkucc_stricmp(sym, "has_fpimm"))
		return((ctx->has_fpim&1)!=0);

	if(!tkucc_stricmp(sym, "abi_argshadow"))
		return((ctx->abi_spillpad&1)!=0);
	if(!tkucc_stricmp(sym, "abi_vskgen"))
		return((ctx->abi_spillpad&2)!=0);
	if(!tkucc_stricmp(sym, "abi_boundschk"))
		return((ctx->abi_spillpad&4)!=0);
	if(!tkucc_stricmp(sym, "abi_vskglobal"))
		return((ctx->abi_spillpad&8)!=0);


	if(!tkucc_stricmp(sym, "abi_evenonly"))
		return(ctx->abi_evenonly);

	if(!tkucc_stricmp(sym, "seen_variant"))
		return((tctx->ccxl_tyc_seen&BGBCC_TYCSEEN_VARIANT)!=0);
	if(!tkucc_stricmp(sym, "seen_int128"))
		return((tctx->ccxl_tyc_seen&BGBCC_TYCSEEN_INT128)!=0);
	if(!tkucc_stricmp(sym, "seen_float128"))
		return((tctx->ccxl_tyc_seen&BGBCC_TYCSEEN_FLOAT128)!=0);
	if(!tkucc_stricmp(sym, "seen_float_fpu"))
		return((tctx->ccxl_tyc_seen&BGBCC_TYCSEEN_FLOAT_FPU)!=0);

	if(!tkucc_stricmp(sym, "seen_idiv_var"))
		return((tctx->ccxl_tyc_seen&BGBCC_TYCSEEN_IDIV_VAR)!=0);
	if(!tkucc_stricmp(sym, "seen_imod_var"))
		return((tctx->ccxl_tyc_seen&BGBCC_TYCSEEN_IMOD_VAR)!=0);

#endif

	return(0);
}

int TKUCC_AsmJx2_ParseOpcode_ScanForBar(TKUCC_MainContext *ctx, char *cs0)
{
	char *cs;
	int rt;
	
	cs=cs0; rt=0;
	while(*cs)
	{
		if(*cs=='\r')
			break;
		if(*cs=='\n')
			break;
			
		if(*cs=='|')
			{ rt=1; break; }
		if(*cs=='?')
			{ rt=1; break; }
		cs++;
	}
	
	return(rt);
}

int TKUCC_AsmJx2_ParseOpcode(TKUCC_MainContext *ctx, char **rcs)
{
	TKUCC_AsmOpArg arg[4];
	char tb0[256];
	char *tk0, *tk1;
	char *cs, *cs1, *cs2, *cs3;
	s64 li;
	f32 ff;
	f64 fd;
	int i, j, k, wx, pfc;

	pfc=0;

	cs=*rcs;
	cs1=TKUCC_AsmJx2_ParseTokenAlt(cs, &tk0);

	if(*tk0=='I')
	{
		cs1=TKUCC_AsmJx2_EatWhiteOnlyNoLinebreak(cs1);
		if(*cs1==':')
		{
			if(ctx->iflvl_f>0)
			{
				*rcs=cs1+1;
				return(1);
			}

#if 1
			if(ctx->asm_modefl&TKUCC_ASMMODE_ALIGN_WEXJ12)
			{
				if(ctx->asm_modefl&TKUCC_ASMMODE_ALIGN_WEXJ2)
				{
					TKUCC_AsmEmitPad32Align(ctx);
				}else
				{
					/* Detect WEX or Pred ops, if so, align label. */
					cs2=TKUCC_AsmJx2_EatWhite(cs1+1);
					if(TKUCC_AsmJx2_ParseOpcode_ScanForBar(ctx, cs2))
					{
						TKUCC_AsmEmitPad32Align(ctx);
					}
				}
			}
#endif
			
			if((tk0[1]=='.') && bgbcc_jx2a_lastlbl)
			{
				TKUCC_AsmEmitPadForLabel(ctx);

				sprintf(tb0, "%s%s", bgbcc_jx2a_lastlbl, tk0+1);
				TKUCC_AsmEmitNamedLabel(ctx, tb0);
			}else
			{
				TKUCC_AsmEmitPadForLabel(ctx);

				bgbcc_jx2a_lastlbl=TKUCC_StrdupInternAsm(ctx, tk0+1);
				TKUCC_AsmEmitNamedLabel(ctx, tk0+1);
			}
			*rcs=cs1+1;
			return(1);
		}
		
		if(cs1[0]=='?')
		{
			if((cs1[1]=='s') || (cs1[1]=='S'))
			{
				if((cs1[2]=='t') || (cs1[2]=='T'))
				{
					cs1+=3; pfc=8;
				}else
					if((cs1[2]=='f') || (cs1[2]=='F'))
				{
					cs1+=3; pfc=9;
				}
			}else
				if((cs1[1]=='t') || (cs1[1]=='T'))
			{
				cs1+=2; pfc=4;
			}else
				if((cs1[1]=='f') || (cs1[1]=='F'))
			{
				cs1+=2; pfc=5;
			}
		}

		cs2=cs1;
		arg[0].aty=0; arg[1].aty=0; arg[2].aty=0; arg[3].aty=0;
		for(i=0; i<4; i++)
		{
			if(TKUCC_AsmJx2_ParseOperand(ctx, &cs2, &arg[i])<=0)
				break;
			cs2=TKUCC_AsmJx2_EatWhiteNoLinebreak(cs2);
			if(*cs2==',')
				cs2++;
		}

		wx=0;
		cs2=TKUCC_AsmJx2_EatWhiteNoLinebreak(cs2);
		if(*cs2=='|')
			{ wx=1; }
		else
		{
			cs3=TKUCC_AsmJx2_EatWhite(cs2);
			if(cs3 && (*cs3=='|'))
				{ wx=1; cs2=cs3; }
		}
		
		if(wx || pfc)
		{
//			BGBCC_JX2_EmitPadForOpWord(ctx, 0xF000);
		}
		
		if((ctx->asm_modefl&TKUCC_ASMMODE_ALIGN_WEXJ12) && (wx || pfc))
		{
			TKUCC_AsmEmitPad32Align(ctx);
		}
			
		if(wx)
		{
			ctx->op_is_wex2=3;
		}
		
		if(pfc)
		{
			if(wx)
				ctx->op_is_wex2=pfc|2;
			else
				ctx->op_is_wex2=pfc;
		}

		k=TKUCC_AsmJx2_TryAssembleOpcode(ctx, tk0+1,
			&arg[0], &arg[1], &arg[2], &arg[3]);
		if(k>0)
		{
			if(*cs2==';')
				cs2++;
			if(*cs2=='|')
			{
				cs2++;
				if(*cs2=='|')
					cs2++;
			}

			if(pfc)
				{ ctx->op_is_wex2=0; }
			
			ctx->op_is_wex2=wx?2:0;
		
			*rcs=cs2;
			return(1);
		}

		if(wx)
			{ ctx->op_is_wex2=0; }
		if(pfc)
			{ ctx->op_is_wex2=0; }

		if(!strcmp(tk0, "I.align"))
		{
			cs2=cs1;
			cs2=TKUCC_AsmJx2_ParseToken(cs2, &tk0);
			TKUCC_AsmEmitBAlign(ctx, tkucc_atoll(tk0+1)*2);
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.balign"))
		{
			cs2=cs1;
			cs2=TKUCC_AsmJx2_ParseToken(cs2, &tk0);
			TKUCC_AsmEmitBAlign(ctx, tkucc_atoll(tk0+1));
			*rcs=cs2;
			return(1);
		}

#if 0
		if(!strcmp(tk0, "I.rept"))
		{
			cs2=cs1;
			cs2=TKUCC_AsmJx2_ParseToken(cs2, &tk0);
			ctx->cnrept=tkucc_atoll(tk0+1);
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
#endif

		if(!strcmp(tk0, "I.long") ||
			!strcmp(tk0, "I.int") ||
			!strcmp(tk0, "I.dword"))
		{
			cs2=cs1;
			cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk0);
			cs2=TKUCC_AsmJx2_EatWhiteNoLinebreak(cs2);

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
					cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk1);
					if(tk1[0]=='I')
					{
						TKUCC_AsmEmitNamedReloc(ctx, tk0+1,
							TKUCC_RELOC_REL32);
						TKUCC_AsmEmitNamedReloc(ctx, tk1+1,
							TKUCC_RELOC_REL32B);
					}else if(tk1[0]=='|')
					{
						TKUCC_AsmEmitNamedReloc(ctx, tk0+1,
							TKUCC_RELOC_DIR32);
						k=-tkucc_atoll(tk1+1);
					}
				}else if(*cs2=='+')
				{
					cs2++;
					cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk1);
					if(tk1[0]=='I')
					{
						TKUCC_AsmEmitNamedReloc(ctx, tk0+1,
							TKUCC_RELOC_REL32);
						TKUCC_AsmEmitNamedReloc(ctx, tk1+1,
							TKUCC_RELOC_REL32);
					}else if(tk1[0]=='|')
					{
						TKUCC_AsmEmitNamedReloc(ctx, tk0+1,
							TKUCC_RELOC_DIR32);
						k=tkucc_atoll(tk1+1);
					}
				}else
				{
					TKUCC_AsmEmitNamedReloc(ctx, tk0+1,
						TKUCC_RELOC_DIR32);
				}

				TKUCC_AsmEmitDWord(ctx, k);
			}else if((*tk0=='|') || (*tk0=='C'))

			{
				if(*tk0=='|')
					i=tkucc_atoll(tk0+1);
				if(*tk0=='C')
					i=(tk0[1]|(tk0[2]<<8)|(tk0[3]<<16)|(tk0[4]<<24));
				TKUCC_AsmEmitDWord(ctx, i);
			}

			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk0);

				if(*tk0=='I')
				{
					TKUCC_AsmEmitNamedReloc(ctx, tk0+1,
						TKUCC_RELOC_DIR32);
					TKUCC_AsmEmitDWord(ctx, 0);
					continue;
				}

				if(*tk0=='|')
					i=tkucc_atoll(tk0+1);
				if(*tk0=='C')
					i=(tk0[1]|(tk0[2]<<8)|(tk0[3]<<16)|(tk0[4]<<24));
				TKUCC_AsmEmitDWord(ctx, i);
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.word") ||
			!strcmp(tk0, "I.short") ||
			!strcmp(tk0, "I.hword"))
		{
			cs2=cs1;
			cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk0);
			if(*tk0=='|')
				i=tkucc_atoll(tk0+1);
			if(*tk0=='C')
				i=(tk0[1]|(tk0[2]<<8));
			TKUCC_AsmEmitWord(ctx, i);

			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk0);

				if(*tk0=='|')
					i=tkucc_atoll(tk0+1);
				if(*tk0=='C')
					i=(tk0[1]|(tk0[2]<<8));
				TKUCC_AsmEmitWord(ctx, i);
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.byte"))
		{
			cs2=cs1;
			cs2=TKUCC_AsmJx2_ParseToken(cs2, &tk0);
			if(*tk0=='|')
				i=tkucc_atoll(tk0+1);
			if(*tk0=='C')
				i=tk0[1];
			TKUCC_AsmEmitByte(ctx, i);
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=TKUCC_AsmJx2_ParseToken(cs2, &tk0);
				if(*tk0=='|')
					i=tkucc_atoll(tk0+1);
				if(*tk0=='C')
					i=tk0[1];
				TKUCC_AsmEmitByte(ctx, i);
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.quad") ||
			!strcmp(tk0, "I.qword"))
		{
			cs2=cs1;
			cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk0);
			TKUCC_AsmEmitQWord(ctx, tkucc_atoll(tk0+1));
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk0);
				TKUCC_AsmEmitQWord(ctx, tkucc_atoll(tk0+1));
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.float") || !strcmp(tk0, "I.single"))
		{
			cs2=cs1;
			cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk0);
			ff=tkucc_atof(tk0+1);
			TKUCC_AsmEmitDWord(ctx, *(u32 *)(&ff));
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk0);
				ff=tkucc_atof(tk0+1);
				TKUCC_AsmEmitDWord(ctx, *(u32 *)(&ff));
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.double"))
		{
			cs2=cs1;
			cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk0);
			fd=tkucc_atof(tk0+1);
			TKUCC_AsmEmitQWord(ctx, *(u64 *)(&fd));
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk0);
				fd=tkucc_atof(tk0+1);
				TKUCC_AsmEmitQWord(ctx, *(u64 *)(&fd));
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.string") ||
			!strcmp(tk0, "I.asciz"))
		{
			cs2=cs1;
			cs2=TKUCC_AsmJx2_ParseToken(cs2, &tk0);
			TKUCC_AsmEmitString(ctx, tk0+1);
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=TKUCC_AsmJx2_ParseToken(cs2, &tk0);
				TKUCC_AsmEmitString(ctx, tk0+1);
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.ascii"))
		{
			cs2=cs1;
			cs2=TKUCC_AsmJx2_ParseToken(cs2, &tk0);
			TKUCC_AsmEmitAscii(ctx, tk0+1);
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=TKUCC_AsmJx2_ParseToken(cs2, &tk0);
				TKUCC_AsmEmitAscii(ctx, tk0+1);
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.ucstr"))
		{
			cs2=cs1;
			cs2=TKUCC_AsmJx2_ParseToken(cs2, &tk0);
			TKUCC_AsmEmitAsciiUTF2UCS2(ctx, tk0+1);
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=TKUCC_AsmJx2_ParseToken(cs2, &tk0);
				TKUCC_AsmEmitAsciiUTF2UCS2(ctx, tk0+1);
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.ucstrz"))
		{
			cs2=cs1;
			cs2=TKUCC_AsmJx2_ParseToken(cs2, &tk0);
			TKUCC_AsmEmitStringUTF2UCS2(ctx, tk0+1);
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=TKUCC_AsmJx2_ParseToken(cs2, &tk0);
				TKUCC_AsmEmitStringUTF2UCS2(ctx, tk0+1);
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.section"))
		{
			cs2=cs1;
			cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk0);
			TKUCC_AsmSetSection(ctx, tk0+1);

			if(*cs2==',')
			{
				while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
					cs2++;
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.arch"))
		{
			cs2=cs1;
			cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk0);

			if(	!strcmp(tk0, "bjx2")	 ||
				!strcmp(tk0, "bjx2a")	)
			{
				TKUCC_AsmEmitBAlign(ctx, 4);
				ctx->asm_modefl&=~TKUCC_ASMMODE_RVOPNREGS;
			}

			if(!strcmp(tk0, "riscv"))
			{
				TKUCC_AsmEmitBAlign(ctx, 4);
				ctx->asm_modefl|=TKUCC_ASMMODE_RVOPNREGS;
			}

			if(!strcmp(tk0, "xg2rv"))
			{
				TKUCC_AsmEmitBAlign(ctx, 4);
				ctx->asm_modefl|=TKUCC_ASMMODE_RVREGS;
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.riscv"))
		{
			TKUCC_AsmEmitBAlign(ctx, 4);
			ctx->asm_modefl|=TKUCC_ASMMODE_RVOPNREGS;
			*rcs=cs1;
			return(1);
		}

		if(!strcmp(tk0, "I.endriscv") ||
			!strcmp(tk0, "I.bjx2"))
		{
			TKUCC_AsmEmitBAlign(ctx, 4);
			ctx->asm_modefl&=~TKUCC_ASMMODE_RVOPNREGS;
			*rcs=cs1;
			return(1);
		}

		if(	!strcmp(tk0, "I.text") ||
			!strcmp(tk0, "I.data") ||
			!strcmp(tk0, "I.utext") ||
			!strcmp(tk0, "I.udata") ||
			!strcmp(tk0, "I.bss") ||
			!strcmp(tk0, "I.rodata"))
		{
			TKUCC_AsmSetSection(ctx, tk0+1);
			*rcs=cs1;
			return(1);
		}

		if(!strcmp(tk0, "I.global") ||
			!strcmp(tk0, "I.globl") ||
			!strcmp(tk0, "I.extern") ||
			!strcmp(tk0, "I.weak"))
		{
			cs2=cs1;
			cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk0);
			TKUCC_AsmEmitNamedGlobal(ctx, tk0+1);
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.local"))
		{
			cs2=cs1;
			cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk0);
			TKUCC_AsmEmitNamedGlobal(ctx, tk0+1);
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.rel32"))
		{
			cs2=cs1;
			cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk0);

			TKUCC_AsmEmitNamedReloc(ctx, tk0+1,
				TKUCC_RELOC_REL32);

			if(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk0);
				TKUCC_AsmEmitNamedReloc(ctx, tk0+1,
					TKUCC_RELOC_REL32B);
			}

			TKUCC_AsmEmitDWord(ctx, 0);
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.comm"))
		{
			cs2=cs1;
			cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk0);

			j=1; k=1;
			if(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk1);
				j=atoi(tk1+1);
			}
			if(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk1);
				k=atoi(tk1+1);
			}

			j=j*k;

			TKUCC_AsmEmitNamedCommSym(ctx, tk0+1, j, 0);
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.skip") || !strcmp(tk0, "I.space"))
		{
			j=1; k=0;

			cs2=cs1;
			cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk0);
			j=tkucc_atoll(tk0+1);

			if(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk1);
				k=tkucc_atoll(tk1+1);
			}
			
			while((j--)>0)
				TKUCC_AsmEmitByte(ctx, k);

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

		if(!strcmp(tk0, "I.break"))
		{
			__debugbreak();

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
			cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk0);

			i=TKUCC_AsmJx2_ParseCheckFeatureList(ctx, tk0+1);
			if(ctx->iflvl_f>0)
			{
				ctx->iflvl_f++;
			}else
			{
				if(i>0)
					{ ctx->iflvl_t++; }
				else
					{ ctx->iflvl_f++; }
			}

			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.ifnarch"))
		{
			cs2=cs1;
			cs2=TKUCC_AsmJx2_ParseTokenAlt(cs2, &tk0);

			i=TKUCC_AsmJx2_ParseCheckFeatureList(ctx, tk0+1);

			if(ctx->iflvl_f>0)
			{
				ctx->iflvl_f++;
			}else
			{
				if(i>0)
					{ ctx->iflvl_f++; }
				else
					{ ctx->iflvl_t++; }
			}

			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.beginwex"))
		{
//			BGBCC_JX2_BeginWex(ctx);
			cs2=cs1;
			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}
		
		if(!strcmp(tk0, "I.endwex"))
		{
//			BGBCC_JX2_EndWex(ctx);
			cs2=cs1;
			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.fix32") ||
			!strcmp(tk0, "I.fix32_on"))
		{
			ctx->asm_modefl|=TKUCC_ASMMODE_FIX32;

			cs2=cs1;
			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.endfix32") ||
			!strcmp(tk0, "I.fix32_off"))
		{
			ctx->asm_modefl&=~TKUCC_ASMMODE_FIX32;

			cs2=cs1;
			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.xg2rv") ||
			!strcmp(tk0, "I.xg2rv_on"))
		{
			ctx->asm_modefl|=TKUCC_ASMMODE_XG2RV;

			cs2=cs1;
			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.endxg2rv") ||
			!strcmp(tk0, "I.xg2rv_off"))
		{
			ctx->asm_modefl&=~TKUCC_ASMMODE_XG2RV;

			cs2=cs1;
			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.riscv") ||
			!strcmp(tk0, "I.riscv_on"))
		{
			ctx->asm_modefl|=TKUCC_ASMMODE_RVOPNREGS;

			cs2=cs1;
			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.endriscv") ||
			!strcmp(tk0, "I.riscv_off"))
		{
			ctx->asm_modefl&=~TKUCC_ASMMODE_RVOPNREGS;

			cs2=cs1;
			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}
		
//		ctx->tctx->n_error++;
		printf("unexpected token '%s'\n", tk0+1);
		*rcs=cs;
		return(-1);
	}

//	ctx->tctx->n_error++;
	printf("unexpected token '%s'\n", tk0+1);
	return(-1);
}



void TKUCC_AsmAssembleLine(TKUCC_MainContext *ctx, char *text)
{
	char *s;
	int i;
	
	s=text;
	while(*s)
	{
		i=TKUCC_AsmJx2_ParseOpcode(ctx, &s);
		if(i<0)
		{
			printf("TKUCC_AsmAssembleLine: Fail %s\n", text);
			break;
		}
	}
}


void TKUCC_DoAssembleFile(TKUCC_MainContext *ctx, char *name)
{
	TKUCC_LinkObject *obj;
	TKUCC_LineBuf *line, *lnxt;

	TKUCC_AsmJx2_Init();

	obj=TKUCC_AsmAllocObject(ctx);
	ctx->cur_obj=obj;

	TKUCC_ObjSetSection(ctx, obj, ".text");
	TKUCC_ObjSetSection(ctx, obj, ".data");
	TKUCC_ObjSetSection(ctx, obj, ".bss");
	TKUCC_ObjSetSection(ctx, obj, ".text");

	TKUCC_PpEnterInclude(ctx, name);
	line=TKUCC_PpGetProcessedLines(ctx);
	while(line)
	{
		TKUCC_AsmAssembleLine(ctx, line->text);
		if(line->next)
		{
			lnxt=line->next;
			TKUCC_FreeLine(ctx, line);
			line=lnxt;
			continue;
		}
		TKUCC_FreeLine(ctx, line);
		line=TKUCC_PpGetProcessedLines(ctx);
	}

	ctx->cur_obj=NULL;

	obj->next=ctx->link_inobj;
	ctx->link_inobj=obj;
}
