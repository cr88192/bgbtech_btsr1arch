/*
Intention is to provide a general wrapper interface for the basic parts of the OpenGL API.
 */

void *(*tkgl_glGetProcAddress_fp)(char *name);

int tkgl_gpa_dummy()
{
	return(0);
}

void *glGetProcAddress(char *name)
{
	if(!tkgl_glGetProcAddress_fp)
		tkgl_glGetProcAddress_fp=tkra_wglGetProcAddress;
	return(tkgl_glGetProcAddress_fp(name));
}

void *glGetProcAddressNoNull(char *name)
{
	void *ptr;
	ptr=glGetProcAddress(name);
	if(!ptr)
		ptr=tkgl_gpa_dummy;
	return(ptr);
}


GLAPI void APIENTRY glClearIndex(GLfloat c)
{
	static void (*func)(GLfloat c);
	if(!func)
		func=glGetProcAddressNoNull("glClearIndex");
	func(c);
}

GLAPI void APIENTRY glClearColor(
	GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	static void (*func)(
		GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
	if(!func)
		func=glGetProcAddressNoNull("glClearColor");
	func(red, green, blue, alpha);
}

GLAPI void APIENTRY glClear(GLbitfield mask)
{
	static void (*func)(GLbitfield c);
	if(!func)
		func=glGetProcAddressNoNull("glClear");
	func(mask);
}

GLAPI void APIENTRY glIndexMask(GLuint mask)
{
	static void (*func)(GLuint c);
	if(!func)
		func=glGetProcAddressNoNull("glIndexMask");
	func(mask);
}

GLAPI void APIENTRY glColorMask(
	GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
	static void (*func)(
		GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
	if(!func)
		func=glGetProcAddressNoNull("glColorMask");
	func(red, green, blue, alpha);
}

GLAPI void APIENTRY glAlphaFunc(GLenum func, GLclampf ref)
{
	static void (*func)(GLenum func, GLclampf ref);
	if(!func)
		func=glGetProcAddressNoNull("glAlphaFunc");
	func(func, ref);
}

GLAPI void APIENTRY glBlendFunc(GLenum sfactor, GLenum dfactor)
{
	static void (*func)(GLenum sfactor, GLenum dfactor);
	if(!func)
		func=glGetProcAddressNoNull("glBlendFunc");
	func(sfactor, dfactor);
}

GLAPI void APIENTRY glLogicOp(GLenum opcode)
{
	static void (*func)(GLenum opcode);
	if(!func)
		func=glGetProcAddressNoNull("glLogicOp");
	func(opcode);
}

GLAPI void APIENTRY glCullFace(GLenum mode)
{
	static void (*func)(GLenum mode);
	if(!func)
		func=glGetProcAddressNoNull("glCullFace");
	func(mode);
}

GLAPI void APIENTRY glFrontFace(GLenum mode)
{
	static void (*func)(GLenum mode);
	if(!func)
		func=glGetProcAddressNoNull("glFrontFace");
	func(mode);
}

GLAPI void APIENTRY glPointSize(GLfloat size)
{
	static void (*func)(GLfloat size);
	if(!func)
		func=glGetProcAddressNoNull("glPointSize");
	func(size);
}

GLAPI void APIENTRY glLineWidth(GLfloat width)
{
	static void (*func)(GLfloat width);
	if(!func)
		func=glGetProcAddressNoNull("glLineWidth");
	func(width);
}

GLAPI void APIENTRY glLineStipple(GLint factor, GLushort pattern)
{
	static void (*func)(GLint factor, GLushort pattern);
	if(!func)
		func=glGetProcAddressNoNull("glLineStipple");
	func(factor, pattern);
}

GLAPI void APIENTRY glPolygonMode(GLenum face, GLenum mode)
{
	static void (*func)(GLenum face, GLenum mode);
	if(!func)
		func=glGetProcAddressNoNull("glPolygonMode");
	func(face, mode);
}

GLAPI void APIENTRY glPolygonOffset(GLfloat factor, GLfloat units)
{
	static void (*func)(GLint factor, GLfloat units);
	if(!func)
		func=glGetProcAddressNoNull("glPolygonOffset");
	func(factor, units);
}

GLAPI void APIENTRY glPolygonStipple(const GLubyte *mask)
{
	static void (*func)(GLubyte *mask);
	if(!func)
		func=glGetProcAddressNoNull("glPolygonStipple");
	func(mask);
}

GLAPI void APIENTRY glGetPolygonStipple(GLubyte *mask)
{
	static void (*func)(GLubyte *mask);
	if(!func)
		func=glGetProcAddressNoNull("glGetPolygonStipple");
	func(mask);
}

GLAPI void APIENTRY glEdgeFlag(GLboolean flag)
{
	static void (*func)(GLboolean flag);
	if(!func)
		func=glGetProcAddressNoNull("glEdgeFlag");
	func(flag);
}

GLAPI void APIENTRY glEdgeFlagv(const GLboolean *flag)
{
	static void (*func)(GLboolean *flag);
	if(!func)
		func=glGetProcAddressNoNull("glEdgeFlagv");
	func(flag);
}

GLAPI void APIENTRY glScissor(
	GLint x, GLint y, GLsizei width, GLsizei height)
{
	static void (*func)(
		GLint x, GLint y, GLsizei width, GLsizei height);
	if(!func)
		func=glGetProcAddressNoNull("glScissor");
	func(x, y, width, height);
}

GLAPI void APIENTRY glClipPlane(GLenum plane, const GLdouble *equation)
{
	static void (*func)(GLenum plane, GLdouble *equation);
	if(!func)
		func=glGetProcAddressNoNull("glClipPlane");
	func(plane, equation);
}

GLAPI void APIENTRY glGetClipPlane(GLenum plane, GLdouble *equation)
{
	static void (*func)(GLenum plane, GLdouble *equation);
	if(!func)
		func=glGetProcAddressNoNull("glGetClipPlane");
	func(plane, equation);
}

GLAPI void APIENTRY glDrawBuffer(GLenum mode)
{
	static void (*func)(GLenum mode);
	if(!func)
		func=glGetProcAddressNoNull("glDrawBuffer");
	func(mode);
}

GLAPI void APIENTRY glReadBuffer(GLenum mode)
{
	static void (*func)(GLenum mode);
	if(!func)
		func=glGetProcAddressNoNull("glReadBuffer");
	func(mode);
}

GLAPI void APIENTRY glEnable(GLenum cap)
{
	static void (*func)(GLenum cap);
	if(!func)
		func=glGetProcAddressNoNull("glEnable");
	func(cap);
}

GLAPI void APIENTRY glDisable(GLenum cap)
{
	static void (*func)(GLenum cap);
	if(!func)
		func=glGetProcAddressNoNull("glDisable");
	func(cap);
}

GLAPI GLboolean APIENTRY glIsEnabled(GLenum cap)
{
	static GLboolean (*func)(GLenum cap);
	if(!func)
		func=glGetProcAddressNoNull("glIsEnabled");
	return(func(cap));
}

GLAPI void APIENTRY glEnableClientState(GLenum cap)
{
	static void (*func)(GLenum cap);
	if(!func)
		func=glGetProcAddressNoNull("glDisable");
	func(cap);
}

GLAPI void APIENTRY glDisableClientState(GLenum cap)
{
	static void (*func)(GLenum cap);
	if(!func)
		func=glGetProcAddressNoNull("glDisable");
	func(cap);
}

GLAPI void APIENTRY glGetBooleanv(GLenum pname, GLboolean *params)
{
	static void (*func)(GLenum pname, GLboolean *params);
	if(!func)
		func=glGetProcAddressNoNull("glGetBooleanv");
	func(pname, params);
}

GLAPI void APIENTRY glGetDoublev(GLenum pname, GLdouble *params)
{
	static void (*func)(GLenum pname, GLdouble *params);
	if(!func)
		func=glGetProcAddressNoNull("glGetDoublev");
	func(pname, params);
}

GLAPI void APIENTRY glGetFloatv(GLenum pname, GLfloat *params)
{
	static void (*func)(GLenum pname, GLfloat *params);
	if(!func)
		func=glGetProcAddressNoNull("glGetFloatv");
	func(pname, params);
}

GLAPI void APIENTRY glGetIntegerv(GLenum pname, GLint *params)
{
	static void (*func)(GLenum pname, GLint *params);
	if(!func)
		func=glGetProcAddressNoNull("glGetIntegerv");
	func(pname, params);
}

GLAPI void APIENTRY glPushAttrib(GLbitfield mask)
{
	static void (*func)(GLbitfield mask);
	if(!func)
		func=glGetProcAddressNoNull("glPushAttrib");
	func(mask);
}

GLAPI void APIENTRY glPopAttrib(void)
{
	static void (*func)();
	if(!func)
		func=glGetProcAddressNoNull("glEnd");
	func();
}

GLAPI void APIENTRY glPushClientAttrib(GLbitfield mask)
{
	static void (*func)(GLbitfield mask);
	if(!func)
		func=glGetProcAddressNoNull("glPushClientAttrib");
	func(mask);
}

GLAPI void APIENTRY glPopClientAttrib(void)
{
	static void (*func)();
	if(!func)
		func=glGetProcAddressNoNull("glPopClientAttrib");
	func();
}

GLAPI GLint APIENTRY glRenderMode(GLenum mode)
{
	static GLint (*func)(GLenum mode);
	if(!func)
		func=glGetProcAddressNoNull("glRenderMode");
	return(func(mode));
}

GLAPI GLenum APIENTRY glGetError(void)
{
	static GLenum (*func)();
	if(!func)
		func=glGetProcAddressNoNull("glGetError");
	return(func());
}

GLAPI const GLubyte* APIENTRY glGetString(GLenum name)
{
	static GLubyte* (*func)(GLenum name);
	if(!func)
		func=glGetProcAddressNoNull("glGetString");
	return(func(name));
}

GLAPI void APIENTRY glFinish(void)
{
	static void (*func)();
	if(!func)
		func=glGetProcAddressNoNull("glFinish");
	func();
}

GLAPI void APIENTRY glFlush(void)
{
	static void (*func)();
	if(!func)
		func=glGetProcAddressNoNull("glFlush");
	func();
}

GLAPI void APIENTRY glHint(GLenum target, GLenum mode)
{
	static void (*func)(GLenum target, GLenum mode);
	if(!func)
		func=glGetProcAddressNoNull("glHint");
	func(target, mode);
}

GLAPI void APIENTRY glClearDepth(GLclampd depth)
{
	static void (*func)(GLclampd depth);
	if(!func)
		func=glGetProcAddressNoNull("glClearDepth");
	func(depth);
}

GLAPI void APIENTRY glDepthFunc(GLenum func)
{
	static void (*func)(GLenum func);
	if(!func)
		func=glGetProcAddressNoNull("glDepthFunc");
	func(func);
}

GLAPI void APIENTRY glDepthMask(GLboolean flag)
{
	static void (*func)(GLboolean flag);
	if(!func)
		func=glGetProcAddressNoNull("glDepthMask");
	func(flag);
}

GLAPI void APIENTRY glDepthRange(GLclampd near_val, GLclampd far_val)
{
	static void (*func)(GLclampd near_val, GLclampd far_val);
	if(!func)
		func=glGetProcAddressNoNull("glDepthRange");
	func(near_val, far_val);
}

GLAPI void APIENTRY glClearAccum(
	GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	static void (*func)(
		GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	if(!func)
		func=glGetProcAddressNoNull("glClearAccum");
	func(red, green, blue, alpha);
}

GLAPI void APIENTRY glAccum(GLenum op, GLfloat value)
{
	static void (*func)(
		GLenum op, GLfloat value);
	if(!func)
		func=glGetProcAddressNoNull("glAccum");
	func(op, value);
}

GLAPI void APIENTRY glMatrixMode(GLenum mode)
{
	static void (*func)(GLenum mode);
	if(!func)
		func=glGetProcAddressNoNull("glMatrixMode");
	func(mode);
}

GLAPI void APIENTRY glOrtho(
	GLdouble left, GLdouble right,
	GLdouble bottom, GLdouble top,
	GLdouble near_val, GLdouble far_val)
{
	static void (*func)(
		GLdouble left, GLdouble right,
		GLdouble bottom, GLdouble top,
		GLdouble near_val, GLdouble far_val);
	if(!func)
		func=glGetProcAddressNoNull("glOrtho");
	func(left, right, bottom, top, near_val, far_val);
}

GLAPI void APIENTRY glFrustum(
	GLdouble left, GLdouble right,
	GLdouble bottom, GLdouble top,
	GLdouble near_val, GLdouble far_val)
{
	static void (*func)(
		GLdouble left, GLdouble right,
		GLdouble bottom, GLdouble top,
		GLdouble near_val, GLdouble far_val);
	if(!func)
		func=glGetProcAddressNoNull("glFrustum");
	func(left, right, bottom, top, near_val, far_val);
}

GLAPI void APIENTRY glViewport(
	GLint x, GLint y, GLsizei width, GLsizei height)
{
	static void (*func)(
		GLint x, GLint y, GLsizei width, GLsizei height);
	if(!func)
		func=glGetProcAddressNoNull("glViewport");
	func(x, y, width, height);
}

GLAPI void APIENTRY glPushMatrix(void)
{
	static void (*func)();
	if(!func)
		func=glGetProcAddressNoNull("glPushMatrix");
	func();
}

GLAPI void APIENTRY glPopMatrix(void)
{
	static void (*func)();
	if(!func)
		func=glGetProcAddressNoNull("glPopMatrix");
	func();
}

GLAPI void APIENTRY glLoadIdentity(void)
{
	static void (*func)();
	if(!func)
		func=glGetProcAddressNoNull("glLoadIdentity");
	func();
}

GLAPI void APIENTRY glLoadMatrixd(const GLdouble *m)
{
	static void (*func)(const GLdouble *m);
	if(!func)
		func=glGetProcAddressNoNull("glLoadMatrixd");
	func(m);
}

GLAPI void APIENTRY glLoadMatrixf(const GLfloat *m)
{
	static void (*func)(const GLfloat *m);
	if(!func)
		func=glGetProcAddressNoNull("glLoadMatrixf");
	func(m);
}

GLAPI void APIENTRY glMultMatrixd(const GLdouble *m)
{
	static void (*func)(const GLdouble *m);
	if(!func)
		func=glGetProcAddressNoNull("glMultMatrixd");
	func(m);
}

GLAPI void APIENTRY glMultMatrixf(const GLfloat *m)
{
	static void (*func)(const GLfloat *m);
	if(!func)
		func=glGetProcAddressNoNull("glMultMatrixf");
	func(m);
}

GLAPI void APIENTRY glRotated(
	GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
	static void (*func)(
		GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
	if(!func)
		func=glGetProcAddressNoNull("glRotated");
	func(angle, x, y, z);
}

GLAPI void APIENTRY glRotatef(
	GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	static void (*func)(
		GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
	if(!func)
		func=glGetProcAddressNoNull("glRotatef");
	func(angle, x, y, z);
}

GLAPI void APIENTRY glScaled(GLdouble x, GLdouble y, GLdouble z)
{
	static void (*func)(
		GLdouble x, GLdouble y, GLdouble z);
	if(!func)
		func=glGetProcAddressNoNull("glScaled");
	func(x, y, z);
}

GLAPI void APIENTRY glScalef(GLfloat x, GLfloat y, GLfloat z)
{
	static void (*func)(
		GLfloat x, GLfloat y, GLfloat z);
	if(!func)
		func=glGetProcAddressNoNull("glScalef");
	func(x, y, z);
}

GLAPI void APIENTRY glTranslated(GLdouble x, GLdouble y, GLdouble z)
{
	static void (*func)(
		GLdouble x, GLdouble y, GLdouble z);
	if(!func)
		func=glGetProcAddressNoNull("glTranslated");
	func(x, y, z);
}

GLAPI void APIENTRY glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
	static void (*func)(
		GLfloat x, GLfloat y, GLfloat z);
	if(!func)
		func=glGetProcAddressNoNull("glTranslatef");
	func(x, y, z);
}


/* Display Lists */
GLAPI GLboolean APIENTRY glIsList(GLuint list)
{
	static GLboolean (*func)(GLuint list);
	if(!func)
		func=glGetProcAddressNoNull("glIsList");
	return(func(list));
}

GLAPI void APIENTRY glDeleteLists(GLuint list, GLsizei range)
{
	static void (*func)(GLuint list, GLsizei range);
	if(!func)
		func=glGetProcAddressNoNull("glDeleteLists");
	func(list, range);
}

GLAPI GLuint APIENTRY glGenLists(GLsizei range)
{
	static void (*func)(GLsizei range);
	if(!func)
		func=glGetProcAddressNoNull("glGenLists");
	return(func(range));
}

GLAPI void APIENTRY glNewList(GLuint list, GLenum mode)
{
	static void (*func)(GLuint list, GLenum range);
	if(!func)
		func=glGetProcAddressNoNull("glNewList");
	func(list, mode);
}


GLAPI void APIENTRY glEndList(void)
{
	static void (*func)();
	if(!func)
		func=glGetProcAddressNoNull("glEndList");
	func();
}

GLAPI void APIENTRY glCallList(GLuint list)
{
	static void (*func)(GLuint base);
	if(!func)
		func=glGetProcAddressNoNull("glCallList");
	func(list);
}

GLAPI void APIENTRY glCallLists(
	GLsizei n, GLenum type, const GLvoid *lists)
{
	static void (*func)(
		GLsizei n, GLenum type, const GLvoid *lists);
	if(!func)
		func=glGetProcAddressNoNull("glCallLists");
	func(n, type, lists);
}

GLAPI void APIENTRY glListBase(GLuint base)
{
	static void (*func)(GLuint base);
	if(!func)
		func=glGetProcAddressNoNull("glListBase");
	func(base);
}


/* Drawing Functions */
GLAPI void APIENTRY glBegin(GLenum mode)
{
	static void (*func)(GLenum mode);
	if(!func)
		func=glGetProcAddressNoNull("glBegin");
	func(mode);
}

GLAPI void APIENTRY glEnd(void)
{
	static void (*func)();
	if(!func)
		func=glGetProcAddressNoNull("glEnd");
	func();
}

GLAPI void APIENTRY glVertex2d(GLdouble x, GLdouble y)
{
	static void (*func)(GLdouble x, GLdouble y);
	if(!func)
		func=glGetProcAddressNoNull("glVertex2d");
	func(x, y);
}

GLAPI void APIENTRY glVertex2f(GLfloat x, GLfloat y)
{
	static void (*func)(GLfloat x, GLfloat y);
	if(!func)
		func=glGetProcAddressNoNull("glVertex2f");
	func(x, y);
}

GLAPI void APIENTRY glVertex2i(GLint x, GLint y)
{
	static void (*func)(GLint x, GLint y);
	if(!func)
		func=glGetProcAddressNoNull("glVertex2i");
	func(x, y);
}

GLAPI void APIENTRY glVertex2s(GLshort x, GLshort y)
{
	static void (*func)(GLshort x, GLshort y);
	if(!func)
		func=glGetProcAddressNoNull("glVertex2s");
	func(x, y);
}

GLAPI void APIENTRY glVertex3d(GLdouble x, GLdouble y, GLdouble z)
{
	static void (*func)(GLdouble x, GLdouble y, GLdouble z);
	if(!func)
		func=glGetProcAddressNoNull("glVertex3d");
	func(x, y, z);
}

GLAPI void APIENTRY glVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
	static void (*func)(GLfloat x, GLfloat y, GLfloat z);
	if(!func)
		func=glGetProcAddressNoNull("glVertex3f");
	func(x, y, z);
}

GLAPI void APIENTRY glVertex3i(GLint x, GLint y, GLint z)
{
	static void (*func)(GLint x, GLint y, GLint z);
	if(!func)
		func=glGetProcAddressNoNull("glVertex3i");
	func(x, y, z);
}

GLAPI void APIENTRY glVertex3s(GLshort x, GLshort y, GLshort z)
{
	static void (*func)(GLshort x, GLshort y, GLshort z);
	if(!func)
		func=glGetProcAddressNoNull("glVertex3s");
	func(x, y, z);
}

GLAPI void APIENTRY glVertex4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
	static void (*func)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
	if(!func)
		func=glGetProcAddressNoNull("glVertex4d");
	func(x, y, z, w);
}

GLAPI void APIENTRY glVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	static void (*func)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	if(!func)
		func=glGetProcAddressNoNull("glVertex4f");
	func(x, y, z, w);
}


GLAPI void APIENTRY glVertex4i(GLint x, GLint y, GLint z, GLint w)
{
	static void (*func)(GLint x, GLint y, GLint z, GLint w);
	if(!func)
		func=glGetProcAddressNoNull("glVertex4i");
	func(x, y, z, w);
}

GLAPI void APIENTRY glVertex4s(GLshort x, GLshort y, GLshort z, GLshort w)
{
	static void (*func)(GLshort x, GLshort y, GLshort z, GLshort w);
	if(!func)
		func=glGetProcAddressNoNull("glVertex4s");
	func(x, y, z, w);
}

GLAPI void APIENTRY glVertex2dv(const GLdouble *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glVertex2dv");
	func(v);
}

GLAPI void APIENTRY glVertex2fv(const GLfloat *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glVertex2fv");
	func(v);
}

GLAPI void APIENTRY glVertex2iv(const GLint *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glVertex2iv");
	func(v);
}

GLAPI void APIENTRY glVertex2sv(const GLshort *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glVertex2sv");
	func(v);
}

GLAPI void APIENTRY glVertex3dv(const GLdouble *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glVertex3dv");
	func(v);
}

GLAPI void APIENTRY glVertex3fv(const GLfloat *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glVertex3fv");
	func(v);
}

GLAPI void APIENTRY glVertex3iv(const GLint *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glVertex3iv");
	func(v);
}

GLAPI void APIENTRY glVertex3sv(const GLshort *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glVertex3sv");
	func(v);
}

GLAPI void APIENTRY glVertex4dv(const GLdouble *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glVertex4dv");
	func(v);
}

GLAPI void APIENTRY glVertex4fv(const GLfloat *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glVertex4fv");
	func(v);
}

GLAPI void APIENTRY glVertex4iv(const GLint *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glVertex4iv");
	func(v);
}

GLAPI void APIENTRY glVertex4sv(const GLshort *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glVertex4sv");
	func(v);
}


GLAPI void APIENTRY glNormal3b(GLbyte nx, GLbyte ny, GLbyte nz)
{
	static void (*func)(GLbyte nx, GLbyte ny, GLbyte nz);
	if(!func)
		func=glGetProcAddressNoNull("glNormal3b");
	func(nx, ny, nz);
}

GLAPI void APIENTRY glNormal3d(GLdouble nx, GLdouble ny, GLdouble nz)
{
	static void (*func)(GLdouble nx, GLdouble ny, GLdouble nz);
	if(!func)
		func=glGetProcAddressNoNull("glNormal3d");
	func(nx, ny, nz);
}

GLAPI void APIENTRY glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz)
{
	static void (*func)(GLfloat nx, GLfloat ny, GLfloat nz);
	if(!func)
		func=glGetProcAddressNoNull("glNormal3f");
	func(nx, ny, nz);
}

GLAPI void APIENTRY glNormal3i(GLint nx, GLint ny, GLint nz)
{
	static void (*func)(GLint nx, GLint ny, GLint nz);
	if(!func)
		func=glGetProcAddressNoNull("glNormal3i");
	func(nx, ny, nz);
}

GLAPI void APIENTRY glNormal3s(GLshort nx, GLshort ny, GLshort nz)
{
	static void (*func)(GLshort nx, GLshort ny, GLshort nz);
	if(!func)
		func=glGetProcAddressNoNull("glNormal3s");
	func(nx, ny, nz);
}

GLAPI void APIENTRY glNormal3bv(const GLbyte *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glNormal3bv");
	func(v);
}

GLAPI void APIENTRY glNormal3dv(const GLdouble *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glNormal3dv");
	func(v);
}

GLAPI void APIENTRY glNormal3fv(const GLfloat *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glNormal3fv");
	func(v);
}

GLAPI void APIENTRY glNormal3iv(const GLint *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glNormal3iv");
	func(v);
}

GLAPI void APIENTRY glNormal3sv(const GLshort *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glNormal3sv");
	func(v);
}

GLAPI void APIENTRY glIndexd(GLdouble c)
{
	static void (*func)(GLdouble c);
	if(!func)
		func=glGetProcAddressNoNull("glIndexd");
	func(c);
}

GLAPI void APIENTRY glIndexf(GLfloat c)
{
	static void (*func)(GLdouble c);
	if(!func)
		func=glGetProcAddressNoNull("glIndexf");
	func(c);
}

GLAPI void APIENTRY glIndexi(GLint c)
{
	static void (*func)(GLint c);
	if(!func)
		func=glGetProcAddressNoNull("glIndexi");
	func(c);
}

GLAPI void APIENTRY glIndexs(GLshort c)
{
	static void (*func)(GLint c);
	if(!func)
		func=glGetProcAddressNoNull("glIndexs");
	func(c);
}

GLAPI void APIENTRY glIndexub(GLubyte c)
{
	static void (*func)(GLint c);
	if(!func)
		func=glGetProcAddressNoNull("glIndexub");
	func(c);
}

GLAPI void APIENTRY glIndexdv(const GLdouble *c)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glIndexdv");
	func(c);
}

GLAPI void APIENTRY glIndexfv(const GLfloat *c)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glIndexfv");
	func(c);
}

GLAPI void APIENTRY glIndexiv(const GLint *c)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glIndexiv");
	func(c);
}

GLAPI void APIENTRY glIndexsv(const GLshort *c)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glIndexsv");
	func(c);
}

GLAPI void APIENTRY glIndexubv(const GLubyte *c)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glIndexubv");
	func(c);
}


GLAPI void APIENTRY glColor3b(GLbyte red, GLbyte green, GLbyte blue)
{
	static void (*func)(
		GLint red, GLint green, GLint blue);
	if(!func)
		func=glGetProcAddressNoNull("glColor3b");
	func(red, green, blue);
}

GLAPI void APIENTRY glColor3d(GLdouble red, GLdouble green, GLdouble blue)
{
	static void (*func)(
		GLdouble red, GLdouble green, GLdouble blue);
	if(!func)
		func=glGetProcAddressNoNull("glColor3d");
	func(red, green, blue);
}

GLAPI void APIENTRY glColor3f(GLfloat red, GLfloat green, GLfloat blue)
{
	static void (*func)(
		GLdouble red, GLdouble green, GLdouble blue);
	if(!func)
		func=glGetProcAddressNoNull("glColor3f");
	func(red, green, blue);
}

GLAPI void APIENTRY glColor3i(GLint red, GLint green, GLint blue)
{
	static void (*func)(
		GLint red, GLint green, GLint blue);
	if(!func)
		func=glGetProcAddressNoNull("glColor3i");
	func(red, green, blue);
}

GLAPI void APIENTRY glColor3s(GLshort red, GLshort green, GLshort blue)
{
	static void (*func)(
		GLint red, GLint green, GLint blue);
	if(!func)
		func=glGetProcAddressNoNull("glColor3s");
	func(red, green, blue);
}

GLAPI void APIENTRY glColor3ub(GLubyte red, GLubyte green, GLubyte blue)
{
	static void (*func)(
		GLint red, GLint green, GLint blue);
	if(!func)
		func=glGetProcAddressNoNull("glColor3ub");
	func(red, green, blue);
}

GLAPI void APIENTRY glColor3ui(GLuint red, GLuint green, GLuint blue)
{
	static void (*func)(
		GLint red, GLint green, GLint blue);
	if(!func)
		func=glGetProcAddressNoNull("glColor3ui");
	func(red, green, blue);
}

GLAPI void APIENTRY glColor3us(GLushort red, GLushort green, GLushort blue)
{
	static void (*func)(
		GLint red, GLint green, GLint blue);
	if(!func)
		func=glGetProcAddressNoNull("glColor3us");
	func(red, green, blue);
}

GLAPI void APIENTRY glColor4b(
	GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha)
{
	static void (*func)(GLint red, GLint green, GLint blue, GLint alpha);
	if(!func)
		func=glGetProcAddressNoNull("glColor4b");
	func(red, green, blue, alpha);
}

GLAPI void APIENTRY glColor4d(
	GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha)
{
	static void (*func)(
		GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
	if(!func)
		func=glGetProcAddressNoNull("glColor4d");
	func(red, green, blue, alpha);
}

GLAPI void APIENTRY glColor4f(
	GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	static void (*func)(
		GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
	if(!func)
		func=glGetProcAddressNoNull("glColor4f");
	func(red, green, blue, alpha);
}

GLAPI void APIENTRY glColor4i(
	GLint red, GLint green, GLint blue, GLint alpha)
{
	static void (*func)(GLint red, GLint green, GLint blue, GLint alpha);
	if(!func)
		func=glGetProcAddressNoNull("glColor4i");
	func(red, green, blue, alpha);
}

GLAPI void APIENTRY glColor4s(
	GLshort red, GLshort green, GLshort blue, GLshort alpha)
{
	static void (*func)(GLint red, GLint green, GLint blue, GLint alpha);
	if(!func)
		func=glGetProcAddressNoNull("glColor4s");
	func(red, green, blue, alpha);
}

GLAPI void APIENTRY glColor4ub(
	GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
{
	static void (*func)(GLint red, GLint green, GLint blue, GLint alpha);
	if(!func)
		func=glGetProcAddressNoNull("glColor4ub");
	func(red, green, blue, alpha);
}

GLAPI void APIENTRY glColor4ui(
	GLuint red, GLuint green, GLuint blue, GLuint alpha)
{
	static void (*func)(GLint red, GLint green, GLint blue, GLint alpha);
	if(!func)
		func=glGetProcAddressNoNull("glColor4ui");
	func(red, green, blue, alpha);
}

GLAPI void APIENTRY glColor4us(
	GLushort red, GLushort green, GLushort blue, GLushort alpha)
{
	static void (*func)(GLint red, GLint green, GLint blue, GLint alpha);
	if(!func)
		func=glGetProcAddressNoNull("glColor4us");
	func(red, green, blue, alpha);
}


GLAPI void APIENTRY glColor3bv(const GLbyte *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glColor3bv");
	func(v);
}

GLAPI void APIENTRY glColor3dv(const GLdouble *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glColor3dv");
	func(v);
}

GLAPI void APIENTRY glColor3fv(const GLfloat *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glColor3fv");
	func(v);
}

GLAPI void APIENTRY glColor3iv(const GLint *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glColor3iv");
	func(v);
}

GLAPI void APIENTRY glColor3sv(const GLshort *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glColor3sv");
	func(v);
}

GLAPI void APIENTRY glColor3ubv(const GLubyte *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glColor3ubv");
	func(v);
}

GLAPI void APIENTRY glColor3uiv(const GLuint *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glColor3uiv");
	func(v);
}

GLAPI void APIENTRY glColor3usv(const GLushort *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glColor3usv");
	func(v);
}

GLAPI void APIENTRY glColor4bv(const GLbyte *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glColor4bv");
	func(v);
}

GLAPI void APIENTRY glColor4dv(const GLdouble *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glColor4dv");
	func(v);
}

GLAPI void APIENTRY glColor4fv(const GLfloat *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glColor4fv");
	func(v);
}

GLAPI void APIENTRY glColor4iv(const GLint *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glColor4iv");
	func(v);
}

GLAPI void APIENTRY glColor4sv(const GLshort *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glColor4sv");
	func(v);
}

GLAPI void APIENTRY glColor4ubv(const GLubyte *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glColor4ubv");
	func(v);
}

GLAPI void APIENTRY glColor4uiv(const GLuint *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glColor4uiv");
	func(v);
}

GLAPI void APIENTRY glColor4usv(const GLushort *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glColor4usv");
	func(v);
}

GLAPI void APIENTRY glTexCoord1d(GLdouble s)
{
	static void (*func)(GLdouble s);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord1d");
	func(s);
}

GLAPI void APIENTRY glTexCoord1f(GLfloat s)
{
	static void (*func)(GLdouble s);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord1f");
	func(s);
}

GLAPI void APIENTRY glTexCoord1i(GLint s)
{
	static void (*func)(GLint s);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord1i");
	func(s);
}

GLAPI void APIENTRY glTexCoord1s(GLshort s)
{
	static void (*func)(GLint s);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord1i");
	func(s);
}

GLAPI void APIENTRY glTexCoord2d(GLdouble s, GLdouble t)
{
	static void (*func)(GLdouble s, GLdouble t);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord2d");
	func(s, t);
}

GLAPI void APIENTRY glTexCoord2f(GLfloat s, GLfloat t)
{
	static void (*func)(GLdouble s, GLdouble t);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord2f");
	func(s, t);
}

GLAPI void APIENTRY glTexCoord2i(GLint s, GLint t)
{
	static void (*func)(GLint s, GLint t);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord2i");
	func(s, t);
}

GLAPI void APIENTRY glTexCoord2s(GLshort s, GLshort t)
{
	static void (*func)(GLint s, GLint t);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord2s");
	func(s, t);
}

GLAPI void APIENTRY glTexCoord3d(GLdouble s, GLdouble t, GLdouble r)
{
	static void (*func)(GLdouble s, GLdouble t, GLdouble r);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord3d");
	func(s, t, r);
}

GLAPI void APIENTRY glTexCoord3f(GLfloat s, GLfloat t, GLfloat r)
{
	static void (*func)(GLdouble s, GLdouble t, GLdouble r);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord3f");
	func(s, t, r);
}

GLAPI void APIENTRY glTexCoord3i(GLint s, GLint t, GLint r)
{
	static void (*func)(GLint s, GLint t, GLint r);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord3i");
	func(s, t, r);
}

GLAPI void APIENTRY glTexCoord3s(GLshort s, GLshort t, GLshort r)
{
	static void (*func)(GLint s, GLint t, GLint r);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord3s");
	func(s, t, r);
}

GLAPI void APIENTRY glTexCoord4d(
	GLdouble s, GLdouble t, GLdouble r, GLdouble q)
{
	static void (*func)(GLdouble s, GLdouble t, GLdouble r, GLdouble q);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord4d");
	func(s, t, r, q);
}

GLAPI void APIENTRY glTexCoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
	static void (*func)(GLdouble s, GLdouble t, GLdouble r, GLdouble q);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord4f");
	func(s, t, r, q);
}

GLAPI void APIENTRY glTexCoord4i(GLint s, GLint t, GLint r, GLint q)
{
	static void (*func)(GLint s, GLint t, GLint r, GLint q);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord4i");
	func(s, t, r, q);
}

GLAPI void APIENTRY glTexCoord4s(GLshort s, GLshort t, GLshort r, GLshort q)
{
	static void (*func)(GLint s, GLint t, GLint r, GLint q);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord4s");
	func(s, t, r, q);
}

GLAPI void APIENTRY glTexCoord1dv(const GLdouble *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord1dv");
	func(v);
}

GLAPI void APIENTRY glTexCoord1fv(const GLfloat *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord1fv");
	func(v);
}

GLAPI void APIENTRY glTexCoord1iv(const GLint *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord1iv");
	func(v);
}

GLAPI void APIENTRY glTexCoord1sv(const GLshort *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord1sv");
	func(v);
}

GLAPI void APIENTRY glTexCoord2dv(const GLdouble *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord2dv");
	func(v);
}

GLAPI void APIENTRY glTexCoord2fv(const GLfloat *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord2fv");
	func(v);
}

GLAPI void APIENTRY glTexCoord2iv(const GLint *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord2iv");
	func(v);
}

GLAPI void APIENTRY glTexCoord2sv(const GLshort *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord2sv");
	func(v);
}

GLAPI void APIENTRY glTexCoord3dv(const GLdouble *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord3dv");
	func(v);
}

GLAPI void APIENTRY glTexCoord3fv(const GLfloat *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord3fv");
	func(v);
}

GLAPI void APIENTRY glTexCoord3iv(const GLint *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord3iv");
	func(v);
}

GLAPI void APIENTRY glTexCoord3sv(const GLshort *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord3sv");
	func(v);
}

GLAPI void APIENTRY glTexCoord4dv(const GLdouble *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord4dv");
	func(v);
}

GLAPI void APIENTRY glTexCoord4fv(const GLfloat *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord4fv");
	func(v);
}

GLAPI void APIENTRY glTexCoord4iv(const GLint *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord4iv");
	func(v);
}

GLAPI void APIENTRY glTexCoord4sv(const GLshort *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoord4sv");
	func(v);
}

GLAPI void APIENTRY glRasterPos2d(GLdouble x, GLdouble y)
{
	static void (*func)(GLdouble x, GLdouble y);
	if(!func)
		func=glGetProcAddressNoNull("glRasterPos2d");
	func(x, y);
}

GLAPI void APIENTRY glRasterPos2f(GLfloat x, GLfloat y)
{
	static void (*func)(GLfloat x, GLfloat y);
	if(!func)
		func=glGetProcAddressNoNull("glRasterPos2f");
	func(x, y);
}

GLAPI void APIENTRY glRasterPos2i(GLint x, GLint y)
{
	static void (*func)(GLint x, GLint y);
	if(!func)
		func=glGetProcAddressNoNull("glRasterPos2i");
	func(x, y);
}

GLAPI void APIENTRY glRasterPos2s(GLshort x, GLshort y)
{
	static void (*func)(GLdouble x, GLdouble y);
	if(!func)
		func=glGetProcAddressNoNull("glRasterPos2s");
	func(x, y);
}

GLAPI void APIENTRY glRasterPos3d(GLdouble x, GLdouble y, GLdouble z)
{
	static void (*func)(GLdouble x, GLdouble y, GLdouble z);
	if(!func)
		func=glGetProcAddressNoNull("glRasterPos3d");
	func(x, y, z);
}

GLAPI void APIENTRY glRasterPos3f(GLfloat x, GLfloat y, GLfloat z)
{
	static void (*func)(GLfloat x, GLfloat y, GLfloat z);
	if(!func)
		func=glGetProcAddressNoNull("glRasterPos3f");
	func(x, y, z);
}

GLAPI void APIENTRY glRasterPos3i(GLint x, GLint y, GLint z)
{
	static void (*func)(GLint x, GLint y, GLint z);
	if(!func)
		func=glGetProcAddressNoNull("glRasterPos3i");
	func(x, y, z);
}

GLAPI void APIENTRY glRasterPos3s(GLshort x, GLshort y, GLshort z)
{
	static void (*func)(GLshort x, GLshort y, GLshort z);
	if(!func)
		func=glGetProcAddressNoNull("glRasterPos3s");
	func(x, y, z);
}

GLAPI void APIENTRY glRasterPos4d(
	GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
	static void (*func)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
	if(!func)
		func=glGetProcAddressNoNull("glRasterPos4d");
	func(x, y, z, w);
}

GLAPI void APIENTRY glRasterPos4f(
	GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	static void (*func)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	if(!func)
		func=glGetProcAddressNoNull("glRasterPos4f");
	func(x, y, z, w);
}

GLAPI void APIENTRY glRasterPos4i(
	GLint x, GLint y, GLint z, GLint w)
{
	static void (*func)(GLint x, GLint y, GLint z, GLint w);
	if(!func)
		func=glGetProcAddressNoNull("glRasterPos4i");
	func(x, y, z, w);
}

GLAPI void APIENTRY glRasterPos4s(
	GLshort x, GLshort y, GLshort z, GLshort w)
{
	static void (*func)(GLshort x, GLshort y, GLshort z, GLshort w);
	if(!func)
		func=glGetProcAddressNoNull("glRasterPos4s");
	func(x, y, z, w);
}

GLAPI void APIENTRY glRasterPos2dv(const GLdouble *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glRasterPos2dv");
	func(v);
}

GLAPI void APIENTRY glRasterPos2fv(const GLfloat *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glRasterPos2fv");
	func(v);
}

GLAPI void APIENTRY glRasterPos2iv(const GLint *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glRasterPos2iv");
	func(v);
}

GLAPI void APIENTRY glRasterPos2sv(const GLshort *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glRasterPos2sv");
	func(v);
}

GLAPI void APIENTRY glRasterPos3dv(const GLdouble *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glRasterPos3dv");
	func(v);
}

GLAPI void APIENTRY glRasterPos3fv(const GLfloat *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glRasterPos3fv");
	func(v);
}

GLAPI void APIENTRY glRasterPos3iv(const GLint *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glRasterPos3iv");
	func(v);
}

GLAPI void APIENTRY glRasterPos3sv(const GLshort *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glRasterPos3sv");
	func(v);
}

GLAPI void APIENTRY glRasterPos4dv(const GLdouble *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glRasterPos4dv");
	func(v);
}

GLAPI void APIENTRY glRasterPos4fv(const GLfloat *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glRasterPos4fv");
	func(v);
}

GLAPI void APIENTRY glRasterPos4iv(const GLint *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glRasterPos4iv");
	func(v);
}

GLAPI void APIENTRY glRasterPos4sv(const GLshort *v)
{
	static void (*func)(void *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glRasterPos4sv");
	func(v);
}


GLAPI void APIENTRY glRectd(
	GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)
{
	static void (*func)(
		GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
	if(!func)
		func=glGetProcAddressNoNull("glRectd");
	func(x1, y1, x2, y2);
}

GLAPI void APIENTRY glRectf(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
	static void (*func)(
		GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
	if(!func)
		func=glGetProcAddressNoNull("glRectf");
	func(x1, y1, x2, y2);
}

GLAPI void APIENTRY glRecti(GLint x1, GLint y1, GLint x2, GLint y2)
{
	static void (*func)(
		GLint x1, GLint y1, GLint x2, GLint y2);
	if(!func)
		func=glGetProcAddressNoNull("glRecti");
	func(x1, y1, x2, y2);
}

GLAPI void APIENTRY glRects(GLshort x1, GLshort y1, GLshort x2, GLshort y2)
{
	static void (*func)(
		GLint x1, GLint y1, GLint x2, GLint y2);
	if(!func)
		func=glGetProcAddressNoNull("glRects");
	func(x1, y1, x2, y2);
}

GLAPI void APIENTRY glRectdv(const GLdouble *v1, const GLdouble *v2)
{
	static void (*func)(void *ptr1, void *ptr2);
	if(!func)
		func=glGetProcAddressNoNull("glRectdv");
	func(v1, v2);
}

GLAPI void APIENTRY glRectfv(const GLfloat *v1, const GLfloat *v2)
{
	static void (*func)(void *ptr1, void *ptr2);
	if(!func)
		func=glGetProcAddressNoNull("glRectfv");
	func(v1, v2);
}

GLAPI void APIENTRY glRectiv(const GLint *v1, const GLint *v2)
{
	static void (*func)(void *ptr1, void *ptr2);
	if(!func)
		func=glGetProcAddressNoNull("glRectiv");
	func(v1, v2);
}

GLAPI void APIENTRY glRectsv(const GLshort *v1, const GLshort *v2)
{
	static void (*func)(void *ptr1, void *ptr2);
	if(!func)
		func=glGetProcAddressNoNull("glRectsv");
	func(v1, v2);
}

/* Lighting */
GLAPI void APIENTRY glShadeModel(GLenum mode)
{
	static void (*func)(GLenum mode);
	if(!func)
		func=glGetProcAddressNoNull("glShadeModel");
	func(mode);
}

GLAPI void APIENTRY glLightf(GLenum light, GLenum pname, GLfloat param)
{
	static void (*func)(GLenum light, GLenum pname, GLfloat param);
	if(!func)
		func=glGetProcAddressNoNull("glLightf");
	func(light, pname, param);
}

GLAPI void APIENTRY glLighti(GLenum light, GLenum pname, GLint param)
{
	static void (*func)(GLenum light, GLenum pname, GLint param);
	if(!func)
		func=glGetProcAddressNoNull("glLighti");
	func(light, pname, param);
}

GLAPI void APIENTRY glLightfv(
	GLenum light, GLenum pname, const GLfloat *params)
{
	static void (*func)(GLenum light, GLenum pname, void *param);
	if(!func)
		func=glGetProcAddressNoNull("glLightfv");
	func(light, pname, params);
}

GLAPI void APIENTRY glLightiv(
	GLenum light, GLenum pname, const GLint *params)
{
	static void (*func)(GLenum light, GLenum pname, void *param);
	if(!func)
		func=glGetProcAddressNoNull("glLightiv");
	func(light, pname, params);
}

GLAPI void APIENTRY glGetLightfv(GLenum light, GLenum pname, GLfloat *params)
{
	static void (*func)(GLenum light, GLenum pname, void *param);
	if(!func)
		func=glGetProcAddressNoNull("glGetLightfv");
	func(light, pname, params);
}

GLAPI void APIENTRY glGetLightiv(GLenum light, GLenum pname, GLint *params)
{
	static void (*func)(GLenum light, GLenum pname, void *param);
	if(!func)
		func=glGetProcAddressNoNull("glGetLightiv");
	func(light, pname, params);
}

GLAPI void APIENTRY glLightModelf(GLenum pname, GLfloat param)
{
	static void (*func)(GLenum pname, GLfloat param);
	if(!func)
		func=glGetProcAddressNoNull("glLightModelf");
	func(pname, param);
}

GLAPI void APIENTRY glLightModeli(GLenum pname, GLint param)
{
	static void (*func)(GLenum pname, GLint param);
	if(!func)
		func=glGetProcAddressNoNull("glLightModeli");
	func(pname, param);
}

GLAPI void APIENTRY glLightModelfv(GLenum pname, const GLfloat *params)
{
	static void (*func)(GLenum pname, void *param);
	if(!func)
		func=glGetProcAddressNoNull("glLightModelfv");
	func(pname, params);
}

GLAPI void APIENTRY glLightModeliv(GLenum pname, const GLint *params)
{
	static void (*func)(GLenum pname, void *param);
	if(!func)
		func=glGetProcAddressNoNull("glLightModeliv");
	func(pname, params);
}

GLAPI void APIENTRY glMaterialf(GLenum face, GLenum pname, GLfloat param)
{
	static void (*func)(GLenum light, GLenum pname, GLfloat param);
	if(!func)
		func=glGetProcAddressNoNull("glMaterialf");
	func(face, pname, param);
}

GLAPI void APIENTRY glMateriali(GLenum face, GLenum pname, GLint param)
{
	static void (*func)(GLenum light, GLenum pname, GLint param);
	if(!func)
		func=glGetProcAddressNoNull("glMateriali");
	func(face, pname, param);
}

GLAPI void APIENTRY glMaterialfv(
	GLenum face, GLenum pname, const GLfloat *params)
{
	static void (*func)(GLenum light, GLenum pname, void *param);
	if(!func)
		func=glGetProcAddressNoNull("glMaterialfv");
	func(face, pname, params);
}

GLAPI void APIENTRY glMaterialiv(
	GLenum face, GLenum pname, const GLint *params)
{
	static void (*func)(GLenum light, GLenum pname, void *param);
	if(!func)
		func=glGetProcAddressNoNull("glMaterialiv");
	func(face, pname, params);
}

GLAPI void APIENTRY glGetMaterialfv(
	GLenum face, GLenum pname, GLfloat *params)
{
	static void (*func)(GLenum light, GLenum pname, void *param);
	if(!func)
		func=glGetProcAddressNoNull("glGetMaterialfv");
	func(face, pname, params);
}

GLAPI void APIENTRY glGetMaterialiv(GLenum face, GLenum pname, GLint *params)
{
	static void (*func)(GLenum light, GLenum pname, void *param);
	if(!func)
		func=glGetProcAddressNoNull("glGetMaterialiv");
	func(face, pname, params);
}

GLAPI void APIENTRY glColorMaterial(GLenum face, GLenum mode)
{
	static void (*func)(GLenum light, GLenum mode);
	if(!func)
		func=glGetProcAddressNoNull("glColorMaterial");
	func(face, mode);
}

/* Raster functions */
GLAPI void APIENTRY glPixelZoom(GLfloat xfactor, GLfloat yfactor)
{
	static void (*func)(GLfloat xfactor, GLfloat yfactor);
	if(!func)
		func=glGetProcAddressNoNull("glPixelZoom");
	func(xfactor, yfactor);
}

GLAPI void APIENTRY glPixelStoref(GLenum pname, GLfloat param)
{
	static void (*func)(GLenum pname, GLfloat param);
	if(!func)
		func=glGetProcAddressNoNull("glPixelStoref");
	func(pname, param);
}

GLAPI void APIENTRY glPixelStorei(GLenum pname, GLint param)
{
	static void (*func)(GLenum pname, GLint param);
	if(!func)
		func=glGetProcAddressNoNull("glPixelStorei");
	func(pname, param);
}

GLAPI void APIENTRY glPixelTransferf(GLenum pname, GLfloat param)
{
	static void (*func)(GLenum pname, GLfloat param);
	if(!func)
		func=glGetProcAddressNoNull("glPixelTransferf");
	func(pname, param);
}

GLAPI void APIENTRY glPixelTransferi(GLenum pname, GLint param)
{
	static void (*func)(GLenum pname, GLint param);
	if(!func)
		func=glGetProcAddressNoNull("glPixelTransferi");
	func(pname, param);
}

GLAPI void APIENTRY glPixelMapfv(
	GLenum map, GLint mapsize, const GLfloat *values)
{
	static void (*func)(GLenum map, GLint mapsize, void *values);
	if(!func)
		func=glGetProcAddressNoNull("glPixelMapfv");
	func(map, mapsize, values);
}

GLAPI void APIENTRY glPixelMapuiv(
	GLenum map, GLint mapsize, const GLuint *values)
{
	static void (*func)(GLenum map, GLint mapsize, void *values);
	if(!func)
		func=glGetProcAddressNoNull("glPixelMapuiv");
	func(map, mapsize, values);
}

GLAPI void APIENTRY glPixelMapusv(
	GLenum map, GLint mapsize, const GLushort *values)
{
	static void (*func)(GLenum map, GLint mapsize, void *values);
	if(!func)
		func=glGetProcAddressNoNull("glPixelMapusv");
	func(map, mapsize, values);
}

GLAPI void APIENTRY glGetPixelMapfv(GLenum map, GLfloat *values)
{
	static void (*func)(GLenum pname, void *param);
	if(!func)
		func=glGetProcAddressNoNull("glGetPixelMapfv");
	func(map, values);
}

GLAPI void APIENTRY glGetPixelMapuiv(GLenum map, GLuint *values)
{
	static void (*func)(GLenum pname, void *param);
	if(!func)
		func=glGetProcAddressNoNull("glGetPixelMapuiv");
	func(map, values);
}

GLAPI void APIENTRY glGetPixelMapusv(GLenum map, GLushort *values)
{
	static void (*func)(GLenum pname, void *param);
	if(!func)
		func=glGetProcAddressNoNull("glGetPixelMapusv");
	func(map, values);
}

GLAPI void APIENTRY glBitmap(
	GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig,
	GLfloat xmove, GLfloat ymove, const GLubyte *bitmap)
{
	static void (*func)(
		GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig,
	GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
	if(!func)
		func=glGetProcAddressNoNull("glBitmap");
	func(
		width, height,
		xorig, yorig,
		xmove, ymove,
		bitmap);
}

GLAPI void APIENTRY glReadPixels(
	GLint x, GLint y, GLsizei width, GLsizei height,
	GLenum format, GLenum type, GLvoid *pixels)
{
	static void (*func)(
		GLint x, GLint y, GLsizei width, GLsizei height,
		GLenum format, GLenum type, GLvoid *pixels);
	if(!func)
		func=glGetProcAddressNoNull("glReadPixels");
	func(x, y, width, height, format, type, pixels);
}

GLAPI void APIENTRY glDrawPixels(
	GLsizei width, GLsizei height, GLenum format, GLenum type,
	const GLvoid *pixels)
{
	static void (*func)(
		GLsizei width, GLsizei height, GLenum format, GLenum type,
		const GLvoid *pixels);
	if(!func)
		func=glGetProcAddressNoNull("glDrawPixels");
	func(x, y, width, height, format, type, pixels);
}

GLAPI void APIENTRY glCopyPixels(
	GLint x, GLint y, GLsizei width, GLsizei height, GLenum type)
{
	static void (*func)(
		GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
	if(!func)
		func=glGetProcAddressNoNull("glCopyPixels");
	func(x, y, width, height, type);
}

/* Stenciling */
GLAPI void APIENTRY glStencilFunc(GLenum func, GLint ref, GLuint mask)
{
	static void (*func)(GLenum func, GLint ref, GLuint mask);
	if(!func)
		func=glGetProcAddressNoNull("glStencilFunc");
	func(func, ref, mask);
}

GLAPI void APIENTRY glStencilMask(GLuint mask)
{
	static void (*func)(GLuint mask);
	if(!func)
		func=glGetProcAddressNoNull("glStencilMask");
	func(mask);
}

GLAPI void APIENTRY glStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
	static void (*func)(GLenum fail, GLenum zfail, GLenum zpass);
	if(!func)
		func=glGetProcAddressNoNull("glStencilOp");
	func(fail, zfail, zpass);
}

GLAPI void APIENTRY glClearStencil(GLint s)
{
	static void (*func)(GLint mask);
	if(!func)
		func=glGetProcAddressNoNull("glClearStencil");
	func(s);
}


/* Texture mapping */
GLAPI void APIENTRY glTexGend(GLenum coord, GLenum pname, GLdouble param)
{
	static void (*func)(GLenum coord, GLenum pname, GLdouble param);
	if(!func)
		func=glGetProcAddressNoNull("glTexGend");
	func(coord, pname, param);
}

GLAPI void APIENTRY glTexGenf(GLenum coord, GLenum pname, GLfloat param)
{
	static void (*func)(GLenum coord, GLenum pname, GLdouble param);
	if(!func)
		func=glGetProcAddressNoNull("glTexGenf");
	func(coord, pname, param);
}

GLAPI void APIENTRY glTexGeni(GLenum coord, GLenum pname, GLint param)
{
	static void (*func)(GLenum coord, GLenum pname, GLint param);
	if(!func)
		func=glGetProcAddressNoNull("glTexGeni");
	func(coord, pname, param);
}

GLAPI void APIENTRY glTexGendv(
	GLenum coord, GLenum pname, const GLdouble *params)
{
	static void (*func)(GLenum coord, GLenum pname, void *params);
	if(!func)
		func=glGetProcAddressNoNull("glTexGendv");
	func(coord, pname, params);
}

GLAPI void APIENTRY glTexGenfv(
	GLenum coord, GLenum pname, const GLfloat *params)
{
	static void (*func)(GLenum coord, GLenum pname, void *params);
	if(!func)
		func=glGetProcAddressNoNull("glTexGenfv");
	func(coord, pname, params);
}

GLAPI void APIENTRY glTexGeniv(
	GLenum coord, GLenum pname, const GLint *params)
{
	static void (*func)(GLenum coord, GLenum pname, void *params);
	if(!func)
		func=glGetProcAddressNoNull("glTexGeniv");
	func(coord, pname, params);
}

GLAPI void APIENTRY glGetTexGendv(
	GLenum coord, GLenum pname, GLdouble *params)
{
	static void (*func)(GLenum coord, GLenum pname, void *params);
	if(!func)
		func=glGetProcAddressNoNull("glGetTexGendv");
	func(coord, pname, params);
}

GLAPI void APIENTRY glGetTexGenfv(GLenum coord, GLenum pname, GLfloat *params)
{
	static void (*func)(GLenum coord, GLenum pname, void *params);
	if(!func)
		func=glGetProcAddressNoNull("glGetTexGenfv");
	func(coord, pname, params);
}

GLAPI void APIENTRY glGetTexGeniv(GLenum coord, GLenum pname, GLint *params)
{
	static void (*func)(GLenum coord, GLenum pname, void *params);
	if(!func)
		func=glGetProcAddressNoNull("glGetTexGeniv");
	func(coord, pname, params);
}

GLAPI void APIENTRY glTexEnvf(GLenum target, GLenum pname, GLfloat param)
{
	static void (*func)(GLenum coord, GLenum pname, GLfloat params);
	if(!func)
		func=glGetProcAddressNoNull("glTexEnvf");
	func(target, pname, param);
}

GLAPI void APIENTRY glTexEnvi(GLenum target, GLenum pname, GLint param)
{
	static void (*func)(GLenum coord, GLenum pname, GLint params);
	if(!func)
		func=glGetProcAddressNoNull("glTexEnvi");
	func(target, pname, param);
}

GLAPI void APIENTRY glTexEnvfv(
	GLenum target, GLenum pname, const GLfloat *params)
{
	static void (*func)(GLenum coord, GLenum pname, void *params);
	if(!func)
		func=glGetProcAddressNoNull("glTexEnvfv");
	func(target, pname, params);
}

GLAPI void APIENTRY glTexEnviv(
	GLenum target, GLenum pname, const GLint *params)
{
	static void (*func)(GLenum coord, GLenum pname, void *params);
	if(!func)
		func=glGetProcAddressNoNull("glTexEnviv");
	func(target, pname, params);
}

GLAPI void APIENTRY glGetTexEnvfv(
	GLenum target, GLenum pname, GLfloat *params)
{
	static void (*func)(GLenum coord, GLenum pname, void *params);
	if(!func)
		func=glGetProcAddressNoNull("glGetTexEnvfv");
	func(target, pname, params);
}

GLAPI void APIENTRY glGetTexEnviv(GLenum target, GLenum pname, GLint *params)
{
	static void (*func)(GLenum coord, GLenum pname, void *params);
	if(!func)
		func=glGetProcAddressNoNull("glGetTexEnviv");
	func(target, pname, params);
}

GLAPI void APIENTRY glTexParameterf(
	GLenum target, GLenum pname, GLfloat param)
{
	static void (*func)(GLenum coord, GLenum pname, GLfloat params);
	if(!func)
		func=glGetProcAddressNoNull("glTexParameterf");
	func(target, pname, param);
}

GLAPI void APIENTRY glTexParameteri(GLenum target, GLenum pname, GLint param)
{
	static void (*func)(GLenum coord, GLenum pname, GLint params);
	if(!func)
		func=glGetProcAddressNoNull("glTexParameteri");
	func(target, pname, param);
}

GLAPI void APIENTRY glTexParameterfv(
	GLenum target, GLenum pname, const GLfloat *params)
{
	static void (*func)(GLenum coord, GLenum pname, void *params);
	if(!func)
		func=glGetProcAddressNoNull("glTexParameterfv");
	func(target, pname, params);
}

GLAPI void APIENTRY glTexParameteriv(
	GLenum target, GLenum pname, const GLint *params)
{
	static void (*func)(GLenum coord, GLenum pname, void *params);
	if(!func)
		func=glGetProcAddressNoNull("glTexParameteriv");
	func(target, pname, params);
}

GLAPI void APIENTRY glGetTexParameterfv(
	GLenum target, GLenum pname, GLfloat *params)
{
	static void (*func)(GLenum coord, GLenum pname, void *params);
	if(!func)
		func=glGetProcAddressNoNull("glGetTexParameterfv");
	func(target, pname, params);
}

GLAPI void APIENTRY glGetTexParameteriv(
	GLenum target, GLenum pname, GLint *params)
{
	static void (*func)(GLenum coord, GLenum pname, void *params);
	if(!func)
		func=glGetProcAddressNoNull("glGetTexParameteriv");
	func(target, pname, params);
}

GLAPI void APIENTRY glGetTexLevelParameterfv(
	GLenum target, GLint level, GLenum pname, GLfloat *params)
{
	static void (*func)(
		GLenum coord, GLint level, GLenum pname, void *params);
	if(!func)
		func=glGetProcAddressNoNull("glGetTexLevelParameterfv");
	func(target, level, pname, params);
}

GLAPI void APIENTRY glGetTexLevelParameteriv(
	GLenum target, GLint level, GLenum pname, GLint *params)
{
	static void (*func)(
		GLenum coord, GLint level, GLenum pname, void *params);
	if(!func)
		func=glGetProcAddressNoNull("glGetTexLevelParameteriv");
	func(target, level, pname, params);
}


GLAPI void APIENTRY glTexImage1D(
	GLenum target, GLint level, GLint internalFormat,
	GLsizei width, GLint border, GLenum format, GLenum type,
	const GLvoid *pixels)
{
	static void (*func)(
		GLenum target, GLint level, GLint internalFormat,
		GLsizei width, GLint border, GLenum format, GLenum type,
		const GLvoid *pixels);
	if(!func)
		func=glGetProcAddressNoNull("glTexImage1D");
	func(target, level, internalformat, width, border, format, type, pixels);
}

GLAPI void APIENTRY glTexImage2D(
	GLenum target, GLint level, GLint internalFormat,
	GLsizei width, GLsizei height, GLint border,
	GLenum format, GLenum type, const GLvoid *pixels)
{
	static void (*func)(
		GLenum target, GLint level, GLint internalFormat,
		GLsizei width, GLsizei height, GLint border,
		GLenum format, GLenum type, const GLvoid *pixels);
	if(!func)
		func=glGetProcAddressNoNull("glTexImage2D");
	func(target, level, internalformat,
		width, height, border, format, type, pixels);
}

GLAPI void APIENTRY glGetTexImage(
	GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels)
{
	static void (*func)(
		GLenum target, GLint level, GLenum format, GLenum type,
		GLvoid *pixels);
	if(!func)
		func=glGetProcAddressNoNull("glGetTexImage");
	func(target, level, format, type, pixels);
}

/* Evaluators */
GLAPI void APIENTRY glMap1d(
	GLenum target, GLdouble u1, GLdouble u2,
	GLint stride, GLint order, const GLdouble *points)
{
	static void (*func)(
		GLenum target, GLdouble u1, GLdouble u2,
		GLint stride, GLint order, void *points);
	if(!func)
		func=glGetProcAddressNoNull("glMap1d");
	func(target, u1, u2, stride, order, points);
}

GLAPI void APIENTRY glMap1f(
	GLenum target, GLfloat u1, GLfloat u2,
	GLint stride, GLint order, const GLfloat *points)
{
	static void (*func)(
		GLenum target, GLdouble u1, GLdouble u2,
		GLint stride, GLint order, void *points);
	if(!func)
		func=glGetProcAddressNoNull("glMap1f");
	func(target, u1, u2, stride, order, points);
}

GLAPI void APIENTRY glMap2d(
	GLenum target, GLdouble u1, GLdouble u2,
	GLint ustride, GLint uorder,
	GLdouble v1, GLdouble v2,
	GLint vstride, GLint vorder,
	const GLdouble *points)
{
	static void (*func)(
		GLenum target,
		GLdouble u1, GLdouble u2, GLint ustride, GLint uorder,
		GLdouble v1, GLdouble v2, GLint vstride, GLint vorder,
		void *points);
	if(!func)
		func=glGetProcAddressNoNull("glMap2d");
	func(target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);
}

GLAPI void APIENTRY glMap2f(
	GLenum target,
	GLfloat u1, GLfloat u2, GLint ustride, GLint uorder,
	GLfloat v1, GLfloat v2, GLint vstride, GLint vorder,
	const GLfloat *points)
{
	static void (*func)(
		GLenum target,
		GLdouble u1, GLdouble u2, GLint ustride, GLint uorder,
		GLdouble v1, GLdouble v2, GLint vstride, GLint vorder,
		void *points);
	if(!func)
		func=glGetProcAddressNoNull("glMap2f");
	func(target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);
}

GLAPI void APIENTRY glGetMapdv(GLenum target, GLenum query, GLdouble *v)
{
	static void (*func)(
		GLenum target, GLenum query, GLvoid *v);
	if(!func)
		func=glGetProcAddressNoNull("glGetMapdv");
	func(target, query, v);
}

GLAPI void APIENTRY glGetMapfv(GLenum target, GLenum query, GLfloat *v)
{
	static void (*func)(
		GLenum target, GLenum query, GLvoid *v);
	if(!func)
		func=glGetProcAddressNoNull("glGetMapfv");
	func(target, query, v);
}

GLAPI void APIENTRY glGetMapiv(GLenum target, GLenum query, GLint *v)
{
	static void (*func)(
		GLenum target, GLenum query, GLvoid *v);
	if(!func)
		func=glGetProcAddressNoNull("glGetMapiv");
	func(target, query, v);
}

GLAPI void APIENTRY glEvalCoord1d(GLdouble u);
GLAPI void APIENTRY glEvalCoord1f(GLfloat u);

GLAPI void APIENTRY glEvalCoord1dv(const GLdouble *u)
{
	static void (*func)(GLvoid *v);
	if(!func)
		func=glGetProcAddressNoNull("glEvalCoord1dv");
	func(u);
}

GLAPI void APIENTRY glEvalCoord1fv(const GLfloat *u)
{
	static void (*func)(GLvoid *v);
	if(!func)
		func=glGetProcAddressNoNull("glEvalCoord1fv");
	func(u);
}

GLAPI void APIENTRY glEvalCoord2d(GLdouble u, GLdouble v)
{
	static void (*func)(GLdouble u, GLdouble v);
	if(!func)
		func=glGetProcAddressNoNull("glEvalCoord2d");
	func(u, v);
}

GLAPI void APIENTRY glEvalCoord2f(GLfloat u, GLfloat v)
{
	static void (*func)(GLdouble u, GLdouble v);
	if(!func)
		func=glGetProcAddressNoNull("glEvalCoord2f");
	func(u, v);
}

GLAPI void APIENTRY glEvalCoord2dv(const GLdouble *u)
{
	static void (*func)(GLvoid *v);
	if(!func)
		func=glGetProcAddressNoNull("glEvalCoord2dv");
	func(u);
}

GLAPI void APIENTRY glEvalCoord2fv(const GLfloat *u)
{
	static void (*func)(GLvoid *v);
	if(!func)
		func=glGetProcAddressNoNull("glEvalCoord2fv");
	func(u);
}

GLAPI void APIENTRY glMapGrid1d(GLint un, GLdouble u1, GLdouble u2)
{
	static void (*func)(
		GLint un, GLdouble u1, GLdouble u2);
	if(!func)
		func=glGetProcAddressNoNull("glMapGrid1d");
	func(un, u1, u2);
}

GLAPI void APIENTRY glMapGrid1f(GLint un, GLfloat u1, GLfloat u2)
{
	static void (*func)(
		GLint un, GLdouble u1, GLdouble u2);
	if(!func)
		func=glGetProcAddressNoNull("glMapGrid1f");
	func(un, u1, u2);
}

GLAPI void APIENTRY glMapGrid2d(
	GLint un, GLdouble u1, GLdouble u2,
	GLint vn, GLdouble v1, GLdouble v2)
{
	static void (*func)(
		GLint un, GLdouble u1, GLdouble u2,
		GLint vn, GLdouble v1, GLdouble v2);
	if(!func)
		func=glGetProcAddressNoNull("glMapGrid2d");
	func(un, u1, u2, vn, v1, v2);
}

GLAPI void APIENTRY glMapGrid2f(
	GLint un, GLfloat u1, GLfloat u2,
	GLint vn, GLfloat v1, GLfloat v2)
{
	static void (*func)(
		GLint un, GLfloat u1, GLfloat u2,
		GLint vn, GLfloat v1, GLfloat v2);
	if(!func)
		func=glGetProcAddressNoNull("glMapGrid2f");
	func(un, u1, u2, vn, v1, v2);
}

GLAPI void APIENTRY glEvalPoint1(GLint i)
{
	static void (*func)(GLint i);
	if(!func)
		func=glGetProcAddressNoNull("glEvalPoint1");
	func(i);
}

GLAPI void APIENTRY glEvalPoint2(GLint i, GLint j)
{
	static void (*func)(GLint i, GLint j);
	if(!func)
		func=glGetProcAddressNoNull("glEvalPoint2");
	func(i, j);
}

GLAPI void APIENTRY glEvalMesh1(GLenum mode, GLint i1, GLint i2)
{
	static void (*func)(
		GLenum mode, GLint i1, GLint i2);
	if(!func)
		func=glGetProcAddressNoNull("glEvalMesh1");
	func(mode, i1, i2);
}

GLAPI void APIENTRY glEvalMesh2(
	GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2)
{
	static void (*func)(
		GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
	if(!func)
		func=glGetProcAddressNoNull("glEvalMesh2");
	func(mode, i1, i2, j1, j2);
}

/* Fog */
GLAPI void APIENTRY glFogf(GLenum pname, GLfloat param)
{
	static void (*func)(
		GLenum pname, GLfloat params);
	if(!func)
		func=glGetProcAddressNoNull("glFogf");
	func(pname, param);
}

GLAPI void APIENTRY glFogi(GLenum pname, GLint param)
{
	static void (*func)(
		GLenum pname, GLint params);
	if(!func)
		func=glGetProcAddressNoNull("glFogi");
	func(pname, param);
}

GLAPI void APIENTRY glFogfv(GLenum pname, const GLfloat *params)
{
	static void (*func)(
		GLenum pname, void *params);
	if(!func)
		func=glGetProcAddressNoNull("glFogfv");
	func(pname, params);
}

GLAPI void APIENTRY glFogiv(GLenum pname, const GLint *params)
{
	static void (*func)(
		GLenum pname, void *params);
	if(!func)
		func=glGetProcAddressNoNull("glFogiv");
	func(pname, params);
}

/* Selection and Feedback */
GLAPI void APIENTRY glFeedbackBuffer(
	GLsizei size, GLenum type, GLfloat *buffer)
{
	static void (*func)(GLsizei size,  GLenum type, GLfloat *buffer);
	if(!func)
		func=glGetProcAddressNoNull("glFeedbackBuffer");
	func(size, type, buffer);
}

GLAPI void APIENTRY glPassThrough(GLfloat token)
{
	static void (*func)(GLfloat token);
	if(!func)
		func=glGetProcAddressNoNull("glPassThrough");
	func(token);
}

GLAPI void APIENTRY glSelectBuffer(GLsizei size, GLuint *buffer)
{
	static void (*func)(GLsizei size, GLuint *buffer);
	if(!func)
		func=glGetProcAddressNoNull("glSelectBuffer");
	func(size, buffer);
}

GLAPI void APIENTRY glInitNames(void)
{
	static void (*func)();
	if(!func)
		func=glGetProcAddressNoNull("glInitNames");
	func();
}

GLAPI void APIENTRY glLoadName(GLuint name)
{
	static void (*func)(GLuint name);
	if(!func)
		func=glGetProcAddressNoNull("glLoadName");
	func(name);
}

GLAPI void APIENTRY glPushName(GLuint name)
{
	static void (*func)(GLuint name);
	if(!func)
		func=glGetProcAddressNoNull("glPushName");
	func(name);
}

GLAPI void APIENTRY glPopName(void)
{
	static void (*func)();
	if(!func)
		func=glGetProcAddressNoNull("glPopName");
	func();
}


/* 1.1 functions */
/* texture objects */
GLAPI void APIENTRY glGenTextures(GLsizei n, GLuint *textures)
{
	static void (*func)(GLsizei n, GLuint *textures);
	if(!func)
		func=glGetProcAddressNoNull("glGenTextures");
	func(n, textures);
}

GLAPI void APIENTRY glDeleteTextures(GLsizei n, const GLuint *textures)
{
	static void (*func)(GLsizei n, GLuint *textures);
	if(!func)
		func=glGetProcAddressNoNull("glDeleteTextures");
	func(n, textures);
}

GLAPI void APIENTRY glBindTexture(GLenum target, GLuint texture)
{
	static void (*func)(GLenum target, GLuint texture);
	if(!func)
		func=glGetProcAddressNoNull("glBindTexture");
	func(target, texture);
}

GLAPI void APIENTRY glPrioritizeTextures(
	GLsizei n, const GLuint *textures, const GLclampf *priorities)
{
	static void (*func)(GLsizei n, GLuint *textures, GLclampf *priorities);
	if(!func)
		func=glGetProcAddressNoNull("glPrioritizeTextures");
	func(n, textures, priorities);
}

GLAPI GLboolean APIENTRY glAreTexturesResident(
	GLsizei n, const GLuint *textures, GLboolean *residences)
{
	static GLboolean (*func)(
		GLsizei n, GLuint *textures, GLboolean *residences);
	if(!func)
		func=glGetProcAddressNoNull("glAreTexturesResident");
	return(func(n, textures, residences));
}

GLAPI GLboolean APIENTRY glIsTexture(GLuint texture)
{
	static GLboolean (*func)(GLuint texture);
	if(!func)
		func=glGetProcAddressNoNull("glIsTexture");
	return(func(texture));
}

/* texture mapping */
GLAPI void APIENTRY glTexSubImage1D(
	GLenum target, GLint level, GLint xoffset,
	GLsizei width, GLenum format, GLenum type,
	const GLvoid *pixels)
{
	static void (*func)(
		GLenum target, GLint level, GLint xoffset,
		GLsizei width, GLenum format, GLenum type,
		const GLvoid *pixels);
	if(!func)
		func=glGetProcAddressNoNull("glTexSubImage1D");
	func(target, level, xoffset, width, format, type, pixels);
}

GLAPI void APIENTRY glTexSubImage2D(
	GLenum target, GLint level, GLint xoffset, GLint yoffset,
	GLsizei width, GLsizei height, GLenum format, GLenum type,
	const GLvoid *pixels)
{
	static void (*func)(
		GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLsizei width, GLsizei height, GLenum format, GLenum type,
		const GLvoid *pixels);
	if(!func)
		func=glGetProcAddressNoNull("glTexSubImage2D");
	func(target, level, xoffset, yoffset, width, height, format, type, pixels);
}

GLAPI void APIENTRY glCopyTexImage1D(
	GLenum target, GLint level, GLenum internalformat,
	GLint x, GLint y, GLsizei width, GLint border)
{
	static void (*func)(
		GLenum target, GLint level, GLenum internalformat,
		GLint x, GLint y, GLsizei width,
		GLint border);
	if(!func)
		func=glGetProcAddressNoNull("glCopyTexImage1D");
	func(target, level, internalformat, x, y, width, border);
}

GLAPI void APIENTRY glCopyTexImage2D(
	GLenum target, GLint level, GLenum internalformat,
	GLint x, GLint y, GLsizei width, GLsizei height,
	GLint border)
{
	static void (*func)(
		GLenum target, GLint level, GLenum internalformat,
		GLint x, GLint y, GLsizei width, GLsizei height,
		GLint border);
	if(!func)
		func=glGetProcAddressNoNull("glCopyTexImage2D");
	func(target, level, internalformat, x, y, width, height, border);
}

GLAPI void APIENTRY glCopyTexSubImage1D(
	GLenum target, GLint level, GLint xoffset,
	GLint x, GLint y, GLsizei width)
{
	static void (*func)(
		GLenum target, GLint level, GLint xoffset,
		GLint x, GLint y, GLsizei width);
	if(!func)
		func=glGetProcAddressNoNull("glCopyTexSubImage1D");
	func(target, level, xoffset, x, y, width);
}

GLAPI void APIENTRY glCopyTexSubImage2D(
	GLenum target, GLint level, GLint xoffset, GLint yoffset,
	GLint x, GLint y, GLsizei width, GLsizei height)
{
	static void (*func)(
		GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLint x, GLint y, GLsizei width, GLsizei height);
	if(!func)
		func=glGetProcAddressNoNull("glCopyTexSubImage2D");
	func(target, level, xoffset, yoffset, x, y, width, height);
}

/* vertex arrays */
GLAPI void APIENTRY glVertexPointer(
	GLint size, GLenum type, GLsizei stride, const GLvoid *ptr)
{
	static void (*func)(
		GLint size, GLenum type, GLsizei stride, const GLvoid *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glVertexPointer");
	func(size, type, stride, ptr);
}

GLAPI void APIENTRY glNormalPointer(
	GLenum type, GLsizei stride, const GLvoid *ptr)
{
	static void (*func)(
		GLenum type, GLsizei stride, const GLvoid *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glNormalPointer");
	func(type, stride, ptr);
}

GLAPI void APIENTRY glColorPointer(
	GLint size, GLenum type, GLsizei stride, const GLvoid *ptr)
{
	static void (*func)(
		GLint size, GLenum type, GLsizei stride, const GLvoid *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glColorPointer");
	func(size, type, stride, ptr);
}

GLAPI void APIENTRY glIndexPointer(
	GLenum type, GLsizei stride, const GLvoid *ptr)
{
	static void (*func)(
		GLenum type, GLsizei stride, const GLvoid *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glIndexPointer");
	func(type, stride, ptr);
}

GLAPI void APIENTRY glTexCoordPointer(
	GLint size, GLenum type, GLsizei stride, const GLvoid *ptr)
{
	static void (*func)(
		GLint size, GLenum type, GLsizei stride, const GLvoid *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glTexCoordPointer");
	func(size, type, stride, ptr);
}

GLAPI void APIENTRY glEdgeFlagPointer(GLsizei stride, const GLvoid *ptr)
{
	static void (*func)(
		GLsizei stride, const GLvoid *ptr);
	if(!func)
		func=glGetProcAddressNoNull("glEdgeFlagPointer");
	func(stride, ptr);
}

GLAPI void APIENTRY glGetPointerv(GLenum pname, GLvoid **params)
{
	static void (*func)(GLenum pname, GLvoid **params);
	if(!func)
		func=glGetProcAddressNoNull("glGetPointerv");
	func(pname, params);
}

GLAPI void APIENTRY glArrayElement(GLint i)
{
	static void (*func)(GLint i);
	if(!func)
		func=glGetProcAddressNoNull("glArrayElement");
	func(i);
}

GLAPI void APIENTRY glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
	static void (*func)(
		GLenum mode, GLint first, GLsizei count);
	if(!func)
		func=glGetProcAddressNoNull("glDrawArrays");
	func(mode, first, count);
}

GLAPI void APIENTRY glDrawElements(
	GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
	static void (*func)(
		GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
	if(!func)
		func=glGetProcAddressNoNull("glDrawElements");
	func(mode, count, type, indices);
}

GLAPI void APIENTRY glInterleavedArrays(
	GLenum format, GLsizei stride, const GLvoid *pointer)
{
	static void (*func)(
		GLenum format, GLsizei stride, const GLvoid *pointer);
	if(!func)
		func=glGetProcAddressNoNull("glInterleavedArrays");
	func(format, stride, pointer);
}


/* Under Windows, we do not define OpenGL 1.2 & 1.3 functionality, since
   it is treated as extensions (defined in glext.h) */

/* 1.2 functions */
GLAPI void APIENTRY glDrawRangeElements(
	GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type,
	const GLvoid *indices)
{
	static void (*func)(
		GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type,
		const GLvoid *indices);
	if(!func)
		func=glGetProcAddressNoNull("glDrawRangeElements");
	func(mode, start, end, count, type, indices);
}

GLAPI void APIENTRY glTexImage3D(
	GLenum target, GLint level, GLenum internalFormat,
	GLsizei width, GLsizei height, GLsizei depth, GLint border,
	GLenum format, GLenum type, const GLvoid *pixels)
{
	static void (*func)(
		GLenum target, GLint level, GLenum internalFormat,
		GLsizei width, GLsizei height, GLsizei depth, GLint border,
		GLenum format, GLenum type, const GLvoid *pixels);
	if(!func)
		func=glGetProcAddressNoNull("glTexImage3D");
	func(
		target, level, internalformat,
		width, height, depth, border,
		format, type, pixels);
}

GLAPI void APIENTRY glTexSubImage3D(
	GLenum target, GLint level,
	GLint xoffset, GLint yoffset, GLint zoffset,
	GLsizei width, GLsizei height, GLsizei depth,
	GLenum format, GLenum type, const GLvoid *pixels)
{
	static void (*func)(
		GLenum target, GLint level,
		GLint xoffset, GLint yoffset, GLint zoffset,
		GLsizei width, GLsizei height, GLsizei depth,
		GLenum format, GLenum type, const GLvoid *pixels);
	if(!func)
		func=glGetProcAddressNoNull("glTexSubImage3D");
	func(
		target, level,
		xoffset, yoffset, zoffset,
		width, height, depth,
		format, type, pixels);
}

GLAPI void APIENTRY glCopyTexSubImage3D(
	GLenum target, GLint level,
	GLint xoffset, GLint yoffset, GLint zoffset,
	GLint x, GLint y, GLsizei width, GLsizei height)
{
	static void (*func)(
		GLenum target, GLint level,
		GLint xoffset, GLint yoffset, GLint zoffset,
		GLint x, GLint y, GLsizei width, GLsizei height);
	if(!func)
		func=glGetProcAddressNoNull("glCopyTexSubImage3D");
	func(
		target, level,
		xoffset, yoffset, zoffset,
		x, y, width, height);
}

/* 1.2 imaging extension functions */
GLAPI void APIENTRY glColorTable(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
GLAPI void APIENTRY glColorSubTable(GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data);
GLAPI void APIENTRY glColorTableParameteriv(GLenum target, GLenum pname, const GLint *params);
GLAPI void APIENTRY glColorTableParameterfv(GLenum target, GLenum pname, const GLfloat *params);
GLAPI void APIENTRY glCopyColorSubTable(GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);
GLAPI void APIENTRY glCopyColorTable(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
GLAPI void APIENTRY glGetColorTable(GLenum target, GLenum format, GLenum type, GLvoid *table);
GLAPI void APIENTRY glGetColorTableParameterfv(GLenum target, GLenum pname, GLfloat *params);
GLAPI void APIENTRY glGetColorTableParameteriv(GLenum target, GLenum pname, GLint *params);

GLAPI void APIENTRY glBlendEquation(GLenum mode)
{
	static void (*func)(GLenum mode);
	if(!func)
		func=glGetProcAddressNoNull("glBlendEquation");
	func(mode);
}

GLAPI void APIENTRY glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
GLAPI void APIENTRY glHistogram(GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
GLAPI void APIENTRY glResetHistogram(GLenum target);
GLAPI void APIENTRY glGetHistogram(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
GLAPI void APIENTRY glGetHistogramParameterfv(GLenum target, GLenum pname, GLfloat *params);
GLAPI void APIENTRY glGetHistogramParameteriv(GLenum target, GLenum pname, GLint *params);
GLAPI void APIENTRY glMinmax(GLenum target, GLenum internalformat, GLboolean sink);
GLAPI void APIENTRY glResetMinmax(GLenum target);
GLAPI void APIENTRY glGetMinmax(GLenum target, GLboolean reset, GLenum format, GLenum types, GLvoid *values);
GLAPI void APIENTRY glGetMinmaxParameterfv(GLenum target, GLenum pname, GLfloat *params);
GLAPI void APIENTRY glGetMinmaxParameteriv(GLenum target, GLenum pname, GLint *params);
GLAPI void APIENTRY glConvolutionFilter1D(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image);
GLAPI void APIENTRY glConvolutionFilter2D(
	GLenum target, GLenum internalformat, GLsizei width, GLsizei height,
	GLenum format, GLenum type, const GLvoid *image);
GLAPI void APIENTRY glConvolutionParameterf(
	GLenum target, GLenum pname, GLfloat params);
GLAPI void APIENTRY glConvolutionParameterfv(
	GLenum target, GLenum pname, const GLfloat *params);
GLAPI void APIENTRY glConvolutionParameteri(
	GLenum target, GLenum pname, GLint params);
GLAPI void APIENTRY glConvolutionParameteriv(
	GLenum target, GLenum pname, const GLint *params);
GLAPI void APIENTRY glCopyConvolutionFilter1D(
	GLenum target, GLenum internalformat, GLint x, GLint y,
	GLsizei width);
GLAPI void APIENTRY glCopyConvolutionFilter2D(
	GLenum target, GLenum internalformat, GLint x, GLint y,
	GLsizei width, GLsizei height);
GLAPI void APIENTRY glGetConvolutionFilter(
	GLenum target, GLenum format, GLenum type, GLvoid *image);
GLAPI void APIENTRY glGetConvolutionParameterfv(
	GLenum target, GLenum pname, GLfloat *params);
GLAPI void APIENTRY glGetConvolutionParameteriv(
	GLenum target, GLenum pname, GLint *params);
GLAPI void APIENTRY glSeparableFilter2D(
	GLenum target, GLenum internalformat, GLsizei width, GLsizei height,
	GLenum format, GLenum type, const GLvoid *row, const GLvoid *column);
GLAPI void APIENTRY glGetSeparableFilter(
	GLenum target, GLenum format, GLenum type,
	GLvoid *row, GLvoid *column, GLvoid *span);

/* 1.3 functions */
GLAPI void APIENTRY glActiveTexture(GLenum texture)
{
	static void (*func)(GLenum texture);
	if(!func)
		func=glGetProcAddressNoNull("glActiveTexture");
	func(texture);
}

GLAPI void APIENTRY glClientActiveTexture(GLenum texture)
{
	static void (*func)(GLenum texture);
	if(!func)
		func=glGetProcAddressNoNull("glClientActiveTexture");
	func(texture);
}

GLAPI void APIENTRY glCompressedTexImage1D(
	GLenum target, GLint level, GLenum internalformat,
	GLsizei width, GLint border, GLsizei imageSize,
	const GLvoid *data)
{
	static void (*func)(
		GLenum target, GLint level, GLenum internalformat,
		GLsizei width, GLint border, GLsizei imageSize,
		const GLvoid *data);
	if(!func)
		func=glGetProcAddressNoNull("glCompressedTexImage1D");
	func(target, level, internalformat, width, border, imageSize, data);
}

GLAPI void APIENTRY glCompressedTexImage2D(
	GLenum target, GLint level, GLenum internalformat,
	GLsizei width, GLsizei height, GLint border,
	GLsizei imageSize, const GLvoid *data)
{
	static void (*func)(
		GLenum target, GLint level, GLenum internalformat,
		GLsizei width, GLsizei height, GLint border,
		GLsizei imageSize, const GLvoid *data);
	if(!func)
		func=glGetProcAddressNoNull("glCompressedTexImage2D");
	func(target, level, internalformat, width, height,
		border, imageSize, data);
}

GLAPI void APIENTRY glCompressedTexImage3D(
	GLenum target, GLint level, GLenum internalformat,
	GLsizei width, GLsizei height, GLsizei depth, GLint border,
	GLsizei imageSize, const GLvoid *data)
{
	static void (*func)(
		GLenum target, GLint level, GLenum internalformat,
		GLsizei width, GLsizei height, GLsizei depth, GLint border,
		GLsizei imageSize, const GLvoid *data);
	if(!func)
		func=glGetProcAddressNoNull("glCompressedTexImage3D");
	func(target, level, internalformat,
		width, height, depth,
		border, imageSize, data);
}

GLAPI void APIENTRY glCompressedTexSubImage1D(
	GLenum target, GLint level, GLint xoffset, GLsizei width,
	GLenum format, GLsizei imageSize, const GLvoid *data)
{
	static void (*func)(
		GLenum target, GLint level,
		GLint xoffset, GLsizei width,
		GLenum format, GLsizei imageSize, const GLvoid *data);
	if(!func)
		func=glGetProcAddressNoNull("glCompressedTexSubImage1D");
	func(target, level, xoffset, width,
		format, imageSize, data);
}

GLAPI void APIENTRY glCompressedTexSubImage2D(
	GLenum target, GLint level,
	GLint xoffset, GLint yoffset, GLsizei width, GLsizei height,
	GLenum format, GLsizei imageSize, const GLvoid *data)
{
	static void (*func)(
		GLenum target, GLint level,
		GLint xoffset, GLint yoffset,
		GLsizei width, GLsizei height,
		GLenum format, GLsizei imageSize, const GLvoid *data);
	if(!func)
		func=glGetProcAddressNoNull("glCompressedTexSubImage2D");
	func(target, level, xoffset, yoffset,
		width, height,
		format, imageSize, data);
}

GLAPI void APIENTRY glCompressedTexSubImage3D(
	GLenum target, GLint level,
	GLint xoffset, GLint yoffset, GLint zoffset,
	GLsizei width, GLsizei height, GLsizei depth,
	GLenum format, GLsizei imageSize, const GLvoid *data)
{
	static void (*func)(
		GLenum target, GLint level,
		GLint xoffset, GLint yoffset, GLint zoffset,
		GLsizei width, GLsizei height, GLsizei depth,
		GLenum format, GLsizei imageSize, const GLvoid *data);
	if(!func)
		func=glGetProcAddressNoNull("glCompressedTexSubImage3D");
	func(target, level, xoffset, yoffset, zoffset,
		width, height, depth,
		format, imageSize, data);
}

GLAPI void APIENTRY glGetCompressedTexImage(
	GLenum target, GLint lod, GLvoid *img)
{
	static void (*func)(
		GLenum target, GLint lod, GLvoid *img);
	if(!func)
		func=glGetProcAddressNoNull("glGetCompressedTexImage");
	func(target, lod, data);
}

GLAPI void APIENTRY glMultiTexCoord1d(GLenum target, GLdouble s)
{
	static void (*func)(
		GLenum target, GLdouble s);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord1f");
	func(target, s);
}

GLAPI void APIENTRY glMultiTexCoord1f(GLenum target, GLfloat s)
{
	static void (*func)(
		GLenum target, GLdouble s);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord1f");
	func(target, s);
}

GLAPI void APIENTRY glMultiTexCoord1i(GLenum target, GLint s)
{
	static void (*func)(
		GLenum target, GLint s);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord1i");
	func(target, s);
}

GLAPI void APIENTRY glMultiTexCoord1s(GLenum target, GLshort s)
{
	static void (*func)(
		GLenum target, GLint s);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord1s");
	func(target, s);
}

GLAPI void APIENTRY glMultiTexCoord2d(GLenum target, GLdouble s, GLdouble t)
{
	static void (*func)(
		GLenum target, GLdouble s, GLdouble t);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord2d");
	func(target, s, t);
}

GLAPI void APIENTRY glMultiTexCoord2f(GLenum target, GLfloat s, GLfloat t)
{
	static void (*func)(
		GLenum target, GLdouble s, GLdouble t);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord2f");
	func(target, s, t);
}

GLAPI void APIENTRY glMultiTexCoord2i(GLenum target, GLint s, GLint t)
{
	static void (*func)(
		GLenum target, GLint s, GLint t);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord2i");
	func(target, s, t);
}

GLAPI void APIENTRY glMultiTexCoord2s(GLenum target, GLshort s, GLshort t)
{
	static void (*func)(
		GLenum target, GLint s, GLint t);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord2s");
	func(target, s, t);
}

GLAPI void APIENTRY glMultiTexCoord3d(
	GLenum target, GLdouble s, GLdouble t, GLdouble r)
{
	static void (*func)(
		GLenum target, GLdouble s, GLdouble t, GLdouble r);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord3d");
	func(target, s, t, r);
}

GLAPI void APIENTRY glMultiTexCoord3f(
	GLenum target, GLfloat s, GLfloat t, GLfloat r)
{
	static void (*func)(
		GLenum target, GLdouble s, GLdouble t, GLdouble r);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord4d");
	func(target, s, t, r);
}

GLAPI void APIENTRY glMultiTexCoord3i(
	GLenum target, GLint s, GLint t, GLint r)
{
	static void (*func)(
		GLenum target, GLint s, GLint t, GLint r);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord3i");
	func(target, s, t, r);
}

GLAPI void APIENTRY glMultiTexCoord3s(
	GLenum target, GLshort s, GLshort t, GLshort r)
{
	static void (*func)(
		GLenum target, GLint s, GLint t, GLint r);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord3s");
	func(target, s, t, r);
}

GLAPI void APIENTRY glMultiTexCoord4d(
	GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q)
{
	static void (*func)(
		GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord4d");
	func(target, s, t, r, q);
}

GLAPI void APIENTRY glMultiTexCoord4f(
	GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
	static void (*func)(
		GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord4f");
	func(target, s, t, r, q);
}

GLAPI void APIENTRY glMultiTexCoord4i(GLenum target, GLint s, GLint t, GLint r, GLint q)
{
	static void (*func)(
		GLenum target, GLint s, GLint t, GLint r, GLint q);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord4i");
	func(target, s, t, r, q);
}

GLAPI void APIENTRY glMultiTexCoord4s(
	GLenum target, GLshort s, GLshort t, GLshort r, GLshort q)
{
	static void (*func)(
		GLenum target, GLint s, GLint t, GLint r, GLint q);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord4s");
	func(target, s, t, r, q);
}

GLAPI void APIENTRY glMultiTexCoord1dv(GLenum target, const GLdouble *v)
{
	static void (*func)(GLenum target, void *v);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord1dv");
	func(target, v);
}

GLAPI void APIENTRY glMultiTexCoord1fv(GLenum target, const GLfloat *v)
{
	static void (*func)(GLenum target, void *v);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord1fv");
	func(target, v);
}

GLAPI void APIENTRY glMultiTexCoord1iv(GLenum target, const GLint *v)
{
	static void (*func)(GLenum target, void *v);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord1iv");
	func(target, v);
}

GLAPI void APIENTRY glMultiTexCoord1sv(GLenum target, const GLshort *v)
{
	static void (*func)(GLenum target, void *v);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord1sv");
	func(target, v);
}

GLAPI void APIENTRY glMultiTexCoord2dv(GLenum target, const GLdouble *v)
{
	static void (*func)(GLenum target, void *v);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord2dv");
	func(target, v);
}

GLAPI void APIENTRY glMultiTexCoord2fv(GLenum target, const GLfloat *v)
{
	static void (*func)(GLenum target, void *v);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord2fv");
	func(target, v);
}

GLAPI void APIENTRY glMultiTexCoord2iv(GLenum target, const GLint *v)
{
	static void (*func)(GLenum target, void *v);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord2iv");
	func(target, v);
}

GLAPI void APIENTRY glMultiTexCoord2sv(GLenum target, const GLshort *v)
{
	static void (*func)(GLenum target, void *v);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord2sv");
	func(target, v);
}

GLAPI void APIENTRY glMultiTexCoord3dv(GLenum target, const GLdouble *v)
{
	static void (*func)(GLenum target, void *v);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord3dv");
	func(target, v);
}

GLAPI void APIENTRY glMultiTexCoord3fv(GLenum target, const GLfloat *v)
{
	static void (*func)(GLenum target, void *v);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord3fv");
	func(target, v);
}

GLAPI void APIENTRY glMultiTexCoord3iv(GLenum target, const GLint *v)
{
	static void (*func)(GLenum target, void *v);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord3iv");
	func(target, v);
}

GLAPI void APIENTRY glMultiTexCoord3sv(GLenum target, const GLshort *v)
{
	static void (*func)(GLenum target, void *v);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord3sv");
	func(target, v);
}

GLAPI void APIENTRY glMultiTexCoord4dv(GLenum target, const GLdouble *v)
{
	static void (*func)(GLenum target, void *v);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord4dv");
	func(target, v);
}

GLAPI void APIENTRY glMultiTexCoord4fv(GLenum target, const GLfloat *v)
{
	static void (*func)(GLenum target, void *v);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord4fv");
	func(target, v);
}

GLAPI void APIENTRY glMultiTexCoord4iv(GLenum target, const GLint *v)
{
	static void (*func)(GLenum target, void *v);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord4iv");
	func(target, v);
}

GLAPI void APIENTRY glMultiTexCoord4sv(GLenum target, const GLshort *v)
{
	static void (*func)(GLenum target, void *v);
	if(!func)
		func=glGetProcAddressNoNull("glMultiTexCoord4sv");
	func(target, v);
}

GLAPI void APIENTRY glLoadTransposeMatrixd(const GLdouble m[16])
{
	static void (*func)(void *v);
	if(!func)
		func=glGetProcAddressNoNull("glLoadTransposeMatrixd");
	func(m);
}

GLAPI void APIENTRY glLoadTransposeMatrixf(const GLfloat m[16])
{
	static void (*func)(void *v);
	if(!func)
		func=glGetProcAddressNoNull("glLoadTransposeMatrixf");
	func(m);
}

GLAPI void APIENTRY glMultTransposeMatrixd(const GLdouble m[16])
{
	static void (*func)(void *v);
	if(!func)
		func=glGetProcAddressNoNull("glMultTransposeMatrixd");
	func(m);
}

GLAPI void APIENTRY glMultTransposeMatrixf(const GLfloat m[16])
{
	static void (*func)(void *v);
	if(!func)
		func=glGetProcAddressNoNull("glMultTransposeMatrixf");
	func(m);
}

GLAPI void APIENTRY glSampleCoverage(GLclampf value, GLboolean invert)
{
	static void (*func)(GLclampf value, GLboolean invert);
	if(!func)
		func=glGetProcAddressNoNull("glSampleCoverage");
	func(value, invert);
}

GLAPI void APIENTRY glSamplePass(GLenum pass)
{
	static void (*func)(GLenum pass);
	if(!func)
		func=glGetProcAddressNoNull("glSamplePass");
	func(pass);
}

