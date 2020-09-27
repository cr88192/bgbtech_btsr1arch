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

typedef struct LVA_FieldInfo_s		LVA_FieldInfo;
typedef struct LVA_FieldInfoD_s	LVA_FieldInfoD;
typedef struct LVA_ClassInfo_s		LVA_ClassInfo;
typedef struct LVA_VTableBasic_s	LVA_VTableBasic;
typedef struct LVA_ObjectBasic_s	LVA_ObjectBasic;
typedef struct LVA_VarObject_s		LVA_VarObject;
typedef struct LVA_TagInfo_s		LVA_TagInfo;

typedef union LVA_TagArray_s		LVA_TagArray;

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
short nkey;			//number of keys
short mkey;			//max allocated keys (0 if fixed)
u16 *keys;			//dynamic alloc keys
u64 *vals;			//dynamic alloc values
u16 fx_key[16];		//fixed keys
u64 fx_val[16];		//fixed values
};

struct LVA_TagInfo_s {
char *name;
short idx;
short chain;
};

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

#ifdef _BGBMETA
typedef __object tk_lva_object;
typedef __variant tk_lva_variant;
#define tk_lva_object_null __object_null
#define	tk_lva_object_getbits(obj)	__object_getbits(obj)
#else
typedef long long tk_lva_object;
typedef long long tk_lva_variant;
#define tk_lva_object_null 0
#define	tk_lva_object_getbits(obj)	(obj)
#endif

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

