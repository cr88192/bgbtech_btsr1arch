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

#define MID_FCC_MThd	0x4D546864
#define MID_FCC_MTrk	0x4D54726B

typedef unsigned char byte;

short *mid_pcmbuf;
short *mid_pcmbufe;
short *mid_pcmend;
float *mid_pcmbuft;
float mid_ticksamp;
float mid_ratesc;

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

void MID_WriteS16LE(FILE *fd, int v)
{
	fputc(v&0xFF, fd);
	fputc((v>>8)&0xFF, fd);
}

void MID_WriteS32LE(FILE *fd, int v)
{
	fputc(v&0xFF, fd);
	fputc((v>>8)&0xFF, fd);
	fputc((v>>16)&0xFF, fd);
	fputc((v>>24)&0xFF, fd);
}

void MID_WriteS16BE(FILE *fd, int v)
{
	fputc((v>>8)&0xFF, fd);
	fputc(v&0xFF, fd);
}

void MID_WriteS32BE(FILE *fd, int v)
{
	fputc((v>>24)&0xFF, fd);
	fputc((v>>16)&0xFF, fd);
	fputc((v>>8)&0xFF, fd);
	fputc(v&0xFF, fd);
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

int MID_DumpMidi0(char *name, byte *strm, int sz, int nticks)
{
	FILE *fd;

	fd=fopen(name, "wb");
	MID_WriteS32BE(fd, MID_FCC_MThd);
	MID_WriteS32BE(fd, 6);		//head size

	MID_WriteS16BE(fd, 0);		//type
	MID_WriteS16BE(fd, 1);		//tracks
	MID_WriteS16BE(fd, nticks);	//note ticks (per quarter-note)

	MID_WriteS32BE(fd, MID_FCC_MTrk);	//track FCC
	MID_WriteS32BE(fd, sz);			//track size
	fwrite(strm, 1, sz, fd);
//	MID_WriteS32BE(fd, 0x00FF2F00);

	fclose(fd);
	return(0);
}

int MID_DumpMidi1(char *name, byte **strm, int *sz, int ntrk, int nticks)
{
	FILE *fd;
	int i;

	if(ntrk==1)
	{
		i=MID_DumpMidi0(name, strm[0], sz[0], nticks);
		return(i);
	}

	fd=fopen(name, "wb");
	MID_WriteS32BE(fd, MID_FCC_MThd);
	MID_WriteS32BE(fd, 6);		//head size

	MID_WriteS16BE(fd, 1);		//type
	MID_WriteS16BE(fd, ntrk);	//tracks
	MID_WriteS16BE(fd, nticks);	//note ticks (per quarter-note)

	for(i=0; i<ntrk; i++)
	{
		MID_WriteS32BE(fd, MID_FCC_MTrk);	//track FCC
		MID_WriteS32BE(fd, sz[i]);			//track size
		fwrite(strm[i], 1, sz[i], fd);
	}

	fclose(fd);
	return(0);
}

int MID_ReadMidi(char *name, byte **strm, int *sz, int *rntrk, int *rnticks)
{
	FILE *fd;
	int i, j, k, n;

	fd=fopen(name, "rb");

	i=MID_ReadS32(fd);
	j=MID_ReadS32(fd);
//	printf("%08X %d\n", i, j);

	if((i!=MID_FCC_MThd) || (j!=6))
	{
		fclose(fd);
		return(-1);
	}

	i=MID_ReadS16(fd);
	j=MID_ReadS16(fd);
	k=MID_ReadS16(fd);
//	printf("Type=%d Trk=%d TimeStep=%d (1/%d s)\n", i, j, k, k*4);

	*rntrk=j;
	*rnticks=k;

	n=j;

	for(i=0; i<n; i++)
		{ strm[i]=NULL; sz[i]=0; }

	for(i=0; i<n; i++)
	{
		j=MID_ReadS32(fd);
		k=MID_ReadS32(fd);

		if(j!=MID_FCC_MTrk)
			break;

		strm[i]=malloc(k);
		sz[i]=k;

		fread(strm[i], 1, k, fd);
	}

	fclose(fd);
	return(0);
}

int MID_DecVLI(byte **rcs)
{
	byte *cs;
	int i, j;

	cs=*rcs;
	i=*cs++; j=0;
	while(i&0x80) { j=(j<<7)|i&0x7F; i=*cs++; }
	j=(j<<7)|i&0x7F;
	*rcs=cs;
	return(j);
}

int MID_EncVLI(byte **rct, int val)
{
	byte *ct;
	int i, j;

	ct=*rct;

	if(val>=2097152)*ct++=0x80|((val>>21)&0x7F);
	if(val>=16384)*ct++=0x80|((val>>14)&0x7F);
	if(val>=128)*ct++=0x80|((val>>7)&0x7F);
	*ct++=val&0x7F;

	*rct=ct;
	return(0);
}

int MID_ReadMidiMerge(char *name, byte **rstrm, int *rsz, int *rnticks)
{
	byte *trkb[64];
	int szb[64];
	byte *trkcs[64];
	byte *trkce[64];
	byte trkclt[64];
	byte trksysx[64];
	int trkcdt[64];
	byte *buf, *ct, *cs;
	int ntrk, nticks, cdt, bdt, btrk;
	int ctag, ltag;
	int i, j, k;

	i=MID_ReadMidi(name, trkb, szb, &ntrk, &nticks);
	if(i<0)return(-1);

	if(ntrk==1)
	{
		*rstrm=trkb[0];
		*rsz=szb[0];
		*rnticks=nticks;
		return(0);
	}

	for(i=0; i<ntrk; i++)
	{
		trkcs[i]=trkb[i];
		trkce[i]=trkb[i]+szb[i];
		trkcdt[i]=0;
		trkclt[i]=0;
		trksysx[i]=0;
	}

	buf=malloc(1<<20);
	ct=buf; cdt=0; ltag=-1;

	while(1)
	{
		bdt=999999; btrk=-1;
		for(i=0; i<ntrk; i++)
		{
			if(trkcs[i]>=trkce[i])
				continue;

			cs=trkcs[i];
			j=MID_DecVLI(&cs);
			k=trkcdt[i]+j;

			if(k<bdt) { bdt=k; btrk=i; }
		}
		if(btrk<0)break;

		cs=trkcs[btrk];
		MID_DecVLI(&cs);

		if((cs[0]==0xFF) && (cs[1]==0x2F))
		{
			cs+=2;
			i=MID_DecVLI(&cs);
			cs+=i;
			trkcs[btrk]=cs;
			trkcdt[btrk]=bdt;
			continue;
		}

#if 0
		if((cs[0]!=0xFF) && ((cs[0]&0xF0)==0xF0))
		{
		}
#endif

#if 0
		if((cs[0]!=0xFF) && ((cs[0]&0xF0)==0xF0))
		{
			switch(cs[0])
			{
			case 0xF0:
				cs++;
				cs++;
				while(!((*cs)&0x80))
					cs++;
				if(*cs==0xF7)cs++;
//				trksysx[btrk]=1;
				break;
			case 0xF1: case 0xF3:
				cs++;
				cs++;
				break;
			case 0xF2:
				cs++;
				cs++;
				break;
			case 0xF7:
//				trksysx[btrk]=0;
				break;
			default:
				cs++;
				break;
			}
			trkcs[btrk]=cs;
			trkcdt[btrk]=bdt;
			continue;
		}
#endif

		MID_EncVLI(&ct, bdt-cdt);

		if(cs[0]==0xFF)
		{
			i=*cs++;
			j=*cs++;
			k=MID_DecVLI(&cs);

			*ct++=i;
			*ct++=j;
			MID_EncVLI(&ct, k);

			memcpy(ct, cs, k);
			cs+=k; ct+=k;

			cdt=bdt; ltag=-1;
			trkcs[btrk]=cs;
			trkcdt[btrk]=bdt;
			continue;
		}

		if((cs[0]&0xF0)==0xF0)
		{
			switch(cs[0])
			{
			case 0xF0:
				*ct++=*cs++;
				*ct++=*cs++;
				while(!(cs[0]&0x80))
					*ct++=*cs++;
				if(*cs==0xF7)*ct++=*cs++;
				break;
			case 0xF1: case 0xF3:
				*ct++=*cs++;
				*ct++=*cs++;
				break;
			case 0xF2:
				*ct++=*cs++;
				*ct++=*cs++;
				break;
			default:
				*ct++=*cs++;
				break;
			}
			cdt=bdt; ltag=-1;
			trkcs[btrk]=cs;
			trkcdt[btrk]=bdt;
			continue;
		}

		if(cs[0]&0x80)
		{
			ctag=*cs++;
			trkclt[btrk]=ctag;
		}else
		{
			ctag=trkclt[btrk];
		}

		if(ctag!=ltag)
		{
			*ct++=ctag;
			ltag=ctag;
		}

		if(((ctag&0xF0)==0xC0) || ((ctag&0xF0)==0xD0))
		{
			*ct++=*cs++;
		}else
		{
			*ct++=*cs++;
			*ct++=*cs++;
		}

		cdt=bdt;
		trkcs[btrk]=cs;
		trkcdt[btrk]=bdt;
	}

	*ct++=0x00;
	*ct++=0xFF;
	*ct++=0x2F;
	*ct++=0x00;

	i=ct-buf;
	buf=realloc(buf, i);

	*rstrm=buf;
	*rsz=i;
	*rnticks=nticks;
	return(0);
}

int main(int argc, char *argv[])
{
	byte tb[256];
	FILE *fd;
	byte *buf, *cs, *ce;
	char *ifn, *ofn;
	int lv, dt, tg, d0, d1, op, ch, sz;
	int i, j, k, stp, tend, noteticks, ntrk;

	mid_ratesc=1;
	ifn=NULL; ofn=NULL;

	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "-rt"))
			{
				mid_ratesc=atof(argv[i+1]);
				i++;
				continue;
			}

			continue;
		}

		if(!ifn) { ifn=argv[i]; continue; }
		if(!ofn) { ofn=argv[i]; continue; }
	}

	if(!ifn)return(-1);
	if(!ofn)ofn="out.mid";

//	if(argc>=3)ofn=argv[2];
//	if(argc<2)return(-1);
//	ifn=argv[1];

	MID_ReadMidiMerge(ifn, &buf, &sz, &noteticks);

#if 1
	cs=buf; ce=buf+sz; lv=0;
	while(cs<ce)
	{
		MID_DecVLI(&cs);

		if((cs[0]==0xFF) && (cs[1]==0x51) && (cs[2]==0x03))
		{
			cs+=3;
			k=(cs[0]<<16)|(cs[1]<<8)|cs[2];
			k*=mid_ratesc;
			if(k>17777216)k=16777216;

			cs[0]=(k>>16)&0xFF;
			cs[1]=(k>>8)&0xFF;
			cs[2]=(k>>0)&0xFF;

			cs+=3;

			continue;
		}

		if(cs[0]==0xFF)
		{
			cs+=2;
			i=MID_DecVLI(&cs);
			cs+=i;
			continue;
		}

		if((cs[0]&0xF0)==0xF0)
			break;

		if(cs[0]&0x80)
		{
			tg=*cs++;
			lv=tg;
		}else
		{
			tg=lv;
		}

		if((tg&0xF0)==0xC0)
			{ cs++; continue; }
		if((tg&0xF0)==0xD0)
			{ cs++; continue; }
		cs+=2;
	}
#endif

	MID_DumpMidi0(ofn, buf, sz, noteticks);

	return(0);
}
