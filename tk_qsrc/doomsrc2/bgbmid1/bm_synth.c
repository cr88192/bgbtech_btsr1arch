#include "bgbmid.h"

u32 bgbmid_fmsyn_regdat[256][16];

BMID_API int BGBMID_NoteOff(BGBMID_Context *ctx,
	int chan, int note, int vel);

BMID_API int BGBMID_ProgramChange(BGBMID_Context *ctx, int chan, int num);

BMID_API int BGBMID_ChannelMapValidate(BGBMID_Context *ctx, int chan);


BMID_API void BGBMID_SetFmRegisterData(BGBMID_Context *ctx,
	int prgn, int idx, u32 val)
{
	bgbmid_fmsyn_regdat[prgn][idx]=val;
}


BMID_API void BGBMID_InitMidiChannel(BGBMID_Context *ctx, int num)
{
	int i, j;

	if(ctx->chan[num])
	{
#if 0
		if(ctx->chan[num]->mix)
		{
			BGBMID_DestroyMixChannel(ctx, ctx->chan[num]);
//			ctx->chan[num]=NULL;
		}
#endif
	}
	
	BGBMID_InitChannel(ctx, num);
	BGBMID_ProgramChange(ctx, num, 0);
}

BMID_API void BGBMID_InitChannel(BGBMID_Context *ctx, int num)
{
	int i, j;

//	if(ctx->chan[num])
//		return;

	if(!ctx->chan[num])
		ctx->chan[num]=bgbmid_malloc(sizeof(BGBMID_Channel));
	
	ctx->chan[num]->id=num;
	ctx->chan[num]->vol=1.0;
	ctx->chan[num]->flags=0;
	
//	for(j=0; j<16; j++)
	for(j=0; j<32; j++)
		ctx->chan[num]->control[j]=0;

	ctx->chan[num]->control[7]=100<<7;
	ctx->chan[num]->control[8]=64<<7;
	ctx->chan[num]->control[10]=64<<7;
	ctx->chan[num]->control[31]=64<<7;

	for(j=0; j<128; j++)
		ctx->chan[num]->notevel[j]=0;

//	V3F_ZERO(ctx->chan[num]->org);
//	V3F_ZERO(ctx->chan[num]->vel);
//	Mat3F_Identity(ctx->chan[num]->rot);

//	BGBMID_ProgramChange(ctx, num, 0);
}

BMID_API int BGBMID_InitChannels(BGBMID_Context *ctx)
{
	int i, j;

	ctx->n_chan=16;
	ctx->m_chan=1024;

	for(i=0; i<ctx->n_chan; i++)
	{
		BGBMID_InitMidiChannel(ctx, i);
	}

	return(0);
}

BMID_API void BGBMID_CheckInitMidiChannel(BGBMID_Context *ctx, int num)
{
	if(ctx->chan[num] && !ctx->chan[num]->mix)
		return;
	BGBMID_InitMidiChannel(ctx, num);
}

BMID_API void BGBMID_CheckInitChannel(BGBMID_Context *ctx, int num)
{
	if(ctx->chan[num])
		return;
	BGBMID_InitChannel(ctx, num);
}

BMID_API void BGBMID_DeleteChannel(BGBMID_Context *ctx, int num)
{
	if(num<0)return;
	if(num>=ctx->m_chan)return;
	
	if(num<16)
	{
		//base MIDI channels are simply reset
		BGBMID_InitMidiChannel(ctx, num);
		return;
	}

	if(ctx->chan[num])
	{
//		if(ctx->chan[num]->mix)
//			{ BGBMID_DestroyMixChannel(ctx, ctx->chan[num]); }
		bgbmid_free(ctx->chan[num]);
		ctx->chan[num]=NULL;
		return;
	}
}

BMID_API int BGBMID_FindFreeChannel(BGBMID_Context *ctx)
{
	int i;
	
	for(i=0; i<ctx->n_chan; i++)
		if(!ctx->chan[i])
			return(i);
	if((ctx->n_chan+1)>=ctx->m_chan)
	{
		return(-1);
	}
	
	i=ctx->n_chan++;
	return(i);
}

BMID_API int BGBMID_AllocChannel(BGBMID_Context *ctx)
{
	int i;
	
	i=BGBMID_FindFreeChannel(ctx);
	BGBMID_CheckInitChannel(ctx, i);
	return(i);
}

float bgbmid_funcgen(float th, int fn)
{
	float th2;
	float f, g, h;
	float v;
	int i, j, k;
	
	th2=th*(1.0/M_PI);
	v=0;
	
	switch(fn)
	{
#if 1
	case 0:
		v=sin(th);
		break;
	case 1:
		v=sin(th);
		if(v<0)v=0;
		break;
	case 2:
		v=sin(th);
		if(v<0)v=-v;
		break;
	case 3:
		i=th2*2;
		v=sin(th);
		if(v<0)v=-v;
		if(i&1)v=0;
		break;
#endif

#if 0
	case 0:
		i=th2;
		j=(byte)(th2*256);
		v=(i&1)?(-1.0+(j/256.0)):(1.0-(j/256.0));
		break;
	case 1:
		i=th2;
		j=(byte)(th2*256);
		v=(i&1)?(-1.0+(j/256.0)):(1.0-(j/256.0));
		if(v<0)v=0;
		break;
	case 2:
		i=th2;
		j=(byte)(th2*256);
		v=(i&1)?(-1.0+(j/256.0)):(1.0-(j/256.0));
		if(v<0)v=-v;
		break;
	case 3:
		i=th2;
		j=(byte)(th2*256);
		v=(-1.0+(j/256.0));
		if(v<0)v=0;
		break;
#endif

	case 4:
		i=th2;
		v=sin(2*th);
//		if(v<0)v=-v;
		if(i&1)v=0;
		break;
	case 5:
		i=th2;
		v=sin(2*th);
		if(v<0)v=-v;
		if(i&1)v=0;
		break;
	case 6:
		i=th2;
		v=(i&1)?-1.0:1.0;
		break;
	case 7:
		i=th2*16;
		v=0;
		if((i&15)== 0)v= 1.0;
		if((i&15)==15)v=-1.0;
		break;

	case 8:
		f=sin(th);
		v=(f>=0)?f*f:-f*f;
		break;
	case 9:
		i=th2;
		v=(i&1)?-1.0:1.0;
		break;
	case 10:
		i=th2;
		j=(byte)(th2*256);
		v=(i&1)?(-1.0+(j/256.0)):(1.0-(j/256.0));
		break;
	case 11:
		i=th2;
		j=(byte)(th2*256);
		v=(i&1)?(-1.0+(j/128.0)):(1.0-(j/128.0));
		break;
	case 12:
		i=th2;
		j=(byte)(th2*256);
		v=(i&1)?(-1.0+(j/192.0)):(1.0-(j/192.0));
		break;
	}
	
	return(v);
}

BMID_API int BGBMID_BlendNoteTone(BGBMID_Context *ctx,
	int chan, int len, int note)
{
	byte *brec;
	u32 *rec;
	float vol, lvol, rvol, freq, pan, bal, pbl;
	float f, g, ssc, th;
	float modsc, carsc, loghz, modvol, carvol;
	int i, j, k, b, ss, se, sm, fcn;
	int i0, i1;

	short basenote;
	byte modfcn, carfcn, modksc;
	byte modlvl, carlvl, carksc;
	byte fbconn;

	i=ctx->chan[chan]->prog;
	if(chan==8)
		{ i=128+(i&63); }
	else
		{ i=i&127; }

	rec=bgbmid_fmsyn_regdat[i];
	brec=(byte *)rec;

	basenote=(short)(rec[4]>>16);

//	modfcn=(rec[2]>>8)&255;
//	carfcn=(rec[3]>>16)&255;
	modfcn=(brec[4+ 3])&255;
	modksc=(brec[4+ 4])&255;
	modlvl=(brec[4+ 5])&255;
	fbconn=(brec[4+ 6])&255;
	carfcn=(brec[4+10])&255;
	carksc=(brec[4+11])&255;
	carlvl=(brec[4+12])&255;

	note+=basenote;
	if(note<0)note=0;

//	f=BGBMID_Sound_CalcChanAttenuation(ctx, ctx->chan[chan]);
	f=1.0;
	vol=ctx->chan[chan]->control[7]*f;
	bal=ctx->chan[chan]->control[8]/16384.0;
	pan=ctx->chan[chan]->control[10]/16384.0;

	if(note>=0)
	{
		f=ctx->chan[chan]->notevel[note]/127.0;
		vol*=f;
	}

	vol=pow(vol/16384.0, 1.66)*16384.0;

	lvol=(1.0-bal)*(1.0-pan*0.5);
	rvol=(0.0+bal)*(0.5+pan*0.5);
	g=sqrt(lvol*lvol + rvol*rvol);
	lvol=vol*lvol*(1.0/g);
	rvol=vol*rvol*(1.0/g);

	b=ctx->listen->pcmbase+(ctx->listen->pcmbufe-ctx->listen->pcmbuf)/2;
	loghz=(note-69)/12.0;
	freq=pow(2, loghz)*440;

	pbl=(ctx->chan[chan]->control[31]-8192)/8192.0;
	freq*=pow(2, pbl);

	if(ctx->chan[chan]->note_end[note])
	{
		ctx->chan[chan]->notevel[note]=0;
		return(0);
	}

	switch(modksc)
	{
	case 0: modsc=1.0; break;
	case 1: modsc=1.0*pow(2.0, loghz); break;
	case 2: modsc=1.0*pow(1.5, loghz); break;
	case 3: modsc=1.0*pow(4.0, loghz); break;
	default: modsc=1.0; break;
	}

	switch(carksc)
	{
	case 0: carsc=1.0; break;
	case 1: carsc=1.0*pow(2.0, loghz); break;
	case 2: carsc=1.0*pow(1.5, loghz); break;
	case 3: carsc=1.0*pow(4.0, loghz); break;
	default: carsc=1.0; break;
	}

//	fcn=2;
//	fcn=3;

	modvol=((modlvl&63)/63.0);
	carvol=((carlvl&63)/63.0);
	
	modvol=1.0-modvol;
	carvol=1.0-carvol;

//	th=M_PI/22050;
	th=(2*M_PI)/(ctx->samplerate);
	for(j=0; j<len; j++)
	{
//		f=sin(M_PI/22050*(b+j)*freq);
//		f=sin(th*(b+j)*freq);

		if(fbconn&1)
		{
#if 0
			if(modfcn!=carfcn)
			{
				if(modvol>carvol)
				{
					f=bgbmid_funcgen(th*(b+j)*freq, modfcn)*modvol*modsc;
				}else
				{
					f=bgbmid_funcgen(th*(b+j)*freq, carfcn)*carvol*carsc;
				}
			}else
			{
				f=bgbmid_funcgen(th*(b+j)*freq, carfcn)*
					(modvol+carvol)*carsc;
			}
#endif

			f=bgbmid_funcgen(th*(b+j)*freq, modfcn)*modvol*modsc;
			g=bgbmid_funcgen(th*(b+j)*freq, carfcn)*carvol*carsc;
			f=f+g;
//			f=g;
		}else
		{
#if 1
//			g=bgbmid_funcgen(th*(b+j)*freq, modfcn)*((modlvl&63)/63.0);
			g=bgbmid_funcgen(th*(b+j)*freq, modfcn);

#if 1
			switch((fbconn>>1)&7)
			{
				case 0: g=0; break;
				case 1: g=g*(M_PI*0.0625); break;
				case 2: g=g*(M_PI*0.1250); break;
				case 3: g=g*(M_PI*0.2500); break;
				case 4: g=g*(M_PI*0.5000); break;
				case 5: g=g*(M_PI*1.0000); break;
				case 6: g=g*(M_PI*2.0000); break;
				case 7: g=g*(M_PI*4.0000); break;
			}
#endif

			g=g*modvol*modsc;
#endif

//			carfcn=6;
//			carfcn=10;
//			carfcn=11;

//			carfcn=12;
//			g=0;

			f=bgbmid_funcgen(
				th*(b+j)*freq+(g*M_PI), carfcn)*
				carvol*carsc;
		}

		ctx->listen->pcmbuft[j*2+0]+=(f*lvol);
		ctx->listen->pcmbuft[j*2+1]+=(f*rvol);
	}

	return(0);
}

BMID_API int BGBMID_BlendNoteSampLoop(BGBMID_Context *ctx,
	int chan, int len, int note,
	BGBMID_Sample *samp, float freq)
{
	float vol, lvol, rvol, pan, bal;
	float loffs, roffs;
	float f, g, h, ssc, scd;
	int i, j, k, b, ss, se, sm, b2, b3;
	int i0, i1;

//	f=BGBMID_Sound_CalcChanAttenuation(ctx, ctx->chan[chan]);
	f=1.0;
	vol=ctx->chan[chan]->control[7]*f;
	bal=ctx->chan[chan]->control[8]/16384.0;
	pan=ctx->chan[chan]->control[10]/16384.0;

	f=ctx->chan[chan]->notevel[note]/127.0;
	vol*=f;

	vol=pow(vol/16384.0, 1.66)*16384.0;

	lvol=(1.0-bal)*(1.0-pan*0.5);
	rvol=(0.0+bal)*(0.5+pan*0.5);
	g=sqrt(lvol*lvol + rvol*rvol);
	lvol=vol*lvol*(1.0/g);
	rvol=vol*rvol*(1.0/g);

	loffs=pan; roffs=1.0-pan;
	g=sqrt(loffs*loffs + roffs*roffs);
	loffs=loffs*(1.0/g);
	roffs=roffs*(1.0/g);

	b=ctx->listen->pcmbase+(ctx->listen->pcmbufe-ctx->listen->pcmbuf)/2;
	b2=b-ctx->chan[chan]->note_strt[note];
	b3=b-ctx->chan[chan]->note_end[note];

	ss=samp->start;
	se=samp->end;
	sm=se-ss;
	ssc=(samp->rate/((float)(ctx->samplerate)))/samp->root;

//	ssc*=freq;
	ssc*=freq*ctx->freqsc;
	scd=log(1.0+ssc)/log(2.0);
	if(scd<0.0)scd=0.0;
	if(scd>6.9)scd=6.9;
	
	lvol/=32767.0;
	rvol/=32767.0;

	loffs=loffs*0.0010*samp->rate;
	roffs=roffs*0.0010*samp->rate;

	if(b3!=b)
	{
		ctx->chan[chan]->notevel[note]=0;
		return(0);
	}

#if 1
	if((ssc*b2)<samp->start)
	{
		for(i=0; i<len; i++)
		{
			f=ssc*(b2+i);
			if(f>=samp->start)break;

			f=ssc*(b2+i)-loffs;
			f=BGBMID_SplinePolate(samp->pcm, samp->len, f);
//			f=v2x(BGBMID_Sample_InterpolatedSamplePosition(
//				ctx, samp, f, scd));
			ctx->listen->pcmbuft[i*2+0]+=f*lvol;
//			if(isnan(ctx->listen->pcmbuft[i*2+0]))*(int *)-1=-1;

			f=ssc*(b2+i)-roffs;
			f=BGBMID_SplinePolate(samp->pcm, samp->len, f);
//			f=v2y(BGBMID_Sample_InterpolatedSamplePosition(
//				ctx, samp, f, scd));
			ctx->listen->pcmbuft[i*2+1]+=f*rvol;
//			if(isnan(ctx->listen->pcmbuft[i*2+1]))*(int *)-1=-1;
		}

#if 1
		j=i+64;
		for(; (i<j) && (i<len); i++)
		{
			h=(j-i)/64.0;

			f=ssc*(b2+i)-loffs;	g=ssc*(b+i)-loffs;
			f=BGBMID_SplinePolate(samp->pcm, samp->len, f);
//			f=v2x(BGBMID_Sample_InterpolatedSamplePosition(
//				ctx, samp, f, scd));
			g=BGBMID_SplinePolateLoop(samp->pcm+ss, sm, g);
//			g=v2x(BGBMID_Sample_InterpolatedSampleSubPosition(ctx,
//				samp, g, scd, ss, sm));
			f=f*h + g*(1.0-h);
			ctx->listen->pcmbuft[i*2+0]+=f*lvol;

			f=ssc*(b2+i)-roffs;	g=ssc*(b+i)-roffs;
			f=BGBMID_SplinePolate(samp->pcm, samp->len, f);
//			f=v2y(BGBMID_Sample_InterpolatedSamplePosition(
//				ctx, samp, f, scd));
			g=BGBMID_SplinePolateLoop(samp->pcm+ss, sm, g);
//			g=v2y(BGBMID_Sample_InterpolatedSampleSubPosition(ctx,
//				samp, g, scd, ss, sm));
			f=f*h + g*(1.0-h);
			ctx->listen->pcmbuft[i*2+1]+=f*rvol;
		}
#endif

		for(; i<len; i++)
		{
			f=ssc*(b+i)-loffs;
			f=BGBMID_SplinePolateLoop(samp->pcm+ss, sm, f);
//			f=v2x(BGBMID_Sample_InterpolatedSampleSubPosition(ctx,
//				samp, f, scd, ss, sm));
			ctx->listen->pcmbuft[i*2+0]+=f*lvol;
//			if(isnan(ctx->listen->pcmbuft[i*2+0]))*(int *)-1=-1;

			f=ssc*(b+i)-roffs;
			f=BGBMID_SplinePolateLoop(samp->pcm+ss, sm, f);
//			f=v2y(BGBMID_Sample_InterpolatedSampleSubPosition(ctx,
//				samp, f, scd, ss, sm));
			ctx->listen->pcmbuft[i*2+1]+=f*rvol;
//			if(isnan(ctx->listen->pcmbuft[i*2+1]))*(int *)-1=-1;
		}

		return(0);
	}
#endif

	for(i=0; i<len; i++)
	{
		f=ssc*(b+i)-loffs;
		f=BGBMID_SplinePolateLoop(samp->pcm+ss, sm, f);
//		f=v2x(BGBMID_Sample_InterpolatedSampleSubPosition(ctx,
//			samp, f, scd, ss, sm));
		ctx->listen->pcmbuft[i*2+0]+=f*lvol;
//		if(isnan(ctx->listen->pcmbuft[i*2+0]))*(int *)-1=-1;

		f=ssc*(b+i)-roffs;
		f=BGBMID_SplinePolateLoop(samp->pcm+ss, sm, f);
//		f=v2y(BGBMID_Sample_InterpolatedSampleSubPosition(ctx,
//			samp, f, scd, ss, sm));
		ctx->listen->pcmbuft[i*2+1]+=f*rvol;
//		if(isnan(ctx->listen->pcmbuft[i*2+1]))*(int *)-1=-1;
	}

	return(0);
}

BMID_API int BGBMID_BlendNoteSampNoLoop(BGBMID_Context *ctx,
	int chan, int len, int note,
	BGBMID_Sample *samp, float freq)
{
	float vol, lvol, rvol, pan, bal;
	float loffs, roffs;
	float f, g, ssc, scd;
	int i, j, k, b;
	int i0, i1;

//	f=BGBMID_Sound_CalcChanAttenuation(ctx, ctx->chan[chan]);
	f=1.0;
	vol=ctx->chan[chan]->control[7]*f;
	bal=ctx->chan[chan]->control[8]/16384.0;
	pan=ctx->chan[chan]->control[10]/16384.0;

	f=ctx->chan[chan]->notevel[note]/127.0;
	vol*=f;

	vol=pow(vol/16384.0, 1.66)*16384.0;

	lvol=(1.0-bal)*(1.0-pan*0.5);
	rvol=(0.0+bal)*(0.5+pan*0.5);
	g=sqrt(lvol*lvol + rvol*rvol);
	lvol=vol*lvol*(1.0/g);
	rvol=vol*rvol*(1.0/g);

	loffs=pan; roffs=1.0-pan;
	g=sqrt(loffs*loffs + roffs*roffs);
	loffs=loffs*(1.0/g);
	roffs=roffs*(1.0/g);

	b=ctx->listen->pcmbase+(ctx->listen->pcmbufe-ctx->listen->pcmbuf)/2;
	b-=ctx->chan[chan]->note_strt[note];

	ssc=(samp->rate/((float)(ctx->samplerate)))/samp->root;
	ssc*=freq;

	scd=log(1.0+ssc)/log(2.0);
	if(scd<0.0)scd=0.0;
	if(scd>6.9)scd=6.9;

	lvol/=32767.0;
	rvol/=32767.0;

	loffs=loffs*0.0010*samp->rate;
	roffs=roffs*0.0010*samp->rate;

	for(i=0; i<len; i++)
	{
		f=ssc*(b+i);
		if(f>=samp->len)break;

		f=ssc*(b+i)-loffs;
		f=BGBMID_SplinePolate(samp->pcm, samp->len, f);
//		f=v2x(BGBMID_Sample_InterpolatedSamplePosition(ctx, samp, f, scd));
		ctx->listen->pcmbuft[i*2+0]+=f*lvol;

		if(ctx->listen->pcmbuft[i*2+0]<-65536)*(int *)-1=-1;
		if(ctx->listen->pcmbuft[i*2+0]>65536)*(int *)-1=-1;

		f=ssc*(b+i)-roffs;
		f=BGBMID_SplinePolate(samp->pcm, samp->len, f);
//		f=v2y(BGBMID_Sample_InterpolatedSamplePosition(ctx, samp, f, scd));
		ctx->listen->pcmbuft[i*2+1]+=f*rvol;

		if(ctx->listen->pcmbuft[i*2+1]<-65536)*(int *)-1=-1;
		if(ctx->listen->pcmbuft[i*2+1]>65536)*(int *)-1=-1;
	}

	if(i<len)
	{
		ctx->chan[chan]->notevel[note]=0;
	}

	return(0);
}

static double bm_log2(double x)
	{ return(log(x)/log(2)); }

BMID_API int BGBMID_BlendNote(BGBMID_Context *ctx,
	int chan, int len, int note)
{
	BGBMID_Sample *samp, *scur;
	BGBMID_Patch *pat;
	float freq, pbl;
	float d, bd, d0, d1;

	pat=ctx->patches[ctx->chan[chan]->prog];

//	if(!pat)
	if(1)
	{
		BGBMID_BlendNoteTone(ctx, chan, len, note);
		return(0);
	}

	freq=pow(2, (note-69)/12.0)*440;
	pbl=(ctx->chan[chan]->control[31]-8192)/8192.0;
	freq*=pow(2, pbl);

	samp=pat->samp;
	while(samp)
	{
		if((freq>=samp->low) && (freq<=samp->high))
			break;
		samp=samp->next;
	}

	if(!samp)
	{
		scur=pat->samp;
		bd=999999.0;
		while(scur)
		{
			d0=fabs(bm_log2(freq)-bm_log2(scur->low));
			d1=fabs(bm_log2(freq)-bm_log2(scur->high));
			d=(d0<d1)?d0:d1;

			if(d<bd) { samp=scur; bd=d; }
			scur=scur->next;
		}
	}

	if(!samp)
	{
		BGBMID_BlendNoteTone(ctx, chan, len, note);
		return(0);
	}

	if(samp->flags&BGBMID_SFL_LOOP)
		BGBMID_BlendNoteSampLoop(ctx, chan, len, note, samp, freq);
		else BGBMID_BlendNoteSampNoLoop(ctx, chan, len, note, samp, freq);

	return(0);
}

BMID_API int BGBMID_BlendNoteDrum(BGBMID_Context *ctx,
	int chan, int len, int note)
{
	BGBMID_Sample *samp, *scur;
	char *s;

	if(!ctx->drumpatches[note])
	{
		s=BGBMID_MidiDrumPatchName(note);
		ctx->drumpatches[note]=BGBMID_LoadPatch(s, note);
	}

//	if(!pat)
	if(1)
	{
		BGBMID_BlendNoteTone(ctx, chan, len, note);
		return(0);
	}

	if(!ctx->drumpatches[note])
		return(0);

	samp=ctx->drumpatches[note]->samp;

	if(!samp)return(0);

	if(samp->flags&BGBMID_SFL_LOOP)
	{
		BGBMID_BlendNoteSampLoop(ctx, chan, len,
			note, samp, samp->root);
	}else
	{
		BGBMID_BlendNoteSampNoLoop(ctx, chan, len,
			note, samp, samp->root);
	}

	return(0);
}

BMID_API int BGBMID_BlendChannel(BGBMID_Context *ctx, int chan, int samp)
{
	int i, j;

	chan=BGBMID_ChannelMapValidate(ctx, chan);
	if(chan<0)
		return(-1);

	if(!ctx->chan[chan])
		return(-1);

#if 0
	if(ctx->chan[chan]->mix)
	{
		if((ctx->cnTime>ctx->chan[chan]->endTime) &&
			!(ctx->chan[chan]->flags&BGBMID_CHFL_LOOP))
		{
			BGBMID_DeleteChannel(ctx, chan);
			return(-1);
		}
	
		i=BGBMID_BlendMixChannel(ctx, ctx->chan[chan], samp);
		return(i);
	}
#endif

	if(chan==9)
	{
		for(i=0; i<128; i++)
			if(ctx->chan[chan]->notevel[i])
				BGBMID_BlendNoteDrum(ctx, chan, samp, i);
		return(0);
	}

	for(i=0; i<128; i++)
		if(ctx->chan[chan]->notevel[i])
			BGBMID_BlendNote(ctx, chan, samp, i);
	return(0);
}

BMID_API int BGBMID_StopChannel(BGBMID_Context *ctx, int chan)
{
	int i;

	if(chan<0)
	{
		BGBMID_InitChannels(ctx);
		return(0);
	}

	chan=BGBMID_ChannelMapValidate(ctx, chan);
	if(chan<0)return(-1);

	if(chan>=ctx->n_chan)
		return(-1);
	if(!ctx->chan[chan])
		return(-1);

#if 0
	if(ctx->chan[chan]->mix)
	{
		BGBMID_DeleteChannel(ctx, chan);
		return(1);
	}
#endif

	for(i=0; i<128; i++)
		ctx->chan[chan]->notevel[i]=0;

	return(0);
}

BMID_API void BGBMID_DeltaDeblock(BGBMID_Context *ctx, int step)
{
	int i, j, k;

	j=step*ctx->ticksamp;

	for(i=0; i<16; i++)
	{
		k=ctx->listen->pcmbuft[i*2+0]*(i/16.0);
		k+=ctx->listen->pcmbufe[i*2+0];
		if(k>32767)k=32767;
		if(k<-32767)k=-32767;
		ctx->listen->pcmbufe[i*2+0]=k;

		k=ctx->listen->pcmbuft[i*2+1]*(i/16.0);
		k+=ctx->listen->pcmbufe[i*2+1];
		if(k>32767)k=32767;
		if(k<-32767)k=-32767;
		ctx->listen->pcmbufe[i*2+1]=k;
	}

	for(i=16; i<j; i++)
	{
		k=ctx->listen->pcmbuft[i*2+0];
//		k+=ctx->listen->pcmbufe[i*2+0];

		if(k>32767)k=32767;
		if(k<-32767)k=-32767;
		ctx->listen->pcmbufe[i*2+0]=k;

		k=ctx->listen->pcmbuft[i*2+1];
//		k+=ctx->listen->pcmbufe[i*2+1];

		if(k>32767)k=32767;
		if(k<-32767)k=-32767;
		ctx->listen->pcmbufe[i*2+1]=k;
	}

	for(i=0; i<16; i++)
	{
		k=ctx->listen->pcmbuft[(j+i)*2+0]*(1.0-(i/16.0));
		k+=ctx->listen->pcmbufe[(j+i)*2+0];
		if(k>32767)k=32767;
		if(k<-32767)k=-32767;
		ctx->listen->pcmbufe[(j+i)*2+0]=k;

		k=ctx->listen->pcmbuft[(j+i)*2+1]*(1.0-(i/16.0));
		k+=ctx->listen->pcmbufe[(j+i)*2+1];
		if(k>32767)k=32767;
		if(k<-32767)k=-32767;
		ctx->listen->pcmbufe[(j+i)*2+1]=k;
	}
}

BMID_API void BGBMID_DeltaDeblock2(BGBMID_Context *ctx, int step)
{
	int i, j, k;

	j=step*ctx->ticksamp;

	for(i=0; i<16; i++)
	{
		k=ctx->listen->pcmbuft[i*2+0]*(i/16.0)+
			ctx->listen->pcmbufe[i*2+0]*(1.0-(i/16.0));
		if(k>32767)k=32767;
		if(k<-32767)k=-32767;
		ctx->listen->pcmbufe[i*2+0]=k;

		k=ctx->listen->pcmbuft[i*2+1]*(i/16.0)+
			ctx->listen->pcmbufe[i*2+1]*(1.0-(i/16.0));
		if(k>32767)k=32767;
		if(k<-32767)k=-32767;
		ctx->listen->pcmbufe[i*2+1]=k;
	}

	for(i=16; i<(j+16); i++)
	{
		k=ctx->listen->pcmbuft[i*2+0];
//		k+=ctx->listen->pcmbufe[i*2+0];

		if(k>32767)k=32767;
		if(k<-32767)k=-32767;
		ctx->listen->pcmbufe[i*2+0]=k;

		k=ctx->listen->pcmbuft[i*2+1];
//		k+=ctx->listen->pcmbufe[i*2+1];

		if(k>32767)k=32767;
		if(k<-32767)k=-32767;
		ctx->listen->pcmbufe[i*2+1]=k;
	}
}

BMID_API void BGBMID_DeltaReverb(BGBMID_Context *ctx, int step)
{
	static int mipbase[18]=
	{
	     0,  65536,  98304, 114688,	//   0, 65536, 32768, 16384
	122880, 126976, 129024, 130048, //8192,  4096,  2048,  1024
	130560, 130816, 130944, 131008, // 512,   256,   128,    64
	131040, 131056, 131064, 131068, //  32,    16,     8,     4
	131070, 131071
	};

//	float avblk[256];
	float *rvm, *efm;
	float *rvw, *efw;
	float f, g, gl, gr;
	float rvb, rve, efb, efe;
	int i, j, k, n, m, b;

	if(!ctx->listen->env)
		return;

	rvb=ctx->listen->env->rvBase;
	rve=ctx->listen->env->rvEffect;
	efb=ctx->listen->env->efBase;
	efe=ctx->listen->env->efEffect;
	if(	(rvb==1.0) && (rve==0.0) &&
		(efb==1.0) && (efe==0.0))
	{
#if 1
		n=step*ctx->ticksamp;
		b=ctx->listen->pcmbase+(ctx->listen->pcmbufe-ctx->listen->pcmbuf)/2;
		rvw=ctx->listen->pcmbufwin_rv;
		efw=ctx->listen->pcmbufwin_ef;

		for(i=0; i<(n+16); i++)
		{
			gl=ctx->listen->pcmbuft[i*2+0];
			gr=ctx->listen->pcmbuft[i*2+1];
			efw[(b+i)&65535]=(gl+gr)*0.5;
			rvw[(b+i)&65535]=(gl+gr)*0.5;
		}
#endif
		return;
	}
	
	n=step*ctx->ticksamp;
	b=ctx->listen->pcmbase+(ctx->listen->pcmbufe-ctx->listen->pcmbuf)/2;
	rvw=ctx->listen->pcmbufwin_rv;
	efw=ctx->listen->pcmbufwin_ef;

	rvm=ctx->listen->env->rvMat;
	efm=ctx->listen->env->efMat;

#if 0
	for(j=0; j<256; j++)
	{
		g=0; m=j+1;
		for(k=0; k<m; k++)
			g+=rvw[(b+i-j*j-k-1)&65535];
		g=g/m;
		avblk[j]=g;
	}
#endif

#if 0
	for(i=0; i<(n+16); i++)
	{
		gl=ctx->listen->pcmbuft[i*2+0];
		gr=ctx->listen->pcmbuft[i*2+1];
		efw[(b+i)&65535]=(gl+gr)*0.5;
	}
#endif

//	if((efb!=1.0) || (efe!=0.0))
	if((fabs(efb-1.0)>0.1) || (fabs(efe)>0.1))
	{
		for(i=0; i<(n+16); i++)
		{
			f=0;
			for(j=0; j<256; j++)
			{
				f+=efm[j]*efw[(b+i-j-1)&65535];
			}

//			gl=ctx->listen->pcmbuft[i*2+0]+f;
//			gr=ctx->listen->pcmbuft[i*2+1]+f;
//			ctx->listen->pcmbuft[i*2+0]=gl;
//			ctx->listen->pcmbuft[i*2+1]=gr;
		
//			if(fabs(f*efe)>4096.0)
//				{ f*=sqrt(4096.0/fabs(f*efe)); }
		
			gl=ctx->listen->pcmbuft[i*2+0];
			gr=ctx->listen->pcmbuft[i*2+1];
			ctx->listen->pcmbuft[i*2+0]=(gl*efb)+(f*efe);
			ctx->listen->pcmbuft[i*2+1]=(gr*efb)+(f*efe);
			efw[(b+i)&65535]=(gl+gr)*0.5;
		}
	}else
	{
#if 1
		for(i=0; i<(n+16); i++)
		{
			gl=ctx->listen->pcmbuft[i*2+0];
			gr=ctx->listen->pcmbuft[i*2+1];
			efw[(b+i)&65535]=(gl+gr)*0.5;
		}
#endif
	}

#if 0
	for(i=0; i<(n+16); i++)
	{
		gl=ctx->listen->pcmbuft[i*2+0];
		gr=ctx->listen->pcmbuft[i*2+1];
		efw[(b+i)&65535]=(gl+gr)*0.5;
		rvw[(b+i)&65535]=(gl+gr)*0.5;
	}
#endif

//	if((rvb!=1.0) || (rve!=0.0))
	if((fabs(rvb-1.0)>0.1) || (fabs(rve)>0.1))
	{
		for(i=0; i<(n+16); i++)
		{
			f=0;
			
			if(fabs(rve)>0.75)
			{
				for(j=0; j<256; j++)
					{ f+=rvm[j]*rvw[(b+i-j*j-1)&65535]; }
			}else if(fabs(rve)>0.5)
			{
				for(j=0; j<256; j+=2)
					{ f+=rvm[j]*rvw[(b+i-j*j-1)&65535]; }
				f*=2;
			}else
			{
				for(j=0; j<256; j+=4)
					{ f+=rvm[j]*rvw[(b+i-j*j-1)&65535]; }
				f*=4;
			}

//			gl=ctx->listen->pcmbuft[i*2+0]+f;
//			gr=ctx->listen->pcmbuft[i*2+1]+f;
//			ctx->listen->pcmbuft[i*2+0]=gl;
//			ctx->listen->pcmbuft[i*2+1]=gr;

//			if(fabs(f*rve)>4096.0)
//				{ f*=sqrt(4096.0/fabs(f*rve)); }
		
			gl=ctx->listen->pcmbuft[i*2+0];
			gr=ctx->listen->pcmbuft[i*2+1];
			ctx->listen->pcmbuft[i*2+0]=(gl*rvb)+(f*rve);
			ctx->listen->pcmbuft[i*2+1]=(gr*rvb)+(f*rve);
			rvw[(b+i)&65535]=(gl+gr)*0.5;
		}
	}else
	{
#if 1
		for(i=0; i<(n+16); i++)
		{
			gl=ctx->listen->pcmbuft[i*2+0];
			gr=ctx->listen->pcmbuft[i*2+1];
			rvw[(b+i)&65535]=(gl+gr)*0.5;
		}
#endif
	}

#if 0
	//update mip levels
	for(i=1; i<18; i++)
	{
		m=(n+16)>>i;
		for(j=0; j<m; j++)
		{
			k=((b+(j<<i))&65535)>>i;
			f=rvw[mipbase[i-1]+k*2+0];
			g=rvw[mipbase[i-1]+k*2+1];
			f=(f+g)*0.5;
			if(f<-32767)f=-32767;
			if(f>32767)f=32767;
			rvw[mipbase[i]+k]=f;
		}
	}
#endif

#if 0
	//update mip levels
	for(i=1; i<18; i++)
	{
		m=65536>>i;
		for(j=0; j<m; j++)
		{
			f=rvw[mipbase[i-1]+j*2+0];
			g=rvw[mipbase[i-1]+j*2+1];
			f=(f+g)*0.5;
			if(f<-32767)f=-32767;
			if(f>32767)f=32767;
			rvw[mipbase[i]+j]=f;
		}
	}
#endif
}

BMID_API int BGBMID_DeltaTime(BGBMID_Context *ctx, int step)
{
	int i, j, k;

	if(!step)return(0);

	//pitch=69+12*log2(freq/440)
	//freq=2^((pitch-69)/12)*440

	j=step*ctx->ticksamp;

	//if output is full, do what is possible and return remainder
	k=(ctx->listen->pcmbufend-ctx->listen->pcmbufe)/2;
	if((j+64)>k)
	{
		printf("DeltaTime: %d %d\n", j, k);

		j=(k-64)/ctx->ticksamp;
		BGBMID_DeltaTime(ctx, j);
		return(step-j);
	}

#if 1
	if(ctx->listen->pcmrqbufe)
	{
		j=(step-1)*ctx->ticksamp;
		k=(ctx->listen->pcmrqbufe-ctx->listen->pcmbufe)/2;

		if(j>k)
		{
			j=(k+ctx->ticksamp-1)/ctx->ticksamp;
			BGBMID_DeltaTime(ctx, j);
			return(step-j);
		}
	}
#endif

	//mixing too much, split up mixing
	if((j+64)>ctx->tempsz)
	{
		if(step<2)return(0);

		i=step/2;
		BGBMID_DeltaTime(ctx, i);
		BGBMID_DeltaTime(ctx, step-i);
		return(0);
	}

	if(!ctx->listen->pcmbuft)
		ctx->listen->pcmbuft=bgbmid_malloc(ctx->tempsz*2*sizeof(float));

	if(!ctx->listen->pcmbufwin_rv)
		ctx->listen->pcmbufwin_rv=bgbmid_malloc(65536*sizeof(float));
	if(!ctx->listen->pcmbufwin_ef)
		ctx->listen->pcmbufwin_ef=bgbmid_malloc(65536*sizeof(float));

	for(i=0; i<(j+16); i++)
	{
		ctx->listen->pcmbuft[i*2+0]=0;
		ctx->listen->pcmbuft[i*2+1]=0;
	}

	for(i=0; i<ctx->n_chan; i++)
	{
		BGBMID_BlendChannel(ctx, i, j+16);
	}

//	BGBMID_UpdateStreams(ctx, j);

	BGBMID_DeltaReverb(ctx, step);

	if(!ctx->listen->pcmrqbufe)
//	if(1)
	{
		BGBMID_DeltaDeblock(ctx, step);
	}else
	{
		BGBMID_DeltaDeblock2(ctx, step);
	}

	ctx->listen->pcmbufe+=2*j;
	ctx->cnTime+=((double)j)/ctx->samplerate;

	return(0);
}

BMID_API int BGBMID_NoteOn(BGBMID_Context *ctx,
	int chan, int note, int vel)
{
	if(vel==0)
	{
		BGBMID_NoteOff(ctx, chan, note, vel);
		return(0);
	}

	ctx->chan[chan]->notevel[note]=vel;
	ctx->chan[chan]->note_strt[note]=
		(ctx->listen->pcmbufe-ctx->listen->pcmbuf)/2;
	ctx->chan[chan]->note_end[note]=0;
	return(0);
}

BMID_API int BGBMID_NoteOff(BGBMID_Context *ctx,
	int chan, int note, int vel)
{
//	ctx->chan[chan]->notevel[note]=0;
	ctx->chan[chan]->note_end[note]=
		(ctx->listen->pcmbufe-ctx->listen->pcmbuf)/2;
	return(0);
}

BMID_API int BGBMID_NoteAftertouch(BGBMID_Context *ctx,
	int chan, int note, int val)
{
	return(0);
}

BMID_API int BGBMID_Controller(BGBMID_Context *ctx,
	int chan, int num, int val)
{
	if((num!=7) && (num!=8) && (num!=10))
//	if(1)
	{
		printf("Controller Ch=%d Num=%d Val=%d\n", chan, num, val);
	}

	if((num&0x60)==0x00)
	{
		ctx->chan[chan]->control[num&0x1F]&=0x007F;
		ctx->chan[chan]->control[num&0x1F]|=(val<<7);
		return(0);
	}
	if((num&0x60)==0x20)
	{
		ctx->chan[chan]->control[num&0x1F]&=0x3F80;
		ctx->chan[chan]->control[num&0x1F]|=val;
		return(0);
	}
	
	switch(num)
	{
	case BGBMID_CONTROL_FLAGS_LSB:
		ctx->chan[chan]->flags&=0x3F80;
		ctx->chan[chan]->flags|=val;		
		break;
	case BGBMID_CONTROL_FLAGS_MSB:
		ctx->chan[chan]->flags&=0x007F;
		ctx->chan[chan]->flags|=(val<<7);
		break;
	default:
		break;
	}
	return(0);
}

BMID_API int BGBMID_ProgramChange(BGBMID_Context *ctx, int chan, int num)
{
	char *s;
	int i, j, k;

	if(num<0)return(-1);
	if(num>=ctx->maxpatches)
	{
		i=ctx->maxpatches;
		while(num>=i)i=i+(i>>1);
		ctx->patches=bgbmid_realloc(ctx->patches, i*sizeof(char *));
		j=ctx->maxpatches; k=i-j;
		memset(ctx->patches+j, 0, k*sizeof(char *));
		ctx->maxpatches=i;
	}

	if(!ctx->patches[num])
	{
//		s=BGBMID_MidiPatchName(num);
		s=BGBMID_GetPatchName(num);
		ctx->patches[num]=BGBMID_LoadPatch(s, num);

		if(!ctx->patches[num])
		{
			ctx->patches[num]=BGBMID_LoadPatchDummy(s);
		}
	}

	BGBMID_CheckInitMidiChannel(ctx, chan);
	ctx->chan[chan]->prog=num;

	return(0);
}

BMID_API int BGBMID_ProgramChange2(BGBMID_Context *ctx, int chan, int num)
{
	int i;

	if(ctx->chan[chan])
		{ i=(ctx->chan[chan]->control[0]*128)+num; }
	else
		{ i=num; }

	return(BGBMID_ProgramChange(ctx, chan, i));
}

BMID_API int BGBMID_ChannelAftertouch(BGBMID_Context *ctx,
	int chan, int val)
{
	return(0);
}

BMID_API int BGBMID_PitchBlend(BGBMID_Context *ctx, int chan, int val)
{
	ctx->chan[chan]->control[31]=val;
	return(0);
}

BMID_API int BGBMID_ProgramDefinePatch(
	BGBMID_Context *ctx, int num, char *name)
{
	char *s;
	int i, j, k;

	if(num<0)return(-1);
	if(num>=ctx->maxpatches)
	{
		i=ctx->maxpatches;
		while(num>=i)i=i+(i>>1);
		ctx->patches=bgbmid_realloc(ctx->patches, i*sizeof(char *));
		j=ctx->maxpatches; k=i-j;
		memset(ctx->patches+j, 0, k*sizeof(char *));
		ctx->maxpatches=i;
	}

	if(!ctx->patches[num])
	{
//		s=BGBMID_MidiPatchName(num);
//		s=BGBMID_GetPatchName(num);
		ctx->patches[num]=BGBMID_LoadPatch(name, num);

		if(!ctx->patches[num])
		{
			s=name;
			ctx->patches[num]=BGBMID_LoadPatchDummy(s);
		}
	}
//	ctx->chan[chan]->prog=num;

	return(0);
}

#if 0
BMID_API int BGBMID_PlayMixSample(BGBMID_Context *ctx, int chan, int num)
{
	char *s;
	int i, j, k;

	if(num<0)return(-1);
	if(num>=ctx->maxpatches)
	{
		i=ctx->maxpatches;
		while(num>=i)i=i+(i>>1);
		ctx->patches=bgbmid_realloc(ctx->patches, i*sizeof(char *));
		j=ctx->maxpatches; k=i-j;
		memset(ctx->patches+j, 0, k*sizeof(char *));
		ctx->maxpatches=i;
	}

	if(!ctx->patches[num])
	{
//		s=BGBMID_MidiPatchName(num);
		s=BGBMID_GetPatchName(num);
		ctx->patches[num]=BGBMID_LoadPatch(s, num);

		if(!ctx->patches[num])
		{
			ctx->patches[num]=BGBMID_LoadPatchDummy(s);
		}
	}

	BGBMID_CheckInitChannel(ctx, chan);
//	ctx->chan[chan]->prog=num;

//	BGBMID_InitMixChannel(ctx, ctx->chan[chan],
//		ctx->patches[num]->samp);

//	V3F_ZERO(ctx->chan[chan]->org);
//	V3F_ZERO(ctx->chan[chan]->vel);
//	Mat3F_Identity(ctx->chan[chan]->rot);

	ctx->chan[chan]->seq=(ctx->sequence++)&16383;
	if(!ctx->chan[chan]->seq)
		{ ctx->chan[chan]->seq=(ctx->sequence++)&16383; }
	i=chan|(ctx->chan[chan]->seq<<16);
	return(i);
}
#endif

BMID_API int BGBMID_ChannelMapValidate(BGBMID_Context *ctx, int chan)
{
	int seq;
	
	seq=(chan>>16);
	chan&=65535;

	if((chan<0) || (chan>ctx->n_chan))
		return(-1);
	if(!ctx->chan[chan])
		return(-1);
	if(seq && (seq!=ctx->chan[chan]->seq))
		return(-1);
	return(chan);
}

BMID_API void BGBMID_Controller2fv(BGBMID_Context *ctx,
	int chan, int num, float *val)
{
	BGBMID_Controller4fv(ctx, chan, num, val);
}

BMID_API void BGBMID_Controller3fv(BGBMID_Context *ctx,
	int chan, int num, float *val)
{
	BGBMID_Controller4fv(ctx, chan, num, val);
}

BMID_API void BGBMID_Controller4fv(BGBMID_Context *ctx,
	int chan, int num, float *val)
{
	chan=BGBMID_ChannelMapValidate(ctx, chan);
	if(chan<0)
		return;

	switch(num)
	{
	case BGBMID_CONTROL_VOLUME:
		ctx->chan[chan]->vol=val[0];
		break;
	case BGBMID_CONTROL_ORIGIN_3D:
		ctx->chan[chan]->orgTime=ctx->cnTime;
//		V3F_COPY(val, ctx->chan[chan]->org);
		break;
	case BGBMID_CONTROL_VELOCITY_3D:
//		V3F_COPY(val, ctx->chan[chan]->vel);
		break;
	case BGBMID_CONTROL_ROTATION_3D:
//		QuatF_To3Matrix(val, ctx->chan[chan]->rot);
		break;
	}
}

BMID_API void BGBMID_Controller1f(BGBMID_Context *ctx,
	int chan, int num, float val)
{
	float tv[4]; tv[0]=val; tv[1]=0; tv[2]=0; tv[3]=0;
	BGBMID_Controller4fv(ctx, chan, num, tv);
}

BMID_API void BGBMID_Controller2f(BGBMID_Context *ctx,
	int chan, int num, float x, float y)
{
	float tv[4]; tv[0]=x; tv[1]=y; tv[2]=0; tv[3]=0;
	BGBMID_Controller4fv(ctx, chan, num, tv);
}

BMID_API void BGBMID_Controller3f(BGBMID_Context *ctx,
	int chan, int num, float x, float y, float z)
{
	float tv[4]; tv[0]=x; tv[1]=y; tv[2]=z; tv[3]=0;
	BGBMID_Controller4fv(ctx, chan, num, tv);
}

BMID_API void BGBMID_Controller4f(BGBMID_Context *ctx,
	int chan, int num, float x, float y, float z, float w)
{
	float tv[4]; tv[0]=x; tv[1]=y; tv[2]=z; tv[3]=w;
	BGBMID_Controller4fv(ctx, chan, num, tv);
}

BMID_API void BGBMID_Controller1i(BGBMID_Context *ctx,
	int chan, int num, int val)
{
//	float tv[4]; tv[0]=x; tv[1]=0; tv[2]=0; tv[3]=0;
//	BGBMID_Controller4fv(ctx, chan, num, tv);

	chan=BGBMID_ChannelMapValidate(ctx, chan);
	if(chan<0)
		return;

	switch(num)
	{
	case BGBMID_CONTROL_FLAGS:
		ctx->chan[chan]->flags=val;
		break;
	default:
		break;
	}
}

BMID_API void BGBMID_Controller2i(BGBMID_Context *ctx,
	int chan, int num, int x, int y)
{
}

BMID_API void BGBMID_Controller3i(BGBMID_Context *ctx,
	int chan, int num, int x, int y, int z)
{
}

BMID_API void BGBMID_Controller4i(BGBMID_Context *ctx,
	int chan, int num, int x, int y, int z, int w)
{
}

BMID_API void BGBMID_GetChannelVectorNfv(BGBMID_Context *ctx,
	int chan, int num, float *val, int sz)
{
	int i, j, k;

	chan=BGBMID_ChannelMapValidate(ctx, chan);
	if(chan<0)
		return;

	switch(num)
	{
	case BGBMID_VECTOR_HISTOGRAM:
		if(!ctx->chan[chan]->mix)
			break;
		ctx->chan[chan]->flags|=BGBMID_CHFL_CALCCURDCT;
		for(i=0; i<sz; i++)
			{ val[i]=ctx->chan[chan]->mix->hist[i]; }
		break;
	case BGBMID_VECTOR_CURRENT_VOLUME:
		if(!ctx->chan[chan]->mix)
			break;
		ctx->chan[chan]->flags|=BGBMID_CHFL_CALCCURVOL;
		val[0]=ctx->chan[chan]->mix->cur_absvol;
		val[1]=ctx->chan[chan]->mix->cur_relvol_lf;
		val[2]=ctx->chan[chan]->mix->cur_relvol_rt;
//		for(i=0; i<sz; i++)
//			{ val[i]=ctx->chan[chan]->hist[i]; }
		break;
	default:
		break;
	}
}

BMID_API void BGBMID_GetProgramVectorNfv(BGBMID_Context *ctx,
	int prg, int num, float *val, int sz)
{
	BGBMID_Patch *pat;
	char *s;
	int i, j, k;

	if(!ctx->patches[prg])
	{
//		s=BGBMID_MidiPatchName(num);
		s=BGBMID_GetPatchName(prg);
		ctx->patches[prg]=BGBMID_LoadPatch(s, prg);

		if(!ctx->patches[prg])
			{ ctx->patches[prg]=BGBMID_LoadPatchDummy(s); }
	}
	
	pat=ctx->patches[prg];

	switch(num)
	{
	case BGBMID_VECTOR_HISTOGRAM:
		if(pat->samp->pcm)
		{
			BGBMID_Wave_GenerateHistogram(val,
				pat->samp->pcm, pat->samp->len);
		}
		break;
	default:
		break;
	}
}

//0=OK, 1=input empty, 2=output full, -1=stream error
BMID_API int BGBMID_SynthStep(BGBMID_Context *ctx)
{
	static int ld0, ld1;
	byte *cs, *ce;
	char *ifn, *ofn;
	char *s;
	float f, g;
	int tg, d0, d1, d2, d3, d4, op, ch, ctrl;
	int i, j, k;

	cs=ctx->ip;
	ce=ctx->ipe;

//	if(cs>=ce)return(1);
	if((cs>=ce) && (!ctx->not_dt))return(1);

	if(!ctx->not_dt)
	{
		i=BGBMID_TryDecVLI(&cs, ce);
//		printf("Delay %d\n", i);
		if(i<0)return(1);
		ctx->dt+=i;
		ctx->not_dt=1;
		ctx->ip=cs;
		return(0);
	}

	i=BGBMID_DeltaTime(ctx, ctx->dt);
	ctx->cdt+=ctx->dt-i;
	ctx->dt=i;
	if(i)
	{
		j=i*ctx->ticksamp;
		k=(ctx->listen->pcmbufend-ctx->listen->pcmbufe)/2;
		if((j+64)>k)
			return(2);
		return(0);
	}

	if(cs>=ce)
	{
		ctx->not_dt=0;
		return(0);
	}

#if 0
	if(cs[0]==0xFD)
	{
		if((cs+3)>ce)return(1);

		cs++;
		i=*cs++;

		if((i>=0x08) && (i<=0x0E))
		{
			ch=BGBMID_TryDecSVLI(&cs, ce);
			d0=BGBMID_TryDecSVLI(&cs, ce);
			if(i<0x0C)
				d1=BGBMID_TryDecSVLI(&cs, ce);
		}

		switch(i)
		{
		case 0x01:
			j=BGBMID_TryDecSVLI(&cs, ce);
			s=(char *)cs; cs+=strlen(s)+1;
			BGBMID_ProgramDefinePatch(ctx, j, s);
			break;
		case 0x02:
			ch=BGBMID_TryDecSVLI(&cs, ce);
			d0=BGBMID_TryDecSVLI(&cs, ce);
//			BGBMID_PlayMixSample(ctx, ch, d0);
			break;

		case 0x06:
			ch=BGBMID_TryDecSVLI(&cs, ce);
//			BGBMID_DeleteChannel(ctx, ch);
			break;
		case 0x07:
			ch=BGBMID_TryDecSVLI(&cs, ce);
			BGBMID_DeleteChannel(ctx, ch);
			break;

		case 0x08:
			BGBMID_NoteOff(ctx, ch, d0, d1);
			break;
		case 0x09:
			BGBMID_NoteOn(ctx, ch, d0, d1);
			break;
		case 0x0A:
			BGBMID_NoteAftertouch(ctx, ch, d0, d1);
			break;
		case 0x0B:
			BGBMID_Controller(ctx, ch, d0, d1);
			break;
		case 0x0C:
			BGBMID_ProgramChange(ctx, ch, d0);
			break;
		case 0x0D:
			BGBMID_ChannelAftertouch(ctx, ch, d0);
			break;
		case 0x0E:
			BGBMID_PitchBlend(ctx, ch, d0);
			break;

		case 0x10:
			ch=BGBMID_TryDecSVLI(&cs, ce);
			ctrl=BGBMID_TryDecSVLI(&cs, ce);
			d0=BGBMID_TryDecSVLI(&cs, ce);
			BGBMID_Controller1i(ctx, ch, ctrl, d0);
			break;
		case 0x11:
			ch=BGBMID_TryDecSVLI(&cs, ce);
			ctrl=BGBMID_TryDecSVLI(&cs, ce);
			d0=BGBMID_TryDecSVLI(&cs, ce);
			d1=BGBMID_TryDecSVLI(&cs, ce);
			BGBMID_Controller2i(ctx, ch, ctrl, d0, d1);
			break;
		case 0x12:
			ch=BGBMID_TryDecSVLI(&cs, ce);
			ctrl=BGBMID_TryDecSVLI(&cs, ce);
			d0=BGBMID_TryDecSVLI(&cs, ce);
			d1=BGBMID_TryDecSVLI(&cs, ce);
			d2=BGBMID_TryDecSVLI(&cs, ce);
			BGBMID_Controller3i(ctx, ch, ctrl, d0, d1, d2);
			break;
		case 0x13:
			ch=BGBMID_TryDecSVLI(&cs, ce);
			ctrl=BGBMID_TryDecSVLI(&cs, ce);
			d0=BGBMID_TryDecSVLI(&cs, ce);
			d1=BGBMID_TryDecSVLI(&cs, ce);
			d2=BGBMID_TryDecSVLI(&cs, ce);
			d3=BGBMID_TryDecSVLI(&cs, ce);
			BGBMID_Controller4i(ctx, ch, ctrl, d0, d1, d2, d3);
			break;
		case 0x14:
			ch=BGBMID_TryDecSVLI(&cs, ce);
			ctrl=BGBMID_TryDecSVLI(&cs, ce);
			d0=BGBMID_TryDecSVLI(&cs, ce);
			d4=BGBMID_TryDecSVLI(&cs, ce); f=pow(2, d4);
			BGBMID_Controller1f(ctx, ch, ctrl, d0*f);
			break;
		case 0x15:
			ch=BGBMID_TryDecSVLI(&cs, ce);
			ctrl=BGBMID_TryDecSVLI(&cs, ce);
			d0=BGBMID_TryDecSVLI(&cs, ce);
			d1=BGBMID_TryDecSVLI(&cs, ce);
			d4=BGBMID_TryDecSVLI(&cs, ce); f=pow(2, d4);
			BGBMID_Controller2f(ctx, ch, ctrl, d0*f, d1*f);
			break;
		case 0x16:
			ch=BGBMID_TryDecSVLI(&cs, ce);
			ctrl=BGBMID_TryDecSVLI(&cs, ce);
			d0=BGBMID_TryDecSVLI(&cs, ce);
			d1=BGBMID_TryDecSVLI(&cs, ce);
			d2=BGBMID_TryDecSVLI(&cs, ce);
			d4=BGBMID_TryDecSVLI(&cs, ce); f=pow(2, d4);
			BGBMID_Controller3f(ctx, ch, ctrl, d0*f, d1*f, d2*f);
			break;
		case 0x17:
			ch=BGBMID_TryDecSVLI(&cs, ce);
			ctrl=BGBMID_TryDecSVLI(&cs, ce);
			d0=BGBMID_TryDecSVLI(&cs, ce);
			d1=BGBMID_TryDecSVLI(&cs, ce);
			d2=BGBMID_TryDecSVLI(&cs, ce);
			d3=BGBMID_TryDecSVLI(&cs, ce);
			d4=BGBMID_TryDecSVLI(&cs, ce); f=pow(2, d4);
			BGBMID_Controller4f(ctx, ch, ctrl, d0*f, d1*f, d2*f, d3*f);
			break;

		default:
			break;
		}
		return(0);
	}

	if(cs[0]==0xFF)
	{
		if((cs+3)>ce)return(1);
		i=*cs++;
		i=*cs++;
		j=BGBMID_TryDecVLI(&cs, ce);
		if(j<0)return(1);
		if((cs+j)>ce)return(1);

		switch(i)
		{
		case 1: case 2:
		case 3: case 4:
		case 5: case 6:
		case 7:
			cs+=j;
			break;

		case 47:
			BGBMID_InitChannels(ctx);
			cs+=j;
			break;

		case 81:
			k=(cs[0]<<16)|(cs[1]<<8)|cs[2];
			cs+=j;

			BGBMID_SetNoteLen(ctx, k/1000000.0);

//			ctx->ticksamp=((float)(ctx->samplerate))*
//				((k/1000000.0)/ctx->noteticks);
//			ctx->ticksamp*=ctx->ratesc;
//			printf("Ticksamp %d %f\n", k, ctx->ticksamp);
			break;

		default:
			cs+=j;
			break;
		}

		ctx->not_dt=0;
		ctx->ip=cs;
		return(0);
	}

	if(cs[0]==0xF0)
	{
		i=*cs++;
		while(1)
		{
			if(cs>=ce)return(1);
			i=*cs++;
			if(i&0x80)break;
		}

		ctx->not_dt=0;
		ctx->ip=cs;
		return(0);
	}
#endif

	i=*cs++;

#if 0
	if((i&0xF0)==0xF0)
	{
//		printf("DT=%d Tag=%02X\n", dt, i);
		return(-1);
	}
#endif

//	printf("@%d DT=%d Tag=%02X\n", ctx->cdt, ctx->dt, i);

#if 0
	if(i&0x80)
	{
		tg=i;
		op=(tg>>4); ch=tg&15;
		d0=*cs++;
		if((op!=12) && (op!=13))
			d1=*cs++;
			else d1=0;
		ctx->ltag=tg;
	}else
	{
		tg=ctx->ltag;
		op=(tg>>4); ch=tg&15;
		d0=i;
		if((op!=12) && (op!=13))
			d1=*cs++;
			else d1=0;
	}
#endif

#if 1
	tg=i;
	op=(tg>>4)&7;
	ch=tg&15;
	d0=*cs++;
//	d0=BGBMID_TryDecVLI(&cs, ce);
	if((op!=0) && (op!=2) && (op!=3) && (op!=7))
	{
		if(op==1)
		{
			if(d0&128)
			{
				d0&=127;
				d1=*cs++;
			}else
			{
				d1=ld1;
			}

			ld0=d0;
			ld1=d1;
		}else
		{
			d1=*cs++;
//			d1=BGBMID_TryDecVLI(&cs, ce);
		}
	}else
	{
		d1=0;
	}
	ctx->ltag=tg;
	if(i&128)
		ctx->not_dt=0;
#endif

	if(op==6)return(1);

	if(cs>ce)return(1);

//	printf("\tOp=%01X Ch=%01X D0=%02X D1=%02X\n",
//		op, ch, d0, d1);

#if 0
	switch(op)
	{
	case 8: BGBMID_NoteOff(ctx, ch, d0, d1); break;
	case 9: BGBMID_NoteOn(ctx, ch, d0, d1); break;
	case 10: BGBMID_NoteAftertouch(ctx, ch, d0, d1); break;
	case 11: BGBMID_Controller(ctx, ch, d0, d1); break;
	case 12: BGBMID_ProgramChange2(ctx, ch, d0); break;
	case 13: BGBMID_ChannelAftertouch(ctx, ch, d0); break;
	case 14: BGBMID_PitchBlend(ctx, ch, (d1<<7)|d0); break;
	}
#endif

#if 1
	switch(op)
	{
	case 0:
		BGBMID_NoteOff(ctx, ch, d0, d1);
		break;
	case 1:
		BGBMID_NoteOn(ctx, ch, d0, d1);
		break;
	case 2:
		BGBMID_PitchBlend(ctx, ch, d0<<6);
		break;
//		BGBMID_PitchBlend(ctx, ch, (d1<<7)|d0); break;

	case 3:
		switch(d0)
		{
		case 10: BGBMID_Controller(ctx, ch, 120, 1);		break;
		case 11: BGBMID_Controller(ctx, ch, 123, 1);		break;
		case 12: BGBMID_Controller(ctx, ch, 126, 1);		break;
		case 13: BGBMID_Controller(ctx, ch, 127, 1);		break;
		case 14: BGBMID_Controller(ctx, ch, 121, 1);		break;
		}
		break;

	case 4:
		switch(d0)
		{
		case 0:		BGBMID_ProgramChange2(ctx, ch, d1); break;
		case 1:		BGBMID_Controller(ctx, ch,  0, d1); break;
		case 2:		BGBMID_Controller(ctx, ch,  1, d1); break;
		case 3:		BGBMID_Controller(ctx, ch,  7, d1); break;
		case 4:		BGBMID_Controller(ctx, ch, 10, d1); break;
		case 5:		BGBMID_Controller(ctx, ch, 11, d1); break;
		case 6:		BGBMID_Controller(ctx, ch, 91, d1); break;
		case 7:		BGBMID_Controller(ctx, ch, 93, d1); break;
		case 8:		BGBMID_Controller(ctx, ch, 64, d1); break;
		case 9:		BGBMID_Controller(ctx, ch, 67, d1); break;
		}
		break;
//		if(d0==0)
//			{ BGBMID_ProgramChange2(ctx, ch, d1); break; }
//		BGBMID_Controller(ctx, ch, d0, d1);		break;

	case 7:
		break;

//	case 10: BGBMID_NoteAftertouch(ctx, ch, d0, d1); break;
//	case 12: BGBMID_ProgramChange2(ctx, ch, d0); break;
//	case 13: BGBMID_ChannelAftertouch(ctx, ch, d0); break;
	default:
		printf("\tUnknown Op %d\n", op);

	}
#endif

//	ctx->not_dt=0;
	ctx->ip=cs;
	return(0);
}

BMID_API int BGBMID_DoSynth(BGBMID_Context *ctx)
{
	int i;

	while(1)
	{
		i=BGBMID_SynthStep(ctx);
		if(i)break;
	}

	return(i);
}

BMID_API int BGBMID_SynthSamples2(
	BGBMID_Context *ctx, short *buf, int len, int ovlen)
{
	int i, j, k;

	while(1)
	{
		j=(ctx->listen->pcmbufe-ctx->listen->pcmbuf)/2;
		if(j>ovlen)
		{
//			printf("A");
			memcpy(buf, ctx->listen->pcmbuf, ovlen*2*2);
//			memmove(ctx->listen->pcmbuf, ctx->listen->pcmbuf+len*2,
//				(j-len+64)*2*2);
			memcpy(ctx->listen->pcmbuf, ctx->listen->pcmbuf+len*2,
				(2*j)*2*2);
			ctx->listen->pcmbufe=ctx->listen->pcmbuf+(j-len)*2;
//			memset(ctx->listen->pcmbufe+16*2, 0, len*2*2);
//			memset(ctx->listen->pcmbufe+256*2, 0, len*2*2);
			ctx->listen->pcmbase+=len;
			return(len);
		}

		ctx->listen->pcmrqbufe=ctx->listen->pcmbufe+ovlen*2;

//		printf("B");
		i=BGBMID_SynthStep(ctx);
		if(!i)continue;

//		printf("B");

		if(i==1)
		{
//			printf("C");
			j=ceil(((double)len)/ctx->ticksamp);
			if(!j)j=1;
			BGBMID_DeltaTime(ctx, j);
			continue;
		}

		if(i==2)
		{
//			printf("D");
			memcpy(buf, ctx->listen->pcmbuf, j*2*2);
			memmove(ctx->listen->pcmbuf, ctx->listen->pcmbufe, 16*2*2);
			ctx->listen->pcmbufe=ctx->listen->pcmbuf;
			memset(ctx->listen->pcmbufe+16*2, 0, j*2*2);

			buf+=j*2;
			len-=j;
			ovlen-=j;
			continue;
		}

		if(i)
		{
//			printf("E");
			break;
		}
	}

	return(len);
}

BMID_API int BGBMID_SynthSamples(BGBMID_Context *ctx, short *buf, int len)
	{ return(BGBMID_SynthSamples2(ctx, buf, len, len)); }
