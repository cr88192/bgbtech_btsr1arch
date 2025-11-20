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





#define	BTIC1H_PXF_RGBA			 0	//RGBA(32)
#define	BTIC1H_PXF_RGB			 1	//RGB(24)
#define	BTIC1H_PXF_BGRA			 3	//BGRA(32)
#define	BTIC1H_PXF_BGR			 4	//BGR(24)
#define	BTIC1H_PXF_YUVA			 5	//YUVA(32)
#define	BTIC1H_PXF_YUV			 6	//YUV(24)
#define	BTIC1H_PXF_Y			 7	//Y(8)
#define	BTIC1H_PXF_YA			 8	//YA(16)
#define	BTIC1H_PXF_YUV422		 9	//YYUV
#define	BTIC1H_PXF_YUV420		10	//YYU/YYV (YYYYUV)

#define	BTIC1H_PXF_YYYA			11
#define	BTIC1H_PXF_ARGB			12
#define	BTIC1H_PXF_ABGR			13
#define	BTIC1H_PXF_UYVY			14	//UYVY Ordering (4:2:2)
#define	BTIC1H_PXF_YUYV			15	//YUYV Ordering (4:2:2)

#define	BTIC1H_PXF_BC1			16	//BC1 / DXT1
#define	BTIC1H_PXF_BC2			17	//BC2 / DXT3
#define	BTIC1H_PXF_BC3			18	//BC3 / DXT5
#define	BTIC1H_PXF_BC4			19	//BC4
#define	BTIC1H_PXF_BC5			20	//BC5
#define	BTIC1H_PXF_BC6			21	//BC6H (Signed)
#define	BTIC1H_PXF_BC7			22	//BC7
#define	BTIC1H_PXF_BC1F			23	//BC1 / DXT1
#define	BTIC1H_PXF_BC3F			24	//BC3 / DXT5
#define	BTIC1H_PXF_BC1A			25	//BC1 / DXT1
#define	BTIC1H_PXF_BC3_UVAY		26	//DXT5-UVAY
#define	BTIC1H_PXF_BC7_SRGB		27	//BC7 (SRGBA)
#define	BTIC1H_PXF_BC6_UF16		28	//BC6H (Unsigned)
#define	BTIC1H_PXF_BC4A			29	//BC4 (Alpha Only)

#define	BTIC1H_PXF_RGBA_Q11_4	32	//RGBA(64 bit, fixed Q11.4)
#define	BTIC1H_PXF_RGB_Q11_4	33	//RGB(48 bit, fixed Q11.4)
#define	BTIC1H_PXF_RGBA_F32		34	//RGBA(128 bit, float32)
#define	BTIC1H_PXF_RGB_F32		35	//RGB(96 bit, float32)
#define	BTIC1H_PXF_RGBA_F16		36	//RGBA(64 bit, float16)
#define	BTIC1H_PXF_RGB_F16		37	//RGB(48 bit, float16)
#define	BTIC1H_PXF_RGBA_LS16	38	//RGBA(64 bit, log-scale)
#define	BTIC1H_PXF_RGB_LS16		39	//RGB(48 bit, log-scale)

#define	BTIC1H_PXF_RGBA_VF		40	//RGBA (Vertical Flip)
#define	BTIC1H_PXF_RGB_VF		41	//RGB (Vertical Flip)
#define	BTIC1H_PXF_RGBA_F16_VF	42	//RGBA(64 bit, float16)
#define	BTIC1H_PXF_RGB_F16_VF	43	//RGB(48 bit, float16)

#define	BTIC1H_PXF_RGBX			44	//RGBx(32)
#define	BTIC1H_PXF_BGRX			45	//BGRx(32)
#define	BTIC1H_PXF_RGBX_VF		46	//RGBx(32)
#define	BTIC1H_PXF_BGRX_VF		47	//BGRx(32)

#define	BTIC1H_PXF_BC1_VF		48	//BC1 / DXT1 (Vertical Flip)
#define	BTIC1H_PXF_BC2_VF		49	//BC2 / DXT3 (Vertical Flip)
#define	BTIC1H_PXF_BC3_VF		50	//BC3 / DXT5 (Vertical Flip)
#define	BTIC1H_PXF_BC4_VF		51	//BC4 (Vertical Flip)
#define	BTIC1H_PXF_BC5_VF		52	//BC5 (Vertical Flip)
#define	BTIC1H_PXF_BC6_VF		53	//BC6H (Signed) (Vertical Flip)
#define	BTIC1H_PXF_BC7_VF		54	//BC7 (Vertical Flip)
#define	BTIC1H_PXF_BC6_VF2		55	//BC6H (Signed) (Vertical Flip)
#define	BTIC1H_PXF_BC7_VF2		56	//BC7 (Vertical Flip)

#define	BTIC1H_PXF_MB5C			57	//MB5C
#define	BTIC1H_PXF_MB5C_VF		58	//BC7 (Vertical Flip)

#define	BTIC1H_PXF_BC6_UVF		60	//BC6H (Unsigned)

#define	BTIC1H_PXF_RGBX_F16		62	//RGBx(32)
#define	BTIC1H_PXF_BGRX_F16		63	//BGRx(32)

#define	BTIC1H_PXF_RGB8E8		64	//RGBE
#define	BTIC1H_PXF_RGB9E5		65	//RGBE
#define	BTIC1H_PXF_RG11B10		66	//R11G11B10

// #define	BTIC1H_QFL_IFRAME			1048576
// #define	BTIC1H_QFL_PFRAME			2097152

#define	BTIC1H_QFL_IFRAME		(1<< 8)		//I-Frame
#define	BTIC1H_QFL_PFRAME		(1<< 9)		//P-Frame
#define	BTIC1H_QFL_USERC		(1<<10)		//Use Range Coder
#define	BTIC1H_QFL_USERC66		(1<<11)		//Use Range Coder if Q<66%
#define	BTIC1H_QFL_USESLICE		(1<<12)		//Use Slice Coding
#define	BTIC1H_QFL_USEGDBDR		(1<<13)		//Use Slice Coding

#define	BTIC1H_QFL_DBGPTUNE		(1<<14)		//Parameter Tuning
#define	BTIC1H_QFL_USEMIP		(1<<15)		//Use Mipmaps
#define	BTIC1H_QFL_ISMIPLVL		(1<<16)		//Encoding a mipmap level

#define	BTIC1H_DBFL_CLEARSKIP	(1<< 8)		//I-Frame

#define	BTIC1H_PTFL_BLKENC		(1<< 0)		//Tune Block Encoding
#define	BTIC1H_PTFL_BLKSKIP		(1<< 1)		//Tune Block Skipping
#define	BTIC1H_PTFL_LQUANTI		(1<< 2)		//Tune Linear Quantization (I)
#define	BTIC1H_PTFL_LQUANTP		(1<< 3)		//Tune Linear Quantization (P)
#define	BTIC1H_PTFL_LQUANT		(3<< 2)		//Tune Linear Quantization (IP)


typedef struct PDJPG_Context_s PDJPG_Context;

struct PDJPG_Context_s {
PDJPG_Context *next;	//next context

byte *huff_cs;			//current pos in bitstream (input)
byte *huff_ct;			//current pos in bitstream (output)
u32 huff_win;			//bitstream window
int huff_pos;			//bitstream offset
int huff_isend;			//bitstream has broken (decoder)

u16 huff_code[8*256];
u16 huff_mask[8*256];	//huffman code masks
s16 huff_next[8*256];	//huffman code next
byte huff_len[8*256];
s16 huff_idx[8*256];

byte jpg_qt[4][64];

int xs, ys;
int lxs, lys;
byte jpg_cid[16];
byte jpg_ch[16];
byte jpg_cv[16];
byte jpg_qid[16];
int jpg_cxi[16];
int jpg_cyi[16];
int jpg_nc;
int jpg_chm;
int jpg_chn;

int jpg_scid[4];
int jpg_scn[4];
byte *jpg_sibuf[4];
byte *jpg_sabuf;
int jpg_sisize;

int jpg_schs[4];		//scan horizontal size
int jpg_scvs[4];		//scan vertical size

int jpg_schsci[4];		//scan horizontal scale
int jpg_scvsci[4];		//scan vertical scale

byte jpg_is420;			//image is 420
byte jpg_is444;			//image is 444
byte jpg_rdct;			//image uses RDCT

//encoder
byte *yb, *ub, *vb;
s16 *ydb, *udb, *vdb;
int jpg_qtfp[4][64];
byte jpg_mono;
int qfl;

int dcs[256];
int acs[256];
int dcsuv[256];
int acsuv[256];
};

