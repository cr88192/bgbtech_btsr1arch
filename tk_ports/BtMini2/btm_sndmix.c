TkMod_MixSample *tkm_samples;

#ifdef BTM_TARGET_SMALL
// #define TKM_SND_MAXCHAN	16
#define TKM_SND_MAXCHAN	8
#else
#define TKM_SND_MAXCHAN	64
#endif


TkMod_MixSample *tkm_chan_samp[TKM_SND_MAXCHAN];
int tkm_chan_pos[TKM_SND_MAXCHAN];
int tkm_chan_delay[TKM_SND_MAXCHAN];
byte tkm_chan_vol[TKM_SND_MAXCHAN];
byte tkm_chan_vol_lf[TKM_SND_MAXCHAN];
byte tkm_chan_vol_rt[TKM_SND_MAXCHAN];
u16 tkm_chan_scale[TKM_SND_MAXCHAN];
u16 tkm_chan_tone[TKM_SND_MAXCHAN];
u64 tkm_chan_spos[TKM_SND_MAXCHAN];
u64 tkm_chan_svel[TKM_SND_MAXCHAN];


TkMod_Info *tkm_snd_song;

byte tkm_vol_sfx=192;
byte tkm_vol_music=192;

s16 tkm_snd_mixbuf[16384];
s16 tkm_snd_omixbuf[65536+1024];
int tkm_snd_omixbufrov;

int tkm_mixfir[256];

u64 tkm_snd_vpos;
u64 tkm_snd_vvel;
byte tkm_snd_vyaw;

#define FCC_RIFF	BTMGL_FOURCC('R', 'I', 'F', 'F')
#define FCC_WAVE	BTMGL_FOURCC('W', 'A', 'V', 'E')
#define FCC_fmt		BTMGL_FOURCC('f', 'm', 't', ' ')
#define FCC_data	BTMGL_FOURCC('d', 'a', 't', 'a')

void * volatile btm_sndmix_mutex;
void * volatile btm_sndload_mutex;

int BTM_SndLockMixer()
{
	if(!btm_sndmix_mutex)
		btm_sndmix_mutex=thMutex();
	thLockMutex(btm_sndmix_mutex);
	return(0);
}

int BTM_SndUnlockMixer()
{
	thUnlockMutex(btm_sndmix_mutex);
	return(0);
}

int BTM_SndLockLoad()
{
	if(!btm_sndload_mutex)
		btm_sndload_mutex=thMutex();
	thLockMutex(btm_sndload_mutex);
	return(0);
}

int BTM_SndUnlockLoad()
{
	thUnlockMutex(btm_sndload_mutex);
	return(0);
}

int btm_mixfir_dist2fp8(int dist)
{
	int ex, v;
	
	if(dist<16)
		return(dist);
	
	ex=1;
	v=dist;
	while(v>=32)
		{ v=v>>1; ex++; }
	v=(ex<<4)|(v&15);
	return(v);
}

int tkm_mixfir_fp8tosamp[256];

int TKM_RebuildMixFir()
{
	int i, j, k, l, shr;

	for(i=0; i<256; i++)
	{
		
		if(i>=16)
			j=(16|(i&15))<<(((i>>4)&15)-1);
		else
			j=i;
			
		j=j<<6;

		tkm_mixfir_fp8tosamp[i]=j;
	}

	for(i=0; i<256; i++)
		tkm_mixfir[i]=0;
	
	shr=14+8;
	if(cam_mvflags&4)
//	if(cam_mvflags&2)
		shr-=6;

	
	for(i=0; i<1024; i++)
	{
//		k=i*i+1;
//		k=i+1;
		k=pow(i, 1.5)+1;
		j=btm_mixfir_dist2fp8(i);
//		tkm_mixfir[j]+=(btm_drawstat_dist[i]*(65536LL/(i+1)))>>10;
//		tkm_mixfir[j]+=(btm_drawstat_dist[i]*(65536LL/k))>>10;
//		tkm_mixfir[j]+=(btm_drawstat_dist[i]*(65536LL/k))>>14;
//		tkm_mixfir[j]+=(btm_drawstat_dist[i]*(65536LL/k))>>12;
		tkm_mixfir[j]+=(btm_drawstat_dist_hrsc[i]*(65536LL/k))>>shr;
	}
	return(0);
}

byte *TkMod_DecodeWavBuf(byte *ibuf, int ibsz, int *rlen)
{
	byte *cs, *cse, *cs1;
	byte *data, *ddat;
	u32 fcc0, sz0, fcc1, dsz;
	int chan, rate, bits, len, len1, ssz, ilen;
	int spos, sstep;
	int i, j, k;
	
	fcc0=get_u32le(ibuf+0);
	sz0=get_u32le(ibuf+4);
	fcc1=get_u32le(ibuf+8);
	
	if(	(fcc0!=FCC_RIFF) ||
		(fcc1!=FCC_WAVE))
			return(NULL);
	cs=ibuf+12;
	cse=cs+(sz0-4);
	
	if((sz0+8)>ibsz)
		return(NULL);
	
	data=NULL;
	dsz=0;
	chan=1;
	rate=16000;
	bits=8;
	while(cs<cse)
	{
		fcc0=get_u32le(cs+0);
		sz0=get_u32le(cs+4);
		cs1=cs+8;
		if(fcc0==FCC_fmt)
		{
			chan=cs1[2];
			rate=cs1[4]+(cs1[5]<<8);
			bits=cs1[14];
		}

		if(fcc0==FCC_data)
		{
			data=cs1;
			dsz=sz0;
		}
		
		cs+=8+((sz0+1)&(~1));
	}
	
	ssz=(chan*bits)/8;
	ilen=dsz/ssz;
	
	len=(ilen*16000)/rate;

	sstep=(256*rate)/16000;
	spos=0;
	
	len1=(len+511)&(~255);
	
	ddat=btm_malloc(len1);
	memset(ddat, 128, len1);
	
	for(i=0; i<len; i++)
	{
		j=(spos>>8);
		spos+=sstep;
		if(bits==16)
		{
			k=(s16)(get_u16le(data+(j*ssz)));
			k=(k>>8)+128;
		}else
		{
			k=data[j*ssz];
		}
		ddat[i]=k;
	}
	
	*rlen=len;
	return(ddat);
}

TkMod_MixSample *TkMod_GetSample(char *name)
{
	TkMod_MixSample *cur;
	byte *buf;
	int sz, len;

	cur=tkm_samples;
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}

	BTM_SndLockLoad();

	cur=btm_malloc(sizeof(TkMod_MixSample));
	memset(cur, 0, sizeof(TkMod_MixSample));

	cur->name=bccx_strdup(name);
	cur->next=tkm_samples;
	tkm_samples=cur;

	sz=0; len=0;
	cur->data=NULL;
	buf=BTM_LoadFileTmp(name, &sz);
//	buf=BTM_LoadFile(name, &sz);
	if(buf)
	{
		cur->data=TkMod_DecodeWavBuf(buf, sz, &len);
		cur->len=len;
	}

	if(!cur->data)
	{
		cur->data=btm_malloc(512);
		memset(cur->data, 128, 512);
		cur->len=1;
	}

	BTM_SndUnlockLoad();
	return(cur);
}

int BTM_FindFreeMixChan()
{
	int i;

	for(i=0; i<TKM_SND_MAXCHAN; i++)
		if(!tkm_chan_samp[i])
			return(i);
	return(-1);
}

int BTM_PlaySample(char *name, int vol)
{
	TkMod_MixSample *samp;
	int chan;
	
	BTM_SndLockMixer();

	samp=TkMod_GetSample(name);
	chan=BTM_FindFreeMixChan();

	if(chan<0)
	{
		BTM_SndUnlockMixer();
		return(-1);
	}
	
	tkm_chan_samp[chan]=samp;
	tkm_chan_pos[chan]=0;
	tkm_chan_vol[chan]=vol;
	tkm_chan_vol_lf[chan]=vol;
	tkm_chan_vol_rt[chan]=vol;
	tkm_chan_delay[chan]=0;
	tkm_chan_scale[chan]=256;
	tkm_chan_tone[chan]=256;

	tkm_chan_spos[chan]=0;
	tkm_chan_svel[chan]=0;
	
	BTM_SndUnlockMixer();

	return(chan);
}

int BTM_SoundSetVpos(u64 vpos, u64 vvel, byte yaw, byte pitch)
{
	tkm_snd_vpos=vpos;
	tkm_snd_vvel=vvel;
	tkm_snd_vyaw=yaw;
	return(0);
}

int BTM_PlaySample3D(char *name, u64 spos, u64 svel, int vol,
	int tone)
{
	int da, attn, i;

	da=BTM_CalcRayDistApprox(spos, tkm_snd_vpos);

	if(da<256)
		da=256;
	attn=65536/da;

	if(attn<4)
		return(0);


	i=BTM_PlaySample(name, vol);
	if(i>=0)
	{
		BTM_SndLockMixer();

		tkm_chan_spos[i]=spos;
		tkm_chan_svel[i]=svel;
		tkm_chan_vol[i]=vol;

		if(tone)
			tkm_chan_tone[i]=tone;

		tkm_chan_delay[i]=(da*16000LL)>>16;
		
		BTM_MixUpdateChannelPos(i);

		BTM_SndUnlockMixer();

		return(i);
	}

	return(i);
}

int BTM_PlaySong(char *name)
{
	TkMod_Info *song;

//	BTM_SndLockMixer();
	BTM_SndLockLoad();

	song=TkMod_GetMod(name);
	tkm_snd_song=song;

	BTM_SndUnlockLoad();
//	BTM_SndUnlockMixer();
	
	return(0);
}

int BTM_AngleForRelCorg(u64 sorg, u64 vorg)
{
	int cx, cy, cz, vx, vy, vz;
	int dx, dy, dz;
	int a;

	vx=(vorg>> 0)&0x00FFFFFF;
	vy=(vorg>>24)&0x00FFFFFF;
	vz=(vorg>>48)&0x0000FFFF;

	cx=(sorg>> 0)&0x00FFFFFF;
	cy=(sorg>>24)&0x00FFFFFF;
	cz=(sorg>>48)&0x0000FFFF;
	
	dx=cx-vx;
	dy=cy-vy;
	dz=cz-vz;
	
	a=atan2(dy, dx)*(128.0/M_PI);
	return(a&255);
}

int BTM_MixUpdateChannelPos(int chan)
{
	byte pvel_lf[16]=
		{
		128,  85,  42,  0,
		  0,  42,  85, 128,
		128, 170, 212, 255,
		255, 212, 170, 128
		};
	u64 spos, spos2, vpos2;
	int da, da2, dv, an, arn, attn, vol, i, tone;
	int v_lf, v_rt;

	if(chan<0)
		return(0);

	if(chan>=TKM_SND_MAXCHAN)
		{ debug_break }

	spos=tkm_chan_spos[chan];
	vol=tkm_chan_vol[chan];
	
	if(!spos)
	{
//		tkm_chan_vol_lf[chan]=vol;
//		tkm_chan_vol_rt[chan]=vol;
		return(0);
	}

	spos2=tkm_chan_spos[chan]+tkm_chan_svel[chan];
	vpos2=tkm_snd_vpos+tkm_snd_vvel;

	da=BTM_CalcRayDistApprox(spos, tkm_snd_vpos);
	da2=BTM_CalcRayDistApprox(spos2, vpos2);
	dv=da2-da;

	if(da<256)
		da=256;

	attn=65536/da;
	attn=(attn*vol)>>8;
	
	an=BTM_AngleForRelCorg(spos, tkm_snd_vpos);
	arn=(an-tkm_snd_vyaw)&255;
	
	v_lf=(attn*(128+(pvel_lf[arn>>4]>>1)))>>8;
	v_rt=(attn*(255-(pvel_lf[arn>>4]>>1)))>>8;
	
	tkm_chan_vol_lf[chan]=v_lf;
	tkm_chan_vol_rt[chan]=v_rt;
	
	tone=tkm_chan_tone[chan];

//	i=(65536+dv)>>8;
	i=((tone<<8)+dv)>>8;
	if(i<16)i=16;
	if(i>65535)i=65535;
	tkm_chan_scale[chan]=i;

	return(0);
}

int BTM_DoMixSamples(int dt)
{
	static int accdt;
	TkMod_Info *song;
	int i0, i1, i2;
	int i, j, k, l, a_lf, a_rt, ssc, sj;

#ifdef __BJX2__
	return(0);		//BGB: For now, save CPU time.
#endif

//	return(0);

#ifndef BTM_TARGET_SMALL
	TKM_RebuildMixFir();
#endif
	
	accdt+=dt;
	while(accdt>=16)
	{
		BTM_SndLockMixer();

		for(j=0; j<TKM_SND_MAXCHAN; j++)
			BTM_MixUpdateChannelPos(j);

		song=NULL;

// #if 0
#ifndef BTM_TARGET_SMALL
		song=tkm_snd_song;
		if(song)
		{
			if((song->mixpos_cs-song->mixbufs)>=8000)
			{
				k=song->mixpos-song->mixpos_cs;
				memcpy(song->mixbufs,
					song->mixpos_cs,
					k*2);
				song->mixpos_cs=song->mixbufs;
				song->mixpos=song->mixbufs+k;
			}
		
			while((song->mixpos_cs+512)>song->mixpos)
			{
				TkMod_StepSongRow(song);
				for(i=0; i<song->tick_div; i++)
					TkMod_StepMixCtx(song, song->tick_samp);
			}
		}
#endif

		for(i=0; i<(2*16*16); i++)
		{
			a_lf=0;
			a_rt=0;
			for(j=0; j<TKM_SND_MAXCHAN; j++)
			{
				if(!tkm_chan_samp[j])
					continue;
				if(tkm_chan_delay[j]>0)
					continue;
					
				ssc=tkm_chan_scale[j];
				sj=(i*ssc)>>8;
					
//				k=tkm_chan_pos[j]+(i>>1);
//				k=tkm_chan_pos[j]+i;
				k=tkm_chan_pos[j]+sj;

				if(k>=tkm_chan_samp[j]->len)
					k=tkm_chan_samp[j]->len-1;

				k=tkm_chan_samp[j]->data[k];
				k=(k-128)<<8;
				a_lf+=(k*tkm_chan_vol_lf[j])>>8;
				a_rt+=(k*tkm_chan_vol_rt[j])>>8;
			}
			
			a_lf=(a_lf*tkm_vol_sfx)>>8;

			if(song)
			{
//				l+=song->mixpos_cs[i];
				k=(song->mixpos_cs[i]*tkm_vol_music)>>8;
				a_lf+=k;
				a_rt+=k;
			}

			if(a_lf<-32767)		a_lf=-32767;
			if(a_lf> 32767)		a_lf= 32767;
			if(a_rt<-32767)		a_rt=-32767;
			if(a_rt> 32767)		a_rt= 32767;

			tkm_snd_mixbuf[i*2+0]=a_lf;
			tkm_snd_mixbuf[i*2+1]=a_rt;
		}

		for(j=0; j<TKM_SND_MAXCHAN; j++)
		{
			if(!tkm_chan_samp[j])
				continue;

			i=16*16;
			ssc=tkm_chan_scale[j];
			sj=(i*ssc)>>8;

			if(tkm_chan_delay[j]>0)
			{
//				k=tkm_chan_delay[j]-(16*16);
				k=tkm_chan_delay[j]-sj;
				if(k<0)
					k=0;
				tkm_chan_delay[j]=k;
				continue;
			}
				
//			k=tkm_chan_pos[j]+(16*16)/2;
//			k=tkm_chan_pos[j]+(16*16);
			k=tkm_chan_pos[j]+sj;
			tkm_chan_pos[j]=k;
			if(k>=tkm_chan_samp[j]->len)
			{
				tkm_chan_samp[j]=NULL;
				tkm_chan_spos[j]=0;
			}
		}

		if(song)
		{
			song->mixpos_cs+=256;
		}
		
//		k=(32768-512)*2;
//		memcpy(tkm_snd_omixbuf, tkm_snd_omixbuf+256*2, k*sizeof(s16));
//		memcpy(tkm_snd_omixbuf+k, tkm_snd_mixbuf, 512*2*sizeof(s16));

#if 0
		k=tkm_snd_omixbufrov;
		memcpy(tkm_snd_omixbuf+k, tkm_snd_mixbuf, 512*2*sizeof(s16));
		tkm_snd_omixbufrov=(k+(256*2))&65535;
		
		for(i=0; i<512; i++)
		{
			a_lf=tkm_snd_mixbuf[i*2+0];
			a_rt=tkm_snd_mixbuf[i*2+1];

			a_lf<<=8;
			a_rt<<=8;
			for(j=0; j<256; j++)
			{
				if(!tkm_mixfir[j])
					continue;
				i0=tkm_mixfir[j];
				i1=tkm_mixfir_fp8tosamp[j];
				i2=((k+i)-i1*2)&65535;
				a_lf+=tkm_snd_omixbuf[i2+0]*i0;
				a_rt+=tkm_snd_omixbuf[i2+1]*i0;
			}

			a_lf>>=8;
			a_rt>>=8;

			if(a_lf<-32767)		a_lf=-32767;
			if(a_lf> 32767)		a_lf= 32767;
			if(a_rt<-32767)		a_rt=-32767;
			if(a_rt> 32767)		a_rt= 32767;

			tkm_snd_mixbuf[i*2+0]=a_lf;
			tkm_snd_mixbuf[i*2+1]=a_rt;
		}
#endif
	
//		tkm_snd_mixbuf[]
		SoundDev_WriteStereoSamples2(tkm_snd_mixbuf, 16*16, 16*16*2);
		accdt-=16;

		BTM_SndUnlockMixer();
	}

	return(0);
}
