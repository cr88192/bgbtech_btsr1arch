#define EXWAD_DIRFL_RVANAME	1
#define EXWAD_DIRFL_DIRPFX	2

#define EXWAD_DIR_CM_STORE		0
#define EXWAD_DIR_CM_DEFLATE	8
#define EXWAD_DIR_CM_DEFLATE64	9

#define EXWAD_MAGIC1 "ExWAD101"
#define EXWAD_MAGIC2 "DAWxE\r\n\xFF"
#define EXWAD_CELLSZ 16
#define EXWAD_NAMESZ 32

typedef byte xu16le_t[2];
typedef byte xu32le_t[4];

typedef struct ExWAD_Context_s ExWAD_Context;
typedef struct ExWAD_Header_s ExWAD_Header;
typedef struct ExWAD_DirEnt_s ExWAD_DirEnt;

struct ExWAD_Context_s
{
byte *img_base;
char *name;
byte *img_map;

int img_size;
int head_rva;

int img_saveSize;

ExWAD_Header *head;
ExWAD_DirEnt *dir;
};

struct ExWAD_Header_s
{
byte magic[8];			//"ExWAD100"
xu32le_t num_dirents;	//number of directory entries
xu32le_t rva_dirents;	//RVA of directory entries
byte magic2[8];			//"DAWxE\r\n\xFF"
xu32le_t head_rva;		//header self RVA
xu16le_t head_size;		//total header size
xu16le_t head_hash;		//hash code for header
};

#if 0
struct ExWAD_DirEnt_s
{
xu32le_t rva_data;		//RVA of (compressed) data
xu32le_t dsize_data;	//uncompressed size of data
xu32le_t csize_data;	//compressed size of data
byte type;				//file/entry type (0=default, 1..255, special members)
byte method;			//compression method (0=store, 8=Deflate, 9=Deflate64)
xu16le_t flags;			//special flags
byte name[16];			//inline name
};
#endif

#if 1
struct ExWAD_DirEnt_s
{
xu32le_t rva_data;		//RVA of (compressed) data
xu32le_t dsize_data;	//uncompressed size of data
xu32le_t csize_data;	//compressed size of data
byte type;				//file/entry type (0=default, 1..255, special members)
byte method;			//compression method (0=store, 8=Deflate, 9=Deflate64)
xu16le_t flags;			//special flags
xu32le_t dir;			//parent directory
byte pad[12];			//inline name
byte name[32];			//inline name
};
#endif
