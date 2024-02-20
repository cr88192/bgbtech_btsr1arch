/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

char *BGBCC_JX2A_EatWhite(char *cs)
{
	return(BGBCP_EatWhite(cs));
}

char *BGBCC_JX2A_EatWhiteNoLinebreak(char *cs)
{
	return(BGBCP_EatWhiteNLb(cs));
}

char *BGBCC_JX2A_EatWhiteOnlyNoLinebreak(char *cs)
{
	return(BGBCP_EatWhiteOnly2(cs));
}

char *BGBCC_JX2A_ParseTokenBasic(char *cs, char *tb)
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

char *BGBCC_JX2A_ParseTokenBasicAltn(char *s, char *tb)
{
	char *t, *te;
	int i, j, k;

	t=tb; te=tb+252;

	s=BGBCC_JX2A_EatWhite(s);
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

	s=BGBCC_JX2A_ParseTokenBasic(s, tb);
	return(s);
}

char *BGBCC_JX2A_ParseToken(char *cs, char **rstr)
{
	char tb[256];
	cs=BGBCC_JX2A_ParseTokenBasic(cs, tb);
	*rstr=bgbcc_rstrdup(tb);
	return(cs);
}

char *BGBCC_JX2A_ParseTokenAlt(char *cs, char **rstr)
{
	char tb[256];
	cs=BGBCC_JX2A_ParseTokenBasicAltn(cs, tb);
	*rstr=bgbcc_rstrdup(tb);
	return(cs);
}

#if 0
char *BGBCC_JX2A_ParseToken(char *cs, char **rtok)
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

char *BGBCC_JX2A_ParseTokenAlt(char *cs, char **rtok)
{
	return(BGBCC_JX2A_ParseToken(cs, rtok));
}
#endif

int BGBCC_JX2A_GetRegId(BGBCC_JX2_Context *ctx, char *str)
{
	int i, t, t1, isdr;

	switch(str[0])
	{
	case 'A':	case 'a':
		if(ctx->emit_riscv&0x33)
		{
			t=BGBCC_SH_REG_R10;

			if(!str[2])
			{
				if((str[1]>='0') && (str[1]<='7'))
					{ return(t+(str[1]-'0')); }
//				if((str[1]=='R') || (str[1]=='r'))
//					return(t);
				break;
			}
			
			if(!stricmp(str, "ar"))
				return(BGBCC_SH_REG_R10);
			if(!stricmp(str, "ar0"))
				return(BGBCC_SH_REG_R10);
			if(!stricmp(str, "ar1"))
				return(BGBCC_SH_REG_R11);
			
		}else
		{
			i=-1;
			if(!str[2])
			{
//				if((str[1]=='R') || (str[1]=='r'))
//					return(BGBCC_SH_REG_R2);

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
				return(BGBCC_SH_REG_R4+(i-0));
			if((i>=4) && (i<8))
				return(BGBCC_SH_REG_R20+(i-4));
			if((i>=8) && (i<12))
				return(BGBCC_SH_REG_R36+(i-8));
			if((i>=12) && (i<16))
				return(BGBCC_SH_REG_R52+(i-12));

			if(!stricmp(str, "ar"))
				return(BGBCC_SH_REG_R2);
			if(!stricmp(str, "ar0"))
				return(BGBCC_SH_REG_R2);
			if(!stricmp(str, "ar1"))
				return(BGBCC_SH_REG_R3);
		}
		break;

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

		if(!bgbcc_stricmp(str, "dlr"))
			return(BGBCC_SH_REG_MACL);
		if(!bgbcc_stricmp(str, "dhr"))
			return(BGBCC_SH_REG_MACH);
		break;

	case 'M':	case 'm':
		if(!bgbcc_stricmp(str, "mach"))
			return(BGBCC_SH_REG_MACH);
		if(!bgbcc_stricmp(str, "macl"))
			return(BGBCC_SH_REG_MACL);
		if(!bgbcc_stricmp(str, "mmcr"))
			return(BGBCC_SH_REG_MMCR);
		break;
	case 'P':	case 'p':
		if(!bgbcc_stricmp(str, "pr"))
			return(BGBCC_SH_REG_PR);
		if(!bgbcc_stricmp(str, "pc"))
			return(BGBCC_SH_REG_PC);
		if(!bgbcc_stricmp(str, "pch"))
			return(BGBCC_SH_REG_PCH);
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
				{
					i=((str[2]-'0')*10)+(str[3]-'0');
					
					if((i>=0) && (i<=31))
						return(t+i);
					if((i>=32) && (i<=63))
						return(t+i);
//						return(BGBCC_SH_REG_R32+(i-32));
				}
			
//				if((str[2]>='0') && (str[2]<='9') &&
//					(str[3]>='0') && (str[3]<='9'))
//						{ return(t+(str[2]-'0')*10+(str[3]-'0')); }
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
//			if((str[1]>='0') && (str[1]<='9') &&
//				(str[2]>='0') && (str[2]<='9'))
//					{ return(t+(str[1]-'0')*10+(str[2]-'0')); }

			if((str[1]>='0') && (str[1]<='9') &&
				(str[2]>='0') && (str[2]<='9'))
			{
				i=((str[1]-'0')*10)+(str[2]-'0');
				
				if((i>=0) && (i<=31))
					return(BGBCC_SH_REG_R0+i);
				if((i>=32) && (i<=63))
					return(BGBCC_SH_REG_R0+i);
//					return(BGBCC_SH_REG_R32+(i-32));
			}
		}

		if(!bgbcc_stricmp(str, "r0u"))
			return(BGBCC_SH_REG_R0U);
		if(!bgbcc_stricmp(str, "r1u"))
			return(BGBCC_SH_REG_R1U);
		if(!bgbcc_stricmp(str, "r15u"))
			return(BGBCC_SH_REG_R15U);

		break;


	case 'X':	case 'x':
		if(ctx->emit_riscv&0x33)
		{
			t=BGBCC_SH_REG_R0;

			if(!str[2])
			{
				if((str[1]>='0') && (str[1]<='9'))
					{ return(t+(str[1]-'0')); }
				break;
			}

			if(!str[3])
			{
	//			if((str[1]>='0') && (str[1]<='9') &&
	//				(str[2]>='0') && (str[2]<='9'))
	//					{ return(t+(str[1]-'0')*10+(str[2]-'0')); }

				if((str[1]>='0') && (str[1]<='9') &&
					(str[2]>='0') && (str[2]<='9'))
				{
					i=((str[1]-'0')*10)+(str[2]-'0');
					
					if((i>=0) && (i<=31))
						return(BGBCC_SH_REG_R0+i);
					if((i>=32) && (i<=63))
						return(BGBCC_SH_REG_R32+(i-32));
				}
			}
		}

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

		t=BGBCC_SH_REG_LR0;

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
		if(ctx->emit_riscv&0x33)
		{
			i=-1;

			if((str[1]>='0') && (str[1]<='9'))
				i=str[1]-0;
			if((str[1]>='0') && (str[1]<='9') &&
				(str[2]>='0') && (str[2]<='9'))
			{
				i=(str[1]-'0')*10+(str[2]-'0');
			}
			
//			if(i>=0)
			if((i>=0) && (i<=11))
			{
				if(i<2)
					return(BGBCC_SH_REG_R8+i);
				return(BGBCC_SH_REG_R18+(i-2));
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
			
//			if(i>=0)
			if((i>=0) && (i<=11))
			{
				if(i<6)
					return(BGBCC_SH_REG_R8+i);
				return(BGBCC_SH_REG_R24+(i-6));
			}
		}

		if(!bgbcc_stricmp(str, "sp"))
			return(BGBCC_SH_REG_SP);

		if(!bgbcc_stricmp(str, "sr"))
			return(BGBCC_SH_REG_SR);

		if(!bgbcc_stricmp(str, "ssp"))
			return(BGBCC_SH_REG_SGR);
		if(!bgbcc_stricmp(str, "ssr"))
			return(BGBCC_SH_REG_SSR);
		if(!bgbcc_stricmp(str, "spc"))
			return(BGBCC_SH_REG_SPC);

		if(!bgbcc_stricmp(str, "sttb"))
			return(BGBCC_SH_REG_STTB);

		break;

	case 'T':	case 't':
		if(ctx->emit_riscv&0x33)
		{
			i=-1;

			if((str[1]>='0') && (str[1]<='9'))
				i=str[1]-0;
			if((str[1]>='0') && (str[1]<='9') &&
				(str[2]>='0') && (str[2]<='9'))
			{
				i=(str[1]-'0')*10+(str[2]-'0');
			}
			
//			if(i>=0)
			if((i>=0) && (i<=6))
			{
				if(i<3)
					return(BGBCC_SH_REG_R5+i);
				return(BGBCC_SH_REG_R28+(i-3));
			}

			if(!bgbcc_stricmp(str, "tp"))
				return(BGBCC_SH_REG_R4);
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
					return(BGBCC_SH_REG_R1+i);
				return(BGBCC_SH_REG_R16+(i-3));
			}
		}

		if(!bgbcc_stricmp(str, "tbr"))
			return(BGBCC_SH_REG_TBR);

		if(!bgbcc_stricmp(str, "ttb"))
			return(BGBCC_SH_REG_TTB);
		if(!bgbcc_stricmp(str, "tea"))
			return(BGBCC_SH_REG_TEA);
		if(!bgbcc_stricmp(str, "teah"))
			return(BGBCC_SH_REG_TEAH);

		break;


	default:
		if(ctx->emit_riscv&0x33)
		{
			if(!bgbcc_stricmp(str, "zero"))
				return(BGBCC_SH_REG_R0);

			if(!bgbcc_stricmp(str, "ra"))
				return(BGBCC_SH_REG_R1);

			if(!bgbcc_stricmp(str, "gp"))
				return(BGBCC_SH_REG_R3);
		}

		if(!bgbcc_stricmp(str, "gbr"))
			return(BGBCC_SH_REG_GBR);
		if(!bgbcc_stricmp(str, "vbr"))
			return(BGBCC_SH_REG_VBR);

		if(!bgbcc_stricmp(str, "lr"))
			return(BGBCC_SH_REG_PR);

		if(!bgbcc_stricmp(str, "exsr"))
			return(BGBCC_SH_REG_EXSR);

		if(!bgbcc_stricmp(str, "krr"))
			return(BGBCC_SH_REG_KRR);

		if(!bgbcc_stricmp(str, "isrsave"))
			return(BGBCC_SH_REG_ISRSAVE);

		if(!bgbcc_stricmp(str, "gbh"))
			return(BGBCC_SH_REG_GBH);
		if(!bgbcc_stricmp(str, "pch"))
			return(BGBCC_SH_REG_PCH);

		if(!bgbcc_stricmp(str, "vipt"))
			return(BGBCC_SH_REG_VIPT);

		break;
	}

	return(-1);
}

static char *bgbcc_jx2a_lastlbl;

int BGBCC_JX2A_ParseOperand_OffsetOf(BGBCC_JX2_Context *ctx,
	char *objn, char *fldn)
{
	BGBCC_CCXL_LiteralInfo *st;
	BGBCC_CCXL_RegisterInfo *fi;
	ccxl_register sreg, dreg;
	ccxl_type bty;
	int fn;
	int i;

	st=BGBCC_CCXL_LookupStructure(ctx->tctx, objn);
	if(!st)
	{
		fprintf(stderr, "BGBCC_JX2A_ParseOperand_OffsetOf: "
			"No struct %s\n", objn);
		BGBCC_CCXL_TagError(ctx->tctx,
			CCXL_TERR_STATUS(CCXL_STATUS_ERR_LOOKUPFAIL));
//		BGBCC_DBGBREAK
		return(0);
	}
	
	fn=BGBCC_CCXL_LookupStructFieldID(ctx->tctx, st, fldn);
	if(fn<0)
	{
		fprintf(stderr, "BGBCC_JX2A_ParseOperand_OffsetOf: "
			"Struct %s no member %s\n", objn, fldn);
		BGBCC_CCXL_TagError(ctx->tctx,
			CCXL_TERR_STATUS(CCXL_STATUS_ERR_LOOKUPFAIL));
		return(0);
	}

	fi=st->decl->fields[fn];

	if(fi->fxmoffs==fi->fxnoffs)
	{
		return(fi->fxmoffs);
	}
	
	BGBCC_DBGBREAK
	return(0);
}

int BGBCC_JX2A_ParseOperand(
	BGBCC_JX2_Context *ctx, char **rcs,
	BGBCC_JX2_OpcodeArg *opv)
{
	char tb0[256];
	char *tk0, *tk1, *tk2, *tk3;
	char *cs, *cs1, *cs2;
	int i, scale;

	cs=*rcs;
	cs=BGBCC_JX2A_EatWhiteNoLinebreak(cs);
	if((*cs=='\r') || (*cs=='\n'))
	{
		cs=BGBCC_JX2A_EatWhite(cs);
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
		cs=BGBCC_JX2A_EatWhite(cs);
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
			cs1=BGBCC_JX2A_ParseTokenAlt(cs, &tk0);

			opv->ty=BGBCC_SH_OPVTY_IMM;
			opv->disp=-bgbcc_atoi(tk0+1);
			*rcs=cs1;
			return(1);
		}
		
		cs1=BGBCC_JX2A_ParseTokenAlt(cs, &tk0);

		opv->ty=BGBCC_SH_OPVTY_IMM;
		opv->disp=bgbcc_atoi(tk0+1);
		*rcs=cs1;
		return(1);
	}

	if((*cs=='@') || (*cs=='('))
	{
		if(*cs=='@')
			cs++;
		while(*cs==' ')cs++;

		if(*cs=='-')
		{
			cs++;

			cs1=BGBCC_JX2A_ParseToken(cs, &tk0);

			if(*tk0=='I')
			{
				opv->ty=BGBCC_SH_OPVTY_DRREGD;
				opv->breg=BGBCC_JX2A_GetRegId(ctx, tk0+1);
				*rcs=cs1;
				return(1);
			}
			return(-1);
		}

		if(*cs=='(')
		{
			cs++;
			cs=BGBCC_JX2A_ParseToken(cs, &tk0);
			tk1=NULL;
			tk2=NULL;
			tk3=NULL;
			scale=0;

			cs=BGBCC_JX2A_EatWhiteNoLinebreak(cs);
			if(*cs=='*')
			{
				cs++;
				cs=BGBCC_JX2A_ParseTokenAlt(cs, &cs2);
				scale=atoi(cs2+1);
			}

			cs=BGBCC_JX2A_EatWhiteNoLinebreak(cs);
			if(*cs==',')
			{
				cs++;
				cs=BGBCC_JX2A_ParseToken(cs, &tk1);
			}

			if(tk1 && !strcmp(tk1, "Ioffsetof"))
			{
				cs=BGBCC_JX2A_ParseTokenAlt(cs, &tk2);
				cs=BGBCC_JX2A_ParseTokenAlt(cs, &tk3);
			}
			
			cs=BGBCC_JX2A_EatWhiteNoLinebreak(cs);
			if(*cs=='*')
			{
				cs++;
				cs=BGBCC_JX2A_ParseTokenAlt(cs, &cs2);
				scale=atoi(cs2+1);
			}

			cs=BGBCC_JX2A_EatWhiteNoLinebreak(cs);
			if(*cs==')')
				cs++;

			if(tk1)
			{
				if((*tk0=='|') && (*tk1=='I'))
				{
					opv->ty=BGBCC_SH_OPVTY_RDMEM;
					opv->breg=BGBCC_JX2A_GetRegId(ctx, tk1+1);
					opv->disp=bgbcc_atoi(tk0+1);
					if(scale>0)
						opv->disp*=scale;
					*rcs=cs;
					return(1);
				}
				
				if(*tk1=='I')
				{
					if(!strcmp(tk1, "Ioffsetof"))
					{
						opv->ty=BGBCC_SH_OPVTY_RDMEM;
						opv->breg=BGBCC_JX2A_GetRegId(ctx, tk0+1);
//						opv->disp=bgbcc_atoi(tk1+1);
						opv->disp=BGBCC_JX2A_ParseOperand_OffsetOf(
							ctx, tk2+1, tk3+1);
						*rcs=cs;
						return(1);
					}

					opv->ty=BGBCC_SH_OPVTY_RRMEM;
//					opv->rb=BGBCC_JX2A_GetRegId(tk0+1);
//					opv->ri=BGBCC_JX2A_GetRegId(tk1+1);

//					opv->ireg=BGBCC_JX2A_GetRegId(tk0+1);
//					opv->breg=BGBCC_JX2A_GetRegId(tk1+1);

					opv->breg=BGBCC_JX2A_GetRegId(ctx, tk0+1);
					opv->ireg=BGBCC_JX2A_GetRegId(ctx, tk1+1);
					opv->sc=scale;

					*rcs=cs;
					return(1);
				}
				if(*tk1=='|')
				{
					opv->ty=BGBCC_SH_OPVTY_RDMEM;
					opv->breg=BGBCC_JX2A_GetRegId(ctx, tk0+1);
					opv->disp=bgbcc_atoi(tk1+1);
					if(scale>0)
						opv->disp*=scale;
					*rcs=cs;
					return(1);
				}
				return(-1);
			}

			opv->ty=BGBCC_SH_OPVTY_DRREG;
			opv->breg=BGBCC_JX2A_GetRegId(ctx, tk0+1);
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

		cs1=BGBCC_JX2A_ParseToken(cs, &tk0);

		if(*tk0=='I')
		{
			opv->ty=BGBCC_SH_OPVTY_DRREG;
			opv->breg=BGBCC_JX2A_GetRegId(ctx, tk0+1);
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

	cs1=BGBCC_JX2A_ParseTokenAlt(cs, &tk0);
	cs2=BGBCC_JX2A_ParseToken(cs1, &tk1);

	if(*tk0=='|')
	{
		opv->ty=BGBCC_SH_OPVTY_IMM;
		opv->disp=bgbcc_atoi(tk0+1);
		*rcs=cs1;
		return(1);
	}

	if(*tk0=='I')
	{
		i=BGBCC_JX2A_GetRegId(ctx, tk0+1);
		if(i>=0)
		{
			opv->ty=BGBCC_SH_OPVTY_REG;
			opv->breg=i;
			*rcs=cs1;
			return(1);
		}

		if((tk0[1]=='.') && bgbcc_jx2a_lastlbl)
		{
			sprintf(tb0, "%s%s", bgbcc_jx2a_lastlbl, tk0+1);

			opv->ty=BGBCC_SH_OPVTY_NAME;
			if(strlen(tb0)<8)
			{
				opv->name=(char *)(&opv->disp);
				strncpy(opv->name, tb0, 7);
			}else
			{
				opv->name=bgbcc_strdup(tb0);
			}
		}else
		{
//			bgbcc_jx2a_lastlbl=bgbcc_strdup(tk0+1);
		
			opv->ty=BGBCC_SH_OPVTY_NAME;
			if(strlen(tk0+1)<8)
			{
				opv->disp=0;
				opv->name=(char *)(&opv->disp);
//				strncpy(opv->name, tk0+1, 7);
				strncpy(opv->name, tk0+1, 8);
			}else
			{
				opv->name=bgbcc_strdup(tk0+1);
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
//{"",		BGBCC_SH_NMID_UNK},
{"mov",		BGBCC_SH_NMID_MOV},
{"mov.b",	BGBCC_SH_NMID_MOVB},
{"mov.w",	BGBCC_SH_NMID_MOVW},
{"mov.l",	BGBCC_SH_NMID_MOVL},
{"mov.q",	BGBCC_SH_NMID_MOVQ},
{"mov.p",	BGBCC_SH_NMID_MOVP},

{"movu.b",	BGBCC_SH_NMID_MOVUB},
{"movu.w",	BGBCC_SH_NMID_MOVUW},
{"movu.l",	BGBCC_SH_NMID_MOVUL},

{"lea.b",	BGBCC_SH_NMID_LEAB},
{"lea.w",	BGBCC_SH_NMID_LEAW},
{"lea.l",	BGBCC_SH_NMID_LEAL},
{"lea.q",	BGBCC_SH_NMID_LEAQ},
{"lea.p",	BGBCC_SH_NMID_LEAP},

{"xmov.b",	BGBCC_SH_NMID_XMOVB},
{"xmov.w",	BGBCC_SH_NMID_XMOVW},
{"xmov.l",	BGBCC_SH_NMID_XMOVL},
{"xmov.q",	BGBCC_SH_NMID_XMOVQ},
{"xmov.p",	BGBCC_SH_NMID_XMOVP},

{"xmovu.b",	BGBCC_SH_NMID_XMOVUB},
{"xmovu.w",	BGBCC_SH_NMID_XMOVUW},
{"xmovu.l",	BGBCC_SH_NMID_XMOVUL},
{"xmov.x",	BGBCC_SH_NMID_XMOVX2},

{"xlea.b",	BGBCC_SH_NMID_XLEAB},
{"xlea.w",	BGBCC_SH_NMID_XLEAW},
{"xlea.l",	BGBCC_SH_NMID_XLEAL},
{"xlea.q",	BGBCC_SH_NMID_XLEAQ},

{"leat.b",	BGBCC_SH_NMID_LEATB},
{"leat.w",	BGBCC_SH_NMID_LEATW},
{"leat.l",	BGBCC_SH_NMID_LEATL},
{"leat.q",	BGBCC_SH_NMID_LEATQ},

{"add",		BGBCC_SH_NMID_ADD},
{"addc",	BGBCC_SH_NMID_ADDC},
{"addv",	BGBCC_SH_NMID_ADDV},
{"sub",		BGBCC_SH_NMID_SUB},
{"subc",	BGBCC_SH_NMID_SUBC},
{"subv",	BGBCC_SH_NMID_SUBV},
{"adc",		BGBCC_SH_NMID_ADDC},
{"sbb",		BGBCC_SH_NMID_SUBC},
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

{"cmphs",	BGBCC_SH_NMID_CMPHS},
{"cmphi",	BGBCC_SH_NMID_CMPHI},
{"cmpeq",	BGBCC_SH_NMID_CMPEQ},
{"cmpge",	BGBCC_SH_NMID_CMPGE},
{"cmpgt",	BGBCC_SH_NMID_CMPGT},
{"cmpqhs",	BGBCC_SH_NMID_CMPQHS},
{"cmpqhi",	BGBCC_SH_NMID_CMPQHI},
{"cmpqeq",	BGBCC_SH_NMID_CMPQEQ},
{"cmpqge",	BGBCC_SH_NMID_CMPQGE},
{"cmpqgt",	BGBCC_SH_NMID_CMPQGT},

{"cmphs.l",	BGBCC_SH_NMID_CMPHS},
{"cmphi.l",	BGBCC_SH_NMID_CMPHI},
{"cmpeq.l",	BGBCC_SH_NMID_CMPEQ},
{"cmpge.l",	BGBCC_SH_NMID_CMPGE},
{"cmpgt.l",	BGBCC_SH_NMID_CMPGT},
{"cmphs.q",	BGBCC_SH_NMID_CMPQHS},
{"cmphi.q",	BGBCC_SH_NMID_CMPQHI},
{"cmpeq.q",	BGBCC_SH_NMID_CMPQEQ},
{"cmpge.q",	BGBCC_SH_NMID_CMPQGE},
{"cmpgt.q",	BGBCC_SH_NMID_CMPQGT},

{"cmppeq",	BGBCC_SH_NMID_CMPPEQ},
{"cmppge",	BGBCC_SH_NMID_CMPPGE},
{"cmppgt",	BGBCC_SH_NMID_CMPPGT},

{"cmpeq.p",	BGBCC_SH_NMID_CMPPEQ},
{"cmpge.p",	BGBCC_SH_NMID_CMPPGE},
{"cmpgt.p",	BGBCC_SH_NMID_CMPPGT},

{"cmtteq",	BGBCC_SH_NMID_CMPTTEQ},
{"cmtaeq",	BGBCC_SH_NMID_CMPTAEQ},
{"cmtahi",	BGBCC_SH_NMID_CMPTAHI},
{"cmtahs",	BGBCC_SH_NMID_CMPTAHS},

// {"cmppeq",	BGBCC_SH_NMID_CMPPEQ},
// {"cmppgt",	BGBCC_SH_NMID_CMPPGT},
{"cmppeqx",	BGBCC_SH_NMID_CMPPEQX},
{"cmppgtx",	BGBCC_SH_NMID_CMPPGTX},

{"jmp",		BGBCC_SH_NMID_JMP},
{"jsr",		BGBCC_SH_NMID_JSR},
{"bra",		BGBCC_SH_NMID_BRA},
{"bsr",		BGBCC_SH_NMID_BSR},
{"bt",		BGBCC_SH_NMID_BT},
{"bf",		BGBCC_SH_NMID_BF},
{"bt/s",	BGBCC_SH_NMID_BTS},
{"bf/s",	BGBCC_SH_NMID_BFS},
{"div1",	BGBCC_SH_NMID_DIV1},

{"bra.b",	BGBCC_SH_NMID_BRAB},
{"bsr.b",	BGBCC_SH_NMID_BSRB},
{"bt.b",	BGBCC_SH_NMID_BTB},
{"bf.b",	BGBCC_SH_NMID_BFB},

{"bra.l",	BGBCC_SH_NMID_BRAL},
{"bsr.l",	BGBCC_SH_NMID_BSRL},
{"bt.l",	BGBCC_SH_NMID_BTL},
{"bf.l",	BGBCC_SH_NMID_BFL},

{"vskg",	BGBCC_SH_NMID_VSKG},
{"vskc",	BGBCC_SH_NMID_VSKC},
{"bndchk",	BGBCC_SH_NMID_BNDCHK},
{"bndcmp",	BGBCC_SH_NMID_BNDCMP},

{"bndchk.b",	BGBCC_SH_NMID_BNDCHKB},
{"bndchk.w",	BGBCC_SH_NMID_BNDCHKW},
{"bndchk.l",	BGBCC_SH_NMID_BNDCHKL},
{"bndchk.q",	BGBCC_SH_NMID_BNDCHKQ},

{"bndcmp.b",	BGBCC_SH_NMID_BNDCMPB},
{"bndcmp.q",	BGBCC_SH_NMID_BNDCMPW},
{"bndcmp.l",	BGBCC_SH_NMID_BNDCMPL},
{"bndcmp.q",	BGBCC_SH_NMID_BNDCMPQ},

{"breq",	BGBCC_SH_NMID_BREQ},
{"brne",	BGBCC_SH_NMID_BRNE},

{"brlt",	BGBCC_SH_NMID_BRLT},
{"brge",	BGBCC_SH_NMID_BRGE},
{"brgt",	BGBCC_SH_NMID_BRGT},
{"brle",	BGBCC_SH_NMID_BRLE},

{"brltu",	BGBCC_SH_NMID_BRLTU},
{"brgeu",	BGBCC_SH_NMID_BRGEU},
{"brgtu",	BGBCC_SH_NMID_BRGTU},
{"brleu",	BGBCC_SH_NMID_BRLEU},

{"brz",		BGBCC_SH_NMID_BREQ},
{"brnz",	BGBCC_SH_NMID_BRNE},

{"breq.q",	BGBCC_SH_NMID_BREQ},
{"brne.q",	BGBCC_SH_NMID_BRNE},

{"brlt.q",	BGBCC_SH_NMID_BRLT},
{"brge.q",	BGBCC_SH_NMID_BRGE},
{"brgt.q",	BGBCC_SH_NMID_BRGT},
{"brle.q",	BGBCC_SH_NMID_BRLE},

{"breq.l",	BGBCC_SH_NMID_BREQL},
{"brne.l",	BGBCC_SH_NMID_BRNEL},

{"brlt.l",	BGBCC_SH_NMID_BRLTL},
{"brge.l",	BGBCC_SH_NMID_BRGEL},
{"brgt.l",	BGBCC_SH_NMID_BRGTL},
{"brle.l",	BGBCC_SH_NMID_BRLEL},

{"divs.q",	BGBCC_SH_NMID_DIVSQ},
{"divu.q",	BGBCC_SH_NMID_DIVUQ},
{"mods.q",	BGBCC_SH_NMID_MODSQ},
{"modu.q",	BGBCC_SH_NMID_MODUQ},

{"divs.l",	BGBCC_SH_NMID_DIVSL},
{"divu.l",	BGBCC_SH_NMID_DIVUL},
{"mods.l",	BGBCC_SH_NMID_MODSL},
{"modu.l",	BGBCC_SH_NMID_MODUL},

{"bcdadc",	BGBCC_SH_NMID_BCDADC},
{"bcdsbb",	BGBCC_SH_NMID_BCDSBB},

{"bcdadd",	BGBCC_SH_NMID_BCDADD},
{"bcdsub",	BGBCC_SH_NMID_BCDSUB},
{"bcdaddx",	BGBCC_SH_NMID_BCDADDX},
{"bcdsubx",	BGBCC_SH_NMID_BCDSUBX},

#if 0
{"jcmpeq",	BGBCC_SH_NMID_BREQL},
{"jcmpne",	BGBCC_SH_NMID_BRNEL},

{"jcmplt",	BGBCC_SH_NMID_BRLTL},
{"jcmpge",	BGBCC_SH_NMID_BRGEL},
{"jcmpgt",	BGBCC_SH_NMID_BRGTL},
{"jcmple",	BGBCC_SH_NMID_BRLEL},

{"jcmpltu",		BGBCC_SH_NMID_BRLTU},
{"jcmpgeu",		BGBCC_SH_NMID_BRGEU},
{"jcmpgtu",		BGBCC_SH_NMID_BRGTU},
{"jcmpleu",		BGBCC_SH_NMID_BRLEU},

{"jcmpqeq",		BGBCC_SH_NMID_BREQ},
{"jcmpqne",		BGBCC_SH_NMID_BRNE},

{"jcmpqlt",		BGBCC_SH_NMID_BRLT},
{"jcmpqge",		BGBCC_SH_NMID_BRGE},
{"jcmpqgt",		BGBCC_SH_NMID_BRGT},
{"jcmpqle",		BGBCC_SH_NMID_BRLE},

{"jcmpqltu",	BGBCC_SH_NMID_BRLTU},
{"jcmpqgeu",	BGBCC_SH_NMID_BRGEU},
{"jcmpqgtu",	BGBCC_SH_NMID_BRGTU},
{"jcmpqleu",	BGBCC_SH_NMID_BRLEU},
#endif

{"dmulu",	BGBCC_SH_NMID_MULUL},
{"dmuls",	BGBCC_SH_NMID_MULSL},
{"dmulu.l",	BGBCC_SH_NMID_MULUL},
{"dmuls.l",	BGBCC_SH_NMID_MULSL},
{"mulu",	BGBCC_SH_NMID_DMULU},
{"muls",	BGBCC_SH_NMID_DMULS},
{"mulu.l",	BGBCC_SH_NMID_DMULU},
{"muls.l",	BGBCC_SH_NMID_DMULS},

{"mulu.w",	BGBCC_SH_NMID_MULUW},
{"muls.w",	BGBCC_SH_NMID_MULSW},
{"dmulu.w",	BGBCC_SH_NMID_MULUW},
{"dmuls.w",	BGBCC_SH_NMID_MULSW},

{"tst",		BGBCC_SH_NMID_TST},
{"test",	BGBCC_SH_NMID_TST},
{"tstq",	BGBCC_SH_NMID_TSTQ},
{"test.q",	BGBCC_SH_NMID_TSTQ},
{"and",		BGBCC_SH_NMID_AND},
{"xor",		BGBCC_SH_NMID_XOR},
{"or",		BGBCC_SH_NMID_OR},
{"div0s",	BGBCC_SH_NMID_DIV0S},
{"cmp/str",	BGBCC_SH_NMID_CMPSTR},
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
{"break",	BGBCC_SH_NMID_BRK},
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
{"rotr",	BGBCC_SH_NMID_ROTR},

{"nop4b",	BGBCC_SH_NMID_NOP4B},

{"rotcl.l",	BGBCC_SH_NMID_ROTCL},
{"rotcr.l",	BGBCC_SH_NMID_ROTCR},
{"rotcl.q",	BGBCC_SH_NMID_ROTCLQ},
{"rotcr.q",	BGBCC_SH_NMID_ROTCRQ},

{"cmp/pl",	BGBCC_SH_NMID_CMPPL},

{"shll1",	BGBCC_SH_NMID_SHLL1},
{"shlr1",	BGBCC_SH_NMID_SHLR1},
{"shar1",	BGBCC_SH_NMID_SHAR1},
{"shll2",	BGBCC_SH_NMID_SHLL2},
{"shlr2",	BGBCC_SH_NMID_SHLR2},
{"shar2",	BGBCC_SH_NMID_SHAR2},
{"shll4",	BGBCC_SH_NMID_SHLL4},
{"shlr4",	BGBCC_SH_NMID_SHLR4},
{"shar4",	BGBCC_SH_NMID_SHAR4},
{"shll8",	BGBCC_SH_NMID_SHLL8},
{"shlr8",	BGBCC_SH_NMID_SHLR8},
{"shar8",	BGBCC_SH_NMID_SHAR8},
{"shll16",	BGBCC_SH_NMID_SHLL16},
{"shlr16",	BGBCC_SH_NMID_SHLR16},
{"shar16",	BGBCC_SH_NMID_SHAR16},
{"shll32",	BGBCC_SH_NMID_SHLL32},
{"shlr32",	BGBCC_SH_NMID_SHLR32},
{"shar32",	BGBCC_SH_NMID_SHAR32},
{"movnt",	BGBCC_SH_NMID_MOVNT},
{"cselt",	BGBCC_SH_NMID_CSELT},
{"clz",		BGBCC_SH_NMID_CLZ},
{"clzq",	BGBCC_SH_NMID_CLZQ},
{"ctz",		BGBCC_SH_NMID_CTZ},
{"ctzq",	BGBCC_SH_NMID_CTZQ},
{"btrns",	BGBCC_SH_NMID_BTRNS},
{"btrnsq",	BGBCC_SH_NMID_BTRNSQ},

{"clz.q",	BGBCC_SH_NMID_CLZQ},
{"ctz.q",	BGBCC_SH_NMID_CTZQ},
{"btrns.q",	BGBCC_SH_NMID_BTRNSQ},

{"pmortl",	BGBCC_SH_NMID_PMORTL},
{"pmortq",	BGBCC_SH_NMID_PMORTQ},
{"pmort.l",	BGBCC_SH_NMID_PMORTL},
{"pmort.q",	BGBCC_SH_NMID_PMORTQ},

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
{"div0",	BGBCC_SH_NMID_DIV0U},
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
{"and.w",	BGBCC_SH_NMID_ANDW},
{"xor.w",	BGBCC_SH_NMID_XORW},
{"or.w",	BGBCC_SH_NMID_ORW},
{"and.l",	BGBCC_SH_NMID_ANDL},
{"xor.l",	BGBCC_SH_NMID_XORL},
{"or.l",	BGBCC_SH_NMID_ORL},
{"and.q",	BGBCC_SH_NMID_AND},
{"xor.q",	BGBCC_SH_NMID_XOR},
{"or.q",	BGBCC_SH_NMID_OR},

{"add.b",	BGBCC_SH_NMID_ADDSB},
{"adds.b",	BGBCC_SH_NMID_ADDSB},
{"addu.b",	BGBCC_SH_NMID_ADDUB},
{"sub.b",	BGBCC_SH_NMID_SUBSB},
{"subs.b",	BGBCC_SH_NMID_SUBSB},
{"subu.b",	BGBCC_SH_NMID_SUBUB},
{"rsub.b",	BGBCC_SH_NMID_RSUBSB},
{"rsubs.b",	BGBCC_SH_NMID_RSUBSB},
{"rsubu.b",	BGBCC_SH_NMID_RSUBUB},

{"add.w",	BGBCC_SH_NMID_ADDSW},
{"adds.w",	BGBCC_SH_NMID_ADDSW},
{"addu.w",	BGBCC_SH_NMID_ADDUW},
{"sub.w",	BGBCC_SH_NMID_SUBSW},
{"subs.w",	BGBCC_SH_NMID_SUBSW},
{"subu.w",	BGBCC_SH_NMID_SUBUW},
{"rsub.w",	BGBCC_SH_NMID_RSUBSW},
{"rsubs.w",	BGBCC_SH_NMID_RSUBSW},
{"rsubu.w",	BGBCC_SH_NMID_RSUBUW},

{"add.l",	BGBCC_SH_NMID_ADDSL},
{"adds.l",	BGBCC_SH_NMID_ADDSL},
{"addu.l",	BGBCC_SH_NMID_ADDUL},
{"sub.l",	BGBCC_SH_NMID_SUBSL},
{"subs.l",	BGBCC_SH_NMID_SUBSL},
{"subu.l",	BGBCC_SH_NMID_SUBUL},
{"rsub.l",	BGBCC_SH_NMID_RSUBSL},
{"rsubs.l",	BGBCC_SH_NMID_RSUBSL},
{"rsubu.l",	BGBCC_SH_NMID_RSUBUL},

{"add.q",	BGBCC_SH_NMID_ADD},
{"sub.q",	BGBCC_SH_NMID_SUB},
{"rsub.q",	BGBCC_SH_NMID_RSUB},

{"sub.p",	BGBCC_SH_NMID_SUBP},
{"subx.p",	BGBCC_SH_NMID_SUBXP},

{"shad",	BGBCC_SH_NMID_SHAD},
{"shld",	BGBCC_SH_NMID_SHLD},
{"clrs",	BGBCC_SH_NMID_CLRS},
{"sets",	BGBCC_SH_NMID_SETS},
{"nott",	BGBCC_SH_NMID_NOTT},
{"brk",		BGBCC_SH_NMID_BRK},
{"movrt",	BGBCC_SH_NMID_MOVRT},
{"movca.l",	BGBCC_SH_NMID_MOVCAL},

{"shad.l",	BGBCC_SH_NMID_SHAD},
{"shld.l",	BGBCC_SH_NMID_SHLD},
{"shad.q",	BGBCC_SH_NMID_SHADQ},
{"shld.q",	BGBCC_SH_NMID_SHLDQ},

{"push",	BGBCC_SH_NMID_PUSH},
{"pop",		BGBCC_SH_NMID_POP},
{"bra2b",	BGBCC_SH_NMID_BRA2B},
{"bra4b",	BGBCC_SH_NMID_BRA4B},
{"bra8b",	BGBCC_SH_NMID_BRA8B},
{"ret",		BGBCC_SH_NMID_RET},
{"rtsu",	BGBCC_SH_NMID_RTSU},
{"syscall",	BGBCC_SH_NMID_SYSCALL},

{"push.x",	BGBCC_SH_NMID_PUSHX2},
{"pop.x",	BGBCC_SH_NMID_POPX2},
{"mov.x",	BGBCC_SH_NMID_MOVX2},
{"movx",	BGBCC_SH_NMID_XMOV},

{"movc",	BGBCC_SH_NMID_MOVC},
{"mov.c",	BGBCC_SH_NMID_MOVC},

{"ldtlb",	BGBCC_SH_NMID_LDTLB},
{"invic",	BGBCC_SH_NMID_INVIC},
{"invdc",	BGBCC_SH_NMID_INVDC},
{"invtlb",	BGBCC_SH_NMID_INVTLB},
{"ldacl",	BGBCC_SH_NMID_LDACL},

{"ldekrr",	BGBCC_SH_NMID_LDEKRR},
{"svekrr",	BGBCC_SH_NMID_SVEKRR},
{"ldekey",	BGBCC_SH_NMID_LDEKEY},
{"ldeenc",	BGBCC_SH_NMID_LDEENC},

{"wexmd",	BGBCC_SH_NMID_WEXMD},
{"cpuid",	BGBCC_SH_NMID_CPUID},

{"jldi",	BGBCC_SH_NMID_JLDI},

{"ldish8",	BGBCC_SH_NMID_LDSH8},
{"ldish16",	BGBCC_SH_NMID_LDSH16},
{"ldsh8",	BGBCC_SH_NMID_LDSH8},
{"ldsh16",	BGBCC_SH_NMID_LDSH16},
{"shori",	BGBCC_SH_NMID_LDSH16},

{"fabs",	BGBCC_SH_NMID_FABS},
{"fadd",	BGBCC_SH_NMID_FADD},
{"fcmp/eq",	BGBCC_SH_NMID_FCMPEQ},
{"fcmp/gt",	BGBCC_SH_NMID_FCMPGT},
{"fcmpeq",	BGBCC_SH_NMID_FCMPEQ},
{"fcmpgt",	BGBCC_SH_NMID_FCMPGT},
{"fcnvds",	BGBCC_SH_NMID_FCNVDS},
{"fcnvsd",	BGBCC_SH_NMID_FCNVSD},
{"fdiv",	BGBCC_SH_NMID_FDIV},
{"fdivx",	BGBCC_SH_NMID_FDIVX},
{"fldi0",	BGBCC_SH_NMID_FLDI0},
{"fldi1",	BGBCC_SH_NMID_FLDI1},
{"flds",	BGBCC_SH_NMID_FLDS},
{"float",	BGBCC_SH_NMID_FLOAT},
{"fmac",	BGBCC_SH_NMID_FMAC},
{"fmov",	BGBCC_SH_NMID_FMOV},
{"fmov.s",	BGBCC_SH_NMID_FMOVS},
{"fmov.f",	BGBCC_SH_NMID_FMOVS},
{"fmov.d",	BGBCC_SH_NMID_FMOVD},
{"fmov.h",	BGBCC_SH_NMID_FMOVH},
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

{"faddx",	BGBCC_SH_NMID_FADDX},
{"fsubx",	BGBCC_SH_NMID_FSUBX},
{"fmulx",	BGBCC_SH_NMID_FMULX},
{"fmacx",	BGBCC_SH_NMID_FMACX},
{"fcmpxeq",	BGBCC_SH_NMID_FCMPXEQ},
{"fcmpxgt",	BGBCC_SH_NMID_FCMPXGT},

{"faddg",	BGBCC_SH_NMID_FADDG},
{"fsubg",	BGBCC_SH_NMID_FSUBG},
{"fmulg",	BGBCC_SH_NMID_FMULG},

{"fadd.g",	BGBCC_SH_NMID_FADDG},
{"fsub.g",	BGBCC_SH_NMID_FSUBG},
{"fmul.g",	BGBCC_SH_NMID_FMULG},

{"cmpxeq",	BGBCC_SH_NMID_CMPXEQ},
{"cmpxgt",	BGBCC_SH_NMID_CMPXGT},
{"addx",	BGBCC_SH_NMID_ADDX},
{"subx",	BGBCC_SH_NMID_SUBX},
{"adcx",	BGBCC_SH_NMID_ADCX},
{"sbbx",	BGBCC_SH_NMID_SBBX},

{"rotlx",	BGBCC_SH_NMID_ROTLX},
{"shadx",	BGBCC_SH_NMID_SHADX},
{"shldx",	BGBCC_SH_NMID_SHLDX},
{"sharx",	BGBCC_SH_NMID_SHARX},
{"shlrx",	BGBCC_SH_NMID_SHLRX},

{"shad.x",	BGBCC_SH_NMID_SHADX},
{"shld.x",	BGBCC_SH_NMID_SHLDX},
{"shar.x",	BGBCC_SH_NMID_SHARX},
{"shlr.x",	BGBCC_SH_NMID_SHLRX},

{"blkutx1",		BGBCC_SH_NMID_BLKUTX1},
{"blkutx2",		BGBCC_SH_NMID_BLKUTX2},
{"blkutx3h",	BGBCC_SH_NMID_BLKUTX3H},
{"blkutx3l",	BGBCC_SH_NMID_BLKUTX3L},
{"blint.w",		BGBCC_SH_NMID_BLINTW},
{"blerp.w",		BGBCC_SH_NMID_BLERPW},
{"blinta.w",	BGBCC_SH_NMID_BLINTAW},

{"blkuab1",		BGBCC_SH_NMID_BLKUAB1},
{"blkuab2",		BGBCC_SH_NMID_BLKUAB2},

{"ldtex",		BGBCC_SH_NMID_LDTEX},

{"convfxi",		BGBCC_SH_NMID_CONVFXI},
{"convfli",		BGBCC_SH_NMID_CONVFLI},
{"snipe.dc",	BGBCC_SH_NMID_SNIPEDC},
{"snipe.ic",	BGBCC_SH_NMID_SNIPEIC},

{"movtt",		BGBCC_SH_NMID_MOVTT},
{"xmovtt",		BGBCC_SH_NMID_XMOVTT},
{"movzt",		BGBCC_SH_NMID_MOVZT},
{"xmovzt",		BGBCC_SH_NMID_XMOVZT},
{"movst",		BGBCC_SH_NMID_MOVST},
{"xmovst",		BGBCC_SH_NMID_XMOVST},


{"pscheq.w",	BGBCC_SH_NMID_PSCHEQW},
{"pscheq.b",	BGBCC_SH_NMID_PSCHEQB},
{"pschne.w",	BGBCC_SH_NMID_PSCHNEW},
{"pschne.b",	BGBCC_SH_NMID_PSCHNEB},

{"frcp",	BGBCC_SH_NMID_FRCP},
{"fsqrta",	BGBCC_SH_NMID_FSQRTA},
{"frcpa",	BGBCC_SH_NMID_FRCPA},

{"fldcf",	BGBCC_SH_NMID_FLDCF},
{"fldcd",	BGBCC_SH_NMID_FLDCD},
{"fldci",	BGBCC_SH_NMID_FLDCI},
{"fldch",	BGBCC_SH_NMID_FLDCH},
{"fstcf",	BGBCC_SH_NMID_FSTCF},
{"fstcd",	BGBCC_SH_NMID_FSTCD},
{"fstci",	BGBCC_SH_NMID_FSTCI},
{"fstch",	BGBCC_SH_NMID_FSTCH},

{"fldcfh",	BGBCC_SH_NMID_FLDCFH},
{"fstcfh",	BGBCC_SH_NMID_FSTCFH},

{"fldcdx",	BGBCC_SH_NMID_FLDCDX},
{"fstcdx",	BGBCC_SH_NMID_FSTCDX},
{"fldcix",	BGBCC_SH_NMID_FLDCIX},
{"fstcix",	BGBCC_SH_NMID_FSTCIX},

{"pldch",	BGBCC_SH_NMID_PLDCH},
{"pldchl",	BGBCC_SH_NMID_PLDCH},
{"pstch",	BGBCC_SH_NMID_PSTCH},
{"pldchh",	BGBCC_SH_NMID_PLDCHH},
// {"pstchh",	BGBCC_SH_NMID_FSTCFH},

{"pldcehl",	BGBCC_SH_NMID_PLDCEHL},
{"pldcehh",	BGBCC_SH_NMID_PLDCEHH},

{"pldcxh",	BGBCC_SH_NMID_PLDCXH},
{"pstcxh",	BGBCC_SH_NMID_PSTCXH},

{"keybrk_xg2",	BGBCC_SH_NMID_KEYBRK_XG2},
{"keybrk_base",	BGBCC_SH_NMID_KEYBRK_BASE},


{"movd.l",	BGBCC_SH_NMID_MOVDL},
{"movd",	BGBCC_SH_NMID_MOVD},
{"movhd",	BGBCC_SH_NMID_MOVHD},
{"movld",	BGBCC_SH_NMID_MOVLD},
{"movhhd",	BGBCC_SH_NMID_MOVHD},
{"movlld",	BGBCC_SH_NMID_MOVLD},
{"movhld",	BGBCC_SH_NMID_MOVHLD},
{"movlhd",	BGBCC_SH_NMID_MOVLHD},

{"cmovt.b",		BGBCC_SH_NMID_CMOVTB},
{"cmovt.w",		BGBCC_SH_NMID_CMOVTW},
{"cmovt.l",		BGBCC_SH_NMID_CMOVTL},
{"cmovt.q",		BGBCC_SH_NMID_CMOVTQ},
{"cmovf.b",		BGBCC_SH_NMID_CMOVFB},
{"cmovf.w",		BGBCC_SH_NMID_CMOVFW},
{"cmovf.l",		BGBCC_SH_NMID_CMOVFL},
{"cmovf.q",		BGBCC_SH_NMID_CMOVFQ},
{"cmovut.b",	BGBCC_SH_NMID_CMOVUTB},
{"cmovut.w",	BGBCC_SH_NMID_CMOVUTW},
{"cmovut.l",	BGBCC_SH_NMID_CMOVUTL},
{"cmovdt.l",	BGBCC_SH_NMID_CMOVDTL},
{"cmovuf.b",	BGBCC_SH_NMID_CMOVUFB},
{"cmovuf.w",	BGBCC_SH_NMID_CMOVUFW},
{"cmovuf.l",	BGBCC_SH_NMID_CMOVUFL},
{"cmovdf.l",	BGBCC_SH_NMID_CMOVDFL},

{"padd.w",		BGBCC_SH_NMID_PADDW},
{"padd.l",		BGBCC_SH_NMID_PADDL},
{"psub.w",		BGBCC_SH_NMID_PSUBW},
{"psub.l",		BGBCC_SH_NMID_PSUBL},
{"padd.h",		BGBCC_SH_NMID_PADDH},
{"psub.h",		BGBCC_SH_NMID_PSUBH},
{"pmul.h",		BGBCC_SH_NMID_PMULH},

{"padd.f",		BGBCC_SH_NMID_PADDF},
{"psub.f",		BGBCC_SH_NMID_PSUBF},
{"pmul.f",		BGBCC_SH_NMID_PMULF},

{"paddx.f",		BGBCC_SH_NMID_PADDFX},
{"psubx.f",		BGBCC_SH_NMID_PSUBFX},
{"pmulx.f",		BGBCC_SH_NMID_PMULFX},
{"paddx.d",		BGBCC_SH_NMID_PADDXD},
{"psubx.d",		BGBCC_SH_NMID_PSUBXD},
{"pmulx.d",		BGBCC_SH_NMID_PMULXD},

{"padd.fa",		BGBCC_SH_NMID_PADDFA},
{"psub.fa",		BGBCC_SH_NMID_PSUBFA},
{"pmul.fa",		BGBCC_SH_NMID_PMULFA},
{"paddx.fa",	BGBCC_SH_NMID_PADDFAX},
{"psubx.fa",	BGBCC_SH_NMID_PSUBFAX},
{"pmulx.fa",	BGBCC_SH_NMID_PMULFAX},

{"fadda",		BGBCC_SH_NMID_FADDA},
{"fsuba",		BGBCC_SH_NMID_FSUBA},
{"fmula",		BGBCC_SH_NMID_FMULA},
{"paddx.da",	BGBCC_SH_NMID_PADDXDA},
{"psubx.da",	BGBCC_SH_NMID_PSUBXDA},
{"pmulx.da",	BGBCC_SH_NMID_PMULXDA},

{"pshuf.b",		BGBCC_SH_NMID_PSHUFB},
{"pshuf.w",		BGBCC_SH_NMID_PSHUFW},
{"pshuf.l",		BGBCC_SH_NMID_PSHUFL},
{"pshufx.l",	BGBCC_SH_NMID_PSHUFXL},

{"pmulu.w",		BGBCC_SH_NMID_PMULUW},
{"pmuls.w",		BGBCC_SH_NMID_PMULSW},
{"pmulu.lw",	BGBCC_SH_NMID_PMULULW},
{"pmuls.lw",	BGBCC_SH_NMID_PMULSLW},
{"pmulu.hw",	BGBCC_SH_NMID_PMULUHW},
{"pmuls.hw",	BGBCC_SH_NMID_PMULSHW},

{"pcselt.w",	BGBCC_SH_NMID_PCSELTW},
{"pcselt.l",	BGBCC_SH_NMID_PCSELTL},
{"pcmpeq.w",	BGBCC_SH_NMID_PCMPEQW},
{"pcmpeq.l",	BGBCC_SH_NMID_PCMPEQL},
{"pcmphi.w",	BGBCC_SH_NMID_PCMPHIW},
{"pcmphi.l",	BGBCC_SH_NMID_PCMPHIL},
{"pcmpgt.w",	BGBCC_SH_NMID_PCMPGTW},
{"pcmpgt.l",	BGBCC_SH_NMID_PCMPGTL},

{"pcmpeq.h",	BGBCC_SH_NMID_PCMPEQH},
{"pcmpeq.f",	BGBCC_SH_NMID_PCMPEQF},
{"pcmpgt.h",	BGBCC_SH_NMID_PCMPGTH},
{"pcmpgt.f",	BGBCC_SH_NMID_PCMPGTF},

{"rgb5shr1",	BGBCC_SH_NMID_RGB5SHR1},
{"rgb5minmax",	BGBCC_SH_NMID_RGB5MINMAX},
{"rgb5ccenc",	BGBCC_SH_NMID_RGB5CCENC1},
{"rgb5ccenc1",	BGBCC_SH_NMID_RGB5CCENC1},
{"rgb5ccenc2",	BGBCC_SH_NMID_RGB5CCENC2},

{"rgb5pck32",	BGBCC_SH_NMID_RGB5PCK32},
{"rgb5pck64",	BGBCC_SH_NMID_RGB5PCK64},
{"rgb5upck32",	BGBCC_SH_NMID_RGB5UPCK32},
{"rgb5upck64",	BGBCC_SH_NMID_RGB5UPCK64},
{"rgb32pck64",	BGBCC_SH_NMID_RGB32PCK64},
{"rgb32upck64",	BGBCC_SH_NMID_RGB32UPCK64},

{"pcvtsb2h",	BGBCC_SH_NMID_PCVTSB2HL},
{"pcvtub2h",	BGBCC_SH_NMID_PCVTUB2HL},
{"pcvtsb2hl",	BGBCC_SH_NMID_PCVTSB2HL},
{"pcvtub2hl",	BGBCC_SH_NMID_PCVTUB2HL},
{"pcvtsb2hh",	BGBCC_SH_NMID_PCVTSB2HH},
{"pcvtub2hh",	BGBCC_SH_NMID_PCVTUB2HH},

{"pcvtsw2f",	BGBCC_SH_NMID_PCVTSW2FL},
{"pcvtuw2f",	BGBCC_SH_NMID_PCVTUW2FL},
{"pcvtsw2fl",	BGBCC_SH_NMID_PCVTSW2FL},
{"pcvtuw2fl",	BGBCC_SH_NMID_PCVTUW2FL},
{"pcvtsw2fh",	BGBCC_SH_NMID_PCVTSW2FH},
{"pcvtuw2fh",	BGBCC_SH_NMID_PCVTUW2FH},

{"pcvth2sb",	BGBCC_SH_NMID_PCVTH2SB},
{"pcvth2ub",	BGBCC_SH_NMID_PCVTH2UB},
{"pcvtsw2h",	BGBCC_SH_NMID_PCVTSW2H},
{"pcvtuw2h",	BGBCC_SH_NMID_PCVTUW2H},

{"pcvtf2sw",	BGBCC_SH_NMID_PCVTF2SW},
{"pcvtf2uw",	BGBCC_SH_NMID_PCVTF2UW},
{"pcvth2sw",	BGBCC_SH_NMID_PCVTH2SW},
{"pcvth2uw",	BGBCC_SH_NMID_PCVTH2UW},

{"pcvth2al",	BGBCC_SH_NMID_PCVTH2AL},
{"pcvtal2h",	BGBCC_SH_NMID_PCVTAL2H},


{"psqrta.h",	BGBCC_SH_NMID_PSQRTAH},
{"psqrtua.h",	BGBCC_SH_NMID_PSQRTUAH},
{"psqrta.f",	BGBCC_SH_NMID_PSQRTAF},
{"psqrtua.h",	BGBCC_SH_NMID_PSQRTUAF},

{"prcpa.h",		BGBCC_SH_NMID_PRCPAH},
{"prelu.h",		BGBCC_SH_NMID_PRELUH},
{"prcpa.f",		BGBCC_SH_NMID_PRCPAF},
{"prelu.f",		BGBCC_SH_NMID_PRELUF},

{"mov.tw",		BGBCC_SH_NMID_MOVTW},
{"movu.tw",		BGBCC_SH_NMID_MOVUTW},
{"mov.htw",		BGBCC_SH_NMID_MOVHTW},

{"mov.twv",		BGBCC_SH_NMID_MOVTW_V},
{"movu.twv",	BGBCC_SH_NMID_MOVUTW_V},
{"mov.htwv",	BGBCC_SH_NMID_MOVHTW_V},


{"mv",			BGBCC_SH_NMID_MOV,		2},
{"li",			BGBCC_SH_NMID_MOV,		2},
{"call",		BGBCC_SH_NMID_BSR,		2},
{"addi",		BGBCC_SH_NMID_ADD,		2},
{"addw",		BGBCC_SH_NMID_ADDSL,	2},

// {"iclrmd.dq",	BGBCC_SH_NMID_ICLRMD_DQ},
// {"isetmd.dq",	BGBCC_SH_NMID_ISETMD_DQ},

{NULL, 0}
};

int bgbcc_jx2a_nmidtab_chn[1024];
int bgbcc_jx2a_nmidtab_hash[256];
int bgbcc_jx2a_init=0;

int BGBCC_JX2A_Init()
{
	int i, j, k;
	
	if(bgbcc_jx2a_init)
		return(0);
	bgbcc_jx2a_init=1;
	
	for(i=0; i<256; i++)
		bgbcc_jx2a_nmidtab_hash[i]=-1;
	
	for(i=0; bgbcc_jx2a_nmidtab[i].name; i++)
	{
		j=BGBCC_CCXL_HashNameCase(bgbcc_jx2a_nmidtab[i].name);
		j&=255;
		
		bgbcc_jx2a_nmidtab_chn[i]=bgbcc_jx2a_nmidtab_hash[j];
		bgbcc_jx2a_nmidtab_hash[j]=i;
		
//		if(!bgbcc_stricmp(bgbcc_jx2a_nmidtab[i].name, name))
//			return(bgbcc_jx2a_nmidtab[i].nmid);
	}
	return(-1);
}

int BGBCC_JX2A_LookupOpcodeNmid(
	BGBCC_JX2_Context *ctx,
	char *name)
{
	int h, skp;
	int i, j;

	h=BGBCC_CCXL_HashNameCase(name);
	h&=255;

	i=bgbcc_jx2a_nmidtab_hash[h];
	while(i>=0)
	{
		if(!bgbcc_stricmp(bgbcc_jx2a_nmidtab[i].name, name))
		{
			skp=0;
			j=bgbcc_jx2a_nmidtab[i].mfl;
			if((j&1) && (ctx->emit_riscv&0x11))
				skp=1;
			if((j&2) && !(ctx->emit_riscv&0x11))
				skp=1;
			
			if(!skp)
				return(bgbcc_jx2a_nmidtab[i].nmid);
		}
		i=bgbcc_jx2a_nmidtab_chn[i];
	}
	return(-1);

//	for(i=0; bgbcc_jx2a_nmidtab[i].name; i++)
//		if(!bgbcc_stricmp(bgbcc_jx2a_nmidtab[i].name, name))
//			return(bgbcc_jx2a_nmidtab[i].nmid);
//	return(-1);
}

int BGBCC_JX2A_LookupOpcodeFmid(
	BGBCC_JX2_OpcodeArg *arg0,
	BGBCC_JX2_OpcodeArg *arg1,
	BGBCC_JX2_OpcodeArg *arg2,
	BGBCC_JX2_OpcodeArg *arg3)
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
			case BGBCC_SH_OPVTY_NAME:
				fm=BGBCC_SH_FMID_REGLBL; break;
			default: fm=0; break;
			}
			break;
		case BGBCC_SH_OPVTY_IMM:
			switch(arg1->ty)
			{
			case BGBCC_SH_OPVTY_REG:
				fm=BGBCC_SH_FMID_REGIMM; break;
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

//	if(1)
	if(!arg3->ty)
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
				case BGBCC_SH_OPVTY_NAME:
					fm=BGBCC_SH_FMID_REGREGLBL; break;
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
		return(fm);
	}

	if(1)
//	if(!arg3->ty)
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
					switch(arg3->ty)
					{
					case BGBCC_SH_OPVTY_REG:
						fm=BGBCC_SH_FMID_REGREGREGREG; break;
					case BGBCC_SH_OPVTY_IMM:
						fm=BGBCC_SH_FMID_REGREGREGIMM; break;
					default: fm=0; break;
					}
					break;
				case BGBCC_SH_OPVTY_IMM:
					switch(arg3->ty)
					{
					case BGBCC_SH_OPVTY_REG:
						fm=BGBCC_SH_FMID_REGREGIMMREG; break;
					default: fm=0; break;
					}
					break;
				default: fm=0; break;
				}
				break;
#if 1
			case BGBCC_SH_OPVTY_IMM:
				switch(arg2->ty)
				{
				case BGBCC_SH_OPVTY_REG:
					switch(arg3->ty)
					{
					case BGBCC_SH_OPVTY_REG:
						fm=BGBCC_SH_FMID_REGIMMREGREG; break;
//					case BGBCC_SH_OPVTY_IMM:
//						fm=BGBCC_SH_FMID_REGREGREGIMM; break;
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

int BGBCC_JX2A_TryAssembleOpcode(
	BGBCC_JX2_Context *ctx,
	char *name,
	BGBCC_JX2_OpcodeArg *arg0,
	BGBCC_JX2_OpcodeArg *arg1,
	BGBCC_JX2_OpcodeArg *arg2,
	BGBCC_JX2_OpcodeArg *arg3)
{
	int nmid, fmid, rt, lbl;
	
	if(name[0]=='.')
		return(0);
	
	nmid=BGBCC_JX2A_LookupOpcodeNmid(ctx, name);
	if(nmid<=0)
	{
		printf("Unknown Instruction %s\n", name);
		return(0);
	}

#if 0
	if(nmid==BGBCC_SH_NMID_ISETMD_DQ)
	{
		BGBCC_JX2C_SetModeDqSet(NULL, ctx);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_ICLRMD_DQ)
	{
		BGBCC_JX2C_SetModeDqClear(NULL, ctx);
		return(1);
	}
#endif

	fmid=BGBCC_JX2A_LookupOpcodeFmid(arg0, arg1, arg2, arg3);
	if(fmid<=0)
		return(0);

	if(ctx->iflvl_f>0)
		return(1);
	rt=0;

	switch(fmid)
	{
	case BGBCC_SH_FMID_REGREG:
		rt=BGBCC_JX2_TryEmitOpRegReg(ctx,
			nmid, arg0->breg, arg1->breg);
		break;
	case BGBCC_SH_FMID_REGIMM:
//		if(nmid==BGBCC_SH_NMID_MOV)
		if((nmid==BGBCC_SH_NMID_MOV) && !(ctx->emit_riscv&0x33))
		{
			if(	(arg1->breg==BGBCC_SH_REG_R0) ||
				(arg1->breg==BGBCC_SH_REG_DLR))
			{
				BGBCC_JX2_EmitLoadDrImm(ctx, arg0->disp);
				rt=1;
				break;
			}
		
			BGBCC_JX2_EmitLoadRegImm64P(ctx,
				arg1->breg, arg0->disp);
			rt=1;
			break;
		}
	
		rt=BGBCC_JX2_TryEmitOpImmReg(ctx,
			nmid, arg0->disp, arg1->breg);
		break;

	case BGBCC_SH_FMID_REGREGREG:
		rt=BGBCC_JX2_TryEmitOpRegRegReg(ctx,
			nmid, arg0->breg, arg1->breg, arg2->breg);
		break;
	case BGBCC_SH_FMID_REGIMMREG:
		rt=BGBCC_JX2_TryEmitOpRegImmReg(ctx,
			nmid, arg0->breg, arg1->disp, arg2->breg);
		break;

	case BGBCC_SH_FMID_REGREGREGREG:
		rt=BGBCC_JX2_TryEmitOpRegRegRegReg(ctx,
			nmid, arg0->breg, arg1->breg, arg2->breg, arg3->breg);
		break;
	case BGBCC_SH_FMID_REGREGIMMREG:
		rt=BGBCC_JX2_TryEmitOpRegRegImmReg(ctx,
			nmid, arg0->breg, arg1->breg, arg2->disp, arg3->breg);
		break;
	case BGBCC_SH_FMID_REGREGREGIMM:
		rt=BGBCC_JX2_TryEmitOpRegRegImmReg(ctx,
			nmid, arg0->breg, arg1->breg, arg3->disp, arg2->breg);
		break;

	case BGBCC_SH_FMID_REGIMMREGREG:
		rt=BGBCC_JX2_TryEmitOpRegImmRegReg(ctx,
			nmid, arg0->breg, arg1->disp, arg2->breg, arg3->breg);
		break;

	case BGBCC_SH_FMID_REGST:
		rt=BGBCC_JX2_TryEmitOpRegStReg(ctx,
			nmid, arg0->breg, arg1->breg);
		break;
	case BGBCC_SH_FMID_REGLD:
		rt=BGBCC_JX2_TryEmitOpLdRegReg(ctx,
			nmid, arg0->breg, arg1->breg);
		break;
	case BGBCC_SH_FMID_REGDECST:
		rt=BGBCC_JX2_TryEmitOpRegStDecReg(ctx,
			nmid, arg0->breg, arg1->breg);
		break;
	case BGBCC_SH_FMID_REGINCLD:
		rt=BGBCC_JX2_TryEmitOpLdIncRegReg(ctx,
			nmid, arg0->breg, arg1->breg);
		break;
	case BGBCC_SH_FMID_REGSTR0N:
		rt=BGBCC_JX2_TryEmitOpRegStReg2(ctx,
			nmid, arg0->breg, arg1->ireg, arg1->breg);
		break;
	case BGBCC_SH_FMID_REGLDR0M:
		rt=BGBCC_JX2_TryEmitOpLdReg2Reg(ctx,
			nmid, arg0->breg, arg0->ireg, arg1->breg);
		break;
	case BGBCC_SH_FMID_REGSTDISP:
		rt=BGBCC_JX2_TryEmitOpRegStRegDisp(ctx,
			nmid, arg0->breg, arg1->breg, arg1->disp);
		break;
	case BGBCC_SH_FMID_REGLDDISP:
		rt=BGBCC_JX2_TryEmitOpLdRegDispReg(ctx,
			nmid, arg0->breg, arg0->disp, arg1->breg);
		break;
	
	case BGBCC_SH_FMID_NONE:
		rt=BGBCC_JX2_TryEmitOpNone(ctx, nmid);
		break;
	
	case BGBCC_SH_FMID_REGRM:
	case BGBCC_SH_FMID_REGRN:
		rt=BGBCC_JX2_TryEmitOpReg(ctx, nmid, arg0->breg);
		break;
	case BGBCC_SH_FMID_IMM:
		rt=BGBCC_JX2_TryEmitOpImm(ctx, nmid, arg0->disp);
		break;
	case BGBCC_SH_FMID_LDREG:
	case BGBCC_SH_FMID_STREG:
		rt=BGBCC_JX2_TryEmitOpMReg(ctx, nmid, arg0->breg);
		break;

	case BGBCC_SH_FMID_LABEL:
//		BGBCC_JX2_EmitPadForOpWord(ctx, 0xF000);
		lbl=BGBCC_JX2_GetNamedLabel(ctx, arg0->name);
//		rt=BGBCC_JX2_TryEmitOpLabel(ctx, nmid, lbl);
		rt=BGBCC_JX2_TryEmitOpAutoLabel(ctx, nmid, lbl);
		break;
	case BGBCC_SH_FMID_LBLREG:
//		if(nmid==BGBCC_SH_NMID_MOV)
		if((nmid==BGBCC_SH_NMID_MOV) && !(ctx->emit_riscv&0x21))
		{
			rt=BGBCC_JX2C_CheckEmitLoadFrameNameReg(
				ctx->tctx, ctx, arg0->name, arg1->breg);
			if(rt>0)
				break;
		}

		lbl=BGBCC_JX2_GetNamedLabel(ctx, arg0->name);
		rt=BGBCC_JX2_TryEmitOpLblReg(ctx, nmid, lbl, arg1->breg);
		break;

	case BGBCC_SH_FMID_REGLBL:
//		if(nmid==BGBCC_SH_NMID_MOV)
		if((nmid==BGBCC_SH_NMID_MOV) && !(ctx->emit_riscv&0x21))
		{
			rt=BGBCC_JX2C_CheckEmitStoreFrameNameReg(
				ctx->tctx, ctx, arg1->name, arg0->breg);
			if(rt>0)
				break;
		}

		lbl=BGBCC_JX2_GetNamedLabel(ctx, arg1->name);
		rt=BGBCC_JX2_TryEmitOpRegLbl(ctx, nmid, lbl, arg0->breg);
		break;

	case BGBCC_SH_FMID_REGREGLBL:
		lbl=BGBCC_JX2_GetNamedLabel(ctx, arg2->name);
		rt=BGBCC_JX2_TryEmitOpRegRegLbl(ctx, nmid,
			arg0->breg, arg1->breg, lbl);
		break;

	default:
		__debugbreak();
		rt=0;
		break;
	}
	return(rt);
}

int BGBCC_JX2A_ParseCheckFeatureList(BGBCC_JX2_Context *ctx, char *sym)
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
			
			ni=BGBCC_JX2A_ParseCheckFeatureList(ctx, tb);
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
				if(BGBCC_JX2A_ParseCheckFeature(ctx, tb)!=0)
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
				if(BGBCC_JX2A_ParseCheckFeature(ctx, tb)==0)
					return(0);
//				dfl=1;
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
		if(BGBCC_JX2A_ParseCheckFeature(ctx, tb)!=0)
			return(1);
	}
	return(0);
}

int BGBCC_JX2A_ParseCheckFeature(BGBCC_JX2_Context *ctx, char *sym)
{
	BGBCC_TransState *tctx;
	int i;

	if(!bgbcc_stricmp(sym, "bjx2"))
		return(1);
	
	if((*sym>='0') && *sym<='9')
	{
		i=atoi(sym);
		return(i!=0);
	}
	
	if(*sym=='!')
	{
		return(!BGBCC_JX2A_ParseCheckFeature(ctx, sym+1));
	}

	tctx=ctx->tctx;

	if(!bgbcc_stricmp(sym, "bjx2_fix32"))
		return((ctx->is_fixed32)!=0);
	if(!bgbcc_stricmp(sym, "bjx2_xg2"))
		return((ctx->is_fixed32&2)!=0);

	if(!bgbcc_stricmp(sym, "bjx1_egpr"))
		return(ctx->has_bjx1egpr);
	if(!bgbcc_stricmp(sym, "bjx1_betav"))
		return(ctx->is_betav);
	if(!bgbcc_stricmp(sym, "bjx1_64"))
		return(ctx->is_addr64);

	if(	!bgbcc_stricmp(sym, "bjx1_nofpu") ||
		!bgbcc_stricmp(sym, "bjx2_nofpu"))
		return(ctx->no_fpu);
	if(	!bgbcc_stricmp(sym, "bjx1_noext32") ||
		!bgbcc_stricmp(sym, "bjx2_noext32"))
		return(ctx->no_ext32);

	if(	!bgbcc_stricmp(sym, "bjx1_fpu_gfp") ||
		!bgbcc_stricmp(sym, "bjx2_fpu_gfp"))
		return(ctx->fpu_gfp);

	if(!bgbcc_stricmp(sym, "bjx2_ptr32"))
		return(ctx->is_addr_x32);
	if(!bgbcc_stricmp(sym, "bjx2_ptr64"))
		return(!(ctx->is_addr_x32));


	if(!bgbcc_stricmp(sym, "bjx2_wex"))
		return(ctx->use_wexmd!=0);
	if(!bgbcc_stricmp(sym, "bjx2_wex2"))
		return(ctx->use_wexmd==1);
	if(!bgbcc_stricmp(sym, "bjx2_wex3"))
		return(ctx->use_wexmd==2);
	if(!bgbcc_stricmp(sym, "bjx2_jumbo"))
		return(ctx->has_jumbo);

	if(!bgbcc_stricmp(sym, "bjx2_wex3p"))
		return(ctx->use_wexmd>=2);

	if(!bgbcc_stricmp(sym, "bjx2_movx"))
		return(ctx->has_pushx2);

	if(!bgbcc_stricmp(sym, "has_movx"))
		return(ctx->has_pushx2);
	if(!bgbcc_stricmp(sym, "has_simdx2"))
		return(ctx->has_simdx2);

	if(!bgbcc_stricmp(sym, "has_alux"))
		return(ctx->has_alux);
	if(!bgbcc_stricmp(sym, "has_fpux"))
		return(ctx->has_fpux);

	if(!bgbcc_stricmp(sym, "has_bra48"))
		return(ctx->has_bra48);
	if(!bgbcc_stricmp(sym, "has_xgpr"))
		return((ctx->has_xgpr&1)!=0);

	if(!bgbcc_stricmp(sym, "has_aluptr"))
		return((ctx->has_qmul&16)!=0);

	if(!bgbcc_stricmp(sym, "abi_is_xgpr"))
	{
		if(tctx->arch_sizeof_ptr==8)
			return(0);
		return((ctx->has_xgpr&2)!=0);
	}

	if(!bgbcc_stricmp(sym, "has_dmacl"))
		return((ctx->has_dmacl&1)!=0);
	if(!bgbcc_stricmp(sym, "has_qmul"))
		return((ctx->has_qmul&1)!=0);

	if(!bgbcc_stricmp(sym, "has_fdiv"))
		return((ctx->has_qmul&2)!=0);
	if(!bgbcc_stricmp(sym, "has_bcd"))
		return((ctx->has_qmul&4)!=0);

	if(!bgbcc_stricmp(sym, "has_fmovs"))
		return((ctx->has_fmovs&1)!=0);
	if(!bgbcc_stricmp(sym, "has_fmovh"))
		return((ctx->has_fmovs&2)!=0);
	if(!bgbcc_stricmp(sym, "has_ldtex"))
		return((ctx->has_fmovs&4)!=0);
	if(!bgbcc_stricmp(sym, "has_ldgbr"))
		return((ctx->has_fmovs&8)!=0);
	if(!bgbcc_stricmp(sym, "has_xmov"))
		return((ctx->has_fmovs&16)!=0);

	if(!bgbcc_stricmp(sym, "has_mov48"))
		return((ctx->has_fmovs&32)!=0);

	if(!bgbcc_stricmp(sym, "has_jcmpz"))
		return((ctx->has_jcmp&1)!=0);
	if(!bgbcc_stricmp(sym, "has_jcmpr"))
		return((ctx->has_jcmp&2)!=0);

	if(!bgbcc_stricmp(sym, "has_jcmp"))
		return((ctx->has_jcmp&3)==3);

	if(!bgbcc_stricmp(sym, "has_fpvsf"))
		return((ctx->has_fpvsf&1)!=0);
	if(!bgbcc_stricmp(sym, "has_fpvsf_sp"))
		return((ctx->has_fpvsf&2)!=0);

	if(!bgbcc_stricmp(sym, "has_fpimm"))
		return((ctx->has_fpim&1)!=0);

	if(!bgbcc_stricmp(sym, "abi_argshadow"))
		return((ctx->abi_spillpad&1)!=0);
	if(!bgbcc_stricmp(sym, "abi_vskgen"))
		return((ctx->abi_spillpad&2)!=0);
	if(!bgbcc_stricmp(sym, "abi_boundschk"))
		return((ctx->abi_spillpad&4)!=0);
	if(!bgbcc_stricmp(sym, "abi_vskglobal"))
		return((ctx->abi_spillpad&8)!=0);


	if(!bgbcc_stricmp(sym, "abi_evenonly"))
		return(ctx->abi_evenonly);

	if(!bgbcc_stricmp(sym, "seen_variant"))
		return((tctx->ccxl_tyc_seen&BGBCC_TYCSEEN_VARIANT)!=0);
	if(!bgbcc_stricmp(sym, "seen_int128"))
		return((tctx->ccxl_tyc_seen&BGBCC_TYCSEEN_INT128)!=0);
	if(!bgbcc_stricmp(sym, "seen_float128"))
		return((tctx->ccxl_tyc_seen&BGBCC_TYCSEEN_FLOAT128)!=0);
	if(!bgbcc_stricmp(sym, "seen_float_fpu"))
		return((tctx->ccxl_tyc_seen&BGBCC_TYCSEEN_FLOAT_FPU)!=0);

	if(!bgbcc_stricmp(sym, "seen_idiv_var"))
		return((tctx->ccxl_tyc_seen&BGBCC_TYCSEEN_IDIV_VAR)!=0);
	if(!bgbcc_stricmp(sym, "seen_imod_var"))
		return((tctx->ccxl_tyc_seen&BGBCC_TYCSEEN_IMOD_VAR)!=0);

	if(!ctx->has_shad)
//	if(1)
	{
		if(!bgbcc_stricmp(sym, "seen_ishl_var"))
			return((tctx->ccxl_tyc_seen&BGBCC_TYCSEEN_ISHL_VAR)!=0);
		if(!bgbcc_stricmp(sym, "seen_ishr_var"))
			return((tctx->ccxl_tyc_seen&BGBCC_TYCSEEN_ISHR_VAR)!=0);
		if(!bgbcc_stricmp(sym, "seen_isar_var"))
			return((tctx->ccxl_tyc_seen&BGBCC_TYCSEEN_ISAR_VAR)!=0);
	}

	return(0);
}

int BGBCC_JX2A_ParseOpcode_ScanForBar(BGBCC_JX2_Context *ctx, char *cs0)
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

int BGBCC_JX2A_ParseOpcode(BGBCC_JX2_Context *ctx, char **rcs)
{
	BGBCC_JX2_OpcodeArg arg[4];
	char tb0[256];
//	BTSHAS_ListingOp *op;
	char *tk0, *tk1;
	char *cs, *cs1, *cs2, *cs3;
	s64 li;
	f32 ff;
	f64 fd;
	int i, j, k, wx, pfc;

	pfc=0;

	cs=*rcs;
	cs1=BGBCC_JX2A_ParseTokenAlt(cs, &tk0);

	if(*tk0=='I')
	{
		cs1=BGBCC_JX2A_EatWhiteOnlyNoLinebreak(cs1);
		if(*cs1==':')
		{
			if(ctx->iflvl_f>0)
			{
				*rcs=cs1+1;
				return(1);
			}

#if 1
			if(ctx->is_align_wexj)
			{
				if(ctx->is_align_wexj&2)
				{
					BGBCC_JX2_EmitPad32AlignLastOp(ctx);
				}else
				{
					/* Detect WEX or Pred ops, if so, align label. */
					cs2=BGBCC_JX2A_EatWhite(cs1+1);
					if(BGBCC_JX2A_ParseOpcode_ScanForBar(ctx, cs2))
					{
						BGBCC_JX2_EmitPad32AlignLastOp(ctx);
					}
				}
			}
#endif
			
			if((tk0[1]=='.') && bgbcc_jx2a_lastlbl)
			{
//				BGBCC_JX2_EmitPadForOpWord(ctx, 0xF000);
				BGBCC_JX2_EmitPadForLabel(ctx);

				sprintf(tb0, "%s%s", bgbcc_jx2a_lastlbl, tk0+1);
				BGBCC_JX2_EmitNamedLabel(ctx, tb0);
				BGBCC_JX2C_ResetModeDqUnknown(NULL, ctx);
			}else
			{
//				BGBCC_JX2_EmitPadForOpWord(ctx, 0xF000);
				BGBCC_JX2_EmitPadForLabel(ctx);

				bgbcc_jx2a_lastlbl=bgbcc_strdup(tk0+1);
				BGBCC_JX2_EmitNamedLabel(ctx, tk0+1);
				BGBCC_JX2C_ResetModeDqUnknown(NULL, ctx);

				if(	(ctx->sec==BGBCC_SH_CSEG_TEXT) ||
					(ctx->sec==BGBCC_SH_CSEG_UTEXT))
				{
					k=BGBCC_JX2_GetNamedLabel(ctx, tk0+1);
					BGBCC_JX2_MarkLabelIsText(ctx, k);
				}

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
		arg[0].ty=0; arg[1].ty=0; arg[2].ty=0; arg[3].ty=0;
//		for(i=0; i<3; i++)
		for(i=0; i<4; i++)
		{
			if(BGBCC_JX2A_ParseOperand(ctx, &cs2, &arg[i])<=0)
				break;
			cs2=BGBCC_JX2A_EatWhiteNoLinebreak(cs2);
			if(*cs2==',')
				cs2++;
		}
//		if(BGBCC_JX2A_ParseOperand(&cs2, arg[0])>0)
//			if(BGBCC_JX2A_ParseOperand(&cs2, arg[1])>0)
//				if(BGBCC_JX2A_ParseOperand(&cs2, arg[2])>0)
//		{
//		}

#if 0
		op=BTSHAS_SHxOpLookup(tk0+1, &arg[0], &arg[1], &arg[2]);
		if(op)
		{
			BGBCC_JX2_EmitListOp(ctx, op, &arg[0], &arg[1], &arg[2]);
			*rcs=cs2;
			return(1);
		}
#endif

		wx=0;
		cs2=BGBCC_JX2A_EatWhiteNoLinebreak(cs2);
		if(*cs2=='|')
			{ wx=1; }
		else
		{
			cs3=BGBCC_JX2A_EatWhite(cs2);
			if(cs3 && (*cs3=='|'))
				{ wx=1; cs2=cs3; }
		}
		
		if(wx || pfc)
		{
			BGBCC_JX2_EmitPadForOpWord(ctx, 0xF000);
		}
		
//		if((ctx->is_align_wexj && (wx || pfc)) ||
//			(ctx->is_align_wexj&2))
		if(ctx->is_align_wexj && (wx || pfc))
		{
			BGBCC_JX2_EmitPad32AlignLastOp(ctx);
		}
			
		if(wx)
		{
			ctx->op_is_wex2=3;
		}
		
		if(pfc)
		{
//			if(ctx->op_is_wex2)
//			if(ctx->op_is_wex2&1)
			if(wx)
				ctx->op_is_wex2=pfc|2;
			else
				ctx->op_is_wex2=pfc;
		}

//		k=BGBCC_JX2A_TryAssembleOpcode(ctx, tk0+1, &arg[0], &arg[1], &arg[2]);
		k=BGBCC_JX2A_TryAssembleOpcode(ctx, tk0+1,
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
			cs2=BGBCC_JX2A_ParseToken(cs2, &tk0);
			BGBCC_JX2_EmitBAlign(ctx, bgbcc_atoi(tk0+1)*2);
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.balign"))
		{
			cs2=cs1;
			cs2=BGBCC_JX2A_ParseToken(cs2, &tk0);
			BGBCC_JX2_EmitBAlign(ctx, bgbcc_atoi(tk0+1));
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.rept"))
		{
			cs2=cs1;
			cs2=BGBCC_JX2A_ParseToken(cs2, &tk0);
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
			!strcmp(tk0, "I.int") ||
			!strcmp(tk0, "I.dword"))
		{
			cs2=cs1;
			cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk0);
			cs2=BGBCC_JX2A_EatWhiteNoLinebreak(cs2);

#if 1
			if(ctx->iflvl_f>0)
			{
				while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
					cs2++;
				*rcs=cs2;
				return(1);
			}
#endif

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
					cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk1);
					if(tk1[0]=='I')
					{
						BGBCC_JX2_EmitNamedReloc(ctx, tk0+1,
							BGBCC_SH_RLC_REL32);
						BGBCC_JX2_EmitNamedReloc(ctx, tk1+1,
							BGBCC_SH_RLC_REL32B);
					}else if(tk1[0]=='|')
					{
						BGBCC_JX2_EmitNamedReloc(ctx, tk0+1,
							BGBCC_SH_RLC_ABS32);
						k=-bgbcc_atoi(tk1+1);
					}
				}else if(*cs2=='+')
				{
					cs2++;
					cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk1);
					if(tk1[0]=='I')
					{
						BGBCC_JX2_EmitNamedReloc(ctx, tk0+1,
							BGBCC_SH_RLC_REL32);
						BGBCC_JX2_EmitNamedReloc(ctx, tk1+1,
							BGBCC_SH_RLC_REL32);
					}else if(tk1[0]=='|')
					{
						BGBCC_JX2_EmitNamedReloc(ctx, tk0+1,
							BGBCC_SH_RLC_ABS32);
						k=bgbcc_atoi(tk1+1);
					}
				}else
				{
					BGBCC_JX2_EmitNamedReloc(ctx, tk0+1,
						BGBCC_SH_RLC_ABS32);
				}

				BGBCC_JX2_EmitDWord(ctx, k);
//				*rcs=cs2;
//				return(1);
			}else if((*tk0=='|') || (*tk0=='C'))

			{
	//			BGBCC_JX2_EmitDWord(ctx, bgbcc_atoi(tk0+1));

				if(*tk0=='|')
					i=bgbcc_atoi(tk0+1);
				if(*tk0=='C')
					i=(ctx->is_le)?
						(tk0[1]|(tk0[2]<<8)|(tk0[3]<<16)|(tk0[4]<<24)):
						(tk0[4]|(tk0[3]<<8)|(tk0[2]<<16)|(tk0[1]<<24));
				BGBCC_JX2_EmitDWord(ctx, i);
			}

			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk0);
//				BGBCC_JX2_EmitDWord(ctx, bgbcc_atoi(tk0+1));

				if(*tk0=='I')
				{
					BGBCC_JX2_EmitNamedReloc(ctx, tk0+1,
						BGBCC_SH_RLC_ABS32);
					BGBCC_JX2_EmitDWord(ctx, 0);
					continue;
				}

				if(*tk0=='|')
					i=bgbcc_atoi(tk0+1);
				if(*tk0=='C')
					i=(ctx->is_le)?
						(tk0[1]|(tk0[2]<<8)|(tk0[3]<<16)|(tk0[4]<<24)):
						(tk0[4]|(tk0[3]<<8)|(tk0[2]<<16)|(tk0[1]<<24));
				BGBCC_JX2_EmitDWord(ctx, i);
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.word") ||
			!strcmp(tk0, "I.short") ||
			!strcmp(tk0, "I.hword"))
		{
			cs2=cs1;

#if 1
			if(ctx->iflvl_f>0)
			{
				while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
					cs2++;
				*rcs=cs2;
				return(1);
			}
#endif

			cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk0);
//			BGBCC_JX2_EmitWord(ctx, bgbcc_atoi(tk0+1));
			if(*tk0=='|')
				i=bgbcc_atoi(tk0+1);
			if(*tk0=='C')
				i=(ctx->is_le)?(tk0[1]|(tk0[2]<<8)):(tk0[2]|(tk0[1]<<8));
			BGBCC_JX2_EmitWord(ctx, i);

			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk0);
//				BGBCC_JX2_EmitWord(ctx, bgbcc_atoi(tk0+1));

				if(*tk0=='|')
					i=bgbcc_atoi(tk0+1);
				if(*tk0=='C')
					i=(ctx->is_le)?(tk0[1]|(tk0[2]<<8)):(tk0[2]|(tk0[1]<<8));
				BGBCC_JX2_EmitWord(ctx, i);
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.byte"))
		{
			cs2=cs1;

#if 1
			if(ctx->iflvl_f>0)
			{
				while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
					cs2++;
				*rcs=cs2;
				return(1);
			}
#endif

			cs2=BGBCC_JX2A_ParseToken(cs2, &tk0);
			if(*tk0=='|')
				i=bgbcc_atoi(tk0+1);
			if(*tk0=='C')
				i=tk0[1];
			BGBCC_JX2_EmitByte(ctx, i);
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_JX2A_ParseToken(cs2, &tk0);
				if(*tk0=='|')
					i=bgbcc_atoi(tk0+1);
				if(*tk0=='C')
					i=tk0[1];
				BGBCC_JX2_EmitByte(ctx, i);
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.quad") ||
			!strcmp(tk0, "I.qword"))
		{
			cs2=cs1;

#if 1
			if(ctx->iflvl_f>0)
			{
				while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
					cs2++;
				*rcs=cs2;
				return(1);
			}
#endif

			cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk0);
			BGBCC_JX2_EmitQWord(ctx, bgbcc_atoi(tk0+1));
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk0);
				BGBCC_JX2_EmitQWord(ctx, bgbcc_atoi(tk0+1));
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.tword"))
		{
			cs2=cs1;

#if 1
			if(ctx->iflvl_f>0)
			{
				while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
					cs2++;
				*rcs=cs2;
				return(1);
			}
#endif

			cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk0);
			BGBCC_JX2_EmitTWord(ctx, bgbcc_atoi(tk0+1));
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk0);
				BGBCC_JX2_EmitTWord(ctx, bgbcc_atoi(tk0+1));
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.float") || !strcmp(tk0, "I.single"))
		{
			cs2=cs1;

#if 1
			if(ctx->iflvl_f>0)
			{
				while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
					cs2++;
				*rcs=cs2;
				return(1);
			}
#endif

			cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk0);
			ff=bgbcc_atof(tk0+1);
			BGBCC_JX2_EmitDWord(ctx, *(u32 *)(&ff));
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk0);
				ff=bgbcc_atof(tk0+1);
				BGBCC_JX2_EmitDWord(ctx, *(u32 *)(&ff));
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.double"))
		{
			cs2=cs1;

#if 1
			if(ctx->iflvl_f>0)
			{
				while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
					cs2++;
				*rcs=cs2;
				return(1);
			}
#endif

			cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk0);
			fd=bgbcc_atof(tk0+1);
			BGBCC_JX2_EmitQWord(ctx, *(u64 *)(&fd));
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk0);
				fd=bgbcc_atof(tk0+1);
				BGBCC_JX2_EmitQWord(ctx, *(u64 *)(&fd));
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.string") ||
			!strcmp(tk0, "I.asciz"))
		{
			cs2=cs1;

#if 1
			if(ctx->iflvl_f>0)
			{
				while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
					cs2++;
				*rcs=cs2;
				return(1);
			}
#endif

			cs2=BGBCC_JX2A_ParseToken(cs2, &tk0);
			BGBCC_JX2_EmitString(ctx, tk0+1);
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_JX2A_ParseToken(cs2, &tk0);
				BGBCC_JX2_EmitString(ctx, tk0+1);
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.ascii"))
		{
			cs2=cs1;

#if 1
			if(ctx->iflvl_f>0)
			{
				while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
					cs2++;
				*rcs=cs2;
				return(1);
			}
#endif

			cs2=BGBCC_JX2A_ParseToken(cs2, &tk0);
			BGBCC_JX2_EmitAscii(ctx, tk0+1);
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_JX2A_ParseToken(cs2, &tk0);
				BGBCC_JX2_EmitAscii(ctx, tk0+1);
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.ucstr"))
		{
			cs2=cs1;

#if 1
			if(ctx->iflvl_f>0)
			{
				while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
					cs2++;
				*rcs=cs2;
				return(1);
			}
#endif

			cs2=BGBCC_JX2A_ParseToken(cs2, &tk0);
			BGBCC_JX2_EmitAsciiUTF2UCS2(ctx, tk0+1);
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_JX2A_ParseToken(cs2, &tk0);
				BGBCC_JX2_EmitAsciiUTF2UCS2(ctx, tk0+1);
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.ucstrz"))
		{
			cs2=cs1;

#if 1
			if(ctx->iflvl_f>0)
			{
				while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
					cs2++;
				*rcs=cs2;
				return(1);
			}
#endif

			cs2=BGBCC_JX2A_ParseToken(cs2, &tk0);
			BGBCC_JX2_EmitStringUTF2UCS2(ctx, tk0+1);
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_JX2A_ParseToken(cs2, &tk0);
				BGBCC_JX2_EmitStringUTF2UCS2(ctx, tk0+1);
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.section"))
		{
			cs2=cs1;
			cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk0);
			BGBCC_JX2_SetSectionName(ctx, tk0+1);

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
			cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk0);
//			BGBCC_JX2_SetSectionName(ctx, tk0+1);

			if(	!strcmp(tk0, "bjx2")	 ||
				!strcmp(tk0, "bjx2a")	)
			{
				BGBCC_JX2_EmitBAlign(ctx, 4);
//				ctx->emit_riscv=0;
				ctx->emit_riscv&=~0x30;
			}

			if(!strcmp(tk0, "riscv"))
			{
				BGBCC_JX2_EmitBAlign(ctx, 4);
//				ctx->emit_riscv=1;
				ctx->emit_riscv|=16;
			}

			if(!strcmp(tk0, "xg2rv"))
			{
				BGBCC_JX2_EmitBAlign(ctx, 4);
				ctx->emit_riscv|=32;
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.riscv"))
		{
			BGBCC_JX2_EmitBAlign(ctx, 4);
//			ctx->emit_riscv=1;
			ctx->emit_riscv|=0x10;
			*rcs=cs1;
			return(1);
		}

		if(!strcmp(tk0, "I.endriscv") ||
			!strcmp(tk0, "I.bjx2"))
		{
			BGBCC_JX2_EmitBAlign(ctx, 4);
			ctx->emit_riscv&=~0x30;
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
			BGBCC_JX2_SetSectionName(ctx, tk0+1);
			*rcs=cs1;
			return(1);
		}

		if(!strcmp(tk0, "I.global") ||
			!strcmp(tk0, "I.globl") ||
			!strcmp(tk0, "I.extern") ||
			!strcmp(tk0, "I.weak"))
		{
			cs2=cs1;
			cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk0);
			BGBCC_JX2_EmitNamedGlobal(ctx, tk0+1);
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.local"))
		{
			cs2=cs1;
			cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk0);
			BGBCC_JX2_EmitNamedGlobal(ctx, tk0+1);
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.rel32"))
		{
			cs2=cs1;
			cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk0);

			BGBCC_JX2_EmitNamedReloc(ctx, tk0+1,
				BGBCC_SH_RLC_REL32);

			if(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk0);
				BGBCC_JX2_EmitNamedReloc(ctx, tk0+1,
					BGBCC_SH_RLC_REL32B);
			}

			BGBCC_JX2_EmitDWord(ctx, 0);
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.comm"))
		{
			cs2=cs1;
			cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk0);

			j=1; k=1;
			if(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk1);
				j=atoi(tk1+1);
			}
			if(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk1);
				k=atoi(tk1+1);
			}

			j=j*k;

			BGBCC_JX2_EmitNamedCommSym(ctx, tk0+1, j, 0);
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.skip") || !strcmp(tk0, "I.space"))
		{
			j=1; k=0;

			cs2=cs1;
			cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk0);
			j=bgbcc_atoi(tk0+1);

			if(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk1);
				k=bgbcc_atoi(tk1+1);
			}
			
			while((j--)>0)
				BGBCC_JX2_EmitByte(ctx, k);

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
			cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk0);

//			i=BGBCC_JX2A_ParseCheckFeature(ctx, tk0+1);
			i=BGBCC_JX2A_ParseCheckFeatureList(ctx, tk0+1);
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

	//		cs2=cs1;
			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.ifnarch"))
		{
			cs2=cs1;
			cs2=BGBCC_JX2A_ParseTokenAlt(cs2, &tk0);

//			i=BGBCC_JX2A_ParseCheckFeature(ctx, tk0+1);
			i=BGBCC_JX2A_ParseCheckFeatureList(ctx, tk0+1);

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

	//		cs2=cs1;
			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.beginwex"))
		{
			BGBCC_JX2_BeginWex(ctx);
			cs2=cs1;
			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}
		
		if(!strcmp(tk0, "I.endwex"))
		{
			BGBCC_JX2_EndWex(ctx);
			cs2=cs1;
			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.fix32") ||
			!strcmp(tk0, "I.fix32_on"))
		{
			ctx->is_fixed32|=16;

			cs2=cs1;
			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.endfix32") ||
			!strcmp(tk0, "I.fix32_off"))
		{
			ctx->is_fixed32&=~16;

			cs2=cs1;
			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.xg2rv") ||
			!strcmp(tk0, "I.xg2rv_on"))
		{
			ctx->emit_riscv|=16;

			cs2=cs1;
			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.endxg2rv") ||
			!strcmp(tk0, "I.xg2rv_off"))
		{
			ctx->emit_riscv&=~16;

			cs2=cs1;
			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.riscv") ||
			!strcmp(tk0, "I.riscv_on"))
		{
			ctx->emit_riscv|=32;

			cs2=cs1;
			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.endriscv") ||
			!strcmp(tk0, "I.riscv_off"))
		{
			ctx->emit_riscv&=~32;

			cs2=cs1;
			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}
		
		ctx->tctx->n_error++;
		printf("unexpected token '%s'\n", tk0+1);
		*rcs=cs;
		return(-1);
	}

	ctx->tctx->n_error++;
	printf("unexpected token '%s'\n", tk0+1);
	return(-1);
}

int BGBCC_JX2A_ParseBuffer(BGBCC_JX2_Context *ctx, char **rcs)
{
	char tb[256];
	char *cs, *t;
	int i;

	BGBCC_JX2A_Init();

	BGBCC_JX2_SetSectionName(ctx, ".text");

	cs=*rcs;
	while(*cs)
	{
		cs=BGBCC_JX2A_EatWhite(cs);
		if(!(*cs))
			break;
		i=BGBCC_JX2A_ParseOpcode(ctx, &cs);
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

int BGBCC_JX2C_AssembleBuffer(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	char *text)
{
	int oldar;
	char *cs;

//	BGBCC_JX2_SetSectionName(sctx, ".text");

	sctx->iflvl_f=0;
	sctx->iflvl_t=0;

	bgbcc_jx2a_lastlbl=NULL;

	oldar=sctx->emit_riscv;
	sctx->emit_riscv=0;

	sctx->is_rawasm=1;
	cs=text;
	BGBCC_JX2A_ParseBuffer(sctx, &cs);
	sctx->is_rawasm=0;

	sctx->emit_riscv=oldar;

	return(0);
}
