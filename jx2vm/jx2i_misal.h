/*
 MIT No Attribution

 Copyright (c) 2018-2024 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person 
 obtaining a copy of this software and associated documentation 
 files (the "Software"), to deal in the Software without 
 restriction, including without limitation the rights to use, 
 copy, modify, merge, publish, distribute, sublicense, and/or sell 
 copies of the Software, and to permit persons to whom the 
 Software is furnished to do so.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
 OR OTHER DEALINGS IN THE SOFTWARE.
*/

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

#define	BJX2_PtrGetQWord(ptr)				(*(s64 *)(ptr))
#define	BJX2_PtrSetQWord(ptr, val)			((*(s64 *)(ptr))=(val))
#define	BJX2_PtrGetQWordIx(ptr, ix)			(((s64 *)(ptr))[ix])
#define	BJX2_PtrSetQWordIx(ptr, ix, val)	(((s64 *)(ptr))[ix]=(val))

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

s64 BJX2_PtrGetQWord(void *ptr)
	{ s64 i; memcpy(&i, ptr, 8); return(i); }
void BJX2_PtrSetQWord(void *ptr, s64 val)
	{ memcpy(ptr, &val, 8); }
s64 BJX2_PtrGetQWordIx(void *ptr, int ix)
	{ s64 i; memcpy(&i, ((byte *)ptr)+(ix*8), 8); return(i); }
void BJX2_PtrSetQWordIx(void *ptr, int ix, s64 val)
	{ memcpy(((byte *)ptr)+(ix*8), &val, 8); }

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


double BJX2_PtrGetF128Ix(void *ptr, int ix)
{
	u64 lo, hi, v;
	double f;

	lo=BJX2_PtrGetQWordIx(ptr, ix+0);
	hi=BJX2_PtrGetQWordIx(ptr, ix+1);
//	memcpy(&f, ((byte *)ptr)+(ix*8), 8);

	v=	((hi    )&0xC000000000000000ULL) |
		((hi<< 4)&0x3FFFFFFFFFFFFFFFULL) |
		 (lo>>60);
	memcpy(&f, &v, 8);

	return(f);
}
void BJX2_PtrSetF128Ix(void *ptr, int ix, double val)
{
	u64 lo, hi, v, v1;

	memcpy(&v, &val, 8);
	
	if(!(v&0x4000000000000000ULL))
		v1=0x3C00000000000000ULL;
	else
		v1=0x0000000000000000ULL;
	hi=	((v&0xC000000000000000ULL)   ) |
		((v&0x3FFFFFFFFFFFFFFFULL)>>4) |
		v1;
	lo=v<<60;
	
	BJX2_PtrSetQWordIx(ptr, ix+0, lo);
	BJX2_PtrSetQWordIx(ptr, ix+1, hi);

//	memcpy(((byte *)ptr)+(ix*8), &val, 8);
}

typedef struct { u64 lo; u64 hi; } bjx2_val128;

bjx2_val128 BJX2_PtrGetVal128Ix(void *ptr, int ix)
{
	bjx2_val128 v;
	v.lo=BJX2_PtrGetQWordIx(ptr, ix+0);
	v.hi=BJX2_PtrGetQWordIx(ptr, ix+1);
	return(v);
}

void BJX2_PtrSetVal128Ix(void *ptr, int ix, bjx2_val128 val)
{
	BJX2_PtrSetQWordIx(ptr, ix+0, val.lo);
	BJX2_PtrSetQWordIx(ptr, ix+1, val.hi);
}
