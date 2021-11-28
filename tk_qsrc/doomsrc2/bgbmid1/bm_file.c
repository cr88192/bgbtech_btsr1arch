#include "bgbmid.h"

BMID_API int BGBMID_ReadS16BE(void *fd)
{
	short i;
	i=bgbmid_fgetc(fd)<<8;
	i|=bgbmid_fgetc(fd);
	return(i);
}

BMID_API int BGBMID_ReadS32BE(void *fd)
{
	int i;
	i=bgbmid_fgetc(fd)<<24;
	i|=bgbmid_fgetc(fd)<<16;
	i|=bgbmid_fgetc(fd)<<8;
	i|=bgbmid_fgetc(fd);
	return(i);
}

BMID_API void BGBMID_WriteS16BE(void *fd, int v)
{
	bgbmid_fputc((v>>8)&0xFF, fd);
	bgbmid_fputc(v&0xFF, fd);
}

BMID_API void BGBMID_WriteS32BE(void *fd, int v)
{
	bgbmid_fputc((v>>24)&0xFF, fd);
	bgbmid_fputc((v>>16)&0xFF, fd);
	bgbmid_fputc((v>>8)&0xFF, fd);
	bgbmid_fputc(v&0xFF, fd);
}

BMID_API int BGBMID_ReadDelta(void *fd)
{
	int i, j;

	i=0;
	j=bgbmid_fgetc(fd);
	while(j>=0x80)
	{
		i=(i<<7)|(j&0x7F);
		j=bgbmid_fgetc(fd);
	}
	i=(i<<7)|(j&0x7F);
	return(i);
}

BMID_API int BGBMID_DumpMidi0(char *name, byte *strm, int sz, int nticks)
{
	void *fd;

	fd=bgbmid_fopen(name, "wb");
	BGBMID_WriteS32BE(fd, BGBMID_FCC_MThd);
	BGBMID_WriteS32BE(fd, 6);		//head size

	BGBMID_WriteS16BE(fd, 0);		//type
	BGBMID_WriteS16BE(fd, 1);		//tracks
	BGBMID_WriteS16BE(fd, nticks);	//note ticks (per quarter-note)

	BGBMID_WriteS32BE(fd, BGBMID_FCC_MTrk);	//track FCC
	BGBMID_WriteS32BE(fd, sz);			//track size
	bgbmid_fwrite(strm, 1, sz, fd);
//	BGBMID_WriteS32BE(fd, 0x00FF2F00);

	bgbmid_fclose(fd);
	return(0);
}

BMID_API int BGBMID_DumpMidi1(char *name, byte **strm, int *sz, int ntrk, int nticks)
{
	void *fd;
	int i;

	if(ntrk==1)
	{
		i=BGBMID_DumpMidi0(name, strm[0], sz[0], nticks);
		return(i);
	}

	fd=bgbmid_fopen(name, "wb");
	BGBMID_WriteS32BE(fd, BGBMID_FCC_MThd);
	BGBMID_WriteS32BE(fd, 6);		//head size

	BGBMID_WriteS16BE(fd, 1);		//type
	BGBMID_WriteS16BE(fd, ntrk);	//tracks
	BGBMID_WriteS16BE(fd, nticks);	//note ticks (per quarter-note)

	for(i=0; i<ntrk; i++)
	{
		BGBMID_WriteS32BE(fd, BGBMID_FCC_MTrk);	//track FCC
		BGBMID_WriteS32BE(fd, sz[i]);			//track size
		bgbmid_fwrite(strm[i], 1, sz[i], fd);
	}

	bgbmid_fclose(fd);
	return(0);
}

BMID_API int BGBMID_ReadMidi(char *name, byte **strm, int *sz, int *rntrk, int *rnticks)
{
	void *fd;
	int i, j, k, n;

	fd=bgbmid_fopen(name, "rb");
	if(!fd)
	{
		return(-1);
	}

	i=BGBMID_ReadS32BE(fd);
	j=BGBMID_ReadS32BE(fd);
//	printf("%08X %d\n", i, j);

	if((i!=BGBMID_FCC_MThd) || (j!=6))
	{
		bgbmid_fclose(fd);
		return(-1);
	}

	i=BGBMID_ReadS16BE(fd);
	j=BGBMID_ReadS16BE(fd);
	k=BGBMID_ReadS16BE(fd);
//	printf("Type=%d Trk=%d TimeStep=%d (1/%d s)\n", i, j, k, k*4);

	*rntrk=j;
	*rnticks=k;

	n=j;

	for(i=0; i<n; i++)
		{ strm[i]=NULL; sz[i]=0; }

	for(i=0; i<n; i++)
	{
		j=BGBMID_ReadS32BE(fd);
		k=BGBMID_ReadS32BE(fd);

		if(j!=BGBMID_FCC_MTrk)
			break;

		strm[i]=bgbmid_malloc(k);
		sz[i]=k;

		bgbmid_fread(strm[i], 1, k, fd);
	}

	bgbmid_fclose(fd);
	return(0);
}

BMID_API int BGBMID_DecVLI(byte **rcs)
{
	byte *cs;
	int i, j;

	cs=*rcs;
	i=*cs++; j=0;
	while(i&0x80) { j=(j<<7)|(i&0x7F); i=*cs++; }
	j=(j<<7)|(i&0x7F);
	*rcs=cs;
	return(j);
}

BMID_API int BGBMID_EncVLI(byte **rct, int val)
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

BMID_API int BGBMID_TryDecVLI(byte **rcs, byte *ce)
{
	byte *cs;
	int i, j;

	cs=*rcs;
	if(cs>=ce)return(-1);
	i=*cs++; j=0;
	while(i&0x80)
	{
		if(cs>=ce)return(-1);
		j=(j<<7)|(i&0x7F); i=*cs++;
	}
	j=(j<<7)|(i&0x7F);
	*rcs=cs;
	return(j);
}

BMID_API int BGBMID_TryDecSVLI(byte **rcs, byte *ce)
{
	int i;
	i=BGBMID_TryDecVLI(rcs, ce);
	return((i&1)?(-((i+1)>>1)):(i>>1));
}

BMID_API int BGBMID_ReadMidiMerge(char *name, byte **rstrm, int *rsz, int *rnticks)
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

	i=BGBMID_ReadMidi(name, trkb, szb, &ntrk, &nticks);
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

	buf=bgbmid_malloc(1<<20);
	ct=buf; cdt=0; ltag=-1;

	while(1)
	{
		bdt=999999; btrk=-1;
		for(i=0; i<ntrk; i++)
		{
			if(trkcs[i]>=trkce[i])
				continue;

			cs=trkcs[i];
			j=BGBMID_DecVLI(&cs);
			k=trkcdt[i]+j;

			if(k<bdt) { bdt=k; btrk=i; }
		}
		if(btrk<0)break;

		cs=trkcs[btrk];
		BGBMID_DecVLI(&cs);

		if((cs[0]==0xFF) && (cs[1]==0x2F))
		{
			cs+=2;
			i=BGBMID_DecVLI(&cs);
			cs+=i;
			trkcs[btrk]=cs;
			trkcdt[btrk]=bdt;
			continue;
		}

		BGBMID_EncVLI(&ct, bdt-cdt);

		if(cs[0]==0xFF)
		{
			i=*cs++;
			j=*cs++;
			k=BGBMID_DecVLI(&cs);

			*ct++=i;
			*ct++=j;
			BGBMID_EncVLI(&ct, k);

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
	buf=bgbmid_realloc(buf, i);

	*rstrm=buf;
	*rsz=i;
	*rnticks=nticks;
	return(0);
}


BMID_API int BGBMID_Midi2Ascii(char *buf, byte *strm, int sz, int nticks)
{
	byte *cs, *ce;
	char *t, *ln;
	int ntrk, cdt, dt;
	int ctag, ltag, ch, d0, d1;
	int i, j, k, l;

	t=buf; ln=t;

	sprintf(t, "RT%d", nticks); t+=strlen(t);

	cs=strm; ce=cs+sz;
	cdt=0; ltag=-1;
	while(cs<ce)
	{
		dt=BGBMID_DecVLI(&cs);
		if(dt)
		{
			if(t>ln) { *t++=' '; *t=0; }
			sprintf(t, "T%d", dt); t+=strlen(t);
			if((t-ln)>=56) { *t++='\n'; *t=0; ln=t; }
		}

		cdt+=dt;

		if((cs[0]==0xFF) && (cs[1]==0x2F))
		{
			cs+=2;
			i=BGBMID_DecVLI(&cs);
			cs+=i;
			ltag=-1;
			continue;
		}

		if(cs[0]==0xFF)
		{
			i=*cs++;
			j=*cs++;
			k=BGBMID_DecVLI(&cs);

			if(j==47)
			{
				if(t>ln) { *t++=' '; *t=0; }
				sprintf(t, "RS"); t+=strlen(t);
				if((t-ln)>=56) { *t++='\n'; *t=0; ln=t; }
			}

			if(j==81)
			{
				l=(cs[0]<<16)|(cs[1]<<8)|cs[2];

				if(t>ln) { *t++=' '; *t=0; }
				sprintf(t, "RN%d", l); t+=strlen(t);
				if((t-ln)>=56) { *t++='\n'; *t=0; ln=t; }
			}

//			*ct++=i; *ct++=j;
//			BGBMID_EncVLI(&ct, k);
//			memcpy(ct, cs, k);
//			cs+=k; ct+=k;

			cs+=k;

			ltag=-1;
			continue;
		}

#if 1
		if((cs[0]&0xF0)==0xF0)
		{
			switch(cs[0])
			{
			case 0xF0:
				cs+=2;
				while(!(cs[0]&0x80))cs++;
				if(*cs==0xF7)cs++;
				break;
			case 0xF1: case 0xF3: cs+=2; break;
			case 0xF2: cs+=2; break;
			default: cs++; break;
			}
			ltag=-1;
			continue;
		}
#endif

#if 0
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
			ltag=-1;
			continue;
		}
#endif

		if(cs[0]&0x80)
		{
			ctag=*cs++;
			ltag=ctag;
		}else
		{
			ctag=ltag;
		}

		if(((ctag&0xF0)==0xC0) || ((ctag&0xF0)==0xD0))
			{ d0=*cs++; }
		else { d0=*cs++; d1=*cs++; }
		ch=ctag&15;

		if(t>ln) { *t++=' '; *t=0; }

		switch(ctag&0xF0)
		{
		case 0x80: sprintf(t, "M%d,%d,%d", ch+1, d0, d1); break;
		case 0x90: sprintf(t, "N%d,%d,%d", ch+1, d0, d1); break;
		case 0xA0: sprintf(t, "A%d,%d,%d", ch+1, d0, d1); break;
		case 0xB0: sprintf(t, "B%d,%d,%d", ch+1, d0, d1); break;
		case 0xC0: sprintf(t, "C%d,%d", ch+1, d0); break;
		case 0xD0: sprintf(t, "D%d,%d", ch+1, d0); break;
		case 0xE0: sprintf(t, "E%d,%d", ch+1, (d1<<7)+d0); break;
		default: break;
		}

		t+=strlen(t);
		if((t-ln)>=56) { *t++='\n'; *t=0; ln=t; }
	}

	*t++=0;
	return(0);
}

int bgbmid_readnum(char **str)
{
	char *s;
	int i;

	s=*str; i=0;
	while((*s>='0') && (*s<='9'))
		i=(i*10)+((*s++)-'0');
	*str=s;
	return(i);
}

BMID_API int BGBMID_Ascii2Midi(byte *buf,
	char *strm, int *rsz, int *rnticks)
{
	byte *t;
	char *s;
	int i, j, k, dt;

	if(rnticks)*rnticks=120;

	s=strm; t=buf; dt=0;
	while(*s)
	{
		while(*s && (*s<=' '))s++;
		if(!*s)break;

		if(*s=='T')
		{
			s++;
			dt+=bgbmid_readnum(&s);
			continue;
		}

		switch(*s++)
		{
		case 'R':
			switch(*s++)
			{
			case 'T':
				i=0;
				while((*s>='0') && (*s<='9'))
					i=(i*10)+((*s++)-'0');
				if(rnticks)*rnticks=i;
				break;

			case 'N':
				i=0;
				while((*s>='0') && (*s<='9'))
					i=(i*10)+((*s++)-'0');

				BGBMID_EncVLI(&t, dt);
				*t++=0xFF; *t++=0x51; *t++=0x03;
				*t++=(i>>16)&0xFF;
				*t++=(i>>8)&0xFF;
				*t++=i&0xFF;

				break;

			case 'z':
				BGBMID_EncVLI(&t, dt);
				*t++=0x80|9; *t++=0; *t++=0;
				break;
			default: break;
			}
			break;

		case 'S':
			switch(*s++)
			{
			case 'B':
				i=bgbmid_readnum(&s)-1;
				if(*s==',')s++; j=bgbmid_readnum(&s);
				BGBMID_EncVLI(&t, dt);
				*t++=0xB0|i; *t++=0; *t++=j>>7;
				BGBMID_EncVLI(&t, 0);
				*t++=32+0; *t++=j&127;
				break;

			case 'v':
				i=bgbmid_readnum(&s)-1;
				if(*s==',')s++; j=bgbmid_readnum(&s);
				BGBMID_EncVLI(&t, dt);
				*t++=0xB0|i; *t++=7; *t++=j>>7;
				BGBMID_EncVLI(&t, 0);
				*t++=32+7; *t++=j&127;
				break;
			case 'b':
				i=bgbmid_readnum(&s)-1;
				if(*s==',')s++; j=bgbmid_readnum(&s);
				BGBMID_EncVLI(&t, dt);
				*t++=0xB0|i; *t++=8; *t++=j>>7;
				BGBMID_EncVLI(&t, 0);
				*t++=32+8; *t++=j&127;
				break;
			case 'p':
				i=bgbmid_readnum(&s)-1;
				if(*s==',')s++; j=bgbmid_readnum(&s);
				BGBMID_EncVLI(&t, dt);
				*t++=0xB0|i; *t++=10; *t++=j>>7;
				BGBMID_EncVLI(&t, 0);
				*t++=32+10; *t++=j&127;
				break;

			default: break;
			}
			break;

		case 'M':
			i=bgbmid_readnum(&s)-1;
			if(i<0)i=0;
			if(*s==',')s++; j=bgbmid_readnum(&s);
			if(*s==',')s++; k=bgbmid_readnum(&s);
			BGBMID_EncVLI(&t, dt);
			*t++=0x80|i; *t++=j; *t++=k;
			break;
		case 'N':
			i=bgbmid_readnum(&s)-1;
			if(i<0)i=0;
			if(*s==',')s++; j=bgbmid_readnum(&s);
			if(*s==',')s++; k=bgbmid_readnum(&s);
			BGBMID_EncVLI(&t, dt);
			*t++=0x90|i; *t++=j; *t++=k;
			break;
		case 'A':
			i=bgbmid_readnum(&s)-1;
			if(i<0)i=0;
			if(*s==',')s++; j=bgbmid_readnum(&s);
			if(*s==',')s++; k=bgbmid_readnum(&s);
			BGBMID_EncVLI(&t, dt);
			*t++=0xA0|i; *t++=j; *t++=k;
			break;
		case 'B':
			i=bgbmid_readnum(&s)-1;
			if(i<0)i=0;
			if(*s==',')s++; j=bgbmid_readnum(&s);
			if(*s==',')s++; k=bgbmid_readnum(&s);
			BGBMID_EncVLI(&t, dt);
			*t++=0xB0|i; *t++=j; *t++=k;
			break;
		case 'C':
			i=bgbmid_readnum(&s)-1;
			if(i<0)i=0;
			if(*s==',')s++; j=bgbmid_readnum(&s);
			BGBMID_EncVLI(&t, dt);
			*t++=0xC0|i; *t++=j;
			break;
		case 'D':
			i=bgbmid_readnum(&s)-1;
			if(i<0)i=0;
			if(*s==',')s++; j=bgbmid_readnum(&s);
			BGBMID_EncVLI(&t, dt);
			*t++=0xD0|i; *t++=j;
			break;
		case 'E':
			i=bgbmid_readnum(&s)-1;
			if(i<0)i=0;
			if(*s==',')s++; j=bgbmid_readnum(&s);
			BGBMID_EncVLI(&t, dt);
			*t++=0xE0|i; *t++=j&127; *t++=(j>>7)&127;
			break;
		}
		dt=0;
	}

	if(dt)
	{
		BGBMID_EncVLI(&t, dt);
		*t++=0x80|9; *t++=0; *t++=0;
		dt=0;
	}

	if(rsz)*rsz=t-buf;
	return(0);
}
