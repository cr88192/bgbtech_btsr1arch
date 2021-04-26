static const char *itoa_radix="0123456789abcdefghijklmnopqrstuvwxyz";

char *lltoa(long long value, char *str, int base)
{
	char tb[128];
	char *s, *t;
	unsigned long long uv;
	int sg, v, v1;

	if(base==10)
	{
		v=value; sg=0;
		if(v<0)
			{ v=-v; sg=1; }
		s=tb;
		while(v || (s==tb))
		{
			v1=v/10;
			*s++=itoa_radix[v-(v1*10)];
			v=v1;
		}
		if(sg)*s++='-';
		
		t=str;
		while(s!=tb)
			*t++=*(--s);
		*t++=0;
		return(str);
	}

	uv=value;
	s=tb;
	while(uv || (s==tb))
	{
		v1=uv/base;
		*s++=itoa_radix[v-(v1*base)];
		v=v1;
	}
	
	t=str;
	while(s!=tb)
		*t++=*(--s);
	*t++=0;
	return(str);
}

char *itoa(int value, char *str, int base)
{
	return(lltoa(value, str, base));
}

char *ltoa(long value, char *str, int base)
{
	return(lltoa(value, str, base));
}

char *ultoa(unsigned long value, char *str, int base)
{
	return(lltoa(value, str, base));
}

char *strupr (char *str)
{
	char *s;
	s=str;
	while (*s) { *s = toupper(*s); s++; }
	return(str);
}

int unlink(const char *path)
{
	return(tk_unlink(path));
}

char *getcwd(char *buf, size_t size)
{
	return(TK_Env_GetCwd(buf, size));
}

#if 1
void MemLzCpy_MatchCopy2(uint8_t *dst, long sz, long d)
{
	uint8_t *cs, *ct, *cte;
	uint64_t v;
	
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
				*(uint64_t *)ct=v;
				ct+=8;
			}
		}else
			if(d==2)
		{
			v=*(uint16_t *)(dst-d);
			v=v|(v<<16);
			v=v|(v<<32);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(uint64_t *)ct=v;
				ct+=8;
			}
		}else
			if(d==4)
		{
			v=*(uint32_t *)(dst-d);
			v=v|(v<<32);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(uint64_t *)ct=v;
				ct+=8;
			}
		}else
		{
			v=*(uint64_t *)(dst-d);
			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(uint64_t *)ct=v;
				ct+=d;
			}
		}
	}else
		if(sz<=16)
	{
		cs=dst-d;
		((uint64_t *)dst)[0]=((uint64_t *)cs)[0];
		((uint64_t *)dst)[1]=((uint64_t *)cs)[1];
	}else
		if(sz<=32)
	{
		cs=dst-d;
		((uint64_t *)dst)[0]=((uint64_t *)cs)[0];
		((uint64_t *)dst)[1]=((uint64_t *)cs)[1];
		((uint64_t *)dst)[2]=((uint64_t *)cs)[2];
		((uint64_t *)dst)[3]=((uint64_t *)cs)[3];
	}else
	{
		cs=dst-d;
		ct=dst; cte=dst+sz;
		while(ct<cte)
		{
			((uint64_t *)ct)[0]=((uint64_t *)cs)[0];
			((uint64_t *)ct)[1]=((uint64_t *)cs)[1];
			ct+=16; cs+=16;
		}
	}
}
#endif

#if 1
void *_memlzcpy(void *dst, void *src, size_t n)
{
	long d;
	
	d=((char *)dst)-((char *)src);
	if(d<=0)
	{
		/* Copying backwards, use normal copy. */
		if((-d)>=n)
			{ memcpy(dst, src, n); }
		else if(d!=0)
			{ memmove(dst, src, n); }
	}else if(d>=n)
	{
		/* No overlap, use memcpy. */
		memcpy(dst, src, n);
	}else
	{
		MemLzCpy_MatchCopy2(dst, n, d);
	}
	return(dst);
}
#endif
