/* Run this to test out "fgets" */

#include <stdio.h>

int main(void)
{
    char buf[100];
    FILE *fp;
    long x;

    fp = fopen("temp.in", "r");
    if (fp == NULL)
    {
        printf("can't open temp.in\n");
        return (0);
    }

    x = 0;
    while (fgets(buf, sizeof buf, fp) != NULL)
    {
        x++;
    }
    fclose(fp);
    printf("x is %ld\n", x);
    return (0);
}
