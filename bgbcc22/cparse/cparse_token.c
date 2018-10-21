#include <bgbccc.h>

char *bgbcp_pbase;
char *bgbcp_fname;
int bgbcp_linenum=1;

char *bgbcp_pbasestack[256];
char *bgbcp_fnamestack[256];
int bgbcp_lnumstack[256];
int bgbcp_lnumstackpos;

char *bgbcp_csfn;
int bgbcp_csln;

char *bgbcp_lfn;
int bgbcp_lln;

int BGBCP_GetLinenum()
	{ return(bgbcp_linenum); }

char *BGBCP_GetFilename()
	{ return(bgbcp_fname); }

int BGBCP_CalcLinenum(char *se)
{
	char *s;
	int i;

	if(!bgbcp_pbase)
		return(0);

	if(bgbcp_csfn && (bgbcp_csfn<=se))
	{
		s=bgbcp_csfn;
		i=bgbcp_csln;
	}else
	{
		s=bgbcp_pbase;
		i=bgbcp_linenum;
	}

	while(s<se)
	{
		if(*s=='\n')i++;
		if((s[0]=='\r') && (s[1]!='\n'))i++;
		s++;
	}

	bgbcp_csfn=se;
	bgbcp_csln=i;

	return(i);
}

int BGBCP_SetLinenum(char *fname, char *base, int num)
{
	bgbcp_fname=bgbcc_strdup(fname);
	bgbcp_pbase=base;
	bgbcp_linenum=num;
	bgbcp_csfn=NULL;
	return(num);
}

int BGBCP_PushLinenum()
{
	if(bgbcp_lnumstackpos>=255)
		return(-1);

	bgbcp_csfn=NULL;

	bgbcp_lnumstack[bgbcp_lnumstackpos]=
		bgbcp_linenum;
	bgbcp_pbasestack[bgbcp_lnumstackpos]=
		bgbcp_pbase;
	bgbcp_fnamestack[bgbcp_lnumstackpos]=
		bgbcp_fname;
	bgbcp_lnumstackpos++;
	return(0);
}

int BGBCP_PopLinenum()
{
	if(bgbcp_lnumstackpos<=0)
		return(-1);

	bgbcp_csfn=NULL;

	bgbcp_lnumstackpos--;
	bgbcp_linenum=
		bgbcp_lnumstack[bgbcp_lnumstackpos];
	bgbcp_pbase=
		bgbcp_pbasestack[bgbcp_lnumstackpos];
	bgbcp_fname=
		bgbcp_fnamestack[bgbcp_lnumstackpos];
	return(bgbcp_linenum);
}

int BGBCP_SkimLinenum(char *s, char **rfn, int *rln)
{
	static char b[64];
	char *fn, *t;
	int ln;

	if(rfn)*rfn=NULL;
	if(rln)*rln=-1;

	fn=NULL; ln=-1;

	while(*s && (*s<=' '))s++;
	if((s[0]!='/') || (s[1]!='*'))
		return(0);

	s+=2;
	if(*s=='\"')
	{
		s++;
		t=b;
		while(*s && (*s!='\"'))*t++=*s++;
		*t=0;

		if(*s!='\"')return(0);
		s++;

		fn=bgbcc_strdup(b);
	}

	t=b;
	while(*s && (*s>='0') && (*s<='9'))
		*t++=*s++;
	*t=0;

	if(!b[0])return(0);

	ln=atoi(b);

	if((s[0]!='*') || (s[1]!='/'))
		return(0);

	if(rfn)*rfn=fn;
	if(rln)*rln=ln;
	return(1);
}

char *BGBCP_GetLastFileName()
	{ return(bgbcp_lfn); }

int BGBCP_GetLastLineNumber()
	{ return(bgbcp_lln); }


char *BGBCP_EatWhiteOnly(char *s)
{
	if(!s)
		return(s);

	while(*s && (*s<=' '))s++;
	return(s);
}

char *BGBCP_EatWhiteOnly2(char *s)
{
	if(!s)
		return(s);

	while(*s && (*s<=' ') && (*s!='\r') && (*s!='\n'))s++;
	return(s);
}

char *BGBCP_EatWhite(char *s)
{
	char *fn;
	int ln;
	int i;

	if(!s)
		return(s);

	while(*s && (*s<=' '))s++;

	if(s[0]=='/')
	{
//		if((s[0]=='/') && (s[1]=='/'))
		if(s[1]=='/')
		{
			while(*s && (*s!='\n'))s++;
			s=BGBCP_EatWhite(s);
			return(s);
		}
//		if((s[0]=='/') && (s[1]=='*'))
		if(s[1]=='*')
		{
			i=BGBCP_SkimLinenum(s, &fn, &ln);
			if(i)
			{
				if(fn)bgbcp_lfn=fn;
				bgbcp_lln=ln;
			}

#if 1
			s+=2; i=1;
			while(*s)
			{
				if(*s=='*')
				{
					s++;
					if(*s=='/')
					{
						s++; i--;
						if(!i)break;
						continue;
					}
					continue;
				}else if(*s=='/')
				{
					s++;
					if(*s=='*')
					{
						s++; i++;
						continue;
					}
					continue;
				}
				s++;
//				else if(*s) { s++; continue; }
//				else break;
			}
#endif

#if 0
			s+=2;
			i=1;
			while(1)
			{
				if((s[0]=='*') && (s[1]=='/'))
				{
					s+=2;
					i--;
					if(!i)break;
					continue;
				}
				if((s[0]=='/') && (s[1]=='*'))
				{
					s+=2;
					i++;
					continue;
				}
				if(!*s)break;
				s++;
			}
#endif
			s=BGBCP_EatWhite(s);
			return(s);
		}
	}

#if 0
	if(s[0]=='#')
	{
		while(*s && (*s!='\n'))s++;
		s=BGBCP_EatWhite(s);
		return(s);
	}
#endif

	return(s);
}


char *BGBCP_EatWhiteNLb(char *s)
{
	char *fn;
	int ln;
	int i;

	if(!s)
		return(s);

	while(*s && (*s<=' ') && (*s!='\r') && (*s!='\n'))s++;

	if(s[0]=='/')
	{
		if(s[1]=='/')
		{
			while(*s && (*s!='\n'))s++;
//			s=BGBCP_EatWhiteNLb(s);
			return(s);
		}
		if(s[1]=='*')
		{
			i=BGBCP_SkimLinenum(s, &fn, &ln);
			if(i)
			{
				if(fn)bgbcp_lfn=fn;
				bgbcp_lln=ln;
			}

#if 1
			s+=2; i=1;
			while(*s)
			{
				if(*s=='*')
				{
					s++;
					if(*s=='/')
					{
						s++; i--;
						if(!i)break;
						continue;
					}
					continue;
				}else if(*s=='/')
				{
					s++;
					if(*s=='*')
					{
						s++; i++;
						continue;
					}
					continue;
				}
				s++;
			}
#endif
			s=BGBCP_EatWhiteNLb(s);
			return(s);
		}
	}

	return(s);
}

int BGBCP_IsWhiteOnly(char *s)
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

int BGBCP_IsLineBreak(char *se)
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


int BGBCP_OpChar(int c)
{
	int i;

	switch(c)
	{
	case '~':
	case '!':
	case '@':
	case '#':
//	case '$':
	case '%':
	case '^':
	case '&':
	case '*':
	case '-':
	case '=':
	case '+':

	case '\\':
	case '|':

	case ':':

	case '<':
	case '>':
	case '?':
	case '/':

	case '`':
	case '.':
		i=1;
		break;

	default:
		i=0;
		break;
	}
	return(i);
}

int BGBCP_PeekChar(char **str)
{
	unsigned char *s;
	int i;

	s=(unsigned char *)(*str);
	i=*s++;

	if(!(i&0x80))return(i);

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

#if 0
int bgbcp_strcmp_chk4(char *s1, char *s2)
{
	u32 a, b, c;
	a=*(u32 *)s1;
	b=*(u32 *)s2;
	c=a-b;
	if(c)return(1);
	if((a-0x01010101)&0x80808080U)
		return(1);
	return(0);

#if 0
	c=	((a&0x000000FFU)-1)|
		((a&0x0000FF00U)-1)|
		((a&0x00FF0000U)-1)|
		((a&0xFF000000U)-1);
	return(c>>31);
//	return((c>>24)==0xFF);
#endif
}
#endif

int bgbcp_strcmp(char *s1, char *s2)
{
//	u32 a, b, c;

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

#if 0
	while(1)
	{
		a=*(u32 *)s1;
		b=*(u32 *)s2;
		if(a-b)break;
		if((a-0x01010101)&0x80808080U)
			break;
		s1+=4; s2+=4;
	}
#endif

#if 1
//	while(!bgbcp_strcmp_chk4(s1, s2))
//		{ s1+=4; s2+=4; }
	while(*s1 && (*s1==*s2))
		{ s1++; s2++; }
	return(*s1-*s2);
#endif

#if 0
	while(1)
	{
		if(!s1[0] || (s1[0]!=s2[0]))
			{ return(s1[0]-s2[0]); }
		if(!s1[1] || (s1[1]!=s2[1]))
			{ return(s1[1]-s2[1]); }
		if(!s1[2] || (s1[2]!=s2[2]))
			{ return(s1[2]-s2[2]); }
		if(!s1[3] || (s1[3]!=s2[3]))
			{ return(s1[3]-s2[3]); }
		s1+=4; s2+=4;
	}
#endif
}

int bgbcp_strcmp1(char *s1, char *s2)
{
	return((*(s16 *)s1)-(*(s16 *)s2));
}

int bgbcp_strcmp2(char *s1, char *s2)
{
	int i;
	i=(*(s16 *)s1)-(*(s16 *)s2);
	if(i)return(i);
	return(s1[2]-s2[2]);
}

int bgbcp_strcmp3(char *s1, char *s2)
{
	return((*(s32 *)s1)-(*(s32 *)s2));
}

int bgbcp_strcmp4(char *s1, char *s2)
{
	int i;
	i=(*(s32 *)s1)-(*(s32 *)s2);
	if(i)return(i);
	return(s1[4]-s2[4]);
}

int bgbcp_strcmp5(char *s1, char *s2)
{
	int i;
	i=(*(s32 *)s1)-(*(s32 *)s2);
	if(i)return(i);
	s1+=4; s2+=4;
	return((*(s16 *)s1)-(*(s16 *)s2));
}

int bgbcp_strcmp6(char *s1, char *s2)
{
	int i;
	i=(*(s32 *)s1)-(*(s32 *)s2);
	if(i)return(i);
	s1+=4; s2+=4;
	i=(*(s16 *)s1)-(*(s16 *)s2);
	if(i)return(i);
	return(s1[2]-s2[2]);
}

int bgbcp_strcmp7(char *s1, char *s2)
{
	int i;
	i=(*(s32 *)s1)-(*(s32 *)s2);
	if(i)return(i);
	s1+=4; s2+=4;
	return((*(s32 *)s1)-(*(s32 *)s2));
}


int bgbcp_strcmp8(char *s1, char *s2)
{
	int i;
	i=(*(s32 *)s1)-(*(s32 *)s2);
	if(i)return(i);
	s1+=4; s2+=4;
	i=(*(s32 *)s1)-(*(s32 *)s2);
	if(i)return(i);
	return(s1[4]-s2[4]);
}

int bgbcp_strcmp9(char *s1, char *s2)
{
	int i;
	i=(*(s32 *)s1)-(*(s32 *)s2);
	if(i)return(i);
	s1+=4; s2+=4;
	i=(*(s32 *)s1)-(*(s32 *)s2);
	if(i)return(i);
	s1+=4; s2+=4;
	return((*(s16 *)s1)-(*(s16 *)s2));
}

int bgbcp_strcmp10(char *s1, char *s2)
{
	int i;
	i=(*(s32 *)s1)-(*(s32 *)s2);
	if(i)return(i);
	s1+=4; s2+=4;
	i=(*(s32 *)s1)-(*(s32 *)s2);
	if(i)return(i);
	s1+=4; s2+=4;
	i=(*(s16 *)s1)-(*(s16 *)s2);
	if(i)return(i);
	return(s1[2]-s2[2]);
}

int bgbcp_strcmp11(char *s1, char *s2)
{
	int i;
	i=(*(s32 *)s1)-(*(s32 *)s2);
	if(i)return(i);
	s1+=4; s2+=4;
	i=(*(s32 *)s1)-(*(s32 *)s2);
	if(i)return(i);
	s1+=4; s2+=4;
	return((*(s32 *)s1)-(*(s32 *)s2));
}


int bgbcp_strncmp2(char *s1, char *s2)
{
	return((*(s16 *)s1)-(*(s16 *)s2));
}

int bgbcp_strncmp3(char *s1, char *s2)
{
	int i;
	i=(*(s16 *)s1)-(*(s16 *)s2);
	if(i)return(i);
	return(s1[2]-s2[2]);
}

int bgbcp_strncmp4(char *s1, char *s2)
{
	return((*(s32 *)s1)-(*(s32 *)s2));
}

int bgbcp_strncmp5(char *s1, char *s2)
{
	int i;
	i=(*(s32 *)s1)-(*(s32 *)s2);
	if(i)return(i);
	return(s1[4]-s2[4]);
}


int BGBCP_StrlenUTF8(char *str)
{
	char *s;
	int n;
	
	s=str; n=0;
	while(*s)
	{
		n++;
		BGBCP_ParseChar(&s);
	}
	return(n);
}

int BGBCP_StrlenUCS2(u16 *str)
{
	u16 *s;
	int n;

	s=str; n=0;
	while(*s)
		{ s++; n++; }
	return(n);
}

int BGBCP_StrcmpUCS2(u16 *stra, u16 *strb)
{
	u16 *sa, *sb;
	int n;

	sa=stra; sb=strb;
	while(*sa && (*sa==*sb))
		{ sa++; sb++; }
	return(*sa-*sb);
}

int BGBCP_ParseChar(char **str)
{
	unsigned char *s;
	int i;

	s=(unsigned char *)(*str);
	i=*s++;

	if(!(i&0x80))
	{
		*str=(char *)s;
		return(i);
	}

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
		BGBCC_DBGBREAK
//		continue;
	}

	*str=(char *)s;
	return(i);
}

int BGBCP_EmitChar(char **str, int j)
{
	unsigned char *t;
	int i;

	t=(unsigned char *)(*str);

	if(j<128)
	{
		if(j<=0) //Modified-UTF8
			{*t++=0xC0; *t++=0x80;}
			else *t++=j;
		*str=(char *)t;
		return(0);
	}

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

//	}else *t++=j;

	*str=(char *)t;
	return(0);
}

int BGBCP_NameInitChar(int c, int lang)
{
	static int vals[]={
	0x00AA, 0x00BA, 0x207F, 0x0386, 0x038C, 0x03DA, 0x03DC, 0x03DE,
	0x03E0, 0x1F59, 0x1F5B, 0x1F5D, 0x05BF, 0x09B2, 0x0A02, 0x0A5E,
	0x0A74, 0x0A8D, 0x0AD0, 0x0AE0,
	0x0B9C, 0x0CDE, 0x0E84, 0x0E8A, 0x0E8D, 0x0EA5, 0x0EA7, 0x0EC6,
	0x0F00, 0x0F35, 0x0F37, 0x0F39, 0x0F97, 0x0FB9, 0x00B5, 0x00B7,
	0x02BB, 0x037A, 0x0559, 0x093D, 0x0B3D, 0x1FBE, 0x2102, 0x2107,
	0x2115, 0x2124,	0x2126, 0x2128, 0}; 

	static int rngs[]={
	0x0388, 0x038A, 0x038E, 0x03A1, 0x03A3, 0x03CE, 0x03D0, 0x03D6,
	0x03E2, 0x03F3, 0x1F00, 0x1F15, 0x1F18, 0x1F1D, 0x1F20, 0x1F45,
	0x1F48, 0x1F4D, 0x1F50, 0x1F57, 0x1F5F, 0x1F7D, 0x1F80, 0x1FB4,
	0x1FB6, 0x1FBC, 0x1FC2, 0x1FC4, 0x1FC6, 0x1FCC, 0x1FD0, 0x1FD3,
	0x1FD6, 0x1FDB, 0x1FE0, 0x1FEC, 0x1FF2, 0x1FF4, 0x1FF6, 0x1FFC,
	0x0401, 0x040C, 0x040E, 0x044F, 0x0451, 0x045C, 0x045E, 0x0481,
	0x0490, 0x04C4, 0x04C7, 0x04C8, 0x04CB, 0x04CC, 0x04D0, 0x04EB,
	0x04EE, 0x04F5, 0x04F8, 0x04F9, 0x0531, 0x0556, 0x0561, 0x0587,
	0x05B0, 0x05B9, 0x05BB, 0x05BD, 0x05C1, 0x05C2, 0x05D0, 0x05EA,
	0x05F0, 0x05F2, 0x0621, 0x063A, 0x0640, 0x0652, 0x0670, 0x06B7,
	0x06BA, 0x06BE, 0x06C0, 0x06CE, 0x06D0, 0x06DC, 0x06E5, 0x06E8,
	0x06EA, 0x06ED, 0x0901, 0x0903, 0x0905, 0x0939, 0x093E, 0x094D,
	0x0950, 0x0952, 0x0958, 0x0963, 0x0981, 0x0983, 0x0985, 0x098C,
	0x098F, 0x0990, 0x0993, 0x09A8, 0x09AA, 0x09B0, 0x09B6, 0x09B9,
	0x09BE, 0x09C4, 0x09C7, 0x09C8, 0x09CB, 0x09CD, 0x09DC, 0x09DD,
	0x09DF, 0x09E3, 0x09F0, 0x09F1, 0x0A05, 0x0A0A, 0x0A0F, 0x0A10,
	0x0A13, 0x0A28, 0x0A2A, 0x0A30, 0x0A32, 0x0A33, 0x0A35, 0x0A36,
	0x0A38, 0x0A39, 0x0A3E, 0x0A42, 0x0A47, 0x0A48, 0x0A4B, 0x0A4D,
	0x0A59, 0x0A5C, 0x0A81, 0x0A83, 0x0A85, 0x0A8B, 0x0A8F, 0x0A91,
	0x0A93, 0x0AA8, 0x0AC7, 0x0AC9, 0x0ACB, 0x0ACD, 0x00C0, 0x00D6,
	0x00D8, 0x00F6, 0x00F8, 0x01F5, 0x01FA, 0x0217, 0x0250, 0x02A8,
	0x1E00, 0x1E9B, 0x1EA0, 0x1EF9, 0x0AAA, 0x0AB0, 0x0AB2, 0x0AB3,
	0x0AB5, 0x0AB9, 0x0ABD, 0x0AC5,
	0x0B01, 0x0B03, 0x0B05, 0x0B0C, 0x0B0F, 0x0B10, 0x0B13, 0x0B28,
	0x0B2A, 0x0B30, 0x0B32, 0x0B33, 0x0B36, 0x0B39, 0x0B3E, 0x0B43,
	0x0B47, 0x0B48, 0x0B4B, 0x0B4D, 0x0B5C, 0x0B5D, 0x0B5F, 0x0B61,
	0x0B82, 0x0B83, 0x0B85, 0x0B8A, 0x0B8E, 0x0B90, 0x0B92, 0x0B95,
	0x0B99, 0x0B9A, 0x0B9E, 0x0B9F, 0x0BA3, 0x0BA4, 0x0BA8, 0x0BAA,
	0x0BAE, 0x0BB5, 0x0BB7, 0x0BB9, 0x0BBE, 0x0BC2, 0x0BC6, 0x0BC8,
	0x0BCA, 0x0BCD, 0x0C01, 0x0C03, 0x0C05, 0x0C0C, 0x0C0E, 0x0C10,
	0x0C12, 0x0C28, 0x0C2A, 0x0C33, 0x0C35, 0x0C39, 0x0C3E, 0x0C44,
	0x0C46, 0x0C48, 0x0C4A, 0x0C4D, 0x0C60, 0x0C61, 0x0C82, 0x0C83,
	0x0C85, 0x0C8C, 0x0C8E, 0x0C90, 0x0C92, 0x0CA8, 0x0CAA, 0x0CB3,
	0x0CB5, 0x0CB9, 0x0CBE, 0x0CC4, 0x0CC6, 0x0CC8, 0x0CCA, 0x0CCD,
	0x0CE0, 0x0CE1, 0x0D02, 0x0D03, 0x0D05, 0x0D0C, 0x0D0E, 0x0D10,
	0x0D12, 0x0D28, 0x0D2A, 0x0D39, 0x0D3E, 0x0D43, 0x0D46, 0x0D48,
	0x0D4A, 0x0D4D, 0x0D60, 0x0D61, 0x0E01, 0x0E3A, 0x0E40, 0x0E5B,
	0x0E81, 0x0E82, 0x0E87, 0x0E88, 0x0E94, 0x0E97, 0x0E99, 0x0E9F,
	0x0EA1, 0x0EA3, 0x0EAA, 0x0EAB, 0x0EAD, 0x0EAE, 0x0EB0, 0x0EB9,
	0x0EBB, 0x0EBD, 0x0EC0, 0x0EC4, 0x0EC8, 0x0ECD, 0x0EDC, 0x0EDD,
	0x0F18, 0x0F19, 0x0F3E, 0x0F47, 0x0F49, 0x0F69, 0x0F71, 0x0F84,
	0x0F86, 0x0F8B, 0x0F90, 0x0F95, 0x0F99, 0x0FAD, 0x0FB1, 0x0FB7,
	0x10A0, 0x10C5, 0x10D0, 0x10F6, 0x3041, 0x3093, 0x309B, 0x309C,
	0x30A1, 0x30F6, 0x30FB, 0x30FC, 0x3105, 0x312C, 0x4E00, 0x9FA5,
	0xAC00, 0xD7A3, 0x0660, 0x0669, 0x06F0, 0x06F9, 0x0966, 0x096F,
	0x09E6, 0x09EF, 0x0A66, 0x0A6F, 0x0AE6, 0x0AEF, 0x0B66, 0x0B6F,
	0x0BE7, 0x0BEF, 0x0C66, 0x0C6F, 0x0CE6, 0x0CEF, 0x0D66, 0x0D6F,
	0x0E50, 0x0E59, 0x0ED0, 0x0ED9, 0x0F20, 0x0F33, 0x02B0, 0x02B8,
	0x02BD, 0x02C1, 0x02D0, 0x02D1, 0x02E0, 0x02E4, 0x203F, 0x2040,
	0x210A, 0x2113, 0x2118, 0x211D, 0x212A, 0x2131, 0x2133, 0x2138,
	0x2160, 0x2182, 0x3005, 0x3007, 0x3021, 0x3029, 0};

	int i;

	if(((u32)(c-'a'))<=((u32)('z'-'a')))	return(1);
	if(((u32)(c-'A'))<=((u32)('Z'-'A')))	return(1);

//	if((c>='a') && (c<='z'))return(1);
//	if((c>='A') && (c<='Z'))return(1);
	if(c=='_')return(1);
	if(c=='$')return(1);
	
	if(lang==BGBCC_LANG_ASM)
	{
		if(c=='.')return(1);
//		if(c=='/')return(1);
		if(c=='@')return(1);
	}

//	if((c>='0') && (c<='9'))return(0);
	if(c<128)return(0);

	for(i=0; vals[i]; i++)
		if(c==vals[i])return(1);

	for(i=0; rngs[i*2+0]; i++)
		if((c>=rngs[i*2+0]) && (c<=rngs[i*2+1]))
			return(1);

	return(0);
}

int BGBCP_NameChar(int c, int lang)
{
	if(((u32)(c-'a'))<=((u32)('z'-'a')))	return(1);
	if(((u32)(c-'A'))<=((u32)('Z'-'A')))	return(1);
	if(((u32)(c-'0'))<=((u32)('9'-'0')))	return(1);
	if(c=='_')return(1);
	if(c=='$')return(1);

	if(lang==BGBCC_LANG_ASM)
	{
		if(c=='.')return(1);
		if(c=='/')return(1);
		if(c=='-')return(1);
		if(c=='@')return(1);
	}

	if(c<128)return(0);

//	if((c>='0') && (c<='9'))return(1);
	if(BGBCP_NameInitChar(c, lang))return(1);


	return(0);
}

static const char bgbcp_nameinitchar_c[128]={
/* 0 1 2 3 4 5 6 7 8 9 A B C D E F */
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //0z
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //1z
   0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0, //2z
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //3z
   0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //4z
   1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1, //5z
   0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //6z
   1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0  //7z
};

static const char bgbcp_namechar_c[128]={
/* 0 1 2 3 4 5 6 7 8 9 A B C D E F */
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //0z
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //1z
   0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0, //2z
   1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0, //3z
   0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //4z
   1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1, //5z
   0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //6z
   1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0  //7z
};

static const char bgbcp_nameinitchar_a[128]={
/* 0 1 2 3 4 5 6 7 8 9 A B C D E F */
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //0z
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //1z
   0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0, //2z
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //3z
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //4z
   1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1, //5z
   0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //6z
   1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0  //7z
};

static const char bgbcp_namechar_a[128]={
/* 0 1 2 3 4 5 6 7 8 9 A B C D E F */
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //0z
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //1z
   0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1, //2z
   1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0, //3z
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //4z
   1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1, //5z
   0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //6z
   1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0  //7z
};

static const char *bgbcp_nameinitchar;
static const char *bgbcp_namechar;

char *BGBCP_TokenI2(char *s, char *b, int *ty, int lang, int sz)
{
	if(lang==BGBCC_LANG_ASM)
	{
		bgbcp_nameinitchar=bgbcp_nameinitchar_a;
		bgbcp_namechar=bgbcp_namechar_a;
	}else
	{
		bgbcp_nameinitchar=bgbcp_nameinitchar_c;
		bgbcp_namechar=bgbcp_namechar_c;
	}

	return(BGBCP_TokenI(s, b, ty, lang, sz));
}

char *BGBCP_TokenI(char *s, char *b, int *ty, int lang, int sz)
{
	u32 tcv;
	char *t, *s0, *s1, *te;
	int i, j, k, sti, ch;

	*b=0;
#if 0
	s=BGBCP_EatWhite(s);
	if(!*s)
	{
		*ty=BTK_NULL;
		return(s);
	}
#endif

#if 1
//	i=*s;
//	if((i<=' ') || (i=='/'))
	if((*s<=' ') || (*s=='/'))
	{
		s=BGBCP_EatWhite(s);
		if(!*s)
		{
			*ty=BTK_NULL;
			return(s);
		}
	}
#endif

//	t=b; te=t+sz-2;
	t=b; te=t+sz-4;

#if 0
	if((ch=*s)&0x80)
		ch=BGBCP_PeekChar(&s);
//	i=BGBCP_PeekChar(&s);
	if(BGBCP_NameInitChar(ch, lang))
	{
		while(t<te)
		{
			s1=s;
//			i=BGBCP_ParseChar(&s);
			if((i=*s)&0x80)		{ i=BGBCP_ParseChar(&s); }
				else			{ s++; }
			if(!BGBCP_NameChar(i, lang))
				{ s=s1; break; }
			if(i>>7)	{ BGBCP_EmitChar(&t, i); }
				else	{ *t++=i; }
		}

		*t++=0;

		*ty=BTK_NAME;
		return(s);
	}
#endif

#if 1
	tcv=BGBCC_GET_U32LE(s);
	if(tcv&BGBCC_ASC_MASK32)
	{
		if((ch=*s)&0x80)
			ch=BGBCP_PeekChar(&s);
		if(BGBCP_NameInitChar(ch, lang))
		{
			while(t<te)
			{
				s1=s;
				ident_l0:
				if((i=*s)&0x80)		{ i=BGBCP_ParseChar(&s); }
					else			{ s++; }
				if(!BGBCP_NameChar(i, lang))
					{ s=s1; break; }
				if(i>>7)	{ BGBCP_EmitChar(&t, i); }
					else	{ *t++=i; }
			}

			*t++=0;

			*ty=BTK_NAME;
			return(s);
		}
	}else
	{
//		ch=*s;
		ch=(byte)tcv;
//		if(BGBCP_NameInitChar(ch, lang))
		if(bgbcp_nameinitchar[ch])
		{
			while(t<te)
			{
				s1=s;
				tcv=BGBCC_GET_U32LE(s);
				if(tcv&BGBCC_ASC_MASK32)
					goto ident_l0;

#if 0
				i=	bgbcp_namechar[(byte)(tcv>> 0)] &
					bgbcp_namechar[(byte)(tcv>> 8)] &
					bgbcp_namechar[(byte)(tcv>>16)] &
					bgbcp_namechar[(byte)(tcv>>24)] ;
				if(i)
				{
					BGBCC_SET_U32LE(t, tcv);
					s+=4;	t+=4;
					continue;
				}
#endif

#if 1
				i=	bgbcp_namechar[(byte)(tcv>> 0)] &
					bgbcp_namechar[(byte)(tcv>> 8)] ;
				if(i)
				{
					j=	bgbcp_namechar[(byte)(tcv>>16)] &
						bgbcp_namechar[(byte)(tcv>>24)] ;
					if(j)
					{
						BGBCC_SET_U32LE(t, tcv);
						s+=4;	t+=4;
						continue;
					}
//					BGBCC_SET_U16LE(t, tcv);
//					s+=2;	t+=2;
//					continue;
					if(bgbcp_namechar[(byte)(tcv>>16)])
					{
						BGBCC_SET_U32LE(t, tcv);
						s+=3;	t+=3;
						break;
					}else
					{
						BGBCC_SET_U16LE(t, tcv);
						s+=2;	t+=2;
						break;
					}
				}else
				{
					i=(byte)tcv;
					if(bgbcp_namechar[i])
						{ s++; *t++=i; }
					break;
				}
#endif

#if 0
				i=(byte)tcv;
				s++;
//				if(!BGBCP_NameChar(i, lang))
				if(!bgbcp_namechar[i])
					{ s=s1; break; }
				*t++=i;
#endif
			}
			*t++=0;
			*ty=BTK_NAME;
			return(s);
		}
	}
#endif

//	if((*s>='0') && (*s<='9'))
	if(((u32)(ch-'0'))<=('9'-'0'))
	{
		if((s[0]=='0') && (s[1]=='x'))
		{
			*t++=*s++;
			*t++=*s++;
			while(((*s>='0') && (*s<='9')) ||
				((*s>='A') && (*s<='F')) ||
				((*s>='a') && (*s<='f')) ||
				(*s=='_') || (*s=='`'))
			{
				if((*s=='_') || (*s=='`'))
					{ s++; continue; }
				*t++=*s++;
			}
			*t++=0;

			*ty=BTK_NUMBER;
			return(s);
		}

#if 0
		if(*s=='-')*t++=*s++;
		while(((*s>='0') && (*s<='9')) || (*s=='.') || (*s=='e'))
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
#endif

		while(((*s>='0') && (*s<='9')) || (*s=='_') || (*s=='`'))
		{
			if((*s=='_') || (*s=='`'))
				{ s++; continue; }
			*t++=*s++;
		}
		if(*s=='.')*t++=*s++;
		while(((*s>='0') && (*s<='9')) || (*s=='_') || (*s=='`'))
		{
			if((*s=='_') || (*s=='`'))
				{ s++; continue; }
			*t++=*s++;
		}
		if((*s=='e') || (*s=='E'))
		{
			*t++=*s++;
			if((*s=='+') || (*s=='-'))*t++=*s++;
			while((*s>='0') && (*s<='9'))
				*t++=*s++;
		}

		*t++=0;

		*ty=BTK_NUMBER;
		return(s);
	}

//	if((s[0]=='.') && (s[1]>='0') && (s[1]<='9'))
	if((ch=='.') && (s[1]>='0') && (s[1]<='9'))
	{
		*t++='0';
		*t++='.';

		if(*s=='.')s++;

		while((*s>='0') && (*s<='9'))
			*t++=*s++;
		if(*s=='e')
		{
			*t++=*s++;
			if((*s=='+') || (*s=='-'))*t++=*s++;
			while((*s>='0') && (*s<='9'))
				*t++=*s++;
		}

		*t++=0;

		*ty=BTK_NUMBER;
		return(s);
	}

//	if((*s=='"') || (*s=='\'')) /* quoted string */
	if((ch=='"') || (ch=='\'')) /* quoted string */
	{
		if(*s=='\'')sti=1;
			else sti=0;
		s0=s;
		s++;
		while(*s && (t<te))
		{
			if(!sti)if(*s=='"')break;
			if(sti)if(*s=='\'')break;

//			if((s[0]=='\\') && ((s[1]=='x') ||
//				(s[1]=='u') || (s[1]=='U')))
			if((s[0]=='\\') && (s[1]=='x'))
			{
#if 0
				k=2;
				if(s[1]=='u')k=4;

				s+=2; j=0;
				while(k--)
				{
					j<<=4;
					if((*s>='0') && (*s<='9'))j+=*s-'0';
					if((*s>='A') && (*s<='F'))j+=*s-'A'+10;
					if((*s>='a') && (*s<='f'))j+=*s-'a'+10;
					s++;
				}
#endif

				s+=2; j=0;
				while(*s)
				{
					k=-1;
					if((*s>='0') && (*s<='9'))k=*s-'0';
					if((*s>='A') && (*s<='F'))k=*s-'A'+10;
					if((*s>='a') && (*s<='f'))k=*s-'a'+10;
					if(k<0)break;
					j=(j<<4)|k;
					s++;
				}

				BGBCP_EmitChar(&t, j);
				continue;
			}

//			if((s[0]=='\\') && (s[1]=='\\') && (s[2]=='U'))
//				__debugbreak();

			if((s[0]=='\\') && (s[1]=='\\'))
			{
				s+=2;
				*t++='\\';
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
					j<<=4;
					if((*s>='0') && (*s<='9'))j+=*s-'0';
					if((*s>='A') && (*s<='F'))j+=*s-'A'+10;
					if((*s>='a') && (*s<='f'))j+=*s-'a'+10;
					s++;
				}
				BGBCP_EmitChar(&t, j);
				continue;
			}

			if((s[0]=='\\') && ((s[1]>='0') && (s[1]<='7')))
			{
				s++; j=0;
				while((*s>='0') && (*s<='7'))
					{ j=(j*8)+((*s++)-'0'); }
				BGBCP_EmitChar(&t, j);
				continue;
			}

			if(*s=='\\')
			{
				s++;
				switch(*s++)
				{
				case 'a':
					*t++='\b';
					break;
				case 'b':
					*t++='\b';
					break;
				case 't':
					*t++='\t';
					break;
				case 'n':
					*t++='\n';
					break;
				case 'v':
					*t++='\v';
					break;
				case 'f':
					*t++='\f';
					break;
				case 'r':
					*t++='\r';
					break;
				case 'e':
					*t++='\x1B';
					break;
				case '\\':
					*t++='\\';
					break;
				case '"':
					*t++='"';
					break;
				case '\'':
					*t++='\'';
					break;
				case '?':
					*t++='?';
					break;
				default:
					break;
				}
				continue;
			}

//			*t++=*s++;
			i=BGBCP_ParseChar(&s);
			BGBCP_EmitChar(&t, i);
		}
		*t++=0;
		if(*s)s++;

		if(t>=te)
		{
			s=s0+1;
			*ty=BTK_STRING_OVF;
			return(s);
		}

		*ty=BTK_STRING;
		if(sti)*ty=BTK_CHARSTRING;
		return(s);
	}

//	if((*s==',') || (*s==';') || ((*s=='.') && (s[1]!='.')))
//	if((ch==',') || (ch==';') || ((ch=='.') && (s[1]!='.')))
	if((ch==',') || (ch==';'))
	{
		*t++=*s++;
		*t++=0;

//		kprint("token: %p '%s'\n", s, b);
		*ty=BTK_SEPERATOR;
		return(s);
	}

	if(ch=='.') 
	{
//		if(!strncmp(s, "...", 3))
		if(!bgbcp_strncmp3(s, "..."))
		{
			*t++=*s++;	*t++=*s++;
			*t++=*s++;	*t++=0;
			*ty=BTK_MISC;
			return(s);
		}
		*t++=*s++;	*t++=0;
		*ty=BTK_SEPERATOR;
		return(s);
	}

	if(	(*s=='{') || (*s=='}') ||
		(*s=='[') || (*s==']') ||
		(*s=='(') || (*s==')'))
	{
		if(s[1]=='`')
		{
			*t++=*s++; *t++=*s++; *t++=0;
			*ty=BTK_BRACE;
			return(s);
		}

		*t++=*s++;	*t++=0;
		*ty=BTK_BRACE;
		return(s);
	}

	if(ch=='#')
	{
//		if(!strncmp(s, "##", 2))
		if(!bgbcp_strncmp2(s, "##"))
		{
			*t++=*s++; *t++=*s++; *t++=0;
			*ty=BTK_OPERATOR;
			return(s);
		}

#if 0
		if(	!strncmp(s, "#<`", 3) || !strncmp(s, "#{`", 3) ||
			!strncmp(s, "#[`", 3) || !strncmp(s, "#(`", 3)
			)
		{
			*t++=*s++; *t++=*s++; *t++=*s++; *t++=0;
			*ty=BTK_BRACE;
			return(s);
		}
		
		if(
			!strncmp(s, "#<", 2) || !strncmp(s, "#{", 2) ||
			!strncmp(s, "#[", 2) || !strncmp(s, "#(", 2)
			)
		{
			*t++=*s++; *t++=*s++; *t++=0;
			*ty=BTK_BRACE;
			return(s);
		}
#endif
	}

	if(ch=='`')
	{
		if(	!strncmp(s, "`>", 2) ||
			!strncmp(s, "`}", 2) ||
			!strncmp(s, "`]", 2) ||
			!strncmp(s, "`)", 2))
		{
			*t++=*s++; *t++=*s++; *t++=0;
			*ty=BTK_BRACE;
			return(s);
		}

		if(	!strncmp(s, "`+", 2) ||
			!strncmp(s, "`-", 2) ||
			!strncmp(s, "`*", 2) ||
			!strncmp(s, "`/", 2) ||
			!strncmp(s, "`%", 2) ||
			!strncmp(s, "`&", 2) ||
			!strncmp(s, "`|", 2) ||
			!strncmp(s, "`^", 2) ||
			!strncmp(s, "`!", 2) ||
			!strncmp(s, "`~", 2) ||
			!strncmp(s, "``", 2))
		{
			*t++=*s++; *t++=*s++; *t++=0;
			*ty=BTK_OPERATOR;
			return(s);
		}

	}else if(ch=='<')
	{
//		if(	!strncmp(s, "<:", 2) ||
//			!strncmp(s, "<%", 2) ||
//			!strncmp(s, "<`", 2))

		if(	!bgbcp_strncmp2(s, "<:") ||
			!bgbcp_strncmp2(s, "<%") ||
			!bgbcp_strncmp2(s, "<`"))
		{
			*t++=*s++; *t++=*s++; *t++=0;
			*ty=BTK_BRACE;
			return(s);
		}
	}else if(s[1]=='`')
	{
		if(	!strncmp(s, "+`", 2) ||
			!strncmp(s, "-`", 2) ||
			!strncmp(s, "*`", 2) ||
			!strncmp(s, "/`", 2) ||
			!strncmp(s, "%`", 2) ||
			!strncmp(s, "&`", 2) ||
			!strncmp(s, "|`", 2) ||
			!strncmp(s, "^`", 2) ||
			!strncmp(s, "!`", 2) ||
			!strncmp(s, "~`", 2))
		{
			*t++=*s++; *t++=*s++; *t++=0;
			*ty=BTK_OPERATOR;
			return(s);
		}
	}else if(s[1]=='>')
	{
//		if(	!strncmp(s, ":>", 2) ||
		if(	!bgbcp_strncmp2(s, ":>") ||
//			!strncmp(s, "%>", 2))
			!bgbcp_strncmp2(s, "%>"))
		{
			*t++=*s++; *t++=*s++; *t++=0;
			*ty=BTK_BRACE;
			return(s);
		}
	}

	if(BGBCP_OpChar(*s))
	{
		*ty=BTK_OPERATOR;

//		if(lang==BGBCC_LANG_ASM)
		if(0)
		{
			*t++=*s++;
			*t++=0;
			if(*b==':')*ty=BTK_SEPERATOR;
			return(s);
		}

		if(lang==BGBCC_LANG_CPP)
		{
//			if(!strncmp(s, "::", 2))
			if(!bgbcp_strncmp2(s, "::"))
			{
				*t++=*s++;
				*t++=*s++;
				*t++=0;

				*ty=BTK_SEPERATOR;
				return(s);
			}
		}

		if(lang==BGBCC_LANG_JAVA)
		{
			if( !strncmp(s, "<<<=", 4) ||
				!strncmp(s, ">>>=", 4) )
			{
				*t++=*s++;
				*t++=*s++;
				*t++=*s++;
				*t++=*s++;
				*t++=0;
				return(s);
			}else if(	!strncmp(s, "<<<", 3) ||
						!strncmp(s, ">>>", 3) )
			{
				*t++=*s++;
				*t++=*s++;
				*t++=*s++;
				*t++=0;
				return(s);
			}
		}

//		if( !strncmp(s, "%:%:", 4) )
		if( !bgbcp_strncmp4(s, "%:%:") )
		{
			*t++=*s++;
			*t++=*s++;
			*t++=*s++;
			*t++=*s++;
			*t++=0;
		}else if(
//			!strncmp(s, "<<=", 3) ||
			!bgbcp_strncmp3(s, "<<=") ||
//			!strncmp(s, ">>=", 3)
			!bgbcp_strncmp3(s, ">>=")
			)
		{
			*t++=*s++;
			*t++=*s++;
			*t++=*s++;
			*t++=0;
		}else if(s[1]=='=')
		{
#if 0
			if(
				!strncmp(s, "==", 2) ||
				!strncmp(s, "<=", 2) ||
				!strncmp(s, ">=", 2) ||
				!strncmp(s, "!=", 2) ||
				!strncmp(s, "+=", 2) ||
				!strncmp(s, "-=", 2) ||
				!strncmp(s, "*=", 2) ||
				!strncmp(s, "/=", 2) ||
				!strncmp(s, "%=", 2) ||
				!strncmp(s, "&=", 2) ||
				!strncmp(s, "|=", 2) ||
				!strncmp(s, "^=", 2))
#endif
			if(	(s[0]=='=') || (s[0]=='<') ||
				(s[0]=='>') || (s[0]=='!') ||
				(s[0]=='+') || (s[0]=='-') ||
				(s[0]=='*') || (s[0]=='/') ||
				(s[0]=='%') || (s[0]=='&') ||
				(s[0]=='|') || (s[0]=='^'))
			{
				*t++=*s++;
				*t++=*s++;
				*t++=0;
			}
		}else if(
#if 0
			!strncmp(s, "<<", 2) ||
			!strncmp(s, ">>", 2) ||
			!strncmp(s, "&&", 2) ||
			!strncmp(s, "||", 2) ||
			!strncmp(s, "++", 2) ||
			!strncmp(s, "--", 2) ||
			!strncmp(s, "%:", 2) ||
			!strncmp(s, "##", 2) ||
			!strncmp(s, "->", 2))
#endif

#if 1
			!bgbcp_strncmp2(s, "<<") ||
			!bgbcp_strncmp2(s, ">>") ||
			!bgbcp_strncmp2(s, "&&") ||
			!bgbcp_strncmp2(s, "||") ||
			!bgbcp_strncmp2(s, "++") ||
			!bgbcp_strncmp2(s, "--") ||
			!bgbcp_strncmp2(s, "%:") ||
			!bgbcp_strncmp2(s, "##") ||
			!bgbcp_strncmp2(s, "->"))
#endif
		{
//			if(!strncmp(s, "##", 2))
//				__debugbreak();

			*t++=*s++;
			*t++=*s++;
			*t++=0;
		}else
		{
			*t++=*s++;
			*t++=0;

			if(*b==':')*ty=BTK_SEPERATOR;
		}

		return(s);
	}

	BGBCP_Error(s, "BGBCP_Token: Parse Error Char '%c'\n", *s);
	*ty=BTK_NULL;
	return(NULL);
}

#if 1

int bgbcp_strcpy(char *dst, char *src)
{
	char *cs, *ct;
	
	ct=dst; cs=src;
	while(*cs)
		{ *ct++=*cs++; }
	*ct++=0;
	return(cs-src);
}

int bgbcp_token_cnt;

#define BGBCP_TKHASHSZ 256
//char *BGBCP_TokenLang2(char *s, char *b, int *ty, int lang)
char *BGBCP_Token2(char *s, char *b, int *ty, int lang)
{
	static char ltb[BGBCP_TKHASHSZ][256];
	static char *ls[BGBCP_TKHASHSZ];
	static char *ls1[BGBCP_TKHASHSZ];
	static int lty[BGBCP_TKHASHSZ];
	static int llen[BGBCP_TKHASHSZ];
	static int flush=0;
	char *tb;
	int i, j, k;
	
	if(!s)
	{
		if(b && ty)
		{
			*b=0;
			*ty=BTK_NULL;
		}

		if(!flush)
		{
			for(i=0; i<BGBCP_TKHASHSZ; i++)
				ls[i]=NULL;
		}
		bgbcp_token_cnt=0;
		flush=64;
		return(NULL);
	}

	if(flush)
	{
		flush--;
		bgbcp_token_cnt++;
		return(BGBCP_TokenI2(s, b, ty, lang, 256));
	}

//	i=(((nlint)s)*7)&63;
//	i=((((nlint)s)*31)>>6)&(BGBCP_TKHASHSZ-1);
	i=((nlint)s)&(BGBCP_TKHASHSZ-1);
	tb=ltb[i];

	if(ls[i]==s)
	{
//		tb[255]=0;

		memcpy(b, tb, llen[i]+1);
//		strcpy(b, tb);
		*ty=lty[i];
		return(ls1[i]);
	}
	
	bgbcp_token_cnt++;
	ls[i]=s;
	ls1[i]=BGBCP_TokenI(s, tb, &j, lang, 256);
	lty[i]=j;
//	strcpy(b, tb);
	k=bgbcp_strcpy(b, tb);
	llen[i]=k;
	*ty=lty[i];
	return(ls1[i]);
}

char *BGBCP_Token(char *s, char *b, int *ty)
{
//	return(BGBCP_TokenLang2(s, b, ty, BGBCC_LANG_C));
	return(BGBCP_Token2(s, b, ty, BGBCC_LANG_C));
}

void BGBCP_FlushToken(char *s)
{
	BGBCP_Token(NULL, NULL, NULL);
}

int BGBCP_GetTokenCount(void)
{
	return(bgbcp_token_cnt);
}
#else

char *BGBCP_Token(char *s, char *b, int *ty)
	{ return(BGBCP_TokenI2(s, b, ty, BGBCC_LANG_C, 256)); }
void BGBCP_FlushToken(char *s)
	{ }

#endif

char *BGBCP_TokenGen(char *s, char *b, int *ty)
{
	char *t;
	int i, j, k, sti;

	*b=0;
	s=BGBCP_EatWhite(s);
	if(!*s)
	{
		*ty=BTK_NULL;
		return(s);
	}
	t=b;

	if(((*s)=='<') || ((*s)=='>'))
	{
		*t++=*s++;
		*t++=0;
		*ty=BTK_BRACE;
		return(s);
	}

	return(BGBCP_Token(s, b, ty));
}

char *BGBCP_TokenLang(char *s, char *b, int *ty, int lang)
{
	return(BGBCP_TokenI2(s, b, ty, lang, 256));
}

char *BGBCP_TokenCtx(BGBCP_ParseState *ctx, char *s, char *b, int *ty)
{
	return(BGBCP_TokenI2(s, b, ty, ctx->lang, 256));
}

#if 1
#define BGBCP_TKHASHSZ 256

char *BGBCP_ReadToken(BGBCP_ParseState *ctx, char **str, int *ty)
{
	static char *lts[BGBCP_TKHASHSZ];
	static char *ls[BGBCP_TKHASHSZ];
	static char *ls1[BGBCP_TKHASHSZ];
	static int lty[BGBCP_TKHASHSZ];
	static int flush=0;
	static char *tb=NULL;
	char *s, *t;
	int i;
	
	if(!str)
	{
		if(!flush)
		{
			for(i=0; i<BGBCP_TKHASHSZ; i++)
				ls[i]=NULL;
		}
		flush=64;
		return(NULL);
	}

	if(!tb)tb=malloc(65536);

	if(flush)
	{
		flush--;
		*str=BGBCP_TokenI(*str, tb, ty, ctx->lang, 4096);
		return(bgbcc_strdup(tb));
	}

	s=*str;

//	i=(((nlint)s)*7)&63;
//	i=((((nlint)s)*31)>>6)&(BGBCP_TKHASHSZ-1);
	i=((nlint)s)&(BGBCP_TKHASHSZ-1);
	t=lts[i];

	if(ls[i]==s)
	{
		*str=ls1[i];
		*ty=lty[i];
		return(t);
	}
	
	ls[i]=s;
	ls1[i]=BGBCP_TokenI(s, tb, &(lty[i]), ctx->lang, 4096);
	lts[i]=bgbcc_strdup(tb);
	*str=ls1[i];
	*ty=lty[i];
	return(lts[i]);
}
#endif
