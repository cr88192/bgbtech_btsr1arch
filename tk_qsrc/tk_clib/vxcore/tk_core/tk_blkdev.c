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
int bdev;
void *udata_p0;
void *udata_p1;
void *udata_p2;
void *udata_p3;
s64 udata_i0;
s64 udata_i1;
s64 udata_i2;
s64 udata_i3;
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
