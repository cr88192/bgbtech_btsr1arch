#define TKUCC_LINEBUF_FIXEDLIM	48
#define TKUCC_ASTNODE_NKEYS		16

#define TKUCC_TOK_NAME			1
#define TKUCC_TOK_NUMBER		2
#define TKUCC_TOK_STRING		3
#define TKUCC_TOK_OPERATOR		4
#define TKUCC_TOK_CHARSTRING	5
#define TKUCC_TOK_OPERATOR_WS	6

#define TKUCC_ASTNODE_KTAG_INT		0x1000
#define TKUCC_ASTNODE_KTAG_STR		0x2000
#define TKUCC_ASTNODE_KTAG_NODE		0x3000
#define TKUCC_ASTNODE_KTAG_REAL		0x4000

#define TKUCC_ASTNODE_KTAG_KMASK	0xF000
#define TKUCC_ASTNODE_KTAG_AMASK	0x0FFF
#define TKUCC_ASTNODE_KTAG_IMASK	0x7FFF

#define TKUCC_ASTNODE_KTAG_NODEIDX	0x8000	//8000..FFFF = Node Arrays

#define TKUCC_ZID_GLOBAL		0
#define TKUCC_ZID_PREPROC		1
#define TKUCC_ZID_PPTEMP		2
#define TKUCC_ZID_TOKSTRM		3
#define TKUCC_ZID_PARSE			4
#define TKUCC_ZID_TAC			5
#define TKUCC_ZID_CGEN			6
#define TKUCC_ZID_ASM			7
#define TKUCC_ZID_LINK			8
#define TKUCC_ZID_NAMESYM		9

#define TKUCC_ATAG_REF			1
#define TKUCC_ATAG_STRING		2
#define TKUCC_ATAG_INT			3
#define TKUCC_ATAG_REAL			4
#define TKUCC_ATAG_BINARY		5
#define TKUCC_ATAG_UNARY		6
#define TKUCC_ATAG_COMPARE		7
#define TKUCC_ATAG_POSTFIX		8
#define TKUCC_ATAG_ASSIGN		9
#define TKUCC_ATAG_CAST			10
#define TKUCC_ATAG_VAR			11
#define TKUCC_ATAG_VARS			12
#define TKUCC_ATAG_LIST			13
#define TKUCC_ATAG_TYPE			14
#define TKUCC_ATAG_CALL			15
#define TKUCC_ATAG_BEGIN		16
#define TKUCC_ATAG_IF			17
#define TKUCC_ATAG_FOR			18
#define TKUCC_ATAG_WHILE		19
#define TKUCC_ATAG_DOWHILE		20

#define TKUCC_ATAG_VALUE		256
#define TKUCC_ATAG_OP			257
#define TKUCC_ATAG_LN			258
#define TKUCC_ATAG_RN			259
#define TKUCC_ATAG_VN			260
#define TKUCC_ATAG_CN			261
#define TKUCC_ATAG_TN			262
#define TKUCC_ATAG_NAME			263
#define TKUCC_ATAG_TYFL			264
#define TKUCC_ATAG_TYN			265
#define TKUCC_ATAG_IND			266
#define TKUCC_ATAG_ARGS			267

#define TKUCC_KW_AUTO			1
#define TKUCC_KW_BREAK			2
#define TKUCC_KW_CASE			3
#define TKUCC_KW_CHAR			4
#define TKUCC_KW_CONST			5
#define TKUCC_KW_CONTINUE		6
#define TKUCC_KW_DEFAULT		7
#define TKUCC_KW_DO				8
#define TKUCC_KW_DOUBLE			9
#define TKUCC_KW_ELSE			10
#define TKUCC_KW_ENUM			11
#define TKUCC_KW_EXTERN			12
#define TKUCC_KW_FLOAT			13
#define TKUCC_KW_FOR			14
#define TKUCC_KW_GOTO			15
#define TKUCC_KW_IF				16
#define TKUCC_KW_INT			17
#define TKUCC_KW_LONG			18
#define TKUCC_KW_REGISTER		19
#define TKUCC_KW_RETURN			20
#define TKUCC_KW_SHORT			21
#define TKUCC_KW_SIGNED			22
#define TKUCC_KW_SIZEOF			23
#define TKUCC_KW_STATIC			24
#define TKUCC_KW_STRUCT			25
#define TKUCC_KW_SWITCH			26
#define TKUCC_KW_TYPEDEF		27
#define TKUCC_KW_UNION			28
#define TKUCC_KW_UNSIGNED		29
#define TKUCC_KW_VOID			30
#define TKUCC_KW_VOLATILE		31
#define TKUCC_KW_WHILE			32


#define TKUCC_TYFL_CHAR			(1ULL<< 0)
#define TKUCC_TYFL_SHORT		(1ULL<< 1)
#define TKUCC_TYFL_INT			(1ULL<< 2)
#define TKUCC_TYFL_LONG			(1ULL<< 3)
#define TKUCC_TYFL_FLOAT		(1ULL<< 4)
#define TKUCC_TYFL_DOUBLE		(1ULL<< 5)
#define TKUCC_TYFL_SIGNED		(1ULL<< 6)
#define TKUCC_TYFL_UNSIGNED		(1ULL<< 7)
#define TKUCC_TYFL_VOID			(1ULL<< 8)
#define TKUCC_TYFL_AUTO			(1ULL<< 9)

#define TKUCC_TYFL_STATIC		(1ULL<<10)
#define TKUCC_TYFL_EXTERN		(1ULL<<11)
#define TKUCC_TYFL_CONST		(1ULL<<12)
#define TKUCC_TYFL_REGISTER		(1ULL<<13)
#define TKUCC_TYFL_STRUCT		(1ULL<<14)
#define TKUCC_TYFL_UNION		(1ULL<<15)
#define TKUCC_TYFL_ENUM			(1ULL<<16)
#define TKUCC_TYFL_TYPEDEF		(1ULL<<17)
#define TKUCC_TYFL_VOLATILE		(1ULL<<18)

#define TKUCC_TYFL_TYMASK		0x03FF


#define TKUCC_TYFL_TY_CHAR		(TKUCC_TYFL_CHAR)
#define TKUCC_TYFL_TY_SCHAR		(TKUCC_TYFL_SIGNED|TKUCC_TYFL_CHAR)
#define TKUCC_TYFL_TY_UCHAR		(TKUCC_TYFL_UNSIGNED|TKUCC_TYFL_CHAR)

#define TKUCC_TYFL_TY_SHORT		(TKUCC_TYFL_SHORT)
#define TKUCC_TYFL_TY_SSHORT	(TKUCC_TYFL_SIGNED|TKUCC_TYFL_SHORT)
#define TKUCC_TYFL_TY_USHORT	(TKUCC_TYFL_UNSIGNED|TKUCC_TYFL_SHORT)

#define TKUCC_TYFL_TY_INT		(TKUCC_TYFL_INT)
#define TKUCC_TYFL_TY_SINT		(TKUCC_TYFL_SIGNED|TKUCC_TYFL_INT)
#define TKUCC_TYFL_TY_UINT		(TKUCC_TYFL_UNSIGNED|TKUCC_TYFL_INT)

#define TKUCC_TYFL_TY_LONG		(TKUCC_TYFL_LONG)
#define TKUCC_TYFL_TY_SLONG		(TKUCC_TYFL_SIGNED|TKUCC_TYFL_LONG)
#define TKUCC_TYFL_TY_ULONG		(TKUCC_TYFL_UNSIGNED|TKUCC_TYFL_LONG)

#define TKUCC_TYFL_TY_SHORTI	\
	(TKUCC_TYFL_SHORT|TKUCC_TYFL_INT)
#define TKUCC_TYFL_TY_SSHORTI	\
	(TKUCC_TYFL_SIGNED|TKUCC_TYFL_SHORT|TKUCC_TYFL_INT)
#define TKUCC_TYFL_TY_USHORTI	\
	(TKUCC_TYFL_UNSIGNED|TKUCC_TYFL_SHORT|TKUCC_TYFL_INT)

#define TKUCC_TYFL_TY_LONGI		\
	(TKUCC_TYFL_LONG|TKUCC_TYFL_INT)
#define TKUCC_TYFL_TY_SLONGI	\
	(TKUCC_TYFL_SIGNED|TKUCC_TYFL_LONG|TKUCC_TYFL_INT)
#define TKUCC_TYFL_TY_ULONGI	\
	(TKUCC_TYFL_UNSIGNED|TKUCC_TYFL_LONG|TKUCC_TYFL_INT)

#define TKUCC_TYFL_TY_LLONG		\
	(TKUCC_TYFL_LONG|TKUCC_TYFL_VOID)
#define TKUCC_TYFL_TY_SLLONG	\
	(TKUCC_TYFL_SIGNED|TKUCC_TYFL_LONG|TKUCC_TYFL_VOID)
#define TKUCC_TYFL_TY_ULLONG	\
	(TKUCC_TYFL_UNSIGNED|TKUCC_TYFL_LONG|TKUCC_TYFL_VOID)

#define TKUCC_TYFL_TY_LLONGI		\
	(TKUCC_TYFL_LONG|TKUCC_TYFL_INT|TKUCC_TYFL_VOID)
#define TKUCC_TYFL_TY_SLLONGI	\
	(TKUCC_TYFL_SIGNED|TKUCC_TYFL_LONG|TKUCC_TYFL_INT|TKUCC_TYFL_VOID)
#define TKUCC_TYFL_TY_ULLONGI	\
	(TKUCC_TYFL_UNSIGNED|TKUCC_TYFL_LONG|TKUCC_TYFL_INT|TKUCC_TYFL_VOID)

#define TKUCC_TYFL_TY_FLOAT		(TKUCC_TYFL_FLOAT)
#define TKUCC_TYFL_TY_DOUBLE	(TKUCC_TYFL_DOUBLE)
#define TKUCC_TYFL_TY_SFLOAT	(TKUCC_TYFL_FLOAT|TKUCC_TYFL_SHORT)
#define TKUCC_TYFL_TY_LDOUBLE	(TKUCC_TYFL_DOUBLE|TKUCC_TYFL_LONG)

#define TKUCC_TYFL_TY_VOID		(TKUCC_TYFL_VOID)
#define TKUCC_TYFL_TY_AUTO		(TKUCC_TYFL_AUTO)

#define TKUCC_LBLID_GENSYN		0x100000


struct TKUCC_LineBuf_s {
TKUCC_LineBuf *next;
char *text;
char t_buf[TKUCC_LINEBUF_FIXEDLIM];
};

struct TKUCC_AstNode_s {
u16 tag;
byte n_keys;
byte n_lvl;
u16 maxidx;
u16 keys[TKUCC_ASTNODE_NKEYS];
u64 vals[TKUCC_ASTNODE_NKEYS];
};

struct TKUCC_PpDefine_s {
TKUCC_PpDefine *next;
char *name;
char *args[16];
char *body;
};

/* Context for main compiler stages. */
struct TKUCC_MainContext_s {
TKUCC_LineBuf *free_line;
FILE *src_fd;

FILE *src_fd_stk[32];
int src_fd_stkpos;

char *pp_incpath[32];
int pp_n_incpath;

/* zone */
char *longline;
TKUCC_ZoneAllocHead *zfirst[256];
TKUCC_ZoneAllocHead *zfree[256];

TKUCC_ZoneStrHead *zstr[256];
TKUCC_ZoneStrHead *zstrfree;
u16 zstrbix[256];

/* preproc / etc */

TKUCC_PpDefine *ppdef[256];
int pp_lvl_ift;
int pp_lvl_iff;

int *tokstrm_span[512];
int tokstrm_idx;
int tokstrm_max;

int *namesym_stix[512];
int *namesym_lbl[512];
int namesym_max;


int gensym_seq;
u64 asm_modefl;

byte op_is_wex2;
byte wexmd_dfl;

int iflvl_f;
int iflvl_t;

/* variables or functions. */
TKUCC_IRDECL	*ir_gbls;
TKUCC_IRDECL	*ir_gbl_ihash[256];
TKUCC_IRDECL	*ir_gbl_nhash[256];
int				ir_gblid_seq;

TKUCC_IRDECL	*cur_func;

/* struct/union declarations and similar. */
TKUCC_IRDECL	*ir_objs;
TKUCC_IRDECL	*ir_obj_ihash[256];
TKUCC_IRDECL	*ir_obj_nhash[256];
int				ir_objid_seq;

/* register allocator */
tkucc_valreg	relalc_val[64];
byte			relalc_lcnt[64];
u64				regalc_regsaved;
u64				regalc_regcansave;
u64				regalc_regdirty;
byte regalc_rov;

/* For 64-bit values that don't fit inline. */
u64 *longtab_v[256];	//long table values
u16 *longtab_c[256];	//long table chain
u16 longtab_h[256];		//long table hash
int longtab_rov;

TKUCC_AsmOp *cblk_asm_first;
TKUCC_AsmOp *cblk_asm_last;

TKUCC_LinkObject *cur_obj;
TKUCC_LinkObject *link_inobj;
};

struct TKUCC_ZoneAllocHead_s {
TKUCC_MainContext *ctx;
TKUCC_ZoneAllocHead *znext;		//next in zone
byte zone;
byte szix;
void *data;
};

struct TKUCC_ZoneStrHead_s {
TKUCC_ZoneStrHead *next;
char strdat[32768];
u16 strofs[4096];
u16 strchn[4096];
u16 strhash[256];
u16 str_sz;
u16 n_str;
u16 bix;
};
