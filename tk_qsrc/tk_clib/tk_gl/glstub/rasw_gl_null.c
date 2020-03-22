#include <tkgl/rasw_glstub.h>

void SglSetErrorNopStub(void)
{
}

/* miscellaneous */
GLDRV_API void GLDRV_APIENTRY GlDrv_glClearIndex(GLfloat c)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glClearColor(
		GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glClear(GLbitfield mask)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glIndexMask(GLuint mask)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColorMask(
		GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glAlphaFunc(GLenum func, GLclampf ref)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glBlendFunc(GLenum sfactor, GLenum dfactor)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glLogicOp(GLenum opcode)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glCullFace(GLenum mode)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glFrontFace(GLenum mode)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glPointSize(GLfloat size)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glLineWidth(GLfloat width)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glLineStipple(GLint factor, GLushort pattern)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glPolygonMode(GLenum face, GLenum mode)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glPolygonOffset(GLfloat factor, GLfloat units)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glPolygonStipple(const GLubyte *mask)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetPolygonStipple(GLubyte *mask)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glEdgeFlag(GLboolean flag)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glEdgeFlagv(const GLboolean *flag)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glScissor(
		GLint x, GLint y, GLsizei width, GLsizei height)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glClipPlane(GLenum plane, const GLdouble *equation)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetClipPlane(GLenum plane, GLdouble *equation)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glDrawBuffer(GLenum mode)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glReadBuffer(GLenum mode)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glEnable(GLenum cap)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glDisable(GLenum cap)
	{ SglSetErrorNopStub(); }
GLDRV_API GLboolean GLDRV_APIENTRY GlDrv_glIsEnabled(GLenum cap)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glEnableClientState(GLenum cap)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glDisableClientState(GLenum cap)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetBooleanv(GLenum pname, GLboolean *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetDoublev(GLenum pname, GLdouble *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetFloatv(GLenum pname, GLfloat *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetIntegerv(GLenum pname, GLint *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glPushAttrib(GLbitfield mask)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glPopAttrib(void)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glPushClientAttrib(GLbitfield mask)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glPopClientAttrib(void)
	{ SglSetErrorNopStub(); }
GLDRV_API GLint GLDRV_APIENTRY GlDrv_glRenderMode(GLenum mode)
	{ SglSetErrorNopStub(); }
GLDRV_API GLenum GLDRV_APIENTRY GlDrv_glGetError(void)
	{ return(GL_INVALID_OPERATION); }
GLDRV_API const GLubyte* GLDRV_APIENTRY GlDrv_glGetString(GLenum name)
	{ return(NULL); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glFinish(void)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glFlush(void)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glHint(GLenum target, GLenum mode)
	{ SglSetErrorNopStub(); }

/* Depth Buffer */
GLDRV_API void GLDRV_APIENTRY GlDrv_glClearDepth(GLclampd depth)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glDepthFunc(GLenum func)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glDepthMask(GLboolean flag)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glDepthRange(GLclampd near_val, GLclampd far_val)
	{ SglSetErrorNopStub(); }

/* Accumulation Buffer */
GLDRV_API void GLDRV_APIENTRY GlDrv_glClearAccum(
		GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glAccum(GLenum op, GLfloat value)
	{ SglSetErrorNopStub(); }

/* Transformation */
GLDRV_API void GLDRV_APIENTRY GlDrv_glMatrixMode(GLenum mode)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glOrtho(
		GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, 
		GLdouble near_val, GLdouble far_val)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glFrustum(
		GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, 
		GLdouble near_val, GLdouble far_val)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glViewport(
		GLint x, GLint y, GLsizei width, GLsizei height)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glPushMatrix(void)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glPopMatrix(void)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glLoadIdentity(void)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glLoadMatrixd(const GLdouble *m)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glLoadMatrixf(const GLfloat *m)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultMatrixd(const GLdouble *m)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultMatrixf(const GLfloat *m)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRotated(
		GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRotatef(
		GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glScaled(GLdouble x, GLdouble y, GLdouble z)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glScalef(GLfloat x, GLfloat y, GLfloat z)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTranslated(GLdouble x, GLdouble y, GLdouble z)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTranslatef(GLfloat x, GLfloat y, GLfloat z)
	{ SglSetErrorNopStub(); }

/* Display Lists */
GLDRV_API GLboolean GLDRV_APIENTRY GlDrv_glIsList(GLuint list)
	{ return(0); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glDeleteLists(GLuint list, GLsizei range)
	{ SglSetErrorNopStub(); }
GLDRV_API GLuint GLDRV_APIENTRY GlDrv_glGenLists(GLsizei range)
	{ return(-1); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glNewList(GLuint list, GLenum mode)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glEndList(void)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glCallList(GLuint list)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glCallLists(
		GLsizei n, GLenum type, const GLvoid *lists)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glListBase(GLuint base)
	{ SglSetErrorNopStub(); }

/* Drawing Functions */
GLDRV_API void GLDRV_APIENTRY GlDrv_glBegin(GLenum mode)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glEnd(void)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertex2d(GLdouble x, GLdouble y)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertex2f(GLfloat x, GLfloat y)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertex2i(GLint x, GLint y)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertex2s(GLshort x, GLshort y)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertex3d(GLdouble x, GLdouble y, GLdouble z)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertex3f(GLfloat x, GLfloat y, GLfloat z)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertex3i(GLint x, GLint y, GLint z)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertex3s(GLshort x, GLshort y, GLshort z)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertex4d(
		GLdouble x, GLdouble y, GLdouble z, GLdouble w)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertex4f(
		GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertex4i(GLint x, GLint y, GLint z, GLint w)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertex4s(GLshort x, GLshort y, GLshort z, GLshort w)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertex2dv(const GLdouble *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertex2fv(const GLfloat *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertex2iv(const GLint *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertex2sv(const GLshort *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertex3dv(const GLdouble *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertex3fv(const GLfloat *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertex3iv(const GLint *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertex3sv(const GLshort *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertex4dv(const GLdouble *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertex4fv(const GLfloat *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertex4iv(const GLint *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertex4sv(const GLshort *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glNormal3b(GLbyte nx, GLbyte ny, GLbyte nz)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glNormal3d(GLdouble nx, GLdouble ny, GLdouble nz)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glNormal3i(GLint nx, GLint ny, GLint nz)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glNormal3s(GLshort nx, GLshort ny, GLshort nz)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glNormal3bv(const GLbyte *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glNormal3dv(const GLdouble *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glNormal3fv(const GLfloat *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glNormal3iv(const GLint *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glNormal3sv(const GLshort *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glIndexd(GLdouble c)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glIndexf(GLfloat c)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glIndexi(GLint c)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glIndexs(GLshort c)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glIndexub(GLubyte c)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glIndexdv(const GLdouble *c)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glIndexfv(const GLfloat *c)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glIndexiv(const GLint *c)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glIndexsv(const GLshort *c)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glIndexubv(const GLubyte *c)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor3b(GLbyte red, GLbyte green, GLbyte blue)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor3d(GLdouble red, GLdouble green, GLdouble blue)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor3f(GLfloat red, GLfloat green, GLfloat blue)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor3i(GLint red, GLint green, GLint blue)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor3s(GLshort red, GLshort green, GLshort blue)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor3ub(GLubyte red, GLubyte green, GLubyte blue)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor3ui(GLuint red, GLuint green, GLuint blue)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor3us(GLushort red, GLushort green, GLushort blue)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor4b(
		GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor4d(
		GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor4f(
		GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor4i(
		GLint red, GLint green, GLint blue, GLint alpha)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor4s(
		GLshort red, GLshort green, GLshort blue, GLshort alpha)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor4ub(
		GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor4ui(
		GLuint red, GLuint green, GLuint blue, GLuint alpha)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor4us(
		GLushort red, GLushort green, GLushort blue, GLushort alpha)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor3bv(const GLbyte *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor3dv(const GLdouble *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor3fv(const GLfloat *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor3iv(const GLint *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor3sv(const GLshort *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor3ubv(const GLubyte *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor3uiv(const GLuint *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor3usv(const GLushort *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor4bv(const GLbyte *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor4dv(const GLdouble *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor4fv(const GLfloat *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor4iv(const GLint *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor4sv(const GLshort *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor4ubv(const GLubyte *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor4uiv(const GLuint *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColor4usv(const GLushort *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord1d(GLdouble s)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord1f(GLfloat s)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord1i(GLint s)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord1s(GLshort s)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord2d(GLdouble s, GLdouble t)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord2f(GLfloat s, GLfloat t)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord2i(GLint s, GLint t)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord2s(GLshort s, GLshort t)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord3d(GLdouble s, GLdouble t, GLdouble r)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord3f(GLfloat s, GLfloat t, GLfloat r)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord3i(GLint s, GLint t, GLint r)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord3s(GLshort s, GLshort t, GLshort r)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord4d(
		GLdouble s, GLdouble t, GLdouble r, GLdouble q)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord4f(
		GLfloat s, GLfloat t, GLfloat r, GLfloat q)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord4i(GLint s, GLint t, GLint r, GLint q)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord4s(
		GLshort s, GLshort t, GLshort r, GLshort q)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord1dv(const GLdouble *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord1fv(const GLfloat *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord1iv(const GLint *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord1sv(const GLshort *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord2dv(const GLdouble *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord2fv(const GLfloat *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord2iv(const GLint *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord2sv(const GLshort *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord3dv(const GLdouble *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord3fv(const GLfloat *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord3iv(const GLint *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord3sv(const GLshort *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord4dv(const GLdouble *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord4fv(const GLfloat *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord4iv(const GLint *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoord4sv(const GLshort *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRasterPos2d(GLdouble x, GLdouble y)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRasterPos2f(GLfloat x, GLfloat y)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRasterPos2i(GLint x, GLint y)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRasterPos2s(GLshort x, GLshort y)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRasterPos3d(GLdouble x, GLdouble y, GLdouble z)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRasterPos3f(GLfloat x, GLfloat y, GLfloat z)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRasterPos3i(GLint x, GLint y, GLint z)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRasterPos3s(GLshort x, GLshort y, GLshort z)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRasterPos4d(
		GLdouble x, GLdouble y, GLdouble z, GLdouble w)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRasterPos4f(
		GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRasterPos4i(GLint x, GLint y, GLint z, GLint w)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRasterPos4s(
		GLshort x, GLshort y, GLshort z, GLshort w)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRasterPos2dv(const GLdouble *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRasterPos2fv(const GLfloat *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRasterPos2iv(const GLint *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRasterPos2sv(const GLshort *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRasterPos3dv(const GLdouble *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRasterPos3fv(const GLfloat *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRasterPos3iv(const GLint *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRasterPos3sv(const GLshort *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRasterPos4dv(const GLdouble *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRasterPos4fv(const GLfloat *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRasterPos4iv(const GLint *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRasterPos4sv(const GLshort *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRectd(
		GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRectf(
		GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRecti(GLint x1, GLint y1, GLint x2, GLint y2)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRects(GLshort x1, GLshort y1, GLshort x2, GLshort y2)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRectdv(const GLdouble *v1, const GLdouble *v2)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRectfv(const GLfloat *v1, const GLfloat *v2)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRectiv(const GLint *v1, const GLint *v2)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glRectsv(const GLshort *v1, const GLshort *v2)
	{ SglSetErrorNopStub(); }

/* Lighting */
GLDRV_API void GLDRV_APIENTRY GlDrv_glShadeModel(GLenum mode)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glLightf(GLenum light, GLenum pname, GLfloat param)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glLighti(GLenum light, GLenum pname, GLint param)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glLightfv(
		GLenum light, GLenum pname, const GLfloat *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glLightiv(
		GLenum light, GLenum pname, const GLint *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetLightfv(
		GLenum light, GLenum pname, GLfloat *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetLightiv(GLenum light, GLenum pname, GLint *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glLightModelf(GLenum pname, GLfloat param)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glLightModeli(GLenum pname, GLint param)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glLightModelfv(GLenum pname, const GLfloat *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glLightModeliv(GLenum pname, const GLint *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMaterialf(GLenum face, GLenum pname, GLfloat param)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMateriali(GLenum face, GLenum pname, GLint param)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMaterialfv(
		GLenum face, GLenum pname, const GLfloat *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMaterialiv(
		GLenum face, GLenum pname, const GLint *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetMaterialfv(
		GLenum face, GLenum pname, GLfloat *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetMaterialiv(
		GLenum face, GLenum pname, GLint *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColorMaterial(GLenum face, GLenum mode)
	{ SglSetErrorNopStub(); }

/* Raster functions */
GLDRV_API void GLDRV_APIENTRY GlDrv_glPixelZoom(GLfloat xfactor, GLfloat yfactor)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glPixelStoref(GLenum pname, GLfloat param)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glPixelStorei(GLenum pname, GLint param)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glPixelTransferf(GLenum pname, GLfloat param)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glPixelTransferi(GLenum pname, GLint param)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glPixelMapfv(
		GLenum map, GLint mapsize, const GLfloat *values)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glPixelMapuiv(
		GLenum map, GLint mapsize, const GLuint *values)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glPixelMapusv(
		GLenum map, GLint mapsize, const GLushort *values)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetPixelMapfv(GLenum map, GLfloat *values)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetPixelMapuiv(GLenum map, GLuint *values)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetPixelMapusv(GLenum map, GLushort *values)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glBitmap(
	GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig,
	GLfloat xmove, GLfloat ymove, const GLubyte *bitmap)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glReadPixels(
		GLint x, GLint y, GLsizei width, GLsizei height,
		GLenum format, GLenum type, GLvoid *pixels)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glDrawPixels(
		GLsizei width, GLsizei height, GLenum format, GLenum type,
		const GLvoid *pixels)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glCopyPixels(
		GLint x, GLint y, GLsizei width, GLsizei height, GLenum type)
	{ SglSetErrorNopStub(); }

/* Stenciling */
GLDRV_API void GLDRV_APIENTRY GlDrv_glStencilFunc(GLenum func, GLint ref, GLuint mask)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glStencilMask(GLuint mask)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glClearStencil(GLint s)
	{ SglSetErrorNopStub(); }

/* Texture mapping */
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexGend(GLenum coord, GLenum pname, GLdouble param)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexGenf(GLenum coord, GLenum pname, GLfloat param)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexGeni(GLenum coord, GLenum pname, GLint param)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexGendv(
		GLenum coord, GLenum pname, const GLdouble *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexGenfv(
		GLenum coord, GLenum pname, const GLfloat *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexGeniv(
		GLenum coord, GLenum pname, const GLint *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetTexGendv(
		GLenum coord, GLenum pname, GLdouble *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetTexGenfv(
		GLenum coord, GLenum pname, GLfloat *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetTexGeniv(GLenum coord, GLenum pname, GLint *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexEnvf(GLenum target, GLenum pname, GLfloat param)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexEnvi(GLenum target, GLenum pname, GLint param)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexEnvfv(
		GLenum target, GLenum pname, const GLfloat *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexEnviv(
		GLenum target, GLenum pname, const GLint *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetTexEnvfv(
		GLenum target, GLenum pname, GLfloat *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetTexEnviv(
		GLenum target, GLenum pname, GLint *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexParameterf(
		GLenum target, GLenum pname, GLfloat param)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexParameteri(
		GLenum target, GLenum pname, GLint param)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexParameterfv(
		GLenum target, GLenum pname, const GLfloat *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexParameteriv(
		GLenum target, GLenum pname, const GLint *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetTexParameterfv(
		GLenum target, GLenum pname, GLfloat *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetTexParameteriv(
		GLenum target, GLenum pname, GLint *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetTexLevelParameterfv(
		GLenum target, GLint level, GLenum pname, GLfloat *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetTexLevelParameteriv(
		GLenum target, GLint level, GLenum pname, GLint *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexImage1D(
		GLenum target, GLint level, GLint internalFormat,
		GLsizei width, GLint border, GLenum format, GLenum type,
		const GLvoid *pixels)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexImage2D(
		GLenum target, GLint level, GLint internalFormat,
		GLsizei width, GLsizei height, GLint border, GLenum format,
		GLenum type, const GLvoid *pixels)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetTexImage(
		GLenum target, GLint level, GLenum format, GLenum type,
		GLvoid *pixels)
	{ SglSetErrorNopStub(); }

/* Evaluators */
GLDRV_API void GLDRV_APIENTRY GlDrv_glMap1d(
		GLenum target, GLdouble u1, GLdouble u2, GLint stride,
		GLint order, const GLdouble *points)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMap1f(
		GLenum target, GLfloat u1, GLfloat u2, GLint stride,
		GLint order, const GLfloat *points)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMap2d(
		GLenum target, GLdouble u1, GLdouble u2, GLint ustride,
		GLint uorder, GLdouble v1, GLdouble v2, GLint vstride,
		GLint vorder, const GLdouble *points)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMap2f(
		GLenum target, GLfloat u1, GLfloat u2, GLint ustride,
		GLint uorder, GLfloat v1, GLfloat v2, GLint vstride,
		GLint vorder, const GLfloat *points)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetMapdv(GLenum target, GLenum query, GLdouble *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetMapfv(GLenum target, GLenum query, GLfloat *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetMapiv(GLenum target, GLenum query, GLint *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glEvalCoord1d(GLdouble u)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glEvalCoord1f(GLfloat u)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glEvalCoord1dv(const GLdouble *u)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glEvalCoord1fv(const GLfloat *u)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glEvalCoord2d(GLdouble u, GLdouble v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glEvalCoord2f(GLfloat u, GLfloat v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glEvalCoord2dv(const GLdouble *u)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glEvalCoord2fv(const GLfloat *u)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMapGrid1d(GLint un, GLdouble u1, GLdouble u2)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMapGrid1f(GLint un, GLfloat u1, GLfloat u2)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMapGrid2d(
		GLint un, GLdouble u1, GLdouble u2, GLint vn,
		GLdouble v1, GLdouble v2)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMapGrid2f(
		GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glEvalPoint1(GLint i)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glEvalPoint2(GLint i, GLint j)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glEvalMesh1(GLenum mode, GLint i1, GLint i2)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glEvalMesh2(
		GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2)
	{ SglSetErrorNopStub(); }

/* Fog */
GLDRV_API void GLDRV_APIENTRY GlDrv_glFogf(GLenum pname, GLfloat param)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glFogi(GLenum pname, GLint param)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glFogfv(GLenum pname, const GLfloat *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glFogiv(GLenum pname, const GLint *params)
	{ SglSetErrorNopStub(); }

/* Selection and Feedback */
GLDRV_API void GLDRV_APIENTRY GlDrv_glFeedbackBuffer(
		GLsizei size, GLenum type, GLfloat *buffer)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glPassThrough(GLfloat token)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glSelectBuffer(GLsizei size, GLuint *buffer)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glInitNames(void)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glLoadName(GLuint name)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glPushName(GLuint name)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glPopName(void)
	{ SglSetErrorNopStub(); }


/* 1.1 functions */
/* texture objects */
GLDRV_API void GLDRV_APIENTRY GlDrv_glGenTextures(GLsizei n, GLuint *textures)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glDeleteTextures(GLsizei n, const GLuint *textures)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glBindTexture(GLenum target, GLuint texture)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glPrioritizeTextures(
		GLsizei n, const GLuint *textures, const GLclampf *priorities)
	{ SglSetErrorNopStub(); }
GLDRV_API GLboolean GLDRV_APIENTRY GlDrv_glAreTexturesResident(
		GLsizei n, const GLuint *textures, GLboolean *residences)
	{ SglSetErrorNopStub(); }
GLDRV_API GLboolean GLDRV_APIENTRY GlDrv_glIsTexture(GLuint texture)
	{ SglSetErrorNopStub(); }
/* texture mapping */
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexSubImage1D(
		GLenum target, GLint level, GLint xoffset, GLsizei width,
		GLenum format, GLenum type, const GLvoid *pixels)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexSubImage2D(
		GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLsizei width, GLsizei height, GLenum format, GLenum type,
		const GLvoid *pixels)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glCopyTexImage1D(
		GLenum target, GLint level, GLenum internalformat,
		GLint x, GLint y, GLsizei width, GLint border)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glCopyTexImage2D(
		GLenum target, GLint level, GLenum internalformat,
		GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glCopyTexSubImage1D(
		GLenum target, GLint level, GLint xoffset, GLint x, GLint y,
		GLsizei width)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glCopyTexSubImage2D(
		GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLint x, GLint y, GLsizei width, GLsizei height)
	{ SglSetErrorNopStub(); }
/* vertex arrays */
GLDRV_API void GLDRV_APIENTRY GlDrv_glVertexPointer(
		GLint size, GLenum type, GLsizei stride, const GLvoid *ptr)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glNormalPointer(
		GLenum type, GLsizei stride, const GLvoid *ptr)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColorPointer(
		GLint size, GLenum type, GLsizei stride, const GLvoid *ptr)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glIndexPointer(
		GLenum type, GLsizei stride, const GLvoid *ptr)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexCoordPointer(
		GLint size, GLenum type, GLsizei stride, const GLvoid *ptr)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glEdgeFlagPointer(GLsizei stride, const GLvoid *ptr)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetPointerv(GLenum pname, GLvoid **params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glArrayElement(GLint i)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glDrawArrays(
		GLenum mode, GLint first, GLsizei count)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glDrawElements(
		GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glInterleavedArrays(
		GLenum format, GLsizei stride, const GLvoid *pointer)
	{ SglSetErrorNopStub(); }


/* 1.2 functions */
GLDRV_API void GLDRV_APIENTRY GlDrv_glDrawRangeElements(
		GLenum mode, GLuint start, GLuint end, GLsizei count,	
		GLenum type, const GLvoid *indices)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexImage3D(
		GLenum target, GLint level, GLenum internalFormat,
		GLsizei width, GLsizei height, GLsizei depth, GLint border,
		GLenum format, GLenum type, const GLvoid *pixels)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glTexSubImage3D(
		GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,
		GLenum format, GLenum type, const GLvoid *pixels)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glCopyTexSubImage3D(
		GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
	{ SglSetErrorNopStub(); }

/* 1.2 imaging extension functions */
GLDRV_API void GLDRV_APIENTRY GlDrv_glColorTable(
		GLenum target, GLenum internalformat, GLsizei width,
		GLenum format, GLenum type, const GLvoid *table)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColorSubTable(
		GLenum target, GLsizei start, GLsizei count, GLenum format,
		GLenum type, const GLvoid *data)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColorTableParameteriv(
		GLenum target, GLenum pname, const GLint *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glColorTableParameterfv(
		GLenum target, GLenum pname, const GLfloat *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glCopyColorSubTable(
		GLenum target, GLsizei start, GLint x, GLint y, GLsizei width)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glCopyColorTable(
		GLenum target, GLenum internalformat, GLint x, GLint y,
		GLsizei width)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetColorTable(
		GLenum target, GLenum format, GLenum type, GLvoid *table)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetColorTableParameterfv(
		GLenum target, GLenum pname, GLfloat *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetColorTableParameteriv(
		GLenum target, GLenum pname, GLint *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glBlendEquation(GLenum mode)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glBlendColor(
		GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glHistogram(
		GLenum target, GLsizei width, GLenum internalformat, GLboolean sink)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glResetHistogram(GLenum target)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetHistogram(
		GLenum target, GLboolean reset, GLenum format,
		GLenum type, GLvoid *values)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetHistogramParameterfv(
		GLenum target, GLenum pname, GLfloat *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetHistogramParameteriv(
		GLenum target, GLenum pname, GLint *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMinmax(
		GLenum target, GLenum internalformat, GLboolean sink)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glResetMinmax(GLenum target)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetMinmax(
		GLenum target, GLboolean reset, GLenum format,
		GLenum types, GLvoid *values)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetMinmaxParameterfv(
		GLenum target, GLenum pname, GLfloat *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetMinmaxParameteriv(
		GLenum target, GLenum pname, GLint *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glConvolutionFilter1D(
		GLenum target, GLenum internalformat, GLsizei width,
		GLenum format, GLenum type, const GLvoid *image)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glConvolutionFilter2D(
		GLenum target, GLenum internalformat,
		GLsizei width, GLsizei height, GLenum format,
		GLenum type, const GLvoid *image)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glConvolutionParameterf(
		GLenum target, GLenum pname, GLfloat params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glConvolutionParameterfv(
		GLenum target, GLenum pname, const GLfloat *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glConvolutionParameteri(
		GLenum target, GLenum pname, GLint params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glConvolutionParameteriv(
		GLenum target, GLenum pname, const GLint *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glCopyConvolutionFilter1D(
		GLenum target, GLenum internalformat, GLint x, GLint y,
		GLsizei width)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glCopyConvolutionFilter2D(
		GLenum target, GLenum internalformat, GLint x, GLint y,
		GLsizei width, GLsizei height)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetConvolutionFilter(
		GLenum target, GLenum format, GLenum type, GLvoid *image)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetConvolutionParameterfv(
		GLenum target, GLenum pname, GLfloat *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetConvolutionParameteriv(
		GLenum target, GLenum pname, GLint *params)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glSeparableFilter2D(
		GLenum target, GLenum internalformat,
		GLsizei width, GLsizei height, GLenum format,
		GLenum type, const GLvoid *row, const GLvoid *column)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetSeparableFilter(
		GLenum target, GLenum format, GLenum type,
		GLvoid *row, GLvoid *column, GLvoid *span)
	{ SglSetErrorNopStub(); }

/* 1.3 functions */
GLDRV_API void GLDRV_APIENTRY GlDrv_glActiveTexture(GLenum texture)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glClientActiveTexture(GLenum texture)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glCompressedTexImage1D(
		GLenum target, GLint level, GLenum internalformat,
		GLsizei width, GLint border, GLsizei imageSize,
		const GLvoid *data)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glCompressedTexImage2D(
		GLenum target, GLint level, GLenum internalformat,
		GLsizei width, GLsizei height, GLint border,
		GLsizei imageSize, const GLvoid *data)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glCompressedTexImage3D(
		GLenum target, GLint level, GLenum internalformat,
		GLsizei width, GLsizei height, GLsizei depth, GLint border,
		GLsizei imageSize, const GLvoid *data)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glCompressedTexSubImage1D(
		GLenum target, GLint level, GLint xoffset, GLsizei width,
		GLenum format, GLsizei imageSize, const GLvoid *data)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glCompressedTexSubImage2D(
		GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLsizei width, GLsizei height, GLenum format,
		GLsizei imageSize, const GLvoid *data)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glCompressedTexSubImage3D(
		GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,
		GLenum format, GLsizei imageSize, const GLvoid *data)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glGetCompressedTexImage(
		GLenum target, GLint lod, GLvoid *img)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord1d(GLenum target, GLdouble s)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord1dv(GLenum target, const GLdouble *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord1f(GLenum target, GLfloat s)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord1fv(GLenum target, const GLfloat *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord1i(GLenum target, GLint s)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord1iv(GLenum target, const GLint *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord1s(GLenum target, GLshort s)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord1sv(GLenum target, const GLshort *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord2d(GLenum target, GLdouble s, GLdouble t)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord2dv(GLenum target, const GLdouble *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord2f(GLenum target, GLfloat s, GLfloat t)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord2fv(GLenum target, const GLfloat *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord2i(GLenum target, GLint s, GLint t)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord2iv(GLenum target, const GLint *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord2s(GLenum target, GLshort s, GLshort t)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord2sv(GLenum target, const GLshort *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord3d(
		GLenum target, GLdouble s, GLdouble t, GLdouble r)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord3dv(GLenum target, const GLdouble *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord3f(
		GLenum target, GLfloat s, GLfloat t, GLfloat r)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord3fv(GLenum target, const GLfloat *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord3i(
		GLenum target, GLint s, GLint t, GLint r)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord3iv(GLenum target, const GLint *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord3s(
		GLenum target, GLshort s, GLshort t, GLshort r)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord3sv(GLenum target, const GLshort *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord4d(
		GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord4dv(GLenum target, const GLdouble *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord4f(
		GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord4fv(GLenum target, const GLfloat *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord4i(
		GLenum target, GLint s, GLint t, GLint r, GLint q)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord4iv(GLenum target, const GLint *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord4s(
		GLenum target, GLshort s, GLshort t, GLshort r, GLshort q)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord4sv(GLenum target, const GLshort *v)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glLoadTransposeMatrixd(const GLdouble m[16])
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glLoadTransposeMatrixf(const GLfloat m[16])
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultTransposeMatrixd(const GLdouble m[16])
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glMultTransposeMatrixf(const GLfloat m[16])
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glSampleCoverage(GLclampf value, GLboolean invert)
	{ SglSetErrorNopStub(); }
GLDRV_API void GLDRV_APIENTRY GlDrv_glSamplePass(GLenum pass)
	{ SglSetErrorNopStub(); }
