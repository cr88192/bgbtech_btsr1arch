// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// DESCRIPTION:
//	Simple basic typedefs, isolated here to make it easier
//	 separating modules.
//    
//-----------------------------------------------------------------------------


#ifndef __DOOMTYPE__
#define __DOOMTYPE__


#ifndef __BYTEBOOL__
#define __BYTEBOOL__
// Fixed to use builtin bool type with C++.
#ifdef __cplusplus
typedef bool boolean;
#else
typedef enum {false, true} boolean;
#endif
typedef unsigned char byte;
#endif


// Predefined with some OS.
#ifdef LINUX
#include <values.h>
#else
#define MAXCHAR		((char)0x7f)
#define MAXSHORT	((short)0x7fff)

// Max pos 32-bit int.
#define MAXINT		((int)0x7fffffff)	
#define MAXLONG		((long)0x7fffffff)
#define MINCHAR		((char)0x80)
#define MINSHORT	((short)0x8000)

// Max negative 32-bit integer.
#define MININT		((int)0x80000000)	
#define MINLONG		((long)0x80000000)
#endif

#ifndef R_OK
#define R_OK 4
#define W_OK 2
#define X_OK 1
#define F_OK 0
#endif

#ifndef _O_RDONLY
#define _O_RDONLY	0x00000000
#define _O_WRONLY	0x00000001
#define _O_RDWR		0x00000002
#define _O_CREAT	0x00000040
#define _O_EXCL		0x00000080
#define _O_NOCTTY	0x00000100
#define _O_TRUNC	0x00000200
#define _O_APPEND	0x00000400
#define _O_NONBLOCK	0x00000800
#define _O_SYNC		0x00001000
#define _O_DIRECT	0x00004000
#define _O_LARGEFILE	0x00008000
#define _O_DIRECTORY	0x00010000
#define _O_NOFOLLOW	0x00020000

#define	_O_TEXT		0x40000000
#define	_O_BINARY	0x80000000
#define	_O_RAW		_O_BINARY

#endif

#ifndef O_RDONLY
#define	O_RDONLY	_O_RDONLY
#define O_WRONLY	_O_WRONLY
#define O_RDWR		_O_RDWR
#define O_ACCMODE	_O_ACCMODE
#define	O_APPEND	_O_APPEND
#define	O_CREAT		_O_CREAT
#define	O_TRUNC		_O_TRUNC
#define	O_EXCL		_O_EXCL
#define	O_TEXT		_O_TEXT
#define	O_BINARY	_O_BINARY
#define	O_TEMPORARY	_O_TEMPORARY
#define O_NOINHERIT	_O_NOINHERIT
#define O_SEQUENTIAL	_O_SEQUENTIAL
#define	O_RANDOM	_O_RANDOM
#endif

#endif
//-----------------------------------------------------------------------------
//
// $Log:$
//
//-----------------------------------------------------------------------------
