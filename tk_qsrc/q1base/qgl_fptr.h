/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
/*
** QGL.H
*/

#ifndef __QGL_H__
#define __QGL_H__

#if defined( __LINT__ )

#include <GL/gl.h>
// #include <GL/glext.h>

#elif defined( _WIN32 )

#pragma warning (disable: 4201)
#pragma warning (disable: 4214)
#pragma warning (disable: 4514)
#pragma warning (disable: 4032)
#pragma warning (disable: 4201)
#pragma warning (disable: 4214)
#include <windows.h>
#include <gl/gl.h>

#elif defined(MACOS_X)

#include "macosx_glimp.h"

#elif defined( __linux__ )

#include <GL/gl.h>
#include <GL/glx.h>
// bk001129 - from cvs1.17 (mkv)
#if defined(__FX__)
#include <GL/fxmesa.h>
#endif

#elif defined( __FreeBSD__ ) // rb010123

#include <GL/gl.h>
#include <GL/glx.h>
#if defined(__FX__)
#include <GL/fxmesa.h>
#endif

#else

// #include <gl.h>
#include <GL/gl.h>

#endif

#ifndef APIENTRY
#define APIENTRY
#endif

#ifndef WINAPI
#define WINAPI
#endif


//===========================================================================

/*
** multitexture extension definitions
*/
#define GL_ACTIVE_TEXTURE_ARB               0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE_ARB        0x84E1
#define GL_MAX_ACTIVE_TEXTURES_ARB          0x84E2

#define GL_TEXTURE0_ARB                     0x84C0
#define GL_TEXTURE1_ARB                     0x84C1
#define GL_TEXTURE2_ARB                     0x84C2
#define GL_TEXTURE3_ARB                     0x84C3

// NOTE: some Linux platforms would need those prototypes
#if defined(MACOS_X)
typedef void (APIENTRY * PFNGLMULTITEXCOORD1DARBPROC) (GLenum target, GLdouble s);
typedef void (APIENTRY * PFNGLMULTITEXCOORD1DVARBPROC) (GLenum target, const GLdouble *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD1FARBPROC) (GLenum target, GLfloat s);
typedef void (APIENTRY * PFNGLMULTITEXCOORD1FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD1IARBPROC) (GLenum target, GLint s);
typedef void (APIENTRY * PFNGLMULTITEXCOORD1IVARBPROC) (GLenum target, const GLint *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD1SARBPROC) (GLenum target, GLshort s);
typedef void (APIENTRY * PFNGLMULTITEXCOORD1SVARBPROC) (GLenum target, const GLshort *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2DARBPROC) (GLenum target, GLdouble s, GLdouble t);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2DVARBPROC) (GLenum target, const GLdouble *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2FARBPROC) (GLenum target, GLfloat s, GLfloat t);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2IARBPROC) (GLenum target, GLint s, GLint t);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2IVARBPROC) (GLenum target, const GLint *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2SARBPROC) (GLenum target, GLshort s, GLshort t);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2SVARBPROC) (GLenum target, const GLshort *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3DARBPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3DVARBPROC) (GLenum target, const GLdouble *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3FARBPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3IARBPROC) (GLenum target, GLint s, GLint t, GLint r);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3IVARBPROC) (GLenum target, const GLint *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3SARBPROC) (GLenum target, GLshort s, GLshort t, GLshort r);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3SVARBPROC) (GLenum target, const GLshort *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4DARBPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4DVARBPROC) (GLenum target, const GLdouble *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4FARBPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4IARBPROC) (GLenum target, GLint s, GLint t, GLint r, GLint q);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4IVARBPROC) (GLenum target, const GLint *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4SARBPROC) (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4SVARBPROC) (GLenum target, const GLshort *v);
typedef void (APIENTRY * PFNGLACTIVETEXTUREARBPROC) (GLenum target);
typedef void (APIENTRY * PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum target);
#endif

// TTimo - VC7 / XP ?
#ifdef _WIN32
typedef void (APIENTRY * PFNGLMULTITEXCOORD2FARBPROC) (GLenum target, GLfloat s, GLfloat t);
typedef void (APIENTRY * PFNGLACTIVETEXTUREARBPROC) (GLenum target);
typedef void (APIENTRY * PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum target);
#endif

/*
** extension constants
*/


// S3TC compression constants
#define GL_RGB_S3TC							0x83A0
#define GL_RGB4_S3TC						0x83A1

#define GLEXTERN extern
// #define GLEXTERN

// extensions will be function pointers on all platforms

GLEXTERN	void ( APIENTRY * qglMultiTexCoord2fARB )( GLenum texture, GLfloat s, GLfloat t );
GLEXTERN	void ( APIENTRY * qglActiveTextureARB )( GLenum texture );
GLEXTERN	void ( APIENTRY * qglClientActiveTextureARB )( GLenum texture );

GLEXTERN	void ( APIENTRY * qglLockArraysEXT) (GLint, GLint);
GLEXTERN	void ( APIENTRY * qglUnlockArraysEXT) (void);

//===========================================================================

// non-windows systems will just redefine qgl* to gl*
// #if !defined( _WIN32 ) && !defined(MACOS_X) && !defined( __linux__ ) && !defined( __FreeBSD__ ) && !defined(__BJX2__) // rb010123

// #include "qgl_linked.h"

// #elif defined(MACOS_X)
// This includes #ifdefs for optional logging and GL error checking after every GL call as well as #defines to prevent incorrect usage of the non-'qgl' versions of the GL API.
// #include "macosx_qgl.h"

// #else

// windows systems use a function pointer for each call so we can load minidrivers

GLEXTERN  void ( APIENTRY * qglAccum )(GLenum op, GLfloat value);
GLEXTERN  void ( APIENTRY * qglAlphaFunc )(GLenum func, GLclampf ref);
GLEXTERN  GLboolean ( APIENTRY * qglAreTexturesResident )(GLsizei n, const GLuint *textures, GLboolean *residences);
GLEXTERN  void ( APIENTRY * qglArrayElement )(GLint i);
GLEXTERN  void ( APIENTRY * qglBegin )(GLenum mode);
GLEXTERN  void ( APIENTRY * qglBindTexture )(GLenum target, GLuint texture);
GLEXTERN  void ( APIENTRY * qglBitmap )(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
GLEXTERN  void ( APIENTRY * qglBlendFunc )(GLenum sfactor, GLenum dfactor);
GLEXTERN  void ( APIENTRY * qglCallList )(GLuint list);
GLEXTERN  void ( APIENTRY * qglCallLists )(GLsizei n, GLenum type, const GLvoid *lists);
GLEXTERN  void ( APIENTRY * qglClear )(GLbitfield mask);
GLEXTERN  void ( APIENTRY * qglClearAccum )(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLEXTERN  void ( APIENTRY * qglClearColor )(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
GLEXTERN  void ( APIENTRY * qglClearDepth )(GLclampd depth);
GLEXTERN  void ( APIENTRY * qglClearIndex )(GLfloat c);
GLEXTERN  void ( APIENTRY * qglClearStencil )(GLint s);
GLEXTERN  void ( APIENTRY * qglClipPlane )(GLenum plane, const GLdouble *equation);
GLEXTERN  void ( APIENTRY * qglColor3b )(GLbyte red, GLbyte green, GLbyte blue);
GLEXTERN  void ( APIENTRY * qglColor3bv )(const GLbyte *v);
GLEXTERN  void ( APIENTRY * qglColor3d )(GLdouble red, GLdouble green, GLdouble blue);
GLEXTERN  void ( APIENTRY * qglColor3dv )(const GLdouble *v);
GLEXTERN  void ( APIENTRY * qglColor3f )(GLfloat red, GLfloat green, GLfloat blue);
GLEXTERN  void ( APIENTRY * qglColor3fv )(const GLfloat *v);
GLEXTERN  void ( APIENTRY * qglColor3i )(GLint red, GLint green, GLint blue);
GLEXTERN  void ( APIENTRY * qglColor3iv )(const GLint *v);
GLEXTERN  void ( APIENTRY * qglColor3s )(GLshort red, GLshort green, GLshort blue);
GLEXTERN  void ( APIENTRY * qglColor3sv )(const GLshort *v);
GLEXTERN  void ( APIENTRY * qglColor3ub )(GLubyte red, GLubyte green, GLubyte blue);
GLEXTERN  void ( APIENTRY * qglColor3ubv )(const GLubyte *v);
GLEXTERN  void ( APIENTRY * qglColor3ui )(GLuint red, GLuint green, GLuint blue);
GLEXTERN  void ( APIENTRY * qglColor3uiv )(const GLuint *v);
GLEXTERN  void ( APIENTRY * qglColor3us )(GLushort red, GLushort green, GLushort blue);
GLEXTERN  void ( APIENTRY * qglColor3usv )(const GLushort *v);
GLEXTERN  void ( APIENTRY * qglColor4b )(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
GLEXTERN  void ( APIENTRY * qglColor4bv )(const GLbyte *v);
GLEXTERN  void ( APIENTRY * qglColor4d )(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
GLEXTERN  void ( APIENTRY * qglColor4dv )(const GLdouble *v);
GLEXTERN  void ( APIENTRY * qglColor4f )(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLEXTERN  void ( APIENTRY * qglColor4fv )(const GLfloat *v);
GLEXTERN  void ( APIENTRY * qglColor4i )(GLint red, GLint green, GLint blue, GLint alpha);
GLEXTERN  void ( APIENTRY * qglColor4iv )(const GLint *v);
GLEXTERN  void ( APIENTRY * qglColor4s )(GLshort red, GLshort green, GLshort blue, GLshort alpha);
GLEXTERN  void ( APIENTRY * qglColor4sv )(const GLshort *v);
GLEXTERN  void ( APIENTRY * qglColor4ub )(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
GLEXTERN  void ( APIENTRY * qglColor4ubv )(const GLubyte *v);
GLEXTERN  void ( APIENTRY * qglColor4ui )(GLuint red, GLuint green, GLuint blue, GLuint alpha);
GLEXTERN  void ( APIENTRY * qglColor4uiv )(const GLuint *v);
GLEXTERN  void ( APIENTRY * qglColor4us )(GLushort red, GLushort green, GLushort blue, GLushort alpha);
GLEXTERN  void ( APIENTRY * qglColor4usv )(const GLushort *v);
GLEXTERN  void ( APIENTRY * qglColorMask )(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
GLEXTERN  void ( APIENTRY * qglColorMaterial )(GLenum face, GLenum mode);
GLEXTERN  void ( APIENTRY * qglColorPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

GLEXTERN  void ( APIENTRY * qglCompressedTexImage2D)(GLenum target,
	GLint level, GLenum internalformat,
	GLsizei width, GLsizei height, GLint border,
	GLsizei imageSize, const void * data);

GLEXTERN  void ( APIENTRY * qglCopyPixels )(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
GLEXTERN  void ( APIENTRY * qglCopyTexImage1D )(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
GLEXTERN  void ( APIENTRY * qglCopyTexImage2D )(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
GLEXTERN  void ( APIENTRY * qglCopyTexSubImage1D )(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
GLEXTERN  void ( APIENTRY * qglCopyTexSubImage2D )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GLEXTERN  void ( APIENTRY * qglCullFace )(GLenum mode);
GLEXTERN  void ( APIENTRY * qglDeleteLists )(GLuint list, GLsizei range);
GLEXTERN  void ( APIENTRY * qglDeleteTextures )(GLsizei n, const GLuint *textures);
GLEXTERN  void ( APIENTRY * qglDepthFunc )(GLenum func);
GLEXTERN  void ( APIENTRY * qglDepthMask )(GLboolean flag);
GLEXTERN  void ( APIENTRY * qglDepthRange )(GLclampd zNear, GLclampd zFar);
GLEXTERN  void ( APIENTRY * qglDisable )(GLenum cap);
GLEXTERN  void ( APIENTRY * qglDisableClientState )(GLenum array);
GLEXTERN  void ( APIENTRY * qglDrawArrays )(GLenum mode, GLint first, GLsizei count);
GLEXTERN  void ( APIENTRY * qglDrawBuffer )(GLenum mode);
GLEXTERN  void ( APIENTRY * qglDrawElements )(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
GLEXTERN  void ( APIENTRY * qglDrawPixels )(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
GLEXTERN  void ( APIENTRY * qglEdgeFlag )(GLboolean flag);
GLEXTERN  void ( APIENTRY * qglEdgeFlagPointer )(GLsizei stride, const GLvoid *pointer);
GLEXTERN  void ( APIENTRY * qglEdgeFlagv )(const GLboolean *flag);
GLEXTERN  void ( APIENTRY * qglEnable )(GLenum cap);
GLEXTERN  void ( APIENTRY * qglEnableClientState )(GLenum array);
GLEXTERN  void ( APIENTRY * qglEnd )(void);
GLEXTERN  void ( APIENTRY * qglEndList )(void);
GLEXTERN  void ( APIENTRY * qglEvalCoord1d )(GLdouble u);
GLEXTERN  void ( APIENTRY * qglEvalCoord1dv )(const GLdouble *u);
GLEXTERN  void ( APIENTRY * qglEvalCoord1f )(GLfloat u);
GLEXTERN  void ( APIENTRY * qglEvalCoord1fv )(const GLfloat *u);
GLEXTERN  void ( APIENTRY * qglEvalCoord2d )(GLdouble u, GLdouble v);
GLEXTERN  void ( APIENTRY * qglEvalCoord2dv )(const GLdouble *u);
GLEXTERN  void ( APIENTRY * qglEvalCoord2f )(GLfloat u, GLfloat v);
GLEXTERN  void ( APIENTRY * qglEvalCoord2fv )(const GLfloat *u);
GLEXTERN  void ( APIENTRY * qglEvalMesh1 )(GLenum mode, GLint i1, GLint i2);
GLEXTERN  void ( APIENTRY * qglEvalMesh2 )(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
GLEXTERN  void ( APIENTRY * qglEvalPoint1 )(GLint i);
GLEXTERN  void ( APIENTRY * qglEvalPoint2 )(GLint i, GLint j);
GLEXTERN  void ( APIENTRY * qglFeedbackBuffer )(GLsizei size, GLenum type, GLfloat *buffer);
GLEXTERN  void ( APIENTRY * qglFinish )(void);
GLEXTERN  void ( APIENTRY * qglFlush )(void);
GLEXTERN  void ( APIENTRY * qglFogf )(GLenum pname, GLfloat param);
GLEXTERN  void ( APIENTRY * qglFogfv )(GLenum pname, const GLfloat *params);
GLEXTERN  void ( APIENTRY * qglFogi )(GLenum pname, GLint param);
GLEXTERN  void ( APIENTRY * qglFogiv )(GLenum pname, const GLint *params);
GLEXTERN  void ( APIENTRY * qglFrontFace )(GLenum mode);
GLEXTERN  void ( APIENTRY * qglFrustum )(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
GLEXTERN  GLuint ( APIENTRY * qglGenLists )(GLsizei range);
GLEXTERN  void ( APIENTRY * qglGenTextures )(GLsizei n, GLuint *textures);
GLEXTERN  void ( APIENTRY * qglGetBooleanv )(GLenum pname, GLboolean *params);
GLEXTERN  void ( APIENTRY * qglGetClipPlane )(GLenum plane, GLdouble *equation);
GLEXTERN  void ( APIENTRY * qglGetDoublev )(GLenum pname, GLdouble *params);
GLEXTERN  GLenum ( APIENTRY * qglGetError )(void);
GLEXTERN  void ( APIENTRY * qglGetFloatv )(GLenum pname, GLfloat *params);
GLEXTERN  void ( APIENTRY * qglGetIntegerv )(GLenum pname, GLint *params);
GLEXTERN  void ( APIENTRY * qglGetLightfv )(GLenum light, GLenum pname, GLfloat *params);
GLEXTERN  void ( APIENTRY * qglGetLightiv )(GLenum light, GLenum pname, GLint *params);
GLEXTERN  void ( APIENTRY * qglGetMapdv )(GLenum target, GLenum query, GLdouble *v);
GLEXTERN  void ( APIENTRY * qglGetMapfv )(GLenum target, GLenum query, GLfloat *v);
GLEXTERN  void ( APIENTRY * qglGetMapiv )(GLenum target, GLenum query, GLint *v);
GLEXTERN  void ( APIENTRY * qglGetMaterialfv )(GLenum face, GLenum pname, GLfloat *params);
GLEXTERN  void ( APIENTRY * qglGetMaterialiv )(GLenum face, GLenum pname, GLint *params);
GLEXTERN  void ( APIENTRY * qglGetPixelMapfv )(GLenum map, GLfloat *values);
GLEXTERN  void ( APIENTRY * qglGetPixelMapuiv )(GLenum map, GLuint *values);
GLEXTERN  void ( APIENTRY * qglGetPixelMapusv )(GLenum map, GLushort *values);
GLEXTERN  void ( APIENTRY * qglGetPointerv )(GLenum pname, GLvoid* *params);
GLEXTERN  void ( APIENTRY * qglGetPolygonStipple )(GLubyte *mask);
GLEXTERN  const GLubyte * ( APIENTRY * qglGetString )(GLenum name);
GLEXTERN  void ( APIENTRY * qglGetTexEnvfv )(GLenum target, GLenum pname, GLfloat *params);
GLEXTERN  void ( APIENTRY * qglGetTexEnviv )(GLenum target, GLenum pname, GLint *params);
GLEXTERN  void ( APIENTRY * qglGetTexGendv )(GLenum coord, GLenum pname, GLdouble *params);
GLEXTERN  void ( APIENTRY * qglGetTexGenfv )(GLenum coord, GLenum pname, GLfloat *params);
GLEXTERN  void ( APIENTRY * qglGetTexGeniv )(GLenum coord, GLenum pname, GLint *params);
GLEXTERN  void ( APIENTRY * qglGetTexImage )(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
GLEXTERN  void ( APIENTRY * qglGetTexLevelParameterfv )(GLenum target, GLint level, GLenum pname, GLfloat *params);
GLEXTERN  void ( APIENTRY * qglGetTexLevelParameteriv )(GLenum target, GLint level, GLenum pname, GLint *params);
GLEXTERN  void ( APIENTRY * qglGetTexParameterfv )(GLenum target, GLenum pname, GLfloat *params);
GLEXTERN  void ( APIENTRY * qglGetTexParameteriv )(GLenum target, GLenum pname, GLint *params);
GLEXTERN  void ( APIENTRY * qglHint )(GLenum target, GLenum mode);
GLEXTERN  void ( APIENTRY * qglIndexMask )(GLuint mask);
GLEXTERN  void ( APIENTRY * qglIndexPointer )(GLenum type, GLsizei stride, const GLvoid *pointer);
GLEXTERN  void ( APIENTRY * qglIndexd )(GLdouble c);
GLEXTERN  void ( APIENTRY * qglIndexdv )(const GLdouble *c);
GLEXTERN  void ( APIENTRY * qglIndexf )(GLfloat c);
GLEXTERN  void ( APIENTRY * qglIndexfv )(const GLfloat *c);
GLEXTERN  void ( APIENTRY * qglIndexi )(GLint c);
GLEXTERN  void ( APIENTRY * qglIndexiv )(const GLint *c);
GLEXTERN  void ( APIENTRY * qglIndexs )(GLshort c);
GLEXTERN  void ( APIENTRY * qglIndexsv )(const GLshort *c);
GLEXTERN  void ( APIENTRY * qglIndexub )(GLubyte c);
GLEXTERN  void ( APIENTRY * qglIndexubv )(const GLubyte *c);
GLEXTERN  void ( APIENTRY * qglInitNames )(void);
GLEXTERN  void ( APIENTRY * qglInterleavedArrays )(GLenum format, GLsizei stride, const GLvoid *pointer);
GLEXTERN  GLboolean ( APIENTRY * qglIsEnabled )(GLenum cap);
GLEXTERN  GLboolean ( APIENTRY * qglIsList )(GLuint list);
GLEXTERN  GLboolean ( APIENTRY * qglIsTexture )(GLuint texture);
GLEXTERN  void ( APIENTRY * qglLightModelf )(GLenum pname, GLfloat param);
GLEXTERN  void ( APIENTRY * qglLightModelfv )(GLenum pname, const GLfloat *params);
GLEXTERN  void ( APIENTRY * qglLightModeli )(GLenum pname, GLint param);
GLEXTERN  void ( APIENTRY * qglLightModeliv )(GLenum pname, const GLint *params);
GLEXTERN  void ( APIENTRY * qglLightf )(GLenum light, GLenum pname, GLfloat param);
GLEXTERN  void ( APIENTRY * qglLightfv )(GLenum light, GLenum pname, const GLfloat *params);
GLEXTERN  void ( APIENTRY * qglLighti )(GLenum light, GLenum pname, GLint param);
GLEXTERN  void ( APIENTRY * qglLightiv )(GLenum light, GLenum pname, const GLint *params);
GLEXTERN  void ( APIENTRY * qglLineStipple )(GLint factor, GLushort pattern);
GLEXTERN  void ( APIENTRY * qglLineWidth )(GLfloat width);
GLEXTERN  void ( APIENTRY * qglListBase )(GLuint base);
GLEXTERN  void ( APIENTRY * qglLoadIdentity )(void);
GLEXTERN  void ( APIENTRY * qglLoadMatrixd )(const GLdouble *m);
GLEXTERN  void ( APIENTRY * qglLoadMatrixf )(const GLfloat *m);
GLEXTERN  void ( APIENTRY * qglLoadName )(GLuint name);
GLEXTERN  void ( APIENTRY * qglLogicOp )(GLenum opcode);
GLEXTERN  void ( APIENTRY * qglMap1d )(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
GLEXTERN  void ( APIENTRY * qglMap1f )(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
GLEXTERN  void ( APIENTRY * qglMap2d )(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
GLEXTERN  void ( APIENTRY * qglMap2f )(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
GLEXTERN  void ( APIENTRY * qglMapGrid1d )(GLint un, GLdouble u1, GLdouble u2);
GLEXTERN  void ( APIENTRY * qglMapGrid1f )(GLint un, GLfloat u1, GLfloat u2);
GLEXTERN  void ( APIENTRY * qglMapGrid2d )(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
GLEXTERN  void ( APIENTRY * qglMapGrid2f )(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
GLEXTERN  void ( APIENTRY * qglMaterialf )(GLenum face, GLenum pname, GLfloat param);
GLEXTERN  void ( APIENTRY * qglMaterialfv )(GLenum face, GLenum pname, const GLfloat *params);
GLEXTERN  void ( APIENTRY * qglMateriali )(GLenum face, GLenum pname, GLint param);
GLEXTERN  void ( APIENTRY * qglMaterialiv )(GLenum face, GLenum pname, const GLint *params);
GLEXTERN  void ( APIENTRY * qglMatrixMode )(GLenum mode);
GLEXTERN  void ( APIENTRY * qglMultMatrixd )(const GLdouble *m);
GLEXTERN  void ( APIENTRY * qglMultMatrixf )(const GLfloat *m);
GLEXTERN  void ( APIENTRY * qglNewList )(GLuint list, GLenum mode);
GLEXTERN  void ( APIENTRY * qglNormal3b )(GLbyte nx, GLbyte ny, GLbyte nz);
GLEXTERN  void ( APIENTRY * qglNormal3bv )(const GLbyte *v);
GLEXTERN  void ( APIENTRY * qglNormal3d )(GLdouble nx, GLdouble ny, GLdouble nz);
GLEXTERN  void ( APIENTRY * qglNormal3dv )(const GLdouble *v);
GLEXTERN  void ( APIENTRY * qglNormal3f )(GLfloat nx, GLfloat ny, GLfloat nz);
GLEXTERN  void ( APIENTRY * qglNormal3fv )(const GLfloat *v);
GLEXTERN  void ( APIENTRY * qglNormal3i )(GLint nx, GLint ny, GLint nz);
GLEXTERN  void ( APIENTRY * qglNormal3iv )(const GLint *v);
GLEXTERN  void ( APIENTRY * qglNormal3s )(GLshort nx, GLshort ny, GLshort nz);
GLEXTERN  void ( APIENTRY * qglNormal3sv )(const GLshort *v);
GLEXTERN  void ( APIENTRY * qglNormalPointer )(GLenum type, GLsizei stride, const GLvoid *pointer);
GLEXTERN  void ( APIENTRY * qglOrtho )(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
GLEXTERN  void ( APIENTRY * qglPassThrough )(GLfloat token);
GLEXTERN  void ( APIENTRY * qglPixelMapfv )(GLenum map, GLsizei mapsize, const GLfloat *values);
GLEXTERN  void ( APIENTRY * qglPixelMapuiv )(GLenum map, GLsizei mapsize, const GLuint *values);
GLEXTERN  void ( APIENTRY * qglPixelMapusv )(GLenum map, GLsizei mapsize, const GLushort *values);
GLEXTERN  void ( APIENTRY * qglPixelStoref )(GLenum pname, GLfloat param);
GLEXTERN  void ( APIENTRY * qglPixelStorei )(GLenum pname, GLint param);
GLEXTERN  void ( APIENTRY * qglPixelTransferf )(GLenum pname, GLfloat param);
GLEXTERN  void ( APIENTRY * qglPixelTransferi )(GLenum pname, GLint param);
GLEXTERN  void ( APIENTRY * qglPixelZoom )(GLfloat xfactor, GLfloat yfactor);
GLEXTERN  void ( APIENTRY * qglPointSize )(GLfloat size);
GLEXTERN  void ( APIENTRY * qglPolygonMode )(GLenum face, GLenum mode);
GLEXTERN  void ( APIENTRY * qglPolygonOffset )(GLfloat factor, GLfloat units);
GLEXTERN  void ( APIENTRY * qglPolygonStipple )(const GLubyte *mask);
GLEXTERN  void ( APIENTRY * qglPopAttrib )(void);
GLEXTERN  void ( APIENTRY * qglPopClientAttrib )(void);
GLEXTERN  void ( APIENTRY * qglPopMatrix )(void);
GLEXTERN  void ( APIENTRY * qglPopName )(void);
GLEXTERN  void ( APIENTRY * qglPrioritizeTextures )(GLsizei n, const GLuint *textures, const GLclampf *priorities);
GLEXTERN  void ( APIENTRY * qglPushAttrib )(GLbitfield mask);
GLEXTERN  void ( APIENTRY * qglPushClientAttrib )(GLbitfield mask);
GLEXTERN  void ( APIENTRY * qglPushMatrix )(void);
GLEXTERN  void ( APIENTRY * qglPushName )(GLuint name);
GLEXTERN  void ( APIENTRY * qglRasterPos2d )(GLdouble x, GLdouble y);
GLEXTERN  void ( APIENTRY * qglRasterPos2dv )(const GLdouble *v);
GLEXTERN  void ( APIENTRY * qglRasterPos2f )(GLfloat x, GLfloat y);
GLEXTERN  void ( APIENTRY * qglRasterPos2fv )(const GLfloat *v);
GLEXTERN  void ( APIENTRY * qglRasterPos2i )(GLint x, GLint y);
GLEXTERN  void ( APIENTRY * qglRasterPos2iv )(const GLint *v);
GLEXTERN  void ( APIENTRY * qglRasterPos2s )(GLshort x, GLshort y);
GLEXTERN  void ( APIENTRY * qglRasterPos2sv )(const GLshort *v);
GLEXTERN  void ( APIENTRY * qglRasterPos3d )(GLdouble x, GLdouble y, GLdouble z);
GLEXTERN  void ( APIENTRY * qglRasterPos3dv )(const GLdouble *v);
GLEXTERN  void ( APIENTRY * qglRasterPos3f )(GLfloat x, GLfloat y, GLfloat z);
GLEXTERN  void ( APIENTRY * qglRasterPos3fv )(const GLfloat *v);
GLEXTERN  void ( APIENTRY * qglRasterPos3i )(GLint x, GLint y, GLint z);
GLEXTERN  void ( APIENTRY * qglRasterPos3iv )(const GLint *v);
GLEXTERN  void ( APIENTRY * qglRasterPos3s )(GLshort x, GLshort y, GLshort z);
GLEXTERN  void ( APIENTRY * qglRasterPos3sv )(const GLshort *v);
GLEXTERN  void ( APIENTRY * qglRasterPos4d )(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLEXTERN  void ( APIENTRY * qglRasterPos4dv )(const GLdouble *v);
GLEXTERN  void ( APIENTRY * qglRasterPos4f )(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLEXTERN  void ( APIENTRY * qglRasterPos4fv )(const GLfloat *v);
GLEXTERN  void ( APIENTRY * qglRasterPos4i )(GLint x, GLint y, GLint z, GLint w);
GLEXTERN  void ( APIENTRY * qglRasterPos4iv )(const GLint *v);
GLEXTERN  void ( APIENTRY * qglRasterPos4s )(GLshort x, GLshort y, GLshort z, GLshort w);
GLEXTERN  void ( APIENTRY * qglRasterPos4sv )(const GLshort *v);
GLEXTERN  void ( APIENTRY * qglReadBuffer )(GLenum mode);
GLEXTERN  void ( APIENTRY * qglReadPixels )(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
GLEXTERN  void ( APIENTRY * qglRectd )(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
GLEXTERN  void ( APIENTRY * qglRectdv )(const GLdouble *v1, const GLdouble *v2);
GLEXTERN  void ( APIENTRY * qglRectf )(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
GLEXTERN  void ( APIENTRY * qglRectfv )(const GLfloat *v1, const GLfloat *v2);
GLEXTERN  void ( APIENTRY * qglRecti )(GLint x1, GLint y1, GLint x2, GLint y2);
GLEXTERN  void ( APIENTRY * qglRectiv )(const GLint *v1, const GLint *v2);
GLEXTERN  void ( APIENTRY * qglRects )(GLshort x1, GLshort y1, GLshort x2, GLshort y2);
GLEXTERN  void ( APIENTRY * qglRectsv )(const GLshort *v1, const GLshort *v2);
GLEXTERN  GLint ( APIENTRY * qglRenderMode )(GLenum mode);
GLEXTERN  void ( APIENTRY * qglRotated )(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
GLEXTERN  void ( APIENTRY * qglRotatef )(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
GLEXTERN  void ( APIENTRY * qglScaled )(GLdouble x, GLdouble y, GLdouble z);
GLEXTERN  void ( APIENTRY * qglScalef )(GLfloat x, GLfloat y, GLfloat z);
GLEXTERN  void ( APIENTRY * qglScissor )(GLint x, GLint y, GLsizei width, GLsizei height);
GLEXTERN  void ( APIENTRY * qglSelectBuffer )(GLsizei size, GLuint *buffer);
GLEXTERN  void ( APIENTRY * qglShadeModel )(GLenum mode);
GLEXTERN  void ( APIENTRY * qglStencilFunc )(GLenum func, GLint ref, GLuint mask);
GLEXTERN  void ( APIENTRY * qglStencilMask )(GLuint mask);
GLEXTERN  void ( APIENTRY * qglStencilOp )(GLenum fail, GLenum zfail, GLenum zpass);
GLEXTERN  void ( APIENTRY * qglTexCoord1d )(GLdouble s);
GLEXTERN  void ( APIENTRY * qglTexCoord1dv )(const GLdouble *v);
GLEXTERN  void ( APIENTRY * qglTexCoord1f )(GLfloat s);
GLEXTERN  void ( APIENTRY * qglTexCoord1fv )(const GLfloat *v);
GLEXTERN  void ( APIENTRY * qglTexCoord1i )(GLint s);
GLEXTERN  void ( APIENTRY * qglTexCoord1iv )(const GLint *v);
GLEXTERN  void ( APIENTRY * qglTexCoord1s )(GLshort s);
GLEXTERN  void ( APIENTRY * qglTexCoord1sv )(const GLshort *v);
GLEXTERN  void ( APIENTRY * qglTexCoord2d )(GLdouble s, GLdouble t);
GLEXTERN  void ( APIENTRY * qglTexCoord2dv )(const GLdouble *v);
GLEXTERN  void ( APIENTRY * qglTexCoord2f )(GLfloat s, GLfloat t);
GLEXTERN  void ( APIENTRY * qglTexCoord2fv )(const GLfloat *v);
GLEXTERN  void ( APIENTRY * qglTexCoord2i )(GLint s, GLint t);
GLEXTERN  void ( APIENTRY * qglTexCoord2iv )(const GLint *v);
GLEXTERN  void ( APIENTRY * qglTexCoord2s )(GLshort s, GLshort t);
GLEXTERN  void ( APIENTRY * qglTexCoord2sv )(const GLshort *v);
GLEXTERN  void ( APIENTRY * qglTexCoord3d )(GLdouble s, GLdouble t, GLdouble r);
GLEXTERN  void ( APIENTRY * qglTexCoord3dv )(const GLdouble *v);
GLEXTERN  void ( APIENTRY * qglTexCoord3f )(GLfloat s, GLfloat t, GLfloat r);
GLEXTERN  void ( APIENTRY * qglTexCoord3fv )(const GLfloat *v);
GLEXTERN  void ( APIENTRY * qglTexCoord3i )(GLint s, GLint t, GLint r);
GLEXTERN  void ( APIENTRY * qglTexCoord3iv )(const GLint *v);
GLEXTERN  void ( APIENTRY * qglTexCoord3s )(GLshort s, GLshort t, GLshort r);
GLEXTERN  void ( APIENTRY * qglTexCoord3sv )(const GLshort *v);
GLEXTERN  void ( APIENTRY * qglTexCoord4d )(GLdouble s, GLdouble t, GLdouble r, GLdouble q);
GLEXTERN  void ( APIENTRY * qglTexCoord4dv )(const GLdouble *v);
GLEXTERN  void ( APIENTRY * qglTexCoord4f )(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
GLEXTERN  void ( APIENTRY * qglTexCoord4fv )(const GLfloat *v);
GLEXTERN  void ( APIENTRY * qglTexCoord4i )(GLint s, GLint t, GLint r, GLint q);
GLEXTERN  void ( APIENTRY * qglTexCoord4iv )(const GLint *v);
GLEXTERN  void ( APIENTRY * qglTexCoord4s )(GLshort s, GLshort t, GLshort r, GLshort q);
GLEXTERN  void ( APIENTRY * qglTexCoord4sv )(const GLshort *v);
GLEXTERN  void ( APIENTRY * qglTexCoordPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
GLEXTERN  void ( APIENTRY * qglTexEnvf )(GLenum target, GLenum pname, GLfloat param);
GLEXTERN  void ( APIENTRY * qglTexEnvfv )(GLenum target, GLenum pname, const GLfloat *params);
GLEXTERN  void ( APIENTRY * qglTexEnvi )(GLenum target, GLenum pname, GLint param);
GLEXTERN  void ( APIENTRY * qglTexEnviv )(GLenum target, GLenum pname, const GLint *params);
GLEXTERN  void ( APIENTRY * qglTexGend )(GLenum coord, GLenum pname, GLdouble param);
GLEXTERN  void ( APIENTRY * qglTexGendv )(GLenum coord, GLenum pname, const GLdouble *params);
GLEXTERN  void ( APIENTRY * qglTexGenf )(GLenum coord, GLenum pname, GLfloat param);
GLEXTERN  void ( APIENTRY * qglTexGenfv )(GLenum coord, GLenum pname, const GLfloat *params);
GLEXTERN  void ( APIENTRY * qglTexGeni )(GLenum coord, GLenum pname, GLint param);
GLEXTERN  void ( APIENTRY * qglTexGeniv )(GLenum coord, GLenum pname, const GLint *params);
GLEXTERN  void ( APIENTRY * qglTexImage1D )(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
GLEXTERN  void ( APIENTRY * qglTexImage2D )(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
GLEXTERN  void ( APIENTRY * qglTexParameterf )(GLenum target, GLenum pname, GLfloat param);
GLEXTERN  void ( APIENTRY * qglTexParameterfv )(GLenum target, GLenum pname, const GLfloat *params);
GLEXTERN  void ( APIENTRY * qglTexParameteri )(GLenum target, GLenum pname, GLint param);
GLEXTERN  void ( APIENTRY * qglTexParameteriv )(GLenum target, GLenum pname, const GLint *params);
GLEXTERN  void ( APIENTRY * qglTexSubImage1D )(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
GLEXTERN  void ( APIENTRY * qglTexSubImage2D )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
GLEXTERN  void ( APIENTRY * qglTranslated )(GLdouble x, GLdouble y, GLdouble z);
GLEXTERN  void ( APIENTRY * qglTranslatef )(GLfloat x, GLfloat y, GLfloat z);
GLEXTERN  void ( APIENTRY * qglVertex2d )(GLdouble x, GLdouble y);
GLEXTERN  void ( APIENTRY * qglVertex2dv )(const GLdouble *v);
GLEXTERN  void ( APIENTRY * qglVertex2f )(GLfloat x, GLfloat y);
GLEXTERN  void ( APIENTRY * qglVertex2fv )(const GLfloat *v);
GLEXTERN  void ( APIENTRY * qglVertex2i )(GLint x, GLint y);
GLEXTERN  void ( APIENTRY * qglVertex2iv )(const GLint *v);
GLEXTERN  void ( APIENTRY * qglVertex2s )(GLshort x, GLshort y);
GLEXTERN  void ( APIENTRY * qglVertex2sv )(const GLshort *v);
GLEXTERN  void ( APIENTRY * qglVertex3d )(GLdouble x, GLdouble y, GLdouble z);
GLEXTERN  void ( APIENTRY * qglVertex3dv )(const GLdouble *v);
GLEXTERN  void ( APIENTRY * qglVertex3f )(GLfloat x, GLfloat y, GLfloat z);
GLEXTERN  void ( APIENTRY * qglVertex3fv )(const GLfloat *v);
GLEXTERN  void ( APIENTRY * qglVertex3i )(GLint x, GLint y, GLint z);
GLEXTERN  void ( APIENTRY * qglVertex3iv )(const GLint *v);
GLEXTERN  void ( APIENTRY * qglVertex3s )(GLshort x, GLshort y, GLshort z);
GLEXTERN  void ( APIENTRY * qglVertex3sv )(const GLshort *v);
GLEXTERN  void ( APIENTRY * qglVertex4d )(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLEXTERN  void ( APIENTRY * qglVertex4dv )(const GLdouble *v);
GLEXTERN  void ( APIENTRY * qglVertex4f )(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLEXTERN  void ( APIENTRY * qglVertex4fv )(const GLfloat *v);
GLEXTERN  void ( APIENTRY * qglVertex4i )(GLint x, GLint y, GLint z, GLint w);
GLEXTERN  void ( APIENTRY * qglVertex4iv )(const GLint *v);
GLEXTERN  void ( APIENTRY * qglVertex4s )(GLshort x, GLshort y, GLshort z, GLshort w);
GLEXTERN  void ( APIENTRY * qglVertex4sv )(const GLshort *v);
GLEXTERN  void ( APIENTRY * qglVertexPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
GLEXTERN  void ( APIENTRY * qglViewport )(GLint x, GLint y, GLsizei width, GLsizei height);

#if defined( _WIN32 )

GLEXTERN  int   ( WINAPI * qwglChoosePixelFormat )(HDC, CONST PIXELFORMATDESCRIPTOR *);
GLEXTERN  int   ( WINAPI * qwglDescribePixelFormat) (HDC, int, UINT, LPPIXELFORMATDESCRIPTOR);
GLEXTERN  int   ( WINAPI * qwglGetPixelFormat)(HDC);
GLEXTERN  BOOL  ( WINAPI * qwglSetPixelFormat)(HDC, int, CONST PIXELFORMATDESCRIPTOR *);
GLEXTERN  BOOL  ( WINAPI * qwglSwapBuffers)(HDC);

GLEXTERN	BOOL  ( WINAPI * qwglGetDeviceGammaRamp3DFX)( HDC, LPVOID );
GLEXTERN	BOOL  ( WINAPI * qwglSetDeviceGammaRamp3DFX)( HDC, LPVOID );

GLEXTERN BOOL  ( WINAPI * qwglCopyContext)(HGLRC, HGLRC, UINT);
GLEXTERN HGLRC ( WINAPI * qwglCreateContext)(HDC);
GLEXTERN HGLRC ( WINAPI * qwglCreateLayerContext)(HDC, int);
GLEXTERN BOOL  ( WINAPI * qwglDeleteContext)(HGLRC);
GLEXTERN HGLRC ( WINAPI * qwglGetCurrentContext)(VOID);
GLEXTERN HDC   ( WINAPI * qwglGetCurrentDC)(VOID);
GLEXTERN PROC  ( WINAPI * qwglGetProcAddress)(LPCSTR);
GLEXTERN BOOL  ( WINAPI * qwglMakeCurrent)(HDC, HGLRC);
GLEXTERN BOOL  ( WINAPI * qwglShareLists)(HGLRC, HGLRC);
GLEXTERN BOOL  ( WINAPI * qwglUseFontBitmaps)(HDC, DWORD, DWORD, DWORD);

GLEXTERN BOOL  ( WINAPI * qwglUseFontOutlines)(HDC, DWORD, DWORD, DWORD, FLOAT,
                                           FLOAT, int, LPGLYPHMETRICSFLOAT);

GLEXTERN BOOL ( WINAPI * qwglDescribeLayerPlane)(HDC, int, int, UINT,
                                            LPLAYERPLANEDESCRIPTOR);
GLEXTERN int  ( WINAPI * qwglSetLayerPaletteEntries)(HDC, int, int, int,
                                                CONST COLORREF *);
GLEXTERN int  ( WINAPI * qwglGetLayerPaletteEntries)(HDC, int, int, int,
                                                COLORREF *);
GLEXTERN BOOL ( WINAPI * qwglRealizeLayerPalette)(HDC, int, BOOL);
GLEXTERN BOOL ( WINAPI * qwglSwapLayerBuffers)(HDC, UINT);

GLEXTERN BOOL ( WINAPI * qwglSwapIntervalEXT)( int interval );

#endif	// _WIN32

#if ( (defined __linux__ )  || (defined __FreeBSD__ ) ) // rb010123

//FX Mesa Functions
// bk001129 - from cvs1.17 (mkv)
#if defined (__FX__)
GLEXTERN fxMesaContext (*qfxMesaCreateContext)(GLuint win, GrScreenResolution_t, GrScreenRefresh_t, const GLint attribList[]);
GLEXTERN fxMesaContext (*qfxMesaCreateBestContext)(GLuint win, GLint width, GLint height, const GLint attribList[]);
GLEXTERN void (*qfxMesaDestroyContext)(fxMesaContext ctx);
GLEXTERN void (*qfxMesaMakeCurrent)(fxMesaContext ctx);
GLEXTERN fxMesaContext (*qfxMesaGetCurrentContext)(void);
GLEXTERN void (*qfxMesaSwapBuffers)(void);
#endif

//GLX Functions
GLEXTERN XVisualInfo * (*qglXChooseVisual)( Display *dpy, int screen, int *attribList );
GLEXTERN GLXContext (*qglXCreateContext)( Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct );
GLEXTERN void (*qglXDestroyContext)( Display *dpy, GLXContext ctx );
GLEXTERN Bool (*qglXMakeCurrent)( Display *dpy, GLXDrawable drawable, GLXContext ctx);
GLEXTERN void (*qglXCopyContext)( Display *dpy, GLXContext src, GLXContext dst, GLuint mask );
GLEXTERN void (*qglXSwapBuffers)( Display *dpy, GLXDrawable drawable );

#endif // __linux__ || __FreeBSD__ // rb010123

// #endif	// _WIN32 && __linux__

#endif
