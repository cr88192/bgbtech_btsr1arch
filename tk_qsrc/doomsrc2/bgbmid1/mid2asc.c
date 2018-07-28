#include <bgbmid.h>

int main(int argc, char *argv[])
{
	byte tb[256];
	FILE *fd;
	BGBMID_Context *ctx;
	short *obuf;
	byte *buf, *cs, *ce;
	char *ifn, *ofn, *str;
	float ratesc;
	int lv, dt, tg, d0, d1, op, ch, sz;
	int i, j, k, noteticks, ntrk;

	ratesc=1;
	ifn=NULL; ofn=NULL;

	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "-rt"))
			{
				ratesc=atof(argv[i+1]);
				i++;
				continue;
			}

			continue;
		}

		if(!ifn) { ifn=argv[i]; continue; }
		if(!ofn) { ofn=argv[i]; continue; }
	}

	if(!ifn)return(-1);
	if(!ofn)ofn="out.txt";

	BGBMID_ReadMidiMerge(ifn, &buf, &sz, &noteticks);

	str=malloc(1<<20);
	BGBMID_Midi2Ascii(str, buf, sz, noteticks);

	fd=fopen(ofn, "wt");
	fputs(str, fd);
	fclose(fd);

	return(0);
}
