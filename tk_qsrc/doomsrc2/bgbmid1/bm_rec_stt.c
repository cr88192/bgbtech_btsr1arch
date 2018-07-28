#include <bgbmid.h>

float bgbmid_fastcos(float ang)
{
	static float costab[1024];
	static int init=0;
	float f, g, t, r, v0, v1;
	int i;

	if(!init)
	{
		for(i=0; i<1024; i++)
		{
			costab[i]=cos((2*M_PI/1024.0)*i);
		}
		init=1;
	}
	
	t=ang*(1024*0.15915494309189533576888376337251);
	i=(int)t; r=t-i;
	v0=costab[(i+0)&1023];
	v1=costab[(i+1)&1023];
	f=(1.0-r)*v0 + r*v1;
	return(f);
	
//	i=((int)(ang*40.743665431525205956834243423364))&255;
//	i=((int)(ang*(1024*0.15915494309189533576888376337251)))&1023;
//	return(costab[i]);
}

int BGBMID_TransformSamplesToMatchBlock(
	int *hist, short *buf, int len, int ovlen)
{
	static float tlblk[1024];
	float tiblk[1024];
	double f, g, sc;
	int i, j, k;

#if 1
#if 0
	g=0;
	for(j=0; j<len; j++)
	{
		f=fabs(buf[j]);
		if(f>g)g=f;
	}
	sc=4096/(g+1);
	if(g<256)sc=1;
#endif

	for(i=0; i<64; i++)
	{
		f=(M_PI/64.0)*(i+0.5);
		g=0;
		for(j=0; j<len; j++)
		{
//			g=g+(sc*buf[j]*cos((j+0.5)*f));
			g=g+(buf[j]*cos((j+0.5)*f));
		}
		
//		g=g/sqrt(2);
//		hist[i]=g;
		hist[i]=fabs(g);
//		hist[i]=log(fabs(g)+1)*10;
	}
//	hist[0]=0;
#endif

#if 0
	for(i=0; i<256; i++)
	{
		f=(M_PI/1024.0)*(4*i+0.5);
		g=0;
		for(j=0; j<len; j++)
		{
			g=g+(buf[j]*bgbmid_fastcos((j+0.5)*f));
//			g=g+(buf[j]*cos((j+0.5)*f));
		}
		
//		tiblk[i]=fabs(g);
		tiblk[i]=g;
//		hist[i]=g;
//		hist[i]=fabs(g);
	}

	for(i=0; i<256; i++)
	{
//		f=0.90*tlblk[i]+0.10*tiblk[i];
		f=0.50*tlblk[i]+0.50*tiblk[i];
		tiblk[i]=f;
		tlblk[i]=f;
	}

	for(i=0; i<64; i++)
	{
		f=0;
		for(j=0; j<4; j++)
		{
			f=f+tiblk[i*4+j];
		}
		f=f/sqrt(2);
//		hist[i]=f/4;
		hist[i]=fabs(f/4);
	}
	
	hist[0]=0;
#endif

#if 0
	for(i=0; i<1024; i++)
	{
		f=(M_PI/1024.0)*(i+0.5);
		g=0;
		for(j=0; j<len; j++)
		{
			g=g+(buf[j]*bgbmid_fastcos((j+0.5)*f));
//			g=g+(buf[j]*cos((j+0.5)*f));
		}
		
		tiblk[i]=g;
//		hist[i]=g;
//		hist[i]=fabs(g);
	}

	for(i=0; i<64; i++)
	{
		f=0;
		for(j=0; j<16; j++)
		{
			f=f+tiblk[i*16+j];
		}
		hist[i]=f/16;
	}
#endif

#if 0
	for(i=0; i<64; i++)
	{
		f=(M_PI/1024.0)*((i+0.5)*16);
		g=0;
		for(j=0; j<len; j++)
		{
			g=g+(buf[j]*cos((j+0.5)*f));
		}
		
		hist[i]=g;
//		hist[i]=fabs(g);
	}
#endif

	return(0);
}

int BGBMID_TransformMatchBlockToSamples(
	int *hist, short *buf, int len, int ovlen)
{
	double f, g;
	int i, j, k;

	for(i=0; i<len; i++)
	{
		g=0;
		for(j=0; j<64; j++)
		{
//			f=(M_PI/1024.0)*((j+0.5)*16);
			f=(M_PI/705.6)*(j+0.5);
//			f=(M_PI/352.8)*(j+0.5);
			g=g+(hist[j]*bgbmid_fastcos((i+0.5)*f));
		}
		
//		g=g*0.25;
//		g=g/1024;
		g=g/sqrt(2);
		if(g<-8192)g=-8192;
		if(g>8192)g=8192;
		buf[i]=g;
	}

	for(i=0; i<32; i++)
	{
		buf[i]=(buf[i]*i)/32;
		buf[len-(i+1)]=(buf[len-(i+1)]*i)/32;
	}

#if 0
	for(i=0; i<64; i++)
	{
		f=(M_PI/1024.0)*((i+0.5)*16);
		g=0;
		for(j=0; j<len; j++)
		{
			buf[j]=buf[j]+(hist[i]*cos((j+0.5)*f));
//			g=g+(buf[j]*cos((j+0.5)*f));
		}
		
//		hist[i]=fabs(g);
	}
#endif

	return(0);
}

BMID_API int BGBMID_RecordHandleSamples(
	BGBMID_Context *ctx, short *buf, int len, int ovlen)
{
	static short tmpa[16384];
	static short tmpb[4000];
	static float volsc=1.0;
	int tblk[64];
	char *s;
	int len2;
	double f, g, h;
	int i, j, k;

//	return(0);

	if(!ctx->bmsr_ctx)
	{
		ctx->bmsr_ctx=BGBMID_BMSR_AllocContext();
		ctx->bmsr_ctx->bmctx=ctx;
		ctx->bmsr_ctx->base=bgbmid_strdup("vsrdata");
		ctx->bmsr_ctx->blksamp=(len*8000)/44100;
		BGBMID_BMSR_LoadProfile(ctx->bmsr_ctx, "profile.txt");
	}

	s=BGBMID_BMSR_LookupMatchWord(ctx->bmsr_ctx);
	if(s && (*s!='_'))
	{
		printf("Match Word: %s\n", s);
	}

	if(!ctx->rec_fblk)
	{
		i=64;
		ctx->rec_fblk=bgbmid_malloc(i*64*sizeof(int));
		ctx->rec_n_fblk=i;
		ctx->rec_fblk_rov=0;
	}

	for(i=0; i<len; i++)
		{ tmpa[i]=(buf[i*2+0]+buf[i*2+1])/2; }

	f=0; g=0; h=0;
	for(i=0; i<len; i++)
	{
//		f=(f*0.90)+(tmpa[i]*0.10);
//		f=(f*0.95)+(tmpa[i]*0.05);
//		g=(g*0.95)+(f*0.05);
		f=(f*0.90)+(tmpa[i]*0.10);
		g=(g*0.90)+(f*0.10);
		if(fabs(g)>h)
			{ h=fabs(g); }
		tmpa[i]=g;
	}
	
	f=4096/(h+1);
	if(f<1.0)f=1.0;
	if(f>8.0)f=8.0;
	volsc=volsc*0.95+0.05*f;

	for(i=0; i<len; i++)
	{
		g=tmpa[i];
		g=g*volsc;
		tmpa[i]=g;
	}
	
	len2=(len*8000)/44100;
	BGBMID_ScaleSample(tmpb, len2, tmpa, len);
	BGBMID_TransformSamplesToMatchBlock(tblk, tmpb, len2, ovlen);

#if 0
	f=0;
	for(i=0; i<len; i++)
	{
		f=(f*0.9)+(buf[i]*0.1);
//		f=(f*0.99)+(buf[i]*0.01);
		buf[i]=f;
	}
#endif

//	BGBMID_TransformSamplesToMatchBlock(tblk, buf, len, ovlen);

#if 1
	j=ctx->rec_fblk_rov;
	for(i=0; i<64; i++)
	{
//		ctx->rec_fblk[j*64+i]=tblk[i];
//		ctx->rec_fblk[j*64+i]=abs(tblk[i]-ctx->rec_bias[i]);

		k=abs(tblk[i]-ctx->rec_bias[i]);
		k=(k/16)*16;
		ctx->rec_fblk[j*64+i]=k;


//		ctx->rec_bias[i]=((63*ctx->rec_bias[i])+tblk[i])>>6;
//		ctx->rec_bias[i]=((1023*ctx->rec_bias[i])+tblk[i])>>10;
//		ctx->rec_bias[i]=((1022*ctx->rec_bias[i])+tblk[i])>>10;

		k=tblk[i]-ctx->rec_bias[i];
		k=(k>=0)?sqrt(k):(-(sqrt(-k)));
		k=ctx->rec_bias[i]+k;
		ctx->rec_bias[i]=((1023*ctx->rec_bias[i])+k)>>10;
	}
#endif

#if 0
	j=ctx->rec_fblk_rov;
	for(i=0; i<64; i++)
	{
		k=ctx->rec_fblk[j*64+i];
//		k=k/256;
		k=log(k)/log(2);
		if(k>25)k=25;
		printf("%c", 'A'+k);
	}
	printf("\n");
#endif

	j=ctx->rec_fblk_rov;
	ctx->rec_fblk_rov=(j+1)&(ctx->rec_n_fblk-1);

	return(0);
}

BMID_API int BGBMID_GetRecordFreqBlock(
	BGBMID_Context *ctx, int *hist, int szHist)
{
	int i, j, k, n;
	if(!ctx->rec_fblk)
		return(-1);

	j=(ctx->rec_fblk_rov-1)&(ctx->rec_n_fblk-1);

	n=szHist;
	if(n>64)n=64;
	for(i=0; i<n; i++)
	{
		hist[i]=ctx->rec_fblk[j*64+i];
	}
	
	return(0);
}

BMID_API int BGBMID_GetPriorRecordFreqBlock(
	BGBMID_Context *ctx, int idx, int *hist, int szHist)
{
	int i, j, k, n;
	if(!ctx->rec_fblk)
		return(-1);

	j=(ctx->rec_fblk_rov-(idx+1))&(ctx->rec_n_fblk-1);

	n=szHist;
	if(n>64)n=64;
	for(i=0; i<n; i++)
	{
		hist[i]=ctx->rec_fblk[j*64+i];
	}
	
	return(0);
}
