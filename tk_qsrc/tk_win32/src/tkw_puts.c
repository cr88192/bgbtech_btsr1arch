#include <stdio.h>
#include <stdarg.h>

#include <tk_core.h>

TK_APIEXPORT void tk_puts(char *str)
{
	fputs(str, stdout);
}

TK_APIEXPORT int tk_printf(char *str, ...)
{
	char tb[1024];
	va_list lst;
	va_start(str, lst);
	vsnprintf(tb, 1022, str, lst);
	tk_puts(tb);
	va_end(lst);
	return(0);
}

TK_APIEXPORT int tk_iskernel()
{
	return(0);
}


TK_APIEXPORT void *tk_malloc(size_t sz)
{
	return(malloc(sz));
}

TK_APIEXPORT void *tk_malloc_krn(size_t sz)
{
	return(malloc(sz));
}

TK_APIEXPORT void *tk_malloc_usr(size_t sz)
{
	return(malloc(sz));
}

TK_APIEXPORT void tk_free(void *ptr)
{
	free(ptr);
}

TK_APIEXPORT void *tk_realloc(void *ptr, size_t sz)
{
	return(realloc(ptr, sz));
}

TK_APIEXPORT char *tk_rstrdup(char *str)
{
}

s64 tk_atoi(char *str)
{
}


TK_APIEXPORT int tk_strdup_in(char *str)
{
}

s64 TK_DateToEpochUsec(TK_DATETIME *rdate)
{
	s64 te;
	/* Lazy / Approximation */
	te =(rdate->year-1970)*	31536000LL;
	te+=rdate->mon*			 2628000LL;
	te+=rdate->day*			   86400LL;
	te+=rdate->hour*		    3600LL;
	te+=rdate->min*			      60LL;
	te+=rdate->sec*			       1LL;
	te=te*1000000LL;
	return(te);
}

void *_memlzcpy(void *dst, void *src, int sz)
{
	byte *cs, *ct, *cte;
	
	cs=src;
	ct=dst; cte=ct+sz;
	while(ct<cte)
		*ct++=*cs++;
	return(ct);
}

void *_memlzcpyf(void *dst, void *src, int sz)
{
	return(_memlzcpy(dst, src, sz));
}

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
	}
	
	return(ct);
}

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
			_memlzcpyf(ct, cs, rl);
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
			_memlzcpyf(ct, cs, rl);
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
		_memlzcpyf(ct, ct-d, l);
		ct+=l;
	}
	
	return(ct-obuf);
}

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

int TKSPI_InitDevice()
{
}

int TKSPI_ReadSectors(void *tbd, s64 lba, int n)
{
}

int TKSPI_WriteSectors(void *tbd, s64 lba, int n)
{
}

int tk_syscall(void *obj, u32 msg, void *vp1, void *vp2)
{
}

void *TK_DlGetApiContextA(u64 apiname, u64 subname)
{
	if(apiname==TK_FCC_GDI)
	{
		return(TKW_TkGdiGetApiContext(apiname, subname));
	}
	return(NULL);
}
