#include "bgbmid.h"

static void *bgbmid_ralloc_buf=NULL;
static int bgbmid_ralloc_pos=0;

static char *bgbmid_strtab=NULL;
static char *bgbmid_strtabe=NULL;
static char *bgbmid_estrtab=NULL;
static char **bgbmid_strhash;

void *(*bgbmid_fopen_fp)(char *name, char *mode);
void (*bgbmid_fclose_fp)(void *fd);
int (*bgbmid_fread_fp)(void *buf, int i, int j, void *fd);
int (*bgbmid_fwrite_fp)(void *buf, int i, int j, void *fd);
void (*bgbmid_fseek_fp)(void *buf, int i, int j);
int (*bgbmid_fgetc_fp)(void *fd);
int (*bgbmid_fputc_fp)(int i, void *fd);
int (*bgbmid_ftell_fp)(void *fd);
int (*bgbmid_feof_fp)(void *fd);
int (*bgbmid_fgets_fp)(void *buf, int sz, void *fd);

BMID_API void *bgbmid_ralloc(int sz)
{
	void *p;
	if(!bgbmid_ralloc_buf)bgbmid_ralloc_buf=bgbmid_malloc(262144);

	if((bgbmid_ralloc_pos+sz)>262144)
		bgbmid_ralloc_pos=0;
	p=(void *)(((byte *)bgbmid_ralloc_buf)+bgbmid_ralloc_pos);
	bgbmid_ralloc_pos=(bgbmid_ralloc_pos+sz+3)&(~3);

	return(p);
}

BMID_API char **bgbmid_split(char *s)
{
	char **a, *t;
	int i;

	a=bgbmid_ralloc(128*sizeof(char *));
	memset(a, 0, 128*sizeof(char *));
	i=0;
	while(*s)
	{
		while(*s && (*s<=' '))s++;
		if(!*s)break;
		t=bgbmid_ralloc(64);
		a[i++]=t;

		if(*s=='"')
		{
			s++;
			while(*s && (*s!='"'))*t++=*s++;
			if(*s=='"')s++;

			*t++=0;
			continue;
		}

		while(*s && (*s>' '))
		{
			if((s[0]=='\\') && (s[1]==' '))s++;
			*t++=*s++;
		}
		*t++=0;
	}
	a[i]=NULL;

	return(a);
}

BMID_API char *bgbmid_rstrdup(char *s)
{
	char *t;

	t=bgbmid_ralloc(strlen(s)+1);
	strcpy(t, s);
	return(t);
}

BMID_API char *bgbmid_rgets(char **str)
{
	char *s, *s1, *b, *t;

	s=*str; s1=s;
	while(*s1 && (*s1!='\n') && (*s1!='\r'))s1++;

	b=bgbmid_ralloc((s1-s)+1);
	t=b;
	while(s<s1)*t++=*s++;
	*t++=0;

	if(*s=='\r')s++;
	if(*s=='\n')s++;
	*str=s;
	
	return(b);
}

#if 0
BMID_API char *bgbmid_strdup(char *s)
{
	char *t;

	if(!bgbmid_strtab)
	{
		bgbmid_strtab=bgbmid_malloc(1<<24);
		bgbmid_strtabe=bgbmid_strtab+1;
		*bgbmid_strtab='0';
	}

	if(!s)return(NULL);
	if(!*s)return(bgbmid_strtab);

	t=bgbmid_strtab+1;
	while(t<bgbmid_strtabe)
	{
		if(!strcmp(t, s))
			return(t);
		t+=strlen(t)+1;
	}

	strcpy(t, s);
	bgbmid_strtabe=t+strlen(t)+1;
	return(t);
}
#endif

BMID_API char *bgbmid_strdup(char *str)
{
	char *s, *t;
	int i;

	if(!str)
	{
//		*(int *)-1=-1;
		return(NULL);
	}
	if(!*str)return("");

	if(!bgbmid_strhash)
	{
//		bgbmid_strtab=bgbmid_malloc(1<<24);
		bgbmid_strtab=bgbmid_tyalloc("bgbmid_strtab_t", 1<<20);
		bgbmid_strtabe=bgbmid_strtab;
		bgbmid_estrtab=bgbmid_strtab+(1<<20);

		i=16384*sizeof(char *);
		bgbmid_strhash=bgbmid_malloc(i);
		memset(bgbmid_strhash, 0, i);
	}
	
	if((bgbmid_strtabe+(strlen(str)+2))>=bgbmid_estrtab)
	{
		bgbmid_strtab=bgbmid_tyalloc("bgbmid_strtab_t", 1<<20);
		bgbmid_strtabe=bgbmid_strtab;
		bgbmid_estrtab=bgbmid_strtab+(1<<20);
	}

	i=0; s=str;
	while(*s)i=i*251+(*s++);
//	i=((i*251)>>8)&0xFFF;
	i=((i*251)>>8)&0x3FFF;

	t=bgbmid_strhash[i];
	while(t)
	{
		s=(char *)(((char **)t)+1);
		t=*(char **)t;
		if(!strcmp(s, str))return(s);
	}

//	t=bgbmid_malloc(strlen(str)+1+sizeof(char *));
//	t=bgbmid_tyalloc("bgbmid_string_t", strlen(str)+1+sizeof(char *));

	t=bgbmid_strtabe;
	bgbmid_strtabe=t+strlen(str)+1+sizeof(char *);

	s=(char *)(((char **)t)+1);
	strcpy(s, str);

	*(char **)t=bgbmid_strhash[i];
	bgbmid_strhash[i]=t;
	return(s);
}


static void *bgbmid_fopen_i(char *name, char *mode)
{
	char tb[256];
	FILE *fd;

	fd=fopen(name, mode);
	if(fd)return(fd);

	sprintf(tb, "resource_dev/%s", name);
	fd=fopen(tb, mode);
	if(fd)return(fd);

//	sprintf(tb, "baseq2/%s", name);
//	fd=fopen(tb, mode);
//	if(fd)return(fd);

	sprintf(tb, "../resource_dev/%s", name);
	fd=fopen(tb, mode);
	if(fd)return(fd);

	return(NULL);
}
static void bgbmid_fclose_i(void *fd)
	{ fclose((FILE *)fd); }
static int bgbmid_fread_i(void *buf, int i, int j, void *fd)
	{ return(fread(buf, i, j, (FILE *)fd)); }
static int bgbmid_fwrite_i(void *buf, int i, int j, void *fd)
	{ return(fwrite(buf, i, j, (FILE *)fd)); }
static void bgbmid_fseek_i(void *fd, int i, int j)
	{ fseek((FILE *)fd, i, j); }
static int bgbmid_fgetc_i(void *fd)
	{ return(fgetc((FILE *)fd)); }
static int bgbmid_fputc_i(int i, void *fd)
	{ return(fputc(i, (FILE *)fd)); }
static int bgbmid_ftell_i(void *fd)
	{ return(ftell((FILE *)fd)); }
static int bgbmid_feof_i(void *fd)
	{ return(feof((FILE *)fd)); }
static int bgbmid_fgets_i(void *buf, int sz, void *fd)
	{ fgets(buf, sz, (FILE *)fd); return(0); }

BMID_API void bgbmid_file_init()
{
	bgbmid_fopen_fp=bgbmid_fopen_i;
	bgbmid_fclose_fp=bgbmid_fclose_i;
	bgbmid_fread_fp=bgbmid_fread_i;
	bgbmid_fwrite_fp=bgbmid_fwrite_i;
	bgbmid_fseek_fp=bgbmid_fseek_i;
	bgbmid_fgetc_fp=bgbmid_fgetc_i;
	bgbmid_fputc_fp=bgbmid_fputc_i;
	bgbmid_ftell_fp=bgbmid_ftell_i;
	bgbmid_feof_fp=bgbmid_feof_i;
	bgbmid_fgets_fp=bgbmid_fgets_i;
}

BMID_API void *bgbmid_fopen(char *name, char *mode)
{
	if(!bgbmid_fopen_fp)
		bgbmid_file_init();
	return(bgbmid_fopen_fp(name, mode));
}

BMID_API void bgbmid_fclose(void *fd)
	{ bgbmid_fclose_fp(fd); }

BMID_API int bgbmid_fread(void *buf, int i, int j, void *fd)
	{ return(bgbmid_fread_fp(buf, i, j, fd)); }
BMID_API int bgbmid_fwrite(void *buf, int i, int j, void *fd)
	{ return(bgbmid_fwrite_fp(buf, i, j, fd)); }
BMID_API void bgbmid_fseek(void *buf, int i, int j)
	{ bgbmid_fseek_fp(buf, i, j); }
BMID_API int bgbmid_fgetc(void *fd)
	{ return(bgbmid_fgetc_fp(fd)); }
BMID_API int bgbmid_fputc(int i, void *fd)
	{ return(bgbmid_fputc_fp(i, fd)); }
BMID_API int bgbmid_ftell(void *fd)
	{ return(bgbmid_ftell_fp(fd)); }
BMID_API int bgbmid_feof(void *fd)
	{ return(bgbmid_feof_fp(fd)); }
BMID_API int bgbmid_fgets(void *buf, int sz, void *fd)
	{ return(bgbmid_fgets_fp(buf, sz, fd)); }


BMID_API void bgbmid_set_fopen(void *fcn)
	{ bgbmid_fopen_fp=fcn; }
BMID_API void bgbmid_set_fclose(void *fcn)
	{ bgbmid_fclose_fp=fcn; }
BMID_API void bgbmid_set_fread(void *fcn)
	{ bgbmid_fread_fp=fcn; }
BMID_API void bgbmid_set_fwrite(void *fcn)
	{ bgbmid_fwrite_fp=fcn; }
BMID_API void bgbmid_set_fseek(void *fcn)
	{ bgbmid_fseek_fp=fcn; }
BMID_API void bgbmid_set_fgetc(void *fcn)
	{ bgbmid_fgetc_fp=fcn; }
BMID_API void bgbmid_set_fputc(void *fcn)
	{ bgbmid_fputc_fp=fcn; }
BMID_API void bgbmid_set_ftell(void *fcn)
	{ bgbmid_ftell_fp=fcn; }
BMID_API void bgbmid_set_feof(void *fcn)
	{ bgbmid_feof_fp=fcn; }
BMID_API void bgbmid_set_fgets(void *fcn)
	{ bgbmid_fgets_fp=fcn; }
