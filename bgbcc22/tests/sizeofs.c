#include <stdio.h>

int main()
{
	printf("sizeof(char)=%d\n", (int) sizeof(char));
	printf("sizeof(short)=%d\n", (int) sizeof(short));
	printf("sizeof(int)=%d\n", (int) sizeof(int));
	printf("sizeof(long)=%d\n", (int) sizeof(long));
	printf("sizeof(long long)=%d\n", (int) sizeof(long long));

	printf("sizeof(unsigned char)=%d\n", (int) sizeof(unsigned char));
	printf("sizeof(unsigned short)=%d\n", (int) sizeof(unsigned short));
	printf("sizeof(unsigned int)=%d\n", (int) sizeof(unsigned int));
	printf("sizeof(unsigned long)=%d\n", (int) sizeof(unsigned long));
	printf("sizeof(unsigned long long)=%d\n", (int) sizeof(unsigned long long));
	return(0);
}