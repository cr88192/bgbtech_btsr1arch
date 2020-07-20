
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

#define RIFF_TAG_RIFF	RIFF_MAKETAG('R','I','F','F')
#define RIFF_TAG_AVI	RIFF_MAKETAG('A','V','I',' ')
#define RIFF_TAG_LIST	RIFF_MAKETAG('L','I','S','T')

#define RIFF_TAG_hdrl	RIFF_MAKETAG('h','d','r','l')
#define RIFF_TAG_avih	RIFF_MAKETAG('a','v','i','h')
#define RIFF_TAG_strl	RIFF_MAKETAG('s','t','r','l')
#define RIFF_TAG_strh	RIFF_MAKETAG('s','t','r','h')
#define RIFF_TAG_strf	RIFF_MAKETAG('s','t','r','f')
#define RIFF_TAG_strd	RIFF_MAKETAG('s','t','r','d')
#define RIFF_TAG_movi	RIFF_MAKETAG('m','o','v','i')
#define RIFF_TAG_rec	RIFF_MAKETAG('r','e','c',' ')
#define RIFF_TAG_idx1	RIFF_MAKETAG('i','d','x','1')

#define RIFF_TAG_auds	RIFF_MAKETAG('a','u','d','s')
#define RIFF_TAG_vids	RIFF_MAKETAG('v','i','d','s')
#define RIFF_TAG_divx	RIFF_MAKETAG('d','i','v','x')
#define RIFF_TAG_xvid	RIFF_MAKETAG('x','v','i','d')
#define RIFF_TAG_DIVX	RIFF_MAKETAG('D','I','V','X')
#define RIFF_TAG_XVID	RIFF_MAKETAG('X','V','I','D')

#define RIFF_TAG_MJPG	RIFF_MAKETAG('M','J','P','G')
#define RIFF_TAG_JPEG	RIFF_MAKETAG('J','P','E','G')
#define RIFF_TAG_GPJM	RIFF_MAKETAG('G','P','J','M')

#define RIFF_TAG_MBTJ	RIFF_MAKETAG('M','B','T','J')
#define RIFF_TAG_BTJP	RIFF_MAKETAG('B','T','J','P')

#define RIFF_TAG_MBTC	RIFF_MAKETAG('M','B','T','C')
#define RIFF_TAG_BTIC	RIFF_MAKETAG('B','T','I','C')
#define RIFF_TAG_btic	RIFF_MAKETAG('b','t','i','c')

#define RIFF_TAG_MPNG	RIFF_MAKETAG('M','B','T','C')
#define RIFF_TAG_PNG	RIFF_MAKETAG('P','N','G',' ')

#define RIFF_TAG_rpza	RIFF_MAKETAG('r','p','z','a')
#define RIFF_TAG_azpr	RIFF_MAKETAG('a','z','p','r')

#define RIFF_TAG_CRAM	RIFF_MAKETAG('C','R','A','M')

#define RIFF_TAG_bt1c	RIFF_MAKETAG('b','t','1','c')
#define RIFF_TAG_c1tb	RIFF_MAKETAG('c','1','t','b')

#define RIFF_TAG_bt1a	RIFF_MAKETAG('b','t','1','a')
#define RIFF_TAG_bt1d	RIFF_MAKETAG('b','t','1','d')

#define RIFF_TAG_THEO	RIFF_MAKETAG('T','H','E','O')
#define RIFF_TAG_theo	RIFF_MAKETAG('t','h','e','o')
#define RIFF_TAG_ther	RIFF_MAKETAG('t','h','e','r')

#define RIFF_TAG_RGBA	RIFF_MAKETAG('B','G','R','A')
#define RIFF_TAG_RGBX	RIFF_MAKETAG('B','G','R','X')

#define RIFF_TAG_BGRA	RIFF_MAKETAG('B','G','R','A')
#define RIFF_TAG_BGRX	RIFF_MAKETAG('B','G','R','X')

#define BGBBTJ_FCC_RGBA	RIFF_MAKETAG('R','G','B','A')
#define BGBBTJ_FCC_RGB	RIFF_MAKETAG('R','G','B',' ')
#define BGBBTJ_FCC_BGRA	RIFF_MAKETAG('B','G','R','A')
#define BGBBTJ_FCC_BGR	RIFF_MAKETAG('B','G','R',' ')
#define BGBBTJ_FCC_YUVA	RIFF_MAKETAG('Y','U','V','A')
#define BGBBTJ_FCC_YUV	RIFF_MAKETAG('Y','U','V',' ')

#define BGBBTJ_FCC_RGBX	RIFF_MAKETAG('R','G','B','X')
#define BGBBTJ_FCC_BGRX	RIFF_MAKETAG('B','G','R','X')
#define BGBBTJ_FCC_YUVX	RIFF_MAKETAG('Y','U','V','X')

#define BGBBTJ_FCC_DXT1	RIFF_MAKETAG('D','X','T','1')
#define BGBBTJ_FCC_DXT3	RIFF_MAKETAG('D','X','T','3')
#define BGBBTJ_FCC_DXT5	RIFF_MAKETAG('D','X','T','5')

#define BGBBTJ_FCC_BC1	RIFF_MAKETAG('B','C','1',' ')
#define BGBBTJ_FCC_BC2	RIFF_MAKETAG('B','C','2',' ')
#define BGBBTJ_FCC_BC3	RIFF_MAKETAG('B','C','3',' ')

#define BGBBTJ_FCC_BC1F	RIFF_MAKETAG('B','C','1','F')
#define BGBBTJ_FCC_BC3F	RIFF_MAKETAG('B','C','3','F')
#define BGBBTJ_FCC_BC1A	RIFF_MAKETAG('B','C','1','A')

#define BGBBTJ_FCC_BC6H	RIFF_MAKETAG('B','C','6','H')
#define BGBBTJ_FCC_BC7	RIFF_MAKETAG('B','C','7',' ')

#define BGBBTJ_FCC_bt1a	RIFF_MAKETAG('b','t','1','a')
#define BGBBTJ_FCC_bt1c	RIFF_MAKETAG('b','t','1','c')
#define BGBBTJ_FCC_bt1d	RIFF_MAKETAG('b','t','1','d')

#define BGBBTJ_FCC_bt2c	RIFF_MAKETAG('b','t','2','c')


#define BGBBTJ_AVI_FL_UPDATING		1

#define BGBBTJ_AVI_AUID_NONE		0

#define BGBBTJ_AVI_AUID_MASK_RATE	0x000F
#define BGBBTJ_AVI_AUID_MASK_BITS	0x00F0
#define BGBBTJ_AVI_AUID_MASK_CHAN	0x0F00
#define BGBBTJ_AVI_AUID_MASK_TYPE	0xF000

#define BGBBTJ_AVI_AUID_8K			1
#define BGBBTJ_AVI_AUID_11K			2
#define BGBBTJ_AVI_AUID_16K			3
#define BGBBTJ_AVI_AUID_22K			4
#define BGBBTJ_AVI_AUID_32K			5
#define BGBBTJ_AVI_AUID_44K			6

#define BGBBTJ_AVI_AUID_8B			0x00
#define BGBBTJ_AVI_AUID_16B			0x10

#define BGBBTJ_AVI_AUID_MONO		0x000
#define BGBBTJ_AVI_AUID_STEREO		0x100

#define BGBBTJ_AVI_AUID_PCM			0x0000
#define BGBBTJ_AVI_AUID_ADPCM		0x1000

#define BGBBTJ_AVI_AUID_8BM		(BGBBTJ_AVI_AUID_8B|BGBBTJ_AVI_AUID_MONO)
#define BGBBTJ_AVI_AUID_16BM	(BGBBTJ_AVI_AUID_16B|BGBBTJ_AVI_AUID_MONO)
#define BGBBTJ_AVI_AUID_8BS		(BGBBTJ_AVI_AUID_8B|BGBBTJ_AVI_AUID_STEREO)
#define BGBBTJ_AVI_AUID_16BS	(BGBBTJ_AVI_AUID_16B|BGBBTJ_AVI_AUID_STEREO)


#define BGBBTJ_AVI_AUID_22K8BM		(BGBBTJ_AVI_AUID_22K|BGBBTJ_AVI_AUID_8BM)
#define BGBBTJ_AVI_AUID_44K8BM		(BGBBTJ_AVI_AUID_44K|BGBBTJ_AVI_AUID_8BM)
#define BGBBTJ_AVI_AUID_22K8BS		(BGBBTJ_AVI_AUID_22K|BGBBTJ_AVI_AUID_8BS)
#define BGBBTJ_AVI_AUID_44K8BS		(BGBBTJ_AVI_AUID_44K|BGBBTJ_AVI_AUID_8BS)
#define BGBBTJ_AVI_AUID_22K16BM		(BGBBTJ_AVI_AUID_22K|BGBBTJ_AVI_AUID_16BM)
#define BGBBTJ_AVI_AUID_44K16BM		(BGBBTJ_AVI_AUID_44K|BGBBTJ_AVI_AUID_16BM)
#define BGBBTJ_AVI_AUID_22K16BS		(BGBBTJ_AVI_AUID_22K|BGBBTJ_AVI_AUID_16BS)
#define BGBBTJ_AVI_AUID_44K16BS		(BGBBTJ_AVI_AUID_44K|BGBBTJ_AVI_AUID_16BS)


#define BGBBTJ_WAVE_FORMAT_PCM			0x0001
#define BGBBTJ_WAVE_FORMAT_ALAW			0x0006
#define BGBBTJ_WAVE_FORMAT_ULAW			0x0007
#define BGBBTJ_WAVE_FORMAT_IMAADPCM		0x0011

#define BGBBTJ_VIFL_FASTENCODE		1
#define BGBBTJ_VIFL_FASTDECODE		2

typedef struct BGBBTJ_VidCodec_s BGBBTJ_VidCodec;
typedef struct BGBBTJ_VidCodecCTX_s BGBBTJ_VidCodecCTX;
typedef struct BGBBTJ_AVI_Context_s BGBBTJ_AVI_Context;

typedef struct BGBBTJ_BITMAPINFOHEADER_s BGBBTJ_BITMAPINFOHEADER;


typedef struct {
int width;
int height;
s64 frametime;
int num_frames;
int vid_fcc, vid_cmpr;
int aud_fcc, aud_cmpr;
}BGBBTJ_Video_Stats;

struct BGBBTJ_VidCodec_s {
BGBBTJ_VidCodec *next;
int *fcc;	//list of handlers, NULL -> try any

BGBBTJ_VidCodecCTX *(*begin_decompress)(int fcc,
	BGBBTJ_BITMAPINFOHEADER *in, BGBBTJ_BITMAPINFOHEADER *out);
BGBBTJ_VidCodecCTX *(*begin_compress)(int fcc,
	BGBBTJ_BITMAPINFOHEADER *in, BGBBTJ_BITMAPINFOHEADER *out);
int (*decompress_query)(int fcc,
	BGBBTJ_BITMAPINFOHEADER *in, BGBBTJ_BITMAPINFOHEADER *out);
int (*compress_query)(int fcc,
	BGBBTJ_BITMAPINFOHEADER *in, BGBBTJ_BITMAPINFOHEADER *out);
};

struct BGBBTJ_VidCodecCTX_s {
void *data;

byte *vidStrd;
byte *audStrd;
int sz_vidStrd;
int sz_audStrd;
int viQuality;
int viNextIFrame;
int viFlags;

int (*decompress_frame)(BGBBTJ_VidCodecCTX *ctx,
	void *src, void *dst, int ssz, int dsz);
int (*compress_frame)(BGBBTJ_VidCodecCTX *ctx,
	void *src, void *dst, int dsz, int qfl, int clrs, int *rfl);
int (*end_decompress)(BGBBTJ_VidCodecCTX *ctx);
int (*end_compress)(BGBBTJ_VidCodecCTX *ctx);
int (*decompress_frame_clrs)(BGBBTJ_VidCodecCTX *ctx,
	void *src, void *dst, int ssz, int dsz, int clrs);
};

struct BGBBTJ_VidLayer_s {
char *tagLayerName;
char *compLayerName;
void *data;
};

typedef struct {
	s32 left;
	s32 top;
	s32 right;
	s32 bottom;
} BGBBTJ_RECT;

typedef struct {
	byte peRed;
	byte peGreen;
	byte peBlue;
	byte peFlags;
} BGBBTJ_PALETTEENTRY;

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
} BGBBTJ_MainAVIHeader;

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
	BGBBTJ_RECT rcFrame;
} BGBBTJ_AVIStreamHeader;

typedef struct {
	u32 ckid;
	u32 dwFlags;
	u32 dwChunkOffset;
	u32 dwChunkLength;
} BGBBTJ_AVIINDEXENTRY;

typedef struct {
	byte bFirstEntry;
	byte bNumEntries;
	u16 wFlags;
	BGBBTJ_PALETTEENTRY peNew[1];
} BGBBTJ_AVIPALCHANGE;

typedef struct {
	u16 wFormatTag;
	u16 nChannels;
	u32 nSamplesPerSec;
	u32 nAvgBytesPerSec;
	u16 nBlockAlign;
	u16 wBitsPerSample;
	u16 cbSize;
} BGBBTJ_WAVEFORMATEX;

struct BGBBTJ_BITMAPINFOHEADER_s {
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
} BGBBTJ_RGBQUAD;

typedef struct {
	BGBBTJ_BITMAPINFOHEADER bmiHeader;
	BGBBTJ_RGBQUAD bmiColors[1];
} BGBBTJ_BITMAPINFO;


struct BGBBTJ_AVI_Context_s {
TK_FILE *fd;
int ofs_hdrl, ofs_movi, ofs_cur;
BGBBTJ_MainAVIHeader *avihead;
BGBBTJ_AVIStreamHeader *strhead, *vidhead, *audhead;
BGBBTJ_AVIINDEXENTRY *avi_index;
BGBBTJ_WAVEFORMATEX *wavefmt;
BGBBTJ_BITMAPINFOHEADER *bmihead;
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

BGBBTJ_VidCodecCTX *codec_ctx;
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
BGBBTJ_BITMAPINFOHEADER *ihead;
BGBBTJ_BITMAPINFOHEADER *ohead;
void *buffer;
}vfw_ctxinfo;

typedef struct {
//HIC hicd;
void *hicd;
BGBBTJ_BITMAPINFOHEADER *ihead;
BGBBTJ_BITMAPINFOHEADER *ohead;
void *buffer;
}cram_ctxinfo;
