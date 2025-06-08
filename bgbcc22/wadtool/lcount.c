#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int total, file, i;
	FILE *fd, *lfd;
	char buf[1024], lfb[256];
	char *s;

	if(argc<2)
	{
		fd=stdin;

		file=0;
		while(fgets(buf, 1023, fd))file++;
		printf("%d\n", file);

		return(0);
	}

	if(!strcmp(argv[1], "-l"))
	{
		lfd=NULL;
		if(!strcmp(argv[2], "-"))
			lfd=stdin;
		if(!lfd)lfd=fopen(argv[2], "rt");
		if(!lfd)
		{
			printf("%s: can't open list %s\n", argv[0], argv[2]);
			return(-1);
		}


		total=0;
		while(!feof(lfd))
		{
			memset(lfb, 0, 256);
			fgets(lfb, 255, lfd);

			s=lfb;
			while(*s)
			{
				if(*s>' ')break;
				s++;
			}
			if(!*s)continue;

			while(*s>' ')s++;
			*s=0;

//			printf("%s\n", lfb);

			file=0;
			fd=fopen(lfb, "rt");
			if(!fd)continue;

			while(fgets(buf, 1023, fd))file++;
			fclose(fd);
//			printf("%s %d\n", lfb, file);
			total+=file;
		}
		printf("total %d\n", total);

		return(0);
	}

	total=0;
	for(i=1; i<argc; i++)
	{
		file=0;
		fd=fopen(argv[i], "rt");
		if(!fd)continue;
		while(fgets(buf, 1023, fd))file++;
		fclose(fd);
		printf("%s %d\n", argv[i], file);
		total+=file;
	}
	printf("total %d in %d files\n", total, argc-1);

	return(0);
}
