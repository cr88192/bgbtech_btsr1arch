
#define TK_UMSG_ROMFCN			0x1000

#define TK_UMSG_GETCONPARAM		0x1001
#define TK_UMSG_CONRESET		0x1002
#define TK_UMSG_PGMEXIT			0x1003
#define TK_UMSG_PAGEALLOC		0x1004
#define TK_UMSG_PAGEFREE		0x1005

#define TK_URES_FALSE			0x0000
#define TK_URES_TRUE			0x0001

#define TK_URES_ERR_UNKNOWN		0xFFFF
#define TK_URES_ERR_BADMSG		0xFFF0

typedef union TK_SysArg_s TK_SysArg;
union TK_SysArg_s {
s32 i;
u32 ui;
s64 l;
u64 ul;
void *p;
};
