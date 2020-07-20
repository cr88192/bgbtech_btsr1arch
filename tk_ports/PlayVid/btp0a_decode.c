/*
Only support 2-color modes for now.

Block, 64-bit:
  (63:48): ColorB
  (47:32): ColorA
  (31:16): Pixel Bits
  (15: 8): ?
  ( 7: 0): Tag

  0000: Flat, ColorA
  0010: Flat, ColorB
  0001: 2x2x1
  0011: 4x4x1

 */

#define		BTPIC_TWOCC(a, b)			((a)|((b)<<8))
#define		BTPIC_FOURCC(a, b, c, d)	((a)|((b)<<8)|((c)<<16)|((d)<<24))

#define		BTPIC_TCC_IX	BTPIC_TWOCC('I', 'X')
#define		BTPIC_TCC_PX	BTPIC_TWOCC('I', 'X')
#define		BTPIC_TCC_IZ	BTPIC_TWOCC('I', 'Z')
#define		BTPIC_TCC_PZ	BTPIC_TWOCC('I', 'Z')
#define		BTPIC_TCC_HX	BTPIC_TWOCC('H', 'X')
#define		BTPIC_TCC_PT	BTPIC_TWOCC('P', 'T')

#define		BTPIC_TCC_Z3	BTPIC_TWOCC('Z', '3')
#define		BTPIC_TCC_Z4	BTPIC_TWOCC('Z', '4')

#if 0

#ifndef BYTE_T
#define BYTE_T
typedef	uint8_t		byte;
#endif

#ifndef SBYTE_T
#define SBYTE_T
typedef	int8_t		sbyte;
#endif

#ifndef PDLIB_INT_BITS_T
#define PDLIB_INT_BITS_T
typedef	uint16_t	u16;
typedef	uint32_t	u32;
typedef	uint64_t	u64;
typedef	int16_t		s16;
typedef	int32_t		s32;
typedef	int64_t		s64;
#endif

#endif

#ifndef unaligned
// #define		unaligned	__unaligned
#define		unaligned
#endif

#define		btpic_getu16(p)		(*(unaligned u16 *)(p))
#define		btpic_getu32(p)		(*(unaligned u32 *)(p))
#define		btpic_getu64(p)		(*(unaligned u64 *)(p))

#define		btpic_setu16(p, v)	((*(unaligned u16 *)(p))=(v))
#define		btpic_setu32(p, v)	((*(unaligned u32 *)(p))=(v))
#define		btpic_setu64(p, v)	((*(unaligned u64 *)(p))=(v))

typedef struct BTPIC0A_DecodeContext_s BTPIC0A_DecodeContext;

struct BTPIC0A_DecodeContext_s {
	u64 *blka;
	u64 *lblka;
	int bxs;
	int bys;
	u16 pal[256];

	int xs;
	int ys;
	int fl;
	
	byte *zfbuf;		//LZ Frame temporary buffer
	int zfbsz;			//LZ Frame buffer size
};


#ifdef __BJX2__
// #if 0
int BTPIC_DecodeBufferRP2(
	byte *ibuf, byte *obuf, int ibsz, int obsz);

__asm {
BTPIC_DecodeBufferRP2:
// R4=ibuf
// R5=obuf
// R6=ibsz / scratch
// R7=obsz / scratch
// R16=scratch
// R17=scratch
// R18=cs
// R19=ct
// R20=rl
// R21=l
// R22=d

	MOV R4, R18
	MOV R5, R19
	
.L0:
	MOV.Q	(R18), R16
	TEST	0x01, R16
	BT		.L2
	TEST	0x02, R16
	BT		.L3
	TEST	0x04, R16
	BT		.L4
	TEST	0x08, R16
	BT		.L5
	TEST	0x10, R16
	BT		.L6
	TEST	0x20, R16
	BT		.L7
	TEST	0x40, R16
	BT		.L8
	BREAK
.L2:
	ADD		2, R18		|	SHLD	R16, -1, R20
	AND		7, R20		|	SHLD	R16, -4, R21
	AND		7, R21		|	SHLD	R16, -7, R22
	AND		511, R22	|	ADD		3, R21	
	BRA		.L9
.L3:
	MOV		8191, R3
	ADD		3, R18	|	SHLD	R16, -2, R20
	AND		7, R20	|	SHLD	R16, -5, R21
	AND		63, R21	|	SHLD	R16, -11, R22
	AND		R3, R22	|	ADD		4, R21	
	BRA		.L9
.L4:
	ADD		4, R18		|	SHLD	R16, -3, R20
	AND		7, R20		|	SHLD	R16, -6, R21
	AND		511, R21	|	SHLD	R16, -15, R22
	ADD		4, R21
	BRA		.L9
.L5:
	ADD		1, R18		|	SHAD	R16, -1, R20
	AND		0x78, R20
	ADD		8, R20
.L5_1:
	ADD		R18, R20, R6	|	ADD		R19, R20, R7
.L5_0:
	MOV.Q	(R18, 0), R16
	MOV.Q	(R18, 8), R17
	ADD		16, R18
	MOV.Q	R16, (R19, 0)
	MOV.Q	R17, (R19, 8)
	ADD		16, R19
	CMPGT	R19, R7
	BT		.L5_0
	MOV		R6, R18		|	MOV		R7, R19
	BRA		.L0
.L6:
	MOV		0x3FFF, R1
	MOV		0x7FFF, R2
	MOV		0x3FFFFF, R3
	ADD			1, R18			|	SHLD		R16, -5, R20
	AND			7, R20			|	TEST		256, R16
	ADD?T		1, R18			|	ADD?F		2, R18
	SHLD?T		R16, -9, R21	|	SHLD?F		R16, -10, R21
	SHLD.Q?T	R16, -16, R7	|	SHLD.Q?F	R16, -24, R7
	AND?T		127, R21		|	AND?F		R1, R21
	ADD			4, R21			|	TEST		1, R7
	ADD?T		2, R18			|	ADD?F		3, R18
	SHLD?T		R7, -1, R22		|	SHLD?F		R7, -2, R22
	AND?T		R2, R22			|	AND?F		R3, R22
	BRA		.L9
.L7:
	ADD		1, R18		|	SHLD	R16, -6, R20
	AND		3, R20
	TEST	R20, R20
	BT		.L1
	MOV.L	(R18), R16
	ADD		R20, R18
	MOV.L	R16, (R19)
	ADD		R20, R19
	BRA		.L0
.L8:
	ADD		2, R18		|	SHLD	R16, -7, R20
	AND		511, R20
	ADD		1, R20
	SHLD	R20, 3, R20
	BRA		.L5_1
.L9:
	MOV.Q	(R18), R16
	ADD		R20, R18
	MOV.Q	R16, (R19)
	ADD		R20, R19
	SUB		R19, R22, R6
	ADD		R19, R21, R7
	CMPGT	15, R22
	BT		.L11
	MOV.Q	(R6, 0), R16
	MOV.Q	(R6, 8), R17
.L12:
	MOV.Q	R16, (R19, 0)
	MOV.Q	R17, (R19, 8)
	ADD		R22, R19
	CMPGT	R19, R7
	BT		.L12
	BRA		.L10
.L11:
	MOV.Q	(R6, 0), R16
	MOV.Q	(R6, 8), R17
	ADD		16, R6
	MOV.Q	R16, (R19, 0)
	MOV.Q	R17, (R19, 8)
	ADD		16, R19
	CMPGT	R19, R7
	BT		.L11
.L10:
	MOV		R7, R19
	BRA		.L0
.L1:
	SUB		R19, R5, R2
	RTS
};
#endif

#ifndef __BJX2__
//#if 1
int BTPIC_DecodeBufferRP2(
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	u32 tag;
	byte *cs, *ct, *cse, *cs1, *cs1e, *ct1e;
	int pl, pd;
	int rl, l, d;
	u64 t0, v0, v1;
	int t1, t2;
	
	cs=ibuf; cse=ibuf+ibsz;
	ct=obuf;
	pl=0; pd=0;
	
	while(1)
	{
		t0=*(u64 *)cs;
		if(!(t0&0x01))
		{
			cs+=2;
			rl=(t0>>1)&7;
			l=((t0>>4)&7)+3;
			d=(t0>>7)&511;
		}else
			if(!(t0&0x02))
		{
			cs+=3;
			rl=(t0>>2)&7;
			l=((t0>>5)&63)+4;
			d=(t0>>11)&8191;
		}else
			if(!(t0&0x04))
		{
			cs+=4;
			rl=(t0>>3)&7;
			l=((t0>>6)&511)+4;
			d=(t0>>15)&131071;
		}else
			if(!(t0&0x08))
		{
			cs++;
			t1=(t0>>4)&15;
			rl=(t1+1)*8;
//			W_RawCopyB(ct, cs, rl);
//			cs+=rl;
//			ct+=rl;

			cs1e=cs+rl;
			ct1e=ct+rl;
			while(ct<ct1e)
			{
				v0=((u64 *)cs)[0];
				v1=((u64 *)cs)[1];
				cs+=16;
				((u64 *)ct)[0]=v0;
				((u64 *)ct)[1]=v1;
				ct+=16;
			}			
			cs=cs1e;
			ct=ct1e;

			continue;
		}else
			if(!(t0&0x10))
		{
			/* Long Match */
			cs++;
			rl=(t0>>5)&7;
			t1=t0>>8;
			if(!(t1&1))
				{ l=((t1>>1)&0x007F)+4; cs+=1; t2=t0>>16; }
			else
				{ l=((t1>>2)&0x3FFF)+4; cs+=2; t2=t0>>24; }
			if(!(t2&1))
				{ d=((t2>>1)&0x007FFF); cs+=2; }
			else
				{ d=((t2>>2)&0x3FFFFF); cs+=3; }
		}else
			if(!(t0&0x20))
		{
			cs++;
			rl=(t0>>6)&3;
			if(!rl)break;
			*(u32 *)ct=*(u32 *)cs;
			cs+=rl;
			ct+=rl;
			continue;
		}else
			if(!(t0&0x40))
		{
			/* Long Raw */
			cs+=2;
			t1=(t0>>7)&511;
			rl=(t1+1)*8;
//			W_RawCopyB(ct, cs, rl);
//			cs+=rl;
//			ct+=rl;

			cs1e=cs+rl;
			ct1e=ct+rl;
			while(ct<ct1e)
			{
				v0=((u64 *)cs)[0];
				v1=((u64 *)cs)[1];
				cs+=16;
				((u64 *)ct)[0]=v0;
				((u64 *)ct)[1]=v1;
				ct+=16;
			}			
			cs=cs1e;
			ct=ct1e;
			continue;
		}else
		{
			__debugbreak();
		}

		*(u64 *)ct=*(u64 *)cs;
		cs+=rl;
		ct+=rl;
		
		cs1=ct-d;
		ct1e=ct+l;
		
		if(d<16)
		{
			v0=((u64 *)cs1)[0];
			v1=((u64 *)cs1)[1];
			while(ct<ct1e)
			{
				((u64 *)ct)[0]=v0;
				((u64 *)ct)[1]=v1;
				ct+=d;
			}
		}else
		{
			while(ct<ct1e)
			{
				v0=((u64 *)cs1)[0];
				v1=((u64 *)cs1)[1];
				cs1+=16;
				((u64 *)ct)[0]=v0;
				((u64 *)ct)[1]=v1;
				ct+=16;
			}
		}
		
		ct=ct1e;
		
//		W_MatchCopy2(ct, l, d);
//		ct+=l;
	}
	
	return(ct-obuf);
}
#endif


BTPIC0A_DecodeContext *BTPIC0A_AllocDecodeContext(void)
{
	BTPIC0A_DecodeContext *ctx;
	ctx=malloc(sizeof(BTPIC0A_DecodeContext));
	memset(ctx, 0, sizeof(BTPIC0A_DecodeContext));
	return(ctx);
}

void BTPIC0A_DecodeImageBlock(u64 blk, u16 *dptr, int ystr)
{
	u16 *ct;
	u64 v;
	int clra, clrb, pix, tg;
	int t0, t1, t2, t3;

	tg=blk;
	clra=(blk>>32);
	clrb=(blk>>48);
	pix=blk>>16;
	ct=dptr;

//	clra=0x7FFF;
//	clrb=0x7FFF;

	if(!(tg&1))
	{
//		if(tg&0x02)		{ t0=clrb; } else { t0=clra; }
		if(tg&0x02)		{ t0=(u16)clrb; } else { t0=(u16)clra; }
		v=t0|(t0<<16);			v=v|(v<<32);
		btpic_setu64(ct, v);	ct+=ystr;
		btpic_setu64(ct, v);	ct+=ystr;
		btpic_setu64(ct, v);	ct+=ystr;
		btpic_setu64(ct, v);
		return;

#if 0
		ct[0]=t0;	ct[1]=t0;	ct[2]=t0;	ct[3]=t0;
		ct+=ystr;
		ct[0]=t0;	ct[1]=t0;	ct[2]=t0;	ct[3]=t0;
		ct+=ystr;
		ct[0]=t0;	ct[1]=t0;	ct[2]=t0;	ct[3]=t0;
		ct+=ystr;
		ct[0]=t0;	ct[1]=t0;	ct[2]=t0;	ct[3]=t0;
		return;
#endif
	}

	if(!(tg&2))
	{
		if(pix&0x0001)	{ t0=clrb; } else { t0=clra; }
		if(pix&0x0002)	{ t1=clrb; } else { t1=clra; }
		if(pix&0x0004)	{ t2=clrb; } else { t2=clra; }
		if(pix&0x0008)	{ t3=clrb; } else { t3=clra; }
		ct[0]=t0;	ct[1]=t0;	ct[2]=t1;	ct[3]=t1;
		ct+=ystr;
		ct[0]=t0;	ct[1]=t0;	ct[2]=t1;	ct[3]=t1;
		ct+=ystr;
		ct[0]=t2;	ct[1]=t2;	ct[2]=t3;	ct[3]=t3;
		ct+=ystr;
		ct[0]=t2;	ct[1]=t2;	ct[2]=t3;	ct[3]=t3;
		return;
	}

	if(!(tg&4))
	{
		if(pix&0x0001)	{ t0=clrb; } else { t0=clra; }
		if(pix&0x0002)	{ t1=clrb; } else { t1=clra; }
		if(pix&0x0004)	{ t2=clrb; } else { t2=clra; }
		if(pix&0x0008)	{ t3=clrb; } else { t3=clra; }
		ct[0]=t0;	ct[1]=t1;	ct[2]=t2;	ct[3]=t3;
		ct+=ystr;
		if(pix&0x0010)	{ t0=clrb; } else { t0=clra; }
		if(pix&0x0020)	{ t1=clrb; } else { t1=clra; }
		if(pix&0x0040)	{ t2=clrb; } else { t2=clra; }
		if(pix&0x0080)	{ t3=clrb; } else { t3=clra; }
		ct[0]=t0;	ct[1]=t1;	ct[2]=t2;	ct[3]=t3;
		ct+=ystr;
		if(pix&0x0100)	{ t0=clrb; } else { t0=clra; }
		if(pix&0x0200)	{ t1=clrb; } else { t1=clra; }
		if(pix&0x0400)	{ t2=clrb; } else { t2=clra; }
		if(pix&0x0800)	{ t3=clrb; } else { t3=clra; }
		ct[0]=t0;	ct[1]=t1;	ct[2]=t2;	ct[3]=t3;
		ct+=ystr;
		if(pix&0x1000)	{ t0=clrb; } else { t0=clra; }
		if(pix&0x2000)	{ t1=clrb; } else { t1=clra; }
		if(pix&0x4000)	{ t2=clrb; } else { t2=clra; }
		if(pix&0x8000)	{ t3=clrb; } else { t3=clra; }
		ct[0]=t0;	ct[1]=t1;	ct[2]=t2;	ct[3]=t3;
		return;
	}
}

void BTPIC0A_DecodeBlockImage(u16 *dptr, u64 *blka, int bxs, int bys, int ystr)
{
	u64 *cs, *cse;
	u16 *ct0, *ct1;
	u64 blk;
	int y;
	
	cs=blka; ct0=dptr;
	for(y=0; y<bys; y++)
	{
		cse=cs+bxs;
		ct1=ct0;
		ct0+=ystr<<2;
		while(cs<cse)
		{
			blk=*cs;
			if(!(blk&0x80))
			{
				*cs=blk|0x80;	//mark already decoded
				BTPIC0A_DecodeImageBlock(*cs, ct1, ystr);
			}
			cs++;
			ct1+=4;
		}
	}
}

void BTPIC0A_DecodeCopyBlocks(
	u64 *ct0, u64 *blka, u64 *lblka, int cnt,
	int dvx, int dvy, int bxs, int bys)
{
	u64 *ct, *csl;
	u64 blk;
	int n;
	
	if(blka==lblka)
		return;

	n=cnt;
	ct=ct0;
	if(dvx|dvy)
	{
		csl=lblka+(ct-blka)+(dvy*bxs)+dvx;
		while(n--)
		{
			blk=*csl++;
			blk&=~0x80;
			*ct++=blk;
		}
	}else
	{
		csl=lblka+(ct-blka);
		while(n--)
		{
			blk=*csl++;
			*ct++=blk;
		}
	}
}

void BTPIC0A_DecodeFrameData(BTPIC0A_DecodeContext *ctx, byte *cdat, int csz)
{
	u64 *blka;
	u64 *lblka;
	u16 *pal;
	int bxs;
	int bys;

	u64 *ct, *csl, *cte;
	byte *cs, *cse;
	int ca, cb, px, n;
	int dx, dy, dvx, dvy;
	u64 tgv, blk;
	int i, j, k;

	blka=ctx->blka;
	lblka=ctx->lblka;
	pal=ctx->pal;
	bxs=ctx->bxs;
	bys=ctx->bys;

	ca=0; cb=0;
	ct=blka; dvx=0; dvy=0;
	cs=cdat; cse=cdat+csz;
	cte=blka+(bxs*bys);
	while(cs<cse)
	{
		if(ct>=cte)
		{
			__debugbreak();
		}

		tgv=btpic_getu64(cs);
		if(!(tgv&1))
		{
			cs+=4;
			ca=(tgv>>16)&255;	cb=(tgv>>24)&255;	px=tgv&65535;
			ca=pal[ca];			cb=pal[cb];
			blk=(((u64)px)<<16)|(((u64)ca)<<32)|(((u64)cb)<<48)|3;
			*ct++=blk;
			continue;
		}

		if(!(tgv&2))
		{
			cs++;
			dx=(tgv>>2)&7;
			dy=(tgv>>5)&7;
			if(dx==7)
			{
				n=dy+1;
			}else if(dy==7)
			{
				dvx=0; dvy=0;
				n=dx+1;

			}else
			{
				dvx+=dx-3;
				dvy+=dy-3;
				n=1;
			}
			BTPIC0A_DecodeCopyBlocks(
				ct, blka, lblka, n,
				dvx, dvy, bxs, bys);
			ct+=n;
			
			continue;
		}

		if(!(tgv&4))
		{
			i=(tgv>>3)&7;
			if(!(i&4))
			{
				cs+=2;

				if(!i)
				{
					n=((tgv>>6)&1023)+1;
					BTPIC0A_DecodeCopyBlocks(
						ct, blka, lblka, n,
						dvx, dvy, bxs, bys);
					ct+=n;
					continue;
				}else if(i==1)
				{
					n=((tgv>>6)&3)+1;
					dvx=((tgv>>8)&15)-7;
					dvy=((tgv>>12)&15)-7;
					BTPIC0A_DecodeCopyBlocks(
						ct, blka, lblka, n,
						dvx, dvy, bxs, bys);
					ct+=n;
					continue;
				}

				n=((tgv>>6)&3)+1;
				if(i==2)
					{ ca=(tgv>>8)&255; ca=pal[ca]; }
				else
					{ cb=(tgv>>8)&255; cb=pal[cb]; }
				blk=(((u64)ca)<<32)|(((u64)cb)<<48)|((i<<1)&2);
				while(n--)
					{ *ct++=blk; }
				continue;
			}else
			{
				cs++;
				n=((tgv>>6)&3)+1;

				if(i==4)
				{
					blk=(((u64)ca)<<32)|(((u64)cb)<<48)|1;
					while(n--)
					{
						j=*cs++;
						*ct++=blk|((j&0x0F)<<16);
						*ct++=blk|((j&0xF0)<<12);
					}
					continue;
				}else if(i==5)
				{
					blk=(((u64)ca)<<32)|(((u64)cb)<<48)|3;
					while(n--)
					{
						px=*(u16 *)cs;	cs+=2;
						*ct++=blk|(((u64)px)<<16);
					}
					continue;
				}else
				{
					/* Not supported */
					__debugbreak();
				}
			}
		}

		if(!(tgv&8))
		{
			cs++;
			px=(tgv>>4)&15;
			blk=(((u64)ca)<<32)|(((u64)cb)<<48)|(px<<16)|1;
			*ct++=blk;
			continue;
		}

		if(!(tgv&16))
		{
			cs++;
			n=((tgv>>6)&3)+1;
			i=tgv>>5;

			blk=(((u64)ca)<<32)|(((u64)cb)<<48)|((i<<1)&2);
			while(n--)
				{ *ct++=blk; }
			continue;
		}

		__debugbreak();
	}
}

int BTPIC0A_DecodeFrame(BTPIC0A_DecodeContext *ctx,
	byte *cdat, int csz, u16 *img, int ystr)
{
	u64 *tblk;
	byte *cs, *cs0, *cs1, *cs2, *cse;
	byte *fdat;
	u32 fcc;
	int sz, dsz, fsz;
	int i, j, k, n;
	
	if(!csz)
		return(0);

//	printf("BTPIC0A_DecodeFrame: %p %d %p %d\n", cdat, csz, img, ystr);
	
	fdat=NULL;
	cs=cdat; cse=cdat+csz;
	while(cs<cse)
	{
		i=cs[0];
		if((i>>5)==0)
		{
			sz=((i&31)<<8)|cs[1];
			fcc=btpic_getu16(cs+2);
			fcc=(u16)fcc;
			cs0=cs+4;	cs1=cs+sz;
		}else if((i>>5)==1)
		{
			sz=((i&31)<<8)|cs[1];
			fcc=btpic_getu32(cs+2);
			cs0=cs+6;	cs1=cs+sz;
		}else if((i>>5)==2)
		{
			sz=((i&31)<<24)|(cs[1]<<16)|(cs[2]<<8)|cs[3];
			fcc=btpic_getu16(cs+4);
			fcc=(u16)fcc;
			cs0=cs+6;	cs1=cs+sz;
		}else if((i>>5)==3)
		{
			sz=((i&31)<<24)|(cs[1]<<16)|(cs[2]<<8)|cs[3];
			fcc=btpic_getu32(cs+4);
			cs0=cs+8;	cs1=cs+sz;
		}else if((i>>5)==4)
		{
			sz=((i&31)<<16)|(cs[1]<<8)|cs[2];
			fcc=btpic_getu16(cs+3);
			fcc=(u16)fcc;
			cs0=cs+5;	cs1=cs+sz;
		}else if((i>>5)==5)
		{
			sz=((i&31)<<16)|(cs[1]<<8)|cs[2];
			fcc=btpic_getu32(cs+3);
			cs0=cs+7;	cs1=cs+sz;
		}

//		printf("%0.4s %d\n", &fcc, cs1-cs0);

		if((fcc==BTPIC_TCC_Z3) || (fcc==BTPIC_TCC_Z4))
		{
			k=btpic_getu32(cs0+0);
			dsz=k&0xFFFFFF;
			cs2=cs0+4;
			
			if(!(ctx->zfbuf))
			{
				i=65536;
				while(i<=(dsz+64))
					i=i+(i>>1);
				ctx->zfbuf=malloc(i);
				ctx->zfbsz=i;
			}
			
			if((dsz+64)>(ctx->zfbsz))
			{
				i=ctx->zfbsz;
				while(i<=(dsz+64))
					i=i+(i>>1);
				ctx->zfbuf=realloc(ctx->zfbuf, i);
				ctx->zfbsz=i;
			}

			if(fcc==BTPIC_TCC_Z3)
			{
				BTPIC_DecodeBufferRP2(
					cs2, ctx->zfbuf,
					cs1-cs2, ctx->zfbsz);
			}
			
//			BTPIC0A_DecodeFrame(ctx, ctx->zfbuf, ctx->zfbsz, img, ystr);
			BTPIC0A_DecodeFrame(ctx, ctx->zfbuf, dsz, img, ystr);
			
			cs=cs1;
			continue;
		}
		
		if(fcc==BTPIC_TCC_HX)
		{
			ctx->xs=btpic_getu16(cs0+0);
			ctx->ys=btpic_getu16(cs0+2);
			ctx->fl=btpic_getu16(cs0+4);

//			ctx->xs=btpic_getu16(cs0+0)&65535;
//			ctx->ys=btpic_getu16(cs0+2)&65535;
//			ctx->fl=btpic_getu16(cs0+4)&65535;
			ctx->bxs=(ctx->xs+3)>>2;
			ctx->bys=(ctx->ys+3)>>2;

			if(!ctx->blka)
			{
				if(ctx->fl&1)
				{
					ctx->blka=malloc(ctx->bxs*ctx->bys*2*sizeof(u64));
					ctx->lblka=ctx->blka;
				}else
				{
					ctx->blka=malloc(ctx->bxs*ctx->bys*2*sizeof(u64));
					ctx->lblka=malloc(ctx->bxs*ctx->bys*2*sizeof(u64));
				}
			}
			
			cs=cs1;
			continue;
		}

		if(fcc==BTPIC_TCC_PT)
		{
			n=(cs1-cs0)/2;
			for(i=0; i<n; i++)
				{ ctx->pal[i]=btpic_getu16(cs0+(i*2)); }
			cs=cs1;
			continue;
		}

		if(	(fcc==BTPIC_TCC_IX) ||
			(fcc==BTPIC_TCC_PX)	)
		{
			fdat=cs0; fsz=cs1-cs0;
			cs=cs1;
			continue;
		}

		cs=cs1;
		continue;
	}
	
	if(fdat)
	{
		if(!ctx->blka)
			return(-1);
		
		tblk=ctx->blka;
		ctx->blka=ctx->lblka;
		ctx->lblka=tblk;
	
		BTPIC0A_DecodeFrameData(ctx, fdat, fsz);
		
		if(img)
		{
			BTPIC0A_DecodeBlockImage(
				img, ctx->blka,
				ctx->bxs, ctx->bys, ystr);
		}else
		{
//			printf("BTPIC0A_DecodeFrame: No Image\n");
		}
	}else
	{
//		printf("BTPIC0A_DecodeFrame: No FDat\n");
	}
	
	return(0);
}
