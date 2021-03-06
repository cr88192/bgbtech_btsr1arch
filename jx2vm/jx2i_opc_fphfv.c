/*
 Copyright (c) 2018-2020 Brendan G Bohannon

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

void BJX2_Op_PADDXD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double a0, b0, c0;
	double a1, b1, c1;
	
	a0=BJX2_PtrGetDoubleIx(ctx->fpreg, op->rm+0);
	a1=BJX2_PtrGetDoubleIx(ctx->fpreg, op->rm+1);
	b0=BJX2_PtrGetDoubleIx(ctx->fpreg, op->ro+0);
	b1=BJX2_PtrGetDoubleIx(ctx->fpreg, op->ro+1);
	c0=a0+b0;	c1=a1+b1;
	BJX2_PtrSetDoubleIx(ctx->fpreg, op->rn+0, c0);
	BJX2_PtrSetDoubleIx(ctx->fpreg, op->rn+1, c1);
}

void BJX2_Op_PSUBXD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double a0, b0, c0;
	double a1, b1, c1;
	
	a0=BJX2_PtrGetDoubleIx(ctx->fpreg, op->rm+0);
	a1=BJX2_PtrGetDoubleIx(ctx->fpreg, op->rm+1);
	b0=BJX2_PtrGetDoubleIx(ctx->fpreg, op->ro+0);
	b1=BJX2_PtrGetDoubleIx(ctx->fpreg, op->ro+1);
	c0=a0-b0;	c1=a1-b1;
	BJX2_PtrSetDoubleIx(ctx->fpreg, op->rn+0, c0);
	BJX2_PtrSetDoubleIx(ctx->fpreg, op->rn+1, c1);
}

void BJX2_Op_PMULXD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double a0, b0, c0;
	double a1, b1, c1;
	
	a0=BJX2_PtrGetDoubleIx(ctx->fpreg, op->rm+0);
	a1=BJX2_PtrGetDoubleIx(ctx->fpreg, op->rm+1);
	b0=BJX2_PtrGetDoubleIx(ctx->fpreg, op->ro+0);
	b1=BJX2_PtrGetDoubleIx(ctx->fpreg, op->ro+1);
	c0=a0*b0;	c1=a1*b1;
	BJX2_PtrSetDoubleIx(ctx->fpreg, op->rn+0, c0);
	BJX2_PtrSetDoubleIx(ctx->fpreg, op->rn+1, c1);
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

int TKRA_GetPixel444A3_Alpha(int a)
{
	int av;
	av=(a&0x0001)|((a&0x0020)>>4)|((a&0x0400)>>8);
	av=(av<<5);
	return(av);
}

u64 	tkra_utx2_cachedblka[64];
u64		tkra_utx2_cachedpels[64][16];

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

	clrc=tkra_pmuluhw(clra, 0xAAAAAAAAAAAAAAAAULL);
	clrd=tkra_pmuluhw(clrb, 0xAAAAAAAAAAAAAAAAULL);
	clrp=tkra_pmuluhw(clra, 0x5555555555555555ULL);
	clrq=tkra_pmuluhw(clrb, 0x5555555555555555ULL);
	clrc+=clrq;
	clrd+=clrp;

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
