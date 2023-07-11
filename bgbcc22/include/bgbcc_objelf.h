typedef byte bgbcc_elf32_half[2];
typedef byte bgbcc_elf32_word[4];
typedef byte bgbcc_elf32_addr[4];
typedef byte bgbcc_elf32_off[4];

typedef byte bgbcc_elf64_half[2];
typedef byte bgbcc_elf64_word[4];
typedef byte bgbcc_elf64_xword[8];
typedef byte bgbcc_elf64_addr[8];
typedef byte bgbcc_elf64_off[8];

typedef struct bgbcc_elf32_hdr_s bgbcc_elf32_hdr;
typedef struct bgbcc_elf64_hdr_s bgbcc_elf64_hdr;

typedef struct bgbcc_elf32_phdr_s bgbcc_elf32_phdr;
typedef struct bgbcc_elf64_phdr_s bgbcc_elf64_phdr;

typedef struct bgbcc_elf32_shdr_s bgbcc_elf32_shdr;
typedef struct bgbcc_elf64_shdr_s bgbcc_elf64_shdr;

typedef struct bgbcc_elf32_sym_s bgbcc_elf32_sym;
typedef struct bgbcc_elf64_sym_s bgbcc_elf64_sym;

typedef struct bgbcc_elf32_rel_s bgbcc_elf32_rel;
typedef struct bgbcc_elf64_rel_s bgbcc_elf64_rel;

typedef struct bgbcc_elf32_rela_s bgbcc_elf32_rela;
typedef struct bgbcc_elf64_rela_s bgbcc_elf64_rela;

struct bgbcc_elf32_hdr_s
{
byte				e_ident[16];
bgbcc_elf32_half	e_type;		//filt type
bgbcc_elf32_half	e_machine;	//target machine
bgbcc_elf32_word	e_version;	//file version
bgbcc_elf32_addr	e_entry;	/* Entry point */
bgbcc_elf32_off		e_phoff;	//program head table's file offset 
bgbcc_elf32_off		e_shoff;	//section head table's file offset
bgbcc_elf32_word	e_flags;	//
bgbcc_elf32_half	e_ehsize;	//elf head's size
bgbcc_elf32_half	e_phentsize;//the size of the entry in the program head table
bgbcc_elf32_half	e_phnum;	//the number of the entry in the program head tabl	e
bgbcc_elf32_half	e_shentsize;//the size of the entry in the program head table
bgbcc_elf32_half	e_shnum;	//the number of the entry in the section head table
bgbcc_elf32_half	e_shstrndx;	//the section head table index of the entry associate with the section name string table
};

struct bgbcc_elf64_hdr_s
{
byte			e_ident[16];	//00
bgbcc_elf64_half	e_type;		//10, filt type
bgbcc_elf64_half	e_machine;	//12, target machine
bgbcc_elf64_word	e_version;	//14, file version
bgbcc_elf64_addr	e_entry;	//18, Entry point
bgbcc_elf64_off	e_phoff;	//20, program head table's file offset 
bgbcc_elf64_off	e_shoff;	//28, section head table's file offset
bgbcc_elf64_word	e_flags;	//30
bgbcc_elf64_half	e_ehsize;	//34, elf head's size
bgbcc_elf64_half	e_phentsize;//36, the size of the entry in the program head table
bgbcc_elf64_half	e_phnum;	//38, the number of the entry in the program head table
bgbcc_elf64_half	e_shentsize;//the size of the entry in the program head table
bgbcc_elf64_half	e_shnum;	//the number of the entry in the section head table
bgbcc_elf64_half	e_shstrndx;	//the section head table index of the entry associate with the section name string table
};

struct bgbcc_elf32_phdr_s
{
bgbcc_elf32_word	p_type;		
bgbcc_elf32_off		p_offset;
bgbcc_elf32_addr	p_vaddr;
bgbcc_elf32_addr	p_paddr;
bgbcc_elf32_word	p_filesz;
bgbcc_elf32_word	p_memsz;
bgbcc_elf32_word	p_flags;
bgbcc_elf32_word	p_align;
};

struct bgbcc_elf64_phdr_s
{
bgbcc_elf64_word	p_type;		
bgbcc_elf64_word	p_flags;
bgbcc_elf64_off		p_offset;
bgbcc_elf64_addr	p_vaddr;
bgbcc_elf64_addr	p_paddr;
bgbcc_elf64_xword	p_filesz;
bgbcc_elf64_xword	p_memsz;
bgbcc_elf64_xword	p_align;
};

struct bgbcc_elf32_shdr_s
{
bgbcc_elf32_word	sh_name;		//00
bgbcc_elf32_word	sh_type;		//04
bgbcc_elf32_off		sh_flags;		//08
bgbcc_elf32_addr	sh_addr;		//0C
bgbcc_elf32_off		sh_offset;		//10
bgbcc_elf32_word	sh_sz;			//14
bgbcc_elf32_word	sh_link;		//18
bgbcc_elf32_word	sh_info;		//1C
bgbcc_elf32_word	sh_addralign;	//20
bgbcc_elf32_word	sh_entsize;		//24
};

struct bgbcc_elf64_shdr_s
{
bgbcc_elf64_word	sh_name;		//00
bgbcc_elf64_word	sh_type;		//04
bgbcc_elf64_off		sh_flags;		//08
bgbcc_elf64_addr	sh_addr;		//10
bgbcc_elf64_off		sh_offset;		//18
bgbcc_elf64_xword	sh_sz;			//20
bgbcc_elf64_word	sh_link;		//28
bgbcc_elf64_word	sh_info;		//2C
bgbcc_elf64_xword	sh_addralign;	//30
bgbcc_elf64_xword	sh_entsize;		//38
};

struct bgbcc_elf32_sym_s
{
bgbcc_elf32_word	st_name;		//00
bgbcc_elf32_addr	st_addr;		//00
bgbcc_elf32_word	st_size;		//00
byte				st_info;
byte				st_other;
bgbcc_elf32_half	st_shndx;		//00
};

struct bgbcc_elf64_sym_s
{
bgbcc_elf64_word	st_name;		//00
byte				st_info;		//04
byte				st_other;		//05
bgbcc_elf64_half	st_shndx;		//06
bgbcc_elf64_addr	st_addr;		//08
bgbcc_elf64_xword	st_size;		//10
};

struct bgbcc_elf32_rel_s
{
bgbcc_elf32_addr	r_offset;		//00
bgbcc_elf32_word	r_info;			//04
};

struct bgbcc_elf32_rela_s
{
bgbcc_elf32_addr	r_offset;		//00
bgbcc_elf32_word	r_info;			//04
bgbcc_elf32_word	r_addend;		//08
};


struct bgbcc_elf64_rel_s
{
bgbcc_elf64_addr	r_offset;		//00
bgbcc_elf64_xword	r_info;			//08
};

struct bgbcc_elf64_rela_s
{
bgbcc_elf64_addr	r_offset;		//00
bgbcc_elf64_xword	r_info;			//08
bgbcc_elf64_xword	r_addend;		//10
};
