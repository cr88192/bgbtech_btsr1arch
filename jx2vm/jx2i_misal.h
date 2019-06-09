#if defined(_M_X64)
#define BJX2_MISAL_LE
#endif

#ifdef BJX2_MISAL_LE

#define BJX2_PtrGetSByteIxLe(ptr, ix)	(((sbyte *)(ptr))[ix])
#define BJX2_PtrGetSWordIxLe(ptr, ix)	(((s16 *)(ptr))[ix])
#define BJX2_PtrGetSDWordIxLe(ptr, ix)	(((s32 *)(ptr))[ix])

#define BJX2_PtrSetByteIxLe(ptr, ix, val)		(((sbyte *)(ptr))[ix]=(val))
#define BJX2_PtrSetWordIxLe(ptr, ix, val)		(((s16 *)(ptr))[ix]=(val))
#define BJX2_PtrSetDWordIxLe(ptr, ix, val)		(((s32 *)(ptr))[ix]=(val))


#define BJX2_PtrGetOfs(ptr, ix)		(((sbyte *)(ptr))+(ix))

#define BJX2_PtrGetSByteOfsLe(ptr, ix)		(*(sbyte *)BJX2_PtrGetOfs(ptr, ix))
#define BJX2_PtrGetSWordOfsLe(ptr, ix)		(*(s16 *)BJX2_PtrGetOfs(ptr, ix))
#define BJX2_PtrGetSDWordOfsLe(ptr, ix)		(*(s32 *)BJX2_PtrGetOfs(ptr, ix))
#define BJX2_PtrGetSQWordOfsLe(ptr, ix)		(*(s64 *)BJX2_PtrGetOfs(ptr, ix))

#define BJX2_PtrSetByteOfsLe(ptr, ix, val)		\
	(*(sbyte *)BJX2_PtrGetOfs(ptr, ix)=(val))
#define BJX2_PtrSetWordOfsLe(ptr, ix, val)		\
	(*(s16 *)BJX2_PtrGetOfs(ptr, ix)=(val))
#define BJX2_PtrSetDWordOfsLe(ptr, ix, val)	\
	(*(s32 *)BJX2_PtrGetOfs(ptr, ix)=(val))
#define BJX2_PtrSetQWordOfsLe(ptr, ix, val)	\
	(*(s64 *)BJX2_PtrGetOfs(ptr, ix)=(val))

#else

#ifdef LITTLEENDIAN

int BJX2_PtrGetSWordLe(byte *ptr)
	{ s16 i; memcpy(&i, ptr, 2); return(i); }
s32 BJX2_PtrGetSDWordLe(byte *ptr)
	{ s32 i; memcpy(&i, ptr, 4); return(i); }
s64 BJX2_PtrGetSQWordLe(byte *ptr)
	{ s64 i; memcpy(&i, ptr, 8); return(i); }

void BJX2_PtrSetWordLe(byte *ptr, int val)
	{ memcpy(ptr, &val, 2); }
void BJX2_PtrSetDWordLe(byte *ptr, s32 val)
	{ memcpy(ptr, &val, 4); }
void BJX2_PtrSetQWordLe(byte *ptr, s64 val)
	{ memcpy(ptr, &val, 8); }

#else
int BJX2_PtrGetSWordLe(byte *ptr)
	{ return((s16)(ptr[0]|(ptr[1]<<8))); }
s32 BJX2_PtrGetSDWordLe(byte *ptr)
	{ return((s32)(ptr[0]|(ptr[1]<<8)|(ptr[2]<<16)|(ptr[3]<<24))); }
s64 BJX2_PtrGetSQWordLe(byte *ptr)
	{ return(((u32)BJX2_PtrGetSDWordLe(ptr))|
		(((s64)BJX2_PtrGetSDWordLe(ptr+4))<<32)); }

void BJX2_PtrSetWordLe(byte *ptr, int val)
	{ ptr[0]=val; ptr[1]=val>>8; }
void BJX2_PtrSetDWordLe(byte *ptr, s32 val)
	{ ptr[0]=val; ptr[1]=val>>8; ptr[2]=val>>16; ptr[3]=val>>24; }
void BJX2_PtrSetQWordLe(byte *ptr, s64 val)
	{ ptr[0]=val    ; ptr[1]=val>> 8; ptr[2]=val>>16; ptr[3]=val>>24;
	  ptr[4]=val>>32; ptr[5]=val>>40; ptr[6]=val>>48; ptr[7]=val>>56; }
#endif

#define BJX2_PtrGetOfs(ptr, ix)		(((byte *)(ptr))+(ix))

#define BJX2_PtrGetSByteOfsLe(ptr, ix)		(*(sbyte *)BJX2_PtrGetOfs(ptr, ix))
#define BJX2_PtrGetSWordOfsLe(ptr, ix)		\
	(BJX2_PtrGetSWordLe(BJX2_PtrGetOfs(ptr, ix)))
#define BJX2_PtrGetSDWordOfsLe(ptr, ix)		\
	(BJX2_PtrGetSDWordLe(BJX2_PtrGetOfs(ptr, ix)))
#define BJX2_PtrGetSQWordOfsLe(ptr, ix)		\
	(BJX2_PtrGetSQWordLe(BJX2_PtrGetOfs(ptr, ix)))

#define BJX2_PtrSetByteOfsLe(ptr, ix, val)		\
	(*(sbyte *)BJX2_PtrGetOfs(ptr, ix)=(val))
#define BJX2_PtrSetWordOfsLe(ptr, ix, val)		\
	(BJX2_PtrSetWordLe(BJX2_PtrGetOfs(ptr, ix), val))
#define BJX2_PtrSetDWordOfsLe(ptr, ix, val)	\
	(BJX2_PtrSetDWordLe(BJX2_PtrGetOfs(ptr, ix), val))
#define BJX2_PtrSetQWordOfsLe(ptr, ix, val)	\
	(BJX2_PtrSetQWordLe(BJX2_PtrGetOfs(ptr, ix), val))

#endif
