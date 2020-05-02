#include <bgbccc.h>

static void *bgbdys_getpos_buf(BCCX_ReadStream strm)
	{ return(strm->data); }
static void bgbdys_setpos_buf(BCCX_ReadStream strm, void *pos)
	{ strm->data=pos; }

static int bgbdys_peekchar_buf(BCCX_ReadStream strm)
{
	if(!(*(char **)strm->data))
		return(-1);
	return(BGBCP_PeekChar((char **)(&strm->data)));
}

static int bgbdys_readchar_buf(BCCX_ReadStream strm)
{
	if(!(*(char **)strm->data))
		return(-1);
	return(BGBCP_ParseChar((char **)(&strm->data)));
}

static int bgbdys_peekidx_buf(BCCX_ReadStream strm, int idx)
{
	char *s; int i;
	s=strm->data; i=idx;
	while(i--)
	{
		if(!*s)return(-1);
		BGBCP_ParseChar(&s);
	}
	if(!*s)return(-1);
	i=BGBCP_ParseChar(&s);
	return(i);
}

BCCX_ReadStream BCCX_NewReadStreamBuf(char *buf)
{
	BCCX_ReadStream strm;

	strm=bgbcc_tmalloc("_bccx_readstream_t",
		sizeof(struct BCCX_ReadStream_s));
	strm->data=buf;
	strm->get_pos_f=bgbdys_getpos_buf;
	strm->set_pos_f=bgbdys_setpos_buf;
	strm->peek_char_f=bgbdys_peekchar_buf;
	strm->read_char_f=bgbdys_readchar_buf;
	strm->peek_idx_f=bgbdys_peekidx_buf;

	return(strm);
}

BCCX_ReadPos BCCX_GetPos(BCCX_ReadStream strm)
	{ return(strm->get_pos_f(strm)); }
void BCCX_SetPos(BCCX_ReadStream strm, BCCX_ReadPos pos)
	{ strm->set_pos_f(strm, pos); }
int BCCX_PeekChar(BCCX_ReadStream strm)
	{ return(strm->peek_char_f(strm)); }
int BCCX_ReadChar(BCCX_ReadStream strm)
	{ return(strm->read_char_f(strm)); }
int BCCX_PeekCharIdx(BCCX_ReadStream strm, int idx)
	{ return(strm->peek_idx_f(strm, idx)); }


int BCCX_ParseEatWhite(BCCX_ReadStream strm)
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

int BCCX_ParseSpecialP(BCCX_ReadStream strm)
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

int BCCX_ParseContSpecialP(BCCX_ReadStream strm)
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

char *BCCX_ParseToken(BCCX_ReadStream strm, int *ty)
{
	char buf[16];
	char *t, *t2, *b;
	int i, j;

	b=bgbcc_ralloc(256); t=b;
	*b=0;

	if(ty)*ty=0;

	i=BCCX_ParseEatWhite(strm);
	if(i<0)return(NULL);

	if(BCCX_ParseSpecialP(strm))
	{
		if(ty)*ty=BGBDY_TTY_SPECIAL;
		*t++=BCCX_ReadChar(strm);
		*t=0;
	}else if(BCCX_PeekChar(strm)=='"') /* quoted string */
	{
		if(ty)*ty=BGBDY_TTY_STRING;
		BCCX_ReadChar(strm);
		while((BCCX_PeekChar(strm)>0) && (BCCX_PeekChar(strm)!='\"'))
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
			}else BGBCP_EmitChar(&t, BCCX_ReadChar(strm));
		}
		*t++=0;
		BCCX_ReadChar(strm);
	}else
	{	/* alphanumeric string that can contain '_' */
		if(ty)*ty=BGBDY_TTY_SYMBOL;

		while(BCCX_PeekChar(strm)>' ')
		{
			if(BCCX_ParseSpecialP(strm))break;
			BGBCP_EmitChar(&t, BCCX_ReadChar(strm));
		}
		*t++=0;
	}

	return(b);
}

char *BCCX_PeekToken(BCCX_ReadStream strm, int *ty)
{
	BCCX_ReadPos pos;
	char *b;

	pos=BCCX_GetPos(strm);
	b=BCCX_ParseToken(strm, ty);
	BCCX_SetPos(strm, pos);
	return(b);
}

char *BCCX_ParseText(BCCX_ReadStream strm)
{
	char *b, *t, *t1;
	char buf[16];
	int i, gws, rws;

	b=bgbcc_ralloc(4096);
	t=b; *t=0;

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
					*t++=' ';
					gws++;
				}
				continue;
			}
			gws=0;
			if(BCCX_PeekChar(strm)<=' ')rws++;
				else rws=0;
			*t++=BCCX_ReadChar(strm);
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

	return(b);
}

#if 0
// BCCX_Attr *BCCX_ParseOpts(BCCX_ReadStream strm)
{
	char *ns, *var, *eq, *val;
	int ty;
	BCCX_Attr *lst, *end, *tmp;

	lst=NULL;
	end=NULL;

	while(1)
	{
		var=BCCX_PeekToken(strm, &ty);
		if(ty==BGBDY_TTY_NULL)return(NULL);

		if((var[0]=='>') && (ty==BGBDY_TTY_SPECIAL)) break;
		if((var[0]=='/') && (ty==BGBDY_TTY_SPECIAL)) break;
		if((var[0]=='?') && (ty==BGBDY_TTY_SPECIAL)) break;

		if(ty==BGBDY_TTY_NULL) return(NULL);
		if(ty!=BGBDY_TTY_SYMBOL)
		{
			printf("parse error (inv attribute).\n");
			*(int *)-1=-1;
			return(NULL);
		}

		ns=NULL;

		var=BCCX_ParseToken(strm, &ty);
		if(ty==BGBDY_TTY_NULL) return(NULL);

		eq=BCCX_ParseToken(strm, &ty);
		if(ty==BGBDY_TTY_NULL) return(NULL);

		if((ty==BGBDY_TTY_SPECIAL) && (eq[0]==':'))
		{
			ns=var;

			var=BCCX_ParseToken(strm, &ty);
			if(ty==BGBDY_TTY_NULL) return(NULL);

			eq=BCCX_ParseToken(strm, &ty);
			if(ty==BGBDY_TTY_NULL) return(NULL);
		}

		if((ty!=BGBDY_TTY_SPECIAL) || (eq[0]!='='))
		{
			printf("parse error (attr equal).\n");
			*(int *)-1=-1;
			return(NULL);
		}

		val=BCCX_ParseToken(strm, &ty);
		if(ty==BGBDY_TTY_NULL) return(NULL);

		if(ty!=BGBDY_TTY_STRING)
		{
			printf("parse error (inv attribute arg).\n");
			*(int *)-1=-1;
			return(NULL);
		}

		tmp=bgbcc_tmalloc("_bccx_attr_t", sizeof(BCCX_Attr));
		tmp->next=NULL;
//		if(ns)tmp->ns=bgbcc_strdup(ns);
//		tmp->var=bgbcc_strdup(var);
		tmp->ivar=BCCX_StringToStridx(var);
		tmp->val=bgbcc_strdup(val);

		if(end)
		{
			end->next=tmp;
			end=tmp;
		}else
		{
			lst=tmp;
			end=tmp;
		}
	}

	return(lst);
}
#endif


#if 1
int BCCX_ParseAttr(BCCX_ReadStream strm, BCCX_Node *node)
{
	char *ns, *var, *eq, *val;
	int ty;

	while(1)
	{
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
			printf("parse error (inv attribute arg).\n");
			*(int *)-1=-1;
			return(-1);
		}

		BCCX_Set(node, var, val);
	}

	return(0);
}
#endif


BCCX_Node *BCCX_ParseTag(BCCX_ReadStream strm, int *rgrp)
{
	char *buf, *buf2, *key, *ns;
	int ty, i, j;
	char *s2;

	BCCX_Node *tmp, *t, *end;

	i=BCCX_ParseEatWhite(strm);
	if(i<0)return(NULL);

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

//		tmp=bgbcc_tmalloc("_bccx_node_t", sizeof(BCCX_Node));
		tmp=BCCX_AllocNode();
		tmp->next=NULL;
//		if(ns)tmp->ns=bgbcc_strdup(ns);
//		tmp->tag=bgbcc_strdup(key);
		tmp->itag=BCCX_StringToStridx(key);
//		tmp->attr=BCCX_ParseOpts(strm);
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

			buf=bgbcc_ralloc(64);
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

//	tmp=bgbcc_tmalloc("_bccx_node_t", sizeof(BCCX_Node));
	tmp=BCCX_AllocNode();
	BCCX_Set(tmp, "$text", s2);

	return(tmp);
}

BCCX_Node *BCCX_ParseExprR(BCCX_ReadStream strm, int *rgrp)
{
	BCCX_Node *tmp, *tmp1, *end;
	int i;

	if(*rgrp)*rgrp=0;
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

		if(*rgrp)*rgrp=0;
		return(tmp);
	}

	if(i==2)
	{
		if(*rgrp)*rgrp=2;
		return(NULL);
	}

	return(tmp);
}

BCCX_Node *BCCX_ParseExpr(BCCX_ReadStream strm)
	{ return(BCCX_ParseExprR(strm, NULL)); }

BCCX_Node *BCCX_ParseExprStr(char *str)
{
	BCCX_ReadStream strm;
	BCCX_Node *lst, *tmp;

	strm=BCCX_NewReadStreamBuf(str);

	lst=NULL;
	while(BCCX_PeekChar(strm)>0)
	{
		tmp=BCCX_ParseExpr(strm);
		if(!tmp)break;
		lst=BCCX_AddEnd(lst, tmp);
	}
	return(lst);
}

