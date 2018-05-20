/*
Written 2007-2009 by Brendan G Bohannon
This code is re-assigned into the public domain.
*/

#include <bgbccc.h>

struct ar_hdr {
  char ar_name[16];		/* name of this member */
  char ar_date[12];		/* file mtime */
  char ar_uid[6];		/* owner uid; printed as decimal */
  char ar_gid[6];		/* owner gid; printed as decimal */
  char ar_mode[8];		/* file mode, printed as octal   */
  char ar_size[10];		/* file size, printed as decimal */
  char ar_fmag[2];		/* should contain ARFMAG */
};


int BGBCC_COFF_ResolveRelocs(BGBCC_Object *ctx, BGBCC_COFF_Info *inf, int sec,
	int offs, int cnt)
{
	byte *buf, *ip, *dp;
	char *s;
	int b, rv, ty;
	int i, j, k, l, i1;

//	printf("Obj1 %s\n", (ctx->flags&BGBCC_FL_X86_64)?"x64":"x86");

//	printf("BGBCC_COFF_ResolveRelocs(%d, %X, %d):\n", sec, offs, cnt);

	rv=0;
	buf=inf->buf;
	for(i=0; i<cnt; i++)
	{
		b=offs+i*10;
		j=buf[b+0]|(buf[b+1]<<8)|(buf[b+2]<<8)|(buf[b+3]<<8);
		k=buf[b+4]|(buf[b+5]<<8)|(buf[b+6]<<8)|(buf[b+7]<<8);
		l=buf[b+8]|(buf[b+9]<<8);

		s=inf->sym_name[k];

		if(!(ctx->flags&(BGBCC_FL_X86_64|BGBCC_FL_ARM)))
		{
			if(*s!='_')continue;
			s++;
		}

//		j=inf->sec_rva_off[sec]+j+4;
		j=inf->sec_rva_off[sec]+j;
		if(inf->sec_flag[sec]&0x20)j|=(1<<24);
			else j|=(2<<24);

		if(ctx->flags&BGBCC_FL_ARM)
		{
			if(l==0x00) { ty=BGBCC_JMP_ABS; j+=8; }
			if(l==0x01) { ty=BGBCC_JMP_ABS32; j+=4; }
			if(l==0x02) { ty=BGBCC_JMP_ABS32NB; j+=4; }
			if(l==0x03) { ty=BGBCC_JMP_ARM_NEAR24; j+=4; }
			if(l==0x04) { ty=BGBCC_JMP_THUMB_NEAR22; j+=4; }

			if(l==0x0E) { ty=BGBCC_JMP_SECTION; j+=4; }
			if(l==0x0F) { ty=BGBCC_JMP_SECREL; j+=4; }
		}else if(ctx->flags&BGBCC_FL_X86_64)
		{
			if(l==0x00) { ty=BGBCC_JMP_ABS; j+=8; }
			if(l==0x01) { ty=BGBCC_JMP_ABS64; j+=8; }
			if(l==0x02) { ty=BGBCC_JMP_ABS32; j+=4; }
			if(l==0x03) { ty=BGBCC_JMP_ABS32NB; j+=4; }
			if(l==0x04) { ty=BGBCC_JMP_NEAR32; j+=4; }

			if(	(l==0x05) || (l==0x06) ||
				(l==0x07) || (l==0x08) ||
				(l==0x09))
			{
				ip=NULL;
				if((j>>24)==1)ip=ctx->text+(j&0xFFFFFF);
				if((j>>24)==2)ip=ctx->data+(j&0xFFFFFF);

				i1=ip[0]|(ip[1]<<8)|(ip[2]<<16)|(ip[3]<<24);
				i1-=(l-0x04);
				ip[0]=i1&0xFF;		ip[1]=(i1>>8)&0xFF;
				ip[2]=(i1>>16)&0xFF;	ip[3]=(i1>>24)&0xFF;

				ty=BGBCC_JMP_NEAR32; j+=4;
			}

			if(l==0x0A) { ty=BGBCC_JMP_SECTION; j+=4; }
			if(l==0x0B) { ty=BGBCC_JMP_SECREL; j+=4; }

		}else
		{
			if(l==0x00) { ty=BGBCC_JMP_ABS; j+=4; }
			if(l==0x01) { ty=BGBCC_JMP_ABS16; j+=2; }
			if(l==0x02) { ty=BGBCC_JMP_NEAR16; j+=2; }
			//3/4/5 undefined
			if(l==0x06) { ty=BGBCC_JMP_ABS32; j+=4; }
			if(l==0x07) { ty=BGBCC_JMP_ABS32NB; j+=4; }
			//8/9 undefined
			if(l==0x0A) { ty=BGBCC_JMP_SECTION; j+=4; }
			if(l==0x0B) { ty=BGBCC_JMP_SECREL; j+=4; }

			if(l==0x14) { ty=BGBCC_JMP_NEAR32; j+=4; }
		}

//		printf("\t%s %08X %d\n", s, j, ty);

		BGBCC_EmitGotoPos(ctx, s, ty, j);
	}

	return(rv);
}

BGBCC_Object *BGBCC_COFF_LoadObjectBuf(char *name, byte *buf, int sz)
{
	char nbuf[256];

	BGBCC_Object *ctx;
	BGBCC_COFF_Info *inf;
//	FILE *fd;
	void *p;
	char *s, *s1;
	int i, j, k, l, o, n;

	if(((buf[0]!=0x4C) || (buf[1]!=0x01)) && 
		((buf[0]!=0x64) || (buf[1]!=0x86)) && 
		((buf[0]!=0xC0) || (buf[1]!=0x01)) && 
		((buf[0]!=0xC2) || (buf[1]!=0x01))
		)
	{
#if 1
		if((buf[0]==0x7F) && (buf[1]=='E') &&
			(buf[2]=='L') && (buf[3]=='F'))
		{
			ctx=BGBCC_ELF_LoadObjectBuf(name, buf, sz);
			return(ctx);
		}
#endif

		printf("COFF Sig Fail\n");
		return(NULL);
	}


	inf=(BGBCC_COFF_Info *)malloc(sizeof(BGBCC_COFF_Info));
	memset(inf, 0, sizeof(BGBCC_COFF_Info));

//	inf->name=bgbcc_strdup(name);

	inf->buf=buf;
	inf->bsz=sz;

	inf->symoffs=buf[8]|(buf[9]<<8)|(buf[10]<<16)|(buf[11]<<24);
	inf->nsyms=buf[12]|(buf[13]<<8)|(buf[14]<<16)|(buf[15]<<24);
	inf->stroffs=inf->symoffs+(inf->nsyms*18);

	inf->secoffs=20+(buf[16]|(buf[17]<<8));
	inf->nsecs=buf[2]|(buf[3]<<8);

//	printf("\tSecs 0x%X %d\n", inf->secoffs, inf->nsecs);

//	printf("\nSections:\n");
	for(i=0; i<inf->nsecs; i++)
	{
		j=inf->secoffs+(i*40);

		s=(char *)(buf+j);
		k=buf[j+12]|(buf[j+13]<<8)|(buf[j+14]<<16)|(buf[j+15]<<24);

		inf->sec_name[i]=s;
//		printf("\t%d %s\n", i+1, s);

		l=buf[j+8]|(buf[j+9]<<8)|(buf[j+10]<<16)|(buf[j+11]<<24);
		k=buf[j+12]|(buf[j+13]<<8)|(buf[j+14]<<16)|(buf[j+15]<<24);
//		printf("\t  VA Addr 0x%08X %d\n", k, l);

		inf->sec_rva_off[i]=k;
		inf->sec_rva_sz[i]=l;


		l=buf[j+16]|(buf[j+17]<<8)|(buf[j+18]<<16)|(buf[j+19]<<24);
		k=buf[j+20]|(buf[j+21]<<8)|(buf[j+22]<<16)|(buf[j+23]<<24);
//		printf("\t  Img Offs 0x%08X %d\n", k, l);

		inf->sec_buf_off[i]=k;
		inf->sec_buf_sz[i]=l;


		k=buf[j+24]|(buf[j+25]<<8)|(buf[j+26]<<16)|(buf[j+27]<<24);
		l=buf[j+32]|(buf[j+33]<<8);
//		printf("\t  Reloc 0x%08X %d\n", k, l);

		inf->sec_rlc_off[i]=k;
		inf->sec_rlc_num[i]=l;


		k=buf[j+28]|(buf[j+29]<<8)|(buf[j+30]<<16)|(buf[j+31]<<24);
		l=buf[j+34]|(buf[j+35]<<8);
//		printf("\t  Line 0x%08X %d\n", k, l);

		k=buf[j+36]|(buf[j+37]<<8)|(buf[j+38]<<16)|(buf[j+39]<<24);
//		printf("\t  Flags 0x%08X\n", k);

		inf->sec_flag[i]=k;
	}

	i=inf->nsyms;
	inf->sym_name=(char **)malloc(i*sizeof(char *));
	inf->sym_addr=(void **)malloc(i*sizeof(void *));
	inf->sym_offs=(int *)malloc(i*sizeof(int));
	inf->sym_sec=(int *)malloc(i*sizeof(int));
	inf->sym_stype=(byte *)malloc(i*4);

	memset(inf->sym_name, 0, i*sizeof(char *));
	memset(inf->sym_stype, 0, i*4);


//	printf("\nSymbols:\n");
	for(i=0; i<inf->nsyms; i++)
	{
		j=inf->symoffs+(i*18);

		if(buf[j])
		{
			memset(nbuf, 0, 16);
			memcpy(nbuf, buf+j, 8);
			s=strdup(nbuf);
		}else
		{
			k=buf[j+4]|(buf[j+5]<<8)|(buf[j+6]<<16)|(buf[j+7]<<24);
			s=(char *)(buf+inf->stroffs+k);
		}

		k=buf[j+8]|(buf[j+9]<<8)|(buf[j+10]<<16)|(buf[j+11]<<24);
		l=buf[j+12]|(buf[j+13]<<8);

		inf->sym_name[i]=s;
		inf->sym_offs[i]=k;
		inf->sym_sec[i]=l;

		inf->sym_stype[i*4+0]=buf[j+14];
		inf->sym_stype[i*4+1]=buf[j+15];
		inf->sym_stype[i*4+2]=buf[j+16];
		inf->sym_stype[i*4+3]=buf[j+17];

		i+=buf[j+17];
	}


//	printf("Load Sections\n");

	j=0; k=0;
	for(i=0; i<inf->nsecs; i++)
	{
		if(inf->sec_flag[i]&0x20)
		{
			inf->sec_rva_off[i]=j;
			j+=inf->sec_buf_sz[i];
			j=(j+15)&(~15);
		}else
		{
			inf->sec_rva_off[i]=k;
			k+=inf->sec_buf_sz[i];
			k=(k+15)&(~15);
		}
	}

	inf->text=(byte *)malloc(j+16);
	inf->tsz=j;

	inf->data=(byte *)malloc(k+16);
	inf->dsz=k;

#if 0
#ifdef X86_64
	i=(int)((long long)inf->text);
	if(i&15)inf->text+=16-(i&15);

	i=(int)((long long)inf->data);
	if(i&15)inf->data+=16-(i&15);

#else
	i=(int)inf->text;
	if(i&15)inf->text+=16-(i&15);

	i=(int)inf->data;
	if(i&15)inf->data+=16-(i&15);
#endif
#endif

	for(i=0; i<inf->nsecs; i++)
	{
		if(inf->sec_flag[i]&0x20)
		{
			j=inf->sec_rva_off[i];
			k=inf->sec_buf_sz[i];
			l=inf->sec_buf_off[i];
			memcpy(inf->text+j, inf->buf+l, k);
		}else
		{
			j=inf->sec_rva_off[i];
			k=inf->sec_buf_sz[i];
			l=inf->sec_buf_off[i];
			memcpy(inf->data+j, inf->buf+l, k);
		}
	}

	ctx=BGBCC_NewContext();
	if(name)ctx->name=bgbcc_strdup(name);

	if((buf[0]==0x64) && (buf[1]==0x86))
		ctx->flags|=BGBCC_FL_X86_64;

	if((buf[0]==0xC0) && (buf[1]==0x01))
		ctx->flags|=BGBCC_FL_ARM;
	if((buf[0]==0xC2) && (buf[1]==0x01))
		ctx->flags|=BGBCC_FL_ARM|BGBCC_FL_THUMB;

//	printf("Obj %02X%02X %s\n", buf[1], buf[0],
//		(ctx->flags&BGBCC_FL_X86_64)?"x64":"x86");

	ctx->text=inf->text;
	ctx->data=inf->data;
	ctx->text_sz=inf->tsz;
	ctx->data_sz=inf->dsz;

//	ctx->ip=ctx->text+inf->tsz;
//	ctx->dp=ctx->data+inf->dsz;

//	printf("Resolve Symbols\n");

	for(i=0; i<inf->nsyms; i++)
	{

		//export symbol
		if((inf->sym_stype[i*4+2]==2) ||
			(inf->sym_stype[i*4+2]==3))
		{
			j=inf->sym_sec[i];

			if((j<1) || (j>inf->nsecs))
				continue;

			k=inf->sec_rva_off[j-1];
			l=inf->sym_offs[i];
			k=k+l;

			if(inf->sec_flag[j-1]&0x20)k|=(1<<24);
				else k|=(2<<24);

			s=inf->sym_name[i];
//			if(*s=='.')continue;

//			if((*s!='_') || (inf->sym_stype[i*4+2]==3))
			if(inf->sym_stype[i*4+2]==3)
			{
				s1=name;
				if(!s1)s1="$";
				sprintf(nbuf, "%s:%s", s1, s);
				s=bgbcc_strdup(nbuf);

//				s=BGBCC_GenSym2();
				inf->sym_name[i]=s;
//				continue;
			}

			if(!(ctx->flags&BGBCC_FL_X86_64))
			{
				if(*s!='_')
				{
					s1=name;
					if(!s1)s1="$";
					sprintf(nbuf, "%s:%s", s1, s);
					s=bgbcc_strdup(nbuf);

					inf->sym_name[i]=s;
				}else s++;
			}

//			printf("export %s %08X\n", s, k);

			BGBCC_EmitLabelPos(ctx, s, k);

			continue;
		}

		//import symbol
		if(inf->sym_stype[i*4+2]==2)
		{
//			s=inf->sym_name[i];
//			printf("import %s\n", s);
		}

	}

//	if((buf[0]==0x64) && (buf[1]==0x86))
//		ctx->flags|=BGBCC_FL_X86_64;
//	printf("Obj2 %02X%02X %s\n", buf[1], buf[0],
//		(ctx->flags&BGBCC_FL_X86_64)?"x64":"x86");

	for(i=0; i<inf->nsecs; i++)
	{
		BGBCC_COFF_ResolveRelocs(ctx, inf, i,
			inf->sec_rlc_off[i],
			inf->sec_rlc_num[i]);
	}

	return(ctx);
}

#if 0
BGBCC_Object *BGBCC_COFF_LoadObject(char *name)
{
	char nbuf[16];

	BGBCC_Object *ctx;
	byte *buf;
//	FILE *fd;
	void *p;
	char *s;
	int i, j, k, l, o, n;

	printf("BGBCC_COFF_LoadObject '%s'\n", name);

#if 0
	fd=fopen(name, "rb");

	if(!fd)
	{
		printf("BGBCC_COFF_LoadObject: fail open\n");
		return(NULL);
	}

	fseek(fd, 0, 2);
	i=ftell(fd);
	fseek(fd, 0, 0);
	buf=(byte *)malloc(i);
	fread(buf, 1, i, fd);
	fclose(fd);
#endif

	buf=bgbcc_loadfile(name, &i);
	if(!buf)
	{
		printf("BGBCC_COFF_LoadObject: fail open\n");
		return(NULL);
	}

	ctx=BGBCC_COFF_LoadObjectBuf(name, buf, i);
	if(!ctx)return(NULL);
	BGBCC_LinkModule(ctx);

//	free(buf);

	return(ctx);
}
#endif

#if 0
int BGBCC_COFF_LoadLibraryBuf(char *name, byte *buf, int sz)
{
	char nbuf[64];

	BGBCC_Object *ctx;
//	FILE *fd;
	byte *cs, *ce;
	struct ar_hdr *arh;
	char *s;
	int i, j, k, l;

//	printf("BGBCC_COFF_LoadLibrary '%s'\n", name);

	if(	((buf[0]==0x4C)&&(buf[1]==0x01)) ||
		((buf[0]==0x64)&&(buf[1]==0x86)))
	{
		ctx=BGBCC_COFF_LoadObjectBuf(name, buf, sz);
		if(!ctx)return(-1);
		BGBCC_LinkModuleQueue(ctx);
		return(0);
	}

#if 1
	if((buf[0]==0x7F)&&(buf[1]=='E')&&(buf[2]=='L')&&(buf[3]=='F'))
	{
		ctx=BGBCC_ELF_LoadObjectBuf(name, buf, sz);
		if(!ctx)return(-1);
		BGBCC_LinkModuleQueue(ctx);
		return(0);
	}
#endif

	if(strncmp((const char *)buf, "!<arch>\n", 8))
	{
		printf("BGBCC_COFF_LoadLibrary: bad archive magic\n");
		return(-1);
	}

	cs=buf+8; ce=buf+sz;

	while(cs<ce)
	{
		arh=(struct ar_hdr *)cs;

		if(arh->ar_fmag[0]!='`')
		{
			printf("Parse Fail @ %X\n", cs-buf);
			break;
		}

		arh->ar_fmag[1]=0;

		i=atoi(arh->ar_size);
		printf("%s %d\n", cs, i);

		nbuf[16]=0;
		strncpy(nbuf, arh->ar_name, 16);
		s=nbuf;
		while(*s)
		{
			if(*s=='/') { *s=0; break; }
			s++;
		}

		ctx=BGBCC_COFF_LoadObjectBuf(nbuf, cs+60, i);
		if(ctx) BGBCC_LinkModuleQueue(ctx);

		if(i&1)i++;
		cs+=60+i;
	}

	return(0);
}
#endif

#if 0
int BGBCC_COFF_LoadLibrary(char *name)
{
	char nbuf[64];

	BGBCC_Object *ctx;
//	FILE *fd;
	byte *buf, *cs, *ce;
	struct ar_hdr *arh;
	char *s;
	int i, j, k, l, sz;

	buf=bgbcc_loadfile(name, &sz);
	if(!buf)
	{
//		printf("BGBCC_COFF_LoadLibrary: fail open '%s'\n", name);
		return(-1);
	}

	BGBCC_COFF_LoadLibraryBuf(name, buf, sz);

	return(0);
}
#endif

#if 0
int BGBCC_COFF_LoadLibrary(char *name)
{
	char nbuf[64];

	BGBCC_Object *ctx;
	FILE *fd;
	byte *buf, *cs, *ce;
	struct ar_hdr *arh;
	char *s;
	int i, j, k, l, sz;

#if 0
	fd=fopen(name, "rb");

	if(!fd)
	{
		printf("BGBCC_COFF_LoadLibrary: fail open\n");
		return(-1);
	}

	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	buf=(byte *)malloc(sz);
	fread(buf, 1, sz, fd);
	fclose(fd);
#endif

	buf=bgbcc_loadfile(name, &sz);
	if(!buf)
	{
//		printf("BGBCC_COFF_LoadLibrary: fail open '%s'\n", name);
		return(-1);
	}

	printf("BGBCC_COFF_LoadLibrary '%s'\n", name);

	if((buf[0]==0x4C) || (buf[1]==0x01))
	{
		ctx=BGBCC_COFF_LoadObjectBuf(name, buf, sz);
		BGBCC_LinkModule(ctx);
		return(0);
	}

	if(strncmp((const char *)buf, "!<arch>\n", 8))
	{
		printf("BGBCC_COFF_LoadLibrary: bad archive magic\n");
		return(-1);
	}

	cs=buf+8; ce=buf+sz;

	while(cs<ce)
	{
		arh=(struct ar_hdr *)cs;

		if(arh->ar_fmag[0]!='`')
		{
			printf("Parse Fail @ %X\n", cs-buf);
			break;
		}

		arh->ar_fmag[1]=0;

		i=atoi(arh->ar_size);
		printf("%s %d\n", cs, i);

		nbuf[16]=0;
		strncpy(nbuf, arh->ar_name, 16);
		s=nbuf;
		while(*s)
		{
			if(*s=='/') { *s=0; break; }
			s++;
		}

		ctx=BGBCC_COFF_LoadObjectBuf(nbuf, cs+60, i);
		BGBCC_LinkModule(ctx);

		if(i&1)i++;
		cs+=60+i;
	}

	return(0);
}
#endif



//COFF generator

struct bgbcc_coff_hdr_s {
byte mach[2];		//machine
byte num_sec[2];	//number of sections
byte date[4];		//timestamp
byte sym_pos[4];	//symbol table offset
byte sym_num[4];	//symbol table count
byte opt_sz[2];		//optional header size
byte flags[2];		//file characteristics
};

struct bgbcc_coff_sec_s {
byte name[8];		//section name
byte rva_size[4];	//virtual size
byte rva_addr[4];	//virtual addr
byte raw_size[4];	//file size
byte raw_offs[4];	//file offset
byte rlc_offs[4];	//relocation offset
byte lin_offs[4];	//line number offset
byte rlc_num[2];	//relocation count
byte lin_num[2];	//line number count
byte flags[4];		//section characteristics
};

struct bgbcc_coff_sym_s {
byte name[8];		//symbol name or str index
byte value[4];		//symbol value/addr
byte sec[2];		//section number
byte type[2];		//variable type
byte cls;		//storage class
byte aux;		//number of aux symbol slots
};

struct bgbcc_coff_reloc_s {
byte addr[4];
byte sym[4];
byte type[2];
};

void bgbcc_coff_set16(byte *ptr, int i)
{
	ptr[0]=i&0xFF;
	ptr[1]=(i>>8)&0xFF;
}

void bgbcc_coff_set32(byte *ptr, int i)
{
	ptr[0]=i&0xFF;
	ptr[1]=(i>>8)&0xFF;
	ptr[2]=(i>>16)&0xFF;
	ptr[3]=(i>>24)&0xFF;
}

int bgbcc_coff_get16(byte *ptr)
{
	int i;
	i=ptr[0]+(ptr[1]<<8);
	return((short)i);
}

int bgbcc_coff_get32(byte *ptr)
{
	int i;
	i=ptr[0]+(ptr[1]<<8)+(ptr[2]<<16)+(ptr[3]<<24);
	return(i);
}


int bgbcc_coff_idxstr(char *strs, char *str)
{
	char *s;
	int i;

	if(!str)return(4);
	if(!*str)return(4);

	s=strs+5;
	while(*s)
	{
		if(!strcmp(s, str))
			return(s-strs);
		s+=strlen(s)+1;
	}

	i=s-strs;
	strcpy(s, str);
	s+=strlen(s)+1;
	bgbcc_coff_set32((byte *)strs, s-strs);
	return(i);
}

int bgbcc_coff_idxstr2(char *strs, char *str, int fl)
{
	char buf[256];
	int i;

	if(!str)return(4);
	if(!*str)return(4);

	if(fl&1)
	{
		i=bgbcc_coff_idxstr(strs, str);
		return(i);
	}

	sprintf(buf, "_%s", str);
	i=bgbcc_coff_idxstr(strs, buf);
	return(i);
}

int BGBCC_COFF_StoreObjectBuf(BGBCC_Object *ctx, byte *buf, int msz)
{
	char tb[256], tb1[256];
	struct bgbcc_coff_hdr_s *hdr;
	struct bgbcc_coff_sec_s *sec_text;
	struct bgbcc_coff_sec_s *sec_data;
	struct bgbcc_coff_sym_s *syms;
	struct bgbcc_coff_reloc_s *rlc_text;
	struct bgbcc_coff_reloc_s *rlc_data;

	byte *ct;
	char *strs;

	char *s, *t;
	int tsz, dsz, toffs, doffs, nsym;
	int ntrlc, ndrlc, x64;
	int i, j, k, l;

#if 0
	for(i=0; i<ctx->n_proxy; i++)
	{
//		BGBCC_AddProxy(ctx->proxy_name[i]);

//		s=BGBCC_GenSym();
		t=ctx->proxy_name[i];

		s=NULL;
		if(!s)s="$";
		sprintf(tb, "%s:%s", s, t);
		s=bgbcc_strdup(tb);


		//rename proxied function
		for(j=0; j<ctx->n_labels; j++)
			if(!strcmp(ctx->label_name[j], t))
				ctx->label_name[j]=bgbcc_strdup(s);

		//generate proxy stub (added to object context)
		sprintf(tb1, "__proxy_%s", t);
		sprintf(tb, "\n\n.data\n%s dd %s\n.text\n%s: jmp [%s]\n",
			tb1, s, t, tb1);
		BGBCC_ParseOpcodeList(ctx, tb);
	}
#endif

//	tsz=ctx->ip-ctx->text;
//	dsz=ctx->dp-ctx->data;

	tsz=ctx->text_sz;
	dsz=ctx->data_sz;

	memset(buf, 0, msz);
	ct=buf;

	hdr=(struct bgbcc_coff_hdr_s *)buf;

	if(ctx->flags&BGBCC_FL_X86_64)
	{
		bgbcc_coff_set16(hdr->mach, 0x8664);
		x64=1;
	}else
	{
		bgbcc_coff_set16(hdr->mach, 0x014C);
		x64=0;
	}

	bgbcc_coff_set16(hdr->num_sec, 2);
	bgbcc_coff_set16(hdr->flags, 0x0104);
	ct+=sizeof(struct bgbcc_coff_hdr_s);

	sec_text=(struct bgbcc_coff_sec_s *)ct;
	ct+=sizeof(struct bgbcc_coff_sec_s);

	sec_data=(struct bgbcc_coff_sec_s *)ct;
	ct+=sizeof(struct bgbcc_coff_sec_s);

	strcpy((char *)sec_text->name, ".text");
	strcpy((char *)sec_data->name, ".data");
	bgbcc_coff_set32(sec_text->flags, 0x60000020);
	bgbcc_coff_set32(sec_data->flags, 0xC0000040);

	toffs=ct-buf; toffs=(toffs+15)&(~15);
	ct=buf+toffs+tsz;
	doffs=ct-buf; doffs=(doffs+15)&(~15);
	ct=buf+doffs+dsz;

	bgbcc_coff_set32(sec_text->raw_offs, toffs);
	bgbcc_coff_set32(sec_text->raw_size, tsz);

	bgbcc_coff_set32(sec_data->raw_offs, doffs);
	bgbcc_coff_set32(sec_data->raw_size, dsz);

	memcpy(buf+toffs, ctx->text, tsz);
	memcpy(buf+doffs, ctx->data, dsz);


	i=ct-buf; i=(i+15)&(~15); ct=buf+i;
	syms=(struct bgbcc_coff_sym_s *)ct;
	bgbcc_coff_set32(hdr->sym_pos, i);
	//dunno size/number yet


	strs=(char *)malloc(1<<16);
	memset(strs, 0, 1<<16);
	bgbcc_coff_set32((byte *)strs, 5);

	for(i=0; i<ctx->n_labels; i++)
	{
		j=bgbcc_coff_idxstr2(strs, ctx->label_name[i], x64);
		bgbcc_coff_set32(syms[i].name+4, j);

		j=(ctx->label_pos[i]>>24);
		k=ctx->label_pos[i]&0xFFFFFF;
		bgbcc_coff_set16(syms[i].sec, j);
		bgbcc_coff_set32(syms[i].value, k);

		if(j==1)bgbcc_coff_set16(syms[i].type, 0x20);
		syms[i].cls=2;
		syms[i].aux=0;
	}

	nsym=i;

	for(i=0; i<ctx->n_gotos; i++)
	{
		j=bgbcc_coff_idxstr2(strs, ctx->goto_name[i], x64);
		for(k=0; k<nsym; k++)
		{
			l=bgbcc_coff_get32(syms[k].name+4);
			if(j==l)break;
		}
		if(k<nsym)continue;

		k=nsym++;
		bgbcc_coff_set32(syms[k].name+4, j);
		syms[k].cls=2;
		syms[k].aux=0;
	}

	bgbcc_coff_set32(hdr->sym_num, nsym);
	ct+=nsym*sizeof(struct bgbcc_coff_sym_s);

	i=bgbcc_coff_get32((byte *)strs)+1;
	memcpy(ct, strs, i);
	free(strs);
	strs=(char *)ct;
	ct+=i;


	i=ct-buf; i=(i+15)&(~15); ct=buf+i;
	rlc_text=(struct bgbcc_coff_reloc_s *)ct;

	bgbcc_coff_set32(sec_text->rlc_offs, i);

	ntrlc=0;
	for(i=0; i<ctx->n_gotos; i++)
	{
		j=(ctx->goto_pos[i]>>24);
		if(j!=1)continue;

		j=ctx->goto_pos[i]&0xFFFFFF;

		if((ctx->goto_type[i]&15)==BGBCC_JMP_ABS16)j-=2;
		if((ctx->goto_type[i]&15)==BGBCC_JMP_ABS32)j-=4;
		if((ctx->goto_type[i]&15)==BGBCC_JMP_ABS64)j-=8;

		if((ctx->goto_type[i]&15)==BGBCC_JMP_NEAR16)j-=2;
		if((ctx->goto_type[i]&15)==BGBCC_JMP_NEAR32)j-=4;

//		bgbcc_coff_set32(rlc_text[ntrlc].addr, j-4);
		bgbcc_coff_set32(rlc_text[ntrlc].addr, j);

		j=bgbcc_coff_idxstr2(strs, ctx->goto_name[i], x64);
		for(k=0; k<nsym; k++)
		{
			l=bgbcc_coff_get32(syms[k].name+4);
			if(j==l)break;
		}
		if(k>=nsym)continue;

		bgbcc_coff_set32(rlc_text[ntrlc].sym, k);

		if(x64&1)
		{
			j=0x0002;
			if((ctx->goto_type[i]&15)==BGBCC_JMP_ABS64)j=0x0001;
			if((ctx->goto_type[i]&15)==BGBCC_JMP_ABS32)j=0x0002;
			if((ctx->goto_type[i]&15)==BGBCC_JMP_NEAR32)j=0x0004;
		}else
		{
			j=0x0006;
			if((ctx->goto_type[i]&15)==BGBCC_JMP_ABS32)j=0x0006;
			if((ctx->goto_type[i]&15)==BGBCC_JMP_NEAR32)j=0x0014;
		}
		bgbcc_coff_set16(rlc_text[ntrlc].type, j);

		ntrlc++;
	}

	bgbcc_coff_set16(sec_text->rlc_num, ntrlc);
	ct+=ntrlc*sizeof(struct bgbcc_coff_reloc_s);


	i=ct-buf; i=(i+15)&(~15); ct=buf+i;
	rlc_data=(struct bgbcc_coff_reloc_s *)ct;

	bgbcc_coff_set32(sec_data->rlc_offs, i);

	ndrlc=0;
	for(i=0; i<ctx->n_gotos; i++)
	{
		j=(ctx->goto_pos[i]>>24);
		if(j!=2)continue;

		j=ctx->goto_pos[i]&0xFFFFFF;
		bgbcc_coff_set32(rlc_data[ndrlc].addr, j-4);

		j=bgbcc_coff_idxstr2(strs, ctx->goto_name[i], x64);
		for(k=0; k<nsym; k++)
		{
			l=bgbcc_coff_get32(syms[k].name+4);
			if(j==l)break;
		}
		if(k>=nsym)continue;

		bgbcc_coff_set32(rlc_data[ndrlc].sym, k);

		j=0x0006;
		if((ctx->goto_type[i]&15)==BGBCC_JMP_ABS32)j=0x0006;
		if((ctx->goto_type[i]&15)==BGBCC_JMP_NEAR32)j=0x0014;
		bgbcc_coff_set16(rlc_data[ndrlc].type, j);

		ndrlc++;
	}

	bgbcc_coff_set16(sec_data->rlc_num, ndrlc);
	ct+=ndrlc*sizeof(struct bgbcc_coff_reloc_s);

	return(ct-buf);
}

int BGBCC_COFF_StoreObject(char *name, BGBCC_Object *ctx)
{
	FILE *fd;
	byte *buf;
	int i, sz;

	buf=(byte *)malloc(1<<24);
	sz=BGBCC_COFF_StoreObjectBuf(ctx, buf, 1<<24);
	if(sz<0)
	{
		free(buf);
		return(sz);
	}

	i=bgbcc_storefile(name, buf, sz);
	free(buf);
	return(i);

#if 0
	fd=fopen(name, "wb");
	if(!fd)
	{
		free(buf);
		return(-1);
	}

	fwrite(buf, 1, sz, fd);
	fclose(fd);

	free(buf);
	return(0);
#endif
}

byte *BGBCC_COFF_EncodeObject(BGBCC_Object *ctx, int *rsz)
{
	FILE *fd;
	byte *buf;
	int i, sz;

	buf=(byte *)malloc(1<<24);
	sz=BGBCC_COFF_StoreObjectBuf(ctx, buf, 1<<24);
	if(sz<0)
	{
		free(buf);
		return(NULL);
	}

	buf=(byte *)realloc(buf, sz);
	if(rsz)*rsz=sz;
	return(buf);
}

