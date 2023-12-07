// #ifdef __BGBCC__
#if 1

typedef struct
{
    u32 mFlags;                       //00, Flags
    u32 mDateTime;                    //04, Data/Time Compiled
    u32 fccMagic;                     //08, 'RWAD'
    u32 mMagicZero;                   //0C, Magic (Set to 0)
    u32 rvaDir;                       //10, RVA of Directory
    u32 szDir;                        //14, Size of Directory (Bytes)
    u32 rvaTypes;                     //18, RVA of Types
    u32 szTypes;                      //1C, Size of Types
    u32 rvaHead;                      //20, RVA of Header (Self-Reference)
    u32 szHead;                       //24, Size of Header (Self-Refernece)
} rwadhead_t;

typedef struct
{
    u32  rvaData;                      //RVA of Lump Data
    u32  cszData;                      //Compressed Size of Lump Data
    u32  dszData;                      //Uncompressed Size of Lump Data
    byte mType;                        //Lump Type
    byte mMethod;                      //Lump Compression Method
    u16  mChain;                       //Lump Lookup Chain.
    byte mName[16];                    //Name of Lump
} rwadlump_t;

#ifdef __BGBCC__
extern void __rsrc_start;
extern void __rsrc_dir;
extern void __rsrc_types;
#endif

rwadhead_t *tk_rwad_head;
rwadlump_t *tk_rwad_dir;
u32 *tk_rwad_types;
u16 *tk_rwad_hash;

int TKPE_DecodeBufferRP2(
	byte *ibuf, byte *obuf, int ibsz, int obsz);
byte *TKPE_UnpackL4(byte *ct, byte *ibuf, int isz);


void TK_RWad_Init()
{
	if(tk_rwad_head)
		return;

#ifdef __BGBCC__
	tk_rwad_head=(&__rsrc_start);
	tk_rwad_dir=(&__rsrc_dir);
	tk_rwad_types=(&__rsrc_types);
	tk_rwad_hash=(u16 *)(((byte *)(&__rsrc_dir))+(tk_rwad_head->szDir));
#endif
}

void TK_RWad_FillName(char *tname, char *name)
{
	char *s, *t, *te;
	int i;

	((u64 *)tname)[0]=0;
	((u64 *)tname)[1]=0;	
	s=name; t=tname; te=t+16;
	while(*s && (t<te))
	{
		i=*s++;
		if(((byte)(i-'A'))<=('Z'-'A'))
			i+='a'-'A';
		*t++=i;
	}
}

int TK_RWad_HashName(char *name)
{
	u32 *pn;
	int v, h;
	pn=(u32 *)name;
	v=pn[0]+pn[1]+pn[2]+pn[3];
	h=((v*15813251)>>24)&63;
	return(h);
}

int TK_RWad_GetLumpNumForName(char *name)
{
	char tn[17];
	rwadlump_t *de;
	char *s;
	int h;
	int i, j, k;

	TK_RWad_Init();

	if(!tk_rwad_head)
		return(-1);

	TK_RWad_FillName(tn, name);
	h=TK_RWad_HashName(tn);
	
	i=tk_rwad_hash[h];
	while(i!=0xFFFF)
	{
		de=tk_rwad_dir+i;
		s=de->mName;
		if(	((((u64 *)tn)[0])==(((u64 *)s)[0]))	&&
			((((u64 *)tn)[1])==(((u64 *)s)[1]))	)
				return(i);
		i=de->mChain;
	}
	return(-1);
}

int TK_RWad_GetLumpSize(int lump)
{
	rwadlump_t *de;
	de=tk_rwad_dir+lump;
	return(de->dszData);
}

int TK_RWad_ReadLump(int lump, void *dest)
{
	rwadlump_t *de;
	byte *ibuf;
	byte *cbuf;
	
	de=tk_rwad_dir+lump;
	ibuf=((byte *)tk_rwad_head)-(tk_rwad_head->rvaHead);
	cbuf=ibuf+de->rvaData;

	if(de->mMethod==0)
	{
		memcpy(dest, cbuf, de->dszData);
		return(0);
	}

	if(de->mMethod==3)
	{
		TKPE_DecodeBufferRP2(cbuf, dest, de->cszData, de->dszData);
		return(0);
	}

	if(de->mMethod==4)
	{
		TKPE_UnpackL4(dest, cbuf, de->cszData);
		return(0);
	}
	
	return(-1);
}

#endif

byte *TKPE_UnpackL4(byte *ct, byte *ibuf, int isz)
{
	byte *cs, *cse;
	register byte *cs1, *cs1e, *ct1;
//	byte *cs1, *cs1e, *ct1;
	register int tg, lr, ll, ld;
//	int tg, lr, ll, ld;
	int i;
	
	__hint_use_egpr();
	
	tg=0; lr=0; ll=0; ld=0;
	
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

#if 0		
		ct1=ct; cs1=cs; cs1e=cs+lr;
		while(cs1<cs1e)
		{
			*(u64 *)ct1=*(u64 *)cs1;
			ct1+=8; cs1+=8;
		}
#endif

		_memlzcpyf(ct, cs, lr);
		ct+=lr; cs+=lr;
		
		if((cs+1)>=cse)
			break;

		ll=(tg&15)+4;
		
//		ld=tkfat_getWord(cs);
		ld=*(u16 *)cs;
		cs+=2;
		if(!ld)
		{
			if(ll==5)
				continue;
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

		ct=_memlzcpyf(ct, ct-ld, ll);
//		ct=_memlzcpy(ct, ct-ld, ll);

#if 0
		cs1=ct-ld; cs1e=cs1+ll;
//		if(ld>=8)
		if(ld>8)
		{
			ct1=ct;
			while(cs1<cs1e)
			{
				((u64 *)ct1)[0]=((u64 *)cs1)[0];
				((u64 *)ct1)[1]=((u64 *)cs1)[1];
				ct1+=16; cs1+=16;
			}
//				{ *(u64 *)ct1=*(u64 *)cs1; ct1+=8; cs1+=8; }
			ct+=ll;
		}else
			if(ld>4)
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
		}else
		{
			while(cs1<cs1e)
				{ *ct++=*cs1++; }
		}
#endif
	}
	
	return(ct);
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
//			W_RawCopyB(ct, cs, rl);
			_memlzcpy(ct, cs, rl);
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
//			W_RawCopyB(ct, cs, rl);
			_memlzcpy(ct, cs, rl);
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
//		W_MatchCopy2(ct, l, d);
		_memlzcpy(ct, ct-d, l);
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
	CMPQGT	R19, R7
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
	TEST	R22, R22
	BREAK?T

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
	CMPQGT	R19, R7
	BT		.L12
	BRA		.L10
.L11:
	MOV.Q	(R6, 0), R16
	MOV.Q	(R6, 8), R17
	ADD		16, R6
	MOV.Q	R16, (R19, 0)
	MOV.Q	R17, (R19, 8)
	ADD		16, R19
	CMPQGT	R19, R7
	BT		.L11
.L10:
	MOV		R7, R19
	BRA		.L0
.L1:
	SUB		R19, R5, R2
	RTS
};
#endif

byte *TKPE_UnpackBuffer(byte *ct, byte *ibuf, int isz, int cmp)
{
	byte *ct1;
	int rsz;

// #ifdef __BJX2__
// #if 1
#if 0
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
	ADD		R16, R19, R17	|	CMPQGT	R4, R6
								BT		.L0
#endif

#if 0
	.L0:
	MOVU.L	(R4), R7
	ADD		R7, R16
	ADD		R16, R17
	ADD		4, R4
	CMPQGT	R4, R6
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
	u32 v, v0, v1, v2, v3;
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
