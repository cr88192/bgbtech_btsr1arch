#include <bgbrasw_glstub.h>

BGBRASW_GlStub_Funcs *SglFunc(void);

GLAPI void APIENTRY glPolygonOffsetEXT(GLfloat factor, GLfloat units)
	{ (SglFunc())->glPolygonOffset(factor, units); }


GLAPI void APIENTRY glBindTextureEXT(GLenum target, GLuint texture)
	{ (SglFunc())->glBindTexture(target, texture); }

GLAPI void APIENTRY glDeleteTexturesEXT(GLsizei n, const GLuint *textures)
	{ (SglFunc())->glDeleteTextures(n, textures); }

GLAPI void APIENTRY glGenTexturesEXT(GLsizei n, GLuint *textures)
	{ (SglFunc())->glGenTextures(n, textures); }

GLAPI void APIENTRY glPrioritizeTexturesEXT(
		GLsizei n, const GLuint *textures, const GLclampf *priorities)
	{ (SglFunc())->glPrioritizeTextures(n, textures, priorities); }


GLAPI void APIENTRY glArrayElementEXT(GLint i)
	{ (SglFunc())->glArrayElement(i); }

GLAPI void APIENTRY glVertexPointerEXT(
		GLint size, GLenum type, GLsizei stride, const GLvoid *ptr)
	{ (SglFunc())->glVertexPointer(size, type, stride, ptr); }

GLAPI void APIENTRY glNormalPointerEXT(
		GLenum type, GLsizei stride, const GLvoid *ptr)
	{ (SglFunc())->glNormalPointer(type, stride, ptr); }

GLAPI void APIENTRY glColorPointerEXT(
		GLint size, GLenum type, GLsizei stride, const GLvoid *ptr)
	{ (SglFunc())->glColorPointer(size, type, stride, ptr); }

GLAPI void APIENTRY glIndexPointerEXT(
		GLenum type, GLsizei stride, const GLvoid *ptr)
	{ (SglFunc())->glIndexPointer(type, stride, ptr); }

GLAPI void APIENTRY glTexCoordPointerEXT(
		GLint size, GLenum type, GLsizei stride, const GLvoid *ptr)
	{ (SglFunc())->glTexCoordPointer(size, type, stride, ptr); }

GLAPI void APIENTRY glGetPointervEXT(GLenum pname, GLvoid **params)
	{ (SglFunc())->glGetPointerv(pname, params); }

GLAPI void APIENTRY glEdgeFlagPointerEXT(GLsizei stride, const GLvoid *ptr)
	{ (SglFunc())->glEdgeFlagPointer(stride, ptr); }

GLAPI void APIENTRY glDrawArraysEXT(GLenum mode, GLint first, GLsizei count)
	{ (SglFunc())->glDrawArrays(mode, first, count); }


#if 1

/* 1.2 functions */
GLAPI void APIENTRY glDrawRangeElementsEXT(
		GLenum mode, GLuint start, GLuint end, GLsizei count,
		GLenum type, const GLvoid *indices)
	{ (SglFunc())->glDrawRangeElements(
		mode, start, end, count, type, indices); }
GLAPI void APIENTRY glTexImage3DEXT(
		GLenum target, GLint level, GLenum internalFormat,
		GLsizei width, GLsizei height, GLsizei depth, GLint border,
		GLenum format, GLenum type, const GLvoid *pixels)
	{ (SglFunc())->glTexImage3D(target, level, internalFormat,
		width, height, depth, border, format, type, pixels); }
GLAPI void APIENTRY glTexSubImage3DEXT(
		GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,
		GLenum format, GLenum type, const GLvoid *pixels)
	{ (SglFunc())->glTexSubImage3D(target, level,
		xoffset, yoffset, zoffset,
		width, height, depth, format, type, pixels); }
GLAPI void APIENTRY glCopyTexSubImage3DEXT(
		GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
	{ (SglFunc())->glCopyTexSubImage3D(target, level,
		xoffset, yoffset, zoffset, x, y,
		width, height); }

/* 1.2 imaging extension functions */
GLAPI void APIENTRY glColorTableEXT(
		GLenum target, GLenum internalformat, GLsizei width,
		GLenum format, GLenum type, const GLvoid *table)
	{ (SglFunc())->glColorTable(target, internalformat,
		width, format, type, table); }
GLAPI void APIENTRY glColorSubTableEXT(
		GLenum target, GLsizei start, GLsizei count, GLenum format,
		GLenum type, const GLvoid *data)
	{ (SglFunc())->glColorSubTable(target, start, count, format,
		type, data); }
GLAPI void APIENTRY glColorTableParameterivEXT(
		GLenum target, GLenum pname, const GLint *params)
	{ (SglFunc())->glColorTableParameteriv(target, pname, params); }
GLAPI void APIENTRY glColorTableParameterfvEXT(
		GLenum target, GLenum pname, const GLfloat *params)
	{ (SglFunc())->glColorTableParameterfv(target, pname, params); }
GLAPI void APIENTRY glCopyColorSubTableEXT(
		GLenum target, GLsizei start, GLint x, GLint y, GLsizei width)
	{ (SglFunc())->glCopyColorSubTable(target, start, x, y, width); }
GLAPI void APIENTRY glCopyColorTableEXT(
		GLenum target, GLenum internalformat, GLint x, GLint y,
		GLsizei width)
	{ (SglFunc())->glCopyColorTable(target, internalformat, x, y, width); }
GLAPI void APIENTRY glGetColorTableEXT(
		GLenum target, GLenum format, GLenum type, GLvoid *table)
	{ (SglFunc())->glGetColorTable(target, format, type, table); }
GLAPI void APIENTRY glGetColorTableParameterfvEXT(
		GLenum target, GLenum pname, GLfloat *params)
	{ (SglFunc())->glGetColorTableParameterfv(target, pname, params); }
GLAPI void APIENTRY glGetColorTableParameterivEXT(
		GLenum target, GLenum pname, GLint *params)
	{ (SglFunc())->glGetColorTableParameteriv(target, pname, params); }
GLAPI void APIENTRY glBlendEquationEXT(
		GLenum mode)
	{ (SglFunc())->glBlendEquation(mode); }
GLAPI void APIENTRY glBlendColorEXT(
		GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
	{ (SglFunc())->glBlendColor(red, green, blue, alpha); }
GLAPI void APIENTRY glHistogramEXT(
		GLenum target, GLsizei width, GLenum internalformat, GLboolean sink)
	{ (SglFunc())->glHistogram(target, width, internalformat, sink); }
GLAPI void APIENTRY glResetHistogramEXT(
		GLenum target)
	{ (SglFunc())->glResetHistogram(target); }
GLAPI void APIENTRY glGetHistogramEXT(
		GLenum target, GLboolean reset, GLenum format,
		GLenum type, GLvoid *values)
	{ (SglFunc())->glGetHistogram(target, reset, format, type, values); }
GLAPI void APIENTRY glGetHistogramParameterfvEXT(
		GLenum target, GLenum pname, GLfloat *params)
	{ (SglFunc())->glGetHistogramParameterfv(target, pname, params); }
GLAPI void APIENTRY glGetHistogramParameterivEXT(
		GLenum target, GLenum pname, GLint *params)
	{ (SglFunc())->glGetHistogramParameteriv(target, pname, params); }
GLAPI void APIENTRY glMinmaxEXT(
		GLenum target, GLenum internalformat, GLboolean sink)
	{ (SglFunc())->glMinmax(target, internalformat, sink); }
GLAPI void APIENTRY glResetMinmaxEXT(GLenum target)
	{ (SglFunc())->glResetMinmax(target); }
GLAPI void APIENTRY glGetMinmaxEXT(
		GLenum target, GLboolean reset, GLenum format,
		GLenum types, GLvoid *values)
	{ (SglFunc())->glGetMinmax(target, reset, format, types, values); }
GLAPI void APIENTRY glGetMinmaxParameterfvEXT(
		GLenum target, GLenum pname, GLfloat *params)
	{ (SglFunc())->glGetMinmaxParameterfv(target, pname, params); }
GLAPI void APIENTRY glGetMinmaxParameterivEXT(
		GLenum target, GLenum pname, GLint *params)
	{ (SglFunc())->glGetMinmaxParameteriv(target, pname, params); }
GLAPI void APIENTRY glConvolutionFilter1DEXT(
		GLenum target, GLenum internalformat, GLsizei width,
		GLenum format, GLenum type, const GLvoid *image)
	{ (SglFunc())->glConvolutionFilter1D(target, internalformat, width,
		format, type, image); }
GLAPI void APIENTRY glConvolutionFilter2DEXT(
		GLenum target, GLenum internalformat, GLsizei width,
		GLsizei height, GLenum format, GLenum type,
		const GLvoid *image)
	{ (SglFunc())->glConvolutionFilter2D(target, internalformat, width,
		height, format, type, image); }
GLAPI void APIENTRY glConvolutionParameterfEXT(
		GLenum target, GLenum pname, GLfloat params)
	{ (SglFunc())->glConvolutionParameterf(target, pname, params); }
GLAPI void APIENTRY glConvolutionParameterfvEXT(
		GLenum target, GLenum pname, const GLfloat *params)
	{ (SglFunc())->glConvolutionParameterfv(target, pname, params); }
GLAPI void APIENTRY glConvolutionParameteriEXT(
		GLenum target, GLenum pname, GLint params)
	{ (SglFunc())->glConvolutionParameteri(target, pname, params); }
GLAPI void APIENTRY glConvolutionParameterivEXT(
		GLenum target, GLenum pname, const GLint *params)
	{ (SglFunc())->glConvolutionParameteriv(target, pname, params); }
GLAPI void APIENTRY glCopyConvolutionFilter1DEXT(
		GLenum target, GLenum internalformat, GLint x, GLint y,
		GLsizei width)
	{ (SglFunc())->glCopyConvolutionFilter1D(
		target, internalformat, x, y, width); }
GLAPI void APIENTRY glCopyConvolutionFilter2DEXT(
		GLenum target, GLenum internalformat, GLint x, GLint y,
		GLsizei width, GLsizei height)
	{ (SglFunc())->glCopyConvolutionFilter2D(
		target, internalformat, x, y, width, height); }
GLAPI void APIENTRY glGetConvolutionFilterEXT(
		GLenum target, GLenum format, GLenum type, GLvoid *image)
	{ (SglFunc())->glGetConvolutionFilter(target, format, type, image); }
GLAPI void APIENTRY glGetConvolutionParameterfvEXT(
		GLenum target, GLenum pname, GLfloat *params)
	{ (SglFunc())->glGetConvolutionParameterfv(target, pname, params); }
GLAPI void APIENTRY glGetConvolutionParameterivEXT(
		GLenum target, GLenum pname, GLint *params)
	{ (SglFunc())->glGetConvolutionParameteriv(target, pname, params); }
GLAPI void APIENTRY glSeparableFilter2DEXT(
		GLenum target, GLenum internalformat, GLsizei width,
		GLsizei height, GLenum format, GLenum type,
		const GLvoid *row, const GLvoid *column)
	{ (SglFunc())->glSeparableFilter2D(target, internalformat,
		width, height, format, type, row, column); }
GLAPI void APIENTRY glGetSeparableFilterEXT(
		GLenum target, GLenum format, GLenum type, GLvoid *row,
		GLvoid *column, GLvoid *span)
	{ (SglFunc())->glGetSeparableFilter(
		target, format, type, row, column, span); }

/* 1.3 functions */
GLAPI void APIENTRY glActiveTextureARB(
		GLenum texture)
	{ (SglFunc())->glActiveTexture(texture); }
GLAPI void APIENTRY glClientActiveTextureARB(
		GLenum texture)
	{ (SglFunc())->glClientActiveTexture(texture); }
GLAPI void APIENTRY glCompressedTexImage1DEXT(
		GLenum target, GLint level, GLenum internalformat,
		GLsizei width, GLint border, GLsizei imageSize,
		const GLvoid *data)
	{ (SglFunc())->glCompressedTexImage1D(target, level, internalformat,
		width, border, imageSize, data); }
GLAPI void APIENTRY glCompressedTexImage2DEXT(
		GLenum target, GLint level, GLenum internalformat,
		GLsizei width, GLsizei height, GLint border,
		GLsizei imageSize, const GLvoid *data)
	{ (SglFunc())->glCompressedTexImage2D(target, level, internalformat,
		width, height, border, imageSize, data); }
GLAPI void APIENTRY glCompressedTexImage3DEXT(
		GLenum target, GLint level, GLenum internalformat,
		GLsizei width, GLsizei height, GLsizei depth, GLint border,
		GLsizei imageSize, const GLvoid *data)
	{ (SglFunc())->glCompressedTexImage3D(target, level, internalformat,
		width, height, depth, border, imageSize, data); }
GLAPI void APIENTRY glCompressedTexSubImage1DEXT(
		GLenum target, GLint level, GLint xoffset, GLsizei width,
		GLenum format, GLsizei imageSize, const GLvoid *data)
	{ (SglFunc())->glCompressedTexSubImage1D(
		target, level, xoffset, width, format, imageSize, data); }
GLAPI void APIENTRY glCompressedTexSubImage2DEXT(
		GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLsizei width, GLsizei height, GLenum format,
		GLsizei imageSize, const GLvoid *data)
	{ (SglFunc())->glCompressedTexSubImage2D(
		target, level, xoffset, yoffset, width, height,
		format, imageSize, data); }
GLAPI void APIENTRY glCompressedTexSubImage3DEXT(
		GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,
		GLenum format, GLsizei imageSize, const GLvoid *data)
	{ (SglFunc())->glCompressedTexSubImage3D(
		target, level, xoffset, yoffset, zoffset, width, height, depth,
		format, imageSize, data); }
GLAPI void APIENTRY glGetCompressedTexImageEXT(
		GLenum target, GLint lod, GLvoid *img)
	{ (SglFunc())->glGetCompressedTexImage(target, lod, img); }
GLAPI void APIENTRY glMultiTexCoord1dARB(
		GLenum target, GLdouble s)
	{ (SglFunc())->glMultiTexCoord1d(target, s); }
GLAPI void APIENTRY glMultiTexCoord1dvARB(
		GLenum target, const GLdouble *v)
	{ (SglFunc())->glMultiTexCoord1dv(target, v); }
GLAPI void APIENTRY glMultiTexCoord1fARB(
		GLenum target, GLfloat s)
	{ (SglFunc())->glMultiTexCoord1f(target, s); }
GLAPI void APIENTRY glMultiTexCoord1fvARB(
		GLenum target, const GLfloat *v)
	{ (SglFunc())->glMultiTexCoord1fv(target, v); }
GLAPI void APIENTRY glMultiTexCoord1iARB(
		GLenum target, GLint s)
	{ (SglFunc())->glMultiTexCoord1i(target, s); }
GLAPI void APIENTRY glMultiTexCoord1ivARB(
		GLenum target, const GLint *v)
	{ (SglFunc())->glMultiTexCoord1iv(target, v); }
GLAPI void APIENTRY glMultiTexCoord1sARB(
		GLenum target, GLshort s)
	{ (SglFunc())->glMultiTexCoord1s(target, s); }
GLAPI void APIENTRY glMultiTexCoord1svARB(
		GLenum target, const GLshort *v)
	{ (SglFunc())->glMultiTexCoord1sv(target, v); }
GLAPI void APIENTRY glMultiTexCoord2dARB(
		GLenum target, GLdouble s, GLdouble t)
	{ (SglFunc())->glMultiTexCoord2d(target, s, t); }
GLAPI void APIENTRY glMultiTexCoord2dvARB(
		GLenum target, const GLdouble *v)
	{ (SglFunc())->glMultiTexCoord2dv(target, v); }
GLAPI void APIENTRY glMultiTexCoord2fARB(
		GLenum target, GLfloat s, GLfloat t)
	{ (SglFunc())->glMultiTexCoord2f(target, s, t); }
GLAPI void APIENTRY glMultiTexCoord2fvARB(
		GLenum target, const GLfloat *v)
	{ (SglFunc())->glMultiTexCoord2fv(target, v); }
GLAPI void APIENTRY glMultiTexCoord2iARB(
		GLenum target, GLint s, GLint t)
	{ (SglFunc())->glMultiTexCoord2i(target, s, t); }
GLAPI void APIENTRY glMultiTexCoord2ivARB(
		GLenum target, const GLint *v)
	{ (SglFunc())->glMultiTexCoord2iv(target, v); }
GLAPI void APIENTRY glMultiTexCoord2sARB(
		GLenum target, GLshort s, GLshort t)
	{ (SglFunc())->glMultiTexCoord2s(target, s, t); }
GLAPI void APIENTRY glMultiTexCoord2svARB(
		GLenum target, const GLshort *v)
	{ (SglFunc())->glMultiTexCoord2sv(target, v); }
GLAPI void APIENTRY glMultiTexCoord3dARB(
		GLenum target, GLdouble s, GLdouble t, GLdouble r)
	{ (SglFunc())->glMultiTexCoord3d(target, s, t, r); }
GLAPI void APIENTRY glMultiTexCoord3dvARB(
		GLenum target, const GLdouble *v)
	{ (SglFunc())->glMultiTexCoord3dv(target, v); }
GLAPI void APIENTRY glMultiTexCoord3fARB(
		GLenum target, GLfloat s, GLfloat t, GLfloat r)
	{ (SglFunc())->glMultiTexCoord3f(target, s, t, r); }
GLAPI void APIENTRY glMultiTexCoord3fvARB(
		GLenum target, const GLfloat *v)
	{ (SglFunc())->glMultiTexCoord3fv(target, v); }
GLAPI void APIENTRY glMultiTexCoord3iARB(
		GLenum target, GLint s, GLint t, GLint r)
	{ (SglFunc())->glMultiTexCoord3i(target, s, t, r); }
GLAPI void APIENTRY glMultiTexCoord3ivARB(
		GLenum target, const GLint *v)
	{ (SglFunc())->glMultiTexCoord3iv(target, v); }
GLAPI void APIENTRY glMultiTexCoord3sARB(
		GLenum target, GLshort s, GLshort t, GLshort r)
	{ (SglFunc())->glMultiTexCoord3s(target, s, t, r); }
GLAPI void APIENTRY glMultiTexCoord3svARB(
		GLenum target, const GLshort *v)
	{ (SglFunc())->glMultiTexCoord3sv(target, v); }
GLAPI void APIENTRY glMultiTexCoord4dARB(
		GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q)
	{ (SglFunc())->glMultiTexCoord4d(target, s, t, r, q); }
GLAPI void APIENTRY glMultiTexCoord4dvARB(
		GLenum target, const GLdouble *v)
	{ (SglFunc())->glMultiTexCoord4dv(target, v); }
GLAPI void APIENTRY glMultiTexCoord4fARB(
		GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q)
	{ (SglFunc())->glMultiTexCoord4f(target, s, t, r, q); }
GLAPI void APIENTRY glMultiTexCoord4fvARB(
		GLenum target, const GLfloat *v)
	{ (SglFunc())->glMultiTexCoord4fv(target, v); }
GLAPI void APIENTRY glMultiTexCoord4iARB(
		GLenum target, GLint s, GLint t, GLint r, GLint q)
	{ (SglFunc())->glMultiTexCoord4i(target, s, t, r, q); }
GLAPI void APIENTRY glMultiTexCoord4ivARB(
		GLenum target, const GLint *v)
	{ (SglFunc())->glMultiTexCoord4iv(target, v); }
GLAPI void APIENTRY glMultiTexCoord4sARB(
		GLenum target, GLshort s, GLshort t, GLshort r, GLshort q)
	{ (SglFunc())->glMultiTexCoord4s(target, s, t, r, q); }
GLAPI void APIENTRY glMultiTexCoord4svARB(
		GLenum target, const GLshort *v)
	{ (SglFunc())->glMultiTexCoord4sv(target, v); }
GLAPI void APIENTRY glLoadTransposeMatrixdEXT(
		const GLdouble m[16])
	{ (SglFunc())->glLoadTransposeMatrixd(m); }
GLAPI void APIENTRY glLoadTransposeMatrixfEXT(
		const GLfloat m[16])
	{ (SglFunc())->glLoadTransposeMatrixf(m); }
GLAPI void APIENTRY glMultTransposeMatrixdEXT(
		const GLdouble m[16])
	{ (SglFunc())->glMultTransposeMatrixd(m); }
GLAPI void APIENTRY glMultTransposeMatrixfEXT(
		const GLfloat m[16])
	{ (SglFunc())->glMultTransposeMatrixf(m); }
GLAPI void APIENTRY glSampleCoverageEXT(
		GLclampf value, GLboolean invert)
	{ (SglFunc())->glSampleCoverage(value, invert); }
GLAPI void APIENTRY glSamplePassEXT(
		GLenum pass)
	{ (SglFunc())->glSamplePass(pass); }

#endif


GLAPI void APIENTRY glResizeBuffersMESA(void)
	{ }

GLAPI void APIENTRY glWindowPos2dMESA(
		GLdouble x, GLdouble y)
	{ }
GLAPI void APIENTRY glWindowPos2dvMESA(
		const GLdouble *v)
	{ }
GLAPI void APIENTRY glWindowPos2fMESA(
		GLfloat x, GLfloat y)
	{ }
GLAPI void APIENTRY glWindowPos2fvMESA(
		const GLfloat *v)
	{ }
GLAPI void APIENTRY glWindowPos2iMESA(
		GLint x, GLint y)
	{ }
GLAPI void APIENTRY glWindowPos2ivMESA(
		const GLint *v)
	{ }
GLAPI void APIENTRY glWindowPos2sMESA(
		GLshort x, GLshort y)
	{ }
GLAPI void APIENTRY glWindowPos2svMESA(
		const GLshort *v)
	{ }
GLAPI void APIENTRY glWindowPos3dMESA(
		GLdouble x, GLdouble y, GLdouble z)
	{ }
GLAPI void APIENTRY glWindowPos3dvMESA(
		const GLdouble *v)
	{ }
GLAPI void APIENTRY glWindowPos3fMESA(
		GLfloat x, GLfloat y, GLfloat z)
	{ }
GLAPI void APIENTRY glWindowPos3fvMESA(
		const GLfloat *v)
	{ }
GLAPI void APIENTRY glWindowPos3iMESA(
		GLint x, GLint y, GLint z)
	{ }
GLAPI void APIENTRY glWindowPos3ivMESA(
		const GLint *v)
	{ }
GLAPI void APIENTRY glWindowPos3sMESA(
		GLshort x, GLshort y, GLshort z)
	{ }
GLAPI void APIENTRY glWindowPos3svMESA(
		const GLshort *v)
	{ }
GLAPI void APIENTRY glWindowPos4dMESA(
		GLdouble x, GLdouble y, GLdouble z, GLdouble w)
	{ }
GLAPI void APIENTRY glWindowPos4dvMESA(
		const GLdouble *v)
	{ }
GLAPI void APIENTRY glWindowPos4fMESA(
		GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{ }
GLAPI void APIENTRY glWindowPos4fvMESA(
		const GLfloat *v)
	{ }
GLAPI void APIENTRY glWindowPos4iMESA(
		GLint x, GLint y, GLint z, GLint w)
	{ }
GLAPI void APIENTRY glWindowPos4ivMESA(
		const GLint *v)
	{ }
GLAPI void APIENTRY glWindowPos4sMESA(
		GLshort x, GLshort y, GLshort z, GLshort w)
	{ }
GLAPI void APIENTRY glWindowPos4svMESA(
		const GLshort *v)
	{ }
