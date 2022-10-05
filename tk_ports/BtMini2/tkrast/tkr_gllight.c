
void tkra_glLightf( int light, int pname, float param )
{
	TKRA_Context *ctx;
	int lnum;

	ctx=TKRA_GetCurrentContext();

	lnum=light-GL_LIGHT0;
	if((lnum<0) || (lnum>=8))
		return;

	switch(pname)
	{
	case GL_SPOT_EXPONENT:
		ctx->light_spot_exp[lnum]=param;
		break;
	case GL_SPOT_CUTOFF:
		ctx->light_spot_cutoff[lnum]=param;
		break;

	case GL_CONSTANT_ATTENUATION:
		ctx->light_attn_const[lnum]=param;
		break;
	case GL_LINEAR_ATTENUATION:
		ctx->light_attn_linear[lnum]=param;
		break;
	case GL_QUADRATIC_ATTENUATION:
		ctx->light_attn_quadratic[lnum]=param;
		break;
	}

//	__debugbreak();
}

void tkra_glLighti( int light, int pname, int param )
{
	tkra_glLightf(light, pname, param);
}

void tkra_glLightfv( int light, int pname, const float *params )
{
	TKRA_Context *ctx;
	int lnum;

	ctx=TKRA_GetCurrentContext();

	lnum=light-GL_LIGHT0;
	if((lnum<0) || (lnum>=8))
		return;

	switch(pname)
	{
	case GL_AMBIENT:
		ctx->light_ambient[lnum]=tkra_mkvec4f(
			params[0], params[1],
			params[2], params[3]);
		break;
	case GL_DIFFUSE:
		ctx->light_diffuse[lnum]=tkra_mkvec4f(
			params[0], params[1],
			params[2], params[3]);
		break;
	case GL_SPECULAR:
		ctx->light_specular[lnum]=tkra_mkvec4f(
			params[0], params[1],
			params[2], params[3]);
		break;
	case GL_POSITION:
		ctx->light_position[lnum]=tkra_mkvec4f(
			params[0], params[1],
			params[2], params[3]);
		break;
	case GL_SPOT_DIRECTION:
		ctx->light_spot_dir[lnum]=tkra_mkvec4f(
			params[0], params[1],
			params[2], params[3]);
		break;
	
	case GL_SPOT_EXPONENT:
		ctx->light_spot_exp[lnum]=params[0];
		break;
	case GL_SPOT_CUTOFF:
		ctx->light_spot_cutoff[lnum]=params[0];
		break;

	case GL_CONSTANT_ATTENUATION:
		ctx->light_attn_const[lnum]=params[0];
		break;
	case GL_LINEAR_ATTENUATION:
		ctx->light_attn_linear[lnum]=params[0];
		break;
	case GL_QUADRATIC_ATTENUATION:
		ctx->light_attn_quadratic[lnum]=params[0];
		break;
	}
}

void tkra_glLightiv( int light, int pname, const int *params )
{
	TKRA_Context *ctx;
	int lnum;

	ctx=TKRA_GetCurrentContext();

	lnum=light-GL_LIGHT0;
	if((lnum<0) || (lnum>=8))
		return;

	switch(pname)
	{
	case GL_AMBIENT:
		ctx->light_ambient[lnum]=tkra_mkvec4f(
			params[0], params[1],
			params[2], params[3]);
		break;
	case GL_DIFFUSE:
		ctx->light_diffuse[lnum]=tkra_mkvec4f(
			params[0], params[1],
			params[2], params[3]);
		break;
	case GL_SPECULAR:
		ctx->light_specular[lnum]=tkra_mkvec4f(
			params[0], params[1],
			params[2], params[3]);
		break;
	case GL_POSITION:
		ctx->light_position[lnum]=tkra_mkvec4f(
			params[0], params[1],
			params[2], params[3]);
		break;
	case GL_SPOT_DIRECTION:
		ctx->light_spot_dir[lnum]=tkra_mkvec4f(
			params[0], params[1],
			params[2], params[3]);
		break;
	
	case GL_SPOT_EXPONENT:
		ctx->light_spot_exp[lnum]=params[0];
		break;
	case GL_SPOT_CUTOFF:
		ctx->light_spot_cutoff[lnum]=params[0];
		break;

	case GL_CONSTANT_ATTENUATION:
		ctx->light_attn_const[lnum]=params[0];
		break;
	case GL_LINEAR_ATTENUATION:
		ctx->light_attn_linear[lnum]=params[0];
		break;
	case GL_QUADRATIC_ATTENUATION:
		ctx->light_attn_quadratic[lnum]=params[0];
		break;
	}
}

void tkra_glGetLightfv( int light, int pname, float *params )
{
//	__debugbreak();
}

void tkra_glGetLightiv( int light, int pname, int *params )
{
//	__debugbreak();
}

void tkra_glLightModelf( int pname, float param )
{
//	__debugbreak();
}

void tkra_glLightModeli( int pname, int param )
{
//	__debugbreak();
}

void tkra_glLightModelfv( int pname, const float *params )
{
	TKRA_Context *ctx;

	ctx=TKRA_GetCurrentContext();

	switch(pname)
	{
	case GL_LIGHT_MODEL_AMBIENT:
		ctx->light_model_ambient=tkra_mkvec4f(
			params[0], params[1],
			params[2], params[3]);
		break;
	}
}

void tkra_glLightModeliv( int pname, const int *params )
{
//	__debugbreak();
}

void tkra_glMaterialf( int face, int pname, float param )
{
//	__debugbreak();
}

void tkra_glMateriali( int face, int pname, int param )
{
//	__debugbreak();
}

void tkra_glMaterialfv( int face, int pname, const float *params )
{
//	__debugbreak();
}

void tkra_glMaterialiv( int face, int pname, const int *params )
{
//	__debugbreak();
}

void tkra_glGetMaterialfv( int face, int pname, float *params )
{
//	__debugbreak();
}

void tkra_glGetMaterialiv( int face, int pname, int *params )
{
//	__debugbreak();
}

void tkra_glColorMaterial( int face, int mode )
{
//	__debugbreak();
}
