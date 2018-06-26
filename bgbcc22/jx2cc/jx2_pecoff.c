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
	int peofs, expofs, expsz;
	int nexps, rvaexpaddr;
	int i, j, k;

	csend=buf+sz;

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

	cspe=buf+peofs;

	if((cspe[0x00]!='P') || (cspe[0x01]!='E'))
	{
		printf("BGBCC_JX2C_CoffLoadBufferDLL: PE Sig Fail\n");
		return(-1);
	}
	i=bgbcc_getu16en(cspe+0x18, 0);
	if(i!=0x010B)
	{
		printf("BGBCC_JX2C_CoffLoadBufferDLL: Opt Header Magic Fail\n");
		return(-1);
	}

	expofs=bgbcc_getu32en(cspe+0x78, 0);
	expsz=bgbcc_getu32en(cspe+0x7C, 0);

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
			if(sctx->is_addr64)
				BGBCC_JX2_EmitDWord(sctx, 0);
		}
		BGBCC_JX2_EmitDWord(sctx, 0);
		if(sctx->is_addr64)
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
			if(sctx->is_addr64)
				BGBCC_JX2_EmitDWord(sctx, 0);
		}
		BGBCC_JX2_EmitDWord(sctx, 0);
		if(sctx->is_addr64)
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
		return(BGBCC_COFF_SCNT_IDATA|
			BGBCC_COFF_SCNT_READ|
			BGBCC_COFF_SCNT_WRITE);
	}
	if(!strcmp(name, ".bss") ||
		!strcmp(name, ".sbss"))
	{
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

ccxl_status BGBCC_JX2C_FlattenImagePECOFF(BGBCC_TransState *ctx,
	byte *obuf, int *rosz, fourcc imgfmt)
{
//	int rlc_rva_page[4096];
//	int rlc_sz_page[4096];
	u32 rlctab[65536];

	BGBCC_JX2_Context *sctx;
	FILE *mapfd;
	char *s0;
	byte *ct, *ct0, *ct1, *ctb;
	int en, ofs, ofs_sdat, ofs_iend, ofs_mend;
	int of_phdr, ne_phdr;
	int of_shdr, ne_shdr;
	int lb_strt, va_strt;
	int img_base, img_base_hi;
	int nm, fl, lva, rva, lsz, sn_strs, imty;
	int lpg, szrlc, ofsrlc, nrlce, mach;
	int ofsimp, szimp, ofsexp, szexp;
	int i, j, k;

	sctx=ctx->uctx;

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
		j=sctx->sec_pos[i]-sctx->sec_buf[i];
		sctx->sec_rva[i]=k;
		sctx->sec_lva[i]=img_base+k;
		sctx->sec_lsz[i]=j;
		k+=j;
		k=(k+63)&(~63);
	}

	lpg=-1; szrlc=0; nrlce=0;
	for(i=0; i<sctx->nrlc; i++)
	{
		if(	(sctx->rlc_ty[i]!=BGBCC_SH_RLC_ABS16) &&
			(sctx->rlc_ty[i]!=BGBCC_SH_RLC_ABS32) &&
			(sctx->rlc_ty[i]!=BGBCC_SH_RLC_ABS64))
				continue;
			
		j=sctx->rlc_sec[i];
		rva=sctx->sec_rva[j]+
			sctx->rlc_ofs[i];

		lva=rva&0x0FFFFFFF;
		if(sctx->rlc_ty[i]==BGBCC_SH_RLC_ABS64)
			lva|=0x10000000;
		if(sctx->rlc_ty[i]==BGBCC_SH_RLC_ABS16)
			lva|=0x20000000;

		rlctab[nrlce++]=lva;

		if((rva>>12)!=lpg)
		{
			szrlc=(szrlc+3)&(~3);
			szrlc+=8;
			lpg=rva>>12;
		}
		szrlc+=2;
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

	if(sctx->is_addr64)
		of_shdr=0x0148;

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
	
//	memset(ct, 0, 0x400);
	ct[0x00]='M';	ct[0x01]='Z';	ct[0x02]=0;		ct[0x03]=0;
	ct[0x3C]=0x40;	ct[0x3D]=0;		ct[0x3E]=0;		ct[0x3F]=0;
	ct[0x40]='P';	ct[0x41]='E';	ct[0x42]=0;		ct[0x43]=0;

	k=sctx->is_le?0x0080:0x8000;
	k|=0x0302;

//	mach=0x1A6;
//	if(sctx->has_bjx1mov)
//		mach=0xB132;
	if(sctx->is_addr64)
	{
//		mach=0xB164;
//		if(sctx->is_betav)
//			mach=0xB64C;
		mach=0xB264;
	}else
	{
		mach=0xB232;
	}

//	bgbcc_setu16en(ct+0x44, en, 0x14C);		//mMachine
//	bgbcc_setu16en(ct+0x44, en, 0x1A6);		//mMachine
	bgbcc_setu16en(ct+0x44, en, mach);		//mMachine
	bgbcc_setu16en(ct+0x46, en, ne_shdr);	//mNumberOfSections
	bgbcc_setu32en(ct+0x48, en, 0);			//mTimeDateStamp
	bgbcc_setu32en(ct+0x4C, en, 0);			//mPointerToSymbolTable
	bgbcc_setu32en(ct+0x50, en, 0);			//mNumberOfSymbols
//	bgbcc_setu16en(ct+0x54, en, 224);		//mSizeOfOptionalHeader
//	bgbcc_setu16en(ct+0x54, en, 144);		//mSizeOfOptionalHeader
	bgbcc_setu16en(ct+0x56, en, k);			//mCharacteristics

	if(!sctx->is_addr64)
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
			{ strncpy(ct+0, s0, 8); }
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
	mapfd=fopen("aout.map", "wt");
	for(i=0; i<sctx->nlbln; i++)
	{
//		if(!strcmp(ctx->lbln_name[i], name))
//			return(ctx->lbln_id[i]);

		lva=BGBCC_JX2C_LookupLabelImgVA(ctx, sctx, sctx->lbln_id[i]);
		fprintf(mapfd, "%08X T %s\n", lva, sctx->lbln_name[i]);
	}
	fclose(mapfd);
#endif

	*rosz=ofs_iend;

	return(0);
}
