#define		TK_W4PU_NONE		0
#define		TK_W4PU_STATIC		16
#define		TK_W4PU_CACHEDIRTY	32
#define		TK_W4PU_CACHE		64

#define		TK_W4ETY_UNUSED		0
#define		TK_W4ETY_NORMAL		1
#define		TK_W4ETY_DIR		2
#define		TK_W4ETY_SYMLINK	3

#define		TK_W4CMP_NONE		0
#define		TK_W4CMP_FRAG		1
#define		TK_W4CMP_PACKED		2
#define		TK_W4CMP_RP2		3
#define		TK_W4CMP_LZ4		4

#define		TK_W4_FRAGSZ		262144

typedef struct TK_Wad2Info_s	TK_Wad2Info;
typedef struct TK_Wad4Info_s	TK_Wad4Info;
typedef struct TK_WadLump_s	TK_WadLump;
typedef struct TK_Wad2Lump_s	TK_Wad2Lump;
typedef struct TK_Wad4Lump_s	TK_Wad4Lump;
typedef struct TK_WadImage_s	TK_WadImage;

struct TK_Wad2Info_s
{
	byte	ident[4];	//00: WAD2
	u32		numlumps;	//04: Number of Lumps
	u32		diroffs;	//08: Offset of Directory
	u32		typeofs;	//0C: Offset of Types
};

struct TK_Wad4Info_s
{
	byte	ident[4];	//00: WAD4
	u32		numlumps;	//04: Number of lumps in the directory.
	u32		diroffs;	//08: Offset of Directory.
 	u32		hashoffs;	//0C: Offset of Directory Hash.
 	u32		bmpoffs;	//10: Offset of Allocation Bitmap.
 	u32		bmpsize;	//14: Bitmap Size
 	u32		resv4;		//18: Reserved
 	u32		resv5;		//1C: Reserved
};

struct TK_WadLump_s
{
	u32		offs;
	u32		size;
	byte	name[8];
};

struct TK_Wad2Lump_s
{
	u32		offs;		//00: offset
	u32		csize;		//04: compressed size
	u32		dsize;		//08: uncompressed size
	byte	ety;		//0C: Entry Type
	byte	cmp;		//0D: Compression
	u16		chn;		//0E: Chain
	byte	name[16];	//10: Name
};

struct TK_Wad4Lump_s
{
	u32		offs;		//00: offset
	u32		csize;		//04: compressed size
	u32		dsize;		//08: uncompressed size
	byte	ety;		//0C: Entry Type
	byte	cmp;		//0D: Compression
	u16		chn;		//0E: Chain
 	u16		dirid;		//10: Directory ID / Parent Directory (DirEnt Index)
 	u16		dirnext;	//12: Next in Directory (DirEnt Index)
 	u16		uid;		//14: UID
 	u16		gid;		//16: GID
 	u16		mode;		//18: Mode Flags (---Srwxrwxrwx)
 	u16		hibits;		//1A: Some extra bits
 	u32		mtime;		//1C: File timestamp
 	byte	name[32];	//20: name
};


struct TK_WadImage_s
{
	TK_FILE			*img_fd;		//Backing Image (file/device)
	TK_Wad2Info		*w2inf;
	TK_Wad4Info		*w4inf;
	
	TK_Wad2Lump		*w2dir;
	TK_Wad4Lump		*w4dir;
	
	byte			wadver;
	byte			readwrite;		//Set if image is Read/Write
	int				hashsz;
	u16				*hash2;
	u32				*hash4;
	
	void			**lca_data;		//Lump Cache, Data
	int				*lca_sz;		//Lump Cache, LumpSize
	int				*lca_id;		//Lump Cache, LumpID
};

typedef struct TK_WadZBlock_s TK_WadZBlock;
struct TK_WadZBlock_s
{
	TK_WadZBlock	*next;
	void			**user;
	byte			*data;
	int				size;
	byte			tag;
	
	void			*evptr;
	int				(*EvFunc)(void *blkptr, void *vfsptr);
};
