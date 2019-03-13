char *tk_lva_strbuf_buf;
char *tk_lva_strbuf_end;
char *tk_lva_strbuf_pos;

char *tk_lva_strbuf_hash[256];

int TKMM_LVA_HashName(char *str)
{
	char *s;
	int h;
	
	s=str; h=1;
	while(*s)
		{ h=(h*65521)+(*s++); }
	return(h*65521+1);
//	return((h*65521)>>16);
}

char *TKMM_LVA_Strdup(char *str)
{
	char *c, *s1;
	int h, l;
	
	h=TKMM_LVA_HashName(str);
	h=(h>>16)&255;
	
	c=tk_lva_strbuf_hash[h];
	while(c)
	{
		s1=(char *)(((char **)c)+1);
		if(!strcmp(s1, str))
			return(s1);
		c=*(char **)c;
	}

	l=strlen(str);

	if(l>512)
	{
		c=TKMM_MMList_Malloc(l+9);
		s1=(char *)(((char **)c)+1);
		strcpy(s1, str);
		*(char **)c=tk_lva_strbuf_hash[h];
		tk_lva_strbuf_hash[h]=c;
		return(s1);
	}
	
	if(!tk_lva_strbuf_buf)
	{
		tk_lva_strbuf_buf=TKMM_MMList_Malloc(4096);
		tk_lva_strbuf_end=tk_lva_strbuf_buf+4096;
		tk_lva_strbuf_pos=tk_lva_strbuf_buf;
	}
	
	if((tk_lva_strbuf_pos+l+9)>=tk_lva_strbuf_end)
	{
		tk_lva_strbuf_buf=TKMM_MMList_Malloc(4096);
		tk_lva_strbuf_end=tk_lva_strbuf_buf+4096;
		tk_lva_strbuf_pos=tk_lva_strbuf_buf;
	}

	c=tk_lva_strbuf_pos;
	tk_lva_strbuf_pos=c+l+9;
	s1=(char *)(((char **)c)+1);
	strcpy(s1, str);
	*(char **)c=tk_lva_strbuf_hash[h];
	tk_lva_strbuf_hash[h]=c;
	return(s1);
}

u64 TKMM_LVA_WrapString(char *str)
{
	char *s0;
	u64 v;	

	if(!tkmm_lvatyi_string)
	{
		TKMM_LVA_ArrayInit();
	}

	s0=TKMM_LVA_Strdup(str);
	v=(u64)s0;
	v|=((u64)tkmm_lvatyi_string)<<48;
	
	return(v);
}
