#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include <bgbmid.h>

int bgbmid_extwav(char *str)
{
	char *s;

	s=str+strlen(str);
	while((s>str) && (*s!='.'))s--;

	if(!strcmp(s, ".wav"))return(1);
	if(!strcmp(s, ".WAV"))return(1);
	return(0);
}

int bmid_filter_1bpp(short *ibp, byte *obp, int len)
{
	int dc;
	int i, j, k;
	memset(obp, 0, (len+31)/8);
	
	dc=0;
	for(i=0; i<len; i++)
	{
		j=ibp[i];
//		k=j>dc;
		if(j>dc)
			{ obp[i>>3]|=0x80>>(i&7); }
//		dc=(31*dc+j)>>5;
		dc=(63*dc+j)>>6;
	}
	return(0);
}

int bmid_filter_2bpp(short *ibp, byte *obp, int len)
{
	int dc;
	int i, j, k;
	memset(obp, 0, (len+31)/4);
	
	dc=0;
	for(i=0; i<len; i++)
	{
		j=ibp[i];
//		k=j>dc;
//		if(j>dc)
//			{ obp[i>>3]|=0x80>>(i&7); }

		if(j>dc)
		{
			k=2;
			if(k>16)k=3;
		}else
		{
			k=1;
			if(k<(-16))k=0;
		}

		obp[i>>2]|=k<<(6-((i&3)*2));

//		dc=(31*dc+j)>>5;
		dc=(63*dc+j)>>6;
	}
	return(0);
}

int main(int argc, char *argv[])
{
	char *ifn[16384];
	char tb[256], tb2[256];
	BGBMID_PackHead hdr;
	BGBMID_PackDirEnt *dir;
	FILE *ofd, *ifd, *ofd2;
	byte *buf;
	char *ofn, *lifn;
	char *ldir;
	short *bp;
	char **a;
	char *s, *t;
	int i, j, k, l, n;
	int ch, rt, len, flags, fl, nifn, cmd;

	ofn=NULL; ldir=NULL; lifn=NULL;
	nifn=0; flags=0; cmd=0;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h"))
			{
				cmd=8;
				continue;
			}

			if(!strcmp(argv[i], "-o"))
			{
				ofn=argv[i+1];
				i++;
				continue;
			}

			if(!strcmp(argv[i], "-L"))
			{
				ldir=argv[i+1];
				i++;
				continue;
			}

			if(!strcmp(argv[i], "-l"))
			{
				lifn=argv[i+1];
				i++;
				continue;
			}

			printf("%s: unrecognized argument '%s'\n", argv[0], argv[i]);
			continue;
		}else
		{
			ifn[nifn++]=argv[i];
		}
	}

//	if(!ofn || (cmd==8))
//	{
//		help(argv[0]);
//		return(0);
//	}

	if(lifn)
	{
		ifd=fopen(lifn, "rt");
		if(ifd)
		{
			while(!feof(ifd))
			{
				fgets(tb, 255, ifd);
				a=gcrsplit(tb);
				if(!a[0])continue;
				if(*a[0]=='#')continue;
				if(*a[0]=='/')continue;
				ifn[nifn++]=a[0];
			}
			fclose(ifd);
		}
	}

	buf=malloc(1<<16);

	ofd=NULL;
	if(ofn)
		{ ofd=fopen(ofn, "wt"); }

	printf("List: %d\n", nifn);

	n=0;
	for(i=0; i<nifn; i++)
	{
		if(ldir)
		{
			sprintf(tb2, "%s/%s", ldir, ifn[i]);
		}else
		{
			strcpy(tb2, ifn[i]);
		}
	
//		strcpy(tb, argv[i]);
		strcpy(tb, ifn[i]);

		if(!bgbmid_extwav(tb2))
		{
			printf("Reject: %s\n", tb2);
			continue;
		}

		printf("Process: %s\n", tb2);

		s=tb+strlen(tb);
		while((s>tb) && (*s!='.'))s--;
//		strcpy(s, ".btac");
		*s=0;

//		bp=BGBMID_LoadWAV_Mono16(tb2, &rt, &len);
//		bp=BGBMID_LoadWAV16(tb2, &ch, &rt, &len);
		bp=BGBMID_LoadWAV_8Mono16(tb2, &len);
		if(!bp)continue;
			
		bmid_filter_1bpp(bp, buf, len);
		bgbmid_free(bp);
		
		sprintf(tb2, "%s_1bpp.txt", tb);
		ofd2=fopen(tb2, "wt");

#if 1
		l=0;
		for(j=0; j<((len+31)/32); j++)
		{
			k=	(buf[j*4+0]<<24)|
				(buf[j*4+1]<<16)|
				(buf[j*4+2]<< 8)|
				(buf[j*4+3]    );
			if(((u32)k)==0x00000000UL)
				continue;
			if(((u32)k)==0xFFFFFFFFUL)
				continue;
			fprintf(ofd2, "0x%08X ", k);
			l++;
			if(!(l&3))fprintf(ofd2, "\n");
		}
#endif

		fclose(ofd2);

		if(ofd && (len<4000))
		{
			s=tb; t=tb2;
			while(*s)
			{
				if((*s>='a') && (*s<='z'))
					{ *t++=*s++; continue; }
				if((*s>='A') && (*s<='Z'))
					{ *t++=*s++; continue; }
				if((*s>='0') && (*s<='9'))
					{ *t++=*s++; continue; }
				s++; *t++='_';
			}
			*t++=0;
			
#if 1
			fprintf(ofd, "u32 %s[]={\n", tb2);
			l=0;
			for(j=0; j<((len+31)/32); j++)
			{
				k=	(buf[j*4+0]<<24)|
					(buf[j*4+1]<<16)|
					(buf[j*4+2]<< 8)|
					(buf[j*4+3]    );
				if(((u32)k)==0x00000000UL)
					continue;
				if(((u32)k)==0xFFFFFFFFUL)
					continue;
				fprintf(ofd, "0x%08X, ", k);
				l++;
				if(!(l&3))fprintf(ofd, "\n");
			}
			fprintf(ofd, "};\n");
			fprintf(ofd, "int %s_length=%d;\n\n", tb2, l);
#endif
		}

		bp=malloc(len*sizeof(short));

#if 1
		l=0;
		for(j=0; j<len; j++)
		{
			k=((j>>3)/4)*4;
			k=(buf[k+0]<<24)|(buf[k+1]<<16)|(buf[k+2]<<8)|(buf[k+3]);
			if(((u32)k)==0x00000000UL)
				continue;
			if(((u32)k)==0xFFFFFFFFUL)
				continue;

			k=(buf[j>>3]&(0x80>>(j&7)))?4096:-4096;
			bp[l++]=k;
		}
#endif

		if(!l)continue;

		sprintf(tb2, "%s_1bpp.wav", tb);
//		BGBMID_StoreWAV(tb2, bp, 1, 8000, 16, len);
		BGBMID_StoreWAV(tb2, bp, 1, 8000, 16, l);
		free(bp);
	}
	
	if(ofd)
		{ fclose(ofd); }
}
