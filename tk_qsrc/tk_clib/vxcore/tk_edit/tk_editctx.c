void *tkedit_malloc(int size)
{
	return(malloc(sz));
}

void *tkedit_realloc(void *ptr, int size)
{
	return(realloc(ptr, sz));
}

void tkedit_free(void *ptr)
{
	free(ptr);
}

void *tkedit_loadfile(char *name, int *rsz)
{
	FILE *fd;
	int sz;
	void *buf;

	fd=fopen(name, "rb");
	if(!fd)return(NULL);
	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	buf=tkedit_malloc(sz+16);
	fread(buf, 1, sz, fd);
	fclose(fd);
	
	*rsz=sz;
	return(buf);
}

void tkedit_storefile(char *name, void *buf, int sz)
{
	FILE *fd;

	fd=fopen(name, "rb");
	if(!fd)return(NULL);
	fwrite(buf, 1, sz, fd);
	fclose(fd);
}

TKEdit_EditContext *TKEdit_AllocContext(void)
{
	TKEdit_EditContext *tmp;
	tmp=tkedit_malloc(sizeof(TKEdit_EditContext));
	memset(tmp, 0, sizeof(TKEdit_EditContext));
	return(tmp);
}

int TKEdit_CountBufferLinesA(tkedit_byte *buf, int sz)
{
	tkedit_byte *cs, *cse;
	int i, n;
	
	cs=buf; cse=buf+sz; n=0;
	while(cs<cse)
	{
		i=*cs++;
		if(i=='\n')n++;
	}
	return(i);
}

int TKEdit_CountBufferLinesU(tkedit_char *buf, int sz)
{
	tkedit_char *cs, *cse;
	int i, n;
	
	cs=buf; cse=buf+sz; n=0;
	while(cs<cse)
	{
		i=*cs++;
		if(i=='\n')n++;
	}
	return(i);
}

tkedit_byte *TKEdit_BufferNextLineA(
	tkedit_byte *ptr, tkedit_byte *cse, int *lsz)
{
	tkedit_byte *cs;
	int i, l;

	cs=ptr;
	while(cs<cse)
	{
		i=*cs;
		if(i=='\r')break;
		if(i=='\n')break;
		cs++;
	}
	l=cs-ptr;

	i=*cs;
	if(i=='\r')
		{ cs++; i=*cs; }
	if(i=='\n')
		{ cs++; i=*cs; }
	
	*lsz=l;
	return(cs);
}

tkedit_char *TKEdit_BufferNextLineU(
	tkedit_char *ptr, tkedit_char *cse, int *lsz)
{
	tkedit_char *cs;
	int i, l;

	cs=ptr;
	while(cs<cse)
	{
		i=*cs;
		if(i=='\r')break;
		if(i=='\n')break;
		cs++;
	}
	l=cs-ptr;

	i=*cs;
	if(i=='\r')
		{ cs++; i=*cs; }
	if(i=='\n')
		{ cs++; i=*cs; }
	
	*lsz=l;
	return(cs);
}

int TKEdit_BufferGetEncId(tkedit_byte *buf, int sz)
{
	tkedit_byte *cs, *cse;
	int i, m, n, u8sfx;

	if((buf[0]==0xFF) && (buf[1]==0xFE))
	{
		return(TKEDIT_LENC_UTF16L);
	}

	if((buf[0]==0xFE) && (buf[1]==0xFF))
	{
		return(TKEDIT_LENC_UTF16B);
	}

	if((buf[0]==0xEF) && (buf[1]==0xBB) && (buf[2]==0xBF))
	{
		return(TKEDIT_LENC_UTF8);
	}
	
	if((buf[0]!=0x00) && (buf[1]==0x00) && (buf[2]!=0x00) && (buf[3]==0x00))
	{
		return(TKEDIT_LENC_UTF16L);
	}
	
	if((buf[0]==0x00) && (buf[1]!=0x00) && (buf[2]==0x00) && (buf[3]!=0x00))
	{
		return(TKEDIT_LENC_UTF16B);
	}

	u8sfx=0;
	cs=buf; cse=buf+sz; n=0; m=0;
	while(cs<cse)
	{
		i=*cs++;

		if(u8sfx)
		{
			/* Check for valid UTF-8 sequence. */
			j=(i-0x80)&255;
			if(j<=0x3F)
			{
				u8sfx--;
				continue;
			}

			/* Invalid Escape Sequence, Not UTF8 */
			m|=2;
			u8sfx=0;
		}

		/* Skip ASCII printable characters. */
		if(((i-' ')&255)<=('~'-' '))
			continue;
		if(i<' ')
		{
			if(i=='\n')
				continue;
			if(i=='\r')
				continue;
			if(i=='\t')
				continue;

			/* Unusual control characters or binary. */
			m|=1;
			break;
		}
		
		j=(i-0x80)&255;
		if(j<=0x3F)
		{
			/* Freestanding 80..BF, Not UTF-8. */
			m|=2;
			break;
		}
		
		if((i&0xE0)==0xC0)
		{
			u8sfx=1;
			m|=4;
			continue;
		}

		if((i&0xF0)==0xE0)
		{
			u8sfx=2;
			m|=4;
			continue;
		}

		if((i&0xF8)==0xF0)
		{
			u8sfx=3;
			m|=4;
			continue;
		}

		/* Longer escapes not allowed. */
		m|=2;
		break;
	}
	
	if(!m)
		return(TKEDIT_LENC_ASCII);
	if(m&1)
		return(TKEDIT_LENC_UNK);
	if(m&2)
		return(TKEDIT_LENC_ASCIICP);
	if(m&4)
		return(TKEDIT_LENC_UTF8);
	
	return(TKEDIT_LENC_ASCII);
}

tkedit_byte *TKEdit_WriteCharUtf16(tkedit_byte *ct, int val, int enc)
{
	int val1;

	if(val>0x10000)
	{
		val1=(val-0x10000);
		ct=TKEdit_WriteCharUtf16(ct, 0xD800|((val1>>10)&1023), enc);
		ct=TKEdit_WriteCharUtf16(ct, 0xDC00|((val1    )&1023), enc);
		return(ct);
	}

	if(enc==TKEDIT_LENC_UTF16L)
	{
		*ct++=(val   )&255;
		*ct++=(val>>8)&255;
		return(ct);
	}else
	{
		*ct++=(val>>8)&255;
		*ct++=(val   )&255;
		return(ct);
	}
}

int TKEdit_BufferDecodeUtf16(
	tkedit_char *dbuf, tkedit_byte *sbuf,
	int sz, int enc)
{
	tkedit_char *ct;
	tkedit_byte *cs, *cse;
	int i;
	
	cs=sbuf;	cse=sbuf+sz;
	ct=dbuf;

	if(enc==TKEDIT_LENC_UTF16L)
	{
		if((cs[0]==0xFF) && (cs[1]==0xFE))
			cs+=2;
		while(cs<cse)
		{
			i=cs[0] | (cs[1]<<8);
			cs+=2;
			*ct++=i;
		}
	}else
	{
		if((cs[0]==0xFE) && (cs[1]==0xFF))
			cs+=2;
		while(cs<cse)
		{
			i=(cs[0]<<8) | cs[1];
			cs+=2;
			*ct++=i;
		}		
	}
	return(ct-dbuf);
}

int TKEdit_ReadCharUtf8(tkedit_byte **rcs)
{
	tkedit_byte *cs;
	int i, k;

	cs=*rcs;
	i=*cs++;
	if(!(i&0x80))
	{
		k=i;
	}else if((i&0xE0)==0xC0)
	{
		k=((i&31)<<6)|(cs[0]&63);
		cs++;
	}else if((i&0xF0)==0xE0)
	{
		k=((i&15)<<12)|((cs[0]&63)<<6)|(cs[1]&63);
		cs+=2;
	}else if((i&0xF8)==0xF0)
	{
		k=((i&7)<<18)|((cs[0]&63)<<12)|((cs[1]&63)<<6)|(cs[2]&63);
		cs+=3;
	}else if((i&0xFC)==0xF8)
	{
		k=((i&3)<<24)|((cs[0]&63)<<18)|((cs[1]&63)<<12)|
			((cs[2]&63)<<6)|(cs[3]&63);
		cs+=4;
	}else
	{
		k=i;
	}
	*rcs=cs;
	return(k);
}

tkedit_byte *TKEdit_WriteCharUtf8(tkedit_byte *ct, int val)
{
	if((val>0) && (val<0x80))
	{
		*ct++=val;
		return(ct);
	}else if(val<0x800)
	{
		*ct++=0xC0|((val>>6)&31);
		*ct++=0x80|((val   )&63);
		return(ct);
	}else if(val<0x10000)
	{
		*ct++=0xE0|((val>>12)&15);
		*ct++=0x80|((val>> 6)&63);
		*ct++=0x80|((val    )&63);
		return(ct);
	}else if(val<0x200000)
	{
		*ct++=0xF0|((val>>18)& 7);
		*ct++=0x80|((val>>12)&63);
		*ct++=0x80|((val>> 6)&63);
		*ct++=0x80|((val    )&63);
		return(ct);
	}else if(val<0x4000000)
	{
		*ct++=0xF8|((val>>24)& 3);
		*ct++=0x80|((val>>18)&63);
		*ct++=0x80|((val>>12)&63);
		*ct++=0x80|((val>> 6)&63);
		*ct++=0x80|((val    )&63);
		return(ct);
	}
	return(ct);
}

int TKEdit_CostWriteCharUtf8(int val)
{
	tkedit_byte tb[16];
	tkedit_byte *ct1;
	ct1=TKEdit_WriteCharUtf8(tb, val);
	return(ct1-tb);
}

int TKEdit_BufferDecodeUtf8(
	tkedit_char *dbuf, tkedit_byte *sbuf,
	int sz, int enc)
{
	tkedit_char *ct;
	tkedit_byte *cs, *cse;
	int i, j;
	
	cs=sbuf;	cse=sbuf+sz;
	ct=dbuf;

	if((cs[0]==0xEF) && (cs[1]==0xBB) && (cs[2]==0xBF))
		cs+=3;
	while(cs<cse)
	{
		i=TKEdit_ReadCharUtf8(&cs);
		if(i>=0x10000)
		{
			j=i-0x10000;
			*ct++=0xD800|((j>>10)&1023);
			*ct++=0xDC00|((j    )&1023);
			continue;
		}
		*ct++=i;
	}
	return(ct-dbuf);
}

static const tkedit_char tkedit_tuc52[32]={
	0x20AC, 0x0081, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021,
	0x02C6, 0x2030, 0x0160, 0x2039, 0x0152, 0x008D, 0x017D, 0xFFFF,
	0x0090, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,
	0x02DC, 0x2122, 0x0161, 0x203A, 0x0153, 0x009C, 0x017E, 0x0178};

int TKEdit_ConvAsciiToEditChar(int asc)
{
	if((asc>=0x00) && (asc<=0x7F))
		return(asc);
	if((asc>=0xA0) && (asc<=0xFF))
		return(asc);
	if((asc>=0x80) && (asc<=0x9F))
		return(tkedit_tuc52[asc-0x80]);
	return(asc);
}

int TKEdit_ConvEditCharToAscii(int asc)
{
	if((asc>=0x00) && (asc<=0x7F))
		return(asc);
	if((asc>=0xA0) && (asc<=0xFF))
		return(asc);
//	if((asc>=0x80) && (asc<=0x9F))
//		return(tkedit_tuc52[asc-0x80]);
	return(asc);
}

int TKEdit_ContextCheckExpandTextBuffer(TKEdit_EditContext *ctx, int max)
{
	int bsz, nsz, sz1;

	if(!ctx->textbuf_buf)
	{
		nsz=max;
		sz1=1024;
		while(msz>=sz1)
			sz1=sz1+(sz1>>1);
		ctx->textbuf_buf=tkedit_malloc(sz1*sizeof(tkedit_char));
		ctx->textbuf_size=sz1;
		ctx->textbuf_pos=0;
		return(1);
	}

	bsz=ctx->textbuf_size;
	nsz=ctx->textbuf_pos+max;

	if(nsz<bsz)
		return(0);
	
	sz1=bsz;
	while(nsz>=sz1)
		sz1=sz1+(sz1>>1);
	ctx->textbuf_buf=tkedit_realloc(ctx->textbuf_buf,
		sz1*sizeof(tkedit_char));
	ctx->textbuf_size=sz1;
	return(1);
}

int TKEdit_ContextCheckExpandLines(TKEdit_EditContext *ctx, int max)
{
	int lm, lm1;
	
	if(!ctx->line_pos)
	{
		lm=256;
		while(max>=lm)
			lm=lm+(lm>>1);

		ctx->line_pos=tkedit_malloc(lm*sizeof(int));
		ctx->line_len=tkedit_malloc(lm*sizeof(tkedit_len));
		ctx->line_max=tkedit_malloc(lm*sizeof(tkedit_len));
		ctx->m_lines=lm;
		ctx->n_lines=0;
		return(1);
	}
	
	lm=ctx->line_max;
	if(max<lm)
		return(0);
	
	lm1=lm;
	while(max>=lm1)
		lm1=lm1+(lm1>>1);

	ctx->line_pos=tkedit_realloc(ctx->line_pos, lm1*sizeof(int));
	ctx->line_len=tkedit_realloc(ctx->line_len, lm1*sizeof(tkedit_len));
	ctx->line_max=tkedit_realloc(ctx->line_max, lm1*sizeof(tkedit_len));
	ctx->m_lines=lm1;
	return(1);
}


int TKEdit_ContextInsertLine(TKEdit_EditContext *ctx,
	int lnum, tkedit_char *str, int len)
{
	int *lpos;				//offset to start of line in text buffer
	tkedit_len *llen;		//current length of line in text buffer
	tkedit_len *lmax;		//max length of line in text buffer
	int i, l, m, p;

	l=ctx->n_lines;
	TKEdit_ContextCheckExpandLines(ctx, l+1);
	ctx->n_lines=l+1;

	lpos=ctx->line_pos;				//offset to start of line in text buffer
	llen=ctx->line_len;		//current length of line in text buffer
	lmax=ctx->line_max;		//max length of line in text buffer
	while(l>lnum)
	{
		lpos[l]=lpos[l-1];
		llen[l]=llen[l-1];
		lmax[l]=lmax[l-1];
		l--;
	}
	
	l=len;
	m=(len+7)&(~3);
	p=TKEdit_ContextAllocTextU(ctx, str, l, m);
	lpos[lnum]=p;
	llen[lnum]=l;
	lmax[lnum]=m;
	return(0);
}

int TKEdit_ContextInsertEmptyLine(TKEdit_EditContext *ctx, int lnum)
{
	tkedit_char c;
	c=0;
	return(TKEdit_ContextInsertLine(ctx, lnum, &c, 0));
}

int TKEdit_ContextInsertLineBreak(TKEdit_EditContext *ctx,
	int lnum, int xpos)
{
	tkedit_char tb0[256];
	tkedit_char *cs;
	int i, j, k, l, ll;

	if(lnum>=ctx->n_lines)
	{
		return(TKEdit_ContextInsertEmptyLine(ctx, ctx->n_lines));
	}

	ll=ctx->line_len[lnum];
	if(xpos>=ll)
	{
		return(TKEdit_ContextInsertEmptyLine(ctx, lnum+1));
	}
	
	cs=ctx->textbuf_buf+ctx->line_pos[lnum]+xpos;
	l=ll-xpos;
	for(i=0; i<l; i++)
	{
		tb0[i]=cs[i];
	}
	TKEdit_ContextInsertLine(ctx, lnum+1, tb0, l);
	ctx->line_len[lnum]=xpos;
	return(1);
}

int TKEdit_ContextInsertChar(TKEdit_EditContext *ctx,
	int lnum, int xpos, int val)
{
	tkedit_char *cs;
	int i, j, k, l, lp, ll, lm, lm1;

	if((val=='\r') || (val=='\n'))
	{
		i=TKEdit_ContextInsertLineBreak(ctx, lnum, xpos);
		return(i);
	}
		
	cs=ctx->textbuf_buf+ctx->line_pos[lnum];
	l=ll-xpos;
	ll=ctx->line_len[lnum];
	lm=ctx->line_max[lnum];
	if(xpos>=ll)
	{
		if(ll<lm)
		{
			cs[ll]=val;
			ctx->line_len[lnum]=ll+1;
			return(1);
		}

		j=lm>>2;
		if(j<8)j=8;
		if(j>32)j=32;
		lm1=lm+j;
		lp=TKEdit_ContextAllocTextU(ctx, cs, ll+1, lm1);
		ctx->line_pos[lnum]=lp;
		ctx->line_len[lnum]=ll+1;
		ctx->line_max[lnum]=lm1;

		cs=ctx->textbuf_buf+lp;
		cs[ll]=val;
		return(1);
	}

	if(ll<lm)
	{
		j=ll;
		while(j>xpos)
			{ cs[j]=cs[j-1]; j--; }
		cs[xpos]=val;
		ctx->line_len[lnum]=ll+1;
		return(1);
	}

	j=lm>>2;
	if(j<8)j=8;
	if(j>32)j=32;
	lm1=lm+j;
	lp=TKEdit_ContextAllocTextU(ctx, cs, ll+1, lm1);
	ctx->line_pos[lnum]=lp;
	ctx->line_len[lnum]=ll+1;
	ctx->line_max[lnum]=lm1;

	cs=ctx->textbuf_buf+lp;
	j=ll;
	while(j>xpos)
		{ cs[j]=cs[j-1]; j--; }
	cs[xpos]=val;
	return(1);
}

int TKEdit_ContextAllocTextA(TKEdit_EditContext *ctx, tkedit_byte *str,
	int len, int max)
{
	tkedit_char *buf;
	int pos;
	int i;

	TKEdit_ContextCheckExpandTextBuffer(ctx, max);
	
	pos=ctx->textbuf_pos;
	ctx->textbuf_pos+=max;
	buf=ctx->textbuf_buf+pos;
	
	for(i=0; i<len; i++)
		{ pos[i]=TKEdit_ConvAsciiToEditChar(str[i]); }
	
	return(pos);
}

int TKEdit_ContextAllocTextU(TKEdit_EditContext *ctx, tkedit_char *str,
	int len, int max)
{
	tkedit_char *buf;
	int pos;
	int i;

	TKEdit_ContextCheckExpandTextBuffer(ctx, max);
	
	pos=ctx->textbuf_pos;
	ctx->textbuf_pos+=max;
	buf=ctx->textbuf_buf+pos;
	
	for(i=0; i<len; i++)
		{ pos[i]=str[i]; }
	
	return(pos);
}

TKEdit_EditContext *TKEdit_NewContextFromBufferA(tkedit_byte *buf, int sz)
{
	TKEdit_EditContext *ctx;
	tkedit_byte *cs, *cse, *cs1;
	int lc, lm, lpsz, lmsz;
	int i, j, k, l, p, n, lety;

	lc=TKEdit_CountBufferLinesA(buf, sz);
	lpsz=sz+(lc*16);
	
	lety=TKEDIT_LETYPE_UNK;
	cs1=TKEdit_BufferNextLineA(buf, buf+sz, &l);
	if(buf[l]=='\n')
		lety=TKEDIT_LETYPE_LF;
	else if((buf[l+0]=='\r') && (buf[l+0]=='\n'))
		lety=TKEDIT_LETYPE_CRLF;
	else if(buf[l]=='\r')
		lety=TKEDIT_LETYPE_CR;

	ctx=TKEdit_AllocContext();
	TKEdit_ContextCheckExpandLines(ctx, lc);
	TKEdit_ContextCheckExpandTextBuffer(ctx, lpsz);
	ctx->letype=lety;
	ctx->lenc=TKEDIT_LENC_ASCII;

	cs=buf; cse=buf+sz; n=0;
	while(cs<cse)
	{
		cs1=TKEdit_BufferNextLineA(cs, cse, &l);
		
		k=7+l;
		k=(k+3)&(~3);
		p=TKEdit_ContextAllocTextA(ctx, cs, l, k);

		ctx->line_pos[n]=p;
		ctx->line_len[n]=l;
		ctx->line_max[n]=k;

		n++;
		cs=cs1;
	}
	ctx->n_lines=n;

	return(i);
}

TKEdit_EditContext *TKEdit_NewContextFromBufferU(tkedit_char *buf, int sz)
{
	TKEdit_EditContext *ctx;
	tkedit_char *cs, *cse, *cs1;
	int lc, lm, lpsz, lmsz;
	int i, j, k, l, p, n, lety;

	lc=TKEdit_CountBufferLinesU(buf, sz);
	lpsz=sz+(lc*16);
	
	lety=TKEDIT_LETYPE_UNK;
	cs1=TKEdit_BufferNextLineU(buf, buf+sz, &l);
	if(buf[l]=='\n')
		lety=TKEDIT_LETYPE_LF;
	else if((buf[l+0]=='\r') && (buf[l+0]=='\n'))
		lety=TKEDIT_LETYPE_CRLF;
	else if(buf[l]=='\r')
		lety=TKEDIT_LETYPE_CR;

	ctx=TKEdit_AllocContext();
	TKEdit_ContextCheckExpandLines(ctx, lc);
	TKEdit_ContextCheckExpandTextBuffer(ctx, lpsz);
	ctx->letype=lety;
	ctx->lenc=TKEDIT_LENC_UTF8;	

	cs=buf; cse=buf+sz; n=0;
	while(cs<cse)
	{
		cs1=TKEdit_BufferNextLineU(cs, cse, &l);
		
		k=7+l;
		k=(k+3)&(~3);
		p=TKEdit_ContextAllocTextU(ctx, cs, l, k);

		ctx->line_pos[n]=p;
		ctx->line_len[n]=l;
		ctx->line_max[n]=k;

		n++;
		cs=cs1;
	}
	ctx->n_lines=n;

	return(i);
}

int TKEdit_ContextGetFlattenSizeA(TKEdit_EditContext *ctx)
{
	int lety, lesz;
	int i, j, k, sz;
	
	lety=ctx->letype;
	lesz=1;
	if(lety==TKEDIT_LETYPE_CRLF)
		lesz=2;
	
	sz=0;
	for(i=0; i<ctx->n_lines; i++)
	{
		sz+=ctx->line_len[i]+lesz;
	}
	return(sz);
}

int TKEdit_ContextGetFlattenSizeU8(TKEdit_EditContext *ctx)
{
	tkedit_char *txtbuf, *cs, *cse;
	int *lpos;
	tkedit_len *llen;
	int lety, lesz;
	int i, j, k, sz;
	
	lety=ctx->letype;
	lesz=1;
	if(lety==TKEDIT_LETYPE_CRLF)
		lesz=2;
	
	lpos=ctx->line_pos;
	llen=ctx->line_len;
	txtbuf=ctx->textbuf_buf;
	sz=0;
	for(i=0; i<ctx->n_lines; i++)
	{
		l=llen[i];
		cs=txtbuf+lpos[i];
		cse=cs+l;
		
		while(cs<cse)
			sz+=TKEdit_CostWriteCharUtf8(*cs++);
		sz+lesz;
	}

	return(sz);
}

int TKEdit_ContextGetFlattenSizeU16(TKEdit_EditContext *ctx)
{
	int sz;
	sz=TKEdit_ContextGetFlattenSizeA(ctx);
	return(sz*2);
}

int TKEdit_ContextFlattenBuffer(TKEdit_EditContext *ctx,
	tkedit_byte *buf, int *rsz)
{
	tkedit_char *txtbuf, *cs;
	int *lpos;
	tkedit_len *llen;
	tkedit_byte *ct;
	int lety, lesz, lenc;
	int i, j, k, l, sz;
	
	if(!buf)
	{
		lenc=ctx->lenc;

		if((lenc==TKEDIT_LENC_ASCII) || (lenc==TKEDIT_LENC_ASCIICP))
		{
			sz=TKEdit_ContextGetFlattenSizeA(ctx);
			*rsz=sz;
			return(1);
		}

		if((lenc==TKEDIT_LENC_UTF16L) || (lenc==TKEDIT_LENC_UTF16B))
		{
			sz=TKEdit_ContextGetFlattenSizeU16(ctx);
			*rsz=sz;
			return(1);
		}

		if(lenc==TKEDIT_LENC_UTF8)
		{
			sz=TKEdit_ContextGetFlattenSizeU8(ctx);
			*rsz=sz;
			return(1);
		}

		*rsz=sz;
		return(1);
	}
	
	lenc=ctx->lenc;
	lety=ctx->letype;
	lesz=1;
	if(lety==TKEDIT_LETYPE_CRLF)
		lesz=2;
	
	lpos=ctx->line_pos;
	llen=ctx->line_len;
	txtbuf=ctx->textbuf_buf;
	ct=buf;
	sz=0;
	for(i=0; i<ctx->n_lines; i++)
	{
		l=llen[i];
		cs=txtbuf+lpos[i];
		
		if(lenc==TKEDIT_LENC_ASCII)
		{
			for(j=0; j<l; j++)
				{ ct[j]=cs[j]; }
			ct+=l;
		}
		else if(lenc==TKEDIT_LENC_ASCIICP)
		{
			for(j=0; j<l; j++)
				{ ct[j]=TKEdit_ConvEditCharToAscii(cs[j]); }
			ct+=l;
		}else if(lenc==TKEDIT_LENC_UTF8)
		{
			for(j=0; j<l; j++)
				{ ct=TKEdit_WriteCharUtf8(ct, cs[j]); }
		}else if(	(lenc==TKEDIT_LENC_UTF16L) ||
					(lenc==TKEDIT_LENC_UTF16B))
		{
			for(j=0; j<l; j++)
				{ ct=TKEdit_WriteCharUtf16(ct, cs[j], lenc); }
		}


		if((lenc==TKEDIT_LENC_ASCII) || (lenc==TKEDIT_LENC_ASCIICP) ||
			(lenc==TKEDIT_LENC_UTF8))
		{
			if(lety==TKEDIT_LETYPE_CRLF)
				{ *ct++='\r'; *ct++='\n'; }
			else if(lety==TKEDIT_LETYPE_LF)
				{ *ct++='\n'; }
			else if(lety==TKEDIT_LETYPE_CR)
				{ *ct++='\r'; }
		}else if(	(lenc==TKEDIT_LENC_UTF16L) ||
					(lenc==TKEDIT_LENC_UTF16B))
		{
			if(lety==TKEDIT_LETYPE_CRLF)
			{	ct=TKEdit_WriteCharUtf16(ct, '\r', lenc);
				ct=TKEdit_WriteCharUtf16(ct, '\n', lenc);	}
			else if(lety==TKEDIT_LETYPE_LF)
				{ ct=TKEdit_WriteCharUtf16(ct, '\n', lenc); }
			else if(lety==TKEDIT_LETYPE_CR)
				{ ct=TKEdit_WriteCharUtf16(ct, '\r', lenc); }
		}
	}
	
	sz=ct-buf;
	*rsz=sz;
	
	return(1);
}

TKEdit_EditContext *TKEdit_NewContextFromFile(char *name)
{
	TKEdit_EditContext *ctx;
	tkedit_char *cbuf;
	void *buf;
	int sz, sz1, enc;

	buf=tkedit_loadfile(name, &sz);
	if(!buf)
		return(NULL);
		
	enc=TKEdit_BufferGetEncId(buf, sz);
	if((enc==TKEDIT_LENC_ASCII) || (enc==TKEDIT_LENC_ASCIICP))
	{
		ctx=TKEdit_NewContextFromBufferA(buf, sz);
		ctx->lenc=enc;
		tkedit_free(buf);
		return(ctx);
	}
	
	if((enc==TKEDIT_LENC_UTF16L) || (enc==TKEDIT_LENC_UTF16B))
	{
		sz1=sz>>1;
		cbuf=tkedit_malloc((sz1+2)*sizeof(tkedit_char));
		sz1=TKEdit_BufferDecodeUtf16(cbuf, buf, sz, enc);
		ctx=TKEdit_NewContextFromBufferU(cbuf, sz1);
		ctx->lenc=enc;
		tkedit_free(buf);
		tkedit_free(cbuf);
		return(ctx);
	}
	
	if(enc==TKEDIT_LENC_UTF8)
	{
		sz1=sz;
		cbuf=tkedit_malloc((sz1+2)*sizeof(tkedit_char));
		sz1=TKEdit_BufferDecodeUtf8(cbuf, buf, sz, enc);
		ctx=TKEdit_NewContextFromBufferU(cbuf, sz1);
		ctx->lenc=enc;
		tkedit_free(buf);
		tkedit_free(cbuf);
		return(ctx);
	}
	
	tkedit_free(buf);
	return(NULL);
}

int TKEdit_SaveContextToFile(TKEdit_EditContext *ctx, char *name)
{
	void *buf;
	int i, j, k, sz;

	i=TKEdit_ContextFlattenBuffer(ctx, NULL, &sz);
	if(i<0)return(i);
	buf=tkedit_malloc(sz+16);
	i=TKEdit_ContextFlattenBuffer(ctx, buf, &sz);
	if(i<0)
	{
		tkedit_free(buf);
		return(i);
	}
}
