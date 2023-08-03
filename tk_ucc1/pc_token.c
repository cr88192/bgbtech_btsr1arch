char *TKUCC_TokEatWhite(char *str)
{
	char *s;
	s=str;
	while(*s && *s<=' ')
		s++;
	return(s);
}

int TKUCC_TokNameChar(int ch)
{
	if(	((ch>='A') && (ch<='Z')) ||
		((ch>='a') && (ch<='z')) ||
		((ch>='0') && (ch<='9')) ||
		ch=='_' ||
		ch=='$')
			return(1);
	return(0);
}

int TKUCC_TokNameInitChar(int ch)
{
	if(	((ch>='A') && (ch<='Z')) ||
		((ch>='a') && (ch<='z')) ||
		ch=='_' ||
		ch=='$')
			return(1);
	return(0);
}

int TKUCC_TokNameOperatorChar(int ch)
{
	if(ch<=' ')
		return(0);
	if(ch>'~')
		return(0);
	if(TKUCC_TokNameChar(ch))
		return(0);
	return(1);
}

int TKUCC_TokOperatorLength(char *s)
{
	int ch0, ch1;

	ch0=s[0];
	if(!TKUCC_TokNameOperatorChar(ch0))
		return(0);

	ch1=s[1];

	if(ch1=='=')
	{
		if(	(ch0=='<') ||
			(ch0=='>') ||
			(ch0=='=') ||
			(ch0=='!') ||
			(ch0=='+') ||
			(ch0=='-') ||
			(ch0=='*') ||
			(ch0=='%') ||
			(ch0=='&') ||
			(ch0=='|') ||
			(ch0=='^'))
		{
			return(2);
		}
	}
	
	if(ch0=='-')
	{
		if((ch1=='-') || (ch1=='>'))
		{
			return(2);
		}
	}

	if(ch0=='+')
	{
		if((ch1=='+') || (ch1=='='))
		{
			return(2);
		}
	}

	if(ch0=='&')
	{
		if(ch1=='&')
		{
			return(2);
		}
	}

	if(ch0=='|')
	{
		if(ch1=='|')
		{
			return(2);
		}
	}

	if(ch0=='<')
	{
		if(ch1=='<')
		{
			if(s[2]=='=')
				return(3);
			return(2);
		}
	}

	if(ch0=='>')
	{
		if(ch1=='>')
		{
			if(s[2]=='=')
				return(3);
			return(2);
		}
	}

	return(1);
}

char *TKUCC_TokReadToken(char *str, char *buf, int *rtokty)
{
	char *s, *t;
	int opl;
	
	t=buf;
	s=str;
	s=TKUCC_TokEatWhite(s);

	if(!*s)
	{
		*buf=0;
		*rtokty=0;
		return(s);
	}

	if(TKUCC_TokNameInitChar(*s))
	{
		while(TKUCC_TokNameChar(*s))
		{
			*t++=*s++;
		}
		*rtokty=1;
		*t=0;
		return(s);
	}
	
	if((*s>='0') && (*s<='9'))
	{
		if(*s=='0')
		{
			if((s[1]=='x') || (s[1]=='X'))
			{
				*t++=*s++;
				*t++=*s++;
				while(	((*s>='A') && (*s<='F')) ||
					((*s>='a') && (*s<='f')) ||
					((*s>='0') && (*s<='9')) ||
					*s=='_')
				{
					if(*s=='_')
						{ s++; continue; }
					*t++=*s++;
				}
				*rtokty=2;
				*t=0;
				return(s);
			}
		}

		while(
			((*s>='0') && (*s<='9')) ||
			*s=='_')
		{
			if(*s=='_')
				{ s++; continue; }
			*t++=*s++;
		}
		*rtokty=2;
		*t=0;
		return(s);
	}

	if(*s=='\"')
	{
		s++;
		while(*s && *s!='\"')
		{
			*t++=*s++;
		}
		if(*s=='\"')
			s++;

		*rtokty=3;
		*t=0;
		return(s);
	}

	if(*s=='\'')
	{
		s++;
		while(*s && *s!='\'')
		{
			*t++=*s++;
		}
		if(*s=='\'')
			s++;

		*rtokty=3;
		*t=0;
		return(s);
	}

	opl=TKUCC_TokOperatorLength(s);

	if(opl)
	{
		if(opl==1)
		{
			*rtokty=4;
			*t++=*s++;
			*t=0;
			return(s);
		}

		if(opl==2)
		{
			*rtokty=4;
			*t++=*s++;
			*t++=*s++;
			*t=0;
			return(s);
		}

		if(opl==3)
		{
			*rtokty=4;
			*t++=*s++;
			*t++=*s++;
			*t++=*s++;
			*t=0;
			return(s);
		}

		if(opl==4)
		{
			*rtokty=4;
			*t++=*s++;
			*t++=*s++;
			*t++=*s++;
			*t++=*s++;
			*t=0;
			return(s);
		}
	}

	*rtokty=4;
	*t++=*s++;
	*t=0;
	return(s);
}

char **TKUCC_TokSplitLine(char *str)
{
	char tbuf[256];
	char *ta[64];
	char *s;
	char **a;
	int i, n, tty;
	
	tty=0;
	s=str; n=0;
	while(*s)
	{
		s=TKUCC_TokReadToken(s, tbuf+1, &tty);
		if(!tty)
			continue;
		tbuf[0]='A'+tty;
		ta[n++]=tkucc_rstrdup(tbuf);
	}
	ta[n]=NULL;
	
	a=tkucc_ralloc((n+1)*sizeof(char *));
	for(i=0; i<=n; i++)
	{
		s=ta[i];
		if(s)s=s+1;
		a[i]=s;
	}
	return(a);
}

int TKUCC_TokenNameP(char *tok)
{
	if(*(tok-1)=='B')
		return(1);
	return(0);
}

int TKUCC_TokenNumberP(char *tok)
{
	if(*(tok-1)=='C')
		return(1);
	return(0);
}

int TKUCC_TokenStringP(char *tok)
{
	if(*(tok-1)=='D')
		return(1);
	return(0);
}

int TKUCC_TokenOperatorP(char *tok)
{
	if(*(tok-1)=='E')
		return(1);
	return(0);
}

int TKUCC_TokenCheckOperatorP(char *tok, char *sref)
{
	if(*(tok-1)!='E')
		return(0);
	if(!strcmp(tok, sref))
		return(1);
	return(0);
}

int TKUCC_TokenCheckNameP(char *tok, char *sref)
{
	if(*(tok-1)!='D')
		return(0);
	if(!strcmp(tok, sref))
		return(1);
	return(0);
}

char *TKUCC_TokFlattenTokenBufNoPad(char *buf, char *tok)
{
	char *t, *s;
	int ch;
	
	t=buf; s=tok;
	if(TKUCC_TokenStringP(tok))
	{
		*t++='\"';
		ch=*s++;
		while(ch)
		{
			if(ch<' ')
			{
				switch(ch)
				{
				case '\n':
					*t++='\\';
					*t++='n';
					break;
				case '\r':
					*t++='\\';
					*t++='r';
					break;
				case '\t':
					*t++='\\';
					*t++='t';
					break;
				default:
					sprintf(t, "\\x%02X", ch);
					t+=strlen(t);
					break;
				}
			}else if((ch=='\"') || ch=='\'')
			{
				*t++='\\';
				*t++=ch;
			}else
			{
				*t++=ch;
			}
			ch=*s++;
		}

		*t++='\"';
		*t=0;
		return(t);
	}
	
	strcpy(t, s);
	t+=strlen(t);
	return(t);
}

char *TKUCC_TokFlattenTokenBuf(char *buf, char *tok)
{
	char *t;

	t=buf;

	if(TKUCC_TokNameChar(*(t-1)) && !TKUCC_TokenOperatorP(tok))
	{
		*t++=' ';
	}

	t=TKUCC_TokFlattenTokenBufNoPad(t, tok);
	return(t);
}

char *TKUCC_TokFlattenTokenListBuf(char *buf, char **tok)
{
	char **a;
	char *t, *s0;
	int ch;
	
	a=tok; t=buf;
	while(*a)
	{
		s0=*a++;
		t=TKUCC_TokFlattenTokenBuf(t, s0);
	}

	return(t);
}

int TKUCC_TokIndexKeyword(char *tok, int kwfl)
{
	int c0, c1;

	if(!TKUCC_TokenNameP(tok))
		return(0);
		
	c0=tok[0];
	c1=tok[1];
	
	if(c0<'i')
	{
		if(c0<'d')
		{
			if(c0=='_')
			{
				return(0);
			}
			
			if(c0=='a')
			{
				if(c1=='u')
				{
					if(!strcmp(tok, "auto"))
						return(TKUCC_KW_AUTO);
					return(0);
				}
				return(0);
			}

			if(c0=='b')
			{
				if(c1=='r')
				{
					if(!strcmp(tok, "break"))
						return(TKUCC_KW_BREAK);
					return(0);
				}
				return(0);
			}

			if(c0=='c')
			{
				if(c1=='a')
				{
					if(!strcmp(tok, "case"))
						return(TKUCC_KW_CASE);
					return(0);
				}
				if(c1=='h')
				{
					if(!strcmp(tok, "char"))
						return(TKUCC_KW_CHAR);
					return(0);
				}
				if(c1=='o')
				{
					if(!strcmp(tok, "const"))
						return(TKUCC_KW_CONST);
					if(!strcmp(tok, "continue"))
						return(TKUCC_KW_CONTINUE);
					return(0);
				}
				return(0);
			}
		}else
		{
			if(c0=='d')
			{
				if(c1=='e')
				{
					if(!strcmp(tok, "default"))
						return(TKUCC_KW_DEFAULT);
					return(0);
				}
				if(c1=='o')
				{
					if(!strcmp(tok, "do"))
						return(TKUCC_KW_DO);
					if(!strcmp(tok, "double"))
						return(TKUCC_KW_DOUBLE);
					return(0);
				}
				return(0);
			}

			if(c0=='e')
			{
				if(c1=='l')
				{
					if(!strcmp(tok, "else"))
						return(TKUCC_KW_ELSE);
					return(0);
				}
				if(c1=='x')
				{
					if(!strcmp(tok, "extern"))
						return(TKUCC_KW_EXTERN);
					return(0);
				}

				return(0);
			}

			if(c0=='f')
			{
				if(c1=='l')
				{
					if(!strcmp(tok, "float"))
						return(TKUCC_KW_FLOAT);
					return(0);
				}
				if(c1=='o')
				{
					if(!strcmp(tok, "for"))
						return(TKUCC_KW_FOR);
					return(0);
				}

				return(0);
			}

			if(c0=='g')
			{
				if(c1=='o')
				{
					if(!strcmp(tok, "goto"))
						return(TKUCC_KW_GOTO);
					return(0);
				}

				return(0);
			}
		}
	}else
	{
		if(c0<'t')
		{
			if(c0=='i')
			{
				if(c1=='f')
				{
					if(!strcmp(tok, "if"))
						return(TKUCC_KW_IF);
					return(0);
				}
				if(c1=='n')
				{
					if(!strcmp(tok, "int"))
						return(TKUCC_KW_INT);
					return(0);
				}

				return(0);
			}

			if(c0=='l')
			{
				if(c1=='o')
				{
					if(!strcmp(tok, "long"))
						return(TKUCC_KW_LONG);
					return(0);
				}
				return(0);
			}

			if(c0=='r')
			{
				if(c1=='e')
				{
					if(!strcmp(tok, "register"))
						return(TKUCC_KW_REGISTER);
					if(!strcmp(tok, "return"))
						return(TKUCC_KW_RETURN);
					return(0);
				}
				return(0);
			}

			if(c0=='s')
			{
				if(c1=='h')
				{
					if(!strcmp(tok, "short"))
						return(TKUCC_KW_SHORT);
					return(0);
				}
				if(c1=='i')
				{
					if(!strcmp(tok, "signed"))
						return(TKUCC_KW_SIGNED);
					if(!strcmp(tok, "sizeof"))
						return(TKUCC_KW_SIZEOF);
					return(0);
				}
				if(c1=='t')
				{
					if(!strcmp(tok, "static"))
						return(TKUCC_KW_STATIC);
					if(!strcmp(tok, "struct"))
						return(TKUCC_KW_STRUCT);
					return(0);
				}
				if(c1=='w')
				{
					if(!strcmp(tok, "switch"))
						return(TKUCC_KW_SWITCH);
					return(0);
				}
				return(0);
			}
		}else
		{
			if(c0=='t')
			{
				if(c1=='y')
				{
					if(!strcmp(tok, "typedef"))
						return(TKUCC_KW_TYPEDEF);
					return(0);
				}
				return(0);
			}

			if(c0=='u')
			{
				if(c1=='n')
				{
					if(!strcmp(tok, "union"))
						return(TKUCC_KW_UNION);
					if(!strcmp(tok, "unsigned"))
						return(TKUCC_KW_UNSIGNED);
					return(0);
				}
				return(0);
			}

			if(c0=='v')
			{
				if(c1=='o')
				{
					if(!strcmp(tok, "void"))
						return(TKUCC_KW_VOID);
					if(!strcmp(tok, "volatile"))
						return(TKUCC_KW_VOLATILE);
					return(0);
				}
				return(0);
			}

			if(c0=='w')
			{
				if(c1=='h')
				{
					if(!strcmp(tok, "while"))
						return(TKUCC_KW_WHILE);
					return(0);
				}
				return(0);
			}
		}
	}

	return(0);
}

s64 tkucc_atoll(char *str)
{
	char *s;
	s64 val;
	int i, ch, base, sg;

	s=str;

	if(*s=='0')
	{
		s++;
		base=8;

		if((*s=='x') || *s=='X')
		{
			s++;
			base=16;
		}else
			if((*s=='d') || *s=='D')
		{
			s++;
			base=10;
		}else
			if((*s=='b') || *s=='B')
		{
			s++;
			base=2;
		}
		
		val=0;
		while(*s)
		{
			ch=*s++;
			i=-1;
			if((ch>='0') && (ch<='9'))
				i=ch-'0';
			if((ch>='A') && (ch<='Z'))
				i=10+(ch-'A');
			if((ch>='a') && (ch<='z'))
				i=10+(ch-'a');
			if(i<0)
				break;
			if(i>=base)
				break;
			val=(val*base)+i;
		}
		return(val);
	}
	
	sg=0;
	if(*s=='-')
	{
		s++;
		sg=1;
	}

	val=0;
	while(*s)
	{
		ch=*s++;
		i=-1;
		if((ch>='0') && (ch<='9'))
			i=ch-'0';
		if(i<0)
			break;
		val=(val*10)+i;
	}
	if(sg)
		val=-val;
	return(val);
}


