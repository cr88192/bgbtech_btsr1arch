#include <bgbccc.h>

#define ET_NONE		0	//No file type
#define ET_REL		1	//Relocatable file
#define ET_EXEC		2	//Executable file
#define ET_DYN		3	//Shared object file
#define ET_CORE		4	//Core file
#define ET_LOPROC	0xff00	//Processor-specific
#define ET_HIPROC	0xffff	//Processor-specific

#define EM_NONE		0	//No machine
#define EM_M32		1	//AT&T WE 32100
#define EM_SPARC	2	//SPARC
#define EM_386		3	//Intel Architecture
#define EM_68K		4	//Motorola 68000
#define EM_88K		5	//Motorola 88000
#define EM_860		7	//Intel 80860
#define EM_MIPS		8	//MIPS RS3000 Big-Endian
#define EM_MIPS_RS4_BE	10	//MIPS RS4000 Big-Endian

#define EI_MAG0		0	//File identification
#define EI_MAG1		1	//File identification
#define EI_MAG2		2	//File identification
#define EI_MAG3		3	//File identification
#define EI_CLASS	4	//File class
#define EI_DATA		5	//Data encoding
#define EI_VERSION	6	//File version
#define EI_PAD		7	//Start of padding bytes
#define EI_NIDENT	16	//Size of e_ident[]

#define ELFMAG0		0x7f	//e_ident[EI_MAG0]
#define ELFMAG1		'E'	//e_ident[EI_MAG1]
#define ELFMAG2		'L'	//e_ident[EI_MAG2]
#define ELFMAG3		'F'	//e_ident[EI_MAG3]

#define ELFCLASSNONE	0	//Invalid class
#define ELFCLASS32	1	//32-bit objects
#define ELFCLASS64	2	//64-bit objects

#define ELFDATANONE	0	//Invalid data encoding
#define ELFDATA2LSB	1	//See below
#define ELFDATA2MSB	2	//See below

#define SHN_UNDEF	0x0000
#define SHN_LORESERVE	0xff00
#define SHN_LOPROC	0xff00
#define SHN_HIPROC	0xff1f
#define SHN_ABS		0xfff1
#define SHN_COMMON	0xfff2
#define SHN_HIRESERVE	0xffff

#define SHT_NULL	0
#define SHT_PROGBITS	1
#define SHT_SYMTAB	2
#define SHT_STRTAB	3
#define SHT_RELA	4
#define SHT_HASH	5
#define SHT_DYNAMIC	6
#define SHT_NOTE	7
#define SHT_NOBITS	8
#define SHT_REL		9
#define SHT_SHLIB	10
#define SHT_DYNSYM	11
#define SHT_LOPROC	0x70000000
#define SHT_HIPROC	0x7fffffff
#define SHT_LOUSER	0x80000000
#define SHT_HIUSER	0xffffffff

#define SHF_WRITE	0x1
#define SHF_ALLOC	0x2
#define SHF_EXECINSTR	0x4
#define SHF_MASKPROC	0xf0000000

#define STB_LOCAL	0
#define STB_GLOBAL	1
#define STB_WEAK	2
#define STB_LOPROC	13
#define STB_HIPROC	15

#define STT_NOTYPE	0
#define STT_OBJECT	1
#define STT_FUNC	2
#define STT_SECTION	3
#define STT_FILE	4
#define STT_LOPROC	13
#define STT_HIPROC	15

#define R_386_NONE	0	//none none
#define R_386_32	1	//word32 S+A
#define R_386_PC32	2	//word32 S+A-P


#define ELF32_ST_BIND(i)	((i)>>4)
#define ELF32_ST_TYPE(i)	((i)&0xf)
#define ELF32_ST_INFO(b,t)	(((b)<<4)+((t)&0xf))

#define ELF32_R_SYM(i)		((i)>>8)
#define ELF32_R_TYPE(i)		((unsigned char)(i))
#define ELF32_R_INFO(s,t)	(((s)<<8)+(unsigned char)(t))


typedef struct {
byte a,b,c,d;
}Elf32_Addr, Elf32_Off;

typedef struct {
byte a,b;
}Elf32_Half;

typedef struct {
byte a,b,c,d;
}Elf32_Sword, Elf32_Word;

typedef struct {
unsigned char e_ident[EI_NIDENT];
Elf32_Half e_type;
Elf32_Half e_machine;
Elf32_Word e_version;
Elf32_Addr e_entry;
Elf32_Off e_phoff;
Elf32_Off e_shoff;
Elf32_Word e_flags;
Elf32_Half e_ehsize;
Elf32_Half e_phentsize;
Elf32_Half e_phnum;
Elf32_Half e_shentsize;
Elf32_Half e_shnum;
Elf32_Half e_shstrndx;
}Elf32_Ehdr;

typedef struct {
Elf32_Word sh_name;
Elf32_Word sh_type;
Elf32_Word sh_flags;
Elf32_Addr sh_addr;
Elf32_Off sh_offset;
Elf32_Word sh_size;
Elf32_Word sh_link;
Elf32_Word sh_info;
Elf32_Word sh_addralign;
Elf32_Word sh_entsize;
}Elf32_Shdr;

typedef struct {
Elf32_Word st_name;
Elf32_Addr st_value;
Elf32_Word st_size;
unsigned char st_info;
unsigned char st_other;
Elf32_Half st_shndx;
}Elf32_Sym;

typedef struct {
Elf32_Addr r_offset;
Elf32_Word r_info;
}Elf32_Rel;

typedef struct {
Elf32_Addr r_offset;
Elf32_Word r_info;
Elf32_Sword r_addend;
}Elf32_Rela;


typedef struct {
byte *buf;
int szbuf;

Elf32_Ehdr *hdr;

int secoffs, nsec, shsz;

int shstr_offs;
char *shstrtab;

int stroffs, stridx;
char *strtab;

int symoffs, nsym, symidx;
Elf32_Sym *symtab;

Elf32_Rel *rel[64];
int nrel[64];

}Elf32_Context;

static int elf32_addr(Elf32_Addr val)
{
	return(val.a|(val.b<<8)|(val.c<<16)|(val.d<<24));
}

static int elf32_off(Elf32_Off val)
{
	return(val.a|(val.b<<8)|(val.c<<16)|(val.d<<24));
}

static int elf32_sword(Elf32_Sword val)
{
	return(val.a|(val.b<<8)|(val.c<<16)|(val.d<<24));
}

static int elf32_word(Elf32_Word val)
{
	return(val.a|(val.b<<8)|(val.c<<16)|(val.d<<24));
}

static int elf32_half(Elf32_Half val)
{
	return(val.a|(val.b<<8));
}


static void elf32_dump_ehdr(Elf32_Ehdr *hdr)
{
	printf("e_type\t\t%d\n", elf32_half(hdr->e_type));
	printf("e_machine\t%d\n", elf32_half(hdr->e_machine));
	printf("e_version\t%d\n", elf32_word(hdr->e_version));
	printf("e_entry\t\t%d\n", elf32_addr(hdr->e_entry));
	printf("e_phoff\t\t%d\n", elf32_off(hdr->e_phoff));
	printf("e_shoff\t\t%d\n", elf32_off(hdr->e_shoff));
	printf("e_flags\t\t%d\n", elf32_word(hdr->e_flags));

	printf("e_ehsize\t%d\n", elf32_half(hdr->e_ehsize));
	printf("e_phentsize\t%d\n", elf32_half(hdr->e_phentsize));
	printf("e_phnum\t\t%d\n", elf32_half(hdr->e_phnum));
	printf("e_shentsize\t%d\n", elf32_half(hdr->e_shentsize));
	printf("e_shnum\t\t%d\n", elf32_half(hdr->e_shnum));
	printf("e_shstrndx\t%d\n", elf32_half(hdr->e_shstrndx));
	printf("\n");
}

static void elf32_dump_shdr(Elf32_Shdr *hdr, char *strtab)
{
	char *s;
	int i;

	i=elf32_word(hdr->sh_name);
	printf("sh_name\t\t%d, %s\n", i, strtab+i);

	printf("sh_type\t\t%d\n", elf32_word(hdr->sh_type));
	printf("sh_flags\t%d\n", elf32_word(hdr->sh_flags));
	printf("sh_addr\t\t%d\n", elf32_addr(hdr->sh_addr));
	printf("sh_offset\t%d\n", elf32_off(hdr->sh_offset));
	printf("sh_size\t\t%d\n", elf32_word(hdr->sh_size));
	printf("sh_link\t\t%d\n", elf32_word(hdr->sh_link));
	printf("sh_info\t\t%d\n", elf32_word(hdr->sh_info));
	printf("sh_addralign\t%d\n", elf32_word(hdr->sh_addralign));
	printf("sh_entsize\t%d\n", elf32_word(hdr->sh_entsize));
	printf("\n");
}

static char *elf32_get_section_name(Elf32_Context *ctx, int num)
{
	Elf32_Shdr *shdr;
	char *s;
	int i;

	shdr=(Elf32_Shdr *)(ctx->buf+ctx->secoffs+(num*ctx->shsz));

	i=elf32_word(shdr->sh_name);
	s=ctx->shstrtab+i;
	return(s);
}

static int elf32_lookup_section(Elf32_Context *ctx, char *name)
{
	Elf32_Shdr *shdr;
	char *s;
	int i, j;

	for(i=1; i<ctx->nsec; i++)
	{
		shdr=(Elf32_Shdr *)(ctx->buf+ctx->secoffs+(i*ctx->shsz));

		j=elf32_word(shdr->sh_name);
		s=ctx->shstrtab+j;
		if(!strcmp(s, name))
			return(i);
	}

	return(0);
}

static int elf32_dump_symbol(Elf32_Context *ctx, int num)
{
	Elf32_Sym *sym;
	char *s;
	int i;

//	sym=(Elf32_Sym *)(ctx->buf+ctx->symoffs+num*sizeof(Elf32_Sym));
	sym=&(ctx->symtab[num]);

	i=elf32_word(sym->st_name);
	printf("st_name\t\t%d, %s\n", i, ctx->strtab+i);

	printf("st_value\t%d\n", elf32_addr(sym->st_value));
	printf("st_size\t\t%d\n", elf32_word(sym->st_size));
	printf("st_info\t\t%d\n", sym->st_info);
	printf("st_other\t%d\n", sym->st_other);
	printf("st_shndx\t%d\n", elf32_half(sym->st_shndx));
	printf("\n");
	return(0);
}

static char *elf32_get_symbol_name(Elf32_Context *ctx, int num)
{
	Elf32_Sym *sym;
	char *s;
	int i;

	sym=&(ctx->symtab[num]);
	i=elf32_word(sym->st_name);
	s=ctx->strtab+i;
	return(s);
}

static int elf32_lookup_symbol(Elf32_Context *ctx, char *name)
{
	Elf32_Sym *sym;
	char *s;
	int i, j;

	for(i=1; i<ctx->nsec; i++)
	{
		sym=&(ctx->symtab[i]);

		j=elf32_word(sym->st_name);
		s=ctx->strtab+j;
		if(!strcmp(s, name))
			return(i);
	}

	return(0);
}

static Elf32_Context *elf32_decompose_ctx(byte *buf, int sz)
{
	Elf32_Ehdr *hdr;
	Elf32_Shdr *shdr;
	Elf32_Rel *rel;

	Elf32_Context *ctx;

	void *p, *q;
	FILE *fd;
	char *s;
	int ssec;
	int i, j, k, l;


	ctx=malloc(sizeof(Elf32_Context));
	memset(ctx, 0, sizeof(Elf32_Context));

	ctx->buf=buf;
	ctx->szbuf=sz;

	hdr=(Elf32_Ehdr *)buf;
	elf32_dump_ehdr(hdr);
	ctx->hdr=hdr;

	ctx->secoffs=elf32_off(hdr->e_shoff);
	ctx->shsz=elf32_half(hdr->e_shentsize);
	ctx->nsec=elf32_half(hdr->e_shnum);
	ssec=elf32_half(hdr->e_shstrndx);

	shdr=(Elf32_Shdr *)(buf+ctx->secoffs+(ssec*ctx->shsz));
	ctx->shstr_offs=elf32_off(shdr->sh_offset);
	ctx->shstrtab=(char *)(buf+ctx->shstr_offs);

	for(i=0; i<ctx->nsec; i++)
	{
		shdr=(Elf32_Shdr *)(buf+ctx->secoffs+(i*ctx->shsz));
		j=elf32_word(shdr->sh_type);
		if(j==SHT_REL)
		{
			j=elf32_word(shdr->sh_info);
			k=elf32_off(shdr->sh_offset);
			ctx->rel[j]=(Elf32_Rel *)(ctx->buf+k);
			ctx->nrel[j]=elf32_word(shdr->sh_size)/
				sizeof(Elf32_Rel);
		}
	}

	ctx->stridx=elf32_lookup_section(ctx, ".strtab");
	shdr=(Elf32_Shdr *)(buf+ctx->secoffs+(ctx->stridx*ctx->shsz));
	ctx->stroffs=elf32_off(shdr->sh_offset);
	ctx->strtab=(char *)(buf+ctx->stroffs);

	ctx->symidx=elf32_lookup_section(ctx, ".symtab");
	shdr=(Elf32_Shdr *)(buf+ctx->secoffs+(ctx->symidx*ctx->shsz));
	ctx->symoffs=elf32_off(shdr->sh_offset);
	ctx->nsym=elf32_word(shdr->sh_size)/sizeof(Elf32_Sym);
	ctx->symtab=(Elf32_Sym *)(ctx->buf+ctx->symoffs);

#if 0
	for(i=0; i<ctx->nsym; i++)
	{
		printf("symbol %d:\n", i);
		elf32_dump_symbol(ctx, i);
	}
#endif

#if 0
	for(i=0; i<ctx->nsec; i++)
		if(ctx->rel[i])
	{
		printf("reloc section %d (%s):\n", i,
			elf32_get_section_name(ctx, i));
		for(j=0; j<ctx->nrel[i]; j++)
		{
			rel=&(ctx->rel[i][j]);

			k=elf32_word(rel->r_info);
			printf("%08X %d %d (%s)\n",
				elf32_addr(rel->r_offset),
				k&255,
				k>>8, elf32_get_symbol_name(ctx, k>>8));
		}
		printf("\n");
	}
#endif

	return(ctx);
}

BGBCC_Object *BGBCC_ELF_LoadObjectBuf(char *name, byte *buf, int sz)
{
	BGBCC_Object *ctx;
	Elf32_Context *ectx;

	Elf32_Shdr *shdr;
	Elf32_Sym *sym;
	Elf32_Rel *rel;

	void *p;
	char *s;
	int i, j, k, l, o, n;

	int sho[64], shi[64];
	byte *bt, *bd;
	int szt, szd;

	if((buf[0]!=ELFMAG0) || (buf[1]!=ELFMAG1) ||
		(buf[2]!=ELFMAG2) || (buf[3]!=ELFMAG3) ||
		(buf[4]!=ELFCLASS32) || (buf[5]!=ELFDATA2LSB) ||
		(buf[18]!=EM_386))
	{
		printf("ELF-i386 Sig Fail\n");
		return(NULL);
	}


	ectx=elf32_decompose_ctx(buf, sz);
	if(!ectx)return(NULL);

	szt=0; szd=0;
	for(i=0; i<ectx->nsec; i++)
	{
		shdr=(Elf32_Shdr *)(buf+ectx->secoffs+(i*ectx->shsz));

		j=elf32_word(shdr->sh_addralign);
		if(j>1)szt=(szt+j-1)&(~(j-1));
		if(j>1)szd=(szd+j-1)&(~(j-1));

		j=elf32_word(shdr->sh_type);
		k=elf32_word(shdr->sh_flags);
		l=elf32_word(shdr->sh_size);

		sho[i]=0;
		shi[i]=0;

		if(k&SHF_ALLOC)
		{
			if(k&SHF_EXECINSTR)
			{
				sho[i]=szt;
				shi[i]=1;
				szt+=l;
			}else
			{
				sho[i]=szd;
				shi[i]=2;
				szd+=l;
			}
		}
	}

	szt=(szt+15)&(~15);
	szd=(szd+15)&(~15);

	bt=malloc(szt);
	bd=malloc(szd);

	memset(bt, 0x90, szt);
	memset(bd, 0x00, szd);

	for(i=0; i<ectx->nsec; i++)
	{
		if(!shi[i])continue;

		shdr=(Elf32_Shdr *)(buf+ectx->secoffs+(i*ectx->shsz));

		j=elf32_word(shdr->sh_type);
		k=elf32_off(shdr->sh_offset);
		l=elf32_word(shdr->sh_size);

		if(j==SHT_NOBITS)continue;
		if(shi[i]==1)memcpy(bt+sho[i], ectx->buf+k, l);
		if(shi[i]==2)memcpy(bd+sho[i], ectx->buf+k, l);
	}


	ctx=BGBCC_NewContext();
	if(name)ctx->name=bgbcc_strdup(name);

	ctx->text=bt;
	ctx->data=bd;
	ctx->text_sz=szt;
	ctx->data_sz=szd;

//	ctx->ip=ctx->text+ctx->text_sz;
//	ctx->dp=ctx->data+ctx->data_sz;

//	printf("Resolve Symbols\n");

	for(i=0; i<ectx->nsym; i++)
	{
		sym=&(ectx->symtab[i]);

		j=elf32_word(sym->st_name);
		s=ectx->strtab+j;

		j=elf32_addr(sym->st_value);
		k=elf32_half(sym->st_shndx);
//		l=sym->st_info;

		if(k==SHN_UNDEF)continue;

//		if((sym->st_info&15)==STB_LOCAL)
//			s=BGBCC_GenSym();

		l=(shi[k]<<24)|(sho[k]+j);
		BGBCC_EmitLabelPos(ctx, s, l);
	}

	for(i=0; i<ectx->nsec; i++)
		if(ectx->rel[i])
	{
		for(j=0; j<ectx->nrel[i]; j++)
		{
			rel=&(ectx->rel[i][j]);

			k=elf32_word(rel->r_info);
			l=elf32_addr(rel->r_offset);
			s=elf32_get_symbol_name(ectx, k>>8);

			l=(shi[i]<<24)|(sho[i]+l);

			o=BGBCC_JMP_ABS32;
			if((k&255)==R_386_32)o=BGBCC_JMP_ABS32;
			if((k&255)==R_386_PC32)o=BGBCC_JMP_NEAR32;

			BGBCC_EmitGotoPos(ctx, s, o, l);
		}
	}

	return(ctx);
}

#if 0
static int main(int argc, char *argv[])
{
	char nbuf[16];

	Elf32_Ehdr *hdr;
	Elf32_Shdr *shdr;
	Elf32_Rel *rel;

	Elf32_Context *ctx;

	void *p, *q;
	byte *buf;
	FILE *fd;
	char *s;
	int sz;
	int ssec;
	int i, j, k, l;

	printf("open %s\n", argv[1]);
	fd=fopen(argv[1], "rb");

	if(!fd)
	{
		printf("fail open\n");
		return(-1);
	}

	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	buf=malloc(sz);
	fread(buf, 1, sz, fd);
	fclose(fd);

	ctx=malloc(sizeof(Elf32_Context));
	memset(ctx, 0, sizeof(Elf32_Context));

	ctx->buf=buf;
	ctx->szbuf=sz;

	hdr=(Elf32_Ehdr *)buf;
	elf32_dump_ehdr(hdr);
	ctx->hdr=hdr;

	ctx->secoffs=elf32_off(hdr->e_shoff);
	ctx->shsz=elf32_half(hdr->e_shentsize);
	ctx->nsec=elf32_half(hdr->e_shnum);
	ssec=elf32_half(hdr->e_shstrndx);

	shdr=(Elf32_Shdr *)(buf+ctx->secoffs+(ssec*ctx->shsz));
	ctx->shstr_offs=elf32_off(shdr->sh_offset);
	ctx->shstrtab=buf+ctx->shstr_offs;

	for(i=0; i<ctx->nsec; i++)
	{
		printf("section %d:\n", i);
		shdr=(Elf32_Shdr *)(buf+ctx->secoffs+(i*ctx->shsz));
		elf32_dump_shdr(shdr, ctx->shstrtab);
	}

	for(i=0; i<ctx->nsec; i++)
	{
		shdr=(Elf32_Shdr *)(buf+ctx->secoffs+(i*ctx->shsz));
		j=elf32_word(shdr->sh_type);
		if(j==SHT_REL)
		{
			j=elf32_word(shdr->sh_info);
//			shdr=(Elf32_Shdr *)(buf+ctx->secoffs+(j*ctx->shsz));

			k=elf32_off(shdr->sh_offset);
			ctx->rel[j]=(Elf32_Rel *)(ctx->buf+k);
			ctx->nrel[j]=elf32_word(shdr->sh_size)/sizeof(Elf32_Rel);
		}
	}

	ctx->stridx=elf32_lookup_section(ctx, ".strtab");
	shdr=(Elf32_Shdr *)(buf+ctx->secoffs+(ctx->stridx*ctx->shsz));
	ctx->stroffs=elf32_off(shdr->sh_offset);
	ctx->strtab=buf+ctx->stroffs;

	ctx->symidx=elf32_lookup_section(ctx, ".symtab");
	shdr=(Elf32_Shdr *)(buf+ctx->secoffs+(ctx->symidx*ctx->shsz));
	ctx->symoffs=elf32_off(shdr->sh_offset);
	ctx->nsym=elf32_word(shdr->sh_size)/sizeof(Elf32_Sym);
	ctx->symtab=(Elf32_Sym *)(ctx->buf+ctx->symoffs);

#if 0
	for(i=0; i<ctx->nsym; i++)
	{
		printf("symbol %d:\n", i);
		elf32_dump_symbol(ctx, i);
	}
#endif

#if 1
	for(i=0; i<ctx->nsec; i++)
		if(ctx->rel[i])
	{
		printf("reloc section %d (%s):\n", i,
			elf32_get_section_name(ctx, i));
		for(j=0; j<ctx->nrel[i]; j++)
		{
			rel=&(ctx->rel[i][j]);

			k=elf32_word(rel->r_info);
			printf("%08X %d %d (%s)\n",
				elf32_addr(rel->r_offset),
				k&255,
				k>>8, elf32_get_symbol_name(ctx, k>>8));
		}
		printf("\n");
	}
#endif

	return(0);
}

#endif

