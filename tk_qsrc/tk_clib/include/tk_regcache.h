/*
Do something akin to a key/value registry.
*/

#define TK_REGCACHE_ETY_DEFAULT		0
#define TK_REGCACHE_ETY_SHORTSTR	1	//short (inline) string
#define TK_REGCACHE_ETY_DIR			2
#define TK_REGCACHE_ETY_LONGSTR		3	//long (non-inline) string

typedef struct TK_RegDirEnt_s TK_RegDirEnt;
typedef struct TK_RegContext_s TK_RegContext;
typedef struct TK_RegDirEnt_LongStr_s TK_RegDirEnt_LongStr;

struct TK_RegDirEnt_s
{
 	byte	name[48];	//00: name
	byte	ety;		//30: Entry Type
	byte	cmp;		//31: Compression
	u16		resv0;		//32: -
 	u32		chain;		//34: Chain
 	u32		dirid;		//38: Directory ID / Parent Directory (DirEnt Index)
 	u32		dirnext;	//3C: Next in Directory (DirEnt Index)
	byte	data[64];	//40: Payload Data
};

struct TK_RegDirEnt_LongStr_s
{
	void *ptr;
	int len;
	int lim;
};

struct TK_RegContext_s
{
	TK_RegDirEnt **regdir;
	int rdehash[1024];
	int lastidx;
};

