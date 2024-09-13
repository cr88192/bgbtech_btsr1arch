#define TKDFS_FCC_FHDR	RIFF_MAKETAG('F','H','D','R')
#define TKDFS_FCC_IDAT	RIFF_MAKETAG('I','D','A','T')
#define TKDFS_FCC_IDX4	RIFF_MAKETAG('I','D','X','4')
#define TKDFS_FCC_IDX6	RIFF_MAKETAG('I','D','X','6')
#define TKDFS_FCC_IDX8	RIFF_MAKETAG('I','D','X','8')

#define TKDFS_TCC_FH	RIFF_MAKETWOCC('F','H')
#define TKDFS_TCC_ID	RIFF_MAKETWOCC('I','D')
#define TKDFS_TCC_I4	RIFF_MAKETWOCC('I','4')
#define TKDFS_TCC_I6	RIFF_MAKETWOCC('I','6')
#define TKDFS_TCC_I8	RIFF_MAKETWOCC('I','8')

typedef struct TKDFS_BootBlock_s TKDFS_BootBlock;
typedef struct TKDFS_INode_File_s TKDFS_INode_File;
typedef struct TKDFS_INode_FileBlockIndex4_s TKDFS_INode_FileBlockIndex4;
typedef struct TKDFS_INode_FileBlockIndex6_s TKDFS_INode_FileBlockIndex6;
typedef struct TKDFS_INode_FileBlockIndex8_s TKDFS_INode_FileBlockIndex8;
typedef struct TKDFS_DirEnt_Basic_s TKDFS_DirEnt_Basic;
typedef struct TKDFS_DirEnt_Namefrag_s TKDFS_DirEnt_Namefrag;
typedef struct TKDFS_DirEnt_Header_s TKDFS_DirEnt_Header;

typedef struct TKDFS_ImageContext_s TKDFS_ImageContext;

struct TKDFS_BootBlock_s {
	byte pad_jmp[3];			//0x00, Boot JMP, Eg, EB-5E-90
	byte oem_name[8];			//0x03, "TKDFS2  "
	byte bytes_sector[2];		//0x0B, 512
	byte sectors_cluster;		//0x0D, 2/4/8/16/32
	byte reserved_sectors[2];	//0x0E, 0
	byte num_fats;				//0x10, 0
	byte root_dirents[2];		//0x11, 0
	byte lba_count16[2];		//0x13, 0
	byte media_type;            //0x15, 0xF8
	u16 sectors_fat;            //0x16, 0
	u16 sectors_track;          //0x18, 63 / ignored
	u16 heads;                  //0x1A, 255 / ignored
	u32 hidden_sectors;         //0x1C, 0
	u32 lba_count32;			//0x20, 0 | sector-count

	byte drive_number;			//0x24, hint
	byte drive_flags;			//0x25, 0
	byte ebp_sig;               //0x26, 0x80
	byte resv1;                 //0x27, 0
	u64 lba_count64;            //0x28, Total Sectors
	u64 blk_inotab;             //0x30, First Block of Inode Table.
	u64 blk_inotab2;            //0x38, Backup Inode Table.
	byte ln2_sz_ino;            //0x40, Size of inode (Log2, 8 or 9)
	byte ln2_sz_de;             //0x41, Size of Dirent (log2, 6)
	byte resv2[6];              //0x42, reserved
	u64 vol_id;                 //0x48, Volume ID Number
	byte label[16];             //0x50, Volume Label
	byte bootstrap[414];        //0x60, x86 bootstrap or similar (N/A)
	byte magic_55aa[2];         //0x1FE, 55-AA
};

struct TKDFS_INode_File_s {
	u64	dsize;		//0x00, File Size (Logical)
	u64	csize;		//0x08, File Size (Compressed)
	u64	ctime;		//0x10, Creation Time
	u64	mtime;		//0x18, Modified Time
	u16	uid;		//0x20, Owner UID
	u16	gid;		//0x22, Owner GID
	u16	mode;		//0x34, File Mode Flags
	byte cmpmode;	//0x26, Compression Type
	byte etype;		//0x27, File Entry Type
};

struct TKDFS_INode_FileBlockIndex4_s {
	u32 block_0l[16];  //direct blocks
	u32 block_1l[ 8];  //indirect blocks (1-level)
	u32 block_2l[ 4];  //indirect blocks (2-level)
	u32 block_3l[ 2];  //indirect blocks (3-level)
	u32 block_4l;      //indirect blocks (4-level)
	u32 block_5l;      //indirect blocks (5-level)
};

struct TKDFS_INode_FileBlockIndex6_s {
	u32 block_0l[16];  //direct blocks
	u32 block_1l[ 8];  //indirect blocks (1-level)
	u32 block_2l[ 4];  //indirect blocks (2-level)
	u32 block_3l[ 2];  //indirect blocks (3-level)
	u32 block_4l;      //indirect blocks (4-level)
	u32 block_5l;      //indirect blocks (5-level)
	u16 block_0h[16];  //direct block, high bits
	u16 block_1h[ 8];  //
	u16 block_2h[ 4];  //
	u16 block_3h[ 2];  //
	u16 block_4h;      //
	u16 block_5h;      //
};

struct TKDFS_INode_FileBlockIndex8_s {
	u64 block_0l[16];  //direct blocks
	u64 block_1l[ 8];  //indirect blocks (1-level)
	u64 block_2l[ 4];  //indirect blocks (2-level)
	u64 block_3l[ 2];  //indirect blocks (3-level)
	u64 block_4l;      //indirect blocks (4-level)
	u64 block_5l;      //indirect blocks (5-level)
};

struct TKDFS_DirEnt_Basic_s {
	u32 ino;			//00, inode number
	u32 lsn;			//04, left child node
	u32 rsn;			//08, right child node
	u16 ino_hi;	        //0C, inode high bits
	byte zdepth;		//0E, Z height of node (0=Leaf)
	byte etype;	        //0F, dirent type
	byte name[48];		//10, name
};

struct TKDFS_DirEnt_Namefrag_s {
	byte name1[12];    //00, name 1
	byte fe_hash;      //0C, hash of first 48 bytes of name
	byte resv1;        //0D, reserved
	byte index;        //0E, Z height of node (0=Leaf)
	byte etype;        //0F, dirent type
	byte name2[48];    //10, name 2
};

struct TKDFS_DirEnt_Header_s {
	u32 ino;			//00, inode number
	u32 lsn;			//04, tree root
	u32 rsn;			//08, first free
	u16 ino_hi;	        //0C, inode high bits
	byte zdepth;		//0E, Z height of node (0=Leaf)
	byte etype;	        //0F, dirent type
	byte name[8];		//10, name
	u32 lastent;		//18, last entry
	byte resv[36];		//1C
};

typedef struct TKDFS_InodeInfo_s TKDFS_InodeInfo;
typedef struct TKDFS_DirentInfo_s TKDFS_DirentInfo;

struct TKDFS_InodeInfo_s {
byte t_data[512];

TKDFS_INode_File *ino_file;
TKDFS_INode_FileBlockIndex4 *ino_idx4;
byte *ino_idat;
int d_ino;
};

struct TKDFS_DirentInfo_s {
char de_name[128];
int de_ino;

TKDFS_DirEnt_Basic *deb;
TKDFS_DirEnt_Header *deh;
TKDFS_DirEnt_Basic t_deb;
};

struct TKDFS_ImageContext_s {
TKDFS_BootBlock *bootblk;

int bdev;
s64 lba_start;
s64 lba_count;
// s64 lba_inotab;

s64 blk_inotab;

byte ln2_blksz;
byte ln2_inosz;

TKDFS_InodeInfo img_inocache[64];

int img_blkcache_blkid[128];
byte img_blkcache_data[128*512];
int img_blkcache_flag[128];

u64 pad0;	//alignment pad
byte t_bootblk[512];
// TKDFS_InodeInfo t_inoroot;
};
