void X3VM_Opc_PADDW_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PADDW(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PSUBW_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PSUBW(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PADDL_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PADDL(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PSUBL_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PSUBL(vs, vt);
	ctx->reg[op->rn]=vn;
}


void X3VM_Opc_PMULSW_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PMULSLW(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PMULUW_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PMULULW(vs, vt);
	ctx->reg[op->rn]=vn;
}


void X3VM_Opc_PMULSLW_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PMULSLW(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PMULULW_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PMULULW(vs, vt);
	ctx->reg[op->rn]=vn;
}


void X3VM_Opc_PMULSHW_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PMULSHW(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PMULUHW_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PMULUHW(vs, vt);
	ctx->reg[op->rn]=vn;
}


void X3VM_Opc_PADDF_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PADDF(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PSUBF_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PSUBF(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PMULF_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PMULF(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PDIVF_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PDIVF(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PMINF_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PMINF(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PMAXF_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PMAXF(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PADDXF_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs0, vs1, vt0, vt1, vn0, vn1;
	vs0=ctx->reg[op->rs+0];
	vs1=ctx->reg[op->rs+1];
	vt0=ctx->reg[op->rt+0];
	vt1=ctx->reg[op->rt+1];
	vn0=X3VM_PADDF(vs0, vt0);
	vn1=X3VM_PADDF(vs1, vt1);
	ctx->reg[op->rn+0]=vn0;
	ctx->reg[op->rn+1]=vn1;
}

void X3VM_Opc_PSUBXF_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs0, vs1, vt0, vt1, vn0, vn1;
	vs0=ctx->reg[op->rs+0];
	vs1=ctx->reg[op->rs+1];
	vt0=ctx->reg[op->rt+0];
	vt1=ctx->reg[op->rt+1];
	vn0=X3VM_PSUBF(vs0, vt0);
	vn1=X3VM_PSUBF(vs1, vt1);
	ctx->reg[op->rn+0]=vn0;
	ctx->reg[op->rn+1]=vn1;
}

void X3VM_Opc_PMULXF_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs0, vs1, vt0, vt1, vn0, vn1;
	vs0=ctx->reg[op->rs+0];
	vs1=ctx->reg[op->rs+1];
	vt0=ctx->reg[op->rt+0];
	vt1=ctx->reg[op->rt+1];
	vn0=X3VM_PMULF(vs0, vt0);
	vn1=X3VM_PMULF(vs1, vt1);
	ctx->reg[op->rn+0]=vn0;
	ctx->reg[op->rn+1]=vn1;
}

void X3VM_Opc_PADDH_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PADDH(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PSUBH_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PSUBH(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PMULH_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PMULH(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PDIVH_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PDIVH(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PMINH_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PMINH(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PMAXH_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PMAXH(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PADDXD_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs0, vs1, vt0, vt1, vn0, vn1;
	vs0=ctx->reg[op->rs+0];
	vs1=ctx->reg[op->rs+1];
	vt0=ctx->reg[op->rt+0];
	vt1=ctx->reg[op->rt+1];
	vn0=X3VM_FADD(vs0, vt0);
	vn1=X3VM_FADD(vs1, vt1);
	ctx->reg[op->rn+0]=vn0;
	ctx->reg[op->rn+1]=vn1;
}

void X3VM_Opc_PSUBXD_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs0, vs1, vt0, vt1, vn0, vn1;
	vs0=ctx->reg[op->rs+0];
	vs1=ctx->reg[op->rs+1];
	vt0=ctx->reg[op->rt+0];
	vt1=ctx->reg[op->rt+1];
	vn0=X3VM_FSUB(vs0, vt0);
	vn1=X3VM_FSUB(vs1, vt1);
	ctx->reg[op->rn+0]=vn0;
	ctx->reg[op->rn+1]=vn1;
}

void X3VM_Opc_PMULXD_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs0, vs1, vt0, vt1, vn0, vn1;
	vs0=ctx->reg[op->rs+0];
	vs1=ctx->reg[op->rs+1];
	vt0=ctx->reg[op->rt+0];
	vt1=ctx->reg[op->rt+1];
	vn0=X3VM_FMUL(vs0, vt0);
	vn1=X3VM_FMUL(vs1, vt1);
	ctx->reg[op->rn+0]=vn0;
	ctx->reg[op->rn+1]=vn1;
}

void X3VM_Opc_PACKUU_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PACKUU(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PACKLL_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PACKLL(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PACKUL_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PACKUL(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PACKLU_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_PACKLU(vs, vt);
	ctx->reg[op->rn]=vn;
}


void X3VM_Opc_FADD_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_FADD(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FSUB_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_FSUB(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FMUL_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_FMUL(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FDIV_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=X3VM_FDIV(vs, vt);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FMAC_4R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vu, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vu=ctx->reg[op->ru];
	vn=X3VM_FADD(vu, X3VM_FMUL(vs, vt));
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PMACF_4R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vu, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vu=ctx->reg[op->ru];
	vn=X3VM_PADDF(vu, X3VM_PMULF(vs, vt));
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PMACH_4R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vu, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vu=ctx->reg[op->ru];
	vn=X3VM_PADDH(vu, X3VM_PMULH(vs, vt));
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FMRS_4R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vu, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vu=ctx->reg[op->ru];
	vn=X3VM_FSUB(vu, X3VM_FMUL(vs, vt));
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PMRSF_4R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vu, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vu=ctx->reg[op->ru];
	vn=X3VM_PSUBF(vu, X3VM_PMULF(vs, vt));
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PMRSH_4R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vu, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vu=ctx->reg[op->ru];
	vn=X3VM_PSUBH(vu, X3VM_PMULH(vs, vt));
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FMAS_4R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vu, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vu=ctx->reg[op->ru];
	vn=X3VM_FSUB(X3VM_FMUL(vs, vt), vu);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PMASF_4R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vu, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vu=ctx->reg[op->ru];
	vn=X3VM_PSUBF(X3VM_PMULF(vs, vt), vu);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PMASH_4R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vu, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vu=ctx->reg[op->ru];
	vn=X3VM_PSUBH(X3VM_PMULH(vs, vt), vu);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FMRA_4R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vu, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vu=ctx->reg[op->ru];
	vn=X3VM_FADD(vu, X3VM_FMUL(vs, vt));
	vn^=0x8000000000000000ULL;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PMRAF_4R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vu, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vu=ctx->reg[op->ru];
	vn=X3VM_PADDF(vu, X3VM_PMULF(vs, vt));
	vn^=0x8000000080000000ULL;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PMRAH_4R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vu, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vu=ctx->reg[op->ru];
	vn=X3VM_PADDH(vu, X3VM_PMULH(vs, vt));
	vn^=0x8000800080008000ULL;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FMIN_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	i=X3VM_FCMP(vs, vt);
	vn=(i<0)?vs:vt;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FMAX_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	i=X3VM_FCMP(vs, vt);
	vn=(i>=0)?vs:vt;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FCVTDS_2R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	vs=ctx->reg[op->rs];
	vn=X3VM_ConvFp32ToFp64(vs);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FCVTDUS_2R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	vs=ctx->reg[op->rs];
	vn=X3VM_ConvFp32ToFp64(vs>>32);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FCVTDH_2R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	vs=ctx->reg[op->rs];
	vn=X3VM_ConvFp16ToFp64(vs);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FCVTSD_2R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	vs=ctx->reg[op->rs];
	vn=X3VM_ConvFp64ToFp32(vs);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FCVTHD_2R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	vs=ctx->reg[op->rs];
	vn=X3VM_ConvFp64ToFp16(vs);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FCVTDI_2R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	vs=ctx->reg[op->rs];
	*(double *)(&vn)=(s64)vs;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FCVTID_2R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	vs=ctx->reg[op->rs];
	vn=(s64)(*(double *)(&vs));
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FCVTSI_2R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	u32 fvn;
	vs=ctx->reg[op->rs];
	*(float *)(&fvn)=(s64)vs;
	ctx->reg[op->rn]=fvn;
}

void X3VM_Opc_FCVTIS_2R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	u32 fvn;
	vs=ctx->reg[op->rs];
	fvn=vs;
	vn=*(float *)(&fvn);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PCVTSH_2R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	vs=ctx->reg[op->rs];
	vn=         X3VM_ConvFp16ToFp32(vs>>16);
	vn=(vn<<32)|X3VM_ConvFp16ToFp32(vs>> 0);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PCVTHS_2R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	vs=ctx->reg[op->rs];
	vn=         X3VM_ConvFp32ToFp16(vs>>32);
	vn=(vn<<16)|X3VM_ConvFp32ToFp16(vs>> 0);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FEQ_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	i=X3VM_FCMP(vs, vt);
	vn=(i==0);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FLT_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	i=X3VM_FCMP(vs, vt);
	vn=(i==-1);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FGE_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	i=X3VM_FCMP(vs, vt);
	vn=(i>=0);
	ctx->reg[op->rn]=vn;
}


void X3VM_Opc_FEQF_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	i=X3VM_FCMPF(vs, vt);
	vn=(i==0);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FLTF_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	i=X3VM_FCMPF(vs, vt);
	vn=(i==-1);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FGEF_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	i=X3VM_FCMPF(vs, vt);
	vn=(i>=0);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FEQH_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	i=X3VM_FCMPH(vs, vt);
	vn=(i==0);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FLTH_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	i=X3VM_FCMPH(vs, vt);
	vn=(i==-1);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FGEH_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	i=X3VM_FCMPH(vs, vt);
	vn=(i>=0);
	ctx->reg[op->rn]=vn;
}



void X3VM_Opc_ZFEQ_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	i=X3VM_FCMP(vs, vt);
	vn=(i==0);
	ctx->pc_hibits=(ctx->pc_hibits&(~1))|(vn&1);
}

void X3VM_Opc_ZFLT_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	i=X3VM_FCMP(vs, vt);
	vn=(i==-1);
	ctx->pc_hibits=(ctx->pc_hibits&(~1))|(vn&1);
}

void X3VM_Opc_ZFGE_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn, i;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	i=X3VM_FCMP(vs, vt);
	vn=(i>=0);
	ctx->pc_hibits=(ctx->pc_hibits&(~1))|(vn&1);
}



void X3VM_Opc_PSQRTF_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vn=X3VM_PSQRTF(vs);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_PSQRTH_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vn=X3VM_PSQRTH(vs);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FSQRT_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vn=X3VM_FSQRT(vs);
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FSGNJ_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=	(vs&0x7FFFFFFFFFFFFFFFULL) |
		(vt&0x8000000000000000ULL) ;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FSGNJN_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=	(  vs &0x7FFFFFFFFFFFFFFFULL) |
		((~vt)&0x8000000000000000ULL) ;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FSGNJX_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=	(vs&0xFFFFFFFFFFFFFFFFULL) ^
		(vt&0x8000000000000000ULL) ;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FSGNJF_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=	(vs&0x7FFFFFFF7FFFFFFFULL) |
		(vt&0x8000000080000000ULL) ;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FSGNJNF_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=	(  vs &0x7FFFFFFF7FFFFFFFULL) |
		((~vt)&0x8000000080000000ULL) ;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FSGNJXF_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=	(vs&0xFFFFFFFFFFFFFFFFULL) ^
		(vt&0x8000000080000000ULL) ;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FSGNJH_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=	(vs&0x7FFF7FFF7FFF7FFFULL) |
		(vt&0x8000800080008000ULL) ;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FSGNJNH_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=	(  vs &0x7FFF7FFF7FFF7FFFULL) |
		((~vt)&0x8000800080008000ULL) ;
	ctx->reg[op->rn]=vn;
}

void X3VM_Opc_FSGNJXH_3R(X3VM_Context *ctx, X3VM_Opcode *op)
{
	u64 vs, vt, vn;
	vs=ctx->reg[op->rs];
	vt=ctx->reg[op->rt];
	vn=	(vs&0xFFFFFFFFFFFFFFFFULL) ^
		(vt&0x8000800080008000ULL) ;
	ctx->reg[op->rn]=vn;
}
