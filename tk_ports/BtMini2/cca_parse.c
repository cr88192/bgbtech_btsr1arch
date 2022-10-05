// #include <bgbccc.h>

char *BCCX_GetPos(char **strm)
{
	return(*strm);
}

void BCCX_SetPos(char **strm, char *pos)
{
	*strm=pos;
}

int BCCX_PeekChar(char **strm)
{
	return(**strm);
}

int BCCX_ReadChar(char **strm)
{
	char *cs;
	int v;
	
	cs=*strm;
	v=*cs++;
	*strm=cs;
	return(v);
}

int BCCX_PeekCharIdx(char **strm, int idx)
{
	return((*strm)[idx]);
}


int BCCX_ParseEatWhite(char **strm)
{
	int i, r;

	i=0;
	while((BCCX_PeekChar(strm)>0) && (BCCX_PeekChar(strm)<=' '))
	{
		i=1;
		BCCX_ReadChar(strm);
	}

	if((BCCX_PeekCharIdx(strm, 0)=='<') &&
		(BCCX_PeekCharIdx(strm, 1)=='!') &&
		(BCCX_PeekCharIdx(strm, 2)=='-') &&
		(BCCX_PeekCharIdx(strm, 3)=='-'))
	{
		i=1;

		BCCX_ReadChar(strm);
		BCCX_ReadChar(strm);
		BCCX_ReadChar(strm);
		BCCX_ReadChar(strm);

		while(BCCX_PeekChar(strm)>0)
		{
			if((BCCX_PeekCharIdx(strm, 0)=='-') &&
				(BCCX_PeekCharIdx(strm, 1)=='-') &&
				(BCCX_PeekCharIdx(strm, 2)=='>'))
			{
				BCCX_ReadChar(strm);
				BCCX_ReadChar(strm);
				BCCX_ReadChar(strm);
				break;
			}
			BCCX_ReadChar(strm);
		}
	}

	if(i)BCCX_ParseEatWhite(strm);

	if(BCCX_PeekChar(strm)<0)
		return(-1);
	return(0);
}

int BCCX_ParseSpecialP(char **strm)
{
	switch(BCCX_PeekChar(strm))
	{
	case '<': case '>':
	case '/': case '=':
	case '?': case ':':
	case 0: case -1:
		return(1);
		break;

	default:
		return(0);
		break;
	}
	return(0);
}

int BCCX_ParseContSpecialP(char **strm)
{
	switch(BCCX_PeekChar(strm))
	{
	case '<': case '&':
	case 0: case -1:
		return(1);
		break;

	default:
		return(0);
		break;
	}
	return(0);
}

char *BCCX_ParseToken(char **strm, int *ty)
{
	char tb[256];
	char buf[16];
//	char *t, *t2, *b;
	char *t, *t2;
	int i, j, qtc;

//	b=bccx_ralloc(256); t=b;
//	*b=0;
	
	t=tb;
	*t=0;

	if(ty)*ty=0;

	i=BCCX_ParseEatWhite(strm);
	if(i<0)return(NULL);

	if(BCCX_ParseSpecialP(strm))
	{
		if(ty)*ty=BGBDY_TTY_SPECIAL;
		*t++=BCCX_ReadChar(strm);
		*t=0;
	}else if(	(BCCX_PeekChar(strm)=='"') ||
				(BCCX_PeekChar(strm)=='\'')) /* quoted string */
	{
		if(ty)*ty=BGBDY_TTY_STRING;
		qtc=BCCX_ReadChar(strm);
//		while((BCCX_PeekChar(strm)>0) && (BCCX_PeekChar(strm)!='\"'))
		while((BCCX_PeekChar(strm)>0) && (BCCX_PeekChar(strm)!=qtc))
		{
			if(BCCX_PeekChar(strm)=='&')
			{
				BCCX_ReadChar(strm);
				t2=buf;
				while((BCCX_PeekChar(strm)>0) &&
					(BCCX_PeekChar(strm)!=';'))
						*t2++=BCCX_ReadChar(strm);
				if(BCCX_PeekChar(strm)<0)return(NULL);
				*t2++=0;
				BCCX_ReadChar(strm);

				if(buf[0]=='#')
				{
					if(buf[1]=='x')
					{
						t2=buf+2;
						i=0;
						while(*t2)
						{
							i<<=4;
							if((*t2>='0') && (*t2<='9'))
								i+=*t2-'0';
							if((*t2>='A') && (*t2<='F'))
								i+=*t2-'A'+10;
							if((*t2>='a') && (*t2<='f'))
								i+=*t2-'a'+10;
							t++;
						}
						*t++=i;
					}else *t++=atoi(buf+1);
				}
				if(!strcmp(buf, "amp"))*t++='&';
				if(!strcmp(buf, "lt"))*t++='<';
				if(!strcmp(buf, "gt"))*t++='>';
				if(!strcmp(buf, "quot"))*t++='"';
				if(!strcmp(buf, "apos"))*t++='\'';
			}else
			{
//				BGBCP_EmitChar(&t, BCCX_ReadChar(strm));
				*t++=BCCX_ReadChar(strm);
			}
		}
		*t++=0;
		BCCX_ReadChar(strm);
	}else
	{	/* alphanumeric string that can contain '_' */
		if(ty)*ty=BGBDY_TTY_SYMBOL;

		while(BCCX_PeekChar(strm)>' ')
		{
			if(BCCX_ParseSpecialP(strm))break;
//			BGBCP_EmitChar(&t, BCCX_ReadChar(strm));
			*t++=BCCX_ReadChar(strm);
		}
		*t++=0;
	}

//	return(b);
	return(bccx_rstrdup(tb));
}

char *BCCX_PeekToken(char **strm, int *ty)
{
	char *pos;
	char *b;

	pos=BCCX_GetPos(strm);
	b=BCCX_ParseToken(strm, ty);
	BCCX_SetPos(strm, pos);
	return(b);
}

char *BCCX_ParseText(char **strm)
{
	char tb[4096];
	char *b, *t, *t1;
	char buf[16];
	int i, j, gws, rws;

//	b=bccx_ralloc(4096);
//	t=b; *t=0;

	t=tb; *t=0;

	i=BCCX_ParseEatWhite(strm);
	if(i<0)return(NULL);

	gws=0; rws=0;
	while(1)
	{
		while(!BCCX_ParseContSpecialP(strm))
		{
			if((BCCX_PeekChar(strm)=='\r') ||
				(BCCX_PeekChar(strm)=='\n'))
			{
				BCCX_ParseEatWhite(strm);
				if(!rws)
				{
//					*t++=' ';
					*t++='\n';
					gws++;
				}
				continue;
			}
			gws=0;
			if(BCCX_PeekChar(strm)<=' ')rws++;
				else rws=0;
			j=BCCX_ReadChar(strm);
			*t++=j;
			if(j==' ')
				gws++;
		}

		if(BCCX_PeekChar(strm)<=0)return(NULL);

		if(BCCX_PeekChar(strm)=='&')
		{
			BCCX_ReadChar(strm);
			t1=buf;
			while(BCCX_PeekChar(strm)!=';')
				*t1++=BCCX_ReadChar(strm);
			if(BCCX_PeekChar(strm)<=0)return(NULL);
			*t1++=0;
			BCCX_ReadChar(strm);

			if(buf[0]=='#')
			{
				if(buf[1]=='x')
				{
					t1=buf+2;
					i=0;
					while(*t1)
					{
						i<<=4;
						if((*t1>='0') && (*t1<='9'))
							i+=*t1-'0';
						if((*t1>='A') && (*t1<='F'))
							i+=*t1-'A'+10;
						if((*t1>='a') && (*t1<='f'))
							i+=*t1-'a'+10;
						t1++;
					}
					gws=0;
					if(i<=' ')rws++;
						else rws=0;
					*t++=i;
				}else
				{
					i=atoi(buf+1);
					gws=0;
					if(i<=' ')rws++;
						else rws=0;
					*t++=i;
				}
				continue;
			}
			rws=0;
			gws=0;

			if(!strcmp(buf, "amp"))*t++='&';
			if(!strcmp(buf, "lt"))*t++='<';
			if(!strcmp(buf, "gt"))*t++='>';
			if(!strcmp(buf, "apos"))*t++='\'';
			if(!strcmp(buf, "quot"))*t++='"';
		}else break;
	}
	t-=gws;
	*t++=0;

//	return(b);
	return(bccx_rstrdup(tb));
}

int BCCX_ParseCheckTokenValidIntP(char *str)
{
	char *s;
	
	s=str;
	if(*s=='-')
		s++;
	if((s[0]=='0') && ((s[1]=='x') || (s[1]=='X')))
	{
		s+=2;
		while(*s)
		{
			if((*s>='0') && (*s<='9'))
				{ s++; continue; }
			if((*s>='A') && (*s<='F'))
				{ s++; continue; }
			if((*s>='a') && (*s<='f'))
				{ s++; continue; }
			break;
		}
		if(!(*s))
			return(1);

		return(0);
	}
	
	if(s[0]=='0')
	{
		while(*s)
		{
			if((*s>='0') && (*s<='7'))
				{ s++; continue; }
			break;
		}
		if(!(*s))
			return(1);
		return(0);
	}

	while(*s)
	{
		if((*s>='0') && (*s<='9'))
			{ s++; continue; }
		break;
	}

	if(!(*s))
		return(1);

	return(0);
}

int BCCX_ParseCheckTokenValidRealP(char *str)
{
	char *s;
	
	s=str;
	if(*s=='-')
		s++;

	while(*s)
	{
		if((*s>='0') && (*s<='9'))
			{ s++; continue; }
		break;
	}
	
	if(*s=='.')
	{
		s++;

		while(*s)
		{
			if((*s>='0') && (*s<='9'))
				{ s++; continue; }
			break;
		}		
	}

	if((*s=='e') || *s=='E')
	{
		s++;

		while(*s)
		{
			if((*s>='0') && (*s<='9'))
				{ s++; continue; }
			break;
		}		
	}

	if(!(*s))
		return(1);

	return(0);
}

#if 1
int BCCX_ParseAttr(char **strm, BCCX_Node *node)
{
	char *ns, *var, *eq, *val, *s1;
	int ty;

	while(1)
	{
		ty=0;
		var=BCCX_PeekToken(strm, &ty);
		if(ty==BGBDY_TTY_NULL)
			return(-1);

		if((var[0]=='>') && (ty==BGBDY_TTY_SPECIAL)) break;
		if((var[0]=='/') && (ty==BGBDY_TTY_SPECIAL)) break;
		if((var[0]=='?') && (ty==BGBDY_TTY_SPECIAL)) break;

		if(ty==BGBDY_TTY_NULL)
			return(-1);
		if(ty!=BGBDY_TTY_SYMBOL)
		{
			printf("parse error (inv attribute).\n");
			*(int *)-1=-1;
			return(-1);
		}

		ns=NULL;

		var=BCCX_ParseToken(strm, &ty);
		if(ty==BGBDY_TTY_NULL)
			return(-1);

		eq=BCCX_ParseToken(strm, &ty);
		if(ty==BGBDY_TTY_NULL)
			return(-1);

		if((ty==BGBDY_TTY_SPECIAL) && (eq[0]==':'))
		{
			ns=var;

			var=BCCX_ParseToken(strm, &ty);
			if(ty==BGBDY_TTY_NULL)
				return(-1);

			eq=BCCX_ParseToken(strm, &ty);
			if(ty==BGBDY_TTY_NULL)
				return(-1);
		}

		if((ty!=BGBDY_TTY_SPECIAL) || (eq[0]!='='))
		{
			printf("parse error (attr equal).\n");
			*(int *)-1=-1;
			return(-1);
		}

		val=BCCX_ParseToken(strm, &ty);
		if(ty==BGBDY_TTY_NULL)
			return(-1);

		if(ty!=BGBDY_TTY_STRING)
		{
			if(BCCX_ParseCheckTokenValidIntP(val))
			{
				BCCX_SetInt(node, var, bccx_atoll(val));
				continue;
			}
		
			if(BCCX_ParseCheckTokenValidRealP(val))
			{
				BCCX_SetFloat(node, var, bccx_atof(val));
				continue;
			}

#if 0
			if(((val[0]>='0') && (val[0]<='9')) ||
				((val[0]=='-') && (val[1]>='0') && (val[1]<='9')))
			{
				s1=val;
				while(*s1 && (*s1!='.'))s1++;
				if(!(*s1))
				{
					BCCX_SetInt(node, var, bccx_atoll(val));
					continue;
				}
			
				BCCX_SetFloat(node, var, bccx_atof(val));
				continue;
			}
#endif

			if(val[0]=='$')
			{
				BCCX_Set(node, var, val);
				continue;
			}

			printf("parse error (inv attribute arg).\n");
			*(int *)-1=-1;
			return(-1);
		}

		BCCX_Set(node, var, val);
	}

	return(0);
}
#endif


BCCX_Node *BCCX_ParseTag(char **strm, int *rgrp)
{
	char *buf, *buf2, *key, *ns;
	int ty, i, j;
	char *s2;

	BCCX_Node *tmp, *t, *end;

	i=BCCX_ParseEatWhite(strm);
	if(i<0)return(NULL);

	ty=0;
	buf=BCCX_PeekToken(strm, &ty);
	if(ty==BGBDY_TTY_NULL) return(NULL);

	if(rgrp)*rgrp=0;

	if((buf[0]=='<') && (ty==BGBDY_TTY_SPECIAL))
	{
		buf=BCCX_ParseToken(strm, &ty);
		if(ty==BGBDY_TTY_NULL) return(NULL);

		if(BCCX_PeekChar(strm)=='?')
			BCCX_ReadChar(strm);
		if(BCCX_PeekChar(strm)=='!')
		{
			i=1;
			while((BCCX_PeekChar(strm)>0) && i)
			{
				j=BCCX_ReadChar(strm);
				if(j=='<')i++;
				if(j=='>')i--;
				if(j=='[')i++;
				if(j==']')i--;
			}
			return(BCCX_ParseExpr(strm));
		}

		if(BCCX_PeekChar(strm)=='/')
		{
			BCCX_ReadChar(strm);	//'/'

			key=BCCX_ParseToken(strm, &ty);	//tag or NS
			if(ty==BGBDY_TTY_NULL) return(NULL);

			if(BCCX_PeekChar(strm)==':')
			{
				ns=key;
				key=BCCX_ParseToken(strm, &ty);	//tag
				if(ty==BGBDY_TTY_NULL) return(NULL);
			}

			buf=BCCX_ParseToken(strm, &ty); //'>'
			if(ty==BGBDY_TTY_NULL) return(NULL);

			if(rgrp)*rgrp=2;
			return(NULL);
		}


		ns=NULL;

		key=BCCX_ParseToken(strm, &ty);
		if(ty==BGBDY_TTY_NULL)return(NULL);

		if(ty!=BGBDY_TTY_SYMBOL)
		{
			printf("parse error (inv tag).\n");
			*(int *)-1=-1;
			return(NULL);
		}

		if(BCCX_PeekChar(strm)==':')
		{
			BCCX_ReadChar(strm);
			ns=key;
			key=BCCX_ParseToken(strm, &ty);

			if(ty==BGBDY_TTY_NULL)
				return(NULL);
		}

		if((BCCX_PeekChar(strm)>' ') &&
			(BCCX_PeekChar(strm)!='>') && (BCCX_PeekChar(strm)!='/'))
		{
			printf("parse error (inv char after tag).\n");
			*(int *)-1=-1;
			return(NULL);
		}

		tmp=BCCX_AllocNode();
		tmp->itag=BCCX_StringToStridx(key);
		BCCX_ParseAttr(strm, tmp);

		buf=BCCX_ParseToken(strm, &ty);
		if(ty==BGBDY_TTY_NULL)
			return(NULL);

		if((buf[0]=='/') && (ty==BGBDY_TTY_SPECIAL))
		{
			buf=BCCX_ParseToken(strm, &ty);
			if(ty==BGBDY_TTY_NULL)
				return(NULL);
			return(tmp);
		}
		if((buf[0]=='?') && (ty==BGBDY_TTY_SPECIAL))
		{
			buf=BCCX_ParseToken(strm, &ty);
			if(ty==BGBDY_TTY_NULL)
				return(NULL);

			buf=bccx_ralloc(64);
			strcpy(buf, "?");
			strcat(buf, BCCX_StridxToString(tmp->itag));
			tmp->itag=BCCX_StringToStridx(buf);
			return(tmp);
		}
		if(buf[0]!='>')
		{
			printf("parse error (expected close '>').\n");
			*(int *)-1=-1;
			return(NULL);
		}

		if(rgrp)*rgrp=1;

		return(tmp);
	}

	s2=BCCX_ParseText(strm);
	if(BCCX_PeekChar(strm)<=0)
		return(NULL);

//	tmp=BCCX_AllocNode();
//	BCCX_Set(tmp, "$text", s2);
	tmp=BCCX_NewText(s2);

	return(tmp);
}

BCCX_Node *BCCX_ParseExprR(char **strm, int *rgrp)
{
	BCCX_Node *tmp, *tmp1, *end;
	int i;

	i=0;
	if(rgrp)*rgrp=0;
	tmp=BCCX_ParseTag(strm, &i);

	if(i==1)
	{
		while(1)
		{
			tmp1=BCCX_ParseExprR(strm, &i);
			if(i==2)break;

			if(!tmp1)return(NULL);

			BCCX_Add(tmp, tmp1);
		}

		if(rgrp)*rgrp=0;
		return(tmp);
	}

	if(i==2)
	{
		if(rgrp)*rgrp=2;
		return(NULL);
	}

	return(tmp);
}

BCCX_Node *BCCX_ParseExpr(char **strm)
	{ return(BCCX_ParseExprR(strm, NULL)); }

BCCX_Node *BCCX_ParseExprStr(char *str)
{
	char **strm, *s0;
	BCCX_Node *lst, *tmp;
	int na;

	s0=str;
	strm=&s0;

	lst=NULL;
	while(BCCX_PeekChar(strm)>0)
	{
		tmp=BCCX_ParseExpr(strm);
		if(!tmp)break;
		lst=BCCX_AddEnd(lst, tmp);
	}
	
	if(BCCX_TagIsP(lst, "$list"))
	{
		na=BCCX_GetNodeChildCount(lst);
		if(na==1)
		{
			tmp=BCCX_GetNodeIndex(lst, 0);
			return(tmp);
		}
	}
	
	return(lst);
}

