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
