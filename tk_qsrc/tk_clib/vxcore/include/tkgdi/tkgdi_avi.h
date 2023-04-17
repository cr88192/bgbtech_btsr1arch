#ifndef TKGDI_AVI_H
#define TKGDI_AVI_H

#ifndef RIFF_MAKETAG
#define RIFF_MAKETAG(a, b, c, d)	((a)+((b)<<8)+((c)<<16)+((d)<<24))
#endif

#ifndef RIFF_MAKE8CC
#define RIFF_MAKE8CC(a, b, c, d, e, f, g, h)	\
	(RIFF_MAKETAG(a, b, c, d) + \
	(((long long)RIFF_MAKETAG(e, f, g, h))<<32))
#endif

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

#define TKGDI_FCC_RIFF	RIFF_MAKETAG('R','I','F','F')
#define TKGDI_FCC_AVI	RIFF_MAKETAG('A','V','I',' ')
#define TKGDI_FCC_LIST	RIFF_MAKETAG('L','I','S','T')

#define TKGDI_FCC_hdrl	RIFF_MAKETAG('h','d','r','l')
#define TKGDI_FCC_avih	RIFF_MAKETAG('a','v','i','h')
#define TKGDI_FCC_strl	RIFF_MAKETAG('s','t','r','l')
#define TKGDI_FCC_strh	RIFF_MAKETAG('s','t','r','h')
#define TKGDI_FCC_strf	RIFF_MAKETAG('s','t','r','f')
#define TKGDI_FCC_strd	RIFF_MAKETAG('s','t','r','d')
#define TKGDI_FCC_movi	RIFF_MAKETAG('m','o','v','i')
#define TKGDI_FCC_rec	RIFF_MAKETAG('r','e','c',' ')
#define TKGDI_FCC_idx1	RIFF_MAKETAG('i','d','x','1')

#define TKGDI_FCC_auds	RIFF_MAKETAG('a','u','d','s')
#define TKGDI_FCC_vids	RIFF_MAKETAG('v','i','d','s')
#define TKGDI_FCC_divx	RIFF_MAKETAG('d','i','v','x')
#define TKGDI_FCC_xvid	RIFF_MAKETAG('x','v','i','d')
#define TKGDI_FCC_DIVX	RIFF_MAKETAG('D','I','V','X')
#define TKGDI_FCC_XVID	RIFF_MAKETAG('X','V','I','D')

#define TKGDI_FCC_MJPG	RIFF_MAKETAG('M','J','P','G')
#define TKGDI_FCC_JPEG	RIFF_MAKETAG('J','P','E','G')
#define TKGDI_FCC_GPJM	RIFF_MAKETAG('G','P','J','M')

#define TKGDI_FCC_MBTJ	RIFF_MAKETAG('M','B','T','J')
#define TKGDI_FCC_BTJP	RIFF_MAKETAG('B','T','J','P')

#define TKGDI_FCC_MBTC	RIFF_MAKETAG('M','B','T','C')
#define TKGDI_FCC_BTIC	RIFF_MAKETAG('B','T','I','C')
#define TKGDI_FCC_btic	RIFF_MAKETAG('b','t','i','c')

#define TKGDI_FCC_MPNG	RIFF_MAKETAG('M','B','T','C')
#define TKGDI_FCC_PNG	RIFF_MAKETAG('P','N','G',' ')

#define TKGDI_FCC_rpza	RIFF_MAKETAG('r','p','z','a')
#define TKGDI_FCC_azpr	RIFF_MAKETAG('a','z','p','r')

#define TKGDI_FCC_CRAM	RIFF_MAKETAG('C','R','A','M')

#define TKGDI_FCC_bt1c	RIFF_MAKETAG('b','t','1','c')
#define TKGDI_FCC_c1tb	RIFF_MAKETAG('c','1','t','b')

#define TKGDI_FCC_bt1a	RIFF_MAKETAG('b','t','1','a')
#define TKGDI_FCC_bt1d	RIFF_MAKETAG('b','t','1','d')

#define TKGDI_FCC_THEO	RIFF_MAKETAG('T','H','E','O')
#define TKGDI_FCC_theo	RIFF_MAKETAG('t','h','e','o')
#define TKGDI_FCC_ther	RIFF_MAKETAG('t','h','e','r')

#define TKGDI_FCC_RGBA	RIFF_MAKETAG('B','G','R','A')
#define TKGDI_FCC_RGBX	RIFF_MAKETAG('B','G','R','X')

#define TKGDI_FCC_BGRA	RIFF_MAKETAG('B','G','R','A')
#define TKGDI_FCC_BGRX	RIFF_MAKETAG('B','G','R','X')

#define TKGDI_FCC_RGBA	RIFF_MAKETAG('R','G','B','A')
#define TKGDI_FCC_RGB	RIFF_MAKETAG('R','G','B',' ')
#define TKGDI_FCC_BGRA	RIFF_MAKETAG('B','G','R','A')
#define TKGDI_FCC_BGR	RIFF_MAKETAG('B','G','R',' ')
#define TKGDI_FCC_YUVA	RIFF_MAKETAG('Y','U','V','A')
#define TKGDI_FCC_YUV	RIFF_MAKETAG('Y','U','V',' ')

#define TKGDI_FCC_RGBX	RIFF_MAKETAG('R','G','B','X')
#define TKGDI_FCC_BGRX	RIFF_MAKETAG('B','G','R','X')
#define TKGDI_FCC_YUVX	RIFF_MAKETAG('Y','U','V','X')

#define TKGDI_FCC_DXT1	RIFF_MAKETAG('D','X','T','1')
#define TKGDI_FCC_DXT3	RIFF_MAKETAG('D','X','T','3')
#define TKGDI_FCC_DXT5	RIFF_MAKETAG('D','X','T','5')

#define TKGDI_FCC_BC1	RIFF_MAKETAG('B','C','1',' ')
#define TKGDI_FCC_BC2	RIFF_MAKETAG('B','C','2',' ')
#define TKGDI_FCC_BC3	RIFF_MAKETAG('B','C','3',' ')

#define TKGDI_FCC_BC1F	RIFF_MAKETAG('B','C','1','F')
#define TKGDI_FCC_BC3F	RIFF_MAKETAG('B','C','3','F')
#define TKGDI_FCC_BC1A	RIFF_MAKETAG('B','C','1','A')

#define TKGDI_FCC_BC6H	RIFF_MAKETAG('B','C','6','H')
#define TKGDI_FCC_BC7	RIFF_MAKETAG('B','C','7',' ')

#define TKGDI_FCC_bt1a	RIFF_MAKETAG('b','t','1','a')
#define TKGDI_FCC_bt1c	RIFF_MAKETAG('b','t','1','c')
#define TKGDI_FCC_bt1d	RIFF_MAKETAG('b','t','1','d')

#define TKGDI_FCC_bt2c	RIFF_MAKETAG('b','t','2','c')

#define TKGDI_FCC_UTX2	RIFF_MAKETAG('U','T','X','2')


#define TKGDI_AVI_FL_UPDATING		1

#define TKGDI_AVI_AUID_NONE		0

#define TKGDI_AVI_AUID_MASK_RATE	0x000F
#define TKGDI_AVI_AUID_MASK_BITS	0x00F0
#define TKGDI_AVI_AUID_MASK_CHAN	0x0F00
#define TKGDI_AVI_AUID_MASK_TYPE	0xF000

#define TKGDI_AVI_AUID_8K			1
#define TKGDI_AVI_AUID_11K			2
#define TKGDI_AVI_AUID_16K			3
#define TKGDI_AVI_AUID_22K			4
#define TKGDI_AVI_AUID_32K			5
#define TKGDI_AVI_AUID_44K			6

#define TKGDI_AVI_AUID_8B			0x00
#define TKGDI_AVI_AUID_16B			0x10

#define TKGDI_AVI_AUID_MONO		0x000
#define TKGDI_AVI_AUID_STEREO		0x100

#define TKGDI_AVI_AUID_PCM			0x0000
#define TKGDI_AVI_AUID_ADPCM		0x1000

#define TKGDI_AVI_AUID_8BM		(TKGDI_AVI_AUID_8B|TKGDI_AVI_AUID_MONO)
#define TKGDI_AVI_AUID_16BM	(TKGDI_AVI_AUID_16B|TKGDI_AVI_AUID_MONO)
#define TKGDI_AVI_AUID_8BS		(TKGDI_AVI_AUID_8B|TKGDI_AVI_AUID_STEREO)
#define TKGDI_AVI_AUID_16BS	(TKGDI_AVI_AUID_16B|TKGDI_AVI_AUID_STEREO)


#define TKGDI_AVI_AUID_22K8BM		(TKGDI_AVI_AUID_22K|TKGDI_AVI_AUID_8BM)
#define TKGDI_AVI_AUID_44K8BM		(TKGDI_AVI_AUID_44K|TKGDI_AVI_AUID_8BM)
#define TKGDI_AVI_AUID_22K8BS		(TKGDI_AVI_AUID_22K|TKGDI_AVI_AUID_8BS)
#define TKGDI_AVI_AUID_44K8BS		(TKGDI_AVI_AUID_44K|TKGDI_AVI_AUID_8BS)
#define TKGDI_AVI_AUID_22K16BM		(TKGDI_AVI_AUID_22K|TKGDI_AVI_AUID_16BM)
#define TKGDI_AVI_AUID_44K16BM		(TKGDI_AVI_AUID_44K|TKGDI_AVI_AUID_16BM)
#define TKGDI_AVI_AUID_22K16BS		(TKGDI_AVI_AUID_22K|TKGDI_AVI_AUID_16BS)
#define TKGDI_AVI_AUID_44K16BS		(TKGDI_AVI_AUID_44K|TKGDI_AVI_AUID_16BS)


#define TKGDI_WAVE_FORMAT_PCM			0x0001
#define TKGDI_WAVE_FORMAT_ALAW			0x0006
#define TKGDI_WAVE_FORMAT_ULAW			0x0007
#define TKGDI_WAVE_FORMAT_IMAADPCM		0x0011

#define TKGDI_VIFL_FASTENCODE		1
#define TKGDI_VIFL_FASTDECODE		2

typedef struct TKGDI_VidCodec_s TKGDI_VidCodec;
typedef struct TKGDI_VidCodecCTX_s TKGDI_VidCodecCTX;
typedef struct TKGDI_AVI_Context_s TKGDI_AVI_Context;

typedef struct TKGDI_BITMAPINFOHEADER_s TKGDI_BITMAPINFOHEADER;


typedef struct {
int width;
int height;
s64 frametime;
int num_frames;
int vid_fcc, vid_cmpr;
int aud_fcc, aud_cmpr;
}TKGDI_Video_Stats;

struct TKGDI_VidCodec_s {
TKGDI_VidCodec *next;
int *fcc;	//list of handlers, NULL -> try any

TKGDI_VidCodecCTX *(*begin_decompress)(int fcc,
	TKGDI_BITMAPINFOHEADER *in, TKGDI_BITMAPINFOHEADER *out);
TKGDI_VidCodecCTX *(*begin_compress)(int fcc,
	TKGDI_BITMAPINFOHEADER *in, TKGDI_BITMAPINFOHEADER *out);
int (*decompress_query)(int fcc,
	TKGDI_BITMAPINFOHEADER *in, TKGDI_BITMAPINFOHEADER *out);
int (*compress_query)(int fcc,
	TKGDI_BITMAPINFOHEADER *in, TKGDI_BITMAPINFOHEADER *out);
};

struct TKGDI_VidCodecCTX_s {
void *data;

byte *vidStrd;
byte *audStrd;
int sz_vidStrd;
int sz_audStrd;
int viQuality;
int viNextIFrame;
int viFlags;

int (*decompress_frame)(TKGDI_VidCodecCTX *ctx,
	void *src, void *dst, int ssz, int dsz);
int (*compress_frame)(TKGDI_VidCodecCTX *ctx,
	void *src, void *dst, int dsz, int qfl, int clrs, int *rfl);
int (*end_decompress)(TKGDI_VidCodecCTX *ctx);
int (*end_compress)(TKGDI_VidCodecCTX *ctx);
int (*decompress_frame_clrs)(TKGDI_VidCodecCTX *ctx,
	void *src, void *dst, int ssz, int dsz, int clrs);
};

struct TKGDI_VidLayer_s {
char *tagLayerName;
char *compLayerName;
void *data;
};

typedef struct {
	s32 left;
	s32 top;
	s32 right;
	s32 bottom;
} TKGDI_RECT;

typedef struct {
	s32 xorg;
	s32 yorg;
	s32 xsize;
	s32 ysize;
} TKGDI_RRECT;

typedef struct {
	byte peRed;
	byte peGreen;
	byte peBlue;
	byte peFlags;
} TKGDI_PALETTEENTRY;

typedef struct
{
	u32 dwMicroSecPerFrame;
	u32 dwMaxBytesPerSec;
	u32 dwPaddingGranularity;
	u32 dwFlags;
	u32 dwTotalFrames;
	u32 dwInitialFrames;
	u32 dwStreams;
	u32 dwSuggestedBufferSize;
	u32 dwWidth;
	u32 dwHeight;
	u32 dwReserved[4];
} TKGDI_MainAVIHeader;

typedef struct {
	u32 fccType;
	u32 fccHandler;
	u32 dwFlags;
	u16 wPriority;
	u16 wLanguage;
	u32 dwInitialFrames;
	u32 dwScale;
	u32 dwRate;
	u32 dwStart;
	u32 dwLength;
	u32 dwSuggestedBufferSize;
	u32 dwQuality;
	u32 dwSampleSize;
	TKGDI_RECT rcFrame;
} TKGDI_AVIStreamHeader;

typedef struct {
	u32 ckid;
	u32 dwFlags;
	u32 dwChunkOffset;
	u32 dwChunkLength;
} TKGDI_AVIINDEXENTRY;

typedef struct {
	byte bFirstEntry;
	byte bNumEntries;
	u16 wFlags;
	TKGDI_PALETTEENTRY peNew[1];
} TKGDI_AVIPALCHANGE;

typedef struct {
	u16 wFormatTag;
	u16 nChannels;
	u32 nSamplesPerSec;
	u32 nAvgBytesPerSec;
	u16 nBlockAlign;
	u16 wBitsPerSample;
	u16 cbSize;
} TKGDI_WAVEFORMATEX;

struct TKGDI_BITMAPINFOHEADER_s {
	u32	biSize;
	s32	biWidth;
	s32	biHeight;
	u16	biPlanes;
	u16	biBitCount;
	u32	biCompression;
	u32	biSizeImage;
	s32	biXPelsPerMeter;
	s32	biYPelsPerMeter;
	u32	biClrUsed;
	u32	biClrImportant;
};

typedef struct {
	byte	rgbBlue;
	byte	rgbGreen;
	byte	rgbRed;
	byte	rgbReserved;
} TKGDI_RGBQUAD;

typedef struct {
	TKGDI_BITMAPINFOHEADER bmiHeader;
	TKGDI_RGBQUAD bmiColors[1];
} TKGDI_BITMAPINFO;


struct TKGDI_AVI_Context_s {
TK_FILE *fd;
int ofs_hdrl, ofs_movi, ofs_cur;
TKGDI_MainAVIHeader *avihead;
TKGDI_AVIStreamHeader *strhead, *vidhead, *audhead;
TKGDI_AVIINDEXENTRY *avi_index;
TKGDI_WAVEFORMATEX *wavefmt;
TKGDI_BITMAPINFOHEADER *bmihead;
void *fcbuf, *fdbuf;
int str_vid, str_aud;
int idxsz, frnum, cindex;

void *fdbuf_norm;	//extension: normal map
void *fdbuf_spec;	//extension: specular
void *fdbuf_luma;	//extension: luma

s64 frame_time;
int texnum;
int chroma_color;	//hack to support chroma-key filtering

int texnum_norm;
int texnum_spec;
int texnum_luma;
int mjpg_cty;		//component type
int mjpg_clrs;		//colorspace

TKGDI_VidCodecCTX *codec_ctx;
void *aud_codec_ctx;

int drain;
void *fadbuf;
short *sampbuf, *esampbuf;
int sz_sampbuf;
int sz_fcbuf;

int flags;
};

typedef struct {
//HIC hicd;
void *hicd;
TKGDI_BITMAPINFOHEADER *ihead;
TKGDI_BITMAPINFOHEADER *ohead;
void *buffer;
}vfw_ctxinfo;

typedef struct {
//HIC hicd;
void *hicd;
TKGDI_BITMAPINFOHEADER *ihead;
TKGDI_BITMAPINFOHEADER *ohead;
void *buffer;
}cram_ctxinfo;


#endif
