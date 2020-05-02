#define TKFAT_SFL_DIRTY		0x00000200

typedef struct JX2R_TKFAT_MBR_Entry_s JX2R_TKFAT_MBR_Entry;
typedef struct JX2R_TKFAT_MBR_s JX2R_TKFAT_MBR;
typedef struct JX2R_TKFAT_FAT16_Boot_s JX2R_TKFAT_FAT16_Boot;
typedef struct JX2R_TKFAT_FAT32_Boot_s JX2R_TKFAT_FAT32_Boot;
typedef struct JX2R_TKFAT_FAT_DirEnt_s JX2R_TKFAT_FAT_DirEnt;
typedef struct JX2R_TKFAT_FAT_DirLfnEnt_s JX2R_TKFAT_FAT_DirLfnEnt;

typedef struct JX2R_TKFAT_FAT_DirEntExt_s JX2R_TKFAT_FAT_DirEntExt;
typedef struct JX2R_TKFAT_FAT_DirInfo_s JX2R_TKFAT_FAT_DirInfo;

struct JX2R_TKFAT_MBR_Entry_s {
	byte flag;				//0x80|=active
	byte shead;				//starting head
	byte ssect[2];			//starting track/sector
	byte fstype;			//filesystem
	byte ehead;				//ending head
	byte esect[2];			//ending track/sector
	byte lba_start[4];		//LBA start
	byte lba_count[4];		//LBA count
};

struct JX2R_TKFAT_MBR_s {
	byte pad[446];		//MBR boot code
	JX2R_TKFAT_MBR_Entry entry[4];
	byte aa55[2];		//should be 0xAA55
};

struct JX2R_TKFAT_FAT16_Boot_s {
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

struct JX2R_TKFAT_FAT32_Boot_s {
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

union JX2R_TKFAT_FAT_Boot_s {
struct JX2R_TKFAT_FAT16_Boot_s fat16;
struct JX2R_TKFAT_FAT32_Boot_s fat32;
};

struct JX2R_TKFAT_FAT_DirEnt_s {
	byte name[8];				//0x00
	byte ext[3];				//0x08
	byte attrib;				//0x0B
	byte lncase;				//0x0C
	byte ctime_ms;				//0x0D
	byte ctime[2];				//0x0E
	byte cdate[2];				//0x10
	byte mdate[2];				//0x12
	byte cluster_hi[2];			//0x14
	byte lmtime[2];				//0x16
	byte lmdate[2];				//0x18
	byte cluster_lo[2];			//0x1A
	byte filesize[4];			//0x1C
};

struct JX2R_TKFAT_FAT_DirLfnEnt_s {
	byte seq;					//0x00
	byte name1[10];				//0x01
	byte attrib;				//0x0B
	byte type;					//0x0C
	byte csum;					//0x0D
	byte name2[12];				//0x0E
	byte cluster_lo[2];			//0x1A
	byte name3[4];				//0x1C
};

typedef struct JX2R_TKFAT_ImageInfo_s JX2R_TKFAT_ImageInfo;
typedef struct JX2R_TKFAT_SegmentInfo_s JX2R_TKFAT_SegmentInfo;

struct JX2R_TKFAT_ImageInfo_s {
byte *pImgData;		//Image Data (Ramdisk)
BJX2_FILE *fdImgData;	//Image Data (File-Backed)
s64 nImgBlks;

byte fsty;			//filesystem type
jx2_bool isfat16;	//FS is FAT16
jx2_bool isfat32e;	//FS is FAT32+
byte szclust;		//sectors/cluster
byte shclust;		//shift for cluster (bytes)

int lba_start;		//LBA start of FAT volume
s64 lba_count;		//LBA count of FAT volume

int lba_fat1;		//LBA of first FAT
int lba_fat2;		//LBA of second FAT
int lba_root;		//LBA of root directory
int lba_data;		//LBA of data start

int tot_clust;
int clid_root;

JX2R_TKFAT_MBR *mbr;
JX2R_TKFAT_FAT16_Boot *boot16;
JX2R_TKFAT_FAT32_Boot *boot32;

u32 sbc_lba[64];		//Static Buffer, LBA
void *sbc_buf[64];		//Static Buffer, Data
short sbc_lbn[256];		//Static Buffer, Number of Blocks
int sbc_num;			//Number of static buffers

u32 tbc_lba[256];		//Temp Buffer, LBA
short tbc_lbn[256];		//Temp Buffer, Number of Blocks
void *tbc_buf[256];		//Temp Buffer, Data
int tbc_num;			//Number of temp buffers

char *exp_iname[256];
char *exp_ename[256];
int exp_cnt;

char *seg_base;
//BJX2_FILE *seg_fd[256];
//u32 seg_id[256];
JX2R_TKFAT_SegmentInfo *seg[256];
int seg_n;

int cl_rov;			//cluster rover
int walk_clid;		//walk starting cluster
int walk_clofs;		//walk cluster offset
int walk_clcur;		//walk cluster current
};

struct JX2R_TKFAT_SegmentInfo_s {
BJX2_FILE *fd;
u32 id;
u32 idx[256];
int nidx;
};

struct JX2R_TKFAT_FAT_DirEntExt_s {
JX2R_TKFAT_FAT_DirEnt deb;	//basic dirent
JX2R_TKFAT_ImageInfo *img;
int clid;				//cluster ID of parent directory
int idx;				//index within directory
byte de_name[512];		//name (UTF-8, LFN)
byte de_aname[512];		//name (UTF-8, LFN)
};

struct JX2R_TKFAT_FAT_DirInfo_s {
JX2R_TKFAT_ImageInfo *img;
int clid;			//cluster ID of current directory
};

int JX2R_TKFAT_ReadSectors(JX2R_TKFAT_ImageInfo *img,
	byte *buf, s64 lba, int num);
int JX2R_TKFAT_WriteSectors(JX2R_TKFAT_ImageInfo *img,
	byte *buf, s64 lba, int num);

int JX2R_TKFAT_SetupDirEntNewDirectory(
	JX2R_TKFAT_FAT_DirEntExt *dee);
