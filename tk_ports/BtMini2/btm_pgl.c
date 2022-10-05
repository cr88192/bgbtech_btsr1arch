#include <GL/gl.h>

#ifdef _WIN32

// #define PGL_NATIVEGL
// #define BTM_RAYTHREAD

#endif



void (APIENTRY *pglAlphaFunc)(GLenum func, GLclampf ref);
void (APIENTRY *pglBindTexture)(GLenum target, GLuint texture);
void (APIENTRY *pglBlendFunc)(GLenum sfactor, GLenum dfactor);
void (APIENTRY *pglClear)(GLbitfield mask);
void (APIENTRY *pglClearColor)(
	GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void (APIENTRY *pglColor4f)(
	GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void (APIENTRY *pglColorMask)(
	GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
void (APIENTRY *pglColorPointer)(
	GLint size, GLenum type, GLsizei stride, const GLvoid *ptr);
void (APIENTRY *pglCopyTexImage2D)(
	GLenum target, GLint level, GLenum internalformat,
	GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void (APIENTRY *pglCullFace)(GLenum mode);
void (APIENTRY *pglDeleteTextures)(GLsizei n, const GLuint *textures);
void (APIENTRY *pglDepthFunc)(GLenum func);
void (APIENTRY *pglDisable)(GLenum cap);
void (APIENTRY *pglDisableClientState)(GLenum cap);
void (APIENTRY *pglDrawArrays)(GLenum mode, GLint first, GLsizei count);
void (APIENTRY *pglDrawElements)(
	GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
void (APIENTRY *pglEnable)(GLenum cap);
void (APIENTRY *pglEnableClientState)(GLenum cap);
void (APIENTRY *pglFinish)(void);
void (APIENTRY *pglFlush)(void);
void (APIENTRY *pglFrontFace)(GLenum mode);
void (APIENTRY *pglGenTextures)(GLsizei n, GLuint *textures);
GLenum (APIENTRY *pglGetError)(void);
void (APIENTRY *pglGetIntegerv)(GLenum pname, GLint *params);
void (APIENTRY *pglHint)(GLenum target, GLenum mode);
const GLubyte* (APIENTRY *pglGetString)(GLenum name);
void (APIENTRY *pglLoadIdentity)(void);
void (APIENTRY *pglMatrixMode)(GLenum mode);
void (APIENTRY *pglMultMatrixf)(const GLfloat *m);
void (APIENTRY *pglNormalPointer)(
	GLenum type, GLsizei stride, const GLvoid *ptr);
void (APIENTRY *pglPolygonMode)(GLenum face, GLenum mode);
void (APIENTRY *pglReadPixels)(
	GLint x, GLint y, GLsizei width, GLsizei height,
	GLenum format, GLenum type, GLvoid *pixels);
void (APIENTRY *pglShadeModel)(GLenum mode);
void (APIENTRY *pglTexCoordPointer)(
	GLint size, GLenum type, GLsizei stride, const GLvoid *ptr);
void (APIENTRY *pglTexEnvf)(
	GLenum target, GLenum pname, GLfloat param);
void (APIENTRY *pglTexEnvi)(
	GLenum target, GLenum pname, GLint param);
void (APIENTRY *pglTexImage2D)(
	GLenum target, GLint level, GLint internalFormat,
	GLsizei width, GLsizei height, GLint border, GLenum format,
	GLenum type, const GLvoid *pixels);
void (APIENTRY *pglTexParameterf)(
	GLenum target, GLenum pname, GLfloat param);
void (APIENTRY *pglTexParameteri)(
	GLenum target, GLenum pname, GLint param);
void (APIENTRY *pglVertexPointer)(
	GLint size, GLenum type, GLsizei stride, const GLvoid *ptr);
void (APIENTRY *pglViewport)(
	GLint x, GLint y, GLsizei width, GLsizei height);

// void (*pglMatrixMode)(int mode);
void (APIENTRY *pglPushMatrix)(void);
void (APIENTRY *pglPopMatrix)(void);
void (APIENTRY *pglLoadIdentity)(void);
void (APIENTRY *pglRotatef)(float angle, float x, float y, float z);
void (APIENTRY *pglScalef)(float x, float y, float z);
void (APIENTRY *pglTranslatef)(float x, float y, float z);

void (APIENTRY *pglOrtho)(
	double left,		double right,
	double bottom,		double top,
	double nearval,	double farval);

void (APIENTRY *pglFrustum)(
	double left,		double right,
	double bottom,		double top,
	double nearval,	double farval);

void (APIENTRY *pglClearDepth)(double depth);
void (APIENTRY *pglDepthRange)(double near_val, double far_val);

void (APIENTRY *pglBegin)(int mode);
void (APIENTRY *pglEnd)(void);

void (APIENTRY *pglColor3f)(float red, float green, float blue);
void (APIENTRY *pglColor4f)(float red, float green, float blue, float alpha);

void (APIENTRY *pglColor4ubv)(const byte *v);

void (APIENTRY *pglTexCoord2f)(float s, float t);

void (APIENTRY *pglVertex2f)(float x, float y);
void (APIENTRY *pglVertex3f)(float x, float y, float z);

void (APIENTRY *pglTexCoord2fv)(const float *v);
void (APIENTRY *pglVertex3fv)(const float *v);

void (APIENTRY *pglCompressedTexImage2D)(int target,
	int level, int internalformat,
	int width, int height, int border,
	int imageSize, void * data);


#ifdef PGL_NATIVEGL

HMODULE frgl_opengl32_dll=NULL;

void *pglGetProcAddress(char *name)
{
	void *ptr;
	
	ptr=wglGetProcAddress(name);
	if(ptr)
		return(ptr);

	if(!frgl_opengl32_dll)
	{
		frgl_opengl32_dll=LoadLibrary("opengl32");
	}

	ptr=GetProcAddress(frgl_opengl32_dll, name);
	if(ptr)return(ptr);

	return(ptr);
}

#else

void *pglGetProcAddress(char *name)
{
	return(tkra_wglGetProcAddress(name));
}

#endif

int BTM_PGL_InitOpenGlFuncs()
{
	if(pglAlphaFunc)
		return(0);

	pglAlphaFunc			=pglGetProcAddress("glAlphaFunc");
	pglBindTexture			=pglGetProcAddress("glBindTexture");
	pglBlendFunc			=pglGetProcAddress("glBlendFunc");
	pglClear				=pglGetProcAddress("glClear");
	pglClearColor			=pglGetProcAddress("glClearColor");
	pglColor4f				=pglGetProcAddress("glColor4f");
	pglColorMask			=pglGetProcAddress("glColorMask");
	pglColorPointer			=pglGetProcAddress("glColorPointer");
	pglCopyTexImage2D		=pglGetProcAddress("glCopyTexImage2D");
	pglCullFace				=pglGetProcAddress("glCullFace");
	pglDeleteTextures		=pglGetProcAddress("glDeleteTextures");
	pglDepthFunc			=pglGetProcAddress("glDepthFunc");
	pglDisable				=pglGetProcAddress("glDisable");
	pglDisableClientState	=pglGetProcAddress("glDisableClientState");
	pglDrawArrays			=pglGetProcAddress("glDrawArrays");
	pglDrawElements			=pglGetProcAddress("glDrawElements");
	pglEnable				=pglGetProcAddress("glEnable");
	pglEnableClientState	=pglGetProcAddress("glEnableClientState");
	pglFinish				=pglGetProcAddress("glFinish");
	pglFlush				=pglGetProcAddress("glFlush");
	pglFrontFace			=pglGetProcAddress("glFrontFace");

	pglGenTextures			=pglGetProcAddress("glGenTextures");

	pglGetError				=pglGetProcAddress("glGetError");
	pglGetIntegerv			=pglGetProcAddress("glGetIntegerv");
	pglGetString			=pglGetProcAddress("glGetString");
	pglHint					=pglGetProcAddress("glHint");
	pglLoadIdentity			=pglGetProcAddress("glLoadIdentity");
	pglMatrixMode			=pglGetProcAddress("glMatrixMode");
	pglMultMatrixf			=pglGetProcAddress("glMultMatrixf");
	pglNormalPointer		=pglGetProcAddress("glNormalPointer");
	pglPolygonMode			=pglGetProcAddress("glPolygonMode");
	pglReadPixels			=pglGetProcAddress("glReadPixels");
	pglShadeModel			=pglGetProcAddress("glShadeModel");
	pglTexCoordPointer		=pglGetProcAddress("glTexCoordPointer");
	pglTexEnvf				=pglGetProcAddress("glTexEnvf");
	pglTexEnvi				=pglGetProcAddress("glTexEnvi");
	pglTexImage2D			=pglGetProcAddress("glTexImage2D");
	pglTexParameterf		=pglGetProcAddress("glTexParameterf");
	pglTexParameteri		=pglGetProcAddress("glTexParameteri");
	pglVertexPointer		=pglGetProcAddress("glVertexPointer");
	pglViewport				=pglGetProcAddress("glViewport");

	pglPushMatrix=pglGetProcAddress("glPushMatrix");
	pglPopMatrix=pglGetProcAddress("glPopMatrix");
	pglLoadIdentity=pglGetProcAddress("glLoadIdentity");
	pglRotatef=pglGetProcAddress("glRotatef");
	pglScalef=pglGetProcAddress("glScalef");
	pglTranslatef=pglGetProcAddress("glTranslatef");

	pglOrtho=pglGetProcAddress("glOrtho");
	pglFrustum=pglGetProcAddress("glFrustum");

	pglClearDepth=pglGetProcAddress("glClearDepth");
	pglDepthRange=pglGetProcAddress("glDepthRange");

	pglBegin=pglGetProcAddress("glBegin");
	pglEnd=pglGetProcAddress("glEnd");
	pglColor3f=pglGetProcAddress("glColor3f");
	pglColor4f=pglGetProcAddress("glColor4f");
	pglColor4ubv=pglGetProcAddress("glColor4ubv");
	pglTexCoord2f=pglGetProcAddress("glTexCoord2f");
	pglTexCoord2fv=pglGetProcAddress("glTexCoord2fv");
	pglVertex2f=pglGetProcAddress("glVertex2f");
	pglVertex3f=pglGetProcAddress("glVertex3f");
	pglVertex3fv=pglGetProcAddress("glVertex3fv");

	pglCompressedTexImage2D=pglGetProcAddress("glCompressedTexImage2D");

	return(1);
}
