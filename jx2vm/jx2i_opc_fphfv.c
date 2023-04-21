/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

u16 bjx2_f2h(float f)
{
	u32 v0;
	u16 v1, vo;
	int ex, zm, om;
	
//	v0=*(u32 *)(&f);
	v0=BJX2_PtrGetDWord(&f);

	ex=((v0>>23)&255)-(127-15);
//	if(ex<=0)
//		return(0);
	zm=~(ex>>31);
	om=(ex>30)-1;
//	v0+=4095;
	v1=((v0>>16)&0xC000) | ((v0>>13)&0x3FFF);
	vo=((v0>>16)&0x8000) | 0x7C00;
	v1=(v1&om)|(vo&(~om));
	v1=v1&zm;
	return(v1);
}

float bjx2_h2f(u16 v0)
{
	u32 v1;
	
	if(!v0)
		return(0);
	
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
//	u16 v1, vo;
//	int ex, zm, om;
	
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

	if(op->imm&8)
	{
		vs&=0xFFFFFF00FFFFFF00ULL;
		vt&=0xFFFFFF00FFFFFF00ULL;
	}
	
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

	if(op->imm&8)
	{
		vs&=0xFFFFFF00FFFFFF00ULL;
		vt&=0xFFFFFF00FFFFFF00ULL;
	}
	
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

	if(op->imm&8)
	{
		vs&=0xFFFFFF00FFFFFF00ULL;
		vt&=0xFFFFFF00FFFFFF00ULL;
	}
	
	jx2_upvec_f(tv0, vs);	jx2_upvec_f(tv1, vt);
	tv2[0]=tv0[0]*tv1[0];	tv2[1]=tv0[1]*tv1[1];
	vn=jx2_mkvec_f(tv2[0], tv2[1]);
	ctx->regs[op->rn]=vn;
}


#ifndef BJX2_CFG_SIMDFXASS
u32 BJX2_OpI_FaddSf(u32 va, u32 vb)
{
	int fra, frb, frc, exa, exb, exc, sga, sgb, sgc;
	int d, bn;
	u32 vc;

	sga=va>>31;
	exa=(va>>23)&255;
	fra=((va>>7)&0xFFFF);
	if(exa!=0)
		fra|=0x10000;

	sgb=vb>>31;
	exb=(vb>>23)&255;
	frb=((vb>>7)&0xFFFF);
	if(exb!=0)
		frb|=0x10000;
	
	if(exb>exa)
	{
		d=sga; sga=sgb; sgb=d;
		d=exa; exa=exb; exb=d;
		d=fra; fra=frb; frb=d;
	}
	
	d=exa-exb;
	if(d>31)
		d=31;
	
	bn=frb>>d;
	if(sgb!=sga)
		bn=~bn;
	
	sgc=sga;
	exc=exa;
	frc=fra+bn;
	
	if(frc>>31)
	{
		sgc=!sgc;
		frc=~frc;
	}
	
	if(frc&0x20000)
	{
		frc>>=1;
		exc++;
	}else
	{
		if(!(frc&(~1)))
			return(0);
		if(!(frc&0x1FE00))
			{ exc-=8; frc<<=8; }
		if(!(frc&0x1E000))
			{ exc-=4; frc<<=4; }
		if(!(frc&0x18000))
			{ exc-=2; frc<<=2; }
		if(!(frc&0x10000))
			{ exc-=1; frc<<=1; }
	}
	
	if(exc<=0)
		return(0);
		
	vc=(sgc<<31)|(exc<<23)|((frc&0xFFFF)<<7);
	return(vc);
}

u32 BJX2_OpI_FmulSf(u32 va, u32 vb)
{
	int fra, frb, frc, exa, exb, exc, sga, sgb, sgc;
	int d, bn;
	u32 vc;

	sga=va>>31;
	exa=(va>>23)&255;
	fra=((va>>7)&0xFFFF)|0x10000;

	sgb=vb>>31;
	exb=(vb>>23)&255;
	frb=((vb>>7)&0xFFFF)|0x10000;
	
	sgc=sga^sgb;
	exc=(exa+exb)-127;
	if(exc<=0)
		return(0);
	
	frc=(((s64)fra)*(frb))>>16;
	
	if(frc&0x20000)
	{
		exc++;
		frc=frc>>1;
	}

	vc=(sgc<<31)|(exc<<23)|((frc&0xFFFF)<<7);
	return(vc);
}
#endif

#ifdef BJX2_CFG_SIMDFXASS
u32 BJX2_OpI_FaddSf(u32 va, u32 vb)
{
	int fra, frb, frc, exa, exb, exc, sga, sgb, sgc;
	int d, bn;
	u32 vc;

	sga=va>>31;
	exa=(va>>23)&255;
	fra=((va>>0)&0x7FFFFF);
	if(exa!=0)
		fra|=0x800000;

	sgb=vb>>31;
	exb=(vb>>23)&255;
	frb=((vb>>0)&0x7FFFFF);
	if(exb!=0)
		frb|=0x800000;
	
	if(exb>exa)
	{
		d=sga; sga=sgb; sgb=d;
		d=exa; exa=exb; exb=d;
		d=fra; fra=frb; frb=d;
	}
	
	d=exa-exb;
	if(d>31)
		d=31;
	
	bn=frb>>d;
	if(sgb!=sga)
	{
//		bn=~bn;
		bn=-bn;
	}
	
	sgc=sga;
	exc=exa;
	frc=fra+bn;
	
	if(frc>>31)
	{
		sgc=!sgc;
		frc=~frc;
	}
	
	if(frc&0x1000000)
	{
		frc>>=1;
		exc++;
	}else
	{
		if(!(frc&(~1)))
			return(0);
		if(!(frc&0xFFFF00))
			{ exc-=16; frc<<=16; }
		if(!(frc&0xFF0000))
			{ exc-= 8; frc<<= 8; }
		if(!(frc&0xF00000))
			{ exc-= 4; frc<<= 4; }
		if(!(frc&0xC00000))
			{ exc-= 2; frc<<= 2; }
		if(!(frc&0x800000))
			{ exc-= 1; frc<<= 1; }
	}
	
	if(exc<=0)
		return(0);
		
	vc=(sgc<<31)|(exc<<23)|((frc&0x7FFFFF)<<0);
	return(vc);
}

u32 BJX2_OpI_FmulSf(u32 va, u32 vb)
{
	int fra, frb, frc, exa, exb, exc, sga, sgb, sgc;
	int d, bn;
	u32 vc;
	
	if(!va || !vb)
		return(0);

	sga=va>>31;
	exa=(va>>23)&255;
	fra=((va>>0)&0x7FFFFF)|0x800000;

	sgb=vb>>31;
	exb=(vb>>23)&255;
	frb=((vb>>0)&0x7FFFFF)|0x800000;
	
	sgc=sga^sgb;
	exc=(exa+exb)-127;
	if(exc<=0)
		return(0);
	
//	frc=(((s64)fra)*(frb))>>16;
	frc=(((s64)fra)*(frb))>>23;

	/* Cheese the multiply. */
//	frc=(((s64)(fra>>7))*(frb>>7))>>9;
//	frc+=(fra&127)+(frb&127);
//	frc+=((fra&127)*(frb>>20)+(frb&127)*(fra>>20))>>3;
//	frc+=((fra&127)*(frb>>17)+(frb&127)*(fra>>17))>>6;
	
	if(frc&0x1000000)
	{
		exc++;
		frc=frc>>1;
	}

	vc=(sgc<<31)|(exc<<23)|((frc&0x7FFFFF)<<0);
	return(vc);
}
#endif

u64 BJX2_OpI_PAddSf(u64 va, u64 vb)
{
	u64 vc;
	u32 vc0, vc1;

	vc0=BJX2_OpI_FaddSf(va, vb);
	vc1=BJX2_OpI_FaddSf(va>>32, vb>>32);
	vc=(((u64)vc1)<<32)|vc0;
	return(vc);
}

u64 BJX2_OpI_PSubSf(u64 va, u64 vb)
{
	return(BJX2_OpI_PAddSf(va, vb^0x8000000080000000ULL));
}

u64 BJX2_OpI_PMulSf(u64 va, u64 vb)
{
	u64 vc;
	u32 vc0, vc1;

	vc0=BJX2_OpI_FmulSf(va, vb);
	vc1=BJX2_OpI_FmulSf(va>>32, vb>>32);
	vc=(((u64)vc1)<<32)|vc0;
	return(vc);
}

void BJX2_Op_PADDXFA_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs0, vs1, vt0, vt1, vn0, vn1;

	vs0=ctx->regs[op->rm+0];	vt0=ctx->regs[op->ro+0];
	vs1=ctx->regs[op->rm+1];	vt1=ctx->regs[op->ro+1];

	vn0=BJX2_OpI_PAddSf(vs0, vt0);
	vn1=BJX2_OpI_PAddSf(vs1, vt1);

	ctx->regs[op->rn+0]=vn0;
	ctx->regs[op->rn+1]=vn1;
}

void BJX2_Op_PSUBXFA_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs0, vs1, vt0, vt1, vn0, vn1;

	vs0=ctx->regs[op->rm+0];	vt0=ctx->regs[op->ro+0];
	vs1=ctx->regs[op->rm+1];	vt1=ctx->regs[op->ro+1];

	vn0=BJX2_OpI_PSubSf(vs0, vt0);
	vn1=BJX2_OpI_PSubSf(vs1, vt1);

	ctx->regs[op->rn+0]=vn0;
	ctx->regs[op->rn+1]=vn1;
}

void BJX2_Op_PMULXFA_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs0, vs1, vt0, vt1, vn0, vn1;

	vs0=ctx->regs[op->rm+0];	vt0=ctx->regs[op->ro+0];
	vs1=ctx->regs[op->rm+1];	vt1=ctx->regs[op->ro+1];

	vn0=BJX2_OpI_PMulSf(vs0, vt0);
	vn1=BJX2_OpI_PMulSf(vs1, vt1);

	ctx->regs[op->rn+0]=vn0;
	ctx->regs[op->rn+1]=vn1;
}

void BJX2_Op_PADDXF_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs0, vs1, vt0, vt1, vn0, vn1;

	vs0=ctx->regs[op->rm+0];	vt0=ctx->regs[op->ro+0];
	vs1=ctx->regs[op->rm+1];	vt1=ctx->regs[op->ro+1];

	if(op->imm&8)
	{
		op->Run=BJX2_Op_PADDXFA_RegRegReg;
		op->Run(ctx, op);
		return;

		vs0&=0xFFFFFF00FFFFFF00ULL;
		vs1&=0xFFFFFF00FFFFFF00ULL;
		vt0&=0xFFFFFF00FFFFFF00ULL;
		vt1&=0xFFFFFF00FFFFFF00ULL;
	}

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

	if(op->imm&8)
	{
		op->Run=BJX2_Op_PSUBXFA_RegRegReg;
		op->Run(ctx, op);
		return;

		vs0&=0xFFFFFF00FFFFFF00ULL;
		vs1&=0xFFFFFF00FFFFFF00ULL;
		vt0&=0xFFFFFF00FFFFFF00ULL;
		vt1&=0xFFFFFF00FFFFFF00ULL;
	}

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

	if(op->imm&8)
	{
		op->Run=BJX2_Op_PMULXFA_RegRegReg;
		op->Run(ctx, op);
		return;

		vs0&=0xFFFFFF00FFFFFF00ULL;
		vs1&=0xFFFFFF00FFFFFF00ULL;
		vt0&=0xFFFFFF00FFFFFF00ULL;
		vt1&=0xFFFFFF00FFFFFF00ULL;
	}

	jx2_upvec_f(tv0+0, vs0);	jx2_upvec_f(tv1+0, vt0);
	jx2_upvec_f(tv0+2, vs1);	jx2_upvec_f(tv1+2, vt1);
	tv2[0]=tv0[0]*tv1[0];	tv2[1]=tv0[1]*tv1[1];
	tv2[2]=tv0[2]*tv1[2];	tv2[3]=tv0[3]*tv1[3];
	vn0=jx2_mkvec_f(tv2[0], tv2[1]);
	vn1=jx2_mkvec_f(tv2[2], tv2[3]);
	ctx->regs[op->rn+0]=vn0;
	ctx->regs[op->rn+1]=vn1;
}

void BJX2_Op_PADDH_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs, vt, vn;

	vs=ctx->regs[op->rm];	vt=ctx->regs[op->ro];
	jx2_upvec_hf(tv0, vs);	jx2_upvec_hf(tv1, vt);
	tv2[0]=tv0[0]+tv1[0];	tv2[1]=tv0[1]+tv1[1];
	tv2[2]=tv0[2]+tv1[2];	tv2[3]=tv0[3]+tv1[3];
	vn=jx2_mkvec_hf(tv2[0], tv2[1], tv2[2], tv2[3]);
	
	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PSUBH_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs, vt, vn;

	vs=ctx->regs[op->rm];	vt=ctx->regs[op->ro];
	jx2_upvec_hf(tv0, vs);	jx2_upvec_hf(tv1, vt);
	tv2[0]=tv0[0]-tv1[0];	tv2[1]=tv0[1]-tv1[1];
	tv2[2]=tv0[2]-tv1[2];	tv2[3]=tv0[3]-tv1[3];
	vn=jx2_mkvec_hf(tv2[0], tv2[1], tv2[2], tv2[3]);
	
	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PMULH_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs, vt, vn;

	vs=ctx->regs[op->rm];	vt=ctx->regs[op->ro];
	jx2_upvec_hf(tv0, vs);	jx2_upvec_hf(tv1, vt);
	tv2[0]=tv0[0]*tv1[0];	tv2[1]=tv0[1]*tv1[1];
	tv2[2]=tv0[2]*tv1[2];	tv2[3]=tv0[3]*tv1[3];
	vn=jx2_mkvec_hf(tv2[0], tv2[1], tv2[2], tv2[3]);
	
	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PADDH_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs, vt, vn;

	vs=ctx->regs[op->rm];
//	vt=ctx->regs[op->ro];
	vt=op->imm;
	jx2_upvec_hf(tv0, vs);	jx2_upvec_hf(tv1, vt);
	tv2[0]=tv0[0]+tv1[0];	tv2[1]=tv0[1]+tv1[1];
	tv2[2]=tv0[2]+tv1[2];	tv2[3]=tv0[3]+tv1[3];
	vn=jx2_mkvec_hf(tv2[0], tv2[1], tv2[2], tv2[3]);
	
	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PMULH_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs, vt, vn;

	vs=ctx->regs[op->rm];
//	vt=ctx->regs[op->ro];
	vt=op->imm;
	jx2_upvec_hf(tv0, vs);	jx2_upvec_hf(tv1, vt);
	tv2[0]=tv0[0]*tv1[0];	tv2[1]=tv0[1]*tv1[1];
	tv2[2]=tv0[2]*tv1[2];	tv2[3]=tv0[3]*tv1[3];
	vn=jx2_mkvec_hf(tv2[0], tv2[1], tv2[2], tv2[3]);
	
	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PADDXF_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs0, vs1, vt0, vt1, vn0, vn1;

	vs0=ctx->regs[op->rm+0];
	vs1=ctx->regs[op->rm+1];

	jx2_upvec_f(tv0+0, vs0);
	jx2_upvec_f(tv0+2, vs1);
	jx2_upvec_hf(tv1, op->imm);
	tv2[0]=tv0[0]+tv1[0];	tv2[1]=tv0[1]+tv1[1];
	tv2[2]=tv0[2]+tv1[2];	tv2[3]=tv0[3]+tv1[3];
	vn0=jx2_mkvec_f(tv2[0], tv2[1]);
	vn1=jx2_mkvec_f(tv2[2], tv2[3]);
	ctx->regs[op->rn+0]=vn0;
	ctx->regs[op->rn+1]=vn1;
}

void BJX2_Op_PMULXF_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs0, vs1, vt0, vt1, vn0, vn1;

	vs0=ctx->regs[op->rm+0];
	vs1=ctx->regs[op->rm+1];

	jx2_upvec_f(tv0+0, vs0);
	jx2_upvec_f(tv0+2, vs1);
	jx2_upvec_hf(tv1, op->imm);
	tv2[0]=tv0[0]*tv1[0];	tv2[1]=tv0[1]*tv1[1];
	tv2[2]=tv0[2]*tv1[2];	tv2[3]=tv0[3]*tv1[3];
	vn0=jx2_mkvec_f(tv2[0], tv2[1]);
	vn1=jx2_mkvec_f(tv2[2], tv2[3]);
	ctx->regs[op->rn+0]=vn0;
	ctx->regs[op->rn+1]=vn1;
}


void bjx2_opr_shuffle4xfloat(float *vec, int sh)
{
	float tv[4];
	tv[0]=vec[(sh>>0)&3];
	tv[1]=vec[(sh>>2)&3];
	tv[2]=vec[(sh>>4)&3];
	tv[3]=vec[(sh>>6)&3];
	vec[0]=tv[0];
	vec[1]=tv[1];
	vec[2]=tv[2];
	vec[3]=tv[3];
}

void BJX2_Op_PSHADDXF_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs0, vs1, vt0, vt1, vn0, vn1;

	vs0=ctx->regs[op->rm+0];	vt0=ctx->regs[op->ro+0];
	vs1=ctx->regs[op->rm+1];	vt1=ctx->regs[op->ro+1];

	jx2_upvec_f(tv0+0, vs0);	jx2_upvec_f(tv1+0, vt0);
	jx2_upvec_f(tv0+2, vs1);	jx2_upvec_f(tv1+2, vt1);

	if(op->imm&0x200)
		bjx2_opr_shuffle4xfloat(tv0, op->imm);
	if(op->imm&0x400)
		bjx2_opr_shuffle4xfloat(tv1, op->imm);

	tv2[0]=tv0[0]+tv1[0];	tv2[1]=tv0[1]+tv1[1];
	tv2[2]=tv0[2]+tv1[2];	tv2[3]=tv0[3]+tv1[3];
	vn0=jx2_mkvec_f(tv2[0], tv2[1]);
	vn1=jx2_mkvec_f(tv2[2], tv2[3]);
	ctx->regs[op->rn+0]=vn0;
	ctx->regs[op->rn+1]=vn1;
}

void BJX2_Op_PSHSUBXF_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs0, vs1, vt0, vt1, vn0, vn1;

	vs0=ctx->regs[op->rm+0];	vt0=ctx->regs[op->ro+0];
	vs1=ctx->regs[op->rm+1];	vt1=ctx->regs[op->ro+1];

	jx2_upvec_f(tv0+0, vs0);	jx2_upvec_f(tv1+0, vt0);
	jx2_upvec_f(tv0+2, vs1);	jx2_upvec_f(tv1+2, vt1);

	if(op->imm&0x200)
		bjx2_opr_shuffle4xfloat(tv0, op->imm);
	if(op->imm&0x400)
		bjx2_opr_shuffle4xfloat(tv1, op->imm);

	tv2[0]=tv0[0]-tv1[0];	tv2[1]=tv0[1]-tv1[1];
	tv2[2]=tv0[2]-tv1[2];	tv2[3]=tv0[3]-tv1[3];
	vn0=jx2_mkvec_f(tv2[0], tv2[1]);
	vn1=jx2_mkvec_f(tv2[2], tv2[3]);
	ctx->regs[op->rn+0]=vn0;
	ctx->regs[op->rn+1]=vn1;
}

void BJX2_Op_PSHMULXF_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs0, vs1, vt0, vt1, vn0, vn1;

	vs0=ctx->regs[op->rm+0];	vt0=ctx->regs[op->ro+0];
	vs1=ctx->regs[op->rm+1];	vt1=ctx->regs[op->ro+1];

	jx2_upvec_f(tv0+0, vs0);	jx2_upvec_f(tv1+0, vt0);
	jx2_upvec_f(tv0+2, vs1);	jx2_upvec_f(tv1+2, vt1);

	if(op->imm&0x200)
		bjx2_opr_shuffle4xfloat(tv0, op->imm);
	if(op->imm&0x400)
		bjx2_opr_shuffle4xfloat(tv1, op->imm);

	tv2[0]=tv0[0]*tv1[0];	tv2[1]=tv0[1]*tv1[1];
	tv2[2]=tv0[2]*tv1[2];	tv2[3]=tv0[3]*tv1[3];
	vn0=jx2_mkvec_f(tv2[0], tv2[1]);
	vn1=jx2_mkvec_f(tv2[2], tv2[3]);
	ctx->regs[op->rn+0]=vn0;
	ctx->regs[op->rn+1]=vn1;
}

void BJX2_Op_PSHADDH_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs, vt, vn;

	vs=ctx->regs[op->rm];	vt=ctx->regs[op->ro];
	jx2_upvec_hf(tv0, vs);	jx2_upvec_hf(tv1, vt);
	if(op->imm&0x200)
		bjx2_opr_shuffle4xfloat(tv0, op->imm);
	if(op->imm&0x400)
		bjx2_opr_shuffle4xfloat(tv1, op->imm);
	
	tv2[0]=tv0[0]+tv1[0];	tv2[1]=tv0[1]+tv1[1];
	tv2[2]=tv0[2]+tv1[2];	tv2[3]=tv0[3]+tv1[3];
	vn=jx2_mkvec_hf(tv2[0], tv2[1], tv2[2], tv2[3]);
	
	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PSHSUBH_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs, vt, vn;

	vs=ctx->regs[op->rm];	vt=ctx->regs[op->ro];
	jx2_upvec_hf(tv0, vs);	jx2_upvec_hf(tv1, vt);
	if(op->imm&0x200)
		bjx2_opr_shuffle4xfloat(tv0, op->imm);
	if(op->imm&0x400)
		bjx2_opr_shuffle4xfloat(tv1, op->imm);

	tv2[0]=tv0[0]-tv1[0];	tv2[1]=tv0[1]-tv1[1];
	tv2[2]=tv0[2]-tv1[2];	tv2[3]=tv0[3]-tv1[3];
	vn=jx2_mkvec_hf(tv2[0], tv2[1], tv2[2], tv2[3]);
	
	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PSHMULH_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs, vt, vn;

	vs=ctx->regs[op->rm];	vt=ctx->regs[op->ro];
	jx2_upvec_hf(tv0, vs);	jx2_upvec_hf(tv1, vt);
	if(op->imm&0x200)
		bjx2_opr_shuffle4xfloat(tv0, op->imm);
	if(op->imm&0x400)
		bjx2_opr_shuffle4xfloat(tv1, op->imm);

	tv2[0]=tv0[0]*tv1[0];	tv2[1]=tv0[1]*tv1[1];
	tv2[2]=tv0[2]*tv1[2];	tv2[3]=tv0[3]*tv1[3];
	vn=jx2_mkvec_hf(tv2[0], tv2[1], tv2[2], tv2[3]);
	
	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PSHADDH_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs, vt, vn;
	int sh;

	vs=ctx->regs[op->rm];
//	vt=ctx->regs[op->ro];
	vt=op->imm;
	
	sh=	(((vt>>50)&3)<<6)	|
		(((vt>>34)&3)<<4)	|
		(((vt>>18)&3)<<2)	|
		(((vt>> 2)&3)<<0)	;
	
	jx2_upvec_hf(tv0, vs);	jx2_upvec_hf(tv1, vt);
	bjx2_opr_shuffle4xfloat(tv0, sh);
	tv2[0]=tv0[0]+tv1[0];	tv2[1]=tv0[1]+tv1[1];
	tv2[2]=tv0[2]+tv1[2];	tv2[3]=tv0[3]+tv1[3];
	vn=jx2_mkvec_hf(tv2[0], tv2[1], tv2[2], tv2[3]);
	
	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PSHMULH_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs, vt, vn;
	int sh;

	vs=ctx->regs[op->rm];
//	vt=ctx->regs[op->ro];
	vt=op->imm;

	sh=	(((vt>>50)&3)<<6)	|
		(((vt>>34)&3)<<4)	|
		(((vt>>18)&3)<<2)	|
		(((vt>> 2)&3)<<0)	;
	
	jx2_upvec_hf(tv0, vs);	jx2_upvec_hf(tv1, vt);
	bjx2_opr_shuffle4xfloat(tv0, sh);
	tv2[0]=tv0[0]*tv1[0];	tv2[1]=tv0[1]*tv1[1];
	tv2[2]=tv0[2]*tv1[2];	tv2[3]=tv0[3]*tv1[3];
	vn=jx2_mkvec_hf(tv2[0], tv2[1], tv2[2], tv2[3]);
	
	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PSHADDXF_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs0, vs1, vt0, vt1, vn0, vn1;
	int sh;

	vs0=ctx->regs[op->rm+0];
	vs1=ctx->regs[op->rm+1];

	sh=	(((op->imm>>50)&3)<<6)	|
		(((op->imm>>34)&3)<<4)	|
		(((op->imm>>18)&3)<<2)	|
		(((op->imm>> 2)&3)<<0)	;
	
	jx2_upvec_f(tv0+0, vs0);
	jx2_upvec_f(tv0+2, vs1);
	jx2_upvec_hf(tv1, op->imm);
	bjx2_opr_shuffle4xfloat(tv0, sh);

	tv2[0]=tv0[0]+tv1[0];	tv2[1]=tv0[1]+tv1[1];
	tv2[2]=tv0[2]+tv1[2];	tv2[3]=tv0[3]+tv1[3];
	vn0=jx2_mkvec_f(tv2[0], tv2[1]);
	vn1=jx2_mkvec_f(tv2[2], tv2[3]);
	ctx->regs[op->rn+0]=vn0;
	ctx->regs[op->rn+1]=vn1;
}

void BJX2_Op_PSHMULXF_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u64	vs0, vs1, vt0, vt1, vn0, vn1;
	int sh;

	vs0=ctx->regs[op->rm+0];
	vs1=ctx->regs[op->rm+1];

	sh=	(((op->imm>>50)&3)<<6)	|
		(((op->imm>>34)&3)<<4)	|
		(((op->imm>>18)&3)<<2)	|
		(((op->imm>> 2)&3)<<0)	;

	jx2_upvec_f(tv0+0, vs0);
	jx2_upvec_f(tv0+2, vs1);
	jx2_upvec_hf(tv1, op->imm);
	bjx2_opr_shuffle4xfloat(tv0, sh);

	tv2[0]=tv0[0]*tv1[0];	tv2[1]=tv0[1]*tv1[1];
	tv2[2]=tv0[2]*tv1[2];	tv2[3]=tv0[3]*tv1[3];
	vn0=jx2_mkvec_f(tv2[0], tv2[1]);
	vn1=jx2_mkvec_f(tv2[2], tv2[3]);
	ctx->regs[op->rn+0]=vn0;
	ctx->regs[op->rn+1]=vn1;
}

#if 1
void BJX2_Op_PADDXD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double a0, b0, c0;
	double a1, b1, c1;
	
	a0=BJX2_PtrGetDoubleIx(ctx->regs, op->rm+0);
	a1=BJX2_PtrGetDoubleIx(ctx->regs, op->rm+1);
	b0=BJX2_PtrGetDoubleIx(ctx->regs, op->ro+0);
	b1=BJX2_PtrGetDoubleIx(ctx->regs, op->ro+1);
	c0=a0+b0;	c1=a1+b1;
	BJX2_PtrSetDoubleIx(ctx->regs, op->rn+0, c0);
	BJX2_PtrSetDoubleIx(ctx->regs, op->rn+1, c1);
}

void BJX2_Op_PSUBXD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double a0, b0, c0;
	double a1, b1, c1;
	
	a0=BJX2_PtrGetDoubleIx(ctx->regs, op->rm+0);
	a1=BJX2_PtrGetDoubleIx(ctx->regs, op->rm+1);
	b0=BJX2_PtrGetDoubleIx(ctx->regs, op->ro+0);
	b1=BJX2_PtrGetDoubleIx(ctx->regs, op->ro+1);
	c0=a0-b0;	c1=a1-b1;
	BJX2_PtrSetDoubleIx(ctx->regs, op->rn+0, c0);
	BJX2_PtrSetDoubleIx(ctx->regs, op->rn+1, c1);
}

void BJX2_Op_PMULXD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double a0, b0, c0;
	double a1, b1, c1;
	
	a0=BJX2_PtrGetDoubleIx(ctx->regs, op->rm+0);
	a1=BJX2_PtrGetDoubleIx(ctx->regs, op->rm+1);
	b0=BJX2_PtrGetDoubleIx(ctx->regs, op->ro+0);
	b1=BJX2_PtrGetDoubleIx(ctx->regs, op->ro+1);
	c0=a0*b0;	c1=a1*b1;
	BJX2_PtrSetDoubleIx(ctx->regs, op->rn+0, c0);
	BJX2_PtrSetDoubleIx(ctx->regs, op->rn+1, c1);
}
#endif

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
	int av;

	vs=ctx->regs[op->rm];

#if 0
	vn=	((vs>> 3)&0x0000001F)|
		((vs>> 6)&0x000003E0)|
		((vs>> 9)&0x00007C00)|
		((vs>>19)&0x001F0000)|
		((vs>>22)&0x03E00000)|
		((vs>>25)&0x7C000000);
//	vn|=vn<<16;
//	vn|=vn<<32;
#endif

#if 1
	vn=	((vs>> 3)&0x0000001F)|
		((vs>> 6)&0x000003E0)|
		((vs>> 9)&0x00007C00);
//	vn|=vn<<16;
//	vn|=vn<<32;

	av=(vs>>28)&15;
	if(av!=15)
	{
		vn&=0x7BDE;
		vn|=0x8000;
		if(av&8)
			vn|=0x0400;
		if(av&4)
			vn|=0x0020;
		if(av&2)
			vn|=0x0001;
	}

#endif

	ctx->regs[op->rn]=vn;
}


void BJX2_Op_RGB5UPCK32_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn, msk;
	int ax, ay, az, aw, av;
	u64 c;

	vs=ctx->regs[op->rm];

#if 0
	vn=	((vs&0x0000001F)<< 3)|
		((vs&0x000003E0)<< 6)|
		((vs&0x00007C00)<< 9)|
		((vs&0x001F0000)<<19)|
		((vs&0x03E00000)<<22)|
		((vs&0x7C000000)<<25)|
		0xFF000000FF000000ULL;
#endif

#if 1
	av=(vs&0x0001)|((vs&0x0020)>>4)|((vs&0x0400)>>8);
	av=av<<5;
	ax=(vs&0x001F)<<3;
	ay=(vs&0x03E0)>>2;
	az=(vs&0x7C00)>>7;
	ax|=ax>>5;
	ay|=ay>>5;
	az|=az>>5;
	aw=(vs&0x80)?av:0xFF;
	vn=	(((u64)ax)<< 0) | (((u64)ay)<<8) |
		(((u64)az)<<16) | (((u64)aw)<<24) ;
#endif

	ctx->regs[op->rn]=vn;
}

void BJX2_Op_RGB5PCK64_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn, msk;
	int av;

	vs=ctx->regs[op->rm];

#if 1
	vn=	((vs>>11)&0x001F)|
		((vs>>22)&0x03E0)|
		((vs>>33)&0x7C00);
	
	av=(vs>>60)&15;
	if(av!=15)
	{
		vn&=0x7BDE;
		vn|=0x8000;
		if(av&8)vn|=0x0400;
		if(av&4)vn|=0x0020;
		if(av&2)vn|=0x0001;
	}
		
//	vn|=vn<<16;
//	vn|=vn<<32;
#endif

	ctx->regs[op->rn]=vn;
}


void BJX2_Op_RGB5UPCK64_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn, msk;
	int ax, ay, az, aw, av;
	u64 c;

	vs=ctx->regs[op->rm];

#if 0
	vn=	((vs&0x001F)<<11)|
		((vs&0x03E0)<<22)|
		((vs&0x7C00)<<33)|
		0xFFFF000000000000ULL;
#endif
	
#if 1
	av=(vs&0x0001)|((vs&0x0020)>>4)|((vs&0x0400)>>8);
//	av=av<<13;
//	ax=(vs&0x001F)<<11;
//	ay=(vs&0x03E0)<< 6;
//	az=(vs&0x7C00)<< 1;

	av=av<<5;
	av=av|(av<<8);

	ax=(vs&0x001F)<<3;
	ay=(vs&0x03E0)>>2;
	az=(vs&0x7C00)>>7;
	ax|=ax>>5; ay|=ay>>5; az|=az>>5;
	ax=ax|(ax<<8);
	ay=ay|(ay<<8);
	az=az|(az<<8);

	aw=(vs&0x8000)?av:0xFFFF;
	vn=	(((u64)ax)<< 0) | (((u64)ay)<<16) |
		(((u64)az)<<32) | (((u64)aw)<<48) ;
#endif

	ctx->regs[op->rn]=vn;
}

void BJX2_Op_RGB32PCK64_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn, msk;

	vs=ctx->regs[op->rm];
	vn=	((vs>> 8)&0x000000FF)|
		((vs>>16)&0x0000FF00)|
		((vs>>24)&0x00FF0000)|
		((vs>>32)&0xFF000000);
	vn|=vn<<32;
	ctx->regs[op->rn]=vn;
}


void BJX2_Op_RGB32UPCK64_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn, msk;
	vs=ctx->regs[op->rm];
	vn=	((vs&0x000000FF)<< 8)|
		((vs&0x0000FF00)<<16)|
		((vs&0x00FF0000)<<24)|
		((vs&0xFF000000)<<32);
	vn|=(vn>>8);
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


void BJX2_Op_PCMPEQL_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;
	u32 sa0, sb0, sc0;
	u32 sa1, sb1, sc1;

	va=ctx->regs[op->rn];
	vb=ctx->regs[op->rm];

	sa0=(u32)(va);	sa1=(u32)(va>>32);
	sb0=(u32)(vb);	sb1=(u32)(vb>>32);

	if(sa0==sb0)	ctx->regs[BJX2_REG_SR]|=1;
	else			ctx->regs[BJX2_REG_SR]&=~1;
	if(sa1==sb1)	ctx->regs[BJX2_REG_SR]|=2;
	else			ctx->regs[BJX2_REG_SR]&=~2;
}

void BJX2_Op_PCMPEQW_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;
	u16 sa0, sb0, sc0;
	u16 sa1, sb1, sc1;
	u16 sa2, sb2, sc2;
	u16 sa3, sb3, sc3;
	va=ctx->regs[op->rn];	vb=ctx->regs[op->rm];
	sa0=(u16)(va);	sa1=(u16)(va>>16);	sa2=(u16)(va>>32);	sa3=(u16)(va>>48);
	sb0=(u16)(vb);	sb1=(u16)(vb>>16);	sb2=(u16)(vb>>32);	sb3=(u16)(vb>>48);

	if(sa0==sb0)	ctx->regs[BJX2_REG_SR]|= 16;
	else			ctx->regs[BJX2_REG_SR]&=~16;
	if(sa1==sb1)	ctx->regs[BJX2_REG_SR]|= 32;
	else			ctx->regs[BJX2_REG_SR]&=~32;
	if(sa2==sb2)	ctx->regs[BJX2_REG_SR]|= 64;
	else			ctx->regs[BJX2_REG_SR]&=~64;
	if(sa3==sb3)	ctx->regs[BJX2_REG_SR]|= 128;
	else			ctx->regs[BJX2_REG_SR]&=~128;
}

void BJX2_Op_PCMPHIL_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;
	u32 sa0, sb0, sc0;
	u32 sa1, sb1, sc1;

	va=ctx->regs[op->rn];
	vb=ctx->regs[op->rm];

	sa0=(u32)(va);	sa1=(u32)(va>>32);
	sb0=(u32)(vb);	sb1=(u32)(vb>>32);

	if(sa0>sb0)		ctx->regs[BJX2_REG_SR]|=1;
	else			ctx->regs[BJX2_REG_SR]&=~1;
	if(sa1>sb1)		ctx->regs[BJX2_REG_SR]|=2;
	else			ctx->regs[BJX2_REG_SR]&=~2;
}

void BJX2_Op_PCMPGTL_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;
	s32 sa0, sb0, sc0;
	s32 sa1, sb1, sc1;

	va=ctx->regs[op->rn];
	vb=ctx->regs[op->rm];

	sa0=(s32)(va);	sa1=(s32)(va>>32);
	sb0=(s32)(vb);	sb1=(s32)(vb>>32);

	if(sa0>sb0)		ctx->regs[BJX2_REG_SR]|=1;
	else			ctx->regs[BJX2_REG_SR]&=~1;
	if(sa1>sb1)		ctx->regs[BJX2_REG_SR]|=2;
	else			ctx->regs[BJX2_REG_SR]&=~2;
}

void BJX2_Op_PCMPHIW_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;
	u16 sa0, sb0, sc0;
	u16 sa1, sb1, sc1;
	u16 sa2, sb2, sc2;
	u16 sa3, sb3, sc3;
	va=ctx->regs[op->rn];	vb=ctx->regs[op->rm];
	sa0=(u16)(va);	sa1=(u16)(va>>16);	sa2=(u16)(va>>32);	sa3=(u16)(va>>48);
	sb0=(u16)(vb);	sb1=(u16)(vb>>16);	sb2=(u16)(vb>>32);	sb3=(u16)(vb>>48);

	if(sa0>sb0)		ctx->regs[BJX2_REG_SR]|= 16;
	else			ctx->regs[BJX2_REG_SR]&=~16;
	if(sa1>sb1)		ctx->regs[BJX2_REG_SR]|= 32;
	else			ctx->regs[BJX2_REG_SR]&=~32;
	if(sa2>sb2)		ctx->regs[BJX2_REG_SR]|= 64;
	else			ctx->regs[BJX2_REG_SR]&=~64;
	if(sa3>sb3)		ctx->regs[BJX2_REG_SR]|= 128;
	else			ctx->regs[BJX2_REG_SR]&=~128;
}

void BJX2_Op_PCMPGTW_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;
	int sa0, sb0, sc0;
	int sa1, sb1, sc1;
	int sa2, sb2, sc2;
	int sa3, sb3, sc3;
	va=ctx->regs[op->rn];	vb=ctx->regs[op->rm];
	sa0=(s16)(va);	sa1=(s16)(va>>16);	sa2=(s16)(va>>32);	sa3=(s16)(va>>48);
	sb0=(s16)(vb);	sb1=(s16)(vb>>16);	sb2=(s16)(vb>>32);	sb3=(s16)(vb>>48);

	if(sa0>sb0)		ctx->regs[BJX2_REG_SR]|= 16;
	else			ctx->regs[BJX2_REG_SR]&=~16;
	if(sa1>sb1)		ctx->regs[BJX2_REG_SR]|= 32;
	else			ctx->regs[BJX2_REG_SR]&=~32;
	if(sa2>sb2)		ctx->regs[BJX2_REG_SR]|= 64;
	else			ctx->regs[BJX2_REG_SR]&=~64;
	if(sa3>sb3)		ctx->regs[BJX2_REG_SR]|= 128;
	else			ctx->regs[BJX2_REG_SR]&=~128;
}



void BJX2_Op_PCMPGTF_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4];
	u64	vs, vt;
	vs=ctx->regs[op->rn];	vt=ctx->regs[op->rm];
	jx2_upvec_f(tv0, vs);	jx2_upvec_f(tv1, vt);
	if(tv0[0]>tv1[0])	ctx->regs[BJX2_REG_SR]|= 1;
	else				ctx->regs[BJX2_REG_SR]&=~1;
	if(tv0[1]>tv1[1])	ctx->regs[BJX2_REG_SR]|= 2;
	else				ctx->regs[BJX2_REG_SR]&=~2;
}

void BJX2_Op_PCMPGTH_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4];
	u64	vs, vt;
	vs=ctx->regs[op->rn];	vt=ctx->regs[op->rm];
	jx2_upvec_hf(tv0, vs);	jx2_upvec_hf(tv1, vt);
	if(tv0[0]>tv1[0])	ctx->regs[BJX2_REG_SR]|=  16;
	else				ctx->regs[BJX2_REG_SR]&=~ 16;
	if(tv0[1]>tv1[1])	ctx->regs[BJX2_REG_SR]|=  32;
	else				ctx->regs[BJX2_REG_SR]&=~ 32;
	if(tv0[2]>tv1[2])	ctx->regs[BJX2_REG_SR]|=  64;
	else				ctx->regs[BJX2_REG_SR]&=~ 64;
	if(tv0[3]>tv1[3])	ctx->regs[BJX2_REG_SR]|= 128;
	else				ctx->regs[BJX2_REG_SR]&=~128;
}


void BJX2_Op_PSCHEQW_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;
	u16 sa0, sb0, sc0;
	u16 sa1, sb1, sc1;
	u16 sa2, sb2, sc2;
	u16 sa3, sb3, sc3;
	int nc;

	va=ctx->regs[op->ro];
	vb=ctx->regs[op->rm];
	sa0=(u16)(va);	sa1=(u16)(va>>16);	sa2=(u16)(va>>32);	sa3=(u16)(va>>48);
	sb0=(u16)(vb);	sb1=(u16)(vb>>16);	sb2=(u16)(vb>>32);	sb3=(u16)(vb>>48);

	if(sa0==sb0)		{ nc=0; }
	else if(sa1==sb1)	{ nc=1; }
	else if(sa2==sb2)	{ nc=2; }
	else if(sa3==sb3)	{ nc=3; }
	else				{ nc=4; }
	ctx->regs[op->rn]=nc;
	if(nc<4)	ctx->regs[BJX2_REG_SR]|= 1;
	else		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_PSCHNEW_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;
	u16 sa0, sb0, sc0;
	u16 sa1, sb1, sc1;
	u16 sa2, sb2, sc2;
	u16 sa3, sb3, sc3;
	int nc;

	va=ctx->regs[op->ro];
	vb=ctx->regs[op->rm];
	sa0=(u16)(va);	sa1=(u16)(va>>16);	sa2=(u16)(va>>32);	sa3=(u16)(va>>48);
	sb0=(u16)(vb);	sb1=(u16)(vb>>16);	sb2=(u16)(vb>>32);	sb3=(u16)(vb>>48);

	if(sa0!=sb0)		{ nc=0; }
	else if(sa1!=sb1)	{ nc=1; }
	else if(sa2!=sb2)	{ nc=2; }
	else if(sa3!=sb3)	{ nc=3; }
	else				{ nc=4; }
	ctx->regs[op->rn]=nc;
	if(nc<4)	ctx->regs[BJX2_REG_SR]|= 1;
	else		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_PSCHEQB_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;
	u16 sa0, sb0, sc0;
	u16 sa1, sb1, sc1;
	u16 sa2, sb2, sc2;
	u16 sa3, sb3, sc3;
	u16 sa4, sb4, sc4;
	u16 sa5, sb5, sc5;
	u16 sa6, sb6, sc6;
	u16 sa7, sb7, sc7;

	int nc;

	va=ctx->regs[op->ro];
	vb=ctx->regs[op->rm];
	sa0=(byte)(va>> 0);	sa1=(byte)(va>> 8);
	sa2=(byte)(va>>16);	sa3=(byte)(va>>24);
	sb0=(byte)(vb>> 0);	sb1=(byte)(vb>> 8);
	sb2=(byte)(vb>>16);	sb3=(byte)(vb>>24);
	sa4=(byte)(va>>32);	sa5=(byte)(va>>40);
	sa6=(byte)(va>>48);	sa7=(byte)(va>>56);
	sb4=(byte)(vb>>32);	sb5=(byte)(vb>>40);
	sb6=(byte)(vb>>48);	sb7=(byte)(vb>>56);

	if(sa0==sb0)		{ nc=0; }
	else if(sa1==sb1)	{ nc=1; }
	else if(sa2==sb2)	{ nc=2; }
	else if(sa3==sb3)	{ nc=3; }
	else if(sa4==sb4)	{ nc=4; }
	else if(sa5==sb5)	{ nc=5; }
	else if(sa6==sb6)	{ nc=6; }
	else if(sa7==sb7)	{ nc=7; }
	else				{ nc=8; }

	ctx->regs[op->rn]=nc;
	if(nc<8)	ctx->regs[BJX2_REG_SR]|= 1;
	else		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_PSCHNEB_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;
	u16 sa0, sb0, sc0;
	u16 sa1, sb1, sc1;
	u16 sa2, sb2, sc2;
	u16 sa3, sb3, sc3;
	u16 sa4, sb4, sc4;
	u16 sa5, sb5, sc5;
	u16 sa6, sb6, sc6;
	u16 sa7, sb7, sc7;

	int nc;

	va=ctx->regs[op->ro];
	vb=ctx->regs[op->rm];
	sa0=(byte)(va>> 0);	sa1=(byte)(va>> 8);
	sa2=(byte)(va>>16);	sa3=(byte)(va>>24);
	sb0=(byte)(vb>> 0);	sb1=(byte)(vb>> 8);
	sb2=(byte)(vb>>16);	sb3=(byte)(vb>>24);
	sa4=(byte)(va>>32);	sa5=(byte)(va>>40);
	sa6=(byte)(va>>48);	sa7=(byte)(va>>56);
	sb4=(byte)(vb>>32);	sb5=(byte)(vb>>40);
	sb6=(byte)(vb>>48);	sb7=(byte)(vb>>56);

	if(sa0!=sb0)		{ nc=0; }
	else if(sa1!=sb1)	{ nc=1; }
	else if(sa2!=sb2)	{ nc=2; }
	else if(sa3!=sb3)	{ nc=3; }
	else if(sa4!=sb4)	{ nc=4; }
	else if(sa5!=sb5)	{ nc=5; }
	else if(sa6!=sb6)	{ nc=6; }
	else if(sa7!=sb7)	{ nc=7; }
	else				{ nc=8; }

	ctx->regs[op->rn]=nc;
	if(nc<8)	ctx->regs[BJX2_REG_SR]|= 1;
	else		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_BLKUTX1_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int cr, cg, cb, cy;
	int mr, mg, mb;
	int nr, ng, nb;
	u64 va, vb, vc, clra, clrb;
	int ix, px;

	va=ctx->regs[op->rm];
	vb=ctx->regs[op->ro];
	
	cr=(va>> 8)&15;	cr=(cr<<4)|cr;
	cg=(va>> 4)&15;	cg=(cg<<4)|cg;
	cb=(va>> 0)&15;	cb=(cb<<4)|cb;
	cy=(va>>12)&15;	cy=(cy<<4)|cy;
	
	mr=cr-(cy>>1);	nr=mr+cy;
	mg=cg-(cy>>1);	ng=mg+cy;
	mb=cb-(cy>>1);	nb=mb+cy;
	
	mr=(mr<<8)|mr;	mg=(mg<<8)|mg;	mb=(mb<<8)|mb;
	nr=(nr<<8)|nr;	ng=(ng<<8)|ng;	nb=(nb<<8)|nb;

	clra=0xFFFF000000000000ULL|
		(((u64)nr)<<32)|
		(((u64)ng)<<16)|
		(((u64)nb)<< 0);
	clrb=0xFFFF000000000000ULL|
		(((u64)mr)<<32)|
		(((u64)mg)<<16)|
		(((u64)mb)<< 0);
	
	px=(va>>16)&65535;
	ix=vb&15;
	
	vc=((px>>ix)&1)?clra:clrb;
	ctx->regs[op->rn]=vc;
}


u64 tkra_pmuluhw(u64 a, u64 b)
{
	int ax, ay, az, aw;
	int bx, by, bz, bw;
	int cx, cy, cz, cw;
	u64 c;
	
	ax=(u16)(a>> 0);	ay=(u16)(a>>16);
	az=(u16)(a>>32);	aw=(u16)(a>>48);
	bx=(u16)(b>> 0);	by=(u16)(b>>16);
	bz=(u16)(b>>32);	bw=(u16)(b>>48);
	cx=(ax*bx)>>16;		cy=(ay*by)>>16;
	cz=(az*bz)>>16;		cw=(aw*bw)>>16;
	cx=(u16)cx;			cy=(u16)cy;
	cz=(u16)cz;			cw=(u16)cw;
	c=	(((u64)cx)<< 0) | (((u64)cy)<<16) |
		(((u64)cz)<<32) | (((u64)cw)<<48) ;
	return(c);
}

u64 tkra_rgbupck64(u16 a)
{
	int ax, ay, az, aw, av;
	u64 c;

//	av=(a&0x0001)|((a&0x0020)>>5)|((a&0x0400)>>10);
	av=(a&0x0001)|((a&0x0020)>>4)|((a&0x0400)>>8);
//	av=av<<13;
//	ax=(a&0x001F)<<11;
//	ay=(a&0x03E0)<< 6;
//	az=(a&0x7C00)<< 1;

	av=av<<5;
	ax=(a&0x001F)<<3;
	ay=(a&0x03E0)>>2;
	az=(a&0x7C00)>>7;
	ax|=(ax>>5);	ay|=(ay>>5);	az|=(az>>5);
	ax=ax|(ax<<8);	ay=ay|(ay<<8);
	az=az|(az<<8);	av=av|(av<<8);

	aw=(a&0x8000)?av:0xFFFF;
	c=	(((u64)ax)<< 0) | (((u64)ay)<<16) |
		(((u64)az)<<32) | (((u64)aw)<<48) ;
	return(c);
	
}

u64 tkra_rgb32upck64(u32 a)
{
	u64 b;
	int cr, cg, cb, ca;
	
	ca=(a>>24)&255;
	cr=(a>>16)&255;
	cg=(a>> 8)&255;
	cb=(a>> 0)&255;
	
	ca|=(ca<<8);
	cr|=(cr<<8);
	cg|=(cg<<8);
	cb|=(cb<<8);
	
	b=(((u64)ca)<<48)|(((u64)cr)<<32)|(((u64)cg)<<16)|cb;
	return(b);
}

u16 bjx2_ldm8uh(byte v);

u64 tkra_rgb32fupck64(u32 a)
{
	u64 b;
	int cr, cg, cb, ca;
	
	ca=(a>>24)&255;
	cr=(a>>16)&255;
	cg=(a>> 8)&255;
	cb=(a>> 0)&255;
	
	ca=bjx2_ldm8uh(ca);
	cr=bjx2_ldm8uh(cr);
	cg=bjx2_ldm8uh(cg);
	cb=bjx2_ldm8uh(cb);
	
	b=(((u64)ca)<<48)|(((u64)cr)<<32)|(((u64)cg)<<16)|cb;
	return(b);
}

int TKRA_GetPixel444A3_Alpha(int a)
{
	int av;
	av=(a&0x0001)|((a&0x0020)>>4)|((a&0x0400)>>8);
	av=(av<<5);
	return(av);
}

u64 	tkra_utx2_cachedblka[64];
u64		tkra_utx2_cachedpels[64][16];

u64 	tkra_utx3_cachedblka[64];
u64 	tkra_utx3_cachedblkb[64];
u64		tkra_utx3_cachedpelsl[64][16];
u64		tkra_utx3_cachedpelsh[64][16];

byte	tkra_utx2_tab511_a[32] = {
	0x00,	0x02,	0x05,	0x08,
	0x0B,	0x0D,	0x10,	0x13,
	0x16,	0x18,	0x1B,	0x1E,
	0x21,	0x23,	0x26,	0x29,
	0x2C,	0x2E,	0x31,	0x34,
	0x37,	0x39,	0x3C,	0x3F,
	0x42,	0x44,	0x47,	0x4A,
	0x4D,	0x4F,	0x52,	0x55
};

byte	tkra_utx2_tab511_b[32] = {
	0x00,	0x05,	0x0B,	0x10,
	0x16,	0x1B,	0x21,	0x26,
	0x2C,	0x31,	0x37,	0x3C,
	0x42,	0x47,	0x4D,	0x52,
	0x58,	0x5D,	0x63,	0x68,
	0x6E,	0x73,	0x79,	0x7E,
	0x84,	0x89,	0x8F,	0x94,
	0x9A,	0x9F,	0xA5,	0xAA
};

u64	TKRA_Utx2Blend511(u64 clra, u64 clrb)
{
	u64 clrc;
	int cra, crb, ca;
	int cga, cgb, cr;
	int cba, cbb, cg;
	int caa, cab, cb;
	
	caa=(clra>>(56+3))&31;			cab=(clrb>>(56+3))&31;
	cra=(clra>>(40+3))&31;			crb=(clrb>>(40+3))&31;
	cga=(clra>>(24+3))&31;			cgb=(clrb>>(24+3))&31;
	cba=(clra>>( 8+3))&31;			cbb=(clrb>>( 8+3))&31;
	caa=tkra_utx2_tab511_a[caa];	cab=tkra_utx2_tab511_b[cab];
	cra=tkra_utx2_tab511_a[cra];	crb=tkra_utx2_tab511_b[crb];
	cga=tkra_utx2_tab511_a[cga];	cgb=tkra_utx2_tab511_b[cgb];
	cba=tkra_utx2_tab511_a[cba];	cbb=tkra_utx2_tab511_b[cbb];
	ca=caa+cab;		cr=cra+crb;
	cg=cga+cgb;		cb=cba+cbb;
	ca=ca|(ca<<8);	cr=cr|(cr<<8);
	cg=cg|(cg<<8);	cb=cb|(cb<<8);
	clrc=	(((u64)ca)<<48) | (((u64)cr)<<32) |
			(((u64)cg)<<16) | (((u64)cb)<< 0) ;
	return(clrc);
}

u64 TKRA_CachedBlkUtx2(u64 blk, int ix)
{
	u64 tca[4];
	int pxa, pxb, pxv;
	int axa, axb, axc;
	u64 clra, clrb, clrc, clrd, clrp, clrq;
	
	int ix0, ix1, hxi;
	int i;

	ix0=ix&15;
	ix1=ix>>4;
//	hxi=((ix1*251)>>8)&255;
//	hxi=(ix1^(ix1>>6))&63;
	hxi=((((blk^(blk>>32))*65521)*251)>>8)&63;
	
	if((tkra_utx2_cachedblka[hxi]==blk))
	{
		return(tkra_utx2_cachedpels[hxi][ix0]);
	}

	pxa=(u16)(blk>> 0);
	pxb=(u16)(blk>>16);
	pxv=blk>>32;
	clra=tkra_rgbupck64(pxa);
	clrb=tkra_rgbupck64(pxb);

	clrc=TKRA_Utx2Blend511(clrb, clra);
	clrd=TKRA_Utx2Blend511(clra, clrb);

//	clrc=tkra_pmuluhw(clra, 0xAAAAAAAAAAAAAAAAULL);
//	clrd=tkra_pmuluhw(clrb, 0xAAAAAAAAAAAAAAAAULL);
//	clrp=tkra_pmuluhw(clra, 0x5555555555555555ULL);
//	clrq=tkra_pmuluhw(clrb, 0x5555555555555555ULL);
//	clrc+=clrq;
//	clrd+=clrp;

	tca[0]=clrb;
	tca[1]=clrd;
	tca[2]=clrc;
	tca[3]=clra;

	if((pxa&0x8000) && (pxb&0x8000))
	{
		for(i=0; i<16; i++)
		{
//			clrp=((pxv>>(i*2+1))&1)?clra:clrb;
			clrp=tca[(pxv>>(i*2))&3];
			tkra_utx2_cachedpels[hxi][i]=clrp;
		}
	}else
		if(pxb&0x8000)
	{
		axa=TKRA_GetPixel444A3_Alpha(pxa);
		axb=TKRA_GetPixel444A3_Alpha(pxb);
		axa=axa<<8;
		axb=axb<<8;

		for(i=0; i<16; i++)
		{
			clrp= ((pxv>>(i*2+1))&1)?clra:clrb;
			axc = ((pxv>>(i*2+0))&1)?axa:axb;
			clrp&=0x0000FFFFFFFFFFFFULL;
			clrp|=((u64)axc)<<48;
			tkra_utx2_cachedpels[hxi][i]=clrp;
		}
	}else
	{
		for(i=0; i<16; i++)
		{
//			clrp=((pxv>>(i*2+1))&1)?clra:clrb;
			clrp=tca[(pxv>>(i*2))&3];
			tkra_utx2_cachedpels[hxi][i]=clrp;
		}
	}

	tkra_utx2_cachedblka[hxi]=blk;
	return(tkra_utx2_cachedpels[hxi][ix0]);
}

u64 TKRA_CachedBlkUtx3L(u64 blka, u64 blkb, int ix)
{
	u64 tca[4], tcah[4];
	int pxa, pxb, pxvc, pxva;
	int axa, axb, axc;
	u64 clra, clrb, clrc, clrd, clrp, clrq;
	
	int ix0, ix1, hxi;
	int i;

	ix0=ix&15;
	ix1=ix>>4;
//	hxi=((ix1*251)>>8)&255;
//	hxi=(ix1^(ix1>>6))&63;
	hxi=((((blka^blkb^(blka>>32))*65521)*251)>>8)&63;
	
	if(	(tkra_utx3_cachedblka[hxi]==blka) &&
		(tkra_utx3_cachedblkb[hxi]==blkb))
	{
		return(tkra_utx3_cachedpelsl[hxi][ix0]);
	}

	pxa=(u32)(blka>> 0);
	pxb=(u32)(blka>>32);
	pxvc=blkb>> 0;
	pxva=blkb>>32;

	clra=tkra_rgb32upck64(pxa);
	clrb=tkra_rgb32upck64(pxb);

	clrc=TKRA_Utx2Blend511(clrb, clra);
	clrd=TKRA_Utx2Blend511(clra, clrb);

	tca[0]=clrb;
	tca[1]=clrd;
	tca[2]=clrc;
	tca[3]=clra;

	clra=tkra_rgb32fupck64(pxa);
	clrb=tkra_rgb32fupck64(pxb);

	clrc=TKRA_Utx2Blend511(clrb, clra);
	clrd=TKRA_Utx2Blend511(clra, clrb);

	tcah[0]=clrb;
	tcah[1]=clrd;
	tcah[2]=clrc;
	tcah[3]=clra;

	for(i=0; i<16; i++)
	{
		clrp=tca[(pxvc>>(i*2))&3];
		clrq=tca[(pxva>>(i*2))&3];
		
		clrp=
			(clrp&0x0000FFFFFFFFFFFFULL) |
			(clra&0xFFFF000000000000ULL) ;
		tkra_utx3_cachedpelsl[hxi][i]=clrp;

		clrp=tcah[(pxvc>>(i*2))&3];
		clrq=tcah[(pxva>>(i*2))&3];
		
		clrp=
			(clrp&0x0000FFFFFFFFFFFFULL) |
			(clra&0xFFFF000000000000ULL) ;
		tkra_utx3_cachedpelsh[hxi][i]=clrp;
	}

	tkra_utx3_cachedblka[hxi]=blka;
	tkra_utx3_cachedblkb[hxi]=blkb;
	return(tkra_utx3_cachedpelsl[hxi][ix0]);
}

u64 TKRA_CachedBlkUtx3H(u64 blka, u64 blkb, int ix)
{
	u64 tca[4], tcah[4];
	int pxa, pxb, pxvc, pxva;
	int axa, axb, axc;
	u64 clra, clrb, clrc, clrd, clrp, clrq;
	
	int ix0, ix1, hxi;
	int i;

	ix0=ix&15;
	ix1=ix>>4;
//	hxi=((ix1*251)>>8)&255;
//	hxi=(ix1^(ix1>>6))&63;
	hxi=((((blka^blkb^(blka>>32))*65521)*251)>>8)&63;
	
	if(	(tkra_utx3_cachedblka[hxi]==blka) &&
		(tkra_utx3_cachedblkb[hxi]==blkb))
	{
		return(tkra_utx3_cachedpelsh[hxi][ix0]);
	}

	TKRA_CachedBlkUtx3L(blka, blkb, ix);

	return(tkra_utx3_cachedpelsh[hxi][ix0]);
}

void BJX2_Op_BLKUTX2_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int cr, cg, cb, cy;
	int mr, mg, mb;
	int nr, ng, nb;
	u64 va, vb, vc, clra, clrb;
	int ix, px, cxa, cxb;

	va=ctx->regs[op->rm];
	vb=ctx->regs[op->ro];

	vc=TKRA_CachedBlkUtx2(va, vb);

#if 0
//	printf("BLKUTX2 %016llX\n", va);
	
	cxa=(va    )&65535;
	cxb=(va>>16)&65535;

	nr=(cxa>>10)&31; nr=(nr<<3)|(nr>>2);
	ng=(cxa>> 5)&31; ng=(ng<<3)|(ng>>2);
	nb=(cxa>> 0)&31; nb=(nb<<3)|(nb>>2);

	mr=(cxb>>10)&31; mr=(mr<<3)|(mr>>2);
	mg=(cxb>> 5)&31; mg=(mg<<3)|(mg>>2);
	mb=(cxb>> 0)&31; mb=(mb<<3)|(mb>>2);

//	clra=0xFFFF000000000000ULL|
//		(((u64)nr)<<32)|
//		(((u64)ng)<<16)|
//		(((u64)nb)<< 0);
//	clrb=0xFFFF000000000000ULL|
//		(((u64)mr)<<32)|
//		(((u64)mg)<<16)|
//		(((u64)mb)<< 0);

	clra=0xFFFF000000000000ULL|
		(((u64)nr)<<40)|
		(((u64)ng)<<24)|
		(((u64)nb)<< 8);
	clrb=0xFFFF000000000000ULL|
		(((u64)mr)<<40)|
		(((u64)mg)<<24)|
		(((u64)mb)<< 8);
	
	px=(u32)(va>>32);
	ix=vb&15;

	vc=((px>>(ix*2+1))&1)?clra:clrb;
	
//	vc=0xFFFF0000FFFF0000ULL;
//	vc=(vc>>1)|0x8000000080000000ULL;
//	vc=vc|0x8000000080000000ULL;
#endif

	ctx->regs[op->rn]=vc;
}


u32 BJX2_PMORT_U16(u16 x);

void BJX2_Op_LDTEX_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 vst, vrb, txb, txc, vc;
	u32 txs, txt, txi, txm;

	vrb=ctx->regs[op->rm];
	vst=ctx->regs[op->ro];
	txs=(vst>>16)&0xFFFF;
	txt=(vst>>48)&0xFFFF;

	if(vrb&7)
	{
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_MISAL);
		return;
	}

	if(op->imm&4)
	{
		if(op->imm&1)
			txs++;
		if(op->imm&2)
			txt++;
	}else
	{
		if(op->imm&3)
		{
			txs+=(vst>>15)&1;
			txt+=(vst>>47)&1;
		}
	}

	txi=	(BJX2_PMORT_U16(txt)<<1)|
			(BJX2_PMORT_U16(txs)   );
	
	txm=((1<<((vrb>>52)&31))-1);
	txm|=15;
	
	txi=txi&txm;

	ctx->trapc=op->pc;
	

	if(((vrb>>57)&7)==0)
	{
		txb=BJX2_MemGetQWord(ctx,
			(bjx2_addr)vrb+((bjx2_addr)(txi>>4)*8));
		vc=TKRA_CachedBlkUtx2(txb, txi);
	}else
		if(((vrb>>57)&7)==5)
	{
		txb=BJX2_MemGetWord(ctx,
			(bjx2_addr)vrb+((bjx2_addr)(txi>>0)*2));
		vc=tkra_rgbupck64(txb);
	}else
		if(((vrb>>57)&7)==2)
	{
//		txb=BJX2_MemGetQWord(ctx,
//			(bjx2_addr)vrb+((bjx2_addr)(txi>>4)*16+0));
//		txc=BJX2_MemGetQWord(ctx,
//			(bjx2_addr)vrb+((bjx2_addr)(txi>>4)*16+8));
		BJX2_MemGetXWord(ctx,
			(bjx2_addr)vrb+((bjx2_addr)(txi>>4)*16+0),
			&txb, &txc);
		vc=TKRA_CachedBlkUtx3L(txb, txc, txi);
	}else
		if(((vrb>>57)&7)==3)
	{
//		txb=BJX2_MemGetQWord(ctx,
//			(bjx2_addr)vrb+((bjx2_addr)(txi>>4)*16+0));
//		txc=BJX2_MemGetQWord(ctx,
//			(bjx2_addr)vrb+((bjx2_addr)(txi>>4)*16+8));
		BJX2_MemGetXWord(ctx,
			(bjx2_addr)vrb+((bjx2_addr)(txi>>4)*16+0),
			&txb, &txc);

		vc=TKRA_CachedBlkUtx3H(txb, txc, txi);
	}else
		if(((vrb>>57)&7)==6)
	{
		txb=BJX2_MemGetDWord(ctx,
			(bjx2_addr)vrb+((bjx2_addr)(txi>>0)*4));
		vc=tkra_rgb32upck64(txb);
	}else
		if(((vrb>>57)&7)==7)
	{
		txb=BJX2_MemGetDWord(ctx,
			(bjx2_addr)vrb+((bjx2_addr)(txi>>0)*4));
		vc=tkra_rgb32fupck64(txb);
	}else
		if(((vrb>>57)&7)==1)
	{
		vc=BJX2_MemGetQWord(ctx,
			(bjx2_addr)vrb+((bjx2_addr)(txi>>0)*8));
	}else
	{
		vc=0;
	}

	ctx->regs[op->rn]=vc;

//	BJX2_DbgAddrAccessTrap(ctx,
//		(bjx2_addr)(ctx->regs[op->rm]),
//		(bjx2_addr)(ctx->regs[op->rm])+((ctx->regs[op->ro])*8), 8);
//	ctx->regs[op->rn]=BJX2_MemGetQWord(ctx,
//		(bjx2_addr)(ctx->regs[op->rm])+((bjx2_addr)(ctx->regs[op->ro])*8));
}

void BJX2_Op_BLKUTX3H_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va0, va1, vb, vc;

	va0=ctx->regs[(op->rm)+0];
	va1=ctx->regs[(op->rm)+1];
	vb=ctx->regs[op->ro];

	vc=TKRA_CachedBlkUtx3H(va0, va1, vb);
	ctx->regs[op->rn]=vc;
}

void BJX2_Op_BLKUTX3L_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va0, va1, vb, vc;

	va0=ctx->regs[(op->rm)+0];
	va1=ctx->regs[(op->rm)+1];
	vb=ctx->regs[op->ro];

	vc=TKRA_CachedBlkUtx3L(va0, va1, vb);
	ctx->regs[op->rn]=vc;
}


int bjx2_sblk0_dec8_12(int v)
{
	int e, v0, v1;
	
	if(v&0x70)
	{
		e=((v>>4)&7)-1;
		v0=(v&15)|16;
		v1=v0<<e;
	}else
	{
		e=0;
		v0=v&15;
		v1=v0<<e;
	}
	if(v&0x80)
		v1=-v1;
	return(v1);
}

int bjx2_uab1_dec(u32 *samp, u64 blk)
{
	int avga, avgb, avge, avgf, avgl, avgp, p, phi, plo, bits;
	int svl, svr;
	int i, j, k;
	
	avga=(blk<<2)&0xFC;
	avgb=(blk>>4)&0xFC;
	avge=(blk>>9)&0x78;

	bits=blk>>16;

	avga=bjx2_sblk0_dec8_12(avga);
	avgb=bjx2_sblk0_dec8_12(avgb);
	avge=bjx2_sblk0_dec8_12(avge);

	for(i=0; i<16; i++)
	{
		j=bits>>i;
		avgp=(((31-2*i)*avga)+((2*i+1)*avgb))/32;
		phi=avgp+avge;
		plo=avgp-avge;
		k=(j&1)?phi:plo;
		svl=k<<4;	svr=k<<4;
		samp[i]=(svl&0x0000FFFFU)|((svr<<16)&0xFFFF0000U);
	}
	return(0);
}

int bjx2_uab2_dec(u32 *samp, u64 blk)
{
	int avga, avgb, avge, avgf, avgl, avgp, p, phi, plo, bits;
	int svl, svr;
	int i, j, k;
	
	avga=(blk>> 0)&0xFF;
	avgb=(blk>> 8)&0xFF;
	avge=(blk>>16)&0xFF;
	avgl=(blk>>16)&0xFF;

	bits=blk>>32;

	avga=bjx2_sblk0_dec8_12(avga);
	avgb=bjx2_sblk0_dec8_12(avgb);
	avge=bjx2_sblk0_dec8_12(avge);
	avgl=bjx2_sblk0_dec8_12(avgl);

	avgf=((avge*85)>>8);
	
	for(i=0; i<16; i++)
	{
		j=bits>>(i*2);
	
		avgp=(((31-2*i)*avga)+((2*i+1)*avgb))/32;
		
		if(j&1)
		{
			phi=avgp+avge;
			plo=avgp-avge;
		}else
		{
			phi=avgp+avgf;
			plo=avgp-avgf;
		}

		k=(j&2)?phi:plo;

//		if(k> 32767)k= 32767;
//		if(k<-32767)k=-32767;

		svl=(k+avgl)<<4;
		svr=(k-avgl)<<4;

		samp[i]=(svl&0x0000FFFFU)|((svr<<16)&0xFFFF0000U);
	}
	return(0);
}

u32 	bjx2_uab1_cachedblka[16];
u32		bjx2_uab1_cachedpels[16][16];

u64 	bjx2_uab2_cachedblka[16];
u32		bjx2_uab2_cachedpels[16][16];

void BJX2_Op_BLKUAB1_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;
	int hxi, ix0, cxa, cxb;

	va=ctx->regs[op->rm];
	vb=ctx->regs[op->ro];

	ix0=vb&15;
	hxi=((((va^(va>>32))*65521)*251)>>24)&15;
	if(bjx2_uab1_cachedblka[hxi]!=va)
	{
		bjx2_uab1_dec(bjx2_uab1_cachedpels[hxi], va);
		bjx2_uab1_cachedblka[hxi]=va;
	}

	vc=bjx2_uab1_cachedpels[hxi][ix0];
	ctx->regs[op->rn]=vc;
}

void BJX2_Op_BLKUAB2_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;
	int hxi, ix0, cxa, cxb;

	va=ctx->regs[op->rm];
	vb=ctx->regs[op->ro];

	ix0=vb&15;
	hxi=((((va^(va>>32))*65521)*251)>>24)&15;
	if(bjx2_uab2_cachedblka[hxi]!=va)
	{
		bjx2_uab2_dec(bjx2_uab2_cachedpels[hxi], va);
		bjx2_uab2_cachedblka[hxi]=va;
	}

	vc=bjx2_uab2_cachedpels[hxi][ix0];
	ctx->regs[op->rn]=vc;
}

void BJX2_Op_BLINT_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 pxa, pxb, pxc, pxd, frsp, frtp;
	u64 clrp, clrq, clrr;
	u64 frsn, frtn;
	int ix, px, cxa, cxb;

	pxa=ctx->regs[op->rm+0];
	pxc=ctx->regs[op->rm+1];
	pxb=ctx->regs[op->ro+0];
	pxd=ctx->regs[op->ro+1];
	
	frsp=ctx->regs[op->rn+0];
	frtp=ctx->regs[op->rn+1];
	frsn=~frsp;
	frtn=~frtp;

	clrp=tkra_pmuluhw(pxa, frsn) + tkra_pmuluhw(pxb, frsp);
	clrq=tkra_pmuluhw(pxc, frsn) + tkra_pmuluhw(pxd, frsp);
	clrr=tkra_pmuluhw(clrp, frtn) + tkra_pmuluhw(clrq, frtp);
	ctx->regs[op->rn]=clrr;
}

void BJX2_Op_BLINTA_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	BJX2_Opcode *opa, *opb;
	u64 pxa, pxb, pxc, pxd, frsp, frtp;
	u64 clrp, clrq, clrr;
	u64 frsn, frtn;
	int ix, px, cxa, cxb;

	opa=op;
	opb=op->data;

	if(!opb)
	{
		ctx->regs[op->rn]=0;
		return;
	}

	pxa=ctx->regs[opa->rm];
	pxc=ctx->regs[opb->rm];
	pxb=ctx->regs[opa->ro];
	pxd=ctx->regs[opb->ro];
	
	frsp=ctx->regs[opa->rn];
	frtp=ctx->regs[opb->rn];
	frsn=~frsp;
	frtn=~frtp;

	clrp=tkra_pmuluhw(pxa, frsn) + tkra_pmuluhw(pxb, frsp);
	clrq=tkra_pmuluhw(pxc, frsn) + tkra_pmuluhw(pxd, frsp);
	clrr=tkra_pmuluhw(clrp, frtn) + tkra_pmuluhw(clrq, frtp);
	ctx->regs[op->rn]=clrr;
}

void BJX2_Op_BLERP_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 pxa, pxb, pxc, pxd, frsp, frtp;
	u64 clrp, clrq, clrr;
	u64 frsn, frtn;
	int ix, px, cxa, cxb;

	pxa=ctx->regs[op->rm];
	pxb=ctx->regs[op->ro];
	
	frsp=ctx->regs[op->rn];
	frsn=~frsp;
	clrp=tkra_pmuluhw(pxa, frsn) + tkra_pmuluhw(pxb, frsp);
	ctx->regs[op->rn]=clrp;
}

void BJX2_Op_PLDCH_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u32 v0, v1;
	u64	vs, vt, vn;

	vs=ctx->regs[op->rm];
	
	v0=BJX2_CvtHalfToFloat(vs>> 0);
	v1=BJX2_CvtHalfToFloat(vs>>16);
	
//	jx2_upvec_hf(tv0, vs);
//	BJX2_PtrSetFloat(&v0, tv0[0]);
//	BJX2_PtrSetFloat(&v1, tv0[1]);
	
	vn=(((u64)v1)<<32)|v0;
	ctx->regs[op->rn]=vn;
	
//	vn=jx2_mkvec_hf(tv2[0], tv2[1], tv2[2], tv2[3]);
//	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PLDCHH_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u32 v0, v1;
	u64	vs, vt, vn;

	vs=ctx->regs[op->rm];

	v0=BJX2_CvtHalfToFloat(vs>> 0);
	v1=BJX2_CvtHalfToFloat(vs>>16);

//	jx2_upvec_hf(tv0, vs);
//	BJX2_PtrSetFloat(&v0, tv0[2]);
//	BJX2_PtrSetFloat(&v1, tv0[3]);
	
	vn=(((u64)v1)<<32)|v0;
	ctx->regs[op->rn]=vn;
	
//	vn=jx2_mkvec_hf(tv2[0], tv2[1], tv2[2], tv2[3]);
//	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PLDCXH_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u32 v0, v1, v2, v3;
	u64	vs, vt, vn0, vn1;

	vs=ctx->regs[op->rm];
	
	v0=BJX2_CvtHalfToFloat(vs>> 0);
	v1=BJX2_CvtHalfToFloat(vs>>16);
	v2=BJX2_CvtHalfToFloat(vs>>32);
	v3=BJX2_CvtHalfToFloat(vs>>48);
	
	vn0=(((u64)v1)<<32)|v0;
	vn1=(((u64)v3)<<32)|v2;

	ctx->regs[op->rn+0]=vn0;
	ctx->regs[op->rn+1]=vn1;
}

void BJX2_Op_PLDCH_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u32 v0, v1;
	u64	vs, vt, vn;

	vs=op->imm;

//	jx2_upvec_hf(tv0, vs);
//	BJX2_PtrSetFloat(&v0, tv0[0]);
//	BJX2_PtrSetFloat(&v1, tv0[1]);

	v0=BJX2_CvtHalfToFloat(vs>> 0);
	v1=BJX2_CvtHalfToFloat(vs>>16);

	vn=(((u64)v1)<<32)|v0;
	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PLDCXH_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u32 v0, v1, v2, v3;
	u64	vs, vt, vn0, vn1;

	vs=op->imm;

	v0=BJX2_CvtHalfToFloat(vs>> 0);
	v1=BJX2_CvtHalfToFloat(vs>>16);
	v2=BJX2_CvtHalfToFloat(vs>>32);
	v3=BJX2_CvtHalfToFloat(vs>>48);

	vn0=(((u64)v1)<<32)|v0;
	vn1=(((u64)v3)<<32)|v2;
	ctx->regs[op->rn+0]=vn0;
	ctx->regs[op->rn+1]=vn1;
}

void BJX2_Op_PLDCEHL_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u32 v0, v1;
	u64	vs, vt, vn;

	vs=ctx->regs[op->rm];

//	jx2_upvec_hf(tv0, vs);
//	BJX2_PtrSetFloat(&v0, tv0[0]);
//	BJX2_PtrSetFloat(&v1, tv0[1]);

	v0=BJX2_CvtHalfToFloat(vs>> 0);
	v1=BJX2_CvtHalfToFloat(vs>>16);

	v0|=((vs>>(48+0))&0x1F)<<8;
	v1|=((vs>>(48+5))&0x1F)<<8;
	
	vn=(((u64)v1)<<32)|v0;
	ctx->regs[op->rn]=vn;
	
//	vn=jx2_mkvec_hf(tv2[0], tv2[1], tv2[2], tv2[3]);
//	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PLDCEHH_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u32 v0, v1;
	u64	vs, vt, vn;

	vs=ctx->regs[op->rm];

//	jx2_upvec_hf(tv0, vs);
//	BJX2_PtrSetFloat(&v0, tv0[2]);
//	BJX2_PtrSetFloat(&v1, tv0[3]);

	v0=BJX2_CvtHalfToFloat(vs>> 0);
	v1=BJX2_CvtHalfToFloat(vs>>16);

	v0|=((vs>>(48+10))&0x1F)<<8;
//	v1|=((vs>>(48+5))&0x1F)<<8;
	v1=0;
	
	vn=(((u64)v1)<<32)|v0;
	ctx->regs[op->rn]=vn;
	
//	vn=jx2_mkvec_hf(tv2[0], tv2[1], tv2[2], tv2[3]);
//	ctx->regs[op->rn]=vn;
}

#if 0
void BJX2_Op_PLDCHX_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u32 v0, v1, v2, v3;
	u64	vs, vt, vn0, vn1;

	vs=ctx->regs[op->rm];
	jx2_upvec_hf(tv0, vs);
	BJX2_PtrSetFloat(&v0, tv0[0]);
	BJX2_PtrSetFloat(&v1, tv0[1]);
	BJX2_PtrSetFloat(&v2, tv0[2]);
	BJX2_PtrSetFloat(&v3, tv0[3]);
	
	vn0=(((u64)v1)<<32)|v0;
	vn1=(((u64)v3)<<32)|v2;
	ctx->regs[op->rn+0]=vn0;
	ctx->regs[op->rn+1]=vn1;
	
//	vn=jx2_mkvec_hf(tv2[0], tv2[1], tv2[2], tv2[3]);
//	ctx->regs[op->rn]=vn;
}
#endif

void BJX2_Op_PSTCH_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u32 v0, v1;
	u64	vs, vt, vn;

	vs=ctx->regs[op->rm];
//	jx2_upvec_hf(tv0, vs);
	
//	BJX2_PtrSetFloat(&v0, tv0[0]);
//	BJX2_PtrSetFloat(&v1, tv0[1]);
	
//	vn=(((u64)v1)<<32)|v0;
//	ctx->regs[op->rn]=vn;
	
	v0=vs;
	v1=vs>>32;
	tv2[0]=BJX2_PtrGetFloat(&v0);
	tv2[1]=BJX2_PtrGetFloat(&v1);
	tv2[2]=0;
	tv2[3]=0;
	
	vn=jx2_mkvec_hf(tv2[0], tv2[1], tv2[2], tv2[3]);
	ctx->regs[op->rn]=vn;
}

// void BJX2_Op_PSTCHX_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
void BJX2_Op_PSTCXH_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float tv0[4], tv1[4], tv2[4];
	u32 v0, v1, v2, v3;
	u64	vs, vt, vn;

	vs=ctx->regs[op->rm+0];
	vt=ctx->regs[op->rm+1];
//	jx2_upvec_hf(tv0, vs);
	
//	BJX2_PtrSetFloat(&v0, tv0[0]);
//	BJX2_PtrSetFloat(&v1, tv0[1]);
	
//	vn=(((u64)v1)<<32)|v0;
//	ctx->regs[op->rn]=vn;
	
	v0=vs;
	v1=vs>>32;
	v2=vt;
	v3=vt>>32;
	tv2[0]=BJX2_PtrGetFloat(&v0);
	tv2[1]=BJX2_PtrGetFloat(&v1);
	tv2[2]=BJX2_PtrGetFloat(&v2);
	tv2[3]=BJX2_PtrGetFloat(&v3);
	
	vn=jx2_mkvec_hf(tv2[0], tv2[1], tv2[2], tv2[3]);
	ctx->regs[op->rn]=vn;
}


u16 bjx2_ldm8uh(byte v)
{
	u16 v1;
	
	if(!v)
		return(0);

//	v1=(v<<6)^0x4000;
	v1=(v<<6)|((v<<7)&0x4000);
	v1^=0x2000;
	
//	v1=(v<<6)|(((~v)<<7)&0x4000);
	return(v1);
}

u16 bjx2_ldm8sh(byte v)
{
	u16 v1;
//	v1= (((v )<<7)&0x3F80)|
//		(((~v)<<8)&0x4000)|
//		((( v)<<8)&0x8000);

//	v1= (((v )<< 6)&0x3F80)|
//		(((~v)<< 7)&0x4000)|
//		((( v)<<15)&0x8000);
	v1= ((v<< 6)&0x3F80)|
		((v<< 7)&0x4000)|
		((v<<15)&0x8000);
	v1^=0x2000;

	return(v1);
}

byte bjx2_stm8uh(u16 v)
{
	u16 v1;
	v1=(v>>6)^0x80;
	if(((v>>13)&3)==0)
		v1=0x00;
	if(((v>>13)&3)==3)
		v1=0xF0;
	return(v1);
}

byte bjx2_stm8sh(u16 v)
{
	u16 v1;
//	v1=(v>>7)|((v>>8)&0x80);
	v1=((v>>6)^0x80)|((v>>15)&1);

	if(((v>>13)&3)==0)
		v1=0;
	if(((v>>13)&3)==3)
		v1=0xF0|((v>>15)&1);
	return(v1);
}

void BJX2_Op_PLDCM8UH_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u16 v0, v1, v2, v3;
	u64	vs, vn;
	vs=ctx->regs[op->rm];
	v0=bjx2_ldm8uh((vs    )&255);
	v1=bjx2_ldm8uh((vs>> 8)&255);
	v2=bjx2_ldm8uh((vs>>16)&255);
	v3=bjx2_ldm8uh((vs>>24)&255);
	vn=(((u64)v3)<<48)|(((u64)v2)<<32)|(((u64)v1)<<16)|v0;
	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PLDCM8SH_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u16 v0, v1, v2, v3;
	u64	vs, vn;
	vs=ctx->regs[op->rm];
	v0=bjx2_ldm8sh((vs    )&255);
	v1=bjx2_ldm8sh((vs>> 8)&255);
	v2=bjx2_ldm8sh((vs>>16)&255);
	v3=bjx2_ldm8sh((vs>>24)&255);
	vn=(((u64)v3)<<48)|(((u64)v2)<<32)|(((u64)v1)<<16)|v0;
	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PSTCM8UH_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u16 v0, v1, v2, v3;
	u64	vs, vn;
	vs=ctx->regs[op->rm];
	v0=bjx2_stm8uh((vs    )&65535);
	v1=bjx2_stm8uh((vs>>16)&65535);
	v2=bjx2_stm8uh((vs>>32)&65535);
	v3=bjx2_stm8uh((vs>>48)&65535);
	vn=(((u64)v3)<<24)|(((u64)v2)<<16)|(((u64)v1)<<8)|v0;
	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PSTCM8SH_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u16 v0, v1, v2, v3;
	u64	vs, vn;
	vs=ctx->regs[op->rm];
	v0=bjx2_stm8sh((vs    )&65535);
	v1=bjx2_stm8sh((vs>>16)&65535);
	v2=bjx2_stm8sh((vs>>32)&65535);
	v3=bjx2_stm8sh((vs>>48)&65535);
	vn=(((u64)v3)<<24)|(((u64)v2)<<16)|(((u64)v1)<<8)|v0;
	ctx->regs[op->rn]=vn;
}


void BJX2_Op_PLDCM8UH_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u16 v0, v1, v2, v3;
	u64	vs, vn;
	vs=op->imm;
	v0=bjx2_ldm8uh((vs    )&255);
	v1=bjx2_ldm8uh((vs>> 8)&255);
	v2=bjx2_ldm8uh((vs>>16)&255);
	v3=bjx2_ldm8uh((vs>>24)&255);
	vn=(((u64)v3)<<48)|(((u64)v2)<<32)|(((u64)v1)<<16)|v0;
	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PLDCM8SH_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u16 v0, v1, v2, v3;
	u64	vs, vn;
	vs=op->imm;
	v0=bjx2_ldm8sh((vs    )&255);
	v1=bjx2_ldm8sh((vs>> 8)&255);
	v2=bjx2_ldm8sh((vs>>16)&255);
	v3=bjx2_ldm8sh((vs>>24)&255);
	vn=(((u64)v3)<<48)|(((u64)v2)<<32)|(((u64)v1)<<16)|v0;
	ctx->regs[op->rn]=vn;
}


void BJX2_Op_BCDADC_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn, vna, vnc, sr;
	int i, j, k;

	vs=ctx->regs[op->rn];
	vt=ctx->regs[op->rm];
	sr=ctx->regs[BJX2_REG_SR];
	
	vn=0;
	for(i=0; i<16; i++)
	{
		j=((vs>>(i*4))&15)+((vt>>(i*4))&15);
		if(i==0)
//			j+=(sr>>1)&1;
			j+=sr&1;
		if(j>9)
			j+=6;
		vn|=((u64)j)<<(i*4);
	}
	
//	sr&=~(2ULL);
	sr&=~(1ULL);
	if(j>=10)
//		sr|=2;
		sr|=1;
	
	ctx->regs[op->rn]=vn;
	ctx->regs[BJX2_REG_SR]=sr;
}

void BJX2_Op_BCDSBB_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn, vna, vnc, sr;
	int i, j, k;

	vs=ctx->regs[op->rn];
	vt=ctx->regs[op->rm];
	sr=ctx->regs[BJX2_REG_SR];
	
	vt=0x9999999999999999ULL-vt;
	
	vn=0;
	for(i=0; i<16; i++)
	{
		j=((vs>>(i*4))&15)+((vt>>(i*4))&15);
		if(i==0)
//			j+=!((sr>>1)&1);
			j+=!(sr&1);
		if(j>9)
			j+=6;
		vn|=((u64)j)<<(i*4);
	}
	
//	sr&=~(2ULL);
	sr&=~(1ULL);
	if(!(j>=10))
//		sr|=2;
		sr|=1;
	
	ctx->regs[op->rn]=vn;
	ctx->regs[BJX2_REG_SR]=sr;
}


u16 bjx2_fsqrta_fp16(u16 val)
{
	return((val&0x8000)|((val&0x7FFF)>>1)+0x1E00);
}

u32 bjx2_fsqrta_fp32(u32 val)
{
	return((val&0x80000000U)|((val&0x7FFFFFFFU)>>1)+0x1FC00000U);
}

void BJX2_Op_PSQRTAH_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn;
	int i, j, k;

	vs=ctx->regs[op->rm];

	vn=				bjx2_fsqrta_fp16(vs>>48);
	vn=(vn<<16) |	bjx2_fsqrta_fp16(vs>>32);
	vn=(vn<<16) |	bjx2_fsqrta_fp16(vs>>16);
	vn=(vn<<16) |	bjx2_fsqrta_fp16(vs>> 0);

	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PSQRTUAH_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn;
	int i, j, k;

	vs=ctx->regs[op->rm];

	vn=				bjx2_fsqrta_fp16(vs>>48);
	vn=(vn<<16) |	bjx2_fsqrta_fp16(vs>>32);
	vn=(vn<<16) |	bjx2_fsqrta_fp16(vs>>16);
	vn=(vn<<16) |	bjx2_fsqrta_fp16(vs>> 0);

	if(	vn&  0x0000000000008000ULL)
		vn&=~0x000000000000FFFFULL;
	if(	vn&  0x0000000080000000ULL)
		vn&=~0x00000000FFFF0000ULL;
	if(	vn&  0x0000800000000000ULL)
		vn&=~0x0000FFFF00000000ULL;
	if(	vn&  0x8000000000000000ULL)
		vn&=~0xFFFF000000000000ULL;

	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PSQRTAF_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn;
	int i, j, k;

	vs=ctx->regs[op->rm];

	vn=				bjx2_fsqrta_fp32(vs>>32);
	vn=(vn<<32) |	bjx2_fsqrta_fp32(vs>>0);

	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PSQRTUAF_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn;
	int i, j, k;

	vs=ctx->regs[op->rm];

	vn=				bjx2_fsqrta_fp32(vs>>32);
	vn=(vn<<32) |	bjx2_fsqrta_fp32(vs>>0);

	if(	vn&  0x0000000080000000ULL)
		vn&=~0x00000000FFFFFFFFULL;
	if(	vn&  0x8000000000000000ULL)
		vn&=~0xFFFFFFFF00000000ULL;

	ctx->regs[op->rn]=vn;
}

u16 bjx2_frcpa_fp16(u16 val)
{
	return((~val)+0x7800);
}

u32 bjx2_frcpa_fp32(u32 val)
{
	return((~val)+0x7F000000U);
}

void BJX2_Op_PRCPAH_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn;
	int i, j, k;

	vs=ctx->regs[op->rm];

	vn=				bjx2_frcpa_fp16(vs>>48);
	vn=(vn<<16) |	bjx2_frcpa_fp16(vs>>32);
	vn=(vn<<16) |	bjx2_frcpa_fp16(vs>>16);
	vn=(vn<<16) |	bjx2_frcpa_fp16(vs>> 0);

	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PRCPAF_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn;
	int i, j, k;

	vs=ctx->regs[op->rm];

	vn=				bjx2_frcpa_fp32(vs>>32);
	vn=(vn<<32) |	bjx2_frcpa_fp32(vs>>0);

	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PRELUH_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn;
	int i, j, k;

//	vs=ctx->regs[op->rn];
	vs=ctx->regs[op->rm];

	vn=vs;
	if(	vn&  0x0000000000008000ULL)
		vn&=~0x000000000000FFFFULL;
	if(	vn&  0x0000000080000000ULL)
		vn&=~0x00000000FFFF0000ULL;
	if(	vn&  0x0000800000000000ULL)
		vn&=~0x0000FFFF00000000ULL;
	if(	vn&  0x8000000000000000ULL)
		vn&=~0xFFFF000000000000ULL;

	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PRELUF_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn;
	int i, j, k;

//	vs=ctx->regs[op->rn];
	vs=ctx->regs[op->rm];

	vn=vs;
	if(	vn&  0x0000000080000000ULL)
		vn&=~0x00000000FFFFFFFFULL;
	if(	vn&  0x8000000000000000ULL)
		vn&=~0xFFFFFFFF00000000ULL;

	ctx->regs[op->rn]=vn;
}

int bjx2_rgb5y_apx8(int vi)
{
	static byte exp_g4[16]={
		0x00, 0x01, 0x08, 0x09, 
		0x40, 0x41, 0x48, 0x49, 
		0x80, 0x81, 0x88, 0x89, 
		0xC0, 0xC1, 0xC8, 0xC9	};
	static byte exp_r2[4]={
		0x00, 0x04, 0x20, 0x24 };
	static byte exp_b2[4]={
		0x00, 0x02, 0x10, 0x12 };
	int y;
	
	y=	exp_g4[(vi>> 6)&15] |
		exp_r2[(vi>>13)& 3] |
		exp_b2[(vi>> 3)& 3] ;
	return(y);
}

void BJX2_Op_RGB5MINMAX_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u16 vi0, vi1, vi2, vi3;
	byte vy0, vy1, vy2, vy3;
	u16 vi4m, vi4n, vi5m, vi5n, vi6m, vi6n;
	byte vy4m, vy4n, vy5m, vy5n, vy6m, vy6n;
	u64	vs, vt, vn;
	int i, j, k;

//	vs=ctx->regs[op->rn];
	vs=ctx->regs[op->rm];

	vi0=(vs>> 0)&0xFFFF;
	vi1=(vs>>16)&0xFFFF;
	vi2=(vs>>32)&0xFFFF;
	vi3=(vs>>48)&0xFFFF;
	vy0=bjx2_rgb5y_apx8(vi0);
	vy1=bjx2_rgb5y_apx8(vi1);
	vy2=bjx2_rgb5y_apx8(vi2);
	vy3=bjx2_rgb5y_apx8(vi3);

	if(vy0>=vy1)
	{	vi4m=vi1; vy4m=vy1;
		vi4n=vi0; vy4n=vy0;	}
	else
	{	vi4m=vi0; vy4m=vy0;
		vi4n=vi1; vy4n=vy1;	}

	if(vy2>=vy3)
	{	vi5m=vi3; vy5m=vy3;
		vi5n=vi2; vy5n=vy2;	}
	else
	{	vi5m=vi2; vy5m=vy2;
		vi5n=vi3; vy5n=vy3;	}


	if(vy4m>=vy5m)
		{	vi6m=vi5m; vy6m=vy4m;	}
	else
		{	vi6m=vi4m; vy6m=vy5m;	}
	if(vy4n>vy5n)
		{	vi6n=vi4n; vy6m=vy5n;	}
	else
		{	vi6n=vi5n; vy6m=vy4n;	}

	vn=(vi6n) | (((u32)vi6m)<<16);

	ctx->regs[op->rn]=vn;
}

void BJX2_Op_RGB5CCENC_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vp, vn;
	u16 vi0, vi1, vi2, vi3;
	byte vy0, vy1, vy2, vy3;
	u16 cmin, cmax;
	byte ymin, ymax, ylo, yhi, ymi;
	int i, j, k;

	vs=ctx->regs[op->rm];
	vt=ctx->regs[op->ro];
	vp=ctx->regs[op->rn];
	
	cmin=(vt>>16)&0xFFFF;
	cmax=(vt>> 0)&0xFFFF;
	ymin=bjx2_rgb5y_apx8(cmin);
	ymax=bjx2_rgb5y_apx8(cmax);

	ymi=(ymin+ymax)>>1;
	ylo=(ymin+ymi )>>1;
	yhi=(ymax+ymi )>>1;

	vi0=(vs>> 0)&0xFFFF;
	vi1=(vs>>16)&0xFFFF;
	vi2=(vs>>32)&0xFFFF;
	vi3=(vs>>48)&0xFFFF;
	vy0=bjx2_rgb5y_apx8(vi0);
	vy1=bjx2_rgb5y_apx8(vi1);
	vy2=bjx2_rgb5y_apx8(vi2);
	vy3=bjx2_rgb5y_apx8(vi3);

	k=0;
	k=(k<<2) | ((vy3>ymi)?((vy3>yhi)?3:2):((vy3>ylo)?1:0));
	k=(k<<2) | ((vy2>ymi)?((vy2>yhi)?3:2):((vy2>ylo)?1:0));
	k=(k<<2) | ((vy1>ymi)?((vy1>yhi)?3:2):((vy1>ylo)?1:0));
	k=(k<<2) | ((vy2>ymi)?((vy0>yhi)?3:2):((vy0>ylo)?1:0));

	vn=((vp>>8)&0x00FFFFFF)|(((u64)k)<<24);

	ctx->regs[op->rn]=vn;
}

int bjx2_opi_cvth2al(int vi)
{
	int v2;
	vi+=0x0400;
	v2=((vi>>6)&0x7F);
	if(!(vi&0x2000))
		v2=0x00;
	if(vi&0x4000)
		v2=0x7F;
	v2|=((vi>>8)&0x80);
	
//	if(vi!=0)
//		printf("bjx2_opi_cvth2al: %04X->%02X\n", vi, v2);
	
	return(v2);
}

int bjx2_opi_cvtal2h(int vi)
{
	int v2;
	v2=0x2000|((vi&0x7F)<<6)|((vi&0x80)<<8);
	v2-=0x0400;
	return(v2);
}

void BJX2_Op_PCVTH2AL_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn;
	int i, j, k;

//	vs=ctx->regs[op->rn];
	vs=ctx->regs[op->rm];
	
	vn=	(bjx2_opi_cvth2al((vs>> 0)&0xFFFF)<< 0) |
		(bjx2_opi_cvth2al((vs>>16)&0xFFFF)<< 8) |
		(bjx2_opi_cvth2al((vs>>32)&0xFFFF)<<16) |
		(bjx2_opi_cvth2al((vs>>48)&0xFFFF)<<24) ;

	ctx->regs[op->rn]=vn;
}

void BJX2_Op_PCVTAL2H_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64	vs, vt, vn;
	int i, j, k;

//	vs=ctx->regs[op->rn];
	vs=ctx->regs[op->rm];
	
	vn=	(((u64)bjx2_opi_cvtal2h((vs>> 0)&0xFF))<< 0) |
		(((u64)bjx2_opi_cvtal2h((vs>> 8)&0xFF))<<16) |
		(((u64)bjx2_opi_cvtal2h((vs>>16)&0xFF))<<32) |
		(((u64)bjx2_opi_cvtal2h((vs>>24)&0xFF))<<48) ;

	ctx->regs[op->rn]=vn;
}
