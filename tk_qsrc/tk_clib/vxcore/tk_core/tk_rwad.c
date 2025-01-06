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
	
//	__hint_use_egpr();
	
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

u16 TKPE_CalculateSmallByteCsum(byte *buf, int sz)
{
	byte *cs, *cse;
	u32 ac0, ac1, csum;
	
	if(!sz)
		return(0);
	
	cs=buf; cse=buf+sz;
	ac0=1; ac1=0;
	while(cs<cse)
	{
		ac0+=*cs;
		ac1+=ac0;
		cs++;
	}
	ac0=((u16)ac0)+(ac0>>16);
	ac1=((u16)ac1)+(ac1>>16);
	ac0=((u16)ac0)+(ac0>>16);
	ac1=((u16)ac1)+(ac1>>16);
	csum=(u16)(ac0^ac1);
	return(csum);
}

u32 TKPE_CalculateImagePel4BChecksumAc(byte *buf, int sz)
{
	u32 csum0, csum1;

	if(!(sz&15))
	{
		csum0=TKPE_CalculateImagePel4BChecksum(buf, sz);
		return(csum0);
	}

	csum0=TKPE_CalculateImagePel4BChecksum(buf, sz&(~15));
	csum1=TKPE_CalculateSmallByteCsum(buf+(sz&(~15)), sz&15);
	return(csum0^csum1);
}


int TKPE_MiniPackBlockRP2(byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	byte *hash[256];
	byte *cs, *cse, *lcs;
	byte *ct, *cte;
	byte *cs1a, *cs1b;
	u32 v;
	int pl, pd;
	int l, d, rl, l1, d1, h;
	int i, j, k;

	for(i=0; i<256; i++)
		hash[i]=ibuf;
	
	cs=ibuf; cse=ibuf+ibsz;
	ct=obuf; cte=obuf+obsz;
	
	pd=0; pl=0;
	
	lcs=cs;
	while((cs<cse) && (ct<cte))
	{
		h=*(u32 *)cs;
		h=(h^(h>>7)^(h>>17))&255;
		
		cs1a=cs;
		cs1b=hash[h];
		d=cs-cs1b;
		
		k=cse-cs;
		if(k>514)
			k=514;
		for(l=0; l<k; l++)
			if(cs1a[j]!=cs1b[j])
				break;
		if(l<4)
			{ l=0; d=0; }
		
		if(!i)
		{
			hash[h]=cs;
			cs++;
			continue;
		}
			
		rl=cs-lcs;

#if 1
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
#endif

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
//			BGBCC_DBGBREAK
		}
		
		pl=l; pd=d;

		for(j=0; j<l; j++)
		{
			h=*(u32 *)cs;
			h=(h^(h>>7)^(h>>17))&255;
			hash[h]=cs;
			cs++;
		}

		lcs=cs;
	}

	rl=cse-lcs;

#if 1
	while(rl>(128+3))
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
#endif

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


void *TK_DlOpenA(const char *path, int flags)
{
	TK_SysArg ar[4];
	void *p;
	int tid;
	
#ifndef __TK_CLIB_ONLY__
	if(tk_iskernel())
	{
		p=TK_DlOpenB((TKPE_TaskInfo *)TK_GET_TBR, path, flags);
		return(p);
	}
#endif

	p=0;
	ar[0].p=(void *)path;
	ar[1].i=flags;
	tk_syscall(NULL, TK_UMSG_DLOPEN, &p, ar);
	return(p);
}

void *TK_DlSymA(void *handle, const char *symbol, int flags)
{
	TK_SysArg ar[4];
	void *p;
	int tid;
	
#ifndef __TK_CLIB_ONLY__
	if(tk_iskernel())
	{
		p=TK_DlSymB((TKPE_TaskInfo *)TK_GET_TBR, handle, symbol, flags);
		return(p);
	}
#endif

	p=0;
	ar[0].p=handle;
	ar[1].p=(void *)symbol;
	ar[2].i=flags;
	tk_syscall(NULL, TK_UMSG_DLSYM, &p, ar);

	tk_printf("TK_DlSymA: DlSym name=%s ptr=%p flags=%d\n", symbol, p, flags);

	return(p);
}



int tkdfs_readutf8(byte **rcs)
{
	byte *cs;
	int c0, c1, c2, c3, v;

	cs=*rcs;
	c0=*cs++;
	if(c0<0x80)
	{
		v=c0;
	}else if(c0<0xC0)
	{
		/* broken */
	}else if(c0<0xE0)
	{
		c1=*cs++;
		v=(((c0)&31)<<6)|(c1&63);
	}else if(c0<0xF0)
	{
		c1=*cs++;
		c2=*cs++;
		v=(((c0)&15)<<12)|((c1&63)<<6)|(c2&63);
	}else if(c0<0xF8)
	{
		c1=*cs++;	c2=*cs++;	c3=*cs++;
		v=(((c0)&7)<<18)|((c1&63)<<12)|((c2&63)<<6)|(c3&63);
	}else
	{
	}
	*rcs=cs;
	return(v);
}

int tkdfs_writeutf8(byte **rct, int val)
{
	byte *ct;
	int c0, c1, c2, c3, v;

	ct=*rct;
	if(val<0x80)
	{
		*ct++=val;
	}else if(val<0x0800)
	{
		*ct++=0xC0|((val>> 6)&31);
		*ct++=0x80|((val>> 0)&63);
	}else if(val<0x10000)
	{
		*ct++=0xE0|((val>>12)&15);
		*ct++=0x80|((val>> 6)&63);
		*ct++=0x80|((val>> 0)&63);
	}else if(val<0x100000)
	{
		*ct++=0xF0|((val>>18)& 7);
		*ct++=0x80|((val>>12)&63);
		*ct++=0x80|((val>> 6)&63);
		*ct++=0x80|((val>> 0)&63);
	}
	*rct=ct;
	return(0);
}

int tkdfs_writeutf8uz(byte **rct, int val)
{
	byte *ct;

	ct=*rct;
	if((val<0x20) && (!val || (val>=0x10)))
	{
		*ct++=0xC0|((val>> 6)&31);
		*ct++=0x80|((val>> 0)&63);
	}else
		if(val<0x80)
	{
		*ct++=val;
	}else
	{
		tkdfs_writeutf8(&ct, val);
	}
	*rct=ct;
	return(0);
}

/*
UTF-8 Compaction (Possible)
* 80..BF (when not in a codepoint): Encodes a relative byte offset:
** UTF-8 Codepoint at target: Repeat the codepoint.
** ASCII byte at target: Repeat byte plus following byte/codepoint.
* 10..1F: Repeat 2..17 bytes.
** Followed by a byte (80..BF) giving the offset.
*/

int tkdfs_trystringcompact(byte *dst, byte *src)
{
	short cph[64];
	byte *cs, *ct, *cs1a, *cs1b;
	int i, j, k, h, v, v0, v1, d;
	
	for(i=0; i<64; i++)
		cph[i]=0;
	
	cs=src;
	ct=dst;
	while(*cs)
	{
		v=*(u16 *)cs;
		h=(v^(v>>7))&63;
		
		cs1a=src+cph[h];
		cs1b=cs;
		d=cs-cs1a;
		for(j=0; j<7; j++)
		{
			if(!(*cs1b))
				break;
			v0=tkdfs_readutf8(&cs1a);
			v1=tkdfs_readutf8(&cs1b);
			if(v0!=v1)
				break;
			k=cs1b-cs;
		}
		if(k>15)
			k=15;
		
		cph[h]=cs-src;
		
		if((k>=2) && (d<64))
		{
			if((j==1) || ((j==2) && (k==2)))
			{
				*ct++=0x80+d;
			}else
//				if((j>=3) || (k>j))
				if(k>=3)
			{
				cs+=k;
				*ct++=0x10+(k-2);
				*ct++=0x80+d;
			}
		}else
		{
			v=tkdfs_readutf8(&cs);
			tkdfs_writeutf8uz(&ct, v);
		}
	}
	*ct++=0;
	return(0);
}

int tkdfs_stringexpand(byte *dst, byte *src)
{
	byte *cs, *ct, *cs1a, *cs1b;
	int i, j, k, h, v, v0, v1, d;
	
	cs=src;
	ct=dst;
	while(*cs)
	{
		v=*cs;
		if((v>=0x20) && (v<=0x7F))
		{
			*ct++=*cs++;
			continue;
		}
		if((v>=0x80) && (v<=0xBF))
		{
			cs++;
			cs1a=ct-(v&63);
			if((*cs1a)&0x80)
			{
				v=tkdfs_readutf8(&cs1a);
				tkdfs_writeutf8(&ct, v);
			}else
			{
				*ct++=*cs1a++;
				*ct++=*cs1a++;
			}
			continue;
		}
		if((v>=0x10) && (v<=0x1F))
		{
			cs++;
			v1=*cs;
			if((v1&0xC0)==0x80)
			{
				cs++;
				cs1a=ct-(v1&63);
				k=2+(v&15);
				while(k--)
					{ *ct++=*cs1a++; }
				continue;
			}
		}
		v=tkdfs_readutf8(&cs1a);
		tkdfs_writeutf8(&ct, v);
	}
	*ct++=0;
	return(0);
}

int TKDFS_CopyName48IsUtf8(byte *dst, byte *src)
{
	u64 v0, v1, v2, vt;
	
	v0=((u64 *)src)[0];
	v1=((u64 *)src)[1];
	v2=((u64 *)src)[2];
	((u64 *)dst)[0]=v0;
	((u64 *)dst)[1]=v1;
	((u64 *)dst)[2]=v2;
	vt=v0|v1|v2;
	v0=((u64 *)src)[3];
	v1=((u64 *)src)[4];
	v2=((u64 *)src)[5];
	((u64 *)dst)[3]=v0;
	((u64 *)dst)[5]=v1;
	((u64 *)dst)[6]=v2;
	vt=vt|v0|v1|v2;
	
	if(vt&0x8080808080808080ULL)
		return(1);
	return(0);
}

int TKDFS_CheckNameValidUtf8(byte *src, int nln)
{
	byte *cs, *cse;
	u64 v0, vm;
	int v;

	vm=0x8080808080808080ULL;
	cs=src; cse=src+nln;
	while(*cs)
	{
		if((cs+8)<=cse)
		{
			v0=*(u64 *)cs;
			if(!(v0&vm))
			{
				cs+=8;
				continue;
			}
		}
	
		v=*cs;
		if(*cs&0x80)
			{ v=tkdfs_readutf8(&cs); }
		else
			cs++;
		if(v<=0)
			break;
	}
	if(*cs)
		return(0);
	return(1);
}


struct tkdfs_utf_comb_s {
u16 v0;
u16 v1;
u16 vc;
u16 fl;
}tkdfs_utf_combarr[] = {
//grave accent
{'A', 0x0300, 0x00C0},	{'E', 0x0300, 0x00C8},
{'I', 0x0300, 0x00CC},	{'O', 0x0300, 0x00D3},
{'U', 0x0300, 0x00D9},	{'a', 0x0300, 0x00E0},
{'e', 0x0300, 0x00E8},	{'i', 0x0300, 0x00EC},
{'o', 0x0300, 0x00F3},	{'u', 0x0300, 0x00F9},

//acute
{'A', 0x0301, 0x00C1},	{'E', 0x0301, 0x00C9},
{'I', 0x0301, 0x00CD},	{'O', 0x0301, 0x00D4},
{'U', 0x0301, 0x00DA},	{'Y', 0x0301, 0x00DC},
{'a', 0x0301, 0x00E1},	{'e', 0x0301, 0x00E9},
{'i', 0x0301, 0x00ED},	{'o', 0x0301, 0x00F4},
{'u', 0x0301, 0x00FA},	{'y', 0x0301, 0x00FC},
{'C', 0x0301, 0x0106},	{'c', 0x0301, 0x0107},
{'L', 0x0301, 0x0139},	{'l', 0x0301, 0x013A},
{'N', 0x0301, 0x0143},	{'n', 0x0301, 0x0144},
{'R', 0x0301, 0x0154},	{'r', 0x0301, 0x0155},
{'S', 0x0301, 0x015A},	{'s', 0x0301, 0x015B},
{'Z', 0x0301, 0x0179},	{'z', 0x0301, 0x017A},

//circumflex
{'A', 0x0302, 0x00C2},	{'E', 0x0302, 0x00CA},
{'I', 0x0302, 0x00CE},	{'O', 0x0302, 0x00D4},
{'U', 0x0302, 0x00DB},	{'a', 0x0302, 0x00E2},
{'e', 0x0302, 0x00EA},	{'i', 0x0302, 0x00EE},
{'o', 0x0302, 0x00F4},	{'u', 0x0302, 0x00FB},

{'C', 0x0302, 0x0108},	{'c', 0x0302, 0x0109},
{'G', 0x0302, 0x011C},	{'g', 0x0302, 0x011D},
{'H', 0x0302, 0x0124},	{'h', 0x0302, 0x0125},
{'J', 0x0302, 0x0134},	{'j', 0x0302, 0x0135},
{'S', 0x0302, 0x015C},	{'s', 0x0302, 0x015D},
{'W', 0x0302, 0x0174},	{'w', 0x0302, 0x0175},
{'Y', 0x0302, 0x0176},	{'y', 0x0302, 0x0177},


//tilde
{'A', 0x0303, 0x00C3},	{'N', 0x0303, 0x00D1},
{'O', 0x0303, 0x00D5},	{'a', 0x0303, 0x00E3},
{'n', 0x0303, 0x00F1},	{'o', 0x0303, 0x00F5},

{'I', 0x0303, 0x0128},	{'i', 0x0303, 0x0129},
{'U', 0x0303, 0x0168},	{'u', 0x0303, 0x0169},

//macron
{'A', 0x0304, 0x0100},	{'a', 0x0304, 0x0101},
{'E', 0x0304, 0x0112},	{'e', 0x0304, 0x0113},
{'I', 0x0304, 0x012A},	{'i', 0x0304, 0x012B},
{'O', 0x0304, 0x014C},	{'o', 0x0304, 0x014D},
{'U', 0x0304, 0x016A},	{'u', 0x0304, 0x016B},


//breve
{'A', 0x0306, 0x0102},	{'a', 0x0306, 0x0103},
{'E', 0x0306, 0x0114},	{'e', 0x0306, 0x0115},
{'G', 0x0306, 0x011E},	{'g', 0x0306, 0x011F},
{'I', 0x0306, 0x012C},	{'i', 0x0306, 0x012D},
{'O', 0x0306, 0x014E},	{'o', 0x0306, 0x014F},
{'U', 0x0306, 0x016C},	{'u', 0x0306, 0x016D},

//dot above
{'C', 0x0307, 0x010A},	{'c', 0x0307, 0x010B},
{'E', 0x0307, 0x0116},	{'e', 0x0307, 0x0117},
{'G', 0x0307, 0x0120},	{'g', 0x0307, 0x0121},
{'I', 0x0307, 0x0130},

{'Z', 0x0307, 0x017B},	{'z', 0x0307, 0x017C},


//umlaut
{'A', 0x0308, 0x00C5},	{'E', 0x0308, 0x00CC},
{'I', 0x0308, 0x00CF},	{'O', 0x0308, 0x00D6},
{'U', 0x0308, 0x00DC},	{'Y', 0x0308, 0x009F, 1},
{'a', 0x0308, 0x00E5},	{'e', 0x0308, 0x00EC},
{'i', 0x0308, 0x00EF},	{'o', 0x0308, 0x00F6},
{'u', 0x0308, 0x00FC},	{'y', 0x0308, 0x00FF},

{'Y', 0x0308, 0x0178},	//alt

//ring above
{'U', 0x030A, 0x016E},	{'u', 0x030A, 0x016F},

//double acute
{'O', 0x030B, 0x0150},	{'o', 0x030B, 0x0151},
{'U', 0x030B, 0x0170},	{'u', 0x030B, 0x0171},

//caron
{'S', 0x030C, 0x008A, 1},	{'Z', 0x030C, 0x008E, 1},
{'s', 0x030C, 0x009A, 1},	{'z', 0x030C, 0x009E, 1},

{'C', 0x030C, 0x010C},	{'c', 0x030C, 0x010D},
{'D', 0x030C, 0x010E},	{'d', 0x030C, 0x010F},
{'E', 0x030C, 0x011A},	{'e', 0x030C, 0x011B},

{'S', 0x030C, 0x0160},	{'Z', 0x030C, 0x017D},
{'s', 0x030C, 0x0161},	{'z', 0x030C, 0x017E},

//cedilla
{'G', 0x0327, 0x0122},	{'g', 0x0327, 0x0123},
{'K', 0x0327, 0x0136},	{'k', 0x0327, 0x0137},
{'L', 0x0327, 0x013B},	{'l', 0x0327, 0x013C},
{'N', 0x0327, 0x0145},	{'n', 0x0327, 0x0146},
{'R', 0x0327, 0x0156},	{'r', 0x0327, 0x0157},
{'S', 0x0327, 0x015E},	{'s', 0x0327, 0x015F},
{'T', 0x0327, 0x0162},	{'t', 0x0327, 0x0163},

//ogonek
{'A', 0x0328, 0x0104},	{'a', 0x0328, 0x0105},
{'E', 0x0328, 0x0118},	{'e', 0x0328, 0x0119},
{'I', 0x0328, 0x012E},	{'i', 0x0328, 0x012F},
{'U', 0x0328, 0x0172},	{'u', 0x0328, 0x0173},

//bar / stroke
{'D', 0x0335, 0x0110},	{'d', 0x0335, 0x0111},
{'H', 0x0335, 0x0126},	{'h', 0x0335, 0x0127},
{'L', 0x0335, 0x0141},	{'l', 0x0335, 0x0142},
{'T', 0x0335, 0x0166},	{'t', 0x0335, 0x0167},

{'B', 0x0335, 0x0180},	{'b', 0x0335, 0x0181},
{'Z', 0x0335, 0x01B5},	{'z', 0x0335, 0x01B6},
{'G', 0x0335, 0x01E4},	{'g', 0x0335, 0x01E5},

{'J', 0x0335, 0x0248},	{'j', 0x0335, 0x0249},
{'R', 0x0335, 0x024C},	{'R', 0x0335, 0x024D},
{'y', 0x0335, 0x024E},	{'y', 0x0335, 0x024F},

// diagonal
{'C', 0x0337, 0x023B},	{'c', 0x0337, 0x023C},
{'I', 0x0337, 0x0197},	{'A', 0x0337, 0x023A},

//decompose only
{'I', 'J', 0x0132, 3},	{'i', 'j', 0x0133, 3},	{'\'', 'n', 0x0149, 1},
{'L', 'J', 0x01C7, 3},	{'L', 'j', 0x01C8, 3},	{'l', 'j', 0x01C9, 3},
{'N', 'J', 0x01CA, 3},	{'N', 'j', 0x01CB, 3},	{'n', 'j', 0x01CC, 3},
{'D', 'Z', 0x01F1, 3},	{'D', 'z', 0x01F2, 3},	{'d', 'z', 0x01F3, 3},

{'I'   , 0  , 0x2160, 3},	{'i'   , 0  , 0x2170, 3},
{'V'   , 0  , 0x2164, 3},	{'v'   , 0  , 0x2174, 3},
{'X'   , 0  , 0x2169, 3},	{'x'   , 0  , 0x2179, 3},
{'L'   , 0  , 0x216C, 3},	{'l'   , 0  , 0x217C, 3},
{'C'   , 0  , 0x216D, 3},	{'c'   , 0  , 0x217D, 3},
{'D'   , 0  , 0x216E, 3},	{'d'   , 0  , 0x217E, 3},
{'M'   , 0  , 0x216F, 3},	{'m'   , 0  , 0x217F, 3},

{0x2160, 0x2160, 0x2161, 0},	{0x2170, 0x2170, 0x2171, 0},
{0x2161, 0x2160, 0x2162, 0},	{0x2171, 0x2170, 0x2172, 0},
{0x2160, 0x2164, 0x2163, 0},	{0x2170, 0x2174, 0x2173, 0},
{0x2164, 0x2160, 0x2165, 0},	{0x2174, 0x2170, 0x2175, 0},
{0x2165, 0x2160, 0x2166, 0},	{0x2175, 0x2170, 0x2176, 0},
{0x2166, 0x2160, 0x2167, 0},	{0x2176, 0x2170, 0x2177, 0},
{0x2160, 0x2169, 0x2168, 0},	{0x2170, 0x2179, 0x2178, 0},
{0x2169, 0x2160, 0x216A, 0},	{0x2179, 0x2170, 0x217A, 0},
{0x216A, 0x2160, 0x216B, 0},	{0x217A, 0x2170, 0x217B, 0},
{0x216D, 0x216E, 0x2180, 0},


{0, 0, 0}
};

s16 *tkdfs_utf_combchn;
s16 *tkdfs_utf_splitchn;

s16 tkdfs_utf_combhash[256];
s16 tkdfs_utf_splithash[256];

int TKDFS_UtfMergeInit()
{
	int v0, v1, vc;
	int i, j, k, h, n;
	
	for(i=0; i<256; i++)
	{
		tkdfs_utf_combhash[i]=-1;
		tkdfs_utf_splithash[i]=-1;
	}

	n=0;
	for(i=0; tkdfs_utf_combarr[i].vc; i++) { }
	n=i;

	tkdfs_utf_combchn=malloc(2*(n+1)*sizeof(short));
	tkdfs_utf_splitchn=tkdfs_utf_combchn+(n+1);

//	for(i=0; tkdfs_utf_combhash[i].vc; i++)
	for(i=n-1; i>=0; i--)
	{
		v0=tkdfs_utf_combarr[i].v0;
		v1=tkdfs_utf_combarr[i].v1;
		vc=tkdfs_utf_combarr[i].vc;

		h=(v0^v1);
		h=h^(h>>7);
		h=h&255;

		j=tkdfs_utf_combhash[h];
		tkdfs_utf_combchn[i]=j;
		tkdfs_utf_combhash[h]=i;

		h=(vc^(vc>>7))&255;
		j=tkdfs_utf_splithash[h];
		tkdfs_utf_splitchn[i]=j;
		tkdfs_utf_splithash[h]=i;
	}
	
	return(0);
}

int TKDFS_UtfCheckSplit(int vi, int *rv0, int *rv1)
{
	int v0, v1, vc, vf, vo0, vo1;
	int i, j, k, h;

	vo0=-1; vo1=-1;
	h=(vi^(vi>>7))&255;
	i=tkdfs_utf_splithash[h];
	while(i>=0)
	{
		v0=tkdfs_utf_combarr[i].v0;
		v1=tkdfs_utf_combarr[i].v1;
		vc=tkdfs_utf_combarr[i].vc;
//		vf=tkdfs_utf_combarr[i].fl;

		if(vc==vi)
		{
			vo0=v0;
			vo1=v1;
			break;
		}

		i=tkdfs_utf_splitchn[i];
	}
	
	if((vo0>0) && (vo1>0))
	{
		*rv0=vo0;
		*rv1=vo1;
		return(1);
	}
	
	*rv0=vi;
	*rv1=-1;
	return(0);
}

int TKDFS_UtfCheckMerge(int v0, int v1, int *rvo)
{
	int av0, av1, avf;
	int vo, i, h;
	
	vo=-1;

	h=(v0^v1);
	h=h^(h>>7);
	h=h&255;

	i=tkdfs_utf_combhash[h];
	while(i>=0)
	{
		av0=tkdfs_utf_combarr[i].v0;
		av1=tkdfs_utf_combarr[i].v1;
		avf=tkdfs_utf_combarr[i].fl;
		if((av0==v0) && (av1==v1) && !(avf&1))
		{
			vo=tkdfs_utf_combarr[i].vc;
			break;
		}
		i=tkdfs_utf_combchn[i];
	}
	
	if(vo>0)
	{
		*rvo=vo;
		return(1);
	}
	
	*rvo=v0;
	return(0);
}

int TKDFS_NormalizePathUtf8I(byte *dst, byte *src, int cfl)
{
	u16 tca[512], tcb[512];
	u16 tcpb[8];
	byte *cs, *ct;
	u16 *csa, *cta, *csae;
	int v, v0, v1, len, len1, pbs;
	
	TKDFS_UtfMergeInit();
	
	cs=src;
	cta=tca;
	while(*cs)
	{
		v=tkdfs_readutf8(&cs);
		if(v<=0xFFFF)
		{
			*cta++=v;
			continue;
		}
		
		v0=0xD800+((v>>10)&1023);
		v1=0xDC00+((v>> 0)&1023);
		*cta++=v0;
		*cta++=v1;
	}
	*cta=0;
	len=cta-tca;

	csa=tca; csae=tca+len;
	cta=tcb; pbs=0;
	while(csa<csae)
	{
		v=*csa++;
		while(TKDFS_UtfCheckSplit(v, &v0, &v1, cfl)>0)
		{
			if(v1)
				tcpb[pbs++]=v1;
			v=v0;
		}
		*cta++=v;
		while(pbs)
			{ pbs--; *cta++=tcpb[pbs]; }
	}
	len1=cta-tcb;

	csa=tcb; csae=tcb+len1;
	cta=tca;
	while(csa<csae)
	{
		v0=*csa++;
		v1=*csa;
		while(TKDFS_UtfCheckMerge(v0, v1, &v, cfl)>0)
			{ csa++; v0=v; v1=*csa; }
		*cta++=v0;
	}
	len=cta-tca;

	csa=tca; csae=tca+len;
	ct=dst;
	while(csa<csae)
	{
		v0=csa[0];
		v1=csa[1];
		
		if(!(cfl&1))
		{
			if(((v0&0xFC00)==0xD800) && ((v1&0xFC00)==0xDC00))
			{
				v=((v0&1023)<<10)|(v1&1023);
				csa+=2;
				tkdfs_writeutf8uz(&ct, v);
				continue;
			}
		}
		
		csa++;
		tkdfs_writeutf8uz(&ct, v0);
	}
	*ct++=0;
	return(0);
}

int TKDFS_NormalizePathUtf8(byte *dst, byte *src, int cfl)
{
	byte *cs;
	int i, l;
	
	cs=src;
	i=*cs;
	while(i && !(i&0x80)) { i=*cs++; }
	l=cs-src;
	
	if(!i)
	{
		memcpy(dst, src, l+1);
		return(0);
	}
	
	TKDFS_NormalizePathUtf8I(dst, src, cfl);
	return(1);
}

char *TKDFS_TempNormalizeStringUtf8(char *src, int cfl)
{
	byte tbuf[512];
	char *tstr1;
	int i;
	
	i=TKDFS_NormalizePathUtf8(tbuf, (byte *)src, cfl);
	
	if(i>0)
	{
		if(!strcmp((char *)tbuf, src))
			return(src);
	
		tstr1=tk_rstrdup((char *)tbuf);
		return(tstr1);
	}
	
	return(src);
}
