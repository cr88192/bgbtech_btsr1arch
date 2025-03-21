TK_FILE *tk_bdspi_fopen(TK_MOUNT *mnt, TK_USERINFO *usri,
	char *name, char *mode);
int tk_bdspi_fread(void *buf, int sz1, int sz2, TK_FILE *fd);
int tk_bdspi_fwrite(void *buf, int sz1, int sz2, TK_FILE *fd);
s64 tk_bdspi_fseek(TK_FILE *fd, s64 ofs, int rel);
s64 tk_bdspi_ftell(TK_FILE *fd);
int tk_bdspi_fclose(TK_FILE *fd);
int tk_bdspi_fgetc(TK_FILE *fd);
int tk_bdspi_fputc(int ch, TK_FILE *fd);
int tk_bdspi_fioctl(TK_FILE *fd, int cmd, void *ptr);

int tk_bdspi_fsctl(TK_MOUNT *mnt, TK_USERINFO *usri,
	char *name, int cmd, void *ptr);

TK_FILE_VT tk_vfile_bdspi_vt={
"bdev_spi",			//fsname
NULL,				//next
NULL,				//mount
tk_bdspi_fopen,		//fopen
NULL,				//fopendir
NULL,				//unlink
NULL,				//rename
NULL,				//fstat

NULL,				//mkdir
NULL,				//rmdir
tk_bdspi_fsctl,		//fsctl

/* FILE Ops */
tk_bdspi_fread,		//fread
tk_bdspi_fwrite,		//fwrite
tk_bdspi_fseek,		//fseek
tk_bdspi_ftell,		//ftell
tk_bdspi_fclose,		//fclose
tk_bdspi_fgetc,		//fgetc
tk_bdspi_fputc,		//fputc
tk_bdspi_fioctl,		//ioctl

/* DIR ops */
NULL,				//readdir
NULL,				//closedir

/* Socket/Device Ops */
NULL,				//fsend
NULL				//frecv
};


int tk_bdspi_ReadSectors(TK_BLKDEV_CTX *ctx, byte *buf, s64 lba, int cnt)
{
	return(TKSPI_ReadSectors(buf, lba+ctx->udata_i0, cnt));
}

int tk_bdspi_WriteSectors(TK_BLKDEV_CTX *ctx, byte *buf, s64 lba, int cnt)
{
	return(TKSPI_WriteSectors(buf, lba+ctx->udata_i0, cnt));
}

TK_BLKDEV_VT tk_bdspi_blkdev_vt = {
"bdspi",
NULL,
NULL,
NULL,
tk_bdspi_ReadSectors,
tk_bdspi_WriteSectors
};

int tk_bdspi_sdix[8][12];

static int tk_bdspi_isinit=0;

int tk_bdspi_dfs_lba;
int tk_bdspi_dfs_lbn;
int tk_bdspi_dfs_fsty;

int tk_bdspi_init()
{
	char tbn[64];
	TKFAT_MBR tmbr;
	TKFAT_MBR *mbr;
	TK_BLKDEVINFO *bdi;
	TK_BLKDEV_CTX *bdctx;
	TK_DEVFSDEV *tmp;
	char *s0;
	s64 maxlba;
	int lba, lbn, fsty;
	int i;

	if(tk_bdspi_isinit)
		return(0);
	tk_bdspi_isinit=1;

	TKSPI_InitDevice();

	tk_dbg_printf("tk_bdspi_init\n");

#if 1
	mbr=&tmbr;
	TKSPI_ReadSectors((byte *)mbr, 0, 1);

	maxlba=0;
	for(i=0; i<4; i++)
	{
		fsty=mbr->entry[i].fstype;
		lba=tkfat_getDWord(mbr->entry[i].lba_start);
		lbn=tkfat_getDWord(mbr->entry[i].lba_count);
		
		s0=tkfat_fstnameforfsty(fsty);
		
		if((lba+lbn)>maxlba)
			maxlba=lba+lbn;

		if(	(fsty==0x3E) ||
			(fsty==0x3F))
		{
			if(tk_bdspi_dfs_lba<=0)
			{
				tk_bdspi_dfs_lba=lba;
				tk_bdspi_dfs_lbn=lbn;
				tk_bdspi_dfs_fsty=fsty;
			}
		}

		tk_dbg_printf("  %08X %08X %02X %s\n", lba, lbn, fsty, s0);
	}

	tk_dbg_printf("tk_bdspi_init: sda 0\n");

	tmp=TK_DevFS_AllocDevice();
	tmp->vt=&tk_vfile_bdspi_vt;
	tmp->src="sda";
	
	bdi=(TK_BLKDEVINFO *)(tmp->budata);
	tmp->udata0=bdi;
	bdi->lba_ofs=0;
	bdi->lba_sz=maxlba;
	
	TK_DevFS_AddDevice(tmp);

	for(i=0; i<4; i++)
	{
		sprintf(tbn, "sda%d", i+1);
	
		fsty=mbr->entry[i].fstype;
		if(!fsty)
			continue;
		
		lba=tkfat_getDWord(mbr->entry[i].lba_start);
		lbn=tkfat_getDWord(mbr->entry[i].lba_count);
		
		tmp=TK_DevFS_AllocDevice();
		tmp->vt=&tk_vfile_bdspi_vt;
		tmp->src=TKMM_LVA_Strdup(tbn);
		
		bdi=(TK_BLKDEVINFO *)(tmp->budata);
		tmp->udata0=bdi;
		bdi->lba_ofs=lba;
		bdi->lba_sz=lbn;
		
		TK_DevFS_AddDevice(tmp);

		bdctx=TKBDEV_AllocNewDevice();
		bdctx->vt=&tk_bdspi_blkdev_vt;
		bdctx->udata_i0=lba;
		bdctx->udata_i1=lbn;
		bdctx->lba_base=lba;
		bdctx->lbn_base=lbn;
		
		bdi->devid=bdctx->bdev;
		
		tk_bdspi_sdix[0][i]=bdctx->bdev;
	}
#endif

	return(1);
}

TK_FILE *tk_bdspi_fopen(TK_MOUNT *mnt, TK_USERINFO *usri,
	char *name, char *mode)
{
	TK_BLKDEVINFO *bdi;
	TK_FILE *fd;

	fd=tk_alloc_file();
	fd->vt=&tk_vfile_bdspi_vt;

	bdi=mnt->udata0;

	fd->udata0=mnt;
	fd->udata1=bdi;
	fd->ofs=0;
	fd->size=bdi->lba_sz<<9;

	return(fd);
}

int tk_bdspi_fread(void *buf, int sz1, int sz2, TK_FILE *fd)
{
	TK_BLKDEVINFO *bdi;
	s64 ofs, sz, lba;

	bdi=fd->udata1;
	ofs=fd->ofs;
	sz=sz1*sz2;

	if(sz<=0)
		return(0);

	if(!((ofs|sz)&511))
	{
		lba=(bdi->lba_ofs)+(ofs>>9);
		TKSPI_ReadSectors(buf, lba, sz>>9);
		fd->ofs+=sz;
		return(sz);
	}
	
//	sz=TKFAT_ReadWriteDirEntFile(
//		fd->udata1, fd->ofs, false, buf, sz);
	if(sz>0)fd->ofs+=sz;
	return(sz);
}

int tk_bdspi_fwrite(void *buf, int sz1, int sz2, TK_FILE *fd)
{
	TK_BLKDEVINFO *bdi;
	s64 ofs, sz, lba;

	bdi=fd->udata1;
	ofs=fd->ofs;
	sz=sz1*sz2;
	
	if(sz<=0)
		return(0);
	
	if(!((ofs|sz)&511))
	{
		lba=(bdi->lba_ofs)+(ofs>>9);
		TKSPI_WriteSectors(buf, lba, sz>>9);
		fd->ofs+=sz;
		return(sz);
	}
	
//	sz=TKFAT_ReadWriteDirEntFile(
//		fd->udata1, fd->ofs, false, buf, sz);
	if(sz>0)fd->ofs+=sz;
	return(sz);
}

s64 tk_bdspi_fseek(TK_FILE *fd, s64 ofs, int rel)
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

s64 tk_bdspi_ftell(TK_FILE *fd)
{
	return(fd->ofs);
}

int tk_bdspi_fclose(TK_FILE *fd)
{
//	free(fd->udata1);
	tk_free_file(fd);
	return(0);
}

int tk_bdspi_fgetc(TK_FILE *fd)
{
	byte buf[4];
	fd->vt->fread(buf, 1, 1, fd);
	return(buf[0]);
}

int tk_bdspi_fputc(int ch, TK_FILE *fd)
{
	byte buf[4];
	ch=(byte)ch;
	buf[0]=ch;
	fd->vt->fwrite(buf, 1, 1, fd);
	return(ch);
}

int tk_bdspi_fioctl(TK_FILE *fd, int cmd, void *ptr)
{
}

int tk_bdspi_fsctl(TK_MOUNT *mnt, TK_USERINFO *usri,
	char *name, int cmd, void *ptr)
{
	int devid;

	if(cmd==TK_IOC_VFS_OPENBLKDEV)
	{
		if(!strcmp(name, "sda"))
		{
			*(s64 *)ptr=tkspi_id_bdev;
			return(1);
		}
		
		if((name[0]=='s') && (name[1]=='d') &&
			(name[2]>='a') && (name[2]<='z') &&
			(name[3]>='1') && (name[3]<='8') &&
			!name[4])
		{
			*(s64 *)ptr=tk_bdspi_sdix[name[2]-'a'][name[3]-'1'];
			return(1);
		}

//		tk_bdspi_sdix=
	
//		devid=TKFAT_GetLvmDeviceId(img, name);
//		*(s64 *)ptr=tkspi_id_bdev;
//		return(0);
	}

	return(-1);
}
