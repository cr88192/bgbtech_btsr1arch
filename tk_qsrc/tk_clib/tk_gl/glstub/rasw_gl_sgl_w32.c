#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include <tkgl/rasw_glstub.h>

HANDLE sgl_hdl_gldrv=NULL;
int sgl_tls_funcs;

void SglInit(void);

BGBRASW_GlStub_Funcs *SglFunc(void)
{
	if(!sgl_hdl_gldrv)
		SglInit();
	return(TlsGetValue(sgl_tls_funcs));
}

void *SglGetProcAddress(const char *name)
{
	char tb[256];
	void *ptr;
	int i;
	
	sprintf(tb, "GlDrv_%s", name);
	ptr=GetProcAddress(sgl_hdl_gldrv, tb);
	if(ptr)return(ptr);

	sprintf(tb, "_GlDrv_%s", name);
	ptr=GetProcAddress(sgl_hdl_gldrv, tb);
	if(ptr)return(ptr);
	
	for(i=0; i<16; i++)
	{
		sprintf(tb, "GlDrv_%s@%d", name, i*4);
		ptr=GetProcAddress(sgl_hdl_gldrv, tb);
		if(ptr)return(ptr);

		sprintf(tb, "_GlDrv_%s@%d", name, i*4);
		ptr=GetProcAddress(sgl_hdl_gldrv, tb);
		if(ptr)return(ptr);
	}
	
	return(NULL);
}

void SglInit(void)
{
	static int init=0;
	BGBRASW_GlStub_Funcs *funcs;
	
	if(init)return;
	init=1;
	
//	sgl_hdl_gldrv=LoadLibrary("rasw_gldrv_null.dll");
	sgl_hdl_gldrv=LoadLibrary("bgbrasw.dll");
	
	sgl_tls_funcs=TlsAlloc();
	funcs=LocalAlloc(LMEM_FIXED, sizeof(BGBRASW_GlStub_Funcs));
	TlsSetValue(sgl_tls_funcs, funcs);

	funcs->glClearIndex=SglGetProcAddress("glClearIndex");
	funcs->glClearColor=SglGetProcAddress("glClearColor");
	funcs->glClear=SglGetProcAddress("glClear");
	funcs->glIndexMask=SglGetProcAddress("glIndexMask");
	funcs->glColorMask=SglGetProcAddress("glColorMask");
	funcs->glAlphaFunc=SglGetProcAddress("glAlphaFunc");
	funcs->glBlendFunc=SglGetProcAddress("glBlendFunc");
	funcs->glLogicOp=SglGetProcAddress("glLogicOp");
	funcs->glCullFace=SglGetProcAddress("glCullFace");
	funcs->glFrontFace=SglGetProcAddress("glFrontFace");
	funcs->glPointSize=SglGetProcAddress("glPointSize");
	funcs->glLineWidth=SglGetProcAddress("glLineWidth");
	funcs->glLineStipple=SglGetProcAddress("glLineStipple");
	funcs->glPolygonMode=SglGetProcAddress("glPolygonMode");
	funcs->glPolygonOffset=SglGetProcAddress("glPolygonOffset");
	funcs->glPolygonStipple=SglGetProcAddress("glPolygonStipple");
	funcs->glGetPolygonStipple=SglGetProcAddress("glGetPolygonStipple");
	funcs->glEdgeFlag=SglGetProcAddress("glEdgeFlag");
	funcs->glEdgeFlagv=SglGetProcAddress("glEdgeFlagv");
	funcs->glScissor=SglGetProcAddress("glScissor");
	funcs->glClipPlane=SglGetProcAddress("glClipPlane");
	funcs->glGetClipPlane=SglGetProcAddress("glGetClipPlane");
	funcs->glDrawBuffer=SglGetProcAddress("glDrawBuffer");
	funcs->glReadBuffer=SglGetProcAddress("glReadBuffer");
	funcs->glEnable=SglGetProcAddress("glEnable");
	funcs->glDisable=SglGetProcAddress("glDisable");
	funcs->glIsEnabled=SglGetProcAddress("glIsEnabled");
	funcs->glEnableClientState=SglGetProcAddress("glEnableClientState");
	funcs->glDisableClientState=SglGetProcAddress("glDisableClientState");
	funcs->glGetBooleanv=SglGetProcAddress("glGetBooleanv");
	funcs->glGetDoublev=SglGetProcAddress("glGetDoublev");
	funcs->glGetFloatv=SglGetProcAddress("glGetFloatv");
	funcs->glGetIntegerv=SglGetProcAddress("glGetIntegerv");
	funcs->glPushAttrib=SglGetProcAddress("glPushAttrib");
	funcs->glPopAttrib=SglGetProcAddress("glPopAttrib");
	funcs->glPushClientAttrib=SglGetProcAddress("glPushClientAttrib");
	funcs->glPopClientAttrib=SglGetProcAddress("glPopClientAttrib");
	funcs->glRenderMode=SglGetProcAddress("glRenderMode");
	funcs->glGetError=SglGetProcAddress("glGetError");
	funcs->glGetString=SglGetProcAddress("glGetString");
	funcs->glFinish=SglGetProcAddress("glFinish");
	funcs->glFlush=SglGetProcAddress("glFlush");
	funcs->glHint=SglGetProcAddress("glHint");

	funcs->glClearDepth=SglGetProcAddress("glClearDepth");
	funcs->glDepthFunc=SglGetProcAddress("glDepthFunc");
	funcs->glDepthMask=SglGetProcAddress("glDepthMask");
	funcs->glDepthRange=SglGetProcAddress("glDepthRange");

	funcs->glClearAccum=SglGetProcAddress("glClearAccum");
	funcs->glAccum=SglGetProcAddress("glAccum");

	funcs->glMatrixMode=SglGetProcAddress("glMatrixMode");
	funcs->glOrtho=SglGetProcAddress("glOrtho");
	funcs->glFrustum=SglGetProcAddress("glFrustum");
	funcs->glViewport=SglGetProcAddress("glViewport");
	funcs->glPushMatrix=SglGetProcAddress("glPushMatrix");
	funcs->glPopMatrix=SglGetProcAddress("glPopMatrix");
	funcs->glLoadIdentity=SglGetProcAddress("glLoadIdentity");
	funcs->glLoadMatrixd=SglGetProcAddress("glLoadMatrixd");
	funcs->glLoadMatrixf=SglGetProcAddress("glLoadMatrixf");
	funcs->glMultMatrixd=SglGetProcAddress("glMultMatrixd");
	funcs->glMultMatrixf=SglGetProcAddress("glMultMatrixf");
	funcs->glRotated=SglGetProcAddress("glRotated");
	funcs->glRotatef=SglGetProcAddress("glRotatef");
	funcs->glScaled=SglGetProcAddress("glScaled");
	funcs->glScalef=SglGetProcAddress("glScalef");
	funcs->glTranslated=SglGetProcAddress("glTranslated");
	funcs->glTranslatef=SglGetProcAddress("glTranslatef");

	funcs->glIsList=SglGetProcAddress("glIsList");
	funcs->glDeleteLists=SglGetProcAddress("glDeleteLists");
	funcs->glGenLists=SglGetProcAddress("glGenLists");
	funcs->glNewList=SglGetProcAddress("glNewList");
	funcs->glEndList=SglGetProcAddress("glEndList");
	funcs->glCallList=SglGetProcAddress("glCallList");
	funcs->glCallLists=SglGetProcAddress("glCallLists");
	funcs->glListBase=SglGetProcAddress("glListBase");

	funcs->glBegin=SglGetProcAddress("glBegin");
	funcs->glEnd=SglGetProcAddress("glEnd");
	funcs->glVertex2d=SglGetProcAddress("glVertex2d");
	funcs->glVertex2f=SglGetProcAddress("glVertex2f");
	funcs->glVertex2i=SglGetProcAddress("glVertex2i");
	funcs->glVertex2s=SglGetProcAddress("glVertex2s");
	funcs->glVertex3d=SglGetProcAddress("glVertex3d");
	funcs->glVertex3f=SglGetProcAddress("glVertex3f");
	funcs->glVertex3i=SglGetProcAddress("glVertex3i");
	funcs->glVertex3s=SglGetProcAddress("glVertex3s");
	funcs->glVertex4d=SglGetProcAddress("glVertex4d");
	funcs->glVertex4f=SglGetProcAddress("glVertex4f");
	funcs->glVertex4i=SglGetProcAddress("glVertex4i");
	funcs->glVertex4s=SglGetProcAddress("glVertex4s");
	funcs->glVertex2dv=SglGetProcAddress("glVertex2dv");
	funcs->glVertex2fv=SglGetProcAddress("glVertex2fv");
	funcs->glVertex2iv=SglGetProcAddress("glVertex2iv");
	funcs->glVertex2sv=SglGetProcAddress("glVertex2sv");
	funcs->glVertex3dv=SglGetProcAddress("glVertex3dv");
	funcs->glVertex3fv=SglGetProcAddress("glVertex3fv");
	funcs->glVertex3iv=SglGetProcAddress("glVertex3iv");
	funcs->glVertex3sv=SglGetProcAddress("glVertex3sv");
	funcs->glVertex4dv=SglGetProcAddress("glVertex4dv");
	funcs->glVertex4fv=SglGetProcAddress("glVertex4fv");
	funcs->glVertex4iv=SglGetProcAddress("glVertex4iv");
	funcs->glVertex4sv=SglGetProcAddress("glVertex4sv");
	funcs->glNormal3b=SglGetProcAddress("glNormal3b");
	funcs->glNormal3d=SglGetProcAddress("glNormal3d");
	funcs->glNormal3f=SglGetProcAddress("glNormal3f");
	funcs->glNormal3i=SglGetProcAddress("glNormal3i");
	funcs->glNormal3s=SglGetProcAddress("glNormal3s");
	funcs->glNormal3bv=SglGetProcAddress("glNormal3bv");
	funcs->glNormal3dv=SglGetProcAddress("glNormal3dv");
	funcs->glNormal3fv=SglGetProcAddress("glNormal3fv");
	funcs->glNormal3iv=SglGetProcAddress("glNormal3iv");
	funcs->glNormal3sv=SglGetProcAddress("glNormal3sv");
	funcs->glIndexd=SglGetProcAddress("glIndexd");
	funcs->glIndexf=SglGetProcAddress("glIndexf");
	funcs->glIndexi=SglGetProcAddress("glIndexi");
	funcs->glIndexs=SglGetProcAddress("glIndexs");
	funcs->glIndexub=SglGetProcAddress("glIndexub");
	funcs->glIndexdv=SglGetProcAddress("glIndexdv");
	funcs->glIndexfv=SglGetProcAddress("glIndexfv");
	funcs->glIndexiv=SglGetProcAddress("glIndexiv");
	funcs->glIndexsv=SglGetProcAddress("glIndexsv");
	funcs->glIndexubv=SglGetProcAddress("glIndexubv");
	funcs->glColor3b=SglGetProcAddress("glColor3b");
	funcs->glColor3d=SglGetProcAddress("glColor3d");
	funcs->glColor3f=SglGetProcAddress("glColor3f");
	funcs->glColor3i=SglGetProcAddress("glColor3i");
	funcs->glColor3s=SglGetProcAddress("glColor3s");
	funcs->glColor3ub=SglGetProcAddress("glColor3ub");
	funcs->glColor3ui=SglGetProcAddress("glColor3ui");
	funcs->glColor3us=SglGetProcAddress("glColor3us");
	funcs->glColor4b=SglGetProcAddress("glColor4b");
	funcs->glColor4d=SglGetProcAddress("glColor4d");
	funcs->glColor4f=SglGetProcAddress("glColor4f");
	funcs->glColor4i=SglGetProcAddress("glColor4i");
	funcs->glColor4s=SglGetProcAddress("glColor4s");
	funcs->glColor4ub=SglGetProcAddress("glColor4ub");
	funcs->glColor4ui=SglGetProcAddress("glColor4ui");
	funcs->glColor4us=SglGetProcAddress("glColor4us");
	funcs->glColor3bv=SglGetProcAddress("glColor3bv");
	funcs->glColor3dv=SglGetProcAddress("glColor3dv");
	funcs->glColor3fv=SglGetProcAddress("glColor3fv");
	funcs->glColor3iv=SglGetProcAddress("glColor3iv");
	funcs->glColor3sv=SglGetProcAddress("glColor3sv");
	funcs->glColor3ubv=SglGetProcAddress("glColor3ubv");
	funcs->glColor3uiv=SglGetProcAddress("glColor3uiv");
	funcs->glColor3usv=SglGetProcAddress("glColor3usv");
	funcs->glColor4bv=SglGetProcAddress("glColor4bv");
	funcs->glColor4dv=SglGetProcAddress("glColor4dv");
	funcs->glColor4fv=SglGetProcAddress("glColor4fv");
	funcs->glColor4iv=SglGetProcAddress("glColor4iv");
	funcs->glColor4sv=SglGetProcAddress("glColor4sv");
	funcs->glColor4ubv=SglGetProcAddress("glColor4ubv");
	funcs->glColor4uiv=SglGetProcAddress("glColor4uiv");
	funcs->glColor4usv=SglGetProcAddress("glColor4usv");
	funcs->glTexCoord1d=SglGetProcAddress("glTexCoord1d");
	funcs->glTexCoord1f=SglGetProcAddress("glTexCoord1f");
	funcs->glTexCoord1i=SglGetProcAddress("glTexCoord1i");
	funcs->glTexCoord1s=SglGetProcAddress("glTexCoord1s");
	funcs->glTexCoord2d=SglGetProcAddress("glTexCoord2d");
	funcs->glTexCoord2f=SglGetProcAddress("glTexCoord2f");
	funcs->glTexCoord2i=SglGetProcAddress("glTexCoord2i");
	funcs->glTexCoord2s=SglGetProcAddress("glTexCoord2s");
	funcs->glTexCoord3d=SglGetProcAddress("glTexCoord3d");
	funcs->glTexCoord3f=SglGetProcAddress("glTexCoord3f");
	funcs->glTexCoord3i=SglGetProcAddress("glTexCoord3i");
	funcs->glTexCoord3s=SglGetProcAddress("glTexCoord3s");
	funcs->glTexCoord4d=SglGetProcAddress("glTexCoord4d");
	funcs->glTexCoord4f=SglGetProcAddress("glTexCoord4f");
	funcs->glTexCoord4i=SglGetProcAddress("glTexCoord4i");
	funcs->glTexCoord4s=SglGetProcAddress("glTexCoord4s");
	funcs->glTexCoord1dv=SglGetProcAddress("glTexCoord1dv");
	funcs->glTexCoord1fv=SglGetProcAddress("glTexCoord1fv");
	funcs->glTexCoord1iv=SglGetProcAddress("glTexCoord1iv");
	funcs->glTexCoord1sv=SglGetProcAddress("glTexCoord1sv");
	funcs->glTexCoord2dv=SglGetProcAddress("glTexCoord2dv");
	funcs->glTexCoord2fv=SglGetProcAddress("glTexCoord2fv");
	funcs->glTexCoord2iv=SglGetProcAddress("glTexCoord2iv");
	funcs->glTexCoord2sv=SglGetProcAddress("glTexCoord2sv");
	funcs->glTexCoord3dv=SglGetProcAddress("glTexCoord3dv");
	funcs->glTexCoord3fv=SglGetProcAddress("glTexCoord3fv");
	funcs->glTexCoord3iv=SglGetProcAddress("glTexCoord3iv");
	funcs->glTexCoord3sv=SglGetProcAddress("glTexCoord3sv");
	funcs->glTexCoord4dv=SglGetProcAddress("glTexCoord4dv");
	funcs->glTexCoord4fv=SglGetProcAddress("glTexCoord4fv");
	funcs->glTexCoord4iv=SglGetProcAddress("glTexCoord4iv");
	funcs->glTexCoord4sv=SglGetProcAddress("glTexCoord4sv");
	funcs->glRasterPos2d=SglGetProcAddress("glRasterPos2d");
	funcs->glRasterPos2f=SglGetProcAddress("glRasterPos2f");
	funcs->glRasterPos2i=SglGetProcAddress("glRasterPos2i");
	funcs->glRasterPos2s=SglGetProcAddress("glRasterPos2s");
	funcs->glRasterPos3d=SglGetProcAddress("glRasterPos3d");
	funcs->glRasterPos3f=SglGetProcAddress("glRasterPos3f");
	funcs->glRasterPos3i=SglGetProcAddress("glRasterPos3i");
	funcs->glRasterPos3s=SglGetProcAddress("glRasterPos3s");
	funcs->glRasterPos4d=SglGetProcAddress("glRasterPos4d");
	funcs->glRasterPos4f=SglGetProcAddress("glRasterPos4f");
	funcs->glRasterPos4i=SglGetProcAddress("glRasterPos4i");
	funcs->glRasterPos4s=SglGetProcAddress("glRasterPos4s");
	funcs->glRasterPos2dv=SglGetProcAddress("glRasterPos2dv");
	funcs->glRasterPos2fv=SglGetProcAddress("glRasterPos2fv");
	funcs->glRasterPos2iv=SglGetProcAddress("glRasterPos2iv");
	funcs->glRasterPos2sv=SglGetProcAddress("glRasterPos2sv");
	funcs->glRasterPos3dv=SglGetProcAddress("glRasterPos3dv");
	funcs->glRasterPos3fv=SglGetProcAddress("glRasterPos3fv");
	funcs->glRasterPos3iv=SglGetProcAddress("glRasterPos3iv");
	funcs->glRasterPos3sv=SglGetProcAddress("glRasterPos3sv");
	funcs->glRasterPos4dv=SglGetProcAddress("glRasterPos4dv");
	funcs->glRasterPos4fv=SglGetProcAddress("glRasterPos4fv");
	funcs->glRasterPos4iv=SglGetProcAddress("glRasterPos4iv");
	funcs->glRasterPos4sv=SglGetProcAddress("glRasterPos4sv");
	funcs->glRectd=SglGetProcAddress("glRectd");
	funcs->glRectf=SglGetProcAddress("glRectf");
	funcs->glRecti=SglGetProcAddress("glRecti");
	funcs->glRects=SglGetProcAddress("glRects");
	funcs->glRectdv=SglGetProcAddress("glRectdv");
	funcs->glRectfv=SglGetProcAddress("glRectfv");
	funcs->glRectiv=SglGetProcAddress("glRectiv");
	funcs->glRectsv=SglGetProcAddress("glRectsv");

	funcs->glShadeModel=SglGetProcAddress("glShadeModel");
	funcs->glLightf=SglGetProcAddress("glLightf");
	funcs->glLighti=SglGetProcAddress("glLighti");
	funcs->glLightfv=SglGetProcAddress("glLightfv");
	funcs->glLightiv=SglGetProcAddress("glLightiv");
	funcs->glGetLightfv=SglGetProcAddress("glGetLightfv");
	funcs->glGetLightiv=SglGetProcAddress("glGetLightiv");
	funcs->glLightModelf=SglGetProcAddress("glLightModelf");
	funcs->glLightModeli=SglGetProcAddress("glLightModeli");
	funcs->glLightModelfv=SglGetProcAddress("glLightModelfv");
	funcs->glLightModeliv=SglGetProcAddress("glLightModeliv");
	funcs->glMaterialf=SglGetProcAddress("glMaterialf");
	funcs->glMateriali=SglGetProcAddress("glMateriali");
	funcs->glMaterialfv=SglGetProcAddress("glMaterialfv");
	funcs->glMaterialiv=SglGetProcAddress("glMaterialiv");
	funcs->glGetMaterialfv=SglGetProcAddress("glGetMaterialfv");
	funcs->glGetMaterialiv=SglGetProcAddress("glGetMaterialiv");
	funcs->glColorMaterial=SglGetProcAddress("glColorMaterial");

	funcs->glPixelZoom=SglGetProcAddress("glPixelZoom");
	funcs->glPixelStoref=SglGetProcAddress("glPixelStoref");
	funcs->glPixelStorei=SglGetProcAddress("glPixelStorei");
	funcs->glPixelTransferf=SglGetProcAddress("glPixelTransferf");
	funcs->glPixelTransferi=SglGetProcAddress("glPixelTransferi");
	funcs->glPixelMapfv=SglGetProcAddress("glPixelMapfv");
	funcs->glPixelMapuiv=SglGetProcAddress("glPixelMapuiv");
	funcs->glPixelMapusv=SglGetProcAddress("glPixelMapusv");
	funcs->glGetPixelMapfv=SglGetProcAddress("glGetPixelMapfv");
	funcs->glGetPixelMapuiv=SglGetProcAddress("glGetPixelMapuiv");
	funcs->glGetPixelMapusv=SglGetProcAddress("glGetPixelMapusv");
	funcs->glBitmap=SglGetProcAddress("glBitmap");
	funcs->glReadPixels=SglGetProcAddress("glReadPixels");
	funcs->glDrawPixels=SglGetProcAddress("glDrawPixels");
	funcs->glCopyPixels=SglGetProcAddress("glCopyPixels");

	funcs->glStencilFunc=SglGetProcAddress("glStencilFunc");
	funcs->glStencilMask=SglGetProcAddress("glStencilMask");
	funcs->glStencilOp=SglGetProcAddress("glStencilOp");
	funcs->glClearStencil=SglGetProcAddress("glClearStencil");

	funcs->glTexGend=SglGetProcAddress("glTexGend");
	funcs->glTexGenf=SglGetProcAddress("glTexGenf");
	funcs->glTexGeni=SglGetProcAddress("glTexGeni");
	funcs->glTexGendv=SglGetProcAddress("glTexGendv");
	funcs->glTexGenfv=SglGetProcAddress("glTexGenfv");
	funcs->glTexGeniv=SglGetProcAddress("glTexGeniv");
	funcs->glGetTexGendv=SglGetProcAddress("glGetTexGendv");
	funcs->glGetTexGenfv=SglGetProcAddress("glGetTexGenfv");
	funcs->glGetTexGeniv=SglGetProcAddress("glGetTexGeniv");
	funcs->glTexEnvf=SglGetProcAddress("glTexEnvf");
	funcs->glTexEnvi=SglGetProcAddress("glTexEnvi");
	funcs->glTexEnvfv=SglGetProcAddress("glTexEnvfv");
	funcs->glTexEnviv=SglGetProcAddress("glTexEnviv");
	funcs->glGetTexEnvfv=SglGetProcAddress("glGetTexEnvfv");
	funcs->glGetTexEnviv=SglGetProcAddress("glGetTexEnviv");
	funcs->glTexParameterf=SglGetProcAddress("glTexParameterf");
	funcs->glTexParameteri=SglGetProcAddress("glTexParameteri");
	funcs->glTexParameterfv=SglGetProcAddress("glTexParameterfv");
	funcs->glTexParameteriv=SglGetProcAddress("glTexParameteriv");
	funcs->glGetTexParameterfv=SglGetProcAddress("glGetTexParameterfv");
	funcs->glGetTexParameteriv=SglGetProcAddress("glGetTexParameteriv");
	funcs->glGetTexLevelParameterfv=
		SglGetProcAddress("glGetTexLevelParameterfv");
	funcs->glGetTexLevelParameteriv=
		SglGetProcAddress("glGetTexLevelParameteriv");
	funcs->glTexImage1D=SglGetProcAddress("glTexImage1D");
	funcs->glTexImage2D=SglGetProcAddress("glTexImage2D");
	funcs->glGetTexImage=SglGetProcAddress("glGetTexImage");

	funcs->glMap1d=SglGetProcAddress("glMap1d");
	funcs->glMap1f=SglGetProcAddress("glMap1f");
	funcs->glMap2d=SglGetProcAddress("glMap2d");
	funcs->glMap2f=SglGetProcAddress("glMap2f");
	funcs->glGetMapdv=SglGetProcAddress("glGetMapdv");
	funcs->glGetMapfv=SglGetProcAddress("glGetMapfv");
	funcs->glGetMapiv=SglGetProcAddress("glGetMapiv");
	funcs->glEvalCoord1d=SglGetProcAddress("glEvalCoord1d");
	funcs->glEvalCoord1f=SglGetProcAddress("glEvalCoord1f");
	funcs->glEvalCoord1dv=SglGetProcAddress("glEvalCoord1dv");
	funcs->glEvalCoord1fv=SglGetProcAddress("glEvalCoord1fv");
	funcs->glEvalCoord2d=SglGetProcAddress("glEvalCoord2d");
	funcs->glEvalCoord2f=SglGetProcAddress("glEvalCoord2f");
	funcs->glEvalCoord2dv=SglGetProcAddress("glEvalCoord2dv");
	funcs->glEvalCoord2fv=SglGetProcAddress("glEvalCoord2fv");
	funcs->glMapGrid1d=SglGetProcAddress("glMapGrid1d");
	funcs->glMapGrid1f=SglGetProcAddress("glMapGrid1f");
	funcs->glMapGrid2d=SglGetProcAddress("glMapGrid2d");
	funcs->glMapGrid2f=SglGetProcAddress("glMapGrid2f");
	funcs->glEvalPoint1=SglGetProcAddress("glEvalPoint1");
	funcs->glEvalPoint2=SglGetProcAddress("glEvalPoint2");
	funcs->glEvalMesh1=SglGetProcAddress("glEvalMesh1");
	funcs->glEvalMesh2=SglGetProcAddress("glEvalMesh2");

	funcs->glFogf=SglGetProcAddress("glFogf");
	funcs->glFogi=SglGetProcAddress("glFogi");
	funcs->glFogfv=SglGetProcAddress("glFogfv");
	funcs->glFogiv=SglGetProcAddress("glFogiv");

	funcs->glFeedbackBuffer=SglGetProcAddress("glFeedbackBuffer");
	funcs->glPassThrough=SglGetProcAddress("glPassThrough");
	funcs->glSelectBuffer=SglGetProcAddress("glSelectBuffer");
	funcs->glInitNames=SglGetProcAddress("glInitNames");
	funcs->glLoadName=SglGetProcAddress("glLoadName");
	funcs->glPushName=SglGetProcAddress("glPushName");
	funcs->glPopName=SglGetProcAddress("glPopName");

	funcs->glGenTextures=SglGetProcAddress("glGenTextures");
	funcs->glDeleteTextures=SglGetProcAddress("glDeleteTextures");
	funcs->glBindTexture=SglGetProcAddress("glBindTexture");
	funcs->glPrioritizeTextures=SglGetProcAddress("glPrioritizeTextures");
	funcs->glAreTexturesResident=SglGetProcAddress("glAreTexturesResident");
	funcs->glIsTexture=SglGetProcAddress("glIsTexture");

	funcs->glTexSubImage1D=SglGetProcAddress("glTexSubImage1D");
	funcs->glTexSubImage2D=SglGetProcAddress("glTexSubImage2D");
	funcs->glCopyTexImage1D=SglGetProcAddress("glCopyTexImage1D");
	funcs->glCopyTexImage2D=SglGetProcAddress("glCopyTexImage2D");
	funcs->glCopyTexSubImage1D=SglGetProcAddress("glCopyTexSubImage1D");
	funcs->glCopyTexSubImage2D=SglGetProcAddress("glCopyTexSubImage2D");
	
	funcs->glVertexPointer=SglGetProcAddress("glVertexPointer");
	funcs->glNormalPointer=SglGetProcAddress("glNormalPointer");
	funcs->glColorPointer=SglGetProcAddress("glColorPointer");
	funcs->glIndexPointer=SglGetProcAddress("glIndexPointer");
	funcs->glTexCoordPointer=SglGetProcAddress("glTexCoordPointer");
	funcs->glEdgeFlagPointer=SglGetProcAddress("glEdgeFlagPointer");
	funcs->glGetPointerv=SglGetProcAddress("glGetPointerv");
	funcs->glArrayElement=SglGetProcAddress("glArrayElement");
	funcs->glDrawArrays=SglGetProcAddress("glDrawArrays");
	funcs->glDrawElements=SglGetProcAddress("glDrawElements");
	funcs->glInterleavedArrays=SglGetProcAddress("glInterleavedArrays");

	funcs->glDrawRangeElements=SglGetProcAddress("glDrawRangeElements");
	funcs->glTexImage3D=SglGetProcAddress("glTexImage3D");
	funcs->glTexSubImage3D=SglGetProcAddress("glTexSubImage3D");
	funcs->glCopyTexSubImage3D=SglGetProcAddress("glCopyTexSubImage3D");

	funcs->glColorTable=SglGetProcAddress("glColorTable");
	funcs->glColorSubTable=SglGetProcAddress("glColorSubTable");
	funcs->glColorTableParameteriv=
		SglGetProcAddress("glColorTableParameteriv");
	funcs->glColorTableParameterfv=
		SglGetProcAddress("glColorTableParameterfv");
	funcs->glCopyColorSubTable=SglGetProcAddress("glCopyColorSubTable");
	funcs->glCopyColorTable=SglGetProcAddress("glCopyColorTable");
	funcs->glGetColorTable=SglGetProcAddress("glGetColorTable");
	funcs->glGetColorTableParameterfv=
		SglGetProcAddress("glGetColorTableParameterfv");
	funcs->glGetColorTableParameteriv=
		SglGetProcAddress("glGetColorTableParameteriv");
	funcs->glBlendEquation=SglGetProcAddress("glBlendEquation");
	funcs->glBlendColor=SglGetProcAddress("glBlendColor");
	funcs->glHistogram=SglGetProcAddress("glHistogram");
	funcs->glResetHistogram=SglGetProcAddress("glResetHistogram");
	funcs->glGetHistogram=SglGetProcAddress("glGetHistogram");
	funcs->glGetHistogramParameterfv=
		SglGetProcAddress("glGetHistogramParameterfv");
	funcs->glGetHistogramParameteriv=
		SglGetProcAddress("glGetHistogramParameteriv");
	funcs->glMinmax=SglGetProcAddress("glMinmax");
	funcs->glResetMinmax=SglGetProcAddress("glResetMinmax");
	funcs->glGetMinmax=SglGetProcAddress("glGetMinmax");
	funcs->glGetMinmaxParameterfv=SglGetProcAddress("glGetMinmaxParameterfv");
	funcs->glGetMinmaxParameteriv=SglGetProcAddress("glGetMinmaxParameteriv");
	funcs->glConvolutionFilter1D=SglGetProcAddress("glConvolutionFilter1D");
	funcs->glConvolutionFilter2D=SglGetProcAddress("glConvolutionFilter2D");
	funcs->glConvolutionParameterf=
		SglGetProcAddress("glConvolutionParameterf");
	funcs->glConvolutionParameterfv=
		SglGetProcAddress("glConvolutionParameterfv");
	funcs->glConvolutionParameteri=
		SglGetProcAddress("glConvolutionParameteri");
	funcs->glConvolutionParameteriv=
		SglGetProcAddress("glConvolutionParameteriv");
	funcs->glCopyConvolutionFilter1D=
		SglGetProcAddress("glCopyConvolutionFilter1D");
	funcs->glCopyConvolutionFilter2D=
		SglGetProcAddress("glCopyConvolutionFilter2D");
	funcs->glGetConvolutionFilter=
		SglGetProcAddress("glGetConvolutionFilter");
	funcs->glGetConvolutionParameterfv=
		SglGetProcAddress("glGetConvolutionParameterfv");
	funcs->glGetConvolutionParameteriv=
		SglGetProcAddress("glGetConvolutionParameteriv");
	funcs->glSeparableFilter2D=
		SglGetProcAddress("glSeparableFilter2D");
	funcs->glGetSeparableFilter=
		SglGetProcAddress("glGetSeparableFilter");

	funcs->glActiveTexture=SglGetProcAddress("glActiveTexture");
	funcs->glClientActiveTexture=SglGetProcAddress("glClientActiveTexture");
	funcs->glCompressedTexImage1D=SglGetProcAddress("glCompressedTexImage1D");
	funcs->glCompressedTexImage2D=SglGetProcAddress("glCompressedTexImage2D");
	funcs->glCompressedTexImage3D=SglGetProcAddress("glCompressedTexImage3D");
	funcs->glCompressedTexSubImage1D=
		SglGetProcAddress("glCompressedTexSubImage1D");
	funcs->glCompressedTexSubImage2D=
		SglGetProcAddress("glCompressedTexSubImage2D");
	funcs->glCompressedTexSubImage3D=
		SglGetProcAddress("glCompressedTexSubImage3D");
	funcs->glGetCompressedTexImage=
		SglGetProcAddress("glGetCompressedTexImage");
	funcs->glMultiTexCoord1d=SglGetProcAddress("glMultiTexCoord1d");
	funcs->glMultiTexCoord1dv=SglGetProcAddress("glMultiTexCoord1dv");
	funcs->glMultiTexCoord1f=SglGetProcAddress("glMultiTexCoord1f");
	funcs->glMultiTexCoord1fv=SglGetProcAddress("glMultiTexCoord1fv");
	funcs->glMultiTexCoord1i=SglGetProcAddress("glMultiTexCoord1i");
	funcs->glMultiTexCoord1iv=SglGetProcAddress("glMultiTexCoord1iv");
	funcs->glMultiTexCoord1s=SglGetProcAddress("glMultiTexCoord1s");
	funcs->glMultiTexCoord1sv=SglGetProcAddress("glMultiTexCoord1sv");
	funcs->glMultiTexCoord2d=SglGetProcAddress("glMultiTexCoord2d");
	funcs->glMultiTexCoord2dv=SglGetProcAddress("glMultiTexCoord2dv");
	funcs->glMultiTexCoord2f=SglGetProcAddress("glMultiTexCoord2f");
	funcs->glMultiTexCoord2fv=SglGetProcAddress("glMultiTexCoord2fv");
	funcs->glMultiTexCoord2i=SglGetProcAddress("glMultiTexCoord2i");
	funcs->glMultiTexCoord2iv=SglGetProcAddress("glMultiTexCoord2iv");
	funcs->glMultiTexCoord2s=SglGetProcAddress("glMultiTexCoord2s");
	funcs->glMultiTexCoord2sv=SglGetProcAddress("glMultiTexCoord2sv");
	funcs->glMultiTexCoord3d=SglGetProcAddress("glMultiTexCoord3d");
	funcs->glMultiTexCoord3dv=SglGetProcAddress("glMultiTexCoord3dv");
	funcs->glMultiTexCoord3f=SglGetProcAddress("glMultiTexCoord3f");
	funcs->glMultiTexCoord3fv=SglGetProcAddress("glMultiTexCoord3fv");
	funcs->glMultiTexCoord3i=SglGetProcAddress("glMultiTexCoord3i");
	funcs->glMultiTexCoord3iv=SglGetProcAddress("glMultiTexCoord3iv");
	funcs->glMultiTexCoord3s=SglGetProcAddress("glMultiTexCoord3s");
	funcs->glMultiTexCoord3sv=SglGetProcAddress("glMultiTexCoord3sv");
	funcs->glMultiTexCoord4d=SglGetProcAddress("glMultiTexCoord4d");
	funcs->glMultiTexCoord4dv=SglGetProcAddress("glMultiTexCoord4dv");
	funcs->glMultiTexCoord4f=SglGetProcAddress("glMultiTexCoord4f");
	funcs->glMultiTexCoord4fv=SglGetProcAddress("glMultiTexCoord4fv");
	funcs->glMultiTexCoord4i=SglGetProcAddress("glMultiTexCoord4i");
	funcs->glMultiTexCoord4iv=SglGetProcAddress("glMultiTexCoord4iv");
	funcs->glMultiTexCoord4s=SglGetProcAddress("glMultiTexCoord4s");
	funcs->glMultiTexCoord4sv=SglGetProcAddress("glMultiTexCoord4sv");
	funcs->glLoadTransposeMatrixd=SglGetProcAddress("glLoadTransposeMatrixd");
	funcs->glLoadTransposeMatrixf=SglGetProcAddress("glLoadTransposeMatrixf");
	funcs->glMultTransposeMatrixd=SglGetProcAddress("glMultTransposeMatrixd");
	funcs->glMultTransposeMatrixf=SglGetProcAddress("glMultTransposeMatrixf");
	funcs->glSampleCoverage=SglGetProcAddress("glSampleCoverage");
	funcs->glSamplePass=SglGetProcAddress("glSamplePass");

	funcs->wglCopyContext=SglGetProcAddress("wglCopyContext");
	funcs->wglCreateContext=SglGetProcAddress("wglCreateContext");
	funcs->wglCreateLayerContext=SglGetProcAddress("wglCreateLayerContext");
	funcs->wglDeleteContext=SglGetProcAddress("wglDeleteContext");
	funcs->wglDescribeLayerPlane=SglGetProcAddress("wglDescribeLayerPlane");
	funcs->wglGetCurrentContext=SglGetProcAddress("wglGetCurrentContext");
	funcs->wglGetCurrentDC=SglGetProcAddress("wglGetCurrentDC");
	funcs->wglGetLayerPaletteEntries=SglGetProcAddress(
		"wglGetLayerPaletteEntries");
	funcs->wglGetProcAddress=SglGetProcAddress("wglGetProcAddress");
	funcs->wglMakeCurrent=SglGetProcAddress("wglMakeCurrent");
	funcs->wglRealizeLayerPalette=SglGetProcAddress("wglRealizeLayerPalette");
	funcs->wglSetLayerPaletteEntries=SglGetProcAddress(
		"wglSetLayerPaletteEntries");
	funcs->wglShareLists=SglGetProcAddress("wglShareLists");
	funcs->wglSwapLayerBuffers=SglGetProcAddress("wglSwapLayerBuffers");
	funcs->wglUseFontBitmapsA=SglGetProcAddress("wglUseFontBitmapsA");
	funcs->wglUseFontBitmapsW=SglGetProcAddress("wglUseFontBitmapsW");
	funcs->wglUseFontOutlinesA=SglGetProcAddress("wglUseFontOutlinesA");
	funcs->wglUseFontOutlinesW=SglGetProcAddress("wglUseFontOutlinesW");
	funcs->wglChoosePixelFormat=SglGetProcAddress("wglChoosePixelFormat");
	funcs->wglGetPixelFormat=SglGetProcAddress("wglGetPixelFormat");
	funcs->wglSetPixelFormat=SglGetProcAddress("wglSetPixelFormat");
	funcs->wglDescribePixelFormat=SglGetProcAddress("wglDescribePixelFormat");
	funcs->wglSwapBuffers=SglGetProcAddress("wglSwapBuffers");

}

