/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  folks - read a VB (MVS) format file in binary mode, and process  */
/*  its contents. Note that the C standard allows binary files to    */
/*  be NUL-padded. This program allows for that.                     */
/*                                                                   */
/*  The intention of this program is to provide example PDPCLIB      */
/*  behaviour that should occur regardless of RECFM=F/V/U when       */
/*  opened in binary mode.                                           */
/*                                                                   */
/*********************************************************************/


/* Here is an example of transferring a VB file with
   "Hello there"
   "Folks"
   in it to the PC, preserving the RDW (Record Descriptor Word)
*/

/*
binary
 EZA1701I >>>TYPE i
 200 Type is Image (Binary)
 EZA1460I Command:
locsite rdw
 EZA1460I Command:
put folks
 EZA1701I >>>SITE VARrecfm Lrecl=256 Recfm=VB BLKSIZE=27998
 500 Syntax error, Command not recognized.
>>> PC ignores SITE command.  Use sendsite to suppress <<<
 EZA1701I >>>STOR folks
 150 Ready to receive "/C/folks". Mode STREAM Type BINARY.
 226 Transfer finished successfully. Closing data connection.
 EZA1617I 24 bytes transferred in 0.076 seconds. Transfer rate 0.32 Kbytes/sec.
 EZA1460I Command:
quit
*/


/* Here is a hexdump of the transferred file on arrival at the PC */

/*
000000  000F0000 C8859393 9640A388 85998500  .........@......
000010  090000C6 969392A2                    ........
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    FILE *fp;
    unsigned char lenb[4];
    int len;
    int c;
    int x;

    if (argc <= 1)
    {
        printf("usage: folks <fnm>\n");
        return (EXIT_FAILURE);
    }    
    fp = fopen(*(argv + 1), "rb");
    if (fp == NULL)
    {
        printf("failed to open %s\n", *(argv + 1));
        return (EXIT_FAILURE);
    }
    while (fread(lenb, sizeof lenb, 1, fp) == 1)
    {
        len = lenb[0] << 8 | lenb[1];
        printf("len is %d\n", len);
        if (lenb[2] != 0 || lenb[3] != 0)
        {
            printf("file is corrupt - reserved\n");
            return (EXIT_FAILURE);
        }
        if (len == 0) /* allow for NUL-padding */
        {
            break;
        }
        if (len < 4)
        {
            printf("file is corrupt - length\n");
            return (EXIT_FAILURE);
        }
        c = fgetc(fp);
        printf("first data byte is %x\n", c);
        for (x = 5; x < len; x++)
        {
            fgetc(fp);
        }
    }
    return (EXIT_SUCCESS);
}
