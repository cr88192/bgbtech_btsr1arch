TK_MOUNT *tk_fat_mount(char *devfn, char *mntfn,
	char *fsty, char *mode, char **opts);
TK_FILE *tk_fat_fopen(TK_MOUNT *mnt, TK_USERINFO *usri, char *name, char *mode);
TK_DIR *tk_fat_opendir(TK_MOUNT *mnt, TK_USERINFO *usri, char *name);
int tk_fat_unlink(TK_MOUNT *mnt, TK_USERINFO *usri, char *name);
int tk_fat_rename(TK_MOUNT *mnt, TK_USERINFO *usri, char *oldfn, char *newfn, char *mode);

int tk_fat_fsctl(TK_MOUNT *mnt, TK_USERINFO *usri,
	char *name, int cmd, void *ptr);

int tk_fat_mkdir(TK_MOUNT *mnt, TK_USERINFO *usri, char *name, char *mode);
int tk_fat_rmdir(TK_MOUNT *mnt, TK_USERINFO *usri, char *name);

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
tk_fat_fsctl,		//fsctl

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

int tk_mount_sdfat(char *path)
{
	TKFAT_FAT_DirEntExt tdee;
	TKFAT_FAT_DirEntExt *dee, *dee2;

	TKFAT_ImageInfo *img;
	TK_MOUNT *mnt;
	int		lba, clid, sz;
	int i, j, k;

	tk_dbg_printf("tk_mount_sdfat\n");

	img=TKFAT_CreateSdFatContext();

	tk_dbg_printf("tk_mount_sdfat: B\n");

	mnt=tk_alloc_mount();
	mnt->vt=&tk_vfile_fat_vt;
	mnt->udata0=img;
	
	if(path)
	{
		mnt->src=tk_strdup_in(path);
		mnt->szSrc=strlen(mnt->src);
	}
	
	tk_vf_addmount(mnt);

	if(img->lba_dfs_start>0)
	{
	}

//	if(mnt->vt->fopen!=tk_fat_fopen)
//		{ __debugbreak(); }

	dee=&tdee;
	memset(dee, 0, sizeof(TKFAT_FAT_DirEntExt));

	tk_dbg_printf("tk_mount_sdfat: C\n");

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
		TK_VMem_Init();
	}
}

TK_MOUNT *tk_fat_mount(char *devfn, char *mntfn,
	char *fsty, char *mode, char **opts)
{
	TKFAT_FAT_DirEntExt tdee;
	TKFAT_FAT_DirEntExt *dee, *dee2;

	TKFAT_ImageInfo *img;
	TK_MOUNT *mnt;
	int		lba, clid, sz, devid;
	int i, j, k;

	tk_dbg_printf("tk_fat_mount\n");

	devid=TKBDEV_OpenPathAsDeviceId(devfn);

	if(devid<=0)
		return(0);

	img=TKFAT_TryOpenImage(devid, 0);
	if(!img)
	{
		return(0);
	}

	tk_dbg_printf("tk_fat_mount: B\n");

	mnt=tk_alloc_mount();
	mnt->vt=&tk_vfile_fat_vt;
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

	tk_dbg_printf("tk_fat_mount: C\n");
}

int tk_fat_mkfs(char *devfn, char **opts)
{
	TKFAT_ImageInfo *img;
	s64		devid;
	int		lba, clid, sz;
	int i, j, k;

	tk_dbg_printf("tk_dfs_mount\n");

	devid=TKBDEV_OpenPathAsDeviceId(devfn);

	if(devid<=0)
		return(0);

	sz=TKBDEV_GetSectorCount(devid);
	img=TKFAT_InitializeNewImage(devid, 0, sz);
	TKBDEV_ReleaseDevice(devid);
	return(1);
}

int tk_fat_fsctl(TK_MOUNT *mnt, TK_USERINFO *usri,
	char *name, int cmd, void *ptr)
{
	TKFAT_ImageInfo *img;
	int devid;

	img=mnt->udata0;

	if(cmd==TK_IOC_VFS_OPENBLKDEV)
	{
		devid=TKFAT_GetLvmDeviceId(img, name);
		*(s64 *)ptr=devid;
		return(1);
	}

	if(cmd==TK_IOC_VFS_MKFS)
	{
		return(tk_fat_mkfs(name, (char **)ptr));
	}

	return(-1);
}

TK_FILE *tk_fat_fopen(TK_MOUNT *mnt, TK_USERINFO *usri, char *name, char *mode)
{
	char tbuf[512];
	TKFAT_FAT_DirEntExt tdee;
	TKFAT_FAT_DirEntExt *dee, *dee2;
	TKFAT_ImageInfo *img;
	byte *clbuf;
	char *name1, *s1;
	TK_FILE *fd;
	int clid, lba;
	int i;

//	tk_dbg_printf("tk_fat_fopen: %s\n", name);

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
		if(mode[0]=='a')
			dee->is_write=1;
	}
	if(i<0)
	{
//		tk_dbg_printf("tk_fat_fopen: fail %s\n", name);
		return(NULL);
	}

//	if(dee->deb.attrib&0x10)
	if(dee->deb.attrib&0xD8)
		return(NULL);

	if(dee->deb.attrib==TKFAT_ATTR_LINKHINT)
	{
		clid=TKFAT_GetDirEntCluster(dee);
		if(clid>0)
		{
			lba=TKFAT_GetClusterLBA(img, clid);
			clbuf=TKFAT_GetSectorTempBuffer(img,
				lba, img->szclust);
			if(	(((u64 *)clbuf)[0]==0xBEAF5A52618F4181ULL) &&
				(((u64 *)clbuf)[1]==0x9C601B210E5B669CULL) &&
				(((u64 *)clbuf)[2]==0x5015D23C5B47459AULL) &&
				(((u64 *)clbuf)[3]==0xA0F43B3C94D04DC2ULL) &&
				(((u64 *)clbuf)[4]==0x9C601B210E5B669CULL) &&
				(((u64 *)clbuf)[5]==0x5015D23C5B47459AULL) &&
				(((u64 *)clbuf)[6]==0xA0F43B3C94D04DC2ULL) &&
				(((u64 *)clbuf)[7]==0xBEAF5A52618F4181ULL) )
			{
				/* Magic Symlink File */
				name1=clbuf+64;

	//			tk_dbg_printf("tk_fat_fopen: Symlink %s %s\n", name, name1);

				if(*name1=='/')
				{
					return(tk_fopen2(usri, name1, mode));
				}

				if(*name1==':')
				{
					return(tk_fat_fopen(mnt, usri, name1+1, mode));
				}
				
				strcpy(tbuf, name);
				s1=strrchr(tbuf, '/');
				if(s1)
				{
					strcpy(s1+1, name1);
					return(tk_fat_fopen(mnt, usri, tbuf, mode));
				}

				return(tk_fat_fopen(mnt, usri, name1, mode));
			}else
			{
	//			tk_dbg_printf("tk_fat_fopen: Not Link %s %X\n",
	//				name, ((u64 *)clbuf)[0]);
			}
		}
	}

	dee2=malloc(sizeof(TKFAT_FAT_DirEntExt));
	memcpy(dee2, dee, sizeof(TKFAT_FAT_DirEntExt));
	dee2->img=img;

	fd=tk_alloc_file();
	fd->vt=&tk_vfile_fat_vt;
	fd->udata0=img;
	fd->udata1=dee2;
	fd->ofs=0;
	fd->size=TKFAT_GetDirEntSize(dee2);

	if(mode[0]=='a')
	{
		fd->ofs=fd->size;
	}

	return(fd);
}

int tk_fat_unlink(TK_MOUNT *mnt, TK_USERINFO *usri, char *name)
{
	TKFAT_FAT_DirEntExt tdee;
	TKFAT_FAT_DirEntExt *dee, *dee2;
	TKFAT_ImageInfo *img;
	TK_FILE *fd;
	int i;

//	tk_dbg_printf("tk_fat_unlink: %s\n", name);

	img=mnt->udata0;
	dee=&tdee;
	memset(dee, 0, sizeof(TKFAT_FAT_DirEntExt));
	i=TKFAT_LookupDirEntPath(img, dee, name);
	if(i<0)
	{
//		tk_dbg_printf("tk_fat_unlink: fail %s\n", name);
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

int tk_fat_rmdir(TK_MOUNT *mnt, TK_USERINFO *usri, char *name)
{
	TKFAT_FAT_DirEntExt tdee;
	TKFAT_FAT_DirEntExt *dee, *dee2;
	TKFAT_ImageInfo *img;
	TK_FILE *fd;
	int i;

//	tk_dbg_printf("tk_fat_unlink: %s\n", name);

	img=mnt->udata0;
	dee=&tdee;
	memset(dee, 0, sizeof(TKFAT_FAT_DirEntExt));
	i=TKFAT_LookupDirEntPath(img, dee, name);
	if(i<0)
	{
//		tk_dbg_printf("tk_fat_unlink: fail %s\n", name);
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

int tk_fat_mkdir(TK_MOUNT *mnt, TK_USERINFO *usri, char *name, char *mode)
{
	char tb[260];
	TKFAT_FAT_DirEntExt tdee;
	TKFAT_FAT_DirEntExt *dee, *dee2;
	TKFAT_ImageInfo *img;
	TK_FILE *fd;
	int i;

//	tk_dbg_printf("tk_fat_fopen: %s\n", name);

	img=mnt->udata0;
	dee=&tdee;
	memset(dee, 0, sizeof(TKFAT_FAT_DirEntExt));

	strcpy(tb, name);
	strcat(tb, "/");

	i=TKFAT_CreateDirEntPath(img, dee, tb);
//	dee->is_write=1;
	return(1);
}


int tk_fat_rename(TK_MOUNT *mnt, TK_USERINFO *usri,
	char *oldfn, char *newfn, char *mode)
{
	char tbuf[512];

	TKFAT_FAT_DirEntExt tdee1, tdee2;
	TKFAT_FAT_DirEntExt *dee1, *dee2;
	TKFAT_ImageInfo *img;
	int i, sz;

	img=mnt->udata0;
	dee1=&tdee1;
	dee2=&tdee2;
	memset(dee1, 0, sizeof(TKFAT_FAT_DirEntExt));
	memset(dee2, 0, sizeof(TKFAT_FAT_DirEntExt));

	if(mode && *mode && (*mode!='r'))
	{
		if((*mode=='l') || (*mode=='s') || (*mode=='S'))
		{
			sz=64+strlen(oldfn)+2;
			((u64 *)tbuf)[0]=0xBEAF5A52618F4181ULL;
			((u64 *)tbuf)[1]=0x9C601B210E5B669CULL;
			((u64 *)tbuf)[2]=0x5015D23C5B47459AULL;
			((u64 *)tbuf)[3]=0xA0F43B3C94D04DC2ULL;
			((u64 *)tbuf)[4]=0x9C601B210E5B669CULL;
			((u64 *)tbuf)[5]=0x5015D23C5B47459AULL;
			((u64 *)tbuf)[6]=0xA0F43B3C94D04DC2ULL;
			((u64 *)tbuf)[7]=0xBEAF5A52618F4181ULL;
			
			if((*mode=='l') || (*mode=='L'))
			{
				tbuf[64]=':';
				strcpy(tbuf+65, oldfn);
			}else
			{
				strcpy(tbuf+64, oldfn);
			}

//			tk_dbg_printf("tk_fat_rename: ln %s %s\n", oldfn, newfn);

			i=TKFAT_CreateDirEntPath(img, dee2, newfn);
			if(i<0)
				return(-1);

//			tk_dbg_printf("tk_fat_rename: A1\n");

			TKFAT_ReadWriteDirEntFile(
				dee2, 0, true, tbuf, sz);

//			tk_dbg_printf("tk_fat_rename: A2\n");

			dee2->is_dirty=1;
			TKFAT_SetDirEntSize(dee2, sz);
			dee2->deb.attrib=TKFAT_ATTR_LINKHINT;

//			tk_dbg_printf("tk_fat_rename: A3\n");

			TKFAT_UpdateDirEnt(dee2);

//			tk_dbg_printf("tk_fat_rename: A4\n");

#if 0
			if(sz>256)
				sz=256;

			TKFAT_ReadWriteDirEntFile(
				dee2, 0, false, tbuf+256, sz);
			
			if(memcmp(tbuf, tbuf+256, sz))
			{
				tk_dbg_printf("tk_fat_rename: read mismatch\n");
			}

#endif

			return(1);
		}
	
		return(-1);
	}

	i=TKFAT_LookupDirEntPath(img, dee1, oldfn);
	if(i<0)
	{
//		tk_dbg_printf("tk_fat_rename: fail %s\n", oldfn);
		return(-1);
	}
	
	i=TKFAT_CreateDirEntPath(img, dee2, newfn);
	if(i<0)
	{
//		tk_dbg_printf("tk_fat_rename: fail %s\n", oldfn);
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

//	tk_dbg_printf("tk_fat_fclose: %p\n", fd);

	dee=fd->udata1;
	if(dee->is_dirty)
	{
		tk_dbg_printf("tk_fat_fclose: update and sync %p\n", fd);
		TKFAT_UpdateDirEnt(dee);
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
	int sz, szb;

	dee=fd->udata1;

	if(!(dee->is_write))
	{
		tk_dbg_printf("tk_fat_fwrite: write to file in read mode\n");
//		__debugbreak();
		return(-1);
	}

//	if((dee->is_write))
//	{
//		__debugbreak();
//		tk_dbg_printf("tk_fat_fwrite: write to file in read mode\n");
//		return(-1);
//	}

	sz=sz1*sz2;
	szb=TKFAT_ReadWriteDirEntFile(
		dee, fd->ofs, true, buf, sz);
	dee->is_dirty=1;
	
	if(szb!=sz)
	{
		tk_dbg_printf("tk_fat_fwrite: write size %d -> %d bytes\n", sz, sz1);
	}
	
	if(sz>0)fd->ofs+=sz;
	if(fd->ofs>fd->size)
	{
		fd->size=fd->ofs;
		dee->is_dirty=1;
		TKFAT_SetDirEntSize(dee, fd->size);
//		TKFAT_UpdateDirEnt(dee);
//		tk_dbg_printf("tk_fat_fwrite: expand file to %d bytes\n", fd->size);
	}
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


TK_DIR *tk_fat_opendir(TK_MOUNT *mnt, TK_USERINFO *usri, char *name)
{
	TKFAT_FAT_DirEntExt tdee, tdee1;
	TKFAT_FAT_DirEntExt *dee, *dee1, *dee2;
	TKFAT_ImageInfo *img;
	TK_DIRENT *tde;
	TK_DIR *fd;
	int dcli, mcli;
	int i;

//	tk_dbg_printf("tk_fat_opendir: %s\n", name);

	img=mnt->udata0;
	dee=&tdee;
	dee1=&tdee1;
	memset(dee, 0, sizeof(TKFAT_FAT_DirEntExt));
	memset(dee1, 0, sizeof(TKFAT_FAT_DirEntExt));
	i=TKFAT_LookupDirEntPath(img, dee, name);
	if(i<0)
	{
//		tk_dbg_printf("tk_fat_opendir: fail %s\n", name);
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

	dee2=tk_malloc(sizeof(TKFAT_FAT_DirEntExt));
	memset(dee2, 0, sizeof(TKFAT_FAT_DirEntExt));
//	memcpy(dee2, dee, sizeof(TKFAT_FAT_DirEntExt));
	dee2->img=img;
//	dee2->clid=dee->clid;
	dee2->clid=dcli;
	dee2->mclid=mcli;
	dee2->idx=-1;
	dee2->midx=-1;

	tde=tk_malloc(sizeof(TK_DIRENT));
	memset(tde, 0, sizeof(TK_DIRENT));

	fd=tk_alloc_dir();
	fd->vt=&tk_vfile_fat_vt;
	fd->udata0=img;
	fd->udata1=dee2;
	fd->udata2=tde;
	fd->udata3=mnt;
	fd->ofs=0;
	fd->size=TKFAT_GetDirEntSize(dee2);

	return(fd);
}

TK_DIRENT *tk_fat_readdir(TK_DIR *fd)
{
	TKFAT_FAT_DirEntExt *dee;
	TKFAT_ImageInfo *img;
	TK_MOUNT *mnt;
	TK_DIRENT *tde;
	byte *clbuf;
	char *name1;
	int clid, lba;
	int i;

	img=fd->udata0;
	dee=fd->udata1;
	tde=fd->udata2;
	mnt=fd->udata3;

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

	if(dee->deb.attrib==TKFAT_ATTR_LINKHINT)
	{
		clid=TKFAT_GetDirEntClusterRaw(dee);
		if(clid>0)
		{
			lba=TKFAT_GetClusterLBA(img, clid);
			clbuf=TKFAT_GetSectorTempBuffer(img,
				lba, img->szclust);
			if(	(((u64 *)clbuf)[0]==0xBEAF5A52618F4181ULL) &&
				(((u64 *)clbuf)[1]==0x9C601B210E5B669CULL) &&
				(((u64 *)clbuf)[2]==0x5015D23C5B47459AULL) &&
				(((u64 *)clbuf)[3]==0xA0F43B3C94D04DC2ULL) &&
				(((u64 *)clbuf)[4]==0x9C601B210E5B669CULL) &&
				(((u64 *)clbuf)[5]==0x5015D23C5B47459AULL) &&
				(((u64 *)clbuf)[6]==0xA0F43B3C94D04DC2ULL) &&
				(((u64 *)clbuf)[7]==0xBEAF5A52618F4181ULL) )
			{
				/* Magic Symlink File */
				name1=clbuf+64;

				memcpy(tde->st_link, name1, 256);
				tde->st_mode|=TKFAT_EMODE_LINK;
				
				if(*name1==':')
				{
					if(mnt->src && mnt->src[0])
					{
						sprintf(tde->st_link, "/%s/%s", mnt->src, name1+1);
					}else
					{
						tde->st_link[0]='/';
					}
				}
			}
		}
	}

	return(tde);
}

int tk_fat_closedir(TK_DIR *fd)
{
	tk_free(fd->udata1);
	tk_free(fd->udata2);
	tk_free_dir(fd);
	return(0);
}
