#include "bgbmid.h"

int (*bgbmid_WriteStereoSamples2_f[16])(short *buffer, int cnt, int ovcnt);
int bgbmid_n_writesamples=0;

int BGBMID_WriteStereoSamples2(BGBMID_Context *ctx,
	short *buffer, int cnt, int ovcnt)
{
	int tblk[64];
	int i;

//	BGBMID_GetRecordFreqBlock(ctx, tblk, 64);
//	BGBMID_TransformMatchBlockToSamples(tblk, buffer, cnt*2, ovcnt*2);

//	BGBMID_SoundDev_RecReadRawSamples(buffer, cnt, ovcnt);
//	BGBMID_SoundDev_RecReadStereoSamples(buffer, cnt, ovcnt);

//	BGBMID_SoundDev_WriteStereoSamples2(buffer, cnt, ovcnt);
	for(i=0; i<bgbmid_n_writesamples; i++)
		bgbmid_WriteStereoSamples2_f[i](buffer, cnt, ovcnt);
	return(0);
}

BMID_API int BGBMID_RegisterWriteStereoSamples2(
	int (*fcn)(short *buffer, int cnt, int ovcnt))
{
	int i;
	i=bgbmid_n_writesamples++;
	bgbmid_WriteStereoSamples2_f[i]=fcn;
	return(i);
}

BMID_API void *bgbmid_malloc(int size)
{
	void *ptr;
	ptr=malloc(size);
//	ptr=gctalloc("bgbmid_malloc_t", size);
	memset(ptr, 0, size);
	return(ptr);
}

BMID_API void *bgbmid_tyalloc(char *ty, int size)
{
	void *ptr;
	ptr=malloc(size);
//	ptr=gctalloc("bgbmid_tyalloc_t", size);
//	ptr=gctalloc(ty, size);
	memset(ptr, 0, size);
	return(ptr);
}

BMID_API void *bgbmid_tyatomic(char *ty, int size)
{
	void *ptr;
	ptr=malloc(size);
//	ptr=gctalloc("bgbmid_tyatomic_t", size);
	memset(ptr, 0, size);
	return(ptr);
}

BMID_API void bgbmid_free(void *buf)
{
//	gcfree(buf);
	free(buf);
}

BMID_API void *bgbmid_realloc(void *buf, int size)
{
//	return(gcrealloc(buf, size));
	return(realloc(buf, size));
}

BMID_API int BGBMID_LoadPackGlobal(char *name)
{
	BGBMID_PackState *tmp;
	tmp=BGBMID_OpenPack(name);
	if(!tmp)
	{
		printf("BGBMID_LoadPackGlobal: Failed Load '%s'\n", name);
		return(-1);
	}
	return(0);
}

BMID_API int BGBMID_BindListener(BGBMID_Context *ctx, int id)
{
	if(!ctx->listeners[id])
	{
		ctx->listeners[id]=bgbmid_malloc(sizeof(BGBMID_Listener));
//		V3F_ZERO(ctx->listeners[id]->org);
//		V3F_ZERO(ctx->listeners[id]->vel);
//		Mat3F_Identity(ctx->listeners[id]->rot);

		ctx->listen=ctx->listeners[id];
//		BGBMID_SetReverbDefault(ctx);
	}
	ctx->listen=ctx->listeners[id];
	return(0);
}

BMID_API int BGBMID_SetInputBuffer(BGBMID_Context *ctx, byte *buf, int sz)
{
	ctx->ips=buf; ctx->ipe=buf+sz;
	ctx->ip=buf;
	if(ctx->not_dt&16)
		ctx->not_dt=1;
	else
		ctx->not_dt=0;
	ctx->ltag=-1;
	return(0);
}

BMID_API int BGBMID_SynthStream(BGBMID_Context *ctx, byte *buf, int sz)
{
	int i, j, k;

	BGBMID_InitChannels(ctx);
	BGBMID_SetInputBuffer(ctx, buf, sz);
	i=BGBMID_DoSynth(ctx);
	return(i);
}

BMID_API int BGBMID_SetOutputBuffer(BGBMID_Context *ctx, short *buf, int len)
{
	if(!buf)
	{
		buf=bgbmid_malloc(len*2*sizeof(short));
		memset(buf, 0, len*2*sizeof(short));
	}

	ctx->listen->pcmbuf=buf;
	ctx->listen->pcmbufe=buf;
	ctx->listen->pcmbufend=ctx->listen->pcmbuf+(len*2);
	ctx->dt=0;
	ctx->cdt=0;
	return(0);
}

BMID_API short *BGBMID_GetOutputBuffer(BGBMID_Context *ctx)
	{ return(ctx->listen->pcmbuf); }
BMID_API int BGBMID_GetOutputBufferPos(BGBMID_Context *ctx)
	{ return((ctx->listen->pcmbufe-ctx->listen->pcmbuf)/2); }
BMID_API int BGBMID_GetOutputBufferSize(BGBMID_Context *ctx)
	{ return((ctx->listen->pcmbufend-ctx->listen->pcmbuf)/2); }
BMID_API int BGBMID_GetOutputSampleRate(BGBMID_Context *ctx)
	{ return(ctx->samplerate); }

BMID_API int BGBMID_SetOutputSampleRate(BGBMID_Context *ctx, int rate)
{
	ctx->samplerate=rate;
	return(0);
}

BMID_API int BGBMID_SetNoteTicks(BGBMID_Context *ctx, int ticks)
{
	ctx->noteticks=ticks;
	ctx->ticksamp=ctx->samplerate*
		(ctx->notelen/ctx->noteticks);
	ctx->ticksamp*=ctx->ratesc;
	return(0);
}

BMID_API int BGBMID_SetNoteLen(BGBMID_Context *ctx, float sec)
{
	ctx->notelen=sec;
	ctx->ticksamp=ctx->samplerate*
		(ctx->notelen/ctx->noteticks);
	ctx->ticksamp*=ctx->ratesc;
	return(0);
}

BMID_API int BGBMID_SetRateScale(BGBMID_Context *ctx, float scale)
{
	ctx->ratesc=scale;
	ctx->ticksamp=ctx->samplerate*
		(ctx->notelen/ctx->noteticks);
	ctx->ticksamp*=ctx->ratesc;
	return(0);
}

BMID_API int BGBMID_SetFreqScale(BGBMID_Context *ctx, float scale)
{
	ctx->freqsc=scale;
	return(0);
}

BMID_API BGBMID_Context *BGBMID_NewContext()
{
	BGBMID_Context *ctx;

	ctx=bgbmid_malloc(sizeof(BGBMID_Context));
	memset(ctx, 0, sizeof(BGBMID_Context));

//	ctx->patches=bgbmid_malloc(128*sizeof(char *));
	ctx->patches=bgbmid_malloc(8192*sizeof(char *));
	ctx->drumpatches=bgbmid_malloc(128*sizeof(char *));
//	memset(ctx->patches, 0, 128*sizeof(char *));
	memset(ctx->patches, 0, 8192*sizeof(char *));
	memset(ctx->drumpatches, 0, 128*sizeof(char *));
//	ctx->maxpatches=128;
	ctx->maxpatches=8192;
	ctx->maxdrumpatches=128;

	ctx->noteticks=120;
	ctx->notelen=1.0;
	ctx->ratesc=1;
	ctx->samplerate=44100;
	ctx->tempsz=131072;
	ctx->freqsc=1;

	ctx->ticksamp=ctx->samplerate*
		(ctx->notelen/ctx->noteticks);

	BGBMID_BindListener(ctx, 0);

	return(ctx);
}

BMID_API int BGBMID_DestroyContext(BGBMID_Context *ctx)
{
//	if(ctx->listen->pcmbuf)bgbmid_free(ctx->listen->pcmbuf);
	if(ctx->patches)bgbmid_free(ctx->patches);
	if(ctx->drumpatches)bgbmid_free(ctx->drumpatches);
	bgbmid_free(ctx);
	return(0);
}

BMID_API int BGBMID_PlayMixTime(BGBMID_Context *ctx, double dt)
{
	static short sbuf[16384*2];
	static double accDt;
	int i, j, sn, st;

	accDt+=dt;

//	st=15;
//	st=30;
//	st=10;
	st=23;
	sn=44100/st;

#if 1
	for(i=0; i<sn; i++)
	{
		j=sin(4*M_TAU*(i/2940.0))*16384;
		sbuf[i*2+0]=j;
		sbuf[i*2+1]=j;
	}
#endif

	while(accDt>=(1.0/st))
	{
		BGBMID_SynthSamples2(ctx, sbuf, sn, sn*2);
		BGBMID_WriteStereoSamples2(ctx, sbuf, sn, sn*2);
//		BGBMID_SoundDev_WriteStereoSamples2(sbuf, sn, sn*2);
//		ctx->cnTime+=1.0/st;
		accDt-=1.0/st;
	}

#if 1
	i=44100*accDt;
	BGBMID_SynthSamples2(ctx, sbuf, i, i*2);
	BGBMID_WriteStereoSamples2(ctx, sbuf, i, i*2);
	accDt-=i/44100.0;
#endif

	return(0);
}

BMID_API int BGBMID_LookupSampleName(BGBMID_Context *ctx, char *name)
{
	int chan, prg;
	
	prg=BGBMID_LookupIndexPatchName(4096, 16384, name);
	return(prg);
}

#if 0
BMID_API int BGBMID_PlaySampleName(BGBMID_Context *ctx, char *name)
{
	int chan, prg, id;
	
	chan=BGBMID_AllocChannel(ctx);
	prg=BGBMID_AllocPatchName(4096, 16384, name);
	
	if((chan<0) || (prg<0))
	{
		printf("BGBMID_PlaySampleName: Failed Alloc Chan=%d Prog=%d\n",
			chan, prg);
		return(-1);
	}

//	printf("BGBMID_PlaySampleName: Chan=%d Prog=%d\n", chan, prg);
	
	id=BGBMID_PlayMixSample(ctx, chan, prg);
	return(id);
//	return(chan);
}
#endif

BMID_API int BGBMID_ChannelStillPlayingP(BGBMID_Context *ctx, int id)
{
	return(BGBMID_ChannelMapValidate(ctx, id)>=0);
}

BMID_API int BGBMID_SetListenerOrigin(BGBMID_Context *ctx,
	int id, float *org)
{
	ctx->listeners[id]->orgTime=ctx->cnTime;
//	V3F_COPY(org, ctx->listeners[id]->org);
	return(0);
}

BMID_API int BGBMID_SetListenerVelocity(BGBMID_Context *ctx,
	int id, float *vel)
{
//	V3F_COPY(vel, ctx->listeners[id]->vel);
	return(0);
}

BMID_API int BGBMID_SetListenerRotation(BGBMID_Context *ctx,
	int id, float *rot)
{
//	Mat3F_Copy(rot, ctx->listeners[id]->rot);
	return(0);
}

BMID_API int BGBMID_SetListenerRotationQuat(BGBMID_Context *ctx,
	int id, float *rot)
{
//	QuatF_To3Matrix(rot, ctx->listeners[id]->rot);
	return(0);
}


BMID_API int BGBMID_CreateStreamName(BGBMID_Context *ctx,
	char *name, int rate, int len)
{
	BGBMID_Patch *pat;

	int chan, prg;
	
//	chan=BGBMID_AllocChannel(ctx);
	prg=BGBMID_AllocPatchName(4096, 16384, name);
	
//	if((chan<0) || (prg<0))
	if(prg<0)
	{
		printf("BGBMID_CreateStreamName: Failed Alloc Prog=%d\n", prg);
		return(-1);
	}

	if(ctx->patches[prg])
		{ return(0); }

	pat=BGBMID_LookupPatch(name);
	if(pat)
	{
		BGBMID_UnlinkPatch(pat);
		ctx->patches[prg]=NULL;
		pat=NULL;
	}
	if(!pat)
	{
		//FIXME: ugly/broke
		pat=BGBMID_CreatePatchStream(name, rate, len);
		pat->str_next=ctx->streams;
		ctx->streams=pat;
	}

	BGBMID_ProgramDefinePatch(ctx, prg, name);

//	printf("BGBMID_PlaySampleName: Chan=%d Prog=%d\n", chan, prg);
	
//	BGBMID_PlayMixSample(ctx, chan, prg);
	return(prg);
}

BMID_API int BGBMID_WriteStreamSamples(
	BGBMID_Context *ctx, int num, short *pcm, int len)
{
	BGBMID_Patch *pat;
	BGBMID_Sample *samp;
	int i, j, k, l;

//	pat=BGBMID_LookupPatch(name);
	pat=ctx->patches[num];
	
	if(!pat)return(-1);
	
	if(!(pat->samp->flags&BGBMID_SFL_STREAM))
	{
		BGBMID_UnlinkPatch(pat);
		ctx->patches[num]=NULL;
		BGBMID_CreateStreamName(ctx, pat->name, 44100, 65536);
		return(-1);
	}
	
	samp=pat->samp;
	
	if(!samp->pcm_strpos)
		samp->pcm_strpos=samp->pcm;
	
	l=samp->len-(samp->pcm_strpos-samp->pcm);
	if(len<l)l=len;
	
	for(i=0; i<l; i++)
		{ samp->pcm_strpos[i]=pcm[i]; }
	samp->pcm_strpos+=l;
	
	return(0);
}

#if 0
// BMID_API int BGBMID_CreateProgramSpeak(
	BGBMID_Context *ctx, char *voice, char *text)
{
	char tb[256];
//	BSG_State *gctx;
	BGBMID_Patch *pat;
	int chan, prg, len, rate;
	char *s;
	int h;
	int i, j, k;

	h=0;

	s=voice;
	while(*s)
		{ h=h*4093+(*s++); }
	h=h*4093;
	s=text;
	while(*s)
		{ h=h*4093+(*s++); }
	h=h*4093;
//	h=((h*4093)>>12)&65535;
	
	sprintf(tb, "_speak_%08X", h);
	
	i=BGBMID_LookupSampleName(ctx, tb);
	if(i>=0)return(i);

	BSG_Init();

	gctx=BSG_NewContext(voice);
	
//	gctx->obuf1=bgbmid_malloc((1<<16)*sizeof(short));
//	gctx->obuf1sz=1<<16;
	
//	ctx->out_cb=bsg_out_samples;
	BSG_SendText(gctx, text);

	BSG_ProcessItems(gctx);
	
//	chan=BGBMID_AllocChannel(ctx);
	prg=BGBMID_AllocPatchName(4096, 16384, tb);
	
//	if((chan<0) || (prg<0))
	if(prg<0)
	{
		printf("BGBMID_CreateStreamName: Failed Alloc Prog=%d\n", prg);
		return(-1);
	}

	pat=BGBMID_LookupPatch(tb);
	if(!pat)
	{
		//FIXME: ugly/broke
		len=gctx->obuf1pos;
		rate=44100;
		pat=BGBMID_CreatePatchSamples(tb, gctx->obuf1, rate, len);
//		pat->str_next=ctx->streams;
//		ctx->streams=pat;
	}

	BSG_FreeContext(gctx);

	BGBMID_ProgramDefinePatch(ctx, prg, tb);

//	printf("BGBMID_PlaySampleName: Chan=%d Prog=%d\n", chan, prg);
	
//	BGBMID_PlayMixSample(ctx, chan, prg);
	return(prg);
}
#endif

#if 0
BMID_API int BGBMID_RecordMixTime(BGBMID_Context *ctx, double dt)
{
	static short sbuf[16384*2];
	static double accDt;
	int i, j, sn, st;

	accDt+=dt;

//	st=15;
//	st=30;
//	st=10;
	st=23;
	sn=44100/st;

#if 0
	for(i=0; i<sn; i++)
	{
		j=sin(4*M_TAU*(i/2940.0))*16384;
		sbuf[i*2+0]=j;
		sbuf[i*2+1]=j;
	}
#endif

	while(accDt>=(1.0/st))
	{
//		BGBMID_SoundDev_RecReadRawSamples(sbuf, sn, sn*2);
		BGBMID_SoundDev_RecReadStereoSamples2(sbuf, sn, sn*2);
		BGBMID_RecordHandleSamples(ctx, sbuf, sn, sn*2);
		
//		BGBMID_SynthSamples2(ctx, sbuf, sn, sn*2);
//		BGBMID_WriteStereoSamples2(ctx, sbuf, sn, sn*2);
//		BGBMID_SoundDev_WriteStereoSamples2(sbuf, sn, sn*2);
//		ctx->cnTime+=1.0/st;
		accDt-=1.0/st;
	}

#if 0
	i=44100*accDt;
//	BGBMID_SynthSamples2(ctx, sbuf, i, i*2);
//	BGBMID_WriteStereoSamples2(ctx, sbuf, i, i*2);
	BGBMID_SoundDev_RecReadRawSamples(sbuf, i, i*2);
	BGBMID_RecordHandleSamples(ctx, sbuf, i, i*2);
	accDt-=i/44100.0;
#endif

	return(0);
}
#endif
