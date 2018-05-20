#include <bgbccc.h>

#ifdef WIN32
#include <windows.h>
// #define CATCH_BLOCK_SEH
#endif

BGBCP_ParseItem *bgbcp_stmts=NULL;
BGBCP_ParseItem *bgbcp_blkstmts=NULL;

char *bgbcp_curfilename=NULL;

int bgbcp_warn, bgbcp_err;

void BGBCP_Warn(char *se, char *str, ...)
{
	va_list lst;
	char *fn;
	int ln;

	va_start(lst, str);

	bgbcp_warn++;
	fn=BGBCP_GetLastFileName();
	ln=BGBCP_GetLastLineNumber();

	printf("%s:%d: warning: ", fn, ln);
	vprintf(str, lst);

	va_end(lst);
}

void BGBCP_Error(char *se, char *str, ...)
{
	va_list lst;
	char *fn;
	int ln;

	va_start(lst, str);

	bgbcp_err++;
	fn=BGBCP_GetLastFileName();
	ln=BGBCP_GetLastLineNumber();

	printf("%s:%d: error: ", fn, ln);
	vprintf(str, lst);

	va_end(lst);
	fflush(stdout);

//	*(int *)-1=-1;
	BGBCC_DieError();
}

void BGBCP_WarnCtx(BGBCP_ParseState *ctx, char *se, char *str, ...)
{
	va_list lst;
	char *fn;
	int ln;

	va_start(lst, str);

	bgbcp_warn++;
	fn=BGBCP_GetLastFileName();
	ln=BGBCP_GetLastLineNumber();

	printf("%s:%d: warning: ", fn, ln);
	vprintf(str, lst);

	va_end(lst);
	fflush(stdout);

//	*(int *)-1=-1;
//	BGBCC_DieError();
}

void BGBCP_ErrorCtx(BGBCP_ParseState *ctx, char *se, char *str, ...)
{
	va_list lst;
	char *fn;
	int ln;

	va_start(lst, str);

	bgbcp_err++;
	fn=BGBCP_GetLastFileName();
	ln=BGBCP_GetLastLineNumber();

	printf("%s:%d: error: ", fn, ln);
	vprintf(str, lst);

	va_end(lst);
	fflush(stdout);

//	*(int *)-1=-1;
	BGBCC_DieError();
}

int BGBCP_AddStatement(char *name,
	BCCX_Node *(*func)(BGBCP_ParseState *ctx, char **s))
{
	BGBCP_ParseItem *tmp;

	tmp=malloc(sizeof(BGBCP_ParseItem));
	tmp->name=strdup(name);
	tmp->func=func;

	tmp->next=bgbcp_stmts;
	bgbcp_stmts=tmp;

	return(0);
}

BCCX_Node *BGBCP_ParseStatementName(
	BGBCP_ParseState *ctx, char *name, char **s)
{
	BGBCP_ParseItem *cur;

	cur=bgbcp_stmts;
	while(cur)
	{
		if(!bgbcp_strcmp(name, cur->name))
			return(cur->func(ctx, s));
		cur=cur->next;
	}
	return(NULL);
}

int BGBCP_AddBlockStatement(char *name,
	BCCX_Node *(*func)(BGBCP_ParseState *ctx, char **s))
{
	BGBCP_ParseItem *tmp;

	tmp=malloc(sizeof(BGBCP_ParseItem));
	tmp->name=strdup(name);
	tmp->func=func;

	tmp->next=bgbcp_blkstmts;
	bgbcp_blkstmts=tmp;

	return(0);
}

BCCX_Node *BGBCP_ParseBlockStatementName(
	BGBCP_ParseState *ctx, char *name, char **s)
{
	BGBCP_ParseItem *cur;

	cur=bgbcp_blkstmts;
	while(cur)
	{
		if(!bgbcp_strcmp(name, cur->name))
			return(cur->func(ctx, s));
		cur=cur->next;
	}
	return(NULL);
}

char *BGBCP_EatSemicolon(char *s)
{
	if(!s)
		return(s);
	s=BGBCP_EatWhite(s);
	if(*s==';')s++;
	s=BGBCP_EatWhite(s);
	return(s);
}

char *BGBCP_EatSemicolonRequired(BGBCP_ParseState *ctx, char *s)
{
	if(!s)
		return(s);
	s=BGBCP_EatWhite(s);
	if(*s==';')
	{
		s++;
	}else
	{
		BGBCP_ErrorCtx(ctx, s, "Missing expected ';'\n");
	}
	s=BGBCP_EatWhite(s);
	return(s);
}

char *BGBCP_EatExpectToken(BGBCP_ParseState *ctx, char *s, char *tok)
{
	char b[256], b2[256];
//	char *s, *t;
	int ty, ty2;

	if(!s)
		return(s);

	s=BGBCP_Token(s, b, &ty); //token
	if(bgbcp_strcmp(b, tok))
		{ BGBCP_ErrorCtx(ctx, s, "Expected '%s' got '%s'\n", tok, b); }

	return(s);
}

BCCX_Node *BGBCP_Statement(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2;

	s=*str;
	s=BGBCP_Token(s, b, &ty);
	BGBCP_Token(s, b2, &ty2);
	if(!*s)
	{
		*str=s;
		return(NULL);
	}

	if(ty!=BTK_NAME)
	{
		s=*str;
//		n=BGBCP_Expression(ctx, &s);
		n=BGBCP_Expression2(ctx, &s);
		*str=s;
		return(n);
	}

	n=BGBCP_ParseStatementName(ctx, b, &s);
	if(n!=NULL)
	{
		*str=s;
		return(n);
	}

	if(!bgbcp_strcmp(b, "break"))
	{
		n=BCCX_NewCst(&bgbcc_rcst_break, "break");
		*str=s;
		return(n);
	}

	if(!bgbcp_strcmp(b, "continue"))
	{
		n=BCCX_NewCst(&bgbcc_rcst_continue, "continue");
		*str=s;
		return(n);
	}

	if(!bgbcp_strcmp4(b, "goto"))
	{
		s=BGBCP_Token(s, b2, &ty2);
//		n=BCCX_NewCst1(&bgbcc_rcst_goto, "goto", BCCX_NewText(b2));
		n=BCCX_NewCst(&bgbcc_rcst_goto, "goto");
		BCCX_SetCst(n, &bgbcc_rcst_name, "name", b2);
		*str=s;
		return(n);
	}

	if(!bgbcp_strcmp(b, "return"))
	{
		BGBCP_Token(s, b2, &ty2);
		if(b2[0]!=';')
		{
			n1=BGBCP_Expression(ctx, &s);
			n=BCCX_NewCst1(&bgbcc_rcst_return, "return",
				BCCX_NewCst1V(&bgbcc_rcst_value, "value", n1));
		}else n=BCCX_NewCst(&bgbcc_rcst_return, "return");

		*str=s;
		return(n);
	}


	s=*str;
	s=BGBCP_EatWhite(s);
	if(!s || !*s)return(NULL);

//	n=BGBCP_Expression(ctx, &s);
	n=BGBCP_Expression2(ctx, &s);
	if(n!=NULL)
	{
		*str=s;
		return(n);
	}

	BGBCP_Error(s, "PDSCR_CParse_Statement: Unrecognizable "
		"statement type\n");
	*str=NULL;
	return(NULL);
}

BCCX_Node *BGBCP_ParseComment(BGBCP_ParseState *ctx, char **str)
{
	char b[65536];
	char *s, *t, *s0, *s1, *s2;
	BCCX_Node *n, *n1;
	int i;

	s=*str;

#if 0
	s1=s;
	while(*s1 && (*s1<=' '))s1++;
	if((s1[0]=='/') && (s1[1]=='/'))
	{
		s=s1+2; t=b;
		while(*s && (*s!='\n')) *t++=*s++;
		*t++=0;

		n=BCCX_NewCst(&bgbcc_rcst_linecomment, "linecomment");
		BCCX_SetCst(n, &bgbcc_rcst_value, "value", b);
		*str=s;
		return(n);
	}

	if((s1[0]=='/') && (s1[1]=='*'))
	{
		s=s1+2; t=b;
		while(*s)
		{
			if((s[0]=='*') && (s[1]=='/'))
				{ s+=2; break; }
			*t++=*s++;
		}
		*t++=0;

		if(BGBCP_SkimLinenum(s1, NULL, NULL))
		{
			//n=BGBCP_BlockStatementInner(ctx, &s);
			*str=s;
			return(NULL);
		}

		n=BCCX_NewCst(&bgbcc_rcst_blockcomment, "blockcomment");
		BCCX_SetCst(n, &bgbcc_rcst_value, "value", b);
		*str=s;
		return(n);
	}
#endif

	return(NULL);
}

BCCX_Node *BGBCP_ParseDocComment(BGBCP_ParseState *ctx, char **str)
{
	char b[65536];
	char *s, *t, *s0, *s1, *s2;
	BCCX_Node *n, *n1;
	int i;

	s=*str;

	s1=s;
	while(*s1 && (*s1<=' '))s1++;
	if((s1[0]=='/') && (s1[1]=='*') && (s1[2]=='*') && (s1[3]!='*'))
	{
		s=s1+3; t=b;
		while(*s)
		{
			if((s[0]=='*') && (s[1]=='/'))
				{ s+=2; break; }
			*t++=*s++;
		}
		*t++=0;

		n=BCCX_NewCst(&bgbcc_rcst_doc_comment, "doc_comment");
		BCCX_SetCst(n, &bgbcc_rcst_value, "value", b);
		*str=s;
		return(n);
	}

	return(NULL);
}

BCCX_Node *BGBCP_BlockStatementInner(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *ab[16];
	char **a;
	char *s, *t, *s0, *s1, *s2;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3, *n4;
	int i;

	s=*str;

	n=BGBCP_ParseComment(ctx, &s);
	if(n)
	{
		*str=s;
		return(n);
	}

	s=BGBCP_Token(s, b, &ty);
	BGBCP_Token(s, b2, &ty2);
	if(!*s)
	{
		*str=s;
		return(NULL);
	}

	if(ty==BTK_SEPERATOR)
	{
		*str=s;
		return(NULL);
	}

	if((ty==BTK_NAME) && !bgbcp_strcmp1(b2, ":"))
	{
		s=BGBCP_Token(s, b2, &ty2);	//:

		if(!bgbcp_strcmp(b, "default"))
		{
			n=BCCX_NewCst(&bgbcc_rcst_case_default, "case_default");
			*str=s;
			return(n);
		}

		n=BCCX_NewCst(&bgbcc_rcst_label, "label");
		BCCX_SetCst(n, &bgbcc_rcst_name, "name", b);

		*str=s;
		return(n);
	}

	if(ty==BTK_NAME)
	{

		if((b[0]=='t') && !bgbcp_strcmp7(b, "typedef") &&
			(ctx->lang==BGBCC_LANG_C))
		{
			n=BGBCP_ForceDefinition(ctx, &s);
			BGBCP_HandleTypedef(ctx, n);

			*str=s;
			return(NULL);
		}

		if((b[0]=='c') && !bgbcp_strcmp4(b, "case"))
		{
			n1=BGBCP_Expression(ctx, &s);
			s=BGBCP_Token(s, b2, &ty2);	//:
			n=BCCX_NewCst1(&bgbcc_rcst_case, "case",
				BCCX_NewCst1V(&bgbcc_rcst_value, "value", n1));

			*str=s;
			return(n);
		}

		if((b[0]=='i') && !bgbcp_strcmp2(b, "if"))
		{
			s=BGBCP_Token(s, b, &ty);	//(
			n1=BGBCP_Expression(ctx, &s);
			s=BGBCP_Token(s, b, &ty);	//)
			n2=BGBCP_BlockStatement3(ctx, &s);

			BGBCP_Token(s, b, &ty);
			if(bgbcp_strcmp4(b, "else"))
			{
				n=BCCX_NewCst2(&bgbcc_rcst_if, "if",
					BCCX_NewCst1V(&bgbcc_rcst_cond, "cond", n1),
					BCCX_NewCst1V(&bgbcc_rcst_then, "then", n2));
				*str=s;
				return(n);
			}

			s=BGBCP_Token(s, b, &ty);	//else
			n3=BGBCP_BlockStatement3(ctx, &s);
			n=BCCX_NewCst3(&bgbcc_rcst_if, "if",
				BCCX_NewCst1V(&bgbcc_rcst_cond, "cond", n1),
				BCCX_NewCst1V(&bgbcc_rcst_then, "then", n2),
				BCCX_NewCst1V(&bgbcc_rcst_else, "else", n3));

			*str=s;
			return(n);
		}

		if((b[0]=='w') && !bgbcp_strcmp5(b, "while"))
		{
			s=BGBCP_Token(s, b, &ty);	//'('
			n1=BGBCP_Expression(ctx, &s);
			s=BGBCP_Token(s, b, &ty);	//')'

			n2=BGBCP_BlockStatement3(ctx, &s);
			n=BCCX_NewCst2(&bgbcc_rcst_while, "while",
				BCCX_NewCst1V(&bgbcc_rcst_cond, "cond", n1),
				BCCX_NewCst1V(&bgbcc_rcst_body, "body", n2));

			*str=s;
			return(n);
		}

		if((b[0]=='f') && !bgbcp_strcmp3(b, "for"))
		{
			s=BGBCP_Token(s, b, &ty);		//(
			s2=BGBCP_Token(s, b, &ty);		//
			s2=BGBCP_Token(s2, b2, &ty);	//

			if(!bgbcp_strcmp1(b, ";"))
				{ n1=NULL; }
			else
				{ n1=BGBCP_Expression2(ctx, &s); }
			s=BGBCP_EatSemicolonRequired(ctx, s);

			s2=BGBCP_Token(s, b, &ty);
			if(!bgbcp_strcmp1(b, ";"))
				{ n2=NULL; }
			else
				{ n2=BGBCP_Expression2(ctx, &s); }
			s=BGBCP_EatSemicolonRequired(ctx, s);

			s2=BGBCP_Token(s, b, &ty);
			if(!bgbcp_strcmp1(b, ")"))
				{ n3=NULL; }
			else
				{ n3=BGBCP_Expression2(ctx, &s); }

			s=BGBCP_Token(s, b, &ty);	//)
			n4=BGBCP_BlockStatement3(ctx, &s);

			n=BCCX_NewCst4(&bgbcc_rcst_for, "for",
				BCCX_NewCst1V(&bgbcc_rcst_init, "init", n1),
				BCCX_NewCst1V(&bgbcc_rcst_cond, "cond", n2),
				BCCX_NewCst1V(&bgbcc_rcst_step, "step", n3),
				BCCX_NewCst1V(&bgbcc_rcst_body, "body", n4));

			*str=s;
			return(n);
		}

		if((b[0]=='d') && !bgbcp_strcmp2(b, "do"))
		{
			n2=BGBCP_BlockStatement3(ctx, &s);

			BGBCP_Token(s, b, &ty);	//'while'
			if(!bgbcp_strcmp(b, "while"))
			{
				s=BGBCP_Token(s, b, &ty);	//'while'

				s=BGBCP_Token(s, b, &ty);	//'('
				n1=BGBCP_Expression(ctx, &s);
				s=BGBCP_Token(s, b, &ty);	//')'

				n=BCCX_NewCst2(&bgbcc_rcst_do_while, "do_while",
					BCCX_NewCst1V(&bgbcc_rcst_cond, "cond", n1),
					BCCX_NewCst1V(&bgbcc_rcst_body, "body", n2));
			}else
			{
				n=n2;
			}

			*str=s;
			return(n);
		}

		if((b[0]=='s') && !bgbcp_strcmp6(b, "switch"))
		{
			s=BGBCP_Token(s, b, &ty);	//(
			n1=BGBCP_Expression(ctx, &s);
			s=BGBCP_Token(s, b, &ty);	//)

			s=BGBCP_Token(s, b, &ty); //{
			n2=BGBCP_Block(ctx, &s);

			n=BCCX_NewCst2(&bgbcc_rcst_switch, "switch",
				BCCX_NewCst1V(&bgbcc_rcst_cond, "cond", n1),
				BCCX_NewCst1V(&bgbcc_rcst_body, "body", n2));

			*str=s;
			return(n);
		}

		if(b[0]=='_')
		{
			if(!bgbcp_strcmp(b, "__asm__"))
			{
	//			s=BGBCP_Token(s, b, &ty);	//(
	//			n1=BGBCP_Expression(ctx, &s);
	//			s=BGBCP_Token(s, b, &ty);	//)

	//			s=BGBCP_Token(s, b, &ty);	//(

				i=0;
				while(1)
				{
					s=BGBCP_Token(s, b, &ty);
					if(!bgbcp_strcmp1(b, "("))i++;
					if(!bgbcp_strcmp1(b, ")"))
					{
						i--;
						if(!i)break;
					}
				}

				n=BCCX_NewCst(&bgbcc_rcst_gcc_asm, "gcc_asm");

				*str=s;
				return(n);
			}

			if(!bgbcp_strcmp(b, "__asm"))
			{
				i=0;
				while(1)
				{
					s=BGBCP_Token(s, b, &ty);
					if(!bgbcp_strcmp1(b, "{"))i++;
					if(!bgbcp_strcmp1(b, "}"))
					{
						i--;
						if(!i)break;
					}
				}

				n=BCCX_NewCst(&bgbcc_rcst_msvc_asm, "msvc_asm");

				*str=s;
				return(n);
			}

			if(	!bgbcp_strcmp(b, "__extern_c") ||
				!bgbcp_strcmp(b, "__extern_cpp") ||
				!bgbcp_strcmp(b, "__extern_java") ||
				!bgbcp_strcmp(b, "__extern_cs") ||
				!bgbcp_strcmp(b, "__extern_bs") ||
				!bgbcp_strcmp(b, "__extern_bs2"))
			{
				i=ctx->lang;
				ctx->lang=BGBCP_LangForName(b+strlen("__extern_"));
				n1=BGBCP_BlockStatement2(ctx, &s);
				n=BCCX_NewCst1(&bgbcc_rcst_extern2_lang,
					"extern2_lang", n1);
				BCCX_SetCst(n, &bgbcc_rcst_lang, "lang",
					BGBCP_NameForLang(ctx->lang));

				ctx->lang=i;
				return(n);
			}

			if(!bgbcp_strcmp(b, "__pragma"))
			{
				s=*str;
				n1=BGBCP_Expression(ctx, &s);
				n=BCCX_NewCst1(&bgbcc_rcst_pragma, "pragma", n1);
				*str=s;
				return(n);
			}
		}

		if((ctx->lang==BGBCC_LANG_CPP) ||
			(ctx->lang==BGBCC_LANG_CS) ||
			(ctx->lang==BGBCC_LANG_CX))
		{
			if(!bgbcp_strcmp(b, "using"))
			{
				s=BGBCP_Token(s, b, &ty);	//'name'
				BGBCP_Token(s, b2, &ty2);

				while(!bgbcp_strcmp1(b2, "."))
				{
					s=BGBCP_Token(s, b2, &ty2);	//.
					s=BGBCP_Token(s, b2, &ty2);
					strcat(b, "/");
					strcat(b, b2);

					BGBCP_Token(s, b2, &ty2);
				}

				s1=bgbcc_strdup(b);

				for(i=0; ctx->cur_nsi[i]; i++);
				ctx->cur_nsi[i++]=s1;
				ctx->cur_nsi[i]=NULL;

				n=BCCX_NewCst(&bgbcc_rcst_using, "using");
				BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);

//				s=BGBCP_EatSemicolon(s);
				s=BGBCP_EatSemicolonRequired(ctx, s);

				*str=s;
				return(n);
			}

			if(!bgbcp_strcmp(b, "namespace"))
			{
				s=BGBCP_Token(s, b, &ty);	//'name'
				BGBCP_Token(s, b2, &ty2);

				while(!bgbcp_strcmp1(b2, "."))
				{
					s=BGBCP_Token(s, b2, &ty2);	//.
					s=BGBCP_Token(s, b2, &ty2);
					strcat(b, "/");
					strcat(b, b2);

					BGBCP_Token(s, b2, &ty2);
				}

				s0=ctx->cur_ns;
				s1=bgbcc_strdup(b);
				ctx->cur_ns=s1;

				a=ctx->cur_nsi;
				ctx->cur_nsi=ab;
				ctx->cur_nsi[0]=NULL;

				s=BGBCP_Token(s, b, &ty); //{
				n1=BGBCP_Block(ctx, &s);
				n=BCCX_NewCst1(&bgbcc_rcst_namespace, "namespace", n1);
				BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);

				ctx->cur_ns=s0;
				ctx->cur_nsi=a;

				*str=s;
				return(n);
			}
		}

		if(	(ctx->lang==BGBCC_LANG_JAVA) ||
			(ctx->lang==BGBCC_LANG_CX))
		{
			if(!bgbcp_strcmp6(b, "import"))
			{
				s=BGBCP_Token(s, b, &ty);	//'name'
				BGBCP_Token(s, b2, &ty2);

				while(!bgbcp_strcmp1(b2, "."))
				{
					s=BGBCP_Token(s, b2, &ty2);	//.
					s=BGBCP_Token(s, b2, &ty2);
					strcat(b, "/");
					strcat(b, b2);

					BGBCP_Token(s, b2, &ty2);
				}

				s1=bgbcc_strdup(b);

				for(i=0; ctx->cur_nsi[i]; i++);
				ctx->cur_nsi[i++]=s1;
				ctx->cur_nsi[i]=NULL;

				n=BCCX_NewCst(&bgbcc_rcst_import, "import");
				BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);

//				s=BGBCP_EatSemicolon(s);
				s=BGBCP_EatSemicolonRequired(ctx, s);

				*str=s;
				return(n);
			}
		}
	}

	if(!bgbcp_strcmp1(b, "{"))
	{
		s=*str;
		n=BGBCP_BlockStatement2(ctx, &s);
		*str=s;
		return(n);
	}

	n=BGBCP_ParseBlockStatementName(ctx, b, &s);
	if(n)
	{
		*str=s;
		return(n);
	}


	s=*str;

#if 0
	n=BGBCP_Definition(ctx, &s);
	if(n!=NULL)
	{
		s=BGBCP_EatSemicolon(s);
		*str=s;
		return(n);
	}
#endif

	n=BGBCP_Statement(ctx, &s);
//	s=BGBCP_EatSemicolon(s);
	s=BGBCP_EatSemicolonRequired(ctx, s);
	*str=s;
	return(n);
}

BCCX_Node *BGBCP_BlockStatementI(BGBCP_ParseState *ctx, char **str, int flag)
{
	char b[256];
	int ty, ln;
	BCCX_Node *n, *n1;
	char *s, *s1, *fn, *t;
	char **a;
	int i;

	s=*str;

#if 1
	BGBCP_Token(s, b, &ty);
	while(!bgbcp_strcmp1(b, "#"))
	{
		s=BGBCP_Token(s, b, &ty);
		while(*s && (*s!='\n'))s++;

		b[0]=0;
		BGBCP_Token(s, b, &ty);
	}
#endif

//	s=BGBCP_EatWhite(s);
	BGBCP_EatWhite(s);
	fn=BGBCP_GetLastFileName();
	ln=BGBCP_GetLastLineNumber();

//	printf("@ %s:%d\n", fn, ln);

#if 1
	n=BGBCP_Definition(ctx, &s);

	if(!n && (flag&1))
	{
		n=BGBCP_DefinitionOldStyle(ctx, &s);
	}

	if(n!=NULL)
	{
		BCCX_SetCst(n, &bgbcc_rcst_fn, "fn", fn);
		BCCX_SetIntCst(n, &bgbcc_rcst_ln, "ln", ln);

		s=BGBCP_EatSemicolon(s);
//		s=BGBCP_EatSemicolonRequired(ctx, s);
		*str=s;
		return(n);
	}
#endif

	s1=ctx->cur_ns; a=ctx->cur_nsi;
	ctx->cur_ns=NULL; ctx->cur_nsi=NULL;

	n=BGBCP_BlockStatementInner(ctx, &s);

	ctx->cur_ns=s1; ctx->cur_nsi=a;

	if(n)
	{
		BCCX_SetCst(n, &bgbcc_rcst_fn, "fn", fn);
		BCCX_SetIntCst(n, &bgbcc_rcst_ln, "ln", ln);
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_BlockStatement(BGBCP_ParseState *ctx, char **str)
{
	BCCX_Node *n;
	n=BGBCP_BlockStatementI(ctx, str, 0);
	return(n);
}

BCCX_Node *BGBCP_BlockI(BGBCP_ParseState *ctx, char **str, int flag)
{
	char b[256];
	char *s, *s1;
	int ty, ty2, i;
	BCCX_Node *n, *lst, *lste;

	s=*str; s1=s;
	lst=NULL; lste=NULL;
	while(1)
	{
		if(!s)
			break;
		BGBCP_Token(s, b, &ty);
		if(!*s || (*b=='}'))
		{
			s=BGBCP_Token(s, b, &ty);
			break;
		}

		s1=BGBCP_EatWhite(s);
		if(!*s1) { s=s1; break; }

//#if 0
#ifdef CATCH_BLOCK_SEH
		__try {
#endif

		n=BGBCP_BlockStatementI(ctx, &s, flag);
		if(n==NULL)
		{
			if(!s || s==s1)
			{
				BGBCP_Error(s, "PDSCR_CParse_Block: "
					"Unknown Parse Error\n");
				break;
			}
			continue;
		}
//		lst=BCCX_AddEnd(lst, n);
		lst=BCCX_AddEnd2(lst, &lste, n);

//#if 0
#ifdef CATCH_BLOCK_SEH
		} __except(EXCEPTION_EXECUTE_HANDLER)
		{
			printf("RECAUGHT\n");
			i=1;
			while(s && *s)
			{
				if(*s=='(') { i++; s++; continue; }
				if(*s==')') { i--; s++; continue; }
				if(*s=='[') { i++; s++; continue; }
				if(*s==']') { i--; s++; continue; }

				if(*s=='{') { i++; s++; continue; }
				if(*s=='}')
				{
					i--; s++;
					if(!i)break;
					continue;
				}
				if((i==1) && (*s==';'))
					{ s++; break; }
				s++;
			}
		}
#endif

	}

	*str=s;
	return(lst);
}

BCCX_Node *BGBCP_Block(BGBCP_ParseState *ctx, char **str)
{
	BCCX_Node *n;
	
	n=BGBCP_BlockI(ctx, str, 0);
	return(n);
}

BCCX_Node *BGBCP_Toplevel(BGBCP_ParseState *ctx, char **str)
{
	BCCX_Node *n;
	
	n=BGBCP_BlockI(ctx, str, 1);
	return(n);
}

BCCX_Node *BGBCP_BlockStatement2(BGBCP_ParseState *ctx, char **str)
{
	char b[256];
	char *s;
	int tk0, tk1, tk2;
	int ty;
	BCCX_Node *n;

//	if(	(ctx->lang==BGBCC_LANG_JAVA) ||
//		(ctx->lang==BGBCC_LANG_CS) ||
//		(ctx->lang==BGBCC_LANG_BS) ||
//		(ctx->lang==BGBCC_LANG_BS2))
//			return(BGBJP_BlockStatement2(ctx, str));

	s=*str;

	BGBCP_Token(s, b, &ty);
	if(!bgbcp_strcmp1(b, "{"))
	{
		s=BGBCP_Token(s, b, &ty);

		tk0=BGBCP_GetTokenCount();
		n=BGBCP_Block(ctx, &s);
		tk1=BGBCP_GetTokenCount();

		n=BCCX_NewCst1(&bgbcc_rcst_begin, "begin", n);

		tk2=tk1-tk0;
		if(tk2>0)
			BCCX_SetIntCst(n, &bgbcc_rcst_tokens, "tokens", tk2);

		*str=s;
		return(n);
	}

	n=BGBCP_BlockStatement(ctx, str);
	return(n);
}

BCCX_Node *BGBCP_BlockStatement3(BGBCP_ParseState *ctx, char **str)
{
	char *s;
	BCCX_Node *n;

	s=*str;
	s=BGBCP_EatWhite(s);
	n=BGBCP_BlockStatement2(ctx, &s);
	*str=s;
	return(n);
}
