#include <bgbccc.h>

#ifdef WIN32
#include <windows.h>
#endif

#define BGBCC_SZSTRHASH		16384

void *bgbcc_alloc_block[1024];
void *bgbcc_alloc_blklst[1024];
int bgbcc_alloc_nblock;
int bgbcc_alloc_curblock;

byte *bgbcc_alloc_rov=NULL;
byte *bgbcc_alloc_srov=NULL;
byte *bgbcc_alloc_erov=NULL;

void *bgbcc_alloc_list=NULL;		//list of larger allocs
// void *bgbcc_alloc_slist=NULL;		//list of smaller allocs

// char *bgbcc_alloc_strhash[BGBCC_SZSTRHASH];

char *bgbcc_stralloc_block[1024];
int bgbcc_stralloc_nblock;

char *bgbcc_stralloc_rov=NULL;
char *bgbcc_stralloc_srov=NULL;
char *bgbcc_stralloc_erov=NULL;

char **bgbcc_str_varr;
int *bgbcc_str_carr;
char *bgbcc_str_sarr;
int bgbcc_str_num;
int bgbcc_str_max;
int bgbcc_str_hash[BGBCC_SZSTRHASH];

byte *bgbcc_rlcbuf=NULL;
int bgbcc_rlcpos;

int bgbcc_gsseq=1;
int bgbcc_gsseed=1;

void BGBCC_DieError()
{
//#ifdef WIN32
#if 0
	ULONG_PTR args[8];
	RaiseException(BGBCC_EXCEPTION, EXCEPTION_NONCONTINUABLE,
		1, args);
#else
//	*(int *)-1=-1;
#endif
}

void BGBCC_DieFatal()
{
//#ifdef WIN32
#if 0
	ULONG_PTR args[8];
	RaiseException(BGBCC_EXCEPTION, EXCEPTION_NONCONTINUABLE,
		1, args);
#else
	*(int *)-1=-1;
#endif
}

void BGBCC_CleanupAll()
{
	int i;

//	bgbcc_alloc_rov=0;
//	bgbcc_alloc_srov=0;
//	bgbcc_alloc_erov=0;

//	for(i=0; i<bgbcc_alloc_nblock; i++)
//		free(bgbcc_alloc_block[i]);
//	bgbcc_alloc_nblock=0;

	if(bgbcc_alloc_rov)
	{
		printf("BGBCC_CleanupAll: Used %d block(s)\n",
			bgbcc_alloc_nblock);

//		for(i=1; i<bgbcc_alloc_nblock; i++)
//			free(bgbcc_alloc_block[i]);
		bgbcc_alloc_nblock=1;

		bgbcc_alloc_rov=bgbcc_alloc_block[0];
		bgbcc_alloc_srov=bgbcc_alloc_block[0];
		bgbcc_alloc_erov=bgbcc_alloc_srov+(1<<22);
	}

//	for(i=0; i<BGBCC_SZSTRHASH; i++)
//		bgbcc_alloc_strhash[i]=NULL;

//	for(i=0; i<BGBCC_SZSTRHASH; i++)
//		bgbcc_str_hash[i]=0;
//	bgbcc_str_varr=NULL;
//	bgbcc_str_carr=NULL;
}

int bgbcc_malloc_memset(void *buf, int val, int sz)
{
	byte *t, *te;
	u32 v;
	
	t=buf; te=t+sz;
	v=(byte)val; v=v|(v<<8); v=v|(v<<16);
#if 1
	while((t+16)<=te)
	{
		((u32 *)t)[0]=v; ((u32 *)t)[1]=v;
		((u32 *)t)[2]=v; ((u32 *)t)[3]=v;
		t+=16;
	}
#endif
	while((t+4)<=te)
		{ *((u32 *)t)=v; t+=4; }
	while(t<te)
		*t++=val;
	return(0);
}

void *bgbcc_tmalloc(char *ty, int sz)
{
	void *bps, *bpe;
	void *p;
	int i, n, tyi, tty, tsz;

	if(sz<0)
		{ BGBCC_DBGBREAK }

//	if(sz>=65536)
	if(sz>=(32768-256))
	{
		sz+=2*sizeof(void *);
		sz=(sz+255)&(~255);

		tsz=(sz<(262144-256))?(0x800|((sz+255)>>8)):(0xC00|((sz+65535)>>16));
		sz=(!(tsz&0x400))?((tsz&0x3FF)<<8):((tsz&0x3FF)<<16);

		p=malloc(sz);
		if(!p)
		{
			printf("bgbcc_tmalloc: Out Of Memory\n");
			*(int *)-1=-1;
		}

//		bgbcc_malloc_memset(p, 0, sz);
		memset(p, 0, sz);
		*(void **)p=bgbcc_alloc_list;
		bgbcc_alloc_list=p;

		tyi=bgbcc_strdup_i(ty, 0);
//		tsz=(sz<524288)?(sz>>8):(0x800|(sz>>16));
//		tsz=(sz<262144)?(0x800|(sz>>8)):(0xC00|(sz>>16));
		tty=tsz|(tyi<<12);
		((void **)p)[1]=(void *)(nlint)tty;

		p=(void *)(((void **)p)+2);
		return(p);
	}

	if(!bgbcc_alloc_rov)
	{
		for(i=0; i<1024; i++)
		{
			bgbcc_alloc_block[i]=NULL;
			bgbcc_alloc_blklst[i]=NULL;
		}
		bgbcc_alloc_nblock=0;

		bgbcc_alloc_srov=malloc(1<<22);
		bgbcc_alloc_erov=bgbcc_alloc_srov+(1<<22);

		bgbcc_alloc_rov=bgbcc_alloc_srov;

		bgbcc_alloc_curblock=bgbcc_alloc_nblock++;
		bgbcc_alloc_block[bgbcc_alloc_curblock]=bgbcc_alloc_rov;
		bgbcc_alloc_blklst[bgbcc_alloc_curblock]=NULL;
	}

	if((bgbcc_alloc_rov+sz)>=(bgbcc_alloc_erov-4096))
	{
		n=bgbcc_alloc_nblock++;
		if(!bgbcc_alloc_block[n])
		{
//			printf("BGBCC: Expand Heap, %d block, %dMiB\n",
//				bgbcc_alloc_nblock,
//				bgbcc_alloc_nblock*4);
			bgbcc_alloc_block[n]=malloc(1<<22);

			if(!bgbcc_alloc_block[n])
			{
				printf("bgbcc_tmalloc: Out Of Memory\n");
				*(int *)-1=-1;
			}
		}

		bgbcc_alloc_blklst[n]=NULL;
		bgbcc_alloc_curblock=n;
		
		bgbcc_alloc_srov=bgbcc_alloc_block[n];
		bgbcc_alloc_erov=bgbcc_alloc_srov+(1<<22);
		bgbcc_alloc_rov=bgbcc_alloc_srov;
	}

	sz+=2*sizeof(void *);
	sz=(sz+15)&(~15);
	tsz=sz+(2*sizeof(void *));

	p=bgbcc_alloc_rov;
	bgbcc_alloc_rov+=(tsz+15)&(~15);

//	bgbcc_malloc_memset(p, 0, sz);
	memset(p, 0, sz);

	bps=bgbcc_alloc_block[bgbcc_alloc_curblock];
	bpe=((char *)bps)+(1<<22);
	
	if((p<bps) || (p>=bpe))
		{ BGBCC_DBGBREAK }

	*(void **)p=bgbcc_alloc_blklst[bgbcc_alloc_curblock];
	bgbcc_alloc_blklst[bgbcc_alloc_curblock]=p;

	tyi=bgbcc_strdup_i(ty, 0);
	tty=((sz>>4)&4095)|(tyi<<12);
	((void **)p)[1]=(void *)(nlint)tty;
	*(void **)(((char *)p)+sz)=(void *)(nlint)tty;

	p=(void *)(((void **)p)+2);

	return(p);
}

void *bgbcc_stralloc(int sz)
{
	void *p;
	int i, n, tyi, tty, tsz;

	if(!bgbcc_stralloc_rov)
	{
		for(i=0; i<1024; i++)
			bgbcc_stralloc_block[i]=NULL;
		bgbcc_stralloc_nblock=0;

		bgbcc_stralloc_srov=malloc(1<<22);
		bgbcc_stralloc_erov=bgbcc_stralloc_srov+(1<<22);

		bgbcc_stralloc_rov=bgbcc_stralloc_srov;

		n=bgbcc_stralloc_nblock++;
//		bgbcc_stralloc_curblock=n;
		bgbcc_stralloc_block[n]=bgbcc_stralloc_rov;
//		bgbcc_stralloc_blklst[n]=NULL;
	}

	if((bgbcc_stralloc_rov+sz)>=(bgbcc_stralloc_erov-4096))
	{
		n=bgbcc_stralloc_nblock++;
		if(!bgbcc_stralloc_block[n])
		{
//			printf("BGBCC: Expand Str Heap, %d block, %dMiB\n",
//				bgbcc_stralloc_nblock,
//				bgbcc_stralloc_nblock*4);
			bgbcc_stralloc_block[n]=malloc(1<<22);

			if(!bgbcc_stralloc_block[n])
			{
				printf("bgbcc_stralloc: Out Of Memory\n");
				*(int *)-1=-1;
			}
		}

//		bgbcc_stralloc_blklst[n]=NULL;
//		bgbcc_stralloc_curblock=n;
		
		bgbcc_stralloc_srov=bgbcc_stralloc_block[n];
		bgbcc_stralloc_erov=bgbcc_stralloc_srov+(1<<22);
		bgbcc_stralloc_rov=bgbcc_stralloc_srov;
	}

	p=bgbcc_stralloc_rov;
	bgbcc_stralloc_rov+=sz;

//	memset(p, 0, sz);
	return(p);
}

void *bgbcc_malloc(int sz)
{
	return(bgbcc_tmalloc(NULL, sz));
}

void *bgbcc_tmalloc2(char *ty, int sz)
{
	void *p;

	p=malloc(sz);
	if(!p)
	{
		printf("bgbcc_tmalloc2: Out Of Memory\n");
		*(int *)-1=-1;
	}

	memset(p, 0, sz);
	return(p);
}

void *bgbcc_malloc2(int sz)
{
	return(bgbcc_tmalloc2(NULL, sz));
}

void bgbcc_free(void *p)
{
//	free(p);
}

void *bgbcc_realloc(void *ptr, int sz)
{
	int sz0, sz1;
	void *ptr2;
	
	if(!ptr)
	{
		return(bgbcc_malloc(sz));
	}
	
	sz0=bgbcc_malloc_getsize(ptr);
	sz1=sz0;
	if(sz<sz1)
		sz1=sz;
	
	ptr2=bgbcc_malloc(sz);
//	memcpy(ptr2, ptr, sz);
	if(sz1>0)
		memcpy(ptr2, ptr, sz1);
	bgbcc_free(ptr);
	return(ptr2);
}

int bgbcc_malloc_lookupblock(void *obj)
{
	byte *p, *ps, *pe;
	int i;
	
	p=obj;
	for(i=0; i<bgbcc_alloc_nblock; i++)
	{
		ps=bgbcc_alloc_block[i];
		pe=ps+(1<<22);
		if((p>=ps) && (p<pe))
			return(i);
	}
	return(-1);
}

int bgbcc_stralloc_lookupblock(void *obj)
{
	char *p, *ps, *pe;
	int i;
	
	p=obj;
	for(i=0; i<bgbcc_stralloc_nblock; i++)
	{
		ps=bgbcc_stralloc_block[i];
		pe=ps+(1<<22);
		if((p>=ps) && (p<pe))
			return(i);
	}
	return(-1);
}

void *bgbcc_malloc_getbase(void *obj)
{
	int bi, tty, tty1, sz;
	void *bps, *bpe;
	void *p, *p1, *pe;
	
	if(!obj)
		return(NULL);
	
	bi=bgbcc_malloc_lookupblock(obj);
	if(bi>=0)
	{
		bps=bgbcc_alloc_block[bi];
		bpe=((char *)bps)+(1<<22);

		p=bgbcc_alloc_blklst[bi];
		while(p)
		{
			if(obj>p)
			{
				tty=(nlint)(((void **)p)[1]);
//				sz=(tty&4095)<<4;
				sz=(tty&2047)<<4;
				pe=((char *)p)+sz;
				if(obj<pe)
					return(p);
				if(pe>=bpe)
					{ BGBCC_DBGBREAK }
				tty1=(nlint)(*(void **)pe);
				if(tty1!=tty)
					{ BGBCC_DBGBREAK }
			}
			p1=*(void **)p;
			if((p1<bps) || (p1>=bpe))
				{ BGBCC_DBGBREAK }
			p=p1;
		}
	}

	p=bgbcc_alloc_list;
	while(p)
	{
		if(obj>p)
		{
			tty=(nlint)(((void **)p)[1]);
//			sz=(tty&4095)<<8;
			sz=tty&4095;
//			sz=(sz<2048)?(sz<<8):((sz&0x7FF)<<16);
			sz=(!(sz&0x400))?((sz&0x3FF)<<8):((sz&0x3FF)<<16);
			pe=((char *)p)+sz;
			if(obj<pe)
				return(p);
		}
		p=*(void **)p;
	}
	
	return(NULL);
}

char *bgbcc_malloc_gettype(void *obj)
{
	int tty, sz, tyi;
	void *p;
	
	p=bgbcc_malloc_getbase(obj);
	if(!p)return(NULL);

	tty=(nlint)(((void **)p)[1]);
	sz=(tty&4095)<<4;
	
	tyi=(tty>>12)&65535;
	return(bgbcc_str_varr[tyi]);
}

int bgbcc_malloc_getsize(void *obj)
{
	int tty, sz, sz0, tyi;
	void *p;
	
	p=bgbcc_malloc_getbase(obj);
	if(!p)
	{
		BGBCC_DBGBREAK
		return(-1);
	}

	tty=(nlint)(((void **)p)[1]);
	sz0=(tty&4095);

	if(sz0&0x800)
	{
		sz=(!(sz0&0x400))?((sz0&0x3FF)<<8):((sz0&0x3FF)<<16);
	}else
	{
		sz=(sz0&2047)<<4;
	}

	return(sz);
}

#if 0
char *bgbcc_strdup(char *str)
{
	char *s, *t;
	int i, hi;

	if(!str)return(NULL);

	s=str; hi=0;
	while(*s)hi=(hi*4093)+(*s++);
	i=((hi*4093)>>12)&4095;

	t=bgbcc_alloc_strhash[i];
	if(t && !strcmp(t, str))
		return(t);

	t=bgbcc_malloc(strlen(str)+1);
	strcpy(t, str);
	bgbcc_alloc_strhash[i]=t;

	return(t);
}
#endif

int bgbcc_strdup_strlen(char *str)
{
	char *s;
	s=str;
	while(*s)s++;
	return(s-str);
}

int bgbcc_strdup_strcmp(char *s1, char *s2)
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

int bgbcc_strdup_strcpy(char *dst, char *src)
{
	char *cs, *ct;
	
	ct=dst; cs=src;
	while(*cs)
		{ *ct++=*cs++; }
	*ct++=0;
	return(cs-src);
}

int bgbcc_strdup_i(char *str, char suf)
{
	char *s, *t;
	int i, l, hi;

	if(!str)return(0);

#if 1
	if(!bgbcc_str_varr)
	{
		for(i=0; i<BGBCC_SZSTRHASH; i++)bgbcc_str_hash[i]=0;
		bgbcc_str_varr=malloc(16384*sizeof(char *));
		bgbcc_str_carr=malloc(16384*sizeof(int));
		bgbcc_str_sarr=malloc(16384*sizeof(char));

		bgbcc_str_varr[0]=NULL;
		bgbcc_str_carr[0]=0;
		bgbcc_str_sarr[0]=0;
		bgbcc_str_num=1;
		bgbcc_str_max=16384;
	}
#endif

	s=str; hi=0;
	while(*s)hi=(hi*251)+(*s++);
	hi&=(BGBCC_SZSTRHASH-1);
//	hi=(hi>>8)&(BGBCC_SZSTRHASH-1);

	i=bgbcc_str_hash[hi];
	while(i)
	{
//		if(!strcmp(bgbcc_str_varr[i], str) &&
		if(!bgbcc_strdup_strcmp(bgbcc_str_varr[i], str) &&
			(bgbcc_str_sarr[i]==suf))
				{ return(i); }
		i=bgbcc_str_carr[i];
	}

	if((bgbcc_str_num+1)>=bgbcc_str_max)
	{
		i=bgbcc_str_max+(bgbcc_str_max>>1);
		bgbcc_str_varr=realloc(bgbcc_str_varr, i*sizeof(char *));
		bgbcc_str_carr=realloc(bgbcc_str_carr, i*sizeof(int));
		bgbcc_str_sarr=realloc(bgbcc_str_sarr, i*sizeof(char));
		bgbcc_str_max=i;
	}

	l=bgbcc_strdup_strlen(str);
//	t=bgbcc_malloc(strlen(str)+1);
//	t=malloc(l+3);
	t=bgbcc_stralloc(l+3);
	bgbcc_strdup_strcpy(t, str);
	t[l+1]=suf;

	i=bgbcc_str_num++;
	bgbcc_str_varr[i]=t;
	bgbcc_str_carr[i]=bgbcc_str_hash[hi];
	bgbcc_str_sarr[i]=suf;
	bgbcc_str_hash[hi]=i;

	return(i);
}

int bgbcc_strdup_2i(char *str)
{
	return(bgbcc_strdup_i(str, 0));
}

char *bgbcc_strtab_i(int i)
	{ return(bgbcc_str_varr[i]); }

char *bgbcc_strdup(char *str)
{
	int i;

	if(!str)return(NULL);
	i=bgbcc_strdup_i(str, 0);
	return(bgbcc_str_varr[i]);
}

char *bgbcc_strdup2(char *str, char suf)
{
	int i;

	if(!str)return(NULL);
	i=bgbcc_strdup_i(str, suf);
	return(bgbcc_str_varr[i]);
}

int bgbcc_strdup_lookup(char *str, char suf)
{
	char *s;
	int hi;
	int i;

	if(!str)
		return(0);

#if 0
	if(!bgbcc_str_varr)
		return(-1);

	for(i=0; i<bgbcc_str_num; i++)
		if(bgbcc_str_varr[i]==str)
			return(i);
	return(-1);
#endif

#if 1
	s=str; hi=0;
	while(*s)hi=(hi*251)+(*s++);
	hi&=(BGBCC_SZSTRHASH-1);
//	hi=(hi>>8)&(BGBCC_SZSTRHASH-1);

	i=bgbcc_str_hash[hi];
	while(i)
	{
		if(!bgbcc_strdup_strcmp(bgbcc_str_varr[i], str) &&
			(bgbcc_str_sarr[i]==suf))
				{ return(i); }
		i=bgbcc_str_carr[i];
	}
	return(-1);
#endif
}

bool bgbcc_strdup_checkp(char *str, char suf)
{
	int i;
	
	i=bgbcc_stralloc_lookupblock(str);
	if(i<0)return(false);
	
	i=bgbcc_strdup_lookup(str, suf);
	if(i<=0)return(false);
	return(bgbcc_str_varr[i]==str);
}

bool bgbcc_strdup_strp(char *str)
{
	return(bgbcc_strdup_checkp(str, 0)!=0);
}

bool bgbcc_strdup_symp(char *str)
{
	return(bgbcc_strdup_checkp(str, '#')!=0);

//	char *s;
//	if(!str)
//		return(0);
//	s=str-1;
//	if(!bgbcc_strdup_strp(s))
//		return(false);
//	return(*s=='#');
}

bool bgbcc_strdup_keyp(char *str)
{
	return(bgbcc_strdup_checkp(str, ':')!=0);

//	char *s;	
//	if(!str)
//		return(0);
//	s=str-1;
//	if(!bgbcc_strdup_strp(s))
//		return(false);
//	return(*s==':');
}

char *bgbcc_strdup_symbol(char *str)
{
	return(bgbcc_strdup2(str, '#'));

//	char tb[256];
//	strcpy(tb, "#");
//	strcat(tb, str);
//	return(bgbcc_strdup(tb)+1);
}

char *bgbcc_strdup_keyword(char *str)
{
	return(bgbcc_strdup2(str, ':'));

//	char tb[256];
//	strcpy(tb, ":");
//	strcat(tb, str);
//	return(bgbcc_strdup(tb)+1);
}

void *bgbcc_ralloc(int sz)
{
	void *p;

	if(!bgbcc_rlcbuf)
	{
		bgbcc_rlcbuf=(byte *)malloc(1<<18);
		bgbcc_rlcpos=0;
	}

	if((bgbcc_rlcpos+sz)>=((1<<18)-4096))
		bgbcc_rlcpos=0;
	p=(void *)(bgbcc_rlcbuf+bgbcc_rlcpos);
	bgbcc_rlcpos=(bgbcc_rlcpos+sz+15)&(~15);

	return(p);
}

char *bgbcc_rstrdup(char *s)
{
	char *t;
	t=(char *)bgbcc_ralloc(strlen(s)+1);
	strcpy(t, s);
	return(t);
}

void *bgbcc_rmemdup(void *buf, int sz)
{
	char *t;
	t=(char *)bgbcc_ralloc(sz);
	memcpy(t, buf, sz);
	return(t);
}

char **bgbcc_split(char *s)
{
	char **a, *t;
	int i;

	a=bgbcc_ralloc(64*sizeof(char *));
	i=0;
	while(*s)
	{
		while(*s && (*s<=' '))s++;
		if(!*s)break;
		if(*s=='#')break;
		if(*s==';')break;
		if((s[0]=='/') && (s[1]=='/'))
			break;

		t=bgbcc_ralloc(256);
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

char *bgbcc_rstradd(char *s0, char *s1)
{
	char *t;

	t=(char *)bgbcc_ralloc(strlen(s0)+strlen(s1)+1);
	strcpy(t, s0);
	strcat(t, s1);
	return(t);
}

char *bgbcc_rstradd2(char *s0, char *s1, char *s2)
{
	char *t;
	int i;

	i=strlen(s0)+strlen(s1)+strlen(s2);
	t=(char *)bgbcc_ralloc(i+1);
	strcpy(t, s0);
	strcat(t, s1);
	strcat(t, s2);
	return(t);
}

char *bgbcc_rstradd3(char *s0, char *s1, char *s2, char *s3)
{
	char *t;
	int i;

	i=strlen(s0)+strlen(s1)+strlen(s2)+strlen(s3);
	t=(char *)bgbcc_ralloc(i+1);
	strcpy(t, s0); strcat(t, s1);
	strcat(t, s2); strcat(t, s3);
	return(t);
}


struct bgbcc_ordtab_s
{
	char **tab;
	short *chain;
	short hash[256];
	int num;
};

void *bgbcc_neword(char **tab)
{
	struct bgbcc_ordtab_s *tmp;
	char *s;
	int i, j, n;
	
	for(n=0; tab[n]; n++);
	tmp=bgbcc_malloc2(sizeof(struct bgbcc_ordtab_s));
	tmp->tab=bgbcc_malloc2((n+1)*sizeof(char *));
	tmp->chain=bgbcc_malloc2((n+1)*sizeof(short));
	tmp->num=n;

	for(i=0; i<256; i++)
		tmp->hash[i]=-1;

	for(i=0; i<n; i++)
		tmp->tab[i]=bgbcc_strdup(tab[i]);

	for(i=0; i<n; i++)
	{
		s=tab[i]; j=0;
		while(*s)j=(j*251)+(*s++);
		j&=255;
		tmp->chain[i]=tmp->hash[j];
		tmp->hash[j]=i;
	}
	
	return((void *)tmp);
}

int bgbcc_strord(char *str, void *tab)
{
	struct bgbcc_ordtab_s *tmp;
	char *s;
	int i, j, n;

	tmp=(struct bgbcc_ordtab_s *)tab;

	s=str; j=0;
	while(*s)j=(j*251)+(*s++);
	j&=255;
	
	i=tmp->hash[j];
	while(i>=0)
	{
		if(!strcmp(tmp->tab[i], str))
			return(i);
		i=tmp->chain[i];
	}
	return(-1);
}

extern char *bgbcc_tokord_strs[];
extern char *bgbcc_tokord_args[];

int BGBCC_GetOrdinal(char *str)
{
	static void *tab=NULL;
	if(!tab)
	{
		tab=bgbcc_neword(bgbcc_tokord_strs);
	}
	return(bgbcc_strord(str, tab));
}

int BGBCC_CheckOrdinalFlag(int i, int ty)
{
	char *s;

	if(i<0)return(0);
	s=bgbcc_tokord_args[i];
	if(s && *s)
	{
		while(*s) { if(*s==ty)break; s++; }
		if(*s)return(1);
	}
	return(0);
}

int BGBCC_GetOrdinalLang(char *str, int lang)
{
	static char *lchrs="-CPSJEF";
	static void *tab=NULL;
	char *s;
	int i, c;
	
	if(!tab)
		{ tab=bgbcc_neword(bgbcc_tokord_strs); }

	i=bgbcc_strord(str, tab);
	if(i<0)return(-1);
	s=bgbcc_tokord_args[i];
	if(s && *s && (lang>0))
	{
		c=lchrs[lang];	
		while(*s) { if(*s==c)break; s++; }
		if(!(*s))return(-1);
	}
	return(i);
}

int BGBCC_GetOrdinalLangType(char *str, int lang, int ty)
{
	char *s;
	int i;

	i=BGBCC_GetOrdinalLang(str, lang);
	if(i<0)return(-1);
	s=bgbcc_tokord_args[i];
	if(s && *s && (lang>0))
	{
		while(*s) { if(*s==ty)break; s++; }
		if(!(*s))return(-1);
	}
	return(i);
}


double BGBCC_ParseNumber(char *str)
{
	long long l;
	char *s, *t;
	int i, sg;

	t=str;
	if((t[0]=='0') && (t[1]=='x'))
	{
		l=0; t+=2;
		while(*t)
		{
			if(*t=='_')
				{ t++; continue; }
			l*=16;
			if((*t>='0') && (*t<='9'))l+=*t-'0';
			if((*t>='A') && (*t<='F'))l+=*t-'A'+10;
			if((*t>='a') && (*t<='f'))l+=*t-'a'+10;
			t++;
		}

		return(l);
	}

	i=0; t=str;
	while(*t) { if(*t=='.')i++; if(*t=='e')i++; t++; }
	if(i)
	{
		return(atof(str));
	}

	t=str;
	if(t[0]=='0')
	{
		l=0; t++;
		while(*t)
		{
			if(*t=='_')
				{ t++; continue; }
			l*=8;
			if((*t>='0') && (*t<='7'))l+=*t-'0';
				else break;
			t++;
		}

		return(l);
	}

	t=str; l=0; sg=0;
	if(*t=='-') { sg=1; t++; }
	while(*t)
	{
		if(*t=='_')
			{ t++; continue; }
		l*=10;
		if((*t>='0') && (*t<='9'))l+=*t-'0';
			else break;
		t++;
	}
	if(sg)l=-l;

	return(l);
}

void BGBCC_SeedGenSym(u32 seed)
{
	bgbcc_gsseed=seed;
}

char *BGBCC_GenSym()
{
	char buf[32];

	sprintf(buf, "GS%08X_%d", bgbcc_gsseed, bgbcc_gsseq++);
	return(bgbcc_strdup(buf));
}

char *BGBCC_GenSym2()
{
	return(BGBCC_GenSym());
//	return(BS1_RStrDup(BASM_GenSym()));
}

int BGBCC_GenSymInt()
{
	int i;
	i=bgbcc_gsseed^(bgbcc_gsseq++);
	i&=0x7FFFFFFF;	//make sure positive
	return(i);
}

int bgbcc_strcmp(char *s1, char *s2)
{
	while(*s1 && *s2 && (*s1==*s2))
		{ s1++; s2++; }
	if(*s1>*s2)return(1);
//	if(*s2>*s1)return(-1);
	if(*s1<*s2)return(-1);
	return(0);
}

int bgbcc_stricmp(char *s1, char *s2)
{
	int i0, i1;

	i0=*s1++; i1=*s2++;
	if((i0>='a') && (i0<='z'))i0=i0-'a'+'A';
	if((i1>='a') && (i1<='z'))i1=i1-'a'+'A';

	while(i0 && i1)
	{
		if(i0!=i1)
			break;

		i0=*s1++; i1=*s2++;
		if((i0>='a') && (i0<='z'))i0=i0-'a'+'A';
		if((i1>='a') && (i1<='z'))i1=i1-'a'+'A';
	}

	if(i0>i1)return( 1);
	if(i0<i1)return(-1);
	return(0);
}

s64 bgbcc_strtol(char *str, int rdx)
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

s64 bgbcc_atoi(char *str)
{
	s64 li;
	char *s;
	int sg;
	
	if(*str=='0')
	{
		if(str[1]=='x')
			return(bgbcc_strtol(str+2, 16));
		if(str[1]=='d')
			return(bgbcc_strtol(str+2, 10));
		if(str[1]=='b')
			return(bgbcc_strtol(str+2, 2));
		return(bgbcc_strtol(str+1, 8));
	}
	
	s=str; sg=0;
	if(*s=='-')
		{ s++; sg=1; }
	li=bgbcc_strtol(s, 10);
	if(sg)li=-li;
	return(li);
}

int bgbcc_strtoxl(char *str, int rdx, u64 *rxlo, u64 *rxhi)
{
	return(bgbcc_strtoxl2(str, rdx, rxlo, rxhi, NULL));
}

int bgbcc_strtoxl2(char *str, int rdx, u64 *rxlo, u64 *rxhi, short *rdpct)
{
	char *s;
//	u64 tl, tm, th;
	u64 tva, tvb, tvc, tvd;
	int i, j, dpct;
	
//	s=str; tl=0; tm=0; th=0; dpct=-9999;
	s=str; tva=0; tvb=0; tvc=0; tvd=0; dpct=-9999;
	while(*s)
	{
		if(*s=='_')
			{ s++; continue; }

		if((*s=='.') && rdpct)
		{
			s++;
			dpct=0;
			continue;
		}

		i=*s++; j=-1;
		if((i>='0') && (i<='9'))
			j=0+(i-'0');
		if((i>='A') && (i<='Z'))
			j=10+(i-'A');
		if((i>='a') && (i<='z'))
			j=10+(i-'a');
		if((j<0) || (j>=rdx))
		{
			s--;
			break;
		}
//		tl=(tl*rdx)+j;
//		tm=(tm*rdx)+(tl>>32);
//		th=(th*rdx)+(tm>>32);
//		tl=(u32)tl;
//		tm=(u32)tm;

		tva=(tva*rdx)+j;
		tvb=(tvb*rdx)+(tva>>32);
		tvc=(tvc*rdx)+(tvb>>32);
		tvd=(tvd*rdx)+(tvc>>32);
		tva=(u32)tva;
		tvb=(u32)tvb;
		tvc=(u32)tvc;

		dpct++;
	}
	
	if(dpct<0)
		dpct=0;
	
//	*rxlo=tl|(tm<<32);
//	*rxhi=th;
	*rxlo=tva|(tvb<<32);
	*rxhi=tvc|(tvd<<32);
	if(rdpct)
		*rdpct=dpct;
	
	if(tvd>>32)
	{
		/* Exceeds 128-bit range... */
		return(-1);
	}
	
	if(*s)
	{
		
		return(0);
	}
	
	return(1);
}

int bgbcc_atoxl(char *str, u64 *rxlo, u64 *rxhi)
{
	u64 t0, t1, t2;
	s64 li;
	char *s;
	int sg;
	int i;
	
	if(*str=='0')
	{
		if(str[1]=='x')
			return(bgbcc_strtoxl(str+2, 16, rxlo, rxhi));
		if(str[1]=='d')
			return(bgbcc_strtoxl(str+2, 10, rxlo, rxhi));
		if(str[1]=='b')
			return(bgbcc_strtoxl(str+2, 2, rxlo, rxhi));
		return(bgbcc_strtoxl(str+1, 8, rxlo, rxhi));
	}
	
	s=str; sg=0;
	if(*s=='-')
		{ s++; sg=1; }
	i=bgbcc_strtoxl(s, 10, rxlo, rxhi);
	if(sg)
	{
//		li=-li;
		t0=~(*rxlo);
		t1=~(*rxhi);
		
		t2=t0;
		t0++;
		if(((t2>>63)&1) && !((t0>>63)&1))
			t1++;

		*rxlo=t0;
		*rxhi=t1;
	}
	return(i);
}

f64 bgbcc_atof(char *str)
{
	return(atof(str));
}

int bgbcc_atoxf(char *str, u64 *rxlo, u64 *rxhi)
{
	bgbcc_vfloat128 c;
	u64 dxlo, dxhi, v;
	int dpct, dpshr;

//	bgbcc_strtoxl2(str, 10, dxlo, dxhi, dpct);
//	dpshr=dpct*217706;

#if 0
	*((double *)(&v))=atof(str);
	dxhi=((v>>4)&0x07FFFFFFFFFFFFFFULL)+
		(  0x3C00000000000000ULL)+
		(v&0x8000000000000000ULL);
	dxlo=v<<60;
#endif
	
	c=BGBCC_Float128_FromString(str);
	dxlo=c.lo;
	dxhi=c.hi;
	
	*rxlo=dxlo;
	*rxhi=dxhi;
	return(0);
}

char *BGBCC_StrPrintUInt(char *t, u32 val)
{
	static u16 lutab[1024];
	char tb[16];
	char *t1;
	u32 v;
	int i, j, k;
	
	if(lutab[999]!=0x999)
	{
		for(i=0; i<1000; i++)
		{
			k=(i%10);
			k+=((i/10)%10)<<4;
			k+=(i/100)<<8;
			lutab[i]=k;
		}
	}
	
	if(!val)
	{
		*t++='0';
		return(t);
	}
	
	t1=tb;
	v=val;
	while(v>=1000)
	{
		i=v%1000;
		v=v/1000;
		j=lutab[i];
		*t1++='0'+((j   )&15);
		*t1++='0'+((j>>4)&15);
		*t1++='0'+((j>>8)&15);
	}

	i=v;
	j=lutab[i];
	if(j)
	{
		*t1++='0'+((j   )&15);
		if(j>>4)
		{
			*t1++='0'+((j>>4)&15);
			if(j>>8)
			{
				*t1++='0'+((j>>8)&15);
			}
		}
	}
	
	while(t1>tb)
		{ *t++=*(--t1); }
	*t=0;
	return(t);
}

char *BGBCC_StrPrintInt(char *t, s32 val)
{
	if(val<0)
	{
		*t++='-';
		return(BGBCC_StrPrintUInt(t, -val));
	}else
	{
		return(BGBCC_StrPrintUInt(t, val));
	}
}

char *BGBCC_StrPrintInt8X(char *t, u32 val)
{
	static char *hexchars="0123456789ABCDEF";
	*t++=hexchars[(val>>28)&15];
	*t++=hexchars[(val>>24)&15];
	*t++=hexchars[(val>>20)&15];
	*t++=hexchars[(val>>16)&15];
	*t++=hexchars[(val>>12)&15];
	*t++=hexchars[(val>> 8)&15];
	*t++=hexchars[(val>> 4)&15];
	*t++=hexchars[(val>> 0)&15];
	*t=0;
	return(t);
}

char *BGBCC_StrPrintRawStr(char *t, char *s)
{
	while(*s)
		*t++=*s++;
	*t=0;
	return(t);
}

#if 1
int bgbcc_strtoxx2(char *str, int rdx, u64 *rxa, int *rdpct)
{
	char *s;
	u64 tvaa[64], tv;
	int i, j, k, dpct;
	
	memset(tvaa, 0, 64*sizeof(u64));
	
//	s=str; tl=0; tm=0; th=0; dpct=-9999;
	s=str; dpct=-99999;
	while(*s)
	{
		if(*s=='_')
			{ s++; continue; }

		if((*s=='.') && rdpct)
		{
			s++;
			dpct=0;
			continue;
		}

		i=*s++; j=-1;
		if((i>='0') && (i<='9'))
			j=0+(i-'0');
		if((i>='A') && (i<='Z'))
			j=10+(i-'A');
		if((i>='a') && (i<='z'))
			j=10+(i-'a');
		if((j<0) || (j>=rdx))
			break;

		tvaa[0]=(tvaa[0]*rdx)+j;
		for(k=1; k<64; k++)
			tvaa[k]=(tvaa[k]*rdx)+(tvaa[k-1]>>32);
		for(k=0; k<64; k++)
			tvaa[k]=(u32)tvaa[k];
		dpct++;
	}
	
	if(dpct<0)
		dpct=0;
	
	for(k=0; k<32; k++)
	{
		tv=tvaa[k*2+0]|(tvaa[k*2+1]<<32);
		rxa[k]=tv;
		if(!tv)
		{
			for(j=k+1; j<32; j++)
			{
				tv=tvaa[j*2+0]|(tvaa[j*2+1]<<32);
				if(tv)
					break;
			}
			if(j>=32)
				break;
		}
	}
	
	*rdpct=0;
	return(k);
}

int bgbcc_strtoxtx2(char *str, int rdx, u64 *rxa, u64 *rxb, int *rdpct)
{
	char *s;
	u64 tvaa[64], tvab[64], tv, tva, tvb;
	int i, j, k, jm, dpct;
	
	memset(tvaa, 0, 64*sizeof(u64));
	memset(tvab, 0, 64*sizeof(u64));
	
//	s=str; tl=0; tm=0; th=0; dpct=-9999;
	s=str; dpct=-99999;
	while(*s)
	{
		if(*s=='_')
			{ s++; continue; }

		if((*s=='.') && rdpct)
		{
			s++;
			dpct=0;
			continue;
		}

		jm=0;

		i=*s++; j=-1;
		if((i>='0') && (i<='9'))
			j=0+(i-'0');
		if((i>='A') && (i<='W'))
			j=10+(i-'A');
		if((i>='a') && (i<='w'))
			j=10+(i-'a');
		
		if((i=='x') || (i=='X'))
			{ j=rdx-1; jm=rdx-1; }
		if((i=='z') || (i=='Z'))
			{ j=0; jm=rdx-1; }
		
		if((j<0) || (j>=rdx))
			break;

		tvaa[0]=(tvaa[0]*rdx)+j;
		tvab[0]=(tvab[0]*rdx)+jm;
		for(k=1; k<64; k++)
		{
			tvaa[k]=(tvaa[k]*rdx)+(tvaa[k-1]>>32);
			tvab[k]=(tvab[k]*rdx)+(tvab[k-1]>>32);
		}
		for(k=0; k<64; k++)
		{
			tvaa[k]=(u32)tvaa[k];
			tvab[k]=(u32)tvab[k];
		}
		dpct++;
	}
	
	if(dpct<0)
		dpct=0;
	
	for(k=0; k<32; k++)
	{
		tva=tvaa[k*2+0]|(tvaa[k*2+1]<<32);
		tvb=tvab[k*2+0]|(tvab[k*2+1]<<32);
		rxa[k]=tva;
		rxb[k]=tvb;
		if(!tva && !tvb)
		{
			for(j=k+1; j<32; j++)
			{
				tva=tvaa[j*2+0]|(tvaa[j*2+1]<<32);
				tvb=tvab[j*2+0]|(tvab[j*2+1]<<32);
				if(tva||tvb)
					break;
			}
			if(j>=32)
				break;
		}
	}
	
	*rdpct=0;
	return(k);
}

int bgbcc_strtoxx(char *str, u64 *rxa, int *rdpct)
{
	char *s;
	u64 tv, tv1;
	int i, j, c, n, sg, rdx;
	
	rdx=10;
	
	s=str; sg=0;
	if(*s=='-')
		{ s++; sg=1; }

	if(*s=='0')
	{
		if((s[1]=='x') || (s[1]=='X'))
		{
			rdx=16;
			s+=2;
		}
		else if((s[1]=='b') || (s[1]=='B'))
		{
			rdx=2;
			s+=2;
		}
		else if((s[1]=='d') || (s[1]=='D'))
		{
			rdx=10;
			s+=2;
		}
		else if((s[1]=='c') || (s[1]=='C'))
		{
			rdx=8;
			s+=2;
		}else
		{
			rdx=8;
			s++;
		}
	}
		
	n=bgbcc_strtoxx2(s, rdx, rxa, rdpct);

	if(sg)
	{
		c=1;
		for(i=0; i<n; i++)
		{
			tv=~rxa[i];
			tv1=tv+c;
			rxa[i]=tv1;
			c=((tv>>63)^(tv1>>63))&1;
		}
	}
	return(n);
}


int bgbcc_strtoxtx(char *str, u64 *rxa, u64 *rxb, int *rdpct)
{
	char *s;
	u64 tv, tv1;
	int i, j, c, n, sg, rdx;
	
	rdx=10;
	
	s=str; sg=0;
	if(*s=='-')
		{ s++; sg=1; }

	if(*s=='0')
	{
		if((s[1]=='x') || (s[1]=='X'))
		{
			rdx=16;
			s+=2;
		}
		else if((s[1]=='b') || (s[1]=='B'))
		{
			rdx=2;
			s+=2;
		}
		else if((s[1]=='d') || (s[1]=='D'))
		{
			rdx=10;
			s+=2;
		}
		else if((s[1]=='c') || (s[1]=='C'))
		{
			rdx=8;
			s+=2;
		}else
		{
			rdx=8;
			s++;
		}
	}
		
	n=bgbcc_strtoxtx2(s, rdx, rxa, rxb, rdpct);

	if(sg)
	{
		c=1;
		for(i=0; i<n; i++)
		{
			tv=~rxa[i];
			tv^=rxb[i];
			tv1=tv+c;
			rxa[i]=tv1;
			c=((tv>>63)^(tv1>>63))&1;
		}
	}
	return(n);
}

int bgbcc_strtoxs(char *str, char *rxb)
{
	u64 tva[64];
	u64 tv;
	char *t;
	int i, j, k, n, pct;

	n=bgbcc_strtoxx(str, tva, &pct);

	t=rxb;
	for(i=0; i<n; i++)
	{
		tv=tva[i];
		t[ 0]='0'+((tv>> 0)&63);
		t[ 1]='0'+((tv>> 6)&63);
		t[ 2]='0'+((tv>>12)&63);
		t[ 3]='0'+((tv>>18)&63);
		t[ 4]='0'+((tv>>24)&63);
		t[ 5]='0'+((tv>>30)&63);
		t[ 6]='0'+((tv>>36)&63);
		t[ 7]='0'+((tv>>42)&63);
		t[ 8]='0'+((tv>>48)&63);
		t[ 9]='0'+((tv>>54)&63);
		t[10]='0'+((tv>>60)&63);
		t+=11;
	}
	*t=0;
	
	return(n);
}

int bgbcc_strtoxts(char *str, char *rxb)
{
	u64 tva[64], tvb[64];
	u64 tv;
	char *t;
	int i, j, k, n, pct;

	n=bgbcc_strtoxtx(str, tva, tvb, &pct);

	t=rxb;
	for(i=0; i<n; i++)
	{
		tv=tva[i];
		t[ 0]='0'+((tv>> 0)&63);
		t[ 1]='0'+((tv>> 6)&63);
		t[ 2]='0'+((tv>>12)&63);
		t[ 3]='0'+((tv>>18)&63);
		t[ 4]='0'+((tv>>24)&63);
		t[ 5]='0'+((tv>>30)&63);
		t[ 6]='0'+((tv>>36)&63);
		t[ 7]='0'+((tv>>42)&63);
		t[ 8]='0'+((tv>>48)&63);
		t[ 9]='0'+((tv>>54)&63);
		t[10]='0'+((tv>>60)&63);
		t+=11;

		tv=tvb[i];
		t[ 0]='0'+((tv>> 0)&63);
		t[ 1]='0'+((tv>> 6)&63);
		t[ 2]='0'+((tv>>12)&63);
		t[ 3]='0'+((tv>>18)&63);
		t[ 4]='0'+((tv>>24)&63);
		t[ 5]='0'+((tv>>30)&63);
		t[ 6]='0'+((tv>>36)&63);
		t[ 7]='0'+((tv>>42)&63);
		t[ 8]='0'+((tv>>48)&63);
		t[ 9]='0'+((tv>>54)&63);
		t[10]='0'+((tv>>60)&63);
		t+=11;
	}
	*t=0;
	
	return(n);
}

s64 bgbcc_abs(s64 v)
{
	if(v<0)
		return(-v);
	return(v);
}

#endif
