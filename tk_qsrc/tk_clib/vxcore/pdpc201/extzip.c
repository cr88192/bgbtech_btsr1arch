/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  extzip.c - extract an encoded zip from a printout                */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

static char buf[800];

int main(void)
{
    int write = 0;
    
    while (fgets(buf, sizeof buf, stdin) != NULL)
    {
        /* allow a formfeed to be represented as any
           one or two control characters */
        if (iscntrl((unsigned char)buf[0]))
        {
            memmove(buf, buf + 1, strlen(buf));
            if (iscntrl((unsigned char)buf[0]))
            {
                memmove(buf, buf + 1, strlen(buf));
            }
            if (memcmp(buf, "504B", 4) == 0)
            {
                write = 1;
            }
        
        }
        else if (buf[0] == '\f')
        {
            write = 0;
        }
        if (write)
        {
            fputs(buf, stdout);
        }
    }
    return (0);
}
