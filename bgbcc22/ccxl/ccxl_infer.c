#include <bgbccc.h>

void BGBCC_CCXL_InferExpr_CapRef(BGBCC_TransState *ctx,
	int lvl, char *name)
{
	int i;

	if(!ctx->do_lambda_capref)
		return;
	if(lvl>ctx->do_lambda_capref)
		return;

	for(i=0; i<ctx->n_lambda_capref; i++)
	{
		if(!strcmp(ctx->lambda_capref[i], name))
			return;
	}

	i=ctx->n_lambda_capref++;
	ctx->lambda_capref[i]=name;
}

int BGBCC_CCXL_InferExpr(BGBCC_TransState *ctx,
	BCCX_Node *l, ccxl_type *rdty)
{
	char tb1[256];
	ccxl_register sreg, treg;
	ccxl_type bty, dty, sty, tty, lty, rty;
	BGBCC_CCXL_LiteralInfo *st;
	BCCX_Node *c, *t, *v, *x, *ln, *rn;
	char *s0, *s1, *s2, *s3;
	char *suf, *op;
	s64 li, lj;
	int i0, i1, i2, i3;
	int opr;
	int i, j, k;

	*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_UNDEF_I);

	if(	BCCX_TagIsCstP(l, &bgbcc_rcst_int, "int") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_int128, "int128"))
	{
		suf=BCCX_GetCst(l, &bgbcc_rcst_tysuf, "tysuf");
		li=BCCX_GetIntCst(l, &bgbcc_rcst_value, "value");
		
		if(!suf)
		{
			if(((s32)li)==li)
			{
				*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I);
				return(1);
			}
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_L);
			return(1);
		}
		
		if(!stricmp(suf, "L"))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_NL);
			return(1);
		}

		if(!stricmp(suf, "UL"))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_UNL);
			return(1);
		}

		if(!stricmp(suf, "LL"))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_L);
			return(1);
		}

		if(!stricmp(suf, "ULL"))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_UL);
			return(1);
		}

		if(!stricmp(suf, "XL"))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I128);
			return(1);
		}

		if(!stricmp(suf, "UXL"))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_UI128);
			return(1);
		}

		if(((s32)li)==li)
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I);
			return(1);
		}
		*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_L);
		return(1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_real, "real"))
	{
		suf=BCCX_GetCst(l, &bgbcc_rcst_tysuf, "tysuf");
		
		if(!suf)
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
			return(1);
		}

		if(!stricmp(suf, "F"))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);
			return(1);
		}

		*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
		return(1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_complex, "complex"))
	{
		*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_DCOMPLEX);
		return(1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_string, "string"))
	{
		tty.val=CCXL_VTY_PCHAR;
		*rdty=tty;
//		*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_VTY_PCHAR);
		return(1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_charstring, "charstring"))
	{
		*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I);
		return(1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_ref, "ref"))
	{
		s0=BCCX_GetCst(l, &bgbcc_rcst_name, "name");

		if(	(ctx->lang==BGBCC_LANG_CPP)	||
			(ctx->lang==BGBCC_LANG_CS)	||
			(ctx->lang==BGBCC_LANG_BS2)	||
			(ctx->lang==BGBCC_LANG_JAVA)	)
		{
			if(!bgbcp_strcmp(s0, "this"))
			{
				i=BGBCC_CCXL_TryGetThisType(ctx, &sty);
				if(i>0)
				{
					BGBCC_CCXL_InferExpr_CapRef(ctx, 0, s0);
					*rdty=sty;
					return(1);
				}
			}
		}

		if(	(ctx->lang==BGBCC_LANG_CS)	||
			(ctx->lang==BGBCC_LANG_BS2)	||
			(ctx->lang==BGBCC_LANG_JAVA)	)
		{
			if(!bgbcp_strcmp(s0, "null"))
			{
				*rdty=BGBCC_CCXL_MakeTypeVTy(ctx, CCXL_VTY_NULL);
				return(0);
			}

			if(!bgbcp_strcmp(s0, "true") ||
				!bgbcp_strcmp(s0, "false"))
			{
				*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_SB);
				return(0);
			}
		}

		for(j=ctx->vlcl_stackpos-1; j>=0; j--)
		{
			sprintf(tb1, "%s!%d", s0, ctx->vlcl_stack[j]);
			i=BGBCC_CCXL_LookupLocalIndex(ctx, tb1);
			if(i>=0)
			{
				s0=bgbcc_strdup(tb1);
				BGBCC_CCXL_InferExpr_CapRef(ctx, j, s0);
				break;
			}
			i=BGBCC_CCXL_LookupStaticIndex(ctx, tb1);
			if(i>=0)
			{
				s0=bgbcc_strdup(tb1);
				BGBCC_CCXL_InferExpr_CapRef(ctx, j, s0);
				break;
			}
		}

//		i=BGBCC_CCXL_LookupAsRegister(ctx, s0, &sreg);
		i=BGBCC_CCXL_TryLookupAsRegister(ctx, s0, &sreg, false);
		if(i<=0)
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I);
			return(0);
		}

		sty=BGBCC_CCXL_GetRegType(ctx, sreg);
		*rdty=sty;
		return(1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_binary, "binary"))
	{
		op=BCCX_GetCst(l, &bgbcc_rcst_op, "op");
		if(!op)
			return(0);

		opr=-1;
		if(!strcmp(op, "=="))opr=CCXL_CMP_EQ;
		if(!strcmp(op, "!="))opr=CCXL_CMP_NE;
		if(!strcmp(op, "<"))opr=CCXL_CMP_LT;
		if(!strcmp(op, ">"))opr=CCXL_CMP_GT;
		if(!strcmp(op, "<="))opr=CCXL_CMP_LE;
		if(!strcmp(op, ">="))opr=CCXL_CMP_GE;

		if(!strcmp(op, "&&"))
			opr=CCXL_BINOP_AND;
		if(!strcmp(op, "||"))
			opr=CCXL_BINOP_OR;

		if(!strcmp(op, "&&&"))
			opr=CCXL_BINOP_AND;
		if(!strcmp(op, "|||"))
			opr=CCXL_BINOP_OR;

		if(opr>=0)
		{
			if(ctx->lang==BGBCC_LANG_C)
				{ *rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I); }
			else
				{ *rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_SB); }
			return(1);
		}

		opr=-1;
		if(!strcmp(op, "+"))
			opr=CCXL_BINOP_ADD;
		if(!strcmp(op, "-"))
			opr=CCXL_BINOP_SUB;
		if(!strcmp(op, "*"))
			opr=CCXL_BINOP_MUL;
		if(!strcmp(op, "/"))
			opr=CCXL_BINOP_DIV;
		if(!strcmp(op, "%"))
			opr=CCXL_BINOP_MOD;
		if(!strcmp(op, "&"))
			opr=CCXL_BINOP_AND;
		if(!strcmp(op, "|"))
			opr=CCXL_BINOP_OR;
		if(!strcmp(op, "^"))
			opr=CCXL_BINOP_XOR;
		if(!strcmp(op, "<<"))
			opr=CCXL_BINOP_SHL;
		if(!strcmp(op, ">>"))
			opr=CCXL_BINOP_SHR;
		if(!strcmp(op, ">>>"))
			opr=CCXL_BINOP_SHRR;

		if(opr<0)
			return(0);

		ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");

//		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
//		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);

		i0=BGBCC_CCXL_InferExpr(ctx, ln, &lty);
		i1=BGBCC_CCXL_InferExpr(ctx, rn, &rty);
		if(!i0 || !i1)
			return(0);

		BGBCC_CCXL_GetTypeBinaryDest(ctx, opr, lty, rty, &dty);
		*rdty=dty;
		return(1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_unary, "unary"))
	{
		op=BCCX_GetCst(l, &bgbcc_rcst_op, "op");
		if(!op)
			return(0);

		if(	!strcmp(op, "!") ||
			!strcmp(op, "!!"))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I);
			return(1);
		}

		t=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
//		t=BGBCC_CCXL_ReduceExpr(ctx, t);

		i0=BGBCC_CCXL_InferExpr(ctx, t, &tty);
		if(!i0)
			return(0);

		if(!strcmp(op, "+") ||
			!strcmp(op, "-") ||
			!strcmp(op, "~"))
		{
			*rdty=tty;
			return(1);
		}

		if(!strcmp(op, "*"))
		{
			BGBCC_CCXL_TypeDerefType(ctx, tty, &dty);
			*rdty=dty;
			return(1);
		}

		if(!strcmp(op, "&"))
		{
			BGBCC_CCXL_TypePointerType(ctx, tty, &dty);
			*rdty=dty;
			return(1);
		}

		dty=tty;

//		BGBCC_CCXL_GetTypeBinaryDest(ctx, opr, lty, rty, &dty);
		*rdty=dty;
		return(0);
	}

	if(	BCCX_TagIsCstP(l, &bgbcc_rcst_preinc , "preinc" ) ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_predec , "predec" ) ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_postinc, "postinc") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_postdec, "postdec") )
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_expr, "expr");
		i0=BGBCC_CCXL_InferExpr(ctx, t, &tty);
		if(!i0)
			return(0);
		*rdty=tty;
		return(1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_assign, "assign"))
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
//		t=BGBCC_CCXL_ReduceExpr(ctx, t);

		i0=BGBCC_CCXL_InferExpr(ctx, t, &tty);
		if(!i0)
			return(0);
		*rdty=tty;
		return(1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_cast, "cast"))
	{
		t=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");
		s0=BGBCC_CCXL_VarTypeString(ctx, t);
		if(!s0)
			return(0);
		BGBCC_CCXL_TypeFromSig(ctx, &dty, s0);
		*rdty=dty;
		return(1);

//		t=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
//		t=BGBCC_CCXL_ReduceExpr(ctx, t);
//		return(i);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_funcall, "funcall"))
	{
		s0=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		if(s0)
		{
			i0=-1;
		
			for(j=ctx->vlcl_stackpos-1; j>=0; j--)
			{
				sprintf(tb1, "%s!%d", s0, ctx->vlcl_stack[j]);
				i0=BGBCC_CCXL_TryLookupAsRegister(ctx, tb1, &treg, false);
				if(i0>0)
					break;
			}

			if(i0<=0)
				i0=BGBCC_CCXL_TryLookupAsRegister(ctx, s0, &treg, false);

			if(i0<=0)
			{
				*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_UNDEF_I);
				return(0);
			}
			tty=BGBCC_CCXL_GetRegReturnType(ctx, treg);
			*rdty=tty;
			return(1);
		}
		
		*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_UNDEF_I);
		return(0);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_if, "if"))
	{
		ln=BCCX_FetchCst(l, &bgbcc_rcst_then, "then");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_else, "else");
//		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
//		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);

		i0=BGBCC_CCXL_InferExpr(ctx, ln, &lty);
		i1=BGBCC_CCXL_InferExpr(ctx, rn, &rty);
		if(!i0 || !i1)
			return(0);

//		BGBCC_CCXL_GetTypeBinaryDest(ctx, CCXL_BINOP_ADD, lty, rty, &dty);
		BGBCC_CCXL_GetTypeCompareBinaryDest(ctx, CCXL_CMP_NV, lty, rty, &dty);
		*rdty=dty;
		return(1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_comma, "comma"))
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");
		t=BGBCC_CCXL_ReduceExpr(ctx, t);

		i0=BGBCC_CCXL_InferExpr(ctx, t, &tty);
		if(!i0)
			return(0);
		*rdty=tty;
		return(1);
	}

#if 1
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_loadindex, "loadindex"))
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_array, "array");
		t=BGBCC_CCXL_ReduceExpr(ctx, t);

		i0=BGBCC_CCXL_InferExpr(ctx, t, &tty);
		if(!i0)
			return(0);

		if(BGBCC_CCXL_TypeVariantP(ctx, tty))
		{
			*rdty=tty;
			return(1);
		}

		BGBCC_CCXL_TypeDerefType(ctx, tty, &dty);
		*rdty=dty;
		return(1);
	}
#endif

#if 1
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_objref, "objref"))
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		t=BGBCC_CCXL_ReduceExpr(ctx, t);

		i0=BGBCC_CCXL_InferExpr(ctx, t, &tty);
		if(!i0)
			return(0);

		s0=BCCX_GetCst(l, &bgbcc_rcst_name, "name");

		st=BGBCC_CCXL_LookupStructureForType(ctx, tty);

		if(BGBCC_CCXL_LookupStructFieldType(ctx, st, s0, &bty)>0)
		{
			*rdty=bty;
			return(1);
		}
		
		if(BGBCC_CCXL_TypeVecP(ctx, tty))
		{
			i=BGBCC_CCXL_VectorTypeIndexForName(ctx, tty, s0);
			if(i>=0)
			{
				BGBCC_CCXL_TypeDerefType(ctx, tty, &dty);
				*rdty=dty;
				return(1);
			}
		}

		if(BGBCC_CCXL_TypeVariantP(ctx, tty))
		{
			*rdty=tty;
			return(1);
		}

		return(0);

//		BGBCC_CCXL_CompileExpr(ctx, );
//		s0=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
//		BGBCC_CCXL_StackLoadSlot(ctx, s0);
//		return;
	}
#endif

#if 1
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_getindex, "getindex"))
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_array, "array");
		v=BCCX_FetchCst(l, &bgbcc_rcst_index, "index");
		t=BGBCC_CCXL_ReduceExpr(ctx, t);
		v=BGBCC_CCXL_ReduceExpr(ctx, v);

		i0=BGBCC_CCXL_InferExpr(ctx, t, &tty);

		if(tty.val==0x200961)	//Debug
		{
			i0=BGBCC_CCXL_InferExpr(ctx, t, &tty);
		}

		if(!i0)
			return(0);

		if(BCCX_TagIsCstP(v, &bgbcc_rcst_colon, "colon"))
		{
			ln=BCCX_FetchCst(v, &bgbcc_rcst_left, "left");
			rn=BCCX_FetchCst(v, &bgbcc_rcst_right, "right");
			ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
			rn=BGBCC_CCXL_ReduceExpr(ctx, rn);

			if(BGBCC_CCXL_IsIntP(ctx, ln) && BGBCC_CCXL_IsIntP(ctx, rn))
			{
				i0=BCCX_GetIntCst(ln, &bgbcc_rcst_value, "value");
				i1=BCCX_GetIntCst(rn, &bgbcc_rcst_value, "value");
				if(i0<i1)
					{ k=i0; i0=i1; i1=k; }

				bty=BGBCC_CCXL_MakeTypeID_Arr(ctx,
					CCXL_TY_UBITINT, (i0-i1)+1);
				*rdty=bty;
				return(1);
			}
		}

		BGBCC_CCXL_TypeDerefType(ctx, tty, &bty);
		*rdty=bty;
		return(1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_sizeof, "sizeof") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_sizeof_expr, "sizeof_expr") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_offsetof, "offsetof"))
	{
		*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I);
		return(1);
	}
#endif

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_lambda, "lambda"))
	{
		*rdty=BGBCC_CCXL_MakeTypeVTy(ctx, CCXL_VTY_PVOID);
		return(0);
	}

//	if(BCCX_TagIsCstP(l, &bgbcc_rcst_object, "object"))
	if(	BCCX_TagIsCstP(l, &bgbcc_rcst_object, "object") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_jnew, "jnew"))
	{
		t=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");

		if(t)
		{
			s0=BGBCC_CCXL_VarTypeString(ctx, t);
			if(!s0)
				return(0);
		}else
		{
			s0="r";
		}

		BGBCC_CCXL_TypeFromSig(ctx, &dty, s0);
		*rdty=dty;
		return(1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_vec4, "vec4"))
	{
		s0=BCCX_GetCst(l, &bgbcc_rcst_tysuf, "tysuf");
		i=-1;

		if(!strcmp(s0, "V3F"))i=CCXL_TY_VEC3F;
		if(!strcmp(s0, "V4F"))i=CCXL_TY_VEC4F;
		if(!strcmp(s0, "QF"))i=CCXL_TY_QUATF;

		if(!strcmp(s0, "V3FX"))i=CCXL_TY_VEC3FX;
		if(!strcmp(s0, "V3FQ"))i=CCXL_TY_VEC3FQ;

		if(!strcmp(s0, "V4SW"))i=CCXL_TY_VEC4SW;
		if(!strcmp(s0, "V4UW"))i=CCXL_TY_VEC4UW;
		if(!strcmp(s0, "V4SI"))i=CCXL_TY_VEC4SI;
		if(!strcmp(s0, "V4UI"))i=CCXL_TY_VEC4UI;

		if(!strcmp(s0, "V3SF"))i=CCXL_TY_VEC3H;
		if(!strcmp(s0, "V4SF"))i=CCXL_TY_VEC4H;

		if(i>=0)
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, i);
			return(1);
		}
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_array, "array"))
	{
		s0=BCCX_GetCst(l, &bgbcc_rcst_tysuf, "tysuf");

		if(!s0)
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_VARIANT);
			return(1);
		}
		
		s1=NULL;
		
		if(!bgbcc_stricmp(s0, "SB"))	s1="Qa";
		if(!bgbcc_stricmp(s0, "B"))		s1="Qa";
		if(!bgbcc_stricmp(s0, "SC"))	s1="Qc";
		if(!bgbcc_stricmp(s0, "D"))		s1="Qd";
		if(!bgbcc_stricmp(s0, "F"))		s1="Qf";
		if(!bgbcc_stricmp(s0, "G"))		s1="Qg";
		if(!bgbcc_stricmp(s0, "UB"))	s1="Qh";
		if(!bgbcc_stricmp(s0, "SI"))	s1="Qi";
		if(!bgbcc_stricmp(s0, "UI"))	s1="Qj";
		if(!bgbcc_stricmp(s0, "SF"))	s1="Qk";
		if(!bgbcc_stricmp(s0, "HF"))	s1="Qk";
		if(!bgbcc_stricmp(s0, "L"))		s1="Qx";
		if(!bgbcc_stricmp(s0, "UL"))	s1="Qy";
		if(!bgbcc_stricmp(s0, "LX"))	s1="Qn";
		if(!bgbcc_stricmp(s0, "ULX"))	s1="Qo";
		if(!bgbcc_stricmp(s0, "V"))		s1="Qr";
		if(!bgbcc_stricmp(s0, "SS"))	s1="Qs";
		if(!bgbcc_stricmp(s0, "US"))	s1="Qt";
		if(!bgbcc_stricmp(s0, "W"))		s1="Qw";

		BGBCC_CCXL_TypeFromSig(ctx, &dty, s1);
		*rdty=dty;
		return(1);
	}

#if 1
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_methodcall, "methodcall"))
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		t=BGBCC_CCXL_ReduceExpr(ctx, t);

		i0=BGBCC_CCXL_InferExpr(ctx, t, &tty);
		if(!i0)
			return(0);

		s0=BCCX_GetCst(l, &bgbcc_rcst_name, "name");

		st=BGBCC_CCXL_LookupStructureForType(ctx, tty);

		if(st && BGBCC_CCXL_LookupStructFieldType(ctx, st, s0, &bty)>0)
		{
			*rdty=BGBCC_CCXL_GetTypeReturnType(ctx, bty);
			return(1);
		}

		if(BGBCC_CCXL_TypeVariantP(ctx, tty))
		{
			*rdty=tty;
			return(1);
		}

		return(0);

//		BGBCC_CCXL_CompileExpr(ctx, );
//		s0=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
//		BGBCC_CCXL_StackLoadSlot(ctx, s0);
//		return;
	}
#endif

	s0=BCCX_Tag(l);

//	BGBCC_DBGBREAK

//	*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I);
	return(0);
}

/*
 &1 = Expression does not have side effects.
 &2 = Expression will not modify conditional status.
 &4 = Expression will not deref a pointer.
 */
int BGBCC_CCXL_InferExprCleanI(BGBCC_TransState *ctx, BCCX_Node *l)
{
	ccxl_register sreg, treg;
	BGBCC_CCXL_LiteralInfo *st;
	BCCX_Node *c, *t, *v, *x, *ln, *rn;
	char *s0, *s1, *s2, *s3;
	char *suf, *op;
	s64 li, lj;
	int i0, i1, i2, i3;
	int opr;
	int i;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int, "int"))
		{ return(7); }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_real, "real"))
		{ return(7); }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_string, "string"))
		{ return(7); }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_charstring, "charstring"))
		{ return(7); }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_ref, "ref"))
		{ return(7); }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_binary, "binary"))
	{
		op=BCCX_GetCst(l, &bgbcc_rcst_op, "op");
		if(!op)
			return(0);

		ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");

		i0=BGBCC_CCXL_InferExprCleanI(ctx, ln);
		i1=BGBCC_CCXL_InferExprCleanI(ctx, rn);

		opr=-1;
		if(!strcmp(op, "+"))
			opr=CCXL_BINOP_ADD;
		if(!strcmp(op, "-"))
			opr=CCXL_BINOP_SUB;
		if(!strcmp(op, "*"))
			opr=CCXL_BINOP_MUL;
		if(!strcmp(op, "&"))
			opr=CCXL_BINOP_AND;
		if(!strcmp(op, "|"))
			opr=CCXL_BINOP_OR;
		if(!strcmp(op, "^"))
			opr=CCXL_BINOP_XOR;

		if(!strcmp(op, "<<"))
			opr=CCXL_BINOP_SHL;
		if(!strcmp(op, ">>"))
			opr=CCXL_BINOP_SHR;
		if(!strcmp(op, ">>>"))
			opr=CCXL_BINOP_SHRR;

		if(opr>=0)
		{
			return(i0&i1);
		}

		opr=-1;
		if(!strcmp(op, "=="))opr=CCXL_CMP_EQ;
		if(!strcmp(op, "!="))opr=CCXL_CMP_NE;
		if(!strcmp(op, "<"))opr=CCXL_CMP_LT;
		if(!strcmp(op, ">"))opr=CCXL_CMP_GT;
		if(!strcmp(op, "<="))opr=CCXL_CMP_LE;
		if(!strcmp(op, ">="))opr=CCXL_CMP_GE;

		if(!strcmp(op, "&&"))
			opr=CCXL_BINOP_AND;
		if(!strcmp(op, "||"))
			opr=CCXL_BINOP_OR;

		if(!strcmp(op, "&&&"))
			opr=CCXL_BINOP_AND;
		if(!strcmp(op, "|||"))
			opr=CCXL_BINOP_OR;

		if(!strcmp(op, "/"))
			opr=CCXL_BINOP_DIV;
		if(!strcmp(op, "%"))
			opr=CCXL_BINOP_MOD;

		if(opr>=0)
			{ return(i0&i1&(~2)); }

		return(0);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_unary, "unary"))
	{
		op=BCCX_GetCst(l, &bgbcc_rcst_op, "op");
		if(!op)
			return(0);

		t=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		i0=BGBCC_CCXL_InferExprCleanI(ctx, t);

		if(	!strcmp(op, "!") ||
			!strcmp(op, "!!"))
		{
			return(i0&(~2));
		}

		if(!strcmp(op, "+") ||
			!strcmp(op, "-") ||
			!strcmp(op, "~"))
		{
			return(i0);
		}

		if(!strcmp(op, "*"))
		{
			return(i0&(~4));
		}

		return(0);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_assign, "assign"))
	{
		return(0);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_cast, "cast"))
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		i0=BGBCC_CCXL_InferExprCleanI(ctx, t);
		return(i0);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_funcall, "funcall"))
	{
/*
		s0=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		if(s0)
		{
			i0=BGBCC_CCXL_TryLookupAsRegister(ctx, s0, &treg, false);
			if(i0<0)
				return(0);
			tty=BGBCC_CCXL_GetRegReturnType(ctx, treg);
			*rdty=tty;
			return(1);
		}
*/
		
		return(0);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_if, "if"))
	{
		c=BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond");
		ln=BCCX_FetchCst(l, &bgbcc_rcst_then, "then");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_else, "else");
		i0=BGBCC_CCXL_InferExprCleanI(ctx, ln);
		i1=BGBCC_CCXL_InferExprCleanI(ctx, rn);
		i2=BGBCC_CCXL_InferExprCleanI(ctx, c);
		return(i0&i1&i2&(~2));
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_comma, "comma"))
	{
		ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");

		i0=BGBCC_CCXL_InferExprCleanI(ctx, ln);
		i1=BGBCC_CCXL_InferExprCleanI(ctx, rn);

		return(i0&i1);
	}

#if 1
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_loadindex, "loadindex"))
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_array, "array");
		v=BCCX_FetchCst(l, &bgbcc_rcst_index, "index");

		i0=BGBCC_CCXL_InferExprCleanI(ctx, t);
		i1=BGBCC_CCXL_InferExprCleanI(ctx, v);
		return(i0&i1&(~4));
	}
#endif

#if 1
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_objref, "objref"))
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		i0=BGBCC_CCXL_InferExprCleanI(ctx, t);
		return(i0&(~4));
	}
#endif

#if 1
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_getindex, "getindex"))
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_array, "array");
		v=BCCX_FetchCst(l, &bgbcc_rcst_index, "index");
		i0=BGBCC_CCXL_InferExprCleanI(ctx, t);
		i1=BGBCC_CCXL_InferExprCleanI(ctx, v);
		return(i0&i1&(~4));
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_sizeof, "sizeof") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_sizeof_expr, "sizeof_expr") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_offsetof, "offsetof"))
	{
		return(7);
	}
#endif

	return(0);
}

int BGBCC_CCXL_InferExprCleanP(BGBCC_TransState *ctx, BCCX_Node *l)
{
	return(BGBCC_CCXL_InferExprCleanI(ctx, l)&1);
}

int BGBCC_CCXL_InferExprCleanNoDerefP(BGBCC_TransState *ctx, BCCX_Node *l)
{
	return((BGBCC_CCXL_InferExprCleanI(ctx, l)&5)==5);
}

bool BGBCC_CCXL_TypePredSafeP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int sz;

//	return(0);

	if(BGBCC_CCXL_TypeSmallLongP(ctx, ty))
		return(1);
//	if(	BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty) &&
//		!BGBCC_CCXL_TypeQuadPointerP(ctx, ty))
	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(1);

#if 1
	if(BGBCC_CCXL_TypeSmallDoubleP(ctx, ty))
		return(1);

	if(BGBCC_CCXL_TypeValueObjectP(ctx, ty))
	{
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, ty);
		if(sz>64)
			return(0);
		if(sz&(sz-1))
			return(0);
		return(1);
	}
	
	if(BGBCC_CCXL_TypeVecP(ctx, ty))
		return(1);
	if(BGBCC_CCXL_TypeFunctionP(ctx, ty))
		return(1);
#endif

	return(0);
}

/*
Determine if block-statement is safe to be compiled as predicated instructions.

There is a small limit for block length here, and the range of operations which can be performed is quite limited.

Return value is a cost heuristic.
 */
int BGBCC_CCXL_InferBlockPredSafeP(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *n, *ln, *rn;
	ccxl_type bty, dty, lty, rty;
	char *s0;
	int i0, i1, i2, i3, na, ci;
	int i, j, k;

	if(!ctx->arch_has_predops)
		return(0);

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_begin, "begin"))
	{
		i=1; k=0;
//		c=BCCX_Child(l);
//		while(c)
//		{
		na=BCCX_GetNodeChildCount(l);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(l, ci);

			if(k>3)
				break;
//			BGBCC_CCXL_CompileStatement(ctx, c);
			j=BGBCC_CCXL_InferBlockPredSafeP(ctx, c);
//			if(!j) { i=0; break; }
			if(j<=0) { i=0; break; }
//			c=BCCX_Next(c);
			i+=j;
			k++;
		}
		
		if(k>3)
			i=0;
		if(i>12)
			return(0);
		
		return(i);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_assign, "assign"))
	{
		ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);
		BGBCC_CCXL_InferExpr(ctx, ln, &lty);
		BGBCC_CCXL_InferExpr(ctx, rn, &rty);

		if(	!BGBCC_CCXL_TypePredSafeP(ctx, lty) ||
			!BGBCC_CCXL_TypePredSafeP(ctx, rty))
		{
			return(0);
		}

		if(ctx->infer_var_loopconst)
		{
			if(BCCX_TagIsCstP(ln, &bgbcc_rcst_ref, "ref"))
			{
				s0=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");
				if(!strcmp(s0, ctx->infer_var_loopconst))
					return(0);
			}
		}

		s0=BCCX_GetCst(l, &bgbcc_rcst_op, "op");

		i0=BGBCC_CCXL_InferBlockPredSafeP(ctx, ln);
		i1=BGBCC_CCXL_InferBlockPredSafeP(ctx, rn);
//		if(!i0 || !i1)
		if((i0<=0) || (i1<=0))
			return(0);

		if(s0)
		{
			i=0;
//			if(!strcmp(s0, "+"))	i=1;
//			if(!strcmp(s0, "-"))	i=1;

			if(	!strcmp(s0, "+") ||
				!strcmp(s0, "-"))
			{
				if(	BGBCC_CCXL_TypeSmallLongP(ctx, lty) &&
					BGBCC_CCXL_TypeSmallLongP(ctx, rty) )
						i=1;
				if(	BGBCC_CCXL_TypeSmallDoubleP(ctx, lty) &&
					BGBCC_CCXL_TypeSmallDoubleP(ctx, rty) )
						i=1;

				if(	BGBCC_CCXL_TypeArrayOrPointerP(ctx, lty) &&
					BGBCC_CCXL_TypeSmallIntP(ctx, rty) )
				{
					BGBCC_CCXL_TypeDerefType(ctx, lty, &bty);
					if(	BGBCC_CCXL_TypeSmallLongP(ctx, bty) ||
						BGBCC_CCXL_TypeSmallDoubleP(ctx, bty) ||
						BGBCC_CCXL_TypePointerP(ctx, bty))
							i=1;
				}
			}

			if(!strcmp(s0, "&"))	i=1;
			if(!strcmp(s0, "|"))	i=1;
			if(!strcmp(s0, "^"))	i=1;
			if(!i)
				return(0);
		}
		
//		return(1);
		return(i0+i1+1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_binary, "binary"))
	{
		ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);
		BGBCC_CCXL_InferExpr(ctx, ln, &lty);
		BGBCC_CCXL_InferExpr(ctx, rn, &rty);

		if(	!BGBCC_CCXL_TypePredSafeP(ctx, lty) ||
			!BGBCC_CCXL_TypePredSafeP(ctx, rty))
		{
			return(0);
		}

		s0=BCCX_GetCst(l, &bgbcc_rcst_op, "op");

		i0=BGBCC_CCXL_InferBlockPredSafeP(ctx, ln);
		i1=BGBCC_CCXL_InferBlockPredSafeP(ctx, rn);
//		if(!i0 || !i1)
		if((i0<=0) || (i1<=0))
			return(0);

		if(!s0)
			return(0);

		i=0;
		if(	!strcmp(s0, "+") ||
			!strcmp(s0, "-"))
		{
			if(	BGBCC_CCXL_TypeSmallLongP(ctx, lty) &&
				BGBCC_CCXL_TypeSmallLongP(ctx, rty) )
					i=1;
			if(	BGBCC_CCXL_TypeSmallDoubleP(ctx, lty) &&
				BGBCC_CCXL_TypeSmallDoubleP(ctx, rty) )
					i=1;

			if(	BGBCC_CCXL_TypeArrayOrPointerP(ctx, lty) &&
				BGBCC_CCXL_TypeSmallIntP(ctx, rty) )
			{
				BGBCC_CCXL_TypeDerefType(ctx, lty, &bty);
				if(	BGBCC_CCXL_TypeSmallLongP(ctx, bty) ||
					BGBCC_CCXL_TypeSmallDoubleP(ctx, bty) ||
					BGBCC_CCXL_TypePointerP(ctx, bty))
						i=1;
			}
			if(	BGBCC_CCXL_TypeArrayOrPointerP(ctx, lty) &&
				(lty.val==rty.val) &&
				!strcmp(s0, "-"))
			{
				BGBCC_CCXL_TypeDerefType(ctx, lty, &bty);
				if(	BGBCC_CCXL_TypeSmallLongP(ctx, bty) ||
					BGBCC_CCXL_TypeSmallDoubleP(ctx, bty) ||
					BGBCC_CCXL_TypePointerP(ctx, bty))
						i=1;
			}
		}

		if(!strcmp(s0, "&"))	i=1;
		if(!strcmp(s0, "|"))	i=1;
		if(!strcmp(s0, "^"))	i=1;

		if(!strcmp(s0, "*"))
		{
			if(BGBCC_CCXL_TypeSmallIntP(ctx, rty))
				i=1;
		}

		if(!i)
			return(0);
		
//		return(1);
		return(i0+i1+1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_unary, "unary"))
	{
		s0=BCCX_GetCst(l, &bgbcc_rcst_op, "op");
		t=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");

		i=BGBCC_CCXL_InferExpr(ctx, l, &dty);
		if(i<=0)
			return(0);

		if(!BGBCC_CCXL_TypePredSafeP(ctx, dty))
			return(0);

		i0=BGBCC_CCXL_InferBlockPredSafeP(ctx, t);
//		if(!i)
		if(i0<=0)
			return(0);

		i=0;
		if(!strcmp(s0, "+"))	i=1;
		if(!strcmp(s0, "-"))	i=1;
		if(!strcmp(s0, "&"))	i=1;
		if(!strcmp(s0, "*"))	i=1;
		if(!i)
			return(0);
		
//		return(1);
		return(i0+1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_funcall_intrin, "funcall_intrin"))
	{
		i=2; k=0;
//		c=BCCX_FetchCst(l, &bgbcc_rcst_args, "args");
		n=BCCX_FindTagCst(l, &bgbcc_rcst_args, "args");
//		while(c)
//		{

		na=BCCX_GetNodeChildCount(n);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(n, ci);

			if(k>3)
				break;
//			BGBCC_CCXL_CompileStatement(ctx, c);
			j=BGBCC_CCXL_InferBlockPredSafeP(ctx, c);
//			if(!j) { i=0; break; }
			if(j<=0) { i=0; break; }
//			c=BCCX_Next(c);
			i+=j;
			k++;
		}
		
		if(k>3)
			i=0;
		if(i>12)
			return(0);
		
		return(i);
	}

#if 1
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_getindex, "getindex") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_vector_ref, "vector-ref"))
	{
		ln=BCCX_FetchCst(l, &bgbcc_rcst_array, "array");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_index, "index");

		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);
		BGBCC_CCXL_InferExpr(ctx, ln, &lty);
		BGBCC_CCXL_InferExpr(ctx, rn, &rty);

		if(!BGBCC_CCXL_TypeArrayOrPointerP(ctx, lty))
			return(0);
		if(!BGBCC_CCXL_TypeSmallIntP(ctx, rty))
			return(0);

		BGBCC_CCXL_TypeDerefType(ctx, lty, &bty);
//		if(!BGBCC_CCXL_TypePredSafeP(ctx, bty))
//			return(0);

		if(	!BGBCC_CCXL_TypeSmallLongP(ctx, bty) &&
			!BGBCC_CCXL_TypeArrayOrPointerP(ctx, bty))
				return(0);

//		if(	!BGBCC_CCXL_TypePredSafeP(ctx, lty) ||
//			!BGBCC_CCXL_TypePredSafeP(ctx, rty))
//		{
//			return(0);
//		}

		i0=BGBCC_CCXL_InferBlockPredSafeP(ctx, ln);
		i1=BGBCC_CCXL_InferBlockPredSafeP(ctx, rn);
		if((i0<=0) || (i1<=0))
			return(0);
		
		return(i0+i1+1);
	}
#endif

#if 1
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_objref, "objref"))
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");

		i=BGBCC_CCXL_InferExpr(ctx, t, &bty);
		if(i<=0)
			return(0);

		i=BGBCC_CCXL_InferExpr(ctx, l, &dty);
		if(i<=0)
			return(0);

//		if(!BGBCC_CCXL_TypePredSafeP(ctx, bty))
		if(!BGBCC_CCXL_TypeArrayOrPointerP(ctx, bty))
			return(0);

//		if(!BGBCC_CCXL_TypePredSafeP(ctx, dty))
//			return(0);
		if(	!BGBCC_CCXL_TypeSmallLongP(ctx, dty) &&
			!BGBCC_CCXL_TypeArrayOrPointerP(ctx, dty))
			return(0);

		i0=BGBCC_CCXL_InferBlockPredSafeP(ctx, t);
		if(i0<=0)
			return(0);

		return(i0+1);
	}
#endif

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_return, "return"))
		{ return(0); }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int, "int"))
		{ return(1); }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_charstring, "charstring"))
		{ return(1); }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_ref, "ref"))
	{
		i=BGBCC_CCXL_InferExpr(ctx, l, &dty);
		if(i<=0)
			return(0);

		if(BGBCC_CCXL_TypePredSafeP(ctx, dty))
			return(1);

#if 0
//		if(BGBCC_CCXL_TypeSmallIntP(ctx, dty))
		if(BGBCC_CCXL_TypeSmallLongP(ctx, dty))
			return(1);
		if(	BGBCC_CCXL_TypeArrayOrPointerP(ctx, dty) &&
			!BGBCC_CCXL_TypeQuadPointerP(ctx, dty))
				return(1);
#endif
		return(0);
	}
	
	return(0);
}


int BGBCC_CCXL_InferBlockCost(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *n, *ln, *rn;
	ccxl_type bty, dty, lty, rty;
	char *s0;
	int i0, i1, i2, i3, na, ci;
	int i, j, k;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_begin, "begin"))
	{
		i=1; k=0;
		na=BCCX_GetNodeChildCount(l);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(l, ci);

			if(k>3)
				break;
			j=BGBCC_CCXL_InferBlockCost(ctx, c);
			if(j<0)
				j=0;
			i+=j;
			k++;
		}
		
		return(i);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_assign, "assign"))
	{
		ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);
		BGBCC_CCXL_InferExpr(ctx, ln, &lty);
		BGBCC_CCXL_InferExpr(ctx, rn, &rty);

		s0=BCCX_GetCst(l, &bgbcc_rcst_op, "op");

		i0=BGBCC_CCXL_InferBlockCost(ctx, ln);
		i1=BGBCC_CCXL_InferBlockCost(ctx, rn);
		if((i0<0) || (i1<0))
			return(-1);

#if 0
		if(s0)
		{
			i=0;
			if(!strcmp(s0, "+"))	i=1;
			if(!strcmp(s0, "-"))	i=1;
			if(!strcmp(s0, "&"))	i=1;
			if(!strcmp(s0, "|"))	i=1;
			if(!strcmp(s0, "^"))	i=1;
			if(!i)
				return(0);
		}
#endif
		
		return(i0+i1+1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_binary, "binary"))
	{
		ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);

		s0=BCCX_GetCst(l, &bgbcc_rcst_op, "op");

		i0=BGBCC_CCXL_InferBlockCost(ctx, ln);
		i1=BGBCC_CCXL_InferBlockCost(ctx, rn);
		if((i0<0) || (i1<0))
			return(-1);

#if 0
		if(!s0)
			return(0);

		i=0;
		if(!strcmp(s0, "+"))	i=1;
		if(!strcmp(s0, "-"))	i=1;
		if(!strcmp(s0, "&"))	i=1;
		if(!strcmp(s0, "|"))	i=1;
		if(!strcmp(s0, "^"))	i=1;
		if(!i)
			return(0);
#endif
		
		return(i0+i1+1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_unary, "unary"))
	{
		s0=BCCX_GetCst(l, &bgbcc_rcst_op, "op");
		t=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");

		i0=BGBCC_CCXL_InferBlockCost(ctx, t);
		if(i0<0)
			return(-1);

#if 0
		i=0;
		if(!strcmp(s0, "+"))	i=1;
		if(!strcmp(s0, "-"))	i=1;
		if(!strcmp(s0, "&"))	i=1;
		if(!strcmp(s0, "*"))	i=1;
		if(!i)
			return(0);
#endif

		return(i0+1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_funcall_intrin, "funcall_intrin"))
	{
		i=2; k=0;
		n=BCCX_FindTagCst(l, &bgbcc_rcst_args, "args");

		na=BCCX_GetNodeChildCount(n);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(n, ci);

			if(k>3)
				break;
			j=BGBCC_CCXL_InferBlockCost(ctx, c);
			if(j<0)
				j=0;
			i+=j;
			k++;
		}
		
		return(i);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_return, "return"))
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		i0=BGBCC_CCXL_InferBlockCost(ctx, t);
		if(i0<0)
			return(-1);
		return(i0+1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int, "int"))
		{ return(1); }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_charstring, "charstring"))
		{ return(1); }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_ref, "ref"))
	{
		return(1);
	}
	
	return(1);
}

int BGBCC_CCXL_InferExprLogicFoldCostP(BGBCC_TransState *ctx, BCCX_Node *l)
{
	int cf;
	
//	cf=BGBCC_CCXL_InferBlockPredSafeP(ctx, l);
	cf=BGBCC_CCXL_InferBlockCost(ctx, l);
	if(cf<=0)
		return(0);
	if(cf>12)
		return(0);
	return(1);
}

int BGBCC_CCXL_InferExprSimpleCmpP(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *ln, *rn;
	ccxl_type bty, dty, lty, rty;
	char *s0;
	int i0, i1, i2, i3;
	int i, j, k;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_binary, "binary"))
	{
		ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);
		BGBCC_CCXL_InferExpr(ctx, ln, &lty);
		BGBCC_CCXL_InferExpr(ctx, rn, &rty);

		s0=BCCX_GetCst(l, &bgbcc_rcst_op, "op");
		j=BGBCC_CCXL_CompareOpIdForName(ctx, s0);
//		j=BGBCC_CCXL_BinaryCmpOpIdForName(ctx, s0);
		if(j<0)
			return(0);
		
//		BGBCC_CCXL_GetTypeBinaryDest(ctx, j, lty, rty, &dty);
		BGBCC_CCXL_GetTypeCompareBinaryDest(ctx, j, lty, rty, &dty);

		if(BGBCC_CCXL_TypeSmallIntP(ctx, dty))
			return(1);
		if(	BGBCC_CCXL_TypeArrayOrPointerP(ctx, dty) &&
			!BGBCC_CCXL_TypeQuadPointerP(ctx, dty))
				return(1);
		return(0);

//		if(	!BGBCC_CCXL_TypeSmallIntP(ctx, dty) &&
//			!BGBCC_CCXL_TypeArrayOrPointerP(ctx, dty))
//				return(0);
//		return(1);
	}

#if 1
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_unary, "unary"))
	{
		ln=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		i=BGBCC_CCXL_InferExpr(ctx, ln, &lty);
		if(i<=0)
			return(0);

		s0=BCCX_GetCst(l, &bgbcc_rcst_op, "op");
		
		if(	!strcmp(s0, "!") ||
			!strcmp(s0, "!!"))
		{
			i=BGBCC_CCXL_InferExprSimpleCmpP(ctx, ln);
			return(i);
		}
		return(0);
	}
#endif

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int, "int"))
		{ return(1); }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_charstring, "charstring"))
		{ return(1); }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_ref, "ref"))
	{
		i=BGBCC_CCXL_InferExpr(ctx, l, &dty);
		if(i<=0)
			return(0);

		if(BGBCC_CCXL_TypeSmallIntP(ctx, dty))
			return(1);
		if(	BGBCC_CCXL_TypeArrayOrPointerP(ctx, dty) &&
			!BGBCC_CCXL_TypeQuadPointerP(ctx, dty))
			return(1);
		return(0);
	}
	
	return(0);
}

int BGBCC_CCXL_InferBlockCapRef(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *n, *ln, *rn;
	ccxl_type bty, dty, lty, rty;
	char *s0;
	int i0, i1, i2, i3, na, ci;
	int i, j, k;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_begin, "begin"))
	{
		i=1; k=0;
//		c=BCCX_Child(l);
//		while(c)
//		{
		na=BCCX_GetNodeChildCount(l);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(l, ci);

			BGBCC_CCXL_InferBlockCapRef(ctx, c);
//			c=BCCX_Next(c);
		}
		return(i);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_assign, "assign"))
	{
		ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);
		BGBCC_CCXL_InferExpr(ctx, ln, &lty);
		BGBCC_CCXL_InferExpr(ctx, rn, &rty);

		s0=BCCX_GetCst(l, &bgbcc_rcst_op, "op");

//		i0=BGBCC_CCXL_InferBlockPredSafeP(ctx, ln);
//		i1=BGBCC_CCXL_InferBlockPredSafeP(ctx, rn);

		return(1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_binary, "binary"))
	{
		ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);

		s0=BCCX_GetCst(l, &bgbcc_rcst_op, "op");

		BGBCC_CCXL_InferExpr(ctx, ln, &lty);
		BGBCC_CCXL_InferExpr(ctx, rn, &rty);
		
		return(1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_unary, "unary"))
	{
		s0=BCCX_GetCst(l, &bgbcc_rcst_op, "op");
		t=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		BGBCC_CCXL_InferExpr(ctx, t, &lty);

		return(1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_funcall_intrin, "funcall_intrin"))
	{
		i=2; k=0;
//		c=BCCX_FetchCst(l, &bgbcc_rcst_args, "args");
		n=BCCX_FindTagCst(l, &bgbcc_rcst_args, "args");
//		while(c)
//		{
		na=BCCX_GetNodeChildCount(n);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(n, ci);

//			j=BGBCC_CCXL_InferBlockPredSafeP(ctx, c);
			BGBCC_CCXL_InferExpr(ctx, c, &lty);
//			c=BCCX_Next(c);
		}
		
		return(i);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_return, "return"))
	{
		c=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		BGBCC_CCXL_InferExpr(ctx, c, &lty);
		return(0);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int, "int"))
		{ return(1); }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_charstring, "charstring"))
		{ return(1); }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_ref, "ref"))
	{
		i=BGBCC_CCXL_InferExpr(ctx, l, &dty);
		return(0);
	}
	
	return(0);
}

int BGBCC_CCXL_InferLambdaCapRef(BGBCC_TransState *ctx, BCCX_Node *l)
{
	ctx->n_lambda_capref=0;
	ctx->do_lambda_capref=ctx->vlcl_stackpos;

	BGBCC_CCXL_InferBlockCapRef(ctx, l);

	ctx->do_lambda_capref=0;
	return(ctx->n_lambda_capref);
}

/* Infer if two expressions are equivalent. */
int BGBCC_CCXL_InferExprIsEquivP(BGBCC_TransState *ctx,
	BCCX_Node *ln, BCCX_Node *rn)
{
	BCCX_Node *ln1, *rn1;
	BCCX_Node *ln2, *rn2;
	s64 li, lj;
	char *s0, *s1;

	if(	BCCX_TagIsCstP(ln, &bgbcc_rcst_ref, "ref") &&
		BCCX_TagIsCstP(rn, &bgbcc_rcst_ref, "ref"))
	{
		s0=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");
		s1=BCCX_GetCst(rn, &bgbcc_rcst_name, "name");
		if(!strcmp(s0, s1))
			return(1);
		return(0);
	}

	if(	BCCX_TagIsCstP(ln, &bgbcc_rcst_int, "int") &&
		BCCX_TagIsCstP(rn, &bgbcc_rcst_int, "int"))
	{
		li=BCCX_GetIntCst(ln, &bgbcc_rcst_value, "value");
		lj=BCCX_GetIntCst(rn, &bgbcc_rcst_value, "value");
		if(li==lj)
			return(1);
		return(0);
	}

	if(	BCCX_TagIsCstP(ln, &bgbcc_rcst_binary, "binary") &&
		BCCX_TagIsCstP(rn, &bgbcc_rcst_binary, "binary"))
	{
		ln1=BCCX_FetchCst(ln, &bgbcc_rcst_left, "left");
		rn1=BCCX_FetchCst(ln, &bgbcc_rcst_right, "right");

		ln2=BCCX_FetchCst(rn, &bgbcc_rcst_left, "left");
		rn2=BCCX_FetchCst(rn, &bgbcc_rcst_right, "right");

		s0=BCCX_GetCst(ln, &bgbcc_rcst_op, "op");
		s1=BCCX_GetCst(rn, &bgbcc_rcst_op, "op");

		if(!strcmp(s0, s1))
		{
			if(	BGBCC_CCXL_InferExprIsEquivP(ctx, ln1, ln2) &&
				BGBCC_CCXL_InferExprIsEquivP(ctx, rn1, rn2))
			{
				return(1);
			}
			return(0);
		}

		return(0);
	}

	return(0);
}
