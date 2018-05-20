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

int bgbcc_storefile(char *name, void *buf, int sz)
	{ return(BGBCC_StoreFile(name, buf, sz)); }

char *bgbcc_loadfile(char *name, int *rsz)
{
	FILE *fd;
	void *buf;
	int sz;

//	printf("bgbcc_loadfile: %s\n", name);

	fd=fopen(name, "rb");
	if(!fd)return(NULL);

	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);

//	buf=bgbcc_malloc(sz+16);
	buf=bgbcc_malloc2(sz+16);
	memset(buf, 0, sz+16);
	fread(buf, 1, sz, fd);

	fclose(fd);

	if(rsz)*rsz=sz;
	return(buf);
}

char *bgbcc_loadfile2(char *name, int *rsz)
{
	char tb[256];
	void *buf;
	int i;

	buf=bgbcc_loadfile(name, rsz);
	if(buf)return(buf);
	
	for(i=0; i<bgbcc_nsrc; i++)
	{
		sprintf(tb, "%s/%s", bgbcc_src[i], name);
		buf=bgbcc_loadfile(tb, rsz);
		if(buf)return(buf);
	}

	for(i=0; i<bgbcc_ninc; i++)
	{
		sprintf(tb, "%s/%s", bgbcc_inc[i], name);
		buf=bgbcc_loadfile(tb, rsz);
		if(buf)return(buf);
	}

	for(i=0; i<bgbcc_nlib; i++)
	{
		sprintf(tb, "%s/%s", bgbcc_lib[i], name);
		buf=bgbcc_loadfile(tb, rsz);
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

	printf("BGBCC_LoadCMeta: %s %d bytes\n", name, sz);

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
	if(mod)
	{
		sprintf(tb1, "dump/%s_ast.txt", mod);
		fd=fopen(tb1, "wt");
		if(fd)
		{
			BCCX_PrintFD(fd, t);
			fclose(fd);
		}

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

	if(!buf)
	{
		printf("BGBCC_LoadCSourceAST: fail load '%s'\n", name);
		return(NULL);
	}

	printf("BGBCC_LoadCSourceAST: %s %d bytes\n", name, sz);

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

#if 0
	if(mod)
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
	BCCX_Node *t, *c, *n;
	byte *buf;
	char *dllname;
	char *s0, *s1;
	fourcc lang;
	int i, sz, omsz;

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

	BGBCC_CCXL_SetupContextForArch(ctx);

//	if(1)
	if(imgfmt==BGBCC_IMGFMT_RIL3)
	{
		BGBCC_CCXLR3_BeginRecRIL(ctx);
	}

	for(i=0; i<nnames; i++)
	{
		printf("BGBCC_LoadCSourcesCCXL: %s\n", names[i]);

		if((names[i][0]=='-') && (names[i][1]=='l'))
		{
			sprintf(tb, "lib%s.ril", names[i]+2);
			buf=bgbcc_loadfile2(tb, &sz);
			if(buf)
				{ BGBCC_CCXLR3_LoadBufferRIL(ctx, buf, sz); continue; }

			sprintf(tb, "%s.dll", names[i]+2);
			buf=bgbcc_loadfile2(tb, &sz);
			if(buf)
				{ BGBCC_CCXL_LoadBufferDLL(ctx, buf, sz); continue; }

			printf("BGBCC_LoadCSourcesCCXL: Can't Find Library %s\n", names[i]);
			continue;
		}

#if 0
		lang=BGBCP_LangForName(names[i]);
		if(lang==BGBCC_IMGFMT_RIL3)
		{
			buf=bgbcc_loadfile2(names[i], &sz);
			if(buf)
				{ BGBCC_CCXLR3_LoadBufferRIL(ctx, buf, sz); }
			continue;
		}

		if(lang==BGBCC_IMGFMT_DLL)
		{
			buf=bgbcc_loadfile2(names[i], &sz);
			if(buf)
				{ BGBCC_CCXL_LoadBufferDLL(ctx, buf, sz); }
			continue;
		}
#endif
		
		t=BGBCC_LoadCSourceAST(names[i]);
		if(!t)
			break;

		t0=clock();
		BGBCC_CCXL_CompileModuleCTX(ctx, names[i], t);
		BCCX_DeleteTree(t);
		t1=clock();
		t2=t1-t0;
		printf("Compile Module %dms\n", t2);
		
		c=ctx->reduce_tmp;
		ctx->reduce_tmp=NULL;
		while(c)
		{
			n=c->hnext;
			BCCX_DeleteTree(c);
			c=n;
		}
	}
	
	if(i<nnames)
	{
		return(-1);
	}

//	if(ctx->ril_ip)
//	{
//		sprintf(tb, "dump/%s_ril3.dat", "tst");
//		BGBCC_StoreFile(tb, ctx->ril_ips, ctx->ril_ip-ctx->ril_ips);
//	}

	if(imgfmt==BGBCC_IMGFMT_RIL3)
	{
		sz=ctx->ril_ip-ctx->ril_ips;
		memcpy(obuf, ctx->ril_ips, sz);
		if(*rsz)*rsz=sz;
		return(0);
	}

//	sz=BGBCC_FrBC_FlattenImage(ctx, obuf, omsz);

	sz=omsz;

	t0=clock();
	i=BGBCC_CCXL_FlattenImage(ctx, obuf, &sz, imgfmt);
	t1=clock();
	t2=t1-t0;
	printf("Flatten Image %dms\n", t2);

//	BIPRO_ProfilerSetActive(0);
//	BIPRO_ProfileDumpStats();

	if(i<0)return(i);
	if(*rsz)*rsz=sz;
	return(0);
}
#endif


u32 BGBCC_GetArch()
	{ return(bgbcc_arch); }

u32 BGBCC_GetSubArch()
	{ return(bgbcc_subarch); }

int BGBCC_LoadConfig(char *name)
{
	char buf[256];
	char **a;
	FILE *fd;
	int i;
	
	fd=fopen(name, "rt");
	if(!fd)return(-1);
	
	while(!feof(fd))
	{
		fgets(buf, 256, fd);
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
	}
	
	return(0);
}

int BGBCC_InitEnv(int argc, char **argv, char **env)
{
	char *ta[16];
	char buf[256];
	static int init=0;
	int inc_ok;
	int i, j, k, m, endian;
	char *s, *t;
	char *mach_name, *gcc_ver, *home, *base, *cfg;
//#ifdef linux
//	struct utsname utsbuf;
//#endif

	if(init)return(-1);
	init=1;

	mach_name=NULL; gcc_ver=NULL;
	cfg=NULL;

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
	bgbcc_gshash=0;

	m=0;
	for(i=1; i<argc; i++)
	{
		for(j=0; argv[i][j]; j++)
			bgbcc_gshash=(bgbcc_gshash*65521+1)+argv[i][j];

#if 1
		if((argv[i][0]=='/') || (argv[i][0]=='-'))
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

			if(!strncmp(argv[i]+1, "Fi", 2))
			{
				m|=64;
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
				cfg=argv[i]+5;
				continue;
			}

			if(!strncmp(argv[i]+1, "home=", 5))
			{
				home=argv[i]+6;
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
	}

	strcpy(buf, argv[0]);
	s=buf+strlen(buf);
	while((s>buf) && (*s!='/') && (*s!='\\'))
		s--;
	if(s>buf)
	{
		*s=0;
		base=bgbcc_strdup(buf);
	}

#ifdef linux
	if(!home)home="~";
#endif

	if(m&64)
	{
		if(home)
		{
			sprintf(buf, "%s/frvmcfg.txt", home);
			BGBCC_LoadConfig(buf);
		}

		if(base)
		{
			sprintf(buf, "%s/frvmcfg.txt", base);
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
		mach_name="SH4";
//		mach_name="BJX1L";
	}

	if(mach_name)
	{
		bgbcc_arch=BGBCP_ArchForName(mach_name);
		bgbcc_subarch=BGBCP_SubArchForName(bgbcc_arch, mach_name);
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

	//FIXME: do this properly
	BGBPP_AddStaticDefine(NULL, "__DATE__", "\"Apr 01 2000\"");
	BGBPP_AddStaticDefine(NULL, "__TIME__", "\"23:59:59\"");

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
		BGBPP_AddStaticDefine(NULL, "__GNUC__", "1");
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

	printf("Add include '%s'\n", path);

	BGBCC_Init();
	i=bgbcc_ninc++;
	bgbcc_inc[i]=bgbcc_strdup(path);
	BGBPP_AddIncludePathFront(path);
}

void ccAddLibrary(char *path)
{
	int i;

	BGBCC_Init();
	i=bgbcc_nlib++;
	bgbcc_lib[i]=bgbcc_strdup(path);
}

void ccAddSource(char *path)
{
	int i;

	BGBCC_Init();
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
	int n, m, nuds, nargs, nadds;
	int t0, dt, te, sz;
	int i, j;

//	dy_init(&argc);

	BGBCC_InitEnv(argc, argv, env);

	BGBCC_SHXC_InitIface();
	BGBCC_BSRC_InitIface();

	metafn=NULL;
	wadfn=NULL;
	frbcfn=NULL;

	nargs=0;
	args[nargs++]=argv[0];

	nadds=0;
	nuds=0;

	n=0; m=0; nadds=0;
	for(i=1; i<argc; i++)
	{
#if 1
		if((argv[i][0]=='/') || (argv[i][0]=='-'))
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
				!strncmp(argv[i]+1, "Fe", 2))
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

			if(!strncmp(argv[i]+1, "l", 1))
			{
				sprintf(tb, "-l%s", argv[i]+2);
//				sprintf(tb, "lib%s.ril", argv[i]+2);
//				uds[nuds++]=bgbcc_strdup(tb);
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

	if(!nuds)
	{
		printf("No input files\n");
//		help(argv[0]);
		return(0);
	}

	if(!metafn && !wadfn && !frbcfn)
	{
		printf("No output file\n");
//		help(argv[0]);
		return(0);
	}

	if(frbcfn)
	{
		bgbcc_imgname=frbcfn;
	
//		fmt=BGBCC_IMGFMT_OBJ;
		fmt=BGBCP_ImageFormatForName(frbcfn);
	
		obuf=malloc(1<<24); sz=1<<24;
		i=BGBCC_LoadCSourcesCCXL(uds, nuds, obuf, &sz, fmt);
		if((i>=0) && (sz>0))
		{
			BGBCC_StoreFile(frbcfn, obuf, sz);
		}else
		{
			printf("Failed to produce output, status=%d\n", i);
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
}

