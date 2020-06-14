#ifdef BGBRASW_DLL

#if !defined(GLAPI)
#if defined(_MSC_VER)
#define GLAPI __declspec(dllexport)
#else
#define GLAPI
#endif
#endif

#if !defined(GLDRV_API)
#if defined(_MSC_VER)
#define GLDRV_API __declspec(dllexport)
#else
#define GLDRV_API
#endif
#endif

#endif

#include <GL/gl.h>

#ifndef WINGDIAPI
typedef char BOOL;
typedef int INT;
typedef unsigned int UINT;
typedef unsigned int DWORD;
typedef float FLOAT;

typedef const char *LPCSTR;
typedef void *HDC;
typedef void *HGLRC;
typedef void *PROC;
typedef void *LPPIXELFORMATDESCRIPTOR;
typedef int PIXELFORMATDESCRIPTOR;
typedef int COLORREF;
typedef void *LPLAYERPLANEDESCRIPTOR;
typedef void *LPGLYPHMETRICSFLOAT;

#define CONST const

#endif

#define GLSTUB_APIFPTR	APIENTRY
#define GLDRV_APIENTRY	APIENTRY

typedef struct BGBRASW_GlStub_Funcs_s BGBRASW_GlStub_Funcs;

struct BGBRASW_GlStub_Funcs_s
{
void (GLSTUB_APIFPTR *glClearIndex)(GLfloat c);
void (GLSTUB_APIFPTR *glClearColor)(
	GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void (GLSTUB_APIFPTR *glClear)(GLbitfield mask);
void (GLSTUB_APIFPTR *glIndexMask)(GLuint mask);
void (GLSTUB_APIFPTR *glColorMask)(
	GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
void (GLSTUB_APIFPTR *glAlphaFunc)(GLenum func, GLclampf ref);
void (GLSTUB_APIFPTR *glBlendFunc)(GLenum sfactor, GLenum dfactor);
void (GLSTUB_APIFPTR *glLogicOp)(GLenum opcode);
void (GLSTUB_APIFPTR *glCullFace)(GLenum mode);
void (GLSTUB_APIFPTR *glFrontFace)(GLenum mode);
void (GLSTUB_APIFPTR *glPointSize)(GLfloat size);
void (GLSTUB_APIFPTR *glLineWidth)(GLfloat width);
void (GLSTUB_APIFPTR *glLineStipple)(GLint factor, GLushort pattern);
void (GLSTUB_APIFPTR *glPolygonMode)(GLenum face, GLenum mode);
void (GLSTUB_APIFPTR *glPolygonOffset)(GLfloat factor, GLfloat units);
void (GLSTUB_APIFPTR *glPolygonStipple)(const GLubyte *mask);
void (GLSTUB_APIFPTR *glGetPolygonStipple)(GLubyte *mask);
void (GLSTUB_APIFPTR *glEdgeFlag)(GLboolean flag);
void (GLSTUB_APIFPTR *glEdgeFlagv)(const GLboolean *flag);
void (GLSTUB_APIFPTR *glScissor)(GLint x, GLint y, GLsizei width, GLsizei height);
void (GLSTUB_APIFPTR *glClipPlane)(GLenum plane, const GLdouble *equation);
void (GLSTUB_APIFPTR *glGetClipPlane)(GLenum plane, GLdouble *equation);
void (GLSTUB_APIFPTR *glDrawBuffer)(GLenum mode);
void (GLSTUB_APIFPTR *glReadBuffer)(GLenum mode);
void (GLSTUB_APIFPTR *glEnable)(GLenum cap);
void (GLSTUB_APIFPTR *glDisable)(GLenum cap);
GLboolean (GLSTUB_APIFPTR *glIsEnabled)(GLenum cap);
void (GLSTUB_APIFPTR *glEnableClientState)(GLenum cap);  /* 1.1 */
void (GLSTUB_APIFPTR *glDisableClientState)(GLenum cap);  /* 1.1 */
void (GLSTUB_APIFPTR *glGetBooleanv)(GLenum pname, GLboolean *params);
void (GLSTUB_APIFPTR *glGetDoublev)(GLenum pname, GLdouble *params);
void (GLSTUB_APIFPTR *glGetFloatv)(GLenum pname, GLfloat *params);
void (GLSTUB_APIFPTR *glGetIntegerv)(GLenum pname, GLint *params);
void (GLSTUB_APIFPTR *glPushAttrib)(GLbitfield mask);
void (GLSTUB_APIFPTR *glPopAttrib)(void);
void (GLSTUB_APIFPTR *glPushClientAttrib)(GLbitfield mask);  /* 1.1 */
void (GLSTUB_APIFPTR *glPopClientAttrib)(void);  /* 1.1 */
GLint (GLSTUB_APIFPTR *glRenderMode)(GLenum mode);
GLenum (GLSTUB_APIFPTR *glGetError)(void);
const GLubyte* (GLSTUB_APIFPTR *glGetString)(GLenum name);
void (GLSTUB_APIFPTR *glFinish)(void);
void (GLSTUB_APIFPTR *glFlush)(void);
void (GLSTUB_APIFPTR *glHint)(GLenum target, GLenum mode);

/* Depth Buffer */
void (GLSTUB_APIFPTR *glClearDepth)(GLclampd depth);
void (GLSTUB_APIFPTR *glDepthFunc)(GLenum func);
void (GLSTUB_APIFPTR *glDepthMask)(GLboolean flag);
void (GLSTUB_APIFPTR *glDepthRange)(GLclampd near_val, GLclampd far_val);

/* Accumulation Buffer */
void (GLSTUB_APIFPTR *glClearAccum)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void (GLSTUB_APIFPTR *glAccum)(GLenum op, GLfloat value);

/* Transformation */
void (GLSTUB_APIFPTR *glMatrixMode)(GLenum mode);
void (GLSTUB_APIFPTR *glOrtho)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near_val, GLdouble far_val);
void (GLSTUB_APIFPTR *glFrustum)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near_val, GLdouble far_val);
void (GLSTUB_APIFPTR *glViewport)(GLint x, GLint y, GLsizei width, GLsizei height);
void (GLSTUB_APIFPTR *glPushMatrix)(void);
void (GLSTUB_APIFPTR *glPopMatrix)(void);
void (GLSTUB_APIFPTR *glLoadIdentity)(void);
void (GLSTUB_APIFPTR *glLoadMatrixd)(const GLdouble *m);
void (GLSTUB_APIFPTR *glLoadMatrixf)(const GLfloat *m);
void (GLSTUB_APIFPTR *glMultMatrixd)(const GLdouble *m);
void (GLSTUB_APIFPTR *glMultMatrixf)(const GLfloat *m);
void (GLSTUB_APIFPTR *glRotated)(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
void (GLSTUB_APIFPTR *glRotatef)(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
void (GLSTUB_APIFPTR *glScaled)(GLdouble x, GLdouble y, GLdouble z);
void (GLSTUB_APIFPTR *glScalef)(GLfloat x, GLfloat y, GLfloat z);
void (GLSTUB_APIFPTR *glTranslated)(GLdouble x, GLdouble y, GLdouble z);
void (GLSTUB_APIFPTR *glTranslatef)(GLfloat x, GLfloat y, GLfloat z);

/* Display Lists */
GLboolean (GLSTUB_APIFPTR *glIsList)(GLuint list);
void (GLSTUB_APIFPTR *glDeleteLists)(GLuint list, GLsizei range);
GLuint (GLSTUB_APIFPTR *glGenLists)(GLsizei range);
void (GLSTUB_APIFPTR *glNewList)(GLuint list, GLenum mode);
void (GLSTUB_APIFPTR *glEndList)(void);
void (GLSTUB_APIFPTR *glCallList)(GLuint list);
void (GLSTUB_APIFPTR *glCallLists)(GLsizei n, GLenum type, const GLvoid *lists);
void (GLSTUB_APIFPTR *glListBase)(GLuint base);

/* Drawing Functions */
void (GLSTUB_APIFPTR *glBegin)(GLenum mode);
void (GLSTUB_APIFPTR *glEnd)(void);
void (GLSTUB_APIFPTR *glVertex2d)(GLdouble x, GLdouble y);
void (GLSTUB_APIFPTR *glVertex2f)(GLfloat x, GLfloat y);
void (GLSTUB_APIFPTR *glVertex2i)(GLint x, GLint y);
void (GLSTUB_APIFPTR *glVertex2s)(GLshort x, GLshort y);
void (GLSTUB_APIFPTR *glVertex3d)(GLdouble x, GLdouble y, GLdouble z);
void (GLSTUB_APIFPTR *glVertex3f)(GLfloat x, GLfloat y, GLfloat z);
void (GLSTUB_APIFPTR *glVertex3i)(GLint x, GLint y, GLint z);
void (GLSTUB_APIFPTR *glVertex3s)(GLshort x, GLshort y, GLshort z);
void (GLSTUB_APIFPTR *glVertex4d)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void (GLSTUB_APIFPTR *glVertex4f)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void (GLSTUB_APIFPTR *glVertex4i)(GLint x, GLint y, GLint z, GLint w);
void (GLSTUB_APIFPTR *glVertex4s)(GLshort x, GLshort y, GLshort z, GLshort w);
void (GLSTUB_APIFPTR *glVertex2dv)(const GLdouble *v);
void (GLSTUB_APIFPTR *glVertex2fv)(const GLfloat *v);
void (GLSTUB_APIFPTR *glVertex2iv)(const GLint *v);
void (GLSTUB_APIFPTR *glVertex2sv)(const GLshort *v);
void (GLSTUB_APIFPTR *glVertex3dv)(const GLdouble *v);
void (GLSTUB_APIFPTR *glVertex3fv)(const GLfloat *v);
void (GLSTUB_APIFPTR *glVertex3iv)(const GLint *v);
void (GLSTUB_APIFPTR *glVertex3sv)(const GLshort *v);
void (GLSTUB_APIFPTR *glVertex4dv)(const GLdouble *v);
void (GLSTUB_APIFPTR *glVertex4fv)(const GLfloat *v);
void (GLSTUB_APIFPTR *glVertex4iv)(const GLint *v);
void (GLSTUB_APIFPTR *glVertex4sv)(const GLshort *v);
void (GLSTUB_APIFPTR *glNormal3b)(GLbyte nx, GLbyte ny, GLbyte nz);
void (GLSTUB_APIFPTR *glNormal3d)(GLdouble nx, GLdouble ny, GLdouble nz);
void (GLSTUB_APIFPTR *glNormal3f)(GLfloat nx, GLfloat ny, GLfloat nz);
void (GLSTUB_APIFPTR *glNormal3i)(GLint nx, GLint ny, GLint nz);
void (GLSTUB_APIFPTR *glNormal3s)(GLshort nx, GLshort ny, GLshort nz);
void (GLSTUB_APIFPTR *glNormal3bv)(const GLbyte *v);
void (GLSTUB_APIFPTR *glNormal3dv)(const GLdouble *v);
void (GLSTUB_APIFPTR *glNormal3fv)(const GLfloat *v);
void (GLSTUB_APIFPTR *glNormal3iv)(const GLint *v);
void (GLSTUB_APIFPTR *glNormal3sv)(const GLshort *v);
void (GLSTUB_APIFPTR *glIndexd)(GLdouble c);
void (GLSTUB_APIFPTR *glIndexf)(GLfloat c);
void (GLSTUB_APIFPTR *glIndexi)(GLint c);
void (GLSTUB_APIFPTR *glIndexs)(GLshort c);
void (GLSTUB_APIFPTR *glIndexub)(GLubyte c);  /* 1.1 */
void (GLSTUB_APIFPTR *glIndexdv)(const GLdouble *c);
void (GLSTUB_APIFPTR *glIndexfv)(const GLfloat *c);
void (GLSTUB_APIFPTR *glIndexiv)(const GLint *c);
void (GLSTUB_APIFPTR *glIndexsv)(const GLshort *c);
void (GLSTUB_APIFPTR *glIndexubv)(const GLubyte *c);  /* 1.1 */
void (GLSTUB_APIFPTR *glColor3b)(GLbyte red, GLbyte green, GLbyte blue);
void (GLSTUB_APIFPTR *glColor3d)(GLdouble red, GLdouble green, GLdouble blue);
void (GLSTUB_APIFPTR *glColor3f)(GLfloat red, GLfloat green, GLfloat blue);
void (GLSTUB_APIFPTR *glColor3i)(GLint red, GLint green, GLint blue);
void (GLSTUB_APIFPTR *glColor3s)(GLshort red, GLshort green, GLshort blue);
void (GLSTUB_APIFPTR *glColor3ub)(GLubyte red, GLubyte green, GLubyte blue);
void (GLSTUB_APIFPTR *glColor3ui)(GLuint red, GLuint green, GLuint blue);
void (GLSTUB_APIFPTR *glColor3us)(GLushort red, GLushort green, GLushort blue);
void (GLSTUB_APIFPTR *glColor4b)(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
void (GLSTUB_APIFPTR *glColor4d)(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
void (GLSTUB_APIFPTR *glColor4f)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void (GLSTUB_APIFPTR *glColor4i)(GLint red, GLint green, GLint blue, GLint alpha);
void (GLSTUB_APIFPTR *glColor4s)(GLshort red, GLshort green, GLshort blue, GLshort alpha);
void (GLSTUB_APIFPTR *glColor4ub)(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
void (GLSTUB_APIFPTR *glColor4ui)(GLuint red, GLuint green, GLuint blue, GLuint alpha);
void (GLSTUB_APIFPTR *glColor4us)(GLushort red, GLushort green, GLushort blue, GLushort alpha);
void (GLSTUB_APIFPTR *glColor3bv)(const GLbyte *v);
void (GLSTUB_APIFPTR *glColor3dv)(const GLdouble *v);
void (GLSTUB_APIFPTR *glColor3fv)(const GLfloat *v);
void (GLSTUB_APIFPTR *glColor3iv)(const GLint *v);
void (GLSTUB_APIFPTR *glColor3sv)(const GLshort *v);
void (GLSTUB_APIFPTR *glColor3ubv)(const GLubyte *v);
void (GLSTUB_APIFPTR *glColor3uiv)(const GLuint *v);
void (GLSTUB_APIFPTR *glColor3usv)(const GLushort *v);
void (GLSTUB_APIFPTR *glColor4bv)(const GLbyte *v);
void (GLSTUB_APIFPTR *glColor4dv)(const GLdouble *v);
void (GLSTUB_APIFPTR *glColor4fv)(const GLfloat *v);
void (GLSTUB_APIFPTR *glColor4iv)(const GLint *v);
void (GLSTUB_APIFPTR *glColor4sv)(const GLshort *v);
void (GLSTUB_APIFPTR *glColor4ubv)(const GLubyte *v);
void (GLSTUB_APIFPTR *glColor4uiv)(const GLuint *v);
void (GLSTUB_APIFPTR *glColor4usv)(const GLushort *v);
void (GLSTUB_APIFPTR *glTexCoord1d)(GLdouble s);
void (GLSTUB_APIFPTR *glTexCoord1f)(GLfloat s);
void (GLSTUB_APIFPTR *glTexCoord1i)(GLint s);
void (GLSTUB_APIFPTR *glTexCoord1s)(GLshort s);
void (GLSTUB_APIFPTR *glTexCoord2d)(GLdouble s, GLdouble t);
void (GLSTUB_APIFPTR *glTexCoord2f)(GLfloat s, GLfloat t);
void (GLSTUB_APIFPTR *glTexCoord2i)(GLint s, GLint t);
void (GLSTUB_APIFPTR *glTexCoord2s)(GLshort s, GLshort t);
void (GLSTUB_APIFPTR *glTexCoord3d)(GLdouble s, GLdouble t, GLdouble r);
void (GLSTUB_APIFPTR *glTexCoord3f)(GLfloat s, GLfloat t, GLfloat r);
void (GLSTUB_APIFPTR *glTexCoord3i)(GLint s, GLint t, GLint r);
void (GLSTUB_APIFPTR *glTexCoord3s)(GLshort s, GLshort t, GLshort r);
void (GLSTUB_APIFPTR *glTexCoord4d)(GLdouble s, GLdouble t, GLdouble r, GLdouble q);
void (GLSTUB_APIFPTR *glTexCoord4f)(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
void (GLSTUB_APIFPTR *glTexCoord4i)(GLint s, GLint t, GLint r, GLint q);
void (GLSTUB_APIFPTR *glTexCoord4s)(GLshort s, GLshort t, GLshort r, GLshort q);
void (GLSTUB_APIFPTR *glTexCoord1dv)(const GLdouble *v);
void (GLSTUB_APIFPTR *glTexCoord1fv)(const GLfloat *v);
void (GLSTUB_APIFPTR *glTexCoord1iv)(const GLint *v);
void (GLSTUB_APIFPTR *glTexCoord1sv)(const GLshort *v);
void (GLSTUB_APIFPTR *glTexCoord2dv)(const GLdouble *v);
void (GLSTUB_APIFPTR *glTexCoord2fv)(const GLfloat *v);
void (GLSTUB_APIFPTR *glTexCoord2iv)(const GLint *v);
void (GLSTUB_APIFPTR *glTexCoord2sv)(const GLshort *v);
void (GLSTUB_APIFPTR *glTexCoord3dv)(const GLdouble *v);
void (GLSTUB_APIFPTR *glTexCoord3fv)(const GLfloat *v);
void (GLSTUB_APIFPTR *glTexCoord3iv)(const GLint *v);
void (GLSTUB_APIFPTR *glTexCoord3sv)(const GLshort *v);
void (GLSTUB_APIFPTR *glTexCoord4dv)(const GLdouble *v);
void (GLSTUB_APIFPTR *glTexCoord4fv)(const GLfloat *v);
void (GLSTUB_APIFPTR *glTexCoord4iv)(const GLint *v);
void (GLSTUB_APIFPTR *glTexCoord4sv)(const GLshort *v);
void (GLSTUB_APIFPTR *glRasterPos2d)(GLdouble x, GLdouble y);
void (GLSTUB_APIFPTR *glRasterPos2f)(GLfloat x, GLfloat y);
void (GLSTUB_APIFPTR *glRasterPos2i)(GLint x, GLint y);
void (GLSTUB_APIFPTR *glRasterPos2s)(GLshort x, GLshort y);
void (GLSTUB_APIFPTR *glRasterPos3d)(GLdouble x, GLdouble y, GLdouble z);
void (GLSTUB_APIFPTR *glRasterPos3f)(GLfloat x, GLfloat y, GLfloat z);
void (GLSTUB_APIFPTR *glRasterPos3i)(GLint x, GLint y, GLint z);
void (GLSTUB_APIFPTR *glRasterPos3s)(GLshort x, GLshort y, GLshort z);
void (GLSTUB_APIFPTR *glRasterPos4d)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void (GLSTUB_APIFPTR *glRasterPos4f)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void (GLSTUB_APIFPTR *glRasterPos4i)(GLint x, GLint y, GLint z, GLint w);
void (GLSTUB_APIFPTR *glRasterPos4s)(GLshort x, GLshort y, GLshort z, GLshort w);
void (GLSTUB_APIFPTR *glRasterPos2dv)(const GLdouble *v);
void (GLSTUB_APIFPTR *glRasterPos2fv)(const GLfloat *v);
void (GLSTUB_APIFPTR *glRasterPos2iv)(const GLint *v);
void (GLSTUB_APIFPTR *glRasterPos2sv)(const GLshort *v);
void (GLSTUB_APIFPTR *glRasterPos3dv)(const GLdouble *v);
void (GLSTUB_APIFPTR *glRasterPos3fv)(const GLfloat *v);
void (GLSTUB_APIFPTR *glRasterPos3iv)(const GLint *v);
void (GLSTUB_APIFPTR *glRasterPos3sv)(const GLshort *v);
void (GLSTUB_APIFPTR *glRasterPos4dv)(const GLdouble *v);
void (GLSTUB_APIFPTR *glRasterPos4fv)(const GLfloat *v);
void (GLSTUB_APIFPTR *glRasterPos4iv)(const GLint *v);
void (GLSTUB_APIFPTR *glRasterPos4sv)(const GLshort *v);
void (GLSTUB_APIFPTR *glRectd)(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
void (GLSTUB_APIFPTR *glRectf)(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
void (GLSTUB_APIFPTR *glRecti)(GLint x1, GLint y1, GLint x2, GLint y2);
void (GLSTUB_APIFPTR *glRects)(GLshort x1, GLshort y1, GLshort x2, GLshort y2);
void (GLSTUB_APIFPTR *glRectdv)(const GLdouble *v1, const GLdouble *v2);
void (GLSTUB_APIFPTR *glRectfv)(const GLfloat *v1, const GLfloat *v2);
void (GLSTUB_APIFPTR *glRectiv)(const GLint *v1, const GLint *v2);
void (GLSTUB_APIFPTR *glRectsv)(const GLshort *v1, const GLshort *v2);

/* Lighting */
void (GLSTUB_APIFPTR *glShadeModel)(GLenum mode);
void (GLSTUB_APIFPTR *glLightf)(GLenum light, GLenum pname, GLfloat param);
void (GLSTUB_APIFPTR *glLighti)(GLenum light, GLenum pname, GLint param);
void (GLSTUB_APIFPTR *glLightfv)(GLenum light, GLenum pname, const GLfloat *params);
void (GLSTUB_APIFPTR *glLightiv)(GLenum light, GLenum pname, const GLint *params);
void (GLSTUB_APIFPTR *glGetLightfv)(GLenum light, GLenum pname, GLfloat *params);
void (GLSTUB_APIFPTR *glGetLightiv)(GLenum light, GLenum pname, GLint *params);
void (GLSTUB_APIFPTR *glLightModelf)(GLenum pname, GLfloat param);
void (GLSTUB_APIFPTR *glLightModeli)(GLenum pname, GLint param);
void (GLSTUB_APIFPTR *glLightModelfv)(GLenum pname, const GLfloat *params);
void (GLSTUB_APIFPTR *glLightModeliv)(GLenum pname, const GLint *params);
void (GLSTUB_APIFPTR *glMaterialf)(GLenum face, GLenum pname, GLfloat param);
void (GLSTUB_APIFPTR *glMateriali)(GLenum face, GLenum pname, GLint param);
void (GLSTUB_APIFPTR *glMaterialfv)(GLenum face, GLenum pname, const GLfloat *params);
void (GLSTUB_APIFPTR *glMaterialiv)(GLenum face, GLenum pname, const GLint *params);
void (GLSTUB_APIFPTR *glGetMaterialfv)(GLenum face, GLenum pname, GLfloat *params);
void (GLSTUB_APIFPTR *glGetMaterialiv)(GLenum face, GLenum pname, GLint *params);
void (GLSTUB_APIFPTR *glColorMaterial)(GLenum face, GLenum mode);

/* Raster functions */
void (GLSTUB_APIFPTR *glPixelZoom)(GLfloat xfactor, GLfloat yfactor);
void (GLSTUB_APIFPTR *glPixelStoref)(GLenum pname, GLfloat param);
void (GLSTUB_APIFPTR *glPixelStorei)(GLenum pname, GLint param);
void (GLSTUB_APIFPTR *glPixelTransferf)(GLenum pname, GLfloat param);
void (GLSTUB_APIFPTR *glPixelTransferi)(GLenum pname, GLint param);
void (GLSTUB_APIFPTR *glPixelMapfv)(GLenum map, GLint mapsize, const GLfloat *values);
void (GLSTUB_APIFPTR *glPixelMapuiv)(GLenum map, GLint mapsize, const GLuint *values);
void (GLSTUB_APIFPTR *glPixelMapusv)(GLenum map, GLint mapsize, const GLushort *values);
void (GLSTUB_APIFPTR *glGetPixelMapfv)(GLenum map, GLfloat *values);
void (GLSTUB_APIFPTR *glGetPixelMapuiv)(GLenum map, GLuint *values);
void (GLSTUB_APIFPTR *glGetPixelMapusv)(GLenum map, GLushort *values);
void (GLSTUB_APIFPTR *glBitmap)(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
void (GLSTUB_APIFPTR *glReadPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
void (GLSTUB_APIFPTR *glDrawPixels)(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
void (GLSTUB_APIFPTR *glCopyPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);

/* Stenciling */
void (GLSTUB_APIFPTR *glStencilFunc)(GLenum func, GLint ref, GLuint mask);
void (GLSTUB_APIFPTR *glStencilMask)(GLuint mask);
void (GLSTUB_APIFPTR *glStencilOp)(GLenum fail, GLenum zfail, GLenum zpass);
void (GLSTUB_APIFPTR *glClearStencil)(GLint s);

/* Texture mapping */
void (GLSTUB_APIFPTR *glTexGend)(GLenum coord, GLenum pname, GLdouble param);
void (GLSTUB_APIFPTR *glTexGenf)(GLenum coord, GLenum pname, GLfloat param);
void (GLSTUB_APIFPTR *glTexGeni)(GLenum coord, GLenum pname, GLint param);
void (GLSTUB_APIFPTR *glTexGendv)(GLenum coord, GLenum pname, const GLdouble *params);
void (GLSTUB_APIFPTR *glTexGenfv)(GLenum coord, GLenum pname, const GLfloat *params);
void (GLSTUB_APIFPTR *glTexGeniv)(GLenum coord, GLenum pname, const GLint *params);
void (GLSTUB_APIFPTR *glGetTexGendv)(GLenum coord, GLenum pname, GLdouble *params);
void (GLSTUB_APIFPTR *glGetTexGenfv)(GLenum coord, GLenum pname, GLfloat *params);
void (GLSTUB_APIFPTR *glGetTexGeniv)(GLenum coord, GLenum pname, GLint *params);
void (GLSTUB_APIFPTR *glTexEnvf)(GLenum target, GLenum pname, GLfloat param);
void (GLSTUB_APIFPTR *glTexEnvi)(GLenum target, GLenum pname, GLint param);
void (GLSTUB_APIFPTR *glTexEnvfv)(GLenum target, GLenum pname, const GLfloat *params);
void (GLSTUB_APIFPTR *glTexEnviv)(GLenum target, GLenum pname, const GLint *params);
void (GLSTUB_APIFPTR *glGetTexEnvfv)(GLenum target, GLenum pname, GLfloat *params);
void (GLSTUB_APIFPTR *glGetTexEnviv)(GLenum target, GLenum pname, GLint *params);
void (GLSTUB_APIFPTR *glTexParameterf)(GLenum target, GLenum pname, GLfloat param);
void (GLSTUB_APIFPTR *glTexParameteri)(GLenum target, GLenum pname, GLint param);
void (GLSTUB_APIFPTR *glTexParameterfv)(GLenum target, GLenum pname, const GLfloat *params);
void (GLSTUB_APIFPTR *glTexParameteriv)(GLenum target, GLenum pname, const GLint *params);
void (GLSTUB_APIFPTR *glGetTexParameterfv)(GLenum target, GLenum pname, GLfloat *params);
void (GLSTUB_APIFPTR *glGetTexParameteriv)(GLenum target, GLenum pname, GLint *params);
void (GLSTUB_APIFPTR *glGetTexLevelParameterfv)(GLenum target, GLint level, GLenum pname, GLfloat *params);
void (GLSTUB_APIFPTR *glGetTexLevelParameteriv)(GLenum target, GLint level, GLenum pname, GLint *params);
void (GLSTUB_APIFPTR *glTexImage1D)(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void (GLSTUB_APIFPTR *glTexImage2D)(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void (GLSTUB_APIFPTR *glGetTexImage)(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);

/* Evaluators */
void (GLSTUB_APIFPTR *glMap1d)(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
void (GLSTUB_APIFPTR *glMap1f)(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
void (GLSTUB_APIFPTR *glMap2d)(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
void (GLSTUB_APIFPTR *glMap2f)(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
void (GLSTUB_APIFPTR *glGetMapdv)(GLenum target, GLenum query, GLdouble *v);
void (GLSTUB_APIFPTR *glGetMapfv)(GLenum target, GLenum query, GLfloat *v);
void (GLSTUB_APIFPTR *glGetMapiv)(GLenum target, GLenum query, GLint *v);
void (GLSTUB_APIFPTR *glEvalCoord1d)(GLdouble u);
void (GLSTUB_APIFPTR *glEvalCoord1f)(GLfloat u);
void (GLSTUB_APIFPTR *glEvalCoord1dv)(const GLdouble *u);
void (GLSTUB_APIFPTR *glEvalCoord1fv)(const GLfloat *u);
void (GLSTUB_APIFPTR *glEvalCoord2d)(GLdouble u, GLdouble v);
void (GLSTUB_APIFPTR *glEvalCoord2f)(GLfloat u, GLfloat v);
void (GLSTUB_APIFPTR *glEvalCoord2dv)(const GLdouble *u);
void (GLSTUB_APIFPTR *glEvalCoord2fv)(const GLfloat *u);
void (GLSTUB_APIFPTR *glMapGrid1d)(GLint un, GLdouble u1, GLdouble u2);
void (GLSTUB_APIFPTR *glMapGrid1f)(GLint un, GLfloat u1, GLfloat u2);
void (GLSTUB_APIFPTR *glMapGrid2d)(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
void (GLSTUB_APIFPTR *glMapGrid2f)(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
void (GLSTUB_APIFPTR *glEvalPoint1)(GLint i);
void (GLSTUB_APIFPTR *glEvalPoint2)(GLint i, GLint j);
void (GLSTUB_APIFPTR *glEvalMesh1)(GLenum mode, GLint i1, GLint i2);
void (GLSTUB_APIFPTR *glEvalMesh2)(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);

/* Fog */
void (GLSTUB_APIFPTR *glFogf)(GLenum pname, GLfloat param);
void (GLSTUB_APIFPTR *glFogi)(GLenum pname, GLint param);
void (GLSTUB_APIFPTR *glFogfv)(GLenum pname, const GLfloat *params);
void (GLSTUB_APIFPTR *glFogiv)(GLenum pname, const GLint *params);

/* Selection and Feedback */
void (GLSTUB_APIFPTR *glFeedbackBuffer)(GLsizei size, GLenum type, GLfloat *buffer);
void (GLSTUB_APIFPTR *glPassThrough)(GLfloat token);
void (GLSTUB_APIFPTR *glSelectBuffer)(GLsizei size, GLuint *buffer);
void (GLSTUB_APIFPTR *glInitNames)(void);
void (GLSTUB_APIFPTR *glLoadName)(GLuint name);
void (GLSTUB_APIFPTR *glPushName)(GLuint name);
void (GLSTUB_APIFPTR *glPopName)(void);


/* 1.1 functions */
/* texture objects */
void (GLSTUB_APIFPTR *glGenTextures)(GLsizei n, GLuint *textures);
void (GLSTUB_APIFPTR *glDeleteTextures)(GLsizei n, const GLuint *textures);
void (GLSTUB_APIFPTR *glBindTexture)(GLenum target, GLuint texture);
void (GLSTUB_APIFPTR *glPrioritizeTextures)(GLsizei n, const GLuint *textures, const GLclampf *priorities);
GLboolean (GLSTUB_APIFPTR *glAreTexturesResident)(GLsizei n, const GLuint *textures, GLboolean *residences);
GLboolean (GLSTUB_APIFPTR *glIsTexture)(GLuint texture);
/* texture mapping */
void (GLSTUB_APIFPTR *glTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
void (GLSTUB_APIFPTR *glTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
void (GLSTUB_APIFPTR *glCopyTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
void (GLSTUB_APIFPTR *glCopyTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void (GLSTUB_APIFPTR *glCopyTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
void (GLSTUB_APIFPTR *glCopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
/* vertex arrays */
void (GLSTUB_APIFPTR *glVertexPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *ptr);
void (GLSTUB_APIFPTR *glNormalPointer)(GLenum type, GLsizei stride, const GLvoid *ptr);
void (GLSTUB_APIFPTR *glColorPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *ptr);
void (GLSTUB_APIFPTR *glIndexPointer)(GLenum type, GLsizei stride, const GLvoid *ptr);
void (GLSTUB_APIFPTR *glTexCoordPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *ptr);
void (GLSTUB_APIFPTR *glEdgeFlagPointer)(GLsizei stride, const GLvoid *ptr);
void (GLSTUB_APIFPTR *glGetPointerv)(GLenum pname, GLvoid **params);
void (GLSTUB_APIFPTR *glArrayElement)(GLint i);
void (GLSTUB_APIFPTR *glDrawArrays)(GLenum mode, GLint first, GLsizei count);
void (GLSTUB_APIFPTR *glDrawElements)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
void (GLSTUB_APIFPTR *glInterleavedArrays)(GLenum format, GLsizei stride, const GLvoid *pointer);



/* Under Windows, we do not define OpenGL 1.2 & 1.3 functionality, since
   it is treated as extensions (defined in glext.h) */
// #if !defined(__WIN32__)

/* 1.2 functions */
void (GLSTUB_APIFPTR *glDrawRangeElements)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
void (GLSTUB_APIFPTR *glTexImage3D)(GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void (GLSTUB_APIFPTR *glTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
void (GLSTUB_APIFPTR *glCopyTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

/* 1.2 imaging extension functions */
void (GLSTUB_APIFPTR *glColorTable)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
void (GLSTUB_APIFPTR *glColorSubTable)(GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data);
void (GLSTUB_APIFPTR *glColorTableParameteriv)(GLenum target, GLenum pname, const GLint *params);
void (GLSTUB_APIFPTR *glColorTableParameterfv)(GLenum target, GLenum pname, const GLfloat *params);
void (GLSTUB_APIFPTR *glCopyColorSubTable)(GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);
void (GLSTUB_APIFPTR *glCopyColorTable)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
void (GLSTUB_APIFPTR *glGetColorTable)(GLenum target, GLenum format, GLenum type, GLvoid *table);
void (GLSTUB_APIFPTR *glGetColorTableParameterfv)(GLenum target, GLenum pname, GLfloat *params);
void (GLSTUB_APIFPTR *glGetColorTableParameteriv)(GLenum target, GLenum pname, GLint *params);
void (GLSTUB_APIFPTR *glBlendEquation)(GLenum mode);
void (GLSTUB_APIFPTR *glBlendColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void (GLSTUB_APIFPTR *glHistogram)(GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
void (GLSTUB_APIFPTR *glResetHistogram)(GLenum target);
void (GLSTUB_APIFPTR *glGetHistogram)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
void (GLSTUB_APIFPTR *glGetHistogramParameterfv)(GLenum target, GLenum pname, GLfloat *params);
void (GLSTUB_APIFPTR *glGetHistogramParameteriv)(GLenum target, GLenum pname, GLint *params);
void (GLSTUB_APIFPTR *glMinmax)(GLenum target, GLenum internalformat, GLboolean sink);
void (GLSTUB_APIFPTR *glResetMinmax)(GLenum target);
void (GLSTUB_APIFPTR *glGetMinmax)(GLenum target, GLboolean reset, GLenum format, GLenum types, GLvoid *values);
void (GLSTUB_APIFPTR *glGetMinmaxParameterfv)(GLenum target, GLenum pname, GLfloat *params);
void (GLSTUB_APIFPTR *glGetMinmaxParameteriv)(GLenum target, GLenum pname, GLint *params);
void (GLSTUB_APIFPTR *glConvolutionFilter1D)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image);
void (GLSTUB_APIFPTR *glConvolutionFilter2D)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image);
void (GLSTUB_APIFPTR *glConvolutionParameterf)(GLenum target, GLenum pname, GLfloat params);
void (GLSTUB_APIFPTR *glConvolutionParameterfv)(GLenum target, GLenum pname, const GLfloat *params);
void (GLSTUB_APIFPTR *glConvolutionParameteri)(GLenum target, GLenum pname, GLint params);
void (GLSTUB_APIFPTR *glConvolutionParameteriv)(GLenum target, GLenum pname, const GLint *params);
void (GLSTUB_APIFPTR *glCopyConvolutionFilter1D)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
void (GLSTUB_APIFPTR *glCopyConvolutionFilter2D)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
void (GLSTUB_APIFPTR *glGetConvolutionFilter)(GLenum target, GLenum format, GLenum type, GLvoid *image);
void (GLSTUB_APIFPTR *glGetConvolutionParameterfv)(GLenum target, GLenum pname, GLfloat *params);
void (GLSTUB_APIFPTR *glGetConvolutionParameteriv)(GLenum target, GLenum pname, GLint *params);
void (GLSTUB_APIFPTR *glSeparableFilter2D)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column);
void (GLSTUB_APIFPTR *glGetSeparableFilter)(GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span);

/* 1.3 functions */
void (GLSTUB_APIFPTR *glActiveTexture)(GLenum texture);
void (GLSTUB_APIFPTR *glClientActiveTexture)(GLenum texture);
void (GLSTUB_APIFPTR *glCompressedTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data);
void (GLSTUB_APIFPTR *glCompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
void (GLSTUB_APIFPTR *glCompressedTexImage3D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
void (GLSTUB_APIFPTR *glCompressedTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data);
void (GLSTUB_APIFPTR *glCompressedTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
void (GLSTUB_APIFPTR *glCompressedTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
void (GLSTUB_APIFPTR *glGetCompressedTexImage)(GLenum target, GLint lod, GLvoid *img);
void (GLSTUB_APIFPTR *glMultiTexCoord1d)(GLenum target, GLdouble s);
void (GLSTUB_APIFPTR *glMultiTexCoord1dv)(GLenum target, const GLdouble *v);
void (GLSTUB_APIFPTR *glMultiTexCoord1f)(GLenum target, GLfloat s);
void (GLSTUB_APIFPTR *glMultiTexCoord1fv)(GLenum target, const GLfloat *v);
void (GLSTUB_APIFPTR *glMultiTexCoord1i)(GLenum target, GLint s);
void (GLSTUB_APIFPTR *glMultiTexCoord1iv)(GLenum target, const GLint *v);
void (GLSTUB_APIFPTR *glMultiTexCoord1s)(GLenum target, GLshort s);
void (GLSTUB_APIFPTR *glMultiTexCoord1sv)(GLenum target, const GLshort *v);
void (GLSTUB_APIFPTR *glMultiTexCoord2d)(GLenum target, GLdouble s, GLdouble t);
void (GLSTUB_APIFPTR *glMultiTexCoord2dv)(GLenum target, const GLdouble *v);
void (GLSTUB_APIFPTR *glMultiTexCoord2f)(GLenum target, GLfloat s, GLfloat t);
void (GLSTUB_APIFPTR *glMultiTexCoord2fv)(GLenum target, const GLfloat *v);
void (GLSTUB_APIFPTR *glMultiTexCoord2i)(GLenum target, GLint s, GLint t);
void (GLSTUB_APIFPTR *glMultiTexCoord2iv)(GLenum target, const GLint *v);
void (GLSTUB_APIFPTR *glMultiTexCoord2s)(GLenum target, GLshort s, GLshort t);
void (GLSTUB_APIFPTR *glMultiTexCoord2sv)(GLenum target, const GLshort *v);
void (GLSTUB_APIFPTR *glMultiTexCoord3d)(GLenum target, GLdouble s, GLdouble t, GLdouble r);
void (GLSTUB_APIFPTR *glMultiTexCoord3dv)(GLenum target, const GLdouble *v);
void (GLSTUB_APIFPTR *glMultiTexCoord3f)(GLenum target, GLfloat s, GLfloat t, GLfloat r);
void (GLSTUB_APIFPTR *glMultiTexCoord3fv)(GLenum target, const GLfloat *v);
void (GLSTUB_APIFPTR *glMultiTexCoord3i)(GLenum target, GLint s, GLint t, GLint r);
void (GLSTUB_APIFPTR *glMultiTexCoord3iv)(GLenum target, const GLint *v);
void (GLSTUB_APIFPTR *glMultiTexCoord3s)(GLenum target, GLshort s, GLshort t, GLshort r);
void (GLSTUB_APIFPTR *glMultiTexCoord3sv)(GLenum target, const GLshort *v);
void (GLSTUB_APIFPTR *glMultiTexCoord4d)(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
void (GLSTUB_APIFPTR *glMultiTexCoord4dv)(GLenum target, const GLdouble *v);
void (GLSTUB_APIFPTR *glMultiTexCoord4f)(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
void (GLSTUB_APIFPTR *glMultiTexCoord4fv)(GLenum target, const GLfloat *v);
void (GLSTUB_APIFPTR *glMultiTexCoord4i)(GLenum target, GLint s, GLint t, GLint r, GLint q);
void (GLSTUB_APIFPTR *glMultiTexCoord4iv)(GLenum target, const GLint *v);
void (GLSTUB_APIFPTR *glMultiTexCoord4s)(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
void (GLSTUB_APIFPTR *glMultiTexCoord4sv)(GLenum target, const GLshort *v);
void (GLSTUB_APIFPTR *glLoadTransposeMatrixd)(const GLdouble m[16]);
void (GLSTUB_APIFPTR *glLoadTransposeMatrixf)(const GLfloat m[16]);
void (GLSTUB_APIFPTR *glMultTransposeMatrixd)(const GLdouble m[16]);
void (GLSTUB_APIFPTR *glMultTransposeMatrixf)(const GLfloat m[16]);
void (GLSTUB_APIFPTR *glSampleCoverage)(GLclampf value, GLboolean invert);
void (GLSTUB_APIFPTR *glSamplePass)(GLenum pass);

BOOL (GLSTUB_APIFPTR *wglCopyContext)(HGLRC a0, HGLRC a1, UINT a2);
HGLRC (GLSTUB_APIFPTR *wglCreateContext)(HDC hdc);
HGLRC (GLSTUB_APIFPTR *wglCreateLayerContext)(HDC a0, int a1);
BOOL (GLSTUB_APIFPTR *wglDeleteContext)(HGLRC a0);
BOOL (GLSTUB_APIFPTR *wglDescribeLayerPlane)(
	HDC a0, int a1, int a2, UINT a3, LPLAYERPLANEDESCRIPTOR a4);
HGLRC (GLSTUB_APIFPTR *wglGetCurrentContext)(void);
HDC (GLSTUB_APIFPTR *wglGetCurrentDC)(void);
int (GLSTUB_APIFPTR *wglGetLayerPaletteEntries)(
	HDC a0, int a1, int a2, int a3, COLORREF *a4);
PROC (GLSTUB_APIFPTR *wglGetProcAddress)(LPCSTR a0);
BOOL (GLSTUB_APIFPTR *wglMakeCurrent)(HDC hdc, HGLRC hglrc);
BOOL (GLSTUB_APIFPTR *wglRealizeLayerPalette)(HDC a0, int a1, BOOL a2);
int (GLSTUB_APIFPTR *wglSetLayerPaletteEntries)(
	HDC a0, int a1, int a2, int a3, const COLORREF *a4);
BOOL (GLSTUB_APIFPTR *wglShareLists)(HGLRC a0, HGLRC a1);
BOOL (GLSTUB_APIFPTR *wglSwapLayerBuffers)(HDC a0, UINT a1);
BOOL (GLSTUB_APIFPTR *wglUseFontBitmapsA)(
	HDC a0, DWORD a1, DWORD a2, DWORD a3);
BOOL (GLSTUB_APIFPTR *wglUseFontBitmapsW)(
	HDC a0, DWORD a1, DWORD a2, DWORD a3);
BOOL (GLSTUB_APIFPTR *wglUseFontOutlinesA)(
	HDC a0, DWORD a1, DWORD a2, DWORD a3, FLOAT a4,
	FLOAT a5, int a6, LPGLYPHMETRICSFLOAT a7);
BOOL (GLSTUB_APIFPTR *wglUseFontOutlinesW)(
	HDC a0, DWORD a1, DWORD a2, DWORD a3, FLOAT a4,
	FLOAT a5, int a6, LPGLYPHMETRICSFLOAT a7);
int (GLSTUB_APIFPTR *wglChoosePixelFormat)(
	HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd);
int (GLSTUB_APIFPTR *wglGetPixelFormat)(HDC hdc);
BOOL (GLSTUB_APIFPTR *wglSetPixelFormat)(
	HDC hdc, INT iPixelFormat, const PIXELFORMATDESCRIPTOR *ppfd);
int (GLSTUB_APIFPTR *wglDescribePixelFormat)(
	HDC hhd, INT iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd);
BOOL (GLSTUB_APIFPTR *wglSwapBuffers)(HDC hdc);

};


void *SglGetProcAddress(const char *name);
