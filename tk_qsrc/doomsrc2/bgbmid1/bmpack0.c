#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include <bgbmid.h>

static int bgbmid_fdcopy(FILE *ifd, FILE *ofd)
{
	char buf[256];
	int i;

	printf("bgbmid_fdcopy\n");

	while(!feof(ifd))
	{
		i=fread(buf, 1, 256, ifd);
		fwrite(buf, 1, i, ofd);
	}
}

int bgbmid_extwav(char *str)
{
	char *s;

	s=str+strlen(str);
	while((s>str) && (*s!='.'))s--;

	if(!strcmp(s, ".wav"))return(1);
	if(!strcmp(s, ".WAV"))return(1);
	return(0);
}

int help(char *arg0)
{
	printf("Usage: %s <opts*> -o <pack> [-L <dir>] <files*>\n", arg0);
	printf("-o <name>		Indicates output PACK fike\n");
	printf("-L <dir>		Indicates base-directory for input files\n");
	printf("--splitstereo	Store audio in split-stereo format\n");
	printf("--centermono	Store audio in center-mono format\n");
	printf("--blk256		Use 256-sample blocks (176kbps SQ)\n");
	printf("--blk256lq		Use 256-sample blocks (88kbps LQ)\n");
	printf("--blk256hq		Use 256-sample blocks (352kbps HQ)\n");
	return(0);
}

int main(int argc, char *argv[])
{
	char *ifn[16384];
	char tb[256], tb2[256];
	BGBMID_PackHead hdr;
	BGBMID_PackDirEnt *dir;
	FILE *ofd, *ifd;
	byte *buf;
	char *ofn, *lifn;
	char *ldir;
	short *bp;
	char **a;
	char *s;
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

			if(!strcmp(argv[i], "--splitstereo"))
			{
				flags|=BGBMID_BTAC_EFL_HALFMONO;
				continue;
			}

			if(!strcmp(argv[i], "--centermono"))
			{
				flags|=BGBMID_BTAC_EFL_CENTERMONO;
				continue;
			}

			if(!strcmp(argv[i], "--encode"))
			{
				cmd=1;
				continue;
			}

			if(!strcmp(argv[i], "--decode"))
			{
				cmd=2;
				continue;
			}

			if(!strcmp(argv[i], "--blk256"))
			{
				flags|=BGBMID_BTAC_EBMODE_BLK256;
				continue;
			}

			if(!strcmp(argv[i], "--blk256lq"))
			{
				flags|=BGBMID_BTAC_EBMODE_BLK256LQ;
				continue;
			}

			if(!strcmp(argv[i], "--blk256hq"))
			{
				flags|=BGBMID_BTAC_EBMODE_BLK256HQ;
				continue;
			}

			printf("%s: unrecognized argument '%s'\n", argv[0], argv[i]);
			continue;
		}else
		{
			ifn[nifn++]=argv[i];
		}
	}

	if(!ofn || (cmd==8))
	{
		help(argv[0]);
		return(0);
	}

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

	ofd=fopen(ofn, "wb");

	fwrite(&hdr, 1, 12, ofd);

	dir=malloc((1<<16)*sizeof(BGBMID_PackDirEnt));
	buf=malloc(1<<24);

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

		if(bgbmid_extwav(tb2))
		{
			printf("WAV %s\n", tb2);

			s=tb+strlen(tb);
			while((s>tb) && (*s!='.'))s--;
			strcpy(s, ".btac");

//			bp=BGBMID_LoadWAV_Mono16(ifn[i], &rt, &len);
			bp=BGBMID_LoadWAV16(tb2, &ch, &rt, &len);
			if(!bp)continue;
			
			fl=flags;
			
//			k=SGWZ_Encode(bp, rt, len, buf);
			k=BGBMID_BTAC_Encode(bp, ch, rt, len, fl, buf, 1<<24);

			j=ftell(ofd);
			fwrite(buf, 1, k, ofd);

			bgbmid_free(bp);
		}else
		{
			printf("RAW %s\n", tb2);
			ifd=fopen(tb2, "rb");
			if(!ifd)continue;

			j=ftell(ofd);
			bgbmid_fdcopy(ifd, ofd);
			k=ftell(ofd)-j;

			fclose(ifd);
		}

		if(strlen(tb)>=56)
		{
			l=ftell(ofd);
			fwrite(tb, 1, strlen(tb)+1, ofd);
			sprintf(tb, "@@0x%08X", l);
		}

		strcpy(dir[n].name, tb);
		*(int *)(dir[n].offs)=j;
		*(int *)(dir[n].size)=k;
		n++;

		printf("%-56s %10d %10d\n", tb, j, k);
	}

	j=ftell(ofd);
	fwrite(dir, 64, n, ofd);

	*(int *)(hdr.offs)=j;
	*(int *)(hdr.ents)=n;

	hdr.fcc[0]='P';
	hdr.fcc[1]='A';
	hdr.fcc[2]='C';
	hdr.fcc[3]='K';

	fseek(ofd, 0, 0);
	fwrite(&hdr, 1, 12, ofd);
	fclose(ofd);
}
