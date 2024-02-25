int BGBCC_JX2_LoadElfRemapReloc(u32 mach, int rlcid)
{
	int rel2;

	switch(rlcid)
	{
	case 1: rel2=BGBCC_SH_RLC_ABS32; break;
	case 2: rel2=BGBCC_SH_RLC_ABS64; break;
	case 3: rel2=0; break;
	case 4: rel2=0; break;
	case 5: rel2=0; break;
	case 6: rel2=0; break;
	case 7: rel2=0; break;
	case 8: rel2=0; break;
	case 9: rel2=0; break;
	case 10: rel2=0; break;
	case 11: rel2=0; break;

	case 16: rel2=BGBCC_SH_RLC_RELW12_RVI; break;
	case 17: rel2=BGBCC_SH_RLC_RELW20_RVI; break;

	case 19: rel2=BGBCC_SH_RLC_REL32UI_RVI; break;

	case 23: rel2=BGBCC_SH_RLC_RELHI20_RVI; break;
	case 24: rel2=BGBCC_SH_RLC_RELLO12I_RVI; break;
	case 25: rel2=BGBCC_SH_RLC_RELLO12S_RVI; break;

	case 26: rel2=BGBCC_SH_RLC_ABSHI20_RVI; break;
	case 27: rel2=BGBCC_SH_RLC_ABSLO12I_RVI; break;
	case 28: rel2=BGBCC_SH_RLC_ABSLO12S_RVI; break;

	case 51: rel2=0; break;

	default: rel2=-1; break;
	}
	return(rel2);
}

int BGBCC_JX2_LoadElfFixupAddend(
	byte *dest, int rlc, int addend)
{
	s64 d, s, a;

	if(!addend)
		return(0);
	if(rlc<=0)
		return(0);

	switch(rlc)
	{
	case BGBCC_SH_RLC_ABS32:
		d=bgbcc_getu32le(dest);
		d=d+addend;
		bgbcc_setu32le(dest, d);
		break;
	case BGBCC_SH_RLC_ABS64:
		d=bgbcc_getu64le(dest);
		d=d+addend;
		bgbcc_setu64le(dest, d);
		break;
	default:
		BGBCC_DBGBREAK
		break;
	}

	return(0);
}

BGBCC_COFF_Info *BGBCC_JX2_LoadElfObj(BGBCC_JX2_Context *ctx,
	byte *data, int szdata)
{
	int					isec_reloc[8];
	int					n_isec_reloc;
	BGBCC_COFF_Info		*obj;
	bgbcc_elf64_hdr		*hdr;
	bgbcc_elf64_shdr	*shdr, *shdr_str, *shdr_sym;
	bgbcc_elf64_sym		*sym;
	bgbcc_elf64_rela	*rela;
	byte				*strtab, *symstrtab;
	char				*s0, *s1;
	byte				*symtab;
	byte				*reldat;
	int					shoff, shnum, shentsize, shstrndx;
	int					shsymndx, shsymstrndx;
	int					strtab_ofs, symsz, symcnt;
	int					relsz, relcnt, tsec, nrlc, mrlc, mach;
	int		i, j, k;

	obj=bgbcc_malloc(sizeof(BGBCC_COFF_Info));
	obj->buf=bgbcc_malloc(szdata);
	obj->bsz=szdata;
	memcpy(obj->buf, data, szdata);

	hdr=(bgbcc_elf64_hdr *)data;
	
	shoff=bgbcc_getu32le(hdr->e_shoff);
	shnum=bgbcc_getu16le(hdr->e_shnum);
	shentsize=bgbcc_getu16le(hdr->e_shentsize);
	shstrndx=bgbcc_getu16le(hdr->e_shstrndx);
	mach=BGBCC_ARCH_RV64;
	
	shdr_str=(bgbcc_elf64_shdr *)(data+shoff+(shstrndx*shentsize));
	strtab_ofs=bgbcc_getu32le(shdr_str->sh_offset);
	
	strtab=data+strtab_ofs;
	shsymndx=-1;
	shsymstrndx=-1;
	n_isec_reloc=0;
	symstrtab=NULL;
	
	for(i=1; i<shnum; i++)
	{
		shdr=(bgbcc_elf64_shdr *)(data+shoff+(i*shentsize));
		
		j=bgbcc_getu32le(shdr->sh_name);
		s0=(char *)(strtab+j);
		
		printf("%02X %s\n", i, s0);
		
		obj->sec_name[i]=bgbcc_strdup(s0);
		obj->sec_buf_off[i]=bgbcc_getu32le(shdr->sh_offset);
		obj->sec_buf_sz[i]=bgbcc_getu32le(shdr->sh_sz);

		obj->sec_rva_off[i]=bgbcc_getu32le(shdr->sh_addr);
		obj->sec_rva_sz[i]=bgbcc_getu32le(shdr->sh_sz);
		
		if(!strcmp(s0, ".symtab"))
		{
			shsymndx=i;
		}
		if(!strcmp(s0, ".strtab"))
		{
			shsymstrndx=i;
			j=bgbcc_getu32le(shdr->sh_offset);
			symstrtab=data+j;
		}

		if(!strncmp(s0, ".rela.", 6))
		{
			j=n_isec_reloc++;
			isec_reloc[j]=i;
		}

		if(
			!strcmp(s0, ".comment")		||
			!strcmp(s0, ".debug")		||
			!strcmp(s0, ".dynamic")		||
			!strcmp(s0, ".hash")		||
			!strcmp(s0, ".line")		||
			!strcmp(s0, ".note")		||
			!strcmp(s0, ".strtab")		||
			!strcmp(s0, ".shstrtab")	||
			!strcmp(s0, ".symtab")
			)
		{
			obj->sec_name[i]=NULL;
		}
	}

	obj->nsecs=shnum;

	shdr_sym=(bgbcc_elf64_shdr *)(data+shoff+(shsymndx*shentsize));
	j=bgbcc_getu32le(shdr_sym->sh_offset);
	symtab=data+j;
	
	symsz=bgbcc_getu32le(shdr_sym->sh_entsize);
	symcnt=bgbcc_getu32le(shdr_sym->sh_sz)/symsz;

	obj->sym_name=bgbcc_malloc(symcnt*sizeof(char *));
	obj->sym_offs=bgbcc_malloc(symcnt*sizeof(int));
	obj->sym_sec=bgbcc_malloc(symcnt*sizeof(int));
	obj->sym_stype=bgbcc_malloc(symcnt);

	for(i=1; i<symcnt; i++)
	{
		sym=(bgbcc_elf64_sym *)(symtab+(i*symsz));

		j=bgbcc_getu32le(sym->st_name);
		if(!j)
			continue;
		
//		s0=strtab+j;
		s0=(char *)(symstrtab+j);
		
		printf("%s\n", s0);
		
		obj->sym_name[i]=bgbcc_strdup((char *)s0);
		obj->sym_offs[i]=bgbcc_getu32le(sym->st_addr);
		obj->sym_sec[i]=bgbcc_getu16le(sym->st_shndx);
	}
	obj->nsyms=symcnt;

//	nrlc=symcnt*4;

	mrlc=symcnt;
	for(i=0; i<n_isec_reloc; i++)
	{
		j=isec_reloc[i];
		shdr=(bgbcc_elf64_shdr *)(data+shoff+(j*shentsize));
		k=bgbcc_getu32le(shdr->sh_offset);
		reldat=data+k;
		
		relsz=bgbcc_getu32le(shdr->sh_entsize);
		relcnt=bgbcc_getu32le(shdr->sh_sz)/relsz;
		tsec=bgbcc_getu16le(shdr->sh_link);
		
		mrlc+=relcnt;
	}

	obj->rlc_offs=bgbcc_malloc(mrlc*sizeof(int));
	obj->rlc_sym=bgbcc_malloc(mrlc*sizeof(int));
	obj->rlc_addend=bgbcc_malloc(mrlc*sizeof(int));
	obj->rlc_sec=bgbcc_malloc(mrlc);
	obj->rlc_type=bgbcc_malloc(mrlc);
	
	nrlc=0;
	for(i=0; i<n_isec_reloc; i++)
	{
		j=isec_reloc[i];
		shdr=(bgbcc_elf64_shdr *)(data+shoff+(j*shentsize));
		k=bgbcc_getu32le(shdr->sh_offset);
		reldat=data+k;
		
		relsz=bgbcc_getu32le(shdr->sh_entsize);
		relcnt=bgbcc_getu32le(shdr->sh_sz)/relsz;
//		tsec=bgbcc_getu16le(shdr->sh_link);
		tsec=bgbcc_getu16le(shdr->sh_info);
		
		for(j=0; j<relcnt; j++)
		{
			rela=(bgbcc_elf64_rela *)(reldat+(j*relsz));
			
			k=bgbcc_getu32le(rela->r_info);
			obj->rlc_offs[nrlc]=bgbcc_getu32le(rela->r_offset);
			obj->rlc_addend[nrlc]=bgbcc_getu32le(rela->r_addend);
			obj->rlc_sec[nrlc]=tsec;
			obj->rlc_sym[nrlc]=k>>8;
			obj->rlc_type[nrlc]=BGBCC_JX2_LoadElfRemapReloc(mach, k&255);
			
			BGBCC_JX2_LoadElfFixupAddend(
				obj->buf+
					obj->sec_buf_off[tsec]+
					obj->rlc_offs[nrlc],
				obj->rlc_type[nrlc],
				obj->rlc_addend[nrlc]);
			nrlc++;
		}
	}
	
	obj->nrlc=nrlc;

	
	return(obj);
}

int BGBCC_JX2_EmitLoadedObject(BGBCC_JX2_Context *ctx,
	BGBCC_COFF_Info *obj)
{
	byte *dat;
	char *s0;
	int i, j, k, ko, ofs, sz;
	
	for(i=0; i<obj->nsecs; i++)
	{
		s0=obj->sec_name[i];
		if(!s0)
			continue;

		BGBCC_JX2_SetSectionName(ctx, s0);
		BGBCC_JX2_EmitBAlign(ctx, 16);
		ofs=BGBCC_JX2_EmitGetOffs(ctx);
		
		dat=obj->buf+obj->sec_buf_off[i];
		sz=obj->sec_buf_sz[i];
		
		BGBCC_JX2_EmitRawBytes(ctx, dat, sz);
		
		for(j=0; j<obj->nsyms; j++)
		{
			s0=obj->sym_name[j];
			if(!s0)
				continue;
			if(obj->sym_sec[j]!=i)
				continue;
			ko=obj->sym_offs[j];
			BGBCC_JX2_EmitNamedLabelOffs(ctx, s0, ofs+ko);
		}

		for(j=0; j<obj->nrlc; j++)
		{
			if(obj->rlc_sec[j]!=i)
				continue;
			k=obj->rlc_sym[j];
			ko=obj->rlc_offs[j];

			s0=obj->sym_name[k];
			if(!s0)
				continue;
			k=BGBCC_JX2_GetNamedLabel(ctx, s0);

			BGBCC_JX2_EmitRelocTyOffs(ctx, k, obj->rlc_type[j],
				ofs+ko);
		}
	}
	return(0);
}

int BGBCC_JX2_BuildLoadObj(BGBCC_JX2_Context *ctx,
	byte *data, int szdata)
{
	BGBCC_COFF_Info		*obj;

	if(!memcmp(data, "\x7F" "ELF", 4))
	{
		obj=BGBCC_JX2_LoadElfObj(ctx, data, szdata);
		BGBCC_JX2_EmitLoadedObject(ctx, obj);
	}

	return(-1);		
}

