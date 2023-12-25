#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int tool_cat(int argc, char **argv)
{
	char *fna[256];
	FILE *fd;
	int i, c, nfn;

	nfn=0;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			continue;
		}
		
		fna[nfn++]=argv[i];
	}

	if(nfn)
	{
		for(i=0; i<nfn; i++)
		{
			fd=fopen(fna[i], "rb");
			if(!fd)
				continue;
			while(!feof(fd))
			{
				c=fgetc(fd);
				if(c<0)
					break;
				fputc(c, stdout);
			}
			fclose(fd);
		}
	}else
	{
		while(!feof(stdin))
		{
			c=fgetc(stdin);
			if(c<0)
				break;
			fputc(c, stdout);
		}
	}
}

int main(int argc, char *argv[])
{
	char *arg0, *arg0cs;
	
	arg0=argv[0];
	arg0cs=arg0+strlen(arg0);
	while((arg0cs>arg0) && (*arg0cs!='/'))
		arg0cs--;
	if(*arg0cs=='/')
		arg0cs++;
	
	switch(*arg0cs)
	{
	case 'c':
		if(!strcmp(arg0cs, "cat"))
			return(tool_cat(argc, argv));
		break;
	default:
		break;
	}
}
