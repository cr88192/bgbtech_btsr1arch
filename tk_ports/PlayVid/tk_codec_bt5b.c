static int bt5b_decompress_frame_clrs(BGBBTJ_VidCodecCTX *ctx,
	void *src, void *dst, int ssz, int dsz, int clrs)
{
	cram_ctxinfo *info;
	int err, i, j, k, ib, ob, clrfl;
	byte *sbuf, *tbuf;

	info=ctx->data;

	info->ihead->biSizeImage=ssz;
	info->ohead->biSizeImage=dsz;

//	printf("bt5b_decompress_frame_clrs: %p %p %d %d\n", src, dst, ssz, dsz);

//	clrfl=BTIC4B_CLRS_RGB555;

//	err=BTIC4B_DecodeImgBufferCtx(info->vctx, src, ssz, dst,
//		info->ihead->biWidth,
//		info->ihead->biHeight, clrfl);

	BTIC5B_DecodeFrame(info->vctx, src, ssz, dst, info->ihead->biWidth);

	return(0);
}

static int bt5b_decompress_frame(BGBBTJ_VidCodecCTX *ctx,
	void *src, void *dst, int ssz, int dsz)
{
	return(bt5b_decompress_frame_clrs(ctx,
		src, dst, ssz, dsz, BGBBTJ_JPG_BGRA));
}

static BGBBTJ_VidCodecCTX *bt5b_begin_decompress(int fcc,
	BGBBTJ_BITMAPINFOHEADER *in, BGBBTJ_BITMAPINFOHEADER *out)
{
	BGBBTJ_VidCodecCTX *ctx;
	BTIC5B_DecodeContext *btctx;
	cram_ctxinfo *info;
	int err;

	if(in->biCompression!=RIFF_TAG_bt5b)
		return(NULL);

	ctx=BGBBTJ_VidCodecCTX_New();
//	info=bgbrpi_malloc(sizeof(bt5b_ctxinfo));
	info=malloc(sizeof(cram_ctxinfo));
	memset(info, 0, sizeof(cram_ctxinfo));
	ctx->data=info;

	btctx=BTIC5B_AllocDecodeContext();
	info->vctx=btctx;

	info->ihead=malloc(sizeof(BGBBTJ_BITMAPINFOHEADER));
	memset(info->ihead, 0, sizeof(BGBBTJ_BITMAPINFOHEADER));
	info->ihead->biSize		= sizeof(BGBBTJ_BITMAPINFOHEADER);
	info->ihead->biWidth		= in->biWidth;
	info->ihead->biHeight		= in->biHeight;
	info->ihead->biPlanes		= in->biPlanes;
	info->ihead->biBitCount		= in->biBitCount;
	info->ihead->biCompression	= in->biCompression;
	info->ihead->biSizeImage	=
		in->biWidth*in->biHeight*in->biBitCount/8;

	info->ohead=malloc(sizeof(BGBBTJ_BITMAPINFOHEADER));
	memset(info->ohead, 0, sizeof(BGBBTJ_BITMAPINFOHEADER));
	info->ohead->biSize		= sizeof(BGBBTJ_BITMAPINFOHEADER);
	info->ohead->biWidth		= out->biWidth;
//	info->ohead->biHeight		= -out->biHeight;
	info->ohead->biHeight		= out->biHeight;
	info->ohead->biPlanes		= out->biPlanes;
	info->ohead->biBitCount		= out->biBitCount;
	info->ohead->biCompression	= out->biCompression;
	info->ihead->biSizeImage	=
		out->biWidth*out->biHeight*out->biBitCount/8;

	info->buffer=malloc(out->biWidth*out->biHeight*out->biBitCount/8);

	ctx->decompress_frame=&bt5b_decompress_frame;
	ctx->decompress_frame_clrs=&bt5b_decompress_frame_clrs;

	return(ctx);
}

static void bt5b_shutdown()
{
	int i;
}

int BGBBTJ_CodecBT5B_Init()
{
	BGBBTJ_VidCodec *codec;
	int i, j;
	short *sw;
	char buf[256];
	char *t;

	printf("BT5B: Init\n");

	codec=BGBBTJ_VidCodec_New();
	codec->begin_decompress=&bt5b_begin_decompress;

	return(0);
}
