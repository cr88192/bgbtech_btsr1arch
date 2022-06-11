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

u64 BJX2_FAddSoft(u64 va, u64 vb)
{
	int exa, exb, exc, exd, sga, sgb, sgc;
	u64 fra, frb, frc, vc;
	
	sga=va>>63;
	sgb=vb>>63;
	exa=(va>>52)&2047;
	exb=(vb>>52)&2047;
	
	fra=(va<<4)&0x00FFFFFFFFFFFFFFULL;
	frb=(vb<<4)&0x00FFFFFFFFFFFFFFULL;
	if(exa)
		fra|=0x0100000000000000ULL;
	if(exb)
		frb|=0x0100000000000000ULL;
	
	
	if(exa>=exb)
	{
		exd=exa-exb;
		if(exd>=64)
			exd=63;

		sgc=sga;
		exc=exa;
		if(sga^sgb)
			frc=fra-(frb>>exd);
		else
			frc=fra+(frb>>exd);
	}else
	{
		exd=exb-exa;
		if(exd>=64)
			exd=63;

		sgc=sgb;
		exc=exb;
		if(sga^sgb)
			frc=frb-(fra>>exd);
		else
			frc=frb+(fra>>exd);
	}
	
	if(frc>>63)
	{
		sgc=!sgc;
		frc=-frc;
	}
	
	if(!frc)
		return(0);
	
	if(frc>>57)
	{
		frc>>=1; exc++;
	}else
	{
		while(!(frc>>48))
			{ frc<<=8; exc-=8; }
		if(!(frc>>52))
			{ frc<<=4; exc-=4; }
		while(!(frc>>56))
			{ frc<<=1; exc--; }
	}
	
	if(exc<=0)
	{
		exc=0;
		frc=0;
	}else
		if(exc>=2047)
	{
		exc=2047;
		frc=0;
	}

	if((frc>>12)==((frc+8)>>12))
		frc=(frc+8)>>4;
	else
		frc=frc>>4;

	frc&=0x000FFFFFFFFFFFFFULL;
	vc=(((u64)sgc)<<63) | (((u64)exc)<<52) | frc;
	return(vc);
}

u64 BJX2_FSubSoft(u64 va, u64 vb)
{
	return(BJX2_FAddSoft(va, vb^0x8000000000000000ULL));
}

u64 BJX2_FMulSoft(u64 va, u64 vb)
{
	int exa, exb, exc, exd, sga, sgb, sgc;
	u64 fra, frb, frc, vc;
	
	if(!va || !vb)
		return(0);
	
	sga=va>>63;
	sgb=vb>>63;
	exa=(va>>52)&2047;
	exb=(vb>>52)&2047;
	
	fra=(va<<8)&0x0FFFFFFFFFFFFFFFULL;
	frb=(vb<<8)&0x0FFFFFFFFFFFFFFFULL;
	fra|=0x1000000000000000ULL;
	frb|=0x1000000000000000ULL;
	
	sgc=sga^sgb;
	exc=exa+exb-1023;
	
	if(exc<=0)
		return(0);
	
	frc=((fra>>32)*((frb)>>32))	+
		((fra>>32)*((u32)frb)>>32) +
		((frb>>32)*((u32)fra)>>32) ;

	if(frc>>57)
	{
		frc>>=1; exc++;
	}
	
	if(exc>=2047)
	{
		exc=2047;
		frc=0;
	}

	if((frc>>12)==((frc+8)>>12))
		frc=(frc+8)>>4;
	else
		frc=frc>>4;

	frc&=0x000FFFFFFFFFFFFFULL;
	vc=(((u64)sgc)<<63) | (((u64)exc)<<52) | frc;
	return(vc);
}

#if 1
void BJX2_Op_FADDD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=BJX2_FAddSoft(ctx->regs[op->rm], ctx->regs[op->ro]);

//	double a, b, c;
//	a=BJX2_PtrGetDoubleIx(ctx->regs, op->rm);
//	b=BJX2_PtrGetDoubleIx(ctx->regs, op->ro);
//	c=a+b;
//	BJX2_PtrSetDoubleIx(ctx->regs, op->rn, c);
}

void BJX2_Op_FSUBD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=BJX2_FSubSoft(ctx->regs[op->rm], ctx->regs[op->ro]);

//	double a, b, c;
//	a=BJX2_PtrGetDoubleIx(ctx->regs, op->rm);
//	b=BJX2_PtrGetDoubleIx(ctx->regs, op->ro);
//	c=a-b;
//	BJX2_PtrSetDoubleIx(ctx->regs, op->rn, c);
}

void BJX2_Op_FMULD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=BJX2_FMulSoft(ctx->regs[op->rm], ctx->regs[op->ro]);

//	double a, b, c;
//	a=BJX2_PtrGetDoubleIx(ctx->regs, op->rm);
//	b=BJX2_PtrGetDoubleIx(ctx->regs, op->ro);
//	c=a*b;
//	BJX2_PtrSetDoubleIx(ctx->regs, op->rn, c);
}

void BJX2_Op_FDIVD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double a, b, c;
	
	a=BJX2_PtrGetDoubleIx(ctx->regs, op->rm);
	b=BJX2_PtrGetDoubleIx(ctx->regs, op->ro);
	c=a/b;
	BJX2_PtrSetDoubleIx(ctx->regs, op->rn, c);
}

void BJX2_Op_FDIVAD_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	
	v0=ctx->regs[op->rm];
	v1=ctx->regs[op->ro];
	v2=	(v0-v1)+0x3FEF5D0000000000ULL;
	v2=	((v0^v1)&0x8000000000000000ULL)|
		((v2   )&0x7FFFFF0000000000ULL);
	ctx->regs[op->rn]=v2;

//	double a, b, c;
//	a=BJX2_PtrGetDoubleIx(ctx->regs, op->rm);
//	b=BJX2_PtrGetDoubleIx(ctx->regs, op->ro);
//	c=a/b;
//	BJX2_PtrSetDoubleIx(ctx->regs, op->rn, c);
}

void BJX2_Op_FDIVXA_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	
	v0=ctx->regs[op->rm+1];
	v1=ctx->regs[op->ro+1];
	v2=	(v0-v1)+0x3FFEF5D000000000ULL;
	v2=	((v0^v1)&0x8000000000000000ULL)|
		((v2   )&0x7FFFFFF000000000ULL);
	ctx->regs[op->rn+0]=0;
	ctx->regs[op->rn+1]=v2;
}
#endif


#if 1
#ifdef _MSC_VER
#pragma fenv_access(on)
#endif

void BJX2_Op_FADDG_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
#ifndef _MSC_VER
	#pragma STDC FENV_ACCESS ON
#endif
	fenv_t tfenv;
	int rm;
	double a, b, c;
	
	switch((ctx->regs[BJX2_REG_GBR]>>48)&15)
	{
	case 0:		rm=FE_TONEAREST;	break;
	case 1:		rm=FE_TOWARDZERO;	break;
	case 2:		rm=FE_DOWNWARD;		break;
	case 3:		rm=FE_UPWARD;		break;
	default:	rm=FE_TOWARDZERO;	break;
	}
	
	fegetenv(&tfenv);
	feclearexcept(FE_ALL_EXCEPT);
	fesetround(rm);
	
	a=BJX2_PtrGetDoubleIx(ctx->regs, op->rm);
	b=BJX2_PtrGetDoubleIx(ctx->regs, op->ro);
	c=a+b;
	BJX2_PtrSetDoubleIx(ctx->regs, op->rn, c);

	if(fetestexcept(FE_INEXACT))
		ctx->regs[BJX2_REG_GBR]|=(0x0100ULL<<48);

	fesetenv(&tfenv);
}

void BJX2_Op_FSUBG_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
#ifndef _MSC_VER
	#pragma STDC FENV_ACCESS ON
#endif
	fenv_t tfenv;
	int rm;
	double a, b, c;
	
	switch((ctx->regs[BJX2_REG_GBR]>>48)&15)
	{
	case 0:		rm=FE_TONEAREST;	break;
	case 1:		rm=FE_TOWARDZERO;	break;
	case 2:		rm=FE_DOWNWARD;		break;
	case 3:		rm=FE_UPWARD;		break;
	default:	rm=FE_TOWARDZERO;	break;
	}
	
	fegetenv(&tfenv);
	feclearexcept(FE_ALL_EXCEPT);
	fesetround(rm);
	
	a=BJX2_PtrGetDoubleIx(ctx->regs, op->rm);
	b=BJX2_PtrGetDoubleIx(ctx->regs, op->ro);
	c=a-b;
	BJX2_PtrSetDoubleIx(ctx->regs, op->rn, c);

	if(fetestexcept(FE_INEXACT))
		ctx->regs[BJX2_REG_GBR]|=(0x0100ULL<<48);

	fesetenv(&tfenv);
}

void BJX2_Op_FMULG_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
#ifndef _MSC_VER
	#pragma STDC FENV_ACCESS ON
#endif
	fenv_t tfenv;
	int rm;
	double a, b, c;
	
	switch((ctx->regs[BJX2_REG_GBR]>>48)&15)
	{
	case 0:		rm=FE_TONEAREST;	break;
	case 1:		rm=FE_TOWARDZERO;	break;
	case 2:		rm=FE_DOWNWARD;		break;
	case 3:		rm=FE_UPWARD;		break;
	default:	rm=FE_TOWARDZERO;	break;
	}
	
	fegetenv(&tfenv);
	feclearexcept(FE_ALL_EXCEPT);
	fesetround(rm);
	
	a=BJX2_PtrGetDoubleIx(ctx->regs, op->rm);
	b=BJX2_PtrGetDoubleIx(ctx->regs, op->ro);
	c=a*b;
	BJX2_PtrSetDoubleIx(ctx->regs, op->rn, c);

	if(fetestexcept(FE_INEXACT))
		ctx->regs[BJX2_REG_GBR]|=(0x0100ULL<<48);

	fesetenv(&tfenv);
}

#ifdef _MSC_VER
#pragma fenv_access(off)
#endif

#endif



u32 BJX2_CvtHalfToFloat(u16 v)
{
	u32 v1;
	int ex, ex0, fr, sg;
	
	if(!v)
		return(0);

	sg=(v>>15)&1;
	ex0=(v>>10)&31;
	fr=v&1023;
	ex=(ex0-15)+127;
	if(ex0==0)ex=0;
	if(ex0==31)ex=255;
	fr=fr<<13;
	v1=(sg<<31)|(ex<<23)|fr;
	return(v1);
}

u16 BJX2_CvtFloatToHalf(u32 v)
{
	u32 v1;
	int ex, fr, sg;

	if(!v)
		return(0);

	sg=(v>>31)&1;
	ex=(v>>23)&255;
	fr=v&0x7FFFFF;
	ex=(ex-127)+15;

//	fr=fr>>13;

	fr=fr>>12;

	if((fr>>9)==((fr+1)>>9))
		fr=(fr+1)>>1;
	else
		fr=fr>>1;

	if(ex<0)
		return(0);
//	if(ex>=15)
	if(ex>=31)
		return(0x7C00|(sg<<15));
	v1=(sg<<15)|(ex<<10)|fr;
	return(v1);
}

u64 BJX2_CvtFloatToDouble(u32 v)
{
	u64 v1;
	s64 ex, fr, sg;

	if(!v)
		return(0);

	sg=(v>>31)&1;
	ex=(v>>23)&255;
	fr=v&0x7FFFFF;

	if(ex==255)
		{ ex=2047; }
	else if(ex!=0)
		{ ex=(ex-127)+1023; }

	fr=fr<<29;

	v1=(sg<<63)|(ex<<52)|fr;
	return(v1);
}

u32 BJX2_CvtDoubleToFloat(u64 v)
{
	u32 v1;
	int ex, ex0, fr, sg;
	
	if(!v)
		return(0);

	sg=(v>>63)&1;
	ex0=(v>>52)&2047;
	fr=(v>>28)&0x00FFFFFF;

	if((fr>>9)==((fr+1)>>9))
		fr=(fr+1)>>1;
	else
		fr=fr>>1;
	
	ex=(ex0-1023)+127;

	if(ex0==0)ex=0;

	if(ex<0)
		return(0);

	if(ex0==2047)
	{
		ex=255;
	}else
		if(ex>=255)
	{
		ex=255;
		fr=0;
	}

	v1=(sg<<31)|(ex<<23)|fr;
	return(v1);
}

#if 1
void BJX2_Op_FLDCF_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 v;
	v=ctx->regs[op->rm];
	ctx->regs[op->rn]=BJX2_CvtFloatToDouble(v);
//	BJX2_PtrSetDoubleIx(ctx->regs, op->rn, BJX2_PtrGetFloat(&v));
}

void BJX2_Op_FSTCF_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	float sf;
//	u32 v;
//	sf=BJX2_PtrGetDoubleIx(ctx->regs, op->rm);
//	BJX2_PtrSetFloat(&v, sf);
//	ctx->regs[op->rn]=v;

	ctx->regs[op->rn]=BJX2_CvtDoubleToFloat(ctx->regs[op->rm]);
}

void BJX2_Op_FLDCI_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v;
	v=ctx->regs[op->rm];
	BJX2_PtrSetDoubleIx(ctx->regs, op->rn, (s64)v);
}

void BJX2_Op_FSTCI_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double f;
	f=BJX2_PtrGetDoubleIx(ctx->regs, op->rm);
	ctx->regs[op->rn]=(s64)f;
}

void BJX2_Op_FLDCH_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 v;
	v=ctx->regs[op->rm];
	v=BJX2_CvtHalfToFloat(v);
	ctx->regs[op->rn]=BJX2_CvtFloatToDouble(v);
//	BJX2_PtrSetDoubleIx(ctx->regs, op->rn, BJX2_PtrGetFloat(&v));
}

void BJX2_Op_FSTCH_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float sf;
	u32 v;
//	sf=BJX2_PtrGetDoubleIx(ctx->regs, op->rm);
//	BJX2_PtrSetFloat(&v, sf);
	v=BJX2_CvtDoubleToFloat(ctx->regs[op->rm]);
	v=BJX2_CvtFloatToHalf(v);
	ctx->regs[op->rn]=v;
}

void BJX2_Op_FLDCFH_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 v;
	v=ctx->regs[op->rm]>>32;
//	BJX2_PtrSetDoubleIx(ctx->regs, op->rn, BJX2_PtrGetFloat(&v));
	ctx->regs[op->rn]=BJX2_CvtFloatToDouble(v);
}

void BJX2_Op_FSTCFH_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	float sf;
	u32 v, v0;
//	sf=BJX2_PtrGetDoubleIx(ctx->regs, op->rm);
//	BJX2_PtrSetFloat(&v, sf);
	v=BJX2_CvtDoubleToFloat(ctx->regs[op->rm]);
//	ctx->regs[op->rn]=((u64)v)<<32;
	v0=ctx->regs[op->rn];
	ctx->regs[op->rn]=(((u64)v)<<32)|v0;
}

void BJX2_Op_FNEG_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]^0x8000000000000000ULL;
}

void BJX2_Op_FABS_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn]=ctx->regs[op->rm]&0x7FFFFFFFFFFFFFFFULL;
}

void BJX2_Op_FSQRT_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double sf;
	sf=BJX2_PtrGetDoubleIx(ctx->regs, op->rm);
	if(sf<0)
		sf=-sqrt(-sf);
	else
		sf=sqrt(sf);
	BJX2_PtrSetDoubleIx(ctx->regs, op->rn, sf);
}

void BJX2_Op_FSQRTA_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	v0=ctx->regs[op->rm];
	v1=(v0>>1)+0x2000000000000000ULL;
	v2=	(v0&0x8000000000000000ULL)|
		(v1&0x7FFFFF0000000000ULL);
	ctx->regs[op->rn]=v2;
}

void BJX2_Op_FSQRTXA_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	v0=ctx->regs[op->rm+1];
	v1=(v0>>1)+0x2000000000000000ULL;
	v2=	(v0&0x8000000000000000ULL)|
		(v1&0x7FFFFFF000000000ULL);
	ctx->regs[op->rn+1]=v2;
}

void BJX2_Op_FCMPEQ_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;
	int isnan;
//	double a, b, c;

	va=ctx->regs[op->rn];
	vb=ctx->regs[op->rm];

	isnan=(((va>>52)&2047)==2047) && (((va>>48)&15)!=0);
	
	if((((va>>52)&2047)==0) && (((vb>>52)&2047)==0))
		{ va=0; vb=0; }

//	a=BJX2_PtrGetDoubleIx(ctx->regs, op->rn);
//	b=BJX2_PtrGetDoubleIx(ctx->regs, op->rm);

//	if(a==b)
	if((va==vb) && !isnan)
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_FCMPGT_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 va, vb, vc;
	int isgt;

//	double a, b, c;
	
//	a=BJX2_PtrGetDoubleIx(ctx->regs, op->rn);
//	b=BJX2_PtrGetDoubleIx(ctx->regs, op->rm);

	va=ctx->regs[op->rn];
	vb=ctx->regs[op->rm];

	if(va>>63)
	{
		if(vb>>63)
			{ isgt=vb>va; }
		else
			{ isgt=0; }
	}else
	{
		if(vb>>63)
			{ isgt=1; }
		else
			{ isgt=va>vb; }
	}

//	if(a>b)
	if(isgt)
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_FLDCH_ImmGReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 v;
//	v=ctx->regs[op->rm];
	v=op->imm;
	v=BJX2_CvtHalfToFloat(v);
	ctx->regs[op->rn]=BJX2_CvtFloatToDouble(v);
//	BJX2_PtrSetDoubleIx(ctx->regs, op->rn, BJX2_PtrGetFloat(&v));
}

void BJX2_Op_FLDCF_ImmGReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 v;
	v=op->imm;
	ctx->regs[op->rn]=BJX2_CvtFloatToDouble(v);
//	BJX2_PtrSetDoubleIx(ctx->regs, op->rn, BJX2_PtrGetFloat(&v));
}
#endif


#if 1
void BJX2_Op_FMOVS_GRegStRegDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	float sf;
	u32 v;
	ctx->trapc=op->pc;
//	sf=BJX2_PtrGetDoubleIx(ctx->regs, op->rm);
//	BJX2_PtrSetFloat(&v, sf);
	v=BJX2_CvtDoubleToFloat(ctx->regs[op->rm]);
	BJX2_MemSetDWord(ctx, ctx->regs[op->rn]+(op->imm*4), v);
}

void BJX2_Op_FMOVS_LdRegDispGReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 v;
	ctx->trapc=op->pc;
	v=BJX2_MemGetDWord(ctx, ctx->regs[op->rm]+(op->imm*4));
	ctx->regs[op->rn]=BJX2_CvtFloatToDouble(v);
//	BJX2_PtrSetDoubleIx(ctx->regs, op->rn, BJX2_PtrGetFloat(&v));
}

void BJX2_Op_FMOVS_GRegStRegDisp1(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	float sf;
	u32 v;
	ctx->trapc=op->pc;
//	sf=BJX2_PtrGetDoubleIx(ctx->regs, op->rm);
//	BJX2_PtrSetFloat(&v, sf);
	v=BJX2_CvtDoubleToFloat(ctx->regs[op->rm]);
	BJX2_MemSetDWord(ctx, ctx->regs[op->rn]+(op->imm*1), v);
}

void BJX2_Op_FMOVS_LdRegDisp1GReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 v;
	ctx->trapc=op->pc;
	v=BJX2_MemGetDWord(ctx, ctx->regs[op->rm]+(op->imm*1));
	ctx->regs[op->rn]=BJX2_CvtFloatToDouble(v);
//	BJX2_PtrSetDoubleIx(ctx->regs, op->rn, BJX2_PtrGetFloat(&v));
}

void BJX2_Op_FMOVS_GRegStReg2(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	float sf;
	u32 v;
	ctx->trapc=op->pc;
//	sf=BJX2_PtrGetDoubleIx(ctx->regs, op->rm);
//	BJX2_PtrSetFloat(&v, sf);
	v=BJX2_CvtDoubleToFloat(ctx->regs[op->rm]);
	BJX2_MemSetDWord(ctx, ctx->regs[op->rn]+(ctx->regs[op->ro]*4), v);
}

void BJX2_Op_FMOVS_LdReg2GReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 v;
	ctx->trapc=op->pc;
	v=BJX2_MemGetDWord(ctx, ctx->regs[op->rm]+(ctx->regs[op->ro]*4));
	ctx->regs[op->rn]=BJX2_CvtFloatToDouble(v);
//	BJX2_PtrSetDoubleIx(ctx->regs, op->rn, BJX2_PtrGetFloat(&v));
}

void BJX2_Op_FMOVS_GRegStReg2B(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	float sf;
	u32 v;
	ctx->trapc=op->pc;
//	sf=BJX2_PtrGetDoubleIx(ctx->regs, op->rm);
//	BJX2_PtrSetFloat(&v, sf);
	v=BJX2_CvtDoubleToFloat(ctx->regs[op->rm]);
	BJX2_MemSetDWord(ctx, ctx->regs[op->rn]+(ctx->regs[op->ro]*1), v);
}

void BJX2_Op_FMOVS_LdReg2GRegB(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u32 v;
	ctx->trapc=op->pc;
	v=BJX2_MemGetDWord(ctx, ctx->regs[op->rm]+(ctx->regs[op->ro]*1));
	ctx->regs[op->rn]=BJX2_CvtFloatToDouble(v);
//	BJX2_PtrSetDoubleIx(ctx->regs, op->rn, BJX2_PtrGetFloat(&v));
}
#endif

#if 1

// u16 bjx2_f2h(float f);
// float bjx2_h2f(u16 v0);

void BJX2_Op_FMOVH_GRegStRegDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	float sf;
	u16 v;
//	sf=BJX2_PtrGetDoubleIx(ctx->regs, op->rm);
//	v=bjx2_f2h(sf);
	v=BJX2_CvtFloatToHalf(BJX2_CvtDoubleToFloat(ctx->regs[op->rm]));
	BJX2_MemSetDWord(ctx, ctx->regs[op->rn]+(op->imm*2), v);
}

void BJX2_Op_FMOVH_LdRegDispGReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u16 v;
	v=BJX2_MemGetWord(ctx, ctx->regs[op->rm]+(op->imm*2));
//	BJX2_PtrSetDoubleIx(ctx->regs, op->rn, bjx2_h2f(v));
	ctx->regs[op->rn]=BJX2_CvtFloatToDouble(BJX2_CvtHalfToFloat(v));
}

void BJX2_Op_FMOVH_GRegStReg2(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	float sf;
	u16 v;
//	sf=BJX2_PtrGetDoubleIx(ctx->regs, op->rm);
//	BJX2_PtrSetFloat(&v, sf);
//	v=bjx2_f2h(sf);
	v=BJX2_CvtFloatToHalf(BJX2_CvtDoubleToFloat(ctx->regs[op->rm]));
	BJX2_MemSetWord(ctx, ctx->regs[op->rn]+(ctx->regs[op->ro]*2), v);
}

void BJX2_Op_FMOVH_LdReg2GReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u16 v;
	v=BJX2_MemGetWord(ctx, ctx->regs[op->rm]+(ctx->regs[op->ro]*2));
//	BJX2_PtrSetDoubleIx(ctx->regs, op->rn, bjx2_h2f(v));
	ctx->regs[op->rn]=BJX2_CvtFloatToDouble(BJX2_CvtHalfToFloat(v));
}

void BJX2_Op_FMOVH_GRegStReg2B(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	float sf;
	u16 v;
//	sf=BJX2_PtrGetDoubleIx(ctx->regs, op->rm);
//	BJX2_PtrSetFloat(&v, sf);
//	v=bjx2_f2h(sf);
	v=BJX2_CvtFloatToHalf(BJX2_CvtDoubleToFloat(ctx->regs[op->rm]));
	BJX2_MemSetWord(ctx, ctx->regs[op->rn]+(ctx->regs[op->ro]*1), v);
}

void BJX2_Op_FMOVH_LdReg2GRegB(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u16 v;
	v=BJX2_MemGetWord(ctx, ctx->regs[op->rm]+(ctx->regs[op->ro]*1));
//	BJX2_PtrSetDoubleIx(ctx->regs, op->rn, bjx2_h2f(v));
	ctx->regs[op->rn]=BJX2_CvtFloatToDouble(BJX2_CvtHalfToFloat(v));
}
#endif


#if 1
void BJX2_Op_FDIVX_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double a, b, c;
	
	a=BJX2_PtrGetF128Ix(ctx->regs, op->rm);
	b=BJX2_PtrGetF128Ix(ctx->regs, op->ro);
	c=a/b;
	BJX2_PtrSetF128Ix(ctx->regs, op->rn, c);
}
#endif

#if 1

bjx2_val128 bjx2_negx_v128(bjx2_val128 va)
{
	bjx2_val128 vt;
	int vtcf;
	
	vt.lo=~va.lo;
	vtcf=((vt.lo+1)<vt.lo);
	vt.hi=(~va.hi)+vtcf;
	vt.lo+=1;
	return(vt);
}

bjx2_val128 bjx2_addx_v128(bjx2_val128 va, bjx2_val128 vb)
{
	bjx2_val128 vt;
	int vtcf;
	
	vt.lo=va.lo+vb.lo; vtcf=vt.lo<va.lo;
	vt.hi=(va.hi+vb.hi)+vtcf;
	return(vt);
}

bjx2_val128 bjx2_shrx_v128(bjx2_val128 va, int cnt)
{
	bjx2_val128 vt;

	if(cnt<=0)
		return(va);
	if(cnt>=128)
		{ vt.lo=0; vt.hi=0; return(vt); }

	if(cnt>=64)
	{
		vt.lo=va.hi>>(cnt-64);
		vt.hi=0;
		return(vt);
	}
	
	vt.lo=(va.lo>>cnt)|(va.hi<<(64-cnt));
	vt.hi=va.hi>>cnt;
	return(vt);
}

bjx2_val128 bjx2_shlx_v128(bjx2_val128 va, int cnt)
{
	bjx2_val128 vt;

	if(cnt<=0)
		return(va);
	if(cnt>=128)
		{ vt.lo=0; vt.hi=0; return(vt); }

	if(cnt>=64)
	{
		vt.hi=va.lo<<(cnt-64);
		vt.lo=0;
		return(vt);
	}
	
	vt.hi=(va.hi<<cnt)|(va.lo>>(64-cnt));
	vt.lo=va.lo<<cnt;
	return(vt);
}


bjx2_val128 bjx2_mulxhi_v128(bjx2_val128 va, bjx2_val128 vb)
{
	bjx2_val128 vs, vt, vu, vv, vx, vy;
	u64 a, b, c, d, e, f, g, h;
	u64 ae, af, ag, ah;
	u64 be, bf, bg, bh;
	u64 ce, cf, cg, ch;
	u64 de, df, dg, dh;
	int vtcf;
	
	a=(u32)(va.hi>>32);		b=(u32)(va.hi>> 0);
	c=(u32)(va.lo>>32);		d=(u32)(va.lo>> 0);
	e=(u32)(vb.hi>>32);		f=(u32)(vb.hi>> 0);
	g=(u32)(vb.lo>>32);		h=(u32)(vb.lo>> 0);
	
//	ae=a*e;		af=a*f;		ag=a*g;		ah=a*h;
//	be=b*e;		bf=b*f;		bg=b*g;		bh=b*h;
//	ce=c*e;		cf=c*f;		cg=c*g;		ch=c*h;
//	de=d*e;		df=d*f;		dg=d*g;		dh=d*h;

	ae=a*e;		af=a*f;		ag=a*g;		ah=a*h;
	be=b*e;		bf=b*f;		bg=b*g;
	ce=c*e;		cf=c*f;
	de=d*e;
	
	vs.hi=ae;
	vs.lo=bf;
	
	vu.hi=0;	vu.lo=ce;
	vv.hi=0;	vv.lo=ag;
	vt=bjx2_addx_v128(vu, vv);
	vs=bjx2_addx_v128(vs, vt);

	vx.hi=0;	vx.lo=ah;
	vy.hi=0;	vy.lo=bg;
	vu=bjx2_addx_v128(vx, vy);

	vx.hi=0;	vx.lo=de;
	vy.hi=0;	vy.lo=cf;
	vv=bjx2_addx_v128(vx, vy);

	vu=bjx2_addx_v128(vu, vv);
	vu=bjx2_shrx_v128(vu, 32);

	vx.hi=0;	vx.lo=af;
	vy.hi=0;	vy.lo=be;
	vv=bjx2_addx_v128(vx, vy);
	vv=bjx2_shlx_v128(vv, 32);

	vt=bjx2_addx_v128(vu, vv);

	vs=bjx2_addx_v128(vs, vt);
	
	return(vs);
}

int bjx2_clzx_v128(bjx2_val128 va)
{
	u64 v;
	int e;

	if(va.hi)
		{ v=va.hi; e=0; }
	else if(va.lo)
		{ v=va.lo; e=64; }
	else
		return(128);
	if(!(v&0xFFFFFFFF00000000ULL))
		{ v=v<<32; e+=32; }
	if(!(v&0xFFFF000000000000ULL))
		{ v=v<<16; e+=16; }
	if(!(v&0xFF00000000000000ULL))
		{ v=v<<8; e+=8; }
	if(!(v&0xF000000000000000ULL))
		{ v=v<<4; e+=4; }
	if(!(v&0xC000000000000000ULL))
		{ v=v<<2; e+=2; }
	if(!(v&0x8000000000000000ULL))
		{ v=v<<1; e+=1; }
	return(1);
}

bjx2_val128 bjx2_faddx_v128(bjx2_val128 va, bjx2_val128 vb)
{
	bjx2_val128 vma, vmb, vmc, vc;
	int exa, exb, exc, sga, sgb, sgc;
	int czc;
	
	exa=(va.hi>>48)&32767;	sga=(va.hi>>63)&1;
	exb=(vb.hi>>48)&32767;	sgb=(vb.hi>>63)&1;
	
	vma.lo=va.lo;
	vma.hi=va.hi&0x0000FFFFFFFFFFFFULL;
	if(exa)		vma.hi|=0x0001000000000000ULL;
	vmb.lo=vb.lo;
	vmb.hi=vb.hi&0x0000FFFFFFFFFFFFULL;
	if(exb)		vmb.hi|=0x0001000000000000ULL;
	
	if(exb>exa)
	{
		exc=exa; exa=exb; exb=exc;
		sgc=sga; sga=sgb; sgb=sgc;
		vmc=vma; vma=vmb; vmb=vmc;
	}
	
	vmb=bjx2_shrx_v128(vmb, exa-exb);
	
	if(sga)
		vma=bjx2_negx_v128(vma);
	if(sgb)
		vmb=bjx2_negx_v128(vmb);

	exc=exa;
	vmc=bjx2_addx_v128(vma, vmb);

	sgc=(vmc.hi>>63)&1;	
	if(sgc)
		vmc=bjx2_negx_v128(vmc);
	
	czc=bjx2_clzx_v128(vmc)-15;
	if(czc<0)
	{
		vmb=bjx2_shrx_v128(vmc, -czc);
		exc-=czc;
	}else if(czc>0)
	{
		vmb=bjx2_shlx_v128(vmc, czc);
		exc-=czc;
	}
	
	if(exc<=0)
	{
		vc.lo=0;
		vc.hi=0;
		return(vc);
	}
	
	vc.lo=vmc.lo;
	vc.hi=(vmc.hi&0x0000FFFFFFFFFFFFULL)|
		(((u64)exc)<<48) | (((u64)sgc)<<63);
	return(vc);
}

bjx2_val128 bjx2_fsubx_v128(bjx2_val128 va, bjx2_val128 vb)
{
	bjx2_val128 vbi, vc;
	vbi.lo=vb.lo;
	vbi.hi=vb.hi^0x8000000000000000ULL;
	vc=bjx2_faddx_v128(va, vbi);
	return(vc);
}

bjx2_val128 bjx2_fmulx_v128(bjx2_val128 va, bjx2_val128 vb)
{
	bjx2_val128 vma, vmb, vmc, vc;
	int exa, exb, exc, sga, sgb, sgc;
	int czc;
	
	exa=(va.hi>>48)&32767;	sga=(va.hi>>63)&1;
	exb=(vb.hi>>48)&32767;	sgb=(vb.hi>>63)&1;
	
	vma.lo=va.lo;
	vma.hi=va.hi&0x0000FFFFFFFFFFFFULL;
	if(exa)		vma.hi|=0x0001000000000000ULL;
	vmb.lo=vb.lo;
	vmb.hi=vb.hi&0x0000FFFFFFFFFFFFULL;
	if(exb)		vmb.hi|=0x0001000000000000ULL;

	vma=bjx2_shlx_v128(vma, 14);
	vmb=bjx2_shlx_v128(vmb, 14);
	
	vmc=bjx2_mulxhi_v128(vma, vmb);
	
//	vmc.hi=vma.hi*vmb.hi;
//	vmc.lo=0;
	vmc=bjx2_shrx_v128(vmc, 12);

	sgc=sga^sgb;
	exc=(exa+exb)-16383;

	
	czc=bjx2_clzx_v128(vmc)-15;
	if(czc<0)
	{
		vmb=bjx2_shrx_v128(vmc, -czc);
		exc-=czc;
	}else if(czc>0)
	{
		vmb=bjx2_shlx_v128(vmc, czc);
		exc-=czc;
	}
	
	if(exc<=0)
	{
		vc.lo=0;
		vc.hi=0;
		return(vc);
	}
	
	vc.lo=vmc.lo;
	vc.hi=(vmc.hi&0x0000FFFFFFFFFFFFULL)|
		(((u64)exc)<<48) | (((u64)sgc)<<63);
	return(vc);
}

void BJX2_Op_FADDX_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	bjx2_val128 va, vb, vc;
	
	va=BJX2_PtrGetVal128Ix(ctx->regs, op->rm);
	vb=BJX2_PtrGetVal128Ix(ctx->regs, op->ro);
	vc=bjx2_faddx_v128(va, vb);
	BJX2_PtrSetVal128Ix(ctx->regs, op->rn, vc);
}

void BJX2_Op_FSUBX_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	bjx2_val128 va, vb, vc;
	
	va=BJX2_PtrGetVal128Ix(ctx->regs, op->rm);
	vb=BJX2_PtrGetVal128Ix(ctx->regs, op->ro);
	vc=bjx2_fsubx_v128(va, vb);
	BJX2_PtrSetVal128Ix(ctx->regs, op->rn, vc);
}

void BJX2_Op_FMULX_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	bjx2_val128 va, vb, vc;
	
	va=BJX2_PtrGetVal128Ix(ctx->regs, op->rm);
	vb=BJX2_PtrGetVal128Ix(ctx->regs, op->ro);
	vc=bjx2_fmulx_v128(va, vb);
	BJX2_PtrSetVal128Ix(ctx->regs, op->rn, vc);
}
#endif

#if 1
void BJX2_Op_FNEGX_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn+1]=ctx->regs[op->rm+1]^0x8000000000000000ULL;
}

void BJX2_Op_FABSX_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[op->rn+1]=ctx->regs[op->rm+1]&0x7FFFFFFFFFFFFFFFULL;
}

void BJX2_Op_FSQRTX_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	double a, b, c;
	
	a=BJX2_PtrGetF128Ix(ctx->regs, op->rm);
	c=sqrt(a);
	BJX2_PtrSetF128Ix(ctx->regs, op->rn, c);

//	bjx2_val128 va, vb, vc;	
//	va=BJX2_PtrGetVal128Ix(ctx->regs, op->rm);
//	vc=bjx2_fmulx_v128(va, vb);
//	BJX2_PtrSetVal128Ix(ctx->regs, op->rn, vc);
}

void BJX2_Op_FCMPXEQ_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
#if 1
	bjx2_val128 va, vb;
	int iseq;
	
	va=BJX2_PtrGetVal128Ix(ctx->regs, op->rn);
	vb=BJX2_PtrGetVal128Ix(ctx->regs, op->rm);

	iseq=1;
	if(va.lo!=vb.lo)
		iseq=0;
	if(va.hi!=vb.hi)
		iseq=0;
		
	if(((va.hi>>48)&0x7FFF)==0x7FFF)
	{
		if((va.hi>>44)&15)
			iseq=0;
	}

	if(iseq)
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
#endif
}

void BJX2_Op_FCMPXGT_GRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
#if 1
	bjx2_val128 va, vb;
	int isgt, isgta, sga, sgb;
	
	va=BJX2_PtrGetVal128Ix(ctx->regs, op->rn);
	vb=BJX2_PtrGetVal128Ix(ctx->regs, op->rm);

	sga=(va.hi>>63)&1;
	sgb=(vb.hi>>63)&1;

	isgta=1;
	if((va.hi&0x7FFFFFFFFFFFFFFFULL) < (vb.hi&0x7FFFFFFFFFFFFFFFULL))
		isgta=0;
	if((va.hi&0x7FFFFFFFFFFFFFFFULL) == (vb.hi&0x7FFFFFFFFFFFFFFFULL))
	{
		if(va.lo<vb.lo)
			isgta=0;
	}

	isgt=1;
	if(sga^sgb)
	{
		if(sga)
			isgt=0;
	}else
	{
		if(sga)
			{ isgt=!isgta; }
		else
			{ isgt=isgta; }
	}

	if(isgt)
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
#endif
}
#endif


#if 1
void BJX2_Op_ADDX_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 valo, vahi, vblo, vbhi, vclo, vchi;
	int vacf;
	
	valo=ctx->regs[op->rm+0];	vahi=ctx->regs[op->rm+1];
	vblo=ctx->regs[op->ro+0];	vbhi=ctx->regs[op->ro+1];
	vclo=valo+vblo; vacf=(vclo<=valo);
	vchi=vahi+vbhi+vacf;
	ctx->regs[op->rn+0]=vclo;
	ctx->regs[op->rn+1]=vchi;
}

void BJX2_Op_SUBX_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 valo, vahi, vblo, vbhi, vclo, vchi;
	int vacf;
	
	valo=ctx->regs[op->rm+0];	vahi=ctx->regs[op->rm+1];
	vblo=ctx->regs[op->ro+0];	vbhi=ctx->regs[op->ro+1];
	
	vblo=~vblo; vacf=((vblo+1)<=vblo);
	vbhi=(~vbhi)+vacf;
	vblo++;
	
	vclo=valo+vblo; vacf=(vclo<=valo);
	vchi=vahi+vbhi+vacf;
	ctx->regs[op->rn+0]=vclo;
	ctx->regs[op->rn+1]=vchi;
}

void BJX2_Op_ANDX_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 valo, vahi, vblo, vbhi, vclo, vchi;
	valo=ctx->regs[op->rm+0];	vahi=ctx->regs[op->rm+1];
	vblo=ctx->regs[op->ro+0];	vbhi=ctx->regs[op->ro+1];
	vclo=valo&vblo;				vchi=vahi&vbhi;
	ctx->regs[op->rn+0]=vclo;	ctx->regs[op->rn+1]=vchi;
}

void BJX2_Op_ORX_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 valo, vahi, vblo, vbhi, vclo, vchi;
	valo=ctx->regs[op->rm+0];	vahi=ctx->regs[op->rm+1];
	vblo=ctx->regs[op->ro+0];	vbhi=ctx->regs[op->ro+1];
	vclo=valo|vblo;				vchi=vahi|vbhi;
	ctx->regs[op->rn+0]=vclo;	ctx->regs[op->rn+1]=vchi;
}

void BJX2_Op_XORX_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 valo, vahi, vblo, vbhi, vclo, vchi;
	valo=ctx->regs[op->rm+0];	vahi=ctx->regs[op->rm+1];
	vblo=ctx->regs[op->ro+0];	vbhi=ctx->regs[op->ro+1];
	vclo=valo^vblo;				vchi=vahi^vbhi;
	ctx->regs[op->rn+0]=vclo;	ctx->regs[op->rn+1]=vchi;
}


void BJX2_Op_SHLDX_RegRegReg_i(BJX2_Context *ctx, BJX2_Opcode *op, int shl)
{
	u64 valo, vahi, vblo, vbhi, vclo, vchi;

	valo=ctx->regs[op->rm+0];	vahi=ctx->regs[op->rm+1];
	if(shl>0)
	{
		if(shl&0x40)
		{
			vchi=valo<<(shl&63);
			vclo=0;
		}else
		{
			vchi=(vahi<<(shl&63))|(valo>>(64-(shl&63)));
			vclo=(valo<<(shl&63));
		}
	}else if(shl<0)
	{
		shl=-shl;
		if(shl&0x40)
		{
			vchi=0;
			vclo=vahi>>(shl&63);
		}else
		{
			vchi=(vahi>>(shl&63));
			vclo=(valo>>(shl&63))|(vahi<<(64-(shl&63)));
		}
	}else
	{
		vclo=valo;
		vchi=vahi;
	}
	ctx->regs[op->rn+0]=vclo;
	ctx->regs[op->rn+1]=vchi;
}

void BJX2_Op_SHADX_RegRegReg_i(BJX2_Context *ctx, BJX2_Opcode *op, int shl)
{
	u64 valo, vahi, vblo, vbhi, vclo, vchi;

	valo=ctx->regs[op->rm+0];	vahi=ctx->regs[op->rm+1];

	if(shl>0)
	{
		if(shl&0x40)
		{
			vchi=valo<<(shl&63);
			vclo=0;
		}else
		{
			vchi=(vahi<<(shl&63))|(valo>>(64-(shl&63)));
			vclo=(valo<<(shl&63));
		}
	}else if(shl<0)
	{
		shl=-shl;
		if(shl&0x40)
		{
			vchi=((s64)vahi)>>63;
			vclo=((s64)vahi)>>(shl&63);
		}else
		{
			vchi=((s64)vahi)>>(shl&63);
			vclo=(valo>>(shl&63))|(vahi<<(64-(shl&63)));
		}
	}else
	{
		vclo=valo;
		vchi=vahi;
	}

	ctx->regs[op->rn+0]=vclo;
	ctx->regs[op->rn+1]=vchi;
}

void BJX2_Op_SHLDX_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	shl=ctx->regs[op->ro];
	BJX2_Op_SHLDX_RegRegReg_i(ctx, op, shl);
}

void BJX2_Op_SHLRX_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	shl=ctx->regs[op->ro];
	BJX2_Op_SHLDX_RegRegReg_i(ctx, op, -shl);
}

void BJX2_Op_SHADX_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	shl=ctx->regs[op->ro];
	BJX2_Op_SHADX_RegRegReg_i(ctx, op, shl);
}

void BJX2_Op_SHARX_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	shl=ctx->regs[op->ro];
	BJX2_Op_SHADX_RegRegReg_i(ctx, op, -shl);
}

void BJX2_Op_SHLDX_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
//	shl=ctx->regs[op->ro];
	shl=(sbyte)(op->imm);
	BJX2_Op_SHLDX_RegRegReg_i(ctx, op, shl);
}

void BJX2_Op_SHADX_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
//	shl=ctx->regs[op->ro];
	shl=(sbyte)(op->imm);
	BJX2_Op_SHADX_RegRegReg_i(ctx, op, shl);
}

void BJX2_Op_ROTLX_RegRegReg_i(BJX2_Context *ctx, BJX2_Opcode *op, int shl)
{
	u64 valo, vahi, vblo, vbhi, vclo, vchi;

	valo=ctx->regs[op->rm+0];	vahi=ctx->regs[op->rm+1];
	if(shl>0)
	{
		if(shl&0x40)
		{
			vchi=(valo<<(shl&63))|(vahi>>(64-(shl&63)));
			vclo=(vahi<<(shl&63))|(valo>>(64-(shl&63)));
		}else
		{
			vchi=(vahi<<(shl&63))|(valo>>(64-(shl&63)));
			vclo=(valo<<(shl&63))|(vahi>>(64-(shl&63)));
		}
	}else if(shl<0)
	{
		shl=-shl;
		if(shl&0x40)
		{
			vchi=(valo>>(shl&63))|(vahi<<(64-(shl&63)));
			vclo=(vahi>>(shl&63))|(valo<<(64-(shl&63)));
		}else
		{
			vchi=(vahi>>(shl&63))|(valo<<(64-(shl&63)));
			vclo=(valo>>(shl&63))|(vahi<<(64-(shl&63)));
		}
	}else
	{
		vclo=valo;
		vchi=vahi;
	}
	ctx->regs[op->rn+0]=vclo;
	ctx->regs[op->rn+1]=vchi;
}

void BJX2_Op_ROTLX_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	shl=ctx->regs[op->ro];
	BJX2_Op_ROTLX_RegRegReg_i(ctx, op, shl);
}

void BJX2_Op_ROTRX_RegRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int shl;
	shl=ctx->regs[op->ro];
	BJX2_Op_ROTLX_RegRegReg_i(ctx, op, -shl);
}

#endif


#if 1
void BJX2_Op_CMPXEQ_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	bjx2_val128 va, vb;
	int iseq;
	
	va=BJX2_PtrGetVal128Ix(ctx->regs, op->rn);
	vb=BJX2_PtrGetVal128Ix(ctx->regs, op->rm);

	iseq=1;
	if(va.lo!=vb.lo)
		iseq=0;
	if(va.hi!=vb.hi)
		iseq=0;
		
	if(iseq)
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPXGT_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	bjx2_val128 va, vb;
	int isgt, isgta, sga, sgb;
	
	va=BJX2_PtrGetVal128Ix(ctx->regs, op->rn);
	vb=BJX2_PtrGetVal128Ix(ctx->regs, op->rm);

	sga=(va.hi>>63)&1;
	sgb=(vb.hi>>63)&1;

	isgta=1;
	if((va.hi&0x7FFFFFFFFFFFFFFFULL) < (vb.hi&0x7FFFFFFFFFFFFFFFULL))
		isgta=0;
	if((va.hi&0x7FFFFFFFFFFFFFFFULL) == (vb.hi&0x7FFFFFFFFFFFFFFFULL))
	{
		if(va.lo<=vb.lo)
			isgta=0;
	}

	isgt=1;
	if(sga^sgb)
	{
		if(sga)
			isgt=0;
	}else
	{
		isgt=isgta;
	}

	if(isgt)
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

void BJX2_Op_CMPXHI_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	bjx2_val128 va, vb;
	int isgt, isgta, sga, sgb;
	
	va=BJX2_PtrGetVal128Ix(ctx->regs, op->rn);
	vb=BJX2_PtrGetVal128Ix(ctx->regs, op->rm);

	isgta=1;
	if(va.hi<vb.hi)
		isgta=0;
	if(va.hi == vb.hi)
	{
		if(va.lo<=vb.lo)
			isgta=0;
	}

	isgt=isgta;

	if(isgt)
		ctx->regs[BJX2_REG_SR]|=1;
	else
		ctx->regs[BJX2_REG_SR]&=~1;
}

#endif