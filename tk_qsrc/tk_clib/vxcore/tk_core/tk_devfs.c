/*
Device Filesystem

Will generally be mounted at "/dev" or similar.
 */

TK_DEVFSDEV *tk_devfs_dev;

TK_DEVFSDEV *TK_DevFS_AllocDevice(void)
{
	TK_DEVFSDEV *tmp;
	
	tmp=tk_malloc(sizeof(TK_DEVFSDEV));
	memset(tmp, 0, sizeof(TK_DEVFSDEV));
	return(tmp);
}

int TK_DevFS_AddDevice(TK_DEVFSDEV *dev)
{
	printf("TK_DevFS_AddDevice: Add %s\n", dev->src);
	dev->szSrc=strlen(dev->src);
	dev->next=tk_devfs_dev;
	tk_devfs_dev=dev;
	return(0);
}


TK_MOUNT *tk_devfs_mount(char *devfn, char *mntfn,
	char *fsty, char *mode, char **opts);
TK_FILE *tk_devfs_fopen(TK_MOUNT *mnt, TK_USERINFO *usri, char *name, char *mode);
int tk_devfs_unlink(TK_MOUNT *mnt, char *name);
int tk_devfs_rename(TK_MOUNT *mnt, char *oldfn, char *newfn);

int tk_devfs_mkdir(TK_MOUNT *mnt, char *name, char *mode);
int tk_devfs_rmdir(TK_MOUNT *mnt, char *name);

int tk_devfs_fread(void *buf, int sz1, int sz2, TK_FILE *fd);
int tk_devfs_fwrite(void *buf, int sz1, int sz2, TK_FILE *fd);
s64 tk_devfs_fseek(TK_FILE *fd, s64 ofs, int rel);
s64 tk_devfs_ftell(TK_FILE *fd);
int tk_devfs_fclose(TK_FILE *fd);
int tk_devfs_fgetc(TK_FILE *fd);
int tk_devfs_fputc(int ch, TK_FILE *fd);
int tk_devfs_fioctl(TK_FILE *fd, int cmd, void *ptr);

TK_DIR *tk_devfs_opendir(TK_MOUNT *mnt, char *name);
TK_DIRENT *tk_devfs_readdir(TK_DIR *fd);
int tk_devfs_closedir(TK_DIR *fd);

TK_FILE_VT tk_vfile_devfs_vt={
"devfs",			//fsname
NULL,				//next
tk_devfs_mount,		//mount
tk_devfs_fopen,		//fopen
tk_devfs_opendir,	//fopendir
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
NULL,				//ioctl

/* DIR ops */
tk_devfs_readdir,	//readdir
tk_devfs_closedir	//closedir
};

int tk_devfs_init()
{
	tk_vf_register(&tk_vfile_devfs_vt);
	return(1);
}

int tk_mount_devfs()
{
//	TKFAT_ImageInfo *img;
	TK_MOUNT *mnt;

	tk_puts("tk_mount_devfs\n");

//	img=TKFAT_CreateSdFatContext();

	mnt=tk_alloc_mount();
	mnt->vt=&tk_vfile_devfs_vt;
//	mnt->udata0=img;
	
	mnt->src="dev";
	mnt->szSrc=strlen(mnt->src);
	
//	mnt->next=tk_vf_mount;
//	tk_vf_mount=mnt;
	tk_vf_addmount(mnt);
}

TK_MOUNT *tk_devfs_mount(char *devfn, char *mntfn,
	char *fsty, char *mode, char **opts)
{
}

TK_FILE *tk_devfs_fopen(TK_MOUNT *mnt, TK_USERINFO *usri, char *name, char *mode)
{
	TK_DEVFSDEV *cur;
	char *s1;
	
	while(*name=='/')
		name++;

	cur=tk_devfs_dev;
	while(cur)
	{
		if(strncmp(mnt->src, name, mnt->szSrc))
		{
			cur=cur->next;
			continue;
		}

		s1=name;
		s1+=mnt->szSrc;
		if(*s1 && (*s1!='?'))
		{
			mnt=mnt->next;
			continue;
		}

		if(cur->vt && cur->vt->fopen)
			return(cur->vt->fopen(cur, usri, name, mode));

		cur=cur->next;
	}
	return(NULL);
}

TK_DIR *tk_devfs_opendir(TK_MOUNT *mnt, char *name)
{
//	TKFAT_FAT_DirEntExt tdee, tdee1;
//	TKFAT_FAT_DirEntExt *dee, *dee1, *dee2;
//	TKFAT_ImageInfo *img;
	TK_DIRENT *tde;
	TK_DIR *fd;
	int dcli, mcli;
	int i;

//	tk_printf("tk_fat_opendir: %s\n", name);

	tde=malloc(sizeof(TK_DIRENT));
	memset(tde, 0, sizeof(TK_DIRENT));

	fd=tk_alloc_dir();
	fd->vt=&tk_vfile_devfs_vt;
	fd->udata1=tk_devfs_dev;
	fd->udata2=tde;
	fd->ofs=0;

	return(fd);
}

TK_DIRENT *tk_devfs_readdir(TK_DIR *fd)
{
	TK_DEVFSDEV *cur;
	TK_DIRENT *tde;

	cur=fd->udata1;
	if(!cur)
		return(NULL);
	fd->udata1=cur->next;

	tde=fd->udata2;
	strcpy(tde->d_name, cur->src);

	tde->st_mode=0;
	tde->st_uid=0;
	tde->st_gid=0;
	tde->st_size=0;
	tde->st_ctime=0;
	tde->st_mtime=0;

	return(tde);
}

int tk_devfs_closedir(TK_DIR *fd)
{
//	free(fd->udata1);
	free(fd->udata2);
	tk_free_dir(fd);
	return(0);
}
