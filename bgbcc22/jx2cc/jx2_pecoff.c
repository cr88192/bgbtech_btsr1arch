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
	if(!strcmp(name, ".text"))
	{
		return(BGBCC_COFF_SCNT_CODE|
			BGBCC_COFF_SCNT_EXECUTE|
			BGBCC_COFF_SCNT_READ);
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


// #define BGBCC_PEL4_HASHSZ		256
// #define BGBCC_PEL4_HASHSZ		4096
#define BGBCC_PEL4_HASHSZ		16384
#define BGBCC_PEL4_HASHMASK		(BGBCC_PEL4_HASHSZ-1)

//#define BGBCC_PEL4_HASHDSZ		16
#define BGBCC_PEL4_HASHDSZ		32
#define BGBCC_PEL4_HASHDMA		(BGBCC_PEL4_HASHDSZ-1)

#define BGBCC_PEL4_HASH_PR		65521
// #define BGBCC_PEL4_HASH_SHR		16
#define BGBCC_PEL4_HASH_SHR		18

// byte *bgbcc_packlz4_hash[256*16];
// byte bgbcc_packlz4_hrov[256];

byte *bgbcc_packlz4_hash[BGBCC_PEL4_HASHSZ*BGBCC_PEL4_HASHDSZ];
byte bgbcc_packlz4_hrov[BGBCC_PEL4_HASHSZ];

int BGBCC_JX2C_PackBlockLZ4_Reset(BGBCC_TransState *ctx)
{
	int i, j, k;
	
	for(i=0; i<BGBCC_PEL4_HASHSZ; i++)
	{
		bgbcc_packlz4_hrov[i]=0;
		for(j=0; j<BGBCC_PEL4_HASHDSZ; j++)
		{
			bgbcc_packlz4_hash[i*BGBCC_PEL4_HASHDSZ+j]=NULL;
		}
	}
	return(0);
}

int BGBCC_JX2C_PackBlockLZ4_LookupMatch(
	BGBCC_TransState *ctx,
	byte *cs, byte *cse, int *rbl, int *rbd)
{
	byte *ct, *ct1, *ct1e, *cs1, *cs1e;
	u32 h;
	int hr, bd, bl;
	int i, j, k, l, d;
	
	h=*(u32 *)cs;
//	h=(h*65521)+1;
//	h=(h*251)+1;
//	h=(h>>16)&BGBCC_PEL4_HASHMASK;
	h=((h*BGBCC_PEL4_HASH_PR)>>BGBCC_PEL4_HASH_SHR)&BGBCC_PEL4_HASHMASK;
	
	hr=bgbcc_packlz4_hrov[h];

//	return(0);

//	cs1e=cs+268;
//	cs1e=cs+266;
//	cs1e=cs+4096;
	cs1e=cs+16384;
	if(cse<cs1e)
		cs1e=cse;
	
	bd=0; bl=0;
	ct1e=cs;
	for(i=0; i<BGBCC_PEL4_HASHDSZ; i++)
	{
		ct=bgbcc_packlz4_hash[
			(h*BGBCC_PEL4_HASHDSZ)+
			((hr+i)&BGBCC_PEL4_HASHDMA)];
		if(!ct)
			continue;

		d=cs-ct;
		if(d>=65536)
			continue;
		if(d<=0)
			continue;
			
		cs1=cs; ct1=ct;
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
	return(bl>=4);
}

int BGBCC_JX2C_PackBlockLZ4_HashByte(
	BGBCC_TransState *ctx, byte *cs)
{
	u32 h;
	int hr, bd, bl;
	int i, j, k, l, d;
	
	h=*(u32 *)cs;
//	h=(h*65521)+1;
//	h=(h*251)+1;
//	h=(h>>16)&BGBCC_PEL4_HASHMASK;
	h=((h*BGBCC_PEL4_HASH_PR)>>BGBCC_PEL4_HASH_SHR)&BGBCC_PEL4_HASHMASK;
	
	hr=bgbcc_packlz4_hrov[h];
	hr=(hr-1)&BGBCC_PEL4_HASHDMA;
	bgbcc_packlz4_hrov[h]=hr;
	bgbcc_packlz4_hash[(h*BGBCC_PEL4_HASHDSZ)+hr]=cs;
	return(0);
}

int BGBCC_JX2C_PackBlockLZ4_HashString(
	BGBCC_TransState *ctx, byte *cs, int sz)
{
	int i;
	
	for(i=0; i<sz; i++)
		BGBCC_JX2C_PackBlockLZ4_HashByte(ctx, cs+i);
	return(0);
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
		i0=BGBCC_JX2C_PackBlockLZ4_LookupMatch(ctx, cs, cse, &bl, &bd);
		if(i0>0)
		{
			i1=BGBCC_JX2C_PackBlockLZ4_LookupMatch(ctx, cs+1, cse, &bl1, &bd1);
			if(bl1>(bl+1))
				i0=0;
		}
		
//		if((ct+9)>=cte)
//			break;

//		rl=cs-cs0;
//		if((ct+rl+5)>=cte)
//			break;

		rl=cs-cs0;
		if((ct+rl+9)>=cte)
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

				while(i>=255)
//				if(i>=255)
					{ *ct++=0xFF; i-=255; }
				if(i>=255)
					{ BGBCC_DBGBREAK }
				*ct++=i;
			}

			if(ct>=cte)
				{ BGBCC_DBGBREAK }
			
			BGBCC_JX2C_PackBlockLZ4_HashString(ctx, cs, bl);
			cs+=bl;

			cs0=cs;
			continue;
		}else
		{
			BGBCC_JX2C_PackBlockLZ4_HashByte(ctx, cs);
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

int BGBCC_JX2C_PackImagePEL4(BGBCC_TransState *ctx,
	byte *obuf, byte *ibuf, int obsz, int ibsz)
{
	byte *cs, *ct, *cse, *cte;
	int i;
	
	BGBCC_JX2C_PackBlockLZ4_Reset(ctx);
	
	ct=obuf; cs=ibuf;
	cte=obuf+obsz;
	cse=ibuf+ibsz;
	
	memcpy(ct, cs, 1024);
	ct+=1024; cs+=1024;
	
	while(cs<cse)
	{
		i=BGBCC_JX2C_PackBlockLZ4(ctx, ct, cs, 1024, cse-cs);
		ct+=1024;
		cs+=i;
	}
	
	return(ct-obuf);
}

byte bgbcc_tkpe_endseen;

byte *BGBCC_TKPE_UnpackL4(byte *obuf, byte *ibuf, int isz)
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
		
//		ld=tkfat_getWord(cs);
		ld=*(u16 *)cs;
		cs+=2;
		if(!ld)
		{
			bgbcc_tkpe_endseen=1;
			break;
		}

		ll=(tg&15)+4;
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
				{ *(u64 *)ct1=*(u64 *)cs1; ct1+=8; cs1+=8; }
			ct+=ll;
		}else
		{
			while(cs1<cs1e)
				{ *ct++=*cs1++; }
		}
	}
	
	return(ct);
}

int BGBCC_JX2C_VerifyImagePEL4(BGBCC_TransState *ctx,
	byte *obuf, byte *ibuf, int obsz, int ibsz)
{
	byte *tbuf;
	byte *cs, *ct, *cse;
	int cb, nb;
	int i, j, k;
	
	tbuf=malloc(ibsz+65536);
	
	bgbcc_tkpe_endseen=0;
	cs=obuf; cse=obuf+obsz;
	ct=tbuf;
	
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
		ct=BGBCC_TKPE_UnpackL4(ct, cs, 1024);
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

ccxl_status BGBCC_JX2C_FlattenImagePECOFF(BGBCC_TransState *ctx,
	byte *obuf, int *rosz, fourcc imgfmt)
{
//	int rlc_rva_page[4096];
//	int rlc_sz_page[4096];
	u32 rlctab[65536];
	char tb[256];

	s32 map_lvatab[16384];
	char *map_lvntab[16384];


	BGBCC_JX2_Context *sctx;
	FILE *mapfd;
	char *s0;
	byte *ct, *ct0, *ct1, *ctb;
	byte no_mz, is_pel4;
	int en, ofs, ofs_sdat, ofs_iend, ofs_mend;
	int of_phdr, ne_phdr;
	int of_shdr, ne_shdr;
	int lb_strt, va_strt, lbl_gptr;
	int img_base, img_base_hi;
	int nm, fl, lva, rva, lsz, sn_strs, imty;
	int lpg, szrlc, ofsrlc, nrlce, mach, lbl;
	int ofsimp, szimp, ofsexp, szexp;
	int i, j, k;

	sctx=ctx->uctx;

//	is_pel4=0;
	is_pel4=1;

	no_mz=0;
	if(is_pel4)
		no_mz=1;
	

	BGBCC_JX2C_CoffBuildExports(ctx, sctx);
	BGBCC_JX2C_CoffBuildImports(ctx, sctx);

//	img_base=0x0C000000;
	img_base=0x01100000;
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
			(sctx->rlc_ty[i]!=BGBCC_SH_RLC_PBO24_BJX) &&
			(sctx->rlc_ty[i]!=BGBCC_SH_RLC_PBO32_BJX) &&
			(sctx->rlc_ty[i]!=BGBCC_SH_RLC_TBR24_BJX))
				continue;
			
		j=sctx->rlc_sec[i];
		rva=sctx->sec_rva[j]+
			sctx->rlc_ofs[i];
		lbl=sctx->rlc_id[i];

		lva=rva&0x0FFFFFFF;
		if(sctx->rlc_ty[i]==BGBCC_SH_RLC_ABS64)
			lva|=0x10000000;
		if(sctx->rlc_ty[i]==BGBCC_SH_RLC_ABS16)
			lva|=0x20000000;

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
	
#if 0
	for(i=0; i<nrlce; i++)
		for(j=i+1; j<nrlce; j++)
	{
		if(rlctab[j]<rlctab[i])
		{
			k=rlctab[i];
			rlctab[i]=rlctab[j];
			rlctab[j]=k;
		}
	}
#endif

	bgbcc_jx2c_qrsort(rlctab, nrlce, 0);

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
			default: s0=".unknown"; break;
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
		sctx->sec_rva[i]=k;
		sctx->sec_lva[i]=img_base+k;
		sctx->sec_lsz[i]=j;
		memcpy(obuf+k, sctx->sec_buf[i], j);
		k+=j;
		k=(k+63)&(~63);

		s0=sctx->sec_name[i];
		printf("%d: %s %08X..%08X %d\n", i, s0,
			sctx->sec_lva[i], sctx->sec_lva[i]+sctx->sec_lsz[i],
			sctx->sec_lsz[i]);

	}

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
			sctx->sec_rva[i]=k;
			sctx->sec_lva[i]=img_base+k;
			sctx->sec_lsz[i]=j;
			memcpy(obuf+k, sctx->sec_buf[i], j);
			k+=j;
			k=(k+63)&(~63);

			s0=sctx->sec_name[i];
			printf("%d: %s %08X..%08X %d\n", i, s0,
				sctx->sec_lva[i], sctx->sec_lva[i]+sctx->sec_lsz[i],
				sctx->sec_lsz[i]);

		}
	}
	
	ofs_iend=k;

	i=BGBCC_SH_CSEG_BSS;
	j=sctx->sec_pos[i]-sctx->sec_buf[i];
	sctx->sec_rva[i]=k;
//	sctx->sec_lva[i]=0x0C000000+k;
	sctx->sec_lva[i]=img_base+k;
	sctx->sec_lsz[i]=j;
	k+=j;
	k=(k+63)&(~63);

	s0=sctx->sec_name[i];
	printf("%d: %s %08X..%08X %d\n", i, s0,
		sctx->sec_lva[i], sctx->sec_lva[i]+sctx->sec_lsz[i],
		sctx->sec_lsz[i]);

	ofs_mend=k;
	
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
		printf("BGBCC_JX2C_FlattenImageELF: No Entry Point Found\n");
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
		rva=k&0x0FFFFFFF;

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
		switch(k>>28)
		{
		case 0:		j=0x3000|(rva&0xFFF);	break;
		case 1:		j=0xA000|(rva&0xFFF);	break;
		case 2:		j=0x2000|(rva&0xFFF);	break;

		case 3:		j=0x6000|(rva&0xFFF);	break;

		case 4:		j=0x8000|(rva&0xFFF);	break;
		case 5:		j=0x7000|(rva&0xFFF);	break;
		case 6:		j=0x9000|(rva&0xFFF);	break;
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
	}else
	{
//		memset(ct, 0, 0x400);
		ct[0x00]='M';	ct[0x01]='Z';	ct[0x02]=0;		ct[0x03]=0;
		ct[0x3C]=0x40;	ct[0x3D]=0;		ct[0x3E]=0;		ct[0x3F]=0;
		ct[0x40]='P';	ct[0x41]='E';	ct[0x42]=0;		ct[0x43]=0;
		ct+=0x40;
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


		bgbcc_setu32en(ct+0x50, en, ofs_mend);	//mSizeOfImage
		bgbcc_setu32en(ct+0x54, en, ofs_sdat);	//mSizeOfHeaders

		bgbcc_setu16en(ct+0x5C, en, 1);			//mSubsystem
		bgbcc_setu16en(ct+0x5E, en, 0x0140);	//mDllCharacteristics

		bgbcc_setu32en(ct+0x60, en, 0x100000);	//mSizeOfStackReserve
		bgbcc_setu32en(ct+0x64, en, 0x10000);	//mSizeOfStackCommit
		bgbcc_setu32en(ct+0x68, en, 0);			//mSizeOfHeapReserve
		bgbcc_setu32en(ct+0x6C, en, 0);			//mSizeOfHeapCommit
		bgbcc_setu32en(ct+0x70, en, 0);			//mLoaderFlags
		bgbcc_setu32en(ct+0x74, en, 6);			//mNumberOfRvaAndSizes

		bgbcc_setu32en(ct+0x78, en, ofsexp);	//rvaExportTable
		bgbcc_setu32en(ct+0x7C, en, szexp);		//szExportTable
		bgbcc_setu32en(ct+0x80, en, ofsimp);	//rvaImportTable
		bgbcc_setu32en(ct+0x84, en, szimp);		//szImportTable

		bgbcc_setu32en(ct+0xA0, en, ofsrlc);	//rvaBaseRelocTable
		bgbcc_setu32en(ct+0xA4, en, szrlc);		//szBaseRelocTable

		j=sctx->is_pbo?sctx->gbr_rva:0;
		bgbcc_setu32en(ct+0xB8, en, j);			//RVA of GBR Base
		j=sctx->is_pbo?(ofs_mend-sctx->gbr_rva):0;
		bgbcc_setu32en(ct+0xBC, en, j);			//Size of PBO Region
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


		bgbcc_setu32en(ct+0x50, en, ofs_mend);	//mSizeOfImage
		bgbcc_setu32en(ct+0x54, en, ofs_sdat);	//mSizeOfHeaders

		bgbcc_setu16en(ct+0x5C, en, 1);			//mSubsystem
		bgbcc_setu16en(ct+0x5E, en, 0x0140);	//mDllCharacteristics

		bgbcc_setu64en(ct+0x60, en, 0x100000);	//mSizeOfStackReserve
		bgbcc_setu64en(ct+0x68, en, 0x10000);	//mSizeOfStackCommit
		bgbcc_setu64en(ct+0x70, en, 0);			//mSizeOfHeapReserve
		bgbcc_setu64en(ct+0x78, en, 0);			//mSizeOfHeapCommit

		bgbcc_setu32en(ct+0x80, en, 0);			//mLoaderFlags
		bgbcc_setu32en(ct+0x84, en, 6);			//mNumberOfRvaAndSizes
		
		bgbcc_setu32en(ct+0x88, en, ofsexp);	//rvaExportTable
		bgbcc_setu32en(ct+0x8C, en, szexp);		//szExportTable
		bgbcc_setu32en(ct+0x90, en, ofsimp);	//rvaImportTable
		bgbcc_setu32en(ct+0x94, en, szimp);		//szImportTable

		bgbcc_setu32en(ct+0xB0, en, ofsrlc);	//rvaBaseRelocTable
		bgbcc_setu32en(ct+0xB4, en, szrlc);		//szBaseRelocTable

		j=sctx->is_pbo?sctx->gbr_rva:0;
		bgbcc_setu32en(ct+0xC8, en, j);			//RVA of GBR Base
		j=sctx->is_pbo?(ofs_mend-sctx->gbr_rva):0;
		bgbcc_setu32en(ct+0xCC, en, j);			//Size of PBO Region
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


		bgbcc_setu32en(ct+0x90, en, ofs_mend);	//mSizeOfImage
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


		bgbcc_setu32en(ct+0x90, en, ofs_mend);	//mSizeOfImage
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
	sprintf(tb, "%s.map", ctx->imgname);
	mapfd=fopen(tb, "wt");
//	mapfd=fopen("aout.map", "wt");
	for(i=0; i<sctx->nlbln; i++)
	{
//		if(!strcmp(ctx->lbln_name[i], name))
//			return(ctx->lbln_id[i]);

		lva=BGBCC_JX2C_LookupLabelImgVA(ctx, sctx, sctx->lbln_id[i]);
//		fprintf(mapfd, "%08X T %s\n", lva, sctx->lbln_name[i]);
		map_lvatab[i]=lva;
		map_lvntab[i]=sctx->lbln_name[i];
	}
	
	for(i=0; i<sctx->nlbln; i++)
	{
		for(j=i+1; j<sctx->nlbln; j++)
		{
			if(map_lvatab[j]<map_lvatab[i])
			{
				k=map_lvatab[j];
				map_lvatab[j]=map_lvatab[i];
				map_lvatab[i]=k;

				s0=map_lvntab[j];
				map_lvntab[j]=map_lvntab[i];
				map_lvntab[i]=s0;
			}
		}
	}

	for(i=0; i<sctx->nlbln; i++)
	{
		fprintf(mapfd, "%08X T %s\n", map_lvatab[i], map_lvntab[i]);
	}

	fclose(mapfd);
#endif

	if(is_pel4)
	{
		ctb=malloc(ofs_iend*2);
	
		j=BGBCC_JX2C_PackImagePEL4(ctx, ctb, obuf, ofs_iend*2, ofs_iend);
		ctb[2]='L';
		ctb[3]='4';
		
		BGBCC_JX2C_VerifyImagePEL4(ctx, ctb, obuf, j, ofs_iend);
		
		if(j<ofs_iend)
		{
			printf("PEL4: %d->%d %d%%\n",
				ofs_iend, j, (j*100)/ofs_iend);
		
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
