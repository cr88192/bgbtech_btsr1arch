#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char byte;

int spliteq(char *str, char *buf0, char *buf1)
{
	char *s, *t;
	s=str; t=buf0;
	while(*s && *s!='=')
		{ *t++=*s++; }
	*t++=0;
	if(*s=='=')s++;
	t=buf1;
	while(*s)
		{ *t++=*s++; }
	*t++=0;
	return(0);
}

byte *loadfile(char *fn, int *rsz)
{
	FILE *fd;
	byte *buf;
	int sz;

	fd=fopen(fn, "rb");
	if(!fd)
		return(NULL);
	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	buf=malloc(sz+64);
	memset(buf, 0, sz+64);
	fread(buf, 1, sz, fd);
	*rsz=sz;
	return(buf);
}

int cify(char *ibuf, char *obuf)
{
	char *s, *t;
	int c;

	s=ibuf; t=obuf;
	while(*s)
	{
		c=*s++;
		if(c=='\n')
		{
			*t++='\\';
			*t++='n';
			continue;
		}
		if(c=='\t')
		{
			*t++='\\';
			*t++='t';
			continue;
		}
		if((c<' ') || (c>'~'))
		{
			sprintf(t, "\\x%02X", c);
			t+=4;
			continue;
		}

		if((c=='\"') || (c=='\\'))
			*t++='\\';
		*t++=c;
	}
	*t++=0;
	return(0);
}

int main(int argc, char *argv[])
{
	char tb0[256], tb1[256];
	char *sns[32];
	char *fns[32];
	char *ofn;
	FILE *ofd;
	byte *fbuf[32];
	byte *buf;
	int n, n1, sz;
	int i, j, k;

	n=0; ofn=NULL;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "-o"))
			{
				ofn=argv[i+1];
				i++;
				continue;
			}
			continue;
		}

		spliteq(argv[i], tb0, tb1);
		sns[n]=strdup(tb0);
		fns[n]=strdup(tb1);
		n++;
	}

	if(!ofn)
	{
		printf("no output file\n");
		return(-1);
	}

	if(!n)
	{
		printf("no input names");
		return(-1);
	}

	ofd=fopen(ofn, "wt");
	if(!ofd)
	{
		printf("fail open %s\n", ofn);
		return(-1);
	}
	for(i=0; i<n; i++)
	{
		buf=loadfile(fns[i], &sz);
		if(!buf)
			continue;
		fprintf(ofd, "char *%s[]={\n", sns[i]);
		n1=((sz+31)/32);
		for(j=0; j<n1; j++)
		{
			memcpy(tb0, buf+(j*32), 32);
			tb0[32]=0;
			cify(tb0, tb1);
			fprintf(ofd, "\"%s\",\n", tb1);

		}
		fprintf(ofd, "(char *)0};\n\n");
	}
	fclose(ofd);
	return(0);
}
