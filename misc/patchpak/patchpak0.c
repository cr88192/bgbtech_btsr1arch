#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tfau_comm.c"
#include "tfau_wave.c"

// #define INSTRUM "instruments/"
#define INSTRUM "instrum2/"

static char *bgbmid_patches[128]=
{
/* 0 */ INSTRUM "acpiano",
/* 1 */ INSTRUM "britepno",
/* 2 */ INSTRUM "synpiano",
/* 3 */ INSTRUM "honky",
/* 4 */ INSTRUM "epiano1",
/* 5 */ INSTRUM "epiano2",
/* 6 */ INSTRUM "hrpschrd",
/* 7 */ INSTRUM "clavinet",
/* 8 */ INSTRUM "celeste",
/* 9 */ INSTRUM "glocken",
/* 10 */ INSTRUM "musicbox",
/* 11 */ INSTRUM "vibes",
/* 12 */ INSTRUM "marimba",
/* 13 */ INSTRUM "xylophon",
/* 14 */ INSTRUM "tubebell",
/* 15 */ INSTRUM "santur",
/* 16 */ INSTRUM "homeorg",
/* 17 */ INSTRUM "percorg",
/* 18 */ INSTRUM "rockorg",
/* 19 */ INSTRUM "church",
/* 20 */ INSTRUM "reedorg",
/* 21 */ INSTRUM "accordn",
/* 22 */ INSTRUM "harmonca",
/* 23 */ INSTRUM "concrtna",
/* 24 */ INSTRUM "nyguitar",
/* 25 */ INSTRUM "acguitar",
/* 26 */ INSTRUM "jazzgtr",
/* 27 */ INSTRUM "cleangtr",
/* 28 */ INSTRUM "mutegtr",
/* 29 */ INSTRUM "odguitar",
/* 30 */ INSTRUM "distgtr",
/* 31 */ INSTRUM "gtrharm",
/* 32 */ INSTRUM "acbass",
/* 33 */ INSTRUM "fngrbass",
/* 34 */ INSTRUM "pickbass",
/* 35 */ INSTRUM "fretless",
/* 36 */ INSTRUM "slapbas1",
/* 37 */ INSTRUM "slapbas2",
/* 38 */ INSTRUM "synbass1",
/* 39 */ INSTRUM "synbass2",
/* 40 */ INSTRUM "violin",
/* 41 */ INSTRUM "viola",
/* 42 */ INSTRUM "cello",
/* 43 */ INSTRUM "contraba",
/* 44 */ INSTRUM "tremstr",
/* 45 */ INSTRUM "pizzcato",
/* 46 */ INSTRUM "harp",
/* 47 */ INSTRUM "timpani",
/* 48 */ INSTRUM "marcato",
/* 49 */ INSTRUM "slowstr",
/* 50 */ INSTRUM "synstr1",
/* 51 */ INSTRUM "synstr2",
/* 52 */ INSTRUM "choir",
/* 53 */ INSTRUM "doo",
/* 54 */ INSTRUM "voices",
/* 55 */ INSTRUM "orchhit",
/* 56 */ INSTRUM "trumpet",
/* 57 */ INSTRUM "trombone",
/* 58 */ INSTRUM "tuba",
/* 59 */ INSTRUM "mutetrum",
/* 60 */ INSTRUM "frenchrn",
/* 61 */ INSTRUM "hitbrass",
/* 62 */ INSTRUM "synbras1",
/* 63 */ INSTRUM "synbras2",
/* 64 */ INSTRUM "sprnosax",
/* 65 */ INSTRUM "altosax",
/* 66 */ INSTRUM "tenorsax",
/* 67 */ INSTRUM "barisax",
/* 68 */ INSTRUM "oboe",
/* 69 */ INSTRUM "englhorn",
/* 70 */ INSTRUM "bassoon",
/* 71 */ INSTRUM "clarinet",
/* 72 */ INSTRUM "piccolo",
/* 73 */ INSTRUM "flute",
/* 74 */ INSTRUM "recorder",
/* 75 */ INSTRUM "woodflut",
/* 76 */ INSTRUM "bottle",
/* 77 */ INSTRUM "shakazul",
/* 78 */ INSTRUM "whistle",
/* 79 */ INSTRUM "ocarina",
/* 80 */ INSTRUM "sqrwave",
/* 81 */ INSTRUM "sawwave",
/* 82 */ INSTRUM "calliope",
/* 83 */ INSTRUM "chiflead",
/* 84 */ INSTRUM "charang",
/* 85 */ INSTRUM "voxlead",
/* 86 */ INSTRUM "lead5th",
/* 87 */ INSTRUM "basslead",
/* 88 */ INSTRUM "fantasia",
/* 89 */ INSTRUM "warmpad",
/* 90 */ INSTRUM "polysyn",
/* 91 */ INSTRUM "ghostie",
/* 92 */ INSTRUM "bowglass",
/* 93 */ INSTRUM "metalpad",
/* 94 */ INSTRUM "halopad",
/* 95 */ INSTRUM "sweeper",
/* 96 */ INSTRUM "aurora",
/* 97 */ INSTRUM "soundtrk",
/* 98 */ INSTRUM "crystal",
/* 99 */ INSTRUM "atmosphr",
/* 100 */ INSTRUM "freshair",
/* 101 */ INSTRUM "unicorn",
/* 102 */ INSTRUM "echovox",
/* 103 */ INSTRUM "startrak",
/* 104 */ INSTRUM "sitar",
/* 105 */ INSTRUM "banjo",
/* 106 */ INSTRUM "shamisen",
/* 107 */ INSTRUM "koto",
/* 108 */ INSTRUM "kalimba",
/* 109 */ INSTRUM "bagpipes",
/* 110 */ INSTRUM "fiddle",
/* 111 */ INSTRUM "shannai",
/* 112 */ INSTRUM "carillon",
/* 113 */ INSTRUM "agogo",
/* 114 */ INSTRUM "steeldrm",
/* 115 */ INSTRUM "woodblk",
/* 116 */ INSTRUM "taiko",
/* 117 */ INSTRUM "toms",
/* 118 */ INSTRUM "syntom",
/* 119 */ INSTRUM "revcym",
/* 120 */ INSTRUM "fx-fret",
/* 121 */ INSTRUM "fx-blow",
/* 122 */ INSTRUM "seashore",
/* 123 */ INSTRUM "jungle",
/* 124 */ INSTRUM "telephon",
/* 125 */ INSTRUM "helicptr",
/* 126 */ INSTRUM "applause",
/* 127 */ INSTRUM "pistol"
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
/* 28 */ INSTRUM "slap",
/* 29 */ INSTRUM "scratch1",
/* 30 */ INSTRUM "scratch2",
/* 31 */ INSTRUM "sticks",
/* 32 */ INSTRUM "sqrclick",
/* 33 */ INSTRUM "metclick",
/* 34 */ INSTRUM "metbell",
/* 35 */ INSTRUM "kick1",
/* 36 */ INSTRUM "kick2",
/* 37 */ INSTRUM "stickrim",
/* 38 */ INSTRUM "snare1",
/* 39 */ INSTRUM "claps",
/* 40 */ INSTRUM "snare2",
/* 41 */ INSTRUM "tomlo2",
/* 42 */ INSTRUM "hihatcl",
/* 43 */ INSTRUM "tomlo1",
/* 44 */ INSTRUM "hihatpd",
/* 45 */ INSTRUM "tommid2",
/* 46 */ INSTRUM "hihatop",
/* 47 */ INSTRUM "tommid1",
/* 48 */ INSTRUM "tomhi2",
/* 49 */ INSTRUM "cymcrsh1",
/* 50 */ INSTRUM "tomhi1",
/* 51 */ INSTRUM "cymride1",
/* 52 */ INSTRUM "cymchina",
/* 53 */ INSTRUM "cymbell",
/* 54 */ INSTRUM "tamborin",
/* 55 */ INSTRUM "cymsplsh",
/* 56 */ INSTRUM "cowbell",
/* 57 */ INSTRUM "cymcrsh2",
/* 58 */ INSTRUM "vibslap",
/* 59 */ INSTRUM "cymride2",
/* 60 */ INSTRUM "bongohi",
/* 61 */ INSTRUM "bongolo",
/* 62 */ INSTRUM "congahi1",
/* 63 */ INSTRUM "congahi2",
/* 64 */ INSTRUM "congalo",
/* 65 */ INSTRUM "timbaleh",
/* 66 */ INSTRUM "timbalel",
/* 67 */ INSTRUM "agogohi",
/* 68 */ INSTRUM "agogolo",
/* 69 */ INSTRUM "cabasa",
/* 70 */ INSTRUM "maracas",
/* 71 */ INSTRUM "whistle1",
/* 72 */ INSTRUM "whistle2",
/* 73 */ INSTRUM "guiro1",
/* 74 */ INSTRUM "guiro2",
/* 75 */ INSTRUM "clave",
/* 76 */ INSTRUM "woodblk1",
/* 77 */ INSTRUM "woodblk2",
/* 78 */ INSTRUM "cuica1",
/* 79 */ INSTRUM "cuica2",
/* 80 */ INSTRUM "triangl1",
/* 81 */ INSTRUM "triangl2",
/* 82 */ INSTRUM "shaker",
/* 83 */ INSTRUM "jingles",
/* 84 */ INSTRUM "belltree",
/* 85 */ INSTRUM "castinet",
/* 86 */ INSTRUM "surdo1",
/* 87 */ INSTRUM "surdo2",

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

s16 *samp_buf[1024];
int samp_len[1024];
u32 samp_chk[1024];
u16 samp_rate[1024];
short samp_chain[1024];
short samp_hash[256];
int n_sample;

int patchidx_inst[1024];
int patchidx_samp[1024];
int patchidx_lbeg[1024];
int patchidx_lend[1024];
int patchidx_flag[1024];

int patchidx_minhz[1024];
int patchidx_maxhz[1024];
int patchidx_nomhz[1024];

int n_patchidx;

int	lump_offs[1024];
int	lump_size[1024];
u64	lump_name[1024];
int n_lump;


u32 checksum_sample(u16 *samp, int len)
{
	int i, j, h0, h1, h;
	
	h0=1; h1=1;
	for(i=0; i<len; i++)
	{
		h0=h0+samp[i];
		h1=h1+h0;
	}
//	h0=(h0^(h0>>16))&0xFFFF;
//	h1=(h1^(h1>>16))&0xFFFF;
	h=h0^h1;
	return(h);
}

int sblk0_enc(int v)
{
	int e, v0, v1;

	if(v<0)
	{
		v1=sblk0_enc(-v);
		return(0x80|v1);
	}
	
	if(v>=256)
	{
		e=0;
		v0=v>>3;
		if(v0&(~31))	{ v0=v0>>1; e++; }
		if(v0&(~31))	{ v0=v0>>1; e++; }
		if(v0&(~31))	{ v0=v0>>1; e++; }
		if(v0&(~31))	{ v0=v0>>1; e++; }
		if(v0&(~31))	{ v0=v0>>1; e++; }
		if(v0&(~31))	{ v0=v0>>1; e++; }
		if(v0&(~31))	{ v0=v0>>1; e++; }

		v1=(e<<4)|(v0&15);
		if(v0&(~31))	{ v1=0x7F; }
	}else
	{
		v1=v>>4;
	}
	
	return(v1);
}

byte *packsamp(s16 *ibuf, int ilen, int rate, int *rsz)
{
	byte *tbuf;
	int i, j, k, l, tg;
	
//	ilen=ilen&(~15);
	
	tbuf=malloc(ilen+64);
	memset(tbuf, 0, ilen+64);
	
	switch(rate)
	{
	case 8000: tg=0;	break;
	case 11025: tg=1;	break;
	case 16000: tg=2;	break;
	case 22050: tg=3;	break;
	case 32000: tg=4;	break;
	case 44100: tg=5;	break;
	case 64000: tg=6;	break;
	case 88200: tg=7;	break;

	case 4000: tg=8;	break;
	case 48000: tg=9;	break;
	case 6000: tg=10;	break;
	case 96000: tg=11;	break;
	}
	
	tbuf[0]='D';
	tbuf[1]='A'+tg;
	tbuf[2]=ilen;
	tbuf[3]=ilen>>8;
	
	l=(ilen+31)&(~15);
	
	k=0;
	for(i=0; i<ilen; i++)
	{
		j=sblk0_enc(ibuf[i]);
//		if(j&0x80)
//			j^=0x7F;
//		tbuf[i+4]=(j-k)&0xFF;
//		tbuf[i+4]=j;
		tbuf[i+16]=j^0x55;
		k=j;
	}

	for(; i<l; i++)
	{
		tbuf[i+16]=j^0x55;
	}
	
//	*rsz=ilen+16;
	*rsz=l+16;
	return(tbuf);
}

int ratereduce(int rate)
{
	int rt1;
	
	switch(rate)
	{
		case 44100:		rt1=32000;	break;
		case 32000:		rt1=22050;	break;
		case 22050:		rt1=16000;	break;
		case 16000:		rt1=11025;	break;
		case 11025:		rt1=8000;	break;
		case 8000:		rt1=6000;	break;
		case 6000:		rt1=4000;	break;
		default:		rt1=rate;	break;
	}
	return(rt1);
}


// #define BASERATE	8000
// #define BASERATE	16000
#define BASERATE	11025

int main()
{
	char tb[256];
	char *s0, *s1;
	byte *imgbuf, *imgct, *pbuf;
	FILE *fd;
	short *sbuf, *sb1;
	int ix, lbeg, lend, lflg;
	double fmin, fmax, fbase;
	int rate, len, hash, h, rt1;
	int i, j, k, l;

	n_sample=1;
	
	for(i=0; i<256; i++)
	{
		if(i<128)
		{
			s0=bgbmid_patches[i];
		}else
		{
			s0=bgbmid_drumpatches[i-128];
		}

		if(!s0)
			continue;
			
		printf("try: %d %s  np=%d\n", i, s0, n_patchidx);
		
		sprintf(tb, "%s_inf.txt", s0);
		fd=fopen(tb, "rb");
		if(!fd)
		{
			printf("fail: %s\n", s0);
			continue;
		}
		
		while(!feof(fd))
		{
			tb[0]=0;
			fgets(tb, 255, fd);
			if(!tb[0] || (tb[0]==';'))
				continue;
			if(tb[0]=='!')
			{
				k=atoi(tb+1);
				if(k<128)
					{ s0=bgbmid_patches[k]; }
				else
					{ s0=bgbmid_drumpatches[k-128]; }
				fclose(fd);

				sprintf(tb, "%s_inf.txt", s0);
				fd=fopen(tb, "rb");
				if(!fd)
				{
					printf("  chain fail %s\n", s0);
					break;
				}
				continue;
			}

			sscanf(tb, "%d %d %d %lf %lf %lf %d",
				&ix, &lbeg, &lend,
				&fmin, &fmax, &fbase, &lflg);
				
			sprintf(tb, "%s_%d.wav", s0, ix);
			
			sbuf=BGBMID_LoadWAV_Stereo16(tb, &rate, &len);
			if(!sbuf)
			{
				printf("fail open patch: %s\n", tb);
				continue;
			}

			rt1=BASERATE;

			k=len*((1.0*rt1)/rate);
//			while(k>8000)
			while(k>7000)
			{
//				rt1=rt1/2;

				j=ratereduce(rt1);
				if(j==rt1)
					break;
				rt1=j;

				k=len*((1.0*rt1)/rate);
			}
//			if(rt1<4000)
//				rt1=4000;
			if(rt1<6000)
				rt1=6000;
			
			if(rate!=rt1)
			{
				k=len*((1.0*rt1)/rate);
				sb1=bgbmid_malloc(k*2*sizeof(short));
				BGBMID_ScaleSampleStereo(sb1, k, sbuf, len);

				lbeg=lbeg*((1.0*rt1)/rate);
				lend=lend*((1.0*rt1)/rate);

				bgbmid_free(sbuf);
				sbuf=sb1;
				len=k;
				rate=rt1;
			}

			hash=checksum_sample(sbuf, len);
			h=(hash^(hash>>8))&255;
			
			j=samp_hash[h];
			while(j)
			{
				if(	(samp_len[j]==len) &&
					(samp_chk[j]==hash))
						break;
				j=samp_chain[j];
			}
			
			if(j)
			{
				bgbmid_free(sbuf);
				sbuf=samp_buf[j];
			}else
			{
				j=n_sample++;
				samp_buf[j]=sbuf;
				samp_len[j]=len;
				samp_chk[j]=hash;
				samp_rate[j]=rate;
				
				samp_chain[j]=samp_hash[h];
				samp_hash[h]=j;
			}
			
			k=n_patchidx++;

			if(fmin<1)
				fmin=1;
			if(fmax>65535)
				fmax=65535;

			patchidx_inst[k]=i;
			patchidx_samp[k]=j;
			patchidx_lbeg[k]=lbeg;
			patchidx_lend[k]=lend;

			patchidx_minhz[k]=floor(fmin);
			patchidx_maxhz[k]=ceil(fmax);
			patchidx_nomhz[k]=fbase;

			patchidx_flag[k]=lflg;
		}
		fclose(fd);
	}
	
	printf("n_sample: %d\n", n_sample);
	printf("n_patchidx: %d\n", n_patchidx);
	
	len=0;
	for(i=0; i<n_sample; i++)
		len+=samp_len[i];
	printf("totlen: %d\n", len);
	
	imgbuf=malloc(len*2);
	imgct=imgbuf;
	
	imgct[0]='I';	imgct[1]='W';
	imgct[2]='A';	imgct[3]='D';
	imgct+=16;

	for(i=0; i<n_sample; i++)
	{
		if(!samp_buf[i])
			continue;

		pbuf=packsamp(samp_buf[i], samp_len[i], samp_rate[i], &k);
		
		sprintf(tb, "SND_%04X", i);
		
		printf("%s %d %d\n", tb, samp_len[i], samp_rate[i]);
		
		j=n_lump++;
		lump_offs[j]=imgct-imgbuf;
		lump_size[j]=k;
		lump_name[j]=*(u64 *)tb;
		
		memcpy(imgct, pbuf, k);
		imgct+=((k+15)&(~15));
		
		free(pbuf);
	}

	for(i=0; i<(n_patchidx-1); i++)
	{
		if(patchidx_inst[i]!=patchidx_inst[i+1])
			continue;
		j=patchidx_maxhz[i+0];
		k=patchidx_minhz[i+1];
		if(j<k)
		{
			j=(j+k)/2;
			patchidx_maxhz[i+0]=j;
			patchidx_minhz[i+1]=j;
		}
	}
	
	pbuf=imgct;
	for(i=0; i<n_patchidx; i++)
	{
		j=patchidx_inst[i];
		imgct[ 0]=j;	imgct[ 1]=j>>8;
		j=patchidx_samp[i];
		imgct[ 2]=j;	imgct[ 3]=j>>8;
		j=patchidx_lbeg[i];
		imgct[ 4]=j;	imgct[ 5]=j>>8;
		j=patchidx_lend[i];
		imgct[ 6]=j;	imgct[ 7]=j>>8;

		j=patchidx_minhz[i];
		imgct[ 8]=j;	imgct[ 9]=j>>8;
		j=patchidx_maxhz[i];
		imgct[10]=j;	imgct[11]=j>>8;
		j=patchidx_nomhz[i];
		imgct[12]=j;	imgct[13]=j>>8;

		j=patchidx_flag[i];
		imgct[14]=j;	imgct[15]=j>>8;
		
		printf("I=%3d S=%04X  Loop=%5d..%5d / %5d  "
				"Hz=%5d..%5d HzA=%5d  Fl=%04X\n",
			patchidx_inst[i],
			patchidx_samp[i],
			patchidx_lbeg[i],
			patchidx_lend[i],
			samp_len[patchidx_samp[i]],
			patchidx_minhz[i],
			patchidx_maxhz[i],
			patchidx_nomhz[i],
			patchidx_flag[i]);
		
		imgct+=16;
	}
	
	l=imgct-pbuf;

	sprintf(tb, "PATCHIDX");
	j=n_lump++;
	lump_offs[j]=pbuf-imgbuf;
	lump_size[j]=l;
	lump_name[j]=*(u64 *)tb;
	
	k=imgct-imgbuf;
	imgbuf[ 4]=n_lump>> 0;
	imgbuf[ 5]=n_lump>> 8;
	imgbuf[ 6]=n_lump>>16;
	imgbuf[ 7]=n_lump>>24;
	imgbuf[ 8]=k>> 0;
	imgbuf[ 9]=k>> 8;
	imgbuf[10]=k>>16;
	imgbuf[11]=k>>24;

	for(i=0; i<n_lump; i++)
	{
		k=lump_offs[i];
		l=lump_size[i];
		imgct[0]=k>> 0;
		imgct[1]=k>> 8;
		imgct[2]=k>>16;
		imgct[3]=k>>24;
		imgct[4]=l>> 0;
		imgct[5]=l>> 8;
		imgct[6]=l>>16;
		imgct[7]=l>>24;
		memcpy(imgct+8, lump_name+i, 8);
		imgct+=16;
	}
	
	fd=fopen("instrum.wd1", "wb");
	fwrite(imgbuf, 1, imgct-imgbuf, fd);
	fclose(fd);
}

