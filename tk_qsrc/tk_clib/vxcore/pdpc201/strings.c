int bcmp(const void *s1, const void *s2, size_t n)
{
	return(memcmp(s1, s2, n));
}

void bcopy(const void *s1, void *s2, size_t n)
{
	memmove(s2, s1, n);
}

void bzero(void *s, size_t n)
{
	memset(s, 0, n);
}

int ffs(int i)
{
	int c, v;

	if(!i)
		return(0);

	c=1; v=i;
	if(!(v&0xFFFF))
		{ v>>=16; c+=16; }
	if(!(v&0xFF))
		{ v>>=8; c+=8; }
	if(!(v&0xF))
		{ v>>=4; c+=4; }
	if(!(v&0x3))
		{ v>>=2; c+=2; }
	if(!(v&0x1))
		{ v>>=1; c+=1; }
	return(c);
}

char *index(const char *s, int c)
{
	return(strchr(s, c));
}

char *rindex(const char *s, int c)
{
	return(strrchr(s, c));
}

int strcasecmp(const char *s1, const char *s2)
{
	return(_stricmp(s1, s2));
}

int strncasecmp(const char *s1, const char *s2, size_t n)
{
	return(_strnicmp(s1, s2, n));
}
