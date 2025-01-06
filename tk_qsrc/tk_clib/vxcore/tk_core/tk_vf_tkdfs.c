TK_MOUNT *tk_dfs_mount(char *devfn, char *mntfn,
	char *fsty, char *mode, char **opts);
TK_FILE *tk_dfs_fopen(TK_MOUNT *mnt, TK_USERINFO *usri, char *name, char *mode);
TK_DIR *tk_dfs_opendir(TK_MOUNT *mnt, TK_USERINFO *usri, char *name);
int tk_dfs_unlink(TK_MOUNT *mnt, TK_USERINFO *usri, char *name);
int tk_dfs_rename(TK_MOUNT *mnt, TK_USERINFO *usri, char *oldfn, char *newfn, char *mode);

int tk_dfs_fsctl(TK_MOUNT *mnt, TK_USERINFO *usri,
	char *name, int cmd, void *ptr);

int tk_dfs_mkdir(TK_MOUNT *mnt, TK_USERINFO *usri, char *name, char *mode);
int tk_dfs_rmdir(TK_MOUNT *mnt, TK_USERINFO *usri, char *name);

int tk_dfs_fread(void *buf, int sz1, int sz2, TK_FILE *fd);
int tk_dfs_fwrite(void *buf, int sz1, int sz2, TK_FILE *fd);
s64 tk_dfs_fseek(TK_FILE *fd, s64 ofs, int rel);
s64 tk_dfs_ftell(TK_FILE *fd);
int tk_dfs_fclose(TK_FILE *fd);
int tk_dfs_fgetc(TK_FILE *fd);
int tk_dfs_fputc(int ch, TK_FILE *fd);
int tk_dfs_fioctl(TK_FILE *fd, int cmd, void *ptr);

TK_DIRENT *tk_dfs_readdir(TK_DIR *fd);
int tk_dfs_closedir(TK_DIR *fd);

TK_FILE_VT tk_vfile_dfs_vt={
"dfs2",				//fsname
NULL,				//next
tk_dfs_mount,		//mount
tk_dfs_fopen,		//fopen
tk_dfs_opendir,		//fopendir
tk_dfs_unlink,		//unlink
tk_dfs_rename,		//rename
NULL,				//fstat

tk_dfs_mkdir,		//mkdir
tk_dfs_rmdir,		//rmdir
tk_dfs_fsctl,		//fsctl

/* FILE Ops */
tk_dfs_fread,		//fread
tk_dfs_fwrite,		//fwrite
tk_dfs_fseek,		//fseek
tk_dfs_ftell,		//ftell
tk_dfs_fclose,		//fclose
tk_dfs_fgetc,		//fgetc
tk_dfs_fputc,		//fputc
tk_dfs_fioctl,		//ioctl

/* DIR ops */
tk_dfs_readdir,		//readdir
tk_dfs_closedir,	//closedir

/* Socket/Device Ops */
NULL,				//fsend
NULL				//frecv
};

int tk_dfs_init()
{
	tk_vf_register(&tk_vfile_dfs_vt);
	return(1);
}

#if 1
int tk_mount_sddfs(char *path)
{
	TKDFS_DirentInfo tdee;
	TKDFS_DirentInfo *dee, *dee2;

	TKDFS_ImageContext *img;
	TK_MOUNT *mnt;
	int		lba, clid, sz;
	int i, j, k;

	if(tk_bdspi_dfs_lba<=0)
		return(0);

	tk_dbg_printf("tk_mount_sddfs\n");

//	img=TKFAT_CreateSdFatContext();
	img=TKDFS_TryOpenImage(tkspi_id_bdev, tk_bdspi_dfs_lba);
	if(!img)
	{
		img=TKDFS_InitializeNewImage(
			tkspi_id_bdev,
			tk_bdspi_dfs_lba,
			tk_bdspi_dfs_lbn);
	}

	if(!path)
	{
		dee=&tdee;
		memset(dee, 0, sizeof(TKDFS_DirentInfo));

		TKDFS_ImageCreateInodePath(img, dee, "boot/");
		TKDFS_ImageCreateInodePath(img, dee, "dev/");
		TKDFS_ImageCreateInodePath(img, dee, "usr/");
		TKDFS_ImageCreateInodePath(img, dee, "mnt/");
		TKDFS_ImageCreateInodePath(img, dee, "tmp/");
		TKDFS_ImageCreateInodePath(img, dee, "tmp/shm/");
	}

	tk_dbg_printf("tk_mount_sddfs: B\n");

	mnt=tk_alloc_mount();
	mnt->vt=&tk_vfile_dfs_vt;
	mnt->udata0=img;
	
	if(path)
	{
		mnt->src=tk_strdup_in(path);
		mnt->szSrc=strlen(mnt->src);
	}
	
	tk_vf_addmount(mnt);

//	if(mnt->vt->fopen!=tk_dfs_fopen)
//		{ __debugbreak(); }

//	dee=&tdee;
//	memset(dee, 0, sizeof(TKDFS_DirentInfo));

	tk_dbg_printf("tk_mount_sddfs: C\n");
}
#endif

TK_MOUNT *tk_dfs_mount(char *devfn, char *mntfn,
	char *fsty, char *mode, char **opts)
{
	TKDFS_DirentInfo tdee;
	TKDFS_DirentInfo *dee, *dee2;

	TKDFS_ImageContext *img;
	TK_MOUNT *mnt;
	s64		devid;
	int		lba, clid, sz;
	int i, j, k;

	tk_dbg_printf("tk_dfs_mount\n");

//	devid=-1;
//	tk_fsctl(devfn, TK_IOC_VFS_OPENBLKDEV, &devid);
	devid=TKBDEV_OpenPathAsDeviceId(devfn);

	if(devid<=0)
		return(0);

	img=TKDFS_TryOpenImage(devid, 0);
	if(!img)
	{
		return(0);
	}

	if(!mntfn)
	{
		dee=&tdee;
		memset(dee, 0, sizeof(TKDFS_DirentInfo));

		TKDFS_ImageCreateInodePath(img, dee, "boot/");
		TKDFS_ImageCreateInodePath(img, dee, "dev/");
		TKDFS_ImageCreateInodePath(img, dee, "usr/");
		TKDFS_ImageCreateInodePath(img, dee, "mnt/");
		TKDFS_ImageCreateInodePath(img, dee, "tmp/");
		TKDFS_ImageCreateInodePath(img, dee, "tmp/shm/");
	}

	tk_dbg_printf("tk_dfs_mount: B\n");

	mnt=tk_alloc_mount();
	mnt->vt=&tk_vfile_dfs_vt;
	mnt->udata0=img;
	
	if(mntfn)
	{
		mnt->src=tk_strdup_in(mntfn);
		mnt->szSrc=strlen(mnt->src);
	}
	
	if(devfn)
	{
		mnt->tgt=tk_strdup_in(devfn);
	}

	tk_vf_addmount(mnt);

	tk_dbg_printf("tk_dfs_mount: C\n");
}

int tk_dfs_mkfs(char *devfn, char **opts)
{
	TKDFS_DirentInfo tdee;
	TKDFS_DirentInfo *dee, *dee2;

	TKDFS_ImageContext *img;
	TK_MOUNT *mnt;
	s64		devid;
	int		lba, clid, sz;
	int i, j, k;

	tk_dbg_printf("tk_dfs_mount\n");

	devid=TKBDEV_OpenPathAsDeviceId(devfn);

	if(devid<=0)
		return(0);

	sz=TKBDEV_GetSectorCount(devid);
	img=TKDFS_InitializeNewImage(devid, 0, sz);
	TKBDEV_ReleaseDevice(devid);
	return(1);
}

int tk_dfs_fsctl(TK_MOUNT *mnt, TK_USERINFO *usri,
	char *name, int cmd, void *ptr)
{
	TKDFS_ImageContext *img;
	int devid;

	img=mnt->udata0;

	if(cmd==TK_IOC_VFS_OPENBLKDEV)
	{
//		devid=TKFAT_GetLvmDeviceId(img, name);
		*(s64 *)ptr=devid;
		return(1);
	}

	if(cmd==TK_IOC_VFS_MKFS)
	{
		return(tk_dfs_mkfs(name, (char **)ptr));
	}

	return(-1);
}

TK_FILE *tk_dfs_fopen(TK_MOUNT *mnt, TK_USERINFO *usri, char *name, char *mode)
{
	char tbuf[512], st_link[384];
	TKDFS_DirentInfo tdee;
	TKDFS_DirentInfo *dee, *dee2;
	TKDFS_ImageContext *img;
	byte *clbuf;
	char *name1, *s1;
	TK_FILE *fd;
	int clid, lba, imode;
	int i, sz;

//	tk_dbg_printf("tk_dfs_fopen: %s\n", name);

	img=mnt->udata0;
	dee=&tdee;
	memset(dee, 0, sizeof(TKDFS_DirentInfo));

	if(mode[0]=='w')
	{
		i=TKDFS_ImageCreateInodePath(img, dee, name);
		dee->is_write=1;
	}else
	{
		i=TKDFS_ImageLookupInodePath(img, dee, name);
		if(mode[0]=='a')
			dee->is_write=1;
	}
	if(i<=0)
	{
//		tk_dbg_printf("tk_dfs_fopen: fail %s\n", name);
		return(NULL);
	}
	
	if(i==2)
	{
		strncpy(st_link, dee->de_name+1, 256);
		if(dee->de_name[0]==':')
		{
			if(mnt->src && mnt->src[0])
				{ sprintf(st_link, "/%s/%s", mnt->src, dee->de_name+1); }
			else
				{ sprintf(st_link, "/%s", dee->de_name+1); }
		}
		return(tk_fopen2(usri, st_link, mode));
	}

	imode=TKDFS_GetDirEntMode(dee);

	if((imode&0xF000)==0xA000)
	{
		sz=TKDFS_ReadWriteDirEntFile(
			dee, 0, 0, tbuf, 512);

		strncpy(st_link, tbuf+9, 256);
		if(tbuf[8]==':')
		{
			if(mnt->src && mnt->src[0])
				{ sprintf(st_link, "/%s/%s", mnt->src, tbuf+9); }
			else
				{ sprintf(st_link, "/%s", tbuf+9); }
		}
		
		return(tk_fopen2(usri, st_link, mode));
	}

	if(imode&0x4000)
		return(NULL);

	dee2=malloc(sizeof(TKDFS_DirentInfo));
//	memcpy(dee2, dee, sizeof(TKDFS_DirentInfo));
	TKDFS_CopyDirentInfo(img, dee2, dee);
	dee2->img=img;

	fd=tk_alloc_file();
	fd->vt=&tk_vfile_dfs_vt;
	fd->udata0=img;
	fd->udata1=dee2;
	fd->ofs=0;
	fd->size=TKDFS_GetDirEntSize(dee2);

	if(mode[0]=='a')
	{
		fd->ofs=fd->size;
	}

	return(fd);
}

int tk_dfs_unlink(TK_MOUNT *mnt, TK_USERINFO *usri, char *name)
{
	TKDFS_DirentInfo tdee;
	TKDFS_DirentInfo *dee, *dee2;
	TKDFS_ImageContext *img;
	TK_FILE *fd;
	int i;

//	tk_dbg_printf("tk_dfs_unlink: %s\n", name);

	img=mnt->udata0;
	dee=&tdee;
	memset(dee, 0, sizeof(TKDFS_DirentInfo));
	i=TKDFS_ImageLookupInodePath(img, dee, name);
	if(i<0)
	{
//		tk_dbg_printf("tk_dfs_unlink: fail %s\n", name);
		return(-1);
	}

//	if(dee->deb.attrib&0x10)
	if(TKDFS_GetDirEntMode(dee)&0x4000)
	{
		return(-1);
//		return(NULL);
	}

	i=TKDFS_GetDirEntInode(dee);
	TKDFS_DeleteInode(img, i);
	TKDFS_DeleteDirEnt(dee);
	return(1);
}

int tk_dfs_rmdir(TK_MOUNT *mnt, TK_USERINFO *usri, char *name)
{
	TKDFS_DirentInfo tdee;
	TKDFS_DirentInfo *dee, *dee2;
	TKDFS_ImageContext *img;
	TK_FILE *fd;
	int i;

//	tk_dbg_printf("tk_dfs_unlink: %s\n", name);

	img=mnt->udata0;
	dee=&tdee;
	memset(dee, 0, sizeof(TKDFS_DirentInfo));
	i=TKDFS_ImageLookupInodePath(img, dee, name);
	if(i<0)
	{
//		tk_dbg_printf("tk_dfs_unlink: fail %s\n", name);
		return(-1);
	}

//	if(!(dee->deb.attrib&0x10))
	if(!(TKDFS_GetDirEntMode(dee)&0x4000))
	{
		return(-1);
//		return(NULL);
	}

	i=TKDFS_GetDirEntInode(dee);
	TKDFS_DeleteInode(img, i);
	TKDFS_DeleteDirEnt(dee);
	return(1);
}

int tk_dfs_mkdir(TK_MOUNT *mnt, TK_USERINFO *usri, char *name, char *mode)
{
	char tb[260];
	TKDFS_DirentInfo tdee;
	TKDFS_DirentInfo *dee, *dee2;
	TKDFS_ImageContext *img;
	TK_FILE *fd;
	int i;

//	tk_dbg_printf("tk_dfs_fopen: %s\n", name);

	img=mnt->udata0;
	dee=&tdee;
	memset(dee, 0, sizeof(TKDFS_DirentInfo));

	strcpy(tb, name);
	strcat(tb, "/");

	i=TKDFS_ImageCreateInodePath(img, dee, tb);
//	dee->is_write=1;
	return(1);
}


int tk_dfs_rename(TK_MOUNT *mnt, TK_USERINFO *usri,
	char *oldfn, char *newfn, char *mode)
{
	char tbuf[512];

	TKDFS_DirentInfo tdee1, tdee2;
	TKDFS_DirentInfo *dee1, *dee2;
	TKDFS_ImageContext *img;
	int i, sz;

	img=mnt->udata0;
	dee1=&tdee1;
	dee2=&tdee2;
	memset(dee1, 0, sizeof(TKDFS_DirentInfo));
	memset(dee2, 0, sizeof(TKDFS_DirentInfo));

	if(mode && *mode && (*mode!='r'))
	{
		if((*mode=='l') || (*mode=='s') || (*mode=='S'))
		{
			sz=8+strlen(oldfn)+2;
			((u64 *)tbuf)[0]=0xBEAF5A52618F4181ULL;
			tbuf[8]=' ';
			strcpy(tbuf+9, oldfn);
			
			if((*mode=='l') || (*mode=='L'))
				{ tbuf[8]=':'; }

//			tk_dbg_printf("tk_dfs_rename: ln %s %s\n", oldfn, newfn);

			i=TKDFS_ImageCreateInodePath(img, dee2, newfn);
			if(i<0)
				return(-1);

//			tk_dbg_printf("tk_dfs_rename: A1\n");

			TKDFS_ReadWriteDirEntFile(
				dee2, 0, 9, tbuf, sz);
			TKDFS_SetDirEntMode(dee2, 0xA000);
			return(1);
		}
	
		return(-1);
	}

	i=TKDFS_ImageLookupInodePath(img, dee1, oldfn);
	if(i<0)
	{
//		tk_dbg_printf("tk_dfs_rename: fail %s\n", oldfn);
		return(-1);
	}
	
	i=TKDFS_ImageCreateInodePath(img, dee2, newfn);
	if(i<0)
	{
//		tk_dbg_printf("tk_dfs_rename: fail %s\n", oldfn);
		return(-1);
	}
	
	i=TKDFS_GetDirEntInode(dee1);
	TKDFS_SetDirEntInode(dee2, i);
	
	TKDFS_DeleteDirEnt(dee1);
	TKDFS_UpdateDirEnt(dee2);
	return(1);
}


s64 tk_dfs_fseek(TK_FILE *fd, s64 ofs, int rel)
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

s64 tk_dfs_ftell(TK_FILE *fd)
{
	return(fd->ofs);
}

int tk_dfs_fclose(TK_FILE *fd)
{
	TKDFS_DirentInfo *dee;

//	tk_dbg_printf("tk_dfs_fclose: %p\n", fd);

	dee=fd->udata1;
	if(dee->is_dirty)
	{
		tk_dbg_printf("tk_dfs_fclose: update and sync %p\n", fd);
		TKDFS_UpdateDirEnt(dee);
		TKDFS_SyncDirEntFile(dee);
	}
	free(fd->udata1);
	tk_free_file(fd);
	return(0);
}

int tk_dfs_fread(void *buf, int sz1, int sz2, TK_FILE *fd)
{
	int sz;

	sz=sz1*sz2;
	sz=TKDFS_ReadWriteDirEntFile(
		fd->udata1, fd->ofs, false, buf, sz);
	if(sz>0)fd->ofs+=sz;
	return(sz);
}

int tk_dfs_fwrite(void *buf, int sz1, int sz2, TK_FILE *fd)
{
	TKDFS_DirentInfo *dee;
	int sz, szb;

	dee=fd->udata1;

	if(!(dee->is_write))
	{
		tk_dbg_printf("tk_dfs_fwrite: write to file in read mode\n");
//		__debugbreak();
		return(-1);
	}

	sz=sz1*sz2;
	szb=TKDFS_ReadWriteDirEntFile(
		dee, fd->ofs, 1, buf, sz);
//	dee->is_dirty=1;
	
	if(szb!=sz)
	{
		tk_dbg_printf("tk_dfs_fwrite: write size %d -> %d bytes\n", sz, sz1);
	}
	
	if(sz>0)fd->ofs+=sz;
	if(fd->ofs>fd->size)
	{
		fd->size=fd->ofs;
//		dee->is_dirty=1;
//		TKDFS_SetDirEntSize(dee, fd->size);
		TKDFS_UpdateDirEnt(dee);
//		tk_dbg_printf("tk_dfs_fwrite: expand file to %d bytes\n", fd->size);
	}
	return(sz);
}

int tk_dfs_fgetc(TK_FILE *fd)
{
	byte buf[4];
	tk_dfs_fread(buf, 1, 1, fd);
	return(buf[0]);
}

int tk_dfs_fputc(int ch, TK_FILE *fd)
{
	byte buf[4];
	ch=(byte)ch;
	buf[0]=ch;
	tk_dfs_fwrite(buf, 1, 1, fd);
	return(ch);
}

int tk_dfs_fioctl(TK_FILE *fd, int cmd, void *ptr)
{
	return(-1);
}


TK_DIR *tk_dfs_opendir(TK_MOUNT *mnt, TK_USERINFO *usri, char *name)
{
	TKDFS_DirentInfo tdee, tdee1;
	char tbuf[512], st_link[384];
	TKDFS_DirentInfo *dee, *dee1, *dee2;
	TKDFS_ImageContext *img;
	TK_DIRENT *tde;
	TK_DIR *fd;
	int dcli, mcli, mode;
	int i, sz;

//	tk_dbg_printf("tk_dfs_opendir: %s\n", name);

	img=mnt->udata0;
	dee=&tdee;
	dee1=&tdee1;
	memset(dee, 0, sizeof(TKDFS_DirentInfo));
	memset(dee1, 0, sizeof(TKDFS_DirentInfo));
	i=TKDFS_ImageLookupInodePath(img, dee, name);
	
	if((i<=0) && (!name || !(*name)))
	{
		dee->de_ino=1;
		i=1;
	}
	
	if(i<=0)
	{
		tk_dbg_printf("tk_dfs_opendir: fail lookup %s\n", name);
		return(NULL);
	}

	if(i==2)
	{
		strncpy(st_link, dee->de_name+1, 256);
		if(dee->de_name[0]==':')
		{
			if(mnt->src && mnt->src[0])
				{ sprintf(st_link, "/%s/%s", mnt->src, dee->de_name+1); }
			else
				{ sprintf(st_link, "/%s", dee->de_name+1); }
		}
		return(tk_opendir2(usri, st_link));
	}

	mode=TKDFS_GetDirEntMode(dee);

	if((mode&0xF000)==0xA000)
	{
		sz=TKDFS_ReadWriteDirEntFile(
			dee, 0, 0, tbuf, 512);

		strncpy(st_link, tbuf+9, 256);
		if(tbuf[8]==':')
		{
			if(mnt->src && mnt->src[0])
				{ sprintf(st_link, "/%s/%s", mnt->src, tbuf+9); }
			else
				{ sprintf(st_link, "/%s", tbuf+9); }
		}
		
		return(tk_opendir2(usri, st_link));
	}

	if(!(mode&0x4000))
	{
		tk_dbg_printf("tk_dfs_opendir: not dir %s ino=%d mode=%04X\n",
			name, dee->de_ino, mode);
		return(NULL);
	}

	dee2=tk_malloc(sizeof(TKDFS_DirentInfo));
	memset(dee2, 0, sizeof(TKDFS_DirentInfo));
//	memcpy(dee2, dee, sizeof(TKDFS_DirentInfo));
	dee2->img=img;

	dee2->di_ino=dee->de_ino;
	dee2->di_idx=-1;

	tde=tk_malloc(sizeof(TK_DIRENT));
	memset(tde, 0, sizeof(TK_DIRENT));

	fd=tk_alloc_dir();
	fd->vt=&tk_vfile_dfs_vt;
	fd->udata0=img;
	fd->udata1=dee2;
	fd->udata2=tde;
	fd->udata3=mnt;
	fd->ofs=0;
	fd->size=TKDFS_GetDirEntSize(dee2);

	return(fd);
}

TK_DIRENT *tk_dfs_readdir(TK_DIR *fd)
{
	char tbuf[512];
	TKDFS_DirentInfo *dee;
	TKDFS_ImageContext *img;
	TK_MOUNT *mnt;
	TK_DIRENT *tde;
	byte *clbuf;
	char *name1;
	int clid, lba;
	int i, sz;

	img=fd->udata0;
	dee=fd->udata1;
	tde=fd->udata2;
	mnt=fd->udata3;

	i=TKDFS_WalkDirEntNext(img, dee);
	if(i<=0)
		return(NULL);

	/* skip over deleted but not removed dirents... */
	while((i>0) && !dee->de_ino)
		i=TKDFS_WalkDirEntNext(img, dee);
	if(i<=0)
		return(NULL);

	memcpy(tde->d_name, dee->de_name, 256);
//	memcpy(tde->st_link, dee->de_aname, 256);
	
	tde->st_mode=TKDFS_GetDirEntMode(dee);
	tde->st_uid=TKDFS_GetDirEntUid(dee);
	tde->st_gid=TKDFS_GetDirEntGid(dee);
	tde->st_size=TKDFS_GetDirEntSize(dee);
	tde->st_ctime=TKDFS_GetDirEntCTime(dee);
	tde->st_mtime=TKDFS_GetDirEntMTime(dee);

	if((tde->st_mode&0xF000)==0xA000)
	{
		sz=TKDFS_ReadWriteDirEntFile(
			dee, 0, 0, tbuf, 512);
		tde->st_mode|=TKFAT_EMODE_LINK;

		strncpy(tde->st_link, tbuf+9, 256);
		if(tbuf[8]==':')
		{
			if(mnt->src && mnt->src[0])
				{ sprintf(tde->st_link, "/%s/%s", mnt->src, tbuf+9); }
			else
				{ sprintf(tde->st_link, "/%s", tbuf+9); }
		}
	}

	return(tde);
}

int tk_dfs_closedir(TK_DIR *fd)
{
	tk_free(fd->udata1);
	tk_free(fd->udata2);
	tk_free_dir(fd);
	return(0);
}
