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

BGBCC_CCXL_BackendFuncs_vt bgbgc_jx2_vt;
static int bgbgc_jx2_init=0;

	
int BGBCC_JX2C_InitIface()
{
	if(bgbgc_jx2_init)
		return(0);
	bgbgc_jx2_init=1;

	bgbgc_jx2_vt.tgtname="JX2";
	
	bgbgc_jx2_vt.SetupContextForArch=	BGBCC_JX2C_SetupContextForArch;
	bgbgc_jx2_vt.FlattenImage=			BGBCC_JX2C_FlattenImage;
	bgbgc_jx2_vt.EndFunction=			BGBCC_JX2C_EndFunction;
	bgbgc_jx2_vt.LoadBufferDLL=			BGBCC_JX2C_LoadBufferDLL;
	bgbgc_jx2_vt.AddResourceData=		BGBCC_JX2C_AddResourceData;

	BGBCC_CCXL_RegisterBackend(&bgbgc_jx2_vt);

	return(1);
}

ccxl_status BGBCC_JX2C_LoadBufferDLL(BGBCC_TransState *ctx,
	byte *buf, int sz)
{
	BGBCC_JX2_Context *sctx;
	BGBCC_CCXL_RegisterInfo *obj;
	BGBCC_CCXL_LiteralInfo *litobj;
	int l0;
	u32 addr;
	int i, j, k;

	sctx=ctx->uctx;

	if((buf[0]=='M') && (buf[1]=='Z'))
	{
		i=BGBCC_JX2C_CoffLoadBufferDLL(ctx, sctx, buf, sz);
		return(i);
	}
	
	return(-1);
}

ccxl_status BGBCC_JX2C_SetupContextForArch(BGBCC_TransState *ctx)
{
	char tb[256];
	BGBCC_JX2_Context *shctx;
	int i, j, k;

	if(ctx->arch!=BGBCC_ARCH_BJX2)
		return(CCXL_STATUS_ERR_UNSUPPORTED);

	shctx=BGBCC_JX2_AllocContext();

	ctx->arch_sizeof_long=8;
	ctx->arch_sizeof_ptr=8;
	ctx->arch_sizeof_int=4;
//	ctx->arch_sizeof_valist=128;
	ctx->arch_sizeof_valist=192;
	ctx->arch_align_max=8;
//	ctx->arch_align_objmin=4;
	ctx->arch_align_objmin=1;
	ctx->uctx=shctx;

	ctx->pel_cmpr=4;
//	ctx->pel_cmpr=6;

	shctx->tctx=ctx;
	shctx->is_le=1;
	shctx->is_addr64=1;
	shctx->has_bjx1egpr=1;
	shctx->is_betav=1;
	shctx->has_bjx1r3mov=0;

//	shctx->use_memmdl=BGBCC_MEMMDL_TINY16;
//	shctx->use_memmdl=BGBCC_MEMMDL_MEDIUM24;

	shctx->has_shad=1;
	shctx->has_movi20=0;
	shctx->has_bjx1jmp=0;
	shctx->has_movi20=0;
	shctx->has_misalgn=1;
	shctx->has_bjx1mov=1;
	shctx->has_bjx1ari=1;
//	shctx->has_bjx1breq=0;
	shctx->has_bjx1breq=1;
	shctx->has_addsl=1;
	shctx->has_jcmp=0;
	shctx->has_pushx2=0;
	
	shctx->has_alux=0;		//128-bit ALU Ops
	shctx->has_fpux=0;		//128-bit FPU Ops
	
	shctx->has_jumbo=0;
	shctx->has_bra48=0;
	shctx->has_xgpr=0;
	shctx->has_fmovs=0;
	shctx->has_fmovc=0;
	shctx->has_dmacl=0;

//	shctx->do_shuffle=0;
	shctx->do_shuffle=1;

//	shctx->no_fpu=1;
	shctx->no_ext32=0;
//	shctx->fpu_soft=1;
	shctx->fpu_lite=0;
	shctx->fpu_gfp=0;
	shctx->is_addr_x32=0;
	shctx->use_padcross=0;
//	shctx->no_ops48=0;
	shctx->no_ops48=1;

	shctx->is_pbo=0;
//	shctx->use_wexmd=1;
	shctx->use_wexmd=0;
	shctx->no_wexify=0;
	shctx->use_wexdbg=0;
	
	shctx->has_qmul=0;

	shctx->has_ldop=0;
	shctx->has_bitmov=0;
	shctx->has_fpim=0;
	shctx->has_fpvsf=0;

//	shctx->no_wexify=1;		//Debug

//	shctx->fpu_gfp=1;
//	shctx->is_pbo=1;

//	shctx->is_align_wexj=3;
//	shctx->is_align_wexj=1;
	shctx->is_align_wexj=0;
	
	shctx->abi_evenonly=0;
	shctx->abi_noexcept=0;
	shctx->abi_spillpad=0;

//	shctx->abi_spillpad=1;
//	shctx->abi_spillpad|=2;

//	shctx->abi_spillpad|=8;		//global canary checks

	if(ctx->optmode==BGBCC_OPT_SIZE)
		shctx->use_wexmd=0;

	/* Enable shadow space for function arguments. */
	if(BGBCC_CCXL_CheckForOptStr(ctx, "argshadow"))
		shctx->abi_spillpad|= 1;
	
	/* Enable VSKGVSKC instructions. */
	if(BGBCC_CCXL_CheckForOptStr(ctx, "vskgen"))
		shctx->abi_spillpad|= 2;
	
	/* Enable bounds-checked pointers. */
	if(BGBCC_CCXL_CheckForOptStr(ctx, "dobounds"))
		shctx->abi_spillpad|= 4;
	
	/* use canary checks for every function */
	if(BGBCC_CCXL_CheckForOptStr(ctx, "vskglobal"))
		shctx->abi_spillpad|= 8;
	
	/* use mode-tagged function pointers. */
	if(BGBCC_CCXL_CheckForOptStr(ctx, "functag"))
		shctx->abi_spillpad|=16;

	/* enable LDIMI instruction. */
	if(BGBCC_CCXL_CheckForOptStr(ctx, "ldimi"))
		shctx->abi_spillpad|=32;

	/* disable canary checks */
	if(BGBCC_CCXL_CheckForOptStr(ctx, "novsk"))
		shctx->abi_spillpad|=64;

	/* use mode-tagged function pointers, UBSM enforce. */
	if(BGBCC_CCXL_CheckForOptStr(ctx, "functag_ex"))
	{
		shctx->abi_spillpad|=16;
		shctx->abi_spillpad|=256;
	}

	/* shuffle function order in the binary. */
	if(BGBCC_CCXL_CheckForOptStr(ctx, "shuffle"))
		shctx->do_shuffle|=1;

//	if(BGBCC_CCXL_CheckForOptStr(ctx, "nowex"))
//		shctx->use_wexmd=0;
	if(BGBCC_CCXL_CheckForOptStr(ctx, "wex2w"))
		shctx->use_wexmd=1;
	if(BGBCC_CCXL_CheckForOptStr(ctx, "wex3w"))
	{
		shctx->use_wexmd=2;
//		shctx->has_jumbo=1;
	}

	if(BGBCC_CCXL_CheckForOptStr(ctx, "wex3w"))
	{
		shctx->use_wexmd=2;
//		shctx->has_jumbo=1;
	}

	if(BGBCC_CCXL_CheckForOptStr(ctx, "wex3w_io2"))
	{
		shctx->use_wexmd=3;
//		shctx->has_jumbo=1;
	}

	if(BGBCC_CCXL_CheckForOptStr(ctx, "wex3w_fp2"))
	{
		shctx->use_wexmd=4;
//		shctx->has_jumbo=1;
	}

	if(BGBCC_CCXL_CheckForOptStr(ctx, "wex3w_io2fp2"))
	{
		shctx->use_wexmd=5;
//		shctx->has_jumbo=1;
	}

	if(BGBCC_CCXL_CheckForOptStr(ctx, "fpu_fpimm"))
	{
		shctx->has_fpim|=1;
		shctx->has_fpvsf|=2;		/* Has Full Binary32 */
	}

	if(BGBCC_CCXL_CheckForOptStr(ctx, "fpu_fpvsf"))
	{
		shctx->has_fpvsf|=1;
	}

	if(BGBCC_CCXL_CheckForOptStr(ctx, "fpu_fpvsf2"))
	{
		shctx->has_fpvsf|=3;
	}

	if(BGBCC_CCXL_CheckForOptStr(ctx, "loadop"))
		shctx->has_ldop|=1;

	if(BGBCC_CCXL_CheckForOptStr(ctx, "bitmov"))
		shctx->has_bitmov|=1;

	if(	BGBCC_CCXL_CheckForOptStr(ctx, "wexj") ||
		BGBCC_CCXL_CheckForOptStr(ctx, "jumbo"))
	{
#if 1
//		shctx->use_wexmd=2;
		shctx->has_jumbo=1;

		if(shctx->use_wexmd>=2)
		{
			shctx->has_pushx2=1;
			shctx->has_simdx2=1;
//			shctx->has_fmovs=1;
			shctx->has_fmovs|=3;
//			shctx->has_fmovc=1;
//			shctx->has_dmacl=1;
		}
#endif
	}

	if(BGBCC_CCXL_CheckForOptStr(ctx, "alux"))
		{ shctx->has_alux|=1; }
	if(BGBCC_CCXL_CheckForOptStr(ctx, "fpux"))
		{ shctx->has_fpux|=1; }
	if(BGBCC_CCXL_CheckForOptStr(ctx, "movc"))
		{ shctx->has_fmovc|=1; }

	if(BGBCC_CCXL_CheckForOptStr(ctx, "wexify"))
		{ shctx->no_wexify=0; }

	if(BGBCC_CCXL_CheckForOptStr(ctx, "ops24"))
		{ shctx->has_ops24=1; }

	if(BGBCC_CCXL_CheckForOptStr(ctx, "jcmp"))
		{ shctx->has_jcmp|=3; }
	if(BGBCC_CCXL_CheckForOptStr(ctx, "jcmpz"))
		{ shctx->has_jcmp|=1; }
	if(BGBCC_CCXL_CheckForOptStr(ctx, "jcmpr"))
		{ shctx->has_jcmp|=2; }

	if(ctx->sub_arch==BGBCC_ARCH_BJX2_XRVA)
	{
		/* For RISC-V Mode, Default to plain PE/COFF */
		ctx->pel_cmpr=255;
	}

	if(BGBCC_CCXL_CheckForOptStr(ctx, "pexe"))
		ctx->pel_cmpr=255;
	if(BGBCC_CCXL_CheckForOptStr(ctx, "pel0"))
		ctx->pel_cmpr=0;
	if(BGBCC_CCXL_CheckForOptStr(ctx, "pel3"))
		ctx->pel_cmpr=3;
	if(BGBCC_CCXL_CheckForOptStr(ctx, "pel4"))
		ctx->pel_cmpr=4;
	if(BGBCC_CCXL_CheckForOptStr(ctx, "pel4b"))
		ctx->pel_cmpr=6;
	if(BGBCC_CCXL_CheckForOptStr(ctx, "pel6"))
		ctx->pel_cmpr=6;


	if(BGBCC_CCXL_CheckForOptStr(ctx, "bra48"))
		{ shctx->has_bra48|=1; }
	if(BGBCC_CCXL_CheckForOptStr(ctx, "xgpr"))
		{ shctx->has_xgpr=1; }
	if(BGBCC_CCXL_CheckForOptStr(ctx, "xgpr_abi"))
		{ shctx->has_xgpr=3; }
	if(BGBCC_CCXL_CheckForOptStr(ctx, "xgpr_ena"))
		{ shctx->has_xgpr=5; }

	if(BGBCC_CCXL_CheckForOptStr(ctx, "dmacl"))
		{ shctx->has_dmacl|=1; }
	if(BGBCC_CCXL_CheckForOptStr(ctx, "qmul"))
		{ shctx->has_qmul|=1; }
	if(BGBCC_CCXL_CheckForOptStr(ctx, "mulq"))
		{ shctx->has_qmul|=1; }

	if(BGBCC_CCXL_CheckForOptStr(ctx, "fdiv"))
		{ shctx->has_qmul|=2; }

	if(BGBCC_CCXL_CheckForOptStr(ctx, "bcd"))
		{ shctx->has_qmul|=4; }

	if(BGBCC_CCXL_CheckForOptStr(ctx, "rsub"))
		{ shctx->has_qmul|=8; }

	if(BGBCC_CCXL_CheckForOptStr(ctx, "aluptr"))
		{ shctx->has_qmul|=16; }

	if(BGBCC_CCXL_CheckForOptStr(ctx, "cmp3r"))
		{ shctx->has_qmul|=32; }

	if(BGBCC_CCXL_CheckForOptStr(ctx, "ldtex"))
		{ shctx->has_fmovs|=4; }

	if(BGBCC_CCXL_CheckForOptStr(ctx, "ldgbr"))
		{ shctx->has_fmovs|=8; }
	if(BGBCC_CCXL_CheckForOptStr(ctx, "xmov"))
		{ shctx->has_fmovs|=16; }

	if(BGBCC_CCXL_CheckForOptStr(ctx, "mov48"))
		{ shctx->has_fmovs|=32; }
	if(BGBCC_CCXL_CheckForOptStr(ctx, "leagbr"))
		{ shctx->has_fmovs|=64; }

//	ctx->arch_has_predops=0;
	ctx->arch_has_predops=1;

	ctx->arch_has_imac=0;
	ctx->arch_has_fmac=0;

	if(BGBCC_CCXL_CheckForOptStr(ctx, "nopredops"))
		{ ctx->arch_has_predops=0; }

	shctx->optmode=ctx->optmode;
	shctx->verbose=ctx->verbose;

	if(ctx->sub_arch==BGBCC_ARCH_BJX2_JX2A)
	{
		shctx->no_ops48=1;
		shctx->fpu_gfp=1;
		shctx->is_pbo=1;

#if 1
//		if(shctx->use_wexmd==0)
		if((shctx->use_wexmd==0) && (ctx->optmode!=BGBCC_OPT_SIZE))
			shctx->use_wexmd=2;

		shctx->has_jumbo=1;
		shctx->has_pushx2=1;
		shctx->has_simdx2=1;

		shctx->has_fmovs|=3;

//		shctx->has_fmovc=1;
//		shctx->has_dmacl=1;

		shctx->abi_spillpad|=1;
		shctx->abi_spillpad|=2;
#endif
	}

	if(ctx->sub_arch==BGBCC_ARCH_BJX2_JX2B)
	{
		shctx->no_ops48=1;
		shctx->is_fixed32=1;

		shctx->fpu_gfp=1;
		shctx->is_pbo=1;

		shctx->abi_spillpad|=1;
//		shctx->abi_spillpad|=2;
	}

	if(ctx->sub_arch==BGBCC_ARCH_BJX2_JX2C)
	{
		ctx->arch_sizeof_long=4;
		ctx->arch_sizeof_ptr=4;
		shctx->is_addr_x32=1;
		shctx->no_ops48=1;
//		shctx->no_ops48=0;

		shctx->fpu_gfp=1;
		shctx->is_pbo=1;
	}

	if(ctx->sub_arch==BGBCC_ARCH_BJX2_JX2D)
	{
		ctx->arch_sizeof_long=4;
		ctx->arch_sizeof_ptr=4;
		shctx->is_fixed32=1;
		shctx->is_addr_x32=1;

//		shctx->fpu_gfp=1;
		shctx->no_fpu=1;
		shctx->fpu_soft=1;
		shctx->is_pbo=1;
	}

	if(ctx->sub_arch==BGBCC_ARCH_BJX2_JX2E)
	{
		ctx->arch_sizeof_long=4;
		ctx->arch_sizeof_ptr=4;
		shctx->is_addr_x32=1;
		shctx->no_ops48=1;

		shctx->no_fpu=1;
		shctx->fpu_soft=1;
		shctx->is_pbo=1;
	}

	if(ctx->sub_arch==BGBCC_ARCH_BJX2_JX2F)
	{
		ctx->arch_sizeof_long=4;
		ctx->arch_sizeof_ptr=4;
		shctx->is_addr_x32=1;
		shctx->no_ops48=1;

		shctx->fpu_gfp=1;
		shctx->is_pbo=1;
	}

	if(ctx->sub_arch==BGBCC_ARCH_BJX2_JX2G)
	{
		ctx->arch_sizeof_long=8;
		ctx->arch_sizeof_ptr=16;
		ctx->arch_align_max=16;
//		ctx->arch_sizeof_valist=192;

		shctx->no_ops48=1;
		shctx->fpu_gfp=1;
		shctx->is_pbo=1;

		shctx->has_bra48=1;
		shctx->has_xgpr=3;
		shctx->has_alux=1;

#if 1
//		if(shctx->use_wexmd==0)
		if((shctx->use_wexmd==0) && (ctx->optmode!=BGBCC_OPT_SIZE))
			shctx->use_wexmd=2;
		shctx->has_jumbo=1;
		shctx->has_pushx2=1;
		shctx->has_simdx2=1;

		shctx->has_fmovs|=3;
		
		shctx->has_fmovs|=16;

//		shctx->has_fmovc=1;
//		shctx->has_dmacl=1;

		shctx->abi_spillpad|=1;
		shctx->abi_spillpad|=2;
#endif
	}

	if(ctx->sub_arch==BGBCC_ARCH_BJX2_JX2H)
	{
		ctx->arch_sizeof_long=8;
		ctx->arch_sizeof_ptr=8;
		ctx->arch_align_max=16;
//		ctx->arch_sizeof_valist=192;

		shctx->no_ops48=1;
		shctx->fpu_gfp=1;
		shctx->is_pbo=1;

		shctx->has_bra48=1;
		shctx->has_xgpr=3;
		shctx->has_alux|=1;

		shctx->has_qmul|=8;

#if 1
//		if(shctx->use_wexmd==0)
		if((shctx->use_wexmd==0) && (ctx->optmode!=BGBCC_OPT_SIZE))
			shctx->use_wexmd=2;
		shctx->has_jumbo=1;
		shctx->has_pushx2=1;
		shctx->has_simdx2=1;

		shctx->has_fmovs|=3;
		shctx->has_fmovs|=16;

//		shctx->has_fmovc=1;
//		shctx->has_dmacl=1;

		shctx->abi_spillpad|=1;
		shctx->abi_spillpad|=2;
#endif
	}

	if(ctx->sub_arch==BGBCC_ARCH_BJX2_JX2I)
	{
		shctx->no_ops48=1;
		shctx->fpu_gfp=1;
		shctx->is_pbo=1;

#if 1
		if((shctx->use_wexmd==0) && (ctx->optmode!=BGBCC_OPT_SIZE))
			shctx->use_wexmd=1;

		shctx->has_jumbo=1;
		shctx->has_pushx2=1;
		shctx->has_simdx2=1;

//		shctx->has_fmovs|=3;

		shctx->abi_spillpad|=1;
		shctx->abi_spillpad|=2;
#endif
	}


	if(ctx->sub_arch==BGBCC_ARCH_BJX2_X2RV)
	{
		shctx->no_ops48=1;
		shctx->fpu_gfp=1;
		shctx->is_pbo=1;

		shctx->is_fixed32|=3;
		shctx->emit_riscv|=0x02;
		shctx->has_xgpr|=3;

#if 1
//		if(shctx->use_wexmd==0)
		if((shctx->use_wexmd==0) && (ctx->optmode!=BGBCC_OPT_SIZE))
			shctx->use_wexmd=2;

		shctx->has_jumbo=1;
		shctx->has_pushx2=1;
		shctx->has_simdx2=1;

		shctx->has_fmovs|=3;

		shctx->abi_spillpad|=1;
		shctx->abi_spillpad|=2;
#endif
	}

	if(ctx->sub_arch==BGBCC_ARCH_BJX2_XRVA)
	{
		/* RISC-V / RV64G */
		shctx->no_ops48=1;
		shctx->fpu_gfp=1;
		shctx->is_pbo=1;
		shctx->has_fpim=0;
		ctx->arch_has_predops=0;

		shctx->has_xgpr&=~7;
		shctx->has_jcmp|=3;
		shctx->has_qmul|=1;
		shctx->has_qmul|=32;
		shctx->has_fmovc|=1;		//

		shctx->is_fixed32|=3;
		shctx->emit_riscv|=0x01;
		shctx->use_wexmd=0;

		shctx->has_jumbo=0;
		shctx->has_pushx2=0;
		shctx->has_simdx2=0;

		if(BGBCC_CCXL_CheckForOptStr(ctx, "rvldix"))
		{
			shctx->has_rvzba|=16;	//Load/Store Indexed
			shctx->has_pushx2|=1;	//LX / SX
			shctx->has_rvzba|=2;	//BitManip Old, ADDWU/SUBWU
		}

		if(BGBCC_CCXL_CheckForOptStr(ctx, "rvjumbo") ||
			BGBCC_CCXL_CheckForOptStr(ctx, "rvjumbo96"))
		{
			shctx->has_jumbo|=2;	//Jumbo Prefixes
			shctx->has_rvzba|=16;	//Load/Store Indexed
			shctx->has_pushx2|=1;	//LX / SX

			shctx->has_rvzba|=2;	//BitManip Old, ADDWU/SUBWU
		}
		if(BGBCC_CCXL_CheckForOptStr(ctx, "rvjumbo96"))
		{
			shctx->has_jumbo|=4;	//Jumbo96
		}
		if(BGBCC_CCXL_CheckForOptStr(ctx, "rvzba"))
		{
			shctx->has_rvzba|=1;	//Zba
		}
		if(BGBCC_CCXL_CheckForOptStr(ctx, "rvbraimm"))
		{
			shctx->has_rvzba|=32;	//Branch-with-Immediate
		}

		if(BGBCC_CCXL_CheckForOptStr(ctx, "rvc"))
		{
			shctx->is_fixed32&=~3;
		}
	}

	if(ctx->sub_arch==BGBCC_ARCH_BJX2_X3RV)
	{
		/* RV64G + XG3 */
		shctx->no_ops48=1;
		shctx->fpu_gfp=1;
		shctx->is_pbo=1;
		shctx->has_fpim=0;
		ctx->arch_has_predops=0;
//		ctx->arch_has_predops=1;

		shctx->has_xgpr&=~7;

		shctx->is_fixed32|=3;
		shctx->emit_riscv|=0x03;
		shctx->use_wexmd=0;

		shctx->has_jumbo=1;
//		shctx->has_pushx2=1;
		shctx->has_pushx2=0;
		shctx->has_simdx2=0;


//		shctx->has_xgpr|=3;
		shctx->has_xgpr|=5;

		shctx->has_jcmp|=3;
		shctx->has_qmul|=1;
		shctx->has_qmul|=8;
		shctx->has_qmul|=32;
		shctx->has_fmovc|=1;		//

		shctx->has_jumbo=0;
		shctx->has_pushx2=0;
		shctx->has_simdx2=0;

		if(ctx->arch_has_predops)
			shctx->has_jcmp|=16;

		shctx->has_jumbo|=2;	//Jumbo Prefixes
		shctx->has_rvzba|=16;	//Load/Store Indexed
		shctx->has_pushx2|=1;	//LX / SX

		shctx->has_rvzba|=2;	//BitManip Old, ADDWU/SUBWU
//		shctx->has_jumbo|=4;	//Jumbo96
		shctx->has_rvzba|=1;	//Zba

		shctx->has_fmovs|=3;
	}

	if(ctx->sub_arch==BGBCC_ARCH_BJX2_XG2A)
	{
		ctx->arch_sizeof_long=8;
		ctx->arch_sizeof_ptr=8;
		ctx->arch_align_max=16;
//		ctx->arch_sizeof_valist=192;

		shctx->no_ops48=1;
		shctx->fpu_gfp=1;
		shctx->is_pbo=1;

		shctx->has_bra48=1;
		shctx->has_xgpr=3;
		shctx->has_alux=1;

#if 1
//		if(shctx->use_wexmd==0)
		if((shctx->use_wexmd==0) && (ctx->optmode!=BGBCC_OPT_SIZE))
			shctx->use_wexmd=2;
		shctx->has_jumbo=1;
		shctx->has_pushx2=1;
		shctx->has_simdx2=1;

		shctx->has_fmovs|=3;
		shctx->has_fmovs|=16;

//		shctx->has_fmovc=1;
//		shctx->has_dmacl=1;

		shctx->abi_spillpad|=1;
		shctx->abi_spillpad|=2;
#endif

		shctx->is_fixed32|=3;		//XG2
//		shctx->has_xgpr=3;			//XGPR ABI

		shctx->has_fmovs|=8;		//LDGBR
		shctx->has_fmovs|=64;		//LEAGBR

		shctx->has_jcmp|=1;			//JCMPZ

	}

	if(BGBCC_CCXL_CheckForOptStr(ctx, "xg2mode"))
		{ shctx->is_fixed32|=3; }

	if(BGBCC_CCXL_CheckForOptStr(ctx, "fix32"))
		{ shctx->is_fixed32|=1; }

	if(BGBCC_CCXL_CheckForOptStr(ctx, "nowex"))
		shctx->use_wexmd=0;

	if(BGBCC_CCXL_CheckForOptStr(ctx, "noargshadow"))
		shctx->abi_spillpad&=~1;
	if(BGBCC_CCXL_CheckForOptStr(ctx, "novskgen"))
		shctx->abi_spillpad&=~2;
	if(BGBCC_CCXL_CheckForOptStr(ctx, "nofunctag"))
		shctx->abi_spillpad&=~16;

	if(BGBCC_CCXL_CheckForOptStr(ctx, "noldimi"))
		shctx->abi_spillpad&=~32;


	if(BGBCC_CCXL_CheckForOptStr(ctx, "nomovx"))
		shctx->has_pushx2=0;
	if(BGBCC_CCXL_CheckForOptStr(ctx, "nosimdx"))
		shctx->has_simdx2=0;
	if(BGBCC_CCXL_CheckForOptStr(ctx, "nowexify"))
		shctx->no_wexify=1;

	if(BGBCC_CCXL_CheckForOptStr(ctx, "noexcept"))
		shctx->abi_noexcept=1;
	if(BGBCC_CCXL_CheckForOptStr(ctx, "no-exceptions"))
		shctx->abi_noexcept=1;

	if(BGBCC_CCXL_CheckForOptStr(ctx, "nojumbo"))
		shctx->has_jumbo=0;
	if(BGBCC_CCXL_CheckForOptStr(ctx, "nowexj"))
		shctx->has_jumbo=0;

	if(BGBCC_CCXL_CheckForOptStr(ctx, "noalux"))
		{ shctx->has_alux=0; }
	if(BGBCC_CCXL_CheckForOptStr(ctx, "nofpux"))
		{ shctx->has_fpux=0; }

	if(BGBCC_CCXL_CheckForOptStr(ctx, "nofmovs"))
		{ shctx->has_fmovs&=~3; }
	if(BGBCC_CCXL_CheckForOptStr(ctx, "nomovc"))
		{ shctx->has_fmovc=0; }

	if(BGBCC_CCXL_CheckForOptStr(ctx, "noldtex"))
		{ shctx->has_fmovs&=~4; }

	if(BGBCC_CCXL_CheckForOptStr(ctx, "nodmacl"))
		{ shctx->has_dmacl=0; }

	if(BGBCC_CCXL_CheckForOptStr(ctx, "noshuffle"))
		shctx->do_shuffle=0;

	if(BGBCC_CCXL_CheckForOptStr(ctx, "pbostatic"))
		shctx->is_pbo=3;

	ctx->arch_has_imac=shctx->has_dmacl;
	ctx->arch_has_fmac=0;

	if(shctx->has_pushx2 || shctx->has_simdx2)
		shctx->abi_evenonly = 1;

	if(ctx->imgfmt==BGBCC_IMGFMT_SYS)
	{
		if(shctx->emit_riscv&0x11)
		{
			shctx->is_pbo=3;
		}
		else
		{
			shctx->is_pbo=0;
		}
	}

	BGBPP_AddStaticDefine(NULL, "__BGBCC__", "");

	BGBPP_AddStaticDefine(NULL, "__STDC__", "");

	if(shctx->emit_riscv&0x11)
	{
		if(shctx->emit_riscv&0x22)
		{
			BGBPP_AddStaticDefine(NULL, "__XG3__", "");
		}

		BGBPP_AddStaticDefine(NULL, "__RISCV__", "");
	}else
	{
		BGBPP_AddStaticDefine(NULL, "__jx2__", "");
		BGBPP_AddStaticDefine(NULL, "__BGBCC_JX2__", "");
		BGBPP_AddStaticDefine(NULL, "__BJX2__", "");
	}


	ctx->arch_endian=1;
	ctx->arch_unaligned=1;

	BGBCC_CCXL_SetupContextTargetDefines(ctx);

	if(ctx->arch_sizeof_ptr==4)
	{
		BGBPP_AddStaticDefine(NULL, "__ADDR_32__", "");
//		BGBPP_AddStaticDefine(NULL, "__SIZEOF_POINTER__", "4");
	}

	if(ctx->arch_sizeof_ptr==8)
	{
		BGBPP_AddStaticDefine(NULL, "__ADDR_64__", "");
		BGBPP_AddStaticDefine(NULL, "__ADDR_X48__", "");
//		BGBPP_AddStaticDefine(NULL, "__SIZEOF_POINTER__", "8");
	}

	if(ctx->arch_sizeof_ptr==16)
	{
		BGBPP_AddStaticDefine(NULL, "__ADDR_128__", "");
		BGBPP_AddStaticDefine(NULL, "__ADDR_X96__", "");
//		BGBPP_AddStaticDefine(NULL, "__SIZEOF_POINTER__", "16");
	}

	if(shctx->is_addr_x32)
	{
		BGBPP_AddStaticDefine(NULL, "__ADDR_X32__", "");
	}
	
	if(shctx->no_fpu)
		BGBPP_AddStaticDefine(NULL, "__NOFPU__", "");

	k=32+4;
	if(shctx->has_xgpr&1)
		k+=32;
	
	ctx->arch_sizeof_regsave=k*8;
		
//	sprintf(tb, "%d", (k*8));
//	BGBPP_AddStaticDefine(NULL, "__ARCH_SIZEOF_REGSAVE__", bgbcc_strdup(tb));
	BGBPP_AddStaticDefine(NULL, "__ARCH_SIZEOF_REGSAVE__", 
		"__arch_sizeof_regsave__");

	return(0);
}

bool BGBCC_JX2C_TypeShortRegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_JX2_Context *sctx;

	sctx=ctx->uctx;
	if(sctx->is_addr64)
		return(false);
	
//	if(BGBCC_CCXL_TypeSgByteP(ctx, ty))
//		return(true);
//	if(BGBCC_CCXL_TypeSignedShortP(ctx, ty))
//		return(true);

	return(false);
}

bool BGBCC_JX2C_TypeIntRegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_JX2_Context *sctx;

	sctx=ctx->uctx;
	
	if(BGBCC_CCXL_TypeSmallIntP(ctx, ty))
		return(true);
		
	if(ctx->arch_sizeof_ptr==4)
	{
		if(BGBCC_CCXL_TypePointerP(ctx, ty))
			return(true);
		if(BGBCC_CCXL_TypeFunctionP(ctx, ty))
			return(true);
	}

	if(sctx->no_fpu || sctx->fpu_soft)
	{
		if(sctx->fpu_gfp)
		{
			if(BGBCC_CCXL_TypeFloatP(ctx, ty))
				return(false);
			if(BGBCC_CCXL_TypeFloat16P(ctx, ty))
				return(false);
		}

		if(BGBCC_CCXL_TypeFloatP(ctx, ty))
			return(true);
		if(BGBCC_CCXL_TypeFloat16P(ctx, ty))
			return(true);
//		if(BGBCC_CCXL_TypeBFloat16P(ctx, ty))
//			return(true);
	}

	if(ty.val==0x000F)
		return(true);

	return(false);
}

bool BGBCC_JX2C_TypeInt2RegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_JX2_Context *sctx;

	sctx=ctx->uctx;
	
	if(	(ctx->arch_sizeof_ptr==8) ||
		(ctx->arch_sizeof_ptr==16))
	{
		return(false);
	}

	if(sctx->is_addr64)
	{
		return(false);
	}

	if(ctx->arch_sizeof_ptr==4)
	{
		if(BGBCC_CCXL_TypeSmallLongP(ctx, ty))
			return(true);

		if(BGBCC_CCXL_TypeVoidP(ctx, ty))
			return(true);
		if(BGBCC_CCXL_TypeVariantP(ctx, ty))
			return(true);

		if(sctx->no_fpu || sctx->fpu_soft)
		{
			if(BGBCC_CCXL_TypeDoubleP(ctx, ty))
				return(true);
		}
	}

	return(false);
}

bool BGBCC_JX2C_TypeFloatRegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_JX2_Context *sctx;

	sctx=ctx->uctx;
	
	if(sctx->no_fpu || sctx->fpu_soft || sctx->fpu_gfp)
		return(0);

	if(BGBCC_CCXL_TypeFloatP(ctx, ty))
		return(true);
	if(BGBCC_CCXL_TypeFloat16P(ctx, ty))
		return(true);
	if(BGBCC_CCXL_TypeBFloat16P(ctx, ty))
		return(true);

//	if(BGBCC_CCXL_TypeDoubleP(ctx, ty))
//		return(true);

	return(false);
}

bool BGBCC_JX2C_TypeFloat2RegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_JX2_Context *sctx;

	sctx=ctx->uctx;
	
	if(sctx->no_fpu || sctx->fpu_soft || sctx->fpu_gfp)
		return(0);
	
//	if(BGBCC_CCXL_TypeDoubleP(ctx, ty))
//		return(true);
	return(false);
}

bool BGBCC_JX2C_TypeDoubleRegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_JX2_Context *sctx;

	sctx=ctx->uctx;
	
	if(sctx->no_fpu || sctx->fpu_soft || sctx->fpu_gfp)
		return(0);
	
	if(BGBCC_CCXL_TypeDoubleP(ctx, ty))
		return(true);
	return(false);
}

int BGBCC_JX2C_TypeGetRegClassP(BGBCC_TransState *ctx, ccxl_type ty)
{
//	static byte rcls_lut[65536];
	static byte rcls_lut[4096];
	static u32 rcls_hash_ty[4096];
	static byte rcls_hash_rcls[4096];
	static BGBCC_TransState *p_ctx=NULL;
	int rc, ty1;
	int i, h;

	if(p_ctx!=ctx)
	{
		p_ctx=ctx;
//		for(i=0; i<65536; i++)
		for(i=0; i<4096; i++)
		{
			rcls_lut[i]=BGBCC_SH_REGCLS_NONE;
			rcls_hash_rcls[i]=BGBCC_SH_REGCLS_NONE;
		}
	}

//	if(ty.val<0)
//	{
//		{ BGBCC_DBGBREAK }
//	}

//	if(ty.val<65536)
	if(ty.val<4096)
	{
		rc=rcls_lut[ty.val];
		if(rc!=BGBCC_SH_REGCLS_NONE)
		{
			return(rc);
		}

		if(ty.val==0x49)
			{ i=-1; }

		rc=BGBCC_JX2C_TypeGetRegClassPI(ctx, ty);
		if(rc==BGBCC_SH_REGCLS_NONE)
		{
			rc=BGBCC_JX2C_TypeGetRegClassPI(ctx, ty);
		}

		rcls_lut[ty.val]=rc;
		return(rc);
	}
	
	h=ty.val;
	h=(h*251)+(h>>23)+1;
	h=(h*251)+(h>>23)+1;
	h=(h*251)+(h>>23)+1;
	h=(h*251)+(h>>23)+1;
	h=(h>>20)&4095;

	rc=rcls_hash_rcls[h];
	ty1=rcls_hash_ty[h];
	if((rc!=BGBCC_SH_REGCLS_NONE) && (ty1==ty.val))
	{
		return(rc);
	}

	if(ty.val==0x49)
		{ i=-1; }

	rc=BGBCC_JX2C_TypeGetRegClassPI(ctx, ty);
	rcls_hash_rcls[h]=rc;
	rcls_hash_ty[h]=ty.val;

	if(rc==BGBCC_SH_REGCLS_NONE)
	{
		rc=BGBCC_JX2C_TypeGetRegClassPI(ctx, ty);
	}

	return(rc);
}

int BGBCC_JX2C_TypeGetRegClassPI(BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_JX2_Context *sctx;
	int sz;

	sctx=ctx->uctx;

	if(ty.val==CCXL_TY_PIL)
		{ return(BGBCC_SH_REGCLS_QGR); }

	if(BGBCC_CCXL_TypeBitIntP(ctx, ty))
	{
		sz=-1;
	}

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
	{
#if 1
		if(ctx->arch_sizeof_ptr==16)
		{
			if(BGBCC_CCXL_TypeArrayP(ctx, ty))
				return(BGBCC_SH_REGCLS_AR_REF2);

//			if(BGBCC_CCXL_TypeNearPointerP(ctx, ty))
//				{ return(BGBCC_SH_REGCLS_QGR); }
//			if(BGBCC_CCXL_TypeFarPointerP(ctx, ty))
//				{ return(BGBCC_SH_REGCLS_QGR2); }
//			if(BGBCC_CCXL_TypeHugePointerP(ctx, ty))
//				{ return(BGBCC_SH_REGCLS_QGR2); }
			return(BGBCC_SH_REGCLS_QGR2);
		}
#endif

		if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		{
			if(ctx->arch_sizeof_ptr==16)
				return(BGBCC_SH_REGCLS_AR_REF2);

			return(BGBCC_SH_REGCLS_AR_REF);
		}

		if(ctx->arch_sizeof_ptr==8)
		{
			if(BGBCC_CCXL_TypeNearPointerP(ctx, ty))
				{ return(BGBCC_SH_REGCLS_QGR); }
			if(BGBCC_CCXL_TypeFarPointerP(ctx, ty))
				{ return(BGBCC_SH_REGCLS_QGR2); }
			if(BGBCC_CCXL_TypeHugePointerP(ctx, ty))
				{ return(BGBCC_SH_REGCLS_QGR2); }
		}else
		{
			if(BGBCC_CCXL_TypeNearPointerP(ctx, ty))
				{ return(BGBCC_SH_REGCLS_GR); }
			if(BGBCC_CCXL_TypeFarPointerP(ctx, ty))
				{ return(BGBCC_SH_REGCLS_QGR); }
			if(BGBCC_CCXL_TypeHugePointerP(ctx, ty))
				{ return(BGBCC_SH_REGCLS_QGR2); }
		}
		
		if(	BGBCC_CCXL_TypeRefArrayP(ctx, ty) ||
			BGBCC_CCXL_TypeSquareArrayP(ctx, ty))
		{
			return(BGBCC_SH_REGCLS_QGR);
		}

//		if(BGBCC_CCXL_TypePointerP(ctx, ty))
		if(1)
		{
			if(ctx->arch_sizeof_ptr==8)
				return(BGBCC_SH_REGCLS_QGR);
			
//			{ BGBCC_DBGBREAK }
			return(BGBCC_SH_REGCLS_GR);
		}
	}

	if(BGBCC_JX2C_TypeShortRegP(ctx, ty))
		return(BGBCC_SH_REGCLS_WGR);

	if(BGBCC_JX2C_TypeIntRegP(ctx, ty))
		return(BGBCC_SH_REGCLS_GR);
	if(BGBCC_JX2C_TypeInt2RegP(ctx, ty))
		return(BGBCC_SH_REGCLS_GR2);

	if(BGBCC_JX2C_TypeFloatRegP(ctx, ty))
		return(BGBCC_SH_REGCLS_FR);
	if(BGBCC_JX2C_TypeFloat2RegP(ctx, ty))
		return(BGBCC_SH_REGCLS_FR2);
	if(BGBCC_JX2C_TypeDoubleRegP(ctx, ty))
		return(BGBCC_SH_REGCLS_DR);

	if(BGBCC_CCXL_TypeVec64P(ctx, ty))
	{
//		return(BGBCC_SH_REGCLS_VO_QGR);
		return(BGBCC_SH_REGCLS_QGR);
	}

	if(BGBCC_CCXL_TypeVecAs64P(ctx, ty))
	{
//		return(BGBCC_SH_REGCLS_VO_QGR);
		return(BGBCC_SH_REGCLS_QGR);
	}

	if(BGBCC_CCXL_TypeVec128P(ctx, ty))
	{
//		return(BGBCC_SH_REGCLS_VO_QGR2);
		return(BGBCC_SH_REGCLS_QGR2);
	}

	if(BGBCC_CCXL_TypeVec256P(ctx, ty))
	{
		if(ctx->arch_sizeof_ptr==16)
			return(BGBCC_SH_REGCLS_VO_REF2);

		return(BGBCC_SH_REGCLS_VO_REF);
	}

	if(BGBCC_CCXL_TypeVarRefP(ctx, ty))
	{
		if(ctx->arch_sizeof_ptr==16)
			return(BGBCC_SH_REGCLS_QGR2);

		return(BGBCC_SH_REGCLS_QGR);
	}

	if(BGBCC_CCXL_TypeValueObjectP(ctx, ty))
	{
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, ty);

#if 1
		if(sz<=4)
			return(BGBCC_SH_REGCLS_VO_GR);
		if(sz<=8)
		{
//			if(ctx->arch_sizeof_ptr==8)
			if(sctx->is_addr64)
				return(BGBCC_SH_REGCLS_VO_QGR);
			return(BGBCC_SH_REGCLS_VO_GR2);
		}
		if(sz<=16)
		{
			return(BGBCC_SH_REGCLS_VO_QGR2);
		}
#endif

		if(ctx->arch_sizeof_ptr==16)
			return(BGBCC_SH_REGCLS_VO_REF2);

		return(BGBCC_SH_REGCLS_VO_REF);
	}

	if(ctx->arch_sizeof_ptr==16)
	{
		if(BGBCC_CCXL_TypeArrayP(ctx, ty))
			return(BGBCC_SH_REGCLS_AR_REF2);

		if(BGBCC_CCXL_TypeVariantP(ctx, ty))
			return(BGBCC_SH_REGCLS_QGR2);

		if(BGBCC_CCXL_TypePointerP(ctx, ty))
			return(BGBCC_SH_REGCLS_QGR2);
		if(BGBCC_CCXL_TypeFunctionP(ctx, ty))
			return(BGBCC_SH_REGCLS_QGR2);
	}

	if(ctx->arch_sizeof_ptr==8)
	{
		if(BGBCC_CCXL_TypeSmallLongP(ctx, ty))
			return(BGBCC_SH_REGCLS_QGR);
		if(BGBCC_CCXL_TypeVoidP(ctx, ty))
			return(BGBCC_SH_REGCLS_QGR);
		if(BGBCC_CCXL_TypeVariantP(ctx, ty))
			return(BGBCC_SH_REGCLS_QGR);

		if(BGBCC_CCXL_TypePointerP(ctx, ty))
			return(BGBCC_SH_REGCLS_QGR);
		if(BGBCC_CCXL_TypeFunctionP(ctx, ty))
			return(BGBCC_SH_REGCLS_QGR);

		if(sctx->fpu_gfp)
		{
			if(BGBCC_CCXL_TypeDoubleP(ctx, ty))
				return(BGBCC_SH_REGCLS_QGR);
			if(BGBCC_CCXL_TypeFloatP(ctx, ty))
				return(BGBCC_SH_REGCLS_QGR);
			if(BGBCC_CCXL_TypeFloat16P(ctx, ty))
				return(BGBCC_SH_REGCLS_QGR);
			if(BGBCC_CCXL_TypeFloat8P(ctx, ty))
				return(BGBCC_SH_REGCLS_QGR);
		}

		if(sctx->fpu_soft)
		{
			if(BGBCC_CCXL_TypeDoubleP(ctx, ty))
				return(BGBCC_SH_REGCLS_QGR);

			if(BGBCC_CCXL_TypeFloatP(ctx, ty))
				return(BGBCC_SH_REGCLS_GR);
			if(BGBCC_CCXL_TypeFloat16P(ctx, ty))
				return(BGBCC_SH_REGCLS_GR);

			if(BGBCC_CCXL_TypeFloat8P(ctx, ty))
				return(BGBCC_SH_REGCLS_GR);
		}
	}

	if(sctx->is_addr64)
	{
		if(BGBCC_CCXL_TypeSmallLongP(ctx, ty))
			return(BGBCC_SH_REGCLS_QGR);
		if(BGBCC_CCXL_TypeVoidP(ctx, ty))
			return(BGBCC_SH_REGCLS_QGR);
		if(BGBCC_CCXL_TypeVariantP(ctx, ty))
			return(BGBCC_SH_REGCLS_QGR);

		if(BGBCC_CCXL_TypePointerP(ctx, ty))
			return(BGBCC_SH_REGCLS_GR);
		if(BGBCC_CCXL_TypeFunctionP(ctx, ty))
			return(BGBCC_SH_REGCLS_GR);

		if(sctx->fpu_gfp)
		{
			if(BGBCC_CCXL_TypeDoubleP(ctx, ty))
				return(BGBCC_SH_REGCLS_QGR);
			if(BGBCC_CCXL_TypeFloatP(ctx, ty))
				return(BGBCC_SH_REGCLS_QGR);
			if(BGBCC_CCXL_TypeFloat16P(ctx, ty))
				return(BGBCC_SH_REGCLS_QGR);
			if(BGBCC_CCXL_TypeFloat8P(ctx, ty))
				return(BGBCC_SH_REGCLS_QGR);
		}

		if(sctx->fpu_soft)
		{
			if(BGBCC_CCXL_TypeDoubleP(ctx, ty))
				return(BGBCC_SH_REGCLS_QGR);

			if(BGBCC_CCXL_TypeFloatP(ctx, ty))
				return(BGBCC_SH_REGCLS_GR);
			if(BGBCC_CCXL_TypeFloat16P(ctx, ty))
				return(BGBCC_SH_REGCLS_GR);
			if(BGBCC_CCXL_TypeFloat8P(ctx, ty))
				return(BGBCC_SH_REGCLS_GR);
		}
	}

	if(BGBCC_CCXL_TypeFatPointerP(ctx, ty))
	{
		return(BGBCC_SH_REGCLS_VO_QGR2);
	}

	if(BGBCC_CCXL_TypeBitIntP(ctx, ty))
	{
		sz=BGBCC_CCXL_TypeGetBitIntSize(ctx, ty);
		
		if(sz<=64)
			return(BGBCC_SH_REGCLS_QGR);
		if(sz<=128)
			return(BGBCC_SH_REGCLS_QGR2);

		if(ctx->arch_sizeof_ptr==16)
			return(BGBCC_SH_REGCLS_VO_REF2);

		return(BGBCC_SH_REGCLS_VO_REF);
	}

	return(BGBCC_SH_REGCLS_NONE);
}

int BGBCC_JX2C_TypeValueObjectRefP(
	BGBCC_TransState *ctx,
	ccxl_type type)
{
	int j;
	j=BGBCC_JX2C_TypeGetRegClassP(ctx, type);
	if(j==BGBCC_SH_REGCLS_VO_REF)
		return(1);
	if(j==BGBCC_SH_REGCLS_VO_REF2)
		return(1);
	return(0);
}


int BGBCC_JX2C_EmitVaArg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register dreg,
	ccxl_register sreg)
{
	int i, j, k;

//	if(sctx->is_addr64)
//	if(ctx->arch_sizeof_ptr==8)
	if(1)
	{
		BGBCC_JX2C_EmitTempLoadReg(ctx, sctx, sreg);
//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ4);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_ARG(0));
//		BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_RQ4);
		BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, sreg,
			BGBCC_JX2CC_PSREG_ARG(0));
	}
	else
	{
		BGBCC_JX2C_EmitTempLoadReg(ctx, sctx, sreg);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
	}

	j=BGBCC_JX2C_TypeGetRegClassP(ctx, type);
	switch(j)
	{
	case BGBCC_SH_REGCLS_GR:
	case BGBCC_SH_REGCLS_VO_GR:
	case BGBCC_SH_REGCLS_VO_REF:
	case BGBCC_SH_REGCLS_WGR:
		if(sctx->abi_spillpad&1)
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__va64_arg_l");
		else
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__va64_arg_i");

//		if(sctx->is_addr64)
//			BGBCC_JX2C_EmitCallName(ctx, sctx, "__va64_arg_i");
//		else
//			BGBCC_JX2C_EmitCallName(ctx, sctx, "__va_arg_i");

		BGBCC_JX2C_ResetModeDqUnknown(ctx, sctx);

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_ARG(0));

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R2);
//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R2);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R2);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_RDRET);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_JX2CC_PSREG_RDRET);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_RDRET);

		break;

	case BGBCC_SH_REGCLS_VO_QGR:
	case BGBCC_SH_REGCLS_QGR:
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__va64_arg_l");

//		if(sctx->is_addr64)
//			BGBCC_JX2C_EmitCallName(ctx, sctx, "__va64_arg_l");
//		else
//			BGBCC_JX2C_EmitCallName(ctx, sctx, "__va_arg_l");

		BGBCC_JX2C_ResetModeDqUnknown(ctx, sctx);

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_ARG(0));

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R2);
//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R2);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R2);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_RQRET);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_JX2CC_PSREG_RQRET);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_RQRET);

		break;

	case BGBCC_SH_REGCLS_GR2:
	case BGBCC_SH_REGCLS_VO_GR2:
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__va64_arg_x");

//		if(sctx->is_addr64)
//			BGBCC_JX2C_EmitCallName(ctx, sctx, "__va64_arg_x");
//		else
//			BGBCC_JX2C_EmitCallName(ctx, sctx, "__va_arg_x");

		BGBCC_JX2C_ResetModeDqUnknown(ctx, sctx);

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_ARG(0));

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR2);
//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR2);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR2);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_LRRET);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_JX2CC_PSREG_LRRET);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_LRRET);

		break;

	case BGBCC_SH_REGCLS_VO_REF2:
	case BGBCC_SH_REGCLS_AR_REF2:
	case BGBCC_SH_REGCLS_VO_QGR2:
	case BGBCC_SH_REGCLS_QGR2:
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__va64_arg_x");
		BGBCC_JX2C_ResetModeDqUnknown(ctx, sctx);

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_ARG(0));

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR2);
//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR2);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR2);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_LRRET);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_JX2CC_PSREG_LRRET);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_LRRET);

		break;

#if 0
	case BGBCC_SH_REGCLS_FR:
		if(sctx->is_addr64)
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__va64_arg_f");
		else
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__va_arg_f");

		BGBCC_JX2C_ResetModeDqUnknown(ctx, sctx);

//		BGBCC_JX2C_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_FR0);
		BGBCC_JX2C_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_FR2);
		break;
#endif

#if 0
	case BGBCC_SH_REGCLS_FR2:
		if(sctx->is_addr64)
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__va64_arg_d");
		else
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__va_arg_d");

		BGBCC_JX2C_ResetModeDqUnknown(ctx, sctx);

//		BGBCC_JX2C_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR0);
		BGBCC_JX2C_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR2);
		break;

	case BGBCC_SH_REGCLS_DR:
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__va64_arg_d");
		BGBCC_JX2C_ResetModeDqUnknown(ctx, sctx);
//		BGBCC_JX2C_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR0);
		BGBCC_JX2C_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR2);
		break;
#endif

	default:
		BGBCC_DBGBREAK
		break;
	}

//	BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
//	BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_ARG(0));

	return(1);
}

int BGBCC_JX2C_EmitVaEnd(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register sreg)
{
	return(0);
}

int BGBCC_JX2C_EmitVaStart(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register sreg,
	ccxl_register treg)
{
	BGBCC_CCXL_RegisterInfo *obj;
	int ni, nf, sofs;
	int ix, kx;
	int i, j, k;
	
	ix=treg.val&4095;
	sofs=999999;
	
	obj=ctx->cur_func;

	return(0);
}

char *BGBCC_JX2C_DebugRegToStr(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register reg)
{
	char tb[256];
	char *s0;
	s64 li;
	int i;
	
	if(reg.val==CCXL_REGID_REG_Z)
	{
		sprintf(tb, "ZZ");
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_LONG)
	{
		li=reg.val<<3; li=li>>3;
		sprintf(tb, "I64(%lld)", li);
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_DOUBLE)
	{
		li=reg.val<<3;
		sprintf(tb, "F64(%g)", *(double *)(&li));
		return(bgbcc_rstrdup(tb));
	}
	
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
	{
		sprintf(tb, "T%d.%d",
			(int)(reg.val&CCXL_REGID_BASEMASK),
			(int)((reg.val&CCXL_REGID_SEQMASK)>>12));
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG)
	{
		sprintf(tb, "A%d.%d",
			(int)(reg.val&CCXL_REGID_BASEMASK),
			(int)((reg.val&CCXL_REGID_SEQMASK)>>12));
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL)
	{
		sprintf(tb, "L%d.%d",
			(int)(reg.val&CCXL_REGID_BASEMASK),
			(int)((reg.val&CCXL_REGID_SEQMASK)>>12));
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_GLOBAL)
	{
		i=(int)(reg.val&CCXL_REGID_REGMASK);
//		i=(int)(reg.val);
		s0=ctx->reg_globals[i]->name;
		if(!s0)s0="?";
		sprintf(tb, "G%d(%s)", i, s0);
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT)
	{
		sprintf(tb, "I32(%d)", (int)(reg.val));
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG)
	{
		li=reg.val<<8; li=li>>8;
		sprintf(tb, "I64(%lld)", li);
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_FLOAT)
	{
		i=(int)(reg.val);
		sprintf(tb, "F32(%g)", *(float *)(&i));
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE)
	{
		li=reg.val<<8;
		sprintf(tb, "F64(%g)", *(double *)(&li));
		return(bgbcc_rstrdup(tb));
	}

	sprintf(tb, "%016llX", reg.val);
	return(bgbcc_rstrdup(tb));
}

ccxl_status BGBCC_JX2C_PrintVirtOp(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, BGBCC_CCXL_VirtOp *op)
{
	ccxl_register *ca;
	char *s0;
	int na;
	int i, j, k;

#if 1
	if(sctx->cgen_log)
	{
//		fprintf(sctx->cgen_log,
//			"\t%04X ty=%08X",
//			op->opn, op->type.val);

		s0=NULL;
		switch(op->opn)
		{
			case CCXL_VOP_NONE:				s0="NONE"; break;
			case CCXL_VOP_DBGFN:			s0="DBGFN"; break;
			case CCXL_VOP_DBGLN:			s0="DBGLN"; break;
			case CCXL_VOP_LABEL:			s0="LABEL"; break;
			case CCXL_VOP_JMP:				s0="JMP"; break;
			case CCXL_VOP_MOV:				s0="MOV"; break;
			case CCXL_VOP_JCMP_ZERO:		s0="JCMP_Z"; break;
			case CCXL_VOP_JCMP:				s0="JCMP"; break;
			case CCXL_VOP_CALL:				s0="CALL"; break;
			case CCXL_VOP_CSRV:				s0="CSRV"; break;
			case CCXL_VOP_RETDFL:			s0="RETDFL"; break;
			case CCXL_VOP_RETV:				s0="RETV"; break;
			case CCXL_VOP_RET:				s0="RET"; break;
			case CCXL_VOP_CONV:				s0="CONV"; break;
			case CCXL_VOP_UNARY:			s0="UNARY"; break;
			case CCXL_VOP_BINARY:			s0="BINARY"; break;
			case CCXL_VOP_COMPARE:			s0="COMPARE"; break;
			case CCXL_VOP_TRINARY:			s0="TRINARY"; break;
			case CCXL_VOP_LDIXIMM:			s0="LDIXIMM"; break;
			case CCXL_VOP_STIXIMM:			s0="STIXIMM"; break;
			case CCXL_VOP_LDIX:				s0="LDIX"; break;
			case CCXL_VOP_STIX:				s0="STIX"; break;
			case CCXL_VOP_LEAIMM:			s0="LEAIMM"; break;
			case CCXL_VOP_LEA:				s0="LEA"; break;
			case CCXL_VOP_LDAVAR:			s0="LDAVAR"; break;
			case CCXL_VOP_SIZEOFVAR:		s0="SIZEOFVAR"; break;
			case CCXL_VOP_DIFFPTR:			s0="DIFFPTR"; break;
			case CCXL_VOP_OFFSETOF:			s0="OFFSETOF"; break;
			case CCXL_VOP_LOADSLOT:			s0="LOADSLOT"; break;
			case CCXL_VOP_STORESLOT:		s0="STORESLOT"; break;
			case CCXL_VOP_LOADSLOTADDR:		s0="LOADSLOTA"; break;
			case CCXL_VOP_LOADSLOTADDRID:	s0="LOADSLOTA"; break;
			case CCXL_VOP_INITOBJ:			s0="INITOBJ"; break;
			case CCXL_VOP_DROPOBJ:			s0="DROPOBJ"; break;
			case CCXL_VOP_INITARR:			s0="INITARR"; break;
			case CCXL_VOP_INITOBJARR:		s0="INITOBJARR"; break;
			case CCXL_VOP_LOADINITARR:		s0="LDINITARR"; break;
			case CCXL_VOP_LOADINITOBJARR:	s0="LDINITOBJARR"; break;
			case CCXL_VOP_VA_START:			s0="VA_START"; break;
			case CCXL_VOP_VA_END:			s0="VA_END"; break;
			case CCXL_VOP_VA_ARG:			s0="VA_ARG"; break;
			case CCXL_VOP_CSELCMP:			s0="CSELCMP"; break;
			case CCXL_VOP_CSELCMP_Z:		s0="CSELCMP_Z"; break;
			case CCXL_VOP_OBJCALL:			s0="OBJCALL"; break;
			case CCXL_VOP_PREDCMP:			s0="PREDCMP"; break;
			case CCXL_VOP_CALL_INTRIN:		s0="CALL_INTRIN"; break;
			case CCXL_VOP_ASMINLINE:		s0="ASM_INLINE"; break;
			case CCXL_VOP_TEMP_PHI:			s0="TEMP_PHI"; break;
//			case CCXL_VOP_TRINARY:			s0="TRINARY"; break;
			case CCXL_VOP_CSRV_RET:			s0="CSRV_RET"; break;
			case CCXL_VOP_LDIXIMMA:			s0="LDIXIMMA"; break;
			case CCXL_VOP_LDIXA:			s0="LDIXA"; break;
			case CCXL_VOP_BITMOV:			s0="BITMOV"; break;
		}

		if(s0)
		{
			fprintf(sctx->cgen_log,
				"\t%s",
				s0);
		}else
		{
			fprintf(sctx->cgen_log,
				"\t%04X",
				op->opn);
		}

		if(op->prd)
		{
			fprintf(sctx->cgen_log,
				" prd=%d",
				op->prd);
		}

		if(op->type.val<256)
		{
			fprintf(sctx->cgen_log,
				" ty=%02X", op->type.val);
		}else if(op->type.val<65536)
		{
			fprintf(sctx->cgen_log,
				" ty=%04X", op->type.val);
		}else
		{
			fprintf(sctx->cgen_log,
				" ty=%08X", op->type.val);
		}
		
		if(op->stype.val)
		{
			if(op->stype.val<256)
			{
				fprintf(sctx->cgen_log,
					" sty=%02X", op->stype.val);
			}else if(op->stype.val<65536)
			{
				fprintf(sctx->cgen_log,
					" sty=%04X", op->stype.val);
			}else
			{
				fprintf(sctx->cgen_log,
					" sty=%08X", op->stype.val);
			}
		}

		if(op->opr)
		{
			fprintf(sctx->cgen_log,
				" opr=%02X", op->opr);
		}

		if(op->dst.val)
		{
//			fprintf(sctx->cgen_log, " dst=%016llX", op->dst.val);
			fprintf(sctx->cgen_log, " dst=%s", 
				BGBCC_JX2C_DebugRegToStr(ctx, sctx, op->type, op->dst));
		}
		if(op->srca.val)
		{
//			fprintf(sctx->cgen_log, " srca=%016llX", op->srca.val);
			fprintf(sctx->cgen_log, " srca=%s", 
				BGBCC_JX2C_DebugRegToStr(ctx, sctx, op->type, op->srca));
		}
		if(op->srcb.val)
		{
//			fprintf(sctx->cgen_log, " srcb=%016llX", op->srcb.val);
			fprintf(sctx->cgen_log, " srcb=%s", 
				BGBCC_JX2C_DebugRegToStr(ctx, sctx, op->type, op->srcb));
		}

		if(op->srcc.val)
		{
			fprintf(sctx->cgen_log, " srcc=%s", 
				BGBCC_JX2C_DebugRegToStr(ctx, sctx, op->type, op->srcc));
		}

		if(op->srcd.val)
		{
			fprintf(sctx->cgen_log, " srcd=%s", 
				BGBCC_JX2C_DebugRegToStr(ctx, sctx, op->type, op->srcd));
		}

		if(op->imm.ul)
		{
			if((op->opn==CCXL_VOP_LOADSLOT) ||
				(op->opn==CCXL_VOP_STORESLOT) ||
				(op->opn==CCXL_VOP_LOADSLOTADDR))
			{
				fprintf(sctx->cgen_log, " obj=%03X.%02X",
					op->imm.obj.gid, op->imm.obj.fid);
			}else if(op->imm.ul==op->imm.ui)
				{ fprintf(sctx->cgen_log, " imm=%08X", op->imm.ui); }
			else 
				{ fprintf(sctx->cgen_log, " imm=%016llX", op->imm.ul); }
		}

		fprintf(sctx->cgen_log, "\n");
		fflush(sctx->cgen_log);
		
//		if(op->opn==CCXL_VOP_CALL)
		if((op->opn==CCXL_VOP_CALL) ||
			(op->opn==CCXL_VOP_OBJCALL) ||
			(op->opn==CCXL_VOP_CALL_INTRIN))
		{
			na=op->imm.call.na;
			ca=op->imm.call.args;
			for(i=0; i<na; i++)
			{
				fprintf(sctx->cgen_log, "\t\tA%d=%s\n",  i,
					BGBCC_JX2C_DebugRegToStr(ctx, sctx, op->type, ca[i]));
			}
			fflush(sctx->cgen_log);
		}
	}
#endif

	return(0);
}

ccxl_status BGBCC_JX2C_CompileVirtOp(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, BGBCC_CCXL_VirtOp *op)
{
	static int rec=0;
	ccxl_register reg1;
	char *s0;
	int i0, i1, i2, i3;
	int i, j, k;

//	BGBCC_JX2C_CompilePrintVirtOp(ctx, sctx, obj, op);

	sctx->test_lclalign=0;

//	sctx->test_lclalign=1;
//	sctx->test_lclalign=3;

	sctx->sreg_live=sctx->sreg_held;
	sctx->sfreg_live=sctx->sfreg_held;
	BGBCC_JX2_EmitCheckFlushIndexImm(sctx);

	if(!(sctx->is_leaftiny&1) && !(sctx->is_leaftiny&4))
	{
		if(sctx->regalc_live || sctx->fregalc_live)
		{
			BGBCC_DBGBREAK
		}
	}

	if((sctx->is_fixed32&1) && !BGBCC_JX2_CheckPadAlign32(sctx))
		{ BGBCC_DBGBREAK }

	if(sctx->csrv_skip)
	{
		if(op->opn!=CCXL_VOP_CSRV)
		{
			k=-1;
		}
	}

	reg1=sctx->regalc_map[0];
	if(((s64)reg1.val)<0)
	{
		BGBCC_DBGBREAK
	}

	switch(op->opn)
	{
	case CCXL_VOP_NONE:
		break;
	case CCXL_VOP_DBGFN:
		ctx->lfn=op->imm.str;
		break;
	case CCXL_VOP_DBGLN:
		ctx->lln=op->imm.si;
		BGBCC_JX2_EmitDebugLine(sctx, ctx->lfn, ctx->lln);
		break;
	case CCXL_VOP_LABEL:
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
//		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
//		BGBCC_JX2_EmitForceFlushIndexImm(sctx);
//		BGBCC_JX2_EmitPadForOpWord(sctx, 0xF000);
		BGBCC_JX2_EmitPadForLabel(sctx);
		if(sctx->is_align_wexj)
			BGBCC_JX2_EmitPad32AlignLastOp(sctx);
		BGBCC_JX2_EmitLabel(sctx,
			op->imm.si);
		break;
	case CCXL_VOP_JMP:
//		BGBCC_JX2_EmitOpLabel(sctx, BGBCC_SH_NMID_BRA,
//			op->imm.si);
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
//		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
//		BGBCC_JX2C_ResetFpscrUnknown(ctx, sctx);

//		BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BRA,
//			op->imm.si);
//		BGBCC_JX2_EmitOpNone(sctx,
//			BGBCC_SH_NMID_NOP);

//		BGBCC_JX2_EmitPadForOpWord(sctx, 0xF000);

		k=op->imm.si;
		if(k==CCXL_LBL_RETURN)
			k=sctx->lbl_ret;
		if(k==CCXL_LBL_RETURN_ZERO)
			k=sctx->lbl_ret_zero;

		BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BRAN,
			k);

//		BGBCC_JX2_EmitFlushIndexImm16(sctx);
//		BGBCC_JX2_EmitFlushIndexImm32(sctx);
		BGBCC_JX2_EmitFlushIndexImmBasic(sctx);
		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
		break;
	case CCXL_VOP_MOV:
		BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, op->type,
			op->dst, op->srca);
		break;
	case CCXL_VOP_JCMP_ZERO:
//		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
		BGBCC_JX2C_EmitSyncDirtyRegisters(ctx, sctx);
//		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
//		BGBCC_JX2C_ResetFpscrUnknown(ctx, sctx);
//		BGBCC_JX2_EmitForceFlushIndexImm(sctx);

//		BGBCC_JX2_EmitPadForOpWord(sctx, 0xF000);

		k=op->imm.si;
		j=k;
		if(k==CCXL_LBL_RETURN)
			k=sctx->lbl_ret;
		if(k==CCXL_LBL_RETURN_ZERO)
			k=sctx->lbl_ret_zero;

		BGBCC_JX2C_EmitJCmpVRegZero(ctx, sctx, op->type,
			op->srca, op->opr, k);
//		if(j!=k)
//			BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
		break;
	case CCXL_VOP_JCMP:
//		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
		BGBCC_JX2C_EmitSyncDirtyRegisters(ctx, sctx);
//		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
//		BGBCC_JX2C_ResetFpscrUnknown(ctx, sctx);
//		BGBCC_JX2_EmitForceFlushIndexImm(sctx);

//		BGBCC_JX2_EmitPadForOpWord(sctx, 0xF000);

		k=op->imm.si;
		j=k;
		if(k==CCXL_LBL_RETURN)
			k=sctx->lbl_ret;
		if(k==CCXL_LBL_RETURN_ZERO)
			k=sctx->lbl_ret_zero;

		BGBCC_JX2C_EmitJCmpVRegVReg(ctx, sctx, op->type,
			op->srca, op->srcb, op->opr, k);
//		if(j!=k)
//			BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
		break;
	case CCXL_VOP_CALL:
	case CCXL_VOP_OBJCALL:
//		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
		BGBCC_JX2C_EmitSyncDirtyRegisters(ctx, sctx);
//		BGBCC_JX2C_ResetFpscrDefaults(ctx, sctx);
//		BGBCC_JX2_EmitForceFlushIndexImm(sctx);

//		sctx->sreg_live|=0x00F4;
//		sctx->sreg_live|=0x00F0;
//		sctx->sreg_live|=0x00F000F0;
//		sctx->sfreg_live|=0x0FF0;
		BGBCC_JX2C_EmitCallVReg(ctx, sctx,
			op->type, op->dst, op->srca, op->srcb,
			op->imm.call.na, op->imm.call.args);

//		sctx->sreg_live&=~0x00F0;
//		sctx->sreg_live&=~0x00F000F0;
//		sctx->sfreg_live&=~0x0FF0;

//		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
		BGBCC_JX2C_EmitSyncAliasRegisters(ctx, sctx);

//		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
//		BGBCC_JX2C_EmitLabelFlushFpRegisters(ctx, sctx);
		break;
	case CCXL_VOP_CSRV:
//		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
//		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
		BGBCC_JX2C_EmitCsrvVReg(ctx, sctx,
			op->type, op->dst, op->srca);
		break;

	case CCXL_VOP_CSRV_RET:
//		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
		BGBCC_JX2C_EmitSyncDirtyRegisters(ctx, sctx);
		if(	sctx->csrv_skip)
		{
			BGBCC_JX2C_EmitReturnVReg(ctx, sctx,
				op->type, op->dst);
			sctx->csrv_skip=0;
		}else
		{
			BGBCC_JX2C_EmitReturnVoid(ctx, sctx);
		}
		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
		BGBCC_JX2_EmitFlushIndexImmBasic(sctx);
		break;

	case CCXL_VOP_CALL_INTRIN:
		BGBCC_JX2C_EmitCallIntrinVReg(ctx, sctx,
			op->type, op->dst, op->srca, op->srcb,
			op->imm.call.na, op->imm.call.args);
		break;

	case CCXL_VOP_RETDFL:
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
//		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
		BGBCC_JX2C_EmitReturnVoid(ctx, sctx);
		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
		BGBCC_JX2_EmitFlushIndexImmBasic(sctx);
		break;
	case CCXL_VOP_RETV:
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
//		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
		BGBCC_JX2C_EmitReturnVoid(ctx, sctx);
		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
		BGBCC_JX2_EmitFlushIndexImmBasic(sctx);
		break;
	case CCXL_VOP_RET:
//		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
		BGBCC_JX2C_EmitSyncDirtyRegisters(ctx, sctx);
//		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
		BGBCC_JX2C_EmitReturnVReg(ctx, sctx,
			op->type, op->srca);
		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
		BGBCC_JX2_EmitFlushIndexImmBasic(sctx);
		break;
	case CCXL_VOP_CONV:
		BGBCC_JX2C_EmitConvVRegVReg(ctx, sctx,
			op->type, op->stype,
			op->dst, op->srca);
		break;
	case CCXL_VOP_UNARY:
		BGBCC_JX2C_EmitUnaryVRegVReg(ctx, sctx, op->type,
			op->dst, op->opr, op->srca);
		break;
	case CCXL_VOP_BINARY:
		BGBCC_JX2C_EmitBinaryVRegVRegVReg(ctx, sctx, op->type,
			op->dst, op->opr, op->srca, op->srcb);
		break;
	case CCXL_VOP_COMPARE:
		BGBCC_JX2C_EmitCompareVRegVRegVReg(ctx, sctx, op->type,
			op->dst, op->opr, op->srca, op->srcb);
		break;
	case CCXL_VOP_TRINARY:
		BGBCC_JX2C_EmitTrinaryVRegVRegVRegVReg(ctx, sctx, op->type,
			op->dst, op->opr, op->srca, op->srcb, op->srcc);
		break;
	case CCXL_VOP_LDIXIMM:
		BGBCC_JX2C_EmitSyncAliasRegisters(ctx, sctx);
		BGBCC_JX2C_EmitLdixVRegVRegImm(ctx, sctx, op->type, op->stype,
			op->dst, op->srca, op->imm.si);
		BGBCC_JX2C_EmitSyncAliasRegisters(ctx, sctx);
		break;
	case CCXL_VOP_STIXIMM:
		BGBCC_JX2C_EmitSyncAliasRegisters(ctx, sctx);
		BGBCC_JX2C_EmitStixVRegVRegImm(ctx, sctx, op->type, op->stype,
			op->dst, op->srca, op->imm.si);
		BGBCC_JX2C_EmitSyncAliasRegisters(ctx, sctx);
		break;
	case CCXL_VOP_LDIX:
		BGBCC_JX2C_EmitSyncAliasRegisters(ctx, sctx);
		BGBCC_JX2C_EmitLdixVRegVRegVReg(ctx, sctx, op->type, op->stype,
			op->dst, op->srca, op->srcb);
		break;
	case CCXL_VOP_STIX:
		BGBCC_JX2C_EmitSyncAliasRegisters(ctx, sctx);
		BGBCC_JX2C_EmitStixVRegVRegVReg(ctx, sctx, op->type, op->stype,
			op->dst, op->srca, op->srcb);
		BGBCC_JX2C_EmitSyncAliasRegisters(ctx, sctx);
		break;
	case CCXL_VOP_LEAIMM:
		BGBCC_JX2C_EmitLeaVRegVRegImm(ctx, sctx, op->type, op->stype,
			op->dst, op->srca, op->imm.si);
		break;
	case CCXL_VOP_LEA:
		BGBCC_JX2C_EmitLeaVRegVRegVReg(ctx, sctx, op->type, op->stype,
			op->dst, op->srca, op->srcb);
		break;
	case CCXL_VOP_LDAVAR:
		BGBCC_JX2C_EmitLdaVRegVReg(ctx, sctx, op->type,
			op->dst, op->srca);
		break;

	case CCXL_VOP_LDIXIMMA:
		BGBCC_JX2C_EmitLdixAddrVRegVRegImm(ctx, sctx, op->type, op->stype,
			op->dst, op->srca, op->imm.si);
		break;
	case CCXL_VOP_LDIXA:
		BGBCC_JX2C_EmitLdixAddrVRegVRegVReg(ctx, sctx, op->type, op->stype,
			op->dst, op->srca, op->srcb);
		break;

	case CCXL_VOP_SIZEOFVAR:
		BGBCC_JX2C_SizeofVar(ctx, sctx, op->type, op->dst);
//		BGBCC_CCXL_StubError(ctx);
		break;

	case CCXL_VOP_DIFFPTR:
		BGBCC_JX2C_EmitDiffPtrVRegVRegVReg(ctx, sctx, op->type,
			op->dst, op->srca, op->srcb);
		break;

	case CCXL_VOP_LOADSLOT:
//		BGBCC_JX2C_EmitSyncAliasRegisters(ctx, sctx);
		BGBCC_JX2C_EmitLoadSlotVRegVRegImm(ctx, sctx, op->type,
			op->dst, op->srca, op->imm.obj.gid, op->imm.obj.fid);
		break;
	case CCXL_VOP_STORESLOT:
//		BGBCC_JX2C_EmitSyncAliasRegisters(ctx, sctx);
		BGBCC_JX2C_EmitStoreSlotVRegVRegImm(ctx, sctx, op->type,
			op->dst, op->srca, op->imm.obj.gid, op->imm.obj.fid);
//		BGBCC_JX2C_EmitSyncAliasRegisters(ctx, sctx);
		break;
	case CCXL_VOP_LOADSLOTADDR:
		BGBCC_JX2C_EmitLoadSlotAddrVRegVRegImm(ctx, sctx, op->type,
			op->dst, op->srca, op->imm.obj.gid, op->imm.obj.fid);
		break;
	
	case CCXL_VOP_INITOBJ:
		if(BGBCC_CCXL_IsRegLocalP(ctx, op->dst) ||
			BGBCC_CCXL_IsRegArgP(ctx, op->dst))
				break;
		if(BGBCC_CCXL_IsRegTempP(ctx, op->dst))
		{
			BGBCC_JX2C_EmitInitObj(ctx, sctx, op->type, op->dst);
			break;
		}
		BGBCC_CCXL_StubError(ctx);
		break;
	case CCXL_VOP_DROPOBJ:
		if(BGBCC_CCXL_IsRegLocalP(ctx, op->dst) ||
			BGBCC_CCXL_IsRegArgP(ctx, op->dst))
				break;
		if(BGBCC_CCXL_IsRegTempP(ctx, op->dst))
			break;
		BGBCC_CCXL_StubError(ctx);
		break;
	case CCXL_VOP_INITARR:
		if(BGBCC_CCXL_IsRegLocalP(ctx, op->dst) ||
			BGBCC_CCXL_IsRegArgP(ctx, op->dst))
				break;
		BGBCC_CCXL_StubError(ctx);
		break;
	case CCXL_VOP_INITOBJARR:
		if(BGBCC_CCXL_IsRegLocalP(ctx, op->dst) ||
			BGBCC_CCXL_IsRegArgP(ctx, op->dst))
				break;
		BGBCC_CCXL_StubError(ctx);
		break;

	case CCXL_VOP_VA_START:
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
		BGBCC_JX2C_EmitVaStart(ctx, sctx, op->dst, op->srca);
		break;
	case CCXL_VOP_VA_END:
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
		BGBCC_JX2C_EmitVaEnd(ctx, sctx, op->srca);
		break;
	case CCXL_VOP_VA_ARG:
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
		BGBCC_JX2C_EmitVaArg(ctx, sctx, op->type, op->dst, op->srca);
		break;

	case CCXL_VOP_JMPTAB:
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
//		BGBCC_JX2_EmitPadForOpWord(sctx, 0xF000);
		BGBCC_JX2C_EmitJmpTab(ctx, sctx,
			op->type, op->srca,
			op->imm.jmptab.vmin, op->imm.jmptab.nlbl, op->imm.jmptab.lbls);
		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
		break;

	case CCXL_VOP_PREDCMP:
//		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
		BGBCC_JX2C_EmitSyncDirtyRegisters(ctx, sctx);
		BGBCC_JX2C_EmitPredCmpVRegVReg(ctx, sctx, op->type,
			op->srca, op->srcb, op->opr);
		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
		break;

	case CCXL_VOP_PREDSYNC:
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
		break;

	case CCXL_VOP_ASMINLINE:
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);

		sctx->is_leaf&=(~4);
		sctx->is_fixed32&=~16;

//		ctx->lfn=op->imm.str;
		BGBCC_JX2C_AssembleBuffer(ctx, sctx, op->imm.str);

		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);

		sctx->is_fixed32&=~16;
		break;

	case CCXL_VOP_TEMP_PHI:
		BGBCC_JX2C_EmitTempPhiRegister(ctx, sctx, op->srca);
		break;

	case CCXL_VOP_BITMOV:
		i0=(s16)(op->imm.ul>> 0);
		i1=(s16)(op->imm.ul>>16);
		i2=(s16)(op->imm.ul>>32);
		BGBCC_JX2C_EmitBitMovVRegVRegVReg(ctx, sctx,
			op->type,	op->dst,
			op->srca,	op->srcb,
			i0, i1, i2);
		break;

	default:
		BGBCC_CCXL_StubError(ctx);
		break;
	}

	reg1=sctx->regalc_map[0];
	if(((s64)reg1.val)<0)
	{
		BGBCC_DBGBREAK
	}


	BGBCC_JX2_EmitCheckFlushIndexImm(sctx);
	
	if(!(sctx->is_leaftiny&1) && !(sctx->is_leaftiny&4))
	{
		if(sctx->regalc_live || sctx->fregalc_live)
		{
			if(!rec)
			{
				rec=1;
				BGBCC_JX2C_CompileVirtOp(ctx, sctx, obj, op);
			}
			BGBCC_DBGBREAK
		}

		if(sctx->sreg_live || sctx->sfreg_live)
		{
			if(!rec)
			{
				rec=1;
				BGBCC_JX2C_CompileVirtOp(ctx, sctx, obj, op);
			}
			if(!(sctx->is_simpass&0x40))
				{ BGBCC_DBGBREAK }
		}

		if(sctx->regalc_noval)
		{
			BGBCC_DBGBREAK
		}
	}

#if 1
//	BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
//	BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);

//	BGBCC_JX2C_EmitSyncFpRegisters(ctx, sctx);
//	BGBCC_JX2C_EmitLabelFlushFpRegisters(ctx, sctx);
#endif
	
	return(1);
}

ccxl_status BGBCC_JX2C_PrintVirtTr(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, BGBCC_CCXL_VirtTr *tr, int idx)
{
	int i, j, k;

	if(sctx->cgen_log)
	{
		fprintf(sctx->cgen_log,
			"   Tr: idx=%d base=%d num=%d\n",
			idx, tr->b_ops, tr->n_ops);
		fflush(sctx->cgen_log);
	}

	for(i=0; i<tr->n_ops; i++)
	{
		BGBCC_JX2C_PrintVirtOp(ctx, sctx, obj, obj->vop[tr->b_ops+i]);
	}

	return(0);
}

ccxl_status BGBCC_JX2C_CompileVirtTr(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, BGBCC_CCXL_VirtTr *tr, int idx)
{
	BGBCC_CCXL_VirtOp *vop, *vop1, *vop2, *vop3;
	BGBCC_CCXL_VirtOp *vop1b, *vop2b, *vop3b;
	int ps0, ps1, tr1, tr2, usewex, lzyflush;
	int i, j, k;

#if 0
	if(sctx->cgen_log)
	{
		fprintf(sctx->cgen_log,
			"   Tr: idx=%d base=%d num=%d\n",
			idx, tr->b_ops, tr->n_ops);
		fflush(sctx->cgen_log);
	}
#endif

	ctx->cur_vtr=tr;

	sctx->is_leaf&=~4;
	sctx->is_fixed32&=(~16);

	sctx->op_wex_align&=~1;

//	if(sctx->is_simpass==1)
	if((sctx->is_simpass==1) || (sctx->is_simpass&64))
	{
		sctx->is_leaf|=4;
	}

	sctx->is_tr_leaf=0;
//	if(tr->trfl&1)
	if(tr->trfl&CCXL_TRFL_LEAF)
		sctx->is_tr_leaf=1;

//	if(tr->trfl&CCXL_TRFL_WEXALIGN)
	if((tr->trfl&CCXL_TRFL_WEXALIGN) || (sctx->is_simpass&64))
		sctx->op_wex_align|=1;
	

	usewex=0;

//	if(tr->n_ops>10)
//	if(tr->n_ops>5)
//	if(tr->n_fops>5)
	if(tr->n_fops>4)
//	if(tr->n_fops>3)
//	if(tr->n_ops>4)
//	if(tr->n_ops>3)
	{
		if(sctx->use_egpr)
//		if(sctx->use_egpr && sctx->is_tr_leaf)
//		if(sctx->is_tr_leaf)
		{
//			sctx->is_fixed32|=16;
			usewex=1;
		}
	}

//	if(tr->n_ops>2)
	if(tr->n_fops>2)
//	if(tr->n_fops>1)
	{
#if 1
//		if((ctx->optmode==BGBCC_OPT_SPEED) && sctx->is_tr_leaf)
		if(ctx->optmode==BGBCC_OPT_SPEED2)
		{
//			sctx->is_fixed32|=16;
			usewex=1;
		}
#endif
	}
	
	if(sctx->is_fixed32&2)
	{
		/* XG2: WEXify everything... */
		usewex=1;
	}

	if(usewex || sctx->op_wex_align)
	{
		BGBCC_JX2_EmitPadForLabel(sctx);
	}

	sctx->is_fixed32&=(~16);

	sctx->state_alias=0;
	if(ctx->cur_func->regflags&BGBCC_REGFL_HASLCLALIAS)
		sctx->state_alias|=1;
	if(ctx->cur_func->regflags&BGBCC_REGFL_HASGBLALIAS)
		sctx->state_alias|=2;
		

	if(usewex)
	{
		if(sctx->is_align_wexj)
		{
			sctx->op_wex_align|=1;		//BJX2D
		}
		BGBCC_JX2_BeginWex(sctx);
	}

//	ps0=BGBCC_JX2_EmitGetOffs(sctx);
//	if(ps0&1)
//		{ BGBCC_DBGBREAK }

#if 0
	ps0=BGBCC_JX2_EmitGetOffs(sctx);
	if(sctx->is_fixed32&16)
		sctx->wex_ofs_begin=ps0;
#endif

	sctx->op_is_wex2=0;
	lzyflush=0;

	for(i=0; i<tr->n_ops; i++)
	{
		sctx->tr_opnum=tr->b_ops+i;
		vop=obj->vop[tr->b_ops+i];
		vop1=NULL;
		vop2=NULL;
		vop3=NULL;
		if((i+1)<tr->n_ops)
			vop1=obj->vop[tr->b_ops+i+1];
		if((i+2)<tr->n_ops)
			vop2=obj->vop[tr->b_ops+i+2];
		if((i+3)<tr->n_ops)
			vop3=obj->vop[tr->b_ops+i+3];

		vop1b=obj->vop[tr->b_ops+i+1];
		vop2b=obj->vop[tr->b_ops+i+2];
		vop3b=obj->vop[tr->b_ops+i+3];

		ctx->cur_vop=vop;

		j=0;
//		if(vop->prd==2)j=(sctx->pred_tfmd)?5:4;
//		if(vop->prd==3)j=(sctx->pred_tfmd)?4:5;
		if(vop->prd==3)j=(sctx->pred_tfmd)?5:4;
		if(vop->prd==2)j=(sctx->pred_tfmd)?4:5;

		if(sctx->is_align_wexj && j)
		{
			BGBCC_JX2_EmitPad32AlignLastOp(sctx);
		}
		
		if(vop->prd && !sctx->is_simpass)
		{
			k=-1;
		}
		
		sctx->op_is_wex2=j;

#if 0
		/*
		 * Try to shuffle 3AC ops to reduce dependencies.
		 */
//		if(vop && vop1 && vop2 && vop3 && (sctx->is_simpass&64))
		if(vop && vop1 && vop2 && vop3 && (sctx->is_simpass))
		{
//			if(	(vop->opn==CCXL_VOP_LOADSLOT) ||
//				(vop->opn==CCXL_VOP_LDIX) ||
//				(vop->opn==CCXL_VOP_LDIXIMM)	)
//			if(BGBCC_CCXL_CheckVirtOpIsLoad(ctx, vop))
			if(1)
			{
//				if(	(vop1->opn==CCXL_VOP_BINARY) ||
//					(vop1->opn==CCXL_VOP_UNARY) )
				if(	!BGBCC_CCXL_CheckVirtOpNoSwaps(ctx, vop1) &&
					!BGBCC_CCXL_CheckVirtOpNoSwaps(ctx, vop2) &&
//					 BGBCC_CCXL_CheckVirtOpOrderDep(ctx, vop, vop1) &&
//					!BGBCC_CCXL_CheckVirtOpOrderDep(ctx, vop, vop2) &&
//					!BGBCC_CCXL_CheckVirtOpOrderDep(ctx, vop1, vop3) &&

					 BGBCC_CCXL_CheckVirtOpOrderOnly(ctx, vop, vop1) &&
					!BGBCC_CCXL_CheckVirtOpOrderOnly(ctx, vop, vop2) &&
					!BGBCC_CCXL_CheckVirtOpOrderOnly(ctx, vop1, vop3) &&

					BGBCC_CCXL_CheckVirtOpCanSwap(ctx, vop1, vop2))
				{
					obj->vop[tr->b_ops+i+1]=vop2;
					obj->vop[tr->b_ops+i+2]=vop1;

					vop1=obj->vop[tr->b_ops+i+1];
					vop2=obj->vop[tr->b_ops+i+2];
				}
			}
		}else
//			if(vop && vop1 && vop2 && (sctx->is_simpass&64))
			if(vop && vop1 && vop2 && (sctx->is_simpass))
		{
//			if(	(vop->opn==CCXL_VOP_LOADSLOT)	||
//				(vop->opn==CCXL_VOP_LDIX)		||
//				(vop->opn==CCXL_VOP_LDIXIMM)	)
			if(1)
			{
//				if(	(vop1->opn==CCXL_VOP_BINARY) ||
//					(vop1->opn==CCXL_VOP_UNARY) )
				if(	!BGBCC_CCXL_CheckVirtOpNoSwaps(ctx, vop1) &&
					!BGBCC_CCXL_CheckVirtOpNoSwaps(ctx, vop2) &&
					BGBCC_CCXL_CheckVirtOpOrderOnly(ctx, vop, vop1) &&
					!BGBCC_CCXL_CheckVirtOpOrderOnly(ctx, vop, vop2) &&
					BGBCC_CCXL_CheckVirtOpCanSwap(ctx, vop1, vop2))
				{
					obj->vop[tr->b_ops+i+1]=vop2;
					obj->vop[tr->b_ops+i+2]=vop1;

					vop1=obj->vop[tr->b_ops+i+1];
					vop2=obj->vop[tr->b_ops+i+2];
				}
			}
#if 0
			else
				if(	(vop1->opn==CCXL_VOP_LOADSLOT) ||
					(vop1->opn==CCXL_VOP_LDIX) ||
					(vop1->opn==CCXL_VOP_LDIXIMM)	)
			{
				if(
					!BGBCC_CCXL_CheckVirtOpOrderOnly(ctx, vop, vop1) &&
					!BGBCC_CCXL_CheckVirtOpOrderOnly(ctx, vop, vop2) &&
					BGBCC_CCXL_CheckVirtOpOrderOnly(ctx, vop1, vop2) &&
					BGBCC_CCXL_CheckVirtOpCanSwap(ctx, vop, vop1))
				{
					obj->vop[tr->b_ops+i+0]=vop1;
					obj->vop[tr->b_ops+i+1]=vop;
					i--;
					continue;
				}
			}
#endif
		}
#endif

#if 0
		if(vop && vop1)
		{
			if(	(vop1->opn==CCXL_VOP_BINARY) ||
				(vop1->opn==CCXL_VOP_LDIX)	)
			{
				if(	!BGBCC_CCXL_RegisterSequenceEqualP(ctx,
						vop->dst, vop1->srca) )
				{
					tr1=BGBCC_JX2C_EmitTryGetRegister(ctx, sctx, vop1->srca, 2);
					if(tr1!=BGBCC_SH_REG_ZZR)
						BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, vop1->srca);
				}

				if(	!BGBCC_CCXL_RegisterSequenceEqualP(ctx,
						vop->dst, vop1->srcb) 	)
				{
					tr1=BGBCC_JX2C_EmitTryGetRegister(ctx, sctx, vop1->srcb, 2);
					if(tr1!=BGBCC_SH_REG_ZZR)
						BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, vop1->srcb);
				}
			}
		}
#endif

#if 0
		if(vop && vop2)
		{
			if(	(vop2->opn==CCXL_VOP_BINARY) ||
				(vop2->opn==CCXL_VOP_LDIX)	)
			{
				if(	!BGBCC_CCXL_RegisterSequenceEqualP(ctx,
						vop->dst, vop2->srca) &&
					!BGBCC_CCXL_RegisterSequenceEqualP(ctx,
						vop1->dst, vop2->srca)	)
				{
					tr1=BGBCC_JX2C_EmitTryGetRegister(ctx, sctx, vop2->srca, 2);
					if(tr1!=BGBCC_SH_REG_ZZR)
						BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, vop2->srca);
				}

				if(	!BGBCC_CCXL_RegisterSequenceEqualP(ctx,
						vop->dst, vop2->srcb) &&
					!BGBCC_CCXL_RegisterSequenceEqualP(ctx,
						vop1->dst, vop2->srcb)	)
				{
					tr1=BGBCC_JX2C_EmitTryGetRegister(ctx, sctx, vop2->srcb, 2);
					if(tr1!=BGBCC_SH_REG_ZZR)
						BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, vop2->srcb);
				}

			}
		}
#endif

#if 1
		/* Hacky Special Case Opt: arr[expr+imm] */
		if(	vop && vop1 && sctx->has_bjx1mov &&
			(vop->opn==CCXL_VOP_BINARY) &&
			(vop->opr==CCXL_BINOP_ADD) &&
			(vop1->opn==CCXL_VOP_LDIX) &&
			BGBCC_CCXL_RegisterSequenceEqualP(ctx,
				vop->dst, vop1->srcb) &&
			BGBCC_CCXL_TypeSmallIntP(ctx, vop1->type) &&
			BGBCC_CCXL_IsRegTempP(ctx, vop->dst) &&
			BGBCC_CCXL_IsRegImmIntP(ctx, vop->srcb)	)
		{
			j=BGBCC_CCXL_GetRegImmIntValue(ctx, vop->srcb);
			if((j>=0) && (j<16))
			{
				BGBCC_JX2C_EmitLdixVRegVRegVRegImm(
					ctx, sctx, vop1->type, vop1->stype,
					vop1->dst, vop1->srca, vop->srca, j);
				i++;
				continue;
			}
		}

#if 1
		/* Hacky Special Case Opt: arr[expr+imm]=val */
		if(	vop && vop1 && sctx->has_bjx1mov &&
			(vop->opn==CCXL_VOP_BINARY) &&
			(vop->opr==CCXL_BINOP_ADD) &&
			(vop1->opn==CCXL_VOP_STIX) &&
			BGBCC_CCXL_RegisterSequenceEqualP(ctx,
				vop->dst, vop1->srca) &&
			BGBCC_CCXL_TypeSmallIntP(ctx, vop1->type) &&
			BGBCC_CCXL_IsRegTempP(ctx, vop->dst) &&
			BGBCC_CCXL_IsRegImmIntP(ctx, vop->srcb)	)
		{
			j=BGBCC_CCXL_GetRegImmIntValue(ctx, vop->srcb);
			if((j>=0) && (j<16))
			{
				BGBCC_JX2C_EmitStixVRegVRegVRegImm(
					ctx, sctx, vop1->type, vop1->stype,
					vop1->dst, vop->srca, vop1->srcb, j);
				i++;
				continue;
			}
		}
#endif
#endif

#if 1
		if(	vop && vop1 &&
			((vop->opn==CCXL_VOP_LOADSLOT) ||
				(vop->opn==CCXL_VOP_LOADSLOTADDR)) &&
			(vop1->opn==CCXL_VOP_LDIXIMM) &&
			BGBCC_CCXL_RegisterSequenceEqualP(ctx,
				vop->dst, vop1->srca) &&
			!BGBCC_JX2C_CheckVRegMoreUsesInTraceStepP(ctx, sctx, vop->dst, 2) &&
			BGBCC_CCXL_TypeArrayP(ctx, vop->type) &&
			!BGBCC_JX2C_CheckVRegCrossTraceP(ctx, sctx, vop->dst) &&
			(BGBCC_CCXL_TypeSmallLongP(ctx, vop1->type) ||
				BGBCC_CCXL_TypePointerP(ctx, vop1->type)) &&
			BGBCC_CCXL_IsRegTempP(ctx, vop->dst)	)
		{
			j=BGBCC_JX2C_EmitLoadSlotAndIndexVRegVRegImm2(ctx, sctx,
				vop1->type, vop1->dst,
				vop->srca, vop->imm.obj.gid, vop->imm.obj.fid,
				vop1->imm.si);
			if(j>0)
			{
				i++;
				continue;
			}
		}
#endif

#if 1
		if(	vop && vop1 &&
			(vop->opn==CCXL_VOP_LOADSLOT) &&
			(vop1->opn==CCXL_VOP_LDIXIMM) &&
			BGBCC_CCXL_RegisterSequenceEqualP(ctx,
				vop->dst, vop1->srca) &&
//			!BGBCC_JX2C_CheckVRegMoreUsesInTraceP(ctx, sctx, vop->dst) &&
			!BGBCC_JX2C_CheckVRegMoreUsesInTraceStepP(ctx, sctx, vop->dst, 2) &&
			BGBCC_CCXL_TypeArrayP(ctx, vop->type) &&
			(BGBCC_CCXL_TypeSmallLongP(ctx, vop1->type) ||
				BGBCC_CCXL_TypePointerP(ctx, vop1->type)) &&
			BGBCC_CCXL_IsRegTempP(ctx, vop->dst)	)
		{
			j=BGBCC_JX2C_EmitLoadSlotAndIndexVRegVRegImm2(ctx, sctx,
				vop1->type, vop1->dst,
				vop->srca, vop->imm.obj.gid, vop->imm.obj.fid,
				vop1->imm.si);
			if(j>0)
			{
				i++;
				continue;
			}
		}
#endif
		
		if(	vop && vop1b &&
			(vop->opn==CCXL_VOP_JMP) &&
			(vop1b->opn==CCXL_VOP_LABEL) &&
			(vop->imm.ui == vop1b->imm.ui)
			)
		{
			/* Skip doing a branch to the following instruction. */
			BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
			BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
			continue;
		}

		if(	(vop->opn==CCXL_VOP_JCMP) ||
			(vop->opn==CCXL_VOP_JCMP_ZERO) )
		{
			lzyflush=1;
		}
		
		BGBCC_JX2C_CompileVirtOp(ctx, sctx, obj, vop);
	}

	if(lzyflush)
	{
		BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
	}

//	BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);
	BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);

//	BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);

	ps1=BGBCC_JX2_EmitGetOffs(sctx);
	if(ps1&1)
	{
//		BGBCC_JX2_EmitPadForOpWord(sctx, 0xF000);
//		BGBCC_JX2_EmitPadForLabel(sctx);
	}

	sctx->op_is_wex2=0;

//	ps1=BGBCC_JX2_EmitGetOffs(sctx);
//	if(ps1&1)
//		{ BGBCC_DBGBREAK }

#if 0
	ps1=BGBCC_JX2_EmitGetOffs(sctx);

	if(sctx->is_fixed32&16)
	{
		ps0=sctx->wex_ofs_begin;
		BGBCC_JX2C_CheckWexify(ctx, sctx, ps0, ps1);
	}
	sctx->is_fixed32&=(~16);
#endif

	if((sctx->is_fixed32&16) && !usewex)
		{ BGBCC_DBGBREAK }

	BGBCC_JX2_EndWex(sctx);

//	if(sctx->use_egpr)
//	if(ctx->is_align_wexj&2)
	if(sctx->is_align_wexj)
	{
		if(!BGBCC_JX2_CheckPadAlign32(sctx))
		{
			BGBCC_JX2_EmitPadCheckExpandLastOp(sctx);
		}
	}

	BGBCC_JX2_EmitCheckFlushIndexImm(sctx);

//	if(sctx->is_simpass==1)
	if((sctx->is_simpass==1) || (sctx->is_simpass&64))
	{
		if(sctx->is_leaf&4)
//			tr->trfl|=1;
			tr->trfl|=CCXL_TRFL_LEAF;
		else
//			tr->trfl&=~1;
			tr->trfl&=~CCXL_TRFL_LEAF;
		sctx->is_leaf&=(~4);
	}
	
//	if(sctx->op_wex_align&1)
	if((sctx->op_wex_align&1) && !(sctx->is_simpass&64))
		tr->trfl|=CCXL_TRFL_WEXALIGN;

	return(0);
}

ccxl_status BGBCC_JX2C_EndFunction(BGBCC_TransState *ctx,
	BGBCC_CCXL_LiteralInfo *obj)
{
	return(0);
}

int BGBCC_JX2C_BuildFunctionLastTraceP(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx)
{
	BGBCC_CCXL_VirtTr *tr;
	BGBCC_CCXL_VirtOp *vop;

	if((sctx->tr_trnum+1)==ctx->cur_func->n_vtr)
		return(1);

	if((sctx->tr_trnum+2)==ctx->cur_func->n_vtr)
	{
		tr=ctx->cur_func->vtr[sctx->tr_trnum+1];
		if(tr->n_ops==1)
		{
			vop=ctx->cur_func->vop[tr->b_ops+0];
			if(vop->opn==CCXL_VOP_RETDFL)
				return(1);
		}
	}

	return(0);
}

ccxl_status BGBCC_JX2C_BuildFunctionBody(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, int fcnlbl)
{
	ccxl_register reg1;

	int bs, bt, bo, co, ce, bo1, co1;
	int plsz, fnsz;
	int i, j, k;

	reg1=sctx->regalc_map[0];
	if(((s64)reg1.val)<0)
		{ BGBCC_DBGBREAK }

	ctx->cur_func=obj;
	sctx->is_tr_leaf=0;
	sctx->rov_sreg=0;
	sctx->rov_rshuf=0;

	BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);

	reg1=sctx->regalc_map[0];
	if(((s64)reg1.val)<0)
		{ BGBCC_DBGBREAK }

	BGBCC_JX2_SetSectionName(sctx, ".text");

	if((sctx->regalc_live || sctx->fregalc_live) && !(sctx->is_leaftiny&1))
			{ BGBCC_DBGBREAK }

	reg1=sctx->regalc_map[0];
	if(((s64)reg1.val)<0)
		{ BGBCC_DBGBREAK }

//	BGBCC_JX2_EmitPadForOpWord(sctx, 0xF000);
	BGBCC_JX2_EmitPadForLabel(sctx);

	bs=BGBCC_JX2_EmitGetOffs(sctx);

	sctx->is_prolog=1;
	BGBCC_JX2C_EmitFrameProlog(ctx, sctx, obj, fcnlbl);
	sctx->is_prolog=0;

	reg1=sctx->regalc_map[0];
	if(((s64)reg1.val)<0)
	{
		BGBCC_DBGBREAK
	}

//	BGBCC_JX2_EmitPadForOpWord(sctx, 0xF000);
	BGBCC_JX2_EmitPadForLabel(sctx);
	
	bt=sctx->cur_fcnbase;
	
	bo=BGBCC_JX2_EmitGetOffs(sctx);

	if((sctx->regalc_live || sctx->fregalc_live ||
		sctx->sreg_live || sctx->sfreg_live) &&
		!(sctx->is_leaftiny&1))
			{ BGBCC_DBGBREAK }

	for(i=0; i<obj->n_vtr; i++)
	{
		bo1=BGBCC_JX2_EmitGetOffs(sctx);
		sctx->tr_trnum=i;
		BGBCC_JX2C_CompileVirtTr(ctx, sctx, obj, obj->vtr[i], i);
		co1=BGBCC_JX2_EmitGetOffs(sctx);
		
		if(i<16)
		{
			sctx->fnsz_lvtr[i]=sctx->fnsz_vtr[i];
			sctx->fnsz_vtr[i]=co1-bo1;
			sctx->fnsz_dvtr[i]=sctx->fnsz_vtr[i]-sctx->fnsz_lvtr[i];
		}
	}

//	BGBCC_JX2_EmitPadForOpWord(sctx, 0xF000);
	BGBCC_JX2_EmitPadForLabel(sctx);

	co=BGBCC_JX2_EmitGetOffs(sctx);
	sctx->fnsz_bod=co-bo;

	sctx->is_tr_leaf=0;

	sctx->is_epilog=1;
	BGBCC_JX2C_EmitFrameEpilog(ctx, sctx, obj);
	sctx->is_epilog=0;

//	BGBCC_JX2_EmitPadForOpWord(sctx, 0xF000);
	BGBCC_JX2_EmitPadForLabel(sctx);
	
	BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);

	ce=BGBCC_JX2_EmitGetOffs(sctx);

	//Exception / Unwind Entry point goes here.
	BGBCC_JX2C_EmitFrameEpilogUnwind(ctx, sctx, obj);

	plsz=(bo-bt)/2;
	fnsz=(ce-bt)/2;

//	if(!sctx->is_simpass)
//	if(!sctx->is_rom)
	if(!sctx->is_rom && !sctx->abi_noexcept)
	{
		BGBCC_JX2_SetSectionName(sctx, ".pdata");

		BGBCC_JX2_EmitRelocTy(sctx, fcnlbl, BGBCC_SH_RLC_RVA32);
		BGBCC_JX2_EmitDWord(sctx, 0);	//start of function

		k=plsz | (fnsz<<8);
		BGBCC_JX2_EmitDWord(sctx, k);	//function size / etc.

		BGBCC_JX2_SetSectionName(sctx, ".text");
	}

	if(!sctx->is_simpass)
	{
		sctx->stat_func_tot++;
		if(sctx->is_leaftiny&1)
			{ sctx->stat_func_leaftiny++; }
		else if(sctx->is_leaf&1)
			{ sctx->stat_func_leaf++; }
		
		if(ctx->cur_func->regflags&BGBCC_REGFL_ALIASPTR)
			{ sctx->stat_func_alias++; }
	}

	ctx->cur_func=NULL;
	sctx->is_tr_leaf=0;

	return(0);
}

ccxl_status BGBCC_JX2C_BuildFunction(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	BGBCC_JX2_Context *sctx;
//	char tbuf[256];
	int fnsz[16];
	int fnsz_pro[16];
	int fnsz_epi[16];
	int fnsz_bod[16];
	int otrov, issta;
	int l0, l1, np, co, lo, bo, bo1, co1, sz, sz1, msz, nsz;
	byte simonly;
	int i, j, k;

	sctx=ctx->uctx;
	
	if(!obj->qname)
		obj->qname=obj->name;

	if(obj->regflags&BGBCC_REGFL_CULL)
	{
		if(sctx->cgen_log)
		{
			fprintf(sctx->cgen_log, "BGBCC_JX2C_BuildFunction: Culled %s\n",
				obj->qname);
			fflush(sctx->cgen_log);
		}

		return(0);
	}

#if 1
	if((obj->flagsint&BGBCC_TYFL_DLLIMPORT) && (obj->n_vtr<=0))
	{
		l0=obj->fxoffs;
		if(l0<=0)
		{
			l0=BGBCC_JX2_GetNamedLabel(sctx, obj->qname);
			obj->fxoffs=l0;
			BGBCC_JX2_MarkLabelIsText(sctx, l0);
		}

		l1=obj->fxnoffs;
		if(l1<=0)
		{
			l1=BGBCC_JX2_GenLabel(sctx);
//			l1=BGBCC_JX2_GetNamedLabel(sctx, obj->qname);
			obj->fxnoffs=l1;
		}

		BGBCC_JX2_SetSectionName(sctx, ".text");
		BGBCC_JX2_EmitBAlign(sctx, 4);
//		BGBCC_JX2_EmitBAlign(sctx, 16);

//		bo=BGBCC_JX2_EmitGetOffs(sctx);

		BGBCC_JX2_EmitLabel(sctx, l0);
//		BGBCC_JX2_EmitWord(sctx, 0xD301);
//		BGBCC_JX2_EmitWord(sctx, 0x6332);
//		BGBCC_JX2_EmitWord(sctx, 0x432B);
//		BGBCC_JX2_EmitWord(sctx, 0x0009);

//		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);
		if(ctx->arch_sizeof_ptr==8)
		{
			BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
				BGBCC_SH_NMID_MOVQ, BGBCC_SH_REG_R1, l1);
			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_BRA, BGBCC_SH_REG_R1);
		}else
		{
			BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
				BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1, l1);
			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_BRA, BGBCC_SH_REG_R1);
		}

//		if(sctx->is_addr64)
		if(ctx->arch_sizeof_ptr==8)
		{
			BGBCC_JX2_EmitBAlign(sctx, 8);
			BGBCC_JX2_EmitLabel(sctx, l1);
			BGBCC_JX2_EmitQWord(sctx, 0);
		}else
		{
			BGBCC_JX2_EmitBAlign(sctx, 4);
			BGBCC_JX2_EmitLabel(sctx, l1);
			BGBCC_JX2_EmitDWord(sctx, 0);
		}

		return(0);
	}
#endif

	simonly=(sctx->is_simpass&64)!=0;

	if(sctx->cgen_log)
	{
		fprintf(sctx->cgen_log, "BGBCC_JX2C_BuildFunction: Begin %s\n",
			obj->qname);
		fflush(sctx->cgen_log);

		for(i=0; i<obj->n_vtr; i++)
		{
			BGBCC_JX2C_PrintVirtTr(ctx, sctx, obj, obj->vtr[i], i);
		}
	}

#if 0
//	if(simonly)
//	if(sctx->is_simpass==2)
//	if(sctx->is_simpass==1)
	if(sctx->is_simpass && !simonly)
	{
		if((obj->n_args+obj->n_locals+obj->n_regs)<10)
		{
			if(!(obj->regflags&BGBCC_REGFL_NOTLEAFTINY))
				obj->regflags|=BGBCC_REGFL_LEAFTINY;
		}
	}
	
	if(!sctx->is_simpass)
	{
		i=(obj->n_args+obj->n_locals+obj->n_regs);
	
		if(obj->regflags&BGBCC_REGFL_LEAFTINY)
			printf("BGBCC_JX2C_BuildFunction: LeafTiny %s\n", obj->qname);
		if(!(obj->regflags&BGBCC_REGFL_NOTLEAFTINY))
			printf("BGBCC_JX2C_BuildFunction: Unclear Tiny %s %d\n",
				obj->qname, i);
	}
#endif

//	if(obj->regflags&BGBCC_REGFL_NOTLEAFTINY)
//		printf("BGBCC_JX2C_BuildFunction: Cull Early %s\n",
//			obj->qname);
	
	l0=obj->fxoffs;
	if(l0<=0)
	{
//		l0=BGBCC_JX2_GenLabel(sctx);
		l0=BGBCC_JX2_GetNamedLabel(sctx, obj->qname);
		obj->fxoffs=l0;
		BGBCC_JX2_MarkLabelIsText(sctx, l0);
	}else
	{
		if(!BGBCC_JX2_CheckLabelIsText(sctx, l0))
		{
			BGBCC_JX2_MarkLabelIsText(sctx, l0);
		}
	}
	
#if 1
	if(sctx->cgen_log)
	{
		fprintf(sctx->cgen_log, "BGBCC_JX2C_BuildFunction: BegSim %s\n",
			obj->qname);
		fflush(sctx->cgen_log);
	}

	sctx->cnt_set_fp32=0;
	sctx->cnt_set_fp64=0;

	sctx->cnt_set_dq0=0;
	sctx->cnt_set_dq1=0;
	sctx->dfl_dq=0;

	sctx->use_egpr=0;
	sctx->use_efpr=0;

	otrov=sctx->lbltrov;
	sctx->reg_vsave=0;
	sctx->freg_vsave=0;
	lo=-1;
//	bo=BGBCC_JX2_EmitGetOffs(sctx);
	sctx->simfnsz=9999999;
	nsz=0;
	msz=999999999;

	sctx->simfnmsz=msz;
	sctx->simfnnsz=nsz;

	sctx->need_farjmp=1;
	sctx->need_f16jmp=1;
	sctx->need_n12jmp=1;
	sctx->is_leaf=2;
	
//	sctx->need_n16bsr=(sctx->sim_txtsz>=(65536-4096));
//	sctx->need_n20bsr=(sctx->sim_txtsz>=(1048576-65536));
//	sctx->need_n24bsr=(sctx->sim_txtsz>=(16777216-262144));

//	sctx->dfl_dq=2;
//	sctx->dfl_dq=3;
	sctx->dfl_dq=4;

	if(sctx->is_addr64 && !sctx->has_bjx1egpr &&
			!sctx->is_betav)
	{
		sctx->dfl_dq=2;
	}

//	{ BGBCC_DBGBREAK }

//	for(np=0; np<8; np++)
//	for(np=0; np<6; np++)
	for(np=0; np<5; np++)
//	for(np=0; np<4; np++)
	{
		sctx->lbltrov=otrov;

		sctx->is_stable=1;		//pretend stable
		if(np==0)
			sctx->is_stable=0;

		if(!simonly)
		{
			BGBCC_JX2_SetBeginSimPass(sctx);
			sctx->is_simpass=np+1;
			
//			if(np<4)
			if(np<3)
			{
				sctx->is_simpass=32|(np+1);
			}

#if 1
//			if((obj->n_args+obj->n_locals+obj->n_regs)<12)
//			if((obj->n_args+obj->n_locals+obj->n_regs)<10)
			if(((obj->n_args+obj->n_locals+obj->n_regs)<10) &&
				(obj->n_args<8))
			{
				if(!(obj->regflags&BGBCC_REGFL_NOTLEAFTINY))
					obj->regflags|=BGBCC_REGFL_LEAFTINY;
			}
#endif
		}

		BGBCC_JX2C_SetupFrameLayout(ctx, sctx, obj);

		BGBCC_JX2_SetSectionName(sctx, ".text");
		BGBCC_JX2_EmitBAlign(sctx, 4);
//		BGBCC_JX2_EmitBAlign(sctx, 16);

		bo=BGBCC_JX2_EmitGetOffs(sctx);

//		BGBCC_JX2_EmitLabel(sctx, l0);
		BGBCC_JX2C_BuildFunctionBody(ctx, sctx, obj, l0);

#if 0
		sctx->is_prolog=1;
		BGBCC_JX2C_EmitFrameProlog(ctx, sctx, obj);
		sctx->is_prolog=0;
		
		for(i=0; i<obj->n_vtr; i++)
		{
			sctx->tr_trnum=i;
			BGBCC_JX2C_CompileVirtTr(ctx, sctx, obj, obj->vtr[i], i);
		}
		
		BGBCC_JX2C_EmitFrameEpilog(ctx, sctx, obj);
#endif

		co=BGBCC_JX2_EmitGetOffs(sctx);

		if(!simonly)
		{
			BGBCC_JX2_SetEndSimPass(sctx);
		}
		
		if(simonly)
			break;

//		sctx->dfl_dq=2;

#if 1
		if(!(sctx->dfl_dq&4))
		{
//			sctx->dfl_dq=0;
//			if(sctx->cnt_set_dq0>sctx->cnt_set_dq1)
			if(sctx->cnt_set_dq0>(sctx->cnt_set_dq1+10))
				sctx->dfl_dq=2;
//			if(sctx->cnt_set_dq1>sctx->cnt_set_dq0)
			if(sctx->cnt_set_dq1>(sctx->cnt_set_dq0+10))
				sctx->dfl_dq=3;
		}
#endif

//		co=BGBCC_JX2_EmitGetOffs(sctx);
		sz=co-bo;
		if(sz<msz)msz=sz;
		if(sz>nsz)nsz=sz;
		sctx->simfnsz=sz;
		sctx->simfnmsz=msz;
		sctx->simfnnsz=nsz;
		
		fnsz[np]=sz;
		fnsz_pro[np]=sctx->fnsz_pro;
		fnsz_epi[np]=sctx->fnsz_epi;
		fnsz_bod[np]=sctx->fnsz_bod;
		
		issta=1;
		if(sctx->reg_vsave!=sctx->reg_save)
			issta=0;
		if(sctx->freg_vsave!=sctx->freg_save)
			issta=0;
		if(co!=lo)
			issta=0;
		if(sctx->is_stable&2)
			issta=0;
		
		sctx->reg_vsave=sctx->reg_save;
		sctx->freg_vsave=sctx->freg_save;
		lo=co;

//		if(issta)
//		if(issta && (np>2))
//			break;

		sctx->need_farjmp=0;
		sctx->need_f16jmp=0;
		sctx->need_n12jmp=0;
		if(sctx->simfnsz>244)
			sctx->need_n12jmp=1;
//		if(sctx->simfnnsz>3840)
		if(sctx->simfnnsz>2032)
			sctx->need_f16jmp=1;
		if(sctx->simfnnsz>30720)
			sctx->need_farjmp=1;
	}

	if(simonly)
		return(0);

	if(sctx->dfl_dq)
	{
		if(sctx->dfl_dq&1)
			sctx->stat_tot_dq1++;
		else
			sctx->stat_tot_dq0++;
	}else
	{
		sctx->stat_tot_dqi++;
	}
	
	sctx->is_stable=issta;
	sctx->simfnsz=sz;
	sctx->simfnmsz=msz;
	sctx->simfnnsz=nsz;

	if(sctx->cgen_log)
	{
		fprintf(sctx->cgen_log, "BGBCC_JX2C_BuildFunction: EndSim %s np=%d\n",
			obj->qname, np);
		fflush(sctx->cgen_log);
	}
#endif


#if 0
//	if(!sctx->is_simpass)
	if(1)
	{
		i=(obj->n_args+obj->n_locals+obj->n_regs);
	
		if(obj->regflags&BGBCC_REGFL_LEAFTINY)
			printf("BGBCC_JX2C_BuildFunction: LeafTiny %s\n", obj->qname);
		else if(!(obj->regflags&BGBCC_REGFL_NOTLEAFTINY))
			printf("BGBCC_JX2C_BuildFunction: Unclear Tiny %s %d\n",
				obj->qname, i);
	}
#endif

	sctx->lbltrov=otrov;

	sz1=sctx->simfnnsz+((sctx->simfnnsz-sctx->simfnmsz)*2);
	sctx->simfnsz=sz1;
	
	sctx->need_farjmp=0;
	sctx->need_f16jmp=0;
	sctx->need_n12jmp=0;
//	if((obj->srctok*4)>=128)
//	if((obj->n_vop*6)>=128)
	if(sctx->simfnsz>244)
//	if(sctx->simfnnsz>120)
//	if(sctx->simfnnsz>60)
		sctx->need_n12jmp=1;
//	if((obj->srctok*4)>=2048)
//	if((obj->n_vop*6)>=2048)
	if(sctx->simfnsz>3840)
		sctx->need_f16jmp=1;
	if(sctx->simfnsz>30720)
		sctx->need_farjmp=1;
	
	BGBCC_JX2C_SetupFrameLayout(ctx, sctx, obj);

	BGBCC_JX2_SetSectionName(sctx, ".text");
	BGBCC_JX2_EmitBAlign(sctx, 4);
//	BGBCC_JX2_EmitBAlign(sctx, 16);

	bo1=BGBCC_JX2_EmitGetOffs(sctx);

//	BGBCC_JX2_EmitLabel(sctx, l0);
	BGBCC_JX2C_BuildFunctionBody(ctx, sctx, obj, l0);

#if 0
	sctx->is_prolog=1;
	BGBCC_JX2C_EmitFrameProlog(ctx, sctx, obj);
	sctx->is_prolog=0;
	
	for(i=0; i<obj->n_vtr; i++)
	{
		sctx->tr_trnum=i;
		BGBCC_JX2C_CompileVirtTr(ctx, sctx, obj, obj->vtr[i], i);
	}
	
	BGBCC_JX2C_EmitFrameEpilog(ctx, sctx, obj);
#endif

	co1=BGBCC_JX2_EmitGetOffs(sctx);
	sz1=co1-bo1;

	fnsz[np]=sz1;
	fnsz_pro[np]=sctx->fnsz_pro;
	fnsz_epi[np]=sctx->fnsz_epi;
	fnsz_bod[np]=sctx->fnsz_bod;

	if(sctx->reg_vsave!=sctx->reg_save)
		{ BGBCC_DBGBREAK }
	if(sctx->freg_vsave!=sctx->freg_save)
		{ BGBCC_DBGBREAK }

//	if(sctx->is_stable&2)
//		{ BGBCC_DBGBREAK }

	if(bo1!=bo)
		{ BGBCC_DBGBREAK }
		
	k=sz1-sz;
	if(k<0)k=-k;
//	if(k>64)
//	if(sz1<msz)
//		{ BGBCC_DBGBREAK }
//	if(sz1>nsz)

//	if(sz1>sctx->simfnsz)
//		{ BGBCC_DBGBREAK }

	if(sctx->cgen_log)
	{
		fprintf(sctx->cgen_log, "BGBCC_JX2C_BuildFunction: End %s\n",
			obj->qname);
		fflush(sctx->cgen_log);
	}

	if(!sctx->is_simpass && (obj->n_vtr>1) && !(sctx->is_leaftiny&1))
	{
		sctx->stat_size_prolog+=sctx->fnsz_pro;
		sctx->stat_size_epilog+=sctx->fnsz_epi;
		sctx->stat_size_body+=sctx->fnsz_bod;
		sctx->stat_size_cnt++;
	}

	return(0);
}

ccxl_status BGBCC_JX2C_BuildGlobal_EmitLitAsType(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register value)
{
	BGBCC_CCXL_LiteralInfo *litobj;
	BGBCC_CCXL_LiteralInfo *littyobj;
	BGBCC_CCXL_RegisterInfo *gblobj;
	bgbcc_vfloat128 xfa, xfb;
	ccxl_register treg;
	ccxl_type tty;
	s64 li, lj;
	s16 si, sj;
	byte bi, bj;
	double f, g;
	char *s0, *s1;
	int n, sz, sz1, al, asz, bty;
	int i, j, k;

	bty=BGBCC_CCXL_GetTypeBaseType(ctx, type);
	
	if(BGBCC_CCXL_TypeArrayP(ctx, type))
	{
		BGBCC_CCXL_TypeDerefType(ctx, type, &tty);

		if(BGBCC_CCXL_IsRegImmLiteralP(ctx, value))
		{
			i=value.val&CCXL_REGINT_MASK;
			litobj=ctx->literals[i];
//			tty=litobj->decl->type;
//			asz=BGBCC_CCXL_TypeGetArraySize(ctx, type);
			asz=BGBCC_CCXL_TypeGetArrayDimSize(ctx, type);

			al=BGBCC_CCXL_TypeGetLogicalAlign(ctx, tty);
			BGBCC_JX2_EmitBAlign(sctx, al);

			n=litobj->decl->n_listdata;
			if((asz>0) && (asz<n))
			{
				n=asz;
			}

			if((n==1) && BGBCC_CCXL_TypeSmallIntP(ctx, tty) &&
				BGBCC_CCXL_IsRegImmStringP(ctx, litobj->decl->listdata[0]))
			{
				s0=BGBCC_CCXL_GetRegImmStringValue(ctx,
					litobj->decl->listdata[0]);

				BGBCC_JX2_EmitBAlign(sctx, al);

				j=BGBCP_StrlenUTF8(s0);
				if(j>asz)j=asz;

				for(i=0; i<j; i++)
				{
					k=BGBCP_ParseChar(&s0);
					BGBCC_CCXL_GetRegForIntValue(ctx, &treg, k);
					BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
						tty, treg);
				}

				BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
				for(i=j; i<asz; i++)
				{
					BGBCC_JX2_EmitBAlign(sctx, al);
					BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
						tty, treg);
				}

				return(1);
			}

			for(i=0; i<n; i++)
			{
				BGBCC_JX2_EmitBAlign(sctx, al);
				BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, litobj->decl->listdata[i]);
			}

			BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
			for(i=n; i<asz; i++)
			{
				BGBCC_JX2_EmitBAlign(sctx, al);
				BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}
			return(1);
		}

#if 0
		if(BGBCC_CCXL_IsRegImmIntP(ctx, value) &&
			BGBCC_CCXL_IsRegImmZeroP(ctx, value))
		{
//			tty=litobj->decl->type;
			asz=BGBCC_CCXL_TypeGetArraySize(ctx, type);
			al=BGBCC_CCXL_TypeGetLogicalAlign(ctx, tty);

			BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
			for(i=0; i<asz; i++)
			{
				BGBCC_JX2_EmitBAlign(sctx, al);
				BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}
			return(1);
		}
#endif

#if 1
		if(BGBCC_CCXL_IsRegImmIntP(ctx, value))
		{
//			tty=litobj->decl->type;
			asz=BGBCC_CCXL_TypeGetArraySize(ctx, type);
			al=BGBCC_CCXL_TypeGetLogicalAlign(ctx, tty);

			i=BGBCC_CCXL_GetRegImmIntValue(ctx, value);
			BGBCC_CCXL_GetRegForIntValue(ctx, &treg, i);
			for(i=0; i<asz; i++)
			{
				BGBCC_JX2_EmitBAlign(sctx, al);
				BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}
			return(1);
		}
#endif

		if(BGBCC_CCXL_IsRegImmStringP(ctx, value))
		{
			s0=BGBCC_CCXL_GetRegImmStringValue(ctx, value);
			asz=BGBCC_CCXL_TypeGetArraySize(ctx, type);

//			j=strlen(s0);
			j=BGBCP_StrlenUTF8(s0);
			
			if(asz<j)j=asz;
			for(i=0; i<j; i++)
			{
				k=BGBCP_ParseChar(&s0);
				BGBCC_CCXL_GetRegForIntValue(ctx, &treg, k);
//				BGBCC_CCXL_GetRegForIntValue(ctx, &treg, s0[i]);
				BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}

			BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
			for(; i<asz; i++)
			{
				BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}
			if(asz<=0)
			{
				BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
	{
//		if(sctx->is_addr64)
		if(ctx->arch_sizeof_ptr==16)
			BGBCC_JX2_EmitBAlign(sctx, 16);
		else if(ctx->arch_sizeof_ptr==8)
			BGBCC_JX2_EmitBAlign(sctx, 8);
		else
			BGBCC_JX2_EmitBAlign(sctx, 4);

//		BGBCC_JX2_EmitBAlign(sctx, 4);

		if(BGBCC_CCXL_IsRegImmStringP(ctx, value))
		{
			s0=BGBCC_CCXL_GetRegImmStringValue(ctx, value);

			if(type.val==CCXL_VTY_PCHAR)
			{
				if(BGBCC_CCXL_IsRegImmU8StringP(ctx, value))
					k=BGBCC_JX2_EmitGetStrtabLabelUTF8(sctx, s0);
				else
					k=BGBCC_JX2_EmitGetStrtabLabelUTF2ASCII(sctx, s0);
			}else
				if(type.val==CCXL_VTY_PWCHAR)
			{
				k=BGBCC_JX2_EmitGetStrtabLabelUTF2UCS2(sctx, s0);
			}
			else
			{
//				k=BGBCC_JX2_EmitGetStrtabLabel(sctx, s0);
				if(BGBCC_CCXL_IsRegImmU8StringP(ctx, value))
					k=BGBCC_JX2_EmitGetStrtabLabelUTF8(sctx, s0);
				else
					k=BGBCC_JX2_EmitGetStrtabLabelUTF2ASCII(sctx, s0);
			}

			if(ctx->arch_sizeof_ptr==16)
			{
				BGBCC_JX2_EmitQWordAbs96(sctx, k);
			}else
				if(ctx->arch_sizeof_ptr==8)
			{
//				BGBCC_JX2_EmitRelocTy(sctx, k, BGBCC_SH_RLC_ABS64);
//				BGBCC_JX2_EmitQWord(sctx, 0);
				BGBCC_JX2_EmitQWordAbs64(sctx, k);
			}else
			{
//				BGBCC_JX2_EmitRelocTy(sctx, k, BGBCC_SH_RLC_ABS32);
//				BGBCC_JX2_EmitDWord(sctx, 0);
				BGBCC_JX2_EmitDWordAbs32(sctx, k);
			}
			return(1);
		}

		if(	BGBCC_CCXL_IsRegImmWStringP(ctx, value) ||
			BGBCC_CCXL_IsRegImmW4StringP(ctx, value))
		{
			s0=BGBCC_CCXL_GetRegImmStringValue(ctx, value);
			if(BGBCC_CCXL_IsRegImmW4StringP(ctx, value))
				k=BGBCC_JX2_EmitGetStrtabLabelUTF2UCS4(sctx, s0);
			else
				k=BGBCC_JX2_EmitGetStrtabLabelUTF2UCS2(sctx, s0);

			if(ctx->arch_sizeof_ptr==16)
			{
				BGBCC_JX2_EmitQWordAbs96(sctx, k);
			}else
				if(ctx->arch_sizeof_ptr==8)
			{
//				BGBCC_JX2_EmitRelocTy(sctx, k, BGBCC_SH_RLC_ABS64);
//				BGBCC_JX2_EmitQWord(sctx, 0);
				BGBCC_JX2_EmitQWordAbs64(sctx, k);
			}else
			{
//				BGBCC_JX2_EmitRelocTy(sctx, k, BGBCC_SH_RLC_ABS32);
//				BGBCC_JX2_EmitDWord(sctx, 0);
				BGBCC_JX2_EmitDWordAbs32(sctx, k);
			}
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, value);
			if(ctx->arch_sizeof_ptr==16)
			{
				BGBCC_JX2_EmitQWord(sctx, i);
				BGBCC_JX2_EmitQWord(sctx, 0);
			}else
				if(ctx->arch_sizeof_ptr==8)
				{ BGBCC_JX2_EmitQWord(sctx, i); }
			else
				{ BGBCC_JX2_EmitDWord(sctx, i); }
			return(1);
		}
		
		if(BGBCC_CCXL_IsRegImmLongP(ctx, value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, value);
			if(ctx->arch_sizeof_ptr==16)
			{
				BGBCC_JX2_EmitQWord(sctx, li);
				BGBCC_JX2_EmitQWord(sctx, 0);
			}else
				if(ctx->arch_sizeof_ptr==8)
				{ BGBCC_JX2_EmitQWord(sctx, li); }
			else
				{ BGBCC_JX2_EmitDWord(sctx, li); }
			return(1);
		}
		
		if(BGBCC_CCXL_IsRegImmGlobalAddrP(ctx, value))
		{
			i=value.val&CCXL_REGINT_MASK;
			j=(value.val>>32)&0xFFFFF;
			j=((s32)(j<<12))>>12;
//			gblobj=ctx->reg_globals[i];
//			k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, i);
			k=BGBCC_JX2C_GetGblIndexLabelAlias(ctx, sctx, i);

			if(ctx->arch_sizeof_ptr==16)
			{
				BGBCC_JX2_EmitQWordAbs64Disp(sctx, k, j);
				BGBCC_JX2_EmitQWord(sctx, 0);
			}else
				if(ctx->arch_sizeof_ptr==8)
			{
//				BGBCC_JX2_EmitQWordAbs64(sctx, k);
				BGBCC_JX2_EmitQWordAbs64Disp(sctx, k, j);
			}else
			{
//				BGBCC_JX2_EmitDWordAbs32(sctx, k);
				BGBCC_JX2_EmitDWordAbs32Disp(sctx, k, j);
			}
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((bty==CCXL_TY_I) || (bty==CCXL_TY_UI) ||
//		(((bty==CCXL_TY_NL) || (bty==CCXL_TY_UNL)) && !sctx->is_addr64))
		(((bty==CCXL_TY_NL) || (bty==CCXL_TY_UNL)) &&
			(ctx->arch_sizeof_long==4)))
	{
		BGBCC_JX2_EmitBAlign(sctx, 4);

//		if(BGBCC_CCXL_IsRegImmIntP(ctx, value))
		if(BGBCC_CCXL_IsRegImmIntP(ctx, value) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, value);
			BGBCC_JX2_EmitDWord(sctx, i);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmFloatP(ctx, value) ||
			BGBCC_CCXL_IsRegImmDoubleP(ctx, value))
		{
//			i=BGBCC_CCXL_GetRegImmIntValue(ctx, value);
			i=BGBCC_CCXL_GetRegImmDoubleValue(ctx, value);
			BGBCC_JX2_EmitDWord(sctx, i);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmStringP(ctx, value))
		{
			s0=BGBCC_CCXL_GetRegImmStringValue(ctx, value);

			if(type.val==CCXL_VTY_PCHAR)
			{
				if(BGBCC_CCXL_IsRegImmU8StringP(ctx, value))
					k=BGBCC_JX2_EmitGetStrtabLabelUTF8(sctx, s0);
				else
					k=BGBCC_JX2_EmitGetStrtabLabelUTF2ASCII(sctx, s0);
//				k=BGBCC_JX2_EmitGetStrtabLabelUTF2ASCII(sctx, s0);
			}
			else if(type.val==CCXL_VTY_PWCHAR)
			{
				k=BGBCC_JX2_EmitGetStrtabLabelUTF2UCS2(sctx, s0);
			}
			else
			{
				if(BGBCC_CCXL_IsRegImmU8StringP(ctx, value))
					k=BGBCC_JX2_EmitGetStrtabLabelUTF8(sctx, s0);
				else
					k=BGBCC_JX2_EmitGetStrtabLabelUTF2ASCII(sctx, s0);
//				k=BGBCC_JX2_EmitGetStrtabLabel(sctx, s0);
			}

			BGBCC_JX2_EmitDWordAbs32(sctx, k);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((bty==CCXL_TY_L) || (bty==CCXL_TY_UL) ||
//		(((bty==CCXL_TY_NL) || (bty==CCXL_TY_UNL)) && sctx->is_addr64))
		(((bty==CCXL_TY_NL) || (bty==CCXL_TY_UNL)) &&
			(ctx->arch_sizeof_long==8)))
	{
		if(sctx->is_addr64)
			BGBCC_JX2_EmitBAlign(sctx, 8);
		else
			BGBCC_JX2_EmitBAlign(sctx, 4);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, value);
			BGBCC_JX2_EmitQWord(sctx, li);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmStringP(ctx, value))
		{
			s0=BGBCC_CCXL_GetRegImmStringValue(ctx, value);

			if(type.val==CCXL_VTY_PCHAR)
			{
				if(BGBCC_CCXL_IsRegImmU8StringP(ctx, value))
					k=BGBCC_JX2_EmitGetStrtabLabelUTF8(sctx, s0);
				else
					k=BGBCC_JX2_EmitGetStrtabLabelUTF2ASCII(sctx, s0);
//				k=BGBCC_JX2_EmitGetStrtabLabelUTF2ASCII(sctx, s0);
			}
			else if(type.val==CCXL_VTY_PWCHAR)
			{
				k=BGBCC_JX2_EmitGetStrtabLabelUTF2UCS2(sctx, s0);
			}
			else
			{
				if(BGBCC_CCXL_IsRegImmU8StringP(ctx, value))
					k=BGBCC_JX2_EmitGetStrtabLabelUTF8(sctx, s0);
				else
					k=BGBCC_JX2_EmitGetStrtabLabelUTF2ASCII(sctx, s0);
//				k=BGBCC_JX2_EmitGetStrtabLabel(sctx, s0);
			}

			BGBCC_JX2_EmitQWordAbs64(sctx, k);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(bty==CCXL_TY_F)
	{
		BGBCC_JX2_EmitBAlign(sctx, 4);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, value);
			(*(float *)(&j))=li;
			BGBCC_JX2_EmitDWord(sctx, j);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmFloatP(ctx, value) ||
			BGBCC_CCXL_IsRegImmDoubleP(ctx, value))
		{
			f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, value);
			(*(float *)(&j))=f;
			BGBCC_JX2_EmitDWord(sctx, j);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(bty==CCXL_TY_D)
	{
		if(sctx->is_addr64)
			BGBCC_JX2_EmitBAlign(sctx, 8);
		else
			BGBCC_JX2_EmitBAlign(sctx, 4);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, value);
			(*(double *)(&lj))=li;
			BGBCC_JX2_EmitQWord(sctx, lj);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmFloatP(ctx, value) ||
			BGBCC_CCXL_IsRegImmDoubleP(ctx, value))
		{
			f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, value);
			(*(double *)(&lj))=f;
			BGBCC_JX2_EmitQWord(sctx, lj);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(bty==CCXL_TY_F16)
	{
		BGBCC_JX2_EmitBAlign(sctx, 2);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, value);
			(*(float *)(&j))=li;
			BGBCC_JX2_ConstConvFloatToHalf(j, (u16 *)(&sj));
			BGBCC_JX2_EmitWord(sctx, (u16)sj);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmFloatP(ctx, value) ||
			BGBCC_CCXL_IsRegImmDoubleP(ctx, value))
		{
			f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, value);
			(*(float *)(&j))=f;
			BGBCC_JX2_ConstConvFloatToHalf(j, (u16 *)(&sj));
			BGBCC_JX2_EmitWord(sctx, (u16)sj);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((bty==CCXL_TY_FP8S) || (bty==CCXL_TY_FP8U))
	{
//		BGBCC_JX2_EmitBAlign(sctx, 2);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, value);
			(*(float *)(&j))=li;
			BGBCC_JX2_ConstConvFloatToHalf(j, (u16 *)(&sj));
			if(bty==CCXL_TY_FP8U)
				BGBCC_JX2_ConstConvHalfToFP8U(sj, &bj);
			else
				BGBCC_JX2_ConstConvHalfToFP8S(sj, &bj);
			BGBCC_JX2_EmitByte(sctx, bj);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmFloatP(ctx, value) ||
			BGBCC_CCXL_IsRegImmDoubleP(ctx, value))
		{
			f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, value);
			(*(float *)(&j))=f;
			BGBCC_JX2_ConstConvFloatToHalf(j, (u16 *)(&sj));
			if(bty==CCXL_TY_FP8U)
				BGBCC_JX2_ConstConvHalfToFP8U(sj, &bj);
			else
				BGBCC_JX2_ConstConvHalfToFP8S(sj, &bj);
			BGBCC_JX2_EmitByte(sctx, bj);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((bty==CCXL_TY_SB) || (bty==CCXL_TY_UB))
	{
//		BGBCC_JX2_EmitBAlign(sctx, 4);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, value);
			BGBCC_JX2_EmitByte(sctx, i&0xFF);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((bty==CCXL_TY_SS) || (bty==CCXL_TY_US))
	{
		BGBCC_JX2_EmitBAlign(sctx, 2);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, value);
			BGBCC_JX2_EmitWord(sctx, i&0xFFFF);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(BGBCC_CCXL_TypeSgInt128P(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmInt128P(ctx, value) ||
			BGBCC_CCXL_IsRegImmILFDP(ctx, value))
		{
			BGBCC_JX2_EmitBAlign(sctx, 8);

			BGBCC_CCXL_GetRegImmInt128Value(ctx, value, &li, &lj);
			BGBCC_JX2_EmitQWord(sctx, li);
			BGBCC_JX2_EmitQWord(sctx, lj);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(BGBCC_CCXL_TypeFloat128P(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmFloat128P(ctx, value) ||
			BGBCC_CCXL_IsRegImmILFDP(ctx, value))
		{
			BGBCC_JX2_EmitBAlign(sctx, 8);

			BGBCC_CCXL_GetRegImmFloat128Value(ctx, value, &li, &lj);
			BGBCC_JX2_EmitQWord(sctx, li);
			BGBCC_JX2_EmitQWord(sctx, lj);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(BGBCC_CCXL_TypeVec64P(ctx, type))
	{
		if(	BGBCC_CCXL_IsRegImmIntP(ctx, value) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, value) ||
			BGBCC_CCXL_IsRegImmX64P(ctx, value))
		{
			BGBCC_JX2_EmitBAlign(sctx, 8);
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, value);
			BGBCC_JX2_EmitQWord(sctx, li);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmX128P(ctx, value))
		{
			BGBCC_JX2_EmitBAlign(sctx, 8);
			BGBCC_CCXL_GetRegImmX128Value(ctx, value, &li, &lj);
			BGBCC_JX2_EmitQWord(sctx, li);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(BGBCC_CCXL_TypeVec128P(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmX128P(ctx, value))
		{
			BGBCC_JX2_EmitBAlign(sctx, 8);

			BGBCC_CCXL_GetRegImmX128Value(ctx, value, &li, &lj);
			BGBCC_JX2_EmitQWord(sctx, li);
			BGBCC_JX2_EmitQWord(sctx, lj);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(BGBCC_CCXL_TypeVec256P(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmX128P(ctx, value))
		{
			BGBCC_JX2_EmitBAlign(sctx, 8);

			BGBCC_CCXL_GetRegImmX128Value(ctx, value, &li, &lj);
			xfa=BGBCC_Float128_FromDoubleBits(li);
			xfb=BGBCC_Float128_FromDoubleBits(lj);
			BGBCC_JX2_EmitQWord(sctx, xfa.lo);
			BGBCC_JX2_EmitQWord(sctx, xfa.hi);
			BGBCC_JX2_EmitQWord(sctx, xfb.lo);
			BGBCC_JX2_EmitQWord(sctx, xfb.hi);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmLiteralP(ctx, value))
		{
			i=value.val&CCXL_REGINT_MASK;
			litobj=ctx->literals[i];
//			tty=litobj->decl->type;
			tty=type;

			sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, tty);
			al=BGBCC_CCXL_TypeGetLogicalAlign(ctx, tty);
			BGBCC_JX2_EmitBAlign(sctx, al);

			littyobj=NULL;
			i=BGBCC_CCXL_GetTypeBaseType(ctx, tty);
			if(i>=256)
				{ littyobj=ctx->literals[i-256]; }

			if(littyobj && (littyobj->littype==CCXL_LITID_STRUCT))
//			if(littyobj &&
//				((littyobj->littype==CCXL_LITID_STRUCT) ||
//				 (littyobj->littype==CCXL_LITID_UNION)) )
			{
//				BGBCC_JX2_EmitBAlign(sctx, al);

				if(BGBCC_CCXL_IsRegImmFieldNameP(ctx,
					litobj->decl->listdata[0]))
				{
					n=littyobj->decl->n_fields;
					BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);

					for(i=0; i<n; i++)
					{
						for(j=0; j<litobj->decl->n_listdata; j+=2)
						{
							s0=BGBCC_CCXL_GetRegImmStringValue(ctx,
								litobj->decl->listdata[j]);
							s1=littyobj->decl->fields[i]->name;
							if(!strcmp(s0, s1))
								{ j++; break; }
						}
						if(j&1)
						{
							BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
								littyobj->decl->fields[i]->type,
								litobj->decl->listdata[j]);
						}else
						{
							BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
								littyobj->decl->fields[i]->type, treg);
						}
					}
				}else
				{
					n=littyobj->decl->n_fields;
					if(litobj->decl->n_listdata<n)
						n=litobj->decl->n_listdata;
				
					for(i=0; i<n; i++)
					{
						BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
							littyobj->decl->fields[i]->type,
							litobj->decl->listdata[i]);
					}

					BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
					for(i=n; i<littyobj->decl->n_fields; i++)
					{
						BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
							littyobj->decl->fields[i]->type, treg);
					}

	//				BGBCC_JX2_EmitBAlign(sctx, al);
				}


				return(1);
			}

			if(littyobj && (littyobj->littype==CCXL_LITID_UNION) )
			{
//				BGBCC_JX2_EmitBAlign(sctx, al);

				n=littyobj->decl->n_fields;
				if(litobj->decl->n_listdata<n)
					n=litobj->decl->n_listdata;

				i=0;
				
//				for(i=0; i<n; i++)
				if(n)
				{
					BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
						littyobj->decl->fields[i]->type,
						litobj->decl->listdata[i]);
				}else
				{
					BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
					BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
						littyobj->decl->fields[i]->type, treg);
				}

				sz1=BGBCC_CCXL_TypeGetLogicalSize(ctx,
					littyobj->decl->fields[i]->type);
				if(sz>sz1)
				{
					BGBCC_JX2_EmitRawBytes(sctx, NULL, sz-sz1);
				}

//				BGBCC_JX2_EmitBAlign(sctx, al);

				return(1);
			}

			BGBCC_CCXL_StubError(ctx);
			return(0);
		}

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value) &&
			BGBCC_CCXL_IsRegImmZeroP(ctx, value))
		{
//			sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
			sz=BGBCC_CCXL_TypeGetLogicalPadSize(ctx, type);
			BGBCC_JX2_EmitRawBytes(sctx, NULL, sz);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

//	if(BGBCC_CCXL_TypeRefStringP(ctx, type))
	if(BGBCC_CCXL_TypeVarRefP(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmStringP(ctx, value))
		{
			s0=BGBCC_CCXL_GetRegImmStringValue(ctx, value);

			if(BGBCC_CCXL_IsRegImmU8StringP(ctx, value))
			{
				k=BGBCC_JX2_EmitGetStrtabLabelUTF2UCS2(sctx, s0);
//				k=BGBCC_JX2_EmitGetStrtabLabelUTF8(sctx, s0);
				al=CCXL_LVA_LookupTagIndexForName(ctx, "_WString");
			}
			else
			{
				k=BGBCC_JX2_EmitGetStrtabLabelUTF2ASCIIB(sctx, s0);
				al=CCXL_LVA_LookupTagIndexForName(ctx, "_String");
			}

			if(ctx->arch_sizeof_ptr==16)
			{
				BGBCC_JX2_EmitBAlign(sctx, 16);
				BGBCC_JX2_EmitQWordAbs96Tag16(sctx, k, al);
			}else
			{
				BGBCC_JX2_EmitBAlign(sctx, 8);

//				BGBCC_JX2_EmitQWordAbs64(sctx, k);
				BGBCC_JX2_EmitQWordAbs64Tag16(sctx, k, al);
			}

			return(1);
		}

		if(	BGBCC_CCXL_IsRegImmWStringP(ctx, value) ||
			BGBCC_CCXL_IsRegImmW4StringP(ctx, value))
		{
			s0=BGBCC_CCXL_GetRegImmStringValue(ctx, value);
			if(BGBCC_CCXL_IsRegImmW4StringP(ctx, value))
			{
				k=BGBCC_JX2_EmitGetStrtabLabelUTF2UCS4(sctx, s0);
				al=CCXL_LVA_LookupTagIndexForName(ctx, "_UString");
			}
			else
			{
				k=BGBCC_JX2_EmitGetStrtabLabelUTF2UCS2(sctx, s0);
				al=CCXL_LVA_LookupTagIndexForName(ctx, "_WString");
			}

			if(ctx->arch_sizeof_ptr==16)
			{
				BGBCC_JX2_EmitBAlign(sctx, 16);
				BGBCC_JX2_EmitQWordAbs96Tag16(sctx, k, al);
			}else
			{
				BGBCC_JX2_EmitBAlign(sctx, 8);
//				BGBCC_JX2_EmitQWordAbs64(sctx, k);
				BGBCC_JX2_EmitQWordAbs64Tag16(sctx, k, al);
			}

			return(1);
		}

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, value);

			if(ctx->arch_sizeof_ptr==16)
			{
				BGBCC_JX2_EmitBAlign(sctx, 16);
				lj=(li>>63)&0x3FFFFFFFFFFFFFFFULL;
				lj|=0x4000000000000000ULL;
				BGBCC_JX2_EmitQWord(sctx, lj);
				BGBCC_JX2_EmitQWord(sctx, li);
			}else
			{
				li&=0x3FFFFFFFFFFFFFFFULL;
				li|=0x4000000000000000ULL;
				BGBCC_JX2_EmitBAlign(sctx, 8);
				BGBCC_JX2_EmitQWord(sctx, li);
			}
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmFloatP(ctx, value) ||
			BGBCC_CCXL_IsRegImmDoubleP(ctx, value))
		{
			f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, value);
			(*(double *)(&lj))=f;

			if(ctx->arch_sizeof_ptr==16)
			{
				BGBCC_JX2_EmitBAlign(sctx, 16);
				li=(lj>>4)&0x3FFFFFFFFFFFFFFFULL;
				li|=0x5000000000000000ULL;

				BGBCC_JX2_EmitQWord(sctx, li);
				BGBCC_JX2_EmitQWord(sctx, lj);
			}else
			{
				li=lj>>2;
				li&=0x3FFFFFFFFFFFFFFFULL;
				li|=0x8000000000000000ULL;
				BGBCC_JX2_EmitBAlign(sctx, 8);
				BGBCC_JX2_EmitQWord(sctx, li);
			}
			return(1);
		}
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

ccxl_status BGBCC_JX2C_BuildGlobalTls(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	BGBCC_JX2_Context *sctx;
	ccxl_type tty;
	int ix, sz, al, ofs, l0, l1;
	
	sctx=ctx->uctx;
	
	ix=sctx->n_tlsvar++;
	if(!ix)
	{
		BGBCC_JX2C_CheckTlsInit(ctx);
	}

	obj->fxoffs=BGBCC_SH_LBL_TLSSTRT+ix;


	sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->type);
	al=BGBCC_CCXL_TypeGetLogicalAlign(ctx, obj->type);

	if(BGBCC_CCXL_TypeArrayP(ctx, obj->type))
	{
		BGBCC_CCXL_TypeDerefType(ctx, obj->type, &tty);
		al=BGBCC_CCXL_TypeGetLogicalAlign(ctx, tty);
	}

	if(al<1)al=1;
	if(sctx->is_addr64)
		{ if(al>8)al=8; }
	else
		{ if(al>4)al=4; }
	if(al&(al-1))al=4;

	if(sz<1)sz=1;
//		sz=(sz+3)&(~3);
	sz=(sz+(al-1))&(~(al-1));

	ofs=sctx->sz_tlsvar;
	ofs=(ofs+(al-1))&(~(al-1));
	sctx->sz_tlsvar=ofs+sz;


	sctx->tlsvar_lbl[ix]=obj->fxoffs;	
//	obj->fxnoffs=ix;

	sctx->tlsvar_ofs[ix]=ofs;
	sctx->tlsvar_sz[ix]=sz;

//	BGBCC_JX2_EmitCommSym(sctx, l0, sz, al);
	return(1);
}

ccxl_status BGBCC_JX2C_BuildGlobal(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	BGBCC_JX2_Context *sctx;
	BGBCC_CCXL_LiteralInfo *litobj;
	BGBCC_CCXL_LiteralInfo *littyobj;
	ccxl_register treg;
	ccxl_type tty;
	char *s0;
	int l0, sz, al, al1, asz, iskv, psz, pad;
	s64 li;
	int n, hasval, donullpad;
	int i, j, k;

	sctx=ctx->uctx;
	
	if(!obj->qname)
		obj->qname=obj->name;

	if(obj->regflags&BGBCC_REGFL_CULL)
	{
		if(sctx->cgen_log)
		{
			fprintf(sctx->cgen_log, "BGBCC_JX2C_BuildGlobal: Culled %s\n",
				obj->qname);
			fflush(sctx->cgen_log);
		}

		s0=obj->qname;
		if((s0[0]=='_') && (s0[1]=='_'))
		{
			k=-1;
		}

		return(0);
	}

//	if(	(obj->flagsint&BGBCC_TYFL_THREAD)	||
//		(obj->flagsint&BGBCC_TYFL_DYNAMIC)	)

	if(obj->flagsint&BGBCC_TYFL_THREAD)
	{
		return(BGBCC_JX2C_BuildGlobalTls(ctx, obj));
	}

	l0=obj->fxoffs;
	if(l0<=0)
	{
		s0=obj->qname;
		if((s0[0]=='_') && (s0[1]=='_'))
		{
			if(	!strcmp(s0, "__rsrc_start") ||
				!strcmp(s0, "__rsrc_dir") ||
				!strcmp(s0, "__rsrc_types"))
			{
				/* referencing the resource section causes it to exist. */
				BGBCC_JX2C_CheckRWadInit(ctx);
			}

			if(!strncmp(s0, "__rsrc_", 7))
			{
				/* referencing the resource section causes it to exist. */
				BGBCC_JX2C_CheckRWadInit(ctx);
			}
		}
		
		if(!strcmp(s0, "__tolow"))
		{
			if(!(sctx->is_simpass))
			{
				k=-1;
			}
		}

//		lbl=BGBCC_JX2_LookupNamedLabel(ctx, name);
//		l0=BGBCC_JX2_GenLabel(sctx);
		l0=BGBCC_JX2_GetNamedLabel(sctx, obj->qname);
		obj->fxoffs=l0;
	}

	if(l0<=0)
	{
		if(!(sctx->is_simpass))
		{
			k=-1;
		}
	}
	
	if((l0>=BGBCC_SH_LBL_ARCHSTRT) && (l0<BGBCC_SH_LBL_ARCHEND))
	{
		/* This variable is a system register. */
		return(0);
	}

	s0=obj->qname;
	if((s0[0]=='_') && (s0[1]=='_'))
	{
		if(	!strcmp(s0, "__text_start") ||
			!strcmp(s0, "__utext_start") ||
			!strcmp(s0, "__udata_start") ||
			!strcmp(s0, "__data_start") ||
			!strcmp(s0, "__bss_start") )
		{
			return(0);
		}

		if(	!strcmp(s0, "__text_end") ||
			!strcmp(s0, "__utext_end") ||
			!strcmp(s0, "__udata_end") ||
			!strcmp(s0, "__data_end") ||
			!strcmp(s0, "__bss_end") )
		{
			return(0);
		}
		
		if(!strcmp(s0, "__global_ptr"))
		{
			return(0);
		}

		if(!strncmp(s0, "__rsrc_", 7))
		{
			return(0);
		}
	}

	if(sctx->cgen_log)
	{
		fprintf(sctx->cgen_log, "BGBCC_JX2C_BuildGlobal: %s\n",
			obj->qname);
		fflush(sctx->cgen_log);
	}

	hasval=1;

	if(!obj->value.val)
		hasval=0;
	if(obj->value.val==CCXL_REGTY_IMM_INT)
		hasval=0;
	if(obj->value.val==CCXL_REGTY_IMM_LONG)
		hasval=0;
	if(obj->value.val==CCXL_REGTY_IMM_FLOAT)
		hasval=0;
	if(obj->value.val==CCXL_REGTY_IMM_DOUBLE)
		hasval=0;
	
//	if(obj->flagsint&BGBCC_TYFL_REGISTER)
//		hasval=1;
	
//	if(!obj->value.val)
	if(!hasval)
	{
//		if(!strcmp(obj->qname, "snd_scaletable"))
//		if(!strcmp(obj->qname, "tkmm_lva_clsinfo"))
//			i=-1;
		
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->type);
		al=BGBCC_CCXL_TypeGetLogicalAlign(ctx, obj->type);
		
		if(BGBCC_CCXL_TypeArrayP(ctx, obj->type))
		{
			psz=BGBCC_JX2C_CheckPadToBndTag16(ctx, sctx, sz);
		}
		else
		{
			psz=0;
			pad=0;
		}

		if(BGBCC_CCXL_TypeArrayP(ctx, obj->type))
		{
			BGBCC_CCXL_TypeDerefType(ctx, obj->type, &tty);
			al=BGBCC_CCXL_TypeGetLogicalAlign(ctx, tty);
			
			if(psz!=sz)
			{
				al=1<<(BGBCC_JX2C_SizeToFp8(psz)>>3);
			}
			
			if(al<8)
				al=8;
		}

		if(al<1)al=1;
		if(psz==sz)
		{
			if(ctx->arch_sizeof_ptr==16)
			{
				if(al>16)al=16;
			}else
			{
				if(al>8)al=8;
			}
		}

//		if(sctx->is_addr64)
//				{ if(al>8)al=8; }
//		else
//			{ if(al>4)al=4; }

//		if(al&(al-1))al=4;
		if(al&(al-1))al=8;

		if(sz<1)sz=1;
//		sz=(sz+3)&(~3);
		sz=(sz+(al-1))&(~(al-1));

		if(psz<1)psz=1;
		psz=(psz+(al-1))&(~(al-1));

//		if(BGBCC_CCXL_TypeArrayP(ctx, obj->type))
//			sz+=8;

//		if(obj->flagsint&BGBCC_TYFL_REGISTER)
//		if((BGBCC_JX2_EmitGetSecOffs(sctx, BGBCC_SH_CSEG_DATA)<512) && (sz<=16))
//		if((BGBCC_JX2_EmitGetSecOffs(sctx, BGBCC_SH_CSEG_DATA)<4096) &&
//		if((BGBCC_JX2_EmitGetSecOffs(sctx, BGBCC_SH_CSEG_DATA)<8192) &&

#if 1
		if((BGBCC_JX2_EmitGetSecOffs(sctx, BGBCC_SH_CSEG_DATA)<32768) &&
			(sz<=16))
#endif

#if 0
		if(((BGBCC_JX2_EmitGetSecOffs(sctx, BGBCC_SH_CSEG_DATA)<32768) &&
			(sz<=16)) ||
			((BGBCC_JX2_EmitGetSecOffs(sctx, BGBCC_SH_CSEG_DATA)<524288) &&
//			(obj->gblrefcnt>=6) && (sctx->has_fmovs&64)))
//			(obj->gblrefcnt>=8) && (sctx->has_fmovs&64)))
			(obj->gblrefcnt>=12) && (sctx->has_fmovs&64)))
//			(obj->gblrefcnt_org>=16) && (sctx->has_fmovs&64)))
#endif
		{
			BGBCC_JX2_SetSectionName(sctx, ".data");
			BGBCC_JX2_EmitBAlign(sctx, al);
			BGBCC_JX2_EmitLabel(sctx, l0);
			BGBCC_JX2_EmitRawBytes(sctx, NULL, sz);
//			BGBCC_JX2_EmitRawBytes(sctx, NULL, psz);
		}else
		{
			BGBCC_JX2_EmitCommSym(sctx, l0, sz, al);
//			BGBCC_JX2_EmitCommSym(sctx, l0, psz, al);
		}

//		BGBCC_JX2_SetSectionName(sctx, ".bss");
//		BGBCC_JX2_EmitBAlign(sctx, 4);
//		BGBCC_JX2_EmitLabel(sctx, l0);
//		BGBCC_JX2_EmitRawBytes(sctx, NULL, sz);
		return(1);
	}
	
	donullpad=0;
	
	sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->type);
	al=BGBCC_CCXL_TypeGetLogicalAlign(ctx, obj->type);

	if(BGBCC_CCXL_TypeArrayP(ctx, obj->type))
	{
		BGBCC_CCXL_TypeDerefType(ctx, obj->type, &tty);
		al=BGBCC_CCXL_TypeGetLogicalAlign(ctx, tty);

		if(BGBCC_CCXL_TypePointerP(ctx, tty))
			donullpad=1;

		if(al<8)
			al=8;
	}

	if(al<1)al=1;
	if(ctx->arch_sizeof_ptr==16)
	{
		if(al>16)al=16;
	}else
	{
		if(al>8)al=8;
	}

//		if(sctx->is_addr64)
//			{ if(al>8)al=8; }
//	else
//		{ if(al>4)al=4; }
//	if(al&(al-1))al=4;
	if(al&(al-1))al=8;

	if(sz>=512)
		{ al=16; }

	iskv=0;

	if((obj->flagsint&BGBCC_TYFL_CONST) &&
		!BGBCC_CCXL_TypePointerP(ctx, obj->type))
			iskv=1;
	if((obj->flagsint&BGBCC_TYFL_CONST) &&
		BGBCC_CCXL_TypeArrayP(ctx, obj->type))
			iskv=1;
	if(obj->flagsint&BGBCC_TYFL_CONST2)
		iskv=1;

	if(iskv)
	{
		BGBCC_JX2_SetSectionName(sctx, ".rodata");
	}else
	{
		BGBCC_JX2_SetSectionName(sctx, ".data");
	}

	if(BGBCC_CCXL_TypeArrayP(ctx, obj->type))
	{
		if(sz>=2048)
		{
			BGBCC_JX2_EmitBAlign(sctx, 16);
			BGBCC_JX2_EmitRelocTy(sctx, l0, BGBCC_SH_RLC_TRIPWIRE_BJX);
			BGBCC_JX2_EmitRawBytes(sctx, NULL, 16);
		}
	}

//	BGBCC_JX2_EmitBAlign(sctx, 4);
	BGBCC_JX2_EmitBAlign(sctx, al);
	BGBCC_JX2_EmitLabel(sctx, l0);
	
//	if(BGBCC_CCXL_TypeSmallIntP(ctx, obj->type))
	if(BGBCC_CCXL_TypeSgIntP(ctx, obj->type) ||
		(BGBCC_CCXL_TypeSgNLongP(ctx, obj->type) &&
			(ctx->arch_sizeof_long==4)))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, obj->value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, obj->value);
			BGBCC_JX2_EmitDWord(sctx, i);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmLongP(ctx, obj->value))
		{
			i=BGBCC_CCXL_GetRegImmLongValue(ctx, obj->value);
			BGBCC_JX2_EmitDWord(sctx, i);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmFloatP(ctx, obj->value) ||
			BGBCC_CCXL_IsRegImmDoubleP(ctx, obj->value))
		{
			i=BGBCC_CCXL_GetRegImmDoubleValue(ctx, obj->value);
			BGBCC_JX2_EmitDWord(sctx, i);
			return(1);
		}
	}

	if(BGBCC_CCXL_TypeSgLongP(ctx, obj->type) ||
//		(BGBCC_CCXL_TypeSgNLongP(ctx, obj->type) && sctx->is_addr64))
		(BGBCC_CCXL_TypeSgNLongP(ctx, obj->type) &&
			(ctx->arch_sizeof_long==8)))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, obj->value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, obj->value);
			BGBCC_JX2_EmitQWord(sctx, i);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmLongP(ctx, obj->value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, obj->value);
			BGBCC_JX2_EmitQWord(sctx, li);
			return(1);
		}
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, obj->type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, obj->type))
	{
		BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
			obj->type, obj->value);
		return(1);
	}

	if(BGBCC_CCXL_TypeSmallIntP(ctx, obj->type))
	{
		BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
			obj->type, obj->value);
		return(1);
	}

	if(BGBCC_CCXL_TypePointerP(ctx, obj->type))
	{
		if(BGBCC_CCXL_IsRegImmStringP(ctx, obj->value))
		{
			s0=BGBCC_CCXL_GetRegImmStringValue(ctx, obj->value);
			if(obj->type.val==CCXL_VTY_PCHAR)
			{
				if(BGBCC_CCXL_IsRegImmU8StringP(ctx, obj->value))
					k=BGBCC_JX2_EmitGetStrtabLabelUTF8(sctx, s0);
				else
					k=BGBCC_JX2_EmitGetStrtabLabelUTF2ASCII(sctx, s0);
//				k=BGBCC_JX2_EmitGetStrtabLabelUTF2ASCII(sctx, s0);
			}
			else if(obj->type.val==CCXL_VTY_PWCHAR)
			{
				k=BGBCC_JX2_EmitGetStrtabLabelUTF2UCS2(sctx, s0);
			}
			else
			{
				if(BGBCC_CCXL_IsRegImmU8StringP(ctx, obj->value))
					k=BGBCC_JX2_EmitGetStrtabLabelUTF8(sctx, s0);
				else
					k=BGBCC_JX2_EmitGetStrtabLabelUTF2ASCII(sctx, s0);
//				k=BGBCC_JX2_EmitGetStrtabLabel(sctx, s0);
			}

			if(ctx->arch_sizeof_ptr==16)
			{
				BGBCC_JX2_EmitQWordAbs96(sctx, k);
			}else
				if(ctx->arch_sizeof_ptr==8)
			{
//				BGBCC_JX2_EmitRelocTy(sctx, k, BGBCC_SH_RLC_ABS64);
//				BGBCC_JX2_EmitQWord(sctx, 0);
				BGBCC_JX2_EmitQWordAbs64(sctx, k);
			}else
			{
//				BGBCC_JX2_EmitRelocTy(sctx, k, BGBCC_SH_RLC_ABS32);
//				BGBCC_JX2_EmitDWord(sctx, 0);
				BGBCC_JX2_EmitDWordAbs32(sctx, k);
			}
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmGlobalAddrP(ctx, obj->value))
		{
			i=obj->value.val&CCXL_REGINT_MASK;
			j=(obj->value.val>>32)&0xFFFFF;
			j=((s32)(j<<12))>>12;

//			k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, i);
			k=BGBCC_JX2C_GetGblIndexLabelAlias(ctx, sctx, i);

			if(ctx->arch_sizeof_ptr==16)
			{
				BGBCC_JX2_EmitQWordAbs96Disp(sctx, k, j);
			}else
				if(ctx->arch_sizeof_ptr==8)
			{
//				BGBCC_JX2_EmitQWordAbs64(sctx, k);
				BGBCC_JX2_EmitQWordAbs64Disp(sctx, k, j);
			}else
			{
//				BGBCC_JX2_EmitDWordAbs32(sctx, k);
				BGBCC_JX2_EmitDWordAbs32Disp(sctx, k, j);
			}
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmIntP(ctx, obj->value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, obj->value);

			if(ctx->arch_sizeof_ptr==16)
			{
				BGBCC_JX2_EmitQWord(sctx, i);
				BGBCC_JX2_EmitQWord(sctx, 0);
			}else
				if(ctx->arch_sizeof_ptr==8)
					{ BGBCC_JX2_EmitQWord(sctx, i); }
			else
				{ BGBCC_JX2_EmitDWord(sctx, i); }
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmLongP(ctx, obj->value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, obj->value);

			if(ctx->arch_sizeof_ptr==16)
			{
				BGBCC_JX2_EmitQWord(sctx, li);
				BGBCC_JX2_EmitQWord(sctx, 0);
			}else
				if(ctx->arch_sizeof_ptr==8)
				{ BGBCC_JX2_EmitQWord(sctx, li); }
			else
				{ BGBCC_JX2_EmitDWord(sctx, li); }
			return(1);
		}
	}

	if(BGBCC_CCXL_IsRegImmLiteralP(ctx, obj->value))
	{
//		if(!strcmp(obj->qname, "keynames"))
//		{
//			k=-1;
//		}
	
		BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
			obj->type, obj->value);
		return(1);
	}

	if(BGBCC_CCXL_TypeArrayP(ctx, obj->type))
	{
		BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
			obj->type, obj->value);
		
		if(donullpad)
		{
			if(ctx->arch_sizeof_ptr==16)
			{
				BGBCC_JX2_EmitQWord(sctx, 0);
				BGBCC_JX2_EmitQWord(sctx, 0);
			}else
				if(ctx->arch_sizeof_ptr==8)
					{ BGBCC_JX2_EmitQWord(sctx, 0); }
			else
				{ BGBCC_JX2_EmitDWord(sctx, 0); }
		}

		if(sz>=2048)
		{
			BGBCC_JX2_EmitBAlign(sctx, 16);
			BGBCC_JX2_EmitRelocTy(sctx, l0, BGBCC_SH_RLC_TRIPWIRE_BJX);
			BGBCC_JX2_EmitRawBytes(sctx, NULL, 16);
		}

		return(1);
	}
	
	if(BGBCC_CCXL_TypeValueObjectP(ctx, obj->type))
	{
		BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
			obj->type, obj->value);

#if 0
		if(sz>=1024)
		{
			BGBCC_JX2_EmitBAlign(ctx, 16);
			BGBCC_JX2_EmitRelocTy(ctx, l0, BGBCC_SH_RLC_TRIPWIRE_BJX);
			BGBCC_JX2_EmitRawBytes(ctx, NULL, 16);
		}
#endif

		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	BGBCC_JX2_EmitRawBytes(sctx, NULL, sz);
	return(1);
}

ccxl_status BGBCC_JX2C_BuildAsmBlob(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	BGBCC_JX2_Context *sctx;
	int l0, sz;
	int i, j, k;

	if(obj->regflags&BGBCC_REGFL_CULL)
	{
		return(0);
	}

	sctx=ctx->uctx;

	sctx->simfnsz=99999999;
	sctx->simfnmsz=99999999;
	sctx->simfnnsz=99999999;

	sctx->need_farjmp=1;
	sctx->need_f16jmp=1;
	sctx->need_n12jmp=1;
	
	ctx->lfn=obj->alc_fn;
	ctx->lln=obj->alc_ln;
	
	BGBCC_JX2C_AssembleBuffer(ctx, sctx, (char *)(obj->text));
	return(1);
}

ccxl_status BGBCC_JX2C_BuildObjBlob(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	BGBCC_JX2_Context *sctx;
	byte *buf;
	int l0, sz;
	int i, j, k;

	sctx=ctx->uctx;
	
	buf=obj->text;
	sz=obj->sz_text;
	
	BGBCC_JX2_BuildLoadObj(sctx, buf, sz);
	
//	BGBCC_JX2C_AssembleBuffer(ctx, sctx, (char *)(obj->text));
	return(1);
}

ccxl_status BGBCC_JX2C_BuildPrestartInit(BGBCC_TransState *ctx)
{
	char tb[256];
	BGBCC_JX2_Context *sctx;
	char *s;
	int l0, l1, l2, l3, nm1;
	int i;

	sctx=ctx->uctx;

//	l0=BGBCC_JX2_GetNamedLabel(sctx, "__prestart_init");
	l0=BGBCC_JX2_GetNamedLabel(sctx, "__start_init");

	/* Object Init Stub */
	BGBCC_JX2_SetSectionName(sctx, ".text");
	BGBCC_JX2_EmitBAlign(sctx, 8);
	BGBCC_JX2_EmitLabel(sctx, l0);

//	BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_PR);

	BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD, -8, BGBCC_SH_REG_SP);
	BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
		BGBCC_SH_REG_PR, BGBCC_SH_REG_R1);
	BGBCC_JX2_EmitOpRegStReg(sctx, BGBCC_SH_NMID_MOVQ,
		BGBCC_SH_REG_R1, BGBCC_SH_REG_SP);
	
	for(i=0; i<1024; i++)
	{
		sprintf(tb, "__prestart_init!%d", i);
		s=bgbcc_strdup(tb);

		l1=BGBCC_JX2_LookupNamedLabel(sctx, s);
		if(l1<=0)
			break;
		
		nm1=BGBCC_SH_NMID_BSRN;
		BGBCC_JX2_EmitOpAutoLabel(sctx, nm1, l1);

#if 0
		if(BGBCC_JX2_EmitCheckAutoLabelNear20B(sctx, l1, nm1))
		{
			BGBCC_JX2_EmitOpFar20Label(sctx, nm1, l1);
			continue;
		}
		if(BGBCC_JX2_EmitCheckAutoLabelNear24B(sctx, l1, nm1))
		{
			BGBCC_JX2_EmitOpFar24Label(sctx, nm1, l1);
			continue;
		}
#endif
	}

	BGBCC_JX2_EmitOpLdRegReg(sctx, BGBCC_SH_NMID_MOVQ,
		BGBCC_SH_REG_SP, BGBCC_SH_REG_R1);
	BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD, 8, BGBCC_SH_REG_SP);
	BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_JMP, BGBCC_SH_REG_R1);

//	BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);
//	BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RET);
	
	return(0);
}

ccxl_status BGBCC_JX2C_BuildStruct(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	char tb[256];
	int vt_lbl[1024];
	BGBCC_CCXL_RegisterInfo *vt_vmi[1024];

	BGBCC_CCXL_RegisterInfo *aclz[64];
	int tlbl[1024];

	int vif_lbl[256];
	BGBCC_CCXL_RegisterInfo *vif_vmi[256];

	BGBCC_JX2_Context *sctx;
	BGBCC_CCXL_LiteralInfo *li_sclz;
	BGBCC_CCXL_RegisterInfo *sclz, *cclz, *cif, *fi, *fj;
	int l0, l1, l2, l3;
	int l4, l5, l6, l7;
	int nvtix, naclz, nif;
	int i, j, k, l;

	sctx=ctx->uctx;

	if(!(obj->flagsint&BGBCC_TYFL_NONPOD))
	{
		return(1);
	}

	if(!obj->qname)
		obj->qname=obj->name;

	sclz=NULL;
	if(obj->n_args>0)
	{
		li_sclz=BGBCC_CCXL_LookupStructureForSig(ctx,
			obj->args[0]->sig);
		sclz=li_sclz->decl;
	}

#if 0
	l0=obj->fxoffs;
	if(l0<=0)
	{
//		lbl=BGBCC_JX2_LookupNamedLabel(ctx, name);
//		l0=BGBCC_JX2_GenLabel(sctx);
		l0=BGBCC_JX2_GetNamedLabel(sctx, obj->qname);
		obj->fxoffs=l0;
	}
#endif

	sprintf(tb, "%s/%s", obj->qname, "__vtable");
	l0=BGBCC_JX2_GetNamedLabel(sctx, tb);

	sprintf(tb, "%s/%s", obj->qname, "__init");
	l1=BGBCC_JX2_GetNamedLabel(sctx, tb);

	sprintf(tb, "%s/%s", obj->qname, "__cinfo");
	l2=BGBCC_JX2_GetNamedLabel(sctx, tb);

	l3=0;
	if(sclz)
	{
		sprintf(tb, "%s/%s", sclz->qname, "__cinfo");
		l3=BGBCC_JX2_GetNamedLabel(sctx, tb);
	}

	nvtix=obj->n_vargs;
	for(i=0; i<nvtix; i++)
	{
		vt_lbl[i]=0;
		vt_vmi[i]=NULL;
	}
	
	if(obj->regflags&BGBCC_REGFL_VARCONV)
	{
		vt_lbl[0]=l2;
	}

	naclz=0;
	cclz=obj;
	while(cclz)
	{
		aclz[naclz++]=cclz;

		for(i=0; i<cclz->n_regs; i++)
		{
			fi=cclz->regs[i];
			j=fi->fxmoffs;
			if(j<=0)
				continue;
			
			sprintf(tb, "%s/%s", cclz->qname, fi->name);
			k=BGBCC_JX2_GetNamedLabel(sctx, tb);
			if(vt_lbl[j]<=0)
				{ vt_lbl[j]=k; vt_vmi[j]=fi; }
		}

		if(cclz->n_args>0)
		{
			li_sclz=BGBCC_CCXL_LookupStructureForSig(ctx,
				cclz->args[0]->sig);
			cclz=li_sclz->decl;
		}else
		{
			cclz=NULL;
		}
	}


	BGBCC_JX2_SetSectionName(sctx, ".rodata");
//	BGBCC_JX2_EmitBAlign(sctx, 4);
	BGBCC_JX2_EmitBAlign(sctx, 8);

	/* Main VTable */

	BGBCC_JX2_EmitLabel(sctx, l0);
	for(i=0; i<nvtix; i++)
		{ BGBCC_JX2_EmitPtrWordAbs(sctx, vt_lbl[i]); }
	BGBCC_JX2_EmitPtrWordAbs(sctx, 0);



	if(obj->regflags&BGBCC_REGFL_VARCONV)
	{
		/* Class Info */

		l4=BGBCC_JX2_GenLabel(sctx);
		l5=BGBCC_JX2_GenLabel(sctx);
		l6=BGBCC_JX2_GenLabel(sctx);

		BGBCC_JX2_EmitLabel(sctx, l2);	
		k=BGBCC_JX2_EmitGetStrtabLabel(sctx, obj->qname);
		BGBCC_JX2_EmitDWordRva32(sctx, l2);		/* Self Pointer */
		BGBCC_JX2_EmitDWordRva32(sctx, k);		/* QName */
		BGBCC_JX2_EmitDWordRva32(sctx, l3);		/* Super */
		BGBCC_JX2_EmitDWordRva32(sctx, l4);		/* Fields */
		BGBCC_JX2_EmitDWordRva32(sctx, l5);		/* Methods */
		BGBCC_JX2_EmitDWordRva32(sctx, l6);		/* IFace */

		/* Build Fields List */
		for(i=0; i<obj->n_fields; i++)
		{
			k=BGBCC_JX2_GenLabel(sctx);
			tlbl[i]=k;

			BGBCC_JX2_EmitBAlign(sctx, 8);
			BGBCC_JX2_EmitLabel(sctx, k);

			fi=obj->fields[i];
			j=BGBCC_JX2_EmitGetStrtabLabel(sctx, fi->name);
			k=BGBCC_JX2_EmitGetStrtabLabel(sctx, fi->sig);
			BGBCC_JX2_EmitDWordRva32(sctx, j);		/* Name */
			BGBCC_JX2_EmitDWordRva32(sctx, k);		/* Sig */
	//		BGBCC_JX2_EmitQWord(sctx, fi->flagsint);
			BGBCC_JX2_EmitDWord(sctx, fi->flagsint);
			BGBCC_JX2_EmitDWord(sctx, fi->fxoffs);
	//		BGBCC_JX2_EmitDWord(sctx, 0);
		}

		BGBCC_JX2_EmitBAlign(sctx, 8);
		BGBCC_JX2_EmitLabel(sctx, l4);
		for(i=0; i<obj->n_fields; i++)
			{ BGBCC_JX2_EmitPtrWordAbs(sctx, tlbl[i]); }
		BGBCC_JX2_EmitPtrWordAbs(sctx, 0);


		/* Build Methods List */
		for(i=0; i<obj->n_regs; i++)
		{
			k=BGBCC_JX2_GenLabel(sctx);
			tlbl[i]=k;

			BGBCC_JX2_EmitBAlign(sctx, 8);
			BGBCC_JX2_EmitLabel(sctx, k);

			fi=obj->regs[i];
			j=BGBCC_JX2_EmitGetStrtabLabel(sctx, fi->name);
			k=BGBCC_JX2_EmitGetStrtabLabel(sctx, fi->sig);
			BGBCC_JX2_EmitDWordRva32(sctx, j);		/* Name */
			BGBCC_JX2_EmitDWordRva32(sctx, k);		/* Sig */
	//		BGBCC_JX2_EmitQWord(sctx, fi->flagsint);
			BGBCC_JX2_EmitDWord(sctx, fi->flagsint);
			BGBCC_JX2_EmitDWord(sctx, fi->fxoffs);
	//		BGBCC_JX2_EmitDWord(sctx, 0);
		}

		BGBCC_JX2_EmitBAlign(sctx, 8);
		BGBCC_JX2_EmitLabel(sctx, l5);
		for(i=0; i<obj->n_regs; i++)
			{ BGBCC_JX2_EmitDWordRva32(sctx, tlbl[i]); }
		BGBCC_JX2_EmitDWordRva32(sctx, 0);


		/* Build Interface List */
		for(i=1; i<obj->n_args; i++)
		{
			k=BGBCC_JX2_GenLabel(sctx);
			tlbl[i]=k;

			BGBCC_JX2_EmitBAlign(sctx, 8);
			BGBCC_JX2_EmitLabel(sctx, k);

			fi=obj->args[i];
			li_sclz=BGBCC_CCXL_LookupStructureForSig(ctx, fi->sig);
			cclz=li_sclz->decl;

			j=BGBCC_JX2_EmitGetStrtabLabel(sctx, cclz->qname);
	//		k=BGBCC_JX2_EmitGetStrtabLabel(sctx, fi->sig);

			sprintf(tb, "%s/%s", cclz->qname, "__cinfo");
			k=BGBCC_JX2_GetNamedLabel(sctx, tb);

			BGBCC_JX2_EmitDWordRva32(sctx, j);		/* Name */
			BGBCC_JX2_EmitDWordRva32(sctx, k);		/* Sig */
	//		BGBCC_JX2_EmitQWord(sctx, fi->flagsint);
			BGBCC_JX2_EmitDWord(sctx, fi->flagsint);
			BGBCC_JX2_EmitDWord(sctx, fi->fxoffs);
	//		BGBCC_JX2_EmitDWord(sctx, 0);
		}

		BGBCC_JX2_EmitBAlign(sctx, 8);
		BGBCC_JX2_EmitLabel(sctx, l6);
		for(i=1; i<obj->n_args; i++)
			{ BGBCC_JX2_EmitDWordRva32(sctx, tlbl[i]); }
		BGBCC_JX2_EmitDWordRva32(sctx, 0);
	}


	/* Build Per-Interface VTables */
	
//	viflbl
	nif=0;

	for(l=0; l<naclz; l++)
	{
		for(i=1; i<aclz[l]->n_args; i++)
		{
			cif=aclz[l]->args[i];
			li_sclz=BGBCC_CCXL_LookupStructureForSig(ctx, cif->sig);
			cclz=li_sclz->decl;

			k=nif++;
			l7=BGBCC_JX2_GenLabel(sctx);
			vif_lbl[k]=l7;
			vif_vmi[k]=cif;

			for(j=0; j<cclz->n_vargs; j++)
			{
				tlbl[j]=0;
			}
			
			for(j=0; j<cclz->n_regs; j++)
			{
				fi=cclz->regs[j];
				for(k=0; k<nvtix; k++)
				{
					fj=vt_vmi[k];
					if(!fj)continue;
					if(!strcmp(fi->name, fj->name))
						break;
				}
				
				if(k<nvtix)
					{ tlbl[fi->fxmoffs]=vt_lbl[k]; }
			}

			sprintf(tb, "%s/%s", cclz->qname, "__cinfo");
			k=BGBCC_JX2_GetNamedLabel(sctx, tb);
//			j=(-(cif->fxoffs))/ctx->arch_sizeof_ptr;
			if(ctx->arch_sizeof_ptr==8)
				j=(-(cif->fxoffs))>>3;
			else
				j=(-(cif->fxoffs))>>2;
			tlbl[0]=k;
//			tlbl[1]=CCXL_LBL_ABS16N|((-(cif->fxoffs))&0xFFFF);
//			tlbl[1]=CCXL_LBL_ABS16SP|(j&0xFFFF);
			tlbl[1]=CCXL_LBL_ABS16NP|(j&0xFFFF);

			BGBCC_JX2_EmitBAlign(sctx, 8);
			BGBCC_JX2_EmitLabel(sctx, l7);

			for(j=0; j<cclz->n_vargs; j++)
				{ BGBCC_JX2_EmitPtrWordAbs(sctx, tlbl[i]); }
			BGBCC_JX2_EmitPtrWordAbs(sctx, 0);
		}
	}


	/* Object Init Stub */
	BGBCC_JX2_SetSectionName(sctx, ".text");
//	BGBCC_JX2_EmitBAlign(sctx, 4);
	BGBCC_JX2_EmitBAlign(sctx, 8);
	BGBCC_JX2_EmitLabel(sctx, l1);
	
	if(ctx->arch_sizeof_ptr==8)
	{
		BGBCC_JX2_EmitLoadRegLabelRel24(sctx, BGBCC_SH_REG_R7, l0);
		BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
			BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R4, 0,
				BGBCC_SH_REG_R7);

		BGBCC_JX2C_EmitOpImmReg(ctx, sctx,
			BGBCC_SH_NMID_MOV, 0, BGBCC_SH_REG_R7);
		BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
			BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R4, 8,
				BGBCC_SH_REG_R7);
				
		for(i=0; i<nif; i++)
		{
//			k=vif_lbl[k];
//			fi=vif_vmi[k];
			k=vif_lbl[i];
			fi=vif_vmi[i];

			BGBCC_JX2_EmitLoadRegLabelRel24(sctx, BGBCC_SH_REG_R7, k);
			BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
				BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R4, fi->fxoffs,
					BGBCC_SH_REG_R7);
		}
	}else
	{
		BGBCC_JX2_EmitLoadRegLabelRel24(sctx, BGBCC_SH_REG_R7, l0);
		BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
			BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R4, 0,
				BGBCC_SH_REG_R7);

		BGBCC_JX2C_EmitOpImmReg(ctx, sctx,
			BGBCC_SH_NMID_MOV, 0, BGBCC_SH_REG_R7);
		BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
			BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R4, 4,
				BGBCC_SH_REG_R7);

		for(i=0; i<nif; i++)
		{
//			k=vif_lbl[k];
//			fi=vif_vmi[k];
			k=vif_lbl[i];
			fi=vif_vmi[i];

			BGBCC_JX2_EmitLoadRegLabelRel24(sctx, BGBCC_SH_REG_R7, k);
			BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
				BGBCC_SH_NMID_MOVL,
					BGBCC_SH_REG_R4, fi->fxoffs,
					BGBCC_SH_REG_R7);
		}
	}
	BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);
	
	return(0);
}


int BGBCC_JX2C_LookupLabelIndex(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int lblid)
{

	return(BGBCC_JX2_LookupLabelIndex(sctx, lblid));

#if 0
	int i, j, k;
	
	for(i=0; i<sctx->nlbl; i++)
	{
		if(sctx->lbl_id[i]==lblid)
			return(i);
	}
	return(-1);
#endif
}

int BGBCC_JX2C_LookupLabelImgOffs(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int lblid)
{
	int i, j, k;
	
	j=BGBCC_JX2C_LookupLabelIndex(ctx, sctx, lblid);
	k=sctx->sec_rva[sctx->lbl_sec[j]]+sctx->lbl_ofs[j];
	return(k);
}

int BGBCC_JX2C_LookupLabelImgVA(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int lblid)
{
	int i, j, k;
	
	
	j=BGBCC_JX2C_LookupLabelIndex(ctx, sctx, lblid);

	if(sctx->lbl_sec[j]==BGBCC_SH_CSEG_ABS)
		return(sctx->lbl_ofs[j]);

	k=sctx->sec_lva[sctx->lbl_sec[j]]+sctx->lbl_ofs[j];
	return(k);
}

int BGBCC_JX2C_LookupLabelImgMapTag(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int lblid)
{
	int i, j, k;
	
	
	j=BGBCC_JX2C_LookupLabelIndex(ctx, sctx, lblid);

	if(sctx->lbl_sec[j]==BGBCC_SH_CSEG_ABS)
		return('A');
	if(sctx->lbl_sec[j]==BGBCC_SH_CSEG_TEXT)
		return('T');
	if(sctx->lbl_sec[j]==BGBCC_SH_CSEG_BSS)
		return('B');
	if(sctx->lbl_sec[j]==BGBCC_SH_CSEG_DATA)
		return('D');
	if(sctx->lbl_sec[j]==BGBCC_SH_CSEG_RODATA)
		return('R');

	if(sctx->lbl_sec[j]==BGBCC_SH_CSEG_UTEXT)
		return('T');
	if(sctx->lbl_sec[j]==BGBCC_SH_CSEG_UDATA)
		return('D');

	return('D');
}


extern char *bgbcc_jx2_srcidx[256];
extern int bgbcc_jx2_nsrcidx;

void bgbcc_jx2cc_setu16en(byte *ct, int en, u16 v)
{
//	if(((v>>12)&15)==0xD)
//	{
//		BGBCC_DBGBREAK
//	}
	bgbcc_setu16en(ct, en, v);
}

void bgbcc_jx2cc_setu32en(byte *ct, int en, u32 v)
{
//	if(((v>>12)&15)==0xD)
//	{
//		BGBCC_DBGBREAK
//	}
	bgbcc_setu32en(ct, en, v);
}

ccxl_status BGBCC_JX2C_RelocRangeError(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int rlcix, int lblix, int rlcty, int disp)
{
	char *lbln;
	
	lbln=BGBCC_JX2_LookupNameForLabel(sctx, sctx->lbl_id[lblix]);
	if(!lbln)
		lbln="?";

	ctx->n_error++;

	printf("BGBCC_JX2C_RelocRangeError: Symbol Out of Range\n");
	printf("  rlc_sec=%d(%s) lbl_sec=%d(%s) rlcty=%02X(%s) "
			"disp=%08X name=%s(%08X)\n",
		sctx->rlc_sec[rlcix], sctx->sec_name[sctx->rlc_sec[rlcix]],
		sctx->lbl_sec[lblix], sctx->sec_name[sctx->lbl_sec[lblix]],
		rlcty, BGBCC_JX2DA_RelocToName(sctx, rlcty),
		disp,
		lbln,
		sctx->lbl_id[lblix]);

	printf("  rlc_ofs=%08X lbl_ofs=%08X disp=%d\n",
		sctx->rlc_ofs[rlcix],
		sctx->lbl_ofs[lblix],
		sctx->lbl_ofs[lblix]-sctx->rlc_ofs[rlcix]);
	return(0);
}

ccxl_status BGBCC_JX2C_ApplyImageRelocs(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	byte *imgbase)
{
	byte *ctl, *ctr, *gbr_base;
	char *s0;
	int en;
	s64 val, var;
	s64 b, d, b1, d1;
	int w0, w1, w2, w3;
	int lbl_istext;
	int i, j, k;

	en=(sctx->is_le==0);
	sctx->stat_ovlbl8=0;
	sctx->stat_rlbl_tot=0;
	
	gbr_base=imgbase+sctx->gbr_rva;

	for(i=0; i<sctx->nrlc; i++)
	{
		j=BGBCC_JX2C_LookupLabelIndex(ctx, sctx, sctx->rlc_id[i]);
		if(j<0)
		{
			k=sctx->rlc_id[i];
			s0=BGBCC_JX2_LookupNameForLabel(sctx, k);
			if(s0 && !strncmp(s0, "__rsrc__", 8))
			{
//				printf("BGBCC_JX2C_ApplyImageRelocs: "
//					"Ignore Missing Resource Label N='%s'\n", s0);
				continue;
			}else
				if(s0)
			{
				ctx->n_error++;
				printf("BGBCC_JX2C_ApplyImageRelocs: "
					"Missing Label N='%s'\n", s0);
			}else
			{
				ctx->n_error++;

				printf("BGBCC_JX2C_ApplyImageRelocs: "
					"Missing Label ID=%08X\n", k);
				if((k&(~65535))==CCXL_LBL_GENSYM2BASE)
				{
					b=k&65535;
					b1=sctx->genlabel_srcpos[b];
					printf("\t%s:%d (Backend)\n",
						bgbcc_jx2_srcidx[b1>>16], (u16)b1);
				}else if((k&(~65535))==CCXL_LBL_GENSYMBASE)
				{
//					printf("\tMiddle GenSym\n");
					b=k&65535;
					b1=ctx->gs_srcpos[b];
					printf("\t%s:%d (Middle)\n",
						ctx->fnidx_str[b1>>16], (u16)b1);
				}
			}
//			{ BGBCC_DBGBREAK }
			continue;
		}

		if(sctx->lbl_sec[j]==BGBCC_SH_CSEG_ABS)
		{
			ctl=NULL;
			val=sctx->lbl_ofs[j];
			lbl_istext=0;
		}else
		{
			ctl=imgbase+sctx->sec_rva[sctx->lbl_sec[j]]+sctx->lbl_ofs[j];
			val=sctx->sec_lva[sctx->lbl_sec[j]]+sctx->lbl_ofs[j];

			lbl_istext=
				(sctx->lbl_sec[j]==BGBCC_SH_CSEG_TEXT) ||
				(sctx->lbl_sec[j]==BGBCC_SH_CSEG_UTEXT) ;

			if((ctl<=imgbase) || (ctl>(imgbase+0x1000000)))
				{ BGBCC_DBGBREAK }
			if(
//					(sctx->lbl_ofs[j]<0) ||
					(sctx->lbl_ofs[j] > sctx->sec_lsz[sctx->lbl_sec[j]]))
				{ BGBCC_DBGBREAK }
		}

		ctr=imgbase+sctx->sec_rva[sctx->rlc_sec[i]]+sctx->rlc_ofs[i];
		var=sctx->sec_lva[sctx->rlc_sec[i]]+sctx->rlc_ofs[i];

		if((ctr<=imgbase) || (ctr>(imgbase+0x1000000)))
			{ BGBCC_DBGBREAK }

//		d=ctl-ctr;
		d=val-var;
		switch(sctx->rlc_ty[i])
		{
		case BGBCC_SH_RLC_RELW8:
		case BGBCC_SH_RLC_RELW12:
		case BGBCC_SH_RLC_RELW16_BJX:
		case BGBCC_SH_RLC_RELW20_BJX:
		case BGBCC_SH_RLC_RELW24_BJX:
		case BGBCC_SH_RLC_RELW8_BSR:
		case BGBCC_SH_RLC_RELW12_RVI:
		case BGBCC_SH_RLC_RELW20_RVI:
		case BGBCC_SH_RLC_RELW10_XG3:
		case BGBCC_SH_RLC_RELW23_XG3:
			d=val-(var&(~1));
			break;
		}

		sctx->stat_rlbl_tot++;
		if(bgbcc_abs(d)<256)
			{ sctx->stat_rlbl_disp8++; }
		else if(bgbcc_abs(d)<4096)
			{ sctx->stat_rlbl_disp12++; }
		else if(bgbcc_abs(d)<65536)
			{ sctx->stat_rlbl_disp16++; }
		else if(bgbcc_abs(d)<(1<<20))
			{ sctx->stat_rlbl_disp20++; }
		else if(bgbcc_abs(d)<(1<<24))
			{ sctx->stat_rlbl_disp24++; }
		else
			{ sctx->stat_rlbl_disp33++; }

		switch(sctx->rlc_ty[i])
		{
		case BGBCC_SH_RLC_REL8:
			b=bgbcc_gets8en(ctr, en);
			d1=b+(d-1);
			if(((sbyte)d1)!=d1)
				{ BGBCC_DBGBREAK }
			bgbcc_sets8en(ctr, en, d1);
			break;
		case BGBCC_SH_RLC_REL16:
			b=bgbcc_gets16en(ctr, en);
			d1=b+(d-2);
			if(((s16)d1)!=d1)
				{ BGBCC_DBGBREAK }
			bgbcc_sets16en(ctr, en, d1);
			break;
		case BGBCC_SH_RLC_REL32:
			b=bgbcc_gets32en(ctr, en);
			bgbcc_sets32en(ctr, en, b+(d-4));
			break;
		case BGBCC_SH_RLC_REL32B:
			b=bgbcc_gets32en(ctr, en);
			bgbcc_sets32en(ctr, en, b-(d-4));
			break;

		case BGBCC_SH_RLC_ABS32:
//			d1=0x0C000000+(ctl-imgbase);
			d1=sctx->image_base+(ctl-imgbase);
			b=bgbcc_gets32en(ctr, en);
			bgbcc_sets32en(ctr, en, b+d1);
			break;

		case BGBCC_SH_RLC_ABS64:
//			d1=0x0C000000+(ctl-imgbase);
			d1=sctx->image_base+(ctl-imgbase);
			b=bgbcc_gets64en(ctr, en);
			b1=b+d1;
			if(lbl_istext && (sctx->abi_spillpad&16))
			{
				b1&=0x0000FFFFFFFFFFFFULL;
				if(sctx->is_fixed32&2)
				{
					b1|=1ULL<<55;
					b1|=1;
				}else
				{
					b1|=1;
				}
			}
			bgbcc_sets64en(ctr, en, b1);
			break;

		case BGBCC_SH_RLC_ABS96:
			d1=sctx->image_base+(ctl-imgbase);
			b=bgbcc_gets64en(ctr, en);
			bgbcc_sets64en(ctr, en, b+d1);
			/* High bits isgonred here. */
			break;

		case BGBCC_SH_RLC_RVA32:
			d1=(ctl-imgbase);
			b=bgbcc_gets32en(ctr, en);
			bgbcc_sets32en(ctr, en, b+d1);
			break;

		case BGBCC_SH_RLC_RVA24:
			d1=(ctl-imgbase);
			b=bgbcc_gets32en(ctr, en);
			b1=	((b+d1)&0x00FFFFFFU) |
				( b    &0xFF000000U) ;
			bgbcc_sets32en(ctr, en, b1);
			break;

		case BGBCC_SH_RLC_RELW12:
			b=bgbcc_getu16en(ctr, en);
			b1=((s32)(b<<20))>>20;
//			d1=b1+(d-4);
			d1=b1+((d-4)>>1);
			if((((s32)(d1<<20))>>20)!=d1)
				{ BGBCC_DBGBREAK }
			bgbcc_jx2cc_setu16en(ctr, en, (b&0xF000)|(d1&0x0FFF));
			break;
		case BGBCC_SH_RLC_RELW8:
			b=bgbcc_getu16en(ctr, en);
			b1=((s32)(b<<24))>>24;
//			d1=b1+(d-4);
			d1=b1+((d-4)>>1);
			if((((s32)(d1<<24))>>24)!=d1)
				{ BGBCC_DBGBREAK }
			bgbcc_jx2cc_setu16en(ctr, en, (b&0xFF00)|(d1&0x00FF));
			break;
		case BGBCC_SH_RLC_RELW12L:
			b=bgbcc_getu16en(ctr, en);
			b1=((s32)(b<<20))>>20;
//			d1=b1+(d-4);
			d1=b1+((d-2)>>2);
//			if(((d1<<20)>>20)!=d1)
			if((d1&4095)!=d1)
				{ BGBCC_DBGBREAK }
			bgbcc_jx2cc_setu16en(ctr, en, (b&0xF000)|(d1&0x0FFF));
			break;
		case BGBCC_SH_RLC_RELW8L:
			b=bgbcc_getu16en(ctr, en);
			b1=((s32)(b<<24))>>24;
//			d1=b1+(d-4);
			d1=b1+((d-2)>>2);
//			if(((d1<<24)>>24)!=d1)
			if((d1&255)!=d1)
				{ BGBCC_DBGBREAK }
			bgbcc_jx2cc_setu16en(ctr, en, (b&0xFF00)|(d1&0x00FF));
			break;

		case BGBCC_SH_RLC_RELW16_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			
//			b=(w0&255)|((w1&255)<<8);
			b=((w0&255)<<8)|(w1&255);
			b1=(b<<16)>>16;

			d1=b1+((d-4)>>1);
			if((((s32)(d1<<16))>>16)!=d1)
				{ BGBCC_DBGBREAK }
				
			if(((((s32)(d1<<24))>>24)==d1) &&
				((w1&0xFF00)!=0x8300))
					sctx->stat_ovlbl8++;
				
			w0=(w0&0xFF00)|((d1>>8)&0x00FF);
			w1=(w1&0xFF00)|((d1   )&0x00FF);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_RELW20_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			
//			b=(w0&255)|((w1&255)<<8);
//			b=((w0&255)<<8)|(w1&255);
			b=((w0&255)<<12)|(w1&4095);
			b1=((s32)(b<<12))>>12;

			d1=b1+((d-4)>>1);
			if((((s32)(d1<<12))>>12)!=d1)
			{
				/* In XGPR and XG2, extend branch by 3 bits. */
				if((sctx->is_fixed32&2) && ((w0&0xEB00)==0xE000))
				{
					if((((s32)(d1<<9))>>9)!=d1)
						{ BGBCC_DBGBREAK }
				}else if((sctx->has_xgpr&1) && ((w0&0xFF00)==0xF000))
				{
					if((((s32)(d1<<9))>>9)!=d1)
						{ BGBCC_DBGBREAK }
				}else
					{ BGBCC_DBGBREAK }
			}

			if((((s32)(d1<<24))>>24)==d1)
				sctx->stat_ovlbl8++;
			
//			if((d1&1) && (sctx->is_fixed32&1))
			if((d1&1) && (sctx->is_fixed32&3))
				{ BGBCC_DBGBREAK }

			w0=(w0&0xFF00)|((d1>>12)&0x00FF);
			w1=(w1&0xF000)|((d1    )&0x0FFF);

			if((((s32)(d1<<12))>>12)!=d1)
			{
//				k=((d1>>20)^(d1>>24))&7;
//				k=((d1>>19)^(d1>>24))&7;
				k=((d1>>19)^(((s32)(d1<<12))>>31))&7;
				if((sctx->is_fixed32&2) && ((w0&0xEB00)==0xE000))
				{
					w0^=k<<13;
					if((d1>>24)!=(((s32)(d1<<12))>>31))
						{ w0^=0xE080; }
				}
				else if((sctx->has_xgpr&1) && ((w0&0xFF00)==0xF000))
				{
					w0&=0x00FF;
					w0|=0x7000;
					w0|=k<<8;
					if((d1>>24)!=(((s32)(d1<<12))>>31))
						{ w0^=0x0780; }
				}else
					{ BGBCC_DBGBREAK }
			}

			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_REL24_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			b=((w0&255)<<16)|(w1&65535);
			b1=((s32)(b<<8))>>8;
			d1=b1+(d-4);

			if((((s32)(d1<<24))>>24)==d1)
					sctx->stat_ovlbl8+=2;

			if((w0&0xFE00)==0xFA00)
			{
				if((((s32)(d1<<7))>>7)!=d1)
					{ BGBCC_DBGBREAK }
				w0=(w0&0xFE00)|((d1>>16)&0x01FF);
			}else
			{
				if((((s32)(d1<<8))>>8)!=d1)
					{ BGBCC_DBGBREAK }
				w0=(w0&0xFF00)|((d1>>16)&0x00FF);
			}
			w1=d1&0xFFFF;
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;
		case BGBCC_SH_RLC_REL24B_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			b=((w0&255)<<16)|(w1&65535);
			b1=((s32)(b<<8))>>8;
			d1=b1-(d-4);

			if((((s32)(d1<<24))>>24)==d1)
					sctx->stat_ovlbl8+=2;

			if((w0&0xFE00)==0xFA00)
			{
				if((((s32)(d1<<7))>>7)!=d1)
					{ BGBCC_DBGBREAK }
				w0=(w0&0xFE00)|((d1>>16)&0x01FF);
			}else
			{
				if((((s32)(d1<<8))>>8)!=d1)
					{ BGBCC_DBGBREAK }
				w0=(w0&0xFF00)|((d1>>16)&0x00FF);
			}
//			if((((s32)(d1<<8))>>8)!=d1)
//				{ BGBCC_DBGBREAK }
//			w0=(w0&0xFF00)|((d1>>16)&0x00FF);
			w1=d1&0xFFFF;
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_RELW24_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			b=((w0&255)<<16)|(w1&65535);
			b1=((s32)(b<<8))>>8;
			d1=b1+((d-4)>>1);

			if((((s32)(d1<<24))>>24)==d1)
					sctx->stat_ovlbl8+=2;

			if((w0&0xFE00)==0xFA00)
			{
				if((((s32)(d1<<7))>>7)!=d1)
					{ BGBCC_DBGBREAK }
				w0=(w0&0xFE00)|((d1>>16)&0x01FF);
			}else
			{
				if((((s32)(d1<<8))>>8)!=d1)
					{ BGBCC_DBGBREAK }
				w0=(w0&0xFF00)|((d1>>16)&0x00FF);
			}
			w1=d1&0xFFFF;
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_RELB13_OP24:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu8en(ctr+2, en);
			
			b=((w0&255)<<4)|(w1&15);
			if(w1&16)
				b|=(~0U)<<12;
			b1=(int)b;

			d1=b1+(d-3);
			if((((s32)(d1<<20))>>20)!=d1)
				{ BGBCC_DBGBREAK }
				
//			if(((((s32)(d1<<24))>>24)==d1) &&
//				((w1&0xFF00)!=0x8300))
//					sctx->stat_ovlbl8++;
				
			w0=(w0&0xFF00)|((d1>>4)&0x00FF);
			w1=(w1&0xF0)|((d1   )&0x0F)|((d1>>16)&0x10);
			bgbcc_setu16en(ctr+0, en, w0);
			bgbcc_setu8en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_PBO24_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			b=((w0&255)<<16)|(w1&65535);
			b1=((s32)(b<<8))>>8;
			d1=b1+(ctl-gbr_base);

			if(d1<0)
				{ BGBCC_DBGBREAK }
			

//			if(b1!=0)
//				{ BGBCC_DBGBREAK }

			if((w0&0xFE00)==0xFA00)
			{
				if((((s32)(d1<<7))>>7)!=d1)
					{ BGBCC_DBGBREAK }
				w0=(w0&0xFE00)|((d1>>16)&0x01FF);
			}else
			{
				BGBCC_DBGBREAK

				if((((s32)(d1<<8))>>8)!=d1)
					{ BGBCC_DBGBREAK }
				w0=(w0&0xFF00)|((d1>>16)&0x00FF);
			}
			w1=d1&0xFFFF;
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

#if 0
		case BGBCC_SH_RLC_PBOJ27_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			w3=bgbcc_getu16en(ctr+6, en);
			b=((w0&255)<<14)|(w1&16383);
			b=(b<<5)|((w3&0x01F0)>>4);
			b1=((s32)(b<<5))>>5;
			d1=b1+(ctl-gbr_base);

			if(d1<0)
				{ BGBCC_DBGBREAK }

			w0=(w0&0xFF00)|((d1>>19)&0x00FF);
			w1=(w1&0xC000)|((d1>> 5)&0x3FFF);
			w3=(w3&0xFE0F)|((d1<< 4)&0x01F0);

//			w1=d1&0xFFFF;
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			bgbcc_jx2cc_setu16en(ctr+6, en, w3);
			break;

		case BGBCC_SH_RLC_PBOJ31_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			w3=bgbcc_getu16en(ctr+6, en);
			b=((w0&255)<<14)|(w1&16383);
			b=(b<<9)|(w3&0x01FF);
			b1=((s32)(b<<1))>>1;
			d1=b1+(ctl-gbr_base);

			if(d1<0)
				{ BGBCC_DBGBREAK }

			w0=(w0&0xFF00)|((d1>>23)&0x00FF);
			w1=(w1&0xC000)|((d1>> 9)&0x3FFF);
			w3=(w3&0xFE00)|((d1    )&0x01FF);

//			w1=d1&0xFFFF;
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			bgbcc_jx2cc_setu16en(ctr+6, en, w3);
			break;

		case BGBCC_SH_RLC_RELJ27_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			w3=bgbcc_getu16en(ctr+6, en);
			b=((w0&255)<<14)|(w1&16383);
			b=(b<<5)|((w3&0x01F0)>>4);
			b1=((s32)(b<<5))>>5;
			d1=b1+(d-8);

			w0=(w0&0xFF00)|((d1>>19)&0x00FF);
			w1=(w1&0xC000)|((d1>> 5)&0x3FFF);
			w3=(w3&0xFE0F)|((d1<< 4)&0x01F0);

			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			bgbcc_jx2cc_setu16en(ctr+6, en, w3);
			break;

		case BGBCC_SH_RLC_RELJ31_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			w3=bgbcc_getu16en(ctr+6, en);
			b=((w0&255)<<14)|(w1&16383);
			b=(b<<9)|(w3&0x01FF);
			b1=((s32)(b<<1))>>1;
			d1=b1+(d-8);

			w0=(w0&0xFF00)|((d1>>23)&0x00FF);
			w1=(w1&0xC000)|((d1>> 9)&0x3FFF);
			w3=(w3&0xFE00)|((d1    )&0x01FF);

			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			bgbcc_jx2cc_setu16en(ctr+6, en, w3);
			break;
#endif

#if 1
		case BGBCC_SH_RLC_PBOJ29_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			w3=bgbcc_getu16en(ctr+6, en);

//			b=((w0&255)<<14)|(w1&16383);
//			b=(b<<5)|((w3&0x01F0)>>4);
			b=((w0&255)<<16)|(w1&65535);
			b=(b<<4)|((w3&0x00F0)>>4);
			if(w3&0x0100)
				b|=(~0UL)<<28;
//			b1=((s32)(b<<5))>>5;
			b1=(s32)b;
			d1=b1+(ctl-gbr_base);

			if(d1<0)
				{ BGBCC_DBGBREAK }

			w0=(w0&0xFF00)|((d1>>20)&0x00FF);
			w1=(w1&0x0000)|((d1>> 4)&0xFFFF);
			w3=(w3&0xFE0F)|((d1<< 4)&0x00F0)|((d1>>20)&0x0100);

//			w1=d1&0xFFFF;
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			bgbcc_jx2cc_setu16en(ctr+6, en, w3);
			break;

		case BGBCC_SH_RLC_PBOJ32_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			w3=bgbcc_getu16en(ctr+6, en);

//			b=((w0&255)<<14)|(w1&16383);
//			b=(b<<9)|(w3&0x01FF);
//			b1=((s32)(b<<1))>>1;

			b=((w0&255)<<16)|(w1&65535);
			b=(b<<8)|(w3&0x00FF);
			b1=(s32)b;

			d1=b1+(ctl-gbr_base);

			if(d1<0)
			{
				BGBCC_JX2C_RelocRangeError(ctx, sctx,
					i, j, sctx->rlc_ty[i], d1);
//				BGBCC_CCXL_Error(ctx, "Negative Displacement (PBOJ32)\n");
//				BGBCC_DBGBREAK
			}

			w0=(w0&0xFF00)|((d1>>24)&0x00FF);
			w1=(w1&0x0000)|((d1>> 8)&0xFFFF);
			w3=(w3&0xFE00)|((d1    )&0x00FF)|((d1>>31)&0x0100);

//			w1=d1&0xFFFF;
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			bgbcc_jx2cc_setu16en(ctr+6, en, w3);
			break;


		case BGBCC_SH_RLC_PBO9_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);

			b=(w1&0x01FF);
			b1=(s32)b;

			d1=b1+(ctl-gbr_base);

			if((d1<0) || (d1>=512))
			{
				BGBCC_JX2C_RelocRangeError(ctx, sctx,
					i, j, sctx->rlc_ty[i], d1);
//				BGBCC_DBGBREAK
			}

			w1=(w1&0xFE00)|(d1&0x01FF);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_PBOQ16_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);

			b=(w1&0xFFFF);
			b1=b<<3;

			d1=b1+(ctl-gbr_base);

			if((d1<0) || (d1>=524288) || (d1&7))
			{
				BGBCC_JX2C_RelocRangeError(ctx, sctx,
					i, j, sctx->rlc_ty[i], d1);
//				BGBCC_DBGBREAK
			}

			w1=(d1>>3)&0xFFFF;
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;
#endif


		case BGBCC_SH_RLC_RELJ29_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			w3=bgbcc_getu16en(ctr+6, en);
			b=((w0&255)<<16)|(w1&65535);
			b=(b<<4)|((w3&0x00F0)>>4);
			if(w3&0x0100)
				b|=(~0UL)<<28;
			b1=(s32)b;
//			d1=b1+(ctl-gbr_base);
			d1=b1+(d-4);

			w0=(w0&0xFF00)|((d1>>20)&0x00FF);
			w1=(w1&0x0000)|((d1>> 4)&0xFFFF);
			w3=(w3&0xFE0F)|((d1<< 4)&0x00F0)|((d1>>20)&0x0100);

			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			bgbcc_jx2cc_setu16en(ctr+6, en, w3);
			break;

		case BGBCC_SH_RLC_RELJ32_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			w3=bgbcc_getu16en(ctr+6, en);
			b=((w0&255)<<16)|(w1&65535);
			b=(b<<8)|((w3&0x00FF)>>0);
//			if(w3&0x0100)
//				b|=(~0UL)<<28;
			b1=(s32)b;
//			d1=b1+(ctl-gbr_base);
			d1=b1+(d-4);

			w0=(w0&0xFF00)|((d1>>24)&0x00FF);
			w1=(w1&0x0000)|((d1>> 8)&0xFFFF);
			w3=(w3&0xFE00)|((d1<< 0)&0x00FF)|((d1>>24)&0x0100);

			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			bgbcc_jx2cc_setu16en(ctr+6, en, w3);
			break;

		case BGBCC_SH_RLC_PBO32_RVI:
			w0=bgbcc_getu32en(ctr+0, en);
			w1=bgbcc_getu32en(ctr+4, en);

//			b1=((w1>>20)&0xFFF);
//			b1=((s32)(b1<<20))>>20;
			b1=((s32)w1)>>20;
			b1+=(s32)((w0>>12)<<12);

			d1=b1+(ctl-gbr_base);

			w2=((s32)(d1<<20))>>20;
			w3=(d1-w2)>>12;
			
//			w0=(w0&0x00000FFF)|(((d1+2048)>>12)<<12);
//			w1=(w1&0x000FFFFF)|(d1<<20);

			w0=(w0&0x00000FFF)|(w3<<12);
			w1=(w1&0x000FFFFF)|(w2<<20);

//			w1=(d1>>3)&0xFFFF;
			bgbcc_jx2cc_setu32en(ctr+0, en, w0);
			bgbcc_jx2cc_setu32en(ctr+4, en, w1);
			break;

		case BGBCC_SH_RLC_PBO32J_RVI:
			w0=bgbcc_getu32en(ctr+0, en);
			w1=bgbcc_getu32en(ctr+4, en);

//			b1=((s32)w1)>>20;
//			b1+=(s32)((w0>>12)<<12);
			b1=0;

			d1=b1+(ctl-gbr_base);

			w2=(d1&2047)|((d1>>31)<<11);
			w3=d1>>11;
			
//			w0=(w0&0x00000FFF)|(((d1+2048)>>12)<<12);
//			w1=(w1&0x000FFFFF)|(d1<<20);

			w0=(w0&0x0000707F)|
				(((w3>> 0)&2047)<<20)|
				(((w3>>11)&  31)<<15)|
				(((w3>>16)&  31)<< 7);
			w1=(w1&0x000FFFFF)|((w2&4095)<<20);

//			w1=(d1>>3)&0xFFFF;
			bgbcc_jx2cc_setu32en(ctr+0, en, w0);
			bgbcc_jx2cc_setu32en(ctr+4, en, w1);
			break;

		case BGBCC_SH_RLC_PBO32LD_RVI:
			w0=bgbcc_getu32en(ctr+0, en);
			w1=bgbcc_getu32en(ctr+8, en);

			b1=((s32)w1)>>20;
			b1+=(s32)((w0>>12)<<12);

			d1=b1+(ctl-gbr_base);

			w2=((s32)(d1<<20))>>20;
			w3=(d1-w2)>>12;

			w0=(w0&0x00000FFF)|(w3<<12);
			w1=(w1&0x000FFFFF)|(w2<<20);

			bgbcc_jx2cc_setu32en(ctr+0, en, w0);
			bgbcc_jx2cc_setu32en(ctr+8, en, w1);
			break;

		case BGBCC_SH_RLC_PBO32ST_RVI:
			w0=bgbcc_getu32en(ctr+0, en);
			w1=bgbcc_getu32en(ctr+8, en);

//			b1=((s32)w1)>>20;
//			b1+=(s32)((w0>>12)<<12);
			b1=0;

			d1=b1+(ctl-gbr_base);

			w2=((s32)(d1<<20))>>20;
			w3=(d1-w2)>>12;

			w0=(w0&0x00000FFF)|(w3<<12);
			w1=(w1&0x01FFF07F)|
				((w2&0xFE0)<<20)|
				((w2&0x01F)<< 7);

			bgbcc_jx2cc_setu32en(ctr+0, en, w0);
			bgbcc_jx2cc_setu32en(ctr+8, en, w1);
			break;

		case BGBCC_SH_RLC_PBO32STJ_RVI:
			w0=bgbcc_getu32en(ctr+0, en);
			w1=bgbcc_getu32en(ctr+4, en);

			b1=0;

			d1=b1+(ctl-gbr_base);

			w2=(d1&2047)|((d1>>31)<<11);
			w3=d1>>11;

			w0=(w0&0x0000707F)|
				(((w3>> 0)&2047)<<20)|
				(((w3>>11)&  31)<<15)|
				(((w3>>16)&  31)<< 7);
//			w1=(w1&0x000FFFFF)|(w2<<20);
			w1=(w1&0x01FFF07F)|
				((w2&0xFE0)<<20)|
				((w2&0x01F)<< 7);

			bgbcc_jx2cc_setu32en(ctr+0, en, w0);
			bgbcc_jx2cc_setu32en(ctr+4, en, w1);
			break;

		case BGBCC_SH_RLC_REL32UI_RVI:
			w0=bgbcc_getu32en(ctr+0, en);
			w1=bgbcc_getu32en(ctr+4, en);

			b1=((w1>>20)&0xFFF);
			b1=((s32)(b1<<20))>>20;
			b1+=(((s32)w0)>>12)<<12;

			d1=b1+d;

			w2=((s32)(d1<<20))>>20;
			w3=(d1-w2)>>12;

//			w0=(w0&0x00000FFF)|(((d1+2048)>>12)<<12);
//			w1=(w1&0x000FFFFF)|(d1<<20);

			w0=(w0&0x00000FFF)|(w3<<12);
			w1=(w1&0x000FFFFF)|(w2<<20);

//			w1=(d1>>3)&0xFFFF;
			bgbcc_jx2cc_setu32en(ctr+0, en, w0);
			bgbcc_jx2cc_setu32en(ctr+4, en, w1);
			break;

		case BGBCC_SH_RLC_TBR24_BJX:
			break;
		case BGBCC_SH_RLC_TBR12_BJX:
			break;

		case BGBCC_SH_RLC_RELW8_BSR:
			w0=bgbcc_getu16en(ctr, en);
			b1=((s32)(w0<<24))>>24;
//			d1=b1+(d-4);
			d1=b1+((d-2)>>1);
			if((((s32)(d1<<24))>>24)!=d1)
			{
				BGBCC_JX2C_RelocRangeError(ctx, sctx,
					i, j, sctx->rlc_ty[i], d1);
//				BGBCC_CCXL_Error(ctx, "Symbol Out of Range (RelW8)\n");
//				break;
//				{ BGBCC_DBGBREAK }
			}
			if((w0&0xFE00)==0x2400)
				w0=(w0&0xFE00)|(d1&0x01FF);
			else
				w0=(w0&0xFF00)|(d1&0x00FF);
//			if(((w0&0xFF00)==0x2400) && (d1&0x100))w0^=0x0100;
			bgbcc_jx2cc_setu16en(ctr, en, w0);
			break;

		case BGBCC_SH_RLC_RELW12_BSR:
			b=bgbcc_getu16en(ctr, en);
			b1=((s32)(b<<20))>>20;
			d1=b1+((d-2)>>1);
			if((((s32)(d1<<20))>>20)!=d1)
			{
				BGBCC_JX2C_RelocRangeError(ctx, sctx,
					i, j, sctx->rlc_ty[i], d1);
//				BGBCC_CCXL_Error(ctx, "Symbol Out of Range (RelW12)\n");
//				break;
//				{ BGBCC_DBGBREAK }
			}
			bgbcc_jx2cc_setu16en(ctr, en, (b&0xF000)|(d1&0x0FFF));
			break;

		case BGBCC_SH_RLC_RELW16_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			b=((w0&255)<<8)|(w1&255);
			b1=((s32)(b<<16))>>16;
			d1=b1+((d-4)>>1);
//			if((((s32)(d1<<16))>>16)!=d1)
//				{ BGBCC_DBGBREAK }
			if((((s32)(d1<<24))>>24)==d1)
					sctx->stat_ovlbl8++;
			if((w0&0xFE00)==0x2400)
			{
				if((((s32)(d1<<15))>>15)!=d1)
				{
					BGBCC_JX2C_RelocRangeError(ctx, sctx,
						i, j, sctx->rlc_ty[i], d1);
//					BGBCC_CCXL_Error(ctx, "Symbol Out of Range (RelW16)\n");
//					break;
//					{ BGBCC_DBGBREAK }
				}
				w0=(w0&0xFE00)|((d1>>8)&0x01FF);
				w1=(w1&0xFF00)|((d1   )&0x00FF);
			}
			else if((w1&0xFE00)==0x2400)
			{
				if((((s32)(d1<<15))>>15)!=d1)
					{ BGBCC_DBGBREAK }
				w0=(w0&0xFF00)|((d1>>8)&0x00FF);
				w1=(w1&0xFE00)|((d1   )&0x00FF)|
					((d1>>16)&0x0100);
			}
			else
			{
				if((((s32)(d1<<16))>>16)!=d1)
					{ BGBCC_DBGBREAK }
				w0=(w0&0xFF00)|((d1>>8)&0x00FF);
				w1=(w1&0xFF00)|((d1   )&0x00FF);
			}
//			if(((w0&0xFF00)==0x2400) && (d1&0x10000))w0^=0x0100;
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_RELW16C_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			b=((w0&255)<<8)|(w1&255);
			b1=((s32)(b<<16))>>16;
			d1=b1+(d-4);
//			if((((s32)(d1<<16))>>16)!=d1)
//				{ BGBCC_DBGBREAK }
			if((w0&0xFE00)==0x2400)
			{
				if((((s32)(d1<<15))>>15)!=d1)
				{
					BGBCC_JX2C_RelocRangeError(ctx, sctx,
						i, j, sctx->rlc_ty[i], d1);
//					BGBCC_CCXL_Error(ctx, "Symbol Out of Range (RelW16C)\n");
//					break;
//					{ BGBCC_DBGBREAK }
				}
				w0=(w0&0xFE00)|((d1>>8)&0x01FF);
				w1=(w1&0xFF00)|((d1   )&0x00FF);
			}
			else
				if((w1&0xFE00)==0x2400)
			{
				if((((s32)(d1<<15))>>15)!=d1)
					{ BGBCC_DBGBREAK }
				w0=(w0&0xFF00)|((d1>>8)&0x00FF);
				w1=(w1&0xFE00)|((d1   )&0x00FF)|
					((d1>>16)&0x0100);
			}
			else
			{
				if((((s32)(d1<<16))>>16)!=d1)
				{
					BGBCC_JX2C_RelocRangeError(ctx, sctx,
						i, j, sctx->rlc_ty[i], d1);
//					BGBCC_CCXL_Error(ctx, "Symbol Out of Range (RelW16C)\n");
//					break;
//					{ BGBCC_DBGBREAK }
				}
				w0=(w0&0xFF00)|((d1>>8)&0x00FF);
				w1=(w1&0xFF00)|((d1   )&0x00FF);
			}
//			if(((w0&0xFF00)==0x2400) && (d1&0x10000))w0^=0x0100;
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_RELW16D_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			b=((w0&4095)<<4)|(w1&15);
			b1=((s32)(b<<16))>>16;
			d1=b1+((d-4)>>1);
			if((((s32)(d1<<16))>>16)!=d1)
			{
				BGBCC_JX2C_RelocRangeError(ctx, sctx,
					i, j, sctx->rlc_ty[i], d1);
//				BGBCC_CCXL_Error(ctx, "Symbol Out of Range (RelW16D)\n");
//				break;
//				{ BGBCC_DBGBREAK }
			}
			if((((s32)(d1<<24))>>24)==d1)
					sctx->stat_ovlbl8++;
			if((w0&0xE000)==0xA000)
				w0=(w0&0xE000)|((d1>>4)&0x1FFF);
			else
				w0=(w0&0xF000)|((d1>>4)&0x0FFF);
			w1=(w1&0xFFF0)|((d1   )&0x000F);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_RELW16A_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			b=((w0&4095)<<4)|(w1&15);
			b1=((s32)(b<<16))>>16;
			d1=b1+(d-4);
			if((((s32)(d1<<15))>>15)!=d1)
			{
				BGBCC_JX2C_RelocRangeError(ctx, sctx,
					i, j, sctx->rlc_ty[i], d1);
//				BGBCC_CCXL_Error(ctx, "Symbol Out of Range (RelW16A)\n");
//				break;
//				{ BGBCC_DBGBREAK }
			}
			if((((s32)(d1<<24))>>24)==d1)
					sctx->stat_ovlbl8++;
			w0=(w0&0xE000)|((d1>>4)&0x1FFF);
			w1=(w1&0xFFF0)|((d1   )&0x000F);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_RELW24_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			b=((w0&255)<<16)|((w1&255)<<8)|(w2&255);
			b1=((s32)(b<<8))>>8;
			d1=b1+((d-6)>>1);
			if((((s32)(d1<<8))>>8)!=d1)
			{
				BGBCC_JX2C_RelocRangeError(ctx, sctx,
					i, j, sctx->rlc_ty[i], d1);
//				BGBCC_CCXL_Error(ctx, "Symbol Out of Range (RelW24)\n");
//				break;
//				{ BGBCC_DBGBREAK }
			}
			if((((s32)(d1<<24))>>24)==d1)
					sctx->stat_ovlbl8++;
			if((w0&0xFE00)==0x2400)
				w0=(w0&0xFE00)|((d1>>16)&0x01FF);
			else
				w0=(w0&0xFF00)|((d1>>16)&0x00FF);
			w1=(w1&0xFF00)|((d1>> 8)&0x00FF);
			w2=(w2&0xFF00)|((d1    )&0x00FF);
//			if(((w0&0xFF00)==0x2400) && (d1&0x1000000))w0^=0x0100;
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			break;

		case BGBCC_SH_RLC_RELW24C_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			b=((w0&255)<<16)|((w1&255)<<8)|(w2&255);
			b1=((s32)(b<<8))>>8;
			d1=b1+(d-6);
			if((((s32)(d1<<8))>>8)!=d1)
			{
				BGBCC_JX2C_RelocRangeError(ctx, sctx,
					i, j, sctx->rlc_ty[i], d1);
//				BGBCC_CCXL_Error(ctx, "Symbol Out of Range (RelW24C)\n");
//				break;
//				{ BGBCC_DBGBREAK }
			}
			if((w0&0xFE00)==0x2400)
				w0=(w0&0xFE00)|((d1>>16)&0x01FF);
			else
				w0=(w0&0xFF00)|((d1>>16)&0x00FF);
			w1=(w1&0xFF00)|((d1>> 8)&0x00FF);
			w2=(w2&0xFF00)|((d1    )&0x00FF);
//			if(((w0&0xFF00)==0x2400) && (d1&0x1000000))w0^=0x0100;
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			break;
		case BGBCC_SH_RLC_RELW32C_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			w3=bgbcc_getu16en(ctr+6, en);
			b=((w0&255)<<24)|((w1&255)<<16)|((w2&255)<<8)|(w3&255);
			b1=(s32)b;
			d1=b1+(d-8);
			if(((s32)d1)!=d1)
				{ BGBCC_DBGBREAK }
			if((w0&0xFE00)==0x2400)
				w0=(w0&0xFE00)|((d1>>32)&0x01FF);
			else
				w0=(w0&0xFF00)|((d1>>32)&0x00FF);
			w1=(w1&0xFF00)|((d1>>16)&0x00FF);
			w2=(w2&0xFF00)|((d1>> 8)&0x00FF);
			w3=(w3&0xFF00)|((d1    )&0x00FF);
//			if(((w0&0xFF00)==0x2400) && (d1&0x1000000))w0^=0x0100;
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			bgbcc_jx2cc_setu16en(ctr+6, en, w3);
			break;

		case BGBCC_SH_RLC_RELW24D_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			b=((w0&4095)<<12)|((w1&255)<<4)|(w2&15);
			b1=((s32)(b<<8))>>8;
			d1=b1+((d-6)>>1);
			if((((s32)(d1<<8))>>8)!=d1)
			{
				BGBCC_JX2C_RelocRangeError(ctx, sctx,
					i, j, sctx->rlc_ty[i], d1);
//				break;
//				{ BGBCC_DBGBREAK }
			}
			if((((s32)(d1<<24))>>24)==d1)
					sctx->stat_ovlbl8++;
			w0=(w0&0xE000)|((d1>>12)&0x1FFF);
			w1=(w1&0xFF00)|((d1>> 4)&0x00FF);
			w2=(w2&0xFFF0)|((d1    )&0x000F);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			break;
		case BGBCC_SH_RLC_RELW32D_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			w3=bgbcc_getu16en(ctr+6, en);
			b=((w0&4095)<<20)|((w1&255)<<12)|((w2&255)<<4)|(w3&15);
//			b1=((s32)(b<<8))>>8;
			b1=(s32)b;
			d1=b1+((d-8)>>1);
//			if((((s32)(d1<<8))>>8)!=d1)
//				{ BGBCC_DBGBREAK }
			if((((s32)(d1<<24))>>24)==d1)
					sctx->stat_ovlbl8++;
			w0=(w0&0xE000)|((d1>>20)&0x1FFF);
			w1=(w1&0xFF00)|((d1>>12)&0x00FF);
			w2=(w2&0xFF00)|((d1>> 4)&0x00FF);
			w3=(w3&0xFFF0)|((d1    )&0x000F);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			bgbcc_jx2cc_setu16en(ctr+6, en, w3);
			break;

		case BGBCC_SH_RLC_RELW24A_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			b=((w0&4095)<<12)|((w1&255)<<4)|(w2&15);
			b1=((s32)(b<<8))>>8;
			d1=b1+(d-6);
			if((((s32)(d1<<8))>>8)!=d1)
				{ BGBCC_DBGBREAK }
			if((((s32)(d1<<24))>>24)==d1)
					sctx->stat_ovlbl8++;
			w0=(w0&0xE000)|((d1>>12)&0x1FFF);
			w1=(w1&0xFF00)|((d1>> 4)&0x00FF);
			w2=(w2&0xFFF0)|((d1    )&0x000F);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			break;
		case BGBCC_SH_RLC_RELW32A_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			w3=bgbcc_getu16en(ctr+6, en);
			b=((w0&4095)<<20)|((w1&255)<<12)|((w2&255)<<4)|(w3&15);
//			b1=((s32)(b<<8))>>8;
			b1=(s32)b;
			d1=b1+(d-8);
//			if((((s32)(d1<<8))>>8)!=d1)
//				{ BGBCC_DBGBREAK }
			if((((s32)(d1<<24))>>24)==d1)
					sctx->stat_ovlbl8++;
			w0=(w0&0xE000)|((d1>>20)&0x1FFF);
			w1=(w1&0xFF00)|((d1>>12)&0x00FF);
			w1=(w1&0xFF00)|((d1>> 4)&0x00FF);
			w2=(w2&0xFFF0)|((d1    )&0x000F);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			bgbcc_jx2cc_setu16en(ctr+6, en, w2);
			break;

		case BGBCC_SH_RLC_ABSW16A_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			b=((w0&255)<<8)|(w1&255);
//			b1=sctx->image_base+(ctl-imgbase);
			b1=val;
			d1=b+b1;
			if((d1&0xFFFF)!=d1)
				{ BGBCC_DBGBREAK }
			w0=(w0&0xFF00)|((d1>>8)&0x00FF);
			w1=(w1&0xFF00)|((d1   )&0x00FF);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_ABSW24A_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			b=((w0&255)<<16)|((w1&255)<<8)|(w2&255);
//			b1=sctx->image_base+(ctl-imgbase);
			b1=val;
			d1=b+b1;
			if((d1&0xFFFFFF)!=d1)
				{ BGBCC_DBGBREAK }
			w0=(w0&0xFF00)|((d1>>16)&0x00FF);
			w1=(w1&0xFF00)|((d1>> 8)&0x00FF);
			w2=(w2&0xFF00)|((d1    )&0x00FF);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			break;

		case BGBCC_SH_RLC_ABSW32A_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			w3=bgbcc_getu16en(ctr+6, en);
			b=((w0&255)<<24)|((w1&255)<<16)|((w2&255)<<8)|(w3&255);
//			b1=sctx->image_base+(ctl-imgbase);
			b1=val;
			d1=b+b1;
			w0=(w0&0xFF00)|((d1>>24)&0x00FF);
			w1=(w1&0xFF00)|((d1>>16)&0x00FF);
			w2=(w2&0xFF00)|((d1>> 8)&0x00FF);
			w3=(w3&0xFF00)|((d1    )&0x00FF);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			bgbcc_jx2cc_setu16en(ctr+6, en, w3);
			break;

		case BGBCC_SH_RLC_ABSW16B_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			b=((w0&4095)<<4)|(w1&15);
//			b1=sctx->image_base+(ctl-imgbase);
			b1=val;
			d1=b+b1;
			if((d1&0xFFFF)!=d1)
				{ BGBCC_DBGBREAK }
			w0=(w0&0xF000)|((d1>>4)&0x0FFF);
			w1=(w1&0xFFF0)|((d1   )&0x000F);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;
		case BGBCC_SH_RLC_ABSW24B_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			b=((w0&4095)<<12)|((w1&255)<<4)|(w2&15);
//			b1=sctx->image_base+(ctl-imgbase);
			b1=val;
			d1=b+b1;
			if((d1&0xFFFFFF)!=d1)
				{ BGBCC_DBGBREAK }
			w0=(w0&0xF000)|((d1>>12)&0x0FFF);
			w1=(w1&0xFF00)|((d1>> 4)&0x00FF);
			w2=(w2&0xFFF0)|((d1    )&0x000F);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			break;
		case BGBCC_SH_RLC_ABSW32B_BSR:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			w3=bgbcc_getu16en(ctr+6, en);
			b=((w0&4095)<<20)|((w1&255)<<12)|((w2&255)<<4)|(w3&15);
//			b1=sctx->image_base+(ctl-imgbase);
			b1=val;
			d1=b+b1;
//			if((d1&0xFFFFFF)!=d1)
//				{ BGBCC_DBGBREAK }
			w0=(w0&0xF000)|((d1>>20)&0x0FFF);
			w1=(w1&0xFF00)|((d1>>12)&0x00FF);
			w2=(w2&0xFF00)|((d1>> 4)&0x00FF);
			w3=(w3&0xFFF0)|((d1    )&0x000F);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			bgbcc_jx2cc_setu16en(ctr+6, en, w3);
			break;

		case BGBCC_SH_RLC_TRIPWIRE_BJX:
			break;

		case BGBCC_SH_RLC_ABS48_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			w3=bgbcc_getu16en(ctr+6, en);
			b=	(((u64)(w0&  255))<<40)|
				(((u64)(w1&65535))<<24)|
				(((u64)(w2&  255))<<16)|
				 ((u64)(w3&65535));

			b1=sctx->image_base+(ctl-imgbase);
			d1=b+b1;

			w0=(w0&0xFF00)|((d1>>40)&0x00FF);
			w1=(w1&0x0000)|((d1>>24)&0xFFFF);
			w2=(w2&0xFF00)|((d1>>16)&0x00FF);
			w3=(w3&0x0000)|((d1    )&0xFFFF);

			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			bgbcc_jx2cc_setu16en(ctr+6, en, w3);
			break;


		case BGBCC_SH_RLC_RELW20_RVI:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			
			b=	((w0&0xF000)<< 0) |
				((w1&0x000F)<<16) |
				((w1&0x0010)<< 7) |
				((w1&0x7FE0)>> 4) |
				((w1&0x8000)<< 4) ;
			b1=((s32)(b<<12))>>12;
			
//			b=((w0&255)<<12)|(w1&4095);
//			b1=((s32)(b<<12))>>12;

//			d1=b1+((d-4)>>1);
//			d1=b1+(d-4);
			d1=b1+d;
//			if((((s32)(d1<<12))>>12)!=d1)
			if((((s32)(d1<<11))>>11)!=d1)
				{ BGBCC_DBGBREAK }

//			if((((s32)(d1<<24))>>24)==d1)
//				sctx->stat_ovlbl8++;

			w0=(w0&0x0FFF)|(d1&0xF000);
			w1=	((d1>>16)&0x000F) |
				((d1>> 7)&0x0010) |
				((d1<< 4)&0x7FE0) |
//				((d1>> 4)&0x8000) ;
				((d1>> 5)&0x8000) ;

//			w0=(w0&0xFF00)|((d1>>12)&0x00FF);
//			w1=(w1&0xF000)|((d1    )&0x0FFF);
			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_RELW12_RVI:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			
			b=	((w0&0x0F00)>> 7) |
				((w0&0x0080)<< 4) |
				((w1&0x7E00)>> 4) |
				((w1&0x8000)>> 3) ;
			b1=((s32)(b<<19))>>19;

//			d1=b1+(d-4);
			d1=b1+(d);
			if((((s32)(d1<<19))>>19)!=d1)
				{ BGBCC_DBGBREAK }

			w0=	(w0&0xF07F) |
				((d1<< 7)&0x0F00) |
				((d1>> 4)&0x0080) ;
			w1=	(w1&0x01FF) |
				((d1<< 4)&0x7E00) |
				((d1<< 3)&0x8000) ;

			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_RELW33J_RVI:
			w0=bgbcc_getu32en(ctr+0, en);
			w1=bgbcc_getu32en(ctr+4, en);
			
			b=	((w1&0x00000F00)>> 7) |
				((w1&0x00000080)<< 4) |
				((w1&0x7E000000)>>20) |
				((w1&0x80000000)>>19) ;
			b1=((s32)(b<<19))>>19;

//			d1=b1+(d-4);
			d1=b1+(d);
//			if((((s32)(d1<<19))>>19)!=d1)
//				{ BGBCC_DBGBREAK }

			w2=(d1&0xFFE)|((d1>>31)<<12);
			w3=d1>>12;

			w0=(w0&0x0000707F) |
				(((w3>> 0)&2047)<<20) |
				(((w3>>11)&  31)<<15) |
				(((w3>>16)&  31)<< 7) ;
			w1=	(w1&0x01FFF07F) |
				((w2<< 7)&0x00000F00) |
				((w2>> 4)&0x00000080) |
				((w2<<20)&0x7E000000) |
				((w2<<19)&0x80000000) ;

			bgbcc_jx2cc_setu32en(ctr+0, en, w0);
			bgbcc_jx2cc_setu32en(ctr+4, en, w1);
			break;

		case BGBCC_SH_RLC_RELW8_BJCMP:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);

			b1=((s32)(w1<<24))>>24;
			d1=b1+((d-4)>>1);
			if((((s32)(d1<<24))>>24)!=d1)
			{
				BGBCC_JX2C_RelocRangeError(ctx, sctx,
					i, j, sctx->rlc_ty[i], d1);
//				BGBCC_CCXL_Error(ctx, "Symbol Out of Range (RelW8)\n");
				break;
			}
			w1=(w1&0xFF00)|(d1&0x00FF);

			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_RELW32_BJCMP:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			w3=bgbcc_getu16en(ctr+6, en);

			b=((w0&255)<<16)|(w1&65535);
			b=(b<<8)|(w3&0x00FF);
			b1=(s32)b;

			d1=b1+((d-8)>>1);

			w0=(w0&0xFF00)|((d1>>24)&0x00FF);
			w1=(w1&0x0000)|((d1>> 8)&0xFFFF);
			w3=(w3&0xFF00)|((d1    )&0x00FF);
			
			if(
				((w2&0x0B00)==0x0200) &&
				(((w3>>12)&15)==0xE))
			{
				w3&=~0x0100;
				if(d1<0)
					w3|=0x0100;
			}

			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			bgbcc_jx2cc_setu16en(ctr+6, en, w3);
			break;


		case BGBCC_SH_RLC_RELW11_BJCMP:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);

//			b1=((s32)(w1<<21))>>21;

			b1=(w1&0x03FF);
			if(w0&1)
				b1|=~0x03FF;
			
			if(sctx->is_fixed32&2)
			{
				if(!(w0&0x2000))
					b1^=0x400;
				if(!(w0&0x4000))
					b1^=0x800;
			}

			d1=b1+((d-4)>>1);

			if((((s32)(d1<<21))>>21)!=d1)
			{
				/* In XGPR and XG2, extend branch by 2 bits. */
				k=0;
				if((sctx->is_fixed32&2) && ((w0&0x0B00)==0x0200))
//				if(sctx->is_fixed32&2)
				{
//					if((w0&0xEB00)!=0xE200)
//						BGBCC_CCXL_Error(ctx, "Symbol Range 11 Opw=%04X\n", w0);
				
					if((((s32)(d1<<19))>>19)!=d1)
						{ k=1; }
				}else if((sctx->has_xgpr&1) && ((w0&0xFF00)==0xF200))
				{
					if((((s32)(d1<<19))>>19)!=d1)
						{ k=1; }
				}else
					{ k=1; }

				if(k)
				{
					BGBCC_JX2C_RelocRangeError(ctx, sctx,
						i, j, sctx->rlc_ty[i], d1);
	//				BGBCC_CCXL_Error(ctx, "Symbol Out of Range (RelW11)\n");
					break;
				}
			}
			w0=(w0&0xFFFE)|((d1>>16)&0x0001);
			w1=(w1&0xFC00)|(d1&0x03FF);

			if((((s32)(d1<<21))>>21)!=d1)
			{
				k=((d1>>10)^(d1>>24))&3;
//				if((sctx->is_fixed32&2) && ((w0&0xEB00)==0xE200))
				if((sctx->is_fixed32&2) && ((w0&0x0B00)==0x0200))
				{
					w0^=k<<13;
				}
				else if((sctx->has_xgpr&1) && ((w0&0xFF00)==0xF000))
				{
					w0&=0x00FF;
					w0|=0x9000;
					w0|=k<<8;
				}
			}

			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_RELW33_BJCMP:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			w2=bgbcc_getu16en(ctr+4, en);
			w3=bgbcc_getu16en(ctr+6, en);

			b=((w0&255)<<16)|(w1&65535);
			b=(b<<8)|(w3&0x00FF);
			b1=(s32)b;

			d1=b1+((d-8)>>1);

			w0=(w0&0xFF00)|((d1>>24)&0x00FF);
			w1=(w1&0x0000)|((d1>> 8)&0xFFFF);
			w3=(w3&0xFE00)|((d1    )&0x00FF)|((d1>>23)&0x0100);

			bgbcc_jx2cc_setu16en(ctr+0, en, w0);
			bgbcc_jx2cc_setu16en(ctr+2, en, w1);
			bgbcc_jx2cc_setu16en(ctr+4, en, w2);
			bgbcc_jx2cc_setu16en(ctr+6, en, w3);
			break;



		case BGBCC_SH_RLC_RELW23_XG3:
			w0=bgbcc_getu32en(ctr+0, en);
			
			b=	(((w0>>16)&0xFFFF)<< 0) |
				(((w0>> 6)&    63)<<16) |
				(((w0>> 5)&     1)<<22) ;
			b1=((s32)(b<<9))>>9;
//			b1<<=2;

			if(d&3)
				{ BGBCC_DBGBREAK }
			
			d1=b1+(d>>2);
			if((((s32)(d1<<9))>>9)!=d1)
				{ BGBCC_DBGBREAK }

			w0=(w0&0x0000F03FU)|
				((d1&0xFFFF)<<16) |
				(((d1>>16)&63)<<6) |
				(((d1>>31)& 1)<<5) ;

			bgbcc_jx2cc_setu32en(ctr+0, en, w0);
			break;

		case BGBCC_SH_RLC_RELW10_XG3:
			w0=bgbcc_getu32en(ctr+0, en);
			
			b=(w0>>22);
			b1=((s32)(b<<22))>>22;

			if(d&3)
				{ BGBCC_DBGBREAK }

			d1=b1+(d>>2);
			if((((s32)(d1<<22))>>22)!=d1)
				{ BGBCC_DBGBREAK }

			w0=(w0&0x003FFFFFU)|((d1&1023)<<22);

			bgbcc_jx2cc_setu32en(ctr+0, en, w0);
			break;

		case BGBCC_SH_RLC_RELW33_XG3:
			w0=bgbcc_getu32en(ctr+0, en);
			w1=bgbcc_getu32en(ctr+4, en);
			
			b1=	(((w0>>16)&0xFFFF)<< 8) |
				(((w0>> 8)&0x00FF)<<24) |
				(((w1>>22)&0x00FF)<< 0) |
				(((w1>>30)&   1LL)<<32) ;
//			b1<<=2;

			if(d&3)
				{ BGBCC_DBGBREAK }
			
			d1=b1+(d>>2);

			w0=(w0&0x000000FFU)|
				(((d1>> 8)&0xFFFF)<<16) |
				(((d1>>24)&0x00FF)<< 8);
			w1=(w1&0x803FFFFFU)|
				(((d1>> 0)&0x00FF)<<22) |
				(((d1>>32)&0x0001)<<30);

			bgbcc_jx2cc_setu32en(ctr+0, en, w0);
			bgbcc_jx2cc_setu32en(ctr+4, en, w1);
			break;

		case BGBCC_SH_RLC_RELB33_XG3:
			w0=bgbcc_getu32en(ctr+0, en);
			w1=bgbcc_getu32en(ctr+4, en);
			
			b1=	(((w0>>16)&0xFFFF)<< 8) |
				(((w0>> 8)&0x00FF)<<24) |
				(((w1>>22)&0x00FF)<< 0) |
				(((w1>>30)&   1LL)<<32) ;
			
			d1=b1+d;

			w0=(w0&0x000000FFU)|
				(((d1>> 8)&0xFFFF)<<16) |
				(((d1>>24)&0x00FF)<< 8);
			w1=(w1&0x803FFFFFU)|
				(((d1>> 0)&0x00FF)<<22) |
				(((d1>>32)&0x0001)<<30);

			bgbcc_jx2cc_setu32en(ctr+0, en, w0);
			bgbcc_jx2cc_setu32en(ctr+4, en, w1);
			break;

		case BGBCC_SH_RLC_PBOJ33_XG3:
			w0=bgbcc_getu32en(ctr+0, en);
			w1=bgbcc_getu32en(ctr+4, en);

			b1=	(((w0>>16)&0xFFFF)<< 8) |
				(((w0>> 8)&0x00FF)<<24) |
				(((w1>>22)&0x00FF)<< 0) |
				(((w1>>30)&   1LL)<<32) ;
			
//			d1=b1+d;
			d1=b1+(ctl-gbr_base);

			w0=(w0&0x000000FFU)|
				(((d1>> 8)&0xFFFF)<<16) |
				(((d1>>24)&0x00FF)<< 8);
			w1=(w1&0x803FFFFFU)|
				(((d1>> 0)&0x00FF)<<22) |
				(((d1>>32)&0x0001)<<30);

			bgbcc_jx2cc_setu32en(ctr+0, en, w0);
			bgbcc_jx2cc_setu32en(ctr+4, en, w1);
			break;


		default:
			BGBCC_CCXL_StubError(ctx);
//			BGBCC_DBGBREAK
			break;
//			{ BGBCC_DBGBREAK }
		}
	}
	
	if(sctx->stat_ovlbl8>0)
	{
		if(sctx->verbose)
		{
			printf("Overlong Branches %d, EstCost=%dB\n",
				sctx->stat_ovlbl8, sctx->stat_ovlbl8*2);
		}
	}

	if(sctx->stat_rlbl_tot>0)
	{
		printf("Rlc-Lbl Disp-Hit 8s=%.2f%% 12s=%.2f%% 16s=%.2f%% "
			"20s=%.2f%% 24s=%.2f%% 33s=%.2f%%  tot=%d\n",
			(100.0*sctx->stat_rlbl_disp8 )/(sctx->stat_rlbl_tot),
			(100.0*sctx->stat_rlbl_disp12)/(sctx->stat_rlbl_tot),
			(100.0*sctx->stat_rlbl_disp16)/(sctx->stat_rlbl_tot),
			(100.0*sctx->stat_rlbl_disp20)/(sctx->stat_rlbl_tot),
			(100.0*sctx->stat_rlbl_disp24)/(sctx->stat_rlbl_tot),
			(100.0*sctx->stat_rlbl_disp33)/(sctx->stat_rlbl_tot),
			sctx->stat_rlbl_tot);
	}

	return(0);
}

extern byte bgbcc_dumpast;

ccxl_status BGBCC_JX2C_CheckTlsInit(BGBCC_TransState *ctx)
{
	BGBCC_JX2_Context *sctx;
	int lbl;

	sctx=ctx->uctx;

	if(sctx->tlsi_lbl)
		return(0);

	lbl=BGBCC_JX2_GetNamedLabel(sctx, "__tls_globalindex");
	BGBCC_JX2_EmitCommSym(sctx, lbl, 8, 8);
	sctx->tlsi_lbl=lbl;


	lbl=BGBCC_JX2_GetNamedLabel(sctx, "__tls_directory");

	BGBCC_JX2_SetSectionName(sctx, ".tls");

	BGBCC_JX2_EmitLabel(sctx, lbl);

	BGBCC_JX2_EmitDWordI(sctx, 0);		//00, vaRawDataStart
	BGBCC_JX2_EmitDWordI(sctx, 0);		//04, vaRawDataEnd
	BGBCC_JX2_EmitRelocTy(sctx, sctx->tlsi_lbl, BGBCC_SH_RLC_RVA32);
	BGBCC_JX2_EmitDWordI(sctx, 0);		//08, vaAddressOfIndex
	BGBCC_JX2_EmitDWordI(sctx, 0);		//0C, vaAddressCallbacks
	BGBCC_JX2_EmitDWordI(sctx, 0);		//10, szSizeOfZeroFill
	BGBCC_JX2_EmitDWordI(sctx, 0);		//14, mCharacteristics
	
	return(0);
}

ccxl_status BGBCC_JX2C_CheckRWadInit(BGBCC_TransState *ctx)
{
	BGBCC_JX2_Context *sctx;
	int i, j, k, n, lbl, lbld, lblt;

	sctx=ctx->uctx;

	if(!sctx->rwad_dir)
	{
		sctx->rwad_dir=bgbcc_malloc(1024*sizeof(BGBCC_CMG_RWadEntry));
		sctx->rwad_ndirent=0;
		sctx->rwad_mdirent=1024;

		lbl=BGBCC_JX2_GetNamedLabel(sctx, "__rsrc_start");
		lbld=BGBCC_JX2_GetNamedLabel(sctx, "__rsrc_dir");
		lblt=BGBCC_JX2_GetNamedLabel(sctx, "__rsrc_types");
		
		sctx->rwad_lblhead=lbl;
		sctx->rwad_lbldir=lbld;
		sctx->rwad_lbltype=lblt;

		BGBCC_JX2_SetSectionName(sctx, ".rsrc");
//		BGBCC_JX2_EmitBAlign(ctx, 8);

		BGBCC_JX2_EmitLabel(sctx, lbl);

		BGBCC_JX2_EmitDWordI(sctx, 0);		//00, flags
		BGBCC_JX2_EmitDWordI(sctx, 0);		//04, date/time
		BGBCC_JX2_EmitDWordI(sctx, BGBCC_IMGFMT_RWAD);		//08, magic
		BGBCC_JX2_EmitDWordI(sctx, 0);		//0C, magic2
		
		BGBCC_JX2_EmitRelocTy(sctx, lbld, BGBCC_SH_RLC_RVA32);
		BGBCC_JX2_EmitDWordI(sctx, 0);		//10, RVA of Directory
		BGBCC_JX2_EmitDWordI(sctx, 0);		//14, Size of Directory

		BGBCC_JX2_EmitRelocTy(sctx, lblt, BGBCC_SH_RLC_RVA32);
		BGBCC_JX2_EmitDWordI(sctx, 0);		//18, RVA of Types
		BGBCC_JX2_EmitDWordI(sctx, 0);		//1C, Size of Types

		BGBCC_JX2_EmitRelocTy(sctx, lbl, BGBCC_SH_RLC_RVA32);
		BGBCC_JX2_EmitDWordI(sctx, 0);		//20, RVA of Header
		BGBCC_JX2_EmitDWordI(sctx, 0x40);	//24, Size of Directory

		BGBCC_JX2_EmitDWordI(sctx, 0);		//28, -
		BGBCC_JX2_EmitDWordI(sctx, 0);		//2C, -
		BGBCC_JX2_EmitDWordI(sctx, 0);		//30, -
		BGBCC_JX2_EmitDWordI(sctx, 0);		//34, -
		BGBCC_JX2_EmitDWordI(sctx, 0);		//38, -
		BGBCC_JX2_EmitDWordI(sctx, 0);		//3C, -
	}
	
	return(0);
}


ccxl_status BGBCC_JX2C_AddRWadLump(BGBCC_TransState *ctx,
	char *name, byte *buf, int csz, int dsz, int cmp, int ety)
{
	char tname[32];
	BGBCC_JX2_Context *sctx;
	BGBCC_CMG_RWadEntry *ent;
	int i, j, k, n, lbl, lbld, lblt, csz1;
	
	sctx=ctx->uctx;

	BGBCC_JX2C_CheckRWadInit(ctx);
		
	if((sctx->rwad_ndirent+1)>=sctx->rwad_mdirent)
	{
		i=sctx->rwad_mdirent; i=i+(i>>1);
		sctx->rwad_dir=bgbcc_realloc(sctx->rwad_dir,
			i*sizeof(BGBCC_CMG_RWadEntry));
		sctx->rwad_mdirent=i;
	}
	
	n=sctx->rwad_ndirent++;

	ent=sctx->rwad_dir+n;
	memset(ent, 0, sizeof(BGBCC_CMG_RWadEntry));
	
	strncpy(ent->name, name, 16);
	
	sprintf(tname, "__rsrc__%s", ent->name);
	lbl=BGBCC_JX2_GetNamedLabel(sctx, tname);
	
	csz1=(csz+7)&(~7);

	BGBCC_JX2_SetSectionName(sctx, ".rsrc");
	BGBCC_JX2_EmitBAlign(sctx, 8);
	BGBCC_JX2_EmitLabel(sctx, lbl);
	BGBCC_JX2_EmitRawBytes(sctx, buf, csz1);
	
	ent->lbl=lbl;
	ent->csize=csz;
	ent->dsize=dsz;
	ent->cmp=cmp;
	ent->ety=ety;

	return(0);
}

int BGBCC_JX2C_ResourceTypeForFourcc(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	fourcc fmt)
{
	int i;

	if(!fmt)
		return(0);
	if(fmt==BGBCC_FMT_LUMP)
		return(0);

	for(i=0x20; i<0x80; i++)
	{
		if(sctx->rwad_types[i]==fmt)
			return(i);
		if(!sctx->rwad_types[i])
		{
			sctx->rwad_types[i]=fmt;
			return(i);
		}
	}
	
	return(0);
}

ccxl_status BGBCC_JX2C_AddResourceData(BGBCC_TransState *ctx,
	char *name, byte *buf, int sz, fourcc imgfmt)
{
	BGBCC_JX2_Context *sctx;
	wad2head_t head;
	wad2lump_t *dir, *ent;
	byte *buf1;
	u32 tyfcc[256];
	char *tn;
	int i, j, k, n, ty, sz1;
	
	sctx=ctx->uctx;

#if 0
	if(imgfmt==BGBCC_FMT_BMP)
	{
		if((buf[0]=='B') && (buf[1]=='M'))
		{
			/* Hack: BMP variant where stuff is correctly aligned. */
			buf1=malloc(sz+16);
			memcpy(buf1+2, buf, sz);
			memcpy(buf1, " BMP", 4);
			j=exwad_getu32(buf1+0x0C);
			k=j+2;
			k=(k+7)&(~7);
			memcpy(buf1+k, buf+j, sz-j);
			exwad_setu32(buf1+0x0C, k);
			exwad_setu32(buf1+0x04, exwad_getu32(buf1+0x04)+(k-j));
			sz1=sz+(k-j);
			k=BGBCC_JX2C_AddResourceData(ctx, name, buf1, sz1, imgfmt);
			free(buf1);
			return(k);
		}
	}
#endif

	if(	(imgfmt==BGBCC_FMT_LUMP) ||
		(imgfmt==BGBCC_FMT_WAV) ||
		(imgfmt==BGBCC_FMT_BMP) ||
		(imgfmt==BGBCC_FMT_AVI) ||
		(imgfmt==BGBCC_FMT_QOIF) ||
		(imgfmt==BGBCC_FMT_LCIF) )
	{
		ty=BGBCC_JX2C_ResourceTypeForFourcc(ctx, sctx, imgfmt);
		tn=BGBCP_BaseNameForNameLC(name);
		BGBCC_JX2C_AddRWadLump(ctx, tn,
			buf, sz, sz, 0, ty);
		return(0);
	}
	
	if(imgfmt==BGBCC_FMT_WAD)
	{
		memcpy(&head, buf+0, sizeof(wad2head_t));
		
		if(head.magic!=BGBCC_FMT_WAD2)
		{
			return(-1);
		}
		
		memset(tyfcc, 0, 256*4);
		k=head.typeoffs;
		if((k>16) && (k<sz))
			{ memcpy(tyfcc+0x20, buf+head.typeoffs, (256-32)*4); }
		
		n=head.numlumps;
		dir=bgbcc_malloc(n*sizeof(wad2lump_t));
		memcpy(dir, buf+head.diroffs, n*sizeof(wad2lump_t));
		
		for(i=0; i<n; i++)
		{
			ent=dir+i;
			ty=BGBCC_JX2C_ResourceTypeForFourcc(ctx, sctx, tyfcc[ent->ety]);
			BGBCC_JX2C_AddRWadLump(ctx, ent->name,
				buf+ent->fileoffs, ent->csize, ent->dsize, ent->cmp, ent->ety);
		}
		
		bgbcc_free(dir);
		return(0);
	}

	return(-1);
}

ccxl_status BGBCC_JX2C_FlattenImage(BGBCC_TransState *ctx,
	byte *obuf, int *rosz, fourcc imgfmt)
{
	char tb[256];
	BGBCC_JX2_Context *sctx;
	BGBCC_CCXL_RegisterInfo *obj, *obj1, *obj2;
	BGBCC_CCXL_LiteralInfo *litobj;
	char *s0, *s1;
	double f, g;
	int *shufarr, *shufarr_rev;
	int t0, t1, t2, t3;
	int l0;
	int shufgbl, sz, al, sz1, al1, sz2, al2, tsz, nswap;
	u64 h;
	u32 addr;
	int i, j, k, l, n;

	sctx=ctx->uctx;
	
	sctx->stat_tot_dq0=0;
	sctx->stat_tot_dq1=0;
	sctx->stat_tot_dqi=0;

	if(bgbcc_dumpast)
	{
		if(!sctx->cgen_log)
			sctx->cgen_log=fopen("bgbcc_shxlog.txt", "wt");
	}

	sctx->do_asm=0;
	if(imgfmt==BGBCC_IMGFMT_ASM)
	{
		sctx->do_asm=1;
//		sctx->is_pbo=0;
	}

	if(bgbcc_dumpast)
	{
		sctx->do_asm=1;
	}

	sctx->is_rom=0;
	if(imgfmt==BGBCC_IMGFMT_ROM)
	{
		sctx->is_pbo=0;		//PBO is N/A for ROM images.
		if(sctx->emit_riscv&0x11)
			sctx->is_pbo=3;		//Except in the case of RISC-V...

		sctx->is_rom=1;
		sctx->lbl_rom_data_strt=
			BGBCC_JX2_GetNamedLabel(sctx, "__rom_data_start");
		sctx->lbl_rom_data_end=
			BGBCC_JX2_GetNamedLabel(sctx, "__rom_data_end");

		if(!sctx->use_memmdl)
		{
			sctx->use_memmdl=BGBCC_MEMMDL_TINY16;
//			sctx->use_memmdl=BGBCC_MEMMDL_MEDIUM24;
		}
	}else
	{
		if(!sctx->use_memmdl)
		{
			sctx->use_memmdl=BGBCC_MEMMDL_MEDIUM32;
//			sctx->use_memmdl=BGBCC_MEMMDL_MEDIUM24;
		}
	}

	sctx->is_dll=0;
	sctx->is_exe=0;
	if(imgfmt==BGBCC_IMGFMT_DLL)
		{ sctx->is_dll=1; }
	if(imgfmt==BGBCC_IMGFMT_EXE)
		{ sctx->is_exe=1; }

	BGBCC_JX2_SetSectionName(sctx, ".text");
	BGBCC_JX2_EmitNamedLabel(sctx, "__text_start");

	BGBCC_JX2_SetSectionName(sctx, ".data");
	BGBCC_JX2_EmitNamedLabel(sctx, "__data_start");

//	if(sctx->is_pbo)
	if(1)
	{
		/*
		 * Emit pointer to self at the start of ".data"
		 * This self-pointer needs to be at the start of the PBO data area.
		 */
	
		BGBCC_JX2_EmitNamedLabel(sctx, "__global_ptr");
		j=BGBCC_JX2_GetNamedLabel(sctx, "__global_ptr");
		BGBCC_JX2_EmitQWordAbs64(sctx, j);
//		BGBCC_JX2_EmitQWord(sctx, 0);

		sctx->lbl_gbl_ptr=j;

//		/* Reserve space for pointers to other images. */
//		for(i=0; i<127; i++)
//		{
//			BGBCC_JX2_EmitQWord(sctx, 0);
//		}
	}

	BGBCC_JX2_SetSectionName(sctx, ".bss");
	BGBCC_JX2_EmitNamedLabel(sctx, "__bss_start");

//	BGBCC_JX2_SetSectionName(sctx, ".rsrc");
//	BGBCC_JX2_EmitNamedLabel(sctx, "__rsrc_start");

	BGBCC_JX2_SetSectionName(sctx, ".utext");
	BGBCC_JX2_EmitNamedLabel(sctx, "__utext_start");

	BGBCC_JX2_SetSectionName(sctx, ".udata");
	BGBCC_JX2_EmitNamedLabel(sctx, "__udata_start");

	sctx->lbl_got=BGBCC_JX2_GenLabel(sctx);

	for(i=0; i<ctx->n_literals; i++)
	{
		litobj=ctx->literals[i];
		BGBCC_CCXL_FixupObjSize(ctx, litobj, 0);
	}

	for(i=0; i<ctx->n_literals; i++)
	{
		litobj=ctx->literals[i];
		BGBCC_CCXL_FixupObjSize(ctx, litobj, 1);
	}

	for(i=0; i<ctx->n_literals; i++)
	{
		litobj=ctx->literals[i];
		BGBCC_CCXL_SanityObjSize(ctx, litobj, 1);
	}

	h=clock();
	
	t0=clock();
	t1=t0;
	while(t1<(t0+(0.1*CLOCKS_PER_SEC)))
	{
		t2=clock();
		t3=t2-t1;
		t1=t2;
		h=(h*65521)+t1;
	}

	for(i=0; i<ctx->n_reg_globals; i++)
	{
		obj=ctx->reg_globals[i];
		if(!obj)
			continue;

		if((obj->flagsint&BGBCC_TYFL_IFARCH) && obj->ifarchstr)
		{
			if(BGBCC_JX2A_ParseCheckFeatureList(sctx, obj->ifarchstr)==0)
			{
				obj->regflags|=BGBCC_REGFL_CULL;
				obj->regflags|=BGBCC_REGFL_IFACULL;
			}
		}

		if((obj->flagsint&BGBCC_TYFL_IFNARCH) && obj->ifarchstr)
		{
			if(BGBCC_JX2A_ParseCheckFeatureList(sctx, obj->ifarchstr)!=0)
			{
				obj->regflags|=BGBCC_REGFL_CULL;
				obj->regflags|=BGBCC_REGFL_IFACULL;
			}
		}

	}

	for(i=0; i<ctx->n_reg_globals; i++)
	{
//		j=shufarr[i];
//		obj=ctx->reg_globals[j];

		obj=ctx->reg_globals[i];
		if(!obj)
			continue;

#if 0
		if(obj->regtype==CCXL_LITID_GLOBALVAR)
		{
			if(obj->flagsint&BGBCC_TYFL_REGISTER)
			{
				obj->gblrefcnt+=10000;
			}
			
//			j=(obj->fxnsize)>>4;
			j=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->type);
			if(j<=0)
				j=9999999;
			j=j>>4;

			while(j)
			{
				obj->gblrefcnt>>=1;
				j>>=1;
			}
		}
#endif
		
//		if(obj->flagsint&BGBCC_TYFL_DLLEXPORT)
		if(	(obj->flagsint&BGBCC_TYFL_DLLEXPORT) ||
			(obj->flagsint&BGBCC_TYFL_NOCULL))
		{
			BGBCC_CCXL_GlobalMarkReachable(ctx, obj);
			continue;
		}

//		if(obj->regtype==CCXL_LITID_FUNCTION)
		if(1)
		{
			if(obj->regflags&BGBCC_REGFL_ACCESSED)
				BGBCC_CCXL_GlobalMarkReachable(ctx, obj);
//			continue;
		}

		if(obj->name)
		{
#if 0
			if(!strncmp(obj->name, "__", 2))
			{
#if 0
				if(!(ctx->ccxl_tyc_seen&BGBCC_TYCSEEN_VARIANT))
				{
					if(!strncmp(obj->name, "__lva_", 6))
						continue;
				}

				if(!(ctx->ccxl_tyc_seen&BGBCC_TYCSEEN_INT128))
				{
					if(!strncmp(obj->name, "__xli_", 6))
						continue;
				}
#else
				if(!strncmp(obj->name, "__lva_", 6))
					continue;
				if(!strncmp(obj->name, "__xli_", 6))
					continue;
#endif

#if 0
				if(!(ctx->ccxl_tyc_seen&BGBCC_TYCSEEN_FLOAT128))
				{
					if(!strncmp(obj->name, "__xlf_", 6))
						continue;
				}
#else
				if(!strncmp(obj->name, "__xlf_", 6))
					continue;
#endif

				if(!strncmp(obj->name, "__xbi_", 6))
					continue;

				if(!strncmp(obj->name, "__vnf_", 6))
					continue;
				if(!strncmp(obj->name, "__vqf_", 6))
					continue;

				if(!strncmp(obj->name, "__c2f_", 6))
					continue;
				if(!strncmp(obj->name, "__c2d_", 6))
					continue;

				if(!strncmp(obj->name, "__v3fq_", 7))
					continue;
				if(!strncmp(obj->name, "__v3fx_", 7))
					continue;
				if(!strncmp(obj->name, "__v4h_", 6))
					continue;
				if(!strncmp(obj->name, "__m64_", 6))
					continue;
				if(!strncmp(obj->name, "__m128_", 7))
					continue;

				if(!strncmp(obj->name, "__operator_", 11))
					continue;

				if(!(ctx->ccxl_tyc_seen&BGBCC_TYCSEEN_BCD))
				{
					if(!strncmp(obj->name, "__bcd64_", 8))
						continue;
					if(!strncmp(obj->name, "__bcd128_", 9))
						continue;
				}

				if(!(ctx->ccxl_tyc_seen&BGBCC_TYCSEEN_FLOAT_FPU))
				{
					if(!strncmp(obj->name, "__sfp_", 6))
						continue;
					if(!strncmp(obj->name, "__lfp_", 6))
						continue;
				}
				
//				if(!(sctx->fpu_soft) && !(sctx->fpu_lite))
				if(1)
				{
					if(!strncmp(obj->name, "__sfp_", 6))
						continue;
				}

				if(!(sctx->fpu_lite))
				{
					if(!strncmp(obj->name, "__lfp_", 6))
						continue;
				}

				if(!(ctx->ccxl_tyc_seen&BGBCC_TYCSEEN_IDIV_VAR))
				{
					if(!strcmp(obj->name, "__udivsi3"))
						continue;
					if(!strcmp(obj->name, "__sdivsi3"))
						continue;
				}
				
				if(!(ctx->ccxl_tyc_seen&BGBCC_TYCSEEN_IMOD_VAR))
				{
					if(!strcmp(obj->name, "__umodsi3"))
						continue;
					if(!strcmp(obj->name, "__smodsi3"))
						continue;
				}

				if(sctx->has_qmul&1)
				{
					if(!strcmp(obj->name, "__udivsi3"))
						continue;
					if(!strcmp(obj->name, "__sdivsi3"))
						continue;

					if(!strcmp(obj->name, "__umodsi3"))
						continue;
					if(!strcmp(obj->name, "__smodsi3"))
						continue;
				}

				if(!strcmp(obj->name, "__datetime"))
					continue;
				if(!strcmp(obj->name, "__ioctl"))
					continue;
				if(!strncmp(obj->name, "__alloca", 8))
					continue;

				if(1)
				{
					/* called by frontend, no free pass. */
					if(!strncmp(obj->name, "__lvo_", 6))
						continue;
				}

				if(!strcmp(obj->name, "__start"))
				{
					obj->regflags|=BGBCC_REGFL_ALIASPTR;
				}
				
				BGBCC_CCXL_GlobalMarkReachable(ctx, obj);
				continue;
			}
#endif

//			if(!strcmp(obj->name, "main"))
			if(!strcmp(obj->name, "main") ||
				!strcmp(obj->name, "_start") ||
				!strcmp(obj->name, "__start") ||
				!strcmp(obj->name, "__isr_interrupt") ||
				!strcmp(obj->name, "__isr_tlbfault") ||
				!strcmp(obj->name, "__isr_syscall"))
			{
				obj->regflags|=BGBCC_REGFL_ALIASPTR;
				BGBCC_CCXL_GlobalMarkReachable(ctx, obj);
				continue;
			}
		}
	}

#if 1
	for(k=0; k<4; k++)
	{
		for(i=0; i<ctx->n_reg_globals; i++)
		{
			obj=ctx->reg_globals[i];
			if(!obj)
				continue;
			
			if(obj->flagsint&BGBCC_TYFL_DLLEXPORT)
			{
				BGBCC_CCXL_GlobalMarkReachable(ctx, obj);
				continue;
			}

	//		if(obj->regtype==CCXL_LITID_FUNCTION)
			if(1)
			{
				if(obj->regflags&BGBCC_REGFL_ACCESSED)
					BGBCC_CCXL_GlobalMarkReachable(ctx, obj);
	//			continue;
			}
		}
	}
#endif

	for(i=0; i<ctx->n_reg_globals; i++)
	{
//		j=shufarr[i];
//		obj=ctx->reg_globals[j];

		obj=ctx->reg_globals[i];
		if(!obj)
			continue;
		
		if(!obj->gblrefcnt_org)
			obj->gblrefcnt_org=obj->gblrefcnt;

		if(obj->regtype==CCXL_LITID_GLOBALVAR)
		{
			if(obj->flagsint&BGBCC_TYFL_REGISTER)
			{
				obj->gblrefcnt+=10000;
			}
			
//			j=(obj->fxnsize)>>4;
			j=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->type);
			if(j<=0)
				j=9999999;
			j=j>>4;

			while(j>1)
			{
				obj->gblrefcnt>>=1;
				j>>=1;
			}
		}

		if(obj->regtype==CCXL_LITID_FUNCTION)
		{
			obj->gblrefcnt>>=2;
			
			j=(obj->n_vop)>>6;
			while(j>1)
			{
				obj->gblrefcnt>>=1;
				j>>=1;
			}
		}
	}


	shufgbl=ctx->n_reg_globals;
	shufarr=malloc(shufgbl*sizeof(int));
	shufarr_rev=malloc(shufgbl*sizeof(int));
	for(i=0; i<shufgbl; i++)
	{
		shufarr[i]=i;
		shufarr_rev[i]=-1;
	}

	if(sctx->do_shuffle)
	{
#if 1
		for(i=1; i<shufgbl; i++)
		{
			j=(((int)(h>>32))&0x0FFFFFFF)%(shufgbl);
			h=h*65521+13;
			while(!j)
			{
				j=(((int)(h>>32))&0x0FFFFFFF)%(shufgbl);
				h=h*65521+13;
			}
			k=shufarr[i];
			shufarr[i]=shufarr[j];
			shufarr[j]=k;
		}
#endif

		/* Sort globals by use-count. */
		for(i=1; i<shufgbl; i++)
		{
			for(j=i+1; j<shufgbl; j++)
			{
				obj1=ctx->reg_globals[shufarr[i]];
				obj2=ctx->reg_globals[shufarr[j]];
				
				f=1.25;
				if(	(obj1->regtype==CCXL_LITID_GLOBALVAR) &&
					(obj2->regtype==CCXL_LITID_GLOBALVAR) )
						f=1.0625;
				if(	(obj1->regtype==CCXL_LITID_FUNCTION) &&
					(obj2->regtype==CCXL_LITID_FUNCTION) )
						f=1.5;
				
				if((obj2->gblrefcnt>obj1->gblrefcnt) &&
//					(((obj2->gblrefcnt+1.0)/(obj1->gblrefcnt+1.0))>1.25))
//					(((obj2->gblrefcnt+1.0)/(obj1->gblrefcnt+1.0))>1.125))
					(((obj2->gblrefcnt+1.0)/(obj1->gblrefcnt+1.0))>f))
				{
					k=shufarr[i];
					shufarr[i]=shufarr[j];
					shufarr[j]=k;
				}
			}
		}
	}else
	{
#if 1
		/* Sort globals by use-count. */
		for(i=1; i<shufgbl; i++)
		{
			for(j=i+1; j<shufgbl; j++)
			{
				obj1=ctx->reg_globals[shufarr[i]];
				obj2=ctx->reg_globals[shufarr[j]];
				if(obj2->gblrefcnt>obj1->gblrefcnt)
				{
					k=shufarr[i];
					shufarr[i]=shufarr[j];
					shufarr[j]=k;
				}
			}
		}
#endif
	}

#if 1
	/* Shuffle global variables for space compacting. */
	for(i=0; i<512; i++)
	{
		tsz=0; nswap=0;
		for(j=1; j<(shufgbl-1); j++)
		{
			obj1=ctx->reg_globals[shufarr[j+0]];
			obj2=ctx->reg_globals[shufarr[j+1]];
			if(	(obj1->regtype==CCXL_LITID_GLOBALVAR) ||
				(obj1->regtype==CCXL_LITID_STATICVAR) )
			{
				if(	(obj2->regtype==CCXL_LITID_GLOBALVAR) ||
					(obj2->regtype==CCXL_LITID_STATICVAR) )
				{
					sz1=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj1->type);
					al1=BGBCC_CCXL_TypeGetLogicalAlign(ctx, obj1->type);
					sz2=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj2->type);
					al2=BGBCC_CCXL_TypeGetLogicalAlign(ctx, obj2->type);
					if(al1<1)al1=1;
					if(al2<1)al2=1;
					
//					if((tsz&(al1-1)) && (sz2<sz1))
					if(	( (tsz&(al1-1)) && (al2<al1)) ||
						(!(tsz&(al2-1)) && (al2>sz1)) ||
						((sz1>16) && (sz2<=16)))
					{
						k=shufarr[j+0];
						shufarr[j+0]=shufarr[j+1];
						shufarr[j+1]=k;
						nswap++;
						
						tsz=(tsz+(al2-1))&(~(al2-1));
						tsz+=sz2;
					}else
					{
						tsz=(tsz+(al1-1))&(~(al1-1));
						tsz+=sz1;
					}
				}else
				{
					sz1=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj1->type);
					al1=BGBCC_CCXL_TypeGetLogicalAlign(ctx, obj1->type);

					tsz=(tsz+(al1-1))&(~(al1-1));
					tsz+=sz1;
				}
			}else
			{
				if(	(obj2->regtype==CCXL_LITID_GLOBALVAR) ||
					(obj2->regtype==CCXL_LITID_STATICVAR) )
				{
					k=shufarr[j+0];
					shufarr[j+0]=shufarr[j+1];
					shufarr[j+1]=k;
					nswap++;
					
					if(j>1)
						j--;
					continue;
				}
			}
		}
		
		if(!nswap)
			break;
	}
#endif

	for(i=0; i<shufgbl; i++)
	{
		j=shufarr[i];
		shufarr_rev[j]=i;
	}

	for(i=0; i<shufgbl; i++)
	{
		j=shufarr_rev[i];
		if(j<0)
			{ BGBCC_DBGBREAK }
	}

	for(i=0; i<ctx->n_reg_globals; i++)
	{
//		j=shufarr[i];
//		obj=ctx->reg_globals[j];

		obj=ctx->reg_globals[i];
		if(!obj)
			continue;

		if(obj->regtype==CCXL_LITID_FUNCTION)
		{
			if(!(obj->regflags&BGBCC_REGFL_ACCESSED))
				{ obj->regflags|=BGBCC_REGFL_CULL; }
			continue;
		}

#if 1
		if(obj->regtype==CCXL_LITID_ASMBLOB)
		{
			if(!(obj->regflags&BGBCC_REGFL_ACCESSED))
				{ obj->regflags|=BGBCC_REGFL_CULL; }
			continue;
		}
#endif

		if(obj->regtype==CCXL_LITID_GLOBALVAR)
		{
			if(!(obj->regflags&BGBCC_REGFL_ACCESSED))
				{ obj->regflags|=BGBCC_REGFL_CULL; }
			continue;
		}

		if(obj->regtype==CCXL_LITID_STATICVAR)
		{
			if(!(obj->regflags&BGBCC_REGFL_ACCESSED))
				{ obj->regflags|=BGBCC_REGFL_CULL; }
			continue;
		}
	}

	sctx->need_n16bsr=1;
	sctx->need_n20bsr=1;
	sctx->need_n23bsr=0;
	sctx->need_n24bsr=0;

	sctx->need_n16dat=1;
	sctx->need_n20dat=1;
	sctx->need_n24dat=0;

#if 1
	for(i=0; i<ctx->n_reg_globals; i++)
	{
		obj=ctx->reg_globals[i];

		if(!obj)
			continue;

		if(obj->regtype==CCXL_LITID_FUNCTION)
		{
			if(!obj->gblsetmask)
			{
				k=((ctx->n_reg_globals+63)/64)*8;
				obj->gblsetmask=bgbcc_malloc(k);
				memset(obj->gblsetmask, 0, k);
			}
		}
	}

	/* Propagate some information along call graph */
	sctx->is_simpass=64;
	for(j=0; j<16; j++)
//	for(j=0; j<32; j++)
//	for(j=0; j<64; j++)
	{
		for(i=0; i<ctx->n_reg_globals; i++)
		{
			obj=ctx->reg_globals[i];

			if(!obj)
				continue;

			if(obj->regtype==CCXL_LITID_FUNCTION)
			{
				if(obj->regflags&
//					(BGBCC_REGFL_REC_GBLSTORE|
					(BGBCC_REGFL_REC_FTGBLSTORE|
						BGBCC_REGFL_REC_NOGBLSTORE))
				{
					continue;
				}
			
				if(obj->vtr)
				{
					BGBCC_JX2C_SetupFrameLayout(ctx, sctx, obj);
				}else
				{
					BGBCC_JX2C_InvalidateGlobalSetMask(ctx, sctx, obj);
					obj->regflags|=BGBCC_REGFL_REC_GBLSTORE;
					obj->regflags|=BGBCC_REGFL_REC_FTGBLSTORE;
				}
				continue;
			}
		}
	}

	for(i=0; i<ctx->n_reg_globals; i++)
	{
		obj=ctx->reg_globals[i];

		if(!obj)
			continue;

		if(obj->regtype==CCXL_LITID_FUNCTION)
		{
			if(obj->vtr)
			{
				BGBCC_JX2C_SetupFrameLayout(ctx, sctx, obj);
			}

			if(obj->regflags&
				(BGBCC_REGFL_REC_GBLSTORE|
					BGBCC_REGFL_REC_NOGBLSTORE))
			{
				continue;
			}

			/* If couldn't find any global stores... */

#if 1
			 /* Assume they exist... */
			obj->regflags|=BGBCC_REGFL_REC_GBLSTORE;
#endif

#if 0
			 /* Assume they don't exist... */
			obj->regflags|=BGBCC_REGFL_REC_NOGBLSTORE;
#endif
		}
	}

#endif


	BGBCC_JX2_SetBeginSimPass(sctx);
	sctx->is_simpass=64;

	for(i=0; i<ctx->n_reg_globals; i++)
	{
		if(i<shufgbl)
			j=shufarr[i];
		else
			j=i;
		obj=ctx->reg_globals[j];

//		obj=ctx->reg_globals[i];
		if(!obj)
			continue;

		if((obj->regtype==CCXL_LITID_FUNCTION) && (obj->vtr))
//		if(obj->regtype==CCXL_LITID_FUNCTION)
		{
			BGBCC_JX2C_BuildFunction(ctx, obj);
			continue;
		}

		if((obj->regtype==CCXL_LITID_FUNCTION) && (!obj->vtr))
		{
			l0=obj->fxoffs;
			if(l0<=0)
			{
				l0=BGBCC_JX2_GetNamedLabel(sctx, obj->qname);
				obj->fxoffs=l0;
				BGBCC_JX2_MarkLabelIsText(sctx, l0);
			}
		}

		if(obj->regtype==CCXL_LITID_GLOBALVAR)
		{
			BGBCC_JX2C_BuildGlobal(ctx, obj);
			continue;
		}

		if(obj->regtype==CCXL_LITID_STATICVAR)
		{
			BGBCC_JX2C_BuildGlobal(ctx, obj);
			continue;
		}

#if 0
		if(obj->regtype==CCXL_LITID_ASMBLOB)
		{
			BGBCC_JX2C_BuildAsmBlob(ctx, obj);
			continue;
		}

		if((obj->regtype==CCXL_LITID_STRUCT) ||
			(obj->regtype==CCXL_LITID_CLASS))
		{
			BGBCC_JX2C_BuildStruct(ctx, obj);
			continue;
		}
#endif
	}

	BGBCC_JX2_SetSectionName(sctx, ".text");
	BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
	BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
	BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
	
	BGBCC_JX2_SetSectionName(sctx, ".text");
	k=BGBCC_JX2_EmitGetOffs(sctx);
	j=k;
	sctx->sim_txtsz=k;

	BGBCC_JX2_SetSectionName(sctx, ".data");
	k=BGBCC_JX2_EmitGetOffs(sctx);
	j+=k;
	sctx->sim_datsz=k;

	BGBCC_JX2_SetSectionName(sctx, ".bss");
	k=BGBCC_JX2_EmitGetOffs(sctx);
	j+=k;
	sctx->sim_bsssz=k;

	BGBCC_JX2_SetSectionName(sctx, ".rodata");
	k=BGBCC_JX2_EmitGetOffs(sctx);
	j+=k;
	sctx->sim_datsz+=k;

	sctx->simimgsz=j;

	BGBCC_JX2_SetEndSimPass(sctx);
//	sctx->nlbl=0;
//	sctx->nvlbl=0;
	sctx->nvlbl=sctx->nlbl;

	if(sctx->verbose)
	{
		printf("Sim .text %d\n", sctx->sim_txtsz);
		printf("Sim .data %d\n", sctx->sim_datsz);
		printf("Sim .bss %d\n", sctx->sim_bsssz);
	}

	sctx->need_n16bsr=(sctx->sim_txtsz>=(65536-4096));
	sctx->need_n20bsr=(sctx->sim_txtsz>=(1048576-65536));
	sctx->need_n23bsr=(sctx->sim_txtsz>=(8388608-131072));
	sctx->need_n24bsr=(sctx->sim_txtsz>=(16777216-262144));

	sctx->need_n16dat=(sctx->simimgsz>=(65536-4096));
	sctx->need_n20dat=(sctx->simimgsz>=(1048576-65536));
	sctx->need_n24dat=(sctx->simimgsz>=(16777216-262144));

	sctx->is_simpass=0;
	BGBCC_JX2C_EmitMemcpy64Autogen(ctx, sctx);

#if 0
#if 1
	for(i=0; i<ctx->n_reg_globals; i++)
	{
		if(i<shufgbl)
			j=shufarr[i];
		else
			j=i;
		obj=ctx->reg_globals[j];

//		obj=ctx->reg_globals[i];
		if(!obj)
			continue;

		if(obj->regtype==CCXL_LITID_GLOBALVAR)
		{
			BGBCC_JX2C_BuildGlobal(ctx, obj);
			continue;
		}

		if(obj->regtype==CCXL_LITID_STATICVAR)
		{
			BGBCC_JX2C_BuildGlobal(ctx, obj);
			continue;
		}

		if((obj->regtype==CCXL_LITID_STRUCT) ||
			(obj->regtype==CCXL_LITID_CLASS))
		{
			BGBCC_JX2C_BuildStruct(ctx, obj);
			continue;
		}
	}
#endif

	for(i=0; i<ctx->n_reg_globals; i++)
	{
		if(i<shufgbl)
			j=shufarr[i];
		else
			j=i;
		obj=ctx->reg_globals[j];

//		obj=ctx->reg_globals[i];
		if(!obj)
			continue;

		if((obj->regtype==CCXL_LITID_FUNCTION) && (obj->vtr))
		{
			BGBCC_JX2C_BuildFunction(ctx, obj);
			continue;
		}

		if(obj->regtype==CCXL_LITID_ASMBLOB)
		{
			BGBCC_JX2C_BuildAsmBlob(ctx, obj);
			continue;
		}

		if(obj->regtype==CCXL_LITID_OBJBLOB)
		{
			BGBCC_JX2C_BuildObjBlob(ctx, obj);
			continue;
		}

#if 0
		if(obj->regtype==CCXL_LITID_GLOBALVAR)
		{
			BGBCC_JX2C_BuildGlobal(ctx, obj);
			continue;
		}

		if(obj->regtype==CCXL_LITID_STATICVAR)
		{
			BGBCC_JX2C_BuildGlobal(ctx, obj);
			continue;
		}

		if((obj->regtype==CCXL_LITID_STRUCT) ||
			(obj->regtype==CCXL_LITID_CLASS))
		{
			BGBCC_JX2C_BuildStruct(ctx, obj);
			continue;
		}
#endif
	}
#endif

#if 1
	n=0;
	while(n<ctx->n_reg_globals)
	{
		l=ctx->n_reg_globals;
		for(i=n; i<l; i++)
		{
			if(i<shufgbl)
				j=shufarr[i];
			else
				j=i;
			obj=ctx->reg_globals[j];

	//		obj=ctx->reg_globals[i];
			if(!obj)
				continue;

			if(obj->regtype==CCXL_LITID_GLOBALVAR)
			{
				BGBCC_JX2C_BuildGlobal(ctx, obj);
				continue;
			}

			if(obj->regtype==CCXL_LITID_STATICVAR)
			{
				BGBCC_JX2C_BuildGlobal(ctx, obj);
				continue;
			}

			if((obj->regtype==CCXL_LITID_STRUCT) ||
				(obj->regtype==CCXL_LITID_CLASS))
			{
				BGBCC_JX2C_BuildStruct(ctx, obj);
				continue;
			}
		}

#if 1
		for(i=n; i<l; i++)
		{
			if(i<shufgbl)
				j=shufarr[i];
			else
				j=i;
			obj=ctx->reg_globals[j];

			if(!obj)
				continue;

#if 1
			if(obj->regtype==CCXL_LITID_ASMBLOB)
			{
				BGBCC_JX2C_BuildAsmBlob(ctx, obj);
				continue;
			}

			if(obj->regtype==CCXL_LITID_OBJECTBLOB)
			{
				BGBCC_JX2C_BuildObjBlob(ctx, obj);
				continue;
			}
#endif
		}
#endif

		for(i=n; i<l; i++)
		{
			if(i<shufgbl)
				j=shufarr[i];
			else
				j=i;
			obj=ctx->reg_globals[j];

			if(!obj)
				continue;

			if((obj->regtype==CCXL_LITID_FUNCTION) && (obj->vtr))
			{
				BGBCC_JX2C_BuildFunction(ctx, obj);
				continue;
			}

#if 0
			if(obj->regtype==CCXL_LITID_ASMBLOB)
			{
				BGBCC_JX2C_BuildAsmBlob(ctx, obj);
				continue;
			}

			if(obj->regtype==CCXL_LITID_OBJECTBLOB)
			{
				BGBCC_JX2C_BuildObjBlob(ctx, obj);
				continue;
			}
#endif
		}
		
		n=l;
	}
#endif

	if(imgfmt==BGBCC_IMGFMT_DLL)
	{
		i=BGBCC_JX2_LookupNamedLabel(sctx, "main");
		j=BGBCC_JX2_LookupLabelIndex(sctx, i);
		if(j<0)
		{
			BGBCC_JX2_SetSectionName(sctx, ".text");
			BGBCC_JX2_EmitNamedLabel(sctx, "main");
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);
		}
	}

	BGBCC_JX2C_BuildPrestartInit(ctx);

	BGBCC_JX2_SetSectionName(sctx, ".text");
	for(j=0; j<8; j++)
		BGBCC_JX2_TryEmitOpNone(sctx, BGBCC_SH_NMID_NOP);


	if(sctx->verbose)
	{
		k=sctx->stat_tot_dq0+sctx->stat_tot_dq1+sctx->stat_tot_dqi;
		if(k>0)
		{
			printf("DQ0/DQ1/DQi=%d/%d/%d %.2f%% %.2f%% %.2f%%\n",
				sctx->stat_tot_dq0,
				sctx->stat_tot_dq1,
				sctx->stat_tot_dqi,
				(100.0*sctx->stat_tot_dq0)/k,
				(100.0*sctx->stat_tot_dq1)/k,
				(100.0*sctx->stat_tot_dqi)/k
				);
		}

		if(sctx->stat_tot_imm>0)
		{
			printf("tot imm %d\n", sctx->stat_tot_imm);
			printf("tot imm8 %d\n", sctx->stat_tot_imm8);
			printf("tot imm16 %d\n", sctx->stat_tot_imm16);
			printf("tot imm8r %d\n", sctx->stat_tot_imm8r);
			printf("tot imm32 %d\n", sctx->stat_tot_imm32);
		}
		
		if(sctx->stat_opc_tot>0)
		{
			printf("Tot Op16=%d Op32=%d OpX32=%d\n",
				sctx->stat_opc_base16,
				sctx->stat_opc_ext8a,
				sctx->stat_opc_7xx+sctx->stat_opc_9xx);
		
			k=sctx->stat_opc_tot;
	//		printf("16=%.2f%% 8A=%.2f%% 8E=%.2f%% CE=%.2f%% "
	//				"CC0=%.2f%% CC3=%.2f%%\n",
			printf("16=%.2f%% Fz=%.2f%% FC=%.2f%% FD=%.2f%% "
					"CC0=%.2f%% CC3=%.2f%% 7z=%.2f%% 9z=%.2f%% RV=%.2f%% \n",
				(100.0*sctx->stat_opc_base16)/k,
				(100.0*sctx->stat_opc_ext8a)/k,
				(100.0*sctx->stat_opc_ext8e)/k,
				(100.0*sctx->stat_opc_extCe)/k,
				(100.0*sctx->stat_opc_extCC0)/k,
				(100.0*sctx->stat_opc_extCC3)/k,
				(100.0*sctx->stat_opc_7xx)/k,
				(100.0*sctx->stat_opc_9xx)/k,
				(100.0*sctx->stat_opc_rv)/k
				);

			printf("16=%.2fkB Fz=%.2fkB FC=%.2fkB FD=%.2fkB 7z=%.2fkB 9z=%.2fkB\n",
				(2.0*sctx->stat_opc_base16)/1024.0,
				(4.0*sctx->stat_opc_ext8a)/1024.0,
				(6.0*sctx->stat_opc_ext8e)/1024.0,
				(6.0*sctx->stat_opc_extCe)/1024.0,
				(3.0*sctx->stat_opc_7xx)/1024.0,
				(3.0*sctx->stat_opc_9xx)/1024.0
				);
		}
		
	#if 1
		printf("High 8 op use map:\n");
		printf("A    ");
		for(j=0; j<8; j++)
				printf("    x%1X", j);
		printf("\n");

		printf("B     ");
		for(j=8; j<16; j++)
				printf("    x%1X", j);
		printf("\n");
		
		for(i=0; i<16; i++)
		{
			if(((i==0xA) || (i==0xB) || (i==0xC) || (i==0xD)) &&
				!(sctx->is_fixed32&1))
			{
				k=0;
				for(j=0; j<16; j++)
					{ k+=sctx->opcnt_hi8[i*16+j]; }
				printf("  %1Xx %d\n", i, k);
				continue;
			}

			k=0;
			for(j=0; j<16; j++)
				{ k+=sctx->opcnt_hi8[i*16+j]; }
			if(!k)continue;
		
			printf("A %1Xx ", i);
			for(j=0; j<8; j++)
			{
				printf(" %5d", sctx->opcnt_hi8[i*16+j]);
			}
			printf("\n");

			printf("B %1Xx  ", i);
			for(j=8; j<16; j++)
			{
				printf(" %5d", sctx->opcnt_hi8[i*16+j]);
			}
			printf("\n");
		}
		printf("\n");
	#endif

	#if 1
//		if(!(sctx->is_fixed32&1))
		if(sctx->n_opcnt_3xx>0)
		{
			printf("High 3xzx op use map:\n");
		//	printf("   ");
		//	for(j=0; j<16; j++)
		//			printf("  x%1X", j);
		//	printf("\n");

			printf("A    ");
			for(j=0; j<8; j++)
					printf("    x%1X", j);
			printf("\n");

			printf("B     ");
			for(j=8; j<16; j++)
					printf("    x%1X", j);
			printf("\n");

		//	for(i=0; i<16; i++)
			for(i=0; i<8; i++)
			{
				k=0;
				for(j=0; j<16; j++)
					{ k+=sctx->opcnt_3xx[i*16+j]; }
				if(!k)continue;

				printf("A %1Xx ", i);
				for(j=0; j<8; j++)
				{
					printf(" %5d", sctx->opcnt_3xx[i*16+j]);
				}
				printf("\n");

				printf("B %1Xx  ", i);
				for(j=8; j<16; j++)
				{
					printf(" %5d", sctx->opcnt_3xx[i*16+j]);
				}
				printf("\n");
			}
			printf("\n");
		}
	#endif

	#if 1
		if(sctx->n_opcnt_f0xx>0)
		{
			printf("High F0xx op use map:\n");
		//	printf("   ");
		//	for(j=0; j<16; j++)
		//			printf("  x%1X", j);
		//	printf("\n");

			printf("A    ");
			for(j=0; j<8; j++)
					printf("    x%1X", j);
			printf("\n");

			printf("B     ");
			for(j=8; j<16; j++)
					printf("    x%1X", j);
			printf("\n");

			for(i=0; i<16; i++)
			{
				k=0;
				for(j=0; j<16; j++)
					{ k+=sctx->opcnt_f0xx[i*16+j]; }
				if(!k)continue;
			
				if(i>=0xC)
				{
					k=0;
					for(j=0; j<16; j++)
						{ k+=sctx->opcnt_f0xx[i*16+j]; }
					printf("  %1Xx %d\n", i, k);
					continue;
				}

				printf("A %1Xx ", i);
				for(j=0; j<8; j++)
				{
					printf(" %5d", sctx->opcnt_f0xx[i*16+j]);
				}
				printf("\n");

				printf("B %1Xx  ", i);
				for(j=8; j<16; j++)
				{
					printf(" %5d", sctx->opcnt_f0xx[i*16+j]);
				}
				printf("\n");
			}
			printf("\n");
		}
	#endif

	#if 1
//		if(sctx->is_fixed32&1)
		if(sctx->n_opcnt_f03xx>0)
		{
			printf("High F0-3xx op use map:\n");

			printf("A    ");
			for(j=0; j<8; j++)
					printf("    x%1X", j);
			printf("\n");

			printf("B     ");
			for(j=8; j<16; j++)
					printf("    x%1X", j);
			printf("\n");

		//	for(i=0; i<16; i++)
			for(i=0; i<8; i++)
			{
				k=0;
				for(j=0; j<16; j++)
					{ k+=sctx->opcnt_f03xx[i*16+j]; }
				if(!k)continue;

				printf("A %1Xx ", i);
				for(j=0; j<8; j++)
				{
					printf(" %5d", sctx->opcnt_f03xx[i*16+j]);
				}
				printf("\n");

				printf("B %1Xx  ", i);
				for(j=8; j<16; j++)
				{
					printf(" %5d", sctx->opcnt_f03xx[i*16+j]);
				}
				printf("\n");
			}
			printf("\n");



			printf("High F0-1xx op use map:\n");

			printf("A    ");
			for(j=0; j<8; j++)
					printf("    x%1X", j);
			printf("\n");

			printf("B     ");
			for(j=8; j<16; j++)
					printf("    x%1X", j);
			printf("\n");

		//	for(i=0; i<16; i++)
			for(i=0; i<8; i++)
			{
				k=0;
				for(j=0; j<16; j++)
					{ k+=sctx->opcnt_f01xx[i*16+j]; }
				if(!k)continue;

				printf("A %1Xx ", i+8);
				for(j=0; j<8; j++)
				{
					printf(" %5d", sctx->opcnt_f01xx[i*16+j]);
				}
				printf("\n");

				printf("B %1Xx  ", i+8);
				for(j=8; j<16; j++)
				{
					printf(" %5d", sctx->opcnt_f01xx[i*16+j]);
				}
				printf("\n");
			}
			printf("\n");
		}
	#endif

	#if 1
		if(sctx->n_opcnt_f1xx>0)
		{
			printf("High F1xx op use map:\n");
			for(i=0; i<8; i++)
			{
				printf(" %5d", sctx->opcnt_f1xx[i]);
			}
			printf("\n");
			for(i=8; i<16; i++)
			{
				printf(" %5d", sctx->opcnt_f1xx[i]);
			}
			printf("\n");
		}

		printf("\n");
	#endif

	#if 1
		if(sctx->n_opcnt_f2xx>0)
		{
			printf("High F2xx op use map:\n");

			printf("A    ");
			for(j=0; j<8; j++)
					printf("    x%1X", j);
			printf("\n");

			printf("B     ");
			for(j=8; j<16; j++)
					printf("    x%1X", j);
			printf("\n");

			printf("0x..7x ");
			for(i=0; i<8; i++)
			{
				k=0;
				for(j=0; j<16; j++)
					{ k+=sctx->opcnt_f2xx[i*16+j]; }
				printf(" %5d", k);
			}
			printf("\n");
			printf("8x..Bx ");
			for(i=8; i<12; i++)
			{
				k=0;
				for(j=0; j<16; j++)
					{ k+=sctx->opcnt_f2xx[i*16+j]; }
				printf(" %5d", k);
			}
			printf("\n");

			for(i=12; i<16; i++)
			{
				k=0;
				for(j=0; j<16; j++)
					{ k+=sctx->opcnt_f2xx[i*16+j]; }
				if(!k)continue;
			
				printf("A %1Xx ", i);
				for(j=0; j<8; j++)
				{
					printf(" %5d", sctx->opcnt_f2xx[i*16+j]);
				}
				printf("\n");

				printf("B %1Xx  ", i);
				for(j=8; j<16; j++)
				{
					printf(" %5d", sctx->opcnt_f2xx[i*16+j]);
				}
				printf("\n");
			}
			printf("\n");
		}
	#endif

	#if 1
		if(sctx->n_opcnt_f8xx>0)
		{
			printf("High F8xx op use map:\n");
			for(i=0; i<8; i++)
			{
				printf(" %5d", sctx->opcnt_f8xx[i]);
			}
			printf("\n");
			for(i=8; i<16; i++)
			{
				printf(" %5d", sctx->opcnt_f8xx[i]);
			}
			printf("\n");

			printf("\n");
		}
	#endif

		if(sctx->stat_opc_rv>0)
		{
			printf("High RV32 op use map:\n");
			for(i=0; i<32; i++)
			{
				k=0;
				for(j=0; j<8; j++)
					k+=sctx->opcnt_rv53xx[i*8+j];
				if(!k)
					continue;
			
				j=0;
				switch(i)
				{
				case 0x00:	s0="LD   ";		break;
				case 0x01:	s0="FLD  ";		break;
				case 0x02:	s0="Usr0 ";		break;
				case 0x03:	s0="FENCE";		break;
				case 0x04:	s0="ALUI ";		break;
				case 0x05:	s0="AUIPC";	j=1;	break;
				case 0x06:	s0="ALUIW";		break;
				case 0x07:	s0="OP48A";		break;

				case 0x08:	s0="ST   ";		break;
				case 0x09:	s0="FST  ";		break;
				case 0x0A:	s0="Usr1 ";		break;
				case 0x0B:	s0="AMO  ";		break;
				case 0x0C:	s0="ALU  ";		break;
				case 0x0D:	s0="LUI  ";	j=1;	break;
				case 0x0E:	s0="ALUW ";		break;
				case 0x0F:	s0="OP64 ";		break;

				case 0x10:	s0="FMA  ";		break;
				case 0x11:	s0="FMS  ";		break;
				case 0x12:	s0="FNMS ";		break;
				case 0x13:	s0="FNMA ";		break;
				case 0x14:	s0="FPU  ";		break;
				case 0x15:	s0="OP-V ";		break;
				case 0x16:	s0="Usr2 ";		break;
				case 0x17:	s0="OP48B";		break;

				case 0x18:	s0="Bcc  ";		break;
				case 0x19:	s0="JALR ";		break;
				case 0x1A:	s0="RSV0 ";		break;
				case 0x1B:	s0="JAL  ";	j=1;	break;
				case 0x1C:	s0="SYS  ";		break;
				case 0x1D:	s0="OP-P ";		break;
				case 0x1E:	s0="Usr3 ";		break;
				case 0x1F:	s0="OP80P";		break;
				}
			
				if(j>0)
				{
					printf("%02X (%s):  %5d\n", i, s0, k);
					continue;
				}
			
				printf("%02X (%s): ", i, s0);
				for(j=0; j<8; j++)
				{
					printf(" %5d", sctx->opcnt_rv53xx[i*8+j]);
				}
				printf("\n");
			}
			printf("\n");
		}

//		printf("\n");

		n=0;
		for(i=0; i<64; i++)
			n+=sctx->reg_heatstat[i];

		if(n>0)
		{
			printf("Register Heat Map:\n");
			for(i=0; i<16; i++)
			{
				for(j=0; j<4; j++)
				{
					k=sctx->reg_heatstat[i*4+j];
					if(k)break;
				}
				if(j>=4)
					continue;

				printf("R%02u: ", i*4);
				for(j=0; j<4; j++)
				{
					k=sctx->reg_heatstat[i*4+j];
					f=(100.0*k)/n;
					printf(" %5u(%2u.%02u%%)",
						k, ((int)f), ((int)(f*100))%100);
				}
				printf("\n");
			}
			printf("\n");
		}

		printf("Consts: MaskHit=%d MaskJumbo=%d MaskTot=%d\n"
			"\tTot_I16=%d (U=%d N=%d Fp16=%d) I20_LUI=%d\n"

			"\tTot_J64=%d (Fp32=%d 2xFp16=%d 4xFp8=%d "
				"i33l=%d i32h=%d i32c=%d)\n"
			"\tTot_J96=%d (PH=%d)\n",
			sctx->stat_const_maskhit,
			sctx->stat_const_maskjumbo,
			sctx->stat_const_masktot,
			
			sctx->stat_const_imm16,
			sctx->stat_const_imm16u,
			sctx->stat_const_imm16n,
			sctx->stat_const_imm16h,
			sctx->stat_const_imm20lui,

			sctx->stat_const_jumbo64,
			sctx->stat_const_jumbo64_f32,
			sctx->stat_const_jumbo64_2xf16,
			sctx->stat_const_jumbo64_4xf8,
			sctx->stat_const_jumbo64_imm33l,
			sctx->stat_const_jumbo64_imm32h,
			sctx->stat_const_jumbo64_imm32c,

			sctx->stat_const_jumbo96,
			sctx->stat_const_jumbo96ph
			);
		
		printf("Disp-Hit(Sc): 5u=%.2f%% 5u=%.2f%% 9u=%.2f%% 10u=%.2f%% "
				"10s=%.2f%% 12s=%.2f%%\n",
			(100.0*sctx->stat_ldst_disp5u)/(sctx->stat_ldst_disptot+1),
			(100.0*sctx->stat_ldst_disp6u)/(sctx->stat_ldst_disptot+1),
			(100.0*sctx->stat_ldst_disp9u)/(sctx->stat_ldst_disptot+1),
			(100.0*sctx->stat_ldst_disp10u)/(sctx->stat_ldst_disptot+1),
			(100.0*sctx->stat_ldst_disp10s)/(sctx->stat_ldst_disptot+1),
			(100.0*sctx->stat_ldst_disp12s)/(sctx->stat_ldst_disptot+1)
			);

		printf("Disp-Hit(Byte): 5u=%.2f%% 9u=%.2f%% "
				"10s=%.2f%% 12s=%.2f%%\n",
			(100.0*sctx->stat_ldst_disp5ub)/(sctx->stat_ldst_disptot+1),
			(100.0*sctx->stat_ldst_disp9ub)/(sctx->stat_ldst_disptot+1),
			(100.0*sctx->stat_ldst_disp10sb)/(sctx->stat_ldst_disptot+1),
			(100.0*sctx->stat_ldst_disp12sb)/(sctx->stat_ldst_disptot+1)
			);

		printf("Imm-Hit(3RI): 5u=%.2f%% 9u=%.2f%% "
				"5n=%.2f%%, 9n=%.2f%%, 9un=%.2f%%\n",
			(100.0*sctx->stat_imm3ri_imm5u)/(sctx->stat_imm3ri_immtot+1),
			(100.0*sctx->stat_imm3ri_imm9u)/(sctx->stat_imm3ri_immtot+1),
			(100.0*sctx->stat_imm3ri_imm5n)/(sctx->stat_imm3ri_immtot+1),
			(100.0*sctx->stat_imm3ri_imm9n)/(sctx->stat_imm3ri_immtot+1),
			(100.0*sctx->stat_imm3ri_imm9un)/(sctx->stat_imm3ri_immtot+1)
			);

		printf("Imm-Hit(3RI): 10u=%.2f%%, 10n=%.2f%%, 10un=%.2f%%\n",
			(100.0*sctx->stat_imm3ri_imm10u)/(sctx->stat_imm3ri_immtot+1),
			(100.0*sctx->stat_imm3ri_imm10n)/(sctx->stat_imm3ri_immtot+1),
			(100.0*sctx->stat_imm3ri_imm10un)/(sctx->stat_imm3ri_immtot+1)
			);

		printf("Imm-Hit(3RI): 10u_sh12=%.2f%%, 10u_sh16=%.2f%%\n",
			(100.0*sctx->stat_imm3ri_imm10u_sh12)/(sctx->stat_imm3ri_immtot+1),
			(100.0*sctx->stat_imm3ri_imm10u_sh16)/(sctx->stat_imm3ri_immtot+1)
			);

#if 0
		for(i=0; i<16; i++)
		{
			if(sctx->stat_imm3ri_immtot_aox[i]<=0)
				continue;

			j=sctx->stat_imm3ri_immtot_aox[i];

			printf("Imm-Hit(3RI, AO=%u): 5u=%.2f%%, 5n=%.2f%%, 5un=%.2f%%\n",
				i,
				(100.0*sctx->stat_imm3ri_imm5u_aox[i])/j,
				(100.0*sctx->stat_imm3ri_imm5n_aox[i])/j,
				(100.0*sctx->stat_imm3ri_imm5un_aox[i])/j
				);

			printf("Imm-Hit(3RI, AO=%u): 9u=%.2f%%, 9n=%.2f%%, 9un=%.2f%%\n",
				i,
				(100.0*sctx->stat_imm3ri_imm9u_aox[i])/j,
				(100.0*sctx->stat_imm3ri_imm9n_aox[i])/j,
				(100.0*sctx->stat_imm3ri_imm9un_aox[i])/j
				);

			printf("Imm-Hit(3RI, AO=%u): 10u=%.2f%%, 10n=%.2f%%, 10un=%.2f%%\n",
				i,
				(100.0*sctx->stat_imm3ri_imm10u_aox[i])/j,
				(100.0*sctx->stat_imm3ri_imm10n_aox[i])/j,
				(100.0*sctx->stat_imm3ri_imm10un_aox[i])/j
				);
		}
#endif

		printf("Imm-Hit(2RI): 6u=%.2f%% 10u=%.2f%% "
				"6n=%.2f%%, 10n=%.2f%%, 10un=%.2f%%\n",
			(100.0*sctx->stat_imm2ri_imm6u)/(sctx->stat_imm2ri_immtot+1),
			(100.0*sctx->stat_imm2ri_imm10u)/(sctx->stat_imm2ri_immtot+1),
			(100.0*sctx->stat_imm2ri_imm6n)/(sctx->stat_imm2ri_immtot+1),
			(100.0*sctx->stat_imm2ri_imm10n)/(sctx->stat_imm2ri_immtot+1),
			(100.0*sctx->stat_imm2ri_imm10un)/(sctx->stat_imm2ri_immtot+1)
			);

		printf("Imm-Hit(2RI): 10u_sh12=%.2f%%, 10u_sh16=%.2f%%\n",
			(100.0*sctx->stat_imm2ri_imm10u_sh12)/(sctx->stat_imm2ri_immtot+1),
			(100.0*sctx->stat_imm2ri_imm10u_sh16)/(sctx->stat_imm2ri_immtot+1)
			);

		printf("Imm-Hit: Miss2RI=%.2f%% Miss3RI=%.2f%% "
				"Jumbo2RI=%.2f%% Jumbo3RI=%.2f%%\n",
			(100.0*sctx->stat_imm2ri_hmiss)/(sctx->stat_imm2ri_hmtot+1),
			(100.0*sctx->stat_imm3ri_hmiss)/(sctx->stat_imm3ri_hmtot+1),
			(100.0*sctx->stat_imm2ri_hjmb)/(sctx->stat_imm2ri_hmtot+1),
			(100.0*sctx->stat_imm3ri_hjmb)/(sctx->stat_imm3ri_hmtot+1)
			);
		
		printf("Imm-Hit: IsFp=%.2f%% Fp16=%.2f%% Fp5=%.2f%% Fp5b=%.2f%%\n",
			(100.0*sctx->stat_fp16_isfpa)/(sctx->stat_const_masktot+1),
			(100.0*sctx->stat_fp16_tot)/(sctx->stat_const_masktot+1),
			(100.0*sctx->stat_fp16_hit5)/(sctx->stat_const_masktot+1),
			(100.0*sctx->stat_fp16_hit5b)/(sctx->stat_const_masktot+1));

		printf("Imm-Hit: IsFp MissExp=%.2f%% MissPow10=%.2f%% "
			"MissPow3=%.2f%% MissPow7=%.2f%%\n",
			(100.0*sctx->stat_fp16_miss_exp)/(sctx->stat_const_masktot+1),
			(100.0*sctx->stat_fp16_miss_pow10)/(sctx->stat_const_masktot+1),
			(100.0*sctx->stat_fp16_miss_pow3)/(sctx->stat_const_masktot+1),
			(100.0*sctx->stat_fp16_miss_pow7)/(sctx->stat_const_masktot+1));

		printf("Imm-Hit(Mask): 10u_sh12=%.2f%%, 10u_sh16=%.2f%%\n",
			(100.0*sctx->stat_mask_imm10u_sh12)/(sctx->stat_const_masktot+1),
			(100.0*sctx->stat_mask_imm10u_sh16)/(sctx->stat_const_masktot+1)
			);

		if(sctx->stat_fpimm_totchk5>0)
		{
			printf("Imm-Hit Fp5: %.2f%%\n",
				(100.0*sctx->stat_fpimm_hitchk5)/(sctx->stat_fpimm_totchk5));
		}
		if(sctx->stat_fpimm_totchk10>0)
		{
			printf("Imm-Hit Fp10: %.2f%%\n",
				(100.0*sctx->stat_fpimm_hitchk10)/(sctx->stat_fpimm_totchk10));
		}

		if(sctx->stat_lbl_tot>0)
		{
			printf("Lbl Disp-Hit 8s=%.2f%% 12s=%.2f%% 16s=%.2f%% "
				"20s=%.2f%% 24s=%.2f%% 33s=%.2f%%  tot=%d\n",
				(100.0*sctx->stat_lbl_disp8 )/(sctx->stat_lbl_tot),
				(100.0*sctx->stat_lbl_disp12)/(sctx->stat_lbl_tot),
				(100.0*sctx->stat_lbl_disp16)/(sctx->stat_lbl_tot),
				(100.0*sctx->stat_lbl_disp20)/(sctx->stat_lbl_tot),
				(100.0*sctx->stat_lbl_disp24)/(sctx->stat_lbl_tot),
				(100.0*sctx->stat_lbl_disp33)/(sctx->stat_lbl_tot),
				sctx->stat_lbl_tot);
		}

		printf("AddrMode(3RI): 2R=%.2f%%, 2R+=%.2f%%\n",
			(100.0*sctx->stat_ldst_2r)/(sctx->stat_ldst_tot+1),
			(100.0*sctx->stat_ldst_2rpi)/(sctx->stat_ldst_tot+1)
			);
		printf("AddrMode(3RI): Disp=%.2f%%, Idx=%.2f%%, IdxDisp=%.2f%%\n",
			(100.0*sctx->stat_ldst_disp)/(sctx->stat_ldst_tot+1),
			(100.0*sctx->stat_ldst_indx)/(sctx->stat_ldst_tot+1),
			(100.0*sctx->stat_ldst_idxdisp)/(sctx->stat_ldst_tot+1)
			);

	#if 0
		printf("Exp-Fp16:\n");
		for(i=0; i<4; i++)
		{
			for(j=0; j<8; j++)
			{
				printf("%.2f%% ",
					(100.0*sctx->stat_fp16_exp[i*8+j])/sctx->stat_fp16_tot);
			}
			printf("\n");
		}
	#endif

		printf("Exp-Funarg:\n");
		k=0;
		for(i=0; i<8; i++)
		{
			for(j=0; j<4; j++)
			{
				k+=sctx->stat_funarg_exp[i*4+j];
				printf("%.2f%%(%.2f%%) ",
					(100.0*sctx->stat_funarg_exp[i*4+j])/sctx->stat_funarg_tot,
					(100.0*k)/sctx->stat_funarg_tot);
			}
			printf("\n");
		}

		printf("Exp-Varcount:\n");
		k=0;
		for(i=0; i<16; i++)
		{
			printf("%3u: ", i*4*4);
			for(j=0; j<4; j++)
			{
				k+=sctx->stat_varcount_exp[i*4+j];
				printf("%.2f%%(%.2f%%) ",
					(100.0*sctx->stat_varcount_exp[i*4+j])/
						sctx->stat_varcount_tot,
					(100.0*k)/sctx->stat_varcount_tot);
			}
			printf("\n");
		}

		if(sctx->stat_ldst_pbotot>0)
		{
			printf("PBO: Indexed=%.2f%% Disp9=%.2f%% "
					"Disp10_sc=%.2f%% Miss=%.2f%%\n",
				(100.0*sctx->stat_ldst_pbotot_ix)/sctx->stat_ldst_pbotot,
				(100.0*sctx->stat_ldst_pbotot_9b)/sctx->stat_ldst_pbotot,
				(100.0*sctx->stat_ldst_pbotot_10b)/sctx->stat_ldst_pbotot,
				(100.0*sctx->stat_ldst_pbotot_33b)/sctx->stat_ldst_pbotot);
		}

		if(sctx->stat_func_tot>0)
		{
			printf("TotalFuncs=%d\n"
					"  Leaf: LeafTiny=%.2f%% LeafOther=%.2f%%\n"
					"  Non-Leaf: MaxRsv=%.2f%% PartRsv=%.2f%% Alias=%.2f%%\n",
				sctx->stat_func_tot,
				(100.0*sctx->stat_func_leaftiny)/sctx->stat_func_tot,
				(100.0*sctx->stat_func_leaf    )/sctx->stat_func_tot,
				(100.0*sctx->stat_func_maxrsv  )/sctx->stat_func_tot,
				(100.0*sctx->stat_func_partrsv )/sctx->stat_func_tot,
				(100.0*sctx->stat_func_alias   )/sctx->stat_func_tot);

			if(sctx->stat_func_partrsv>0)
			{
				printf("  Part-Rsv Avg: rsv/val/tot=%.2f/%.2f/%.2f max=%.2f\n",
					(1.0*sctx->stat_func_acc_vsprsv )/(sctx->stat_func_partrsv),
					(1.0*sctx->stat_func_acc_vspval )/(sctx->stat_func_partrsv),
					(1.0*sctx->stat_func_acc_vspan  )/(sctx->stat_func_partrsv),
					(1.0*sctx->stat_func_acc_vspmax )/(sctx->stat_func_partrsv));
			}

			printf("  Avg Pro=%d Epi=%d Bod=%d\n",
				sctx->stat_size_prolog/sctx->stat_size_cnt,
				sctx->stat_size_epilog/sctx->stat_size_cnt,
				sctx->stat_size_body/sctx->stat_size_cnt);

			f=	sctx->stat_size_prolog+
				sctx->stat_size_epilog+
				sctx->stat_size_body;
			printf("  Avg Pro=%.2f%% Epi=%.2f%% Bod=%.2f%%\n",
				(100.0*sctx->stat_size_prolog)/f,
				(100.0*sctx->stat_size_epilog)/f,
				(100.0*sctx->stat_size_body)/f);
		}

		printf("WEXed: F0=%.2f%% F1=%.2f%% F2=%.2f%% F8=%.2f%% Tot=%.2f%%\n",
			(100.0*(sctx->opcnt_hi8[0xF4]+
				sctx->opcnt_hi8[0xEA]+sctx->opcnt_hi8[0xEE]))/
				(sctx->opcnt_hi8[0xF0]+sctx->opcnt_hi8[0xF4]+
				sctx->opcnt_hi8[0xEA]+sctx->opcnt_hi8[0xEE]+1),

			(100.0*sctx->opcnt_hi8[0xF5])/
				(sctx->opcnt_hi8[0xF1]+sctx->opcnt_hi8[0xF5]+1),

			(100.0*(sctx->opcnt_hi8[0xF6]+
					sctx->opcnt_hi8[0xEB]+sctx->opcnt_hi8[0xEF]))/
				(sctx->opcnt_hi8[0xF2]+sctx->opcnt_hi8[0xF6]+
				sctx->opcnt_hi8[0xEB]+sctx->opcnt_hi8[0xEF]+1),

			(100.0*sctx->opcnt_hi8[0xFC])/
				(sctx->opcnt_hi8[0xF8]+sctx->opcnt_hi8[0xFC]+1),

			(100.0*(
				sctx->opcnt_hi8[0xF4]+
				sctx->opcnt_hi8[0xF5]+
				sctx->opcnt_hi8[0xF6]+
				sctx->opcnt_hi8[0xFC]+
				sctx->opcnt_hi8[0xEA]+
				sctx->opcnt_hi8[0xEB]+
				sctx->opcnt_hi8[0xEE]+
				sctx->opcnt_hi8[0xEF]
				))/
				(
				sctx->opcnt_hi8[0xF0]+
				sctx->opcnt_hi8[0xF1]+
				sctx->opcnt_hi8[0xF2]+
				sctx->opcnt_hi8[0xF4]+
				sctx->opcnt_hi8[0xF5]+
				sctx->opcnt_hi8[0xF8]+
				sctx->opcnt_hi8[0xF6]+
				sctx->opcnt_hi8[0xFC]+
				sctx->opcnt_hi8[0xEA]+
				sctx->opcnt_hi8[0xEB]+
				sctx->opcnt_hi8[0xEE]+
				sctx->opcnt_hi8[0xEF]+
				1)

			);
		if(sctx->stat_cnt_frm_size>0)
		{
			printf("Average Stack Frame Size: %d\n",
				(int)(sctx->stat_tot_frm_size/sctx->stat_cnt_frm_size));
		}
	}


	if(sctx->lvt16_n_idx>0)
	{
		BGBCC_JX2_SetSectionName(sctx, ".data");
		BGBCC_JX2_EmitBAlign(sctx, 16);
		for(i=0; i<sctx->lvt16_n_idx; i++)
		{
			BGBCC_JX2_EmitLabel(sctx, sctx->lvt16_lbl[i]);
			if(sctx->is_le)
			{
				BGBCC_JX2_EmitQWord(sctx, sctx->lvt16_val[i*2+0]);
				BGBCC_JX2_EmitQWord(sctx, sctx->lvt16_val[i*2+1]);
			}else
			{
				BGBCC_JX2_EmitQWord(sctx, sctx->lvt16_val[i*2+1]);
				BGBCC_JX2_EmitQWord(sctx, sctx->lvt16_val[i*2+0]);
			}
		}
	}

	if(sctx->got_n_gblidx>0)
	{
		BGBCC_JX2_SetSectionName(sctx, ".got");
		BGBCC_JX2_EmitBAlign(sctx, 4);
		BGBCC_JX2_EmitLabel(sctx, sctx->lbl_got);
		
		addr=0;
		for(i=0; i<sctx->got_n_gblidx; i++)
		{
			j=sctx->got_gblidx[i];
			l0=ctx->reg_globals[j]->fxoffs;
//			BGBCC_JX2_EmitRelocAbs32(sctx, l0);
//			BGBCC_JX2_EmitDWord(sctx, 0);
			BGBCC_JX2_EmitDWordAbs32(sctx, k);
		}
	}

	BGBCC_JX2_SetSectionName(sctx, ".text");
	BGBCC_JX2_EmitNamedLabel(sctx, "__text_end");
	BGBCC_JX2_EmitBAlign(sctx, 16);

	BGBCC_JX2_SetSectionName(sctx, ".data");
	BGBCC_JX2_EmitNamedLabel(sctx, "__data_end");

	BGBCC_JX2_SetSectionName(sctx, ".bss");
	BGBCC_JX2_EmitNamedLabel(sctx, "_end");
	BGBCC_JX2_EmitNamedLabel(sctx, "__bss_end");

	BGBCC_JX2_SetSectionName(sctx, ".utext");
	BGBCC_JX2_EmitNamedLabel(sctx, "__utext_end");

	BGBCC_JX2_SetSectionName(sctx, ".udata");
	BGBCC_JX2_EmitNamedLabel(sctx, "__udata_end");

	if(sctx->rwad_dir)
	{
		BGBCC_JX2_SetSectionName(sctx, ".rsrc");

		BGBCC_JX2_EmitBAlign(sctx, 4);
		BGBCC_JX2_EmitLabel(sctx, sctx->rwad_lbltype);
		for(i=0; i<(256-32); i++)
		{
			j=sctx->rwad_types[i+0x20];
			if(!j)break;
			BGBCC_JX2_EmitDWordI(sctx, j);
		}
		BGBCC_JX2_EmitSetOffsDWord(sctx, 0x1C, i*4);

		for(i=0; i<128; i++)
			sctx->rwad_hash[i]=0xFFFF;
		for(i=(sctx->rwad_ndirent-1); i>=0; i--)
		{
			j=bgbcc_getu32le((byte *)(sctx->rwad_dir[i].name)+0);
			j+=bgbcc_getu32le((byte *)(sctx->rwad_dir[i].name)+4);
			j+=bgbcc_getu32le((byte *)(sctx->rwad_dir[i].name)+8);
			j+=bgbcc_getu32le((byte *)(sctx->rwad_dir[i].name)+12);
//			j=j*16777213;
//			j=j*15813251;
			j=j*0xF14A83;
//			j=(j>>24)&127;
			j=(j>>24)&63;
			sctx->rwad_dir[i].chain=sctx->rwad_hash[j];
			sctx->rwad_hash[j]=i;
		}

		BGBCC_JX2_EmitSetOffsDWord(sctx, 0x14, sctx->rwad_ndirent*32);

		BGBCC_JX2_EmitBAlign(sctx, 16);
		BGBCC_JX2_EmitLabel(sctx, sctx->rwad_lbldir);
		for(i=0; i<sctx->rwad_ndirent; i++)
		{
			j=sctx->rwad_dir[i].lbl;
			BGBCC_JX2_EmitRelocTy(sctx, j, BGBCC_SH_RLC_ABS32);
			BGBCC_JX2_EmitDWordI(sctx, 0);
			BGBCC_JX2_EmitDWordI(sctx, sctx->rwad_dir[i].csize);
			BGBCC_JX2_EmitDWordI(sctx, sctx->rwad_dir[i].dsize);
			BGBCC_JX2_EmitByteI(sctx, sctx->rwad_dir[i].ety);
			BGBCC_JX2_EmitByteI(sctx, sctx->rwad_dir[i].cmp);
			BGBCC_JX2_EmitWordI(sctx, sctx->rwad_dir[i].chain);
			BGBCC_JX2_EmitRawBytes(sctx,
				(byte *)(sctx->rwad_dir[i].name), 16);
		}

//		for(i=0; i<128; i++)
		for(i=0; i<64; i++)
		{
			BGBCC_JX2_EmitWordI(sctx, sctx->rwad_hash[i]);
		}
	}

	if(sctx->tlsi_lbl)
	{
		BGBCC_JX2_SetSectionName(sctx, ".tls");
		BGBCC_JX2_EmitSetOffsDWord(sctx, 0x10, sctx->sz_tlsvar);
	}


	if(ctx->n_error)
	{
		printf("Errors Seen (CGEN)\n");
		return(-1);
	}

	if(bgbcc_dumpast)
	{
		if(ctx->imgbasename)
			sprintf(tb, "dump/%s_asm.txt", ctx->imgbasename);
		else
			sprintf(tb, "dump/aout_asm.txt");
		
		BGBCC_JX2C_DumpImageASM(ctx, tb);


	}

	if((imgfmt==BGBCC_IMGFMT_EXE) ||
		(imgfmt==BGBCC_IMGFMT_DLL) ||
		(imgfmt==BGBCC_IMGFMT_SYS))
	{
		BGBCC_JX2C_FlattenImagePECOFF(ctx, obuf, rosz, imgfmt);

		if(bgbcc_dumpast)
		{
			if(ctx->imgbasename)
				sprintf(tb, "dump/%s_dasm.txt", ctx->imgbasename);
			else
				sprintf(tb, "dump/aout_dasm.txt");
			BGBCC_JX2C_DumpImageDisAsm(ctx, tb);
		}


		if(ctx->n_error)
		{
			printf("Errors Seen (IMG)\n");
			return(-1);
		}

		return(1);
	}

//	if((imgfmt==BGBCC_IMGFMT_ELXE) ||
//		(imgfmt==BGBCC_IMGFMT_ELSO))
//	{
//		BGBCC_JX2C_FlattenImageELF(ctx, obuf, rosz, imgfmt);
//		return(1);
//	}

	if(imgfmt==BGBCC_IMGFMT_ROM)
	{
		BGBCC_JX2C_FlattenImageROM(ctx, obuf, rosz, imgfmt);

		if(bgbcc_dumpast)
		{
			if(ctx->imgbasename)
				sprintf(tb, "dump/%s_dasm.txt", ctx->imgbasename);
			else
				sprintf(tb, "dump/aout_dasm.txt");
			BGBCC_JX2C_DumpImageDisAsm(ctx, tb);
		}

		if(ctx->n_error)
		{
			printf("Errors Seen (IMG)\n");
			return(-1);
		}

		return(1);
	}

	if(imgfmt==BGBCC_IMGFMT_ASM)
	{
		BGBCC_JX2C_FlattenImageASM(ctx, obuf, rosz, imgfmt);
		return(1);
	}

	return(-1);
}
