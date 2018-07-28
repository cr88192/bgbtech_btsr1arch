#include <bgbmid.h>

BMID_API void BGBMID_Sound_MixPopReduceStereo(short *buffer, int cnt)
{
	float f;
	int i, j, k, l;
	
	k=((int)buffer[-2]+(int)buffer[0])/2;
	l=((int)buffer[-1]+(int)buffer[1])/2;

	for(i=0; i<cnt; i++)
	{
		f=i/(float)cnt;
		buffer[i*2+0]=(buffer[i*2+0]*f)+(k*(1.0-f));
		buffer[i*2+1]=(buffer[i*2+1]*f)+(l*(1.0-f));
		buffer[-i*2+0]=(buffer[-i*2+0]*f)+(k*(1.0-f));
		buffer[-i*2+1]=(buffer[-i*2+1]*f)+(l*(1.0-f));
	}
}

BMID_API void BGBMID_Sound_MixPopReduceStereo2(
	short *buffer, int size, int offs, int cnt)
{
	if(offs<(2*cnt))return;
	if((offs+(2*cnt))>=size)return;
	BGBMID_Sound_MixPopReduceStereo(buffer+offs, cnt);
}

float BGBMID_Sound_CalcDoppler(
	float *lorg, float *lvel, float *sorg, float *svel)
{
	float vl, vs, r, c;
	float nv[3];
	int i;

//	vl=Vl*N(Os-Ol)
//	vs=Vs*N(Ol-Os)

	V3F_SUB(sorg, lorg, nv);
	V3F_NORMALIZE(nv, nv);
	vl=V3F_DOT(lvel, nv);
	vs=V3F_DOT(svel, nv);

//	r=((c+vl)/(c-vs))
//	Fl=Fs*r
//	Fs=Fl/r

	c=BGBMID_SOUND_C;
	r=((c+vl)/(c-vs));

//	Ll=Ls/r
//	Ls=Ll*r

	return(r);
}

BMID_API float BGBMID_Sound_CalcChanAttenuation(
	BGBMID_Context *ctx, BGBMID_Channel *chan)
{
	BGBMID_AttParams att;
	
	att=BGBMID_Sound_CalcChanAttParms(ctx, chan, 1470);
	return(att.vol);
}

BMID_API BGBMID_AttParams BGBMID_Sound_CalcChanAttParms(
	BGBMID_Context *ctx, BGBMID_Channel *chan, int samp)
{
	BGBMID_AttParams att;
	float dira[3], dirb[3];
	float orga[3], orgb[3];
	float lorga[3], lorgb[3];
	float d, df, vt, vtf, vprt, vpfw, vpup, ds;
	float f, g;

	if(chan->flags&BGBMID_CHFL_POINT)
//	if(0)
	{
		f=ctx->cnTime-ctx->listen->orgTime;
		if(f<0)
			{ f=0; }
		g=f+(((float)samp)/ctx->samplerate);
		V3F_ADDSCALE(ctx->listen->org, ctx->listen->vel, f, lorga);
		V3F_ADDSCALE(ctx->listen->org, ctx->listen->vel, g, lorgb);

		f=ctx->cnTime-chan->orgTime;
		if(f<0)
			{ f=0; }
		g=f+(((float)samp)/ctx->samplerate);
		V3F_ADDSCALE(chan->org, chan->vel, f, orga);
		V3F_ADDSCALE(chan->org, chan->vel, g, orgb);

//		V3F_SUB(chan->org, ctx->listen->org, dir);
		V3F_SUB(orga, lorga, dira);
		d=V3F_NORMALIZE(dira, dira);

		V3F_SUB(orgb, lorgb, dirb);
		df=V3F_NORMALIZE(dirb, dirb);

		vprt=(1.0+V3F_DOT(dira, ctx->listen->rot+0))*0.5;
		vpfw=(1.0+V3F_DOT(dira, ctx->listen->rot+3))*0.5;
		vpup=(1.0+V3F_DOT(dira, ctx->listen->rot+6))*0.5;

		vt=d;
		if(vt<1)vt=1;
		vt=1/vt;

		vtf=df;
		if(vtf<1)vtf=1;
		vtf=1/vtf;
		
		if(chan->flags&BGBMID_CHFL_HIGHATTN)
			{ vt=vt*vt; vtf=vtf*vtf; }
		if(chan->flags&BGBMID_CHFL_LOWATTN)
			{ vt=sqrt(vt); vtf=sqrt(vtf); }

		ds=BGBMID_Sound_CalcDoppler(
			ctx->listen->org, ctx->listen->vel,
			chan->org, chan->vel);

		att.vol=vt;
		att.volf=vtf;
		att.pan=2.0*vprt-1.0;
		att.delay=d/BGBMID_SOUND_C;
		att.doppler=ds;
		return(att);
	}
	
	att.vol=1.0;
	att.volf=1.0;
	att.pan=0;
	att.delay=0;
	att.doppler=1.0;
	return(att);
}

int bgbmid_ringmod(int sb, int sl, int v)
{
	if(v<0)
	{
//		if(!sl) { *(int *)-1=-1; }
//		while(v<0)v+=sl;
//		return(v);

		return(sb+(v%sl+sl));
	}

	return(sb+v%sl);
}

int bgbmid_mod(int x, int y)
{
	if(x<0)
	{
//		if(!sl) { *(int *)-1=-1; }
//		while(v<0)v+=sl;
//		return(v);

		return((y+x%y));
	}

	return(x%y);
}

int bgbmid_log2up(int x)
{
	int i, j;
	
	i=x; j=0;
	while(i>1) { i=(i+1)>>1; j++; }
	return(j);
}

int bgbmid_log2dn(int x)
{
	int i, j;
	
	i=x; j=0;
	while(i>1) { i=i>>1; j++; }
	return(j);
}

//double bgbmid_ssqrt(double x)
//{
//	return((x>=0)?sqrt(x):-sqrt(-x));
//}

BMID_API int BGBMID_Sample_NearestPosition(
	BGBMID_Context *ctx, BGBMID_Sample *samp,
	int base, int depth, int chan)
{
	int i, p;

	if(samp->pcm_mip[depth])
	{
		if(samp->chan==1)
		{
//			i=(base%samp->len)>>depth;
//			i=base%(samp->len>>depth);
			i=bgbmid_mod(base, (samp->len>>depth));
			p=samp->pcm_mip[depth][i];
		}else
		{
//			i=(base%samp->len)>>depth;
//			i=base%(samp->len>>depth);
			i=bgbmid_mod(base, (samp->len>>depth));
			p=samp->pcm_mip[depth][i*samp->chan+chan];
		}
	}else if(samp->pcm)
	{
		if(samp->chan==1)
		{
//			i=(base<<depth)%samp->len;
			i=bgbmid_mod((base<<depth), samp->len);
			p=samp->pcm[i];
//			p=samp->pcm[base%samp->len];
		}else
		{
//			i=(base<<depth)%samp->len;
			i=bgbmid_mod((base<<depth), samp->len);
			p=samp->pcm[i*samp->chan+chan];
//			p=samp->pcm[(base%samp->len)*samp->chan+chan];
		}
	}else
	{
		p=BGBMID_BTAC_GetSamplePoint(samp, base, depth, chan);
	}
	
	return(p);
}

float BGBMID_LerpValsF(
	float a, float b, float t)
{
	return(((1.0-t)*a)+(t*b));
}

float BGBMID_SplinePolateValsF(
	float a, float b, float c, float d, float t)
{
	float s, p, u, v;

	s=1.0-t;
	u=b+t*(b-a);
	v=c+s*(c-d);
	p=(u*s)+(v*t);

	return(p);
}

BMID_API vec2 BGBMID_Sample_InterpolatedSamplePosition(
	BGBMID_Context *ctx, BGBMID_Sample *samp,
	double base, double depth)
{
	float a0, b0, c0, d0, p0;
	float a1, b1, c1, d1, p1;
	float p2, pl, pr;
	double sn, sr0, sr1, dr;
	int sb0, sb1, db, ch, snb;
	int i, j, k;

//	depth=0;

	db=floor(depth);
	dr=depth-db;
	
//	sn=base*(samp->rate>>db);
	sn=base/(1<<db);
	sb0=floor(sn);
	sr0=sn-sb0;
	sb1=floor(0.5*sn);
	sr1=(0.5*sn)-sb1;

//	snb=floor(base*samp->rate);
	ch=samp->chan;
	
	for(i=0; i<ch; i++)
	{
		a0=BGBMID_Sample_NearestPosition(ctx, samp, sb0-1, db+0, i);
		b0=BGBMID_Sample_NearestPosition(ctx, samp, sb0+0, db+0, i);
		c0=BGBMID_Sample_NearestPosition(ctx, samp, sb0+1, db+0, i);
		d0=BGBMID_Sample_NearestPosition(ctx, samp, sb0+2, db+0, i);

		a1=BGBMID_Sample_NearestPosition(ctx, samp, sb1-1, db+1, i);
		b1=BGBMID_Sample_NearestPosition(ctx, samp, sb1+0, db+1, i);
		c1=BGBMID_Sample_NearestPosition(ctx, samp, sb1+1, db+1, i);
		d1=BGBMID_Sample_NearestPosition(ctx, samp, sb1+2, db+1, i);
		
		p0=BGBMID_SplinePolateValsF(a0, b0, c0, d0, sr0);
		p1=BGBMID_SplinePolateValsF(a1, b1, c1, d1, sr1);
		p2=BGBMID_LerpValsF(p0, p1, dr);

//		b0=BGBMID_Sample_NearestPosition(ctx, samp, snb, 0, i);
//		p2=b0;
		
		if(i) { pr=p2; }
		else { pl=p2; }
	}
	
	if(ch==1) { pr=pl; }

//	return(vec2(0, 0));
	return(vec2(pl, pr));
}

BMID_API vec2 BGBMID_Sample_InterpolatedSampleSubPosition(
	BGBMID_Context *ctx, BGBMID_Sample *samp,
	double base, double depth, int sbase, int slen)
{
	float a0, b0, c0, d0, p0;
	float a1, b1, c1, d1, p1;
	float p2, pl, pr;
	double sn, sr0, sr1, dr;
	int sb0, sb1, db, ch, snb;
	int sm0, sm1, sl0, sl1;
	int i, j, k;

	db=floor(depth);
	dr=depth-db;
	
//	sn=base*(samp->rate>>db);
	sn=base/(1<<db);
	sb0=floor(sn);
	sr0=sn-sb0;
	sb1=floor(0.5*sn);
	sr1=(0.5*sn)-sb1;

	sm0=sbase>>(db+0);
	sl0=slen>>(db+0);
	sm1=sbase>>(db+1);
	sl1=slen>>(db+1);
	if(!sl0)sl0=1;
	if(!sl1)sl1=1;

//	snb=floor(base*samp->rate);
	ch=samp->chan;
	
	for(i=0; i<ch; i++)
	{
		a0=BGBMID_Sample_NearestPosition(ctx, samp,
			bgbmid_ringmod(sm0, sl0, sb0-1), db+0, i);
		b0=BGBMID_Sample_NearestPosition(ctx, samp,
			bgbmid_ringmod(sm0, sl0, sb0+0), db+0, i);
		c0=BGBMID_Sample_NearestPosition(ctx, samp,
			bgbmid_ringmod(sm0, sl0, sb0+1), db+0, i);
		d0=BGBMID_Sample_NearestPosition(ctx, samp,
			bgbmid_ringmod(sm0, sl0, sb0+2), db+0, i);

		a1=BGBMID_Sample_NearestPosition(ctx, samp,
			bgbmid_ringmod(sm1, sl1, sb1-1), db+1, i);
		b1=BGBMID_Sample_NearestPosition(ctx, samp,
			bgbmid_ringmod(sm1, sl1, sb1+0), db+1, i);
		c1=BGBMID_Sample_NearestPosition(ctx, samp,
			bgbmid_ringmod(sm1, sl1, sb1+1), db+1, i);
		d1=BGBMID_Sample_NearestPosition(ctx, samp,
			bgbmid_ringmod(sm1, sl1, sb1+2), db+1, i);
		
		p0=BGBMID_SplinePolateValsF(a0, b0, c0, d0, sr0);
		p1=BGBMID_SplinePolateValsF(a1, b1, c1, d1, sr1);
		p2=BGBMID_LerpValsF(p0, p1, dr);

//		b0=BGBMID_Sample_NearestPosition(ctx, samp, snb, 0, i);
//		p2=b0;
		
		if(i) { pr=p2; }
		else { pl=p2; }
	}
	
	if(ch==1) { pr=pl; }

//	return(vec2(0, 0));
	return(vec2(pl, pr));
}

BMID_API vec2 BGBMID_Sample_InterpolatedPosition(
	BGBMID_Context *ctx, BGBMID_Sample *samp,
	double base, double depth)
{
	return(BGBMID_Sample_InterpolatedSamplePosition(
		ctx, samp, base*samp->rate, depth));
}

BMID_API vec2 BGBMID_Sample_InterpolatedSubPosition(
	BGBMID_Context *ctx, BGBMID_Sample *samp,
	double base, double depth, int sbase, int slen)
{
	return(BGBMID_Sample_InterpolatedSampleSubPosition(
		ctx, samp, base*samp->rate, depth, sbase, slen));
}

BMID_API void BGBMID_BlendMixChannelReverb(
	BGBMID_Context *ctx, BGBMID_Channel *chan,
	BGBMID_ListenerChannel *lchan, int samp)
{
	float *rvm, *efm;
	float *rvw, *efw;
	float f, g, gl, gr;
	float rvb, rve, efb, efe;
	int i, j, k, n, m, b;

	if(!chan->env)
		return;

	rvb=chan->env->rvBase;
	rve=chan->env->rvEffect;
	efb=chan->env->efBase;
	efe=chan->env->efEffect;
	if(	(rvb==1.0) && (rve==0.0) &&
		(efb==1.0) && (efe==0.0))
		return;
	
	b=ctx->listen->pcmbase+(ctx->listen->pcmbufe-ctx->listen->pcmbuf)/2;
	rvw=lchan->pcmwin_rv;
	efw=lchan->pcmwin_ef;

	rvm=chan->env->rvMat;
	efm=chan->env->efMat;

	if((efb!=1.0) || (efe!=0.0))
	{
		if(!efw)
		{
			efw=bgbmid_malloc(4096*sizeof(float));
			lchan->pcmwin_ef=efw;
		}

		for(i=0; i<samp; i++)
		{
			f=0;
			for(j=0; j<64; j++)
				{ f+=efm[j]*efw[(b+i-j-1)&4095]; }
			gl=lchan->pcmtmp[i*2+0];
			gr=lchan->pcmtmp[i*2+1];
			lchan->pcmtmp[i*2+0]=(gl*efb)+(f*efe);
			lchan->pcmtmp[i*2+1]=(gr*efb)+(f*efe);
			efw[(b+i)&4095]=(gl+gr)*0.5;
		}
	}

	if((rvb!=1.0) || (rve!=0.0))
	{
		if(!rvw)
		{
			rvw=bgbmid_malloc(4096*sizeof(float));
			lchan->pcmwin_rv=rvw;
		}

		for(i=0; i<samp; i++)
		{
			f=0;
			for(j=0; j<64; j++)
				{ f+=rvm[j]*rvw[(b+i-j*j-1)&4095]; }
			gl=lchan->pcmtmp[i*2+0];
			gr=lchan->pcmtmp[i*2+1];
			
			lchan->pcmtmp[i*2+0]=(gl*rvb)+(f*rve);
			lchan->pcmtmp[i*2+1]=(gr*rvb)+(f*rve);
			rvw[(b+i)&4095]=(gl+gr)*0.5;
		}
	}
}

BMID_API int BGBMID_BlendMixChannel(
	BGBMID_Context *ctx, BGBMID_Channel *chan, int samp)
{
	BGBMID_AttParams att;
	BGBMID_ListenerChannel *lchan;
	vec2 sv0;
	float vol, lvol, rvol, freq, pan, bal, pbl;
	float slvol, srvol, elvol, ervol;
	float loffs, roffs;
	float f, g, h, ssc, scd, svol, evol;
	float lh, hb, hl, hh;
	double base, pos, end;
	int i, j, k, b, ss, se, sm;
	int i0, i1;

	if(!chan->samp || !chan->samp->patch)
		return(-1);

	//Hack: Eliminate dummy samples if another sample exists.
//	if((chan->samp->len==144) && (chan->samp->rate==16000))
	if(1)
	{
		if(chan->samp->patch->samp!=chan->samp)
			{ chan->samp=chan->samp->patch->samp; }
		chan->endTime=chan->startTime+
			(((double)chan->samp->len)/chan->samp->rate);
	}

//	f=BGBMID_Sound_CalcChanAttenuation(ctx, chan);
	att=BGBMID_Sound_CalcChanAttParms(ctx, chan, samp);
	svol=att.vol;
	evol=att.volf;

	if((svol<0.001) && (evol<0.001))
		return(0);

//	f=1.0;
	svol=chan->control[7]*chan->vol*svol;
	evol=chan->control[7]*chan->vol*evol;
//	vol=chan->control[7]*chan->vol;
//	vol=16384*chan->vol;
	bal=chan->control[8]/16384.0;
	pan=chan->control[10]/16384.0;

	pan=pan+(att.pan*0.5);
	if(pan<0)pan=0;
	if(pan>1)pan=1;

//	vol=pow(vol/16384.0, 1.66)*16384.0;
//	vol=pow(vol/12800.0, 1.66)*12800.0;
	svol=pow(svol/12800.0, 1.66)*12800.0;
	evol=pow(evol/12800.0, 1.66)*12800.0;

	lvol=(1.0-bal)*(1.0-pan*0.5);
	rvol=(0.0+bal)*(0.5+pan*0.5);
	g=sqrt(lvol*lvol + rvol*rvol);
//	lvol=vol*lvol*(1.0/g);
//	rvol=vol*rvol*(1.0/g);

	slvol=svol*lvol*(1.0/g);
	srvol=svol*rvol*(1.0/g);
	elvol=evol*lvol*(1.0/g);
	ervol=evol*rvol*(1.0/g);

	loffs=pan; roffs=1.0-pan;
	g=sqrt(loffs*loffs + roffs*roffs);
	loffs=loffs*(1.0/g);
	roffs=roffs*(1.0/g);

//	lvol/=32767.0;
//	rvol/=32767.0;

	slvol/=32767.0;
	srvol/=32767.0;
	elvol/=32767.0;
	ervol/=32767.0;

//	if((lvol<0.001) && (rvol<0.001))
//		return(0);

	if((slvol<0.001) && (srvol<0.001) &&
		(elvol<0.001) && (ervol<0.001))
			return(0);

	lchan=BGBMID_GetListenerChannel(ctx, chan);
	if(!lchan->pcmtmp || (samp>lchan->pcmtlen))
	{
		lchan->pcmtmp=bgbmid_realloc(lchan->pcmtmp, samp*2*sizeof(float));
		lchan->pcmtlen=samp;
	}
	
	if(chan->samp->flags&BGBMID_SFL_STREAM)
	{
		base=0;
		end=base+999999.0;
//		att.doppler=1.0;
	}else
	{
		base=(ctx->cnTime-chan->startTime)*att.doppler-att.delay;
		if((chan->endTime>0) && !(chan->flags&BGBMID_CHFL_LOOP))
		{
			end=(chan->endTime-chan->startTime)*att.doppler-att.delay;
			if(base>end)
				return(0);
		}else
		{
			end=base+999999.0;
		}
	}
	
	scd=log((chan->samp->rate*att.doppler)/44100.0)/log(2.0);
	if(scd<0)scd=0;
	if(scd>6.9)scd=6.9;
	
	for(i=0; i<samp; i++)
	{
//		pos=base+i*(1.0/ctx->samplerate);
		pos=base+i*(1.0/(ctx->samplerate*att.doppler));
		
		if((pos<0) || (pos>end))
		{
			sv0=vec2(0, 0);
		}else
		{
			sv0=BGBMID_Sample_InterpolatedPosition(
				ctx, chan->samp, pos, scd);
		}

//		f=((float)i)/samp;
//		g=((1.0-f)*svol)+(f*evol);

		lchan->pcmtmp[i*2+0]=v2x(sv0);
		lchan->pcmtmp[i*2+1]=v2y(sv0);

//		lchan->pcmtmp[i*2+0]+=v2x(sv0)*lvol;
//		lchan->pcmtmp[i*2+1]+=v2y(sv0)*rvol;

//		ctx->listen->pcmbuft[i*2+0]+=v2x(sv0)*lvol;
//		ctx->listen->pcmbuft[i*2+1]+=v2y(sv0)*rvol;
	}

	BGBMID_BlendMixChannelReverb(ctx, chan, lchan, samp);

	for(i=0; i<samp; i++)
	{
		f=((float)i)/samp;
		lvol=((1.0-f)*slvol)+(f*elvol);
		rvol=((1.0-f)*srvol)+(f*ervol);

		f=lchan->pcmtmp[i*2+0];
		g=lchan->pcmtmp[i*2+1];
		h=(f+g)*0.5;
		ctx->listen->pcmbuft[i*2+0]+=f*lvol;
		ctx->listen->pcmbuft[i*2+1]+=g*rvol;

		if(chan->flags&BGBMID_CHFL_CALCCURVOL)
		{
			chan->mix->cur_absvol=
				chan->mix->cur_absvol*0.999+
				fabs(h)*0.001;
			chan->mix->cur_relvol_lf=
				chan->mix->cur_relvol_lf*0.999+
				fabs(f*lvol)*0.001;
			chan->mix->cur_relvol_rt=
				chan->mix->cur_relvol_rt*0.999+
				fabs(g*rvol)*0.001;
		}
		
		if(chan->flags&BGBMID_CHFL_CALCCURDCT)
		{
			BGBMID_Wave_UpdateHistogram(
				chan->mix->hist, h, lh);
#if 0
			for(j=0; j<15; j++)
			{
				hb=	chan->mix->hist[j]*0.37+
					chan->mix->hist[j+1]*0.63;
				chan->mix->hist[j]=hb;
			}
			chan->mix->hist[15]=
				chan->mix->hist[15]*0.37+(h-lh)*0.63;
#endif
		}
		
		lh=h;
	}
	
	return(0);
}

BMID_API int BGBMID_DestroyMixChannel(
	BGBMID_Context *ctx, BGBMID_Channel *chan)
{
	bgbmid_free(chan->mix);
	chan->mix=NULL;
	return(0);
}

BMID_API int BGBMID_InitMixChannel(
	BGBMID_Context *ctx, BGBMID_Channel *chan, BGBMID_Sample *samp)
{
//	bgbmid_free(chan->mix);
//	chan->mix=NULL;

	if(!chan->mix)
		{ chan->mix=bgbmid_malloc(sizeof(BGBMID_MixChannel)); }
		
	chan->startTime=ctx->cnTime;
	chan->endTime=ctx->cnTime+(((double)samp->len)/samp->rate);
	chan->samp=samp;
	return(0);
}

BMID_API BGBMID_ListenerChannel *BGBMID_GetListenerChannel(
	BGBMID_Context *ctx, BGBMID_Channel *chan)
{
	BGBMID_ListenerChannel *lchan;
	
	lchan=ctx->listen->lchan[chan->id];
	if(!lchan)
	{
		lchan=bgbmid_malloc(sizeof(BGBMID_ListenerChannel));
		ctx->listen->lchan[chan->id]=lchan;
	}

	return(lchan);

//	bgbmid_free(chan->mix);
//	chan->mix=NULL;
}


BMID_API void BGBMID_SetReverbDefault(BGBMID_Context *ctx)
{
	if(!ctx->listen->env)
		ctx->listen->env=bgbmid_malloc(sizeof(BGBMID_EnvFilter));

	BGBMID_SetReverbBaseEffect(ctx, 1.0, 0.0);
	BGBMID_SetFilterEffect(ctx, 1.0, 0.0);
}

BMID_API void BGBMID_SetReverbBaseEffect(
	BGBMID_Context *ctx, float base, float effect)
{
	ctx->listen->env->rvBase=base;
	ctx->listen->env->rvEffect=effect;
}

BMID_API void BGBMID_SetFilterEffect(
	BGBMID_Context *ctx, float base, float effect)
{
	ctx->listen->env->efBase=base;
	ctx->listen->env->efEffect=effect;
}

BMID_API void BGBMID_SetupReverbSinc(
	BGBMID_Context *ctx, float sc)
{
	float f, g;
	int i;
	
	g=0;
	for(i=0; i<256; i++)
	{
		f=sin(i*sc*(M_TAU/256))*(1.0-(i+1)/256.0);
		ctx->listen->env->rvMat[i]=f;
		g=g+f*f;
	}
	
	if(g>0)g=1.0/sqrt(g);
	for(i=0; i<256; i++)
		{ ctx->listen->env->rvMat[i]*=g; }
}

BMID_API void BGBMID_SetupReverbCosc(
	BGBMID_Context *ctx, float sc)
{
	float f, g;
	int i;
	
	g=0;
	for(i=0; i<256; i++)
	{
		f=cos(i*sc*(M_TAU/256))*(1.0-(i+1)/256.0);
		ctx->listen->env->rvMat[i]=f;
		g=g+f*f;
	}
	
	if(g>0)g=1.0/sqrt(g);
	for(i=0; i<256; i++)
		{ ctx->listen->env->rvMat[i]*=g; }
}

BMID_API void BGBMID_SetupReverbSin(
	BGBMID_Context *ctx, float sc)
{
	float f, g;
	int i;
	
	g=0;
	for(i=0; i<256; i++)
	{
		f=sin(i*sc*(M_TAU/256));
		ctx->listen->env->rvMat[i]=f;
		g=g+f*f;
	}
	
	if(g>0)g=1.0/sqrt(g);
	for(i=0; i<256; i++)
		{ ctx->listen->env->rvMat[i]*=g; }
}

BMID_API void BGBMID_SetupReverbCos(
	BGBMID_Context *ctx, float sc)
{
	float f, g;
	int i;
	
	g=0;
	for(i=0; i<256; i++)
	{
		f=cos(i*sc*(M_TAU/256));
		ctx->listen->env->rvMat[i]=f;
		g=g+f*f;
	}
	
	if(g>0)g=1.0/sqrt(g);
	for(i=0; i<256; i++)
		{ ctx->listen->env->rvMat[i]*=g; }
}

BMID_API void BGBMID_SetupFilterCos(
	BGBMID_Context *ctx, float sc)
{
	float f, g;
	int i;
	
	g=0;
	for(i=0; i<256; i++)
	{
		f=cos(i*sc*(M_TAU/256));
		ctx->listen->env->efMat[i]=f;
		g=g+f*f;
	}
	
	if(g>0)g=1.0/sqrt(g);
	for(i=0; i<256; i++)
		{ ctx->listen->env->efMat[i]*=g; }
}

BMID_API void BGBMID_SetupFilterCosc(
	BGBMID_Context *ctx, float sc)
{
	float f, g;
	int i;
	
	g=0;
	for(i=0; i<256; i++)
	{
		f=cos(i*sc*(M_TAU/256))*(1.0-(i+1)/256.0);
		ctx->listen->env->efMat[i]=f;
		g=g+f*f;
	}
	
	if(g>0)g=1.0/sqrt(g);
	for(i=0; i<256; i++)
		{ ctx->listen->env->efMat[i]*=g; }
}

BMID_API void BGBMID_SetupFilterFalloff(
	BGBMID_Context *ctx, float sc)
{
	float f, g;
	int i;
	
	g=0;
	for(i=0; i<256; i++)
	{
		f=1.0-sc*(i+1)/256.0;
		if(f<0)f=0;
		ctx->listen->env->efMat[i]=f;
		g=g+f*f;
	}
	
	if(g>0)g=1.0/sqrt(g);
	for(i=0; i<256; i++)
		{ ctx->listen->env->efMat[i]*=g; }
}
