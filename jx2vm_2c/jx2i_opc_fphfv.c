u16 bjx2_f2h(float f)
{
	u32 v0;
	u16 v1, vo;
	int ex, zm, om;
	
//	v0=*(u32 *)(&f);
	v0=BJX2_PtrGetDWord(&f);

	ex=((v0>>23)&256)-(127-15);
	zm=~(ex>>31);
	om=(ex>30)-1;
	v0+=4095;
	v1=((v0>>16)&0xC000) | ((v0>>13)&0x3FFF);
	vo=((v0>>16)&0x8000) | 0x7C00;
	v1=(v1&om)|(vo&(~om));
	v1=v1&zm;
	return(v1);
}

float bjx2_h2f(u16 v0)
{
	u32 v1;
	
	v1=((v0&0x8000)<<16) |
		(((v0&0x7FFF)+0x1C000)<<13);
//	return(*(float *)(&v1));
	return(BJX2_PtrGetFloat(&v1));
}

float bjx2_ss2f(u32 v0)
{
	u32 v1;
	
//	v1=((v0&0x8000)<<16) |
//		(((v0&0x7FFF)+0x1C000)<<13);
//	return(*(float *)(&v0));
//	return(BJX2_PtrGetFloat(&v1));
	return(BJX2_PtrGetFloat(&v0));
}

u32 bjx2_f2ss(float f)
{
	u32 v0;
	u16 v1, vo;
	int ex, zm, om;
	
//	v0=*(u32 *)(&f);
	v0=BJX2_PtrGetDWord(&f);
	return(v0);
}

u64 jx2_mkvec_hf(float f0, float f1, float f2, float f3)
{
	u16 hv0, hv1, hv2, hv3;
	u32 v0, v1;
	u64 v;
	
	hv0=bjx2_f2h(f0);
	hv1=bjx2_f2h(f1);
	hv2=bjx2_f2h(f2);
	hv3=bjx2_f2h(f3);
	
	v0=hv0 | (hv1<<16);
	v1=hv2 | (hv3<<16);
	v=v0 | (((u64)v1)<<32);
	return(v);
}

void jx2_upvec_hf(float *fv, u64 v0)
{
	fv[0]=bjx2_h2f(v0>> 0);
	fv[1]=bjx2_h2f(v0>>16);
	fv[2]=bjx2_h2f(v0>>32);
	fv[3]=bjx2_h2f(v0>>48);
}

u64 jx2_dovec_hf(u64 v0, u64 v1, int op)
{
	float tv0[4], tv1[4], tv2[4];
	u64 v2;

	jx2_upvec_hf(tv0, v0);
	jx2_upvec_hf(tv1, v1);
	
	switch(op)
	{
	case 0:
		tv2[0]=0;
		tv2[1]=0;
		tv2[2]=0;
		tv2[3]=0;
		break;
	case 1:
		tv2[0]=tv0[0]+tv1[0];
		tv2[1]=tv0[1]+tv1[1];
		tv2[2]=tv0[2]+tv1[2];
		tv2[3]=tv0[3]+tv1[3];
		break;
	case 2:
		tv2[0]=tv0[0]-tv1[0];
		tv2[1]=tv0[1]-tv1[1];
		tv2[2]=tv0[2]-tv1[2];
		tv2[3]=tv0[3]-tv1[3];
		break;
	case 3:
		tv2[0]=tv0[0]*tv1[0];
		tv2[1]=tv0[1]*tv1[1];
		tv2[2]=tv0[2]*tv1[2];
		tv2[3]=tv0[3]*tv1[3];
		break;
	default:
		tv2[0]=0;
		tv2[1]=0;
		tv2[2]=0;
		tv2[3]=0;
		break;
	}
	
	v2=jx2_mkvec_hf(tv2[0], tv2[1], tv2[2], tv2[3]);
	return(v2);
}

void jx2_upvec_f(float *fv, u64 v0)
{
	fv[0]=bjx2_ss2f(v0>> 0);
	fv[1]=bjx2_ss2f(v0>>32);
}

u64 jx2_mkvec_f(float f0, float f1)
{
	u16 hv0, hv1, hv2, hv3;
	u32 v0, v1;
	u64 v;
	
	v0=bjx2_f2ss(f0);
	v1=bjx2_f2ss(f1);
	v=v0 | (((u64)v1)<<32);
	return(v);
}

void BJX2_Op_PADDH_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs, vt, vn;

	vs=ctx->regs[op->rn];	vt=ctx->regs[op->rn];
	jx2_upvec_hf(tv0, vs);	jx2_upvec_hf(tv1, vt);
	tv2[0]=tv0[0]+tv1[0];	tv2[1]=tv0[1]+tv1[1];
	tv2[2]=tv0[2]+tv1[2];	tv2[3]=tv0[3]+tv1[3];
	vn=jx2_mkvec_hf(tv2[0], tv2[1], tv2[2], tv2[3]);
	
	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PSUBH_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs, vt, vn;

	vs=ctx->regs[op->rn];	vt=ctx->regs[op->rn];
	jx2_upvec_hf(tv0, vs);	jx2_upvec_hf(tv1, vt);
	tv2[0]=tv0[0]-tv1[0];	tv2[1]=tv0[1]-tv1[1];
	tv2[2]=tv0[2]-tv1[2];	tv2[3]=tv0[3]-tv1[3];
	vn=jx2_mkvec_hf(tv2[0], tv2[1], tv2[2], tv2[3]);
	
	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PMULH_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs, vt, vn;

	vs=ctx->regs[op->rn];	vt=ctx->regs[op->rn];
	jx2_upvec_hf(tv0, vs);	jx2_upvec_hf(tv1, vt);
	tv2[0]=tv0[0]*tv1[0];	tv2[1]=tv0[1]*tv1[1];
	tv2[2]=tv0[2]*tv1[2];	tv2[3]=tv0[3]*tv1[3];
	vn=jx2_mkvec_hf(tv2[0], tv2[1], tv2[2], tv2[3]);
	
	ctx->regs[op->rn]=vn;
}


void BJX2_Op_PADDF_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs, vt, vn;

	vs=ctx->regs[op->rm];	vt=ctx->regs[op->ro];
	jx2_upvec_f(tv0, vs);	jx2_upvec_f(tv1, vt);
	tv2[0]=tv0[0]+tv1[0];	tv2[1]=tv0[1]+tv1[1];
	vn=jx2_mkvec_f(tv2[0], tv2[1]);
	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PSUBF_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs, vt, vn;

	vs=ctx->regs[op->rm];	vt=ctx->regs[op->ro];
	jx2_upvec_f(tv0, vs);	jx2_upvec_f(tv1, vt);
	tv2[0]=tv0[0]-tv1[0];	tv2[1]=tv0[1]-tv1[1];
	vn=jx2_mkvec_f(tv2[0], tv2[1]);
	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PMULF_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs, vt, vn;

	vs=ctx->regs[op->rm];	vt=ctx->regs[op->ro];
	jx2_upvec_f(tv0, vs);	jx2_upvec_f(tv1, vt);
	tv2[0]=tv0[0]*tv1[0];	tv2[1]=tv0[1]*tv1[1];
	vn=jx2_mkvec_f(tv2[0], tv2[1]);
	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PADDXF_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs0, vs1, vt0, vt1, vn0, vn1;

	vs0=ctx->regs[op->rm+0];	vt0=ctx->regs[op->ro+0];
	vs1=ctx->regs[op->rm+1];	vt1=ctx->regs[op->ro+1];
	jx2_upvec_f(tv0+0, vs0);	jx2_upvec_f(tv1+0, vt0);
	jx2_upvec_f(tv0+2, vs1);	jx2_upvec_f(tv1+2, vt1);
	tv2[0]=tv0[0]+tv1[0];	tv2[1]=tv0[1]+tv1[1];
	tv2[2]=tv0[2]+tv1[2];	tv2[3]=tv0[3]+tv1[3];
	vn0=jx2_mkvec_f(tv2[0], tv2[1]);
	vn1=jx2_mkvec_f(tv2[2], tv2[3]);
	ctx->regs[op->rn+0]=vn0;
	ctx->regs[op->rn+1]=vn1;
}

void BJX2_Op_PSUBXF_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs0, vs1, vt0, vt1, vn0, vn1;

	vs0=ctx->regs[op->rm+0];	vt0=ctx->regs[op->ro+0];
	vs1=ctx->regs[op->rm+1];	vt1=ctx->regs[op->ro+1];
	jx2_upvec_f(tv0+0, vs0);	jx2_upvec_f(tv1+0, vt0);
	jx2_upvec_f(tv0+2, vs1);	jx2_upvec_f(tv1+2, vt1);
	tv2[0]=tv0[0]-tv1[0];	tv2[1]=tv0[1]-tv1[1];
	tv2[2]=tv0[2]-tv1[2];	tv2[3]=tv0[3]-tv1[3];
	vn0=jx2_mkvec_f(tv2[0], tv2[1]);
	vn1=jx2_mkvec_f(tv2[2], tv2[3]);
	ctx->regs[op->rn+0]=vn0;
	ctx->regs[op->rn+1]=vn1;
}

void BJX2_Op_PMULXF_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs0, vs1, vt0, vt1, vn0, vn1;

	vs0=ctx->regs[op->rm+0];	vt0=ctx->regs[op->ro+0];
	vs1=ctx->regs[op->rm+1];	vt1=ctx->regs[op->ro+1];
	jx2_upvec_f(tv0+0, vs0);	jx2_upvec_f(tv1+0, vt0);
	jx2_upvec_f(tv0+2, vs1);	jx2_upvec_f(tv1+2, vt1);
	tv2[0]=tv0[0]*tv1[0];	tv2[1]=tv0[1]*tv1[1];
	tv2[2]=tv0[2]*tv1[2];	tv2[3]=tv0[3]*tv1[3];
	vn0=jx2_mkvec_f(tv2[0], tv2[1]);
	vn1=jx2_mkvec_f(tv2[2], tv2[3]);
	ctx->regs[op->rn+0]=vn0;
	ctx->regs[op->rn+1]=vn1;
}

void BJX2_Op_MOVHD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn;

	vs=ctx->regs[op->rm];	vt=ctx->regs[op->ro];
	vn=	((vs    )&0xFFFFFFFF00000000ULL) |
		((vt>>32)&0x00000000FFFFFFFFULL) ;
	ctx->regs[op->rn]=vn;
}

void BJX2_Op_MOVLD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn;

	vs=ctx->regs[op->rm];	vt=ctx->regs[op->ro];
	vn=	((vs<<32)&0xFFFFFFFF00000000ULL) |
		((vt    )&0x00000000FFFFFFFFULL) ;
	ctx->regs[op->rn]=vn;
}

void BJX2_Op_MOVHLD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn;

	vs=ctx->regs[op->rm];	vt=ctx->regs[op->ro];
	vn=	(vs&0xFFFFFFFF00000000ULL) |
		(vt&0x00000000FFFFFFFFULL) ;
	ctx->regs[op->rn]=vn;
}

void BJX2_Op_MOVLHD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn;

	vs=ctx->regs[op->rm];	vt=ctx->regs[op->ro];
	vn=	((vs<<32)&0xFFFFFFFF00000000ULL) |
		((vt>>32)&0x00000000FFFFFFFFULL) ;
	ctx->regs[op->rn]=vn;
}


void BJX2_Op_RGBSHR1_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn, msk;

	vs=ctx->regs[op->rm];

	msk=0x7BDE7BDE7BDE7BDEULL;
	vn=(vs&msk)>>1;

	ctx->regs[op->rn]=vn;
}

void BJX2_Op_RGBAVG_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn, msk;

	vs=ctx->regs[op->rm];	vt=ctx->regs[op->ro];

	msk=0x7BDE7BDE7BDE7BDEULL;
	vn=((vs&msk)>>1)+((vt&msk)>>1);

	ctx->regs[op->rn]=vn;
}

u16 bjx2_rgbmod(u16 px0, u16 px1)
{
	int cr, cg, cb, cc;

	cr=((px0>>10)&31)*((px1>>10)&31);	cr=(cr+15)>>5;
	cg=((px0>> 5)&31)*((px1>> 5)&31);	cg=(cg+15)>>5;
	cb=((px0>> 0)&31)*((px1>> 0)&31);	cb=(cb+15)>>5;
	if(cr>31)cr=31;
	if(cg>31)cg=31;
	if(cb>31)cb=31;
	cc=(cr<<10)|(cg<<5)|cb;
	
	if(px0&0x8000)
	{
		if(px1&0x8000)
		{
			cc=(cc&0x7BDE)|0x8000|(px0&0x0421);
		}else
		{
			cc=(cc&0x7BDE)|0x8000|(px0&0x0421);
		}
	}else
	{
		if(px1&0x8000)
		{
			cc=(cc&0x7BDE)|0x8000|(px1&0x0421);
		}
	}

	return(cc);
}

void BJX2_Op_RGBMOD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn, msk;

	vs=ctx->regs[op->rm];	vt=ctx->regs[op->ro];

	vn=
		(((u64)bjx2_rgbmod(vs>> 0, vt>> 0))    )	|
		(((u64)bjx2_rgbmod(vs>>16, vt>>16))<<16)	|
		(((u64)bjx2_rgbmod(vs>>32, vt>>32))<<16)	|
		(((u64)bjx2_rgbmod(vs>>48, vt>>16))<<16);

	ctx->regs[op->rn]=vn;
}

void BJX2_Op_RGB5PCK32_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn, msk;

	vs=ctx->regs[op->rm];

	vn=	((vs>> 3)&0x0000001F)|
		((vs>> 6)&0x000003E0)|
		((vs>> 9)&0x00007C00)|
		((vs>>19)&0x001F0000)|
		((vs>>22)&0x03E00000)|
		((vs>>25)&0x7C000000);
//	vn|=vn<<16;
	vn|=vn<<32;

	ctx->regs[op->rn]=vn;
}


void BJX2_Op_RGB5UPCK32_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn, msk;

	vs=ctx->regs[op->rm];

	vn=	((vs&0x0000001F)<< 3)|
		((vs&0x000003E0)<< 6)|
		((vs&0x00007C00)<< 9)|
		((vs&0x001F0000)<<19)|
		((vs&0x03E00000)<<22)|
		((vs&0x7C000000)<<25)|
		0xFF000000FF000000ULL;

	ctx->regs[op->rn]=vn;
}

void BJX2_Op_RGB5PCK64_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn, msk;

	vs=ctx->regs[op->rm];

	vn=	((vs>>11)&0x001F)|
		((vs>>22)&0x03E0)|
		((vs>>33)&0x7C00);
	vn|=vn<<16;
	vn|=vn<<32;

	ctx->regs[op->rn]=vn;
}


void BJX2_Op_RGB5UPCK64_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn, msk;

	vs=ctx->regs[op->rm];

	vn=	((vs&0x001F)<<11)|
		((vs&0x03E0)<<22)|
		((vs&0x7C00)<<33)|
		0xFFFF000000000000ULL;

	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PMULSLW_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int ia, ib, ic, id, ja, jb, jc, jd, ka, kb, kc, kd;
	u64	vs, vt, vn, msk;

	vs=ctx->regs[op->rm];	vt=ctx->regs[op->ro];

	ia=(s16)(vs    );	ib=(s16)(vs>>16);
	ic=(s16)(vs>>32);	id=(s16)(vs>>48);
	ja=(s16)(vt    );	jb=(s16)(vt>>16);
	jc=(s16)(vt>>32);	jd=(s16)(vt>>48);
	ka=ia*ja;			kb=ib*jb;
	kc=ic*jc;			kd=id*jd;
	
	vn=	(((u64)((u16)ka))    ) |
		(((u64)((u16)kb))<<16) |
		(((u64)((u16)kc))<<32) |
		(((u64)((u16)kd))<<48) ;

	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PMULULW_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int ia, ib, ic, id, ja, jb, jc, jd, ka, kb, kc, kd;
	u64	vs, vt, vn, msk;

	vs=ctx->regs[op->rm];	vt=ctx->regs[op->ro];

	ia=(u16)(vs    );	ib=(u16)(vs>>16);
	ic=(u16)(vs>>32);	id=(u16)(vs>>48);
	ja=(u16)(vt    );	jb=(u16)(vt>>16);
	jc=(u16)(vt>>32);	jd=(u16)(vt>>48);
	ka=ia*ja;			kb=ib*jb;
	kc=ic*jc;			kd=id*jd;
	
	vn=	(((u64)((u16)ka))    ) |
		(((u64)((u16)kb))<<16) |
		(((u64)((u16)kc))<<32) |
		(((u64)((u16)kd))<<48) ;

	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PMULSHW_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int ia, ib, ic, id, ja, jb, jc, jd, ka, kb, kc, kd;
	u64	vs, vt, vn, msk;

	vs=ctx->regs[op->rm];	vt=ctx->regs[op->ro];

	ia=(s16)(vs    );	ib=(s16)(vs>>16);
	ic=(s16)(vs>>32);	id=(s16)(vs>>48);
	ja=(s16)(vt    );	jb=(s16)(vt>>16);
	jc=(s16)(vt>>32);	jd=(s16)(vt>>48);
	ka=(ia*ja)>>16;		kb=(ib*jb)>>16;
	kc=(ic*jc)>>16;		kd=(id*jd)>>16;
	
	vn=	(((u64)((u16)ka))    ) |
		(((u64)((u16)kb))<<16) |
		(((u64)((u16)kc))<<32) |
		(((u64)((u16)kd))<<48) ;

	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PMULUHW_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int ia, ib, ic, id, ja, jb, jc, jd, ka, kb, kc, kd;
	u64	vs, vt, vn, msk;

	vs=ctx->regs[op->rm];	vt=ctx->regs[op->ro];

	ia=(u16)(vs    );	ib=(u16)(vs>>16);
	ic=(u16)(vs>>32);	id=(u16)(vs>>48);
	ja=(u16)(vt    );	jb=(u16)(vt>>16);
	jc=(u16)(vt>>32);	jd=(u16)(vt>>48);
	ka=(ia*ja)>>16;		kb=(ib*jb)>>16;
	kc=(ic*jc)>>16;		kd=(id*jd)>>16;
	
	vn=	(((u64)((u16)ka))    ) |
		(((u64)((u16)kb))<<16) |
		(((u64)((u16)kc))<<32) |
		(((u64)((u16)kd))<<48) ;

	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PSHUFB_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn, msk;
	int imm;

	imm=op->imm;
	vs=ctx->regs[op->rm];	// vt=ctx->regs[op->ro];

	vn=	(((vs>>(((imm   )&3)*8))&255)    ) |
		(((vs>>(((imm>>2)&3)*8))&255)<< 8) |
		(((vs>>(((imm>>4)&3)*8))&255)<<16) |
		(((vs>>(((imm>>6)&3)*8))&255)<<24) |
		(vs&0xFFFFFFFF00000000ULL);


	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PSHUFW_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn, msk;
	int imm;

	imm=op->imm;
	vs=ctx->regs[op->rm];	// vt=ctx->regs[op->ro];

	vn=	(((vs>>(((imm   )&3)*16))&65535)    ) |
		(((vs>>(((imm>>2)&3)*16))&65535)<<16) |
		(((vs>>(((imm>>4)&3)*16))&65535)<<32) |
		(((vs>>(((imm>>6)&3)*16))&65535)<<48) ;

	ctx->regs[op->rn]=vn;
}
