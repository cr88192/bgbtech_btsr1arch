TK_MOUNT *tk_wad4_mount(char *devfn, char *mntfn,
	char *fsty, char *mode, char **opts);
TK_FILE *tk_wad4_fopen(TK_MOUNT *mnt, char *name, char *mode);
TK_DIR *tk_wad4_opendir(TK_MOUNT *mnt, char *name);

int tk_wad4_fread(void *buf, int sz1, int sz2, TK_FILE *fd);
int tk_wad4_fwrite(void *buf, int sz1, int sz2, TK_FILE *fd);
s64 tk_wad4_fseek(TK_FILE *fd, s64 ofs, int rel);
s64 tk_wad4_ftell(TK_FILE *fd);
int tk_wad4_fclose(TK_FILE *fd);
int tk_wad4_fgetc(TK_FILE *fd);
int tk_wad4_fputc(int ch, TK_FILE *fd);
int tk_wad4_fioctl(TK_FILE *fd, int cmd, void *ptr);

int tk_wad4_fsend(TK_FILE *fd, int cmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr);
int tk_wad4_frecv(TK_FILE *fd, int cmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr);

TK_DIRENT *tk_wad4_readdir(TK_DIR *fd);
int tk_wad4_closedir(TK_DIR *fd);

TK_FILE_VT tk_vfile_wad4_vt={
"wadfs",			//fsname
NULL,				//next
tk_wad4_mount,		//mount
tk_wad4_fopen,		//fopen
tk_wad4_opendir,	//fopendir
NULL,				//unlink
NULL,				//rename
NULL,				//fstat

NULL,				//mkdir
NULL,				//rmdir
NULL,				//fsctl

/* FILE Ops */
tk_wad4_fread,		//fread
tk_wad4_fwrite,		//fwrite
tk_wad4_fseek,		//fseek
tk_wad4_ftell,		//ftell
tk_wad4_fclose,		//fclose
tk_wad4_fgetc,		//fgetc
tk_wad4_fputc,		//fputc
tk_wad4_fioctl,		//ioctl

/* DIR ops */
tk_wad4_readdir,	//readdir
tk_wad4_closedir,	//closedir

/* Socket/Device Ops */
tk_wad4_fsend,		//fsend
tk_wad4_frecv		//frecv
};

int tk_wad4_init()
{
	tk_vf_register(&tk_vfile_wad4_vt);
	return(1);
}

#if 0
int tk_mount_wad4()
{
	TK_MOUNT *mnt;

	tk_puts("tk_mount_wad4\n");

	mnt=tk_alloc_mount();
	mnt->vt=&tk_vfile_wad4_vt;
//	mnt->udata0=img;
	
//	mnt->next=tk_vf_mount;
//	tk_vf_mount=mnt;
	tk_vf_addmount(mnt);
}
#endif

TK_MOUNT *tk_wad4_mount(char *devfn, char *mntfn,
	char *fsty, char *mode, char **opts)
{
	TK_MOUNT *mnt;
	TK_FILE *ifd;
	TK_WadImage *img;

	tk_puts("tk_mount_wad4\n");

	ifd=fopen(devfn);
	if(!ifd)return(NULL);
	
	img=TK_Wad4_OpenImage(ifd);
	if(!img)return(NULL);

	mnt=tk_alloc_mount();
	mnt->vt=&tk_vfile_wad4_vt;
	mnt->udata0=img;

	mnt->src=tk_strdup_in(mntfn);
	mnt->szSrc=strlen(mnt->src);

	mnt->tgt=tk_strdup_in(devfn);
	
	tk_vf_addmount(mnt);
	return(mnt);
}

TK_FILE *tk_wad4_fopen(TK_MOUNT *mnt, char *name, char *mode)
{
	TK_WadImage *img;
	TK_FILE *fd;
	void *p;
	int id, sz;

//	tk_printf("tk_wad4_fopen: %s\n", name);

	img=mnt->udata0;
	id=TK_Wad4_LookupLumpPath(img, name);
	if(id<0)
		return(NULL);

	sz=TK_Wad4_GetLumpSize(img, id);

	fd=tk_alloc_file();
	fd->vt=&tk_vfile_wad4_vt;
	fd->udata0=img;
	fd->ifd=id;
	fd->size=sz;
	
	return(fd);
}

TK_DIR *tk_wad4_opendir(TK_MOUNT *mnt, char *name)
{
	TK_WadImage *img;
	TK_DIR *fd;
	TK_DIRENT *tde;
	void *p;
	int id, ety, ipos;

//	tk_printf("tk_wad4_opendir: %s -> %s\n", name, tfname);

	img=mnt->udata0;
	id=TK_Wad4_LookupLumpPath(img, name);
	if(id<0)
		return(NULL);

	ety=TK_Wad4_GetLumpEntType(img, id);
	if(ety!=TK_W4ETY_DIR)
		return(NULL);
		
	ipos=TK_Wad4_GetLumpDirChild(img, id);

	tde=malloc(sizeof(TK_DIRENT));
	memset(tde, 0, sizeof(TK_DIRENT));

	fd=tk_alloc_dir();
	fd->vt=&tk_vfile_wad4_vt;
	fd->udata0=img;
	fd->ifd=id;
	fd->ipos=ipos;
	fd->udata2=tde;
	
	return(fd);
}

int tk_wad4_fclose(TK_FILE *fd)
{
	void *p;
	int i;
	
	tk_free_file(fd);
	return(i);
}

int tk_wad4_closedir(TK_DIR *fd)
{
	void *p;
	int i;
	
	free(fd->udata2);
	tk_free_dir(fd);
	return(i);
}

int tk_wad4_fread(void *buf, int sz1, int sz2, TK_FILE *fd)
{
	TK_WadImage *img;
	byte *cs, *ct, *cte;
	void *ptr;
	int sz, sz1, fofs, rofs, rsz, rsz1;
	int i;

	sz=sz1*sz2;
	ct=buf; cte=buf+sz;

	img=fd->udata0;
	fofs=fd->ofs;
	
	while(ct<cte)
	{
		ptr=TK_Wad4_GetCacheLumpNumOffs(img, fd->ifd, fofs, &rofs, &rsz);
		if(!ptr)
			break;
		cs=((byte *)ptr)+(fofs-rofs);
		sz1=cte-ct;
		rsz1=rsz-rofs;
		if(rsz1<sz1)
			sz1=rsz1;
		memcpy(ct, cs, sz1);
		ct+=sz1;
		fofs+=sz1;
	}

	fd->ofs=fofs;
	
	sz1=ct-((byte *)buf);
	
	return(sz1);
}

int tk_wad4_fwrite(void *buf, int sz1, int sz2, TK_FILE *fd)
{
	return(-1);
}

s64 tk_wad4_fseek(TK_FILE *fd, s64 ofs, int rel)
{
	if(rel==0)
	{
		fd->ofs=ofs;
		if(fd->ofs<0)
			fd->ofs=0;
		if(fd->ofs>fd->size)
			fd->ofs=fd->size;
		return(fd->ofs);
	}

	if(rel==1)
	{
		fd->ofs=fd->ofs+ofs;
		if(fd->ofs<0)
			fd->ofs=0;
		if(fd->ofs>fd->size)
			fd->ofs=fd->size;
		return(fd->ofs);
	}

	if(rel==2)
	{
		fd->ofs=fd->size+ofs;
		if(fd->ofs<0)
			fd->ofs=0;
		if(fd->ofs>fd->size)
			fd->ofs=fd->size;
		return(fd->ofs);
	}

	return(-1);
}

s64 tk_wad4_ftell(TK_FILE *fd)
{
	return(fd->ofs);
}

int tk_wad4_fgetc(TK_FILE *fd)
{
	byte tb[8];
	int i;

	i=tk_wad4_fread(tb, 1, 1, fd);
	if(i<=0)
		return(-1);
	return(tb[0]);
}

int tk_wad4_fputc(int ch, TK_FILE *fd)
{
	byte tb[8];
	int i;

	tb[0]=ch;
	i=tk_wad4_fwrite(tb, 1, 1, fd);
	if(i<=0)
		return(-1);
	return(ch);
}

int tk_wad4_fioctl(TK_FILE *fd, int cmd, void *ptr)
{
	return(-1);
}

int tk_wad4_fsend(TK_FILE *fd, int cmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr)
{
	return(-1);
}

int tk_wad4_frecv(TK_FILE *fd, int cmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr)
{
	return(-1);
}

TK_DIRENT *tk_wad4_readdir(TK_DIR *fd)
{
	TK_WadImage *img;
	TK_DIRENT *tde;
	void *p;
	int ifd, ipos, ip1, mode, ety, dty;

	img=fd->udata0;
	tde=fd->udata2;
	ifd=fd->ifd;
	ipos=fd->ipos;
	if(ipos<=0)
		return(NULL);

	tde->d_ino=ipos;
	tde->d_off=0;
	tde->d_type=0;
	tde->d_reclen=sizeof(TK_DIRENT);

	TK_Wad4_GetLumpDirName(img, ipos, tde->d_name);
	memset(tde->st_link, 0, 8);

	ety=TK_Wad4_GetLumpEntType(img, ipos);
	mode=TK_Wad4_GetLumpDirMode(img, ipos);

	dty=TKFAT_DT_UNKNOWN;
	if(ety==TK_W4ETY_DIR)
		{ mode|=TKFAT_EMODE_DIR; dty=TKFAT_DT_DIR; }
	else if(ety==TK_W4ETY_SYMLINK)
		{ mode|=TKFAT_EMODE_LINK; dty=TKFAT_DT_LNK; }
	else
		{ mode|=TKFAT_EMODE_REG; dty=TKFAT_DT_REG; }

	if(ety==TK_W4ETY_SYMLINK)
	{
		TK_Wad4_GetLumpDirLink(img, ipos, tde->st_link);
	}

	tde->d_type=dty;
	tde->st_mode=mode;

	tde->st_uid=TK_Wad4_GetLumpDirUid(img, ipos);
	tde->st_gid=TK_Wad4_GetLumpDirGid(img, ipos);
	tde->st_size=TK_Wad4_GetLumpSize(img, ipos);
	tde->st_ctime=TK_Wad4_GetLumpDirMtime(img, ipos);
	tde->st_mtime=TK_Wad4_GetLumpDirMtime(img, ipos);

	ip1=TK_Wad4_GetLumpDirNext(img, ipos);
	fd->ipos=ip1;
	
	return(tde);
}
