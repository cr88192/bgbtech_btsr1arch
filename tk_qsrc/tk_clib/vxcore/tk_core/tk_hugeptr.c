#ifdef __BJX2__

tk_hugeptr TK_MakeHugePtrFromPtr(void *ptr);
tk_hugeptr TK_MakeHugePtr(u64 lo, u64 hi);
__huge void *TK_CvtHugePtr2Ptr(tk_hugeptr ptr);

s32 TK_HugePtrIndexS32(tk_hugeptr ptr, int idx);
u32 TK_HugePtrIndexU32(tk_hugeptr ptr, int idx);
s64 TK_HugePtrIndexS64(tk_hugeptr ptr, int idx);
u64 TK_HugePtrIndexU64(tk_hugeptr ptr, int idx);

tk_hugeptr TK_HugePtrIndexPtr(tk_hugeptr ptr, int idx);

__asm {
TK_MakeHugePtr:
TK_CvtHugePtr2Ptr:
	MOV		A0, AR0
	MOV		A1, AR1
	RTS

TK_MakeHugePtrFromPtr:
	MOV		A0, AR0
	MOV		GBH, AR1
	RTS

TK_HugePtrIndexS32:
	XMOV.L	(A0, A2), AR0
	RTS
TK_HugePtrIndexU32:
	XMOVU.L	(A0, A2), AR0
	RTS

TK_HugePtrIndexS64:
TK_HugePtrIndexU64:
	XMOV.Q	(A0, A2), AR0
	RTS

TK_HugePtrIndexPtr:
	XMOV.Q	(A0, A2), AR0
	MOV		A1, AR1
	RTS
}

void TK_MemcpyHuge(tk_hugeptr dst, tk_hugeptr src, size_t len)
{
	__huge u64 *ct;
	__huge u64 *cs;
	u64 v0, v1, v2, v3;
	s64 rem;
	
	ct=TK_CvtHugePtr2Ptr(dst);
	cs=TK_CvtHugePtr2Ptr(src);
	rem=len;

	while(rem>=32)
	{
		v0=cs[0];	v1=cs[1];
		v2=cs[2];	v3=cs[3];
		ct[0]=v0;	ct[1]=v1;
		ct[2]=v2;	ct[3]=v3;
		rem-=32;
		cs+=4;
		ct+=4;
	}

	while(rem>=8)
	{
		v0=cs[0];
		ct[0]=v0;
		rem-=8;		cs++;	ct++;
	}
}

#else

tk_hugeptr TK_MakeHugePtr(u64 lo, u64 hi)
{
	tk_hugeptr t;
//	t.lo=lo;
//	t.hi=hi;
	return(t);
}

tk_hugeptr TK_MakeHugePtrFromPtr(void *ptr)
{
	return(TK_MakeHugePtr((long)ptr, 0));
}

void TK_MemcpyHuge(tk_hugeptr dst, tk_hugeptr src, size_t len)
{
}

#endif