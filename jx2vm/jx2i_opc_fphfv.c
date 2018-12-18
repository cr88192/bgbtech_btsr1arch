u16 bjx2_f2h(float f)
{
	u32 v0;
	u16 v1, vo;
	int ex, zm, om;
	
	v0=*(u32 *)(&f);

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
	return(*(float *)(&v1));
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
		break;
	}
	
	v2=jx2_mkvec_hf(tv2[0], tv2[1], tv2[2], tv2[3]);
	return(v2);
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
