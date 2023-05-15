/*
 0: TagRef / R
 1: -
 2: -
 3: -
 4: Fixnum
 5: Fixnum
 6: Fixnum
 7: Fixnum
 8: Flonum
 9: Flonum
 A: Flonum
 B: Flonum
 C: -
 D: -
 E: -
 F: TagRef2
 
*/

#define		LVA_MAGIC_NULL			0x0000000000000000ULL
#define		LVA_MAGIC_UNDEFINED		0x0000000000000001ULL
#define		LVA_POINTER_MASK		0x0000FFFFFFFFFFFFULL

#define		LVA_OPR_ADD				0x00
#define		LVA_OPR_SUB				0x01
#define		LVA_OPR_MUL				0x02
#define		LVA_OPR_DIV				0x03
#define		LVA_OPR_MOD				0x04
#define		LVA_OPR_AND				0x05
#define		LVA_OPR_OR				0x06
#define		LVA_OPR_XOR				0x07
#define		LVA_OPR_SHL				0x08
#define		LVA_OPR_SHR				0x09

#define		LVA_OPR_NEG				0x0C
#define		LVA_OPR_NOT				0x0D
#define		LVA_OPR_CMPEQ			0x0E
#define		LVA_OPR_CMPGT			0x0F
#define		LVA_OPR_INC				0x10
#define		LVA_OPR_DEC				0x11


#define		LVA_TAGAMT_SI			0x00
#define		LVA_TAGAMT_SL			0x01
#define		LVA_TAGAMT_F			0x02
#define		LVA_TAGAMT_D			0x03
#define		LVA_TAGAMT_P			0x04
#define		LVA_TAGAMT_VAR			0x05

#define		LVA_TAGAMT_NL			0x07
#define		LVA_TAGAMT_SB			0x08
#define		LVA_TAGAMT_UB			0x09
#define		LVA_TAGAMT_SS			0x0A
#define		LVA_TAGAMT_US			0x0B
#define		LVA_TAGAMT_UI			0x0C
#define		LVA_TAGAMT_UL			0x0D
#define		LVA_TAGAMT_UNL			0x0E

#define		LVA_LVATY_TAGARRAY		0x01
#define		LVA_LVATY_VARIANT		0x02
#define		LVA_LVATY_STRING		0x03
#define		LVA_LVATY_WSTRING		0x04
#define		LVA_LVATY_USTRING		0x05
#define		LVA_LVATY_CLASSOBJ		0x06
#define		LVA_LVATY_POINTER		0x07
#define		LVA_LVATY_BYTE			0x08
#define		LVA_LVATY_SBYTE			0x09
#define		LVA_LVATY_SHORT			0x0A
#define		LVA_LVATY_USHORT		0x0B
#define		LVA_LVATY_INT			0x0C
#define		LVA_LVATY_UINT			0x0D
#define		LVA_LVATY_LONG			0x0E
#define		LVA_LVATY_ULONG			0x0F
#define		LVA_LVATY_FLOAT			0x10
#define		LVA_LVATY_DOUBLE		0x11
#define		LVA_LVATY_INT128		0x12
#define		LVA_LVATY_FLOAT128		0x13

#define		LVA_LVATY_FIXNUM		0x14
#define		LVA_LVATY_FLONUM		0x15
#define		LVA_LVATY_TAGOBJ		0x16
#define		LVA_LVATY_CONS			0x17
#define		LVA_LVATY_BIGINT		0x18

typedef struct LVA_FieldInfo_s		LVA_FieldInfo;
typedef struct LVA_FieldInfoD_s	LVA_FieldInfoD;
typedef struct LVA_ClassInfo_s		LVA_ClassInfo;
typedef struct LVA_VTableBasic_s	LVA_VTableBasic;
typedef struct LVA_ObjectBasic_s	LVA_ObjectBasic;
typedef struct LVA_VarObject_s		LVA_VarObject;
typedef struct LVA_TagInfo_s		LVA_TagInfo;

//typedef union LVA_TagArray_s		LVA_TagArray;
typedef struct LVA_TagArray_s		LVA_TagArray;

struct LVA_FieldInfo_s {
char *name;		//field/method name (QName for Interfaces)
char *sig;		//type signature (ClassInfo Pointer for Interfaces)
u32 flags;		//flags
u32 offset;		//offset (within object), VTable Index (method)
};

struct LVA_FieldInfoD_s {
u32 name;		//field/method name (QName for Interfaces)
u32 sig;		//type signature (ClassInfo Pointer for Interfaces)
u32 flags;		//flags
u32 offset;		//offset (within object), VTable Index (method)
};

struct LVA_ClassInfo_s {
u32 self;		//self pointer
u32 qname;		//object qname (or type signature).
u32 super;		//superclass
u32 fields;		//fields list, NULL terminated
u32 methods;	//methods list (vtable), NULL terminated
u32 iface;		//interface list, NULL terminated
};

struct LVA_VTableBasic_s {
LVA_ClassInfo *clazz;		//Class Info
long base_offs;				//offset from base (interfaces)
void	*resv1;
void	*resv2;
//class methods follow
void	*methods[1];
};

struct LVA_ObjectBasic_s {
LVA_VTableBasic	*vt;		//vtable
LVA_VarObject	*vo;		//dynamic object
byte data[1];				//object data
};

struct LVA_VarObject_s {
u16 *keys;			//dynamic alloc keys
u64 *vals;			//dynamic alloc values
short nkey;			//number of keys
short mkey;			//max allocated keys (0 if fixed)
u64 fx_val[16];		//fixed values
u16 fx_key[16];		//fixed keys
};

struct LVA_TagInfo_s {
char *name;
short idx;
short chain;
};

#if 0
union LVA_TagArray_s {
	struct {
		LVA_VTableBasic *vt;
		void *dptr;
		s32 size;
		u16 tty;
		u16 pad;
		u64 data[1];
	}p;
	struct {
		LVA_VTableBasic *vt;
		void *dptr;
		s32 size;
		s32 base;
		u64 data_ty;
	}n;
};
#endif

#if 1
struct LVA_TagArray_s {
	LVA_VTableBasic *vt;	//00
	void *data;				//08
	u32 size;				//10
	u16 tty;				//14
	s16 base;				//16
	u64 t_data[1];			//18
};
#endif

typedef struct LVA_TagOperator_s LVA_TagOperator;
struct LVA_TagOperator_s {
LVA_TagOperator	*next;
LVA_TagOperator	*hnext;
// u64 (*Run)(u64 vala, u64 valb);
void *run;
u64 ident;
//u16 lty;
//u16 rty;
//byte opr;
};

#ifdef _BGBMETA
typedef __object tk_lva_object;
typedef __variant tk_lva_variant;
#define tk_lva_object_null __object_null
#define	tk_lva_object_getbits(obj)	__object_getbits(obj)
#define	tk_lva_object_frombits(obj)	__object_frombits(obj)
#else
typedef long long tk_lva_object;
typedef long long tk_lva_variant;
#define tk_lva_object_null 0
#define	tk_lva_object_getbits(obj)	(obj)
#define	tk_lva_object_frombits(obj)	(obj)
#endif

typedef struct LVA_Cons_s LVA_Cons;
struct LVA_Cons_s {
// tk_lva_variant car;
// tk_lva_variant cdr;
u64 car;
u64 cdr;
};

void TKMM_LVA_ArrayInit(void);

LVA_TagArray *TKMM_LVA_NewVarArray(int n);
LVA_TagArray *TKMM_LVA_NewTagArray(int n, int mt);

void TKMM_LVA_TagInit(void);
LVA_TagInfo *TKMM_LVA_GetTagInfoForName(char *name);
int TKMM_LVA_GetTagIndexForName(char *name);
char *TKMM_LVA_GetTagNameForIndex(int idx);
void *TKMM_LVA_TyMalloc(char *tyn, int sz);
void *TKMM_LVA_TagMalloc(int tyi, int sz);
void *TKMM_LVA_ReAlloc(void *ptr, int sz);
char *TKMM_LVA_GetPtrTypeName(void *ptr);
int TKMM_LVA_GetPtrTypeTag(void *ptr);

u64 TKMM_LVA_WrapFixnum(s64 val);
u64 TKMM_LVA_WrapFlonum(double val);

s64 TKMM_LVA_UnwrapFixnum(u64 vv);
double TKMM_LVA_UnwrapFlonum(u64 vv);

u64 TKMM_LVA_WrapPointer(void *val);
void *TKMM_LVA_UnwrapPointer(u64 val);



u64 __lva_conv_fromi32(s32 val);
u64 __lva_conv_fromi64(s64 val);
u64 __lva_conv_fromf32(float val);
u64 __lva_conv_fromf64(double val);

s32 __lva_conv_toi32(u64 val);
s64 __lva_conv_toi64(u64 val);
float __lva_conv_tof32(u64 val);
double __lva_conv_tof64(u64 val);
