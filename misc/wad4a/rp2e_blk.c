/*
RP2 Encoder, using a block-sorting encoder rather than hash chaining.
 */

#define HAVE_STDINT_H	/* Probably safe to assume at this point... */

#ifdef _MSC_VER
#if (_MSC_VER<1800)
#undef HAVE_STDINT_H	/* Older MSVC */
#endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#ifdef HAVE_STDINT_H
#include <stdint.h>

typedef uint8_t	byte;
typedef uint16_t	u16;
typedef uint32_t	u32;
typedef uint64_t	u64;

#else

typedef unsigned char byte;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

#endif


/* Portability Cruft... */

#ifdef _MSC_VER
#define force_inline	__forceinline
#define	debug_break		__debugbreak();
#ifdef _M_IX86
#define unaligned
#else
#define unaligned		__unaligned
#endif
#define HAVE_MISAL
#else

#if defined(__BGBCC__) || defined(__BJX2__)
#define force_inline	
#define	debug_break		__debugbreak();
#define unaligned		__unaligned
#define HAVE_MISAL
#else
#define force_inline
#define	debug_break	
#define unaligned
#endif

#endif

#if defined(__i386__) || defined(__x86_64__) || defined(__arm__)
#define HAVE_MISAL_MEMCPY
#endif

#ifdef HAVE_MISAL

#define	memcpy_4B(dst, src)		\
	(*(unaligned u32 *)(dst)=*(unaligned u32 *)(src))
#define	memcpy_8B(dst, src)		\
	(*(unaligned u64 *)(dst)=*(unaligned u64 *)(src))
#define	memcpy_16B(dst, src)	\
	(((unaligned u64 *)(dst))[0]=((unaligned u64 *)(src))[0]);	\
	(((unaligned u64 *)(dst))[1]=((unaligned u64 *)(src))[1])

#define	get_u16le(ptr)			(*(unaligned u16 *)(ptr))
#define	get_u32le(ptr)			(*(unaligned u32 *)(ptr))
#define	get_u64le(ptr)			(*(unaligned u64 *)(ptr))
#define	set_u16le(ptr, val)		(*(unaligned u16 *)(ptr))=(val)
#define	set_u32le(ptr, val)		(*(unaligned u32 *)(ptr))=(val)
#define	set_u64le(ptr, val)		(*(unaligned u64 *)(ptr))=(val)

#else

#define	memcpy_4B(dst, src)		memcpy(dst, src, 4)
#define	memcpy_8B(dst, src)		memcpy(dst, src, 8)
// #define	memcpy_16B(dst, src)	memcpy(dst, src, 16)
#define	memcpy_16B(dst, src)	\
	memcpy(((byte *)(dst))+0, ((byte *)(src))+0, 8);		\
	memcpy(((byte *)(dst))+8, ((byte *)(src))+8, 8);

#ifdef HAVE_MISAL_MEMCPY

force_inline u16 get_u16le(byte *ptr)
	{ u16 v; memcpy(&v, ptr, 2); return(v); }
force_inline u32 get_u32le(byte *ptr)
	{ u32 v; memcpy(&v, ptr, 4); return(v); }
force_inline u64 get_u64le(byte *ptr)
	{ u64 v; memcpy(&v, ptr, 8); return(v); }
force_inline void set_u16le(byte *ptr, u16 val)
	{ memcpy(ptr, &val, 2); }
force_inline void set_u32le(byte *ptr, u32 val)
	{ memcpy(ptr, &val, 4); }
force_inline void set_u64le(byte *ptr, u64 val)
	{ memcpy(ptr, &val, 8); }

#else

force_inline u16 get_u16le(byte *ptr)
	{ return(ptr[0]|(ptr[1]<<8)); }
force_inline u32 get_u32le(byte *ptr)
{	return(
		 ((u32)(ptr[0]))     |(((u32)(ptr[1]))<< 8) |
		(((u32)(ptr[2]))<<16)|(((u32)(ptr[3]))<<24) );		}
force_inline u64 get_u64le(byte *ptr)
{	return(
		 ((u64)(ptr[0]))     |(((u64)(ptr[1]))<< 8) |
		(((u64)(ptr[2]))<<16)|(((u64)(ptr[3]))<<24) |
		(((u64)(ptr[4]))<<32)|(((u64)(ptr[5]))<<40) |
		(((u64)(ptr[6]))<<48)|(((u64)(ptr[7]))<<56) );		}
force_inline void set_u16le(byte *ptr, u16 val)
{	ptr[0]=val;		ptr[1]=val>>8;			}
force_inline void set_u32le(byte *ptr, u32 val)
{	ptr[0]=val;			ptr[1]=val>>8;
	ptr[2]=val>>16;		ptr[3]=val>>24;		}
force_inline void set_u64le(byte *ptr, u64 val)
{	ptr[0]=val;			ptr[1]=val>>8;
	ptr[2]=val>>16;		ptr[3]=val>>24;
	ptr[4]=val>>32;		ptr[5]=val>>40;
	ptr[6]=val>>48;		ptr[7]=val>>56;		}

#endif

#endif


typedef struct Rp2BeLz_Context_s Rp2BeLz_Context;

struct Rp2BeLz_Context_s {
int		*ptrs_a;		//pointer block A
int		*ptrs_b;		//pointer block B
int		*chix_a;		//character index A
int		*chix_b;		//character index B
byte	*buf_a_css;		//buffer A start
byte	*buf_b_css;		//buffer B end
byte	*buf_a_cse;		//buffer A start
byte	*buf_b_cse;		//buffer B end
int		maxwin;			//maximum window size (also block size)
int		maxdist;		//maximum distance
int		maxlen;			//maximum match length

byte	*css;			//input buffer start
byte	*cse;			//input buffer end

u32		csum;
};

byte *Rp2BeLz_LoadFile(char *name, int *rsz)
{
	byte *buf;
	FILE *fd;
	int sz, i;
	
	fd=fopen(name, "rb");
	if(!fd)
	{
		return(NULL);
	}
	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	buf=malloc(sz+24);
	i=fread(buf, 1, sz, fd);
	fclose(fd);
	
	if(i!=sz)
	{
		if(i>0)
		{
			sz=i;
		}else
		{
			free(buf);
			return(NULL);
		}
	}
	
//	*(u64 *)(buf+sz+0)=0;
//	*(u64 *)(buf+sz+8)=0;
	memset(buf+sz, 0, 16);
	
	*rsz=sz;
	return(buf);
}

int Rp2BeLz_StoreFile(char *name, void *buf, int sz)
{
	FILE *fd;
	
	fd=fopen(name, "wb");
	if(!fd)
		return(-1);
	
	fwrite(buf, 1, sz, fd);
	fclose(fd);
	return(0);
}

int Rp2BeLz_SortComparePtrs(
	byte *css, byte *cse, int p0, int p1, int max)
{
	int l0, l1, l;
	byte *s0, *s1;
	int k;

	if(p0==p1)
		return(0);

	s0=css+p0;
	s1=css+p1;
	l=max;
	l0=cse-s0;
	l1=cse-s1;
	if(l0<l)l=l0;
	if(l1<l)l=l1;
	k=memcmp(s0, s1, l);
	return(k);
}

int Rp2BeLz_SortCompareStrPtr(
	byte *css, byte *cse, byte *str, int ptr, int max)
{
	int l0, l1, l;
	byte *s0, *s1;
	int k;

	s0=css+ptr;
	if(s0==str)
		return(0);
	
	l=max;
	l0=cse-s0;
	if(l0<l)l=l0;
	k=memcmp(str, s0, l);
	return(k);
}


int Rp2BeLz_SelSortBlockPtrs(Rp2BeLz_Context *ctx,
	int *ptrs, int cnt, byte *css, byte *cse, int ml)
{
	byte *s0, *s1;
	int l0, l1, l, pv;
	int i, j, k, n;

	for(i=0; i<cnt; i++)
	{
		for(j=i+1; j<cnt; j++)
		{
			k=Rp2BeLz_SortComparePtrs(css, cse,
				ptrs[i], ptrs[j], ml);
			if(k>0)
			{
				k=ptrs[i];
				ptrs[i]=ptrs[j];
				ptrs[j]=k;
			}
		}
	}
	return(0);
}

int Rp2BeLz_SortBlockPtrs(Rp2BeLz_Context *ctx,
	int *ptrs, int cnt, byte *css, byte *cse, int ml)
{
	byte *s0, *s1;
	int l0, l1, l, pv;
	int i, j, k, n;

	if(cnt<7)
	{
		Rp2BeLz_SelSortBlockPtrs(ctx, ptrs, cnt, css, cse, ml);
		return(0);
	}
	
	pv=ptrs[cnt/2];

	l0=ptrs[0];
	l1=ptrs[cnt-1];

#if 1
	k=Rp2BeLz_SortComparePtrs(css, cse, l0, l1, ml);
	if(k>0)
		{ j=l1; l1=l0; l0=j; }
	k=Rp2BeLz_SortComparePtrs(css, cse, l0, pv, ml);
	if(k>0)
		{ j=pv; pv=l0; l0=j; }
	k=Rp2BeLz_SortComparePtrs(css, cse, pv, l1, ml);
	if(k>0)
		{ j=pv; pv=l1; l1=j; }
#endif

	l0=0;
	l1=cnt;
	while(l1>l0)
	{
		k=Rp2BeLz_SortComparePtrs(css, cse, ptrs[l0], pv, ml);
		if(k>0)
		{
			l1--;
			k=ptrs[l0];
			ptrs[l0]=ptrs[l1];
			ptrs[l1]=k;
		}else
		{
			l0++;
		}
	}

	if((l0==0) || (cnt==l1))
	{
		Rp2BeLz_SelSortBlockPtrs(ctx, ptrs, cnt, css, cse, ml);
		return(0);
	}
	
	Rp2BeLz_SortBlockPtrs(ctx, ptrs+ 0, l0    , css, cse, ml);
	Rp2BeLz_SortBlockPtrs(ctx, ptrs+l0, cnt-l0, css, cse, ml);
	return(0);
}

int Rp2BeLz_RebuildBlockA(Rp2BeLz_Context *ctx)
{
	int *ptrs;
	byte *css, *cse, *s0;
	int i, j, k, ws, ml;
	
	ptrs=ctx->ptrs_a;
	css=ctx->buf_a_css;
	cse=ctx->buf_a_cse;
	ws=cse-css;
	for(i=0; i<ws; i++)
		{ ptrs[i]=i; }

	ml=ctx->maxlen;

	Rp2BeLz_SortBlockPtrs(ctx, ptrs, ws, css, cse, ml);

	j=0;
	for(i=0; i<ws; i++)
	{
		s0=css+ptrs[i];
		k=*s0;
		while(j<=k)
		{
			ctx->chix_a[j++]=i;
		}
	}
	k=256;
	while(j<=k)
		{ ctx->chix_a[j++]=ws; }
	
	return(0);
}

int Rp2BeLz_HashStr(Rp2BeLz_Context *ctx, byte *cs, int l)
{
	int *ps0, *ps1;
	byte *s0, *s1;
	int mw;

	if(!ctx->buf_a_css)
	{
		mw=ctx->maxwin;
		s0=ctx->css;
		s1=s0+mw;
		if(s1>ctx->cse)
			s1=ctx->cse;
		ctx->buf_a_css=s0;
		ctx->buf_a_cse=s1;

		ctx->buf_b_css=NULL;
		ctx->buf_b_cse=NULL;
		
		ctx->ptrs_a=malloc(mw*sizeof(int));
		ctx->ptrs_b=malloc(mw*sizeof(int));

		ctx->chix_a=malloc(258*sizeof(int));
		ctx->chix_b=malloc(258*sizeof(int));
		
		Rp2BeLz_RebuildBlockA(ctx);
	}

	if(cs>=ctx->buf_a_cse)
	{
		s0=ctx->buf_a_css;
		s1=ctx->buf_a_cse;
		ctx->buf_b_css=s0;
		ctx->buf_b_cse=s1;
		ps0=ctx->ptrs_a;	ps1=ctx->ptrs_b;
		ctx->ptrs_a=ps1;	ctx->ptrs_b=ps0;

		ps0=ctx->chix_a;	ps1=ctx->chix_b;
		ctx->chix_a=ps1;	ctx->chix_b=ps0;

		mw=ctx->maxwin;
		s0=ctx->buf_b_cse;
		s1=s0+mw;
		if(s1>ctx->cse)
			s1=ctx->cse;
		ctx->buf_a_css=s0;
		ctx->buf_a_cse=s1;
		
		Rp2BeLz_RebuildBlockA(ctx);
	}
	
	return(0);
}

int Rp2BeLz_HashByte(Rp2BeLz_Context *ctx, byte *cs)
{
	Rp2BeLz_HashStr(ctx, cs, 1);
	return(0);
}

int Rp2BeLz_MatchCmpStrsGetLen(byte *str0, byte *str1, int ml)
{
	byte *s0, *s1, *s0e;
	
	s0=str0;
	s1=str1;
	s0e=s0+ml;
	while((s0+8)<=s0e)
	{
		if((*(u64 *)s0)!=(*(u64 *)s1))
			break;
		s0+=8;
		s1+=8;
	}

	while(s0<=s0e)
	{
		if(*s0!=*s1)
			break;
		s0++; s1++;
	}
	return(s0-str0);
}

int Rp2BeLz_LookupMatchBlock(Rp2BeLz_Context *ctx,
	byte *cs, byte *cse, int bln,
	int *rl, int *rd)
{
	int *bptrs, *bchix;
	byte *bcss, *bcse;
	byte *s0;
	int bl, bd, cl, cd, cn;
	int lo, hi, mi, ml, md, mw;
	int i, j, k;
	
	if(bln)
	{
		bptrs=ctx->ptrs_b;
		bchix=ctx->chix_b;
		bcss=ctx->buf_b_css;
		bcse=ctx->buf_b_cse;
	}else
	{
		bptrs=ctx->ptrs_a;
		bchix=ctx->chix_a;
		bcss=ctx->buf_a_css;
		bcse=ctx->buf_a_cse;
	}
	
	mw=bcse-bcss;
	md=ctx->maxdist;
	ml=ctx->maxlen;

	if(mw<=0)
	{
		*rl=0;
		*rd=0;
		return(0);
	}

	j=cse-cs;
	if(j<ml)
		ml=j;
	
	i=*cs;
	lo=bchix[i];
	hi=bchix[i+1];
	while(hi>(lo+2))
	{
		mi=(lo+hi)/2;
		k=Rp2BeLz_SortCompareStrPtr(
			bcss, bcse, cs, bptrs[mi], ml);
		if(k>0)
			{ lo=mi; }
		else
			{ hi=mi; }
	}
	
	bl=0; bd=0; cn=24;
	for(i=0; i<512; i++)
	{
		j=(i>>1)^(((int)(i<<31))>>31);
		j=mi+j;
		if((j<0) || (j>=mw))
			continue;
		
		s0=bcss+bptrs[j];
		if(s0>=cs)
			continue;
		cd=cs-s0;
		if(cd>=md)
			continue;
		cl=Rp2BeLz_MatchCmpStrsGetLen(cs, s0, ml);
		
		if(cl>bl)
		{
			bl=cl;
			bd=cd;
		}else if((cd<bd) && (cl==bl))
		{
			bl=cl;
			bd=cd;
		}

		cn--;
		if(cn<=0)
			break;
	}
	
	*rl=bl;
	*rd=bd;
	return(bl>4);
}

int Rp2BeLz_LookupMatch(Rp2BeLz_Context *ctx,
	byte *str, byte *stre,
	int *rl, int *rd)
{
	int l0, l1, d0, d1, bl, bd;

	if(!ctx->buf_a_css)
	{
		*rl=0;
		*rd=0;
		return(0);
	}

	l0=0;
	l1=0;

	if(ctx->buf_a_css)
	{
		Rp2BeLz_LookupMatchBlock(ctx,
			str, stre, 0, &l0, &d0);
	}
	if(ctx->buf_b_css)
	{
		Rp2BeLz_LookupMatchBlock(ctx,
			str, stre, 1, &l1, &d1);
	}
	
	if(l0 && l1)
	{
		if(d0<d1)
		{
			bl=l0;	bd=d0;
			if(l1>bl)
				{ bl=l1; bd=d1; }
		}else
		{
			bl=l1;	bd=d1;
			if(l0>bl)
				{ bl=l0; bd=d0; }
		}
	}else if(l0)
		{ bl=l0; bd=d0; }
	else if(l1)
		{ bl=l1; bd=d1; }
	else
		{ bl=0; bd=0; }

	*rl=bl;
	*rd=bd;
//	return(bl>4);
	return((bl>=4) || ((bl>=3) && (bd<512)));
}



int tgvlz_log2u(int val)
{
	int i, v;
	
	v=val; i=0;
	while(v)
		{ i++; v=v>>1; }
	return(i);
}

int Rp2BeLz_EstRawCost(Rp2BeLz_Context *ctx, int rl)
{
	int i, j, c;

	c=rl;
	i=rl;
	while(i>=4096)
		{ c+=2; i-=4096; }
	if(i>(128+7))
		{ c+=2; i=i&7; }
	if(i>=8)
		{ c+=1; i=i&7; }
	return(c);
}

int Rp2BeLz_EstMatchCost(Rp2BeLz_Context *ctx, int rl, int bl, int bd)
{
	int c, i;

	c=Rp2BeLz_EstRawCost(ctx, rl);
	if((bl<=10) && (bd<512))
		{ c+=2; }
	else if((bl<=67) && (bd<8192))
		{ c+=3; }
	else if((bl<=515) && (bd<131072))
		{ c+=4; }
	else if((bl<=16383) && (bd<(1<<22)))
	{
		c+=1+((bl<128)?1:2)+((bd<32768)?2:3);
	}
	return(c);
}

int Rp2BeLz_LookupMatchB(Rp2BeLz_Context *ctx,
	byte *cs, byte *cse, byte *lcs,
	int *rl, int *rd)
{
	int l, d, l1, d1;
	int rl0, rl1, rl2;
	int mc0, mc1;
	int i, j, k;

	i=Rp2BeLz_LookupMatch(ctx, cs, cse, &l, &d);
	rl0=cs-lcs;
	rl1=rl0+1;	rl2=rl0+2;
	
	mc0=Rp2BeLz_EstMatchCost(ctx, rl0, l, d);
	mc1=Rp2BeLz_EstRawCost(ctx, rl0+l);
	
	if(mc0>=mc1)
	{
		/* Skip match if it costs more than it saves. */
		i=0;
	}
	
	if(i)
	{
		/* Check if a better match is just around the corner. */

		j=Rp2BeLz_LookupMatch(ctx, cs+1, cse, &l1, &d1);
		if(j && (l1>(l+1)) && ((rl0>>3)==(rl1>>3)))
			i=0;

		mc1=Rp2BeLz_EstMatchCost(ctx, rl1, l1, d1);
		if(j && ((mc1-l1)<=(mc0-l)))
			i=0;

		if(i)
		{
			j=Rp2BeLz_LookupMatch(ctx, cs+2, cse, &l1, &d1);

			mc1=Rp2BeLz_EstMatchCost(ctx, rl2, l1, d1);
			if(j && ((mc1-l1)<(mc0-l)))
				i=0;
		}
	}
	
	*rl=l;
	*rd=d;
	return(i);
}

int Rp2BeLz_EncodeBufferRP2(Rp2BeLz_Context *ctx,
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	byte *cs, *cse, *lcs;
	byte *ct;
	u32 v;
	int pl, pd;
	int l, d, rl, l1, d1;
	int i, j, k;
	
	ctx->css=ibuf;
	ctx->cse=ibuf+ibsz;
	cs=ibuf; cse=ibuf+ibsz;
	ct=obuf;
	
	pd=0; pl=0;
	
	lcs=cs;
	while(cs<cse)
	{
//		i=Rp2BeLz_LookupMatch(ctx, cs, cse, &l, &d);
		i=Rp2BeLz_LookupMatchB(ctx, cs, cse, lcs, &l, &d);

		if(!i)
		{
			Rp2BeLz_HashByte(ctx, cs);
			cs++;
			continue;
		}
			
		rl=cs-lcs;

//		ctx->stat_rlen[tgvlz_log2u(rl)]++;
//		ctx->stat_len[tgvlz_log2u(l)]++;
//		ctx->stat_dist[tgvlz_log2u(d)]++;

		while(rl>(128+7))
		{
			j=(rl>>3)-1;
			if(j>511)j=511;
			
			k=(j+1)*8;

			v=0x3F+(j<<7);
			*ct++=v;
			*ct++=v>>8;

			memcpy(ct, lcs, k);
			ct+=k;	lcs+=k;	rl-=k;
		}

		while(rl>=8)
		{
			j=(rl>>3)-1;
			if(j>15)j=15;
			
			k=(j+1)*8;
			*ct++=0x07+(j<<4);
			memcpy(ct, lcs, k);
			ct+=k;	lcs+=k;	rl-=k;
		}

		if((rl<8) && (l<=10) && (d<=511))
		{
			d1=d;
			l1=l-3;
			v=(d1<<7)|(l1<<4)|(rl<<1)|0;
			*ct++=v;
			*ct++=v>>8;
			memcpy(ct, lcs, rl);
			ct+=rl;
		}
		else
			if((rl<8) && (l<=67) && (d<=8191))
		{
			d1=d;
			l1=l-4;

			v=(d1<<11)|(l1<<5)|(rl<<2)|1;
			*ct++=v;
			*ct++=v>>8;
			*ct++=v>>16;
			memcpy(ct, lcs, rl);
			ct+=rl;
		}
		else
			if((rl<8) && (l<=515) && (d<=131071))
		{
			d1=d;
			l1=l-4;

			v=(d1<<15)|(l1<<6)|(rl<<3)|3;
			*ct++=v;
			*ct++=v>>8;
			*ct++=v>>16;
			*ct++=v>>24;

			memcpy(ct, lcs, rl);
			ct+=rl;
		}
		else
			if((rl<8) && (l<=0x3FFF) && (d<=0x3FFFFF))
		{
			*ct++=0x0F|(rl<<5);
			d1=d;
			l1=l-4;

			if(l1<128)
			{
				*ct++=(l1<<1);
			}else
			{
				v=(l1<<2)|1;
				*ct++=v;
				*ct++=v>>8;
			}

			if(d1<32768)
			{
				v=d1<<1;
				*ct++=v;
				*ct++=v>>8;
			}else
			{
				v=(d1<<2)|1;
				*ct++=v;
				*ct++=v>>8;
				*ct++=v>>16;
			}

			memcpy(ct, lcs, rl);
			ct+=rl;
		}
		else
		{
			debug_break
		}
		
		pl=l; pd=d;
		
		Rp2BeLz_HashStr(ctx, cs, l);
		cs+=l;
		lcs=cs;
	}

	rl=cs-lcs;
	
	while(rl>8)
	{
		j=(rl>>3)-1;
		if(j>15)j=15;
		
		k=(j+1)*8;
		*ct++=0x07+(j<<4);
		memcpy(ct, lcs, k);
		ct+=k;	rl-=k;	lcs+=k;
	}

	while(rl)
	{
		j=rl;
		if(j>3)j=3;

		*ct++=0x1F+(j<<6);
		memcpy(ct, lcs, j);
		ct+=j;	rl-=j;	lcs+=j;
	}

	*ct++=0x1F;
	
	return(ct-obuf);
}


u32 Rp2BeLz_CalculateImagePel4BChecksum(byte *buf, int size)
{
	byte *cs, *cse;
//	u32 v;
	u32 v0, v1, v2, v3;
	u64 acc_lo, acc_hi;
	u32 csum;
	
	cs=buf;
	cse=cs+size;
	acc_lo=1;
	acc_hi=0;

	while(cs<cse)
	{
		v0=get_u32le(cs+0);	v1=get_u32le(cs+ 4);
		v2=get_u32le(cs+8);	v3=get_u32le(cs+12);
		acc_lo=acc_lo+v0;	acc_hi=acc_hi+acc_lo;
		acc_lo=acc_lo+v1;	acc_hi=acc_hi+acc_lo;
		acc_lo=acc_lo+v2;	acc_hi=acc_hi+acc_lo;
		acc_lo=acc_lo+v3;	acc_hi=acc_hi+acc_lo;
		cs+=16;
	}

	acc_lo=((u32)acc_lo)+(acc_lo>>32);
	acc_lo=((u32)acc_lo)+(acc_lo>>32);
	acc_hi=((u32)acc_hi)+(acc_hi>>32);
	acc_hi=((u32)acc_hi)+(acc_hi>>32);
	csum=(u32)(acc_lo^acc_hi);
	return(csum);
}

int Rp2BeLz_DoEncode(Rp2BeLz_Context *ctx,
	byte *ibuf, byte *obuf, int isz)
{
	long long ttsz;
	byte *obuf2;
	double f, g;
	int t0, t1, te;
	int csum1, csum2;
	int osz, osz2;
	int i, j, k;

	t0=clock();
	osz=Rp2BeLz_EncodeBufferRP2(ctx, ibuf, obuf, isz, 2*isz);
	t1=clock();
	
//	printf("%s: %d -> %d\n", ctx->tstName, isz, osz);
	printf("%d -> %d, %dms\n", isz, osz, t1-t0);

	csum1=Rp2BeLz_CalculateImagePel4BChecksum(ibuf, isz);	
	ctx->csum=csum1;
	return(osz);
}



#ifndef TGVLZ_NOMAIN
int main(int argc, char *argv[])
{
	Rp2BeLz_Context *ctx;
	long long ttsz;
	byte *ibuf;
	byte *obuf;
	byte *obuf2;
	char *ifn, *ofn;
	double f, g;
	int t0, t1, te;
	int isz, csz, osz, osz2, dsz, dsum, csum, mode;
	int i, j, k;
	
	ifn=NULL;
	ofn=NULL;
	
	mode=0;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "-z"))
				mode=1;
			if(!strcmp(argv[i], "-d"))
				mode=2;
			if(!strcmp(argv[i], "-b"))
				mode=3;
			if(!strcmp(argv[i], "-t"))
				mode=4;
			continue;
		}
		
		if(!ifn)
		{
			ifn=argv[i];
			continue;
		}

		if(!ofn)
		{
			ofn=argv[i];
			continue;
		}
	}
	
	if((mode==0) && !ifn)
	{
		printf("usage: %s [opts*] infile [outfile]\n", argv[0]);
		printf("\t-z\t\tEncode\n");
		printf("\t-d\t\tDecode\n");
		printf("\t-b\t\tBenchmark\n");
	}
	
	ibuf=NULL;

	if(ifn)
	{
		ibuf=Rp2BeLz_LoadFile(ifn, &isz);
	}else
	{
		printf("%s: no input file\n", argv[0]);
		return(-1);
	}
	
	ctx=malloc(sizeof(Rp2BeLz_Context));
	memset(ctx, 0, sizeof(Rp2BeLz_Context));
//	ctx->maxlen=16383;
//	ctx->maxdist=(1<<22)-1;

	ctx->maxlen=16383;
//	ctx->maxdist=(1<<18)-17;
//	ctx->maxwin=1<<18;

	ctx->maxdist=(1<<20)-17;
	ctx->maxwin=1<<20;

#if 0
	if((mode==3) || ((mode==0) && !ofn))
	{
		obuf=malloc(isz*2);
		osz=Rp2BeLz_DoTest(ctx, ibuf, obuf+16, isz);
		
		memcpy(obuf, "RP2A", 4);
		set_u32le(obuf+ 4, osz);
		set_u32le(obuf+ 8, isz);
		set_u32le(obuf+12, ctx->csum);

		if(ofn)
		{
			Rp2BeLz_StoreFile(ofn, obuf, osz+16);
		}
		return(0);
	}
#endif
	
	if((mode==1) || ((mode==0) && ofn))
	{
		obuf=malloc(isz*2);
		osz=Rp2BeLz_DoEncode(ctx, ibuf, obuf+16, isz);
		
		memcpy(obuf, "RP2A", 4);
		set_u32le(obuf+ 4, osz);
		set_u32le(obuf+ 8, isz);
		set_u32le(obuf+12, ctx->csum);

		if(ofn)
		{
			Rp2BeLz_StoreFile(ofn, obuf, osz+16);
		}
		return(0);
	}

#if 0
	if(((mode==2) && ofn) || (mode==4))
	{
		if(memcmp(ibuf, "RP2A", 4))
		{
			printf("Magic Fail\n");
			return(0);
		}
		
		osz=get_u32le(ibuf+4);
		dsz=get_u32le(ibuf+8);
		dsum=get_u32le(ibuf+12);

		obuf=malloc(dsz*2);
		osz2=ctx->DecodeBuffer(ibuf+16, obuf, osz, dsz*2);
		memset(obuf+osz2, 0, 16);
		
		csum=Rp2BeLz_CalculateImagePel4BChecksum(obuf, osz2);

		if(osz2!=dsz)
		{
			printf("Size mismatch\n");
		}else
		{
			if(csum!=dsum)
			{
				printf("Checksum mismatch %08X->%08X\n", dsum, csum);
			}else
			{
				if(mode==4)
				{
					printf("OK\n");
				}
			}
		}

		if(ofn)
		{
			Rp2BeLz_StoreFile(ofn, obuf, osz2);
		}
		return(0);
	}
#endif

	return(0);
}
#endif
