#include <stdio.h>

int main(int argc, char *argv[])
{
	char *s0;
	int i, j;
	printf("Simple Shell Command\n");
	printf("Called as %s, argc=%d\n", argv[0], argc);
	
	for(i=1; i<argc; i++)
	{
		s0=argv[i];
		if(!s0)
			continue;
		printf(" argv[%d]=%s\n", i, s0);
	}
	return(0);
}
