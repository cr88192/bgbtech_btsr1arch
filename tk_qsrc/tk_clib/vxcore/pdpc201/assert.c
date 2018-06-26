/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  assert.c - implementation of stuff in assert.h                   */
/*                                                                   */
/*********************************************************************/

#include "assert.h"
#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"

__PDPCLIB_API__ int __assert(char *x, char *y, int z)
{
    fprintf(stderr, "assertion failed for statement %s in "
            "file %s on line %d\n", x, y, z);
    abort();
    return (0);
}
