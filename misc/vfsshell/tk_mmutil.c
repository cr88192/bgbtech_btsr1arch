void *tk_malloc(size_t sz)
{
	void *ptr;
	ptr=malloc(sz);
	memset(ptr, 0, sz);
	return(ptr);
}

void tk_free(void *ptr)
{
	free(ptr);
}

void *tk_realloc(void *ptr, size_t sz)
{
	return(realloc(ptr, sz));
}

void *tk_malloc_krn(size_t sz)
{
	return(malloc(sz));
}

void tk_puts(char *str)
{
	puts(str);
}


byte *tk_rlcbuf=NULL;
int tk_rlcpos;

void *tk_ralloc(int sz)
{
	void *p;

	if(!tk_rlcbuf)
	{
		tk_rlcbuf=(byte *)malloc(1<<18);
		tk_rlcpos=0;
	}

	if((tk_rlcpos+sz)>=((1<<18)-4096))
		tk_rlcpos=0;
	p=(void *)(tk_rlcbuf+tk_rlcpos);
	tk_rlcpos=(tk_rlcpos+sz+15)&(~15);

	return(p);
}

char *tk_rstrdup(char *s)
{
	char *t;
	t=(char *)tk_ralloc(strlen(s)+1);
	strcpy(t, s);
	return(t);
}

char **tk_split(char *s)
{
	char **a, *t;
	int i;

	a=tk_ralloc(64*sizeof(char *));
	i=0;
	while(*s)
	{
		while(*s && (*s<=' '))s++;
		if(!*s)break;
		if(*s=='#')break;
		if(*s==';')break;
		if((s[0]=='/') && (s[1]=='/'))
			break;

		t=tk_ralloc(256);
		a[i++]=t;

		if(*s=='"')
		{
			s++;
			while(*s && (*s!='"'))*t++=*s++;
			if(*s=='"')s++;

			*t++=0;
			continue;
		}

		while(*s && (*s>' '))*t++=*s++;
		*t++=0;
	}
	a[i]=NULL;

	return(a);
}


void tk_printf(char *str, ...)
{
	va_list lst;
	
	va_start(lst, str);
	vfprintf(stdout, str, lst);
	va_end(lst);
}


void tk_dbg_printf(char *str, ...)
{
	va_list lst;
	
	va_start(lst, str);
	vfprintf(stdout, str, lst);
	va_end(lst);
}

int tk_iskernel()
{
	return(1);
}


s64 tk_strtol(char *str, int rdx)
{
	char *s;
	s64 t;
	int i, j;
	
	s=str; t=0;
	while(*s)
	{
		if(*s=='_')
			{ s++; continue; }

		i=*s++; j=-1;
		if((i>='0') && (i<='9'))
			j=0+(i-'0');
		if((i>='A') && (i<='Z'))
			j=10+(i-'A');
		if((i>='a') && (i<='z'))
			j=10+(i-'a');
		if((j<0) || (j>=rdx))
			break;
		t=t*rdx+j;
	}
	return(t);
}

s64 tk_atoi(char *str)
{
	s64 li;
	char *s;
	int sg;
	
	if(*str=='0')
	{
		if(str[1]=='x')
			return(tk_strtol(str+2, 16));
		if(str[1]=='d')
			return(tk_strtol(str+2, 10));
		if(str[1]=='b')
			return(tk_strtol(str+2, 2));
		return(tk_strtol(str+1, 8));
	}
	
	s=str; sg=0;
	if(*s=='-')
		{ s++; sg=1; }
	li=tk_strtol(s, 10);
	if(sg)li=-li;
	return(li);
}


char **tk_strintern_lst;

char *tk_strdup_in(char *str)
{
	char **ts;
	char *s;
	
	if(!str)
		return(NULL);
	
	ts=tk_strintern_lst;
	while(ts)
	{
		s=(char *)(ts+1);
		if(!strcmp(s, str))
			return(s);
		ts=(char **)(*ts);
	}
	
	ts=tk_malloc(sizeof(char *)+strlen(str)+1);
	s=(char *)(ts+1);
	strcpy(s, str);
	ts[0]=(char *)tk_strintern_lst;
	tk_strintern_lst=ts;
	return(s);
}

int tk_syscall(void *obj, int msg, void *rtptr, TK_SysArg *args)
{
	return(-1);
}

void TK_VMem_AddSdSwap(s64 lba, s64 sz)
{
}


char *tk_cwd;

int TK_Env_SetCwd(char *str)
{
	tk_cwd=tk_strdup_in(str);
	return(0);
}

int TK_Env_GetCwd(char *buf, int sz)
{
	if(!tk_cwd)
	{
		strcpy(buf, "/");
		return(0);
	}

	strcpy(buf, tk_cwd);
	return(0);
}

s64 TK_DateToEpochUsec(TK_DATETIME *date)
{
	s64 us;
	
	us=	(date->year-1970)*(365.2500*24*60*60*1000)+
		(date->mon      )*( 30.4375*24*60*60*1000)+
		(date->day      )*(     1.0*24*60*60*1000)+
		(date->hour     )*(        1.0*60*60*1000)+
		(date->min      )*(           1.0*60*1000)+
		(date->sec      )*(              1.0*1000);
	return(us);
}

/** Convert size into a 5.3 minifloat, Round Up */
int TKMM_SizeToFxiU(int sz)
{
	int fr, ex;
	int i;
	
	fr=sz;
	ex=0;
//	while((fr>>3)>1)
	while(fr>=16)
		{ fr=(fr+1)>>1; ex++; }
	i=(fr&7)|(ex<<3);

	if(i&(~255))
		__debugbreak();

	return(i);
}

/** Convert size into a 5.3 minifloat, Round Down */
int TKMM_SizeToFxiD(int sz)
{
	int fr, ex;
	int i;
	
	fr=sz;
	ex=0;
//	while((fr>>3)>1)
	while(fr>=16)
		{ fr=fr>>1; ex++; }
	i=(fr&7)|(ex<<3);
	
	if(i&(~255))
		__debugbreak();
	
	return(i);
}

/** Convert Fxi to size */
int TKMM_FxiToSize(int ix)
{
	int ex, fr;
	int sz;
	
	if(ix<8)return(ix);
	fr=(ix&7)|8; ex=(ix>>3)&31;
	sz=fr<<ex;
	return(sz);
}

void TKSH_NormalizePath(char *dst, char *src)
{
	char *cs, *ct, *ct0;
	
	cs=src;
	ct=dst;
	
	while(*cs=='/')
		cs++;
	*ct++='/';
	ct0=ct;
	while(*cs)
	{
		if(((cs[0]=='/') || cs[0]=='\\') && ((cs[1]=='/') || (cs[1]=='\\')))
			{ cs++; continue; }
		if(*cs=='\\')
			{ *ct++='/'; cs++; continue; }

		if(	((cs[0]=='/') || cs[0]=='\\') &&
			(cs[1]=='.') &&
			((cs[2]=='/') || (cs[2]=='\\') || (cs[2]==0)))
		{
			cs+=2;
//			*ct++='/'; 
			continue;
		}

		if(	((cs[0]=='/') || cs[0]=='\\') &&
			(cs[1]=='.') &&
			(cs[2]=='.') &&
			((cs[3]=='/') || (cs[3]=='\\') || (cs[3]==0)))
		{
			if(ct>ct0)
			{
				if(*ct=='/')
					ct--;
				while(*ct!='/')
					ct--;
				if(ct>ct0)
				{
//					if(*ct=='/')
//						ct--;
					*ct=0;
				}else if(cs[3]==0)
				{
					ct++;
					*ct=0;
				}else
				{
					*ct=0;
				}
			}
			cs+=3;
			continue;
		}

		*ct++=*cs++;
	}
	*ct++=0;
}

void *_memlzcpy(void *dst, void *src, int sz)
{
	byte *ct, *cs, *cse;

	ct=dst; cs=src; cse=cs+sz;
	while(cs<cse)
	{
		*ct++=*cs++;
	}
	return(ct);
}

void *_memlzcpyf(void *dst, void *src, int sz)
{
	return(_memlzcpy(dst, src, sz));
}
