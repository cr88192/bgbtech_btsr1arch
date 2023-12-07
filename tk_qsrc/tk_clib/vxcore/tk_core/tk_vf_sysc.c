TK_MOUNT *tk_sysc_mount(char *devfn, char *mntfn,
	char *fsty, char *mode, char **opts);
TK_FILE *tk_sysc_fopen(TK_MOUNT *mnt, TK_USERINFO *usri, char *name, char *mode);
TK_DIR *tk_sysc_opendir(TK_MOUNT *mnt, TK_USERINFO *usri, char *name);

int tk_sysc_rename(TK_MOUNT *mnt, TK_USERINFO *usri,
	char *oldname, char *newname, char *mode);

int tk_sysc_fread(void *buf, int sz1, int sz2, TK_FILE *fd);
int tk_sysc_fwrite(void *buf, int sz1, int sz2, TK_FILE *fd);
s64 tk_sysc_fseek(TK_FILE *fd, s64 ofs, int rel);
s64 tk_sysc_ftell(TK_FILE *fd);
int tk_sysc_fclose(TK_FILE *fd);
int tk_sysc_fgetc(TK_FILE *fd);
int tk_sysc_fputc(int ch, TK_FILE *fd);
int tk_sysc_fioctl(TK_FILE *fd, int cmd, void *ptr);

int tk_sysc_fsend(TK_FILE *fd, int cmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr);
int tk_sysc_frecv(TK_FILE *fd, int cmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr);

TK_DIRENT *tk_sysc_readdir(TK_DIR *fd);
int tk_sysc_closedir(TK_DIR *fd);

TK_FILE_VT tk_vfile_sysc_vt={
"syscall",			//fsname
NULL,				//next
tk_sysc_mount,		//mount
tk_sysc_fopen,		//fopen
tk_sysc_opendir,	//fopendir
NULL,				//unlink
tk_sysc_rename,		//rename
NULL,				//fstat

NULL,				//mkdir
NULL,				//rmdir
NULL,				//fsctl

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
tk_sysc_readdir,	//readdir
tk_sysc_closedir,	//closedir

/* Socket/Device Ops */
tk_sysc_fsend,		//fsend
tk_sysc_frecv		//frecv
};

int tk_sysc_init()
{
	tk_vf_register(&tk_vfile_sysc_vt);
	return(1);
}

int tk_mount_sysc()
{
	TK_MOUNT *mnt;

	tk_dbg_printf("tk_mount_sysc\n");

	mnt=tk_alloc_mount();
	mnt->vt=&tk_vfile_sysc_vt;
//	mnt->udata0=img;

	if(mnt->vt->fopen!=tk_sysc_fopen)
		{ __debugbreak(); }
	
//	mnt->next=tk_vf_mount;
//	tk_vf_mount=mnt;
	tk_vf_addmount(mnt);
	
	if(mnt->vt->fopen!=tk_sysc_fopen)
		{ __debugbreak(); }
}

TK_MOUNT *tk_sysc_mount(char *devfn, char *mntfn,
	char *fsty, char *mode, char **opts)
{
}

TK_FILE *tk_sysc_fopen(TK_MOUNT *mnt, TK_USERINFO *usri, char *name, char *mode)
{
	char tfname[512];
	TK_SysArg ar[4];
	TK_FILE *fd;
	void *p;
	int i;
	
//	TK_Env_GetCwdQualifyName(tfname, 512, name);
	strcpy(tfname, name);
	
	tk_dbg_printf("tk_sysc_fopen: %s -> %s\n", name, tfname);
	
	p=NULL;
//	ar[0].p=name;
	ar[0].p=tfname;
	ar[1].p=mode;
	i=tk_syscall(NULL, TK_UMSG_VFOPEN, &p, ar);
	if(i<=0)
		return(NULL);

	fd=tk_alloc_file();
	fd->vt=&tk_vfile_sysc_vt;
	fd->ifd=i;
	
	return(fd);
}

TK_DIR *tk_sysc_opendir(TK_MOUNT *mnt, TK_USERINFO *usri, char *name)
{
	char tfname[512];
	TK_SysArg ar[4];
	TK_DIR *fd;
	TK_DIRENT *tde;
	void *p;
	int i;

//	TK_Env_GetCwdQualifyName(tfname, 512, name);
	strcpy(tfname, name);

	tk_dbg_printf("tk_sysc_opendir: %s -> %s\n", name, tfname);
	
	p=NULL;
//	ar[0].p=name;
	ar[0].p=tfname;
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

int tk_sysc_rename(TK_MOUNT *mnt, TK_USERINFO *usri,
	char *oldname, char *newname, char *mode)
{
	char tfoname[512];
	char tfnname[512];
	TK_SysArg ar[4];
	TK_FILE *fd;
	void *p;
	int i;
	
//	if(*mode=='S')
//		{ strcpy(tfoname, oldname); }
//	else
//		{ TK_Env_GetCwdQualifyName(tfoname, 512, oldname); }
//	TK_Env_GetCwdQualifyName(tfnname, 512, newname);

	strcpy(tfoname, oldname);
	strcpy(tfnname, newname);
	
//	tk_dbg_printf("tk_sysc_fopen: %s -> %s\n", name, tfname);
	
	p=NULL;
//	ar[0].p=name;
	ar[0].p=tfoname;
	ar[1].p=tfnname;
	ar[2].p=mode;
	i=tk_syscall(NULL, TK_UMSG_VFRENAME, &p, ar);
	if(i<=0)
		return(-1);

	return(i);
}

int tk_sysc_fclose(TK_FILE *fd)
{
	TK_SysArg ar[4];
	void *p;
	int i;
	
	p=NULL;
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
	
	p=NULL;
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
	
	p=NULL;
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
	
	p=NULL;
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
	
	li=0;
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
	
	li=0;
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
	
	p=NULL;
	ar[0].i=fd->ifd;
	ar[1].i=cmd;
	ar[2].p=ptr;
	i=tk_syscall(NULL, TK_UMSG_VFIOCTL, &p, ar);
	return(i);
}

int tk_sysc_fsend(TK_FILE *fd, int cmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr)
{
	TK_SysArg ar[8];
	void *p;
	int i;
	
	p=NULL;
	ar[0].i=fd->ifd;
	ar[1].i=cmd;
	ar[2].p=msgbuf;
	ar[3].i=szmsg;
	ar[4].i=flag;
	ar[5].p=sockaddr;
	ar[6].i=szsockaddr;
	i=tk_syscall(NULL, TK_UMSG_VFSEND, &p, ar);
	return(i);
}

int tk_sysc_frecv(TK_FILE *fd, int cmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr)
{
	TK_SysArg ar[8];
	void *p;
	int i;
	
	p=NULL;
	ar[0].i=fd->ifd;
	ar[1].i=cmd;
	ar[2].p=msgbuf;
	ar[3].i=szmsg;
	ar[4].i=flag;
	ar[5].p=sockaddr;
	ar[6].i=szsockaddr;
	i=tk_syscall(NULL, TK_UMSG_VFRECV, &p, ar);
	return(i);
}

TK_DIRENT *tk_sysc_readdir(TK_DIR *fd)
{
	TK_SysArg ar[4];
	void *p;
	int i;
	
	p=NULL;
	ar[0].i=fd->ifd;
	ar[1].p=fd->udata2;
	ar[2].i=sizeof(TK_DIRENT);
	ar[3].i=1;
	i=tk_syscall(NULL, TK_UMSG_VFREADDIR, &p, ar);
	if(i<=0)
		return(NULL);
	return(fd->udata2);
}


int tk_multidir_closedir(TK_DIR *fd);
TK_DIRENT *tk_multidir_readdir(TK_DIR *fd);
int tk_multidir_fioctl(TK_FILE *fd, int cmd, void *ptr);

TK_FILE_VT tk_vfile_multidir_vt={
"multidir",			//fsname
NULL,				//next
NULL,				//mount
NULL,				//fopen
NULL,				//fopendir
NULL,				//unlink
NULL,				//rename
NULL,				//fstat

NULL,				//mkdir
NULL,				//rmdir
NULL,				//fsctl

/* FILE Ops */
NULL,				//fread
NULL,				//fwrite
NULL,				//fseek
NULL,				//ftell
NULL,				//fclose
NULL,				//fgetc
NULL,				//fputc
tk_multidir_fioctl,		//ioctl

/* DIR ops */
tk_multidir_readdir,	//readdir
tk_multidir_closedir,	//closedir

/* Socket/Device Ops */
NULL,				//fsend
NULL				//frecv
};

struct tk_multidir_mergecache_s {
u64 win[256];
byte chn[256];
byte hash[32];
byte wpos;
};

u64 tk_multidir_hashname(char *str)
{
	char *cs;
	u64 v;
	int i;
	
	cs=str; v=0;
	while(*cs)
	{
		i=*cs++;
		v=v*0xE20B7AC6ULL+i;
	}
	v=v*0xE20B7AC6ULL;
	return(v);
}

TK_DIR *tk_multidir_create(TK_DIR **fda, int nfd)
{
	TK_DIR **tfda;
	TK_DIRENT *tde;
	struct tk_multidir_mergecache_s *mce;
	TK_DIR *fd;
	int i, j, k;

	void *p;
//	int i;

//	tde=tk_malloc(sizeof(TK_DIRENT));
//	memset(tde, 0, sizeof(TK_DIRENT));
	tde=NULL;

	mce=tk_malloc(sizeof(struct tk_multidir_mergecache_s));
	memset(mce, 0, sizeof(struct tk_multidir_mergecache_s));

	fd=tk_alloc_dir();
	fd->vt=&tk_vfile_multidir_vt;

	if((nfd*sizeof(void *))<=TK_FILE_SZTFDA)
		{ tfda=(TK_DIR **)(fd->tfda); }
	else
		{ tfda=tk_malloc(nfd*sizeof(void *)); }

	for(i=0; i<nfd; i++)
		{ tfda[i]=fda[i]; }

	fd->ifd=nfd;
	fd->ipos=0;
	fd->udata1=tfda;
	fd->udata2=tde;
	fd->udata3=mce;
	
	return(fd);
}

int tk_multidir_closedir(TK_DIR *fd)
{
	TK_DIR **fda;
	void *p;
	int i, nfd;
	
	fda=fd->udata1;
	nfd=fd->ifd;
	
	for(i=0; i<nfd; i++)
		{ tk_closedir(fda[i]); }
	if((nfd*sizeof(void *))>TK_FILE_SZTFDA)
		{ tk_free(fda); }
	if(fd->udata3)
		{ tk_free(fd->udata3); }
	tk_free_dir(fd);
	return(i);
}

TK_DIRENT *tk_multidir_readdir(TK_DIR *fd)
{
	struct tk_multidir_mergecache_s *mce;
	TK_DIR **fda;
	TK_DIRENT *tde;
	void *p;
	u64 v;
	int i, j, k, h, c, nfd;
	
	fda=fd->udata1;
	tde=fd->udata2;
	mce=fd->udata3;
	nfd=fd->ifd;

	i=fd->ipos; tde=NULL;
	while(!tde && (i<nfd))
	{
		tde=tk_readdir(fda[i]);
		if(!tde)i++;
		
		/* Try to merge name collisions. */
		if(tde && mce)
		{
			v=tk_multidir_hashname(tde->d_name);
			h=(v>>32)&31;
			j=mce->hash[h]; c=32;
			while(j && (c>0))
			{
				if(mce->win[j]==v)
					break;
				j=mce->chn[j];
			}
			if(j && (c>0))
			{
				tde=NULL;
			}else
			{
				j=(mce->wpos++)&255;
				while(!j)
					{ j=(mce->wpos++)&255; }
				mce->win[j]=v;
				mce->chn[j]=mce->hash[h];
				mce->hash[h]=j;
			}
		}
	}
	fd->ipos=i;
	
	return(tde);
}

int tk_multidir_fioctl(TK_FILE *fd, int cmd, void *ptr)
{
	return(-1);
}

