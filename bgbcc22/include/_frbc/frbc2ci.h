#ifndef FRBC2CI_H
#define FRBC2CI_H

#ifdef FRBC2CI_DLL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#include <wchar.h>

#include <frbc2ci_conf.h>

#ifndef __cplusplus
#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif
#endif

#ifndef UNDEFINED
#define UNDEFINED ((void *)-1)
#endif

typedef struct { u64 v; } fr2ci_variant;

typedef union FR2CI_Value_u FR2CI_Value;

typedef struct FR2CI_Trace_s FR2CI_Trace;
typedef struct FR2CI_Opcode_s FR2CI_Opcode;
typedef struct FR2CI_TailOpcode_s FR2CI_TailOpcode;
typedef struct FR2CI_Frame_s FR2CI_Frame;

typedef struct FR2CI_ArgOpcode_s FR2CI_ArgOpcode;
typedef struct FR2CI_CleanupOpcode_s FR2CI_CleanupOpcode;
typedef struct FR2CI_CallInfo_s FR2CI_CallInfo;

typedef struct FR2CI_LiteralInfo_s FR2CI_LiteralInfo;
typedef struct FR2CI_Function_s FR2CI_Function;
typedef struct FR2CI_Function_s FR2CI_VarInfo;
typedef struct FR2CI_Image_s FR2CI_Image;

typedef struct FR2CI_OpcodeSlab_s FR2CI_OpcodeSlab;
typedef struct FR2CI_TailOpcodeSlab_s FR2CI_TailOpcodeSlab;
typedef struct FR2CI_TraceSlab_s FR2CI_TraceSlab;
typedef struct FR2CI_FrameSlab_s FR2CI_FrameSlab;

typedef struct FR2CI_LoaderCtx_s FR2CI_LoaderCtx;
typedef struct FR2CI_Context_s FR2CI_Context;

/* Union of various primitive types */
union FR2CI_Value_u {
s32 i;
s64 l;
u32 ui;
u64 ul;
f32 f;
f64 d;
void *p;
nlint pi;
unlint upi;
fr2ci_variant va;
struct { int i, j; }ij;
};

struct FR2CI_LiteralInfo_s {
FR2CI_Image *image;			//image
char *name;					//function name
char *sig;					//function signature
char *flagstr;				//flags string
void *litdata;				//literal payload data
byte *tagdata;				//TLV data

u32 tag;					//FOURCC tag

FR2CI_VarInfo **field;
int n_field;
int st_size;				//Struct Size (Memory)
int st_align;				//Struct Alignment (Memory)
int st_fxsize;				//Struct Size (Fixed Form)
};

/* Bytecoded Function */
struct FR2CI_Function_s {
FR2CI_Trace *entry;			//function entry trace
FR2CI_Opcode **ops;			//opcodes array (trace-ordered)
FR2CI_TailOpcode **tops;	//tail opcodes (trace-ordered)
FR2CI_Trace **traces;		//traces
byte *text;					//bytecode
u32 tag;					//FOURCC tag
int n_ops;					//number of opcodes
int n_tops;					//number of tail-opcodes
int n_traces;				//number of traces
int sz_text;				//size of bytecode

FR2CI_Image *image;			//image
char *name;					//function name
char *sig;					//function signature
char *flagstr;				//flags string
char *libname;				//library name
int nlocals;				//number of locals
int nregs;					//number of registers
int nargs;					//number of arguments
int ncargs;					//number of call arguments
int value;					//initialization value (variables)
int vi_offset;				//field offset
int vi_size;				//field size
int vi_align;				//field alignment
int vi_fxoffset;			//field fixed-form offset
int vi_fxsize;				//field fixed-form size

void *natfcn;				//native function pointer
};

struct FR2CI_ArgOpcode_s {
void (*run)(FR2CI_Frame *frm, FR2CI_Frame *frm2, FR2CI_ArgOpcode *op);
int d, s, t;
FR2CI_Value imm;
};

struct FR2CI_CleanupOpcode_s {
FR2CI_CleanupOpcode *next;
void (*run)(FR2CI_Frame *frm, FR2CI_CleanupOpcode *op);
int d, s, t;
FR2CI_Value imm;
};

struct FR2CI_CallInfo_s {
FR2CI_Function *func;
FR2CI_ArgOpcode **oparg;
FR2CI_TailOpcode *vacallop;		//vararg call opcode
short *artag;					//argument type tags
int n_oparg;
int n_arg;
};

struct FR2CI_Trace_s {
FR2CI_Trace *(*run)(FR2CI_Frame *frm, FR2CI_Trace *tr);
FR2CI_Opcode **op;
FR2CI_TailOpcode *top;
byte *ip;
int n_op;
};

struct FR2CI_Opcode_s {
void (*run)(FR2CI_Frame *frm, FR2CI_Opcode *op);
int d, s, t;
FR2CI_Value imm;
};

struct FR2CI_TailOpcode_s {
FR2CI_Trace *(*run)(FR2CI_Frame *frm, FR2CI_TailOpcode *op);
int d, s, t;
FR2CI_Value imm;
FR2CI_Trace *trace;
FR2CI_Trace *nexttrace;
FR2CI_Trace *jmptrace;
byte *jmpip;
};

struct FR2CI_Frame_s {
FR2CI_Value *reg;				//registers
FR2CI_Value *arg;				//arguments
FR2CI_Value *loc;				//locals
FR2CI_Trace *rettrace;			//return trace
FR2CI_Context *ctx;				//owning context
FR2CI_CleanupOpcode *clop;		//cleanup opcode
FR2CI_CallInfo *cinf;			//caller's call-info
FR2CI_Function *func;			//current function
// byte *artag;
int n_reg;
int n_arg;
int n_loc;
};

struct FR2CI_OpcodeSlab_s {
FR2CI_OpcodeSlab *next;
FR2CI_Opcode op[256];
};

struct FR2CI_TailOpcodeSlab_s {
FR2CI_TailOpcodeSlab *next;
FR2CI_TailOpcode op[256];
};

struct FR2CI_TraceSlab_s {
FR2CI_TraceSlab *next;
FR2CI_Trace tr[256];
};

struct FR2CI_FrameSlab_s {
FR2CI_FrameSlab *next;
FR2CI_Frame fr[256];
};

struct FR2CI_Image_s {
FR2CI_Image *next;
FR2CI_LoaderCtx *loader;
char *name;

byte *imgbuf;
int sz_imgbuf;

char *strtab;
byte *wstrtab;
int sz_strtab;
int sz_wstrtab;
u16 *wstrtab2;

byte *lvt4;
byte *lvt8;
int sz_lvt4, sz_lvt8;

byte *gdef, *gidx;
byte *ldef, *lidx;
int sz_gdef, sz_gidx, sr_gidx;
int sz_ldef, sz_lidx, sr_lidx;

FR2CI_Function **gbl_func;
FR2CI_VarInfo **gbl_dvar;
FR2CI_Value *gbl;
int n_gbl;

FR2CI_LiteralInfo **litab;
FR2CI_Value *litv;
int n_lit;

FR2CI_OpcodeSlab *slab_opcode;
FR2CI_TraceSlab *slab_trace;
FR2CI_TailOpcodeSlab *slab_tailopcode;
FR2CI_Opcode *free_opcode;
FR2CI_Trace *free_trace;
FR2CI_TailOpcode *free_tailopcode;

void *free_pool[64];
int status;
};

struct FR2CI_LoaderCtx_s {
// FR2CI_Image *image[256];
// int n_image;
FR2CI_Image *image;
FR2CI_Image *img_main;

byte *(*LoadImageFile)(char *name, int *rsz, int *rfl);
byte *(*ReleaseImageFileBuffer)(byte *buf, int fl);
};

struct FR2CI_Context_s {
// FR2CI_Image *image[256];
// int n_image;
FR2CI_LoaderCtx *loader;

FR2CI_FrameSlab *slab_frame;
FR2CI_Frame *free_frame;

FR2CI_Frame *framestack[1024];
int framestackpos;
FR2CI_Frame *frame;

FR2CI_Trace *nexttrace;		//trace to begin execution
FR2CI_Value retval;
int status;
FR2CI_Trace *trap_trace;		//trace which has thrown
FR2CI_Opcode *trap_opcode;		//trap opcode
FR2CI_TailOpcode *trap_tailop;		//trap opcode

FR2CI_CleanupOpcode *free_clop;

void *free_pool[64];

int stat_n_op;
int stat_n_tr;
};

#include <frbc2ci_frbc.h>
#include <frbc2ci_image.h>

#include <frbc2ci_auto.h>

#endif
