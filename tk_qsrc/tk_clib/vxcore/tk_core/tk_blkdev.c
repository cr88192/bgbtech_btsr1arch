/*
 * Low Level Block Device
 * Access is via ID number and LBA.
 * Will nominally assume a 512 byte sector size.
 */

typedef struct TK_BLKDEV_VT_s TK_BLKDEV_VT;
typedef struct TK_BLKDEV_CTX_s TK_BLKDEV_CTX;

struct TK_BLKDEV_VT_s {
char *name;
void *resv1;
void *resv2;
void *resv3;

int (*ReadSectors)(TK_BLKDEV_CTX *ctx, byte *buf, s64 lba, int cnt);
int (*WriteSectors)(TK_BLKDEV_CTX *ctx, byte *buf, s64 lba, int cnt);
};

struct TK_BLKDEV_CTX_s {
TK_BLKDEV_VT *vt;
int bdev;	//current block device
int pbdev;	//parent block device
void *udata_p0;
void *udata_p1;
void *udata_p2;
void *udata_p3;
s64 udata_i0;
s64 udata_i1;
s64 udata_i2;
s64 udata_i3;

s64 lba_base;
s64 lbn_base;
};

TK_BLKDEV_CTX *tk_blkdev_arr[256];
int tk_n_blkdev;

int TKBDEV_ReadSectors(int bdev, byte *buf, s64 lba, int cnt)
{
	TK_BLKDEV_CTX *ctx;
	if((bdev<0) || (bdev>=tk_n_blkdev))
		return(-1);
	ctx=tk_blkdev_arr[bdev];
	if(!ctx || !ctx->vt)
		return(-1);
	return(ctx->vt->ReadSectors(ctx, buf, lba, cnt));
}

int TKBDEV_WriteSectors(int bdev, byte *buf, s64 lba, int cnt)
{
	TK_BLKDEV_CTX *ctx;
	if((bdev<0) || (bdev>=tk_n_blkdev))
		return(-1);
	ctx=tk_blkdev_arr[bdev];
	if(!ctx || !ctx->vt)
		return(-1);
	return(ctx->vt->WriteSectors(ctx, buf, lba, cnt));
}

s64 TKBDEV_GetSectorCount(int bdev)
{
	TK_BLKDEV_CTX *ctx;
	if((bdev<0) || (bdev>=tk_n_blkdev))
		return(-1);
	ctx=tk_blkdev_arr[bdev];
	if(!ctx || !ctx->vt)
		return(-1);
	return(ctx->lbn_base);
}

int TKBDEV_ReleaseDevice(int bdev)
{
}

TK_BLKDEV_CTX *TKBDEV_AllocNewDevice(void)
{
	TK_BLKDEV_CTX *ctx;
	int i;
	
	for(i=0; i<tk_n_blkdev; i++)
	{
		ctx=tk_blkdev_arr[i];
		if(!ctx)
		{
			ctx=tk_malloc(sizeof(TK_BLKDEV_CTX));
			memset(ctx, 0, sizeof(TK_BLKDEV_CTX));
			ctx->bdev=i;
			tk_blkdev_arr[i]=ctx;
			return(ctx);
		}
	}
	
	i=tk_n_blkdev++;
	ctx=tk_malloc(sizeof(TK_BLKDEV_CTX));
	memset(ctx, 0, sizeof(TK_BLKDEV_CTX));
	ctx->bdev=i;
	tk_blkdev_arr[i]=ctx;
	return(ctx);
}

int TKBDEV_subdev_ReadSectors(TK_BLKDEV_CTX *ctx, byte *buf, s64 lba, int cnt)
{
	return(TKBDEV_ReadSectors(ctx->pbdev, buf, lba+ctx->udata_i0, cnt));
}

int TKBDEV_subdev_WriteSectors(TK_BLKDEV_CTX *ctx, byte *buf, s64 lba, int cnt)
{
	return(TKBDEV_WriteSectors(ctx->pbdev, buf, lba+ctx->udata_i0, cnt));
}

TK_BLKDEV_VT tkbdev_subdev_vt = {
"subdev",
NULL,
NULL,
NULL,
TKBDEV_subdev_ReadSectors,
TKBDEV_subdev_WriteSectors
};

TK_BLKDEV_CTX *TKBDEV_CreateSubDevice(int hdev, s64 lba, s64 cnt)
{
	TK_BLKDEV_CTX *bdctx;

	bdctx=TKBDEV_AllocNewDevice();
	bdctx->vt=&tkbdev_subdev_vt;
	bdctx->pbdev=hdev;
	bdctx->udata_i0=lba;
	bdctx->udata_i1=cnt;
	return(bdctx);
}

int TKBDEV_CreateSubDeviceId(int hdev, s64 lba, s64 cnt)
{
	TK_BLKDEV_CTX *bdctx;

	bdctx=TKBDEV_CreateSubDevice(hdev, lba, cnt);
	return(bdctx->bdev);
}

int TKBDEV_OpenPathAsDeviceId(char *devfn)
{
	s64 val;
	
	val=-1;
	tk_fsctl(devfn, TK_IOC_VFS_OPENBLKDEV, &val);
	if(val>0)
		return(val);
	return(0);
}
