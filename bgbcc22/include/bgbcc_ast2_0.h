#ifndef BCAST_H
#define BCAST_H

#define BGBDY_TTY_NULL			0
#define BGBDY_TTY_SPECIAL		1
#define BGBDY_TTY_SYMBOL		2
#define BGBDY_TTY_STRING		3

#define BCAST_IVTY_DEFAULT		0
#define BCAST_IVTY_INT			1
#define BCAST_IVTY_REAL			2
#define BCAST_IVTY_NODE			3
#define BCAST_IVTY_STRING		4
#define BCAST_IVTY_LIST			5

#define BCAST_NTY_DEFAULT		0	//default/unknown
#define BCAST_NTY_BASIC			1	//basic XML node
#define BCAST_NTY_TRANS			4	//transforms into attribute

#define BCAST_ZTY_NONE			0	//No Zone assigned
#define BCAST_ZTY_FREED			1

#define BCAST_ZTY_GLOBAL		8
#define BCAST_ZTY_MODULE		32	//Module Scope
#define BCAST_ZTY_REDUCE		64	//Expression Reduction

// #define BCAST_NODE_FIXATTR		6
#define BCAST_NODE_FIXATTR		8
#define BCAST_NODE_FESCATTR		12

typedef struct BCAST_Node_s		BCAST_Node;
typedef struct BCAST_NodeList_s	BCAST_NodeList;
typedef union BCAST_AttrVal_u		BCAST_AttrVal;

typedef int bcast_cxstate;

struct BCAST_Node_s {
BCAST_NodeList	*list;		//list pointer
u16 itag;		//node tag
byte nattr;
// byte mattr;
byte malvl;
byte ztag;		//zone tag
byte lifx;
u16 attr_n[BCAST_NODE_FIXATTR];
u64 attr_v[BCAST_NODE_FIXATTR];
};

struct BCAST_NodeList_s {
int lisz;
byte lilvl;
void *lidat[16];
};

union BCAST_AttrVal_u {
char *s;
void *p;
s64 i;
double f;
};

#endif
