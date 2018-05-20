/*
Written 2007-2009 by Brendan G Bohannon
This code is re-assigned into the public domain.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASM_API
#define BASM2_API

#define BASM_TOKEN_NAME			1
#define BASM_TOKEN_NUMBER		2
#define BASM_TOKEN_STRING		3
#define BASM_TOKEN_CHARSTRING	4
#define BASM_TOKEN_SEPERATOR	5
#define BASM_TOKEN_OPERATOR		6
#define BASM_TOKEN_BRACE		7
#define BASM_TOKEN_MISC			8
#define BASM_TOKEN_NULL			9


typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed int s32;
typedef signed long long s64;

int BASM_PP_Line(char *str);
int BASM_PP_LineArith(char *str);

char *basm_strtab;	//strings table
char *basm_strtabe;	//strings table end

char *basm_strhash[4096];

char **basm_str_varr;
int *basm_str_carr;
int basm_str_num;
int basm_str_max;
int basm_str_hash[4096];

char *basm_loadfile(char *name, int *rsz)
{
	FILE *fd;
	void *buf;
	int sz;

	fd=fopen(name, "rb");
	if(!fd)return(NULL);

	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);

	buf=malloc(sz+16);
	memset(buf, 0, sz+16);
	fread(buf, 1, sz, fd);

	fclose(fd);

	if(rsz)*rsz=sz;
	return(buf);
}


#if 0
BASM_API char *basm_strdup(char *str)
{
	char *s, *t;
	int i, hi;

#if 1
	if(!basm_strtab)
	{
		basm_strtab=(char *)malloc(1<<22);
		memset(basm_strtab, 0, 1<<22);
		basm_strtabe=basm_strtab+1;
		for(i=0; i<4096; i++)basm_strhash[i]=NULL;
	}
#endif

	if((str>=basm_strtab) && (str<basm_strtabe))
		return(str);

	if(!str)return(NULL);
	if(!*str)return(basm_strtab);


	s=str; i=0;
	while(*s)i=(i*251)+(*s++);
	hi=((i*251)>>8)&0xFFF;

//	t=basm_strhash[hi];
//	if(t && !strcmp(t, str))return(t);

	if(basm_strhash[hi] && !strcmp(basm_strhash[hi], str))
		return(basm_strhash[hi]);

	s=basm_strtab+1;
	while(*s)
	{
		if(!strcmp(s, str))
			return(s);
		s+=strlen(s)+1;
	}

	strcpy(s, str);
	basm_strtabe=s+strlen(s)+1;
	basm_strhash[hi]=s;

	i=basm_strtabe-basm_strtab;
	if(i>=(1<<19))
	{
		printf("basm_strdup: string table size limit approach\n");
	}

	return(s);
}

BASM2_API int basm_strdup_i(char *str)
{
	char *s;
	s=basm_strdup(str);
	return(s-basm_strtab);
}

BASM2_API char *basm_strtab_i(int str)
{
	return(basm_strtab+str);
}
#endif


#if 1
char *basm_strdup_alloc(char *str)
{
	char *s, *t;
	int i, hi;

	if(!basm_strtab)
	{
		basm_strtab=(char *)malloc(1<<20);
		basm_strtabe=basm_strtab+1;
	}

	if((str>=basm_strtab) && (str<basm_strtabe))
		return(str);

	if(!str)return(NULL);
	if(!*str)return(basm_strtab);

	s=basm_strtabe;
	strcpy(s, str);
	basm_strtabe=s+strlen(s)+1;

	i=basm_strtabe-basm_strtab;
	if(i>=((1<<20)-4096))
	{
		//new string table
		basm_strtab=(char *)malloc(1<<20);
		basm_strtabe=basm_strtab+1;
	}

	return(s);
}

BASM2_API int basm_strdup_i(char *str)
{
	char *s, *t;
	int i, hi;

	if(!str)return(0);

#if 1
	if(!basm_str_varr)
	{
		for(i=0; i<4096; i++)basm_str_hash[i]=0;
		basm_str_varr=malloc(16384*sizeof(char *));
		basm_str_carr=malloc(16384*sizeof(int));

		basm_str_varr[0]=NULL;
		basm_str_carr[0]=0;
		basm_str_num=1;
		basm_str_max=16384;
	}
#endif

	s=str; hi=0;
	while(*s)hi=(hi*251)+(*s++);
	hi&=4095;

	i=basm_str_hash[hi];
	while(i)
	{
		if(!strcmp(basm_str_varr[i], str))
			return(i);
		i=basm_str_carr[i];
	}

	if((basm_str_num+1)>=basm_str_max)
	{
		i=basm_str_max+(basm_str_max>>1);
		basm_str_varr=realloc(basm_str_varr, i*sizeof(char *));
		basm_str_carr=realloc(basm_str_carr, i*sizeof(int));
		basm_str_max=i;
	}

	t=basm_strdup_alloc(str);

	i=basm_str_num++;
	basm_str_varr[i]=t;
	basm_str_carr[i]=basm_str_hash[hi];
	basm_str_hash[hi]=i;

	return(i);
}

BASM2_API char *basm_strtab_i(int i)
	{ return(basm_str_varr[i]); }

BASM_API char *basm_strdup(char *str)
{
	int i;

	if(!str)return(NULL);
	i=basm_strdup_i(str);
	return(basm_str_varr[i]);
}
#endif

BASM2_API void *basm_ralloc(int sz);

BASM2_API int basm_strildup_i(char *str)
{
	char *s, *t, *b;
	int i;

	b=basm_ralloc(strlen(str)+1);

	s=str; t=b;
	while(*s)
	{
		i=*s++;
		if((i>='A') && (i<='Z'))
			i=i-'A'+'a';
		*t++=i;
	}
	*t++=0;

	return(basm_strdup_i(b));
}

BASM2_API void *basm_ralloc(int sz)
{
	static char *buf=NULL;
	static int pos=0;
	char *s;

	if(!buf)buf=malloc(1<<16);
	if((pos+sz)>=(1<<16))pos=0;

	s=buf+pos;
	pos=(pos+sz+7)&(~7);
	return(s);
}

BASM2_API char *basm_rstrdup(char *str)
{
	char *t;
	t=basm_ralloc(strlen(str)+1);
	strcpy(t, str);
	return(t);
}

BASM2_API int basm_strisreal(char *str)
{
	char *s;

	s=str;
	if((s[0]=='0') && (s[1]=='x'))
		return(0);
	while(*s)
	{
		if(*s=='.')break;
		if(*s=='e')break;
		if(*s=='E')break;
		s++;
	}
	if(*s)return(1);
	return(0);
}

u32 basm_atof_u32(char *str)
{
	float f;
	f=atof(str);
	return(*(u32 *)(&f));
}

u64 basm_atof_u64(char *str)
{
	double f;
	f=atof(str);
	return(*(u64 *)(&f));
}

u32 basm_atof_u32_sg(char *str, int sg)
{
	float f;
	f=atof(str)*sg;
	return(*(u32 *)(&f));
}

u64 basm_atof_u64_sg(char *str, int sg)
{
	double f;
	f=atof(str)*sg;
	return(*(u64 *)(&f));
}


s64 basm_atoi(char *str)
{
	long long l;
	char *s;
	int i, sg;

	s=str;
	if((s[0]=='0') && (s[1]=='x'))
	{
		l=0; s+=2;
		while(*s)
		{
			l*=16;
			if((*s>='0') && (*s<='9'))l+=*s-'0';
			if((*s>='A') && (*s<='F'))l+=*s-'A'+10;
			if((*s>='a') && (*s<='f'))l+=*s-'a'+10;
			s++;
		}

		return(l);
	}

	if(s[0]=='0')
	{
		l=0; s++;
		while(*s)
		{
			l*=8;
			if((*s>='0') && (*s<='7'))l+=*s-'0';
				else break;
			s++;
		}

		return(l);
	}

	while(s[1])s++;
	if((*s=='b') || (*s=='B'))
	{
		l=0; s=str;
		while(*s)
		{
			l*=2;
			if((*s>='0') && (*s<='1'))l+=*s-'0';
				else break;
			s++;
		}

		return(l);
	}

	if((*s=='h') || (*s=='H'))
	{
		l=0; s=str;
		while(*s)
		{
			l*=16;
			if((*s>='0') && (*s<='9'))
				{ l+=*s-'0'; }
			else if((*s>='A') && (*s<='F'))
				{ l+=*s-'A'+10; }
			else if((*s>='a') && (*s<='f'))
				{ l+=*s-'a'+10; }
			else break;
			s++;
		}

		return(l);
	}


	s=str;
	l=0; sg=0;
	if(*s=='-') { sg=1; s++; }
	while(*s)
	{
		l*=10;
		if((*s>='0') && (*s<='9')) { l+=*s-'0'; }
			else break;
		s++;
	}
	if(sg)l=-l;

	return(l);
}

int basm_stricmp(char *s1, char *s2)
{
	int i, j;

	if(s1==s2)return(0);

	while(*s1 && *s2)
	{
		i=((*s1>='A') && (*s1<='Z'))?(*s1-'A'+'a'):(*s1);
		j=((*s2>='A') && (*s2<='Z'))?(*s2-'A'+'a'):(*s2);
		if(i<j)return(-1);
		if(i>j)return(1);
		s1++; s2++;
	}

	if(*s1)return(1);
	if(*s2)return(-1);
	return(0);
}


BASM2_API char *BASM_Parse_EatWhiteOnly(char *s)
{
	while(*s && (*s<=' '))s++;
	return(s);
}

BASM2_API char *BASM_Parse_EatWhite(char *s)
{
	char *is;
	int i;

	is=s;
	while(*s && (*s<=' '))s++;

#if 0
	if((s!=is) && (s[0]==';'))
	{
		while(*s && (*s!='\n'))s++;
		s=BASM_Parse_EatWhite(s);
		return(s);
	}
#endif

	if((s[0]=='/') && (s[1]=='/'))
	{
		while(*s && (*s!='\n'))s++;
		s=BASM_Parse_EatWhite(s);
		return(s);
	}
	if((s[0]=='/') && (s[1]=='*'))
	{
		s+=2;
		i=1;
		while(*s && i)
		{
			if((s[0]=='/') && (s[1]=='*'))
			{
				s+=2;
				i++;
				continue;
			}
			if((s[0]=='*') && (s[1]=='/'))
			{
				s+=2;
				i--;
				continue;
			}
			s++;
		}
		s=BASM_Parse_EatWhite(s);
		return(s);
	}

	return(s);
}

BASM2_API int BASM_Parse_IsWhiteOnly(char *s)
{
	while((*s==' ') || (*s=='\t'))
		s++;

	if((s[0]=='/') && (s[1]=='/'))return(0);
	if((s[0]=='/') && (s[1]=='*'))return(0);
	if((s[0]=='*') && (s[1]=='/'))return(0);

	if(*s>' ')return(1);
	if(*s==' ')return(1);
	if(*s=='\t')return(1);

	return(0);
}

BASM2_API int BASM_Parse_IsLineBreak(char *se)
{
	char *s, *b, *t;
	int i, j;

	s=se;
	while(*s && (*s<=' '))
	{
		if(*s=='\r')return(1);
		if(*s=='\n')return(1);
		s++;
	}
	if(!*s)return(1);

	return(0);
}


BASM2_API int BASM_Parse_OpChar(int c)
{
	int i;

	switch(c)
	{
	case '~': case '!': case '@': case '#':
	case '$': case '%': case '^': case '&':
	case '*': case '-': case '=': case '+':
	case '\\': case '|':
	case ':':

	case '<': case '>': case '?': case '/':
		i=1;
		break;

	default:
		i=0;
		break;
	}
	return(i);
}

BASM2_API int BASM_Parse_PeekChar(char **str)
{
	unsigned char *s;
	int i;

	s=(unsigned char *)(*str);
	i=*s++;

	if(i>=0xFC)
	{
		i=(i&1)<<30;
		i+=((*s++)&63)<<24;
		i+=((*s++)&63)<<18;
		i+=((*s++)&63)<<12;
		i+=((*s++)&63)<<6;
		i+=(*s++)&63;
	}else if(i>=0xF8)
	{
		i=(i&3)<<24;
		i+=((*s++)&63)<<18;
		i+=((*s++)&63)<<12;
		i+=((*s++)&63)<<6;
		i+=(*s++)&63;
	}else if(i>=0xF0)
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

	return(i);
}

BASM2_API int BASM_Parse_ParseChar(char **str)
{
	unsigned char *s;
	int i;

	s=(unsigned char *)(*str);
	i=*s++;

	if(i>=0xFC)
	{
		i=(i&1)<<30;
		i+=((*s++)&63)<<24;
		i+=((*s++)&63)<<18;
		i+=((*s++)&63)<<12;
		i+=((*s++)&63)<<6;
		i+=(*s++)&63;
	}else if(i>=0xF8)
	{
		i=(i&3)<<24;
		i+=((*s++)&63)<<18;
		i+=((*s++)&63)<<12;
		i+=((*s++)&63)<<6;
		i+=(*s++)&63;
	}else if(i>=0xF0)
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

BASM2_API int BASM_Parse_EmitChar(char **str, int j)
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

int BASM_Parse_MatchTokenListLen(char *str, char **lst)
{
	char *s, *t;

	while(*lst)
	{
		s=str; t=*lst;

		while(*t)
		{
			if((*s)!=(*t))break;
			s++; t++;
		}

		if(!(*t))return(strlen(*lst));
		lst++;
	}
	return(0);
}

BASM2_API char *BASM_Parse_Token(char *s, char *b, int *ty)
{
	static char *token_ops[]={
		"+++", "---", "==", "!=", "<=", ">=", "++", "--",
		"<<", ">>", "(*", "*)", "##", NULL};

	char *t;
	int i, j, k;

	*b=0;
	s=BASM_Parse_EatWhite(s);
	if(!*s)
	{
		*ty=BASM_TOKEN_NULL;
		return(s);
	}
	t=b;

	if(	((*s>='a') && (*s<='z')) ||
		((*s>='A') && (*s<='Z')) ||
		(*s=='_') || (*s=='.') || (*s=='$') || (*s=='@')
		)
	{
		while(
			((*s>='a') && (*s<='z')) ||
			((*s>='A') && (*s<='Z')) ||
			((*s>='0') && (*s<='9')) ||
			(*s=='_') || (*s=='.') ||
			(*s=='$') || (*s=='@')
			)
			*t++=*s++;
		*t++=0;

		*ty=BASM_TOKEN_NAME;
		return(s);
	}

	if((*s>='0') && (*s<='9'))
	{
		if((s[0]=='0') && (s[1]=='x'))
		{
			*t++=*s++;
			*t++=*s++;
			while(((*s>='0') && (*s<='9')) ||
				((*s>='A') && (*s<='F')) ||
				((*s>='a') && (*s<='f')))
			{
				*t++=*s++;
			}
			*t++=0;

			*ty=BASM_TOKEN_NUMBER;
			return(s);
		}

		if(*s=='-')*t++=*s++;
		while(((*s>='0') && (*s<='9')) || (*s=='.') || (*s=='e') ||
			(*s=='b') || (*s=='h') || (*s=='B') || (*s=='H'))
		{
			if(*s=='e')
			{
				*t++=*s++;
				if((*s=='+') || (*s=='-'))
					*t++=*s++;
				continue;
			}
			*t++=*s++;
		}
		*t++=0;

		*ty=BASM_TOKEN_NUMBER;
		return(s);
	}

	if((*s=='"') || (*s=='\'')) /* quoted string */
	{
		if(*s=='\'')i=1;
			else i=0;
		s++;
		while(*s)
		{
			if(!i)if(*s=='\"')break;
			if(i)if(*s=='\'')break;

			if((s[0]=='\\') && (s[1]=='x'))
			{
				k=2; s+=2; j=0;
				while(k--)
				{
					j<<=4;
					if((*s>='0') && (*s<='9'))j+=*s-'0';
					if((*s>='A') && (*s<='F'))j+=*s-'A'+10;
					if((*s>='a') && (*s<='f'))j+=*s-'a'+10;
					s++;
				}

				*t++=j;
//				BASM_Parse_EmitChar(&t, j);
				continue;
			}

			if((s[0]=='\\') &&
				((s[1]=='u') || (s[1]=='U') || (s[1]=='X')))
			{
				k=2;
				if(s[1]=='u')k=4;
				if(s[1]=='U')k=8;
				if(s[1]=='X')k=4;	//raw 16-bit hex value

				s+=2; j=0;
				while(k--)
				{
					j<<=4;
					if((*s>='0') && (*s<='9'))j+=*s-'0';
					if((*s>='A') && (*s<='F'))j+=*s-'A'+10;
					if((*s>='a') && (*s<='f'))j+=*s-'a'+10;
					s++;
				}

				BASM_Parse_EmitChar(&t, j);
				continue;
			}

			if(*s=='\\')
			{
				s++;
				switch(*s)
				{
				case 'a': *t++='\b'; break;
				case 'b': *t++='\b'; break;
				case 'v': *t++='\v'; break;
				case 'f': *t++='\f'; break;
				case 'e': *t++='\x1B'; break;
				case 'n': *t++='\n'; break;
				case 't': *t++='\t'; break;
				case 'r': *t++='\r'; break;
				case '\\': *t++='\\'; break;
				case '\"': *t++='\"'; break;
				case '\'': *t++='\''; break;
				case '?': *t++='?'; break;
				default:
					break;
				}
				s++;
				continue;
			}

//			*t++=*s++;
			j=BASM_Parse_ParseChar(&s);
			BASM_Parse_EmitChar(&t, j);
		}
		*t++=0;
		if(*s)s++;

		*ty=BASM_TOKEN_STRING;
		if(i)*ty=BASM_TOKEN_CHARSTRING;
		return(s);
	}

//	if((*s==',') || (*s==';') || ((*s=='.') && (s[1]!='.')))
	if((*s==',') || (*s==';'))
	{
		*t++=*s++;
		*t++=0;

		*ty=BASM_TOKEN_SEPERATOR;
		return(s);
	}

	if(
		(*s=='[') || (*s==']') ||
		(*s=='{') || (*s=='}') ||
		(*s=='(') || (*s==')'))
	{
		*t++=*s++;
		*t++=0;

		*ty=BASM_TOKEN_BRACE;
		return(s);
	}

	i=BASM_Parse_MatchTokenListLen(s, token_ops);
	if(i)
	{
		strncpy(t, s, i);
		t[i]=0; s+=i;
		*ty=BASM_TOKEN_OPERATOR;
		return(s);
	}

	if(BASM_Parse_OpChar(*s))
	{
		*t++=*s++;
		*t++=0;

		*ty=BASM_TOKEN_OPERATOR;
		if(!strcmp(b, ":"))*ty=BASM_TOKEN_SEPERATOR;

		return(s);
	}

	*ty=BASM_TOKEN_NULL;
	return(NULL);
}







/*
Written 2007-2009 by Brendan G Bohannon
This code is re-assigned into the public domain.
*/

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

struct basm_ppdef_s {
struct basm_ppdef_s *next;
char *name;
char *value;
char **args;
};

char *basm_pp_ibuf;
char *basm_pp_obuf;

struct basm_ppdef_s *basm_pp_sdef;	//static defines
struct basm_ppdef_s *basm_pp_def;	//normal defines

struct basm_ppdef_s *basm_pp_ildef[256];	//include-local defs

int basm_pp_warn, basm_pp_err;

int basm_pp_ilvl;	//include level

int basm_pp_iflvl;	//nested ifdef-level (true branches)
int basm_pp_iflvl2;	//nested ifdef-level (false branch)

char *basm_pp_spos;	//temp source position
char *basm_pp_lfn;	//last file name

char *basm_pp_inmacro;

//includes
char *basm_pp_sinc[16];	//static include paths
int basm_pp_nsinc=0;	//num static include paths

char *basm_pp_inc[16];	//include paths
int basm_pp_ninc;	//num include paths

char *basm_pp_headn[1024];	//virtual header names
char *basm_pp_headv[1024];	//virtual header values
int basm_pp_nhead;		//num virtual headers


// void *basm_loadfile(char *name, int *rsz);
void BASM_PP_Buffer(char *ibuf);

int BASM_PP_AddIncludePathFront(char *str)
{
	int i;

	i=basm_pp_nsinc++;
	basm_pp_sinc[i]=basm_strdup(str);
	return(i);
}

int BASM_PP_AddIncludePathBack(char *str)
{
	int i;

	for(i=basm_pp_nsinc; i>0; i++)
		basm_pp_sinc[i]=basm_pp_sinc[i-1];
	basm_pp_nsinc++;
	basm_pp_sinc[0]=basm_strdup(str);
	return(i);
}

int BASM_PP_AddVirtualHeader(char *name, char *buf)
{
	int i;

	for(i=0; i<basm_pp_nhead; i++)
		if(!strcmp(basm_pp_headn[i], name))
	{
		free(basm_pp_headv[i]);
		basm_pp_headv[i]=strdup(buf);
		return(i);
	}

	i=basm_pp_nhead++;
	basm_pp_headn[i]=basm_strdup(name);
	basm_pp_headv[i]=strdup(buf);
	return(i);
}

char *BASM_PP_LookupVirtualHeader(char *name)
{
	static int hash[256];
	static int init=0;
	char *s;
	int i, hi;

	if(!init)
	{
		memset(hash, 0, 256*sizeof(int));
		init=1;
	}

	hi=0; s=name;
	while(*s)hi=hi*4093+(*s++);
	hi=(hi>>12)&255;

	i=hash[hi];
	if(basm_pp_headn[i] && !strcmp(basm_pp_headn[i], name))
		return(basm_pp_headv[i]);

	for(i=0; i<basm_pp_nhead; i++)
		if(!strcmp(basm_pp_headn[i], name))
	{
		hash[hi]=i;
		return(basm_pp_headv[i]);
	}

	return(NULL);
}

char *BASM_PP_EmitString(char *t, char *s)
{
	int i;

	*t++='\"';
	while(*s)
	{
		i=BASM_Parse_ParseChar(&s);

		if((i<' ') || (i>='~'))
		{
			if(i>0xFF)
			{
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
		BASM_Parse_EmitChar(&t, i);
//		*t++=i;
	}

	*t++='\"';
	*t=0;

	return(t);
}

char *BASM_PP_EmitCharString(char *t, char *s)
{
	int i;

	*t++='\'';
	while(*s)
	{
		i=BASM_Parse_ParseChar(&s);

		if((i<' ') || (i>='~'))
		{
			if(i>0xFF)
			{
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
		BASM_Parse_EmitChar(&t, i);
//		*t++=i;
	}

	*t++='\'';
	*t=0;

	return(t);
}

struct basm_ppdef_s *BASM_PP_LookupDefineLvls(char *name);

struct basm_ppdef_s *BASM_PP_LookupDefine(char *name)
{
	struct basm_ppdef_s *cur;

	cur=BASM_PP_LookupDefineLvls(name);
	if(cur)return(cur);

	cur=basm_pp_def;
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}

	cur=basm_pp_sdef;
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}

	return(NULL);
}

struct basm_ppdef_s *BASM_PP_LookupDefine2(char *name)
{
	struct basm_ppdef_s *cur;

	cur=basm_pp_def;
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}

	return(NULL);
}

struct basm_ppdef_s *BASM_PP_LookupDefineLvl(char *name, int lvl)
{
	struct basm_ppdef_s *cur;

	cur=basm_pp_ildef[lvl];
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}

	return(NULL);
}

struct basm_ppdef_s *BASM_PP_LookupDefineLvls(char *name)
{
	struct basm_ppdef_s *cur;
	int i;

	for(i=basm_pp_ilvl; i>=0; i--)
	{
		cur=BASM_PP_LookupDefineLvl(name, i);
		if(cur)return(cur);
	}

	return(NULL);
}

struct basm_ppdef_s *BASM_PP_LookupDefine3(char *name)
{
	struct basm_ppdef_s *cur;

	cur=BASM_PP_LookupDefineLvls(name);
	if(cur)return(cur);
	cur=BASM_PP_LookupDefine2(name);
	return(cur);
}

void BASM_PP_DeleteDefine(char *name)
{
	struct basm_ppdef_s *cur, *prv;

	cur=basm_pp_def; prv=NULL;
	while(cur)
	{
		if(!strcmp(cur->name, name))
		{
			if(prv)
			{
				prv->next=cur->next;
			}else
			{
				basm_pp_def=cur->next;
			}
			if(cur->args)free(cur->args);
			free(cur->value);
			free(cur);
			return;
		}
		prv=cur; cur=cur->next;
	}
}

void BASM_PP_CleanupDefines()
{
	struct basm_ppdef_s *cur, *nxt;

	cur=basm_pp_def;
	basm_pp_def=NULL;

	while(cur)
	{
		nxt=cur->next;
		if(cur->args)free(cur->args);
		free(cur->value);
		free(cur);
		cur=nxt; 
	}
}

void BASM_PP_CleanupLocalDefines()
{
	struct basm_ppdef_s *cur, *nxt;

	cur=basm_pp_ildef[basm_pp_ilvl];
	basm_pp_ildef[basm_pp_ilvl]=NULL;

	while(cur)
	{
		nxt=cur->next;
		if(cur->args)free(cur->args);
		free(cur->value);
		free(cur);
		cur=nxt; 
	}
}

void BASM_PP_AddDefine(char *name, char *str)
{
	struct basm_ppdef_s *cur;

//	cur=BASM_PP_LookupDefine2(name);
	cur=BASM_PP_LookupDefine3(name);
	if(cur && !cur->args)
	{
		free(cur->value);
		cur->value=strdup(str);
		return;
	}

	cur=malloc(sizeof(struct basm_ppdef_s));
	memset(cur, 0, sizeof(struct basm_ppdef_s));

	cur->name=basm_strdup(name);
	cur->value=strdup(str);

	cur->next=basm_pp_def;
	basm_pp_def=cur;
}

void BASM_PP_AddDefineArgs(char *name, char **args, char *str)
{
	struct basm_ppdef_s *cur;
	int i;

	cur=malloc(sizeof(struct basm_ppdef_s));
	memset(cur, 0, sizeof(struct basm_ppdef_s));

	cur->name=basm_strdup(name);
	cur->value=strdup(str);

	for(i=0; args[i]; i++);
	cur->args=malloc((i+1)*sizeof(char *));
	for(i=0; args[i]; i++)cur->args[i]=args[i];
	cur->args[i]=NULL;

	cur->next=basm_pp_def;
	basm_pp_def=cur;
}

void BASM_PP_AppendDefineLine(char *name, char *str)
{
	char tb[1024];
	struct basm_ppdef_s *cur;

	cur=BASM_PP_LookupDefine2(name);
	if(!cur)return;

	if(!(*cur->value))
	{
		free(cur->value);
		cur->value=strdup(str);
		return;
	}

	sprintf(tb, "%s\n%s", cur->value, str);
	free(cur->value);
	cur->value=strdup(tb);
	return;
}


void BASM_PP_AddStaticDefine(char *name, char *str)
{
	struct basm_ppdef_s *cur;

	cur=malloc(sizeof(struct basm_ppdef_s));
	memset(cur, 0, sizeof(struct basm_ppdef_s));

	cur->name=basm_strdup(name);
	cur->value=strdup(str);

	cur->next=basm_pp_sdef;
	basm_pp_sdef=cur;
}

void BASM_PP_AddStaticDefineArgs(char *name, char **args, char *str)
{
	struct basm_ppdef_s *cur;
	int i;

	cur=malloc(sizeof(struct basm_ppdef_s));
	memset(cur, 0, sizeof(struct basm_ppdef_s));

	cur->name=basm_strdup(name);
	cur->value=strdup(str);

	for(i=0; args[i]; i++);
	cur->args=malloc((i+1)*sizeof(char *));
	for(i=0; args[i]; i++)
		cur->args[i]=args[i];
	cur->args[i]=NULL;

	cur->next=basm_pp_sdef;
	basm_pp_sdef=cur;
}

void BASM_PP_AddLocalDefine(char *name, char *str)
{
	struct basm_ppdef_s *cur;

	cur=BASM_PP_LookupDefineLvl(name, basm_pp_ilvl);
	if(cur && !cur->args)
	{
		free(cur->value);
		cur->value=strdup(str);
		return;
	}

	cur=malloc(sizeof(struct basm_ppdef_s));
	memset(cur, 0, sizeof(struct basm_ppdef_s));

	cur->name=basm_strdup(name);
	cur->value=strdup(str);

	cur->next=basm_pp_ildef[basm_pp_ilvl];
	basm_pp_ildef[basm_pp_ilvl]=cur;
}

void BASM_PP_AddLocalDefineArgs(char *name, char **args, char *str)
{
	struct basm_ppdef_s *cur;
	int i;

	cur=malloc(sizeof(struct basm_ppdef_s));
	memset(cur, 0, sizeof(struct basm_ppdef_s));

	cur->name=basm_strdup(name);
	cur->value=strdup(str);

	for(i=0; args[i]; i++);
	cur->args=malloc((i+1)*sizeof(char *));
	for(i=0; args[i]; i++)cur->args[i]=args[i];
	cur->args[i]=NULL;

	cur->next=basm_pp_ildef[basm_pp_ilvl];
	basm_pp_ildef[basm_pp_ilvl]=cur;
}

char *BASM_PP_LoadInclude(char *name, int *rsz)
{
	char b[256];
	char *buf;
	int i, sz;

	buf=BASM_PP_LookupVirtualHeader(name);
	if(buf)
	{
		sz=strlen(buf);
		buf=strdup(buf);
		if(rsz)*rsz=sz;
		return(buf);
	}

	buf=basm_loadfile(name, &sz);
	if(buf) { if(rsz)*rsz=sz; return(buf); }

	buf=NULL;
	for(i=(basm_pp_ninc-1); i>=0; i--)
	{
		sprintf(b, "%s/%s", basm_pp_inc[i], name);
		buf=basm_loadfile(b, &sz);
		if(buf) { if(rsz)*rsz=sz; return(buf); }
	}

	for(i=(basm_pp_nsinc-1); i>=0; i--)
	{
		sprintf(b, "%s/%s", basm_pp_sinc[i], name);
		buf=basm_loadfile(b, &sz);
		if(buf) { if(rsz)*rsz=sz; return(buf); }
	}

	return(NULL);
}

void BASM_PP_Include(char *str)
{
	char b[256];
	char *buf;
	int i;

	buf=BASM_PP_LookupVirtualHeader(str);
	if(buf)
	{
//		BASM_PP_PushIncludeLevel();
		BASM_PP_Buffer(buf);
//		BASM_PP_PopIncludeLevel();
		return;
	}

	buf=NULL;
	for(i=(basm_pp_ninc-1); i>=0; i--)
	{
		sprintf(b, "%s/%s", basm_pp_inc[i], str);
		buf=basm_loadfile(b, NULL);
		if(buf)break;
	}

	if(!buf)
	{
		for(i=(basm_pp_nsinc-1); i>=0; i--)
		{
			sprintf(b, "%s/%s", basm_pp_sinc[i], str);
			buf=basm_loadfile(b, NULL);
			if(buf)break;
		}
	}

	if(!buf)
	{
		printf("BASM_PP_Directive: fail open include"
			" '%s'\n", str);
		basm_pp_err++;
		return;
	}

//	BASM_PP_PushIncludeLevel();
	BASM_PP_Buffer(buf);
//	BASM_PP_PopIncludeLevel();
	free(buf);
}

void BASM_PP_PushIncludeLevel()
{
	basm_pp_ilvl++;
	basm_pp_ildef[basm_pp_ilvl]=NULL;
}

void BASM_PP_PopIncludeLevel()
{
	BASM_PP_CleanupLocalDefines();
	basm_pp_ilvl--;
}


void BASM_PP_Directive2(char *str)
{
	char b[256], b2[256];
	struct basm_ppdef_s *def;
	int ty, ty2;
	char *s, *t, *s1;
	int i;

	s=str;
	s++;

	s=BASM_Parse_Token(s, b, &ty);
	BASM_Parse_Token(s, b2, &ty2);

	if(!strcmp(b, "if"))		{ basm_pp_iflvl2++; return; }
	if(!strcmp(b, "ifdef"))		{ basm_pp_iflvl2++; return; }
	if(!strcmp(b, "ifndef"))	{ basm_pp_iflvl2++; return; }
	if(!strcmp(b, "endif"))		{ basm_pp_iflvl2--; return; }

	if(!strcmp(b, "else"))
	{
		if(basm_pp_iflvl2==1)
		{
			basm_pp_iflvl2--;
			basm_pp_iflvl++;
			return;
		}
		return;
	}

	if(!strcmp(b, "elif"))
	{
		if(basm_pp_iflvl2==1)
		{
			strcpy(b2, s);
			i=1; while(i)i=BASM_PP_Line(b2);

			s1=b2;
//			n0=BASM_Parse_Expression(&s1);
//			n0=BGBCC_ReduceExpr(NULL, n0);
//			i=BGBCC_BoolExpr(NULL, n0);

			i=0;

			if(i>0)
			{
				basm_pp_iflvl2--;
				basm_pp_iflvl++;
				return;
			}

			return;
		}

		return;
	}
}

void BASM_PP_Directive(char *str)
{
	char b[256], b2[256];
	char *ab[64];
	struct basm_ppdef_s *def;
	char *s, *t, *s1, *s2, *tn;
	int ty, ty2;
	int i, j, n;

	if(basm_pp_iflvl2)
	{
		BASM_PP_Directive2(str);
		return;
	}

	s=str;
	s++;

	s=BASM_Parse_Token(s, b, &ty);
	s1=BASM_Parse_Token(s, b2, &ty2);

	if(!strcmp(b, "include"))
	{
		i=1; strcpy(b, s);
		while(i)i=BASM_PP_Line(b);

		s1=BASM_Parse_Token(b, b2, &ty2);

		if(!strcmp(b2, "<"))
		{
			t=b2;
			while(*s1 && (*s1!='>'))*t++=*s1++;
			*t++=0;

			BASM_PP_Include(b2);
			return;
		}

		t=b2; t+=strlen(t);
		while(*s1)
		{
			s1=BASM_Parse_Token(s1, t, &ty2);
			t+=strlen(t);
		}
		*t=0;

		BASM_PP_Include(b2);
		return;
	}

	if(!strcmp(b, "define"))
	{
		if(*s1!='(')
		{
			while(*s1 && (*s1<=' '))s1++;
			BASM_PP_AddDefine(b2, s1);
			return;
		}

		tn=basm_strdup(b2);

		s1++;
		n=0;
		while(1)
		{
			s1=BASM_Parse_Token(s1, b, &ty);
			if(!strcmp(b, ")"))break;
			if(!strcmp(b, ","))continue;

			ab[n++]=basm_strdup(b);
		}
		ab[n]=NULL;

		while(*s1 && (*s1<=' '))s1++;

		BASM_PP_AddDefineArgs(tn, ab, s1);
		return;
	}

	if(!strcmp(b, "definelocal"))
	{
		if(*s1!='(')
		{
			while(*s1 && (*s1<=' '))s1++;
			BASM_PP_AddLocalDefine(b2, s1);
			return;
		}

		tn=basm_strdup(b2);

		s1++;
		n=0;
		while(1)
		{
			s1=BASM_Parse_Token(s1, b, &ty);
			if(!strcmp(b, ")"))break;
			if(!strcmp(b, ","))continue;

			ab[n++]=basm_strdup(b);
		}
		ab[n]=NULL;

		while(*s1 && (*s1<=' '))s1++;

		BASM_PP_AddLocalDefineArgs(tn, ab, s1);
		return;
	}

	if(!strcmp(b, "endif"))
	{
		basm_pp_iflvl--;
		return;
	}

	if(!strcmp(b, "else"))
	{
		basm_pp_iflvl--;
		basm_pp_iflvl2++;
		return;
	}

	if(!strcmp(b, "elif"))
	{
		basm_pp_iflvl--;
		basm_pp_iflvl2++;
		return;
	}

	if(!strcmp(b, "ifdef"))
	{
		def=BASM_PP_LookupDefine(b2);
		if(!def)basm_pp_iflvl2++;
			else basm_pp_iflvl++;
		return;
	}
	if(!strcmp(b, "ifndef"))
	{
		def=BASM_PP_LookupDefine(b2);
		if(!def)basm_pp_iflvl++;
			else basm_pp_iflvl2++;
		return;
	}

	if(!strcmp(b, "undef"))
	{
		BASM_PP_DeleteDefine(b2);
		return;
	}

	if(!strcmp(b, "if"))
	{
		i=1;
		strcpy(b2, s);
		while(i)i=BASM_PP_Line(b2);

//		s2=b2;
//		n0=BASM_Parse_Expression(&s2);
//		n0=BGBCC_ReduceExpr(NULL, n0);
//		i=BGBCC_BoolExpr(NULL, n0);
//		i=0;

		while(i)i=BASM_PP_LineArith(b2);

		s2=b2; while(*s2 && *s2<=' ')s2++;
		i=atoi(s2);

		if(i>0)basm_pp_iflvl++;
			else basm_pp_iflvl2++;

		return;
	}

	if(!strcmp(b, "assign"))
	{
		strcpy(b2, s1);
		i=1; while(i)i=BASM_PP_Line(b2);
		i=1; while(i)i=BASM_PP_LineArith(b2);
		s2=b2; while(*s2 && (*s2<=' '))s2++;
		BASM_PP_AddDefine(b2, s2);
		return;
	}

	if(!strcmp(b, "assignlocal"))
	{
		strcpy(b2, s1);
		i=1; while(i)i=BASM_PP_Line(b2);
		i=1; while(i)i=BASM_PP_LineArith(b2);
		s2=b2; while(*s2 && (*s2<=' '))s2++;
		BASM_PP_AddLocalDefine(b2, s2);
		return;
	}

	if(!strcmp(b, "macro"))
	{
		tn=basm_strdup(b2);

		n=0;
		while(*s1)
		{
			s1=BASM_Parse_Token(s1, b, &ty);
			if(!strcmp(b, ","))continue;

			ab[n++]=basm_strdup(b);
		}
		ab[n]=NULL;

		BASM_PP_AddDefineArgs(tn, ab, "");
		basm_pp_inmacro=tn;
		return;
	}

	if(!strcmp(b, "endmacro"))
	{
		basm_pp_inmacro=NULL;
		return;
	}

	if(!strcmp(b, "enterscope"))
	{
		BASM_PP_PushIncludeLevel();
		return;
	}

	if(!strcmp(b, "exitscope"))
	{
		BASM_PP_PopIncludeLevel();
		return;
	}


	printf("BASM_PP_Directive: Unsupported preproc directive %s\n", b);
	basm_pp_err++;
}

char *BASM_PP_ParseArg(char *s, char *b)
{
	int i;

	i=0;
	while(*s)
	{
		if(*s=='(') { i++; *b++=*s++; continue; }
		if(*s==')') { if(!i)break; i--; *b++=*s++; continue; }
		if(*s=='[') { i++; *b++=*s++; continue; }
		if(*s==']') { i--; *b++=*s++; continue; }
		if(*s=='{') { i++; *b++=*s++; continue; }
		if(*s=='}') { i--; *b++=*s++; continue; }
		if(*s==',') { if(!i)break; *b++=*s++; continue; }
		*b++=*s++;
	}

	*b++=0;
	return(s);
}

char *BASM_PP_PrintName(char *t, char *s)
{
	int i;

	i=*(t-1);
	if((i==';') || (i=='_') || ((i>='0') && (i<='9')) ||
		((i>='A') && (i<='Z')) || ((i>='a') && (i<='z')))
			*t++=' ';
	strcpy(t, s);
	t+=strlen(s);
	return(t);
}

char *BASM_PP_PrintToken(char *t, char *s, int ty)
{
	if(ty==BASM_TOKEN_NAME)
	{
		t=BASM_PP_PrintName(t, s);
		return(t);
	}

	if(ty==BASM_TOKEN_NUMBER)
	{
		t=BASM_PP_PrintName(t, s);
		return(t);
	}

	if(ty==BASM_TOKEN_STRING)
	{
		t=BASM_PP_EmitString(t, s);
		return(t);
	}

	if(ty==BASM_TOKEN_CHARSTRING)
	{
		t=BASM_PP_EmitCharString(t, s);
		return(t);
	}

	if(!strcmp(s, ";"))
	{
		*t++=';'; *t++=' '; *t=0;
		return(t);
	}

	if(BASM_Parse_OpChar(*s) && BASM_Parse_OpChar(*(t-1)))
		*t++=' ';
	sprintf(t, "%s", s);
	t+=strlen(t);
	return(t);
}

int BASM_PP_Expand(char **src, char **dst, char *op)
{
	char b[256], b2[256];
	char *ab[64];
	char *s, *s1, *t;
	struct basm_ppdef_s *def;
	int i, j, ty, ty2;

	s=*src; t=*dst;

	if(!strcmp(op, "defined"))
	{
		s=BASM_Parse_Token(s, b, &ty);	//<name> | '('
		if(!strcmp(b, "("))
		{
			s=BASM_Parse_Token(s, b, &ty); //<name>
			def=BASM_PP_LookupDefine(b);
			s=BASM_Parse_Token(s, b, &ty); //')'

			sprintf(t, "%d ", def?1:0);
			t+=strlen(t);

			*src=s; *dst=t;
			return(1);
		}

		def=BASM_PP_LookupDefine(b);
		sprintf(t, "%d ", def?1:0);
		t+=strlen(t);

		*src=s; *dst=t;
		return(1);
	}

	def=BASM_PP_LookupDefine(op);
	if(!def)return(0);

	if(!def->args)
	{
		t=BASM_PP_PrintName(t, def->value);
		*src=s; *dst=t;
		return(1);
	}

//	if(*s!='(')return(0);

	if(*s=='(')s++;
	j=0;
	while(def->args[j])
	{
		if(*s==')') break;
		if(*s==',') { s++; continue; }
		while(*s && (*s<=' '))s++;

		s=BASM_PP_ParseArg(s, b2);
		ab[j++]=basm_strdup(b2);
	}
	ab[j]=NULL;

	if(def->args[j] || (*s!=')'))
	{
		printf("BASM_PP_Expand: macro parse problem\n");
		basm_pp_err++;
		return(0);
	}
	s++;

	*src=s;

	s=def->value;
	while(*s)
	{
		s=BASM_Parse_Token(s, b, &ty);

		if(ty==BASM_TOKEN_NAME)
		{
			for(i=0; def->args[i]; i++)
				if(!strcmp(def->args[i], b))
					break;

			if(def->args[i])
			{
				t=BASM_PP_PrintName(t, ab[i]);
				continue;
			}

			t=BASM_PP_PrintName(t, b);
			continue;
		}

		t=BASM_PP_PrintToken(t, b, ty);
	}

	*t=0;

	*dst=t;
	return(1);
}

int BASM_PP_ExpandBinary(char *larg, char *op, char *rarg, char *dst)
{
	float f, g, h;
	int i, j, k;

	f=atof(larg); g=atof(rarg); i=0;
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

int BASM_PP_LineArith(char *str)
{
	char lbuf[4096];
	char b[256], b2[256], b3[256];
	char *s, *s1, *s2, *t, *t1;
	int i, ty, ty2, ty3, ni;

	ni=0;
	s=str; t=lbuf;
	while(*s)
	{
		s=BASM_Parse_Token(s, b, &ty);
		s1=BASM_Parse_Token(s, b2, &ty2);
		s2=BASM_Parse_Token(s1, b3, &ty3);

#if 1
		if((ty==BASM_TOKEN_NUMBER) &&
			(ty2==BASM_TOKEN_OPERATOR) &&
			(ty3==BASM_TOKEN_NUMBER))
		{
			i=BASM_PP_ExpandBinary(b, b2, b3, b2);
			if(i>=0)
			{
				s=s2; t=BASM_PP_PrintName(t, b2);
				ni++;
				continue;
			}

			t=BASM_PP_PrintName(t, b);
			continue;
		}
		if(!strcmp(b, "(") && !strcmp(b3, ")") &&
			(ty2==BASM_TOKEN_NUMBER))
		{
			t=BASM_PP_PrintName(t, b2);
			ni++;
			continue;
		}
#endif

		if(ty==BASM_TOKEN_NAME)
		{
			i=BASM_PP_Expand(&s, &t, b);
			if(i>0)
			{
				ni++;
				continue;
			}

			t=BASM_PP_PrintName(t, b);
			continue;
		}

		t=BASM_PP_PrintToken(t, b, ty);
	}
	*t=0;

	strcpy(str, lbuf);
	return(ni);
}

int BASM_PP_Expand2(char *src, char *dst)
{
	char lbuf[4096];
	int i, ni;

	strcpy(lbuf, src);
	i=1; ni=0;
	while(i>0)
	{
		i=BASM_PP_Line(lbuf);
		if(i>0)ni++;
	}
	strcpy(dst, lbuf);
	return(ni);
}

int BASM_PP_Line(char *str)
{
	char lbuf[4096];
	char b[256], b2[256];
	char *s, *t, *t1;
	int i, ty, ty2, ty3, ni;

	ni=0;
	s=str; t=lbuf;
	while(*s && (*s<=' '))
		*t++=*s++;
	while(*s)
	{
		while(*s && (*s<=' ') && (*s!='\n'))s++;
		if(*s=='\n') { *t++=*s++; *t=0; continue; }

		if((s[0]=='$') && (s[1]=='('))
		{
			s+=2; t1=b; i=1;
			while(*s && i)
			{
				if(*s=='(')
					{ *t1++=*s++; i++; continue; }
				if(*s==')')
					{ i--; if(i>0)*t1++=*s++; continue; }
				*t1++=*s++;
			}
			*t1++=0;

			i=BASM_PP_Expand2(b, b2);
			if(i>0) { strcpy(b2, t); ni++; }
			continue;
		}

		s=BASM_Parse_Token(s, b, &ty);
		BASM_Parse_Token(s, b2, &ty2);

		if((ty2==BASM_TOKEN_OPERATOR) &&
			(!strcmp(b2, "##") || !strcmp(b, "%:%:")))
		{
			if(ty==BASM_TOKEN_NAME)
			{
				t1=t;
				i=BASM_PP_Expand(&s, &t1, b);
				if(i>0) { strcpy(b, t); ni++; }
			}

			s=BASM_Parse_Token(s, b2, &ty2);	//'##'

			s=BASM_Parse_Token(s, b2, &ty2);

			if(ty2==BASM_TOKEN_NAME)
			{
				t1=t;
				i=BASM_PP_Expand(&s, &t1, b2);
				if(i>0) { strcpy(b2, t); ni++; }
			}

			strcat(b, b2);
			t=BASM_PP_PrintName(t, b);
			continue;
		}

		if(ty==BASM_TOKEN_NAME)
		{
			i=BASM_PP_Expand(&s, &t, b);
			if(i>0)
			{
				ni++;
				continue;
			}

			t=BASM_PP_PrintName(t, b);
			continue;
		}

		if((ty==BASM_TOKEN_OPERATOR) &&
			(!strcmp(b, "#") || !strcmp(b, "%:")))
				if(ty2==BASM_TOKEN_NAME)
		{
			s=BASM_Parse_Token(s, b2, &ty2);

			t1=t;
			i=BASM_PP_Expand(&s, &t1, b2);
			if(i>0) { strcpy(b2, t); ni++; }

			t=BASM_PP_PrintToken(t, b2, BASM_TOKEN_STRING);
			continue;
		}

		t=BASM_PP_PrintToken(t, b, ty);
	}
	*t=0;

	strcpy(str, lbuf);
	return(ni);
}

char *BASM_PP_ParseLine(char *s, char *b)
{
	static int is=0;
	char *t;

	t=b;
//	while(*s && (*s!='\n') && (*s!='\r'))
	while(*s)
	{

		if(*s=='\"')
		{
			is=!is;
			*t++=*s++;
			continue;
		}

		if(is)
		{
			if(is && (s[0]=='\\') && (s[1]=='\"'))
				{ *t++=*s++; *t++=*s++; continue; }
			if(is && (s[0]=='\\') && (s[1]=='\\'))
				{ *t++=*s++; *t++=*s++; continue; }

			*t++=*s++;
			continue;
		}

		if(s[0]<=' ')
		{
			if((*s=='\n') || (*s=='\r'))
				break;

//			if((s[0] && (s[0]<=' ')) && (s[1]==';'))
//			if((s[0]<=' ') && (s[1]==';'))
			if(s[1]==';')
			{
				while(*s && (*s!='\n') && (*s!='\r'))
					s++;
				break;
			}

			*t++=*s++;
			continue;
		}

		if(s[0]=='/')
		{
			if((s[0]=='/') && (s[1]=='/'))
			{
				while(*s && (*s!='\n') && (*s!='\r'))
					s++;
				break;
			}

			if((s[0]=='/') && (s[1]=='*'))
			{
				s+=2;
//				while(*s && (*s!='\n') && (*s!='\r'))
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

		if((*s=='\\') && ((s[1]=='\r') || (s[1]=='\n')))
		{
			s++;
			while(*s && (*s<=' '))s++;
			*t++=' ';
			continue;
		}
		*t++=*s++;
	}
	*t++=0;
	if(*s=='\r')s++;
	if(*s=='\n')s++;

	return(s);
}


void BASM_PP_Buffer(char *ibuf)
{
	char b[4096];
	char *s, *t, *s1;
	int i;

	s=ibuf;
	while(*s)
	{
		basm_pp_spos=s;
		s=BASM_PP_ParseLine(s, b);

		t=b; while(*t && (*t<=' '))t++;
		if(!*t)
		{
//			sprintf(basm_pp_obuf, "\n");
//			basm_pp_obuf+=strlen(basm_pp_obuf);

			*basm_pp_obuf=0;
			continue;
		}

		if((b[0]=='#') && (b[1]=='!'))
		{
			*basm_pp_obuf=0;
			continue;
		}

		if((b[0]=='#') && (b[1]=='#'))
		{
			if(basm_pp_inmacro)
			{
				if(basm_pp_iflvl2)continue;
				BASM_PP_AppendDefineLine(
					basm_pp_inmacro, t+1);
				continue;
			}else
			{
				//intended to be passed through
				sprintf(basm_pp_obuf, "%s\n", t+1);
				basm_pp_obuf+=strlen(basm_pp_obuf);
				continue;
			}
		}

		if(b[0]=='#')
		{
			BASM_PP_Directive(b);
			continue;
		}
		if(b[0]=='%')
		{
			BASM_PP_Directive(b);
			continue;
		}

		if(basm_pp_iflvl2)continue;

		if(basm_pp_inmacro)
		{
			BASM_PP_AppendDefineLine(basm_pp_inmacro, b);
			continue;
		}

		i=1;
		while(i)i=BASM_PP_Line(b);

#if 1
		//special case: macros expand to multiple lines or
		//preprocessor directives
		s1=b; while(*s1 && (*s1<=' '))s1++;
		if(*s1!='#') { while(*s1 && (*s1!='\n'))s1++; }
		if(*s1)
		{
			BASM_PP_Buffer(b);
			continue;
		}
#endif

		sprintf(basm_pp_obuf, "%s\n", b);
		basm_pp_obuf+=strlen(basm_pp_obuf);
	}
}


int BASM_PP_Filter(char *ibuf, char *obuf)
{
	char tb[256];
	char *s, *t;
	int i;


	for(i=0; i<basm_pp_nsinc; i++)
		basm_pp_inc[i]=basm_pp_sinc[i];
	basm_pp_ninc=basm_pp_nsinc;

	basm_pp_ibuf=ibuf;
	basm_pp_obuf=obuf;

	basm_pp_def=NULL;
	basm_pp_ilvl=0;
	basm_pp_ildef[0]=NULL;

	basm_pp_warn=0;
	basm_pp_err=0;

	BASM_PP_Buffer(ibuf);

	BASM_PP_CleanupDefines();

	return(0);
}

void ccAddInclude(char *path)
{
	int i;

	printf("Add include '%s'\n", path);
	BASM_PP_AddIncludePathFront(path);
}

void ccAddDefineString(char *str)
{
	char tb[256], tb2[256];
	char *s, *t;
	
	s=str; t=tb;
	while(*s && (*s!='='))
		*t++=*s++;
	*t++=0;

	if(*s=='=')s++;

	t=tb2;
	while(*s)
		*t++=*s++;
	*t++=0;

//	BASM_PP_AddStaticDefine(NULL, tb, tb2);
	BASM_PP_AddStaticDefine(tb, tb2);
}

int main(int argc, char **argv)
{
	char tb[256];
	char *uds[64], *args[256];
	FILE *fd;
	char *metafn, *oext;
	void *p;
	char *ibuf, *obuf;
	char *s, *t;
	int n, m, nargs, cmd;
	int t0, dt, te, sz;
	int i;

//	dy_init(&argc);

	metafn=NULL;
	oext=NULL;

	nargs=0;
	args[nargs++]=argv[0];

	n=0; m=0; cmd=0;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='/')
		{
			if(!strncmp(argv[i], "/I", 2))
			{
				ccAddInclude(argv[i]+2);
				continue;
			}

#if 0
			if(!strncmp(argv[i], "/L", 2))
			{
				ccAddLibrary(argv[i]+2);
				continue;
			}
			if(!strncmp(argv[i], "/S", 2))
			{
				ccAddSource(argv[i]+2);
				continue;
			}
#endif

			if(!strncmp(argv[i], "/D", 2))
			{
				ccAddDefineString(argv[i]+2);
				continue;
			}

			if(!strncmp(argv[i], "/Fn", 3))
			{
				metafn=argv[i]+3;
				continue;
			}

			if(!strncmp(argv[i], "/FZx", 4))
			{
				m|=16;
				continue;
			}

			if(!strncmp(argv[i], "/Fx", 3))
			{
				cmd=1;
				oext=argv[i]+3;
				continue;
			}

			continue;
		}

		uds[n++]=argv[i];
	}

	nargs=0; args[nargs++]=argv[0];
	for(; i<argc; i++)args[nargs++]=argv[i];

	for(i=0; i<basm_pp_nsinc; i++)
		basm_pp_inc[i]=basm_pp_sinc[i];
	basm_pp_ninc=basm_pp_nsinc;

	if(cmd==0)
	{
		obuf=malloc(1<<24);

//		basm_pp_ibuf=ibuf;
		basm_pp_obuf=obuf;

		basm_pp_def=NULL;
		basm_pp_ilvl=0;
		basm_pp_ildef[0]=NULL;

		basm_pp_warn=0;
		basm_pp_err=0;

//		BASM_PP_Buffer(ibuf);

		for(i=0; i<n; i++)
		{
			ibuf=BASM_PP_LoadInclude(uds[i], &sz);
			if(!ibuf)
			{
				printf("Can't read %s\n", uds[i]);
				continue;
			}
			BASM_PP_Buffer(ibuf);
		}

		BASM_PP_CleanupDefines();

		if(!metafn)
		{
			printf("%s\n", obuf);
		}else
		{
			fd=fopen(metafn, "wt");
			if(fd)
			{
				fwrite(obuf, 1, strlen(obuf), fd);
//				fprintf(fd, "%s", obuf);
				fclose(fd);
			}
		}
	}

	if(cmd==1)
	{
//		BASM_PP_Buffer(ibuf);

		obuf=malloc(1<<24);

		for(i=0; i<n; i++)
		{
//			basm_pp_ibuf=ibuf;
			basm_pp_obuf=obuf;

			basm_pp_def=NULL;
			basm_pp_ilvl=0;
			basm_pp_ildef[0]=NULL;

			basm_pp_warn=0;
			basm_pp_err=0;

			ibuf=BASM_PP_LoadInclude(uds[i], &sz);
			if(!ibuf)
			{
				printf("Can't read %s\n", uds[i]);
				continue;
			}
			BASM_PP_Buffer(ibuf);

			BASM_PP_CleanupDefines();

			strcpy(tb, uds[i]);
			t=tb+strlen(tb);
			while((t>tb) && (*t!='.') && (*t!='/'))t--;
			
			if(*t=='.')
			{
				strcpy(t+1, oext);
				fd=fopen(tb, "wt");
				if(fd)
				{
					fwrite(obuf, 1, strlen(obuf), fd);
					fclose(fd);
				}
			}
		}
	}

	return(0);
}
