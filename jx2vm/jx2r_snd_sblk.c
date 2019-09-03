// #include "sblk_multi.c"

#include "jx2r_snd_smus.c"
#include "jx2r_snd_aupcm.c"

void SoundDev_Submit();
int SoundDev_WriteStereoSamples(short *buffer, int cnt);

int Sblkau_SampToPred9(int val)
{
	int v, ex, sg;
	
	if(val>=0)
		{ v=val; sg=0; }
	else
		{ v=-val; sg=1; }
	
	ex=0; v=v>>2;
	if(v>=64)
	{
		while(v>=64)
			{ ex++; v=v>>1; }
		if(ex>7)
			{ ex=7; v=63; }
	}else
	{
		v=(v>>1);
	}
	return((sg<<8)|(ex<<5)|(v&31));
}

int Sblkau_Pred9ToSamp(int val)
{
	int sg, ex, fr, v;
	
	sg=(val>>8)&1;
	ex=(val>>5)&7;
	if(ex)
		fr=32|(val&31);
	else
		fr=(val&31);
	v=fr<<(ex+2);
	if(sg)v=~v;
	return(v);
}

static int sblkau_rangetab[64]=
{
       0,     1,     2,     3,
       4,     5,     6,     7,
       8,    10,    12,    14,
      16,    20,    24,    28,
      32,    40,    48,    56,
      64,    80,    96,   112,
     128,   160,   192,   224,
     256,   320,   384,   448,
     512,   640,   768,   960,
    1024,  1280,  1536,  1920,
    2048,  2560,  3072,  3840,
    4096,  5120,  6144,  7680,
    8192, 10240, 12288, 15360,
   16384, 20480, 24576, 30720,
   32768, 40960, 49152, 61440,
   65536, 81920, 98304,     0
};

int sblkau_stat[16];

int Sblkau_StepForSamp(int val)
{
	int st, ra, v;
	
	v=val;
	if(v<0)v=-v;
	
	st=1;
	ra=sblkau_rangetab[st];
//	ra|=ra>>4;
//	ra|=ra>>8;
//	ra--;
//	ra*=0.75;

	while(v>(ra/2))
	{
		st++;
		ra=sblkau_rangetab[st];
	}
	
	return(st);
}

int Sblkau_DeltaForSamp(int ss, int dsv)
{
	int v, ra;

	ra=sblkau_rangetab[ss];

	if(dsv&4)
	{
		v=-(ra*((dsv&3)*2+1))/7;
//		v=-((ra*((dsv&3)+1))/4);
	}
	else
	{
		v=(ra*((dsv&3)*2+1))/7;
//		v=((ra*((dsv&3)+1))/4);
	}
	return(v);
}

int Sblkau_SampForDelta(int ss, int pr, int val)
{
	int ra, d, dsv, dsg;

	ra=sblkau_rangetab[ss];
//	ra|=ra>>4;
//	ra|=ra>>8;
//	ra--;
//	ra*=0.75;

	if(!ra)ra=1;
//	d=samp_s[i]-pr;
	d=val-pr;
	dsg=0;
	if(d<0)
		{ dsg=4; d=-d; }
//	dsv=(4*d+(ra/2))/ra;
//	dsv=(4*d-(ra/2))/ra;
	dsv=(4*d)/ra;
//	dsv=(4*d+(ra/4))/ra;
//	dsv=(4*d-(ra/4))/ra;
	if(dsv>3)dsv=3;
	dsv|=dsg;
	return(dsv);
}

int Sblkau_AdjustStepForSamp(int ss, int dsv)
{
	if((dsv&3)==0)ss--;
//	if((dsv&3)==3)ss++;
//	if((dsv&3)==2)ss++;
	if((dsv&3)==3)ss+=2;
	if(ss<1)ss=1;
	if(ss>62)ss=62;
//	if(ss<0)ss=0;
	return(ss);
}

int Sblkau_EncodeBlock(short *ibuf, u32 *obuf, u32 *rpred)
{
	int samp_c[128];
	int samp_s0[128];
	int samp_s[32];
	int samp_c4[32];
	u64 bit0, bit1, bit2, bit3, bit4, bit5;
	u32 bis0, bis1, bis2;
	u32 pred;
	u16 spc, sps;
	int cpr, spr;
	int ssc, sss;
	int pr, ss, spmono;
	int d, ra, dsg, dsv;
	int i, j, k;
	
	pred=*rpred;

	sss=0;	ssc=0;
	spr=0;	cpr=0;

	for(i=0; i<128; i++)
	{
		j=ibuf[i*2+0];
		k=ibuf[i*2+1];
		samp_c[i]=(j+k)/2;
		samp_s0[i]=(j-k)/2;
	}

	bit0=0;
	for(i=0; i<32; i++)
	{
		j=(	samp_s0[i*4+0] +
			samp_s0[i*4+1] +
			samp_s0[i*4+2] +
			samp_s0[i*4+3] )/4;
		samp_s[i]=j;
		bit0+=j*j;
	}

	for(i=0; i<32; i++)
	{
		j=(	samp_c[i*4+0] +
			samp_c[i*4+1] +
			samp_c[i*4+2] +
			samp_c[i*4+3] )/4;
		samp_c4[i]=j;
	}
	
	spmono=0;
	if(pred&0x00010000)
		spmono=1;
	if(bit0<(1<<16))
		spmono=1;
	if(bit0>(1<<20))
		spmono=0;

//	spmono=1;
//	spmono=0;

//	if(bit0<65536)
//	if(1)
//	if(0)
	if(spmono)
	{
		if(!(pred&0x00010000))
			pred=0;
		
		if(!pred)
		{
			spr=samp_s[0];
			cpr=((ibuf[0]+ibuf[1])/2)-spr;
//			ssc=8;
//			sss=8;

			ssc=Sblkau_StepForSamp(cpr);
			sss=Sblkau_StepForSamp(samp_s[1]-samp_s[0]);
			
			spc=(Sblkau_SampToPred9(cpr)<<7)|(ssc<<1);
			sps=(Sblkau_SampToPred9(spr)<<7)|(sss<<1);
			pred=(sps<<16)|spc;
		}
	
		for(i=0; i<32; i++)
		{
			j=(	samp_c[i*4+0] +
				samp_c[i*4+1] +
				samp_c[i*4+2] +
				samp_c[i*4+3] )/4;
			samp_s[i]=j;
		}
		
		spmono=1;
		
		sblkau_stat[1]++;
	}else
	{
		if(pred&0x00010000)
			pred=0;

		if(!pred)
		{
			cpr=(ibuf[0]+ibuf[1])/2;
			spr=(ibuf[0]-ibuf[1])/2;
//			ssc=8;
//			sss=8;

			ssc=Sblkau_StepForSamp(samp_c[1]-samp_c[0]);
			sss=Sblkau_StepForSamp(samp_s[1]-samp_s[0]);
			
			spc=(Sblkau_SampToPred9(cpr)<<7)|(ssc<<1);
			sps=(Sblkau_SampToPred9(spr)<<7)|(sss<<1);
			pred=(sps<<16)|spc;
		}
	
#if 0
		for(i=0; i<128; i++)
		{
			k=samp_s[i/4];
			j=(ibuf[i*2+0]+ibuf[i*2+1])/2;
			samp_c[i]+=(j-k)/2;
	//		samp_c[i]+=j-k;
		}
#endif

		sblkau_stat[0]++;
	}


	bis0=0;	bis1=0; bis2=0;
	pr=spr; ss=sss;
	for(i=0; i<32; i++)
	{
#if 0
		ra=sblkau_rangetab[ss];
		if(!ra)ra=1;
		d=samp_s[i]-pr; dsg=0;
		if(d<0)
			{ dsg=4; d=-d; }
//		dsv=(4*d+(ra/2))/ra;
//		dsv=(4*d-(ra/2))/ra;
		dsv=(4*d)/ra;
		if(dsv>3)dsv=3;
		dsv|=dsg;
#endif

//		d=samp_s[i]-pr;
		dsv=Sblkau_SampForDelta(ss, pr, samp_s[i]);
//		ra=sblkau_rangetab[ss];

		pr=pr+Sblkau_DeltaForSamp(ss, dsv);
		ss=Sblkau_AdjustStepForSamp(ss, dsv);

		samp_s[i]=pr;

//		if((dsv&3)==0)ss--;
//		if((dsv&3)==3)ss++;
//		if(ss<1)ss=1;

		bis0=(bis1<<29)|(bis0>>3);
		bis1=(bis2<<29)|(bis1>>3);
		bis2=(dsv <<29)|(bis2>>3);
	}
	spr=pr; sss=ss;

	if(spmono)
	{
		for(i=0; i<128; i++)
		{
			k=samp_s[i/4];
			j=(ibuf[i*2+0]+ibuf[i*2+1])/2;
//			samp_c[i]=(j-k)/2;
			samp_c[i]=(j-k);
		}
	}else
	{
#if 0
		for(i=0; i<128; i++)
		{
			k=samp_s[i/4];
			j=(ibuf[i*2+0]+ibuf[i*2+1])/2;
			samp_c[i]+=(j-k)/2;
		}
#endif

#if 0
		for(i=0; i<128; i++)
		{
			k=samp_s[i/4];
			k=((samp_c[i]+k)+(samp_c[i]-k))/2;
			j=(ibuf[i*2+0]+ibuf[i*2+1])/2;
			samp_c[i]+=(j-k)/2;
		}
#endif

	}

	bit0=0;	bit1=0;
	bit2=0;	bit3=0;
	bit4=0;	bit5=0;
	
	pr=cpr; ss=ssc;
	for(i=0; i<128; i++)
	{
#if 0
		ra=sblkau_rangetab[ss];
		if(!ra)ra=1;
		d=samp_c[i]-pr; dsg=0;
		if(d<0)
			{ dsg=4; d=-d; }
//		dsv=(4*d+(ra/2))/ra;
//		dsv=(4*d-(ra/2))/ra;
		dsv=(4*d)/ra;
		if(dsv>3)dsv=3;
		dsv|=dsg;
#endif

//		d=samp_c[i]-pr;
		dsv=Sblkau_SampForDelta(ss, pr, samp_c[i]);
//		ra=sblkau_rangetab[ss];

		pr=pr+Sblkau_DeltaForSamp(ss, dsv);
		ss=Sblkau_AdjustStepForSamp(ss, dsv);

//		if((dsv&3)==0)ss--;
//		if((dsv&3)==3)ss++;
//		if(ss<1)ss=1;

//		printf("%d ", dsv);
	
		bit0=      (bit1<<61)|(bit0>>3);
		bit1=      (bit2<<61)|(bit1>>3);
		bit2=      (bit3<<61)|(bit2>>3);
		bit3=      (bit4<<61)|(bit3>>3);
		bit4=      (bit5<<61)|(bit4>>3);
		bit5=(((u64)dsv)<<61)|(bit5>>3);
	}
	cpr=pr; ssc=ss;

//	printf("\n");



	if(spmono)
		pred|=0x00010000;
	else
		pred&=~0x00010000;

	obuf[ 0]=pred;

	obuf[ 1]=bis0;
	obuf[ 2]=bis1;
	obuf[ 3]=bis2;

	obuf[ 4]=bit0    ;
	obuf[ 5]=bit0>>32;
	obuf[ 6]=bit1    ;
	obuf[ 7]=bit1>>32;
	obuf[ 8]=bit2    ;
	obuf[ 9]=bit2>>32;
	obuf[10]=bit3    ;
	obuf[11]=bit3>>32;
	obuf[12]=bit4    ;
	obuf[13]=bit4>>32;
	obuf[14]=bit5    ;
	obuf[15]=bit5>>32;

	spc=(Sblkau_SampToPred9(cpr)<<7)|(ssc<<1);
	sps=(Sblkau_SampToPred9(spr)<<7)|(sss<<1);
	pred=(sps<<16)|spc;

	if(spmono)
		pred|=0x00010000;
	else
		pred&=~0x00010000;

	*rpred=pred;

	return(0);
}

int Sblkau_DecodeBlock(short *buf, u32 *blk)
{
	int samp_c[128];
	int samp_s[32];
	u64 bits;
	u32 pred;
	u16 spc, sps;
	int cpr, spr;
	int ssc, sss;
	int pr, pr1, ss;
	int d, ra, dsg, dsv;
	int vl, vr, lvl, lvr, spmon;
	int i, j, k;

	pred=blk[0];

	spc=(u16)(pred    );
	sps=(u16)(pred>>16);
	cpr=Sblkau_Pred9ToSamp(spc>>7);
	spr=Sblkau_Pred9ToSamp(sps>>7);
	ssc=(spc>>1)&63;
	sss=(sps>>1)&63;

	spmon=sps&1;

	pr=cpr; ss=ssc;
	for(i=0; i<128; i++)
	{
		j=i*3;
		bits=	(((u64)blk[(j>>5)+5])<<32) |
				(((u64)blk[(j>>5)+4])    ) ;
		dsv=(bits>>(j&31))&7;
	
//		printf("%d ", dsv);
	
//		ra=sblkau_rangetab[ss];
//		if((dsv&3)==0)ss--;
//		if((dsv&3)==3)ss++;
//		if(ss<1)ss=1;

		d=Sblkau_DeltaForSamp(ss, dsv);
		pr1=pr+d;
		ss=Sblkau_AdjustStepForSamp(ss, dsv);
		samp_c[i]=pr1;
		
//		if((s16)pr1!=pr1)
//			__debugbreak();

//		if((((s32)(pr1<<15))>>15)!=pr1)
		if(((s16)pr1)!=pr1)
			break;


		pr=pr1;
	}
//	printf("\n");

	if(i<128)
	{
		for(i=0; i<128; i++)
		{
			buf[i*2+0]=cpr;
			buf[i*2+1]=cpr;
		}
		return(-1);
	}

	pr=spr; ss=sss;
	for(i=0; i<32; i++)
	{
		j=i*3;
		bits=	(((u64)blk[(j>>5)+2])<<32) |
				(((u64)blk[(j>>5)+1])    ) ;
		dsv=(bits>>(j&31))&7;

//		printf("%d ", dsv);
	
//		ra=sblkau_rangetab[ss];
//		if((dsv&3)==0)ss--;
//		if((dsv&3)==3)ss++;
//		if(ss<1)ss=1;

		pr=pr+Sblkau_DeltaForSamp(ss, dsv);
		ss=Sblkau_AdjustStepForSamp(ss, dsv);
		samp_s[i]=pr;

//		if((((s32)(pr1<<15))>>15)!=pr1)
		if(((s16)pr)!=pr)
			break;
	}
//	printf("\n");

	if(i<32)
	{
		for(i=0; i<32; i++)
		{
			samp_s[i]=0;
		}

#if 0
		for(i=0; i<128; i++)
		{
			buf[i*2+0]=cpr;
			buf[i*2+1]=cpr;
		}
		return(-1);
#endif
	}

	lvl=0; lvr=0;
	for(i=0; i<128; i++)
	{
		j=samp_c[i];
		k=samp_s[i/4];

//		vl=j;
//		vr=j;

//		vl=k;
//		vr=k;
		if(spmon)
		{
//			vl=k+(j*2);
//			vr=k+(j*2);
			vl=k+j;
			vr=k+j;
		}else
		{
			vl=j+k;
			vr=j-k;
		}
		
		if(i)
		{
//			vl=(3*vl+lvl)/4;
//			vr=(3*vr+lvr)/4;
//			vl=(vl+lvl)/2;
//			vr=(vr+lvr)/2;
		}
		lvl=vl;
		lvr=vr;
		
		if((vl<-32767))vl=-32767;
		if((vl> 32767))vl= 32767;
		if((vr<-32767))vr=-32767;
		if((vr> 32767))vr= 32767;
		buf[i*2+0]=vl;
		buf[i*2+1]=vr;
	}
	return(0);
}

int Sblkau_CalcMSE(short *buf1, short *buf2, int len)
{
	s64 e;
	s64 dl, dr;
	int i;
	
	e=0;
	for(i=0; i<len; i++)
	{
		dl=buf1[i*2+0]-buf2[i*2+0];
		dr=buf1[i*2+1]-buf2[i*2+1];
		e+=dl*dl;
		e+=dr*dr;
	}
	
	e=e/(2*len);
	e=sqrt(e);
	return(e);
}

#if 0
// int main(int argc, char *argv[])
{
	u32 tblk[16];
	u32 tpred;
	short *sibuf;
	short *sobuf;
	char *ifn, *ofn;
	int ilen, nblk;
	int i, j, k;
	
	ifn=argv[1];
	ofn=argv[2];

	sibuf=BGBMID_LoadWAV_16Stereo16(ifn, &ilen);
//	sibuf=BGBMID_LoadWAV_32Stereo16(ifn, &ilen);
	
//	nblk=(ilen+127)/128;
	nblk=ilen/128;
	sobuf=malloc((nblk*128)*2*sizeof(short));

#if 0
	for(i=0; i<ilen; i++)
	{
		j=Sblkau_SampToPred9(sibuf[i*2+0]);
		k=Sblkau_SampToPred9(sibuf[i*2+1]);
		j=Sblkau_Pred9ToSamp(j);
		k=Sblkau_Pred9ToSamp(k);
		
		sobuf[i*2+0]=j;
		sobuf[i*2+1]=k;
	}
#endif

//	tpred=0; i=0;
//	Sblkau_EncodeBlock(sibuf+i*128*2, tblk, &tpred);
//	Sblkau_DecodeBlock(sobuf+i*128*2, tblk);

#if 1
	tpred=0;
	for(i=0; i<nblk; i++)
	{
		Sblkau_EncodeBlock(sibuf+i*128*2, tblk, &tpred);
		Sblkau_DecodeBlock(sobuf+i*128*2, tblk);
	}
#endif
	
	printf("J/S %d\n", sblkau_stat[0]);
	printf("S/M %d\n", sblkau_stat[1]);
	
	k=Sblkau_CalcMSE(sibuf, sobuf, ilen);
	printf("MSE=%d\n", k);
	
	BGBMID_StoreWAV(ofn, (byte *)sobuf, 2, 16000, 16, ilen);
//	BGBMID_StoreWAV(ofn, (byte *)sobuf, 2, 32000, 16, ilen);
}
#endif


u32 jx2i_sndblk_blkbuf[1024];
byte jx2i_sndblk_rov;
int jx2i_sndblk_accms;

int BJX2_SndSblk_Update(BJX2_Context *ctx, int dt)
{
	static short tsblk[256];
	static short tsblk1b[512*2];
	static short tsblk1c[512*2];
	static short tsblk2[65536];
	static int llv, lrv;
	static s64 lus;
	short *ct, *cte;
	int i, j, k, n, pdt;
	int lv, rv;
	
	if(dt>500)dt=500;
	
	ct=tsblk2;
	cte=ct+64512;
	pdt=jx2i_sndblk_accms+2*dt;
	jx2i_sndblk_accms+=dt;

//	llv=0; lrv=0;
	n=jx2i_sndblk_rov;
	while((pdt>=8) && (ct<cte))
	{
		Sblkau_DecodeBlock(tsblk,
			jx2i_sndblk_blkbuf+(n*16));
		n=(n+1)&63;

#if 0
		for(i=0; i<128; i++)
		{
			tsblk[i*2+0]=((i>>3)&1)?4096:-4096;
			tsblk[i*2+1]=((i>>3)&1)?4096:-4096;
		}
#endif

#if 0
		SMus_GetStepSamples(tsblk1b, 128);
		for(i=0; i<128; i++)
		{
			tsblk[i*2+0]=smus_clamp16s(tsblk[i*2+0]+tsblk1b[i*2+0]);
			tsblk[i*2+1]=smus_clamp16s(tsblk[i*2+1]+tsblk1b[i*2+1]);
		}
#endif

#if 0
		if(ct==tsblk2)
		{
			llv=tsblk[0];
			lrv=tsblk[1];
		}
#endif

		for(i=0; i<353; i++)
		{
			ct[i*2+0]=0;		ct[i*2+1]=0;
		}

#if 1
		for(i=0; i<353; i++)
		{
			j=(i*0.3628);
			lv=tsblk[j*2+0];	rv=tsblk[j*2+1];
//			lv=(3*llv+lv)/4;	rv=(3*lrv+rv)/4;
			lv=(7*llv+lv)/8;	rv=(7*lrv+rv)/8;
			
//			while(abs(lv-llv)>256)		lv=(lv+llv)>>1;
//			while(abs(rv-lrv)>256)		rv=(rv+lrv)>>1;
			
			llv=lv;				lrv=rv;
			ct[i*2+0]=lv;		ct[i*2+1]=rv;
		
//			j=(i*0.3628);
//			ct[i*2+0]=tsblk[j*2+0];
//			ct[i*2+1]=tsblk[j*2+1];
		}
#endif

#if 0
		SMus_GetStepSamples(tsblk1b, 353);
		for(i=0; i<353; i++)
		{
			lv=ct[i*2+0]; lv+=tsblk1b[i*2+0];
			rv=ct[i*2+1]; rv+=tsblk1b[i*2+1];
			ct[i*2+0]=smus_clamp16s(lv);
			ct[i*2+1]=smus_clamp16s(rv);
		}
#endif

		ct+=353*2;
	}

	n=0;
	while(jx2i_sndblk_accms>=8)
	{
		n++;
		jx2i_sndblk_accms-=8;
		jx2i_sndblk_rov=(jx2i_sndblk_rov+1)&63;
	}

#if 0
	ct=tsblk2;
	for(j=0; j<n; j++)
	{
#if 1
		SMus_GetStepSamples(tsblk1b, 353, 32);
		for(i=0; i<353; i++)
		{
#if 0
			if(!i && !j)
			{
				llv=tsblk1b[i*2+0];
				lus=jx2i_smus_usec;
			}
			if(abs(llv-tsblk1b[i*2+0])>1024)
				__debugbreak();
			llv=tsblk1b[i*2+0];
			lus=jx2i_smus_usec;
#endif
		
			lv=ct[i*2+0]; lv+=tsblk1b[i*2+0];
			rv=ct[i*2+1]; rv+=tsblk1b[i*2+1];
			ct[i*2+0]=smus_clamp16s(lv);
			ct[i*2+1]=smus_clamp16s(rv);
		}
#endif

		ct+=353*2;
	}
#endif

#if 1
	ct=tsblk2;
	for(j=0; j<n; j++)
	{
#if 1
		SMus_GetStepSamples(tsblk1b, 256, 32);
//		SMus_GetStepSamples(tsblk1b, 353, 32);

		SndAuPcm_GetStepSamples(tsblk1c, 256, 32);

		for(i=0; i<353; i++)
		{		
//			k=(i*0.3628);
			k=(i*0.725623583);
//			k=i;

			lv=ct[i*2+0]; lv+=tsblk1b[k*2+0];
			rv=ct[i*2+1]; rv+=tsblk1b[k*2+1];

			lv+=tsblk1c[k*2+0];
			rv+=tsblk1c[k*2+1];

			ct[i*2+0]=smus_clamp16s(lv);
			ct[i*2+1]=smus_clamp16s(rv);
		}
#endif

		ct+=353*2;
	}
	SMus_SyncTimeUsec(ctx);
	SndAuPcm_SyncTimeUsec(ctx);
#endif

	if(n>0)
	{
//	i=(ct-tsblk2)/2;
		i=n*353;
		SoundDev_WriteStereoSamples(tsblk2, i);
	}

	SoundDev_Submit();

	return(0);
}


int BJX2_MemSndSblkCb_GetByte(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

int BJX2_MemSndSblkCb_GetWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

s32 BJX2_MemSndSblkCb_GetDWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	s64 rvq;
	int ra, rv, vn;

	ra=addr-sp->addr_base;
	
	rv=0;

	if((ra>>2)>=1024)
	{
		if((ra>=0x1800) && (ra<0x1A00))
		{
			rv=SMus_GetRegister(ctx, (ra-0x1800)>>2);
			return(rv);
		}
	}

//	if(jx2i_sndblk_blkbuf)
	if(1)
	{
		rv=jx2i_sndblk_blkbuf[(ra>>2)&1023];
	}

	return(rv);
}


int BJX2_MemSndSblkCb_SetByte(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int val)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

int BJX2_MemSndSblkCb_SetWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int val)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

int BJX2_MemSndSblkCb_SetDWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s32 val)
{
	int ra;
	ra=addr-sp->addr_base;

	if((ra>>2)>=1024)
	{
		if((ra>=0x1800) && (ra<0x1A00))
		{
			SMus_SetRegister(ctx, (ra-0x1800)>>2, val);
			return(0);
		}
	}

//	if(jx2i_sndblk_blkbuf)
	if(1)
	{
		jx2i_sndblk_blkbuf[(ra>>2)&1023]=val;
//		jx2i_gfxcon_dirty=1;
	}

	return(0);
}

int BJX2_MemDefineSndSblk(BJX2_Context *ctx,
	char *name, bjx2_addr base, bjx2_addr lim)
{
	BJX2_MemSpan *sp;
	
	sp=BJX2_MemAllocSpan(ctx);
	
	sp->name=name;
	sp->addr_base=base;
	sp->addr_lim=lim;
	sp->addr_sz=lim-base;
//	sp->data=malloc((lim-base)+8);
	
	sp->GetByte=BJX2_MemSndSblkCb_GetByte;
	sp->GetWord=BJX2_MemSndSblkCb_GetWord;
	sp->GetDWord=BJX2_MemSndSblkCb_GetDWord;
	
	sp->SetByte=BJX2_MemSndSblkCb_SetByte;
	sp->SetWord=BJX2_MemSndSblkCb_SetWord;
	sp->SetDWord=BJX2_MemSndSblkCb_SetDWord;
	
	BJX2_MemAddSpan(ctx, sp);
	return(0);
}

