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

#define	BJX2_PtrGetDWord(ptr)				(*(u32 *)(ptr))
#define	BJX2_PtrSetDWord(ptr, val)			((*(u32 *)(ptr))=(val))

#define	BJX2_PtrGetFloat(ptr)				(*(float *)(ptr))
#define	BJX2_PtrSetFloat(ptr, val)			((*(float *)(ptr))=(val))

#define	BJX2_PtrGetDouble(ptr)				(*(double *)(ptr))
#define	BJX2_PtrSetDouble(ptr, val)			((*(double *)(ptr))=(val))
#define	BJX2_PtrGetDoubleIx(ptr, ix)		(((double *)(ptr))[ix])
#define	BJX2_PtrSetDoubleIx(ptr, ix, val)	(((double *)(ptr))[ix]=(val))

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


s32 BJX2_PtrGetDWord(void *ptr)
	{ s32 i; memcpy(&i, ptr, 4); return(i); }
void BJX2_PtrSetDWord(void *ptr, s32 val)
	{ memcpy(ptr, &val, 4); }

float BJX2_PtrGetFloat(void *ptr)
	{ float f; memcpy(&f, ptr, 4); return(f); }
void BJX2_PtrSetFloat(void *ptr, float val)
	{ memcpy(ptr, &val, 4); }

double BJX2_PtrGetDouble(void *ptr)
	{ double f; memcpy(&f, ptr, 8); return(f); }
void BJX2_PtrSetDouble(void *ptr, double val)
	{ memcpy(ptr, &val, 8); }

double BJX2_PtrGetDoubleIx(void *ptr, int ix)
	{ double f; memcpy(&f, ((byte *)ptr)+(ix*8), 8); return(f); }
void BJX2_PtrSetDoubleIx(void *ptr, int ix, double val)
	{ memcpy(((byte *)ptr)+(ix*8), &val, 8); }

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
