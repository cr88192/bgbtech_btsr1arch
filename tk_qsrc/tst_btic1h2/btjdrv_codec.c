/*
Copyright (C) 2015 by Brendan G Bohannon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// #include <bgbbtj.h>

BTIC1H_VidCodec *lbxgl_codecs=NULL;

BTIC1H_VidCodec *BTIC1H_VidCodec_New()
{
	BTIC1H_VidCodec *tmp;

	tmp=btjpg_gctalloc("lbxgl_vidcodec_t", sizeof(BTIC1H_VidCodec));
	tmp->next=lbxgl_codecs;
	lbxgl_codecs=tmp;

	return(tmp);
}

BTIC1H_VidCodecCTX *BTIC1H_VidCodecCTX_New()
{
	BTIC1H_VidCodecCTX *tmp;

	tmp=btjpg_gctalloc("lbxgl_vidcodecctx_t", sizeof(BTIC1H_VidCodecCTX));
	return(tmp);
}

void BTIC1H_VidCodecCTX_Destroy(BTIC1H_VidCodecCTX *ctx)
{
}

int BTIC1H_Codec_DecompressQuery(int fcc,
	BTIC1H_BMPInfoHeader *in, BTIC1H_BMPInfoHeader *out)
{
	BTIC1H_VidCodec *cur;
	int i;

	BTIC1H_Codec_Init();

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

int BTIC1H_Codec_CompressQuery(int fcc,
	BTIC1H_BMPInfoHeader *in, BTIC1H_BMPInfoHeader *out)
{
	BTIC1H_VidCodec *cur;
	int i;

	BTIC1H_Codec_Init();

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

BTIC1H_VidCodecCTX *BTIC1H_Codec_BeginDecompress(int fcc,
	BTIC1H_BMPInfoHeader *in, BTIC1H_BMPInfoHeader *out)
{
	BTIC1H_VidCodec *cur;
	BTIC1H_VidCodecCTX *tmp;
	int *cct;

	BTIC1H_Codec_Init();

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

	btjpg_printf("Codec: Decode Fail Codec %.4s\n", &fcc);

	return(NULL);
}

BTIC1H_VidCodecCTX *BTIC1H_Codec_BeginCompress(int fcc,
	BTIC1H_BMPInfoHeader *in, BTIC1H_BMPInfoHeader *out)
{
	BTIC1H_VidCodec *cur;
	BTIC1H_VidCodecCTX *tmp;
	int *cct;

	BTIC1H_Codec_Init();

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

	btjpg_printf("Codec: Encode Fail Codec %.4s/%.4s \n",
		&fcc, &out->biCompression);
	return(NULL);
}

//default: base at upper left corner, rgba ordering, a=255
int BTIC1H_Codec_DecompressFrame(BTIC1H_VidCodecCTX *ctx,
	void *src, void *dst, int ssz, int dsz)
{
	if(!ctx->decompress_frame)return(-1);
	return(ctx->decompress_frame(ctx, src, dst, ssz, dsz));
}

int BTIC1H_Codec_DecompressFrameClrs(BTIC1H_VidCodecCTX *ctx,
	void *src, void *dst, int ssz, int dsz, int clrs)
{
	if(!ctx->decompress_frame_clrs)return(-1);
	return(ctx->decompress_frame_clrs(ctx, src, dst, ssz, dsz, clrs));
}

int BTIC1H_Codec_DecompressFrame2(BTIC1H_VidCodecCTX *ctx,
	void *src, void *dst, int ssz, int dsz, int clrs)
{
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
int BTIC1H_Codec_CompressFrame(BTIC1H_VidCodecCTX *ctx,
	void *src, void *dst, int dsz, int qfl, int clrs, int *rfl)
{
	if(!ctx->compress_frame)return(-1);
	return(ctx->compress_frame(ctx, src, dst, dsz, qfl, clrs, rfl));
}

int BTIC1H_VidCodecCTX_EndCompress(BTIC1H_VidCodecCTX *ctx)
{
	if(!ctx->end_compress)return(-1);
	return(ctx->end_compress(ctx));
}

int BTIC1H_VidCodecCTX_EndDecompress(BTIC1H_VidCodecCTX *ctx)
{
	if(!ctx->end_decompress)return(-1);
	return(ctx->end_decompress(ctx));
}

int BTIC1H_Codec_Init()
{
	static int init;

	if(init)return(0);
	init=1;

//	BTIC1H_CodecMJPG_Init();
//	BTIC1H_CodecVFW_Init();
//	BTIC1H_CodecMJPG_Init();
//	BTIC1H_CodecMPNG_Init();
//	BTIC1H_CodecMBTC_Init();
//	BTIC1H_CodecTHEO_Init();
//	BTIC1H_CodecRPZA_Init();
//	BTIC1H_CodecBT1C_Init();
//	BTIC1H_CodecBT1D_Init();
//	BTIC1H_CodecBT2C_Init();
//	BTIC1H_CodecBTIC_Init();

	BTIC1H_CodecBT1H_Init();
	BTIC1H_CodecBT4B_Init();

	return(1);
}
