struct tkra_glfunc_s {
char *name;
void *func;
};

const struct tkra_glfunc_s tkra_glfuncs[]={
{ "glVertexPointer",		tkra_glVertexPointer },
{ "glTexCoordPointer",		tkra_glTexCoordPointer },
{ "glColorPointer",			tkra_glColorPointer },
{ "glNormalPointer",		tkra_glNormalPointer },
{ "glInterleavedArrays",	tkra_glInterleavedArrays },
{ "glDrawArrays",			tkra_glDrawArrays },
{ "glDrawElements",			tkra_glDrawElements },
{ "glBegin",				tkra_glBegin },
{ "glEnd",					tkra_glEnd },
{ "glArrayElement",			tkra_glArrayElement },
{ "glLockArraysEXT",		tkra_glLockArraysEXT },
{ "glUnlockArraysEXT",		tkra_glUnlockArraysEXT },

{ "glTexImage2D",			tkra_glTexImage2D },
{ "glTexSubImage2D",		tkra_glTexSubImage2D },
{ "glCompressedTexImage2D",	tkra_glCompressedTexImage2D },
{ "glBindTexture",			tkra_glBindTexture },
{ "glTexParameterf",		tkra_glTexParameterf },
{ "glTexParameteri",		tkra_glTexParameteri },
{ "glTexParameterfv",		tkra_glTexParameterfv },
{ "glTexParameteriv",		tkra_glTexParameteriv },
{ "glTexEnvf",				tkra_glTexEnvf },
{ "glShadeModel",			tkra_glShadeModel },
{ "glDeleteTextures",		tkra_glDeleteTextures },

{ "glColor3b",				tkra_glColor3b },
{ "glColor3s",				tkra_glColor3s },
{ "glColor3i",				tkra_glColor3i },
{ "glColor3f",				tkra_glColor3f },
{ "glColor3d",				tkra_glColor3d },
{ "glColor3ub",				tkra_glColor3ub },
{ "glColor3us",				tkra_glColor3us },
{ "glColor3ui",				tkra_glColor3ui },
{ "glColor4b",				tkra_glColor4b },
{ "glColor4s",				tkra_glColor4s },
{ "glColor4i",				tkra_glColor4i },
{ "glColor4f",				tkra_glColor4f },
{ "glColor4d",				tkra_glColor4d },
{ "glColor4ub",				tkra_glColor4ub },
{ "glColor4us",				tkra_glColor4us },
{ "glColor4ui",				tkra_glColor4ui },
{ "glColor3bv",				tkra_glColor3bv },
{ "glColor3sv",				tkra_glColor3sv },
{ "glColor3iv",				tkra_glColor3iv },
{ "glColor3fv",				tkra_glColor3fv },
{ "glColor3dv",				tkra_glColor3dv },
{ "glColor3ubv",			tkra_glColor3ubv },
{ "glColor3usv",			tkra_glColor3usv },
{ "glColor3uiv",			tkra_glColor3uiv },
{ "glColor4bv",				tkra_glColor4bv },
{ "glColor4sv",				tkra_glColor4sv },
{ "glColor4iv",				tkra_glColor4iv },
{ "glColor4fv",				tkra_glColor4fv },
{ "glColor4dv",				tkra_glColor4dv },
{ "glColor4ubv",			tkra_glColor4ubv },
{ "glColor4usv",			tkra_glColor4usv },
{ "glColor4uiv",			tkra_glColor4uiv },
{ "glTexCoord1s",			tkra_glTexCoord1s },
{ "glTexCoord1i",			tkra_glTexCoord1i },
{ "glTexCoord1f",			tkra_glTexCoord1f },
{ "glTexCoord1d",			tkra_glTexCoord1d },
{ "glTexCoord2s",			tkra_glTexCoord2s },
{ "glTexCoord2i",			tkra_glTexCoord2i },
{ "glTexCoord2f",			tkra_glTexCoord2f },
{ "glTexCoord2d",			tkra_glTexCoord2d },
{ "glTexCoord3s",			tkra_glTexCoord3s },
{ "glTexCoord3i",			tkra_glTexCoord3i },
{ "glTexCoord3f",			tkra_glTexCoord3f },
{ "glTexCoord3d",			tkra_glTexCoord3d },
{ "glTexCoord4s",			tkra_glTexCoord4s },
{ "glTexCoord4i",			tkra_glTexCoord4i },
{ "glTexCoord4f",			tkra_glTexCoord4f },
{ "glTexCoord4d",			tkra_glTexCoord4d },
{ "glTexCoord1sv",			tkra_glTexCoord1sv },
{ "glTexCoord1iv",			tkra_glTexCoord1iv },
{ "glTexCoord1fv",			tkra_glTexCoord1fv },
{ "glTexCoord1dv",			tkra_glTexCoord1dv },
{ "glTexCoord2sv",			tkra_glTexCoord2sv },
{ "glTexCoord2iv",			tkra_glTexCoord2iv },
{ "glTexCoord2fv",			tkra_glTexCoord2fv },
{ "glTexCoord2dv",			tkra_glTexCoord2dv },
{ "glTexCoord3sv",			tkra_glTexCoord3sv },
{ "glTexCoord3iv",			tkra_glTexCoord3iv },
{ "glTexCoord3fv",			tkra_glTexCoord3fv },
{ "glTexCoord3dv",			tkra_glTexCoord3dv },
{ "glTexCoord4sv",			tkra_glTexCoord4sv },
{ "glTexCoord4iv",			tkra_glTexCoord4iv },
{ "glTexCoord4fv",			tkra_glTexCoord4fv },
{ "glTexCoord4dv",			tkra_glTexCoord4dv },
{ "glVertex2s",				tkra_glVertex2s },
{ "glVertex2i",				tkra_glVertex2i },
{ "glVertex2f",				tkra_glVertex2f },
{ "glVertex2d",				tkra_glVertex2d },
{ "glVertex3s",				tkra_glVertex3s },
{ "glVertex3i",				tkra_glVertex3i },
{ "glVertex3f",				tkra_glVertex3f },
{ "glVertex3d",				tkra_glVertex3d },
{ "glVertex4s",				tkra_glVertex4s },
{ "glVertex4i",				tkra_glVertex4i },
{ "glVertex4f",				tkra_glVertex4f },
{ "glVertex4d",				tkra_glVertex4d },
{ "glVertex2sv",			tkra_glVertex2sv },
{ "glVertex2iv",			tkra_glVertex2iv },
{ "glVertex2fv",			tkra_glVertex2fv },
{ "glVertex2dv",			tkra_glVertex2dv },
{ "glVertex3sv",			tkra_glVertex3sv },
{ "glVertex3iv",			tkra_glVertex3iv },
{ "glVertex3fv",			tkra_glVertex3fv },
{ "glVertex3dv",			tkra_glVertex3dv },
{ "glVertex4sv",			tkra_glVertex4sv },
{ "glVertex4iv",			tkra_glVertex4iv },
{ "glVertex4fv",			tkra_glVertex4fv },
{ "glVertex4dv",			tkra_glVertex4dv },
{ "glNormal3b",				tkra_glNormal3b },
{ "glNormal3d",				tkra_glNormal3d },
{ "glNormal3f",				tkra_glNormal3f },
{ "glNormal3i",				tkra_glNormal3i },
{ "glNormal3s",				tkra_glNormal3s },
{ "glNormal3bv",			tkra_glNormal3bv },
{ "glNormal3dv",			tkra_glNormal3dv },
{ "glNormal3fv",			tkra_glNormal3fv },
{ "glNormal3iv",			tkra_glNormal3iv },
{ "glNormal3sv",			tkra_glNormal3sv },

{ "glMatrixMode",			tkra_glMatrixMode },
{ "glOrtho",				tkra_glOrtho },
{ "glFrustum",				tkra_glFrustum },
{ "glViewport",				tkra_glViewport },
{ "glPushMatrix",			tkra_glPushMatrix },
{ "glPopMatrix",			tkra_glPopMatrix },
{ "glLoadIdentity",			tkra_glLoadIdentity },
{ "glLoadMatrixd",			tkra_glLoadMatrixd },
{ "glLoadMatrixf",			tkra_glLoadMatrixf },
{ "glMultMatrixd",			tkra_glMultMatrixd },
{ "glMultMatrixf",			tkra_glMultMatrixf },
{ "glRotated",				tkra_glRotated },
{ "glRotatef",				tkra_glRotatef },
{ "glScaled",				tkra_glScaled },
{ "glScalef",				tkra_glScalef },
{ "glTranslated",			tkra_glTranslated },
{ "glTranslatef",			tkra_glTranslatef },

{ "glClearDepth",			tkra_glClearDepth },
{ "glDepthFunc",			tkra_glDepthFunc },
{ "glDepthMask",			tkra_glDepthMask },
{ "glDepthRange",			tkra_glDepthRange },
{ "glClearIndex",			tkra_glClearIndex },
{ "glClearColor",			tkra_glClearColor },
{ "glClear",				tkra_glClear },
{ "glIndexMask",			tkra_glIndexMask },
{ "glColorMask",			tkra_glColorMask },
{ "glAlphaFunc",			tkra_glAlphaFunc },
{ "glBlendFunc",			tkra_glBlendFunc },
{ "glLogicOp",				tkra_glLogicOp },
{ "glCullFace",				tkra_glCullFace },
{ "glFrontFace",			tkra_glFrontFace },
{ "glPointSize",			tkra_glPointSize },
{ "glLineWidth",			tkra_glLineWidth },
{ "glLineStipple",			tkra_glLineStipple },
{ "glPolygonMode",			tkra_glPolygonMode },
{ "glPolygonOffset",		tkra_glPolygonOffset },
{ "glPolygonStipple",		tkra_glPolygonStipple },
{ "glGetPolygonStipple",	tkra_glGetPolygonStipple },
{ "glEdgeFlag",				tkra_glEdgeFlag },
{ "glEdgeFlagv",			tkra_glEdgeFlagv },
{ "glScissor",				tkra_glScissor },
{ "glClipPlane",			tkra_glClipPlane },
{ "glGetClipPlane",			tkra_glGetClipPlane },
{ "glDrawBuffer",			tkra_glDrawBuffer },
{ "glReadBuffer",			tkra_glReadBuffer },
{ "glEnable",				tkra_glEnable },
{ "glDisable",				tkra_glDisable },
{ "glIsEnabled",			tkra_glIsEnabled },
{ "glEnableClientState",	tkra_glEnableClientState },
{ "glDisableClientState",	tkra_glDisableClientState },
{ "glGetBooleanv",			tkra_glGetBooleanv },
{ "glGetDoublev",			tkra_glGetDoublev },
{ "glGetFloatv",			tkra_glGetFloatv },
{ "glGetIntegerv",			tkra_glGetIntegerv },
{ "glPushAttrib",			tkra_glPushAttrib },
{ "glPopAttrib",			tkra_glPopAttrib },
{ "glPushClientAttrib",		tkra_glPushClientAttrib },
{ "glPopClientAttrib",		tkra_glPopClientAttrib },
{ "glRenderMode",			tkra_glRenderMode },
{ "glGetError",				tkra_glGetError },
{ "glGetString",			tkra_glGetString },
{ "glFinish",				tkra_glFinish },
{ "glFlush",				tkra_glFlush },
{ "glHint",					tkra_glHint },

{ "glLightf",				tkra_glLightf },
{ "glLighti",				tkra_glLighti },
{ "glLightfv",				tkra_glLightfv },
{ "glLightiv",				tkra_glLightiv },
{ "glGetLightfv",			tkra_glGetLightfv },
{ "glGetLightiv",			tkra_glGetLightiv },
{ "glLightModelf",			tkra_glLightModelf },
{ "glLightModeli",			tkra_glLightModeli },
{ "glLightModelfv",			tkra_glLightModelfv },
{ "glLightModeliv",			tkra_glLightModeliv },
{ "glMaterialf",			tkra_glMaterialf },
{ "glMateriali",			tkra_glMateriali },
{ "glMaterialfv",			tkra_glMaterialfv },
{ "glMaterialiv",			tkra_glMaterialiv },
{ "glGetMaterialfv",		tkra_glGetMaterialfv },
{ "glGetMaterialiv",		tkra_glGetMaterialiv },
{ "glColorMaterial",		tkra_glColorMaterial },

{ "glStencilFunc",			tkra_glStencilFunc },
{ "glStencilMask",			tkra_glStencilMask },
{ "glStencilOp",			tkra_glStencilOp },
{ "glClearStencil",			tkra_glClearStencil },

{ "glMultiTexCoord1s",		tkra_glMultiTexCoord1s },
{ "glMultiTexCoord1i",		tkra_glMultiTexCoord1i },
{ "glMultiTexCoord1f",		tkra_glMultiTexCoord1f },
{ "glMultiTexCoord1d",		tkra_glMultiTexCoord1d },
{ "glMultiTexCoord2s",		tkra_glMultiTexCoord2s },
{ "glMultiTexCoord2i",		tkra_glMultiTexCoord2i },
{ "glMultiTexCoord2f",		tkra_glMultiTexCoord2f },
{ "glMultiTexCoord2d",		tkra_glMultiTexCoord2d },
{ "glMultiTexCoord3s",		tkra_glMultiTexCoord3s },
{ "glMultiTexCoord3i",		tkra_glMultiTexCoord3i },
{ "glMultiTexCoord3f",		tkra_glMultiTexCoord3f },
{ "glMultiTexCoord3d",		tkra_glMultiTexCoord3d },
{ "glMultiTexCoord4s",		tkra_glMultiTexCoord4s },
{ "glMultiTexCoord4i",		tkra_glMultiTexCoord4i },
{ "glMultiTexCoord4f",		tkra_glMultiTexCoord4f },
{ "glMultiTexCoord4d",		tkra_glMultiTexCoord4d },

{ "glMultiTexCoord1sv",		tkra_glMultiTexCoord1sv },
{ "glMultiTexCoord1iv",		tkra_glMultiTexCoord1iv },
{ "glMultiTexCoord1fv",		tkra_glMultiTexCoord1fv },
{ "glMultiTexCoord1dv",		tkra_glMultiTexCoord1dv },
{ "glMultiTexCoord2sv",		tkra_glMultiTexCoord2sv },
{ "glMultiTexCoord2iv",		tkra_glMultiTexCoord2iv },
{ "glMultiTexCoord2fv",		tkra_glMultiTexCoord2fv },
{ "glMultiTexCoord2dv",		tkra_glMultiTexCoord2dv },
{ "glMultiTexCoord3sv",		tkra_glMultiTexCoord3sv },
{ "glMultiTexCoord3iv",		tkra_glMultiTexCoord3iv },
{ "glMultiTexCoord3fv",		tkra_glMultiTexCoord3fv },
{ "glMultiTexCoord3dv",		tkra_glMultiTexCoord3dv },
{ "glMultiTexCoord4sv",		tkra_glMultiTexCoord4sv },
{ "glMultiTexCoord4iv",		tkra_glMultiTexCoord4iv },
{ "glMultiTexCoord4fv",		tkra_glMultiTexCoord4fv },
{ "glMultiTexCoord4dv",		tkra_glMultiTexCoord4dv },

{ "glActiveTexture",		tkra_glActiveTexture },

{ "glMTexCoord2fSGIS",		tkra_glMultiTexCoord2f },
{ "glSelectTextureSGIS",	tkra_glActiveTexture },

{ NULL,						NULL }
};


short	tkra_glfuncs_chn[512];
short	tkra_glfuncs_hash[64];
int		tkra_glfuncs_init=0;

int TKRA_GpaHashName(char *name)
{
	byte *cs;
	int h;
	
	cs=(byte *)name; h=0;
	while(*cs)
		h=h*251+(*cs++);
	h=(h>>8)&63;
	return(h);
}

void *tkra_wglGetProcAddress(char *name)
{
	int i, h;
	
	if(!tkra_glfuncs_init)
	{
		for(i=0; i<64; i++)
			tkra_glfuncs_hash[i]=(-1);

		for(i=0; tkra_glfuncs[i].name; i++)
		{
			h=TKRA_GpaHashName(tkra_glfuncs[i].name);
			tkra_glfuncs_chn[i]=tkra_glfuncs_hash[h];
			tkra_glfuncs_hash[h]=i;
		}
		tkra_glfuncs_init=1;
	}
	
	h=TKRA_GpaHashName(name);
	i=tkra_glfuncs_hash[h];
	while(i>=0)
	{
		if(!strcmp(tkra_glfuncs[i].name, name))
			return(tkra_glfuncs[i].func);
		i=tkra_glfuncs_chn[i];
	}
	return(NULL);
}
