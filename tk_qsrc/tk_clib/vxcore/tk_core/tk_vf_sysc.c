TK_MOUNT *tk_sysc_mount(char *devfn, char *mntfn,
	char *fsty, char *mode, char **opts);
TK_FILE *tk_sysc_fopen(TK_MOUNT *mnt, char *name, char *mode);
TK_DIR *tk_sysc_opendir(TK_MOUNT *mnt, char *name);

int tk_sysc_fread(void *buf, int sz1, int sz2, TK_FILE *fd);
int tk_sysc_fwrite(void *buf, int sz1, int sz2, TK_FILE *fd);
s64 tk_sysc_fseek(TK_FILE *fd, s64 ofs, int rel);
s64 tk_sysc_ftell(TK_FILE *fd);
int tk_sysc_fclose(TK_FILE *fd);
int tk_sysc_fgetc(TK_FILE *fd);
int tk_sysc_fputc(int ch, TK_FILE *fd);
int tk_sysc_fioctl(TK_FILE *fd, int cmd, void *ptr);

TK_DIRENT *tk_sysc_readdir(TK_DIR *fd);
int tk_sysc_closedir(TK_DIR *fd);

TK_FILE_VT tk_vfile_sysc_vt={
"syscall",			//fsname
NULL,				//next
tk_sysc_mount,		//mount
tk_sysc_fopen,		//fopen
tk_sysc_opendir,	//fopendir
NULL,				//unlink
NULL,				//rename
NULL,				//fstat

/* FILE Ops */
tk_sysc_fread,		//fread
tk_sysc_fwrite,		//fwrite
tk_sysc_fseek,		//fseek
tk_sysc_ftell,		//ftell
tk_sysc_fclose,		//fclose
tk_sysc_fgetc,		//fgetc
tk_sysc_fputc,		//fputc
tk_sysc_fioctl,		//ioctl

/* DIR ops */
tk_sysc_readdir,		//readdir
tk_sysc_closedir		//closedir
};

int tk_sysc_init()
{
	tk_vf_register(&tk_vfile_sysc_vt);
	return(1);
}

int tk_mount_sysc()
{
	TK_MOUNT *mnt;

	mnt=tk_alloc_mount();
	mnt->vt=&tk_vfile_sysc_vt;
//	mnt->udata0=img;
	
//	mnt->next=tk_vf_mount;
//	tk_vf_mount=mnt;
	tk_vf_addmount(mnt);
}

TK_MOUNT *tk_sysc_mount(char *devfn, char *mntfn,
	char *fsty, char *mode, char **opts)
{
}

TK_FILE *tk_sysc_fopen(TK_MOUNT *mnt, char *name, char *mode)
{
	TK_SysArg ar[4];
	TK_FILE *fd;
	void *p;
	int i;
	
	ar[0].p=name;
	ar[1].p=mode;
	i=tk_syscall(NULL, TK_UMSG_VFOPEN, &p, ar);
	if(i<=0)
		return(NULL);

	fd=tk_alloc_file();
	fd->vt=&tk_vfile_sysc_vt;
	fd->ifd=i;
	
	return(fd);
}

TK_DIR *tk_sysc_opendir(TK_MOUNT *mnt, char *name)
{
	TK_SysArg ar[4];
	TK_DIR *fd;
	TK_DIRENT *tde;
	void *p;
	int i;
	
	ar[0].p=name;
	i=tk_syscall(NULL, TK_UMSG_VFOPENDIR, &p, ar);
	if(i<=0)
		return(NULL);

	tde=malloc(sizeof(TK_DIRENT));
	memset(tde, 0, sizeof(TK_DIRENT));

	fd=tk_alloc_dir();
	fd->vt=&tk_vfile_sysc_vt;
	fd->ifd=i;
	fd->udata2=tde;
	
	return(fd);
}

int tk_sysc_fclose(TK_FILE *fd)
{
	TK_SysArg ar[4];
	void *p;
	int i;
	
	ar[0].i=fd->ifd;
	i=tk_syscall(NULL, TK_UMSG_VFCLOSE, &p, ar);
	tk_free_file(fd);
	return(i);
}

int tk_sysc_closedir(TK_DIR *fd)
{
	TK_SysArg ar[4];
	void *p;
	int i;
	
	ar[0].i=fd->ifd;
	i=tk_syscall(NULL, TK_UMSG_VFCLOSEDIR, &p, ar);
	free(fd->udata2);
	tk_free_dir(fd);
	return(i);
}

int tk_sysc_fread(void *buf, int sz1, int sz2, TK_FILE *fd)
{
	TK_SysArg ar[4];
	void *p;
	int i;
	
	ar[0].i=fd->ifd;
	ar[1].p=buf;
	ar[2].i=sz1*sz2;
	i=tk_syscall(NULL, TK_UMSG_VFREAD, &p, ar);
	return(i);
}

int tk_sysc_fwrite(void *buf, int sz1, int sz2, TK_FILE *fd)
{
	TK_SysArg ar[4];
	void *p;
	int i;
	
	ar[0].i=fd->ifd;
	ar[1].p=buf;
	ar[2].i=sz1*sz2;
	i=tk_syscall(NULL, TK_UMSG_VFWRITE, &p, ar);
	return(i);
}

s64 tk_sysc_fseek(TK_FILE *fd, s64 ofs, int rel)
{
	TK_SysArg ar[4];
	s64 li;
	void *p;
	int i;
	
	ar[0].i=fd->ifd;
	ar[1].l=ofs;
	ar[2].i=rel;
	i=tk_syscall(NULL, TK_UMSG_VFSEEK, &li, ar);
	return(li);
}

s64 tk_sysc_ftell(TK_FILE *fd)
{
	TK_SysArg ar[4];
	s64 li;
	void *p;
	int i;
	
	ar[0].i=fd->ifd;
	ar[1].l=0;
	ar[2].i=1;
	i=tk_syscall(NULL, TK_UMSG_VFSEEK, &li, ar);
	return(li);
}

int tk_sysc_fgetc(TK_FILE *fd)
{
	byte tb[8];
	int i;

	i=tk_sysc_fread(tb, 1, 1, fd);
	if(i<=0)
		return(-1);
	return(tb[0]);
}

int tk_sysc_fputc(int ch, TK_FILE *fd)
{
	byte tb[8];
	int i;

	tb[0]=ch;
	i=tk_sysc_fwrite(tb, 1, 1, fd);
	if(i<=0)
		return(-1);
	return(ch);
}

int tk_sysc_fioctl(TK_FILE *fd, int cmd, void *ptr)
{
	TK_SysArg ar[4];
	void *p;
	int i;
	
	ar[0].i=fd->ifd;
	ar[1].i=cmd;
	ar[2].p=ptr;
	i=tk_syscall(NULL, TK_UMSG_VFIOCTL, &p, ar);
	return(i);
}

TK_DIRENT *tk_sysc_readdir(TK_DIR *fd)
{
	TK_SysArg ar[4];
	void *p;
	int i;
	
	ar[0].i=fd->ifd;
	ar[1].p=fd->udata2;
	ar[2].i=sizeof(TK_DIRENT);
	ar[3].i=1;
	i=tk_syscall(NULL, TK_UMSG_VFREADDIR, &p, ar);
	if(i<=0)
		return(NULL);
	return(i);
}
