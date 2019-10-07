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

int tk_fat_init();
int tk_mount_sdfat();

int tk_sysc_init();
int tk_mount_sysc();


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
	
	if(tk_iskernel())
	{
//		tk_ird_init();
		tk_fat_init();
	
		tk_mount_sdfat();
	}else
	{
		tk_sysc_init();	
		tk_mount_sysc();
	}

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
	return((TK_FILE *)tk_alloc_file());
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

int tk_fmount(char *src, char *dst, char *fsty, char *parm)
{
	return(-1);
}

int tk_unlink(char *name)
{
	return(-1);
}

int tk_rename(char *oldname, char *newname)
{
	return(-1);
}

int tk_fstat(char *name, TK_FSTAT *st)
{
	return(-1);
}

// #include "tk_vf_ird.c"
#include "tk_vf_fat.c"
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
		if(mnt->src)
		{
			if(strncmp(mnt->src, name, mnt->szSrc))
			{
				mnt=mnt->next;
				continue;
			}
			s1+=mnt->szSrc;
		}
	
//		fd=mnt->vt->fopen(mnt, name, mode);
		fd=mnt->vt->fopen(mnt, s1, mode);
		if(fd)
			return(fd);
		mnt=mnt->next;
	}

	tk_printf("tk_fopen: Fail %s\n", name);

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

s64 tk_fseek(TK_FILE *fd, s64 ofs, int rel)
{
	if(fd->vt->fseek)
		return(fd->vt->fseek(fd, ofs, rel));
	return(-1);
}

s64 tk_ftell(TK_FILE *fd)
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

int tk_fioctl(TK_FILE *fd, int cmd, void *ptr)
{
	if(fd->vt->fioctl)
		return(fd->vt->fioctl(fd, cmd, ptr));
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
		if(mnt->src)
		{
			if(strncmp(mnt->src, name, mnt->szSrc))
			{
				mnt=mnt->next;
				continue;
			}
			s1+=mnt->szSrc;
		}
	
//		fd=mnt->vt->opendir(mnt, name);
		fd=mnt->vt->opendir(mnt, s1);
		if(fd)
			return(fd);
		mnt=mnt->next;
	}

	tk_printf("tk_opendir: Fail %s\n", name);

	return(NULL);
}

TK_DIRENT *tk_readdir(TK_DIR *fd)
{
	if(fd->vt->readdir)
		return(fd->vt->readdir(fd));
	return(NULL);
}

int tk_closedir(TK_DIR *fd)
{
	if(fd->vt->closedir)
		return(fd->vt->closedir(fd));
	return(NULL);
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
