/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards                             */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  mvsendec - mvs encode and decode                                 */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

#define CHUNKSZ 40

static char *ascii2l(char *buf);
static int fasc(int asc);

int main(int argc, char **argv)
{
    int enc = 0;
    int dec = 0;
    int text = 0;
    FILE *fp;
    FILE *fq;
    char inbuf[CHUNKSZ * 2 + 1];
    char outbuf[CHUNKSZ * 2 + 1];
    size_t count;
    size_t x;
    static char tohex[] = { '0', '1', '2', '3', '4', '5', '6', '7',
                            '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    static char fromhex[UCHAR_MAX + 1];

    if (argc < 4)
    {
        printf("usage: mvsendec <encb/decb/enct/dect> <infile> <outfile>\n");
        return (EXIT_FAILURE);
    }
    if ((strcmp(*(argv + 1), "encb") == 0)
        || (strcmp(*(argv + 1), "ENCB") == 0))
    {
        enc = 1;
    }
    else if ((strcmp(*(argv + 1), "enct") == 0)
             || (strcmp(*(argv + 1), "ENCT") == 0))
    {
        enc = 1;
        text = 1;
    }
    else if ((strcmp(*(argv + 1), "decb") == 0)
             || (strcmp(*(argv + 1), "DECB") == 0))
    {
        dec = 1;
    }
    else if ((strcmp(*(argv + 1), "dect") == 0)
             || (strcmp(*(argv + 1), "DECT") == 0))
    {
        dec = 1;
        text = 1;
    }
    else
    {
        printf("need to specify encode or decode (binary or text)\n");
        return (EXIT_FAILURE);
    }

    fromhex['0'] = 0;
    fromhex['1'] = 1;
    fromhex['2'] = 2;
    fromhex['3'] = 3;
    fromhex['4'] = 4;
    fromhex['5'] = 5;
    fromhex['6'] = 6;
    fromhex['7'] = 7;
    fromhex['8'] = 8;
    fromhex['9'] = 9;
    fromhex['A'] = 10;
    fromhex['B'] = 11;
    fromhex['C'] = 12;
    fromhex['D'] = 13;
    fromhex['E'] = 14;
    fromhex['F'] = 15;

    if (enc)
    {
        fp = fopen(*(argv + 2), text ? "r" : "rb");
        fq = fopen(*(argv + 3), "w");
        if ((fp == NULL) || (fq == NULL))
        {
            printf("file open error\n");
            return (EXIT_FAILURE);
        }
    }
    else if (dec)
    {
        /* don't be tempted to open fp in binary mode knowing
           that it will be 80-character records, as they might
           have transferred the data into a LRECL=200 dataset
           etc instead */
        fp = fopen(*(argv + 2), "r");
        fq = fopen(*(argv + 3), text ? "w" : "wb");
        if ((fp == NULL) || (fq == NULL))
        {
            printf("file open error\n");
            return (EXIT_FAILURE);
        }
    }

    if (enc)
    {
        while (1)
        {
            count = fread(inbuf, 1, CHUNKSZ, fp);
            for (x = 0; x < count; x++)
            {
                outbuf[x * 2] = tohex[((unsigned char)inbuf[x] & 0xf0) >> 4];
                outbuf[x * 2 + 1] = tohex[(unsigned char)inbuf[x] & 0x0f];
            }
            if (count != 0)
            {
                outbuf[count * 2] = '\n';
                fwrite(outbuf, 1, count * 2 + 1, fq);
            }
            if (count < CHUNKSZ) break;
        }
    }
    else if (dec)
    {
        while (1)
        {
            count = fread(inbuf, 1, CHUNKSZ * 2 + 1, fp);
            for (x = 0; (x + 1) < count; x += 2)
            {
                if (!isxdigit((unsigned char)inbuf[x])
                    || !isxdigit((unsigned char)inbuf[x + 1]))
                {
                    count = x;
                    break;
                }
                outbuf[x / 2] = (fromhex[inbuf[x]] << 4)
                                | fromhex[inbuf[x + 1]];
            }
            if (text)
            {
                outbuf[count / 2] = '\0';
                ascii2l(outbuf);
            }
            fwrite(outbuf, 1, count / 2, fq);
            if (count < (CHUNKSZ * 2 + 1)) break;
        }
    }

    fclose(fp);
    fclose(fq);
    return (0);
}

static char *ascii2l(char *buf)
{
    char *p;
    int c;

    p = buf;
    while (*p != '\0')
    {
        c = fasc(*p);
        if (c == '\0')
        {
            printf("error translating %x\n", c);
            exit(0);
        }
        *p = c;
        p++;
    }
    return (buf);
}

static int fasc(int asc)
{
  switch (asc)
  {
    case 0x09 : return('\t');
    case 0x0a : return('\n');
    case 0x0c : return('\f');
    case 0x0d : return('\r');
    case 0x20 : return(' ');
    case 0x21 : return('!');
    case 0x22 : return('\"');
    case 0x23 : return('#');
    case 0x24 : return('$');
    case 0x25 : return('%');
    case 0x26 : return('&');
    case 0x27 : return('\'');
    case 0x28 : return('(');
    case 0x29 : return(')');
    case 0x2a : return('*');
    case 0x2b : return('+');
    case 0x2c : return(',');
    case 0x2d : return('-');
    case 0x2e : return('.');
    case 0x2f : return('/');
    case 0x30 : return('0');
    case 0x31 : return('1');
    case 0x32 : return('2');
    case 0x33 : return('3');
    case 0x34 : return('4');
    case 0x35 : return('5');
    case 0x36 : return('6');
    case 0x37 : return('7');
    case 0x38 : return('8');
    case 0x39 : return('9');
    case 0x3a : return(':');
    case 0x3b : return(';');
    case 0x3c : return('<');
    case 0x3d : return('=');
    case 0x3e : return('>');
    case 0x3f : return('?');
    case 0x40 : return('@');
    case 0x41 : return('A');
    case 0x42 : return('B');
    case 0x43 : return('C');
    case 0x44 : return('D');
    case 0x45 : return('E');
    case 0x46 : return('F');
    case 0x47 : return('G');
    case 0x48 : return('H');
    case 0x49 : return('I');
    case 0x4a : return('J');
    case 0x4b : return('K');
    case 0x4c : return('L');
    case 0x4d : return('M');
    case 0x4e : return('N');
    case 0x4f : return('O');
    case 0x50 : return('P');
    case 0x51 : return('Q');
    case 0x52 : return('R');
    case 0x53 : return('S');
    case 0x54 : return('T');
    case 0x55 : return('U');
    case 0x56 : return('V');
    case 0x57 : return('W');
    case 0x58 : return('X');
    case 0x59 : return('Y');
    case 0x5a : return('Z');
    case 0x5b : return('[');
    case 0x5c : return('\\');
    case 0x5d : return(']');
    case 0x5e : return('^');
    case 0x5f : return('_');
    case 0x60 : return('`');
    case 0x61 : return('a');
    case 0x62 : return('b');
    case 0x63 : return('c');
    case 0x64 : return('d');
    case 0x65 : return('e');
    case 0x66 : return('f');
    case 0x67 : return('g');
    case 0x68 : return('h');
    case 0x69 : return('i');
    case 0x6a : return('j');
    case 0x6b : return('k');
    case 0x6c : return('l');
    case 0x6d : return('m');
    case 0x6e : return('n');
    case 0x6f : return('o');
    case 0x70 : return('p');
    case 0x71 : return('q');
    case 0x72 : return('r');
    case 0x73 : return('s');
    case 0x74 : return('t');
    case 0x75 : return('u');
    case 0x76 : return('v');
    case 0x77 : return('w');
    case 0x78 : return('x');
    case 0x79 : return('y');
    case 0x7a : return('z');
    case 0x7b : return('{');
    case 0x7c : return('|');
    case 0x7d : return('}');
    case 0x7e : return('~');
    default   : return(0);
  }
}

