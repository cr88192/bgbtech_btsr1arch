#ifndef BCCX_H
#define BCCX_H

#define BGBDY_TTY_NULL			0
#define BGBDY_TTY_SPECIAL		1
#define BGBDY_TTY_SYMBOL		2
#define BGBDY_TTY_STRING		3

#define BCCX_IVTY_DEFAULT		0
#define BCCX_IVTY_INT			1
#define BCCX_IVTY_REAL			2
#define BCCX_IVTY_NODE			3
#define BCCX_IVTY_STRING		4
#define BCCX_IVTY_LIST			5

#define BCCX_NTY_DEFAULT		0	//default/unknown
#define BCCX_NTY_BASIC			1	//basic XML node
#define BCCX_NTY_TRANS			4	//transforms into attribute

#define BCCX_ZTY_NONE			0	//No Zone assigned
#define BCCX_ZTY_FREED			1

#define BCCX_ZTY_GLOBAL		8
#define BCCX_ZTY_MODULE		32	//Module Scope
#define BCCX_ZTY_REDUCE		64	//Expression Reduction

// #define BCCX_NODE_FIXATTR		6
#define BCCX_NODE_FIXATTR		8
#define BCCX_NODE_FESCATTR		12

typedef struct BCCX_Node_s			BCCX_Node;
typedef struct BCCX_NodeList_s		BCCX_NodeList;
typedef union BCCX_AttrVal_u		BCCX_AttrVal;

typedef struct BCCX_PrintStream_s *BCCX_PrintStream;
typedef struct BCCX_ReadStream_s *BCCX_ReadStream;

typedef void *BCCX_ReadPos;

typedef int bccx_cxstate;

struct BCCX_Node_s {
BCCX_NodeList	*list;		//list pointer
u16 itag;		//node tag
byte nattr;
// byte mattr;
byte malvl;
byte ztag;		//zone tag
byte lifx;
u16 attr_n[BCCX_NODE_FIXATTR];
u64 attr_v[BCCX_NODE_FIXATTR];
};

struct BCCX_NodeList_s {
int lisz;
byte lilvl;
byte ztag;
void *lidat[16];
};

union BCCX_AttrVal_u {
char *s;
void *p;
s64 i;
double f;
};

#endif
