#include <bgbccc.h>

#ifdef WIN32
#include <windows.h>
// #define CATCH_PP_SEH
#endif

char *bgbpp_ibuf;
char *bgbpp_obuf;
char *bgbpp_obufe;

BGBPP_Def *bgbpp_sdef;	//static defines
// BGBPP_Def *bgbpp/_def;

int bgbpp_warn, bgbpp_err;

int bgbpp_iflvl;	//nested ifdef-level (true branches)
int bgbpp_iflvl2;	//nested ifdef-level (false branch)
int bgbpp_ifmark;	//set if 'true' branch already taken
int bgbpp_ifflag[256];	//ifdef level flags

char *bgbpp_ifdname[256];	//ifdef level flags
char *bgbpp_ifdname2[256];	//ifdef level flags

char *bgbpp_spos;	//temp source position
char *bgbpp_lfn;	//last file name
int bgbpp_lln;		//last line number

char *bgbpp_inmacro;

char *bgbpp_cfn;	//current file name
int bgbpp_cln;	//current file name


//includes
char *bgbpp_sinc[256];	//static include paths
int bgbpp_nsinc=0;	//num static include paths

char *bgbpp_inc[256];	//include paths
int bgbpp_ninc;	//num include paths

int bgbpp_noinclude_p=0;	//_noinclude set

//char *(*bgbpp_loadfile_fp)(char *name, int *sz);

//char *bgbpp_loadfile(char *name, int *sz)
//{
//	if(bgbpp_loadfile_fp)
//		return(bgbpp_loadfile_fp(name, sz));
//	return(NULL);
//}

char *bgbpp_cache_incname[4096];
byte *bgbpp_cache_incbuf[4096];
int bgbpp_cache_incsz[4096];
int bgbpp_cache_ninc=0;

char *bgbpp_seen_incname[4096];
int bgbpp_seen_ninc=0;
byte bgbpp_seen_isnew=0;
byte bgbpp_seen_isabort=0;


BGBPP_Def *bgbpp_freedef=NULL;

int bgbpp_strcmp(char *s1, char *s2)
{
#if 1
	if(*s1 && (*s1==*s2))	{ s1++; s2++; }
	else					{ return(*s1-*s2); }
	if(*s1 && (*s1==*s2))	{ s1++; s2++; }
	else					{ return(*s1-*s2); }
	if(*s1 && (*s1==*s2))	{ s1++; s2++; }
	else					{ return(*s1-*s2); }
	if(*s1 && (*s1==*s2))	{ s1++; s2++; }
	else					{ return(*s1-*s2); }
#endif

	while(*s1 && (*s1==*s2))
		{ s1++; s2++; }
	return(*s1-*s2);
}

int BGBPP_CheckIncludeHashName(char *str)
{
	int h;
	char *s;
	
	s=str; h=0;
	while(*s)h=(h*251)+(*s++);
	h=h*251;
	return((h>>8)&4095);
}

int BGBPP_CheckIncludeHashNext(int hash)
{
	return(((hash*251)>>8)&4095);
}

int BGBPP_CheckIncludeCache(char *str, byte **rbuf, int *rsz)
{
	int hi, n;
	
	hi=BGBPP_CheckIncludeHashName(str); n=64;
	while(bgbpp_cache_incname[hi] && ((n--)>0))
	{
		if(!bgbpp_strcmp(bgbpp_cache_incname[hi], str))
		{
			if(rbuf)*rbuf=bgbpp_cache_incbuf[hi];
			if(rsz)*rsz=bgbpp_cache_incsz[hi];
			return(1);
		}
		hi=BGBPP_CheckIncludeHashNext(hi);
	}

	if(rbuf)*rbuf=NULL;
	if(rsz)*rsz=0;
	return(0);
}

int BGBPP_AddIncludeCache(char *str, byte *buf, int sz)
{
	int hi, n;
	
	hi=BGBPP_CheckIncludeHashName(str); n=64;
	while(bgbpp_cache_incname[hi] && ((n--)>0))
	{
		if(!bgbpp_strcmp(bgbpp_cache_incname[hi], str))
		{
			bgbpp_cache_incbuf[hi]=buf;
			bgbpp_cache_incsz[hi]=sz;
			return(1);
		}
		hi=BGBPP_CheckIncludeHashNext(hi);
	}
	
	if(n>0)
	{
		bgbpp_cache_incname[hi]=bgbcc_strdup(str);
		bgbpp_cache_incbuf[hi]=buf;
		bgbpp_cache_incsz[hi]=sz;
		return(1);
	}
	
	return(0);
}

int BGBPP_AddIncludePathFront(char *str)
{
	int i;

	if(!str)return(-1);

	for(i=0; i<bgbpp_nsinc; i++)
		if(!bgbpp_strcmp(bgbpp_sinc[i], str))
			return(i);
	i=bgbpp_nsinc++;
	bgbpp_sinc[i]=strdup(str);
	return(i);
}

int BGBPP_AddIncludePathBack(char *str)
{
	int i;

	if(!str)return(-1);

	for(i=0; i<bgbpp_nsinc; i++)
		if(!bgbpp_strcmp(bgbpp_sinc[i], str))
			return(i);

	for(i=bgbpp_nsinc; i>0; i++)
		bgbpp_sinc[i]=bgbpp_sinc[i-1];
	bgbpp_nsinc++;
	bgbpp_sinc[0]=strdup(str);
	return(i);
}


#if 0
// int BGBPP_ParseChar(char **str)
{
	unsigned char *s;
	int i;

	s=(unsigned char *)(*str);
	i=*s++;

	if(i>=0xF0)
	{
		i=(i&7)<<18;
		i+=((*s++)&63)<<12;
		i+=((*s++)&63)<<6;
		i+=(*s++)&63;
	}else if(i>=0xE0)
	{
		i=(i&15)<<12;
		i+=((*s++)&63)<<6;
		i+=(*s++)&63;
	}else if(i>=0xC0)
	{
		i=(i&31)<<6;
		i+=(*s++)&63;
	}else if(i>=0x80)
	{
//		continue;
	}

	*str=(char *)s;
	return(i);
}

// int BGBPP_EmitChar(char **str, int j)
{
	unsigned char *t;
	int i;

	t=(unsigned char *)(*str);

	if(j>0x3FFFFFF)
	{
		*t++=0xFC+(j>>30);
		*t++=0x80+((j>>24)&0x3F);
		*t++=0x80+((j>>18)&0x3F);
		*t++=0x80+((j>>12)&0x3F);
		*t++=0x80+((j>>6)&0x3F);
		*t++=0x80+(j&0x3F);
	}else if(j>0x1FFFFF)
	{
		*t++=0xF8+(j>>24);
		*t++=0x80+((j>>18)&0x3F);
		*t++=0x80+((j>>12)&0x3F);
		*t++=0x80+((j>>6)&0x3F);
		*t++=0x80+(j&0x3F);
	}else if(j>0xFFFF)
	{
		*t++=0xF0+(j>>18);
		*t++=0x80+((j>>12)&0x3F);
		*t++=0x80+((j>>6)&0x3F);
		*t++=0x80+(j&0x3F);
	}else if(j>0x7FF)
	{
		*t++=0xE0+(j>>12);
		*t++=0x80+((j>>6)&0x3F);
		*t++=0x80+(j&0x3F);
	}else if((j>0x7F) || !j)
	{
		*t++=0xC0+(j>>6);
		*t++=0x80+(j&0x3F);
	}else
	{
		if(!j) //Modified-UTF8
			{ *t++=0xC0; *t++=0x80; }
			else *t++=j;
	}

	*str=(char *)t;
	return(0);
}
#endif

char *BGBPP_EmitString(char *t, char *s)
{
	int i;

	if((*(t-1)=='\"') || (*(t-1)=='\''))
		*t++=' ';

	*t++='\"';
	while(*s)
	{
		i=BGBCP_ParseChar(&s);

		if((i<' ') || (i>'~'))
		{
			if(i>0xFF)
			{
				if(i>=0x10000)
				{
					sprintf(t, "\\U%08X", i); t+=10;
					continue;
				}
				sprintf(t, "\\u%04X", i); t+=6;
				continue;
			}

			switch(i)
			{
			case '\r': *t++='\\'; *t++='r'; break;
			case '\n': *t++='\\'; *t++='n'; break;
			case '\t': *t++='\\'; *t++='t'; break;
			case '\x1B': *t++='\\'; *t++='e'; break;
			default: sprintf(t, "\\x%02X", i); t+=4; break;
			}
			continue;
		}

		if(i=='\"')*t++='\\';
		if(i=='\'')*t++='\\';
		if(i=='\\')*t++='\\';
		BGBCP_EmitChar(&t, i);
//		*t++=i;
	}

	*t++='\"';
	*t=0;

	return(t);
}

char *BGBPP_EmitCharString(char *t, char *s)
{
	int i;

	if((*(t-1)=='\"') || (*(t-1)=='\''))
		*t++=' ';

	*t++='\'';
	while(*s)
	{
		i=BGBCP_ParseChar(&s);

		if((i<' ') || (i>='~'))
		{
			if(i>0xFF)
			{
				if(i>=0x10000)
				{
					sprintf(t, "\\U%08X", i); t+=10;
					continue;
				}
				sprintf(t, "\\u%04X", i); t+=6;
				continue;
			}

			switch(i)
			{
			case '\r': *t++='\\'; *t++='r'; break;
			case '\n': *t++='\\'; *t++='n'; break;
			case '\t': *t++='\\'; *t++='t'; break;
			case '\x1B': *t++='\\'; *t++='e'; break;
			default: sprintf(t, "\\x%02X", i); t+=4; break;
			}
			continue;
		}

		if(i=='\"')*t++='\\';
		if(i=='\'')*t++='\\';
		if(i=='\\')*t++='\\';
		BGBCP_EmitChar(&t, i);
//		*t++=i;
	}

	*t++='\'';
	*t=0;

	return(t);
}

void BGBPP_Trigraphs(BGBCP_ParseState *ctx, char *str)
{
	char *s, *t;

	//limit trigraphs to C/C++
	if((ctx->lang!=BGBCC_LANG_C) && (ctx->lang!=BGBCC_LANG_CPP))
		return;

	s=str; t=str;
	while(*s)
	{
		if((s[0]=='?') && (s[1]=='?'))
		{
			if(s[2]=='=') { *t++='#'; s+=3; continue; }
			if(s[2]==')') { *t++=']'; s+=3; continue; }
			if(s[2]=='!') { *t++='|'; s+=3; continue; }
			if(s[2]=='(') { *t++='['; s+=3; continue; }
			if(s[2]=='\'') { *t++='^'; s+=3; continue; }
			if(s[2]=='>') { *t++='}'; s+=3; continue; }
			if(s[2]=='/') { *t++='\\'; s+=3; continue; }
			if(s[2]=='<') { *t++='{'; s+=3; continue; }
			if(s[2]=='-') { *t++='~'; s+=3; continue; }
		}
		*t++=*s++;
		continue;
	}
	*t++=0;
}

void BGBPP_UniEscape(BGBCP_ParseState *ctx, char *str)
{
	char *s, *t;
	int i, j, k;

//	return;

	s=str; t=str;
	while(*s)
	{
		if((s[0]=='\\') && (s[1]=='\\'))
		{
			*t++=*s++;
			*t++=*s++;
			continue;
		}

		if((s[0]=='\\') && ((s[1]=='u') || (s[1]=='U')))
		{
			k=2;
			if(s[1]=='u')k=4;
			if(s[1]=='U')k=8;

			s+=2; j=0;
			while(k--)
			{
				if((*s>='0') && (*s<='9'))
					{ j<<=4; j+=(*s++)-'0'; continue; }
				if((*s>='A') && (*s<='F'))
					{ j<<=4; j+=(*s++)-'A'+10; continue; }
				if((*s>='a') && (*s<='f'))
					{ j<<=4; j+=(*s++)-'a'+10; continue; }
				break;
//				s++;
			}
			
			if(k>0)
			{
				s-=2;
				*t++=*s++;
				*t++=*s++;
				continue;
			}
			
			BGBCP_EmitChar(&t, j);
			continue;
		}
		
		*t++=*s++;
		continue;
	}
	*t++=0;
}

int BGBPP_Error(BGBCP_ParseState *ctx, char *str, ...)
{
	char tb[1024];
	va_list lst;
	int l;
	
	va_start(lst, str);
	vsprintf(tb, str, lst);
	va_end(lst);
	
	l=BGBCP_CalcLinenum(bgbpp_spos);
	printf("pp-error: %s:%d: %s\n", bgbpp_lfn, l, tb);
	bgbpp_err++;

	return(0);
}

BGBPP_Def *BGBPP_AllocDefine(BGBCP_ParseState *ctx)
{
	BGBPP_Def *tmp;
	
#if 1
	if(bgbpp_freedef)
	{
		tmp=bgbpp_freedef;
		bgbpp_freedef=tmp->next;
		
		memset(tmp, 0, sizeof(BGBPP_Def));
		return(tmp);
	}
#endif

	tmp=bgbcc_malloc(sizeof(BGBPP_Def));
	memset(tmp, 0, sizeof(BGBPP_Def));
	return(tmp);
}

void BGBPP_FreeDefine(BGBCP_ParseState *ctx, BGBPP_Def *def)
{
	if(!(def->name))
		{ BGBCC_DBGBREAK }
	def->name=NULL;

	def->next=bgbpp_freedef;
	bgbpp_freedef=def;
}

BGBPP_Def *BGBPP_LookupDefine(BGBCP_ParseState *ctx, char *name)
{
	char *args[64];
	char buf[4096];
	BGBPP_Def *cur;
	char *s;
	int i;

	s=name; i=0; while(*s)i=(i*251)+(*s++);
//	i&=255;
	i&=BGBCC_PPDEF_MASK;

	cur=ctx->ppdef[i];
	while(cur)
	{
		if(!bgbpp_strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}

	cur=bgbpp_sdef;
	while(cur)
	{
		if(!bgbpp_strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}

	if(!bgbpp_noinclude_p)
		return(NULL);

	s=BGBCP_LookupPPKey(ctx, name);
	if(s && !bgbpp_strcmp(s, "define"))
	{
		sprintf(buf, "%s:value", name);
		s=BGBCP_LookupPPKey(ctx, buf);
		if(!s)s="";
		BGBPP_AddDefine(ctx, name, s);

		cur=BGBPP_LookupDefine(ctx, name);
		return(cur);
	}

	if(s && !bgbpp_strcmp(s, "macro"))
	{
		sprintf(buf, "%s:value", name);
		s=BGBCP_LookupPPKey(ctx, buf);
		if(!s)s="";

		for(i=0; i<64; i++)
		{
			sprintf(buf, "%s:arg.%d", name, i);
			s=BGBCP_LookupPPKey(ctx, buf);
			if(!s)break;
			args[i]=bgbcc_strdup(s);
		}
		args[i]=NULL;

		BGBPP_AddDefineArgs(ctx, name, args, s);

		cur=BGBPP_LookupDefine(ctx, name);
		return(cur);
	}

	return(NULL);
}

BGBPP_Def *BGBPP_LookupDefine2(BGBCP_ParseState *ctx, char *name)
{
	char *args[64];
	char buf[4096];
	BGBPP_Def *cur;
	char *s;
	int i;

	s=name; i=0; while(*s)i=(i*251)+(*s++);
//	i&=255;
	i&=BGBCC_PPDEF_MASK;

	cur=ctx->ppdef[i];
	while(cur)
	{
		if(!bgbpp_strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}

	return(NULL);
}

void BGBPP_DeleteDefine(BGBCP_ParseState *ctx, char *name)
{
	BGBPP_Def *cur, *prv;
	char *s;
	int i;

	s=name; i=0; while(*s)i=(i*251)+(*s++);
//	i&=255;
	i&=BGBCC_PPDEF_MASK;

	cur=ctx->ppdef[i]; prv=NULL;
	while(cur)
	{
		if(!bgbpp_strcmp(cur->name, name))
		{
			if(prv)
			{
				prv->next=cur->next;
			}else
			{
				ctx->ppdef[i]=cur->next;
			}
			BGBPP_FreeDefine(ctx, cur);
//			if(cur->args)bgbcc_free(cur->args);
//			bgbcc_free(cur->value);
//			bgbcc_free(cur);
			return;
		}
		prv=cur; cur=cur->next;
	}
}

void BGBPP_CleanupDefines(BGBCP_ParseState *ctx)
{
	BGBPP_Def *cur, *nxt;
	int i;

#if 1
//	for(i=0; i<256; i++)
	for(i=0; i<BGBCC_PPDEF_SIZE; i++)
	{
		cur=ctx->ppdef[i];
		ctx->ppdef[i]=NULL;

		while(cur)
		{
			nxt=cur->next;
			BGBPP_FreeDefine(ctx, cur);
			cur=nxt; 
		}
	}
#endif

#if 0
	BGBPP_Def *cur, *nxt;

	cur=ctx->ppdef;
	ctx->ppdef=NULL;

	while(cur)
	{
		nxt=cur->next;
		if(cur->args)bgbcc_free(cur->args);
		bgbcc_free(cur->value);
		bgbcc_free(cur);
		cur=nxt; 
	}
#endif
}

void BGBPP_AddDefine(BGBCP_ParseState *ctx, char *name, char *str)
{
	BGBPP_Def *cur;
	char *s;
	int i;

	cur=BGBPP_LookupDefine2(ctx, name);
//	cur=BGBPP_LookupDefine3(name);
	if(cur && !cur->args)
	{
//		free(cur->value);
		cur->value=bgbcc_strdup(str);
		return;
	}

	s=name; i=0;
	while(*s)i=(i*251)+(*s++);
//	i&=255;
	i&=BGBCC_PPDEF_MASK;

//	cur=bgbcc_malloc(sizeof(BGBPP_Def));
//	memset(cur, 0, sizeof(BGBPP_Def));
	cur=BGBPP_AllocDefine(ctx);

	cur->name=bgbcc_strdup(name);
	cur->value=bgbcc_strdup(str);

	cur->next=ctx->ppdef[i];
	ctx->ppdef[i]=cur;
}

void BGBPP_AddDefineArgs(BGBCP_ParseState *ctx,
	char *name, char **args, char *str)
{
	BGBPP_Def *cur;
	char *s;
	int i, hi;

	s=name; hi=0; while(*s)hi=(hi*251)+(*s++);
//	hi&=255;
	hi&=BGBCC_PPDEF_MASK;

//	cur=bgbcc_malloc(sizeof(BGBPP_Def));
//	memset(cur, 0, sizeof(BGBPP_Def));
	cur=BGBPP_AllocDefine(ctx);

	cur->name=bgbcc_strdup(name);
	cur->value=bgbcc_strdup(str);

	for(i=0; args[i]; i++);
//	cur->args=bgbcc_malloc((i+1)*sizeof(char *));

	if(i>=8)
		{ cur->args=bgbcc_malloc((i+1)*sizeof(char *)); }
	else
		{ cur->args=cur->t_args; }

	for(i=0; args[i]; i++)cur->args[i]=bgbcc_strdup(args[i]);
	cur->args[i]=NULL;

	cur->next=ctx->ppdef[hi];
	ctx->ppdef[hi]=cur;
}

void BGBPP_AppendDefineLine(BGBCP_ParseState *ctx,
	char *name, char *str)
{
//	char tb[1024];
	char tb[4096];
	BGBPP_Def *cur;

	cur=BGBPP_LookupDefine2(ctx, name);
	if(!cur)return;

	if(!(*cur->value))
	{
//		free(cur->value);
		cur->value=bgbcc_strdup(str);
		return;
	}

	sprintf(tb, "%s\n%s", cur->value, str);
//	free(cur->value);
	cur->value=bgbcc_strdup(tb);
	return;
}


void BGBPP_AddStaticDefine(BGBCP_ParseState *ctx,
	char *name, char *str)
{
	BGBPP_Def *cur;

	cur=bgbcc_malloc2(sizeof(BGBPP_Def));
	memset(cur, 0, sizeof(BGBPP_Def));

	cur->name=bgbcc_strdup(name);
	cur->value=bgbcc_strdup(str);

	cur->next=bgbpp_sdef;
	bgbpp_sdef=cur;
}

void BGBPP_AddStaticDefineArgs(BGBCP_ParseState *ctx,
	char *name, char **args, char *str)
{
	BGBPP_Def *cur;
	int i;

	cur=bgbcc_malloc2(sizeof(BGBPP_Def));
//	cur=malloc(sizeof(BGBPP_Def));
	memset(cur, 0, sizeof(BGBPP_Def));

	cur->name=bgbcc_strdup(name);
	cur->value=bgbcc_strdup(str);

	for(i=0; args[i]; i++);
//	cur->args=bgbcc_malloc2((i+1)*sizeof(char *));
	if(i>=8)
		{ cur->args=bgbcc_malloc2((i+1)*sizeof(char *)); }
	else
		{ cur->args=cur->t_args; }

	for(i=0; args[i]; i++)
		cur->args[i]=bgbcc_strdup(args[i]);
	cur->args[i]=NULL;

	cur->next=bgbpp_sdef;
	bgbpp_sdef=cur;
}

void BGBPP_SendDefines(BGBCP_ParseState *ctx)
{
	char buf[4096];
	BGBPP_Def *cur;
	char *s;
	int i, j;

	if(!BGBCC_BindIsMetaP(ctx))
		return;

//	for(i=0; i<256; i++)
	for(i=0; i<BGBCC_PPDEF_SIZE; i++)
	{
		cur=ctx->ppdef[i];
		while(cur)
		{
//			if(!bgbpp_strcmp(cur->name, name))
//				return(cur);

			if(cur->args)
			{
				BGBCC_BindPPKey(ctx, cur->name, "macro");
				sprintf(buf, "%s:value", cur->name);
				BGBCC_BindPPKey(ctx, buf, cur->value);

				for(j=0; cur->args[j]; j++)
				{
					sprintf(buf, "%s:arg.%d",
						cur->name, j);
					BGBCC_BindPPKey(ctx,
						buf, cur->args[j]);
				}

				sprintf(buf, "%s:arg.%d", cur->name, j);
				BGBCC_BindPPKey(ctx, buf, NULL);
			}else
			{
				BGBCC_BindPPKey(ctx, cur->name, "define");
				sprintf(buf, "%s:value", cur->name);
				BGBCC_BindPPKey(ctx, buf, cur->value);
			}

			cur=cur->next;
		}
	}
}

char *BGBPP_LoadInclude(BGBCP_ParseState *ctx, char *name, int *rsz)
{
	char b[4096];
	char *buf, *buf1, *s, *t;
	int i, sz;

	if(BGBPP_CheckIncludeCache(name, (byte **)(&buf), &sz)>0)
	{
		if(rsz)*rsz=sz;
		return(buf);
	}	

	buf=NULL;
	for(i=(bgbpp_ninc-1); i>=0; i--)
	{
		sprintf(b, "%s/%s", bgbpp_inc[i], name);
//		buf=bgbpp_loadfile(b, &sz);
//		buf=bgbcc_loadfile(b, &sz);
		buf=bgbcc_loadfile_txt(b, &sz);
//		if(buf) { if(rsz)*rsz=sz; return(buf); }
		if(buf)break;
	}

	if(!buf && bgbpp_cfn)
	{
		strcpy(b, bgbpp_cfn);
		t=b+strlen(b);
		while((t>b) && (*t!='/') && (*t!='\\'))
			t--;
		if((t>b) && ((*t=='/') || (*t=='\\')))
		{
			strcpy(t+1, name);
//			buf=bgbcc_loadfile(b, &sz);
			buf=bgbcc_loadfile_txt(b, &sz);
		}
	}

	if(!buf)
	{
		for(i=(bgbpp_nsinc-1); i>=0; i--)
		{
			sprintf(b, "%s/%s", bgbpp_sinc[i], name);
//			buf=bgbcc_loadfile(b, &sz);
			buf=bgbcc_loadfile_txt(b, &sz);
//			if(buf) { if(rsz)*rsz=sz; return(buf); }
			if(buf)break;
		}
	}
	
	if(buf)
	{
//		printf("Loaded Include: '%s' %d bytes\n", name, sz);
		BGBPP_AddIncludeCache(name, (byte *)buf, sz);
		if(rsz)*rsz=sz;
		return(buf);
	}

	return(NULL);
}

int BGBPP_FetchSourceLine(char *lfn, int lln, char *dstbuf)
{
	static char *c_lbuf, *c_lbufe;
	static char *c_lfn;
	static char *c_chpos;
	static int c_chln;
	char *tbuf, *tbufe;
	char *cs, *ct;
	int i, tsz, n;
	
	if(!c_lfn || strcmp(c_lfn, lfn))
	{
		tsz=0;
		tbuf=BGBPP_LoadInclude(NULL, lfn, &tsz);
		tbufe=tbuf+tsz;
		c_lbuf=tbuf;
		c_lbufe=tbufe;
		c_lfn=bgbcc_strdup(lfn);
		c_chpos=tbuf;
		c_chln=1;
	}else
	{
		tbuf=c_lbuf;
		tbufe=c_lbufe;
		
		if(lln<c_chln)
		{
			c_chpos=tbuf;
			c_chln=1;
		}
	}

	if(!tbuf)
	{
		strcpy(dstbuf, "");
		return(-1);
	}
	
	n=c_chln; cs=c_chpos;
	while((cs<tbufe) && (n<lln))
	{
		i=*cs++;
		if(i=='\n')
			n++;
	}

	c_chpos=cs;
	c_chln=n;
	
	if(n==lln)
	{
		ct=dstbuf;

		while((cs<tbufe) && *cs && (*cs<=' '))
			cs++;
		while(cs<tbufe)
		{
			i=*cs++;
			if((i=='\r') || (i=='\n'))
				break;
			*ct++=i;				
		}
		*ct=0;
		return(1);
	}
	
	strcpy(dstbuf, "");
	return(0);
}

void BGBPP_Include(BGBCP_ParseState *ctx, char *str, int angle)
{
	char b[4096];
	char *oldfn;
	char *buf;
	char *s;
	int oisnew;
	int l0, l1, l2, l3;
	int i;

	oldfn=bgbpp_cfn;
	buf=NULL;

//	if(bgbpp_seen_ninc>0)
	if(bgbpp_cfn && !angle)
	{
//		i=bgbpp_seen_ninc-1;
//		strcpy(b, bgbpp_seen_incname[i]);
		strcpy(b, bgbpp_cfn);
		s=b+strlen(b);
		while(s>b)
		{
			if(*s=='/')
				break;
			s--;
		}
		
		if(*s=='/')
		{
			s++;
			strcpy(s, str);

			buf=BGBPP_LoadInclude(ctx, b, NULL);
			if(buf)
				{ bgbpp_cfn=bgbcc_strdup(b); }
		}
	}

#if 0
	buf=NULL;
	for(i=(bgbpp_ninc-1); i>=0; i--)
	{
		sprintf(b, "%s/%s", bgbpp_inc[i], str);
		buf=bgbcc_loadfile(b, NULL);
		if(buf)break;
	}
#endif

	if(!buf)
	{
		buf=BGBPP_LoadInclude(ctx, str, NULL);
		if(buf)
			{ bgbpp_cfn=bgbcc_strdup(str); }
	}

	if(!buf)
	{
//		printf("BGBPP_Include: fail open include"
//			" '%s'\n", str);
//		bgbpp_err++;
		BGBPP_Error(ctx, "#include fail to load '%s'\n", str);
		return;
	}

//	printf("Include: '%s' %d bytes\n", str, strlen(buf));

	for(i=0; i<bgbpp_seen_ninc; i++)
	{
		if(!bgbpp_strcmp(bgbpp_seen_incname[i], str))
			break;
	}
	
	oisnew=bgbpp_seen_isnew;
	if(i>=bgbpp_seen_ninc)
	{
		i=bgbpp_seen_ninc++;
		bgbpp_seen_incname[i]=bgbcc_strdup(str);
		bgbpp_seen_isnew=1;
		bgbpp_seen_isabort=0;
	}else
	{
		bgbpp_seen_isnew=0;
		bgbpp_seen_isabort=0;
	}


	l0=bgbpp_iflvl;
	l1=bgbpp_iflvl2;

	BGBCP_PushLinenum();
	BGBCP_SetLinenum(str, buf, 1);

	BGBPP_Buffer(ctx, buf);
//	free(buf);

	BGBCP_PopLinenum();

	bgbpp_cfn=oldfn;

	bgbpp_seen_isnew=oisnew;
	bgbpp_seen_isabort=0;

	l2=bgbpp_iflvl;
	l3=bgbpp_iflvl2;
	if((l0!=l2) || (l1!=l3))
	{
		printf("BGBPP_Include: PP Mismatched Nesting Levels"
			" '%s': %d->%d, %d->%d\n", str, l0, l2, l1, l3);

		for(i=l0-2; i<l2+2; i++)
		{
			if(i<0)continue;
			s=bgbpp_ifdname[i];
			if(!s)s="--";
			printf("    %d: %s\n", i, s);
		}
		for(i=l1-2; i<l3+2; i++)
		{
			if(i<0)continue;
			s=bgbpp_ifdname2[i];
			if(!s)s="--";
			printf("    %d: %s\n", i, s);
		}

		bgbpp_iflvl=l0;
		bgbpp_iflvl2=l1;
	}
}

void BGBPP_Directive2(BGBCP_ParseState *ctx, char *str)
{
	char b[256], b2[4096];
	int ty, ty2;
	char *s, *t, *s1;
	BCCX_Node *n0;
	int i;

	s=str;
	s=BGBCP_EatWhiteOnly(s);
	s++;

	s=BGBCP_TokenCtx(ctx, s, b, &ty);
	BGBCP_TokenCtx(ctx, s, b2, &ty2);

	if(b[0]=='i')
	{
	//	if(!bgbpp_strcmp(b, "if"))
		if(!bgbcp_strcmp2(b, "if"))
		{
			bgbpp_ifdname2[bgbpp_iflvl2]=bgbcc_strdup(str);
			bgbpp_iflvl2++;
	//		printf("%s: %d %d\n", str, bgbpp_iflvl, bgbpp_iflvl2);
			return;
		}

	//	if(!bgbpp_strcmp(b, "ifdef"))
		if(!bgbcp_strcmp5(b, "ifdef"))
		{
			bgbpp_ifdname2[bgbpp_iflvl2]=bgbcc_strdup(b2);
			bgbpp_iflvl2++;
	//		printf("%s: %d %d\n", str, bgbpp_iflvl, bgbpp_iflvl2);
			return;
		}

	//	if(!bgbpp_strcmp(b, "ifndef"))
		if(!bgbcp_strcmp6(b, "ifndef"))
		{
			bgbpp_ifdname2[bgbpp_iflvl2]=bgbcc_strdup(b2);
			bgbpp_iflvl2++;
	//		printf("%s: %d %d\n", str, bgbpp_iflvl, bgbpp_iflvl2);
			return;
		}
	}

	if(b[0]=='e')
	{
	//	if(!bgbpp_strcmp(b, "endif"))
		if(!bgbcp_strcmp5(b, "endif"))
		{
			bgbpp_iflvl2--;
	//		printf("%s: %d %d\n", str, bgbpp_iflvl, bgbpp_iflvl2);
			return;
		}

	//	if(!bgbpp_strcmp(b, "else"))
		if(!bgbcp_strcmp4(b, "else"))
		{
	//		if(bgbpp_iflvl2==1)
			if((bgbpp_iflvl2==1) && !bgbpp_ifmark)
			{
				bgbpp_ifdname[bgbpp_iflvl]=
					bgbpp_ifdname2[bgbpp_iflvl2-1];
				bgbpp_iflvl2--;
				bgbpp_iflvl++;
	//			printf("%s: %d %d\n", str, bgbpp_iflvl, bgbpp_iflvl2);
				return;
			}
	//		printf("%s: %d %d\n", str, bgbpp_iflvl, bgbpp_iflvl2);
			return;
		}

	//	if(!bgbpp_strcmp(b, "elif"))
		if(!bgbcp_strcmp4(b, "elif"))
		{
	//		if(bgbpp_iflvl2==1)
			if((bgbpp_iflvl2==1) && !bgbpp_ifmark)
			{
				strcpy(b2, s);
				i=1; while(i)i=BGBPP_Line(ctx, b2);

	//			printf("pp-expr: %s\n", b2);

				s1=b2;
				n0=BGBCP_Expression(ctx, &s1);
				n0=BGBCP_ReduceExpr(ctx, n0);
				i=BGBCP_BoolExpr(ctx, n0);

				if(i>0)
				{
					bgbpp_ifdname[bgbpp_iflvl]=
						bgbpp_ifdname2[bgbpp_iflvl2-1];
					bgbpp_iflvl2--;
					bgbpp_iflvl++;

					if(!bgbpp_iflvl2)bgbpp_ifmark=1;
	//				printf("%s (T): %d %d\n", str, bgbpp_iflvl, bgbpp_iflvl2);
					return;
				}else if(i<0)
				{
					BGBPP_Error(ctx, "#elif pp-expr couldn't be evaluated\n");
					//error
				}

	//			printf("%s (F): %d %d\n", str, bgbpp_iflvl, bgbpp_iflvl2);
				return;
			}

			return;
		}
	}
}


int BGBPP_ExpandBinary(char *larg, char *op, char *rarg, char *dst)
{
	float f, g, h;
	int i, j, k;

	f=atof(larg); g=atof(rarg);
	i=0;	h=0;

	if(!strcmp(op, "+")) { h=f+g; i=1; }
	if(!strcmp(op, "-")) { h=f-g; i=1; }
	if(!strcmp(op, "*")) { h=f*g; i=1; }

	if(!strcmp(op, "/"))
	{
		if(g==0)return(-1);
		h=f/g; i=1;
	}

	if(i)
	{
		if(((int)h)==h)
		{
			sprintf(dst, "%d", (int)h);
			return(0);
		}
		sprintf(dst, "%f", h);
		return(0);
	}

	i=f; j=g;
	if(i!=f)return(-1);
	if(j!=g)return(-1);

	if(!strcmp(op, "&"))
		{ sprintf(dst, "%d", i&j); return(0); }
	if(!strcmp(op, "|"))
		{ sprintf(dst, "%d", i|j); return(0); }
	if(!strcmp(op, "^"))
		{ sprintf(dst, "%d", i^j); return(0); }
	if(!strcmp(op, "&&"))
		{ sprintf(dst, "%d", i&&j); return(0); }
	if(!strcmp(op, "||"))
		{ sprintf(dst, "%d", i||j); return(0); }

	return(-1);
}

int BGBPP_LineArith(BGBCP_ParseState *ctx, char *str)
{
	char lbuf[4096];
	char b[256], b2[256], b3[256];
	char *s, *s1, *s2, *t, *t1, *te;
	int i, ty, ty2, ty3, ni;

	ni=0;
	s=str; t=lbuf; te=lbuf+2040;
	while(*s)
	{
		s=BGBCP_TokenCtx(ctx, s, b, &ty);
		s1=BGBCP_TokenCtx(ctx, s, b2, &ty2);
		s2=BGBCP_TokenCtx(ctx, s1, b3, &ty3);

#if 1
		if((ty==BTK_NUMBER) &&
			(ty2==BTK_OPERATOR) &&
			(ty3==BTK_NUMBER))
		{
			i=BGBPP_ExpandBinary(b, b2, b3, b2);
			if(i>=0)
			{
				s=s2; t=BGBPP_PrintName(t, b2);
				ni++;
				continue;
			}

			t=BGBPP_PrintName(t, b);
			continue;
		}
		if(!strcmp(b, "(") && !strcmp(b3, ")") &&
			(ty2==BTK_NUMBER))
		{
			t=BGBPP_PrintName(t, b2);
			ni++;
			continue;
		}
#endif

		if(ty==BTK_NAME)
		{
			i=BGBPP_Expand(ctx, &s, &t, b, te);
			if(i>0)
			{
				ni++;
				continue;
			}

			t=BGBPP_PrintName(t, b);
			continue;
		}

		if(ty==BTK_NUMBER)
		{
			t=BGBPP_PrintName(t, b);
			continue;
		}

		if(ty==BTK_STRING)
		{
			t=BGBPP_EmitString(t, b);
			continue;
		}

		if(ty==BTK_CHARSTRING)
		{
			t=BGBPP_EmitCharString(t, b);
			continue;
		}

		sprintf(t, "%s", b);
		t+=strlen(t);
	}
	*t=0;

	strcpy(str, lbuf);
	return(ni);
}


void BGBPP_Directive(BGBCP_ParseState *ctx, char *str)
{
//	char b[256], b2[4096], b3[256];
	char b[4096], b2[4096], b3[4096], b4[256];
	char *ab[256];
	BGBPP_Def *def;
	BCCX_Node *exp;
	char *s, *t, *s1, *s2, *tn;
	int ty, ty2, ty3, ty4;
	int i, j, n;

#if 0
	if(strlen(str)>=2048)
	{
		printf("PP ?: %s\n", str);
		return;
	}
#endif

	if(bgbpp_iflvl2)
	{
		BGBPP_Directive2(ctx, str);
		return;
	}

	s=str;
//	while(*s && (*s<=' '))s++;
	s=BGBCP_EatWhiteOnly(s);

	s++;

	s=BGBCP_TokenCtx(ctx, s, b, &ty);
	s1=BGBCP_TokenCtx(ctx, s, b2, &ty2);

	if(b[0]=='a')
	{
		if(!strcmp(b, "assign"))
		{
			strcpy(b2, s1);
			i=1; while(i)i=BGBPP_Line(ctx, b2);
			i=1; while(i)i=BGBPP_LineArith(ctx, b2);
			s2=b2; while(*s2 && (*s2<=' '))s2++;
			BGBPP_AddDefine(ctx, b2, s2);
			return;
		}

#if 0
		if(!strcmp(b, "assignlocal"))
		{
			strcpy(b2, s1);
			i=1; while(i)i=BGBPP_Line(ctx, b2);
			i=1; while(i)i=BGBPP_LineArith(ctx, b2);
			s2=b2; while(*s2 && (*s2<=' '))s2++;
			BGBPP_AddLocalDefine(b2, s2);
			return;
		}
#endif
	}

	if(b[0]=='d')
	{
	//	if(!bgbpp_strcmp(b, "define"))
		if(!bgbcp_strcmp6(b, "define"))
		{
			if(*s1!='(')
			{
				if(!bgbpp_strcmp(b2, "_noinclude"))
					{ bgbpp_noinclude_p=1; return; }

				while(*s1 && (*s1<=' '))s1++;
				BGBPP_AddDefine(ctx, b2, s1);
				return;
			}

			tn=bgbcc_strdup(b2);

			s1++;
			n=0;
			while(1)
			{
				s1=BGBCP_TokenCtx(ctx, s1, b, &ty);
	//			if(!bgbpp_strcmp(b, ")"))break;
				if(!bgbcp_strcmp1(b, ")"))break;
	//			if(!bgbpp_strcmp(b, ","))continue;
				if(!bgbcp_strcmp1(b, ","))continue;

				ab[n++]=bgbcc_strdup(b);
			}
			ab[n]=NULL;

			while(*s1 && (*s1<=' '))s1++;

			BGBPP_AddDefineArgs(ctx, tn, ab, s1);
			return;
		}

		if(!bgbpp_strcmp(b, "define_arch"))
		{
			if(*s1!='(')
			{
				while(*s1 && (*s1<=' '))s1++;
				if(*s1)
				{
					sprintf(b3, "#ARCH:%s", s1);
				}else
				{
					sprintf(b3, "#ARCH:%s", b2);
				}
				BGBPP_AddDefine(ctx, b2, b3);
				return;
			}

			return;
		}

		if(!bgbcp_strcmp6(b, "defmacro"))
		{
			if(*s1!='(')
			{
				if(!bgbpp_strcmp(b2, "_noinclude"))
					{ bgbpp_noinclude_p=1; return; }

				tn=bgbcc_strdup(b2);

				while(*s1 && (*s1<=' '))s1++;
				BGBPP_AddDefine(ctx, tn, s1);
				bgbpp_inmacro=tn;
				return;
			}

			tn=bgbcc_strdup(b2);

			s1++;
			n=0;
			while(1)
			{
				s1=BGBCP_TokenCtx(ctx, s1, b, &ty);
	//			if(!bgbpp_strcmp(b, ")"))break;
				if(!bgbcp_strcmp1(b, ")"))break;
	//			if(!bgbpp_strcmp(b, ","))continue;
				if(!bgbcp_strcmp1(b, ","))continue;

				ab[n++]=bgbcc_strdup(b);
			}
			ab[n]=NULL;

			while(*s1 && (*s1<=' '))s1++;

			BGBPP_AddDefineArgs(ctx, tn, ab, s1);
			bgbpp_inmacro=tn;
			return;
		}
	}

	if(b[0]=='e')
	{
	//	if(!bgbpp_strcmp(b, "error"))
		if(!bgbcp_strcmp5(b, "error"))
		{
			s=BGBCP_EatWhiteOnly(s);
			printf("BGBPP_Directive: %s:%d: #error: %s\n",
				BGBCP_GetFilename(),
				BGBCP_CalcLinenum(bgbpp_spos), s);
			bgbpp_err++;

			return;
		}

	//	if(!bgbpp_strcmp(b, "endif"))
		if(!bgbcp_strcmp5(b, "endif"))
		{
			if(bgbpp_ifflag[bgbpp_iflvl]&1)
			{
				sprintf(bgbpp_obuf, "/*%d*/ }\n",
					BGBCP_CalcLinenum(bgbpp_spos));
			}
			bgbpp_iflvl--;
	//		printf("%s: %d %d\n", str, bgbpp_iflvl, bgbpp_iflvl2);
			return;
		}

	//	if(!bgbpp_strcmp(b, "else"))
		if(!bgbcp_strcmp4(b, "else"))
		{
			if(bgbpp_ifflag[bgbpp_iflvl]&1)
			{
				sprintf(bgbpp_obuf, "/*%d*/ }else{\n",
					BGBCP_CalcLinenum(bgbpp_spos));
				return;
			}

			bgbpp_ifdname2[bgbpp_iflvl2]=
				bgbpp_ifdname[bgbpp_iflvl-1];
			bgbpp_iflvl--;
			bgbpp_iflvl2++;
	//		printf("%s: %d %d\n", str, bgbpp_iflvl, bgbpp_iflvl2);
			return;
		}

	//	if(!bgbpp_strcmp(b, "elif"))
		if(!bgbcp_strcmp4(b, "elif"))
		{
	#if 0
			strcpy(b2, s);
			i=1; while(i)i=BGBPP_Line(ctx, b2);

			s1=b2;
			exp=BGBCP_Expression(ctx, &s1);
			exp=BGBCP_ReduceExpr(ctx, exp);
			i=BGBCP_BoolExpr(ctx, exp);

			if(i==0)
			{
				bgbpp_iflvl--;
				bgbpp_iflvl2++;
				return;
			}
	#endif

			bgbpp_ifdname2[bgbpp_iflvl2]=
				bgbpp_ifdname[bgbpp_iflvl-1];
			bgbpp_iflvl--;
			bgbpp_iflvl2++;
	//		printf("%s: %d %d\n", str, bgbpp_iflvl, bgbpp_iflvl2);
			return;
		}

		if(!strcmp(b, "endmacro"))
		{
			if(!strcmp(bgbpp_inmacro, "_MACROED_"))
			{
				def=BGBPP_LookupDefine(ctx, bgbpp_inmacro);
				BGBPP_InterpMacroed(ctx, def->value);
				BGBPP_DeleteDefine(ctx, bgbpp_inmacro);
			}
		
			bgbpp_inmacro=NULL;
			return;
		}
	}

	if(b[0]=='i')
	{

	//	if(!bgbpp_strcmp(b, "include"))
		if(!bgbcp_strcmp7(b, "include"))
		{
			i=1;
//			strcpy(b, s);
			strncpy(b, s, 4000);
	//		while(i)i=BGBPP_Line(ctx, b);

			s1=BGBCP_TokenCtx(ctx, b, b2, &ty2);

	//		if(!bgbpp_strcmp(b2, "<"))
			if(!bgbcp_strcmp1(b2, "<"))
			{
	//			BGBCP_TokenCtx(ctx, s1, b2, &ty2);

				t=b2;
				while(*s1 && (*s1!='>'))*t++=*s1++;
				*t++=0;

				BGBPP_Include(ctx, b2, 1);
				return;
			}

#if 0
			t=b2; t+=strlen(t);
			while(*s1)
			{
				s1=BGBCP_TokenCtx(ctx, s1, t, &ty2);
				t+=strlen(t);
			}
			*t=0;
#endif

			BGBPP_Include(ctx, b2, 0);
			return;
		}

	//	if(!bgbpp_strcmp(b, "ifdef"))
		if(!bgbcp_strcmp5(b, "ifdef"))
		{
			bgbpp_ifmark=0;
			bgbpp_ifflag[bgbpp_iflvl]=0;
			def=BGBPP_LookupDefine(ctx, b2);
			if(!def)
			{
				bgbpp_ifdname2[bgbpp_iflvl2]=bgbcc_strdup(b2);
				bgbpp_iflvl2++;
			}else
			{
				if(def->value && !strncmp(def->value, "#ARCH:", 6))
				{
					bgbpp_ifflag[bgbpp_iflvl]|=1;
					sprintf(bgbpp_obuf, "/*%d*/ __ifdef(%s) {\n",
						BGBCP_CalcLinenum(bgbpp_spos), def->value+6);
				}
				bgbpp_ifdname[bgbpp_iflvl]=bgbcc_strdup(b2);
				bgbpp_iflvl++;
			}
			if(!bgbpp_iflvl2)bgbpp_ifmark=1;
	//		printf("%s: %d %d\n", str, bgbpp_iflvl, bgbpp_iflvl2);
			return;
		}

	//	if(!bgbpp_strcmp(b, "ifndef"))
		if(!bgbcp_strcmp6(b, "ifndef"))
		{
			bgbpp_ifmark=0;
			bgbpp_ifflag[bgbpp_iflvl]=0;
			def=BGBPP_LookupDefine(ctx, b2);
			if(!def)
			{
				bgbpp_ifdname[bgbpp_iflvl]=bgbcc_strdup(b2);
				bgbpp_iflvl++;
			}else
			{
				if(def->value && !strncmp(def->value, "#ARCH:", 6))
				{
					bgbpp_ifflag[bgbpp_iflvl]|=1;
					sprintf(bgbpp_obuf, "/*%d*/ __ifdef(%s) {\n",
						BGBCP_CalcLinenum(bgbpp_spos), def->value+6);
					bgbpp_ifdname[bgbpp_iflvl]=bgbcc_strdup(b2);
					bgbpp_iflvl++;
				}else
				{
					bgbpp_ifdname2[bgbpp_iflvl2]=bgbcc_strdup(b2);
					bgbpp_iflvl2++;
				}
			}
			if(!bgbpp_iflvl2)bgbpp_ifmark=1;
	//		printf("%s: %d %d\n", str, bgbpp_iflvl, bgbpp_iflvl2);
			return;
		}

	//	if(!bgbpp_strcmp(b, "if"))
		if(!bgbcp_strcmp2(b, "if"))
		{
			i=1;
			strcpy(b2, s);
			while(i)i=BGBPP_Line(ctx, b2);

			s2=b2;
			exp=BGBCP_Expression(ctx, &s2);
			exp=BGBCP_ReduceExpr(ctx, exp);
			i=BGBCP_BoolExpr(ctx, exp);

			if(i<0)
			{
				BGBPP_Line(ctx, b2);
				i=0;
			}

			bgbpp_ifmark=0;
			if(i>0)
			{
				bgbpp_ifdname[bgbpp_iflvl]=bgbcc_strdup(b2);
				bgbpp_iflvl++;
			} else {
				bgbpp_ifdname2[bgbpp_iflvl2]=bgbcc_strdup(b2);
				bgbpp_iflvl2++;
			}
			if(!bgbpp_iflvl2)bgbpp_ifmark=1;

			if(i<0)
			{
				BGBPP_Error(ctx, "#if pp-expr couldn't be evaluated\n");
#ifndef BGBCC_BCCX2
				BCCX_Print(exp);
#endif
				BGBCC_DBGBREAK
			}

	//		printf("%s: %d %d\n", str, bgbpp_iflvl, bgbpp_iflvl2);

	#if 0
			else if(!i) { bgbpp_iflvl2++; }
			else if(i<0)
			{
				BGBPP_Error(ctx, "#if pp-expr couldn't be evaluated\n");
			}
	#endif
			return;
		}
	}

	if(b[0]=='m')
	{
		if(!bgbcp_strcmp5(b, "macroed"))
		{
//			tn=bgbcc_strdup(b2);
			tn="_MACROED_";

//			while(*s1 && (*s1<=' '))s1++;
			BGBPP_AddDefine(ctx, tn, "");
			bgbpp_inmacro=tn;
			return;
		}
	}

//	if(!bgbpp_strcmp(b, "pragma"))
	if(!bgbcp_strcmp6(b, "pragma"))
	{
//		if(!bgbpp_strcmp(b2, "once"))
		if(!bgbcp_strcmp4(b2, "once"))
		{
			if(!bgbpp_seen_isnew)
				bgbpp_seen_isabort=1;
		}
	
		if(!bgbcp_strcmp4(b2, "setlocale"))
		{
			s1=BGBCP_TokenCtx(ctx, s1, b3, &ty2);	//'('
			s1=BGBCP_TokenCtx(ctx, s1, b3, &ty2);
			sprintf(bgbpp_obuf, "_Pragma(setlocale(\"%s\"))\n", b3);
			return;
		}

		if(!bgbcp_strcmp(b2, "fenv_access"))
		{
			s1=BGBCP_TokenCtx(ctx, s1, b3, &ty2);	//'('
			s1=BGBCP_TokenCtx(ctx, s1, b3, &ty2);
			sprintf(bgbpp_obuf, "_Pragma(fenv_access(\"%s\"))\n", b3);
			return;
		}

		if(!bgbcp_strcmp(b2, "STDC"))
		{
			s1=BGBCP_TokenCtx(ctx, s1, b3, &ty3);
			s1=BGBCP_TokenCtx(ctx, s1, b4, &ty4);
			
			if(!bgbcp_strcmp(b3, "FENV_ACCESS"))
			{
				sprintf(bgbpp_obuf, "_Pragma(fenv_access(\"%s\"))\n", b4);
				return;
			}
		}

		return;
	}

//	if(!bgbpp_strcmp(b, "undef"))
	if(!bgbcp_strcmp5(b, "undef"))
	{
		BGBPP_DeleteDefine(ctx, b2);
		return;
	}

//	if(!bgbpp_strcmp(b, "warning"))
	if(!bgbcp_strcmp7(b, "warning"))
	{
		s=BGBCP_EatWhiteOnly(s);
		printf("BGBPP_Directive: %s:%d: #warning: \"%s\"\n",
			BGBCP_GetFilename(),
			BGBCP_CalcLinenum(bgbpp_spos), s);
		bgbpp_warn++;

		return;
	}

	printf("BGBPP_Directive: %s:%d: Unsupported preproc directive %s\n",
		BGBCP_GetFilename(),
		BGBCP_CalcLinenum(bgbpp_spos), b);
	bgbpp_err++;
}


static int bgbpp_ncnt_expand;

char *BGBPP_ParseArg(BGBCP_ParseState *ctx,
	BGBPP_Def *pudef, char *s, char *b)
{
	char b2[4096];
	BGBPP_Def *def;
	int i, ty2;

	i=0;
	while(*s)
	{
		if((*s=='\"') || (*s=='\''))
		{
			s=BGBCP_TokenCtx(ctx, s, b2, &ty2);
			b=BGBPP_PrintToken(b, b2, ty2);
			continue;
		}
		
		if(*s<=' ')
		{
			*b++=*s++;
			continue;
		}

		if(*s=='(') { i++; *b++=*s++; continue; }
		if(*s==')') { if(!i)break; i--; *b++=*s++; continue; }
		if(*s==',') { if(!i)break; *b++=*s++; continue; }

//		if(1)
		if(0)
		{
			*b++=*s++;
			continue;
		}

		s=BGBCP_TokenCtx(ctx, s, b2, &ty2);
//		s=BGBCP_EatWhite(s);

//		if(ty2!=BTK_NAME)
		if((ty2!=BTK_NAME) || (bgbpp_ncnt_expand<8))
//		if(1)
		{
			b=BGBPP_PrintToken(b, b2, ty2);
			continue;
		}

		def=BGBPP_LookupDefine(ctx, b2);
		if(!def || (def==pudef))
		{
			b=BGBPP_PrintToken(b, b2, ty2);
			continue;
		}

		if((*s!='(') && !def->args)
		{
			strcpy(b, def->value);
			b+=strlen(b);
			continue;
		}
		
//		BGBCC_DBGBREAK
		b=BGBPP_PrintToken(b, b2, ty2);
		continue;
	}

	*b++=0;
	return(s);
}

char *BGBPP_PrintName(char *t, char *s)
{
	int i;

	i=*(t-1);
	if((i=='_') || (i=='$') || ((i>='0') && (i<='9')) ||
		((i>='A') && (i<='Z')) || ((i>='a') && (i<='z')))
			*t++=' ';

	if(BGBCP_OpChar(i) && BGBCP_OpChar(*s))
		*t++=' ';

	strcpy(t, s);
	t+=strlen(s);
	return(t);
}

char *BGBPP_PrintToken(char *t, char *s, int ty)
{
	if(ty==BTK_NAME)
	{
		t=BGBPP_PrintName(t, s);
		return(t);
	}

	if(ty==BTK_NUMBER)
	{
		if(BGBCP_OpChar(*(t-1)) && BGBCP_OpChar(*s))
			*t++=' ';

		t=BGBPP_PrintName(t, s);
		return(t);
	}

	if(ty==BTK_STRING)
	{
		t=BGBPP_EmitString(t, s);
		return(t);
	}

	if(ty==BTK_CHARSTRING)
	{
		t=BGBPP_EmitCharString(t, s);
		return(t);
	}

	if(ty==BTK_OPERATOR)
	{
		if(BGBCP_OpChar(*(t-1)))
			*t++=' ';
//		sprintf(t, "%s", s);
//		t+=strlen(t);
		t=BGBCC_StrPrintRawStr(t, s);
		return(t);
	}

	if(BGBCP_OpChar(*(t-1)) && BGBCP_OpChar(*s))
		*t++=' ';

//	sprintf(t, "%s", s);
//	t+=strlen(t);
	t=BGBCC_StrPrintRawStr(t, s);
	return(t);
}

int BGBPP_Expand(BGBCP_ParseState *ctx,
	char **src, char **dst, char *op, char *dste)
{
//	char b[256], b2[256];
	char b[4096], b2[4096];
	char *ab[64];
	BGBPP_Def *def;
	char *s, *s1, *t, *te;
	int i, j, ty, ty2, isws;

	s=*src; t=*dst; te=dste;

	if((t+16)>=te)
	{
		BGBPP_Error(ctx, "BGBPP_Expand: Buffer Overflow\n");
		return(-1);
	}

//	if(!bgbpp_strcmp(op, "defined"))
	if(!bgbcp_strcmp7(op, "defined"))
	{
		s=BGBCP_TokenCtx(ctx, s, b, &ty);	//<name> | '('
//		if(!bgbpp_strcmp(b, "("))
		if(!bgbcp_strcmp1(b, "("))
		{
			s=BGBCP_TokenCtx(ctx, s, b, &ty); //<name>
			def=BGBPP_LookupDefine(ctx, b);
			s=BGBCP_TokenCtx(ctx, s, b, &ty); //')'

//			sprintf(t, "%d ", def?1:0);
//			t+=strlen(t);

//			if(def)
//				{ *t++'1'; *t++' '; *t=0; }
//			else
//				{ *t++'0'; *t++' '; *t=0; }

			*t++='0'+(def!=NULL);
			*t++=' ';
			*t=0;

			*src=s; *dst=t;
			return(1);
		}

		def=BGBPP_LookupDefine(ctx, b);
//		sprintf(t, "%d ", def!=NULL);
//		t+=strlen(t);
		*t++='0'+(def!=NULL);
		*t++=' ';
		*t=0;

		*src=s; *dst=t;
		return(1);
	}

	if(bgbpp_ncnt_expand<2)
		return(0);

	def=BGBPP_LookupDefine(ctx, op);
	if(!def)return(0);

//	if(!strcmp(def->name, "__Q8_2J") ||
//		!strcmp(def->name, "__Q8_1J"))
//		__debugbreak();

//	if(!strcmp(def->name, "__Q8_UI") ||
//		!strcmp(def->name, "__Q8_TI"))
//	{
//		__debugbreak();
//		j=-1;
//	}

	if(!def->args)
	{
		if((t+strlen(def->value)+16)>=te)
		{
			BGBPP_Error(ctx, "BGBPP_Expand: Buffer Overflow\n");
			return(-1);
		}

		t=BGBPP_PrintName(t, def->value);
		*src=s; *dst=t;
		return(1);
	}

	s=BGBCP_EatWhiteOnly(s);

	if(*s!='(')return(0);

	s++; j=0;
	while(def->args[j])
	{
		while(*s && (*s<=' '))s++;

		if(*s==',')
		{
			ab[j++]=bgbcc_strdup("");
			s++; continue;
		}
		if(*s==')') break;
		while(*s && (*s<=' '))s++;

		s=BGBPP_ParseArg(ctx, def, s, b2);
		ab[j++]=bgbcc_strdup(b2);

		if(*s==',')
		{
			s++;
			while(*s && (*s<=' '))s++;
			if(*s==')')
			{
				ab[j++]=bgbcc_strdup("");
				break;
			}
			continue;
		}
	}
	ab[j]=NULL;

	if(def->args[j] || (*s!=')'))
	{
		BGBPP_Error(ctx, "BGBPP_Expand: macro parse problem (%s)\n", op);
//		printf("BGBPP_Expand: macro parse problem (%s)\n", op);
		bgbpp_err++;
		return(0);
	}
	s++;

//	if(!strcmp(def->name, "__Q8_2J") ||
//		!strcmp(def->name, "__Q8_1J"))
//		__debugbreak();

	*src=s;

	s=def->value;
	while(*s)
	{
		isws=((*s)<=' ');
	
		s=BGBCP_TokenCtx(ctx, s, b, &ty);
		s1=BGBCP_TokenCtx(ctx, s, b2, &ty2);

		if(isws)
			*t++=' ';

		if((ty==BTK_OPERATOR) && !strcmp(b, "#"))
		{
			if(ty2==BTK_NAME)
			{
				for(i=0; def->args[i]; i++)
					if(!bgbpp_strcmp(def->args[i], b2))
						break;

				if(def->args[i])
				{
					if((t+strlen(ab[i])+16)>=te)
					{
						BGBPP_Error(ctx, "BGBPP_Expand: Buffer Overflow\n");
						return(-1);
					}

					t=BGBPP_EmitString(t, ab[i]);
					s=s1;
					continue;
				}

				s=s1;
				t=BGBPP_EmitString(t, b2);
				continue;
			}

			s=s1;
			t=BGBPP_EmitString(t, b2);
			continue;
		}


		if(ty==BTK_NAME)
		{
			for(i=0; def->args[i]; i++)
				if(!bgbpp_strcmp(def->args[i], b))
					break;

			if(def->args[i])
			{
				if((t+strlen(ab[i])+16)>=te)
				{
					BGBPP_Error(ctx, "BGBPP_Expand: Buffer Overflow\n");
					return(-1);
				}

				t=BGBPP_PrintName(t, ab[i]);
				continue;
			}

			if((t+strlen(b)+16)>=te)
			{
				BGBPP_Error(ctx, "BGBPP_Expand: Buffer Overflow\n");
				return(-1);
			}

			t=BGBPP_PrintName(t, b);
			continue;
		}

		if((t+strlen(b)+16)>=te)
		{
			BGBPP_Error(ctx, "BGBPP_Expand: Buffer Overflow\n");
			return(-1);
		}

		if(ty==BTK_NUMBER)
		{
			t=BGBPP_PrintName(t, b);
			continue;
		}

		if(ty==BTK_STRING)
		{
			t=BGBPP_EmitString(t, b);
			continue;
		}

		if(ty==BTK_CHARSTRING)
		{
			t=BGBPP_EmitCharString(t, b);
			continue;
		}

		if(ty==BTK_OPERATOR)
		{
			if(BGBCP_OpChar(*(t-1)))
				*t++=' ';
//			sprintf(t, "%s", b);
//			t+=strlen(t);
			t=BGBCC_StrPrintRawStr(t, b);
			continue;
		}

		if(ty==BTK_STRING_QQQ)
		{
			BGBCP_CopyStringQQQB(&t, &s);
//			t=BGBPP_EmitCharString(t, b);
			continue;
		}

//		sprintf(t, "%s", b);
//		t+=strlen(t);
		t=BGBCC_StrPrintRawStr(t, b);
	}

	*t=0;

	*dst=t;
	return(1);
}

static char *bgbpp_lbuf=NULL;
static char *bgbpp_lbufe;

int BGBPP_CheckExpandLBuf(int sz, char **rt)
{
	char *t;
	int i, j, ret;
	
	if(!bgbpp_lbuf)
	{
		bgbpp_lbuf=malloc(16384);
		bgbpp_lbufe=bgbpp_lbuf+16384;
	}
	
	if(!sz)
		return(0);

	ret=0;
	t=bgbpp_lbuf;
	if(rt)t=*rt;

	if((t+(sz+16))>=bgbpp_lbufe)
	{
		i=bgbpp_lbufe-bgbpp_lbuf;
		j=t-bgbpp_lbuf;
		while((j+(sz+16))>=i)
			i=i+(i>>1);
		bgbpp_lbuf=realloc(bgbpp_lbuf, i);
		bgbpp_lbufe=bgbpp_lbuf+i;
//		te=bgbpp_lbufe;
		t=bgbpp_lbuf+j;
		ret=1;
	}

	if(rt)*rt=t;
	return(ret);
}

static char *bgbpp_lbuf_tmp;

int BGBPP_ScriptPuts(BGBCP_ParseState *ctx, char *str)
{
	strcpy(bgbpp_lbuf_tmp, str);
	bgbpp_lbuf_tmp+=strlen(str);
	return(0);
}

int BGBPP_Line(BGBCP_ParseState *ctx, char *str)
{
//	static char lbuf[1<<20];
//	char lbuf[16384];
//	char lbuf[65536];
	char b[4096], b2[4096], b3[256];
	char *s, *t, *t1, *t2, *te, *t1e, *t2e;
	int i, j, k, ty, ty2, ty3, ni, isws;

	BGBPP_CheckExpandLBuf(0, NULL);

	BGBCP_FlushToken(str);

	ni=0;
	s=str; t=bgbpp_lbuf;
//	te=bgbpp_lbuf+((1<<20)-4096);
	te=bgbpp_lbufe;
	while(*s)
	{
		isws=((*s)<=' ');
		s=BGBCP_TokenCtx(ctx, s, b, &ty);
		BGBCP_TokenCtx(ctx, s, b2, &ty2);

		if((ty2==BTK_OPERATOR) &&
//			(!bgbpp_strcmp(b2, "##") || !bgbpp_strcmp(b, "%:%:")))
			(!bgbcp_strcmp2(b2, "##") || !bgbcp_strcmp4(b, "%:%:")))
		{
			if(ty==BTK_NAME)
			{
				t1=t+1;	t1e=t1+2044;
				i=BGBPP_Expand(ctx, &s, &t1, b, t1e);
				if(i>0) { strcpy(b, t+1); ni++; }
			}

			s=BGBCP_TokenCtx(ctx, s, b2, &ty2);	//'##'

			s=BGBCP_TokenCtx(ctx, s, b2, &ty2);

			if(ty2==BTK_NAME)
			{
				t1=t+1;	t1e=t1+2044;
				i=BGBPP_Expand(ctx, &s, &t1, b2, t1e);
				if(i>0) { strcpy(b2, t+1); ni++; }
			}

//			printf("## operator: '%s' ## '%s' ('%s')\n", b, b2, str);

			*t=0;
			strcat(b, b2);
			
//			if((t+strlen(b)+16)>=te)
//			{
//				BGBPP_Error(ctx, "BGBPP_Line: Buffer Overflow\n");
//				return(-1);
//			}

			BGBPP_CheckExpandLBuf(strlen(b), &t);
			te=bgbpp_lbufe;
			
			if(isws)
				*t++=' ';
			
			if((ty==BTK_STRING) || (ty2==BTK_STRING))
			{
				t=BGBPP_EmitString(t, b);
			}else if((ty==BTK_CHARSTRING)||(ty2==BTK_CHARSTRING))
			{
				t=BGBPP_EmitCharString(t, b);
			}else
			{
				t=BGBPP_PrintName(t, b);
			}
			
//			printf("## operator: '%s'\n", b);
			
			ni++;
			continue;
		}

//		if((t+strlen(b)+16)>=te)
//		{
//			BGBPP_Error(ctx, "BGBPP_Line: Buffer Overflow\n");
//			return(-1);
//		}

//		BGBPP_CheckExpandLBuf(strlen(b), &t);
		BGBPP_CheckExpandLBuf(8192, &t);
		te=bgbpp_lbufe;

		if(isws)
			*t++=' ';

		if(ty==BTK_NAME)
		{
			i=BGBPP_Expand(ctx, &s, &t, b, te);
			if(i>0)
			{
				ni++;
				continue;
			}
			
//			if(!strcmp(b, "__FILE__"))
			if(!bgbcp_strcmp8(b, "__FILE__"))
			{
				t=BGBPP_PrintToken(t, bgbpp_lfn, BTK_STRING);
				ni++;
				continue;
			}

//			if(!strcmp(b, "__LINE__"))
			if(!bgbcp_strcmp8(b, "__LINE__"))
			{
//				t=BGBPP_PrintToken(t, bgbpp_lln, BTK_NUMBER);
//				sprintf(t, "%d ", bgbpp_lln);
//				t+=strlen(t);
				t=BGBCC_StrPrintInt(t, bgbpp_lln);
				*t++=' ';	*t=0;
				ni++;
				continue;
			}

			t=BGBPP_PrintName(t, b);
			continue;
		}

		if((ty==BTK_OPERATOR) && (ctx->lang!=BGBCC_LANG_ASM) &&
			(ctx->lang!=BGBCC_LANG_BS2) &&
//			(!bgbpp_strcmp(b, "#") || !bgbpp_strcmp(b, "%:")))
			(!bgbcp_strcmp1(b, "#") || !bgbcp_strcmp2(b, "%:")))
		{
			if(ty2==BTK_NAME)
			{
				s=BGBCP_TokenCtx(ctx, s, b2, &ty2);

				t1=t;
				i=BGBPP_Expand(ctx, &s, &t1, b2, te);
				if(i>0) { strcpy(b2, t); ni++; }

				t=BGBPP_PrintToken(t, b2, BTK_STRING);
				ni++;
				continue;
			}

			s=BGBCP_TokenCtx(ctx, s, b2, &ty2);
			t=BGBPP_PrintToken(t, b2, BTK_STRING);
			ni++;
			continue;
		}

		if((ty==BTK_OPERATOR) && !bgbcp_strcmp2(b, "#``") && (ty2==BTK_NAME))
		{
			s=BGBCP_TokenCtx(ctx, s, b2, &ty2);

			t1=t;
			i=BGBPP_Expand(ctx, &s, &t1, b2, te);
			if(i>0)
			{
				strcpy(b2, t);
			}

			if(*s=='(')
			{
				t1=b2+strlen(b2);
				t1e=b2+2044;

				*t1++=*s++; i=1;
				while(*s && (i>0))
				{
					s=BGBCP_TokenCtx(ctx, s, b3, &ty3);

					if(!strcmp(b3, "("))
					{
						t1=BGBPP_PrintToken(t1, b3, ty3);
						i++;
						continue;
					}

					if(!strcmp(b3, ")"))
					{
						t1=BGBPP_PrintToken(t1, b3, ty3);
						i--;
						continue;
					}

					if(ty3==BTK_NAME)
					{
						t2=t1;	t2e=t1e;
						j=BGBPP_Expand(ctx, &s, &t2, b3, t2e);
						if(j>0)
						{
							t1=t2;
							continue;
						}
					}

					t1=BGBPP_PrintToken(t1, b3, ty3);
				}
				*t1=0;
			}
			
			bgbpp_lbuf_tmp=t;
			BGBPP_InterpMacroed(ctx, b2);
			t=bgbpp_lbuf_tmp;

			ni++;
			continue;
		}

		if(ty==BTK_NUMBER)
		{
			t=BGBPP_PrintName(t, b);
			continue;
		}

		if(ty==BTK_STRING)
		{
			t=BGBPP_EmitString(t, b);
			continue;
		}

		if(ty==BTK_CHARSTRING)
		{
			t=BGBPP_EmitCharString(t, b);
			continue;
		}

		if(ty==BTK_OPERATOR)
		{
			if(BGBCP_OpChar(*(t-1)))
				*t++=' ';
//			sprintf(t, "%s", b);
//			t+=strlen(t);
			t=BGBCC_StrPrintRawStr(t, b);
			continue;
		}

		if(ty==BTK_STRING_QQQ)
		{
			BGBCP_CopyStringQQQB(&t, &s);
//			t=BGBPP_EmitCharString(t, b);
			continue;
		}

//		sprintf(t, "%s", b);
//		t+=strlen(t);
		t=BGBCC_StrPrintRawStr(t, b);
	}
	*t=0;


	BGBCP_FlushToken(str);

	memset(str, 0, 256);
	strcpy(str, bgbpp_lbuf);
	return(ni);
}

int BGBPP_LineDigraph(BGBCP_ParseState *ctx, char *str)
{
//	char lbuf[16384], b[4096];
//	static char lbuf[1<<20];
	char b[4096];
	char *s, *t;
	int i, j, ty, l, isws;

	//limit digraphs to C/C++
	if((ctx->lang!=BGBCC_LANG_C) && (ctx->lang!=BGBCC_LANG_CPP))
		return(0);

	BGBPP_CheckExpandLBuf(0, NULL);

	s=str; t=bgbpp_lbuf;
	while(*s)
	{
		isws=((*s)<=' ');
		s=BGBCP_TokenCtx(ctx, s, b, &ty);

		if((ty==BTK_BRACE) || (ty==BTK_OPERATOR))
		{
#if 0
			if(!bgbpp_strcmp(b, "<:"))strcpy(b, "[");
			if(!bgbpp_strcmp(b, ":>"))strcpy(b, "]");
			if(!bgbpp_strcmp(b, "<%"))strcpy(b, "{");
			if(!bgbpp_strcmp(b, "%>"))strcpy(b, "}");
			if(!bgbpp_strcmp(b, "%:"))strcpy(b, "#");
			if(!bgbpp_strcmp(b, "%:%:"))strcpy(b, "##");
#endif

#if 1
			if(!bgbcp_strcmp2(b, "<:"))strcpy(b, "[");
			if(!bgbcp_strcmp2(b, ":>"))strcpy(b, "]");
			if(!bgbcp_strcmp2(b, "<%"))strcpy(b, "{");
			if(!bgbcp_strcmp2(b, "%>"))strcpy(b, "}");
			if(!bgbcp_strcmp2(b, "%:"))strcpy(b, "#");
			if(!bgbcp_strcmp4(b, "%:%:"))strcpy(b, "##");
#endif
		}

#if 0
		l=strlen(b);
		if((t+(l+16))>bgbpp_lbufe)
		{
			i=bgbpp_lbufe-bgbpp_lbuf;
			j=t-bgbpp_lbuf;
			while((j+(l+16))>=i)
				i=i+(i>>1);
			bgbpp_lbuf=realloc(bgbpp_lbuf, i);
			bgbpp_lbufe=lbuf+i;
			t=lbuf+j;
		}
#endif

		BGBPP_CheckExpandLBuf(strlen(b), &t);
		
		if(isws)
			*t++=' ';

		t=BGBPP_PrintToken(t, b, ty);
	}
	*t=0;

	strcpy(str, bgbpp_lbuf);
	return(0);
}

int BGBPP_LinePostFilter(BGBCP_ParseState *ctx, char *str)
{
	BGBPP_LineDigraph(ctx, str);
	return(0);
}

static const char bgbpp_char_crlf[256]={
/* 0 1 2 3 4 5 6 7 8 9 A B C D E F */
   1,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0, //0z
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //1z
};

/* Non-special chars. */
static const char bgbpp_nospchar[128]={
/* 0 1 2 3 4 5 6 7 8 9 A B C D E F */
   0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0, //0z
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //1z
   1,1,0,1,1,1,1,0,0,0,1,1,1,1,1,0, //2z
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //3z
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //4z
   1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1, //5z
   0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //6z
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0  //7z
};


char *BGBPP_ParseLine(BGBCP_ParseState *ctx, char *s, char *b)
{
	static int is=0, is2=0;
	char *t, *s1;
	u32 tcv;
	int i, j, k;
	int nplvl;

	if((*s=='\n') || (*s=='\r'))
	{
		*b=0;
		if(*s=='\r')s++;
		if(*s=='\n')s++;
		return(s);
	}

	is=0;
	is2=0;

	t=b; nplvl=0;
	while(*s)
	{
#if 0
		if(*s=='\"')
		{
			is=!is;
			*t++=*s++;
			continue;
		}
#endif

#if 0
		if(bgbpp_nospchar[*s])
		{
			*t++=*s++;
			continue;
		}
#endif

#if 0
		tcv=BGBCC_GET_U32LE(s);
		if(!(tcv&BGBCC_ASC_MASK32))
		{
			i=	bgbpp_nospchar[(byte)(tcv>> 0)] &
				bgbpp_nospchar[(byte)(tcv>> 8)] ;
			if(i)
			{
				j=	bgbpp_nospchar[(byte)(tcv>>16)] &
					bgbpp_nospchar[(byte)(tcv>>24)] ;
				if(j)
				{
					BGBCC_SET_U32LE(t, tcv);
					s+=4;	t+=4;
					continue;
				}
				BGBCC_SET_U16LE(t, tcv);
				s+=2;	t+=2;
				continue;
			}
		}
#endif


		if(is)
		{
			if(*s=='\"')
			{
				is=0;
				*t++=*s++;
				continue;
			}

#if 0
			if((s[0]=='\\') && (s[1]=='\''))
				{ *t++=*s++; *t++=*s++; continue; }
			if((s[0]=='\\') && (s[1]=='\"'))
				{ *t++=*s++; *t++=*s++; continue; }
			if((s[0]=='\\') && (s[1]=='\\'))
				{ *t++=*s++; *t++=*s++; continue; }
#endif
			
			if(s[0]=='\\')
			{
				if((s[1]=='\'') || (s[1]=='\"') || (s[1]=='\\'))
					{ *t++=*s++; *t++=*s++; continue; }
			}

			*t++=*s++;
			continue;
		}

		if(is2)
		{
			if(*s=='\'')
			{
				is2=0;
				*t++=*s++;
				continue;
			}

#if 0
			if((s[0]=='\\') && (s[1]=='\''))
				{ *t++=*s++; *t++=*s++; continue; }
			if((s[0]=='\\') && (s[1]=='\"'))
				{ *t++=*s++; *t++=*s++; continue; }
			if((s[0]=='\\') && (s[1]=='\\'))
				{ *t++=*s++; *t++=*s++; continue; }
#endif

			if(s[0]=='\\')
			{
				if((s[1]=='\'') || (s[1]=='\"') || (s[1]=='\\'))
					{ *t++=*s++; *t++=*s++; continue; }
			}

			*t++=*s++;
			continue;
		}

		if(*s=='\"')
		{
			if((s[1]=='\"') && (s[2]=='\"'))
			{
				s+=3;
				BGBCP_CopyStringQQQB(&t, &s);
				continue;
			}
		
			is=1;
			*t++=*s++;
			continue;
		}

		if(*s=='\'')
		{
			is2=1;
			*t++=*s++;
			continue;
		}

		if((*s=='\n') || (*s=='\r'))
		{
			if((t-b)>8192)
				break;
				
			s1=s;
			while(*s1 && (*s1<=' '))s1++;
			if(*s1=='#')
				break;

			if(nplvl>0)
			{
				*t++=*s++;
				continue;
			}
			break;
		}

#if 0
		if((s[0]=='/') && (s[1]=='/'))
		{
			if(nplvl>0)
			{
				*t++=' ';
				while(*s && (*s!='\n') && (*s!='\r'))
					s++;
				continue;
			}

			*t++=' ';
			while(*s && (*s!='\n') && (*s!='\r'))
				s++;
			break;
		}

		if((s[0]=='/') && (s[1]=='*'))
		{
			s+=2;
//			while(*s && (*s!='\n') && (*s!='\r'))
			while(*s)
			{
				if((s[0]=='*') && (s[1]=='/'))
				{
					s+=2;
					break;
				}
				s++;
			}
			*t++=' ';
			continue;
		}
#endif

#if 1
		if(s[0]=='/')
		{
			if(s[1]=='/')
			{
				s+=2;

				if(nplvl>0)
				{
					*t++=' ';
					while(*s && (*s!='\n') && (*s!='\r'))
						s++;
					continue;
				}

				*t++=' ';
				
#if 0
				while(1)
				{
					i=BGBCC_GET_U32LE(s);
					j=	bgbpp_char_crlf[(byte)(i>> 0)] |
						bgbpp_char_crlf[(byte)(i>> 8)] |
						bgbpp_char_crlf[(byte)(i>>16)] |
						bgbpp_char_crlf[(byte)(i>>24)] ;
					if(j)	break;
					s+=4;
				}
#endif
				
				while(*s && (*s!='\n') && (*s!='\r'))
//				while(!bgbpp_char_crlf[*s])
					s++;
				break;
			}else if(s[1]=='*')
			{
				s+=2;
				while(*s)
				{
					if((s[0]=='*') && (s[1]=='/'))
					{
						s+=2;
						break;
					}
					s++;
				}
				*t++=' ';
				continue;
			}
		}
#endif

		if((*s=='\\') && ((s[1]=='\r') || (s[1]=='\n')))
		{
			s++;
			if(*s=='\r')s++;
			if(*s=='\n')s++;
			while(*s && (*s<=' ') && (*s!='\r') && (*s!='\n'))
				s++;
			*t++=' ';
			continue;
		}
		
		if(*s=='(')nplvl++;
		if(*s==')')nplvl--;
		
		*t++=*s++;
	}
	*t++=0;
	if(*s=='\r')s++;
	if(*s=='\n')s++;

	if(is || is2)
		{ BGBCC_DBGBREAK }

	return(s);
}

int BGBPP_BufferLine(BGBCP_ParseState *ctx, char *b)
{
	char *s, *t, *s1, *t1;
	int i, cln, pln;

	t=b; while(*t && (*t<=' '))t++;
	if(!*t)
	{
//		if(!bgbpp_iflvl2)
//			sprintf(bgbpp_obuf, "\n");
		*b=0;
		return(0);
	}

	BGBPP_Trigraphs(ctx, b);
	BGBPP_UniEscape(ctx, b);

	s1=b; while(*s1 && (*s1<=' '))s1++;

	if((s1[0]=='#') && (s1[1]=='!'))
	{
//		*b=0;
		return(0);
	}

	if((s1[0]=='#') && (s1[1]=='#'))
	{
		if(bgbpp_inmacro)
		{
			if(bgbpp_iflvl2)
				return(0);
			BGBPP_AppendDefineLine(ctx,
				bgbpp_inmacro, s1+1);
			return(0);
		}
	}

//	if(*s1=='#')
	if((s1[0]=='#') && (s1[1]!='`'))
	{
		BGBCP_FlushToken(b);
		BGBPP_Directive(ctx, b);
		return(0);
	}

	if(bgbpp_iflvl2)
		{ return(0); }

	if(bgbpp_inmacro)
	{
		BGBPP_AppendDefineLine(ctx, bgbpp_inmacro, b);
		return(0);
	}

	bgbpp_ncnt_expand=256;
	i=1;
	while(i && (bgbpp_ncnt_expand>0))
	{
		bgbpp_ncnt_expand--;
		BGBCP_FlushToken(b);
		i=BGBPP_Line(ctx, b);
	}

	BGBCP_FlushToken(b);
	BGBPP_LinePostFilter(ctx, b);

	if((bgbpp_obuf+strlen(b)+256)>=bgbpp_obufe)
	{
		BGBPP_Error(ctx, "BGBPP_BufferLine: Output Buffer Overflow\n");
		return(-1);
	}

	cln=BGBCP_CalcLinenum(bgbpp_spos);
	pln=bgbpp_lln+1;

	s1=BGBCP_GetFilename();
	if((s1!=bgbpp_lfn) && (*s1!='$'))
	{
//		sprintf(bgbpp_obuf, "/*\"%s\"%d*/ %s\n",
//			s1, cln, b);

		t1=bgbpp_obuf;
		*t1++='/';	*t1++='*';
		*t1++='"';
		t1=BGBCC_StrPrintRawStr(t1, s1);
		*t1++='"';
		t1=BGBCC_StrPrintInt(t1, cln);
		*t1++='*';	*t1++='/';
		t1=BGBCC_StrPrintRawStr(t1, b);
		*t1++='\n';
		*t1=0;
		
		bgbpp_lfn=s1;
		bgbpp_lln=cln;
	}else if((cln!=pln) && (*s1!='$'))
	{
//		sprintf(bgbpp_obuf, "/*%d*/ %s\n", cln, b);
		
		t1=bgbpp_obuf;
		*t1++='/';	*t1++='*';
		t1=BGBCC_StrPrintInt(t1, cln);
		*t1++='*';	*t1++='/';
		t1=BGBCC_StrPrintRawStr(t1, b);
		*t1++='\n';
		*t1=0;
		
		bgbpp_lln=cln;
	}else
	{
//		sprintf(bgbpp_obuf, "%s\n", b);
		t1=BGBCC_StrPrintRawStr(bgbpp_obuf, b);
		*t1++='\n';
		*t1=0;
		
		bgbpp_lln=cln;
	}
	return(0);
}

int BGBPP_Buffer(BGBCP_ParseState *ctx, char *ibuf)
{
	static char b[1<<20];
//	char b[16384];
//	char b[65536];
	char *s, *t, *s1;
	int i;

	s=ibuf;
	while(*s)
	{
		if((bgbpp_obuf+4096)>=bgbpp_obufe)
		{
			BGBPP_Error(ctx, "BGBPP_Buffer: Output Buffer Overflow\n");
			return(-1);
		}
	
		bgbpp_spos=s;
		
#if 0
		s1=s;
		while(*s1 && (*s1<=' '))s1++;
		if((s1[0]=='/') && (s1[1]=='/'))
		{
			s=s1; t=bgbpp_obuf;
			while(*s && (*s!='\r') && (*s!='\n')) *t++=*s++;
			if(*s=='\r')s++; if(*s=='\n')*t++=*s++;
//			while(*s && (*s<=' '))*t++=*s++;
			bgbpp_obuf=t; *t=0;
			continue;
		}

		if((s1[0]=='/') && (s1[1]=='*') && (s1[2]=='*'))
		{
			s=s1; t=bgbpp_obuf;
			*t++=*s++; *t++=*s++; *t++=*s++;
			while(*s)
			{
				if((s[0]=='*') && (s[1]=='/'))
					{ *t++=*s++; *t++=*s++; break; }
				if(*s=='\r') { s++; continue; }
				*t++=*s++;
			}
//			*t++=' ';
			while(*s && ((*s==' ')||(*s=='\t'))) *t++=*s++;
			if(*s=='\r')s++; if(*s=='\n')*t++=*s++;
//			while(*s && (*s<=' '))*t++=*s++;
			bgbpp_obuf=t; *t=0;
			continue;
		}

		if((s1[0]=='/') && (s1[1]=='*'))
		{
			s=s1; t=bgbpp_obuf;
			s+=2;
			while(*s)
			{
				if((s[0]=='*') && (s[1]=='/'))
					{ s+=2; break; }
				s++;
			}
			while(*s && (*s<=' '))
				{ *s++; }
			bgbpp_obuf=t; *t=0;
			continue;
		}
#endif
		
		s=BGBPP_ParseLine(ctx, s, b);

#ifdef CATCH_PP_SEH
		__try {
#endif

		BGBPP_BufferLine(ctx, b);

		if(bgbpp_seen_isabort)
			break;

#ifdef CATCH_PP_SEH
		} __except(EXCEPTION_EXECUTE_HANDLER)
		{
			printf("RECAUGHT(PP)\n");
			continue;
		}
#endif

//		sprintf(bgbpp_obuf, "%s\n", b);
		bgbpp_obuf+=strlen(bgbpp_obuf);
	}
	
	return(0);
}


int BGBPP_Filter(BGBCP_ParseState *ctx, char *ibuf, char *obuf, int osz)
{
//	char tb[4096];
	char tb[512];
	char *s, *t;
	int i;


	for(i=0; i<bgbpp_nsinc; i++)
		bgbpp_inc[i]=bgbpp_sinc[i];
	bgbpp_ninc=bgbpp_nsinc;

	s=BGBCP_GetFilename();
	if(*s)
	{
		strcpy(tb, s);
		s=tb+strlen(tb);
		while((s>tb) && (*s!='/'))s--;

		if(*s=='/')
		{
			*s=0;

			i=bgbpp_ninc++;
			bgbpp_inc[i]=bgbcc_strdup(tb);
		}
	}

	bgbpp_ibuf=ibuf;
	bgbpp_obuf=obuf;
	bgbpp_obufe=obuf+osz;
	bgbpp_noinclude_p=0;

//	for(i=0; i<256; i++)
	for(i=0; i<BGBCC_PPDEF_SIZE; i++)
		ctx->ppdef[i]=NULL;

	bgbpp_warn=0;
	bgbpp_err=0;

	bgbpp_iflvl=0;
	bgbpp_iflvl2=0;
	bgbpp_seen_ninc=0;

//	BGBPP_Include(ctx, "_bscc_conf.h");

	BGBCP_FlushToken(ibuf);
	BGBPP_Buffer(ctx, ibuf);

	BGBPP_SendDefines(ctx);
	BGBCP_FlushToken(obuf);
	
	BGBPP_CleanupDefines(ctx);
	
	if(bgbpp_err>0)
		return(-1);
	return(0);
}

