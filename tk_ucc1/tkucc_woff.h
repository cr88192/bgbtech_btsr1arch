/*
 * WOFF: WAD-like Object Format
 *
 * Normal section names will start with '.'
 * Special (metadata) sections will start with '$'
 *
 * WOFF section numbers will be the lump index + 1.
 * Section 0 will be used for undefined/extern symbols.
 */

#define TKUCC_FCC_WOFF		TKUCC_FOURCC('W','O','F','F')
#define TKUCC_FCC_B264		TKUCC_FOURCC('B','2','6','4')

#define TKUCC_FCC_PE		TKUCC_FOURCC('P','E',0,0)

typedef struct TKUCC_WoffHead_s		TKUCC_WoffHead;
typedef struct TKUCC_WoffLump_s		TKUCC_WoffLump;
typedef struct TKUCC_WoffSymbol_s		TKUCC_WoffSymbol;

struct TKUCC_WoffHead_s {
u32 fcc_magic;	//'WOFF'
u32 num_sec;	//number of lumps/sections
u32 ofs_sec;	//offset to lumps/sections
u32 arch;		//architecture magic
};

struct TKUCC_WoffLump_s {
u32 ofs_data;	//offset of (compressed) data
u32 dsize;		//uncompressed size of data
u32 csize;		//compressed size of data
byte type;		//file/entry type (0=default, 1..255, special members)
byte method;	//compression method (0=store)
u16 flags;		//special flags
byte name[16];	//inline name
};

struct TKUCC_WoffSymbol_s {
u32 sym_name;	//offset of name in strtab(symbol), symbol index (reloc)
u32 sym_ofs;	//offset of symbol (or reloc) in section
byte sym_sec;	//section this applies to (1-base, 0=undef/extern)
byte sym_type;	//symbol or reloc type
u16 pad0;		//reserved, MBZ
};

typedef struct TKUCC_ArchFileHead_s TKUCC_ArchFileHead;

struct TKUCC_ArchFileHead_s {
char name[16];
char mtime[12];
char uid[6];
char gid[6];
char mode[8];
char size[10];
char eol[2];
};
