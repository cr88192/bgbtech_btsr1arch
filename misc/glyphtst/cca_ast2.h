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
#define BCCX_ZTY_REGION		48	//Regional Scope
#define BCCX_ZTY_REDUCE		64	//Expression Reduction

// #define BCCX_NODE_FIXATTR		6
#define BCCX_NODE_FIXATTR		8
#define BCCX_NODE_FESCATTR		12

typedef struct BCCX_Node_s			BCCX_Node;
typedef struct BCCX_NodeList_s		BCCX_NodeList;
typedef union BCCX_AttrVal_u		BCCX_AttrVal;

typedef struct BCCX_PrintStream_s	*BCCX_PrintStream;
typedef struct BCCX_ReadStream_s	*BCCX_ReadStream;

typedef struct BCCX_ParseAbxe_s	BCCX_ParseAbxe;

typedef void *BCCX_ReadPos;

typedef int bccx_cxstate;

struct BCCX_Node_s {
BCCX_NodeList	*list;		//list pointer
u16 itag;		//node tag
u16 zrgn;		//zone-tag region
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
u16 zrgn;
void *lidat[16];
};

union BCCX_AttrVal_u {
char *s;
void *p;
s64 i;
double f;
};

struct BCCX_ParseAbxe_s {
int		mru_tag[32];
int		mru_str[32];
byte	rov_tag;
byte	rov_str;

byte	*cs, *cse;
byte	*ct, *cts;
};

void BCCX_FreeNode(BCCX_Node *n);
void BCCX_FreeNodeList(BCCX_NodeList *n);

char *BCCX_Get(BCCX_Node *n, char *var);
char *BCCX_GetCst(BCCX_Node *n,
	bccx_cxstate *rcst, char *var);

s64 BCCX_GetIntCst(BCCX_Node *n, bccx_cxstate *rcst, char *var);
double BCCX_GetFloatCst(BCCX_Node *n, bccx_cxstate *rcst, char *var);
s64 BCCX_GetInt(BCCX_Node *n, char *var);
double BCCX_GetFloat(BCCX_Node *n, char *var);

void BCCX_Set(BCCX_Node *n, char *var, char *val);
void BCCX_SetCst(BCCX_Node *n, bccx_cxstate *rcst, char *var, char *val);

void BCCX_SetIntCst(BCCX_Node *n,
	bccx_cxstate *rcst, char *var, s64 val);
void BCCX_SetFloatCst(BCCX_Node *n,
	bccx_cxstate *rcst, char *var, double val);
void BCCX_SetInt(BCCX_Node *n, char *var, s64 val);
void BCCX_SetFloat(BCCX_Node *n, char *var, double val);

void BCCX_SetTagCst(BCCX_Node *n, bccx_cxstate *rcst, char *str);
void BCCX_SetTag(BCCX_Node *n, char *str);

int BCCX_TagIsCstP(BCCX_Node *n, bccx_cxstate *rcst, char *str);
int BCCX_TagIsP(BCCX_Node *n, char *str);

int BCCX_AttrIsCstP(BCCX_Node *n,
	bccx_cxstate *rcst, char *var, char *val);
int BCCX_TagAttrIsP(BCCX_Node *n, char *tag, char *var, char *val);
int BCCX_AttrIsP(BCCX_Node *n, char *var, char *val);

BCCX_Node *BCCX_GetNodeCst(BCCX_Node *n, bccx_cxstate *rcst, char *var);
void BCCX_SetNodeCst(BCCX_Node *n,
	bccx_cxstate *rcst, char *var, BCCX_Node *val);

BCCX_Node *BCCX_GetNodeIndex(BCCX_Node *n, int idx0);
void BCCX_SetNodeIndex(BCCX_Node *n,
	int idx0, BCCX_Node *val);

BCCX_Node *BCCX_New(char *tag);
BCCX_Node *BCCX_NewCst(bccx_cxstate *rcst, char *tag);
BCCX_Node *BCCX_NewCstIx(bccx_cxstate cst);
BCCX_Node *BCCX_NewText(char *text);
BCCX_Node *BCCX_NewCData(char *text);
BCCX_Node *BCCX_NewList(void);

int BCCX_GetNodeChildCount(BCCX_Node *node);
BCCX_Node *BCCX_AddEnd(BCCX_Node *node, BCCX_Node *child);
BCCX_Node *BCCX_AddEnd2(BCCX_Node *node, BCCX_Node **rne, BCCX_Node *child);
int BCCX_Add(BCCX_Node *node, BCCX_Node *child);
void BCCX_AddV(BCCX_Node *parent, BCCX_Node *child);

BCCX_Node *BCCX_FindAttr(BCCX_Node *node, char *var, char *val);
BCCX_Node *BCCX_Child(BCCX_Node *parent);

BCCX_Node *BCCX_FindTagIx(BCCX_Node *parent, int iv);
BCCX_Node *BCCX_FindTagCst(BCCX_Node *parent, bccx_cxstate *rcst, char *tag);
BCCX_Node *BCCX_FindTag(BCCX_Node *parent, char *tag);
BCCX_Node *BCCX_Fetch(BCCX_Node *parent, char *tag);
BCCX_Node *BCCX_FetchCst(BCCX_Node *parent, bccx_cxstate *rcst, char *tag);

BCCX_Node *BCCX_New1V(char *tag, BCCX_Node *a);
BCCX_Node *BCCX_NewCst1V(bccx_cxstate *rcst, char *tag, BCCX_Node *a);
BCCX_Node *BCCX_New1(char *tag, BCCX_Node *a);

char *BCCX_Tag(BCCX_Node *n);
char *BCCX_Text(BCCX_Node *n);

int BCCX_MarkNodeListZone(BCCX_NodeList *nl, int zone);
int BCCX_MarkAttrListZone(BCCX_Node *n, int zone);
int BCCX_MarkTreeZone(BCCX_Node *n, int zone);

BCCX_Node *BCCX_CloneNodeZone(BCCX_Node *n, int zone);
BCCX_Node *BCCX_Clone(BCCX_Node *n);
BCCX_Node *BCCX_CloneS(BCCX_Node *n);

void BCCX_ClearZoneLevel(int zone);

void BCCX_PrintBuf(BCCX_Node *node, int ind,
	void (*pb)(void *p, char *b), void *ob, int flag);

BCCX_Node *BCCX_ParseExpr(char **strm);

bccx_cxstate bgbcc_rcst_size;
bccx_cxstate bgbcc_rcst_Stext;

#endif
