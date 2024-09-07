typedef byte tk_elf32_half[2];
typedef byte tk_elf32_word[4];
typedef byte tk_elf32_addr[4];
typedef byte tk_elf32_off[4];

typedef byte tk_elf64_half[2];
typedef byte tk_elf64_word[4];
typedef byte tk_elf64_xword[8];
typedef byte tk_elf64_addr[8];
typedef byte tk_elf64_off[8];

typedef struct btsh2_elf32_hdr_s btsh2_elf32_hdr;
typedef struct btsh2_elf64_hdr_s btsh2_elf64_hdr;

struct btsh2_elf32_hdr_s
{
byte			e_ident[16];
tk_elf32_half	e_type;		//filt type
tk_elf32_half	e_machine;	//target machine
tk_elf32_word	e_version;	//file version
tk_elf32_addr	e_entry;	/* Entry point */
tk_elf32_off	e_phoff;	//program head table's file offset 
tk_elf32_off	e_shoff;	//section head table's file offset
tk_elf32_word	e_flags;	//
tk_elf32_half	e_ehsize;	//elf head's size
tk_elf32_half	e_phentsize;//the size of the entry in the program head table
tk_elf32_half	e_phnum;	//the number of the entry in the program head tabl	e
tk_elf32_half	e_shentsize;//the size of the entry in the program head table
tk_elf32_half	e_shnum;	//the number of the entry in the section head table
tk_elf32_half	e_shstrndx;	//the section head table index of the entry associate with the section name string table
};

struct btsh2_elf64_hdr_s
{
byte			e_ident[16];	//00
tk_elf64_half	e_type;		//10, filt type
tk_elf64_half	e_machine;	//12, target machine
tk_elf64_word	e_version;	//14, file version
tk_elf64_addr	e_entry;	//18, Entry point
tk_elf64_off	e_phoff;	//20, program head table's file offset 
tk_elf64_off	e_shoff;	//28, section head table's file offset
tk_elf64_word	e_flags;	//30
tk_elf64_half	e_ehsize;	//34, elf head's size
tk_elf64_half	e_phentsize;//36, the size of the entry in the program head table
tk_elf64_half	e_phnum;	//38, the number of the entry in the program head table
tk_elf64_half	e_shentsize;//the size of the entry in the program head table
tk_elf64_half	e_shnum;	//the number of the entry in the section head table
tk_elf64_half	e_shstrndx;	//the section head table index of the entry associate with the section name string table
};

struct btsh2_elf32_phdr_s
{
tk_elf32_word	p_type;		
tk_elf32_off	p_offset;
tk_elf32_addr	p_vaddr;
tk_elf32_addr	p_paddr;
tk_elf32_word	p_filesz;
tk_elf32_word	p_memsz;
tk_elf32_word	p_flags;
tk_elf32_word	p_align;
};

struct btsh2_elf64_phdr_s
{
tk_elf64_word	p_type;		
tk_elf64_word	p_flags;
tk_elf64_off	p_offset;
tk_elf64_addr	p_vaddr;
tk_elf64_addr	p_paddr;
tk_elf64_xword	p_filesz;
tk_elf64_xword	p_memsz;
tk_elf64_xword	p_align;
};

#if 1
int btsh2_ptrGetSW(byte *ptr, byte en)
{
	if(en)
		return((s16)((ptr[1]<<8)|(ptr[0])));
	return((s16)((ptr[0]<<8)|(ptr[1])));
}

int btsh2_ptrGetUW(byte *ptr, byte en)
{
	if(en)
		return((u16)((ptr[1]<<8)|(ptr[0])));
	return((u16)((ptr[0]<<8)|(ptr[1])));
}

s32 btsh2_ptrGetSD(byte *ptr, byte en)
{
	if(en)
		return((s32)((ptr[3]<<24)|(ptr[2]<<16)|(ptr[1]<<8)|(ptr[0])));
	return((s32)((ptr[0]<<24)|(ptr[1]<<16)|(ptr[2]<<8)|(ptr[3])));
}

u32 btsh2_ptrGetUD(byte *ptr, byte en)
{
	if(en)
		return((u32)((ptr[3]<<24)|(ptr[2]<<16)|(ptr[1]<<8)|(ptr[0])));
	return((u32)((ptr[0]<<24)|(ptr[1]<<16)|(ptr[2]<<8)|(ptr[3])));
}

s64 btsh2_ptrGetSQ(byte *ptr, byte en)
{
	u64 v0, v1, v;
	if(en)
	{
		v0=btsh2_ptrGetUD(ptr+0, en);
		v1=btsh2_ptrGetUD(ptr+4, en);
		v=(v1<<32)|v0;
		return(v);
	}
	v0=btsh2_ptrGetUD(ptr+4, en);
	v1=btsh2_ptrGetUD(ptr+0, en);
	v=(v1<<32)|v0;
	return(v);
}

u64 btsh2_ptrGetUQ(byte *ptr, byte en)
{
	u64 v0, v1, v;
	if(en)
	{
		v0=btsh2_ptrGetUD(ptr+0, en);
		v1=btsh2_ptrGetUD(ptr+4, en);
		v=(v1<<32)|v0;
		return(v);
	}
	v0=btsh2_ptrGetUD(ptr+4, en);
	v1=btsh2_ptrGetUD(ptr+0, en);
	v=(v1<<32)|v0;
	return(v);
}
#endif

int TKPE_LoadStaticELF(TK_FILE *fd, void **rbootptr, void **rbootgbr)
{
//	byte tbuf[1024];
	byte tbuf[1024+32];
	byte *imgptr, *ct, *cte, *bss_ptr;

	u64 imgbase;
	s64 reloc_disp;
	btsh2_elf64_hdr *ehdr;
	struct btsh2_elf64_phdr_s *phdr;
	u64 entry, phoff, shoff;
	u32 paddr, pmsz, poff, mach;
	byte en, isriscv, isbjx2, isxg2, isxg2rv;
	int phentsz, phnum;
	int i, j, k;

	
	tk_fread(tbuf, 1, 1024, fd);

//	is_pel4=0;
//	cmp=4;

	if((tbuf[0]!=0x7F) || (tbuf[1]!='E') ||
		(tbuf[2]!='L') || (tbuf[3]!='F'))
	{
		printf("No ELF sig\n");
		return(-1);
	}
	
	ehdr=(btsh2_elf64_hdr *)tbuf;
	en=0;
	if(btsh2_ptrGetUW(ehdr->e_type, 1)==2)
		en=1;
	
	if(btsh2_ptrGetUW(ehdr->e_type, en)!=2)
	{
		printf("Not Exec %d\n",
			btsh2_ptrGetUW(ehdr->e_type, en));
		return(-1);
	}
	
	isriscv=0;
	isbjx2=0;
	isxg2=0;
	isxg2rv=0;
	
	entry=btsh2_ptrGetUD(ehdr->e_entry, en);
	phoff=btsh2_ptrGetUD(ehdr->e_phoff, en);
//	shoff=btsh2_ptrGetUD(ehdr->e_eshoff, en);
	
	phentsz=btsh2_ptrGetUW(ehdr->e_phentsize, en);
	phnum=btsh2_ptrGetUW(ehdr->e_phnum, en);

	mach=btsh2_ptrGetUW(ehdr->e_machine, en);
	if(mach==0x243)
		isriscv=1;
	if(mach==0xB264)
		isbjx2=1;
	if(mach==0xB265)
		isxg2=1;
	if(mach==0xB250)
		isxg2rv=1;

	for(i=0; i<phnum; i++)
	{
		phdr=(struct btsh2_elf64_phdr_s *)(tbuf+(phoff+(i*phentsz)));
		if(btsh2_ptrGetUD(phdr->p_type, en)!=1)
		{
			continue;
		}

		paddr=btsh2_ptrGetUD(phdr->p_paddr, en);
		pmsz=btsh2_ptrGetUD(phdr->p_memsz, en);
		poff=btsh2_ptrGetUD(phdr->p_offset, en);
		
		printf("%08X -> %08X %08X\n", poff, paddr, pmsz);

#if 0
		if((poff+pmsz)>szibuf)
		{
			printf("BTESH2_BootLoadElf: Copy exeeds image by %d bytes\n",
				(poff+pmsz)-szibuf);
			printf("Truncating: %d -> %d\n", pmsz, szibuf-poff);
			pmsz=szibuf-poff;
		}
#endif

		tk_fseek(fd, poff, 0);
		tk_fread((void *)paddr, 1, pmsz, fd);

//		j=BTESH2_MemCpyIn(cpu, paddr&0x1FFFFFFF, ibuf+poff, pmsz);
//		j=BTESH2_MemCpyIn(cpu, paddr, ibuf+poff, pmsz);
//		if(j<0)
//		{
//			printf("BTESH2_BootLoadElf: MemCpy Failed\n");
//		}
	}
	
	if(isbjx2)
	{
//		entry|=1;
		entry|=0x0000000000000001ULL;
	}

	if(isriscv)
	{
//		entry|=1;
		entry|=0x0004000000000001ULL;
	}

	if(isxg2)
	{
		entry|=0x0088000000000001ULL;
	}

	if(isxg2rv)
	{
		entry|=0x008C000000000001ULL;
	}

	*rbootptr=(void *)entry;
	*rbootgbr=NULL;

	return(0);
}

void *TKPE_LookupImageElfExport(TKPE_ImageInfo *img, char *name)
{
	byte *imgbase;
	byte *symtab_ptr;
	byte *strtab_ptr;
	byte *sym, *ptr;
	char *sn;
	int sym_ent, rva_symtab, rva_strtab;
	int sym_name, sym_value, sym_shndx;
	int i, j, k;
	
	imgbase=img->imgbase;
	rva_symtab=img->rva_symtab;
	rva_strtab=img->rva_strtab;
	sym_ent=img->sz_syment;
	symtab_ptr=imgbase+rva_symtab;
	strtab_ptr=imgbase+rva_strtab;

	tk_dbg_printf("TKPE_LookupImageElfExport: "
		"RVA_Sym=%X RVA_Str=%X EntSz=%d\n",
		rva_symtab, rva_strtab, sym_ent);

	if(!rva_symtab || !rva_strtab)
	{
		return(NULL);
	}

//	k=rel_info>>32;

	for(i=1;; i++)
	{
		sym=symtab_ptr+i*sym_ent;
		sym_name=btsh2_ptrGetUD(sym+0, 1);
		sym_value=btsh2_ptrGetUQ(sym+8, 1);
		sym_shndx=btsh2_ptrGetUW(sym+6, 1);
		
		if(!sym_name && !sym_shndx && !sym_value)
		{
			tk_dbg_printf("TKPE_LookupImageElfExport: End of syms %d\n", i);
			break;
		}

		if(!sym_shndx)
			continue;
		
		if((sym_name<0) || (sym_name>0x10000))
		{
			continue;
		}

		sn=strtab_ptr+sym_name;

//		tk_dbg_printf("TKPE_LookupImageElfExport: Check %s\n", sn);

		if(!strcmp(sn, name))
		{
			ptr=img->imgbase+sym_value;
			tk_dbg_printf("TKPE_LookupImageElfExport: Found %s %p\n", sn, ptr);
			return(ptr);
		}
	}
	
	tk_dbg_printf("TKPE_LookupImageElfExport: %s not found in %s\n",
		name, img->imgname);
	return(NULL);
}

TKPE_ImageInfo *TKPE_LoadDynELF(TK_FILE *fd, int fdoffs,
	char *imgname, char *cwd, int is_dll)
{
//	byte tbuf[1024];
	byte tbuf[1024+32];
	char *a_needed[64];
	TKPE_ImageInfo *img_needed[64];
	TKPE_ImageInfo *img, *idll;
	byte *imgptr, *ct, *cte, *bss_ptr;
	byte *dyn_ptr, *rela_ptr, *strtab_ptr, *symtab_ptr;
	u64 *rel_where;
	byte *ptr;
	int n_needed;

	u64 imgbase, imgbase1;
	s64 reloc_disp;
	btsh2_elf64_hdr *ehdr;
	struct btsh2_elf64_phdr_s *phdr;
	u64 entry, phoff, shoff;
	u64 paddr, pmsz, pfsz, poff, mach;
	s64 rel_info, rel_addend, rel_org;
	byte en, isriscv, isbjx2, isxg2, isxg2rv, isdyn;
	int phentsz, phnum, phty, imgsz, imgsz1;
	int rela_offs, rela_sz, rela_ent;
	int pltrel_offs, pltrel_sz, pltrel_ent;
	int sym_offs, sym_sz, sym_ent, strs_ofs;
	int sym_name, sym_shndx;
	s64 sym_value;
	int pboix;
	int i, j, k, l;

//	if(is_dll&3)
//		return(NULL);

	tk_dbg_printf("TKPE_LoadDynELF: Begin Load %s\n", imgname);

	tk_fseek(fd, fdoffs, 0);
	tk_fread(tbuf, 1, 1024, fd);

//	is_pel4=0;
//	cmp=4;

	if(	(tbuf[0]!=0x7F)	|| (tbuf[1]!='E')	||
		(tbuf[2]!='L')	|| (tbuf[3]!='F')	)
	{
		printf("Not ELF sig\n");
		return(NULL);
	}

	img=tk_malloc_krn(sizeof(TKPE_ImageInfo));
	memset(img, 0, sizeof(TKPE_ImageInfo));

	ehdr=(btsh2_elf64_hdr *)tbuf;
	en=0;
//	if(btsh2_ptrGetUW(ehdr->e_type, 1)==2)
	if(btsh2_ptrGetUW(ehdr->e_type, 1)==(btsh2_ptrGetUW(ehdr->e_type, 1)&255))
		en=1;
	
	phty=btsh2_ptrGetUW(ehdr->e_type, en);
	if((phty!=2) && (phty!=3))
	{
		printf("Not Exec %d\n",
			btsh2_ptrGetUW(ehdr->e_type, en));
		return(NULL);
	}
	
	isriscv=0;
	isbjx2=0;
	isxg2=0;
	isxg2rv=0;
	imgbase=0;
	isdyn=0;
	
	entry=btsh2_ptrGetUD(ehdr->e_entry, en);
	phoff=btsh2_ptrGetUD(ehdr->e_phoff, en);
//	shoff=btsh2_ptrGetUD(ehdr->e_eshoff, en);
	
	phentsz=btsh2_ptrGetUW(ehdr->e_phentsize, en);
	phnum=btsh2_ptrGetUW(ehdr->e_phnum, en);

	mach=btsh2_ptrGetUW(ehdr->e_machine, en);
//	if(mach==0x243)
	if(mach==0x00F3)
		isriscv=1;
	else if(mach==0xB264)
		isbjx2=1;
	else if(mach==0xB265)
		isxg2=1;
	else if(mach==0xB250)
		isxg2rv=1;
	else
	{
		tk_printf("TKPE_LoadDynELF: Unhandled Machine Mach=%04X\n", mach);
		return(NULL);
	}

	imgsz=0;
	for(i=0; i<phnum; i++)
	{
		phdr=(struct btsh2_elf64_phdr_s *)(tbuf+(phoff+(i*phentsz)));
		phty=btsh2_ptrGetUD(phdr->p_type, en);
		if(phty!=1)
		{
			if(phty==2)
				isdyn=1;
			continue;
		}

		paddr=btsh2_ptrGetUD(phdr->p_paddr, en);
		pmsz=btsh2_ptrGetUD(phdr->p_memsz, en);
		poff=btsh2_ptrGetUD(phdr->p_offset, en);
		
		if((paddr+pmsz)>imgsz)
			imgsz=(paddr+pmsz);
	}

	if(!isdyn)
	{
		tk_printf("TKPE_LoadDynELF: No DYNAMIC entry, can't load\n", mach);
		return(NULL);
	}

//	imgsz1=(imgsz+16383)&(~16383);
	imgsz1=(imgsz+16384+16383)&(~16383);

//	imgptr=TKMM_PageAlloc(imgsz1);

//	imgptr=TKMM_PageAllocVaMap(imgsz1, TKMM_PROT_RWX,
//		TKMM_MAP_SHARED|TKMM_MAP_32BIT|TKMM_MAP_DIRECT);
	imgptr=tk_mmap(0, imgsz1, TKMM_PROT_RWX,
		TKMM_MAP_SHARED|TKMM_MAP_32BIT|TKMM_MAP_DIRECT, -1, 0);
//	imgptr=NULL;

	if(!imgptr)
	{
		imgptr=TKMM_PageAlloc(imgsz1);
	}

//	memset(imgptr, 0, imgsz1-32);
	memset(imgptr, 0, imgsz1);

	if(imgptr)
	{
		TK_VMem_MProtectPages((u64)imgptr, imgsz1,
			TKMM_PROT_READ|TKMM_PROT_WRITE|
			TKMM_PROT_EXEC);
	}

	if(!imgptr)
	{
		imgptr=TKMM_PageAlloc(imgsz1);
	}

//	imgptr+=i;

	imgbase=imgptr;

	img->imgbase=imgptr;
	img->imgname=TKMM_LVA_Strdup(imgname);
	img->iself=1;

	imgbase1=(u64)imgptr;
	tk_dbg_printf("TKPE!LDA:%s=%04X_%08X\n", imgname,
		(u16)(imgbase1>>32), (u32)imgbase1);

//	entry+=(u64)imgptr;
	entry+=imgbase;

	dyn_ptr=NULL;

	for(i=0; i<phnum; i++)
	{
		phdr=(struct btsh2_elf64_phdr_s *)(tbuf+(phoff+(i*phentsz)));
		phty=btsh2_ptrGetUD(phdr->p_type, en);
		if(phty!=1)
		{
			paddr=btsh2_ptrGetUD(phdr->p_paddr, en);
			pmsz=btsh2_ptrGetUD(phdr->p_memsz, en);
			poff=btsh2_ptrGetUD(phdr->p_offset, en);
			
			if(phty==2)
			{
				printf("DYN: %08X -> %08X %08X\n",
					poff, paddr, pmsz, phty);

				dyn_ptr=imgptr+paddr;
				continue;
			}
			
//			printf("SKIP: %08X -> %08X %08X  TY=%08X\n", poff, paddr, pmsz,
//				phty);

			continue;
		}

		paddr=btsh2_ptrGetUD(phdr->p_paddr, en);
		pfsz=btsh2_ptrGetUD(phdr->p_filesz, en);
		pmsz=btsh2_ptrGetUD(phdr->p_memsz, en);
		poff=btsh2_ptrGetUD(phdr->p_offset, en);
		
		printf("%08X -> %08X %08X\n", poff, paddr, pmsz);

#if 0
		if((poff+pmsz)>szibuf)
		{
			printf("BTESH2_BootLoadElf: Copy exeeds image by %d bytes\n",
				(poff+pmsz)-szibuf);
			printf("Truncating: %d -> %d\n", pmsz, szibuf-poff);
			pmsz=szibuf-poff;
		}
#endif

		if(pmsz>pfsz)
			memset(imgptr+paddr+pfsz, 0, pmsz-pfsz);

		tk_fseek(fd, poff, 0);
//		tk_fread(imgptr+paddr, 1, pmsz, fd);
		tk_fread(imgptr+paddr, 1, pfsz, fd);

//		j=BTESH2_MemCpyIn(cpu, paddr&0x1FFFFFFF, ibuf+poff, pmsz);
//		j=BTESH2_MemCpyIn(cpu, paddr, ibuf+poff, pmsz);
//		if(j<0)
//		{
//			printf("BTESH2_BootLoadElf: MemCpy Failed\n");
//		}
	}
	
	n_needed=0;
	if(dyn_ptr)
	{
		rela_offs=0;
		rela_sz=0;
		rela_ent=0;

		pltrel_offs=0;
		pltrel_ent=0;
		sym_offs=0;
		strs_ofs=0;
		sym_ent=0;

		for(i=0;; i++)
		{
			phty=btsh2_ptrGetUQ(dyn_ptr+i*16+0, en);
			paddr=btsh2_ptrGetUQ(dyn_ptr+i*16+8, en);
			if(!phty)
				break;
//			printf("BTESH2_BootLoadElf: Tag=%08X = %08X\n", phty, paddr);

			if(phty==1)
			{	
//				pltrel_sz=paddr;
			}

			if(phty==2)
				{ pltrel_sz=paddr; }

			if(phty==5)
				{ strs_ofs=paddr; }
			if(phty==6)
				{ sym_offs=paddr; }
			if(phty==7)
				{ rela_offs=paddr; }
			if(phty==8)
				{ rela_sz=paddr; }
			if(phty==9)
				{ rela_ent=paddr; }

			if(phty==11)
				{ sym_ent=paddr; }

			if(phty==20)
				{ pltrel_ent=paddr; }
			if(phty==23)
				{ pltrel_offs=paddr; }
		}

		strtab_ptr=imgptr+strs_ofs;
		symtab_ptr=imgptr+sym_offs;

		tk_dbg_printf("TKPE_LoadDynELF: RVA_Sym=%X RVA_str=%x SymEnt=%d\n",
			sym_offs, strs_ofs, sym_ent);

		img->rva_symtab=sym_offs;
		img->rva_strtab=strs_ofs;
		img->sz_syment=sym_ent;

		tk_dbg_printf("TKPE_LoadDynELF: RVA_Sym=%X RVA_str=%x SymEnt=%d\n",
			img->rva_symtab, img->rva_strtab, img->sz_syment);

		for(i=0;; i++)
		{
			phty=btsh2_ptrGetUQ(dyn_ptr+i*16+0, en);
			paddr=btsh2_ptrGetUQ(dyn_ptr+i*16+8, en);
			if(!phty)
				break;
//			printf("BTESH2_BootLoadElf: Tag=%08X = %08X\n", phty, paddr);

			if(phty==1)
			{
				j=n_needed++;
				a_needed[j]=strtab_ptr+paddr;
				k=TKPE_TryLoadProgramImage(a_needed[j], cwd, 1);
				if(k>0)
				{
					img_needed[j]=TK_GetImageForIndex(k);
					tk_printf("BTESH2_BootLoadElf: Got %s ix=%d p=%p\n",
						a_needed[j], k, img_needed[j]);
				}else
				{
					tk_printf("BTESH2_BootLoadElf: Failed SO %s\n",
						a_needed[j]);
					img_needed[j]=NULL;
				}
			}
		}

//		if(pltrel_ent==7)
			pltrel_ent=rela_ent;

		if(rela_offs && (rela_ent>0))
		{
			rela_ptr=imgptr+rela_offs;
			l=rela_sz/rela_ent;
			for(i=0; i<l; i++)
			{
				paddr=btsh2_ptrGetUQ(rela_ptr+i*rela_ent+0, en);
				rel_info=btsh2_ptrGetUQ(rela_ptr+i*rela_ent+8, en);
				rel_addend=btsh2_ptrGetUQ(rela_ptr+i*rela_ent+16, en);
				
				rel_where=(u64 *)(imgptr+paddr);
	
				if((paddr>imgsz) || (paddr&7))
				{
					tk_printf("BTESH2_BootLoadElf: Reloc: "
						"Address image, VA=%016llX\n",
						paddr);
				}

				k=rel_info>>32;

				sym_name=btsh2_ptrGetUD(symtab_ptr+k*sym_ent+0, en);
				sym_value=btsh2_ptrGetUQ(symtab_ptr+k*sym_ent+8, en);
				sym_shndx=btsh2_ptrGetUW(symtab_ptr+k*sym_ent+6, en);

//				if(sym_shndx==0)
				if((sym_shndx==0) && sym_name)
				{
					ptr=NULL;
					for(j=0; j<n_needed; j++)
					{
						if(!img_needed[j])
							continue;
						ptr=TKPE_LookupImageDllExport(img_needed[j],
							strtab_ptr+sym_name);
						if(ptr)
							break;
					}
					
					if(ptr)
					{
						sym_value=((s64)ptr)-imgbase;

//						__debugbreak();

						tk_printf("BTESH2_BootLoadElf: "
							"Got Sym n=%s a=%p imb=%p rel=%p\n",
							strtab_ptr+sym_name, ptr,
							imgbase, sym_value);
					}else
					{
						tk_printf("BTESH2_BootLoadElf: Fail Sym %s\n",
							strtab_ptr+sym_name);
					}
				}else
				{
					if(sym_value>imgsz)
					{
						tk_printf("BTESH2_BootLoadElf: Reloc: "
							"Symbol outside image, VA=%016llX\n",
							sym_value);
					}
				}

//				tk_printf("BTESH2_BootLoadElf: Reloc: "
//					"addr=%08llX info=%08llX addend=%08llX\n",
//					paddr, rel_info, rel_addend);

				switch(rel_info&255)
				{
				case 0:
					break;
				case 3:		//R_RISCV_RELATIVE
//					tk_printf("BTESH2_BootLoadElf: Reloc: "
//						"addr=%08llX info=%016llX addend=%08llX\n",
//						paddr, rel_info, rel_addend);

					rel_org=*rel_where;

#if 0
					if(rel_org)
					{
						*rel_where=imgbase+rel_org;

						tk_printf("BTESH2_BootLoadElf: Reloc RELATIVE-A: "
							"%012llX -> %012llX\n",
							rel_org, *rel_where);
						break;
					}
#endif

					*rel_where=imgbase+rel_addend;
					
					if((*rel_where<imgbase) || (*rel_where>(imgbase+imgsz)))
					{
						tk_printf("BTESH2_BootLoadElf: Reloc RELATIVE-B: "
							"%012llX -> %012llX\n",
							rel_org, *rel_where);
					}
					break;

				case 2:		//R_RISCV_64
					rel_org=*rel_where;

					*rel_where=imgbase+sym_value+rel_addend;
//					*rel_where=imgbase+sym_value;

					if((*rel_where<imgbase) || (*rel_where>(imgbase+imgsz)))
					{
						tk_printf("BTESH2_BootLoadElf: Reloc R_RISCV_64: "
							"%012llX -> %012llX\n",
							rel_org, *rel_where);
					}
					break;

				case 5:		//R_RISCV_JUMP_SLOT
					rel_org=*rel_where;

					*rel_where=imgbase+sym_value;
//					*rel_where=imgbase+sym_value+rel_addend;

					if((*rel_where<imgbase) || (*rel_where>(imgbase+imgsz)))
					{
						tk_printf("BTESH2_BootLoadElf: Reloc R_RISCV_JUMP_SLOT: "
							"%012llX -> %012llX\n",
							rel_org, *rel_where);
					}
					break;

				case 10:		//R_RISCV_TPREL32
					rel_org=*(u32 *)rel_where;

					*(u32 *)rel_where=offsetof(TKPE_TaskInfo, img_elftls)+
						sym_value+rel_addend;
//					*rel_where=imgbase+sym_value;

					if((*rel_where<imgbase) || (*rel_where>(imgbase+imgsz)))
					{
						tk_printf("BTESH2_BootLoadElf: Reloc R_RISCV_64: "
							"%012llX -> %012llX\n",
							rel_org, *rel_where);
					}
					break;

				case 11:		//R_RISCV_TPREL64
					rel_org=*rel_where;

					*rel_where=offsetof(TKPE_TaskInfo, img_elftls)+
						sym_value+rel_addend;
//					*rel_where=imgbase+sym_value;

					if((*rel_where<imgbase) || (*rel_where>(imgbase+imgsz)))
					{
						tk_printf("BTESH2_BootLoadElf: Reloc R_RISCV_64: "
							"%012llX -> %012llX\n",
							rel_org, *rel_where);
					}
					break;

				default:
					tk_printf("BTESH2_BootLoadElf: Reloc %02X: Miss\n",
						rel_info&255);
					break;
				}
			}
		}

		
		if(pltrel_offs && (pltrel_ent>0))
		{
			rela_ptr=imgptr+pltrel_offs;
			l=pltrel_sz/pltrel_ent;
			for(i=0; i<l; i++)
			{
				paddr=btsh2_ptrGetUQ(rela_ptr+i*pltrel_ent+0, en);
				rel_info=btsh2_ptrGetUQ(rela_ptr+i*pltrel_ent+8, en);
				rel_addend=btsh2_ptrGetUQ(rela_ptr+i*pltrel_ent+16, en);
				
				rel_where=(u64 *)(imgptr+paddr);
	
				if((paddr>imgsz) || (paddr&7))
				{
					tk_dbg_printf("BTESH2_BootLoadElf: Reloc: "
						"Address image, VA=%016llX\n",
						paddr);
				}

				k=rel_info>>32;

				sym_name=btsh2_ptrGetUD(symtab_ptr+k*sym_ent+0, en);
				sym_value=btsh2_ptrGetUQ(symtab_ptr+k*sym_ent+8, en);
				sym_shndx=btsh2_ptrGetUW(symtab_ptr+k*sym_ent+6, en);

//				if(sym_shndx==0)
				if((sym_shndx==0) && sym_name)
				{
					ptr=NULL;
					for(j=0; j<n_needed; j++)
					{
						if(!img_needed[j])
							continue;
						ptr=TKPE_LookupImageDllExport(img_needed[j],
							strtab_ptr+sym_name);
						if(ptr)
							break;
					}
					
					if(ptr)
					{
						sym_value=((s64)ptr)-imgbase;

//						__debugbreak();

						tk_printf("BTESH2_BootLoadElf: "
							"Got Sym n=%s a=%p imb=%p rel=%p\n",
							strtab_ptr+sym_name, ptr,
							imgbase, sym_value);
					}else
					{
						tk_printf("BTESH2_BootLoadElf: Fail Sym %s\n",
							strtab_ptr+sym_name);
					}
				}else
				{
					if(sym_value>imgsz)
					{
						tk_printf("BTESH2_BootLoadElf: Reloc: "
							"Symbol outside image, VA=%016llX\n",
							sym_value);
					}
				}

				if(sym_value>imgsz)
				{
					tk_dbg_printf("BTESH2_BootLoadElf: Reloc: "
						"Symbol outside image, VA=%016llX\n",
						sym_value);
				}

//				tk_printf("BTESH2_BootLoadElf: Reloc: "
//					"addr=%08llX info=%08llX addend=%08llX\n",
//					paddr, rel_info, rel_addend);

				switch(rel_info&255)
				{
				case 0:
					break;
				case 3:		//R_RISCV_RELATIVE
//					tk_printf("BTESH2_BootLoadElf: Reloc: "
//						"addr=%08llX info=%016llX addend=%08llX\n",
//						paddr, rel_info, rel_addend);

					rel_org=*rel_where;
					*rel_where=imgbase+rel_addend;
					
					if((*rel_where<imgbase) || (*rel_where>(imgbase+imgsz)))
					{
						tk_printf("BTESH2_BootLoadElf: Reloc RELATIVE-B: "
							"%012llX -> %012llX\n",
							rel_org, *rel_where);
					}
					break;

				case 2:		//R_RISCV_64
					rel_org=*rel_where;

					*rel_where=imgbase+sym_value+rel_addend;
//					*rel_where=imgbase+sym_value;

					if((*rel_where<imgbase) || (*rel_where>(imgbase+imgsz)))
					{
						tk_printf("BTESH2_BootLoadElf: Reloc R_RISCV_64: "
							"%012llX -> %012llX\n",
							rel_org, *rel_where);
					}
					break;

				case 5:		//R_RISCV_JUMP_SLOT
					rel_org=*rel_where;

					*rel_where=imgbase+sym_value;
//					*rel_where=imgbase+sym_value+rel_addend;

					if((*rel_where<imgbase) || (*rel_where>(imgbase+imgsz)))
					{
						tk_printf("BTESH2_BootLoadElf: Reloc R_RISCV_JUMP_SLOT: "
							"%012llX -> %012llX\n",
							rel_org, *rel_where);
					}
					break;

				default:
					tk_printf("BTESH2_BootLoadElf: Reloc %02X: Miss\n",
						rel_info&255);
					break;
				}
			}
		}
	}

	TK_FlushCacheL1D();
	TK_FlushCacheL1D_INVIC(NULL);

	if(isbjx2)
	{
//		entry|=1;
		entry|=0x0000000000000001ULL;
	}

	if(isriscv)
	{
//		entry|=1;
		entry|=0x0004000000000001ULL;
	}

	if(isxg2)
	{
		entry|=0x0088000000000001ULL;
	}

	if(isxg2rv)
	{
		entry|=0x008C000000000001ULL;
	}

	if(!(entry&1))
	{
		tk_dbg_printf("TKPE_LoadDynELF: ISA Mode Not Set, Mach=%04X\n", mach);
		return(NULL);
	}

	tk_dbg_printf("TKPE_LoadDynELF: Entry=%016X\n", entry);

//	*rbootptr=(void *)entry;
//	*rbootgbr=NULL;

//	img->bootptr=imgptr+startrva;
	img->bootptr=(void *)entry;
//	img->bootgbr=imgptr+gbr_rva;
//	img->bootgbre=imgptr+imgsz;
	
	img->rlc_imgbase=imgbase;
//	img->rlc_basedisp=rlc_disp;
//	img->ptr_reloc=imgptr+rva_rlc;
//	img->sz_reloc=sz_rlc;

//	img->gbr_rva=gbr_rva;

//	szcpy=imgsz-gbr_rva;
//	if(szcpy>gbr_sz)
//		szcpy=gbr_sz;
//	img->gbr_szcpy=szcpy;
//	img->gbr_sz=gbr_sz;

#if 1
	if(1)
	{
		img->isdll=is_dll;

		i=tkpe_nimgix++;
		if(!i)
			i=tkpe_nimgix++;
		tkpe_pbo_image[i]=img;
		img->imgix=i;

		*(u32 *)(imgptr+0x0C)=i;

		if(is_dll&1)
		{
			pboix=tkpe_npboix++;
			tkpe_pbo_dllimg[pboix]=img;
			img->pboix=pboix;
		}
	}
#endif

	tk_dbg_printf("TKPE_LoadDynELF: Done Load %s\n", imgname);

	return(img);
}

