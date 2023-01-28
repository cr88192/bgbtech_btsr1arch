#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

//#ifdef linux
//#include <sys/utsname.h>
//#endif

#include <bgbccc.h>

char *bgbcc_loads[4096];
int bgbcc_nloads=0;

char *bgbcc_src[16];	//source paths
int bgbcc_nsrc;		//num source paths

char *bgbcc_inc[16];	//source paths
int bgbcc_ninc;		//num source paths

char *bgbcc_lib[16];	//source paths
int bgbcc_nlib;		//num source paths

int bgbcc_arch;
int bgbcc_subarch;

int bgbcc_argc;
char **bgbcc_argv;

u32 bgbcc_gshash;

char *bgbcc_imgname;

byte bgbcc_dumpast;
byte bgbcc_optmode;

int bgbcc_msec_pp;
int bgbcc_msec_cp;
int bgbcc_msec_cc;
int bgbcc_msec_tot;

char *bgbcc_opts[256];
int bgbcc_nopts;

int bgbcc_istool;
int bgbcc_verbose;

#if 0
// BGBCC_API int BGBCC_BindSig(BGBCC_State *ctx, char *name, char *sig)
{
	char tb[256];

	DYLL_MetaPath_BindKey(name, "func");
	sprintf(tb, "%s:sig", name);
	DYLL_MetaPath_BindKey(tb, sig);

	return(0);
}

// int BGBCC_BindTypeSig(BGBCC_State *ctx, char *name, char *sig)
{
	char tb[256];

	DYLL_MetaPath_BindKey(name, "type");
	sprintf(tb, "%s:sig", name);
	DYLL_MetaPath_BindKey(tb, sig);
	return(0);
}

// int BGBCC_BindVarSig(BGBCC_State *ctx, char *name, char *sig)
{
	char tb[256];
	char *ts;

	ts="var";
	if(ctx->cur_struct)ts="field";

	DYLL_MetaPath_BindKey(name, ts);
	sprintf(tb, "%s:sig", name);
	DYLL_MetaPath_BindKey(tb, sig);
	return(0);
}

// int BGBCC_BindVarInfo(BGBCC_State *ctx, char *name, char *key, char *val)
{
	char tb[256];

	sprintf(tb, "%s:%s", name, key);
	DYLL_MetaPath_BindKey(tb, val);
	return(0);
}

// int BGBCC_BindTypeInfo(BGBCC_State *ctx, char *name, char *key, char *val)
	{ return(BGBCC_BindVarInfo(ctx, name, key, val)); }

// int BGBCC_BindDyTypeSig(BGBCC_State *ctx, char *name, char *sig)
{
	char tb[256];

	sprintf(tb, "bgbdy/%s", name);
	DYLL_MetaPath_BindKey(tb, "dytype");
	sprintf(tb, "bgbdy/%s:sig", name);
	DYLL_MetaPath_BindKey(tb, sig);
	return(0);
}

#endif


char *BGBCP_LookupSig(BGBCP_ParseState *ctx, char *name)
{
	char tb[256];
	char *s;

	s=DYLL_MetaPath_LookupKey(name);
	if(!s)return(NULL);
	if(strcmp(s, "func"))return(NULL);

	sprintf(tb, "%s:sig", name);
	s=DYLL_MetaPath_LookupKey(tb);
	return(s);
}

char *BGBCP_LookupTypeSig(BGBCP_ParseState *ctx, char *name)
{
	char tb[256];
	char *s;

	s=DYLL_MetaPath_LookupKey(name);

	if(!s)return(NULL);
	if(strcmp(s, "type"))return(NULL);

	sprintf(tb, "%s:sig", name);
	s=DYLL_MetaPath_LookupKey(tb);
	return(s);
}

#if 0
// int BGBCC_BindStructSig(BGBCC_State *ctx, char *name, char *sig)
{
	char tb[256];

	DYLL_MetaPath_BindKey(name, sig);

	return(0);
}

// int BGBCC_BindStructInfo(BGBCC_State *ctx,
	char *name, char *key, char *val)
{
	char tb[256];

	sprintf(tb, "%s:%s", name, key);
	DYLL_MetaPath_BindKey(tb, val);
	return(0);
}

// int BGBCC_BindStructInfoIdx(BGBCC_State *ctx,
	char *name, char *key, int idx, char *val)
{
	char tb[256];

	sprintf(tb, "%s:%s.%d", name, key, idx);
	DYLL_MetaPath_BindKey(tb, val);
	return(0);
}

// int BGBCC_BindFieldSig(BGBCC_State *ctx, char *cls, char *name, char *sig)
{
	char tb[256];

	sprintf(tb, "%s/%s", cls, name);
	DYLL_MetaPath_BindKey(tb, "field");

	sprintf(tb, "%s/%s:sig", cls, name);
	DYLL_MetaPath_BindKey(tb, sig);
	return(0);
}

// int BGBCC_BindFieldInfo(BGBCC_State *ctx,
	char *cls, char *name, char *key, char *val)
{
	char tb[256];

	sprintf(tb, "%s/%s:%s", cls, name, key);
	DYLL_MetaPath_BindKey(tb, val);
	return(0);
}

// int BGBCC_BindStructInfoI(BGBCC_State *ctx,
	char *name, char *key, int val)
{
	char tb[64];

	sprintf(tb, "%d", val);
	BGBCC_BindStructInfo(ctx, name, key, tb);
	return(0);
}

// int BGBCC_BindFieldInfoI(BGBCC_State *ctx,
	char *cls, char *name, char *key, int val)
{
	char tb[64];

	sprintf(tb, "%d", val);
	BGBCC_BindFieldInfo(ctx, cls, name, key, tb);
	return(0);
}
#endif

char *BGBCP_LookupPPKey(BGBCP_ParseState *ctx, char *name)
{
	char tb[256];
	char *s;

	sprintf(tb, "CPreProc/%s", name);
//	s=DYLL_MetaPath_LookupKey(tb);
	s=dyllMetaLookupKey(tb);
	return(s);
}

int BGBCC_BindPPKey(BGBCP_ParseState *ctx, char *name, char *sig)
{
	char tb[256];

	if(!BGBCC_BindIsMetaP(ctx))
		return(0);

	sprintf(tb, "CPreProc/%s", name);

//	DYLL_MetaPath_BindKey(tb, sig);
	dyllMetaBindKey(tb, sig);
	return(0);
}

/* Should we preserve metadata? */
int BGBCC_BindIsMetaP(BGBCP_ParseState *ctx)
{
	return(0);
}

int BGBCC_StoreTextFile(char *name, char *buf)
{
	FILE *fd;

	fd=fopen(name, "wt");
	if(fd)
	{
		fwrite(buf, 1, strlen(buf), fd);
		fclose(fd);
	}

	return(0);
}

int BGBCC_StoreFile(char *name, void *buf, int sz)
{
	FILE *fd;

	fd=fopen(name, "wb");
	if(!fd)return(-1);
	
	if(fd)
	{
		fwrite(buf, 1, sz, fd);
		fclose(fd);
	}

	return(0);
}

char *bgbcc_protos_c[] =
{
"void       __debugbreak();\n",
"void       __hint_use_egpr();\n",
"int        __int_min(int x, int y);\n",
"int        __int_max(int x, int y);\n",
"int        __int_clamp(int x, int min, int max);\n",

"__uint32   __float32_getbits(float val);\n",
"float      __float32_frombits(__uint32 val);\n",
"__uint64   __float64_getbits(double val);\n",
"double     __float64_frombits(__uint64 val);\n",

"__uint64   __object_getbits(__object val);\n",
"__object   __object_frombits(__uint64 val);\n",
"__uint64   __variant_getbits(__variant val);\n",
"__variant  __variant_frombits(__uint64 val);\n",
"void	   *__operator_new(int sz);\n",
"void       __lvo_throw(__object exc);\n",
"__variant  __lvo_getslot(__object obj, char *name);\n",
"void       __lvo_setslot(__object obj, char *name, __variant val);\n",
"int        __lvo_get_length(__object obj);\n",
"__variant  __lvo_methodcall(__object obj, char *name, __object args);\n",

"__object   __lvo_newvararray_0(void);\n",
"__object   __lvo_newvararray_1(__object a0);\n",
"__object   __lvo_newvararray_2(__object a0, __object a1);\n",
"__object   __lvo_newvararray_3(__object a0, __object a1, __object a2);\n",
"__object   __lvo_newvararray_4("
	"__object a0, __object a1, __object a2, __object a3);\n",
"__object   __lvo_newvararray_5("
	"__object a0, __object a1, __object a2, __object a3, "
	"__object a4);\n",
"__object   __lvo_newvararray_6("
	"__object a0, __object a1, __object a2, __object a3, "
	"__object a4, __object a5);\n",
"__object   __lvo_newvararray_7("
	"__object a0, __object a1, __object a2, __object a3, "
	"__object a4, __object a5, __object a6);\n",
"__object   __lvo_newvararray_8("
	"__object a0, __object a1, __object a2, __object a3, "
	"__object a4, __object a5, __object a6, __object a7);\n",
"__object   __lvo_newvararray_n(int n, ...);\n",

"int        __lvo_loadindex_i(__object obj, int idx);\n",
"__int64    __lvo_loadindex_l(__object obj, int idx);\n",
"float      __lvo_loadindex_f(__object obj, int idx);\n",
"double     __lvo_loadindex_d(__object obj, int idx);\n",
"void      *__lvo_loadindex_p(__object obj, int idx);\n",
"__object   __lvo_loadindex_v(__object obj, int idx);\n",
"int        __lvo_loadindex_sb(__object obj, int idx);\n",
"int        __lvo_loadindex_ub(__object obj, int idx);\n",
"int        __lvo_loadindex_ss(__object obj, int idx);\n",
"int        __lvo_loadindex_us(__object obj, int idx);\n",
"__uint32   __lvo_loadindex_ui(__object obj, int idx);\n",
"__uint64   __lvo_loadindex_ul(__object obj, int idx);\n",
"__variant  __lvo_loadindex_va(__object obj, int idx);\n",
"void       __lvo_storeindex_i(__object obj, int idx, int val);\n",
"void       __lvo_storeindex_l(__object obj, int idx, __int64 val);\n",
"void       __lvo_storeindex_f(__object obj, int idx, float val);\n",
"void       __lvo_storeindex_d(__object obj, int idx, double val);\n",
"void       __lvo_storeindex_p(__object obj, int idx, void *val);\n",
"void       __lvo_storeindex_v(__object obj, int idx, __object val);\n",
"void       __lvo_storeindex_b(__object obj, int idx, int val);\n",
"void       __lvo_storeindex_s(__object obj, int idx, int val);\n",
"void       __lvo_storeindex_va(__object obj, int idx, __variant val);\n",

"__object __lvo_newarray_sb_0(void);\n",
"__object __lvo_newarray_sb_1(int a0);\n",
"__object __lvo_newarray_sb_2(int a0, int a1);\n",
"__object __lvo_newarray_sb_3(int a0, int a1, int a2);\n",
"__object __lvo_newarray_sb_4(int a0, int a1, int a2, int a3);\n",
"__object __lvo_newarray_sb_5(int a0, int a1, int a2, int a3, int a4);\n",
"__object __lvo_newarray_sb_6(int a0, int a1, int a2, int a3, int a4, "
	"int a5);\n",
"__object __lvo_newarray_sb_7(int a0, int a1, int a2, int a3, int a4, "
	"int a5, int a6);\n",
"__object __lvo_newarray_sb_8(int a0, int a1, int a2, int a3, int a4, "
	"int a5, int a6, int a7);\n",
"__object __lvo_newarray_sb_n(int n, ...);\n",

"__object __lvo_newarray_ub_0(void);\n",
"__object __lvo_newarray_ub_1(int a0);\n",
"__object __lvo_newarray_ub_2(int a0, int a1);\n",
"__object __lvo_newarray_ub_3(int a0, int a1, int a2);\n",
"__object __lvo_newarray_ub_4(int a0, int a1, int a2, int a3);\n",
"__object __lvo_newarray_ub_5(int a0, int a1, int a2, int a3, int a4);\n",
"__object __lvo_newarray_ub_6(int a0, int a1, int a2, int a3, int a4, "
	"int a5);\n",
"__object __lvo_newarray_ub_7(int a0, int a1, int a2, int a3, int a4, "
	"int a5, int a6);\n",
"__object __lvo_newarray_ub_8(int a0, int a1, int a2, int a3, int a4, "
	"int a5, int a6, int a7);\n",
"__object __lvo_newarray_ub_n(int n, ...);\n",

"__object __lvo_newarray_ss_0(void);\n",
"__object __lvo_newarray_ss_1(int a0);\n",
"__object __lvo_newarray_ss_2(int a0, int a1);\n",
"__object __lvo_newarray_ss_3(int a0, int a1, int a2);\n",
"__object __lvo_newarray_ss_4(int a0, int a1, int a2, int a3);\n",
"__object __lvo_newarray_ss_5(int a0, int a1, int a2, int a3, int a4);\n",
"__object __lvo_newarray_ss_6(int a0, int a1, int a2, int a3, int a4, "
	"int a5);\n",
"__object __lvo_newarray_ss_7(int a0, int a1, int a2, int a3, int a4, "
	"int a5, int a6);\n",
"__object __lvo_newarray_ss_8(int a0, int a1, int a2, int a3, int a4, "
	"int a5, int a6, int a7);\n",
"__object __lvo_newarray_ss_n(int n, ...);\n",

"__object __lvo_newarray_us_0(void);\n",
"__object __lvo_newarray_us_1(int a0);\n",
"__object __lvo_newarray_us_2(int a0, int a1);\n",
"__object __lvo_newarray_us_3(int a0, int a1, int a2);\n",
"__object __lvo_newarray_us_4(int a0, int a1, int a2, int a3);\n",
"__object __lvo_newarray_us_5(int a0, int a1, int a2, int a3, int a4);\n",
"__object __lvo_newarray_us_6(int a0, int a1, int a2, int a3, int a4, "
	"int a5);\n",
"__object __lvo_newarray_us_7(int a0, int a1, int a2, int a3, int a4, "
	"int a5, int a6);\n",
"__object __lvo_newarray_us_8(int a0, int a1, int a2, int a3, int a4, "
	"int a5, int a6, int a7);\n",
"__object __lvo_newarray_us_n(int n, ...);\n",

"__object __lvo_newarray_si_0(void);\n",
"__object __lvo_newarray_si_1(int a0);\n",
"__object __lvo_newarray_si_2(int a0, int a1);\n",
"__object __lvo_newarray_si_3(int a0, int a1, int a2);\n",
"__object __lvo_newarray_si_4(int a0, int a1, int a2, int a3);\n",
"__object __lvo_newarray_si_5(int a0, int a1, int a2, int a3, int a4);\n",
"__object __lvo_newarray_si_6(int a0, int a1, int a2, int a3, int a4, "
	"int a5);\n",
"__object __lvo_newarray_si_7(int a0, int a1, int a2, int a3, int a4, "
	"int a5, int a6);\n",
"__object __lvo_newarray_si_8(int a0, int a1, int a2, int a3, int a4, "
	"int a5, int a6, int a7);\n",
"__object __lvo_newarray_si_n(int n, ...);\n",

"__object __lvo_newarray_ui_0(void);\n",
"__object __lvo_newarray_ui_1(int a0);\n",
"__object __lvo_newarray_ui_2(int a0, int a1);\n",
"__object __lvo_newarray_ui_3(int a0, int a1, int a2);\n",
"__object __lvo_newarray_ui_4(int a0, int a1, int a2, int a3);\n",
"__object __lvo_newarray_ui_5(int a0, int a1, int a2, int a3, int a4);\n",
"__object __lvo_newarray_ui_6(int a0, int a1, int a2, int a3, int a4, "
	"int a5);\n",
"__object __lvo_newarray_ui_7(int a0, int a1, int a2, int a3, int a4, "
	"int a5, int a6);\n",
"__object __lvo_newarray_ui_8(int a0, int a1, int a2, int a3, int a4, "
	"int a5, int a6, int a7);\n",
"__object __lvo_newarray_ui_n(int n, ...);\n",

"__object __lvo_newarray_sl_0(void);\n",
"__object __lvo_newarray_sl_1(__int64 a0);\n",
"__object __lvo_newarray_sl_2(__int64 a0, __int64 a1);\n",
"__object __lvo_newarray_sl_3(__int64 a0, __int64 a1, __int64 a2);\n",
"__object __lvo_newarray_sl_4("
	"__int64 a0, __int64 a1, __int64 a2, __int64 a3);\n",
"__object __lvo_newarray_sl_5("
	"__int64 a0, __int64 a1, __int64 a2, __int64 a3, __int64 a4);\n",
"__object __lvo_newarray_sl_6("
	"__int64 a0, __int64 a1, __int64 a2, __int64 a3, __int64 a4, "
	"__int64 a5);\n",
"__object __lvo_newarray_sl_7("
	"__int64 a0, __int64 a1, __int64 a2, __int64 a3, __int64 a4, "
	"__int64 a5, __int64 a6);\n",
"__object __lvo_newarray_sl_8("
	"__int64 a0, __int64 a1, __int64 a2, __int64 a3, __int64 a4, "
	"__int64 a5, __int64 a6, __int64 a7);\n",
"__object __lvo_newarray_sl_n(int n, ...);\n",

"__object __lvo_newarray_ul_0(void);\n",
"__object __lvo_newarray_ul_1(__uint64 a0);\n",
"__object __lvo_newarray_ul_2(__uint64 a0, __uint64 a1);\n",
"__object __lvo_newarray_ul_3(__uint64 a0, __uint64 a1, __uint64 a2);\n",
"__object __lvo_newarray_ul_4("
	"__uint64 a0, __uint64 a1, __uint64 a2, int a3);\n",
"__object __lvo_newarray_ul_5("
	"__uint64 a0, __uint64 a1, __uint64 a2, __uint64 a3, __uint64 a4);\n",
"__object __lvo_newarray_ul_6("
	"__uint64 a0, __uint64 a1, __uint64 a2, __uint64 a3, __uint64 a4, "
	"__uint64 a5);\n",
"__object __lvo_newarray_ul_7("
	"__uint64 a0, __uint64 a1, __uint64 a2, __uint64 a3, __uint64 a4, "
	"__uint64 a5, __uint64 a6);\n",
"__object __lvo_newarray_ul_8("
	"__uint64 a0, __uint64 a1, __uint64 a2, __uint64 a3, __uint64 a4, "
	"__uint64 a5, __uint64 a6, __uint64 a7);\n",
"__object __lvo_newarray_ul_n(int n, ...);\n",

"__object __lvo_newarray_f_0(void);\n",
"__object __lvo_newarray_f_1(float a0);\n",
"__object __lvo_newarray_f_2(float a0, float a1);\n",
"__object __lvo_newarray_f_3(float a0, float a1, float a2);\n",
"__object __lvo_newarray_f_4(float a0, float a1, float a2, float a3);\n",
"__object __lvo_newarray_f_5("
	"float a0, float a1, float a2, float a3, float a4);\n",
"__object __lvo_newarray_f_6("
	"float a0, float a1, float a2, float a3, float a4, "
	"float a5);\n",
"__object __lvo_newarray_f_7("
	"float a0, float a1, float a2, float a3, float a4, "
	"float a5, float a6);\n",
"__object __lvo_newarray_f_8("
	"float a0, float a1, float a2, float a3, float a4, "
	"float a5, float a6, float a7);\n",
"__object __lvo_newarray_f_n(int n, ...);\n",

"__object __lvo_newarray_d_0(void);\n",
"__object __lvo_newarray_d_1(double a0);\n",
"__object __lvo_newarray_d_2(double a0, double a1);\n",
"__object __lvo_newarray_d_3(double a0, double a1, double a2);\n",
"__object __lvo_newarray_d_4(double a0, double a1, double a2, double a3);\n",
"__object __lvo_newarray_d_5("
	"double a0, double a1, double a2, double a3, double a4);\n",
"__object __lvo_newarray_d_6("
	"double a0, double a1, double a2, double a3, double a4, "
	"double a5);\n",
"__object __lvo_newarray_d_7("
	"double a0, double a1, double a2, double a3, double a4, "
	"double a5, double a6);\n",
"__object __lvo_newarray_d_8("
	"double a0, double a1, double a2, double a3, double a4, "
	"double a5, double a6, double a7);\n",
"__object __lvo_newarray_d_n(int n, ...);\n",

"__object __lvo_array_append_0(void);\n",
"__object __lvo_array_append_1(__object a0);\n",
"__object __lvo_array_append_2(__object a0, __object a1);\n",
"__object __lvo_array_append_3(__object a0, __object a1, __object a2);\n",
"__object __lvo_array_append_4("
	"__object a0, __object a1, __object a2, __object a3);\n",
"__object __lvo_array_append_5("
	"__object a0, __object a1, __object a2, __object a3, __object a4);\n",
"__object __lvo_array_append_6("
	"__object a0, __object a1, __object a2, __object a3, "
	"__object a4, __object a5);\n",
"__object __lvo_array_append_7("
	"__object a0, __object a1, __object a2, __object a3, "
	"__object a4, __object a5, __object a6);\n",
"__object __lvo_array_append_8("
	"__object a0, __object a1, __object a2, __object a3, "
	"__object a4, __object a5, __object a6, __object a7);\n",
"__object __lvo_array_append_n(int n, ...);\n",

NULL
};


int bgbcc_storefile(char *name, void *buf, int sz)
	{ return(BGBCC_StoreFile(name, buf, sz)); }

void *bgbcc_loadfile(char *name, int *rsz)
{
	FILE *fd;
	char *t;
	void *buf;
	int i, j, k, sz;

	if(name[0]=='$')
	{
		if(!strcmp(name, "$protos$.c"))
		{
			sz=0;
			for(i=0; bgbcc_protos_c[i]; i++)
				sz+=strlen(bgbcc_protos_c[i]);
		
			buf=bgbcc_malloc2(sz+8);
			t=buf;
			for(i=0; bgbcc_protos_c[i]; i++)
			{
				j=strlen(bgbcc_protos_c[i]);
				memcpy(t, bgbcc_protos_c[i], j+1);
				t+=j;
			}

			*rsz=sz;
			return(buf);
		}
	}

//	printf("bgbcc_loadfile: %s\n", name);

	fd=fopen(name, "rb");
	if(!fd)return(NULL);

	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);

//	buf=bgbcc_malloc(sz+16);
	buf=bgbcc_malloc2(sz+16);
	memset(buf, 0, sz+16);
	j=fread(buf, 1, sz, fd);
	if(j<sz)sz=j;

	fclose(fd);

	if(rsz)*rsz=sz;
	return(buf);
}

int bgbcc_loadfile_checkbinary(byte *buf)
{
	int i, j, k;

//	i=BGBCC_GET_U16LE(buf);
//	if(i==)
	if((buf[0]=='M') && (buf[1]=='Z'))
		return(1);
	if((buf[0]=='P') && (buf[1]=='E'))
		return(1);
	if((buf[0]=='\x7F') && (buf[1]=='E') &&
			(buf[2]=='L') && (buf[3]=='F'))
		return(1);
	if((buf[0]=='R') && (buf[1]=='I') &&
			(buf[2]=='L') && (buf[3]=='3'))
		return(1);
	if((buf[0]=='R') && (buf[1]=='I') &&
			(buf[2]=='F') && (buf[3]=='F'))
		return(1);

	return(0);
}

/*
Categorize buffer by characters:
  0: Plain ASCII Text
  1: Valid UTF-8 (BMP)
  2/3: Probably codepage text.
  4/5: UTF-8 (Non-BMP)
  8..15: Binary Data
 */
int bgbcc_loadfile_CheckCategory(byte *buf, int sz)
{
	byte *cs, *cse;
	int ret;
	int i, j, k;
	
	cs=buf; cse=buf+sz; ret=0;
	while(cs<cse)
	{
		i=*cs++;
		if((i>=0x20) && (i<0x7F))
			continue;
		if((i=='\r') || (i=='\n') || (i=='\t'))
			continue;
		if(i<0x20)
			{ ret|=8; continue; }
			
		if((i&0xE0)==0xC0)
		{
			j=*cs;
			if((j&0xC0)==0x80)
				{ ret|=1; cs++; continue; }
			ret|=2;
			continue;
		}
		if((i&0xF0)==0xE0)
		{
			j=cs[0];
			k=cs[1];
			if(((j&0xC0)==0x80) && ((k&0xC0)==0x80))
				{ ret|=1; cs+=2; continue; }
			ret|=2;
			continue;
		}
		if((i&0xF8)==0xF0)
		{
			j=cs[0];	k=cs[1];
			if(((j&0xC0)==0x80) && ((k&0xC0)==0x80) && ((cs[2]&0xC0)==0x80))
				{ ret|=4; cs+=3; continue; }
			ret|=2;
			continue;
		}
		ret|=2;
	}
	return(ret);
}

static const int bgbcc_tuc52[32]={
	0x20AC, 0x0081, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021,
	0x02C6, 0x2030, 0x0160, 0x2039, 0x0152, 0x008D, 0x017D, 0xFFFF,
	0x0090, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,
	0x02DC, 0x2122, 0x0161, 0x203A, 0x0153, 0x009C, 0x017E, 0x0178};

int bgbcc_asc2ucs(int v)
{
	if(!(v&0x80))
		return(v);
	if(v&0x60)
		return(v);
	return(bgbcc_tuc52[v&0x1F]);
}

void *bgbcc_loadfile_txt(char *name, int *rsz)
{
	byte *buf, *buf1, *s, *se, *t;
	int i, j, k, en, sz, sz1;

	buf=bgbcc_loadfile(name, &sz);
	buf1=NULL;

	if(buf && !bgbcc_loadfile_checkbinary(buf))
	{
		/* Attempt Normalization */
	
		en=-1;
		i=BGBCC_GET_U16LE(buf);
		j=BGBCC_GET_U16LE(buf+2);
		k=BGBCC_GET_U16LE(buf+4);
		if(i==0xFEFF)en=0;
		if(i==0xFFFE)en=1;
		
		if(!(i&0xFF00) && !(j&0xFF00) && !(k&0xFF00) &&
			(i&0x00FF) && (j&0x00FF) && (k&0x00FF))
				en=2;
		if(!(i&0x00FF) && !(j&0x00FF) && !(k&0x00FF) &&
			(i&0xFF00) && (j&0xFF00) && (k&0xFF00))
				en=3;
		
		if(en>=0)
		{
			/* File has UTF16 BOM or identified as UCS2; Convert to UTF8 */

			buf1=bgbcc_malloc2(sz*2);
			
			s=buf+((en&2)?0:2); se=buf+sz;
			t=buf1;
			
			while(s<se)
			{
				i=BGBCC_GET_U16EN(s+0, (en&1));
				j=BGBCC_GET_U16EN(s+2, (en&1));
				if((i=='\r') && (j=='\n'))
					{ s+=2; continue; }
				BGBCP_EmitChar((char **)(&t), i);
				s+=2;
			}
			
			sz1=t-buf1;
		}
		else
			if((buf[0]==0xEF) && (buf[1]==0xBB) && (buf[2]==0xBF))
		{
			/* File has UTF8 BOM, Copy data over it. */

			s=buf+3; se=buf+sz;
			t=buf;
			while(s<se)
			{
				i=*s++;
				if((i=='\r') && (s[1]=='\n'))
					continue;
				*t++=i;
			}
			*t=0;
			sz=sz-3;
		}else
		{
#if 1
			i=bgbcc_loadfile_CheckCategory(buf, sz);
//			printf("category: %s %d\n", name, i);
			
			if((i==0) || (i==1))
			{
				/* ASCII or UTF-8 */
				/* Normalize CRLF to LF */

				s=buf; se=buf+sz;
				t=buf;
				while(s<se)
				{
					if((s[0]=='\r') && (s[1]=='\n'))
						{ s++; continue; }
//					i=*s++;
//					*t++=i;
					i=BGBCP_ParseChar((char **)(&s));
//					if(i>0x80)
//						printf("U%04X\n", i);
					BGBCP_EmitChar((char **)(&t), i);
				}
				*t=0;
				sz=t-buf;
			}
			else if((i==2) || (i==3))
			{
				/* Codepage Text */

				buf1=bgbcc_malloc2(sz*2);
				s=buf; se=buf+sz;
				t=buf1;
				while(s<se)
				{
					i=*s++;
					i=bgbcc_asc2ucs(i);
					BGBCP_EmitChar((char **)(&t), i);
				}				
				sz1=t-buf1;
			}
			else if((i==4) || (i==5))
			{
				/* Non-BMP UTF-8 */

				buf1=bgbcc_malloc2(sz*2);
				s=buf; se=buf+sz;
				t=buf1;
				while(s<se)
				{
					if((s[0]=='\r') && (s[1]=='\n'))
						{ s++; continue; }
					i=BGBCP_ParseChar((char **)(&s));
					if(i>0xFFFF)
					{
						BGBCP_EmitChar((char **)(&t), 0xD800|((i>>10)&0x3FF));
						BGBCP_EmitChar((char **)(&t), 0xDC00|((i    )&0x3FF));
					}else
					{
						BGBCP_EmitChar((char **)(&t), i);
					}
				}				
				sz1=t-buf1;
			}else
			{
				/* Probably binary data or similar. */
				/* Leave as-is */
			}
#endif
		}
	}
	
	if(buf1)
	{
		bgbcc_free(buf);
		buf=bgbcc_malloc2(sz1+4);
		memcpy(buf, buf1, sz1);
		buf[sz1]=0;
		sz=sz1;
		bgbcc_free(buf1);
	}

	*rsz=sz;
	return(buf);
}

void *bgbcc_loadfile2(char *name, int *rsz)
{
	char tb[256];
	void *buf;
	int i;

//	buf=bgbcc_loadfile(name, rsz);
	buf=bgbcc_loadfile_txt(name, rsz);
	if(buf)return(buf);
	
	for(i=0; i<bgbcc_nsrc; i++)
	{
		sprintf(tb, "%s/%s", bgbcc_src[i], name);
//		buf=bgbcc_loadfile(tb, rsz);
		buf=bgbcc_loadfile_txt(tb, rsz);
		if(buf)return(buf);
	}

	for(i=0; i<bgbcc_ninc; i++)
	{
		sprintf(tb, "%s/%s", bgbcc_inc[i], name);
//		buf=bgbcc_loadfile(tb, rsz);
		buf=bgbcc_loadfile_txt(tb, rsz);
		if(buf)return(buf);
	}

	for(i=0; i<bgbcc_nlib; i++)
	{
		sprintf(tb, "%s/%s", bgbcc_lib[i], name);
//		buf=bgbcc_loadfile(tb, rsz);
		buf=bgbcc_loadfile_txt(tb, rsz);
		if(buf)return(buf);
	}
	
	return(NULL);
}

#if 0
int BGBCC_LoadCMeta(char *name)
{
	char tb[256], tb1[256];
	FILE *fd;
	BCCX_Node *t;
	byte *obj;
	char *s, *buf, *buf1, *buf2, *mod;
	int i, sz, t0, t1, dt;

	buf=bgbcc_loadfile(name, &sz);

	if(!buf)
	{
		for(i=0; i<bgbcc_nsrc; i++)
		{
			sprintf(tb, "%s/%s", bgbcc_src[i], name);
			buf=bgbcc_loadfile(tb, &sz);
			if(buf)break;
		}
	}

	if(!buf)
	{
		buf=BGBPP_LoadInclude(NULL, name, &sz);
	}

	if(!buf)
	{
		printf("BGBCC_LoadCMeta: fail load '%s'\n", name);
		return(-1);
	}

	if(bgbcc_verbose)
	{
		printf("BGBCC_LoadCMeta: %s %d bytes\n", name, sz);
	}

	strcpy(tb, name);
	s=tb;
	while(*s)
	{
		if(*s=='/')*s='_';
		if(*s=='\\')*s='_';
		if(*s==':')*s='_';
		s++;
	}

	while((s>tb) && (*s!='.'))s--;
	if(s>tb)*s=0;

	mod=bgbcc_strdup(tb);


	t0=clock();
	t=BGBCP_ModuleBuffer(name, mod, buf);

	if(!t)return(-1);

#if 1
	if(mod && bgbcc_dumpast)
	{
//#ifndef BGBCC_BCCX2
#if 1
		sprintf(tb1, "dump/%s_ast.txt", mod);
		fd=fopen(tb1, "wt");
		if(fd)
		{
			BCCX_PrintFD(fd, t);
			fclose(fd);
		}
#endif

#if 0
		buf1=malloc(1<<24);
		i=SBXE_WriteNodeList(buf1, t);
		printf("SBXE Encoded %d bytes\n", i);

		sprintf(tb1, "dump/%s_ast.sbxe", mod);
		fd=fopen(tb1, "wb");
		if(fd && (i>0))
		{
			fwrite(buf1, 1, i, fd);
			fclose(fd);
		}
		free(buf1);
#endif
	}
#endif

	t1=clock();
	dt=t1-t0;
	printf("BGBCC_LoadModule: Parse took %dms\n",
		(int)(1000*(dt/(float)CLOCKS_PER_SEC)));

//	printf("BGBCC_LoadModule: Parse OK\n");

#if 0
	buf1=BGBCC_CompileModule(mod, t);

//	BGBCC_GAstCompileModule(mod, t, &i);

	if(mod && buf1)
	{
		sprintf(tb1, "dump/%s_ril.txt", mod);
		BGBCC_StoreTextFile(tb1, buf1);
	}
#endif

#if 0
	buf1=BGBCC_BS2IL_CompileModule(mod, t);

	if(mod && buf1)
	{
		sprintf(tb1, "dump/%s_bs2il.txt", mod);
		BGBCC_StoreTextFile(tb1, buf1);
	}
#endif

//	DYLL_MetaPath_BindKey(name, "header");
//	dyllMetaBindKey(name, "header");

//	BGBGC_SaveSig();
	dyllMetaCommit();

//	BGBCC_CleanupAll();

//	BGBCC_UpdateCheckFile(name);

	bgbcc_loads[bgbcc_nloads++]=strdup(name);

//	free(buf1);
	return(0);
}
#endif

BCCX_Node *BGBCC_LoadCSourceAST(char *name)
{
	char tb[256], tb1[256];
	FILE *fd;
	BCCX_Node *t;
	byte *obj;
	char *s, *buf, *buf1, *buf2, *mod;
	int i, sz, t0, t1, dt;

	buf=bgbcc_loadfile2(name, &sz);

#if 0
	buf=bgbcc_loadfile(name, &sz);

	if(!buf)
	{
		for(i=0; i<bgbcc_nsrc; i++)
		{
			sprintf(tb, "%s/%s", bgbcc_src[i], name);
			buf=bgbcc_loadfile(tb, &sz);
			if(buf)break;
		}
	}

	if(!buf)
	{
		buf=BGBPP_LoadInclude(NULL, name, &sz);
	}

	if(!buf)
	{
		for(i=0; i<bgbcc_nlib; i++)
		{
			sprintf(tb, "%s/%s", bgbcc_lib[i], name);
			buf=bgbcc_loadfile(tb, &sz);
			if(buf)break;
		}
	}
#endif

	if(!buf)
	{
		printf("BGBCC_LoadCSourceAST: fail load '%s'\n", name);
		return(NULL);
	}

	if(bgbcc_verbose)
	{
//		printf("BGBCC_LoadCSourceAST: %s %d bytes\n", name, sz);
		printf("%s %d bytes\n", name, sz);
	}

	strcpy(tb, name);
	s=tb;
	while(*s)
	{
		if(*s=='/')*s='_';
		if(*s=='\\')*s='_';
		if(*s==':')*s='_';
		s++;
	}

	while((s>tb) && (*s!='.'))s--;
	if(s>tb)*s=0;

	mod=bgbcc_strdup(tb);

	t0=clock();
	t=BGBCP_ModuleBuffer(name, mod, buf);

	if(!t)return(NULL);

#if 1
// #ifndef BGBCC_BCCX2
	if(mod && bgbcc_dumpast)
	{
		sprintf(tb1, "dump/%s_ast.txt", mod);
		fd=fopen(tb1, "wt");
		if(fd)
		{
			BCCX_PrintFD(fd, t);
			fclose(fd);
		}
	}
#endif

	return(t);
}

#if 0
// int BGBCC_LoadCSourcesCCXL(
	char **names, int nnames,
	byte *obuf, int *rsz)
{
	return(0);
}
#endif

#if 1
int BGBCC_LoadCSourcesCCXL(
	char **names, int nnames,
	byte *obuf, int *rsz, fourcc imgfmt)
{
	char tb[256];
	BGBCC_TransState *ctx;
	int t0, t1, t2;
	char *asts_bsn[256];
	BCCX_Node *asts_bsa[256];
	BCCX_Node *t, *c, *n;
	byte *buf, *tb1;
	char *dllname;
	char *archsfx;
	char *s0, *s1;
	fourcc lang;
	int i, j, k, sz, omsz;
	int n_asts_bs;

	omsz=*rsz;
	
	s0=bgbcc_imgname;
	s0=s0+strlen(s0);
	while(s0>bgbcc_imgname)
	{
		if((*s0=='/') || (*s0=='\\'))
			break;
		s0--;
	}
	if((*s0=='/') || (*s0=='\\'))
		s0++;

	s1=tb;
	while(*s0 && (*s0!='.'))
		*s1++=*s0++;
	*s1++=0;
	
	dllname=bgbcc_strdup(tb);
	archsfx=BGBCP_NameSuffixForArch(bgbcc_arch);

//	BIPRO_ProfilerSetActive(1);

	ctx=bgbcc_malloc(sizeof(BGBCC_TransState));
	memset(ctx, 0, sizeof(BGBCC_TransState));

//	ctx->gs_seq=bgbcc_gshash;
//	bgbcc_gshash*=65521;
	bgbcc_gshash=bgbcc_gshash*65521+1;
	BGBCC_SeedGenSym(bgbcc_gshash);
	
	ctx->arch=bgbcc_arch;
	ctx->sub_arch=bgbcc_subarch;
	ctx->imgbasename=dllname;
	ctx->imgname=bgbcc_strdup(bgbcc_imgname);
	ctx->optmode=bgbcc_optmode;
	ctx->verbose=bgbcc_verbose;
	ctx->imgfmt=imgfmt;
	
	ctx->optstrs=bgbcc_opts;
	ctx->noptstrs=bgbcc_nopts;

	BGBCC_CCXL_SetupContextForArch(ctx);

	bgbcc_msec_pp=0;
	bgbcc_msec_cp=0;
	bgbcc_msec_cc=0;
	bgbcc_msec_tot=0;

//	if(1)
	if(imgfmt==BGBCC_IMGFMT_RIL3)
	{
		BGBCC_CCXLR3_BeginRecRIL(ctx);
	}
	
	n_asts_bs=0;

	for(i=0; i<nnames; i++)
	{
//		BGBCC_CCXL_CheckSanityGlobals(ctx);
	
//		printf("BGBCC_LoadCSourcesCCXL: %s\n", names[i]);

		if((names[i][0]=='-') && (names[i][1]=='l'))
		{
			if(archsfx)
			{
				sprintf(tb, "lib%s.%s.ril", names[i]+2, archsfx);
				buf=bgbcc_loadfile2(tb, &sz);
				if(buf)
				{
					BGBCC_CCXLR3_LoadBufferRIL(ctx, buf, sz);
					if(ctx->n_error)
						break;
					continue;
				}
			}
			
			sprintf(tb, "lib%s.ril", names[i]+2);
			buf=bgbcc_loadfile2(tb, &sz);
			if(buf)
			{
				BGBCC_CCXLR3_LoadBufferRIL(ctx, buf, sz);
				if(ctx->n_error)
					break;
				continue;
			}

			sprintf(tb, "%s.dll", names[i]+2);
			buf=bgbcc_loadfile2(tb, &sz);
			if(buf)
				{ BGBCC_CCXL_LoadBufferDLL(ctx, buf, sz); continue; }

			printf("BGBCC_LoadCSourcesCCXL: Can't Find Library %s\n", names[i]);
			continue;
		}

		lang=BGBCP_LangForName(names[i]);

#if 1
//		lang=BGBCP_LangForName(names[i]);
		if(	(lang==BGBCC_IMGFMT_RIL3) ||
			(lang==BGBCC_IMGFMT_OBJ))
		{
			buf=bgbcc_loadfile2(names[i], &sz);
			if(buf)
			{
				BGBCC_CCXLR3_LoadBufferRIL(ctx, buf, sz);
				if(ctx->n_error)
					break;
			}
			continue;
		}
#endif

#if 0
		if(lang==BGBCC_IMGFMT_DLL)
		{
			buf=bgbcc_loadfile2(names[i], &sz);
			if(buf)
				{ BGBCC_CCXL_LoadBufferDLL(ctx, buf, sz); }
			continue;
		}
#endif

		if( (lang==BGBCC_FMT_WAD) ||
			(lang==BGBCC_FMT_LUMP) ||
			(lang==BGBCC_FMT_WAV) ||
			(lang==BGBCC_FMT_BMP) ||
			(lang==BGBCC_FMT_AVI) )
		{
			buf=bgbcc_loadfile2(names[i], &sz);
			if(buf)
			{
				BGBCC_CCXL_AddResourceData(ctx,
					names[i], buf, sz, lang);
			}
			continue;
		}

		if(lang==BGBCC_FMT_DEF)
		{
			continue;
		}

		t=BGBCC_LoadCSourceAST(names[i]);
		if(!t)
			break;

//		BGBCC_CCXL_CheckSanityGlobals(ctx);

//		if(0)
		if((lang==BGBCC_LANG_CS) ||
			(lang==BGBCC_LANG_BS2))
		{
			BCCX_MarkTreeZone(t, BCCX_ZTY_GLOBAL);
		
			j=n_asts_bs++;
			asts_bsn[j]=names[i];
			asts_bsa[j]=t;
			BGBCC_CCXL_CompileModuleTopOnlyCTX(ctx, names[i], t);
		}else
		{
			t0=clock();
			BGBCC_CCXL_CompileModuleCTX(ctx, names[i], t);

#ifdef BGBCC_BCCX2
//			BCCX_ClearZoneLevel(BCCX_ZTY_GLOBAL);
			BCCX_ClearZoneLevel(BCCX_ZTY_NONGLOBAL);
#else
			BCCX_DeleteTree(t);
#endif

			BGBCC_CCXL_CheckSanityGlobals(ctx);
			BGBCC_CCXL_PrintTagWarn(ctx);
			
			t1=clock();
			t2=t1-t0;
	//		printf("Compile Module %dms\n", t2);

			bgbcc_msec_cc+=t2;
			bgbcc_msec_tot++;
			
			if(ctx->n_error)
				break;
			
#ifndef BGBCC_BCCX2
			c=ctx->reduce_tmp;
			ctx->reduce_tmp=NULL;
			while(c)
			{
				n=c->hnext;
				BCCX_DeleteTree(c);
				c=n;
			}
#endif
		}
	}
	
	if(i<nnames)
	{
		printf("Errors seen, aborting compilation\n");
		return(-1);
	}

	for(i=0; i<n_asts_bs; i++)
	{
		BGBCC_CCXL_CompileModuleTopOnlyCTX(ctx,
			asts_bsn[i], asts_bsa[i]);
		BGBCC_CCXL_CheckSanityGlobals(ctx);
		BGBCC_CCXL_PrintTagWarn(ctx);
	}
	
	for(i=0; i<n_asts_bs; i++)
	{
		t=asts_bsa[i];
		t0=clock();
		BGBCC_CCXL_CompileModuleCTX(ctx, asts_bsn[i], t);
#ifndef BGBCC_BCCX2
		BCCX_DeleteTree(t);
#endif

		BGBCC_CCXL_CheckSanityGlobals(ctx);

		BGBCC_CCXL_PrintTagWarn(ctx);

		t1=clock();
		t2=t1-t0;
//		printf("Compile Module %dms\n", t2);

		bgbcc_msec_cc+=t2;
		bgbcc_msec_tot++;
		
#ifndef BGBCC_BCCX2
		c=ctx->reduce_tmp;
		ctx->reduce_tmp=NULL;
		while(c)
		{
			n=c->hnext;
			BCCX_DeleteTree(c);
			c=n;
		}
#endif
	}

#ifdef BGBCC_BCCX2
	BCCX_ClearZoneLevel(BCCX_ZTY_GLOBAL);
#endif

	BGBCC_CCXL_PrintTagWarn(ctx);

//	if(ctx->ril_ip)
//	{
//		sprintf(tb, "dump/%s_ril3.dat", "tst");
//		BGBCC_StoreFile(tb, ctx->ril_ips, ctx->ril_ip-ctx->ril_ips);
//	}

	if(ctx->verbose)
	{
		if(bgbcc_msec_tot>0)
		{
			printf("Preproc Module tot=%dms, avg=%d\n",
				bgbcc_msec_pp, bgbcc_msec_pp/bgbcc_msec_tot);
			printf("Parse Module tot=%dms, avg=%d\n",
				bgbcc_msec_cp, bgbcc_msec_cp/bgbcc_msec_tot);
			printf("Compile Module tot=%dms, avg=%d\n",
				bgbcc_msec_cc, bgbcc_msec_cc/bgbcc_msec_tot);
		}
	}

	if(imgfmt==BGBCC_IMGFMT_RIL3)
	{
		sz=ctx->ril_ip-ctx->ril_ips;
		memcpy(obuf, ctx->ril_ips, sz);
		if(*rsz)*rsz=sz;
		return(0);
	}


//	if(ctx->ril_ip)
	if(0)
//	if(1)
	{
		tb1=BGBCC_FR2E_FlattenImage(ctx, &sz);
		sprintf(tb, "dump/%s_fr2e.dat", "tst");
		BGBCC_StoreFile(tb, tb1, sz);
	}

	BGBCC_CCXL_PrintTagWarn(ctx);

//	sz=BGBCC_FrBC_FlattenImage(ctx, obuf, omsz);

	sz=omsz;

	t0=clock();
	i=BGBCC_CCXL_FlattenImage(ctx, obuf, &sz, imgfmt);
	t1=clock();
	t2=t1-t0;
	if(ctx->verbose)
	{
		printf("Flatten Image %dms\n", t2);
	}

	BGBCC_CCXL_PrintTagWarn(ctx);

//	BIPRO_ProfilerSetActive(0);
//	BIPRO_ProfileDumpStats();

	if(i<0)return(i);
	if(*rsz)*rsz=sz;
	return(0);
}
#endif


char *bgbcc_share_base;
char *bgbcc_share_inc;
char *bgbcc_share_lib;

u32 BGBCC_GetArch()
	{ return(bgbcc_arch); }

u32 BGBCC_GetSubArch()
	{ return(bgbcc_subarch); }

int BGBCC_LoadConfig(char *name)
{
	char buf[256];
	char **a, *s;
	FILE *fd;
	int i;
	
	if(bgbcc_verbose)
	{
		printf("BGBCC_LoadConfig: Try %s\n", name);
	}
	
	fd=fopen(name, "rt");
	if(!fd)
	{
		if(bgbcc_verbose)
		{
			printf("BGBCC_LoadConfig: Fail %s\n", name);
		}
		return(-1);
	}
	
	while(!feof(fd))
	{
		s=fgets(buf, 256, fd);
		if(!s)break;
		a=bgbcc_split(buf);

		if(!a[0])continue;
		if(a[0][0]=='#')continue;
		if(a[0][0]==';')continue;
		if(a[0][0]=='/')continue;
		
		if(!strcmp(a[0], "include"))
		{
			for(i=1; a[i]; i++)
				{ ccAddInclude(a[i]); }
			continue;
		}

		if(!strcmp(a[0], "lib"))
		{
			for(i=1; a[i]; i++)
				{ ccAddLibrary(a[i]); }
			continue;
		}

		if(!strcmp(a[0], "define"))
		{
			for(i=1; a[i]; i++)
				{ ccAddDefineString(a[i]); }
			continue;
		}

		if(!strcmp(a[0], "arch"))
		{
			bgbcc_arch=BGBCP_ArchForName(a[1]);
			if(a[2])
				{ bgbcc_subarch=BGBCP_SubArchForName(bgbcc_arch, a[2]); }
			else
				{ bgbcc_subarch=BGBCP_SubArchForName(bgbcc_arch, a[1]); }
			continue;
		}

		if(!strcmp(a[0], "share_base"))
		{
			bgbcc_share_base=bgbcc_strdup(a[1]);
			continue;
		}

		if(!strcmp(a[0], "share_include"))
		{
			bgbcc_share_inc=bgbcc_strdup(a[1]);
			continue;
		}

		if(!strcmp(a[0], "share_lib"))
		{
			bgbcc_share_lib=bgbcc_strdup(a[1]);
			continue;
		}
	}

	if(bgbcc_verbose)
	{
		printf("BGBCC_LoadConfig: Done %s\n", name);
	}

	return(0);
}

int BGBCC_CheckArgIsAbsPath(char *str)
{
	char *s;
	int ispath;
	int c;
	
	if(
		(((str[0]>='A') && (str[0]<='Z')) ||
			((str[0]>='a') && (str[0]<='z'))) &&
		(str[1]==':') && ((str[2]=='\\') || (str[2]=='/')))
	{
		s=str+3;
	}else
		if(*str=='/')
	{
		s=str+1;
	}else
	{
		return(0);
	}
	
	ispath=0;
	while(*s)
	{
		if((*s=='=') || (*s==':') || (*s=='&') || (*s=='|') ||
			(*s=='<') || (*s=='>') || (*s=='\"') ||
			(*s=='?') || (*s=='*') || (*s<' '))
				{ ispath=0; break; }
		if(*s=='/')
			ispath=1; break;
		if(*s=='.')
			ispath=1; break;
	}
	return(ispath);
}

int BGBCC_InitEnv(int argc, char **argv, char **env)
{
	char *ta[16];
	char buf[256], tb[256];
	static int init=0;
	int inc_ok;
	int i, j, k, m, endian;
	char *s, *t;
	char *mach_name, *gcc_ver, *home, *base, *cfg, *prefix;
	char *usrlib_home;
	time_t tt0;
	struct tm *tm0;
//#ifdef linux
//	struct utsname utsbuf;
//#endif

	if(init)return(-1);
	init=1;

	mach_name=NULL; gcc_ver=NULL;
	cfg=NULL;
	
	bgbcc_nopts=0;

#if 0
#ifdef __GNUC__
	sprintf(buf, "%d.%d.%d",
		__GNUC__,  __GNUC_MINOR__,  __GNUC_PATCHLEVEL__);
	gcc_ver=strdup(buf);
#endif

#ifdef linux
//	uname(&utsbuf);
//	mach_name=utsbuf.machine;
#define MKSTR(arg) #arg
	mach_name=MACHTYPE;
	printf("MachName=%s\n", mach_name);
#endif
#endif

	home=NULL;
	base=NULL;
	prefix=NULL;
	bgbcc_share_base=NULL;
	usrlib_home=NULL;
	
	bgbcc_gshash=0;
	bgbcc_dumpast=0;
	bgbcc_optmode=BGBCC_OPT_DEFAULT;
//	bgbcc_verbose=1;
	bgbcc_verbose=0;

	bgbcc_istool=0;
	if(!strcmp(argv[0]+strlen(argv[0])-3, "-cc"))
		bgbcc_istool=1;
	if(!strcmp(argv[0]+strlen(argv[0])-4, "-gcc"))
		bgbcc_istool=1;

	if(!strcmp(argv[0]+strlen(argv[0])-3, "-ld"))
		bgbcc_istool=2;
	if(!strcmp(argv[0]+strlen(argv[0])-3, "-as"))
		bgbcc_istool=3;

	if(bgbcc_istool)
	{
		bgbcc_verbose=0;
//		printf("BGBCC_InitEnv: IsTool=%d\n", bgbcc_istool);
	}

	m=0;
	for(i=1; i<argc; i++)
	{
		for(j=0; argv[i][j]; j++)
			bgbcc_gshash=(bgbcc_gshash*65521+1)+argv[i][j];

		if((argv[i][0]=='-') && (argv[i][1]=='-'))
		{
			if(!strncmp(argv[i]+2, "cfg=", 4))
			{
				cfg=argv[i]+6;
				continue;
			}

			if(!strncmp(argv[i]+2, "home=", 5))
			{
				home=argv[i]+7;
				continue;
			}

			if(!strncmp(argv[i]+2, "usrlib_home=", 5))
			{
				usrlib_home=argv[i]+7;
				continue;
			}

			if(!strncmp(argv[i]+2, "locale=", 7))
			{
				BGBCP_SetDefaultLocale(argv[i]+9);
				continue;
			}
		}

#if 1
		if(((argv[i][0]=='/') || (argv[i][0]=='-')) &&
			!BGBCC_CheckArgIsAbsPath(argv[i]))
		{
			if(!strncmp(argv[i]+1, "FZx", 3))
			{
				m|=16;
				continue;
			}
			if(!strncmp(argv[i]+1, "FZy", 3))
			{
				m|=32;
				continue;
			}

			if(!strncmp(argv[i]+1, "Fi", 2) ||
				!strncmp(argv[i]+1, "Fe", 2) ||
				!strncmp(argv[i]+1, "Fo", 2) ||
				!strncmp(argv[i]+1, "Fa", 2))
			{
				m|=64;
				continue;
			}

			if(!strcmp(argv[i]+1, "o"))
			{
				m|=64;
				continue;
			}

			if(!strcmp(argv[i]+1, "v"))
			{
				bgbcc_verbose=1;
				continue;
			}

			if(!strncmp(argv[i]+1, "m", 1))
			{
				if(!strcmp(argv[i]+1, "m?"))
				{
					m|=128;
					continue;
				}
			
				mach_name=argv[i]+2;
				continue;
			}

			if(!strncmp(argv[i]+1, "cfg=", 4))
			{
				cfg=argv[i]+4;
				continue;
			}

			if(!strncmp(argv[i]+1, "home=", 5))
			{
				home=argv[i]+5;
				continue;
			}

			if(!strncmp(argv[i]+1, "usrlib_home=", 12))
			{
				usrlib_home=argv[i]+12;
				continue;
			}

			if(!strncmp(argv[i]+1, "locale=", 7))
			{
				BGBCP_SetDefaultLocale(argv[i]+7);
				continue;
			}

			if(!strncmp(argv[i]+1, "Zz", 2))
			{
				bgbcc_dumpast=1;
				bgbcc_verbose=1;
				continue;
			}

			if(!strcmp(argv[i]+1, "O2") ||
				!strcmp(argv[i]+1, "O3"))
			{
				bgbcc_optmode=BGBCC_OPT_SPEED2;
				continue;
			}

			if(!strcmp(argv[i]+1, "Os"))
			{
				bgbcc_optmode=BGBCC_OPT_SIZE;
				continue;
			}

			if(!strcmp(argv[i]+1, "O1"))
			{
//				bgbcc_optmode=BGBCC_OPT_SIZE;
				bgbcc_optmode=BGBCC_OPT_SPEED;
				continue;
			}

			if(!strcmp(argv[i]+1, "O0"))
			{
				bgbcc_optmode=BGBCC_OPT_DEBUG;
				continue;
			}
			
			if(argv[i][1]=='f')
			{
				bgbcc_opts[bgbcc_nopts++]=bgbcc_strdup(argv[i]+2);
				continue;
			}

			if(!strncmp(argv[i]+1, "stack=", 6))
			{
				bgbcc_opts[bgbcc_nopts++]=bgbcc_strdup(argv[i]+2);
				continue;
			}


			continue;
		}
#endif

#if 0
		if(argv[i][0]=='/')
		{
			if(!strncmp(argv[i], "/FZx", 4))
			{
				m|=16;
				continue;
			}
			if(!strncmp(argv[i], "/FZy", 4))
			{
				m|=32;
				continue;
			}

			if(!strncmp(argv[i], "/Fi", 3))
			{
				m|=64;
				continue;
			}

			if(!strncmp(argv[i], "/m", 2))
			{
				mach_name=argv[i]+2;
				continue;
			}

			if(!strncmp(argv[i], "/cfg=", 5))
			{
				cfg=argv[i]+5;
				continue;
			}

			continue;
		}

		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "--"))
				{ i++; break; }
			if(!strcmp(argv[i], "--pc"))
				m|=1;

			if(!strncmp(argv[i], "-I", 2))
			{
				ccAddInclude(argv[i]+2);
				continue;
			}
			if(!strncmp(argv[i], "-L", 2))
			{
				ccAddLibrary(argv[i]+2);
				continue;
			}
			if(!strncmp(argv[i], "-S", 2))
			{
				ccAddSource(argv[i]+2);
				continue;
			}

			if(!strncmp(argv[i], "-D", 2))
			{
				ccAddDefineString(argv[i]+2);
				continue;
			}
			
			if(!strncmp(argv[i], "-m", 2))
			{
				mach_name=argv[i]+2;
				continue;
			}

			if(!strncmp(argv[i], "-cfg=", 5))
			{
				cfg=argv[i]+5;
				continue;
			}

			continue;
		}
#endif

		k=BGBCP_LangForName(argv[i]);
		
		if(bgbcc_istool==1)
		{
			if((k==BGBCC_LANG_C) ||
				(k==BGBCC_LANG_CPP) ||
				(k==BGBCC_LANG_CS) ||
				(k==BGBCC_LANG_BS) ||
				(k==BGBCC_LANG_BS2) ||
				(k==BGBCC_LANG_JAVA) ||
				(k==BGBCC_LANG_ASM))
			{
				m|=64;
			}
		}
	}

	if(m&128)
	{
		BGBCP_DumpTargets();
	}

	inc_ok=0;
	for(i=0; env[i]; i++)
	{
//		printf("ENV %s\n", env[i]);
#if 0
//		if(!(m&32) && !(m&64) && 
		if(
			!strnicmp(env[i], "Include=", strlen("Include=")))
//		if(0)
		{
			s=env[i]+strlen("Include=");
			while(*s)
			{
				while(*s==' ')s++;

				t=buf;
				while(*s && (*s!=';'))
					*t++=*s++;
				*t++=0;

				while(*s==';')s++;

				if(buf[0])
				{
					ccAddInclude(buf);
					inc_ok=1;
				}
			}
		}
#endif

#if 0
		if(!strnicmp(env[i], "MACHTYPE=", strlen("MACHTYPE=")))
		{
			s=env[i]+strlen("MACHTYPE=");
			mach_name=s;
		}
#endif

#if 1
		if(!strnicmp(env[i], "HOME=", strlen("HOME=")))
		{
			s=env[i]+strlen("HOME=");
			home=s;
		}
#endif

#if 1
		if(!strnicmp(env[i], "PREFIX=", strlen("PREFIX=")))
		{
			s=env[i]+strlen("PREFIX=");
			prefix=s;
		}
#endif

#if 1
		if(!strnicmp(env[i], "USRLIB_HOME=", strlen("USRLIB_HOME=")))
		{
			s=env[i]+strlen("USRLIB_HOME=");
			usrlib_home=s;
		}
#endif
	}

#ifdef _WIN32
	GetModuleFileName(NULL, buf, 256);
#else
	strcpy(buf, argv[0]);
#endif

	if(bgbcc_verbose)
	{
		printf("BGBCC_InitEnv: Try %s\n", buf);
	}

	s=buf+strlen(buf);
	while((s>buf) && (*s!='/') && (*s!='\\'))
		s--;

	if(bgbcc_verbose)
	{
		printf("BGBCC_InitEnv: Try %s\n", s);
	}

	if(s>buf)
	{
		*s=0;
		base=bgbcc_strdup(buf);
	}

	if(bgbcc_verbose)
	{
		printf("BGBCC_InitEnv: Buf %s\n", buf);
	}

#ifdef linux
	if(!home)home="~";
	
	if(!base)
	{
		if(prefix)
		{
			sprintf(tb, "%s/bin", prefix);
			base=bgbcc_strdup(tb);
		}else
		{
			base=bgbcc_strdup("/usr/bin");
		}
	}
	
	if(!bgbcc_share_base)
	{
		if(usrlib_home)
		{
			bgbcc_share_base=bgbcc_strdup(usrlib_home);
		}else
			if(prefix)
		{
			sprintf(tb, "%s/share/bgbcc", prefix);
			bgbcc_share_base=bgbcc_strdup(tb);
		}else
		{
			bgbcc_share_base=bgbcc_strdup("/usr/share/bgbcc");
		}
	}
#endif

#ifdef _WIN32
	if(!bgbcc_share_base)
	{
		if(usrlib_home)
		{
			bgbcc_share_base=bgbcc_strdup(usrlib_home);
		}else
		{
		}
	}
#endif

	if(bgbcc_verbose)
	{
		if(home)
			printf("BGBCC_InitEnv: Home %s\n", home);
		if(base)
			printf("BGBCC_InitEnv: Base %s\n", base);
	}

	if(m&64)
	{
		if(base)
		{
			BGBCC_CCXL_SetBasePath(base);

//			sprintf(buf, "%s/frvmcfg.txt", base);
			sprintf(buf, "%s/bgbcc.cfg", base);
			BGBCC_LoadConfig(buf);

			sprintf(buf, "%s/../etc/bgbcc.cfg", base);
			BGBCC_LoadConfig(buf);
		}

		if(home)
		{
//			sprintf(buf, "%s/frvmcfg.txt", home);
			sprintf(buf, "%s/bgbcc.cfg", home);
			BGBCC_LoadConfig(buf);
		}
	}

	if(cfg)
	{
		if((*cfg!='/') && (*cfg!='\\'))
			{ sprintf(buf, "%s/%s", base, cfg); }
		else
			{ strcpy(buf, cfg); }
		BGBCC_LoadConfig(buf);
	}

	if(!mach_name && !bgbcc_arch)
	{
//		mach_name="SH4";
		mach_name="BJX2";
//		mach_name="BJX1L";
	}

	if(mach_name)
	{
		bgbcc_arch=BGBCP_ArchForName(mach_name);
		bgbcc_subarch=BGBCP_SubArchForName(bgbcc_arch, mach_name);
	}

	if(bgbcc_share_base && bgbcc_arch)
	{
		if(!bgbcc_share_inc)
		{
			sprintf(tb, "%s/include", bgbcc_share_base);

//			sprintf(tb, "%s/%s/include", bgbcc_share_base, 
//				BGBCP_NameSuffixForArch(bgbcc_arch));
			bgbcc_share_inc=bgbcc_strdup(tb);
		}

		if(!bgbcc_share_lib)
		{
//			sprintf(tb, "%s/%s/lib", bgbcc_share_base, 
//				BGBCP_NameSuffixForArch(bgbcc_arch));
			sprintf(tb, "%s/lib", bgbcc_share_base);
			bgbcc_share_lib=bgbcc_strdup(tb);
		}
	}

	if(bgbcc_share_inc)
	{
		ccAddInclude(bgbcc_share_inc);

		sprintf(tb, "%s/bgbcc", bgbcc_share_inc);
		ccAddInclude(tb);
	}

	if(bgbcc_share_lib)
	{
		ccAddLibrary(bgbcc_share_lib);

		sprintf(tb, "%s/bgbcc", bgbcc_share_lib);
		ccAddLibrary(tb);
	}

// #ifdef linux
#if 0
	if(gcc_ver && mach_name && !(m&64))
	{
		sprintf(buf, "/usr/lib/gcc/%s/%s/include", mach_name, gcc_ver);
		ccAddInclude(buf);
	}

	if(mach_name && !(m&64))
	{
		sprintf(buf, "/usr/%s/include", mach_name);
		ccAddInclude(buf);
	}

	if(!(m&64))
	{
		ccAddInclude("/usr/include");
		ccAddInclude("/usr/local/include");
	}
	inc_ok=1;
#endif

#if 0
	if(!inc_ok)
	{
		BGBPP_AddIncludePathFront("cc_usr/include");
		BGBPP_AddIncludePathFront("cc_usr/bscc/include");
//		BGBPP_AddIncludePathFront("build/include");
//		BGBPP_AddIncludePathFront("include");
//		BGBPP_AddIncludePathFront("");
	}
#endif

	BGBPP_AddStaticDefine(NULL, "_BGBMETA", "");
	BGBPP_AddStaticDefine(NULL, "_BGBCC", "");
	BGBPP_AddStaticDefine(NULL, "__BGBCC__", "");

	tt0 = time(NULL);
//	tm0 = localtime(&tt0);
	tm0 = gmtime(&tt0);

	//FIXME: do this properly
//	BGBPP_AddStaticDefine(NULL, "__DATE__", "\"Apr 01 2000\"");
//	BGBPP_AddStaticDefine(NULL, "__TIME__", "\"23:59:59\"");

	strftime(tb, 255, "\"%b %d %Y\"", tm0);
	BGBPP_AddStaticDefine(NULL, "__DATE__", strdup(tb));

	strftime(tb, 255, "\"%H:%M:%S\"", tm0);
	BGBPP_AddStaticDefine(NULL, "__TIME__", strdup(tb));

#if 0
//	if(!(m&64))
	if(0)
	{
#ifdef _WIN32
		BGBPP_AddStaticDefine(NULL, "_WIN32", "");
#endif

#ifdef linux
		BGBPP_AddStaticDefine(NULL, "linux", "");
#endif

#ifdef __i386__
		BGBPP_AddStaticDefine(NULL, "__i386__", "");
		bgbcc_arch=BGBCC_ARCH_X86;
#endif
#ifdef __x86_64__
		BGBPP_AddStaticDefine(NULL, "__x86_64__", "");
		bgbcc_arch=BGBCC_ARCH_X64;
#endif

#ifdef _M_IX86
//		BGBPP_AddStaticDefine(NULL, "_X86_", "");
		BGBPP_AddStaticDefine(NULL, "_M_IX86", "600");
		bgbcc_arch=BGBCC_ARCH_X86;
#endif
#ifdef _M_X64
		BGBPP_AddStaticDefine(NULL, "_M_X64", "");
		bgbcc_arch=BGBCC_ARCH_X64;
#endif

#ifdef __GNUC__
//		BGBPP_AddStaticDefine(NULL, "__GNUC__", "1");
#endif

		BGBPP_AddStaticDefine(NULL, "__inline__", "");

#if 1
#ifdef _MSC_VER
		sprintf(buf, "%d", _MSC_VER);
		BGBPP_AddStaticDefine(NULL, "_MSC_VER", buf);
#endif

#ifdef _MSC_FULL_VER
		sprintf(buf, "%d", _MSC_FULL_VER);
		BGBPP_AddStaticDefine(NULL, "_MSC_FULL_VER", buf);
#endif
#endif

#ifdef _INTEGRAL_MAX_BITS
		sprintf(buf, "%d", _INTEGRAL_MAX_BITS);
		BGBPP_AddStaticDefine(NULL, "_INTEGRAL_MAX_BITS", buf);
#endif

#ifdef _MSC_EXTENSIONS
		sprintf(buf, "%d", _MSC_EXTENSIONS);
		BGBPP_AddStaticDefine(NULL, "_MSC_EXTENSIONS", buf);
#endif
	}
#endif

#if 0
	if(bgbcc_arch==BGBCC_ARCH_SH)
	{
		BGBPP_AddStaticDefine(NULL, "__superh__", "");
		BGBPP_AddStaticDefine(NULL, "__sh__", "");
		
		if((bgbcc_subarch==BGBCC_ARCH_SH_SH2) ||
			(bgbcc_subarch==BGBCC_ARCH_SH_SH2L) ||
			(bgbcc_subarch==BGBCC_ARCH_SH_SH2B))
				BGBPP_AddStaticDefine(NULL, "__sh2__", "");
		if((bgbcc_subarch==BGBCC_ARCH_SH_SH4) ||
			(bgbcc_subarch==BGBCC_ARCH_SH_SH4L) ||
			(bgbcc_subarch==BGBCC_ARCH_SH_SH4B))
				BGBPP_AddStaticDefine(NULL, "__sh4__", "");

		endian=0;

		if((bgbcc_subarch==BGBCC_ARCH_SH_SH2L) ||
			(bgbcc_subarch==BGBCC_ARCH_SH_SH4L))
				endian=1;

		if((bgbcc_subarch==BGBCC_ARCH_SH_SH2B) ||
			(bgbcc_subarch==BGBCC_ARCH_SH_SH4B))
				endian=2;
				
		if((bgbcc_subarch==BGBCC_ARCH_SH_SH2) && !endian)
			endian=2;
		if((bgbcc_subarch==BGBCC_ARCH_SH_SH4) && !endian)
			endian=1;

		if(endian==1)
			BGBPP_AddStaticDefine(NULL, "__LITTLE_ENDIAN__", "");
		if(endian==2)
			BGBPP_AddStaticDefine(NULL, "__BIG_ENDIAN__", "");
	}
#endif

//	BGBPP_AddStaticDefine(NULL, "__STDC__", "1");
	BGBPP_AddStaticDefine(NULL, "__STDC_HOSTED__", "1");
	BGBPP_AddStaticDefine(NULL, "__STDC_VERSION__", "199901");

	return(0);
}

int BGBCC_Init()
	{ return(BGBCC_InitEnv(0, NULL, NULL)); }

void ccAddInclude(char *path)
{
	int i;

	if(bgbcc_verbose)
	{
		printf("Add include path '%s'\n", path);
	}

	BGBCC_Init();
	
	for(i=0; i<bgbcc_ninc; i++)
		if(!strcmp(bgbcc_inc[i], path))
			return;
	
	i=bgbcc_ninc++;
	bgbcc_inc[i]=bgbcc_strdup(path);
	BGBPP_AddIncludePathFront(path);
}

void ccAddLibrary(char *path)
{
	int i;

	if(bgbcc_verbose)
	{
		printf("Add library path '%s'\n", path);
	}

	BGBCC_Init();

	for(i=0; i<bgbcc_nlib; i++)
		if(!strcmp(bgbcc_lib[i], path))
			return;

	i=bgbcc_nlib++;
	bgbcc_lib[i]=bgbcc_strdup(path);
}

void ccAddSource(char *path)
{
	int i;

	BGBCC_Init();

	for(i=0; i<bgbcc_nsrc; i++)
		if(!strcmp(bgbcc_src[i], path))
			return;

	i=bgbcc_nsrc++;
	bgbcc_src[i]=bgbcc_strdup(path);
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

	BGBPP_AddStaticDefine(NULL, tb, tb2);
}

int help(char *arg0)
{
	printf("Usage: %s [options] [files]\n", arg0);
	printf("For many options, '-' and '/' are equivalent\n");
	printf("/D<name>[=val]      Define Name\n");
	printf("/I<path>            Add Include Path\n");
	printf("/L<path>            Add Library Path\n");
	printf("/S<path>            Add Source Path\n");
	printf("/Fn<name>           Metadata Output Name\n");
	printf("/Fw<name>           ExWAD Output Name\n");
	printf("/Fi<name>           Program Image Name\n");
	printf("/Fe<name>           Program Executable Name\n");
	printf("/m<arch>            Specify target arch\n");
	printf("/cfg=<name>         Set Config File\n");
	printf("/home=<path>        Set Home Path (Config Search)\n");
	return(0);
}

int main(int argc, char *argv[], char **env)
{
	char tb[256];
	char *uds[256], *args[256], *adds[256];
	ExWAD_Context *wad;
	char *metafn;
	char *wadfn, *frbcfn;
	byte *obuf;
	void *p;
	char *s, *t, *s0;
	fourcc fmt;
	int n, m, nuds, nargs, nadds, minuds;
	int is_compile_only;
	int t0, dt, te, sz;
	int i, j;

//	dy_init(&argc);

	BGBCC_InitEnv(argc, argv, env);

	BGBCC_SHXC_InitIface();
	BGBCC_BSRC_InitIface();
	BGBCC_JX2C_InitIface();
	
	BGBCC_Float128_Init();

	metafn=NULL;
	wadfn=NULL;
	frbcfn=NULL;

	nargs=0;
	args[nargs++]=argv[0];

	nadds=0;
	nuds=0;

	uds[nuds++]="$protos$.c";
	
	minuds=nuds;
	is_compile_only=0;

	n=0; m=0; nadds=0;
	for(i=1; i<argc; i++)
	{
#if 1
		if(((argv[i][0]=='/') || (argv[i][0]=='-')) &&
			!BGBCC_CheckArgIsAbsPath(argv[i]))
		{
			if(!strncmp(argv[i]+1, "I", 1))
			{
				ccAddInclude(argv[i]+2);
				continue;
			}
			if(!strncmp(argv[i]+1, "L", 1))
			{
				ccAddLibrary(argv[i]+2);
				continue;
			}
			if(!strncmp(argv[i]+1, "S", 1))
			{
				ccAddSource(argv[i]+2);
				continue;
			}

			if(!strncmp(argv[i]+1, "D", 1))
			{
				ccAddDefineString(argv[i]+2);
				continue;
			}

			if(!strncmp(argv[i]+1, "Fn", 2))
			{
				metafn=argv[i]+3;
				continue;
			}

			if(!strncmp(argv[i]+1, "Fw", 2))
			{
				wadfn=argv[i]+3;
				continue;
			}

			if(!strncmp(argv[i]+1, "Fi", 2) ||
				!strncmp(argv[i]+1, "Fe", 2) ||
				!strncmp(argv[i]+1, "Fo", 2) ||
				!strncmp(argv[i]+1, "Fa", 2))
			{
				frbcfn=argv[i]+3;
				continue;
			}

			if(!strcmp(argv[i]+1, "o"))
			{
				frbcfn=argv[i+1];
				i++;
				continue;
			}
			
			if(!strcmp(argv[i]+1, "c"))
			{
				is_compile_only=1;
				i++;
				continue;
			}

			if(!strncmp(argv[i]+1, "l", 1))
			{
				sprintf(tb, "-l%s", argv[i]+2);
//				sprintf(tb, "lib%s.ril", argv[i]+2);
//				uds[nuds++]=bgbcc_strdup(tb);

				for(j=0; j<nuds; j++)
				{
					if(!strcmp(uds[j], tb))
						break;
				}
				
				if(j<nuds)
					continue;

				j=nuds++;
				while(j>0)
					{ uds[j]=uds[j-1]; j--; }
				uds[0]=bgbcc_strdup(tb);

				continue;
			}

			if(!strcmp(argv[i]+1, "add"))
			{
				for(j=0; j<(argc-i-1); j++)
				{
					if(argv[i+j+1][0]=='/')break;
					if(argv[i+j+1][0]=='-')break;
					adds[nadds++]=argv[i+j+1];
				}
				i+=j;
				continue;
			}

			if(!strncmp(argv[i]+1, "FZx", 3))
			{
				m|=16;
				continue;
			}

			if(!strcmp(argv[i], "/?") || !strcmp(argv[i], "/help") ||
				!strcmp(argv[i], "-help") || !strcmp(argv[i], "--help"))
			{
				m|=32;
				continue;
			}

			continue;
		}
#endif

#if 0
		if(argv[i][0]=='/')
		{
			if(!strncmp(argv[i], "/I", 2))
			{
				ccAddInclude(argv[i]+2);
				continue;
			}
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

			if(!strncmp(argv[i], "/Fw", 3))
			{
				wadfn=argv[i]+3;
				continue;
			}

			if(!strncmp(argv[i], "/Fi", 3) ||
				!strncmp(argv[i], "/Fe", 3))
			{
				frbcfn=argv[i]+3;
				continue;
			}

			if(!strcmp(argv[i], "/add"))
			{
				for(j=0; j<(argc-i-1); j++)
				{
					if(argv[i+j+1][0]=='/')break;
					if(argv[i+j+1][0]=='-')break;
					adds[nadds++]=argv[i+j+1];
				}
				i+=j;
				continue;
			}

			if(!strncmp(argv[i], "/FZx", 4))
			{
				m|=16;
				continue;
			}

			if(!strcmp(argv[i], "/?") || !strcmp(argv[i], "/help"))
			{
				m|=32;
				continue;
			}

			continue;
		}

		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "--"))
				{ i++; break; }
			if(!strcmp(argv[i], "--pc"))
				m|=1;

			if(!strncmp(argv[i], "-I", 2))
			{
				ccAddInclude(argv[i]+2);
				continue;
			}
			if(!strncmp(argv[i], "-L", 2))
			{
				ccAddLibrary(argv[i]+2);
				continue;
			}
			if(!strncmp(argv[i], "-S", 2))
			{
				ccAddSource(argv[i]+2);
				continue;
			}

			if(!strncmp(argv[i], "-D", 2))
			{
				ccAddDefineString(argv[i]+2);
				continue;
			}

			if(!strncmp(argv[i], "-Fn", 3))
			{
				metafn=argv[i]+3;
				continue;
			}

			if(!strncmp(argv[i], "-Fw", 3))
			{
				wadfn=argv[i]+3;
				continue;
			}

			if(!strncmp(argv[i], "-Fi", 3))
			{
				frbcfn=argv[i]+3;
				continue;
			}

			if(!strcmp(argv[i], "-o"))
			{
				frbcfn=argv[i+1];
				i++;
				continue;
			}

			if(!strcmp(argv[i], "-help") || !strcmp(argv[i], "--help"))
			{
				m|=32;
				continue;
			}

			continue;
		}
#endif
		uds[nuds++]=argv[i];
	}

	nargs=0; args[nargs++]=argv[0];
	for(; i<argc; i++)args[nargs++]=argv[i];

	if(m&32)
	{
		help(argv[0]);
		return(0);
	}

//	if(!nuds)
	if(nuds<=minuds)
	{
		printf("%s: No input files\n", argv[0]);
//		help(argv[0]);
		return(0);
	}
	
	if(bgbcc_istool==1)
	{
		if(!metafn && !wadfn && !frbcfn)
//		if(!frbcfn)
		{
			s=BGBCP_BaseNameForName(uds[minuds]);
			sprintf(tb, "%s.exe", s);
			frbcfn=bgbcc_strdup(tb);

			if(bgbcc_verbose)
			{
				printf("%s: automatic output name = %s\n",
					argv[0], frbcfn);
			}
			
//			frbcfn="a.exe";
		}
	}
	

	if(!metafn && !wadfn && !frbcfn)
	{
		printf("%s: No output file\n", argv[0]);
//		help(argv[0]);
		return(0);
	}

	if(frbcfn)
	{
		bgbcc_imgname=frbcfn;
	
//		fmt=BGBCC_IMGFMT_OBJ;
		fmt=BGBCP_ImageFormatForName(frbcfn);

		if(!fmt && !is_compile_only)
		{
			fmt=BGBCC_IMGFMT_EXE;
		}

		if((fmt==BGBCC_IMGFMT_EXE) || (fmt==BGBCC_IMGFMT_DLL))
		{
			/* Try to infer if we need implicit libc */
			for(i=0; i<nuds; i++)
			{
				s0=uds[i];
				if((s0[0]!='-') || (s0[1]!='l'))
					continue;
				if(!strcmp(s0, "-lc"))
					break;
				if(!strcmp(s0, "-lcdll"))
					break;
				if(!strncmp(s0+2, "vxcore", 6))
					break;
			}
			
			if(!(i<nuds))
			{
				j=nuds++;
				while(j>0)
					{ uds[j]=uds[j-1]; j--; }
//				uds[0]=bgbcc_strdup("-lc");
				uds[0]=bgbcc_strdup("-lvxcore64");
			}
		}
		
		if(is_compile_only)
		{
//			fmt=BGBCC_IMGFMT_RIL3;
		}
		
		if(fmt==BGBCC_IMGFMT_OBJ)
		{
			fmt=BGBCC_IMGFMT_RIL3;
		}

		obuf=malloc(1<<24); sz=1<<24;
		i=BGBCC_LoadCSourcesCCXL(uds, nuds, obuf, &sz, fmt);
		if((i>=0) && (sz>0))
		{
			BGBCC_StoreFile(frbcfn, obuf, sz);
		}else
		{
			printf("Failed to produce output, status=%d\n", i);
			return(-1);
		}
	}else
	{
#if 0
		for(i=0; i<nuds; i++)
			BGBCC_LoadCMeta(uds[i]);

		if(m&16)
		{
			t0=clock();
			te=t0+30*CLOCKS_PER_SEC;
			while(t0<te)
			{
				for(i=0; i<nuds; i++)
					BGBCC_LoadCMeta(uds[i]);
				t0=clock();

				BGBCC_CleanupAll();
			}
		}
#endif
	}

	wad=NULL;
	if(wadfn)
	{
		wad=ExWAD_LoadImage2(wadfn);
	}

	if(wad)
	{
		for(i=0; i<nadds; i++)
			ExWAD_AddFile(wad, adds[i]);
			
		if(nuds)
		{
#if 1
			s0=malloc(1<<24);
			i=DYLL_MetaPath_SaveDBBuffer(s0, 1<<24);
			if(i>0)
			{
				ExWAD_StoreFile(wad, "meta.txt", (byte *)s0, i);
			}
			free(s0);
#endif
		}
		
		ExWAD_SaveImage(wad);
	}

	if(metafn)
		DYLL_MetaPath_SaveDB(metafn);
	return(0);
}

