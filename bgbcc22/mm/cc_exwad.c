#include <bgbccc.h>

u16 exwad_getu16(byte *b)
{
	return(b[0]+(b[1]<<8));
}

u32 exwad_getu32(byte *b)
{
	return(b[0]+(b[1]<<8)+(b[2]<<16)+(b[3]<<24));
}

void exwad_setu16(byte *b, u16 a)
{
	b[0]=a&255;
	b[1]=(a>>8)&255;
}

void exwad_setu32(byte *b, u32 a)
{
	b[0]=a&255;
	b[1]=(a>>8)&255;
	b[2]=(a>>16)&255;
	b[3]=(a>>24)&255;
}

u32 ExWAD_CRC32(void *buf, int sz, u32 lcrc)
{
	static u32 crctab[256];
	static int init=0;

	byte *s;
	u32 c;
	int i, j;

	if(!init)
	{
		init=1;

		for(i=0; i<256; i++)
		{
			c=i;
			for(j=0; j<8; j++)
				if(c&1)c=0xedb88320^(c>>1);
					else c>>=1;
			crctab[i]=c;
		}
	}

	c=lcrc; s=buf; i=sz;
	while(i--)c=crctab[(c^(*s++))&0xFF]^(c>>8);
	return(c);
}

void ExWAD_StoreImage(char *name, ExWAD_Context *ctx)
{
	ExWAD_FixupImage(ctx);
	BGBCC_StoreFile(name, ctx->img_base, ctx->img_size);
}

int ExWAD_SaveImage(ExWAD_Context *ctx)
{
	int i, sz;

	ExWAD_FixupImage(ctx);

	sz=ctx->img_saveSize;
//	sz=ctx->img_size;

	i=BGBCC_StoreFile(ctx->name, ctx->img_base, sz);
	if(i<0)
	{
		printf("ExWAD_SaveImage: failed write %s\n", ctx->name);
		return(-1);
	}

	printf("ExWAD_SaveImage: wrote %s, %d bytes\n", ctx->name, sz);
	return(0);
}

ExWAD_Context *ExWAD_LoadImage(char *name)
{
	ExWAD_Context *tmp;
	byte *buf, *buf1;
	char *s;
	int i, j, k, n, sz;

	buf=bgbcc_loadfile(name, &sz);
	if(!buf)return(NULL);

	tmp=ExWAD_LoadImageBuffer(name, buf, sz);
	return(tmp);
}

ExWAD_Context *ExWAD_LoadImageBuffer(char *name, byte *buf, int sz)
{
	ExWAD_Context *tmp;
	byte *buf1;
	char *s;
	int i, j, k, n;

//	buf=bgbcc_loadfile(name, &sz);
//	if(!buf)return(NULL);

	tmp=malloc(sizeof(ExWAD_Context));
	memset(tmp, 0, sizeof(ExWAD_Context));
	tmp->img_base=buf;
	tmp->img_size=sz;
	tmp->head=NULL;
	tmp->dir=NULL;
	
	tmp->name=bgbcc_strdup(name);
	
	if((buf[0]=='M') && (buf[1]=='Z'))
	{
		i=exwad_getu32(buf+0x38);
		if((i>0) && (i<=(sz-EXWAD_CELLSZ)) && !(i&(EXWAD_CELLSZ-1)))
		{
			buf1=buf+i;
			if(!strncmp(buf1, EXWAD_MAGIC1, 8))
			{
				tmp->head=(ExWAD_Header *)buf1;
				tmp->head_rva=i;
			}
		}
	}else if((buf[0]==0x7F) && (buf[1]=='E') &&
		(buf[2]=='L') && (buf[3]=='F'))
	{
		i=exwad_getu32(buf+0x0C);
		if((i>0) && (i<=(sz-EXWAD_CELLSZ)) && !(i&(EXWAD_CELLSZ-1)))
		{
			buf1=buf+i;
			if(!strncmp(buf1, EXWAD_MAGIC1, 8))
			{
				tmp->head=(ExWAD_Header *)buf1;
				tmp->head_rva=i;
			}
		}
	}
	
	if(!tmp->head)
	{
		for(i=0; i<(sz/EXWAD_CELLSZ); i++)
		{
			buf1=buf+(i*EXWAD_CELLSZ);
			if(!memcmp(buf1, EXWAD_MAGIC1, 8) &&
				!memcmp(buf1+16, EXWAD_MAGIC2, 8))
			{
				tmp->head=(ExWAD_Header *)buf1;
				tmp->head_rva=i*EXWAD_CELLSZ;
				break;
			}
		}
	}

	if(tmp->head)
	{
		i=(tmp->img_size+(EXWAD_CELLSZ-1))/EXWAD_CELLSZ;
		tmp->img_map=malloc(i);
		memset(tmp->img_map, 0, i);

		i=exwad_getu32(tmp->head->rva_dirents);
		buf1=buf+i;
		tmp->dir=(ExWAD_DirEnt *)buf1;

		n=exwad_getu32(tmp->head->num_dirents);
				
		ExWAD_MarkRangeResv(tmp, 0, tmp->head_rva+32);
		ExWAD_MarkRangeUsed(tmp, i, n*sizeof(ExWAD_DirEnt));

		for(i=0; i<n; i++)
		{
			j=exwad_getu32(tmp->dir[i].rva_data);
			k=exwad_getu32(tmp->dir[i].csize_data);
			ExWAD_MarkRangeUsed(tmp, j, k);

			j=exwad_getu16(tmp->dir[i].flags);
			if(j&EXWAD_DIRFL_RVANAME)
			{
				k=exwad_getu32(tmp->dir[i].name);
				s=(char *)(tmp->img_base+k);
				ExWAD_MarkRangeResv(tmp, k, strlen(s+1));
			}
		}
		
		return(tmp);
	}

	i=(tmp->img_size+(EXWAD_CELLSZ-1))/EXWAD_CELLSZ;
	tmp->img_map=malloc(i);
	memset(tmp->img_map, 0, i);
	ExWAD_MarkRangeResv(tmp, 0, tmp->img_size);
	
	return(tmp);
}

void ExWAD_MarkRangeResv(ExWAD_Context *ctx, int base, int sz)
{
	int i, j, n;

	j=base/EXWAD_CELLSZ; n=(sz+(EXWAD_CELLSZ-1))/EXWAD_CELLSZ;
	for(i=0; i<n; i++)
		ctx->img_map[j+i]|=9;
}

void ExWAD_MarkRangeUsed(ExWAD_Context *ctx, int base, int sz)
{
	int i, j, n;

	j=base/EXWAD_CELLSZ; n=(sz+(EXWAD_CELLSZ-1))/EXWAD_CELLSZ;
	for(i=0; i<n; i++)
		ctx->img_map[j+i]|=1;
}

void ExWAD_MarkRangeFree(ExWAD_Context *ctx, int base, int sz)
{
	int i, j, n;

	j=base/EXWAD_CELLSZ; n=(sz+(EXWAD_CELLSZ-1))/EXWAD_CELLSZ;
	for(i=0; i<n; i++)
	{
		//check for and skip reserved
		if(ctx->img_map[j+i]&8)
			continue;
		ctx->img_map[j+i]=0;
	}
}

void ExWAD_CheckExpandImage(ExWAD_Context *img, int sz)
{
	int rva_head, rva_dir;
	int i, j, k;

	if(sz<=img->img_size)
		return;

	printf("ExWAD_CheckExpandImage: %d\n", sz);

	rva_head=((byte *)img->head)-img->img_base;
	rva_dir=((byte *)img->dir)-img->img_base;
	
	i=img->img_size;
	if(i<=0)i=4096;
	while(i<sz)i+=i>>2;
	i=(i+(EXWAD_CELLSZ-1))&(~(EXWAD_CELLSZ-1));		//pad up to even cell
	
	img->img_base=realloc(img->img_base, i);
	memset(img->img_base+img->img_size, 0, i-img->img_size);

	j=i/EXWAD_CELLSZ;
	k=(img->img_size+(EXWAD_CELLSZ-1))/EXWAD_CELLSZ;
	img->img_map=realloc(img->img_map, j);
	memset(img->img_map+k, 0, j-k);

	img->head=(ExWAD_Header *)(img->img_base+rva_head);
	img->dir=(ExWAD_DirEnt *)(img->img_base+rva_dir);
	img->img_size=i;
}

int ExWAD_FindFreeRange(ExWAD_Context *ctx, int sz)
{
	int i, j, k, l, n;
	
	//try to find free run of cells
	n=ctx->img_size/EXWAD_CELLSZ; l=(sz+(EXWAD_CELLSZ-1))/EXWAD_CELLSZ;
	for(i=0; i<n; i++)
	{
		if(ctx->img_map[i])
			continue;
		for(j=0; j<l; j++)
			if(ctx->img_map[i+j])
				break;
		if(j>=l)return(i);
		i+=j;
	}
	
	//allocate on end
	i=(ctx->img_size+(EXWAD_CELLSZ-1))/EXWAD_CELLSZ;
	ExWAD_CheckExpandImage(ctx, (i*EXWAD_CELLSZ)+(l*EXWAD_CELLSZ));
	
	i=ExWAD_FindFreeRange(ctx, sz);
	return(i);
}

int ExWAD_AllocRange(ExWAD_Context *ctx, int sz)
{
	int i;
	
	i=ExWAD_FindFreeRange(ctx, sz);
	ExWAD_MarkRangeUsed(ctx, i*EXWAD_CELLSZ, sz);
	memset(ctx->img_base+i*EXWAD_CELLSZ, 0, sz);
	return(i);
}

int ExWAD_FindLastCell(ExWAD_Context *ctx)
{
	int i, n;

	n=(ctx->img_size+(EXWAD_CELLSZ-1))/EXWAD_CELLSZ;
	for(i=n-1; i>0; i--)
		if(ctx->img_map[i])
			break;
	return(i);
}

void ExWAD_FixupEXE(ExWAD_Context *ctx)
{
	int i, pehdr, pealgn, peszimg;

	//check magic...
	if((ctx->img_base[0]!='M') || (ctx->img_base[1]!='Z'))
		return;	//not an EXE

	//look for PE/COFF header
	pehdr=exwad_getu32(ctx->img_base+0x3C);
	if((pehdr<=0) || pehdr>=ctx->img_size)
		return;	//bad address
//	if((ctx->img_base[pehdr+0]!='P') || (ctx->img_base[pehdr+1]!='E'))
//		return;	//not PE/COFF
	i=exwad_getu32(ctx->img_base+pehdr);
	if(i!=0x00004550)
		return;	//not PE/COFF
	pealgn=exwad_getu32(ctx->img_base+pehdr+24+32);
	peszimg=exwad_getu32(ctx->img_base+pehdr+24+56);

	printf("ExWAD_FixupEXE: SectionAlignment=%d, SizeOfImage=%d\n",
		pealgn, peszimg);

	//force to alignment
//	i=(ctx->img_size+(pealgn-1))/pealgn;
	i=(ctx->img_saveSize+(pealgn-1))/pealgn;
//	exwad_setu32(ctx->img_base+pehdr+24+56, i*pealgn);

	ctx->img_saveSize=i*pealgn;

	printf("ExWAD_FixupEXE: new SizeOfImage=%d\n",
		ctx->img_saveSize);

	ExWAD_CheckExpandImage(ctx, ctx->img_saveSize);

	//update header RVA
	exwad_setu32(ctx->img_base+0x38, ctx->head_rva);
}

void ExWAD_FixupImage(ExWAD_Context *ctx)
{
	int i;

	i=ExWAD_FindLastCell(ctx);
	ctx->img_saveSize=(i+1)*EXWAD_CELLSZ;
	
	printf("ExWAD_FixupImage: End of Image At %08X, %d\n",
		ctx->img_saveSize, ctx->img_saveSize);

	if((ctx->img_base[0]=='M') && (ctx->img_base[1]=='Z'))
	{
//		exwad_setu32(tmp->img_base+0x38, tmp->head_rva);
		ExWAD_FixupEXE(ctx);
	}

	if((ctx->img_base[0]=='\x7F') && (ctx->img_base[1]=='E') &&
		(ctx->img_base[2]=='L') && (ctx->img_base[3]=='F'))
	{
//		exwad_setu32(ctx->img_base+0x0C, ctx->head_rva);
	}
}

int ExWAD_Hash16(void *buf, int szbuf)
{
	byte *cs, *ce;
	int i, h;
	h=0; cs=(byte *)buf; ce=(byte *)buf+szbuf;
	while(cs<ce)h=(h*65521+1)+(*cs++);
	i=((h*65521+1)>>16)&65535;
	return(i);
}

ExWAD_Context *ExWAD_LoadImage2(char *name)
{
	ExWAD_Context *tmp;
	int i, j;
	
	tmp=ExWAD_LoadImage(name);
	if(!tmp)return(NULL);
	if(tmp->head)return(tmp);
	
	printf("ExWAD_LoadImage2: Create header\n");
	
	i=ExWAD_AllocRange(tmp, sizeof(ExWAD_Header));
	tmp->head=(ExWAD_Header *)(tmp->img_base+(i*EXWAD_CELLSZ));
	tmp->head_rva=i*EXWAD_CELLSZ;

	printf("ExWAD_LoadImage2: Create header at %08X\n", tmp->head_rva);

	memcpy(tmp->head->magic, EXWAD_MAGIC1, 8);
	memcpy(tmp->head->magic2, EXWAD_MAGIC2, 8);

	i=ExWAD_AllocRange(tmp, 16*sizeof(ExWAD_DirEnt));
	tmp->dir=(ExWAD_DirEnt *)(tmp->img_base+(i*EXWAD_CELLSZ));
	exwad_setu32(tmp->head->rva_dirents, i*EXWAD_CELLSZ);
	exwad_setu32(tmp->head->num_dirents, 16);

	exwad_setu32(tmp->head->head_rva, tmp->head_rva);
	exwad_setu16(tmp->head->head_size, sizeof(ExWAD_Header));
	exwad_setu16(tmp->head->head_hash, 0);
	
	j=ExWAD_Hash16(tmp->head, sizeof(ExWAD_Header));
	exwad_setu16(tmp->head->head_hash, j);

	printf("ExWAD_LoadImage2: Header Hash %04X\n", j);

	printf("ExWAD_LoadImage2: Create dir at %08X..%08X\n",
		(int)(i*EXWAD_CELLSZ), (int)(i*EXWAD_CELLSZ+16*sizeof(ExWAD_DirEnt)));
	
//	if((tmp->img_base[0]=='M') && (tmp->img_base[1]=='Z'))
//		{ exwad_setu32(tmp->img_base+0x38, tmp->head_rva); }
	ExWAD_FixupImage(tmp);

	return(tmp);
}

ExWAD_Context *ExWAD_LoadImageBufferRead(char *name, byte *buf, int sz)
{
	ExWAD_Context *tmp;
	int i;
	
	tmp=ExWAD_LoadImageBuffer(name, buf, sz);
	if(!tmp)return(NULL);
	if(tmp->head)return(tmp);

	free(tmp);
	return(NULL);
}

int ExWAD_GetDirEntIndexName(ExWAD_Context *ctx,
	int idx, char *nbuf)
{
	char tb[256];
	ExWAD_DirEnt *de;
	char *s, *t;
	int i, j, fl;

	de=&(ctx->dir[idx]);
	fl=exwad_getu16(de->flags);

	if(fl&EXWAD_DIRFL_RVANAME)
	{
		if(fl&EXWAD_DIRFL_DIRPFX)
		{
//			i=exwad_getu32(de->name+12);
			i=exwad_getu32(de->dir);
			j=exwad_getu32(de->name);
			ExWAD_GetDirEntIndexName(ctx, i, tb);
			s=(char *)(ctx->img_base+j);
			t=tb+strlen(tb);
			strcpy(t, s);
			strcpy(nbuf, tb);
		}else
		{
			j=exwad_getu32(de->name);
			s=(char *)(ctx->img_base+j);
			strcpy(nbuf, s);
		}
	}else if(fl&EXWAD_DIRFL_DIRPFX)
	{
//		i=de->name[14]+(de->name[15]<<8);
//		i=exwad_getu16(de->name+14);
		i=exwad_getu32(de->dir);
		ExWAD_GetDirEntIndexName(ctx, i, tb);
		s=tb+strlen(tb);
		memcpy(s, de->name, EXWAD_NAMESZ);
		s[EXWAD_NAMESZ+1]=0;
		strcpy(nbuf, tb);
	}else
	{
		memcpy(tb, de->name, EXWAD_NAMESZ);
		tb[EXWAD_NAMESZ+1]=0;
		s=tb;
		strcpy(nbuf, s);
	}
	return(0);
}

int ExWAD_LookupDirEnt(ExWAD_Context *ctx, char *name)
{
	char tb[256];
	ExWAD_DirEnt *de;
	char *s;
	int i, j, b, n, fl;
	
	b=exwad_getu32(ctx->head->rva_dirents);
	n=exwad_getu32(ctx->head->num_dirents);
	for(i=0; i<n; i++)
	{
		de=&(ctx->dir[i]);
		fl=exwad_getu16(de->flags);
		
		if(fl&EXWAD_DIRFL_DIRPFX)
		{
			ExWAD_GetDirEntIndexName(ctx, i, tb);
			s=tb;
		}else if(fl&EXWAD_DIRFL_RVANAME)
		{
			j=exwad_getu32(de->name);
			s=(char *)(ctx->img_base+j);
		}else
		{
			memcpy(tb, de->name, EXWAD_NAMESZ);
			tb[EXWAD_NAMESZ+1]=0;
			s=tb;
		}
		
		if(!strcmp(s, name))
			return(i);
	}
	
	return(-1);
}

void ExWAD_CheckExpandDir(ExWAD_Context *ctx, int num)
{
	int i, b, n, n1;

	b=exwad_getu32(ctx->head->rva_dirents);
	n=exwad_getu32(ctx->head->num_dirents);
	if(num<=n)return;
	
	printf("ExWAD_CheckExpandDir: %d\n", num);
	
	n1=n;
	while(n1<=num)n1=n1+(n1>>1);

	i=ExWAD_AllocRange(ctx, n1*sizeof(ExWAD_DirEnt));
	memcpy(ctx->img_base+i*EXWAD_CELLSZ, ctx->img_base+b,
		n*sizeof(ExWAD_DirEnt));
	ExWAD_MarkRangeFree(ctx, b, n);

	ctx->dir=(ExWAD_DirEnt *)(ctx->img_base+(i*EXWAD_CELLSZ));
	exwad_setu32(ctx->head->rva_dirents, i*EXWAD_CELLSZ);
	exwad_setu32(ctx->head->num_dirents, n1);
}

int ExWAD_GetStringRVA(ExWAD_Context *ctx, char *str)
{
	char *t;
	int i, n;
	
	//HACK: look for string somewhere in image (even aligned)
	n=ctx->img_size/EXWAD_CELLSZ;
	for(i=0; i<n; i++)
	{
		t=(char *)(ctx->img_base+(i*EXWAD_CELLSZ));
		if(!strcmp(t, str))
		{
			ExWAD_MarkRangeResv(ctx, i*EXWAD_CELLSZ, strlen(str)+1);
			return(i*EXWAD_CELLSZ);
		}
	}

	//HACK: look for string somewhere in image (byte aligned)
	for(i=0; i<ctx->img_size; i++)
	{
		t=(char *)(ctx->img_base+i);
		if(!strcmp(t, str))
		{
			ExWAD_MarkRangeResv(ctx, i, strlen(str)+1);
			return(i);
		}
	}

	//reserve a range to put string into
	i=ExWAD_AllocRange(ctx, strlen(str)+1);
	ExWAD_MarkRangeResv(ctx, i*EXWAD_CELLSZ, strlen(str)+1);
	t=(char *)(ctx->img_base+(i*EXWAD_CELLSZ));
	strcpy(t, str);
	return(i*EXWAD_CELLSZ);
}

int ExWAD_GetDirEnt(ExWAD_Context *ctx, char *name)
{
	char tb[256];
	ExWAD_DirEnt *de;
	char *s, *name1;
	int i, j, b, n, n1, fl, pfx;
	
	i=ExWAD_LookupDirEnt(ctx, name);
	if(i>=0)return(i);

#if 1
	if(strlen(name)>EXWAD_NAMESZ)
	{
		strcpy(tb, name);
		s=tb+strlen(tb);
//		while((s>tb) && (*s!='/'))s--;
		while(s>tb)
		{
			if((*s=='/') && s[1])
				break;
			s--;
		}
		if((s>tb) && (*s=='/') && s[1])
		{
			s[1]=0;
			pfx=ExWAD_GetDirEnt(ctx, tb);

			s=name+strlen(name);
//			while((s>name) && (*s!='/'))s--;
			while(s>tb)
			{
				if((*s=='/') && s[1])
					break;
				s--;
			}
			name1=s+1;
		}else
		{
			pfx=-1;
			name1=name;
		}
	}else
	{
		pfx=-1;
		name1=name;
	}
#endif

	b=exwad_getu32(ctx->head->rva_dirents);
	n=exwad_getu32(ctx->head->num_dirents);
	for(i=0; i<n; i++)
	{
		de=&(ctx->dir[i]);
//		fl=exwad_getxu16(de->flags);
		j=exwad_getu32(de->name);
		if(!j)
		{
			if((pfx>=0) && (pfx<65536) && (strlen(name1)<=EXWAD_NAMESZ))
			{
//				de->name[14]=pfx&0xFF;
//				de->name[15]=(pfx>>8)&0xFF;
				strcpy(de->name, name1);
//				exwad_setu16(de->name+14, pfx);
				exwad_setu32(de->dir, pfx);
				exwad_setu16(de->flags, EXWAD_DIRFL_DIRPFX);
			}else if(strlen(name)<=EXWAD_NAMESZ)
			{
				strcpy(de->name, name);
				exwad_setu16(de->flags, 0);
			}else
			{
				if((pfx>=0) && (pfx<65536))
				{
					j=ExWAD_GetStringRVA(ctx, name1);
					exwad_setu32(de->name, j);
//					exwad_setu32(de->name+12, pfx);
					exwad_setu32(de->dir, pfx);
					exwad_setu16(de->flags, EXWAD_DIRFL_RVANAME|
						EXWAD_DIRFL_DIRPFX);
				}else
				{
					j=ExWAD_GetStringRVA(ctx, name);
					exwad_setu32(de->name, j);
					exwad_setu16(de->flags, EXWAD_DIRFL_RVANAME);
				}
			}

			de->type=0;
			de->method=0;
			if(name[strlen(name)-1]=='/')
				de->type=2;
			
			return(i);
		}
	}
	
	ExWAD_CheckExpandDir(ctx, n+16);
	i=n+1;

	de=&(ctx->dir[i]);
	if((pfx>=0) && (pfx<65536) && (strlen(name1)<=EXWAD_NAMESZ))
	{
//		de->name[14]=pfx&0xFF;
//		de->name[15]=(pfx>>8)&0xFF;
		strcpy(de->name, name1);
//		exwad_setu16(de->name+14, pfx);
		exwad_setu32(de->dir, pfx);
		exwad_setu16(de->flags, EXWAD_DIRFL_DIRPFX);
	}else if(strlen(name)<=EXWAD_NAMESZ)
	{
		strcpy(de->name, name);
		exwad_setu16(de->flags, 0);
	}else
	{
		if((pfx>=0) && (pfx<65536))
		{
			j=ExWAD_GetStringRVA(ctx, name1);
			exwad_setu32(de->name, j);
//			exwad_setu32(de->name+12, pfx);
			exwad_setu32(de->dir, pfx);
			exwad_setu16(de->flags, EXWAD_DIRFL_RVANAME|
				EXWAD_DIRFL_DIRPFX);
		}else
		{
			j=ExWAD_GetStringRVA(ctx, name);
			exwad_setu32(de->name, j);
			exwad_setu16(de->flags, EXWAD_DIRFL_RVANAME);
		}
	}

	de->type=0;
	de->method=0;
	if(name[strlen(name)-1]=='/')
		de->type=2;

	return(i);
}

byte *ExWAD_LoadFile(ExWAD_Context *ctx, char *name, int *rsz)
{
	ExWAD_DirEnt *de;
	byte *buf;
	int i, j, k, l, b, sz;

	i=ExWAD_LookupDirEnt(ctx, name);
	if(i<0)return(NULL);

	de=&(ctx->dir[i]);

	j=exwad_getu32(de->rva_data);
	k=exwad_getu32(de->csize_data);
	l=exwad_getu32(de->dsize_data);
	
	if(de->method==EXWAD_DIR_CM_STORE)
	{
		buf=malloc(l);
		memcpy(buf, ctx->img_base+j, l);
		if(rsz)*rsz=l;
		return(buf);
	}

	if((de->method==EXWAD_DIR_CM_DEFLATE) ||
		(de->method==EXWAD_DIR_CM_DEFLATE64))
	{
		buf=malloc(l);
		PDUNZ_DecodeStream(ctx->img_base+j, buf, k, l);
		if(rsz)*rsz=l;
		return(buf);
	}
	
	return(NULL);
}

void ExWAD_StoreFile(ExWAD_Context *ctx, char *name, byte *buf, int sz)
{
	ExWAD_DirEnt *de;
	byte *cbuf;
	int i, j, k, l, b, di, csz, sz1, cm;

	di=ExWAD_GetDirEnt(ctx, name);
	if(di<0)return;

	de=&(ctx->dir[di]);

	j=exwad_getu32(de->rva_data);
	k=exwad_getu32(de->csize_data);
	
	if(j||k)
	{
		printf("ExWAD_StoreFile: free range, %08X..%08X, %d bytes\n",
			j, j+k, k);
		ExWAD_MarkRangeFree(ctx, j, k);
	}

	if(sz>=256)
	{
		cbuf=malloc(2*sz);
		csz=PDZ2_EncodeStream64(buf, cbuf, sz, sz*2);
		cm=EXWAD_DIR_CM_DEFLATE64;
		
		if(csz>0)
		{
			k=ExWAD_CRC32(buf, sz, (u32)-1);
//			cbuf[csz+0]=(k>>24)&0xFF;
//			cbuf[csz+1]=(k>>18)&0xFF;
//			cbuf[csz+2]=(k>>8)&0xFF;
//			cbuf[csz+3]=k&0xFF;

			cbuf[csz+0]=k&0xFF;
			cbuf[csz+1]=(k>>8)&0xFF;
			cbuf[csz+2]=(k>>18)&0xFF;
			cbuf[csz+3]=(k>>24)&0xFF;
			csz+=4;
		}
		
		if((csz<0) || (csz>=sz))
		{
			free(cbuf);
			cm=EXWAD_DIR_CM_STORE;
			csz=sz; cbuf=buf;
		}
	}else
	{
		cm=EXWAD_DIR_CM_STORE;
		csz=sz; cbuf=buf;
	}

	if(csz>0)
	{
		i=ExWAD_AllocRange(ctx, csz);
		b=i*EXWAD_CELLSZ;
	}else
	{
		i=0; b=0;
	}

	sz1=(sz>0)?sz:1;
	printf("ExWAD_StoreFile: Name=%s, Method=%d, Size %d->%d, Ratio=%d%%\n",
		name, cm, sz, csz, 100-(100*csz)/sz1);
	printf("ExWAD_StoreFile: Using Range, %08X..%08X, %d bytes\n",
		b, b+csz, csz);

	de=&(ctx->dir[di]);		//dir or image may have moved

	memcpy(ctx->img_base+b, cbuf, csz);
	exwad_setu32(de->rva_data, b);
	exwad_setu32(de->csize_data, csz);
	exwad_setu32(de->dsize_data, sz);
//	de->method=EXWAD_DIR_CM_STORE;
	de->method=cm;
	
	if(cm!=EXWAD_DIR_CM_STORE)
		{ free(cbuf); }
}

void ExWAD_AddFile(ExWAD_Context *ctx, char *name)
{
	byte *buf, *buf1;
	int i, j, k, n, sz;

	buf=bgbcc_loadfile(name, &sz);
	if(!buf)
	{
		printf("reading '%s' failed\n", name);
		return;
	}

	ExWAD_StoreFile(ctx, name, buf, sz);
	printf("added '%s'\n", name);
}
