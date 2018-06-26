#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char *argv[])
{
	double a, f;
	int i, j, k;
	
	for(i=0; i<128; i++)
	{
		if(i && !(i&7))
			printf("\n");
		a=i*(6.283185307179586476925286766559/128);
		f=sin(a);
		printf("0x%04X, ", ((int)(f*32767))&65535);
	}
}
