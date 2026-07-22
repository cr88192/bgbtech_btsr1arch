GfxEdit_ConCmd *gfxedit_concmds;
GfxEdit_ConVar *gfxedit_convars;

char **gfxedit_strtab_str;
u16 *gfxedit_strtab_chn;
u16 gfxedit_strtab_hash[256];
int gfxedit_n_strs;
int gfxedit_m_strs;


int gfxedit_strhashi(char *str)
{
	char *cs;
	int h;
	
	h=1; cs=str;
	while(*cs)
		h=h*251+(*cs++);
	h=h*251;
	return((h>>8)&255);
}

int gfxedit_strdupi(char *str)
{
	int i, h;

	if(!str)
		return(0);

	if(!gfxedit_strtab_str)
	{
		gfxedit_strtab_str=malloc(16384*sizeof(char *));
		gfxedit_strtab_chn=malloc(16384*sizeof(short));
		gfxedit_n_strs=1;
		gfxedit_m_strs=16384;
	}
	
	h=gfxedit_strhashi(str);
	i=gfxedit_strtab_hash[h];
	while(i>0)
	{
		if(!strcmp(gfxedit_strtab_str[i], str))
			return(i);
		i=gfxedit_strtab_chn[i];
	}
	
	i=gfxedit_n_strs++;
	gfxedit_strtab_str[i]=strdup(str);
	gfxedit_strtab_chn[i]=gfxedit_strtab_hash[h];
	gfxedit_strtab_hash[h]=i;
	return(i);
}

char *gfxedit_strdup(char *str)
{
	int i;
	if(!str)
		return(str);
	i=gfxedit_strdupi(str);
	return(gfxedit_strtab_str[i]);
}

static char *gfxedit_splitabuf[1024];
u16 gfxedit_splitabuf_rov;

char **gfxedit_split(char *str)
{
	char *abuf[128];
	char t1buf[256];
	char *cs, *ct;
	char **a;
	int i, n;
	
	cs=str;		n=0;
	while(*cs && (*cs<=' '))	cs++;
	while(*cs)
	{
		ct=t1buf;
		if(*cs=='"')
		{
			cs++;
			while(*cs && (*cs!='"'))
			{
				if(*cs=='\\')
				{
					if((cs[1]=='\\') || (cs[1]=='\"') || (cs[1]=='\''))
						{ *ct++=cs[1]; cs+=2; continue; }
					if(cs[1]=='r')
						{ *ct++='\r'; cs+=2; continue; }
					if(cs[1]=='n')
						{ *ct++='\n'; cs+=2; continue; }
					if(cs[1]=='t')
						{ *ct++='\t'; cs+=2; continue; }
				}
				*ct++=*cs++;
			}
			if(*cs=='"')	cs++;
		}else
		{
			while(*cs>' ')
				{ *ct++=*cs++; }
		}
		*ct=0;

		while(*cs && (*cs<=' '))	cs++;
		
		abuf[n++]=gfxedit_strdup(t1buf);
	}
	
	if((gfxedit_splitabuf_rov+n)>=1024)
		gfxedit_splitabuf_rov=0;
	a=gfxedit_splitabuf+gfxedit_splitabuf_rov;
	gfxedit_splitabuf_rov+=n+1;
	
	for(i=0; i<n; i++)
		a[i]=abuf[i];
	a[n]=NULL;
	return(a);
}

char *gfxedit_strcify(char *str)
{
	char tbuf[512];
	char *cs, *ct;
	
	cs=str; ct=tbuf;
	while(*cs)
	{
		if(*cs=='\\')
			{ *ct++='\\'; *ct++='\\'; cs++; continue; }
		if(*cs=='\n')
			{ *ct++='\\'; *ct++='n'; cs++; continue; }
		if(*cs=='\r')
			{ *ct++='\\'; *ct++='r'; cs++; continue; }
		if(*cs=='\t')
			{ *ct++='\\'; *ct++='t'; cs++; continue; }
		*ct++=*cs++;
	}
	*ct=0;
	return(gfxedit_strdup(tbuf));
}

int gfxedit_tolower(int ch)
{
	if((ch>='A') && (ch<='Z'))
		return(ch+('a'-'A'));
	return(ch);
}

int gfxedit_stricmp(char *s1, char *s2)
{
	int c0, c1;

	c0=gfxedit_tolower(*s1); c1=gfxedit_tolower(*s2);
	while(c0 && (c0==c1))
		{ s1++; s2++; c0=gfxedit_tolower(*s1); c1=gfxedit_tolower(*s2); }
	return(c0-c1);
}

int gfxedit_strisext(char *s1, char *s2)
{
	int l1, l2;

	l1=strlen(s1);
	l2=strlen(s2);
	if(l2>=l1)
		return(0);
	return(!gfxedit_stricmp(s1+(l1-l2), s2));
}

char *gfxedit_emitutf(char *ct, int val)
{
	if(val<=0)
	{
		*ct++=0xC0;
		*ct++=0x80;
		return(ct);
	}

	if(val<0x80)
	{
		*ct++=val;
		return(ct);
	}

	if(val<0x0800)
	{
		*ct++=0xC0|((val>>6)&31);
		*ct++=0x80|((val>>0)&63);
		return(ct);
	}

	if(val<0x10000)
	{
		*ct++=0xE0|((val>>12)&15);
		*ct++=0x80|((val>> 6)&63);
		*ct++=0x80|((val>> 0)&63);
		return(ct);
	}

	*ct++=0xF0|((val>>18)& 7);
	*ct++=0x80|((val>>12)&63);
	*ct++=0x80|((val>> 6)&63);
	*ct++=0x80|((val>> 0)&63);
	return(ct);
}

int gfxedit_readutf(char **rcs)
{
	byte *cs;
	int c;
	
	cs=(byte *)(*rcs);
	c=*cs;
	
	if(c<0x80)
	{
		*rcs=(char *)(cs+1);
		return(c);
	}
	
	if(c<0xC0)
	{
		*rcs=(char *)(cs+1);
		return(c);
	}
	
	if(c<0xE0)
	{
		c=((cs[0]&0x1F)<<6)|((cs[1]&63)<<0);
		*rcs=(char *)(cs+2);
		return(c);
	}

	if(c<0xF0)
	{
		c=((cs[0]&0x0F)<<12)|((cs[1]&63)<<6)|((cs[2]&63)<<0);
		*rcs=(char *)(cs+3);
		return(c);
	}

	if(c<0xF8)
	{
		c=((cs[0]&0x07)<<18)|((cs[1]&63)<<12)|((cs[2]&63)<<6)|((cs[3]&63)<<0);
		*rcs=(char *)(cs+4);
		return(c);
	}
	
	return(0);
}

char *gfxedit_strdup_wcs2utf(wchar_t *wstr)
{
	char tbuf[512];
	char *ct;
	wchar_t *wcs;
	
	wcs=wstr; ct=tbuf;
	while(*wcs)
		{ ct=gfxedit_emitutf(ct, *wcs++); }
	*ct=0;
	return(gfxedit_strdup(tbuf));
}

GfxEdit_ConCmd *GfxEdit_AllocConCmd()
{
	GfxEdit_ConCmd *cmd;
	
	cmd=malloc(sizeof(GfxEdit_ConCmd));
	memset(cmd, 0, sizeof(GfxEdit_ConCmd));
	return(cmd);
}

GfxEdit_ConVar *GfxEdit_AllocConVar()
{
	GfxEdit_ConVar *cmd;
	
	cmd=malloc(sizeof(GfxEdit_ConVar));
	memset(cmd, 0, sizeof(GfxEdit_ConVar));
	return(cmd);
}

GfxEdit_ConCmd *GfxEdit_RegisterCommand(char *name, char *hint,
	void (*func)(GfxEdit_Context *ctx, char **args))
{
	GfxEdit_ConCmd *cmd;
	
	cmd=GfxEdit_AllocConCmd();
	cmd->name=gfxedit_strdup(name);
	cmd->hint=gfxedit_strdup(hint);
	cmd->Func=func;
	
	cmd->next=gfxedit_concmds;
	gfxedit_concmds=cmd;
	return(cmd);
}

GfxEdit_ConCmd *GfxEdit_LookupCommand(char *name)
{
	GfxEdit_ConCmd *cmd;
	
	cmd=gfxedit_concmds;
	while(cmd)
	{
		if(!strcmp(cmd->name, name))
			return(cmd);
		cmd=cmd->next;
	}
	return(NULL);
}

GfxEdit_ConVar *GfxEdit_LookupConVar(char *name)
{
	GfxEdit_ConVar *cmd;
	
	cmd=gfxedit_convars;
	while(cmd)
	{
		if(!strcmp(cmd->name, name))
			return(cmd);
		cmd=cmd->next;
	}
	return(NULL);
}

GfxEdit_ConVar *GfxEdit_GetConVar(char *name, char *initval)
{
	GfxEdit_ConVar *cvar;
	
	cvar=GfxEdit_LookupConVar(name);
	if(cvar)
		return(cvar);
	
	if(!initval)
		initval="";
	
	cvar=GfxEdit_AllocConVar();
	cvar->name=gfxedit_strdup(name);
	cvar->svalue=gfxedit_strdup(initval);
	cvar->dvalue=atof(cvar->svalue);
	
	cvar->next=gfxedit_convars;
	gfxedit_convars=cvar;
	return(cvar);
}

void GfxEdit_ConScrollUp(GfxEdit_Context *ctx)
{
	memmove(ctx->conbuf, ctx->conbuf+40, 24*40);
	memset(ctx->conbuf+24*40, 0, 40);
}

void GfxEdit_ConPutc(GfxEdit_Context *ctx, int ch)
{
	int p;

	if(ch=='\r')
	{
		ctx->con_prn_pos=0;
		return;
	}

	if(ch=='\n')
	{
		GfxEdit_ConScrollUp(ctx);
		ctx->con_prn_pos=0;
		return;
	}

	if(ch=='\b')
	{
		if(ctx->con_prn_pos>0)
			ctx->con_prn_pos--;
		return;
	}
	
	p=ctx->con_prn_pos;
	ctx->conbuf[24*40+p]=ch;
	ctx->con_prn_pos=p+1;
	if(p>=39)
	{
		GfxEdit_ConScrollUp(ctx);
		ctx->con_prn_pos=0;
	}
}

void GfxEdit_ConPuts(GfxEdit_Context *ctx, char *str)
{
	char *cs;
	int p;
	
	fputs(str, stdout);
	
	cs=str;
	while(*cs)
		{ GfxEdit_ConPutc(ctx, *cs++); }
}

void GfxEdit_ConPrintf(GfxEdit_Context *ctx, char *str, ...)
{
	char tbuf[512];
	va_list lst;

	va_start(lst, str);
	vsprintf(tbuf, str, lst);
	va_end(lst);
	GfxEdit_ConPuts(ctx, tbuf);
}

void GfxEdit_ConRunCmd(GfxEdit_Context *ctx, char *str)
{
	GfxEdit_ConCmd *cmd;
	GfxEdit_ConVar *cvar;
	char **a;

	a=gfxedit_split(str);
	if(!a[0])
		return;

	if(a[0][strlen(a[0])-1]==':')
		return;

	cmd=GfxEdit_LookupCommand(a[0]);
	if(!cmd)
	{
		cvar=GfxEdit_LookupConVar(a[0]);
		if(cvar)
		{
			if(a[1])
			{
				cvar->svalue=gfxedit_strdup(a[1]);
				cvar->dvalue=atof(cvar->svalue);
			}else
			{
				GfxEdit_ConPrintf(ctx, " %s '%s'\n", a[0], cvar->svalue);
			}
			return;
		}
		
		GfxEdit_ConPrintf(ctx, "Unhandled command '%s'\n", a[0]);
		return;
	}
	
	cmd->Func(ctx, a);
}

char *GfxEdit_ConLookupBufferLabel(GfxEdit_Context *ctx,
	char *scrbuf, char *lbl)
{
	char *cs, *ct;
	int n;
	
	n=strlen(lbl);
	cs=scrbuf;
	while(*cs)
	{
		while(*cs && (*cs<=' '))
			cs++;
		if(!strncmp(cs, lbl, n) && (cs[n]==':'))
			return(cs+n+1);
		while(*cs && (*cs!='\r') && (*cs!='\n'))
			cs++;
		if(*cs=='\r')	cs++;
		if(*cs=='\n')	cs++;
	}
	return(NULL);
}

void GfxEdit_ConRunBuffer(GfxEdit_Context *ctx, char *scrbuf)
{
	char tbuf[256];
	char *cs, *ct, *cs1;
	
	cs=scrbuf;
	while(*cs)
	{
		while(*cs && (*cs<=' '))
			cs++;
		ct=tbuf;
		while(*cs && (*cs!='\r') && (*cs!='\n'))
			*ct++=*cs++;
		*ct=0;
		if(*cs=='\r')	cs++;
		if(*cs=='\n')	cs++;
		if(tbuf[0]=='#')
			continue;
		if(tbuf[0]=='/')
			continue;

		GfxEdit_ConRunCmd(ctx, tbuf);
		if(ctx->exec_goto)
		{
			cs1=GfxEdit_ConLookupBufferLabel(ctx, scrbuf, ctx->exec_goto);
			if(cs1)
				{ cs=cs1; continue; }
		}
	}
}

byte *GfxEdit_LoadFile(GfxEdit_Context *ctx, char *name, int *rsz)
{
	FILE *fd;
	char *name1;
	byte *buf;
	int sz;
	
	fd=NULL;
	
	if(ctx)
	{
		if(!ctx->cwd)
			ctx->cwd=gfxedit_getcwd();
		name1=gfxedit_combinepath(ctx->cwd, name);
		fd=fopen(name1, "rb");
	}

	if(!fd)
		fd=fopen(name, "rb");
	if(!fd)
	{
		return(NULL);
	}
	
	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	buf=malloc(sz+16);
	memset(buf, 0, sz+16);
	fread(buf, 1, sz, fd);
	fclose(fd);
	
	if(rsz)
		*rsz=sz;
	return(buf);
}

void GfxEdit_StoreFile(GfxEdit_Context *ctx, char *name, byte *buf, int sz)
{
	FILE *fd;
	char *name1;

	if(!ctx->cwd)
		ctx->cwd=gfxedit_getcwd();
	
	name1=gfxedit_combinepath(ctx->cwd, name);

	fd=fopen(name1, "wb");
	if(!fd)
		return;
	fwrite(buf, 1, sz, fd);
	fclose(fd);
}

int GfxEdit_ColorDistanceRgb32(u32 clr0, u32 clr1)
{
	int cr0, cg0, cb0, ca0, cr1, cg1, cb1, ca1, cy0, cy1;
	int dr, dg, db, da, dy, d;
	if(clr0==clr1)
		return(0);
	cb0=(clr0>> 0)&255;	cg0=(clr0>> 8)&255;
	cr0=(clr0>>16)&255;	ca0=(clr0>>24)&255;
	cb1=(clr1>> 0)&255;	cg1=(clr1>> 8)&255;
	cr1=(clr1>>16)&255;	ca1=(clr1>>24)&255;
	cy0=(2*cg0+cr0+cb0)/4;
	cy1=(2*cg1+cr1+cb1)/4;
	
	dr=cr0-cr1;
	dg=cg0-cg1;
	db=cb0-cb1;
	da=ca0-ca1;
	dy=cy0-cy1;
	
	d=dr*dr+dg*dg+db*db+2*dy*dy+8*da*da;
	return(d);
}

int gfxedit_log2up(int val)
{
	int v, e;
	v=val; e=0;
	if(v<0)		v=-v;
	while(v>1)
		{ v=(v+1)>>1; e++; }
	return(e);
}

int gfxedit_log2dn(int val)
{
	int v, e;
	v=val; e=0;
	if(v<0)		v=-v;
	while(v>1)
		{ v=v>>1; e++; }
	return(e);
}

int gfxedit_isqrtapx(int val)
{
	float f;
	u32 fi;
	int e, v1;

	if(val<0)
	{
		return(-gfxedit_isqrtapx(-val));
	}

#if 1
	f=val;
//	memcpy(&fi, &f, 4);
	fi=*(u32 *)(&f);
	fi=(fi>>1)+0x1FC00000U;
//	memcpy(&f, &fi, 4);
	*(u32 *)(&f)=fi;
	v1=f;
	return(v1);
#endif

#if 1
	int i;
	i=gfxedit_log2up(val);
//	if((i&1) && (i>3))
//		return((val-(1<<(i-2)))>>(i>>1));
	return(val>>(i>>1));

//	int i;
//	i=gfxedit_log2up(val);
//	return(val>>(i/2));
#endif
}

void GfxEdit_BuildPalRemap(GfxEdit_Context *ctx,
	u32 *oldpal, u32 *newpal, int oldbpp, int newbpp,
	byte *remap, byte *reld)
{
	int bi, bd, bi1, bd1, d, d1, d2;
	int x, y, xs, ys;
	int i, j, k, l, n, nc1;
	
	nc1=1<<ctx->canvas_bpp;
	for(i=0; i<256; i++)
	{
		bi=0; bd=99999999;
		for(j=0; j<nc1; j++)
		{
			d=GfxEdit_ColorDistanceRgb32(oldpal[i], newpal[j]);
			if(d<bd)
				{	bi1=bi; bd1=bd; bi=j; bd=d; }
			else if(d<bd1)
				{	bi1=j; bd1=d; }
		}
		
		if((bd*4)<bd1)
			bi1=bi;
		
		if(	((oldpal[i]>>24)==(newpal[bi ]>>24)) &&
			((oldpal[i]>>24)!=(newpal[bi1]>>24)))
				bi1=bi;
		if(	((oldpal[i]>>24)==(ctx->canvas_pal4[bi1]>>24)) &&
			((oldpal[i]>>24)!=(ctx->canvas_pal4[bi ]>>24)))
				bi=bi1;
		
		if(	!(oldpal[i]>>24) &&
			((oldpal[i]>>24)!=(ctx->canvas_pal4[bi ]>>24)) )
		{
			if(nc1<256)
				{ bi=nc1; bi1=nc1; }
		}
		
		
		remap[  0+i]=bi;
		remap[256+i]=bi1;
		
		if(reld)
		{
			reld[i]=0;
			if(bd1>0)
			{
				d1=gfxedit_isqrtapx(bd);
				d2=gfxedit_isqrtapx(bd1);
				reld[i]=(128*d1)/d2;
			}
		}
		
		if((ctx->canvas_bpp==1) &&
			!(ctx->canvas_pal4[0]&0xFFFFFF))
		{
			reld[i]=GfxEdit_RGBA32toLuma(oldpal[i]);
			remap[  0+i]=0;
			remap[256+i]=1;
		}
	}
}

void GfxEdit_ConRun_DoConvPal(GfxEdit_Context *ctx, u32 *oldpal, int dithfl)
{
	static byte bayer4[16]=
		{  0,  8,  2, 10,
		  12,  4, 14,  6,
		   3, 11,  1,  9,
		  15,  7, 13,  5 };
	static byte bayer8[64]=
		{  0, 32,  8, 40,  2, 34, 10, 42,
		  48, 16, 56, 24, 50, 18, 58, 26,
		  12, 44,  4, 36, 14, 46,  6, 38,
		  60, 28, 52, 20, 62, 30, 54, 22,
		   3, 35, 11, 43,  1, 33,  9, 41,
		  51, 19, 59, 27, 49, 17, 57, 25,
		  15, 47,  7, 39, 13, 45,  5, 37,
		  63, 31, 55, 23, 61, 29, 53, 21 };
	static byte remap[512];
	static byte reld[256];
	byte *pixels;
	int bi, bd, bi1, bd1, d, d1, d2;
	int x, y, xs, ys;
	int i, j, k, l, n, nc1;
	
	if((dithfl&7)==7)
	{
		if(ctx->canvas_bpp==8)
			return;
	}
	
	GfxEdit_BuildPalRemap(ctx,
		oldpal, ctx->canvas_pal4,
		8, ctx->canvas_bpp,
		remap, reld);

	nc1=1<<ctx->canvas_bpp;
	
	if((dithfl&7)==0)
	{
		n=ctx->canvas_width*ctx->canvas_height;
		pixels=ctx->canvas_pixels;
		for(i=0; i<n; i++)
		{
			j=pixels[i];
			k=remap[j];
			pixels[i]=k;
		}
		
		for(l=0; l<ctx->layer_max; l++)
		{
			pixels=ctx->layer[l]->pixels;
			for(i=0; i<n; i++)
			{
				j=pixels[i];
				k=remap[j];
				pixels[i]=k;
			}
		}
		
		return;
	}

	if((dithfl&7)==7)
	{
		n=ctx->canvas_width*ctx->canvas_height;
		for(i=0; i<n; i++)
		{
			j=ctx->canvas_pixels[i];
			if(j>nc1)
				k=remap[j];
			else
				k=j;
			ctx->canvas_pixels[i]=k;
		}
		
		for(l=0; l<ctx->layer_max; l++)
		{
			pixels=ctx->layer[l]->pixels;
			for(i=0; i<n; i++)
			{
				j=pixels[i];
				if(j>nc1)	k=remap[j];
				else		k=j;
				pixels[i]=k;
			}
		}
		return;
	}

	if((dithfl&7)==1)
	{
		xs=ctx->canvas_width;
		ys=ctx->canvas_height;
		for(y=0; y<ys; y++)
			for(x=0; x<xs; x++)
		{
			i=y*xs+x;
			j=ctx->canvas_pixels[i];
			if((x^y)&1)
				k=remap[j+256];
			else
				k=remap[j];
			ctx->canvas_pixels[i]=k;
		}
		
		for(l=0; l<ctx->layer_max; l++)
		{
			pixels=ctx->layer[l]->pixels;
			for(y=0; y<ys; y++)
				for(x=0; x<xs; x++)
			{
				i=y*xs+x;
				j=pixels[i];
				if((x^y)&1)
					k=remap[j+256];
				else
					k=remap[j];
				pixels[i]=k;
			}
		}
		return;
	}

	if((dithfl&7)==2)
	{
		xs=ctx->canvas_width;
		ys=ctx->canvas_height;
		for(y=0; y<ys; y++)
			for(x=0; x<xs; x++)
		{
			i=y*xs+x;
			j=ctx->canvas_pixels[i];
			
			bd=bayer4[(y&3)*4+(x&3)];
			d=reld[j];
			
			if(d>(bd*16))
				k=remap[j+256];
			else
				k=remap[j];
			ctx->canvas_pixels[i]=k;
		}
		
		for(l=0; l<ctx->layer_max; l++)
		{
			pixels=ctx->layer[l]->pixels;
			for(y=0; y<ys; y++)
				for(x=0; x<xs; x++)
			{
				i=y*xs+x;
				j=pixels[i];
				bd=bayer4[(y&3)*4+(x&3)];
				d=reld[j];
				
				if(d>(bd*16))
					k=remap[j+256];
				else
					k=remap[j];
				pixels[i]=k;
			}
		}
		return;
	}

	if((dithfl&7)==3)
	{
		xs=ctx->canvas_width;
		ys=ctx->canvas_height;
		for(y=0; y<ys; y++)
			for(x=0; x<xs; x++)
		{
			i=y*xs+x;
			j=ctx->canvas_pixels[i];
			bd=bayer8[(y&7)*8+(x&7)];
			d=reld[j];
			if(d>(bd*4))
				k=remap[j+256];
			else
				k=remap[j];
			ctx->canvas_pixels[i]=k;
		}
		
		for(l=0; l<ctx->layer_max; l++)
		{
			pixels=ctx->layer[l]->pixels;
			for(y=0; y<ys; y++)
				for(x=0; x<xs; x++)
			{
				i=y*xs+x;
				j=pixels[i];
				bd=bayer8[(y&7)*8+(x&7)];
				d=reld[j];
				if(d>(bd*4))
					k=remap[j+256];
				else
					k=remap[j];
				pixels[i]=k;
			}
		}
		return;
	}
}

void GfxEdit_ConRun_LoadPal(GfxEdit_Context *ctx, char **args)
{
	byte *buf;
	byte *cs, *cs1;
	u32 orgb[256];
	u32 rgb[384];
	int isconv;
	int sz, n, dith, psub;
	int i;

	isconv=!gfxedit_stricmp(args[0], "convpal");
	dith=0;

	if(!args[1])
	{
		if(isconv)
		{
			GfxEdit_ConPrintf(ctx, "%s <palette> [<dither>]\n", args[0]);
		}else
		{
			GfxEdit_ConPrintf(ctx, "%s <palette>\n", args[0]);
		}
		return;
	}

	if(args[2])
	{
		if(!gfxedit_stricmp(args[2], "direct"))		dith=0;
		if(!gfxedit_stricmp(args[2], "bayer2"))		dith=1;
		if(!gfxedit_stricmp(args[2], "bayer4"))		dith=2;
		if(!gfxedit_stricmp(args[2], "bayer8"))		dith=3;
	}
	
	memcpy(orgb, ctx->canvas_pal4, 256*4);
	GfxEdit_MarkUndoPal(ctx);

	if(!gfxedit_stricmp(args[1], "rgbi"))
	{
		GfxEdit_SetupPalRGBI(ctx);
		if(isconv)
			GfxEdit_ConRun_DoConvPal(ctx, orgb, dith);
		else
			GfxEdit_ConRun_DoConvPal(ctx, orgb, 7);
		GfxEdit_MarkRedoPal(ctx);
		return;
	}

	if(!gfxedit_stricmp(args[1], "rgbi_x2"))
	{
		GfxEdit_SetupPalRGBIx2(ctx);
		if(isconv)
			GfxEdit_ConRun_DoConvPal(ctx, orgb, dith);
		else
			GfxEdit_ConRun_DoConvPal(ctx, orgb, 7);
		GfxEdit_MarkRedoPal(ctx);
		return;
	}

	if(!gfxedit_stricmp(args[1], "web216"))
	{
		GfxEdit_SetupPalWeb216(ctx);
		if(isconv)
			GfxEdit_ConRun_DoConvPal(ctx, orgb, dith);
		else
			GfxEdit_ConRun_DoConvPal(ctx, orgb, 7);
		GfxEdit_MarkRedoPal(ctx);
		return;
	}

	if(!gfxedit_stricmp(args[1], "grad16"))
	{
		GfxEdit_BmpFillPalGradient((byte *)(ctx->canvas_pal4));
		ctx->canvas_bpp=8;
		GfxEdit_SetupPalFinish(ctx);
		if(isconv)
			GfxEdit_ConRun_DoConvPal(ctx, orgb, dith);
		else
			GfxEdit_ConRun_DoConvPal(ctx, orgb, 7);
		GfxEdit_MarkRedoPal(ctx);
		return;
	}

	if(	!gfxedit_stricmp(args[1], "mono") ||
		!gfxedit_stricmp(args[1], "monochrome"))
	{
		GfxEdit_SetupPalMono(ctx);
		if(isconv)
			GfxEdit_ConRun_DoConvPal(ctx, orgb, dith);
		else
			GfxEdit_ConRun_DoConvPal(ctx, orgb, 7);
		GfxEdit_MarkRedoPal(ctx);
		return;
	}

	if(!gfxedit_stricmp(args[1], "cga2b") ||
		!gfxedit_stricmp(args[1], "cga2b_rg") ||
		!gfxedit_stricmp(args[1], "cga2b_rgy") ||
		!gfxedit_stricmp(args[1], "cga2b_cr") ||
		!gfxedit_stricmp(args[1], "cga2b_rc"))
	{
		psub=0;
		if(!gfxedit_stricmp(args[1], "cga2b_rg"))
			psub=1;
		if(!gfxedit_stricmp(args[1], "cga2b_rgy"))
			psub=2;
		if(	!gfxedit_stricmp(args[1], "cga2b_cr") ||
			!gfxedit_stricmp(args[1], "cga2b_rc"))
			psub=3;
	
		GfxEdit_SetupPalCga2b(ctx, psub);
		if(isconv)
			GfxEdit_ConRun_DoConvPal(ctx, orgb, dith);
		else
			GfxEdit_ConRun_DoConvPal(ctx, orgb, 7);
		GfxEdit_MarkRedoPal(ctx);
		return;
	}

	buf=GfxEdit_LoadFile(ctx, args[1], &sz);
	if(!buf)
	{
		GfxEdit_ConPrintf(ctx, "loadpal failed load '%s'\n", args[1]);
		return;
	}
	
	cs=buf; n=0;
	while(*cs)
	{
		while(*cs && *cs<=' ')
			cs++;
		if(*cs=='/')
		{
			while(*cs && (*cs!='\r') && (*cs!='\n'))
				cs++;
			if(*cs=='\r')	cs++;
			if(*cs=='\n')	cs++;
			continue;
		}

		if(*cs=='#')
			cs++;
		cs1=NULL;
		rgb[n]=strtoll(cs, &cs1, 16);
		if(cs1==(cs+6))
			rgb[n]|=0xFF000000U;
		n++;
		
		if(n>=258)
			break;
		while(*cs && (*cs!='\r') && (*cs!='\n'))
			cs++;
		if(*cs=='\r')	cs++;
		if(*cs=='\n')	cs++;
	}
	
	if(n>256)
		n=256;
	for(i=0; i<n; i++)
	{
//		ctx->canvas_pal4[i]=rgb[i]|0xFF000000U;
		ctx->canvas_pal4[i]=rgb[i];
	}
	
	ctx->palname=gfxedit_strdup(args[1]);
	
	ctx->canvas_bpp=1;
	if(n>2)		ctx->canvas_bpp=2;
	if(n>4)		ctx->canvas_bpp=3;
	if(n>8)		ctx->canvas_bpp=4;
	if(n>16)	ctx->canvas_bpp=5;
	if(n>32)	ctx->canvas_bpp=8;
	
	GfxEdit_SetupPalFinish(ctx);
	if(isconv)
		GfxEdit_ConRun_DoConvPal(ctx, orgb, dith);
	else
		GfxEdit_ConRun_DoConvPal(ctx, orgb, 7);
	GfxEdit_MarkRedoPal(ctx);
}

void GfxEdit_ConRun_SavePal(GfxEdit_Context *ctx, char **args)
{
	char *palname;
	byte *buf;
	byte *cs, *ct;
	u32 rgb[384];
	int sz, n;
	int i;

	palname=ctx->palname;
	if(args[1])
		palname=args[1];

	if(!palname)
	{
		GfxEdit_ConPrintf(ctx, "savepal <palette>\n");
		return;
	}

	buf=malloc(3172);
	ct=buf;
	
	n=1<<ctx->canvas_bpp;
	for(i=0; i<n; i++)
	{
		sprintf(ct, "%06X\n", ctx->canvas_pal4[i]&0xFFFFFF);
		ct+=strlen(ct);
	}

	GfxEdit_StoreFile(ctx, palname, buf, ct-buf);
	free(buf);

//	buf=GfxEdit_LoadFile(args[1], &sz);
}
	
void GfxEdit_ConRun_Help(GfxEdit_Context *ctx, char **args)
{
	GfxEdit_ConCmd *cmd;

	if(!args[1])
	{
		cmd=gfxedit_concmds;
		while(cmd)
		{
			GfxEdit_ConPrintf(ctx, "%-12s %s\n",
				cmd->name, cmd->hint);
			cmd=cmd->next;
		}
		return;
	}
}

void GfxEdit_ConRun_Save(GfxEdit_Context *ctx, char **args)
{
	char *imgname;
	byte *buf, *pixels;
	byte *cs, *ct;
	u32 rgb[384];
	int ext_ispcx, ext_isgel;
	int ext_isbmp, ext_isdib;
	int sz, n, islayer;
	int i;

	islayer=!gfxedit_stricmp(args[0], "savelayer");

	imgname=ctx->imgname;
	if(islayer)
		imgname=NULL;

	if(args && args[1])
		imgname=args[1];

	if(!imgname)
	{
		if(args)
			GfxEdit_ConPrintf(ctx, "%s <image>\n", args[0]);
		return;
	}

	ext_ispcx=!gfxedit_stricmp(imgname+(strlen(imgname)-4), ".pcx");
	ext_isbmp=!gfxedit_stricmp(imgname+(strlen(imgname)-4), ".bmp");
	ext_isdib=!gfxedit_stricmp(imgname+(strlen(imgname)-4), ".dib");
	ext_isgel=!gfxedit_stricmp(imgname+(strlen(imgname)-4), ".gel");

	if(ext_isgel && !islayer)
	{
		buf=NULL; sz=0;
		GfxEdit_FlattenContextGELI(ctx, &buf, &sz);
	}

	if(ext_ispcx)
	{
		pixels=ctx->canvas_pixels;
		if(islayer)
			pixels=ctx->layer_pixels;
	
		sz=0;
		buf=malloc(1536+(ctx->canvas_width+3)*ctx->canvas_height);
		sz=GfxEdit_EncodeImagePCX8(buf, pixels,
			ctx->canvas_width, ctx->canvas_height,
			ctx->canvas_pal4, 8);
	}

	if(ext_isbmp || ext_isdib)
	{
		pixels=ctx->canvas_pixels;
		if(islayer)
			pixels=ctx->layer_pixels;
	
		sz=0;
		buf=malloc(1536+(ctx->canvas_width+3)*ctx->canvas_height);

		if(ctx->canvas_bpp==1)
		{
			sz=GfxEdit_EncodeImageBMP1I(buf, pixels,
				ctx->canvas_width, ctx->canvas_height,
				ctx->canvas_pal4, 8);
		}else
			if(ctx->canvas_bpp==2)
		{
			sz=GfxEdit_EncodeImageBMP2I(buf, pixels,
				ctx->canvas_width, ctx->canvas_height,
				ctx->canvas_pal4, 8);
		}else
			if((ctx->canvas_bpp>=2) && (ctx->canvas_bpp<=4))
		{
			memset(rgb, 0, 256*4);
			memcpy(rgb, ctx->canvas_pal4, (1<<ctx->canvas_bpp)*4);
			sz=GfxEdit_EncodeImageBMP4I(buf, pixels,
				ctx->canvas_width, ctx->canvas_height, rgb, 8);
		}else
			if((ctx->canvas_bpp>=5) && (ctx->canvas_bpp<=8))
		{
			memset(rgb, 0, 256*4);
			memcpy(rgb, ctx->canvas_pal4, (1<<ctx->canvas_bpp)*4);
			sz=GfxEdit_EncodeImageBMP8I(buf, pixels,
				ctx->canvas_width, ctx->canvas_height, rgb, 8);
		}
	}

	if(sz<=0)
		return;

	GfxEdit_StoreFile(ctx, imgname, buf, sz);
	free(buf);
}

void GfxEdit_ConRun_Load(GfxEdit_Context *ctx, char **args)
{
	char *imgname;
	byte *buf, *ibuf, *lpix;
	byte *cs, *ct;
	byte remap[512];
	u32 rgb[384];
	int sz, n, xs, ys, islayer, bpp;
	int x, y, c, cxs, cys, mxs, mys;
	int i;
	
	islayer=!gfxedit_stricmp(args[0], "loadlayer");

	imgname=ctx->imgname;
	if(islayer)
		imgname=NULL;

	if(args && args[1])
		imgname=args[1];

	if(!imgname)
	{
		if(args)
			GfxEdit_ConPrintf(ctx, "%s <image>\n", args[0]);
		return;
	}

	buf=GfxEdit_LoadFile(ctx, imgname, &sz);
	if(!buf)
	{
		GfxEdit_ConPrintf(ctx, "load failed load '%s'\n", imgname);
		return;
	}
	
	if(!islayer)
	{
		if(*(u32 *)buf==GFXEDIT_FCC_GELI)
		{
			GfxEdit_LoadContextGELI(ctx, buf, sz);
			return;
		}
	}
	
	memset(rgb, 0, 256*4);
	ibuf=GfxEdit_DecodeBMP8(buf, rgb, &xs, &ys, 8);
	if(!ibuf)
		ibuf=GfxEdit_DecodePCX(buf, rgb, &xs, &ys, 8);
	if(!ibuf)
		ibuf=GfxEdit_DecodeTGA(buf, rgb, &xs, &ys, 8);
	if(!ibuf)
	{
		free(buf);
		GfxEdit_ConPrintf(ctx, "load failed decode '%s'\n", imgname);
		return;
	}

	bpp=8;
	if(!rgb[32] && !rgb[33])
	{
		bpp=5;
		if(!rgb[16] && !rgb[17])
		{
			bpp=4;
			if(!rgb[4] && !rgb[5])
			{
				bpp=2;
				if(!rgb[2] && !rgb[3])
					bpp=1;
			}
		}
	}

	if(islayer)
	{
		GfxEdit_BuildPalRemap(ctx,
			rgb, ctx->canvas_pal4,
			bpp, ctx->canvas_bpp,
			remap, NULL);

		cxs=ctx->canvas_width;
		cys=ctx->canvas_height;
		
		lpix=ctx->layer_pixels;
		
		mxs=cxs;		mys=cys;
		if(xs<mxs)		mxs=xs;
		if(ys<mys)		mys=ys;
		
		for(y=0; y<mys; y++)
		{
			if(mxs!=cxs)
				memset(lpix+y*cxs, ctx->clr_trans, cxs);
//			memcpy(lpix+y*cxs, ibuf+y*xs, mxs);
			for(x=0; x<mxs; x++)
			{
				c=ibuf[y*xs+x];
				if((x^y)&1)
					c=remap[256+c];
				else
					c=remap[  0+c];
				lpix[y*cxs+x]=c;
			}
		}
		
		free(ibuf);
		free(buf);
		return;
	}

	if(!islayer)
	{
		if(ctx->canvas_pixels)
			free(ctx->canvas_pixels);
		ctx->canvas_pixels=ibuf;
		ctx->canvas_width=xs;
		ctx->canvas_height=ys;
		
		ctx->layer_pixels=ibuf;
		ctx->layer_max=0;
		
		ctx->canvas_bpp=bpp;
		memcpy(ctx->canvas_pal4, rgb, (1<<ctx->canvas_bpp)*4);
		GfxEdit_SetupPalFinish(ctx);
		free(buf);
		return;
	}
}

void GfxEdit_ConRun_New(GfxEdit_Context *ctx, char **args)
{
	int xs, ys, clr;

	if(args[1] && args[2])
	{
		xs=atoi(args[1]);
		ys=atoi(args[2]);
	}else
		if(args[1])
	{
		xs=atoi(args[1]);
		ys=xs;
	}else
	{
		xs=64;
		ys=64;
	}
	
	if((xs!=ctx->canvas_width) || (ys!=ctx->canvas_height))
	{
		free(ctx->canvas_pixels);
		ctx->canvas_pixels=malloc(xs*ys);
		ctx->canvas_width=xs;
		ctx->canvas_height=ys;
		ctx->canvas_xorg=0;
		ctx->canvas_yorg=0;
	}

	ctx->layer_pixels=ctx->canvas_pixels;
	ctx->layer_max=0;

	clr=15;
	if(ctx->canvas_bpp<4)
		{ clr=1; }
	memset(ctx->canvas_pixels, clr, xs*ys);
}

void GfxEdit_ConRun_Resize(GfxEdit_Context *ctx, char **args)
{
	byte *newbuf, *oldbuf;
	int xs, ys, oxs, oys, cxs, cys, y;
	int i, j, k;

	if(args[1] && args[2])
	{
		xs=atoi(args[1]);
		ys=atoi(args[2]);
	}else
	{
		GfxEdit_ConPrintf(ctx, "resize <width> <height>\n");
		return;
	}
	
	if((xs==ctx->canvas_width) && (ys==ctx->canvas_height))
		return;

	newbuf=malloc(xs*ys);
	memset(newbuf, 15, xs*ys);

	oldbuf=ctx->canvas_pixels;
	oxs=ctx->canvas_width;
	oys=ctx->canvas_height;
	cxs=oxs;	cys=oys;
	if(xs<cxs)	cxs=xs;
	if(ys<cys)	cys=ys;

	for(y=0; y<cys; y++)
		{ memcpy(newbuf+y*xs, oldbuf+y*oxs, cxs); }

	free(ctx->canvas_pixels);
	ctx->canvas_pixels=newbuf;
	ctx->canvas_width=xs;
	ctx->canvas_height=ys;

	if(ctx->layer_max!=0)
	{
		for(i=0; i<ctx->layer_max; i++)
		{
			oldbuf=ctx->layer[i]->pixels;
			newbuf=malloc(xs*ys);
			memset(newbuf, ctx->clr_trans, xs*ys);

			for(y=0; y<cys; y++)
				{ memcpy(newbuf+y*xs, oldbuf+y*oxs, cxs); }
			free(oldbuf);
			ctx->layer[i]->pixels=newbuf;
		}

		ctx->layer_pixels=ctx->layer[ctx->layer_cur]->pixels;
	}else
	{
		ctx->layer_pixels=ctx->canvas_pixels;
	}
}

void GfxEdit_ConRun_ChDir(GfxEdit_Context *ctx, char **args)
{
	if(!ctx->cwd)
		ctx->cwd=gfxedit_getcwd();
	
	if(args[1])
	{
		ctx->cwd=gfxedit_chdir(ctx->cwd, args[1]);
		GfxEdit_ConPrintf(ctx, "CWD %s\n", ctx->cwd);
	}
}

void GfxEdit_ConRun_LsDir(GfxEdit_Context *ctx, char **args)
{
	void *dir;
	char *str1, *str2;

	if(!ctx->cwd)
		ctx->cwd=gfxedit_getcwd();
		
	dir=gfxedit_opendir(ctx->cwd);
	if(!dir)
		return;
	while(1)
	{
		str1=gfxedit_readdir(dir);
		if(!str1)break;
		
		if(strlen(str1)<=18)
		{
			str2=gfxedit_readdir(dir);
			if(!str2)
			{
				GfxEdit_ConPrintf(ctx, "%s\n", str1);
				break;
			}
			
			if(strlen(str2)<=18)
			{
				GfxEdit_ConPrintf(ctx, "%-18s %-18s\n", str1, str2);
			}else
			{
				GfxEdit_ConPrintf(ctx, "%s\n", str1);
				GfxEdit_ConPrintf(ctx, "%s\n", str2);
			}
		}else
		{
			GfxEdit_ConPrintf(ctx, "%s\n", str1);
		}
	}
	gfxedit_closedir(dir);
}

void GfxEdit_ConRun_Exec(GfxEdit_Context *ctx, char **args)
{
	char *imgname;
	byte *buf, *ibuf;
	byte *cs, *ct;
	u32 rgb[384];
	int sz, n, xs, ys;
	int i;
	
	imgname=NULL;
	if(args && args[1])
		imgname=args[1];

	if(!imgname)
	{
		if(args)
			GfxEdit_ConPrintf(ctx, "exec <script>\n");
		return;
	}

	buf=GfxEdit_LoadFile(ctx, imgname, &sz);
	if(!buf)
	{
		GfxEdit_ConPrintf(ctx, "load failed load '%s'\n", imgname);
		return;
	}
	
	GfxEdit_ConRunBuffer(ctx, buf);
}

void GfxEdit_ConRun_PalColor(GfxEdit_Context *ctx, char **args)
{
	char *cs, *cs1;
	u32 v;
	int ix;

	if(!args[1])
	{
		GfxEdit_ConPrintf(ctx, "%s <index> [<newcolor>]\n", args[0]);
		return;
	}
	
	ix=atoi(args[1]);
	if((ix<0) || (ix>=256))
		return;
	
	if(args[2])
	{
		cs=args[2]; cs1=NULL;
		if(*cs=='#')	cs++;
		v=strtoll(cs, &cs1, 16);
		if((cs+6)==cs1)
			v|=0xFF000000U;
		ctx->canvas_pal4[ix]=v;
		ctx->canvas_pal2[ix]=GfxEdit_RGB24to555(ctx->canvas_pal4[ix]);

		ctx->redraw_tools_dirty=1;
		ctx->redraw_img_dirty=1;
	}else
	{
		GfxEdit_ConPrintf(ctx, "%s %d=#%08X\n",
			args[0], ix, ctx->canvas_pal4[ix]);
	}
}

void GfxEdit_ConRun_DrawText(GfxEdit_Context *ctx, char **args)
{
	int xorg, yorg, cw, ch, clr;
	char *str;
	
//	xorg=ctx->line_x0;
//	yorg=ctx->line_y0;

	xorg=atoi(args[1]);
	yorg=atoi(args[2]);
	cw=atoi(args[3]);
	ch=atoi(args[4]);
	clr=ctx->sel_color1;
	str=args[5];
	
	GfxEdit_SdfDrawString(
		ctx->layer_pixels, ctx->canvas_width, ctx->canvas_height,
		xorg, yorg, cw, ch, str, clr);
}

void GfxEdit_ConRun_BenchRp2(GfxEdit_Context *ctx, char **args)
{
	byte *bpix, *cpix, *dpix;
	s64 tot;
	int t0, t1, t2, npix, csz, dsz;
	
	npix=ctx->canvas_width*ctx->canvas_height;
//	bpix=GfxEdit_GetLayerPixelsPred(ctx, 0);
//	bpix=GfxEdit_GetLayerPixels(ctx, 0);
	cpix=malloc(npix*2);
	dpix=malloc(npix*2);

	bpix=GfxEdit_GetLayerPixelsPred(ctx, 0);
	csz=GfxEdit_EncodeRP2(cpix, bpix, npix*2, npix);
	dsz=GfxEdit_DecodeRP2(dpix, cpix, npix*2, csz);
	
	GfxEdit_ConPrintf(ctx, "CSize1 %d\n", csz);

	if(dsz!=npix)
	{
		GfxEdit_ConPrintf(ctx, "Decode size mismatch 1\n");
		return;
	}

	if(memcmp(dpix, bpix, npix))
	{
		GfxEdit_ConPrintf(ctx, "Decode data mismatch 1\n");
		return;
	}

	bpix=GfxEdit_GetLayerPixels(ctx, 0);
	csz=GfxEdit_EncodeRP2(cpix, bpix, npix*2, npix);
	dsz=GfxEdit_DecodeRP2(dpix, cpix, npix*2, csz);
	
	GfxEdit_ConPrintf(ctx, "CSize2 %d\n", csz);

	if(dsz!=npix)
	{
		GfxEdit_ConPrintf(ctx, "Decode size mismatch 2\n");
		return;
	}

	if(memcmp(dpix, bpix, npix))
	{
		GfxEdit_ConPrintf(ctx, "Decode data mismatch 2\n");
		return;
	}
	
	GfxEdit_ConPrintf(ctx, "Starting Benchmark\n");
	t0=FRGL_TimeMS();
	t2=t0+10000;
	t1=t0;
	tot=0;
	while(t1<t2)
	{
		tot+=npix;
		csz=GfxEdit_EncodeRP2(cpix, bpix, npix*2, npix);
		t1=FRGL_TimeMS();
	}
	GfxEdit_ConPrintf(ctx, "Enc %fK/s\n", (tot>>10)/((t1-t0)/1000.0));


	t0=FRGL_TimeMS();
	t2=t0+10000;
	t1=t0;
	tot=0;
	while(t1<t2)
	{
		tot+=npix;
		GfxEdit_DecodeRP2(dpix, cpix, npix*2, csz);
		t1=FRGL_TimeMS();
	}
	GfxEdit_ConPrintf(ctx, "Dec %fK/s\n", (tot>>10)/((t1-t0)/1000.0));
}

void GfxEdit_InitConsole()
{
	if(gfxedit_concmds)
		return;
	GfxEdit_RegisterCommand("help",			"describes commmands",
		GfxEdit_ConRun_Help);
	GfxEdit_RegisterCommand("loadpal",		"load color palette",
		GfxEdit_ConRun_LoadPal);
	GfxEdit_RegisterCommand("savepal",		"save color palette",
		GfxEdit_ConRun_SavePal);
	GfxEdit_RegisterCommand("convpal",		"convert to new palette",
		GfxEdit_ConRun_LoadPal);
	GfxEdit_RegisterCommand("load",			"load image",
		GfxEdit_ConRun_Load);
	GfxEdit_RegisterCommand("save",			"save image",
		GfxEdit_ConRun_Save);
	GfxEdit_RegisterCommand("loadlayer",	"load layer as image",
		GfxEdit_ConRun_Load);
	GfxEdit_RegisterCommand("savelayer",	"save layer as image",
		GfxEdit_ConRun_Save);
	GfxEdit_RegisterCommand("palcolor",		"get/set color",
		GfxEdit_ConRun_PalColor);

	GfxEdit_RegisterCommand("new",			"new image",
		GfxEdit_ConRun_New);
	GfxEdit_RegisterCommand("resize",		"resize image",
		GfxEdit_ConRun_Resize);
	GfxEdit_RegisterCommand("exec",			"run command script",
		GfxEdit_ConRun_Exec);

	GfxEdit_RegisterCommand("drawtext",		"draw some text",
		GfxEdit_ConRun_DrawText);

	GfxEdit_RegisterCommand("cd", "change directory",
		GfxEdit_ConRun_ChDir);
	GfxEdit_RegisterCommand("ls", "list directory",
		GfxEdit_ConRun_LsDir);

	GfxEdit_RegisterCommand("benchrp2", "benchmark RP2 encoder",
		GfxEdit_ConRun_BenchRp2);
	
	GfxEdit_InitSdf();
}
