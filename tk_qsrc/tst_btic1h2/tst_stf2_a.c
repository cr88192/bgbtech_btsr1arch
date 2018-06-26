#include <stdio.h>

int main()
{
	int i0, i1;
	int i, j, k;
	
	printf("Swap:\n");
	for(i=0; i<16; i++)
	{
		printf("%01X: ", i);
		for(j=0; j<16; j++)
		{
			i0=i*16+j;
			i1=(i0*7)>>3;
			printf("%02X ", i1);
		}
		printf("\n");
	}

	printf("Swap dist to 0:\n");
	for(i=0; i<16; i++)
	{
		printf("%01X: ", i);
		for(j=0; j<16; j++)
		{
			i0=i*16+j;
			i1=(i0*7)>>3;
			k=0;
			
			while(i1>0)
				{ k++; i1=(i1*7)>>3; }
			
			printf("%02X ", k);
		}
		printf("\n");
	}
}