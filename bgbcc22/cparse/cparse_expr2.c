#include <bgbccc.h>

//Lit:		Literal values/builtin expressions
//PE:		(<expr>) <expr>[<expr>]
//Postfix:	<name>++ <name>-- <expr>(<args>)  <expr>.<name>
//Unary:	++<name> --<name> + - ! ~ *
//Cast:		(<Type>)<Unary>
//E:		<expr>**<expr>
//MD:		* / % \ &	*. /. %. \. &.
//AS:		+ - f| ^		+. -. |. ^.
//SHLR:		<< >>
//RCmp:		< > <= >= == === != <=> <<== >>==
//Lop:		&&
//Lop2:		|| ^^
//TCond:	<expr>?<expr>:<expr>
//Attr:		:= :!= :< :> :<= :>= :<< :>> :<<= :>>=
//Equals:	= += -= *= /= \= %= &= |= ^= >>= <<=

BGBCP_ParseItem *bgbcp_exprs=NULL;

int BGBCP_AddExpression(char *name,
	BCCX_Node *(*func)(BGBCP_ParseState *ctx, char **s))
{
	BGBCP_ParseItem *tmp;

	tmp=malloc(sizeof(BGBCP_ParseItem));
	tmp->name=strdup(name);
	tmp->func=func;

	tmp->next=bgbcp_exprs;
	bgbcp_exprs=tmp;

	return(0);
}

BCCX_Node *BGBCP_ParseExpressionName(
	BGBCP_ParseState *ctx, char *name, char **s)
{
	BGBCP_ParseItem *cur;

	cur=bgbcp_exprs;
	while(cur)
	{
		if(!bgbcp_strcmp(name, cur->name))
			return(cur->func(ctx, s));
		cur=cur->next;
	}
	return(NULL);
}

BCCX_Node *BGBCP_Number(BGBCP_ParseState *ctx, char *str)
{
	return(BGBCP_NumberSuf(ctx, str, NULL));
}

BCCX_Node *BGBCP_NumberSuf(BGBCP_ParseState *ctx, char *str, char *suf)
{
	char tba[512];
	BCCX_Node *n;
	char *s, *suf1, *s1;
	s64 li, lj;
	int i, big, bigts, tsi_sz;

	s=str;
//	while(*s && (*s!='.') && (*s!='e') && (*s!='E')) s++;
	while(*s && (*s!='.')) s++;

	if(!*s)
	{
//		li=bgbcc_atoi(str);

		i=bgbcc_atoxl(str, (u64 *)(&li), (u64 *)(&lj));
		
		big=0;
		bigts=0;
		tsi_sz=0;
		if(!suf && (i<0))
		{
//			big=1;

			s=str;
			if((s[0]=='0') &&
				((s[1]=='x') || (s[1]=='X') ||
				 (s[1]=='b') || (s[1]=='B')))
					s+=2;
			while(*s)
			{
				if((*s=='x') || (*s=='z') || (*s=='X') || (*s=='Z'))
					break;
				s++;
			}
		
			if(*s)
				bigts=1;
			else
				big=1;
		}

		if(suf &&
			((suf[0]=='I') || (suf[0]=='U')) &&
			(suf[1]>='0') && (suf[1]<='9'))
		{
			tsi_sz=atoi(suf+1);
		}
		
		if((i<=0) && suf &&
			((suf[0]=='I') || (suf[0]=='U')) &&
			(suf[1]>='0') && (suf[1]<='9'))
		{
			s=str;
			if((s[0]=='0') &&
				((s[1]=='x') || (s[1]=='X') ||
				 (s[1]=='b') || (s[1]=='B')))
					s+=2;
			while(*s)
			{
				if((*s=='x') || (*s=='z') || (*s=='X') || (*s=='Z'))
					break;
				s++;
			}
		
			if(*s)
				bigts=1;
			else
				big=1;
		}
		
		suf1=suf;
#if 1
		if(!suf && (*str=='0'))
		{
			if(li!=((s32)li))
			{
				if(li==((u32)li))
				{
					suf1="U";
				}else
				{
					if(lj!=(li>>63))
						{ suf1="ULL"; }
					else
						{ suf1="LL"; }
				}
			}
			
			if((lj!=0) && (lj!=(-1)))
			{
				if(lj>>63)
					{ suf1="UXL"; }
				else
					{ suf1="XL"; }
			}
		}else if(!suf)
		{
			if(li!=((s32)li))
			{
				suf1="LL";
			}

			if(((lj!=0) && (lj!=(-1))) || (lj!=(li>>63)))
			{
				suf1="XL";
			}
		}
#endif
		
		if(bigts)
		{
			bgbcc_strtoxts(str, tba);
			
			if(!tsi_sz)
			{
				tsi_sz=(strlen(tba)/22)*64;
			}

			if(strlen(tba)==22)
			{
				/* special-case tristate values <= 64 bits. */
				s=tba;
				li=	(((u64)(s[ 0]-'0'))<< 0) |
					(((u64)(s[ 1]-'0'))<< 6) |
					(((u64)(s[ 2]-'0'))<<12) |
					(((u64)(s[ 3]-'0'))<<18) |
					(((u64)(s[ 4]-'0'))<<24) |
					(((u64)(s[ 5]-'0'))<<30) |
					(((u64)(s[ 6]-'0'))<<36) |
					(((u64)(s[ 7]-'0'))<<42) |
					(((u64)(s[ 8]-'0'))<<58) |
					(((u64)(s[ 9]-'0'))<<54) |
					(((u64)(s[10]-'0'))<<60) ;
				s+=11;
				lj=	(((u64)(s[ 0]-'0'))<< 0) |
					(((u64)(s[ 1]-'0'))<< 6) |
					(((u64)(s[ 2]-'0'))<<12) |
					(((u64)(s[ 3]-'0'))<<18) |
					(((u64)(s[ 4]-'0'))<<24) |
					(((u64)(s[ 5]-'0'))<<30) |
					(((u64)(s[ 6]-'0'))<<36) |
					(((u64)(s[ 7]-'0'))<<42) |
					(((u64)(s[ 8]-'0'))<<58) |
					(((u64)(s[ 9]-'0'))<<54) |
					(((u64)(s[10]-'0'))<<60) ;

				n=BCCX_NewCst(&bgbcc_rcst_int_ts, "int_ts");
				BCCX_SetIntCst(n, &bgbcc_rcst_value_lo, "value_lo", li);
				BCCX_SetIntCst(n, &bgbcc_rcst_value_hi, "value_hi", lj);
				BCCX_SetIntCst(n, &bgbcc_rcst_value_sz, "value_sz", tsi_sz);
				return(n);
			}

			n=BCCX_NewCst(&bgbcc_rcst_bigtsi, "bigtsi");
			BCCX_SetCst(n, &bgbcc_rcst_value, "value", tba);
			if(tsi_sz)
				BCCX_SetIntCst(n, &bgbcc_rcst_value_sz, "value_sz", tsi_sz);
//			BCCX_SetIntCst(n, &bgbcc_rcst_value, "value", li);			
			if(suf1)
				BCCX_SetCst(n, &bgbcc_rcst_tysuf, "tysuf", suf1);
			return(n);
		}
		
		if(big)
		{
			bgbcc_strtoxs(str, tba);
		
			n=BCCX_NewCst(&bgbcc_rcst_bigint, "bigint");
			BCCX_SetCst(n, &bgbcc_rcst_value, "value", tba);
			if(tsi_sz)
				BCCX_SetIntCst(n, &bgbcc_rcst_value_sz, "value_sz", tsi_sz);
//			BCCX_SetIntCst(n, &bgbcc_rcst_value, "value", li);			
			if(suf1)
				BCCX_SetCst(n, &bgbcc_rcst_tysuf, "tysuf", suf1);
			return(n);
		}
		
		if(((lj!=0) && (lj!=(-1))) ||
			(suf && (lj!=(li>>63)) &&
				(!strcmp(suf, "XL") || !strcmp(suf, "UXL")))
			)
		{
			n=BCCX_NewCst(&bgbcc_rcst_int128, "int128");
			BCCX_SetIntCst(n, &bgbcc_rcst_value_lo, "value_lo", li);
			BCCX_SetIntCst(n, &bgbcc_rcst_value_hi, "value_hi", lj);
			if(tsi_sz)
				BCCX_SetIntCst(n, &bgbcc_rcst_value_sz, "value_sz", tsi_sz);
			return(n);
		}

		n=BCCX_NewCst(&bgbcc_rcst_int, "int");
//		BCCX_SetCst(n, &bgbcc_rcst_value, "value", str);
		BCCX_SetIntCst(n, &bgbcc_rcst_value, "value", li);
		if(tsi_sz)
			BCCX_SetIntCst(n, &bgbcc_rcst_value_sz, "value_sz", tsi_sz);
		
		if(suf1)
			BCCX_SetCst(n, &bgbcc_rcst_tysuf, "tysuf", suf1);

		return(n);
	}

	n=BCCX_NewCst(&bgbcc_rcst_real, "real");
//	BCCX_SetCst(n, &bgbcc_rcst_value, "value", str);
	BCCX_SetFloatCst(n, &bgbcc_rcst_value, "value", BGBCC_ParseNumber(str));
	return(n);
}

BCCX_Node *BGBCP_NumberXF(BGBCP_ParseState *ctx, char *str)
{
	BCCX_Node *n;
	char *s;
	s64 li, lj;
	int i;

	bgbcc_atoxf(str, (u64 *)(&li), (u64 *)(&lj));

	n=BCCX_NewCst(&bgbcc_rcst_float128, "float128");
//	BCCX_SetFloatCst(n, &bgbcc_rcst_value, "value", BGBCC_ParseNumber(str));
	BCCX_SetIntCst(n, &bgbcc_rcst_value_lo, "value_lo", li);
	BCCX_SetIntCst(n, &bgbcc_rcst_value_hi, "value_hi", lj);
	return(n);
}

BCCX_Node *BGBCP_ExpressionLitString(BGBCP_ParseState *ctx, char **str)
{
//	static char stbufa[16384];
	static char *stbuf=NULL;
	static char *stbufe;
	char b[256], b2[256], b3[256];
	char *s, *t, *s1, *t1;
	char *suf;
	int ty, ty2, ty3;
	BCCX_Node *n, *n1, *n2;
	int i, j, k, l;

	if(!stbuf)
	{
//		stbuf=stbufa;

		l=16384;
		stbuf=malloc(l);
		stbufe=stbuf+l;
	}

	s=*str;

	t1=stbuf;

#if 0
	s1=BGBCP_Token(s, b, &ty);
	if(ty==BTK_STRING)
	{
		s=s1;
		strcpy(t1, b);
		t1+=strlen(t1);
	}
#endif

	s1=BGBCP_Token(s, b2, &ty2);
//	while(ty2==BTK_STRING)
	while(1)
	{
		if(ty2==BTK_STRING)
		{
			s=s1;
//			s=BGBCP_Token(s, b2, &ty2);
	//			strcat(b, b2);
	//		strcpy(t1, b2);
	//		t1+=strlen(t1);

			l=strlen(b2);
			
			if((t1+l)>stbufe)
			{
				j=t1-stbuf;
				k=stbufe-stbuf;
				
				k=k+(k>>1);
				stbuf=realloc(stbuf, k);
				stbufe=stbuf+k;
				t1=stbuf+j;
			}
			
			memcpy(t1, b2, l+1);
			t1+=l;

			s=BGBCP_EatWhite(s);
			if((s[0]=='L') && (s[1]=='\"'))		s++;
			if((s[0]=='u') && (s[1]=='\"'))		s++;
			if((s[0]=='U') && (s[1]=='\"'))		s++;

			if(		(s[0]=='u')		&&
					(s[1]=='8')		&&
					(s[2]=='\"')	)
				s+=2;

			s1=BGBCP_Token(s, b2, &ty2);
			continue;
		}

		if(ty2==BTK_STRING_OVF)
		{
			s=s1;
			while(*s)
			{
				if((t1+16)>stbufe)
				{
					j=t1-stbuf;
					k=stbufe-stbuf;
					
					k=k+(k>>1);
					stbuf=realloc(stbuf, k);
					stbufe=stbuf+k;
					t1=stbuf+j;
				}
				
				if(*s=='\\')
				{
					switch(s[1])
					{
					case 'a':	s+=2;	*t1++='\a';		break;
					case 'b':	s+=2;	*t1++='\b';		break;
					case 'e':	s+=2;	*t1++='\x1B';	break;
					case 'f':	s+=2;	*t1++='\f';		break;
					case 'r':	s+=2;	*t1++='\r';		break;
					case 'n':	s+=2;	*t1++='\n';		break;
					case 'v':	s+=2;	*t1++='\v';		break;
					case 't':	s+=2;	*t1++='\t';		break;
					case '\\':	s+=2;	*t1++='\\';		break;
					case '\"':	s+=2;	*t1++='\"';		break;
					case '\'':	s+=2;	*t1++='\'';		break;
					case '?':	s+=2;	*t1++='?';		break;
					
					case 'x':
					case 'u':
					case 'U':
						i=2;
						if(s[1]=='u')i=4;
						if(s[1]=='U')i=8;
					
						s+=2; j=0;
						while(*s && i--)
						{
							k=-1;
							if((*s>='0') && (*s<='9'))k=*s-'0';
							if((*s>='A') && (*s<='F'))k=*s-'A'+10;
							if((*s>='a') && (*s<='f'))k=*s-'a'+10;
							if(k<0)break;
							j=(j<<4)|k;
							s++;
						}
						BGBCP_EmitChar(&t1, j);
						break;
						
					}
					continue;
				}
				
				if(*s=='\"')
				{
					s++;
					break;
				}
				
				*t1++=*s++;
			}
			*t1=0;
			
			s=BGBCP_EatWhite(s);
			if((s[0]=='L') && (s[1]=='\"'))		s++;
			if((s[0]=='u') && (s[1]=='\"'))		s++;
			if((s[0]=='U') && (s[1]=='\"'))		s++;

			if(		(s[0]=='u')		&&
					(s[1]=='8')		&&
					(s[2]=='\"')	)
				s+=2;

			s1=BGBCP_Token(s, b2, &ty2);
			continue;
		}
		break;
	}

	n=BCCX_NewCst(&bgbcc_rcst_string, "string");
//		BCCX_SetCst(n, &bgbcc_rcst_value, "value", b);
	BCCX_SetCst(n, &bgbcc_rcst_value, "value", stbuf);

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionLitStringQQQ(BGBCP_ParseState *ctx, char **str)
{
//	static char stbufa[16384];
	static char *stbuf=NULL;
	static char *stbufe;
	char b[256], b2[256], b3[256];
	char *s, *t, *s1, *t1;
	char *suf;
	int ty, ty2, ty3;
	BCCX_Node *n, *n1, *n2;
	int i, j, k, l;

	if(!stbuf)
	{
//		stbuf=stbufa;

		l=16384;
		stbuf=malloc(l);
		stbufe=stbuf+l;
	}

	s=*str;
	t1=stbuf;

	while(*s)
	{
		if((t1+16)>stbufe)
		{
			j=t1-stbuf;
			k=stbufe-stbuf;
			
			k=k+(k>>1);
			stbuf=realloc(stbuf, k);
			stbufe=stbuf+k;
			t1=stbuf+j;
		}

		if((s[0]=='\\') && (s[1]=='\"') && (s[2]=='\"') && (s[3]=='\"'))
		{
			s++;
			*t1++=*s++;
			*t1++=*s++;
			*t1++=*s++;
			continue;
		}
		
		if((s[0]=='\"') && (s[1]=='\"') && (s[2]=='\"'))
		{
			*t1=0;
			s+=3;
			break;
		}
	
		*t1++=*s++;
	}
	
	*t1=0;

	n=BCCX_NewCst(&bgbcc_rcst_string, "string");
	BCCX_SetCst(n, &bgbcc_rcst_value, "value", stbuf);

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionLit(BGBCP_ParseState *ctx, char **str)
{
//	static char stbuf[16384];
	char b[256], b2[256], b3[256];
	char *s, *t, *s1, *s2, *s3, *t1;
	char *suf;
	int ty, ty2, ty3;
	BCCX_Node *n, *n1, *n2, *n3, *n4;
	s64 li;
	int i, j, k, is_n, isveril;

	isveril=(ctx->lang==BGBCC_LANG_VERILOG);

	s=*str;
	n=NULL;

	s1=BGBCP_Token(s, b, &ty);
	if(!bgbcp_strcmp1(b, "(") && (ty==BTK_BRACE))
	{
		s=BGBCP_Token(s, b, &ty);	//(
		n=BGBCP_Expression2(ctx, &s);
		s=BGBCP_Token(s, b, &ty);	//)

		if(bgbcp_strcmp1(b, ")"))
		{
			BGBCP_ErrorCtx(ctx, s, "parser, token '%s'\n", b);
//			printf("parser, token '%s'\n", b);
		}

//		n=BGBCP_FunArgs(ctx, &s);
//		if(CDR(n))n=CONS(SYM("begin"), n);

		*str=s;
		return(n);
	}

	if(ty==BTK_STRING)
	{
		n=BGBCP_ExpressionLitString(ctx, &s);
		*str=s;
		return(n);
	}

	if(ty==BTK_STRING_QQQ)
	{
		s=s1;
		n=BGBCP_ExpressionLitStringQQQ(ctx, &s);
		*str=s;
		return(n);
	}

	s=*str;

	s=BGBCP_Token(s, b, &ty);
	s1=BGBCP_Token(s, b2, &ty2);
	if(!s)return(NULL);


	if(ctx->lang!=BGBCC_LANG_BS2)
	{
		if(!bgbcp_strcmp1(b, "{") && (ty==BTK_BRACE))
		{
			n1=BGBCP_FunArgs(ctx, &s);
			n=BCCX_NewCst1(&bgbcc_rcst_list, "list", n1);

			*str=s;
			return(n);
		}
	}

#if 0
	if(!bgbcp_strcmp1(b, "{") && (ty==BTK_BRACE))
	{
		n1=BGBCP_FunArgs(ctx, &s);
		n=BCCX_NewCst1(&bgbcc_rcst_list, "list", n1);

#if 0
		if(ctx->lang==BGBCC_LANG_BS2)
		{
			s1=BGBCP_Token(s, b, &ty);
			if(!bgbcp_strcmp1(b, ":"))
			{
				n=BCCX_NewCst1(&bgbcc_rcst_object, "object", n);

				s=s1;
				n1=BGBCP_DefExpectType(ctx, &s);
				if(n1)
					BCCX_Add(n, n1);
			}
		}
#endif

		*str=s;
		return(n);
	}
#endif

	if(ctx->lang==BGBCC_LANG_BS2)
	{
		if(!bgbcp_strcmp1(b, "{") && (ty==BTK_BRACE))
		{
			n1=BGBCP_FunArgs(ctx, &s);
			n1=BCCX_NewCst1(&bgbcc_rcst_list, "list", n1);
			n=BCCX_NewCst1(&bgbcc_rcst_object, "object", n1);

			s1=BGBCP_Token(s, b, &ty);
			if(!bgbcp_strcmp1(b, ":"))
			{
				s=s1;
				n1=BGBCP_DefExpectType(ctx, &s);
				if(n1)
					BCCX_Add(n, n1);
			}else if(ty==BTK_NAME)
			{
				s=s1;
				BCCX_SetCst(n, &bgbcc_rcst_tysuf, "tysuf", b);
			}

			*str=s;
			return(n);
		}

		if(!bgbcp_strcmp1(b, "[") && (ty==BTK_BRACE))
		{
			n1=BGBCP_FunArgs(ctx, &s);
			n1=BCCX_NewCst1(&bgbcc_rcst_list, "list", n1);
			n=BCCX_NewCst1(&bgbcc_rcst_array, "array", n1);

			s1=BGBCP_Token(s, b, &ty);
			if(!bgbcp_strcmp1(b, ":"))
			{
				s=s1;
				n1=BGBCP_DefExpectType(ctx, &s);
				if(n1)
					BCCX_Add(n, n1);
			}else if(ty==BTK_NAME)
			{
				s=s1;
				BCCX_SetCst(n, &bgbcc_rcst_tysuf, "tysuf", b);
			}

			*str=s;
			return(n);
		}

		if(!bgbcp_strcmp1(b, "#") && (ty==BTK_OPERATOR))
		{
			if(!bgbcp_strcmp1(b2, "[") && (ty2==BTK_BRACE))
			{
				s=s1;
				n1=BGBCP_FunArgs(ctx, &s);
				n1=BCCX_NewCst1(&bgbcc_rcst_list, "list", n1);
				n=BCCX_NewCst1(&bgbcc_rcst_vector, "vector", n1);

				s1=BGBCP_Token(s, b, &ty);
				if(!bgbcp_strcmp1(b, ":"))
				{
					s=s1;
					n1=BGBCP_DefExpectType(ctx, &s);
					if(n1)
						BCCX_Add(n, n1);
				}else if(ty==BTK_NAME)
				{
					s=s1;
					BCCX_SetCst(n, &bgbcc_rcst_tysuf, "tysuf", b);
				}

				*str=s;
				return(n);
			}

			if(!bgbcp_strcmp1(b2, ":") && (ty2==BTK_SEPERATOR))
			{
				s=s1;
				s=BGBCP_Token(s, b, &ty);
				n=BCCX_NewCst(&bgbcc_rcst_keyword, "keyword");
				BCCX_SetCst(n, &bgbcc_rcst_name, "name", b);

				*str=s;
				return(n);
			}

			if((ty2==BTK_NAME) || (ty2==BTK_STRING))
			{
				s=s1;
				s=BGBCP_Token(s, b, &ty);
				n=BCCX_NewCst(&bgbcc_rcst_symbol, "symbol");
				BCCX_SetCst(n, &bgbcc_rcst_name, "name", b);

				*str=s;
				return(n);
			}

			if(ty2==BTK_CHARSTRING)
			{
				s=s1;
				s=BGBCP_Token(s, b, &ty);
				n=BCCX_NewCst(&bgbcc_rcst_ref, "ref");
				BCCX_SetCst(n, &bgbcc_rcst_name, "name", b);

				*str=s;
				return(n);
			}
		}
	}

	if(ty==BTK_NAME)
	{
		s1=BGBCP_Token(s, b2, &ty2);

		if((b[0]=='_') && !bgbcp_strcmp10(b, "__offsetof"))
		{
			s=BGBCP_Token(s, b2, &ty2);
			if(bgbcp_strcmp1(b2, "("))
				BGBCP_Error(s, 
					"offsetof token '%s'\n", b2);

//			n1=BGBCP_DefType(ctx, &s);
//			n2=BGBCP_DefName2(ctx, &s);
//			n1=BGBCP_ExpandDefinition(n1, n2);
//			n1=CDR(n1);

			n1=BGBCP_ArgDefinition(ctx, &s);

			s=BGBCP_Token(s, b2, &ty2);
			if(bgbcp_strcmp1(b2, ","))
				BGBCP_Error(s, 
					"offsetof token '%s'\n", b2);

//			s=BGBCP_Token(s, b, &ty);
//			n2=BCCX_NewCst(&bgbcc_rcst_symbol, "symbol"); BCCX_Set(n2, "name", b2);

			n2=BGBCP_Expression2(ctx, &s);

			s=BGBCP_Token(s, b2, &ty2);
			if(bgbcp_strcmp1(b2, ")"))
			{
				BGBCP_Error(s, 
					"offsetof token '%s'\n", b2);
			}

			if(BCCX_TagIsCstP(n2, &bgbcc_rcst_ref, "ref"))
			{
				s1=BCCX_GetCst(n2, &bgbcc_rcst_name, "name");

				n=BCCX_NewCst1(&bgbcc_rcst_offsetof, "offsetof",
					BCCX_NewCst1V(&bgbcc_rcst_tyexpr, "tyexpr", n1));
				BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);

				*str=s;
				return(n);
			}

			n=BCCX_NewCst2(&bgbcc_rcst_offsetof, "offsetof",
				BCCX_NewCst1V(&bgbcc_rcst_tyexpr, "tyexpr", n1),
				BCCX_NewCst1V(&bgbcc_rcst_value, "value", n2));

//			n=BCCX_NewCst1(&bgbcc_rcst_offsetof, "offsetof",
//				BCCX_NewCst1V(&bgbcc_rcst_tyexpr, "tyexpr", n1));
//			BCCX_SetCst(n, &bgbcc_rcst_name, "name", b);

			*str=s;
			return(n);
		}

		if((!bgbcp_strcmp3(b, "new") && (ctx->lang!=BGBCC_LANG_C)) ||
			!bgbcp_strcmp5(b, "__new") ||
			!bgbcp_strcmp6(b, "__jnew") ||
			!bgbcp_strcmp6(b, "__vnew"))
		{
			if(!bgbcp_strcmp3(b, "new"))
			{
				if(ctx->lang!=BGBCC_LANG_CPP)
				{
					n=BCCX_NewCst(&bgbcc_rcst_jnew, "jnew");
				}else
				{
					n=BCCX_NewCst(&bgbcc_rcst_new, "new");
				}
			}else
				if(!bgbcp_strcmp6(b, "__vnew"))
			{
				n=BCCX_NewCst(&bgbcc_rcst_new, "new");
			}else
				if(	!bgbcp_strcmp6(b, "__jnew") ||
					!bgbcp_strcmp5(b, "__new"))
			{
				n=BCCX_NewCst(&bgbcc_rcst_jnew, "jnew");
			}else
			{
				n=BCCX_NewCst(&bgbcc_rcst_new, "new");
			}

			if(ctx->lang==BGBCC_LANG_BS2)
			{
				BGBCP_Token(s, b, &ty);
				if(!bgbcp_strcmp1(b, "("))
				{
					s=BGBCP_Token(s, b, &ty); //'('
					n1=BGBCP_FunArgs(ctx, &s);
					BCCX_Add(n, BCCX_NewCst1V(&bgbcc_rcst_ref, "ref", n1));
				}
			}

//			i=ctx->expect_type;
//			ctx->expect_type=0;
//			if(	(ctx->lang==BGBCC_LANG_CS) ||
//				(ctx->lang==BGBCC_LANG_BS2))
//					ctx->expect_type=1;
//			n1=BGBCP_DefType(ctx, &s);
//			ctx->expect_type=i;

			n1=BGBCP_DefExpectType(ctx, &s);

			if(!n1)
			{
				BGBCP_Error(s, "invalid type for new\n");
				*str=NULL;
				return(NULL);
			}

//			BCCX_Add(n, n1);
			BCCX_Add(n, BCCX_NewCst1V(&bgbcc_rcst_type, "type", n1));

			BGBCP_Token(s, b, &ty);
			if(!bgbcp_strcmp1(b, "("))
			{
				s=BGBCP_Token(s, b, &ty); //'('
				n1=BGBCP_FunArgs(ctx, &s);
				BCCX_Add(n, BCCX_NewCst1(&bgbcc_rcst_args, "args", n1));
			}

			if(!bgbcp_strcmp1(b, "["))
			{
				s=BGBCP_Token(s, b, &ty); //'['
				n1=BGBCP_FunArgs(ctx, &s);
				BCCX_Add(n, BCCX_NewCst1(&bgbcc_rcst_size, "size", n1));
			}

			*str=s;
			return(n);
		}

//		if(!bgbcp_strcmp1(b, "L") && (ty2==BTK_STRING))
		if(	(	!bgbcp_strcmp1(b, "L") ||
				!bgbcp_strcmp1(b, "u") ||
				!bgbcp_strcmp1(b, "U") ||
				!bgbcp_strcmp2(b, "u8")) &&
			(ty2==BTK_STRING)	)
		{
			n=BGBCP_ExpressionLit(ctx, &s);
//			BCCX_SetCst(n, &bgbcc_rcst_tysuf, "tysuf", "L");
			BCCX_SetCst(n, &bgbcc_rcst_tysuf, "tysuf", b);
			*str=s;
			return(n);
		}

		if(!bgbcp_strcmp5 (b, "__var") ||
			((ctx->lang==BGBCC_LANG_BS2) &&
				!bgbcp_strcmp3(b, "var"))
			)
		{
			if(!bgbcp_strcmp1(b2, "{") && (ty2==BTK_BRACE))
			{
				s=s1;

//				n1=BGBCP_FunArgs(ctx, &s);
				n1=BGBCP_FunArgsFlag(ctx, &s, 1);
				n1=BCCX_NewCst1(&bgbcc_rcst_list, "list", n1);
				n=BCCX_NewCst1(&bgbcc_rcst_object, "object", n1);

				s1=BGBCP_Token(s, b, &ty);
				if(!bgbcp_strcmp1(b, ":"))
				{
					s=s1;
					n1=BGBCP_DefExpectType(ctx, &s);
					if(n1)
						BCCX_Add(n, n1);
				}else if(ty==BTK_NAME)
				{
					s=s1;
					BCCX_SetCst(n, &bgbcc_rcst_tysuf, "tysuf", b);
				}

				*str=s;
				return(n);
			}

			if(!bgbcp_strcmp1(b2, "[") && (ty2==BTK_BRACE))
			{
				s=s1;

				n1=BGBCP_FunArgs(ctx, &s);
				n1=BCCX_NewCst1(&bgbcc_rcst_list, "list", n1);
				n=BCCX_NewCst1(&bgbcc_rcst_array, "array", n1);

				s1=BGBCP_Token(s, b, &ty);
				if(!bgbcp_strcmp1(b, ":"))
				{
					s=s1;
					n1=BGBCP_DefExpectType(ctx, &s);
					if(n1)
						BCCX_Add(n, n1);
				}else if(ty==BTK_NAME)
				{
					s=s1;
					BCCX_SetCst(n, &bgbcc_rcst_tysuf, "tysuf", b);
				}

				*str=s;
				return(n);
			}
		}

		if(	((b[0]=='_') &&
				(	!bgbcp_strcmp8 (b, "__func__")		||
					!bgbcp_strcmp10(b, "__function")	||
					!bgbcp_strcmp5 (b, "__var")			)) ||
			((ctx->lang==BGBCC_LANG_BS2) &&
				(	!bgbcp_strcmp8(b, "function")	||
					!bgbcp_strcmp3(b, "var")		))
			)
		{
			n1=NULL;
			n2=NULL;
			n3=NULL;
			n4=NULL;
			is_n=0;

			if(!bgbcp_strcmp1(b2, "!"))
			{
				s=BGBCP_Token(s, b2, &ty2); //'!'
				is_n=1;
			}

			if(!bgbcp_strcmp1(b2, "["))
			{
				n4=BGBCP_FunArgs(ctx, &s);
				s=BGBCP_Token(s, b2, &ty2);
			}
			
			if(!bgbcp_strcmp1(b2, "("))
			{
				s=BGBCP_Token(s, b2, &ty2); //'('
				n1=BGBCP_FunVarsList(ctx, &s);
				BGBCP_Token(s, b2, &ty2);
			}

			if(	!bgbcp_strcmp1(b2, ":") ||
				!bgbcp_strcmp2(b2, "->"))
			{
				s=BGBCP_Token(s, b2, &ty2); //'('
				n2=BGBCP_DefExpectType(ctx, &s);
				BGBCP_Token(s, b2, &ty2);
			}else
			{
				n2=BCCX_NewCst(&bgbcc_rcst_type, "type");
				BCCX_SetCst(n2, &bgbcc_rcst_name, "name", "variant");
			}

			if(	!bgbcp_strcmp1(b2, "{"))
			{
//				s=BGBCP_Token(s, b2, &ty2); //'('

				i=ctx->in_func_body;
				ctx->in_func_body=1;
//				tk0=BGBCP_GetTokenCount();
				n3=BGBCP_BlockStatement2(ctx, &s);
//				tk1=BGBCP_GetTokenCount();
				ctx->in_func_body=i;

				BGBCP_Token(s, b2, &ty2);
			}
			
			n=BCCX_NewCst(&bgbcc_rcst_lambda, "lambda");
			if(n1)
			{
//				BCCX_Add(n, n1);
				n1=BCCX_NewCst1(&bgbcc_rcst_args, "args", n1);
				BCCX_AddV(n, n1);

			}
			if(n2)
			{
				BCCX_Add(n, n2);
			}
			if(n3)
			{
//				BCCX_Add(n, n3);
				n3=BCCX_NewCst1V(&bgbcc_rcst_body, "body", n3);
				BCCX_AddV(n, n3);
			}

			if(n4)
			{
				n4=BCCX_NewCst1(&bgbcc_rcst_list, "list", n4);
				BCCX_AddV(n, n4);
			}

			li=BGBCC_GenSymInt();
			li&=0x0FFFFFFFU;
			if(is_n)
				li|=0x10000000U;
			BCCX_SetIntCst(n, &bgbcc_rcst_index, "index", li);

			*str=s;
			return(n);
		}

		n=BGBCP_ParseExpressionName(ctx, b, &s);
		if(n)
		{
			*str=s;
			return(n);
		}

//		if(ctx->lang==BGBCC_LANG_CPP)
		if(1)
		{
			while(!bgbcp_strcmp2(b2, "::"))
			{
				s=BGBCP_Token(s, b2, &ty2);
				s=BGBCP_Token(s, b3, &ty3);
				strcat(b, "/");
				strcat(b, b3);
				BGBCP_Token(s, b2, &ty2);
			}
		}
#if 0
		else
			if(	(ctx->lang==BGBCC_LANG_BS2) ||
				(ctx->lang==BGBCC_LANG_CS))
		{
			while(!bgbcp_strcmp1(b2, "."))
			{
				s=BGBCP_Token(s, b2, &ty2);
				s=BGBCP_Token(s, b3, &ty3);
				strcat(b, "/");
				strcat(b, b3);
				BGBCP_Token(s, b2, &ty2);
			}
		}
#endif

		n=BCCX_NewCst(&bgbcc_rcst_ref, "ref");
		BCCX_SetCst(n, &bgbcc_rcst_name, "name", b);
		*str=s;
		return(n);
	}

	if(	(ctx->lang==BGBCC_LANG_C) ||
		(ctx->lang==BGBCC_LANG_CPP))
	{
//		if(!bgbcp_strcmp1(b, "[") && (ty==BTK_BRACE))
		if(!bgbcp_strcmp1(b, "[") && (ty==BTK_BRACE) &&
			(bgbcp_strcmp1(b2, "[")!=0))
		{
			s1=BGBCP_Token(s, b2, &ty2);
			if(!strcmp(b2, "="))
				{ s=BGBCP_Token(s, b2, &ty2); }
			else if(!strcmp(b2, "&"))
				{ s=BGBCP_Token(s, b2, &ty2); }
			
			n4=BGBCP_FunArgs(ctx, &s);

			BGBCP_Token(s, b2, &ty2);
			
//			if(!strcmp(b2, "]"))
//			{
//			}
			
//			s2=BGBCP_Token(s1, b3, &ty3);
			
//			if(!strcmp(b2, "]") && !strcmp(b3, "("))
			if(1)
			{
//				s=BGBCP_Token(s1, b2, &ty2);

				is_n = 1;

				n1=NULL;
				n2=NULL;
				n3=NULL;
				if(!bgbcp_strcmp1(b2, "("))
				{
					s=BGBCP_Token(s, b2, &ty2); //'('
					n1=BGBCP_FunVarsList(ctx, &s);
					BGBCP_Token(s, b2, &ty2);
				}

				if(	!bgbcp_strcmp1(b2, ":") ||
					!bgbcp_strcmp2(b2, "->"))
				{
					s=BGBCP_Token(s, b2, &ty2); //'('
					n2=BGBCP_DefExpectType(ctx, &s);
					BGBCP_Token(s, b2, &ty2);
				}else
				{
					n2=BCCX_NewCst(&bgbcc_rcst_type, "type");
					BCCX_SetCst(n2, &bgbcc_rcst_name, "name", "variant");
				}

				if(	!bgbcp_strcmp1(b2, "{"))
				{
	//				s=BGBCP_Token(s, b2, &ty2); //'('

					i=ctx->in_func_body;
					ctx->in_func_body=1;
	//				tk0=BGBCP_GetTokenCount();
					n3=BGBCP_BlockStatement2(ctx, &s);
	//				tk1=BGBCP_GetTokenCount();
					ctx->in_func_body=i;

					BGBCP_Token(s, b2, &ty2);
				}
				
				n=BCCX_NewCst(&bgbcc_rcst_lambda, "lambda");
				if(n1)
				{
	//				BCCX_Add(n, n1);
					n1=BCCX_NewCst1(&bgbcc_rcst_args, "args", n1);
					BCCX_AddV(n, n1);

				}
				if(n2)
				{
					BCCX_Add(n, n2);
				}
				if(n3)
				{
	//				BCCX_Add(n, n3);
					n3=BCCX_NewCst1V(&bgbcc_rcst_body, "body", n3);
					BCCX_AddV(n, n3);
				}

				if(n4)
				{
					n4=BCCX_NewCst1(&bgbcc_rcst_list, "list", n4);
					BCCX_AddV(n, n4);
				}

				li=BGBCC_GenSymInt();
				li&=0x0FFFFFFFU;
				if(is_n)
					li|=0x10000000U;
				BCCX_SetIntCst(n, &bgbcc_rcst_index, "index", li);

				*str=s;
				return(n);


			}
		}
	}

	BGBCP_Token(s, b2, &ty2);

	if(ty==BTK_NUMBER)
	{
		BGBCP_Token(s, b2, &ty2);
		
		if(isveril && bgbcp_strcmp1(b2, "\'"))
		{
			sprintf(b3, "U%u", atoi(b));
			suf=bgbcc_strdup(b3);			

			s=BGBCP_Token(s, b2, &ty2);
			s=BGBCP_Token(s, b3, &ty3);

			if((b3[0]=='h') || (b3[0]=='H'))
			{
				strcpy(b, "0x");
				strcat(b, b3+1);
				n=BGBCP_NumberSuf(ctx, b, suf);
				*str=s;
				return(n);
			}
			if((b3[0]=='b') || (b3[0]=='B'))
			{
				strcpy(b, "0b");
				strcat(b, b3+1);
				n=BGBCP_NumberSuf(ctx, b, suf);
				*str=s;
				return(n);
			}
			if((b3[0]=='o') || (b3[0]=='O'))
			{
				strcpy(b, "0");
				strcat(b, b3+1);
				n=BGBCP_NumberSuf(ctx, b, suf);
				*str=s;
				return(n);
			}
			if((b3[0]=='d') || (b3[0]=='D'))
			{
				strcpy(b, b3+1);
				n=BGBCP_NumberSuf(ctx, b, suf);
				*str=s;
				return(n);
			}
		}
		
		if((	!bgbcp_strcmp1(b2, "i") ||
				!bgbcp_strcmp1(b2, "I") ||
				!bgbcp_strcmp2(b2, "if") ||
				!bgbcp_strcmp2(b2, "iF") ||
				!bgbcp_strcmp2(b2, "If") ||
				!bgbcp_strcmp2(b2, "IF") ||
				!bgbcp_strcmp2(b2, "id") ||
				!bgbcp_strcmp2(b2, "iD") ||
				!bgbcp_strcmp2(b2, "Id") ||
				!bgbcp_strcmp2(b2, "ID")
				) && (ty2==BTK_NAME))
		{
			s=BGBCP_Token(s, b2, &ty2);
//			if(!bgbcp_strcmp1(b2, "i"))
			if(1)
			{
				n=BCCX_NewCst(&bgbcc_rcst_complex, "complex");
				BCCX_SetCst(n, &bgbcc_rcst_real, "real", "0");
				BCCX_SetCst(n, &bgbcc_rcst_imag, "imag", b);
			}
			*str=s;
			return(n);

		}

		if((	!bgbcp_strcmp1(b2, "j") ||
				!bgbcp_strcmp1(b2, "J") ||
				!bgbcp_strcmp2(b2, "jf") ||
				!bgbcp_strcmp2(b2, "jF") ||
				!bgbcp_strcmp2(b2, "Jf") ||
				!bgbcp_strcmp2(b2, "JF") ||
				!bgbcp_strcmp2(b2, "jd") ||
				!bgbcp_strcmp2(b2, "jD") ||
				!bgbcp_strcmp2(b2, "Jd") ||
				!bgbcp_strcmp2(b2, "JD")
				) && (ty2==BTK_NAME))
		{
			s=BGBCP_Token(s, b2, &ty2);
			n=BCCX_NewCst(&bgbcc_rcst_vec4, "vec4");
			BCCX_SetCst(n, &bgbcc_rcst_value_x0, "value_x0", "0");
			BCCX_SetCst(n, &bgbcc_rcst_value_x1, "value_x1", b);
			BCCX_SetCst(n, &bgbcc_rcst_value_x2, "value_x2", "0");
			BCCX_SetCst(n, &bgbcc_rcst_value_x3, "value_x3", "0");
			BCCX_SetCst(n, &bgbcc_rcst_tysuf, "tysuf", "QF");
			*str=s;
			return(n);
		}

		if((	!bgbcp_strcmp1(b2, "k") ||
				!bgbcp_strcmp1(b2, "K") ||
				!bgbcp_strcmp2(b2, "kf") ||
				!bgbcp_strcmp2(b2, "kF") ||
				!bgbcp_strcmp2(b2, "Kf") ||
				!bgbcp_strcmp2(b2, "KF") ||
				!bgbcp_strcmp2(b2, "kd") ||
				!bgbcp_strcmp2(b2, "kD") ||
				!bgbcp_strcmp2(b2, "Kd") ||
				!bgbcp_strcmp2(b2, "KD")
				) && (ty2==BTK_NAME))
		{
			s=BGBCP_Token(s, b2, &ty2);
			n=BCCX_NewCst(&bgbcc_rcst_vec4, "vec4");
			BCCX_SetCst(n, &bgbcc_rcst_value_x0, "value_x0", "0");
			BCCX_SetCst(n, &bgbcc_rcst_value_x1, "value_x1", "0");
			BCCX_SetCst(n, &bgbcc_rcst_value_x2, "value_x2", b);
			BCCX_SetCst(n, &bgbcc_rcst_value_x3, "value_x3", "0");
			BCCX_SetCst(n, &bgbcc_rcst_tysuf, "tysuf", "QF");
			*str=s;
			return(n);
		}

		suf=NULL;
		if(ty2==BTK_NAME)
		{
			if(!bgbcp_strcmp1(b2, "F") || !bgbcp_strcmp1(b2, "f"))suf="F";
			if(!bgbcp_strcmp1(b2, "D") || !bgbcp_strcmp1(b2, "d"))suf="D";
			if(!bgbcp_strcmp2(b2, "G") || !bgbcp_strcmp2(b2, "g"))suf="XF";
			if(!bgbcp_strcmp2(b2, "XF") || !bgbcp_strcmp2(b2, "xf"))suf="XF";
			if(!bgbcp_strcmp2(b2, "HF") || !bgbcp_strcmp2(b2, "hf"))suf="HF";
			if(!bgbcp_strcmp2(b2, "SF") || !bgbcp_strcmp2(b2, "sf"))suf="HF";

			if(!bgbcp_strcmp1(b2, "L") || !bgbcp_strcmp1(b2, "l"))suf="L";
			if(!bgbcp_strcmp2(b2, "LL") || !bgbcp_strcmp2(b2, "ll"))suf="LL";
			if(!bgbcp_strcmp1(b2, "U") || !bgbcp_strcmp1(b2, "u"))suf="U";
			if(!bgbcp_strcmp2(b2, "UL") || !bgbcp_strcmp2(b2, "ul"))suf="UL";
			if(!bgbcp_strcmp3(b2, "ULL") || !bgbcp_strcmp3(b2, "ull"))suf="ULL";

			if(!bgbcp_strcmp2(b2, "XL") || !bgbcp_strcmp2(b2, "xl"))suf="XL";
			if(!bgbcp_strcmp3(b2, "UXL") || !bgbcp_strcmp3(b2, "uxl"))suf="UXL";

			if(!bgbcp_strcmp3(b2, "LLX") || !bgbcp_strcmp3(b2, "llx"))suf="XL";
			if(!bgbcp_strcmp4(b2, "ULLX") || !bgbcp_strcmp4(b2, "ullx"))
				suf="UXL";


			if(!bgbcp_strcmp3(b2, "F16") || !bgbcp_strcmp3(b2, "f16"))suf="HF";
			if(!bgbcp_strcmp3(b2, "F32") || !bgbcp_strcmp3(b2, "f32"))suf="F";
			if(!bgbcp_strcmp3(b2, "F64") || !bgbcp_strcmp3(b2, "f64"))suf="D";
			if(!bgbcp_strcmp4(b2, "F128") || !bgbcp_strcmp4(b2, "f128"))
				suf="XF";

			if(!bgbcp_strcmp3(b2, "I32") || !bgbcp_strcmp3(b2, "i32"))suf="I";
			if(!bgbcp_strcmp3(b2, "I64") || !bgbcp_strcmp3(b2, "i64"))suf="LL";
			if(!bgbcp_strcmp4(b2, "I128") || !bgbcp_strcmp4(b2, "i128"))
				suf="XL";
//			if(!bgbcp_strcmp3(b2, "U32") || !bgbcp_strcmp3(b2, "u32"))suf="U";
//			if(!bgbcp_strcmp3(b2, "U64") || !bgbcp_strcmp3(b2, "u64"))
//				suf="ULL";
//			if(!bgbcp_strcmp4(b2, "U128") || !bgbcp_strcmp4(b2, "u128"))
//				suf="UXL";

			if(!bgbcp_strcmp4(b2, "UI32") || !bgbcp_strcmp4(b2, "ui32"))
				suf="U";
			if(!bgbcp_strcmp4(b2, "UI64") || !bgbcp_strcmp4(b2, "ui64"))
				suf="ULL";
			if(!bgbcp_strcmp5(b2, "UI128") || !bgbcp_strcmp5(b2, "ui128"))
				suf="UXL";
			
			if(	((b2[0]=='i') || (b2[0]=='I')) &&
				(b2[1]>='0') && (b2[1]<='9') &&
				!suf)
			{
				sprintf(b3, "I%u", atoi(b2+1));
				suf=bgbcc_strdup(b3);
			}

			if(	((b2[0]=='u') || (b2[0]=='U')) &&
				(b2[1]>='0') && (b2[1]<='9') &&
				!suf)
			{
				sprintf(b3, "U%u", atoi(b2+1));
				suf=bgbcc_strdup(b3);
			}

			if(	((b2[0]=='u') || (b2[0]=='U')) &&
				((b2[1]=='i') || (b2[1]=='I')) &&
				(b2[2]>='0') && (b2[2]<='9') &&
				!suf)
			{
				sprintf(b3, "U%u", atoi(b2+2));
				suf=bgbcc_strdup(b3);
			}
		}

		if(suf && (ty2==BTK_NAME) && !bgbcp_strcmp2(suf, "XF"))
		{
			s=BGBCP_Token(s, b2, &ty2);
			*str=s;
			n=BGBCP_NumberXF(ctx, b);
			BCCX_SetCst(n, &bgbcc_rcst_tysuf, "tysuf", suf);
			return(n);
		}

		if(suf && (ty2==BTK_NAME))
		{
			s=BGBCP_Token(s, b2, &ty2);
			*str=s;
//			n=BGBCP_Number(ctx, b);
			n=BGBCP_NumberSuf(ctx, b, suf);
			BCCX_SetCst(n, &bgbcc_rcst_tysuf, "tysuf", suf);
			return(n);
		}

		n=BGBCP_Number(ctx, b);
		*str=s;
		return(n);
	}

#if 0
	if(ty==BTK_STRING)
	{
		t1=stbuf;
		strcpy(t1, b);
		t1+=strlen(t1);

		BGBCP_Token(s, b2, &ty2);
		while(ty2==BTK_STRING)
		{
			s=BGBCP_Token(s, b2, &ty2);
//			strcat(b, b2);
			strcpy(t1, b2);
			t1+=strlen(t1);
			
			BGBCP_Token(s, b2, &ty2);
		}

		n=BCCX_NewCst(&bgbcc_rcst_string, "string");
//		BCCX_SetCst(n, &bgbcc_rcst_value, "value", b);
		BCCX_SetCst(n, &bgbcc_rcst_value, "value", stbuf);

		*str=s;
		return(n);
	}
#endif

	if(ty==BTK_CHARSTRING)
	{
		BGBCP_Token(s, b2, &ty2);
		while(ty2==BTK_CHARSTRING)
		{
			s=BGBCP_Token(s, b2, &ty2);
			strcat(b, b2);
			BGBCP_Token(s, b2, &ty2);
		}

		n=BCCX_NewCst(&bgbcc_rcst_charstring, "charstring");
		BCCX_SetCst(n, &bgbcc_rcst_value, "value", b);

		*str=s;
		return(n);
	}

	BGBCP_Error(s, "parse error before '%s'\n", b);

	*str=NULL;
	return(NULL);
}

/**
 * Check if function name refers to a known intrinsic function.
 * These are functions which map fairly directly to CPU instructions.
 * This excludes builtins with more complex logic (ex: memcpy or similar).
 * Also excludes functions which may effect predication state.
 *
 * This is primarily intended for the front-end.
 * For the back-end, intrinsics are handled elsewhere.
 */
int BGBCP_FunctionNameIsIntrinsicP(BGBCP_ParseState *ctx, char *str)
{
	if(!strcmp(str, "__debugbreak"))
		return(1);
	return(0);
}

BCCX_Node *BGBCP_ExpressionPostfix(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t, *s0, *s1, *s2;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;
	int i;

	s=*str;

	n=BGBCP_ExpressionLit(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		s1=BGBCP_Token(s, b, &ty);
		s2=BGBCP_Token(s1, b2, &ty2);

		if(!bgbcp_strcmp1(b, "[") && (ty==BTK_BRACE))
		{
//			s=BGBCP_Token(s, b, &ty); //[
			s=s1;
//			n1=BGBCP_Expression(ctx, &s);
			n1=BGBCP_ExpressionColon(ctx, &s);
			s=BGBCP_Token(s, b, &ty); //]
			n=BCCX_NewCst2(&bgbcc_rcst_getindex, "getindex",
				BCCX_NewCst1V(&bgbcc_rcst_array, "array", n),
				BCCX_NewCst1V(&bgbcc_rcst_index, "index", n1));
			continue;
		}

//		if(!bgbcp_strcmp1(b, ".") && (ty==BTK_SEPERATOR))
		if(!bgbcp_strcmp1(b, ".") &&
			(ty==BTK_SEPERATOR) &&
			(ty2==BTK_NAME))
		{
//			s=BGBCP_Token(s, b, &ty); //.
//			s=BGBCP_Token(s, b, &ty); //name
			s=s2;
			n=BCCX_NewCst1(&bgbcc_rcst_objref, "objref",
				BCCX_NewCst1V(&bgbcc_rcst_value, "value", n));
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", b2);
			continue;
		}

//		if(!bgbcp_strcmp2(b, "->") && (ty==BTK_OPERATOR))
		if(!bgbcp_strcmp2(b, "->") &&
			(ty==BTK_OPERATOR) &&
			(ty2==BTK_NAME))
		{
//			s=BGBCP_Token(s, b, &ty); //.
//			s=BGBCP_Token(s, b, &ty); //name
			s=s2;
			n=BCCX_NewCst1(&bgbcc_rcst_objref, "objref",
				BCCX_NewCst1V(&bgbcc_rcst_value, "value", n));
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", b2);
			continue;
		}

#if 1
		if(!bgbcp_strcmp1(b, "(") && (ty==BTK_BRACE))
		{
			if(BCCX_TagIsCstP(n, &bgbcc_rcst_objref, "objref"))
			{
				s=BGBCP_Token(s, b, &ty); //'('
				n1=BGBCP_FunArgs(ctx, &s);

				BCCX_SetTagCst(n, &bgbcc_rcst_methodcall, "methodcall");
				BCCX_AddV(n, BCCX_NewCst1(&bgbcc_rcst_args, "args", n1));
				continue;
			}

			s=BGBCP_Token(s, b, &ty); //'('
			n1=BGBCP_FunArgs(ctx, &s);
			if(!s)
			{
				*str=s;
				return(n);
			}

			if(BCCX_TagIsCstP(n, &bgbcc_rcst_ref, "ref"))
			{
				s0=BCCX_GetCst(n, &bgbcc_rcst_name, "name");
				if(BGBCP_FunctionNameIsIntrinsicP(ctx, s0))
				{
					n=BCCX_NewCst1(
						&bgbcc_rcst_funcall_intrin, "funcall_intrin",
						BCCX_NewCst1(&bgbcc_rcst_args, "args", n1));
				}else
				{
					n=BCCX_NewCst1(&bgbcc_rcst_funcall, "funcall",
						BCCX_NewCst1(&bgbcc_rcst_args, "args", n1));
				}
				BCCX_SetCst(n, &bgbcc_rcst_name, "name", s0);
				continue;
			}

			n=BCCX_NewCst2(&bgbcc_rcst_funcall, "funcall",
				BCCX_NewCst1V(&bgbcc_rcst_value, "value", n),
				BCCX_NewCst1(&bgbcc_rcst_args, "args", n1));
			continue;
		}

		if(!bgbcp_strcmp1(b, "<") &&
			((ty==BTK_BRACE) || (ty==BTK_OPERATOR)))
		{
			if(BCCX_TagIsCstP(n, &bgbcc_rcst_ref, "ref"))
			{
				s0=BCCX_GetCst(n, &bgbcc_rcst_name, "name");
				n1=BGBCP_LookupTemplateFunc(ctx, s0);
				if(n1)
				{
					s=s1;
					n2=BGBCP_GenArgs(ctx, &s);
					BGBCP_GenMangledTemplateName(ctx, b2, s0, n2);
					BGBCP_InstanceTemplate(ctx, b2, n1, n2);

					n=BCCX_Clone(n);
					BCCX_SetCst(n, &bgbcc_rcst_name, "name", b2);
					continue;
				}
			}
		}

		if(!bgbcp_strcmp2(b, "++") && (ty==BTK_OPERATOR))
		{
//			s=BGBCP_Token(s, b, &ty);
			s=s1;
			n=BCCX_NewCst1(&bgbcc_rcst_postinc, "postinc",
				BCCX_NewCst1V(&bgbcc_rcst_expr, "expr", n));
			continue;
		}
		if(!bgbcp_strcmp2(b, "--") && (ty==BTK_OPERATOR))
		{
//			s=BGBCP_Token(s, b, &ty);
			s=s1;
			n=BCCX_NewCst1(&bgbcc_rcst_postdec, "postdec",
				BCCX_NewCst1V(&bgbcc_rcst_expr, "expr", n));
			continue;
		}
#endif

		break;
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionUnary(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	t=BGBCP_Token(s, b, &ty);
	BGBCP_Token(t, b2, &ty2);

	if(!bgbcp_strcmp2(b, "++") && (ty==BTK_OPERATOR))
	{
		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionUnary(ctx, &s);
		n=BCCX_NewCst1(&bgbcc_rcst_preinc, "preinc",
			BCCX_NewCst1V(&bgbcc_rcst_expr, "expr", n1));
		*str=s;
		return(n);
	}
	if(!bgbcp_strcmp2(b, "--") && (ty==BTK_OPERATOR))
	{
		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionUnary(ctx, &s);
		n=BCCX_NewCst1(&bgbcc_rcst_predec, "predec",
			BCCX_NewCst1V(&bgbcc_rcst_expr, "expr", n1));
		*str=s;
		return(n);
	}

	if((!bgbcp_strcmp1(b, "+") ||	!bgbcp_strcmp1(b, "-") ||
		!bgbcp_strcmp1(b, "!") ||	!bgbcp_strcmp1(b, "~") ||
		!bgbcp_strcmp1(b, "&") ||	!bgbcp_strcmp1(b, "*")) &&
			(ty==BTK_OPERATOR))
	{
		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionCast(ctx, &s);
		n1=BCCX_NewCst1V(&bgbcc_rcst_value, "value", n1);
		n=BCCX_NewCst1(&bgbcc_rcst_unary, "unary", n1);
		BCCX_SetCst(n, &bgbcc_rcst_op, "op", b);
		*str=s;
		return(n);
	}

	if(ctx->lang==BGBCC_LANG_VERILOG)
	{
		if((	!bgbcp_strcmp(b, "posedge") ||
				!bgbcp_strcmp(b, "negedge")	) &&
				(ty==BTK_NAME))
		{
			s=BGBCP_Token(s, b, &ty);
			n1=BGBCP_ExpressionCast(ctx, &s);
			n1=BCCX_NewCst1V(&bgbcc_rcst_value, "value", n1);
			n=BCCX_NewCst1(&bgbcc_rcst_unary, "unary", n1);
			BCCX_SetCst(n, &bgbcc_rcst_op, "op", b);
			*str=s;
			return(n);
		}

	}

#if 1
//	if(!bgbcp_strcmp(b, "sizeof") &&
	if(!bgbcp_strcmp6(b, "sizeof") &&
		(ty==BTK_NAME))
	{
		s=BGBCP_Token(s, b, &ty);

		if(!bgbcp_strcmp1(b2, "(") && (ty2==BTK_BRACE))
		{
			s=BGBCP_Token(s, b2, &ty2);

//			n=BGBCP_DefType(ctx, &s);
//			n1=BGBCP_DefName2(ctx, &s);
//			n=BGBCP_ExpandDefinition(n, n1);
//			n=CDR(n);

			n=BGBCP_ArgDefinition2(ctx, &s);
			if(n)
			{
				n=BCCX_NewCst1(&bgbcc_rcst_sizeof, "sizeof",
					BCCX_NewCst1(&bgbcc_rcst_tyexpr, "tyexpr", n));
			}else
			{
				n1=BGBCP_Expression(ctx, &s);
				n=BCCX_NewCst1(&bgbcc_rcst_sizeof_expr, "sizeof_expr", 
					BCCX_NewCst1(&bgbcc_rcst_value, "value", n1));
			}

			s=BGBCP_Token(s, b2, &ty2);
			if(bgbcp_strcmp1(b2, ")"))
				BGBCP_Error(s,
					"sizeof token '%s'\n", b2);

			*str=s;
			return(n);
		}

		n=BGBCP_ArgDefinition2(ctx, &s);
		if(n)
		{
			n=BCCX_NewCst1(&bgbcc_rcst_sizeof, "sizeof",
				BCCX_NewCst1(&bgbcc_rcst_tyexpr, "tyexpr", n));
		}else
		{
			n1=BGBCP_Expression(ctx, &s);
			n=BCCX_NewCst1(&bgbcc_rcst_sizeof_expr, "sizeof_expr",
				BCCX_NewCst1(&bgbcc_rcst_value, "value", n1));
		}

//		n1=BGBCP_Expression(ctx, &s);
//		n=BCCX_NewCst1(&bgbcc_rcst_sizeof_expr, "sizeof_expr", 
//			BCCX_NewCst1V(&bgbcc_rcst_value, "value", n1));
		*str=s;
		return(n);
	}
#endif

	s=*str;
	n=BGBCP_ExpressionPostfix(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionCast(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t, *s1;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2;

	s=*str;
	s1=BGBCP_Token(s, b, &ty);
	if(!bgbcp_strcmp1(b, "(") && (ty==BTK_BRACE))
	{
//		n=BGBCP_DefType(ctx, &s1);
		n=BGBCP_ArgDefinition2(ctx, &s1);
		if(n)
		{
//			n1=BGBCP_DefName2(ctx, &s1);
//			n=BGBCP_ExpandDefinition(n, n1);
//			n=CDR(n);

			n2=BCCX_FindTagCst(n, &bgbcc_rcst_type, "type");
			n2=BCCX_Clone(n2);

			s=s1;
			s=BGBCP_Token(s, b, &ty);
			s1=BGBCP_Token(s, b2, &ty2);

			if(bgbcp_strcmp1(b, ")"))
			{
//				printf("parser, cast token '%s'\n", b);
				BGBCP_ErrorCtx(ctx, s, "parser, cast token '%s'\n", b);
			}

			if(!bgbcp_strcmp1(b2, "{") && (ty2==BTK_BRACE))
			{
				s=s1;

//				n1=BGBCP_FunArgs(ctx, &s);
				n1=BGBCP_FunArgsFlag(ctx, &s, 1);
				n1=BCCX_NewCst1(&bgbcc_rcst_list, "list", n1);
				n=BCCX_NewCst1(&bgbcc_rcst_object, "object", n1);
				BCCX_Add(n, n2);

				*str=s;
				return(n);
			}else
			{
				n1=BGBCP_ExpressionCast(ctx, &s);

				BCCX_CheckDeleteUnlinked(n);

				n=BCCX_NewCst2(&bgbcc_rcst_cast, "cast", n2,
					BCCX_NewCst1V(&bgbcc_rcst_value, "value", n1));

				*str=s;
				return(n);
			}
		}
	}

	n=BGBCP_ExpressionUnary(ctx, str);
	return(n);
}

BCCX_Node *BGBCP_BinaryExpr(char *op, BCCX_Node *l, BCCX_Node *r)
{
	BCCX_Node *n, *n1, *n2;

	n1=BCCX_NewCst1V(&bgbcc_rcst_left, "left", l);
	n2=BCCX_NewCst1V(&bgbcc_rcst_right, "right", r);
	n=BCCX_NewCst2(&bgbcc_rcst_binary, "binary", n1, n2);
	BCCX_SetCst(n, &bgbcc_rcst_op, "op", op);

	return(n);
}

BCCX_Node *BGBCP_UnaryExpr(char *op, BCCX_Node *v)
{
	BCCX_Node *n, *n1, *n2;

	n1=BCCX_NewCst1V(&bgbcc_rcst_value, "value", v);
	n=BCCX_NewCst1(&bgbcc_rcst_unary, "unary", n1);
	BCCX_SetCst(n, &bgbcc_rcst_op, "op", op);

	return(n);
}

BCCX_Node *BGBCP_IfExpr(BCCX_Node *cond, BCCX_Node *ln, BCCX_Node *rn)
{
	BCCX_Node *n, *n1, *n2, *n3;

	if(ln && rn)
	{
		n1=BCCX_NewCst1V(&bgbcc_rcst_then, "then", ln);
		n2=BCCX_NewCst1V(&bgbcc_rcst_else, "else", rn);
		n3=BCCX_NewCst1V(&bgbcc_rcst_cond, "cond", cond);
		n=BCCX_NewCst3(&bgbcc_rcst_if, "if", n3, n1, n2);
	}else if(ln)
	{
		n1=BCCX_NewCst1V(&bgbcc_rcst_then, "then", ln);
		n3=BCCX_NewCst1V(&bgbcc_rcst_cond, "cond", cond);
		n=BCCX_NewCst2(&bgbcc_rcst_if, "if", n3, n1);
	}else if(rn)
	{
		n1=BCCX_NewCst1V(&bgbcc_rcst_then, "then", rn);
		n2=BGBCP_UnaryExpr("!", cond);
		n3=BCCX_NewCst1V(&bgbcc_rcst_cond, "cond", n2);
		n=BCCX_NewCst2(&bgbcc_rcst_if, "if", n3, n1);
	}else
	{
		n=NULL;
	}

	return(n);
}

#if 0
BCCX_Node *BGBCP_ExpressionExp(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionCast(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		t=BGBCP_Token(s, b, &ty);
		if(ty!=BTK_OPERATOR)
			break;

		if(	bgbcp_strcmp1(b, "`") && bgbcp_strcmp2(b, "``") &&
			bgbcp_strcmp1(b, "&") && bgbcp_strcmp2(b, "&`") &&
			bgbcp_strcmp2(b, "`&") && bgbcp_strcmp1(b, "|") &&
			bgbcp_strcmp2(b, "|`") && bgbcp_strcmp2(b, "`|") &&
			bgbcp_strcmp1(b, "^") && bgbcp_strcmp2(b, "^`") &&
			bgbcp_strcmp2(b, "`^")	)
				break;

		s=BGBCP_Token(s, b, &ty);
		if(!bgbcp_strcmp1(b, "`"))strcpy(b, "**");

		n1=BGBCP_ExpressionCast(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}

	*str=s;
	return(n);
}
#endif

BCCX_Node *BGBCP_ExpressionMD(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
//	n=BGBCP_ExpressionExp(ctx, &s);
	n=BGBCP_ExpressionCast(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		t=BGBCP_Token(s, b, &ty);
		if(ty!=BTK_OPERATOR)
			break;

		if(	bgbcp_strcmp1(b, "*") && bgbcp_strcmp1(b, "/") &&
			bgbcp_strcmp1(b, "%") && bgbcp_strcmp1(b, "*`") &&
			bgbcp_strcmp2(b, "/`") && bgbcp_strcmp2(b, "%`") &&
			bgbcp_strcmp2(b, "`*") && bgbcp_strcmp2(b, "`/") &&
			bgbcp_strcmp2(b, "`%") && bgbcp_strcmp1(b, "~") &&
			bgbcp_strcmp2(b, "~`") && bgbcp_strcmp2(b, "`~"))
				break;

		s=BGBCP_Token(s, b, &ty);
//		n1=BGBCP_ExpressionExp(ctx, &s);
		n1=BGBCP_ExpressionCast(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionAS(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionMD(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		t=BGBCP_Token(s, b, &ty);
		if(ty!=BTK_OPERATOR)
			break;

		if(	bgbcp_strcmp1(b, "+") && bgbcp_strcmp1(b, "-") &&
			bgbcp_strcmp2(b, "+`") && bgbcp_strcmp2(b, "-`") &&
			bgbcp_strcmp2(b, "`+") && bgbcp_strcmp2(b, "`-"))
			break;

		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionMD(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionSHLR(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionAS(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		t=BGBCP_Token(s, b, &ty);
		if(ty!=BTK_OPERATOR)
			break;
		if(bgbcp_strcmp2(b, "<<") && bgbcp_strcmp2(b, ">>"))
			break;

		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionAS(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionRCmp(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t, *s1;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionSHLR(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		s1=BGBCP_Token(s, b, &ty);

		if(ty==BTK_NAME)
		{
			if(	(!bgbcp_strcmp(b, "instanceof") && (
					(ctx->lang==BGBCC_LANG_JAVA) ||
					(ctx->lang==BGBCC_LANG_BS2))) ||
				!bgbcp_strcmp(b, "__instanceof"))
			{
				n2=BGBCP_DefExpectType(ctx, &s1);
				if(n2)
				{
					s=s1;
					n=BCCX_NewCst2(&bgbcc_rcst_instanceof, "instanceof", n2,
						BCCX_NewCst1V(&bgbcc_rcst_value, "value", n));
					continue;
				}
			}
		}

		if(ty!=BTK_OPERATOR)
			break;

		if(bgbcp_strcmp1(b, "<") && bgbcp_strcmp1(b, ">") &&
			bgbcp_strcmp2(b, "<=") && bgbcp_strcmp2(b, ">="))
			break;

		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionSHLR(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionECmp(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionRCmp(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		BGBCP_Token(s, b, &ty);

		if(ty!=BTK_OPERATOR)
			break;
		if(bgbcp_strcmp2(b, "==") && bgbcp_strcmp2(b, "!="))
			break;

		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionRCmp(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionBitAnd(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionECmp(ctx, &s);
	if(!n) { *str=NULL; return(n); }
	while(1)
	{
		t=BGBCP_Token(s, b, &ty);
		if(ty!=BTK_OPERATOR)break;
		if(bgbcp_strcmp1(b, "&"))break;
		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionECmp(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}
	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionBitXor(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionBitAnd(ctx, &s);
	if(!n) { *str=NULL; return(n); }
	while(1)
	{
		t=BGBCP_Token(s, b, &ty);
		if(ty!=BTK_OPERATOR)break;
		if(bgbcp_strcmp1(b, "^"))break;
		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionBitAnd(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}
	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionBitOr(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionBitXor(ctx, &s);
	if(!n) { *str=NULL; return(n); }
	while(1)
	{
		t=BGBCP_Token(s, b, &ty);
		if(ty!=BTK_OPERATOR)break;
		if(bgbcp_strcmp1(b, "|"))break;
		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionBitXor(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}
	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionLop(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionBitOr(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		BGBCP_Token(s, b, &ty);

		if(ty!=BTK_OPERATOR)break;
		if(bgbcp_strcmp2(b, "&&"))break;

		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionBitOr(ctx, &s);

		n=BGBCP_BinaryExpr("&&", n, n1);
//		n=BCCX_NewCst2(&bgbcc_rcst_l_and, "l_and",
//			BCCX_NewCst1V(&bgbcc_rcst_left, "left", n),
//			BCCX_NewCst1V(&bgbcc_rcst_right, "right", n1));

#if 0
		if(BCCX_TagIsCstP(n, &bgbcc_rcst_and, "and"))
		{
			BCCX_Add(n, BCCX_NewCst1V(&bgbcc_rcst_value, "value", n1));
			continue;
		}

		n=BCCX_NewCst2(&bgbcc_rcst_and, "and",
			BCCX_NewCst1V(&bgbcc_rcst_value, "value", n),
			BCCX_NewCst1V(&bgbcc_rcst_value, "value", n1));
#endif
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionLop2(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionLop(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		BGBCP_Token(s, b, &ty);

		if(ty!=BTK_OPERATOR)
			break;
		if(bgbcp_strcmp2(b, "||"))
			break;

		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionLop(ctx, &s);

		n=BGBCP_BinaryExpr("||", n, n1);
//		n=BCCX_NewCst2(&bgbcc_rcst_l_or, "l_or",
//			BCCX_NewCst1V(&bgbcc_rcst_left, "left", n),
//			BCCX_NewCst1V(&bgbcc_rcst_right, "right", n1));

#if 0
		if(BCCX_TagIsCstP(n, &bgbcc_rcst_or, "or"))
		{
			BCCX_Add(n, BCCX_NewCst1(&bgbcc_rcst_value, "value", n1));
			continue;
		}

		n=BCCX_NewCst2(&bgbcc_rcst_or, "or",
			BCCX_NewCst1(&bgbcc_rcst_value, "value", n),
			BCCX_NewCst1(&bgbcc_rcst_value, "value", n1));
#endif
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionTCond(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n1=BGBCP_ExpressionLop2(ctx, &s);
	BGBCP_Token(s, b, &ty);
	if(bgbcp_strcmp1(b, "?") || (ty!=BTK_OPERATOR))
	{
		*str=s;
		return(n1);
	}

//	s=BGBCP_Token(s, b, &ty); //?
	s=BGBCP_EatExpectToken(ctx, s, "?");
//	n2=BGBCP_ExpressionLop2(ctx, &s);
//	n2=BGBCP_Expression(ctx, &s);
	n2=BGBCP_Expression2(ctx, &s);
//	s=BGBCP_Token(s, b, &ty); //:
	s=BGBCP_EatExpectToken(ctx, s, ":");
//	n3=BGBCP_ExpressionTCond(ctx, &s);
//	n3=BGBCP_Expression(ctx, &s);
	n3=BGBCP_Expression2(ctx, &s);

	n=BCCX_NewCst3(&bgbcc_rcst_if, "if",
		BCCX_NewCst1V(&bgbcc_rcst_cond, "cond", n1),
		BCCX_NewCst1V(&bgbcc_rcst_then, "then", n2),
		BCCX_NewCst1V(&bgbcc_rcst_else, "else", n3));

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionEquals(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionTCond(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		BGBCP_Token(s, b, &ty);

		if(ty!=BTK_OPERATOR)
			break;

		if(	bgbcp_strcmp1(b, "=") &&	bgbcp_strcmp2(b, "+=") &&
			bgbcp_strcmp2(b, "-=") &&	bgbcp_strcmp2(b, "*=") &&
			bgbcp_strcmp2(b, "/=") &&	bgbcp_strcmp2(b, "%=")  &&
			bgbcp_strcmp2(b, "&=") &&	bgbcp_strcmp2(b, "|=") &&
			bgbcp_strcmp2(b, "^=") &&	bgbcp_strcmp3(b, "<<=") &&
			bgbcp_strcmp3(b, ">>="))
				break;

		t=NULL;
		if(!bgbcp_strcmp2(b, "+="))t="+";
		if(!bgbcp_strcmp2(b, "-="))t="-";
		if(!bgbcp_strcmp2(b, "*="))t="*";
		if(!bgbcp_strcmp2(b, "/="))t="/";
		if(!bgbcp_strcmp2(b, "%="))t="%";
		if(!bgbcp_strcmp2(b, "&="))t="&";
		if(!bgbcp_strcmp2(b, "|="))t="|";
		if(!bgbcp_strcmp2(b, "^="))t="^";
		if(!bgbcp_strcmp3(b, "<<="))t="<<";
		if(!bgbcp_strcmp3(b, ">>="))t=">>";

		s=BGBCP_Token(s, b, &ty);

#if 0
		if(BCCX_TagIsCstP(n, &bgbcc_rcst_ref, "ref"))
		{
			n1=BGBCP_ExpressionEquals(ctx, &s);
			n2=n;

			n=BCCX_NewCst1(&bgbcc_rcst_assign, "assign",
				BCCX_NewCst1(&bgbcc_rcst_value, "value", n1));
			if(t)BCCX_SetCst(n, &bgbcc_rcst_op, "op", t);

			t=BCCX_GetCst(n2, &bgbcc_rcst_name, "name");
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", t);

			continue;
		}

		if(	BCCX_TagIsCstP(n, &bgbcc_rcst_objref, "objref") ||
			BCCX_TagIsCstP(n, &bgbcc_rcst_getindex, "getindex"))
		{
			if(BCCX_TagIsCstP(n, &bgbcc_rcst_objref, "objref"))
			{
				n1=BGBCP_ExpressionEquals(ctx, &s);
				if(t)n1=BGBCP_BinaryExpr(t,
					BCCX_Clone(n), n1);

				n2=BCCX_Find(n, "value");
				BCCX_SetTagCst(n2, &bgbcc_rcst_object, "object");

				BCCX_SetTagCst(n, &bgbcc_rcst_objset, "objset");
				BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_value, "value", n1));
				continue;
			}
			if(BCCX_TagIsCstP(n, &bgbcc_rcst_getindex, "getindex"))
			{
				n1=BGBCP_ExpressionEquals(ctx, &s);
				if(t)n1=BGBCP_BinaryExpr(t,
					BCCX_Clone(n), n1);
				BCCX_SetTagCst(n, &bgbcc_rcst_setindex, "setindex");
				BCCX_Add(n, BCCX_NewCst1(&bgbcc_rcst_value, "value", n1));
				continue;
			}
		}
#endif

		n1=BGBCP_ExpressionEquals(ctx, &s);
		n=BCCX_NewCst2(&bgbcc_rcst_assign, "assign",
			BCCX_NewCst1V(&bgbcc_rcst_left, "left", n),
			BCCX_NewCst1V(&bgbcc_rcst_right, "right", n1));
		if(t)BCCX_SetCst(n, &bgbcc_rcst_op, "op", t);

		continue;
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionComma(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionEquals(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		BGBCP_Token(s, b, &ty);
		if(ty!=BTK_SEPERATOR)break;
		if(bgbcp_strcmp1(b, ","))break;

		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionEquals(ctx, &s);
		n=BCCX_NewCst2(&bgbcc_rcst_comma, "comma",
			BCCX_NewCst1V(&bgbcc_rcst_left, "left", n),
			BCCX_NewCst1V(&bgbcc_rcst_right, "right", n1));
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_Expression(BGBCP_ParseState *ctx, char **str)
{
	return(BGBCP_ExpressionEquals(ctx, str));
}

BCCX_Node *BGBCP_Expression2(BGBCP_ParseState *ctx, char **str)
{
	return(BGBCP_ExpressionComma(ctx, str));
}

#if 0
// BCCX_Node *BGBCP_Expression3(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	int ty, ty2;
	char *s;
	BCCX_Node *n;

	s=*str;
	n=BGBCP_ExpressionEquals(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	BGBCP_Token(s, b, &ty);
	if(!bgbcp_strcmp1(b, ":") && (ty==BTK_SEPERATOR))
	{
		s=BGBCP_Token(s, b, &ty);
		if(BS1_SYMBOLP(n))n=KEYSYM(TOSYM(n));
			else n=LIST2S("colon", n);
	}

	*str=s;
	return(n);
}
#endif

BCCX_Node *BGBCP_ExpressionColon(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionEquals(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		BGBCP_Token(s, b, &ty);
		if(ty!=BTK_SEPERATOR)break;
		if(bgbcp_strcmp1(b, ":"))break;

		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionEquals(ctx, &s);
		n=BCCX_NewCst2(&bgbcc_rcst_colon, "colon",
			BCCX_NewCst1V(&bgbcc_rcst_left, "left", n),
			BCCX_NewCst1V(&bgbcc_rcst_right, "right", n1));
	}

	*str=s;
	return(n);
}

