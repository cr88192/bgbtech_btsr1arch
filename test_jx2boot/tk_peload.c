// extern u64 __arch_gbr;

byte tkpe_imgend;

byte *TKPE_UnpackL4(byte *ct, byte *ibuf, int isz)
{
	byte *cs, *cse;
//	register byte *cs1, *cs1e, *ct1;
	byte *cs1, *cs1e, *ct1;
//	register int tg, lr, ll, ld;
	u64 tv;
	int tg, lr, ll, ld;
	int i;
	
	__hint_use_egpr();
	
	tg=0;	lr=0;
	ll=0;	ld=0;
	
	cs=ibuf; cse=ibuf+isz;
	while(cs<cse)
	{
		tg=*cs++;
		lr=(tg>>4)&15;
		if(lr==15)
		{
			i=*cs++;
			while(i==255)
				{ lr+=255; i=*cs++; }
			lr+=i;
		}
		
		ct1=ct; cs1=cs; cs1e=cs+lr;
		while(cs1<cs1e)
		{
			*(u64 *)ct1=*(u64 *)cs1;
			ct1+=8; cs1+=8;
		}
		ct+=lr; cs+=lr;
		
		if((cs+1)>=cse)
		{
//			printf("TKPE_UnpackL4: Hit CSE\n");
			break;
		}

		ll=(tg&15)+4;
		
//		ld=tkfat_getWord(cs);
		ld=*(u16 *)cs;
		cs+=2;
		if(!ld)
		{
			if(ll==5)
				continue;
			printf("TKPE_UnpackL4: End Of Image\n");
			tkpe_imgend=1;
			break;
		}
//		ll=(tg&15)+4;
		if(ll==19)
		{
			i=*cs++;
			while(i==255)
				{ ll+=255; i=*cs++; }
			ll+=i;
		}
		
		cs1=ct-ld; cs1e=cs1+ll;
		if(ld>=8)
//		if(ld>8)
//		if(ld>4)
//		if(0)
		{
			ct1=ct;
			while(cs1<cs1e)
			{
				((u64 *)ct1)[0]=((u64 *)cs1)[0];
				((u64 *)ct1)[1]=((u64 *)cs1)[1];
				ct1+=16; cs1+=16;
			}
//				{ *(u64 *)ct1=*(u64 *)cs1; ct1+=8; cs1+=8; }
//				{ *(u32 *)ct1=*(u32 *)cs1; ct1+=4; cs1+=4; }
			ct+=ll;
//			__debugbreak();
		}else
//			if(ld>4)
			if(ld>=4)
//		if(0)
		{
			ct1=ct;
			while(cs1<cs1e)
			{
				((u32 *)ct1)[0]=((u32 *)cs1)[0];
				((u32 *)ct1)[1]=((u32 *)cs1)[1];
				ct1+=8; cs1+=8;
			}
			ct+=ll;
//			__debugbreak();
		}else if(ld==1)
		{
			tv=*cs1;		tv|=tv<<8;
			tv|=tv<<16;		tv|=tv<<32;
			ct1=ct;
			while(cs1<cs1e)
			{
				((u64 *)ct1)[0]=tv;
				((u64 *)ct1)[1]=tv;
				ct1+=16; cs1+=16;
			}
			ct+=ll;
		}else
		{
			while(cs1<cs1e)
				{ *ct++=*cs1++; }
		}
	}
	
	tg=0;	lr=0;
	ll=0;	ld=0;
	
	return(ct);
}

#ifndef __BJX2__
byte *TKPE_UnpackL6(byte *ct, byte *ibuf, int isz)
	{ return(TKPE_UnpackL4(ct, ibuf, isz)); }
#endif

#ifdef __BJX2__
byte *TKPE_UnpackL6(byte *ct, byte *ibuf, int isz);

__asm {
TKPE_UnpackL6:
//	cs=ibuf; cse=ibuf+isz;
	WEXMD	2
	ADD		R5, R6, R7
	.L0:

	/* Fetch Token Byte */
	MOVU.B	(R5, 0), R16
	MOVU.B	(R5, 1), R17
	ADD		1, R5
	SHLD	R16, -4, R18	|	AND		R16, 15, R19
	/* Fetch Extended Length */
	CMPEQ	15, R18
	ADD?T	R17, R18		|	ADD?T	1, R5
	ADD		R5, R18, R22
	MOV		R4,	R20			|	MOV		R5,	R21
	ADD		R18, R4			|	ADD		R18, R5
	/* Copy Literal Bytes */
	.L1:
	MOV.Q	(R21, 0), R16
	MOV.Q	(R21, 8), R17
	ADD		16, R21
	MOV.Q	R16, (R20, 0)
	MOV.Q	R17, (R20, 8)
	ADD		16, R20
	CMPGT	R21, R22
	BT		.L1

	MOVU.W	(R5, 0), R18
	MOVU.B	(R5, 2), R17
	ADD		R5, 1, R23
	ADD		2, R5

	/* Hit end of buffer yet? */
	CMPGT	R23, R7
	BF		.Done
	CMPEQ	15, R19
	ADD?T	1, R5		| ADD?T	R17, R19
	ADD		4, R19

	/* Zero-Distance Cases? */
	TEST	R18, R18
	BF		.Copy
	CMPEQ	5, R19
	BT		.Cont
	BRA		.Done

	.Cont:
	CMPGT	R5, R7
	BT		.L0

	.Done:
	MOV		R4, R2
	RTS

/* Match Copy Stuff */

	/* R4=Dest, R5=Source, R18=Dist, R19=Length */
	.Copy:
	ADD		R4, R19, R22	| SUB		R4, R18, R21
	MOV		R4,	R20
	ADD		R19, R4
	CMPGE	8, R18
	BF		.CopyRLE
	CMPGE	16, R18
	BF		.CopyL1S

#if 1
	.CopyL1:
	MOV.Q	(R21, 0), R16
	MOV.Q	(R21, 8), R17
	ADD		16, R21
	MOV.Q	R16, (R20, 0)
	MOV.Q	R17, (R20, 8)
	ADD		16, R20
	CMPGT	R20, R22
	BT		.CopyL1
	BRA		.Cont
#endif

#if 1
	.CopyL1S:
	MOV.Q	(R21, 0), R16
	MOV.Q	R16, (R20, 0)
	MOV.Q	(R21, 8), R17
	MOV.Q	R17, (R20, 8)
	ADD		16, R21
	ADD		16, R20
	CMPGT	R20, R22
	BT		.CopyL1S
	BRA		.Cont
#endif

#if 0
	.CopyL1:
	MOV.B	(R21), R16
	ADD		1, R21
	MOV.B	R16, (R20)
	ADD		1, R20
	CMPGT	R20, R22
	BT		.CopyL1
	BRA		.Cont
#endif
	
	.CopyRLE:

#if 1
	CMPEQ	1, R18
	BT		.CopyRLE1
	CMPEQ	2, R18
	BT		.CopyRLE2
	CMPEQ	4, R18
	BT		.CopyRLE4
//	CMPEQ	3, R18
//	BT		.CopyRLE3
#endif

#if 1
	MOV.Q		(R21), R16
	.CopyRLE_L1:
	MOV.Q	R16, (R20, 0)
	ADD		R18, R20
	CMPGT	R20, R22
	BT		.CopyRLE_L1
	BRA		.Cont
#endif

#if 0
	.CopyRLE_L1:
	MOV.B	(R21), R16
	ADD		1, R21
	MOV.B	R16, (R20, 0)
	ADD		1, R20
	CMPGT	R20, R22
	BT		.CopyRLE_L1
	BRA		.Cont
#endif
	
	.CopyRLE1:
	MOVU.B		(R21), R16
	PSHUF.B		R16, 0x00, R16
	PSHUF.W		R16, 0x00, R16
	BRA			.CopyRLEN_L1
	.CopyRLE2:
	MOVU.W		(R21), R16
	PSHUF.W		R16, 0x00, R16
	BRA			.CopyRLEN_L1
	.CopyRLE4:
	MOVU.L		(R21), R16
	MOVLLD		R16, R16, R16
	BRA			.CopyRLEN_L1

	.CopyRLE3:
	MOVU.L		(R21), R16
	MOV			6, R18
//	MOV			0x00FFFFFF, R0
//	AND			R0, R16
	SHLD		R16, 8, R3		|	SHLD		R16, 24, R17
	SHLD		R3, -8, R16
	OR			R17, R16
	BRA			.CopyRLE_L1

	NOP

	.CopyRLEN_L1:
	MOV.Q	R16, (R20, 0)
	MOV.Q	R16, (R20, 8)
	ADD		16, R20
	CMPGT	R20, R22
	BT		.CopyRLEN_L1
	BRA		.Cont
};

#endif

void W_MatchCopy2(byte *dst, int sz, int d)
{
	byte *cs, *ct, *cte;
	u64 v;
	
	if(d<8)
	{
		if(d==1)
		{
			v=*(dst-d);
			v=v|(v<<8);
			v=v|(v<<16);
			v=v|(v<<32);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(u64 *)ct=v;
				ct+=8;
			}
		}else
			if(d==2)
		{
			v=*(u16 *)(dst-d);
			v=v|(v<<16);
			v=v|(v<<32);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(u64 *)ct=v;
				ct+=8;
			}
		}else
			if(d==4)
		{
			v=*(u32 *)(dst-d);
			v=v|(v<<32);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(u64 *)ct=v;
				ct+=8;
			}
		}else
		{
			v=*(u64 *)(dst-d);
			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(u64 *)ct=v;
				ct+=d;
			}
		}
	}else
		if(sz<=16)
	{
		cs=dst-d;
		((u64 *)dst)[0]=((u64 *)cs)[0];
		((u64 *)dst)[1]=((u64 *)cs)[1];
	}else
	{
		cs=dst-d;
		ct=dst; cte=dst+sz;
		while(ct<cte)
		{
			((u64 *)ct)[0]=((u64 *)cs)[0];
			((u64 *)ct)[1]=((u64 *)cs)[1];
			ct+=16; cs+=16;
		}
	}
}

void W_RawCopyB(byte *dst, byte *src, int sz)
{
	byte *cs, *ct, *cte;

	cs=src;
	ct=dst; cte=dst+sz;
	while(ct<cte)
	{
		((u64 *)ct)[0]=((u64 *)cs)[0];
		((u64 *)ct)[1]=((u64 *)cs)[1];
		ct+=16; cs+=16;
	}
}

#ifndef __BJX2__
int TKPE_DecodeBufferRP2(
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	u32 tag;
	byte *cs, *ct, *cse;
	int pl, pd;
	int rl, l, d;
	u64 t0;
	int t1, t2;
	
	cs=ibuf; cse=ibuf+ibsz;
	ct=obuf;
	pl=0; pd=0;
	
//	while(1)
	while(cs<cse)
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
			W_RawCopyB(ct, cs, rl);
			cs+=rl;
			ct+=rl;
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
			W_RawCopyB(ct, cs, rl);
			cs+=rl;
			ct+=rl;
			continue;
		}else
		{
			__debugbreak();
		}

		*(u64 *)ct=*(u64 *)cs;
		cs+=rl;
		ct+=rl;
		W_MatchCopy2(ct, l, d);
		ct+=l;
	}
	
	return(ct-obuf);
}
#endif

#ifdef __BJX2__
// #if 0
int TKPE_DecodeBufferRP2(
	byte *ibuf, byte *obuf, int ibsz, int obsz);

__asm {
TKPE_DecodeBufferRP2:
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

	WEXMD	2

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

u64 TK_GetRandom();

#ifdef __BJX2__
__asm {
TK_GetRandom:
	CPUID	31
	NOP
	NOP
	MOV		DLR, R2
	RTS
};
#endif

byte *TKPE_UnpackBuffer(byte *ct, byte *ibuf, int isz, int cmp)
{
	byte *ct1;
	int rsz;

#ifdef __BJX2__
	if(cmp==6)
	{
		return(TKPE_UnpackL6(ct, ibuf, isz));
	}
#endif

//	if(cmp==4)
	if((cmp==4) || (cmp==6))
	{
		return(TKPE_UnpackL4(ct, ibuf, isz));
	}

	if(cmp==3)
	{
		rsz=TKPE_DecodeBufferRP2(ibuf, ct, isz, 999999);
		return(ct+rsz);
	}
}

#if 0
u32 TKPE_CalculateImagePel4Checksum(byte *buf, int size)
{
	byte *cs, *cse;
//	u64 acc;
	u32 acc_lo, acc_hi;
	u32 csum;
	
	cs=buf;
	cse=cs+size;
	acc_lo=1;
	acc_hi=0;
	while(cs<cse)
	{
		acc_lo=acc_lo+(*cs++);
		if(acc_lo>=65521)
			acc_lo-=65521;
		acc_hi=acc_hi+acc_lo;
		if(acc_hi>=65521)
			acc_hi-=65521;
	}
	csum=acc_lo|(acc_hi<<16);
//	csum+=size;
	return(csum);
}
#endif

#ifdef __BJX2__
// #if 0
u32 TKPE_CalculateImagePel4BChecksum(byte *buf, int size);

__asm {
TKPE_CalculateImagePel4BChecksum:
	WEXMD	2
	ADD		R4, R5, R6
	MOV		1, R16
	MOV		0, R17

#if 1
	.L0:
								MOVU.L	(R4,  0), R20
								MOVU.L	(R4,  4), R21
								MOVU.L	(R4,  8), R22
								MOVU.L	(R4, 12), R23
	ADD		R20, R16, R18	|	ADD		R4, 16, R4
	ADD		R18, R17, R19	|	ADD		R21, R18, R16
	ADD		R16, R19, R17	|	ADD		R22, R16, R18
	ADD		R18, R17, R19	|	ADD		R23, R18, R16
	ADD		R16, R19, R17	|	CMPGT	R4, R6
								BT		.L0
#endif

#if 0
	.L0:
	MOVU.L	(R4), R7
	ADD		R7, R16
	ADD		R16, R17
	ADD		4, R4
	CMPGT	R4, R6
	BT		.L0
#endif

	SHLD.Q	R16, -32, R18
	EXTU.L	R16, R19
	ADD		R18, R19, R16
	SHLD.Q	R16, -32, R18
	EXTU.L	R16, R19
	ADD		R18, R19, R16

	SHLD.Q	R17, -32, R18
	EXTU.L	R17, R19
	ADD		R18, R19, R17
	SHLD.Q	R17, -32, R18
	EXTU.L	R17, R19
	ADD		R18, R19, R17
	
	XOR		R16, R17, R2
	RTS
};
#else
u32 TKPE_CalculateImagePel4BChecksum(byte *buf, int size)
{
	byte *cs, *cse;
	u32 v;
	u64 acc_lo, acc_hi;
	u32 csum;
	
	cs=buf;
	cse=cs+size;
	acc_lo=1;
	acc_hi=0;
#if 0
	while(cs<cse)
	{
		v=*(u32 *)cs;
		acc_lo=acc_lo+v;
//		acc_lo=((u32)acc_lo)+(acc_lo>>32);

		acc_hi=acc_hi+acc_lo;
//		acc_hi=((u32)acc_hi)+(acc_hi>>32);
		cs+=4;
	}
#endif
#if 1
	while(cs<cse)
	{
		v0=((u32 *)cs)[0];	v1=((u32 *)cs)[1];
		v2=((u32 *)cs)[2];	v3=((u32 *)cs)[3];
//		v0=get_u32le(cs+0);	v1=get_u32le(cs+ 4);
//		v2=get_u32le(cs+8);	v3=get_u32le(cs+12);
		acc_lo=acc_lo+v0;	acc_hi=acc_hi+acc_lo;
		acc_lo=acc_lo+v1;	acc_hi=acc_hi+acc_lo;
		acc_lo=acc_lo+v2;	acc_hi=acc_hi+acc_lo;
		acc_lo=acc_lo+v3;	acc_hi=acc_hi+acc_lo;
		cs+=16;
	}
#endif
	acc_lo=((u32)acc_lo)+(acc_lo>>32);
	acc_lo=((u32)acc_lo)+(acc_lo>>32);
	acc_hi=((u32)acc_hi)+(acc_hi>>32);
	acc_hi=((u32)acc_hi)+(acc_hi>>32);
	csum=(u32)(acc_lo^acc_hi);
//	csum+=size;
	return(csum);
}
#endif

void __setmemtrap(void *ptr, int mode);

#if 1
int TKPE_ApplyStaticRelocs(byte *imgptr, byte *rlc, int szrlc,
	s64 disp, int pboix,
	s64 imgbase, int gbr_rva, int gbr_sz)
{
	byte *cs, *cse, *cs1, *cs1e;
	byte *pdst;
	s64 v0, v1;
	u32 pv, pv0, pv1;
	int tgt_rva, gbr_end_rva;
	int rva_page, sz_blk;
	int tg;

//	printf("TKPE_ApplyStaticRelocs: disp=%X rlc=%p sz=%d\n",
//		(int)disp, rlc, szrlc);

	gbr_end_rva=gbr_rva+gbr_sz;

	cs=rlc;		cse=rlc+szrlc;
	while(cs<cse)
	{
		rva_page=((u32 *)cs)[0];
		sz_blk=((u32 *)cs)[1];
		cs1=cs+8; cs1e=cs+sz_blk;
		cs+=sz_blk;

//		tk_printf("  RVA=%08X sz=%d\n", rva_page, sz_blk);
		
		while(cs1<cs1e)
		{
			tg=*(u16 *)cs1;
			cs1+=2;

			tgt_rva=rva_page+(tg&4095);
			pdst=imgptr+tgt_rva;
//			pdst=imgptr+rva_page+(tg&4095);
			
			if(rva_page==0x62000)
			{
//				tk_printf("    %08X: %d\n", tgt_rva, (tg>>12)&15);
			}

#if 0
			tgt_rva=pdst-imgptr;
			if((pboix>0) && (tgt_rva>=gbr_rva) && (tgt_rva<gbr_end_rva))
			{
				/* Pointer is within data area. */
				continue;
			}
#endif
			
			switch((tg>>12)&15)
			{
			case 0:
				break;
			case 1:
				*((u16 *)pdst)=(*((u16 *)pdst))+(disp>>16);
				break;
			case 2:
				*((u16 *)pdst)=(*((u16 *)pdst))+(disp&65535);
				break;
			case 3:
//				tgt_rva=(*((u32 *)pdst))-imgbase;				
				*((u32 *)pdst)=(*((u32 *)pdst))+disp;
				break;
			case 4:
				*((u32 *)pdst)=(*((u32 *)pdst))+(disp>>32);
				break;
			case 5:
				pv=*((u32 *)pdst);
				if((pv&0x0000FE00U)==0x0000FE00U)
				{
					pv0=((u32 *)pdst)[0];
					pv1=((u32 *)pdst)[1];
					v0=((pv0&0x00FF)<<16)|((pv0>>16)&0xFFFF);
					v1=((pv1&0x00FF)<<16)|((pv1>>16)&0xFFFF);
					v1+=disp;
					v0+=(disp>>24)+(v1>>24);
					pv0=(pv0&0x0000FF00U)|
						((v0>>16)&0x000000FFU)|
						((v0<<16)&0xFFFF0000U);
					pv1=(pv1&0x0000FF00U)|
						((v1>>16)&0x000000FFU)|
						((v1<<16)&0xFFFF0000U);
					((u32 *)pdst)[0]=pv0;
					((u32 *)pdst)[1]=pv1;
					break;
				}
			
//				pv=*((u32 *)pdst);
				v0=((pv&0x01FF)<<16)|((pv>>16)&0xFFFF);
				v1=v0+disp;
				pv=(pv&0x0000FE00U)|
					((v1>>16)&0x1FF)|
					((v1<<16)&0xFFFF0000U);
				
//				pv=(pv&0xFE000000)|((pv+disp)&0x01FFFFFF);
				*((u32 *)pdst)=pv;
				break;

			case 6:
				pv=*((u16 *)pdst);
				if((pv==0xA000) && pboix)
				{
					pv=pv|(((-pboix)*8)&0x1FFF);
					*((u16 *)pdst)=pv;
					break;
				}
				break;
				
			case 7:
				break;
			case 8:
				break;

			case 9:
				pv=*((u32 *)pdst);
				if((pv==0xFA000000UL) && pboix)
				{
					pv=pv|(((-pboix)*8)&0x01FFFFFF);
					*((u32 *)pdst)=pv;
					break;
				}
				break;
			case 10:
				*((s64 *)pdst)=(*((s64 *)pdst))+disp;
				break;
			case 11:
				__setmemtrap(pdst, 3);
				break;
			}
		}
	}
	return(0);
}
#endif

int TKPE_LoadStaticPE(TK_FILE *fd, void **rbootptr, void **rbootgbr)
{
//	byte tbuf[1024];
	byte tbuf[1024+32];
	byte *imgptr, *ct, *cte, *bss_ptr;
	u64 imgbase;
	s64 reloc_disp;
	u32 imgsz, startrva, gbr_rva, gbr_sz, imgsz1, imgsz2;
	u32 reloc_rva, reloc_sz, bss_sz;
	u32 csum1, csum2;
	byte is64, is_pel4, cmp;
	int sig_mz, sig_pe, mach, mmagic;
	int ofs_pe;
	int cb, nb, kb;
	int i, l;
	
	tk_fread(tbuf, 1, 1024, fd);

	is_pel4=0;
	cmp=4;

#if 1
	sig_mz=tkfat_getWord(tbuf);
	if(sig_mz!=0x4550)
	{
		if(	(tbuf[0]==0x7F) &&
			(tbuf[1]=='E') &&
			(tbuf[2]=='L') &&
			(tbuf[3]=='F'))
		{
			tk_fseek(fd, 0, 0);
			return(TKPE_LoadStaticELF(fd,
				rbootptr, rbootgbr));
		}
	
		if(sig_mz!=0x5A4D)
		{
			printf("TKPE: MZ Sig Fail\n");
			return(-1);
		}

		ofs_pe=tkfat_getDWord(tbuf+0x3C);
		sig_pe=tkfat_getWord(tbuf+ofs_pe);
		if(sig_pe!=0x4550)
		{
			printf("TKPE: PE Sig Fail\n");
			return(-1);
		}
	}else
	{
		mach=tkfat_getWord(tbuf+2);
		if(mach==0x364C)
			{ is_pel4=1; cmp=6; }
		if(mach==0x344C)
			{ is_pel4=1; cmp=4; }
		if(mach==0x334C)
			{ is_pel4=1; cmp=3; }
	
		ofs_pe=0;
		sig_pe=sig_mz;
	}

	mach=tkfat_getWord(tbuf+ofs_pe+0x04);
	if(mach!=0xB264)
	{
		printf("TKPE: Unexpected Arch %04X\n", mach);
		return(-1);
	}

	printf("TKPE: PE Sig OK\n");
	
	mmagic=tkfat_getWord(tbuf+ofs_pe+0x18);
	if(mmagic==0x020B)
	{
//		printf("TKPE: Magic 64 %04X\n", mmagic);
		is64=1;
	}else if(mmagic==0x010B)
	{
//		__debugbreak();
//		printf("TKPE: Magic 32 %04X\n", mmagic);
		is64=0;
	}else
	{
		printf("TKPE: Unexpected mMagic %04X\n", mach);
		return(-1);
	}
	
	if(is64)
	{
//		puts("TKPE: PE64\n");
		startrva=*(u32 *)(tbuf+ofs_pe+0x28);
		imgbase=*(u64 *)(tbuf+ofs_pe+0x30);
		imgsz=*(u32 *)(tbuf+ofs_pe+0x50);
		gbr_rva=*(u32 *)(tbuf+ofs_pe+0xC8);
		gbr_sz=*(u32 *)(tbuf+ofs_pe+0xCC);
		reloc_rva=*(u32 *)(tbuf+ofs_pe+0xB0);
		reloc_sz=*(u32 *)(tbuf+ofs_pe+0xB4);
	}else
	{
//		puts("TKPE: PE32\n");
		startrva=*(u32 *)(tbuf+ofs_pe+0x28);
		imgbase=*(u32 *)(tbuf+ofs_pe+0x34);
		imgsz=*(u32 *)(tbuf+ofs_pe+0x50);
		gbr_rva=*(u32 *)(tbuf+ofs_pe+0xB8);
		gbr_sz=*(u32 *)(tbuf+ofs_pe+0xBC);
		reloc_rva=*(u32 *)(tbuf+ofs_pe+0xA0);
		reloc_sz=*(u32 *)(tbuf+ofs_pe+0xA4);
	}

	printf("TKPE: Base=%08X Sz=%d BootRVA=%08X GbrRVA=%08X GbrSz=%08X\n",
		imgbase, imgsz, startrva, gbr_rva, gbr_sz);
	
	bss_ptr=NULL;
	bss_sz=0;

//	if(reloc_rva)
//	{
		imgsz1=gbr_rva+gbr_sz;
		imgsz2=imgsz;
		if(imgsz1>imgsz)
			imgsz2=imgsz1;
//	}

	imgptr=(byte *)imgbase;
	reloc_disp=0;

#if 0
	i=TK_GetRandom();
	i=(i*64)&65535;
	imgptr+=i;
	reloc_disp+=i;
#endif

	bss_ptr=imgptr+imgsz;
	bss_sz=imgsz2-imgsz;
	
	printf("TKPE: Bss_Ptr=%08X Bss_Sz=%d\n", bss_ptr, bss_sz);

	if(!is_pel4)
	{
		tk_fseek(fd, 0, 0);
	//	tk_fread(imgptr, 1, imgsz, fd);

#if 1
		cb=0; nb=imgsz>>10;
		ct=imgptr; cte=imgptr+imgsz;
		while((ct+1024)<=cte)
		{
			printf("%d/%dkB\r", cb, nb);
			tk_fread(ct, 1, 1024, fd);
			ct+=1024; cb++;
		}
		printf("%d/%dkB\r", cb, nb);
		tk_fread(ct, 1, cte-ct, fd);
		printf("\n", cb, nb);
#endif
	}else
	{
		memcpy(imgptr, tbuf, 1024);
		tkpe_imgend=0;

#if 1
		cb=0; nb=imgsz>>10;
		ct=imgptr+1024; cte=imgptr+imgsz;
		l=1024;
//		while((ct+1024)<=cte)
		while(ct<cte)
		{
			kb=(ct-imgptr)>>10;
//			printf("%d/%dkB\r", cb, nb);
			printf("%d/%dkB\r", kb, nb);
//			tk_fread(ct, 1, 1024, fd);
//			ct+=1024;
			memset(tbuf, 0, 1024);
			l=tk_fread(tbuf, 1, 1024, fd);
//			ct=TKPE_UnpackL4(ct, tbuf, 1024);
//			ct=TKPE_UnpackL4(ct, tbuf, l);
			ct=TKPE_UnpackBuffer(ct, tbuf, l, cmp);
			if(tkpe_imgend)
				break;
			if(l<1024)
				break;
//			ct+=1024;
			cb++;
		}

#if 0
		kb=(ct-imgptr)>>10;
//		printf("%d/%dkB\r", cb, nb);
		printf("%d/%dkB\r", kb, nb);
		if(l>=1024)
		{
//			tk_fread(ct, 1, cte-ct, fd);
			l=tk_fread(tbuf, 1, 1024, fd);
//			ct=TKPE_UnpackL4(ct, tbuf, l);
			ct=TKPE_UnpackBuffer(ct, tbuf, l);
		}
#endif

		kb=(ct-imgptr)>>10;
		printf("%d/%dkB\r", kb, nb);

		printf("\n");
#endif
	}
	
	if(is_pel4)
	{
		csum1=*(u32 *)(imgptr+ofs_pe+0x58);
		
		if(csum1)
		{
			if(ct!=cte)
			{
				printf("Image Size Mismatch %08X -> %08X\n",
					cte-imgptr, ct-imgptr);
				__debugbreak();
			}
		
			*(u16 *)(imgptr+ofs_pe+0x02)=0;		
			*(u32 *)(imgptr+ofs_pe+0x58)=0;
			
	//		csum2=TKPE_CalculateImagePel4Checksum(imgptr, imgsz);
			csum2=TKPE_CalculateImagePel4BChecksum(imgptr, imgsz);
			if(csum1!=csum2)
			{
				printf("Image Checksum Mismatch %08X -> %08X\n", csum1, csum2);
				__debugbreak();
			}
		}
	}
	
	if(bss_ptr && (bss_sz>0))
	{
		memset(bss_ptr, 0, bss_sz);
	}

//	if(reloc_disp)
	if(1)
	{
//		tk_printf("Reloc: RVA=%08X, sz=%d\n", reloc_rva, reloc_sz);
		TKPE_ApplyStaticRelocs(imgptr, imgptr+reloc_rva, reloc_sz,
			reloc_disp, 0, imgbase, gbr_rva, gbr_sz);
	}

	*rbootptr=imgptr+startrva;
	*rbootgbr=imgptr+gbr_rva;
#endif
}
