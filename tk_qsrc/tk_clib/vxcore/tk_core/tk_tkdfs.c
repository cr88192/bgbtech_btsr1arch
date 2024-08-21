u32 TKDFS_ReadImageInodeWord32(TKDFS_ImageContext *img,
	int d_ino, int d_idx);
int TKDFS_WriteImageInodeWord32(TKDFS_ImageContext *img,
	int d_ino, int d_idx, u32 d_val);

TKDFS_ImageContext *TKDFS_AllocContext()
{
	TKDFS_ImageContext *tmp;
	
	tmp=tk_malloc(sizeof(TKDFS_ImageContext));
	return(tmp);
}


TKDFS_ImageContext *TKDFS_TryOpenImage(int bdev, s64 lbastart)
{
	byte tblk[512];
	TKDFS_BootBlock *bblk;
	TKDFS_ImageContext *img;
	int i, j, k;

	TKBDEV_ReadSectors(bdev, tblk, lbastart, 1);
	
	bblk=(TKDFS_BootBlock *)tblk;
	if(memcmp(bblk->oem_name, "TKDFS2  ", 8))
		return(NULL);

	img=TKDFS_AllocContext();
	img->bootblk=(TKDFS_BootBlock *)(img->t_bootblk);
	memcpy(img->t_bootblk, tblk, 512);
	
	img->bdev=bdev;
	bblk=img->bootblk;
	img->lba_start=lbastart;
	img->lba_count=bblk->lba_count64;
	
	k=bblk->sectors_cluster;
	j=0;
	for(i=0; i<8; i++)
		if((1<<i)==k)
			j=i;

	img->ln2_blksz=9+j;
	img->ln2_inosz=bblk->ln2_sz_ino;
	
//	img->lba_inotab=lbastart+bblk->blk_inotab;
	img->blk_inotab=bblk->blk_inotab;

	return(img);
}

byte *TKDFS_GetImageCachedBlock(TKDFS_ImageContext *img, int d_blk, int d_flg)
{
	byte *ptr;
	s64 lba;
	int h, sh, cblkid;
	
	sh=img->ln2_blksz-9;
	h=(((d_blk*251)>>8)&127)>>sh;
	ptr=img->img_blkcache_data+(h<<(9+sh));
	
	cblkid=img->img_blkcache_blkid[h];
	if(cblkid==d_blk)
	{
		if(d_flg&1)
			img->img_blkcache_flag[h]|=1;
		return(ptr);
	}
	
	if(img->img_blkcache_flag[h]&1)
	{
		lba=img->lba_start+(cblkid<<sh);
		TKBDEV_WriteSectors(img->bdev, ptr, lba, 1<<sh);
	}
	
	lba=img->lba_start+(d_blk<<sh);
	TKBDEV_ReadSectors(img->bdev, ptr, lba, 1<<sh);
	img->img_blkcache_blkid[h]=d_blk;
	img->img_blkcache_flag[h]=0;
	if(d_flg&1)
		img->img_blkcache_flag[h]|=1;
	return(ptr);
}

int TKDFS_SyncImageCachedBlocks(TKDFS_ImageContext *img)
{
	byte *ptr;
	s64 lba;
	int h, n, sh, cblkid;
	
	sh=img->ln2_blksz-9;
	n=128>>sh;

	for(h=0; h<n; h++)
	{
		ptr=img->img_blkcache_data+(h<<(9+sh));
		
		cblkid=img->img_blkcache_blkid[h];
		
		if(img->img_blkcache_flag[h]&1)
		{
			lba=img->lba_start+(cblkid<<sh);
			TKBDEV_WriteSectors(img->bdev, ptr, lba, 1<<sh);
			img->img_blkcache_flag[h]=0;
		}
	}
	return(0);
}

int TKDFS_ImageAllocateBitmap(TKDFS_ImageContext *img, int ino, int lim)
{
	u32 bm0;
	int h, n, n1, sh;
	int i, j, k;
	
	n=lim;
	n1=n>>5;
	
	for(i=0; i<n1; i++)
	{
		bm0=TKDFS_ReadImageInodeWord32(img, ino, i>>5);
		if(bm0==0xFFFFFFFFU)
			continue;
		for(j=0; j<32; j++)
		{
			if(!(bm0&(1U<<j)))
			{
				bm0|=(1U<<j);
				TKDFS_WriteImageInodeWord32(img, ino, i>>5, bm0);
				return((i<<5)|j);
			}
		}
	}

	/* no free space... */
	return(0);
}

int TKDFS_ImageAllocateBlock(TKDFS_ImageContext *img)
{
	int sh, n;

	sh=img->ln2_blksz-9;
	n=img->lba_count>>sh;

	return(TKDFS_ImageAllocateBitmap(img, 2, n));
}

int TKDFS_ImageAllocateInode(TKDFS_ImageContext *img)
{
	int sh, n;

//	sh=img->ln2_blksz-9;
//	n=img->lba_count>>sh;

	n=1<<20;

	return(TKDFS_ImageAllocateBitmap(img, 3, n));
}

u32 TKDFS_ReadBlockWord32(TKDFS_ImageContext *img, int d_blk, int d_idx)
{
	byte *blk;
	
	blk=TKDFS_GetImageCachedBlock(img, d_blk, 0);
	return(((u32 *)blk)[d_idx]);
}

u64 TKDFS_ReadBlockWord64(TKDFS_ImageContext *img, int d_blk, int d_idx)
{
	byte *blk;
	
	blk=TKDFS_GetImageCachedBlock(img, d_blk, 0);
	return(((u64 *)blk)[d_idx]);
}

u32 TKDFS_WriteBlockWord32(TKDFS_ImageContext *img,
	int d_blk, int d_idx, u32 d_val)
{
	byte *blk;
	
	blk=TKDFS_GetImageCachedBlock(img, d_blk, 1);
	((u32 *)blk)[d_idx]=d_val;
	return(0);
}

u64 TKDFS_WriteBlockWord64(TKDFS_ImageContext *img,
	int d_blk, int d_idx, u64 d_val)
{
	byte *blk;
	
	blk=TKDFS_GetImageCachedBlock(img, d_blk, 1);
	((u64 *)blk)[d_idx]=d_val;
	return(0);
}


byte *TKDFS_GetImageCachedInodeBlock(
	TKDFS_ImageContext *img, int d_ino, int d_blk, int d_flg);

int TKDFS_UnpackImageInode(TKDFS_ImageContext *img,
	TKDFS_InodeInfo *info, int ino)
{
	s64 ino_offs;
	u16 tg16;
	u32 tg32, tl32;
	int lba, tgofs, tglen, inoblk, ino_brem;
	int i, j, k;

	info->ino_file=NULL;
	info->ino_idx4=NULL;
	info->ino_idat=NULL;
	
	j=0;
	for(i=0; i<32; i++)
	{
		tg32=*(u32 *)(info->t_data+(i*4));
		if(!tg32)
		{
			/* end of list */
			break;
		}
		if(tg32&0x8080)
		{
			/* length/offset */
			continue;
		}

		if(tg32&0x80800000)
		{
			/* TWOCC */
			tg16=(u16)tg32;
			tglen=(((~tg32)>>16)&255)*8;
			tgofs=(tg32>>24)*8;
		}else
		{
			tg16=0;
			tl32=*(u32 *)(info->t_data+(i*4)+4);
			tglen=(~tl32)&0xFFFF;
			tgofs=(tl32>>16)&0xFFFF;
		}
		
		if((tg32==TKDFS_FCC_FHDR) || (tg16==TKDFS_TCC_FH))
		{
			info->ino_file=(TKDFS_INode_File *)(info->t_data+tgofs);
		}

		if((tg32==TKDFS_FCC_IDX4) || (tg16==TKDFS_TCC_I4))
		{
			info->ino_idx4=(TKDFS_INode_FileBlockIndex4 *)(info->t_data+tgofs);
		}

		if((tg32==TKDFS_FCC_IDAT) || (tg16==TKDFS_TCC_ID))
		{
			info->ino_idat=info->t_data+tgofs;
		}
	}
	return(1);
}

int TKDFS_ReadImageInode(TKDFS_ImageContext *img,
	TKDFS_InodeInfo *info, int ino)
{
//	byte tblk[512];
//	TKDFS_InodeInfo *ino0_info;
	byte *pblk;
	s64 ino_offs;
	u16 tg16;
	u32 tg32, tl32;
	int lba, tgofs, tglen, inoblk, ino_brem;
	int i, j, k;

	ino_offs=((s64)ino)<<img->ln2_inosz;
	ino_brem=ino_offs&((1<<img->ln2_inosz)-1);
	
	if(ino<32)
	{
		/* assumed fixed inodes are directly accessible */
		inoblk=img->blk_inotab+(ino_offs>>img->ln2_blksz);
		pblk=TKDFS_GetImageCachedBlock(img, inoblk, 0);
	}else
	{
		inoblk=ino_offs>>img->ln2_blksz;
		pblk=TKDFS_GetImageCachedInodeBlock(img, 0, inoblk, 0);
	}

	memcpy(info->t_data, pblk+ino_brem, 1<<img->ln2_inosz);

	TKDFS_UnpackImageInode(img, info, ino);
	return(1);
}

int TKDFS_WriteImageInode(TKDFS_ImageContext *img,
	TKDFS_InodeInfo *info, int ino)
{
//	byte tblk[512];
//	TKDFS_InodeInfo *ino0_info;
	byte *pblk;
	s64 ino_offs;
	u16 tg16;
	u32 tg32, tl32;
	int lba, tgofs, tglen, inoblk, ino_brem;
	int i, j, k;

	ino_offs=((s64)ino)<<img->ln2_inosz;
	ino_brem=ino_offs&((1<<img->ln2_inosz)-1);
	
	if(ino<32)
	{
		/* assumed fixed inodes are directly accessible */
		inoblk=img->blk_inotab+(ino_offs>>img->ln2_blksz);
		pblk=TKDFS_GetImageCachedBlock(img, inoblk, 1);
	}else
	{
		inoblk=ino_offs>>img->ln2_blksz;
		pblk=TKDFS_GetImageCachedInodeBlock(img, 1, inoblk, 1);
	}

	memcpy(pblk+ino_brem, info->t_data, 1<<img->ln2_inosz);
	return(1);
}

TKDFS_InodeInfo *TKDFS_GetImageCachedInode(TKDFS_ImageContext *img,
	int d_ino, int d_flg)
{
	TKDFS_InodeInfo *ino_inf;
	int h;
	
	h=((d_ino*251)>>8)&63;
	ino_inf=img->img_inocache+h;
	if(ino_inf->d_ino==d_ino)
	{
		return(ino_inf);
	}
	
	TKDFS_ReadImageInode(img, ino_inf, d_ino);
	ino_inf->d_ino=d_ino;
	return(ino_inf);
}

int TKDFS_InitializeImageFileInode(TKDFS_ImageContext *img,
	TKDFS_InodeInfo *info, int ino)
{
	u32 *taga;
	byte *ct;
	int l0, o0, tl1;
	
	ct=info->t_data;
	memset(ct, 0, 512);
	
	taga=(u32 *)ct;
	ct+=32;
	
	taga[0]=TKDFS_FCC_FHDR;
	taga[1]=0;
	taga[2]=TKDFS_FCC_IDX4;
	taga[3]=0;
	taga[4]=0;
	taga[5]=0;
	
	l0=sizeof(TKDFS_INode_File);
	l0=(l0+7)&(~7);
	o0=ct-info->t_data;
	
	tl1=((~l0)&0xFFFF)|(o0<<16);
	taga[1]=tl1;
	
	info->ino_file=(TKDFS_INode_File *)ct;
	ct+=l0;


	l0=sizeof(TKDFS_INode_FileBlockIndex4);
	l0=(l0+7)&(~7);
	o0=ct-info->t_data;
	
	tl1=((~l0)&0xFFFF)|(o0<<16);
	taga[3]=tl1;
	
	info->ino_idx4=(TKDFS_INode_FileBlockIndex4 *)ct;
	ct+=l0;
}

byte *TKDFS_GetImageCachedInodeBlock(TKDFS_ImageContext *img,
	int d_ino, int d_blk, int d_flg)
{
	TKDFS_InodeInfo *ino_inf;
	byte *blkptr;
	int blknum, blkofs, idblksh;
	int idx0, idx1, idx2, idx0r, idx1r, idx2r, blk0, blk1, blk2;
	
	ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 0);
	blkofs=d_blk;
	
	if(!ino_inf->ino_file)
	{
		/* Uninitialized Inode */
		if(!(d_flg&1))
			return(NULL);
		ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
		TKDFS_InitializeImageFileInode(img, ino_inf, d_ino);

		ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
		if(!ino_inf->ino_file)
			return(NULL);
	}

	if(ino_inf->ino_idx4)
	{
		idblksh=img->ln2_blksz-2;
		idx0=blkofs>>idblksh;
		idx0r=blkofs&((1<<idblksh)-1);
		idx1=idx0>>idblksh;
		idx1r=idx0&((1<<idblksh)-1);
		idx2=idx1>>idblksh;
		idx2r=idx1&((1<<idblksh)-1);
		if(blkofs<16)
		{
			blknum=ino_inf->ino_idx4->block_0l[blkofs];
			if(!blknum && (d_flg&1))
			{
				ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
				blknum=TKDFS_ImageAllocateBlock(img);
				ino_inf->ino_idx4->block_0l[blkofs]=blknum;
			}
		}else if(idx0<8)
		{
			blk0=ino_inf->ino_idx4->block_1l[idx0];
			if(!blk0 && (d_flg&1))
			{
				ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
				blk0=TKDFS_ImageAllocateBlock(img);
				ino_inf->ino_idx4->block_1l[idx0]=blk0;
			}
			if(!blk0)
				return(NULL);
			blknum=TKDFS_ReadBlockWord32(img, blk0, idx0r);
			if(!blknum && (d_flg&1))
			{
				blknum=TKDFS_ImageAllocateBlock(img);
				TKDFS_WriteBlockWord32(img, blk0, idx0r, blknum);
			}
		}else if(idx1<4)
		{
			blk1=ino_inf->ino_idx4->block_2l[idx1];
			if(!blk1 && (d_flg&1))
			{
				ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
				blk1=TKDFS_ImageAllocateBlock(img);
				ino_inf->ino_idx4->block_2l[idx1]=blk1;
			}
			if(!blk1)
				return(NULL);
			blk0=TKDFS_ReadBlockWord32(img, blk1, idx1r);
			if(!blk0 && (d_flg&1))
			{
				blk0=TKDFS_ImageAllocateBlock(img);
				TKDFS_WriteBlockWord32(img, blk1, idx1r, blk0);
			}
			if(!blk0)
				return(NULL);
			blknum=TKDFS_ReadBlockWord32(img, blk0, idx0r);
			if(!blknum && (d_flg&1))
			{
				blknum=TKDFS_ImageAllocateBlock(img);
				TKDFS_WriteBlockWord32(img, blk0, idx0r, blknum);
			}
		}else if(idx2<2)
		{
			blk2=ino_inf->ino_idx4->block_3l[idx2];
			if(!blk2 && (d_flg&1))
			{
				ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
				blk2=TKDFS_ImageAllocateBlock(img);
				ino_inf->ino_idx4->block_3l[idx2]=blk2;
			}
			if(!blk2)
				return(NULL);
			blk1=TKDFS_ReadBlockWord32(img, blk2, idx2r);
			if(!blk1 && (d_flg&1))
			{
				blk1=TKDFS_ImageAllocateBlock(img);
				TKDFS_WriteBlockWord32(img, blk2, idx2r, blk1);
			}
			if(!blk1)
				return(NULL);
			blk0=TKDFS_ReadBlockWord32(img, blk1, idx1r);
			if(!blk0 && (d_flg&1))
			{
				blk0=TKDFS_ImageAllocateBlock(img);
				TKDFS_WriteBlockWord32(img, blk1, idx1r, blk0);
			}
			if(!blk0)
				return(NULL);
			blknum=TKDFS_ReadBlockWord32(img, blk0, idx0r);
			if(!blknum && (d_flg&1))
			{
				blknum=TKDFS_ImageAllocateBlock(img);
				TKDFS_WriteBlockWord32(img, blk0, idx0r, blknum);
			}
		}
	}

	if(!blknum)
		return(NULL);

	blkptr=TKDFS_GetImageCachedBlock(img, blknum, d_flg);
	return(blkptr);
}

u32 TKDFS_ReadImageInodeWord32(TKDFS_ImageContext *img,
	int d_ino, int d_idx)
{
	byte *ptr;
	u32 d_val;
	int sh, d_blk, d_ix2;

	sh=img->ln2_blksz;
	d_blk=d_idx>>(sh-2);
	d_ix2=d_idx&((1<<(sh-2))-1);
	ptr=TKDFS_GetImageCachedInodeBlock(img, d_ino, d_blk, 0);
	
	d_val=((u32 *)ptr)[d_ix2];
	return(d_val);
}

int TKDFS_WriteImageInodeWord32(TKDFS_ImageContext *img,
	int d_ino, int d_idx, u32 d_val)
{
	byte *ptr;
	int sh, d_blk, d_ix2;
	sh=img->ln2_blksz;
	d_blk=d_idx>>(sh-2);
	d_ix2=d_idx&((1<<(sh-2))-1);
	ptr=TKDFS_GetImageCachedInodeBlock(img, d_ino, d_blk, 1);
	((u32 *)ptr)[d_ix2]=d_val;
	return(0);
}

int TKDFS_ReadImageDirent(TKDFS_ImageContext *img,
	TKDFS_DirentInfo *info, int d_ino, int d_idx)
{
	TKDFS_DirEnt_Basic *deb;
	byte *blkptr, *deptr;
	char *ct;
	int ofs, blkofs, blkrem;

	ofs=d_idx*64;
	blkofs=ofs>>img->ln2_blksz;
	blkrem=ofs&((1<<img->ln2_blksz)-1);
	blkptr=TKDFS_GetImageCachedInodeBlock(img, d_ino, blkofs, 0);
	deptr=blkptr+blkrem;
	
//	deb=(TKDFS_DirEnt_Basic *)deptr;
	deb=&(info->t_deb);
	info->deb=deb;
	info->deh=(TKDFS_DirEnt_Header *)deb;
	memcpy(deb, deptr, 64);
	
	memcpy(info->de_name, deb->name, 48);
	info->de_ino=deb->ino;
	
	if(deb->etype==2)
	{
		/* file with multipart name */
		ct=info->de_name+48;
		while(1)
		{
			ofs+=64;
			blkofs=ofs>>img->ln2_blksz;
			blkrem=ofs&((1<<img->ln2_blksz)-1);
			blkptr=TKDFS_GetImageCachedInodeBlock(img, d_ino, blkofs, 0);
			deptr=blkptr+blkrem;
			if(deptr[15]!=3)
				break;
			memcpy(ct+ 0, deptr+ 0, 12);
			memcpy(ct+12, deptr+16, 48);
			ct+=60;
		}
		*ct=0;
	}
	
	return(0);
}

int TKDFS_ShortNameHash48(byte *name)
{
	u32 *nptr;
	u32 i0, i1, i2, i3;
	u32 j0, j1, j2, j3;
	u32 k0, k1, k2, k3;
	int h0, h1, h2, h;

	nptr=(u32 *)name;

#if 1
	j0=nptr[0];			j1=nptr[1];
	j2=nptr[2];			j3=nptr[3];
	k0=j0;				k1=j1;
	k2=j2;				k3=j3;
	i0=nptr[4];			i1=nptr[5];
	i2=nptr[6];			i3=nptr[7];
	j0+=i0;				j1+=i1;
	j2+=i2;				j3+=i3;
	k0+=j0;				k1+=j1;
	k2+=j2;				k3+=j3;
	i0=nptr[ 8];		i1=nptr[ 9];
	i2=nptr[10];		i3=nptr[11];
	j0+=i0;				j1+=i1;
	j2+=i2;				j3+=i3;
	k0+=j0;				k1+=j1;
	k2+=j2;				k3+=j3;

	i0=j0+k0;			i1=j1+k1;
	i2=j2+k2;			i3=j3+k3;

	i0=i0+i1;			i1=i2+i3;
	i2=i0+i1;
	h=i2+(i2>>16);
	h=h+(h>>8);
	return(h&255);
#endif

#if 0
	i0=nptr[0];			i1=nptr[1];
	i2=nptr[2];			i3=nptr[3];
	j0=i0*0x00FEDCA9;	j1=i1*0x00FEDCA9;
	j2=i2*0x00FEDCA9;	j3=i3*0x00FEDCA9;
	j0=j0>>24;			j1=j1>>24;
	j2=j2>>24;			j3=j3>>24;
	h0=(((j0*251+j1)*251+j2)*251+j3)*251;

	i0=nptr[4];			i1=nptr[5];
	i2=nptr[6];			i3=nptr[7];
	j0=i0*0x00FEDCA9;	j1=i1*0x00FEDCA9;
	j2=i2*0x00FEDCA9;	j3=i3*0x00FEDCA9;
	j0=j0>>24;			j1=j1>>24;
	j2=j2>>24;			j3=j3>>24;
	h1=(((j0*251+j1)*251+j2)*251+j3)*251;

	i0=nptr[ 8];		i1=nptr[ 9];
	i2=nptr[10];		i3=nptr[11];
	j0=i0*0x00FEDCA9;	j1=i1*0x00FEDCA9;
	j2=i2*0x00FEDCA9;	j3=i3*0x00FEDCA9;
	j0=j0>>24;			j1=j1>>24;
	j2=j2>>24;			j3=j3>>24;
	h2=(((j0*251+j1)*251+j2)*251+j3)*251;

	h=((h0*251+h1)*251+h2)*251;
	h=(h>>8)&255;
#endif

	return(h);
}

int TKDFS_WriteImageDirent(TKDFS_ImageContext *img,
	TKDFS_DirentInfo *info, int d_ino, int d_idx)
{
	TKDFS_DirEnt_Basic *deb;
	byte *blkptr, *deptr;
	char *cs, *ct;
	int ofs, blkofs, blkrem, nln, nl1, feh;

	ofs=d_idx*64;
	blkofs=ofs>>img->ln2_blksz;
	blkrem=ofs&((1<<img->ln2_blksz)-1);
	blkptr=TKDFS_GetImageCachedInodeBlock(img, d_ino, blkofs, 1);
	deptr=blkptr+blkrem;

	deb=&(info->t_deb);
	info->deb=deb;

	nln=strlen(info->de_name);
	nl1=nln;
	if(nl1>48)
		nl1=48;

	memset(deb->name, 0, 48);
	memcpy(deb->name, info->de_name, nl1);
	deb->ino=info->de_ino;

	if(nln>48)
	{
		feh=TKDFS_ShortNameHash48(deb->name);
		deb->etype=2;
	}else
	{
		feh=0;
		deb->etype=1;
	}

	memcpy(deptr, deb, 64);
		
	if(deb->etype==2)
	{
		/* file with multipart name */
		cs=info->de_name+48;
		while(1)
		{
			ofs+=64;
			blkofs=ofs>>img->ln2_blksz;
			blkrem=ofs&((1<<img->ln2_blksz)-1);
			blkptr=TKDFS_GetImageCachedInodeBlock(img, d_ino, blkofs, 1);
			deptr=blkptr+blkrem;

			memset(deptr, 0, 64);
			deptr[12]=feh;
			deptr[15]=3;
			nl1=strlen(cs);
			if(cs>60)
			{
				memcpy(deptr+ 0, cs+ 0, 12);
				memcpy(deptr+16, cs+12, 48);
				cs+=60;
			}else if(nl1>12)
			{
				memcpy(deptr+ 0, cs+ 0, 12);
				memcpy(deptr+16, cs+12, nl1-12);
				break;
			}else
			{
				memcpy(deptr+ 0, cs+ 0, nl1);
				break;
			}
		}
	}
	
	return(0);
}

int tkdfs_strcmp8(char *ds0, char *ds1)
{
	byte *cs0, *cs1;
	byte c0, c1;
	cs0=(byte *)ds0;
	cs1=(byte *)ds1;
	
	c0=*cs0++;	c1=*cs1++;
	while(c0 && (c0==c1))
		{ c0=*cs0++; c1=*cs1++; }
	return(c0-c1);
}

int TKDFS_CopyDirentInfo(TKDFS_ImageContext *img,
	TKDFS_DirentInfo *dinfo,
	TKDFS_DirentInfo *sinfo)
{
	dinfo->deb=&dinfo->t_deb;
	dinfo->deh=(TKDFS_DirEnt_Header *)(dinfo->deb);
	memcpy(dinfo->deb, sinfo->deb, sizeof(TKDFS_DirEnt_Basic));
	
	strncpy(dinfo->de_name, sinfo->de_name, 128);
	dinfo->de_ino=sinfo->de_ino;
	return(0);
}

int TKDFS_ImageAllocInodeDirentName(TKDFS_ImageContext *img,
	TKDFS_DirentInfo *info,
	int d_ino, char *d_name)
{
	TKDFS_DirentInfo t_inf0;
	TKDFS_DirentInfo t_inf1;
	TKDFS_DirentInfo *inf0;
	TKDFS_DirentInfo *inf1;
	int piv0, piv1, cmp;

	inf0=&t_inf0;
	inf1=&t_inf1;

	TKDFS_ReadImageDirent(img, inf0, d_ino, 0);
	piv0=inf0->deb->rsn;
	
	if(piv0)
	{
		TKDFS_ReadImageDirent(img, inf1, d_ino, piv0);
		piv1=inf1->deb->rsn;
		
		inf0->deb->rsn=piv1;
		
		strcpy(inf1->de_name, d_name);
		inf1->deb->rsn=0;
		inf1->deb->lsn=0;
		inf1->deb->ino=0;

		TKDFS_WriteImageDirent(img, inf0, d_ino, 0);
		TKDFS_WriteImageDirent(img, inf1, d_ino, piv0);
		TKDFS_CopyDirentInfo(img, info, inf1);
		return(piv0);
	}
	
	piv0=inf0->deh->lastent;
	if(piv0)
	{
		inf0->deh->lastent=piv0+1;

		TKDFS_ReadImageDirent(img, inf1, d_ino, piv0);

		strcpy(inf1->de_name, d_name);
		inf1->deb->rsn=0;
		inf1->deb->lsn=0;
		inf1->deb->ino=0;

		TKDFS_WriteImageDirent(img, inf0, d_ino, 0);
		TKDFS_WriteImageDirent(img, inf1, d_ino, piv0);
		TKDFS_CopyDirentInfo(img, info, inf1);
		return(piv0);
	}

	inf0->deh->lastent=16;
	
	piv1=0;
	for(piv0=1; piv0<16; piv0++)
	{
		TKDFS_ReadImageDirent(img, inf1, d_ino, piv0);
		inf0->deb->rsn=piv1;
		piv1=piv0;
		TKDFS_WriteImageDirent(img, inf1, d_ino, piv0);
	}
	TKDFS_WriteImageDirent(img, inf0, d_ino, 0);
	
	return(TKDFS_ImageAllocInodeDirentName(img, info, d_ino, d_name));
}

int TKDFS_ImageLookupInodeDirentName(TKDFS_ImageContext *img,
	TKDFS_DirentInfo *info, int d_ino, char *d_name, int dfl)
{
	TKDFS_DirentInfo t_info1;
	TKDFS_DirentInfo t_info2;
	TKDFS_DirentInfo *info1;
	TKDFS_DirentInfo *info2;
	int pivot, piv1, cmp;

	TKDFS_ReadImageDirent(img, info, d_ino, 0);
	pivot=info->deb->lsn;
	
	info1=&t_info1;
	
	while(pivot)
	{
		TKDFS_ReadImageDirent(img, info, d_ino, pivot);
		cmp=tkdfs_strcmp8(d_name, info->de_name);
		if(cmp<0)
		{
			piv1=info->deb->lsn;
			if((piv1<=0) && (dfl&1))
			{
				piv1=TKDFS_ImageAllocInodeDirentName(img, info1, d_ino, d_name);
				info->deb->lsn=piv1;
				TKDFS_WriteImageDirent(img, info1, d_ino, piv1);
				TKDFS_WriteImageDirent(img, info, d_ino, pivot);
			}

			pivot=piv1;
			continue;
		}
		if(cmp>0)
		{
			piv1=info->deb->rsn;
			if((piv1<=0) && (dfl&1))
			{
				piv1=TKDFS_ImageAllocInodeDirentName(img, info1, d_ino, d_name);
				info->deb->rsn=piv1;
				TKDFS_WriteImageDirent(img, info1, d_ino, piv1);
				TKDFS_WriteImageDirent(img, info, d_ino, pivot);
			}

			pivot=piv1;
			continue;
		}
		return(1);
	}
	/* not found */
	return(0);
}

int TKDFS_ImageLookupInodePath(TKDFS_ImageContext *img,
	TKDFS_DirentInfo *info, char *path, int dfl)
{
	char tbn[128];
	TKDFS_DirentInfo t_info;
//	TKDFS_DirentInfo *info;
	char *cs, *ct;
	int ino, rv;
	
//	info=&t_info;

	cs=path;
	while(*cs=='/')
		cs++;
	ino=1; /* root directory */
	while(*cs)
	{
		ct=tbn;
		while(*cs && (*cs!='/'))
			*ct++=*cs++;
		*ct=0;
		if(*cs=='/')
			cs++;
		rv=TKDFS_ImageLookupInodeDirentName(img, info, ino, tbn, dfl);
		if(rv<=0)
			return(0);
		ino=info->de_ino;
	}
	return(ino);
}

int TKDFS_ReadImageInodeFileData(TKDFS_ImageContext *img,
	byte *data, int d_ino, s64 fd_offs, int fd_len)
{
	TKDFS_InodeInfo *ino_inf;
	byte *blkptr, *ct;
	int blknum, blkofs, idblksh;
	s64 maxsize;
	int ofs1, len1, len2;
	
	ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 0);
//	blkofs=d_blk;

	if(ino_inf->ino_idat)
	{
		len1=ino_inf->ino_file->dsize-fd_offs;
		if(fd_len<len1)
			len1=fd_len;
		if(len1<=0)
			return(0);
		ofs1=fd_offs;
		memcpy(data, ino_inf->ino_idat+ofs1, len1);
		return(len1);
	}

	maxsize=ino_inf->ino_file->dsize-fd_offs;
	len1=fd_len;
	if(maxsize<len1)
		len1=maxsize;
	if(len1<=0)
		return(0);

	idblksh=img->ln2_blksz;
	blkofs=fd_offs&((1<<idblksh)-1);
	blknum=fd_offs>>idblksh;

	blkptr=TKDFS_GetImageCachedInodeBlock(img, d_ino, blknum, 0);
	ct=data;

	len2=(1<<idblksh)-blkofs;
	if(len1<len2)
		len2=len1;
	while(len1>0)
	{
		memcpy(ct, blkptr+blkofs, len2);
		ct+=len2;
		len1-=len2;
		if(len1<=0)
			break;
		
		blkofs=0;
		blknum++;
		len2=1<<idblksh;
		if(len1<len2)
			len2=len1;
		blkptr=TKDFS_GetImageCachedInodeBlock(img, d_ino, blknum, 0);
	}

	len1=ct-data;
	return(len1);
}
