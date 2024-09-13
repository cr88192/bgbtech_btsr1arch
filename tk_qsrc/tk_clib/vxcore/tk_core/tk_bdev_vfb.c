TK_FILE *tk_bdvfb_fopen(TK_MOUNT *mnt, TK_USERINFO *usri, char *name, char *mode);
int tk_bdvfb_fread(void *buf, int sz1, int sz2, TK_FILE *fd);
int tk_bdvfb_fwrite(void *buf, int sz1, int sz2, TK_FILE *fd);
s64 tk_bdvfb_fseek(TK_FILE *fd, s64 ofs, int rel);
s64 tk_bdvfb_ftell(TK_FILE *fd);
int tk_bdvfb_fclose(TK_FILE *fd);
int tk_bdvfb_fgetc(TK_FILE *fd);
int tk_bdvfb_fputc(int ch, TK_FILE *fd);
int tk_bdvfb_fioctl(TK_FILE *fd, int cmd, void *ptr);

int tk_bdvfb_fsend(TK_FILE *fd, int cmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr);
int tk_bdvfb_frecv(TK_FILE *fd, int cmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr);

TK_FILE_VT tk_vfile_bdvfb_vt={
"bdev_vfb",			//fsname
NULL,				//next
NULL,				//mount
tk_bdvfb_fopen,		//fopen
NULL,				//fopendir
NULL,				//unlink
NULL,				//rename
NULL,				//fstat

NULL,				//mkdir
NULL,				//rmdir
NULL,				//fsctl

/* FILE Ops */
tk_bdvfb_fread,		//fread
tk_bdvfb_fwrite,		//fwrite
tk_bdvfb_fseek,		//fseek
tk_bdvfb_ftell,		//ftell
tk_bdvfb_fclose,		//fclose
tk_bdvfb_fgetc,		//fgetc
tk_bdvfb_fputc,		//fputc
tk_bdvfb_fioctl,		//ioctl

/* DIR ops */
NULL,				//readdir
NULL,				//closedir

/* Socket/Device Ops */
tk_bdvfb_fsend,		//fsend
tk_bdvfb_frecv		//frecv
};

static int tk_bdvfb_isinit=0;

int tk_bdvfb_init()
{
	char tbn[64];
	TK_BLKDEVINFO *bdi;
	TK_DEVFSDEV *tmp;
	char *s0;
	s64 maxlba;
	int lba, lbn, fsty;
	int i;

	if(tk_bdvfb_isinit)
		return(0);
	tk_bdvfb_isinit=1;

	printf("tk_bdvfb_init\n");

#if 1
	tmp=TK_DevFS_AllocDevice();
	tmp->vt=&tk_vfile_bdvfb_vt;
	tmp->src="vfb";
	
	bdi=(TK_BLKDEVINFO *)(tmp->budata);
	tmp->udata0=bdi;
//	bdi->lba_ofs=0;
//	bdi->lba_sz=maxlba;
	
	TK_DevFS_AddDevice(tmp);
#endif

	return(1);
}

TK_FILE *tk_bdvfb_fopen(TK_MOUNT *mnt, TK_USERINFO *usri,
	char *name, char *mode)
{
	TK_BLKDEVINFO *bdi;
	TK_FILE *fd;

	fd=tk_alloc_file();
	fd->vt=&tk_vfile_bdvfb_vt;

	bdi=mnt->udata0;

	fd->udata0=mnt;
	fd->udata1=bdi;
	fd->udata2=(void *)(0xFFFFF00A0000UL);
	fd->ofs=0;
//	fd->size=bdi->lba_sz<<9;
	fd->size=1<<17;

	return(fd);
}

int tk_bdvfb_fread(void *buf, int sz1, int sz2, TK_FILE *fd)
{
//	TK_BLKDEVINFO *bdi;
	u64 *cs, *ct, *cse;
	s64 ofs, sz, lba;

//	bdi=fd->udata1;
	ofs=fd->ofs;
	sz=sz1*sz2;

	if(sz<=0)
		return(0);

	if(!((ofs|sz)&7))
	{
		cs=(u64 *)(((byte *)(fd->udata2))+ofs);
		ct=(u64 *)buf;
		cse=cs+(sz>>3);
		
		while(cs<cse)
			{ *ct++=*cs++; }

		fd->ofs+=sz;
		return(sz);
	}
	
	if(sz>0)fd->ofs+=sz;
	return(sz);
}

int tk_bdvfb_fwrite(void *buf, int sz1, int sz2, TK_FILE *fd)
{
	TK_BLKDEVINFO *bdi;
	u64 *cs, *ct, *cse;
	s64 ofs, sz, lba;

	bdi=fd->udata1;
	ofs=fd->ofs;
	sz=sz1*sz2;
	
	if(sz<=0)
		return(0);

	if(!((ofs|sz)&31))
	{
		cs=(u64 *)buf;
		ct=(u64 *)(((byte *)(fd->udata2))+ofs);
		cse=cs+(sz>>3);
		
		while(cs<cse)
		{	ct[0]=cs[0];	ct[1]=cs[1];
			ct[2]=cs[2];	ct[3]=cs[3];
			ct+=4;			cs+=4;		}

		fd->ofs+=sz;
		return(sz);
	}
	
	if(!((ofs|sz)&7))
	{
		cs=(u64 *)buf;
		ct=(u64 *)(((byte *)(fd->udata2))+ofs);
		cse=cs+(sz>>3);
		
		while(cs<cse)
			{ *ct++=*cs++; }

		fd->ofs+=sz;
		return(sz);
	}
	
//	sz=TKFAT_ReadWriteDirEntFile(
//		fd->udata1, fd->ofs, false, buf, sz);
	if(sz>0)fd->ofs+=sz;
	return(sz);
}

s64 tk_bdvfb_fseek(TK_FILE *fd, s64 ofs, int rel)
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

s64 tk_bdvfb_ftell(TK_FILE *fd)
{
	return(fd->ofs);
}

int tk_bdvfb_fclose(TK_FILE *fd)
{
//	free(fd->udata1);
	tk_free_file(fd);
	return(0);
}

int tk_bdvfb_fgetc(TK_FILE *fd)
{
	byte buf[4];
	fd->vt->fread(buf, 1, 1, fd);
	return(buf[0]);
}

int tk_bdvfb_fputc(int ch, TK_FILE *fd)
{
	byte buf[4];
	ch=(byte)ch;
	buf[0]=ch;
	fd->vt->fwrite(buf, 1, 1, fd);
	return(ch);
}

int tk_bdvfb_fioctl(TK_FILE *fd, int cmd, void *ptr)
{
}

int tk_bdvfb_fsend(TK_FILE *fd, int cmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr)
{
	TK_VFB_RECT *rect;
	u64 *cs0, *cs1, *cs2, *cs3, *ct, *scr;
	int xo, yo, xs, ys;
	int xo1, yo1, xs1, ys1, ystr, bystr;
	int x, y;

	scr=fd->udata2;

	if((cmd==TK_IOC_VFB_COPYRECT) ||
		(cmd==TK_IOC_SENDTO))
	{
		rect=sockaddr;
		xo=rect->xofs;
		yo=rect->yofs;
		xs=rect->xsize;
		ys=rect->ysize;
		ystr=rect->ystr;
		
		bystr=(320/4)*4;
		
		if(!((xo|yo|xs|ys)&3))
		{
			xo1=xo>>2;	yo1=yo>>2;
			xs1=xs>>2;	ys1=ys>>2;
			for(y=0; y<ys1; y++)
			{
				cs0=(u64 *)(((u16 *)msgbuf)+((y*4+0)*ystr));
				cs1=(u64 *)(((u16 *)msgbuf)+((y*4+1)*ystr));
				cs2=(u64 *)(((u16 *)msgbuf)+((y*4+2)*ystr));
				cs3=(u64 *)(((u16 *)msgbuf)+((y*4+3)*ystr));
				ct=scr+((yo1+y)*bystr)+(xo1*4);
				for(x=0; x<xs1; x++)
				{
					ct[0]=cs0[0];	ct[1]=cs1[0];
					ct[2]=cs2[0];	ct[3]=cs3[0];
					cs0++;	cs1++;
					cs2++;	cs3++;
					ct+=4;
				}
			}
		}
		
		return(0);
	}

	return(-1);
}

int tk_bdvfb_frecv(TK_FILE *fd, int cmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr)
{
	return(-1);
}
