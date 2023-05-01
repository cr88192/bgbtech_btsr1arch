#ifndef __TKGDI_TKGDI_H
#define __TKGDI_TKGDI_H

#include <tk_core.h>
#include <tkgdi/tkgdi_avi.h>

#define TKGDI_SCRMODE_TEXT					0
#define TKGDI_SCRMODE_640x400_CC			1
#define TKGDI_SCRMODE_800x600_CC			2

#define TKGDI_SCRMODE_320x200_RGB555		4
#define TKGDI_SCRMODE_640x400_RGB555		5
#define TKGDI_SCRMODE_640x480_PAL8B			6

#define TKGHDC_NULL		0

#define TKGDI_FCC_crea		RIFF_MAKETAG('c','r','e','a')
#define TKGDI_FCC_resz		RIFF_MAKETAG('r','e','s','z')
#define TKGDI_FCC_move		RIFF_MAKETAG('m','o','v','e')
#define TKGDI_FCC_text		RIFF_MAKETAG('t','e','x','t')
#define TKGDI_FCC_styl		RIFF_MAKETAG('s','t','y','l')

#define TKGDI_FCC_auds		RIFF_MAKETAG('a','u','d','s')

#define TKGDI_FCC_mcmd		RIFF_MAKETAG('m','c','m','d')
#define TKGDI_FCC_msmp		RIFF_MAKETAG('m','s','m','p')
#define TKGDI_FCC_mpat		RIFF_MAKETAG('m','p','a','t')

#define TKGDI_FCC_IWAD		RIFF_MAKETAG('I','W','A','D')
#define TKGDI_FCC_SND_		RIFF_MAKETAG('S','N','D','_')

#define TKGDI_ECC_PATCHIDX	RIFF_MAKE8CC('P','A','T','C','H','I','D','X')

#define TKGDI_BI_RGB		0
#define TKGDI_BI_RLE8		1
#define TKGDI_BI_RLE4		2
#define TKGDI_BI_BITFIELDS	3
#define TKGDI_BI_JPEG		4
#define TKGDI_BI_PNG		5

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


typedef int TKGHDC;		//TKGDI Handle Display Context
typedef int TKGHSND;		//TKGDI Handle Sound Device
typedef int TKGSTATUS;		//TKGDI Error Status

typedef int TKGHFONT;		//TKGDI Handle Font

typedef unsigned int TKGFOURCC;	//FOURCC


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

struct _tkgdi_window_s {
int base_x, base_y;
int size_x, size_y;
char *title;
int		idx;
int		size_bxs;		//size, X cells
int		size_bys;		//size, Y cells
int		size_bmxs;		//bitmap X stride
int		size_bmsz;

byte dirty1;
byte dirty2;

TKGDI_BITMAPINFOHEADER t_buf_info;
TKGDI_BITMAPINFOHEADER *buf_info;
u16		*buf_data;		//backbuffer data
u64		*buf_utx2;		//backbuffer (UTX2)
byte	*buf_dirty1;	//buffer (dirty, pixel data)
byte	*buf_dirty2;	//buffer (dirty, cells since last redraw)
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

#endif
