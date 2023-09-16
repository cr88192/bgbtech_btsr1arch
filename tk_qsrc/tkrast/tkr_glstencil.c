void tkra_glStencilFunc(int func, int ref, unsigned int mask)
{
	TKRA_Context *ctx;

	ctx=TKRA_GetCurrentContext();

	ctx->zat_stref=ref;
	ctx->zat_stmask=mask;

	switch(func)
	{
	case GL_NEVER:		ctx->zat_stfunc=TKRA_ZAT_NV; break;
	case GL_ALWAYS:		ctx->zat_stfunc=TKRA_ZAT_AL; break;
	case GL_EQUAL:		ctx->zat_stfunc=TKRA_ZAT_EQ; break;
	case GL_NOTEQUAL:	ctx->zat_stfunc=TKRA_ZAT_NE; break;
	case GL_LESS:		ctx->zat_stfunc=TKRA_ZAT_LT; break;
	case GL_LEQUAL:		ctx->zat_stfunc=TKRA_ZAT_LE; break;
	case GL_GREATER:	ctx->zat_stfunc=TKRA_ZAT_GT; break;
	case GL_GEQUAL:		ctx->zat_stfunc=TKRA_ZAT_GE; break;
	}
}

void tkra_glStencilMask(unsigned int mask)
{
	TKRA_Context *ctx;

	ctx=TKRA_GetCurrentContext();
	ctx->zat_stmask=mask;
}

void tkra_glStencilOp(int sfail, int zfail, int zpass)
{
	TKRA_Context *ctx;

	ctx=TKRA_GetCurrentContext();

#if 1
	switch(sfail)
	{
	case GL_KEEP:		ctx->zat_sto_stfail=TKRA_STO_KEEP;			break;
	case GL_ZERO:		ctx->zat_sto_stfail=TKRA_STO_ZERO;			break;
	case GL_INVERT:		ctx->zat_sto_stfail=TKRA_STO_INVERT;		break;
	case GL_REPLACE:	ctx->zat_sto_stfail=TKRA_STO_KEEP;			break;
	case GL_INCR:		ctx->zat_sto_stfail=TKRA_STO_INCR_CLAMP;	break;
	case GL_DECR:		ctx->zat_sto_stfail=TKRA_STO_DECR_CLAMP;	break;
	case GL_INCR_WRAP:	ctx->zat_sto_stfail=TKRA_STO_INCR_WRAP;		break;
	case GL_DECR_WRAP:	ctx->zat_sto_stfail=TKRA_STO_DECR_WRAP;		break;
	}

	switch(zfail)
	{
	case GL_KEEP:		ctx->zat_sto_zfail=TKRA_STO_KEEP;			break;
	case GL_ZERO:		ctx->zat_sto_zfail=TKRA_STO_ZERO;			break;
	case GL_INVERT:		ctx->zat_sto_zfail=TKRA_STO_INVERT;			break;
	case GL_REPLACE:	ctx->zat_sto_zfail=TKRA_STO_KEEP;			break;
	case GL_INCR:		ctx->zat_sto_zfail=TKRA_STO_INCR_CLAMP;		break;
	case GL_DECR:		ctx->zat_sto_zfail=TKRA_STO_DECR_CLAMP;		break;
	case GL_INCR_WRAP:	ctx->zat_sto_zfail=TKRA_STO_INCR_WRAP;		break;
	case GL_DECR_WRAP:	ctx->zat_sto_zfail=TKRA_STO_DECR_WRAP;		break;
	}

	switch(zpass)
	{
	case GL_KEEP:		ctx->zat_sto_zpass=TKRA_STO_KEEP;			break;
	case GL_ZERO:		ctx->zat_sto_zpass=TKRA_STO_ZERO;			break;
	case GL_INVERT:		ctx->zat_sto_zpass=TKRA_STO_INVERT;			break;
	case GL_REPLACE:	ctx->zat_sto_zpass=TKRA_STO_KEEP;			break;
	case GL_INCR:		ctx->zat_sto_zpass=TKRA_STO_INCR_CLAMP;		break;
	case GL_DECR:		ctx->zat_sto_zpass=TKRA_STO_DECR_CLAMP;		break;
	case GL_INCR_WRAP:	ctx->zat_sto_zpass=TKRA_STO_INCR_WRAP;		break;
	case GL_DECR_WRAP:	ctx->zat_sto_zpass=TKRA_STO_DECR_WRAP;		break;
	}
#endif
}

void tkra_glClearStencil(int s)
{
	TKRA_Context *ctx;

	ctx=TKRA_GetCurrentContext();
	ctx->zat_stclear=s;
}

