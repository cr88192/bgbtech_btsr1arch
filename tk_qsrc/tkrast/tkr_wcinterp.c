TKRA_ShaderTrace *TKRA_AllocShaderTrace(TKRA_Context *ctx)
{
	TKRA_ShaderTrace *tr;
	
	tr=ctx->sdr_tr_free;
	if(tr)
	{
		ctx->sdr_tr_free=tr->tr_next;
		return(tr);
	}
	
	tr=malloc(sizeof(TKRA_ShaderTrace));
	memset(tr, 0, sizeof(TKRA_ShaderTrace));
	return(tr);
}

TKRA_ShaderOp *TKRA_AllocShaderOp(TKRA_Context *ctx)
{
	TKRA_ShaderOp *op;
	
	op=ctx->sdr_op_free;
	if(op)
	{
		ctx->sdr_op_free=*(TKRA_ShaderOp **)op;
		memset(op, 0, sizeof(TKRA_ShaderOp));
		return(op);
	}

	op=malloc(sizeof(TKRA_ShaderOp));
	memset(op, 0, sizeof(TKRA_ShaderOp));
	return(op);
}

TKRA_ShaderProg	*TKRA_LookupShaderProg(TKRA_Context *ctx, int id)
{
	TKRA_ShaderProg	*prg;

	prg=ctx->sdr_progs;
	while(prg)
	{
		if(prg->sdr_id==id)
			return(prg);
		prg=prg->next;
	}
	return(NULL);
}

TKRA_ShaderTrace *tkra_TrOp_Run0Null(TKRA_Context *ctx, TKRA_ShaderTrace *tr)
{
	return(NULL);
}

TKRA_ShaderTrace *tkra_TrOp_Run1(TKRA_Context *ctx, TKRA_ShaderTrace *tr)
{
	TKRA_ShaderOp		**ops;
	ctx->sdr_tr_next=tr->tr_next;
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->sdr_tr_next);
}

TKRA_ShaderTrace *tkra_TrOp_Run2(TKRA_Context *ctx, TKRA_ShaderTrace *tr)
{
	TKRA_ShaderOp		**ops;
	ctx->sdr_tr_next=tr->tr_next;
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->sdr_tr_next);
}

TKRA_ShaderTrace *tkra_TrOp_Run3(TKRA_Context *ctx, TKRA_ShaderTrace *tr)
{
	TKRA_ShaderOp		**ops;
	ctx->sdr_tr_next=tr->tr_next;
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->sdr_tr_next);
}

TKRA_ShaderTrace *tkra_TrOp_Run4(TKRA_Context *ctx, TKRA_ShaderTrace *tr)
{
	TKRA_ShaderOp		**ops;
	ctx->sdr_tr_next=tr->tr_next;
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->sdr_tr_next);
}

TKRA_ShaderTrace *tkra_TrOp_Run5(TKRA_Context *ctx, TKRA_ShaderTrace *tr)
{
	TKRA_ShaderOp		**ops;
	ctx->sdr_tr_next=tr->tr_next;
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->sdr_tr_next);
}

TKRA_ShaderTrace *tkra_TrOp_Run6(TKRA_Context *ctx, TKRA_ShaderTrace *tr)
{
	TKRA_ShaderOp		**ops;
	ctx->sdr_tr_next=tr->tr_next;
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->sdr_tr_next);
}

TKRA_ShaderTrace *tkra_TrOp_Run7(TKRA_Context *ctx, TKRA_ShaderTrace *tr)
{
	TKRA_ShaderOp		**ops;
	ctx->sdr_tr_next=tr->tr_next;
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->sdr_tr_next);
}

TKRA_ShaderTrace *tkra_TrOp_Run8(TKRA_Context *ctx, TKRA_ShaderTrace *tr)
{
	TKRA_ShaderOp		**ops;
	ctx->sdr_tr_next=tr->tr_next;
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->sdr_tr_next);
}

TKRA_ShaderTrace *tkra_TrOp_Run9(TKRA_Context *ctx, TKRA_ShaderTrace *tr)
{
	TKRA_ShaderOp		**ops;
	ctx->sdr_tr_next=tr->tr_next;
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->sdr_tr_next);
}

TKRA_ShaderTrace *tkra_TrOp_Run10(TKRA_Context *ctx, TKRA_ShaderTrace *tr)
{
	TKRA_ShaderOp		**ops;
	ctx->sdr_tr_next=tr->tr_next;
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->sdr_tr_next);
}

TKRA_ShaderTrace *tkra_TrOp_Run11(TKRA_Context *ctx, TKRA_ShaderTrace *tr)
{
	TKRA_ShaderOp		**ops;
	ctx->sdr_tr_next=tr->tr_next;
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->sdr_tr_next);
}

TKRA_ShaderTrace *tkra_TrOp_Run12(TKRA_Context *ctx, TKRA_ShaderTrace *tr)
{
	TKRA_ShaderOp		**ops;
	ctx->sdr_tr_next=tr->tr_next;
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->sdr_tr_next);
}

TKRA_ShaderTrace *tkra_TrOp_Run13(TKRA_Context *ctx, TKRA_ShaderTrace *tr)
{
	TKRA_ShaderOp		**ops;
	ctx->sdr_tr_next=tr->tr_next;
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->sdr_tr_next);
}

TKRA_ShaderTrace *tkra_TrOp_Run14(TKRA_Context *ctx, TKRA_ShaderTrace *tr)
{
	TKRA_ShaderOp		**ops;
	ctx->sdr_tr_next=tr->tr_next;
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->sdr_tr_next);
}

TKRA_ShaderTrace *tkra_TrOp_Run15(TKRA_Context *ctx, TKRA_ShaderTrace *tr)
{
	TKRA_ShaderOp		**ops;
	ctx->sdr_tr_next=tr->tr_next;
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->sdr_tr_next);
}

TKRA_ShaderTrace *tkra_TrOp_Run16(TKRA_Context *ctx, TKRA_ShaderTrace *tr)
{
	TKRA_ShaderOp		**ops;
	ctx->sdr_tr_next=tr->tr_next;
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->sdr_tr_next);
}

void TKRA_ShaderOp_End(TKRA_Context *ctx, TKRA_ShaderOp *op)
{
	ctx->sdr_tr_next=NULL;
}

float TKRA_ShaderOp_RealGet(TKRA_Context *ctx, int reg)
{
	int ix;
	if(reg<0x40)
	{
		ix=(reg-0x00)<<1;
		return(*(float *)(ctx->tkgl_sdr_local+ix));
	}else
		if(reg<0x80)
	{
		ix=(reg-0x40)<<1;
		return(*(float *)(ctx->tkgl_sdr_attrib+ix));
	}else
	{
		ix=(reg-0x80)<<1;
		return(*(float *)(ctx->tkgl_sdr_uniform+ix));
	}
}

void TKRA_ShaderOp_RealSet(TKRA_Context *ctx, int reg, float val)
{
	int ix;
	if(reg<0x40)
	{
		ix=(reg-0x00)<<1;
		*(float *)(ctx->tkgl_sdr_local+ix)=val;
	}else
		if(reg<0x80)
	{
		ix=(reg-0x40)<<1;
		*(float *)(ctx->tkgl_sdr_attrib+ix)=val;
	}else
	{
		ix=(reg-0x80)<<1;
		*(float *)(ctx->tkgl_sdr_uniform+ix)=val;
	}
}

tkra_vec4f TKRA_ShaderOp_Vec4Get(TKRA_Context *ctx, int reg)
{
	int ix;
	if(reg<0x40)
	{
		ix=(reg-0x00)<<1;
		return(tkra_mkvec4fv(ctx->tkgl_sdr_local+ix));
	}else
		if(reg<0x80)
	{
		ix=(reg-0x40)<<1;
		return(tkra_mkvec4fv(ctx->tkgl_sdr_attrib+ix));
	}else
	{
		ix=(reg-0x80)<<1;
		return(tkra_mkvec4fv(ctx->tkgl_sdr_uniform+ix));
	}
}

void TKRA_ShaderOp_Vec4Set(TKRA_Context *ctx, int reg, tkra_vec4f vec)
{
	int ix;
	if(reg<0x40)
	{
		ix=(reg-0x00)<<1;
		tkra_upvec4fv(ctx->tkgl_sdr_local+ix, vec);
	}else
		if(reg<0x80)
	{
		ix=(reg-0x40)<<1;
		tkra_upvec4fv(ctx->tkgl_sdr_attrib+ix, vec);
	}else
	{
		ix=(reg-0x80)<<1;
		tkra_upvec4fv(ctx->tkgl_sdr_uniform+ix, vec);
	}
}

void TKRA_ShaderOp_MovV4(TKRA_Context *ctx, TKRA_ShaderOp *op)
{
	tkra_vec4f v;
	v=TKRA_ShaderOp_Vec4Get(ctx, op->rs);
	TKRA_ShaderOp_Vec4Set(ctx, op->rn, v);
}

void TKRA_ShaderOp_NegV4(TKRA_Context *ctx, TKRA_ShaderOp *op)
{
	tkra_vec4f v;
	v=TKRA_ShaderOp_Vec4Get(ctx, op->rs);
	v=tkra_v4f_scale(v, -1.0);
	TKRA_ShaderOp_Vec4Set(ctx, op->rn, v);
}

void TKRA_ShaderOp_RsqV4(TKRA_Context *ctx, TKRA_ShaderOp *op)
{
	tkra_vec4f v;
	v=TKRA_ShaderOp_Vec4Get(ctx, op->rs);

	v=tkra_mkvec4f(
		1.0/sqrt(tkra_v4f_x(v)),
		1.0/sqrt(tkra_v4f_y(v)),
		1.0/sqrt(tkra_v4f_z(v)),
		1.0/sqrt(tkra_v4f_w(v))	);

//	v=tkra_v4f_scale(v, -1.0);
	TKRA_ShaderOp_Vec4Set(ctx, op->rn, v);
}

void TKRA_ShaderOp_RcpV4(TKRA_Context *ctx, TKRA_ShaderOp *op)
{
	tkra_vec4f v;
	v=TKRA_ShaderOp_Vec4Get(ctx, op->rs);
	
	v=tkra_mkvec4f(
		1.0/tkra_v4f_x(v),
		1.0/tkra_v4f_y(v),
		1.0/tkra_v4f_z(v),
		1.0/tkra_v4f_w(v)	);
	
//	v=tkra_v4f_scale(v, -1.0);
	TKRA_ShaderOp_Vec4Set(ctx, op->rn, v);
}

void TKRA_ShaderOp_AddV4(TKRA_Context *ctx, TKRA_ShaderOp *op)
{
	tkra_vec4f va, vb, vc;
	va=TKRA_ShaderOp_Vec4Get(ctx, op->rs);
	vb=TKRA_ShaderOp_Vec4Get(ctx, op->rt);
	vc=tkra_v4fadd(va, vb);
	TKRA_ShaderOp_Vec4Set(ctx, op->rn, vc);
}

void TKRA_ShaderOp_SubV4(TKRA_Context *ctx, TKRA_ShaderOp *op)
{
	tkra_vec4f va, vb, vc;
	va=TKRA_ShaderOp_Vec4Get(ctx, op->rs);
	vb=TKRA_ShaderOp_Vec4Get(ctx, op->rt);
	vc=tkra_v4fsub(va, vb);
	TKRA_ShaderOp_Vec4Set(ctx, op->rn, vc);
}

void TKRA_ShaderOp_MulV4(TKRA_Context *ctx, TKRA_ShaderOp *op)
{
	tkra_vec4f va, vb, vc;
	va=TKRA_ShaderOp_Vec4Get(ctx, op->rs);
	vb=TKRA_ShaderOp_Vec4Get(ctx, op->rt);
	vc=tkra_v4fmul(va, vb);
	TKRA_ShaderOp_Vec4Set(ctx, op->rn, vc);
}

void TKRA_ShaderOp_DivV4(TKRA_Context *ctx, TKRA_ShaderOp *op)
{
	tkra_vec4f va, vb, vbi, vc;
	va=TKRA_ShaderOp_Vec4Get(ctx, op->rs);
	vb=TKRA_ShaderOp_Vec4Get(ctx, op->rt);
	vbi=tkra_mkvec4f(
		1.0/tkra_v4f_x(vb),
		1.0/tkra_v4f_y(vb),
		1.0/tkra_v4f_z(vb),
		1.0/tkra_v4f_w(vb)	);
	vc=tkra_v4fmul(va, vbi);
	TKRA_ShaderOp_Vec4Set(ctx, op->rn, vc);
}

void TKRA_ShaderOp_Dot4V4(TKRA_Context *ctx, TKRA_ShaderOp *op)
{
	tkra_vec4f va, vb, vc;
	float c;
	va=TKRA_ShaderOp_Vec4Get(ctx, op->rs);
	vb=TKRA_ShaderOp_Vec4Get(ctx, op->rt);
	c=tkra_v4fdot(va, vb);
	TKRA_ShaderOp_RealSet(ctx, op->rn, c);
}

void TKRA_ShaderOp_Dot3V4(TKRA_Context *ctx, TKRA_ShaderOp *op)
{
	tkra_vec4f va, vb, vc;
	float c;
	va=TKRA_ShaderOp_Vec4Get(ctx, op->rs);
	vb=TKRA_ShaderOp_Vec4Get(ctx, op->rt);
	c=tkra_v3fdot(va, vb);
	TKRA_ShaderOp_RealSet(ctx, op->rn, c);
}

void TKRA_ShaderOp_DothV4(TKRA_Context *ctx, TKRA_ShaderOp *op)
{
	tkra_vec4f va, vb, vc;
	float c;
	va=TKRA_ShaderOp_Vec4Get(ctx, op->rs);
	vb=TKRA_ShaderOp_Vec4Get(ctx, op->rt);
	c=tkra_v3fdot(va, vb);
	TKRA_ShaderOp_RealSet(ctx, op->rn, c);
}

void TKRA_ShaderOp_DistV4(TKRA_Context *ctx, TKRA_ShaderOp *op)
{
	tkra_vec4f va, vb, vc;
	float c;
	va=TKRA_ShaderOp_Vec4Get(ctx, op->rs);
	vb=TKRA_ShaderOp_Vec4Get(ctx, op->rt);
	c=sqrt(tkra_v4f_dist(va, vb));
	TKRA_ShaderOp_RealSet(ctx, op->rn, c);
}

void TKRA_ShaderOp_CrossV4(TKRA_Context *ctx, TKRA_ShaderOp *op)
{
	tkra_vec4f va, vb, vc;
	va=TKRA_ShaderOp_Vec4Get(ctx, op->rs);
	vb=TKRA_ShaderOp_Vec4Get(ctx, op->rt);
	vc=tkra_v4fcross(va, vb);
	TKRA_ShaderOp_Vec4Set(ctx, op->rn, vc);
}

void TKRA_DecodeShaderOp(TKRA_Context *ctx,
	TKRA_ShaderOp *op, u64 opw)
{
	op->rn=(opw>> 8)&255;
	op->rs=(opw>>16)&255;
	op->rt=(opw>>24)&255;
	op->imm=(u32)(opw>>32);

	switch(opw&63)
	{
	case 0x00:
		switch((opw>>32)&255)
		{
		case 0x00:
			op->Run=TKRA_ShaderOp_End;
			op->opfl|=1;
			break;
		}
		break;
	case 0x01:
		switch((opw>>32)&255)
		{
		case 0x00:
			op->Run=TKRA_ShaderOp_MovV4;
			break;
		case 0x01:
			op->Run=TKRA_ShaderOp_NegV4;
			break;
		case 0x02:
			op->Run=TKRA_ShaderOp_RsqV4;
			break;
		case 0x03:
			op->Run=TKRA_ShaderOp_RcpV4;
			break;
		}
		break;
	case 0x02:
		switch((opw>>32)&255)
		{
		case 0x00:
			op->Run=TKRA_ShaderOp_AddV4;
			break;
		case 0x01:
			op->Run=TKRA_ShaderOp_SubV4;
			break;
		case 0x02:
			op->Run=TKRA_ShaderOp_MulV4;
			break;
		case 0x03:
			op->Run=TKRA_ShaderOp_DivV4;
			break;

		case 0x04:
			op->Run=TKRA_ShaderOp_Dot4V4;
			break;
		case 0x05:
			op->Run=TKRA_ShaderOp_Dot3V4;
			break;
		case 0x06:
			op->Run=TKRA_ShaderOp_DothV4;
			break;
		case 0x07:
			op->Run=TKRA_ShaderOp_DistV4;
			break;
		case 0x08:
			op->Run=TKRA_ShaderOp_CrossV4;
			break;
		}
		break;
	}
}

void TKRA_DecodeShaderTrace(TKRA_Context *ctx,
	TKRA_ShaderTrace *tr, u32 idpc)
{
	TKRA_ShaderOp	*ops;
	TKRA_ShaderProg	*prg;
	TKRA_ShaderOp	*op;
	u64 *pimg;
	u64 opw;
	int pc, n;
	
	prg=TKRA_LookupShaderProg(ctx, idpc>>16);
	if(!prg)
	{
		tr->Run=tkra_TrOp_Run0Null;
		tr->n_ops=0;
		return;
	}
	
	pimg=prg->wordcode_img[(idpc>>13)&3];
	pc=idpc&8191;
	
	ops=tr->ops; n=0;
	while(n<TKRA_SHADER_MAXTROPS)
	{
		opw=pimg[pc];
		pc++;
		op=TKRA_AllocShaderOp(ctx);
		TKRA_DecodeShaderOp(ctx, op, opw);
		ops[n++]=op;
		if(op->opfl&1)
			break;
	}
	tr->n_ops=n;
	
	switch(n)
	{
		case 1:		tr->Run=tkra_TrOp_Run1; break;
		case 2:		tr->Run=tkra_TrOp_Run2; break;
		case 3:		tr->Run=tkra_TrOp_Run3; break;
		case 4:		tr->Run=tkra_TrOp_Run4; break;
		case 5:		tr->Run=tkra_TrOp_Run5; break;
		case 6:		tr->Run=tkra_TrOp_Run6; break;
		case 7:		tr->Run=tkra_TrOp_Run7; break;
		case 8:		tr->Run=tkra_TrOp_Run8; break;
		case 9:		tr->Run=tkra_TrOp_Run9; break;
		case 10:	tr->Run=tkra_TrOp_Run10; break;
		case 11:	tr->Run=tkra_TrOp_Run11; break;
		case 12:	tr->Run=tkra_TrOp_Run12; break;
		case 13:	tr->Run=tkra_TrOp_Run13; break;
		case 14:	tr->Run=tkra_TrOp_Run14; break;
		case 15:	tr->Run=tkra_TrOp_Run15; break;
		case 16:	tr->Run=tkra_TrOp_Run16; break;
		default:	tr->Run=tkra_TrOp_Run0Null; break;
	}
}

TKRA_ShaderTrace *TKRA_GetShaderTrace(TKRA_Context *ctx, u32 idpc)
{
	TKRA_ShaderTrace *tr;
	int i, h;
	
	h=((idpc*(251*65521))>>24)&255;
	i=ctx->sdr_trhash[h];
	while(i>0)
	{
		tr=ctx->sdr_trcache[i]
		if(tr->idpc==idpc)
			return(tr);
		i=ctx->sdr_trchain[i];
	}
	
	i=ctx->sdr_n_trace++;
	
	tr=TKRA_AllocShaderTrace(ctx);
	tr->idpc=idpc;
	
	ctx->sdr_trcache[i]=tr;
	ctx->sdr_trchain[i]=ctx->sdr_trhash[h];
	ctx->sdr_trhash[h]=i;
	
	TKRA_DecodeShaderTrace(ctx, tr, idpc);
	
	return(tr);
}

void TKRA_SetupCurrentVertexShader(TKRA_Context *ctx)
{
	TKRA_ShaderProg	*prg;
	TKRA_ShaderTrace *tr;
	int id;

	if(ctx->tkgl_usepgm_vtx<=0)
		return;

	id=ctx->tkgl_usepgm_vtx;
	prg=ctx->sdr_prog_cur;
	if(!prg || (prg->sdr_id!=id))
	{
		prg=TKRA_LookupShaderProg(ctx, id);
		if(!prg)
		{
			return;
		}
		ctx->sdr_prog_cur=prg;
		tr=TKRA_GetShaderTrace(ctx, id<<16);
		ctx->sdr_tr_e_vtx=tr;
	}
}
	
void TKRA_RunCurrentVertexShader(TKRA_Context *ctx)
{
	TKRA_ShaderTrace *tr;
	tr=ctx->sdr_tr_e_vtx;
	while(tr)
	{
		tr=tr->Run(ctx, tr);
	}
}
