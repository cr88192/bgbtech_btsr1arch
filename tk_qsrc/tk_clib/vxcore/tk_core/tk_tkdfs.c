u32 TKDFS_ReadImageInodeWord32(TKDFS_ImageContext *img,
	int d_ino, int d_idx);
int TKDFS_WriteImageInodeWord32(TKDFS_ImageContext *img,
	int d_ino, int d_idx, u32 d_val);

byte *TKDFS_GetImageCachedInodeBlock(TKDFS_ImageContext *img,
	int d_ino, s64 d_blk, int d_flg);
int TKDFS_InitializeImageFileInode(TKDFS_ImageContext *img,
	TKDFS_InodeInfo *info, int ino, int d_flg);
int TKDFS_InitializeImageFileInodeIdat(TKDFS_ImageContext *img,
	TKDFS_InodeInfo *info, int ino);
TKDFS_InodeInfo *TKDFS_GetImageCachedInode(TKDFS_ImageContext *img,
	int d_ino, int d_flg);

int TKDFS_ImageSetInodeInfoDeh(TKDFS_ImageContext *img,
	TKDFS_InodeInfo *info,
	char *bname, s64 dirino, int dfl);

int TKDFS_CopyName48Expand(byte *dst, byte *src);

void *tkdfs_malloc(int sz)
{
	return(malloc(sz));
}

int tkdfs_printf(char *str, ...)
{
	return(0);
}

TKDFS_ImageContext *TKDFS_AllocContext()
{
	TKDFS_ImageContext *tmp;
	
	tmp=tkdfs_malloc(sizeof(TKDFS_ImageContext));
	memset(tmp, 0, sizeof(TKDFS_ImageContext));
	return(tmp);
}


TKDFS_ImageContext *TKDFS_InitializeNewImage(
	int bdev, s64 lbastart, s64 lbasize)
{
	byte tblk[1024];
	TKDFS_BootBlock *bblk;
	TKDFS_ImageContext *img;
	TKDFS_InodeInfo *ino_mft;
	TKDFS_InodeInfo *ino_mft2;
	TKDFS_InodeInfo *ino_root;
	TKDFS_InodeInfo *ino_bbmp;
	TKDFS_InodeInfo *ino_ibmp;
	int bsz_bbmp, blk_bbmp, blk_bbmp_l1;
	int bsz_ibmp, blk_ibmp, blk_ibmp_l1;
	int bsz_mft, blk_mft1, blk_mft2, blk_mft1_l1, blk_mft2_l1;
	int bsz_rov, rsv_mft;
	int i, j, k;

	TKBDEV_ReadSectors(bdev, tblk, lbastart, 1);
	
	bblk=(TKDFS_BootBlock *)tblk;

	memset(tblk, 0, 512);
	memcpy(bblk->oem_name, "TKDFS2  ", 8);
	bblk->pad_jmp[0]=0xEB;
	bblk->pad_jmp[1]=0x5E;
	bblk->pad_jmp[2]=0x90;

	bblk->magic_55aa[0]=0x55;
	bblk->magic_55aa[1]=0xAA;

	bblk->bytes_sector[0]=0x00;
	bblk->bytes_sector[1]=0x02;

	bblk->lba_count64=lbasize;
	bblk->sectors_cluster=2;
//	bblk->ln2_sz_ino=9;
	bblk->ln2_sz_ino=8;
	bblk->ln2_sz_de=6;
	bblk->ln2_sz_cblk=14;
	bblk->ln2_ino_fixed=5;

	bblk->media_type=0xF8;
	bblk->ebp_sig=0x80;
	bblk->sectors_track=63;
	bblk->heads=255;

	rsv_mft=(lbasize<<9)>>TKDFS_AVG_LG2FILESZ;
	if(rsv_mft<64)
		rsv_mft=64;
	
	for(i=5; (1<<i)<rsv_mft; i++) { }
	bblk->ln2_ino_fixed=i;
	rsv_mft=1<<i;
	
	bsz_mft=(rsv_mft<<bblk->ln2_sz_ino)>>10;
	bsz_bbmp=(((lbasize+7)>>3)+1023)>>10;
	bsz_ibmp=(((rsv_mft+7)>>3)+1023)>>10;

	bsz_rov=4;
	
	blk_bbmp_l1=0;
	blk_ibmp_l1=0;
	blk_mft1_l1=0;
	blk_mft2_l1=0;
	
	blk_mft1=bsz_rov;
	bsz_rov+=bsz_mft;
	if(bsz_mft>=16)
	{
		blk_mft1_l1=bsz_rov;
		bsz_rov++;
	}
	
	blk_bbmp=bsz_rov;
	bsz_rov+=bsz_bbmp;
	if(bsz_bbmp>=16)
	{
		blk_bbmp_l1=bsz_rov;
		bsz_rov++;
	}

	blk_ibmp=bsz_rov;
	bsz_rov+=bsz_ibmp;
	if(bsz_ibmp>=16)
	{
		blk_ibmp_l1=bsz_rov;
		bsz_rov++;
	}

	blk_mft2=bsz_rov;
	bsz_rov+=bsz_mft;
	if(bsz_mft>=16)
	{
		blk_mft2_l1=bsz_rov;
		bsz_rov++;
	}

//	bblk->blk_inotab=4;
	bblk->blk_inotab=blk_mft1;
	bblk->blk_inotab2=blk_mft2;

	TKBDEV_WriteSectors(bdev, tblk, lbastart, 1);


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
	img->ln2_cblksz=bblk->ln2_sz_cblk;
	img->ln2_ino_fixed=bblk->ln2_ino_fixed;
	
	img->blk_count=img->lba_count>>j;
	
//	img->lba_inotab=lbastart+bblk->blk_inotab;
	img->blk_inotab=bblk->blk_inotab;
	img->blk_inotab2=bblk->blk_inotab2;

	memset(tblk, 0, 1024);

	//Inode Table (Initial Blocks)
//	for(i=0; i<32; i++)
//		TKBDEV_WriteSectors(bdev, tblk, lbastart+8+i, 1);

	for(i=0; i<bsz_mft; i++)
		TKBDEV_WriteSectors(bdev, tblk, lbastart+(blk_mft1+i)*2+0, 2);
	for(i=0; i<bsz_mft; i++)
		TKBDEV_WriteSectors(bdev, tblk, lbastart+(blk_mft2+i)*2+0, 2);

	if(blk_mft1_l1>0)
		{ TKBDEV_WriteSectors(bdev, tblk, lbastart+(blk_mft1_l1)*2+0, 2); }
	if(blk_mft2_l1>0)
		{ TKBDEV_WriteSectors(bdev, tblk, lbastart+(blk_mft2_l1)*2+0, 2); }

	//Root Directory (first block)
	TKBDEV_WriteSectors(bdev, tblk, lbastart+2, 1);
	TKBDEV_WriteSectors(bdev, tblk, lbastart+3, 1);

//	TKBDEV_WriteSectors(bdev, tblk, lbastart+5, 1);
//	TKBDEV_WriteSectors(bdev, tblk, lbastart+7, 1);

	//Block Bitmap (first block)

	memset(tblk, 0, 1024);
	for(i=0; i<bsz_bbmp; i++)
		TKBDEV_WriteSectors(bdev, tblk, lbastart+(blk_bbmp+i)*2+0, 2);

	if(blk_bbmp_l1>0)
		{ TKBDEV_WriteSectors(bdev, tblk, lbastart+(blk_bbmp_l1)*2+0, 2); }

	if(bsz_rov>=8192)
	{
		k=bsz_rov>>13;
		memset(tblk, 0xFF, 1024);

		for(i=0; i<k; i++)
			TKBDEV_WriteSectors(bdev, tblk, lbastart+(blk_bbmp+i)*2+0, 2);

		j=((bsz_rov&8191)+7)>>3;
		if(j)
		{
			memset(tblk, 0, 1024);
			for(i=0; i<j; i++)
				tblk[i]=0xFF;
			TKBDEV_WriteSectors(bdev, tblk, lbastart+(blk_bbmp+k)*2+0, 2);
		}
	}else
	{
#if 0
		tblk[0]=0xFF;
		tblk[1]=0xFF;
		tblk[2]=0xFF;
		tblk[3]=0x00;
		TKBDEV_WriteSectors(bdev, tblk, lbastart+4, 1);
	//	TKBDEV_WriteSectors(bdev, tblk, lbastart+5, 1);
#endif

		j=(bsz_rov+7)>>3;
//		memset(tblk, 0, 1024);
		for(i=0; i<j; i++)
			tblk[i]=0xFF;
		TKBDEV_WriteSectors(bdev, tblk, lbastart+(blk_bbmp+0)*2+0, 2);
	}

	//Inode Bitmap (first block)

	memset(tblk, 0, 1024);
	for(i=0; i<bsz_ibmp; i++)
		TKBDEV_WriteSectors(bdev, tblk, lbastart+(blk_ibmp+i)*2+0, 2);

	if(blk_ibmp_l1>0)
		{ TKBDEV_WriteSectors(bdev, tblk, lbastart+(blk_ibmp_l1)*2+0, 2); }

	tblk[0]=0xFF;
	tblk[1]=0x00;
	tblk[2]=0x00;
	tblk[3]=0x00;
	TKBDEV_WriteSectors(bdev, tblk, lbastart+blk_ibmp*2+0, 1);

//	TKBDEV_WriteSectors(bdev, tblk, lbastart+6, 1);
//	TKBDEV_WriteSectors(bdev, tblk, lbastart+7, 1);


	ino_mft=TKDFS_GetImageCachedInode(img, 0, 0);	
	TKDFS_InitializeImageFileInode(img, ino_mft, 0, 0);
	ino_mft=TKDFS_GetImageCachedInode(img, 0, 1);
	TKDFS_ImageSetInodeInfoDeh(img, ino_mft, "$.MFT1", 0, 1);

	ino_root=TKDFS_GetImageCachedInode(img, 1, 0);	
	TKDFS_InitializeImageFileInode(img, ino_root, 1, 0);
	ino_root=TKDFS_GetImageCachedInode(img, 1, 1);
	TKDFS_ImageSetInodeInfoDeh(img, ino_root, "$.ROOT", 0, 1);

	ino_bbmp=TKDFS_GetImageCachedInode(img, 2, 0);	
	TKDFS_InitializeImageFileInode(img, ino_bbmp, 2, 0);
	ino_bbmp=TKDFS_GetImageCachedInode(img, 2, 1);
	TKDFS_ImageSetInodeInfoDeh(img, ino_bbmp, "$.BBMP", 0, 1);

	ino_ibmp=TKDFS_GetImageCachedInode(img, 3, 0);	
	TKDFS_InitializeImageFileInode(img, ino_ibmp, 3, 0);
	ino_ibmp=TKDFS_GetImageCachedInode(img, 3, 1);
	TKDFS_ImageSetInodeInfoDeh(img, ino_ibmp, "$.IBMP", 0, 1);

	ino_mft2=TKDFS_GetImageCachedInode(img, 4, 0);
	TKDFS_InitializeImageFileInode(img, ino_mft2, 4, 0);
	ino_mft2=TKDFS_GetImageCachedInode(img, 4, 1);
	TKDFS_ImageSetInodeInfoDeh(img, ino_mft2, "$.MFT2", 0, 1);

	ino_bbmp=TKDFS_GetImageCachedInode(img, 2, 1);
	k=bsz_bbmp;
	if(k>16)k=16;
	for(i=0; i<k; i++)
		ino_bbmp->ino_idx4->block_0l[i]=blk_bbmp+i;
	ino_bbmp->ino_idx4->block_1l[0]=blk_bbmp_l1;
	ino_bbmp->ino_file->dsize=bsz_bbmp*1024;
	ino_bbmp->ino_file->csize=bsz_bbmp*1024;

	ino_ibmp=TKDFS_GetImageCachedInode(img, 3, 1);
	k=bsz_ibmp;
	if(k>16)k=16;
	for(i=0; i<k; i++)
		ino_ibmp->ino_idx4->block_0l[i]=blk_ibmp+i;
	ino_ibmp->ino_idx4->block_1l[0]=blk_ibmp_l1;
	ino_ibmp->ino_file->dsize=bsz_ibmp*1024;
	ino_ibmp->ino_file->csize=bsz_ibmp*1024;

	ino_mft=TKDFS_GetImageCachedInode(img, 0, 1);
	k=bsz_mft;
	if(k>16)k=16;
	for(i=0; i<k; i++)
		ino_mft->ino_idx4->block_0l[i]=blk_mft1+i;
	ino_mft->ino_idx4->block_1l[0]=blk_mft1_l1;
	ino_mft->ino_file->dsize=bsz_mft*1024;
	ino_mft->ino_file->csize=bsz_mft*1024;

#if 0
	ino_mft->ino_idx4->block_0l[ 0]= 4;
	ino_mft->ino_idx4->block_0l[ 1]= 5;
	ino_mft->ino_idx4->block_0l[ 2]= 6;
	ino_mft->ino_idx4->block_0l[ 3]= 7;
	ino_mft->ino_idx4->block_0l[ 4]= 8;
	ino_mft->ino_idx4->block_0l[ 5]= 9;
	ino_mft->ino_idx4->block_0l[ 6]=10;
	ino_mft->ino_idx4->block_0l[ 7]=11;
	ino_mft->ino_idx4->block_0l[ 8]=12;
	ino_mft->ino_idx4->block_0l[ 9]=13;
	ino_mft->ino_idx4->block_0l[10]=14;
	ino_mft->ino_idx4->block_0l[11]=15;
	ino_mft->ino_idx4->block_0l[12]=16;
	ino_mft->ino_idx4->block_0l[13]=17;
	ino_mft->ino_idx4->block_0l[14]=18;
	ino_mft->ino_idx4->block_0l[15]=19;
#endif


	ino_mft2=TKDFS_GetImageCachedInode(img, 4, 1);
	k=bsz_mft;
	if(k>16)k=16;
	for(i=0; i<k; i++)
		ino_mft2->ino_idx4->block_0l[i]=blk_mft2+i;
	ino_mft2->ino_idx4->block_1l[0]=blk_mft2_l1;
	ino_mft2->ino_file->dsize=bsz_mft*1024;
	ino_mft2->ino_file->csize=bsz_mft*1024;


	ino_root=TKDFS_GetImageCachedInode(img, 1, 1);
	ino_root->ino_idx4->block_0l[0]=1;
	ino_root->ino_idx4->block_0l[1]=2;
	ino_root->ino_idx4->block_0l[2]=3;
	ino_root->ino_file->dsize=3*1024;
	ino_root->ino_file->csize=3*1024;
	ino_root->ino_file->mode=0x4000;

	TKDFS_SyncImageInodes(img);

	for(i=16; i<bsz_bbmp; i++)
	{
		ino_bbmp=TKDFS_GetImageCachedInode(img, 2, 1);
		TKDFS_SetImageInodeBlockNum(img,
			ino_bbmp, i, blk_bbmp+i);
	}

	for(i=16; i<bsz_ibmp; i++)
	{
		ino_ibmp=TKDFS_GetImageCachedInode(img, 3, 1);
		TKDFS_SetImageInodeBlockNum(img,
			ino_ibmp, i, blk_ibmp+i);
	}

	for(i=16; i<bsz_mft; i++)
	{
		ino_mft=TKDFS_GetImageCachedInode(img, 0, 1);
		TKDFS_SetImageInodeBlockNum(img,
			ino_mft, i, blk_mft1+i);
	}

	for(i=16; i<bsz_mft; i++)
	{
		ino_mft2=TKDFS_GetImageCachedInode(img, 4, 1);
		TKDFS_SetImageInodeBlockNum(img,
			ino_mft2, i, blk_mft2+i);
	}

	TKDFS_SyncImageInodes(img);

	return(img);
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
	img->ln2_cblksz=bblk->ln2_sz_cblk;
	img->ln2_ino_fixed=bblk->ln2_ino_fixed;

	img->blk_count=img->lba_count>>j;

//	img->lba_inotab=lbastart+bblk->blk_inotab;
	img->blk_inotab=bblk->blk_inotab;
	img->blk_inotab2=bblk->blk_inotab2;

	return(img);
}

byte *TKDFS_GetImageCachedBlock(TKDFS_ImageContext *img, s64 d_blk, int d_flg)
{
	byte *ptr;
	s64 lba;
	int h, sh, cblkid;
	
	sh=img->ln2_blksz-9;
//	h=(((d_blk*251)>>8)&127)>>sh;
	h=((d_blk^(d_blk>>7))&127)>>sh;
	ptr=(byte *)(img->img_blkcache_data)+(h<<(9+sh));
	
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
	if((d_flg&3)==3)
	{
		/* Initializing Block, so don't bother reading. */
		memset(ptr, 0, 1<<sh);
	}else
	{
		TKBDEV_ReadSectors(img->bdev, ptr, lba, 1<<sh);
	}
	img->img_blkcache_blkid[h]=d_blk;
	img->img_blkcache_flag[h]=0;
	if(d_flg&1)
		img->img_blkcache_flag[h]|=1;
	return(ptr);
}

int TKDFS_SyncImageCachedBlocks(TKDFS_ImageContext *img)
{
	byte *ptr;
	s64 lba, cblkid;
	int h, n, sh;
	
	sh=img->ln2_blksz-9;
	n=128>>sh;

	for(h=0; h<n; h++)
	{
		ptr=(byte *)(img->img_blkcache_data)+(h<<(9+sh));
		
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

s64 TKDFS_ImageAllocateBitmap(TKDFS_ImageContext *img, int ino, s64 lim)
{
	u32 bm0;
	s64 n, n1, li;
	int h, sh;
	int i, j, k;
	
	n=lim;
	n1=n>>5;
	
	for(li=0; li<n1; li++)
	{
		bm0=TKDFS_ReadImageInodeWord32(img, ino, li);
		if(bm0==0xFFFFFFFFU)
			continue;
		for(j=0; j<32; j++)
		{
			if(((bm0>>j)&255)==255)
				{ j+=7; continue; }
			if(!(bm0&(1U<<j)))
			{
				bm0|=(1U<<j);
				TKDFS_WriteImageInodeWord32(img, ino, li, bm0);
				return((li<<5)|j);
			}
		}
	}

	/* no free space... */
	return(0);
}

int TKDFS_ImageBitmapAllocSpan(TKDFS_ImageContext *img,
	int ino, s64 lim, int cnt)
{
	u64 bm;
	u32 bm0, bm1;
	s64 n, n1;
	int h, sh;
	int i, j, k;
	
	n=lim;
	n1=n>>5;
	
	for(i=0; i<lim; i++)
	{
		j=i&31;
		bm0=TKDFS_ReadImageInodeWord32(img, ino, i>>5);
		if(bm0==0xFFFFFFFFU)
		{
			i+=31-j;
			continue;
		}
		if(((u16)(bm0>>j))==0xFFFF)
			{ i+=15; continue; }
		if(((byte)(bm0>>j))==0xFF)
			{ i+=7; continue; }
		if((bm0>>j)&1)
			continue;

		bm1=TKDFS_ReadImageInodeWord32(img, ino, (i>>5)+1);
		bm=(((u64)bm1)<<32)|bm0;
		
		if((j+cnt)<=64)
		{
			if(!((bm>>j)&((1ULL<<cnt)-1)))
			{
				bm|=((1ULL<<cnt)-1)<<j;
				
				bm0=bm;
				bm1=bm>>32;
				TKDFS_WriteImageInodeWord32(img, ino, (i>>5)+0, bm0);
				TKDFS_WriteImageInodeWord32(img, ino, (i>>5)+1, bm1);
				return(i);
			}
			continue;
		}
		
		for(k=0; k<cnt; k++)
		{
		}
//		bm0&=~(1U<<j);
//		TKDFS_WriteImageInodeWord32(img, ino, i>>5, bm0);
	}

	return(0);
}

int TKDFS_ImageBitmapFreeSpan(TKDFS_ImageContext *img,
	int ino, s64 lim, int sbase, int scnt)
{
	u32 bm0;
	int h, n, n1, sh, slim;
	int i, j, k;
	
	n=lim;
	n1=n>>5;
	
	slim=sbase+scnt;
	
	for(i=sbase; i<slim; i++)
	{
		j=i&31;
		bm0=TKDFS_ReadImageInodeWord32(img, ino, i>>5);
		bm0&=~(1U<<j);
		TKDFS_WriteImageInodeWord32(img, ino, i>>5, bm0);
	}
	return(0);
}

int TKDFS_ImageAllocateBlock(TKDFS_ImageContext *img)
{
	int sh, n;

	sh=img->ln2_blksz-9;
	n=img->lba_count>>sh;

	return(TKDFS_ImageAllocateBitmap(img, 2, n));
}

int TKDFS_ImageAllocateBlockZeroed(TKDFS_ImageContext *img)
{
	byte *buf;
	int blk;
	blk=TKDFS_ImageAllocateBlock(img);
	if(blk>0)
	{
		buf=TKDFS_GetImageCachedBlock(img, blk, 3);
//		memset(buf, 0, 1<<img->ln2_blksz);
	}
	return(blk);
}

int TKDFS_ImageAllocateBlockSpan(TKDFS_ImageContext *img, int cnt)
{
	int sh, n;

	sh=img->ln2_blksz-9;
	n=img->lba_count>>sh;

	return(TKDFS_ImageBitmapAllocSpan(img, TKDFS_FIXINO_BLKBMP, n, cnt));
}

int TKDFS_ImageFreeBlockSpan(TKDFS_ImageContext *img, u64 base, int cnt)
{
	int sh, n;

	sh=img->ln2_blksz-9;
	n=img->lba_count>>sh;

	return(TKDFS_ImageBitmapFreeSpan(img, TKDFS_FIXINO_BLKBMP, n, base, cnt));
}

int TKDFS_ImageSetInodeInfoDeh(TKDFS_ImageContext *img,
	TKDFS_InodeInfo *info,
	char *bname, s64 dirino, int dfl)
{
	int islfn;

	islfn=0;

	if(info->ino_dmhd)
	{
//		strncpy(info->ino_dmhd->refname, bname, 48);
		islfn=TKDFS_InitBaseName48(info->ino_dmhd->refname, bname);

		info->ino_dmhd->dirino_lo=dirino;
		info->ino_dmhd->dirino_hi=dirino>>32;
	}

	if(dirino>0)
	{
		if(islfn)
			info->ino_file->etype=TKDFS_ETYPE_FILE_LFN;
		else
			info->ino_file->etype=TKDFS_ETYPE_FILE_BASE;
	}else
	{
		info->ino_file->etype=TKDFS_ETYPE_INO_SYS;
	}

	return(0);
}

int TKDFS_ImageAllocateInode(TKDFS_ImageContext *img)
{
	int sh, n;

	n=1<<28;
	return(TKDFS_ImageAllocateBitmap(img, TKDFS_FIXINO_INOBMP, n));
}

int TKDFS_ImageAllocateInodeDeh(TKDFS_ImageContext *img,
	char *bname, s64 dirino, int dfl)
{
	TKDFS_InodeInfo *inf;
	int ino;
	
	ino=TKDFS_ImageAllocateInode(img);

	inf=TKDFS_GetImageCachedInode(img, ino, 3);	
	TKDFS_InitializeImageFileInode(img, inf, ino, 0);
	inf=TKDFS_GetImageCachedInode(img, ino, 1);	
	TKDFS_ImageSetInodeInfoDeh(img, inf, bname, dirino, dfl);
	return(ino);
}

int TKDFS_ImageFreeInode(TKDFS_ImageContext *img, int ino)
{
	int sh, n;

	if(ino<8)
		return(0);

	n=1<<28;
	return(TKDFS_ImageBitmapFreeSpan(img, TKDFS_FIXINO_INOBMP, n, ino, 1));
}

u32 TKDFS_ReadBlockWord32(TKDFS_ImageContext *img, s64 d_blk, int d_idx)
{
	byte *blk;
	
	blk=TKDFS_GetImageCachedBlock(img, d_blk, 0);
	return(((u32 *)blk)[d_idx]);
}

u64 TKDFS_ReadBlockWord64(TKDFS_ImageContext *img, s64 d_blk, int d_idx)
{
	byte *blk;
	
	blk=TKDFS_GetImageCachedBlock(img, d_blk, 0);
	return(((u64 *)blk)[d_idx]);
}

u32 TKDFS_WriteBlockWord32(TKDFS_ImageContext *img,
	s64 d_blk, int d_idx, u32 d_val)
{
	byte *blk;
	
	blk=TKDFS_GetImageCachedBlock(img, d_blk, 1);
	((u32 *)blk)[d_idx]=d_val;
	return(0);
}

u64 TKDFS_WriteBlockWord64(TKDFS_ImageContext *img,
	s64 d_blk, int d_idx, u64 d_val)
{
	byte *blk;
	
	blk=TKDFS_GetImageCachedBlock(img, d_blk, 1);
	((u64 *)blk)[d_idx]=d_val;
	return(0);
}


byte *TKDFS_GetImageCachedInodeBlock(
	TKDFS_ImageContext *img, int d_ino, s64 d_blk, int d_flg);

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
	info->ino_idx8=NULL;
	info->ino_idxc8=NULL;
	info->ino_idat=NULL;
	info->ino_dmhd=NULL;
	
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

		if((tg32==TKDFS_FCC_IDX8) || (tg16==TKDFS_TCC_I8))
		{
			info->ino_idx8=(TKDFS_INode_FileBlockIndex8 *)(info->t_data+tgofs);
		}

		if((tg32==TKDFS_FCC_IXC8) || (tg16==TKDFS_TCC_C8))
		{
			info->ino_idxc8=(TKDFS_INode_FileBlockIndexC8 *)
				(info->t_data+tgofs);
		}

		if((tg32==TKDFS_FCC_IDAT) || (tg16==TKDFS_TCC_ID))
		{
			info->ino_idat=info->t_data+tgofs;
			info->ino_szidat=tglen;
		}

		if((tg32==TKDFS_FCC_DHDR) || (tg16==TKDFS_TCC_DH))
		{
			info->ino_dmhd=(TKDFS_INode_DirMeta *)(info->t_data+tgofs);
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

	if(!img)
		{ TKDFS_DBGBREAK }
	if(!info)
		{ TKDFS_DBGBREAK }

	ino_offs=((s64)ino)<<img->ln2_inosz;
	ino_brem=ino_offs&((1<<img->ln2_blksz)-1);
	
//	if(ino<32)
	if(ino<(1<<(img->ln2_ino_fixed)))
	{
		/* assumed fixed inodes are directly accessible */
		inoblk=img->blk_inotab+(ino_offs>>img->ln2_blksz);
		pblk=TKDFS_GetImageCachedBlock(img, inoblk, 0);
	}else
	{
		inoblk=ino_offs>>img->ln2_blksz;
		pblk=TKDFS_GetImageCachedInodeBlock(img,
			TKDFS_FIXINO_INOTAB, inoblk, 0);
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
	ino_brem=ino_offs&((1<<img->ln2_blksz)-1);

	if(img->blk_inotab2)
	{
		/* if we have a backup inode table, update it first */

		if(ino<1<<(img->ln2_ino_fixed))
		{
			/* assumed fixed inodes are directly accessible */
			inoblk=img->blk_inotab2+(ino_offs>>img->ln2_blksz);
			pblk=TKDFS_GetImageCachedBlock(img, inoblk, 1);
		}else
		{
			inoblk=ino_offs>>img->ln2_blksz;
			pblk=TKDFS_GetImageCachedInodeBlock(img,
				TKDFS_FIXINO_INOTAB2, inoblk, 1);
		}

		memcpy(pblk+ino_brem, info->t_data, 1<<img->ln2_inosz);
	}
	
//	if(ino<32)
	if(ino<(1<<(img->ln2_ino_fixed)))
	{
		/* assumed fixed inodes are directly accessible */
		inoblk=img->blk_inotab+(ino_offs>>img->ln2_blksz);
		pblk=TKDFS_GetImageCachedBlock(img, inoblk, 1);
	}else
	{
		inoblk=ino_offs>>img->ln2_blksz;
		pblk=TKDFS_GetImageCachedInodeBlock(img,
			TKDFS_FIXINO_INOTAB, inoblk, 1);
	}

	memcpy(pblk+ino_brem, info->t_data, 1<<img->ln2_inosz);
	return(1);
}

TKDFS_InodeInfo *TKDFS_GetImageCachedInode(TKDFS_ImageContext *img,
	int d_ino, int d_flg)
{
	TKDFS_InodeInfo *ino_inf;
	int h;
	
	if(!img)
		{ TKDFS_DBGBREAK }

//	h=((d_ino*251)>>8)&63;
	h=(d_ino^(d_ino>>7))&63;
	ino_inf=img->img_inocache+h;
//	if(ino_inf->d_ino==d_ino)
	if((ino_inf->d_ino==d_ino) && (*(u64 *)(ino_inf->t_data)!=0) &&
		!(ino_inf->d_flag&2))
	{
		return(ino_inf);
	}
	
	if(ino_inf->d_flag&1)
	{
		TKDFS_WriteImageInode(img, ino_inf, ino_inf->d_ino);
		ino_inf->d_flag&=~3;
	}
	
	TKDFS_ReadImageInode(img, ino_inf, d_ino);
	ino_inf->d_ino=d_ino;
	if(d_flg&1)
		ino_inf->d_flag|=1;
	return(ino_inf);
}

int TKDFS_SyncImageInodes(TKDFS_ImageContext *img)
{
	TKDFS_InodeInfo *ino_inf;
	int i;
	
	TKDFS_SyncCompressedBlocks(img);
	
	for(i=0; i<64; i++)
	{
		ino_inf=img->img_inocache+i;		
		if(ino_inf->d_flag&1)
		{
			TKDFS_WriteImageInode(img, ino_inf, ino_inf->d_ino);
			ino_inf->d_flag&=~3;
		}
	}
	TKDFS_SyncImageCachedBlocks(img);
	return(0);
}

int TKDFS_InitializeImageFileInode(TKDFS_ImageContext *img,
	TKDFS_InodeInfo *info, int ino, int d_flg)
{
	u32 *taga;
	byte *ct;
	int l0, o0, tl1, inosz;
	
	inosz=1<<img->ln2_inosz;
	
	ct=info->t_data;
	memset(ct, 0, 512);

	info->ino_idx4=NULL;
	info->ino_idx8=NULL;
	info->ino_idxc8=NULL;
	info->ino_idat=NULL;
	
	taga=(u32 *)ct;
	ct+=32;
	
	taga[0]=TKDFS_FCC_FHDR;
	taga[1]=0;
	taga[2]=TKDFS_FCC_IDX4;
	taga[3]=0;
	taga[4]=TKDFS_FCC_DHDR;
	taga[5]=0;
	taga[6]=0;
	taga[7]=0;

	if((d_flg&0x20) || (img->blk_count>1LL<<30))
	{
		taga[2]=TKDFS_FCC_IXC8;
		if(img->ln2_inosz>=9)
			taga[2]=TKDFS_FCC_IDX8;
	}

	l0=sizeof(TKDFS_INode_File);
	l0=(l0+7)&(~7);
	o0=ct-info->t_data;
	
	tl1=((~l0)&0xFFFF)|(o0<<16);
	taga[1]=tl1;
	
	info->ino_file=(TKDFS_INode_File *)ct;
	ct+=l0;


	l0=sizeof(TKDFS_INode_DirMeta);
	l0=(l0+7)&(~7);
	o0=ct-info->t_data;
	
	tl1=((~l0)&0xFFFF)|(o0<<16);
	taga[5]=tl1;
	
	info->ino_dmhd=(TKDFS_INode_DirMeta *)ct;
	ct+=l0;



	if(taga[2]==TKDFS_FCC_IDX8)
		l0=sizeof(TKDFS_INode_FileBlockIndex8);
	else if(taga[2]==TKDFS_FCC_IXC8)
		l0=sizeof(TKDFS_INode_FileBlockIndexC8);
	else
		l0=sizeof(TKDFS_INode_FileBlockIndex4);
	l0=(l0+7)&(~7);
	o0=ct-info->t_data;
	
	tl1=((~l0)&0xFFFF)|(o0<<16);
	taga[3]=tl1;
	
	if(taga[2]==TKDFS_FCC_IDX8)
		info->ino_idx8=(TKDFS_INode_FileBlockIndex8 *)ct;
	else if(taga[2]==TKDFS_FCC_IXC8)
		info->ino_idxc8=(TKDFS_INode_FileBlockIndexC8 *)ct;
	else
		info->ino_idx4=(TKDFS_INode_FileBlockIndex4 *)ct;

	if(d_flg&TKDFS_DFLAG_INLINE)
	{
		taga[2]=TKDFS_FCC_IDAT;
		info->ino_idat=ct;
		info->ino_szidat=l0;

		info->ino_idx4=NULL;
		info->ino_idx8=NULL;
		info->ino_idxc8=NULL;
	}

	ct+=l0;

	o0=ct-info->t_data;
	if(o0>inosz)
		{ TKDFS_DBGBREAK }

	info->d_flag|=3;
	return(0);
}

int TKDFS_InitializeImageFileInodeIdat(TKDFS_ImageContext *img,
	TKDFS_InodeInfo *info, int ino)
{
	u32 *taga;
	byte *ct;
	int l0, o0, tl1;
	
	TKDFS_InitializeImageFileInode(img, info, ino,
		TKDFS_DFLAG_INLINE);

#if 0
	ct=info->t_data;
	taga=(u32 *)ct;
	taga[2]=TKDFS_FCC_IDAT;

	info->ino_idat=(byte *)info->ino_idx4;
	info->ino_idx4=NULL;
#endif

	return(0);
}

u64 TKDFS_GetImageIndirect32BlockNum(TKDFS_ImageContext *img,
	u64 d_blk, u64 blkofs, int lvl)
{
	u64 blknum, idx;
	int idblksh, idxr;

	if(!d_blk)
		return(0);

	idblksh=img->ln2_blksz-2;
	idx=blkofs>>(idblksh*lvl);
	idxr=idx&((1<<idblksh)-1);

	blknum=TKDFS_ReadBlockWord32(img, d_blk, idxr);
	if(lvl>0)
		{ blknum=TKDFS_GetImageIndirect32BlockNum(img, blknum, blkofs, lvl-1); }
	return(blknum);
}

u64 TKDFS_GetImageIndirect64BlockNum(TKDFS_ImageContext *img,
	u64 d_blk, u64 blkofs, int lvl)
{
	u64 blknum, idx;
	int idblksh, idxr;

	if(!d_blk)
		return(0);

	idblksh=img->ln2_blksz-3;
	idx=blkofs>>(idblksh*lvl);
	idxr=idx&((1<<idblksh)-1);

	blknum=TKDFS_ReadBlockWord64(img, d_blk, idxr);
	if(lvl>0)
		{ blknum=TKDFS_GetImageIndirect64BlockNum(img, blknum, blkofs, lvl-1); }
	return(blknum);
}

u64 TKDFS_SetImageIndirect32BlockNum(TKDFS_ImageContext *img,
	u64 d_blk, u64 blkofs, int lvl, u64 blkval)
{
	u64 blknum, idx;
	int idblksh, idxr;

	if(!d_blk)
	{
		d_blk=TKDFS_ImageAllocateBlockZeroed(img);
	}

	idblksh=img->ln2_blksz-2;
	idx=blkofs>>(idblksh*lvl);
	idxr=idx&((1<<idblksh)-1);

	if(lvl>0)
	{
		blknum=TKDFS_ReadBlockWord32(img, d_blk, idxr);
		if(!blknum)
		{
			blknum=TKDFS_ImageAllocateBlockZeroed(img);
			TKDFS_WriteBlockWord32(img, d_blk, idxr, blknum);
		}
		blknum=TKDFS_SetImageIndirect32BlockNum(img,
			blknum, blkofs, lvl-1, blkval);
	}else
	{
		TKDFS_WriteBlockWord32(img, d_blk, idxr, blkval);
	}
	return(d_blk);
}

u64 TKDFS_SetImageIndirect64BlockNum(TKDFS_ImageContext *img,
	u64 d_blk, u64 blkofs, int lvl, u64 blkval)
{
	u64 blknum, idx;
	int idblksh, idxr;

	if(!d_blk)
	{
		d_blk=TKDFS_ImageAllocateBlockZeroed(img);
	}

	idblksh=img->ln2_blksz-3;
	idx=blkofs>>(idblksh*lvl);
	idxr=idx&((1<<idblksh)-1);

	if(lvl>0)
	{
		blknum=TKDFS_ReadBlockWord64(img, d_blk, idxr);
		if(!blknum)
		{
			blknum=TKDFS_ImageAllocateBlockZeroed(img);
			TKDFS_WriteBlockWord64(img, d_blk, idxr, blknum);
		}
		blknum=TKDFS_SetImageIndirect64BlockNum(img,
			blknum, blkofs, lvl-1, blkval);
	}else
	{
		TKDFS_WriteBlockWord64(img, d_blk, idxr, blkval);
	}
	return(d_blk);
}

u64 TKDFS_GetImageInodeBlockNum(TKDFS_ImageContext *img,
	TKDFS_InodeInfo *ino_inf, s64 d_blk)
{
	u64 blknum, blk0, blk1, blk2, blk3, blk4;
	int blkofs, idblksh, d_ino;
	s64 idx0, idx1, idx2, idx3, idx4;
	int idx0r, idx1r, idx2r, idx3r, idx4r;

	d_ino=ino_inf->d_ino;
	blkofs=d_blk;

	if(ino_inf->ino_idx4)
	{
		idblksh=img->ln2_blksz-2;
		idx0=blkofs>>idblksh;
		idx0r=blkofs&((1<<idblksh)-1);
		idx1=idx0>>idblksh;
		idx1r=idx0&((1<<idblksh)-1);
		idx2=idx1>>idblksh;
		idx2r=idx1&((1<<idblksh)-1);
		idx3=idx2>>idblksh;
		idx3r=idx2&((1<<idblksh)-1);
		idx4=idx3>>idblksh;
		idx4r=idx3&((1<<idblksh)-1);
		if(blkofs<16)
		{
			blknum=ino_inf->ino_idx4->block_0l[blkofs];
		}else if(idx0<8)
		{
			blk0=ino_inf->ino_idx4->block_1l[idx0];
			if(!blk0)
				return(0);
			blknum=TKDFS_ReadBlockWord32(img, blk0, idx0r);
		}else if(idx1<4)
		{
			blk1=ino_inf->ino_idx4->block_2l[idx1];
			if(!blk1)
				return(0);
			blknum=TKDFS_GetImageIndirect32BlockNum(img, blk1, blkofs, 1);
		}else if(idx2<2)
		{
			blk2=ino_inf->ino_idx4->block_3l[idx2];
			blknum=TKDFS_GetImageIndirect32BlockNum(img, blk2, blkofs, 2);
		}else if(!idx3)
		{
			blk3=ino_inf->ino_idx4->block_4l;
			blknum=TKDFS_GetImageIndirect32BlockNum(img, blk3, blkofs, 3);
		}else if(!idx4)
		{
			blk4=ino_inf->ino_idx4->block_5l;
			blknum=TKDFS_GetImageIndirect32BlockNum(img, blk4, blkofs, 4);
		}
	}

	if(ino_inf->ino_idxc8)
	{
		idblksh=img->ln2_blksz-3;
		idx0=blkofs>>idblksh;
		idx0r=blkofs&((1<<idblksh)-1);
		idx1=idx0>>idblksh;
		idx1r=idx0&((1<<idblksh)-1);
		idx2=idx1>>idblksh;
		idx2r=idx1&((1<<idblksh)-1);
		idx3=idx2>>idblksh;
		idx3r=idx2&((1<<idblksh)-1);
		idx4=idx3>>idblksh;
		idx4r=idx3&((1<<idblksh)-1);
		if(blkofs<8)
		{
			blknum=ino_inf->ino_idxc8->block_0l[blkofs];
		}else if(idx0<4)
		{
			blk0=ino_inf->ino_idxc8->block_1l[idx0];
			if(!blk0)
				return(0);
			blknum=TKDFS_ReadBlockWord64(img, blk0, idx0r);
		}else if(!idx1)
		{
			blk1=ino_inf->ino_idxc8->block_2l;
			blknum=TKDFS_GetImageIndirect64BlockNum(img, blk1, blkofs, 1);
		}else if(!idx2)
		{
			blk1=ino_inf->ino_idxc8->block_3l;
			blknum=TKDFS_GetImageIndirect64BlockNum(img, blk1, blkofs, 2);
		}else if(!idx3)
		{
			blk1=ino_inf->ino_idxc8->block_4l;
			blknum=TKDFS_GetImageIndirect64BlockNum(img, blk1, blkofs, 3);
		}else if(!idx4)
		{
			blk1=ino_inf->ino_idxc8->block_5l;
			blknum=TKDFS_GetImageIndirect64BlockNum(img, blk1, blkofs, 4);
		}
	}

	if(ino_inf->ino_idx8)
	{
		idblksh=img->ln2_blksz-3;
		idx0=blkofs>>idblksh;
		idx0r=blkofs&((1<<idblksh)-1);
		idx1=idx0>>idblksh;
		idx1r=idx0&((1<<idblksh)-1);
		idx2=idx1>>idblksh;
		idx2r=idx1&((1<<idblksh)-1);
		idx3=idx2>>idblksh;
		idx3r=idx2&((1<<idblksh)-1);
		idx4=idx3>>idblksh;
		idx4r=idx3&((1<<idblksh)-1);
		if(blkofs<16)
		{
			blknum=ino_inf->ino_idx8->block_0l[blkofs];
		}else if(idx0<8)
		{
			blk0=ino_inf->ino_idx8->block_1l[idx0];
			if(!blk0)
				return(0);
			blknum=TKDFS_ReadBlockWord64(img, blk0, idx0r);
		}else if(idx1<4)
		{
			blk1=ino_inf->ino_idx8->block_2l[idx1];
			blknum=TKDFS_GetImageIndirect64BlockNum(img, blk1, blkofs, 1);
		}else if(!idx2)
		{
			blk1=ino_inf->ino_idx8->block_3l;
			blknum=TKDFS_GetImageIndirect64BlockNum(img, blk1, blkofs, 2);
		}else if(!idx3)
		{
			blk1=ino_inf->ino_idx8->block_4l;
			blknum=TKDFS_GetImageIndirect64BlockNum(img, blk1, blkofs, 3);
		}else if(!idx4)
		{
			blk1=ino_inf->ino_idx8->block_5l;
			blknum=TKDFS_GetImageIndirect64BlockNum(img, blk1, blkofs, 4);
		}
	}

	return(blknum);
}

int TKDFS_SetImageInodeBlockNum(TKDFS_ImageContext *img,
	TKDFS_InodeInfo *ino_inf, int d_blk, u64 newblk)
{
	u64 blk0, blk1, blk2, blk3, blk4;
	int blkofs, idblksh, d_ino;
	s64 idx0, idx1, idx2, idx3, idx4, idx5;
	int idx0r, idx1r, idx2r, idx3r, idx4r, idx5r;

	d_ino=ino_inf->d_ino;
	blkofs=d_blk;

	if(ino_inf->ino_idx4)
	{
		idblksh=img->ln2_blksz-2;
		idx0=blkofs>>idblksh;
		idx0r=blkofs&((1<<idblksh)-1);
		idx1=idx0>>idblksh;
		idx1r=idx0&((1<<idblksh)-1);
		idx2=idx1>>idblksh;
		idx2r=idx1&((1<<idblksh)-1);
		idx3=idx2>>idblksh;
		idx3r=idx2&((1<<idblksh)-1);
		idx4=idx3>>idblksh;
		idx4r=idx3&((1<<idblksh)-1);
		if(blkofs<16)
		{
			ino_inf->ino_idx4->block_0l[blkofs]=newblk;
			return(1);
		}else if(idx0<8)
		{
			blk0=ino_inf->ino_idx4->block_1l[idx0];
			if(!blk0)
			{
				blk0=TKDFS_ImageAllocateBlockZeroed(img);
				ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
				ino_inf->ino_idx4->block_1l[idx0]=blk0;
			}
			if(!blk0)
				return(0);
			TKDFS_WriteBlockWord32(img, blk0, idx0r, newblk);
			return(1);
		}else if(idx1<4)
		{
			blk1=ino_inf->ino_idx4->block_2l[idx1];
			if(!blk1)
			{
				blk1=TKDFS_ImageAllocateBlockZeroed(img);
				ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
				ino_inf->ino_idx4->block_2l[idx1]=blk1;
			}
			TKDFS_SetImageIndirect32BlockNum(img, blk1, blkofs, 1, newblk);
			return(1);
		}else if(idx2<2)
		{
			blk2=ino_inf->ino_idx4->block_3l[idx2];
			if(!blk2)
			{
				blk2=TKDFS_ImageAllocateBlockZeroed(img);
				ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
				ino_inf->ino_idx4->block_3l[idx2]=blk2;
			}
			TKDFS_SetImageIndirect32BlockNum(img, blk2, blkofs, 2, newblk);
			return(1);
		}else if(!idx3)
		{
			blk2=ino_inf->ino_idx4->block_4l;
			if(!blk2)
			{
				blk2=TKDFS_ImageAllocateBlockZeroed(img);
				ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
				ino_inf->ino_idx4->block_4l=blk2;
			}
			TKDFS_SetImageIndirect32BlockNum(img, blk2, blkofs, 3, newblk);
			return(1);
		}else if(!idx4)
		{
			blk2=ino_inf->ino_idx4->block_5l;
			if(!blk2)
			{
				blk2=TKDFS_ImageAllocateBlockZeroed(img);
				ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
				ino_inf->ino_idx4->block_5l=blk2;
			}
			TKDFS_SetImageIndirect32BlockNum(img, blk2, blkofs, 4, newblk);
			return(1);
		}
	}

	if(ino_inf->ino_idxc8)
	{
		idblksh=img->ln2_blksz-3;
		idx0=blkofs>>idblksh;
		idx0r=blkofs&((1<<idblksh)-1);
		idx1=idx0>>idblksh;
		idx1r=idx0&((1<<idblksh)-1);
		idx2=idx1>>idblksh;
		idx2r=idx1&((1<<idblksh)-1);
		idx3=idx2>>idblksh;
		idx3r=idx2&((1<<idblksh)-1);
		idx4=idx3>>idblksh;
		idx4r=idx3&((1<<idblksh)-1);
		if(blkofs<8)
		{
			ino_inf->ino_idxc8->block_0l[blkofs]=newblk;
			return(1);
		}else if(idx0<4)
		{
			blk0=ino_inf->ino_idxc8->block_1l[idx0];
			if(!blk0)
			{
				blk0=TKDFS_ImageAllocateBlockZeroed(img);
				ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
				ino_inf->ino_idxc8->block_1l[idx0]=blk0;
			}
			if(!blk0)
				return(0);
			TKDFS_WriteBlockWord64(img, blk0, idx0r, newblk);
			return(1);
		}else if(!idx1)
		{
			blk1=ino_inf->ino_idxc8->block_2l;
			if(!blk1)
			{
				blk1=TKDFS_ImageAllocateBlockZeroed(img);
				ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
				ino_inf->ino_idxc8->block_2l=blk1;
			}
			TKDFS_SetImageIndirect64BlockNum(img, blk1, blkofs, 1, newblk);
			return(1);
		}else if(!idx2)
		{
			blk2=ino_inf->ino_idxc8->block_3l;
			if(!blk2)
			{
				blk2=TKDFS_ImageAllocateBlockZeroed(img);
				ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
				ino_inf->ino_idxc8->block_3l=blk2;
			}
			if(!blk2)
				return(0);
			TKDFS_SetImageIndirect64BlockNum(img, blk2, blkofs, 2, newblk);
			return(1);
		}else if(!idx3)
		{
			blk2=ino_inf->ino_idxc8->block_4l;
			if(!blk2)
			{
				blk2=TKDFS_ImageAllocateBlockZeroed(img);
				ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
				ino_inf->ino_idxc8->block_4l=blk2;
			}
			if(!blk2)
				return(0);
			TKDFS_SetImageIndirect64BlockNum(img, blk2, blkofs, 3, newblk);
			return(1);
		}else if(!idx4)
		{
			blk2=ino_inf->ino_idxc8->block_5l;
			if(!blk2)
			{
				blk2=TKDFS_ImageAllocateBlockZeroed(img);
				ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
				ino_inf->ino_idxc8->block_5l=blk2;
			}
			if(!blk2)
				return(0);
			TKDFS_SetImageIndirect64BlockNum(img, blk2, blkofs, 4, newblk);
			return(1);
		}
	}


	if(ino_inf->ino_idx8)
	{
		idblksh=img->ln2_blksz-3;
		idx0=blkofs>>idblksh;
		idx0r=blkofs&((1<<idblksh)-1);
		idx1=idx0>>idblksh;
		idx1r=idx0&((1<<idblksh)-1);
		idx2=idx1>>idblksh;
		idx2r=idx1&((1<<idblksh)-1);
		idx3=idx2>>idblksh;
		idx3r=idx2&((1<<idblksh)-1);
		idx4=idx3>>idblksh;
		idx4r=idx3&((1<<idblksh)-1);
		idx5=idx4>>idblksh;
		idx5r=idx4&((1<<idblksh)-1);
		if(blkofs<16)
		{
			ino_inf->ino_idx8->block_0l[blkofs]=newblk;
			return(1);
		}else if(idx0<8)
		{
			blk0=ino_inf->ino_idx8->block_1l[idx0];
			if(!blk0)
			{
				blk0=TKDFS_ImageAllocateBlockZeroed(img);
				ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
				ino_inf->ino_idx8->block_1l[idx0]=blk0;
			}
			if(!blk0)
				return(0);
			TKDFS_WriteBlockWord64(img, blk0, idx0r, newblk);
			return(1);
		}else if(idx1<4)
		{
			blk1=ino_inf->ino_idx8->block_2l[idx1];
			if(!blk1)
			{
				blk1=TKDFS_ImageAllocateBlockZeroed(img);
				ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
				ino_inf->ino_idx8->block_2l[idx1]=blk1;
			}
			TKDFS_SetImageIndirect64BlockNum(img, blk1, blkofs, 1, newblk);
			return(1);
		}else if(!idx2)
		{
			blk2=ino_inf->ino_idx8->block_3l;
			if(!blk2)
			{
				blk2=TKDFS_ImageAllocateBlockZeroed(img);
				ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
				ino_inf->ino_idx8->block_3l=blk2;
			}
			if(!blk2)
				return(0);
			TKDFS_SetImageIndirect64BlockNum(img, blk2, blkofs, 2, newblk);
			return(1);
		}else if(!idx3)
		{
			blk2=ino_inf->ino_idx8->block_4l;
			if(!blk2)
			{
				blk2=TKDFS_ImageAllocateBlockZeroed(img);
				ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
				ino_inf->ino_idx8->block_4l=blk2;
			}
			if(!blk2)
				return(0);
			TKDFS_SetImageIndirect64BlockNum(img, blk2, blkofs, 3, newblk);
			return(1);
		}else if(!idx4)
		{
			blk2=ino_inf->ino_idx8->block_5l;
			if(!blk2)
			{
				blk2=TKDFS_ImageAllocateBlockZeroed(img);
				ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
				ino_inf->ino_idx8->block_5l=blk2;
			}
			if(!blk2)
				return(0);
			TKDFS_SetImageIndirect64BlockNum(img, blk2, blkofs, 4, newblk);
			return(1);
		}else if(!idx5)
		{
			blk2=ino_inf->ino_idx8->block_6l;
			if(!blk2)
			{
				blk2=TKDFS_ImageAllocateBlockZeroed(img);
				ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
				ino_inf->ino_idx8->block_6l=blk2;
			}
			if(!blk2)
				return(0);
			TKDFS_SetImageIndirect64BlockNum(img, blk2, blkofs, 5, newblk);
			return(1);
		}
	}

	return(0);
}

u64 TKDFS_GetImageInodeBlockNum2(TKDFS_ImageContext *img,
	TKDFS_InodeInfo *ino_inf,
	int d_blk, int d_flg)
{
	u64 blknum;
	int d_ino;
	
	d_ino=ino_inf->d_ino;
	
	blknum=TKDFS_GetImageInodeBlockNum(img, ino_inf, d_blk);
	if(!blknum && ((d_flg&1) || (d_flg&0x80))  && !(d_flg&0x10))
	{
		blknum=TKDFS_ImageAllocateBlockZeroed(img);
		ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
		TKDFS_SetImageInodeBlockNum(img, ino_inf, d_blk, blknum);
	}
	return(blknum);
}

int TKDFS_ImageIndirect32FreeAllBlocks(TKDFS_ImageContext *img,
	int d_blk, int lvl, int d_flg)
{
	byte *blk;
	u64 blknum;
	int i, j, k, sh, n;
	
	sh=img->ln2_blksz-2;
	n=1<<sh;
	
	blk=TKDFS_GetImageCachedBlock(img, d_blk, 1);
	for(i=0; i<n; i++)
	{
		blknum=((u32 *)blk)[i];
		if(blknum)
		{
			if(lvl>0)
			{
				TKDFS_ImageIndirect32FreeAllBlocks(
					img, blknum, lvl-1, d_flg);
			}
			TKDFS_ImageFreeBlockSpan(img, blknum, 1);
			((u32 *)blk)[i]=0;
		}
	}
	return(0);
}

int TKDFS_ImageIndirect64FreeBlock(TKDFS_ImageContext *img,
	u64 blknum, int d_flg)
{
	int i, j, k, sh, shcb, n, ncb;
	
	sh=img->ln2_blksz-3;
	shcb=img->ln2_cblksz-img->ln2_blksz;
	n=1<<sh;
	ncb=1<<shcb;
	
	if(d_flg&0x20)
	{
		if((blknum>>62)==0)
		{
			TKDFS_ImageFreeBlockSpan(img,
				blknum&0x0000FFFFFFFFFFFFULL, ncb);
		}
		else if((blknum>>62)==1)
		{
			j=(blknum>>48)&1023;
			TKDFS_ImageFreeBlockSpan(img,
				blknum&0x0000FFFFFFFFFFFFULL, j);
		}
	}else
	{
		TKDFS_ImageFreeBlockSpan(img, blknum, 1);
	}
	return(0);
}

int TKDFS_ImageIndirect64FreeAllBlocks(TKDFS_ImageContext *img,
	int d_blk, int lvl, int d_flg)
{
	byte *blk;
	u64 blknum;
	int i, j, k, sh, shcb, n, ncb;
	
	sh=img->ln2_blksz-3;
	shcb=img->ln2_cblksz-img->ln2_blksz;
	n=1<<sh;
	ncb=1<<shcb;
	
	blk=TKDFS_GetImageCachedBlock(img, d_blk, 1);
	for(i=0; i<n; i++)
	{
		blknum=((u64 *)blk)[i];
		if(blknum)
		{
			if(lvl>0)
			{
				TKDFS_ImageIndirect64FreeAllBlocks(
					img, blknum, lvl-1, d_flg);
				TKDFS_ImageFreeBlockSpan(img, blknum, 1);
				((u64 *)blk)[i]=0;
			}else
			{
				TKDFS_ImageIndirect64FreeBlock(img, blknum, d_flg);
				((u64 *)blk)[i]=0;
			}
		}
	}
	return(0);
}

int TKDFS_ImageInodeFreeAllBlocks(TKDFS_ImageContext *img,
	int d_ino, int d_flg)
{
	TKDFS_InodeInfo *ino_inf;
	u64 blknum;
	int i, j, k;

//	if(d_ino<8)
//		return(0);

	ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);

	if(ino_inf->ino_idx4)
	{
		for(i=0; i<16; i++)
		{
			blknum=ino_inf->ino_idx4->block_0l[i];
			if(blknum)
			{
				TKDFS_ImageFreeBlockSpan(img, blknum, 1);
				ino_inf->ino_idx4->block_0l[i]=0;
			}
		}

		for(i=0; i<8; i++)
		{
			blknum=ino_inf->ino_idx4->block_1l[i];
			if(blknum)
			{
				TKDFS_ImageIndirect32FreeAllBlocks(img, blknum, 0, 1);
				TKDFS_ImageFreeBlockSpan(img, blknum, 1);
				ino_inf->ino_idx4->block_1l[i]=0;
			}
		}

		for(i=0; i<4; i++)
		{
			blknum=ino_inf->ino_idx4->block_2l[i];
			if(blknum)
			{
				TKDFS_ImageIndirect32FreeAllBlocks(img, blknum, 1, 1);
				TKDFS_ImageFreeBlockSpan(img, blknum, 1);
				ino_inf->ino_idx4->block_2l[i]=0;
			}
		}

		for(i=0; i<2; i++)
		{
			blknum=ino_inf->ino_idx4->block_3l[i];
			if(blknum)
			{
				TKDFS_ImageIndirect32FreeAllBlocks(img, blknum, 2, 1);
				TKDFS_ImageFreeBlockSpan(img, blknum, 1);
				ino_inf->ino_idx4->block_3l[i]=0;
			}
		}

		blknum=ino_inf->ino_idx4->block_4l;
		if(blknum)
		{
			TKDFS_ImageIndirect32FreeAllBlocks(img, blknum, 3, 1);
			TKDFS_ImageFreeBlockSpan(img, blknum, 1);
			ino_inf->ino_idx4->block_4l=0;
		}

		blknum=ino_inf->ino_idx4->block_5l;
		if(blknum)
		{
			TKDFS_ImageIndirect32FreeAllBlocks(img, blknum, 4, 1);
			TKDFS_ImageFreeBlockSpan(img, blknum, 1);
			ino_inf->ino_idx4->block_5l=0;
		}
	}

	if(ino_inf->ino_idxc8)
	{
		for(i=0; i<8; i++)
		{
			blknum=ino_inf->ino_idxc8->block_0l[i];
			if(blknum)
			{
				TKDFS_ImageIndirect64FreeBlock(img, blknum, d_flg);
				ino_inf->ino_idxc8->block_0l[i]=0;
			}
		}

		for(i=0; i<4; i++)
		{
			blknum=ino_inf->ino_idxc8->block_1l[i];
			if(blknum)
			{
				TKDFS_ImageIndirect64FreeAllBlocks(img, blknum, 0, d_flg);
				TKDFS_ImageFreeBlockSpan(img, blknum, 1);
				ino_inf->ino_idxc8->block_1l[i]=0;
			}
		}


		blknum=ino_inf->ino_idxc8->block_2l;
		if(blknum)
		{
			TKDFS_ImageIndirect32FreeAllBlocks(img, blknum, 2, 1);
			TKDFS_ImageFreeBlockSpan(img, blknum, 1);
			ino_inf->ino_idxc8->block_2l=0;
		}

		blknum=ino_inf->ino_idxc8->block_3l;
		if(blknum)
		{
			TKDFS_ImageIndirect32FreeAllBlocks(img, blknum, 3, 1);
			TKDFS_ImageFreeBlockSpan(img, blknum, 1);
			ino_inf->ino_idxc8->block_3l=0;
		}

		blknum=ino_inf->ino_idxc8->block_4l;
		if(blknum)
		{
			TKDFS_ImageIndirect32FreeAllBlocks(img, blknum, 4, 1);
			TKDFS_ImageFreeBlockSpan(img, blknum, 1);
			ino_inf->ino_idxc8->block_4l=0;
		}

		blknum=ino_inf->ino_idxc8->block_5l;
		if(blknum)
		{
			TKDFS_ImageIndirect32FreeAllBlocks(img, blknum, 5, 1);
			TKDFS_ImageFreeBlockSpan(img, blknum, 1);
			ino_inf->ino_idxc8->block_5l=0;
		}
	}
	
	if(d_flg&TKDFS_DFLAG_DELETE)
	{
		if(ino_inf->ino_file)
		{
			ino_inf->ino_file->dsize=0;
			ino_inf->ino_file->csize=0;
			ino_inf->ino_file->ctime=0;
			ino_inf->ino_file->mtime=0;
			ino_inf->ino_file->uid=0;
			ino_inf->ino_file->gid=0;
			ino_inf->ino_file->cmpmode=0;
			ino_inf->ino_file->etype=TKDFS_ETYPE_DEL_BASE;
		}
	}

	return(0);
}

int TKDFS_ImageDestroyInode(TKDFS_ImageContext *img,
	int d_ino, int d_flg)
{
	TKDFS_InodeInfo *ino_inf;
	int dfl;

	if(d_ino<8)
		return(0);

	ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);

	dfl=d_flg;
	if(ino_inf->ino_file && ino_inf->ino_file->cmpmode)
		dfl|=TKDFS_DFLAG_COMPRESS;

	TKDFS_ImageInodeFreeAllBlocks(img, d_ino, dfl|TKDFS_DFLAG_DELETE);
	TKDFS_ImageFreeInode(img, d_ino);
	return(0);
}

int TKDFS_CompressBlockBuffer(byte *dst, byte *src, int dsz, int ssz, int cmp)
{
	u64 *csq, *csqe;
	u32 csum;
	int sz;

	if(cmp==0)
	{
		memcpy(dst, src, ssz);
		return(ssz);
	}
	
#if 1
	/* Check if block is entirely zeroes. */
	csq=(u64 *)src;
	csqe=(u64 *)(src+ssz);
	while(csq<csqe)
	{
		if(*csq)
			break;
		csq++;
	}
	if(csq>=csqe)
		return(0);
#endif

	if(cmp==3)
	{
		sz=TKPE_MiniPackBlockRP2(src, dst+12, ssz, dsz);
		csum=TKPE_CalculateImagePel4BChecksum(src, ssz);
		if(sz<=0)
			return(-1);
		
		sz+=12;
		((u32 *)dst)[0]=~sz;
		((u32 *)dst)[1]=csum;
		dst[ 8]=12;		dst[ 9]=cmp;
		dst[10]=0;		dst[11]=0;
		return(sz);
	}
	
	return(-1);
}

int TKDFS_DecompressBlockBuffer(
	byte *dst, byte *src, int dsz, int ssz, int cmp)
{
	byte *ct;
	int csz, hsz, cm1;

	if(cmp==0)
	{
		memcpy(dst, src, ssz);
		return(ssz);
	}
	
	csz=~(*(u32 *)src);
	hsz=src[8];
	cm1=src[9];
	if((csz<=12) || (csz>ssz))
	{
		return(-1);
	}

	ct=TKPE_UnpackBuffer(dst, src+hsz, csz-hsz, cm1);
	if(!ct)
		return(-1);
	return(ct-dst);
//	return(-1);
}

int TKDFS_SyncCompressedBlockIdx(TKDFS_ImageContext *img, int idx)
{
	TKDFS_InodeInfo *ino_inf;
	byte *ptr;
	u64 blknum, cblkid;
	s64 lba;
	int h, sh, shb, sz, szb, cblkszb, cblkino, cblkidx, docmp;
	int i, j, k;
	
	if(!img->img_cblkcache_data)
		return(0);
	
	cblkszb=1<<(img->ln2_cblksz-img->ln2_blksz);
	
	sh=img->ln2_cblksz-14;
	h=idx;
	ptr=(byte *)(img->img_cblkcache_data)+(h<<(14+sh));

	shb=img->ln2_blksz-9;
	
	cblkino=img->img_cblkcache_ino[h];
	cblkidx=img->img_cblkcache_idx[h];

	if(img->img_cblkcache_flag[h]&1)
	{
		ino_inf=TKDFS_GetImageCachedInode(img, cblkino, 0);
		blknum=TKDFS_GetImageInodeBlockNum(img, ino_inf, cblkidx);

		sz=TKDFS_CompressBlockBuffer(
			img->img_cblkcache_tbuf, ptr,
			1<<(img->ln2_cblksz+1),
			1<<(img->ln2_cblksz+0),
			ino_inf->ino_file->cmpmode);
		szb=(sz+((1<<img->ln2_blksz)-1))>>(img->ln2_blksz);
		docmp=1;
		
		if(szb>=cblkszb)
		{
			/* block didn't compress... */
			szb=cblkszb;
			docmp=0;
			
			memcpy(img->img_cblkcache_tbuf, ptr,
				1<<(img->ln2_cblksz));
		}
		
		if(blknum)
		{
			/* check and free existing block data if not a match */
			if(docmp)
			{
				j=(blknum>>48)&1023;
				if(j!=szb)
				{
					if(!j)
						j=cblkszb;
					TKDFS_ImageFreeBlockSpan(img,
						blknum&0x0000FFFFFFFFFFFFULL,
						j);
					ino_inf=TKDFS_GetImageCachedInode(img, cblkino, 0);
					TKDFS_SetImageInodeBlockNum(img, ino_inf, cblkidx, 0);
					blknum=0;
				}
			}else
			{
				if(blknum>>48)
				{
					j=(blknum>>48)&1023;
					TKDFS_ImageFreeBlockSpan(img,
						blknum&0x0000FFFFFFFFFFFFULL,
						j);
					ino_inf=TKDFS_GetImageCachedInode(img, cblkino, 0);
					TKDFS_SetImageInodeBlockNum(img, ino_inf, cblkidx, 0);
					blknum=0;
				}
			}
		}
		
		if(!blknum && szb)
		{
			blknum=TKDFS_ImageAllocateBlockSpan(img, szb);
			if(docmp)
			{
				blknum|=(1ULL<<60)|(((u64)szb)<<48);
			}
			ino_inf=TKDFS_GetImageCachedInode(img, cblkino, 1);
			TKDFS_SetImageInodeBlockNum(img, ino_inf, cblkidx, blknum);
		}
		
		if(blknum)
		{
			cblkid=blknum&0x0000FFFFFFFFFFFF;
			lba=img->lba_start+(cblkid<<shb);
			TKBDEV_WriteSectors(img->bdev,
				img->img_cblkcache_tbuf, lba, szb<<shb);
		}

		img->img_cblkcache_flag[h]&=~3;
		return(1);
	}
	
	return(0);
}

int TKDFS_SyncCompressedBlocks(TKDFS_ImageContext *img)
{
	int i, sh;

	sh=img->ln2_cblksz-14;
	
	for(i=0; i<(16>>sh); i++)
	{
		TKDFS_SyncCompressedBlockIdx(img, i);
	}
	return(0);
}

byte *TKDFS_GetImageCompressedBlock(TKDFS_ImageContext *img,
	int d_ino, int d_blk, int d_flg)
{
	TKDFS_InodeInfo *ino_inf;
	byte *ptr;
	u64 blknum, cblkid;
	s64 lba;
	int h, sh, shb, sz, szb, cblkszb, cblkino, cblkidx, docmp;
	int i, j, k;
	
	if(!img->img_cblkcache_data)
	{
		img->img_cblkcache_data=tkdfs_malloc(1<<18);
		img->img_cblkcache_tbuf=tkdfs_malloc(1<<(img->ln2_cblksz+1));
	}
	
	cblkszb=1<<(img->ln2_cblksz-img->ln2_blksz);
	
	sh=img->ln2_cblksz-14;
//	h=(((d_blk*251)>>8)&15)>>sh;
	h=((d_blk^(d_blk>>3)^(d_blk>>7))&15)>>sh;
	ptr=(byte *)(img->img_cblkcache_data)+(h<<(14+sh));

	shb=img->ln2_blksz-9;
	
	cblkino=img->img_cblkcache_ino[h];
	cblkidx=img->img_cblkcache_idx[h];
	if((cblkino==d_ino) && (cblkidx==d_blk))
	{
		if(d_flg&1)
			img->img_cblkcache_flag[h]|=1;
		return(ptr);
	}

	if(img->img_cblkcache_flag[h]&1)
	{
		TKDFS_SyncCompressedBlockIdx(img, h);
	}

	
	ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 0);
	blknum=TKDFS_GetImageInodeBlockNum(img, ino_inf, d_blk);

	if(!blknum)
	{
		/* no block here, so read in as zeroes. */
		memset(ptr, 0, 1<<sh);
	}else
	{
		cblkid=blknum&0x0000FFFFFFFFFFFF;
		lba=img->lba_start+(cblkid<<shb);
		szb=(blknum>>48)&1023;

		if(szb)
		{
			TKBDEV_ReadSectors(img->bdev,
				img->img_cblkcache_tbuf, lba, szb<<shb);
			TKDFS_DecompressBlockBuffer(ptr, img->img_cblkcache_tbuf, 
				1<<(img->ln2_cblksz+0),
				1<<(img->ln2_cblksz+1),
				ino_inf->ino_file->cmpmode);
		}else
		{
			TKBDEV_ReadSectors(img->bdev,
				ptr, lba, cblkszb<shb);
		}
	}

	img->img_cblkcache_ino[h]=d_ino;
	img->img_cblkcache_idx[h]=d_blk;
	img->img_cblkcache_flag[h]=0;
	if(d_flg&1)
		img->img_cblkcache_flag[h]|=1;
	return(ptr);
}

byte *TKDFS_GetImageCachedInodeBlock(TKDFS_ImageContext *img,
	int d_ino, s64 d_blk, int d_flg)
{
	TKDFS_InodeInfo *ino_inf;
	byte *blkptr;
	u64 blknum;
	int blkofs, idblksh;
//	int idx0, idx1, idx2, idx0r, idx1r, idx2r, blk0, blk1, blk2;
	
	ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 0);
	blkofs=d_blk;
	
	if(!ino_inf->ino_file)
	{
		/* Uninitialized Inode */
		if(!(d_flg&1))
			return(NULL);
		ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
		if(d_flg&8)
			{ TKDFS_InitializeImageFileInodeIdat(img, ino_inf, d_ino); }
		else
			{ TKDFS_InitializeImageFileInode(img, ino_inf, d_ino, d_flg); }

		ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
		if(!ino_inf->ino_file)
			return(NULL);
		
		if(d_flg&0x04)
		{
			/* If doing dirent IO, flag as a directory... */
			ino_inf->ino_file->mode=0x4000;
		}

		if(d_flg&0x20)
		{
			ino_inf->ino_file->cmpmode=3;
		}
	}

	if(d_flg&0x04)
	{
		if(!(ino_inf->ino_file->mode&0x4000))
		{
			tkdfs_printf("TKDFS_GetImageCachedInodeBlock: "
				"Debug, Mark As Dir, ino=%d\n", d_ino);
			ino_inf=TKDFS_GetImageCachedInode(img, d_ino, 1);
			ino_inf->ino_file->mode|=0x4000;
		}
	}

	if(ino_inf->ino_idat)
	{
		if(d_blk)
			return(NULL);
		return(ino_inf->ino_idat);
	}


	if(ino_inf->ino_file->cmpmode==0)
	{
		blknum=TKDFS_GetImageInodeBlockNum2(img, ino_inf, d_blk, d_flg);
		if(!blknum)
			return(NULL);

		blkptr=TKDFS_GetImageCachedBlock(img, blknum, d_flg);
		return(blkptr);
	}else
	{
		blkptr=TKDFS_GetImageCompressedBlock(img,
			d_ino, d_blk, d_flg);
		return(blkptr);
	}
	
	return(NULL);
}

int TKDFS_ConvertImageInlineToFileInode(
	TKDFS_ImageContext *img, TKDFS_InodeInfo *info)
{
	byte idbuf[512];
	byte *buf;
	int idsz, d_ino;
	
	idsz=info->ino_file->dsize;
	d_ino=info->d_ino;

	memcpy(idbuf, info->ino_idat, idsz);
	
	TKDFS_InitializeImageFileInode(img, info, d_ino, 0);
	info->d_ino=d_ino;
	info->d_flag|=1;
	
	buf=TKDFS_GetImageCachedInodeBlock(img, d_ino, 0, 1);
	memcpy(buf, idbuf, idsz);
	info->ino_file->dsize=idsz;
	info->ino_file->csize=idsz;
	return(1);
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
	static byte rec=0;
	byte *ptr, *ptr1;
	int sh, d_blk, d_ix2;
	sh=img->ln2_blksz;
	d_blk=d_idx>>(sh-2);
	d_ix2=d_idx&((1<<(sh-2))-1);
	if((d_ino==TKDFS_FIXINO_BLKBMP) && !rec)
	{
		rec++;
		ptr1=TKDFS_GetImageCachedInodeBlock(img, d_ino, d_blk+1,
			TKDFS_DFLAG_PREALLOC);
		rec--;
	}
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
	blkptr=TKDFS_GetImageCachedInodeBlock(img, d_ino, blkofs,
		TKDFS_DFLAG_ISDIR_RD);
	deptr=blkptr+blkrem;
	
//	deb=(TKDFS_DirEnt_Basic *)deptr;
	deb=&(info->t_deb);
	info->deb=deb;
	info->deh=(TKDFS_DirEnt_Header *)deb;

	if(blkptr)
		{ memcpy(deb, deptr, 64); }
	else
		{ memset(deb, 0, 64); }
	
//	memcpy(info->de_name, deb->name, 48);
	info->de_ino=deb->ino;

	info->img=img;
	info->di_ino=d_ino;
	info->di_idx=d_idx;
	
	if(deb->etype==1)
	{
		TKDFS_CopyName48Expand(info->de_name, deb->name);
	}else
		if(deb->etype==2)
	{
		/* file with multipart name */
		memcpy(info->de_name, deb->name, 40);
		ct=info->de_name+40;
		while(1)
		{
			ofs+=64;
			blkofs=ofs>>img->ln2_blksz;
			blkrem=ofs&((1<<img->ln2_blksz)-1);
			blkptr=TKDFS_GetImageCachedInodeBlock(img, d_ino, blkofs,
				TKDFS_DFLAG_ISDIR_RD);
			if(!blkptr)
				break;
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

u64 TKDFS_LongNameHash64(byte *name)
{
	byte tb[288];
	u32 *cs;
	u64 v;
	u64 h0, h1, h2, h3, h;
	int i;
	
	memset(tb, 0, 288);
	strncpy((char *)tb, (char *)name, 256);
	
	h0=1; h1=0; cs=(u32 *)tb;
	while(*cs)
		{ v=*cs++; h0+=v; h1+=h0; }
	h0=((u32)h0)+(h0>>32);
	h1=((u32)h1)+(h1>>32);
	h0=((u32)h0)+(h0>>32);
	h1=((u32)h1)+(h1>>32);
	h=((u32)h0)|(h1<<32);
//	h2=0x8080808080808080ULL;
//	h3=~h2;
//	h=h+(h&h2);
//	h=h&h3;
	return(h);
}


int TKDFS_CopyName48Expand(byte *dst, byte *src)
{
	int isu8;

	memcpy(dst, src, 48);
	return(0);
}

int TKDFS_InitBaseName48(char *bname, char *d_name)
{
	int nl1, nl2, nln, isu8;

	nln=strlen(d_name);
	nl1=nln;
	if(nl1>48)
		nl1=48;

	memset(bname, 0, 48);
	memcpy(bname, d_name, nl1);

	if(nln>48)
	{
		*(u64 *)(bname+40)=TKDFS_LongNameHash64(d_name);
		return(1);
	}
	return(0);
}

int TKDFS_WriteImageDirent(TKDFS_ImageContext *img,
	TKDFS_DirentInfo *info, int d_ino, int d_idx)
{
	TKDFS_DirEnt_Basic *deb;
	byte *blkptr, *deptr;
	char *cs, *ct;
	int ofs, blkofs, blkrem, nln, nl1, feh, islfn;

	if(info->di_idx!=d_idx)
		{ TKDFS_DBGBREAK }

	ofs=d_idx*64;
	blkofs=ofs>>img->ln2_blksz;
	blkrem=ofs&((1<<img->ln2_blksz)-1);
	blkptr=TKDFS_GetImageCachedInodeBlock(img, d_ino, blkofs,
		TKDFS_DFLAG_ISDIR_WR);
	deptr=blkptr+blkrem;

	deb=&(info->t_deb);
	info->deb=deb;

	if(d_idx)
	{
		nln=strlen(info->de_name);
		nl1=nln;
		if(nl1>48)
			nl1=48;

		deb->ino=info->de_ino;
		deb->ino_hi=info->de_ino>>32;

	//	memset(deb->name, 0, 48);
	//	memcpy(deb->name, info->de_name, nl1);
	//	deb->ino=info->de_ino;
		islfn=TKDFS_InitBaseName48(deb->name, info->de_name);

//		if(nln>48)
		if(islfn)
		{
	//		*(u64 *)(deb->name+40)=TKDFS_LongNameHash64(info->de_name);
			feh=TKDFS_ShortNameHash48(deb->name);
			if((deb->etype&7)!=2)
				deb->etype=2;
		}else
		{
			feh=0;
			if((deb->etype&7)!=1)
				deb->etype=1;
		}
	}

	memcpy(deptr, deb, 64);
	tkdfs_printf("TKDFS_WriteImageDirent: %2u/%2u n=%-24s ln=%2u rn=%2u pn=%2u\n",
		d_ino, d_idx, deb->name, deb->lsn, deb->rsn, deb->psn);
		
	if(deb->etype==2)
	{
		/* file with multipart name */
		cs=info->de_name+40;
		while(1)
		{
			ofs+=64;
			blkofs=ofs>>img->ln2_blksz;
			blkrem=ofs&((1<<img->ln2_blksz)-1);
			blkptr=TKDFS_GetImageCachedInodeBlock(img, d_ino, blkofs,
				TKDFS_DFLAG_ISDIR_WR);
			deptr=blkptr+blkrem;

			memset(deptr, 0, 64);
			deptr[12]=feh;
			deptr[15]=3;
			nl1=strlen(cs);
			if(nl1>60)
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

int tkdfs_qwordcmp8(u64 v0, u64 v1)
{
	u64 t0, t1;
	int rel;

	if(v0==v1)
		return(0);

	t0=(u32)v0;	t1=(u32)v1;
	if(t0!=t1)
	{
		t0=ntohl(t0);
		t1=ntohl(t1);

		rel=((s64)(t0-t1))>>63;
		return(rel*2+1);
	}

	t0=ntohl(v0>>32);
	t1=ntohl(v1>>32);
	rel=((s64)(t0-t1))>>63;
	return(rel*2+1);
}

int tkdfs_memcmp48(void *ds0, void *ds1)
{
	u64 *cs0, *cs1;
	u64 v0, v1, v2, v3;

	cs0=(u64 *)ds0;
	cs1=(u64 *)ds1;

	v0=cs0[0];	v1=cs1[0];
	v2=cs0[1];	v3=cs1[1];
	if((v0!=v1) || (v2!=v3))
	{
		if(v0!=v1)
			return(tkdfs_qwordcmp8(v0, v1));
		return(tkdfs_qwordcmp8(v2, v3));
	}
	v0=cs0[2];	v1=cs1[2];
	v2=cs0[3];	v3=cs1[3];
	if((v0!=v1) || (v2!=v3))
	{
		if(v0!=v1)
			return(tkdfs_qwordcmp8(v0, v1));
		return(tkdfs_qwordcmp8(v2, v3));
	}
	v0=cs0[4];	v1=cs1[4];
	v2=cs0[5];	v3=cs1[5];
	if((v0!=v1) || (v2!=v3))
	{
		if(v0!=v1)
			return(tkdfs_qwordcmp8(v0, v1));
		return(tkdfs_qwordcmp8(v2, v3));
	}

	return(0);
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

	dinfo->di_ino=sinfo->di_ino;
	dinfo->di_idx=sinfo->di_idx;

	dinfo->is_write=sinfo->is_write;
	dinfo->is_dirty=sinfo->is_dirty;

	return(0);
}

int TKDFS_ImageAllocInodeDirentName(TKDFS_ImageContext *img,
	TKDFS_DirentInfo *info,
	int d_ino, char *d_name)
{
	TKDFS_DirentInfo t_inf0;
	TKDFS_DirentInfo t_inf1;
	TKDFS_DirentInfo t_inf2;
	TKDFS_DirentInfo *inf0;
	TKDFS_DirentInfo *inf1;
	TKDFS_DirentInfo *inf2;
	int piv0, piv1, piv2, cmp, nln, nde;

	inf0=&t_inf0;
	inf1=&t_inf1;
	inf2=&t_inf2;

	memset(inf0, 0, sizeof(TKDFS_DirentInfo));
	memset(inf1, 0, sizeof(TKDFS_DirentInfo));
	memset(inf2, 0, sizeof(TKDFS_DirentInfo));

	nln=strlen(d_name);

	TKDFS_ReadImageDirent(img, inf0, d_ino, 0);
	piv0=inf0->deb->rsn;
	
	if(piv0 && (nln<48))
	{
		TKDFS_ReadImageDirent(img, inf1, d_ino, piv0);
		piv1=inf1->deb->rsn;
		TKDFS_ReadImageDirent(img, inf2, d_ino, piv1);
		
		inf0->deb->rsn=piv1;
		inf2->deb->lsn=0;
		
		strcpy(inf1->de_name, d_name);
		inf1->deb->rsn=0;
		inf1->deb->lsn=0;
		inf1->deb->psn=0;
		inf1->deb->hsn=0;
		inf1->deb->ino=0;
		inf1->deb->ino_hi=0;

		TKDFS_WriteImageDirent(img, inf0, d_ino, 0);
		TKDFS_WriteImageDirent(img, inf1, d_ino, piv0);
		TKDFS_WriteImageDirent(img, inf2, d_ino, piv1);
		TKDFS_CopyDirentInfo(img, info, inf1);
		return(piv0);
	}
	
	piv0=inf0->deh->lastent;
	if(piv0>0)
	{
		nde=1;
		if(nln>48)
			nde++;
		if(nln>100)
			nde++;
		if(nln>160)
			nde++;
		if(nln>220)
			nde++;
	
		inf0->deh->lastent=piv0+nde;

		TKDFS_ReadImageDirent(img, inf1, d_ino, piv0);

		strcpy(inf1->de_name, d_name);
		inf1->deb->rsn=0;
		inf1->deb->lsn=0;
		inf1->deb->psn=0;
		inf1->deb->hsn=0;
		inf1->deb->ino=0;
		inf1->deb->ino_hi=0;

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
		TKDFS_ReadImageDirent(img, inf2, d_ino, piv1);
		inf1->deb->rsn=piv1;
		if(piv1>0)
			inf2->deb->lsn=piv0;
		TKDFS_WriteImageDirent(img, inf1, d_ino, piv0);
		if(piv1>0)
			TKDFS_WriteImageDirent(img, inf2, d_ino, piv1);

		inf0->deb->rsn=piv0;
		piv1=piv0;
		piv2=piv0;
	}
	TKDFS_WriteImageDirent(img, inf0, d_ino, 0);
	
	return(TKDFS_ImageAllocInodeDirentName(img, info, d_ino, d_name));
}

int TKDFS_GetDirentLeftIdx(
	TKDFS_DirentInfo *info)
{
	int piv;
	piv=info->deb->lsn;
	piv|=((info->deb->hsn>>0)&31)<<16;
	return(piv);
}

int TKDFS_GetDirentRightIdx(
	TKDFS_DirentInfo *info)
{
	int piv;
	piv=info->deb->rsn;
	piv|=((info->deb->hsn>>5)&31)<<16;
	return(piv);
}

int TKDFS_GetDirentParentIdx(
	TKDFS_DirentInfo *info)
{
	int piv;
	piv=info->deb->psn;
	piv|=((info->deb->hsn>>10)&31)<<16;
	return(piv);
}

int TKDFS_SetDirentLeftIdx(
	TKDFS_DirentInfo *info, int idx)
{
	info->deb->lsn=idx&0xFFFF;
	info->deb->hsn&=~(31<<0);
	info->deb->hsn|=((idx>>16)&31)<<0;
	return(0);
}

int TKDFS_SetDirentRightIdx(
	TKDFS_DirentInfo *info, int idx)
{
	info->deb->rsn=idx&0xFFFF;
	info->deb->hsn&=~(31<<5);
	info->deb->hsn|=((idx>>16)&31)<<5;
	return(0);
}

int TKDFS_SetDirentParentIdx(
	TKDFS_DirentInfo *info, int idx)
{
	info->deb->psn=idx&0xFFFF;
	info->deb->hsn&=~(31<<10);
	info->deb->hsn|=((idx>>16)&31)<<10;
	return(0);
}

int TKDFS_ImageCheckDirentUpdateZ(
	TKDFS_ImageContext *img, int d_ino, int d_idx)
{
	TKDFS_DirentInfo t_info0;
	TKDFS_DirentInfo t_info1;
	TKDFS_DirentInfo t_info2;
	TKDFS_DirentInfo *info0;
	TKDFS_DirentInfo *info1;
	TKDFS_DirentInfo *info2;
	int piv1, piv2, z, z1, z2, ohsn;
	int piv1p, piv2p;

	info0=&t_info0;
	info1=&t_info1;
	info2=&t_info2;
	
	TKDFS_ReadImageDirent(img, info0, d_ino, d_idx);
		
	z=0;

	piv1=TKDFS_GetDirentLeftIdx(info0);
	piv2=TKDFS_GetDirentRightIdx(info0);
	z1=-1;
	z2=-1;
		
	if(piv1>0)
	{
		TKDFS_ReadImageDirent(img, info1, d_ino, piv1);
		z1=info1->deb->zdepth;
		piv1p=TKDFS_GetDirentParentIdx(info1);
		if(piv1p!=d_idx)
			{ TKDFS_DBGBREAK }
	}

	if(piv2>0)
	{
		TKDFS_ReadImageDirent(img, info2, d_ino, piv2);
		z2=info2->deb->zdepth;
		piv2p=TKDFS_GetDirentParentIdx(info2);
		if(piv2p!=d_idx)
			{ TKDFS_DBGBREAK }
	}
		
	if(z1>=z)
		z=z1+1;
	if(z2>=z)
		z=z2+1;
	
	ohsn=info0->deb->hsn;
	info0->deb->hsn&=~0x8000;
	if((z1+2)<z2)
		info0->deb->hsn|=0x8000;
	if((z2+2)<z1)
		info0->deb->hsn|=0x8000;
	
	if((z!=info0->deb->zdepth) || (info0->deb->hsn!=ohsn))
	{
		info0->deb->zdepth=z;
		TKDFS_WriteImageDirent(img, info0, d_ino, d_idx);
	}
	return(0);
}

int TKDFS_ImageCheckDirentRebalance(
	TKDFS_ImageContext *img, TKDFS_DirentInfo *info)
{
	TKDFS_DirentInfo t_info0p;
	TKDFS_DirentInfo t_info0;
	TKDFS_DirentInfo t_info1;
	TKDFS_DirentInfo t_info2;

	TKDFS_DirentInfo t_info1nl;
	TKDFS_DirentInfo t_info1nr;
	TKDFS_DirentInfo t_info2nl;
	TKDFS_DirentInfo t_info2nr;

	TKDFS_DirentInfo *info0p;
	TKDFS_DirentInfo *info0;
	TKDFS_DirentInfo *info1;
	TKDFS_DirentInfo *info2;

	TKDFS_DirentInfo *info1nl;
	TKDFS_DirentInfo *info1nr;
	TKDFS_DirentInfo *info2nl;
	TKDFS_DirentInfo *info2nr;

	int toppiv, pivot, piv0, piv1, piv2, z, z1, z2;
	int piv0pl, piv0pr;
	int piv1nl, piv1nr;
	int piv2nl, piv2nr;
	int piv0p, d_ino, lim;
	int i, j, k;

	info0p=&t_info0p;
	info0=&t_info0;
	info1=&t_info1;
	info2=&t_info2;

	info1nl=&t_info1nl;
	info1nr=&t_info1nr;
	info2nl=&t_info2nl;
	info2nr=&t_info2nr;

	d_ino=info->di_ino;
	pivot=info->di_idx;

	lim=64;
	while((pivot>0) && (lim>0))
	{
		TKDFS_ImageCheckDirentUpdateZ(img, d_ino, pivot);
		TKDFS_ReadImageDirent(img, info0, d_ino, pivot);
		lim--;
		
		piv0=pivot;

		if(!(info0->deb->hsn&0x8000) || (lim<32))
		{
			pivot=TKDFS_GetDirentParentIdx(info0);
			continue;
		}

		tkdfs_printf("TKDFS_ImageCheckDirentRebalance: %2u/%2u\n", d_ino, pivot);

		piv1=TKDFS_GetDirentLeftIdx(info0);
		piv2=TKDFS_GetDirentRightIdx(info0);
		piv0p=TKDFS_GetDirentParentIdx(info0);
		z1=-1;
		z2=-1;

//		if(piv0p>0)
		if(1)
		{
			TKDFS_ReadImageDirent(img, info0p, d_ino, piv0p);
			piv0pl=TKDFS_GetDirentLeftIdx(info0p);
			piv0pr=TKDFS_GetDirentRightIdx(info0p);
		}

		piv1nl=0;	piv1nr=0;
		piv2nl=0;	piv2nr=0;
			
		if(piv1>0)
		{
			TKDFS_ReadImageDirent(img, info1, d_ino, piv1);
			z1=info1->deb->zdepth;
			piv1nl=TKDFS_GetDirentLeftIdx(info1);
			piv1nr=TKDFS_GetDirentRightIdx(info1);
			if(piv1nl>0)
				TKDFS_ReadImageDirent(img, info1nl, d_ino, piv1nl);
			if(piv1nr>0)
				TKDFS_ReadImageDirent(img, info1nr, d_ino, piv1nr);
		}

		if(piv2>0)
		{
			TKDFS_ReadImageDirent(img, info2, d_ino, piv2);
			z2=info2->deb->zdepth;
			piv2nl=TKDFS_GetDirentLeftIdx(info2);
			piv2nr=TKDFS_GetDirentRightIdx(info2);
			if(piv2nl>0)
				TKDFS_ReadImageDirent(img, info2nl, d_ino, piv2nl);
			if(piv2nr>0)
				TKDFS_ReadImageDirent(img, info2nr, d_ino, piv2nr);
		}
		
		if(z1>(z2+2))
		{
			TKDFS_SetDirentLeftIdx(info0, piv1nr);
			TKDFS_SetDirentRightIdx(info0, piv2);

			TKDFS_SetDirentLeftIdx(info1, piv1nl);
			TKDFS_SetDirentRightIdx(info1, pivot);

			TKDFS_SetDirentParentIdx(info0, piv1);
			TKDFS_SetDirentParentIdx(info1, piv0p);
			
			if(piv0pl==pivot)
				TKDFS_SetDirentLeftIdx(info0p, piv1);
			if(piv0pr==pivot)
				TKDFS_SetDirentRightIdx(info0p, piv1);

			if(piv1nr>0)
				TKDFS_SetDirentParentIdx(info1nr, piv0);
			if(piv2>0)
				TKDFS_SetDirentParentIdx(info2, piv0);
			if(piv1nl>0)
				TKDFS_SetDirentParentIdx(info1nl, piv1);

			if(piv1nl>0)
				TKDFS_WriteImageDirent(img, info1nl, d_ino, info1nl->di_idx);
			if(piv1nr>0)
				TKDFS_WriteImageDirent(img, info1nr, d_ino, info1nr->di_idx);
			if(piv2nl>0)
				TKDFS_WriteImageDirent(img, info2nl, d_ino, info2nl->di_idx);
			if(piv2nr>0)
				TKDFS_WriteImageDirent(img, info2nr, d_ino, info2nr->di_idx);
			if(piv1>0)
				TKDFS_WriteImageDirent(img, info1, d_ino, info1->di_idx);
			if(piv2>0)
				TKDFS_WriteImageDirent(img, info2, d_ino, info2->di_idx);
			TKDFS_WriteImageDirent(img, info0, d_ino, info0->di_idx);
			TKDFS_WriteImageDirent(img, info0p, d_ino, info0p->di_idx);

			TKDFS_ImageCheckDirentUpdateZ(img, d_ino, info0->di_idx);
			TKDFS_ImageCheckDirentUpdateZ(img, d_ino, info1->di_idx);

//			TKDFS_ImageCheckDirentUpdateZ(ing, d_ino, pivot)

			tkdfs_printf("TKDFS_ImageCheckDirentRebalance: OK-A %2u/%2u\n",
				d_ino, pivot);

			continue;
		}
		
		if(z2>(z1+2))
		{
			TKDFS_SetDirentLeftIdx(info0, piv1);
			TKDFS_SetDirentRightIdx(info0, piv2nl);

			TKDFS_SetDirentLeftIdx(info2, pivot);
			TKDFS_SetDirentRightIdx(info2, piv2nr);

			TKDFS_SetDirentParentIdx(info0, piv2);
			TKDFS_SetDirentParentIdx(info2, piv0p);
			
			if(piv0pl==pivot)
				TKDFS_SetDirentLeftIdx(info0p, piv2);
			if(piv0pr==pivot)
				TKDFS_SetDirentRightIdx(info0p, piv2);

			if(piv2nl>0)
				TKDFS_SetDirentParentIdx(info2nl, piv0);
			if(piv1>0)
				TKDFS_SetDirentParentIdx(info1, piv0);
			if(piv2nr>0)
				TKDFS_SetDirentParentIdx(info2nr, piv2);

			if(piv1nl>0)
				TKDFS_WriteImageDirent(img, info1nl, d_ino, info1nl->di_idx);
			if(piv1nr>0)
				TKDFS_WriteImageDirent(img, info1nr, d_ino, info1nr->di_idx);
			if(piv2nl>0)
				TKDFS_WriteImageDirent(img, info2nl, d_ino, info2nl->di_idx);
			if(piv2nr>0)
				TKDFS_WriteImageDirent(img, info2nr, d_ino, info2nr->di_idx);
			if(piv1>0)
				TKDFS_WriteImageDirent(img, info1, d_ino, info1->di_idx);
			if(piv2>0)
				TKDFS_WriteImageDirent(img, info2, d_ino, info2->di_idx);
			TKDFS_WriteImageDirent(img, info0, d_ino, info0->di_idx);
			TKDFS_WriteImageDirent(img, info0p, d_ino, info0p->di_idx);

			TKDFS_ImageCheckDirentUpdateZ(img, d_ino, info0->di_idx);
			TKDFS_ImageCheckDirentUpdateZ(img, d_ino, info2->di_idx);

			tkdfs_printf("TKDFS_ImageCheckDirentRebalance: OK-B %2u/%2u\n",
				d_ino, pivot);

//			TKDFS_ImageCheckDirentUpdateZ(ing, d_ino, pivot)
			continue;
		}
			
		tkdfs_printf("TKDFS_ImageCheckDirentRebalance: OK %2u/%2u\n", d_ino, pivot);

		pivot=TKDFS_GetDirentParentIdx(info0);
	}
	return(0);
}

int TKDFS_ImageFreeDirent(
	TKDFS_ImageContext *img, TKDFS_DirentInfo *info)
{
	TKDFS_DirentInfo t_info0h;
	TKDFS_DirentInfo t_info0;
	TKDFS_DirentInfo t_info1;
	TKDFS_DirentInfo *info0h;
	TKDFS_DirentInfo *info0;
	TKDFS_DirentInfo *info1;
	int piv0, piv1, d_ino;

	info0h=&t_info0h;
	info0=&t_info0;
	info1=&t_info1;

	d_ino=info->di_ino;
	piv0=info->di_idx;

	TKDFS_ReadImageDirent(img, info0h, d_ino, 0);
	TKDFS_ReadImageDirent(img, info0, d_ino, piv0);

//	piv0=TKDFS_GetDirentLeftIdx(info0p);
	piv1=TKDFS_GetDirentRightIdx(info0h);
	TKDFS_ReadImageDirent(img, info1, d_ino, piv1);

	TKDFS_SetDirentRightIdx(info0, piv1);
	TKDFS_SetDirentLeftIdx(info1, piv0);
	TKDFS_SetDirentLeftIdx(info0, 0);

	TKDFS_SetDirentParentIdx(info0, 0);

	TKDFS_SetDirentRightIdx(info0h, piv0);


	TKDFS_WriteImageDirent(img, info0, d_ino, info0->di_idx);
	TKDFS_WriteImageDirent(img, info1, d_ino, info1->di_idx);
	TKDFS_WriteImageDirent(img, info0h, d_ino, 0);
	
	return(0);
}

int TKDFS_ImageRemoveDirentRebalance(
	TKDFS_ImageContext *img, TKDFS_DirentInfo *info)
{
	TKDFS_DirentInfo t_info0p;
//	TKDFS_DirentInfo t_info0h;
	TKDFS_DirentInfo t_info0;
	TKDFS_DirentInfo t_info1;
	TKDFS_DirentInfo t_info2;

	TKDFS_DirentInfo t_info1nl;
	TKDFS_DirentInfo t_info1nr;
	TKDFS_DirentInfo t_info2nl;
	TKDFS_DirentInfo t_info2nr;

	TKDFS_DirentInfo *info0p;
//	TKDFS_DirentInfo *info0h;
	TKDFS_DirentInfo *info0;
	TKDFS_DirentInfo *info1;
	TKDFS_DirentInfo *info2;

	TKDFS_DirentInfo *info1nl;
	TKDFS_DirentInfo *info1nr;
	TKDFS_DirentInfo *info2nl;
	TKDFS_DirentInfo *info2nr;

	int toppiv, pivot, piv0, piv1, piv2, z, z1, z2;
	int piv0pl, piv0pr;
	int piv1nl, piv1nr;
	int piv2nl, piv2nr;
	int piv0p, d_ino, lim, delgood;
	int i, j, k;

	info0p=&t_info0p;
//	info0h=&t_info0h;
	info0=&t_info0;
	info1=&t_info1;
	info2=&t_info2;

	info1nl=&t_info1nl;
	info1nr=&t_info1nr;
	info2nl=&t_info2nl;
	info2nr=&t_info2nr;

	d_ino=info->di_ino;
	pivot=info->di_idx;

	TKDFS_ReadImageDirent(img, info0, d_ino, pivot);
		
	piv0=pivot;

	tkdfs_printf("TKDFS_ImageRemoveDirentRebalance: %2u/%2u\n", d_ino, pivot);

	piv1=TKDFS_GetDirentLeftIdx(info0);
	piv2=TKDFS_GetDirentRightIdx(info0);
	piv0p=TKDFS_GetDirentParentIdx(info0);

//	TKDFS_ReadImageDirent(img, info0h, d_ino, 0);

	TKDFS_ReadImageDirent(img, info0p, d_ino, piv0p);
	piv0pl=TKDFS_GetDirentLeftIdx(info0p);
	piv0pr=TKDFS_GetDirentRightIdx(info0p);

	piv1nl=0;	piv1nr=0;
	piv2nl=0;	piv2nr=0;
			
	if(piv1>0)
	{
		TKDFS_ReadImageDirent(img, info1, d_ino, piv1);
		z1=info1->deb->zdepth;
		piv1nl=TKDFS_GetDirentLeftIdx(info1);
		piv1nr=TKDFS_GetDirentRightIdx(info1);
		if(piv1nl>0)
			TKDFS_ReadImageDirent(img, info1nl, d_ino, piv1nl);
		if(piv1nr>0)
			TKDFS_ReadImageDirent(img, info1nr, d_ino, piv1nr);
	}

	if(piv2>0)
	{
		TKDFS_ReadImageDirent(img, info2, d_ino, piv2);
		z2=info2->deb->zdepth;
		piv2nl=TKDFS_GetDirentLeftIdx(info2);
		piv2nr=TKDFS_GetDirentRightIdx(info2);
		if(piv2nl>0)
			TKDFS_ReadImageDirent(img, info2nl, d_ino, piv2nl);
		if(piv2nr>0)
			TKDFS_ReadImageDirent(img, info2nr, d_ino, piv2nr);
	}

	delgood=1;

	if(!piv1 && !piv2)
	{
		if(piv0pl==pivot)
			TKDFS_SetDirentLeftIdx(info0p, 0);
		if(piv0pr==pivot)
			TKDFS_SetDirentRightIdx(info0p, 0);
	}
	else
		if(piv1 && !piv2)
	{
		if(piv0pl==pivot)
			TKDFS_SetDirentLeftIdx(info0p, piv1);
		if(piv0pr==pivot)
			TKDFS_SetDirentRightIdx(info0p, piv1);
		TKDFS_SetDirentParentIdx(info1, piv0p);
	}else
		if(!piv1 && piv2)
	{
		if(piv0pl==pivot)
			TKDFS_SetDirentLeftIdx(info0p, piv2);
		if(piv0pr==pivot)
			TKDFS_SetDirentRightIdx(info0p, piv2);
		TKDFS_SetDirentParentIdx(info2, piv0p);
	}else
		if(piv1 && piv2)
	{
		if(!piv1nr)
		{
			TKDFS_SetDirentRightIdx(info1, piv2);
			TKDFS_SetDirentParentIdx(info2, piv1);
			delgood=2;
		}else
			if(!piv2nl)
		{
			TKDFS_SetDirentLeftIdx(info2, piv1);
			TKDFS_SetDirentParentIdx(info1, piv2);
			delgood=3;
		}
		else
		{
			//errm... delay for now...
			info0->de_ino=0;
			delgood=0;
		}
	}

	if(piv1nl>0)
		TKDFS_WriteImageDirent(img, info1nl, d_ino, info1nl->di_idx);
	if(piv1nr>0)
		TKDFS_WriteImageDirent(img, info1nr, d_ino, info1nr->di_idx);
	if(piv2nl>0)
		TKDFS_WriteImageDirent(img, info2nl, d_ino, info2nl->di_idx);
	if(piv2nr>0)
		TKDFS_WriteImageDirent(img, info2nr, d_ino, info2nr->di_idx);
	if(piv1>0)
		TKDFS_WriteImageDirent(img, info1, d_ino, info1->di_idx);
	if(piv2>0)
		TKDFS_WriteImageDirent(img, info2, d_ino, info2->di_idx);
	TKDFS_WriteImageDirent(img, info0, d_ino, info0->di_idx);
	TKDFS_WriteImageDirent(img, info0p, d_ino, info0p->di_idx);

	TKDFS_ImageCheckDirentUpdateZ(img, d_ino, info0->di_idx);
	TKDFS_ImageCheckDirentUpdateZ(img, d_ino, info1->di_idx);

	if(delgood)
	{
//		TKDFS_ReadImageDirent(img, info0, d_ino, pivot);
		TKDFS_ImageFreeDirent(img, info0);
	}

	if(piv0p && !info0p->de_ino && delgood)
	{
		TKDFS_ImageRemoveDirentRebalance(img, info0p);
	}else if(delgood)
	{
		if(delgood==2)
			{ TKDFS_ImageCheckDirentRebalance(img, info1); }
		if(delgood==3)
			{ TKDFS_ImageCheckDirentRebalance(img, info2); }
		if((delgood==1) && piv0p)
			TKDFS_ImageCheckDirentRebalance(img, info0p);
	}

	return(0);
}

int TKDFS_ImageLookupInodeDirentName(TKDFS_ImageContext *img,
	TKDFS_DirentInfo *info, int d_ino, char *d_name, int dfl)
{
	TKDFS_DirentInfo t_info1;
	TKDFS_DirentInfo t_info2;
	char d_basename[52];
//	int pivstk[64];
	TKDFS_DirentInfo *info1;
	TKDFS_DirentInfo *info2;
	int pivot, piv1, piv2, cmp, rsnix;
	int pivstkpos, treedirty;

	if(!(*d_name))
	{
		/* reject, invalid name */
		info->de_ino=0;
		info->di_ino=0;
		info->di_idx=0;
		return(0);
	}	

	TKDFS_ReadImageDirent(img, info, d_ino, 0);
//	pivot=info->deb->lsn;
	pivot=TKDFS_GetDirentLeftIdx(info);
	rsnix=-1;
	
	info1=&t_info1;
	
	TKDFS_InitBaseName48(d_basename, d_name);
	
	if(!pivot && (dfl&1))
	{
		pivot=TKDFS_ImageAllocInodeDirentName(img, info1, d_ino, d_name);
		TKDFS_ReadImageDirent(img, info, d_ino, 0);

		TKDFS_SetDirentLeftIdx(info, pivot);
		TKDFS_SetDirentParentIdx(info1, 0);
		TKDFS_WriteImageDirent(img, info1, d_ino, pivot);
		TKDFS_WriteImageDirent(img, info, d_ino, 0);

		TKDFS_ReadImageDirent(img, info, d_ino, pivot);
		return(1);
	}
	
	pivstkpos=0;
	treedirty=0;
	
	while(pivot)
	{
		TKDFS_ReadImageDirent(img, info, d_ino, pivot);
//		cmp=tkdfs_strcmp8(d_name, info->de_name);
		cmp=tkdfs_memcmp48(d_basename, info->deb->name);
		if(cmp<0)
		{
//			pivstk[pivstkpos++]=pivot|0x10000000;

//			piv1=info->deb->lsn;
//			piv2=info->deb->rsn;
			piv1=TKDFS_GetDirentLeftIdx(info);
			piv2=TKDFS_GetDirentRightIdx(info);
			if((piv1<=0) && (dfl&1))
			{
				piv1=TKDFS_ImageAllocInodeDirentName(img, info1, d_ino, d_name);
				TKDFS_SetDirentLeftIdx(info, piv1);
				TKDFS_SetDirentParentIdx(info1, pivot);
				TKDFS_WriteImageDirent(img, info1, d_ino, piv1);
				TKDFS_WriteImageDirent(img, info, d_ino, pivot);
				treedirty=1;
			}

			rsnix=pivot;
			pivot=piv1;
			continue;
		}
		if(cmp>0)
		{
//			pivstk[pivstkpos++]=pivot|0x20000000;

//			piv1=info->deb->rsn;
			piv1=TKDFS_GetDirentRightIdx(info);
			if((piv1<=0) && (dfl&1))
			{
				piv1=TKDFS_ImageAllocInodeDirentName(img, info1, d_ino, d_name);
				TKDFS_SetDirentRightIdx(info, piv1);
				TKDFS_SetDirentParentIdx(info1, pivot);
				TKDFS_WriteImageDirent(img, info1, d_ino, piv1);
				TKDFS_WriteImageDirent(img, info, d_ino, pivot);
				treedirty=1;
			}

			pivot=piv1;
			
			continue;
		}

//		pivstk[pivstkpos++]=pivot|0x30000000;

//		memcpy(info->pivstk, pivstk, pivstkpos*sizeof(int));
//		info->pivstkpos=pivstkpos;
		
		if(dfl&TKDFS_DFLAG_DELETE)
		{
			TKDFS_ImageRemoveDirentRebalance(img, info);
			return(1);
		}
			
		if(treedirty)
		{
			TKDFS_ImageCheckDirentRebalance(img, info);
		}

		return(1);
	}
	
	info->de_ino=0;
	info->di_ino=0;
	info->di_idx=0;
	/* not found */
	return(0);
}

int TKDFS_WalkDirEntNext(TKDFS_ImageContext *img, TKDFS_DirentInfo *dee)
{
	TKDFS_DirentInfo t_info1;
	TKDFS_DirentInfo t_info2;
	TKDFS_DirentInfo *info1;
	TKDFS_DirentInfo *info2;
	int toppiv, pivot, piv1, piv2;

	info1=&t_info1;

	if(dee->di_idx<0)
	{
		/* new readdir, initialize walk */
		TKDFS_ReadImageDirent(img, info1, dee->di_ino, 0);
//		pivot=info1->deb->lsn;
		pivot=TKDFS_GetDirentLeftIdx(info1);

		if(!pivot)
		{
			/* empty directory */
			return(0);
		}
		
		/* step all the way down left side of tree */
		TKDFS_ReadImageDirent(img, info1, dee->di_ino, pivot);
		piv1=TKDFS_GetDirentLeftIdx(info1);
		while(piv1>0)
		{
			pivot=piv1;
			TKDFS_ReadImageDirent(img, info1, dee->di_ino, pivot);
			piv1=TKDFS_GetDirentLeftIdx(info1);
		}

		TKDFS_ReadImageDirent(img, dee, dee->di_ino, pivot);
		return(1);
	}

	/* Check descent into right subtree. */
	piv1=TKDFS_GetDirentRightIdx(dee);
	if(piv1>0)
	{
		/* step all the way down left side of tree */
		while(piv1>0)
		{
			pivot=piv1;
			TKDFS_ReadImageDirent(img, info1, dee->di_ino, pivot);
			piv1=TKDFS_GetDirentLeftIdx(info1);
		}
		TKDFS_ReadImageDirent(img, dee, dee->di_ino, pivot);
		return(1);
	}

	pivot=dee->di_idx;
	piv1=TKDFS_GetDirentParentIdx(dee);
	if(piv1>0)
	{
		TKDFS_ReadImageDirent(img, info1, dee->di_ino, piv1);
		
		/* if coming from right sub-tree, walk up the tree. */
		piv2=TKDFS_GetDirentRightIdx(info1);
		while((piv1>0) && (pivot==piv2))
		{
			pivot=piv1;
			piv1=TKDFS_GetDirentParentIdx(info1);
			TKDFS_ReadImageDirent(img, info1, dee->di_ino, piv1);
			piv2=TKDFS_GetDirentRightIdx(info1);
		}
		
		if(piv1>0)
		{
			pivot=piv1;
			TKDFS_ReadImageDirent(img, dee, dee->di_ino, pivot);
			return(1);
		}

		/* end of tree */
		return(0);
	}

	/* end of tree */
	return(0);
}

int TKDFS_ImageLookupInodePathI(TKDFS_ImageContext *img,
	TKDFS_DirentInfo *info, char *path, int dfl)
{
	char tbn[128];
	TKDFS_DirentInfo t_info1;
	TKDFS_DirentInfo *info1;
	char *cs, *ct;
	int ino, rv, isdir, imd, sz, dfl1;
	
	info1=&t_info1;

	cs=path;
	while(*cs=='/')
		cs++;
	ino=1; /* root directory */

	info->img=img;
	info->de_ino=ino;
	info->di_ino=ino;
	info->di_idx=0;

	while(*cs)
	{
		ct=tbn;
		while(*cs && (*cs!='/'))
			*ct++=*cs++;
		*ct=0;
		isdir=0;
		if(*cs=='/')
		{
			isdir=1;
			cs++;
		}

		if(!tbn[0])
			break;

		dfl1=dfl;
		if(isdir)
			dfl1&=~TKDFS_DFLAG_DELETE;

		rv=TKDFS_ImageLookupInodeDirentName(img, info, ino, tbn, dfl1);
		if(rv<=0)
			return(0);
		if(!info->de_ino)
		{
			if(!(dfl&1))
				return(0);
				
			dfl1=1;
			if(isdir)
				dfl1|=TKDFS_DFLAG_ISDIR;
//			info->de_ino=TKDFS_ImageAllocateInode(img);
			info->de_ino=TKDFS_ImageAllocateInodeDeh(img, tbn, ino, dfl1);
			TKDFS_WriteImageDirent(img, info, ino, info->di_idx);
			if(isdir)
			{
				TKDFS_ReadImageDirent(img, info1, info->de_ino, 0);
				TKDFS_WriteImageDirent(img, info1, info->de_ino, 0);
			}
		}
		ino=info->de_ino;

		imd=TKDFS_GetDirEntMode(info);
		if(((imd&0xF000)==0xA000) && isdir)
		{
			sz=TKDFS_ReadWriteDirEntFile(
				info, 0, 0, tbn, 512);
			strncpy(info->de_name, tbn+8, sz-8);
			strcat(info->de_name, "/");
			strcat(info->de_name, cs);
			return(2);
		}
	}
	return(ino);
}

int TKDFS_ImageLookupInodePath(TKDFS_ImageContext *img,
	TKDFS_DirentInfo *info, char *path, int dfl)
{
	return(TKDFS_ImageLookupInodePathI(img, info, path, 0));
}

int TKDFS_ImageCreateInodePath(TKDFS_ImageContext *img,
	TKDFS_DirentInfo *info, char *path, int dfl)
{
	return(TKDFS_ImageLookupInodePathI(img, info, path, 1));
}

int TKDFS_ReadWriteImageInodeFileData(TKDFS_ImageContext *img,
	byte *data, int d_ino, s64 fd_offs, int fd_len, int dfl)
{
	TKDFS_InodeInfo *ino_inf;
	byte *blkptr, *ct;
	int blknum, blkofs, idblksh;
	s64 maxsize;
	int ofs1, len1, len2;
	
	ino_inf=TKDFS_GetImageCachedInode(img, d_ino, dfl&1);
//	blkofs=d_blk;

	if(!ino_inf->ino_file && (dfl&1))
	{
//		if((dfl&8) && (fd_len<128))
//			blkptr=TKDFS_GetImageCachedInodeBlock(img, d_ino, 0, 9);
//		else
//			blkptr=TKDFS_GetImageCachedInodeBlock(img, d_ino, 0, 1);
		blkptr=TKDFS_GetImageCachedInodeBlock(img, d_ino, 0, dfl);
		ino_inf=TKDFS_GetImageCachedInode(img, d_ino, dfl&1);
	}

	if(ino_inf->ino_idat)
	{
		len1=ino_inf->ino_file->dsize-fd_offs;
		if(fd_len<len1)
			len1=fd_len;
		
		if((dfl&1) && (len1!=fd_len))
		{
			if((fd_offs+fd_len)>ino_inf->ino_szidat)
			{
				/* need to convert to normal file, and try again */
				TKDFS_ConvertImageInlineToFileInode(img, ino_inf);
				if(ino_inf->ino_idat)
					return(0);
				len1=TKDFS_ReadWriteImageInodeFileData(
					img, data, d_ino, fd_offs, fd_len, dfl);
				return(len1);
			}else
			{
				/* adjust size */
				ino_inf->ino_file->dsize=fd_offs+fd_len;
				len1=fd_len;
			}
		}
		
		if(len1<=0)
			return(0);
		ofs1=fd_offs;
		if(dfl&1)
			{ memcpy(ino_inf->ino_idat+ofs1, data, len1); }
		else
			{ memcpy(data, ino_inf->ino_idat+ofs1, len1); }
		return(len1);
	}

	maxsize=ino_inf->ino_file->dsize-fd_offs;
	len1=fd_len;
	if(maxsize<len1)
		len1=maxsize;

	if((dfl&1) && (len1!=fd_len))
	{
		ino_inf->ino_file->dsize=fd_offs+fd_len;
		ino_inf->ino_file->csize=ino_inf->ino_file->dsize;
		len1=fd_len;
	}

	if(len1<=0)
		return(0);

	if(ino_inf->ino_file->cmpmode==0)
	{
		idblksh=img->ln2_blksz;
		blkofs=fd_offs&((1<<idblksh)-1);
		blknum=fd_offs>>idblksh;
	}else
	{
		idblksh=img->ln2_cblksz;
		blkofs=fd_offs&((1<<idblksh)-1);
		blknum=fd_offs>>idblksh;
	}

//	blkptr=TKDFS_GetImageCachedInodeBlock(img, d_ino, blknum, dfl&1);
	blkptr=TKDFS_GetImageCachedInodeBlock(img, d_ino, blknum, dfl);
	ct=data;

	len2=(1<<idblksh)-blkofs;
	if(len1<len2)
		len2=len1;
	while(len1>0)
	{
		if(dfl&1)
			{ memcpy(blkptr+blkofs, ct, len2); }
		else
			{ memcpy(ct, blkptr+blkofs, len2); }

		ct+=len2;
		len1-=len2;
		if(len1<=0)
			break;
		
		blkofs=0;
		blknum++;
		len2=1<<idblksh;
		if(len1<len2)
			len2=len1;
//		blkptr=TKDFS_GetImageCachedInodeBlock(img, d_ino, blknum, dfl&1);
		blkptr=TKDFS_GetImageCachedInodeBlock(img, d_ino, blknum, dfl);
	}

	len1=ct-data;
	return(len1);
}

int TKDFS_ReadImageInodeFileData(TKDFS_ImageContext *img,
	byte *data, int d_ino, s64 fd_offs, int fd_len)
{
	int sz;
	sz=TKDFS_ReadWriteImageInodeFileData(
		img, data, d_ino, fd_offs, fd_len, 0);
	return(sz);
}

int TKDFS_WriteImageInodeFileData(TKDFS_ImageContext *img,
	byte *data, int d_ino, s64 fd_offs, int fd_len)
{
	int sz;
	sz=TKDFS_ReadWriteImageInodeFileData(
		img, data, d_ino, fd_offs, fd_len, 1);
	return(sz);
}

int TKDFS_ReadWriteDirEntFile(
	TKDFS_DirentInfo *dee, s64 ofs, int dfl, byte *dbuf, s64 dsz)
{
	TKDFS_ImageContext *img;
	int sz;
	
	img=dee->img;
	sz=TKDFS_ReadWriteImageInodeFileData(
		img, dbuf, dee->de_ino, ofs, dsz, dfl);
	return(sz);
}

s64 TKDFS_GetDirEntSize(TKDFS_DirentInfo *dee)
{
	TKDFS_InodeInfo *ino_inf;
	
	ino_inf=TKDFS_GetImageCachedInode(dee->img, dee->de_ino, 0);
	if(!ino_inf->ino_file)
		return(0);
	return(ino_inf->ino_file->dsize);
}

int TKDFS_SetDirEntSize(TKDFS_DirentInfo *dee, s64 size)
{
	TKDFS_InodeInfo *ino_inf;
	
	ino_inf=TKDFS_GetImageCachedInode(dee->img, dee->de_ino, 1);
	if(!ino_inf->ino_file)
		return(0);
	ino_inf->ino_file->dsize=size;
	ino_inf->ino_file->csize=size;
	return(0);
}

s64 TKDFS_GetDirEntInode(TKDFS_DirentInfo *dee)
{
	return(dee->de_ino);
}

int TKDFS_SetDirEntInode(TKDFS_DirentInfo *dee, s64 d_ino)
{
	dee->de_ino=d_ino;
	dee->deb->ino=d_ino;
	return(0);
}

int TKDFS_ImageDestroyInode(TKDFS_ImageContext *img,
	int d_ino, int d_flg);

int TKDFS_DeleteInode(TKDFS_ImageContext *img, int d_ino)
{
	TKDFS_ImageDestroyInode(img, d_ino, 1);
	return(0);
}

int TKDFS_DeleteDirEnt(TKDFS_DirentInfo *dee)
{
	TKDFS_ImageRemoveDirentRebalance(dee->img, dee);
	return(0);
}

int TKDFS_UpdateDirEnt(TKDFS_DirentInfo *dee)
{
	if(!dee->img)
		return(0);
	TKDFS_WriteImageDirent(dee->img, dee, dee->di_ino, dee->di_idx);
	return(0);
}

int TKDFS_SyncDirEntFile(TKDFS_DirentInfo *dee)
{
	if(!dee->img)
		return(0);
	TKDFS_SyncImageInodes(dee->img);
	return(0);
}


int TKDFS_GetDirEntMode(TKDFS_DirentInfo *dee)
{
	TKDFS_InodeInfo *ino_inf;
	
	if(!dee->img)
		return(0);
	ino_inf=TKDFS_GetImageCachedInode(dee->img, dee->de_ino, 0);
	if(!ino_inf->ino_file)
		return(0);
	return(ino_inf->ino_file->mode);
}

int TKDFS_SetDirEntMode(TKDFS_DirentInfo *dee, int d_mode)
{
	TKDFS_InodeInfo *ino_inf;
	
	if(!dee->img)
		return(0);
	ino_inf=TKDFS_GetImageCachedInode(dee->img, dee->de_ino, 1);
	if(!ino_inf->ino_file)
		return(0);
	ino_inf->ino_file->mode=d_mode;
	return(0);
}

int TKDFS_GetDirEntUid(TKDFS_DirentInfo *dee)
{
	TKDFS_InodeInfo *ino_inf;
	
	ino_inf=TKDFS_GetImageCachedInode(dee->img, dee->de_ino, 0);
	if(!ino_inf->ino_file)
		return(0);
	return(ino_inf->ino_file->uid);
}

int TKDFS_GetDirEntGid(TKDFS_DirentInfo *dee)
{
	TKDFS_InodeInfo *ino_inf;
	
	ino_inf=TKDFS_GetImageCachedInode(dee->img, dee->de_ino, 0);
	if(!ino_inf->ino_file)
		return(0);
	return(ino_inf->ino_file->gid);
}

s64 TKDFS_GetDirEntCTime(TKDFS_DirentInfo *dee)
{
	TKDFS_InodeInfo *ino_inf;
	
	ino_inf=TKDFS_GetImageCachedInode(dee->img, dee->de_ino, 0);
	if(!ino_inf->ino_file)
		return(0);
	return(ino_inf->ino_file->ctime);
}

s64 TKDFS_GetDirEntMTime(TKDFS_DirentInfo *dee)
{
	TKDFS_InodeInfo *ino_inf;
	
	ino_inf=TKDFS_GetImageCachedInode(dee->img, dee->de_ino, 0);
	if(!ino_inf->ino_file)
		return(0);
	return(ino_inf->ino_file->mtime);
}

