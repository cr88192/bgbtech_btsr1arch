#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main()
{
	FILE *fd;
	unsigned int ui;
	int i, j, k;

	fd=fopen("genmidi.lmp", "rb");
	while(!feof(fd))
	{
		for(i=0; i<4; i++)
		{
			fread(&ui, 1, 4, fd);
			printf("0x%08X, ", ui);
		}
		printf("\n");
	}
	fclose(fd);
}