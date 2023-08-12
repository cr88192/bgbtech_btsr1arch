int tkucc_getmodeforcmd(char *cmd)
{
	char *s;
	
	s=cmd+strlen(cmd)-3;
	
	if(!strcmp(s, "-cc") || !strcmp(s, "/cc"))
		return(1);
	if(!strcmp(s, "-as") || !strcmp(s, "/as"))
		return(2);
	if(!strcmp(s, "-ld") || !strcmp(s, "/ld"))
		return(3);
	return(0);
}

char *TKUCC_GetFileExtension(char *cmd)
{
	char *s;
	
	s=cmd+strlen(cmd);
	while((s>cmd) && (*s!='.'))
		s--;
	if(*s=='.')
		return(s);
	return(NULL);
}

u32 TKUCC_GetFileExtType(char *cmd)
{
	char *ext;
	ext=TKUCC_GetFileExtension(cmd);

	if(!ext)
		return(0);
	
	if(!tkucc_stricmp(ext, ".c"))
		return(TKUCC_FCC_C);
	if(!tkucc_stricmp(ext, ".s"))
		return(TKUCC_FCC_S);
	if(!tkucc_stricmp(ext, ".o"))
		return(TKUCC_FCC_O);
	if(!tkucc_stricmp(ext, ".a"))
		return(TKUCC_FCC_A);

	if(!tkucc_stricmp(ext, ".exe"))
		return(TKUCC_FCC_EXE);
	if(!tkucc_stricmp(ext, ".dll"))
		return(TKUCC_FCC_DLL);

	return(0);
}

int TKUCC_LoadFile(char *name, byte **rbuf, int *rsz)
{
	FILE *fd;
	byte *tbuf;
	int tsz;
	
	*rbuf=NULL;
	*rsz=0;
	
	fd=fopen(name, "rb");
	if(!fd)
		return(-1);
	
	fseek(fd, 0, 2);
	tsz=ftell(fd);
	fseek(fd, 0, 0);
	tbuf=tkucc_malloc(tsz);
	fread(tbuf, 1, tsz, fd);
	fclose(fd);

	*rbuf=tbuf;
	*rsz=tsz;
	return(0);
}

int TKUCC_StoreFile(char *name, byte *buf, int sz)
{
	FILE *fd;
	
	fd=fopen(name, "wb");
	if(!fd)
		return(-1);
	fwrite(buf, 1, sz, fd);
	fclose(fd);
	return(0);
}

int TKUCC_LoadFileIPath(TKUCC_MainContext *ctx,
	char *name, byte **rbuf, int *rsz)
{
	char tb[256];
	byte *tbuf;
	int tsz;
	int i;
	
	tbuf=NULL;
	tsz=0;

	TKUCC_LoadFile(name, &tbuf, &tsz);
	
	if(tbuf)
	{
		*rbuf=tbuf;
		*rsz=tsz;
		return(0);
	}

	if(!tbuf)
	{
		for(i=0; i<ctx->pp_n_incpath; i++)
		{
			sprintf(tb, "%s/%s", ctx->pp_incpath[i], name);
			TKUCC_LoadFile(tb, &tbuf, &tsz);
			if(tbuf)
				break;
		}
	}

	*rbuf=tbuf;
	*rsz=tsz;
	return(0);
}

int main(int argc, char **argv)
{
	char *infiles[256];
	u32 infileext[256];
	TKUCC_MainContext *ctx;
	TKUCC_LinkObject *obj;
	char *outfile;
	byte *tobjbuf;
	int tobjsz;
	u32 outfileext;
	u64 modeopts;
	int md, n_infiles;
	int i, j, k;
	
	md=tkucc_getmodeforcmd(argv[0]);
	
	outfile=NULL;
	modeopts=0;
	n_infiles=0;

	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "-o"))
			{
				outfile=argv[i+1];
				i++;
				continue;
			}

			if(!strcmp(argv[i], "-c"))
			{
				modeopts|=1;
				continue;
			}
		
			continue;
		}
		
		infiles[n_infiles++]=argv[i];
	}
	
	if(!n_infiles)
	{
		printf("%s: no input files\n", argv[0]);
		return(1);
	}
	
	if(!outfile)
	{
		outfile="a.exe";
		if(modeopts&1)
			outfile="a.o";
	}
	
	for(i=0; i<n_infiles; i++)
		{ infileext[i]=TKUCC_GetFileExtType(infiles[i]); }
	outfileext=TKUCC_GetFileExtType(outfile);


	ctx=TKUCC_AllocMainContext();

	for(i=0; i<n_infiles; i++)
	{
		if(infileext[i]==TKUCC_FCC_S)
		{
			TKUCC_DoAssembleFile(ctx, infiles[i]);
		}
	}

	for(i=0; i<n_infiles; i++)
	{
		if(infileext[i]==TKUCC_FCC_O)
		{
			tobjbuf=NULL; tobjsz=0;
			TKUCC_LoadFileIPath(ctx, infiles[i], &tobjbuf, &tobjsz);
			TKUCC_LinkDoLoadObj(ctx, tobjbuf, tobjsz);
		}

		if(infileext[i]==TKUCC_FCC_A)
		{
			tobjbuf=NULL; tobjsz=0;
			TKUCC_LoadFileIPath(ctx, infiles[i], &tobjbuf, &tobjsz);
			TKUCC_LinkDoLoadLib(ctx, tobjbuf, tobjsz);
		}
	}
	
	obj=TKUCC_DoLinkObjectsStage(ctx);
	tobjbuf=NULL; tobjsz=0;

	if((outfileext==TKUCC_FCC_EXE) || (outfileext==TKUCC_FCC_DLL) ||
		!outfileext)
	{
		TKUCC_LinkFlattenObjPEL(ctx, obj, &tobjbuf, &tobjsz);
	}
	
	if(outfileext==TKUCC_FCC_O)
	{
		TKUCC_LinkFlattenObjWoff(ctx, obj, &tobjbuf, &tobjsz);
	}

	if(tobjbuf)
	{
		TKUCC_StoreFile(outfile, tobjbuf, tobjsz);
	}

	return(0);
}
