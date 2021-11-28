#include "bgbmid.h"

static char *bgbmid_patches[128]=
{
/* 0 */ "instruments/acpiano",
/* 1 */ "instruments/britepno",
/* 2 */ "instruments/synpiano",
/* 3 */ "instruments/honky",
/* 4 */ "instruments/epiano1",
/* 5 */ "instruments/epiano2",
/* 6 */ "instruments/hrpschrd",
/* 7 */ "instruments/clavinet",
/* 8 */ "instruments/celeste",
/* 9 */ "instruments/glocken",
/* 10 */ "instruments/musicbox",
/* 11 */ "instruments/vibes",
/* 12 */ "instruments/marimba",
/* 13 */ "instruments/xylophon",
/* 14 */ "instruments/tubebell",
/* 15 */ "instruments/santur",
/* 16 */ "instruments/homeorg",
/* 17 */ "instruments/percorg",
/* 18 */ "instruments/rockorg",
/* 19 */ "instruments/church",
/* 20 */ "instruments/reedorg",
/* 21 */ "instruments/accordn",
/* 22 */ "instruments/harmonca",
/* 23 */ "instruments/concrtna",
/* 24 */ "instruments/nyguitar",
/* 25 */ "instruments/acguitar",
/* 26 */ "instruments/jazzgtr",
/* 27 */ "instruments/cleangtr",
/* 28 */ "instruments/mutegtr",
/* 29 */ "instruments/odguitar",
/* 30 */ "instruments/distgtr",
/* 31 */ "instruments/gtrharm",
/* 32 */ "instruments/acbass",
/* 33 */ "instruments/fngrbass",
/* 34 */ "instruments/pickbass",
/* 35 */ "instruments/fretless",
/* 36 */ "instruments/slapbas1",
/* 37 */ "instruments/slapbas2",
/* 38 */ "instruments/synbass1",
/* 39 */ "instruments/synbass2",
/* 40 */ "instruments/violin",
/* 41 */ "instruments/viola",
/* 42 */ "instruments/cello",
/* 43 */ "instruments/contraba",
/* 44 */ "instruments/tremstr",
/* 45 */ "instruments/pizzcato",
/* 46 */ "instruments/harp",
/* 47 */ "instruments/timpani",
/* 48 */ "instruments/marcato",
/* 49 */ "instruments/slowstr",
/* 50 */ "instruments/synstr1",
/* 51 */ "instruments/synstr2",
/* 52 */ "instruments/choir",
/* 53 */ "instruments/doo",
/* 54 */ "instruments/voices",
/* 55 */ "instruments/orchhit",
/* 56 */ "instruments/trumpet",
/* 57 */ "instruments/trombone",
/* 58 */ "instruments/tuba",
/* 59 */ "instruments/mutetrum",
/* 60 */ "instruments/frenchrn",
/* 61 */ "instruments/hitbrass",
/* 62 */ "instruments/synbras1",
/* 63 */ "instruments/synbras2",
/* 64 */ "instruments/sprnosax",
/* 65 */ "instruments/altosax",
/* 66 */ "instruments/tenorsax",
/* 67 */ "instruments/barisax",
/* 68 */ "instruments/oboe",
/* 69 */ "instruments/englhorn",
/* 70 */ "instruments/bassoon",
/* 71 */ "instruments/clarinet",
/* 72 */ "instruments/piccolo",
/* 73 */ "instruments/flute",
/* 74 */ "instruments/recorder",
/* 75 */ "instruments/woodflut",
/* 76 */ "instruments/bottle",
/* 77 */ "instruments/shakazul",
/* 78 */ "instruments/whistle",
/* 79 */ "instruments/ocarina",
/* 80 */ "instruments/sqrwave",
/* 81 */ "instruments/sawwave",
/* 82 */ "instruments/calliope",
/* 83 */ "instruments/chiflead",
/* 84 */ "instruments/charang",
/* 85 */ "instruments/voxlead",
/* 86 */ "instruments/lead5th",
/* 87 */ "instruments/basslead",
/* 88 */ "instruments/fantasia",
/* 89 */ "instruments/warmpad",
/* 90 */ "instruments/polysyn",
/* 91 */ "instruments/ghostie",
/* 92 */ "instruments/bowglass",
/* 93 */ "instruments/metalpad",
/* 94 */ "instruments/halopad",
/* 95 */ "instruments/sweeper",
/* 96 */ "instruments/aurora",
/* 97 */ "instruments/soundtrk",
/* 98 */ "instruments/crystal",
/* 99 */ "instruments/atmosphr",
/* 100 */ "instruments/freshair",
/* 101 */ "instruments/unicorn",
/* 102 */ "instruments/echovox",
/* 103 */ "instruments/startrak",
/* 104 */ "instruments/sitar",
/* 105 */ "instruments/banjo",
/* 106 */ "instruments/shamisen",
/* 107 */ "instruments/koto",
/* 108 */ "instruments/kalimba",
/* 109 */ "instruments/bagpipes",
/* 110 */ "instruments/fiddle",
/* 111 */ "instruments/shannai",
/* 112 */ "instruments/carillon",
/* 113 */ "instruments/agogo",
/* 114 */ "instruments/steeldrm",
/* 115 */ "instruments/woodblk",
/* 116 */ "instruments/taiko",
/* 117 */ "instruments/toms",
/* 118 */ "instruments/syntom",
/* 119 */ "instruments/revcym",
/* 120 */ "instruments/fx-fret",
/* 121 */ "instruments/fx-blow",
/* 122 */ "instruments/seashore",
/* 123 */ "instruments/jungle",
/* 124 */ "instruments/telephon",
/* 125 */ "instruments/helicptr",
/* 126 */ "instruments/applause",
/* 127 */ "instruments/pistol"
};

static char *bgbmid_drumpatches[128]=
{
/* 0 */ NULL,
/* 1 */ NULL,
/* 2 */ NULL,
/* 3 */ NULL,
/* 4 */ NULL,
/* 5 */ NULL,
/* 6 */ NULL,
/* 7 */ NULL,
/* 8 */ NULL,
/* 9 */ NULL,

/* 10 */ NULL,
/* 11 */ NULL,
/* 12 */ NULL,
/* 13 */ NULL,
/* 14 */ NULL,
/* 15 */ NULL,
/* 16 */ NULL,
/* 17 */ NULL,
/* 18 */ NULL,
/* 19 */ NULL,
/* 20 */ NULL,
/* 21 */ NULL,
/* 22 */ NULL,
/* 23 */ NULL,
/* 24 */ NULL,
/* 25 */ NULL,
/* 26 */ NULL,

/* 27 */ "instrument/highq",
/* 28 */ "instruments/slap",
/* 29 */ "instruments/scratch1",
/* 30 */ "instruments/scratch2",
/* 31 */ "instruments/sticks",
/* 32 */ "instruments/sqrclick",
/* 33 */ "instruments/metclick",
/* 34 */ "instruments/metbell",
/* 35 */ "instruments/kick1",
/* 36 */ "instruments/kick2",
/* 37 */ "instruments/stickrim",
/* 38 */ "instruments/snare1",
/* 39 */ "instruments/claps",
/* 40 */ "instruments/snare2",
/* 41 */ "instruments/tomlo2",
/* 42 */ "instruments/hihatcl",
/* 43 */ "instruments/tomlo1",
/* 44 */ "instruments/hihatpd",
/* 45 */ "instruments/tommid2",
/* 46 */ "instruments/hihatop",
/* 47 */ "instruments/tommid1",
/* 48 */ "instruments/tomhi2",
/* 49 */ "instruments/cymcrsh1",
/* 50 */ "instruments/tomhi1",
/* 51 */ "instruments/cymride1",
/* 52 */ "instruments/cymchina",
/* 53 */ "instruments/cymbell",
/* 54 */ "instruments/tamborin",
/* 55 */ "instruments/cymsplsh",
/* 56 */ "instruments/cowbell",
/* 57 */ "instruments/cymcrsh2",
/* 58 */ "instruments/vibslap",
/* 59 */ "instruments/cymride2",
/* 60 */ "instruments/bongohi",
/* 61 */ "instruments/bongolo",
/* 62 */ "instruments/congahi1",
/* 63 */ "instruments/congahi2",
/* 64 */ "instruments/congalo",
/* 65 */ "instruments/timbaleh",
/* 66 */ "instruments/timbalel",
/* 67 */ "instruments/agogohi",
/* 68 */ "instruments/agogolo",
/* 69 */ "instruments/cabasa",
/* 70 */ "instruments/maracas",
/* 71 */ "instruments/whistle1",
/* 72 */ "instruments/whistle2",
/* 73 */ "instruments/guiro1",
/* 74 */ "instruments/guiro2",
/* 75 */ "instruments/clave",
/* 76 */ "instruments/woodblk1",
/* 77 */ "instruments/woodblk2",
/* 78 */ "instruments/cuica1",
/* 79 */ "instruments/cuica2",
/* 80 */ "instruments/triangl1",
/* 81 */ "instruments/triangl2",
/* 82 */ "instruments/shaker",
/* 83 */ "instruments/jingles",
/* 84 */ "instruments/belltree",
/* 85 */ "instruments/castinet",
/* 86 */ "instruments/surdo1",
/* 87 */ "instruments/surdo2",

/* 88 */ NULL,
/* 89 */ NULL,
/* 90 */ NULL,
/* 91 */ NULL,
/* 92 */ NULL,
/* 93 */ NULL,
/* 94 */ NULL,
/* 95 */ NULL,
/* 96 */ NULL,
/* 97 */ NULL,
/* 98 */ NULL,
/* 99 */ NULL,
/* 100 */ NULL,
/* 101 */ NULL,
/* 102 */ NULL,
/* 103 */ NULL,
/* 104 */ NULL,
/* 105 */ NULL,
/* 106 */ NULL,
/* 107 */ NULL,
/* 108 */ NULL,
/* 109 */ NULL,
/* 110 */ NULL,
/* 111 */ NULL,
/* 112 */ NULL,
/* 113 */ NULL,
/* 114 */ NULL,
/* 115 */ NULL,
/* 116 */ NULL,
/* 117 */ NULL,
/* 118 */ NULL,
/* 119 */ NULL,
/* 120 */ NULL,
/* 121 */ NULL,
/* 122 */ NULL,
/* 123 */ NULL,
/* 124 */ NULL,
/* 125 */ NULL,
/* 126 */ NULL,
/* 127 */ NULL
};

BGBMID_Patch *bgbmid_patchroot=NULL;

static char ***bgbmid_bankpatches;
int bgbmid_maxbank=0;

BMID_API BGBMID_Patch *BGBMID_LookupPatch(char *name)
{
	BGBMID_Patch *cur;

	cur=bgbmid_patchroot;
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

BMID_API void BGBMID_UnlinkPatch(BGBMID_Patch *pat)
{
	BGBMID_Patch *cur, *prv;

	cur=bgbmid_patchroot; prv=NULL;
	while(cur)
	{
//		if(!strcmp(cur->name, name))
//			return(cur);
		
		if(cur==pat)
		{
			if(prv)
				{ prv->next=cur->next; }
			else
				{ bgbmid_patchroot=cur->next; }
			break;
		}
		
		prv=cur;
		cur=cur->next;
	}
//	return(NULL);
}

BMID_API char *BGBMID_MidiPatchName(int idx)
{
	if((idx<0) || (idx>=128))
		return(NULL);
	return(bgbmid_patches[idx]);
}

BMID_API char *BGBMID_MidiDrumPatchName(int idx)
{
	if((idx<0) || (idx>=128))
		return(NULL);
	return(bgbmid_drumpatches[idx]);
}


BMID_API char **BGBMID_FetchPatchBank(int bank)
{
	if((bank<0) || (bank>=bgbmid_maxbank))
		return(NULL);
	return(bgbmid_bankpatches[bank]);
}

BMID_API char **BGBMID_GetPatchBank(int bank)
{
	int i, j, k;

	if(bank<0)return(NULL);

	if(!bgbmid_bankpatches)
	{
		bgbmid_bankpatches=bgbmid_malloc(128*sizeof(char **));
		memset(bgbmid_bankpatches, 0, 128*sizeof(char **));
		bgbmid_maxbank=128;
	}

	if(bank>=bgbmid_maxbank)
	{
		i=bgbmid_maxbank;
		while(i<=bank)i=i+(i>>1);

		bgbmid_bankpatches=bgbmid_realloc(
			bgbmid_bankpatches, i*sizeof(char **));
		j=bgbmid_maxbank; k=i-j;
		memset(bgbmid_bankpatches+j, 0, k*sizeof(char **));
		bgbmid_maxbank=i;
	}

	if(!bgbmid_bankpatches[bank])
	{
		bgbmid_bankpatches[bank]=bgbmid_malloc(128*sizeof(char *));
		memset(bgbmid_bankpatches[bank], 0, 128*sizeof(char *));
	}
	return(bgbmid_bankpatches[bank]);
}

BMID_API char *BGBMID_GetPatchName2(int bank, int idx)
{
	char **bnk;

	if((idx<0) || (idx>=128))
		return(NULL);

	if(bank==0)
	{
		bnk=BGBMID_FetchPatchBank(bank);
		if(bnk && bnk[idx])return(bnk[idx]);
		return(bgbmid_patches[idx]);
	}

	bnk=BGBMID_FetchPatchBank(bank);
	if(bnk)return(bnk[idx]);
	return(NULL);
}

BMID_API int BGBMID_SetPatchName2(int bank, int idx, char *name)
{
	char **bnk;

	if((idx<0) || (idx>=128))
		return(-1);

//	bnk=BGBMID_FetchPatchBank(bank);
	bnk=BGBMID_GetPatchBank(bank);
	if(bnk)
	{
		bnk[idx]=strdup(name);
		return(0);
	}
	return(-1);
}

BMID_API char *BGBMID_GetPatchName(int idx)
{
	if(idx<0)return(NULL);
	return(BGBMID_GetPatchName2(idx/128, idx%128));
}

BMID_API int BGBMID_SetPatchName(int idx, char *name)
{
	if(idx<0)return(-1);
	return(BGBMID_SetPatchName2(idx/128, idx%128, name));
}


BMID_API int BGBMID_AllocPatchName(
	int minidx, int maxidx,
	char *name)
{
	char *s;
	int i;

//	if(idx<0)return(-1);
//	return(BGBMID_SetPatchName2(idx/128, idx%128, name));

	for(i=minidx; i<maxidx; i++)
	{
		s=BGBMID_GetPatchName(i);
		if(!s)
		{
			BGBMID_SetPatchName(i, name);
			return(i);
		}
		if(!strcmp(s, name))
			return(i);
	}
	
	return(-1);
}

BMID_API int BGBMID_LookupIndexPatchName(
	int minidx, int maxidx,
	char *name)
{
	char *s;
	int i;

//	if(idx<0)return(-1);
//	return(BGBMID_SetPatchName2(idx/128, idx%128, name));

	for(i=minidx; i<maxidx; i++)
	{
		s=BGBMID_GetPatchName(i);
		if(!s)
		{
			continue;
//			BGBMID_SetPatchName(i, name);
//			return(i);
		}
		if(!strcmp(s, name))
			return(i);
	}
	
	return(-1);
}

BMID_API int BGBMID_BufferRebuildMip(
	short *bp, int chan, int len)
{
	short *bps, *bpt;
	int a, b, c;
	int i, j, k, n, ch;
	float f;
	
//	return(0);

#if 0
	ch=chan; n=2;
	for(i=0; i<n; i++)
		for(j=0; j<ch; j++)
	{
		f=(i+1.0)/(n+1.0);
		bp[i*ch+j]*=f;
		bp[(len-i-1)*ch+j]*=f;
	}
#endif

	n=len; ch=chan;
	bps=bp; bpt=bps+n*ch;
	for(i=0; i<8; i++)
	{
//		samp->pcm_mip[i]=bps;
		n=n>>1;
		for(j=0; j<n; j++)
			for(k=0; k<ch; k++)
		{
			a=bps[(2*j+0)*ch+k];
			b=bps[(2*j+1)*ch+k];
			c=(a+b)>>1;
			bpt[j*ch+k]=c;
		}
		bps=bpt;
		bpt=bps+n*ch;
	}

	return(0);
}

BMID_API int BGBMID_SampleRebuildMip(
	BGBMID_Sample *samp)
{
	short *bp, *bps, *bpt;
	int a, b, c;
	int i, j, k, n, ch;
	
//	return(0);
	
	bp=bgbmid_malloc(2*samp->len*samp->chan*sizeof(short));

	n=samp->len;
	ch=samp->chan;
	for(i=0; i<n*ch; i++)
		{ bp[i]=samp->pcm[i]; }

	bps=bp; bpt=bps+n*ch;
	for(i=0; i<8; i++)
	{
		samp->pcm_mip[i]=bps;
		n=n>>1;
		for(j=0; j<n; j++)
			for(k=0; k<ch; k++)
		{
			a=bps[(2*j+0)*ch+k];
			b=bps[(2*j+1)*ch+k];
			c=(a+b)>>1;
			bpt[j*ch+k]=c;
		}
		bps=bpt;
		bpt=bps+n*ch;
	}

	bgbmid_free(samp->pcm);
	samp->pcm=bp;

	return(0);
}



BMID_API int BGBMID_ReadS16LE(void *fd)
{
	short i;
	i=bgbmid_fgetc(fd);
	i|=bgbmid_fgetc(fd)<<8;
	return(i);
}

BMID_API int BGBMID_ReadS32LE(void *fd)
{
	int i;
	i=bgbmid_fgetc(fd);
	i|=bgbmid_fgetc(fd)<<8;
	i|=bgbmid_fgetc(fd)<<16;
	i|=bgbmid_fgetc(fd)<<24;
	return(i);
}

BMID_API void BGBMID_WriteS16LE(void *fd, int v)
{
	bgbmid_fputc(v&0xFF, fd);
	bgbmid_fputc((v>>8)&0xFF, fd);
}

BMID_API void BGBMID_WriteS32LE(void *fd, int v)
{
	bgbmid_fputc(v&0xFF, fd);
	bgbmid_fputc((v>>8)&0xFF, fd);
	bgbmid_fputc((v>>16)&0xFF, fd);
	bgbmid_fputc((v>>24)&0xFF, fd);
}

struct patch_tsamp_s {
int size;
int start, end;
int rate;
int low, high, root;
int fscale, fscalef;
int modes;
};

BMID_API BGBMID_Patch *BGBMID_LoadPatchGUS(char *name)
{
	byte tb[1024];
	struct patch_tsamp_s tsmp[16];
	BGBMID_Sample *csmp, *lsmp, *rsmp;
	BGBMID_Patch *tmp;
	byte *buf;
	void *fd;
	int nsmp;
	int i, j, k, l;

	if(!name)return(NULL);

	printf("Load Patch %s\n", name);

	sprintf((char *)tb, "%s.pat", name);
	fd=bgbmid_fopen((char *)tb, "rb");
	if(!fd)return(NULL);

	bgbmid_fread(tb, 1, 239, fd);

	if(memcmp(tb, "GF1PATCH110\0ID#000002", 22))
	{
		printf("\tBad Patch\n");
		bgbmid_fclose(fd);
		return(NULL);
	}

	nsmp=tb[198];
//	printf("\t%d BGBMID_Samples\n", nsmp);

	lsmp=NULL;

	for(i=0; i<nsmp; i++)
	{
		bgbmid_fread(tb, 1, 7, fd);
		tb[7]=0;

//		printf("\t%s\n", tb);

		bgbmid_fgetc(fd);		//fractions
		tsmp[i].size=BGBMID_ReadS32LE(fd);		//data len
		tsmp[i].start=BGBMID_ReadS32LE(fd);	//loop start
		tsmp[i].end=BGBMID_ReadS32LE(fd);		//loop end

		tsmp[i].rate=BGBMID_ReadS16LE(fd);		//sample rate
		tsmp[i].low=BGBMID_ReadS32LE(fd);		//low freq
		tsmp[i].high=BGBMID_ReadS32LE(fd);		//high freq
		tsmp[i].root=BGBMID_ReadS32LE(fd);		//root freq
		BGBMID_ReadS16LE(fd);	//tuning

		tsmp[i].rate&=0xFFFF;

		bgbmid_fgetc(fd);
		bgbmid_fread(tb, 1, 18, fd);

		tsmp[i].modes=bgbmid_fgetc(fd);		//modes
		tsmp[i].fscale=BGBMID_ReadS16LE(fd);	//frequency scale
		tsmp[i].fscalef=BGBMID_ReadS16LE(fd);	//frequency scale factor

//		printf("\t%d %d\n", tsmp[i].fscale, tsmp[i].fscalef);

		bgbmid_fread(tb, 1, 36, fd);

		buf=bgbmid_malloc(tsmp[i].size+16);
		j=bgbmid_fread(buf, 1, tsmp[i].size, fd);

		if(j!=tsmp[i].size)
		{
			printf("\tPremature EOF\n");
			break;
		}


		csmp=bgbmid_malloc(sizeof(BGBMID_Sample));
		memset(csmp, 0, sizeof(BGBMID_Sample));
		csmp->next=lsmp;
		lsmp=csmp;

		if(tsmp[i].modes&1)	//16 bit
		{
			csmp->pcm=bgbmid_malloc(tsmp[i].size);
			csmp->len=tsmp[i].size/2;
			csmp->start=tsmp[i].start/2;
			csmp->end=tsmp[i].end/2;

			for(j=0; j<csmp->len; j++)
				csmp->pcm[j]=(buf[j*2+1]<<8)|buf[j*2+0];
//				csmp->pcm[j]=(buf[j*2+0]<<8)|buf[j*2+1];
		}else
		{
			csmp->pcm=bgbmid_malloc(tsmp[i].size * sizeof(short));
			csmp->len=tsmp[i].size;
			csmp->start=tsmp[i].start;
			csmp->end=tsmp[i].end;

			for(j=0; j<csmp->len; j++)
				csmp->pcm[j]=buf[j]<<8;
		}
		bgbmid_free(buf);


		if(tsmp[i].modes&2)	//unsigned
		{
			for(j=0; j<csmp->len; j++)
				csmp->pcm[j]^=0x8000;
//				csmp->pcm[j]=csmp->pcm[j]-32768;
		}

		if(tsmp[i].modes&16)	//reverse
		{
			l=csmp->len/2;
			for(j=0; j<l; j++)
			{
				k=csmp->pcm[j];
				csmp->pcm[j]=csmp->pcm[csmp->len-j-1];
				csmp->pcm[csmp->len-j-1]=k;
			}

			csmp->start=csmp->len-csmp->start;
			csmp->end=csmp->len-csmp->end;
		}

		csmp->rate=tsmp[i].rate;
		csmp->flags=tsmp[i].modes;
		csmp->chan=1;

		if(tsmp[i].fscalef)
		{
			csmp->low=((float)tsmp[i].low)/tsmp[i].fscalef;
			csmp->high=((float)tsmp[i].high)/tsmp[i].fscalef;
			csmp->root=((float)tsmp[i].root)/tsmp[i].fscalef;
		}else
		{
//			tsmp[i].fscalef=1;

			csmp->low=0;
			csmp->high=999999.0;
			csmp->root=440.0;
		}

		BGBMID_SampleRebuildMip(csmp);

		//modes&4 == loop
		//modes&8 == bidir loop
		//modes&16 == reverse

		printf("\tBGBMID_Sample(%p) Len=%d Rt=%d (Strt=%d End=%d)\n",
			csmp,
			csmp->len, csmp->rate,
			csmp->start, csmp->end);

		printf("\tLow=%f High=%f Root=%f\n",
			csmp->low, csmp->high, csmp->root);
	}

	bgbmid_fclose(fd);

	//reverse samples list (putting it in correct order)
	csmp=lsmp; lsmp=NULL;
	while(csmp)
	{
		rsmp=csmp->next;
		csmp->next=lsmp;
		lsmp=csmp;
		csmp=rsmp;
	}

#if 1
	//addjust samples to "reasonable" rates
	csmp=lsmp;
	while(csmp)
	{
		i=8000;
		if(csmp->rate>8000)i=11025;
		if(csmp->rate>11025)i=22050;
		if(csmp->rate>22050)i=44100;
		if(csmp->rate>44100)i=48000;

		if(csmp->rate!=i)
		{
			j=csmp->len*(((float)i)/csmp->rate);
			buf=bgbmid_malloc(j*sizeof(short));

			BGBMID_ScaleSampleSinc(
				(short *)buf, j, csmp->pcm, csmp->len);
//			BGBMID_ScaleSampleSpline(
//				(short *)buf, j, csmp->pcm, csmp->len);
			bgbmid_free(csmp->pcm);

			csmp->start=csmp->start*(((float)i)/csmp->rate);
			csmp->end=csmp->end*(((float)i)/csmp->rate);

			csmp->pcm=(short *)buf;
			csmp->len=j;
			csmp->rate=i;
			csmp->chan=1;
		}

		csmp=csmp->next;
	}
#endif

//	BGBMID_StorePatchWav(name, lsmp);
//	BGBMID_TestPatchFilt(name, lsmp);

	tmp=bgbmid_malloc(sizeof(BGBMID_Patch));
	memset(tmp, 0, sizeof(BGBMID_Patch));
	tmp->samp=lsmp;

	tmp->name=strdup(name);
	tmp->next=bgbmid_patchroot;
	bgbmid_patchroot=tmp;

	return(tmp);
}

BMID_API int BGBMID_TestPatchFilt(char *name, BGBMID_Patch *pat)
{
	BGBMID_Sample *cur;
	short *buf0, *buf1;
	float e0, e1, e2;
	int i, j;

	cur=pat->samp;
	while(cur)
	{
		j=cur->len*(1.0/M_E);
		buf0=bgbmid_malloc(j*sizeof(short));
		buf1=bgbmid_malloc(cur->len*sizeof(short));

#if 0
		BGBMID_ScaleSample(buf0, j, cur->pcm, cur->len);

		e0=0; e1=0; e2=0;

		BGBMID_ScaleSample(buf1, cur->len, buf0, j);
		for(i=0; i<cur->len; i++)
			e0+=fabs(cur->pcm[i]-buf1[i]);

		BGBMID_ScaleSampleSinc(buf1, cur->len, buf0, j);
		for(i=0; i<cur->len; i++)
			e1+=fabs(cur->pcm[i]-buf1[i]);

		BGBMID_ScaleSampleSpline(buf1, cur->len, buf0, j);
		for(i=0; i<cur->len; i++)
			e2+=fabs(cur->pcm[i]-buf1[i]);

		e0/=cur->len;
		e1/=cur->len;
		e2/=cur->len;

		printf("LERP=%f SINC=%f Spline=%f\n", e0, e1, e2);
#endif

#if 1
		e0=0; e1=0; e2=0;

		BGBMID_ScaleSample(buf0, j, cur->pcm, cur->len);
		BGBMID_ScaleSampleSpline(buf1, cur->len, buf0, j);
		for(i=0; i<cur->len; i++)
			e0+=fabs(cur->pcm[i]-buf1[i]);

		BGBMID_ScaleSampleSinc(buf0, j, cur->pcm, cur->len);
		BGBMID_ScaleSampleSpline(buf1, cur->len, buf0, j);
		for(i=0; i<cur->len; i++)
			e1+=fabs(cur->pcm[i]-buf1[i]);

		BGBMID_ScaleSampleSpline(buf0, j, cur->pcm, cur->len);
		BGBMID_ScaleSampleSpline(buf1, cur->len, buf0, j);
		for(i=0; i<cur->len; i++)
			e2+=fabs(cur->pcm[i]-buf1[i]);

		e0/=cur->len;
		e1/=cur->len;
		e2/=cur->len;

		printf("LERP=%f SINC=%f Spline=%f\n", e0, e1, e2);
#endif

		cur=cur->next;
	}
	return(0);
}

BMID_API int BGBMID_WaveBaseFreqIdx(short *bp, int len)
{
	int i, j, k, l, n, bi;
	float f, g, bf;
	
	n=4096;
	l=len; bi=0; bf=0;
	if(l>n)l=n;
	for(i=0; i<n; i++)
	{
		f=0;
		for(j=0; j<l; j++)
		{
//			g=cos((M_PI/n)*(j+0.5)*i);
			g=sin((M_PI/n)*(j+0.5)*i);
			f+=bp[j]*g;
		}
		
		g=(1.0*i)/n;
//		f=f*(1.0-g*g);
		f=f*(1.0-sqrt(g));
		
		if(fabs(f)>bf)
			{ bf=f; bi=i; }
	}
	
	return(bi);
}

BMID_API float BGBMID_WaveBaseFreq(short *bp, int rate, int len)
{
	float f;
	int i, j, k, l, n;
	
#if 0
	n=len/4096+1; k=0;
	if(n>4)n=4;
	for(i=0; i<n; i++)
	{
		l=len-(i*4096);
		j=BGBMID_WaveBaseFreqIdx(bp+(i*4096), l);
		k+=j;
	}
	
	i=k/n;
#endif

	i=BGBMID_WaveBaseFreqIdx(bp, len);

	f=i*(rate/4096.0);
	return(f);
}

BMID_API BGBMID_Patch *BGBMID_LoadPatchRawWav(char *name)
{
	char tb[256];
	BGBMID_Patch *tmp;
	BGBMID_Sample *cur, *lst, *nxt;
	void *fd;
	short *bp;
	char **a;
	char *s;
	int rt, len;
	int i, j, k;

	lst=NULL;

	sprintf(tb, "%s.wav", name);
	bp=BGBMID_LoadWAV_Mono16(tb, &rt, &len);
	if(!bp)
		return(NULL);

	cur=bgbmid_malloc(sizeof(BGBMID_Sample));
	memset(cur, 0, sizeof(BGBMID_Sample));
	cur->next=lst;
	lst=cur;

	cur->pcm=bp;
	cur->rate=rt;
	cur->len=len;
	cur->chan=1;

	cur->start=0;
	cur->end=len;

	k=440;
//	k=BGBMID_WaveBaseFreq(bp, rt, len);
//	printf("Tune Base Freq %dHz\n", k);

	cur->low=30;
	cur->high=4096;
//	cur->root=440;
	cur->root=k;

	cur->flags=103;

	BGBMID_SampleRebuildMip(cur);

	cur=lst; lst=NULL;
	while(cur)
	{
		nxt=cur->next;
		cur->next=lst;
		lst=cur; cur=nxt;
	}

	tmp=bgbmid_malloc(sizeof(BGBMID_Patch));
	memset(tmp, 0, sizeof(BGBMID_Patch));
	tmp->samp=lst;

	cur=lst;
	while(cur)
	{
		cur->patch=tmp;
		cur=cur->next;
	}

	tmp->name=bgbmid_strdup(name);
	tmp->next=bgbmid_patchroot;
	bgbmid_patchroot=tmp;

	return(tmp);
}

BMID_API BGBMID_Patch *BGBMID_LoadPatchWav(char *name)
{
	char tb[256];
	BGBMID_Patch *tmp;
	BGBMID_Sample *cur, *lst, *nxt;
	void *fd;
	short *bp;
	char **a;
	char *s;
	int rt, len;
	int i, j, k;

	if(!name)return(NULL);

	lst=NULL;

	sprintf(tb, "%s_inf.txt", name);
	fd=bgbmid_fopen(tb, "rt");

	if(!fd)
		return(NULL);

#if 0
	if(!fd)
	{
		sprintf(tb, "%s.wav", name, i);
		bp=BGBMID_LoadWAV_Mono16(tb, &rt, &len);
		if(!bp)
			return(NULL);

		cur=bgbmid_malloc(sizeof(BGBMID_Sample));
		memset(cur, 0, sizeof(BGBMID_Sample));
		cur->next=lst;
		lst=cur;

		cur->pcm=bp;
		cur->rate=rt;
		cur->len=len;
		cur->chan=1;

		cur->start=0;
		cur->end=len;

		k=BGBMID_WaveBaseFreq(bp, rt, len);
		printf("Tune Base Freq %dHz\n", k);

		cur->low=30;
		cur->high=4096;
//		cur->root=440;
		cur->root=k;

		cur->flags=103;
	}
#endif

	while(fd && !bgbmid_feof(fd))
	{
		tb[0]=0;
		bgbmid_fgets(tb, 255, fd);
		a=bgbmid_split(tb);

		if(!a[0])continue;
		if(*a[0]==';')continue;

		i=atoi(a[0]);
		sprintf(tb, "%s_%d.wav", name, i);
		bp=BGBMID_LoadWAV_Mono16(tb, &rt, &len);

		if(!bp)continue;

		cur=bgbmid_malloc(sizeof(BGBMID_Sample));
		memset(cur, 0, sizeof(BGBMID_Sample));
		cur->next=lst;
		lst=cur;

		cur->pcm=bp;
		cur->rate=rt;
		cur->len=len;
		cur->chan=1;

		cur->start=atoi(a[1]);
		cur->end=atoi(a[2]);

		cur->low=atof(a[3]);
		cur->high=atof(a[4]);
		cur->root=atof(a[5]);

		cur->flags=atoi(a[6]);

		BGBMID_SampleRebuildMip(cur);
	}

	if(fd)bgbmid_fclose(fd);

	cur=lst; lst=NULL;
	while(cur)
	{
		nxt=cur->next;
		cur->next=lst;
		lst=cur; cur=nxt;
	}

	tmp=bgbmid_malloc(sizeof(BGBMID_Patch));
	memset(tmp, 0, sizeof(BGBMID_Patch));
	tmp->samp=lst;

	cur=lst;
	while(cur)
	{
		cur->patch=tmp;
		cur=cur->next;
	}

	tmp->name=bgbmid_strdup(name);
	tmp->next=bgbmid_patchroot;
	bgbmid_patchroot=tmp;

	return(tmp);
}

BMID_API int BGBMID_StorePatchWav(char *name, BGBMID_Patch *pat)
{
	char tb[256];
	BGBMID_Sample *cur;
	void *fd;
	char *s;
	int i;

	if(!pat)return(-1);

	sprintf(tb, "%s_inf.txt", name);
	fd=bgbmid_fopen(tb, "wt");

	sprintf(tb, ";; idx  loop_strt loop_end  "
		"min_hz max_hz base_hz  flags\n");
	bgbmid_fwrite(tb, 1, strlen(tb), fd);

	cur=pat->samp; i=0;
	while(cur)
	{
		sprintf(tb, "%s_%d.wav", name, i);
		BGBMID_StoreWAV(tb, (byte *)cur->pcm,
			1, cur->rate, 16, cur->len);

		sprintf(tb, "%d  %d %d  %f %f %f  %d\n", i,
			cur->start, cur->end,
			cur->low, cur->high, cur->root, cur->flags);
		bgbmid_fwrite(tb, 1, strlen(tb), fd);

		cur=cur->next; i++;
	}

	bgbmid_fclose(fd);

	return(0);
}

BMID_API BGBMID_Patch *BGBMID_LoadPatchDummy(char *name)
{
	char tb[256];
	BGBMID_Patch *tmp;
	BGBMID_Sample *cur, *lst, *nxt;
	void *fd;
	short *bp;
	char **a;
	char *s;
	int rt, len;
	int i, j, k;

	lst=NULL;

//	sprintf(tb, "%s.wav", name, i);
//	bp=BGBMID_LoadWAV_Mono16(tb, &rt, &len);
//	if(!bp)
//		return(NULL);

	bp=bgbmid_malloc(256*sizeof(short));
	for(i=0; i<144; i++)
	{
		bp[i]=sin(M_TAU*(i/36.0))*32767;
	}
	
	rt=16000; len=144;

	cur=bgbmid_malloc(sizeof(BGBMID_Sample));
	memset(cur, 0, sizeof(BGBMID_Sample));
	cur->next=lst;
	lst=cur;

	cur->pcm=bp;
	cur->rate=rt;
	cur->len=len;
	cur->chan=1;

	cur->start=0;
	cur->end=len;

//	k=BGBMID_WaveBaseFreq(bp, rt, len);
//	printf("Tune Base Freq %dHz\n", k);
	k=440;

	cur->low=30;
	cur->high=4096;
//	cur->root=440;
	cur->root=k;

	cur->flags=103;

	BGBMID_SampleRebuildMip(cur);

	cur=lst; lst=NULL;
	while(cur)
	{
		nxt=cur->next;
		cur->next=lst;
		lst=cur; cur=nxt;
	}

	tmp=bgbmid_malloc(sizeof(BGBMID_Patch));
	memset(tmp, 0, sizeof(BGBMID_Patch));
	tmp->samp=lst;
	
	cur=lst;
	while(cur)
	{
		cur->patch=tmp;
		cur=cur->next;
	}

	tmp->name=bgbmid_strdup(name);
	tmp->next=bgbmid_patchroot;
	bgbmid_patchroot=tmp;

	return(tmp);
}

BMID_API BGBMID_Patch *BGBMID_CreatePatchStream(
	char *name, int rate, int len)
{
	char tb[256];
	BGBMID_Patch *tmp;
	BGBMID_Sample *cur, *lst, *nxt;
	void *fd;
	short *bp;
	char **a;
	char *s;
	int i, j, k;

	lst=NULL;

//	sprintf(tb, "%s.wav", name, i);
//	bp=BGBMID_LoadWAV_Mono16(tb, &rt, &len);
//	if(!bp)
//		return(NULL);

	cur=bgbmid_malloc(sizeof(BGBMID_Sample));
	memset(cur, 0, sizeof(BGBMID_Sample));
	cur->next=lst;
	lst=cur;

	bp=bgbmid_malloc(len*sizeof(short));
	cur->pcm=bp;
	cur->rate=rate;
	cur->len=len;
	cur->chan=1;

	cur->start=0;
	cur->end=len;

//	k=BGBMID_WaveBaseFreq(bp, rt, len);
//	printf("Tune Base Freq %dHz\n", k);
	k=440;

	cur->low=30;
	cur->high=4096;
//	cur->root=440;
	cur->root=k;

	cur->flags=103;
	cur->flags|=BGBMID_SFL_STREAM;

//	BGBMID_SampleRebuildMip(cur);

	cur=lst; lst=NULL;
	while(cur)
	{
		nxt=cur->next;
		cur->next=lst;
		lst=cur; cur=nxt;
	}

	tmp=bgbmid_malloc(sizeof(BGBMID_Patch));
	memset(tmp, 0, sizeof(BGBMID_Patch));
	tmp->samp=lst;

	cur=lst;
	while(cur)
	{
		cur->patch=tmp;
		cur=cur->next;
	}

	tmp->name=bgbmid_strdup(name);
	tmp->next=bgbmid_patchroot;
	bgbmid_patchroot=tmp;

	return(tmp);
}

BMID_API BGBMID_Patch *BGBMID_CreatePatchSamples(
	char *name, short *buf, int rate, int len)
{
	char tb[256];
	BGBMID_Patch *tmp;
	BGBMID_Sample *cur, *lst, *nxt;
	void *fd;
	short *bp;
	char **a;
	char *s;
	int i, j, k;

	lst=NULL;

//	sprintf(tb, "%s.wav", name, i);
//	bp=BGBMID_LoadWAV_Mono16(tb, &rt, &len);
//	if(!bp)
//		return(NULL);

	cur=bgbmid_malloc(sizeof(BGBMID_Sample));
	memset(cur, 0, sizeof(BGBMID_Sample));
	cur->next=lst;
	lst=cur;

	bp=bgbmid_malloc(len*sizeof(short));
	cur->pcm=bp;
	cur->rate=rate;
	cur->len=len;
	cur->chan=1;

	cur->start=0;
	cur->end=len;

	memcpy(bp, buf, len*sizeof(short));

//	k=BGBMID_WaveBaseFreq(bp, rt, len);
//	printf("Tune Base Freq %dHz\n", k);
	k=440;

	cur->low=30;
	cur->high=4096;
//	cur->root=440;
	cur->root=k;

	cur->flags=103;
//	cur->flags|=BGBMID_SFL_STREAM;

//	BGBMID_SampleRebuildMip(cur);

	cur=lst; lst=NULL;
	while(cur)
	{
		nxt=cur->next;
		cur->next=lst;
		lst=cur; cur=nxt;
	}

	tmp=bgbmid_malloc(sizeof(BGBMID_Patch));
	memset(tmp, 0, sizeof(BGBMID_Patch));
	tmp->samp=lst;

	cur=lst;
	while(cur)
	{
		cur->patch=tmp;
		cur=cur->next;
	}

	tmp->name=bgbmid_strdup(name);
	tmp->next=bgbmid_patchroot;
	bgbmid_patchroot=tmp;

	return(tmp);
}

#if 0
// BMID_API BGBMID_Patch *BGBMID_LoadPatchPakBTAC(char *name)
{
	char tb[256];
	BGBMID_Patch *tmp;
	BGBMID_Sample *cur, *lst, *nxt;
	void *fd;
	short *bp;
	char **a;
	char *s, *cs, *infbuf;
	int rt, len;
	int i, j, k;

	if(!name)return(NULL);

	lst=NULL;

	sprintf(tb, "%s_inf.txt", name);
//	fd=bgbmid_fopen(tb, "rt");
	infbuf=BGBMID_PackReadFile(tb, NULL);

	if(!infbuf)
		return(NULL);

	cs=infbuf;
	while(*cs)
	{
		s=bgbmid_rgets(&cs);
		a=bgbmid_split(s);

		if(!a[0])continue;
		if(*a[0]==';')continue;

		i=atoi(a[0]);
		sprintf(tb, "%s_%d.btac", name, i);
		
		cur=BGBMID_BTAC_LoadSample(tb);
		
		cur->next=lst;
		lst=cur;

		cur->start=atoi(a[1]);
		cur->end=atoi(a[2]);

		cur->low=atof(a[3]);
		cur->high=atof(a[4]);
		cur->root=atof(a[5]);

		cur->flags=atoi(a[6]);

//		BGBMID_SampleRebuildMip(cur);
	}

	bgbmid_free(infbuf);

//	if(fd)bgbmid_fclose(fd);

	cur=lst; lst=NULL;
	while(cur)
	{
		nxt=cur->next;
		cur->next=lst;
		lst=cur; cur=nxt;
	}

	tmp=bgbmid_malloc(sizeof(BGBMID_Patch));
	memset(tmp, 0, sizeof(BGBMID_Patch));
	tmp->samp=lst;

	cur=lst;
	while(cur)
	{
		cur->patch=tmp;
		cur=cur->next;
	}

	tmp->name=bgbmid_strdup(name);
	tmp->next=bgbmid_patchroot;
	bgbmid_patchroot=tmp;

	return(tmp);
}
#endif

BMID_API int BGBMID_DumpPatches()
{
	BGBMID_Patch *pat;
	int i;

	for(i=0; i<128; i++)
	{
		pat=BGBMID_LoadPatchGUS(bgbmid_patches[i]);
		BGBMID_StorePatchWav(bgbmid_patches[i], pat);
	}

	for(i=0; i<128; i++)
	{
		pat=BGBMID_LoadPatchGUS(bgbmid_drumpatches[i]);
		BGBMID_StorePatchWav(bgbmid_drumpatches[i], pat);
	}

	return(0);
}

BMID_API BGBMID_Patch *BGBMID_LoadPatch(char *name, int idx)
{
	static byte plf[256];
	BGBMID_Patch *tmp;

	if(!name)return(NULL);
	
	if(plf[idx&255])
		return(NULL);

	tmp=BGBMID_LookupPatch(name);
	if(tmp)return(tmp);

	printf("Load Patch %s (%d)\n", name, idx);

#if 1
	tmp=BGBMID_LoadPatchWav(name);
	if(tmp)return(tmp);

	tmp=BGBMID_LoadPatchGUS(name);
	if(tmp)return(tmp);
#endif

//	tmp=BGBMID_LoadPatchPakBTAC(name);
//	if(tmp)return(tmp);

	tmp=BGBMID_LoadPatchRawWav(name);
	if(tmp)return(tmp);

//	tmp=BGBMID_LoadPatchRawBTAC(name);
//	if(tmp)return(tmp);

	printf("Load Patch: Fail %s\n", name);
	plf[idx&255]=1;
	return(NULL);
}

