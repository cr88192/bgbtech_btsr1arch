// byte *tk_ird_imgbuf=NULL;

char kerninit[256];

TK_FILE *tk_vf_freelist=NULL;
TK_FILE_VT *tk_fsty_root=NULL;
TK_MOUNT *tk_vf_mount=NULL;

int tk_vf_register(TK_FILE_VT *fsty)
{
	fsty->next=tk_fsty_root;
	tk_fsty_root=fsty;
}

int tk_vfile_init()
{
	static int init=0;
	
	if(init)return(0);
	init=1;
	
//	tk_ird_init();
	tk_fat_init();
	return(1);
}

TK_FILE *tk_alloc_file()
{
	TK_FILE *tmp;
	
	tmp=tk_vf_freelist;
	if(tmp)
	{
		tk_vf_freelist=tmp->udata0;
		memset(tmp, 0, sizeof(TK_FILE));
		return(tmp);
	}
	
	tmp=malloc(sizeof(TK_FILE));
	memset(tmp, 0, sizeof(TK_FILE));
	return(tmp);
}

void tk_free_file(TK_FILE *tmp)
{
	tmp->udata0=tk_vf_freelist;
	tk_vf_freelist=tmp;
}

// #include "tk_vf_ird.c"
#include "tk_vf_fat.c"

TK_FILE *tk_fopen(char *name, char *mode)
{
	TK_MOUNT *mnt;
	TK_FILE *fd;

	tk_vfile_init();

	if((name[0]=='.') && (name[1]=='/'))
		name+=2;
	if(*name=='/')
		name++;

//	fd=tk_ird_fopen(NULL, name, mode);
//	if(fd)return(fd);

	mnt=tk_vf_mount;
	while(mnt)
	{
		fd=mnt->vt->fopen(mnt, name, mode);
		if(fd)
			return(fd);
		mnt=mnt->next;
	}

	return(NULL);
}

int tk_fread(void *buf, int sz1, int sz2, TK_FILE *fd)
{
	if(fd->vt->fread)
		return(fd->vt->fread(buf, sz1, sz2, fd));
	return(-1);
}

int tk_fwrite(void *buf, int sz1, int sz2, TK_FILE *fd)
{
	if(fd->vt->fwrite)
		return(fd->vt->fwrite(buf, sz1, sz2, fd));
	return(-1);
}

int tk_fseek(TK_FILE *fd, int ofs, int rel)
{
	if(fd->vt->fseek)
		return(fd->vt->fseek(fd, ofs, rel));
	return(-1);
}

int tk_ftell(TK_FILE *fd)
{
	if(fd->vt->ftell)
		return(fd->vt->ftell(fd));
	return(-1);
}

int tk_fclose(TK_FILE *fd)
{
	if(fd->vt->fclose)
		return(fd->vt->fclose(fd));
	return(-1);
}

int tk_fgetc(TK_FILE *fd)
{
	if(fd->vt->fgetc)
		return(fd->vt->fgetc(fd));
	return(-1);
}

int tk_fputc(int ch, TK_FILE *fd)
{
	if(fd->vt->fputc)
		return(fd->vt->fputc(ch, fd));
	return(-1);
}
