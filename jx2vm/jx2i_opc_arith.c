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

void BJX2_Op_MOV_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	ctx->regs[op->rn]=(s64)((s32)(op->imm));
	ctx->regs[op->rn]=(s64)(op->imm);
}

void BJX2_Op_MOV_ImmxReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=((s32)op->imm)^0xFFFFFFFF00000000LL;
}

void BJX2_Op_MOV_ImmuReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=((u32)op->imm);
}

void BJX2_Op_MOV_ImmnReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=((s32)op->imm)|0xFFFFFFFF00000000LL;
}

void BJX2_Op_MOV_ImmHiReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=((u64)(op->imm))<<32;
}

void BJX2_Op_MOV_ImmMidReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=((u64)((u32)(op->imm)))<<16;
}

void BJX2_Op_MOV_JImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	BJX2_Opcode *op1;

	op1=op->data;
	ctx->regs[op->rn]=(((u64)(op->imm))<<32)|((u32)(op1->imm));
}

void BJX2_Op_MOVPT_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
		ctx->regs[op->rn]=(s64)(op->imm);
}

void BJX2_Op_MOVPF_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
		ctx->regs[op->rn]=(s64)(op->imm);
}

void BJX2_Op_MOV_Dr4Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[BJX2_REG_DR]*16+op->imm;
}

void BJX2_Op_ADD_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
#if 0
	if(op->rn==BJX2_REG_SP)
	{
		printf("ADJ SP %llX %d(%X) ", ctx->regs[op->rn], op->imm, op->imm);
	}
#endif

//	ctx->regs[op->rn]+=op->imm;
	ctx->regs[op->rn]=ctx->regs[op->rn]+((s64)((s32)(op->imm)));

#if 0
	if(op->rn==BJX2_REG_SP)
	{
		printf("%llX\n", ctx->regs[op->rn]);
	}
#endif
}

void BJX2_Op_ADD_Dr4Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]+=ctx->regs[BJX2_REG_DR]*16+op->imm;
}

void BJX2_Op_ADD_RegDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+ctx->regs[BJX2_REG_DR];
}

void BJX2_Op_ADD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+ctx->regs[op->ro];
}

void BJX2_Op_ADD_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]+op->imm;
}


void BJX2_Op_ADDSL_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	ctx->regs[op->rn]=(s32)(ctx->regs[op->rm]+ctx->regs[op->rn]);
	ctx->regs[op->rn]=(s32)(ctx->regs[op->rm])+(s32)(ctx->regs[op->rn]);
}

void BJX2_Op_ADDSL_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	ctx->regs[op->rn]=(s32)(ctx->regs[op->rm]+ctx->regs[op->ro]);
	ctx->regs[op->rn]=(s32)(ctx->regs[op->rm])+(s32)(ctx->regs[op->ro]);
}

void BJX2_Op_ADDUL_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	ctx->regs[op->rn]=(u32)(ctx->regs[op->rm]+ctx->regs[op->ro]);
	ctx->regs[op->rn]=(u32)(ctx->regs[op->rm])+(u32)(ctx->regs[op->ro]);
}

void BJX2_Op_ADDSL_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	ctx->regs[op->rn]=(s32)(ctx->regs[op->rm]+op->imm);
	ctx->regs[op->rn]=(s32)(ctx->regs[op->rm])+(s32)(op->imm);
}

void BJX2_Op_ADDUL_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	ctx->regs[op->rn]=(u32)(ctx->regs[op->rm]+op->imm);
	ctx->regs[op->rn]=(u32)(ctx->regs[op->rm])+(u32)(op->imm);
}

void BJX2_Op_SUBSL_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	ctx->regs[op->rn]=(s32)(ctx->regs[op->rm]-ctx->regs[op->ro]);
	ctx->regs[op->rn]=(s32)(ctx->regs[op->rm])-(s32)(ctx->regs[op->ro]);
}

void BJX2_Op_SUBUL_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	ctx->regs[op->rn]=(u32)(ctx->regs[op->rm]-ctx->regs[op->ro]);
	ctx->regs[op->rn]=(u32)(ctx->regs[op->rm])-(u32)(ctx->regs[op->ro]);
}

void BJX2_Op_SUBSL_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	ctx->regs[op->rn]=(s32)(ctx->regs[op->rm]-op->imm);
	ctx->regs[op->rn]=(s32)(ctx->regs[op->rm])-(s32)(op->imm);
}

void BJX2_Op_SUBUL_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	ctx->regs[op->rn]=(u32)(ctx->regs[op->rm]-op->imm);
	ctx->regs[op->rn]=(u32)(ctx->regs[op->rm])-(u32)(op->imm);
}

void BJX2_Op_SUBP_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		((ctx->regs[op->rm])&0x0000FFFFFFFFFFFFULL)-
		((ctx->regs[op->ro])&0x0000FFFFFFFFFFFFULL);
}

void BJX2_Op_SUBXP_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va0, vb0, vc0;
	u64 va1, vb1, vc1;

	va0=((ctx->regs[op->rm+0])&0x0000FFFFFFFFFFFFULL);
	vb0=((ctx->regs[op->ro+0])&0x0000FFFFFFFFFFFFULL);
	vc0=va0-vb0;

	va1=((ctx->regs[op->rm+1])&0x0000FFFFFFFFFFFFULL);
	vb1=((ctx->regs[op->ro+1])&0x0000FFFFFFFFFFFFULL);
	vc1=va1-vb1;
	if((vc0>>63)&1)
		vc1--;

	ctx->regs[op->rn+0]=vc0;
	ctx->regs[op->rn+1]=vc1;
}

void BJX2_Op_MOV_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm];
}

void BJX2_Op_MOVC_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&(1<<30)))
	{
		if(	(op->rn==BJX2_REG_PC) ||
			(op->rn==BJX2_REG_SR) ||
			(op->rn==BJX2_REG_VBR) ||
			(op->rn==BJX2_REG_SPC) ||
			(op->rn==BJX2_REG_SSP) ||
			(op->rn==BJX2_REG_TBR) ||
			(op->rn==BJX2_REG_TTB) ||
			(op->rn==BJX2_REG_TEA) ||
			(op->rn==BJX2_REG_MMCR) ||
			(op->rn==BJX2_REG_EXSR) ||
			(op->rn==BJX2_REG_STTB) ||
			(op->rn==BJX2_REG_KRR))
		{
			ctx->trapc=op->pc;
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
		}

		if(	(op->rm==BJX2_REG_PC) ||
			(op->rm==BJX2_REG_SR) ||
			(op->rm==BJX2_REG_VBR) ||
			(op->rm==BJX2_REG_SPC) ||
			(op->rm==BJX2_REG_SSP) ||
			(op->rm==BJX2_REG_TTB) ||
			(op->rm==BJX2_REG_TEA) ||
			(op->rm==BJX2_REG_MMCR) ||
			(op->rm==BJX2_REG_EXSR) ||
			(op->rm==BJX2_REG_STTB) ||
			(op->rm==BJX2_REG_KRR))
		{
			ctx->trapc=op->pc;
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
		}
	}

	ctx->regs[op->rn]=ctx->regs[op->rm];
}

void BJX2_Op_MOVD_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm];
}

void BJX2_Op_MOVX_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn+0]=ctx->regs[op->rm+0];
	ctx->regs[op->rn+1]=ctx->regs[op->rm+1];
}

void BJX2_Op_MOVHD_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		(ctx->regs[op->rn]&0x00000000FFFFFFFFULL)|
		(ctx->regs[op->rm]&0xFFFFFFFF00000000ULL);
}

void BJX2_Op_MOVLD_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		(ctx->regs[op->rn]&0xFFFFFFFF00000000ULL)|
		(ctx->regs[op->rm]&0x00000000FFFFFFFFULL);
}

void BJX2_Op_ADD_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]+=ctx->regs[op->rm];
}

void BJX2_Op_SUB_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]-=ctx->regs[op->rm];
}

void BJX2_Op_SUB_RegDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]-ctx->regs[BJX2_REG_DR];
}

void BJX2_Op_SUB_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]-ctx->regs[op->ro];
}

void BJX2_Op_SUB_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]-op->imm;
}

void BJX2_Op_RSUB_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=op->imm-ctx->regs[op->rm];
}

void BJX2_Op_ADC_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;

	va=ctx->regs[op->rn];
	vb=ctx->regs[op->rm];
	vc=va+vb+(ctx->regs[BJX2_REG_SR]&1);
	ctx->regs[op->rn]=vc;
	if(vc<(va|vb))
//	if((vc>>32)&1)
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;

//	ctx->regs[op->rn]+=ctx->regs[op->rm]+(ctx->regs[BJX2_REG_SR]&1);
}

void BJX2_Op_SBB_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;

	va=ctx->regs[op->rn];
	vb=ctx->regs[op->rm];
	vc=va+(~vb)+(!(ctx->regs[BJX2_REG_SR]&1));
	ctx->regs[op->rn]=vc;
//	if(vc<(va|(~vb)))
	if(!(vc<(va|(~vb))))
//	if((vc>>32)&1)
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;

//	ctx->regs[op->rn]+=(~ctx->regs[op->rm])+(!(ctx->regs[BJX2_REG_SR]&1));
}

void BJX2_Op_PADDW_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	const u64 m1=0xFFFF0000FFFF0000ULL;
	const u64 m2=0x0000FFFF0000FFFFULL;
	ctx->regs[op->rn]=
		(((ctx->regs[op->rn]&m1)+(ctx->regs[op->rm]&m1))&m1)|
		(((ctx->regs[op->rn]&m2)+(ctx->regs[op->rm]&m2))&m2);
}

void BJX2_Op_PSUBW_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	const u64 m1=0xFFFF0000FFFF0000ULL;
	const u64 m2=0x0000FFFF0000FFFFULL;
	const u64 b1=0x0001000000010000ULL;
	const u64 b2=0x0000000100000001ULL;
	u64 pm1, pm2;
//	ctx->regs[op->rn]=
//		(((ctx->regs[op->rn]&m1)-(ctx->regs[op->rm]&m1))&m1)|
//		(((ctx->regs[op->rn]&m2)-(ctx->regs[op->rm]&m2))&m2);

	pm1=(~(ctx->regs[op->rm]&m1)+b1)&m1;
	pm2=(~(ctx->regs[op->rm]&m2)+b2)&m2;
	ctx->regs[op->rn]=
		(((ctx->regs[op->rn]&m1)+pm1)&m1)|
		(((ctx->regs[op->rn]&m2)+pm2)&m2);
}

void BJX2_Op_PADDL_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	const u64 m1=0xFFFFFFFF00000000ULL;
	const u64 m2=0x00000000FFFFFFFFULL;
	ctx->regs[op->rn]=
		(((ctx->regs[op->rn]&m1)+(ctx->regs[op->rm]&m1))&m1)|
		(((ctx->regs[op->rn]&m2)+(ctx->regs[op->rm]&m2))&m2);
}

void BJX2_Op_PSUBL_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	const u64 m1=0xFFFFFFFF00000000ULL;
	const u64 m2=0x00000000FFFFFFFFULL;
	ctx->regs[op->rn]=
		(((ctx->regs[op->rn]&m1)-(ctx->regs[op->rm]&m1))&m1)|
		(((ctx->regs[op->rn]&m2)-(ctx->regs[op->rm]&m2))&m2);
}

void BJX2_Op_PMULUW_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 x0, x1, x2, x3;
	u32 y0, y1, y2, y3;
	u32 z0, z1, z2, z3;
	u64 lv, hv;
	
	x0=(ctx->regs[op->rn]>> 0)&65535;
	x1=(ctx->regs[op->rn]>>16)&65535;
	x2=(ctx->regs[op->rn]>>32)&65535;
	x3=(ctx->regs[op->rn]>>48)&65535;

	y0=(ctx->regs[op->rm]>> 0)&65535;
	y1=(ctx->regs[op->rm]>>16)&65535;
	y2=(ctx->regs[op->rm]>>32)&65535;
	y3=(ctx->regs[op->rm]>>48)&65535;
	
	z0=x0*y0;
	z1=x1*y1;
	z2=x2*y2;
	z3=x3*y3;
	
	lv=	(((u64)((u16)z0))<< 0)|
		(((u64)((u16)z1))<<16)|
		(((u64)((u16)z2))<<32)|
		(((u64)((u16)z3))<<48);

	hv=	(((u64)((u16)(z0>>16)))<< 0)|
		(((u64)((u16)(z1>>16)))<<16)|
		(((u64)((u16)(z2>>16)))<<32)|
		(((u64)((u16)(z3>>16)))<<48);
	
	ctx->regs[BJX2_REG_DLR]=lv;
	ctx->regs[BJX2_REG_DHR]=hv;
}

void BJX2_Op_PMULULW_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 x0, x1, x2, x3;
	u32 y0, y1, y2, y3;
	u32 z0, z1, z2, z3;
	u64 lv, hv;
	
	x0=(ctx->regs[op->ro]>> 0)&65535;
	x1=(ctx->regs[op->ro]>>16)&65535;
	x2=(ctx->regs[op->ro]>>32)&65535;
	x3=(ctx->regs[op->ro]>>48)&65535;

	y0=(ctx->regs[op->rm]>> 0)&65535;
	y1=(ctx->regs[op->rm]>>16)&65535;
	y2=(ctx->regs[op->rm]>>32)&65535;
	y3=(ctx->regs[op->rm]>>48)&65535;
	
	z0=x0*y0;
	z1=x1*y1;
	z2=x2*y2;
	z3=x3*y3;
	
	lv=	(((u64)((u16)z0))<< 0)|
		(((u64)((u16)z1))<<16)|
		(((u64)((u16)z2))<<32)|
		(((u64)((u16)z3))<<48);
	
	ctx->regs[op->rn]=lv;
}

void BJX2_Op_PMULUHW_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 x0, x1, x2, x3;
	u32 y0, y1, y2, y3;
	u32 z0, z1, z2, z3;
	u64 lv, hv;
	
	x0=(ctx->regs[op->ro]>> 0)&65535;
	x1=(ctx->regs[op->ro]>>16)&65535;
	x2=(ctx->regs[op->ro]>>32)&65535;
	x3=(ctx->regs[op->ro]>>48)&65535;

	y0=(ctx->regs[op->rm]>> 0)&65535;
	y1=(ctx->regs[op->rm]>>16)&65535;
	y2=(ctx->regs[op->rm]>>32)&65535;
	y3=(ctx->regs[op->rm]>>48)&65535;
	
	z0=(x0*y0)>>16;
	z1=(x1*y1)>>16;
	z2=(x2*y2)>>16;
	z3=(x3*y3)>>16;
	
//	hv=	(((u64)((u16)(z0>>16)))<< 0)|
//		(((u64)((u16)(z1>>16)))<<16)|
//		(((u64)((u16)(z2>>16)))<<32)|
//		(((u64)((u16)(z3>>16)))<<48);

	hv=	(((u64)((u16)z0))<< 0)|
		(((u64)((u16)z1))<<16)|
		(((u64)((u16)z2))<<32)|
		(((u64)((u16)z3))<<48);
	
	ctx->regs[op->rn]=hv;
}

void BJX2_Op_PMULSLW_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s32 x0, x1, x2, x3;
	s32 y0, y1, y2, y3;
	s32 z0, z1, z2, z3;
	u64 lv, hv;
	
	x0=(s16)(ctx->regs[op->ro]>> 0);
	x1=(s16)(ctx->regs[op->ro]>>16);
	x2=(s16)(ctx->regs[op->ro]>>32);
	x3=(s16)(ctx->regs[op->ro]>>48);

	y0=(s16)(ctx->regs[op->rm]>> 0);
	y1=(s16)(ctx->regs[op->rm]>>16);
	y2=(s16)(ctx->regs[op->rm]>>32);
	y3=(s16)(ctx->regs[op->rm]>>48);
	
	z0=x0*y0;
	z1=x1*y1;
	z2=x2*y2;
	z3=x3*y3;
	
	lv=	(((u64)((u16)z0))<< 0)|
		(((u64)((u16)z1))<<16)|
		(((u64)((u16)z2))<<32)|
		(((u64)((u16)z3))<<48);
	
	ctx->regs[op->rn]=lv;
}

void BJX2_Op_PMULSHW_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s32 x0, x1, x2, x3;
	s32 y0, y1, y2, y3;
	s32 z0, z1, z2, z3;
	u64 lv, hv;
	
	x0=(s16)(ctx->regs[op->ro]>> 0);
	x1=(s16)(ctx->regs[op->ro]>>16);
	x2=(s16)(ctx->regs[op->ro]>>32);
	x3=(s16)(ctx->regs[op->ro]>>48);

	y0=(s16)(ctx->regs[op->rm]>> 0);
	y1=(s16)(ctx->regs[op->rm]>>16);
	y2=(s16)(ctx->regs[op->rm]>>32);
	y3=(s16)(ctx->regs[op->rm]>>48);
	
	z0=x0*y0;
	z1=x1*y1;
	z2=x2*y2;
	z3=x3*y3;
	
	hv=	(((u64)((u16)(z0>>16)))<< 0)|
		(((u64)((u16)(z1>>16)))<<16)|
		(((u64)((u16)(z2>>16)))<<32)|
		(((u64)((u16)(z3>>16)))<<48);
	
	ctx->regs[op->rn]=hv;
}

void BJX2_Op_PADDW_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	const u64 m1=0xFFFF0000FFFF0000ULL;
	const u64 m2=0x0000FFFF0000FFFFULL;
	ctx->regs[op->rn]=
		(((ctx->regs[op->rm]&m1)+(ctx->regs[op->ro]&m1))&m1)|
		(((ctx->regs[op->rm]&m2)+(ctx->regs[op->ro]&m2))&m2);
}

void BJX2_Op_PSUBW_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	const u64 m1=0xFFFF0000FFFF0000ULL;
	const u64 m2=0x0000FFFF0000FFFFULL;
	const u64 b1=0x0001000000010000ULL;
	const u64 b2=0x0000000100000001ULL;
	u64 pm1, pm2;

	pm1=(~(ctx->regs[op->ro]&m1)+b1)&m1;
	pm2=(~(ctx->regs[op->ro]&m2)+b2)&m2;
	ctx->regs[op->rn]=
		(((ctx->regs[op->rm]&m1)+pm1)&m1)|
		(((ctx->regs[op->rm]&m2)+pm2)&m2);
}

void BJX2_Op_PADDL_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	const u64 m1=0xFFFFFFFF00000000ULL;
	const u64 m2=0x00000000FFFFFFFFULL;
	ctx->regs[op->rn]=
		(((ctx->regs[op->rm]&m1)+(ctx->regs[op->ro]&m1))&m1)|
		(((ctx->regs[op->rm]&m2)+(ctx->regs[op->ro]&m2))&m2);
}

void BJX2_Op_PSUBL_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	const u64 m1=0xFFFFFFFF00000000ULL;
	const u64 m2=0x00000000FFFFFFFFULL;
	ctx->regs[op->rn]=
		(((ctx->regs[op->rm]&m1)-(ctx->regs[op->ro]&m1))&m1)|
		(((ctx->regs[op->rm]&m2)-(ctx->regs[op->ro]&m2))&m2);
}


void BJX2_Op_TST_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if((u32)(ctx->regs[op->rn]&ctx->regs[op->rm]))
		ctx->regs[BJX2_REG_SR]&=~1;
	else
		ctx->regs[BJX2_REG_SR]|=1;
}

void BJX2_Op_TST_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if((u32)(ctx->regs[op->rn]&op->imm))
		ctx->regs[BJX2_REG_SR]&=~1;
	else
		ctx->regs[BJX2_REG_SR]|=1;
}

void BJX2_Op_TSTQ_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[op->rn]&ctx->regs[op->rm])
		ctx->regs[BJX2_REG_SR]&=~1;
	else
		ctx->regs[BJX2_REG_SR]|=1;
}

void BJX2_Op_TSTQ_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[op->rn]&op->imm)
		ctx->regs[BJX2_REG_SR]&=~1;
	else
		ctx->regs[BJX2_REG_SR]|=1;
}

void BJX2_Op_AND_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]&=ctx->regs[op->rm];
}

void BJX2_Op_AND_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]&=op->imm;
}

void BJX2_Op_AND_RegDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]&ctx->regs[BJX2_REG_DR];
}

void BJX2_Op_AND_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]&ctx->regs[op->ro];
}

void BJX2_Op_AND_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]&op->imm;
}

void BJX2_Op_OR_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]|=ctx->regs[op->rm];
}

void BJX2_Op_OR_RegDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]|ctx->regs[BJX2_REG_DR];
}

void BJX2_Op_OR_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]|ctx->regs[op->ro];
}

void BJX2_Op_OR_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]|op->imm;
}


void BJX2_Op_XOR_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]^=ctx->regs[op->rm];
}

void BJX2_Op_XOR_RegDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]^ctx->regs[BJX2_REG_DR];
}

void BJX2_Op_XOR_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]^ctx->regs[op->ro];
}

void BJX2_Op_XOR_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]^op->imm;
}

void BJX2_Op_MULS_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 va, vb, vc;
	va=(s32)(ctx->regs[op->rn]);
	vb=(s32)(ctx->regs[op->rm]);
	vc=va*vb;
	ctx->regs[BJX2_REG_DLR]=(u32)(vc    );
	ctx->regs[BJX2_REG_DHR]=(s32)(vc>>32);
}

void BJX2_Op_MULU_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;
	va=(u32)(ctx->regs[op->rn]);
	vb=(u32)(ctx->regs[op->rm]);
	vc=va*vb;
	ctx->regs[BJX2_REG_DLR]=(u32)(vc    );
	ctx->regs[BJX2_REG_DHR]=(u32)(vc>>32);
}

void BJX2_Op_MULS_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	s64 va, vb, vc;
	s32 va, vb, vc;
	va=(s32)(ctx->regs[op->ro]);
	vb=(s32)(ctx->regs[op->rm]);
	vc=va*vb;
	ctx->regs[op->rn]=vc;
}

void BJX2_Op_MULS_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	ctx->regs[op->rn]=((s64)((s32)ctx->regs[op->rm]))*op->imm;
	ctx->regs[op->rn]=(s32)(((s32)ctx->regs[op->rm])*op->imm);
}

void BJX2_Op_MULU_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	s64 va, vb, vc;
	u32 va, vb, vc;
	va=(u32)(ctx->regs[op->ro]);
	vb=(u32)(ctx->regs[op->rm]);
	vc=va*vb;
	ctx->regs[op->rn]=vc;
}

void BJX2_Op_MULU_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	ctx->regs[op->rn]=((u64)((u32)ctx->regs[op->rm]))*op->imm;
	ctx->regs[op->rn]=(u32)(((u32)ctx->regs[op->rm])*op->imm);
}

#if 1
void BJX2_Op_MULSW_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	s64 va, vb, vc;
	s32 va, vb, vc;
	va=(s16)(ctx->regs[op->ro]);
	vb=(s16)(ctx->regs[op->rm]);
	vc=va*vb;
	ctx->regs[op->rn]=vc;
}

void BJX2_Op_MULSW_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	ctx->regs[op->rn]=((s64)((s32)ctx->regs[op->rm]))*op->imm;
	ctx->regs[op->rn]=(s32)(((s16)ctx->regs[op->rm])*op->imm);
}

void BJX2_Op_MULUW_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	s64 va, vb, vc;
	u32 va, vb, vc;
	va=(u16)(ctx->regs[op->ro]);
	vb=(u16)(ctx->regs[op->rm]);
	vc=va*vb;
	ctx->regs[op->rn]=vc;
}

void BJX2_Op_MULUW_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	ctx->regs[op->rn]=((u64)((u32)ctx->regs[op->rm]))*op->imm;
	ctx->regs[op->rn]=(u32)(((u16)ctx->regs[op->rm])*op->imm);
}


void BJX2_Op_MULSW_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(s32)(((s16)ctx->regs[op->rn])*op->imm);
}

void BJX2_Op_MULUW_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(u32)(((u16)ctx->regs[op->rn])*op->imm);
}
#endif

#if 1
void BJX2_Op_MULSL_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	s64 va, vb, vc;
	s64 va, vb, vc;
	va=(s32)(ctx->regs[op->ro]);
	vb=(s32)(ctx->regs[op->rm]);
	vc=va*vb;
	ctx->regs[op->rn]=vc;
}

void BJX2_Op_MULSL_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=((s64)((s32)ctx->regs[op->rm]))*op->imm;
//	ctx->regs[op->rn]=(s32)(((s32)ctx->regs[op->rm])*op->imm);
}

void BJX2_Op_MULUL_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 va, vb, vc;
//	u32 va, vb, vc;
	va=(u32)(ctx->regs[op->ro]);
	vb=(u32)(ctx->regs[op->rm]);
	vc=va*vb;
	ctx->regs[op->rn]=vc;
}

void BJX2_Op_MULUL_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=((u64)((u32)ctx->regs[op->rm]))*op->imm;
//	ctx->regs[op->rn]=(u32)(((u32)ctx->regs[op->rm])*op->imm);
}
#endif

void BJX2_Op_MULSQ_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	s64 va, vb, vc;
	s64 va, vb, vc;
	va=(s64)(ctx->regs[op->ro]);
	vb=(s64)(ctx->regs[op->rm]);
	vc=va*vb;
	ctx->regs[op->rn]=vc;
}

void BJX2_Op_MULUQ_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;
	va=(u64)(ctx->regs[op->ro]);
	vb=(u64)(ctx->regs[op->rm]);
	vc=va*vb;
	ctx->regs[op->rn]=vc;
}

void BJX2_Op_MULSHQ_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	s64 va, vb, vc;
	s64 va, vb, vc, vc1;
	va=(s64)(ctx->regs[op->ro]);
	vb=(s64)(ctx->regs[op->rm]);
	
	vc1=	(((u32)va)*(vb>>32)) +
			(((u32)vb)*(va>>32)) ;
	vc=((va>>32)*(vb>>32)) + (vc1>>32);

	ctx->regs[op->rn]=vc;
}

void BJX2_Op_MULUHQ_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc, vc1;
	va=(u64)(ctx->regs[op->ro]);
	vb=(u64)(ctx->regs[op->rm]);

	vc1=	(((u32)va)*(vb>>32)) +
			(((u32)vb)*(va>>32)) ;
	vc=((va>>32)*(vb>>32)) + (vc1>>32);

	ctx->regs[op->rn]=vc;
}

void BJX2_Op_DIVSQ_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	s64 va, vb, vc;
	s64 va, vb, vc;
	va=(s64)(ctx->regs[op->rm]);
	vb=(s64)(ctx->regs[op->ro]);
	if(vb!=0)
		{ vc=va/vb; }
	else
		{ vc=0; }
	ctx->regs[op->rn]=vc;
}

void BJX2_Op_DIVUQ_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;
	va=(u64)(ctx->regs[op->rm]);
	vb=(u64)(ctx->regs[op->ro]);
	if(vb!=0)
		{ vc=va/vb; }
	else
		{ vc=0; }
	ctx->regs[op->rn]=vc;
}

void BJX2_Op_MODSQ_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	s64 va, vb, vc;
	s64 va, vb, vc;
	va=(s64)(ctx->regs[op->rm]);
	vb=(s64)(ctx->regs[op->ro]);
	if(vb!=0)
		{ vc=va%vb; }
	else
		{ vc=0; }
	ctx->regs[op->rn]=vc;
}

void BJX2_Op_MODUQ_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;
	va=(u64)(ctx->regs[op->rm]);
	vb=(u64)(ctx->regs[op->ro]);
	if(vb!=0)
		{ vc=va%vb; }
	else
		{ vc=0; }
	ctx->regs[op->rn]=vc;
}

void BJX2_Op_DIVSL_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	s64 va, vb, vc;
	s64 va, vb, vc;
	va=(s32)(ctx->regs[op->rm]);
	vb=(s32)(ctx->regs[op->ro]);
	if(vb!=0)
		{ vc=va/vb; }
	else
		{ vc=0; }
	ctx->regs[op->rn]=vc;
	
	if((va<0) || (vb<0) || (vb>31))
	{
		ctx->tr_cur->acc_pencyc+=33;
		ctx->miss_cyc_bra+=33;
	}
}

void BJX2_Op_DIVUL_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;
	va=(u32)(ctx->regs[op->rm]);
	vb=(u32)(ctx->regs[op->ro]);
	if(vb!=0)
		{ vc=va/vb; }
	else
		{ vc=0; }
	ctx->regs[op->rn]=vc;

	if(vb>31)
	{
		ctx->tr_cur->acc_pencyc+=33;
		ctx->miss_cyc_bra+=33;
	}
}


void BJX2_Op_MACSL_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	s64 va, vb, vc;
	s64 va, vb, vc;
	va=(s32)(ctx->regs[op->ro]);
	vb=(s32)(ctx->regs[op->rm]);
	vc=va*vb;
	ctx->regs[op->rn]=(s32)(ctx->regs[op->rn]+vc);
}

void BJX2_Op_MACSL_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 va, vb, vc;
	va=(s32)(op->imm);
	vb=(s32)(ctx->regs[op->rm]);
	vc=va*vb;
	ctx->regs[op->rn]=(s32)(ctx->regs[op->rn]+vc);

//	ctx->regs[op->rn]=(s32)(ctx->regs[op->rn]+((s32)ctx->regs[op->rm])*op->imm);
}

void BJX2_Op_MACUL_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 va, vb, vc;
//	u32 va, vb, vc;
	va=(u32)(ctx->regs[op->ro]);
	vb=(u32)(ctx->regs[op->rm]);
	vc=va*vb;
//	ctx->regs[op->rn]+=vc;
	ctx->regs[op->rn]=(u32)(ctx->regs[op->rn]+vc);
}

void BJX2_Op_MACUL_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(u32)(ctx->regs[op->rn]+((u32)ctx->regs[op->rm])*op->imm);
}

void BJX2_Op_DMACSL_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	s64 va, vb, vc;
	s64 va, vb, vc;
	va=(s32)(ctx->regs[op->ro]);
	vb=(s32)(ctx->regs[op->rm]);
	vc=va*vb;
	ctx->regs[op->rn]+=vc;
}

void BJX2_Op_DMACSL_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]+=((s64)((s32)ctx->regs[op->rm]))*op->imm;
}

void BJX2_Op_DMACUL_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 va, vb, vc;
//	u32 va, vb, vc;
	va=(u32)(ctx->regs[op->ro]);
	vb=(u32)(ctx->regs[op->rm]);
	vc=va*vb;
	ctx->regs[op->rn]+=vc;
}

void BJX2_Op_DMACUL_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]+=((u64)((u32)ctx->regs[op->rm]))*op->imm;
//	ctx->regs[op->rn]=(u32)(((u32)ctx->regs[op->rm])*op->imm);
}


#if 1
void BJX2_Op_MACSL_RegRegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	s64 va, vb, vc;
	s64 va, vb, vc;
	va=(s32)(ctx->regs[op->ro]);
	vb=(s32)(ctx->regs[op->rm]);
	vc=va*vb;
	ctx->regs[op->rn]=(s32)(ctx->regs[op->rp]+vc);
}

void BJX2_Op_MACUL_RegRegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 va, vb, vc;
//	u32 va, vb, vc;
	va=(u32)(ctx->regs[op->ro]);
	vb=(u32)(ctx->regs[op->rm]);
	vc=va*vb;
//	ctx->regs[op->rn]+=vc;
	ctx->regs[op->rn]=(u32)(ctx->regs[op->rp]+vc);
}

void BJX2_Op_DMACSL_RegRegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	s64 va, vb, vc;
	s64 va, vb, vc;
	va=(s32)(ctx->regs[op->ro]);
	vb=(s32)(ctx->regs[op->rm]);
	vc=va*vb;
//	ctx->regs[op->rn]+=vc;
	ctx->regs[op->rn]=(ctx->regs[op->rp]+vc);
}

void BJX2_Op_DMACUL_RegRegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 va, vb, vc;
//	u32 va, vb, vc;
	va=(u32)(ctx->regs[op->ro]);
	vb=(u32)(ctx->regs[op->rm]);
	vc=va*vb;
//	ctx->regs[op->rn]+=vc;
	ctx->regs[op->rn]=(ctx->regs[op->rp]+vc);
}

void BJX2_Op_MACSL_RegImmRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 va, vb, vc;
	va=(s32)(op->imm);
	vb=(s32)(ctx->regs[op->rm]);
	vc=va*vb;
	ctx->regs[op->rn]=(s32)(ctx->regs[op->ro]+vc);
}

void BJX2_Op_MACUL_RegImmRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 va, vb, vc;
	va=(u32)(op->imm);
	vb=(u32)(ctx->regs[op->rm]);
	vc=va*vb;
	ctx->regs[op->rn]=(u32)(ctx->regs[op->ro]+vc);
}

void BJX2_Op_DMACSL_RegImmRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 va, vb, vc;
	va=(s32)(op->imm);
	vb=(s32)(ctx->regs[op->rm]);
	vc=va*vb;
	ctx->regs[op->rn]=(ctx->regs[op->ro]+vc);
}

void BJX2_Op_DMACUL_RegImmRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 va, vb, vc;
	va=(u32)(op->imm);
	vb=(u32)(ctx->regs[op->rm]);
	vc=va*vb;
	ctx->regs[op->rn]=(ctx->regs[op->ro]+vc);
}

#endif

void BJX2_Op_CMPEQ_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((u32)ctx->regs[op->rn])==((u32)ctx->regs[op->rm]))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPHI_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((u32)ctx->regs[op->rn])>((u32)ctx->regs[op->rm]))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPGT_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((s32)ctx->regs[op->rn])>((s32)ctx->regs[op->rm]))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPHS_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((u32)ctx->regs[op->rn])>=((u32)ctx->regs[op->rm]))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPGE_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((s32)ctx->regs[op->rn])>=((s32)ctx->regs[op->rm]))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPEQ_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((u32)ctx->regs[op->rn])==((u32)op->imm))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPGT_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((s32)ctx->regs[op->rn])>((s32)op->imm))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPGE_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((s32)ctx->regs[op->rn])>=((s32)op->imm))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPHI_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((u32)ctx->regs[op->rn])>((u32)op->imm))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPHS_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((u32)ctx->regs[op->rn])>=((u32)op->imm))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}


void BJX2_Op_CMPQEQ_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[op->rn]==ctx->regs[op->rm])
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPQHI_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((u64)ctx->regs[op->rn])>((u64)ctx->regs[op->rm]))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPQGT_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((s64)ctx->regs[op->rn])>((s64)ctx->regs[op->rm]))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPQHS_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((u64)ctx->regs[op->rn])>=((u64)ctx->regs[op->rm]))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPQGE_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((s64)ctx->regs[op->rn])>=((s64)ctx->regs[op->rm]))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPQEQ_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((u64)ctx->regs[op->rn])==((u64)op->imm))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPQGT_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((s64)ctx->regs[op->rn])>((s64)op->imm))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPQGE_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((s64)ctx->regs[op->rn])>=((s64)op->imm))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPQHI_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((u64)ctx->regs[op->rn])>((u64)op->imm))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPQHS_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((u64)ctx->regs[op->rn])>=((u64)op->imm))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}


void BJX2_Op_CMPPEQ_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(	(ctx->regs[op->rn]&0x0000FFFFFFFFFFFFULL) ==
		(ctx->regs[op->rm]&0x0000FFFFFFFFFFFFULL) )
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPPGT_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(	(ctx->regs[op->rn]&0x0000FFFFFFFFFFFFULL) >
		(ctx->regs[op->rm]&0x0000FFFFFFFFFFFFULL) )
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPPEQX_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va0, vb0, va1, vb1;

	va0 = ctx->regs[op->rn+0]&0x0000FFFFFFFFFFFFULL;
	vb0 = ctx->regs[op->rm+0]&0x0000FFFFFFFFFFFFULL;
	va1 = ctx->regs[op->rn+1]&0x0000FFFFFFFFFFFFULL;
	vb1 = ctx->regs[op->rm+1]&0x0000FFFFFFFFFFFFULL;

	if(	(va0 == vb0) && (va1 == vb1) )
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPPGTX_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va0, vb0, va1, vb1;

	va0 = ctx->regs[op->rn+0]&0x0000FFFFFFFFFFFFULL;
	vb0 = ctx->regs[op->rm+0]&0x0000FFFFFFFFFFFFULL;
	va1 = ctx->regs[op->rn+1]&0x0000FFFFFFFFFFFFULL;
	vb1 = ctx->regs[op->rm+1]&0x0000FFFFFFFFFFFFULL;

	if(	(va1>vb1) || ((va0 > vb0) && (va1 == vb1)) )
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}


void BJX2_Op_CMPTTEQ_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v;
	int i, p;

	v=ctx->regs[op->rn];
	i=op->imm;
	p=0;

	if(!(i&1))
	{
		if(i&32)
			{ p=((v&15)==((i>>1)&15)); }
		else
			{ p=(((v>>60)&15)==((i>>1)&15)); }
	}else if(!(i&2))
	{
		if(i&32)
			{ p=((v&7)==((i>>2)&7)); }
		else
			{ p=(((v>>61)&7)==((i>>2)&7)); }
	}else if(!(i&4))
	{
		if(i&32)
			{ p=((v&3)==((i>>3)&3)); }
		else
			{ p=(((v>>62)&3)==((i>>3)&3)); }
	}else if(!(i&8))
	{
		if(i&32)
			{ p=((v&1)==((i>>4)&1)); }
		else
			{ p=(((v>>63)&1)==((i>>4)&1)); }
	}

//	if(((u32)ctx->regs[op->rn])==((u32)op->imm))
	if(p)
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPTAEQ_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((ctx->regs[op->rn]>>48)&4095)==((u32)ctx->regs[op->rm]))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPTAHI_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((ctx->regs[op->rn]>>48)&4095)>((u32)ctx->regs[op->rm]))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPTAHS_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((ctx->regs[op->rn]>>48)&4095)>=((u32)ctx->regs[op->rm]))
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}


void BJX2_Op_LDIZ_Imm(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=op->imm&255;
}

void BJX2_Op_LDIN_Imm(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=op->imm|(~255);
}

void BJX2_Op_LDISH_Imm(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=(ctx->regs[BJX2_REG_DR]<<8)|(op->imm&255);
}

void BJX2_Op_LDISH16_Imm(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=(ctx->regs[BJX2_REG_DR]<<16)|(op->imm&65535);
}

void BJX2_Op_LDISH12_Imm(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=(ctx->regs[BJX2_REG_DR]<<12)|(op->imm&4095);
}

void BJX2_Op_LDISH20_Imm(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=(ctx->regs[BJX2_REG_DR]<<20)|(op->imm&0xFFFFF);
}

void BJX2_Op_LDISH32_Imm(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=
		(((u64)ctx->regs[BJX2_REG_DR])<<32)|
		((u32)(op->imm));
}


void BJX2_Op_LDISH16_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(ctx->regs[op->rn]<<16)|((u16)(op->imm));
}

void BJX2_Op_LDISH32_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(ctx->regs[op->rn]<<32)|((u32)(op->imm));
}


void BJX2_Op_LDIZ_Imm12(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=op->imm&4095;
}

void BJX2_Op_LDIN_Imm12(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=op->imm|(~4095);
}

void BJX2_Op_LDIZ_Imm24(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=op->imm&16777215;
}

void BJX2_Op_LDIN_Imm24(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=op->imm|(~16777215);
}

void BJX2_Op_LDIZ_Imm32(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=(u32)op->imm;
}

void BJX2_Op_LDIN_Imm32(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	ctx->regs[BJX2_REG_DR]=((u32)op->imm)|(-1LL<<32);
	ctx->regs[BJX2_REG_DR]=((u32)op->imm)|(~((1LL<<32)-1));
}


void BJX2_Op_LDIZ_Imm48(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=op->imm;
}

void BJX2_Op_LDIN_Imm48(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_DR]=op->imm|(~((1LL<<48)-1));
}

void BJX2_Op_LEAB_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v1;
	v1=(bjx2_addr)(ctx->regs[op->rm])+(op->imm);
	v1=v1&0x0000FFFFFFFFFFFFULL;
	ctx->regs[op->rn]=v1;
}

void BJX2_Op_LEAW_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v1;
	v1=(bjx2_addr)(ctx->regs[op->rm])+(op->imm*2);
	v1=v1&0x0000FFFFFFFFFFFFULL;
	ctx->regs[op->rn]=v1;

//	ctx->regs[op->rn]=(bjx2_addr)(ctx->regs[op->rm])+(op->imm*2);
}

void BJX2_Op_LEAD_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v1;
	v1=(bjx2_addr)(ctx->regs[op->rm])+(op->imm*4);
	v1=v1&0x0000FFFFFFFFFFFFULL;
	ctx->regs[op->rn]=v1;

//	ctx->regs[op->rn]=(bjx2_addr)(ctx->regs[op->rm])+(op->imm*4);
}

void BJX2_Op_LEAQ_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v1;
	v1=(bjx2_addr)(ctx->regs[op->rm])+(op->imm*8);
	v1=v1&0x0000FFFFFFFFFFFFULL;
	ctx->regs[op->rn]=v1;

//	ctx->regs[op->rn]=(bjx2_addr)(ctx->regs[op->rm])+(op->imm*8);
}

void BJX2_Op_LEAB_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v1;
	v1=(bjx2_addr)(ctx->regs[op->rm])+
		((bjx2_addr)(ctx->regs[BJX2_REG_DR]));
	v1=v1&0x0000FFFFFFFFFFFFULL;
	ctx->regs[op->rn]=v1;

//	ctx->regs[op->rn]=(bjx2_addr)(ctx->regs[op->rm])+
//		(bjx2_addr)(ctx->regs[BJX2_REG_DR]);
}

void BJX2_Op_LEAW_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v1;
	v1=(bjx2_addr)(ctx->regs[op->rm])+
		((bjx2_addr)(ctx->regs[BJX2_REG_DR])*2);
	v1=v1&0x0000FFFFFFFFFFFFULL;
	ctx->regs[op->rn]=v1;

//	ctx->regs[op->rn]=(bjx2_addr)(ctx->regs[op->rm])+
//		((bjx2_addr)(ctx->regs[BJX2_REG_DR])*2);
}

void BJX2_Op_LEAD_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v1;
	v1=(bjx2_addr)(ctx->regs[op->rm])+
		((bjx2_addr)(ctx->regs[BJX2_REG_DR])*4);
	v1=v1&0x0000FFFFFFFFFFFFULL;
	ctx->regs[op->rn]=v1;

//	ctx->regs[op->rn]=(bjx2_addr)(ctx->regs[op->rm])+
//		((bjx2_addr)(ctx->regs[BJX2_REG_DR])*4);
}

void BJX2_Op_LEAQ_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v1;
	v1=(bjx2_addr)(ctx->regs[op->rm])+
		((bjx2_addr)(ctx->regs[BJX2_REG_DR])*8);
	v1=v1&0x0000FFFFFFFFFFFFULL;
	ctx->regs[op->rn]=v1;

//	ctx->regs[op->rn]=(bjx2_addr)(ctx->regs[op->rm])+
//		((bjx2_addr)(ctx->regs[BJX2_REG_DR])*8);
}

void BJX2_Op_LEAB_LdDrPcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	ctx->regs[op->rn]=(op->pc+2)+(ctx->regs[BJX2_REG_DR]*1);
	ctx->regs[op->rn]=(bjx2_addr)(op->pc2)+
		(bjx2_addr)(ctx->regs[BJX2_REG_DR]);
}

void BJX2_Op_LEAW_LdDrPcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	ctx->regs[op->rn]=(op->pc+2)+(ctx->regs[BJX2_REG_DR]*2);
	ctx->regs[op->rn]=(bjx2_addr)(op->pc2)+
		((bjx2_addr)(ctx->regs[BJX2_REG_DR])*2);
}

void BJX2_Op_LEAD_LdDrPcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	ctx->regs[op->rn]=(op->pc+2)+(ctx->regs[BJX2_REG_DR]*4);
	ctx->regs[op->rn]=(bjx2_addr)(op->pc2)+
		((bjx2_addr)(ctx->regs[BJX2_REG_DR])*4);
}

void BJX2_Op_LEAQ_LdDrPcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	ctx->regs[op->rn]=(op->pc+2)+(ctx->regs[BJX2_REG_DR]*8);
	ctx->regs[op->rn]=(bjx2_addr)(op->pc2)+
		((bjx2_addr)(ctx->regs[BJX2_REG_DR])*8);
}

#if 0
void BJX2_Op_LEAB_LdDr4PcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(op->pc+2)+(ctx->regs[BJX2_REG_DR]*16)+(op->imm);
}

void BJX2_Op_LEAW_LdDr4PcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(op->pc+2)+(ctx->regs[BJX2_REG_DR]*32)+(op->imm*2);
}
#endif


void BJX2_Op_LEAB_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v1;
	v1=(bjx2_addr)(ctx->regs[op->rm])+
		(bjx2_addr)(ctx->regs[op->ro]);
	v1=v1&0x0000FFFFFFFFFFFFULL;
	ctx->regs[op->rn]=v1;

//	ctx->regs[op->rn]=(bjx2_addr)(ctx->regs[op->rm])+
//		((bjx2_addr)(ctx->regs[op->ro])*1);
}

void BJX2_Op_LEAW_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v1;
	v1=(bjx2_addr)(ctx->regs[op->rm])+
		((bjx2_addr)(ctx->regs[op->ro])*2);
	v1=v1&0x0000FFFFFFFFFFFFULL;
	ctx->regs[op->rn]=v1;

//	ctx->regs[op->rn]=(bjx2_addr)(ctx->regs[op->rm])+
//		((bjx2_addr)(ctx->regs[op->ro])*2);
}

void BJX2_Op_LEAD_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v1;
	v1=(bjx2_addr)(ctx->regs[op->rm])+
		((bjx2_addr)(ctx->regs[op->ro])*4);
	v1=v1&0x0000FFFFFFFFFFFFULL;
	ctx->regs[op->rn]=v1;

//	ctx->regs[op->rn]=(bjx2_addr)(ctx->regs[op->rm])+
//		((bjx2_addr)(ctx->regs[op->ro])*4);
}

void BJX2_Op_LEAQ_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v1;
	v1=(bjx2_addr)(ctx->regs[op->rm])+
		((bjx2_addr)(ctx->regs[op->ro])*8);
	v1=v1&0x0000FFFFFFFFFFFFULL;
	ctx->regs[op->rn]=v1;

//	ctx->regs[op->rn]=(bjx2_addr)(ctx->regs[op->rm])+
//		((bjx2_addr)(ctx->regs[op->ro])*8);
}

void BJX2_Op_LEAB_LdPcDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(bjx2_addr)(op->pc2)+op->imm;
}

void BJX2_Op_LEAB_LdPcIdxReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[op->ro]);
}


void BJX2_Op_XLEA_AdjustBounds(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 pslo, pshi, pdlo, pdhi;
	int bslo, bshi, bdlo, bdhi, ex, badj;

	pslo=ctx->regs[op->rm+0];
	pshi=ctx->regs[op->rm+1];
	pdlo=ctx->regs[op->rn+0];
	pdhi=ctx->regs[op->rn+1];

	if((pslo>>60)!=3)
		return;
	
	ex=(pshi>>60)&15;
	bslo=(pshi>>48)&4095;
	bshi=(pslo>>48)&4095;
	
	badj=(int)((pdlo-pslo)>>(ex*2+4));
	
	bslo+=badj;
	bshi-=badj;
	
	if((bslo<0) || (bshi<0))
	{
		pdlo=pdlo&0x0000FFFFFFFFFFFFULL;
		pdlo=pdlo|0x1FFF000000000000ULL;
	}else
	{
		pdlo=pdlo&0x0000FFFFFFFFFFFFULL;
		pdlo=pdlo|0x3000000000000000ULL;
		pdlo=pdlo|(((u64)bshi)<<48);
	}

	ctx->regs[op->rn+0]=pdlo;
	ctx->regs[op->rn+1]=pdhi;
}

void BJX2_Op_XLEAB_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn+0]=(bjx2_addr)(ctx->regs[op->rm+0])+(op->imm);
	ctx->regs[op->rn+1]=(bjx2_addr)(ctx->regs[op->rm+1]);
	BJX2_Op_XLEA_AdjustBounds(ctx, op);
}

void BJX2_Op_XLEAW_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn+0]=(bjx2_addr)(ctx->regs[op->rm+0])+(op->imm*2);
	ctx->regs[op->rn+1]=(bjx2_addr)(ctx->regs[op->rm+1]);
	BJX2_Op_XLEA_AdjustBounds(ctx, op);
}

void BJX2_Op_XLEAD_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn+0]=(bjx2_addr)(ctx->regs[op->rm+0])+(op->imm*4);
	ctx->regs[op->rn+1]=(bjx2_addr)(ctx->regs[op->rm+1]);
	BJX2_Op_XLEA_AdjustBounds(ctx, op);
}

void BJX2_Op_XLEAQ_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn+0]=(bjx2_addr)(ctx->regs[op->rm+0])+(op->imm*8);
	ctx->regs[op->rn+1]=(bjx2_addr)(ctx->regs[op->rm+1]);
	BJX2_Op_XLEA_AdjustBounds(ctx, op);
}

void BJX2_Op_XLEAB_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn+0]=(bjx2_addr)(ctx->regs[op->rm+0])+
		(bjx2_addr)(ctx->regs[op->ro]);
	ctx->regs[op->rn+1]=(bjx2_addr)(ctx->regs[op->rm+1]);
	BJX2_Op_XLEA_AdjustBounds(ctx, op);
}

void BJX2_Op_XLEAW_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn+0]=(bjx2_addr)(ctx->regs[op->rm+0])+
		((bjx2_addr)(ctx->regs[op->ro])*2);
	ctx->regs[op->rn+1]=(bjx2_addr)(ctx->regs[op->rm+1]);
	BJX2_Op_XLEA_AdjustBounds(ctx, op);
}

void BJX2_Op_XLEAD_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn+0]=(bjx2_addr)(ctx->regs[op->rm+0])+
		((bjx2_addr)(ctx->regs[op->ro])*4);
	ctx->regs[op->rn+1]=(bjx2_addr)(ctx->regs[op->rm+1]);
	BJX2_Op_XLEA_AdjustBounds(ctx, op);
}

void BJX2_Op_XLEAQ_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn+0]=(bjx2_addr)(ctx->regs[op->rm+0])+
		((bjx2_addr)(ctx->regs[op->ro])*8);
	ctx->regs[op->rn+1]=(bjx2_addr)(ctx->regs[op->rm+1]);
	BJX2_Op_XLEA_AdjustBounds(ctx, op);
}

void BJX2_Op_MOVZT_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		(ctx->regs[op->rm]&0x0000FFFFFFFFFFFFULL);
}

void BJX2_Op_XMOVZT_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn+0]=
		(ctx->regs[op->rm+0]&0x0000FFFFFFFFFFFFULL);
	ctx->regs[op->rn+1]=
		(ctx->regs[op->rm+1]&0x0000FFFFFFFFFFFFULL);
}

void BJX2_Op_MOVST_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		(ctx->regs[op->rm]&0x0000FFFFFFFFFFFFULL)|
		((((ctx->regs[op->rm]>>47)&1)?0xFFFFULL:0x0000ULL)<<48);
}

void BJX2_Op_XMOVST_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn+0]=
		(ctx->regs[op->rm+0]&0x0000FFFFFFFFFFFFULL)|
		((((ctx->regs[op->rm+0]>>47)&1)?0xFFFFULL:0x0000ULL)<<48);
	ctx->regs[op->rn+1]=
		(ctx->regs[op->rm+1]&0x0000FFFFFFFFFFFFULL)|
		((((ctx->regs[op->rm+1]>>47)&1)?0xFFFFULL:0x0000ULL)<<48);
}


void BJX2_Op_MOVTT_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		(ctx->regs[op->rm]&0x0000FFFFFFFFFFFFULL)|
		((ctx->regs[op->ro]&0xFFFFULL)<<48);
}

void BJX2_Op_MOVTT_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v, vi;
	
	v=ctx->regs[op->rm];
	vi=op->imm;

	if(!(vi&1))
	{
		v=v&0x0FFFFFFFFFFFFFFFULL;
		v=v|(vi&0x1E)<<59;
	}else if(!(vi&2))
	{
		v=v&0x1FFFFFFFFFFFFFFFULL;
		v=v|(vi&0x1C)<<59;
	}else if(!(vi&4))
	{
		v=v&0x3FFFFFFFFFFFFFFFULL;
		v=v|(vi&0x18)<<59;
	}else if(!(vi&8))
	{
		v=v&0x7FFFFFFFFFFFFFFFULL;
		v=v|(vi&0x10)<<59;
	}
	
	ctx->regs[op->rn]=v;
}

void BJX2_Op_XMOVTT_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn+0]=(ctx->regs[op->rn+0]&0x0000FFFFFFFFFFFFULL)|
		((op->imm&0xFFFFULL)<<48);
	ctx->regs[op->rn+1]=(ctx->regs[op->rn+1]&0x0000FFFFFFFFFFFFULL)|
		(((op->imm>>16)&0xFFFFULL)<<48);
}

void BJX2_Op_XMOVTT_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn+0]=(ctx->regs[op->rm+0]&0x0000FFFFFFFFFFFFULL)|
		((op->imm&0xFFFFULL)<<48);
	ctx->regs[op->rn+1]=(ctx->regs[op->rm+1]&0x0000FFFFFFFFFFFFULL)|
		(((op->imm>>16)&0xFFFFULL)<<48);
}


void BJX2_Op_MOVTT_RegImm17Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v, vi;
	
	v=ctx->regs[op->rm];
	vi=op->imm;
	v=(v&0x0000FFFFFFFFFFFFULL)|(vi<<48);
	ctx->regs[op->rn]=v;
}

void BJX2_Op_NOP_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
}

void BJX2_Op_CLRT_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_SETT_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_SR]|=1;
}

void BJX2_Op_CLRS_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_SR]&=~2;
}

void BJX2_Op_SETS_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_SR]|=2;
}

void BJX2_Op_NOTT_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_SR]^=1;
}

void BJX2_Op_NOTS_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_SR]^=2;
}


void BJX2_Op_CMPPL_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((s32)ctx->regs[op->rn])>0)
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPPZ_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(((s32)ctx->regs[op->rn])>=0)
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_EXTUB_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(byte)ctx->regs[op->rn];
}

void BJX2_Op_EXTUW_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(u16)ctx->regs[op->rn];
}

void BJX2_Op_EXTSB_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(sbyte)ctx->regs[op->rn];
}

void BJX2_Op_EXTSW_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(s16)ctx->regs[op->rn];
}

void BJX2_Op_EXTUL_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(u32)ctx->regs[op->rn];
}

void BJX2_Op_EXTSL_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(s32)ctx->regs[op->rn];
}


void BJX2_Op_EXTUB_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(byte)ctx->regs[op->rm];
}

void BJX2_Op_EXTUW_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(u16)ctx->regs[op->rm];
}

void BJX2_Op_EXTSB_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(sbyte)ctx->regs[op->rm];
}

void BJX2_Op_EXTSW_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(s16)ctx->regs[op->rm];
}

void BJX2_Op_EXTUL_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(u32)ctx->regs[op->rm];
}

void BJX2_Op_EXTSL_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(s32)ctx->regs[op->rm];
}

void BJX2_Op_CONVFXI_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		( ctx->regs[op->rm] &
			0x3FFFFFFFFFFFFFFFULL ) | 
			0x4000000000000000ULL ;
}

void BJX2_Op_CONVFLI_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		(ctx->regs[op->rm]>>2) |
		0x8000000000000000ULL ;
}

void BJX2_Op_SNIPEDC_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=0x10000 | ((u16)ctx->regs[op->rm]);
}

void BJX2_Op_SNIPEIC_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=0x30000 | ((u16)ctx->regs[op->rm]);
}


void BJX2_Op_SHLL_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rn]<<op->imm;
}

void BJX2_Op_SHLR_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=((u64)(ctx->regs[op->rn]))>>op->imm;
}

void BJX2_Op_SHAR_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=((s64)(ctx->regs[op->rn]))>>op->imm;
}

void BJX2_Op_NOT_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=~ctx->regs[op->rn];
}

void BJX2_Op_NEG_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=-ctx->regs[op->rn];
}

//void BJX2_Op_NEG_RegDr(BJX2_Context *ctx, BJX2_Opcode *op)
//{
//	ctx->regs[BJX2_REG_DLR]=-ctx->regs[op->rn];
//}

void BJX2_Op_NEGC_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(~ctx->regs[op->rn])+
		(~(ctx->regs[BJX2_REG_SR]&1));
}

void BJX2_Op_NOT_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=~ctx->regs[op->rm];
}

void BJX2_Op_NEG_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=-ctx->regs[op->rm];
}

void BJX2_Op_ROTL_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		(ctx->regs[op->rn]<<1)|
		(ctx->regs[op->rn]>>31);
}

void BJX2_Op_ROTR_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		(ctx->regs[op->rn]>>1)|
		(ctx->regs[op->rn]<<31);
}

void BJX2_Op_ROTCL_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 sr;
	sr=ctx->regs[BJX2_REG_SR];
	ctx->regs[BJX2_REG_SR]=sr&(~1);
	ctx->regs[BJX2_REG_SR]|=((ctx->regs[op->rn]>>31)&1);
	ctx->regs[op->rn]=
		(ctx->regs[op->rn]<<1) | (sr&1);
}

void BJX2_Op_ROTCR_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 sr;
	sr=ctx->regs[BJX2_REG_SR];
	ctx->regs[BJX2_REG_SR]=sr&(~1);
	ctx->regs[BJX2_REG_SR]|=((ctx->regs[op->rn]   )&1);
	ctx->regs[op->rn]=
		(((u32)ctx->regs[op->rn])>>1) | ((sr&1)<<31);
}

void BJX2_Op_ROTCLQ_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 sr;
	sr=ctx->regs[BJX2_REG_SR];
	ctx->regs[BJX2_REG_SR]=sr&(~1);
	ctx->regs[BJX2_REG_SR]|=((ctx->regs[op->rn]>>63)&1);
	ctx->regs[op->rn]=
		(ctx->regs[op->rn]<<1) | (sr&1);
}

void BJX2_Op_ROTCRQ_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 sr;
	sr=ctx->regs[BJX2_REG_SR];
	ctx->regs[BJX2_REG_SR]=sr&(~1);
	ctx->regs[BJX2_REG_SR]|=((ctx->regs[op->rn]   )&1);
	ctx->regs[op->rn]=
		(ctx->regs[op->rn]>>1) | ((sr&1)<<63);
}

void BJX2_Op_SHLL_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_SR]&=(~1);
	ctx->regs[BJX2_REG_SR]|=((ctx->regs[op->rn]>>31)&1);
	ctx->regs[op->rn]=ctx->regs[op->rn]<<1;
}

void BJX2_Op_SHLR_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_SR]&=(~1);
	ctx->regs[BJX2_REG_SR]|=((ctx->regs[op->rn]    )&1);
	ctx->regs[op->rn]=ctx->regs[op->rn]>>1;
}

void BJX2_Op_SHAR_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_SR]&=(~1);
	ctx->regs[BJX2_REG_SR]|=((ctx->regs[op->rn]>>31)&1);
	ctx->regs[op->rn]=((s32)ctx->regs[op->rn])>>1;
}

void BJX2_Op_MOVT_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=(ctx->regs[BJX2_REG_SR]&1);
}

void BJX2_Op_MOVNT_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=!(ctx->regs[BJX2_REG_SR]&1);
}

u32 BJX2_OpI_GenRegChk(BJX2_Context *ctx)
{
	u64 v0;

	v0=	ctx->regs[BJX2_REG_R8] ^
		ctx->regs[BJX2_REG_R9] ^
		ctx->regs[BJX2_REG_R10] ^
		ctx->regs[BJX2_REG_R11] ^
		ctx->regs[BJX2_REG_R12] ^
		ctx->regs[BJX2_REG_R13] ^
		ctx->regs[BJX2_REG_R14] ^
		ctx->regs[BJX2_REG_R24] ^
		ctx->regs[BJX2_REG_R25] ^
		ctx->regs[BJX2_REG_R26] ^
		ctx->regs[BJX2_REG_R27] ^
		ctx->regs[BJX2_REG_R28] ^
		ctx->regs[BJX2_REG_R29] ^
		ctx->regs[BJX2_REG_R30] ^
		ctx->regs[BJX2_REG_R31] ^
		ctx->regs[BJX2_REG_R40] ^
		ctx->regs[BJX2_REG_R41] ^
		ctx->regs[BJX2_REG_R42] ^
		ctx->regs[BJX2_REG_R43] ^
		ctx->regs[BJX2_REG_R44] ^
		ctx->regs[BJX2_REG_R45] ^
		ctx->regs[BJX2_REG_R46] ^
		ctx->regs[BJX2_REG_R47] ^
		ctx->regs[BJX2_REG_R56] ^
		ctx->regs[BJX2_REG_R57] ^
		ctx->regs[BJX2_REG_R58] ^
		ctx->regs[BJX2_REG_R59] ^
		ctx->regs[BJX2_REG_R60] ^
		ctx->regs[BJX2_REG_R61] ^
		ctx->regs[BJX2_REG_R62] ^
		ctx->regs[BJX2_REG_R63] ;
	v0=v0^(v0>>32);
	
	return(v0);
}

void BJX2_Op_REGCHKG_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v;
	v=BJX2_OpI_GenRegChk(ctx);
	ctx->regs[op->rn]=((u32)(ctx->regs[op->rn]))|(v<<32);
}

void BJX2_Op_REGCHKC_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1;
	v0=BJX2_OpI_GenRegChk(ctx);
	v1=ctx->regs[op->rn]>>32;
//	ctx->regs[op->rn]=((u32)(ctx->regs[op->rn]))|(v<<32);
	if(v0!=v1)
	{
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BOUNDCHK);
	}
}

void BJX2_Op_SWAPB_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		((ctx->regs[op->rm]&0xFFFFFFFF00000000ULL)   ) |
		((ctx->regs[op->rm]&0x00000000FF00FF00ULL)>>8) |
		((ctx->regs[op->rm]&0x0000000000FF00FFULL)<<8);
}

void BJX2_Op_SWAP8B_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		((ctx->regs[op->rm]&0xFF00FF00FF00FF00ULL)>>8) |
		((ctx->regs[op->rm]&0x00FF00FF00FF00FFULL)<<8);
}

void BJX2_Op_SWAPW_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		((ctx->regs[op->rm]&0xFFFF0000FFFF0000ULL)>>16) |
		((ctx->regs[op->rm]&0x0000FFFF0000FFFFULL)<<16);
}

void BJX2_Op_SWAPL_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		((ctx->regs[op->rm]&0xFFFFFFFF00000000ULL)>>16) |
		((ctx->regs[op->rm]&0x00000000FFFFFFFFULL)<<16);
}

void BJX2_Op_SWAPLB_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		((ctx->regs[op->rm]&0xFFFFFFFFFFFF0000ULL)   ) |
		((ctx->regs[op->rm]&0x000000000000FF00ULL)>>8) |
		((ctx->regs[op->rm]&0x00000000000000FFULL)<<8);
}

void BJX2_Op_SWAPLW_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		((ctx->regs[op->rm]&0xFFFFFFFF00000000ULL)    ) |
		((ctx->regs[op->rm]&0x00000000FFFF0000ULL)>>16) |
		((ctx->regs[op->rm]&0x000000000000FFFFULL)<<16);
}

void BJX2_Op_SWAPMB_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		((ctx->regs[op->rm]&0xFFFFFFFFFF00FF00ULL)    ) |
		((ctx->regs[op->rm]&0x0000000000FF0000ULL)>>16) |
		((ctx->regs[op->rm]&0x00000000000000FFULL)<<16);
}

void BJX2_Op_SWAPMW_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		((ctx->regs[op->rm]&0xFFFF0000FFFF0000ULL)    ) |
		((ctx->regs[op->rm]&0x0000FFFF00000000ULL)>>32) |
		((ctx->regs[op->rm]&0x000000000000FFFFULL)<<32);
}

void BJX2_Op_SWAPHB_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		((ctx->regs[op->rm]&0xFFFFFFFF00FFFF00ULL)   ) |
		((ctx->regs[op->rm]&0x00000000FF000000ULL)>>24) |
		((ctx->regs[op->rm]&0x00000000000000FFULL)<<24);
}

void BJX2_Op_SWAPHW_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		((ctx->regs[op->rm]&0x0000FFFFFFFF0000ULL)    ) |
		((ctx->regs[op->rm]&0xFFFF000000000000ULL)>>48) |
		((ctx->regs[op->rm]&0x000000000000FFFFULL)<<48);
}


#if 1

void BJX2_Op_SWCPLB_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		((ctx->regs[op->rm]&0xFFFFFFFFFFFF00FFULL)   ) |
		((ctx->regs[op->rm]&0x00000000000000FFULL)<<8);
}

void BJX2_Op_SWCPLW_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		((ctx->regs[op->rm]&0xFFFFFFFF0000FFFFULL)    ) |
		((ctx->regs[op->rm]&0x000000000000FFFFULL)<<16);
}

void BJX2_Op_SWCPMB_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		((ctx->regs[op->rm]&0xFFFFFFFFFF00FFFFULL)    ) |
		((ctx->regs[op->rm]&0x00000000000000FFULL)<<16);
}

void BJX2_Op_SWCPMW_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		((ctx->regs[op->rm]&0xFFFF0000FFFFFFFFULL)    ) |
		((ctx->regs[op->rm]&0x000000000000FFFFULL)<<32);
}

void BJX2_Op_SWCPHB_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		((ctx->regs[op->rm]&0xFFFFFFFF00FFFFFFULL)   ) |
		((ctx->regs[op->rm]&0x00000000000000FFULL)<<24);
}

void BJX2_Op_SWCPHW_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		((ctx->regs[op->rm]&0x0000FFFFFFFFFFFFULL)    ) |
		((ctx->regs[op->rm]&0x000000000000FFFFULL)<<48);
}
#endif


void BJX2_Op_SHAD_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[op->rm];
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			((s32)ctx->regs[op->rn])<<(shl&31);
	}else
	{
		ctx->regs[op->rn]=
			((s32)ctx->regs[op->rn])>>((-shl)&31);
	}
}

void BJX2_Op_SHLD_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[op->rm];
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			((u32)ctx->regs[op->rn])<<(shl&31);
	}else
	{
		ctx->regs[op->rn]=
			((u32)ctx->regs[op->rn])>>((-shl)&31);
	}
}


void BJX2_Op_SHADQ_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[op->rm];
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			ctx->regs[op->rn]<<(shl&63);
	}else
	{
		ctx->regs[op->rn]=
			((s64)ctx->regs[op->rn])>>((-shl)&63);
	}
}

void BJX2_Op_SHLDQ_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[op->rm];
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			ctx->regs[op->rn]<<(shl&63);
	}else
	{
		ctx->regs[op->rn]=
			((u64)ctx->regs[op->rn])>>((-shl)&63);
	}
}

void BJX2_Op_CLZ_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 v;
	int n;
	
	v=(u32)ctx->regs[op->rm];

	if(!v)
	{
		ctx->regs[op->rn]=32;
		return;
	}

	n=0;
	if(!(v&0xFFFF0000U))
		{ v<<=16; n+=16; }
	if(!(v&0xFF000000U))
		{ v<<=8; n+=8; }
	if(!(v&0xF0000000U))
		{ v<<=4; n+=4; }
	if(!(v&0xC0000000U))
		{ v<<=2; n+=2; }
	if(!(v&0x80000000U))
		{ v<<=1; n+=1; }
	ctx->regs[op->rn]=n;
}

void BJX2_Op_CLZQ_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v;
	int n;
	
	v=(u64)ctx->regs[op->rm];

	if(!v)
	{
		ctx->regs[op->rn]=64;
		return;
	}

	n=0;
	if(!(v&0xFFFFFFFF00000000ULL))
		{ v<<=32; n+=32; }
	if(!(v&0xFFFF000000000000ULL))
		{ v<<=16; n+=16; }
	if(!(v&0xFF00000000000000ULL))
		{ v<<=8; n+=8; }
	if(!(v&0xF000000000000000ULL))
		{ v<<=4; n+=4; }
	if(!(v&0xC000000000000000ULL))
		{ v<<=2; n+=2; }
	if(!(v&0x8000000000000000ULL))
		{ v<<=1; n+=1; }
	ctx->regs[op->rn]=n;
}

void BJX2_Op_CTZ_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 v;
	int n;
	
	v=(u32)ctx->regs[op->rm];

	if(!v)
	{
		ctx->regs[op->rn]=32;
		return;
	}

	n=0;
	if(!(v&0x0000FFFFU))
		{ v>>=16; n+=16; }
	if(!(v&0x000000FFU))
		{ v>>=8; n+=8; }
	if(!(v&0x0000000FU))
		{ v>>=4; n+=4; }
	if(!(v&0x00000003U))
		{ v>>=2; n+=2; }
	if(!(v&0x00000001U))
		{ v>>=1; n+=1; }
	ctx->regs[op->rn]=n;
}

void BJX2_Op_CTZQ_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v;
	int n;
	
	v=(u64)ctx->regs[op->rm];

	if(!v)
	{
		ctx->regs[op->rn]=64;
		return;
	}

	n=0;
	if(!(v&0x00000000FFFFFFFFULL))
		{ v>>=32; n+=32; }
	if(!(v&0x000000000000FFFFULL))
		{ v>>=16; n+=16; }
	if(!(v&0x00000000000000FFULL))
		{ v>>=8; n+=8; }
	if(!(v&0x000000000000000FULL))
		{ v>>=4; n+=4; }
	if(!(v&0x0000000000000003ULL))
		{ v>>=2; n+=2; }
	if(!(v&0x0000000000000001ULL))
		{ v>>=1; n+=1; }
	ctx->regs[op->rn]=n;
}

int BJX2_BTRNS_U8(int x)
{
	static char trans4[16]={0,8,4,12,2,10,6,14, 1,9,5,13,3,11,7,15};
	return(
		(((int)trans4[(x   )&15])<<4)	|
		 ((int)trans4[(x>>4)&15])		);
}

int BJX2_BTRNS_U16(int x)
{
	return(
		(((u32)BJX2_BTRNS_U8(x    ))<<8)	|
		 ((u32)BJX2_BTRNS_U8(x>>8))		);
}

u32 BJX2_BTRNS_U32(u32 x)
{
	return(
		(((u32)BJX2_BTRNS_U16(x    ))<<16)	|
		 ((u32)BJX2_BTRNS_U16(x>>16))		);
}


u64 BJX2_BTRNS_U64(u64 x)
{
	return(
		(((u64)BJX2_BTRNS_U32(x    ))<<32)	|
		 ((u64)BJX2_BTRNS_U32(x>>32))		);
}

void BJX2_Op_BTRNS_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 v, v1;
	v=(u32)ctx->regs[op->rm];
	v1=BJX2_BTRNS_U32(v);
	ctx->regs[op->rn]=v1;
}

void BJX2_Op_BTRNSQ_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v, v1;
	v=(u64)ctx->regs[op->rm];
	v1=BJX2_BTRNS_U64(v);
	ctx->regs[op->rn]=v1;
}


u32 BJX2_PMORT_U16(u16 x)
{
	const byte pmort_exptab[16] = {
		0x00, 0x01, 0x04, 0x05,  0x10, 0x11, 0x14, 0x15,
		0x40, 0x41, 0x44, 0x45,  0x50, 0x51, 0x54, 0x55
	};
	u32 x1;

	x1=	(pmort_exptab[(x    )&15]    ) |
		(pmort_exptab[(x>> 4)&15]<< 8) |
		(pmort_exptab[(x>> 8)&15]<<16) |
		(pmort_exptab[(x>>12)&15]<<24) ;
	return(x1);

#if 0
	x1=0;
	if(x&0x0001)x1|=0x00000001;
	if(x&0x0002)x1|=0x00000004;
	if(x&0x0004)x1|=0x00000010;
	if(x&0x0008)x1|=0x00000040;
	if(x&0x0010)x1|=0x00000100;
	if(x&0x0020)x1|=0x00000400;
	if(x&0x0040)x1|=0x00001000;
	if(x&0x0080)x1|=0x00004000;
	if(x&0x0100)x1|=0x00010000;
	if(x&0x0200)x1|=0x00040000;
	if(x&0x0400)x1|=0x00100000;
	if(x&0x0800)x1|=0x00400000;
	if(x&0x1000)x1|=0x01000000;
	if(x&0x2000)x1|=0x04000000;
	if(x&0x4000)x1|=0x10000000;
	if(x&0x8000)x1|=0x40000000;
	return(x1);
#endif
}

u64 BJX2_PMORT_U32(u32 x)
{
	u64 x1;
	x1=(((u64)(BJX2_PMORT_U16(x>>16)))<<32)	|
		((u64)(BJX2_PMORT_U16(x    )))		;
	return(x1);
}

void BJX2_Op_PMORTL_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v, v1;
	v=(u64)ctx->regs[op->rm];
	v1=	(BJX2_PMORT_U16(v>>48)<<1)|
		(BJX2_PMORT_U16(v>>16)   );
	ctx->regs[op->rn]=v1;
}

void BJX2_Op_PMORTQ_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v, v1;
	v=(u64)ctx->regs[op->rm];
	v1=	(BJX2_PMORT_U32(v>>32)<<1)|
		(BJX2_PMORT_U32(v    )   );
	ctx->regs[op->rn]=v1;
}


void BJX2_Op_SHAD_RegDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[BJX2_REG_DLR];
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			((s32)ctx->regs[op->rm])<<(shl&31);
	}else
	{
		ctx->regs[op->rn]=
			((s32)ctx->regs[op->rm])>>((-shl)&31);
	}
}

void BJX2_Op_SHLD_RegDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[BJX2_REG_DLR];
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			((u32)ctx->regs[op->rm])<<(shl&31);
	}else
	{
		ctx->regs[op->rn]=
			((u32)ctx->regs[op->rm])>>((-shl)&31);
	}
}

void BJX2_Op_SHADQ_RegDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[BJX2_REG_DLR];
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			ctx->regs[op->rm]<<(shl&63);
	}else
	{
		ctx->regs[op->rn]=
			((s64)ctx->regs[op->rm])>>((-shl)&63);
	}
}

void BJX2_Op_SHLDQ_RegDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[BJX2_REG_DLR];
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			ctx->regs[op->rm]<<(shl&63);
	}else
	{
		ctx->regs[op->rn]=
			((u64)ctx->regs[op->rm])>>((-shl)&63);
	}
}

void BJX2_Op_SHAD_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)(op->imm);
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			((s32)ctx->regs[op->rm])<<(shl&31);
//			(ctx->regs[op->rm])<<(shl&31);
	}else
	{
		ctx->regs[op->rn]=
			((s32)ctx->regs[op->rm])>>((-shl)&31);
	}
}

void BJX2_Op_SHLD_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)(op->imm);
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			((u32)ctx->regs[op->rm])<<(shl&31);
//			(ctx->regs[op->rm])<<(shl&31);
	}else
	{
		ctx->regs[op->rn]=
			((u32)ctx->regs[op->rm])>>((-shl)&31);
	}
}

void BJX2_Op_SHADQ_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)(op->imm);
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			ctx->regs[op->rm]<<(shl&63);
	}else
	{
		ctx->regs[op->rn]=
			((s64)ctx->regs[op->rm])>>((-shl)&63);
	}
}

void BJX2_Op_SHLDQ_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)(op->imm);
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			ctx->regs[op->rm]<<(shl&63);
	}else
	{
		ctx->regs[op->rn]=
			((u64)ctx->regs[op->rm])>>((-shl)&63);
	}
}

void BJX2_Op_SHAD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[op->ro];
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			((s32)ctx->regs[op->rm])<<(shl&31);
//			(ctx->regs[op->rm])<<(shl&31);
	}else
	{
		ctx->regs[op->rn]=
			((s32)ctx->regs[op->rm])>>((-shl)&31);
	}
}

void BJX2_Op_SHLD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[op->ro];
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			((u32)ctx->regs[op->rm])<<(shl&31);
//			(ctx->regs[op->rm])<<(shl&31);
	}else
	{
		ctx->regs[op->rn]=
			((u32)ctx->regs[op->rm])>>((-shl)&31);
	}
}

void BJX2_Op_SHADQ_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[op->ro];
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			ctx->regs[op->rm]<<(shl&63);
	}else
	{
		ctx->regs[op->rn]=
			((s64)ctx->regs[op->rm])>>((-shl)&63);
	}
}

void BJX2_Op_SHLDQ_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[op->ro];
	if(shl>=0)
	{
		ctx->regs[op->rn]=
			ctx->regs[op->rm]<<(shl&63);
	}else
	{
		ctx->regs[op->rn]=
			((u64)ctx->regs[op->rm])>>((-shl)&63);
	}
}

void BJX2_Op_ROTL_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vc;
	int shl, shr;
	
	va=ctx->regs[op->rm];
	shl=(sbyte)ctx->regs[op->ro];
	shr=-shl;
	if(shl>=0)
		{ vc=(va<<(shl&63))|(va>>(64-(shl&63))); }
	else
		{ vc=va>>(shr&63)|(va<<(64-(shr&63))); }
	ctx->regs[op->rn]=vc;
}

void BJX2_Op_ROTR_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vc;
	int shl, shr;
	
	va=ctx->regs[op->rm];
	shl=(sbyte)ctx->regs[op->ro];
	shl=-shl;
	shr=-shl;
	if(shl>=0)
		{ vc=(va<<(shl&63))|(va>>(64-(shl&63))); }
	else
		{ vc=va>>(shr&63)|(va<<(64-(shr&63))); }
	ctx->regs[op->rn]=vc;
}

#if 1
void BJX2_Op_SHAR_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[op->ro];
	ctx->regs[op->rn]=
		((s32)ctx->regs[op->rm])>>(shl&31);
}

void BJX2_Op_SHLR_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[op->ro];
	ctx->regs[op->rn]=
		((u32)ctx->regs[op->rm])>>(shl&31);
}

void BJX2_Op_SHARQ_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[op->ro];
	ctx->regs[op->rn]=
		((s64)ctx->regs[op->rm])>>(shl&63);
}

void BJX2_Op_SHLRQ_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	
	shl=(sbyte)ctx->regs[op->ro];
	ctx->regs[op->rn]=
		((u64)ctx->regs[op->rm])>>(shl&63);
}
#endif

void BJX2_Op_CSELT_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=
		(ctx->regs[BJX2_REG_SR]&1)?
			ctx->regs[op->rm]:
			ctx->regs[op->ro];
}

void BJX2_Op_PCSELTL_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;
	u32 sa0, sb0, sc0;
	u32 sa1, sb1, sc1;

	va=ctx->regs[op->rm];
	vb=ctx->regs[op->ro];

	sa0=(u32)(va);	sa1=(u32)(va>>32);
	sb0=(u32)(vb);	sb1=(u32)(vb>>32);
	sc0=(ctx->regs[BJX2_REG_SR]&1)?sa0:sb0;
	sc1=(ctx->regs[BJX2_REG_SR]&2)?sa1:sb1;
	vc=sc0 | (((u64)sc1)<<32);

	ctx->regs[op->rn]=vc;
}

void BJX2_Op_PCSELTW_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;
	u16 sa0, sb0, sc0;
	u16 sa1, sb1, sc1;
	u16 sa2, sb2, sc2;
	u16 sa3, sb3, sc3;
	va=ctx->regs[op->rm];	vb=ctx->regs[op->ro];
	sa0=(u16)(va);	sa1=(u16)(va>>16);	sa2=(u16)(va>>32);	sa3=(u16)(va>>48);
	sb0=(u16)(vb);	sb1=(u16)(vb>>16);	sb2=(u16)(vb>>32);	sb3=(u16)(vb>>48);
	sc0=(ctx->regs[BJX2_REG_SR]& 16)?sa0:sb0;
	sc1=(ctx->regs[BJX2_REG_SR]& 32)?sa1:sb1;
	sc2=(ctx->regs[BJX2_REG_SR]& 64)?sa2:sb2;
	sc3=(ctx->regs[BJX2_REG_SR]&128)?sa3:sb3;
	vc=sc0 | (((u64)sc1)<<16) | (((u64)sc2)<<32) | (((u64)sc3)<<48);
	ctx->regs[op->rn]=vc;
}


#if 1
u64 BJX2_OpI_LeaAdjustBoundPtr(BJX2_Context *ctx, u64 base, s64 disp)
{
	u64 v1, v1b;
	u64 v2, v2bi;
	int bstep, lobi, cmsk;
	byte bnd, shl, cbit;

	v1b=base+disp;
	v1b=v1b&0x0000FFFFFFFFFFFFULL;

//	return(v1b);

	if((base>>60)==0)
	{
		/* Non-bounded pointers are stripped. */
		return(v1b);
	}


	bnd=(base>>48)&255;
	if(bnd<0x10)
	{
		lobi=((base>>56)&15);
		v2=bnd;
		v2bi=lobi;
		shl=0;
	}
	else
	{
		lobi=((base>>56)&15);
		shl=((bnd>>3)-1);
		v2=(8|(bnd&7))<<shl;
		v2bi=((u64)lobi)<<shl;
	}
	
	cmsk=(1<<shl)-1;
	cbit=((base&cmsk)+(disp&cmsk))>>shl;
//	bstep=(v1b-base)>>shl;
	bstep=(disp>>shl)+cbit;
	lobi+=bstep;

	if((base>>60)==3)
	{
		if((lobi<0) || (lobi>15))
		{
			return(v1b);
		}
		
		v1=((base>>48)&0xF0FF)|(lobi<<8);
		v1=(v1<<48)|v1b;
		return(v1);
	}

	return(v1b);
}

void BJX2_Op_LEATB_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v1;
	v1=BJX2_OpI_LeaAdjustBoundPtr(ctx, ctx->regs[op->rm], op->imm*1);
	ctx->regs[op->rn]=v1;
}

void BJX2_Op_LEATW_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v1;
	v1=BJX2_OpI_LeaAdjustBoundPtr(ctx, ctx->regs[op->rm], op->imm*2);
	ctx->regs[op->rn]=v1;
}

void BJX2_Op_LEATL_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v1;
	v1=BJX2_OpI_LeaAdjustBoundPtr(ctx, ctx->regs[op->rm], op->imm*4);
	ctx->regs[op->rn]=v1;

//	ctx->regs[op->rn]=(bjx2_addr)(ctx->regs[op->rm])+(op->imm*4);
}

void BJX2_Op_LEATQ_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v1;
	v1=BJX2_OpI_LeaAdjustBoundPtr(ctx, ctx->regs[op->rm], op->imm*8);
	ctx->regs[op->rn]=v1;
}

void BJX2_Op_LEATB_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v1;
	v1=BJX2_OpI_LeaAdjustBoundPtr(ctx,
		ctx->regs[op->rm], (ctx->regs[op->ro])*1);
	ctx->regs[op->rn]=v1;

//	ctx->regs[op->rn]=(bjx2_addr)(ctx->regs[op->rm])+
//		(bjx2_addr)(ctx->regs[op->ro]);
}

void BJX2_Op_LEATW_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v1;
	v1=BJX2_OpI_LeaAdjustBoundPtr(ctx,
		ctx->regs[op->rm], (ctx->regs[op->ro])*2);
	ctx->regs[op->rn]=v1;

//	ctx->regs[op->rn]=(bjx2_addr)(ctx->regs[op->rm])+
//		((bjx2_addr)(ctx->regs[op->ro])*2);
}

void BJX2_Op_LEATL_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v1;
	v1=BJX2_OpI_LeaAdjustBoundPtr(ctx,
		ctx->regs[op->rm], (ctx->regs[op->ro])*4);
	ctx->regs[op->rn]=v1;

//	ctx->regs[op->rn]=(bjx2_addr)(ctx->regs[op->rm])+
//		((bjx2_addr)(ctx->regs[op->ro])*4);
}

void BJX2_Op_LEATQ_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v1;
	v1=BJX2_OpI_LeaAdjustBoundPtr(ctx,
		ctx->regs[op->rm], (ctx->regs[op->ro])*8);
	ctx->regs[op->rn]=v1;

//	ctx->regs[op->rn]=(bjx2_addr)(ctx->regs[op->rm])+
//		((bjx2_addr)(ctx->regs[op->ro])*8);
}
#endif