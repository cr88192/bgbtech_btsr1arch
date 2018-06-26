/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  This program reads from an input file and writes to an output    */
/*  file.                                                            */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char buf[6144]; /* arbitrary buffer size */

int main(int argc, char **argv)
{
    FILE *fp;
    FILE *fq;
    char *in_name;
    char *out_name;
    int c;
    int off = 0;
    char *in = "r";
    char *out = "w";
    unsigned long total = 0;
    
    if (argc < 3)
    {
        printf("usage: copyfile [-bb/-tt/-tb/-bt] <infile> <outfile>\n");
        printf("default is text to text copy\n");
        return (EXIT_FAILURE);
    }

    if (argc > 3)
    {
        if (argv[1][0] == '-')
        {
            if ((argv[1][1] == 'b') || (argv[1][1] == 'B'))
            {
                in = "rb";
            }
            if ((argv[1][2] == 'b') || (argv[1][2] == 'B'))
            {
                out = "wb";
            }
            off++;
        }
    }
    in_name = *(argv + off + 1);
    fp = fopen(in_name, in);
    if (fp == NULL)
    {
        printf("failed to open %s for reading\n", in_name);
        return (EXIT_FAILURE);
    }

    out_name = *(argv + off + 2);
    fq = fopen(out_name, out);
    if (fq == NULL)
    {
        printf("failed to open %s for writing\n", out_name);
        return (EXIT_FAILURE);
    }

    printf("copying from file %s, mode %s\n",
           in_name,
           (strlen(in) == 1) ? "text" : "binary");
        
    printf("to file %s, mode %s\n",
           out_name,
           (strlen(out) == 1) ? "text" : "binary");
        
    while ((c = fread(buf, 1, sizeof buf, fp)) > 0)
    {
        total += c;
        fwrite(buf, 1, c, fq);
        if (ferror(fq)) break;
    }

    if (ferror(fp) || ferror(fq))
    {
        printf("i/o error\n");
        return (EXIT_FAILURE);
    }
    printf("%lu bytes copied\n", total);
    
    return (0);
}
