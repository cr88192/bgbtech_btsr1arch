/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  memtest.c - Test of ATL memory                                   */
/*                                                                   */
/*  This program does a quick test of 2 large blocks of memory,      */
/*  one of 20 MB and one of 9 MB, and displays the address and       */
/*  checksum of the blocks. The checksum should be exactly as        */
/*  shown below (on an EBCDIC machine) but the addresses will vary   */
/*  depending on the operating system.                               */
/*                                                                   */
/*  addr 4118360 approx 65 MB location                               */
/*  checksum is -54967290                                            */
/*  addr 542B0B0 approx 84 MB location                               */
/*  checksum is 1908000006                                           */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define BSIZE 20000000
#define BSIZE2 9000000

int main(int argc, char **argv)
{
    char *p;
    int x;
    int i;
    
    x = 0;
    p = malloc(BSIZE);
    if (p == NULL)
    {
        printf("out of memory\n");
        return (EXIT_FAILURE);
    }
    printf("addr %p approx %d MB location\n",
           p, ((int)p)/1024/1024);
    memset(p, 'M', BSIZE);
    memcpy(p, "ABC", 3);
    memcpy(p + BSIZE - 3 - 1, "XYZ", 3);
    for (i = 0; i < BSIZE; i++)
    {
        x += p[i];
    }
    printf("checksum is %d\n", x);

    x = 0;
    p = malloc(BSIZE2);
    if (p == NULL)
    {
        printf("out of memory\n");
        return (EXIT_FAILURE);
    }
    printf("addr %p approx %d MB location\n",
           p, ((int)p)/1024/1024);
    memset(p, 'M', BSIZE2);
    memcpy(p, "ABC", 3);
    memcpy(p + BSIZE2 - 3 - 1, "XYZ", 3);
    for (i = 0; i < BSIZE2; i++)
    {
        x += p[i];
    }
    printf("checksum is %d\n", x);
    return (0);
}
