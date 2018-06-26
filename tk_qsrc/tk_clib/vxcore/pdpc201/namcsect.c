/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  namcsect - provide a name for a CSECT                            */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    char buf[500];
    
    if (argc < 2)
    {
        fprintf(stderr, "usage: namcsect <csect name>\n");
        fprintf(stderr, "will read from stdin and write to stdout\n");
        return (EXIT_FAILURE);
    }
    
    while (fgets(buf, sizeof buf, stdin) != NULL)
    {
        if (memcmp(buf, "         CSECT", 14) == 0)
        {
            memcpy(buf, *(argv + 1), strlen(*(argv + 1)));
        }
        fputs(buf, stdout);
    }

    return (0);
}
