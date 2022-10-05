typedef struct TkMod_SampHead_s	TkMod_SampHead;
typedef struct TkMod_Head2_s		TkMod_Head2;
typedef struct TkMod_Info_s		TkMod_Info;

typedef struct TkMod_S3SampHead_s	TkMod_S3SampHead;
typedef struct TkMod_S3Head_s		TkMod_S3Head;

struct TkMod_Head_s {
byte modname[20];
};

struct TkMod_SampHead_s {
byte sampname[22];		//00
byte samplen[2];		//16
byte finetune;			//18
byte volume;			//19
byte repeat_start[2];	//1A
byte repeat_len[2];		//1C
};

struct TkMod_Head2_s {
byte num_positions;		//00
byte restartpos;		//01
byte pattern_tab[128];	//02
byte sig[4];			//82, "M.K."
};

struct TkMod_S3Head_s {
byte name[28];			//00
byte sig1;				//1C
byte type1;				//1D
byte pad1[2];			//1E
byte orderCnt[2];		//20
byte instrumCnt[2];		//22
byte patternCnt[2];		//24
byte flags[2];			//26
byte version[2];		//28
byte sampFmt[2];		//2A
byte sig2[4];			//2C

byte gblVolume;			//30
byte initSpeed;			//31
byte initTempo;			//32
byte mastVol;			//33
byte gusClickRemove;	//34
byte defPan;			//35
byte pad2[8];			//36
byte ofsSpecial[2];		//3E
byte chanSetting[32];	//40
//60, Orders, Etc
};

struct TkMod_S3SampHead_s {
byte type;				//00
byte dosname[12];		//01
byte rvaDataHi;			//0D
byte rvaDataLo[2];		//0E
byte szDataLen[4];		//10
byte repeat_start[4];	//14
byte repeat_len[4];		//18
byte volume;			//1C
byte pad1;				//1D
byte pack;				//1E
byte flags;				//1F
byte c2spd[4];			//20
byte pad2[12];			//24
byte sampname[28];		//30
byte sig[4];			//4C
};


struct TkMod_Info_s {
u32				magic1;

TkMod_Info		*next;
char			*name;

u32				magic2;

TkMod_SampHead	samphead[64];
TkMod_Head2		hed2;
byte			modname[32];

byte			*sampbuf;
byte			*patterns;
int				sampofs[256];
byte			*sampdat[256];

int				samp_samplen[256];
int				samp_c2hz[256];
byte			samp_volume[256];
u16				samp_repeat_start[256];
u16				samp_repeat_len[256];

byte			chan_remap[32];
byte			n_chan_remap;
byte			colstride;
byte			rowstride;
short			patstride;

u32				rvaSamp[256];
u32				rvaPat[256];
int				nSamp;
int				nPat;

u32				magic3;

s16				*mixbuf;
s16				*mixbufs;
s16				*mixbufe;
s16				*mixpos;
s16				*mixpos_cs;
int				mixrate;

int				row;
int				tick_divmin;	//divisions(rows) per minute
int				tick_bpm;		//beats/minute
int				tick_div;		//ticks per division
int				tick_divsamp;	//samples per division
int				tick_samp;		//samples per tick

int				songlen;		//song length (seconds)

int				sampidx[32];
byte			sampvol[32];
byte			samprefvol[32];
byte			sampeffvol[32];
int				sampc2hz[32];
int				stepfrac[32];
int				steprate[32];
int				step_fq[32];
int				step_fq_slide[32];
int				step_fq_stgt[32];

int				samp_lbeg[32];
int				samp_lend[32];
byte			samp_isloop[32];

u32				magic4;

};


typedef struct TkMod_MixSample_s TkMod_MixSample;

struct TkMod_MixSample_s {
TkMod_MixSample *next;
char *name;
byte *data;
int len;
};
