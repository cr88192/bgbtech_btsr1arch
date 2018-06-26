/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  w32start - startup code for WIN32                                */
/*                                                                   */
/*********************************************************************/

/* This is the main entry point of a console mode executable */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

int main(int argc, char **argv);
void **__get_main_fp();

void mainCRTStartup(void)
{
#ifdef __MAIN_FP__
    (*(__get_main_fp()))=main;
#endif
    __start(0);
    return;
}

void __main(void)
{
    return;
}
