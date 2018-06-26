#include <bgbrasw_glstub.h>

BGBRASW_GlStub_Funcs *SglFunc(void);
// { }

GLAPI void APIENTRY glClearIndex(GLfloat c)
	{ (SglFunc())->glClearIndex(c); }

GLAPI void APIENTRY glClearColor(
		GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
	{ (SglFunc())->glClearColor(red, green, blue, alpha); }

GLAPI void APIENTRY glClear(GLbitfield mask)
	{ (SglFunc())->glClear(mask); }

GLAPI void APIENTRY glIndexMask(GLuint mask)
	{ (SglFunc())->glIndexMask(mask); }

GLAPI void APIENTRY glColorMask(
		GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
	{ (SglFunc())->glColorMask(red, green, blue, alpha); }

GLAPI void APIENTRY glAlphaFunc(GLenum func, GLclampf ref)
	{ (SglFunc())->glAlphaFunc(func, ref); }

GLAPI void APIENTRY glBlendFunc(GLenum sfactor, GLenum dfactor)
	{ (SglFunc())->glBlendFunc(sfactor, dfactor); }

GLAPI void APIENTRY glLogicOp(GLenum opcode)
	{ (SglFunc())->glLogicOp(opcode); }

GLAPI void APIENTRY glCullFace(GLenum mode)
	{ (SglFunc())->glCullFace(mode); }

GLAPI void APIENTRY glFrontFace(GLenum mode)
	{ (SglFunc())->glFrontFace(mode); }

GLAPI void APIENTRY glPointSize(GLfloat size)
	{ (SglFunc())->glPointSize(size); }

GLAPI void APIENTRY glLineWidth(GLfloat width)
	{ (SglFunc())->glLineWidth(width); }

GLAPI void APIENTRY glLineStipple(GLint factor, GLushort pattern)
	{ (SglFunc())->glLineStipple(factor, pattern); }

GLAPI void APIENTRY glPolygonMode(GLenum face, GLenum mode)
	{ (SglFunc())->glPolygonMode(face, mode); }

GLAPI void APIENTRY glPolygonOffset(GLfloat factor, GLfloat units)
	{ (SglFunc())->glPolygonOffset(factor, units); }

GLAPI void APIENTRY glPolygonStipple(const GLubyte *mask)
	{ (SglFunc())->glPolygonStipple(mask); }

GLAPI void APIENTRY glGetPolygonStipple(GLubyte *mask)
	{ (SglFunc())->glGetPolygonStipple(mask); }

GLAPI void APIENTRY glEdgeFlag(GLboolean flag)
	{ (SglFunc())->glEdgeFlag(flag); }

GLAPI void APIENTRY glEdgeFlagv(const GLboolean *flag)
	{ (SglFunc())->glEdgeFlagv(flag); }

GLAPI void APIENTRY glScissor(
		GLint x, GLint y, GLsizei width, GLsizei height)
	{ (SglFunc())->glScissor(x, y, width, height); }

GLAPI void APIENTRY glClipPlane(GLenum plane, const GLdouble *equation)
	{ (SglFunc())->glClipPlane(plane, equation); }

GLAPI void APIENTRY glGetClipPlane(GLenum plane, GLdouble *equation)
	{ (SglFunc())->glGetClipPlane(plane, equation); }

GLAPI void APIENTRY glDrawBuffer(GLenum mode)
	{ (SglFunc())->glDrawBuffer(mode); }

GLAPI void APIENTRY glReadBuffer(GLenum mode)
	{ (SglFunc())->glReadBuffer(mode); }

GLAPI void APIENTRY glEnable(GLenum cap)
	{ (SglFunc())->glEnable(cap); }

GLAPI void APIENTRY glDisable(GLenum cap)
	{ (SglFunc())->glDisable(cap); }

GLAPI GLboolean APIENTRY glIsEnabled(GLenum cap)
	{ return((SglFunc())->glIsEnabled(cap)); }

GLAPI void APIENTRY glEnableClientState(GLenum cap)
	{ (SglFunc())->glEnableClientState(cap); }

GLAPI void APIENTRY glDisableClientState(GLenum cap)
	{ (SglFunc())->glDisableClientState(cap); }

GLAPI void APIENTRY glGetBooleanv(GLenum pname, GLboolean *params)
	{ (SglFunc())->glGetBooleanv(pname, params); }

GLAPI void APIENTRY glGetDoublev(GLenum pname, GLdouble *params)
	{ (SglFunc())->glGetDoublev(pname, params); }

GLAPI void APIENTRY glGetFloatv(GLenum pname, GLfloat *params)
	{ (SglFunc())->glGetFloatv(pname, params); }

GLAPI void APIENTRY glGetIntegerv(GLenum pname, GLint *params)
	{ (SglFunc())->glGetIntegerv(pname, params); }

GLAPI void APIENTRY glPushAttrib(GLbitfield mask)
	{ (SglFunc())->glPushAttrib(mask); }

GLAPI void APIENTRY glPopAttrib(void)
	{ (SglFunc())->glPopAttrib(); }

GLAPI void APIENTRY glPushClientAttrib(GLbitfield mask)
	{ (SglFunc())->glPushClientAttrib(mask); }

GLAPI void APIENTRY glPopClientAttrib(void)
	{ (SglFunc())->glPopClientAttrib(); }

GLAPI GLint APIENTRY glRenderMode(GLenum mode)
	{ return((SglFunc())->glRenderMode(mode)); }

GLAPI GLenum APIENTRY glGetError(void)
	{ return((SglFunc())->glGetError()); }

GLAPI const GLubyte* APIENTRY glGetString(GLenum name)
	{ return((SglFunc())->glGetString(name)); }

GLAPI void APIENTRY glFinish(void)
	{ (SglFunc())->glFinish(); }

GLAPI void APIENTRY glFlush(void)
	{ (SglFunc())->glFlush(); }

GLAPI void APIENTRY glHint(GLenum target, GLenum mode)
	{ (SglFunc())->glHint(target, mode); }

/* Depth Buffer */
GLAPI void APIENTRY glClearDepth(GLclampd depth)
	{ (SglFunc())->glClearDepth(depth); }

GLAPI void APIENTRY glDepthFunc(GLenum func)
	{ (SglFunc())->glDepthFunc(func); }

GLAPI void APIENTRY glDepthMask(GLboolean flag)
	{ (SglFunc())->glDepthMask(flag); }

GLAPI void APIENTRY glDepthRange(GLclampd near_val, GLclampd far_val)
	{ (SglFunc())->glDepthRange(near_val, far_val); }

/* Accumulation Buffer */
GLAPI void APIENTRY glClearAccum(
		GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
	{ (SglFunc())->glClearAccum(red, green, blue, alpha); }

GLAPI void APIENTRY glAccum(GLenum op, GLfloat value)
	{ (SglFunc())->glAccum(op, value); }

/* Transformation */
GLAPI void APIENTRY glMatrixMode(GLenum mode)
	{ (SglFunc())->glMatrixMode(mode); }

GLAPI void APIENTRY glOrtho(
		GLdouble left, GLdouble right, GLdouble bottom, GLdouble top,
		GLdouble near_val, GLdouble far_val)
	{ (SglFunc())->glOrtho(left, right, bottom, top, near_val, far_val); }

GLAPI void APIENTRY glFrustum(
		GLdouble left, GLdouble right, GLdouble bottom, GLdouble top,
		GLdouble near_val, GLdouble far_val)
	{ (SglFunc())->glFrustum(left, right, bottom, top, near_val, far_val); }

GLAPI void APIENTRY glViewport(
		GLint x, GLint y, GLsizei width, GLsizei height)
	{ (SglFunc())->glViewport(x, y, width, height); }

GLAPI void APIENTRY glPushMatrix(void)
	{ (SglFunc())->glPushMatrix(); }

GLAPI void APIENTRY glPopMatrix(void)
	{ (SglFunc())->glPopMatrix(); }

GLAPI void APIENTRY glLoadIdentity(void)
	{ (SglFunc())->glLoadIdentity(); }

GLAPI void APIENTRY glLoadMatrixd(const GLdouble *m)
	{ (SglFunc())->glLoadMatrixd(m); }

GLAPI void APIENTRY glLoadMatrixf(const GLfloat *m)
	{ (SglFunc())->glLoadMatrixf(m); }

GLAPI void APIENTRY glMultMatrixd(const GLdouble *m)
	{ (SglFunc())->glMultMatrixd(m); }

GLAPI void APIENTRY glMultMatrixf(const GLfloat *m)
	{ (SglFunc())->glMultMatrixf(m); }

GLAPI void APIENTRY glRotated(
		GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
	{ (SglFunc())->glRotated(angle, x, y, z); }

GLAPI void APIENTRY glRotatef(
		GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
	{ (SglFunc())->glRotatef(angle, x, y, z); }

GLAPI void APIENTRY glScaled(GLdouble x, GLdouble y, GLdouble z)
	{ (SglFunc())->glScaled(x, y, z); }

GLAPI void APIENTRY glScalef(GLfloat x, GLfloat y, GLfloat z)
	{ (SglFunc())->glScalef(x, y, z); }

GLAPI void APIENTRY glTranslated(GLdouble x, GLdouble y, GLdouble z)
	{ (SglFunc())->glTranslated(x, y, z); }

GLAPI void APIENTRY glTranslatef(GLfloat x, GLfloat y, GLfloat z)
	{ (SglFunc())->glTranslatef(x, y, z); }

/* Display Lists */
GLAPI GLboolean APIENTRY glIsList(GLuint list)
	{ return((SglFunc())->glIsList(list)); }

GLAPI void APIENTRY glDeleteLists(GLuint list, GLsizei range)
	{ (SglFunc())->glDeleteLists(list, range); }

GLAPI GLuint APIENTRY glGenLists(GLsizei range)
	{ return((SglFunc())->glGenLists(range)); }

GLAPI void APIENTRY glNewList(GLuint list, GLenum mode)
	{ (SglFunc())->glNewList(list, mode); }

GLAPI void APIENTRY glEndList(void)
	{ (SglFunc())->glEndList(); }

GLAPI void APIENTRY glCallList(GLuint list)
	{ (SglFunc())->glCallList(list); }

GLAPI void APIENTRY glCallLists(
		GLsizei n, GLenum type, const GLvoid *lists)
	{ (SglFunc())->glCallLists(n, type, lists); }

GLAPI void APIENTRY glListBase(GLuint base)
	{ (SglFunc())->glListBase(base); }

/* Drawing Functions */
GLAPI void APIENTRY glBegin(GLenum mode)
	{ (SglFunc())->glBegin(mode); }

GLAPI void APIENTRY glEnd(void)
	{ (SglFunc())->glEnd(); }

GLAPI void APIENTRY glVertex2d(GLdouble x, GLdouble y)
	{ (SglFunc())->glVertex2d(x, y); }

GLAPI void APIENTRY glVertex2f(GLfloat x, GLfloat y)
	{ (SglFunc())->glVertex2f(x, y); }

GLAPI void APIENTRY glVertex2i(GLint x, GLint y)
	{ (SglFunc())->glVertex2i(x, y); }

GLAPI void APIENTRY glVertex2s(GLshort x, GLshort y)
	{ (SglFunc())->glVertex2s(x, y); }

GLAPI void APIENTRY glVertex3d(GLdouble x, GLdouble y, GLdouble z)
	{ (SglFunc())->glVertex3d(x, y, z); }

GLAPI void APIENTRY glVertex3f(GLfloat x, GLfloat y, GLfloat z)
	{ (SglFunc())->glVertex3f(x, y, z); }

GLAPI void APIENTRY glVertex3i(GLint x, GLint y, GLint z)
	{ (SglFunc())->glVertex3i(x, y, z); }

GLAPI void APIENTRY glVertex3s(GLshort x, GLshort y, GLshort z)
	{ (SglFunc())->glVertex3s(x, y, z); }

GLAPI void APIENTRY glVertex4d(
		GLdouble x, GLdouble y, GLdouble z, GLdouble w)
	{ (SglFunc())->glVertex4d(x, y, z, w); }

GLAPI void APIENTRY glVertex4f(
		GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{ (SglFunc())->glVertex4f(x, y, z, w); }

GLAPI void APIENTRY glVertex4i(
		GLint x, GLint y, GLint z, GLint w)
	{ (SglFunc())->glVertex4i(x, y, z, w); }

GLAPI void APIENTRY glVertex4s(
		GLshort x, GLshort y, GLshort z, GLshort w)
	{ (SglFunc())->glVertex4s(x, y, z, w); }

GLAPI void APIENTRY glVertex2dv(const GLdouble *v)
	{ (SglFunc())->glVertex2dv(v); }

GLAPI void APIENTRY glVertex2fv(const GLfloat *v)
	{ (SglFunc())->glVertex2fv(v); }

GLAPI void APIENTRY glVertex2iv(const GLint *v)
	{ (SglFunc())->glVertex2iv(v); }

GLAPI void APIENTRY glVertex2sv(const GLshort *v)
	{ (SglFunc())->glVertex2sv(v); }

GLAPI void APIENTRY glVertex3dv(const GLdouble *v)
	{ (SglFunc())->glVertex3dv(v); }

GLAPI void APIENTRY glVertex3fv(const GLfloat *v)
	{ (SglFunc())->glVertex3fv(v); }

GLAPI void APIENTRY glVertex3iv(const GLint *v)
	{ (SglFunc())->glVertex3iv(v); }

GLAPI void APIENTRY glVertex3sv(const GLshort *v)
	{ (SglFunc())->glVertex3sv(v); }

GLAPI void APIENTRY glVertex4dv(const GLdouble *v)
	{ (SglFunc())->glVertex4dv(v); }

GLAPI void APIENTRY glVertex4fv(const GLfloat *v)
	{ (SglFunc())->glVertex4fv(v); }

GLAPI void APIENTRY glVertex4iv(const GLint *v)
	{ (SglFunc())->glVertex4iv(v); }

GLAPI void APIENTRY glVertex4sv(const GLshort *v)
	{ (SglFunc())->glVertex4sv(v); }

GLAPI void APIENTRY glNormal3b(GLbyte nx, GLbyte ny, GLbyte nz)
	{ (SglFunc())->glNormal3b(nx, ny, nz); }

GLAPI void APIENTRY glNormal3d(GLdouble nx, GLdouble ny, GLdouble nz)
	{ (SglFunc())->glNormal3d(nx, ny, nz); }

GLAPI void APIENTRY glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz)
	{ (SglFunc())->glNormal3f(nx, ny, nz); }

GLAPI void APIENTRY glNormal3i(GLint nx, GLint ny, GLint nz)
	{ (SglFunc())->glNormal3i(nx, ny, nz); }

GLAPI void APIENTRY glNormal3s(GLshort nx, GLshort ny, GLshort nz)
	{ (SglFunc())->glNormal3s(nx, ny, nz); }

GLAPI void APIENTRY glNormal3bv(const GLbyte *v)
	{ (SglFunc())->glNormal3bv(v); }

GLAPI void APIENTRY glNormal3dv(const GLdouble *v)
	{ (SglFunc())->glNormal3dv(v); }

GLAPI void APIENTRY glNormal3fv(const GLfloat *v)
	{ (SglFunc())->glNormal3fv(v); }

GLAPI void APIENTRY glNormal3iv(const GLint *v)
	{ (SglFunc())->glNormal3iv(v); }

GLAPI void APIENTRY glNormal3sv(const GLshort *v)
	{ (SglFunc())->glNormal3sv(v); }

GLAPI void APIENTRY glIndexd(GLdouble c)
	{ (SglFunc())->glIndexd(c); }

GLAPI void APIENTRY glIndexf(GLfloat c)
	{ (SglFunc())->glIndexf(c); }

GLAPI void APIENTRY glIndexi(GLint c)
	{ (SglFunc())->glIndexi(c); }

GLAPI void APIENTRY glIndexs(GLshort c)
	{ (SglFunc())->glIndexs(c); }

GLAPI void APIENTRY glIndexub(GLubyte c)
	{ (SglFunc())->glIndexub(c); }

GLAPI void APIENTRY glIndexdv(const GLdouble *c)
	{ (SglFunc())->glIndexdv(c); }

GLAPI void APIENTRY glIndexfv(const GLfloat *c)
	{ (SglFunc())->glIndexfv(c); }

GLAPI void APIENTRY glIndexiv(const GLint *c)
	{ (SglFunc())->glIndexiv(c); }

GLAPI void APIENTRY glIndexsv(const GLshort *c)
	{ (SglFunc())->glIndexsv(c); }

GLAPI void APIENTRY glIndexubv(const GLubyte *c)
	{ (SglFunc())->glIndexubv(c); }

GLAPI void APIENTRY glColor3b(GLbyte red, GLbyte green, GLbyte blue)
	{ (SglFunc())->glColor3b(red, green, blue); }

GLAPI void APIENTRY glColor3d(GLdouble red, GLdouble green, GLdouble blue)
	{ (SglFunc())->glColor3d(red, green, blue); }

GLAPI void APIENTRY glColor3f(GLfloat red, GLfloat green, GLfloat blue)
	{ (SglFunc())->glColor3f(red, green, blue); }

GLAPI void APIENTRY glColor3i(GLint red, GLint green, GLint blue)
	{ (SglFunc())->glColor3i(red, green, blue); }

GLAPI void APIENTRY glColor3s(GLshort red, GLshort green, GLshort blue)
	{ (SglFunc())->glColor3s(red, green, blue); }

GLAPI void APIENTRY glColor3ub(GLubyte red, GLubyte green, GLubyte blue)
	{ (SglFunc())->glColor3ub(red, green, blue); }

GLAPI void APIENTRY glColor3ui(GLuint red, GLuint green, GLuint blue)
	{ (SglFunc())->glColor3ui(red, green, blue); }

GLAPI void APIENTRY glColor3us(
		GLushort red, GLushort green, GLushort blue)
	{ (SglFunc())->glColor3us(red, green, blue); }

GLAPI void APIENTRY glColor4b(
		GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha)
	{ (SglFunc())->glColor4b(red, green, blue, alpha); }

GLAPI void APIENTRY glColor4d(
		GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha)
	{ (SglFunc())->glColor4d(red, green, blue, alpha); }

GLAPI void APIENTRY glColor4f(
		GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
	{ (SglFunc())->glColor4f(red, green, blue, alpha); }

GLAPI void APIENTRY glColor4i(
		GLint red, GLint green, GLint blue, GLint alpha)
	{ (SglFunc())->glColor4i(red, green, blue, alpha); }

GLAPI void APIENTRY glColor4s(
		GLshort red, GLshort green, GLshort blue, GLshort alpha)
	{ (SglFunc())->glColor4s(red, green, blue, alpha); }

GLAPI void APIENTRY glColor4ub(
		GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
	{ (SglFunc())->glColor4ub(red, green, blue, alpha); }

GLAPI void APIENTRY glColor4ui(
		GLuint red, GLuint green, GLuint blue, GLuint alpha)
	{ (SglFunc())->glColor4ui(red, green, blue, alpha); }

GLAPI void APIENTRY glColor4us(
		GLushort red, GLushort green, GLushort blue, GLushort alpha)
	{ (SglFunc())->glColor4us(red, green, blue, alpha); }

GLAPI void APIENTRY glColor3bv(const GLbyte *v)
	{ (SglFunc())->glColor3bv(v); }

GLAPI void APIENTRY glColor3dv(const GLdouble *v)
	{ (SglFunc())->glColor3dv(v); }

GLAPI void APIENTRY glColor3fv(const GLfloat *v)
	{ (SglFunc())->glColor3fv(v); }

GLAPI void APIENTRY glColor3iv(const GLint *v)
	{ (SglFunc())->glColor3iv(v); }

GLAPI void APIENTRY glColor3sv(const GLshort *v)
	{ (SglFunc())->glColor3sv(v); }

GLAPI void APIENTRY glColor3ubv(const GLubyte *v)
	{ (SglFunc())->glColor3ubv(v); }

GLAPI void APIENTRY glColor3uiv(const GLuint *v)
	{ (SglFunc())->glColor3uiv(v); }

GLAPI void APIENTRY glColor3usv(const GLushort *v)
	{ (SglFunc())->glColor3usv(v); }

GLAPI void APIENTRY glColor4bv(const GLbyte *v)
	{ (SglFunc())->glColor4bv(v); }

GLAPI void APIENTRY glColor4dv(const GLdouble *v)
	{ (SglFunc())->glColor4dv(v); }

GLAPI void APIENTRY glColor4fv(const GLfloat *v)
	{ (SglFunc())->glColor4fv(v); }

GLAPI void APIENTRY glColor4iv(const GLint *v)
	{ (SglFunc())->glColor4iv(v); }

GLAPI void APIENTRY glColor4sv(const GLshort *v)
	{ (SglFunc())->glColor4sv(v); }

GLAPI void APIENTRY glColor4ubv(const GLubyte *v)
	{ (SglFunc())->glColor4ubv(v); }

GLAPI void APIENTRY glColor4uiv(const GLuint *v)
	{ (SglFunc())->glColor4uiv(v); }

GLAPI void APIENTRY glColor4usv(const GLushort *v)
	{ (SglFunc())->glColor4usv(v); }

GLAPI void APIENTRY glTexCoord1d(GLdouble s)
	{ (SglFunc())->glTexCoord1d(s); }

GLAPI void APIENTRY glTexCoord1f(GLfloat s)
	{ (SglFunc())->glTexCoord1f(s); }

GLAPI void APIENTRY glTexCoord1i(GLint s)
	{ (SglFunc())->glTexCoord1i(s); }

GLAPI void APIENTRY glTexCoord1s(GLshort s)
	{ (SglFunc())->glTexCoord1s(s); }

GLAPI void APIENTRY glTexCoord2d(GLdouble s, GLdouble t)
	{ (SglFunc())->glTexCoord2d(s, t); }

GLAPI void APIENTRY glTexCoord2f(GLfloat s, GLfloat t)
	{ (SglFunc())->glTexCoord2f(s, t); }

GLAPI void APIENTRY glTexCoord2i(GLint s, GLint t)
	{ (SglFunc())->glTexCoord2i(s, t); }

GLAPI void APIENTRY glTexCoord2s(GLshort s, GLshort t)
	{ (SglFunc())->glTexCoord2s(s, t); }

GLAPI void APIENTRY glTexCoord3d(GLdouble s, GLdouble t, GLdouble r)
	{ (SglFunc())->glTexCoord3d(s, t, r); }

GLAPI void APIENTRY glTexCoord3f(GLfloat s, GLfloat t, GLfloat r)
	{ (SglFunc())->glTexCoord3f(s, t, r); }

GLAPI void APIENTRY glTexCoord3i(GLint s, GLint t, GLint r)
	{ (SglFunc())->glTexCoord3i(s, t, r); }

GLAPI void APIENTRY glTexCoord3s(GLshort s, GLshort t, GLshort r)
	{ (SglFunc())->glTexCoord3s(s, t, r); }

GLAPI void APIENTRY glTexCoord4d(
		GLdouble s, GLdouble t, GLdouble r, GLdouble q)
	{ (SglFunc())->glTexCoord4d(s, t, r, q); }

GLAPI void APIENTRY glTexCoord4f(
		GLfloat s, GLfloat t, GLfloat r, GLfloat q)
	{ (SglFunc())->glTexCoord4f(s, t, r, q); }

GLAPI void APIENTRY glTexCoord4i(GLint s, GLint t, GLint r, GLint q)
	{ (SglFunc())->glTexCoord4i(s, t, r, q); }

GLAPI void APIENTRY glTexCoord4s(GLshort s, GLshort t, GLshort r, GLshort q)
	{ (SglFunc())->glTexCoord4s(s, t, r, q); }

GLAPI void APIENTRY glTexCoord1dv(const GLdouble *v)
	{ (SglFunc())->glTexCoord1dv(v); }

GLAPI void APIENTRY glTexCoord1fv(const GLfloat *v)
	{ (SglFunc())->glTexCoord1fv(v); }

GLAPI void APIENTRY glTexCoord1iv(const GLint *v)
	{ (SglFunc())->glTexCoord1iv(v); }

GLAPI void APIENTRY glTexCoord1sv(const GLshort *v)
	{ (SglFunc())->glTexCoord1sv(v); }

GLAPI void APIENTRY glTexCoord2dv(const GLdouble *v)
	{ (SglFunc())->glTexCoord2dv(v); }

GLAPI void APIENTRY glTexCoord2fv(const GLfloat *v)
	{ (SglFunc())->glTexCoord2fv(v); }

GLAPI void APIENTRY glTexCoord2iv(const GLint *v)
	{ (SglFunc())->glTexCoord2iv(v); }

GLAPI void APIENTRY glTexCoord2sv(const GLshort *v)
	{ (SglFunc())->glTexCoord2sv(v); }

GLAPI void APIENTRY glTexCoord3dv(const GLdouble *v)
	{ (SglFunc())->glTexCoord3dv(v); }

GLAPI void APIENTRY glTexCoord3fv(const GLfloat *v)
	{ (SglFunc())->glTexCoord3fv(v); }

GLAPI void APIENTRY glTexCoord3iv(const GLint *v)
	{ (SglFunc())->glTexCoord3iv(v); }

GLAPI void APIENTRY glTexCoord3sv(const GLshort *v)
	{ (SglFunc())->glTexCoord3sv(v); }

GLAPI void APIENTRY glTexCoord4dv(const GLdouble *v)
	{ (SglFunc())->glTexCoord4dv(v); }

GLAPI void APIENTRY glTexCoord4fv(const GLfloat *v)
	{ (SglFunc())->glTexCoord4fv(v); }

GLAPI void APIENTRY glTexCoord4iv(const GLint *v)
	{ (SglFunc())->glTexCoord4iv(v); }

GLAPI void APIENTRY glTexCoord4sv(const GLshort *v)
	{ (SglFunc())->glTexCoord4sv(v); }

GLAPI void APIENTRY glRasterPos2d(GLdouble x, GLdouble y)
	{ (SglFunc())->glRasterPos2d(x, y); }

GLAPI void APIENTRY glRasterPos2f(GLfloat x, GLfloat y)
	{ (SglFunc())->glRasterPos2f(x, y); }

GLAPI void APIENTRY glRasterPos2i(GLint x, GLint y)
	{ (SglFunc())->glRasterPos2i(x, y); }

GLAPI void APIENTRY glRasterPos2s(GLshort x, GLshort y)
	{ (SglFunc())->glRasterPos2s(x, y); }

GLAPI void APIENTRY glRasterPos3d(GLdouble x, GLdouble y, GLdouble z)
	{ (SglFunc())->glRasterPos3d(x, y, z); }

GLAPI void APIENTRY glRasterPos3f(GLfloat x, GLfloat y, GLfloat z)
	{ (SglFunc())->glRasterPos3f(x, y, z); }

GLAPI void APIENTRY glRasterPos3i(GLint x, GLint y, GLint z)
	{ (SglFunc())->glRasterPos3i(x, y, z); }

GLAPI void APIENTRY glRasterPos3s(GLshort x, GLshort y, GLshort z)
	{ (SglFunc())->glRasterPos3s(x, y, z); }

GLAPI void APIENTRY glRasterPos4d(
		GLdouble x, GLdouble y, GLdouble z, GLdouble w)
	{ (SglFunc())->glRasterPos4d(x, y, z, w); }

GLAPI void APIENTRY glRasterPos4f(
		GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{ (SglFunc())->glRasterPos4f(x, y, z, w); }

GLAPI void APIENTRY glRasterPos4i(
		GLint x, GLint y, GLint z, GLint w)
	{ (SglFunc())->glRasterPos4i(x, y, z, w); }

GLAPI void APIENTRY glRasterPos4s(
		GLshort x, GLshort y, GLshort z, GLshort w)
	{ (SglFunc())->glRasterPos4s(x, y, z, w); }

GLAPI void APIENTRY glRasterPos2dv(const GLdouble *v)
	{ (SglFunc())->glRasterPos2dv(v); }

GLAPI void APIENTRY glRasterPos2fv(const GLfloat *v)
	{ (SglFunc())->glRasterPos2fv(v); }

GLAPI void APIENTRY glRasterPos2iv(const GLint *v)
	{ (SglFunc())->glRasterPos2iv(v); }

GLAPI void APIENTRY glRasterPos2sv(const GLshort *v)
	{ (SglFunc())->glRasterPos2sv(v); }

GLAPI void APIENTRY glRasterPos3dv(const GLdouble *v)
	{ (SglFunc())->glRasterPos3dv(v); }

GLAPI void APIENTRY glRasterPos3fv(const GLfloat *v)
	{ (SglFunc())->glRasterPos3fv(v); }

GLAPI void APIENTRY glRasterPos3iv(const GLint *v)
	{ (SglFunc())->glRasterPos3iv(v); }

GLAPI void APIENTRY glRasterPos3sv(const GLshort *v)
	{ (SglFunc())->glRasterPos3sv(v); }

GLAPI void APIENTRY glRasterPos4dv(const GLdouble *v)
	{ (SglFunc())->glRasterPos4dv(v); }

GLAPI void APIENTRY glRasterPos4fv(const GLfloat *v)
	{ (SglFunc())->glRasterPos4fv(v); }

GLAPI void APIENTRY glRasterPos4iv(const GLint *v)
	{ (SglFunc())->glRasterPos4iv(v); }

GLAPI void APIENTRY glRasterPos4sv(const GLshort *v)
	{ (SglFunc())->glRasterPos4sv(v); }

GLAPI void APIENTRY glRectd(
		GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)
	{ (SglFunc())->glRectd(x1, y1, x2, y2); }

GLAPI void APIENTRY glRectf(
		GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
	{ (SglFunc())->glRectf(x1, y1, x2, y2); }

GLAPI void APIENTRY glRecti(
		GLint x1, GLint y1, GLint x2, GLint y2)
	{ (SglFunc())->glRecti(x1, y1, x2, y2); }

GLAPI void APIENTRY glRects(
		GLshort x1, GLshort y1, GLshort x2, GLshort y2)
	{ (SglFunc())->glRects(x1, y1, x2, y2); }

GLAPI void APIENTRY glRectdv(const GLdouble *v1, const GLdouble *v2)
	{ (SglFunc())->glRectdv(v1, v2); }

GLAPI void APIENTRY glRectfv(const GLfloat *v1, const GLfloat *v2)
	{ (SglFunc())->glRectfv(v1, v2); }

GLAPI void APIENTRY glRectiv(const GLint *v1, const GLint *v2)
	{ (SglFunc())->glRectiv(v1, v2); }

GLAPI void APIENTRY glRectsv(const GLshort *v1, const GLshort *v2)
	{ (SglFunc())->glRectsv(v1, v2); }

/* Lighting */
GLAPI void APIENTRY glShadeModel(GLenum mode)
	{ (SglFunc())->glShadeModel(mode); }

GLAPI void APIENTRY glLightf(
		GLenum light, GLenum pname, GLfloat param)
	{ (SglFunc())->glLightf(light, pname, param); }

GLAPI void APIENTRY glLighti(
		GLenum light, GLenum pname, GLint param)
	{ (SglFunc())->glLighti(light, pname, param); }

GLAPI void APIENTRY glLightfv(
		GLenum light, GLenum pname, const GLfloat *params)
	{ (SglFunc())->glLightfv(light, pname, params); }

GLAPI void APIENTRY glLightiv(
		GLenum light, GLenum pname, const GLint *params)
	{ (SglFunc())->glLightiv(light, pname, params); }

GLAPI void APIENTRY glGetLightfv(
		GLenum light, GLenum pname, GLfloat *params)
	{ (SglFunc())->glGetLightfv(light, pname, params); }

GLAPI void APIENTRY glGetLightiv(
		GLenum light, GLenum pname, GLint *params)
	{ (SglFunc())->glGetLightiv(light, pname, params); }

GLAPI void APIENTRY glLightModelf(GLenum pname, GLfloat param)
	{ (SglFunc())->glLightModelf(pname, param); }

GLAPI void APIENTRY glLightModeli(GLenum pname, GLint param)
	{ (SglFunc())->glLightModeli(pname, param); }

GLAPI void APIENTRY glLightModelfv(GLenum pname, const GLfloat *params)
	{ (SglFunc())->glLightModelfv(pname, params); }

GLAPI void APIENTRY glLightModeliv(GLenum pname, const GLint *params)
	{ (SglFunc())->glLightModeliv(pname, params); }

GLAPI void APIENTRY glMaterialf(GLenum face, GLenum pname, GLfloat param)
	{ (SglFunc())->glMaterialf(face, pname, param); }

GLAPI void APIENTRY glMateriali(GLenum face, GLenum pname, GLint param)
	{ (SglFunc())->glMateriali(face, pname, param); }

GLAPI void APIENTRY glMaterialfv(
		GLenum face, GLenum pname, const GLfloat *params)
	{ (SglFunc())->glMaterialfv(face, pname, params); }

GLAPI void APIENTRY glMaterialiv(
		GLenum face, GLenum pname, const GLint *params)
	{ (SglFunc())->glMaterialiv(face, pname, params); }

GLAPI void APIENTRY glGetMaterialfv(
		GLenum face, GLenum pname, GLfloat *params)
	{ (SglFunc())->glGetMaterialfv(face, pname, params); }

GLAPI void APIENTRY glGetMaterialiv(
		GLenum face, GLenum pname, GLint *params)
	{ (SglFunc())->glGetMaterialiv(face, pname, params); }

GLAPI void APIENTRY glColorMaterial(GLenum face, GLenum mode)
	{ (SglFunc())->glColorMaterial(face, mode); }

/* Raster functions */
GLAPI void APIENTRY glPixelZoom(GLfloat xfactor, GLfloat yfactor)
	{ (SglFunc())->glPixelZoom(xfactor, yfactor); }

GLAPI void APIENTRY glPixelStoref(GLenum pname, GLfloat param)
	{ (SglFunc())->glPixelStoref(pname, param); }

GLAPI void APIENTRY glPixelStorei(GLenum pname, GLint param)
	{ (SglFunc())->glPixelStorei(pname, param); }

GLAPI void APIENTRY glPixelTransferf(GLenum pname, GLfloat param)
	{ (SglFunc())->glPixelTransferf(pname, param); }

GLAPI void APIENTRY glPixelTransferi(GLenum pname, GLint param)
	{ (SglFunc())->glPixelTransferi(pname, param); }

GLAPI void APIENTRY glPixelMapfv(
		GLenum map, GLint mapsize, const GLfloat *values)
	{ (SglFunc())->glPixelMapfv(map, mapsize, values); }

GLAPI void APIENTRY glPixelMapuiv(
		GLenum map, GLint mapsize, const GLuint *values)
	{ (SglFunc())->glPixelMapuiv(map, mapsize, values); }

GLAPI void APIENTRY glPixelMapusv(
		GLenum map, GLint mapsize, const GLushort *values)
	{ (SglFunc())->glPixelMapusv(map, mapsize, values); }

GLAPI void APIENTRY glGetPixelMapfv(GLenum map, GLfloat *values)
	{ (SglFunc())->glGetPixelMapfv(map, values); }

GLAPI void APIENTRY glGetPixelMapuiv(GLenum map, GLuint *values)
	{ (SglFunc())->glGetPixelMapuiv(map, values); }

GLAPI void APIENTRY glGetPixelMapusv(GLenum map, GLushort *values)
	{ (SglFunc())->glGetPixelMapusv(map, values); }

GLAPI void APIENTRY glBitmap(
		GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig,
		GLfloat xmove, GLfloat ymove, const GLubyte *bitmap)
	{ (SglFunc())->glBitmap(
		width, height, xorig, yorig, xmove, ymove, bitmap); }

GLAPI void APIENTRY glReadPixels(
		GLint x, GLint y, GLsizei width, GLsizei height,
		GLenum format, GLenum type, GLvoid *pixels)
	{ (SglFunc())->glReadPixels(
		x, y, width, height, format, type, pixels); }

GLAPI void APIENTRY glDrawPixels(
		GLsizei width, GLsizei height, GLenum format, GLenum type,
		const GLvoid *pixels)
	{ (SglFunc())->glDrawPixels(width, height, format, type, pixels); }

GLAPI void APIENTRY glCopyPixels(
		GLint x, GLint y, GLsizei width, GLsizei height, GLenum type)
	{ (SglFunc())->glCopyPixels(x, y, width, height, type); }

/* Stenciling */
GLAPI void APIENTRY glStencilFunc(GLenum func, GLint ref, GLuint mask)
	{ (SglFunc())->glStencilFunc(func, ref, mask); }

GLAPI void APIENTRY glStencilMask(GLuint mask)
	{ (SglFunc())->glStencilMask(mask); }

GLAPI void APIENTRY glStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
	{ (SglFunc())->glStencilOp(fail, zfail, zpass); }

GLAPI void APIENTRY glClearStencil(GLint s)
	{ (SglFunc())->glClearStencil(s); }

/* Texture mapping */
GLAPI void APIENTRY glTexGend(
		GLenum coord, GLenum pname, GLdouble param)
	{ (SglFunc())->glTexGend(coord, pname, param); }

GLAPI void APIENTRY glTexGenf(
		GLenum coord, GLenum pname, GLfloat param)
	{ (SglFunc())->glTexGenf(coord, pname, param); }

GLAPI void APIENTRY glTexGeni(
		GLenum coord, GLenum pname, GLint param)
	{ (SglFunc())->glTexGeni(coord, pname, param); }

GLAPI void APIENTRY glTexGendv(
		GLenum coord, GLenum pname, const GLdouble *params)
	{ (SglFunc())->glTexGendv(coord, pname, params); }

GLAPI void APIENTRY glTexGenfv(
		GLenum coord, GLenum pname, const GLfloat *params)
	{ (SglFunc())->glTexGenfv(coord, pname, params); }

GLAPI void APIENTRY glTexGeniv(
		GLenum coord, GLenum pname, const GLint *params)
	{ (SglFunc())->glTexGeniv(coord, pname, params); }

GLAPI void APIENTRY glGetTexGendv(
		GLenum coord, GLenum pname, GLdouble *params)
	{ (SglFunc())->glGetTexGendv(coord, pname, params); }

GLAPI void APIENTRY glGetTexGenfv(
		GLenum coord, GLenum pname, GLfloat *params)
	{ (SglFunc())->glGetTexGenfv(coord, pname, params); }

GLAPI void APIENTRY glGetTexGeniv(
		GLenum coord, GLenum pname, GLint *params)
	{ (SglFunc())->glGetTexGeniv(coord, pname, params); }

GLAPI void APIENTRY glTexEnvf(
		GLenum target, GLenum pname, GLfloat param)
	{ (SglFunc())->glTexEnvf(target, pname, param); }

GLAPI void APIENTRY glTexEnvi(
		GLenum target, GLenum pname, GLint param)
	{ (SglFunc())->glTexEnvi(target, pname, param); }

GLAPI void APIENTRY glTexEnvfv(
		GLenum target, GLenum pname, const GLfloat *params)
	{ (SglFunc())->glTexEnvfv(target, pname, params); }

GLAPI void APIENTRY glTexEnviv(
		GLenum target, GLenum pname, const GLint *params)
	{ (SglFunc())->glTexEnviv(target, pname, params); }

GLAPI void APIENTRY glGetTexEnvfv(
		GLenum target, GLenum pname, GLfloat *params)
	{ (SglFunc())->glGetTexEnvfv(target, pname, params); }

GLAPI void APIENTRY glGetTexEnviv(
		GLenum target, GLenum pname, GLint *params)
	{ (SglFunc())->glGetTexEnviv(target, pname, params); }

GLAPI void APIENTRY glTexParameterf(
		GLenum target, GLenum pname, GLfloat param)
	{ (SglFunc())->glTexParameterf(target, pname, param); }

GLAPI void APIENTRY glTexParameteri(
		GLenum target, GLenum pname, GLint param)
	{ (SglFunc())->glTexParameteri(target, pname, param); }

GLAPI void APIENTRY glTexParameterfv(
		GLenum target, GLenum pname, const GLfloat *params)
	{ (SglFunc())->glTexParameterfv(target, pname, params); }

GLAPI void APIENTRY glTexParameteriv(
		GLenum target, GLenum pname, const GLint *params)
	{ (SglFunc())->glTexParameteriv(target, pname, params); }

GLAPI void APIENTRY glGetTexParameterfv(
		GLenum target, GLenum pname, GLfloat *params)
	{ (SglFunc())->glGetTexParameterfv(target, pname, params); }

GLAPI void APIENTRY glGetTexParameteriv(
		GLenum target, GLenum pname, GLint *params)
	{ (SglFunc())->glGetTexParameteriv(target, pname, params); }

GLAPI void APIENTRY glGetTexLevelParameterfv(
		GLenum target, GLint level, GLenum pname, GLfloat *params)
	{ (SglFunc())->glGetTexLevelParameterfv(target, level, pname, params); }

GLAPI void APIENTRY glGetTexLevelParameteriv(
		GLenum target, GLint level, GLenum pname, GLint *params)
	{ (SglFunc())->glGetTexLevelParameteriv(target, level, pname, params); }

GLAPI void APIENTRY glTexImage1D(
		GLenum target, GLint level, GLint internalFormat,
		GLsizei width, GLint border, GLenum format, GLenum type,
		const GLvoid *pixels)
	{ (SglFunc())->glTexImage1D(target, level, internalFormat,
		width, border, format, type, pixels); }

GLAPI void APIENTRY glTexImage2D(
		GLenum target, GLint level, GLint internalFormat,
		GLsizei width, GLsizei height, GLint border, GLenum format,
		GLenum type, const GLvoid *pixels)
	{ (SglFunc())->glTexImage2D(target, level, internalFormat,
		width, height, border, format, type, pixels); }

GLAPI void APIENTRY glGetTexImage(
		GLenum target, GLint level, GLenum format,
		GLenum type, GLvoid *pixels)
	{ (SglFunc())->glGetTexImage(target, level, 
		format, type, pixels); }

/* Evaluators */
GLAPI void APIENTRY glMap1d(
		GLenum target, GLdouble u1, GLdouble u2, GLint stride,
		GLint order, const GLdouble *points)
	{ (SglFunc())->glMap1d(target, u1, u2, stride, order, points); }

GLAPI void APIENTRY glMap1f(
		GLenum target, GLfloat u1, GLfloat u2, GLint stride,
		GLint order, const GLfloat *points)
	{ (SglFunc())->glMap1f(target, u1, u2, stride, order, points); }

GLAPI void APIENTRY glMap2d(
		GLenum target, GLdouble u1, GLdouble u2, GLint ustride,
		GLint uorder, GLdouble v1, GLdouble v2, GLint vstride,
		GLint vorder, const GLdouble *points)
	{ (SglFunc())->glMap2d(target, u1, u2, ustride, uorder,
			v1, v2, vstride, vorder, points); }

GLAPI void APIENTRY glMap2f(
		GLenum target, GLfloat u1, GLfloat u2, GLint ustride,
		GLint uorder, GLfloat v1, GLfloat v2, GLint vstride,
		GLint vorder, const GLfloat *points)
	{ (SglFunc())->glMap2f(target, u1, u2, ustride, uorder,
			v1, v2, vstride, vorder, points); }

GLAPI void APIENTRY glGetMapdv(GLenum target, GLenum query, GLdouble *v)
	{ (SglFunc())->glGetMapdv(target, query, v); }

GLAPI void APIENTRY glGetMapfv(GLenum target, GLenum query, GLfloat *v)
	{ (SglFunc())->glGetMapfv(target, query, v); }

GLAPI void APIENTRY glGetMapiv(GLenum target, GLenum query, GLint *v)
	{ (SglFunc())->glGetMapiv(target, query, v); }

GLAPI void APIENTRY glEvalCoord1d(GLdouble u)
	{ (SglFunc())->glEvalCoord1d(u); }

GLAPI void APIENTRY glEvalCoord1f(GLfloat u)
	{ (SglFunc())->glEvalCoord1f(u); }

GLAPI void APIENTRY glEvalCoord1dv(const GLdouble *u)
	{ (SglFunc())->glEvalCoord1dv(u); }

GLAPI void APIENTRY glEvalCoord1fv(const GLfloat *u)
	{ (SglFunc())->glEvalCoord1fv(u); }

GLAPI void APIENTRY glEvalCoord2d(GLdouble u, GLdouble v)
	{ (SglFunc())->glEvalCoord2d(u, v); }

GLAPI void APIENTRY glEvalCoord2f(GLfloat u, GLfloat v)
	{ (SglFunc())->glEvalCoord2f(u, v); }

GLAPI void APIENTRY glEvalCoord2dv(const GLdouble *u)
	{ (SglFunc())->glEvalCoord2dv(u); }

GLAPI void APIENTRY glEvalCoord2fv(const GLfloat *u)
	{ (SglFunc())->glEvalCoord2fv(u); }

GLAPI void APIENTRY glMapGrid1d(GLint un, GLdouble u1, GLdouble u2)
	{ (SglFunc())->glMapGrid1d(un, u1, u2); }

GLAPI void APIENTRY glMapGrid1f(GLint un, GLfloat u1, GLfloat u2)
	{ (SglFunc())->glMapGrid1f(un, u1, u2); }

GLAPI void APIENTRY glMapGrid2d(
		GLint un, GLdouble u1, GLdouble u2, GLint vn,
		GLdouble v1, GLdouble v2)
	{ (SglFunc())->glMapGrid2d(un, u1, u2, vn, v1, v2); }

GLAPI void APIENTRY glMapGrid2f(
		GLint un, GLfloat u1, GLfloat u2, GLint vn,
		GLfloat v1, GLfloat v2)
	{ (SglFunc())->glMapGrid2f(un, u1, u2, vn, v1, v2); }

GLAPI void APIENTRY glEvalPoint1(GLint i)
	{ (SglFunc())->glEvalPoint1(i); }

GLAPI void APIENTRY glEvalPoint2(GLint i, GLint j)
	{ (SglFunc())->glEvalPoint2(i, j); }

GLAPI void APIENTRY glEvalMesh1(GLenum mode, GLint i1, GLint i2)
	{ (SglFunc())->glEvalMesh1(mode, i1, i2); }

GLAPI void APIENTRY glEvalMesh2(
		GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2)
	{ (SglFunc())->glEvalMesh2(mode, i1, i2, j1, j2); }

/* Fog */
GLAPI void APIENTRY glFogf(GLenum pname, GLfloat param)
	{ (SglFunc())->glFogf(pname, param); }

GLAPI void APIENTRY glFogi(GLenum pname, GLint param)
	{ (SglFunc())->glFogi(pname, param); }

GLAPI void APIENTRY glFogfv(GLenum pname, const GLfloat *params)
	{ (SglFunc())->glFogfv(pname, params); }

GLAPI void APIENTRY glFogiv(GLenum pname, const GLint *params)
	{ (SglFunc())->glFogiv(pname, params); }

/* Selection and Feedback */
GLAPI void APIENTRY glFeedbackBuffer(
		GLsizei size, GLenum type, GLfloat *buffer)
	{ (SglFunc())->glFeedbackBuffer(size, type, buffer); }

GLAPI void APIENTRY glPassThrough(GLfloat token)
	{ (SglFunc())->glPassThrough(token); }

GLAPI void APIENTRY glSelectBuffer(GLsizei size, GLuint *buffer)
	{ (SglFunc())->glSelectBuffer(size, buffer); }

GLAPI void APIENTRY glInitNames(void)
	{ (SglFunc())->glInitNames(); }

GLAPI void APIENTRY glLoadName(GLuint name)
	{ (SglFunc())->glLoadName(name); }

GLAPI void APIENTRY glPushName(GLuint name)
	{ (SglFunc())->glPushName(name); }

GLAPI void APIENTRY glPopName(void)
	{ (SglFunc())->glPopName(); }


/* 1.1 functions */
/* texture objects */
GLAPI void APIENTRY glGenTextures(GLsizei n, GLuint *textures)
	{ (SglFunc())->glGenTextures(n, textures); }

GLAPI void APIENTRY glDeleteTextures(GLsizei n, const GLuint *textures)
	{ (SglFunc())->glDeleteTextures(n, textures); }

GLAPI void APIENTRY glBindTexture(GLenum target, GLuint texture)
	{ (SglFunc())->glBindTexture(target, texture); }

GLAPI void APIENTRY glPrioritizeTextures(
		GLsizei n, const GLuint *textures, const GLclampf *priorities)
	{ (SglFunc())->glPrioritizeTextures(n, textures, priorities); }

GLAPI GLboolean APIENTRY glAreTexturesResident(
		GLsizei n, const GLuint *textures, GLboolean *residences)
	{ return((SglFunc())->glAreTexturesResident(n, textures, residences)); }

GLAPI GLboolean APIENTRY glIsTexture(GLuint texture)
	{ return((SglFunc())->glIsTexture(texture)); }

/* texture mapping */
GLAPI void APIENTRY glTexSubImage1D(
		GLenum target, GLint level, GLint xoffset, GLsizei width,
		GLenum format, GLenum type, const GLvoid *pixels)
	{ (SglFunc())->glTexSubImage1D(target, level, xoffset, width,
		format, type, pixels); }

GLAPI void APIENTRY glTexSubImage2D(
		GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLsizei width, GLsizei height, GLenum format, GLenum type,
		const GLvoid *pixels)
	{ (SglFunc())->glTexSubImage2D(target, level, xoffset, yoffset,
		width, height, format, type, pixels); }

GLAPI void APIENTRY glCopyTexImage1D(
		GLenum target, GLint level, GLenum internalformat,
		GLint x, GLint y, GLsizei width, GLint border)
	{ (SglFunc())->glCopyTexImage1D(
		target, level, internalformat, x, y, width, border); }

GLAPI void APIENTRY glCopyTexImage2D(
		GLenum target, GLint level, GLenum internalformat,
		GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
	{ (SglFunc())->glCopyTexImage2D(
		target, level, internalformat, x, y, width, height, border); }

GLAPI void APIENTRY glCopyTexSubImage1D(
		GLenum target, GLint level, GLint xoffset,
		GLint x, GLint y, GLsizei width)
	{ (SglFunc())->glCopyTexSubImage1D(target, level, xoffset, x, y, width); }

GLAPI void APIENTRY glCopyTexSubImage2D(
		GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLint x, GLint y, GLsizei width, GLsizei height)
	{ (SglFunc())->glCopyTexSubImage2D(
		target, level, xoffset, yoffset, x, y, width, height); }

/* vertex arrays */
GLAPI void APIENTRY glVertexPointer(
		GLint size, GLenum type, GLsizei stride, const GLvoid *ptr)
	{ (SglFunc())->glVertexPointer(size, type, stride, ptr); }

GLAPI void APIENTRY glNormalPointer(
		GLenum type, GLsizei stride, const GLvoid *ptr)
	{ (SglFunc())->glNormalPointer(type, stride, ptr); }

GLAPI void APIENTRY glColorPointer(
		GLint size, GLenum type, GLsizei stride, const GLvoid *ptr)
	{ (SglFunc())->glColorPointer(size, type, stride, ptr); }

GLAPI void APIENTRY glIndexPointer(
		GLenum type, GLsizei stride, const GLvoid *ptr)
	{ (SglFunc())->glIndexPointer(type, stride, ptr); }

GLAPI void APIENTRY glTexCoordPointer(
		GLint size, GLenum type, GLsizei stride, const GLvoid *ptr)
	{ (SglFunc())->glTexCoordPointer(size, type, stride, ptr); }

GLAPI void APIENTRY glEdgeFlagPointer(GLsizei stride, const GLvoid *ptr)
	{ (SglFunc())->glEdgeFlagPointer(stride, ptr); }

GLAPI void APIENTRY glGetPointerv(GLenum pname, GLvoid **params)
	{ (SglFunc())->glGetPointerv(pname, params); }

GLAPI void APIENTRY glArrayElement(GLint i)
	{ (SglFunc())->glArrayElement(i); }

GLAPI void APIENTRY glDrawArrays(GLenum mode, GLint first, GLsizei count)
	{ (SglFunc())->glDrawArrays(mode, first, count); }

GLAPI void APIENTRY glDrawElements(
		GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
	{ (SglFunc())->glDrawElements(mode, count, type, indices); }

GLAPI void APIENTRY glInterleavedArrays(
		GLenum format, GLsizei stride, const GLvoid *pointer)
	{ (SglFunc())->glInterleavedArrays(format, stride, pointer); }



/* Under Windows, we do not define OpenGL 1.2 & 1.3 functionality, since
   it is treated as extensions (defined in glext.h) */

#if 1

/* 1.2 functions */
GLAPI void APIENTRY glDrawRangeElements(
		GLenum mode, GLuint start, GLuint end, GLsizei count,
		GLenum type, const GLvoid *indices)
	{ (SglFunc())->glDrawRangeElements(
		mode, start, end, count, type, indices); }
GLAPI void APIENTRY glTexImage3D(
		GLenum target, GLint level, GLenum internalFormat,
		GLsizei width, GLsizei height, GLsizei depth, GLint border,
		GLenum format, GLenum type, const GLvoid *pixels)
	{ (SglFunc())->glTexImage3D(target, level, internalFormat,
		width, height, depth, border, format, type, pixels); }
GLAPI void APIENTRY glTexSubImage3D(
		GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,
		GLenum format, GLenum type, const GLvoid *pixels)
	{ (SglFunc())->glTexSubImage3D(target, level,
		xoffset, yoffset, zoffset,
		width, height, depth, format, type, pixels); }
GLAPI void APIENTRY glCopyTexSubImage3D(
		GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
	{ (SglFunc())->glCopyTexSubImage3D(target, level,
		xoffset, yoffset, zoffset, x, y,
		width, height); }

/* 1.2 imaging extension functions */
GLAPI void APIENTRY glColorTable(
		GLenum target, GLenum internalformat, GLsizei width,
		GLenum format, GLenum type, const GLvoid *table)
	{ (SglFunc())->glColorTable(target, internalformat,
		width, format, type, table); }
GLAPI void APIENTRY glColorSubTable(
		GLenum target, GLsizei start, GLsizei count, GLenum format,
		GLenum type, const GLvoid *data)
	{ (SglFunc())->glColorSubTable(target, start, count, format,
		type, data); }
GLAPI void APIENTRY glColorTableParameteriv(
		GLenum target, GLenum pname, const GLint *params)
	{ (SglFunc())->glColorTableParameteriv(target, pname, params); }
GLAPI void APIENTRY glColorTableParameterfv(
		GLenum target, GLenum pname, const GLfloat *params)
	{ (SglFunc())->glColorTableParameterfv(target, pname, params); }
GLAPI void APIENTRY glCopyColorSubTable(
		GLenum target, GLsizei start, GLint x, GLint y, GLsizei width)
	{ (SglFunc())->glCopyColorSubTable(target, start, x, y, width); }
GLAPI void APIENTRY glCopyColorTable(
		GLenum target, GLenum internalformat, GLint x, GLint y,
		GLsizei width)
	{ (SglFunc())->glCopyColorTable(target, internalformat, x, y, width); }
GLAPI void APIENTRY glGetColorTable(
		GLenum target, GLenum format, GLenum type, GLvoid *table)
	{ (SglFunc())->glGetColorTable(target, format, type, table); }
GLAPI void APIENTRY glGetColorTableParameterfv(
		GLenum target, GLenum pname, GLfloat *params)
	{ (SglFunc())->glGetColorTableParameterfv(target, pname, params); }
GLAPI void APIENTRY glGetColorTableParameteriv(
		GLenum target, GLenum pname, GLint *params)
	{ (SglFunc())->glGetColorTableParameteriv(target, pname, params); }
GLAPI void APIENTRY glBlendEquation(
		GLenum mode)
	{ (SglFunc())->glBlendEquation(mode); }
GLAPI void APIENTRY glBlendColor(
		GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
	{ (SglFunc())->glBlendColor(red, green, blue, alpha); }
GLAPI void APIENTRY glHistogram(
		GLenum target, GLsizei width, GLenum internalformat, GLboolean sink)
	{ (SglFunc())->glHistogram(target, width, internalformat, sink); }
GLAPI void APIENTRY glResetHistogram(
		GLenum target)
	{ (SglFunc())->glResetHistogram(target); }
GLAPI void APIENTRY glGetHistogram(
		GLenum target, GLboolean reset, GLenum format,
		GLenum type, GLvoid *values)
	{ (SglFunc())->glGetHistogram(target, reset, format, type, values); }
GLAPI void APIENTRY glGetHistogramParameterfv(
		GLenum target, GLenum pname, GLfloat *params)
	{ (SglFunc())->glGetHistogramParameterfv(target, pname, params); }
GLAPI void APIENTRY glGetHistogramParameteriv(
		GLenum target, GLenum pname, GLint *params)
	{ (SglFunc())->glGetHistogramParameteriv(target, pname, params); }
GLAPI void APIENTRY glMinmax(
		GLenum target, GLenum internalformat, GLboolean sink)
	{ (SglFunc())->glMinmax(target, internalformat, sink); }
GLAPI void APIENTRY glResetMinmax(GLenum target)
	{ (SglFunc())->glResetMinmax(target); }
GLAPI void APIENTRY glGetMinmax(
		GLenum target, GLboolean reset, GLenum format,
		GLenum types, GLvoid *values)
	{ (SglFunc())->glGetMinmax(target, reset, format, types, values); }
GLAPI void APIENTRY glGetMinmaxParameterfv(
		GLenum target, GLenum pname, GLfloat *params)
	{ (SglFunc())->glGetMinmaxParameterfv(target, pname, params); }
GLAPI void APIENTRY glGetMinmaxParameteriv(
		GLenum target, GLenum pname, GLint *params)
	{ (SglFunc())->glGetMinmaxParameteriv(target, pname, params); }
GLAPI void APIENTRY glConvolutionFilter1D(
		GLenum target, GLenum internalformat, GLsizei width,
		GLenum format, GLenum type, const GLvoid *image)
	{ (SglFunc())->glConvolutionFilter1D(target, internalformat, width,
		format, type, image); }
GLAPI void APIENTRY glConvolutionFilter2D(
		GLenum target, GLenum internalformat, GLsizei width,
		GLsizei height, GLenum format, GLenum type,
		const GLvoid *image)
	{ (SglFunc())->glConvolutionFilter2D(target, internalformat, width,
		height, format, type, image); }
GLAPI void APIENTRY glConvolutionParameterf(
		GLenum target, GLenum pname, GLfloat params)
	{ (SglFunc())->glConvolutionParameterf(target, pname, params); }
GLAPI void APIENTRY glConvolutionParameterfv(
		GLenum target, GLenum pname, const GLfloat *params)
	{ (SglFunc())->glConvolutionParameterfv(target, pname, params); }
GLAPI void APIENTRY glConvolutionParameteri(
		GLenum target, GLenum pname, GLint params)
	{ (SglFunc())->glConvolutionParameteri(target, pname, params); }
GLAPI void APIENTRY glConvolutionParameteriv(
		GLenum target, GLenum pname, const GLint *params)
	{ (SglFunc())->glConvolutionParameteriv(target, pname, params); }
GLAPI void APIENTRY glCopyConvolutionFilter1D(
		GLenum target, GLenum internalformat, GLint x, GLint y,
		GLsizei width)
	{ (SglFunc())->glCopyConvolutionFilter1D(
		target, internalformat, x, y, width); }
GLAPI void APIENTRY glCopyConvolutionFilter2D(
		GLenum target, GLenum internalformat, GLint x, GLint y,
		GLsizei width, GLsizei height)
	{ (SglFunc())->glCopyConvolutionFilter2D(
		target, internalformat, x, y, width, height); }
GLAPI void APIENTRY glGetConvolutionFilter(
		GLenum target, GLenum format, GLenum type, GLvoid *image)
	{ (SglFunc())->glGetConvolutionFilter(target, format, type, image); }
GLAPI void APIENTRY glGetConvolutionParameterfv(
		GLenum target, GLenum pname, GLfloat *params)
	{ (SglFunc())->glGetConvolutionParameterfv(target, pname, params); }
GLAPI void APIENTRY glGetConvolutionParameteriv(
		GLenum target, GLenum pname, GLint *params)
	{ (SglFunc())->glGetConvolutionParameteriv(target, pname, params); }
GLAPI void APIENTRY glSeparableFilter2D(
		GLenum target, GLenum internalformat, GLsizei width,
		GLsizei height, GLenum format, GLenum type,
		const GLvoid *row, const GLvoid *column)
	{ (SglFunc())->glSeparableFilter2D(target, internalformat,
		width, height, format, type, row, column); }
GLAPI void APIENTRY glGetSeparableFilter(
		GLenum target, GLenum format, GLenum type, GLvoid *row,
		GLvoid *column, GLvoid *span)
	{ (SglFunc())->glGetSeparableFilter(
		target, format, type, row, column, span); }

/* 1.3 functions */
GLAPI void APIENTRY glActiveTexture(
		GLenum texture)
	{ (SglFunc())->glActiveTexture(texture); }
GLAPI void APIENTRY glClientActiveTexture(
		GLenum texture)
	{ (SglFunc())->glClientActiveTexture(texture); }
GLAPI void APIENTRY glCompressedTexImage1D(
		GLenum target, GLint level, GLenum internalformat,
		GLsizei width, GLint border, GLsizei imageSize,
		const GLvoid *data)
	{ (SglFunc())->glCompressedTexImage1D(target, level, internalformat,
		width, border, imageSize, data); }
GLAPI void APIENTRY glCompressedTexImage2D(
		GLenum target, GLint level, GLenum internalformat,
		GLsizei width, GLsizei height, GLint border,
		GLsizei imageSize, const GLvoid *data)
	{ (SglFunc())->glCompressedTexImage2D(target, level, internalformat,
		width, height, border, imageSize, data); }
GLAPI void APIENTRY glCompressedTexImage3D(
		GLenum target, GLint level, GLenum internalformat,
		GLsizei width, GLsizei height, GLsizei depth, GLint border,
		GLsizei imageSize, const GLvoid *data)
	{ (SglFunc())->glCompressedTexImage3D(target, level, internalformat,
		width, height, depth, border, imageSize, data); }
GLAPI void APIENTRY glCompressedTexSubImage1D(
		GLenum target, GLint level, GLint xoffset, GLsizei width,
		GLenum format, GLsizei imageSize, const GLvoid *data)
	{ (SglFunc())->glCompressedTexSubImage1D(
		target, level, xoffset, width, format, imageSize, data); }
GLAPI void APIENTRY glCompressedTexSubImage2D(
		GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLsizei width, GLsizei height, GLenum format,
		GLsizei imageSize, const GLvoid *data)
	{ (SglFunc())->glCompressedTexSubImage2D(
		target, level, xoffset, yoffset, width, height,
		format, imageSize, data); }
GLAPI void APIENTRY glCompressedTexSubImage3D(
		GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,
		GLenum format, GLsizei imageSize, const GLvoid *data)
	{ (SglFunc())->glCompressedTexSubImage3D(
		target, level, xoffset, yoffset, zoffset, width, height, depth,
		format, imageSize, data); }
GLAPI void APIENTRY glGetCompressedTexImage(
		GLenum target, GLint lod, GLvoid *img)
	{ (SglFunc())->glGetCompressedTexImage(target, lod, img); }
GLAPI void APIENTRY glMultiTexCoord1d(
		GLenum target, GLdouble s)
	{ (SglFunc())->glMultiTexCoord1d(target, s); }
GLAPI void APIENTRY glMultiTexCoord1dv(
		GLenum target, const GLdouble *v)
	{ (SglFunc())->glMultiTexCoord1dv(target, v); }
GLAPI void APIENTRY glMultiTexCoord1f(
		GLenum target, GLfloat s)
	{ (SglFunc())->glMultiTexCoord1f(target, s); }
GLAPI void APIENTRY glMultiTexCoord1fv(
		GLenum target, const GLfloat *v)
	{ (SglFunc())->glMultiTexCoord1fv(target, v); }
GLAPI void APIENTRY glMultiTexCoord1i(
		GLenum target, GLint s)
	{ (SglFunc())->glMultiTexCoord1i(target, s); }
GLAPI void APIENTRY glMultiTexCoord1iv(
		GLenum target, const GLint *v)
	{ (SglFunc())->glMultiTexCoord1iv(target, v); }
GLAPI void APIENTRY glMultiTexCoord1s(
		GLenum target, GLshort s)
	{ (SglFunc())->glMultiTexCoord1s(target, s); }
GLAPI void APIENTRY glMultiTexCoord1sv(
		GLenum target, const GLshort *v)
	{ (SglFunc())->glMultiTexCoord1sv(target, v); }
GLAPI void APIENTRY glMultiTexCoord2d(
		GLenum target, GLdouble s, GLdouble t)
	{ (SglFunc())->glMultiTexCoord2d(target, s, t); }
GLAPI void APIENTRY glMultiTexCoord2dv(
		GLenum target, const GLdouble *v)
	{ (SglFunc())->glMultiTexCoord2dv(target, v); }
GLAPI void APIENTRY glMultiTexCoord2f(
		GLenum target, GLfloat s, GLfloat t)
	{ (SglFunc())->glMultiTexCoord2f(target, s, t); }
GLAPI void APIENTRY glMultiTexCoord2fv(
		GLenum target, const GLfloat *v)
	{ (SglFunc())->glMultiTexCoord2fv(target, v); }
GLAPI void APIENTRY glMultiTexCoord2i(
		GLenum target, GLint s, GLint t)
	{ (SglFunc())->glMultiTexCoord2i(target, s, t); }
GLAPI void APIENTRY glMultiTexCoord2iv(
		GLenum target, const GLint *v)
	{ (SglFunc())->glMultiTexCoord2iv(target, v); }
GLAPI void APIENTRY glMultiTexCoord2s(
		GLenum target, GLshort s, GLshort t)
	{ (SglFunc())->glMultiTexCoord2s(target, s, t); }
GLAPI void APIENTRY glMultiTexCoord2sv(
		GLenum target, const GLshort *v)
	{ (SglFunc())->glMultiTexCoord2sv(target, v); }
GLAPI void APIENTRY glMultiTexCoord3d(
		GLenum target, GLdouble s, GLdouble t, GLdouble r)
	{ (SglFunc())->glMultiTexCoord3d(target, s, t, r); }
GLAPI void APIENTRY glMultiTexCoord3dv(
		GLenum target, const GLdouble *v)
	{ (SglFunc())->glMultiTexCoord3dv(target, v); }
GLAPI void APIENTRY glMultiTexCoord3f(
		GLenum target, GLfloat s, GLfloat t, GLfloat r)
	{ (SglFunc())->glMultiTexCoord3f(target, s, t, r); }
GLAPI void APIENTRY glMultiTexCoord3fv(
		GLenum target, const GLfloat *v)
	{ (SglFunc())->glMultiTexCoord3fv(target, v); }
GLAPI void APIENTRY glMultiTexCoord3i(
		GLenum target, GLint s, GLint t, GLint r)
	{ (SglFunc())->glMultiTexCoord3i(target, s, t, r); }
GLAPI void APIENTRY glMultiTexCoord3iv(
		GLenum target, const GLint *v)
	{ (SglFunc())->glMultiTexCoord3iv(target, v); }
GLAPI void APIENTRY glMultiTexCoord3s(
		GLenum target, GLshort s, GLshort t, GLshort r)
	{ (SglFunc())->glMultiTexCoord3s(target, s, t, r); }
GLAPI void APIENTRY glMultiTexCoord3sv(
		GLenum target, const GLshort *v)
	{ (SglFunc())->glMultiTexCoord3sv(target, v); }
GLAPI void APIENTRY glMultiTexCoord4d(
		GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q)
	{ (SglFunc())->glMultiTexCoord4d(target, s, t, r, q); }
GLAPI void APIENTRY glMultiTexCoord4dv(
		GLenum target, const GLdouble *v)
	{ (SglFunc())->glMultiTexCoord4dv(target, v); }
GLAPI void APIENTRY glMultiTexCoord4f(
		GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q)
	{ (SglFunc())->glMultiTexCoord4f(target, s, t, r, q); }
GLAPI void APIENTRY glMultiTexCoord4fv(
		GLenum target, const GLfloat *v)
	{ (SglFunc())->glMultiTexCoord4fv(target, v); }
GLAPI void APIENTRY glMultiTexCoord4i(
		GLenum target, GLint s, GLint t, GLint r, GLint q)
	{ (SglFunc())->glMultiTexCoord4i(target, s, t, r, q); }
GLAPI void APIENTRY glMultiTexCoord4iv(
		GLenum target, const GLint *v)
	{ (SglFunc())->glMultiTexCoord4iv(target, v); }
GLAPI void APIENTRY glMultiTexCoord4s(
		GLenum target, GLshort s, GLshort t, GLshort r, GLshort q)
	{ (SglFunc())->glMultiTexCoord4s(target, s, t, r, q); }
GLAPI void APIENTRY glMultiTexCoord4sv(
		GLenum target, const GLshort *v)
	{ (SglFunc())->glMultiTexCoord4sv(target, v); }
GLAPI void APIENTRY glLoadTransposeMatrixd(
		const GLdouble m[16])
	{ (SglFunc())->glLoadTransposeMatrixd(m); }
GLAPI void APIENTRY glLoadTransposeMatrixf(
		const GLfloat m[16])
	{ (SglFunc())->glLoadTransposeMatrixf(m); }
GLAPI void APIENTRY glMultTransposeMatrixd(
		const GLdouble m[16])
	{ (SglFunc())->glMultTransposeMatrixd(m); }
GLAPI void APIENTRY glMultTransposeMatrixf(
		const GLfloat m[16])
	{ (SglFunc())->glMultTransposeMatrixf(m); }
GLAPI void APIENTRY glSampleCoverage(
		GLclampf value, GLboolean invert)
	{ (SglFunc())->glSampleCoverage(value, invert); }
GLAPI void APIENTRY glSamplePass(
		GLenum pass)
	{ (SglFunc())->glSamplePass(pass); }

#endif



GLAPI BOOL APIENTRY wglCopyContext(
		HGLRC hglrc1, HGLRC hglrc2, UINT mask)
	{ return((SglFunc())->wglCopyContext(hglrc1, hglrc2, mask)); }

GLAPI HGLRC APIENTRY wglCreateContext(HDC hdc)
	{ return((SglFunc())->wglCreateContext(hdc)); }

GLAPI HGLRC APIENTRY wglCreateLayerContext(
		HDC hdc, int iLayerPlane)
	{ return((SglFunc())->wglCreateLayerContext(hdc, iLayerPlane)); }

GLAPI BOOL APIENTRY wglDeleteContext(HGLRC hglrc)
	{ return((SglFunc())->wglDeleteContext(hglrc)); }

GLAPI BOOL APIENTRY wglDescribeLayerPlane(
		HDC a0, int a1, int a2, UINT a3, LPLAYERPLANEDESCRIPTOR a4)
	{ return((SglFunc())->wglDescribeLayerPlane(a0, a1, a2, a3, a4)); }

GLAPI HGLRC APIENTRY wglGetCurrentContext(void)
	{ return((SglFunc())->wglGetCurrentContext()); }

GLAPI HDC APIENTRY wglGetCurrentDC(void)
	{ return((SglFunc())->wglGetCurrentDC()); }

GLAPI int APIENTRY wglGetLayerPaletteEntries(
		HDC a0, int a1, int a2, int a3, COLORREF *a4)
	{ return((SglFunc())->wglGetLayerPaletteEntries(a0, a1, a2, a3, a4)); }

GLAPI PROC APIENTRY wglGetProcAddress(LPCSTR name)
{
	void *ptr;
	ptr=(SglFunc())->wglGetProcAddress(name);
	if(ptr)return(ptr);
	ptr=SglGetProcAddress(name);
	return(ptr);
}

GLAPI BOOL APIENTRY wglMakeCurrent(HDC hdc, HGLRC hglrc)
	{ return((SglFunc())->wglMakeCurrent(hdc, hglrc)); }

GLAPI BOOL APIENTRY wglRealizeLayerPalette(
		HDC a0, int a1, BOOL a2)
	{ return((SglFunc())->wglRealizeLayerPalette(a0, a1, a2)); }

GLAPI int APIENTRY wglSetLayerPaletteEntries(
		HDC a0, int a1, int a2, int a3, const COLORREF *a4)
	{ return((SglFunc())->wglSetLayerPaletteEntries(a0, a1, a2, a3, a4)); }

GLAPI BOOL APIENTRY wglShareLists(HGLRC a0, HGLRC a1)
	{ return((SglFunc())->wglShareLists(a0, a1)); }

GLAPI BOOL APIENTRY wglSwapLayerBuffers(HDC a0, UINT a1)
	{ return((SglFunc())->wglSwapLayerBuffers(a0, a1)); }

GLAPI BOOL APIENTRY wglUseFontBitmapsA(
		HDC a0, DWORD a1, DWORD a2, DWORD a3)
	{ return((SglFunc())->wglUseFontBitmapsA(a0, a1, a2, a3)); }

GLAPI BOOL APIENTRY wglUseFontBitmapsW(
		HDC a0, DWORD a1, DWORD a2, DWORD a3)
	{ return((SglFunc())->wglUseFontBitmapsW(a0, a1, a2, a3)); }

GLAPI BOOL APIENTRY wglUseFontOutlinesA(
		HDC a0, DWORD a1, DWORD a2, DWORD a3, FLOAT a4,
		FLOAT a5, int a6, LPGLYPHMETRICSFLOAT a7)
	{ return((SglFunc())->wglUseFontOutlinesA(
		a0, a1, a2, a3, a4, a5, a6, a7)); }

GLAPI BOOL APIENTRY wglUseFontOutlinesW(
		HDC a0, DWORD a1, DWORD a2, DWORD a3, FLOAT a4,
		FLOAT a5, int a6, LPGLYPHMETRICSFLOAT a7)
	{ return((SglFunc())->wglUseFontOutlinesW(
		a0, a1, a2, a3, a4, a5, a6, a7)); }

GLAPI int APIENTRY wglChoosePixelFormat(
		HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd)
	{ return((SglFunc())->wglChoosePixelFormat(hdc, ppfd)); }

GLAPI int APIENTRY wglGetPixelFormat(HDC hdc)
	{ return((SglFunc())->wglGetPixelFormat(hdc)); }

GLAPI BOOL APIENTRY wglSetPixelFormat(
		HDC hdc, INT iPixelFormat, const PIXELFORMATDESCRIPTOR *ppfd)
	{ return((SglFunc())->wglSetPixelFormat(hdc, iPixelFormat, ppfd)); }

GLAPI int APIENTRY wglDescribePixelFormat(
		HDC hdc, INT iPixelFormat, UINT nBytes,
		LPPIXELFORMATDESCRIPTOR ppfd)
	{ return((SglFunc())->wglDescribePixelFormat(hdc, iPixelFormat,
		nBytes, ppfd)); }

GLAPI BOOL APIENTRY wglSwapBuffers(HDC hdc)
	{ return((SglFunc())->wglSwapBuffers(hdc)); }
