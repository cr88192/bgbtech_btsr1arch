void X3VM_Opc_NOP_NONE(X3VM_Context *ctx, X3VM_Opcode *op)
{
}

void X3VM_Opc_MOV_2R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[op->rn]=ctx->reg[op->rs];
}

void X3VM_Opc_MOV_2RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[op->rn]=op->imm;
}

void X3VM_Opc_ADD_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[op->rn]=ctx->reg[op->rs]+ctx->reg[op->rt];
}

void X3VM_Opc_SUB_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[op->rn]=ctx->reg[op->rs]-ctx->reg[op->rt];
}

void X3VM_Opc_AND_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[op->rn]=ctx->reg[op->rs]&ctx->reg[op->rt];
}

void X3VM_Opc_OR_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[op->rn]=ctx->reg[op->rs]|ctx->reg[op->rt];
}

void X3VM_Opc_XOR_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[op->rn]=ctx->reg[op->rs]^ctx->reg[op->rt];
}

void X3VM_Opc_ADDW_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[op->rn]=(s32)(ctx->reg[op->rs]+ctx->reg[op->rt]);
}

void X3VM_Opc_SUBW_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[op->rn]=(s32)(ctx->reg[op->rs]-ctx->reg[op->rt]);
}

void X3VM_Opc_ADDWU_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[op->rn]=(u32)(ctx->reg[op->rs]+ctx->reg[op->rt]);
}

void X3VM_Opc_SUBWU_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[op->rn]=(u32)(ctx->reg[op->rs]-ctx->reg[op->rt]);
}

void X3VM_Opc_ADDUW_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[op->rn]=((u32)ctx->reg[op->rs])+ctx->reg[op->rt];
}

void X3VM_Opc_MULW_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s32 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=ctx->reg[op->rt];
	vc=va*vb;
	ctx->reg[op->rn]=((s64)vc);
}

void X3VM_Opc_MULWU_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u32 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=ctx->reg[op->rt];
	vc=va*vb;
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_MULQ_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=ctx->reg[op->rt];
	vc=va*vb;
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_MULQU_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=ctx->reg[op->rt];
	vc=va*vb;
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_DMULW_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=ctx->reg[op->rt];
	va=(s32)va;
	vb=(s32)vb;
	vc=va*vb;
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_DMULWU_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=ctx->reg[op->rt];
	va=(u32)va;
	vb=(u32)vb;
	vc=va*vb;
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_DMULH_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s32 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=ctx->reg[op->rt];
	va=(s16)va;
	vb=(s16)vb;
	vc=va*vb;
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_DMULHU_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u32 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=ctx->reg[op->rt];
	va=(u16)va;
	vb=(u16)vb;
	vc=va*vb;
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_DMULH_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s32 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=op->imm;
	va=(s16)va;
	vb=(s16)vb;
	vc=va*vb;
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_DMULHU_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u32 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=op->imm;
	va=(u16)va;
	vb=(u16)vb;
	vc=va*vb;
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_DIVW_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s32 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=ctx->reg[op->rt];
	if(vb==0)
		{ X3VM_ThrowFault(ctx, X3VM_FAULT_INVOP); return; }
	vc=va/vb;
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_DIVUW_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u32 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=ctx->reg[op->rt];
	if(vb==0)
		{ X3VM_ThrowFault(ctx, X3VM_FAULT_INVOP); return; }
	vc=va/vb;
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_DIV_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=ctx->reg[op->rt];
	if(vb==0)
		{ X3VM_ThrowFault(ctx, X3VM_FAULT_INVOP); return; }
	vc=va/vb;
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_DIVU_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=ctx->reg[op->rt];
	if(vb==0)
		{ X3VM_ThrowFault(ctx, X3VM_FAULT_INVOP); return; }
	vc=va/vb;
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_REMW_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s32 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=ctx->reg[op->rt];
	if(vb==0)
		{ X3VM_ThrowFault(ctx, X3VM_FAULT_INVOP); return; }
	vc=va%vb;
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_REMUW_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u32 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=ctx->reg[op->rt];
	if(vb==0)
		{ X3VM_ThrowFault(ctx, X3VM_FAULT_INVOP); return; }
	vc=va%vb;
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_REM_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=ctx->reg[op->rt];
	if(vb==0)
		{ X3VM_ThrowFault(ctx, X3VM_FAULT_INVOP); return; }
	vc=va%vb;
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_REMU_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=ctx->reg[op->rt];
	if(vb==0)
		{ X3VM_ThrowFault(ctx, X3VM_FAULT_INVOP); return; }
	vc=va%vb;
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_MIN_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=ctx->reg[op->rt];
	vc=(va<vb)?va:vb;
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_MAX_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=ctx->reg[op->rt];
	vc=(va>vb)?va:vb;
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_SLL_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 va, vc;
	int sh;
	va=ctx->reg[op->rs];
	sh=(sbyte)(ctx->reg[op->rt]);
	if(sh>=0)
		vc=(va<<(sh&63));
	else
		vc=va>>((-sh)&63);
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_SLA_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 va, vc;
	int sh;
	va=ctx->reg[op->rs];
	sh=(sbyte)(ctx->reg[op->rt]);
	if(sh>=0)
		vc=(va<<(sh&63));
	else
		vc=va>>((-sh)&63);
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_SLLW_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u32 va, vc;
	int sh;
	va=ctx->reg[op->rs];
	sh=(sbyte)(ctx->reg[op->rt]);
	if(sh>=0)
		vc=(va<<(sh&31));
	else
		vc=va>>((-sh)&31);
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_SLAW_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s32 va, vc;
	int sh;
	va=ctx->reg[op->rs];
	sh=(sbyte)(ctx->reg[op->rt]);
	if(sh>=0)
		vc=(va<<(sh&31));
	else
		vc=va>>((-sh)&31);
	ctx->reg[op->rn]=vc;
}


void X3VM_Opc_SRL_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 va, vc;
	int sh;
	va=ctx->reg[op->rs];
	sh=(sbyte)(ctx->reg[op->rt]);
	sh=-sh;
	if(sh>=0)
		vc=(va<<(sh&63));
	else
		vc=va>>((-sh)&63);
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_SRA_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 va, vc;
	int sh;
	va=ctx->reg[op->rs];
	sh=(sbyte)(ctx->reg[op->rt]);
	sh=-sh;
	if(sh>=0)
		vc=(va<<(sh&63));
	else
		vc=va>>((-sh)&63);
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_SRLW_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u32 va, vc;
	int sh;
	va=ctx->reg[op->rs];
	sh=(sbyte)(ctx->reg[op->rt]);
	sh=-sh;
	if(sh>=0)
		vc=(va<<(sh&31));
	else
		vc=va>>((-sh)&31);
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_SRAW_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s32 va, vc;
	int sh;
	va=ctx->reg[op->rs];
	sh=(sbyte)(ctx->reg[op->rt]);
	sh=-sh;
	if(sh>=0)
		vc=(va<<(sh&31));
	else
		vc=va>>((-sh)&31);
	ctx->reg[op->rn]=vc;
}


void X3VM_Opc_ADD_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[op->rn]=ctx->reg[op->rs]+op->imm;
}

void X3VM_Opc_SUB_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[op->rn]=ctx->reg[op->rs]-op->imm;
}

void X3VM_Opc_AND_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[op->rn]=ctx->reg[op->rs]&op->imm;
}

void X3VM_Opc_OR_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[op->rn]=ctx->reg[op->rs]|op->imm;
}

void X3VM_Opc_XOR_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[op->rn]=ctx->reg[op->rs]^op->imm;
}

void X3VM_Opc_ADDW_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[op->rn]=(s32)(ctx->reg[op->rs]+op->imm);
}

void X3VM_Opc_ADDWU_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[op->rn]=(u32)(ctx->reg[op->rs]+op->imm);
}

void X3VM_Opc_RSUB_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[op->rn]=op->imm-ctx->reg[op->rs];
}

void X3VM_Opc_SHORI_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[op->rn]=(ctx->reg[op->rs]<<16)|op->imm;
}

void X3VM_Opc_SHORI32_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[op->rn]=(ctx->reg[op->rs]<<32)|op->imm;
}

void X3VM_Opc_FLDCH_2RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	ctx->reg[op->rn]=X3VM_ConvFp16ToFp64(op->imm);
}


void X3VM_Opc_MULW_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s32 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=op->imm;
	vc=va*vb;
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_MULWU_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u32 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=op->imm;
	vc=va*vb;
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_MULQ_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=op->imm;
	vc=va*vb;
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_MULQU_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=op->imm;
	vc=va*vb;
	ctx->reg[op->rn]=vc;
}


void X3VM_Opc_MULQH_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=ctx->reg[op->rt];
	vc=((va>>32)*(vb>>32))+
		(((va>>32)*((u32)vb))>>32)+
		(((vb>>32)*((u32)va))>>32);
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_MULQHU_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=ctx->reg[op->rt];
	vc=((va>>32)*(vb>>32))+
		(((va>>32)*((u32)vb))>>32)+
		(((vb>>32)*((u32)va))>>32);
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_MULQHSU_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 va, vb, vc;
	va=ctx->reg[op->rs];
	vb=ctx->reg[op->rt];
	vc=((va>>32)*(vb>>32))+
		(((va>>32)*((u32)vb))>>32)+
		(((vb>>32)*((u32)va))>>32);
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_SLL_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 va, vc;
	int sh;
	va=ctx->reg[op->rs];
	sh=(sbyte)(op->imm);
	if(sh>=0)
		vc=(va<<sh);
	else
		vc=va>>(-sh);
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_SLA_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 va, vc;
	int sh;
	va=ctx->reg[op->rs];
	sh=(sbyte)(op->imm);
	if(sh>=0)
		vc=(va<<sh);
	else
		vc=va>>(-sh);
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_SLLW_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u32 va, vc;
	int sh;
	va=ctx->reg[op->rs];
	sh=(sbyte)(op->imm);
	if(sh>=0)
		vc=va<<(sh&31);
	else
		vc=va>>((-sh)&31);
	ctx->reg[op->rn]=vc;
}

void X3VM_Opc_SLAW_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s32 va, vc;
	int sh;
	va=ctx->reg[op->rs];
	sh=(sbyte)(op->imm);
	if(sh>=0)
		vc=va<<(sh&31);
	else
		vc=va>>((-sh)&31);
	ctx->reg[op->rn]=((s64)vc);
}

void X3VM_Opc_BITMOV_4RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 va, vb, vc, vn, vm, vhm, vlm;
	int sh, vh, vl;

	va=ctx->reg[op->rs];
	vb=ctx->reg[op->rt];

	sh=(sbyte)(op->imm);
	vl=(op->imm>> 8)&255;
	vh=(op->imm>>16)&255;
	
	vhm=  (1ULL<<vh)-1;
	vlm=~((1ULL<<vl)-1);
	
	if(vh>vl)
		{ vm=vhm&vlm; }
	else
		{ vm=vhm|vlm; }

	if(sh>=0)
		vc=va<<sh;
	else
		vc=va>>(-sh);
	vn=(vc&vm)|(vb&(~vm));
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_BITMOVS_4RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 va, vb, vc, vn, vm, vhm, vlm;
	int sh, vh, vl;

	va=ctx->reg[op->rs];
	vb=ctx->reg[op->rt];

	sh=(sbyte)(op->imm);
	vl=(op->imm>> 8)&255;
	vh=(op->imm>>16)&255;
	
	vhm=  (1ULL<<vh)-1;
	vlm=~((1ULL<<vl)-1);
	
	if(vh>vl)
		{ vm=vhm&vlm; }
	else
		{ vm=vhm|vlm; }

	if(sh>=0)
		vc=va<<sh;
	else
		vc=va>>(-sh);

	if((vc>>(vh-1))&1)
		vb=~vb;

	vn=(vc&vm)|(vb&(~vm));
	ctx->reg[op->rn]=vn;
}



void X3VM_Opc_SEQ_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=(vs==vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_SNE_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=(vs==vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_SLT_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=vs<vt;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_SGE_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=vs>=vt;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_SLTU_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=vs<vt;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_SGEU_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=vs>=vt;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_SEQ_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=op->imm;
	vn=(vs==vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_SNE_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=op->imm;
	vn=(vs==vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_SLT_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=op->imm;
	vn=vs<vt;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_SGE_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=op->imm;
	vn=vs>=vt;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_SGT_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=op->imm;
	vn=vs>vt;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_SLTU_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=op->imm;
	vn=vs<vt;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_SGEU_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=op->imm;
	vn=vs>=vt;
	ctx->reg[op->rn]=vn;
}


void X3VM_Opc_STST_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=((vs&vt)!=0);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_SNTST_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=((vs&vt)==0);
	ctx->reg[op->rn]=vn;
}



void X3VM_Opc_ZSEQ_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=(vs==vt);
	ctx->pc_hibits=(ctx->pc_hibits&(~1))|(vn&1);
}

void X3VM_Opc_ZSNE_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=(vs==vt);
	ctx->pc_hibits=(ctx->pc_hibits&(~1))|(vn&1);
}

void X3VM_Opc_ZSLT_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=vs<vt;
	ctx->pc_hibits=(ctx->pc_hibits&(~1))|(vn&1);
}

void X3VM_Opc_ZSGE_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=vs>=vt;
	ctx->pc_hibits=(ctx->pc_hibits&(~1))|(vn&1);
}

void X3VM_Opc_ZSLTU_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=vs<vt;
	ctx->pc_hibits=(ctx->pc_hibits&(~1))|(vn&1);
}

void X3VM_Opc_ZSGEU_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=vs>=vt;
	ctx->pc_hibits=(ctx->pc_hibits&(~1))|(vn&1);
}

void X3VM_Opc_ZSEQ_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=op->imm;
	vn=(vs==vt);
	ctx->pc_hibits=(ctx->pc_hibits&(~1))|(vn&1);
}

void X3VM_Opc_ZSNE_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=op->imm;
	vn=(vs==vt);
	ctx->pc_hibits=(ctx->pc_hibits&(~1))|(vn&1);
}

void X3VM_Opc_ZSLT_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=op->imm;
	vn=vs<vt;
	ctx->pc_hibits=(ctx->pc_hibits&(~1))|(vn&1);
}

void X3VM_Opc_ZSGE_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=op->imm;
	vn=vs>=vt;
	ctx->pc_hibits=(ctx->pc_hibits&(~1))|(vn&1);
}

void X3VM_Opc_ZSGT_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=op->imm;
	vn=vs>vt;
	ctx->pc_hibits=(ctx->pc_hibits&(~1))|(vn&1);
}

void X3VM_Opc_ZSLTU_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=op->imm;
	vn=vs<vt;
	ctx->pc_hibits=(ctx->pc_hibits&(~1))|(vn&1);
}

void X3VM_Opc_ZSGEU_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=op->imm;
	vn=vs>=vt;
	ctx->pc_hibits=(ctx->pc_hibits&(~1))|(vn&1);
}


void X3VM_Opc_ZSTST_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=((vs&vt)!=0);
	ctx->pc_hibits=(ctx->pc_hibits&(~1))|(vn&1);
}

void X3VM_Opc_ZSNTST_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=((vs&vt)==0);
	ctx->pc_hibits=(ctx->pc_hibits&(~1))|(vn&1);
}



void X3VM_Opc_PSHUFB_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	int im;

	vs=ctx->reg[op->rs];
	im=op->imm;
	vn=        (byte)(vs>>(((im>>6)&3)*8));
	vn=(vn<<8)|(byte)(vs>>(((im>>4)&3)*8));
	vn=(vn<<8)|(byte)(vs>>(((im>>2)&3)*8));
	vn=(vn<<8)|(byte)(vs>>(((im>>0)&3)*8));
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PSHUFH_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	int im;

	vs=ctx->reg[op->rs];
	im=op->imm;
	vn=         (u16)(vs>>(((im>>6)&3)*16));
	vn=(vn<<16)|(u16)(vs>>(((im>>4)&3)*16));
	vn=(vn<<16)|(u16)(vs>>(((im>>2)&3)*16));
	vn=(vn<<16)|(u16)(vs>>(((im>>0)&3)*16));

	if(imm>>16)
	{
		if(((imm>> 8)&3)==0)		vn&=~0x000000000000FFFFULL;
		if(((imm>> 8)&3)==2)		vn^= 0x000000000000FFFFULL;
		if(((imm>> 8)&3)==3)		vn^= 0x0000000000008000ULL;
		if(((imm>>10)&3)==0)		vn&=~0x00000000FFFF0000ULL;
		if(((imm>>10)&3)==2)		vn^= 0x00000000FFFF0000ULL;
		if(((imm>>10)&3)==3)		vn^= 0x0000000080000000ULL;
		if(((imm>>12)&3)==0)		vn&=~0x0000FFFF00000000ULL;
		if(((imm>>12)&3)==2)		vn^= 0x0000FFFF00000000ULL;
		if(((imm>>12)&3)==3)		vn^= 0x0000800000000000ULL;
		if(((imm>>14)&3)==0)		vn&=~0xFFFF000000000000ULL;
		if(((imm>>14)&3)==2)		vn^= 0xFFFF000000000000ULL;
		if(((imm>>14)&3)==3)		vn^= 0x8000000000000000ULL;
	}

	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PMULTH_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	s64 vs, vt, vn;
	int im;

	vs=ctx->reg[op->rs];
	im=op->imm;
	vn=vs;

	if(((imm>>0)&3)==0)		vn&=~0x000000000000FFFFULL;
	if(((imm>>0)&3)==2)		vn^= 0x000000000000FFFFULL;
	if(((imm>>0)&3)==3)		vn^= 0x0000000000008000ULL;
	if(((imm>>2)&3)==0)		vn&=~0x00000000FFFF0000ULL;
	if(((imm>>2)&3)==2)		vn^= 0x00000000FFFF0000ULL;
	if(((imm>>2)&3)==3)		vn^= 0x0000000080000000ULL;
	if(((imm>>4)&3)==0)		vn&=~0x0000FFFF00000000ULL;
	if(((imm>>4)&3)==2)		vn^= 0x0000FFFF00000000ULL;
	if(((imm>>4)&3)==3)		vn^= 0x0000800000000000ULL;
	if(((imm>>6)&3)==0)		vn&=~0xFFFF000000000000ULL;
	if(((imm>>6)&3)==2)		vn^= 0xFFFF000000000000ULL;
	if(((imm>>6)&3)==3)		vn^= 0x8000000000000000ULL;

	ctx->reg[op->rn]=vn;
}


void X3VM_Opc_PREDT(X3VM_Context *ctx, X3VM_Opcode *op)
{
	if(ctx->pc_hibits&1)
		op->subop->Run(ctx, op);
}

void X3VM_Opc_PREDF(X3VM_Context *ctx, X3VM_Opcode *op)
{
	if(!(ctx->pc_hibits&1))
		op->subop->Run(ctx, op);
}


void X3VM_Opc_ADDX_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs0, vt0, vs1, vt1, vn0, vn1;
	
	vs0=ctx->reg[op->rs+0];
	vs1=ctx->reg[op->rs+1];
	vt0=ctx->reg[op->rt+0];
	vt1=ctx->reg[op->rt+1];
//	if(!op->rs) vs1=0;
//	if(!op->rt) vt1=0;

	vn0=vs0+vt0;
	vn1=vs1+vt1+(vn0<vs0);

	ctx->reg[op->rn+0]=vn0;
	ctx->reg[op->rn+1]=vn1;
}

void X3VM_Opc_SUBX_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs0, vt0, vs1, vt1, vn0, vn1;
	
	vs0=ctx->reg[op->rs+0];
	vs1=ctx->reg[op->rs+1];
	vt0=ctx->reg[op->rt+0];
	vt1=ctx->reg[op->rt+1];
//	if(!op->rs) vs1=0;
//	if(!op->rt) vt1=0;
	
	vt0=~vt0;
	vt1=~vt1;
	
	vn0=vs0+vt0+1;
	vn1=vs1+vt1+(vn0<vs0);

	ctx->reg[op->rn+0]=vn0;
	ctx->reg[op->rn+1]=vn1;
}

void X3VM_Opc_ANDX_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs0, vt0, vs1, vt1, vn0, vn1;
	
	vs0=ctx->reg[op->rs+0];
	vs1=ctx->reg[op->rs+1];
	vt0=ctx->reg[op->rt+0];
	vt1=ctx->reg[op->rt+1];
//	if(!op->rs) vs1=0;
//	if(!op->rt) vt1=0;

	vn0=vs0&vt0;
	vn1=vs1&vt1;
	ctx->reg[op->rn+0]=vn0;
	ctx->reg[op->rn+1]=vn1;
}

void X3VM_Opc_ORX_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs0, vt0, vs1, vt1, vn0, vn1;
	
	vs0=ctx->reg[op->rs+0];
	vs1=ctx->reg[op->rs+1];
	vt0=ctx->reg[op->rt+0];
	vt1=ctx->reg[op->rt+1];
//	if(!op->rs) vs1=0;
//	if(!op->rt) vt1=0;

	vn0=vs0|vt0;
	vn1=vs1|vt1;
	ctx->reg[op->rn+0]=vn0;
	ctx->reg[op->rn+1]=vn1;
}

void X3VM_Opc_XORX_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs0, vt0, vs1, vt1, vn0, vn1;
	
	vs0=ctx->reg[op->rs+0];
	vs1=ctx->reg[op->rs+1];
	vt0=ctx->reg[op->rt+0];
	vt1=ctx->reg[op->rt+1];
//	if(!op->rs) vs1=0;
//	if(!op->rt) vt1=0;

	vn0=vs0^vt0;
	vn1=vs1^vt1;
	ctx->reg[op->rn+0]=vn0;
	ctx->reg[op->rn+1]=vn1;
}

void X3VM_Opc_SLLX_I3R(X3VM_Context *ctx, X3VM_Opcode *op, int sh0)
{
	u64 vs0, vt0, vs1, vt1, vn0, vn1;
	int sh;
	
	sh=(sbyte)(sh0);
	
	vs0=ctx->reg[op->rs+0];
	vs1=ctx->reg[op->rs+1];
	if(sh>0)
	{
		if(sh>=64)
		{
			vn1=vs0<<(sh-64);
			vn0=0;
		}else
		{
			vn1=(vs1<<sh)|(vs0>>(64-sh));
			vn0=vs0<<sh;
		}
	}else
		if(sh<0)
	{
		sh=-sh;
		if(sh>=64)
		{
			vn1=0;
			vn0=vs1>>(sh-64);
		}else
		{
			vn1=vs1>>sh;
			vn0=(vs0>>sh)|(vs1<<(64-sh));
		}
	}else
	{
		vn0=vs0;
		vn1=vs1;
	}
	ctx->reg[op->rn+0]=vn0;
	ctx->reg[op->rn+1]=vn1;
}

void X3VM_Opc_SLAX_I3R(X3VM_Context *ctx, X3VM_Opcode *op, int sh0)
{
	s64 vs0, vt0, vs1, vt1, vn0, vn1;
	int sh;
	
	sh=(sbyte)(sh0);
	
	vs0=ctx->reg[op->rs+0];
	vs1=ctx->reg[op->rs+1];
	if(sh>0)
	{
		if(sh>=64)
		{
			vn1=vs0<<(sh-64);
			vn0=0;
		}else
		{
			vn1=(vs1<<sh)|(vs0>>(64-sh));
			vn0=vs0<<sh;
		}
	}else
		if(sh<0)
	{
		sh=-sh;
		if(sh>=64)
		{
			vn1=vs1>>63;
			vn0=vs1>>(sh-64);
		}else
		{
			vn1=vs1>>sh;
			vn0=(vs0>>sh)|(vs1<<(64-sh));
		}
	}else
	{
		vn0=vs0;
		vn1=vs1;
	}
	ctx->reg[op->rn+0]=vn0;
	ctx->reg[op->rn+1]=vn1;
}

void X3VM_Opc_SLLX_3R(X3VM_Context *ctx, X3VM_Opcode *op)
	{ X3VM_Opc_SLLX_I3R(ctx, op, ctx->reg[op->rt]); }
void X3VM_Opc_SLAX_3R(X3VM_Context *ctx, X3VM_Opcode *op)
	{ X3VM_Opc_SLAX_I3R(ctx, op, ctx->reg[op->rt]); }
void X3VM_Opc_SLLX_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
	{ X3VM_Opc_SLLX_I3R(ctx, op, op->imm); }
void X3VM_Opc_SLAX_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
	{ X3VM_Opc_SLAX_I3R(ctx, op, op->imm); }

void X3VM_Opc_SRLX_3R(X3VM_Context *ctx, X3VM_Opcode *op)
	{ X3VM_Opc_SLLX_I3R(ctx, op, -ctx->reg[op->rt]); }
void X3VM_Opc_SRAX_3R(X3VM_Context *ctx, X3VM_Opcode *op)
	{ X3VM_Opc_SLAX_I3R(ctx, op, -ctx->reg[op->rt]); }
void X3VM_Opc_SRLX_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
	{ X3VM_Opc_SLLX_I3R(ctx, op, -op->imm); }
void X3VM_Opc_SRAX_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
	{ X3VM_Opc_SLAX_I3R(ctx, op, -op->imm); }


void X3VM_Opc_EXTSB_2R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vn=(sbyte)vs;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_EXTUB_2R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vn=(byte)vs;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_EXTSH_2R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vn=(s16)vs;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_EXTUH_2R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vn=(u16)vs;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_MOVTT_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=	(vs&0x0000FFFFFFFFFFFFULL) |
		(vt&0xFFFF000000000000ULL) ;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_MOVTT_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	
	vs=ctx->reg[op->rs];
//	vt=ctx->reg[op->rt];
	vt=op->imm;
	vn=	(vs&0x0000FFFFFFFFFFFFULL) |
		(vt&0xFFFF000000000000ULL) ;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_MOVTT5_3RI(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	int rt;
	
	rt=op->imm;
	vs=ctx->reg[op->rs];
	if(!(rt&1))
		{ vn=(vs&0x0FFFFFFFFFFFFFFFULL)|(((rt>>1)&15ULL)<<60); }
	else if(!(rt&2))
		{ vn=(vs&0x1FFFFFFFFFFFFFFFULL)|(((rt>>2)& 7ULL)<<61); }
	else if(!(rt&4))
		{ vn=(vs&0x3FFFFFFFFFFFFFFFULL)|(((rt>>3)& 3ULL)<<62); }
	else if(!(rt&8))
		{ vn=(vs&0x7FFFFFFFFFFFFFFFULL)|(((rt>>4)& 1ULL)<<63); }
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_MOVZT_2R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;	
	vs=ctx->reg[op->rs];
	vn=	(vs&0x0000FFFFFFFFFFFFULL) ;
	ctx->reg[op->rn]=vn;
}

u64 X3VM_ALU_ROTQ(u64 val, int sh)
{
	u64 vc;
	int sh1;
	if(sh>0)
		{ sh1=(sh&63); vc=(val<<sh1)|(val>>(64-sh1)); }
	else if(sh<0)
		{ sh1=((-sh)&63); vc=(val>>sh1)|(val<<(64-sh1)); }
	else
		{ vc=val; }
	return(vc);
}

void X3VM_Opc_ROTLQ_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_ALU_ROTQ(vs, (sbyte)vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_ROTRQ_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_ALU_ROTQ(vs, -((sbyte)vt));
	ctx->reg[op->rn]=vn;
}
