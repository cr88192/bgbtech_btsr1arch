/*********************************************************************/
/*																   */
/*  This Program Written by Paul Edwards.							*/
/*  Released to the Public Domain									*/
/*																   */
/*********************************************************************/
/*********************************************************************/
/*																   */
/*  pdptest.c - Test of PDPCLIB									  */
/*																   */
/*********************************************************************/

#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
#if 1
	FILE *fp;
	FILE *fq;
//	char buf[5000];
	size_t x;
	int c;
	time_t tt;
#endif
	int i;

#if 0
	fq = fopen("dd:outfile", "w");
	fgets(buf, sizeof buf, stdin);
	fputs(buf, fq);
	fgets(buf, sizeof buf, stdin);
	fputs(buf, fq);
	fgets(buf, sizeof buf, stdin);
	fputs(buf, fq);
	fclose(fq);
#endif

	printf("hello there\n");
	printf("argc = %d\n" , argc);
	for (i = 0; i < argc; i++)
	{
		printf("arg %d is <%s>\n", i, argv[i]);
	}

#if 0
	for(i=0; i<1000000; i++)
	{
		buf[i&4095]=buf[(i-63)&4095]^buf[(i-127)&4095]^rand();
	}
#endif

#ifdef LOOP
	/* note - we use two newlines because of PDPCLIB's MVS
	   implementation (MACRF=PL) */
	printf("deliberately looping!\n\n");
	for (;;) ;
#endif
	return (0);
#if 0
	fp = fopen("pdptest.in", "r+b");
	if (fp == NULL)
	{
		printf("couldn't open input file\n");
		return (0);
	}
	x = fread(buf, 1, 5, fp);
	buf[x] = '\0';
	printf("x is %d\n", x);
	printf("buf is %s\n", buf);
	fwrite("test\ntest2\n", 1, 11, stdout);
	printf("data is:\n");
	printf("%s\n", buf);
	printf("position is %ld\n", ftell(fp));
	x = fseek(fp, 15, SEEK_SET);
	fputc('Z', fp);
	printf("x is %d\n", x);
	x = fread(buf, 1, 5, fp);
	printf("x is %d\n", x);
	printf("%s\n", buf);
	fclose(fp);
	printf("This is a test of 342 %d!\n", 342);
	printf("Please enter your name\n");
/*	fgets(buf, sizeof buf, stdin);
	printf("hello, %s", buf); */
	printf("is 'C' uppercase? %d\n", isupper('C'));
	printf("is 'C' lowercase? %d\n", islower('C'));
	printf("what is lowercase 'C'? %d\n", tolower('C'));
	tt = time(NULL);
	printf("time is %lu\n", tt);
	fp = fopen("pdptest.in", "r");
	if (fp == NULL)
	{
		printf("failed to open file\n");
		return (0);
	}
	c = getc(fp);
	printf("c is %d\n", c);
	while (fgets(buf, sizeof buf, fp) != NULL)
	{
		if (*buf != ',')
		{
			printf(buf);
		}
	}
	fclose(fp);
	printf("argc is %d\n", argc);
	for (x = 0; x < argc; x++)
	{
		printf("argv[%d] is %s\n", x, argv[x]);
	}
	sprintf(buf, "%.4d-%.2d-%.2d", 51, 41, 31);
	printf("%s\n", buf);
	printf("%-15s%s\n", "hello", "there");
	printf("%-06dJJ\n", 37);
	printf("%2d\n", 57);
	printf("%2d\n", 7);
	printf("%2d\n", -3);
	printf("%d\n", -457);
	printf("should be there %s\n", strstr("hello there", "there"));
#endif
	return (0);
}
