float	__fpu_frcp_s(float x);

no_cull		tkra_vec4f		tkra_prj_xyzsc;
no_cull		tkra_vec4f		tkra_prj_xyzbi;

no_cull int TKRA_DummyTouchVars(void)
{
	int i;
	i=((byte *)(&tkra_prj_xyzsc))-((byte *)0);
	i+=((byte *)(&tkra_prj_xyzbi))-((byte *)0);
	return(i);
}

#ifdef __BJX2__
// #if 0

tkra_vec4f TKRA_ProjectVertexB(tkra_vec4f vec, tkra_mat4 mat);

__asm {
TKRA_ProjectVertexB:
	MOV.X		(R6,  0), R22
	MOVLD		R4, R4, R20		|	MOVLD	R4, R4, R21
	MOV.X		(R6, 16), R18
	PMULX.FA	R20, R22, R2
	MOVHD		R4, R4, R20		|	MOVHD	R4, R4, R21
	MOV.X		(R6, 32), R22
	PMULX.FA	R20, R18, R16
	PADDX.FA	R2, R16, R2
	MOVLD		R5, R5, R20		|	MOVLD	R5, R5, R21
	MOV.X		(R6, 48), R18
	PMULX.FA	R20, R22, R16
	MOVHD		R5, R5, R20		|	MOVHD	R5, R5, R21
	PADDX.FA	R2, R16, R2
	PMULX.FA	R20, R18, R16
	PADDX.FA	R2, R16, R18

	FLDCFH		R19, R4
//	FLDCH		0x1A00, R5
	FLDCH		0x0A00, R5
	FABS		R4, R4
//	FADD		R5, R4
	FADDA		R4, R5, R4

//	MOV			0x7FF, R5	| MOV	0x3FF, R6
	MOV			0x7FE, R5	| MOV	0x3FF, R6
	SHLD.Q		R5, 52, R5	| MOV	0x400, R3
	SHLD.Q		R6, 52, R6	| SUB	R5, R4, R5
	FMULA		R5, R4, R7
	SHLD.Q		R3, 52, R3	| SUB	R6, R7, R7
	ADD			R7, R5, R2

//	FMUL		R2, R4, R6
	FMULA		R2, R4, R6
//	FSUB		R3, R6, R7
	FSUBA		R3, R6, R7
//	FMUL		R7, R2
	FMULA		R2, R7, R2

//	FMULA		R2, R4, R6
//	FSUBA		R3, R6, R7
//	FMULA		R2, R7, R2

//	MOV			tkra_prj_xyzsc, R22
//	MOV			tkra_prj_xyzbi, R20
//	MOV.X		(R22,0), R22
//	MOV.X		(R20,0), R20

	MOV.X		tkra_prj_xyzsc, R22
	MOV.X		tkra_prj_xyzbi, R20
	
//	BREAK
	
	MOV			0x3F800000, R5	|	FSTCF		R2, R1
	MOVLD		R1, R1, R16		|	MOVLD		R5, R1, R17
	PMULX.FA	R16, R22, R16
	PMULX.FA	R18, R16, R18
	PADDX.FA	R18, R20, R2
	NOP
	NOP
	RTS
};

#else
tkra_vec4f TKRA_ProjectVertexB(tkra_vec4f vec, tkra_mat4 mat)
{
	tkra_vec4f		prj_xyzsc, prj_xyzbi;
	tkra_vec4f		v0xyzw, v0ww, v0p;
	float			f0, f1;

	v0xyzw=TKRA_ProjectVertex(vec, mat);
	f0=tkra_v4f_w(v0xyzw);
	f1=tkra_frcpabs(f0);

//	prj_xyzsc=tkra_prj_xyzsc;
//	prj_xyzbi=tkra_prj_xyzbi;
	v0ww=tkra_mkvec4f(f1, f1, f1, 1.0);
//	if(f0<0)
//		v0ww=tkra_mkvec4f(-f1, -f1, -f1, -1.0);
	
//	v0ww=tkra_v4fmul(v0ww, prj_xyzsc);
	v0ww=tkra_v4fmul(v0ww, tkra_prj_xyzsc);
//	v0p=tkra_v4fadd(tkra_v4fmul(v0xyzw, v0ww), prj_xyzbi);
	v0p=tkra_v4fadd(tkra_v4fmul(v0xyzw, v0ww), tkra_prj_xyzbi);

	return(v0p);
}
#endif

#ifdef __BJX2__
// #if 0
int TKRA_FinalProjectVertex_Float2Fixed32pN(double val, int qb);
u64 TKRA_FinalProjectVertex_Vec4F2H(u64 va, u64 vb);

__asm {
#if 1
TKRA_FinalProjectVertex_Float2Fixed32pN:
	SHLD.Q	R5, 52, R6
	ADD		R4, R6, R7
	FSTCI	R7, R2
	RTS
#else
TKRA_FinalProjectVertex_Float2Fixed32pN:
//	MOV		1075, R17
	MOV		0x000FFFFFFFFFFFFF, R18
	SHLD.Q	R4, -52, R6		|	AND		R4, R18, R7
	MOV		0x0010000000000000, R19
	OR		R7, R19, R7		|	MOV		1075, R17
	MOV		2047, R16		|	SUB		R17, R5, R17
	AND		R6, R16, R6		|	CMPQGE	0, R4
	SUB		R6, R17, R3		|	NEG?F	R7, R7
	MOV		0, R2			|	CMPGE	-52, R3
								SHAD.Q?T	R7, R3, R2
	RTS
#endif

TKRA_FinalProjectVertex_Vec4F2H:
	MOV		0x8000800080008000, R3
	SHLD.Q	R4, -31, R20	|	SHLD.Q	R4, -63, R21
	SHLD.Q	R5, -31, R22	|	SHLD.Q	R5, -63, R23
	MOV		0xFFFF, R16		|	TEST	1, R20
								OR?F	R16, R3
	SHLD.Q	R16, 16, R16	|	TEST	1, R21
	OR?F	R16, R3			|	PSTCH	R4, R16
	SHLD.Q	R16, 16, R16	|	TEST	1, R22
	OR?F	R16, R3			|	PSTCH	R5, R17
	SHLD.Q	R16, 16, R16	|	TEST	1, R23
	OR?F	R16, R3			|	MOVLLD	R17, R16, R2
	XOR		R2, R3, R2
	RTS
};

#else
int TKRA_FinalProjectVertex_Float2Fixed32pN(float val, int qb)
{
//	return(val*65536);
	return(val*(1<<qb));
}

u64 TKRA_FinalProjectVertex_Vec4F2H(u64 va, u64 vb)
{
	u64 vc;
	vc=	((va>>16)&0x000000000000FFFFULL) |
		((va>>32)&0x00000000FFFF0000ULL) |
		((vb<<16)&0x0000FFFF00000000ULL) |
		((vb>> 0)&0xFFFF000000000000ULL) ;

	if(vc&0x0000000000008000ULL)
			vc^=0x000000000000FFFFULL;
	else	vc^=0x0000000000008000ULL;
	if(vc&0x0000000080000000ULL)
			vc^=0x00000000FFFF0000ULL;
	else	vc^=0x0000000080000000ULL;
	if(vc&0x0000800000000000ULL)
			vc^=0x0000FFFF00000000ULL;
	else	vc^=0x0000800000000000ULL;
	if(vc&0x8000000000000000ULL)
			vc^=0xFFFF000000000000ULL;
	else	vc^=0x8000000000000000ULL;

	return(vc);
}

#endif

void TKRA_FinalProjectVertex(
	TKRA_Context *ctx,
	tkra_projvertex *pv0,
	tkra_trivertex *iv,
	tkra_vec4f	v0p)
{
	TKRA_TexImage *img;
	float xbi, ybi, s, t;
	int txs, tys;
	int i0, i1, i2;

	xbi=0.5;
	ybi=-1.0;
//	ybi=0.5;

	img=ctx->tex_cur;
	txs=1<<img->tex_xshl;
	tys=1<<img->tex_yshl;

//	i0=tkra_v4f_x(v0p)*65536;
//	i1=tkra_v4f_y(v0p)*65536;
//	i2=tkra_v4f_z(v0p)*65536;
	i0=TKRA_FinalProjectVertex_Float2Fixed32pN(tkra_v4f_x(v0p), 16);
	i1=TKRA_FinalProjectVertex_Float2Fixed32pN(tkra_v4f_y(v0p), 16);
	i2=TKRA_FinalProjectVertex_Float2Fixed32pN(tkra_v4f_z(v0p), 16);
	pv0->x=i0;	pv0->y=i1;	pv0->z=i2;
		
//	pv0->s=(tkra_v2f_x(iv->st)*txs+xbi)*65536;
//	pv0->t=(tkra_v2f_y(iv->st)*tys+ybi)*65536;

//	pv0->s=TKRA_FinalProjectVertex_Float2Fixed32pN(
//		tkra_v2f_x(iv->st)*txs+xbi, 16);
//	pv0->t=TKRA_FinalProjectVertex_Float2Fixed32pN(
//		tkra_v2f_y(iv->st)*tys+ybi, 16);

	s=tkra_v2f_x(iv->st);
	t=tkra_v2f_y(iv->st);
	pv0->s=TKRA_FinalProjectVertex_Float2Fixed32pN(
		s, 16+img->tex_xshl)+32767;
	pv0->t=TKRA_FinalProjectVertex_Float2Fixed32pN(
		t, 16+img->tex_yshl)-65535;

	pv0->rgb=iv->rgb;
	if(iv->fl&2)
		pv0->rgb=iv->mrgb;

	if(ctx->stateflag1&TKRA_STFL1_DOSHADER_MASK)
	{
		pv0->attrib[0]=TKRA_FinalProjectVertex_Vec4F2H(
			iv->attrib[0], iv->attrib[1]);
		pv0->attrib[1]=TKRA_FinalProjectVertex_Vec4F2H(
			iv->attrib[2], iv->attrib[3]);
		pv0->attrib[2]=TKRA_FinalProjectVertex_Vec4F2H(
			iv->attrib[4], iv->attrib[5]);
		pv0->attrib[3]=TKRA_FinalProjectVertex_Vec4F2H(
			iv->attrib[6], iv->attrib[7]);
	}
}

void TKRA_TransformProjectVertexLight(
	TKRA_Context *ctx,
	tkra_trivertex *rvo,
	tkra_mat4 prjmat)
{
	tkra_vec4f mrgb, argb;
	tkra_vec4f norm, pnorm, porg, pdir, lorg, ldir, hdir;
	float dist_x2, dist_x, attn, pdot, sdot;
	int cr, cg, cb, ca, crgb;
	int i, j, k;
	
	mrgb = tkra_mkvec4f(
		((rvo->rgb>>16)&255)*(1.0/255),
		((rvo->rgb>> 8)&255)*(1.0/255),
		((rvo->rgb>> 0)&255)*(1.0/255),
		((rvo->rgb>>24)&255)*(1.0/255)	);
	norm = tkra_mkvec4f(
		((sbyte)(rvo->nv>> 0))*(1.0/127),
		((sbyte)(rvo->nv>> 8))*(1.0/127),
		((sbyte)(rvo->nv>>16))*(1.0/127),
		1.0	);

	porg=TKRA_ProjectVertex(rvo->xyz, ctx->mat_xform);

//	pnorm=TKRA_ProjectVertex(norm, ctx->mat_xform);
	pnorm=TKRA_ProjectVector(norm, ctx->mat_xform);
	
	argb=ctx->light_model_ambient;
	
	for(i=0; i<8; i++)
	{
		if(!(ctx->light_mask&(1<<i)))
			continue;

		argb=tkra_v4fadd(argb, ctx->light_ambient[i]);

		lorg=TKRA_ProjectVertex(ctx->light_position[i], ctx->mat_xform);
		ldir=TKRA_ProjectVector(ctx->light_spot_dir[i], ctx->mat_xform);
		
		if(tkra_v4f_w(ctx->light_position[i])!=0.0)
		{
			dist_x2=tkra_v4f_dist(lorg, porg);
			dist_x=sqrt(dist_x2);
			attn=	ctx->light_attn_const[i]	+
					(ctx->light_attn_linear[i] * dist_x)	+
					(ctx->light_attn_quadratic[i] * dist_x2)	;
			if(attn<1)
				attn=1;
			attn=1.0/attn;
		}else
		{
			attn=1.0;
		}

		pdir=tkra_v4fadd(porg, lorg);

//		pdot=tkra_v3fdot(ldir, pdir);
		pdot=tkra_v3fdot(ldir, pnorm);
		if(pdot<0)
			pdot=0;

		argb=tkra_v4fadd(argb,
			tkra_v4f_scale(ctx->light_diffuse[i], pdot*attn));
			
		hdir=tkra_v4fadd(pdir, ldir);
		sdot=tkra_v3fdot(hdir, pnorm);
		if(pdot<0)
			pdot=0;
		
		sdot=pow(sdot, ctx->light_spot_exp[i]);

		argb=tkra_v4fadd(argb,
			tkra_v4f_scale(ctx->light_specular[i], sdot*attn));
	}
	
	argb=tkra_v4fmul(argb, mrgb);
	
	cr=tkra_v4f_x(argb)*255;
	cg=tkra_v4f_y(argb)*255;
	cb=tkra_v4f_z(argb)*255;
	ca=tkra_v4f_w(argb)*255;
	cr=__int_clamp(cr, 0, 255);
	cg=__int_clamp(cg, 0, 255);
	cb=__int_clamp(cb, 0, 255);
	ca=__int_clamp(ca, 0, 255);
	
	rvo->mrgb=(ca<<24)|(cr<<16)|(cg<<8)|cb;
}

void TKRA_TransformProjectVertexShader(
	TKRA_Context *ctx,
	tkra_trivertex *rvo,
	tkra_trivertex *rvi,
	tkra_mat4 prjmat)
{
	if(ctx->tkgl_usepgm_vtx<=0)
	{
		if(rvi!=rvo)
			{ *rvo=*rvi; }
		rvo->pv=TKRA_ProjectVertexB(rvi->xyz, prjmat);
		rvo->fl=rvi->fl|1;
		
		if(ctx->stateflag1&TKRA_STFL1_LIGHTING)
		{
			TKRA_TransformProjectVertexLight(ctx, rvo, prjmat);
			rvo->fl=rvo->fl|2;
		}
		
		return;
	}
}

void TKRA_TransformCalcMidpointVertex(
	TKRA_Context *ctx,
	tkra_trivertex *rv3,
	tkra_trivertex *rv0,
	tkra_trivertex *rv1,
	tkra_mat4 prjmat)
{
//	tkra_trivertex v3;
	tkra_vec4f	a0, a1, a3;
	int i;

	rv3->xyz=tkra_v4f_midpoint (rv0->xyz, rv1->xyz);
	rv3->st =tkra_v2f_midpoint (rv0->st , rv1->st );
	rv3->rgb=tkra_rgba_midpoint(rv0->rgb, rv1->rgb);

//	if((ctx->tkgl_usepgm_vtx<=0) &&
//		!(ctx->stateflag1&TKRA_STFL1_LIGHTING))
	if(!(ctx->stateflag1&TKRA_STFL1_DOSHADER_MASK))
	{
		rv3->pv=TKRA_ProjectVertexB(rv3->xyz, prjmat);
		rv3->fl=1;
	}else
	{
//		rv3->norm=tkra_v4f_midpoint (rv0->norm, rv1->norm);
		rv3->mrgb=tkra_rgba_midpoint(rv0->mrgb, rv1->mrgb);
		rv3->nv=tkra_norm_midpoint(rv0->nv, rv1->nv);
		rv3->fl=0;
		
		if(ctx->tkgl_usepgm_vtx>0)
		{
			for(i=0; i<8; i+=2)
			{
				a0=tkra_mkvec4fv(rv0->attrib+i);
				a1=tkra_mkvec4fv(rv1->attrib+i);
				a3=tkra_v4f_midpoint(a0, a1);
				*(tkra_vec4f *)(rv3->attrib+i)=a0;
			}
		}
		
		TKRA_TransformProjectVertexShader(ctx, rv3, rv3, prjmat);
	}
	
//	return(v3);
}

int TKRA_TransformProjectTriangle(
	TKRA_Context *ctx,
	tkra_trivertex iv0,	tkra_trivertex iv1,
	tkra_trivertex iv2,
//	tkra_mat4 xform,
//	tkra_mat4 xproj)
	tkra_mat4 prjmat)
{
	tkra_trivertex *v0stk;
	tkra_trivertex *v1stk;
	tkra_trivertex *v2stk;
	tkra_trivertex *v3stk;
	int vstkpos;

	tkra_projvertex pv0;
	tkra_projvertex pv1;
	tkra_projvertex pv2;
	tkra_trivertex	v0, v1, v2;
	tkra_trivertex	v3, v4, v5;
	tkra_vec4f		v0xyzw, v0ww, v0p;
	tkra_vec4f		v1xyzw, v1ww, v1p;
	tkra_vec4f		v2xyzw, v2ww, v2p;
	tkra_vec4f		v0d, v1d, v2d;
	tkra_vec2f		v0dxy, v1dxy, v2dxy;
	tkra_vec4f		prj_xyzsc, prj_xyzbi;

	TKRA_TexImage *img;

	float			scr_xsc, scr_ysc;
	float			scr_xc, scr_yc;
	float			scr_clip_l, scr_clip_r;
	float			scr_clip_t, scr_clip_b;
	float			txs, tys, txsn1, tysn1, xbi, ybi;
	float			f0, f1, f2, f3, f4, f5, fdx, fdy;
	float			g0, g1, g2, g3, g4, g5;
	float			w0, w1, w2, skew;
	int				i0, i1, i2, tfl;
	int				ecfl, lim;
	byte			wasfrag, nosubdiv;
	
	TKRA_SetupDrawBlend(ctx);

	v0stk=ctx->v0stk;
	v1stk=ctx->v1stk;
	v2stk=ctx->v2stk;
//	v3stk=ctx->v3stk;

	v0stk[0]=iv0;
	v1stk[0]=iv1;
	v2stk[0]=iv2;
	vstkpos=1;
	ctx->stat_base_tris++;

	v0stk[0].fl=0;
	v1stk[0].fl=0;
	v2stk[0].fl=0;
	
	tfl=0;

	if((iv0.rgb&iv1.rgb&iv2.rgb&0xF0000000)!=0xF0000000)
		tfl|=1;
	
//	ctx->triflag=tfl;
	TKRA_SetupDrawEdgeForTriFlag(ctx, tfl);
	tfl=ctx->triflag;
	
	if(tfl&TKRA_TRFL_NOCMOD)
	{
		v0stk[0].rgb=0xFFFFFFFFU;
		v1stk[0].rgb=0xFFFFFFFFU;
		v2stk[0].rgb=0xFFFFFFFFU;
	}
	
	prj_xyzsc=ctx->prj_xyzsc;
	prj_xyzbi=ctx->prj_xyzbi;
	
	tkra_prj_xyzsc=prj_xyzsc;
	tkra_prj_xyzbi=prj_xyzbi;
//	*(tkra_vec4f *)(&tkra_prj_xyzsc)=prj_xyzsc;
//	*(tkra_vec4f *)(&tkra_prj_xyzbi)=prj_xyzbi;
	
//	__debugbreak();

	scr_clip_l=ctx->scr_clip_l;
	scr_clip_r=ctx->scr_clip_r;
	scr_clip_t=ctx->scr_clip_t;
	scr_clip_b=ctx->scr_clip_b;


//	txs=(1<<(ctx->tex_xshl));
//	tys=(1<<(ctx->tex_yshl));

	img=ctx->tex_cur;
	txs=1<<img->tex_xshl;
	tys=1<<img->tex_yshl;
	txsn1=txs-1;
	tysn1=tys-1;
	xbi=0.5;
//	xbi=-0.5;
//	xbi=-1.0;
//	xbi=0.0;
//	ybi=0.5;
//	ybi=-0.5;
	ybi=-1.0;
//	ybi=-2.0;
//	ybi= 0.0;
	wasfrag=0;

	lim=4096;

	w0=0;
	w1=0;
	w2=0;

	nosubdiv=0;
	if((ctx->tex_xshl==0) && (ctx->tex_yshl==0))
		nosubdiv=1;
			
	if(ctx->stateflag1&TKRA_STFL1_NOSUBDIV)
		nosubdiv=1;

	while((vstkpos>0) && (lim>0))
	{
		lim--;
		vstkpos--;
		v0=v0stk[vstkpos];
		v1=v1stk[vstkpos];
		v2=v2stk[vstkpos];

// #if 0
#if 1
//		if(ctx->tkgl_usepgm_vtx>0)
//		if((ctx->tkgl_usepgm_vtx>0) ||
//			(ctx->stateflag1&TKRA_STFL1_LIGHTING))
		if(ctx->stateflag1&TKRA_STFL1_DOSHADER_MASK)
		{
			if(!(v0.fl&1))
				{ TKRA_TransformProjectVertexShader(ctx, &v0, &v0, prjmat); }
			if(!(v1.fl&1))
				{ TKRA_TransformProjectVertexShader(ctx, &v1, &v1, prjmat); }
			if(!(v2.fl&1))
				{ TKRA_TransformProjectVertexShader(ctx, &v2, &v2, prjmat); }
			v0p=v0.pv;
			v1p=v1.pv;
			v2p=v2.pv;
		}else
		{
			if(v0.fl&1)
				v0p=v0.pv;
			else
				v0p=TKRA_ProjectVertexB(v0.xyz, prjmat);

			if(v1.fl&1)
				v1p=v1.pv;
			else
				v1p=TKRA_ProjectVertexB(v1.xyz, prjmat);

			if(v2.fl&1)
				v2p=v2.pv;
			else
				v2p=TKRA_ProjectVertexB(v2.xyz, prjmat);
		}
#endif

		v0ww=tkra_v4f_bboxmins3(v0p, v1p, v2p);
		v1ww=tkra_v4f_bboxmaxs3(v0p, v1p, v2p);
		
		ecfl=0;

//		if(tkra_v4f_z(v0ww)<(-32767))
//			continue;
//		if(tkra_v4f_z(v1ww)>( 32767))
//			continue;

		if(tkra_v4f_w(v1ww)<=0.0)
		{
			ctx->stat_reject_tris++;
			ctx->stat_negw_tris++;
			continue;
		}

		if(tkra_v4f_x(v0ww)>scr_clip_r)
		{
			ctx->stat_reject_tris++;
			ctx->stat_frustum_tris++;
			continue;
		}

		if(tkra_v4f_x(v1ww)<scr_clip_l)
		{
			ctx->stat_reject_tris++;
			ctx->stat_frustum_tris++;
			continue;
		}

		if(tkra_v4f_y(v0ww)>scr_clip_t)
		{
			ctx->stat_reject_tris++;
			ctx->stat_frustum_tris++;
			continue;
		}

		if(tkra_v4f_y(v1ww)<scr_clip_b)
		{
			ctx->stat_reject_tris++;
			ctx->stat_frustum_tris++;
			continue;
		}

#if 1
//		if(tkra_v4f_z(v0ww)<0.0)
//			{ ecfl|=1; }

		if((tkra_v4f_w(v0ww)<0.0) && (tkra_v4f_w(v1ww)>0.0))
			{ ecfl|=1; }

//		if((tkra_v4f_z(v0ww)<0.0) && (tkra_v4f_z(v1ww)>0.0))
//			{ ecfl|=1; }
		if(	(tkra_v4f_x(v1ww)>scr_clip_r) ||
			(tkra_v4f_x(v0ww)<scr_clip_l) )
				{ ecfl|=2; }
		if(	(tkra_v4f_y(v1ww)>scr_clip_t) ||
			(tkra_v4f_y(v0ww)<scr_clip_b) )
				{ ecfl|=4; }
#endif

#if 1
		/* Check if triangle is below size limit. */
		v0d=tkra_v4fsub(v1p, v0p);
		v1d=tkra_v4fsub(v2p, v1p);
		v2d=tkra_v4fsub(v0p, v2p);

		v0dxy=tkra_v4f_xy(v0d);
		v1dxy=tkra_v4f_xy(v1d);
		v2dxy=tkra_v4f_xy(v2d);
		f0=tkra_v2fdot(v0dxy, v0dxy);
		f1=tkra_v2fdot(v1dxy, v1dxy);
		f2=tkra_v2fdot(v2dxy, v2dxy);		
#endif
		
//		f3=tkra_v2fcross(v0dxy, v1dxy);
//		if(f3<=0)
//		if(f3>0)
//			continue;

#if 1
		if((ctx->stateflag1&TKRA_STFL1_CULLFACE) && !(ecfl&1))
		{
			f3=tkra_v2fcross(v0dxy, v1dxy);
			if(ctx->stateflag1&TKRA_STFL1_CULL_CW)
				f3=-f3;
//			if(f3<=0)
//			if((ctx->stateflag1&TKRA_STFL1_CULL_BK) && (f3>0))
			if((ctx->stateflag1&TKRA_STFL1_CULL_BK) && (f3<0))
			{
				ctx->stat_reject_tris++;
				ctx->stat_backface_tris++;
				continue;
			}
//			if((ctx->stateflag1&TKRA_STFL1_CULL_FT) && (f3<=0))
			if((ctx->stateflag1&TKRA_STFL1_CULL_FT) && (f3>=0))
			{
				ctx->stat_reject_tris++;
				ctx->stat_backface_tris++;
				continue;
			}
		}
#endif

		f3=f0+f1+f2;

//		if(f3 < (5*5*3))
//		if(f3 < (4*4*3))
		if(f3 < (3*3*3))
//		if(f3 < (2*2*3))
//		if(f3 < (3*3))
//		if(f3 < (4*3))
		{
#if 1
			if(	(f0<0.75) | (f1<0.75) | (f2<0.75)	)
			{
				ctx->stat_reject_tris++;
	//			ctx->stat_micro_tris++;
				if(wasfrag&1)
					ctx->stat_microfrag_tris++;
				else
					ctx->stat_microbase_tris++;

				continue;
			}
#endif

//			ctx->stat_reject_tris++;
//			ctx->stat_micro_tris++;
//			if(wasfrag&1)
//				ctx->stat_microfrag_tris++;
//			else
//				ctx->stat_microbase_tris++;

//			if(f3 < (0.5*3))
//			if(f3 < (0.75*3))
			if(f3 < (1*3))
			{
				ctx->stat_microbase_tris++;
				continue;				
			}

//			if(1)
//			if(f3 < (2*3))
			if(f3 < (3*3))
//			if(f3 < (2*2*3))
			{
				TKRA_TransformCalcMidpointVertex(ctx, &v3, &v0, &v1, prjmat);
				TKRA_TransformCalcMidpointVertex(ctx, &v4, &v1, &v2, prjmat);
				TKRA_TransformCalcMidpointVertex(ctx, &v5, &v3, &v4, prjmat);

				if(!(v5.fl&1))
					TKRA_TransformProjectVertexShader(ctx, &v5, &v5, prjmat);

				TKRA_FinalProjectVertex(ctx, &pv0, &v5, v5.pv);

				ctx->stat_drawpts1_tris++;
				TKRA_EmitProjectedPoint(ctx, pv0);
				continue;
			}

#if 1
			TKRA_FinalProjectVertex(ctx, &pv0, &v0, v0p);
			TKRA_FinalProjectVertex(ctx, &pv1, &v1, v1p);
			TKRA_FinalProjectVertex(ctx, &pv2, &v2, v2p);

			ctx->stat_drawpts3_tris++;
			TKRA_EmitProjectedTrianglePts(ctx, pv0, pv1, pv2);
			continue;
#endif
		}

#if 1
		if((f0<1) | (f1<1) | (f2<1))
		{
			ctx->stat_reject_tris++;
//			ctx->stat_micro_tris++;
			if(wasfrag&1)
				ctx->stat_microfrag_tris++;
			else
				ctx->stat_microbase_tris++;
			continue;
		}
#endif

//		f4=(128*128*3);
//		f4=(96*96*3);


#if 1
//		f4=(72*72*3);
//		f4=(64*64*3);
		f4=TKRA_PARAM_TRISUBDIV_DFL;
//		f4=(48*48*3);
//		if(ecfl)
//			f4=(32*32*3);

//		if(ecfl&4)
		if(ecfl&6)
		{
			f4=(32*32*3);
			f4=TKRA_PARAM_TRISUBDIV_EDGE;
//			f4=(48*48*3);
		}

		if(ecfl&1)
		{
//			f4=(32*32*3);
//			f4=(24*24*3);
			f4=TKRA_PARAM_TRISUBDIV_NEAR;
//			f4=(16*16*3);
		}
#endif

		if(ecfl&1)
			nosubdiv=0;

//		if((ctx->tex_xshl==0) && (ctx->tex_yshl==0))
//			f4=999999;

//		f4 *= skew;
	
//		if(f3>(64*64*3))
//		if(f3>(32*32*3))
//		if(f3>(128*128*3))
//		if(f3>f4)
		if(!nosubdiv && (f3>f4))
//		if(0)
		{
//			if((vstkpos+5)>=32)
			if((vstkpos+5)>=48)
//			if((vstkpos+5)>=64)
//			if((vstkpos+5)>=16)
//			if((vstkpos+5)>=24)
			{
				printf("blown: Wn: %f %f %f\n", w0, w1, w2);
				printf("blown: Fn: %f %f %f\n", f0, f1, f2);
			
				TKRA_DumpMatrix(prjmat, "PrjMat");
				TKRA_DumpVec4(iv0.xyz, "Iv0");
				TKRA_DumpVec4(iv1.xyz, "Iv1");
				TKRA_DumpVec4(iv2.xyz, "Iv2");
			
				ctx->stat_blown_tris++;
				return(-1);
			}
			
			v0xyzw=v0.xyz;
			v1xyzw=v1.xyz;
			v2xyzw=v2.xyz;
			v0d=tkra_v4fsub(v1xyzw, v0xyzw);
			v1d=tkra_v4fsub(v2xyzw, v1xyzw);
			v2d=tkra_v4fsub(v0xyzw, v2xyzw);
			g0=tkra_v3fdot(v0d, v0d);
			g1=tkra_v3fdot(v1d, v1d);
			g2=tkra_v3fdot(v2d, v2d);
			g3=g0+g1+g2;
			
//			if(ctx->tkgl_usepgm_vtx<=0)
//			if((ctx->tkgl_usepgm_vtx<=0) &&
//				!(ctx->stateflag1&TKRA_STFL1_LIGHTING))
			if(!(ctx->stateflag1&TKRA_STFL1_DOSHADER_MASK))
			{
				v0.pv=v0p;
				v1.pv=v1p;
				v2.pv=v2p;
				v0.fl=1;
				v1.fl=1;
				v2.fl=1;
			}

			/* Subdivide Triangle */
			TKRA_TransformCalcMidpointVertex(ctx, &v3, &v0, &v1, prjmat);
			TKRA_TransformCalcMidpointVertex(ctx, &v4, &v1, &v2, prjmat);
			TKRA_TransformCalcMidpointVertex(ctx, &v5, &v2, &v0, prjmat);

//			f5=f3*0.1;
//			f5=f3*0.05;
//			f5=f3*0.066;
//			g5=g3*0.066;
			g5=g3*0.15;

//			if(f5>f0)
			if(g5>g0)
			{
				v0stk[vstkpos]=v0;
				v1stk[vstkpos]=v4;
				v2stk[vstkpos]=v5;
				vstkpos++;
				v0stk[vstkpos]=v0;
				v1stk[vstkpos]=v1;
				v2stk[vstkpos]=v4;
				vstkpos++;
				v0stk[vstkpos]=v5;
				v1stk[vstkpos]=v4;
				v2stk[vstkpos]=v2;
				vstkpos++;

				ctx->stat_frag_tris+=3;
				wasfrag|=1;

				continue;
			}

#if 1
//			if(f5>f1)
			if(g5>g1)
			{
				v0stk[vstkpos]=v0;
				v1stk[vstkpos]=v3;
				v2stk[vstkpos]=v5;
				vstkpos++;
				v0stk[vstkpos]=v3;
				v1stk[vstkpos]=v1;
				v2stk[vstkpos]=v2;
				vstkpos++;
				v0stk[vstkpos]=v3;
				v1stk[vstkpos]=v2;
				v2stk[vstkpos]=v5;
				vstkpos++;

				ctx->stat_frag_tris+=3;
				wasfrag|=1;

				continue;
			}
#endif

#if 1
//			if(f5>f2)
			if(g5>g2)
			{
				v0stk[vstkpos]=v0;
				v1stk[vstkpos]=v3;
				v2stk[vstkpos]=v2;
				vstkpos++;
				v0stk[vstkpos]=v2;
				v1stk[vstkpos]=v3;
				v2stk[vstkpos]=v4;
				vstkpos++;
				v0stk[vstkpos]=v3;
				v1stk[vstkpos]=v1;
				v2stk[vstkpos]=v4;
				vstkpos++;

				ctx->stat_frag_tris+=3;
				wasfrag|=1;

				continue;
			}
#endif

			v0stk[vstkpos]=v0;
			v1stk[vstkpos]=v3;
			v2stk[vstkpos]=v5;
			vstkpos++;
			v0stk[vstkpos]=v1;
			v1stk[vstkpos]=v4;
			v2stk[vstkpos]=v3;
			vstkpos++;
			v0stk[vstkpos]=v2;
			v1stk[vstkpos]=v5;
			v2stk[vstkpos]=v4;
			vstkpos++;
			v0stk[vstkpos]=v3;
			v1stk[vstkpos]=v4;
			v2stk[vstkpos]=v5;
			vstkpos++;

			ctx->stat_frag_tris+=4;
			wasfrag|=1;

			continue;
		}

#if 0
		if(ctx->stateflag1&TKRA_STFL1_CULLFACE)
		{
			f3=tkra_v2fcross(v0dxy, v1dxy);
			if(ctx->stateflag1&TKRA_STFL1_CULL_CW)
				f3=-f3;
//			if(f3<=0)
//			if((ctx->stateflag1&TKRA_STFL1_CULL_BK) && (f3>0))
			if((ctx->stateflag1&TKRA_STFL1_CULL_BK) && (f3<0))
			{
				ctx->stat_reject_tris++;
				ctx->stat_backface_tris++;
				continue;
			}
//			if((ctx->stateflag1&TKRA_STFL1_CULL_FT) && (f3<=0))
			if((ctx->stateflag1&TKRA_STFL1_CULL_FT) && (f3>=0))
			{
				ctx->stat_reject_tris++;
				ctx->stat_backface_tris++;
				continue;
			}
		}
#endif

		if(tkra_v4f_w(v0ww)<0)
		{
			ctx->stat_reject_tris++;
			ctx->stat_negw_tris++;
			continue;
		}

		ctx->stat_draw_tris++;

		TKRA_FinalProjectVertex(ctx, &pv0, &v0, v0p);
		TKRA_FinalProjectVertex(ctx, &pv1, &v1, v1p);
		TKRA_FinalProjectVertex(ctx, &pv2, &v2, v2p);

		TKRA_EmitProjectedTriangle(ctx, pv0, pv1, pv2);
	}
	
	return(0);
}

int TKRA_TransformProjectQuad(
	TKRA_Context *ctx,
	tkra_trivertex iv0, tkra_trivertex iv1,
	tkra_trivertex iv2, tkra_trivertex iv3,
	tkra_mat4 prjmat)
{
	tkra_trivertex	*v0stk;
	tkra_trivertex	*v1stk;
	tkra_trivertex	*v2stk;
	tkra_trivertex	*v3stk;

	int vstkpos;

	tkra_projvertex pv0;
	tkra_projvertex pv1;
	tkra_projvertex pv2;
	tkra_projvertex pv3;
	tkra_trivertex	v0, v1, v2, v3;
	tkra_trivertex	v4, v5, v6, v7, v8;
	tkra_vec4f		v0xyzw, v0ww, v0p;
	tkra_vec4f		v1xyzw, v1ww, v1p;
	tkra_vec4f		v2xyzw, v2ww, v2p;
	tkra_vec4f		v3xyzw, v3ww, v3p;
	tkra_vec4f		v0d, v1d, v2d, v3d;
	tkra_vec2f		v0dxy, v1dxy, v2dxy, v3dxy;
	tkra_vec4f		prj_xyzsc, prj_xyzbi;

	TKRA_TexImage *img;

	float			scr_xsc, scr_ysc;
	float			scr_xc, scr_yc;
	float			scr_clip_l, scr_clip_r;
	float			scr_clip_t, scr_clip_b;
	float			txs, tys, txsn1, tysn1, xbi, ybi;
	float			f0, f1, f2, f3, f4, f5, fdx, fdy;
	float			g0, g1, g2, g3;
	float			w0, w1, w2, skew;
	int				i0, i1, i2, tfl;
	int				ecfl, lim;
	byte			wasfrag, nosubdiv, isinit, inosubdiv;
	
//	return(0);
	
	TKRA_SetupDrawBlend(ctx);

	v0stk=ctx->v0stk;
	v1stk=ctx->v1stk;
	v2stk=ctx->v2stk;
	v3stk=ctx->v3stk;

//	v0stk[0]=iv0;
//	v1stk[0]=iv1;
//	v2stk[0]=iv2;
//	v3stk[0]=iv3;
	vstkpos=1;
	ctx->stat_base_tris++;

//	v0stk[0].fl=0;	v1stk[0].fl=0;
//	v2stk[0].fl=0;	v3stk[0].fl=0;

	tfl=0;

	if((iv0.rgb&iv1.rgb&iv2.rgb&iv3.rgb&0xF0000000)!=0xF0000000)
		tfl|=1;
	
//	ctx->triflag=tfl;
	TKRA_SetupDrawEdgeForTriFlag(ctx, tfl);
	tfl=ctx->triflag;
	
//	if(tfl&TKRA_TRFL_NOCMOD)
//	{
//		v0stk[0].rgb=0xFFFFFFFFU;
//		v1stk[0].rgb=0xFFFFFFFFU;
//		v2stk[0].rgb=0xFFFFFFFFU;
//		v3stk[0].rgb=0xFFFFFFFFU;
//	}

	v0=iv0;		v1=iv1;
	v2=iv2;		v3=iv3;
	v0.fl=0;	v1.fl=0;
	v2.fl=0;	v3.fl=0;

	if(tfl&TKRA_TRFL_NOCMOD)
	{
		v0.rgb=0xFFFFFFFFU;
		v1.rgb=0xFFFFFFFFU;
		v2.rgb=0xFFFFFFFFU;
		v3.rgb=0xFFFFFFFFU;
	}

	prj_xyzsc=ctx->prj_xyzsc;
	prj_xyzbi=ctx->prj_xyzbi;
	
	tkra_prj_xyzsc=prj_xyzsc;
	tkra_prj_xyzbi=prj_xyzbi;

	scr_clip_l=ctx->scr_clip_l;
	scr_clip_r=ctx->scr_clip_r;
	scr_clip_t=ctx->scr_clip_t;
	scr_clip_b=ctx->scr_clip_b;

	img=ctx->tex_cur;
	txs=1<<img->tex_xshl;
	tys=1<<img->tex_yshl;
	txsn1=txs-1;
	tysn1=tys-1;
//	xbi=0.0;
	xbi=0.5;
	ybi=-1.0;
	wasfrag=0;
	isinit=1;

	lim=4096;

	w0=0;
	w1=0;
	w2=0;

	inosubdiv=0;
	if((ctx->tex_xshl==0) && (ctx->tex_yshl==0))
		inosubdiv=1;
			
	if(ctx->stateflag1&TKRA_STFL1_NOSUBDIV)
		inosubdiv=1;

	while((vstkpos>0) && (lim>0))
	{
		lim--;
		if(isinit)
		{
			isinit=0;
			vstkpos=0;
//			v0=iv0;
//			v1=iv1;
//			v2=iv2;
//			v3=iv3;
		}else
		{
			vstkpos--;
			v0=v0stk[vstkpos];
			v1=v1stk[vstkpos];
			v2=v2stk[vstkpos];
			v3=v3stk[vstkpos];
		}
		
		nosubdiv=inosubdiv;

//		if(ctx->tkgl_usepgm_vtx>0)
//		if((ctx->tkgl_usepgm_vtx>0) ||
//			(ctx->stateflag1&TKRA_STFL1_LIGHTING))
		if(ctx->stateflag1&TKRA_STFL1_DOSHADER_MASK)
		{
			if(!(v0.fl&1))
				TKRA_TransformProjectVertexShader(ctx, &v0, &v0, prjmat);
			if(!(v1.fl&1))
				TKRA_TransformProjectVertexShader(ctx, &v1, &v1, prjmat);
			if(!(v2.fl&1))
				TKRA_TransformProjectVertexShader(ctx, &v2, &v2, prjmat);
			if(!(v3.fl&1))
				TKRA_TransformProjectVertexShader(ctx, &v3, &v3, prjmat);
			v0p=v0.pv;
			v1p=v1.pv;
			v2p=v2.pv;
			v3p=v3.pv;
		}else
		{
			if(v0.fl&1)
				v0p=v0.pv;
			else
				v0p=TKRA_ProjectVertexB(v0.xyz, prjmat);

			if(v1.fl&1)
				v1p=v1.pv;
			else
				v1p=TKRA_ProjectVertexB(v1.xyz, prjmat);

			if(v2.fl&1)
				v2p=v2.pv;
			else
				v2p=TKRA_ProjectVertexB(v2.xyz, prjmat);

			if(v3.fl&1)
				v3p=v3.pv;
			else
				v3p=TKRA_ProjectVertexB(v3.xyz, prjmat);
		}

		v0ww=tkra_v4f_bboxmins4(v0p, v1p, v2p, v3p);
		v1ww=tkra_v4f_bboxmaxs4(v0p, v1p, v2p, v3p);
		
		ecfl=0;

		if(tkra_v4f_w(v1ww)<=0.0)
		{
			ctx->stat_reject_tris++;
			ctx->stat_negw_tris++;
			continue;
		}

		if(tkra_v4f_x(v0ww)>scr_clip_r)
		{
			ctx->stat_reject_tris++;
			ctx->stat_frustum_tris++;
			continue;
		}

		if(tkra_v4f_x(v1ww)<scr_clip_l)
		{
			ctx->stat_reject_tris++;
			ctx->stat_frustum_tris++;
			continue;
		}

		if(tkra_v4f_y(v0ww)>scr_clip_t)
		{
			ctx->stat_reject_tris++;
			ctx->stat_frustum_tris++;
			continue;
		}

		if(tkra_v4f_y(v1ww)<scr_clip_b)
		{
			ctx->stat_reject_tris++;
			ctx->stat_frustum_tris++;
			continue;
		}

#if 1
		if((tkra_v4f_w(v0ww)<0.0) && (tkra_v4f_w(v1ww)>0.0))
			{ ecfl|=1; }

//		if((tkra_v4f_z(v0ww)<0.0) && (tkra_v4f_z(v1ww)>0.0))
//			{ ecfl|=1; }

		if(	(tkra_v4f_x(v1ww)>scr_clip_r) ||
			(tkra_v4f_x(v0ww)<scr_clip_l) )
				{ ecfl|=2; }
		if(	(tkra_v4f_y(v1ww)>scr_clip_t) ||
			(tkra_v4f_y(v0ww)<scr_clip_b) )
				{ ecfl|=4; }
#endif		

#if 0
//		if(!ecfl)
		if(!(ecfl&1))
		{
			f0=tkra_v4f_z(v0ww);
			f1=tkra_v4f_z(v1ww);
			f2=f1-f0;
			
			if(f2<0.25)
				nosubdiv=1;
		}
#endif

#if 1
		/* Check if quad is below size limit. */
		v0d=tkra_v4fsub(v1p, v0p);
		v1d=tkra_v4fsub(v2p, v1p);
		v2d=tkra_v4fsub(v3p, v2p);
		v3d=tkra_v4fsub(v0p, v3p);

		v0dxy=tkra_v4f_xy(v0d);
		v1dxy=tkra_v4f_xy(v1d);
		v2dxy=tkra_v4f_xy(v2d);
		v3dxy=tkra_v4f_xy(v3d);
		f0=tkra_v2fdot(v0dxy, v0dxy);
		f1=tkra_v2fdot(v1dxy, v1dxy);
		f2=tkra_v2fdot(v2dxy, v2dxy);
		f3=tkra_v2fdot(v3dxy, v3dxy);
#endif

#if 1
//		if(ctx->stateflag1&TKRA_STFL1_CULLFACE)
		if((ctx->stateflag1&TKRA_STFL1_CULLFACE) && !(ecfl&1))
		{
			f4=tkra_v2fcross(v0dxy, v1dxy);
			if(ctx->stateflag1&TKRA_STFL1_CULL_CW)
				f4=-f4;
			if((ctx->stateflag1&TKRA_STFL1_CULL_BK) && (f4<0))
			{
				ctx->stat_backface_tris++;
				ctx->stat_reject_tris++;
				continue;
			}
			if((ctx->stateflag1&TKRA_STFL1_CULL_FT) && (f4>=0))
			{
				ctx->stat_backface_tris++;
				ctx->stat_reject_tris++;
				continue;
			}
		}
#endif

		f5=f0+f1+f2+f3;

		if(f5 < (4*4))
		{
			if(f5 < (1*4))
			{
				ctx->stat_microbase_tris++;
				continue;				
			}

			if(1)
			{
				TKRA_TransformCalcMidpointVertex(ctx, &v8, &v0, &v2, prjmat);
				if(!(v8.fl&1))
					TKRA_TransformProjectVertexShader(ctx, &v8, &v8, prjmat);
				TKRA_FinalProjectVertex(ctx, &pv0, &v8, v8.pv);

				ctx->stat_drawpts1_tris++;
				TKRA_EmitProjectedPoint(ctx, pv0);
				continue;
			}
		}

#if 1
		f4=TKRA_PARAM_QUADSUBDIV_DFL;

//		if(ecfl&4)
		if(ecfl&6)
		{
			f4=TKRA_PARAM_QUADSUBDIV_EDGE;
		}

		if(ecfl&1)
		{
			f4=TKRA_PARAM_QUADSUBDIV_NEAR;
		}
#endif

		if(ecfl&1)
			nosubdiv=0;

		if(!nosubdiv && (f5>f4))
		{
			if((vstkpos+5)>=48)
			{
				printf("blown: Wn: %f %f %f\n", w0, w1, w2);
				printf("blown: Fn: %f %f %f\n", f0, f1, f2);
			
				TKRA_DumpMatrix(prjmat, "PrjMat");
				TKRA_DumpVec4(iv0.xyz, "Iv0");
				TKRA_DumpVec4(iv1.xyz, "Iv1");
				TKRA_DumpVec4(iv2.xyz, "Iv2");
				TKRA_DumpVec4(iv3.xyz, "Iv3");
			
				ctx->stat_blown_tris++;
				return(-1);
			}

#if 1
			if(!ecfl)
			{
				TKRA_FinalProjectVertex(ctx, &pv0, &v0, v0p);
				TKRA_FinalProjectVertex(ctx, &pv1, &v1, v1p);
				TKRA_FinalProjectVertex(ctx, &pv2, &v2, v2p);
				TKRA_FinalProjectVertex(ctx, &pv3, &v3, v3p);
				if(TKRA_CheckZCullQuad(ctx, pv0, pv1, pv2, pv3))
				{
					ctx->stat_reject_tris++;
					ctx->stat_zocc_tris++;
					continue;
				}
			}
#endif

			v0xyzw=v0.xyz;	v1xyzw=v1.xyz;
			v2xyzw=v2.xyz;	v3xyzw=v3.xyz;

			v0d=tkra_v4fsub(v1xyzw, v0xyzw);
			v1d=tkra_v4fsub(v2xyzw, v1xyzw);
			v2d=tkra_v4fsub(v3xyzw, v2xyzw);
			v3d=tkra_v4fsub(v0xyzw, v3xyzw);

			g0=tkra_v3fdot(v0d, v0d);
			g1=tkra_v3fdot(v1d, v1d);
			g2=tkra_v3fdot(v2d, v2d);
			g3=tkra_v3fdot(v3d, v3d);

			v0.pv=v0p;		v1.pv=v1p;
			v2.pv=v2p;		v3.pv=v3p;
			v0.fl=1;		v1.fl=1;
			v2.fl=1;		v3.fl=1;

			/* Subdivide Quad */
			TKRA_TransformCalcMidpointVertex(ctx, &v4, &v0, &v1, prjmat);
			TKRA_TransformCalcMidpointVertex(ctx, &v5, &v1, &v2, prjmat);
			TKRA_TransformCalcMidpointVertex(ctx, &v6, &v2, &v3, prjmat);
			TKRA_TransformCalcMidpointVertex(ctx, &v7, &v3, &v0, prjmat);

#if 1
			if(((g0+g2)>(2*(g1+g3))) && !(f5>(f4*64)))
			{
				v0stk[vstkpos]=v0;
				v1stk[vstkpos]=v4;
				v2stk[vstkpos]=v6;
				v3stk[vstkpos]=v3;
				vstkpos++;

				v0stk[vstkpos]=v4;
				v1stk[vstkpos]=v1;
				v2stk[vstkpos]=v2;
				v3stk[vstkpos]=v6;
				vstkpos++;

				ctx->stat_frag_tris+=2;
				wasfrag|=1;

				continue;
			}

			if(((g1+g3)>(2*(g0+g2))) && !(f5>(f4*64)))
			{
				v0stk[vstkpos]=v0;
				v1stk[vstkpos]=v1;
				v2stk[vstkpos]=v5;
				v3stk[vstkpos]=v7;
				vstkpos++;

				v0stk[vstkpos]=v7;
				v1stk[vstkpos]=v5;
				v2stk[vstkpos]=v2;
				v3stk[vstkpos]=v3;

				vstkpos++;

				ctx->stat_frag_tris+=2;
				wasfrag|=1;

				continue;
			}
#endif

#if 1
			if((g0+g2)>(g1+g3))
			{
				TKRA_TransformCalcMidpointVertex(ctx, &v8, &v4, &v6, prjmat);
			}else
			{
				TKRA_TransformCalcMidpointVertex(ctx, &v8, &v5, &v7, prjmat);
			}

			v0stk[vstkpos]=v0;
			v1stk[vstkpos]=v4;
			v2stk[vstkpos]=v8;
			v3stk[vstkpos]=v7;
			vstkpos++;

			v0stk[vstkpos]=v4;
			v1stk[vstkpos]=v1;
			v2stk[vstkpos]=v5;
			v3stk[vstkpos]=v8;
			vstkpos++;

			v0stk[vstkpos]=v5;
			v1stk[vstkpos]=v2;
			v2stk[vstkpos]=v6;
			v3stk[vstkpos]=v8;
			vstkpos++;

			v0stk[vstkpos]=v6;
			v1stk[vstkpos]=v3;
			v2stk[vstkpos]=v7;
			v3stk[vstkpos]=v8;
			vstkpos++;

			ctx->stat_frag_tris+=4;
			wasfrag|=1;

			continue;
#endif
		}

		if(tkra_v4f_w(v0ww)<0)
		{
			ctx->stat_reject_tris++;
			ctx->stat_negw_tris++;
			continue;
		}

		TKRA_FinalProjectVertex(ctx, &pv0, &v0, v0p);
		TKRA_FinalProjectVertex(ctx, &pv1, &v1, v1p);
		TKRA_FinalProjectVertex(ctx, &pv2, &v2, v2p);
		TKRA_FinalProjectVertex(ctx, &pv3, &v3, v3p);

//		ctx->stat_draw_tris++;

		i0=TKRA_EmitProjectedQuad(ctx, pv0, pv1, pv2, pv3);

		if(i0<0)
		{
			ctx->stat_reject_tris++;
			ctx->stat_zocc_tris++;
		}else
		{
			ctx->stat_draw_tris++;
		}
	}
	
	return(0);
}

