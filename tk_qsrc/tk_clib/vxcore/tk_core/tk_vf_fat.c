TK_MOUNT *tk_fat_mount(char *devfn, char *mntfn,
	char *fsty, char *mode, char **opts);
TK_FILE *tk_fat_fopen(TK_MOUNT *mnt, char *name, char *mode);
TK_DIR *tk_fat_opendir(TK_MOUNT *mnt, char *name);
int tk_fat_unlink(TK_MOUNT *mnt, char *name);
int tk_fat_rename(TK_MOUNT *mnt, char *oldfn, char *newfn);

int tk_fat_mkdir(TK_MOUNT *mnt, char *name, char *mode);
int tk_fat_rmdir(TK_MOUNT *mnt, char *name);

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
tk_fat_unlink,		//unlink
tk_fat_rename,		//rename
NULL,				//fstat

tk_fat_mkdir,		//mkdir
tk_fat_rmdir,		//rmdir
NULL,				//fsctl

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
tk_fat_closedir,	//closedir

/* Socket/Device Ops */
NULL,				//fsend
NULL				//frecv
};

int tk_fat_init()
{
	tk_vf_register(&tk_vfile_fat_vt);
	return(1);
}

int tk_mount_sdfat()
{
	TKFAT_FAT_DirEntExt tdee;
	TKFAT_FAT_DirEntExt *dee, *dee2;

	TKFAT_ImageInfo *img;
	TK_MOUNT *mnt;
	int		lba, clid, sz;
	int i, j, k;

	tk_puts("tk_mount_sdfat\n");

	img=TKFAT_CreateSdFatContext();

	mnt=tk_alloc_mount();
	mnt->vt=&tk_vfile_fat_vt;
	mnt->udata0=img;
	
//	mnt->next=tk_vf_mount;
//	tk_vf_mount=mnt;
	tk_vf_addmount(mnt);


	dee=&tdee;
	memset(dee, 0, sizeof(TKFAT_FAT_DirEntExt));

	i=TKFAT_LookupDirEntPath(img, dee, "swapfile.sys");
	if(i>=0)
	{
		tk_puts("tk_mount_sdfat: found swapfile\n");

		clid=TKFAT_GetDirEntCluster(dee);
		sz=TKFAT_GetDirEntSize(dee);
		lba=TKFAT_GetClusterLBA(img, clid);
		TK_VMem_AddSdSwap(lba, sz);
	}else
	{
		tk_puts("tk_mount_sdfat: no swapfile\n");
	}
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

//	tk_printf("tk_fat_fopen: %s\n", name);

	img=mnt->udata0;
	dee=&tdee;
	memset(dee, 0, sizeof(TKFAT_FAT_DirEntExt));

	if(mode[0]=='w')
	{
		i=TKFAT_CreateDirEntPath(img, dee, name);
		dee->is_write=1;
	}else
	{
		i=TKFAT_LookupDirEntPath(img, dee, name);
	}
	if(i<0)
	{
//		tk_printf("tk_fat_fopen: fail %s\n", name);
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

int tk_fat_unlink(TK_MOUNT *mnt, char *name)
{
	TKFAT_FAT_DirEntExt tdee;
	TKFAT_FAT_DirEntExt *dee, *dee2;
	TKFAT_ImageInfo *img;
	TK_FILE *fd;
	int i;

//	tk_printf("tk_fat_unlink: %s\n", name);

	img=mnt->udata0;
	dee=&tdee;
	memset(dee, 0, sizeof(TKFAT_FAT_DirEntExt));
	i=TKFAT_LookupDirEntPath(img, dee, name);
	if(i<0)
	{
//		tk_printf("tk_fat_unlink: fail %s\n", name);
		return(-1);
	}

	if(dee->deb.attrib&0x10)
	{
		return(-1);
//		return(NULL);
	}

	i=TKFAT_GetDirEntCluster(dee);
	TKFAT_UnlinkClusterChain(img, i);
	TKFAT_DeleteDirEnt(dee);
	return(1);
}

int tk_fat_rmdir(TK_MOUNT *mnt, char *name)
{
	TKFAT_FAT_DirEntExt tdee;
	TKFAT_FAT_DirEntExt *dee, *dee2;
	TKFAT_ImageInfo *img;
	TK_FILE *fd;
	int i;

//	tk_printf("tk_fat_unlink: %s\n", name);

	img=mnt->udata0;
	dee=&tdee;
	memset(dee, 0, sizeof(TKFAT_FAT_DirEntExt));
	i=TKFAT_LookupDirEntPath(img, dee, name);
	if(i<0)
	{
//		tk_printf("tk_fat_unlink: fail %s\n", name);
		return(-1);
	}

	if(!(dee->deb.attrib&0x10))
	{
		return(-1);
//		return(NULL);
	}

	i=TKFAT_GetDirEntCluster(dee);
	TKFAT_UnlinkClusterChain(img, i);
	TKFAT_DeleteDirEnt(dee);
	return(1);
}

int tk_fat_mkdir(TK_MOUNT *mnt, char *name, char *mode)
{
	char tb[260];
	TKFAT_FAT_DirEntExt tdee;
	TKFAT_FAT_DirEntExt *dee, *dee2;
	TKFAT_ImageInfo *img;
	TK_FILE *fd;
	int i;

//	tk_printf("tk_fat_fopen: %s\n", name);

	img=mnt->udata0;
	dee=&tdee;
	memset(dee, 0, sizeof(TKFAT_FAT_DirEntExt));

	strcpy(tb, name);
	strcat(tb, "/");

	i=TKFAT_CreateDirEntPath(img, dee, tb);
//	dee->is_write=1;
	return(1);
}


int tk_fat_rename(TK_MOUNT *mnt, char *oldfn, char *newfn)
{
	TKFAT_FAT_DirEntExt tdee1, tdee2;
	TKFAT_FAT_DirEntExt *dee1, *dee2;
	TKFAT_ImageInfo *img;
	int i;

	img=mnt->udata0;
	dee1=&tdee1;
	dee2=&tdee2;
	memset(dee1, 0, sizeof(TKFAT_FAT_DirEntExt));
	memset(dee2, 0, sizeof(TKFAT_FAT_DirEntExt));
	i=TKFAT_LookupDirEntPath(img, dee1, oldfn);
	if(i<0)
	{
//		tk_printf("tk_fat_rename: fail %s\n", oldfn);
		return(-1);
	}
	
	i=TKFAT_CreateDirEntPath(img, dee2, newfn);
	if(i<0)
	{
//		tk_printf("tk_fat_rename: fail %s\n", oldfn);
		return(-1);
	}
	
	memcpy(dee2->deb.ext+3, dee1->deb.ext+3, 21);
	
	TKFAT_DeleteDirEnt(dee1);
	TKFAT_UpdateDirEnt(dee2);
	return(1);
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
	TKFAT_FAT_DirEntExt *dee;

//	printf("tk_fat_fclose: %p\n", fd);

	dee=fd->udata1;
	if(dee->is_dirty)
	{
//		printf("tk_fat_sync: %p\n", fd);
		TKFAT_SyncDirEntFile(dee);
	}
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
	TKFAT_FAT_DirEntExt *dee;
	int sz;

	dee=fd->udata1;

	if(!(dee->is_write))
	{
		tk_printf("tk_fat_fwrite: write to file in read mode\n");
//		__debugbreak();
		return(-1);
	}

//	if((dee->is_write))
//	{
//		__debugbreak();
//		tk_printf("tk_fat_fwrite: write to file in read mode\n");
//		return(-1);
//	}

	sz=sz1*sz2;
	sz=TKFAT_ReadWriteDirEntFile(
		dee, fd->ofs, true, buf, sz);
	dee->is_dirty=1;
	if(sz>0)fd->ofs+=sz;
	if(fd->ofs>fd->size)
		fd->size=fd->ofs;
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
	TKFAT_FAT_DirEntExt tdee, tdee1;
	TKFAT_FAT_DirEntExt *dee, *dee1, *dee2;
	TKFAT_ImageInfo *img;
	TK_DIRENT *tde;
	TK_DIR *fd;
	int dcli, mcli;
	int i;

//	tk_printf("tk_fat_opendir: %s\n", name);

	img=mnt->udata0;
	dee=&tdee;
	dee1=&tdee1;
	memset(dee, 0, sizeof(TKFAT_FAT_DirEntExt));
	memset(dee1, 0, sizeof(TKFAT_FAT_DirEntExt));
	i=TKFAT_LookupDirEntPath(img, dee, name);
	if(i<0)
	{
//		tk_printf("tk_fat_opendir: fail %s\n", name);
		return(NULL);
	}

	if(!(dee->deb.attrib&0x10))
		return(NULL);

	dcli=TKFAT_GetDirEntCluster(dee);
	mcli=0;
	
	if(dcli>0)
	{
//		mcli=TKFAT_LookupDirEntNameQuick(img, dcli, dee1, "TKDIREXT.---");
		mcli=TKFAT_LookupDirEntNameQuick(img, dcli, dee1, "!!TKMETA.!!!");
	}

	dee2=malloc(sizeof(TKFAT_FAT_DirEntExt));
	memset(dee2, 0, sizeof(TKFAT_FAT_DirEntExt));
//	memcpy(dee2, dee, sizeof(TKFAT_FAT_DirEntExt));
	dee2->img=img;
//	dee2->clid=dee->clid;
	dee2->clid=dcli;
	dee2->mclid=mcli;
	dee2->idx=-1;
	dee2->midx=-1;

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
	memcpy(tde->st_link, dee->de_aname, 256);
	
	tde->st_mode=TKFAT_GetDirEntMode(dee);
	tde->st_uid=TKFAT_GetDirEntUid(dee);
	tde->st_gid=TKFAT_GetDirEntGid(dee);
	tde->st_size=TKFAT_GetDirEntSize(dee);
	tde->st_ctime=TKFAT_GetDirEntCTime(dee);
	tde->st_mtime=TKFAT_GetDirEntMTime(dee);
	
	return(tde);
}

int tk_fat_closedir(TK_DIR *fd)
{
	free(fd->udata1);
	free(fd->udata2);
	tk_free_dir(fd);
	return(0);
}
