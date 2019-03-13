typedef struct LVA_FieldInfo_s		LVA_FieldInfo;
typedef struct LVA_ClassInfo_s		LVA_ClassInfo;
typedef struct LVA_VTableBasic_s	LVA_VTableBasic;
typedef struct LVA_ObjectBasic_s	LVA_ObjectBasic;
typedef struct LVA_VarObject_s		LVA_VarObject;
typedef struct LVA_TagInfo_s		LVA_TagInfo;

typedef union LVA_TagArray_s		LVA_TagArray;

struct LVA_FieldInfo_s {
u32 name;		//field/method name (QName for Interfaces)
u32 sig;		//type signature (ClassInfo Pointer for Interfaces)
u32 flags;		//flags
u32 offset;		//offset (within object), VTable Index (method)
};

struct LVA_ClassInfo_s {
u32 self;		//self pointer
u32 qname;		//object qname
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
union {
	struct {
		u16 fx_key[16];		//fixed keys
		u64 fx_val[16];		//fixed values
	}fx;
	struct {
		u16 *keys;		//dynamic alloc keys
		u64 *vals;		//dynamic alloc values
	}d;
}u;
};

struct LVA_TagInfo_s {
char *name;
int idx;
};

union LVA_TagArray_s {
	struct {
		s32 size;
		u16 tty;
		u16 pad;
		u64 data[1];
	}p;
	struct {
		s32 size;
		s32 base;
		u64 data_ty;
	}n;
};
