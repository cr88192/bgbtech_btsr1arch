/* Run this to test out "fread" on binary files,
   for small reads */

#include <stdio.h>

static char buf[80];

int main(void)
{
    FILE *fp;
    long x;

    fp = fopen("temp.in", "rb");
    if (fp == NULL)
    {
        printf("can't open temp.in\n");
        return (0);
    }

    x = 0;
    while (fread(buf, sizeof buf, 1, fp) == 1)
    {
        x++;
    }
    fclose(fp);
    printf("x is %ld\n", x);
    return (0);
}
