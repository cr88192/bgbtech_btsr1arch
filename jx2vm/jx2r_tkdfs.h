#ifndef TKDFS_DBGBREAK
#ifdef _MSC_VER
#define TKDFS_DBGBREAK __debugbreak();
#endif
#endif

#ifndef TKDFS_DBGBREAK
#ifdef __BGBCC__
#define TKDFS_DBGBREAK __debugbreak();
#endif
#endif

#ifndef TKDFS_DBGBREAK
#define TKDFS_DBGBREAK
#endif

#define TKDFS_FCC_FHDR	RIFF_MAKETAG('F','H','D','R')
#define TKDFS_FCC_DHDR	RIFF_MAKETAG('D','H','D','R')

#define TKDFS_FCC_IDAT	RIFF_MAKETAG('I','D','A','T')
#define TKDFS_FCC_IDX4	RIFF_MAKETAG('I','D','X','4')
#define TKDFS_FCC_IDX6	RIFF_MAKETAG('I','D','X','6')
#define TKDFS_FCC_IDX8	RIFF_MAKETAG('I','D','X','8')
#define TKDFS_FCC_IXC8	RIFF_MAKETAG('I','X','C','8')

#define TKDFS_TCC_FH	RIFF_MAKETWOCC('F','H')
#define TKDFS_TCC_DH	RIFF_MAKETWOCC('D','H')

#define TKDFS_TCC_ID	RIFF_MAKETWOCC('I','D')
#define TKDFS_TCC_I4	RIFF_MAKETWOCC('I','4')
#define TKDFS_TCC_I6	RIFF_MAKETWOCC('I','6')
#define TKDFS_TCC_I8	RIFF_MAKETWOCC('I','8')
#define TKDFS_TCC_C8	RIFF_MAKETWOCC('C','8')

#define TKDFS_DFLAG_DIRTY		0x01	//General: Mark as dirty
#define TKDFS_DFLAG_NEWINIT		0x02	//Block: Don't read, clear to zero
#define TKDFS_DFLAG_ISDIR		0x04	//Mark as a directory
#define TKDFS_DFLAG_INLINE		0x08	//Inode: Hint to store data inline
#define TKDFS_DFLAG_NOALLOC		0x10	//Block: Do Not Allocate
#define TKDFS_DFLAG_COMPRESS	0x20	//Inode: Use Compression
#define TKDFS_DFLAG_DELETE		0x40	//Inode/Dir: Delete Entry
#define TKDFS_DFLAG_PREALLOC	0x80	//Block: Allocate Early

#define TKDFS_DFLAG_ISDIR_RD	0x04
#define TKDFS_DFLAG_ISDIR_WR	0x05


#define TKDFS_MODE_XO		0x0001
#define TKDFS_MODE_WO		0x0002
#define TKDFS_MODE_RO		0x0004
#define TKDFS_MODE_XG		0x0008
#define TKDFS_MODE_WG		0x0010
#define TKDFS_MODE_RG		0x0020
#define TKDFS_MODE_XU		0x0040
#define TKDFS_MODE_WU		0x0080
#define TKDFS_MODE_RU		0x0100

#define TKDFS_MODE_SETGID	0x0400
#define TKDFS_MODE_SETUID	0x0800

#define TKDFS_MODE_TYMASK	0xF000
#define TKDFS_MODE_DIR		0x4000
#define TKDFS_MODE_FILE		0x8000
#define TKDFS_MODE_SYMLINK	0xA000

#define TKDFS_FIXINO_INOTAB		0
#define TKDFS_FIXINO_ROOTDIR	1
#define TKDFS_FIXINO_BLKBMP		2
#define TKDFS_FIXINO_INOBMP		3
#define TKDFS_FIXINO_INOTAB2	4

#define TKDFS_FIXINO_FIRSTFILE	8

#define TKDFS_ETYPE_FREE		0
#define TKDFS_ETYPE_FILE_BASE	1		//name is base 48 bytes
#define TKDFS_ETYPE_FILE_LFN	2		//name is/was an LFN
#define TKDFS_ETYPE_FRAG_LFN	3

#define TKDFS_ETYPE_DEL_BASE	9		//deleted, basic name
#define TKDFS_ETYPE_DEL_LFN		10		//deleted, LFN

#define TKDFS_ETYPE_INO_SYS		TKDFS_ETYPE_FRAG_LFN	//system inode


#define TKDFS_AVG_LG2FILESZ		18	//Log2 of estimated average filesize

typedef struct TKDFS_BootBlock_s TKDFS_BootBlock;
typedef struct TKDFS_INode_File_s TKDFS_INode_File;
typedef struct TKDFS_INode_DirMeta_s TKDFS_INode_DirMeta;
typedef struct TKDFS_INode_FileBlockIndex4_s TKDFS_INode_FileBlockIndex4;
// typedef struct TKDFS_INode_FileBlockIndex6_s TKDFS_INode_FileBlockIndex6;
typedef struct TKDFS_INode_FileBlockIndex8_s TKDFS_INode_FileBlockIndex8;
typedef struct TKDFS_INode_FileBlockIndexC8_s TKDFS_INode_FileBlockIndexC8;
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
	byte ln2_sz_cblk;			//0x42, Size of Logical Compressed Block (16)
	byte ln2_ino_fixed;			//0x43, Log2 of fixed inodes
	byte resv2[4];              //0x43, reserved
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

struct TKDFS_INode_DirMeta_s {
   byte refname[48];  //00, filename, or first part of longname
   u32 dirino_lo;     //30, irectory inode, low
   u16 dirino_hi;     //34, directory inode, high
   byte tag1;         //36, tag byte / reserved
   byte tag2;         //37, tag byte / reserved
};

struct TKDFS_INode_FileBlockIndex4_s {
	u32 block_0l[16];  //direct blocks
	u32 block_1l[ 8];  //indirect blocks (1-level)
	u32 block_2l[ 4];  //indirect blocks (2-level)
	u32 block_3l[ 2];  //indirect blocks (3-level)
	u32 block_4l;      //indirect blocks (4-level)
	u32 block_5l;      //indirect blocks (5-level)
};

#if 0
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
#endif

struct TKDFS_INode_FileBlockIndex8_s {
	u64 block_0l[16];  //direct blocks
	u64 block_1l[ 8];  //indirect blocks (1-level)
	u64 block_2l[ 4];  //indirect blocks (2-level)
	u64 block_3l;      //indirect blocks (3-level)
	u64 block_4l;      //indirect blocks (4-level)
	u64 block_5l;      //indirect blocks (5-level)
	u64 block_6l;      //indirect blocks (6-level)
};

struct TKDFS_INode_FileBlockIndexC8_s {
	u64 block_0l[8];	//direct blocks
	u64 block_1l[4];	//indirect blocks (1-level)
	u64 block_2l;		//indirect blocks (2-level)
	u64 block_3l;		//indirect blocks (3-level)
	u64 block_4l;		//indirect blocks (4-level)
	u64 block_5l;		//indirect blocks (5-level)
};

struct TKDFS_DirEnt_Basic_s {
	u32 ino;			//00, inode number
	u16 lsn;			//04, left child node
	u16 psn;			//06, parent node
	u16 rsn;			//08, right child node
	u16 hsn;			//0A, node high bits

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
//	u32 lsn;			//04, tree root
//	u32 rsn;			//08, first free
	u16 lsn;			//04, left child node
	u16 psn;			//06, parent node
	u16 rsn;			//08, right child node
	u16 hsn;			//0A, node high bits
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

TKDFS_INode_File				*ino_file;
TKDFS_INode_DirMeta				*ino_dmhd;
TKDFS_INode_FileBlockIndex4		*ino_idx4;
TKDFS_INode_FileBlockIndex8		*ino_idx8;
TKDFS_INode_FileBlockIndexC8	*ino_idxc8;
byte *ino_idat;		//inline data
short ino_szidat;	//inline data (size limit)
int d_ino;
int d_flag;
};

struct TKDFS_DirentInfo_s {
char de_name[128];
s64 de_ino;

s64 di_ino;
int di_idx;
byte is_write;
byte is_dirty;

// int pivstk[64];
// int pivstkpos;

TKDFS_ImageContext *img;
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
s64 blk_count;

s64 blk_inotab;
s64 blk_inotab2;

byte ln2_blksz;
byte ln2_inosz;
byte ln2_cblksz;
byte ln2_ino_fixed;

TKDFS_InodeInfo img_inocache[64];

int img_blkcache_blkid[128];
byte img_blkcache_data[128*512];
int img_blkcache_flag[128];

int img_cblkcache_ino[16];
int img_cblkcache_idx[16];
byte *img_cblkcache_data;
byte *img_cblkcache_tbuf;	//temp buffer for compressed block data
int img_cblkcache_flag[16];

u64 pad0;	//alignment pad
byte t_bootblk[512];
// TKDFS_InodeInfo t_inoroot;
};



u32 TKDFS_ReadImageInodeWord32(TKDFS_ImageContext *img,
	int d_ino, int d_idx);
int TKDFS_WriteImageInodeWord32(TKDFS_ImageContext *img,
	int d_ino, int d_idx, u32 d_val);

int TKDFS_SyncCompressedBlockIdx(TKDFS_ImageContext *img, int idx);
int TKDFS_SyncCompressedBlocks(TKDFS_ImageContext *img);
byte *TKDFS_GetImageCompressedBlock(TKDFS_ImageContext *img,
	int d_ino, int d_blk, int d_flg);

byte *TKDFS_GetImageCachedInodeBlock(TKDFS_ImageContext *img,
	int d_ino, s64 d_blk, int d_flg);
int TKDFS_InitializeImageFileInode(TKDFS_ImageContext *img,
	TKDFS_InodeInfo *info, int ino, int d_flg);
int TKDFS_InitializeImageFileInodeIdat(TKDFS_ImageContext *img,
	TKDFS_InodeInfo *info, int ino);
TKDFS_InodeInfo *TKDFS_GetImageCachedInode(TKDFS_ImageContext *img,
	int d_ino, int d_flg);

int TKDFS_ImageSetInodeInfoDeh(TKDFS_ImageContext *img,
	TKDFS_InodeInfo *info,
	char *bname, s64 dirino, int dfl);

int TKDFS_CopyName48Expand(byte *dst, byte *src);

int TKDFS_SyncImageInodes(TKDFS_ImageContext *img);
int TKDFS_SetImageInodeBlockNum(TKDFS_ImageContext *img,
	TKDFS_InodeInfo *ino_inf, int d_blk, u64 newblk);
int TKDFS_InitBaseName48(char *bname, char *d_name);
int TKDFS_WriteImageDirent(TKDFS_ImageContext *img,
	TKDFS_DirentInfo *info, int d_ino, int d_idx);
int TKDFS_CopyDirentInfo(TKDFS_ImageContext *img,
	TKDFS_DirentInfo *dinfo,
	TKDFS_DirentInfo *sinfo);
int TKDFS_ImageAllocInodeDirentName(TKDFS_ImageContext *img,
	TKDFS_DirentInfo *info,
	int d_ino, char *d_name);

int TKDFS_GetDirentLeftIdx(
	TKDFS_DirentInfo *info);
int TKDFS_GetDirentRightIdx(
	TKDFS_DirentInfo *info);
int TKDFS_GetDirentParentIdx(
	TKDFS_DirentInfo *info);

int TKDFS_SetDirentLeftIdx(
	TKDFS_DirentInfo *info, int idx);
int TKDFS_SetDirentRightIdx(
	TKDFS_DirentInfo *info, int idx);
int TKDFS_SetDirentParentIdx(
	TKDFS_DirentInfo *info, int idx);

int TKDFS_ImageCheckDirentUpdateZ(
	TKDFS_ImageContext *img, int d_ino, int d_idx);
int TKDFS_ImageCheckDirentRebalance(
	TKDFS_ImageContext *img, TKDFS_DirentInfo *info);
int TKDFS_ImageFreeDirent(
	TKDFS_ImageContext *img, TKDFS_DirentInfo *info);
int TKDFS_ImageRemoveDirentRebalance(
	TKDFS_ImageContext *img, TKDFS_DirentInfo *info);
int TKDFS_ImageLookupInodeDirentName(TKDFS_ImageContext *img,
	TKDFS_DirentInfo *info, int d_ino, char *d_name, int dfl);
int TKDFS_WalkDirEntNext(TKDFS_ImageContext *img, TKDFS_DirentInfo *dee);
int TKDFS_ImageLookupInodePathI(TKDFS_ImageContext *img,
	TKDFS_DirentInfo *info, char *path, int dfl);
int TKDFS_ImageLookupInodePath(TKDFS_ImageContext *img,
	TKDFS_DirentInfo *info, char *path, int dfl);
int TKDFS_ImageCreateInodePath(TKDFS_ImageContext *img,
	TKDFS_DirentInfo *info, char *path, int dfl);
int TKDFS_ReadWriteImageInodeFileData(TKDFS_ImageContext *img,
	byte *data, int d_ino, s64 fd_offs, int fd_len, int dfl);
int TKDFS_ReadImageInodeFileData(TKDFS_ImageContext *img,
	byte *data, int d_ino, s64 fd_offs, int fd_len);
int TKDFS_WriteImageInodeFileData(TKDFS_ImageContext *img,
	byte *data, int d_ino, s64 fd_offs, int fd_len);
int TKDFS_ReadWriteDirEntFile(
	TKDFS_DirentInfo *dee, s64 ofs, int dfl, byte *dbuf, s64 dsz);
s64 TKDFS_GetDirEntSize(TKDFS_DirentInfo *dee);
int TKDFS_SetDirEntSize(TKDFS_DirentInfo *dee, s64 size);
s64 TKDFS_GetDirEntInode(TKDFS_DirentInfo *dee);
int TKDFS_SetDirEntInode(TKDFS_DirentInfo *dee, s64 d_ino);
int TKDFS_DeleteDirEnt(TKDFS_DirentInfo *dee);
int TKDFS_UpdateDirEnt(TKDFS_DirentInfo *dee);
int TKDFS_SyncDirEntFile(TKDFS_DirentInfo *dee);

int TKDFS_GetDirEntMode(TKDFS_DirentInfo *dee);
int TKDFS_SetDirEntMode(TKDFS_DirentInfo *dee, int d_mode);
int TKDFS_GetDirEntUid(TKDFS_DirentInfo *dee);
int TKDFS_GetDirEntGid(TKDFS_DirentInfo *dee);
s64 TKDFS_GetDirEntCTime(TKDFS_DirentInfo *dee);
s64 TKDFS_GetDirEntMTime(TKDFS_DirentInfo *dee);

int TKDFS_ImageDestroyInode(TKDFS_ImageContext *img,
	int d_ino, int d_flg);
int TKDFS_DeleteInode(TKDFS_ImageContext *img, int d_ino);

