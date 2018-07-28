/*

Patch Notes
238 bytes for header
byte 198=samples

Samples follow header

Sample:
7 byte name, ignored
1 fractions
int data_len
int loop_start
int loop_end
short rate
int low_freq
int high_freq
int root_freq

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#define MID_SFL_16BIT		1
#define MID_SFL_UNSIGNED	2
#define MID_SFL_LOOP		4
#define MID_SFL_BIDIRLOOP	8
#define MID_SFL_REVERSE		16
#define MID_SFL_ENVELOPESUST	32
#define MID_SFL_ENVELOPEFILT	64

typedef unsigned char byte;

#include "wave.c"

//byte mid_notemask[16][16];
byte mid_notevel[16][128];
unsigned short mid_control[16][32];
byte mid_prog[16];

int mid_note_strt[16][128];
int mid_note_end[16][128];

short *mid_pcmbuf;
short *mid_pcmbufe;
short *mid_pcmend;
float *mid_pcmbuft;
float mid_ticksamp;
float mid_ratesc;

typedef struct Sample_S Sample;
struct Sample_S {
Sample *next;
short *pcm;
int len;
int rate;
int start, end;
int flags;
float low, high, root;
};

char *mid_patches[128]=
{
/* 0 */ "instruments/acpiano.pat",
/* 1 */ "instruments/britepno.pat",
/* 2 */ "instruments/synpiano.pat",
/* 3 */ "instruments/honky.pat",
/* 4 */ "instruments/epiano1.pat",
/* 5 */ "instruments/epiano2.pat",
/* 6 */ "instruments/hrpschrd.pat",
/* 7 */ "instruments/clavinet.pat",
/* 8 */ "instruments/celeste.pat",
/* 9 */ "instruments/glocken.pat",
/* 10 */ "instruments/musicbox.pat",
/* 11 */ "instruments/vibes.pat",
/* 12 */ "instruments/marimba.pat",
/* 13 */ "instruments/xylophon.pat",
/* 14 */ "instruments/tubebell.pat",
/* 15 */ "instruments/santur.pat",
/* 16 */ "instruments/homeorg.pat",
/* 17 */ "instruments/percorg.pat",
/* 18 */ "instruments/rockorg.pat",
/* 19 */ "instruments/church.pat",
/* 20 */ "instruments/reedorg.pat",
/* 21 */ "instruments/accordn.pat",
/* 22 */ "instruments/harmonca.pat",
/* 23 */ "instruments/concrtna.pat",
/* 24 */ "instruments/nyguitar.pat",
/* 25 */ "instruments/acguitar.pat",
/* 26 */ "instruments/jazzgtr.pat",
/* 27 */ "instruments/cleangtr.pat",
/* 28 */ "instruments/mutegtr.pat",
/* 29 */ "instruments/odguitar.pat",
/* 30 */ "instruments/distgtr.pat",
/* 31 */ "instruments/gtrharm.pat",
/* 32 */ "instruments/acbass.pat",
/* 33 */ "instruments/fngrbass.pat",
/* 34 */ "instruments/pickbass.pat",
/* 35 */ "instruments/fretless.pat",
/* 36 */ "instruments/slapbas1.pat",
/* 37 */ "instruments/slapbas2.pat",
/* 38 */ "instruments/synbass1.pat",
/* 39 */ "instruments/synbass2.pat",
/* 40 */ "instruments/violin.pat",
/* 41 */ "instruments/viola.pat",
/* 42 */ "instruments/cello.pat",
/* 43 */ "instruments/contraba.pat",
/* 44 */ "instruments/tremstr.pat",
/* 45 */ "instruments/pizzcato.pat",
/* 46 */ "instruments/harp.pat",
/* 47 */ "instruments/timpani.pat",
/* 48 */ "instruments/marcato.pat",
/* 49 */ "instruments/slowstr.pat",
/* 50 */ "instruments/synstr1.pat",
/* 51 */ "instruments/synstr2.pat",
/* 52 */ "instruments/choir.pat",
/* 53 */ "instruments/doo.pat",
/* 54 */ "instruments/voices.pat",
/* 55 */ "instruments/orchhit.pat",
/* 56 */ "instruments/trumpet.pat",
/* 57 */ "instruments/trombone.pat",
/* 58 */ "instruments/tuba.pat",
/* 59 */ "instruments/mutetrum.pat",
/* 60 */ "instruments/frenchrn.pat",
/* 61 */ "instruments/hitbrass.pat",
/* 62 */ "instruments/synbras1.pat",
/* 63 */ "instruments/synbras2.pat",
/* 64 */ "instruments/sprnosax.pat",
/* 65 */ "instruments/altosax.pat",
/* 66 */ "instruments/tenorsax.pat",
/* 67 */ "instruments/barisax.pat",
/* 68 */ "instruments/oboe.pat",
/* 69 */ "instruments/englhorn.pat",
/* 70 */ "instruments/bassoon.pat",
/* 71 */ "instruments/clarinet.pat",
/* 72 */ "instruments/piccolo.pat",
/* 73 */ "instruments/flute.pat",
/* 74 */ "instruments/recorder.pat",
/* 75 */ "instruments/woodflut.pat",
/* 76 */ "instruments/bottle.pat",
/* 77 */ "instruments/shakazul.pat",
/* 78 */ "instruments/whistle.pat",
/* 79 */ "instruments/ocarina.pat",
/* 80 */ "instruments/sqrwave.pat",
/* 81 */ "instruments/sawwave.pat",
/* 82 */ "instruments/calliope.pat",
/* 83 */ "instruments/chiflead.pat",
/* 84 */ "instruments/charang.pat",
/* 85 */ "instruments/voxlead.pat",
/* 86 */ "instruments/lead5th.pat",
/* 87 */ "instruments/basslead.pat",
/* 88 */ "instruments/fantasia.pat",
/* 89 */ "instruments/warmpad.pat",
/* 90 */ "instruments/polysyn.pat",
/* 91 */ "instruments/ghostie.pat",
/* 92 */ "instruments/bowglass.pat",
/* 93 */ "instruments/metalpad.pat",
/* 94 */ "instruments/halopad.pat",
/* 95 */ "instruments/sweeper.pat",
/* 96 */ "instruments/aurora.pat",
/* 97 */ "instruments/soundtrk.pat",
/* 98 */ "instruments/crystal.pat",
/* 99 */ "instruments/atmosphr.pat",
/* 100 */ "instruments/freshair.pat",
/* 101 */ "instruments/unicorn.pat",
/* 102 */ "instruments/echovox.pat",
/* 103 */ "instruments/startrak.pat",
/* 104 */ "instruments/sitar.pat",
/* 105 */ "instruments/banjo.pat",
/* 106 */ "instruments/shamisen.pat",
/* 107 */ "instruments/koto.pat",
/* 108 */ "instruments/kalimba.pat",
/* 109 */ "instruments/bagpipes.pat",
/* 110 */ "instruments/fiddle.pat",
/* 111 */ "instruments/shannai.pat",
/* 112 */ "instruments/carillon.pat",
/* 113 */ "instruments/agogo.pat",
/* 114 */ "instruments/steeldrm.pat",
/* 115 */ "instruments/woodblk.pat",
/* 116 */ "instruments/taiko.pat",
/* 117 */ "instruments/toms.pat",
/* 118 */ "instruments/syntom.pat",
/* 119 */ "instruments/revcym.pat",
/* 120 */ "instruments/fx-fret.pat",
/* 121 */ "instruments/fx-blow.pat",
/* 122 */ "instruments/seashore.pat",
/* 123 */ "instruments/jungle.pat",
/* 124 */ "instruments/telephon.pat",
/* 125 */ "instruments/helicptr.pat",
/* 126 */ "instruments/applause.pat",
/* 127 */ "instruments/pistol.pat"
};

char *mid_drumpatches[128]=
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

/* 27 */ "instrument/highq.pat",
/* 28 */ "instruments/slap.pat",
/* 29 */ "instruments/scratch1.pat",
/* 30 */ "instruments/scratch2.pat",
/* 31 */ "instruments/sticks.pat",
/* 32 */ "instruments/sqrclick.pat",
/* 33 */ "instruments/metclick.pat",
/* 34 */ "instruments/metbell.pat",
/* 35 */ "instruments/kick1.pat",
/* 36 */ "instruments/kick2.pat",
/* 37 */ "instruments/stickrim.pat",
/* 38 */ "instruments/snare1.pat",
/* 39 */ "instruments/claps.pat",
/* 40 */ "instruments/snare2.pat",
/* 41 */ "instruments/tomlo2.pat",
/* 42 */ "instruments/hihatcl.pat",
/* 43 */ "instruments/tomlo1.pat",
/* 44 */ "instruments/hihatpd.pat",
/* 45 */ "instruments/tommid2.pat",
/* 46 */ "instruments/hihatop.pat",
/* 47 */ "instruments/tommid1.pat",
/* 48 */ "instruments/tomhi2.pat",
/* 49 */ "instruments/cymcrsh1.pat",
/* 50 */ "instruments/tomhi1.pat",
/* 51 */ "instruments/cymride1.pat",
/* 52 */ "instruments/cymchina.pat",
/* 53 */ "instruments/cymbell.pat",
/* 54 */ "instruments/tamborin.pat",
/* 55 */ "instruments/cymsplsh.pat",
/* 56 */ "instruments/cowbell.pat",
/* 57 */ "instruments/cymcrsh2.pat",
/* 58 */ "instruments/vibslap.pat",
/* 59 */ "instruments/cymride2.pat",
/* 60 */ "instruments/bongohi.pat",
/* 61 */ "instruments/bongolo.pat",
/* 62 */ "instruments/congahi1.pat",
/* 63 */ "instruments/congahi2.pat",
/* 64 */ "instruments/congalo.pat",
/* 65 */ "instruments/timbaleh.pat",
/* 66 */ "instruments/timbalel.pat",
/* 67 */ "instruments/agogohi.pat",
/* 68 */ "instruments/agogolo.pat",
/* 69 */ "instruments/cabasa.pat",
/* 70 */ "instruments/maracas.pat",
/* 71 */ "instruments/whistle1.pat",
/* 72 */ "instruments/whistle2.pat",
/* 73 */ "instruments/guiro1.pat",
/* 74 */ "instruments/guiro2.pat",
/* 75 */ "instruments/clave.pat",
/* 76 */ "instruments/woodblk1.pat",
/* 77 */ "instruments/woodblk2.pat",
/* 78 */ "instruments/cuica1.pat",
/* 79 */ "instruments/cuica2.pat",
/* 80 */ "instruments/triangl1.pat",
/* 81 */ "instruments/triangl2.pat",
/* 82 */ "instruments/shaker.pat",
/* 83 */ "instruments/jingles.pat",
/* 84 */ "instruments/belltree.pat",
/* 85 */ "instruments/castinet.pat",
/* 86 */ "instruments/surdo1.pat",
/* 87 */ "instruments/surdo2.pat",

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

Sample *mid_samples[128];
Sample *mid_drumsamples[128];

int MID_ReadS16LE(FILE *fd)
{
	short i;
	i=fgetc(fd);
	i|=fgetc(fd)<<8;
	return(i);
}

int MID_ReadS32LE(FILE *fd)
{
	int i;
	i=fgetc(fd);
	i|=fgetc(fd)<<8;
	i|=fgetc(fd)<<16;
	i|=fgetc(fd)<<24;
	return(i);
}

struct patch_tsamp_s {
int size;
int start, end;
int rate;
int low, high, root;
int fscale, fscalef;
int modes;
};

Sample *MID_LoadPatch(char *name)
{
	byte tb[1024];
	struct patch_tsamp_s tsmp[16];
	Sample *csmp, *lsmp, *rsmp;
	byte *buf;
	FILE *fd;
	int nsmp;
	int i, j, k, l;

	if(!name)return(NULL);

	printf("Load Patch %s\n", name);

	fd=fopen(name, "rb");
	if(!fd)return(NULL);

	fread(tb, 1, 239, fd);

	if(memcmp(tb, "GF1PATCH110\0ID#000002", 22))
	{
		printf("\tBad Patch\n");
		fclose(fd);
		return(NULL);
	}

	nsmp=tb[198];
//	printf("\t%d Samples\n", nsmp);

	lsmp=NULL;

	for(i=0; i<nsmp; i++)
	{
		fread(tb, 1, 7, fd);
		tb[7]=0;

//		printf("\t%s\n", tb);

		fgetc(fd);		//fractions
		tsmp[i].size=MID_ReadS32LE(fd);		//data len
		tsmp[i].start=MID_ReadS32LE(fd);	//loop start
		tsmp[i].end=MID_ReadS32LE(fd);		//loop end

		tsmp[i].rate=MID_ReadS16LE(fd);		//sample rate
		tsmp[i].low=MID_ReadS32LE(fd);		//low freq
		tsmp[i].high=MID_ReadS32LE(fd);		//high freq
		tsmp[i].root=MID_ReadS32LE(fd);		//root freq
		MID_ReadS16LE(fd);	//tuning

		tsmp[i].rate&=0xFFFF;

		fgetc(fd);
		fread(tb, 1, 18, fd);

		tsmp[i].modes=fgetc(fd);		//modes
		tsmp[i].fscale=MID_ReadS16LE(fd);	//frequency scale
		tsmp[i].fscalef=MID_ReadS16LE(fd);	//frequency scale factor

//		printf("\t%d %d\n", tsmp[i].fscale, tsmp[i].fscalef);

		fread(tb, 1, 36, fd);

		buf=bgbmid_malloc(tsmp[i].size+16);
		j=fread(buf, 1, tsmp[i].size, fd);

		if(j!=tsmp[i].size)
		{
			printf("\tPremature EOF\n");
			break;
		}


		csmp=bgbmid_malloc(sizeof(Sample));
		memset(csmp, 0, sizeof(Sample));
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

#if 0	//normalize
		l=1;
//		for(j=0; j<csmp->len; j++)
		for(j=csmp->start; j<csmp->end; j++)
		{
			k=abs(csmp->pcm[j]);
			if(k>l)l=k;
		}

//		for(j=0; j<csmp->len; j++)
//			csmp->pcm[j]*=32767.0/l;

		for(j=0; j<csmp->len; j++)
			csmp->pcm[j]*=24576.0/l;
#endif

		csmp->rate=tsmp[i].rate;
		csmp->flags=tsmp[i].modes;

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


		//modes&4 == loop
		//modes&8 == bidir loop
		//modes&16 == reverse

		printf("\tSample(%p) Len=%d Rt=%d (Strt=%d End=%d)\n",
			csmp,
			csmp->len, csmp->rate,
			csmp->start, csmp->end);

		printf("\tLow=%f High=%f Root=%f\n",
			csmp->low, csmp->high, csmp->root);
	}

	fclose(fd);

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

			BSWV_ScaleSampleSinc(
				(short *)buf, j, csmp->pcm, csmp->len);
			bgbmid_free(csmp->pcm);

			csmp->pcm=(short *)buf;
			csmp->len=j;
			csmp->rate=i;

			csmp->start=csmp->start*(((float)i)/csmp->rate);
			csmp->end=csmp->end*(((float)i)/csmp->rate);
		}

		csmp=csmp->next;
	}
#endif

//	MID_StorePatchWav(name, lsmp);
//	MID_TestPatchFilt(name, lsmp);

	return(lsmp);
}

int MID_TestPatchFilt(char *name, Sample *samp)
{
	Sample *cur;
	short *buf0, *buf1;
	float e0, e1, e2;
	int i, j;

	cur=samp;
	while(cur)
	{
		j=cur->len*(1.0/M_E);
		buf0=bgbmid_malloc(j*sizeof(short));
		buf1=bgbmid_malloc(cur->len*sizeof(short));

#if 0
		BSWV_ScaleSample(buf0, j, cur->pcm, cur->len);

		e0=0; e1=0; e2=0;

		BSWV_ScaleSample(buf1, cur->len, buf0, j);
		for(i=0; i<cur->len; i++)
			e0+=fabs(cur->pcm[i]-buf1[i]);

		BSWV_ScaleSampleSinc(buf1, cur->len, buf0, j);
		for(i=0; i<cur->len; i++)
			e1+=fabs(cur->pcm[i]-buf1[i]);

		BSWV_ScaleSampleSpline(buf1, cur->len, buf0, j);
		for(i=0; i<cur->len; i++)
			e2+=fabs(cur->pcm[i]-buf1[i]);

		e0/=cur->len;
		e1/=cur->len;
		e2/=cur->len;

		printf("LERP=%f SINC=%f Spline=%f\n", e0, e1, e2);
#endif

#if 1
		e0=0; e1=0; e2=0;

		BSWV_ScaleSample(buf0, j, cur->pcm, cur->len);
		BSWV_ScaleSampleSpline(buf1, cur->len, buf0, j);
		for(i=0; i<cur->len; i++)
			e0+=fabs(cur->pcm[i]-buf1[i]);

		BSWV_ScaleSampleSinc(buf0, j, cur->pcm, cur->len);
		BSWV_ScaleSampleSpline(buf1, cur->len, buf0, j);
		for(i=0; i<cur->len; i++)
			e1+=fabs(cur->pcm[i]-buf1[i]);

		BSWV_ScaleSampleSpline(buf0, j, cur->pcm, cur->len);
		BSWV_ScaleSampleSpline(buf1, cur->len, buf0, j);
		for(i=0; i<cur->len; i++)
			e2+=fabs(cur->pcm[i]-buf1[i]);

		e0/=cur->len;
		e1/=cur->len;
		e2/=cur->len;

		printf("LERP=%f SINC=%f Spline=%f\n", e0, e1, e2);
#endif

		cur=cur->next;
	}
}

int MID_StorePatchWav(char *name, Sample *samp)
{
	char tb0[256], tb1[256];
	Sample *cur;
	FILE *fd;
	char *s;
	int i;

	if(!samp)return(-1);

	strcpy(tb0, name);
	s=tb0+strlen(tb0);
	while((s>tb0) && (*s!='.'))s--;
	*s=0;

	sprintf(tb1, "%s_inf.txt", tb0);
	fd=fopen(tb1, "wt");

	cur=samp; i=0;
	while(cur)
	{
		sprintf(tb1, "%s_%d.wav", tb0, i);
		BSWV_StoreWAV(tb1, (byte *)cur->pcm,
			1, cur->rate, 16, cur->len);

		fprintf(fd, "%d  %d %d  %f %f %f\n", i,
			cur->start, cur->end,
			cur->low, cur->high, cur->root);

		cur=cur->next; i++;
	}

	fclose(fd);

	return(0);
}

int MID_ReadS16(FILE *fd)
{
	short i;
	i=fgetc(fd)<<8;
	i|=fgetc(fd);
	return(i);
}

int MID_ReadS32(FILE *fd)
{
	int i;
	i=fgetc(fd)<<24;
	i|=fgetc(fd)<<16;
	i|=fgetc(fd)<<8;
	i|=fgetc(fd);
	return(i);
}

int MID_ReadDelta(FILE *fd)
{
	int i, j;

	i=0;
	j=fgetc(fd);
	while(j>=0x80)
	{
		i=(i<<7)|(j&0x7F);
		j=fgetc(fd);
	}
	i=(i<<7)|(j&0x7F);
	return(i);
}

int MID_InitChannels()
{
	int i, j;

	for(i=0; i<16; i++)
	{
		for(j=0; j<16; j++)
			mid_control[i][j]=0;

		mid_control[i][7]=100<<7;
		mid_control[i][8]=64<<7;
		mid_control[i][10]=64<<7;
		mid_control[i][31]=64<<7;

		for(j=0; j<128; j++)
			mid_notevel[i][j]=0;

		MID_ProgramChange(i, 0);
	}

	return(0);
}

#if 0
int MID_BlendNote(int chan, int len, int note)
{
	Sample *samp;
	float vol, lvol, rvol, freq, pan, bal, pbl;
	float f, g, ssc;
	int i, j, k, b, ss, se, sm;
	int i0, i1;

	vol=mid_control[chan][7];
	bal=mid_control[chan][8];
	pan=mid_control[chan][10];
//	if(vol<256.0)return(0);

	vol=pow(vol/16384.0, 1.66)*16384.0;
//	vol*=0.25;

//	f=pan/16384.0;
//	lvol=vol*(1.0-f);
//	rvol=vol*f;

//	g=sin((M_PI/2)*f);
//	lvol=vol*(g+cos((M_PI/2)*f))*0.707;
//	rvol=vol*(g+cos((M_PI/2)*(1.0-f)))*0.707;

	f=pan/16384.0;
	lvol=1.0-f; rvol=f;
	g=sqrt(lvol*lvol + rvol*rvol);
	lvol=vol*lvol*(1.0/g);
	rvol=vol*rvol*(1.0/g);

	f=bal/16384.0;
	lvol*=1.0-f;
	rvol*=f;

	b=(mid_pcmbufe-mid_pcmbuf)/2;
	freq=pow(2, (note-69)/12.0)*440;

	pbl=(mid_control[chan][31]-8192)/8192.0;
	freq*=pow(2, pbl);

	samp=mid_samples[mid_prog[chan]];
	while(samp)
	{
		if((freq>=samp->low) && (freq<=samp->high))
			break;
		samp=samp->next;
	}

	if(!samp)samp=mid_samples[mid_prog[chan]];
//	if(!samp)return(0);

//	samp=NULL;

	if(!samp)
	{
		for(j=0; j<len; j++)
		{
			f=sin(M_PI/22050*(b+j)*freq);
//			f=rint(f);

			mid_pcmbuft[j*2+0]+=(f*lvol);
			mid_pcmbuft[j*2+1]+=(f*rvol);
		}

		return(0);
	}

	ss=samp->start;
	se=samp->end;
	sm=se-ss;
//	ssc=(samp->rate/44100.0)/440.0;
	ssc=(samp->rate/44100.0)/samp->root;
//	ssc=(samp->rate/44100.0)/(samp->root*2);

	ssc*=freq;
	lvol/=32767.0;
	rvol/=32767.0;

	for(i=0; i<len; i++)
	{
//		f=sin(M_PI/22050*(b+i)*freq);

//		f=ssc*(b+i)*freq;
		f=ssc*(b+i);
		j=(int)f; f-=j;
		i0=ss+(j%sm); i1=i0+1;
//		i1=ss+((j+1)%sm);
//		f=(samp->pcm[i0]*(1.0-f) + samp->pcm[i1]*f) / 32767.0;
		f=samp->pcm[i0]*(1.0-f) + samp->pcm[i1]*f;

		mid_pcmbuft[i*2+0]+=f*lvol;
		mid_pcmbuft[i*2+1]+=f*rvol;
	}

	return(0);
}
#endif

int MID_BlendNoteTone(int chan, int len, int note)
{
	float vol, lvol, rvol, freq, pan, bal, pbl;
	float f, g, ssc;
	int i, j, k, b, ss, se, sm;
	int i0, i1;

	vol=mid_control[chan][7];
	bal=mid_control[chan][8]/16384.0;
	pan=mid_control[chan][10]/16384.0;

	if(note>=0)
	{
		f=mid_notevel[chan][note]/127.0;
		vol*=f;
	}

	vol=pow(vol/16384.0, 1.66)*16384.0;

//	f=bal/16384.0;
//	lvol=1.0-f; rvol=f;
//	g=sqrt(lvol*lvol + rvol*rvol);
//	lvol=vol*lvol*(1.0/g);
//	rvol=vol*rvol*(1.0/g);

	lvol=(1.0-bal)*(1.0-pan*0.5);
	rvol=(0.0+bal)*(0.5+pan*0.5);
	g=sqrt(lvol*lvol + rvol*rvol);
	lvol=vol*lvol*(1.0/g);
	rvol=vol*rvol*(1.0/g);

//	f=bal/16384.0;
//	lvol*=1.0-f;
//	rvol*=f;


	b=(mid_pcmbufe-mid_pcmbuf)/2;
	freq=pow(2, (note-69)/12.0)*440;

	pbl=(mid_control[chan][31]-8192)/8192.0;
	freq*=pow(2, pbl);

	for(j=0; j<len; j++)
	{
		f=sin(M_PI/22050*(b+j)*freq);
		mid_pcmbuft[j*2+0]+=(f*lvol);
		mid_pcmbuft[j*2+1]+=(f*rvol);
	}

	return(0);
}

int MID_BlendNoteSampLoop(int chan, int len, int note, Sample *samp)
{
	float vol, lvol, rvol, freq, pan, bal, pbl;
	float loffs, roffs;
	float f, g, ssc;
	int i, j, k, b, ss, se, sm, b2, b3;
	int i0, i1;

	vol=mid_control[chan][7];
	bal=mid_control[chan][8]/16384.0;
	pan=mid_control[chan][10]/16384.0;

//	vol=pow(vol/16384.0, 1.66)*16384.0;

	if(note>=0)
	{
		f=mid_notevel[chan][note]/127.0;
//		f=pow(f, 1.66);
		vol*=f;
	}

	vol=pow(vol/16384.0, 1.66)*16384.0;
//	vol=16384;

//	f=pan;
//	f=bal;
	lvol=(1.0-bal)*(1.0-pan*0.5);
	rvol=(0.0+bal)*(0.5+pan*0.5);
	g=sqrt(lvol*lvol + rvol*rvol);
	lvol=vol*lvol*(1.0/g);
	rvol=vol*rvol*(1.0/g);

//	f=bal;
//	lvol*=1.0-f;
//	rvol*=f;

//	rvol=16384;

	f=pan;
	loffs=f; roffs=1.0-f;
	g=sqrt(loffs*loffs + roffs*roffs);
	loffs=loffs*(1.0/g);
	roffs=roffs*(1.0/g);

	b=(mid_pcmbufe-mid_pcmbuf)/2;

	if(note>=0)
	{
		freq=pow(2, (note-69)/12.0)*440;
		pbl=(mid_control[chan][31]-8192)/8192.0;
		freq*=pow(2, pbl);
		b2=b-mid_note_strt[chan][note];
		b3=b-mid_note_end[chan][note];
	}else
	{
		freq=samp->root;
		b2=b-mid_note_strt[chan][0];
		b3=b-mid_note_end[chan][0];
	}

	ss=samp->start;
	se=samp->end;
	sm=se-ss;
	ssc=(samp->rate/44100.0)/samp->root;

	ssc*=freq;
	lvol/=32767.0;
	rvol/=32767.0;

	loffs=loffs*0.0010*samp->rate;
	roffs=roffs*0.0010*samp->rate;

#if 1
	if(b3!=b)
	{
//		if((ssc*b2)<samp->end)
//		{
//			MID_BlendNoteSampNoLoop(chan, len, note, samp);
//			return(0);
//		}

		i=0;

#if 0
		for(i=0; i<len; i++)
		{
			f=samp->end + ssc*(b3+i);
			if(f>=samp->len)break;

			f=samp->end + ssc*(b3+i)-loffs;

			j=(int)f; f-=j;
			i0=j; i1=j+1;
			f=samp->pcm[i0]*(1.0-f) + samp->pcm[i1]*f;

//			f=BSWV_SincPolate(samp->pcm, samp->len, f, 16);
			mid_pcmbuft[i*2+0]+=f*lvol;

			f=samp->end + ssc*(b3+i)-roffs;
			j=(int)f; f-=j;
			i0=j; i1=j+1;
			f=samp->pcm[i0]*(1.0-f) + samp->pcm[i1]*f;

//			f=BSWV_SincPolate(samp->pcm, samp->len, f, 16);
			mid_pcmbuft[i*2+1]+=f*rvol;
		}
#endif

		if(i<len)
		{
			if(note>=0)
				mid_notevel[chan][note]=0;
			else	mid_notevel[chan][0]=0;
		}

		return(0);
	}
#endif

#if 1
	if((ssc*b2)<samp->start)
	{
		for(i=0; i<len; i++)
		{
			f=ssc*(b2+i);
			if(f>=samp->start)break;
//			if(f>=samp->len)break;

			f=ssc*(b2+i)-loffs;
			if(f<0)f=0;

//			j=(int)f; f-=j;
//			i0=j; i1=j+1;
//			f=samp->pcm[i0]*(1.0-f) + samp->pcm[i1]*f;

			f=BSWV_SplinePolate(samp->pcm, samp->len, f);
//			f=BSWV_SincPolate(samp->pcm, samp->len, f, 16);
			mid_pcmbuft[i*2+0]+=f*lvol;

			f=ssc*(b2+i)-roffs;
			if(f<0)f=0;

//			j=(int)f; f-=j;
//			i0=j; i1=j+1;
//			f=samp->pcm[i0]*(1.0-f) + samp->pcm[i1]*f;

			f=BSWV_SplinePolate(samp->pcm, samp->len, f);
//			f=BSWV_SincPolate(samp->pcm, samp->len, f, 16);
			mid_pcmbuft[i*2+1]+=f*rvol;
		}

#if 1
		for(; i<len; i++)
		{
			f=ssc*(b+i)-loffs;
			if(f<0)f=0;

			j=(int)f; f-=j;
			i0=ss+(j%sm); i1=ss+((j+1)%sm);
			f=samp->pcm[i0]*(1.0-f) + samp->pcm[i1]*f;

//			f=BSWV_SincPolate(samp->pcm, samp->len, f, 64);
			mid_pcmbuft[i*2+0]+=f*lvol;

			f=ssc*(b+i)-roffs;
			if(f<0)f=0;

			j=(int)f; f-=j;
			i0=ss+(j%sm); i1=ss+((j+1)%sm);
			f=samp->pcm[i0]*(1.0-f) + samp->pcm[i1]*f;

//			f=BSWV_SincPolate(samp->pcm, samp->len, f, 64);
			mid_pcmbuft[i*2+1]+=f*rvol;
		}
#endif

		return(0);
	}
#endif

	for(i=0; i<len; i++)
	{
		f=ssc*(b+i)-loffs;
		if(f<0)f=0;

//		j=(int)f; f-=j;
//		i0=ss+(j%sm); i1=ss+((j+1)%sm);
//		f=samp->pcm[i0]*(1.0-f) + samp->pcm[i1]*f;

		f=BSWV_SplinePolateLoop(samp->pcm+ss, sm, f);
//		f=BSWV_SincPolate(samp->pcm, samp->len, f, 64);
		mid_pcmbuft[i*2+0]+=f*lvol;

		f=ssc*(b+i)-roffs;
		if(f<0)f=0;

//		j=(int)f; f-=j;
//		i0=ss+(j%sm); i1=ss+((j+1)%sm);
//		f=samp->pcm[i0]*(1.0-f) + samp->pcm[i1]*f;

		f=BSWV_SplinePolateLoop(samp->pcm+ss, sm, f);
//		f=BSWV_SincPolate(samp->pcm, samp->len, f, 64);
		mid_pcmbuft[i*2+1]+=f*rvol;
	}

	return(0);
}

int MID_BlendNoteSampNoLoop(int chan, int len, int note, Sample *samp)
{
	float vol, lvol, rvol, freq, pan, bal, pbl;
	float loffs, roffs;
	float f, g, ssc;
	int i, j, k, b;
	int i0, i1;

	vol=mid_control[chan][7];
	bal=mid_control[chan][8];
	pan=mid_control[chan][10];

	vol=pow(vol/16384.0, 1.66)*16384.0;

//	vol=16384;

	f=pan/16384.0;
	lvol=1.0-f; rvol=f;
	g=sqrt(lvol*lvol + rvol*rvol);
	lvol=vol*lvol*(1.0/g);
	rvol=vol*rvol*(1.0/g);

	f=bal/16384.0;
	lvol*=1.0-f;
	rvol*=f;

	f=pan;
	loffs=f; roffs=1.0-f;
	g=sqrt(loffs*loffs + roffs*roffs);
	loffs=loffs*(1.0/g);
	roffs=roffs*(1.0/g);


	b=(mid_pcmbufe-mid_pcmbuf)/2;

	if(note>=0)
	{
		freq=pow(2, (note-69)/12.0)*440;
		pbl=(mid_control[chan][31]-8192)/8192.0;
		freq*=pow(2, pbl);
		b-=mid_note_strt[chan][note];
	}else
	{
		freq=samp->root;
		b-=mid_note_strt[chan][0];
	}

	ssc=(samp->rate/44100.0)/samp->root;
	ssc*=freq;

	lvol/=32767.0;
	rvol/=32767.0;

	loffs=loffs*0.0010*samp->rate;
	roffs=roffs*0.0010*samp->rate;

	for(i=0; i<len; i++)
	{
#if 0
		f=ssc*(b+i);

		j=(int)f; f-=j;
		i0=j; i1=j+1;
		if(i1>=samp->len)break;
		f=samp->pcm[i0]*(1.0-f) + samp->pcm[i1]*f;

		mid_pcmbuft[i*2+0]+=f*lvol;
		mid_pcmbuft[i*2+1]+=f*rvol;
#endif

		f=ssc*(b+i);
		if(f>=samp->len)break;

		f=ssc*(b+i)-loffs;
		f=BSWV_SplinePolate(samp->pcm, samp->len, f);
		mid_pcmbuft[i*2+0]+=f*lvol;

		f=ssc*(b+i)-roffs;
		f=BSWV_SplinePolate(samp->pcm, samp->len, f);
		mid_pcmbuft[i*2+1]+=f*rvol;

	}

	if(i<len)
	{
		if(note>=0)
			mid_notevel[chan][note]=0;
		else	mid_notevel[chan][0]=0;
	}

	return(0);
}

int MID_BlendNote(int chan, int len, int note)
{
	Sample *samp, *scur;
	float freq, pbl;
	float d, bd, d0, d1;

	freq=pow(2, (note-69)/12.0)*440;

	pbl=(mid_control[chan][31]-8192)/8192.0;
	freq*=pow(2, pbl);

	samp=mid_samples[mid_prog[chan]];
	while(samp)
	{
		if((freq>=samp->low) && (freq<=samp->high))
			break;
		samp=samp->next;
	}

	if(!samp)
	{
//		samp=mid_samples[mid_prog[chan]];
//		bd=fabs(freq-samp->root);
//		scur=samp;

		scur=mid_samples[mid_prog[chan]];
		bd=999999.0;
		while(scur)
		{
//			d=fabs(freq-scur->root);
//			d=fabs(log2(freq)-log2(scur->root));

			d0=fabs(log2(freq)-log2(scur->low));
			d1=fabs(log2(freq)-log2(scur->high));
			d=(d0<d1)?d0:d1;

			if(d<bd) { samp=scur; bd=d; }
			scur=scur->next;
		}

//		printf("Samp %f %f %f  %f\n", samp->low, samp->high, samp->root, freq);
	}

//	if(!samp)samp=mid_samples[mid_prog[chan]];
//	if(!samp)return(0);

//	samp=NULL;

	if(!samp)
	{
		MID_BlendNoteTone(chan, len, note);
		return(0);
	}

//	printf("Samp %f %f %f  %f\n", samp->low, samp->high, samp->root, freq);

	if(samp->flags&MID_SFL_LOOP)
		MID_BlendNoteSampLoop(chan, len, note, samp);
		else MID_BlendNoteSampNoLoop(chan, len, note, samp);
}

int MID_BlendNoteDrum(int chan, int len, int note)
{
	Sample *samp, *scur;

	if(!mid_drumsamples[note])
		mid_drumsamples[note]=MID_LoadPatch(mid_drumpatches[note]);

	samp=mid_drumsamples[note];
//	if(!samp)printf("Bad Drum %d\n", note);

	if(!samp)return(0);

//	printf("Drum %d (%s)\n", note, mid_drumpatches[note]);

//	return(0);

	mid_note_strt[chan][0]=
		mid_note_strt[chan][note];

	mid_notevel[chan][0]=1;

	if(samp->flags&MID_SFL_LOOP)
		MID_BlendNoteSampLoop(chan, len, -1, samp);
		else MID_BlendNoteSampNoLoop(chan, len, -1, samp);

	if(!mid_notevel[chan][0])
		mid_notevel[chan][note]=0;
	mid_notevel[chan][0]=0;
}

int MID_BlendChannel(int chan, int samp)
{
	int i, j;

	if(chan==9)
	{
//		printf("Drum\n");

		for(i=0; i<128; i++)
		{
			if(mid_notevel[chan][i])
				MID_BlendNoteDrum(chan, samp, i);
		}
		return(0);
	}

//	for(i=0; i<128; i++)
//		if(mid_notemask[chan][i>>3]&(~(1<<(i&7))))
//			MID_BlendNote(chan, samp, i);

	for(i=0; i<128; i++)
	{
		if(mid_notevel[chan][i])
			MID_BlendNote(chan, samp, i);
	}
}

int MID_DeltaTime(int step)
{
	int i, j, k;

	if(!step)return(0);

	//pitch=69+12*log2(freq/440)
	//freq=2^((pitch-69)/12)*440

	j=step*mid_ticksamp;
//	memset(mid_pcmbuft, 0, 2*4*j);

	if(j>131072)
	{
		if(step<2)return(0);

		i=step/2;
		MID_DeltaTime(i);
		MID_DeltaTime(step-i);
		return(0);
	}

#if 0
	if(j>=1000000)
	{
		printf("Sanity: %d>1000000 mix samples\n", j);
		mid_pcmbufe+=2*j;
		return(0);
	}
#endif

	for(i=0; i<j; i++)
	{
		mid_pcmbuft[i*2+0]=0;
		mid_pcmbuft[i*2+1]=0;
	}

	for(i=0; i<16; i++)
		MID_BlendChannel(i, j);

	for(i=0; i<j; i++)
	{
		k=mid_pcmbuft[i*2+0];
		k+=mid_pcmbufe[i*2+0];
		if(k>32767)k=32767;
		if(k<-32767)k=-32767;
		mid_pcmbufe[i*2+0]=k;

		k=mid_pcmbuft[i*2+1];
		k+=mid_pcmbufe[i*2+1];
		if(k>32767)k=32767;
		if(k<-32767)k=-32767;
		mid_pcmbufe[i*2+1]=k;
	}

#if 1
	if(mid_pcmbufe>mid_pcmbuf)
	{
		k=(mid_pcmbufe[-2] + mid_pcmbufe[0])*0.5;
		for(i=0; i<16; i++)
		{
			mid_pcmbufe[-(i*2+2)]=
				mid_pcmbufe[-(i*2+2)]*((i+1)/17.0) +
				k*((16-i)/17.0);
			mid_pcmbufe[i*2+0]=
				mid_pcmbufe[i*2+0]*((i+1)/17.0) +
				k*((16-i)/17.0);
		}

		k=(mid_pcmbufe[-1] + mid_pcmbufe[1])*0.5;
		for(i=0; i<16; i++)
		{
			mid_pcmbufe[-(i*2+1)]=
				mid_pcmbufe[-(i*2+1)]*((i+1)/17.0) +
				k*((16-i)/17.0);
			mid_pcmbufe[i*2+1]=
				mid_pcmbufe[i*2+1]*((i+1)/17.0) +
				k*((16-i)/17.0);
		}
	}
#endif

#if 0
	if(mid_pcmbufe>mid_pcmbuf)
	{
		k=(mid_pcmbufe[-2] + mid_pcmbufe[0])*0.5;
		mid_pcmbufe[0]=k;

		k=(mid_pcmbufe[-1] + mid_pcmbufe[1])*0.5;
		mid_pcmbufe[1]=k;
	}
#endif

	mid_pcmbufe+=2*j;
}

int MID_NoteOn(int chan, int note, int vel)
{
	if(vel==0)
	{
		MID_NoteOff(chan, note, vel);
		return(0);
	}

//	if(vel<32)printf("Weak Note: %d %d %d\n", chan, note, vel);

	mid_notevel[chan][note]=vel;
	mid_note_strt[chan][note]=(mid_pcmbufe-mid_pcmbuf)/2;
	mid_note_end[chan][note]=0;
	return(0);
}

int MID_NoteOff(int chan, int note, int vel)
{
//	mid_notevel[chan][note]=0;
	mid_note_end[chan][note]=(mid_pcmbufe-mid_pcmbuf)/2;
	return(0);
}

int MID_NoteAftertouch(int chan, int note, int val)
{
}

int MID_Controller(int chan, int num, int val)
{
	if((num!=7) && (num!=8) && (num!=10))
//	if(1)
	{
		printf("Controller Ch=%d Num=%d Val=%d\n", chan, num, val);
	}

	if((num&0x60)==0x00)
	{
		mid_control[chan][num&0x1F]&=0x007F;
		mid_control[chan][num&0x1F]|=(val<<7);
		return(0);
	}
	if((num&0x60)==0x20)
	{
		mid_control[chan][num&0x1F]&=0x3F80;
		mid_control[chan][num&0x1F]|=val;
		return(0);
	}
}

int MID_ProgramChange(int chan, int num)
{
//	if(num==0)num=2;

	if(!mid_samples[num])
		mid_samples[num]=MID_LoadPatch(mid_patches[num]);
	mid_prog[chan]=num;
}

int MID_ChannelAftertouch(int chan, int val)
{
}

int MID_PitchBlend(int chan, int val)
{
	mid_control[chan][31]=val;
}

int main(int argc, char *argv[])
{
	byte tb[256];
	FILE *fd;
	char *ofn;
	int lv, dt, tg, d0, d1, op, ch, cdt;
	int i, j, k, stp, tend, noteticks;

	mid_ratesc=1;
//	mid_ratesc=0.25;

#if 0
	for(i=0; i<128; i++)
	{
		if(!mid_samples[i])
			mid_samples[i]=MID_LoadPatch(mid_patches[i]);
		MID_StorePatchWav(mid_patches[i], mid_samples[i]);
	}

	for(i=0; i<128; i++)
	{
		if(!mid_drumsamples[i])
			mid_drumsamples[i]=MID_LoadPatch(mid_drumpatches[i]);
		MID_StorePatchWav(mid_drumpatches[i], mid_drumsamples[i]);
	}

	return(0);
#endif

	ofn="out.pcm";
	if(argc>=3)ofn=argv[2];

	if(argc<2)return(-1);
	fd=fopen(argv[1], "rb");
	if(!fd)return(-1);

	i=MID_ReadS32(fd);
	j=MID_ReadS32(fd);
	printf("%08X %d\n", i, j);

	i=MID_ReadS16(fd);
	j=MID_ReadS16(fd);
	k=MID_ReadS16(fd);
	printf("Type=%d Trk=%d TimeStep=%d (1/%d s)\n", i, j, k, k*4);

	noteticks=k;
//	mid_ticksamp=11025.0/k;
	mid_ticksamp=44100.0/k;
	mid_ticksamp*=mid_ratesc;

	mid_pcmbuf=bgbmid_malloc(1<<27);
	mid_pcmbufe=mid_pcmbuf;

	mid_pcmbuft=bgbmid_malloc(1<<20);

	memset(mid_pcmbuf, 0, 1<<27);

	while(!feof(fd))
	{
		i=MID_ReadS32(fd);
		j=MID_ReadS32(fd);
		printf("%08X %d\n", i, j);

		if(i!=0x4D54726B)
		{
			printf("Bad Track Tag\n");
			break;
		}

		tend=ftell(fd)+j;

		MID_InitChannels();
		mid_pcmbufe=mid_pcmbuf;

		stp=65536; cdt=0;
		while(1 && (stp--))
		{
			if(feof(fd))break;
			if(ftell(fd)>=tend)break;

			dt=MID_ReadDelta(fd);
			i=fgetc(fd);

			MID_DeltaTime(dt);
			cdt+=dt;

			if(i==0xFF)
			{
//				printf("DT=%d Tag=%02X\n", dt, i);

				i=fgetc(fd);
				j=MID_ReadDelta(fd);
//				printf("\tMeta %d, Sz=%d\n", i, j);
				switch(i)
				{
					case 1:
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
					case 7:
						fread(tb, 1, j, fd);
						tb[j]=0;
//						printf("\tStr=%s\n", tb);
						break;

					case 47:
						fseek(fd, j, 1);
						stp=0;
						break;

					case 81:
						fread(tb, 1, j, fd);
						k=(tb[0]<<16)|(tb[1]<<8)|tb[2];
//						mid_ticksamp=11025.0*(k/24000000.0);
						mid_ticksamp=44100.0*(k/1000000.0)/noteticks;
						mid_ticksamp*=mid_ratesc;
						printf("Ticksamp %d %f\n", k, mid_ticksamp);
						break;

					default:
						fseek(fd, j, 1);
						break;
				}

				continue;
			}

			if(i==0xF0)
			{
				while(1)
				{
					i=fgetc(fd);
					if(i&0x80)break;
				}
				continue;
			}

			if((i&0xF0)==0xF0)
			{
				printf("DT=%d Tag=%02X\n", dt, i);
				break;
			}

//			printf("@%d DT=%d Tag=%02X\n", cdt, dt, i);

			if(i&0x80)
			{
				tg=i;
				op=(tg>>4); ch=tg&15;
				d0=fgetc(fd);
				if((op!=12) && (op!=13))
					d1=fgetc(fd);
					else d1=0;
				lv=tg;
			}else
			{
				tg=lv;
				op=(tg>>4); ch=tg&15;
				d0=i;
//				d1=fgetc(fd);
				if((op!=12) && (op!=13))
					d1=fgetc(fd);
					else d1=0;
			}

//			printf("\tOp=%01X Ch=%01X D0=%02X D1=%02X\n",
//				op, ch, d0, d1);

			switch(op)
			{
			case 8:
//				printf("Note_Off @%d Ch=%d Note=%d Vel=%d\n", cdt, ch, d0, d1);
				MID_NoteOff(ch, d0, d1);
				break;
			case 9:
//				printf("Note_On @%d Ch=%d Note=%d Vel=%d\n", cdt, ch, d0, d1);
				MID_NoteOn(ch, d0, d1);
				break;
			case 10:
				printf("Note_Aftertouch @%d Ch=%d Note=%d Val=%d\n", cdt, ch, d0, d1);
				MID_NoteAftertouch(ch, d0, d1);
				break;
			case 11:
//				printf("Controller @%d Ch=%d Num=%d Val=%d\n", cdt, ch, d0, d1);
				MID_Controller(ch, d0, d1);
				break;
			case 12:
				printf("Program_Change @%d Ch=%d Num=%d\n", cdt, ch, d0);
				MID_ProgramChange(ch, d0);
				break;
			case 13:
				printf("Channel_Aftertouch @%d Ch=%d Val=%d\n", cdt, ch, d0);
				MID_ChannelAftertouch(ch, d0);
				break;
			case 14:
//				printf("Pitch_Blend @%d Ch=%d Val=%d\n", cdt, ch, (d1<<7)|d0);
				MID_PitchBlend(ch, (d1<<7)|d0);
				break;
			}
		}

		fseek(fd, tend, 0);
		if(mid_pcmbufe>mid_pcmend)
			mid_pcmend=mid_pcmbufe;
	}

	fclose(fd);

	i=(mid_pcmend-mid_pcmbuf)/2;
	printf("Mixed %d samples (%f s)\n", i, i/44100.0);

	fd=fopen(ofn, "wb");
	fwrite(mid_pcmbuf, 2*2, i, fd);
	fclose(fd);

	return(0);
}
