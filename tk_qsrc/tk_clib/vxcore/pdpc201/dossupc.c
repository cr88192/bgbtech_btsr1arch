/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  dossupc.c - some support routines for floating point work        */
/*  for MSDOS                                                        */
/*                                                                   */
/*********************************************************************/

#ifdef __OS2__
int __turboFloat;

long _ftol(double x)
{
    (void)x;
    return (0);
}
#endif

#ifdef __MSDOS__

#ifdef __WATCOMC__
#define CTYP __cdecl
#else
#define CTYP
#endif

unsigned long CTYP __divide(unsigned long x, unsigned long y)
{
    unsigned int a;
    unsigned int b;
    unsigned int c;
    unsigned int d;
    unsigned int e;
    unsigned int f;
    unsigned int g = 0;
    unsigned int h;

    b = (unsigned int)(y & 0xffffU);
    a = (unsigned int)(y >> 16);
    c = -b;
    d = a;
    if (b > 0)
    {
        d++;
    }
    f = 1;
    while (f != 0)
    {
        e = (unsigned int)(x >> 16);
        h = (unsigned int)(x & 0xffffU);
        f = e / d;
        g += f;
        e = e % d;
        x = ((unsigned long)e << 16) + (unsigned long)c * f + h;
    }
    if (x >= y)
    {
        g++;
    }
    return ((unsigned long)g);
}

unsigned long CTYP __modulo(unsigned long x, unsigned long y)
{
    unsigned int a;
    unsigned int b;
    unsigned int c;
    unsigned int d;
    unsigned int e;
    unsigned int f;
    unsigned int g = 0;
    unsigned int h;

    b = (unsigned int)(y & 0xffffU);
    a = (unsigned int)(y >> 16);
    c = -b;
    d = a;
    if (b > 0)
    {
        d++;
    }
    f = 1;
    while (f != 0)
    {
        e = (unsigned int)(x >> 16);
        h = (unsigned int)(x & 0xffffU);
        f = e / d;
        g += f;
        e = e % d;
        x = ((unsigned long)e << 16) + (unsigned long)c * f + h;
    }
    if (x >= y)
    {
        x -= y;
    }
    return (x);
}

#ifdef __WATCOMC__
void _cstart(void)
{
    return;
}

int _argc, _8087;

#endif

#endif

