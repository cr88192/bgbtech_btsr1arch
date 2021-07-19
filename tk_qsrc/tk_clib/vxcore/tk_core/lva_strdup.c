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

int TKMM_LVA_HashNameU16(u16 *str)
{
	u16 *s;
	int h;
	
	s=str; h=1;
	while(*s)
		{ h=(h*65521)+(*s++); }
	return(h*65521+1);
//	return((h*65521)>>16);
}

int TKMM_LVA_StrcmpU16(u16 *stra, u16 *strb)
{
	u16 *csa, *csb;
	int ca, cb;
	
	csa=stra;
	csb=strb;
	
	ca=*csa++;
	cb=*csb++;
	while(ca && cb)
	{
		if(ca!=cb)
			break;
		ca=*csa++;
		cb=*csb++;
	}
	return(ca-cb);
}

void TKMM_LVA_StrcpyU16(u16 *strd, u16 *strs)
{
	u16 *ct, *cs;
	int ca;

	cs=strs; ct=strd;
	ca=*cs++;
	while(ca)
		{ *ct++=ca; ca=*cs++; }
	*ct++=ca;
}

void TKMM_LVA_StrEncodeLengthRev(byte *dst, int len, int tag)
{
	if(len<0)
		return;
//	if(len<128)
	if(len<64)
	{
		dst[0]=tag;
		dst[1]=0x80|len;
		return;
	}

	if(len<4096)
	{
		dst--;
		dst[0]=tag;
		dst[1]=0x80|((len>>0)&0x3F);
		dst[2]=0xC0|((len>>6)&0x1F);

//		(dst-1)[0]=0xC0|((len>>6)&0x1F);
//		(dst-1)[1]=0x80|((len>>0)&0x3F);
		return;
	}

	if(len<65536)
	{
		dst-=2;
		dst[0]=tag;
		dst[1]=0x80|((len>> 0)&0x3F);
		dst[2]=0x80|((len>> 6)&0x3F);
		dst[3]=0xE0|((len>>12)&0x0F);

//		(dst-2)[0]=0xE0|((len>>12)&0x0F);
//		(dst-2)[1]=0x80|((len>> 6)&0x3F);
//		(dst-2)[2]=0x80|((len>> 0)&0x3F);
		return;
	}
	if(len<2097152)
	{
		dst-=3;
		dst[0]=tag;
		dst[1]=0x80|((len>> 0)&0x3F);
		dst[2]=0x80|((len>> 6)&0x3F);
		dst[3]=0x80|((len>>12)&0x3F);
		dst[4]=0xF0|((len>>18)&0x07);

//		(dst-3)[0]=0xF0|((len>>18)&0x07);
//		(dst-3)[1]=0x80|((len>>12)&0x3F);
//		(dst-3)[2]=0x80|((len>> 6)&0x3F);
//		(dst-3)[3]=0x80|((len>> 0)&0x3F);
		return;
	}
	if(len<67108864)
	{
		dst-=4;
		dst[0]=tag;
		dst[1]=0x80|((len>> 0)&0x3F);
		dst[2]=0x80|((len>> 6)&0x3F);
		dst[3]=0x80|((len>>12)&0x3F);
		dst[4]=0x80|((len>>18)&0x3F);
		dst[5]=0xF8|((len>>24)&0x03);

//		(dst-4)[0]=0xF8|((len>>24)&0x03);
//		(dst-4)[1]=0x80|((len>>18)&0x3F);
//		(dst-4)[2]=0x80|((len>>12)&0x3F);
//		(dst-4)[3]=0x80|((len>> 6)&0x3F);
//		(dst-4)[4]=0x80|((len>> 0)&0x3F);
		return;
	}
	if(len<2147483647)
	{
		dst-=5;
		dst[0]=tag;
		dst[1]=0x80|((len>> 0)&0x3F);
		dst[2]=0x80|((len>> 6)&0x3F);
		dst[3]=0x80|((len>>12)&0x3F);
		dst[4]=0x80|((len>>18)&0x3F);
		dst[5]=0x80|((len>>24)&0x3F);
		dst[6]=0xFC|((len>>30)&0x01);

//		(dst-5)[0]=0xFC|((len>>30)&0x01);
//		(dst-5)[1]=0x80|((len>>24)&0x3F);
//		(dst-5)[2]=0x80|((len>>18)&0x3F);
//		(dst-5)[3]=0x80|((len>>12)&0x3F);
//		(dst-5)[4]=0x80|((len>> 6)&0x3F);
//		(dst-5)[5]=0x80|((len>> 0)&0x3F);
		return;
	}
}

char *TKMM_LVA_StrdupPfx(char *str, byte pfx)
{
	char *c, *s1;
	int h, l;
	
	h=TKMM_LVA_HashName(str);
	h=(h>>16)&255;
	
	c=tk_lva_strbuf_hash[h];
	while(c)
	{
		s1=(char *)(((char **)c)+2);
		if((*(s1-1)==pfx) && !strcmp(s1, str))
			return(s1);
		c=*(char **)c;
	}

	l=strlen(str);

	if(l>512)
	{
		c=TKMM_MMList_Malloc(l+24);
		s1=(char *)(((char **)c)+2);
		strcpy(s1, str);
//		*(s1-1)=pfx;
//		*(u16 *)(s1-4)=l;
		TKMM_LVA_StrEncodeLengthRev((byte *)(s1-2), l, pfx);
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
	
	if((tk_lva_strbuf_pos+l+24)>=tk_lva_strbuf_end)
	{
		tk_lva_strbuf_buf=TKMM_MMList_Malloc(4096);
		tk_lva_strbuf_end=tk_lva_strbuf_buf+4096;
		tk_lva_strbuf_pos=tk_lva_strbuf_buf;
	}

	c=tk_lva_strbuf_pos;
	tk_lva_strbuf_pos=c+l+24;
	s1=(char *)(((char **)c)+2);
	strcpy(s1, str);
//	*(s1-1)=pfx;
//	*(u16 *)(s1-4)=l;
	TKMM_LVA_StrEncodeLengthRev((byte *)(s1-2), l, pfx);
	*(char **)c=tk_lva_strbuf_hash[h];
	tk_lva_strbuf_hash[h]=c;
	return(s1);
}

char *TKMM_LVA_Strdup(char *str)
{
	return(TKMM_LVA_StrdupPfx(str, 'c'));
}

u16 *TKMM_LVA_StrdupU16(u16 *str)
{
	char *c, *s1;
	int h, l;
	
	h=TKMM_LVA_HashNameU16(str);
	h=(h>>16)&255;
	
	c=tk_lva_strbuf_hash[h];
	while(c)
	{
		s1=(char *)(((char **)c)+2);
		if(!TKMM_LVA_StrcmpU16((u16 *)s1, str))
			return((u16 *)s1);
		c=*(char **)c;
	}

	l=strlen(str);

	if(l>512)
	{
		c=TKMM_MMList_Malloc((l*2)+24);
		s1=(char *)(((char **)c)+2);
//		strcpy(s1, str);
		TKMM_LVA_StrcpyU16((u16 *)s1, str);
//		*(s1-1)='w';
//		*(u16 *)(s1-4)=l;
		TKMM_LVA_StrEncodeLengthRev((byte *)(s1-2), l, 'w');
		*(char **)c=tk_lva_strbuf_hash[h];
		tk_lva_strbuf_hash[h]=c;
		return((u16 *)s1);
	}
	
	if(!tk_lva_strbuf_buf)
	{
		tk_lva_strbuf_buf=TKMM_MMList_Malloc(4096);
		tk_lva_strbuf_end=tk_lva_strbuf_buf+4096;
		tk_lva_strbuf_pos=tk_lva_strbuf_buf;
	}
	
	if((tk_lva_strbuf_pos+(l*2)+24)>=tk_lva_strbuf_end)
	{
		tk_lva_strbuf_buf=TKMM_MMList_Malloc(4096);
		tk_lva_strbuf_end=tk_lva_strbuf_buf+4096;
		tk_lva_strbuf_pos=tk_lva_strbuf_buf;
	}

	c=tk_lva_strbuf_pos;
	tk_lva_strbuf_pos=c+(l*2)+24;
	s1=(char *)(((char **)c)+2);
	TKMM_LVA_StrcpyU16((u16 *)s1, str);
//	*(s1-1)='w';
//	*(u16 *)(s1-4)=l;
	TKMM_LVA_StrEncodeLengthRev((byte *)(s1-2), l, 'w');
	*(char **)c=tk_lva_strbuf_hash[h];
	tk_lva_strbuf_hash[h]=c;
	return((u16 *)s1);
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

u64 TKMM_LVA_WrapStringU16(u16 *str)
{
	u16 *s0;
	u64 v;	

	if(!tkmm_lvatyi_string)
	{
		TKMM_LVA_ArrayInit();
	}

	s0=TKMM_LVA_StrdupU16(str);
	v=(u64)s0;
	v|=((u64)tkmm_lvatyi_string)<<48;
	
	return(v);
}

int TKMM_LVA_StringP(u64 val)
{
	return((((int)(val>>48))&65535)==tkmm_lvatyi_string);

//	int tg;
//	if(!TKMM_LVA_ObjRefP(val))
//		return(0);
//	tg=TKMM_LVA_GetObjTag(val);
//	if(tg!=tkmm_lvatyi_string)
//		return(0);
//	return(1);
}
