// byte *tk_ird_imgbuf=NULL;

char kerninit[256];

void *tk_handle_arr[256];

TK_FILE *tk_vf_freelist=NULL;
TK_MOUNT *tk_mnt_freelist=NULL;

TK_FILE_VT *tk_fsty_root=NULL;
TK_MOUNT *tk_vf_mount=NULL;

int tk_vf_register(TK_FILE_VT *fsty)
{
	fsty->next=tk_fsty_root;
	tk_fsty_root=fsty;
}

int tk_vf_addmount(TK_MOUNT *mnt)
{
	TK_MOUNT *cur, *prv;
	
	mnt->szSrc=0;
	if(mnt->src)
		{ mnt->szSrc=strlen(mnt->src); }
	
	cur=tk_vf_mount; prv=NULL;
	while(cur)
	{
		if(mnt->szSrc>=cur->szSrc)
			break;
		prv=cur;
		cur=cur->next;
	}
	
	if(prv)
	{
		mnt->next=cur;
		prv->next=mnt;
	}else
	{
		mnt->next=tk_vf_mount;
		tk_vf_mount=mnt;
	}
}

TK_MOUNT *tk_vf_nextmount(TK_MOUNT *mnt)
{
	return(mnt->next);
}

TK_MOUNT *tk_vf_firstmount(void)
{
	return(tk_vf_mount);
}

int tk_fat_init();
int tk_mount_sdfat();

int tk_sysc_init();
int tk_mount_sysc();

int tk_wad4_init();


static int tk_vf_init=0;

int TK_FindFreeObjHandle()
{
	int i;
	
	for(i=1; i<256; i++)
	{
		if(!tk_handle_arr[i])
			return(i);
	}
	return(-1);
}

int TK_FreeObjHandle(int ix)
{
	tk_handle_arr[ix]=NULL;
	return(0);
}

int TK_LookupHandleForPtr(void *ptr)
{
	int i;
	
	if(!ptr)
		return(0);
	
	for(i=1; i<256; i++)
	{
		if(tk_handle_arr[i]==ptr)
			return(i);
	}
	return(-1);
}

int TK_GetHandleForPtr(void *ptr)
{
	int i;
	i=TK_LookupHandleForPtr(ptr);
	if(i>=0)
		return(i);
	i=TK_FindFreeObjHandle();
	if(i>0)
	{
		tk_handle_arr[i]=ptr;
		return(i);
	}
	return(-1);
}

void *TK_GetPtrForHandle(int ix)
{
	return(tk_handle_arr[ix]);
}

int tk_vfile_init()
{
	if(tk_vf_init==1)return(0);
	tk_vf_init=1;
	
	tk_puts("tk_vfile_init\n");
	
#ifndef __TK_CLIB_ONLY__
	if(tk_iskernel())
	{
		tk_devfs_init();
		tk_bdspi_init();
		tk_bdvfb_init();
		tk_bdram_init();
	
//		tk_ird_init();
		tk_fat_init();
		tk_wad4_init();

		tk_mount_wad4temp(NULL);
		tk_mount_sdfat("boot");

//		tk_mount_sdfat(NULL);
		tk_mount_devfs();
		
		tk_wad4_mount("boot/tkusrimg.wd4", "usr", "wad4", NULL, NULL);
	}else
	{
		tk_sysc_init();	
		tk_mount_sysc();
	}
#else
	tk_sysc_init();	
	tk_mount_sysc();
#endif

	tk_puts("tk_vfile_init: OK\n");

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

TK_DIR *tk_alloc_dir()
{
	return((TK_DIR *)tk_alloc_file());
}

void tk_free_file(TK_FILE *tmp)
{
	tmp->udata0=tk_vf_freelist;
	tk_vf_freelist=tmp;
}

void tk_free_dir(TK_DIR *tmp)
{
	tk_free_file((TK_FILE *)tmp);
//	tmp->udata0=tk_vf_freelist;
//	tk_vf_freelist=tmp;
}


TK_MOUNT *tk_alloc_mount()
{
	TK_MOUNT *tmp;
	
	tmp=tk_mnt_freelist;
	if(tmp)
	{
		tk_mnt_freelist=tmp->udata0;
		memset(tmp, 0, sizeof(TK_MOUNT));
		return(tmp);
	}
	
	tmp=malloc(sizeof(TK_MOUNT));
	memset(tmp, 0, sizeof(TK_MOUNT));
	return(tmp);
}

void tk_free_mount(TK_MOUNT *tmp)
{
	tmp->udata0=tk_mnt_freelist;
	tk_mnt_freelist=tmp;
}

int TK_VF_FlagsFromModeString(char *mode)
{
	int fl;
	
	fl=0;
	
	if(mode[0]=='r')
	{
		if(mode[1]='+')
		{
			fl|=TKVF_O_RDWR;
		}else
		{
			fl|=TKVF_O_RDONLY;
		}
	}else
		if(mode[0]=='w')
	{
		if(mode[1]='+')
		{
			fl|=TKVF_O_RDWR|TKVF_O_TRUNC|TKVF_O_CREAT;
		}else
		{
			fl|=TKVF_O_WRONLY|TKVF_O_TRUNC|TKVF_O_CREAT;
		}
	}else
		if(mode[0]=='a')
	{
		if(mode[1]='+')
		{
			fl|=TKVF_O_RDWR|TKVF_O_CREAT;
		}else
		{
			fl|=TKVF_O_WRONLY|TKVF_O_CREAT;
		}
	}
	
	return(fl);
}

int tk_fmount(char *devfn, char *mntfn, char *fsty, char *mode, char **opts)
{
	TK_FILE_VT *cur;
	TK_MOUNT *mnt;
	
	cur=tk_fsty_root;
	while(cur)
	{
		if(!strcmp(cur->fsname, fsty))
		{
			if(cur->mount)
			{
				mnt=cur->mount(devfn, mntfn, fsty, mode, opts);
				if(mnt)
					return(0);
			}
		}
		cur=cur->next;
	}
	return(-1);
}

int tk_unlink(char *name)
{
	TK_MOUNT *mnt;
	TK_FILE *fd;
	char *s1;
	int i;

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
		s1=name;
		if(mnt->src && (mnt->szSrc>0))
		{
			if(strncmp(mnt->src, name, mnt->szSrc))
			{
				mnt=mnt->next;
				continue;
			}
			s1+=mnt->szSrc;
			if(*s1 && (*s1!='/') && (*(s1-1)!=':'))
			{
				mnt=mnt->next;
				continue;
			}
			if(*s1=='/')
				s1++;
		}
	
//		fd=mnt->vt->fopen(mnt, name, mode);
		if(mnt->vt->unlink)
		{
			i=mnt->vt->unlink(mnt, s1);
			if(i>=0)
				return(i);
		}
		mnt=mnt->next;
	}

	tk_printf("tk_unlink: Fail %s\n", name);
	return(-1);
}

int tk_rmdir(char *name)
{
	TK_MOUNT *mnt;
	TK_FILE *fd;
	char *s1;
	int i;

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
		s1=name;
		if(mnt->src && (mnt->szSrc>0))
		{
			if(strncmp(mnt->src, name, mnt->szSrc))
			{
				mnt=mnt->next;
				continue;
			}
			s1+=mnt->szSrc;
			if(*s1 && (*s1!='/') && (*(s1-1)!=':'))
			{
				mnt=mnt->next;
				continue;
			}
			if(*s1=='/')
				s1++;
		}
	
//		fd=mnt->vt->fopen(mnt, name, mode);
		if(mnt->vt->rmdir)
		{
			i=mnt->vt->rmdir(mnt, s1);
			if(i>=0)
				return(i);
		}
		mnt=mnt->next;
	}

	tk_printf("tk_rmdir: Fail %s\n", name);
	return(-1);
}


int tk_mkdir(char *name, char *mode)
{
	TK_MOUNT *mnt;
	TK_FILE *fd;
	char *s1;
	int i;

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
		s1=name;
		if(mnt->src && (mnt->szSrc>0))
		{
			if(strncmp(mnt->src, name, mnt->szSrc))
			{
				mnt=mnt->next;
				continue;
			}
			s1+=mnt->szSrc;
			if(*s1 && (*s1!='/') && (*(s1-1)!=':'))
			{
				mnt=mnt->next;
				continue;
			}
			if(*s1=='/')
				s1++;
		}
	
		if(mnt->vt->mkdir)
		{
			i=mnt->vt->mkdir(mnt, s1, mode);
			if(i>=0)
				return(i);
		}
		mnt=mnt->next;
	}

	tk_printf("tk_mkdir: Fail %s\n", name);
	return(-1);
}

int tk_rename(char *oldname, char *newname)
{
	TK_MOUNT *mnt;
	TK_FILE *fd;
	char *s1, *t1;
	int i;

	tk_vfile_init();

	if((oldname[0]=='.') && (oldname[1]=='/'))
		oldname+=2;
	if(*oldname=='/')
		oldname++;

	if((newname[0]=='.') && (newname[1]=='/'))
		newname+=2;
	if(*oldname=='/')
		newname++;

//	fd=tk_ird_fopen(NULL, name, mode);
//	if(fd)return(fd);

	mnt=tk_vf_mount;
	while(mnt)
	{
		s1=oldname;
		t1=newname;
		if(mnt->src && (mnt->szSrc>0))
		{
			if(strncmp(mnt->src, oldname, mnt->szSrc) ||
				strncmp(mnt->src, newname, mnt->szSrc))
			{
				mnt=mnt->next;
				continue;
			}
			s1+=mnt->szSrc;
			t1+=mnt->szSrc;

			if(*s1 && (*s1!='/') && (*(s1-1)!=':'))
			{
				mnt=mnt->next;
				continue;
			}
			if(*s1=='/')
				s1++;

			if(*t1 && (*t1!='/') && (*(t1-1)!=':'))
			{
				mnt=mnt->next;
				continue;
			}
			if(*t1=='/')
				t1++;
		}
	
		if(mnt->vt->rename)
		{
	//		fd=mnt->vt->fopen(mnt, name, mode);
			i=mnt->vt->rename(mnt, s1, t1);
			if(i>=0)
				return(i);
		}
		mnt=mnt->next;
	}

	tk_printf("tk_rename: Fail %s %s\n", oldname, newname);
	return(-1);
}

int tk_fcopy(char *oldname, char *newname)
{
	char tb[1024];
	TK_FILE *oldfd, *newfd;
	int n;
	
	oldfd=tk_fopen(oldname, "rb");

	if(!oldfd)
	{
//		tk_printf("fail open %s\n", oldname);
		return(-1);
	}

	newfd=tk_fopen(newname, "wb");
	
	if(!newfd)
	{
//		tk_printf("fail open %s\n", newname);
		tk_fclose(oldfd);
		return(-1);
	}

//	while(!tk_feof(oldfd))
	while(1)
	{
		n=tk_fread(tb, 1, 1024, oldfd);
		if(n<1024)
		{
			if(n<=0)
				break;
			tk_fwrite(tb, 1, n, newfd);
			break;
		}
		tk_fwrite(tb, 1, 1024, newfd);
	}

	tk_fclose(oldfd);
	tk_fclose(newfd);
	return(1);
}

int tk_fstat(char *name, TK_FSTAT *st)
{
	return(-1);
}

#ifndef __TK_CLIB_ONLY__
// #include "tk_vf_ird.c"
#include "tk_vf_fat.c"
#endif

#include "tk_vf_sysc.c"

TK_FILE *tk_fopen(char *name, char *mode)
{
	TK_MOUNT *mnt;
	TK_FILE *fd;
	char *s1;

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
		s1=name;
		if(mnt->src && (mnt->szSrc>0))
		{
			if(strncmp(mnt->src, name, mnt->szSrc))
			{
				mnt=mnt->next;
				continue;
			}
			s1+=mnt->szSrc;
			if(*s1 && (*s1!='/') && (*(s1-1)!=':'))
			{
				mnt=mnt->next;
				continue;
			}
			if(*s1=='/')
				s1++;
		}
	
		if(mnt->vt->fopen)
		{
	//		fd=mnt->vt->fopen(mnt, name, mode);
			fd=mnt->vt->fopen(mnt, s1, mode);
			if(fd)
				return(fd);
		}
		mnt=mnt->next;
	}

//	tk_printf("tk_fopen: Fail %s\n", name);

	return(NULL);
}

int tk_hfopen(char *name, char *mode)
{
	FILE *fd;
	fd=tk_fopen(name, mode);
	if(!fd)
		return(0);
	return(TK_GetHandleForPtr(fd));
}

int tk_fread(void *buf, int sz1, int sz2, TK_FILE *fd)
{
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->fread)
		return(fd->vt->fread(buf, sz1, sz2, fd));
	return(-1);
}

int tk_fwrite(void *buf, int sz1, int sz2, TK_FILE *fd)
{
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->fwrite)
		return(fd->vt->fwrite(buf, sz1, sz2, fd));
	return(-1);
}

s64 tk_fseek(TK_FILE *fd, s64 ofs, int rel)
{
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->fseek)
		return(fd->vt->fseek(fd, ofs, rel));
	return(-1);
}

s64 tk_ftell(TK_FILE *fd)
{
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->ftell)
		return(fd->vt->ftell(fd));
	return(-1);
}

int tk_fclose(TK_FILE *fd)
{
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->fclose)
		return(fd->vt->fclose(fd));
	return(-1);
}

int tk_fgetc(TK_FILE *fd)
{
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->fgetc)
		return(fd->vt->fgetc(fd));
	return(-1);
}

int tk_fputc(int ch, TK_FILE *fd)
{
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->fputc)
		return(fd->vt->fputc(ch, fd));
	return(-1);
}

int tk_fioctl(TK_FILE *fd, int cmd, void *ptr)
{
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->fioctl)
		return(fd->vt->fioctl(fd, cmd, ptr));
	return(-1);
}

int tk_fsend(TK_FILE *fd, int cmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr)
{
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->fsend)
		return(fd->vt->fsend(fd, cmd,
			msgbuf, szmsg, flag,
			sockaddr, szsockaddr));
	return(-1);
}

int tk_frecv(TK_FILE *fd, int cmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr)
{
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->frecv)
		return(fd->vt->frecv(fd, cmd,
			msgbuf, szmsg, flag,
			sockaddr, szsockaddr));
	return(-1);
}

int tk_hread(int iHdl, void *pBuf, int szBuf)
{
	FILE *fd;
	int i, sde;

	fd=TK_GetPtrForHandle(iHdl);
	if(!fd)
		return(-1);

#if 0
	if(szBuf==1)
	{
		i=tk_fgetc(fd);
		if(i<0)
			return(0);
		*(byte *)pBuf=i;
		return(1);
	}
#endif

	return(tk_fread(pBuf, 1, szBuf, fd));
}

int tk_hwrite(int iHdl, void *pBuf, int szBuf)
{
	FILE *fd;
	int i, j, sde;

	fd=TK_GetPtrForHandle(iHdl);
	if(!fd)
		return(-1);

#if 0
	if(szBuf==1)
	{
		i=*(byte *)pBuf;
		j=tk_fputc(i, fd);
		if(j<=0)
			return(0);
		return(1);
	}
#endif

	return(tk_fwrite(pBuf, 1, szBuf, fd));
}

s64 tk_hseek(int iHdl, s64 lOffs, int iMod)
{
	FILE *fd;
	int i, sde;

	fd=TK_GetPtrForHandle(iHdl);
	if(!fd)
		return(-1);

	if(!lOffs)
	{
		if(iMod==1)
		{
			return(tk_ftell(fd));
		}
	}

	return(tk_fseek(fd, lOffs, iMod));
}

int tk_hclose(int iHdl)
{
	FILE *fd;
	int i, sde;

	fd=TK_GetPtrForHandle(iHdl);
	if(!fd)
		return(-1);
	return(tk_fclose(fd));
}

s64 tk_hioctl(int iHdl, int iCmd, void *ptr)
{
	FILE *fd;
	int i, sde;

	fd=TK_GetPtrForHandle(iHdl);
	if(!fd)
		return(-1);
	return(tk_fioctl(fd, iCmd, ptr));
}

s64 tk_hsend(int iHdl, int iCmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr)
{
	FILE *fd;
	int i, sde;

	fd=TK_GetPtrForHandle(iHdl);
	if(!fd)
		return(-1);
	return(tk_fsend(fd, iCmd,
		msgbuf, szmsg, flag,
		sockaddr, szsockaddr));
}

s64 tk_hrecv(int iHdl, int iCmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr)
{
	FILE *fd;
	int i, sde;

	fd=TK_GetPtrForHandle(iHdl);
	if(!fd)
		return(-1);
	return(tk_frecv(fd, iCmd,
		msgbuf, szmsg, flag,
		sockaddr, szsockaddr));
}

TK_DIR *tk_opendir(char *name)
{
	TK_MOUNT *mnt;
	TK_DIR *fd;
	char *s1;

	tk_vfile_init();

	if((name[0]=='.') && (name[1]=='/'))
		name+=2;
	if(*name=='/')
		name++;

	mnt=tk_vf_mount;
	while(mnt)
	{
		s1=name;
		if(mnt->src && (mnt->szSrc>0))
		{
//			tk_printf("tk_opendir: Src=%s\n", mnt->src);

			if(strncmp(mnt->src, name, mnt->szSrc))
			{
				mnt=mnt->next;
				continue;
			}
			s1+=mnt->szSrc;
			if(*s1 && (*s1!='/') && (*(s1-1)!=':'))
			{
				mnt=mnt->next;
				continue;
			}
			if(*s1=='/')
				s1++;
		}

//		tk_printf("tk_opendir: Check %s\n", mnt->src);
	
		if(mnt->vt->opendir)
		{
	//		fd=mnt->vt->opendir(mnt, name);
			fd=mnt->vt->opendir(mnt, s1);
			if(fd)
				return(fd);
		}
		mnt=mnt->next;
	}

	tk_printf("tk_opendir: Fail %s\n", name);

	return(NULL);
}

TK_DIRENT *tk_readdir(TK_DIR *fd)
{
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->readdir)
		return(fd->vt->readdir(fd));
	return(NULL);
}

int tk_closedir(TK_DIR *fd)
{
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->closedir)
		return(fd->vt->closedir(fd));
	return(-1);
}


int tk_hopendir(char *name)
{
	FILE *fd;
	fd=tk_opendir(name);
	return(TK_GetHandleForPtr(fd));
}

int tk_hreaddir(int iHdl, void *pDe, int szDe, int nDe)
{
	TK_DIRENT *de;
	void *de1;
	FILE *fd;
	int i, sde;

	fd=TK_GetPtrForHandle(iHdl);
	if(!fd)
		return(-1);
	
	sde=sizeof(TK_DIRENT);
	if(szDe<sde)sde=szDe;
	
	memset(pDe, 0, szDe*nDe);

	for(i=0; i<nDe; i++)
	{
		de=tk_readdir(fd);
		if(!de)
			break;
		de1=((byte *)pDe)+(i*szDe);
		memcpy(de1, de, sde);
	}
	return(i);

//	fd=tk_opendir(name);
//	return(TK_GetHandleForPtr(fd));
}

int tk_hclosedir(int iHdl)
{
	TK_DIRENT *de;
	void *de1;
	FILE *fd;
	int i, sde;

	fd=TK_GetPtrForHandle(iHdl);
	if(!fd)
		return(-1);
	return(tk_closedir(fd));
}
