typedef struct TkMod_SampHead_s	TkMod_SampHead;
typedef struct TkMod_Head2_s		TkMod_Head2;
typedef struct TkMod_Info_s		TkMod_Info;

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

struct TkMod_Info_s {
TkMod_SampHead	samphead[64];
TkMod_Head2		hed2;
byte			modname[20];

byte			*sampbuf;
byte			*patterns;
int				sampofs[64];
byte			*sampdat[64];

s16				*mixbuf;
s16				*mixbufe;
s16				*mixpos;
int				mixrate;

int				row;
int				tick_divmin;	//divisions(rows) per minute
int				tick_bpm;		//beats/minute
int				tick_div;		//ticks per division
int				tick_divsamp;	//samples per division
int				tick_samp;		//samples per tick

int				songlen;		//song length (seconds)

int				sampidx[4];
int				sampvol[4];
int				stepfrac[4];
int				steprate[4];
int				step_fq[4];
int				step_fq_slide[4];
int				step_fq_stgt[4];

int				samp_lbeg[4];
int				samp_lend[4];
byte			samp_isloop[4];

};
