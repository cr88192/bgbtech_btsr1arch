#define TK_FILE		FILE
#define tk_fopen	fopen
#define tk_fread	fread
#define tk_fgetc	fgetc
#define tk_ftell	ftell
#define tk_fseek	fseek

#define BGBBTJ_JPG_RGBA		0
#define BGBBTJ_JPG_BGRA		1
#define BGBBTJ_JPG_RGB555	2

#define BGBBTJ_JPG_RAWCON 64

BGBBTJ_VidCodec *lbxgl_codecs=NULL;

BGBBTJ_VidCodec *BGBBTJ_VidCodec_New()
{
	BGBBTJ_VidCodec *tmp;

//	tmp=bgbrpi_talloc("lbxgl_vidcodec_t", sizeof(BGBBTJ_VidCodec));
	tmp=malloc(sizeof(BGBBTJ_VidCodec)+256);
	memset(tmp, 0, sizeof(BGBBTJ_VidCodec));
	tmp->next=lbxgl_codecs;
	lbxgl_codecs=tmp;

	return(tmp);
}

BGBBTJ_VidCodecCTX *BGBBTJ_VidCodecCTX_New()
{
	BGBBTJ_VidCodecCTX *tmp;

//	tmp=bgbrpi_talloc("lbxgl_vidcodecctx_t", sizeof(BGBBTJ_VidCodecCTX));
	tmp=malloc(sizeof(BGBBTJ_VidCodecCTX)+256);
	memset(tmp, 0, sizeof(BGBBTJ_VidCodecCTX));
	return(tmp);
}

void BGBBTJ_VidCodecCTX_Destroy(BGBBTJ_VidCodecCTX *ctx)
{
}

int BGBBTJ_Codec_DecompressQuery(int fcc,
	BGBBTJ_BITMAPINFOHEADER *in, BGBBTJ_BITMAPINFOHEADER *out)
{
	BGBBTJ_VidCodec *cur;
	int i;

	BGBBTJ_Codec_Init();

	cur=lbxgl_codecs;
	while(cur)
	{
		if(cur->decompress_query)
		{
			i=cur->decompress_query(fcc, in, out);
			if(i>0)return(i);
		}
		cur=cur->next;
	}
	return(0);
}

int BGBBTJ_Codec_CompressQuery(int fcc,
	BGBBTJ_BITMAPINFOHEADER *in, BGBBTJ_BITMAPINFOHEADER *out)
{
	BGBBTJ_VidCodec *cur;
	int i;

	BGBBTJ_Codec_Init();

	cur=lbxgl_codecs;
	while(cur)
	{
		if(cur->compress_query)
		{
			i=cur->compress_query(fcc, in, out);
			if(i>0)return(i);
		}
		cur=cur->next;
	}
	return(0);
}

BGBBTJ_VidCodecCTX *BGBBTJ_Codec_BeginDecompress(int fcc,
	BGBBTJ_BITMAPINFOHEADER *in, BGBBTJ_BITMAPINFOHEADER *out)
{
	BGBBTJ_VidCodec *cur;
	BGBBTJ_VidCodecCTX *tmp;
	int *cct;

	BGBBTJ_Codec_Init();

	cur=lbxgl_codecs;
	while(cur)
	{
		if(cur->fcc)
		{
			cct=cur->fcc;
			while(*cct)
			{
				if(fcc==*cct)break;
				cct++;
			}
			if(*cct && cur->begin_decompress)
			{
				tmp=cur->begin_decompress(fcc, in, out);
				if(tmp)return(tmp);
			}
		}
		cur=cur->next;
	}

	cur=lbxgl_codecs;
	while(cur)
	{
		if(!cur->fcc && cur->begin_decompress)
		{
			tmp=cur->begin_decompress(fcc, in, out);
			if(tmp)return(tmp);
		}
		cur=cur->next;
	}

	printf("Codec: Decode Fail Codec %.4s\n", (char *)(&fcc));

	return(NULL);
}

BGBBTJ_VidCodecCTX *BGBBTJ_Codec_BeginCompress(int fcc,
	BGBBTJ_BITMAPINFOHEADER *in, BGBBTJ_BITMAPINFOHEADER *out)
{
	BGBBTJ_VidCodec *cur;
	BGBBTJ_VidCodecCTX *tmp;
	int *cct;

	BGBBTJ_Codec_Init();

	cur=lbxgl_codecs;
	while(cur)
	{
		if(cur->fcc)
		{
			cct=cur->fcc;
			while(*cct)
			{
				if(fcc==*cct)break;
				cct++;
			}
			if(*cct && cur->begin_compress)
			{
				tmp=cur->begin_compress(fcc, in, out);
				if(tmp)return(tmp);
			}
		}
		cur=cur->next;
	}

	cur=lbxgl_codecs;
	while(cur)
	{
		if(!cur->fcc && cur->begin_compress)
		{
			tmp=cur->begin_compress(fcc, in, out);
			if(tmp)return(tmp);
		}
		cur=cur->next;
	}

	printf("Codec: Encode Fail Codec %.4s/%.4s \n",
		(char *)(&fcc), (char *)(&out->biCompression));
	return(NULL);
}

//default: base at upper left corner, rgba ordering, a=255
int BGBBTJ_Codec_DecompressFrame(BGBBTJ_VidCodecCTX *ctx,
	void *src, void *dst, int ssz, int dsz)
{
	if(!ctx->decompress_frame)return(-1);
	return(ctx->decompress_frame(ctx, src, dst, ssz, dsz));
}

int BGBBTJ_Codec_DecompressFrameClrs(BGBBTJ_VidCodecCTX *ctx,
	void *src, void *dst, int ssz, int dsz, int clrs)
{
	if(!ctx->decompress_frame_clrs)return(-1);
	return(ctx->decompress_frame_clrs(ctx, src, dst, ssz, dsz, clrs));
}

int BGBBTJ_Codec_DecompressFrame2(BGBBTJ_VidCodecCTX *ctx,
	void *src, void *dst, int ssz, int dsz, int clrs)
{
//	printf("BGBBTJ_Codec_DecompressFrame2: Src p=%p sz=%d\n", src, ssz);

//	return(-1);

	if(ctx->decompress_frame_clrs)
	{
		return(ctx->decompress_frame_clrs(ctx,
			src, dst, ssz, dsz, clrs));
	}

	if(ctx->decompress_frame)
	{
		return(ctx->decompress_frame(ctx,
			src, dst, ssz, dsz));
	}

	return(-1);
}

//default: base at upper left corner, rgba ordering, a=255
int BGBBTJ_Codec_CompressFrame(BGBBTJ_VidCodecCTX *ctx,
	void *src, void *dst, int dsz, int qfl, int clrs, int *rfl)
{
	if(!ctx->compress_frame)return(-1);
	return(ctx->compress_frame(ctx, src, dst, dsz, qfl, clrs, rfl));
}

int BGBBTJ_VidCodecCTX_EndCompress(BGBBTJ_VidCodecCTX *ctx)
{
	if(!ctx->end_compress)return(-1);
	return(ctx->end_compress(ctx));
}

int BGBBTJ_VidCodecCTX_EndDecompress(BGBBTJ_VidCodecCTX *ctx)
{
	if(!ctx->end_decompress)return(-1);
	return(ctx->end_decompress(ctx));
}

#include "tk_codec_cram.c"
#include "tk_codec_bt4b.c"
#include "tk_codec_bt5a.c"

int BGBBTJ_Codec_Init()
{
	static int init;

	if(init)return(0);
	init=1;

//	BGBBTJ_CodecVFW_Init();

	BGBBTJ_CodecCRAM_Init();
	BGBBTJ_CodecBT4B_Init();
	BGBBTJ_CodecBT5A_Init();

	return(1);
}

void BGBBTJ_ConvBGR2RGB(byte *sbuf, byte *tbuf, int sz)
{
	int i;
	
	for(i=0; i<sz; i++)
	{
		tbuf[i*3+0]=sbuf[i*3+2];
		tbuf[i*3+1]=sbuf[i*3+1];
		tbuf[i*3+2]=sbuf[i*3+0];
	}
}

void BGBBTJ_ConvBGRA2RGBA(byte *sbuf, byte *tbuf, int sz)
{
	int i;
	
	for(i=0; i<sz; i++)
	{
		tbuf[i*4+0]=sbuf[i*4+2];
		tbuf[i*4+1]=sbuf[i*4+1];
		tbuf[i*4+2]=sbuf[i*4+0];
		tbuf[i*4+3]=255;
	}
}

static int vfw_decompress_frame_clrs(BGBBTJ_VidCodecCTX *ctx,
	void *src, void *dst, int ssz, int dsz, int clrs)
{
	vfw_ctxinfo *info;
	int err, i, j, k, ib, ob;
	byte *sbuf, *tbuf;

	info=ctx->data;

	info->ihead->biSizeImage=ssz;
	info->ohead->biSizeImage=dsz;

//	memset(dst, 255, info->ohead->biWidth*info->ohead->biHeight*4);

#if 0
	i=(!dst)?ICDECOMPRESS_PREROLL:0;	//frame skip
//	i=ICDECOMPRESS_NOTKEYFRAME;
//	err=ICDecompress(info->hicd, 0, info->ihead, src, info->ohead, dst);
	if(dst && (clrs==BGBBTJ_JPG_BGRA))
	{
		err=ICDecompress(info->hicd, i,
			(BITMAPINFOHEADER *)info->ihead, src,
			(BITMAPINFOHEADER *)info->ohead, dst);
	}else
	{
		err=ICDecompress(info->hicd, i,
			(BITMAPINFOHEADER *)info->ihead, src,
			(BITMAPINFOHEADER *)info->ohead, info->buffer);
	}
#endif

	if(dst)
	{
		sbuf=info->buffer;
		tbuf=dst;

		j=info->ohead->biHeight;
		if(j<0)j=-j;
		j=info->ohead->biWidth*j;
		k=info->ohead->biBitCount/8;

		if(clrs==BGBBTJ_JPG_RGBA)
		{
			if(k==3)BGBBTJ_ConvBGR2RGB(sbuf, tbuf, j);
			if(k==4)BGBBTJ_ConvBGRA2RGBA(sbuf, tbuf, j);
		}

//		if(clrs==BGBBTJ_JPG_BGRA)
//		{
//			if(k==4) { memcpy(tbuf, sbuf, j*4); }
//		}
	}

//	if(err!=ICERR_OK)printf("VFW: decompress %d\n", err);
//	if(err!=ICERR_OK)return(-1);
	return(0);
}

static int vfw_decompress_frame(BGBBTJ_VidCodecCTX *ctx,
	void *src, void *dst, int ssz, int dsz)
{
	return(vfw_decompress_frame_clrs(ctx,
		src, dst, ssz, dsz, BGBBTJ_JPG_BGRA));
}

static BGBBTJ_VidCodecCTX *vfw_begin_decompress(int fcc,
	BGBBTJ_BITMAPINFOHEADER *in, BGBBTJ_BITMAPINFOHEADER *out)
{
	BGBBTJ_VidCodecCTX *ctx;
	vfw_ctxinfo *info;
	int err;

	ctx=BGBBTJ_VidCodecCTX_New();
//	info=bgbrpi_malloc(sizeof(vfw_ctxinfo));
	info=malloc(sizeof(vfw_ctxinfo));
	memset(info, 0, sizeof(vfw_ctxinfo));
	ctx->data=info;

	info->ihead=malloc(sizeof(BGBBTJ_BITMAPINFOHEADER)+256);
	memset(info->ihead, 0, sizeof(BGBBTJ_BITMAPINFOHEADER));
	info->ihead->biSize			= sizeof(BGBBTJ_BITMAPINFOHEADER);
	info->ihead->biWidth		= in->biWidth;
	info->ihead->biHeight		= in->biHeight;
	info->ihead->biPlanes		= in->biPlanes;
	info->ihead->biBitCount		= in->biBitCount;
	info->ihead->biCompression	= in->biCompression;
	info->ihead->biSizeImage	=
		in->biWidth*in->biHeight*in->biBitCount/8;

	info->ohead=malloc(sizeof(BGBBTJ_BITMAPINFOHEADER)+256);
	memset(info->ohead, 0, sizeof(BGBBTJ_BITMAPINFOHEADER));
	info->ohead->biSize			= sizeof(BGBBTJ_BITMAPINFOHEADER);
	info->ohead->biWidth		= out->biWidth;
//	info->ohead->biHeight		= -out->biHeight;
	info->ohead->biHeight		= out->biHeight;
	info->ohead->biPlanes		= out->biPlanes;
	info->ohead->biBitCount		= out->biBitCount;
	info->ohead->biCompression	= out->biCompression;
	info->ihead->biSizeImage	=
		out->biWidth*out->biHeight*out->biBitCount/8;

//	info->buffer=malloc(out->biWidth*out->biHeight*out->biBitCount/8);
	info->buffer=malloc((out->biWidth+3)*(out->biHeight+3)*out->biBitCount/8);

//	info->hicd = ICOpen(ICTYPE_VIDEO, fcc, ICMODE_FASTDECOMPRESS);
//	info->hicd = ICLocate(ICTYPE_VIDEO, fcc,
//		(BITMAPINFOHEADER *)info->ihead,
//		(BITMAPINFOHEADER *)info->ohead,
//		ICMODE_FASTDECOMPRESS);
//	if(!info->hicd)
//	{
//		printf("VFW: decompress %0.4s %0.4s: fail\n",
//			&fcc, &in->biCompression);
//		return(NULL);
//	}
//	printf("VFW: decompress %0.4s %0.4s: ok\n", &fcc, &in->biCompression);


//	err=ICDecompressBegin(info->hicd,
//		(BITMAPINFOHEADER *)info->ihead,
//		(BITMAPINFOHEADER *)info->ohead);
//	if(err)
//	{
//		printf("VFW: err %d, %0.4s %0.4s: fail\n", err,
//			&fcc, &in->biCompression);
//		return(NULL);
//	}

//	vfw_hicd[vfw_n_hicd++]=info->hicd;

	ctx->decompress_frame=&vfw_decompress_frame;
	ctx->decompress_frame_clrs=&vfw_decompress_frame_clrs;

	return(ctx);
}

static void vfw_shutdown()
{
	int i;
	
//	for(i=0; i<vfw_n_hicd; i++)
//		ICDecompressEnd(vfw_hicd[i]);
}

int BGBBTJ_CodecVFW_Init()
{
	BGBBTJ_VidCodec *codec;
//	ICINFO *info;
	int i, j;
	short *sw;
	char buf[256];
	char *t;

	printf("VFW: Init\n");

	codec=BGBBTJ_VidCodec_New();
	codec->begin_decompress=&vfw_begin_decompress;

//	PDGL_RegisterShutdown(&vfw_shutdown);

#if 0
	info=malloc(sizeof(ICINFO));
	memset(info, 0, sizeof(ICINFO));

	for(i=0; i<1024; i++)
	{
		j=ICInfo(0, i, info);
		if(j!=1)break;
		printf("fccType: %04.4s\n", &info->fccType);
		printf("fccHandler: %04.4s\n", &info->fccHandler);
		if(info->szName[0])
			printf("szName: %S\n", info->szName);
		if(info->szDescription[0])
			printf("szDescription: %S\n", info->szDescription);
	}
#endif

	return(0);
}

int RIFF_ReadInt32(TK_FILE *fd)
{
	int i;

	i=tk_fgetc(fd);
	i+=tk_fgetc(fd)<<8;
	i+=tk_fgetc(fd)<<16;
	i+=tk_fgetc(fd)<<24;

	return(i);
}

int RIFF_ReadChunkInfo(TK_FILE *fd, int ofs, int *id, int *sz, int *lid)
{
	tk_fseek(fd, ofs, 0);

	if(id)*id=RIFF_ReadInt32(fd);
	if(sz)*sz=RIFF_ReadInt32(fd);
	if(lid)*lid=RIFF_ReadInt32(fd);

	return(0);
}

int RIFF_NextChunk(TK_FILE *fd, int ofs)
{
	int cc1, cc2, sz;

	RIFF_ReadChunkInfo(fd, ofs, &cc1, &sz, &cc2);
	if(sz&1)sz++;

	return(ofs+sz+8);
}

int RIFF_FindSubChunk(TK_FILE *fd, int ofs, int id, int lid)
{
	int cc1, cc2, sz;
	int end, cur;

	RIFF_ReadChunkInfo(fd, ofs, &cc1, &sz, &cc2);

	end=ofs+sz+8;
	cur=ofs+12;
	while(cur<end)
	{
		RIFF_ReadChunkInfo(fd, cur, &cc1, &sz, &cc2);
		if(cc1==id)if(!lid || (cc2==lid))
			return(cur);
		cur=RIFF_NextChunk(fd, cur);
	}
	return(-1);
}

void *RIFF_ReadInChunk(TK_FILE *fd, int ofs, int *size)
{
	int cc1, cc2, sz;
	void *buf;

	RIFF_ReadChunkInfo(fd, ofs, &cc1, &sz, &cc2);
	if(size)*size=sz;

	buf=malloc(sz+1024);
	tk_fseek(fd, ofs+8, 0);
	tk_fread(buf, 1, sz+1024, fd);

	return(buf);
}

void *RIFF_FindReadInChunk(TK_FILE *fd, int ofs, int id, int *size)
{
	int ch_ofs;
	void *p;

	ch_ofs=RIFF_FindSubChunk(fd, ofs, id, 0);
	if(ch_ofs<0)
	{
		printf("Missing %04s\n", (char *)(&id));
		return(NULL);
	}

	p=RIFF_ReadInChunk(fd, ch_ofs, size);
	return(p);
}


int RIFF_ReadInChunkTBuf(TK_FILE *fd, int ofs, void **rbuf, int *rbsz)
{
	int cc1, cc2, sz, sz1;
	void *buf;

	RIFF_ReadChunkInfo(fd, ofs, &cc1, &sz, &cc2);
//	if(size)*size=sz;

//	buf=malloc(sz+1024);
	buf=*rbuf;
	sz1=*rbsz;

	if(buf && ((sz+256)>sz1))
	{
		printf("RIFF_ReadInChunkTBuf: Free %p %d\n", buf, sz1);
		free(buf); buf=NULL;
	}
	
	if(!buf)
	{
//		sz1=sz+4096;
		sz1=sz+32768;
		
		if(sz1<65536)
			sz1=65536;

		printf("RIFF_ReadInChunkTBuf: Malloc %d\n", sz1);

		buf=malloc(sz1);
		*rbuf=buf;
		*rbsz=sz1;

		printf("RIFF_ReadInChunkTBuf: Malloc OK %p %d\n", buf, sz1);
	}

	tk_fseek(fd, ofs+8, 0);
	tk_fread(buf, 1, sz, fd);
//	tk_fread(buf, 1, sz+256, fd);

//	printf("RIFF_ReadInChunkTBuf: Lump %p %0.4s %d\n",
//		buf, (char *)(&cc1), sz);

	return(sz);
}

int BGBBTJ_AVI_DumpMainAVIHeader(BGBBTJ_MainAVIHeader *avihead)
{
#if 1
	printf("MainAVIHeader:\n");
	printf("dwMicroSecPerFrame=%d\n", avihead->dwMicroSecPerFrame);
	printf("dwMaxBytesPerSec=%d\n", avihead->dwMaxBytesPerSec);
	printf("dwPaddingGranularity=%d\n", avihead->dwPaddingGranularity);
	printf("dwFlags=%d\n", avihead->dwFlags);
	printf("dwTotalFrames=%d\n", avihead->dwTotalFrames);
	printf("dwInitialFrames=%d\n", avihead->dwInitialFrames);
	printf("dwStreams=%d\n", avihead->dwStreams);
	printf("dwSuggestedBufferSize=%d\n", avihead->dwSuggestedBufferSize);
	printf("dwWidth=%d\n", avihead->dwWidth);
	printf("dwHeight=%d\n", avihead->dwHeight);
	printf("\n");
#endif
	return(0);
}

int BGBBTJ_AVI_DumpAVIStreamHeader(BGBBTJ_AVIStreamHeader *strhead, int num)
{
#if 1
	printf("AVIStreamHeader(%d):\n", num);

	printf("fccType=%0.4s\n", (char *)(&strhead->fccType));
	printf("fccHandler=%0.4s\n", (char *)(&strhead->fccHandler));
	printf("dwFlags=%d\n", strhead->dwFlags);
	printf("wPriority=%d\n", strhead->wPriority);
	printf("wLanguage=%d\n", strhead->wLanguage);
	printf("dwInitialFrames=%d\n", strhead->dwInitialFrames);
	printf("dwScale=%d\n", strhead->dwScale);
	printf("dwRate=%d\n", strhead->dwRate);
	printf("dwStart=%d\n", strhead->dwStart);
	printf("dwLength=%d\n", strhead->dwLength);
	printf("dwSuggestedBufferSize=%d\n", strhead->dwSuggestedBufferSize);
	printf("dwQuality=%d\n", strhead->dwQuality);
	printf("dwSampleSize=%d\n", strhead->dwSampleSize);
	printf("rcFrame=left:%d top:%d right:%d bottom:%d\n",
		strhead->rcFrame.left, strhead->rcFrame.top,
		strhead->rcFrame.right, strhead->rcFrame.bottom);
	printf("\n");
#endif
	return(0);
}

int BGBBTJ_AVI_DumpAVIWaveFmt(BGBBTJ_WAVEFORMATEX *wavefmt, int num)
{
#if 1
	printf("WAVEFORMATEX(%d):\n", num);
	printf("wFormatTag=%d\n", wavefmt->wFormatTag);
	printf("nChannels=%d\n", wavefmt->nChannels);
	printf("nSamplesPerSec=%d\n", wavefmt->nSamplesPerSec);
	printf("nAvgBytesPerSec=%d\n", wavefmt->nAvgBytesPerSec);
	printf("nBlockAlign=%d\n", wavefmt->nBlockAlign);
	printf("wBitsPerSample=%d\n", wavefmt->wBitsPerSample);
	printf("cbSize=%d\n", wavefmt->cbSize);
	printf("\n");
#endif
	return(0);
}

int BGBBTJ_AVI_DumpBMIHead(BGBBTJ_BITMAPINFOHEADER *bmihead, int num)
{
#if 1
	printf("BITMAPINFOHEADER(%d):\n", num);
	printf("biSize=%d\n", bmihead->biSize);
	printf("biWidth=%d\n", bmihead->biWidth);
	printf("biHeight=%d\n", bmihead->biHeight);
	printf("biPlanes=%d\n", bmihead->biPlanes);
	printf("biBitCount=%d\n", bmihead->biBitCount);
	printf("biCompression=%0.4s\n", (char *)(&bmihead->biCompression));
	printf("biSizeImage=%d\n", bmihead->biSizeImage);
	printf("biXPelsPerMeter=%d\n", bmihead->biXPelsPerMeter);
	printf("biYPelsPerMeter=%d\n", bmihead->biYPelsPerMeter);
	printf("biClrUsed=%d\n", bmihead->biClrUsed);
	printf("biClrImportant=%d\n", bmihead->biClrImportant);
	printf("\n");
#endif
	return(0);
}

int BGBBTJ_AVI_ExtractTagNum(int tag)
{
	int i, c;

	i=0;

	c=tag&0xff;
	if((c>='0') && (c<='9'))i+=c-'0';
	if((c>='A') && (c<='Z'))i+=c-'A'+10;
	if((c>='a') && (c<='z'))i+=c-'a'+10;

	i<<=4;

	c=(tag>>8)&0xff;
	if((c>='0') && (c<='9'))i+=c-'0';
	if((c>='A') && (c<='Z'))i+=c-'A'+10;
	if((c>='a') && (c<='z'))i+=c-'a'+10;

	return(i);
}

int BGBBTJ_AVI_Init()
{
	BGBBTJ_Codec_Init();
	return(0);
}

int avi_getsamples(short *samples, int cnt, void *data)
{
	return(BGBBTJ_AVI_GetMonoSamplesRate(data, samples, cnt, 44100));
}

int BGBBTJ_AVI_GetMonoSamplesNoRate(BGBBTJ_AVI_Context *ctx,
	short *samples, int cnt)
{
	int i, i2, j, rl, l, sr;
	short *ss, *st;

	sr=ctx->wavefmt->nSamplesPerSec;

	l=(ctx->esampbuf-ctx->sampbuf)/2;
	if(cnt<l)l=cnt;

	for(i=0; i<l; i++)
	{
		j=ctx->sampbuf[i*2];
		samples[i]=j;
	}

	i2=cnt;
	rl=(ctx->esampbuf-ctx->sampbuf)/2;
	if(i2<rl)rl=i2;

	ss=ctx->sampbuf+(rl*2);
	st=ctx->sampbuf;

	while(ss<ctx->esampbuf)*st++=*ss++;
	ctx->esampbuf=st;

	return(l);
}

int BGBBTJ_AVI_GetMonoSamplesRate(BGBBTJ_AVI_Context *ctx,
	short *samples, int cnt, int rate)
{
	int i, i2, j, rl, l, sr;
	short *ss, *st;

	sr=ctx->wavefmt->nSamplesPerSec;

	if(rate==sr)
		{ return(BGBBTJ_AVI_GetMonoSamplesNoRate(ctx, samples, cnt)); }

	l=(ctx->esampbuf-ctx->sampbuf)/2;
//	l*=44100.0/48000.0;
	l*=((double)rate)/sr;
	if(cnt<l)l=cnt;

//	printf("avi: mix %d samples, have %d\n", cnt, l);

	if(l<cnt)
	{
//		printf("avi: sample drain\n");
//		ctx->drain+=(cnt-l);
	}

	for(i=0; i<l; i++)
	{
		i2=i;
//		i2*=(48000.0/44100.0);
//		i2*=(sr/44100.0);
		i2*=(sr/((double)rate));

		j=ctx->sampbuf[i2*2];
//		j=(ctx->sampbuf[i2*2]+ctx->sampbuf[(i2*2)+1])/2;
//		if(j<-32767)j=-32767;
//		if(j>32767)j=32767;
		samples[i]=j;
	}


	i2=cnt;
//	i2*=(48000.0/44100.0);
	i2*=(sr/((double)rate));
	rl=(ctx->esampbuf-ctx->sampbuf)/2;
	if(i2<rl)rl=i2;

	ss=ctx->sampbuf+(rl*2);
	st=ctx->sampbuf;

	while(ss<ctx->esampbuf)*st++=*ss++;
	ctx->esampbuf=st;

	return(l);
}

BGBBTJ_AVI_Context *BGBBTJ_AVI_LoadAVI(char *name)
{
	int cc1, cc2, sz;
	int i, j, w, h;
	BGBBTJ_AVI_Context *ctx;
	BGBBTJ_BITMAPINFOHEADER *inhead, *outhead;

//	ctx=bgbrpi_talloc("lbxgl_avi_t", sizeof(BGBBTJ_AVI_Context));
	ctx=malloc(sizeof(BGBBTJ_AVI_Context));
	memset(ctx, 0, sizeof(BGBBTJ_AVI_Context));

	ctx->fd=tk_fopen(name, "rb");
	if(!ctx->fd)
	{
		printf("BGBBTJ_AVI_LoadAVI: Fail Open\n");
		return(NULL);
	}

	RIFF_ReadChunkInfo(ctx->fd, 0, &cc1, &sz, &cc2);
	if((cc1!=RIFF_TAG_RIFF) || (cc2!=RIFF_TAG_AVI))
	{
		printf("AVI: Not AVI\n");
		return(NULL);
	}

	ctx->ofs_hdrl=RIFF_FindSubChunk(ctx->fd, 0,
		RIFF_TAG_LIST, RIFF_TAG_hdrl);
	ctx->avihead=RIFF_FindReadInChunk(ctx->fd, ctx->ofs_hdrl,
		RIFF_TAG_avih, NULL);

	BGBBTJ_AVI_DumpMainAVIHeader(ctx->avihead);

	ctx->ofs_cur=ctx->ofs_hdrl+12;
	ctx->ofs_cur=RIFF_NextChunk(ctx->fd, ctx->ofs_cur);	//avih

	ctx->str_vid=-1;
	ctx->str_aud=-1;
	for(i=0; i<ctx->avihead->dwStreams; i++)
	{
		ctx->strhead=RIFF_FindReadInChunk(ctx->fd, ctx->ofs_cur,
			RIFF_TAG_strh, NULL);
		BGBBTJ_AVI_DumpAVIStreamHeader(ctx->strhead, i);

		if(ctx->strhead->fccType==RIFF_TAG_auds)
		{
			ctx->wavefmt=RIFF_FindReadInChunk(ctx->fd,
				ctx->ofs_cur, RIFF_TAG_strf, NULL);
			BGBBTJ_AVI_DumpAVIWaveFmt(ctx->wavefmt, i);
			ctx->audhead=ctx->strhead;
			ctx->str_aud=i;
		}

		if(ctx->strhead->fccType==RIFF_TAG_vids)
		{
			ctx->bmihead=RIFF_FindReadInChunk(ctx->fd,
				ctx->ofs_cur, RIFF_TAG_strf, NULL);
			BGBBTJ_AVI_DumpBMIHead(ctx->bmihead, i);
			ctx->vidhead=ctx->strhead;
			ctx->str_vid=i;
		}

		ctx->ofs_cur=RIFF_NextChunk(ctx->fd, ctx->ofs_cur);	//strl
	}

	ctx->ofs_movi=RIFF_FindSubChunk(ctx->fd, 0, RIFF_TAG_LIST, RIFF_TAG_movi);
	ctx->avi_index=RIFF_FindReadInChunk(ctx->fd, 0, RIFF_TAG_idx1, &ctx->idxsz);
	printf("AVI: Index %d ents\n",
		(int)(ctx->idxsz/sizeof(BGBBTJ_AVIINDEXENTRY)));

	w=ctx->bmihead->biWidth;
	h=ctx->bmihead->biHeight;
	if(w<0)w=-w;
	if(h<0)h=-h;

	switch(ctx->bmihead->biCompression)
	{
	case 0:
		break;

	default:
		printf("AVI: codec api\n");
		i=ctx->vidhead->fccHandler;

		inhead=malloc(sizeof(BGBBTJ_BITMAPINFOHEADER));
		outhead=malloc(sizeof(BGBBTJ_BITMAPINFOHEADER));
		inhead->biWidth=w;
		inhead->biHeight=h;
		inhead->biPlanes=ctx->bmihead->biPlanes;
		inhead->biBitCount=ctx->bmihead->biBitCount;
		inhead->biCompression=ctx->bmihead->biCompression;

		outhead->biWidth=w;
		outhead->biHeight=h;
		outhead->biPlanes=1;
		outhead->biBitCount=32;
		outhead->biCompression=0;

		ctx->codec_ctx=BGBBTJ_Codec_BeginDecompress(i,
			inhead, outhead);
		break;
	}

	ctx->fdbuf=malloc((w+3)*(h+3)*4);
//	ctx->fadbuf=malloc(16384);

	ctx->frnum=0;
	ctx->cindex=0;
	ctx->frame_time=0;
	ctx->texnum=-1;
	ctx->mjpg_clrs=BGBBTJ_JPG_BGRA;

	ctx->sampbuf=malloc(1<<16);
	ctx->esampbuf=ctx->sampbuf;
	ctx->sz_sampbuf=(1<<16)/4;

	return(ctx);
}

BGBBTJ_Video_Stats *BGBBTJ_AVI_GetStats(BGBBTJ_AVI_Context *ctx)
{
	BGBBTJ_Video_Stats *tmp;
	int i;

	tmp=malloc(sizeof(BGBBTJ_Video_Stats));

	tmp->width=ctx->bmihead->biWidth;
	tmp->height=ctx->bmihead->biHeight;
//	tmp->frametime=ctx->avihead->dwMicroSecPerFrame/1000000.0;
	tmp->frametime=ctx->avihead->dwMicroSecPerFrame;
	tmp->num_frames=ctx->avihead->dwTotalFrames;

	tmp->vid_fcc=ctx->vidhead->fccHandler;
	tmp->vid_cmpr=ctx->bmihead->biCompression;

	i=0;
	if(ctx->wavefmt)
		{ i=ctx->wavefmt->wFormatTag; }
	switch(i)
	{
	case 1:
		i=RIFF_MAKETAG('P', 'C', 'M', ' ');
		break;
	case 85:
		i=RIFF_MAKETAG('M', 'P', '3', ' ');
		break;
	default:
		i=RIFF_MAKEHEXTAG(i);
		break;
	}
	tmp->aud_cmpr=i;

	i=0;
	if(ctx->audhead)
		{ i=ctx->audhead->fccHandler; }
	tmp->aud_fcc=i;

	return(tmp);
}

int BGBBTJ_AVI_DecodeAudio(BGBBTJ_AVI_Context *ctx, int ofs)
{
	static int rhack=0;
	int cc1, cc2, sz, sz2, bsz;
	int i, j, k, l, err;
	byte *buf, *bufe, *cs, *cse;
	short *sbuf, *sbufe, *ct, *ct1;

	RIFF_ReadChunkInfo(ctx->fd, ofs, &cc1, &sz, &cc2);
//	printf("AVI Frame: @%08X  %.4s %.4s %d\n", ofs, &cc1, &cc2, sz);

	//assume a bad index
	if((sz<0) || (sz>=(1<<22)) ||
		(cc1!=RIFF_MAKETAG('0', '1', 'w', 'b')))
	{
		if(!rhack)
		{
			//try to compensate for encoder bug...
			rhack=1;
			j=ofs-(ctx->ofs_movi+8);
//			printf("B:\n");
			i=BGBBTJ_AVI_DecodeAudio(ctx, j);
			rhack=0;
			return(i);
		}
		return(-1);
	}

//	ctx->fcbuf=RIFF_ReadInChunk(ctx->fd, ofs, &sz);
	sz=RIFF_ReadInChunkTBuf(ctx->fd, ofs, &ctx->fcbuf, &ctx->sz_fcbuf);

	switch(ctx->wavefmt->wFormatTag)
	{
	case 1:	//PCM wave
//		k=sz/(ctx->wavefmt->wBitsPerSample/8)/ctx->wavefmt->nChannels;
		k=sz;
		if(ctx->wavefmt->wBitsPerSample>8)
			k=k>>1;
		if(ctx->wavefmt->nChannels>1)
			k=k>>1;
		
		buf=ctx->fcbuf;
		sbuf=ctx->fcbuf;
		
		//skip if buffer would overflow
		if((ctx->esampbuf+(k*2))>=(ctx->sampbuf+ctx->sz_sampbuf*2))
			break;
		
		for(i=0; i<k; i++)
		{
			if(ctx->wavefmt->wBitsPerSample==8)
			{
				j=(buf[i*ctx->wavefmt->nChannels]-128)*256;
				if(j<-32767)j=-32767;
				if(j>32767)j=32767;
			}else
			{
				j=sbuf[i*ctx->wavefmt->nChannels];
			}
			ctx->esampbuf[i*2]=j;

			if(ctx->wavefmt->nChannels>1)
			{
				if(ctx->wavefmt->wBitsPerSample==8)
				{
					j=(buf[i*ctx->wavefmt->nChannels+1]
						-128)*256;
					if(j<-32767)j=-32767;
					if(j>32767)j=32767;
				}else
				{
					j=sbuf[i*ctx->wavefmt->nChannels+1];
				}
			}
			ctx->esampbuf[(i*2)+1]=j;
		}
		ctx->esampbuf+=k*2;
		break;

	case 0x11:	//ADPCM
//		k=sz/(ctx->wavefmt->wBitsPerSample/8)/ctx->wavefmt->nChannels;
		k=sz*2;
//		if(ctx->wavefmt->wBitsPerSample>8)
//			k=k>>1;
		if(ctx->wavefmt->nChannels>1)
			k=k>>1;
		
		buf=ctx->fcbuf;
//		sbuf=ctx->fcbuf;

		//skip if buffer would overflow
//		if((ctx->esampbuf+(k*2))>=(ctx->sampbuf+ctx->sz_sampbuf*2))
//			break;
		
		cs=buf;
		cse=buf+sz;
		ct=ctx->esampbuf;
		sbufe=ctx->sampbuf+ctx->sz_sampbuf*2;

		if(ctx->wavefmt->nChannels>1)
//		if(0)
		{
			l=(ctx->wavefmt->nBlockAlign-8);
			while(cs<cse)
			{
				ct1=ct+(l*2);
//				if((ctx->esampbuf+(l*2))>=(ctx->sampbuf+ctx->sz_sampbuf*2))
				if(ct1>sbufe)
				{
					printf("BGBBTJ_AVI_DecodeAudio: Sample Buffer Limit");
					break;
				}

				BGBDT_MsImaAdpcm_DecodeBlockStereo(
//					cs, ctx->esampbuf, l);
					cs, ct, l);
					
				cs+=ctx->wavefmt->nBlockAlign;
//				ctx->esampbuf+=l*2;
				ct=ct1;
			}
			ctx->esampbuf=ct;
		}else
		{
			bsz=ctx->wavefmt->nBlockAlign;
			l=(bsz-4)*2;

//			printf("sz=%d bsz=%d l=%d  ", sz, bsz, l);

			while(cs<cse)
			{
				ct1=ct+(l*2);
				if(ct1>sbufe)
				{
					printf("BGBBTJ_AVI_DecodeAudio: Sample Buffer Limit");
					break;
				}

				BGBDT_MsImaAdpcm_DecodeBlockMonoAsStereo(cs, ct, l);
				cs+=bsz;		ct=ct1;
//				printf(".");
			}
			ctx->esampbuf=ct;
//			printf("\n");
		}
		break;

//	case 17:
//		break;

	case 85:	//MP3
		l=0;
		ctx->esampbuf+=l;
		break;

	default:
		break;
	}

//	free(ctx->fcbuf);

	return(0);
}

int BGBBTJ_AVI_DecodeVideo(BGBBTJ_AVI_Context *ctx,
	int ofs, int skip, int clrs)
{
	static int rhack=0;
	int cc1, cc2, sz, sz2;
	int i, j, k, l, w, h, ib, ob;
	byte *buf, *buf2, *dbuf;
	short *sbuf;

	RIFF_ReadChunkInfo(ctx->fd, ofs, &cc1, &sz, &cc2);
//	printf("AVI Frame: @%08X  %.4s %.4s %d\n", ofs, &cc1, &cc2, sz);

	//assume a bad index
	if((sz<0) || (sz>=(1<<22)) ||
		(cc1!=RIFF_MAKETAG('0', '0', 'd', 'c')))
	{
		if(!rhack)
		{
			//try to compensate for encoder bug...
			rhack=1;
			j=ofs-(ctx->ofs_movi+8);
			printf("B: %08X  \n", ofs);
			i=BGBBTJ_AVI_DecodeVideo(ctx, j, skip, clrs);
			rhack=0;
			return(i);
		}
		printf("Bad: %08X  \n", ofs);
		return(-1);
	}

//	printf("AVI Frame: @%08X  %.4s %.4s %d\n", ofs, &cc1, &cc2, sz);

//	ctx->fcbuf=RIFF_ReadInChunk(ctx->fd, ofs, &sz);
	sz=RIFF_ReadInChunkTBuf(ctx->fd, ofs, &ctx->fcbuf, &ctx->sz_fcbuf);

//	dbuf=skip?NULL:ctx->fdbuf;
	dbuf=ctx->fdbuf;
	if(skip)
		dbuf=NULL;

//	printf("AVI Frame: B %d\n", sz);

	switch(ctx->bmihead->biCompression)
	{
	case 0:
		buf=ctx->fcbuf;
		buf2=ctx->fdbuf;
		k=ctx->bmihead->biBitCount/8;
		l=ctx->bmihead->biWidth*ctx->bmihead->biHeight;

		w=ctx->bmihead->biWidth;
		h=ctx->bmihead->biHeight;

		if(sz<(k*l))break;

		for(i=0; i<ctx->bmihead->biHeight; i++)
		{
			ib=(((h-i-1)*w)*k);
			ob=((i*w)*4);
			for(j=0; j<ctx->bmihead->biWidth; j++)
			{
				buf2[ob+(j*4)+0]=buf[ib+(j*k)+2];
				buf2[ob+(j*4)+1]=buf[ib+(j*k)+1];
				buf2[ob+(j*4)+2]=buf[ib+(j*k)+0];
				buf2[ob+(j*4)+3]=255;
			}
		}
		break;

	default:
		if(ctx->codec_ctx)
		{
			w=ctx->bmihead->biWidth;
			h=ctx->bmihead->biHeight;
			BGBBTJ_Codec_DecompressFrame2(ctx->codec_ctx,
//				ctx->fcbuf, skip?NULL:ctx->fdbuf,
				ctx->fcbuf, dbuf,
				sz, w*h*4, clrs);
		}
		break;
	}

//	if(ctx->chroma_color && !skip)
//		BGBBTJ_AVI_FilterChroma(ctx, ctx->fdbuf);
//	free(ctx->fcbuf);

	return(0);
}

void *BGBBTJ_AVI_DecodeFrame2(
	BGBBTJ_AVI_Context *ctx, int skip, int clrs)
{
	int cc1, cc2, sz, sz2;
	int i, j, k;
	byte *buf;

	if(ctx->cindex<0)
	{
		ctx->cindex=0;
		ctx->frnum=0;
	}

	for(i=ctx->cindex; i<(ctx->idxsz/sizeof(BGBBTJ_AVIINDEXENTRY)); i++)
	{
		j=BGBBTJ_AVI_ExtractTagNum(ctx->avi_index[i].ckid);

		if(j==ctx->str_aud)
		{
			j=ctx->ofs_movi+ctx->avi_index[i].dwChunkOffset+8;
			BGBBTJ_AVI_DecodeAudio(ctx, j);
			continue;
		}

		if(j==ctx->str_vid)
		{
			j=ctx->ofs_movi+ctx->avi_index[i].dwChunkOffset+8;

			BGBBTJ_AVI_DecodeVideo(ctx, j, skip, clrs);

			ctx->cindex=i+1;
			ctx->frnum++;

			break;
		}

		printf("BGBBTJ_AVI_DecodeFrame2: ? %d\n", j);
	}

	if(i>=(ctx->idxsz/sizeof(BGBBTJ_AVIINDEXENTRY)))
	{
		ctx->cindex=-1;
	}

	return(ctx->fdbuf);
}

void *BGBBTJ_AVI_DecodeFrame(BGBBTJ_AVI_Context *ctx)
	{ return(BGBBTJ_AVI_DecodeFrame2(ctx, 0, BGBBTJ_JPG_BGRA)); }
void *BGBBTJ_AVI_SkipFrame(BGBBTJ_AVI_Context *ctx)
	{ return(BGBBTJ_AVI_DecodeFrame2(ctx, 1, BGBBTJ_JPG_BGRA)); }

void *BGBBTJ_AVI_DecodeFrameBGRA(BGBBTJ_AVI_Context *ctx)
	{ return(BGBBTJ_AVI_DecodeFrame2(ctx, 0, BGBBTJ_JPG_BGRA)); }

void *BGBBTJ_AVI_DecodeFrameRGB555(BGBBTJ_AVI_Context *ctx)
	{ return(BGBBTJ_AVI_DecodeFrame2(ctx, 0, BGBBTJ_JPG_RGB555)); }
void *BGBBTJ_AVI_SkipFrameRGB555(BGBBTJ_AVI_Context *ctx)
	{ return(BGBBTJ_AVI_DecodeFrame2(ctx, 1, BGBBTJ_JPG_RGB555)); }

int BGBBTJ_AVI_SkipFrames(BGBBTJ_AVI_Context *ctx, int num)
{
	int cc1, cc2, sz, sz2;
	int i, j, k;
	byte *buf;

	if(ctx->cindex<0)
	{
		ctx->cindex=0;
		ctx->frnum=0;
	}

	for(i=ctx->cindex; i<(ctx->idxsz/sizeof(BGBBTJ_AVIINDEXENTRY)); i++)
	{
		j=BGBBTJ_AVI_ExtractTagNum(ctx->avi_index[i].ckid);

		if(j==ctx->str_vid)
		{
			ctx->cindex=i+1;
			ctx->frnum++;

			num--;
			if(num<=0)break;
		}
	}

	if(i>=(ctx->idxsz/sizeof(BGBBTJ_AVIINDEXENTRY)))
	{
		ctx->cindex=-1;
	}

	return(0);
}

int BGBBTJ_AVI_BackFrames(BGBBTJ_AVI_Context *ctx, int num)
{
	int cc1, cc2, sz, sz2;
	int i, j, k;
	byte *buf;

	if(ctx->cindex<0)
	{
		ctx->cindex=0;
		ctx->frnum=0;
	}

	for(i=ctx->cindex; i>0; i--)
	{
		j=BGBBTJ_AVI_ExtractTagNum(ctx->avi_index[i].ckid);

		if(j==ctx->str_vid)
		{
			ctx->cindex=i;
			ctx->frnum--;

			num--;
			if(num<=0)break;
		}
	}

	return(0);
}

void *BGBBTJ_AVI_FrameRaw(BGBBTJ_AVI_Context *ctx, s64 dt)
{
	int fc;
	int iw, ih;

	if(!ctx)return(NULL);

//	if((dt<=0) || (dt>=10))
//		return(ctx->fdbuf);

	fc=0;
	ctx->frame_time-=dt;
	while(ctx->frame_time<=0)
	{
		BGBBTJ_AVI_DecodeFrame(ctx);
//		ctx->frame_time+=ctx->avihead->dwMicroSecPerFrame/1000000.0;
		ctx->frame_time+=ctx->avihead->dwMicroSecPerFrame;
		fc=1;
	}
	return(ctx->fdbuf);
}

void *BGBBTJ_AVI_FrameRawClrs(BGBBTJ_AVI_Context *ctx, s64 dt, int clrs)
{
//	double ft;
	s64 ft;
	int fc, n;
	int iw, ih;

	if(!ctx)return(NULL);

//	if((dt<=0) || (dt>=10))
//		return(ctx->fdbuf);

//	ft=ctx->avihead->dwMicroSecPerFrame/1000000.0;
	ft=ctx->avihead->dwMicroSecPerFrame;
	fc=0;
	ctx->frame_time-=dt;

	if(ctx->frame_time<=(-500000))
		ctx->frame_time=0;

//	n=2;
//	n=4;
	n=8;
	ctx->flags&=~1;

	if(ctx->frame_time<=0)
	{
		while((ctx->frame_time<=(0-ft)) && ((n--)>0))
		{
			BGBBTJ_AVI_DecodeFrame2(ctx, 1, clrs);
			ctx->frame_time+=ft;
			fc=1;
		}
		
		if(n<0)
			{ ctx->frame_time=0; }

		BGBBTJ_AVI_DecodeFrame2(ctx, 0, clrs);
		ctx->frame_time+=ft;
		ctx->flags|=1;
	}
	return(ctx->fdbuf);
}

int BGBBTJ_AVI_Seek(BGBBTJ_AVI_Context *ctx, s64 dt)
{
	s64 dt1, ft;
	int fc;
	int iw, ih;

//	fc=dt/(ctx->avihead->dwMicroSecPerFrame/1000000.0);
	ft=ctx->avihead->dwMicroSecPerFrame;
	dt1=dt; fc=0;
	while(dt1>=ft)
		{ fc++; dt1-=ft; }

	if(fc>0)BGBBTJ_AVI_SkipFrames(ctx, fc);
	if(fc<0)BGBBTJ_AVI_BackFrames(ctx, -fc);
	return(0);
}

s64 BGBBTJ_AVI_GetPosition(BGBBTJ_AVI_Context *ctx)
{
//	float t;
	s64 t;

//	t=ctx->frnum*(ctx->avihead->dwMicroSecPerFrame/1000000.0);
	t=ctx->frnum*ctx->avihead->dwMicroSecPerFrame;
	return(t);
}

int BGBBTJ_AVI_Restart(BGBBTJ_AVI_Context *ctx)
{
	ctx->cindex=-1;
	ctx->frnum=0;
	return(0);
}
