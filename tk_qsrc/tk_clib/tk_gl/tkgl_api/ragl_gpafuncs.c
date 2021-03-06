struct tkgl_gpafunc_s
{
char *name;
void *fcn;
short chain;
}tkgl_gpafunc[]=
{
{"glClearIndex", GlDrv_glClearIndex, -1},
{"glClearColor", GlDrv_glClearColor, -1},
{"glClear", GlDrv_glClear, -1},
{"glIndexMask", GlDrv_glIndexMask, -1},
{"glColorMask", GlDrv_glColorMask, -1},
{"glAlphaFunc", GlDrv_glAlphaFunc, -1},
{"glBlendFunc", GlDrv_glBlendFunc, -1},
{"glLogicOp", GlDrv_glLogicOp, -1},
{"glCullFace", GlDrv_glCullFace, -1},
{"glFrontFace", GlDrv_glFrontFace, 1},
{"glPointSize", GlDrv_glPointSize, 8},
{"glLineWidth", GlDrv_glLineWidth, -1},
{"glLineStipple", GlDrv_glLineStipple, -1},
{"glPolygonMode", GlDrv_glPolygonMode, -1},
{"glPolygonOffset", GlDrv_glPolygonOffset, -1},
{"glPolygonStipple", GlDrv_glPolygonStipple, -1},
{"glGetPolygonStipple", GlDrv_glGetPolygonStipple, 7},
{"glEdgeFlag", GlDrv_glEdgeFlag, -1},
{"glEdgeFlagv", GlDrv_glEdgeFlagv, 5},
{"glScissor", GlDrv_glScissor, -1},
{"glClipPlane", GlDrv_glClipPlane, -1},
{"glGetClipPlane", GlDrv_glGetClipPlane, -1},
{"glDrawBuffer", GlDrv_glDrawBuffer, -1},
{"glReadBuffer", GlDrv_glReadBuffer, -1},
{"glEnable", GlDrv_glEnable, -1},
{"glDisable", GlDrv_glDisable, 0},
{"glEnableClientState", GlDrv_glEnableClientState, -1},
{"glDisableClientState", GlDrv_glDisableClientState, -1},
{"glGetBooleanv", GlDrv_glGetBooleanv, -1},
{"glGetDoublev", GlDrv_glGetDoublev, -1},
{"glGetFloatv", GlDrv_glGetFloatv, -1},
{"glGetIntegerv", GlDrv_glGetIntegerv, -1},
{"glPushAttrib", GlDrv_glPushAttrib, 27},
{"glPopAttrib", GlDrv_glPopAttrib, -1},
{"glPushClientAttrib", GlDrv_glPushClientAttrib, -1},
{"glPopClientAttrib", GlDrv_glPopClientAttrib, -1},
{"glFinish", GlDrv_glFinish, -1},
{"glFlush", GlDrv_glFlush, -1},
{"glHint", GlDrv_glHint, 19},
{"glClearDepth", GlDrv_glClearDepth, -1},
{"glDepthFunc", GlDrv_glDepthFunc, -1},
{"glDepthMask", GlDrv_glDepthMask, 21},
{"glDepthRange", GlDrv_glDepthRange, 4},
{"glClearAccum", GlDrv_glClearAccum, -1},
{"glAccum", GlDrv_glAccum, 23},
{"glMatrixMode", GlDrv_glMatrixMode, -1},
{"glOrtho", GlDrv_glOrtho, -1},
{"glFrustum", GlDrv_glFrustum, 37},
{"glViewport", GlDrv_glViewport, -1},
{"glPushMatrix", GlDrv_glPushMatrix, 41},
{"glPopMatrix", GlDrv_glPopMatrix, 45},
{"glLoadIdentity", GlDrv_glLoadIdentity, 12},
{"glLoadMatrixd", GlDrv_glLoadMatrixd, 38},
{"glLoadMatrixf", GlDrv_glLoadMatrixf, 50},
{"glMultMatrixd", GlDrv_glMultMatrixd, -1},
{"glMultMatrixf", GlDrv_glMultMatrixf, 34},
{"glRotated", GlDrv_glRotated, 48},
{"glRotatef", GlDrv_glRotatef, 43},
{"glScaled", GlDrv_glScaled, 32},
{"glScalef", GlDrv_glScalef, 6},
{"glTranslated", GlDrv_glTranslated, 49},
{"glTranslatef", GlDrv_glTranslatef, -1},
{"glDeleteLists", GlDrv_glDeleteLists, 42},
{"glNewList", GlDrv_glNewList, 60},
{"glEndList", GlDrv_glEndList, -1},
{"glCallList", GlDrv_glCallList, -1},
{"glCallLists", GlDrv_glCallLists, 18},
{"glListBase", GlDrv_glListBase, -1},
{"glBegin", GlDrv_glBegin, 54},
{"glEnd", GlDrv_glEnd, 44},
{"glVertex2d", GlDrv_glVertex2d, 62},
{"glVertex2f", GlDrv_glVertex2f, 35},
{"glVertex2i", GlDrv_glVertex2i, 52},
{"glVertex2s", GlDrv_glVertex2s, -1},
{"glVertex3d", GlDrv_glVertex3d, 47},
{"glVertex3f", GlDrv_glVertex3f, -1},
{"glVertex3i", GlDrv_glVertex3i, 57},
{"glVertex3s", GlDrv_glVertex3s, -1},
{"glVertex4d", GlDrv_glVertex4d, 58},
{"glVertex4f", GlDrv_glVertex4f, 59},
{"glVertex4i", GlDrv_glVertex4i, 68},
{"glVertex4s", GlDrv_glVertex4s, 39},
{"glVertex2dv", GlDrv_glVertex2dv, -1},
{"glVertex2fv", GlDrv_glVertex2fv, 20},
{"glVertex2iv", GlDrv_glVertex2iv, 61},
{"glVertex2sv", GlDrv_glVertex2sv, 53},
{"glVertex3dv", GlDrv_glVertex3dv, 30},
{"glVertex3fv", GlDrv_glVertex3fv, -1},
{"glVertex3iv", GlDrv_glVertex3iv, -1},
{"glVertex3sv", GlDrv_glVertex3sv, 79},
{"glVertex4dv", GlDrv_glVertex4dv, 33},
{"glVertex4fv", GlDrv_glVertex4fv, 22},
{"glVertex4iv", GlDrv_glVertex4iv, 24},
{"glVertex4sv", GlDrv_glVertex4sv, 14},
{"glNormal3b", GlDrv_glNormal3b, 67},
{"glNormal3d", GlDrv_glNormal3d, 65},
{"glNormal3f", GlDrv_glNormal3f, -1},
{"glNormal3i", GlDrv_glNormal3i, 85},
{"glNormal3s", GlDrv_glNormal3s, 89},
{"glNormal3bv", GlDrv_glNormal3bv, -1},
{"glNormal3dv", GlDrv_glNormal3dv, 71},
{"glNormal3fv", GlDrv_glNormal3fv, 98},
{"glNormal3iv", GlDrv_glNormal3iv, 78},
{"glNormal3sv", GlDrv_glNormal3sv, 56},
{"glIndexd", GlDrv_glIndexd, 25},
{"glIndexf", GlDrv_glIndexf, -1},
{"glIndexi", GlDrv_glIndexi, 31},
{"glIndexs", GlDrv_glIndexs, 81},
{"glIndexub", GlDrv_glIndexub, -1},
{"glIndexdv", GlDrv_glIndexdv, 86},
{"glIndexfv", GlDrv_glIndexfv, 28},
{"glIndexiv", GlDrv_glIndexiv, 88},
{"glIndexsv", GlDrv_glIndexsv, 105},
{"glIndexubv", GlDrv_glIndexubv, 82},
{"glColor3b", GlDrv_glColor3b, 93},
{"glColor3d", GlDrv_glColor3d, 113},
{"glColor3f", GlDrv_glColor3f, 69},
{"glColor3i", GlDrv_glColor3i, 11},
{"glColor3s", GlDrv_glColor3s, 74},
{"glColor3ub", GlDrv_glColor3ub, 9},
{"glColor3ui", GlDrv_glColor3ui, 29},
{"glColor3us", GlDrv_glColor3us, -1},
{"glColor4b", GlDrv_glColor4b, 120},
{"glColor4d", GlDrv_glColor4d, 94},
{"glColor4f", GlDrv_glColor4f, 95},
{"glColor4i", GlDrv_glColor4i, 3},
{"glColor4s", GlDrv_glColor4s, 104},
{"glColor4ub", GlDrv_glColor4ub, 97},
{"glColor4ui", GlDrv_glColor4ui, 121},
{"glColor4us", GlDrv_glColor4us, 108},
{"glColor3bv", GlDrv_glColor3bv, 119},
{"glColor3dv", GlDrv_glColor3dv, 2},
{"glColor3fv", GlDrv_glColor3fv, -1},
{"glColor3iv", GlDrv_glColor3iv, 116},
{"glColor3sv", GlDrv_glColor3sv, 17},
{"glColor3ubv", GlDrv_glColor3ubv, 46},
{"glColor3uiv", GlDrv_glColor3uiv, 77},
{"glColor3usv", GlDrv_glColor3usv, 129},
{"glColor4bv", GlDrv_glColor4bv, 127},
{"glColor4dv", GlDrv_glColor4dv, 55},
{"glColor4fv", GlDrv_glColor4fv, 76},
{"glColor4iv", GlDrv_glColor4iv, 103},
{"glColor4sv", GlDrv_glColor4sv, 123},
{"glColor4ubv", GlDrv_glColor4ubv, -1},
{"glColor4uiv", GlDrv_glColor4uiv, -1},
{"glColor4usv", GlDrv_glColor4usv, 137},
{"glTexCoord1d", GlDrv_glTexCoord1d, 16},
{"glTexCoord1f", GlDrv_glTexCoord1f, -1},
{"glTexCoord1i", GlDrv_glTexCoord1i, -1},
{"glTexCoord1s", GlDrv_glTexCoord1s, 110},
{"glTexCoord2d", GlDrv_glTexCoord2d, 139},
{"glTexCoord2f", GlDrv_glTexCoord2f, -1},
{"glTexCoord2i", GlDrv_glTexCoord2i, 114},
{"glTexCoord2s", GlDrv_glTexCoord2s, -1},
{"glTexCoord3d", GlDrv_glTexCoord3d, 149},
{"glTexCoord3f", GlDrv_glTexCoord3f, 92},
{"glTexCoord3i", GlDrv_glTexCoord3i, 96},
{"glTexCoord3s", GlDrv_glTexCoord3s, 15},
{"glTexCoord4d", GlDrv_glTexCoord4d, 140},
{"glTexCoord4f", GlDrv_glTexCoord4f, 146},
{"glTexCoord4i", GlDrv_glTexCoord4i, 99},
{"glTexCoord4s", GlDrv_glTexCoord4s, 111},
{"glTexCoord1dv", GlDrv_glTexCoord1dv, 118},
{"glTexCoord1fv", GlDrv_glTexCoord1fv, 144},
{"glTexCoord1iv", GlDrv_glTexCoord1iv, 134},
{"glTexCoord1sv", GlDrv_glTexCoord1sv, 26},
{"glTexCoord2dv", GlDrv_glTexCoord2dv, 10},
{"glTexCoord2fv", GlDrv_glTexCoord2fv, 124},
{"glTexCoord2iv", GlDrv_glTexCoord2iv, 142},
{"glTexCoord2sv", GlDrv_glTexCoord2sv, 106},
{"glTexCoord3dv", GlDrv_glTexCoord3dv, 130},
{"glTexCoord3fv", GlDrv_glTexCoord3fv, 131},
{"glTexCoord3iv", GlDrv_glTexCoord3iv, 36},
{"glTexCoord3sv", GlDrv_glTexCoord3sv, 160},
{"glTexCoord4dv", GlDrv_glTexCoord4dv, 138},
{"glTexCoord4fv", GlDrv_glTexCoord4fv, 117},
{"glTexCoord4iv", GlDrv_glTexCoord4iv, 80},
{"glTexCoord4sv", GlDrv_glTexCoord4sv, 161},
{"glRasterPos2d", GlDrv_glRasterPos2d, 166},
{"glRasterPos2f", GlDrv_glRasterPos2f, 148},
{"glRasterPos2i", GlDrv_glRasterPos2i, 136},
{"glRasterPos2s", GlDrv_glRasterPos2s, 100},
{"glRasterPos3d", GlDrv_glRasterPos3d, 133},
{"glRasterPos3f", GlDrv_glRasterPos3f, 152},
{"glRasterPos3i", GlDrv_glRasterPos3i, 107},
{"glRasterPos3s", GlDrv_glRasterPos3s, 75},
{"glRasterPos4d", GlDrv_glRasterPos4d, 167},
{"glRasterPos4f", GlDrv_glRasterPos4f, 156},
{"glRasterPos4i", GlDrv_glRasterPos4i, -1},
{"glRasterPos4s", GlDrv_glRasterPos4s, 101},
{"glRasterPos2dv", GlDrv_glRasterPos2dv, 40},
{"glRasterPos2fv", GlDrv_glRasterPos2fv, 183},
{"glRasterPos2iv", GlDrv_glRasterPos2iv, 143},
{"glRasterPos2sv", GlDrv_glRasterPos2sv, 164},
{"glRasterPos3dv", GlDrv_glRasterPos3dv, 150},
{"glRasterPos3fv", GlDrv_glRasterPos3fv, 158},
{"glRasterPos3iv", GlDrv_glRasterPos3iv, -1},
{"glRasterPos3sv", GlDrv_glRasterPos3sv, 168},
{"glRasterPos4dv", GlDrv_glRasterPos4dv, -1},
{"glRasterPos4fv", GlDrv_glRasterPos4fv, 180},
{"glRasterPos4iv", GlDrv_glRasterPos4iv, 51},
{"glRasterPos4sv", GlDrv_glRasterPos4sv, 172},
{"glRectd", GlDrv_glRectd, 66},
{"glRectf", GlDrv_glRectf, 193},
{"glRecti", GlDrv_glRecti, 154},
{"glRects", GlDrv_glRects, 91},
{"glRectdv", GlDrv_glRectdv, 73},
{"glRectfv", GlDrv_glRectfv, 184},
{"glRectiv", GlDrv_glRectiv, 194},
{"glRectsv", GlDrv_glRectsv, 87},
{"glShadeModel", GlDrv_glShadeModel, 153},
{"glLightf", GlDrv_glLightf, 128},
{"glLighti", GlDrv_glLighti, 125},
{"glLightfv", GlDrv_glLightfv, 192},
{"glLightiv", GlDrv_glLightiv, 189},
{"glGetLightfv", GlDrv_glGetLightfv, 202},
{"glGetLightiv", GlDrv_glGetLightiv, 147},
{"glLightModelf", GlDrv_glLightModelf, 165},
{"glLightModeli", GlDrv_glLightModeli, 135},
{"glLightModelfv", GlDrv_glLightModelfv, 210},
{"glLightModeliv", GlDrv_glLightModeliv, 196},
{"glMaterialf", GlDrv_glMaterialf, 109},
{"glMateriali", GlDrv_glMateriali, 84},
{"glMaterialfv", GlDrv_glMaterialfv, 157},
{"glMaterialiv", GlDrv_glMaterialiv, 171},
{"glGetMaterialfv", GlDrv_glGetMaterialfv, 141},
{"glGetMaterialiv", GlDrv_glGetMaterialiv, 221},
{"glColorMaterial", GlDrv_glColorMaterial, 204},
{"glPixelZoom", GlDrv_glPixelZoom, 217},
{"glPixelStoref", GlDrv_glPixelStoref, 206},
{"glPixelStorei", GlDrv_glPixelStorei, 190},
{"glPixelTransferf", GlDrv_glPixelTransferf, 212},
{"glPixelTransferi", GlDrv_glPixelTransferi, 211},
{"glPixelMapfv", GlDrv_glPixelMapfv, 162},
{"glPixelMapuiv", GlDrv_glPixelMapuiv, 170},
{"glPixelMapusv", GlDrv_glPixelMapusv, 115},
{"glGetPixelMapfv", GlDrv_glGetPixelMapfv, 122},
{"glGetPixelMapuiv", GlDrv_glGetPixelMapuiv, 233},
{"glGetPixelMapusv", GlDrv_glGetPixelMapusv, 229},
{"glBitmap", GlDrv_glBitmap, 238},
{"glReadPixels", GlDrv_glReadPixels, 200},
{"glDrawPixels", GlDrv_glDrawPixels, 112},
{"glCopyPixels", GlDrv_glCopyPixels, 179},
{"glStencilFunc", GlDrv_glStencilFunc, 176},
{"glStencilMask", GlDrv_glStencilMask, 209},
{"glStencilOp", GlDrv_glStencilOp, 13},
{"glClearStencil", GlDrv_glClearStencil, 83},
{"glTexGend", GlDrv_glTexGend, 174},
{"glTexGenf", GlDrv_glTexGenf, 239},
{"glTexGeni", GlDrv_glTexGeni, 224},
{"glTexGendv", GlDrv_glTexGendv, 151},
{"glTexGenfv", GlDrv_glTexGenfv, 240},
{"glTexGeniv", GlDrv_glTexGeniv, 195},
{"glGetTexGendv", GlDrv_glGetTexGendv, 223},
{"glGetTexGenfv", GlDrv_glGetTexGenfv, 225},
{"glGetTexGeniv", GlDrv_glGetTexGeniv, 132},
{"glTexEnvf", GlDrv_glTexEnvf, 90},
{"glTexEnvi", GlDrv_glTexEnvi, 64},
{"glTexEnvfv", GlDrv_glTexEnvfv, 222},
{"glTexEnviv", GlDrv_glTexEnviv, 175},
{"glGetTexEnvfv", GlDrv_glGetTexEnvfv, 169},
{"glGetTexEnviv", GlDrv_glGetTexEnviv, 145},
{"glTexParameterf", GlDrv_glTexParameterf, 259},
{"glTexParameteri", GlDrv_glTexParameteri, 182},
{"glTexParameterfv", GlDrv_glTexParameterfv, 205},
{"glTexParameteriv", GlDrv_glTexParameteriv, 245},
{"glGetTexParameterfv", GlDrv_glGetTexParameterfv, 262},
{"glGetTexParameteriv", GlDrv_glGetTexParameteriv, 260},
{"glGetTexLevelParameterfv", GlDrv_glGetTexLevelParameterfv, 230},
{"glGetTexLevelParameteriv", GlDrv_glGetTexLevelParameteriv, 236},
{"glTexImage1D", GlDrv_glTexImage1D, 252},
{"glTexImage2D", GlDrv_glTexImage2D, 253},
{"glGetTexImage", GlDrv_glGetTexImage, 242},
{"glMap1d", GlDrv_glMap1d, 155},
{"glMap1f", GlDrv_glMap1f, 264},
{"glMap2d", GlDrv_glMap2d, 226},
{"glMap2f", GlDrv_glMap2f, 254},
{"glGetMapdv", GlDrv_glGetMapdv, 198},
{"glGetMapfv", GlDrv_glGetMapfv, 199},
{"glGetMapiv", GlDrv_glGetMapiv, 251},
{"glEvalCoord1d", GlDrv_glEvalCoord1d, 207},
{"glEvalCoord1f", GlDrv_glEvalCoord1f, 277},
{"glEvalCoord1dv", GlDrv_glEvalCoord1dv, 197},
{"glEvalCoord1fv", GlDrv_glEvalCoord1fv, 126},
{"glEvalCoord2d", GlDrv_glEvalCoord2d, 247},
{"glEvalCoord2f", GlDrv_glEvalCoord2f, 248},
{"glEvalCoord2dv", GlDrv_glEvalCoord2dv, 201},
{"glEvalCoord2fv", GlDrv_glEvalCoord2fv, 263},
{"glMapGrid1d", GlDrv_glMapGrid1d, 283},
{"glMapGrid1f", GlDrv_glMapGrid1f, 241},
{"glMapGrid2d", GlDrv_glMapGrid2d, 269},
{"glMapGrid2f", GlDrv_glMapGrid2f, 282},
{"glEvalPoint1", GlDrv_glEvalPoint1, 281},
{"glEvalPoint2", GlDrv_glEvalPoint2, 275},
{"glEvalMesh1", GlDrv_glEvalMesh1, 216},
{"glEvalMesh2", GlDrv_glEvalMesh2, 178},
{"glFogf", GlDrv_glFogf, 231},
{"glFogi", GlDrv_glFogi, 271},
{"glFogfv", GlDrv_glFogfv, 286},
{"glFogiv", GlDrv_glFogiv, 232},
{"glFeedbackBuffer", GlDrv_glFeedbackBuffer, 287},
{"glPassThrough", GlDrv_glPassThrough, 256},
{"glSelectBuffer", GlDrv_glSelectBuffer, 249},
{"glInitNames", GlDrv_glInitNames, 187},
{"glLoadName", GlDrv_glLoadName, 278},
{"glPushName", GlDrv_glPushName, 213},
{"glPopName", GlDrv_glPopName, 284},
{"glGenTextures", GlDrv_glGenTextures, 303},
{"glDeleteTextures", GlDrv_glDeleteTextures, 266},
{"glBindTexture", GlDrv_glBindTexture, 177},
{"glPrioritizeTextures", GlDrv_glPrioritizeTextures, 273},
{"glTexSubImage1D", GlDrv_glTexSubImage1D, 257},
{"glTexSubImage2D", GlDrv_glTexSubImage2D, 289},
{"glCopyTexImage1D", GlDrv_glCopyTexImage1D, 163},
{"glCopyTexImage2D", GlDrv_glCopyTexImage2D, 258},
{"glCopyTexSubImage1D", GlDrv_glCopyTexSubImage1D, 173},
{"glCopyTexSubImage2D", GlDrv_glCopyTexSubImage2D, 218},
{"glVertexPointer", GlDrv_glVertexPointer, 265},
{"glNormalPointer", GlDrv_glNormalPointer, 203},
{"glColorPointer", GlDrv_glColorPointer, 309},
{"glIndexPointer", GlDrv_glIndexPointer, 307},
{"glTexCoordPointer", GlDrv_glTexCoordPointer, 181},
{"glEdgeFlagPointer", GlDrv_glEdgeFlagPointer, 299},
{"glGetPointerv", GlDrv_glGetPointerv, 314},
{"glArrayElement", GlDrv_glArrayElement, 288},
{"glDrawArrays", GlDrv_glDrawArrays, 261},
{"glDrawElements", GlDrv_glDrawElements, 306},
{"glInterleavedArrays", GlDrv_glInterleavedArrays, 244},
{"glDrawRangeElements", GlDrv_glDrawRangeElements, 250},
{"glTexImage3D", GlDrv_glTexImage3D, 234},
{"glTexSubImage3D", GlDrv_glTexSubImage3D, 291},
{"glCopyTexSubImage3D", GlDrv_glCopyTexSubImage3D, 321},
{"glColorTable", GlDrv_glColorTable, 214},
{"glColorSubTable", GlDrv_glColorSubTable, 208},
{"glColorTableParameteriv", GlDrv_glColorTableParameteriv, 268},
{"glColorTableParameterfv", GlDrv_glColorTableParameterfv, 322},
{"glCopyColorSubTable", GlDrv_glCopyColorSubTable, 325},
{"glCopyColorTable", GlDrv_glCopyColorTable, 255},
{"glGetColorTable", GlDrv_glGetColorTable, 237},
{"glGetColorTableParameterfv", GlDrv_glGetColorTableParameterfv, 338},
{"glGetColorTableParameteriv", GlDrv_glGetColorTableParameteriv, 323},
{"glBlendEquation", GlDrv_glBlendEquation, 274},
{"glBlendColor", GlDrv_glBlendColor, 318},
{"glHistogram", GlDrv_glHistogram, 293},
{"glResetHistogram", GlDrv_glResetHistogram, 272},
{"glGetHistogram", GlDrv_glGetHistogram, 235},
{"glGetHistogramParameterfv", GlDrv_glGetHistogramParameterfv, 316},
{"glGetHistogramParameteriv", GlDrv_glGetHistogramParameteriv, 332},
{"glMinmax", GlDrv_glMinmax, 243},
{"glResetMinmax", GlDrv_glResetMinmax, 294},
{"glGetMinmax", GlDrv_glGetMinmax, 334},
{"glGetMinmaxParameterfv", GlDrv_glGetMinmaxParameterfv, 327},
{"glGetMinmaxParameteriv", GlDrv_glGetMinmaxParameteriv, 311},
{"glConvolutionFilter1D", GlDrv_glConvolutionFilter1D, 279},
{"glConvolutionFilter2D", GlDrv_glConvolutionFilter2D, 333},
{"glConvolutionParameterf", GlDrv_glConvolutionParameterf, 350},
{"glConvolutionParameterfv", GlDrv_glConvolutionParameterfv, 312},
{"glConvolutionParameteri", GlDrv_glConvolutionParameteri, 285},
{"glConvolutionParameteriv", GlDrv_glConvolutionParameteriv, 324},
{"glCopyConvolutionFilter1D", GlDrv_glCopyConvolutionFilter1D, 331},
{"glCopyConvolutionFilter2D", GlDrv_glCopyConvolutionFilter2D, 185},
{"glGetConvolutionFilter", GlDrv_glGetConvolutionFilter, 191},
{"glGetConvolutionParameterfv", GlDrv_glGetConvolutionParameterfv, 295},
{"glGetConvolutionParameteriv", GlDrv_glGetConvolutionParameteriv, 352},
{"glSeparableFilter2D", GlDrv_glSeparableFilter2D, 298},
{"glGetSeparableFilter", GlDrv_glGetSeparableFilter, 297},
{"glActiveTexture", GlDrv_glActiveTexture, 313},
{"glClientActiveTexture", GlDrv_glClientActiveTexture, 246},
{"glCompressedTexImage1D", GlDrv_glCompressedTexImage1D, 326},
{"glCompressedTexImage2D", GlDrv_glCompressedTexImage2D, 353},
{"glCompressedTexImage3D", GlDrv_glCompressedTexImage3D, 354},
{"glCompressedTexSubImage1D", GlDrv_glCompressedTexSubImage1D, 220},
{"glCompressedTexSubImage2D", GlDrv_glCompressedTexSubImage2D, 355},
{"glCompressedTexSubImage3D", GlDrv_glCompressedTexSubImage3D, 344},
{"glGetCompressedTexImage", GlDrv_glGetCompressedTexImage, 267},
{"glMultiTexCoord1d", GlDrv_glMultiTexCoord1d, 186},
{"glMultiTexCoord1dv", GlDrv_glMultiTexCoord1dv, 276},
{"glMultiTexCoord1f", GlDrv_glMultiTexCoord1f, 320},
{"glMultiTexCoord1fv", GlDrv_glMultiTexCoord1fv, 317},
{"glMultiTexCoord1i", GlDrv_glMultiTexCoord1i, 188},
{"glMultiTexCoord1iv", GlDrv_glMultiTexCoord1iv, 351},
{"glMultiTexCoord1s", GlDrv_glMultiTexCoord1s, 356},
{"glMultiTexCoord1sv", GlDrv_glMultiTexCoord1sv, 346},
{"glMultiTexCoord2d", GlDrv_glMultiTexCoord2d, 363},
{"glMultiTexCoord2dv", GlDrv_glMultiTexCoord2dv, 369},
{"glMultiTexCoord2f", GlDrv_glMultiTexCoord2f, 362},
{"glMultiTexCoord2fv", GlDrv_glMultiTexCoord2fv, 72},
{"glMultiTexCoord2i", GlDrv_glMultiTexCoord2i, 384},
{"glMultiTexCoord2iv", GlDrv_glMultiTexCoord2iv, 341},
{"glMultiTexCoord2s", GlDrv_glMultiTexCoord2s, 70},
{"glMultiTexCoord2sv", GlDrv_glMultiTexCoord2sv, 345},
{"glMultiTexCoord3d", GlDrv_glMultiTexCoord3d, 390},
{"glMultiTexCoord3dv", GlDrv_glMultiTexCoord3dv, 389},
{"glMultiTexCoord3f", GlDrv_glMultiTexCoord3f, 300},
{"glMultiTexCoord3fv", GlDrv_glMultiTexCoord3fv, 102},
{"glMultiTexCoord3i", GlDrv_glMultiTexCoord3i, 308},
{"glMultiTexCoord3iv", GlDrv_glMultiTexCoord3iv, 379},
{"glMultiTexCoord3s", GlDrv_glMultiTexCoord3s, 339},
{"glMultiTexCoord3sv", GlDrv_glMultiTexCoord3sv, 159},
{"glMultiTexCoord4d", GlDrv_glMultiTexCoord4d, 330},
{"glMultiTexCoord4dv", GlDrv_glMultiTexCoord4dv, 377},
{"glMultiTexCoord4f", GlDrv_glMultiTexCoord4f, 375},
{"glMultiTexCoord4fv", GlDrv_glMultiTexCoord4fv, 393},
{"glMultiTexCoord4i", GlDrv_glMultiTexCoord4i, 296},
{"glMultiTexCoord4iv", GlDrv_glMultiTexCoord4iv, 381},
{"glMultiTexCoord4s", GlDrv_glMultiTexCoord4s, 394},
{"glMultiTexCoord4sv", GlDrv_glMultiTexCoord4sv, 215},
{"glLoadTransposeMatrixd", GlDrv_glLoadTransposeMatrixd, 337},
{"glLoadTransposeMatrixf", GlDrv_glLoadTransposeMatrixf, 371},
{"glMultTransposeMatrixd", GlDrv_glMultTransposeMatrixd, 388},
{"glMultTransposeMatrixf", GlDrv_glMultTransposeMatrixf, 386},
{"glSampleCoverage", GlDrv_glSampleCoverage, 358},
{"glSamplePass", GlDrv_glSamplePass, 301},
{"wglCopyContext", GlDrv_wglCopyContext, 370},
{"wglCreateContext", GlDrv_wglCreateContext, 373},
{"wglCreateLayerContext", GlDrv_wglCreateLayerContext, 402},
{"wglDeleteContext", GlDrv_wglDeleteContext, 343},
{"wglDescribeLayerPlane", GlDrv_wglDescribeLayerPlane, 366},
{"wglGetCurrentContext", GlDrv_wglGetCurrentContext, 329},
{"wglGetCurrentDC", GlDrv_wglGetCurrentDC, 378},
{"wglGetLayerPaletteEntries", GlDrv_wglGetLayerPaletteEntries, 417},
{"wglGetProcAddress", GlDrv_wglGetProcAddress, 406},
{"wglMakeCurrent", GlDrv_wglMakeCurrent, 408},
{"wglRealizeLayerPalette", GlDrv_wglRealizeLayerPalette, 328},
{"wglSetLayerPaletteEntries", GlDrv_wglSetLayerPaletteEntries, 376},
{"wglShareLists", GlDrv_wglShareLists, 385},
{"wglSwapLayerBuffers", GlDrv_wglSwapLayerBuffers, 396},
{"wglUseFontBitmapsA", GlDrv_wglUseFontBitmapsA, 368},
{"wglUseFontBitmapsW", GlDrv_wglUseFontBitmapsW, 391},
{"wglUseFontOutlinesA", GlDrv_wglUseFontOutlinesA, 422},
{"wglUseFontOutlinesW", GlDrv_wglUseFontOutlinesW, 342},
{"wglChoosePixelFormat", GlDrv_wglChoosePixelFormat, 424},
{"wglGetPixelFormat", GlDrv_wglGetPixelFormat, 429},
{"wglSetPixelFormat", GlDrv_wglSetPixelFormat, 304},
{"wglDescribePixelFormat", GlDrv_wglDescribePixelFormat, 228},
{"wglSwapBuffers", GlDrv_wglSwapBuffers, 349},
{NULL, NULL}
};

int tkgl_gpahash[64]={
310, 419, 414, 367, 397, 292, 374, 348, 365, 335, 401, 359, 430, 387, 398, 416, 
407, 361, 411, 405, 357, 409, 418, 319, 383, 360, 63, 395, 413, 434, 364, 400, 
290, 420, 433, 435, 431, 432, 428, 404, 347, 403, 410, 227, 380, 425, 315, 340, 
280, 423, 336, 302, 372, 399, 392, 421, 412, 270, 219, 415, 305, 382, 426, 427
};

int TKGL_GpaHash(char *str)
{
	char *s;
	int i, h;
	
	s=str; h=1;
	while(*s)h=h*251+(*s++);
	h=h*251+1;
	h=h*251+1;
	h=h*251+1;
	return((h>>8)&63);
}

void *TKGL_GetProcAddress(char *name)
{
	struct tkgl_gpafunc_s *gpa;
	int h, i;
	
	h=TKGL_GpaHash(name);
	i=tkgl_gpahash[h];
	while(i>=0)
	{
		gpa=tkgl_gpafunc+i;
		if(!strcmp(gpa->name, name))
			return(gpa->fcn);
		i=gpa->chain;
	}
	return(NULL);
}
