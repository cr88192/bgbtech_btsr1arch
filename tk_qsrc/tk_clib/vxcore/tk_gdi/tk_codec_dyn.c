/*
Dynamic Codec
Try to fetch a DLL for the codec.
 */

static char *tkcodec_dyn_dllpath[256];
static void *tkcodec_dyn_dllhdl[256];
static int tkcodec_dyn_ndll;

static u32 tkcodec_dyn_lstfcc[256];
static int tkcodec_dyn_lstdll[256];
static void *tkcodec_dyn_drvproc[256];
static int tkcodec_dyn_nlst;

int tkcodec_dyn_GetDllIndex(char *path)
{
	int i, j, k;
	
	for(i=0; i<tkcodec_dyn_ndll; i++)
	{
		if(!strcmp(tkcodec_dyn_dllpath[i], path))
			return(i);
	}

	i=tkcodec_dyn_ndll++;
	tkcodec_dyn_dllpath[i]=tk_strdup_in(path);
	tkcodec_dyn_dllhdl[i]=NULL;
	return(i);
}

int tkcodec_dyn_AddList(u32 fcc, char *path)
{
	int ix, i;

	ix=tkcodec_dyn_GetDllIndex(path);
	
	i=tkcodec_dyn_nlst++;
	tkcodec_dyn_lstfcc[i]=fcc;
	tkcodec_dyn_lstdll[i]=ix;
	return(i);
}

void *tkcodec_dyn_GetDllHdlForFcc(u32 fcc)
{
	void *p;
	int i, j, k;

	for(i=0; i<tkcodec_dyn_nlst; i++)
	{
		if(tkcodec_dyn_lstfcc[i]==fcc)
		{
			j=tkcodec_dyn_lstdll[i];
			p=tkcodec_dyn_dllhdl[j];
			if(p)
				return(p);
			p=dlopen(tkcodec_dyn_dllpath[j], 0);
			if(!p)
				continue;
			tkcodec_dyn_dllhdl[j]=p;
			return(p);
		}
	}

	return(NULL);
}

void *tkcodec_dyn_GetDllDrvProcForFcc(u32 fcc)
{
	void *p, *hdll;
	int i, j, k;

	for(i=0; i<tkcodec_dyn_nlst; i++)
	{
		if(tkcodec_dyn_lstfcc[i]==fcc)
		{
			p=tkcodec_dyn_drvproc[i];
			if(p)
				return(p);
			hdll=tkcodec_dyn_GetDllHdlForFcc(fcc);
			if(!hdll)
				continue;
			p=dlsym(hdll, "TkDriverProc");
			tkcodec_dyn_drvproc[i]=p;
			return(p);
		}
	}

	return(NULL);
}


static TKGDI_VidCodecCTX *tkcodec_dyn_begin_decompress(int fcc,
	TKGDI_BITMAPINFOHEADER *in, TKGDI_BITMAPINFOHEADER *out)
{
	TKGDI_VidCodecCTX *ctx;
	cram_ctxinfo *info;
	void *hdll, *hdrvprc;
	int err;

	ctx=NULL;

	hdrvprc=tkcodec_dyn_GetDllDrvProcForFcc(in->biCompression);
	if(!hdrvprc)
		return(NULL);

#if 0
	if(in->biCompression!=TKGDI_FCC_CRAM)
		return(NULL);

	ctx=TKGDI_VidCodecCTX_New();
//	info=bgbrpi_malloc(sizeof(cram_ctxinfo));
	info=malloc(sizeof(cram_ctxinfo));
	ctx->data=info;

	info->ihead=malloc(sizeof(TKGDI_BITMAPINFOHEADER));
	memset(info->ihead, 0, sizeof(TKGDI_BITMAPINFOHEADER));
	info->ihead->biSize		= sizeof(TKGDI_BITMAPINFOHEADER);
	info->ihead->biWidth		= in->biWidth;
	info->ihead->biHeight		= in->biHeight;
	info->ihead->biPlanes		= in->biPlanes;
	info->ihead->biBitCount		= in->biBitCount;
	info->ihead->biCompression	= in->biCompression;
	info->ihead->biSizeImage	=
		in->biWidth*in->biHeight*in->biBitCount/8;

	info->ohead=malloc(sizeof(TKGDI_BITMAPINFOHEADER));
	memset(info->ohead, 0, sizeof(TKGDI_BITMAPINFOHEADER));
	info->ohead->biSize		= sizeof(TKGDI_BITMAPINFOHEADER);
	info->ohead->biWidth		= out->biWidth;
//	info->ohead->biHeight		= -out->biHeight;
	info->ohead->biHeight		= out->biHeight;
	info->ohead->biPlanes		= out->biPlanes;
	info->ohead->biBitCount		= out->biBitCount;
	info->ohead->biCompression	= out->biCompression;
	info->ihead->biSizeImage	=
		out->biWidth*out->biHeight*out->biBitCount/8;

	info->buffer=malloc(out->biWidth*out->biHeight*out->biBitCount/8);

	ctx->decompress_frame=&cram_decompress_frame;
	ctx->decompress_frame_clrs=&cram_decompress_frame_clrs;

#endif

	return(ctx);
}

int TKGDI_CodecDYN_Init()
{
	char buf[256], tb1[256];
	FILE *fdlst;
	TKGDI_VidCodec *codec;
	short *sw;
	char *ct, *cs;
	int i, j;
	u32 fcc;

	printf("DYN: Init\n");

	tkcodec_dyn_nlst=0;
	fdlst=fopen("/boot/tksys/etc/codeclst.ini", "rb");
	if(!fdlst)
		return(0);
	while(!feof(fdlst))
	{
//		buf[0]=0;
		memset(buf, 0, 256);
		fgets(buf, 255, fdlst);
		if(buf[0]=='/')
			continue;
		if(buf[0]<=' ')
			continue;
		if(buf[4]=='=')
		{
			cs=buf+5;
			ct=tb1;
			while(*cs>' ')
				*ct++=*cs++;
			*ct=0;
			fcc=RIFF_MAKETAG(buf[0], buf[1], buf[2], buf[3]);
			tkcodec_dyn_AddList(fcc, tb1);
		}
		if(buf[6]=='=')
		{
			cs=buf+5;
			ct=tb1;
			while(*cs>' ')
				*ct++=*cs++;
			*ct=0;
			fcc=RIFF_MAKETAG(buf[1], buf[2], buf[3], buf[4]);
			tkcodec_dyn_AddList(fcc, tb1);
		}
	}
	fclose(fd);

	codec=TKGDI_VidCodec_New();
	codec->begin_decompress=&tkcodec_dyn_begin_decompress;

	return(0);
}
