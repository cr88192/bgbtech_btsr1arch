#define BT4B_USETSKIP		//use translation-skip
//#define BT4B_TSKIP_MAX 12	//translation-skip max value
#define BT4B_TSKIP_MAX 8	//translation-skip max value


#if defined(_MSC_VER) || (_M_IX86_FP>=1)
#define BT4B_XMMINTRIN
#include <xmmintrin.h>
#include <emmintrin.h>
#endif

#ifndef BYTE_T
#define BYTE_T
typedef unsigned char byte;
#endif

#ifndef SBYTE_T
#define SBYTE_T
typedef signed char sbyte;
#endif

#ifndef PDLIB_INT_BITS_T
#define PDLIB_INT_BITS_T
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;
#endif

#ifndef UINT_T
#define UINT_T
typedef unsigned int uint;
#endif

#if defined(__x86_64__) || defined(_M_X64)
#ifndef X86_64
#define X86_64
#endif
#endif

#if defined(__i386__) || defined(_M_IX86)
#ifndef X86
#define X86
#endif
#endif

#if defined(__arm__) || defined(_M_ARM)
#ifndef ARM
#define ARM
#endif
#endif

#if defined(__BJX2__)
#ifndef BJX2
#define BJX2
#endif
#endif


#if !defined(LITTLEENDIAN) && !defined(BIGENDIAN)
#if defined(X86) || defined(X86_64) || defined(ARM) || defined(BJX2)
#define LITTLEENDIAN
#endif
#endif

#if defined(_MSC_VER) && defined(BTEIFGL_DLL)
#ifndef BTIC4B_API
#define BTIC4B_API __declspec(dllexport)
#endif
#endif

#if defined(_MSC_VER) && defined(BTIC4B_DLL)
#ifndef BTIC4B_API
#define BTIC4B_API __declspec(dllexport)
#endif
#endif

#ifndef BTIC4B_API
#define BTIC4B_API
#endif

#ifdef _MSC_VER
#define force_inline __forceinline
#define default_inline __inline
#endif

// #ifdef __GNUC__
// #define force_inline inline
// #define default_inline inline
// #endif

#ifndef force_inline
#define force_inline
#define default_inline
#endif

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#ifndef M_TAU
#define M_TAU 6.283185307179586476925286766559
#endif

#define BTIC4B_TWOCC(a, b)			((a)|((b)<<8))
#define BTIC4B_FOURCC(a, b, c, d)	((a)|((b)<<8)|((c)<<16)|((d)<<24))

#define BTIC4B_TCC_I0		BTIC4B_TWOCC('I', '0')
#define BTIC4B_TCC_I1		BTIC4B_TWOCC('I', '1')
#define BTIC4B_TCC_I2		BTIC4B_TWOCC('I', '2')
#define BTIC4B_TCC_I3		BTIC4B_TWOCC('I', '3')

#define BTIC4B_TCC_AX		BTIC4B_TWOCC('A', 'X')
#define BTIC4B_TCC_HX		BTIC4B_TWOCC('H', 'X')

#define BTIC4B_FCC_BT4B		BTIC4B_FOURCC('B', 'T', '4', 'B')

#define BTIC4B_CLRS_RGBA		0x00	//R, G, B, A
#define BTIC4B_CLRS_BGRA		0x01	//B, G, R, A
#define BTIC4B_CLRS_RGBX		0x02	//R, G, B, x
#define BTIC4B_CLRS_BGRX		0x03	//B, G, R, x
#define BTIC4B_CLRS_RGB			0x04	//R, G, B
#define BTIC4B_CLRS_BGR			0x05	//B, G, R
#define BTIC4B_CLRS_RGB11F		0x06	//B10.G11.R11 (UF16)
#define BTIC4B_CLRS_RGB48F		0x07	//RGB48 (F16)
#define BTIC4B_CLRS_BC1			0x08
#define BTIC4B_CLRS_BC3			0x09
#define BTIC4B_CLRS_BC6			0x0A
#define BTIC4B_CLRS_BC7			0x0B
#define BTIC4B_CLRS_BC1MIP		0x0C
#define BTIC4B_CLRS_BC3MIP		0x0D
#define BTIC4B_CLRS_BC6MIP		0x0E
#define BTIC4B_CLRS_BC7MIP		0x0F
#define BTIC4B_CLRS_RGB8E8		0x10	//RGB8_E8
#define BTIC4B_CLRS_BC4			0x11
#define BTIC4B_CLRS_BC5			0x12
#define BTIC4B_CLRS_BC4MIP		0x13
#define BTIC4B_CLRS_BC5MIP		0x14

#define BTIC4B_CLRS_RGB555		0x17

#define BTIC4B_CLRT_GDBDR		0
#define BTIC4B_CLRT_RCT			1
#define BTIC4B_CLRT_YCBCR		2

#define BTIC4B_IMGT_LDR8		0
#define BTIC4B_IMGT_LDR8A		1
#define BTIC4B_IMGT_LDR10		2
#define BTIC4B_IMGT_LDR12		3
#define BTIC4B_IMGT_HDR16		4
#define BTIC4B_IMGT_HDR16A		5
#define BTIC4B_IMGT_HDR12		6

#ifdef _MSC_VER
#define BTIC4B_DBGTRAP		__debugbreak();
#else
#define BTIC4B_DBGTRAP		
#endif

#define BTIC4B_QFL_PFRAME		0x0100
#define BTIC4B_QFL_IFRAME		0x0200
#define BTIC4B_QFL_USEPRED		0x0400
#define BTIC4B_QFL_USEBFQ		0x0800
#define BTIC4B_QFL_OPTBCN		0x1000

#define BTIC4B_ERRS_GENERIC		-1
#define BTIC4B_ERRS_BADFCC		-16
#define BTIC4B_ERRS_BADIBUFSZ	-17
#define BTIC4B_ERRS_NOIMAGE		-18

#if defined(X86)||defined(X86_64)||defined(ARM) || defined(BJX2)
#define btic4b_getu16le(ptr)		(*(u16 *)(ptr))
#define btic4b_getu32le(ptr)		(*(u32 *)(ptr))
#define btic4b_getu64le(ptr)		(*(u64 *)(ptr))
#define btic4b_gets16le(ptr)		(*(s16 *)(ptr))
#define btic4b_gets32le(ptr)		(*(s32 *)(ptr))
#define btic4b_gets64le(ptr)		(*(s64 *)(ptr))
#define btic4b_setu16le(ptr, val)	(*(u16 *)(ptr))=(val)
#define btic4b_setu32le(ptr, val)	(*(u32 *)(ptr))=(val)
#define btic4b_setu64le(ptr, val)	(*(u64 *)(ptr))=(val)
#define btic4b_sets16le(ptr, val)	(*(s16 *)(ptr))=(val)
#define btic4b_sets32le(ptr, val)	(*(s32 *)(ptr))=(val)
#define btic4b_sets64le(ptr, val)	(*(s64 *)(ptr))=(val)

#define btic4b_setu24lef(ptr, val)	(*(u32 *)(ptr))=(val)
default_inline void btic4b_setu24le(byte *ptr, u32 val)
	{ *(u16 *)ptr=val; ptr[2]=val>>16; }

#else

default_inline u16 btic4b_getu16le(byte *ptr)
	{ return(ptr[0]|(ptr[1]<<8)); }
default_inline u32 btic4b_getu32le(byte *ptr)
	{ return(ptr[0]|(ptr[1]<<8)|(ptr[2]<<16)|(ptr[3]<<24)); }
default_inline u64 btic4b_getu64le(byte *ptr)
	{ return(btic4b_getu32le(ptr)|(((u64)btic4b_getu32le(ptr+4))<<32)); }

default_inline s16 btic4b_gets16le(byte *ptr)
	{ return(ptr[0]|(ptr[1]<<8)); }
default_inline s32 btic4b_gets32le(byte *ptr)
	{ return(ptr[0]|(ptr[1]<<8)|(ptr[2]<<16)|(ptr[3]<<24)); }
default_inline s64 btic4b_gets64le(byte *ptr)
	{ return(btic4b_getu32le(ptr)|(((s64)btic4b_gets32le(ptr+4))<<32)); }

default_inline void btic4b_setu16le(byte *ptr, u16 val)
	{ ptr[0]=val; ptr[1]=val>>8; }
default_inline void btic4b_setu32le(byte *ptr, u32 val)
	{ ptr[0]=val; ptr[1]=val>>8; ptr[2]=val>>16; ptr[3]=val>>24; }
default_inline void btic4b_setu64le(byte *ptr, u64 val)
	{ btic4b_setu32le(ptr, val); btic4b_setu32le(ptr+4, val>>32); }

default_inline void btic4b_sets16le(byte *ptr, s16 val)
	{ btic4b_setu16le(ptr, (u16)val); }
default_inline void btic4b_sets32le(byte *ptr, s32 val)
	{ btic4b_setu32le(ptr, (u32)val); }
default_inline void btic4b_sets64le(byte *ptr, s64 val)
	{ btic4b_setu64le(ptr, (u64)val); }

default_inline void btic4b_setu24le(byte *ptr, u32 val)
	{ ptr[0]=val; ptr[1]=val>>8; ptr[2]=val>>16; }
#define btic4b_setu24lef(ptr, val)	btic4b_setu24le(ptr, val)

#endif


#ifdef __BJX2__
// #if 0

#define lqtvq_clamp255(x)	__int_clamp((x), 0, 255)
#define lqtvq_clamp2047(x)	__int_clamp((x), 0, 2047)
#define lqtvq_clamp65535(x)	__int_clamp((x), 0, 65535)

#define lqtvq_clamp63(x)	__int_clamp((x), 0, 63)
#define lqtvq_clamp31(x)	__int_clamp((x), 0, 31)

#define lqtvq_clamp_0_88(x)	__int_clamp((x), 0, 88)
#define lqtvq_clampS16(x)	__int_clamp((x), -32768, 32767)

#else 
force_inline int lqtvq_clamp255(int x)
{
	return((x<0)?0:((x>255)?255:x));
}

force_inline int lqtvq_clamp2047(int x)
{
	return((x<0)?0:((x>2047)?2047:x));
}

force_inline int lqtvq_clamp65535(int x)
{
	return((x<0)?0:((x>65535)?65535:x));
}

force_inline int lqtvq_clamp63(int x)
{
	return((x<0)?0:((x>63)?63:x));
}

force_inline int lqtvq_clamp31(int x)
{
	return((x<0)?0:((x>31)?31:x));
}

force_inline int lqtvq_clampS16(int x)
{
	return((x<(-32768))?(-32768):((x>32767)?32767:x));
}

force_inline int lqtvq_clamp_0_88(int x)
{
	return((x<0)?0:((x>88)?88:x));
}
#endif


typedef struct {
byte tab[256];
byte idx[256];
byte rov;
byte rk;
int cnt;
int bits;
}BTIC4B_SmtfState;

typedef struct {
byte *cs, *cse;
byte *ct, *cte;
u32 win;
int pos;
}BTIC4B_BitStream;

typedef struct BTIC4B_Context_s BTIC4B_Context;

struct BTIC4B_Context_s {
byte *ct, *cs;
u32 bit_win;
int bit_pos;

int xs, ys;
int xsb, ysb, nblk, blksz;
int qfl;

byte *blks;		//current frame blocks
byte *lblks;	//last frame blocks
byte *pblk;		//pixel blocks (mipmap decode)

int cy, cu, cv, dy, du, dv;
int qfy, qfuv, qfdy, qfduv;
int qy, quv, qdy, qduv;

int qdy_flat;
int qdy_2x2x2;
int qdy_4x4x2;
int qdy_8x8x2;
int qdy_8x8x3;
int qduv_flat;
int qduv_2x2;
int qdce_sc;
int qdcea_sc;

byte rk_cy, rk_cuv;
byte rk_dy, rk_duv;
byte rk_cnt, rk_misc;
byte cmask;				//current mask
byte imask;				//ideal mask
byte pred;				//predictor
byte imgt;				//image type
byte clrt;				//colorspace transform
byte pred_l8p;			//predictor needs to deal with LDR8/Skip
byte usegrad;

byte clrs;				//(codec) current default colorspace
byte flip;				//(codec) image needs to be flipped
byte xstr;				//colorspace X stride

BTIC4B_SmtfState sm_cmd;
BTIC4B_SmtfState sm_mask;

int stat_bits;
// int stat_tbits;
int stat_yuvbits;
int stat_yuvcnt;
int stat_yuvcbits[8];
int stat_pixbits;

int yuv_cz[8];

void (*DecUpdatePred)(BTIC4B_Context *ctx,
	byte *ct, byte *blks);
void (*DecGetBlkPredClrs3)(BTIC4B_Context *ctx,
	byte *blka, byte *blkb, byte *blkc, int *rcyuv);

// used during image transform
void (*DecBlock)(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr);
void (*DecBlockHalf)(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr);

u32 (*ClrDec1)(int cy, int cu, int cv);
u32 (*ClrDec1A)(int cy, int cu, int cv, int ca);
void (*ClrDec4)(
	int cy0, int cy1, int cy2, int cy3, int cu, int cv,
	u32 *rpx0, u32 *rpx1);
void (*ClrDec4B)(
	int cy0, int cy1, int cy2, int cy3,
	int cu0, int cv0, int cu1, int cv1,
	u32 *rpx0, u32 *rpx1);
void (*ClrDec4C)(
	int cy0, int cy1, int cy2, int cy3,
	int cu0, int cv0, int cu1, int cv1,
	int cu2, int cv2, int cu3, int cv3,
	u32 *rpx0, u32 *rpx1);
void (*ClrDec2T)(int tag,
	int cy, int cu, int cv,
	int dy, int du, int dv,
	int *rr0, int *rg0, int *rb0,
	int *rr1, int *rg1, int *rb1);

u32 (*ClrDecPack1)(int cr, int cg, int cb);
u32 (*ClrDecPack1A)(int cr, int cg, int cb, int ca);

void (*ClrDecPack4)(
	int cr0, int cg0, int cb0,
	int cr1, int cg1, int cb1,
	int ca, u32 *tab);

//block format query
short bfq_qdy[32];		//dy cutoff
short bfq_qduv[32];		//duv cutoff
byte bfq_rqfl[32];		//require flags
byte bfq_exfl[32];		//exclude flags
byte bfq_cost[32];		//block cost

//BCn transcode
void (*BCnEncodeBlockGen)(byte *block,
	s16 *pxy, s16 *pxa, int *min, int *max,
	int mcy, int ncy, int mca, int nca);
void (*BCnEncodeBlockBits32)(byte *block,
	u32 pxy, u32 pxa, int *min, int *max);
void (*BCnEncodeBlockBits48)(byte *block,
	u64 pxy, u64 pxa, int *min, int *max);
void (*BCnEncodeBlockBits64)(byte *block,
	u64 pxy, int *min, int *max);
void (*BCnEncodeBlockFlat)(byte *block, int *avg);

void (*BCnEncodeBlockBGRA)(byte *block, byte *tpx, int ystr);
};

BTIC4B_API int BTIC4B_DecodeImgBufferCtx(BTIC4B_Context *ctx,
	byte *cbuf, int cbsz, byte *ibuf,
	int xs, int ys, int clrfl);
BTIC4B_API byte *BTIC4B_BufBmpGetImg(byte *buf,
	int *rxs, int *rys,
	u32 *rfcc, int *risz);
BTIC4B_API int BTIC4B_DecodeImgBmpBufferCtx(BTIC4B_Context *ctx,
	byte *cbuf, int cbsz, byte *ibuf,
	int *rxs, int *rys, int clrfl);
BTIC4B_API int BTIC4B_DecodeImgBmpBuffer(byte *cbuf, int cbsz,
	byte *ibuf, int *rxs, int *rys, int clrfl);

BTIC4B_API int BTIC4B_EncodeImgBufferCtx(BTIC4B_Context *ctx,
	byte *obuf, int cbsz, byte *ibuf,
	int xs, int ys, int qfl, int clrs);
BTIC4B_API int BTIC4B_DumpStatsCtx(BTIC4B_Context *ctx);
BTIC4B_API int BTIC4B_EncFlattenBmp(byte *obuf,
	int xs, int ys, int fcc,
	byte *ibuf, int isz);
BTIC4B_API int BTIC4B_EncodeImgBmpBufferCtx(BTIC4B_Context *ctx,
	byte *obuf, int cbsz, byte *ibuf,
	int xs, int ys, int qfl, int clrs);
BTIC4B_API BTIC4B_Context *BTIC4B_AllocContext(void);
BTIC4B_API void BTIC4B_FreeContext(BTIC4B_Context *ctx);
BTIC4B_API int BTIC4B_EncodeImgBmpBuffer(
	byte *obuf, int cbsz, byte *ibuf,
	int xs, int ys, int qfl, int clrs);

void BTIC4B_DecBlockBGRX(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr);
void BTIC4B_DecImageSetupClrsI(BTIC4B_Context *ctx, int clrs);

void BTIC4B_ConvImageBC4n(BTIC4B_Context *ctx,
	byte *iblock, int iblkstr,
	byte *oblock, int xs, int ys);
void BTIC4B_ConvImageBC4nMip(BTIC4B_Context *ctx,
	byte *iblock, int iblkstr,
	byte *oblock, int xs, int ys);

void BTIC4B_ConvImageBC5n(BTIC4B_Context *ctx,
	byte *iblock, int iblkstr,
	byte *oblock, int xs, int ys);
void BTIC4B_ConvImageBC5nMip(BTIC4B_Context *ctx,
	byte *iblock, int iblkstr,
	byte *oblock, int xs, int ys);
