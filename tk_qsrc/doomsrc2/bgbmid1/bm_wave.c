#include "bgbmid.h"

#define PDGL_RIFF_RIFF 0x46464952
#define PDGL_RIFF_WAVE 0x45564157
#define PDGL_RIFF_fmt  0x20746D66
#define PDGL_RIFF_data 0x61746164

BMID_API int BGBMID_ScaleSample(short *d, int dl, short *s, int sl);

BMID_API byte *BGBMID_LoadWAV(char *name,
	int *rch, int *rr, int *rbits, int *rlen)
{
	static byte tbuf[4096];
	void *fd;
	byte *buf;

	unsigned int tag, len, rsrc, fend, fstrt;
	int chan, rate, bits, slen;
	int i, j;

	chan=0;
	rate=0;
	bits=0;
	slen=0;

	fd=bgbmid_fopen(name, "rb");

	if(!fd)
	{
//		printf("BGBMID_LoadWAV: Failed open sample '%s'\n", name);
		return(NULL);
	}

	slen=0; tag=0; len=0; rsrc=0;
	for(i=0; i<4; i++)tag+=bgbmid_fgetc(fd)<<(i*8);
	for(i=0; i<4; i++)len+=bgbmid_fgetc(fd)<<(i*8);
	for(i=0; i<4; i++)rsrc+=bgbmid_fgetc(fd)<<(i*8);
	fstrt=bgbmid_ftell(fd);
	fend=fstrt+len;

	if(tag!=PDGL_RIFF_RIFF)
	{
		printf("PDGL_Sound_LoadWAV: File Tag not RIFF (%s)\n", name);
		bgbmid_fclose(fd);
		return(NULL);
	}
	if(rsrc!=PDGL_RIFF_WAVE)
	{
		printf("PDGL_Sound_LoadWAV: Resource type not WAVE (%s)\n", name);
		bgbmid_fclose(fd);
		return(NULL);
	}

	//fmt
	while(bgbmid_ftell(fd)<fend)
	{
		tag=0; len=0;
		for(i=0; i<4; i++)tag+=bgbmid_fgetc(fd)<<(i*8);
		for(i=0; i<4; i++)len+=bgbmid_fgetc(fd)<<(i*8);

		if(tag!=PDGL_RIFF_fmt)
		{
			len=(len+1)&~1;
			bgbmid_fseek(fd, len, 1);
			continue;
		}

		bgbmid_fread(tbuf, 1, len, fd);

		if((tbuf[0]!=1) || (tbuf[1]!=0))
		{
			printf("PDGL_Sound_LoadWAV: WAVE not PCM (%s)\n",
				name);
			bgbmid_fclose(fd);
			return(NULL);
		}

		chan=tbuf[2];
		rate=tbuf[4]+(tbuf[5]<<8);
		bits=tbuf[14];

		break;
	}


	//data
	buf=NULL;
	bgbmid_fseek(fd, fstrt, 0);
	while(bgbmid_ftell(fd)<fend)
	{
		tag=0; len=0;
		for(i=0; i<4; i++)tag+=bgbmid_fgetc(fd)<<(i*8);
		for(i=0; i<4; i++)len+=bgbmid_fgetc(fd)<<(i*8);

		if(tag!=PDGL_RIFF_data)
		{
			len=(len+1)&~1;
			bgbmid_fseek(fd, len, 1);
			continue;
		}

		buf=bgbmid_malloc(len+1024);
		bgbmid_fread(buf, 1, len, fd);

		i=chan*(bits/8);
		slen=len/i;

		break;
	}

	bgbmid_fclose(fd);

	*rch=chan;
	*rr=rate;
	*rbits=bits;
	*rlen=slen;
	return(buf);
}

BMID_API void BGBMID_StoreWAV(char *name, byte *buf, int ch, int rt, int bits, int len)
{
	void *fd;
	int i;

	fd=bgbmid_fopen(name, "wb");
	bgbmid_fputc('R', fd);	bgbmid_fputc('I', fd);	bgbmid_fputc('F', fd);	bgbmid_fputc('F', fd);
	bgbmid_fputc(0, fd);	bgbmid_fputc(0, fd);	bgbmid_fputc(0, fd);	bgbmid_fputc(0, fd);
	bgbmid_fputc('W', fd);	bgbmid_fputc('A', fd);	bgbmid_fputc('V', fd);	bgbmid_fputc('E', fd);

	//format header
	bgbmid_fputc('f', fd);	bgbmid_fputc('m', fd);	bgbmid_fputc('t', fd);	bgbmid_fputc(' ', fd);
	bgbmid_fputc(16, fd);	bgbmid_fputc(0, fd);	bgbmid_fputc(0, fd);	bgbmid_fputc(0, fd);

	bgbmid_fputc(1, fd);		bgbmid_fputc(0, fd);
	bgbmid_fputc(ch, fd);		bgbmid_fputc(0, fd);

	bgbmid_fputc(rt&0xFF, fd);	bgbmid_fputc((rt>>8)&0xFF, fd);
	bgbmid_fputc(0, fd);		bgbmid_fputc(0, fd);

	i=rt*ch*(bits/8);
	bgbmid_fputc(i&0xFF, fd);		bgbmid_fputc((i>>8)&0xFF, fd);
	bgbmid_fputc((i>>16)&0xFF, fd);	bgbmid_fputc((i>>24)&0xFF, fd);

	bgbmid_fputc(ch*(bits/8), fd);		bgbmid_fputc(0, fd);
	bgbmid_fputc(bits, fd);		bgbmid_fputc(0, fd);


	//data
	bgbmid_fputc('d', fd);	bgbmid_fputc('a', fd);	bgbmid_fputc('t', fd);	bgbmid_fputc('a', fd);

	i=len*ch*(bits/8);
	bgbmid_fputc(i&0xFF, fd);		bgbmid_fputc((i>>8)&0xFF, fd);
	bgbmid_fputc((i>>16)&0xFF, fd);	bgbmid_fputc((i>>24)&0xFF, fd);

	bgbmid_fwrite(buf, 1, i, fd);
	if(i)bgbmid_fputc(0, fd);

	i=bgbmid_ftell(fd);
	bgbmid_fseek(fd, 4, 0);

	i-=8;	
	bgbmid_fputc(i&0xFF, fd);		bgbmid_fputc((i>>8)&0xFF, fd);
	bgbmid_fputc((i>>16)&0xFF, fd);	bgbmid_fputc((i>>24)&0xFF, fd);

	bgbmid_fclose(fd);
}

BMID_API short *BGBMID_LoadWAV16(char *name,
	int *rch, int *rrt, int *rlen)
{
	byte *bpb;
	short *bp, *bp1;
	int ch, rt, bts, len;
	int i, j, k;

	bpb=BGBMID_LoadWAV(name, &ch, &rt, &bts, &len);
	if(!bpb)return(NULL);

	bp=NULL;
	if(bts==8)
	{
		bp=bgbmid_malloc(ch*len*sizeof(short)+1024);

		for(i=0; i<(ch*len); i++)
		{
			j=bpb[i]; j=(j-128)*256;
			bp[i]=j;
		}

		bgbmid_free(bpb);
	}
	if(bts==16)bp=(short *)bpb;

	if(!bp)
	{
		bgbmid_free(bpb);
		printf("PDGL_Sound_LoadWAV_44Mono16: "
			"WAVE not 8 or 16 bits (%s)\n", name);
		return(NULL);
	}

	*rch=ch;
	*rrt=rt;
	*rlen=len;
	return(bp);
}

BMID_API short *BGBMID_LoadWAV_Mono16(char *name, int *rrt, int *rlen)
{
	byte *bpb;
	short *bp, *bp1;
	int ch, rt, bts, len;
	int i, j, k;

	bpb=BGBMID_LoadWAV(name, &ch, &rt, &bts, &len);
	if(!bpb)return(NULL);

	bp=NULL;
	if(bts==8)
	{
		bp=bgbmid_malloc(ch*len*sizeof(short)+1024);

		for(i=0; i<(ch*len); i++)
		{
			j=bpb[i]; j=(j-128)*256;
			bp[i]=j;
		}

		bgbmid_free(bpb);
	}
	if(bts==16)bp=(short *)bpb;

	if(!bp)
	{
		bgbmid_free(bpb);
		printf("PDGL_Sound_LoadWAV_44Mono16: "
			"WAVE not 8 or 16 bits (%s)\n", name);
		return(NULL);
	}

	if(ch<1)
	{
		bgbmid_free(bpb);
		printf("PDGL_Sound_LoadWAV_44Mono16: "
			"WAVE bad number of channels (%s)\n", name);
		return(NULL);
	}

	if(ch>1)
	{
		bp1=bgbmid_malloc(len*sizeof(short));
		for(i=0; i<len; i++)
		{
			k=0;
			for(j=0; j<ch; j++)
				k+=bp[i*ch+j];
			if(k<-32767)k=-32767;
			if(k>32767)k=32767;
			bp1[i]=k;
		}

		bgbmid_free(bp);
		bp=bp1;
	}

	*rrt=rt;
	*rlen=len;
	return(bp);
}

BMID_API short *BGBMID_LoadWAV_44Mono16(char *name, int *rlen)
{
	short *bp, *bp1;
	int rt, len;
	int i, j, k;

	bp=BGBMID_LoadWAV_Mono16(name, &rt, &len);
	if(!bp)return(NULL);

	if(rt!=44100)
	{
		i=len*(44100.0/rt);
		bp1=bgbmid_malloc(i*sizeof(short));

		BGBMID_ScaleSample(bp1, i, bp, len);

		bgbmid_free(bp);
		bp=bp1;
		len=i;
	}

	*rlen=len;
	return(bp);
}

BMID_API short *BGBMID_LoadWAV_16Mono16(char *name, int *rlen)
{
	short *bp, *bp1;
	int rt, len;
	int i, j, k;

	bp=BGBMID_LoadWAV_Mono16(name, &rt, &len);
	if(!bp)return(NULL);

	if(rt!=16000)
	{
		i=len*(16000.0/rt);
		bp1=bgbmid_malloc(i*sizeof(short));

		BGBMID_ScaleSample(bp1, i, bp, len);

		bgbmid_free(bp);
		bp=bp1;
		len=i;
	}

	*rlen=len;
	return(bp);
}

BMID_API short *BGBMID_LoadWAV_8Mono16(char *name, int *rlen)
{
	short *bp, *bp1;
	int rt, len;
	int i, j, k;

	bp=BGBMID_LoadWAV_Mono16(name, &rt, &len);
	if(!bp)return(NULL);

	if(rt!=8000)
	{
		i=len*(8000.0/rt);
		bp1=bgbmid_malloc(i*sizeof(short));

		BGBMID_ScaleSample(bp1, i, bp, len);

		bgbmid_free(bp);
		bp=bp1;
		len=i;
	}

	*rlen=len;
	return(bp);
}

BMID_API short *BGBMID_LoadWAV_Stereo16(char *name, int *rrt, int *rlen)
{
	byte *bpb;
	short *bp, *bp1;
	int ch, rt, bts, len;
	int i, j, k;

	bpb=BGBMID_LoadWAV(name, &ch, &rt, &bts, &len);
	if(!bpb)return(NULL);

	bp=NULL;
	if(bts==8)
	{
		bp=bgbmid_malloc(ch*len*sizeof(short));

		for(i=0; i<(ch*len); i++)
		{
			j=bpb[i]; j=(j-128)*256;
			bp[i]=j;
		}

		bgbmid_free(bpb);
	}
	if(bts==16)bp=(short *)bpb;

	if(!bp)
	{
		bgbmid_free(bpb);
		printf("PDGL_Sound_LoadWAV_44Mono16: "
			"WAVE not 8 or 16 bits (%s)\n", name);
		return(NULL);
	}

	if(ch<1)
	{
		bgbmid_free(bpb);
		printf("PDGL_Sound_LoadWAV_44Stereo16: "
			"WAVE bad number of channels (%s)\n", name);
		return(NULL);
	}

	if(ch==1)
	{
		bp1=bgbmid_malloc(len*2*sizeof(short));
		for(i=0; i<len; i++)
		{
			k=bp[i];
			bp1[i*2+0]=k;
			bp1[i*2+1]=k;
		}

		bgbmid_free(bp);
		bp=bp1;
	}

	*rrt=rt;
	*rlen=len;
	return(bp);
}

BMID_API short *BGBMID_LoadWAV_44Stereo16(char *name, int *rlen)
{
	short *bp, *bp1;
	int rt, len;
	int i, j, k;

	bp=BGBMID_LoadWAV_Stereo16(name, &rt, &len);
	if(!bp)return(NULL);

	if(rt!=44100)
	{
		i=len*(44100.0/rt);
		bp1=bgbmid_malloc(i*2*sizeof(short));

//		BGBMID_ScaleSample(bp1, i, bp, len);

		bgbmid_free(bp);
		bp=bp1;
		len=i;
	}

	*rlen=len;
	return(bp);
}


BMID_API int BGBMID_NormalizeSample(short *dst, short *src, int len)
{
	float c, r;
	int i, j, k;

	c=0;
	for(i=0; i<len; i++)
		c+=src[i];
	c/=len;

	r=0;
	for(i=0; i<len; i++)
		r+=fabs(src[i]-c);
	r/=len;

	if(r<1)r=1;

	for(i=0; i<len; i++)
	{
		j=(src[i]-c)*(4096.0/r);
		if(j>32767)j=32767;
		if(j<-32767)j=-32767;
		dst[i]=j;
	}

	return(0);
}

BMID_API double BGBMID_SplinePolate(short *samp, int len, double t);
BMID_API double BGBMID_SplinePolateF(float *samp, int len, double t);

BMID_API int BGBMID_ScaleSampleTrilinear(short *d, int dl, short *s, int sl)
{
	float *tb, *tb1, *tb2;
	float s1, s2, s3;
	float r, r1, p1, p2, f, g;
	int v, tl;
	int i, j, k;

	//first: upscale sample to a power of 2 size
	tl=1;
	while(sl>tl)tl<<=1;

	tb=bgbmid_malloc(tl*sizeof(float));
	tb1=bgbmid_malloc(tl*sizeof(float));

//	for(i=0; i<sl; i++)
//		tb[i]=s[i];
//	BGBMID_ScaleSample(tb, tl, s, sl);

	r=(((float)(sl-1))/((float)tl));
	for(i=0; i<tl; i++)
	{
		p1=i*r; j=floor(p1);
		p2=p1-j;
		s1=s[j]; s2=s[j+1];
		f=(s1*(1-p2))+(s2*p2);

//		f=BGBMID_SplinePolate(s, sl, i*r);
		tb[i]=f;
	}


	//second: downscale by powers of 2

	while(tl>dl)
	{
		tl>>=1;
		for(j=0; j<tl; j++)
		{
			f=(tb[j*2+0]+tb[j*2+1])/2;
			tb1[j]=f;
		}

		tb2=tb; tb=tb1; tb1=tb2;
	}

	if(tl==dl)
	{
		//temp len == dest len
		for(i=0; i<dl; i++)
		{
			v=tb[i];
			if(v<(-32767))v=-32767;
			if(v>32767)v=32767;
			d[i]=v;
		}
		return(0);
	}

	//use trilinear upsample filter
	r=(((float)tl)/((float)dl));
	r1=(((float)sl)/((float)dl));
	for(i=0; i<dl; i++)
	{
#if 1
		p1=i*r;
		j=floor(p1);
		p2=p1-j;

		k=floor(i*r1+0.5);

		s1=tb[j];
		s2=tb[j+1];
		s3=s[k];
//		v=((s1*(1-p2))+(s2*p2))*0.666666+(s3*0.333333);
		v=((s1*(1-p2))+(s2*p2))*0.5+(s3*0.5);
#endif

//		f=BGBMID_SplinePolateF(tb, tl, i*r);
//		g=BGBMID_SplinePolate(s, sl, i*r1);
//		v=0.5*f+0.5*g;

		if(v<(-32767))v=-32767;
		if(v>32767)v=32767;
		d[i]=v;
	}

	bgbmid_free(tb);
	bgbmid_free(tb1);

	return(0);
}

BMID_API double BGBMID_Sinc(double x)
{
	if(x==0)return(1);
	return(sin(M_PI*x)/(M_PI*x));
}

BMID_API double BGBMID_SincPolate(short *samp, int len, double f, int n)
{
	double p, g;
	int i, j, k, ws, we;

	i=floor(f);
	if(i==f)return(samp[i]);

	ws=i-n;
	we=i+n;
	if(ws<0)ws=0;
	if(we>len)we=len;

	p=0;
	for(i=ws; i<we; i++)
	{
//		p+=samp[i]*BGBMID_Sinc(f-i);

		g=M_PI*(f-i);
		p+=samp[i]*sin(g)/g;
	}

	return(p);
}

BMID_API int BGBMID_ScaleSampleSinc(short *d, int dl, short *s, int sl)
{
	double r;
	int i, j;

	r=(((double)sl)/((double)dl));
	for(i=0; i<dl; i++)
	{
		j=BGBMID_SincPolate(s, sl, i*r, 64);
		if(j>32767)j=32767;
		if(j<-32768)j=-32768;
		d[i]=j;
	}
	return(0);
}

BMID_API double BGBMID_SplinePolate(short *samp, int len, double t)
{
	double f, g, u, v;
	double a, b, c, d;
	double p;
	int i;

	i=floor(t);

	if(i<1)
	{
		if(i<0)return(samp[0]);
		f=t-i; g=1.0-f;
		a=samp[0]; b=samp[1];
		p=(a*g)+(b*f);
		return(p);
	}
	if(i>=(len-2))
	{
		if(i>=(len-1))
			return(samp[len-1]);
		f=t-i; g=1.0-f;
		a=samp[i+0]; b=samp[i+1];
		p=(a*g)+(b*f);
		return(p);
	}

	f=t-i; g=1.0-f;

	a=samp[i-1];	b=samp[i+0];
	c=samp[i+1];	d=samp[i+2];
	u=b+f*(b-a);
	v=c+g*(c-d);
	p=(u*g)+(v*f);

	return(p);
}

BMID_API double BGBMID_SplinePolateF(float *samp, int len, double t)
{
	double f, g, u, v;
	double a, b, c, d;
	double p;
	int i;

	i=floor(t);
	f=t-i; g=1.0-f;

	a=samp[i-1];	b=samp[i+0];
	c=samp[i+1];	d=samp[i+2];
	u=b+f*(b-a);
	v=c+g*(c-d);
	p=(u*g)+(v*f);

	return(p);
}

BMID_API double BGBMID_SplinePolateLoop(short *samp, int len, double t)
{
	double f, g, u, v;
	double a, b, c, d;
	double p;
	int i;

	i=t; f=t-i; g=1.0-f;

	if(i<=0)
	{
		i+=len*((len-i)/len);
	}else i=i%len;

	if((i+2)>=len)
	{
		a=samp[(i-1)%len];	b=samp[(i+0)%len];
		c=samp[(i+1)%len];	d=samp[(i+2)%len];
		u=b+f*(b-a);
		v=c+g*(c-d);
		p=(u*g)+(v*f);

//		if(isnan(p))*(int *)-1=-1;

		return(p);
	}

//	if(i<1)return(0);
//	if(i>=(len-2))return(0);

	a=samp[i-1];	b=samp[i+0];
	c=samp[i+1];	d=samp[i+2];
	u=b+f*(b-a);
	v=c+g*(c-d);
	p=(u*g)+(v*f);

//	if(isnan(p))*(int *)-1=-1;

	return(p);
}

BMID_API int BGBMID_ScaleSampleSpline(short *d, int dl, short *s, int sl)
{
	double r;
	int i, j;

	r=(((double)sl)/((double)dl));
	for(i=0; i<dl; i++)
	{
		j=BGBMID_SplinePolate(s, sl, i*r);
		if(j>32767)j=32767;
		if(j<-32768)j=-32768;
		d[i]=j;
	}
	return(0);
}

BMID_API int BGBMID_ScaleSample(short *d, int dl, short *s, int sl)
{
	int i, j, k, s1, s2, v;
	float r, p1, p2;

	if(dl==sl)
	{
		for(i=0; i<sl; i++)d[i]=s[i];
		return(0);
	}

#if 0
	//upsample, sinc
	if(dl>sl)
	{
		r=(((float)sl)/((float)dl));
		for(i=0; i<dl; i++)
		{
			v=BGBMID_SincPolate(s, sl, i*r, 64);
			d[i]=v;
		}
		return(0);
	}
#endif

#if 1
	//upsample, linear
	if(dl>sl)
	{
		r=(((float)(sl-1))/((float)dl));
		for(i=0; i<dl; i++)
		{
			p1=i*r;
			j=floor(p1);
			p2=p1-j;

			s1=s[j];
			s2=s[j+1];
			v=(s1*(1-p2))+(s2*p2);

			d[i]=v;
		}
		return(0);
	}
#endif

#if 0
	r=(((float)sl)/((float)dl));
	for(i=0; i<dl; i++)
	{
		j=floor(i*r);
		d[i]=s[j];
	}
	return(0);
#endif

	//downsample, trilinear
	BGBMID_ScaleSampleTrilinear(d, dl, s, sl);
	return(0);
}

BMID_API int BGBMID_ScaleSampleHQ(short *d, int dl, short *s, int sl)
{
	int i;

	if(dl==sl)
	{
		for(i=0; i<sl; i++)d[i]=s[i];
		return(0);
	}

	if(dl>sl)
	{
		BGBMID_ScaleSampleSpline(d, dl, s, sl);
		return(0);
	}

	//downsample, trilinear
	BGBMID_ScaleSampleTrilinear(d, dl, s, sl);
	return(0);
}

BMID_API int BGBMID_SampleCompressor(short *db, int dl, short *sb, int sl,
	int ivl, int isl, int irl)
{
//	static short *buf1=NULL;
//	static short *buf2=NULL;
	short *buf1, *buf2, *buf3;
	int sp, dp;
	int i, j, k, l;

	i=irl*3*sizeof(short);
	buf1=bgbmid_malloc(i);
	buf2=bgbmid_malloc(i);
	memset(buf1, 0, i);
	memset(buf2, 0, i);

	buf3=bgbmid_malloc(ivl*sizeof(short));

	sp=0; dp=0;
	while((sp<sl) && (dp<dl))
	{
		for(i=0; i<ivl; i++)
			buf3[i]=sb[sp+(i%(sl-sp))];

		j=isl; k=0;
		while(k<irl)
		{
			BGBMID_ScaleSample(buf2+k, isl, buf3, ivl);

			if(k)
			{
				l=(buf2[k-1]+buf2[k])/2;
				buf2[k-1]=(buf2[k-1]+l)/2;
				buf2[k-2]=(buf2[k-2]+buf2[k-1])/2;
				buf2[k-3]=(buf2[k-3]+buf2[k-2])/2;
				buf2[k]=(buf2[k]+l)/2;
				buf2[k+1]=(buf2[k+1]+buf2[k])/2;
				buf2[k+2]=(buf2[k+2]+buf2[k+1])/2;
				buf2[k+3]=(buf2[k+3]+buf2[k+2])/2;
			}

			k+=isl;
		}

		j=irl*2;
		for(i=0; i<j; i++)
		{
			k=buf1[i+irl];
			k+=buf2[i];
			if(k<(-32767))k=-32767;
			if(k>32767)k=32767;
			buf1[i]=k;
		}

		for(i=0; i<irl; i++)db[dp++]=buf1[i];
		sp+=ivl;
	}

	bgbmid_free(buf1);
	bgbmid_free(buf2);
	bgbmid_free(buf3);

	return(0);
}

BMID_API int BGBMID_Wave_UpdateHistogram(
	float *hist, float val, float lval)
{
	float f, g, h;
	int i, j, k;

	for(j=0; j<15; j++)
	{
		h=	hist[j]*0.37+
			hist[j+1]*0.63;
		hist[j]=h;
	}
	hist[15]=
		hist[15]*0.37+(val)*0.63;
//		hist[15]*0.37+(val-lval)*0.63;
	return(0);
}

BMID_API int BGBMID_Wave_GenerateHistogram(
	float *hist, short *sb, int sl)
{
	float h, lh;
	int i, j, k;
	
	for(i=0; i<16; i++)
		hist[i]=0;
	lh=0;

	for(i=0; i<sl; i++)
	{
		h=sb[i];
		BGBMID_Wave_UpdateHistogram(hist, h, lh);
		lh=h;
	}
	return(0);
}
