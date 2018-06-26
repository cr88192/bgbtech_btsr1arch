/*
Copyright (C) 2015 by Brendan G Bohannon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "bt1h_conf.h"

#define BT1H_USEHV			//use half-blocks
// #define BT1H_USEGRAD		//use gradient blocks
#define BT1H_USETSKIP		//use translation-skip
// #define BT1H_TSKIP_MAX 4	//translation-skip max value
#define BT1H_TSKIP_MAX 12	//translation-skip max value

// #define BT1H_CHEAPYUV		//use cheaper YUV
// #define BT1H_USEGDBDR		//use GDbDr

// #define BT1H_USECLRPRED		//use color predictors

#define BT1H_ENABLE_AX		//enable alpha extension
#define BT1H_ENABLE_MX		//enable mipmap extension

#define BT1H_BITSTATS

#ifdef linux
#define BT1H_TRAPCRASH
#endif


#ifndef BT1H_TRAPCRASH
#define BT1H_TRAPCRASH		*(int *)-1=-1;
#endif


#define BT1H_DEBUG_TRAPRANGE

typedef unsigned char byte;
typedef signed char sbyte;

typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

typedef unsigned long long u64;
typedef signed long long s64;

#ifdef _MSC_VER
#define force_inline __forceinline
#define default_inline __inline
#endif

#ifdef __GNUC__
#define force_inline inline
#define default_inline inline
#endif

#ifndef force_inline
#define force_inline
#define default_inline
#endif


#define RIFF_MAKETAG(a, b, c, d)	((a)+((b)<<8)+((c)<<16)+((d)<<24))
#define RIFF_UNHEX_NIBBLE(a)		( \
	(((a)>='0')&&((a)<='9'))?((a)-'0') : \
	(((a)>='A')&&((a)<='F'))?((a)-'A'+10) : \
	(((a)>='a')&&((a)<='f'))?((a)-'a'+10) : 0)
#define RIFF_HEX_NIBBLE(a)		(((a)<10)?((a)+'0'):((a)+'A'))

#define RIFF_MAKEHEXTAG(a)	RIFF_MAKETAG( \
					RIFF_HEX_NIBBLE(((a)>>12)&0xf), \
					RIFF_HEX_NIBBLE(((a)>>8)&0xf), \
					RIFF_HEX_NIBBLE(((a)>>4)&0xf), \
					RIFF_HEX_NIBBLE((a)&0xf) \
					)

#define BTIC1H_FCC_RGBA	RIFF_MAKETAG('R','G','B','A')
#define BTIC1H_FCC_RGB	RIFF_MAKETAG('R','G','B',' ')
#define BTIC1H_FCC_BGRA	RIFF_MAKETAG('B','G','R','A')
#define BTIC1H_FCC_BGR	RIFF_MAKETAG('B','G','R',' ')
#define BTIC1H_FCC_YUVA	RIFF_MAKETAG('Y','U','V','A')
#define BTIC1H_FCC_YUV	RIFF_MAKETAG('Y','U','V',' ')

#define BTIC1H_FCC_RGBX	RIFF_MAKETAG('R','G','B','X')
#define BTIC1H_FCC_BGRX	RIFF_MAKETAG('B','G','R','X')
#define BTIC1H_FCC_YUVX	RIFF_MAKETAG('Y','U','V','X')

#define BTIC1H_FCC_BTIC	RIFF_MAKETAG('B','T','I','C')
#define BTIC1H_FCC_bt1h	RIFF_MAKETAG('b','t','1','h')
#define BTIC1H_FCC_bt4b	RIFF_MAKETAG('b','t','4','b')

#define BTIC1H_FCC_YUY2	RIFF_MAKETAG('Y','U','Y','2')
#define BTIC1H_FCC_UYVY	RIFF_MAKETAG('U','Y','V','Y')

#define BTIC1H_FCC_DXT1	RIFF_MAKETAG('D','X','T','1')
#define BTIC1H_FCC_DXT5	RIFF_MAKETAG('D','X','T','5')
#define BTIC1H_FCC_BC1	RIFF_MAKETAG('B','C','1',' ')
#define BTIC1H_FCC_BC3	RIFF_MAKETAG('B','C','3',' ')
#define BTIC1H_FCC_BC7	RIFF_MAKETAG('B','C','7',' ')

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

#ifndef BTIC1H_API
#define BTIC1H_API __declspec(dllexport)
#endif

typedef struct BTIC1H_Context_s BTIC1H_Context;

typedef struct BTIC1H_PTune_s BTIC1H_PTune;

struct BTIC1H_PTune_s {
float dflat;
float d2x2;
float d4x4x1;
float d4x4x2;
float d4x4x3;
float d2x2h;
float d4x4x2h;
float dchflat;

float dflate;
float d2x2e;
float d4x4x1e;
float d4x4x2e;
float d4x4x3e;
float d2x2he;
float d4x4x2he;
float dchflate;

float dyem;
float duvem;
float dyen;
float duven;
float dyeme;
float duveme;
float dyene;
float duvene;

float qyi, quvi, qdyi;
float qyp, quvp, qdyp;

float ebit;
float ebpp;
float ermse;
int parmfl;
};

struct BTIC1H_Context_s {
u32 mark1;

BTIC1H_Context *wqnext;				//next in work queue

//bitstream
byte *bs_ct;						//bitstream output
byte *bs_cts;						//bitstream output start
byte *bs_cte;						//bitstream output end

byte *bs_cs;						//bitstream input
byte *bs_css;						//bitstream input start
byte *bs_cse;						//bitstream input end

u32 bs_win;							//bitstream window
int bs_pos;							//bitstream window position
int bs_bits;						//bitstream total bits

//range coder
u32 rmin;							//range coder min
u32 rmax;							//range coder max
u32 rval;							//range coder value
u32 wctx;							//range coder bit context

//BTEPFS
u32 bfs_win;						//bitfs window
int bfs_pos;						//bitfs window position
int bfs_bits;						//bitfs total bits

byte *bfs_mdl;						//bitfs model buffer
byte *bfs_mdlval;					//bitfs model values
byte *bfs_mdlidx;					//bitfs model index
byte *bfs_mdlrk;					//bitfs model rk


//video stuff

byte *ibuf;							//image buffer
byte *blks;							//blocks buffer
byte *lblks;						//last-frame blocks buffer
byte *blksfl;						//block flags

byte *mtibuf;						//mip temp image buffer

byte cmdwin[256];					//command window
short cmdidx[256];					//command window index
byte cmdwpos;						//command window position

int xs, ys, clrs;					//resolution and colorspace
int xbsz, ybsz;						//image size in blocks
int nblks;							//image blocks count
int qfl;							//quantization and flags

int cy, cu, cv, cd;					//Current YUVD
int qfy, qfuv, qfd;					//Current Quantization Factors
int absyuvbias;						//Absolute YUV Bias

int cdy, cdu, cdv;					//Dyuv
int qfdy, qfduv;					//Dyuf Quantization Factors

int lcy, lcu, lcv, lcd;				//Last YUVD
int lcdy, lcdu, lcdv;				//Last Dyuv

int qfay, qfauv, qfad;				//AbsYUVD Quantization Factors
int qfady, qfaduv;					//AbsDyuv Quantization Factors

byte flip;							//Vertical Flip
byte dyuv;							//DYUV mode
byte updmask;						//YUVD update mask.
byte nextupdmask;					//Next YUVD update mask.
byte tgtupdmask;					//Target YUVD update mask (encode).

byte absupdmask;					//YUVD update mask (absolute).
byte nextabsupdmask;				//Next YUVD update mask (absolute).
byte colorpred;						//Color Prediction Mode
byte reqcolorpred;					//Request Color Prediction Mode

byte clryuv;						//color YUV type
byte clrdty;						//color data type
byte hasax;

u32 mark2;

int fx_qfy, fx_qfuv, fx_qfd;		//Fixed Point Quant Recip (YUVD)
int fx_qfay, fx_qfauv, fx_qfad;		//Fixed Point Quant Recip (AbsYUVD)
int fx_qfdy, fx_qfduv;				//Fixed Point Quant Recip (Dyuv)
int fx_qfady, fx_qfaduv;			//Fixed Point Quant Recip (AbsDyuv)

int rk_cmdidx;						//
int rk_cmdabs;						//
int rk_cmdcnt;						//
int rk_maskidx;						//

int rk_parmxy;						//
int rk_parmvar;						//
int rk_parmval;						//
int rk_parmix;						//

int rk_dy;							//
int rk_duv;							//
int rk_ddy;							//
int rk_dduv;						//

int rk_ay;							//
int rk_auv;							//
int rk_ady;							//
int rk_aduv;						//

int rk_qfy;							//
int rk_qfuv;						//
int rk_qfdy;						//
int rk_qfduv;						//

int bits_total;						//
int cnt_statframes;					//
int cnt_totqfl;						//quality

int cnt_cmds;						//
int bits_cmdidx;					//
int bits_cmdabs;					//

int cnt_parms;						//
int bits_parm;						//

int bits_dumask;					//
int bits_dyuv;						//
int bits_dy;						//
int bits_duv;						//
int bits_ddy;						//
int bits_dduv;						//

int bits_pix4x4;					//
int bits_pix4x4x1;					//
int bits_pix4x4x3;					//
int bits_pix4x2;					//
int bits_pix2x2;					//
int bits_pix2x1;					//

int cnt_dpts;						//count delta points
int cnt_dzpts;						//count zero delta points

int cnt_dcpts[8];					//count delta points
int cnt_dczpts[8];					//count zero delta points
byte maskwin[256];					//mask update window
byte maskidx[256];					//mask update index

int cnt_pred[4];

int stat_cmds[256];

byte lcsim;
byte slscl;
int rc_ctx_raw;
int rc_msk_raw;
byte *rc_mdl_raw;
byte *rc_mdl_rc;

int slys;
int slbs;
int slix;
int sltid;

int (*NextByte)(BTIC1H_Context *ctx);
int (*ReadNBits)(BTIC1H_Context *ctx, int n);
int (*Read8Bits)(BTIC1H_Context *ctx);
int (*Read16Bits)(BTIC1H_Context *ctx);
void (*SkipNBits)(BTIC1H_Context *ctx, int n);
int (*ReadAdRice)(BTIC1H_Context *ctx, int *rk);
int (*ReadAdSRice)(BTIC1H_Context *ctx, int *rk);
int (*ReadAdRiceDc)(BTIC1H_Context *ctx, int *rk);
int (*ReadAdSRiceDc)(BTIC1H_Context *ctx, int *rk);
int (*ReadCommandCode)(BTIC1H_Context *ctx);

void (*WriteNBits)(BTIC1H_Context *ctx, int v, int n);
void (*Write8Bits)(BTIC1H_Context *ctx, int v);
void (*Write16Bits)(BTIC1H_Context *ctx, int v);
void (*WriteAdRice)(BTIC1H_Context *ctx, int v, int *rk);
void (*WriteAdSRice)(BTIC1H_Context *ctx, int v, int *rk);
void (*WriteAdRiceDc)(BTIC1H_Context *ctx, int v, int *rk);
void (*WriteAdSRiceDc)(BTIC1H_Context *ctx, int v, int *rk);
void (*FlushBits)(BTIC1H_Context *ctx);
int (*EmitCommandCode)(BTIC1H_Context *ctx, int cmd);

void (*DecTransColor1)(BTIC1H_Context *ctx,
	int cy, int cu, int cv,
	int *rcr, int *rcg, int *rcb);
void (*DecTransColor1AB)(BTIC1H_Context *ctx,
	int cya, int cua, int cva, int *rcra, int *rcga, int *rcba,
	int cyb, int cub, int cvb, int *rcrb, int *rcgb, int *rcbb);
void (*DecTransColor2)(BTIC1H_Context *ctx,
	int cy0, int cy1, int cu, int cv,
	int *rcr0, int *rcg0, int *rcb0,
	int *rcr1, int *rcg1, int *rcb1);
void (*DecTransColor420)(BTIC1H_Context *ctx,
	int cy0, int cy1, int cy2, int cy3, int cu, int cv,
	int *rcr0, int *rcg0, int *rcb0,
	int *rcr1, int *rcg1, int *rcb1,
	int *rcr2, int *rcg2, int *rcb2,
	int *rcr3, int *rcg3, int *rcb3);

int (*DoWork)(BTIC1H_Context *ctx);

u32 mark3;
};

typedef struct BTIC1H_BMPInfoHeader_s BTIC1H_BMPInfoHeader;
typedef struct BTIC1H_VidCodec_s BTIC1H_VidCodec;
typedef struct BTIC1H_VidCodecCTX_s BTIC1H_VidCodecCTX;

struct BTIC1H_BMPInfoHeader_s {
unsigned int biSize;
unsigned int biWidth;
unsigned int biHeight;
unsigned short biPlanes;
unsigned short biBitCount;
unsigned int biCompression;
unsigned int biSizeImage;
unsigned int biXPelsPerMeter;
unsigned int biYPelsPerMeter;
unsigned int biClrUsed;
unsigned int biClrImportant;
};

struct BTIC1H_VidCodec_s {
BTIC1H_VidCodec *next;
int *fcc;	//list of handlers, NULL -> try any

BTIC1H_VidCodecCTX *(*begin_decompress)(int fcc,
	BTIC1H_BMPInfoHeader *in, BTIC1H_BMPInfoHeader *out);
BTIC1H_VidCodecCTX *(*begin_compress)(int fcc,
	BTIC1H_BMPInfoHeader *in, BTIC1H_BMPInfoHeader *out);
int (*decompress_query)(int fcc,
	BTIC1H_BMPInfoHeader *in, BTIC1H_BMPInfoHeader *out);
int (*compress_query)(int fcc,
	BTIC1H_BMPInfoHeader *in, BTIC1H_BMPInfoHeader *out);
};

struct BTIC1H_VidCodecCTX_s {
void *data;

byte *vidStrd;
byte *audStrd;
int sz_vidStrd;
int sz_audStrd;
int viQuality;
int viNextIFrame;
int viFlags;

int viFrameAvg;
int viRunQuality;

int (*decompress_frame)(BTIC1H_VidCodecCTX *ctx,
	void *src, void *dst, int ssz, int dsz);
int (*compress_frame)(BTIC1H_VidCodecCTX *ctx,
	void *src, void *dst, int dsz, int qfl, int clrs, int *rfl);
int (*end_decompress)(BTIC1H_VidCodecCTX *ctx);
int (*end_compress)(BTIC1H_VidCodecCTX *ctx);
int (*decompress_frame_clrs)(BTIC1H_VidCodecCTX *ctx,
	void *src, void *dst, int ssz, int dsz, int clrs);
};

void BTIC1H_AccumEncodeStats(BTIC1H_Context *ctx, BTIC1H_Context *ctx1);

void btjpg_puts(char *str);
