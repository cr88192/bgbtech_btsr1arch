/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

int BGBCC_JX2C_CoffLoadBufferDLL(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	byte *buf, int sz)
{
	BGBCC_CCXL_LiteralInfo *litobj;
	byte *cs, *csend, *cspe, *csexp;
	byte *csexpaddrtab;
	byte *csexpnametab;
	char **expnames;
	char *dllname;
	char *s0, *s1;
	byte is64;
	int peofs, expofs, expsz;
	int nexps, rvaexpaddr;
	int i, j, k;

	csend=buf+sz;

	if((buf[0]!='P') || (buf[1]!='E'))
	{
		if((buf[0]!='M') || (buf[1]!='Z'))
		{
			printf("BGBCC_JX2C_CoffLoadBufferDLL: MZ Sig Fail\n");
			return(-1);
		}
		
		peofs=bgbcc_getu32en(buf+0x3C, 0);
		if((peofs<=0) || ((peofs+4)>=sz))
		{
			printf("BGBCC_JX2C_CoffLoadBufferDLL: PE Header Offset Fail\n");
			return(-1);
		}
	}else
	{
		peofs=0;
	}

	cspe=buf+peofs;

	if((cspe[0x00]!='P') || (cspe[0x01]!='E'))
	{
		printf("BGBCC_JX2C_CoffLoadBufferDLL: PE Sig Fail\n");
		return(-1);
	}
	i=bgbcc_getu16en(cspe+0x18, 0);
	if(i==0x020B)
	{
		is64=1;
	}else if(i==0x010B)
	{
		is64=0;
	}else
	{
		printf("BGBCC_JX2C_CoffLoadBufferDLL: Opt Header Magic Fail\n");
		return(-1);
	}

//	if(i!=0x010B)

	if(is64)
	{
		expofs=bgbcc_getu32en(cspe+0x88, 0);
		expsz=bgbcc_getu32en(cspe+0x8C, 0);
	}else
	{
		expofs=bgbcc_getu32en(cspe+0x78, 0);
		expsz=bgbcc_getu32en(cspe+0x7C, 0);
	}

	if((expofs==0) && (expsz==0))
	{
		printf("BGBCC_JX2C_CoffLoadBufferDLL: No Exports\n");
		return(0);
	}

	if((expofs<=0) || (expofs>=sz) ||
		(expsz<0) || ((expofs+expsz)>sz))
	{
		printf("BGBCC_JX2C_CoffLoadBufferDLL: Exports Offsets Fail\n");
		return(-1);
	}
	
	csexp=buf+expofs;

	i=bgbcc_getu32en(csexp+0x0C, 0);
	if((i<=0) || (i>=sz))
	{
		printf("BGBCC_JX2C_CoffLoadBufferDLL: DllName RVA Fail\n");
		return(-1);
	}

	s0=(char *)(buf+i);
	dllname=bgbcc_strdup(s0);

	nexps=bgbcc_getu32en(csexp+0x18, 0);
	if((nexps<0) || (nexps>65535))
	{
		printf("BGBCC_JX2C_CoffLoadBufferDLL: Non Sane Exports Count\n");
		return(-1);
	}

	rvaexpaddr=bgbcc_getu32en(csexp+0x20, 0);
	csexpnametab=buf+rvaexpaddr;
	
	expnames=bgbcc_malloc(nexps*sizeof(char *));
	
	for(i=0; i<nexps; i++)
	{
		j=bgbcc_getu32en(csexpnametab+(i*4), 0);
		if((j<=0) || (j>=sz))
			continue;

		s0=(char *)(buf+j);
		expnames[i]=bgbcc_strdup(s0);
	}
	
	litobj=BGBCC_CCXL_GetExportList(ctx, dllname);
	if(!litobj->decl)
	{
		litobj->decl=bgbcc_malloc(sizeof(BGBCC_CCXL_RegisterInfo));
		litobj->decl->regtype=CCXL_LITID_EXPLIST;
	}
	
	litobj->decl->goto_name=expnames;
	litobj->decl->n_goto=nexps;
	litobj->decl->m_goto=nexps;
	
	return(1);
}

int BGBCC_JX2C_CoffBuildExports(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	BGBCC_CCXL_RegisterInfo **exptab;
	BGBCC_CCXL_RegisterInfo *obj;
	char *dllname;
	int lbl_dllname;
	int lbl_expaddrtab;
	int lbl_nametab;
	int lbl_ordtab;
	int nexps, mexps;
	
	int i, j, k;

	exptab=NULL; nexps=0; mexps=0;
	for(i=0; i<ctx->n_reg_globals; i++)
	{
		obj=ctx->reg_globals[i];
		if(!obj)
			continue;
		if(!(obj->flagsint&BGBCC_TYFL_DLLEXPORT))
			continue;
		if(!obj->name)
			continue;
			
		if(!exptab)
		{
			mexps=4096;
			exptab=bgbcc_malloc(mexps*sizeof(BGBCC_CCXL_RegisterInfo *));
		}
		
		if((nexps+1)>=mexps)
		{
			mexps+=mexps>>1;
			exptab=bgbcc_realloc(exptab,
				mexps*sizeof(BGBCC_CCXL_RegisterInfo *));
		}
		exptab[nexps++]=obj;
	}

	if(nexps<=0)
		return(0);

	for(i=0; i<nexps; i++)
		for(j=i+1; j<nexps; j++)
	{
		if(strcmp(exptab[j]->name, exptab[i]->name)<0)
			{ obj=exptab[j]; exptab[j]=exptab[i]; exptab[i]=obj; }
	}

	lbl_dllname=BGBCC_JX2_GenLabel(sctx);
	lbl_expaddrtab=BGBCC_JX2_GenLabel(sctx);
	lbl_nametab=BGBCC_JX2_GenLabel(sctx);
	lbl_ordtab=BGBCC_JX2_GenLabel(sctx);

	dllname=ctx->imgbasename;
	if(!dllname)
		dllname="default";

	BGBCC_JX2_SetSectionName(sctx, ".edata");
	BGBCC_JX2_EmitDWord(sctx, 0);		//0x00, export flags
	BGBCC_JX2_EmitDWord(sctx, 0);		//0x04, time/date
	BGBCC_JX2_EmitWord(sctx, 0);		//0x08, major version
	BGBCC_JX2_EmitWord(sctx, 0);		//0x0A, minor version

	BGBCC_JX2_EmitRelocTy(sctx, lbl_dllname, BGBCC_SH_RLC_RVA32);
	BGBCC_JX2_EmitDWord(sctx, 0);		//0x0C, name RVA

	BGBCC_JX2_EmitDWord(sctx, 1);		//0x10, ordinal base
	BGBCC_JX2_EmitDWord(sctx, nexps);	//0x14, address table entries
	BGBCC_JX2_EmitDWord(sctx, nexps);	//0x18, number of name pointers

	BGBCC_JX2_EmitRelocTy(sctx, lbl_expaddrtab, BGBCC_SH_RLC_RVA32);
	BGBCC_JX2_EmitDWord(sctx, 0);		//0x1C, export address table RVA
	BGBCC_JX2_EmitRelocTy(sctx, lbl_nametab, BGBCC_SH_RLC_RVA32);
	BGBCC_JX2_EmitDWord(sctx, 0);		//0x20, name pointer table RVA
	BGBCC_JX2_EmitRelocTy(sctx, lbl_ordtab, BGBCC_SH_RLC_RVA32);
	BGBCC_JX2_EmitDWord(sctx, 0);		//0x24, ordinal table RVA

	BGBCC_JX2_EmitLabel(sctx, lbl_dllname);
	BGBCC_JX2_EmitString(sctx, dllname);

	for(i=0; i<nexps; i++)
	{
		obj=exptab[i];

		k=BGBCC_JX2_GenLabel(sctx);
		obj->fxnalgn=k;

		BGBCC_JX2_EmitLabel(sctx, k);
		BGBCC_JX2_EmitString(sctx, obj->name);
	}

	BGBCC_JX2_EmitBAlign(sctx, 4);

	BGBCC_JX2_EmitLabel(sctx, lbl_expaddrtab);
	for(i=0; i<nexps; i++)
	{
		obj=exptab[i];
		BGBCC_JX2_EmitRelocTy(sctx,
			obj->fxoffs, BGBCC_SH_RLC_RVA32);
		BGBCC_JX2_EmitDWord(sctx, 0);	//export RVA
	}

	BGBCC_JX2_EmitLabel(sctx, lbl_nametab);
	for(i=0; i<nexps; i++)
	{
		obj=exptab[i];
		BGBCC_JX2_EmitRelocTy(sctx,
			obj->fxnalgn, BGBCC_SH_RLC_RVA32);
		BGBCC_JX2_EmitDWord(sctx, 0);	//name RVA
	}

	BGBCC_JX2_EmitLabel(sctx, lbl_ordtab);
	for(i=0; i<nexps; i++)
	{
		BGBCC_JX2_EmitDWord(sctx, i+1);	//ordinal
	}
	
	return(0);
}

int BGBCC_JX2C_CoffBuildImports(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	BGBCC_CCXL_LiteralInfo *lodlls[256];
	char *dllntab[256];
	int dllsytab[256];
	int dlliltab[256];
	int dlliatab[256];
	int dllexpi[256];

	BGBCC_CCXL_LiteralInfo *lobj;
	BGBCC_CCXL_RegisterInfo **exptab;
	BGBCC_CCXL_RegisterInfo *obj;
//	char *dllname;
//	int lbl_dllname;
//	int lbl_expaddrtab;
//	int lbl_nametab;
//	int lbl_ordtab;
	int nexps, mexps, ndlls;
	
	int i, j, k;

	exptab=NULL; nexps=0; mexps=0;
	for(i=0; i<ctx->n_reg_globals; i++)
	{
		obj=ctx->reg_globals[i];
		if(!obj)
			continue;
		if(!(obj->flagsint&BGBCC_TYFL_DLLIMPORT))
			continue;
		if(!obj->name)
			continue;
			
		if(!exptab)
		{
			mexps=4096;
			exptab=bgbcc_malloc(mexps*sizeof(BGBCC_CCXL_RegisterInfo *));
		}
		
		if((nexps+1)>=mexps)
		{
			mexps+=mexps>>1;
			exptab=bgbcc_realloc(exptab,
				mexps*sizeof(BGBCC_CCXL_RegisterInfo *));
		}
		exptab[nexps++]=obj;
	}

	if(nexps<=0)
		return(0);

//	for(i=0; i<nexps; i++)
//		for(j=i+1; j<nexps; j++)
//	{
//		if(strcmp(exptab[j]->name, exptab[i]->name)<0)
//			{ obj=exptab[j]; exptab[j]=exptab[i]; exptab[i]=obj; }
//	}

	ndlls=0;
	for(i=0; i<nexps; i++)
	{
		lobj=BGBCC_CCXL_LookupExportListForName(ctx, exptab[i]->name);
		for(j=0; j<ndlls; j++)
		{
			if(lodlls[j]==lobj)
				break;
		}
		if(j>=ndlls)
		{
			j=ndlls++;
			lodlls[j]=lobj;
			dllntab[j]=lobj->name;
			dllsytab[j]=BGBCC_JX2_GenLabel(sctx);

			dlliltab[j]=BGBCC_JX2_GenLabel(sctx);
			dlliatab[j]=BGBCC_JX2_GenLabel(sctx);

			dllexpi[j]=-1;
		}
		
		exptab[i]->fxmalgn=dllexpi[j];
		exptab[i]->fxmoffs=lobj->decl->fxmoffs;
		dllexpi[j]=i;
	}


	BGBCC_JX2_SetSectionName(sctx, ".idata");

	for(i=0; i<ndlls; i++)
	{
		BGBCC_JX2_EmitRelocTy(sctx, dlliltab[i], BGBCC_SH_RLC_RVA32);
		BGBCC_JX2_EmitDWord(sctx, 0);	//import lookup table RVA
		BGBCC_JX2_EmitDWord(sctx, 0);	//time/date

		BGBCC_JX2_EmitDWord(sctx, 0);	//forwarder chain

		BGBCC_JX2_EmitRelocTy(sctx, dllsytab[i], BGBCC_SH_RLC_RVA32);
		BGBCC_JX2_EmitDWord(sctx, 0);	//name RVA

		BGBCC_JX2_EmitRelocTy(sctx, dlliatab[i], BGBCC_SH_RLC_RVA32);
		BGBCC_JX2_EmitDWord(sctx, 0);	//import address table RVA
	}
	
	//null entry
	BGBCC_JX2_EmitDWord(sctx, 0);	//import lookup table RVA
	BGBCC_JX2_EmitDWord(sctx, 0);	//time/date
	BGBCC_JX2_EmitDWord(sctx, 0);	//forwarder chain
	BGBCC_JX2_EmitDWord(sctx, 0);	//name RVA
	BGBCC_JX2_EmitDWord(sctx, 0);	//import address table RVA

	for(i=0; i<ndlls; i++)
	{
		k=dllsytab[i];
		BGBCC_JX2_EmitLabel(sctx, k);
		BGBCC_JX2_EmitString(sctx, dllntab[i]);
	}

	for(i=0; i<nexps; i++)
	{
		obj=exptab[i];

		k=BGBCC_JX2_GenLabel(sctx);
		obj->fxnalgn=k;

		BGBCC_JX2_EmitBAlign(sctx, 2);
		BGBCC_JX2_EmitLabel(sctx, k);
//		BGBCC_JX2_EmitDWord(sctx, obj->fxmoffs);	//hint
		BGBCC_JX2_EmitWord(sctx, obj->fxmoffs);		//hint
		BGBCC_JX2_EmitString(sctx, obj->name);		//name
	}

	BGBCC_JX2_EmitBAlign(sctx, 4);
	for(i=0; i<ndlls; i++)
	{
		k=dlliltab[i];
		BGBCC_JX2_EmitLabel(sctx, k);
//		BGBCC_JX2_EmitString(sctx, dllntab[i]);
		j=dllexpi[i];
		while(j>=0)
		{
			obj=exptab[j];
			BGBCC_JX2_EmitRelocTy(sctx, obj->fxnalgn, BGBCC_SH_RLC_RVA32);
			BGBCC_JX2_EmitDWord(sctx, 0);	//name
			if(sctx->is_addr64 && !sctx->is_addr_x32)
				BGBCC_JX2_EmitDWord(sctx, 0);
			j=obj->fxmalgn;
		}
		BGBCC_JX2_EmitDWord(sctx, 0);
		if(sctx->is_addr64 && !sctx->is_addr_x32)
			BGBCC_JX2_EmitDWord(sctx, 0);
	}

	for(i=0; i<ndlls; i++)
	{
		k=dlliatab[i];
		BGBCC_JX2_EmitLabel(sctx, k);
//		BGBCC_JX2_EmitString(sctx, dllntab[i]);
		j=dllexpi[i];
		while(j>=0)
		{
			obj=exptab[j];
			BGBCC_JX2_EmitLabel(sctx, obj->fxnoffs);
			BGBCC_JX2_EmitRelocTy(sctx, obj->fxnalgn, BGBCC_SH_RLC_RVA32);
			BGBCC_JX2_EmitDWord(sctx, 0);	//name
			if(sctx->is_addr64 && !sctx->is_addr_x32)
				BGBCC_JX2_EmitDWord(sctx, 0);
			j=obj->fxmalgn;
		}
		BGBCC_JX2_EmitDWord(sctx, 0);
		if(sctx->is_addr64 && !sctx->is_addr_x32)
			BGBCC_JX2_EmitDWord(sctx, 0);
	}
	
	return(0);
}

int BGBCC_JX2C_CoffSectionFlags(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	char *name)
{
	if(!strcmp(name, ".text") ||
		!strcmp(name, ".utext"))
	{
		return(BGBCC_COFF_SCNT_CODE|
			BGBCC_COFF_SCNT_EXECUTE|
			BGBCC_COFF_SCNT_READ);
	}
	if(!strcmp(name, ".udata"))
	{
		return(BGBCC_COFF_SCNT_IDATA|
			BGBCC_COFF_SCNT_READ|
			BGBCC_COFF_SCNT_WRITE);
	}

	if(!strcmp(name, ".data") ||
		!strcmp(name, ".idata") ||
		!strcmp(name, ".vsdata") ||
		!strcmp(name, ".tls") ||
		!strcmp(name, ".sdata"))
	{
		if(sctx->is_pbo)
		{
			return(BGBCC_COFF_SCNT_IDATA|
				BGBCC_COFF_SCNT_READ|
				BGBCC_COFF_SCNT_WRITE|
				BGBCC_COFF_SCNT_GPREL);
		}
	
		return(BGBCC_COFF_SCNT_IDATA|
			BGBCC_COFF_SCNT_READ|
			BGBCC_COFF_SCNT_WRITE);
	}
	if(!strcmp(name, ".bss") ||
		!strcmp(name, ".sbss"))
	{
		if(sctx->is_pbo)
		{
			return(BGBCC_COFF_SCNT_UDATA|
				BGBCC_COFF_SCNT_READ|
				BGBCC_COFF_SCNT_WRITE|
				BGBCC_COFF_SCNT_GPREL);
		}
	
		return(BGBCC_COFF_SCNT_UDATA|
			BGBCC_COFF_SCNT_READ|
			BGBCC_COFF_SCNT_WRITE);
	}
	if(!strcmp(name, ".rdata") ||
		!strcmp(name, ".edata") ||
		!strcmp(name, ".xdata") ||
		!strcmp(name, ".srdata") ||
		!strcmp(name, ".rsrc") ||
		!strcmp(name, ".strtab"))
	{
		return(BGBCC_COFF_SCNT_IDATA|
			BGBCC_COFF_SCNT_READ);
	}
	if(!strcmp(name, ".reloc"))
	{
		return(BGBCC_COFF_SCNT_IDATA|
			BGBCC_COFF_SCNT_READ|
			BGBCC_COFF_SCNT_DISCARD);
	}

	return(BGBCC_COFF_SCNT_IDATA|
		BGBCC_COFF_SCNT_READ|
		BGBCC_COFF_SCNT_WRITE);
//	return(0);
}

int bgbcc_jx2c_qrsort(u32 *arr, int cnt, int rd)
{
	int lo, hi, cn, pvt;
	u32 v, v0, v1, v2;
	int i, j, k;
	
	if((cnt<8) || (rd>=20))
	{
		for(i=0; i<cnt; i++)
			for(j=i+1; j<cnt; j++)
		{
			if(arr[j]<arr[i])
				{ v=arr[i]; arr[i]=arr[j]; arr[j]=v; }
		}
		return(0);
	}
	
	cn=cnt>>1;
	v0=arr[cn>>1];
	v1=arr[cn];
	v2=arr[cn+(cn>>1)];
	if(v0>v1) { v=v0; v0=v1; v1=v; }
	if(v0>v2) { v=v0; v0=v2; v2=v; }
	if(v1>v2) { v=v1; v1=v2; v2=v; }
	pvt=v1;
	
	lo=0; hi=cnt;
	
	while(lo<hi)
	{
		v=arr[lo];
		if(v<=pvt)
			{ lo++; continue; }
		hi--;		v1=arr[hi];
		arr[hi]=v;	arr[lo]=v1;
	}
	
	bgbcc_jx2c_qrsort(arr, lo, rd+1);
	bgbcc_jx2c_qrsort(arr+lo, cnt-lo, rd+1);
	return(0);
}


// #define BGBCC_PEL_HASHSZ		256
// #define BGBCC_PEL_HASHSZ		4096
#define BGBCC_PEL_HASHSZ		16384
#define BGBCC_PEL_HASHMASK		(BGBCC_PEL_HASHSZ-1)

//#define BGBCC_PEL_HASHDSZ		16
// #define BGBCC_PEL_HASHDSZ		32
#define BGBCC_PEL_HASHDSZ		64
#define BGBCC_PEL_HASHDMA		(BGBCC_PEL_HASHDSZ-1)

#define BGBCC_PEL_HASH_PR		65521
// #define BGBCC_PEL_HASH_SHR		16
#define BGBCC_PEL_HASH_SHR		18

// byte *bgbcc_packlz_hash[256*16];
// byte bgbcc_packlz_hrov[256];

// byte *bgbcc_packlz_hash[BGBCC_PEL_HASHSZ*BGBCC_PEL_HASHDSZ];
// byte bgbcc_packlz_hrov[BGBCC_PEL_HASHSZ];
byte **bgbcc_packlz_hash;
byte *bgbcc_packlz_hrov;
int bgbcc_packlz_cmp;

int BGBCC_JX2C_PackBlockLZ_Reset(BGBCC_TransState *ctx)
{
	int i, j, k;
	
	if(!bgbcc_packlz_hash)
	{
		bgbcc_packlz_hash=malloc(
			BGBCC_PEL_HASHSZ*BGBCC_PEL_HASHDSZ*sizeof(byte *));
		bgbcc_packlz_hrov=malloc(BGBCC_PEL_HASHSZ);
	}
	
	for(i=0; i<BGBCC_PEL_HASHSZ; i++)
	{
		bgbcc_packlz_hrov[i]=0;
		for(j=0; j<BGBCC_PEL_HASHDSZ; j++)
		{
			bgbcc_packlz_hash[i*BGBCC_PEL_HASHDSZ+j]=NULL;
		}
	}
	bgbcc_packlz_cmp=4;
	return(0);
}

int BGBCC_JX2C_PackBlockLZ_LookupMatch(
	BGBCC_TransState *ctx,
	byte *cs, byte *cse, int *rbl, int *rbd)
{
	byte *ct, *ct1, *ct1e, *cs1, *cs1e;
	u32 h;
	int hr, bd, bl, md, ml;
	int i, j, k, l, d;
	
	h=*(u32 *)cs;
//	h=(h*65521)+1;
//	h=(h*251)+1;
//	h=(h>>16)&BGBCC_PEL_HASHMASK;
	h=((h*BGBCC_PEL_HASH_PR)>>BGBCC_PEL_HASH_SHR)&BGBCC_PEL_HASHMASK;
	
	hr=bgbcc_packlz_hrov[h];

	md=65535;
	ml=16383;

	if(bgbcc_packlz_cmp==3)
	{
//		md=131071;
		md=(1<<21)-1;
	}

	if(bgbcc_packlz_cmp==6)
		{ ml=273; }

//	return(0);

//	cs1e=cs+268;
//	cs1e=cs+266;
//	cs1e=cs+4096;
//	cs1e=cs+16384;
//	cs1e=cs+16383;
	cs1e=cs+ml;
	if(cse<cs1e)
		cs1e=cse;
	
	bd=0; bl=0;
	ct1e=cs;
	for(i=0; i<BGBCC_PEL_HASHDSZ; i++)
	{
		ct=bgbcc_packlz_hash[
			(h*BGBCC_PEL_HASHDSZ)+
			((hr+i)&BGBCC_PEL_HASHDMA)];
		if(!ct)
			continue;

		d=cs-ct;
//		if(d>=65536)
		if(d>md)
			continue;
		if(d<=0)
			continue;
			
		cs1=cs; ct1=ct;
		
#ifdef BGBCC_GET_BITS
		if(d>=8)
		{
			while((cs1+8)<cs1e)
			{
				if(BGBCC_GET_U64LE(cs1)!=BGBCC_GET_U64LE(ct1))
					break;
				cs1+=8;	ct1+=8;
			}
		}
#endif
		
		while(cs1<cs1e)
		{
			if((*cs1)!=(*ct1))
				break;
			cs1++;	ct1++;
		}
		
		l=cs1-cs;
		
		if(l>bl)
			{ bl=l; bd=d; }
	}
	
	*rbl=bl;
	*rbd=bd;

	if(bd<=0)
		return(0);
	
	if(bgbcc_packlz_cmp==3)
	{
		if((bl>=3) && (bd<512))
			return(1);
	}

	return(bl>=4);
}

int BGBCC_JX2C_PackBlockLZ_HashByte(
	BGBCC_TransState *ctx, byte *cs)
{
	u32 h;
	int hr, bd, bl;
	int i, j, k, l, d;
	
	h=*(u32 *)cs;
//	h=(h*65521)+1;
//	h=(h*251)+1;
//	h=(h>>16)&BGBCC_PEL_HASHMASK;
	h=((h*BGBCC_PEL_HASH_PR)>>BGBCC_PEL_HASH_SHR)&BGBCC_PEL_HASHMASK;
	
	hr=bgbcc_packlz_hrov[h];
	hr=(hr-1)&BGBCC_PEL_HASHDMA;
	bgbcc_packlz_hrov[h]=hr;
	bgbcc_packlz_hash[(h*BGBCC_PEL_HASHDSZ)+hr]=cs;
	return(0);
}

int BGBCC_JX2C_PackBlockLZ_HashString(
	BGBCC_TransState *ctx, byte *cs, int sz)
{
	int i;
	
	for(i=0; i<sz; i++)
		BGBCC_JX2C_PackBlockLZ_HashByte(ctx, cs+i);
	return(0);
}

int BGBCC_JX2C_PackBlockLZ_EstRawCost(BGBCC_TransState *ctx, int rl)
{
	int i, j, c;

	if((bgbcc_packlz_cmp==4) || (bgbcc_packlz_cmp==6))
	{
		c=rl+1;
		if(rl>=15)
		{
			i=rl-15;
			while(i>=255)
				{ c++; i-=255; }
			c++;
		}
		c+=3;
		return(c);
	}

	c=rl;
	i=rl;
	while(i>=4096)
		{ c+=2; i-=4096; }
	if(i>(128+7))
		{ c+=2; i=i&7; }
	if(i>=8)
		{ c+=1; i=i&7; }

//	c=(rl>>3)+rl;
//	c=(rl>>7)+rl+(rl>=8);
	return(c);
}

int BGBCC_JX2C_PackBlockLZ_EstMatchCost(BGBCC_TransState *ctx,
	int rl, int bl, int bd)
{
	int c, i;

	if((bgbcc_packlz_cmp==4) || (bgbcc_packlz_cmp==6))
	{
		c=rl+1;
		if(rl>=15)
		{
			i=rl-15;
			while(i>=255)
				{ c++; i-=255; }
			c++;
		}
		if(bl>=19)
		{
			i=bl-19;
			while(i>=255)
				{ c++; i-=255; }
			c++;
		}
		
		return(c);
	}

//	c=(rl>>3)+rl;
//	c=(rl>>7)+rl+(rl>=8);
	c=BGBCC_JX2C_PackBlockLZ_EstRawCost(ctx, rl);
	if((bl<=10) && (bd<512))
		{ c+=2; }
	else if((bl<=67) && (bd<8192))
		{ c+=3; }
	else if((bl<=515) && (bd<131072))
		{ c+=4; }
	else if((bl<=16383) && (bd<(1<<22)))
	{
		c+=1+((bl<128)?1:2)+((bd<32768)?2:3);
	}
	return(c);
}

int BGBCC_JX2C_PackBlockLZ_LookupMatchB(BGBCC_TransState *ctx,
	byte *cs, byte *cse, byte *lcs,
	int *rl, int *rd)
{
	int l, d, l1, d1;
	int rl0, rl1, rl2;
	int mc0, mc1;
	int i, j, k;

	i=BGBCC_JX2C_PackBlockLZ_LookupMatch(ctx, cs, cse, &l, &d);
	rl0=cs-lcs;
	rl1=rl0+1;	rl2=rl0+2;
	
	mc0=BGBCC_JX2C_PackBlockLZ_EstMatchCost(ctx, rl0, l, d);
	mc1=BGBCC_JX2C_PackBlockLZ_EstRawCost(ctx, rl0+l);
	
	if(mc0>=mc1)
	{
		/* Skip match if it costs more than it saves. */
		i=0;
	}
	
	if(i)
	{
		/* Check if a better match is just around the corner. */

		j=BGBCC_JX2C_PackBlockLZ_LookupMatch(ctx, cs+1, cse, &l1, &d1);
		if(j && (l1>(l+1)) && ((rl0>>3)==(rl1>>3)))
			i=0;

		mc1=BGBCC_JX2C_PackBlockLZ_EstMatchCost(ctx, rl1, l1, d1);
//		if(j && (l1>(l+1)) && (mc1<=mc0))
//		if(j && (l1>(l+0)) && (mc1<=mc0))
		if(j && ((mc1-l1)<=(mc0-l)))
			i=0;

//		if(j && (l1>(l+1)))
//		if(j && (l1>(l+2)))
//			i=0;

		if(i)
		{
			j=BGBCC_JX2C_PackBlockLZ_LookupMatch(ctx, cs+2, cse, &l1, &d1);

			mc1=BGBCC_JX2C_PackBlockLZ_EstMatchCost(ctx, rl2, l1, d1);
//			if(j && (l1>(l+0)) && (mc1<=mc0))
			if(j && ((mc1-l1)<(mc0-l)))
//			if(j && ((mc1-l1)<=(mc0-l)))
				i=0;
		}
	}
	
	*rl=l;
	*rd=d;
	return(i);
}



int BGBCC_JX2C_PackBlockLZ4(BGBCC_TransState *ctx,
	byte *obuf, byte *ibuf, int obsz, int ibsz)
{
	byte *ct, *cs, *cte, *cse;
	byte *cs0;
	int bl, bd, rl, tg, bl1, bd1;
	int i0, i1, lmc;
	int i;
	
	ct=obuf; cte=obuf+obsz;
	cs=ibuf; cse=ibuf+ibsz;
	cs0=cs;
	while((ct<cte) && (cs<cse))
	{
#if 0
		i0=BGBCC_JX2C_PackBlockLZ_LookupMatch(ctx, cs, cse, &bl, &bd);
		if(i0>0)
		{
			i1=BGBCC_JX2C_PackBlockLZ_LookupMatch(ctx, cs+1, cse, &bl1, &bd1);
//			if(bl1>(bl+1))
			if(i1 && (bl1>(bl+1)))
				i0=0;
		}
#endif

		i0=BGBCC_JX2C_PackBlockLZ_LookupMatchB(ctx, cs, cse, cs0, &bl, &bd);
		
//		if((ct+9)>=cte)
//			break;

//		rl=cs-cs0;
//		if((ct+rl+5)>=cte)
//			break;

		rl=cs-cs0;
//		if((ct+rl+9)>=cte)
		if((ct+rl+(rl>>8)+(bl>>8)+5)>=cte)
		{
			/* Near the end of a block, check if we need to stop. */
			if(i0>0)
			{
				/* Find cost of encoding match. */
				lmc=3;
				if(rl>=15)
					{ lmc+=((rl-15)/255)+1; }
				if(bl>=19)
					{ lmc+=((bl-19)/255)+1; }

				if((ct+rl+lmc)>=cte)
					break;
			}else
			{
				/* Find cost of a span of raw bytes. */
				lmc=1;
				if(rl>=15)
					{ lmc+=((rl-15)/255)+1; }
				if((ct+rl+lmc)>=cte)
					break;
			}
		}
		
		if(bgbcc_packlz_cmp==6)
		{
//			if(rl>=268)
			if(rl>=256)
			{
				tg=0xF1;
				*ct++=tg;
				*ct++=rl-15;

				while(cs0<cs)
					{ *ct++=*cs0++; }

				if(ct>cte)
					{ BGBCC_DBGBREAK }
				*(u16 *)ct=0;
				ct+=2;
				if(ct>cte)
					{ BGBCC_DBGBREAK }
				continue;
			}
		}

		if(i0>0)
		{
			rl=cs-cs0;
			if(rl>=15)
				tg=0xF0;
			else
				tg=(rl<<4);

			if(bl>=19)
				tg|=0x0F;
			else
				tg|=(bl-4);

			*ct++=tg;
//			if(rl>=15)
			if((tg&0xF0)==0xF0)
			{
				i=rl-15;
				
				if((bgbcc_packlz_cmp==6) && (i>=255))
					{ BGBCC_DBGBREAK }
				while(i>=255)
					{ *ct++=255; i-=255; }

				if(i<0)
					{ BGBCC_DBGBREAK }

				*ct++=i;
			}

			while(cs0<cs)
			{
//				*ct++=0x99; cs0++;
				*ct++=*cs0++;
			}

			if(ct>cte)
				{ BGBCC_DBGBREAK }

			*(u16 *)ct=bd;
			ct+=2;

			if(ct>=cte)
				{ BGBCC_DBGBREAK }

//			if(bl>=19)
			if((tg&0x0F)==0x0F)
			{
				i=bl-19;

				if(i<0)
					{ BGBCC_DBGBREAK }

				if((bgbcc_packlz_cmp==6) && (i>=255))
					{ BGBCC_DBGBREAK }
				while(i>=255)
//				if(i>=255)
					{ *ct++=0xFF; i-=255; }
				if(i>=255)
					{ BGBCC_DBGBREAK }
				*ct++=i;
			}

			if(ct>=cte)
				{ BGBCC_DBGBREAK }
			
			BGBCC_JX2C_PackBlockLZ_HashString(ctx, cs, bl);
			cs+=bl;

			cs0=cs;
			continue;
		}else
		{
			BGBCC_JX2C_PackBlockLZ_HashByte(ctx, cs);
			cs++;
		}
	}

	if(ct>cte)
		{ BGBCC_DBGBREAK }
	
	rl=cte-ct;
//	bl=cs-cs0;
	bl=cse-cs0;
	
//	if((rl+4)>bl)
//	if((bl+5)<rl)
	if(	((bl+5)<rl) ||
		(((bl+1)<rl) && (bl< 16)) ||
		(((bl+2)<rl) && (bl<271)) ||
		(((bl+3)<rl) && (bl<526)) ||
		(((bl+4)<rl) && (bl<781)) )
	{
		/* Out of input, encode end marker. */
	
		if(bl>=15)
			tg=0xF0;
		else
			tg=(bl<<4);
		
		*ct++=tg;

		if(bl>=15)
		{
			i=bl-15;
			if((bgbcc_packlz_cmp==6) && (i>=255))
				{ BGBCC_DBGBREAK }
			while(i>=255)
				{ *ct++=0xFF; i-=255; }
			*ct++=i;
		}
		while(cs0<cs)
		{
//			*ct++=0xAA; cs0++;
			*ct++=*cs0++;
		}
			
//		if((ct+1)<cte)
//			{ *(u16 *)ct=0; ct+=2; }

		while(ct<cte)
			*ct++=0;

		if(ct>cte)
			{ BGBCC_DBGBREAK }

		return(cs-ibuf);
	}
	
	if(rl<2)
	{
		if(ct>cte)
			{ BGBCC_DBGBREAK }

//		if(ct<cte)
//			*ct++=0;

//		if((ct+1)<cte)
//			{ *(u16 *)ct=0; ct+=2; }

		while(ct<cte)
			*ct++=0;

		return(cs-ibuf);
	}

	/* Encode raw bytes up to the end of the block. */

	cs=cs0;
	
//	if((rl-1)<15)
	if((rl-1)<=15)
	{
		i=rl-1;
		if(i>=15)
			{ rl-=(i-14); i=14; }
		*ct++=(i<<4);

		i=rl-1;
		while(i--)
		{
//			*ct++=0xAA; cs++;
			*ct++=*cs++;
		}
		
//		if((ct+1)<cte)
//			{ *(u16 *)ct=0; ct+=2; }

		while(ct<cte)
			*ct++=0;

		if(ct>cte)
			{ BGBCC_DBGBREAK }

		return(cs-ibuf);
	}

	if((rl-2)<=270)
	{
		*ct++=0xF0;
		i=(rl-2)-15;
		if(i<0)
//			{ rl+=-i; i=0; }
			{ BGBCC_DBGBREAK }
		if((bgbcc_packlz_cmp==6) && (i>=255))
			{ BGBCC_DBGBREAK }
		if(i>=255)
			{ rl-=i-254; i=254; }
		*ct++=i;

		i=rl-2;
		while(i--)
		{
//			*ct++=0xAA; cs++;
			*ct++=*cs++;
		}

//		if((ct+1)<cte)
//			{ *(u16 *)ct=0; ct+=2; }
		while(ct<cte)
			*ct++=0;

		if(ct>cte)
			{ BGBCC_DBGBREAK }

		return(cs-ibuf);
	}

	if((rl-3)<=525)
	{
		*ct++=0xF0;
		*ct++=0xFF;
		i=(rl-3)-15-255;
		if(i<0)
//			{ rl+=-i; i=0; }
			{ BGBCC_DBGBREAK }
		if((bgbcc_packlz_cmp==6) && (i>=255))
			{ BGBCC_DBGBREAK }
		if(i>=255)
			{ rl-=i-254; i=254; }
		*ct++=i;

		i=rl-3;
		while(i--)
		{
//			*ct++=0xAA; cs++;
			*ct++=*cs++;
		}

//		if((ct+1)<cte)
//			{ *(u16 *)ct=0; ct+=2; }

		while(ct<cte)
			*ct++=0;

		if(ct>cte)
			{ BGBCC_DBGBREAK }
		
		return(cs-ibuf);
	}

	if((rl-4)<=780)
	{
		*ct++=0xF0;
		*ct++=0xFF;
		*ct++=0xFF;
		i=(rl-4)-15-255-255;
		if(i<0)
//			{ rl+=-i; i=0; }
			{ BGBCC_DBGBREAK }
		if((bgbcc_packlz_cmp==6) && (i>=255))
			{ BGBCC_DBGBREAK }
		if(i>=255)
			{ rl-=i-254; i=254; }
		*ct++=i;

		i=rl-4;
		while(i--)
		{
//			*ct++=0xAA; cs++;
			*ct++=*cs++;
		}

//		if((ct+1)<cte)
//			{ *(u16 *)ct=0; ct+=2; }
		while(ct<cte)
			*ct++=0;

		if(ct>cte)
			{ BGBCC_DBGBREAK }
		
		return(cs-ibuf);
	}

	if((rl-5)<1035)
	{
		*ct++=0xF0;
		*ct++=0xFF;
		*ct++=0xFF;
		*ct++=0xFF;
		i=(rl-5)-15-255-255-255;
		if(i<0)
//			{ rl+=-i; i=0; }
			{ BGBCC_DBGBREAK }
		if((bgbcc_packlz_cmp==6) && (i>=255))
			{ BGBCC_DBGBREAK }
		if(i>=255)
			{ rl-=i-254; i=254; }
		*ct++=i;

		i=rl-5;
		while(i--)
		{
//			*ct++=0xAA; cs++;
			*ct++=*cs++;
		}

//		if((ct+1)<cte)
//			{ *(u16 *)ct=0; ct+=2; }
		while(ct<cte)
			*ct++=0;
		
		if(ct>cte)
			{ BGBCC_DBGBREAK }
		
		return(cs-ibuf);
	}
	
	BGBCC_DBGBREAK
	return(0);
}


int BGBCC_JX2C_PackBlockRP2(BGBCC_TransState *ctx,
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	byte *cs, *cse, *lcs;
	byte *ct, *cte;
	u32 v;
	int pl, pd;
	int l, d, rl, l1, d1;
	int i, j, k;
	
//	ctx->chn_base=ibuf;
	cs=ibuf; cse=ibuf+ibsz;
	ct=obuf; cte=obuf+obsz;
	
	pd=0; pl=0;
	
	lcs=cs;
	while((cs<cse) && (ct<cte))
	{
//		i=TgvLz_LookupMatch(ctx, cs, cse, &l, &d);
//		i=TgvLz_LookupMatchB(ctx, cs, cse, lcs, &l, &d);

		i=BGBCC_JX2C_PackBlockLZ_LookupMatchB(ctx, cs, cse, lcs, &l, &d);

#if 0
		i=BGBCC_JX2C_PackBlockLZ_LookupMatch(ctx, cs, cse, &l, &d);
		if(i>0)
		{
			j=BGBCC_JX2C_PackBlockLZ_LookupMatch(ctx, cs+1, cse, &l1, &d1);
			if(j && (l1>(l+1)))
//			if(j && (l1>(l+2)))
				i=0;
		}
#endif
		
//		if(d<=0)i=0;
		

		if(!i)
		{
			BGBCC_JX2C_PackBlockLZ_HashByte(ctx, cs);
			cs++;
			continue;
		}
			
		rl=cs-lcs;
		
//		j=rl+1+(rl/128)+5;
//		if((ct+j+1)>=cte)
//			break;

		j=BGBCC_JX2C_PackBlockLZ_EstMatchCost(ctx, rl, l, d);
		if((ct+j+1)>=cte)
			break;

//		ctx->stat_rlen[tgvlz_log2u(rl)]++;
//		ctx->stat_len[tgvlz_log2u(l)]++;
//		ctx->stat_dist[tgvlz_log2u(d)]++;

#if 1
		while(rl>(128+7))
		{
			j=(rl>>3)-1;
			if(j>511)j=511;
			
			k=(j+1)*8;

			v=0x3F+(j<<7);
			*ct++=v;
			*ct++=v>>8;

			memcpy(ct, lcs, k);
			ct+=k;	lcs+=k;	rl-=k;
		}
#endif

		while(rl>=8)
		{
			j=(rl>>3)-1;
			if(j>15)j=15;
			
			k=(j+1)*8;
			*ct++=0x07+(j<<4);
			memcpy(ct, lcs, k);
			ct+=k;	lcs+=k;	rl-=k;
		}

		if((rl<8) && (l<=10) && (d<=511))
		{
			d1=d;
			l1=l-3;
			v=(d1<<7)|(l1<<4)|(rl<<1)|0;
			*ct++=v;
			*ct++=v>>8;
			memcpy(ct, lcs, rl);
			ct+=rl;
		}
		else
			if((rl<8) && (l<=67) && (d<=8191))
		{
			d1=d;
			l1=l-4;

			v=(d1<<11)|(l1<<5)|(rl<<2)|1;
			*ct++=v;
			*ct++=v>>8;
			*ct++=v>>16;
			memcpy(ct, lcs, rl);
			ct+=rl;
		}
		else
			if((rl<8) && (l<=515) && (d<=131071))
		{
			d1=d;
			l1=l-4;

			v=(d1<<15)|(l1<<6)|(rl<<3)|3;
			*ct++=v;
			*ct++=v>>8;
			*ct++=v>>16;
			*ct++=v>>24;

			memcpy(ct, lcs, rl);
			ct+=rl;
		}
		else
			if((rl<8) && (l<=0x3FFF) && (d<=0x3FFFFF))
		{
			*ct++=0x0F|(rl<<5);
			d1=d;
			l1=l-4;

			if(l1<128)
			{
				*ct++=(l1<<1);
			}else
			{
				v=(l1<<2)|1;
				*ct++=v;
				*ct++=v>>8;
			}

			if(d1<32768)
			{
				v=d1<<1;
				*ct++=v;
				*ct++=v>>8;
			}else
			{
				v=(d1<<2)|1;
				*ct++=v;
				*ct++=v>>8;
				*ct++=v>>16;
			}

			memcpy(ct, lcs, rl);
			ct+=rl;
		}
		else
		{
			BGBCC_DBGBREAK
		}
		
		pl=l; pd=d;
		
//		TgvLz_HashStr(ctx, cs, l);
		BGBCC_JX2C_PackBlockLZ_HashString(ctx, cs, l);
		cs+=l;
		lcs=cs;
	}

//	rl=cs-lcs;
	rl=cse-lcs;

#if 1
	while(rl>(128+3))
	{
		j=(rl>>3)-1;
		if(j>511)j=511;
		
		k=(j+1)*8;

		while((ct+k+2)>=cte)
//		while((ct+k+2)>cte)
		{
			j--;
			if(j<0)break;
			k=(j+1)*8;
		}
		if(j<15)
			break;

		v=0x3F+(j<<7);
		*ct++=v;
		*ct++=v>>8;

		memcpy(ct, lcs, k);
		ct+=k;	lcs+=k;	rl-=k;
	}
#endif

	while(rl>8)
	{
		j=(rl>>3)-1;
		if(j>15)j=15;
		
		k=(j+1)*8;

//		if((ct+k+1)>=cte)
//			break;
		while((ct+k+1)>=cte)
//		while((ct+k+1)>cte)
		{
			j--;
			if(j<0)break;
			k=(j+1)*8;
		}
		if(j<0)break;

		*ct++=0x07+(j<<4);
		memcpy(ct, lcs, k);
		ct+=k;	rl-=k;	lcs+=k;
	}

	while(rl)
	{
		j=rl;
		if(j>3)j=3;
		
		k=(cte-ct)-2;
//		k=(cte-ct)-1;
		if(k<1)break;
		if(k<j)j=k;
		
		if((ct+j+1)>=cte)
//		if((ct+j+1)>cte)
			break;

		*ct++=0x1F+(j<<6);
		memcpy(ct, lcs, j);
		ct+=j;	rl-=j;	lcs+=j;
	}

	if(cs<cte)
		*ct++=0x1F;
	
//	return(ct-obuf);
	return(lcs-ibuf);
}

int BGBCC_JX2C_PackBlockLZ(BGBCC_TransState *ctx,
	byte *obuf, byte *ibuf, int obsz, int ibsz)
{
	int i, j, k;

	if((bgbcc_packlz_cmp==4) || (bgbcc_packlz_cmp==6))
	{
		i=BGBCC_JX2C_PackBlockLZ4(ctx, obuf, ibuf, obsz, ibsz);
		return(i);
	}

	if(bgbcc_packlz_cmp==3)
	{
		i=BGBCC_JX2C_PackBlockRP2(ctx, ibuf, obuf, ibsz, obsz);
		return(i);
	}

	if(bgbcc_packlz_cmp==0)
	{
		memcpy(obuf, ibuf, obsz);
		return(obsz);
	}
	
	return(-1);
}

//int BGBCC_JX2C_PackBlockRP2(BGBCC_TransState *ctx,
//	byte *ibuf, byte *obuf, int ibsz, int obsz)

int BGBCC_JX2C_PackImagePEL(BGBCC_TransState *ctx,
	byte *obuf, byte *ibuf, int obsz, int ibsz)
{
	byte *cs, *ct, *cse, *cte;
	int i;
	
	BGBCC_JX2C_PackBlockLZ_Reset(ctx);
//	bgbcc_packlz_cmp=4;
//	bgbcc_packlz_cmp=3;
	bgbcc_packlz_cmp=ctx->pel_cmpr;
	
	ct=obuf; cs=ibuf;
	cte=obuf+obsz;
	cse=ibuf+ibsz;
	
	memcpy(ct, cs, 1024);
	ct+=1024; cs+=1024;
	
	while(cs<cse)
	{
//		i=BGBCC_JX2C_PackBlockLZ4(ctx, ct, cs, 1024, cse-cs);
		i=BGBCC_JX2C_PackBlockLZ(ctx, ct, cs, 1024, cse-cs);
		ct+=1024;
		cs+=i;
	}
	
	return(ct-obuf);
}

byte bgbcc_tkpe_endseen;

byte *BGBCC_TKPE_UnpackL4(byte *obuf, byte *ibuf, int isz, byte *imgbase)
{
	byte *cs, *ct, *cse, *cs1, *cs1e, *ct1, *cs0, *ct0;
	byte *cs0a, *ct0a;
	int tg, lr, ll, ld;
	int i;
	
	cs0=NULL; ct0=NULL;
	
	ct=obuf;
	cs=ibuf; cse=ibuf+isz;
	while(cs<cse)
	{
		cs0a=cs0;	ct0a=ct0;
		cs0=cs;		ct0=ct;
		tg=*cs++;
		lr=(tg>>4)&15;
		if(lr==15)
		{
			i=*cs++;
			while(i==255)
				{ lr+=255; i=*cs++; }
			lr+=i;
		}
		
		ct1=ct; cs1=cs; cs1e=cs+lr;
		while(cs1<cs1e)
		{
			*(u64 *)ct1=*(u64 *)cs1;
			ct1+=8; cs1+=8;
		}
		ct+=lr; cs+=lr;
		
		if((cs+1)>=cse)
			break;
		
		ll=(tg&15)+4;

//		ld=tkfat_getWord(cs);
		ld=*(u16 *)cs;
//		if(!ld)
//			break;
		cs+=2;
		if(!ld)
		{
			if(ll==5)
			{
				continue;
			}
		
			bgbcc_tkpe_endseen=1;
			break;
		}

		if(ll==19)
		{
			i=*cs++;
			while(i==255)
				{ ll+=255; i=*cs++; }
			ll+=i;
		}
		
		if(cs>cse)
		{
			BGBCC_DBGBREAK
//			cs=ibuf;
//			ct=obuf;

			cs=cs0a;
			ct=ct0a;
			continue;
		}
		
		cs1=ct-ld; cs1e=cs1+ll;
		if(ld>=8)
//		if(0)
		{
			ct1=ct;
			while(cs1<cs1e)
			{
//				*(u64 *)ct1=*(u64 *)cs1; ct1+=8; cs1+=8;
				((u64 *)ct1)[0]=((u64 *)cs1)[0];
				((u64 *)ct1)[1]=((u64 *)cs1)[1];
				ct1+=16; cs1+=16;
			}
			ct+=ll;
		}else
			if(ld>=4)
//			if(0)
		{
			ct1=ct;
			while(cs1<cs1e)
			{
				((u32 *)ct1)[0]=((u32 *)cs1)[0];
				((u32 *)ct1)[1]=((u32 *)cs1)[1];
				ct1+=8; cs1+=8;
			}
			ct+=ll;
//			__debugbreak();
		}else
		{
			while(cs1<cs1e)
				{ *ct++=*cs1++; }
		}
	}
	
	return(ct);
}

void W_MatchCopy2(byte *dst, int sz, int d)
{
	byte *cs, *ct, *cte;
	u64 v;
	
	if(d<8)
	{
		if(d==1)
		{
			v=*(dst-d);
			v=v|(v<<8);
			v=v|(v<<16);
			v=v|(v<<32);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(u64 *)ct=v;
				ct+=8;
			}
		}else
			if(d==2)
		{
			v=*(u16 *)(dst-d);
			v=v|(v<<16);
			v=v|(v<<32);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(u64 *)ct=v;
				ct+=8;
			}
		}else
			if(d==4)
		{
			v=*(u32 *)(dst-d);
			v=v|(v<<32);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(u64 *)ct=v;
				ct+=8;
			}
		}else
		{
			v=*(u64 *)(dst-d);
			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(u64 *)ct=v;
				ct+=d;
			}
		}
	}else
		if(sz<=16)
	{
		cs=dst-d;
//		((u64 *)dst)[0]=((u64 *)cs)[0];
//		((u64 *)dst)[1]=((u64 *)cs)[1];
		memcpy(dst+0, cs+0, 8);
		memcpy(dst+8, cs+8, 8);
	}else
	{
		cs=dst-d;
		ct=dst; cte=dst+sz;
		while(ct<cte)
		{
//			((u64 *)ct)[0]=((u64 *)cs)[0];
//			((u64 *)ct)[1]=((u64 *)cs)[1];
			memcpy(ct+0, cs+0, 8);
			memcpy(ct+8, cs+8, 8);
			ct+=16; cs+=16;
		}
	}
}

void W_RawCopyB(byte *dst, byte *src, int sz)
{
	byte *cs, *ct, *cte;

	cs=src;
	ct=dst; cte=dst+sz;
	while(ct<cte)
	{
//		((u64 *)ct)[0]=((u64 *)cs)[0];
//		((u64 *)ct)[1]=((u64 *)cs)[1];
//		memcpy(ct, cs, 16);
		memcpy(ct+0, cs+0, 8);
		memcpy(ct+8, cs+8, 8);
		ct+=16; cs+=16;
	}
}

int W_DecodeBufferRP2(
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	u32 tag;
	byte *cs, *ct, *cse;
	int pl, pd;
	int rl, l, d;
	u64 t0;
	int t1, t2;
	
	cs=ibuf; cse=ibuf+ibsz;
	ct=obuf;
	pl=0; pd=0;
	
//	while(1)
	while(cs<cse)
	{
		t0=*(u64 *)cs;
		if(!(t0&0x01))
		{
			cs+=2;
			rl=(t0>>1)&7;
			l=((t0>>4)&7)+3;
			d=(t0>>7)&511;
		}else
			if(!(t0&0x02))
		{
			cs+=3;
			rl=(t0>>2)&7;
			l=((t0>>5)&63)+4;
			d=(t0>>11)&8191;
		}else
			if(!(t0&0x04))
		{
			cs+=4;
			rl=(t0>>3)&7;
			l=((t0>>6)&511)+4;
			d=(t0>>15)&131071;
		}else
			if(!(t0&0x08))
		{
			cs++;
			t1=(t0>>4)&15;
			rl=(t1+1)*8;
			W_RawCopyB(ct, cs, rl);
			cs+=rl;
			ct+=rl;
			continue;
		}else
			if(!(t0&0x10))
		{
			/* Long Match */
			cs++;
			rl=(t0>>5)&7;
			t1=t0>>8;
			if(!(t1&1))
				{ l=((t1>>1)&0x007F)+4; cs+=1; t2=t0>>16; }
			else
				{ l=((t1>>2)&0x3FFF)+4; cs+=2; t2=t0>>24; }
			if(!(t2&1))
				{ d=((t2>>1)&0x007FFF); cs+=2; }
			else
				{ d=((t2>>2)&0x3FFFFF); cs+=3; }
		}else
			if(!(t0&0x20))
		{
			cs++;
			rl=(t0>>6)&3;
			if(!rl)break;
//			*(u32 *)ct=*(u32 *)cs;
			memcpy(ct, cs, 4);
			cs+=rl;
			ct+=rl;
			continue;
		}else
			if(!(t0&0x40))
		{
			/* Long Raw */
			cs+=2;
			t1=(t0>>7)&511;
			rl=(t1+1)*8;
			W_RawCopyB(ct, cs, rl);
			cs+=rl;
			ct+=rl;
			continue;
		}else
		{
//			__debugbreak();
			BGBCC_DBGBREAK
		}

//		*(u64 *)ct=*(u64 *)cs;
		memcpy(ct, cs, 8);
		cs+=rl;
		ct+=rl;
		W_MatchCopy2(ct, l, d);
		ct+=l;
	}
	
	return(ct-obuf);
}


byte *BGBCC_TKPE_UnpackBuffer(
	byte *obuf, byte *ibuf, int isz, byte *imgbase,
	int cmp)
{
	int sz;
	if((cmp==4) || (cmp==6))
	{
		return(BGBCC_TKPE_UnpackL4(obuf, ibuf, isz, imgbase));
	}
	
	if(cmp==3)
	{
		sz=W_DecodeBufferRP2(ibuf, obuf, isz, 999999);
		return(obuf+sz);
	}
	
	if(cmp==0)
	{
		memcpy(obuf, ibuf, isz);
		return(obuf+isz);
	}
	
	return(NULL);
}

int BGBCC_JX2C_VerifyImagePEL(BGBCC_TransState *ctx,
	byte *obuf, byte *ibuf, int obsz, int ibsz)
{
	byte *tbuf;
	byte *cs, *ct, *cse;
	int cb, nb, cmp;
	int i, j, k;
	
	tbuf=malloc(ibsz+65536);
	
	bgbcc_tkpe_endseen=0;
	cs=obuf; cse=obuf+obsz;
	ct=tbuf;
	
	cmp=0;
	if((cs[2]=='L') && (cs[3]>='0') && (cs[3]<='9'))
	{
		cmp=cs[3]-'0';
	}
	
	memcpy(ct, cs, 1024);
	ct[2]=0;	ct[3]=0;
	ct+=1024;	cs+=1024;
	
	cb=0; nb=obsz>>10;
	while(cs<cse)
	{
		k=cse-cs;
		if(k>1024)k=1024;
//		k=1024;
//		ct=BGBCC_TKPE_UnpackL4(ct, cs, k);
//		ct=BGBCC_TKPE_UnpackL4(ct, cs, 1024, obuf);
		ct=BGBCC_TKPE_UnpackBuffer(ct, cs, 1024, obuf, cmp);
		cs+=k;
		cb++;
		if(bgbcc_tkpe_endseen)
			break;
	}
	
	if((ct-tbuf)!=ibsz)
		{ BGBCC_DBGBREAK }
	
	if(memcmp(ibuf, tbuf, ibsz))
	{
		for(i=0; i<ibsz; i++)
			if(ibuf[i]!=tbuf[i])
				break;
		BGBCC_DBGBREAK
	}
	
	free(tbuf);
	
	return(0);
}

/* Calculate PE/COFF checksum.
 * This algorithm is pretty weak.
 */
u32 BGBCC_JX2C_CalculateImagePeChecksum(byte *buf, int size, int en)
{
	byte *cs, *cse, *cs1e;
	u64 acc;
	u32 csum;
	
	cs=buf;
	cse=cs+size;
	acc=0;
	while(cs<cse)
	{
		acc=acc+bgbcc_getu32en(cs, en);
		acc=((u32)acc)+(acc>>32);
		cs+=4;
	}
	acc=((u32)acc)+(acc>>32);
	acc=((u32)acc)+(acc>>32);
	csum=acc;
	csum=((u16)csum)+(csum>>16);
	csum=((u16)csum)+(csum>>16);
	
	csum+=size;
	return(csum);
}

u32 BGBCC_JX2C_CalculateImagePel4Checksum(byte *buf, int size, int en)
{
	byte *cs, *cse;
//	u64 acc;
	u32 acc_lo, acc_hi;
	u32 csum;
	
	cs=buf;
	cse=cs+size;
	acc_lo=1;
	acc_hi=0;
	while(cs<cse)
	{
		acc_lo=acc_lo+(*cs++);
		if(acc_lo>=65521)
			acc_lo-=65521;
		acc_hi=acc_hi+acc_lo;
		if(acc_hi>=65521)
			acc_hi-=65521;
	}
	csum=acc_lo|(acc_hi<<16);
//	csum+=size;
	return(csum);
}

u32 BGBCC_JX2C_CalculateImagePel4BChecksum(byte *buf, int size, int en)
{
	byte *cs, *cse;
//	u64 acc;
	u32 v;
	u64 acc_lo, acc_hi;
	u32 csum;
	
	cs=buf;
	cse=cs+size;
	acc_lo=1;
	acc_hi=0;
	while(cs<cse)
	{
		v=bgbcc_getu32en(cs, en);
		acc_lo=acc_lo+v;
//		acc_lo=((u32)acc_lo)+(acc_lo>>32);

		acc_hi=acc_hi+acc_lo;
//		acc_hi=((u32)acc_hi)+(acc_hi>>32);
		cs+=4;
	}
	acc_lo=((u32)acc_lo)+(acc_lo>>32);
	acc_lo=((u32)acc_lo)+(acc_lo>>32);
	acc_hi=((u32)acc_hi)+(acc_hi>>32);
	acc_hi=((u32)acc_hi)+(acc_hi>>32);
	csum=(u32)(acc_lo^acc_hi);
//	csum+=size;
	return(csum);
}

ccxl_status BGBCC_JX2C_FlattenImagePECOFF(BGBCC_TransState *ctx,
	byte *obuf, int *rosz, fourcc imgfmt)
{
//	int rlc_rva_page[4096];
//	int rlc_sz_page[4096];
	u32 rlctab[65536];
	char tb[256];

//	s32 map_lvatab[16384];
//	char *map_lvntab[16384];
//	byte map_lvmtab[16384];

	s32 *map_lvatab;
	char **map_lvntab;
	byte *map_lvmtab;
	int map_nlbln;

	BGBCC_JX2_Context *sctx;
	FILE *mapfd;
	char *s0;
	byte *ct, *ct0, *ct1, *ctb;
	byte no_mz, is_pel;
	u32 csum;

	int en, ofs, ofs_sdat, ofs_iend, ofs_mend, ofs_iend2;
	int of_phdr, ne_phdr;
	int of_shdr, ne_shdr;
	int lb_strt, va_strt, lbl_gptr;
	int img_base, img_base_hi;
	int nm, fl, lva, rva, lsz, sn_strs, imty;
	int lpg, szrlc, ofsrlc, nrlce, mach, lbl;
	int ofsimp, szimp, ofsexp, szexp;
	int ofsexc, szexc, ofsrsrc, szrsrc, ofstlsd, sztlsd;
	int szstack;
	int i, j, k;

	sctx=ctx->uctx;

//	is_pel=0;
//	is_pel=1;
	is_pel=((ctx->pel_cmpr&255)!=255);

	no_mz=0;
	if(is_pel)
		no_mz=1;
	

	BGBCC_JX2C_CoffBuildExports(ctx, sctx);
	BGBCC_JX2C_CoffBuildImports(ctx, sctx);

//	img_base=0x0C000000;
//	img_base=0x01100000;
	img_base=0x01100000;
	
	if(imgfmt==BGBCC_IMGFMT_EXE)
//		img_base=0x02000000;
		img_base=0x01100000;
	if(imgfmt==BGBCC_IMGFMT_SYS)
		img_base=0x01100000;
	if(imgfmt==BGBCC_IMGFMT_DLL)
		img_base=0x00010000;
	
	img_base_hi=0;
	sctx->image_base=img_base;
	
	k=0x400;
	for(i=0; i<sctx->nsec; i++)
	{
		if(i==BGBCC_SH_CSEG_BSS)
			continue;
		if(BGBCC_JX2_IsSectionGpRel(sctx, i))
			continue;
		j=sctx->sec_pos[i]-sctx->sec_buf[i];
		sctx->sec_rva[i]=k;
		sctx->sec_lva[i]=img_base+k;
		sctx->sec_lsz[i]=j;
		k+=j;
		k=(k+63)&(~63);
	}
	
	/* If .utext exists, pad data sections to a new page. */
	i=BGBCC_SH_CSEG_UTEXT;
	j=sctx->sec_pos[i]-sctx->sec_buf[i];
	if((j>0) && !(sctx->is_pbo))
		{ k=(k+16383)&(~16383); }

	sctx->gbr_rva=k;
	
	if(sctx->is_pbo)
	{
		for(i=0; i<sctx->nsec; i++)
		{
			if(i==BGBCC_SH_CSEG_BSS)
				continue;
			if(!BGBCC_JX2_IsSectionGpRel(sctx, i))
				continue;
			j=sctx->sec_pos[i]-sctx->sec_buf[i];
			sctx->sec_rva[i]=k;
			sctx->sec_lva[i]=img_base+k;
			sctx->sec_lsz[i]=j;
			k+=j;
			k=(k+63)&(~63);
		}
	}

	i=BGBCC_SH_CSEG_BSS;
	j=sctx->sec_pos[i]-sctx->sec_buf[i];
	sctx->sec_rva[i]=k;
	sctx->sec_lva[i]=img_base+k;
	sctx->sec_lsz[i]=j;

	lbl_gptr=BGBCC_JX2_LookupNamedLabel(sctx, "__global_ptr");

	lpg=-1; szrlc=0; nrlce=0;
	for(i=0; i<sctx->nrlc; i++)
	{
		if(	(sctx->rlc_ty[i]!=BGBCC_SH_RLC_ABS16) &&
			(sctx->rlc_ty[i]!=BGBCC_SH_RLC_ABS32) &&
			(sctx->rlc_ty[i]!=BGBCC_SH_RLC_ABS64) &&
			(sctx->rlc_ty[i]!=BGBCC_SH_RLC_ABS96) &&
			(sctx->rlc_ty[i]!=BGBCC_SH_RLC_PBO24_BJX) &&
			(sctx->rlc_ty[i]!=BGBCC_SH_RLC_PBO32_BJX) &&
			(sctx->rlc_ty[i]!=BGBCC_SH_RLC_TBR24_BJX) &&
			(sctx->rlc_ty[i]!=BGBCC_SH_RLC_TBR12_BJX) &&
			(sctx->rlc_ty[i]!=BGBCC_SH_RLC_ABS48_BJX) &&
			(sctx->rlc_ty[i]!=BGBCC_SH_RLC_TRIPWIRE_BJX))
				continue;
			
		j=sctx->rlc_sec[i];
		rva=sctx->sec_rva[j]+
			sctx->rlc_ofs[i];
		lbl=sctx->rlc_id[i];

		rva=(j<<24)|(sctx->rlc_ofs[i]);

		lva=rva&0x0FFFFFFF;
		if(sctx->rlc_ty[i]==BGBCC_SH_RLC_ABS64)
			lva|=0x10000000;
		if(sctx->rlc_ty[i]==BGBCC_SH_RLC_ABS16)
			lva|=0x20000000;

		if(sctx->rlc_ty[i]==BGBCC_SH_RLC_ABS96)
			lva|=0xA0000000;

		if(sctx->rlc_ty[i]==BGBCC_SH_RLC_ABS48_BJX)
			lva|=0x90000000;

		if(sctx->rlc_ty[i]==BGBCC_SH_RLC_TRIPWIRE_BJX)
			lva|=0x80000000;

		if(sctx->rlc_ty[i]==BGBCC_SH_RLC_PBO24_BJX)
		{
			if(lbl==lbl_gptr)
			{
//				lva|=0x30000000;
				lva|=0x60000000;
			}else
			{
				lva|=0x40000000;
				continue;
			}
//			if(rva!=sctx->gbr_rva)
//				continue;
		}
		if(sctx->rlc_ty[i]==BGBCC_SH_RLC_PBO32_BJX)
		{
			lva|=0x50000000;
			if(rva!=sctx->gbr_rva)
				continue;
		}
		if(sctx->rlc_ty[i]==BGBCC_SH_RLC_TBR24_BJX)
			lva|=0x60000000;
		if(sctx->rlc_ty[i]==BGBCC_SH_RLC_TBR12_BJX)
		{
//			lva|=0x70000000;
			lva|=0x30000000;
		}
		
		if(nrlce>=65536)
			{ BGBCC_DBGBREAK }

		rlctab[nrlce++]=lva;

#if 0
		if((rva>>12)!=lpg)
		{
			szrlc=(szrlc+3)&(~3);
			szrlc+=8;
			lpg=rva>>12;
		}
		szrlc+=2;
#endif

	}

#if 1
	for(i=0; i<nrlce; i++)
		for(j=i+1; j<nrlce; j++)
	{
//		if(rlctab[j]<rlctab[i])
		if((rlctab[j]&0x0FFFFFFF)<(rlctab[i]&0x0FFFFFFF))
		{
			k=rlctab[i];
			rlctab[i]=rlctab[j];
			rlctab[j]=k;
		}
	}
#endif

//	bgbcc_jx2c_qrsort(rlctab, nrlce, 0);

#if 1
	lpg=-1; szrlc=0;
	for(i=0; i<nrlce; i++)
	{
		lva=rlctab[i];
		rva=lva&0x0FFFFFFF;

		if((rva>>12)!=lpg)
		{
			szrlc=(szrlc+3)&(~3);
			szrlc+=8;
			lpg=rva>>12;
		}
		szrlc+=2;
	}
#endif

	BGBCC_JX2_SetSectionName(sctx, ".reloc");
	BGBCC_JX2_EmitBAlign(sctx, 4);
	ofsrlc=BGBCC_JX2_EmitGetOffs(sctx);
	BGBCC_JX2_EmitRawBytes(sctx, NULL, szrlc+256);

	BGBCC_JX2_EmitGetStrtabLabel(sctx, "");

	ne_shdr=0;
	for(i=0; i<sctx->nsec; i++)
	{
		j=sctx->sec_pos[i]-sctx->sec_buf[i];
		if(j>0)
			ne_shdr++;

		s0=sctx->sec_name[i];
		if(s0)
		{
			BGBCC_JX2_EmitGetStrtabLabel(sctx, s0);
		}else
		{
			switch(i)
			{
			case BGBCC_SH_CSEG_TEXT: s0=".text"; break;
			case BGBCC_SH_CSEG_STRTAB: s0=".strtab"; break;
			case BGBCC_SH_CSEG_GOT: s0=".got"; break;
			case BGBCC_SH_CSEG_DATA: s0=".data"; break;
			case BGBCC_SH_CSEG_RODATA: s0=".rdata"; break;
			case BGBCC_SH_CSEG_BSS: s0=".bss"; break;
			case BGBCC_SH_CSEG_RELOC: s0=".reloc"; break;

			case BGBCC_SH_CSEG_UTEXT: s0=".utext"; break;
			case BGBCC_SH_CSEG_UDATA: s0=".udata"; break;

			case BGBCC_SH_CSEG_DYN: s0=".dyn"; break;
			case BGBCC_SH_CSEG_ABS: s0=".abs"; break;
			default:
				sprintf(tb, ".unk%03X", i);
				s0=bgbcc_strdup(tb);
//				s0=".unknown";
				break;
			}

			BGBCC_JX2_EmitGetStrtabLabel(sctx, s0);
			sctx->sec_name[i]=s0;
		}
	}

	en=(sctx->is_le==0);
//	ne_phdr=1;
//	ne_shdr=sctx->nsec;
	
//	of_phdr=64;
	
//	of_shdr=of_phdr+(ne_phdr*32);
//	of_shdr=(of_shdr+15)&(~15);

	of_shdr=0x0138;
//	of_shdr=0x00E8;

	if(sctx->is_addr64 && !sctx->is_addr_x32)
		of_shdr=0x0148;

	if(no_mz)
	{
		of_shdr-=0x40;
	}

	ofs_sdat=of_shdr+(ne_shdr*40);
	ofs_sdat=(ofs_sdat+63)&(~63);

#if 1
	k=0x400;
	for(i=0; i<sctx->nsec; i++)
	{
//		if(i==BGBCC_SH_CSEG_BSS)
//			continue;

		j=sctx->sec_pos[i]-sctx->sec_buf[i];
//		sctx->sec_rva[i]=k;
//		sctx->sec_lva[i]=img_base+k;
//		sctx->sec_lsz[i]=j;

		if((j>0) && (i==BGBCC_SH_CSEG_UTEXT) && !(sctx->is_pbo))
		{
			k=(k+16383)&(~16383);
		}

		k+=j;
		k=(k+63)&(~63);
	}
	memset(obuf, 0, k);
#endif

//	k=ofs_sdat;
	k=0x400;
	for(i=0; i<sctx->nsec; i++)
	{
//		if(sctx->sec_lsz[i]<=0)
//			continue;

		if(i==BGBCC_SH_CSEG_BSS)
			continue;

//		s0=sctx->sec_name[i];
//		j=BGBCC_SHXC_CoffSectionFlags(ctx, sctx, s0);
//		if(j&BGBCC_COFF_SCNT_GPREL)
//			continue;
		if(BGBCC_JX2_IsSectionGpRel(sctx, i))
			continue;

		j=sctx->sec_pos[i]-sctx->sec_buf[i];
		if(!j)
			continue;

		/* pad .utext to its own page. */
		if((j>0) && (i==BGBCC_SH_CSEG_UTEXT) && !(sctx->is_pbo))
			{ k=(k+16383)&(~16383); }

		sctx->sec_rva[i]=k;
		sctx->sec_lva[i]=img_base+k;
		sctx->sec_lsz[i]=j;
		memcpy(obuf+k, sctx->sec_buf[i], j);
		k+=j;
		k=(k+63)&(~63);

		if(sctx->verbose)
		{
			s0=sctx->sec_name[i];
			printf("%d: %s %08X..%08X %d\n", i, s0,
				sctx->sec_lva[i], sctx->sec_lva[i]+sctx->sec_lsz[i],
				sctx->sec_lsz[i]);
		}
	}

	/* If .utext exists, pad data sections to a new page. */
	i=BGBCC_SH_CSEG_UTEXT;
	j=sctx->sec_pos[i]-sctx->sec_buf[i];
	if((j>0) && !(sctx->is_pbo))
		{ k=(k+16383)&(~16383); }

	sctx->gbr_rva=k;
	
	if(sctx->is_pbo)
	{
		for(i=0; i<sctx->nsec; i++)
		{
			if(i==BGBCC_SH_CSEG_BSS)
				continue;

//			s0=sctx->sec_name[i];
//			j=BGBCC_SHXC_CoffSectionFlags(ctx, sctx, s0);
//			if(!(j&BGBCC_COFF_SCNT_GPREL))
//				continue;
			if(!BGBCC_JX2_IsSectionGpRel(sctx, i))
				continue;

			j=sctx->sec_pos[i]-sctx->sec_buf[i];
			if(!j)
				continue;

			sctx->sec_rva[i]=k;
			sctx->sec_lva[i]=img_base+k;
			sctx->sec_lsz[i]=j;
			memcpy(obuf+k, sctx->sec_buf[i], j);
			k+=j;
			k=(k+63)&(~63);

			if(sctx->verbose)
			{
				s0=sctx->sec_name[i];
				printf("%d: %s %08X..%08X %d\n", i, s0,
					sctx->sec_lva[i], sctx->sec_lva[i]+sctx->sec_lsz[i],
					sctx->sec_lsz[i]);
			}

		}
	}
	
	ofs_iend=k;

	i=BGBCC_SH_CSEG_BSS;
	j=sctx->sec_pos[i]-sctx->sec_buf[i];
	sctx->sec_rva[i]=k;
//	sctx->sec_lva[i]=0x0C000000+k;
	sctx->sec_lva[i]=img_base+k;
	sctx->sec_lsz[i]=j;
	memset(obuf+k, 0, j);
	k+=j;
	k=(k+63)&(~63);

	if(sctx->verbose)
	{
		s0=sctx->sec_name[i];
		printf("%d: %s %08X..%08X %d\n", i, s0,
			sctx->sec_lva[i], sctx->sec_lva[i]+sctx->sec_lsz[i],
			sctx->sec_lsz[i]);
	}


	if((sctx->gbr_rva!=sctx->sec_rva[BGBCC_SH_CSEG_DATA]) && (sctx->is_pbo))
	{
		BGBCC_DBGBREAK
	}

	ofs_mend=k;
	
	ofs_iend2=ofs_mend;
//	if(is_pel)
	if(is_pel && sctx->is_pbo)
		ofs_iend2=ofs_iend;
	
	lb_strt=0;
//	lb_strt=BGBCC_JX2_LookupNamedLabel(sctx, "__start");
//	if(lb_strt<=0)
//		lb_strt=BGBCC_JX2_LookupNamedLabel(sctx, "___start");
	if(lb_strt<=0)
		lb_strt=BGBCC_JX2_LookupNamedLabel(sctx, "_start");
	if(lb_strt<=0)
		lb_strt=BGBCC_JX2_LookupNamedLabel(sctx, "__start");
	
	if(lb_strt>0)
		{ va_strt=BGBCC_JX2C_LookupLabelImgVA(ctx, sctx, lb_strt); }
	else
	{
		if(ctx->imgfmt!=BGBCC_IMGFMT_DLL)
			printf("BGBCC_JX2C_FlattenImagePECOFF: No Entry Point Found\n");
		va_strt=0;
	}

	ofsimp=0;	szimp=0;
	i=BGBCC_JX2_LookupSectionID(sctx, ".idata");
	if(i>=0)
	{	ofsimp=sctx->sec_rva[i];
		szimp=sctx->sec_lsz[i];		}

	ofsexp=0;	szexp=0;
	i=BGBCC_JX2_LookupSectionID(sctx, ".edata");
	if(i>=0)
	{	ofsexp=sctx->sec_rva[i];
		szexp=sctx->sec_lsz[i];		}

	ofsexc=0;	szexc=0;
	i=BGBCC_JX2_LookupSectionID(sctx, ".pdata");
	if(i>=0)
	{	ofsexc=sctx->sec_rva[i];
		szexc=sctx->sec_lsz[i];		}

	ofsrsrc=0;	szrsrc=0;
	i=BGBCC_JX2_LookupSectionID(sctx, ".rsrc");
	if(i>=0)
	{	ofsrsrc=sctx->sec_rva[i];
		szrsrc=sctx->sec_lsz[i];		}

	ofstlsd=0;	sztlsd=0;
	i=BGBCC_JX2_LookupSectionID(sctx, ".tls");
	if(i>=0)
	{	ofstlsd=sctx->sec_rva[i];
		sztlsd=sctx->sec_lsz[i];		}


	ofsrlc=sctx->sec_rva[BGBCC_SH_CSEG_RELOC]+ofsrlc;
	ctb=obuf+ofsrlc;
	szrlc=0; ct=ctb; ct0=ctb; lpg=-1;
//	for(i=0; i<sctx->nrlc; i++)
	for(i=0; i<nrlce; i++)
	{
//		if(	(sctx->rlc_ty[i]!=BGBCC_SH_RLC_ABS16) &&
//			(sctx->rlc_ty[i]!=BGBCC_SH_RLC_ABS32) &&
//			(sctx->rlc_ty[i]!=BGBCC_SH_RLC_ABS64))
//				continue;
			
//		j=sctx->rlc_sec[i];
//		rva=sctx->sec_rva[j]+
//			sctx->rlc_ofs[i];

		k=rlctab[i];
//		rva=k&0x0FFFFFFF;

		j=(k>>24)&15;
		rva=sctx->sec_rva[j]+(k&0x00FFFFFF);

		if((rva>>12)!=lpg)
		{
			j=ct-ctb;
			j=(j+3)&(~3);
			ct=ctb+j;
			j=ct-ct0;
			bgbcc_setu32en(ct0+4, en, j);

			ct0=ct;
			ct+=8;
			lpg=rva>>12;
			bgbcc_setu32en(ct0+0, en, (lpg<<12));
			bgbcc_setu32en(ct0+4, en, 0);
		}

#if 1
		switch((k>>28)&15)
		{
		case 0:		j=0x3000|(rva&0xFFF);	break;
		case 1:		j=0xA000|(rva&0xFFF);	break;
		case 2:		j=0x2000|(rva&0xFFF);	break;

		case 3:		j=0x6000|(rva&0xFFF);	break;

		case 4:		j=0x8000|(rva&0xFFF);	break;
		case 5:		j=0x7000|(rva&0xFFF);	break;
		case 6:		j=0x9000|(rva&0xFFF);	break;
		case 8:		j=0xB000|(rva&0xFFF);	break;
		case 9:		j=0x5000|(rva&0xFFF);	break;
		case 10:	j=0xC000|(rva&0xFFF);	break;
		default:	BGBCC_DBGBREAK			break;
		}
#endif

		bgbcc_setu16en(ct, en, j);
		ct+=2;
	}
	if(1)
	{
		j=ct-ctb;
		j=(j+3)&(~3);
		ct=ctb+j;
		j=ct-ct0;
		bgbcc_setu32en(ct0+4, en, j);
	}
	szrlc=ct-ctb;

	ct=obuf;
	if(no_mz)
	{
//		memset(ct, 0, 0x400);
		ct[0x00]='P';	ct[0x01]='E';	ct[0x02]=0;		ct[0x03]=0;
		of_phdr=0x00;
	}else
	{
//		memset(ct, 0, 0x400);
		ct[0x00]='M';	ct[0x01]='Z';	ct[0x02]=0;		ct[0x03]=0;
		ct[0x3C]=0x40;	ct[0x3D]=0;		ct[0x3E]=0;		ct[0x3F]=0;
		ct[0x40]='P';	ct[0x41]='E';	ct[0x42]=0;		ct[0x43]=0;
		ct+=0x40;
		of_phdr=0x40;
	}

	k=sctx->is_le?0x0080:0x8000;
	k|=0x0302;

//	mach=0x1A6;
//	if(sctx->has_bjx1mov)
//		mach=0xB132;
	if(sctx->is_addr64 && !sctx->is_addr_x32)
	{
//		mach=0xB164;
//		if(sctx->is_betav)
//			mach=0xB64C;
		mach=0xB264;
	}else
	{
		if(sctx->is_addr_x32)
			mach=0xB264;
		else
			mach=0xB232;
	}
	
	if(sctx->is_fixed32&2)
		mach=0xB265;

	szstack=1<<19;
	
	i=BGBCC_CCXL_CheckForOptParmInt(ctx, "stack");
	if(i>0)
	{
		if(!(i&(i-1)) && (i>=1024))
			szstack=i;
		if((i>=10) && (i<=26))
			szstack=1<<i;
	}

#if 1
	bgbcc_setu16en(ct+0x04, en, mach);		//mMachine
	bgbcc_setu16en(ct+0x06, en, ne_shdr);	//mNumberOfSections
	bgbcc_setu32en(ct+0x08, en, 0);			//mTimeDateStamp
	bgbcc_setu32en(ct+0x0C, en, 0);			//mPointerToSymbolTable
	bgbcc_setu32en(ct+0x10, en, 0);			//mNumberOfSymbols
	bgbcc_setu16en(ct+0x16, en, k);			//mCharacteristics

	if(!sctx->is_addr64 || sctx->is_addr_x32)
	{
		bgbcc_setu16en(ct+0x14, en, 224);		//mSizeOfOptionalHeader

		bgbcc_setu16en(ct+0x18, en, 0x010B);	//mMagic
		bgbcc_setu16en(ct+0x1A, en, 0x0000);	//mLinkerVersion

		j=sctx->sec_lsz[BGBCC_SH_CSEG_TEXT];
		bgbcc_setu32en(ct+0x1C, en, j);			//mSizeOfCode
		j=sctx->sec_lsz[BGBCC_SH_CSEG_DATA];
		bgbcc_setu32en(ct+0x20, en, j);			//mSizeOfInitializedData
		j=sctx->sec_lsz[BGBCC_SH_CSEG_BSS];
		bgbcc_setu32en(ct+0x24, en, j);			//mSizeOfUninitializedData
		j=va_strt-img_base;
		bgbcc_setu32en(ct+0x28, en, j);			//rvaAddressOfEntryPoint
		j=sctx->sec_rva[BGBCC_SH_CSEG_TEXT];
		bgbcc_setu32en(ct+0x2C, en, j);			//mBaseOfCode
		j=sctx->sec_rva[BGBCC_SH_CSEG_DATA];
		bgbcc_setu32en(ct+0x30, en, j);			//mBaseOfData
		bgbcc_setu32en(ct+0x34, en, img_base);	//mImageBase
		bgbcc_setu32en(ct+0x38, en, 64);		//mSectionAlignment
		bgbcc_setu32en(ct+0x3C, en, 64);		//mFileAlignment


//		bgbcc_setu32en(ct+0x50, en, ofs_mend);	//mSizeOfImage
		bgbcc_setu32en(ct+0x50, en, ofs_iend2);	//mSizeOfImage
		bgbcc_setu32en(ct+0x54, en, ofs_sdat);	//mSizeOfHeaders

		bgbcc_setu16en(ct+0x5C, en, 1);			//mSubsystem
		bgbcc_setu16en(ct+0x5E, en, 0x0140);	//mDllCharacteristics

//		bgbcc_setu32en(ct+0x60, en, 0x100000);	//mSizeOfStackReserve
		bgbcc_setu32en(ct+0x60, en, szstack);	//mSizeOfStackReserve
		bgbcc_setu32en(ct+0x64, en, 0x10000);	//mSizeOfStackCommit
		bgbcc_setu32en(ct+0x68, en, 0);			//mSizeOfHeapReserve
		bgbcc_setu32en(ct+0x6C, en, 0);			//mSizeOfHeapCommit
		bgbcc_setu32en(ct+0x70, en, 0);			//mLoaderFlags
		bgbcc_setu32en(ct+0x74, en, 6);			//mNumberOfRvaAndSizes

		bgbcc_setu32en(ct+0x78, en, ofsexp);	//rvaExportTable
		bgbcc_setu32en(ct+0x7C, en, szexp);		//szExportTable
		bgbcc_setu32en(ct+0x80, en, ofsimp);	//rvaImportTable
		bgbcc_setu32en(ct+0x84, en, szimp);		//szImportTable
		bgbcc_setu32en(ct+0x88, en, ofsrsrc);	//rvaBaseResourceTable
		bgbcc_setu32en(ct+0x8C, en, szrsrc);	//szBaseResourceTable
		bgbcc_setu32en(ct+0x90, en, ofsexc);	//rvaBaseExceptionTable
		bgbcc_setu32en(ct+0x94, en, szexc);		//szBaseExceptionTable

		bgbcc_setu32en(ct+0xA0, en, ofsrlc);	//rvaBaseRelocTable
		bgbcc_setu32en(ct+0xA4, en, szrlc);		//szBaseRelocTable

		j=sctx->is_pbo?sctx->gbr_rva:0;
		bgbcc_setu32en(ct+0xB8, en, j);			//RVA of GBR Base
		j=sctx->is_pbo?(ofs_mend-sctx->gbr_rva):0;
		bgbcc_setu32en(ct+0xBC, en, j);			//Size of PBO Region

		bgbcc_setu32en(ct+0xC0, en, ofstlsd);	//rvaTlsTable
		bgbcc_setu32en(ct+0xC4, en, sztlsd);	//szTlsTable
	}else
	{
		bgbcc_setu16en(ct+0x14, en, 240);		//mSizeOfOptionalHeader

		bgbcc_setu16en(ct+0x18, en, 0x020B);	//mMagic
		bgbcc_setu16en(ct+0x1A, en, 0x0000);	//mLinkerVersion

		j=sctx->sec_lsz[BGBCC_SH_CSEG_TEXT];
		bgbcc_setu32en(ct+0x1C, en, j);			//mSizeOfCode
		j=sctx->sec_lsz[BGBCC_SH_CSEG_DATA];
		bgbcc_setu32en(ct+0x20, en, j);			//mSizeOfInitializedData
		j=sctx->sec_lsz[BGBCC_SH_CSEG_BSS];
		bgbcc_setu32en(ct+0x24, en, j);			//mSizeOfUninitializedData
		j=va_strt-img_base;
		bgbcc_setu32en(ct+0x28, en, j);			//rvaAddressOfEntryPoint
		j=sctx->sec_rva[BGBCC_SH_CSEG_TEXT];
		bgbcc_setu32en(ct+0x2C, en, j);			//mBaseOfCode
		j=sctx->sec_rva[BGBCC_SH_CSEG_DATA];
//		bgbcc_setu32en(ct+0x30, en, j);			//mBaseOfData
		bgbcc_setu32en(ct+0x30, en, img_base);		//mImageBase (low)
		bgbcc_setu32en(ct+0x36, en, img_base_hi);	//mImageBase (high)
		bgbcc_setu32en(ct+0x38, en, 64);		//mSectionAlignment
		bgbcc_setu32en(ct+0x3C, en, 64);		//mFileAlignment


//		bgbcc_setu32en(ct+0x50, en, ofs_mend);	//mSizeOfImage
		bgbcc_setu32en(ct+0x50, en, ofs_iend2);	//mSizeOfImage
		bgbcc_setu32en(ct+0x54, en, ofs_sdat);	//mSizeOfHeaders

		bgbcc_setu16en(ct+0x5C, en, 1);			//mSubsystem
		bgbcc_setu16en(ct+0x5E, en, 0x0140);	//mDllCharacteristics

//		bgbcc_setu64en(ct+0x60, en, 0x100000);	//mSizeOfStackReserve
		bgbcc_setu64en(ct+0x60, en, szstack);	//mSizeOfStackReserve
		bgbcc_setu64en(ct+0x68, en, 0x10000);	//mSizeOfStackCommit
		bgbcc_setu64en(ct+0x70, en, 0);			//mSizeOfHeapReserve
		bgbcc_setu64en(ct+0x78, en, 0);			//mSizeOfHeapCommit

		bgbcc_setu32en(ct+0x80, en, 0);			//mLoaderFlags
		bgbcc_setu32en(ct+0x84, en, 6);			//mNumberOfRvaAndSizes
		
		bgbcc_setu32en(ct+0x88, en, ofsexp);	//rvaExportTable
		bgbcc_setu32en(ct+0x8C, en, szexp);		//szExportTable
		bgbcc_setu32en(ct+0x90, en, ofsimp);	//rvaImportTable
		bgbcc_setu32en(ct+0x94, en, szimp);		//szImportTable
		bgbcc_setu32en(ct+0x98, en, ofsrsrc);	//rvaBaseResourceTable
		bgbcc_setu32en(ct+0x9C, en, szrsrc);	//szBaseResourceTable
		bgbcc_setu32en(ct+0xA0, en, ofsexc);	//rvaBaseExceptionTable
		bgbcc_setu32en(ct+0xA4, en, szexc);		//szBaseExceptionTable

		bgbcc_setu32en(ct+0xB0, en, ofsrlc);	//rvaBaseRelocTable
		bgbcc_setu32en(ct+0xB4, en, szrlc);		//szBaseRelocTable

		j=sctx->is_pbo?sctx->gbr_rva:0;
		bgbcc_setu32en(ct+0xC8, en, j);			//RVA of GBR Base
		j=sctx->is_pbo?(ofs_mend-sctx->gbr_rva):0;
		bgbcc_setu32en(ct+0xCC, en, j);			//Size of PBO Region

		bgbcc_setu32en(ct+0xD0, en, ofstlsd);	//rvaTlsTable
		bgbcc_setu32en(ct+0xD4, en, sztlsd);	//szTlsTable
	}
#endif

#if 0
	bgbcc_setu16en(ct+0x44, en, mach);		//mMachine
	bgbcc_setu16en(ct+0x46, en, ne_shdr);	//mNumberOfSections
	bgbcc_setu32en(ct+0x48, en, 0);			//mTimeDateStamp
	bgbcc_setu32en(ct+0x4C, en, 0);			//mPointerToSymbolTable
	bgbcc_setu32en(ct+0x50, en, 0);			//mNumberOfSymbols
	bgbcc_setu16en(ct+0x56, en, k);			//mCharacteristics

	if(!sctx->is_addr64 || sctx->is_addr_x32)
	{
		bgbcc_setu16en(ct+0x54, en, 224);		//mSizeOfOptionalHeader

		bgbcc_setu16en(ct+0x58, en, 0x010B);	//mMagic
		bgbcc_setu16en(ct+0x5A, en, 0x0000);	//mLinkerVersion

		j=sctx->sec_lsz[BGBCC_SH_CSEG_TEXT];
		bgbcc_setu32en(ct+0x5C, en, j);			//mSizeOfCode
		j=sctx->sec_lsz[BGBCC_SH_CSEG_DATA];
		bgbcc_setu32en(ct+0x60, en, j);			//mSizeOfInitializedData
		j=sctx->sec_lsz[BGBCC_SH_CSEG_BSS];
		bgbcc_setu32en(ct+0x64, en, j);			//mSizeOfUninitializedData
		j=va_strt-img_base;
		bgbcc_setu32en(ct+0x68, en, j);			//rvaAddressOfEntryPoint
		j=sctx->sec_rva[BGBCC_SH_CSEG_TEXT];
		bgbcc_setu32en(ct+0x6C, en, j);			//mBaseOfCode
		j=sctx->sec_rva[BGBCC_SH_CSEG_DATA];
		bgbcc_setu32en(ct+0x70, en, j);			//mBaseOfData
		bgbcc_setu32en(ct+0x74, en, img_base);	//mImageBase
		bgbcc_setu32en(ct+0x78, en, 64);		//mSectionAlignment
		bgbcc_setu32en(ct+0x7C, en, 64);		//mFileAlignment


//		bgbcc_setu32en(ct+0x90, en, ofs_mend);	//mSizeOfImage
		bgbcc_setu32en(ct+0x90, en, ofs_iend2);	//mSizeOfImage
		bgbcc_setu32en(ct+0x94, en, ofs_sdat);	//mSizeOfHeaders

		bgbcc_setu16en(ct+0x9C, en, 1);			//mSubsystem
		bgbcc_setu16en(ct+0x9E, en, 0x0140);	//mDllCharacteristics

		bgbcc_setu32en(ct+0xA0, en, 0x100000);	//mSizeOfStackReserve
		bgbcc_setu32en(ct+0xA4, en, 0x10000);	//mSizeOfStackCommit
		bgbcc_setu32en(ct+0xA8, en, 0);			//mSizeOfHeapReserve
		bgbcc_setu32en(ct+0xAC, en, 0);			//mSizeOfHeapCommit
		bgbcc_setu32en(ct+0xB0, en, 0);			//mLoaderFlags
		bgbcc_setu32en(ct+0xB4, en, 6);			//mNumberOfRvaAndSizes

		bgbcc_setu32en(ct+0xB8, en, ofsexp);	//rvaExportTable
		bgbcc_setu32en(ct+0xBC, en, szexp);		//szExportTable
		bgbcc_setu32en(ct+0xC0, en, ofsimp);	//rvaImportTable
		bgbcc_setu32en(ct+0xC4, en, szimp);		//szImportTable

		bgbcc_setu32en(ct+0xE0, en, ofsrlc);	//rvaBaseRelocTable
		bgbcc_setu32en(ct+0xE4, en, szrlc);		//szBaseRelocTable
	}else
	{
		bgbcc_setu16en(ct+0x54, en, 240);		//mSizeOfOptionalHeader

		bgbcc_setu16en(ct+0x58, en, 0x020B);	//mMagic
		bgbcc_setu16en(ct+0x5A, en, 0x0000);	//mLinkerVersion

		j=sctx->sec_lsz[BGBCC_SH_CSEG_TEXT];
		bgbcc_setu32en(ct+0x5C, en, j);			//mSizeOfCode
		j=sctx->sec_lsz[BGBCC_SH_CSEG_DATA];
		bgbcc_setu32en(ct+0x60, en, j);			//mSizeOfInitializedData
		j=sctx->sec_lsz[BGBCC_SH_CSEG_BSS];
		bgbcc_setu32en(ct+0x64, en, j);			//mSizeOfUninitializedData
		j=va_strt-img_base;
		bgbcc_setu32en(ct+0x68, en, j);			//rvaAddressOfEntryPoint
		j=sctx->sec_rva[BGBCC_SH_CSEG_TEXT];
		bgbcc_setu32en(ct+0x6C, en, j);			//mBaseOfCode
		j=sctx->sec_rva[BGBCC_SH_CSEG_DATA];
//		bgbcc_setu32en(ct+0x70, en, j);			//mBaseOfData
		bgbcc_setu32en(ct+0x70, en, img_base);		//mImageBase (low)
		bgbcc_setu32en(ct+0x76, en, img_base_hi);	//mImageBase (high)
		bgbcc_setu32en(ct+0x78, en, 64);		//mSectionAlignment
		bgbcc_setu32en(ct+0x7C, en, 64);		//mFileAlignment


//		bgbcc_setu32en(ct+0x90, en, ofs_mend);	//mSizeOfImage
		bgbcc_setu32en(ct+0x90, en, ofs_iend2);	//mSizeOfImage
		bgbcc_setu32en(ct+0x94, en, ofs_sdat);	//mSizeOfHeaders

		bgbcc_setu16en(ct+0x9C, en, 1);			//mSubsystem
		bgbcc_setu16en(ct+0x9E, en, 0x0140);	//mDllCharacteristics

		bgbcc_setu64en(ct+0xA0, en, 0x100000);	//mSizeOfStackReserve
		bgbcc_setu64en(ct+0xA8, en, 0x10000);	//mSizeOfStackCommit
		bgbcc_setu64en(ct+0xB0, en, 0);			//mSizeOfHeapReserve
		bgbcc_setu64en(ct+0xB8, en, 0);			//mSizeOfHeapCommit

		bgbcc_setu32en(ct+0xC0, en, 0);			//mLoaderFlags
		bgbcc_setu32en(ct+0xC4, en, 6);			//mNumberOfRvaAndSizes
		
		bgbcc_setu32en(ct+0xC8, en, ofsexp);	//rvaExportTable
		bgbcc_setu32en(ct+0xCC, en, szexp);		//szExportTable
		bgbcc_setu32en(ct+0xD0, en, ofsimp);	//rvaImportTable
		bgbcc_setu32en(ct+0xD4, en, szimp);		//szImportTable

		bgbcc_setu32en(ct+0xF0, en, ofsrlc);	//rvaBaseRelocTable
		bgbcc_setu32en(ct+0xF4, en, szrlc);		//szBaseRelocTable
	}
#endif

	ct=obuf+of_shdr;
	for(i=0; i<sctx->nsec; i++)
	{
		if(sctx->sec_lsz[i]<=0)
			continue;

		nm=0;
		s0=sctx->sec_name[i];
		if(s0)
		{
			nm=BGBCC_JX2_EmitGetStrtabSecOfs(sctx, s0);
			nm+=sctx->sec_rva[BGBCC_SH_CSEG_STRTAB];
		}

		fl=3;
		if(i==BGBCC_SH_CSEG_TEXT)fl|=4;

		lva=sctx->sec_lva[i];
		rva=sctx->sec_rva[i];
		lsz=sctx->sec_lsz[i];

		if(strlen(s0)<=8)
			{ strncpy((char *)(ct+0), s0, 8); }
		else
			{ bgbcc_setu32en(ct+0x04, en, nm); }
		bgbcc_setu32en(ct+0x08, en, lsz);	//mVirtualSize
		bgbcc_setu32en(ct+0x0C, en, lva);	//mVirtualAddress
		k=(i==BGBCC_SH_CSEG_BSS)?0:lsz;
		bgbcc_setu32en(ct+0x10, en, k);		//mSizeOfRawData
		bgbcc_setu32en(ct+0x14, en, rva);	//mPointerToRawData
		bgbcc_setu32en(ct+0x18, en, 0);		//mPointerToRealocations
		bgbcc_setu32en(ct+0x1C, en, 0);		//mPointerToLinenumbers
		bgbcc_setu16en(ct+0x20, en, 0);		//mNumberOfRealocations
		bgbcc_setu16en(ct+0x22, en, 0);		//mNumberOfLinenumbers

		k=BGBCC_JX2C_CoffSectionFlags(ctx, sctx, s0);
		bgbcc_setu32en(ct+0x24, en, k);		//mCharacteristics
		ct+=40;
	}

//	sn_strs=BGBCC_SH_CSEG_STRTAB+1;

	imty=2;
//	if(sctx->is_pic)
//		imty=3;

	BGBCC_JX2C_ApplyImageRelocs(ctx, sctx, obuf);

#if 1
	i=sctx->nlbln+sctx->lblllnrov;
	k=4096;
	while(k<i)
		k=k+(k>>1);

	map_lvatab=bgbcc_malloc(k*sizeof(u32));
	map_lvntab=bgbcc_malloc(k*sizeof(char *));
	map_lvmtab=bgbcc_malloc(k*sizeof(byte));


	sprintf(tb, "%s.map", ctx->imgname);
	mapfd=fopen(tb, "wt");
//	mapfd=fopen("aout.map", "wt");
	map_nlbln=0;
	for(i=0; i<sctx->nlbln; i++)
	{
//		if(!strcmp(ctx->lbln_name[i], name))
//			return(ctx->lbln_id[i]);

		lva=BGBCC_JX2C_LookupLabelImgVA(ctx, sctx, sctx->lbln_id[i]);
//		fprintf(mapfd, "%08X T %s\n", lva, sctx->lbln_name[i]);
		map_lvatab[i]=lva;
		map_lvntab[i]=sctx->lbln_name[i];
//		map_lvmtab[i]='T';
		map_lvmtab[i]=BGBCC_JX2C_LookupLabelImgMapTag(
			ctx, sctx, sctx->lbln_id[i]);
	}
	map_nlbln+=sctx->nlbln;

	for(i=0; i<sctx->lblllnrov; i++)
	{
		k=sctx->dbglln_srcpos[i];
		if(k&0x8000)
			continue;

		lva=BGBCC_JX2C_LookupLabelImgVA(ctx, sctx,
			(CCXL_LBL_GENLLNBASE+i));
		
		sprintf(tb, "%s:%d", bgbcc_jx2_NameForSrcIdx(k>>16), k&65535);

		map_lvatab[map_nlbln+i]=lva;
		map_lvntab[map_nlbln+i]=bgbcc_strdup(tb);
		map_lvmtab[map_nlbln+i]='L';
	}
	map_nlbln+=sctx->lblllnrov;
	
//	for(i=0; i<sctx->nlbln; i++)
	for(i=0; i<map_nlbln; i++)
	{
//		for(j=i+1; j<sctx->nlbln; j++)
		for(j=i+1; j<map_nlbln; j++)
		{
			if(map_lvatab[j]<map_lvatab[i])
			{
				k=map_lvatab[j];
				map_lvatab[j]=map_lvatab[i];
				map_lvatab[i]=k;

				s0=map_lvntab[j];
				map_lvntab[j]=map_lvntab[i];
				map_lvntab[i]=s0;

				k=map_lvmtab[j];
				map_lvmtab[j]=map_lvmtab[i];
				map_lvmtab[i]=k;
			}
		}
	}

//	for(i=0; i<sctx->nlbln; i++)
	for(i=0; i<map_nlbln; i++)
	{
//		fprintf(mapfd, "%08X T %s\n", map_lvatab[i], map_lvntab[i]);
		fprintf(mapfd, "%08X %c %s\n",
			map_lvatab[i],
			map_lvmtab[i],
			map_lvntab[i]);
	}

	fclose(mapfd);
#endif

	if(is_pel)
//		csum=BGBCC_JX2C_CalculateImagePel4Checksum(obuf, ofs_iend2, en);
		csum=BGBCC_JX2C_CalculateImagePel4BChecksum(obuf, ofs_iend2, en);
	else
		csum=BGBCC_JX2C_CalculateImagePeChecksum(obuf, ofs_iend2, en);

	ct=obuf+of_phdr;
	bgbcc_setu32en(ct+0x58, en, csum);	//mCheckSum
	
	if(sctx->verbose)
	{
		printf("PE Checksum: %08X / %dkB\n", csum, ofs_iend2>>10);
	}

	if(is_pel)
	{
//		ctb=malloc(ofs_iend*2);
		ctb=malloc(ofs_iend2*2);
	
//		j=BGBCC_JX2C_PackImagePEL(ctx, ctb, obuf, ofs_iend*2, ofs_iend);
		j=BGBCC_JX2C_PackImagePEL(ctx, ctb, obuf, ofs_iend2*2, ofs_iend2);
		ctb[2]='L';
//		ctb[3]='4';
		ctb[3]='0'+bgbcc_packlz_cmp;
		
//		BGBCC_JX2C_VerifyImagePEL(ctx, ctb, obuf, j, ofs_iend);
		BGBCC_JX2C_VerifyImagePEL(ctx, ctb, obuf, j, ofs_iend2);
		
//		if(j<ofs_iend)
		if(j<ofs_iend2)
		{
			if(sctx->verbose)
			{
	//			printf("PEL: %d->%d %d%%\n",
				printf("PEL%d: %d->%d %d%%\n", ctx->pel_cmpr,
	//				ofs_iend, j, (j*100)/ofs_iend);
					ofs_iend2, j, (j*100)/ofs_iend2);
			}
		
			memcpy(obuf, ctb, j);
			*rosz=j;
			free(ctb);
			return(0);
		}

		free(ctb);
	}

	*rosz=ofs_iend;

	return(0);
}
