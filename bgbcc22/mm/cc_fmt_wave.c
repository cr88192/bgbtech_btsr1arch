#define BGBCC_RIFF_RIFF 0x46464952
#define BGBCC_RIFF_WAVE 0x45564157
#define BGBCC_RIFF_fmt  0x20746D66
#define BGBCC_RIFF_data 0x61746164

int BGBCC_WAVE_ParseHeader(byte *ibuf,
	int *rfofs, int *rfsz,
	int *rdofs, int *rdsz)
{
	u32 tag1, tag2;
	int sz1, sz2, ofs1, ofs1e, ofs2;
	
	tag1=((u32 *)ibuf)[0];
	sz1=((u32 *)ibuf)[1];
	tag2=((u32 *)ibuf)[2];
	
	if(tag1==BGBCC_RIFF_WAVE)
	{
		ofs2=((u32 *)ibuf)[2];
		sz2=sz1-ofs2;

		*rfofs=0x10;
		*rfsz=ofs2-0x10;
		*rdofs=ofs2;
		*rdsz=sz2;
		return(1);
	}

	*rfofs=0;
	*rfsz=0;
	*rdofs=0;
	*rdsz=0;

	if(	(tag1!=BGBCC_RIFF_RIFF) ||
		(tag2!=BGBCC_RIFF_WAVE))
	{
		return(-1);
	}
	
	ofs1=12; ofs1e=12+sz1;
	while(ofs1<ofs1e)
	{
		tag2=((u32 *)(ibuf+ofs1))[0];
		sz2=((u32 *)(ibuf+ofs1))[1];
		ofs2=ofs1+8;
		
		if(sz2<0)
			break;
		
		if(tag2==BGBCC_RIFF_fmt)
		{
			*rfofs=ofs2;
			*rfsz=sz2;
		}

		if(tag2==BGBCC_RIFF_data)
		{
			*rdofs=ofs2;
			*rdsz=sz2;
		}
		
		ofs1=ofs1+8+((sz2+1)&(~1));
	}

	return(1);
}

byte *BGBCC_WAVE_LoadWAV(byte *ibuf, int ibufsz,
	int *rch, int *rrt, int *rbts, int *rlen, int *rfmt, int *rblka)
{
	byte *pfmt, *pdat, *dat1;
	int fofs, fsz, dofs, dsz, ret, nblk, blen;
	int ch, rt, bts, bsz, len, fmt, bsz1;
	int i, j, k;
	
	fofs=0; fsz=0; dofs=0; dsz=0;
	ret=BGBCC_WAVE_ParseHeader(ibuf, &fofs, &fsz, &dofs, &dsz);
	if(ret<=0)
		return(NULL);
	
	pfmt=ibuf+fofs;
	pdat=ibuf+dofs;
	
	dat1=bgbcc_tmalloc2("misc_fmt_wave", dsz);
	memcpy(dat1, pdat, dsz);
	
	fmt=pfmt[0]+(pfmt[1]<<8);
	ch=pfmt[2];
	rt=pfmt[4]+(pfmt[5]<<8);
	bts=pfmt[14];
	bsz=pfmt[12]+(pfmt[13]<<8);

	if(	(fmt==BGBCC_WAVE_FMT_PCM) ||
		(fmt==BGBCC_WAVE_FMT_ALAW) ||
		(fmt==BGBCC_WAVE_FMT_ULAW))
	{
		len=dsz/(ch*bts/8);
	}
	
	if(fmt==BGBCC_WAVE_FMT_ADLQ)
	{
		nblk=dsz/bsz;
		bsz1=bsz;
		if(ch==2)
		{
			for(i=0; i<10; i++)
			{
				j=1<<(i+3);
				k=j+(j>>2);
				if(bsz==k)
					{ bsz1=j; break; }
			}
		}
		blen=(bsz1-2)*4;
		len=nblk*blen;
	}
	
	if(fmt==BGBCC_WAVE_FMT_IMAADPCM)
	{
		nblk=dsz/bsz;
		if(bts==4)
		{
			blen=(bsz-4)*2;
			if(ch==2)
				blen=(bsz-8)*1;
		}
		if(bts==2)
		{
			blen=(bsz-4)*4;
			if(ch==2)
				blen=(bsz-8)*2;
		}
		len=nblk*blen;
	}
	
	*rch=ch;
	*rrt=rt;
	*rbts=bts;
	*rlen=len;
	*rfmt=fmt;
	*rblka=bsz;
	return(dat1);
}

short *BGBCC_WAVE_LoadWAV16(
	byte *ibuf, int ibufsz,
	int *rch, int *rrt, int *rlen)
{
	byte *bpb;
	short *bp, *bp1;
	int ch, rt, bts, len, fmt, blksz;
	int i, j, k;

	fmt=1;
	bpb=BGBCC_WAVE_LoadWAV(ibuf, ibufsz, &ch, &rt, &bts, &len, &fmt, &blksz);
	if(!bpb)return(NULL);

	bp=NULL;

	if(fmt==BGBCC_WAVE_FMT_PCM)
	{
		if(bts==8)
		{
			bp=bgbcc_tmalloc2("misc_fmt_wave", ch*len*sizeof(short)+1024);

			for(i=0; i<(ch*len); i++)
			{
				j=bpb[i]; j=(j-128)*256;
				bp[i]=j;
			}

			free(bpb);
		}
		if(bts==16)bp=(short *)bpb;
	}

	if(fmt==BGBCC_WAVE_FMT_ALAW)
	{
		bp=bgbcc_tmalloc2("misc_fmt_wave", ch*len*sizeof(short)+1024);

		for(i=0; i<(ch*len); i++)
		{
			j=bpb[i];
//			j=bgbcc_wave_alaw2samp(j^0x55);
			j=bgbcc_wave_alaw2samp(j^0xD5);
			bp[i]=j;
		}

		free(bpb);
	}

	if(fmt==BGBCC_WAVE_FMT_ADLQ)
	{
		bp=bgbcc_tmalloc2("misc_fmt_wave", ch*len*sizeof(short)+1024);
		BGBCC_WAVE_DecodeStreamAdlq(bp, bpb, ch, len);
		free(bpb);
	}

	if(fmt==BGBCC_WAVE_FMT_IMAADPCM)
	{
		bp=bgbcc_tmalloc2("misc_fmt_wave", ch*len*sizeof(short)+1024);
		if(bts==4)
			BGBCC_WAVE_DecodeStreamImaAdpcm(bp, bpb, ch, len, blksz);
		if(bts==2)
			BGBCC_WAVE_DecodeStreamImaAdpcm2b(bp, bpb, ch, len, blksz);
		free(bpb);
	}

	if(!bp)
	{
		free(bpb);
		printf("BGBCC_WAVE_LoadWAV16: WAVE not 8 or 16 bits\n");
		return(NULL);
	}

	*rch=ch;
	*rrt=rt;
	*rlen=len;
	return(bp);
}

short *BGBCC_WAVE_LoadWAV_Mono16(
	byte *ibuf, int ibufsz,
	int *rrt, int *rlen)
{
	byte *bpb;
	short *bp, *bp1;
	int ch, rt, bts, len;
	int i, j, k;

	bp=BGBCC_WAVE_LoadWAV16(ibuf, ibufsz, &ch, &rt, &len);

#if 0
	bpb=BGBCC_WAVE_LoadWAV(ibuf, ibufsz, &ch, &rt, &bts, &len);
	if(!bpb)return(NULL);

	bp=NULL;
	if(bts==8)
	{
		bp=bgbcc_tmalloc2("misc_fmt_wave", ch*len*sizeof(short)+1024);

		for(i=0; i<(ch*len); i++)
		{
			j=bpb[i]; j=(j-128)*256;
			bp[i]=j;
		}

		free(bpb);
	}
	if(bts==16)bp=(short *)bpb;
#endif

	if(!bp)
	{
//		free(bpb);
		printf("BGBCC_WAVE_LoadWAV_Mono16: WAVE not 8 or 16 bits\n");
		return(NULL);
	}

	if(ch<1)
	{
//		free(bpb);
		printf("BGBCC_WAVE_LoadWAV_Mono16: WAVE bad number of channels\n");
		return(NULL);
	}

	if(ch>1)
	{
		bp1=bgbcc_tmalloc2("misc_fmt_wave", len*sizeof(short));
		for(i=0; i<len; i++)
		{
			k=0;
			for(j=0; j<ch; j++)
				k+=bp[i*ch+j];
			if(k<-32767)k=-32767;
			if(k>32767)k=32767;
			bp1[i]=k;
		}

		free(bp);
		bp=bp1;
	}

	*rrt=rt;
	*rlen=len;
	return(bp);
}

short *BGBCC_WAVE_LoadWAV_Stereo16(
	byte *ibuf, int ibufsz, int *rrt, int *rlen)
{
	byte *bpb;
	short *bp, *bp1;
	int ch, rt, bts, len;
	int i, j, k;

	bp=BGBCC_WAVE_LoadWAV16(ibuf, ibufsz, &ch, &rt, &len);

#if 0
	bpb=BGBCC_WAVE_LoadWAV(ibuf, ibufsz, &ch, &rt, &bts, &len);
	if(!bpb)return(NULL);

	bp=NULL;
	if(bts==8)
	{
		bp=bgbcc_tmalloc2("misc_fmt_wave", ch*len*sizeof(short));

		for(i=0; i<(ch*len); i++)
		{
			j=bpb[i]; j=(j-128)*256;
			bp[i]=j;
		}

		free(bpb);
	}
	if(bts==16)bp=(short *)bpb;
#endif

	if(!bp)
	{
//		free(bpb);
		printf("BGBCC_WAVE_LoadWAV_Stereo16: WAVE not 8 or 16 bits\n");
		return(NULL);
	}

	if(ch<1)
	{
//		free(bpb);
		printf("BGBCC_WAVE_LoadWAV_Stereo16: WAVE bad number of channels\n");
		return(NULL);
	}

	if(ch==1)
	{
		bp1=bgbcc_tmalloc2("misc_fmt_wave", len*2*sizeof(short));
		for(i=0; i<len; i++)
		{
			k=bp[i];
			bp1[i*2+0]=k;
			bp1[i*2+1]=k;
		}

		free(bp);
		bp=bp1;
	}

	*rrt=rt;
	*rlen=len;
	return(bp);
}

short *BGBCC_WAVE_LoadWAV_RateMono16(
	byte *ibuf, int ibufsz,
	int rate, int *rlen)
{
	short *bp, *bp1;
	int rt, len;
	int i, j, k;

	bp=BGBCC_WAVE_LoadWAV_Mono16(ibuf, ibufsz, &rt, &len);
	if(!bp)return(NULL);

	if(rt!=rate)
	{
		i=len*((1.0*rate)/rt);
		bp1=bgbcc_tmalloc2("misc_fmt_wave", i*sizeof(short));

		BGBCC_WAVE_ScaleSampleMono(bp1, i, bp, len);

		free(bp);
		bp=bp1;
		len=i;
	}

	*rlen=len;
	return(bp);
}

short *BGBCC_WAVE_LoadWAV_RateStereo16(
	byte *ibuf, int ibufsz,
	int rate, int *rlen)
{
	short *bp, *bp1;
	int rt, len;
	int i, j, k;

	bp=BGBCC_WAVE_LoadWAV_Stereo16(ibuf, ibufsz, &rt, &len);
	if(!bp)return(NULL);

	if(rt!=rate)
	{
		i=len*((1.0*rate)/rt);
		bp1=bgbcc_tmalloc2("misc_fmt_wave", i*2*sizeof(short));

		BGBCC_WAVE_ScaleSampleStereo(bp1, i, bp, len);

		free(bp);
		bp=bp1;
		len=i;
	}

	*rlen=len;
	return(bp);
}

short *BGBCC_WAVE_LoadWAV_16Mono16(
	byte *ibuf, int ibufsz, int *rlen)
{
	return(BGBCC_WAVE_LoadWAV_RateMono16(ibuf, ibufsz, 16000, rlen));
}

short *BGBCC_WAVE_LoadWAV_16Stereo16(
	byte *ibuf, int ibufsz, int *rlen)
{
	return(BGBCC_WAVE_LoadWAV_RateStereo16(ibuf, ibufsz, 16000, rlen));
}

int BGBCC_WAVE_ScaleSampleTrilinearMono(short *d, int dl, short *s, int sl)
{
	float *tb, *tb1, *tb2;
	float s1, s2, s3;
	float r, r1, p1, p2, f, g;
	int v, tl;
	int i, j, k;

	//first: upscale sample to a power of 2 size
	tl=1;
	while(sl>tl)tl<<=1;

	tb=bgbcc_tmalloc2("misc_fmt_wave", tl*sizeof(float));
	tb1=bgbcc_tmalloc2("misc_fmt_wave", tl*sizeof(float));

	r=(((float)(sl-1))/((float)tl));
	for(i=0; i<tl; i++)
	{
		p1=i*r; j=floor(p1);
		p2=p1-j;
		s1=s[j]; s2=s[j+1];
		f=(s1*(1-p2))+(s2*p2);
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
		p1=i*r;
		j=floor(p1);
		p2=p1-j;

		k=floor(i*r1+0.5);

		s1=tb[j];
		s2=tb[j+1];
		s3=s[k];
		v=((s1*(1-p2))+(s2*p2))*0.5+(s3*0.5);

		if(v<(-32767))v=-32767;
		if(v>32767)v=32767;
		d[i]=v;
	}

	free(tb);
	free(tb1);

	return(0);
}

int BGBCC_WAVE_ScaleSampleTrilinearStereo(
	short *d, int dl, short *s, int sl)
{
	float *tb, *tb1, *tb2;
	float s1, s2, s3;
	float r, r1, p1, p2, f, g;
	int v, tl;
	int i, j, k;

	//first: upscale sample to a power of 2 size
	tl=1;
	while(sl>tl)tl<<=1;

	tb=bgbcc_tmalloc2("misc_fmt_wave", tl*2*sizeof(float));
	tb1=bgbcc_tmalloc2("misc_fmt_wave", tl*2*sizeof(float));

	r=(((float)(sl-1))/((float)tl));
	for(i=0; i<tl; i++)
	{
		p1=i*r; j=floor(p1);
		p2=p1-j;

		s1=s[(j+0)*2+0];	s2=s[(j+1)*2+0];
		f=(s1*(1-p2))+(s2*p2);
		tb[i*2+0]=f;

		s1=s[(j+0)*2+1];	s2=s[(j+1)*2+1];
		f=(s1*(1-p2))+(s2*p2);
		tb[i*2+1]=f;
	}


	//second: downscale by powers of 2

	while(tl>dl)
	{
		tl>>=1;
		for(j=0; j<tl; j++)
		{
			f=(tb[(j*2+0)*2+0]+tb[(j*2+1)*2+0])/2;
			g=(tb[(j*2+0)*2+1]+tb[(j*2+1)*2+1])/2;
			tb1[j*2+0]=f;
			tb1[j*2+1]=g;
		}

		tb2=tb; tb=tb1; tb1=tb2;
	}

	if(tl==dl)
	{
		//temp len == dest len
		for(i=0; i<dl; i++)
		{
			v=tb[i*2+0];
			if(v<(-32767))v=-32767;
			if(v>32767)v=32767;
			d[i*2+0]=v;

			v=tb[i*2+1];
			if(v<(-32767))v=-32767;
			if(v>32767)v=32767;
			d[i*2+1]=v;
		}
		return(0);
	}

	//use trilinear upsample filter
	r=(((float)tl)/((float)dl));
	r1=(((float)sl)/((float)dl));
	for(i=0; i<dl; i++)
	{
		p1=i*r;
		j=floor(p1);
		p2=p1-j;

		k=floor(i*r1+0.5);


		s1=tb[(j+0)*2+0];
		s2=tb[(j+1)*2+0];
		s3=s[k*2+0];
		v=((s1*(1-p2))+(s2*p2))*0.5+(s3*0.5);

		if(v<(-32767))v=-32767;
		if(v>32767)v=32767;
		d[i*2+0]=v;

		s1=tb[(j+0)*2+1];
		s2=tb[(j+1)*2+1];
		s3=s[k*2+1];
		v=((s1*(1-p2))+(s2*p2))*0.5+(s3*0.5);

		if(v<(-32767))v=-32767;
		if(v>32767)v=32767;
		d[i*2+1]=v;
	}

	free(tb);
	free(tb1);

	return(0);
}

int BGBCC_WAVE_ScaleSampleMono(short *d, int dl, short *s, int sl)
{
	int i, j, k, s1, s2, v;
	float r, p1, p2;

	if(dl==sl)
	{
		for(i=0; i<sl; i++)d[i]=s[i];
		return(0);
	}

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

	//downsample, trilinear
	BGBCC_WAVE_ScaleSampleTrilinearMono(d, dl, s, sl);
	return(0);
}

int BGBCC_WAVE_ScaleSampleStereo(short *d, int dl, short *s, int sl)
{
	int i, j, k, s1, s2, vl, vr;
	float r, p1, p2;

	if(dl==sl)
	{
		for(i=0; i<sl; i++)
		{
			d[i*2+0]=s[i*2+0];
			d[i*2+1]=s[i*2+1];
		}
		return(0);
	}

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

			s1=s[(j+0)*2+0];
			s2=s[(j+1)*2+0];
			vl=(s1*(1-p2))+(s2*p2);

			s1=s[(j+0)*2+1];
			s2=s[(j+1)*2+1];
			vr=(s1*(1-p2))+(s2*p2);

			d[i*2+0]=vl;
			d[i*2+1]=vr;
		}
		return(0);
	}
#endif

#if 0
	r=(((float)sl)/((float)dl));
	for(i=0; i<dl; i++)
	{
		j=floor(i*r);
		d[i*2+0]=s[j*2+0];
		d[i*2+1]=s[j*2+1];
	}
	return(0);
#endif

	//downsample, trilinear
	BGBCC_WAVE_ScaleSampleTrilinearStereo(d, dl, s, sl);
	return(0);
}


int BGBCC_WAVE_StoreWaveFmtI(
	byte *obuf, byte *ibuf, int wft, int ch, int rt, int bits, int len)
{
	byte *ct;
	int i;

	ct=obuf;

	*ct++='R';	*ct++='I';
	*ct++='F';	*ct++='F';
	*ct++=0;	*ct++=0;
	*ct++=0;	*ct++=0;
	*ct++='W';	*ct++='A';
	*ct++='V';	*ct++='E';

	//format header
	*ct++='f';	*ct++='m';
	*ct++='t';	*ct++=' ';
	*ct++=16;	*ct++=0;
	*ct++=0;	*ct++=0;

	*ct++=wft;	*ct++=wft>>8;		//wFormat
	*ct++=ch;	*ct++=0;			//nChannels

	*ct++=rt;		*ct++=rt>> 8;	//nSamplesPerSec
	*ct++=rt>>16;	*ct++=rt>>24;

	i=rt*ch*(bits/8);
	*ct++=i;		*ct++=i>> 8;	//nAvgBytesPerSec
	*ct++=i>>16;	*ct++=i>>24;

	*ct++=ch*(bits/8);	*ct++=0;	//nBlockAlign
	*ct++=bits;			*ct++=0;	//wBitsPerSample


	//data

	i=len*ch*(bits/8);

	*ct++='d';	*ct++='a';
	*ct++='t';	*ct++='a';
	*ct++=i;		*ct++=i>> 8;
	*ct++=i>>16;	*ct++=i>>24;

	memcpy(ct, ibuf, i);
	ct+=i;
	if(i&1)
		*ct++=0;
	
	i=(ct-obuf)-8;
	obuf[4]=i;		obuf[5]=i>> 8;
	obuf[6]=i>>16;	obuf[7]=i>>24;
	
	return(ct-obuf);
}

int BGBCC_WAVE_StoreWaveFmtBlockI(
	byte *obuf, byte *ibuf, int ibsz,
	int wft, int ch, int rt, int bits, int len,
	int blksz, int realbits)
{
	byte *ct;
	int i;

	ct=obuf;

	*ct++='R';	*ct++='I';
	*ct++='F';	*ct++='F';
	*ct++=0;	*ct++=0;
	*ct++=0;	*ct++=0;
	*ct++='W';	*ct++='A';
	*ct++='V';	*ct++='E';

	//format header
	*ct++='f';	*ct++='m';
	*ct++='t';	*ct++=' ';
	*ct++=20;	*ct++=0;
	*ct++=0;	*ct++=0;

	*ct++=wft;	*ct++=wft>>8;		//wFormat
	*ct++=ch;	*ct++=0;			//nChannels

	*ct++=rt;		*ct++=rt>> 8;	//nSamplesPerSec
	*ct++=rt>>16;	*ct++=rt>>24;

	i=(rt*realbits)/8;
	*ct++=i;		*ct++=i>> 8;	//nAvgBytesPerSec
	*ct++=i>>16;	*ct++=i>>24;

	*ct++=blksz;		*ct++=blksz>>8;	//nBlockAlign
	*ct++=bits;			*ct++=0;		//wBitsPerSample

	*ct++=2;	*ct++=0;		//cbSize
//	*ct++=0xF9;	*ct++=0x07;
	*ct++=0xF9;	*ct++=0x03;

	//fact
	*ct++='f';	*ct++='a';
	*ct++='c';	*ct++='t';
	*ct++=4;	*ct++=0;
	*ct++=0;	*ct++=0;

	*ct++=len;		*ct++=len>>8;
	*ct++=len>>16;	*ct++=len>>24;

	//data

	i=ibsz;

	*ct++='d';	*ct++='a';
	*ct++='t';	*ct++='a';
	*ct++=i;		*ct++=i>> 8;
	*ct++=i>>16;	*ct++=i>>24;

	memcpy(ct, ibuf, i);
	ct+=i;
	if(i&1)
		*ct++=0;
	
	i=(ct-obuf)-8;
	obuf[4]=i;		obuf[5]=i>> 8;
	obuf[6]=i>>16;	obuf[7]=i>>24;
	
	return(ct-obuf);
}

int BGBCC_WAVE_StoreWavePCM(
	byte *obuf, byte *ibuf, int ch, int rt, int bits, int len)
{
	return(BGBCC_WAVE_StoreWaveFmtI(obuf, ibuf,
		BGBCC_WAVE_FMT_PCM, ch, rt, bits, len));
}

int bgbcc_wave_samp2alaw(int val)
{
	int fr, ex, sg, v;
	
	fr=val; sg=0;
	if(val<0)
		{ fr=-val; sg=1; }
	
	if(fr<256)
	{
		v=(sg<<7)|(fr>>4);
		return(v);
	}
	
	fr>>=3; ex=0;
	while(fr>=32)
		{ fr>>=1; ex++; }
	v=(sg<<7)|(ex<<4)|(fr&15);
	
	if(ex>7)
	{
		v=(sg<<7)|0x7F;
	}
	
	return(v);
}

int bgbcc_wave_alaw2samp(int val)
{
	int fr, ex, sg, v;

	sg=(val>>7)&1;
	ex=(val>>4)&7;
	fr=16|(val&15);
	
	v=fr<<(ex+3);
	if(sg)
		v=-v;
	return(v);
}

int bgbcc_wave_samp2alaw_nq16(int val)
{
	int v0, v, vm;
	v0=bgbcc_wave_samp2alaw(val);
	v=(s16)(v0<<8);
	v^=(v>>16)&0x7FFF;
	return(v);
}

int bgbcc_wave_alaw2samp_nq16(int val)
{
	int v0;
	v0=(val>>8);
	v0^=(((s16)val)>>16)&0x7F;
	return(bgbcc_wave_alaw2samp(v0));
}

int bgbcc_wave_alaw2nq16(int val)
{
	int v0, v, vm;
	v0=val;
	v=(s16)(v0<<8);
	v^=(v>>16)&0x7FFF;
	return(v);
}

int bgbcc_wave_nq16toalaw(int val)
{
	int v0;
	v0=(val>>8);
	v0^=(((s16)val)>>16)&0x7F;
	return(v0);
}

int BGBCC_WAVE_StoreWaveCnvALaw(
	byte *obuf, s16 *ibuf, int ch, int rt, int len)
{
	byte *tbuf;
	s64 err;
	int d;
	int i, j, k, s0, s1, v, n;
	
	err=0;
	n=ch*len;
	tbuf=bgbcc_tmalloc2("misc_fmt_wave", n);
	for(i=0; i<n; i++)
	{
		s0=ibuf[i];
		v=bgbcc_wave_samp2alaw(s0);
//		tbuf[i]=bgbcc_wave_samp2alaw(ibuf[i])^0x55;
		tbuf[i]=v^0xD5;
		
		s1=bgbcc_wave_alaw2samp(v);
		d=s0-s1; d=d^(d>>31);
		err+=d;
	}
	
	err/=n;
	printf("BGBCC_WAVE_StoreWaveCnvALaw: Len=%d AvgErr=%d\n", len, (int)err);
	
	i=BGBCC_WAVE_StoreWaveFmtI(obuf, tbuf,
		BGBCC_WAVE_FMT_ALAW, ch, rt, 8, len);
	free(tbuf);
	return(i);
}


int BGBCC_WAVE_StoreWaveCnvPcm8(
	byte *obuf, s16 *ibuf, int ch, int rt, int len)
{
	byte *tbuf;
	s64 err;
	int d;
	int i, j, k, s0, s1, v, n;
	
	err=0;
	n=ch*len;
	tbuf=bgbcc_tmalloc2("misc_fmt_wave", n);
	for(i=0; i<n; i++)
	{
		s0=ibuf[i];
		v=s0>>8;
		tbuf[i]=(ibuf[i]>>8)+128;
		
		s1=v<<8;
		d=s0-s1; d=d^(d>>31);
		err+=d;
	}
	
	err/=n;
	printf("BGBCC_WAVE_StoreWaveCnvPcm8: Len=%d AvgErr=%d\n", len, (int)err);
	
	i=BGBCC_WAVE_StoreWaveFmtI(obuf, tbuf,
		BGBCC_WAVE_FMT_PCM, ch, rt, 8, len);
	free(tbuf);
	return(i);
}


static s64 bgbcc_wave_blockerr;

int BGBCC_WAVE_EncodeBlockAdlq(u16 *dst, s16 *src, int blksz,
	int *rpred, int *rdsc)
{
	static byte remap_3201[4]={3,2,0,1};
	u64 excl;
	int pred, dsc, dscf, dsc1, prd2;
	int pred_a0, dsc_a0, dsc1_a0, dscf_a0;
	int pred_a1, dsc_a1, dsc1_a1, dscf_a1;
	int pred_a2, dsc_a2, dsc1_a2, dscf_a2;
	int pred_a3, dsc_a3, dsc1_a3, dscf_a3;
	int s0, s1, s2, s3, d0, d1, d2, d3;
	int d, bd, bk, lbk, islqal;
	s64 err;
	int i, j, k, k0, k1, j0, j1, j2, ilen, stp, stp1, stp2;
	
	ilen=((blksz>>1)-1)*8;
	pred=*rpred;
	dsc=*rdsc;
	
	islqal=0;
//	islqal=1;

	k=blksz>>1;
	for(i=0; i<k; i++)
		dst[i]=0;

	for(i=0; i<8; i++)
	{
		k=((1<<(i+2))-1)*8;
		if(k==ilen)
			{ j=i; break; }
	}

//	pred=(src[0]+src[1]+2*pred)/4;
	pred=(src[0]+src[1])/2;
//	pred=(src[0]+pred)/2;
//	pred=src[0];


//	if(dsc<8)
//		dsc=8;
//	if(dsc>21)
//		dsc=21;

//	s0=src[i+0];
//	s0=src[i+1];
	s0=src[i+2];

	if(islqal)
	{
		pred=bgbcc_wave_samp2alaw_nq16(pred);
		s0=bgbcc_wave_samp2alaw_nq16(s0);
	}

	while(dsc<21)
	{
		dsc1=dsc>>1;
		k=((s0-pred)>>(dsc1+3))+2;
		if((k==0) || (k==3))
			{ dsc++; continue; }
		break;
	}

	while(dsc>8)
	{
		dsc1=dsc>>1;
		k=((s0-pred)>>(dsc1+2))+2;
		if((k==1) || (k==2))
			{ dsc--; continue; }
		break;
	}

	if(dsc<8)
		dsc=8;
	if(dsc>23)
		dsc=23;

#if 1
	bk=0;

	j0=bgbcc_wave_samp2alaw(pred);
	j1=j0+1; j2=j0-1;
	s0=bgbcc_wave_alaw2samp(j0);
	s1=bgbcc_wave_alaw2samp(j1);
	s2=bgbcc_wave_alaw2samp(j2);
	
	d0=s0-pred;	d0^=d0>>31;
	d1=s1-pred;	d1^=d1>>31;
	d2=s2-pred;	d2^=d2>>31;
	
	bd=d0; bk=j0;
	if(d1<bd) { bd=d1; bk=j1; }
	if(d2<bd) { bd=d2; bk=j2; }

	k0=bk;
#endif

	if(islqal)
		{ bk=(pred>>8)&0xFF; }


//	k=bgbcc_wave_samp2alaw(pred)|((dsc-4)<<8)|(j<<13);
//	k=bgbcc_wave_samp2alaw(pred)|((dsc-8)<<8)|(j<<13);
//	k=bgbcc_wave_samp2alaw(pred)|((dsc-6)<<8)|(j<<13);
	k=bk|((dsc-8)<<8)|(j<<13)|((islqal&1)<<12);
	dst[0]=k;
	if(islqal)
		pred=(s16)(bk<<8);
	else
		pred=bgbcc_wave_alaw2samp(k);
	err=0;
	bk=0;

	dscf=dsc<<1;

	for(i=0; i<ilen; i++)
	{

#if 1
		s0=src[i+0];
		s1=src[i+1];
		s2=src[i+2];
		s3=src[i+3];

		if(islqal)
		{
			s0=bgbcc_wave_samp2alaw_nq16(s0);
			s1=bgbcc_wave_samp2alaw_nq16(s1);
			s2=bgbcc_wave_samp2alaw_nq16(s2);
			s3=bgbcc_wave_samp2alaw_nq16(s3);
		}

		lbk=bk;
		excl=0;
		bk=-1;
		bd=999999999;
//		for(k=-1; k<64; k++)
		for(k=-4; k<64; k++)
//		for(k=0; k<64; k++)
//		for(k=0; k<256; k++)
		{
//			pred_a0=pred; dsc_a0=dsc;
			k0=k;

			/* Skip paths for which no valid path exists. */
			if(excl&(1ULL<<(k0&63)))
				continue;

			/* Initial guess, try extending last best path... */
			if(k<0)
				{ k0=(lbk>>2)|((k&3)<<4); }


			dsc1=dsc>>1;
			stp=(2|(dsc&1))<<dsc1;
//			dsc1=dsc>>2;
//			stp=(2|((dsc>>1)&1))<<dsc1;
			k1=(k0>>0)&3;
			j0=((s32)(k1<<31))>>31;
			j1=((s32)(k1<<30))>>31;
			stp=stp+((stp<<1)&j0);
			stp^=j1;
			dsc_a0=dsc+((k1&1)*2-1);
//			dsc_a0=dsc+((k1&1)*3-1);
//			dscf_a0=dscf+((k1&1)*3-1);	dsc_a0=dscf_a0>>1;
			pred_a0=pred+stp;

//			d0=(s0-pred_a0)>>2;
//			d0=(d0*d0)>>14;
			d0=(s0-pred_a0); d0=d0^(d0>>31);
			
			if(	(d0>bd) ||
				(pred_a0<(-32767)) || (pred_a0>( 32767)) ||
				(dsc_a0<2) || (dsc_a0>25))
			{
//				excl|=1<<(k&15);
				excl|=0x1111111111111111ULL<<(k&3);
				continue;
			}

			dsc1_a0=dsc_a0>>1;
			stp=(2|(dsc_a0&1))<<dsc1_a0;
//			dsc1_a0=dsc_a0>>2;
//			stp=(2|((dsc_a0>>1)&1))<<dsc1_a0;
			k1=(k0>>2)&3;
			j0=((s32)(k1<<31))>>31;
			j1=((s32)(k1<<30))>>31;
			stp=stp+((stp<<1)&j0);
			stp^=j1;
			dsc_a1=dsc_a0+((k1&1)*2-1);
//			dsc_a1=dsc_a0+((k1&1)*3-1);
//			dscf_a1=dscf_a0+((k1&1)*3-1);	dsc_a1=dscf_a1>>1;
			pred_a1=pred_a0+stp;

//			d1=(s1-pred_a1)>>2;
//			d1=(d1*d1)>>14;
			d1=(s1-pred_a1); d1=d1^(d1>>31);

			if(	((d0+d1)>bd) ||
				(pred_a1<(-32767)) || (pred_a1>( 32767)) ||
				(dsc_a1<2) || (dsc_a1>25) )
			{
				excl|=0x0001000100010001ULL<<(k&15);
				continue;
			}

			dsc1_a1=dsc_a1>>1;
			stp=(2|(dsc_a1&1))<<dsc1_a1;
//			dsc1_a1=dsc_a1>>2;
//			stp=(2|((dsc_a1>>1)&1))<<dsc1_a1;
			k1=(k0>>4)&3;
			j0=((s32)(k1<<31))>>31;
			j1=((s32)(k1<<30))>>31;
			stp=stp+((stp<<1)&j0);
			stp^=j1;
			dsc_a2=dsc_a1+((k1&1)*2-1);
//			dsc_a2=dsc_a1+((k1&1)*3-1);
//			dscf_a2=dscf_a1+((k1&1)*3-1);	dsc_a2=dscf_a2>>1;
			pred_a2=pred_a1+stp;

//			d2=(s2-pred_a2)>>2;
//			d2=(d2*d2)>>14;
			d2=(s2-pred_a2); d2=d2^(d2>>31);

			if(	((d0+d1+d2)>bd) ||
				(pred_a2<(-32767)) || (pred_a2>( 32767)) ||
				(dsc_a2<2) || (dsc_a2>25) )
			{
				excl|=0x0000000000000001ULL<<(k&63);
				continue;
			}

#if 0
			dsc1_a2=dsc_a2>>1;
			stp=(2|(dsc_a2&1))<<dsc1_a2;
			k1=(k0>>6)&3;
			j0=((s32)(k1<<31))>>31;
			j1=((s32)(k1<<30))>>31;
			stp=stp+((stp<<1)&j0);
			stp^=j1;
			dsc_a3=dsc_a2+((k1&1)*2-1);
//			dsc_a3=dsc_a2+((k1&1)*3-1);
			pred_a3=pred_a2+stp;

//			d3=(s3-pred_a3)>>2;
//			d3=(d3*d3)>>14;
			d3=(s3-pred_a3); d3=d3^(d3>>31);

			if((pred_a3<(-32767)) || (pred_a3>( 32767)))
				continue;
			if((dsc_a3<8) || (dsc_a3>23))
				continue;
#else
			d3=0;
#endif

			if(d0<0)
				continue;
			if(d1<0)
				continue;
			if(d2<0)
				continue;

//			d=((d0*8)+(d1*5)*(d2*3))>>4;
//			d=d0+d1+d2;
			d=d0+d1+d2+d3;
			
			if(d<0)
				continue;
			
			if(d<bd)
				{ bd=d; bk=k0; }

#if 0
			if(bk>=0)
			{
				if((k>=16) && ((bk&15)==((lbk>>2)&15)))
					break;

				j0=15&(~(1<<(bk&3)));
				j1=excl&(excl>>4)&(excl>>8)&(excl>>12)&15;
				if(j0==j1)
					break;
			}
#endif

		}
		
		if(bk<0)
		{
			bk=(pred<0)?0:2;
		}
		
		k=bk;
		
//		if(dsc>=23)			k&=~1;
		if(pred<-32767)		k=0;
		if(pred> 32767)		k=2;

		k1=k&3;
		dst[(i>>3)+1]|=k1<<((i&7)*2);

		dsc1=dsc>>1;
		stp=(2|(dsc&1))<<dsc1;
//		dsc1=dsc>>2;
//		stp=(2|((dsc>>1)&1))<<dsc1;
		
		j0=((s32)(k1<<31))>>31;
		j1=((s32)(k1<<30))>>31;

		stp=stp+((stp<<1)&j0);
		stp^=j1;
		dsc+=(k1&1)*2-1;
//		dsc+=(k1&1)*3-1;
//		dscf+=(k1&1)*3-1; dsc=dscf>>1;
//		dscf+=(k1&1)*4-2; dsc=dscf>>1;
		pred+=stp;
		
		d0=s0-pred;
		d0^=d0>>31;
		err+=d0;
#endif

#if 0
		j=src[i];
		s0=j;
//		if(j>= 24576)j= 24575;
//		if(j<=-24576)j=-24575;

		dsc1=dsc>>1;
//		k=((j-pred)>>(dsc1+4))+2;
		k=((j-pred)>>(dsc1+3))+2;
//		k=((j-pred)>>(dsc1+2))+2;
//		k=((j-pred)>>(dsc1+1))+2;
		if(k<0)k=0;
		if(k>3)k=3;

		stp1=(2|(dsc&1))<<dsc1;
		stp2=(stp1<<1)+stp1;

		if(dsc>21)
		{
			if(k==0)k++;
			if(k==3)k--;
		}

		if(dsc<9)
		{
			if(k==1)k--;
			if(k==2)k++;
		}

		while(1)
		{
			if(k==0)	{ prd2=pred+(~stp2); }
			if(k==1)	{ prd2=pred+(~stp1); }
			if(k==2)	{ prd2=pred+  stp1 ; }
			if(k==3)	{ prd2=pred+  stp2 ; }
			if((prd2>(-32768)) && (prd2<32767))
				break;
			if(k>=2)	{ k--; }
			else		{ k++; }
				
			if(dsc<9)
			{
				k=(pred>0)?0:3;
			}
		}
		
		if(k==0)	k1=3;
		if(k==1)	k1=2;
		if(k==2)	k1=0;
		if(k==3)	k1=1;
		dst[(i>>3)+1]|=k1<<((i&7)*2);

//		if(k==0)	{ pred+=~stp2; dsc++; }
//		if(k==1)	{ pred+=~stp1; dsc--; }
//		if(k==2)	{ pred+= stp1; dsc--; }
//		if(k==3)	{ pred+= stp2; dsc++; }

		if(k==0)	{ pred+=~stp2; dscf+=2; dsc=dscf>>1; }
		if(k==1)	{ pred+=~stp1; dscf-=1; dsc=dscf>>1; }
		if(k==2)	{ pred+= stp1; dscf-=1; dsc=dscf>>1; }
		if(k==3)	{ pred+= stp2; dscf+=2; dsc=dscf>>1; }

//		if(dsc<4)	dsc=4;
//		if(dsc>19)	dsc=19;
//		if(dsc<8)	dsc=8;
//		if(dsc>23)	dsc=23;

//		if(dsc<6)	dsc=6;
//		if(dsc>21)	dsc=21;

		d0=s0-pred;
		d0^=d0>>31;
		err+=d0;
#endif
	}

	err/=ilen;
	bgbcc_wave_blockerr+=err;

	*rpred=pred;
	*rdsc=dsc;
	return(1);
}

int BGBCC_WAVE_DecodeBlockAdlq(u16 *src, s16 *dst)
{
	int pred, bsz, dsc, dscf, dsc1, prd2;
	int i, j, k, j0, j1, ilen, stp, islqal;
	
	k=src[0];
	ilen=((1<<(((k>>13)&7)+2))-1)*8;
	bsz=(ilen+8)/4;
	
	islqal=(k>>12)&1;
	
//	dsc=4+((k>>8)&15);
	dsc=8+((k>>8)&15);
//	dsc=6+((k>>8)&15);
	pred=bgbcc_wave_alaw2samp(k);
	
	if(islqal)
		pred=(s16)(k<<8);

	dscf=dsc<<1;

	for(i=0; i<ilen; i++)
	{
		k=(src[(i>>3)+1]>>((i&7)*2))&3;

		dsc1=dsc>>1;
		stp=(2|(dsc&1))<<dsc1;
		
		j0=((s32)(k<<31))>>31;
		j1=((s32)(k<<30))>>31;

		stp=stp+((stp<<1)&j0);
		stp^=j1;
		dsc+=(k&1)*2-1;
//		dscf+=(k&1)*3-1;	dsc=dscf>>1;
		pred+=stp;

		if(islqal)
			{ dst[i]=bgbcc_wave_alaw2samp_nq16(pred); }
		else
			{ dst[i]=pred; }
	}

	return(bsz);
}

int BGBCC_WAVE_DecodeStreamAdlq(s16 *dst, byte *src, int ch, int len)
{
	s16 tbc[1024], tbs[1024];
	s16 *ct, *cte;
	u16 *cs;
	int tag, bsz, bsz1, blen;
	int i, j, k;
	
	if(ch==1)
	{
		ct=dst; cte=dst+len;
		cs=(u16 *)src;
		while(ct<cte)
		{
			bsz=BGBCC_WAVE_DecodeBlockAdlq(cs, ct);
			blen=(bsz-2)*4;
			cs+=bsz>>1;
			ct+=blen;
		}
	}
	
	if(ch==2)
	{
		ct=dst; cte=dst+len*2;
		cs=(u16 *)src;
		while(ct<cte)
		{
			bsz=BGBCC_WAVE_DecodeBlockAdlq(cs, tbc);
			blen=(bsz-2)*4;
			cs+=bsz>>1;
			bsz1=BGBCC_WAVE_DecodeBlockAdlq(cs, tbs);
			cs+=bsz1>>1;

			for(i=0; i<blen; i++)
			{
				ct[i*2+0]=tbc[i]+tbs[i>>2];
				ct[i*2+1]=tbc[i]-tbs[i>>2];
			}

			ct+=blen*2;
		}
	}
	
	return(0);
}

int BGBCC_WAVE_StoreWaveAdlq(
	byte *obuf, short *ibuf, int ch, int rt, int len)
{
	s16 tbc[1024], tbs[1024];
	short *cs, *cse;
	byte *ct;
	int pred1, pred2, dsc1, dsc2;
	int wft, blksz, nblk, blksz1;
	int vc0, vc1, vc2, vs, vcl, vcr;
	int i, j, k;

	wft=0xAD10;
	blksz=256;
	nblk=(len+1015)/1016;

	blksz1=blksz;
	if(ch==2)
		blksz1+=blksz>>2;

	ct=obuf;

	*ct++='W';	*ct++='A';
	*ct++='V';	*ct++='E';
	*ct++=0;	*ct++=0;
	*ct++=0;	*ct++=0;

	*ct++=32;	*ct++=0;
	*ct++=0;	*ct++=0;

	*ct++=0;	*ct++=0;
	*ct++=0;	*ct++=0;

	//format header
	*ct++=wft;	*ct++=wft>>8;			//wFormat
	*ct++=ch;	*ct++=0;				//nChannels
	*ct++=rt;		*ct++=rt>> 8;		//nSamplesPerSec
	*ct++=rt>>16;	*ct++=rt>>24;
	i=(rt*ch)/4;
	*ct++=i;		*ct++=i>> 8;		//nAvgBytesPerSec
	*ct++=i>>16;	*ct++=i>>24;
	*ct++=blksz1;	*ct++=blksz1>>8;	//nBlockAlign
	*ct++=16;		*ct++=0;			//wBitsPerSample

	//data

	cs=ibuf;	cse=ibuf+ch*len;
	pred1=ibuf[0];	pred2=ibuf[1];
	dsc1=10;			dsc2=10;
	bgbcc_wave_blockerr=0;
	
	for(i=0; i<nblk; i++)
	{
		if(ch==2)
		{
			for(j=0; j<1016; j++)
			{
				if((cs+j*2)>=cse)
				{
					tbc[j]=0;
					tbs[j]=0;
				}else
				{
					tbc[j]=(cs[j*2+0]+cs[j*2+1])/2;
					tbs[j]=(cs[j*2+0]-cs[j*2+1])/2;
				}
			}
			for(j=0; j<254; j++)
				{ tbs[j]=(tbs[j*4+0]+tbs[j*4+1]+tbs[j*4+2]+tbs[j*4+3])/4; }
			for(j=1; j<253; j++)
			{
				tbs[j]=(2*tbs[j]+tbs[j-1]+tbs[j+2])/4;
			}

			for(j=0; j<1016; j++)
			{
				if((cs+j*2)>=cse)
					break;
				vc0=(cs[j*2+0]+cs[j*2+1])/2;
				
				vcl=tbc[j]+tbs[j>>2];
				vcr=tbc[j]-tbs[j>>2];
				vc1=(vcl+vcr)/2;
				
				vc2=vc0+(vc0-vc1);
				if(vc2<-32767)	vc2=-32767;
				if(vc2> 32767)	vc2= 32767;
				tbc[j]=vc2;
			}
			
			BGBCC_WAVE_EncodeBlockAdlq((u16 *)ct, tbc, blksz, &pred1, &dsc1);
			ct+=blksz;
			BGBCC_WAVE_EncodeBlockAdlq((u16 *)ct, tbs, blksz>>2, &pred2, &dsc2);
			ct+=blksz>>2;
			cs+=2032;
		}else
		{
			if((cs+1016)>cse)
			{
				for(j=0; j<1016; j++)
				{
					if((cs+j)<cse)
						tbc[j]=cs[j];
					else
						tbc[j]=0;
				}
				BGBCC_WAVE_EncodeBlockAdlq(
					(u16 *)ct, tbc, blksz, &pred1, &dsc1);
			}else
			{
				BGBCC_WAVE_EncodeBlockAdlq(
					(u16 *)ct, cs, blksz, &pred1, &dsc1);
			}
			cs+=1016;
			ct+=blksz;
		}
	}
	
	k=bgbcc_wave_blockerr/nblk;
	printf("BGBCC_WAVE_StoreWaveAdlq: ch=%d len=%d AvgErr=%d\n", ch, len, k);
	
	i=ct-obuf;
	obuf[4]=i;		obuf[5]=i>> 8;
	obuf[6]=i>>16;	obuf[7]=i>>24;
	
	return(ct-obuf);
}

static int bgbmid_ima_index_table[16] = {
  -1, -1, -1, -1, 2, 4, 6, 8,
  -1, -1, -1, -1, 2, 4, 6, 8
}; 

//static int bgbmid_ima_step_table[89] = { 
static int bgbmid_ima_step_table[128] = { 
     7,     8,     9,    10,    11,    12,    13,    14,    16,    17, 
    19,    21,    23,    25,    28,    31,    34,    37,    41,    45, 
    50,    55,    60,    66,    73,    80,    88,    97,   107,   118, 
   130,   143,   157,   173,   190,   209,   230,   253,   279,   307,
   337,   371,   408,   449,   494,   544,   598,   658,   724,   796,
   876,   963,  1060,  1166,  1282,  1411,  1552,  1707,  1878,  2066, 
  2272,  2499,  2749,  3024,  3327,  3660,  4026,  4428,  4871,  5358,
  5894,  6484,  7132,  7845,  8630,  9493, 10442, 11487, 12635, 13899, 
 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767 
};

void BGBCC_MsImaAdpcm_DecodeBlockMono(
	byte *ibuf, s16 *obuf, int len)
{
	int pred, index, step, diff, uni, sni;
	int i, j;
	
	pred=(s16)(ibuf[0]+(ibuf[1]<<8));
	index=ibuf[2];
	
	step=bgbmid_ima_step_table[index&127];

	for(i=0; i<len; i++)
	{
		j=(ibuf[4+(i>>1)]>>((i&1)*4))&15;
		uni=j;
		sni=(j&8)?(-(j&7)):(j&7);
	
		index=index+bgbmid_ima_index_table[uni];
		index=(index<0)?0:((index>88)?88:index);
//		diff=(sni+0.5)*step/4;
		diff=((2*(uni&7)+1)*step)/8;
		if(uni&8)diff=-diff;
		pred=pred+diff;
		step=bgbmid_ima_step_table[index];

		pred=(pred<(-32768))?(-32768):((pred>32767)?32767:pred);
		obuf[i]=pred;
	}
}


void BGBCC_MsImaAdpcm_EncodeBlockMono(
	s16 *ibuf, byte *obuf, int len, int *ridx)
{
	int s0, s1, p0, p1, p2, d0, d1, d2, uni1, uni2;
	int uni0b, uni1b, uni2b, p0b, p1b, p2b, d0b, d1b, d2b;
	int stp0b, stp1b, stp2b, ix0b, ix1b, ix2b;
	int lsbt, lsbit;
	int pred, index, step, diff, uni, sni;
	int err;
	int i, j, k;
	
	pred=(ibuf[0]+ibuf[1])>>1;
	index=ridx?(*ridx):0;
	
	obuf[0]=pred&255;
	obuf[1]=(pred>>8)&255;
	obuf[2]=index;
	obuf[3]=0;
	
	memset(obuf+4, 0, (len+1)/2);
	
	step=bgbmid_ima_step_table[index&127];
	lsbt=0;
	err=0;

	for(i=0; i<len; i++)
	{
		j=ibuf[i];
		k=j-pred;
		s0=j;
		s1=ibuf[i+1];

		if(k>=0)
		{
			sni=(k*4)/step;
			uni=(sni>7)?7:sni;
		}else
		{
			sni=(-k*4)/step;
			uni=(sni>7)?15:(8|sni);
		}

#if 1
		uni1=uni+1;
		uni2=uni-1;

		if((uni^uni1)&(~7))uni1=uni;
		if((uni^uni2)&(~7))uni2=uni;

		diff=((2*(uni&7)+1)*step)/8;
		if(uni&8)diff=-diff;
		p0=pred+diff; d0=j-p0;
		d0=(d0<<1)^(d0>>31);

		diff=((2*(uni1&7)+1)*step)/8;
		if(uni1&8)diff=-diff;
		p1=pred+diff; d1=j-p1;
		d1=(d1<<1)^(d1>>31);

		diff=((2*(uni2&7)+1)*step)/8;
		if(uni2&8)diff=-diff;
		p2=pred+diff; d2=j-p2;
		d2=(d2<<1)^(d2>>31);

#if 1
		ix0b=index+bgbmid_ima_index_table[uni];
		ix1b=index+bgbmid_ima_index_table[uni1];
		ix2b=index+bgbmid_ima_index_table[uni2];
		ix0b=(ix0b<0)?0:((ix0b>88)?88:ix0b);
		ix1b=(ix1b<0)?0:((ix1b>88)?88:ix1b);
		ix2b=(ix2b<0)?0:((ix2b>88)?88:ix2b);
		stp0b=bgbmid_ima_step_table[ix0b];
		stp1b=bgbmid_ima_step_table[ix1b];
		stp2b=bgbmid_ima_step_table[ix2b];

		k=s1-p0;
		if(k>=0)	{ sni=(k*4)/stp0b; uni0b=(sni>7)?7:sni; }
		else		{ sni=(-k*4)/stp0b; uni0b=(sni>7)?15:(8|sni); }
		k=s1-p1;
		if(k>=0)	{ sni=(k*4)/stp1b; uni1b=(sni>7)?7:sni; }
		else		{ sni=(-k*4)/stp1b; uni1b=(sni>7)?15:(8|sni); }
		k=s1-p2;
		if(k>=0)	{ sni=(k*4)/stp2b; uni2b=(sni>7)?7:sni; }
		else		{ sni=(-k*4)/stp2b; uni2b=(sni>7)?15:(8|sni); }

		diff=((2*(uni0b&7)+1)*stp0b)/8;
		if(uni0b&8)diff=-diff;
		p0b=p0+diff; d0b=s1-p0b;
		d0b=(d0b<<1)^(d0b>>31);

		diff=((2*(uni1b&7)+1)*stp1b)/8;
		if(uni1b&8)diff=-diff;
		p1b=p1+diff; d1b=s1-p1b;
		d1b=(d1b<<1)^(d1b>>31);

		diff=((2*(uni2b&7)+1)*stp2b)/8;
		if(uni2b&8)diff=-diff;
		p2b=p2+diff; d2b=s1-p2b;
		d2b=(d2b<<1)^(d2b>>31);
		
		d0+=d0b/2;
		d1+=d1b/2;
		d2+=d2b/2;
#endif
		
		if(d1<d0)uni=uni1;
		if(d2<d0)uni=uni2;
#endif
	
		index=index+bgbmid_ima_index_table[uni];
		index=(index<0)?0:((index>88)?88:index);
		diff=((2*(uni&7)+1)*step)/8;
		if(uni&8)diff=-diff;
		pred=pred+diff;
		step=bgbmid_ima_step_table[index];
		
		obuf[4+(i>>1)]|=uni<<((i&1)*4);
		
		d0=s0-pred;		d0^=d0>>31;
		err+=d0;
	}
	
	err/=len;
	bgbcc_wave_blockerr+=err;

	if(ridx)
		{ *ridx=index; }
}

void BGBCC_MsImaAdpcm_DecodeBlockStereo(
	byte *ibuf, s16 *obuf, int len)
{
	int lpred, lindex, lstep, ldiff, luni, lsni;
	int rpred, rindex, rstep, rdiff, runi, rsni;
	int i, j, k;
	
	lpred=(s16)(ibuf[0]+(ibuf[1]<<8));
	lindex=ibuf[2];

	rpred=(s16)(ibuf[4]+(ibuf[5]<<8));
	rindex=ibuf[6];
	
	lstep=bgbmid_ima_step_table[lindex&127];
	rstep=bgbmid_ima_step_table[rindex&127];

	for(i=0; i<len; i++)
	{
//		k=((i>>3)*4)+((i&7)>>1);
		k=((i>>3)*8)+((i&7)>>1);
		j=(ibuf[ 8+k]>>((i&1)*4))&15;
		luni=j;
		lsni=(j&8)?(-(j&7)):(j&7);

		j=(ibuf[12+k]>>((i&1)*4))&15;
		runi=j;
		rsni=(j&8)?(-(j&7)):(j&7);
	
		lindex=lindex+bgbmid_ima_index_table[luni];
		lindex=(lindex<0)?0:((lindex>88)?88:lindex);
//		ldiff=(lsni+0.5)*lstep/4;
		ldiff=((2*(luni&7)+1)*lstep)/8;
		if(luni&8)ldiff=-ldiff;
		lpred=lpred+ldiff;
		lstep=bgbmid_ima_step_table[lindex];

		rindex=rindex+bgbmid_ima_index_table[runi];
		rindex=(rindex<0)?0:((rindex>88)?88:rindex);
//		rdiff=(rsni+0.5)*rstep/4;
		rdiff=((2*(runi&7)+1)*rstep)/8;
		if(runi&8)rdiff=-rdiff;
		rpred=rpred+rdiff;
		rstep=bgbmid_ima_step_table[rindex];

//		lindex=(lindex<0)?0:((lindex>88)?88:lindex);
//		rindex=(rindex<0)?0:((rindex>88)?88:rindex);
		lpred=(lpred<(-32768))?(-32768):((lpred>32767)?32767:lpred);
		rpred=(rpred<(-32768))?(-32768):((rpred>32767)?32767:rpred);
		
		obuf[i*2+0]=lpred;
		obuf[i*2+1]=rpred;
	}
}

void BGBCC_MsImaAdpcm_EncodeBlockStereo(
	s16 *ibuf, byte *obuf, int len, int *rlidx, int *rridx)
{
	int lpred, lindex, lstep, ldiff, luni, lsni;
	int rpred, rindex, rstep, rdiff, runi, rsni;
	int lsamp, rsamp, d0, d1, d2;
	int uni1, uni2, pred0, pred1, pred2;
	int i, j, k;
	
	lpred=(ibuf[0]+ibuf[2])/2;
	lindex=*rlidx;
	rpred=(ibuf[1]+ibuf[3])/2;
	rindex=*rridx;

	memset(obuf, 0, 8+((len+7)/8)*8);
	
	obuf[0]=lpred&255;
	obuf[1]=(lpred>>8)&255;
	obuf[2]=lindex;
	obuf[3]=0;

	obuf[4]=rpred&255;
	obuf[5]=(rpred>>8)&255;
	obuf[6]=rindex;
	obuf[7]=0;
	
//	memset(obuf+4, 0, (len+1)/2);
	
	lstep=bgbmid_ima_step_table[lindex&127];
	rstep=bgbmid_ima_step_table[rindex&127];

	for(i=0; i<len; i++)
	{
#if 0
		ldiff=ibuf[i*2+0]-lpred;
		lsni=(ldiff*4)/lstep-0.5;
		lsni=(lsni<(-7))?(-7):((lsni>7)?7:lsni);
		luni=(lsni<0)?((-lsni)|8):lsni;

		rdiff=ibuf[i*2+1]-rpred;
		rsni=(rdiff*4)/rstep-0.5;
		rsni=(rsni<(-7))?(-7):((rsni>7)?7:rsni);
		runi=(rsni<0)?((-rsni)|8):rsni;
#endif

		lsamp=ibuf[i*2+0];
		rsamp=ibuf[i*2+1];

#if 1
		ldiff=ibuf[i*2+0]-lpred;
		if(ldiff>=0)
		{
			lsni=(ldiff*4)/lstep;
			luni=(lsni>7)?7:lsni;
		}else
		{
			lsni=(-ldiff*4)/lstep;
			luni=(lsni>7)?15:(8|lsni);
		}

		rdiff=ibuf[i*2+1]-rpred;
		if(rdiff>=0)
		{
			rsni=(rdiff*4)/rstep;
			runi=(rsni>7)?7:rsni;
		}else
		{
			rsni=(-rdiff*4)/rstep;
			runi=(rsni>7)?15:(8|rsni);
		}
#endif

#if 1
		uni1=luni-1;	uni2=luni+1;
		if((uni1^luni)&8)	uni1=luni;
		if((uni2^luni)&8)	uni2=luni;

		ldiff=(((2*(luni&7)+1)*lstep)/8)*((luni&8)?-1:1);
		pred0=lpred+ldiff;
		ldiff=(((2*(uni1&7)+1)*lstep)/8)*((uni1&8)?-1:1);
		pred1=lpred+ldiff;
		ldiff=(((2*(uni2&7)+1)*lstep)/8)*((uni2&8)?-1:1);
		pred2=lpred+ldiff;
		d0=lsamp-pred0;		d0^=d0>>31;
		d1=lsamp-pred1;		d1^=d1>>31;
		d2=lsamp-pred2;		d2^=d2>>31;
		if(d1<d0)	{ luni=uni1; d0=d1; }
		if(d2<d0)	{ luni=uni2; d0=d2; }


		uni1=runi-1;	uni2=runi+1;
		if((uni1^runi)&8)	uni1=runi;
		if((uni2^runi)&8)	uni2=runi;

		rdiff=(((2*(runi&7)+1)*rstep)/8)*((runi&8)?-1:1);
		pred0=rpred+rdiff;
		rdiff=(((2*(uni1&7)+1)*rstep)/8)*((uni1&8)?-1:1);
		pred1=rpred+rdiff;
		rdiff=(((2*(uni2&7)+1)*rstep)/8)*((uni2&8)?-1:1);
		pred2=rpred+rdiff;
		d0=rsamp-pred0;		d0^=d0>>31;
		d1=rsamp-pred1;		d1^=d1>>31;
		d2=rsamp-pred2;		d2^=d2>>31;
		if(d1<d0)	{ runi=uni1; d0=d1; }
		if(d2<d0)	{ runi=uni2; d0=d2; }
#endif

		lsni=(luni&8)?(-(luni&7)):(luni&7);
		lindex=lindex+bgbmid_ima_index_table[luni];
		lindex=(lindex<0)?0:((lindex>88)?88:lindex);
		ldiff=((2*(luni&7)+1)*lstep)/8;
		if(luni&8)ldiff=-ldiff;
		lpred=lpred+ldiff;
		lstep=bgbmid_ima_step_table[lindex];

		rsni=(runi&8)?(-(runi&7)):(runi&7);
		rindex=rindex+bgbmid_ima_index_table[runi];
		rindex=(rindex<0)?0:((rindex>88)?88:rindex);
		rdiff=((2*(runi&7)+1)*rstep)/8;
		if(runi&8)rdiff=-rdiff;
		rpred=rpred+rdiff;
		rstep=bgbmid_ima_step_table[rindex];
		
		lpred=(lpred<(-32768))?(-32768):((lpred>32767)?32767:lpred);
		rpred=(rpred<(-32768))?(-32768):((rpred>32767)?32767:rpred);
		
		k=((i>>3)*8)+((i&7)>>1);
		obuf[ 8+k]|=luni<<((i&1)*4);
		obuf[12+k]|=runi<<((i&1)*4);

	}
	
	*rlidx=lindex;
	*rridx=rindex;
}

void BGBCC_MsImaAdpcm_EncodeBlockMono2b(
	s16 *ibuf, byte *obuf, int len, int *ridx)
{
	memset(obuf, 0, 4+((len+3)/4));
	BGBCC_MsImaAdpcm_EncodeBlockChan2b(ibuf, obuf, 1, len, ridx);
}

void BGBCC_MsImaAdpcm_EncodeBlockStereo2b(
	s16 *ibuf, byte *obuf, int len, int *rlidx, int *rridx)
{
	memset(obuf, 0, 8+((len+1)/2));
	BGBCC_MsImaAdpcm_EncodeBlockChan2b(ibuf+0, obuf+0, 2, len, rlidx);
	BGBCC_MsImaAdpcm_EncodeBlockChan2b(ibuf+1, obuf+4, 2, len, rridx);
}

void BGBCC_MsImaAdpcm_EncodeBlockChan2b(
	s16 *ibuf, byte *obuf, int chan, int len, int *ridx)
{
	int s0, s1, s2, p0, p1, p2, d0, d1, d2, uni1, uni2;
	int ix0, ix1, ix2, stp0, stp1, stp2;
	int lsbt, lsbit, err, bd, bk, d;
	int pred, index, step, diff, uni, sni;
	int excl, noovf, chanx4;
	int i, j, k;
	
	pred=(ibuf[0*chan]+ibuf[1*chan])>>1;
	index=ridx?(*ridx):0;
	
	obuf[0]=pred&255;
	obuf[1]=(pred>>8)&255;
	obuf[2]=index;
	obuf[3]=0;

//	noovf=1;
	noovf=0;
	chanx4=chan*4;
	
//	memset(obuf+4, 0, (len+3)/4);
	
	step=bgbmid_ima_step_table[index&127];
	lsbt=0;
	err=0;

	for(i=0; i<len; i++)
	{
		s0=ibuf[(i+0)*chan];
		s1=ibuf[(i+1)*chan];
		s2=ibuf[(i+2)*chan];

		excl=0;
		bd=999999999; bk=-1;
//		for(k=0; k<16; k++)
		for(k=0; k<64; k++)
		{
			if((excl>>(k&15))&1)
				continue;
		
			uni=(k>>0)&3;
			ix0=index+((uni&1)*3-1);
			if(noovf && ((ix0<0) || (ix0>88)))
				continue;
			ix0=(ix0<0)?0:((ix0>88)?88:ix0);
			diff=(step>>1)+((uni&1)*step);
			if(uni&2)diff=-diff;
			p0=pred+diff;
			stp0=bgbmid_ima_step_table[ix0];
			if(noovf && ((p0<-32767) || (p0>32767)))
				continue;
			p0=(p0<(-32768))?(-32768):((p0>32767)?32767:p0);

			d0=p0-s0; d0^=d0>>31;
			if(d0>=bd)
			{
				excl|=0x1111<<(k&3);
				continue;
			}

			uni=(k>>2)&3;
			ix1=ix0+((uni&1)*3-1);
			if(noovf && ((ix1<0) || (ix1>88)))
				continue;
			ix1=(ix1<0)?0:((ix1>88)?88:ix1);
			diff=(stp0>>1)+((uni&1)*stp0);
			if(uni&2)diff=-diff;
			p1=p0+diff;
			stp1=bgbmid_ima_step_table[ix1];
			if(noovf && ((p1<-32767) || (p1>32767)))
				continue;
			p1=(p1<(-32768))?(-32768):((p1>32767)?32767:p1);

			d1=p1-s1; d1^=d1>>31;
			if((d0+d1)>=bd)
			{
				excl|=0x0001<<(k&15);
				continue;
			}

#if 1			
			uni=(k>>4)&3;
			ix2=ix1+((uni&1)*3-1);
			if(noovf && ((ix2<0) || (ix2>88)))
				continue;
			ix2=(ix2<0)?0:((ix2>88)?88:ix2);
			diff=(stp1>>1)+((uni&1)*stp1);
			if(uni&2)diff=-diff;
			p2=p1+diff;
			stp2=bgbmid_ima_step_table[ix2];
			if(noovf && ((p2<-32767) || (p2>32767)))
				continue;
			p2=(p2<(-32768))?(-32768):((p2>32767)?32767:p2);
#endif
			
			d2=p2-s2; d2^=d2>>31;

			d=d0+d1+d2;
			if(d<bd)
				{ bd=d; bk=k; }
		}

		uni=bk&3;

#if 0
		j=ibuf[i];
		k=j-pred;
		if(k>=0)
		{
			sni=k/step;
			uni=(sni>1)?1:0;
		}else
		{
			sni=-k/step;
			uni=(sni>1)?3:2;
		}
#endif
	
		index=index+((uni&1)*3-1);
		index=(index<0)?0:((index>88)?88:index);
		diff=(step>>1)+((uni&1)*step);
		if(uni&2)diff=-diff;
		pred=pred+diff;
		step=bgbmid_ima_step_table[index];

		pred=(pred<(-32768))?(-32768):((pred>32767)?32767:pred);
		
		k=((i>>4)*chanx4)+((i&15)>>2);
		obuf[chanx4+k]|=uni<<((i&3)*2);

//		obuf[4+(i>>2)]|=uni<<((i&3)*2);
		
		d0=s0-pred;		d0^=d0>>31;
		err+=d0;
	}

	err/=len;
	bgbcc_wave_blockerr+=err;

	if(ridx)
		{ *ridx=index; }
}

void BGBCC_MsImaAdpcm_DecodeBlockMono2b(
	byte *ibuf, s16 *obuf, int len)
{
	int pred, index, step, diff, uni, sni;
	int i, j;
	
	pred=(s16)(ibuf[0]+(ibuf[1]<<8));
	index=ibuf[2];
	
	step=bgbmid_ima_step_table[index&127];

	for(i=0; i<len; i++)
	{
		j=(ibuf[4+(i>>2)]>>((i&3)*2))&3;
		uni=j;
	
		index=index+((uni&1)*3-1);
		index=(index<0)?0:((index>88)?88:index);
//		diff=((2*(uni&7)+1)*step)/8;
		diff=(step>>1)+((uni&1)*step);
		if(uni&2)diff=-diff;
		pred=pred+diff;
		step=bgbmid_ima_step_table[index];

		pred=(pred<(-32768))?(-32768):((pred>32767)?32767:pred);
		obuf[i]=pred;
	}
}

void BGBCC_MsImaAdpcm_DecodeBlockStereo2b(
	byte *ibuf, s16 *obuf, int len)
{
	int lpred, lindex, lstep, ldiff, luni;
	int rpred, rindex, rstep, rdiff, runi;
	int i, j, k;
	
	lpred=(s16)(ibuf[0]+(ibuf[1]<<8));
	lindex=ibuf[2];

	rpred=(s16)(ibuf[4]+(ibuf[5]<<8));
	rindex=ibuf[6];
	
	lstep=bgbmid_ima_step_table[lindex&127];
	rstep=bgbmid_ima_step_table[rindex&127];

	for(i=0; i<len; i++)
	{
		k=((i>>4)*8)+((i&15)>>2);
		luni=(ibuf[ 8+k]>>((i&3)*2))&3;
		runi=(ibuf[12+k]>>((i&3)*2))&3;
	
		lindex=lindex+((luni&1)*3-1);
		lindex=(lindex<0)?0:((lindex>88)?88:lindex);
		ldiff=(lstep>>1)+((luni&1)*lstep);
		if(luni&2)ldiff=-ldiff;
		lpred=lpred+ldiff;
		lstep=bgbmid_ima_step_table[lindex];
		lpred=(lpred<(-32768))?(-32768):((lpred>32767)?32767:lpred);

		rindex=rindex+((runi&1)*3-1);
		rindex=(rindex<0)?0:((rindex>88)?88:rindex);
		rdiff=(rstep>>1)+((runi&1)*rstep);
		if(runi&2)rdiff=-rdiff;
		rpred=rpred+rdiff;
		rstep=bgbmid_ima_step_table[rindex];
		rpred=(rpred<(-32768))?(-32768):((rpred>32767)?32767:rpred);

		obuf[i*2+0]=lpred;
		obuf[i*2+1]=rpred;
	}
}

int BGBCC_WAVE_StoreWaveFmtBlockI(
	byte *obuf, byte *ibuf, int ibsz,
	int wft, int ch, int rt, int bits, int len,
	int blksz, int realbits);

void BGBCC_MsImaAdpcm_EncodeBlockMono2b_Opt(
	s16 *ibuf, byte *obuf, int len, int *rpred, int *ridx, int qfl);
void BGBCC_MsImaAdpcm_EncodeBlockMono4b_Opt(
	s16 *ibuf, byte *obuf, int len, int *rpred, int *ridx, int qfl);


int BGBCC_WAVE_StoreWaveImaAdpcm_Opt(
	byte *obuf, short *ibuf, int ch, int rt, int len, int qfl)
{
	s16 tblk[2048];
	byte *tbuf;
	byte *ct;
	s16 *cs, *cse;
	int nblk, blksz, blksc, idx, idxb, pred, bts;
	int i, j, k;
	
//	nblk=(len+255)/256;
//	blksz=(256/2)+4;

//	blksc=248;
//	nblk=(len+247)/248;
//	blksz=((248/2)+4)*ch;

	blksc=1016;
	nblk=(len+(blksc-1))/blksc;
	blksz=((blksc/2)+4)*ch;

	tbuf=bgbcc_tmalloc2("misc_fmt_wave", nblk*blksz);
	
	cs=ibuf;	cse=ibuf+(len*ch);
	ct=tbuf;
	
	bgbcc_wave_blockerr=0;
	idx=4; idxb=4;
	pred=ibuf[0];

	for(i=0; i<nblk; i++)
	{
		if((cs+(blksc*ch))>cse)
		{
			memset(tblk, 0, 2*blksc*sizeof(s16));
			memcpy(tblk, cs, (cse-cs)*sizeof(s16));
			if(ch==1)
			{
//				BGBCC_MsImaAdpcm_EncodeBlockMono(
//					tblk, ct, blksc, &idx);
				BGBCC_MsImaAdpcm_EncodeBlockMono4b_Opt(
					tblk, ct, blksc, &pred, &idx, qfl);
			}
			if(ch==2)
				BGBCC_MsImaAdpcm_EncodeBlockStereo(
					tblk, ct, blksc, &idx, &idxb);
		}else
		{
			if(ch==1)
			{
//				BGBCC_MsImaAdpcm_EncodeBlockMono(cs, ct, blksc, &idx);
				BGBCC_MsImaAdpcm_EncodeBlockMono4b_Opt(
					cs, ct, blksc, &pred, &idx, qfl);
			}
			if(ch==2)
				BGBCC_MsImaAdpcm_EncodeBlockStereo(cs, ct, blksc, &idx, &idxb);
		}
		cs+=blksc*ch;
		ct+=blksz;
	}
	
	k=bgbcc_wave_blockerr/nblk;
	printf("BGBCC_WAVE_StoreWaveImaAdpcm: Ch=%d Len=%d AvgErr=%d\n", ch, len, k);
	
	k=BGBCC_WAVE_StoreWaveFmtBlockI(
		obuf, tbuf, nblk*blksz,
		BGBCC_WAVE_FMT_IMAADPCM, ch, rt, 4, len, blksz, (ch==2)?8:4);
	free(tbuf);
	return(k);
}

int BGBCC_WAVE_StoreWaveImaAdpcm(
	byte *obuf, short *ibuf, int ch, int rt, int len)
{
	return(BGBCC_WAVE_StoreWaveImaAdpcm_Opt(obuf, ibuf, ch, rt, len, 100));
}

int BGBCC_WAVE_DecodeStreamImaAdpcm(
	s16 *dst, byte *src, int ch, int len, int blksz)
{
	byte *cs;
	s16 *ct;
	int nblk, blksc, idx;
	int i, j, k;
	
	blksc=(blksz-4)*2;
	if(ch==2)
		blksc=(blksz-8)*1;
	nblk=(len+(blksc-1))/blksc;
	
	cs=src; ct=dst;
	for(i=0; i<nblk; i++)
	{
		if(ch==1)
		{
			BGBCC_MsImaAdpcm_DecodeBlockMono(cs, ct, blksc);
			cs+=blksz;
			ct+=blksc;
		}
		if(ch==2)
		{
			BGBCC_MsImaAdpcm_DecodeBlockStereo(cs, ct, blksc);
			cs+=blksz;
			ct+=2*blksc;
		}
	}
	return(0);
}


int BGBCC_WAVE_StoreWaveImaAdpcm2b_Opt(
	byte *obuf, short *ibuf, int ch, int rt, int len, int qfl)
{
	s16 tblk[2048];
	byte *tbuf;
	byte *ct;
	s16 *cs, *cse;
	int nblk, blksz, blksc, idx, idxb, bts, pred;
	int i, j, k;
	
	blksc=1008;
	nblk=(len+(blksc-1))/blksc;
	blksz=((blksc/4)+4)*ch;

	tbuf=bgbcc_tmalloc2("misc_fmt_wave", nblk*blksz);
	
	cs=ibuf;	cse=ibuf+(len*ch);
	ct=tbuf;
	bgbcc_wave_blockerr=0;
	
	idx=4; idxb=4;
	pred=ibuf[0];

	BGBCC_MsImaAdpcm_EncodeBlockMono2b_Opt(
		NULL, tbuf, blksc, NULL, NULL, qfl);

	for(i=0; i<nblk; i++)
	{
		if((cs+(blksc*ch))>cse)
		{
			memset(tblk, 0, 2*blksc*sizeof(s16));
			memcpy(tblk, cs, (cse-cs)*sizeof(s16));
			if(ch==1)
			{
//				BGBCC_MsImaAdpcm_EncodeBlockMono2b(
//					tblk, ct, blksc, &idx);
				BGBCC_MsImaAdpcm_EncodeBlockMono2b_Opt(
					tblk, ct, blksc, &pred, &idx, qfl);
			}
			if(ch==2)
				BGBCC_MsImaAdpcm_EncodeBlockStereo2b(
					tblk, ct, blksc, &idx, &idxb);
		}else
		{
			if(ch==1)
			{
//				BGBCC_MsImaAdpcm_EncodeBlockMono2b(cs, ct, blksc, &idx);
				BGBCC_MsImaAdpcm_EncodeBlockMono2b_Opt(
					cs, ct, blksc, &pred, &idx, qfl);
			}
			if(ch==2)
				BGBCC_MsImaAdpcm_EncodeBlockStereo2b(
					cs, ct, blksc, &idx, &idxb);
		}
		cs+=blksc*ch;
		ct+=blksz;
	}
	
	k=bgbcc_wave_blockerr/nblk;
	printf("BGBCC_WAVE_StoreWaveImaAdpcm2b: Ch=%d Len=%d AvgErr=%d\n",
		ch, len, k);
	
	k=BGBCC_WAVE_StoreWaveFmtBlockI(
		obuf, tbuf, nblk*blksz,
		BGBCC_WAVE_FMT_IMAADPCM, ch, rt, 2, len, blksz, (ch==2)?4:2);
	free(tbuf);
	return(k);
}

int BGBCC_WAVE_StoreWaveImaAdpcm2b(
	byte *obuf, short *ibuf, int ch, int rt, int len)
{
	return(BGBCC_WAVE_StoreWaveImaAdpcm2b_Opt(obuf, ibuf, ch, rt, len, 100));
}

int BGBCC_WAVE_DecodeStreamImaAdpcm2b(
	s16 *dst, byte *src, int ch, int len, int blksz)
{
	byte *cs;
	s16 *ct;
	int nblk, blksc, idx;
	int i, j, k;
	
	blksc=(blksz-4)*4;
	if(ch==2)
		blksc=(blksz-8)*2;
	nblk=(len+(blksc-1))/blksc;
	
	cs=src; ct=dst;
	for(i=0; i<nblk; i++)
	{
		if(ch==1)
		{
			BGBCC_MsImaAdpcm_DecodeBlockMono2b(cs, ct, blksc);
			cs+=blksz;
			ct+=blksc;
		}
		if(ch==2)
		{
			BGBCC_MsImaAdpcm_DecodeBlockStereo2b(cs, ct, blksc);
			cs+=blksz;
			ct+=2*blksc;
		}
	}
	return(0);
}



#if 1
int ad2b_fixpats[256]={
0xAAAA, 0x0000, 0xFF92, 0x556A, 0x8888, 0x5560, 0xAAA8, 0x0003,
0x000B, 0x004B, 0x0002, 0x2222, 0xAAA1, 0xAAE1, 0xAAA9, 0x554A,
0xBFC0, 0xFF88, 0x55A0, 0x5528, 0xFF80, 0xF50A, 0xA14B, 0xE59A,
0x014B, 0xAFC0, 0xEA89, 0xAAA0, 0x4E72, 0xFFB0, 0x000A, 0x016A,
0x0103, 0x53E1, 0xAA13, 0xFF02, 0x5438, 0x3D30, 0x5722, 0xA04B,
0xABE1, 0x5575, 0xF01B, 0x02E1, 0xE038, 0x82E1, 0x010B, 0xA84B,
0xD52A, 0x284B, 0x546A, 0xFF82, 0xA86A, 0x85DA, 0x80B1, 0xC6D2,
0x006A, 0x001B, 0x16E1, 0xABA9, 0x12E1, 0x14A9, 0x6A03, 0x82E9,
0xC5BA, 0xB84B, 0xABA1, 0x204B, 0xA843, 0xFFE9, 0xE4D8, 0x4023,
0xA16A, 0xABC0, 0x7F82, 0x4FC0, 0xE528, 0xC0A9, 0xAA1B, 0x506A,
0x5462, 0x406A, 0xA8A9, 0xFC03, 0xFFE8, 0x2803, 0xFF0A, 0x8829,
0xA2E1, 0x5520, 0x5023, 0xF8C0, 0xFC0B, 0xAAB1, 0xE829, 0x8AA1,
0xF94B, 0x13A9, 0xFAC0, 0x056A, 0xD70A, 0xF13A, 0x5FA0, 0xF782,
0x9588, 0xFFDF, 0x52E5, 0x4C78, 0xBE03, 0x4283, 0x1532, 0x5F80,
0x00B8, 0x0083, 0x5A42, 0x2E78, 0xB803, 0x156A, 0x2843, 0xFF00,
0x02C0, 0x3F03, 0x7E06, 0x53A9, 0x6E92, 0x72B0, 0x200B, 0x686A,
0xA8E1, 0xB802, 0x814B, 0x8A08, 0xAA83, 0x8AE1, 0x83C0, 0x56A9,
0xAAC0, 0xE002, 0x86D8, 0x02A1, 0x4E92, 0xA103, 0x024B, 0xA01B,
0x550A, 0xC4C0, 0xEEC0, 0x084B, 0x000E, 0x6F98, 0x5322, 0xAEA1,
0x2B03, 0xAA4B, 0x0AB1, 0xEAC0, 0x00B1, 0x0AA1, 0x4CB1, 0x2BE1,
0x4F18, 0xA56A, 0x4A92, 0x220A, 0xBFE0, 0x55A8, 0x0BE1, 0x4432,
0x4BE1, 0xC39C, 0x40B1, 0xA81B, 0x0AE1, 0x2A4B, 0x0503, 0xFFA0,
0xFDA0, 0x404E, 0x03E1, 0xFF08, 0x86E9, 0xA00B, 0x280B, 0x8528,
0x6FA1, 0x84C9, 0xFD82, 0x0BC0, 0x2A0A, 0x2283, 0xE4B8, 0x00B9,
0xB02A, 0x2A1B, 0xE6D2, 0x001A, 0x00E1, 0xA220, 0x6939, 0x0029,
0x552A, 0x80B9, 0x5038, 0xAAB0, 0xAEE1, 0x2A03, 0x6E42, 0xEF80,
0x6D38, 0xFD28, 0x20A2, 0x0B90, 0x4522, 0xFA13, 0x82C9, 0xEF88,
0x281B, 0x5F82, 0xAAA2, 0x94A9, 0x50E2, 0x88A0, 0xFBA0, 0x7E82,
0x6F82, 0xFAB0, 0xAF92, 0xA0A0, 0x856A, 0x6C39, 0x14B9, 0x0008,
0x02B1, 0x8539, 0xFD2A, 0x2282, 0x82B1, 0x77A0, 0xFFFF, 0xEAA9,
0xB0AA, 0x116A, 0xA883, 0x0A22, 0x8BC0, 0xF85B, 0x2AE1, 0x0BA1,
0xFA83, 0xAAE4, 0x8BE1, 0x8BA1, 0xFDA8, 0xAAC9, 0xE3C0, 0x55A2,
};
#endif

byte *ad2b_pat_obase;
byte *ad2b_pat_ohash[4096*8];



int AD2B_SimDecodeN(s16 *obuf, int samp0, int idx0,
	u32 pat, int len,
	int *rsamp1, int *ridx1)
{
	int pred, index, step, diff, uni, sni;
	int i, j;
	
	pred=samp0;
	index=idx0;

	step=bgbmid_ima_step_table[index&127];

	for(i=0; i<len; i++)
	{
		j=(pat>>(i*2))&3;
		uni=j;
	
		index=index+((uni&1)*3-1);
		index=(index<0)?0:((index>88)?88:index);
//		diff=((2*(uni&7)+1)*step)/8;
		diff=(step>>1)+((uni&1)*step);
		if(uni&2)diff=-diff;
		pred=pred+diff;
		step=bgbmid_ima_step_table[index];

		pred=(pred<(-32768))?(-32768):((pred>32767)?32767:pred);
		obuf[i]=pred;
	}
	if(rsamp1)	*rsamp1=pred;
	if(ridx1)	*ridx1=index;

	return(0);
}

int AD2B_EncCalcMseN(s16 *i0buf, s16 *i1buf, int len)
{
	int i, j, k;
	
	k=0;
	for(i=0; i<len; i++)
	{
		j=i0buf[i]-i1buf[i];
		j=j^(j>>31);
		k+=j;
//		k+=j*j;
	}
	return(k);
}


int AD2B_EncCalcMseReN(s16 *i0buf, s16 *i1buf, int len)
{
	static const byte escale[16]={
		0x80,0x60,0x40,0x30, 0x20,0x18,0x14,0x10,
		0x10,0x10,0x0C,0x0A, 0x08,0x06,0x05,0x04};
	int e0, e1, s0, s1, sa;
	
	e0=AD2B_EncCalcMseN(i0buf, i1buf, len);
	s0=i0buf[0];		s1=i0buf[len-1];
	s0=s0^(s0>>31);		s1=s1^(s1>>31);
	sa=(s0+s1)>>1;
	e1=(e0*escale[sa>>12])>>4;
	return(e1);
}

int AD2B_EncSimMsePat(s16 *ibuf, int pred0, int idx0, u32 pat, int len)
{
	s16 tbuf[32];
	AD2B_SimDecodeN(tbuf, pred0, idx0, pat, len, NULL, NULL);
	return(AD2B_EncCalcMseReN(ibuf, tbuf, len)/len);
}

int AD2B_EncSimMsePatB(s16 *ibuf, int pred0, int idx0, u32 pat, int len,
	int *rpred1, int *ridx1)
{
	s16 tbuf[32];
	AD2B_SimDecodeN(tbuf, pred0, idx0, pat, len, rpred1, ridx1);
	return(AD2B_EncCalcMseReN(ibuf, tbuf, len)/len);
}

int AD2B_EncBest8b(s16 *ibuf, int *rpred, int *ridx)
{
	s16 tbuf[8];
	int pred0, idx0;
	int bi, be, ce;
	int i, j, k;
	
	pred0=*rpred;
	idx0=*ridx;
	
	bi=0; be=99999999;
	for(i=0; i<4096; i++)
	{
		AD2B_SimDecodeN(tbuf, pred0, idx0, i, 6, NULL, NULL);
		ce=AD2B_EncCalcMseN(ibuf, tbuf, 6);
		if(ce<be)
			{ bi=i; be=ce; }
	}
	
	k=bi&255;
	AD2B_SimDecodeN(tbuf, pred0, idx0, k, 4, rpred, ridx);
	return(k);
}

int AD2B_EncBestPat16b(s16 *ibuf, int *rpred, int *ridx)
{
	s16 tbuf[32];
	int pred0, idx0;
	int bi, be, ce;
	int i, j, k;
	
	pred0=*rpred;
	idx0=*ridx;
	
	bi=0; be=99999999;
	for(i=0; i<256; i++)
	{
		j=ad2b_fixpats[i];
		j|=j<<16;
		AD2B_SimDecodeN(tbuf, pred0, idx0, j, 10, NULL, NULL);
		ce=AD2B_EncCalcMseReN(ibuf, tbuf, 10);
		if(ce<be)
			{ bi=i; be=ce; }
	}
	
	k=ad2b_fixpats[bi];
	AD2B_SimDecodeN(tbuf, pred0, idx0, k, 8, rpred, ridx);
	return(k);
}



void BGBCC_MsImaAdpcm_EncodeBlockMono2b_Opt(
	s16 *ibuf, byte *obuf, int len, int *rpred, int *ridx, int qfl)
{
	byte *ct, *csa, *csb;
	int pred, idx, pred1, idx1, n;
	int pred_j0, idx_j0;
	int pred_j1, idx_j1;
	int pred_j2, idx_j2;
	int j0, j1, j2, e0, e1, e2, emax, hi, nl, bl;
	int i, j, k, l;
	
	if(!ibuf)
	{
		ad2b_pat_obase=obuf;
		for(i=0; i<4096; i++)
		{
			ad2b_pat_ohash[i*4+0]=NULL;
			ad2b_pat_ohash[i*4+1]=NULL;
			ad2b_pat_ohash[i*4+2]=NULL;
			ad2b_pat_ohash[i*4+3]=NULL;
		}
		return;
	}
	
	pred=0;
	idx=0;
	if(rpred)	pred=*rpred;
	if(ridx)	idx=*ridx;
	obuf[0]=pred;
	obuf[1]=pred>>8;
	obuf[2]=idx;
	obuf[3]=0;
	
#if 0
	n=len/4;
	for(i=0; i<n; i++)
	{
		j=AD2B_EncBest8b(ibuf+(i*4), &pred, &idx);
		obuf[4+i]=j;
	}
#endif

#if 1

//	emax=1024;
//	emax=4096;
//	emax=8192;
	
	emax=192*(100-(qfl&127));

	n=len/4;
	for(i=0; i<n; i++)
	{
		ct=obuf+4+i;
		hi=(ct[-1]*7)+(ct[-2]*29)+(ct[-3]*47)+(ct[-4]*61);
		hi=((hi*251)>>8)&4095;

		e2=99999999;
		csb=NULL;
		bl=0;

		for(k=0; k<4; k++)
		{
			csa=ad2b_pat_ohash[hi*4+k];
			if(!csa)
				continue;
			if(!(
				(csa[-1]==ct[-1]) && (csa[-2]==ct[-2]) &&
				(csa[-3]==ct[-3]) && (csa[-4]==ct[-4]) ))
					continue;

			j2=csa[0];
			e2=AD2B_EncSimMsePatB(ibuf+(i*4), pred, idx, j2, 4,
				&pred_j2, &idx_j2);
			if(e2>emax)
				continue;

			nl=csa-ad2b_pat_obase;
			for(l=0; l<nl; l++)
				if(csa[-l]!=ct[-l])
					break;
			if((l>bl) && (l>=4))
				{ csb=csa; bl=l; }
		}

		if(csb)
		{
			j2=csb[0];
			e2=AD2B_EncSimMsePatB(ibuf+(i*4), pred, idx, j2, 4,
				&pred_j2, &idx_j2);
		}
		
		ad2b_pat_ohash[hi*4+3]=ad2b_pat_ohash[hi*4+2];
		ad2b_pat_ohash[hi*4+2]=ad2b_pat_ohash[hi*4+1];
		ad2b_pat_ohash[hi*4+1]=ad2b_pat_ohash[hi*4+0];
		ad2b_pat_ohash[hi*4+0]=ct;
	
		e1=99999999;
		if((i+1)<n)
		{
			pred_j1=pred;	idx_j1=idx;
			j1=AD2B_EncBestPat16b(ibuf+(i*4), &pred_j1, &idx_j1);
			e1=AD2B_EncSimMsePat(ibuf+(i*4), pred, idx, j1, 8);
		}

		if(e2<emax)
		{
			obuf[4+i]=j2;
			pred=pred_j2;
			idx=idx_j2;
			continue;
		}

		if(e1<emax)
//		if(1)
		{
			obuf[4+(i+0)]=j1;
			obuf[4+(i+1)]=j1>>8;
			pred=pred_j1;
			idx=idx_j1;
			i++;
			continue;
		}

#if 1
		pred_j0=pred;	idx_j0=idx;
		j0=AD2B_EncBest8b(ibuf+(i*4), &pred_j0, &idx_j0);
		e0=AD2B_EncSimMsePat(ibuf+(i*4), pred, idx, j0, 4);
#endif

		obuf[4+i]=j0;
		pred=pred_j0;
		idx=idx_j0;
	}
#endif

	if(rpred)	*rpred=pred;
	if(ridx)	*ridx=idx;
}


#if 1
int ad4b_fixpats[256]={
0x0000, 0x8888, 0x8F4B, 0xBDA2, 0x1F80, 0x9BC3, 0x39C3, 0x1208,
0x6B80, 0x0D3B, 0x114C, 0x953B, 0x044B, 0xF83B, 0x07C3, 0x20C3,
0x793B, 0x914B, 0x31B3, 0xA719, 0x3219, 0xBD08, 0x9C08, 0x3708,
0x7891, 0x074B, 0xAD2A, 0x8FC3, 0x9FB2, 0x8A80, 0x1008, 0xAAC3,
0x2E3B, 0xBA91, 0x70B4, 0x91A2, 0x0080, 0x9FC2, 0x3991, 0x7718,
0x72A2, 0xB091, 0x842A, 0x252A, 0x3819, 0x8000, 0xC1B3, 0x20B4,
0x8719, 0x722A, 0x4808, 0x0091, 0x4080, 0x7108, 0x0880, 0x7408,
0x1508, 0xB04C, 0xF9A2, 0xC84B, 0x423B, 0x122A, 0x08B4, 0x9CA2,
0xFA08, 0x1749, 0xFFA0, 0xA04B, 0x133B, 0x3CB3, 0xCB2A, 0xFD80,
0x0888, 0x0A80, 0x3B3C, 0x7319, 0x8700, 0x2C08, 0x0AC3, 0xDAB3,
0x5700, 0x893B, 0xB93B, 0x1908, 0x83A2, 0xE23B, 0x21B3, 0xF0B3,
0x4891, 0xE308, 0xFF80, 0xC4A2, 0x1DB3, 0x3319, 0x00B3, 0x09A2,
0x3A08, 0x0C3B, 0x9108, 0xFB2A, 0xABB3, 0xF980, 0x3291, 0xB219,
0xAA4B, 0xF480, 0x9B19, 0x0191, 0xFC1A, 0x80B3, 0x4208, 0x3BC3,
0x803C, 0x3B80, 0x10B3, 0xAFC8, 0x0019, 0x1A2A, 0xF008, 0x7080,
0x0BA2, 0xB308, 0xA84B, 0x3A4B, 0x2A19, 0x3380, 0x9608, 0x7730,
0x22B4, 0xC108, 0x0119, 0x0D91, 0x083B, 0x8A2A, 0xBF91, 0xA319,
0x4A3C, 0x182A, 0x894B, 0x6480, 0x9819, 0x23C3, 0x1708, 0x4491,
0x0808, 0x97A2, 0x173A, 0xC808, 0x013B, 0x0D08, 0xAFA1, 0x2A91,
0x01C3, 0x3191, 0xB9B3, 0x1C08, 0x8008, 0x2008, 0xEC19, 0xBA3B,
0x90C3, 0x9880, 0x9908, 0x503B, 0x0991, 0x8880, 0x184B, 0xA980,
0x15A2, 0x23B3, 0xF219, 0x8208, 0x08C3, 0xA108, 0x07A2, 0x80C3,
0xB2B4, 0x0C91, 0x023B, 0xF2A2, 0x8919, 0x4A4B, 0x9008, 0x912A,
0x4519, 0x204B, 0xC919, 0x0208, 0x88B3, 0x814B, 0x87B3, 0x1C91,
0x1C4B, 0x8580, 0x014B, 0x9480, 0x9391, 0x05B3, 0xC080, 0x95B3,
0x8808, 0xB3C3, 0x1180, 0x11C3, 0xA480, 0x384B, 0xC891, 0xCA80,
0x2AC3, 0x0AB3, 0x20B3, 0x073A, 0x8780, 0x1091, 0x2080, 0x983B,
0x09C3, 0xA02A, 0xA3B4, 0x89C3, 0x94C3, 0x903B, 0xA880, 0xCA08,
0xA92A, 0x904B, 0xBB08, 0x1B19, 0xA8C3, 0xB3B3, 0x004B, 0xC819,
0x8119, 0xB0C3, 0x8AB3, 0xF091, 0xA408, 0x1480, 0x0491, 0x0C19,
0x084B, 0x8908, 0xD980, 0x172A, 0x89B3, 0x8491, 0xA82A, 0x813B,
0x88C3, 0xAAB3, 0xA4B3, 0xC880, 0x9980, 0x883B, 0x003B, 0xF1B3,
};
#endif

int AD4B_SimDecodeN(s16 *obuf, int samp0, int idx0,
	u32 pat, int len,
	int *rsamp1, int *ridx1)
{
	int pred, index, step, diff, uni, sni;
	int i, j;
	
	pred=samp0;
	index=idx0;

	step=bgbmid_ima_step_table[index&127];

	for(i=0; i<len; i++)
	{
		j=(pat>>(i*4))&15;
	
		uni=j;
		sni=(j&8)?(-(j&7)):(j&7);
	
		index=index+bgbmid_ima_index_table[uni];
		index=(index<0)?0:((index>88)?88:index);
		diff=((2*(uni&7)+1)*step)/8;
		if(uni&8)diff=-diff;
		pred=pred+diff;
		step=bgbmid_ima_step_table[index];

		pred=(pred<(-32768))?(-32768):((pred>32767)?32767:pred);
		obuf[i]=pred;
	}
	if(rsamp1)	*rsamp1=pred;
	if(ridx1)	*ridx1=index;

	return(0);
}

int AD4B_EncSimMsePatB(s16 *ibuf, int pred0, int idx0, u32 pat, int len,
	int *rpred1, int *ridx1)
{
	s16 tbuf[32];
	AD4B_SimDecodeN(tbuf, pred0, idx0, pat, len, rpred1, ridx1);
	return(AD2B_EncCalcMseReN(ibuf, tbuf, len)/len);
}

int AD4B_EncSimMsePat(s16 *ibuf, int pred0, int idx0, u32 pat, int len)
	{ return(AD4B_EncSimMsePatB(ibuf, pred0, idx0, pat, len, NULL, NULL)); }

int AD4B_EncBest8b(s16 *ibuf, int *rpred, int *ridx)
{
	s16 tbuf[8];
	int pred0, idx0;
	int bi, be, ce;
	int i, j, k;
	
	pred0=*rpred;
	idx0=*ridx;
	
	bi=0; be=99999999;
	for(i=0; i<4096; i++)
	{
		AD4B_SimDecodeN(tbuf, pred0, idx0, i, 3, NULL, NULL);
		ce=AD2B_EncCalcMseN(ibuf, tbuf, 3);
		if(ce<be)
			{ bi=i; be=ce; }
	}
	
	k=bi&255;
	AD4B_SimDecodeN(tbuf, pred0, idx0, k, 2, rpred, ridx);
	return(k);
}

int AD4B_EncBestPat16b(s16 *ibuf, int *rpred, int *ridx)
{
	s16 tbuf[32];
	int pred0, idx0;
	int bi, be, ce;
	int i, j, k;
	
	pred0=*rpred;
	idx0=*ridx;
	
	bi=0; be=99999999;
	for(i=0; i<256; i++)
	{
		j=ad4b_fixpats[i];
		j|=j<<16;
		AD4B_SimDecodeN(tbuf, pred0, idx0, j, 5, NULL, NULL);
		ce=AD2B_EncCalcMseReN(ibuf, tbuf, 5);
		if(ce<be)
			{ bi=i; be=ce; }
	}
	
	k=ad4b_fixpats[bi];
	AD4B_SimDecodeN(tbuf, pred0, idx0, k, 4, rpred, ridx);
	return(k);
}

void BGBCC_MsImaAdpcm_EncodeBlockMono4b_Opt(
	s16 *ibuf, byte *obuf, int len, int *rpred, int *ridx, int qfl)
{
	byte *ct, *csa, *csb, *lct[4], *lcsb[4];
	int pred, idx, pred1, idx1, n;
	int pred_j0, idx_j0;
	int pred_j1, idx_j1;
	int pred_j2, idx_j2;
	int j0, j1, j2, e0, e1, e2, e3, emax, hi, nl, bl, be;
	int i, j, k, l;
	
	if(!ibuf)
	{
		ad2b_pat_obase=obuf;
		for(i=0; i<4096; i++)
			for(j=0; j<8; j++)
				{ ad2b_pat_ohash[i*8+j]=NULL; }
		return;
	}
	
	pred=*rpred;
	idx=*ridx;
	obuf[0]=pred;
	obuf[1]=pred>>8;
	obuf[2]=idx;
	obuf[3]=0;

#if 1	
	emax=192*(100-(qfl&127));

	for(i=0; i<4; i++)
		{ lcsb[i]=NULL; lct[i]=NULL; }

	n=len/2;
	for(i=0; i<n; i++)
	{
		ct=obuf+4+i;
		hi=(ct[-1]*7)+(ct[-2]*29)+(ct[-3]*47)+(ct[-4]*61);
		hi=((hi*251)>>8)&4095;

		e2=99999999;
		be=99999999;
		csb=NULL;
		bl=0;

		for(k=0; k<12; k++)
		{
			if(k<8)
			{
				csa=ad2b_pat_ohash[hi*8+k];
			}else if(lcsb[k-8])
			{
				csa=lcsb[k-8]+(ct-lct[k-8]);
			}else
			{
				csa=NULL;
			}
			
			if(!csa)
				continue;
			if(!(
				(csa[-1]==ct[-1]) && (csa[-2]==ct[-2]) &&
				(csa[-3]==ct[-3]) && (csa[-4]==ct[-4]) ))
					continue;

			j2=csa[0];
			e2=AD4B_EncSimMsePatB(ibuf+(i*2), pred, idx, j2, 2,
				&pred_j2, &idx_j2);
			if(e2>emax)
				continue;

			nl=csa-ad2b_pat_obase;
			for(l=0; l<nl; l++)
				if(csa[-l]!=ct[-l])
					break;

//			e3=e2/sqrt(nl);
			e3=e2/nl;

//			if((l>bl) && (l>=4))
			if(e3<be)
				{ csb=csa; bl=l; be=e3; }
		}

		if(csb)
		{
			j2=csb[0];
			e2=AD4B_EncSimMsePatB(ibuf+(i*2), pred, idx, j2, 2,
				&pred_j2, &idx_j2);
			lcsb[3]=lcsb[2]; lct[3]=lct[2];
			lcsb[2]=lcsb[1]; lct[2]=lct[1];
			lcsb[1]=lcsb[0]; lct[1]=lct[0];
			lcsb[0]=csb; lct[0]=ct;
		}
		
		for(j=7; j>0; j--)
			ad2b_pat_ohash[hi*8+j]=ad2b_pat_ohash[hi*8+(j-1)];
		ad2b_pat_ohash[hi*8+0]=ct;
	
		e1=99999999;
		if((i+1)<n)
		{
			pred_j1=pred;	idx_j1=idx;
			j1=AD4B_EncBestPat16b(ibuf+(i*2), &pred_j1, &idx_j1);
			e1=AD4B_EncSimMsePat(ibuf+(i*2), pred, idx, j1, 8);
		}

		if(e2<emax)
		{
			obuf[4+i]=j2;
			pred=pred_j2;
			idx=idx_j2;
			continue;
		}

		if(e1<emax)
//		if(1)
		{
			obuf[4+(i+0)]=j1;
			obuf[4+(i+1)]=j1>>8;
			pred=pred_j1;
			idx=idx_j1;
			i++;
			continue;
		}

#if 1
		pred_j0=pred;	idx_j0=idx;
		j0=AD4B_EncBest8b(ibuf+(i*2), &pred_j0, &idx_j0);
		e0=AD4B_EncSimMsePat(ibuf+(i*2), pred, idx, j0, 2);
#endif

		obuf[4+i]=j0;
		pred=pred_j0;
		idx=idx_j0;
	}
#endif

	*rpred=pred;
	*ridx=idx;
}
