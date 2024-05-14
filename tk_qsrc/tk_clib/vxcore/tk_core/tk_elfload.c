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

TKPE_ImageInfo *TKPE_LoadDynELF(TK_FILE *fd, int fdoffs,
	char *imgname, char *cwd, int is_dll)
{
//	byte tbuf[1024];
	byte tbuf[1024+32];
	TKPE_ImageInfo *img, *idll;
	byte *imgptr, *ct, *cte, *bss_ptr;

	u64 imgbase, imgbase1;
	s64 reloc_disp;
	btsh2_elf64_hdr *ehdr;
	struct btsh2_elf64_phdr_s *phdr;
	u64 entry, phoff, shoff;
	u64 paddr, pmsz, poff, mach;
	byte en, isriscv, isbjx2, isxg2, isxg2rv;
	int phentsz, phnum, imgsz, imgsz1;
	int i, j, k;

	if(is_dll&3)
		return(NULL);

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
	if(btsh2_ptrGetUW(ehdr->e_type, 1)==2)
		en=1;
	
	if(btsh2_ptrGetUW(ehdr->e_type, en)!=2)
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
		if(btsh2_ptrGetUD(phdr->p_type, en)!=1)
		{
			continue;
		}

		paddr=btsh2_ptrGetUD(phdr->p_paddr, en);
		pmsz=btsh2_ptrGetUD(phdr->p_memsz, en);
		poff=btsh2_ptrGetUD(phdr->p_offset, en);
		
		if((paddr+pmsz)>imgsz)
			imgsz=(paddr+pmsz);
	}

//	imgsz1=(imgsz+16383)&(~16383);
	imgsz1=(imgsz+16384+16383)&(~16383);

//	imgptr=TKMM_PageAlloc(imgsz1);
	imgptr=TKMM_PageAllocVaMap(imgsz1, TKMM_PROT_RWX,
		TKMM_MAP_SHARED|TKMM_MAP_32BIT|TKMM_MAP_DIRECT);

//	memset(imgptr, 0, imgsz1-32);
//	memset(imgptr, 0, imgsz1);

	TK_VMem_MProtectPages((u64)imgptr, imgsz1,
		TKMM_PROT_READ|TKMM_PROT_WRITE|
		TKMM_PROT_EXEC);

//	imgptr+=i;

	imgbase=imgptr;

	img->imgbase=imgptr;
	img->imgname=TKMM_LVA_Strdup(imgname);

	imgbase1=(u64)imgptr;
	printf("TKPE!LDA:%s=%04X_%08X\n", imgname,
		(u16)(imgbase1>>32), (u32)imgbase1);

	entry+=(u64)imgptr;


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
		tk_fread(imgptr+paddr, 1, pmsz, fd);

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

	if(!(entry&1))
	{
		tk_printf("TKPE_LoadDynELF: ISA Mode Not Set, Mach=%04X\n", mach);
		return(NULL);
	}

	tk_printf("TKPE_LoadDynELF: Entry=%016llX\n", entry);

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

	return(img);
}

