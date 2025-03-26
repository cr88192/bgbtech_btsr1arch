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

	fn=BGBCP_GetFilename();
//	if(fn)
	if(0)
	{
		ln=BGBCP_CalcLinenum(se);
	}else
	{
		fn=BGBCP_GetLastFileName();
//		ln=BGBCP_GetLastLineNumber();
		ln=BGBCP_GetPredictLineNumber(se);
	}

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

	fn=BGBCP_GetFilename();
//	if(fn)
	if(0)
	{
		ln=BGBCP_CalcLinenum(se);
	}else
	{
		fn=BGBCP_GetLastFileName();
//		ln=BGBCP_GetLastLineNumber();
		ln=BGBCP_GetPredictLineNumber(se);
	}

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

	fn=BGBCP_GetFilename();
//	if(fn)
	if(0)
	{
		ln=BGBCP_CalcLinenum(se);
	}else
	{
		fn=BGBCP_GetLastFileName();
//		ln=BGBCP_GetLastLineNumber();
		ln=BGBCP_GetPredictLineNumber(se);
	}

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

	fn=BGBCP_GetFilename();
//	if(fn)
	if(0)
	{
		ln=BGBCP_CalcLinenum(se);
	}else
	{
		fn=BGBCP_GetLastFileName();
//		ln=BGBCP_GetLastLineNumber();
		ln=BGBCP_GetPredictLineNumber(se);
	}

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
		if(	(ctx->lang==BGBCC_LANG_BS)	||
			(ctx->lang==BGBCC_LANG_BS2)	)
		{
			return(s);
		}

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

	s=BGBCP_Token2(s, b, &ty, ctx->lang); //token
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
	s=BGBCP_Token2(s, b, &ty, ctx->lang);
	BGBCP_Token2(s, b2, &ty2, ctx->lang);
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

//	if(!bgbcp_strcmp(b, "break"))
	if(!bgbcp_strcmp5(b, "break"))
	{
		n=BCCX_NewCst(&bgbcc_rcst_break, "break");
		*str=s;
		return(n);
	}

//	if(!bgbcp_strcmp(b, "continue"))
	if(!bgbcp_strcmp8(b, "continue"))
	{
		n=BCCX_NewCst(&bgbcc_rcst_continue, "continue");
		*str=s;
		return(n);
	}

	if(!bgbcp_strcmp4(b, "goto"))
	{
		s=BGBCP_Token2(s, b2, &ty2, ctx->lang);

		if(!bgbcp_strcmp4(b2, "case"))
		{
			n1=BGBCP_Expression(ctx, &s);
			n=BCCX_NewCst1(&bgbcc_rcst_goto_case, "goto_case",
				BCCX_NewCst1V(&bgbcc_rcst_value, "value", n1));
			*str=s;
			return(n);
		}

//		n=BCCX_NewCst1(&bgbcc_rcst_goto, "goto", BCCX_NewText(b2));
		n=BCCX_NewCst(&bgbcc_rcst_goto, "goto");
		BCCX_SetCst(n, &bgbcc_rcst_name, "name", b2);
		*str=s;
		return(n);
	}

//	if(!bgbcp_strcmp(b, "return"))
	if(!bgbcp_strcmp6(b, "return"))
	{
		BGBCP_Token2(s, b2, &ty2, ctx->lang);
		if(b2[0]!=';')
		{
			n1=BGBCP_Expression(ctx, &s);
			n=BCCX_NewCst1(&bgbcc_rcst_return, "return",
				BCCX_NewCst1V(&bgbcc_rcst_value, "value", n1));
		}else n=BCCX_NewCst(&bgbcc_rcst_return, "return");

		*str=s;
		return(n);
	}

	if((ctx->lang==BGBCC_LANG_CPP) ||
		(ctx->lang==BGBCC_LANG_CS) ||
		(ctx->lang==BGBCC_LANG_CX) ||
		(ctx->lang==BGBCC_LANG_JAVA) ||
		(ctx->lang==BGBCC_LANG_BS2))
	{
		if(!bgbcp_strcmp5(b, "throw"))
		{
			n1=BGBCP_Expression(ctx, &s);
			n=BCCX_NewCst1(&bgbcc_rcst_throw, "throw",
				BCCX_NewCst1V(&bgbcc_rcst_value, "value", n1));
			*str=s;
			return(n);
		}
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
	int ocnsi;
	int is_cpp_cx, is_c;
	int i;

	s=*str;

	n=BGBCP_ParseComment(ctx, &s);
	if(n)
	{
		*str=s;
		return(n);
	}

	s=BGBCP_Token2(s, b, &ty, ctx->lang);
	BGBCP_Token2(s, b2, &ty2, ctx->lang);
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
		s=BGBCP_Token2(s, b2, &ty2, ctx->lang);	//:

//		if(!bgbcp_strcmp(b, "default"))
		if(!bgbcp_strcmp7(b, "default"))
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
			((ctx->lang==BGBCC_LANG_C) ||
			(ctx->lang==BGBCC_LANG_CPP)))
		{
			n=BGBCP_ForceDefinition(ctx, &s);
			BGBCP_HandleTypedef(ctx, n);

			*str=s;
			return(NULL);
		}

		if((b[0]=='t') && !bgbcp_strcmp7(b, "template") &&
			(ctx->lang==BGBCC_LANG_CPP))
		{
			s=BGBCP_Token2(s, b2, &ty2, ctx->lang);	//'<'
			n1=BGBCP_TemplateArgs(ctx, &s);

			BGBCP_PushTemplateArgs(ctx, n1);
			n=BGBCP_ForceDefinition(ctx, &s);
			BGBCP_PopTemplateArgs(ctx);

			BGBCP_HandleTemplate(ctx, n, n1);

			*str=s;
			return(NULL);
		}

		if((b[0]=='c') && !bgbcp_strcmp4(b, "case"))
		{
			n1=BGBCP_Expression(ctx, &s);
			s=BGBCP_Token2(s, b2, &ty2, ctx->lang);	//:
			n=BCCX_NewCst1(&bgbcc_rcst_case, "case",
				BCCX_NewCst1V(&bgbcc_rcst_value, "value", n1));

			*str=s;
			return(n);
		}

		if((b[0]=='i') && !bgbcp_strcmp2(b, "if"))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//(
//			n1=BGBCP_Expression(ctx, &s);
			n1=BGBCP_Expression2(ctx, &s);
			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//)
			n2=BGBCP_BlockStatement3(ctx, &s);

			BGBCP_Token2(s, b, &ty, ctx->lang);
			if(bgbcp_strcmp4(b, "else"))
			{
				n=BCCX_NewCst2(&bgbcc_rcst_if, "if",
					BCCX_NewCst1V(&bgbcc_rcst_cond, "cond", n1),
					BCCX_NewCst1V(&bgbcc_rcst_then, "then", n2));
				*str=s;
				return(n);
			}

			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//else
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
			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//'('
//			n1=BGBCP_Expression(ctx, &s);
			n1=BGBCP_Expression2(ctx, &s);
			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//')'

			n2=BGBCP_BlockStatement3(ctx, &s);
			n=BCCX_NewCst2(&bgbcc_rcst_while, "while",
				BCCX_NewCst1V(&bgbcc_rcst_cond, "cond", n1),
				BCCX_NewCst1V(&bgbcc_rcst_body, "body", n2));

			*str=s;
			return(n);
		}

		if((b[0]=='f') && !bgbcp_strcmp3(b, "for"))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);		//(
			s2=BGBCP_Token2(s, b, &ty, ctx->lang);		//
			s2=BGBCP_Token(s2, b2, &ty);	//

			if(!bgbcp_strcmp1(b, ";"))
			{
				n1=NULL;
				s=BGBCP_EatSemicolonRequired(ctx, s);
			}
			else
			{
				n1=BGBCP_Definition(ctx, &s);
			
				if(!n1)
				{
					n1=BGBCP_Expression2(ctx, &s);
					s=BGBCP_EatSemicolonRequired(ctx, s);
				}
			}

			s2=BGBCP_Token2(s, b, &ty, ctx->lang);
			if(!bgbcp_strcmp1(b, ";"))
				{ n2=NULL; }
			else
				{ n2=BGBCP_Expression2(ctx, &s); }
			s=BGBCP_EatSemicolonRequired(ctx, s);

			s2=BGBCP_Token2(s, b, &ty, ctx->lang);
			if(!bgbcp_strcmp1(b, ")"))
				{ n3=NULL; }
			else
				{ n3=BGBCP_Expression2(ctx, &s); }

			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//)
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

			BGBCP_Token2(s, b, &ty, ctx->lang);	//'while'
//			if(!bgbcp_strcmp(b, "while"))
			if(!bgbcp_strcmp5(b, "while"))
			{
				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//'while'

				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//'('
//				n1=BGBCP_Expression(ctx, &s);
				n1=BGBCP_Expression2(ctx, &s);
				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//')'

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
			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//(
//			n1=BGBCP_Expression(ctx, &s);
			n1=BGBCP_Expression2(ctx, &s);
			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//)

			s=BGBCP_Token2(s, b, &ty, ctx->lang); //{
			n2=BGBCP_Block(ctx, &s);

			n=BCCX_NewCst2(&bgbcc_rcst_switch, "switch",
				BCCX_NewCst1V(&bgbcc_rcst_cond, "cond", n1),
				BCCX_NewCst1(&bgbcc_rcst_body, "body", n2));

			*str=s;
			return(n);
		}

		if(b[0]=='_')
		{
			if(!bgbcp_strcmp(b, "__asm__"))
			{
	//			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//(
	//			n1=BGBCP_Expression(ctx, &s);
	//			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//)

	//			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//(

				i=0;
				while(1)
				{
					s=BGBCP_Token2(s, b, &ty, ctx->lang);
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
				s2=malloc(65536);

				i=0;
				t=s2;
				while(1)
				{
					s=BGBCP_Token2(s, b, &ty, ctx->lang);
					if(!bgbcp_strcmp1(b, "{"))
					{
						i++;
						if(i==1)
						{
							s=BGBCP_EatWhite(s);
							continue;
						}
					}
					if(!bgbcp_strcmp1(b, "}"))
					{
						i--;
						if(!i)break;
						continue;
					}
					
					t=BGBPP_PrintToken(t, b, ty);
					while(*s && *s<=' ')
					{
						*t++=*s++;
					}
				}

				*t=0;

				n1=BCCX_NewText(s2);
				n=BCCX_NewCst1(&bgbcc_rcst_msvc_asm, "msvc_asm", n1);
//				BCCX_SetCst(n, &bgbcc_rcst_value, "value", stbuf);

				free(s2);

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

			if(	!bgbcp_strcmp(b, "__pragma") ||
				!bgbcp_strcmp(b, "_Pragma"))
			{
				s=*str;
				n1=BGBCP_Expression(ctx, &s);
				n=BCCX_NewCst1(&bgbcc_rcst_pragma, "pragma", n1);

				if(BCCX_TagIsCstP(n1, &bgbcc_rcst_funcall, "funcall"))
				{
					s0=BCCX_GetCst(n1, &bgbcc_rcst_name, "name");
					if(!strcmp(s0, "setlocale"))
					{
						n2=BCCX_FetchCst(n1, &bgbcc_rcst_args, "args");
						if(BCCX_TagIsCstP(n2, &bgbcc_rcst_string, "string"))
						{
							s2=BCCX_Get(n2, "value");
							BGBCP_SetLocale(ctx, s2);
						}
					}

					if(!strcmp(s0, "fenv_access"))
					{
						ctx->fenv_access=1;
					}
				}

				*str=s;
				return(n);
			}

#if 1
			if(!bgbcp_strcmp(b, "__ifarch"))
			{
				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//(
				n1=BGBCP_Expression2(ctx, &s);
				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//)
				n2=BGBCP_BlockStatement3(ctx, &s);

				BGBCP_Token2(s, b, &ty, ctx->lang);
				if(bgbcp_strcmp4(b, "else"))
				{
					n=BCCX_NewCst2(&bgbcc_rcst_ifarch, "ifarch",
						BCCX_NewCst1V(&bgbcc_rcst_cond, "cond", n1),
						BCCX_NewCst1V(&bgbcc_rcst_then, "then", n2));
					*str=s;
					return(n);
				}

				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//else
				n3=BGBCP_BlockStatement3(ctx, &s);
				n=BCCX_NewCst3(&bgbcc_rcst_ifarch, "ifarch",
					BCCX_NewCst1V(&bgbcc_rcst_cond, "cond", n1),
					BCCX_NewCst1V(&bgbcc_rcst_then, "then", n2),
					BCCX_NewCst1V(&bgbcc_rcst_else, "else", n3));

				*str=s;
				return(n);
			}
#endif

			if(!bgbcp_strcmp(b, "__switchz"))
			{
				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//(
	//			n1=BGBCP_Expression(ctx, &s);
				n1=BGBCP_Expression2(ctx, &s);
				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//)

				s=BGBCP_Token2(s, b, &ty, ctx->lang); //{
				n2=BGBCP_Block(ctx, &s);

				n=BCCX_NewCst2(&bgbcc_rcst_switchz, "switchz",
					BCCX_NewCst1V(&bgbcc_rcst_cond, "cond", n1),
					BCCX_NewCst1(&bgbcc_rcst_body, "body", n2));

				*str=s;
				return(n);
			}

			if(
				!bgbcp_strcmp(b, "__vlcase") ||
				!bgbcp_strcmp(b, "__vlcasez") )
			{
				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//(
	//			n1=BGBCP_Expression(ctx, &s);
				n1=BGBCP_Expression2(ctx, &s);
				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//)

				s=BGBCP_Token2(s, b, &ty, ctx->lang); //{
//				n2=BGBCP_Block(ctx, &s);
				n2=BGBCP_VlCaseBlock(ctx, &s);

				n=BCCX_NewCst2(&bgbcc_rcst_vlcasez, "vlcasez",
					BCCX_NewCst1V(&bgbcc_rcst_cond, "cond", n1),
					BCCX_NewCst1(&bgbcc_rcst_body, "body", n2));

				*str=s;
				return(n);
			}
		}

//		if(ctx->lang==BGBCC_LANG_VERILOG)
		if(1)
		{
			if(	(ctx->lang==BGBCC_LANG_VERILOG) &&
				(	!bgbcp_strcmp(b, "case") ||
					!bgbcp_strcmp(b, "casez")))
			{
				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//(
	//			n1=BGBCP_Expression(ctx, &s);
				n1=BGBCP_Expression2(ctx, &s);
				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//)

				n2=BGBCP_VlCaseBlock(ctx, &s);

				if(!bgbcp_strcmp(b, "casez"))
				{
					n=BCCX_NewCst2(&bgbcc_rcst_vlcasez, "vlcasez",
						BCCX_NewCst1V(&bgbcc_rcst_cond, "cond", n1),
						BCCX_NewCst1(&bgbcc_rcst_body, "body", n2));
				}else
				{
					n=BCCX_NewCst2(&bgbcc_rcst_vlcase, "vlcase",
						BCCX_NewCst1V(&bgbcc_rcst_cond, "cond", n1),
						BCCX_NewCst1(&bgbcc_rcst_body, "body", n2));
				}

				*str=s;
				return(n);
			}
			
			if(((ctx->lang==BGBCC_LANG_VERILOG) &&
				!bgbcp_strcmp(b, "assign")) ||
				!bgbcp_strcmp(b, "__vl_assign"))
			{
				n1=BGBCP_BlockStatement2(ctx, &s);
				n2=BCCX_NewCst1V(&bgbcc_rcst_body, "body", n1);
				n=BCCX_NewCst1(&bgbcc_rcst_vlassign, "vlassign", n2);
				*str=s;
				return(n);
			}

			if(
				((ctx->lang==BGBCC_LANG_VERILOG) &&
					!bgbcp_strcmp(b, "always")) ||
				!bgbcp_strcmp(b, "__vl_always"))
			{
				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//@
				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//(
				if(!bgbcp_strcmp(b, "("))
				{
					n2=BGBCP_Expression2(ctx, &s);
					s=BGBCP_Token2(s, b, &ty, ctx->lang);	//)
				}else
				{
					n2=NULL;
				}

				n1=BGBCP_BlockStatement2(ctx, &s);
				n1=BCCX_NewCst1V(&bgbcc_rcst_body, "body", n1);
				n=BCCX_NewCst1(&bgbcc_rcst_vlalways, "vlalways", n1);
				if(n2)
					BCCX_Add(n, BCCX_NewCst1V(&bgbcc_rcst_cond, "cond", n2));
				*str=s;
				return(n);
			}
		}

		if(ctx->lang==BGBCC_LANG_CS)
		{
			if(!bgbcp_strcmp(b, "get") && !bgbcp_strcmp1(b2, "{"))
			{
				n1=BGBCP_BlockStatement2(ctx, &s);
				n2=BCCX_NewCst1V(&bgbcc_rcst_body, "body", n1);
				n=BCCX_NewCst1(&bgbcc_rcst_get, "get", n2);
				*str=s;
				return(n);
			}

			if(!bgbcp_strcmp(b, "set") && !bgbcp_strcmp1(b2, "{"))
			{
				n1=BGBCP_BlockStatement2(ctx, &s);
				n2=BCCX_NewCst1V(&bgbcc_rcst_body, "body", n1);
				n=BCCX_NewCst1(&bgbcc_rcst_set, "set", n2);
				*str=s;
				return(n);
			}
		}

		if(	(ctx->lang==BGBCC_LANG_CS) ||
			(ctx->lang==BGBCC_LANG_BS2) )
		{
			if(!bgbcp_strcmp(b, "try"))
			{
				n1=BGBCP_BlockStatement2(ctx, &s);
				n2=BCCX_NewCst1V(&bgbcc_rcst_body, "body", n1);
				n=BCCX_NewCst1(&bgbcc_rcst_try_catch, "try_catch", n2);

				s1=BGBCP_Token2(s, b2, &ty2, ctx->lang);
				if(!bgbcp_strcmp(b2, "catch"))
				{
					s=s1;

					n1=NULL;

					BGBCP_Token(s, b, &ty);
					if(!bgbcp_strcmp1(b, "("))
					{
						s=BGBCP_Token(s, b, &ty); //'('
						n1=BGBCP_FunArgs(ctx, &s);
						n1=BCCX_NewCst1(&bgbcc_rcst_args, "args", n1);
					}

					n2=BGBCP_BlockStatement2(ctx, &s);
					n2=BCCX_NewCst1V(&bgbcc_rcst_body, "body", n2);

					n1=BCCX_NewCst2(&bgbcc_rcst_catch, "catch", n1, n2);
					BCCX_Add(n, n1);

					s1=BGBCP_Token2(s, b2, &ty2, ctx->lang);
				}

				if(!bgbcp_strcmp(b2, "finally"))
				{
					s=s1;

					n1=BGBCP_BlockStatement2(ctx, &s);
					n2=BCCX_NewCst1V(&bgbcc_rcst_finally, "finally", n1);
					BCCX_Add(n, n2);
				}

				*str=s;
				return(n);
			}
		}

		is_c = (ctx->lang==BGBCC_LANG_C);
		is_cpp_cx =
			((ctx->lang==BGBCC_LANG_CPP) ||
			(ctx->lang==BGBCC_LANG_CS) ||
			(ctx->lang==BGBCC_LANG_CX));

//		if((ctx->lang==BGBCC_LANG_CPP) ||
//			(ctx->lang==BGBCC_LANG_CS) ||
//			(ctx->lang==BGBCC_LANG_CX))
		if(1)
		{
			if((is_cpp_cx && !bgbcp_strcmp(b, "using")) ||
				(is_c && !bgbcp_strcmp(b, "__using")))
			{
				s1=BGBCP_Token2(s, b2, &ty2, ctx->lang);
				if(!bgbcp_strcmp(b2, "namespace"))
					s=s1;

				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//'name'
				BGBCP_Token2(s, b2, &ty2, ctx->lang);

				while(!bgbcp_strcmp1(b2, ".") ||
					!bgbcp_strcmp2(b2, "::"))
				{
					s=BGBCP_Token2(s, b2, &ty2, ctx->lang);	//.
					s=BGBCP_Token2(s, b2, &ty2, ctx->lang);
					strcat(b, "/");
					strcat(b, b2);

					BGBCP_Token2(s, b2, &ty2, ctx->lang);
				}

				s1=bgbcc_strdup(b);

//				for(i=0; ctx->cur_nsi[i]; i++);
//				ctx->cur_nsi[i++]=s1;
//				ctx->cur_nsi[i]=NULL;

				ctx->cur_nsi[ctx->n_cur_nsi++]=s1;

				n=BCCX_NewCst(&bgbcc_rcst_using, "using");
				BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);

//				s=BGBCP_EatSemicolon(s);
				s=BGBCP_EatSemicolonRequired(ctx, s);

				*str=s;
				return(n);
			}

			if((is_cpp_cx && !bgbcp_strcmp(b, "namespace")) ||
				(is_c && !bgbcp_strcmp(b, "__namespace")))
			{
				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//'name'
				BGBCP_Token2(s, b2, &ty2, ctx->lang);

				while(!bgbcp_strcmp1(b2, ".") ||
					!bgbcp_strcmp2(b2, "::"))
				{
					s=BGBCP_Token2(s, b2, &ty2, ctx->lang);	//.
					s=BGBCP_Token2(s, b2, &ty2, ctx->lang);
					strcat(b, "/");
					strcat(b, b2);

					BGBCP_Token2(s, b2, &ty2, ctx->lang);
				}

				s0=ctx->cur_ns;
				s1=bgbcc_strdup(b);
				ctx->cur_ns=s1;

//				a=ctx->cur_nsi;
//				ctx->cur_nsi=ab;
//				ctx->cur_nsi[0]=NULL;
				ocnsi=ctx->n_cur_nsi;

				s=BGBCP_Token2(s, b, &ty, ctx->lang); //{
				n1=BGBCP_Block(ctx, &s);
				n=BCCX_NewCst1(&bgbcc_rcst_namespace, "namespace", n1);
				BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);

				ctx->cur_ns=s0;
//				ctx->cur_nsi=a;
				ctx->n_cur_nsi=ocnsi;

				*str=s;
				return(n);
			}
		}

		if(	(ctx->lang==BGBCC_LANG_JAVA) ||
			(ctx->lang==BGBCC_LANG_CX))
		{
			if(!bgbcp_strcmp6(b, "import"))
			{
				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//'name'
				BGBCP_Token2(s, b2, &ty2, ctx->lang);

				while(!bgbcp_strcmp1(b2, "."))
				{
					s=BGBCP_Token2(s, b2, &ty2, ctx->lang);	//.
					s=BGBCP_Token2(s, b2, &ty2, ctx->lang);
					strcat(b, "/");
					strcat(b, b2);

					BGBCP_Token2(s, b2, &ty2, ctx->lang);
				}

				s1=bgbcc_strdup(b);

//				for(i=0; ctx->cur_nsi[i]; i++);
//				ctx->cur_nsi[i++]=s1;
//				ctx->cur_nsi[i]=NULL;
				ctx->cur_nsi[ctx->n_cur_nsi++]=s1;

				n=BCCX_NewCst(&bgbcc_rcst_import, "import");
				BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);

//				s=BGBCP_EatSemicolon(s);
				s=BGBCP_EatSemicolonRequired(ctx, s);

				*str=s;
				return(n);
			}
		}
		
		if(ctx->lang==BGBCC_LANG_BS2)
		{
			if(!bgbcp_strcmp(b, "import") ||
				!bgbcp_strcmp(b, "using"))
			{
				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//import
				BGBCP_Token2(s, b2, &ty2, ctx->lang);

				while(!bgbcp_strcmp1(b2, "."))
				{
					s=BGBCP_Token2(s, b2, &ty2, ctx->lang);	//.
					s=BGBCP_Token2(s, b2, &ty2, ctx->lang);
					strcat(b, "/");
					strcat(b, b2);

					BGBCP_Token2(s, b2, &ty2, ctx->lang);
				}

				s1=bgbcc_strdup(b);

//				for(i=0; ctx->cur_nsi[i]; i++);
//				ctx->cur_nsi[i++]=s1;
//				ctx->cur_nsi[i]=NULL;
				ctx->cur_nsi[ctx->n_cur_nsi++]=s1;

				n=BCCX_NewCst(&bgbcc_rcst_using, "using");
				BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);

//				s=BGBCP_EatSemicolon(s);
				s=BGBCP_EatSemicolonRequired(ctx, s);

				*str=s;
				return(n);
			}

			if(!bgbcp_strcmp(b, "package") ||
				!bgbcp_strcmp(b, "namespace"))
			{
				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//'name'
				BGBCP_Token2(s, b2, &ty2, ctx->lang);

				while(!bgbcp_strcmp1(b2, "."))
				{
					s=BGBCP_Token2(s, b2, &ty2, ctx->lang);	//.
					s=BGBCP_Token2(s, b2, &ty2, ctx->lang);
					strcat(b, "/");
					strcat(b, b2);

					BGBCP_Token2(s, b2, &ty2, ctx->lang);
				}

				s0=ctx->cur_ns;
				s1=bgbcc_strdup(b);
				ctx->cur_ns=s1;

//				a=ctx->cur_nsi;
//				ctx->cur_nsi=ab;
//				ctx->cur_nsi[0]=NULL;
				ocnsi=ctx->n_cur_nsi;

				s=BGBCP_Token2(s, b, &ty, ctx->lang); //{
				n1=BGBCP_Block(ctx, &s);
				n=BCCX_NewCst1(&bgbcc_rcst_namespace, "namespace", n1);
				BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);

				ctx->cur_ns=s0;
//				ctx->cur_nsi=a;
				ctx->n_cur_nsi=ocnsi;

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
	
	if(ctx->lang==BGBCC_LANG_BS2)
	{
		s=BGBCP_EatWhite(s);
		if(*s=='}')
		{
			n=BCCX_NewCst1(&bgbcc_rcst_return, "return", n);
			*str=s;
			return(n);
		}
	}
	
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
	char *s, *s1, *s0, *fn, *t;
	char **a;
	int i;

	s=*str;

#if 1
	BGBCP_Token2(s, b, &ty, ctx->lang);
	while(!bgbcp_strcmp1(b, "#"))
	{
		s=BGBCP_Token2(s, b, &ty, ctx->lang);
		while(*s && (*s!='\n'))s++;

		b[0]=0;
		BGBCP_Token2(s, b, &ty, ctx->lang);
	}
#endif

#if 0
	if(b[0]=='_')
	{
		if(	!strcmp(b, "_Pragma") ||
			!strcmp(b, "__pragma"))
		{
			n=BGBCP_DeclAttributeC(ctx, &s);

			if(n)
				BCCX_CheckDeleteUnlinked(n);

			*str=s;
			return(n);
		}
	}
#endif

//	s=BGBCP_EatWhite(s);
	BGBCP_EatWhite(s);
	fn=BGBCP_GetLastFileName();
//	ln=BGBCP_GetLastLineNumber();
	ln=BGBCP_GetPredictLineNumber(s);

//	printf("@ %s:%d\n", fn, ln);

#if 1
	s1=BGBCP_EatWhite(s);

	s0=s;
	n=BGBCP_Definition(ctx, &s);

	if(!n && (s!=s0) && (s!=s1))
	{
		*str=s;
		return(n);
	}

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

	s1=ctx->cur_ns; 
	ctx->cur_ns=NULL;
//	a=ctx->cur_nsi;
//	ctx->cur_nsi=NULL;

	n=BGBCP_BlockStatementInner(ctx, &s);

	ctx->cur_ns=s1;
//	ctx->cur_nsi=a;

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
	int ty, ty2, i, isveril;
	BCCX_Node *n, *ln, *rn, *n1, *lst, *lste;

	isveril=(ctx->lang==BGBCC_LANG_VERILOG);

	s=*str; s1=s;
	lst=NULL; lste=NULL;
	while(1)
	{
		if(!s)
			break;
		BGBCP_Token2(s, b, &ty, ctx->lang);
		if(!*s || (!isveril && (*b=='}')))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);
			break;
		}
		
		if(isveril)
		{
			if(	(!bgbcc_strcmp(b, "end") && !(flag&(16|32))) ||
				(!bgbcc_strcmp(b, "endcase") && (flag&16)) ||
				(!bgbcc_strcmp(b, "endmodule") && (flag&32)))
			{
				s=BGBCP_Token2(s, b, &ty, ctx->lang);
				break;
			}
		}

		s1=BGBCP_EatWhite(s);
		if(!*s1) { s=s1; break; }

//#if 0
#ifdef CATCH_BLOCK_SEH
		__try {
#endif

		if(flag&16)
		{
			s1=s;
			n=BGBCP_ExpressionComma(ctx, &s1);

			BGBCP_Token2(s1, b, &ty, ctx->lang);
			if(*b==':')
			{
				s=BGBCP_Token2(s1, b, &ty, ctx->lang);

				while(BCCX_TagIsCstP(n, &bgbcc_rcst_comma, "comma"))
				{
					ln=BCCX_FetchCst(n, &bgbcc_rcst_left, "left");
					rn=BCCX_FetchCst(n, &bgbcc_rcst_right, "right");
					
					n1=BCCX_NewCst1(&bgbcc_rcst_case, "case",
						BCCX_NewCst1V(&bgbcc_rcst_value, "value", rn));
					lst=BCCX_AddEnd2(lst, &lste, n1);
					
					n=ln;
				}

				n1=BCCX_NewCst1(&bgbcc_rcst_case, "case",
					BCCX_NewCst1V(&bgbcc_rcst_value, "value", n));
				lst=BCCX_AddEnd2(lst, &lste, n1);
			}
		}

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

BCCX_Node *BGBCP_VlCaseBlock(BGBCP_ParseState *ctx, char **str)
{
	BCCX_Node *n;
	
	n=BGBCP_BlockI(ctx, str, 16);
	return(n);
}

BCCX_Node *BGBCP_VlModuleBlock(BGBCP_ParseState *ctx, char **str)
{
	BCCX_Node *n;
	
	n=BGBCP_BlockI(ctx, str, 32);
	return(n);
}

BCCX_Node *BGBCP_Toplevel(BGBCP_ParseState *ctx, char **str)
{
#if 0
	BCCX_Node *n;
	
	n=BGBCP_BlockI(ctx, str, 1);
	return(n);
#endif

#if 1
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
		BGBCP_Token2(s, b, &ty, ctx->lang);
		if(!*s || (*b=='}'))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);
			break;
		}

		s1=BGBCP_EatWhite(s);
		if(!*s1) { s=s1; break; }

		n=BGBCP_BlockStatementI(ctx, &s, 1);
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
		
		BGBCP_AddDefToplevel(ctx, n);
		
//		lst=BCCX_AddEnd(lst, n);
//		lst=BCCX_AddEnd2(lst, &lste, n);
	}

	*str=s;
//	return(lst);
	return(NULL);
#endif
}

BCCX_Node *BGBCP_BlockStatement2(BGBCP_ParseState *ctx, char **str)
{
	char b[256];
	char *s;
	s64 dfl_fl;
	int tk0, tk1, tk2, bn;
	int ty, isveril;
	BCCX_Node *n;

//	if(	(ctx->lang==BGBCC_LANG_JAVA) ||
//		(ctx->lang==BGBCC_LANG_CS) ||
//		(ctx->lang==BGBCC_LANG_BS) ||
//		(ctx->lang==BGBCC_LANG_BS2))
//			return(BGBJP_BlockStatement2(ctx, str));

	s=*str;

	isveril=(ctx->lang==BGBCC_LANG_VERILOG);

	BGBCP_Token2(s, b, &ty, ctx->lang);
	if(		(!isveril && !bgbcp_strcmp1(b, "{")) ||
			(isveril && !bgbcp_strcmp5(b, "begin"))
		)
	{
		s=BGBCP_Token2(s, b, &ty, ctx->lang);

		dfl_fl=ctx->dfl_flags;

		tk0=BGBCP_GetTokenCount();
		n=BGBCP_Block(ctx, &s);
		tk1=BGBCP_GetTokenCount();

		ctx->dfl_flags=dfl_fl;

		n=BCCX_NewCst1(&bgbcc_rcst_begin, "begin", n);

		tk2=tk1-tk0;
		if(tk2>0)
			BCCX_SetIntCst(n, &bgbcc_rcst_tokens, "tokens", tk2);

		bn=ctx->blkidx++;
		BCCX_SetIntCst(n, &bgbcc_rcst_index, "index", bn);
		
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
