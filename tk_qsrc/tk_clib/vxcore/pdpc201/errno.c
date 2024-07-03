/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  errno.c - implementation of stuff in errno.h                     */
/*                                                                   */
/*********************************************************************/

#include "stddef.h"

#ifndef __TK_CLIB_DLLSTUB__

int __errno = 0;

__PDPCLIB_API__ int *__get_errno()
    { return(&__errno); }

#endif
