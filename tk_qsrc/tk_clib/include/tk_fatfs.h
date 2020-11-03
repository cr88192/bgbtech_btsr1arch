#define TKFAT_SFL_DIRTY		0x00000200

#define TKFAT_ATTR_LFNENT		0x0F
#define TKFAT_ATTR_READONLY		0x01
#define TKFAT_ATTR_HIDDEN		0x02
#define TKFAT_ATTR_SYSTEM		0x04
#define TKFAT_ATTR_VOLLABEL		0x08
#define TKFAT_ATTR_DIRECTORY	0x10
#define TKFAT_ATTR_ARCHIVE		0x20
#define TKFAT_ATTR_DEVICE		0x40
#define TKFAT_ATTR_RESERVED		0x80

#define TKFAT_EMETA_BASE		0x01
#define TKFAT_EMETA_LINK		0x04
#define TKFAT_EMETA_LINKEXT		0x05

#define TKFAT_EMODE_IFMT		0x00017000
#define TKFAT_EMODE_SOCK		0x00014000
#define TKFAT_EMODE_LINK		0x00012000
#define TKFAT_EMODE_BLOCK		0x00006000
#define TKFAT_EMODE_REG			0x00010000
#define TKFAT_EMODE_DIR			0x00004000
#define TKFAT_EMODE_CHAR		0x00002000
#define TKFAT_EMODE_FIFO		0x00001000

#define TKFAT_EMODE_SETUID		0x00000800
#define TKFAT_EMODE_SETGID		0x00000400
#define TKFAT_EMODE_STICKY		0x00000200

#define TKFAT_EMODE_ACC_USR		0x000001C0
#define TKFAT_EMODE_ACC_GRP		0x00000038
#define TKFAT_EMODE_ACC_OTH		0x00000007
#define TKFAT_EMODE_ACC_RU		0x00000100
#define TKFAT_EMODE_ACC_WU		0x00000080
#define TKFAT_EMODE_ACC_XU		0x00000040
#define TKFAT_EMODE_ACC_RG		0x00000020
#define TKFAT_EMODE_ACC_WG		0x00000010
#define TKFAT_EMODE_ACC_XG		0x00000008
#define TKFAT_EMODE_ACC_RO		0x00000004
#define TKFAT_EMODE_ACC_WO		0x00000002
#define TKFAT_EMODE_ACC_XO		0x00000001

#define TKFAT_DT_UNKNOWN        0
#define TKFAT_DT_FIFO			1
#define TKFAT_DT_CHR			2
#define TKFAT_DT_DIR			4
#define TKFAT_DT_BLK			6
#define TKFAT_DT_REG			8
#define TKFAT_DT_LNK			10
#define TKFAT_DT_SOCK			12
#define TKFAT_DT_WHT			14


typedef struct TKFAT_MBR_Entry_s TKFAT_MBR_Entry;
typedef struct TKFAT_MBR_s TKFAT_MBR;
typedef struct TKFAT_FAT16_Boot_s TKFAT_FAT16_Boot;
typedef struct TKFAT_FAT32_Boot_s TKFAT_FAT32_Boot;
typedef struct TKFAT_FAT_DirEnt_s TKFAT_FAT_DirEnt;
typedef struct TKFAT_FAT_DirLfnEnt_s TKFAT_FAT_DirLfnEnt;

typedef struct TKFAT_FAT_DirEntExt_s TKFAT_FAT_DirEntExt;
typedef struct TKFAT_FAT_DirInfo_s TKFAT_FAT_DirInfo;

typedef struct TKFAT_FAT_MetaEnt_s TKFAT_FAT_MetaEnt;

struct TKFAT_MBR_Entry_s {
	byte flag;				//00, 0x80|=active
	byte shead;				//01, starting head
	byte ssect[2];			//02, starting track/sector
	byte fstype;			//04, filesystem
	byte ehead;				//05, ending head
	byte esect[2];			//06, ending track/sector
	byte lba_start[4];		//08, LBA start
	byte lba_count[4];		//0C, LBA count
};

struct TKFAT_MBR_s {
	byte pad[446];		//MBR boot code
	TKFAT_MBR_Entry entry[4];
	byte aa55[2];		//should be 0xAA55
};

struct TKFAT_FAT16_Boot_s {
	byte pad_jmp[3];			//0x00, boot JMP
	byte oem_name[8];			//0x03
	byte bytes_sector[2];		//0x0B
	byte sectors_cluster;		//0x0D
	byte reserved_sectors[2];	//0x0E
	byte num_fats;				//0x10
	byte root_dirents[2];		//0x11
	byte lba_count16[2];		//0x13
	byte media_type;			//0x15
	byte sectors_fat[2];		//0x16
	byte sectors_track[2];		//0x18
	byte heads[2];				//0x1A
	byte hidden_sectors[4];		//0x1C
	byte lba_count[4];			//0x20
	byte drive_id;				//0x24
	byte drive_flag;			//0x25
	byte ebsig;					//0x26
	byte vol_sn[4];				//0x27
	byte vol_label[11];			//0x2B
	byte fs_tyname[8];			//0x36
	byte boot_code[448];		//0x3E
	byte aa55[2];				//0x1FE, should be 0xAA55	
};

struct TKFAT_FAT32_Boot_s {
	byte pad_jmp[3];			//0x00, boot JMP
	byte oem_name[8];			//0x03
	byte bytes_sector[2];		//0x0B
	byte sectors_cluster;		//0x0D
	byte reserved_sectors[2];	//0x0E
	byte num_fats;				//0x10
	byte root_dirents[2];		//0x11
	byte lba_count16[2];		//0x13
	byte media_type;			//0x15
	byte sectors_fat[2];		//0x16
	byte sectors_track[2];		//0x18
	byte heads[2];				//0x1A
	byte hidden_sectors[4];		//0x1C
	byte lba_count[4];			//0x20
	byte sectors_fat32[4];		//0x24
	byte drive_flag2[2];		//0x28
	byte version[2];			//0x2A
	byte root_cluster[4];		//0x2C
	byte fsis_sector[2];		//0x30
	byte fsaltboot_sector[2];	//0x32
	byte resv[12];				//0x34
	byte drive_id;				//0x40
	byte drive_flag;			//0x41
	byte ebsig;					//0x42
	byte vol_sn[4];				//0x43
	byte vol_label[11];			//0x47
	byte fs_tyname[8];			//0x52
};

struct TKFAT_FAT_DirEnt_s {
byte name[8];					//0x00
byte ext[3];					//0x08
byte attrib;					//0x0B
byte lncase;					//0x0C
byte ctimems;					//0x0D
byte ctime[2];					//0x0E
byte cdate[2];					//0x10
byte ugid[2];					//0x12
byte cluster_hi[2];				//0x14
byte lmtime[2];					//0x16
byte lmdate[2];					//0x18
byte cluster_lo[2];				//0x1A
byte filesize[4];				//0x1C
};

struct TKFAT_FAT_DirLfnEnt_s {
byte seq;						//0x00
byte name1[10];					//0x01
byte attrib;					//0x0B
byte type;						//0x0C
byte csum;						//0x0D
byte name2[12];					//0x0E
byte cluster_lo[2];				//0x1A
byte name3[4];					//0x1C
};


struct TKFAT_FAT_MetaEnt_s {
byte name[8];		//8.3 Name, Base Entry
byte ext[3];		//8.3 Ext, Base Entry
byte etype;			//Entry Type
byte eseq;			//Entry Sequence or Count
byte eflag;			//Entry Flag
byte data[50];		//Payload Data
};

struct TKFAT_Volume_s {
TKFAT_FAT32_Boot boot;
};

typedef struct TKFAT_ImageInfo_s TKFAT_ImageInfo;

struct TKFAT_ImageInfo_s {
byte *pImgData;
int nImgBlks;
byte fsty;			//filesystem type
bool isfat16;		//FS is FAT16
byte szclust;		//sectors/cluster
byte shclust;		//shift for cluster (bytes)

int lba_start;		//LBA start of FAT volume
int lba_count;		//LBA count of FAT volume

int lba_fat1;		//LBA of first FAT
int lba_fat2;		//LBA of second FAT
int lba_root;		//LBA of root directory
int lba_data;		//LBA of data start

int tot_clust;
int clid_root;

TKFAT_MBR *mbr;
TKFAT_FAT16_Boot *boot16;
TKFAT_FAT32_Boot *boot32;

/* Static Buffer Cache */
u32 sbc_lba[64];
s16 sbc_lbn[64];
void *sbc_buf[64];
int sbc_num;

/* Temp Buffer Cache */
u32 tbc_lba[256];
s16 tbc_lbn[256];
void *tbc_buf[256];

//u32 tbc_lba[1024];
//s16 tbc_lbn[1024];
//void *tbc_buf[1024];

int tbc_num;
int tbc_max;
int tbc_pred0;
int tbc_pred1;
int tbc_rov;

#if 1
/* Temp FAT Buffer Cache */

// u32 tfbc_lba[1024];
// s16 tfbc_lbn[1024];
// void *tfbc_buf[1024];

u32 tfbc_lba[256];
s16 tfbc_lbn[256];
void *tfbc_buf[256];

int tfbc_num;
int tfbc_pred0;
int tfbc_pred1;
int tfbc_rov;

byte *tfbc_pr_ofs;
int tfbc_pr_lba;

#endif

int cl_rov;			//cluster rover
int walk_clid;		//walk starting cluster
int walk_clofs;		//walk cluster offset
int walk_clcur;		//walk cluster current

int walk2_clid;		//walk starting cluster
int walk2_clofs;	//walk cluster offset
int walk2_clcur;	//walk cluster current

int walk3_clid;		//walk starting cluster
int walk3_clofs;	//walk cluster offset
int walk3_clcur;	//walk cluster current

int walk4_clid;		//walk starting cluster
int walk4_clofs;	//walk cluster offset
int walk4_clcur;	//walk cluster current

int walk_luhint[65536];		//lookup, once per 128 clusters
int walk_lumax;
};


struct TKFAT_FAT_DirEntExt_s {
TKFAT_FAT_DirEnt deb;	//basic dirent
TKFAT_FAT_MetaEnt meb;	//metadata entry
TKFAT_ImageInfo *img;
int clid;				//cluster ID of parent directory
int mclid;				//cluster ID of parent directory
int idx;				//index within directory
int midx;				//index within metadata

u32 de_clid;			//dirent cluster (cache)
u32 de_size;
u32 de_mclid;			//metadata cluster (cache)

u16 me_uid;
u16 me_gid;
u32 me_mode;

bool is_write;
bool is_dirty;


byte de_name[512];		//name (UTF-8, LFN)
byte de_aname[512];		//alt name (UTF-8, LFN)
};

struct TKFAT_FAT_DirInfo_s {
TKFAT_ImageInfo *img;
int clid;			//cluster ID of current directory
};
