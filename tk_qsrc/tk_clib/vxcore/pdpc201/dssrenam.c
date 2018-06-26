/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  dssrenam - fix dfdss dumps from another site to have names that  */
/*  are within this site's allowable high level qualifiers           */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char buf[40000];

#define MLEN 44 /* length of MVS dataset name */

int main(int argc, char **argv)
{
    unsigned char lll[4];
    FILE *fp;
    FILE *fq;
    char *from;
    char *to;
    int diff;
    int reclen;
    int flen;
    int numrecs = 0;

    if (argc != 4)
    {
        printf("usage: dssrenam <in file> <out file> <substitution>\n");
        printf("e.g. dssrenam dd:in dd:out SED./SCOTT.SED.\n");
        printf("or dssrenam dd:in dd:out /SCOTT.\n");
        printf("or dssrenam dd:in dd:out SCOTT./\n");
        printf("input and output file should be in ftprdw (ie VB) format\n");
        return (EXIT_FAILURE);
    }

    fp = fopen(*(argv + 1), "rb");
    if (fp == NULL)
    {
        printf("can't open %s\n", *(argv + 1));
        return (EXIT_FAILURE);
    }

    fq = fopen(*(argv + 2), "wb");
    if (fq == NULL)
    {
        printf("can't open %s\n", *(argv + 2));
        return (EXIT_FAILURE);
    }
    
    from = *(argv + 3);
    
    to = strchr(from, '/');
    
    if (to == NULL)
    {
        printf("missing / in 3rd parameter\n");
        return (EXIT_FAILURE);
    }
    
    *to = '\0';
    to++;
    
    diff = strlen(to) - strlen(from);

    flen = strlen(from);
    while (fread(lll, sizeof lll, 1, fp) == 1)
    {
        numrecs++;
        reclen = lll[0] << 8 | lll[1];
        if (reclen >= 4) reclen -= 4;
        if (fread(buf, reclen, 1, fp) != 1) break;

        /* if we have a reasonable sized buffer, see if we need to
           do some manipulations */
        if (reclen >= MLEN)
        {
            /* if a type 40 record */
            if (buf[10] == 0x40)
            {
                int i;
                int dlen;
                
                i = 16;

                /* add MLEN because the catalog name will be that */
                while ((i + MLEN + flen) < reclen)
                {
                    dlen = buf[i];
                    /* if the dsn prefix matches */
                    if ((dlen >= flen)
                        && (memcmp(&buf[i+MLEN+1], from, flen) == 0)
                       )
                    {
                        if (diff != 0)
                        {
                            memmove(&buf[i+MLEN+1+strlen(to)],
                                    &buf[i+MLEN+1+strlen(from)],
                                    reclen-i-MLEN-1);
                            reclen += diff;
                            *(short *)&buf[6] = reclen;
                            dlen += diff;
                            buf[i] = dlen;
                        }
                        memcpy(&buf[i+MLEN+1], to, strlen(to));
                    }
                    
                    i += (dlen + MLEN + 1);
                }
            }

            /* if a type 20 record */
            if (buf[10] == 0x20)
            {
                /* and the dsn prefix matches */
                if (memcmp(&buf[75], from, strlen(from)) == 0)
                {
                    /* if to is bigger, then shift to right */
                    if (diff >= 0)
                    {
                        memmove(&buf[75 + diff], &buf[75], MLEN - diff);
                    }
                    /* otherwise, shift to left - diff is negative */
                    else
                    {
                        memmove(&buf[75], &buf[75 - diff], MLEN + diff);
                        memset(&buf[75 + MLEN + diff], ' ', -diff);
                    }
                    memcpy(&buf[75], to, strlen(to));
                    buf[16] += diff;
                }
            }

            /* if a type 10 record */
            if (buf[10] == 0x10)
            {
                int extadj; /* adjustment for number of extents */
                
                /* and the dsn prefix matches */
                if (memcmp(&buf[44], from, strlen(from)) == 0)
                {
                    /* if to is bigger, then shift to right */
                    if (diff >= 0)
                    {
                        memmove(&buf[44 + diff], &buf[44], MLEN - diff);
                    }
                    /* otherwise, shift to left - diff is negative */
                    else
                    {
                        memmove(&buf[44], &buf[44 - diff], MLEN + diff);
                        memset(&buf[44 + MLEN + diff], ' ', -diff);
                    }
                    memcpy(&buf[44], to, strlen(to));
                }
                /* now we need to get to the dataset in the VVR */
                /* this technique will only work for DSSDUMP */
                extadj = (buf[42] - 1) * 10;
                if ((reclen > (205 + extadj))
                    && (memcmp(&buf[205+extadj], from, flen) == 0))
                {
                    if (diff != 0)
                    {
                        memmove(&buf[205+extadj+strlen(to)],
                                &buf[205+extadj+strlen(from)],
                                reclen-205-extadj-strlen(from));
                        reclen += diff;
                        *(short *)&buf[6] = reclen;
                        *(short *)&buf[194+extadj] += diff;
                        buf[204+extadj] += diff;
                        
                        /* change the subrecord length in dummy VVR */
                        *(short *)&buf[196+extadj] += diff;
                    }
                    memcpy(&buf[205+extadj], to, strlen(to));
                }
            }

        }
        if (diff != 0)
        {
            lll[0] = (reclen + 4) >> 8;
            lll[1] = (reclen + 4) & 0xff;
        }
        fwrite(lll, sizeof lll, 1, fq);
        fwrite(buf, reclen, 1, fq);
    }
    printf("%d records processed\n", numrecs);

    return (0);
}
