void *tkucc_malloc(int sz)
{
	return(malloc(sz));
}

void tkucc_free(void *p)
{
	free(p);
}

void *tkucc_ralloc(int sz)
{
	static char *rabuf=NULL;
	static u16 rov;
	
	char *p;
	
	if(!rabuf)
	{
		rabuf=malloc(65536);
		rov=0;
	}
	
	if((rov+sz)>=65536)
		rov=0;
	
	p=rabuf+rov;
	rov+=sz;
	rov=(rov+7)&(~7);
	return(p);
}

char *tkucc_rstrdup(char *str)
{
	char *p;
	
	p=tkucc_ralloc(strlen(str)+1);
	strcpy(p, str);
	return(p);
}

TKUCC_MainContext *TKUCC_AllocMainContext(void)
{
	TKUCC_MainContext *ctx;
	
	ctx=tkucc_malloc(sizeof(TKUCC_MainContext));
	memset(ctx, 0, sizeof(TKUCC_MainContext));
	return(ctx);
}

int TKUCC_ZmSizeToIndex(int sz)
{
	int e, h, i;

	h=sz; e=0;
	while(h>=16)
		{ h=(h+1)>>1; e++; }
	i=(e<<3)|(h&7);
	return(i);
}

int TKUCC_ZmIndexToSize(int ix)
{
	int i;
	
	i=(8|(ix&7))<<((ix>>3)&31);
	return(i);
}

void *TKUCC_ZMalloc(TKUCC_MainContext *ctx,
	int sz, int zid)
{
	TKUCC_ZoneAllocHead *zhd;
	int ix, sz1;
	
	ix=TKUCC_ZmSizeToIndex(sz);
	
	zhd=ctx->zfree[ix];
	if(zhd)
	{
		ctx->zfree[ix]=zhd->znext;
		
		zhd->znext=ctx->zfirst[zid];
		ctx->zfirst[zid]=zhd;
		zhd->zone=zid;
		return((char *)(zhd->data)+sizeof(void *));
	}
	
	sz1=TKUCC_ZmIndexToSize(ix);
	zhd=tkucc_malloc(sizeof(TKUCC_ZoneAllocHead));
	zhd->data=tkucc_malloc(sz1+sizeof(void *));
	*(void **)(zhd->data)=zhd;

	zhd->znext=ctx->zfirst[zid];
	ctx->zfirst[zid]=zhd;
	zhd->zone=zid;
	zhd->szix=ix;
	zhd->ctx=ctx;
	return((char *)(zhd->data)+sizeof(void *));
}

void *TKUCC_ZMallocZero(TKUCC_MainContext *ctx,
	int sz, int zid)
{
	void *p;
	
	p=TKUCC_ZMalloc(ctx, sz, zid);
	if(!p)
		return(NULL);
	memset(p, 0, sz);
	return(p);
}

void TKUCC_ZUnlinkHeader(TKUCC_MainContext *ctx,
	TKUCC_ZoneAllocHead *zhd)
{
	TKUCC_ZoneAllocHead *zcur;
	
	zcur=ctx->zfirst[zhd->zone];
	if(zcur==zhd)
	{
		ctx->zfirst[zhd->zone]=zhd->znext;
	}else if(zcur)
	{
		while(zcur->znext)
		{
			if(zcur->znext==zhd)
				break;
			zcur=zcur->znext;
		}
		if(zcur->znext==zhd)
		{
			zcur->znext=zhd->znext;
		}
	}
}

void TKUCC_ZFreeHeader(TKUCC_MainContext *ctx,
	TKUCC_ZoneAllocHead *zhd)
{
	int ix;
	ix=zhd->szix;
	zhd->znext=ctx->zfree[ix];
	ctx->zfree[ix]=zhd;
}


void TKUCC_ZFree(void *ptr)
{
	TKUCC_ZoneAllocHead *zhd;
	TKUCC_MainContext *ctx;
	zhd=((void **)ptr)[-1];
	ctx=zhd->ctx;
	TKUCC_ZUnlinkHeader(ctx, zhd);
	TKUCC_ZFreeHeader(ctx, zhd);
}

TKUCC_MainContext *TKUCC_ZGetContext(void *ptr)
{
	TKUCC_ZoneAllocHead *zhd;
	zhd=((void **)ptr)[-1];
	return(zhd->ctx);
}

int TKUCC_ZGetZoneId(void *ptr)
{
	TKUCC_ZoneAllocHead *zhd;
	zhd=((void **)ptr)[-1];
	return(zhd->zone);
}


void TKUCC_ZFreeZone(TKUCC_MainContext *ctx,
	int zid)
{
	TKUCC_ZoneAllocHead *zcur, *znxt;
	TKUCC_ZoneStrHead *zscur, *zsnxt;


	zcur=ctx->zfirst[zid];
	while(zcur)
	{
		znxt=zcur->znext;
		TKUCC_ZFreeHeader(ctx, zcur);
		zcur=znxt;
	}
	ctx->zfirst[zid]=NULL;


	zscur=ctx->zstr[zid];
	while(zcur)
	{
		zsnxt=zscur->next;
//		TKUCC_ZFreeHeader(ctx, zcur);

		zscur->next=ctx->zstrfree;
		ctx->zstrfree=zscur;

		zscur=zsnxt;
	}
	ctx->zstr[zid]=NULL;
}

TKUCC_ZoneStrHead *TKUCC_ZAllocStringHead(TKUCC_MainContext *ctx, int zid)
{
	TKUCC_ZoneStrHead *zstr;
	int bix;

	zstr=ctx->zstrfree;
	if(zstr)
	{
		ctx->zstrfree=zstr->next;
		memset(zstr, 0, sizeof(TKUCC_ZoneStrHead));

		bix=ctx->zstrbix[zid];
		ctx->zstrbix[zid]=(bix+1)&4095;
		zstr->bix=bix;

		zstr->next=ctx->zstr[zid];
		ctx->zstr[zid]=zstr;
		return(zstr);
	}

	zstr=tkucc_malloc(sizeof(TKUCC_ZoneStrHead));
	memset(zstr, 0, sizeof(TKUCC_ZoneStrHead));
	
	bix=ctx->zstrbix[zid];
	ctx->zstrbix[zid]=(bix+1)&4095;
	zstr->bix=bix;

	zstr->next=ctx->zstr[zid];
	ctx->zstr[zid]=zstr;
	return(zstr);
}

int TKUCC_InternTokenToIndexI(TKUCC_MainContext *ctx,
	char *str, int tty, int zid)
{
	TKUCC_ZoneStrHead *zstr;
	char *s;
	int i, j, h, l, ty;

	zstr=ctx->zstr[zid];
	if(!zstr)
	{
		zstr=TKUCC_ZAllocStringHead(ctx, zid);
		zstr->n_str=1;
	}
	
	h=TKUCC_PpHashName(str);
	ty='A'+tty;
	l=strlen(str);
	
	while(zstr)
	{
		i=zstr->strhash[h];
		while(i>0)
		{
			s=zstr->strdat+zstr->strofs[i];
			if((*s==ty) && !strcmp(s+1, str))
			{
				j=(zid<<24)|(zstr->bix<<12)|i;
				return(j);
			}
			i=zstr->strchn[i];
		}
		zstr=zstr->next;
	}

	zstr=ctx->zstr[zid];
	while(zstr)
	{
		if((zstr->n_str<4096) && (zstr->str_sz+l)<32764)
		{
			i=zstr->n_str++;
			j=zstr->str_sz;
			zstr->str_sz=j+l+3;
			zstr->strofs[i]=j;
			zstr->strchn[i]=zstr->strhash[h];
			zstr->strhash[h]=i;

			s=zstr->strdat+j;
			*s=ty;
			strcpy(s+1, str);

			j=(zid<<24)|(zstr->bix<<12)|i;
			return(j);
		}
	
		zstr=zstr->next;
	}


	zstr=TKUCC_ZAllocStringHead(ctx, zid);
	zstr->n_str=1;

	i=zstr->n_str++;
	j=zstr->str_sz;
	zstr->str_sz=j+l+3;
	zstr->strofs[i]=j;
	zstr->strchn[i]=zstr->strhash[h];
	zstr->strhash[h]=i;

	s=zstr->strdat+j;
	*s=ty;
	strcpy(s+1, str);

	j=(zid<<24)|(zstr->bix<<12)|i;
	return(j);
}

int TKUCC_InternTokenToIndex(TKUCC_MainContext *ctx, char *str, int zid)
{
	if(!str)
		return(0);
	return(TKUCC_InternTokenToIndexI(ctx, str, *(str-1)-'A', zid));
}

int TKUCC_InternStringToIndex(TKUCC_MainContext *ctx, char *str, int zid)
{
	if(!str)
		return(0);
	return(TKUCC_InternTokenToIndexI(ctx, str, 0, zid));
}

char *TKUCC_InternIndexToString(TKUCC_MainContext *ctx, int idx)
{
	TKUCC_ZoneStrHead *zstr;
	int zid, bix, ix;
	char *s;

	if(!idx)
		return(NULL);

	zid=(idx>>24)&255;
	bix=(idx>>12)&4095;
	ix =(idx>> 0)&4095;

	zstr=ctx->zstr[zid];
	while(zstr)
	{
		if(zstr->bix==bix)
			break;
		zstr=zstr->next;
	}
	if(!zstr)
		return(NULL);
	s=zstr->strdat+zstr->strofs[ix];
	return(s+1);
}

char *TKUCC_StrdupIntern(TKUCC_MainContext *ctx, char *str, int zid)
{
	int ix;
	ix=TKUCC_InternStringToIndex(ctx, str, zid);
	return(TKUCC_InternIndexToString(ctx, ix));
}

char *TKUCC_StrdupTokIntern(TKUCC_MainContext *ctx, char *str, int zid)
{
	int ix;
	ix=TKUCC_InternTokenToIndex(ctx, str, zid);
	return(TKUCC_InternIndexToString(ctx, ix));
}

char *TKUCC_StrdupInternPp(TKUCC_MainContext *ctx, char *str)
{
	return(TKUCC_StrdupIntern(ctx, str, TKUCC_ZID_PREPROC));
}

char *TKUCC_StrdupInternPpTemp(TKUCC_MainContext *ctx, char *str)
{
	return(TKUCC_StrdupIntern(ctx, str, TKUCC_ZID_PPTEMP));
}

char *TKUCC_StrdupTokInternPpTemp(TKUCC_MainContext *ctx, char *str)
{
	return(TKUCC_StrdupTokIntern(ctx, str, TKUCC_ZID_PPTEMP));
}

char *TKUCC_StrdupInternTokStrm(TKUCC_MainContext *ctx, char *str)
{
	return(TKUCC_StrdupIntern(ctx, str, TKUCC_ZID_TOKSTRM));
}

char *TKUCC_StrdupInternParse(TKUCC_MainContext *ctx, char *str)
{
	return(TKUCC_StrdupIntern(ctx, str, TKUCC_ZID_PARSE));
}

char *TKUCC_StrdupInternTac(TKUCC_MainContext *ctx, char *str)
{
	return(TKUCC_StrdupIntern(ctx, str, TKUCC_ZID_TAC));
}

char *TKUCC_StrdupInternAsm(TKUCC_MainContext *ctx, char *str)
{
	return(TKUCC_StrdupIntern(ctx, str, TKUCC_ZID_ASM));
}

char *TKUCC_StrdupInternLink(TKUCC_MainContext *ctx, char *str)
{
	return(TKUCC_StrdupIntern(ctx, str, TKUCC_ZID_LINK));
}

char *TKUCC_StrdupInternNameSym(TKUCC_MainContext *ctx, char *str)
{
	return(TKUCC_StrdupIntern(ctx, str, TKUCC_ZID_NAMESYM));
}
