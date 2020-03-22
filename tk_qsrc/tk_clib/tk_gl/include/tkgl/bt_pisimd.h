/*
 * Packed Integer SIMD
 *
 * Goal:
 * Does SIMD using packed integers;
 * Be reasonably competitive with specially written code;
 * Provide some semblance of an abstraction.
 *
 * Logical Packing is Little-Endian
 * Each larger unit logically consists of the smaller units in LE order.
 * A is first, which is lowest-order
 * D is last, which is highest order.
 * AS: bits  0..15 of A
 * AT: bits 16..31 of A
 * AX: bits  0.. 7 of A
 * AY: bits  8..15 of A
 * AZ: bits 16..23 of A
 * AW: bits 24..31 of A
 *
 * AP: bits  0.. 9 of A
 * AQ: bits 10..19 of A
 * AR: bits 20..29 of A
 *
 * P64: S, T, U, V
 * P64 may be treated as 4 16 bit units.
 */

#ifndef BT_PISIMD_H
#define BT_PISIMD_H

typedef union _psimd32_s psimd32;		//32-bit packed integer SIMD
typedef struct _psimd64_s psimd64;		//64-bit packed integer SIMD
typedef struct _psimd128_s psimd128;	//128 bit packed integer SIMD

union _psimd32_s {
u32 a;
byte xyzw[4];
u16 ust[2];
s16 sst[2];
};

struct _psimd64_s {
u32 a;
u32 b;
};

struct _psimd128_s {
psimd32 a;
psimd32 b;
psimd32 c;
psimd32 d;
};

#define psiMakeP32ST(s, t)	\
	psiMakeP32(((s)&0xFFFF)|(((t)&0xFFFF)<<16))
#define psiMakeP32XYZW(x, y, z, w) \
	psiMakeP32(	( (x)&0xFF)     |(((y)&0xFF)<< 8)| \
				(((z)&0xFF)<<16)|(((w)&0xFF)<<24))

#define psiMakeP32STn(s, t)	\
	psiMakeP32((s)|((t)<<16))
#define psiMakeP32XYZWn(x, y, z, w) \
	psiMakeP32(	 (x)     |((y)<< 8)| \
				((z)<<16)|((w)<<24))

#define psiMakeP64STUV(s, t, u, v)	\
	psiMakeP32(	((s)&0xFFFF)|(((t)&0xFFFF)<<16), \
				((u)&0xFFFF)|(((v)&0xFFFF)<<16))
#define psiMakeP64STUVn(s, t, u, v)	\
	psiMakeP64((s)|((t)<<16), (u)|((v)<<16))

static inline psimd32 psiMakeP32(u32 a)
	{ psimd32 t; t.a=a; return(t); }
static inline psimd64 psiMakeP64AB(u32 a, u32 b)
	{ psimd64 t; t.a=a; t.b=b; return(t); }

static inline psimd64 psiMakeP64PAB(psimd32 a, psimd32 b)
	{ psimd64 t; t.a=a.a; t.b=b.a; return(t); }

static inline psimd128 psiMakeP128ABCD(u32 a, u32 b, u32 c, u32 d)
	{ psimd64 t; t.a=a; t.b=b; t.c=c; t.d=d; return(t); }
static inline psimd128 psiMakeP128PABCD(
		psimd32 a, psimd32 b, psimd32 c, psimd32 d)
	{ psimd64 t; t.a=a.a; t.b=b.a; t.c=c.a; t.d=d.a; return(t); }

static inline psimd128 psiMakeP128PAB(psimd64 a, psimd64 b)
	{ psimd128 t; t.a=a.a; t.b=a.b; t.c=b.a; t.d=b.b; return(t); }

#define psiGetP32A(a)	((a).a)
#define psiGetP32S(a)	((u16)((a).a))
#define psiGetP32T(a)	((u16)((a).a>>16))
#define psiGetP32X(a)	((byte)((a).a))
#define psiGetP32Y(a)	((byte)((a).a>>8))
#define psiGetP32Z(a)	((byte)((a).a>>16))
#define psiGetP32W(a)	((byte)((a).a>>24))

#define psiGetP64A(a)	((a).a)
#define psiGetP64B(a)	((a).b)
#define psiGetP64S(a)	((u16)((a).a))
#define psiGetP64T(a)	((u16)((a).a>>16))
#define psiGetP64U(a)	((u16)((a).b))
#define psiGetP64V(a)	((u16)((a).b>>16))
#define psiGetP64AS(a)	((u16)((a).a))
#define psiGetP64AT(a)	((u16)((a).a>>16))
#define psiGetP64BS(a)	((u16)((a).b))
#define psiGetP64BT(a)	((u16)((a).b>>16))
#define psiGetP64AX(a)	((byte)((a).a))
#define psiGetP64AY(a)	((byte)((a).a>>8))
#define psiGetP64AZ(a)	((byte)((a).a>>16))
#define psiGetP64AW(a)	((byte)((a).a>>24))
#define psiGetP64BX(a)	((byte)((a).b))
#define psiGetP64BY(a)	((byte)((a).b>>8))
#define psiGetP64BZ(a)	((byte)((a).b>>16))
#define psiGetP64BW(a)	((byte)((a).b>>24))

#define psiGetP128A(a)	((a).a)
#define psiGetP128B(a)	((a).b)
#define psiGetP128C(a)	((a).c)
#define psiGetP128D(a)	((a).d)

#define psiGetP128AS(a)	((u16)((a).a))
#define psiGetP128AT(a)	((u16)((a).a>>16))
#define psiGetP128BS(a)	((u16)((a).b))
#define psiGetP128BT(a)	((u16)((a).b>>16))
#define psiGetP128CS(a)	((u16)((a).c))
#define psiGetP128CT(a)	((u16)((a).c>>16))
#define psiGetP128DS(a)	((u16)((a).d))
#define psiGetP128DT(a)	((u16)((a).d>>16))

#define psiGetP128AX(a)	((byte)((a).a))
#define psiGetP128AY(a)	((byte)((a).a>>8))
#define psiGetP128AZ(a)	((byte)((a).a>>16))
#define psiGetP128AW(a)	((byte)((a).a>>24))
#define psiGetP128BX(a)	((byte)((a).b))
#define psiGetP128BY(a)	((byte)((a).b>>8))
#define psiGetP128BZ(a)	((byte)((a).b>>16))
#define psiGetP128BW(a)	((byte)((a).b>>24))
#define psiGetP128CX(a)	((byte)((a).c))
#define psiGetP128CY(a)	((byte)((a).c>>8))
#define psiGetP128CZ(a)	((byte)((a).c>>16))
#define psiGetP128CW(a)	((byte)((a).c>>24))
#define psiGetP128DX(a)	((byte)((a).d))
#define psiGetP128DY(a)	((byte)((a).d>>8))
#define psiGetP128DZ(a)	((byte)((a).d>>16))
#define psiGetP128DW(a)	((byte)((a).d>>24))

#if 0
static inline u32 psiGetP32A(psimd32 a)
	{ return(a.a); }
static inline int psiGetP32S(psimd32 a)
	{ return((u16)(a.a)); }
static inline int psiGetP32T(psimd32 a)
	{ return((u16)(a.a>>16)); }

static inline int psiGetP32X(psimd32 a)
	{ return((byte)(a.a)); }
static inline int psiGetP32Y(psimd32 a)
	{ return((byte)(a.a>>8)); }

static inline u32 psiGetP64A(psimd64 a)
	{ return(a.a); }
static inline u32 psiGetP64B(psimd64 a)
	{ return(a.b); }

static inline u32 psiGetP128A(psimd128 a)
	{ return(a.a); }
static inline u32 psiGetP128B(psimd128 a)
	{ return(a.b); }
static inline u32 psiGetP128C(psimd128 a)
	{ return(a.c); }
static inline u32 psiGetP128D(psimd128 a)
	{ return(a.d); }
#endif

static inline psimd32 psiGetP64PA(psimd64 a)
	{ psimd32 t; t.a=a.a; return(t); }
static inline psimd32 psiGetP64PB(psimd64 a)
	{ psimd32 t; t.a=a.b; return(t); }
static inline psimd64 psiGetP64PBA(psimd64 a)
	{ psimd64 t; t.a=a.b; t.b=a.a; return(t); }

static inline psimd32 psiGetP128PA(psimd128 a)
	{ psimd32 t; t.a=a.a; return(t); }
static inline psimd32 psiGetP128PB(psimd128 a)
	{ psimd32 t; t.a=a.b; return(t); }
static inline psimd32 psiGetP128PC(psimd128 a)
	{ psimd32 t; t.a=a.c; return(t); }
static inline psimd32 psiGetP128PD(psimd128 a)
	{ psimd32 t; t.a=a.d; return(t); }

static inline psimd64 psiGetP128PAA(psimd128 a)
	{ psimd64 t; t.a=a.a; t.b=a.a; return(t); }
static inline psimd64 psiGetP128PAB(psimd128 a)
	{ psimd64 t; t.a=a.a; t.b=a.b; return(t); }
static inline psimd64 psiGetP128PAC(psimd128 a)
	{ psimd64 t; t.a=a.a; t.b=a.c; return(t); }
static inline psimd64 psiGetP128PAD(psimd128 a)
	{ psimd64 t; t.a=a.a; t.b=a.b; return(t); }
static inline psimd64 psiGetP128PBA(psimd128 a)
	{ psimd64 t; t.a=a.b; t.b=a.b; return(t); }
static inline psimd64 psiGetP128PBB(psimd128 a)
	{ psimd64 t; t.a=a.b; t.b=a.b; return(t); }
static inline psimd64 psiGetP128PBC(psimd128 a)
	{ psimd64 t; t.a=a.b; t.b=a.c; return(t); }
static inline psimd64 psiGetP128PBD(psimd128 a)
	{ psimd64 t; t.a=a.b; t.b=a.b; return(t); }
static inline psimd64 psiGetP128PCA(psimd128 a)
	{ psimd64 t; t.a=a.c; t.b=a.b; return(t); }
static inline psimd64 psiGetP128PCB(psimd128 a)
	{ psimd64 t; t.a=a.c; t.b=a.b; return(t); }
static inline psimd64 psiGetP128PCC(psimd128 a)
	{ psimd64 t; t.a=a.c; t.b=a.c; return(t); }
static inline psimd64 psiGetP128PCD(psimd128 a)
	{ psimd64 t; t.a=a.c; t.b=a.b; return(t); }
static inline psimd64 psiGetP128PDA(psimd128 a)
	{ psimd64 t; t.a=a.d; t.b=a.b; return(t); }
static inline psimd64 psiGetP128PDB(psimd128 a)
	{ psimd64 t; t.a=a.d; t.b=a.b; return(t); }
static inline psimd64 psiGetP128PDC(psimd128 a)
	{ psimd64 t; t.a=a.d; t.b=a.c; return(t); }
static inline psimd64 psiGetP128PDD(psimd128 a)
	{ psimd64 t; t.a=a.d; t.b=a.b; return(t); }


static inline psimd32 psiAddP32A(psimd32 a, psimd32 b)
	{ psimd32 c; c.a=a.a+b.a; return(c); }
static inline psimd32 psiSubP32A(psimd32 a, psimd32 b)
	{ psimd32 c; c.a=a.a-b.a; return(c); }
static inline psimd32 psiMulloP32A(psimd32 a, psimd32 b)
	{ psimd32 c; c.a=a.a*b.a; return(c); }

static inline psimd64 psiAddP64A(psimd64 a, psimd64 b)
	{ psimd64 c; c.a=a.a+b.a; c.b=a.b+b.b; return(c); }
static inline psimd64 psiSubP64A(psimd64 a, psimd64 b)
	{ psimd64 c; c.a=a.a-b.a; c.b=a.b-b.b; return(c); }
static inline psimd64 psiMulloP64A(psimd64 a, psimd64 b)
	{ psimd64 c; c.a=a.a*b.a; c.b=a.b*b.b; return(c); }

static inline psimd128 psiAddP128A(psimd128 a, psimd128 b)
{	psimd128 c; c.a=a.a+b.a; c.b=a.b+b.b;
	c.c=a.c+b.c; c.d=a.d+b.d; return(c); }
static inline psimd128 psiSubP128A(psimd128 a, psimd128 b)
{	psimd128 c; c.a=a.a-b.a; c.b=a.b-b.b;
	c.c=a.c-b.c; c.d=a.d-b.d; return(c); }
static inline psimd128 psiMulloP128A(psimd128 a, psimd128 b)
{	psimd128 c; c.a=a.a*b.a; c.b=a.b*b.b;
	c.c=a.c*b.c; c.d=a.d*b.d; return(c); }


static inline psimd32 psiMuluhiP32A(psimd32 a, psimd32 b)
	{ psimd32 c; c.a=(((u64)a.a)*b.a)>>32; return(c); }
static inline psimd32 psiMulshiP32A(psimd32 a, psimd32 b)
	{ psimd32 c; c.a=(((s64)((s32)a.a))*((s32)b.a))>>32; return(c); }

static inline psimd64 psiMuluhiP64A(psimd64 a, psimd64 b)
{
	psimd64 c;
	c.a=(((u64)a.a)*b.a)>>32; c.b=(((u64)a.b)*b.b)>>32;
	return(c);
}

static inline psimd64 psiMulshiP64A(psimd64 a, psimd64 b)
{
	psimd64 c;
	c.a=(((s64)((s32)a.a))*((s32)b.a))>>32;
	c.b=(((s64)((s32)a.b))*((s32)b.b))>>32;
	return(c);
}


static inline psimd128 psiMuluhiP128A(psimd128 a, psimd128 b)
{
	psimd128 c;
	c.a=(((u64)a.a)*b.a)>>32; c.b=(((u64)a.b)*b.b)>>32;
	c.c=(((u64)a.c)*b.c)>>32; c.d=(((u64)a.d)*b.d)>>32;
	return(c);
}

static inline psimd128 psiMulshiP128A(psimd128 a, psimd128 b)
{
	psimd128 c;
	c.a=(((s64)((s32)a.a))*((s32)b.a))>>32;
	c.b=(((s64)((s32)a.b))*((s32)b.b))>>32;
	c.c=(((s64)((s32)a.c))*((s32)b.c))>>32;
	c.d=(((s64)((s32)a.d))*((s32)b.d))>>32;
	return(c);
}


static inline psimd32 psiAddP32ST(psimd32 a, psimd32 b)
{
	psimd32 c;
	c.ust[0]=a.ust[0]+b.ust[0];
	c.ust[1]=a.ust[1]+b.ust[1];
//	c.a=((a.a+ b.a              )&0x0000FFFFUL)|
//		((a.a+(b.a&0xFFFF0000UL))&0xFFFF0000UL);
	return(c);
}

static inline psimd32 psiSubP32ST(psimd32 a, psimd32 b)
{
	psimd32 c;
	c.ust[0]=a.ust[0]-b.ust[0];
	c.ust[1]=a.ust[1]-b.ust[1];
//	c.a=((a.a- b.a              )&0x0000FFFFUL)|
//		((a.a-(b.a&0xFFFF0000UL))&0xFFFF0000UL);
	return(c);
}

static inline psimd32 psiMulloP32ST(psimd32 a, psimd32 b)
{
	psimd32 c;
	c.ust[0]=a.ust[0]*b.ust[0];
	c.ust[1]=a.ust[1]*b.ust[1];
//	c.a=((a.a*b.a)&0x0000FFFFUL)|
//		((a.a&0xFFFF0000UL)*(b.a&0xFFFF0000UL));
	return(c);
}

static inline psimd32 psiMuluhiP32ST(psimd32 a, psimd32 b)
{
	psimd32 c;
	c.ust[0]=(a.ust[0]*b.ust[0])>>16;
	c.ust[1]=(a.ust[1]*b.ust[1])>>16;
//	c.a=(((a.a&0x0000FFFFUL)*(b.a&0x0000FFFFUL))>>16)|
//		(((a.a>>16)*(b.a>>16))&0xFFFF0000UL);
	return(c);
}

static inline psimd32 psiMulshiP32ST(psimd32 a, psimd32 b)
{
	psimd32 c;
	c.sst[0]=(a.sst[0]*b.sst[0])>>16;
	c.sst[1]=(a.sst[1]*b.sst[1])>>16;
//	c.a=(((a.a&0x0000FFFFUL)*(b.a&0x0000FFFFUL))>>16)|
//		(((a.a>>16)*(b.a>>16))&0xFFFF0000UL);
	return(c);
}


static inline psimd64 psiAddP64ST(psimd64 a, psimd64 b)
{
	psimd64 c;
	c.a=((a.a+ b.a              )&0x0000FFFFUL)|
		((a.a+(b.a&0xFFFF0000UL))&0xFFFF0000UL);
	c.b=((a.b+ b.b              )&0x0000FFFFUL)|
		((a.b+(b.b&0xFFFF0000UL))&0xFFFF0000UL);
	return(c);
}

static inline psimd64 psiSubP64ST(psimd64 a, psimd64 b)
{
	psimd64 c;
	c.a=((a.a- b.a              )&0x0000FFFFUL)|
		((a.a-(b.a&0xFFFF0000UL))&0xFFFF0000UL);
	c.b=((a.b- b.b              )&0x0000FFFFUL)|
		((a.b-(b.b&0xFFFF0000UL))&0xFFFF0000UL);
	return(c);
}

static inline psimd64 psiMulloP64ST(psimd64 a, psimd64 b)
{
	psimd64 c;
	c.a=((a.a*b.a)&0x0000FFFFUL)|
		((a.a&0xFFFF0000UL)*(b.a&0xFFFF0000UL));
	c.c=((a.c*b.c)&0x0000FFFFUL)|
		((a.c&0xFFFF0000UL)*(b.c&0xFFFF0000UL));
	return(c);
}

static inline psimd64 psiMuluhiP64ST(psimd64 a, psimd64 b)
{
	psimd64 c;
	c.a=(((a.a&0x0000FFFFUL)*(b.a&0x0000FFFFUL))>>16)|
		(((a.a>>16)*(b.a>>16))&0xFFFF0000UL);
	c.b=(((a.b&0x0000FFFFUL)*(b.b&0x0000FFFFUL))>>16)|
		(((a.b>>16)*(b.b>>16))&0xFFFF0000UL);
	return(c);
}



static inline psimd32 psiAddP32XYZW(psimd32 a, psimd32 b)
{
	psimd32 c;
#if 1
	c.xyzw[0]=a.xyzw[0]+b.xyzw[0];
	c.xyzw[1]=a.xyzw[1]+b.xyzw[1];
	c.xyzw[2]=a.xyzw[2]+b.xyzw[2];
	c.xyzw[3]=a.xyzw[3]+b.xyzw[3];
#endif
#if 0
	c.a=((a.a+b.a)&0x000000FFUL)|
		((a.a+b.a)&0x0000FF00UL)|
		((a.a+b.a)&0x00FF0000UL)|
		((a.a+b.a)&0xFF000000UL);
#endif
	return(c);
}

static inline psimd32 psiSubP32XYZW(psimd32 a, psimd32 b)
{
	psimd32 c;
#if 1
	c.xyzw[0]=a.xyzw[0]-b.xyzw[0];
	c.xyzw[1]=a.xyzw[1]-b.xyzw[1];
	c.xyzw[2]=a.xyzw[2]-b.xyzw[2];
	c.xyzw[3]=a.xyzw[3]-b.xyzw[3];
#endif
#if 0
	c.a=((a.a-b.a)&0x000000FFUL)|
		((a.a-b.a)&0x0000FF00UL)|
		((a.a-b.a)&0x00FF0000UL)|
		((a.a-b.a)&0xFF000000UL);
#endif
	return(c);
}

static inline psimd32 psiMulloP32XYZW(psimd32 a, psimd32 b)
{
	psimd32 c;
#if 1
	c.xyzw[0]=a.xyzw[0]*b.xyzw[0];
	c.xyzw[1]=a.xyzw[1]*b.xyzw[1];
	c.xyzw[2]=a.xyzw[2]*b.xyzw[2];
	c.xyzw[3]=a.xyzw[3]*b.xyzw[3];
#endif
#if 0
	c.a=(((a.a&0x000000FFUL)*(b.a&0x000000FFUL))&0x000000FFUL)|
		(((a.a&0x0000FF00UL)*(b.a&0x0000FF00UL))&0x0000FF00UL)|
		(((a.a&0x00FF0000UL)*(b.a&0x00FF0000UL))&0x00FF0000UL)|
		(((a.a&0xFF000000UL)*(b.a&0xFF000000UL))&0xFF000000UL);
#endif
	return(c);
}

static inline psimd32 psiMuluhiP32XYZW(psimd32 a, psimd32 b)
{
	psimd32 c;

#if 1
	c.xyzw[0]=(a.xyzw[0]*b.xyzw[0])>>8;
	c.xyzw[1]=(a.xyzw[1]*b.xyzw[1])>>8;
	c.xyzw[2]=(a.xyzw[2]*b.xyzw[2])>>8;
	c.xyzw[3]=(a.xyzw[3]*b.xyzw[3])>>8;
#endif

#if 0
	c.a=((((a.a&0x000000FFUL)   )*((b.a&0x000000FFUL)    )>>8)&0x000000FFUL)|
		((((a.a&0x0000FF00UL)>>8)*((b.a&0x0000FF00UL)>> 8)   )&0x0000FF00UL)|
		((((a.a&0x00FF0000UL)>>8)*((b.a&0x00FF0000UL)>>16)   )&0x00FF0000UL)|
		((((a.a&0xFF000000UL)>>8)*((b.a&0xFF000000UL)>>24)   )&0xFF000000UL);
#endif
	return(c);
}

#endif
