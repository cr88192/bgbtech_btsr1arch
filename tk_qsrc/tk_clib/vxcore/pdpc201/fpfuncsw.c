/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  fpfuncsw.c - some support routines for floating point work       */
/*  for Watcom C compiler for OS/2                                   */
/*                                                                   */
/*********************************************************************/

int _fltused_ = 0;
int __real87 = 0;
int _argc;

__init_387_emulator(void)
{
    return;
}

__8087(void)
{
    return;
}

__STOSD(void)
{
    return;
}

__CHP(void)
{
    return;
}

void __main(void);

_cstart_(void)
{
    __main();
    return;
}
