// #include <windows.h>
#include <bgbrasw.h>

RASWGL_Context *rasw_glctx=NULL;

#define GLCTX	RaGlGetContext()

BGBRASW_API RASWGL_Context *RaGlGetContext()
{
	return(rasw_glctx);
}

BGBRASW_API void RaGlSetContext(RASWGL_Context *ctx)
{
	rasw_glctx=ctx;
}

BGBRASW_API RASWGL_Context *RaGlCreateContext(int xs, int ys, int flags)
{
	return(RASWGL_CreateContext(xs, ys, flags));
}

void RaGlSetErrorNopStub(void)
{
	RASWGL_Context *ctx;
	ctx=RaGlGetContext();
	ctx->glerror=GL_INVALID_OPERATION;
	*(int *)-1=-1;
}


/* miscellaneous */
BGBRASW_API void GLDRV_APIENTRY GlDrv_glClearIndex(
		GLfloat c)
	{ RaGlSetErrorNopStub(); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glClearColor(
		GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	GLCTX->clear_clr=bgbrasw_makePixel4f(
		red, green, blue, alpha);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glClear(
	GLbitfield mask)
{
	RASWGL_Clear(GLCTX, mask);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glIndexMask(
		GLuint mask)
	{ RaGlSetErrorNopStub(); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glColorMask(
		GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
	GLCTX->mask_clr=
		(red  ?0x00FF0000UL:0)|
		(green?0x0000FF00UL:0)|
		(blue ?0x000000FFUL:0)|
		(alpha?0xFF000000UL:0);
		
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glAlphaFunc(
		GLenum func, GLclampf ref)
{
	RASWGL_AlphaFunc(GLCTX, func, ref);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glBlendFunc(
		GLenum sfactor, GLenum dfactor)
{
	RASWGL_BlendFunc(GLCTX, sfactor, dfactor);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glLogicOp(
		GLenum opcode)
	{ RaGlSetErrorNopStub(); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glCullFace(
		GLenum mode)
{
	GLCTX->cull_face=mode;
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glFrontFace(
		GLenum mode)
{
	GLCTX->front_face=mode;
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glPointSize(
		GLfloat size)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glLineWidth(
		GLfloat width)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glLineStipple(
		GLint factor, GLushort pattern)
	{ RaGlSetErrorNopStub(); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glPolygonMode(
		GLenum face, GLenum mode)
{
	GLCTX->polygon_mode=mode;
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glPolygonOffset(
		GLfloat factor, GLfloat units)
{
	GLCTX->offset_factor=factor;
	GLCTX->offset_units=units;
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glPolygonStipple(
		const GLubyte *mask)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetPolygonStipple(
		GLubyte *mask)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glEdgeFlag(
		GLboolean flag)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glEdgeFlagv(
		const GLboolean *flag)
	{ RaGlSetErrorNopStub(); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glScissor(
		GLint x, GLint y, GLsizei width, GLsizei height)
{
	GLCTX->scissor_x=x;
	GLCTX->scissor_y=y;
	GLCTX->scissor_xs=width;
	GLCTX->scissor_ys=height;
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glClipPlane(
		GLenum plane, const GLdouble *equation)
{
	RASWGL_ClipPlane(GLCTX, plane, (GLdouble *)equation);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetClipPlane(
		GLenum plane, GLdouble *equation)
	{ RaGlSetErrorNopStub(); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glDrawBuffer(
		GLenum mode)
{
	GLCTX->draw_buffer=mode;
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glReadBuffer(
		GLenum mode)
{
	GLCTX->read_buffer=mode;
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glEnable(
	GLenum cap)
{
	RASWGL_Enable(GLCTX, cap);
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glDisable(
	GLenum cap)
{
	RASWGL_Disable(GLCTX, cap);
}

BGBRASW_API GLboolean GLDRV_APIENTRY GlDrv_glIsEnabled(
	GLenum cap)
{
	return(RASWGL_IsEnabled(GLCTX, cap));
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glEnableClientState(GLenum cap)
{
	RASWGL_EnableClientState(GLCTX, cap);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glDisableClientState(GLenum cap)
{
	RASWGL_DisableClientState(GLCTX, cap);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetBooleanv(
		GLenum pname, GLboolean *params)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetDoublev(
		GLenum pname, GLdouble *params)
	{ RaGlSetErrorNopStub(); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetFloatv(
		GLenum pname, GLfloat *params)
{
	RASWGL_GetFloatv(GLCTX, pname, params);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetIntegerv(
		GLenum pname, GLint *params)
{
	RASWGL_GetIntegerv(GLCTX, pname, params);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glPushAttrib(GLbitfield mask)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glPopAttrib(void)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glPushClientAttrib(GLbitfield mask)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glPopClientAttrib(void)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API GLint GLDRV_APIENTRY GlDrv_glRenderMode(GLenum mode)
	{ RaGlSetErrorNopStub(); return(-1); }

BGBRASW_API GLenum GLDRV_APIENTRY GlDrv_glGetError(void)
{
	RASWGL_Context *ctx;
	int i;

	ctx=RaGlGetContext();
	if(ctx)
	{
		i=ctx->glerror;
		ctx->glerror=0;
		return(i);
	}
	return(GL_INVALID_OPERATION);
	
//	return(GL_INVALID_OPERATION);
}

BGBRASW_API const GLubyte* GLDRV_APIENTRY GlDrv_glGetString(GLenum name)
{
	return(RASWGL_GetString(GLCTX, name));
//	return(NULL);
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glFinish(void)
{
	RASWGL_Finish(GLCTX);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glFlush(void)
{
	RASWGL_Flush(GLCTX);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glHint(GLenum target, GLenum mode)
{
//	RaGlSetErrorNopStub();
}

/* Depth Buffer */
BGBRASW_API void GLDRV_APIENTRY GlDrv_glClearDepth(GLclampd depth)
	{ GLCTX->depth_clear=depth; }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glDepthFunc(GLenum func)
	{ GLCTX->depth_func=func; }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glDepthMask(GLboolean flag)
{
	if(flag)
		{ RASWGL_Enable(GLCTX, GL_DEPTH_WRITEMASK); }
	else
		{ RASWGL_Disable(GLCTX, GL_DEPTH_WRITEMASK); }
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glDepthRange(
		GLclampd near_val, GLclampd far_val)
{
	GLCTX->depth_near=near_val;
	GLCTX->depth_far=far_val;
}

/* Accumulation Buffer */
BGBRASW_API void GLDRV_APIENTRY GlDrv_glClearAccum(
		GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glAccum(GLenum op, GLfloat value)
	{ RaGlSetErrorNopStub(); }

/* Transformation */

BGBRASW_API void GLDRV_APIENTRY GlDrv_glMatrixMode(GLenum mode)
{
	RASWGL_MatrixMode(GLCTX, mode);
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glOrtho(
		GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, 
		GLdouble near_val, GLdouble far_val)
{
	RASWGL_SetupOrtho(GLCTX, left, right, bottom, top,
		near_val, far_val);
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glFrustum(
		GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, 
		GLdouble near_val, GLdouble far_val)
{
	RASWGL_SetupFrustum(GLCTX, left, right, bottom, top,
		near_val, far_val);
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glViewport(
		GLint x, GLint y, GLsizei width, GLsizei height)
{
	GLCTX->viewport_x=x;
	GLCTX->viewport_y=y;
	GLCTX->viewport_xs=width;
	GLCTX->viewport_ys=height;
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glPushMatrix(void)
	{ RASWGL_PushMatrix(GLCTX); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glPopMatrix(void)
	{ RASWGL_PopMatrix(GLCTX); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glLoadIdentity(void)
{
	RASWGL_LoadIdentityMatrix(GLCTX);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glLoadMatrixd(const GLdouble *m)
{
	float tm[16];
	VecND2F_Copy((double *)m, tm, 16);
	RASWGL_LoadMatrix(GLCTX, tm);
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glLoadMatrixf(const GLfloat *m)
{
	RASWGL_LoadMatrix(GLCTX, (float *)m);
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultMatrixd(const GLdouble *m)
{
	float tm[16];
	VecND2F_Copy((double *)m, tm, 16);
	RASWGL_MultMatrix(GLCTX, tm);
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultMatrixf(const GLfloat *m)
{
	RASWGL_MultMatrix(GLCTX, (float *)m);
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glRotated(
		GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
	{ RASWGL_Rotatef(GLCTX, angle, x, y, z); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRotatef(
		GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
	{ RASWGL_Rotatef(GLCTX, angle, x, y, z); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glScaled(
		GLdouble x, GLdouble y, GLdouble z)
	{ RASWGL_Scalef(GLCTX, x, y, z); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glScalef(
		GLfloat x, GLfloat y, GLfloat z)
	{ RASWGL_Scalef(GLCTX, x, y, z); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTranslated(
		GLdouble x, GLdouble y, GLdouble z)
	{ RASWGL_Translatef(GLCTX, x, y, z); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTranslatef(
		GLfloat x, GLfloat y, GLfloat z)
	{ RASWGL_Translatef(GLCTX, x, y, z); }

/* Display Lists */
BGBRASW_API GLboolean GLDRV_APIENTRY GlDrv_glIsList(GLuint list)
	{ return(0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glDeleteLists(GLuint list, GLsizei range)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API GLuint GLDRV_APIENTRY GlDrv_glGenLists(GLsizei range)
	{ return(-1); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glNewList(GLuint list, GLenum mode)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glEndList(void)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glCallList(GLuint list)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glCallLists(
		GLsizei n, GLenum type, const GLvoid *lists)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glListBase(GLuint base)
	{ RaGlSetErrorNopStub(); }

/* Drawing Functions */
BGBRASW_API void GLDRV_APIENTRY GlDrv_glBegin(GLenum mode)
{
	RASWGL_Begin(GLCTX, mode);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glEnd(void)
{
	RASWGL_End(GLCTX);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertex2d(GLdouble x, GLdouble y)
	{ RASWGL_Vertex2f(GLCTX, x, y); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertex2f(GLfloat x, GLfloat y)
	{ RASWGL_Vertex2f(GLCTX, x, y); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertex2i(GLint x, GLint y)
	{ RASWGL_Vertex2f(GLCTX, x, y); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertex2s(GLshort x, GLshort y)
	{ RASWGL_Vertex2f(GLCTX, x, y); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertex3d(
		GLdouble x, GLdouble y, GLdouble z)
	{ RASWGL_Vertex3f(GLCTX, x, y, z); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertex3f(
		GLfloat x, GLfloat y, GLfloat z)
	{ RASWGL_Vertex3f(GLCTX, x, y, z); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertex3i(
		GLint x, GLint y, GLint z)
	{ RASWGL_Vertex3f(GLCTX, x, y, z); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertex3s(
		GLshort x, GLshort y, GLshort z)
	{ RASWGL_Vertex3f(GLCTX, x, y, z); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertex4d(
		GLdouble x, GLdouble y, GLdouble z, GLdouble w)
	{ RASWGL_Vertex4f(GLCTX, x, y, z, w); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertex4f(
		GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{ RASWGL_Vertex4f(GLCTX, x, y, z, w); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertex4i(
		GLint x, GLint y, GLint z, GLint w)
	{ RASWGL_Vertex4f(GLCTX, x, y, z, w); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertex4s(
		GLshort x, GLshort y, GLshort z, GLshort w)
	{ RASWGL_Vertex4f(GLCTX, x, y, z, w); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertex2dv(const GLdouble *v)
	{ RASWGL_Vertex2f(GLCTX, v[0], v[1]); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertex2fv(const GLfloat *v)
	{ RASWGL_Vertex2f(GLCTX, v[0], v[1]); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertex2iv(const GLint *v)
	{ RASWGL_Vertex2f(GLCTX, v[0], v[1]); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertex2sv(const GLshort *v)
	{ RASWGL_Vertex2f(GLCTX, v[0], v[1]); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertex3dv(const GLdouble *v)
	{ RASWGL_Vertex3f(GLCTX, v[0], v[1], v[2]); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertex3fv(const GLfloat *v)
	{ RASWGL_Vertex3fv(GLCTX, (float *)v); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertex3iv(const GLint *v)
	{ RASWGL_Vertex3f(GLCTX, v[0], v[1], v[2]); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertex3sv(const GLshort *v)
	{ RASWGL_Vertex3f(GLCTX, v[0], v[1], v[2]); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertex4dv(const GLdouble *v)
	{ RASWGL_Vertex4f(GLCTX, v[0], v[1], v[2], v[3]); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertex4fv(const GLfloat *v)
	{ RASWGL_Vertex4fv(GLCTX, (float *)v); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertex4iv(const GLint *v)
	{ RASWGL_Vertex4f(GLCTX, v[0], v[1], v[2], v[3]); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertex4sv(const GLshort *v)
	{ RASWGL_Vertex4f(GLCTX, v[0], v[1], v[2], v[3]); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glNormal3b(
		GLbyte nx, GLbyte ny, GLbyte nz)
	{ RASWGL_Normal3f(GLCTX, nx/127.0, ny/127.0, nz/127.0); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glNormal3d(
		GLdouble nx, GLdouble ny, GLdouble nz)
	{ RASWGL_Normal3f(GLCTX, nx, ny, nz); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glNormal3f(
		GLfloat nx, GLfloat ny, GLfloat nz)
	{ RASWGL_Normal3f(GLCTX, nx, ny, nz); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glNormal3i(
		GLint nx, GLint ny, GLint nz)
	{ RASWGL_Normal3f(GLCTX,
		nx/2147483647.0, ny/2147483647.0, nz/2147483647.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glNormal3s(
		GLshort nx, GLshort ny, GLshort nz)
	{ RASWGL_Normal3f(GLCTX, nx/32767.0, ny/32767.0, nz/32767.0); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glNormal3bv(const GLbyte *v)
	{ RASWGL_Normal3f(GLCTX, v[0]/127.0, v[1]/127.0, v[2]/127.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glNormal3dv(const GLdouble *v)
	{ RASWGL_Normal3f(GLCTX, v[0], v[1], v[2]); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glNormal3fv(const GLfloat *v)
	{ RASWGL_Normal3f(GLCTX, v[0], v[1], v[2]); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glNormal3iv(const GLint *v)
	{ RASWGL_Normal3f(GLCTX, 
		v[0]/2147483647.0, v[1]/2147483647.0, v[2]/2147483647.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glNormal3sv(const GLshort *v)
	{ RASWGL_Normal3f(GLCTX, v[0]/32767.0, v[1]/32767.0, v[2]/32767.0); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glIndexd(GLdouble c)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glIndexf(GLfloat c)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glIndexi(GLint c)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glIndexs(GLshort c)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glIndexub(GLubyte c)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glIndexdv(const GLdouble *c)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glIndexfv(const GLfloat *c)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glIndexiv(const GLint *c)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glIndexsv(const GLshort *c)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glIndexubv(const GLubyte *c)
	{ RaGlSetErrorNopStub(); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor3b(
		GLbyte red, GLbyte green, GLbyte blue)
	{ RASWGL_Color3f(GLCTX, red/127.0, green/127.0, blue/127.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor3d(
		GLdouble red, GLdouble green, GLdouble blue)
	{ RASWGL_Color3f(GLCTX, red, green, blue); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor3f(
		GLfloat red, GLfloat green, GLfloat blue)
	{ RASWGL_Color3f(GLCTX, red, green, blue); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor3i(
		GLint red, GLint green, GLint blue)
	{ RASWGL_Color3f(GLCTX, red/2147483647.0,
		green/2147483647.0, blue/2147483647.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor3s(
		GLshort red, GLshort green, GLshort blue)
	{ RASWGL_Color3f(GLCTX, red/32767.0, green/32767.0, blue/32767.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor3ub(
		GLubyte red, GLubyte green, GLubyte blue)
	{ RASWGL_Color3f(GLCTX, red/255.0, green/255.0, blue/255.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor3ui(
		GLuint red, GLuint green, GLuint blue)
	{ RASWGL_Color3f(GLCTX, red/4294967295.0,
		green/4294967295.0, blue/4294967295.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor3us(
		GLushort red, GLushort green, GLushort blue)
	{ RASWGL_Color3f(GLCTX, red/65535.0, green/65535.0, blue/65535.0); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor4b(
		GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha)
	{ RASWGL_Color4f(GLCTX, red/127.0, green/127.0,
		blue/127.0, alpha/127.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor4d(
		GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha)
	{ RASWGL_Color4f(GLCTX, red, green, blue, alpha); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor4f(
		GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
	{ RASWGL_Color4f(GLCTX, red, green, blue, alpha); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor4i(
		GLint red, GLint green, GLint blue, GLint alpha)
	{ RASWGL_Color4f(GLCTX, red/2147483647.0, green/2147483647.0,
		blue/2147483647.0, alpha/2147483647.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor4s(
		GLshort red, GLshort green, GLshort blue, GLshort alpha)
	{ RASWGL_Color4f(GLCTX, red/32767.0, green/32767.0,
		blue/32767.0, alpha/32767.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor4ub(
		GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
	{ RASWGL_Color4f(GLCTX, red/255.0, green/255.0,
		blue/255.0, alpha/255.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor4ui(
		GLuint red, GLuint green, GLuint blue, GLuint alpha)
	{ RASWGL_Color4f(GLCTX, red/4294967295.0, green/4294967295.0,
		blue/4294967295.0, alpha/4294967295.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor4us(
		GLushort red, GLushort green, GLushort blue, GLushort alpha)
	{ RASWGL_Color4f(GLCTX, red/65535.0, green/65535.0,
		blue/65535.0, alpha/65535.0); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor3bv(const GLbyte *v)
	{ RASWGL_Color3f(GLCTX, v[0]/127.0, v[1]/127.0, v[2]/127.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor3dv(const GLdouble *v)
	{ RASWGL_Color3f(GLCTX, v[0], v[1], v[2]); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor3fv(const GLfloat *v)
	{ RASWGL_Color3f(GLCTX, v[0], v[1], v[2]); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor3iv(const GLint *v)
	{ RASWGL_Color3f(GLCTX, v[0]/127.0, v[1]/127.0, v[2]/127.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor3sv(const GLshort *v)
	{ RASWGL_Color3f(GLCTX, v[0]/127.0, v[1]/127.0, v[2]/127.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor3ubv(const GLubyte *v)
	{ RASWGL_Color3f(GLCTX, v[0]/255.0, v[1]/255.0, v[2]/255.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor3uiv(const GLuint *v)
	{ RASWGL_Color3f(GLCTX, v[0]/127.0, v[1]/127.0, v[2]/127.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor3usv(const GLushort *v)
	{ RASWGL_Color3f(GLCTX, v[0]/127.0, v[1]/127.0, v[2]/127.0); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor4bv(const GLbyte *v)
	{ RASWGL_Color4f(GLCTX,
		v[0]/127.0, v[1]/127.0, v[2]/127.0, v[3]/127.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor4dv(const GLdouble *v)
	{ RASWGL_Color4f(GLCTX, v[0], v[1], v[2], v[3]); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor4fv(const GLfloat *v)
	{ RASWGL_Color4f(GLCTX, v[0], v[1], v[2], v[3]); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor4iv(const GLint *v)
	{ RASWGL_Color4f(GLCTX,
		v[0]/2147483647.0, v[1]/2147483647.0,
			v[2]/2147483647.0, v[3]/2147483647.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor4sv(const GLshort *v)
	{ RASWGL_Color4f(GLCTX,
		v[0]/32767.0, v[1]/32767.0, v[2]/32767.0, v[3]/32767.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor4ubv(const GLubyte *v)
	{ RASWGL_Color4f(GLCTX,
		v[0]/255.0, v[1]/255.0, v[2]/255.0, v[3]/255.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor4uiv(const GLuint *v)
	{ RASWGL_Color4f(GLCTX,
		v[0]/4294967295.0, v[1]/4294967295.0,
			v[2]/4294967295.0, v[3]/4294967295.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColor4usv(const GLushort *v)
	{ RASWGL_Color4f(GLCTX,
		v[0]/65535.0, v[1]/65535.0, v[2]/65535.0, v[3]/65535.0); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord1d(GLdouble s)
	{ RASWGL_TexCoord2f(GLCTX, s, 0.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord1f(GLfloat s)
	{ RASWGL_TexCoord2f(GLCTX, s, 0.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord1i(GLint s)
	{ RASWGL_TexCoord2f(GLCTX, s, 0.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord1s(GLshort s)
	{ RASWGL_TexCoord2f(GLCTX, s/32767.0, 0.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord2d(GLdouble s, GLdouble t)
	{ RASWGL_TexCoord2f(GLCTX, s, t); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord2f(GLfloat s, GLfloat t)
	{ RASWGL_TexCoord2f(GLCTX, s, t); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord2i(GLint s, GLint t)
	{ RASWGL_TexCoord2f(GLCTX, s/2147483647.0, t/2147483647.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord2s(GLshort s, GLshort t)
	{ RASWGL_TexCoord2f(GLCTX, s/32767.0, t/32767.0); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord3d(
		GLdouble s, GLdouble t, GLdouble r)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord3f(
		GLfloat s, GLfloat t, GLfloat r)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord3i(
		GLint s, GLint t, GLint r)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord3s(
		GLshort s, GLshort t, GLshort r)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord4d(
		GLdouble s, GLdouble t, GLdouble r, GLdouble q)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord4f(
		GLfloat s, GLfloat t, GLfloat r, GLfloat q)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord4i(
		GLint s, GLint t, GLint r, GLint q)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord4s(
		GLshort s, GLshort t, GLshort r, GLshort q)
	{ RaGlSetErrorNopStub(); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord1dv(const GLdouble *v)
	{ RASWGL_TexCoord2f(GLCTX, v[0], 0.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord1fv(const GLfloat *v)
	{ RASWGL_TexCoord2f(GLCTX, v[0], 0.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord1iv(const GLint *v)
	{ RASWGL_TexCoord2f(GLCTX, v[0]/2147483647.0, 0.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord1sv(const GLshort *v)
	{ RASWGL_TexCoord2f(GLCTX, v[0]/32767.0, 0.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord2dv(const GLdouble *v)
	{ RASWGL_TexCoord2f(GLCTX, v[0], v[1]); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord2fv(const GLfloat *v)
	{ RASWGL_TexCoord2f(GLCTX, v[0], v[1]); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord2iv(const GLint *v)
	{ RASWGL_TexCoord2f(GLCTX, v[0]/2147483647.0, v[1]/2147483647.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord2sv(const GLshort *v)
	{ RASWGL_TexCoord2f(GLCTX, v[0]/32767.0, v[1]/32767.0); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord3dv(const GLdouble *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord3fv(const GLfloat *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord3iv(const GLint *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord3sv(const GLshort *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord4dv(const GLdouble *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord4fv(const GLfloat *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord4iv(const GLint *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoord4sv(const GLshort *v)
	{ RaGlSetErrorNopStub(); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glRasterPos2d(
		GLdouble x, GLdouble y)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRasterPos2f(
		GLfloat x, GLfloat y)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRasterPos2i(
		GLint x, GLint y)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRasterPos2s(
		GLshort x, GLshort y)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRasterPos3d(
		GLdouble x, GLdouble y, GLdouble z)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRasterPos3f(
		GLfloat x, GLfloat y, GLfloat z)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRasterPos3i(
	GLint x, GLint y, GLint z)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRasterPos3s(
		GLshort x, GLshort y, GLshort z)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRasterPos4d(
		GLdouble x, GLdouble y, GLdouble z, GLdouble w)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRasterPos4f(
		GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRasterPos4i(
		GLint x, GLint y, GLint z, GLint w)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRasterPos4s(
		GLshort x, GLshort y, GLshort z, GLshort w)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRasterPos2dv(
		const GLdouble *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRasterPos2fv(const GLfloat *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRasterPos2iv(const GLint *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRasterPos2sv(const GLshort *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRasterPos3dv(const GLdouble *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRasterPos3fv(const GLfloat *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRasterPos3iv(const GLint *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRasterPos3sv(const GLshort *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRasterPos4dv(const GLdouble *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRasterPos4fv(const GLfloat *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRasterPos4iv(const GLint *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRasterPos4sv(const GLshort *v)
	{ RaGlSetErrorNopStub(); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glRectd(
		GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRectf(
		GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRecti(GLint x1, GLint y1, GLint x2, GLint y2)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRects(GLshort x1, GLshort y1, GLshort x2, GLshort y2)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRectdv(const GLdouble *v1, const GLdouble *v2)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRectfv(const GLfloat *v1, const GLfloat *v2)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRectiv(const GLint *v1, const GLint *v2)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glRectsv(const GLshort *v1, const GLshort *v2)
	{ RaGlSetErrorNopStub(); }

/* Lighting */

BGBRASW_API void GLDRV_APIENTRY GlDrv_glShadeModel(GLenum mode)
{
	GLCTX->shade_model=mode;
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glLightf(
	GLenum light, GLenum pname, GLfloat param)
{
	RASWGL_Lightf(GLCTX, light, pname, param);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glLighti(
	GLenum light, GLenum pname, GLint param)
{
	RASWGL_Lighti(GLCTX, light, pname, param);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glLightfv(
	GLenum light, GLenum pname, const GLfloat *params)
{
	RASWGL_Lightfv(GLCTX, light, pname, params);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glLightiv(
		GLenum light, GLenum pname, const GLint *params)
{
	RASWGL_Lightiv(GLCTX, light, pname, params);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetLightfv(
		GLenum light, GLenum pname, GLfloat *params)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetLightiv(
		GLenum light, GLenum pname, GLint *params)
	{ RaGlSetErrorNopStub(); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glLightModelf(
		GLenum pname, GLfloat param)
{
	RASWGL_LightModelf(GLCTX, pname, param);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glLightModeli(
		GLenum pname, GLint param)
{
	RASWGL_LightModeli(GLCTX, pname, param);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glLightModelfv(
		GLenum pname, const GLfloat *params)
{
	RASWGL_LightModelfv(GLCTX, pname, params);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glLightModeliv(
		GLenum pname, const GLint *params)
{
	RASWGL_LightModeliv(GLCTX, pname, params);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glMaterialf(
	GLenum face, GLenum pname, GLfloat param)
{
	RASWGL_Materialf(GLCTX, face, pname, param);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glMateriali(
	GLenum face, GLenum pname, GLint param)
{
	RASWGL_Materiali(GLCTX, face, pname, param);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glMaterialfv(
	GLenum face, GLenum pname, const GLfloat *params)
{
	RASWGL_Materialfv(GLCTX, face, pname, params);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glMaterialiv(
		GLenum face, GLenum pname, const GLint *params)
{
	RASWGL_Materialiv(GLCTX, face, pname, params);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetMaterialfv(
		GLenum face, GLenum pname, GLfloat *params)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetMaterialiv(
		GLenum face, GLenum pname, GLint *params)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColorMaterial(
		GLenum face, GLenum mode)
	{ RaGlSetErrorNopStub(); }

/* Raster functions */
BGBRASW_API void GLDRV_APIENTRY GlDrv_glPixelZoom(
		GLfloat xfactor, GLfloat yfactor)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glPixelStoref(
		GLenum pname, GLfloat param)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glPixelStorei(
		GLenum pname, GLint param)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glPixelTransferf(
		GLenum pname, GLfloat param)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glPixelTransferi(
		GLenum pname, GLint param)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glPixelMapfv(
		GLenum map, GLint mapsize, const GLfloat *values)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glPixelMapuiv(
		GLenum map, GLint mapsize, const GLuint *values)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glPixelMapusv(
		GLenum map, GLint mapsize, const GLushort *values)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetPixelMapfv(
		GLenum map, GLfloat *values)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetPixelMapuiv(
		GLenum map, GLuint *values)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetPixelMapusv(
		GLenum map, GLushort *values)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glBitmap(
	GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig,
	GLfloat xmove, GLfloat ymove, const GLubyte *bitmap)
	{ RaGlSetErrorNopStub(); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glReadPixels(
		GLint x, GLint y, GLsizei width, GLsizei height,
		GLenum format, GLenum type, GLvoid *pixels)
{
	RASWGL_ReadPixels(GLCTX, x, y, width, height,
		format, type, pixels);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glDrawPixels(
		GLsizei width, GLsizei height, GLenum format, GLenum type,
		const GLvoid *pixels)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glCopyPixels(
		GLint x, GLint y, GLsizei width, GLsizei height, GLenum type)
	{ RaGlSetErrorNopStub(); }

/* Stenciling */
BGBRASW_API void GLDRV_APIENTRY GlDrv_glStencilFunc(
		GLenum func, GLint ref, GLuint mask)
{
	RASWGL_StencilFunc(GLCTX, func, ref, mask);
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glStencilMask(GLuint mask)
{
	RASWGL_StencilMask(GLCTX, mask);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glStencilOp(
		GLenum fail, GLenum zfail, GLenum zpass)
{
	RASWGL_StencilOp(GLCTX, fail, zfail, zpass);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glClearStencil(GLint s)
	{ RaGlSetErrorNopStub(); }

/* Texture mapping */
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexGend(
		GLenum coord, GLenum pname, GLdouble param)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexGenf(
		GLenum coord, GLenum pname, GLfloat param)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexGeni(
		GLenum coord, GLenum pname, GLint param)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexGendv(
		GLenum coord, GLenum pname, const GLdouble *params)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexGenfv(
		GLenum coord, GLenum pname, const GLfloat *params)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexGeniv(
		GLenum coord, GLenum pname, const GLint *params)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetTexGendv(
		GLenum coord, GLenum pname, GLdouble *params)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetTexGenfv(
		GLenum coord, GLenum pname, GLfloat *params)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetTexGeniv(
		GLenum coord, GLenum pname, GLint *params)
	{ RaGlSetErrorNopStub(); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexEnvf(
		GLenum target, GLenum pname, GLfloat param)
	{ RASWGL_TexEnvf(GLCTX, target, pname, param); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexEnvi(
		GLenum target, GLenum pname, GLint param)
	{ RASWGL_TexEnvi(GLCTX, target, pname, param); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexEnvfv(
		GLenum target, GLenum pname, const GLfloat *params)
	{ RASWGL_TexEnvfv(GLCTX, target, pname, params); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexEnviv(
		GLenum target, GLenum pname, const GLint *params)
	{ RASWGL_TexEnviv(GLCTX, target, pname, params); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetTexEnvfv(
		GLenum target, GLenum pname, GLfloat *params)
	{ RASWGL_GetTexEnvfv(GLCTX, target, pname, params); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetTexEnviv(
		GLenum target, GLenum pname, GLint *params)
	{ RASWGL_GetTexEnviv(GLCTX, target, pname, params); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexParameterf(
		GLenum target, GLenum pname, GLfloat param)
	{ RASWGL_TexParameterf(GLCTX, target, pname, param); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexParameteri(
		GLenum target, GLenum pname, GLint param)
	{ RASWGL_TexParameteri(GLCTX, target, pname, param); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexParameterfv(
		GLenum target, GLenum pname, const GLfloat *params)
	{ RASWGL_TexParameterfv(GLCTX, target, pname, params); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexParameteriv(
		GLenum target, GLenum pname, const GLint *params)
	{ RASWGL_TexParameteriv(GLCTX, target, pname, params); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetTexParameterfv(
		GLenum target, GLenum pname, GLfloat *params)
	{ RASWGL_GetTexParameterfv(GLCTX, target, pname, params); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetTexParameteriv(
		GLenum target, GLenum pname, GLint *params)
	{ RASWGL_GetTexParameteriv(GLCTX, target, pname, params); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetTexLevelParameterfv(
		GLenum target, GLint level, GLenum pname, GLfloat *params)
	{ RASWGL_GetTexLevelParameterfv(GLCTX, target, level, pname, params); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetTexLevelParameteriv(
		GLenum target, GLint level, GLenum pname, GLint *params)
	{ RASWGL_GetTexLevelParameteriv(GLCTX, target, level, pname, params); }

BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexImage1D(
		GLenum target, GLint level, GLint internalFormat,
		GLsizei width, GLint border, GLenum format, GLenum type,
		const GLvoid *pixels)
{
	RASWGL_TexImage1D(GLCTX,
		target, level, internalFormat,
		width, border, format, type,
		pixels);
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexImage2D(
		GLenum target, GLint level, GLint internalFormat,
		GLsizei width, GLsizei height, GLint border, GLenum format,
		GLenum type, const GLvoid *pixels)
{
	RASWGL_TexImage2D(GLCTX,
		target, level, internalFormat,
		width, height, border, format, type,
		pixels);
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetTexImage(
		GLenum target, GLint level, GLenum format, GLenum type,
		GLvoid *pixels)
{
	RASWGL_GetTexImage(GLCTX,
		target, level, format, type, pixels);
}

/* Evaluators */
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMap1d(
		GLenum target, GLdouble u1, GLdouble u2, GLint stride,
		GLint order, const GLdouble *points)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMap1f(
		GLenum target, GLfloat u1, GLfloat u2, GLint stride,
		GLint order, const GLfloat *points)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMap2d(
		GLenum target, GLdouble u1, GLdouble u2, GLint ustride,
		GLint uorder, GLdouble v1, GLdouble v2, GLint vstride,
		GLint vorder, const GLdouble *points)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMap2f(
		GLenum target, GLfloat u1, GLfloat u2, GLint ustride,
		GLint uorder, GLfloat v1, GLfloat v2, GLint vstride,
		GLint vorder, const GLfloat *points)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetMapdv(
		GLenum target, GLenum query, GLdouble *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetMapfv(
		GLenum target, GLenum query, GLfloat *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetMapiv(
		GLenum target, GLenum query, GLint *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glEvalCoord1d(GLdouble u)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glEvalCoord1f(GLfloat u)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glEvalCoord1dv(const GLdouble *u)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glEvalCoord1fv(const GLfloat *u)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glEvalCoord2d(GLdouble u, GLdouble v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glEvalCoord2f(GLfloat u, GLfloat v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glEvalCoord2dv(const GLdouble *u)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glEvalCoord2fv(const GLfloat *u)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMapGrid1d(
		GLint un, GLdouble u1, GLdouble u2)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMapGrid1f(
		GLint un, GLfloat u1, GLfloat u2)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMapGrid2d(
		GLint un, GLdouble u1, GLdouble u2, GLint vn,
		GLdouble v1, GLdouble v2)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMapGrid2f(
		GLint un, GLfloat u1, GLfloat u2, GLint vn,
		GLfloat v1, GLfloat v2)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glEvalPoint1(
		GLint i)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glEvalPoint2(
		GLint i, GLint j)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glEvalMesh1(
		GLenum mode, GLint i1, GLint i2)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glEvalMesh2(
		GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2)
	{ RaGlSetErrorNopStub(); }

/* Fog */
BGBRASW_API void GLDRV_APIENTRY GlDrv_glFogf(
		GLenum pname, GLfloat param)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glFogi(
		GLenum pname, GLint param)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glFogfv(
		GLenum pname, const GLfloat *params)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glFogiv(
		GLenum pname, const GLint *params)
	{ RaGlSetErrorNopStub(); }

/* Selection and Feedback */
BGBRASW_API void GLDRV_APIENTRY GlDrv_glFeedbackBuffer(
		GLsizei size, GLenum type, GLfloat *buffer)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glPassThrough(
		GLfloat token)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glSelectBuffer(
		GLsizei size, GLuint *buffer)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glInitNames(void)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glLoadName(GLuint name)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glPushName(GLuint name)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glPopName(void)
	{ RaGlSetErrorNopStub(); }


/* 1.1 functions */

/* texture objects */
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGenTextures(
	GLsizei n, GLuint *textures)
{
	RASWGL_GenTextures(GLCTX, n, textures);
//	RaGlSetErrorNopStub();
}
BGBRASW_API void GLDRV_APIENTRY GlDrv_glDeleteTextures(
	GLsizei n, const GLuint *textures)
{
	RASWGL_DeleteTextures(GLCTX, n, (int *)textures);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glBindTexture(
	GLenum target, GLuint texture)
{
	RASWGL_BindTexture(GLCTX, target, texture);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glPrioritizeTextures(
		GLsizei n, const GLuint *textures, const GLclampf *priorities)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API GLboolean GLDRV_APIENTRY GlDrv_glAreTexturesResident(
		GLsizei n, const GLuint *textures, GLboolean *residences)
	{ RaGlSetErrorNopStub(); return(0); }
BGBRASW_API GLboolean GLDRV_APIENTRY GlDrv_glIsTexture(GLuint texture)
	{ RaGlSetErrorNopStub(); return(0); }

/* texture mapping */
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexSubImage1D(
		GLenum target, GLint level, GLint xoffset, GLsizei width,
		GLenum format, GLenum type, const GLvoid *pixels)
{
	RASWGL_TexSubImage1D(GLCTX, target, level, xoffset, width,
		format, type, pixels);
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexSubImage2D(
		GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLsizei width, GLsizei height, GLenum format, GLenum type,
		const GLvoid *pixels)
{
	RASWGL_TexSubImage2D(GLCTX, target, level,
		xoffset, yoffset, width, height,
		format, type, pixels);
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glCopyTexImage1D(
		GLenum target, GLint level, GLenum internalformat,
		GLint x, GLint y, GLsizei width, GLint border)
{
	RASWGL_CopyTexImage1D(GLCTX,
		target, level, internalformat,
		x, y, width, border);
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glCopyTexImage2D(
		GLenum target, GLint level, GLenum internalformat,
		GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
	RASWGL_CopyTexImage2D(GLCTX,
		target, level, internalformat,
		x, y, width, height, border);
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glCopyTexSubImage1D(
		GLenum target, GLint level, GLint xoffset, GLint x, GLint y,
		GLsizei width)
{
	RASWGL_CopyTexSubImage1D(GLCTX,
		target, level, xoffset, x, y, width);
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glCopyTexSubImage2D(
		GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLint x, GLint y, GLsizei width, GLsizei height)
{
	RASWGL_CopyTexSubImage2D(GLCTX,
		target, level, xoffset, yoffset,
		x, y, width, height);
}

/* vertex arrays */
BGBRASW_API void GLDRV_APIENTRY GlDrv_glVertexPointer(
		GLint size, GLenum type, GLsizei stride, const GLvoid *ptr)
{
	RASWGL_VertexPointer(GLCTX, size, type, stride, ptr);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glNormalPointer(
		GLenum type, GLsizei stride, const GLvoid *ptr)
{
	RASWGL_NormalPointer(GLCTX, type, stride, ptr);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glColorPointer(
		GLint size, GLenum type, GLsizei stride, const GLvoid *ptr)
{
	RASWGL_ColorPointer(GLCTX, size, type, stride, ptr);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glIndexPointer(
		GLenum type, GLsizei stride, const GLvoid *ptr)
{
	RASWGL_IndexPointer(GLCTX, type, stride, ptr);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexCoordPointer(
		GLint size, GLenum type, GLsizei stride, const GLvoid *ptr)
{
	RASWGL_TexCoordPointer(GLCTX, size, type, stride, ptr);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glEdgeFlagPointer(
		GLsizei stride, const GLvoid *ptr)
{
	RASWGL_EdgeFlagPointer(GLCTX, stride, ptr);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetPointerv(
		GLenum pname, GLvoid **params)
{
	RASWGL_GetPointerv(GLCTX, pname, params);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glArrayElement(GLint i)
{
	RASWGL_ArrayElement(GLCTX, i);
//	RaGlSetErrorNopStub();
}
BGBRASW_API void GLDRV_APIENTRY GlDrv_glDrawArrays(
		GLenum mode, GLint first, GLsizei count)
{
	RASWGL_DrawArrays(GLCTX, mode, first, count);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glDrawElements(
		GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
	RASWGL_DrawElements(GLCTX, mode, count, type, indices);
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glInterleavedArrays(
		GLenum format, GLsizei stride, const GLvoid *pointer)
{
	RASWGL_InterleavedArrays(GLCTX, format, stride, pointer);
//	RaGlSetErrorNopStub();
}


/* 1.2 functions */
BGBRASW_API void GLDRV_APIENTRY GlDrv_glDrawRangeElements(
		GLenum mode, GLuint start, GLuint end, GLsizei count,	
		GLenum type, const GLvoid *indices)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexImage3D(
		GLenum target, GLint level, GLenum internalFormat,
		GLsizei width, GLsizei height, GLsizei depth, GLint border,
		GLenum format, GLenum type, const GLvoid *pixels)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glTexSubImage3D(
		GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,
		GLenum format, GLenum type, const GLvoid *pixels)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glCopyTexSubImage3D(
		GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
	{ RaGlSetErrorNopStub(); }

/* 1.2 imaging extension functions */
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColorTable(
		GLenum target, GLenum internalformat, GLsizei width,
		GLenum format, GLenum type, const GLvoid *table)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColorSubTable(
		GLenum target, GLsizei start, GLsizei count, GLenum format,
		GLenum type, const GLvoid *data)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColorTableParameteriv(
		GLenum target, GLenum pname, const GLint *params)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glColorTableParameterfv(
		GLenum target, GLenum pname, const GLfloat *params)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glCopyColorSubTable(
		GLenum target, GLsizei start, GLint x, GLint y, GLsizei width)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glCopyColorTable(
		GLenum target, GLenum internalformat, GLint x, GLint y,
		GLsizei width)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetColorTable(
		GLenum target, GLenum format, GLenum type, GLvoid *table)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetColorTableParameterfv(
		GLenum target, GLenum pname, GLfloat *params)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetColorTableParameteriv(
		GLenum target, GLenum pname, GLint *params)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glBlendEquation(GLenum mode)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glBlendColor(
		GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glHistogram(
		GLenum target, GLsizei width, GLenum internalformat, GLboolean sink)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glResetHistogram(GLenum target)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetHistogram(
		GLenum target, GLboolean reset, GLenum format,
		GLenum type, GLvoid *values)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetHistogramParameterfv(
		GLenum target, GLenum pname, GLfloat *params)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetHistogramParameteriv(
		GLenum target, GLenum pname, GLint *params)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMinmax(
		GLenum target, GLenum internalformat, GLboolean sink)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glResetMinmax(GLenum target)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetMinmax(
		GLenum target, GLboolean reset, GLenum format,
		GLenum types, GLvoid *values)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetMinmaxParameterfv(
		GLenum target, GLenum pname, GLfloat *params)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetMinmaxParameteriv(
		GLenum target, GLenum pname, GLint *params)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glConvolutionFilter1D(
		GLenum target, GLenum internalformat, GLsizei width,
		GLenum format, GLenum type, const GLvoid *image)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glConvolutionFilter2D(
		GLenum target, GLenum internalformat,
		GLsizei width, GLsizei height, GLenum format,
		GLenum type, const GLvoid *image)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glConvolutionParameterf(
		GLenum target, GLenum pname, GLfloat params)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glConvolutionParameterfv(
		GLenum target, GLenum pname, const GLfloat *params)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glConvolutionParameteri(
		GLenum target, GLenum pname, GLint params)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glConvolutionParameteriv(
		GLenum target, GLenum pname, const GLint *params)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glCopyConvolutionFilter1D(
		GLenum target, GLenum internalformat, GLint x, GLint y,
		GLsizei width)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glCopyConvolutionFilter2D(
		GLenum target, GLenum internalformat, GLint x, GLint y,
		GLsizei width, GLsizei height)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetConvolutionFilter(
		GLenum target, GLenum format, GLenum type, GLvoid *image)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetConvolutionParameterfv(
		GLenum target, GLenum pname, GLfloat *params)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetConvolutionParameteriv(
		GLenum target, GLenum pname, GLint *params)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glSeparableFilter2D(
		GLenum target, GLenum internalformat,
		GLsizei width, GLsizei height, GLenum format,
		GLenum type, const GLvoid *row, const GLvoid *column)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetSeparableFilter(
		GLenum target, GLenum format, GLenum type,
		GLvoid *row, GLvoid *column, GLvoid *span)
	{ RaGlSetErrorNopStub(); }

/* 1.3 functions */
BGBRASW_API void GLDRV_APIENTRY GlDrv_glActiveTexture(
		GLenum texture)
{
//	RaGlSetErrorNopStub();
}

BGBRASW_API void GLDRV_APIENTRY GlDrv_glClientActiveTexture(
		GLenum texture)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glCompressedTexImage1D(
		GLenum target, GLint level, GLenum internalformat,
		GLsizei width, GLint border, GLsizei imageSize,
		const GLvoid *data)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glCompressedTexImage2D(
		GLenum target, GLint level, GLenum internalformat,
		GLsizei width, GLsizei height, GLint border,
		GLsizei imageSize, const GLvoid *data)
{
	RASWGL_CompressedTexImage2D(GLCTX,
		target, level, internalformat,
		width, height, border, imageSize,
		data);

//	RaGlSetErrorNopStub();
}
BGBRASW_API void GLDRV_APIENTRY GlDrv_glCompressedTexImage3D(
		GLenum target, GLint level, GLenum internalformat,
		GLsizei width, GLsizei height, GLsizei depth, GLint border,
		GLsizei imageSize, const GLvoid *data)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glCompressedTexSubImage1D(
		GLenum target, GLint level, GLint xoffset, GLsizei width,
		GLenum format, GLsizei imageSize, const GLvoid *data)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glCompressedTexSubImage2D(
		GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLsizei width, GLsizei height, GLenum format,
		GLsizei imageSize, const GLvoid *data)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glCompressedTexSubImage3D(
		GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,
		GLenum format, GLsizei imageSize, const GLvoid *data)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glGetCompressedTexImage(
		GLenum target, GLint lod, GLvoid *img)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord1d(
		GLenum target, GLdouble s)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord1dv(
		GLenum target, const GLdouble *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord1f(
		GLenum target, GLfloat s)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord1fv(
		GLenum target, const GLfloat *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord1i(
		GLenum target, GLint s)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord1iv(
		GLenum target, const GLint *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord1s(
		GLenum target, GLshort s)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord1sv(
		GLenum target, const GLshort *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord2d(
		GLenum target, GLdouble s, GLdouble t)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord2dv(
		GLenum target, const GLdouble *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord2f(
		GLenum target, GLfloat s, GLfloat t)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord2fv(
		GLenum target, const GLfloat *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord2i(
		GLenum target, GLint s, GLint t)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord2iv(
		GLenum target, const GLint *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord2s(
		GLenum target, GLshort s, GLshort t)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord2sv(
		GLenum target, const GLshort *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord3d(
		GLenum target, GLdouble s, GLdouble t, GLdouble r)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord3dv(
		GLenum target, const GLdouble *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord3f(
		GLenum target, GLfloat s, GLfloat t, GLfloat r)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord3fv(
		GLenum target, const GLfloat *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord3i(
		GLenum target, GLint s, GLint t, GLint r)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord3iv(
		GLenum target, const GLint *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord3s(
		GLenum target, GLshort s, GLshort t, GLshort r)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord3sv(
		GLenum target, const GLshort *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord4d(
		GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord4dv(
		GLenum target, const GLdouble *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord4f(
		GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord4fv(
		GLenum target, const GLfloat *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord4i(
		GLenum target, GLint s, GLint t, GLint r, GLint q)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord4iv(
		GLenum target, const GLint *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord4s(
		GLenum target, GLshort s, GLshort t, GLshort r, GLshort q)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultiTexCoord4sv(
		GLenum target, const GLshort *v)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glLoadTransposeMatrixd(
		const GLdouble m[16])
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glLoadTransposeMatrixf(
		const GLfloat m[16])
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultTransposeMatrixd(
		const GLdouble m[16])
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glMultTransposeMatrixf(
		const GLfloat m[16])
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glSampleCoverage(
		GLclampf value, GLboolean invert)
	{ RaGlSetErrorNopStub(); }
BGBRASW_API void GLDRV_APIENTRY GlDrv_glSamplePass(
		GLenum pass)
	{ RaGlSetErrorNopStub(); }
