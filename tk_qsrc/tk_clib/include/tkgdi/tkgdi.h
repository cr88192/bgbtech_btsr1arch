#ifndef __TKGDI_TKGDI_H
#define __TKGDI_TKGDI_H

#include <stdio.h>
#include <stdlib.h>

#include <tk_core.h>
#include <tkgdi/tkgdi_avi.h>
#include <tkgdi/tkgdi_gtf.h>

#define TKGDI_SCRMODE_TEXT					0
#define TKGDI_SCRMODE_640x400_CC			1
#define TKGDI_SCRMODE_800x600_CC			2
#define TKGDI_SCRMODE_1024x768_M			3

#define TKGDI_SCRMODE_320x200_RGB555		4
#define TKGDI_SCRMODE_640x400_RGB555		5

#define TKGDI_SCRMODE_320x200_RGB555_LFB	6

#define TKGDI_SCRMODE_640x480_PAL8B			8
#define TKGDI_SCRMODE_640x400_PAL8B			9
#define TKGDI_SCRMODE_800x600_PAL8B			10

#define TKGHDC_NULL		0

#define TKGDI_FCC_4xSpace	RIFF_MAKETAG(' ',' ',' ',' ')

#define TKGDI_FCC_crea		RIFF_MAKETAG('c','r','e','a')
#define TKGDI_FCC_resz		RIFF_MAKETAG('r','e','s','z')
#define TKGDI_FCC_move		RIFF_MAKETAG('m','o','v','e')
#define TKGDI_FCC_text		RIFF_MAKETAG('t','e','x','t')
#define TKGDI_FCC_styl		RIFF_MAKETAG('s','t','y','l')

#define TKGDI_FCC_mapf		RIFF_MAKETAG('m','a','p','f')
#define TKGDI_FCC_umap		RIFF_MAKETAG('u','m','a','p')
#define TKGDI_FCC_flip		RIFF_MAKETAG('f','l','i','p')

#define TKGDI_FCC_poll		RIFF_MAKETAG('p','o','l','l')
#define TKGDI_FCC_keyb		RIFF_MAKETAG('k','e','y','b')
#define TKGDI_FCC_mous		RIFF_MAKETAG('m','o','u','s')

#define TKGDI_FCC_fcmd		RIFF_MAKETAG('f','c','m','d')

#define TKGDI_FCC_auds		RIFF_MAKETAG('a','u','d','s')

#define TKGDI_FCC_mcmd		RIFF_MAKETAG('m','c','m','d')
#define TKGDI_FCC_msmp		RIFF_MAKETAG('m','s','m','p')
#define TKGDI_FCC_mpat		RIFF_MAKETAG('m','p','a','t')

#define TKGDI_FCC_IWAD		RIFF_MAKETAG('I','W','A','D')
#define TKGDI_FCC_SND_		RIFF_MAKETAG('S','N','D','_')

#define TKGDI_FCC_BMP		RIFF_MAKETAG('B','M','P',' ')
#define TKGDI_FCC_QOI		RIFF_MAKETAG('Q','O','I',' ')

#define TKGDI_FCC_SpBMP		RIFF_MAKETAG(' ','B','M','P')

#define TKGDI_ECC_PATCHIDX	RIFF_MAKE8CC('P','A','T','C','H','I','D','X')

#define TKGDI_TCC_BM		RIFF_MAKETCC('B', 'M')
#define TKGDI_TCC_qo		RIFF_MAKETCC('q', 'o')
#define TKGDI_TCC_if		RIFF_MAKETCC('i', 'f')
#define TKGDI_TCC_li		RIFF_MAKETCC('l', 'i')
#define TKGDI_TCC_l4		RIFF_MAKETCC('l', '4')

#define TKGDI_FCC_hdr		RIFF_MAKETAG('h','d','r',' ')
#define TKGDI_FCC_hdru		RIFF_MAKETAG('h','d','r','u')


#define TKGDI_BI_RGB		0
#define TKGDI_BI_RLE8		1
#define TKGDI_BI_RLE4		2
#define TKGDI_BI_BITFIELDS	3
#define TKGDI_BI_JPEG		4
#define TKGDI_BI_PNG		5

#define TKGDI_BI_HDRU		TKGDI_FCC_hdru

#define TKGDI_DSTR_PX			0x000000FF	//Font size in pixels
#define TKGDI_DSTR_8PX			0x00000008	//8px
#define TKGDI_DSTR_9PX			0x00000009	//9px

#define TKGDI_DSTR_FG_BLACK			0x00000000	//Black Text
#define TKGDI_DSTR_FG_BLUE			0x00000100	//
#define TKGDI_DSTR_FG_GREEN			0x00000200	//
#define TKGDI_DSTR_FG_CYAN			0x00000300	//
#define TKGDI_DSTR_FG_RED			0x00000400	//
#define TKGDI_DSTR_FG_VIOLET		0x00000500	//
#define TKGDI_DSTR_FG_YELLOW		0x00000600	//
#define TKGDI_DSTR_FG_LIGHTGRAY		0x00000700	//
#define TKGDI_DSTR_FG_DARKGRAY		0x00000800	//
#define TKGDI_DSTR_FG_BLUE_HI		0x00000900	//
#define TKGDI_DSTR_FG_GREEN_HI		0x00000A00	//
#define TKGDI_DSTR_FG_CYAN_HI		0x00000B00	//
#define TKGDI_DSTR_FG_RED_HI		0x00000C00	//
#define TKGDI_DSTR_FG_VIOLET_HI		0x00000D00	//
#define TKGDI_DSTR_FG_YELLOW_HI		0x00000E00	//
#define TKGDI_DSTR_FG_WHITE			0x00000F00	//

#define TKGDI_DSTR_BG_BLACK			0x00000000	//Black Background
#define TKGDI_DSTR_BG_BLUE			0x00000100	//
#define TKGDI_DSTR_BG_GREEN			0x00000200	//
#define TKGDI_DSTR_BG_CYAN			0x00000300	//
#define TKGDI_DSTR_BG_RED			0x00000400	//
#define TKGDI_DSTR_BG_VIOLET		0x00000500	//
#define TKGDI_DSTR_BG_YELLOW		0x00000600	//
#define TKGDI_DSTR_BG_LIGHTGRAY		0x00000700	//
#define TKGDI_DSTR_BG_DARKGRAY		0x00000800	//
#define TKGDI_DSTR_BG_BLUE_HI		0x00000900	//
#define TKGDI_DSTR_BG_GREEN_HI		0x00000A00	//
#define TKGDI_DSTR_BG_CYAN_HI		0x00000B00	//
#define TKGDI_DSTR_BG_RED_HI		0x00000C00	//
#define TKGDI_DSTR_BG_VIOLET_HI		0x00000D00	//
#define TKGDI_DSTR_BG_YELLOW_HI		0x00000E00	//
#define TKGDI_DSTR_BG_WHITE			0x00000F00	//

#define TKGDI_STATUS_OK				0x0000	//

#define TKGDI_STATUS_YES			0x8000	//
#define TKGDI_STATUS_NO				0x8001	//
#define TKGDI_STATUS_FAIL			0x8002	//


#define TKGDI_ICOPX_FF			0xFFFFFFFFFFFFFFFFULL	//
#define TKGDI_ICOPX_00			0x0000000000000000ULL	//
#define TKGDI_ICOPX_X			0x01452911294501FFULL	//
#define TKGDI_ICOPX_DN			0x01010101013D01FFULL	//
#define TKGDI_ICOPX_UP			0x01013D01010101FFULL	//

#define TKGDI_CLR_GRAY50			0x3DEF	//
#define TKGDI_CLR_GRAY25			0x1CE7	//
#define TKGDI_CLR_GRAY75			0x5AD6	//
#define TKGDI_CLR_TITLEORA			0x55AA	//
#define TKGDI_CLR_TITLEBLU			0x01AA	//
#define TKGDI_CLR_TITLEBLUSEL		0x01BF	//

#define TKGDI_CTX_MAGIC				0x12389CD

typedef int TKGHDC;		//TKGDI Handle Display Context
typedef int TKGHSND;		//TKGDI Handle Sound Device
typedef int TKGSTATUS;		//TKGDI Error Status

typedef int TKGHFONT;		//TKGDI Handle Font

typedef unsigned int TKGFOURCC;	//FOURCC

typedef struct TKGDI_EVENT_s TKGDI_EVENT;
typedef struct TKGDI_EVENTBUF_s TKGDI_EVENTBUF;



typedef struct _tkgdi_context_vtable_s _tkgdi_context_vtable_t;
typedef struct _tkgdi_context_s _tkgdi_context_t;

struct _tkgdi_context_vtable_s {
void *resv0;
void *resv1;

void *magic0;

TKGSTATUS (*BlitSubImage)(
	_tkgdi_context_t *ctx,
	TKGHDC dev, int xo_dev, int yo_dev,
	TKGDI_BITMAPINFOHEADER *info, void *data,
	int xo_src, int yo_src, int xs_src, int ys_src);
TKGSTATUS (*QueryDisplay)(
	_tkgdi_context_t *ctx,
	TKGHDC dev,		TKGFOURCC parm,
	void *ifmt,		void *ofmt);
TKGHDC (*CreateDisplay)(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGFOURCC parm,
	TKGDI_BITMAPINFOHEADER *info);
TKGSTATUS (*DestroyDisplay)(
	_tkgdi_context_t *ctx,
	TKGHDC dev);
TKGSTATUS (*ModifyDisplay)(
	_tkgdi_context_t *ctx,
	TKGHDC dev,		TKGFOURCC parm,
	void *ifmt,		void *ofmt);

TKGSTATUS (*DrawString)(
	_tkgdi_context_t *ctx,
	TKGHDC dev, int xo_dev, int yo_dev,
	char *text, TKGHFONT font, long long mode);

void *magic1;

TKGHSND (*CreateAudioDevice)(
	_tkgdi_context_t *ctx,
	TKGHDC dev, TKGFOURCC clz, TKGDI_WAVEFORMATEX *info);
TKGSTATUS (*DestroyAudioDevice)(
	_tkgdi_context_t *ctx,
	TKGHSND dev);
TKGSTATUS (*ModifyAudioDevice)(
	_tkgdi_context_t *ctx,
	TKGHSND dev, TKGFOURCC parm,
	void *info, void *rinfo);
TKGSTATUS (*QueryAudioDevice)(
	_tkgdi_context_t *ctx,
	TKGHSND dev, TKGFOURCC parm,
	void *info, void *rinfo);
TKGSTATUS (*WriteAudioSamples)(
	_tkgdi_context_t *ctx,
	TKGHSND dev, void *buffer, int cnt, int ovcnt);

void *magic2;

TKGSTATUS (*BlitSubImageNew)(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGDI_RRECT *drect,
	TKGDI_BITMAPINFOHEADER *info, void *data,
	TKGDI_RRECT *rect);

void	*pad1;
void	*pad2;
void	*pad3;
void	*pad4;
void	*pad5;
void	*pad6;
void	*pad7;
void	*pad8;
void	*pad9;
void	*pad10;

void *magic3;
};

struct _tkgdi_context_s {
_tkgdi_context_vtable_t *vt;
void *sobj;
};

typedef struct _tkgdi_window_s		_tkgdi_window_t;
typedef struct _tkgdi_conparm_s 	_tkgdi_conparm;

struct _tkgdi_window_s {
int		base_x, base_y;
int		size_x, size_y;
u32		magic0;
char	*title;
int		idx;
int		size_bxs;		//size, X cells
int		size_bys;		//size, Y cells
int		size_bmxs;		//bitmap X stride
int		size_bmsz;
u64		style;

byte dirty1;
byte dirty2;
byte tabactive;

byte cell_xs;
byte cell_ys;

int owner_pid;

TKGDI_BITMAPINFOHEADER t_buf_info;
TKGDI_BITMAPINFOHEADER *buf_info;
u16		*buf_data;		//backbuffer data
u64		*buf_utx2;		//backbuffer (UTX2)
byte	*buf_dirty1;	//buffer (dirty, pixel data)
byte	*buf_dirty2;	//buffer (dirty, cells since last redraw)

_tkgdi_conparm	*con;
TKGDI_EVENTBUF	*msgqueue;
_tkgdi_conparm	*contab[16];
u32		magic1;
};

struct _tkgdi_conparm_s {
u32 text_attr;
u32 text_attr_dfl;

u16 fgclr_555;
u16 bgclr_555;

byte x;
byte y;
byte ena;
byte tabactive;

byte cell_xs;	//width of character cell
byte cell_ys;	//height of character cell
byte dirty;

u64 *conbuf;
u16 *pixbuf;
u64 *conmask;
u64 conrowmask;

int isesc;
int escval0;
int escval1;
int escval2;
int escval3;
};


typedef struct TKGDI_FONT_COMMAND_s TKGDI_FONT_COMMAND;
struct TKGDI_FONT_COMMAND_s {
int fcSize;			//size of structure
int fcCmd;			//font command type
int sName;			//name (as relevant)
int sPath;			//path (as relevant)
int fcFont;			//font handle
int fcChar;			//codepoint
short fcSizeX;		//cell width
short fcSizeY;		//cell height
int fcStyle;		//cell style
};


typedef struct TKGDI_MIDI_COMMAND_s TKGDI_MIDI_COMMAND;
typedef struct TKGDI_MIDI_PATCHINFO_s TKGDI_MIDI_PATCHINFO;

struct TKGDI_MIDI_COMMAND_s {
byte op;	//operation
byte ch;	//channel
byte d0;	//first parameter
byte d1;	//second parameter
u32 u0;		//first large parameter
u32 u1;		//second large parameter
};

struct TKGDI_MIDI_PATCHINFO_s {
	u16 wFormatTag;
	u16 nChannels;
	u32 nSamplesPerSec;
	u32 nAvgBytesPerSec;
	u16 nBlockAlign;
	u16 wBitsPerSample;
	u16 cbSize;
	u16 ptIndex;
	u32 ptLength;
	u32 ptLoopBeg;
	u32 ptLoopEnd;
	u16 ptRefHz;
	u16 ptMinHz;
	u16 ptMaxHz;
	u16 ptFlags;
};

struct TKGDI_EVENT_s {
	u32 evSize;			//00, size of event structure
	TKGHDC dev;			//04, associated device
	TKGFOURCC fccMsg;	//08, message tag
	u32 ptMsec;			//0C, event time in milliseconds
	u32 wParm1;			//10, first argument
	u32 wParm2;			//14, second argument
	u64 lParm1;			//18, third argument
	u32 ptMsX;			//20, Mouse X (if relevant)
	u32 ptMsY;			//24, Mouse Y (if relevant)
	u32 ptMsB;			//28, Mouse Button
	u32 evSeqNum;		//2C, Event Sequence Number
};

struct TKGDI_EVENTBUF_s {
TKGDI_EVENTBUF *next;
TKGDI_EVENT ev;
u32 magic0;
};


typedef struct TKGDI_Object3D_s TKGDI_Object3D;

struct TKGDI_Object3D_s {
TKGDI_Object3D *next;
char *name;

float *vtxa_xyz;
float *vtxa_ijk;
float *vtxa_st;
float *vtxa_rgba;
byte *vtxa_tag;
int n_vtxa;

int *poly_vidx;
short *poly_vcnt;
byte *poly_mat;
int n_poly;

char **mata;
int n_mat;
};


void *tkgGlobalAlloc(size_t size);
TKGSTATUS tkgGlobalFree(void *obj);

/* Blit an image to Display Device.
 * The info field gives the image format.
 *   Should be compatible with the target.
 *  xo_dev / yo_dev give the offsets on the device.
 */
TKGSTATUS tkgBlitImage(TKGHDC dev, int xo_dev, int yo_dev,
	TKGDI_BITMAPINFOHEADER *info, void *data);

/* Blit an image to Display Device.
 * The info field gives the image format.
 *   Should be compatible with the target.
 * xo_dev / yo_dev
 *   Give the offsets on the device.
 * xo_src / yo_src / xs_src / ys_src:
 *   Give the offsets and size within the source image.
 */
TKGSTATUS tkgBlitSubImage(TKGHDC dev,
	int xo_dev, int yo_dev,
//	TKGDI_RRECT *drect,
	TKGDI_BITMAPINFOHEADER *info, void *data,
//	TKGDI_RRECT *srect);
	int xo_src, int yo_src, int xs_src, int ys_src);

/* Query Display Device Parameter:
 * dev: Device to handle query.
 * parm: What we are querying.
 * ifmt: Input data for request.
 * ofmt: Output data for request.
 */
TKGSTATUS tkgQueryDisplay(
	TKGHDC dev,		TKGFOURCC parm,
	void *ifmt,		void *ofmt);

TKGSTATUS tkgModifyDisplay(
	TKGHDC dev,		TKGFOURCC parm,
	void *ifmt,		void *ofmt);

/* Query Primary Display Device (Display Creation):
 *   ifmt (if not Null): Requested Format
 *   ofmt (if not Null): Output Format
 * If ifmt is NULL, Fetch the current screen mode.
 * If ofmt is NULL, Return Status based on if ifmt is a valid mode.
 */
TKGSTATUS tkgQueryCreateDisplay(TKGDI_BITMAPINFOHEADER *ifmt,
	TKGDI_BITMAPINFOHEADER *ofmt);

/*
 * Create a Display with the requested mode.
 * Returns TKGHDC_NULL if the display can't be created.
 * If running in a GUI, may create a basic window.
 */
TKGHDC tkgCreateDisplay(TKGDI_BITMAPINFOHEADER *info);

TKGHDC tkgCreateWindow(
	TKGHDC hDev,
	char *title,
	u64 style,
	int x_org,
	int y_org,
	TKGDI_BITMAPINFOHEADER *info);

/*
 * Destroy a previously created display.
 * The handle may not be used once the display is destroyed.
 */
TKGSTATUS tkgDestroyDisplay(TKGHDC dev);

/*
 * Resize a display.
 * Status indicates whether the resizing was successful.
 */
TKGSTATUS tkgResizeDisplay(TKGHDC dev, TKGDI_BITMAPINFOHEADER *info);

/*
 * Set titlebar text for window.
 */
TKGHDC tkgSetWindowTitle(
	TKGHDC hDev,
	char *title);

/*
 * Set style flags for window.
 */
TKGHDC tkgSetWindowStyle(
	TKGHDC hDev,
	u64 style);

/*
 * Move window origin.
 */
TKGHDC tkgMoveWindow(
	TKGHDC hDev,
	int xorg, int yorg);

/*
 * Poll for new event from window or device.
 */
TKGSTATUS tkgPollEvent(TKGHDC dev, TKGDI_EVENT *imsg);

/*
 * Push event to window or device.
 */
TKGSTATUS tkgPushEvent(TKGHDC dev, TKGDI_EVENT *imsg);

void *tkgTryMapFrameBuffer(TKGHDC dev,
	TKGDI_BITMAPINFOHEADER *info);

TKGSTATUS tkgMapReleaseFrameBuffer(TKGHDC dev,
	TKGDI_BITMAPINFOHEADER *info, void *ptr);

TKGSTATUS tkgMapFlipFrame(TKGHDC dev);

/*
 * Draw a text string to the Display.
 * Font encodes a handle for what font to use.
 * Mode describes basic text-drawing parameters.
 */
TKGSTATUS tkgDrawString(TKGHDC dev, int xo_dev, int yo_dev,
	char *text, TKGHFONT font, long long mode);

/*
 * Create an audio mixer output.
 * It is connected to a window context.
 */
TKGHSND tkgCreateAudioDevice(TKGHDC dev,
	TKGFOURCC clz, TKGDI_WAVEFORMATEX *info);

/*
 * Create an audio mixer output.
 * It is connected to a window context.
 */
TKGSTATUS tkgDestroyAudioDevice(TKGHSND dev);

/* Modify Audio Device Parameter:
 * dev: Device to handle query.
 * parm: What we are querying.
 * ifmt: Input data for request.
 * ofmt: Output data for request.
 */
TKGSTATUS tkgModifyAudioDevice(
	TKGHSND dev,	TKGFOURCC parm,
	void *ifmt,		void *ofmt);

/* Send a MIDI command to device.
 */
TKGSTATUS tkgDeviceMidiCommand(
	TKGHSND dev,	TKGDI_MIDI_COMMAND *mcmd);

/* Query Audio Device Parameter:
 * dev: Device to handle query.
 * parm: What we are querying.
 * ifmt: Input data for request.
 * ofmt: Output data for request.
 */
TKGSTATUS tkgQueryAudioDevice(
	TKGHSND dev,	TKGFOURCC parm,
	void *ifmt,		void *ofmt);

/*
 * Send samples to a sound device.
 * buffer: Audio data in format specified for the stream.
 * cnt: Logical number of samples to advance stream;
 * ovcnt: Number of samples including "overmix".
 */
TKGSTATUS tkgWriteSamples(
	TKGHSND dev, void *buffer, int cnt, int ovcnt);


/**
 * Get image parameters from a compressed image buffer.
 *
 * The BITMAPINFOHEADER is to be filled with parameters extracted from the
 * compressed image. The cmpfmt argument will identify the image format.
 *
 * This may return the raw format of the image data, rather than a format
 * desirable for a buffer which can be used to hold the extracted pixels.
 */
TKGSTATUS tkgGetImageInfoFromBuffer(
	void *cmpbuf, int sz_cmpbuf, TKGFOURCC cmpfmt,
	TKGDI_BITMAPINFOHEADER *info);

/**
 * Get image data from a compressed image buffer.
 *
 * The BITMAPINFOHEADER is to be filled with parameters compatible with the
 * compressed image. The cmpfmt argument will identify the image format.
 *
 * The BITMAPINFO header will describe the format of the destination image.
 * If the destination image is NULL, this will return a status for whether the
 * image could be extracted into the requested format.
 */
TKGSTATUS tkgGetImageDataFromBuffer(
	void *cmpbuf, int sz_cmpbuf, TKGFOURCC cmpfmt,
	void *destimage, TKGDI_BITMAPINFOHEADER *info);

#ifndef __BGBCC__
int __hint_use_egpr();
int __int_clamp(int x, int m, int n);
#endif

#endif
