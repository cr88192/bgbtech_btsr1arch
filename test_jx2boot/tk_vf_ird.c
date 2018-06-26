TK_MOUNT *tk_ird_mount(char *devfn, char *mntfn,
	char *fsty, char *mode, char **opts);
TK_FILE *tk_ird_fopen(TK_MOUNT *mnt, char *name, char *mode);

int tk_ird_fread(void *buf, int sz1, int sz2, TK_FILE *fd);
int tk_ird_fseek(TK_FILE *fd, int ofs, int rel);
int tk_ird_ftell(TK_FILE *fd);
int tk_ird_fclose(TK_FILE *fd);
int tk_ird_fgetc(TK_FILE *fd);

TK_FILE_VT tk_vfile_ird_vt={
"initrd",			//fsname
NULL,				//next
tk_ird_mount,		//mount
tk_ird_fopen,		//fopen
NULL,				//fopendir
NULL,				//unlink
NULL,				//rename
NULL,				//fstat

/* FILE Ops */
tk_ird_fread,		//fread
NULL,				//fwrite
tk_ird_fseek,		//fseek
tk_ird_ftell,		//ftell
tk_ird_fclose,		//fclose
tk_ird_fgetc,		//fgetc
NULL				//fputc
};

int tk_ird_init()
{
	u32 reladdr, absa;
	byte *ird, *ird1;

	if(tk_ird_imgbuf)
		return(0);

	reladdr=P_INITRD_ADDR;
	if(!reladdr)
	{
		tk_puts("tk_ird_init: no initrd\n");
		tk_ird_imgbuf=NULL;
		return(-1);
	}else
	{
		tk_printf("tk_ird_init: OK %08X\n", reladdr);
	}
	
	tk_vf_register(&tk_vfile_ird_vt);
	
	strcpy(kerninit, P_KINIT_ADDR);
	
#ifdef ARCH_SH4
// #if 1
//	ird=(byte *)(0x90000000+reladdr);
	ird=(byte *)(0x8C000000U+reladdr);
#else
	ird=(byte *)(0x10000000+reladdr);
#endif
	tk_ird_imgbuf=ird;
//	__debugbreak();
	
	ird1=ird;

	if(ird1!=ird)
		__debugbreak();

	absa=(u32)ird1;

//	__debugbreak();

	tk_printf("tk_ird_init: IRD=%p\n", absa);
//	tk_printf("tk_ird_init: IRD=%p\n", tk_ird_imgbuf);
	return(1);
}

TK_MOUNT *tk_ird_mount(char *devfn, char *mntfn,
	char *fsty, char *mode, char **opts)
{
}

int tk_ird_decoctal_n(byte *cs, int n)
{
	int i, v;

	v=0;
	for(i=0; i<n; i++)
	{
		v=(v*8)+(cs[i]-'0');
	}
	return(v);
}

TK_FILE *tk_ird_fopen(TK_MOUNT *mnt, char *name, char *mode)
{
	TK_FILE *fd;
	u32 reladdr;
	byte *ird;
	byte *cs;
	int sz;
	
	if((mode[0]!='r') || (mode[1]=='+'))
	{
		tk_puts("tk_ird_fopen: bad mode\n");
		return(NULL);
	}

	ird=tk_ird_imgbuf;
	if(!ird)
	{
		tk_puts("tk_ird_fopen: no initrd\n");
		return(NULL);
	}
	
	cs=ird;
	while(cs[0])
	{
		sz=tk_ird_decoctal_n(cs+124, 11);

		if(!strcmp(cs, name))
		{
			fd=tk_alloc_file();
			fd->vt=&tk_vfile_ird_vt;
			fd->ram_base=cs+0x200;
			fd->ram_end=fd->ram_base+sz;
			fd->ram_ofs=fd->ram_base;
			
			tk_printf("tk_ird_fopen: %s %08X %08X\n", name,
				fd->ram_base, fd->ram_end);
			return(fd);
		}
		
		sz=(sz+511)&(~511);
		cs+=512+sz;
	}
	
	tk_printf("tk_ird_fopen: can't find %s\n", name);
	return(NULL);
}

int tk_ird_fseek(TK_FILE *fd, int ofs, int rel)
{
	if(fd->ram_base)
	{
		tk_printf("tk_ird_fseek: %p %d %d\n", fd, ofs, rel);

		if(rel==0)
		{
			if(ofs<0)
				return(-1);
			if(ofs>(fd->ram_end-fd->ram_base))
				return(-1);
			fd->ram_ofs=fd->ram_base+ofs;
			return(0);
		}

		if(rel==1)
		{
			fd->ram_ofs=fd->ram_ofs+ofs;
			if(fd->ram_ofs>fd->ram_end)
				fd->ram_ofs=fd->ram_end;
			if(fd->ram_ofs<fd->ram_base)
				fd->ram_ofs=fd->ram_base;
			return(0);
		}

		if(rel==2)
		{
			if(ofs>0)
				return(-1);
			if((-ofs)>(fd->ram_end-fd->ram_base))
				return(-1);
			fd->ram_ofs=fd->ram_end+ofs;
			return(0);
		}

		return(-1);
	}

	return(-1);
}

int tk_ird_fread(void *buf, int sz1, int sz2, TK_FILE *fd)
{
	int sz;

	if(fd->ram_base)
	{
		sz=sz1*sz2;
		if((fd->ram_ofs+sz)>fd->ram_end)
		{
			tk_printf("tk_ird_fread: EOF, rng=%p..%p ofs=%p sz=%d\n",
				fd->ram_base, fd->ram_end, fd->ram_ofs, sz);
			sz=fd->ram_end-fd->ram_ofs;
		}
		if(sz<=0)
		{
			tk_printf("tk_ird_fread: sz=%d, sz1=%d, sz2=%d\n", sz, sz1, sz2);
			return(0);
		}
		memcpy(buf, fd->ram_ofs, sz);
		fd->ram_ofs+=sz;
		return(sz);
	}

	tk_printf("tk_ird_fread: base=%p\n", fd->ram_base);
	return(-1);
}

int tk_ird_fgetc(TK_FILE *fd)
{
	int i;

	if(fd->ram_base)
	{
		if(fd->ram_ofs>=fd->ram_end)
			{ fd->ram_ofs=fd->ram_end; return(-1); }

		i=*fd->ram_ofs++;
		return(i);
	}

	return(-1);
}

int tk_ird_ftell(TK_FILE *fd)
{
	int i;
	if(fd->ram_base)
	{
		i=fd->ram_ofs-fd->ram_base;
		return(i);
	}

	return(-1);
}

int tk_ird_fclose(TK_FILE *fd)
{
	return(-1);
}

