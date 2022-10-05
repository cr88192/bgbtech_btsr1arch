
void tkra_glMatrixMode(int mode)
{
	TKRA_Context *ctx;
	ctx=TKRA_GetCurrentContext();
	ctx->matmode=mode;
}

void tkra_glPushMatrix(void)
{
	TKRA_Context *ctx;
	int ix;

	ctx=TKRA_GetCurrentContext();
	
	if(ctx->matmode==0)
	{
		ix=ctx->stkpos_xform++;
		ctx->stk_xform[ix]=ctx->mat_xform;
		return;
	}

	if(ctx->matmode==1)
	{
		ix=ctx->stkpos_xproj++;
		ctx->stk_xproj[ix]=ctx->mat_xproj;
		return;
	}
}

void tkra_glPopMatrix(void)
{
	TKRA_Context *ctx;
	int ix;

	ctx=TKRA_GetCurrentContext();
	
	if(ctx->matmode==0)
	{
		ix=ctx->stkpos_xform-1;
		ctx->stkpos_xform=ix;
		ctx->mat_xform=ctx->stk_xform[ix];
		ctx->mat_tproj=TKRA_MatrixMultiply(
			ctx->mat_xform, ctx->mat_xproj);
		return;
	}

	if(ctx->matmode==1)
	{
		ix=ctx->stkpos_xproj-1;
		ctx->stkpos_xproj=ix;
		ctx->mat_xproj=ctx->stk_xproj[ix];
		ctx->mat_tproj=TKRA_MatrixMultiply(
			ctx->mat_xform, ctx->mat_xproj);
		return;
	}
}

void tkra_glLoadIdentity(void)
{
	TKRA_Context *ctx;
	int ix;

	ctx=TKRA_GetCurrentContext();
	
	if(ctx->matmode==0)
	{
		ctx->mat_xform=TKRA_MatrixIdentify();
		ctx->mat_tproj=TKRA_MatrixMultiply(
			ctx->mat_xform, ctx->mat_xproj);
		return;
	}
	
	if(ctx->matmode==1)
	{
		ctx->mat_xproj=TKRA_MatrixIdentify();
		ctx->mat_tproj=TKRA_MatrixMultiply(
			ctx->mat_xform, ctx->mat_xproj);
		return;
	}
}

void tkra_glLoadMatrixI(tkra_mat4 mat)
{
	TKRA_Context *ctx;
	int ix;

	ctx=TKRA_GetCurrentContext();
	
	if(ctx->matmode==0)
		{ ctx->mat_xform=mat; }
	if(ctx->matmode==1)
		{ ctx->mat_xproj=mat; }
	ctx->mat_tproj=TKRA_MatrixMultiply(
		ctx->mat_xform, ctx->mat_xproj);
}

void tkra_glMultMatrixI(tkra_mat4 mat)
{
	TKRA_Context *ctx;
	int ix;

	ctx=TKRA_GetCurrentContext();
	
	if(ctx->matmode==0)
		{ ctx->mat_xform=TKRA_MatrixMultiply(mat, ctx->mat_xform); }
	if(ctx->matmode==1)
		{ ctx->mat_xproj=TKRA_MatrixMultiply(mat, ctx->mat_xproj); }
	ctx->mat_tproj=TKRA_MatrixMultiply(
		ctx->mat_xform, ctx->mat_xproj);
}

void tkra_glViewport(int x, int y, int width, int height)
{
	TKRA_Context *ctx;
	int ix;

	ctx=TKRA_GetCurrentContext();
	TKRA_SetViewport(ctx, x, y, width, height);
}

void tkra_glLoadMatrixd(const double *m)
{
	tkra_glLoadMatrixI(TKRA_Matrix16dv((double *)m));
}

void tkra_glLoadMatrixf(const float *m)
{
	tkra_glLoadMatrixI(TKRA_Matrix16fv((float *)m));
}

void tkra_glMultMatrixd(const double *m)
{
	tkra_glMultMatrixI(TKRA_Matrix16dv((double *)m));
}

void tkra_glMultMatrixf(const float *m)
{
	tkra_glMultMatrixI(TKRA_Matrix16fv((float *)m));
}

void tkra_glRotatef(float angle, float x, float y, float z)
{
	float tv[4];
	float tmat[16];

	tv[0]=x; tv[1]=y; tv[2]=z; tv[3]=0;
	TKRA_Mat4F_AxisTo4MatrixT(tv, -angle*(M_PI/180), tmat);
	tkra_glMultMatrixf(tmat);
}

void tkra_glScalef(float x, float y, float z)
{
	float tmat[16];
	TKRA_Mat4F_Identity(tmat);
	tmat[0]=x; tmat[5]=y; tmat[10]=z;
	tkra_glMultMatrixf(tmat);
}

void tkra_glTranslatef(float x, float y, float z)
{
	float tmat[16];
	float xyz0[16];

	TKRA_Mat4F_Identity(tmat);
	tmat[12]=x; tmat[13]=y; tmat[14]=z;
	tkra_glMultMatrixf(tmat);
}

void tkra_glRotated(double angle, double x, double y, double z)
	{ tkra_glRotatef(angle, x, y, z); }
void tkra_glScaled(double x, double y, double z)
	{ tkra_glScalef(x, y, z); }
void tkra_glTranslated(double x, double y, double z)
	{ tkra_glTranslatef(x, y, z); }

void tkra_glOrtho(
	double left,		double right,
	double bottom,		double top,
	double nearval,	double farval)
{
	float tmat[16];
	TKRA_Mat4F_SetupOrtho(tmat, left, right, bottom, top, nearval, farval);
	tkra_glMultMatrixf(tmat);
}

void tkra_glFrustum(
	double left,		double right,
	double bottom,		double top,
	double nearval,	double farval)
{
	float tmat[16];
	TKRA_Mat4F_SetupFrustum(tmat, left, right, bottom, top, nearval, farval);
	tkra_glMultMatrixf(tmat);
}
