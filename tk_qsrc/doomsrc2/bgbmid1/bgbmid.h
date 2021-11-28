#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

// #include <bgbdy.h>
// #include <libvecmath.h>

#include "bgbmid_conf.h"


#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#ifndef M_E
#define M_E 2.7182818284590452354
#endif

#ifndef M_TAU
#define M_TAU 6.283185307179586476925286766559
#endif

#ifndef BGBMID_H
#define BGBMID_H

//Sample Flags
#define BGBMID_SFL_16BIT			1
#define BGBMID_SFL_UNSIGNED			2
#define BGBMID_SFL_LOOP				4
#define BGBMID_SFL_BIDIRLOOP		8
#define BGBMID_SFL_REVERSE			16
#define BGBMID_SFL_ENVELOPESUST		32
#define BGBMID_SFL_ENVELOPEFILT		64

//BTAC Encoding Flags
#define BGBMID_BTAC_EFL_STEREO		256		//samples are stereo
#define BGBMID_BTAC_EFL_CENTERMONO	512		//encode as centered-mono
#define BGBMID_BTAC_EFL_HALFMONO	1024	//dealing with a single channel
#define BGBMID_BTAC_EFL_MIDSTREAM	4096	//middle-part of a stream
#define BGBMID_BTAC_EFL_MIPMAP		8192	//mipmapped sample

#define BGBMID_BTAC_EBMODE_MASK		0xE000	//encoded block mode
#define BGBMID_BTAC_EBMODE_BLK64	0x0000	//64-sample 256-bit blocks
#define BGBMID_BTAC_EBMODE_BLK256	0x2000	//256-sample 1024-bit blocks
#define BGBMID_BTAC_EBMODE_BLK256LQ	0x4000	//256-sample 512-bit blocks
#define BGBMID_BTAC_EBMODE_BLK256HQ	0x6000	//256-sample 2048-bit blocks

#define BGBMID_SFL_STREAM		0x00010000

#define BGBMID_FCC_MThd	0x4D546864
#define BGBMID_FCC_MTrk	0x4D54726B

//Channel Flags
#define BGBMID_CHFL_LOOP		1	//loop the sample
#define BGBMID_CHFL_POINT		2	//point source
#define BGBMID_CHFL_NOATTN		4	//disable attenuation
#define BGBMID_CHFL_HIGHATTN	8	//rapid falloff
#define BGBMID_CHFL_LOWATTN		16	//gradual falloff
#define BGBMID_CHFL_CALCCURVOL	32	//calculate current volume
#define BGBMID_CHFL_CALCCURDCT	64	//calculate current DCT (low frequency)
#define BGBMID_CHFL_UNLINK		256		//remove sound from mixer

#define BGBMID_SOUND_C			331	//speed of sound in m/s


#define BGBMID_CONTROL_BANK_MSB		0	//
#define BGBMID_CONTROL_MOD_WHEEL	1	//
#define BGBMID_CONTROL_BREATH		2	//

#define BGBMID_CONTROL_FOOT			4	//
#define BGBMID_CONTROL_PORTMENTO	5	//
#define BGBMID_CONTROL_DATA_MSB		6	//
#define BGBMID_CONTROL_VOLUME		7	//
#define BGBMID_CONTROL_BALANCE		8	//

#define BGBMID_CONTROL_PAN			10	//
#define BGBMID_CONTROL_EXPRESSION	11	//
#define BGBMID_CONTROL_EFFECT1		12	//
#define BGBMID_CONTROL_EFFECT2		13	//

#define BGBMID_CONTROL_GENERAL1		16	//
#define BGBMID_CONTROL_GENERAL2		17	//
#define BGBMID_CONTROL_GENERAL3		18	//
#define BGBMID_CONTROL_GENERAL4		19	//

#define BGBMID_CONTROL_BANK_LSB		32	//

#define BGBMID_CONTROL_FLAGS_LSB	108	//
#define BGBMID_CONTROL_FLAGS_MSB	109	//
#define BGBMID_CONTROL_FLAGS2_LSB	110	//
#define BGBMID_CONTROL_FLAGS2_MSB	111	//
#define BGBMID_CONTROL_ORIGIN_3D	112	//
#define BGBMID_CONTROL_VELOCITY_3D	113	//
#define BGBMID_CONTROL_ROTATION_3D	114	//
#define BGBMID_CONTROL_FLAGS		115	//

#define BGBMID_VECTOR_HISTOGRAM				0x1000	//
#define BGBMID_VECTOR_CURRENT_VOLUME		0x1001	//


#ifndef BYTE_T
#define BYTE_T
typedef unsigned char byte;
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct { double x, y; } bmVec2;
typedef struct { double x, y, z; } bmVec3;
typedef struct { double x, y, z, w; } bmVec4;

typedef struct BGBMID_AttParams_s BGBMID_AttParams;

typedef struct BGBMID_Sample_s BGBMID_Sample;
typedef struct BGBMID_Patch_s BGBMID_Patch;
typedef struct BGBMID_EnvFilter_s BGBMID_EnvFilter;
typedef struct BGBMID_EnvVoxel_s BGBMID_EnvVoxel;

typedef struct BGBMID_MixChannel_s BGBMID_MixChannel;
typedef struct BGBMID_Channel_s BGBMID_Channel;
typedef struct BGBMID_ListenerChannel_s BGBMID_ListenerChannel;
typedef struct BGBMID_Listener_s BGBMID_Listener;
typedef struct BGBMID_Context_s BGBMID_Context;

// typedef struct BGBMID_BMSR_Context_s BGBMID_BMSR_Context;
// typedef struct BGBMID_BMSR_Sample_s BGBMID_BMSR_Sample;

struct BGBMID_AttParams_s {
float vol;
float volf;
float pan;
double delay;
double doppler;
};

struct BGBMID_Sample_s {
BGBMID_Sample *next;
BGBMID_Patch *patch;
short *pcm;
byte *btac;
short *pcm_mip[8];
int len;
int rate;
int start, end;
int flags;
float low, high, root;

//BTAC
byte chan;
byte bits;
byte blk_size;
byte blk_samples;
int btac_offs;

//other codecs
void *codecdata;
void (*DecodeBlock)(BGBMID_Sample *self,
	int blkid, s16 *obuf, int flags);
	
//stream
short *pcm_strpos;
};

struct BGBMID_Patch_s {
BGBMID_Patch *next;
BGBMID_Sample *samp;
char *name;
//stream
BGBMID_Patch *str_next;
};

struct BGBMID_EnvVoxel_s {
byte type;	//content type
byte aux;	//high 4: amplitude, low 4: inverse delay
};

struct BGBMID_EnvFilter_s {
float rvMat[256];	//reverb matrix
float rvBase;		//reverb base sample
float rvEffect;

float efMat[256];	//env filter matrix
float efBase;
float efEffect;

float voxTime;
int org[3];
BGBMID_EnvVoxel *vox_fine;
BGBMID_EnvVoxel *vox_coarse;
};


struct BGBMID_MixChannel_s {
BGBMID_MixChannel *next;
// char *name;		//sample/stream name
// int id;			//channel id

// int flags;
// float vol;
int delay;		//delay until sample start

// int curvol;		//volume at last point mixed
// int currvol;	//current absolute volume
//float scale;
//float org[3]; //vol/(scale*|org-cam|)

//multipoint
// int num_points;
// int max_points;	//buffer size
// float points[4*4];	//x y z scale
// float vel[4*4];	//vx vy vz vw

//sound
int offs, len;

//stream
void *data;
int (*GetSamples)(BGBMID_MixChannel *chan, short *mixbuf, int cnt,
	bmVec2 v0, bmVec2 v1);

// int *curdct;	//current mixed DCT block
float hist[16];			//histogram
float cur_absvol;		//current absolute volume (from source)
float cur_relvol_lf;	//current relative volume (from listener, left)
float cur_relvol_rt;	//current relative volume (from listener, right)
};

struct BGBMID_Channel_s {
byte notevel[128];
unsigned short control[32];
int prog;
int id;

int note_strt[128];
int note_end[128];

//spatial mixing
float org[3];		//source origin
float vel[3];		//source velocity
// float qrot[4];		//source rotation (quat)
float rot[9];		//source rotation
float vol;
int flags;

double orgTime;	//time origin was set
double startTime;	//starting time
double endTime;	//ending time

BGBMID_Sample *samp;
BGBMID_MixChannel *mix;		//mixer channel
BGBMID_EnvFilter *env;
int seq;
};

struct BGBMID_ListenerChannel_s {
float *pcmtmp;
int pcmtlen;

float *pcmwin_rv;	//mix buffer window (reverb)
float *pcmwin_ef;	//mix buffer window (effect)
};

struct BGBMID_Listener_s {
BGBMID_ListenerChannel *lchan[1024];

float *pcmbuft;		//temp internal mix buffer
short *pcmbuf;		//main mix buffer
short *pcmbufe;		//current end of main mix buffer
short *pcmbufend;	//end of mix buffer
int pcmbase;		//sample base index (for pcm buffer)

float *pcmbufwin_rv;	//mix buffer window (reverb)
float *pcmbufwin_ef;	//mix buffer window (effect)

short *pcmrqbufe;	//requested end of main mix buffer

double orgTime;
float org[3];		//listener origin
float vel[3];		//listener velocity
float rot[9];		//listener rotation

BGBMID_EnvFilter *env;
};

struct BGBMID_Context_s {
// BGBMID_Sample *samples[128];
// BGBMID_Sample *drumsamples[128];

BGBMID_Patch **patches;
BGBMID_Patch **drumpatches;
int maxpatches;
int maxdrumpatches;

//channels
BGBMID_Channel *chan[1024];
int n_chan;
int m_chan;

//listeners
BGBMID_Listener *listeners[256];
int n_listen;
int m_listen;

//active listener
BGBMID_Listener *listen;

//synth state

//byte notevel[16][128];
//unsigned short control[16][32];
//int prog[16];

//int note_strt[16][128];
//int note_end[16][128];

// float *pcmbuft;		//temp internal mix buffer
// short *pcmbuf;		//main mix buffer
// short *pcmbufe;		//current end of main mix buffer
// short *pcmbufend;	//end of mix buffer
// int pcmbase;		//sample base index (for pcm buffer)

float ticksamp;		//samples per tick (non-int)
float ratesc;		//playback rate scale
float freqsc;		//frequency scale

int noteticks;		//ticks per note
float notelen;		//length of note (seconds)
int samplerate;		//sample rate
int tempsz;		//temp mixbuffer size (samples)

byte *ip;		//current midi buffer pos
byte *ips;		//current midi buffer start
byte *ipe;		//current midi buffer end

int cdt;		//current time (ticks)
int dt;			//delta time (ticks before next op)
int ltag;		//last tag
int not_dt;		//ip does not point at delta

double cnTime;	//current normal time
int sequence;

//mixer channels:
// BGBMID_MixChannel *mixchan;

BGBMID_Patch *streams;		//audio streams

// BGBMID_BMSR_Context *bmsr_ctx;
int *rec_fblk;
int rec_n_fblk;
int rec_fblk_rov;
int rec_bias[64];
};

typedef struct BGBMID_AudioCodecCtx_s BGBMID_AudioCodecCtx;

struct BGBMID_AudioCodecCtx_s {
void *data;

int out_chan;
int out_rate;
int out_bits;

int in_chan;
int in_rate;
int in_bits;

int (*Decode)(BGBMID_AudioCodecCtx *ctx,
	byte *ibuf, int ibsz, short *obuf, int obsz, int *rlen);
int (*Close)(BGBMID_AudioCodecCtx *ctx);
};

// #include <bgbmid_voxel.h>
#include "bgbmid_pack.h"

// #include <bgbmid_btac2.h>
// #include <bgbmid_bsgen.h>
// #include <bgbmid_bmsr.h>

// #include "bgbmid_auto.h"

BMID_API void *bgbmid_malloc(int size);
BMID_API void *bgbmid_tyalloc(char *ty, int size);
BMID_API void bgbmid_free(void *buf);
BMID_API void *bgbmid_realloc(void *buf, int size);

BGBMID_PackState *BGBMID_OpenPack(char *name);
byte *BGBMID_PackReadFile(char *name, int *rsize);

BMID_API BGBMID_Context *BGBMID_NewContext();
BMID_API int BGBMID_DestroyContext(BGBMID_Context *ctx);
BMID_API int BGBMID_LookupSampleName(BGBMID_Context *ctx, char *name);
BMID_API int BGBMID_ChannelStillPlayingP(BGBMID_Context *ctx, int id);

BMID_API int BGBMID_SetFreqScale(BGBMID_Context *ctx, float scale);
BMID_API int BGBMID_SetRateScale(BGBMID_Context *ctx, float scale);
BMID_API int BGBMID_SetNoteLen(BGBMID_Context *ctx, float sec);
BMID_API int BGBMID_SetNoteTicks(BGBMID_Context *ctx, int ticks);
BMID_API int BGBMID_SetOutputSampleRate(BGBMID_Context *ctx, int rate);

BMID_API short *BGBMID_GetOutputBuffer(BGBMID_Context *ctx);
BMID_API int BGBMID_GetOutputBufferPos(BGBMID_Context *ctx);
BMID_API int BGBMID_GetOutputBufferSize(BGBMID_Context *ctx);
BMID_API int BGBMID_GetOutputSampleRate(BGBMID_Context *ctx);
BMID_API int BGBMID_SetOutputBuffer(BGBMID_Context *ctx, short *buf, int len);
BMID_API int BGBMID_SynthStream(BGBMID_Context *ctx, byte *buf, int sz);
BMID_API int BGBMID_SetInputBuffer(BGBMID_Context *ctx, byte *buf, int sz);
BMID_API int BGBMID_BindListener(BGBMID_Context *ctx, int id);
BMID_API int BGBMID_LoadPackGlobal(char *name);

BMID_API void BGBMID_InitChannel(BGBMID_Context *ctx, int num);

BMID_API void BGBMID_Controller4fv(BGBMID_Context *ctx,
	int chan, int num, float *val);

BMID_API int BGBMID_TryDecVLI(byte **rcs, byte *ce);

BMID_API void BGBMID_SetFmRegisterData(BGBMID_Context *ctx,
	int prgn, int idx, u32 val);

#ifdef __cplusplus
}
#endif

#endif
