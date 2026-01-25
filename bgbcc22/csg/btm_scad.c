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
		v=BGBPP_PpiInterpBodyStatement(ctx, c);
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

	if(BCCX_TagIsCstP(n, &bgbcc_rcst_real, "real"))
	{
		f=BCCX_GetFloatCst(n, &bgbcc_rcst_value, "value");
		vec[0]=f;
		vec[1]=f;
		vec[2]=f;
		vec[3]=f;
	}

	if(BCCX_TagIsCstP(n, &bgbcc_rcst_int, "int"))
	{
		f=BCCX_GetIntCst(n, &bgbcc_rcst_value, "value");
		vec[0]=f;
		vec[1]=f;
		vec[2]=f;
		vec[3]=f;
	}
}

u32 BTM_ScadPPI_InterpSanitizeColor(u32 clr)
{
	int cr, cg, cb, cv;
	cr=(clr>>16)&255;
	cg=(clr>> 8)&255;
	cb=(clr>> 0)&255;
	if(cr<4)	cr=4;
	if(cg<4)	cg=4;
	if(cb<4)	cb=4;
	if(cr>255)	cr=255;
	if(cg>255)	cg=255;
	if(cb>255)	cb=255;
	cv=(cr<<16)|(cg<<8)|cb;
	return(cv);
}

u32 BTM_ScadPPI_InterpUnwrapColor(BGBCP_ParseState *ctx, BCCX_Node *n)
{
	double fv[4];
	char *s;
	int cr, cg, cb, cv;
	int i, j, k;

	if(BCCX_TagIsCstP(n, &bgbcc_rcst_array, "array"))
	{
		BTM_ScadPPI_InterpUnwrapVec4(ctx, n, fv);
		cr=fv[0]*255;
		cg=fv[1]*255;
		cb=fv[2]*255;
		if(cr<4)	cr=4;
		if(cg<4)	cg=4;
		if(cb<4)	cb=4;
		if(cr>255)	cr=255;
		if(cg>255)	cg=255;
		if(cb>255)	cb=255;
		cv=(cr<<16)|(cg<<8)|cb;
		return(cv);
	}

	if(BCCX_TagIsCstP(n, &bgbcc_rcst_int, "int"))
	{
		cv=BCCX_GetIntCst(n, &bgbcc_rcst_value, "value");
		cv=BTM_ScadPPI_InterpSanitizeColor(cv);
		return(cv);
	}

	if(BCCX_TagIsCstP(n, &bgbcc_rcst_string, "string"))
	{
		s=BCCX_Get(n, "value");

		if(*s=='#')
		{
			if(strlen(s)==7)
			{
				sscanf(s, "#%06X", &cv);
				cv=BTM_ScadPPI_InterpSanitizeColor(cv);
				return(cv);
			}
		}

		for(i=0; btm_csg_colornames[i].name; i++)
		{
			if(!strcmp(btm_csg_colornames[i].name, s))
			{
				j=btm_csg_colornames[i].rgb;
				if(((j>>16)&255)<4)		j|=(4<<16);
				if(((j>> 8)&255)<4)		j|=(4<< 8);
				if(((j>> 0)&255)<4)		j|=(4<< 0);
				return(j);
			}
		}

		i=bccx_strdup_i(s);
		return(i);
	}
	
	return(0xFFFFFF);
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
	BCCX_SetInt(tmp, "opr", 0);
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
	BCCX_SetInt(tmp, "opr", 0);
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
	BCCX_SetInt(tmp, "opr", 0);
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
	BCCX_SetInt(tmp, "opr", 0);
	return(tmp);
}

BCCX_Node *BTM_ScadPPI_Cube(BGBCP_ParseState *ctx,
	BGBPP_PpiDef *func,
	BCCX_Node **args, int nargs,
	BCCX_Node *ext)
{
	double fv[4];
	BCCX_Node *tmp, *vlst, *va0, *nv, *nc;
	char *s;
	double f, hgt, hgt0, hgt1, ra1, ra2;
	int sfn, ctr;
	int i, j, k;
	
	BTM_ScadPPI_InterpUnwrapVec4(ctx, args[0], fv);

	sfn=0;
	ctr=0;

	for(i=0; i<nargs; i++)
	{
		nc=args[i];
		if(BCCX_TagIsCstP(nc, &bgbcc_rcst_attr, "attr"))
		{
			s=BCCX_GetCst(nc, &bgbcc_rcst_name, "name");
			nv=BCCX_FetchCst(nc, &bgbcc_rcst_value, "value");
			nv=BGBCP_ReduceExpr(ctx->ppi_ctx, nv);
			if(nv)
			{
				f=BCCX_GetFloatCst(nv, &bgbcc_rcst_value, "value");
				k=BCCX_GetIntCst(nv, &bgbcc_rcst_value, "value");
				
//				if(!strcmp(s, "h"))			hgt=f;
//				if(!strcmp(s, "r1"))		ra1=f;
//				if(!strcmp(s, "r2"))		ra2=f;
//				if(!strcmp(s, "r"))			{ ra1=f; ra2=f; }
//				if(!strcmp(s, "d1"))		ra1=f/2;
//				if(!strcmp(s, "d2"))		ra2=f/2;
//				if(!strcmp(s, "d"))			{ ra1=f/2; ra2=f/2; }
				if(!strcmp(s, "center"))	ctr=k;
				if(!strcmp(s, "$fn"))		sfn=k;
			}
		}
	}

	tmp=BCCX_New("cube");
	
	if(ctr)
	{
		BCCX_SetFloat(tmp, "org_x", -fv[0]/2);
		BCCX_SetFloat(tmp, "org_y", -fv[1]/2);
		BCCX_SetFloat(tmp, "org_z", -fv[2]/2);
	}
	
	BCCX_SetFloat(tmp, "size_x", fv[0]);
	BCCX_SetFloat(tmp, "size_y", fv[1]);
	BCCX_SetFloat(tmp, "size_z", fv[2]);
	return(tmp);
}

BCCX_Node *BTM_ScadPPI_Cylinder(BGBCP_ParseState *ctx,
	BGBPP_PpiDef *func,
	BCCX_Node **args, int nargs,
	BCCX_Node *ext)
{
	BCCX_Node *tmp, *nv, *nc, *vlst, *va0;
	BGBPP_PpiDef *def;
	char *s;
	double f, hgt, hgt0, hgt1, ra1, ra2;
	int sfn, ctr;
	int i, j, k;

	hgt=1;
	ra1=1;
	ra2=1;
	ctr=0;
	sfn=0;

	def=BGBPP_PpiLookupNameDef(ctx, "$fn");
	if(def)
	{
		nv=def->value;
		if(nv)
			{ sfn=BCCX_GetIntCst(nv, &bgbcc_rcst_value, "value"); }
	}

	for(i=0; i<nargs; i++)
	{
		nc=args[i];
		if(BCCX_TagIsCstP(nc, &bgbcc_rcst_attr, "attr"))
		{
			s=BCCX_GetCst(nc, &bgbcc_rcst_name, "name");
			nv=BCCX_FetchCst(nc, &bgbcc_rcst_value, "value");
			nv=BGBCP_ReduceExpr(ctx->ppi_ctx, nv);
			if(nv)
			{
				f=BCCX_GetFloatCst(nv, &bgbcc_rcst_value, "value");
				k=BCCX_GetIntCst(nv, &bgbcc_rcst_value, "value");
				
				if(!strcmp(s, "h"))			hgt=f;
				if(!strcmp(s, "r1"))		ra1=f;
				if(!strcmp(s, "r2"))		ra2=f;
				if(!strcmp(s, "r"))			{ ra1=f; ra2=f; }
				if(!strcmp(s, "d1"))		ra1=f/2;
				if(!strcmp(s, "d2"))		ra2=f/2;
				if(!strcmp(s, "d"))			{ ra1=f/2; ra2=f/2; }
				if(!strcmp(s, "center"))	ctr=k;
				if(!strcmp(s, "$fn"))		sfn=k;
			}
		}
	}
	
	if(ctr)
		{ hgt0=-(hgt/2); hgt1= (hgt/2); }
	else
		{ hgt0=0; hgt1=hgt; }

	if(!sfn)
	{
		sfn=64;
		if(sfn<0.25)	sfn=8;
		if(sfn<0.5)		sfn=16;
		if(sfn<2)		sfn=32;
		if(sfn>128)		sfn=256;
	}
	
//	BTM_ScadPPI_InterpUnwrapVec4(ctx, args[0], fv);
	tmp=BCCX_New("cylinder");
	BCCX_SetFloat(tmp, "hgt0", hgt0);
	BCCX_SetFloat(tmp, "hgt1", hgt1);
	BCCX_SetFloat(tmp, "rad0", ra1);
	BCCX_SetFloat(tmp, "rad1", ra2);
	BCCX_SetInt(tmp, "sfn", sfn);
	return(tmp);
}

BCCX_Node *BTM_ScadPPI_Sphere(BGBCP_ParseState *ctx,
	BGBPP_PpiDef *func,
	BCCX_Node **args, int nargs,
	BCCX_Node *ext)
{
	BCCX_Node *tmp, *nv, *nc, *vlst, *va0;
	BGBPP_PpiDef *def;
	char *s;
	double f, hgt, hgt0, hgt1, ra1, ra2;
	int sfn, ctr;
	int i, j, k;

	hgt=1;
	ra1=1;
	ra2=1;
	ctr=0;
	sfn=0;

	def=BGBPP_PpiLookupNameDef(ctx, "$fn");
	if(def)
	{
		nv=def->value;
		if(nv)
			{ sfn=BCCX_GetIntCst(nv, &bgbcc_rcst_value, "value"); }
	}

	for(i=0; i<nargs; i++)
	{
		nc=args[i];
		if(BCCX_TagIsCstP(nc, &bgbcc_rcst_attr, "attr"))
		{
			s=BCCX_GetCst(nc, &bgbcc_rcst_name, "name");
			nv=BCCX_FetchCst(nc, &bgbcc_rcst_value, "value");
			nv=BGBCP_ReduceExpr(ctx->ppi_ctx, nv);
			if(nv)
			{
				f=BCCX_GetFloatCst(nv, &bgbcc_rcst_value, "value");
				k=BCCX_GetIntCst(nv, &bgbcc_rcst_value, "value");
				
				if(!strcmp(s, "h"))			hgt=f;
				if(!strcmp(s, "r1"))		ra1=f;
				if(!strcmp(s, "r2"))		ra2=f;
				if(!strcmp(s, "r"))			{ ra1=f; ra2=f; }
				if(!strcmp(s, "d1"))		ra1=f/2;
				if(!strcmp(s, "d2"))		ra2=f/2;
				if(!strcmp(s, "d"))			{ ra1=f/2; ra2=f/2; }
				if(!strcmp(s, "center"))	ctr=k;
				if(!strcmp(s, "$fn"))		sfn=k;
			}
		}

		if(BCCX_TagIsCstP(nc, &bgbcc_rcst_real, "real"))
		{
			f=BCCX_GetFloatCst(nc, &bgbcc_rcst_value, "value");
			ra1=f; ra2=f;
		}

		if(BCCX_TagIsCstP(nc, &bgbcc_rcst_int, "int"))
		{
			f=BCCX_GetIntCst(nc, &bgbcc_rcst_value, "value");
			ra1=f; ra2=f;
		}
	}
	
	if(!sfn)
	{
		sfn=64;
		if(sfn<0.25)	sfn=8;
		if(sfn<0.5)		sfn=16;
		if(sfn<2)		sfn=32;
		if(sfn>128)		sfn=256;
		
		if(sfn>16)
			sfn=16;
	}
	
//	BTM_ScadPPI_InterpUnwrapVec4(ctx, args[0], fv);
	tmp=BCCX_New("sphere");
	BCCX_SetFloat(tmp, "size_x", ra1);
	BCCX_SetFloat(tmp, "size_y", ra1);
	BCCX_SetFloat(tmp, "size_z", ra1);
	BCCX_SetInt(tmp, "sfn", sfn);
	return(tmp);
}

BCCX_Node *BTM_ScadPPI_Union(BGBCP_ParseState *ctx,
	BGBPP_PpiDef *func,
	BCCX_Node **args, int nargs,
	BCCX_Node *ext)
{
	double fv[4];
	BCCX_Node *tmp, *vlst, *va0;
	
	vlst=BTM_ScadPPI_InterpBody(ctx, ext);
	if(!vlst)
		return(NULL);
	
	tmp=BCCX_New1("union", vlst);
	BCCX_SetInt(tmp, "opr", 0);
	return(tmp);
}

BCCX_Node *BTM_ScadPPI_Difference(BGBCP_ParseState *ctx,
	BGBPP_PpiDef *func,
	BCCX_Node **args, int nargs,
	BCCX_Node *ext)
{
	double fv[4];
	BCCX_Node *tmp, *vlst, *va0;
	
	vlst=BTM_ScadPPI_InterpBody(ctx, ext);
	if(!vlst)
		return(NULL);
	
	tmp=BCCX_New1("union", vlst);
	BCCX_SetInt(tmp, "opr", 1);
	return(tmp);
}

BCCX_Node *BTM_ScadPPI_Intersection(BGBCP_ParseState *ctx,
	BGBPP_PpiDef *func,
	BCCX_Node **args, int nargs,
	BCCX_Node *ext)
{
	double fv[4];
	BCCX_Node *tmp, *vlst, *va0;
	
	vlst=BTM_ScadPPI_InterpBody(ctx, ext);
	if(!vlst)
		return(NULL);
	
	tmp=BCCX_New1("union", vlst);
	BCCX_SetInt(tmp, "opr", 2);
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

	def=BGBPP_PpiGetGlobalDef("color", BGBCC_LANG_SCAD);
	def->PpiDoFuncall2=BTM_ScadPPI_Color;

	def=BGBPP_PpiGetGlobalDef("union", BGBCC_LANG_SCAD);
	def->PpiDoFuncall2=BTM_ScadPPI_Union;
	def=BGBPP_PpiGetGlobalDef("difference", BGBCC_LANG_SCAD);
	def->PpiDoFuncall2=BTM_ScadPPI_Difference;
	def=BGBPP_PpiGetGlobalDef("intersection", BGBCC_LANG_SCAD);
	def->PpiDoFuncall2=BTM_ScadPPI_Intersection;

	def=BGBPP_PpiGetGlobalDef("cube", BGBCC_LANG_SCAD);
	def->PpiDoFuncall2=BTM_ScadPPI_Cube;
	def=BGBPP_PpiGetGlobalDef("cylinder", BGBCC_LANG_SCAD);
	def->PpiDoFuncall2=BTM_ScadPPI_Cylinder;
	def=BGBPP_PpiGetGlobalDef("sphere", BGBCC_LANG_SCAD);
	def->PpiDoFuncall2=BTM_ScadPPI_Sphere;

	return(1);
}

BTM_CsgNode *BTM_Scad_BuildCnodeFromXnodes(BCCX_Node *n)
{
	BTM_CsgNode *prims[32], *prim;
	BTM_CsgBrush *bru;
	float org[3], rot[4], scale[3], ang[4];
	float mins[3], maxs[3];
	float mat[16];
	float mat1[16];
	float mat2[16];
	float mat_rot[16];
	float mat_scl[16];
	float mat_tns[16];
	double f, g, h;
	BCCX_Node *c, *v;
	u64 clrmat;
	int nprim, na, ci, unopr;

	TKRA_Vec3F_Zero(org);
//	TKRA_Vec3F_Zero(rot);
	TKRA_Vec3F_Zero(ang);
	TKRA_Vec3F_Zero(scale);
	QuatF_Identity(rot);

	if(BCCX_TagIsCstP(n, &bgbcc_rcst_union, "union"))
	{
		nprim=0;
		clrmat=0;

		unopr=BCCX_GetInt(n, "opr");
		clrmat=BCCX_GetInt(n, "color");

		org[0]=BCCX_GetFloat(n, "org_x");
		org[1]=BCCX_GetFloat(n, "org_y");
		org[2]=BCCX_GetFloat(n, "org_z");

		ang[0]=BCCX_GetFloat(n, "angles_x");
		ang[1]=BCCX_GetFloat(n, "angles_y");
		ang[2]=BCCX_GetFloat(n, "angles_z");

		scale[0]=BCCX_GetFloat(n, "scale_x");
		scale[1]=BCCX_GetFloat(n, "scale_y");
		scale[2]=BCCX_GetFloat(n, "scale_z");
		if(scale[0]==0)		scale[0]=1;
		if(scale[1]==0)		scale[1]=1;
		if(scale[2]==0)		scale[2]=1;

		ang[0]=ang[0]*(M_PI/180.0);
		ang[1]=ang[1]*(M_PI/180.0);
		ang[2]=ang[2]*(M_PI/180.0);
		QuatF_FromAngles(ang, rot);
//		QuatF_FromAnglesB(ang, rot);

		na=BCCX_GetNodeChildCount(n);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(n, ci);
			prim=BTM_Scad_BuildCnodeFromXnodes(c);
			if(prim)
				prims[nprim++]=prim;
		}

		QuatF_ToMatrix(rot, mat_rot);
		TKRA_Mat4F_Identity(mat_scl);
		TKRA_Mat4F_Identity(mat_tns);

//		memset(mat1, 0, 16*sizeof(float));
		mat_scl[0*4+0]=scale[0];
		mat_scl[1*4+1]=scale[1];
		mat_scl[2*4+2]=scale[2];
		mat_scl[3*4+3]=1;

		mat_tns[12]+=org[0];
		mat_tns[13]+=org[1];
		mat_tns[14]+=org[2];

//		TKRA_Mat4F_MatMult(mat, mat1, mat2);
//		TKRA_Mat4F_MatMultT(mat, mat1, mat2);

		TKRA_Mat4F_MatMult(mat_rot, mat_scl, mat1);
		TKRA_Mat4F_MatMult(mat1, mat_tns, mat);

		if(nprim>1)
		{
			prim=BTM_MakeCsgNodeUnionMulti(prims, nprim);
			prim->clrmat=clrmat;
			prim->opr=unopr;
//			TKRA_Mat4F_Copy(mat2, prim->trans);
			TKRA_Mat4F_Copy(mat, prim->trans);
		}else
		{
			prim=prims[0];
			TKRA_Mat4F_MatMult(prim->trans, mat, mat1);
//			TKRA_Mat4F_MatMultT(prim->trans, mat, mat1);
			TKRA_Mat4F_Copy(mat1, prim->trans);
//			TKRA_Mat4F_Copy(mat, prim->trans);
			if(clrmat && !prim->clrmat)
				prim->clrmat=clrmat;
		}
		
		return(prim);
	}

	if(BCCX_TagIsCstP(n, &bgbcc_rcst_cube, "cube"))
	{
		org[0]=BCCX_GetFloat(n, "org_x");
		org[1]=BCCX_GetFloat(n, "org_y");
		org[2]=BCCX_GetFloat(n, "org_z");

		ang[0]=BCCX_GetFloat(n, "size_x");
		ang[1]=BCCX_GetFloat(n, "size_y");
		ang[2]=BCCX_GetFloat(n, "size_z");
//		mins[0]=0;			mins[1]=0;			mins[2]=0;
//		maxs[0]=ang[0];		maxs[1]=ang[1];		maxs[2]=ang[2];

		mins[0]=org[0];			mins[1]=org[1];			mins[2]=org[2];
		maxs[0]=org[0]+ang[0];	maxs[1]=org[1]+ang[1];	maxs[2]=org[2]+ang[2];

		bru=BTM_AllocBrushAabb(mins, maxs);
		prim=BTM_MakeCsgNodeFromBrushList(bru);
		return(prim);		
	}

	if(BCCX_TagIsCstP(n, &bgbcc_rcst_cylinder, "cylinder"))
	{
		ang[0]=BCCX_GetFloat(n, "hgt0");
		ang[1]=BCCX_GetFloat(n, "hgt1");
		ang[2]=BCCX_GetFloat(n, "rad0");
		ang[3]=BCCX_GetFloat(n, "rad1");
		na=BCCX_GetInt(n, "sfn");
		org[0]=0; org[1]=0; org[2]=ang[0];
		bru=BTM_AllocBrushCylinder(org, ang[1]-ang[0], ang[2], ang[3], na);
		prim=BTM_MakeCsgNodeFromBrushList(bru);
		return(prim);		
	}
	
	if(BCCX_TagIsCstP(n, &bgbcc_rcst_sphere, "sphere"))
	{
		org[0]=0; org[1]=0; org[2]=0;
		ang[0]=BCCX_GetFloat(n, "size_x");
		ang[1]=BCCX_GetFloat(n, "size_y");
		ang[2]=BCCX_GetFloat(n, "size_z");
		na=BCCX_GetInt(n, "sfn");
		bru=BTM_AllocBrushSphereScale(org, ang, na);
		prim=BTM_MakeCsgNodeFromBrushList(bru);
		return(prim);		
	}
	
	return(NULL);
}

BTM_SolidMesh *BTM_ScadCsgToMesh(BTM_CsgNode *csgn, BTM_SolidMesh *olst)
{
	char tb[256];
	float tmat[16];
	BTM_CsgPoly *plmat[256];
	u64 plclr[256];
	int ntv[4];
	btm_vec3f v0, v1, v2, v3;
	float *tris;
	u16 *rgb5;
	BTM_SolidMesh *mtmp, *mlst;
	BTM_CsgPoly *plst, *plcur, *plnxt;
	char *matname;
	float f;
	int i, j, k, nplmat, plix, ntris, clr5;
		
	TKRA_Mat4F_Identity(tmat);
	plst=BTM_GetCsgPolysForCsgNode(csgn, NULL, tmat, 0, 0);
	
	nplmat=0;
	plcur=plst;
	while(plcur)
	{
		for(plix=0; plix<nplmat; plix++)
			if(plclr[plix]==plcur->clrmat)
				break;
		if(plix>=nplmat)
		{
			plix=nplmat++;
			plmat[plix]=NULL;
			plclr[plix]=plcur->clrmat;
		}
	
		plnxt=plcur->next;
		
		plcur->next=plmat[plix];
		plmat[plix]=plcur;
		
		plcur=plnxt;
	}
	
	mlst=olst;
	for(plix=0; plix<nplmat; plix++)
	{
		tris=btm_malloc(256*9*sizeof(float));
		rgb5=btm_malloc(256*sizeof(u16));
		ntv[0]=0;
		ntv[1]=256;

		clr5=BTM_Rgb24ToRgb555(plclr[plix]);
		ntris=0;
		plcur=plmat[plix];
		while(plcur)
		{
			BTM_Mesh_TrisEmitPolygon(&tris, &rgb5, ntv,
				plcur->pts, plcur->npts, clr5);
			plcur=plcur->next;
		}
		
		mtmp=BTM_AllocMesh();
		mtmp->tris=tris;
		mtmp->rgb5=rgb5;
		mtmp->nTris=ntv[0];
		mtmp->mTris=ntv[1];

		mtmp->bbox[0]= 999999999.0;
		mtmp->bbox[1]= 999999999.0;
		mtmp->bbox[2]= 999999999.0;
		mtmp->bbox[3]=-999999999.0;
		mtmp->bbox[4]=-999999999.0;
		mtmp->bbox[5]=-999999999.0;

		mtmp->scale[0]=1.0;
		mtmp->scale[1]=1.0;
		mtmp->scale[2]=1.0;
		
		mtmp->clrmat=plclr[plix];
		
		if((mtmp->clrmat&0xFFFFFF)>=0x040000)
		{
			sprintf(tb, "#%06llX", mtmp->clrmat&0xFFFFFF);
			mtmp->usetex[0]=bccx_strdup(tb);
		}else if((mtmp->clrmat&0xFFFFFF)>0)
		{
			mtmp->usetex[0]=bgbcc_strtab_i(mtmp->clrmat&0xFFFFFF);
		}else
		{
			mtmp->usetex[0]=bccx_strdup("#FFFFFF");
		}
		
		if(plclr[plix])
		{
			mtmp->baseclr[0]=((plclr[plix]>>16)&255)*(1.0/255.0);
			mtmp->baseclr[1]=((plclr[plix]>> 8)&255)*(1.0/255.0);
			mtmp->baseclr[2]=((plclr[plix]>> 0)&255)*(1.0/255.0);
			mtmp->baseclr[3]=1.0;
		}

		mtmp->norm=btm_malloc(mtmp->mTris*4*sizeof(float));
		for(i=0; i<mtmp->nTris; i++)
		{
			for(j=0; j<3; j++)
				for(k=0; k<3; k++)
			{
				f=mtmp->tris[i*9+j*3+k];
				if(f<mtmp->bbox[0+k])
					mtmp->bbox[0+k]=f;
				if(f>mtmp->bbox[3+k])
					mtmp->bbox[3+k]=f;
			}
		
			v0=btm_v3f_sub(
				btm_mkvec3fv(mtmp->tris+i*9+3),
				btm_mkvec3fv(mtmp->tris+i*9+0));
			v1=btm_v3f_sub(
				btm_mkvec3fv(mtmp->tris+i*9+6),
				btm_mkvec3fv(mtmp->tris+i*9+3));
			v2=btm_v3f_cross(v0, v1);
			v3=btm_v3f_norm(v2);

			mtmp->norm[i*4+0]=btm_v3f_x(v3);
			mtmp->norm[i*4+1]=btm_v3f_y(v3);
			mtmp->norm[i*4+2]=btm_v3f_z(v3);
			mtmp->norm[i*4+3]=btm_v3f_dot(btm_mkvec3fv(mtmp->tris+i*9+0), v3);
		}

		mtmp->next=mlst;
		mlst=mtmp;
	}
	
	return(mlst);
}

BTM_SolidMesh *BTM_LoadMeshListScadBuffer(byte *ibuf, int ibsz)
{
	static BGBCP_ParseState *ctx, *ppictx;
	BTM_CsgNode *prims[32], *prim;
	BTM_SolidMesh *mlst, *mcur;
	BTM_CsgNode *csgn;
	BCCX_Node *n, *c, *v, *lst;
	char *s;
	int ci, na, nprim;
	int i, j, k;
	
	if(!ctx)
	{
		BTM_ScadInitGlobals();

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
	ctx->ppi_ctx->toplevel=NULL;
	lst=BGBCP_Toplevel(ctx->ppi_ctx, &s);
	lst=ctx->ppi_ctx->toplevel;

	mlst=NULL;
	nprim=0;

#ifdef BGBCC_BCCX2
	na=BCCX_GetNodeChildCount(lst);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(lst, ci);
		v=BGBPP_PpiInterpTopStatement(ctx, c);
		if(v)
		{
			csgn=BTM_Scad_BuildCnodeFromXnodes(v);
			if(csgn)
				prims[nprim++]=csgn;
		}
	}
#endif

	if(nprim>1)
	{
		prim=BTM_MakeCsgNodeUnionMulti(prims, nprim);
	}else
	{
		prim=prims[0];
	}
	
	mlst=BTM_ScadCsgToMesh(prim, NULL);

#if 0
	mcur=mlst;
	while(mcur)
	{
		mcur->scale[0]*=512.0/1000.0;
		mcur->scale[1]*=512.0/1000.0;
		mcur->scale[2]*=512.0/1000.0;
		mcur=mcur->next;
	}
#endif

	return(mlst);

//	n=BGBCP_ModuleBuffer("generic.scad", "generic", ibuf);
}

BTM_SolidMesh *BTM_LoadMeshListScadFile(char *name)
{
	byte *ibuf;
	int ibsz;
	ibuf=BTM_LoadFile(name, &ibsz);
	return(BTM_LoadMeshListScadBuffer(ibuf, ibsz));
}
