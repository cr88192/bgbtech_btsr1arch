static char *tk_base85_1924=
	"0123456789"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"!#$%&()*+-;<=>?@^_`{|}~";

char *tk_base85r_1924[128]={
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  //00..0F
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  //10..1F
-1, 62, -1, 63, 64, 65, 66, -1, 67, 68, 69, 70, -1, 71, -1, -1,  //20..2F
 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, 72, 73, 74, 75, 76,  //30..3F
77, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,  //40..4F
25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, 78, 79,  //50..5F
80, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,  //60..6F
51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 81, 82, 83, 84, -1,  //70..7F
};

char *TK_RegCache_Base85EncodeWord(char *ct, u32 val)
{
	u32 v0, v1;
	int i0, i1, i2, i3, i4, i5;
	
	v0=val/85;
	v1=v0/85;
	i0=val-(v0*85);
	v0=v1;
	v1=v0/85;
	i1=v0-(v1*85);
	v0=v1;
	v1=v0/85;
	i2=v0-(v1*85);
	v0=v1;
	v1=v0/85;
	i3=v0-(v1*85);
	v0=v1;
	v1=v0/85;
	i4=v0-(v1*85);
	
	ct[0]=tk_base85_1924[i4];
	ct[1]=tk_base85_1924[i3];
	ct[2]=tk_base85_1924[i2];
	ct[3]=tk_base85_1924[i1];
	ct[4]=tk_base85_1924[i0];
	ct[5]=0;
	return(ct+5);
}

u32 TK_RegCache_Base85DecodeWord(char **rcs)
{
	char *cs;
	int i0, i1, i2, i3, i4;
	u32 v;
	cs=*rcs;
	i0=tk_base85r_1924[cs[0]];
	i1=tk_base85r_1924[cs[1]];
	i2=tk_base85r_1924[cs[2]];
	i3=tk_base85r_1924[cs[3]];
	i4=tk_base85r_1924[cs[4]];
	v=((((i0*85)+i1)*85+i2)*85+i3)*85+i4;
	*rcs=cs+5;
	return(v);
}

TK_RegContext *TK_RegCache_AllocRegContext()
{
	TK_RegContext *tmp;
	
	tmp=tk_malloc(sizeof(TK_RegContext));
	memset(tmp, 0, sizeof(TK_RegContext));
	return(tmp);
}

TK_RegDirEnt *TK_RegCache_GetDirEntIndex(TK_RegContext *ctx, int index)
{
	int ix1;
	
	ix1=index>>8;
	if(!(ctx->regdir))
	{
		ctx->regdir=tk_malloc(1024*sizeof(TK_RegDirEnt *));
		memset(ctx->regdir, 0, 1024*sizeof(TK_RegDirEnt *));
	}
	
	if(!(ctx->regdir[ix1]))
	{
		ctx->regdir[ix1]=tk_malloc(256*sizeof(TK_RegDirEnt));
	}
}

int TK_RegCache_HashNameDir(char *name, int dirid)
{
	char *cs;
	int h;

	cs=name;
	h=dirid*65521;
	while(*cs)
	{
		h=h*65521+(*cs++);
	}
	h=h*65521+1;
	h=h*65521+1;	
	return((h>>16)&1023);
}

int TK_RegCache_LookupKeyIndexNameDir(TK_RegContext *ctx,
	char *name, int dirid)
{
	TK_RegDirEnt *tde;
	int h, i;
	
	h=TK_RegCache_HashNameDir(name, dirid);

	i=ctx->rdehash[h];
	while(i>0)
	{
		tde=TK_RegCache_GetDirEntIndex(ctx, i);
		if((tde->dirid==dirid) && !strcmp(tde->name, name))
			return(i);
		i=tde->chain;
	}
	
	return(0);
}

int TK_RegCache_GetKeyIndexNameDir(TK_RegContext *ctx,
	char *name, int dirid)
{
	TK_RegDirEnt *tde;
	int h, i;
	
	h=TK_RegCache_HashNameDir(name, dirid);

	i=ctx->rdehash[h];
	while(i>0)
	{
		tde=TK_RegCache_GetDirEntIndex(ctx, i);
		if((tde->dirid==dirid) && !strcmp(tde->name, name))
			return(i);
		i=tde->chain;
	}
	
	i=ctx->lastidx++;
	if(!i)
		i=ctx->lastidx++;
	tde=TK_RegCache_GetDirEntIndex(ctx, i);
	tde->dirid=dirid;
	strcpy(tde->name, name);
	
	tde->chain=ctx->rdehash[h];
	ctx->rdehash[h]=i;
	
	return(0);
}

int TK_RegCache_LookupKeyIndexPath(TK_RegContext *ctx, char *path)
{
	char tbn[64];
	char *cs, *ct;
	int dxi;
	
	cs=path;
	dxi=0;
	while(*cs)
	{
		while(*cs=='/')
			cs++;
		ct=tbn;
		while(*cs && (*cs!='/'))
			*ct++=*cs++;
		*ct++=0;
		dxi=TK_RegCache_LookupKeyIndexNameDir(ctx, tbn, dxi);
		if(dxi<=0)
			break;
	}
	return(dxi);
}

int TK_RegCache_GetKeyIndexPath(TK_RegContext *ctx, char *path)
{
	char tbn[64];
	TK_RegDirEnt *tde;
	char *cs, *ct;
	int dxi;
	
	cs=path;
	dxi=0;
	while(*cs)
	{
		while(*cs=='/')
			cs++;
		ct=tbn;
		while(*cs && (*cs!='/'))
			*ct++=*cs++;
		*ct++=0;
		dxi=TK_RegCache_GetKeyIndexNameDir(ctx, tbn, dxi);
		if(dxi<=0)
			break;
		if(*cs=='/')
		{
			tde=TK_RegCache_GetDirEntIndex(ctx, dxi);
			if(tde->ety==TK_REGCACHE_ETY_DEFAULT)
			{
				tde->ety=TK_REGCACHE_ETY_DIR;
			}
			if(tde->ety!=TK_REGCACHE_ETY_DIR)
				return(-1);
		}
	}
	return(dxi);
}

int TK_RegCache_GetKeyPath_Str(TK_RegContext *ctx, char *path,
	char *buf, int bsz)
{
	TK_RegDirEnt *tde;
	TK_RegDirEnt_LongStr *tdels;
	int dxi, lim;

	dxi=TK_RegCache_LookupKeyIndexPath(ctx, path);
	if(dxi<=0)
	{
		buf[0]=0;
		return(-1);
	}

	tde=TK_RegCache_GetDirEntIndex(ctx, dxi);
	if(tde->ety==TK_REGCACHE_ETY_SHORTSTR)
	{
		lim=bsz;
		if(lim>64)
			lim=64;
		strncpy(buf, tde->data, lim);
		return(0);
	}

	if(tde->ety==TK_REGCACHE_ETY_LONGSTR)
	{
		tdels=(TK_RegDirEnt_LongStr *)(tde->data);

		lim=bsz;
		if(lim>tdels->len)
			lim=tdels->len;
		strncpy(buf, tdels->ptr, lim);
		return(0);
	}

	buf[0]=0;
	return(-1);
}

int TK_RegCache_SetKeyPath_Str(TK_RegContext *ctx, char *path, char *buf)
{
	TK_RegDirEnt *tde;
	TK_RegDirEnt_LongStr *tdels;
	int dxi, lim, len;

	dxi=TK_RegCache_LookupKeyIndexPath(ctx, path);
	if(dxi<=0)
	{
		return(-1);
	}

	tde=TK_RegCache_GetDirEntIndex(ctx, dxi);

	if(tde->ety==TK_REGCACHE_ETY_DIR)
	{
		return(-1);
	}

	len=strlen(buf);
	if((tde->ety==TK_REGCACHE_ETY_SHORTSTR) ||
		(tde->ety==TK_REGCACHE_ETY_DEFAULT))
	{
		if(len<=64)
		{
			tde->ety=TK_REGCACHE_ETY_SHORTSTR;
			strncpy(tde->data, buf, 64);
			return(0);
		}
		/* Expand to longstr */

		tde->ety=TK_REGCACHE_ETY_LONGSTR;
		tdels=(TK_RegDirEnt_LongStr *)(tde->data);

		lim=64;
		while(lim<=len)
			lim=lim+(lim>>1);
		tdels->ptr=tk_malloc(lim);
		tdels->len=len;
		tdels->lim=lim;
		strncpy(tdels->ptr, buf, len);
		return(0);
	}
	
	if(tde->ety==TK_REGCACHE_ETY_LONGSTR)
	{
		tdels=(TK_RegDirEnt_LongStr *)(tde->data);
		lim=tdels->lim;

		if(len<lim)
		{
			tdels->len=len;
			strncpy(tdels->ptr, buf, len);
			return(0);
		}

		while(lim<=len)
			lim=lim+(lim>>1);
		tdels->ptr=tk_realloc(tdels->ptr, lim);
		tdels->len=len;
		tdels->lim=lim;
		strncpy(tdels->ptr, buf, len);
		return(0);
	}

	return(-1);
}

int TK_RegCache_ParseIniBuf(TK_RegContext *ctx, char *inibuf)
{
	char tbp[256];
	char tbuf[512];
	char tbn[64];
	char tbv[256];
	char *cs, *cs1, *ct1;
	
	strcpy(tbp, "");
	
	cs=inibuf;
	while(*cs)
	{
		cs=tk_getstrline(tbuf, 511, cs);
		if(tbuf[0]=='[')
		{
			cs1=tbuf+1;
			ct1=tbp;
			while(*cs1 && (*cs1!=']'))
				*ct1++=*cs1++;
			*ct1=0;
			continue;
		}
		
		cs1=tbuf;
		while(*cs1 && *cs1<=' ')
			cs1++;
		
		if(!(*cs1))
			continue;

		if(*cs1==';')
			continue;
		if(*cs1=='/')
			continue;
		if(*cs1=='#')
			continue;
		
		ct1=tbn;
		while(*cs1 && (*cs1>' ') && (*cs1!='='))
			*ct1++=*cs1++;
		*ct1=0;

		while(*cs1 && *cs1<=' ')
			cs1++;
		
		if(*cs1=='=')
		{
			cs1++;
			while(*cs1 && *cs1<=' ')
				cs1++;

			ct1=tbv;
			while(*cs1)
				*ct1++=*cs1++;
			*ct1=0;
			
			if(tbp[0])
			{
				strcpy(tbuf, tbp);
				strcat(tbuf, "/");
				strcat(tbuf, tbn);
			}else
			{
				strcpy(tbuf, tbn);
			}

			TK_RegCache_SetKeyPath_Str(ctx, tbuf, tbv);
			continue;
		}
	}
}

TK_RegContext *tk_regcache_gblctx;

int TK_RegGbl_SetKeyPath_Str(char *path, char *buf)
{
	if(!tk_regcache_gblctx)
	{
		tk_regcache_gblctx=TK_RegCache_AllocRegContext();
	}
	
	return(TK_RegCache_SetKeyPath_Str(tk_regcache_gblctx, path, buf));
}

int TK_RegGbl_GetKeyPath_Str(char *path, char *buf, int bufsz)
{
	if(!tk_regcache_gblctx)
	{
		return(-1);
//		tk_regcache_gblctx=TK_RegCache_AllocRegContext();
	}
	
	return(TK_RegCache_GetKeyPath_Str(tk_regcache_gblctx, path, buf, bufsz));
}
