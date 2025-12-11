BCCX_Node *BTM_ScadPPI_InterpBody(BGBCP_ParseState *ctx, BCCX_Node *lst)
{
	BCCX_Node *n, *c, *v, *alst;
	char *s;
	int ci, na;
	int i, j, k;
	
	if(!lst)
		return(NULL);
	
	alst=NULL;
	na=BCCX_GetNodeChildCount(lst);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(lst, ci);
		v=BGBPP_PpiInterpTopStatement(ctx, c);
		if(v)
		{
			alst=BCCX_AddEnd(alst, v);
		}
	}
	return(alst);
}

void BTM_ScadPPI_InterpUnwrapVec4(BGBCP_ParseState *ctx,
	BCCX_Node *n, double *vec)
{
	BCCX_Node *n1, *v, *c;
	double f;
	int ci, na;
	
	vec[0]=0;	vec[1]=0;
	vec[2]=0;	vec[3]=0;
	
	if(BCCX_TagIsCstP(n, &bgbcc_rcst_array, "array"))
	{
		n1=BCCX_FindTagCst(n, &bgbcc_rcst_list, "list");
		if(n1)
		{
			na=BCCX_GetNodeChildCount(n1);
			for(ci=0; ci<na; ci++)
			{
				c=BCCX_GetNodeIndex(n1, ci);
				v=BGBCP_ReduceExpr(ctx->ppi_ctx, c);
				if(v)
				{
					f=BCCX_GetFloatCst(v, &bgbcc_rcst_value, "value");
					vec[ci]=f;
				}
			}
		}
	}
}

u32 BTM_ScadPPI_InterpUnwrapColor(BGBCP_ParseState *ctx, BCCX_Node *n)
{
	double fv[4];
	int cr, cg, cb, cv;

	if(BCCX_TagIsCstP(n, &bgbcc_rcst_array, "array"))
	{
		BTM_ScadPPI_InterpUnwrapVec4(ctx, n, fv);
		cr=fv[0]*255;
		cg=fv[1]*255;
		cb=fv[2]*255;
		cv=0xFF000000|(cr<<16)|(cg<<8)|cb;
		return(cv);
	}

	if(BCCX_TagIsCstP(n, &bgbcc_rcst_int, "int"))
	{
		cv=BCCX_GetIntCst(n, &bgbcc_rcst_value, "value");
		cv|=0xFF000000;
		return(cv);
	}
	
	return(0xFFFFFFFF);
}

BCCX_Node *BTM_ScadPPI_Translate(BGBCP_ParseState *ctx,
	BGBPP_PpiDef *func,
	BCCX_Node **args, int nargs,
	BCCX_Node *ext)
{
	double fv[4];
	BCCX_Node *tmp, *vlst, *va0;
	
	vlst=BTM_ScadPPI_InterpBody(ctx, ext);
	if(!vlst)
		return(NULL);
	
	BTM_ScadPPI_InterpUnwrapVec4(ctx, args[0], fv);
	tmp=BCCX_New1("union", vlst);
	BCCX_SetFloat(tmp, "org_x", fv[0]);
	BCCX_SetFloat(tmp, "org_y", fv[1]);
	BCCX_SetFloat(tmp, "org_z", fv[2]);
	return(tmp);
}

BCCX_Node *BTM_ScadPPI_Scale(BGBCP_ParseState *ctx,
	BGBPP_PpiDef *func,
	BCCX_Node **args, int nargs,
	BCCX_Node *ext)
{
	double fv[4];
	BCCX_Node *tmp, *vlst, *va0;
	
	vlst=BTM_ScadPPI_InterpBody(ctx, ext);
	if(!vlst)
		return(NULL);
	
	BTM_ScadPPI_InterpUnwrapVec4(ctx, args[0], fv);
	tmp=BCCX_New1("union", vlst);
	BCCX_SetFloat(tmp, "scale_x", fv[0]);
	BCCX_SetFloat(tmp, "scale_y", fv[1]);
	BCCX_SetFloat(tmp, "scale_z", fv[2]);
	return(tmp);
}

BCCX_Node *BTM_ScadPPI_Rotate(BGBCP_ParseState *ctx,
	BGBPP_PpiDef *func,
	BCCX_Node **args, int nargs,
	BCCX_Node *ext)
{
	double fv[4];
	BCCX_Node *tmp, *vlst, *va0;
	
	vlst=BTM_ScadPPI_InterpBody(ctx, ext);
	if(!vlst)
		return(NULL);
	
	BTM_ScadPPI_InterpUnwrapVec4(ctx, args[0], fv);
	tmp=BCCX_New1("union", vlst);
	BCCX_SetFloat(tmp, "angles_x", fv[0]);
	BCCX_SetFloat(tmp, "angles_y", fv[1]);
	BCCX_SetFloat(tmp, "angles_z", fv[2]);
	return(tmp);
}

BCCX_Node *BTM_ScadPPI_Color(BGBCP_ParseState *ctx,
	BGBPP_PpiDef *func,
	BCCX_Node **args, int nargs,
	BCCX_Node *ext)
{
//	double fv[4];
	BCCX_Node *tmp, *vlst, *va0;
	u32 rgb;
	
	vlst=BTM_ScadPPI_InterpBody(ctx, ext);
	if(!vlst)
		return(NULL);
	
	rgb=BTM_ScadPPI_InterpUnwrapColor(ctx, args[0]);
	tmp=BCCX_New1("union", vlst);
	BCCX_SetInt(tmp, "color", rgb);
	return(tmp);
}

BCCX_Node *BTM_ScadPPI_Cube(BGBCP_ParseState *ctx,
	BGBPP_PpiDef *func,
	BCCX_Node **args, int nargs,
	BCCX_Node *ext)
{
	double fv[4];
	BCCX_Node *tmp, *vlst, *va0;
	
	BTM_ScadPPI_InterpUnwrapVec4(ctx, args[0], fv);
	tmp=BCCX_New("cube");
	BCCX_SetFloat(tmp, "size_x", fv[0]);
	BCCX_SetFloat(tmp, "size_y", fv[1]);
	BCCX_SetFloat(tmp, "size_z", fv[2]);
	return(tmp);
}

int BTM_ScadInitGlobals()
{
	BGBPP_PpiDef *def;

	def=BGBPP_PpiGetGlobalDef("translate", BGBCC_LANG_SCAD);
	if(def && def->PpiDoFuncall2)
		return(0);

	def=BGBPP_PpiGetGlobalDef("translate", BGBCC_LANG_SCAD);
	def->PpiDoFuncall2=BTM_ScadPPI_Translate;
	def=BGBPP_PpiGetGlobalDef("scale", BGBCC_LANG_SCAD);
	def->PpiDoFuncall2=BTM_ScadPPI_Scale;
	def=BGBPP_PpiGetGlobalDef("rotate", BGBCC_LANG_SCAD);
	def->PpiDoFuncall2=BTM_ScadPPI_Rotate;
	def=BGBPP_PpiGetGlobalDef("cube", BGBCC_LANG_SCAD);
	def->PpiDoFuncall2=BTM_ScadPPI_Cube;

	return(1);
}

BTM_SolidMesh *BTM_LoadMeshListScadBuffer(byte *ibuf, int ibsz)
{
	static BGBCP_ParseState *ctx, *ppictx;
	BTM_SolidMesh *mlst;
	BCCX_Node *n, *c, *v, *lst;
	char *s;
	int ci, na;
	int i, j, k;
	
	if(!ctx)
	{
		ctx=bgbcc_malloc(sizeof(BGBCP_ParseState));
		memset(ctx, 0, sizeof(BGBCP_ParseState));

		ppictx=bgbcc_malloc(sizeof(BGBCP_ParseState));
		memset(ppictx, 0, sizeof(BGBCP_ParseState));
		ctx->ppi_ctx=ppictx;
		ppictx->ppi_upctx=ctx;
		
#ifdef BGBCC_BCCX2
		for(i=0; i<256; i++)
			{ ctx->struct_hash_ix[i]=-1; }
		for(i=0; i<1024; i++)
			{ ctx->type_hash_ix[i]=-1; }

		for(i=0; i<256; i++)
			{ ppictx->struct_hash_ix[i]=-1; }
		for(i=0; i<1024; i++)
			{ ppictx->type_hash_ix[i]=-1; }
#endif

		ctx->lang=BGBCC_FMT_SCAD;
		ppictx->lang=BGBCC_FMT_SCAD;
		BGBCP_SetLocale(ctx, "C");
	}

	s=ibuf;
	lst=BGBCP_Toplevel(ctx->ppi_ctx, &s);

	mlst=NULL;

#ifdef BGBCC_BCCX2
	na=BCCX_GetNodeChildCount(lst);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(lst, ci);
		v=BGBPP_PpiInterpTopStatement(ctx, c);
		if(v)
		{
		}
	}
#endif

	return(mlst);

//	n=BGBCP_ModuleBuffer("generic.scad", "generic", ibuf);
}
