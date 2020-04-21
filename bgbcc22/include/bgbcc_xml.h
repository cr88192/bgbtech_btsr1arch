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

#define BCCX_NTY_DEFAULT		0	//default/unknown
#define BCCX_NTY_BASIC			1	//basic XML node
#define BCCX_NTY_TEXT			2	//text node
#define BCCX_NTY_SPECIAL		3	//special node
#define BCCX_NTY_TRANS			4	//transforms into attribute

typedef struct BCCX_Node_s BCCX_Node;

typedef union BCCX_AttrVal_u BCCX_AttrVal;

typedef struct BCCX_PrintStream_s *BCCX_PrintStream;
typedef struct BCCX_ReadStream_s *BCCX_ReadStream;

typedef void *BCCX_ReadPos;

typedef int bccx_cxstate;

union BCCX_AttrVal_u {
char *s;
void *p;
s64 i;
double f;
};

#define BCCX_NODE_FIXATTR		6
#define BCCX_NODE_FESCATTR		12

#define BCCX_NODE_MAGIC			0x40DE

struct BCCX_Node_s {
BCCX_Node *next;
BCCX_Node *prev;
BCCX_Node *up;
BCCX_Node *down;
BCCX_Node *down_end;
BCCX_Node *hnext;		//hash next
u16	magic;
u16 itag;
byte nattr, mattr;
u16 attr_n[BCCX_NODE_FIXATTR];
BCCX_AttrVal attr_v[BCCX_NODE_FIXATTR];
};

struct BCCX_PrintStream_s {
BCCX_Node *sarr;
BCCX_Node *marr;
int n_sarr;
int n_marr;

void *buf;
void (*prints_f)(void *buf, char *str);
int ind;
};

struct BCCX_ReadStream_s {
void *data;
void *(*get_pos_f)(BCCX_ReadStream strm);
void (*set_pos_f)(BCCX_ReadStream strm, void *pos);
int (*peek_char_f)(BCCX_ReadStream strm);
int (*read_char_f)(BCCX_ReadStream strm);
int (*peek_idx_f)(BCCX_ReadStream strm, int idx);
};

#endif
