TK_FILE *tk_bdram_fopen(TK_MOUNT *mnt, char *name, char *mode);
int tk_bdram_fread(void *buf, int sz1, int sz2, TK_FILE *fd);
int tk_bdram_fwrite(void *buf, int sz1, int sz2, TK_FILE *fd);
s64 tk_bdram_fseek(TK_FILE *fd, s64 ofs, int rel);
s64 tk_bdram_ftell(TK_FILE *fd);
int tk_bdram_fclose(TK_FILE *fd);
int tk_bdram_fgetc(TK_FILE *fd);
int tk_bdram_fputc(int ch, TK_FILE *fd);
int tk_bdram_fioctl(TK_FILE *fd, int cmd, void *ptr);

TK_FILE_VT tk_vfile_bdram_vt={
"bdev_ramdsk",		//fsname
NULL,				//next
NULL,				//mount
tk_bdram_fopen,		//fopen
NULL,				//fopendir
NULL,				//unlink
NULL,				//rename
NULL,				//fstat

NULL,				//mkdir
NULL,				//rmdir
NULL,				//fsctl

/* FILE Ops */
tk_bdram_fread,		//fread
tk_bdram_fwrite,		//fwrite
tk_bdram_fseek,		//fseek
tk_bdram_ftell,		//ftell
tk_bdram_fclose,		//fclose
tk_bdram_fgetc,		//fgetc
tk_bdram_fputc,		//fputc
tk_bdram_fioctl,		//ioctl

/* DIR ops */
NULL,				//readdir
NULL,				//closedir

/* Socket/Device Ops */
NULL,				//fsend
NULL				//frecv
};

static int tk_bdram_isinit=0;
static int tk_bdram_rov=0;

int TK_BDRAM_CreateRamDisk(int maxlba)
{
	char tbn[64];
	TKFAT_MBR tmbr;
	TKFAT_MBR *mbr;
	TK_BLKDEVINFO *bdi;
	TK_DEVFSDEV *tmp;
	char *s0;
	s64 maxlba;
	int lba, lbn, fsty, id;
	int i;

	id=(tk_bdram_rov++)+1;
	sprintf(tbn, "ram%d", id);

	tmp=TK_DevFS_AllocDevice();
	tmp->vt=&tk_vfile_bdram_vt;
	tmp->src=tk_strdup_in(tbn);
	
	bdi=(TK_BLKDEVINFO *)(tmp->budata);
	tmp->udata0=bdi;

	bdi->lba_ofs=0;
	bdi->lba_sz=maxlba;
	bdi->buf_ptrs=NULL;
	
	TK_DevFS_AddDevice(tmp);
	
	return(id);
}

int tk_bdram_init()
{
	char tbn[64];
	TKFAT_MBR tmbr;
	TKFAT_MBR *mbr;
	TK_BLKDEVINFO *bdi;
	TK_DEVFSDEV *tmp;
	char *s0;
	s64 maxlba;
	int lba, lbn, fsty;
	int i;

	if(tk_bdram_isinit)
		return(0);
	tk_bdram_isinit=1;

//	TKSPI_InitDevice();

	printf("tk_bdram_init\n");

	return(1);
}

TK_FILE *tk_bdram_fopen(TK_MOUNT *mnt, char *name, char *mode)
{
	TK_BLKDEVINFO *bdi;
	TK_FILE *fd;

	fd=tk_alloc_file();
	fd->vt=&tk_vfile_bdram_vt;

	bdi=mnt->udata0;

	fd->udata0=mnt;
	fd->udata1=bdi;
	fd->ofs=0;
	fd->size=bdi->lba_sz<<9;

	return(fd);
}

int tk_bdram_fread(void *buf, int sz1, int sz2, TK_FILE *fd)
{
	TK_BLKDEVINFO *bdi;
	void **ptrs;
	byte *cs0, *ct, *cte;
	int ofs, ofs1, sz, szt, lba, lba1, lba2;

	bdi=fd->udata1;
	ofs=fd->ofs;
	sz=sz1*sz2;

	if(sz<=0)
		return(0);

	lba=ofs>>18;
	lba1=(ofs+sz)>>18;
	ofs1=ofs-(lba<<18);

	ptrs=bdi->buf_ptrs;
	if(!ptrs || (lba>=bdi->buf_max))
	{
		memset(buf, 0, sz);
		fd->ofs+=sz;
		return(sz);
	}
	
	if(lba==lba1)
	{
		cs0=ptrs[lba];
		if(!cs0)
		{
			memset(buf, 0, sz);
			fd->ofs+=sz;
			return(sz);
		}

		memcpy(buf, cs0+ofs1, sz);
		fd->ofs+=sz;
		return(sz);
	}
	
	ct=buf;
	cte=buf+sz;
	
	szt=(1<<18)-ofs1;
	memcpy(ct, cs0+ofs1, szt);
	ct+=szt;
	lba2=lba+1;

	while(ct<cte)
	{
		cs0=ptrs[lba2];
		if(!cs0)
		{
			szt=cte-ct;
			if(szt>(1<<18))
				szt=1<<18;

			memset(ct, 0, 1<<18);
			ct+=1<<18;
			lba2++;
			continue;
		}

		szt=cte-ct;
		if(szt>(1<<18))
			szt=1<<18;
		memcpy(ct, cs0, szt);
		ct+=szt;
		lba2++;
		continue;
	}

	if(sz>0)fd->ofs+=sz;
	return(sz);
}

int tk_bdram_fwrite(void *buf, int sz1, int sz2, TK_FILE *fd)
{
	TK_BLKDEVINFO *bdi;
	void **ptrs;
	byte *cs0, *ct, *cte;
	int ofs, ofs1, sz, szt, lba, lba1, lba2, lbamax;

	bdi=fd->udata1;
	ofs=fd->ofs;
	sz=sz1*sz2;

	if(sz<=0)
		return(0);

	lba=ofs>>18;
	lba1=(ofs+sz)>>18;
	ofs1=ofs-(lba<<18);

	ptrs=bdi->buf_ptrs;
	if(!ptrs)
	{
		lbamax=bdi->lba_sz>>9;
		bdi->buf_max=lbamax;
		
		ptrs=tk_malloc(lbamax*sizeof(void *));
		memset(ptrs, 0, lbamax*sizeof(void *));
		bdi->buf_ptrs=ptrs;
	}
	
	lbamax=bdi->buf_max;
	
	if(lba>=lbamax)
	{
		return(0);
	}
	
	if(lba==lba1)
	{
		cs0=ptrs[lba];
		if(!cs0)
		{
			cs0=tk_malloc(1<<18);
			memset(cs0, 0, 1<<18);
			ptrs[lba]=cs0;
		}

		memcpy(cs0+ofs1, buf, sz);
		fd->ofs+=sz;
		return(sz);
	}
	
	ct=buf;
	cte=buf+sz;
	
	szt=(1<<18)-ofs1;
	memcpy(ct, cs0+ofs1, szt);
	ct+=szt;
	lba2=lba+1;

	while((ct<cte) && (lba2<lbamax))
	{
		cs0=ptrs[lba2];
		if(!cs0)
		{
			cs0=tk_malloc(1<<18);
			memset(cs0, 0, 1<<18);
			ptrs[lba2]=cs0;
		}

		szt=cte-ct;
		if(szt>(1<<18))
			szt=1<<18;
		memcpy(cs0, ct, szt);
		ct+=szt;
		lba2++;
		continue;
	}

	if(sz>0)fd->ofs+=sz;
	return(sz);
}

s64 tk_bdram_fseek(TK_FILE *fd, s64 ofs, int rel)
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

s64 tk_bdram_ftell(TK_FILE *fd)
{
	return(fd->ofs);
}

int tk_bdram_fclose(TK_FILE *fd)
{
//	free(fd->udata1);
	tk_free_file(fd);
	return(0);
}

int tk_bdram_fgetc(TK_FILE *fd)
{
	byte buf[4];
	fd->vt->fread(buf, 1, 1, fd);
	return(buf[0]);
}

int tk_bdram_fputc(int ch, TK_FILE *fd)
{
	byte buf[4];
	ch=(byte)ch;
	buf[0]=ch;
	fd->vt->fwrite(buf, 1, 1, fd);
	return(ch);
}

int tk_bdram_fioctl(TK_FILE *fd, int cmd, void *ptr)
{
}
