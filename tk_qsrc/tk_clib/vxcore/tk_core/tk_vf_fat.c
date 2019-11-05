TK_MOUNT *tk_fat_mount(char *devfn, char *mntfn,
	char *fsty, char *mode, char **opts);
TK_FILE *tk_fat_fopen(TK_MOUNT *mnt, char *name, char *mode);
TK_DIR *tk_fat_opendir(TK_MOUNT *mnt, char *name);

int tk_fat_fread(void *buf, int sz1, int sz2, TK_FILE *fd);
int tk_fat_fwrite(void *buf, int sz1, int sz2, TK_FILE *fd);
s64 tk_fat_fseek(TK_FILE *fd, s64 ofs, int rel);
s64 tk_fat_ftell(TK_FILE *fd);
int tk_fat_fclose(TK_FILE *fd);
int tk_fat_fgetc(TK_FILE *fd);
int tk_fat_fputc(int ch, TK_FILE *fd);
int tk_fat_fioctl(TK_FILE *fd, int cmd, void *ptr);

TK_DIRENT *tk_fat_readdir(TK_DIR *fd);
int tk_fat_closedir(TK_DIR *fd);

TK_FILE_VT tk_vfile_fat_vt={
"vfat",				//fsname
NULL,				//next
tk_fat_mount,		//mount
tk_fat_fopen,		//fopen
tk_fat_opendir,		//fopendir
NULL,				//unlink
NULL,				//rename
NULL,				//fstat

/* FILE Ops */
tk_fat_fread,		//fread
tk_fat_fwrite,		//fwrite
tk_fat_fseek,		//fseek
tk_fat_ftell,		//ftell
tk_fat_fclose,		//fclose
tk_fat_fgetc,		//fgetc
tk_fat_fputc,		//fputc
tk_fat_fioctl,		//ioctl

/* DIR ops */
tk_fat_readdir,		//readdir
tk_fat_closedir		//closedir
};

int tk_fat_init()
{
	tk_vf_register(&tk_vfile_fat_vt);
	return(1);
}

int tk_mount_sdfat()
{
	TKFAT_ImageInfo *img;
	TK_MOUNT *mnt;

	img=TKFAT_CreateSdFatContext();

	mnt=tk_alloc_mount();
	mnt->vt=&tk_vfile_fat_vt;
	mnt->udata0=img;
	
//	mnt->next=tk_vf_mount;
//	tk_vf_mount=mnt;
	tk_vf_addmount(mnt);
}

TK_MOUNT *tk_fat_mount(char *devfn, char *mntfn,
	char *fsty, char *mode, char **opts)
{
}

TK_FILE *tk_fat_fopen(TK_MOUNT *mnt, char *name, char *mode)
{
	TKFAT_FAT_DirEntExt tdee;
	TKFAT_FAT_DirEntExt *dee, *dee2;
	TKFAT_ImageInfo *img;
	TK_FILE *fd;
	int i;

	tk_printf("tk_fat_fopen: %s\n", name);

	img=mnt->udata0;
	dee=&tdee;
	memset(dee, 0, sizeof(TKFAT_FAT_DirEntExt));
	i=TKFAT_LookupDirEntPath(img, dee, name);
	if(i<0)
	{
		tk_printf("tk_fat_fopen: fail %s\n", name);
		return(NULL);
	}

	if(dee->deb.attrib&0x10)
		return(NULL);

	dee2=malloc(sizeof(TKFAT_FAT_DirEntExt));
	memcpy(dee2, dee, sizeof(TKFAT_FAT_DirEntExt));
	dee2->img=img;

	fd=tk_alloc_file();
	fd->vt=&tk_vfile_fat_vt;
	fd->udata0=img;
	fd->udata1=dee2;
	fd->ofs=0;
	fd->size=TKFAT_GetDirEntSize(dee2);

	return(fd);
}

s64 tk_fat_fseek(TK_FILE *fd, s64 ofs, int rel)
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

s64 tk_fat_ftell(TK_FILE *fd)
{
	return(fd->ofs);
}

int tk_fat_fclose(TK_FILE *fd)
{
	free(fd->udata1);
	tk_free_file(fd);
	return(0);
}

int tk_fat_fread(void *buf, int sz1, int sz2, TK_FILE *fd)
{
	int sz;

	sz=sz1*sz2;
	sz=TKFAT_ReadWriteDirEntFile(
		fd->udata1, fd->ofs, false, buf, sz);
	if(sz>0)fd->ofs+=sz;
	return(sz);
}

int tk_fat_fwrite(void *buf, int sz1, int sz2, TK_FILE *fd)
{
	int sz;

	sz=sz1*sz2;
	sz=TKFAT_ReadWriteDirEntFile(
		fd->udata1, fd->ofs, true, buf, sz);
	if(sz>0)fd->ofs+=sz;
	return(sz);
}

int tk_fat_fgetc(TK_FILE *fd)
{
	byte buf[4];
	tk_fat_fread(buf, 1, 1, fd);
	return(buf[0]);
}

int tk_fat_fputc(int ch, TK_FILE *fd)
{
	byte buf[4];
	ch=(byte)ch;
	buf[0]=ch;
	tk_fat_fwrite(buf, 1, 1, fd);
	return(ch);
}

int tk_fat_fioctl(TK_FILE *fd, int cmd, void *ptr)
{
	return(-1);
}


TK_DIR *tk_fat_opendir(TK_MOUNT *mnt, char *name)
{
	TKFAT_FAT_DirEntExt tdee;
	TKFAT_FAT_DirEntExt *dee, *dee2;
	TKFAT_ImageInfo *img;
	TK_DIRENT *tde;
	TK_DIR *fd;
	int dcli;
	int i;

//	tk_printf("tk_fat_opendir: %s\n", name);

	img=mnt->udata0;
	dee=&tdee;
	memset(dee, 0, sizeof(TKFAT_FAT_DirEntExt));
	i=TKFAT_LookupDirEntPath(img, dee, name);
	if(i<0)
	{
		tk_printf("tk_fat_opendir: fail %s\n", name);
		return(NULL);
	}

	if(!(dee->deb.attrib&0x10))
		return(NULL);

	dcli=TKFAT_GetDirEntCluster(dee);

	dee2=malloc(sizeof(TKFAT_FAT_DirEntExt));
	memset(dee2, 0, sizeof(TKFAT_FAT_DirEntExt));
//	memcpy(dee2, dee, sizeof(TKFAT_FAT_DirEntExt));
	dee2->img=img;
//	dee2->clid=dee->clid;
	dee2->clid=dcli;
	dee2->idx=-1;

	tde=malloc(sizeof(TK_DIRENT));
	memset(tde, 0, sizeof(TK_DIRENT));

	fd=tk_alloc_dir();
	fd->vt=&tk_vfile_fat_vt;
	fd->udata0=img;
	fd->udata1=dee2;
	fd->udata2=tde;
	fd->ofs=0;
	fd->size=TKFAT_GetDirEntSize(dee2);

	return(fd);
}

TK_DIRENT *tk_fat_readdir(TK_DIR *fd)
{
	TKFAT_FAT_DirEntExt *dee;
	TKFAT_ImageInfo *img;
	TK_DIRENT *tde;
	int i;

	img=fd->udata0;
	dee=fd->udata1;
	tde=fd->udata2;

	i=TKFAT_WalkDirEntNext(img, dee);
	if(i<0)
		return(NULL);
	memcpy(tde->d_name, dee->de_name, 256);
	return(tde);
}

int tk_fat_closedir(TK_DIR *fd)
{
	free(fd->udata1);
	free(fd->udata2);
	tk_free_dir(fd);
	return(0);
}
