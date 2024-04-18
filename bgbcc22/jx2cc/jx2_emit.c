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

int BGBCC_JX2_EmitCheckRegQuad(BGBCC_JX2_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
	{
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16)
	{
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ32)
	{
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ48)
	{
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitCheckRegExt4(BGBCC_JX2_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R16)
	{
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD16)
	{
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16)
	{
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR16)
	{
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR16)
	{
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR16)
	{
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R48)
		return(1);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ48)
		return(1);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD48)
		return(1);

	return(0);
}

int BGBCC_JX2_EmitCheckRegExt5(BGBCC_JX2_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R32)
	{
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R48)
	{
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ32)
	{
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ48)
	{
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD32)
	{
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD48)
	{
		return(1);
	}


	return(0);
}

int BGBCC_JX2_EmitCheckRegNeedSx(BGBCC_JX2_Context *ctx, int reg)
{
	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD16) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD32) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD48))
	{
//		if(ctx->reg_pszx[reg&15]!=BGBCC_PSZX_SX)
//			return(1);
//		return(0);

		if((ctx->reg_pszx[reg&63]==BGBCC_PSZX_SX) ||
			(ctx->reg_pszx[reg&63]==BGBCC_PSZX_SSX) ||
			(ctx->reg_pszx[reg&63]==BGBCC_PSZX_SZX))
			return(0);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitCheckRegNeedZx(BGBCC_JX2_Context *ctx, int reg)
{
	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD16) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD32) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD48))
	{
//		if(ctx->reg_pszx[reg&15]!=BGBCC_PSZX_SX)
//			return(1);
//		return(0);

		if((ctx->reg_pszx[reg&63]==BGBCC_PSZX_ZX) ||
			(ctx->reg_pszx[reg&63]==BGBCC_PSZX_SZX))
			return(0);
		return(1);
	}

	return(0);

#if 0
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0)
	{
		if(ctx->reg_pszx[reg&63]!=BGBCC_PSZX_ZX)
			return(1);
		return(0);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD16)
	{
		if(ctx->reg_pszx[reg&63]!=BGBCC_PSZX_ZX)
			return(1);
		return(0);
	}
#endif

	return(0);
}

int BGBCC_JX2_EmitCheckRegNeedSzx(BGBCC_JX2_Context *ctx, int reg)
{
	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD16) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD32) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD48))
	{
//		if(ctx->reg_pszx[reg&31]!=BGBCC_PSZX_SX)
//			return(1);
		return(0);

#if 1
		if(	(ctx->reg_pszx[reg&63]==BGBCC_PSZX_SX) ||
			(ctx->reg_pszx[reg&63]==BGBCC_PSZX_ZX) ||
			(ctx->reg_pszx[reg&63]==BGBCC_PSZX_SSX) ||
			(ctx->reg_pszx[reg&63]==BGBCC_PSZX_SZX))
			return(0);
#endif
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitCheckRegExtPlainGPR(BGBCC_JX2_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_R0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_RD0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_RQ0)
		return(1);

	return(0);
}

int BGBCC_JX2_EmitCheckRegExtGPR(BGBCC_JX2_Context *ctx, int reg)
{
	if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		return(1);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R16)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD16)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16)
		return(1);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR16)
		return(1);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R32)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD32)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ32)
		return(1);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R48)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD48)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ48)
		return(1);

	return(0);
}

int BGBCC_JX2_EmitCheckRegExt32GPR(BGBCC_JX2_Context *ctx, int reg)
{
//	if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
//		return(1);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
		return(1);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R16)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD16)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16)
		return(1);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
	{
		if(reg&1)
			return(0);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR16)
	{
		if(reg&1)
			return(0);
		return(1);
	}

	return(0);

//	if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
//		return(0);
//	if(BGBCC_JX2_EmitCheckRegExt5(ctx, reg))
//		return(0);
//	return(1);
}

int BGBCC_JX2_EmitCheckRegBase32GPR(BGBCC_JX2_Context *ctx, int reg)
{
	return(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg));

//	if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
//		return(0);
//	if(BGBCC_JX2_EmitCheckRegExt5(ctx, reg))
//		return(0);
//	return(1);
}

int BGBCC_JX2_EmitCheckRegExtAddrGPR(BGBCC_JX2_Context *ctx, int reg)
{
	if(BGBCC_JX2_EmitCheckRegAddrGPR(ctx, reg))
		return(1);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R16)
		return(1);
//	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD16)
//		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16)
		return(1);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R32)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ32)
		return(1);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R48)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ48)
		return(1);


	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD16)
	{
		if(ctx->is_addr_x32)
			return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitCheckRegBaseGPR(BGBCC_JX2_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
		return(1);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0)
	{
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
	{
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
	{
		if(reg&1)
			return(0);
		return(1);
	}

	if(reg==BGBCC_SH_REG_SP)
		return(1);

	return(0);
}

int BGBCC_JX2_EmitCheckRegAddrGPR(BGBCC_JX2_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
	{
		if((reg&31)<2)
			return(0);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
	{
		if((reg&31)<2)
			return(0);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0)
	{
		if((reg&31)<2)
			return(0);
		if(ctx->is_addr_x32)
			return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_LR0)
	{
		if(ctx->has_xgpr&2)
			return(1);
		return(0);
	}

	if(reg==BGBCC_SH_REG_SP)
		return(1);

	return(0);
}

int BGBCC_JX2_EmitCheckRegNonBaseGPR(BGBCC_JX2_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
		return(0);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0)
		return(0);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
		return(0);

	if(reg==BGBCC_SH_REG_SP)
		return(0);

	return(1);
}


int BGBCC_JX2_EmitCheckRegBaseFPR(BGBCC_JX2_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
		return(1);

	return(0);
}

int BGBCC_JX2_EmitCheckRegExtFPR(BGBCC_JX2_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
		return(1);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR16)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR16)
		return(1);

	return(0);
}

int BGBCC_JX2_EmitOpCheckRepackOp24(
	BGBCC_JX2_Context *ctx, int *ropw1, int *ropw2, int isjumbo)
{
	int opw1, opw2, opw3, opw4, imm;
	int i, j, k;

	if(!ctx->has_ops24 || (ctx->has_xgpr&1) ||
		ctx->op_is_wex2 || ctx->is_fixed32 || isjumbo || (ctx->emit_riscv&0x22))
			return(0);
	
	if(ctx->op_wex_align&1)
		return(0);

	opw1=*ropw1;
	opw2=*ropw2;
	
	if((opw1<0) || (opw2<0))
		return(0);
		
	if(((opw1>>16)>4) || ((opw2>>16)>4))
		return(0);
	
	if(((opw1&0xFF00)==0xF000) && ((opw2&0xF000)==0x0000))
	{
		opw3=0x39000 |
			((opw2<<8)&0x0F00) |
			((opw1   )&0x00FF) ;
		opw4=0x70000 | ((opw2>>4)&0x00FF);

		*ropw1=opw3;
		*ropw2=opw4;
		return(1);
	}
	
	if(((opw1&0xFF00)==0xF000) && ((opw2&0xF008)==0x1008))
	{
		opw3=0x37000 |
			((opw2<<8)&0x0F00) |
			((opw1   )&0x00FF) ;
		opw4=0x70000 | ((opw2>>4)&0x00FF);

		*ropw1=opw3;
		*ropw2=opw4;
		return(1);
	}
	
	if(((opw1&0xFF00)==0xF000) && ((opw2&0xF808)==0x1000))
	{
		opw3=0x37000 |
			((opw2<<8)&0x0F00) |
			((opw1   )&0x00FF) ;
		opw4=0x70000 | ((opw2>>4)&0x00FF);

		*ropw1=opw3;
		*ropw2=opw4;
		return(1);
	}

	if(	((opw1&0xFF00)==0xF100) &&
		((opw2&0x4000)==0x0000))
	{
		/* Skip PC and GBR Relative */
		if((opw1&0x000E)==0x0000)
//		if((opw1&0x000F)==0x0000)
			return(0);

		/* Reject larger displacements. */
		if((opw2&0x1FF)>0x1F)
			return(0);
	
		opw3=0x39000 |
			((opw2<<8)&0x0F00) |
			((opw1   )&0x00FF);
		opw4=0x70000 |
			((opw2>>4)&0x00E0) |
			((opw2   )&0x001F);

		*ropw1=opw3;
		*ropw2=opw4;
		return(1);
	}

	if((opw1&0xFF00)==0xF200)
	{
		j=-1;
		imm=(opw2&0x1FF);
		if((opw2&0xF800)==0x0000)	j=0;
		if((opw2&0xF800)==0x2000)	j=2;
		if((opw2&0xF800)==0x2800)	j=3;
		if(imm>0x1F)		j=-1;

		if((opw2&0xF800)==0x1000)
		{
			imm=-(imm|((~0U)<<9));
			j=1;
			if(imm>0x1F)	j=-1;
		}

		if((opw2&0xF880)==0x8000)	j=4;
		if((opw2&0xF880)==0x8080)	j=5;
		if((opw2&0xF880)==0x9000)	j=6;
		if((opw2&0xF880)==0x8080)	j=7;
		
		if(j>=0)
		{
			opw3=0x37000 |
				((   j<<8)&0x0F00) |
				((opw1   )&0x00FF);
			opw4=0x70080 |
				((opw2>>4)&0x00E0) |
				((imm    )&0x001F);

			*ropw1=opw3;
			*ropw2=opw4;
			return(1);
		}
	}

	if(((opw1&0xFF00)==0xF000) && ((opw2&0xF80F)==0x3000))
	{
		opw3=0x39F00 |
			((opw1   )&0x00FF) ;
		opw4=0x70080 | ((opw2>>4)&0x00FF);

		*ropw1=opw3;
		*ropw2=opw4;
		return(1);
	}
	
	return(0);
}

int BGBCC_JX2_EmitOpCheckExpandOp24(
	BGBCC_JX2_Context *ctx, int op0, int op1, int *ropw1, int *ropw2)
{
	int opw1, opw2, z, i;

	if(!ctx->has_ops24 || (ctx->has_xgpr&1))
		return(0);

	if((ctx->is_fixed32&2) || (ctx->emit_riscv&0x22))
		return(0);

	opw1=-1;
	opw2=-1;

	if((op0&0xF000)==0x9000)
	{
		opw1=0xF000 | (op0&0x00FF);
		opw2=0x0000 |
			((op1<<4)&0x0FF0) |
			((op0>>8)&0x000F) ;

		if(((op0&0xFF00)==0x9B00) && (op1&0x80))
			{ opw1=-1; opw2=-1; }
		if(((op0&0xFF00)==0x9F00) && (op1&0x80))
		{
			opw2=0x3000 | ((op1<<4)&0x07F0);
		}
	}

	if((op0&0xF800)==0x7800)
	{
		opw1=0xF000 | (op0&0x00FF);
		opw2=0x1000 |
			((op1<<4)&0x0FF0) |
			((op0>>8)&0x000F) ;
	}

	if(((op0&0xF800)==0x7000) && ((op1&0x80)==0x00))
	{
		opw1=0xF000 | (op0&0x00FF);
		opw2=0x1000 |
			((op1<<4)&0x0FF0) |
			((op0>>8)&0x000F) ;
	}
	
	if(((op0&0xF800)==0x7000) && ((op1&0x80)==0x80))
	{
		z=-1; i=op1&0x1F;
		
		if((op0&0x0F00)==0x0000)	z=0x0000;
		if((op0&0x0F00)==0x0100)	{ z=0x11E0; i=-i; }
		if((op0&0x0F00)==0x0200)	z=0x2000;
		if((op0&0x0F00)==0x0300)	z=0x2800;

		if((op0&0x0F00)==0x0400)	z=0x8000;
		if((op0&0x0F00)==0x0500)	z=0x81E0;
		if((op0&0x0F00)==0x0600)	z=0x9000;
		if((op0&0x0F00)==0x0700)	z=0x91E0;
	
		if(z>=0)
		{
			opw1=0xF200 | (op0&0x00FF);
			opw2=z | ((op1<<4)&0x0600) | (i&0x01FF) ;
		}
	}
	
	*ropw1=opw1;
	*ropw2=opw2;
	return((opw1>=0) && (opw2>=0));
}

int BGBCC_JX2_EmitOpCheckRepack(
	BGBCC_JX2_Context *ctx, int *ropw1, int *ropw2, int isjumbo)
{
	int opw1, opw2, opw3, opw4;
	int opw1v, opw3v;
	int i, flip;

	if(!ropw1)
		return(0);

	opw1=*ropw1;
	
	if(	(((opw1>>16)&15)==3) ||
		(((opw1>>16)&15)==4) ||
		(((opw1>>16)&15)==5) ||
		(((opw1>>16)&15)==6) )
			return(0);

	opw1v=opw1;
	if((ctx->is_fixed32&2) || (ctx->emit_riscv&0x22))
		opw1v|=0xE000;

#ifdef BGBCC_BJX2_FLIPSTNM
	if((opw1v&0xE000)!=0xE000)
	{
		opw3=opw1;
		flip=0;

		if((opw1&0xF800)==0x0000)
		{
			opw3=(opw3&0xFF00)|
				((opw3<<4)&0x00F0)|
				((opw3>>4)&0x000F);
			flip=1;
		}

#if 0
		if((opw1&0xF800)==0x4000)
		{
			opw3=(opw3&0xFF00)|
				((opw3<<4)&0x00F0)|
				((opw3>>4)&0x000F);
			flip=1;
		}
#endif

		if(((opw1&0xF800)==0x8000) &&
			((opw1&0xFF00)!=0x8000))
		{
			opw3=(opw3&0xFF00)|
				((opw3<<4)&0x00F0)|
				((opw3>>4)&0x000F);
			flip=1;
		}

#if 0
		if((opw1&0xFC00)==0x9000)
		{
			opw3=(opw3&0xFF00)|
				((opw3<<4)&0x00F0)|
				((opw3>>4)&0x000F);
			flip=1;
		}
#endif

//		*ropw1=opw3;
		*ropw1=0x30000|opw3;
	
		return(0);
	}
#endif

	if(!ropw2)
		return(0);

	opw1=*ropw1;
	opw2=*ropw2;

	if((opw1<0) || (opw2<0))
		return(0);

	if((opw1v&0xFF00)==0xF000)
	{
		if(((opw2&0xF000)>=0xC000))
			return(0);

		flip=0;

		opw3=(opw1&0xFF00)|
			(opw2&0x00FF);
		opw4=(opw2&0xF000)|
			((opw2>>8)&0x000F)|
			((opw1<<4)&0x0FF0);

#ifdef BGBCC_BJX2_FLIPSTNM
		if((opw4&0xF808)==0x0000)
		{
			opw3=(opw3&0xFF00)|
				((opw3<<4)&0x00F0)|
				((opw3>>4)&0x000F);
			opw4=(opw4&0xF9FF)|
				((opw4<<1)&0x0400)|
				((opw4>>1)&0x0200);
			flip=1;
		}
		
#if 0
		if((opw4&0xF80C)==0x9000)
		{
			opw3=(opw3&0xFF00)|
				((opw3<<4)&0x00F0)|
				((opw3>>4)&0x000F);
			opw4=(opw4&0xF9FF)|
				((opw4<<1)&0x0400)|
				((opw4>>1)&0x0200);
		}
#endif

		if((opw4&0xF808)==0x4000)
		{
			opw3=(opw3&0xFF00)|
				((opw3<<4)&0x00F0)|
				((opw3>>4)&0x000F);
			opw4=(opw4&0xF9FF)|
				((opw4<<1)&0x0400)|
				((opw4>>1)&0x0200);
			flip=1;
		}

#if 1
		if((opw4&0xF808)==0x9000)
		{
			opw3=(opw3&0xFF00)|
				((opw3<<4)&0x00F0)|
				((opw3>>4)&0x000F);
			opw4=(opw4&0xF9FF)|
				((opw4<<1)&0x0400)|
				((opw4>>1)&0x0200);
			flip=1;
		}
#endif

//		if((opw4&0xF008)==0x8000)
//		if((opw4&0xF808)==0x8000)
		if(	((opw4&0xF808)==0x8000) ||
			((opw4&0xF80B)==0x8803)	)
		{
			opw3=(opw3&0xFF00)|
				((opw3<<4)&0x00F0)|
				((opw3>>4)&0x000F);
			opw4=(opw4&0xF9FF)|
				((opw4<<1)&0x0400)|
				((opw4>>1)&0x0200);
			flip=1;
		}
#endif

		opw3|=0x40000;
		opw4|=0x50000;
		
		if(flip)
			opw3|=0x60000;

		BGBCC_JX2_EmitOpCheckRepackOp24(ctx, &opw3, &opw4, isjumbo);
		*ropw1=opw3;
		*ropw2=opw4;
		return(1);
	}

#ifdef BGBCC_BJX2_FLIPSTNM
	if((opw1v&0xFF00)==0xF100)
	{
		opw3=opw1;
		opw4=opw2;
		flip=0;
	
		if(	((opw4&0xC800)==0x0000)||
//			((opw4&0xE800)==0x4000))
			((opw4&0xE000)==0x4000))
		{
			opw3=(opw3&0xFF00)|
				((opw3<<4)&0x00F0)|
				((opw3>>4)&0x000F);
			opw4=(opw4&0xF9FF)|
				((opw4<<1)&0x0400)|
				((opw4>>1)&0x0200);
			flip=1;
		}

		opw3|=0x40000;
		opw4|=0x50000;

		if(flip)
			opw3|=0x60000;

		BGBCC_JX2_EmitOpCheckRepackOp24(ctx, &opw3, &opw4, isjumbo);
		*ropw1=opw3;
		*ropw2=opw4;
		return(1);
	}
#endif

	opw3=opw1;
	opw4=opw2;	
	i=BGBCC_JX2_EmitOpCheckRepackOp24(ctx, &opw3, &opw4, isjumbo);
	if(i>0)
	{
		*ropw1=opw3;
		*ropw2=opw4;
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpCheckRepack3(
	BGBCC_JX2_Context *ctx, int *ropw1, int *ropw2, int *ropw3)
{
	int opw1, opw2, opw3;
	int l1, l2;

	if(!ropw1)
		return(0);

	if(!ropw2 || !ropw3)
	{
		BGBCC_JX2_EmitOpCheckRepack(ctx, ropw1, ropw2, 0);
		return(0);
	}

	opw1=*ropw1;
	opw2=*ropw2;
	opw3=*ropw3;

#if 0
	if((opw1&0xFC00)==0xFC00)
	{
		if(((opw1&0xFFF8)==0xFC00) && ((opw2&0xF800)==0x0000))
		{
			opw1=(opw1&0xFFF9)|
				((opw1<<1)&0x0004)|
				((opw1>>1)&0x0002);
			opw2=(opw2&0xFF00)|
				((opw2<<4)&0x00F0)|
				((opw2>>4)&0x000F);
		
			*ropw1=opw1;
			*ropw2=opw2;
			return(1);
		}

		return(0);
	}
#endif

//	BGBCC_JX2_EmitOpCheckRepack(ctx, ropw1, ropw2);
	BGBCC_JX2_EmitOpCheckRepack4(ctx, ropw1, ropw2, ropw3, NULL);
	return(0);
}

int BGBCC_JX2_EmitOpGetLength(
	BGBCC_JX2_Context *ctx,
	int opw)
{
	if(opw<0)
		return(0);

	if((ctx->is_fixed32&2) || (ctx->emit_riscv&0x22))
		return(2);

	if((opw&0xE000)==0xE000)
	{
//		if((opw&0x0C00)==0x0C00)
//			return(3);
		return(2);
	}
	
	if(ctx->has_xgpr&1)
	{
		if((opw&0xF000)==0x7000)
			return(2);
		if((opw&0xF000)==0x9000)
			return(2);
	}
	
	return(1);
}

int BGBCC_JX2_EmitOpCheckRepack4(
	BGBCC_JX2_Context *ctx,
	int *ropw1, int *ropw2,
	int *ropw3, int *ropw4)
{
	int opw1, opw2;
	int l1, l2, isj;

//	if(!ropw1 || !ropw2)
	if(!ropw1)
		return(0);

	if(!ropw2)
	{
		BGBCC_JX2_EmitOpCheckRepack(ctx, ropw1, ropw2, 0);
		return(0);
	}
	
	opw1=*ropw1;
	opw2=*ropw2;
	
	l1=BGBCC_JX2_EmitOpGetLength(ctx, opw1);
	l2=BGBCC_JX2_EmitOpGetLength(ctx, opw2);

	if(l1==3)
	{
		BGBCC_JX2_EmitOpCheckRepack3(ctx, ropw1, ropw2, ropw3);
		BGBCC_JX2_EmitOpCheckRepack(ctx, ropw4, NULL, 0);
		return(0);
	}
	
//	if((opw1&0xF000)==0xF000)
	if(l1==2)
	{
		isj=((opw1&0xFE00)==0xFE00);
		BGBCC_JX2_EmitOpCheckRepack(ctx, ropw1, ropw2, 0);
		BGBCC_JX2_EmitOpCheckRepack(ctx, ropw3, ropw4, isj);
		return(0);
	}else if(l1==1)
	{
		BGBCC_JX2_EmitOpCheckRepack(ctx, ropw1, NULL, 0);
		BGBCC_JX2_EmitOpCheckRepack(ctx, ropw2, ropw3, 0);
		BGBCC_JX2_EmitOpCheckRepack(ctx, ropw4, NULL, 0);
	}

	return(0);
}

int BGBCC_JX2_EmitOpCheckRepack5(
	BGBCC_JX2_Context *ctx, int *ropw1, int *ropw2, int *ropw3,
	int *ropw4, int *ropw5)
{
	BGBCC_JX2_EmitOpCheckRepack6(ctx, ropw1, ropw2, ropw3, ropw4, ropw5, NULL);
	return(0);
}

int BGBCC_JX2_EmitOpCheckRepack6(
	BGBCC_JX2_Context *ctx,
	int *ropw1, int *ropw2,
	int *ropw3, int *ropw4,
	int *ropw5, int *ropw6)
{
	int opw1, opw2;
	int opw3, opw4;
	int l1, l2, isj1, isj2;
	int l3, l4;

//	if(!ropw1 || !ropw2)
	if(!ropw1)
		return(0);
	if(!ropw2)
	{
		BGBCC_JX2_EmitOpCheckRepack(ctx,
			ropw1, ropw2, 0);
		return(0);
	}
	if(!ropw3 || !ropw4)
	{
		BGBCC_JX2_EmitOpCheckRepack4(ctx,
			ropw1, ropw2, ropw3, ropw4);
		return(0);
	}
	
	opw1=*ropw1;
	opw2=*ropw2;
	opw3=*ropw3;
	opw4=*ropw4;

	l1=BGBCC_JX2_EmitOpGetLength(ctx, opw1);
	l2=BGBCC_JX2_EmitOpGetLength(ctx, opw2);
	l3=BGBCC_JX2_EmitOpGetLength(ctx, opw3);
	l4=BGBCC_JX2_EmitOpGetLength(ctx, opw4);

	if(l1==3)
	{
		BGBCC_JX2_EmitOpCheckRepack3(ctx, ropw1, ropw2, ropw3);

		if(l4==1)
		{
			BGBCC_JX2_EmitOpCheckRepack(ctx, ropw4, NULL, 0);
			BGBCC_JX2_EmitOpCheckRepack(ctx, ropw5, ropw6, 0);
		}else
			if(l4==2)
		{
			BGBCC_JX2_EmitOpCheckRepack(ctx, ropw4, ropw5, 0);
			BGBCC_JX2_EmitOpCheckRepack(ctx, ropw6, NULL, 0);
		}
		return(0);
	}
	
//	if((opw1&0xF000)==0xF000)
	if(l1==2)
	{
		BGBCC_JX2_EmitOpCheckRepack(ctx, ropw1, ropw2, 0);
		isj1=((opw1&0xFE00)==0xFE00);
		isj2=((opw3&0xFE00)==0xFE00);

		if(l3==3)
		{
			BGBCC_JX2_EmitOpCheckRepack3(ctx, ropw3, ropw4, ropw5);
			BGBCC_JX2_EmitOpCheckRepack(ctx, ropw6, NULL, 0);
		}else if(l3==2)
		{
			BGBCC_JX2_EmitOpCheckRepack(ctx, ropw3, ropw4, isj1);
			BGBCC_JX2_EmitOpCheckRepack(ctx, ropw5, ropw6, isj2);
		}else if(l3==1)
		{
			if(l4==3)
			{
				BGBCC_JX2_EmitOpCheckRepack3(ctx, ropw4, ropw5, ropw6);
			}else if(l4==2)
			{
				isj2=((opw4&0xFE00)==0xFE00);
				BGBCC_JX2_EmitOpCheckRepack(ctx, ropw4, ropw5, 0);
				BGBCC_JX2_EmitOpCheckRepack(ctx, ropw6, NULL, isj2);
			}else if(l4==1)
			{
				BGBCC_JX2_EmitOpCheckRepack(ctx, ropw5, ropw6, 0);
			}
		}

		return(0);
	}else if(l1==1)
	{
		BGBCC_JX2_EmitOpCheckRepack(ctx, ropw1, NULL, 0);

		isj1=((opw2&0xFE00)==0xFE00);
		isj2=((opw4&0xFE00)==0xFE00);

		if(l2==3)
		{
			BGBCC_JX2_EmitOpCheckRepack3(ctx, ropw2, ropw3, ropw4);
			BGBCC_JX2_EmitOpCheckRepack(ctx, ropw5, ropw6, 0);
		}else if(l2==2)
		{
			BGBCC_JX2_EmitOpCheckRepack(ctx, ropw2, ropw3, 0);
			BGBCC_JX2_EmitOpCheckRepack(ctx, ropw4, ropw5, isj1);
			BGBCC_JX2_EmitOpCheckRepack(ctx, ropw6, NULL, isj2);
		}else if(l2==1)
		{
			BGBCC_JX2_EmitOpCheckRepack(ctx, ropw2, NULL, 0);

			if(l3==2)
			{
				isj1=((opw3&0xFE00)==0xFE00);
				BGBCC_JX2_EmitOpCheckRepack(ctx, ropw3, ropw4, 0);
				BGBCC_JX2_EmitOpCheckRepack(ctx, ropw5, ropw6, isj1);
			}else
				if(l3==1)
			{
				BGBCC_JX2_EmitOpCheckRepack(ctx, ropw3, NULL, 0);

				if(l4==3)
				{
					BGBCC_JX2_EmitOpCheckRepack3(ctx, ropw4, ropw5, ropw6);
				}else if(l4==2)
				{
					BGBCC_JX2_EmitOpCheckRepack(ctx, ropw4, ropw5, 0);
					BGBCC_JX2_EmitOpCheckRepack(ctx, ropw6, NULL, isj2);
				}else if(l4==1)
				{
					BGBCC_JX2_EmitOpCheckRepack(ctx, ropw4, NULL, 0);
					BGBCC_JX2_EmitOpCheckRepack(ctx, ropw5, ropw6, 0);
				}
			}
		}
	}

	return(0);
}

int BGBCC_JX2_EmitOpCheckRepack2Exw(
	BGBCC_JX2_Context *ctx, int exw,
	int *ropw1, int *ropw2)
{
	int opw1, opw2, opw3, opw4;

	if(!exw)
		return(0);
	
	if(*ropw1<0)
		return(0);
	
	if(!(ctx->has_xgpr&1))
	{
		*ropw1=-1;
		*ropw2=-1;
		return(-1);
	}
	
	opw1=*ropw1;
	opw2=*ropw2;

	if((ctx->is_fixed32&2) || (ctx->emit_riscv&0x22))
	{
		if((opw1&0xFF00)==0xFF00)
			return(0);
		if((opw1&0xE000)!=0xE000)
			return(0);

		opw3=opw1;
		opw4=opw2;

		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw3, &opw4, 0);
		if(((opw3>>16)&15)==6)
		{
			exw=( exw    &0xF9FF)|
				((exw<<1)&0x0400)|
				((exw>>1)&0x0200);
		}
		
//		opw3=(0x4E000|(opw3&0x1FFF));		//debug
		opw3=(0x4E000|(opw3&0x1FFF))^((exw<<5)&0xE000);
		*ropw1=opw3;
		*ropw2=opw4;
		return(1);
	}

	if((opw1&0xFF00)==0xFF00)
		return(0);
	
	if((opw1&0xF000)==0x7000)
		return(0);
	if((opw1&0xF000)==0x9000)
		return(0);

//	if(ctx->op_is_wex2&4)
	if(ctx->op_is_wex2&12)
	{
		*ropw1=-1;
		*ropw2=-1;
		return(-1);
	}

	if(exw&0x1000)
	{
		*ropw1=-1;
		*ropw2=-1;
		return(-1);
	}

	opw3=opw1;
	opw4=opw2;

	if((opw1&0xFF00)==0xF000)
	{
		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw3, &opw4, 0);
//		if((opw3&0x00FF)!=(opw2&0x00FF))
		if(((opw3>>16)&15)==6)
		{
			exw=( exw    &0xF9FF)|
				((exw<<1)&0x0400)|
				((exw>>1)&0x0200);
		}
		
		opw3=0x47000|(opw3&0x00FF)|(exw&0x0700);
		*ropw1=opw3;
		*ropw2=opw4;
		return(1);
	}

	if((opw1&0xFF00)==0xF100)
	{
		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw3, &opw4, 0);
//		if((opw3&0x00FF)!=(opw1&0x00FF))
//		if(	((opw3&0xFFFF)!=(opw1&0xFFFF)) ||
//			((opw4&0xFFFF)!=(opw2&0xFFFF)) )
		if(((opw3>>16)&15)==6)
		{
			exw=( exw    &0xF9FF)|
				((exw<<1)&0x0400)|
				((exw>>1)&0x0200);
		}

		opw3=0x49100|(opw3&0x00FF)|(exw&0x0700);
		*ropw1=opw3;
		*ropw2=opw4;
		return(1);
	}

	if((opw1&0xFF00)==0xF200)
	{
		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw3, &opw4, 0);

#if 1
//		if((opw3&0x00FF)!=(opw1&0x00FF))
		if(((opw3>>16)&15)==6)
		{
			exw=( exw    &0xF9FF)|
				((exw<<1)&0x0400)|
				((exw>>1)&0x0200);
		}
#endif

		opw3=0x49000|(opw3&0x00FF)|(exw&0x0700);
		*ropw1=opw3;
		*ropw2=opw4;
		return(1);
	}

	if((opw1&0xFF00)==0xF800)
	{
		*ropw1=-1;
		*ropw2=-1;
		return(-1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpCheckRepack4Exw(
	BGBCC_JX2_Context *ctx, int exw,
	int *ropw1, int *ropw2,
	int *ropw3, int *ropw4)
{
	int opw1, opw2, opw3, opw4;
	int i0, i1, imm, rn, iex, is_imm10;

	if(!exw)
		return(0);

	opw1=*ropw1;
	opw2=*ropw2;
	opw3=*ropw3;
	opw4=*ropw4;

	if((opw1&0xFF00)==0xFF00)
		return(0);
		
	if((opw3<0) && !(ctx->is_fixed32&2) && !(ctx->emit_riscv&0x22))
	{
		if((opw1&0xFF00)==0xF800)
		{
//			return(0);
		}

		if((opw1&0xFFE0)==0xF800)
		{
			imm=opw2;
			rn=opw1&0x1F;
			if(exw&0x0400)
				rn|=0x20;
			
//			opw1=0x4FF00|((rn&0x0020)<<1);
//			opw2=0x50000|((imm>>8)&255);
//			opw3=0x4F200|((rn&15)<<4);
//			opw4=0x5C000|((imm>>0)&255)|((rn&0x0010)<<6);

			opw1=0x4FF00|((rn&0x0020)<<1);
			opw2=0x50000;
			opw3=0x4F800|(rn&31);
			opw4=0x50000|(imm&0xFFFF);

			*ropw1=opw1;
			*ropw2=opw2;
			*ropw3=opw3;
			*ropw4=opw4;
			return(1);
		}

		if((opw1&0xFFE0)==0xF820)
		{
			imm=opw2;
			rn=opw1&0x1F;
			if(exw&0x0400)
				rn|=0x20;
			
//			opw1=0x4FF00|((rn&0x0020)<<1);
//			opw2=0x50000|((imm>>8)&255);
//			opw3=0x4F201|((rn&15)<<4);
//			opw4=0x5C000|((imm>>0)&255)|((rn&0x0010)<<6);

//			opw1=0x4FF00|((rn&0x0020)<<1);
			opw1=0x4FF10|((rn&0x0020)<<1);
			opw2=0x5FFFF;
			opw3=0x4F800|(rn&31);
			opw4=0x50000|(imm&0xFFFF);

			*ropw1=opw1;
			*ropw2=opw2;
			*ropw3=opw3;
			*ropw4=opw4;
			return(1);
		}

		if((opw1&0xFFE0)==0xF840)
		{
			imm=opw2;
			rn=opw1&0x1F;
			if(exw&0x0400)
				rn|=0x20;

//			opw1=0x4FF00|((rn&0x0020)<<1);
//			opw2=0x50000|((imm>>8)&255);
//			opw3=0x4F200|((rn&15)<<4);
//			opw4=0x5D000|((imm>>0)&255)|((rn&0x0010)<<6);
//			if(imm&0x8000)
//				{ opw3|=0x0001; opw4|=0x0300; }

//			opw1=0x4FF00|((rn&0x0020)<<1);
			opw1=0x4FF00|((rn&0x0020)<<1)|((imm&0x8000)?0x0010:0x0000);
			opw2=0x50000|((imm&0x8000)?0xFFFF:0x0000);
			opw3=0x4F840|(rn&31);
			opw4=0x50000|(imm&0xFFFF);

			*ropw1=opw1;
			*ropw2=opw2;
			*ropw3=opw3;
			*ropw4=opw4;
			return(1);
		}

		if((opw1&0xFFE0)==0xF860)
		{
			imm=opw2;
			rn=opw1&0x1F;
			if(exw&0x0400)
				rn|=0x20;

			opw1=0x4FF00|((rn&0x0020)<<1);
			opw2=0x50000|((imm>>8)&255);
			opw3=0x4F202|((rn&15)<<4);
			opw4=0x5C000|((imm>>0)&255)|((rn&0x0010)<<6);

			*ropw1=opw1;
			*ropw2=opw2;
			*ropw3=opw3;
			*ropw4=opw4;
			return(1);
		}

		if((opw1&0xFFE0)==0xF880)
		{
			imm=opw2;
			
			imm=BGBCC_ConstConvHalfToFloat(imm);
			
			rn=opw1&0x1F;
			if(exw&0x0400)
				rn|=0x20;

			opw1=0x4FF00|((rn&0x0020)<<1);
			opw2=0x50000|((imm>>16)&0xFFFF);
			opw3=0x4F880|(rn&31);
			opw4=0x50000|(imm&0xFFFF);

			*ropw1=opw1;
			*ropw2=opw2;
			*ropw3=opw3;
			*ropw4=opw4;
			return(1);
		}

//		if(ctx->op_is_wex2&4)
		if((ctx->op_is_wex2&12) || (exw&0x1000))
		{

			if((opw1&0xFF00)==0xF000)
			{
				opw3=opw1;
				opw4=opw2;

				BGBCC_JX2_EmitOpCheckRepack(ctx, &opw3, &opw4, 1);
//				if((opw3&0x00FF)!=(opw2&0x00FF))
//				if(	(opw3&0x00FF)!=(opw2&0x00FF) ||
//					(opw4&0x0600)!=((opw1&0x0060)<<4))
				if(((opw3>>16)&15)==6)
				{
					exw=( exw    &0xF9FF)|
						((exw<<1)&0x0400)|
						((exw>>1)&0x0200);
				}

				if((opw3&0xF000)!=0xF000)
					{ BGBCC_DBGBREAK }

				opw1=0x4FF00;
				opw2=0x50000;
				opw1|=(exw>>4)&0x0070;
				if(exw&0x1000)
					opw1|=0x0080;

				*ropw1=opw1;
				*ropw2=opw2;
				*ropw3=opw3;
				*ropw4=opw4;
				return(1);
			}

			if((opw1&0xFF00)==0xF100)
			{
				opw3=opw1;
				opw4=opw2;

				BGBCC_JX2_EmitOpCheckRepack(ctx, &opw3, &opw4, 1);
//				if((opw3&0x00FF)!=(opw1&0x00FF))

//				if(	(opw3&0xFFFF)!=(opw1&0xFFFF) ||
//					(opw4&0xFFFF)!=(opw2&0xFFFF) )
				if(((opw3>>16)&15)==6)
				{
					exw=( exw    &0xF9FF)|
						((exw<<1)&0x0400)|
						((exw>>1)&0x0200);
				}

				if((opw3&0xF000)!=0xF000)
					{ BGBCC_DBGBREAK }

				opw1=0x4FF00;
				opw2=0x50000;
				opw1|=(exw>>4)&0x0060;
				if(exw&0x1000)
					opw1|=0x0080;

				if(opw4&0x0100)
				{
					opw4&=~0x0100;
					opw2|= 0x0001;
				}

				*ropw1=opw1;
				*ropw2=opw2;
				*ropw3=opw3;
				*ropw4=opw4;
				return(1);
			}

			if((opw1&0xFF00)==0xF200)
			{
				iex=0;
				is_imm10=0;
				switch((opw2>>12)&15)
				{
					case 0x1: iex=1; break;
					case 0x4: iex=1; break;
					case 0xC:
					case 0xD:
					case 0xE:
					case 0xF:
						is_imm10=1;
						iex=(opw1&1);
						break;
				}
			
				opw3=opw1;
				opw4=opw2;

				BGBCC_JX2_EmitOpCheckRepack(ctx, &opw3, &opw4, 1);
#if 0
				if((opw3&0x00FF)!=(opw1&0x00FF))
					exw=( exw    &0xF9FF)|
						((exw<<1)&0x0400)|
						((exw>>1)&0x0200);
#endif

				if((opw3&0xF000)!=0xF000)
					{ BGBCC_DBGBREAK }

				opw1=0x4FF00;
				opw2=0x50000;
				opw1|=(exw>>4)&0x0060;
				if(iex&1)
					opw2|=0x00FF;
				if(exw&0x1000)
					opw1|=0x0080;

				if(iex)
					opw1|=0x0010;
				
				if(!iex)
				{
					if(opw4&0x0100)
						opw2|= 0x0001;
					opw4&=~0x0100;
				}else
				{
					opw2&=~0x0001;
					if(opw4&0x0100)
						opw2|= 0x0001;
					opw4|= 0x0100;
				}
				
				if(is_imm10)
				{
					if(!iex)
					{
						if(opw4&0x0200)
							opw2|= 0x0002;
						opw4&=~0x0200;
					}else
					{
						opw2&=~0x0002;
						if(opw4&0x0200)
							opw2|= 0x0002;
						opw4|= 0x0200;
					}
				}

				*ropw1=opw1;
				*ropw2=opw2;
				*ropw3=opw3;
				*ropw4=opw4;
				return(1);
			}
		}
	}

	if((opw1&0xFF00)==0xFE00)
	{
		i0=0;
		i1=BGBCC_JX2_EmitOpCheckRepack2Exw(ctx, exw, ropw3, ropw4);
	}else
	{
		i0=BGBCC_JX2_EmitOpCheckRepack2Exw(ctx, exw, ropw1, ropw2);
		i1=BGBCC_JX2_EmitOpCheckRepack2Exw(ctx, exw, ropw3, ropw4);
	}

	if((i0<0) || (i1<0))
	{
		*ropw1=-1;
		*ropw2=-1;
		*ropw3=-1;
		*ropw4=-1;
		return(-1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpCheckRepack6Exw(
	BGBCC_JX2_Context *ctx, int exw,
	int *ropw1, int *ropw2,
	int *ropw3, int *ropw4,
	int *ropw5, int *ropw6)
{
	int i0, i1, i2;

	if(!exw)
		return(0);

	if((*ropw5)<0)
	{
		if(((*ropw1)&0xFF00)==0xFF00)
		{
			return(0);
		}
	}

	if(((*ropw3)<0) && ((*ropw5)<0))
	{
		i0=BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, exw,
			ropw1, ropw2, ropw3, ropw4);
		return(i0);
	}

	if(((*ropw1)&0xFE00)==0xFE00)
	{
		i0=BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, exw,
			ropw1, ropw2, ropw3, ropw4);
		i1=0;
		i2=BGBCC_JX2_EmitOpCheckRepack2Exw(ctx, exw, ropw5, ropw6);
	}else if((*ropw5)<0)
	{
		i0=BGBCC_JX2_EmitOpCheckRepack2Exw(ctx, exw, ropw1, ropw2);
		i1=BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, exw,
			ropw2, ropw3, ropw4, ropw5);
		i2=0;
	}else
	{
		i0=BGBCC_JX2_EmitOpCheckRepack2Exw(ctx, exw, ropw1, ropw2);
		i1=BGBCC_JX2_EmitOpCheckRepack2Exw(ctx, exw, ropw3, ropw4);
		i2=BGBCC_JX2_EmitOpCheckRepack2Exw(ctx, exw, ropw5, ropw6);
	}

	if((i0<0) || (i1<0) || (i2<0))
	{
		*ropw1=-1;
		*ropw2=-1;
		*ropw3=-1;
		*ropw4=-1;
		return(-1);
	}

	return(0);
}

int BGBCC_JX2_EmitLoadDrImm(
	BGBCC_JX2_Context *ctx, s64 imm)
{
	int opw1, opw2, opw3, opw4, opw5, opw6;

#if 0
	if(ctx->is_fixed32&16)
//	if(1)
	{
		BGBCC_JX2_EmitLoadRegImm64P(ctx, BGBCC_SH_REG_RQ0, imm);
		return(1);
	}
#endif

//	BGBCC_JX2DA_EmitLoadRegImm64(ctx,
//		BGBCC_SH_NMID_MOV, BGBCC_SH_REG_RQ0, imm);

	if(!ctx->is_fixed32 && !ctx->op_is_wex2 && !(ctx->emit_riscv&0x22))
	{
#if 0
		if((imm&255)==imm)
		{
			opw1=0x2400|(imm&255);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitPadCheckAligned(ctx);
			return(1);
		}

		if((imm|(~255LL))==imm)
		{
			opw1=0x2500|(imm&255);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitPadCheckAligned(ctx);
			return(1);
		}
#endif

#if 1
		if(1)
		{
			if((imm&4095)==imm)
			{
				opw1=0xA000|(imm&4095);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}

			if((imm|(~4095LL))==imm)
			{
				opw1=0xB000|(imm&4095);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}
		}
#endif

#if 0
		opw1=0x2600|(imm&255);
		BGBCC_JX2_EmitLoadDrImm(ctx, imm>>8);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
#endif
	}

#if 1
//	if(1)
	if(!BGBCC_JX2_CheckPadCross32(ctx) || ctx->is_fixed32)
	{
#if 1
		opw1=-1;
		if((imm&1023)==imm)
		{
			opw1=0xF200;
			opw2=0xC000|(imm&1023);
		}
	
		if((imm|(~1023))==imm)
		{
			opw1=0xF201;
			opw2=0xC000|(imm&1023);
		}
		
		if((opw1<0) && (ctx->abi_spillpad&32))
		{
			if((imm&0x03FF0000)==imm)
			{
				opw1=0xF200;
				opw2=0xC800|((imm>>16)&1023);
			}
		
			if(((imm&0x03FF0000LL)|0x0FC000000LL)==imm)
			{
				opw1=0xF201;
				opw2=0xC800|((imm>>16)&1023);
			}
		}

		if(opw1>=0)
		{
			BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2, 0);
			BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			BGBCC_JX2_EmitPadCheckAligned(ctx);
			return(1);
		}
#endif

#if 1
		if((imm&65535)==imm)
		{
			opw1=0xF800;
			opw2=(u16)imm;

			BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2, 0);
			BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			BGBCC_JX2_EmitPadCheckAligned(ctx);
			return(1);
		}

		if((imm|(~65535))==imm)
		{
			opw1=0xF820;
			opw2=(u16)imm;

			BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2, 0);
			BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			BGBCC_JX2_EmitPadCheckAligned(ctx);
			return(1);
		}
#endif
		
		if(!(ctx->op_is_wex2&12))
		{
			if((imm&16777215)==imm)
			{
				opw1=0xFA00|((imm>>16)&255);
				opw2=(u16)imm;

				BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2, 0);
				BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw2);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}

			if((imm|(~16777215))==imm)
			{
				opw1=0xFB00|((imm>>16)&255);
				opw2=(u16)imm;

				BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2, 0);
				BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw2);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}
		}

#if 1
//		if(ctx->has_jumbo)
		if(ctx->has_jumbo &&
			((ctx->tctx->optmode!=BGBCC_OPT_SIZE) ||
			ctx->is_fixed32 || ctx->op_is_wex2 || (ctx->emit_riscv&0x22)))
		{
			if((((s32)imm)==imm) || (((u32)imm)==imm))
			{
				opw1=0xFE00|((imm>>24)&255);
				opw2=(u16)(imm>>8);
				opw3=0xF200;
				opw4=0xC000|(imm&255)|((imm>>63)&0x0100);

				BGBCC_JX2_EmitOpCheckRepack4(ctx,
					&opw1, &opw2, &opw3, &opw4);
				BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw2);
				BGBCC_JX2_EmitWord(ctx, opw3);
				BGBCC_JX2_EmitWord(ctx, opw4);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}
		}
#endif

		if(ctx->has_bra48)
		{
			if((imm&0x0000FFFFFFFFFFFFLL)==imm)
			{
				opw1=0xFE00|((imm>>40)&255);
				opw2=(u16)(imm>>24);
				opw3=0xFA00|((imm>>16)&255);
				opw4=(u16)imm;

				BGBCC_JX2_EmitOpCheckRepack4(ctx,
					&opw1, &opw2, &opw3, &opw4);
				BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw2);
				BGBCC_JX2_EmitWord(ctx, opw3);
				BGBCC_JX2_EmitWord(ctx, opw4);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}

			if((imm|(~0x0000FFFFFFFFFFFFLL))==imm)
			{
				opw1=0xFE00|((imm>>40)&255);
				opw2=(u16)(imm>>24);
				opw3=0xFB00|((imm>>16)&255);
				opw4=(u16)imm;

				BGBCC_JX2_EmitOpCheckRepack4(ctx,
					&opw1, &opw2, &opw3, &opw4);
				BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw2);
				BGBCC_JX2_EmitWord(ctx, opw3);
				BGBCC_JX2_EmitWord(ctx, opw4);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}
		}

#if 1
//		if(ctx->has_jumbo)
		if(ctx->has_jumbo &&
			!BGBCC_JX2_EmitCheckInhibitOp96(ctx) &&
			((ctx->tctx->optmode!=BGBCC_OPT_SIZE) ||
			ctx->is_fixed32 || ctx->op_is_wex2 || (ctx->emit_riscv&0x22)))
		{
			opw1=0xFE00|((imm>>56)&255);
			opw2=(u16)(imm>>40);
			opw3=0xFE00|((imm>>32)&255);
			opw4=(u16)(imm>>16);
			opw5=0xF800;
			opw6=(u16)imm;

			BGBCC_JX2_EmitOpCheckRepack6(ctx,
				&opw1, &opw2, &opw3, &opw4, &opw5, &opw6);
//			BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
			BGBCC_JX2_EmitPadForOpWord6(ctx,
				opw1, opw2, opw3, opw4, opw5, opw6);	
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			BGBCC_JX2_EmitWord(ctx, opw3);
			BGBCC_JX2_EmitWord(ctx, opw4);
			BGBCC_JX2_EmitWord(ctx, opw5);
			BGBCC_JX2_EmitWord(ctx, opw6);
			BGBCC_JX2_EmitPadCheckAligned(ctx);
			return(1);
		}
#endif
	}
#endif

#if 0
	if(!ctx->is_fixed32 && !ctx->no_ops48 && !ctx->op_is_wex2)
	{
	//	if(1)
	//	if(!BGBCC_JX2_CheckPadCross48(ctx))
		if(!BGBCC_JX2_CheckPadCross48(ctx) ||
			BGBCC_JX2_CheckPadCross32(ctx))
		{
			if(((u32)imm)==imm)
			{
				opw1=0xFC24;
				opw2=(u16)(imm    );
				opw3=(u16)(imm>>16);

//				BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
				BGBCC_JX2_EmitOpCheckRepack3(ctx, &opw1, &opw2, &opw3);
				BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw2);
				BGBCC_JX2_EmitWord(ctx, opw3);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}

			if(((s32)imm)==imm)
			{
				opw1=0xFC25;
				opw2=(u16)(imm    );
				opw3=(u16)(imm>>16);

//				BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
				BGBCC_JX2_EmitOpCheckRepack3(ctx, &opw1, &opw2, &opw3);
				BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw2);
				BGBCC_JX2_EmitWord(ctx, opw3);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}
		}
	}
#endif

#if 0
	if(!ctx->is_fixed32 && !ctx->op_is_wex2)
	{
		if((imm&0xFFFFFFFFULL)==0x00000000ULL)
		{
			opw1=0x3600;
			BGBCC_JX2_EmitLoadDrImm(ctx, imm>>32);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitPadCheckAligned(ctx);
			return(1);
		}
	}
#endif

#if 0
	if((imm&0xFFFFFFFFULL)==0x00000000ULL)
	{
		opw1=0x30C2;
		BGBCC_JX2_EmitLoadDrImm(ctx, imm>>32);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}

	if((imm&0xFFFFFFFFULL)==0xFFFFFFFFULL)
	{
		opw1=0x30D2;
		BGBCC_JX2_EmitLoadDrImm(ctx, imm>>32);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}
#endif

#if 0
	if((imm&0xFFFFF)==0x00000)
	{
		opw1=0x30A2;
		BGBCC_JX2_EmitLoadDrImm(ctx, imm>>20);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}

	if((imm&0xFFFFF)==0xFFFFF)
	{
		opw1=0x30B2;
		BGBCC_JX2_EmitLoadDrImm(ctx, imm>>20);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}
#endif

#if 0
	if(!ctx->is_fixed32 && !ctx->op_is_wex2)
	{
		if((imm&0xFFFF)==0x0000)
		{
//			opw1=0x30A0;
			opw1=0x340C;
			BGBCC_JX2_EmitLoadDrImm(ctx, imm>>16);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitPadCheckAligned(ctx);
			return(1);
		}

#if 0
		if((imm&0xFFFF)==0xFFFF)
		{
			opw1=0x30B0;
			BGBCC_JX2_EmitLoadDrImm(ctx, imm>>16);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitPadCheckAligned(ctx);
			return(1);
		}
#endif
	}
#endif

#if 0
	if((imm&0xFFF)==0x000)
	{
		opw1=0x3082;
		BGBCC_JX2_EmitLoadDrImm(ctx, imm>>12);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}

	if((imm&0xFFF)==0xFFF)
	{
		opw1=0x3092;
		BGBCC_JX2_EmitLoadDrImm(ctx, imm>>12);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}
#endif

#if 0
//	if(!BGBCC_JX2_CheckPadCross48(ctx))
//	if(!BGBCC_JX2_CheckPadCross48(ctx) ||
//		BGBCC_JX2_CheckPadCross32(ctx))
	if(1)
	{
		opw1=0xFC26;
		opw2=(u16)(imm    );
		opw3=(u16)(imm>>16);
		BGBCC_JX2_EmitLoadDrImm(ctx, imm>>32);

		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw2);
		BGBCC_JX2_EmitWord(ctx, opw3);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}
#endif

	if(ctx->is_fixed32 || ctx->no_ops48 ||
		ctx->op_is_wex2 || (ctx->emit_riscv&0x22))
	{
#if 1
		if(!ctx->is_fixed32 && !ctx->op_is_wex2)
		{
			if((((imm>>8)&16777215)==(imm>>8)) ||
				(((imm>>8)|(~16777215))==(imm>>8)))
			{
				BGBCC_JX2_EmitLoadDrImm(ctx, imm>>8);

				opw1=0x2600|((imm>> 0)&255);
				BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}
		}
#endif

		BGBCC_JX2_EmitLoadDrImm(ctx, imm>>16);

//		opw1=0xF000|((imm>> 8)&255);
//		opw2=0x2600|((imm>> 0)&255);
		opw1=0xF860;
		opw2=imm&65535;

		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2, 0);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw2);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}

#if 1
	if(1)
	{
		BGBCC_JX2_EmitLoadDrImm(ctx, imm>>32);

#if 0
		if(!BGBCC_JX2_CheckPadCross48(ctx) ||
			BGBCC_JX2_CheckPadCross32(ctx))
		{
			opw1=0xFC26;
			opw2=(u16)(imm    );
			opw3=(u16)(imm>>16);

			BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
			BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			BGBCC_JX2_EmitWord(ctx, opw3);
			BGBCC_JX2_EmitPadCheckAligned(ctx);
			return(1);
		}
#endif

#if 0
		opw1=0xF000|((imm>>24)&255);
		opw2=0x2600|((imm>>16)&255);

		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw2);

		opw1=0xF000|((imm>> 8)&255);
		opw2=0x2600|((imm>> 0)&255);

		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw2);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
#endif

#if 0
		opw1=0x2600|((imm>>24)&255);
		opw2=0x2600|((imm>>16)&255);

		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw2);

		opw1=0x2600|((imm>> 8)&255);
		opw2=0x2600|((imm>> 0)&255);

		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw2);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
#endif

#if 1
		opw1=0xF860;
		opw2=(imm>>16)&65535;

		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2, 0);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw2);

		opw1=0xF860;
		opw2=(imm>> 0)&65535;

		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2, 0);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw2);

		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
#endif
	}
#endif

#if 1
	opw1=0x2600|(imm&255);
	BGBCC_JX2_EmitLoadDrImm(ctx, imm>>8);
	BGBCC_JX2_EmitWord(ctx, opw1);
	BGBCC_JX2_EmitPadCheckAligned(ctx);
	return(1);
#endif
}


#if 1
int BGBCC_JX2_EmitLoadRegImm(
	BGBCC_JX2_Context *ctx, int nmid, int reg, s32 imm)
{
	return(BGBCC_JX2_EmitLoadRegImm64P(ctx, reg, imm));
}
#endif

#if 0
int BGBCC_JX2_EmitLoadRegImm(
	BGBCC_JX2_Context *ctx, int nmid, int reg, s32 imm)
{
	u32 imm_f32;
	u16 imm_f16;
	int rt1;
	int opw1, opw2, opw3, opw4;

//	ctx->test_lclalign=3;

	BGBCC_JX2DA_EmitLoadRegImm(ctx, nmid, reg, imm);

#if 1
	if(!(ctx->is_fixed32&16))
	{
		if((reg==BGBCC_SH_REG_R0) ||
			(reg==BGBCC_SH_REG_RD0) ||
			(reg==BGBCC_SH_REG_RQ0) ||
			(reg==BGBCC_SH_REG_DLR))
		{
			BGBCC_JX2_EmitLoadDrImm(ctx, imm);
			return(1);
		}
	}
#endif

	if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
//	if(0)
	{
//		ctx->test_lclalign=3;
//		BGBCC_JX2_EmitLoadDrImm(ctx, imm);

		if(!ctx->is_fixed32 && !ctx->op_is_wex2 && !(ctx->emit_riscv&0x22))
		{
			if(((sbyte)imm)==imm)
			{
//				opw1=0xE000|((reg&15)<<8)|(imm&255);
				opw1=0xD000|((reg&15)<<8)|(imm&255);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}
		}

#if 1
		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((((s16)imm)==imm) || (((u16)imm)==imm))
			{
				opw1=0xF800|(reg&31)|((imm>>16)&0x0020);
				opw2=(imm&65535);

				BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
				BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw2);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}

#if 1
			opw1=-1;

			if((opw1<0) && (imm==(((imm>>22)&1023)<<22)))
			{	opw1=0xF203|((reg&15)<<4);
				opw2=0xC000|((reg&16)<<6)|((imm>>22)&1023);	}

//			if((opw1<0) && (imm==(((imm>>54)&1023)<<54)))
//			{	opw1=0xF203|((reg&15)<<4);
//				opw2=0xC800|((reg&16)<<6)|((imm>>54)&1023);	}

			if(opw1>=0)
			{
				BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
				BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw2);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}
#endif
		}
#endif

#if 1
		if(!ctx->is_fixed32 && !ctx->no_ops48 && !ctx->op_is_wex2 &&
			!(ctx->emit_riscv&0x22))
		{
	//		if(!BGBCC_JX2_CheckPadCross48(ctx))
			if(!BGBCC_JX2_CheckPadCross48(ctx) ||
				BGBCC_JX2_CheckPadCross32(ctx))
			{
//				if(((s32)imm)==imm)
				if(((u32)imm)==imm)
				{
					opw1=0xFCE0|(reg&15);
					opw2=((imm    )&65535);
					opw3=((imm>>16)&65535);

					BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
					BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
					BGBCC_JX2_EmitWord(ctx, opw1);
					BGBCC_JX2_EmitWord(ctx, opw2);
					BGBCC_JX2_EmitWord(ctx, opw3);
					BGBCC_JX2_EmitPadCheckAligned(ctx);
					return(1);
				}
			}
		}

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			opw1=0xF860|(reg&31);
			opw2=(imm&65535);
			BGBCC_JX2_EmitLoadRegImm(ctx, nmid, reg, imm>>16);

			BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
			BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			BGBCC_JX2_EmitPadCheckAligned(ctx);
			return(1);
		}
#endif

#if 1
		if(ctx->has_jumbo && !ctx->op_is_wex2)
		{
			opw1=-1;
			opw2=-1;
			opw3=-1;
			opw4=-1;

#if 0		
			if(((u32)imm)==imm)
			{
//				opw1=0xF400|((imm>>24)&0x00FF);
//				opw2=0xC000|((imm>>10)&0x3FFF);
//				opw3=0xF200|((reg&15)<<4);
//				opw4=0xC000|((reg&16)<<6)|(imm&0x3FF);
			}
#endif

//			if((imm|0xFFFFFFFF00000000ULL)==imm)
			if(1)
			{
//				opw1=0xF400|((imm>>24)&0x00FF);
//				opw2=0xC000|((imm>>10)&0x3FFF);
//				opw3=0xF201|((reg&15)<<4);
//				opw4=0xC000|((reg&16)<<6)|(imm&0x3FF);

//				opw1=0xFE00|((imm>>24)&0x00FF);
//				opw2=0x0000|((imm>>8)&0xFFFF);
//				opw3=0xF200|((reg&15)<<4)|((imm>>31)&1);
//				opw4=0xC000|((reg&16)<<6)|(imm&0x0FF);
			}
			
			if(opw1>=0)
			{
//				BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
//				BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw1);

				if(opw2>=0)
					BGBCC_JX2_EmitWord(ctx, opw2);
				if(opw3>=0)
					BGBCC_JX2_EmitWord(ctx, opw3);
				if(opw4>=0)
					BGBCC_JX2_EmitWord(ctx, opw4);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}
		}
#endif


		if(ctx->is_fixed32 || ctx->op_is_wex2 || (ctx->emit_riscv&0x22))
		{
			BGBCC_DBGBREAK
		}

//		opw1=0xD000|((reg&15)<<8)|(imm&255);
//		BGBCC_JX2_EmitLoadRegImm(ctx, nmid, reg, imm>>8);

//		opw1=0x4800|((reg&15)<<4)|(imm&15);
//		BGBCC_JX2_EmitLoadDrImm(ctx, imm>>4);
//		BGBCC_JX2_EmitWord(ctx, opw1);
//		return(1);

		opw1=0x1800|((reg&15)<<4);
		BGBCC_JX2_EmitLoadDrImm(ctx, imm);
		BGBCC_JX2_EmitWord(ctx, opw1);

//		ctx->test_lclalign=0;
		return(1);
	}

	if(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
	{
#if 0
		if(!ctx->is_fixed32 && !ctx->op_is_wex2)
		{
			if(((imm<<27)>>27)==imm)
			{
//				opw1=0xE000|((reg&15)<<8)|(imm&255);
				opw1=0x7400|((reg&15)<<4)|(imm&0x010F);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}
		}
#endif

#if 1
		if(!ctx->is_fixed32 && !ctx->op_is_wex2 && !(ctx->emit_riscv&0x22))
		{
			if((imm&15)==imm)
			{
//				opw1=0x2B00|((reg&15)<<4)|(imm&15);
//				opw1=0x2900|((reg&15)<<4)|(imm&15);
				opw1=0x6A00|((reg&15)<<4)|(imm&15);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}

			if((imm|(~15))==imm)
			{
//				opw1=0x2B00|((reg&15)<<4)|(imm&15);
				opw1=0x6B00|((reg&15)<<4)|(imm&15);
//				opw1=0x2900|((reg&15)<<4)|(imm&15);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}
		}
#endif


		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((((s16)imm)==imm) || (((u16)imm)==imm))
			{
				opw1=0xF800|(reg&31)|((imm>>16)&0x0020);
				opw2=(imm&65535);

				BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
				BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw2);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}
		}

		if(!ctx->is_fixed32 && !ctx->no_ops48 && !ctx->op_is_wex2 &&
			!(ctx->emit_riscv&0x22))
		{
	//		if(!BGBCC_JX2_CheckPadCross48(ctx))
			if(!BGBCC_JX2_CheckPadCross48(ctx) ||
				BGBCC_JX2_CheckPadCross32(ctx))
			{
//				if(((s32)imm)==imm)
				if(((u32)imm)==imm)
				{
		//			opw1=0xFDE0|(reg&15);
					opw1=0xFCE0|(reg&15)|((reg&16)<<4);
					opw2=((imm    )&65535);
					opw3=((imm>>16)&65535);

					BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
					BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
					BGBCC_JX2_EmitWord(ctx, opw1);
					BGBCC_JX2_EmitWord(ctx, opw2);
					BGBCC_JX2_EmitWord(ctx, opw3);
					BGBCC_JX2_EmitPadCheckAligned(ctx);
					return(1);
				}
			}
		}

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			opw1=0xF860|(reg&31);
			opw2=(imm&65535);
			BGBCC_JX2_EmitLoadRegImm(ctx, nmid, reg, imm>>16);

			BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
			BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			BGBCC_JX2_EmitPadCheckAligned(ctx);
			return(1);
		}

		if(ctx->is_fixed32 || ctx->op_is_wex2 || (ctx->emit_riscv&0x22))
		{
			BGBCC_DBGBREAK
		}

//		opw1=0xD000|((reg&15)<<8)|(imm&255);
//		opw1=0x4800|((reg&15)<<4)|(imm&15);
//		BGBCC_JX2_EmitLoadDrImm(ctx, imm>>4);
//		BGBCC_JX2_EmitWord(ctx, opw1);
//		return(1);

		opw1=0x1A00|((reg&15)<<4);
		BGBCC_JX2_EmitLoadDrImm(ctx, imm);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}

#if 0
	if(BGBCC_JX2C_EmitRegIsDpReg(NULL, ctx, reg) ||
		BGBCC_JX2C_EmitRegIsFpReg(NULL, ctx, reg))
	{
		imm_f32=imm;
		rt1=BGBCC_JX2_ConstConvFloatToHalf(imm_f32, &imm_f16);
		
		if(!imm_f32)
			rt1=0;
		
		if(rt1>0)
		{
			if(ctx->is_fixed32 || ctx->op_is_wex2 || (ctx->emit_riscv&0x22))
			{
				opw1=0xF003;
				opw2=0x9F00|((reg&15)<<4);
				BGBCC_JX2_EmitLoadDrImm(ctx, imm_f16);
				BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw2);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}

			opw1=0x9F03|((reg&15)<<4);
			BGBCC_JX2_EmitLoadDrImm(ctx, imm_f16);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitPadCheckAligned(ctx);
			return(1);
		}

		if(ctx->is_fixed32 || ctx->op_is_wex2 || (ctx->emit_riscv&0x22))
		{
			opw1=0xF000;
			opw2=0x9F00|((reg&15)<<4);
			BGBCC_JX2_EmitLoadDrImm(ctx, imm_f32);
			BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			BGBCC_JX2_EmitPadCheckAligned(ctx);
			return(1);
		}

		opw1=0x9F00|((reg&15)<<4);
		BGBCC_JX2_EmitLoadDrImm(ctx, imm_f32);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}
#endif

	BGBCC_DBGBREAK
	return(0);
}
#endif

int BGBCC_JX2_ConstConvDoubleToFloat(u64 v, u32 *rv)
{
	int exp, sgn;
	u64 fra, fra1;
	u32 v1;
	int ret;
	
	if(!v)
	{
		*rv=0;
		return(1);
	}
	
	ret=1;

	fra=v&0x000FFFFFFFFFFFFFULL;
	exp=(v>>52)&2047;
	sgn=(v>>63)&1;
	
	exp=(exp-1023)+127;
	fra1=fra>>29;
	
	if((fra1<<29)!=fra)
		ret=0;
	
	if(exp<=0)
	{
		ret=0;
		exp=0;
		fra1=0;
	}
	
	if(exp>=255)
	{
		ret=-1;
		exp=255;
		fra1=0;
	}

	v1=(sgn<<31)|(exp<<23)|fra1;
	*rv=v1;
	return(ret);
}


int BGBCC_JX2_ConstConvFloatToHalf(u32 v, u16 *rv)
{
	int exp, sgn;
	u32 fra, fra1;
	u16 v1, ve;
	int ret;
	
	if(!v)
	{
		*rv=0;
		return(1);
	}
	
	ret=1;

	fra=v&0x007FFFFF;
	exp=(v>>23)&255;
	sgn=(v>>31)&1;
	
	exp=(exp-127)+15;
	fra1=fra>>13;
	
	ve=(fra>>8)&31;
	
	if((fra1<<13)!=fra)
		ret=0;
	
	if(exp<=0)
	{
		ret=0;
		exp=0;
		fra1=0;
	}
	
	if(exp>=31)
	{
		ret=-1;
		exp=31;
		fra1=0;
	}

	v1=(sgn<<15)|(exp<<10)|fra1;
	*rv=v1;
//	*rve=ve;
	return(ret);
}

int BGBCC_JX2_ConstConvPackedFloatToHalf2x(u64 v, u32 *rv)
{
	u32 v2;
	u16 v0, v1;
	int r0, r1;
	
	r0=BGBCC_JX2_ConstConvFloatToHalf((u32)(v>> 0), &v0);
	r1=BGBCC_JX2_ConstConvFloatToHalf((u32)(v>>32), &v1);
	v2=(v1<<16)|v0;
	if((r0<=0) || (r1<=0))
	{
		*rv=v2;
		return(0);
	}
	*rv=v2;
	return(1);
}

int BGBCC_JX2_ConstConvPackedFloatToHalf4x(u64 va, u64 vb, u64 *rv)
{
	u64 v2;
	u32 v0, v1;
	int r0, r1;
	
	r0=BGBCC_JX2_ConstConvPackedFloatToHalf2x(va, &v0);
	r1=BGBCC_JX2_ConstConvPackedFloatToHalf2x(vb, &v1);
	v2=(((u64)v1)<<32)|v0;
	if((r0<=0) || (r1<=0))
	{
		*rv=v2;
		return(0);
	}
	*rv=v2;
	return(1);
}

int BGBCC_JX2_ConstConvHalfToFP8S(u16 v, byte *rv)
{
	int sg, exp, fra, ret, v1;

	if(!v)
	{
		*rv=0;
		return(1);
	}
	
	ret=1;
	sg=(v>>15)&1;
	exp=(v>>10)&31;
	fra=v&0x3FF;
	
	exp=(exp-15)+7;
	if(exp<=0)
	{
		*rv=0;
		return(0);
	}
	
	if(exp>15)
		return(-1);
	
	if((fra&0x380)!=fra)
		ret=0;
//	fra=(fra>>7);
//	fra=(fra>>7)<<1;
	fra=(fra>>6)&0xE;
	
//	v1=(sg<<7)|(exp<<3)|fra;
	v1=(exp<<4)|fra|sg;
	*rv=v1;
	return(ret);
}

int BGBCC_JX2_ConstConvHalfToFP8U(u16 v, byte *rv)
{
	int sg, exp, fra, ret, v1;

	if(!v)
	{
		*rv=0;
		return(1);
	}
	
	ret=1;
	sg=(v>>15)&1;
	exp=(v>>10)&31;
	fra=v&0x3FF;
	
	if(sg)
	{
		*rv=0;
		return(-1);
	}
	
	exp=(exp-15)+7;
	if(exp<=0)
	{
		*rv=0;
		return(0);
	}
	
	if(exp>15)
		return(-1);
	
	if((fra&0x3C0)!=fra)
		ret=0;
	fra=(fra>>6);
	
	v1=(exp<<4)|fra;
	*rv=v1;
	return(ret);
}

int BGBCC_JX2_ConstConvHalfToFP5A(u16 imm_f16)
{
	u16 imm_f16b;
	int i, j, k, ex, fr;

	j=((imm_f16-0x3000)>>8)&0x1F;
//	if(!imm_f16)
//		j=0;
	imm_f16b=0x3000+(j<<8);
//	if(!j)
//		imm_f16b=0;

	if(imm_f16==imm_f16b)
		return(j);
	return(-1);
}

int BGBCC_JX2_ConstConvHalfToFP5B(u16 imm_f16)
{
	u16 imm_f16b;
	int i, j, k, ex, fr;

	if(!imm_f16)
		return(0);

	j=((imm_f16-0x3A00)>>8)&0x0F;
	imm_f16b=0x3A00+(j<<8);
	if(imm_f16==imm_f16b)
		return(0x10+j);


	ex=(imm_f16>>10)&31;
	j=10-(ex-15);
	k=(0x400|(imm_f16&0x03FF));
	if(j>0)
	{
		fr=k&((1<<j)-1);
		k=k>>j;
	}
	else
	{
		k=k<<(-j);
		fr=0;
	}
	
	if((k>0) && (k<=15) && !fr)
	{
		return(k);
	}

	return(-1);
}

int BGBCC_JX2_ConstConvHalfToFP5C(u16 imm_f16)
{
	u16 imm_f16b;
	int i, j, k, ex, fr;

	j=((imm_f16-0x3000)>>8)&0x1F;

	if(j>=24)
		j=-1;

	imm_f16b=0x3000+(j<<8);

	if(imm_f16==imm_f16b)
		return(j);


	ex=(imm_f16>>10)&31;
	j=10-(ex-15);
	k=(0x400|(imm_f16&0x03FF));
	if(j>0)
	{
		fr=k&((1<<j)-1);
		k=k>>j;
	}
	else
	{
		k=k<<(-j);
		fr=0;
	}
	
	if((k>8) && (k<=15) && !fr)
	{
		return(24+(k-8));
	}

	return(-1);
}

int BGBCC_JX2_ConstConvHalfToFP5D(u16 imm_f16)
{
	static short tab[32]={
	0x0000, 0x3800, 0x3C00, 0x3E00,		// 0.0,  0.5,  1.0,  1.5
	0x4000, 0x4100, 0x4200, 0x4300,		// 2.0,  2.5,  3.0,  3.5
	0x4400, 0x4480, 0x4500, 0x4580,		// 4.0,  4.5,  5.0,  5.5
	0x4600, 0x4680, 0x4700, 0x4780,		// 6.0,  6.5,  7.0,  7.5
	0x4800, 0x4840, 0x4880, 0x48C0,		// 8.0,  8.5,  9.0,  9.5
	0x4900, 0x4940, 0x4980, 0x49C0,		//10.0, 10.5, 11.0, 11.5
	0x4A00, 0x4A40, 0x4A80, 0x4AC0,		//12.0, 12.5, 13.0, 13.5
	0x4B00, 0x4B40, 0x4B80, 0x4BC0		//14.0, 14.5, 15.0, 15.5
	};
	
	int i;
	for(i=0; i<32; i++)
		if(imm_f16==tab[i])
			return(i);
	return(-1);
}

int BGBCC_JX2_ConstConvV4HToV4FP8S(u64 v, u32 *rv)
{
	byte tv[4];
	u32 v1;
	int rt;
	
	rt=BGBCC_JX2_ConstConvHalfToFP8S(v>>0, tv+0);
	if(rt<=0)return(-1);
	rt=BGBCC_JX2_ConstConvHalfToFP8S(v>>16, tv+1);
	if(rt<=0)return(-1);
	rt=BGBCC_JX2_ConstConvHalfToFP8S(v>>32, tv+2);
	if(rt<=0)return(-1);
	rt=BGBCC_JX2_ConstConvHalfToFP8S(v>>48, tv+3);
	if(rt<=0)return(-1);
	
	v1=(tv[3]<<24)|(tv[2]<<16)|(tv[1]<<8)|(tv[0]<<0);
	*rv=v1;
	return(1);
}

int BGBCC_JX2_ConstConvV4HToV4FP8U(u64 v, u32 *rv)
{
	byte tv[4];
	u32 v1;
	int rt;
	
	rt=BGBCC_JX2_ConstConvHalfToFP8U(v>>0, tv+0);
	if(rt<=0)return(-1);
	rt=BGBCC_JX2_ConstConvHalfToFP8U(v>>16, tv+1);
	if(rt<=0)return(-1);
	rt=BGBCC_JX2_ConstConvHalfToFP8U(v>>32, tv+2);
	if(rt<=0)return(-1);
	rt=BGBCC_JX2_ConstConvHalfToFP8U(v>>48, tv+3);
	if(rt<=0)return(-1);
	
	v1=(tv[3]<<24)|(tv[2]<<16)|(tv[1]<<8)|(tv[0]<<0);
	*rv=v1;
	return(1);
}

u32 BGBCC_ConstConvHalfToFloat(u16 v)
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

int BGBCC_JX2_ConstIsFull64(BGBCC_JX2_Context *ctx, s64 v)
{
	if(v==((s64)((s32)v)))
		return(0);
	if(v==((s64)((u32)v)))
		return(0);

	if(v==((v>>48)<<48))
		return(0);
		
//	if(((u32)v)==0)
//		return(0);

	return(1);
}

int BGBCC_JX2_EmitLoadRegImm128P(
	BGBCC_JX2_Context *ctx, int dreg, s64 imma, s64 immb)
{
	u64 immv;
	int opw1, opw2, opw3, opw4, opw5, opw6;
	int reg, xreg, rt;

	if(BGBCC_JX2C_EmitRegIsExtLpReg(NULL, ctx, dreg))
	{
		reg=BGBCC_JX2C_MapLpRegToQgr(NULL, ctx, dreg);
		xreg=dreg&31;
	}else
	{
		reg=dreg;
		xreg=BGBCC_JX2_TryNormalizeXReg(ctx, BGBCC_SH_NMID_MOVX2, dreg);
	}

#if 1
	opw1=-1;	opw2=-1;
	opw3=-1;	opw4=-1;
	opw5=-1;	opw6=-1;

	if(ctx->has_fpim&1)
	{
		rt=BGBCC_JX2_ConstConvPackedFloatToHalf4x(imma, immb, &immv);

#if 0
		if(!immv)
		{
			/* More compact to load two zeroes. */
			rt=0;
		}
#endif
	
		if(
			!BGBCC_JX2_EmitCheckInhibitOp96(ctx) &&
			(opw1<0) &&
			(xreg>0) &&
			(rt>0))
		{
			BGBCC_JX2DA_EmitLoadRegImm64(ctx, BGBCC_SH_NMID_PLDCXH, dreg, immv);

			if(!ctx->is_simpass)
			{
				ctx->stat_const_masktot++;
			}

//			if(msk_hi>=0)
//				{ ctx->stat_const_maskjumbo++; }

			if(!ctx->is_simpass)
			{
				ctx->stat_const_jumbo96++;
				ctx->stat_const_jumbo96ph++;
			}

			if(ctx->cgen_log)
			{
				fprintf(ctx->cgen_log, "Imm96_PH: %016llX\n", immv);
			}

#if 1
			opw1=0xFE00|((immv>>56)&  255);
			opw2=0x0000|((immv>>40)&65535);
			opw3=0xFE00|((immv>>32)&  255);
			opw4=0x0000|((immv>>16)&65535);
			opw5=0xF880|(xreg&31);
			opw6=0x0000|((immv>> 0)&65535);
#endif
		}else
		{
			fprintf(ctx->cgen_log, "Imm96_PH Reject: %016llX-%016llX\n",
				immb, imma);
		}
	}

	if(opw1>=0)
	{
		BGBCC_JX2_EmitPadForOpWord6(ctx,
			opw1, opw2, opw3, opw4, opw5, opw6);	
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
		if(opw5>=0)
			BGBCC_JX2_EmitWord(ctx, opw5);
		if(opw6>=0)
			BGBCC_JX2_EmitWord(ctx, opw6);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}
#endif

	BGBCC_JX2_EmitLoadRegImm64P(ctx, reg+0, imma);
	BGBCC_JX2_EmitLoadRegImm64P(ctx, reg+1, immb);
	return(1);
}

int BGBCC_JX2_EmitLoadRegImm64P(
	BGBCC_JX2_Context *ctx, int reg, s64 imm)
{
	u32 imm_f32, imm1_f32, imm2_f32;
	u16 imm_f16, imm1_f16, imm2_f16, imm3_f16, imm4_f16;
	s32 lo, hi;
	s64 imm_fh, imm1;
	int rt1, rt2, shl, msk_hi, msk_lo;
	int opw1, opw2, opw3, opw4, opw5, opw6;
	int i, j, k;

//	ctx->test_lclalign=3;

	if((reg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_LR0)
	{
		rt1=BGBCC_SH_REG_RQ0+(reg&0x1E);
		rt2=rt1+1;
		if(reg&1)
		{
			rt1+=32;
			rt2+=32;
		}
		BGBCC_JX2_EmitLoadRegImm64P(ctx, rt1, imm);
		BGBCC_JX2_EmitLoadRegImm64P(ctx, rt2, imm>>63);
		return(0);
	}

#if 1
	if(!(ctx->is_fixed32&16))
	{
		if((reg==BGBCC_SH_REG_R0) ||
			(reg==BGBCC_SH_REG_RD0) ||
			(reg==BGBCC_SH_REG_RQ0) ||
			(reg==BGBCC_SH_REG_DLR))
		{
			BGBCC_JX2DA_EmitLoadRegImm64(ctx, BGBCC_SH_NMID_MOV, reg, imm);
			BGBCC_JX2_EmitLoadDrImm(ctx, imm);
			return(1);
		}
	}
#endif

	msk_hi=-1;
	msk_lo=-1;

	if(!ctx->is_simpass)
//	if(1)
	{
		for(msk_lo=0; msk_lo<64; msk_lo++)
			if((imm>>msk_lo)&1)
				break;
		for(msk_hi=64; msk_hi>0; msk_hi--)
			if((imm>>(msk_hi-1))&1)
				break;
		i=msk_hi-msk_lo;
		if(i>0)
		{
			imm1=((1<<msk_hi)-1) & (~((1<<msk_lo)-1));
			
			if(imm1!=imm)
			{
				msk_hi=-1;
				msk_lo=-1;
			}
		}else
		{
			msk_hi=-1;
			msk_lo=-1;
		}
		
		if(msk_hi>=0)
		{
			ctx->stat_const_maskhit++;
		}

		ctx->stat_const_masktot++;

		j=1;
		if(((imm>>48)&0x7FFF)<0x1000)
			j=0;
		if(((imm>>48)&0x7FFF)>0x7000)
			j=0;
		if(j)
			ctx->stat_fp16_isfpa++;

		if(((imm>>12)>=0) && ((imm>>12)<512) && !(imm&4095) &&
				(((s16)imm)!=imm) && (((u16)imm)!=imm))
			ctx->stat_mask_imm10u_sh12++;
		if(((imm>>16)>=0) && ((imm>>16)<512) && !(imm&65535))
			ctx->stat_mask_imm10u_sh16++;

#if 1
		rt1=BGBCC_JX2_ConstConvDoubleToFloat(imm, &imm_f32);
		rt2=BGBCC_JX2_ConstConvFloatToHalf(imm_f32, &imm_f16);
		if((rt1>0) && (rt2>0) && (imm!=0))
		{
			j=((((imm_f16)>>10)&31)-15)+3;
			if((j<0) || (j>7))
				j=-1;
			if(!imm_f16)
				j=0;
			if(j>=0)
			{
				k=imm_f16&0x7F00;
			}
			
			ctx->stat_fp16_exp[(imm_f16>>10)&31]++;
		
			ctx->stat_fp16_tot++;
//			if(k==imm_f16)
//			{
//				ctx->stat_fp16_hit5++;
//			}

			k=BGBCC_JX2_ConstConvHalfToFP5A(imm_f16);
			if((k>=0) && (k<=31))
			{
				ctx->stat_fp16_hit5++;
			}

//			j=((((imm_f16)>>10)&31)-15)+2;
//			k=imm_f16&0x7F00;
//			if((j<0) || (j>3))
//				k=-1;			
//			if(k==imm_f16)
//			{
//				ctx->stat_fp16_hit5b++;
//			}

//			k=BGBCC_JX2_ConstConvHalfToFP5B(imm_f16);
//			k=BGBCC_JX2_ConstConvHalfToFP5C(imm_f16);
			k=BGBCC_JX2_ConstConvHalfToFP5D(imm_f16);
			if((k>=0) && (k<=31))
			{
				ctx->stat_fp16_hit5b++;
			}
		}
#endif
	}

	if(!ctx->is_simpass && ctx->tctx->cur_vop && ctx->tctx->cur_vop->prd)
	{
		k=-1;
	}

//	BGBCC_JX2DA_EmitLoadRegImm64(ctx, BGBCC_SH_NMID_MOVQ, reg, imm);
	BGBCC_JX2DA_EmitLoadRegImm64(ctx, BGBCC_SH_NMID_MOV, reg, imm);

#if 0
	if((reg==BGBCC_SH_REG_R0) || (reg==BGBCC_SH_REG_DLR))
	{
		BGBCC_JX2_EmitLoadDrImm(ctx, imm);
		return(1);
	}
#endif

#if 0
	if(BGBCC_JX2C_EmitRegIsLpReg(NULL, ctx, reg))
	{
		lo=imm;		hi=imm>>32;
		return(BGBCC_JX2_EmitLoadRegImmPair32(ctx, reg, lo, hi));
	}
#endif

#if 1
	if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
	{
		if(!ctx->is_fixed32 && !ctx->op_is_wex2 && !(ctx->emit_riscv&0x22))
		{
			if(((sbyte)imm)==imm)
			{
//				opw1=0xE000|((reg&15)<<8)|(imm&255);
				opw1=0xD000|((reg&15)<<8)|(imm&255);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}
		}
	}
#endif

	if(	BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg) &&
		BGBCC_JX2_EmitCheckRegExt5(ctx, reg))
	{
#if 1
		if((ctx->is_fixed32&2) || (ctx->emit_riscv&0x22))
		{
			if((imm&1023)==imm)
			{
				opw1=0x4F200|((reg&15)<<4);
				opw2=0x5C000|(imm&0x03FF);
				if(BGBCC_JX2_EmitCheckRegExt5(ctx, reg))	opw1&=~0x8000;
				if(BGBCC_JX2_EmitCheckRegExt4(ctx, reg))	opw2|= 0x0400;
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw2);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}

			if((imm|(~1023))==imm)
			{
				opw1=0x4F201|((reg&15)<<4);
				opw2=0x5C000|(imm&0x03FF);
				if(BGBCC_JX2_EmitCheckRegExt5(ctx, reg))	opw1&=~0x8000;
				if(BGBCC_JX2_EmitCheckRegExt4(ctx, reg))	opw2|= 0x0400;
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw2);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}

			if(((imm|(~65535LL))==imm) || (((u16)imm)==imm))
			{
				opw1=0x47800|(reg&31)|((imm>>16)&0x0020);
				opw2=0x50000|(imm&65535);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw2);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}

#if 1
			if(ctx->abi_spillpad&32)
			{
				if((imm&0x3FF0000)==imm)
				{
//					opw1=0x4F202|((reg&15)<<4);
					opw1=0x4F200|((reg&15)<<4);
					opw2=0x5C800|((imm>>16)&0x03FF);
					if(BGBCC_JX2_EmitCheckRegExt5(ctx, reg))	opw1&=~0x8000;
					if(BGBCC_JX2_EmitCheckRegExt4(ctx, reg))	opw2|= 0x0400;
					BGBCC_JX2_EmitWord(ctx, opw1);
					BGBCC_JX2_EmitWord(ctx, opw2);
					BGBCC_JX2_EmitPadCheckAligned(ctx);
					return(1);
				}

#if 1
				if(((imm&0x0000000003FF0000LL)|0xFFFFFFFFFC000000LL)==imm)
				{
					opw1=0x4F201|((reg&15)<<4);
					opw2=0x5C800|((imm>>16)&0x03FF);
					if(BGBCC_JX2_EmitCheckRegExt5(ctx, reg))	opw1&=~0x8000;
					if(BGBCC_JX2_EmitCheckRegExt4(ctx, reg))	opw2|= 0x0400;
					BGBCC_JX2_EmitWord(ctx, opw1);
					BGBCC_JX2_EmitWord(ctx, opw2);
					BGBCC_JX2_EmitPadCheckAligned(ctx);
					return(1);
				}
#endif
			}
#endif

#if 1
//			if(opw1<0)
//			if((reg&31)!=15)
			if((reg&63)!=15)
//			if(1)
			{
				rt1=BGBCC_JX2_ConstConvDoubleToFloat(imm, &imm_f32);
				rt2=BGBCC_JX2_ConstConvFloatToHalf(imm_f32, &imm_f16);
				if((rt1>0) && (rt2>0))
				{
					opw1=0x7880|(reg&31);
					opw2=(u16)imm_f16;

					BGBCC_JX2_EmitWord(ctx, opw1);
					BGBCC_JX2_EmitWord(ctx, opw2);
					BGBCC_JX2_EmitPadCheckAligned(ctx);
					return(1);
				}
			}
#endif
		}
#endif

#if 1
		if(	((imm&1023)==imm) &&
			!(ctx->op_is_wex2&12) && !(ctx->is_fixed32&2) &&
			!(ctx->emit_riscv&0x22))
		{
			opw1=0x49000|((reg&15)<<4);
			opw2=0x5C000|(imm&0x03FF);
			if(BGBCC_JX2_EmitCheckRegExt5(ctx, reg))	opw1|=0x0400;
			if(BGBCC_JX2_EmitCheckRegExt4(ctx, reg))	opw2|=0x0400;
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			BGBCC_JX2_EmitPadCheckAligned(ctx);
			return(1);
		}

		if(	((imm|(~1023))==imm) &&
			!(ctx->op_is_wex2&12) && !(ctx->is_fixed32&2) &&
				!(ctx->emit_riscv&0x22))
		{
			opw1=0x49001|((reg&15)<<4);
			opw2=0x5C000|(imm&0x03FF);
			if(BGBCC_JX2_EmitCheckRegExt5(ctx, reg))	opw1|=0x0400;
			if(BGBCC_JX2_EmitCheckRegExt4(ctx, reg))	opw2|=0x0400;
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			BGBCC_JX2_EmitPadCheckAligned(ctx);
			return(1);
		}
#endif

#if 1
//		if((((u32)imm)==imm) || (((s32)imm)==imm))
		if((((u32)imm)==imm) || ((imm|0xFFFFFFFF00000000LL)==imm))
		{
			if(msk_hi>=0)
				{ ctx->stat_const_maskjumbo++; }

			if(!ctx->is_simpass)
				{ ctx->stat_const_jumbo64++; }

#if 1
			if((ctx->is_fixed32&2) || !BGBCC_JX2_EmitCheckRegExt5(ctx, reg))
			{
				opw1=0x4FE00|((imm>>24)&0x00FF);
				opw2=0x50000|((imm>> 8)&0xFFFF);
				opw3=0x4F200|((reg&15)<<4);
				opw4=0x5C000|(imm&0x00FF);
				if(BGBCC_JX2_EmitCheckRegExt5(ctx, reg))
					{ opw3^=0x8000; }
				if(BGBCC_JX2_EmitCheckRegExt4(ctx, reg))
					{ opw4|=0x0400; }
				if(imm<0)	
					{ opw4|=0x0100; }
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw2);
				BGBCC_JX2_EmitWord(ctx, opw3);
				BGBCC_JX2_EmitWord(ctx, opw4);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}
#endif

			opw1=0x4FF00;
			opw2=0x50000|((imm>>16)&0xFFFF);
			opw3=0x4F800|(reg&31);
			opw4=0x50000|(imm&0xFFFF);
			if(BGBCC_JX2_EmitCheckRegExt5(ctx, reg))
				{ opw1|=0x0040; }
			if(imm<0)	
				{ opw1|=0x0010; opw3|=0x0020; }
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			BGBCC_JX2_EmitWord(ctx, opw3);
			BGBCC_JX2_EmitWord(ctx, opw4);
			BGBCC_JX2_EmitPadCheckAligned(ctx);
			return(1);
		}
#endif

#if 1
		rt1=BGBCC_JX2_ConstConvDoubleToFloat(imm, &imm_f32);
		if(rt1>0)
		{
			if(msk_hi>=0)
				{ ctx->stat_const_maskjumbo++; }

			if(!ctx->is_simpass)
			{
				ctx->stat_const_jumbo64++;
				ctx->stat_const_jumbo64_f32++;
			}

			opw1=0x4FF00;
			opw2=0x50000|((imm_f32>>16)&0xFFFF);
			opw3=0x4F880|(reg&31);
			opw4=0x50000|(imm_f32&0xFFFF);

			if(BGBCC_JX2_EmitCheckRegExt5(ctx, reg))
				{ opw1|=0x0040; }

			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			BGBCC_JX2_EmitWord(ctx, opw3);
			BGBCC_JX2_EmitWord(ctx, opw4);
			BGBCC_JX2_EmitPadCheckAligned(ctx);
			return(1);
		}
#endif

		if(msk_hi>=0)
			{ ctx->stat_const_maskjumbo++; }

		if(!ctx->is_simpass)
			{ ctx->stat_const_jumbo96++; }


#if 1
		opw1=0x4FE00|((imm>>56)&  255);
		opw2=0x50000|((imm>>40)&65535);
		opw3=0x4FE00|((imm>>32)&  255);
		opw4=0x50000|((imm>>16)&65535);
//		opw5=0x4F820|(reg&31);
		opw5=0x4F800|(reg&63);
		opw6=0x50000|((imm>> 0)&65535);

		BGBCC_JX2_EmitPadForOpWord6(ctx,
			opw1, opw2, opw3, opw4, opw5, opw6);	

		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw2);
		BGBCC_JX2_EmitWord(ctx, opw3);
		BGBCC_JX2_EmitWord(ctx, opw4);
		BGBCC_JX2_EmitWord(ctx, opw5);
		BGBCC_JX2_EmitWord(ctx, opw6);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
#endif
	}

	if(	BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg) &&
		!BGBCC_JX2_EmitCheckRegExt5(ctx, reg))
//	if(0)
	{
#if 1
//		if(!ctx->is_fixed32 && !ctx->op_is_wex2)
		if(!ctx->is_fixed32 && !ctx->op_is_wex2 && !(ctx->emit_riscv&0x22))
		{
			if(((imm&15)==imm) && (reg&16))
			{
				opw1=0x6A00|((reg&15)<<4)|(imm&15);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}

			if(((imm|(~15))==imm) && (reg&16))
			{
				opw1=0x6B00|((reg&15)<<4)|(imm&15);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}
		}
#endif
	
		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
#if 1
			if((imm&1023)==imm)
			{
				opw1=0x4F200|((reg&15)<<4);
				opw2=0x5C000|(imm&0x03FF);
				if(BGBCC_JX2_EmitCheckRegExt4(ctx, reg))	opw2|=0x0400;
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw2);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}

			if((imm|(~1023))==imm)
			{
				opw1=0x4F201|((reg&15)<<4);
				opw2=0x5C000|(imm&0x03FF);
				if(BGBCC_JX2_EmitCheckRegExt4(ctx, reg))	opw2|=0x0400;
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw2);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}
#endif

//			if((((s16)imm)==imm) || (((u16)imm)==imm))
			if(((imm|(~65535LL))==imm) || (((u16)imm)==imm))
			{
				opw1=0xF800|(reg&31)|((imm>>16)&0x0020);
				opw2=(imm&65535);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw2);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}

#if 1
			if(ctx->abi_spillpad&32)
			{
				if((imm&0x3FF0000)==imm)
				{
//					opw1=0x4F202|((reg&15)<<4);
					opw1=0x4F200|((reg&15)<<4);
					opw2=0x5C800|((imm>>16)&0x03FF);
	//				if(BGBCC_JX2_EmitCheckRegExt5(ctx, reg))	opw1&=~0x8000;
					if(BGBCC_JX2_EmitCheckRegExt4(ctx, reg))	opw2|= 0x0400;
					BGBCC_JX2_EmitWord(ctx, opw1);
					BGBCC_JX2_EmitWord(ctx, opw2);
					BGBCC_JX2_EmitPadCheckAligned(ctx);
					return(1);
				}

#if 1
				if(((imm&0x0000000003FF0000LL)|0xFFFFFFFFFC000000LL)==imm)
				{
					opw1=0x4F201|((reg&15)<<4);
					opw2=0x5C800|((imm>>16)&0x03FF);
//					if(BGBCC_JX2_EmitCheckRegExt5(ctx, reg))	opw1&=~0x8000;
					if(BGBCC_JX2_EmitCheckRegExt4(ctx, reg))	opw2|= 0x0400;
					BGBCC_JX2_EmitWord(ctx, opw1);
					BGBCC_JX2_EmitWord(ctx, opw2);
					BGBCC_JX2_EmitPadCheckAligned(ctx);
					return(1);
				}
#endif
			}
#endif

			opw1=-1;

#if 1
			if((opw1<0) && (imm==(((imm>>22)&1023)<<22)))
			{	opw1=0xF203|((reg&15)<<4);
				opw2=0xC000|((reg&16)<<6)|((imm>>22)&1023);	}

			if((opw1<0) && (imm==(((imm>>54)&1023)<<54)))
			{	opw1=0xF203|((reg&15)<<4);
				opw2=0xC800|((reg&16)<<6)|((imm>>54)&1023);	}
#endif

#if 0

#if 1
			if((opw1<0) && (imm==(((imm>>0)&255)<< 0)))
			{	opw1=0xF204|((reg&15)<<4);
				opw2=0xD000|((reg&16)<<6)|((imm>> 0)&255);	}
			if((opw1<0) && (imm==(((imm>>8)&255)<< 8)))
			{	opw1=0xF204|((reg&15)<<4);
				opw2=0xD100|((reg&16)<<6)|((imm>> 8)&255);	}
			if((opw1<0) && (imm==(((imm>>16)&255)<<16)))
			{	opw1=0xF204|((reg&15)<<4);
				opw2=0xD200|((reg&16)<<6)|((imm>>16)&255);	}
			if((opw1<0) && (imm==(((imm>>24)&255)<<24)))
			{	opw1=0xF204|((reg&15)<<4);
				opw2=0xD300|((reg&16)<<6)|((imm>>24)&255);	}
#endif

#if 1
			if((opw1<0) && (imm==(((imm>>32)&255)<<32)))
			{	opw1=0xF204|((reg&15)<<4);
				opw2=0xD800|((reg&16)<<6)|((imm>>32)&255);	}
			if((opw1<0) && (imm==(((imm>>40)&255)<<40)))
			{	opw1=0xF204|((reg&15)<<4);
				opw2=0xD900|((reg&16)<<6)|((imm>>40)&255);	}
			if((opw1<0) && (imm==(((imm>>48)&255)<<48)))
			{	opw1=0xF204|((reg&15)<<4);
				opw2=0xDA00|((reg&16)<<6)|((imm>>48)&255);	}
			if((opw1<0) && (imm==(((imm>>56)&255)<<56)))
			{	opw1=0xF204|((reg&15)<<4);
				opw2=0xDB00|((reg&16)<<6)|((imm>>56)&255);	}
#endif

#if 1
			if((opw1<0) && (imm==(((imm>>0)|(~255))<<0)))
			{	opw1=0xF205|((reg&15)<<4);
				opw2=0xD000|((reg&16)<<6)|((imm>>0)&255);	}
			if((opw1<0) && (imm==(((imm>>8)|(~255))<<8)))
			{	opw1=0xF205|((reg&15)<<4);
				opw2=0xD100|((reg&16)<<6)|((imm>>8)&255);	}
			if((opw1<0) && (imm==(((imm>>16)|(~255))<<16)))
			{	opw1=0xF205|((reg&15)<<4);
				opw2=0xD200|((reg&16)<<6)|((imm>>16)&255);	}
			if((opw1<0) && (imm==(((imm>>24)|(~255))<<24)))
			{	opw1=0xF205|((reg&15)<<4);
				opw2=0xD300|((reg&16)<<6)|((imm>>24)&255);	}
#endif

#if 0
			if((opw1<0) && (imm==(((imm>>32)|(~255))<<32)))
//			if(imm==(((imm>>32)|(-256LL))<<32))
			{	opw1=0xF205|((reg&15)<<4);
				opw2=0xD800|((reg&16)<<6)|((imm>>32)&255);	}
			if((opw1<0) && (imm==(((imm>>40)|(~255))<<40)))
//			if(imm==(((imm>>40)|(-256LL))<<40))
			{	opw1=0xF205|((reg&15)<<4);
				opw2=0xD900|((reg&16)<<6)|((imm>>40)&255);	}
#endif

#if 0
			if((opw1<0) && (imm==(((imm>>48)|(~255))<<48)))
//			if(imm==(((imm>>48)|(-256LL))<<48))
			{	opw1=0xF205|((reg&15)<<4);
				opw2=0xDA00|((reg&16)<<6)|((imm>>48)&255);	}
			if((opw1<0) && (imm==(((imm>>56)|(~255))<<56)))
//			if(imm==(((imm>>56)|(-256LL))<<56))
			{	opw1=0xF205|((reg&15)<<4);
				opw2=0xDB00|((reg&16)<<6)|((imm>>56)&255);	}
#endif

#endif

#if 1
			if(opw1<0)
			{
				rt1=BGBCC_JX2_ConstConvDoubleToFloat(imm, &imm_f32);
				rt2=BGBCC_JX2_ConstConvFloatToHalf(imm_f32, &imm_f16);
				if((rt1>0) && (rt2>0))
				{
					opw1=0xF880|(reg&31);
					opw2=(u16)imm_f16;
				}
			}
#endif
			
			if(opw1>=0)
			{
				BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2, 0);
				BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw2);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}
		}

#if 1
//		if(ctx->has_jumbo && !ctx->op_is_wex2)
		if(ctx->has_jumbo)
//		if(0)
		{
			opw1=-1;
			opw2=-1;
			opw3=-1;
			opw4=-1;
			opw5=-1;
			opw6=-1;
		
			if((opw1<0) && (((u32)imm)==imm))
			{
				if(msk_hi>=0)
					{ ctx->stat_const_maskjumbo++; }

				if(!ctx->is_simpass)
				{
					ctx->stat_const_jumbo64++;
					ctx->stat_const_jumbo64_imm33l++;
				}

//				opw1=0xF400|((imm>>24)&0x00FF);
//				opw2=0xC000|((imm>>10)&0x3FFF);
//				opw3=0xF200|((reg&15)<<4);
//				opw4=0xC000|((reg&16)<<6)|(imm&0x3FF);

				opw1=0xFE00|((imm>>24)&0x00FF);
				opw2=0x0000|((imm>> 8)&0xFFFF);
				opw3=0xF200|((reg&15)<<4);
				opw4=0xC000|((reg&16)<<6)|(imm&0x0FF);
			}

			if((opw1<0) && ((imm|0xFFFFFFFF00000000ULL)==imm))
			{
				if(msk_hi>=0)
					{ ctx->stat_const_maskjumbo++; }

				if(!ctx->is_simpass)
				{
					ctx->stat_const_jumbo64++;
					ctx->stat_const_jumbo64_imm33l++;
				}

//				opw1=0xF400|((imm>>24)&0x00FF);
//				opw2=0xC000|((imm>>10)&0x3FFF);
//				opw3=0xF201|((reg&15)<<4);
//				opw4=0xC000|((reg&16)<<6)|(imm&0x3FF);

				opw1=0xFE00|((imm>>24)&0x00FF);
				opw2=0x0000|((imm>> 8)&0xFFFF);
				opw3=0xF201|((reg&15)<<4);
				opw4=0xC100|((reg&16)<<6)|(imm&0x0FF);
			}

#if 0
			if((opw1<0) && ((imm&0xFFFFFFFF00000000ULL)==imm))
			{
				imm1=imm>>32;
//				opw1=0xF400|((imm1>>24)&0x00FF);
//				opw2=0xC000|((imm1>>10)&0x3FFF);
//				opw3=0xF201|((reg&15)<<4);
//				opw4=0xC000|((reg&16)<<6)|(imm1&0x3FF);
//				opw5=0xF200|((reg&15)<<4)|(reg&15);
//				opw6=0x8800|((reg&16)?0x0600:0x0000)|32;
			}
#endif

#if 1
			if((opw1<0) && ((imm&0xFFFFFFFF00000000ULL)==imm))
			{
				if(msk_hi>=0)
					{ ctx->stat_const_maskjumbo++; }

				if(!ctx->is_simpass)
				{
					ctx->stat_const_jumbo64++;
					ctx->stat_const_jumbo64_imm32h++;
				}

				imm1=imm>>32;
//				opw1=0xF400|((imm1>>24)&0x00FF);
//				opw2=0xC000|((imm1>>10)&0x3FFF);
//				opw3=0xF203|((reg&15)<<4);
//				opw4=0xC800|((reg&16)<<6)|(imm1&0x3FF);

				opw1=0xFE00|((imm1>>24)&0x00FF);
				opw2=0x0000|((imm1>> 8)&0xFFFF);
				opw3=0xF203|((reg&15)<<4);
				opw4=0xC800|((reg&16)<<6)|(imm1&0x0FF);
			}
#endif

#if 1
			if((opw1<0) && ((imm&0x0000FFFFFFFF0000ULL)==imm))
			{
				if(msk_hi>=0)
					{ ctx->stat_const_maskjumbo++; }

				if(!ctx->is_simpass)
				{
					ctx->stat_const_jumbo64++;
					ctx->stat_const_jumbo64_imm32c++;
				}

				imm1=imm>>16;
//				opw1=0xF400|((imm1>>24)&0x00FF);
//				opw2=0xC000|((imm1>>10)&0x3FFF);
//				opw3=0xF203|((reg&15)<<4);
//				opw4=0xC000|((reg&16)<<6)|(imm1&0x3FF);

				opw1=0xFE00|((imm1>>24)&0x00FF);
				opw2=0x0000|((imm1>> 8)&0xFFFF);
				opw3=0xF203|((reg&15)<<4);
				opw4=0xC000|((reg&16)<<6)|(imm1&0x0FF);
			}
#endif

			if(	(opw1<0) &&
				ctx->has_bra48 &&
//				((reg&31)==0))
				((reg&63)==0))
			{
				if((imm&0x0000FFFFFFFFFFFFULL)==imm)
				{
					if(msk_hi>=0)
						{ ctx->stat_const_maskjumbo++; }

					if(!ctx->is_simpass)
						{ ctx->stat_const_jumbo64++; }


					opw1=0xFE00|((imm>>40)&  255);
					opw2=0x0000|((imm>>24)&65535);
					opw3=0xFA00|((imm>>16)&  255);
					opw4=0x0000|((imm>> 0)&65535);
				}

				if((imm|0xFFFF000000000000ULL)==imm)
				{
					if(msk_hi>=0)
						{ ctx->stat_const_maskjumbo++; }

					if(!ctx->is_simpass)
						{ ctx->stat_const_jumbo64++; }

					opw1=0xFE00|((imm>>40)&  255);
					opw2=0x0000|((imm>>24)&65535);
					opw3=0xFB00|((imm>>16)&  255);
					opw4=0x0000|((imm>> 0)&65535);
				}
			}

#if 1
			if((opw1<0) && (ctx->has_xgpr&1))
			{
				rt1=BGBCC_JX2_ConstConvDoubleToFloat(imm, &imm_f32);
//				rt2=BGBCC_JX2_ConstConvFloatToHalf(imm_f32, &imm_f16);
//				if((rt1>0) && (rt2>0))
//				{
//					opw1=0xF880|(reg&31);
//					opw2=(u16)imm_f16;
//				}

				if(rt1>0)
				{
					if(msk_hi>=0)
						{ ctx->stat_const_maskjumbo++; }

					if(!ctx->is_simpass)
					{
						ctx->stat_const_jumbo64++;
						ctx->stat_const_jumbo64_f32++;
					}

					opw1=0xFF00;
					opw2=0x0000|((imm_f32>>16)&0xFFFF);
					opw3=0xF880|(reg&31);
					opw4=0x0000|((imm_f32>> 0)&0xFFFF);
				}

#if 1
				rt1=BGBCC_JX2_ConstConvFloatToHalf((imm>>32), &imm1_f16);
				rt2=BGBCC_JX2_ConstConvFloatToHalf((imm>> 0), &imm2_f16);

				if((opw1<0) && ctx->has_simdx2 && (rt1>0) && (rt2>0))
				{
					if(msk_hi>=0)
						{ ctx->stat_const_maskjumbo++; }

					if(!ctx->is_simpass)
					{
						ctx->stat_const_jumbo64++;
						ctx->stat_const_jumbo64_2xf16++;
					}

					opw1=0xFF10;
					opw2=0x0000|(imm1_f16&0xFFFF);
					opw3=0xF880|(reg&31);
					opw4=0x0000|(imm2_f16&0xFFFF);
				}
#endif

#if 1
				rt1=BGBCC_JX2_ConstConvV4HToV4FP8S(imm, &imm1_f32);
				rt2=BGBCC_JX2_ConstConvV4HToV4FP8U(imm, &imm2_f32);

				if((opw1<0) && ctx->has_simdx2 && (rt1>0))
				{
					if(msk_hi>=0)
						{ ctx->stat_const_maskjumbo++; }

					if(!ctx->is_simpass)
					{
						ctx->stat_const_jumbo64++;
						ctx->stat_const_jumbo64_4xf8++;
					}

					opw1=0xFF20;
					opw2=0x0000|((imm1_f32>>16)&0xFFFF);
					opw3=0xF880|(reg&31);
					opw4=0x0000|((imm1_f32>> 0)&0xFFFF);
				}

				if((opw1<0) && ctx->has_simdx2 && (rt2>0))
				{
					if(msk_hi>=0)
						{ ctx->stat_const_maskjumbo++; }
					if(!ctx->is_simpass)
					{
						ctx->stat_const_jumbo64++;
						ctx->stat_const_jumbo64_4xf8++;
					}

					opw1=0xFF30;
					opw2=0x0000|((imm2_f32>>16)&0xFFFF);
					opw3=0xF880|(reg&31);
					opw4=0x0000|((imm2_f32>> 0)&0xFFFF);
				}
#endif

			}
#endif

			if(	(opw1<0) &&
				!BGBCC_JX2_EmitCheckInhibitOp96(ctx) &&
//				BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg) &&
//				BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg) &&
				BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg) &&
				BGBCC_JX2_ConstIsFull64(ctx, imm))
//				((imm&0x000000000000FFFFULL)!=0) &&
//				((imm&0x00000000FFFF0000ULL)!=0) &&
//				((imm&0x0000FFFF00000000ULL)!=0) &&
//				((imm&0xFFFF000000000000ULL)!=0))
			{
#if 0
//				opw1=0xF400|((imm>>56)&  255);
//				opw2=0xC000|((imm>>44)& 4095);
//				opw3=0xF400|((imm>>36)&  255);
//				opw4=0xC000|((imm>>24)& 4095);
//				opw5=0xFA00|((imm>>16)&  255);
//				opw6=0x0000|((imm>> 0)&65535);
//				opw2|=((reg>>2)&3)<<12;
//				opw4|=((reg>>0)&3)<<12;
//				opw5|=((reg>>4)&1)<< 8;
#endif

				if(msk_hi>=0)
					{ ctx->stat_const_maskjumbo++; }

				if(!ctx->is_simpass)
					{ ctx->stat_const_jumbo96++; }

				if(ctx->cgen_log)
				{
					fprintf(ctx->cgen_log, "Imm96: %016llX\n", imm);
				}

#if 1
				opw1=0xFE00|((imm>>56)&  255);
				opw2=0x0000|((imm>>40)&65535);
				opw3=0xFE00|((imm>>32)&  255);
				opw4=0x0000|((imm>>16)&65535);
				opw5=0xF800|(reg&31);
				opw6=0x0000|((imm>> 0)&65535);
#endif
			}

			if(opw1>=0)
			{
//				BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
//				BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
				BGBCC_JX2_EmitPadForOpWord6(ctx,
					opw1, opw2, opw3, opw4, opw5, opw6);	
				BGBCC_JX2_EmitWord(ctx, opw1);

				if(opw2>=0)
					BGBCC_JX2_EmitWord(ctx, opw2);
				if(opw3>=0)
					BGBCC_JX2_EmitWord(ctx, opw3);
				if(opw4>=0)
					BGBCC_JX2_EmitWord(ctx, opw4);
				if(opw5>=0)
					BGBCC_JX2_EmitWord(ctx, opw5);
				if(opw6>=0)
					BGBCC_JX2_EmitWord(ctx, opw6);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}
		}
#endif
	}

	if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
//	if(0)
	{

#if 1
		if(!ctx->is_fixed32 && !ctx->op_is_wex2 && !(ctx->emit_riscv&0x22))
		{
#if 0
	//		if(!BGBCC_JX2_CheckPadCross48(ctx))
			if(!BGBCC_JX2_CheckPadCross48(ctx) ||
				BGBCC_JX2_CheckPadCross32(ctx))
			{
//				if(((s32)imm)==imm)
				if(((u32)imm)==imm)
				{
					opw1=0xFCE0|(reg&15);
					opw2=((imm    )&65535);
					opw3=((imm>>16)&65535);

//					BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
					BGBCC_JX2_EmitOpCheckRepack3(ctx, &opw1, &opw2, &opw3);
					BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
					BGBCC_JX2_EmitWord(ctx, opw1);
					BGBCC_JX2_EmitWord(ctx, opw2);
					BGBCC_JX2_EmitWord(ctx, opw3);
					BGBCC_JX2_EmitPadCheckAligned(ctx);
					return(1);
				}

//				if((((s32)imm)^0xFFFFFFFF00000000LL)==imm)
				if((((s32)imm)|0xFFFFFFFF00000000LL)==imm)
				{
					opw1=0xFCF0|(reg&15);
					opw2=((imm    )&65535);
					opw3=((imm>>16)&65535);

//					BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
					BGBCC_JX2_EmitOpCheckRepack3(ctx, &opw1, &opw2, &opw3);
					BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
					BGBCC_JX2_EmitWord(ctx, opw1);
					BGBCC_JX2_EmitWord(ctx, opw2);
					BGBCC_JX2_EmitWord(ctx, opw3);
					BGBCC_JX2_EmitPadCheckAligned(ctx);
					return(1);
				}
			}
#endif

#if 1
//			if(!ctx->is_fixed32 && !ctx->op_is_wex2)
//			if(1)
			if(!ctx->op_is_wex2)
			{
				imm_fh=imm;
				shl=0;
				
				while(shl<63)
				{
					if(imm_fh&1)
						break;
					if(((imm_fh<<52)>>52)==imm_fh)
						break;
					imm_fh=imm_fh>>1;
					shl++;
				}

				if(	((((imm_fh<<52)>>52)==imm_fh) &&
						!(ctx->is_fixed32   ) && shl) ||
					((((imm_fh<<40)>>40)==imm_fh) &&
						!(ctx->is_fixed32&16) && shl) ||
					(shl>=32))
//					(shl>32))
				{
					BGBCC_JX2_EmitLoadDrImm(ctx, imm_fh);
					opw1=0xF200|((reg&15)<<4);
					opw2=0x8800|((reg&16)<<6)|shl;

					BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2, 0);
					BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
					BGBCC_JX2_EmitWord(ctx, opw1);
					BGBCC_JX2_EmitWord(ctx, opw2);
					BGBCC_JX2_EmitPadCheckAligned(ctx);
					return(1);
				}
			}
#endif

#if 0
			if(!((u32)imm))
			{
				opw1=0x3600|((reg&15)<<4);
				BGBCC_JX2_EmitLoadRegImm64P(ctx, reg, imm>>32);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}
#endif

#if 0
	//		if(!BGBCC_JX2_CheckPadCross48(ctx))
			if(!BGBCC_JX2_CheckPadCross48(ctx) ||
				BGBCC_JX2_CheckPadCross32(ctx))
			{			
				opw1=0xFCD0|(reg&15);
				opw2=(u16)(imm    );
				opw3=(u16)(imm>>16);
				BGBCC_JX2_EmitLoadRegImm64P(ctx, reg, imm>>32);

//				BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
				BGBCC_JX2_EmitOpCheckRepack3(ctx, &opw1, &opw2, &opw3);
				BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw2);
				BGBCC_JX2_EmitWord(ctx, opw3);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}
#endif
		}
#endif

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			opw1=0xF860|(reg&31);
			opw2=(imm&65535);
			BGBCC_JX2_EmitLoadRegImm64P(ctx, reg, imm>>16);

			BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2, 0);
			BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			BGBCC_JX2_EmitPadCheckAligned(ctx);
			return(1);
		}

		if(ctx->is_fixed32 || ctx->op_is_wex2 || (ctx->emit_riscv&0x22))
		{
			BGBCC_DBGBREAK
		}

#if 0
//		opw1=0xD000|((reg&15)<<8)|(imm&255);
//		BGBCC_JX2_EmitLoadRegImm(ctx, nmid, reg, imm>>8);

//		opw1=0x4800|((reg&15)<<4)|(imm&15);
//		BGBCC_JX2_EmitLoadDrImm(ctx, imm>>4);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
#endif

		opw1=0x1800|((reg&15)<<4);
		BGBCC_JX2_EmitLoadDrImm(ctx, imm);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}

	if(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
	{
		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
//			if((((s16)imm)==imm) || (((u16)imm)==imm))
			if(((imm|(-65536))==imm) || (((u16)imm)==imm))
			{
				opw1=0xF800|(reg&31)|((imm>>16)&0x0020);
				opw2=(imm&65535);

				BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2, 0);
				BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw2);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}
		}

#if 0
		if(!ctx->is_fixed32 && !ctx->op_is_wex2)
		{
#if 0
	//		if(!BGBCC_JX2_CheckPadCross48(ctx))
			if(!BGBCC_JX2_CheckPadCross48(ctx) ||
				BGBCC_JX2_CheckPadCross32(ctx))
			{
//				if(((s32)imm)==imm)
				if(((u32)imm)==imm)
				{
		//			opw1=0xFDE0|(reg&15);
					opw1=0xFCE0|(reg&15)|((reg&16)<<4);
					opw2=((imm    )&65535);
					opw3=((imm>>16)&65535);

//					BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
					BGBCC_JX2_EmitOpCheckRepack3(ctx, &opw1, &opw2, &opw3);
					BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
					BGBCC_JX2_EmitWord(ctx, opw1);
					BGBCC_JX2_EmitWord(ctx, opw2);
					BGBCC_JX2_EmitWord(ctx, opw3);
					BGBCC_JX2_EmitPadCheckAligned(ctx);
					return(1);
				}

//				if((((s32)imm)^0xFFFFFFFF00000000LL)==imm)
				if((((s32)imm)|0xFFFFFFFF00000000LL)==imm)
				{
					opw1=0xFCF0|(reg&15)|((reg&16)<<4);
					opw2=((imm    )&65535);
					opw3=((imm>>16)&65535);

//					BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
					BGBCC_JX2_EmitOpCheckRepack3(ctx, &opw1, &opw2, &opw3);
					BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
					BGBCC_JX2_EmitWord(ctx, opw1);
					BGBCC_JX2_EmitWord(ctx, opw2);
					BGBCC_JX2_EmitWord(ctx, opw3);
					BGBCC_JX2_EmitPadCheckAligned(ctx);
					return(1);
				}
			}
#endif

#if 0
	//		if(!BGBCC_JX2_CheckPadCross48(ctx))
			if(!BGBCC_JX2_CheckPadCross48(ctx) ||
				BGBCC_JX2_CheckPadCross32(ctx))
			{
		//		opw1=0xFDD0|(reg&15);
				opw1=0xFCD0|(reg&15)|((reg&16)<<4);
				opw2=(u16)(imm    );
				opw3=(u16)(imm>>16);
				BGBCC_JX2_EmitLoadRegImm64P(ctx, reg, imm>>32);

//				BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
				BGBCC_JX2_EmitOpCheckRepack3(ctx, &opw1, &opw2, &opw3);
				BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw2);
				BGBCC_JX2_EmitWord(ctx, opw3);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}
#endif
		}
#endif

#if 1
//			if(!ctx->is_fixed32 && !ctx->op_is_wex2)
//			if(1)
			if(!ctx->op_is_wex2)
			{
				imm_fh=imm;
				shl=0;
				
				while(shl<63)
				{
					if(imm_fh&1)
						break;
					if(((imm_fh<<52)>>52)==imm_fh)
						break;
					imm_fh=imm_fh>>1;
					shl++;
				}

				if(	((((imm_fh<<52)>>52)==imm_fh) &&
						!(ctx->is_fixed32   ) && shl) ||
					((((imm_fh<<40)>>40)==imm_fh) &&
						!(ctx->is_fixed32&16) && shl) ||
					(shl>=32))
//					(shl>32))
				{
					BGBCC_JX2_EmitLoadDrImm(ctx, imm_fh);
					opw1=0xF200|((reg&15)<<4);
					opw2=0x8800|((reg&16)<<6)|shl;

					BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2, 0);
					BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
					BGBCC_JX2_EmitWord(ctx, opw1);
					BGBCC_JX2_EmitWord(ctx, opw2);
					BGBCC_JX2_EmitPadCheckAligned(ctx);
					return(1);
				}
			}
#endif

#if 1
		if(ctx->is_fixed32 || ctx->op_is_wex2 || (ctx->emit_riscv&0x22))
//		if(1)
		{
			opw1=0xF860|(reg&31);
			opw2=(imm&65535);
			BGBCC_JX2_EmitLoadRegImm64P(ctx, reg, imm>>16);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			BGBCC_JX2_EmitPadCheckAligned(ctx);
			return(1);
		}
#endif

		if(reg&16)
			opw1=0x1A00|((reg&15)<<4);
		else
			opw1=0x1800|((reg&15)<<4);
		BGBCC_JX2_EmitLoadDrImm(ctx, imm);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}

#if 0
	if(BGBCC_JX2C_EmitRegIsDpReg(NULL, ctx, reg) ||
		BGBCC_JX2C_EmitRegIsFpReg(NULL, ctx, reg))
	{
		rt1=BGBCC_JX2_ConstConvDoubleToFloat(imm, &imm_f32);
		rt2=BGBCC_JX2_ConstConvFloatToHalf(imm_f32, &imm_f16);

		if(!imm)
			rt1=0;
		
		if(rt1<=0)
			rt2=0;
		if(!imm_f32)
			rt2=0;

		if(ctx->is_fixed32 || ctx->op_is_wex2 || (ctx->emit_riscv&0x22))
		{
			if(rt2>0)
			{
				opw1=0xF003;
				opw2=0x9F00|((reg&15)<<4);
				BGBCC_JX2_EmitLoadDrImm(ctx, imm_f16);
				BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw2);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}

			if(rt1>0)
			{
				opw1=0xF000;
				opw2=0x9F00|((reg&15)<<4);
				BGBCC_JX2_EmitLoadDrImm(ctx, imm_f32);
				BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
				BGBCC_JX2_EmitWord(ctx, opw1);
				BGBCC_JX2_EmitWord(ctx, opw2);
				BGBCC_JX2_EmitPadCheckAligned(ctx);
				return(1);
			}

			opw1=0xF001;
			opw2=0x9F00|((reg&15)<<4);
			BGBCC_JX2_EmitLoadDrImm(ctx, imm);
			BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			BGBCC_JX2_EmitPadCheckAligned(ctx);
			return(1);
		}

		if(rt2>0)
		{
			opw1=0x9F03|((reg&15)<<4);
			BGBCC_JX2_EmitLoadDrImm(ctx, imm_f16);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitPadCheckAligned(ctx);
			return(1);
		}

		if(rt1>0)
		{
			opw1=0x9F00|((reg&15)<<4);
			BGBCC_JX2_EmitLoadDrImm(ctx, imm_f32);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitPadCheckAligned(ctx);
			return(1);
		}

		opw1=0x9F01|((reg&15)<<4);
		BGBCC_JX2_EmitLoadDrImm(ctx, imm);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}
#endif

	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2_EmitLoadRegImmPair32(
	BGBCC_JX2_Context *ctx, int reg, s32 imma, s32 immb)
{
	int reg2;

	if(BGBCC_JX2C_EmitRegIsLpReg(NULL, ctx, reg))
	{
		reg2=BGBCC_SH_REG_R0+(reg&15);
		BGBCC_JX2_EmitLoadRegImm(ctx, 0, reg2+0, imma);
		BGBCC_JX2_EmitLoadRegImm(ctx, 0, reg2+1, immb);
		return(1);
	}

	if(BGBCC_JX2C_EmitRegIsDpReg(NULL, ctx, reg))
	{
		BGBCC_JX2_EmitLoadRegImm64P(ctx, reg,
			(((u64)immb)<<32)|((u32)imma));
		return(1);
	}

	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2_EmitCheckFlushIndexImmP(BGBCC_JX2_Context *ctx)
{
	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2_EmitCheckFlushIndexImm(BGBCC_JX2_Context *ctx)
{
	return(0);
}

int BGBCC_JX2_EmitForceFlushIndexImm(BGBCC_JX2_Context *ctx)
{
	return(0);
}

int BGBCC_JX2_EmitFlushIndexImmBasic(BGBCC_JX2_Context *ctx)
{
	return(0);
}

int BGBCC_JX2_EmitLoadRegLabelRel(
	BGBCC_JX2_Context *ctx, int reg, int lbl, int ofs)
{
	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2_EmitLoadRegLabelAbs(
	BGBCC_JX2_Context *ctx, int reg, int lbl)
{
	BGBCC_JX2_EmitOpLblReg(ctx, BGBCC_SH_NMID_MOV, lbl, reg);
//	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2_EmitLoadRegLabelRel32(
	BGBCC_JX2_Context *ctx, int reg, int lbl, int ofs)
{
	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2_EmitLoadRegLabelRel16(
	BGBCC_JX2_Context *ctx, int reg, int lbl, int ofs)
{
	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2_EmitLoadRegLabelRel24(
	BGBCC_JX2_Context *ctx, int reg, int lbl)
{
	int tr0, tr1;
	int j;

#if 0
	if(	BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg) &&
		!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
	{
		j=-1;
		
		tr0=BGBCC_JX2C_ScratchAllocReg(ctx->tctx, ctx, 
			BGBCC_SH_REGCLS_QGR);

		j=BGBCC_JX2_EmitLoadRegLabelRel24(ctx, tr0, lbl);
		BGBCC_JX2_EmitOpRegReg(ctx, BGBCC_SH_NMID_MOV,
			tr0, reg);
		BGBCC_JX2C_ScratchReleaseReg(ctx->tctx, ctx, tr0);
		return(j);
	}
#endif

	if(BGBCC_JX2C_EmitRegIsExtLpReg(NULL, ctx, reg))
	{
		tr0=BGBCC_JX2C_MapLpRegToQgr(NULL, ctx, reg);
		tr1=tr0+1;

		BGBCC_JX2_EmitOpLblReg(ctx, BGBCC_SH_NMID_MOV, lbl, tr0);
		BGBCC_JX2_EmitOpRegReg(ctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_GBH, tr1);
		return(1);
	}

	BGBCC_JX2_EmitOpLblReg(ctx, BGBCC_SH_NMID_MOV, lbl, reg);
	return(1);

//	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
//	return(0);
}

int BGBCC_JX2_TryEmitLoadRegLabelVarPbo24(
	BGBCC_JX2_Context *ctx, int nmid, int reg, int lbl)
{
	int opw1, opw2, opw3, opw4, opw5, opw6, opw7, opw8;
	int opwf, opwf1, opwf2, opwf3;
	int rlty, prlty, rlty2, usewxj;
	int tr0;
	int i, j, k;

//	ctx->test_lclalign=3;

	nmid=BGBCC_JX2_EmitRemapPseudoOp(ctx, nmid);

	if(lbl==ctx->lbl_gbl_ptr)
	{
		opw1=-1;	opw2=-1;
		opw3=-1;	opw4=-1;
		opw5=-1;	opw6=-1;
		opw7=-1;	opw8=-1;

		if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
			{ BGBCC_DBGBREAK }

#if 1
		opw5=0xF121;
		if(ctx->is_addr_x32)
			opw6=0xA000;
		else
			opw6=0xB000;
//		BGBCC_JX2_EmitWord(ctx, opw1);
//		BGBCC_JX2_EmitWord(ctx, opw2);
#endif

#if 1
		if(ctx->has_jumbo)
		{
			rlty=BGBCC_SH_RLC_TBR24_BJX;
			opw1=0xFE00;
			opw2=0x0000;
			opw3=0xF102|((reg&15)<<4);
			if(ctx->is_addr_x32)
				opw4=0xA000|((reg&0x10)<<6);
			else
				opw4=0xB000|((reg&0x10)<<6);
		}else
		{
			rlty=BGBCC_SH_RLC_TBR12_BJX;
			opw1=0xF102|((reg&15)<<4);
			if(ctx->is_addr_x32)
				opw2=0xA000|((reg&0x10)<<6);
			else
				opw2=0xB000|((reg&0x10)<<6);
			opw3=0xF000;
			opw4=0x3000;
		}
#endif

#if 0
		rlty=BGBCC_SH_RLC_TBR12_BJX;
		opw1=0xA000;
		if(ctx->is_addr_x32)
			opw2=0x0E01|((reg&15)<<4);
//			opw2=0x0E02|((reg&15)<<4);
		else
			opw2=0x0F01|((reg&15)<<4);
//			opw2=0x0F02|((reg&15)<<4);

		opw3=0xF000;
		opw4=0x3000;
#endif

		BGBCC_JX2DA_EmitLoadRegLabelVarRel24(ctx, nmid, reg, lbl);

		if(opw5>=0)
			BGBCC_JX2_EmitWord(ctx, opw5);
		if(opw6>=0)
			BGBCC_JX2_EmitWord(ctx, opw6);

		BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw2);

		/* NOP4 */
//		BGBCC_JX2_EmitWord(ctx, 0xF000);
//		BGBCC_JX2_EmitWord(ctx, 0x3000);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}

	prlty=BGBCC_SH_RLC_PBO24_BJX;

	usewxj=0;
//	if(ctx->has_jumbo && !ctx->op_is_wex2)
	if(ctx->has_jumbo)
	{
		usewxj=1;
//		prlty=BGBCC_SH_RLC_PBOJ32_BJX;
	}

//	if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg) && (ctx->op_is_wex2&12))
	if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg) && (ctx->op_is_wex2&12) &&
		!(ctx->is_fixed32&2))
	{
		usewxj=0;
	}

//	if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
//		usewxj=0;

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;
	opw5=-1; opw6=-1; opw7=-1; opw8=-1;
	rlty=-1; rlty2=-1;
	opwf=-1; opwf1=-1;	opwf2=-1; opwf3=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
//		if(usewxj && BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
//		if(usewxj &&
//			(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg) ||
//			!(ctx->op_is_wex2&12)))
		if(usewxj)
		{
			rlty=BGBCC_SH_RLC_PBOJ32_BJX;
			opwf=0xF101|((reg&15)<<4);
			opwf1=0x8000|((reg&16)<<6);
			break;
		}
		
		if(prlty==BGBCC_SH_RLC_PBOJ32_BJX)
			prlty=BGBCC_SH_RLC_PBO24_BJX;
	
//		if(ctx->is_fixed32 || BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
		if(ctx->is_fixed32 || ctx->op_is_wex2 || (ctx->emit_riscv&0x22) ||
			!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				break;

			rlty=prlty;
			opwf=0xF000|((reg&16)<<2);
			opwf1=0x0C01|((reg&15)<<4);
			break;
		}

		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			rlty=prlty;
			opwf=0x0C01|((reg&15)<<4);
			break;
		}
		break;
	case BGBCC_SH_NMID_MOVW:
//		if(usewxj && BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
//		if(usewxj &&
//			(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg) ||
//			!(ctx->op_is_wex2&12)))
		if(usewxj)
		{
			rlty=BGBCC_SH_RLC_PBOJ32_BJX;
			opwf=0xF101|((reg&15)<<4);
			opwf1=0x9000|((reg&16)<<6);
			break;
		}
	
		if(prlty==BGBCC_SH_RLC_PBOJ32_BJX)
			prlty=BGBCC_SH_RLC_PBO24_BJX;
	
		if(ctx->is_fixed32 || ctx->op_is_wex2 ||
			(ctx->emit_riscv&0x22) ||
			!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				break;

			rlty=prlty;
			opwf=0xF000|((reg&16)<<2);
			opwf1=0x0D01|((reg&15)<<4);
			break;
		}

		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			rlty=prlty;
			opwf=0x0D01|((reg&15)<<4);
			break;
		}
		break;
	case BGBCC_SH_NMID_MOVL:
//		if(usewxj && BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
//		if(usewxj &&
//			(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg) ||
//			!(ctx->op_is_wex2&12)))
		if(usewxj)
		{
			rlty=BGBCC_SH_RLC_PBOJ32_BJX;
			opwf=0xF101|((reg&15)<<4);
			opwf1=0xA000|((reg&16)<<6);
			break;
		}
	
		if(prlty==BGBCC_SH_RLC_PBOJ32_BJX)
			prlty=BGBCC_SH_RLC_PBO24_BJX;
	
		if(ctx->is_fixed32 || ctx->op_is_wex2 ||
			(ctx->emit_riscv&0x22) ||
			!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
//		if(1)
		{
//			if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				break;

			rlty=prlty;
			opwf=0xF000|((reg&16)<<2);
			opwf1=0x0E01|((reg&15)<<4);
			break;
		}

		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			rlty=prlty;
			opwf=0x0E01|((reg&15)<<4);
			break;
		}
		break;
	case BGBCC_SH_NMID_MOVQ:
//		if(usewxj && BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
//		if(usewxj &&
//			(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg) ||
//			!(ctx->op_is_wex2&12)))
		if(usewxj)
		{
			rlty=BGBCC_SH_RLC_PBOJ32_BJX;
			opwf=0xF101|((reg&15)<<4);
			opwf1=0xB000|((reg&16)<<6);
			break;
		}
	
		if(prlty==BGBCC_SH_RLC_PBOJ32_BJX)
			prlty=BGBCC_SH_RLC_PBO24_BJX;
	
		if(ctx->is_fixed32 || ctx->op_is_wex2 ||
			(ctx->emit_riscv&0x22) ||
			!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				break;

			rlty=prlty;
			opwf=0xF000|((reg&16)<<2);
			opwf1=0x0F01|((reg&15)<<4);
			break;
		}

		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			rlty=prlty;
			opwf=0x0F01|((reg&15)<<4);
			break;
		}
		break;


	case BGBCC_SH_NMID_MOVUB:
//		if(usewxj && BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
//		if(usewxj &&
//			(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg) ||
//			!(ctx->op_is_wex2&12)))
		if(usewxj)
		{
			rlty=BGBCC_SH_RLC_PBOJ32_BJX;
			opwf=0xF101|((reg&15)<<4);
			opwf1=0x8800|((reg&16)<<6);
			break;
		}
	
		if(prlty==BGBCC_SH_RLC_PBOJ32_BJX)
			prlty=BGBCC_SH_RLC_PBO24_BJX;
	
		if(ctx->is_fixed32 || ctx->op_is_wex2 ||
			(ctx->emit_riscv&0x22) ||
			!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				break;

			rlty=prlty;
			opwf=0xF080|((reg&16)<<2);
			opwf1=0x0C01|((reg&15)<<4);
			break;
		}

		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			rlty=prlty;
			opwf=0x5201|((reg&15)<<4);
			break;
		}
		break;
	case BGBCC_SH_NMID_MOVUW:
//		if(usewxj && BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
//		if(usewxj &&
//			(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg) ||
//			!(ctx->op_is_wex2&12)))
		if(usewxj)
		{
			rlty=BGBCC_SH_RLC_PBOJ32_BJX;
			opwf=0xF101|((reg&15)<<4);
			opwf1=0x9800|((reg&16)<<6);
			break;
		}
	
		if(prlty==BGBCC_SH_RLC_PBOJ32_BJX)
			prlty=BGBCC_SH_RLC_PBO24_BJX;
	
		if(ctx->is_fixed32 || ctx->op_is_wex2 ||
			(ctx->emit_riscv&0x22) ||
			!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				break;

			rlty=prlty;
			opwf=0xF080|((reg&16)<<2);
			opwf1=0x0D01|((reg&15)<<4);
			break;
		}

		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			rlty=prlty;
			opwf=0x5301|((reg&15)<<4);
			break;
		}
		break;
	case BGBCC_SH_NMID_MOVUL:
	case BGBCC_SH_NMID_MOVDL:
//		if(usewxj && BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
//		if(usewxj &&
//			(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg) ||
//			!(ctx->op_is_wex2&12)))
		if(usewxj)
		{
			rlty=BGBCC_SH_RLC_PBOJ32_BJX;
			opwf=0xF101|((reg&15)<<4);
			opwf1=0xA800|((reg&16)<<6);
			break;
		}
	
		if(prlty==BGBCC_SH_RLC_PBOJ32_BJX)
			prlty=BGBCC_SH_RLC_PBO24_BJX;
	
		if(ctx->is_fixed32 || ctx->op_is_wex2 ||
			(ctx->emit_riscv&0x22) ||
			!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				break;

			rlty=prlty;
			opwf=0xF080|((reg&16)<<2);
			opwf1=0x0E01|((reg&15)<<4);
			break;
		}

		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			rlty=prlty;
			opwf=0x8801|((reg&15)<<4);
			break;
		}
		break;

#if 0
	case BGBCC_SH_NMID_MOVDL:
		if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
			break;

		rlty=prlty;
		opwf=0xF080|((reg&16)<<2);
		opwf1=0x0F01|((reg&15)<<4);
		break;
#endif

	case BGBCC_SH_NMID_MOVX2:
#if 0
		if(usewxj && BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
		{
//			rlty=BGBCC_SH_RLC_PBOJ27_BJX;
			rlty=BGBCC_SH_RLC_PBOJ29_BJX;
			opwf=0xF000|((reg&16)<<2);
			opwf1=0x4801|((reg&15)<<4);
			break;
		}
#endif

#if 1
//		if(usewxj && BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
//		if(usewxj &&
//			(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg) ||
//			!(ctx->op_is_wex2&12)))
		if(usewxj)
		{
			rlty=BGBCC_SH_RLC_PBOJ32_BJX;
			opwf=0xF101|((reg&15)<<4);
			opwf1=0x7800|((reg&16)<<6);
			break;
		}
#endif
	
		if(prlty==BGBCC_SH_RLC_PBOJ32_BJX)
			prlty=BGBCC_SH_RLC_PBO24_BJX;
	
//		if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			break;

		rlty=prlty;
		opwf=0xF000|((reg&16)<<2);
		opwf1=0x4C01|((reg&15)<<4);
		break;

	case BGBCC_SH_NMID_FMOVS:
//		if(usewxj && BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
//		if(usewxj &&
//			(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg) ||
//			!(ctx->op_is_wex2&12)))
		if(usewxj)
		{
			rlty=BGBCC_SH_RLC_PBOJ32_BJX;
			opwf=0xF101|((reg&15)<<4);
			opwf1=0x6000|((reg&16)<<6);
			break;
		}

		if(prlty==BGBCC_SH_RLC_PBOJ32_BJX)
			prlty=BGBCC_SH_RLC_PBO24_BJX;

//		if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			break;
		rlty=prlty;
		opwf=0xF000|((reg&16)<<2);
		opwf1=0x4E01|((reg&15)<<4);
		break;

//	case BGBCC_SH_NMID_FMOVS:
	case BGBCC_SH_NMID_FMOVD:
		BGBCC_DBGBREAK
		break;

	default:
		BGBCC_DBGBREAK
		break;
	
	}

	if(rlty==BGBCC_SH_RLC_PBOJ32_BJX)
	{
		ctx->stat_ldst_pbotot++;

		i=BGBCC_JX2_LookupLabelIndex(ctx, lbl);
		if(i>=0)
		{
			ctx->stat_ldst_pbotot_ix++;

			j=ctx->lbl_sec[i];
			k=ctx->lbl_ofs[i];
			
			if((j==BGBCC_SH_CSEG_DATA) && (k<=480) && (k>=0))
//			if((j==BGBCC_SH_CSEG_DATA) && (k<=496) && (k>=0))
			{
//				rlty=BGBCC_SH_RLC_PBO9_BJX;

				opw1=opwf;
				opw2=opwf1|k;
				rlty=-1;

				if(reg&32)
				{
					BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0400,
						&opw1, &opw2, &opw3, &opw4);
				}

				ctx->stat_ldst_pbotot_9b++;
			}

			if((ctx->has_fmovs&8) && (opw1<0))
			{
				if((j==BGBCC_SH_CSEG_DATA) && (nmid==BGBCC_SH_NMID_MOVL) &&
					(k>=0) && (k<=3840) && !(k&3))
				{
					opw1=0xF20A|((reg&15)<<4);
					opw2=0xD000|((reg&16)<<6)|(k>>2);
					rlty=-1;

					if(reg&32)
					{
						BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0400,
							&opw1, &opw2, &opw3, &opw4);
					}
				}

				if((j==BGBCC_SH_CSEG_DATA) && (nmid==BGBCC_SH_NMID_MOVQ) &&
					(k>=0) && (k<=7680) && !(k&7))
				{
					opw1=0xF20B|((reg&15)<<4);
					opw2=0xD000|((reg&16)<<6)|(k>>3);
					rlty=-1;

					if(reg&32)
					{
						BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0400,
							&opw1, &opw2, &opw3, &opw4);
					}
				}

				if((j==BGBCC_SH_CSEG_DATA) && (nmid==BGBCC_SH_NMID_MOVUL) &&
					(k>=0) && (k<=3840) && !(k&3))
				{
					opw1=0xF20A|((reg&15)<<4);
					opw2=0xD800|((reg&16)<<6)|(k>>2);
					rlty=-1;

					if(reg&32)
					{
						BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0400,
							&opw1, &opw2, &opw3, &opw4);
					}
				}

				if((j==BGBCC_SH_CSEG_DATA) && (nmid==BGBCC_SH_NMID_MOVX2) &&
					(k>=0) && (k<=7680) && !(k&7) && !(reg&32))
				{
					opw1=0xF20B|((reg&15)<<4);
					opw2=0xD800|((reg&16)<<6)|(k>>3);
					rlty=-1;
				}

#if 1
				if((ctx->is_fixed32&2) && (opw1<0))
				{
					if((j==BGBCC_SH_CSEG_DATA) &&
						((nmid==BGBCC_SH_NMID_MOVL) ||
							(nmid==BGBCC_SH_NMID_MOVUL)) &&
						(k>=0) && (k<=15360) && !(k&3))
//						(k>=0) && (k<=7680) && !(k&3))
					{
						opw1=0xF20A|((reg&15)<<4);
						opw2=0xD000|((reg&16)<<6)|((k>>2)&0x03FF);
						rlty=-1;
						
						if(nmid==BGBCC_SH_NMID_MOVUL)
							opw2|=0x0800;

						if(reg&32)
						{
							BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0400,
								&opw1, &opw2, &opw3, &opw4);
						}
						if((k>>12)&1)
							opw1^=0x2000;
						if((k>>13)&1)
							opw1^=0x4000;

						if(opw3>=0)
							opw1=-1;
					}

					if((j==BGBCC_SH_CSEG_DATA) && (nmid==BGBCC_SH_NMID_MOVQ) &&
						(k>=0) && (k<=30720) && !(k&7))
//						(k>=0) && (k<=15360) && !(k&7))
					{
						opw1=0xF20B|((reg&15)<<4);
						opw2=0xD000|((reg&16)<<6)|((k>>3)&0x03FF);
						rlty=-1;

						if(reg&32)
						{
							BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0400,
								&opw1, &opw2, &opw3, &opw4);
						}
						if((k>>13)&1)
							opw1^=0x2000;
						if((k>>14)&1)
							opw1^=0x4000;

						if(opw3>=0)
							opw1=-1;
					}

#if 1
					if((j==BGBCC_SH_CSEG_DATA) &&
						(nmid==BGBCC_SH_NMID_MOVX2) &&
						(k>=0) && (k<=30720) && !(k&7) && !(reg&32))
					{
						opw1=0xF20B|((reg&15)<<4);
						opw2=0xD800|((reg&16)<<6)|((k>>3)&0x03FF);
						rlty=-1;

						if((k>>13)&1)
							opw1^=0x2000;
						if((k>>14)&1)
							opw1^=0x4000;
					}
#endif
				}
#endif

				if(opw1>=0)
					ctx->stat_ldst_pbotot_10b++;

			}
		}

		if(opw1<0)
			ctx->stat_ldst_pbotot_33b++;
	}
	
//	if(reg&32)
	if((reg&32) && (opwf>=0))
	{
		BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0400,
			&opwf, &opwf1, &opwf2, &opwf3);
	}

	if((rlty>=0) && (opw1<0) && (opwf>=0))
	{
//		if(rlty==BGBCC_SH_RLC_PBOJ27_BJX)
//			{ opw1=0xF400; opw2=0xC000; opw3=opwf; opw4=opwf1; }
//		if(rlty==BGBCC_SH_RLC_PBOJ31_BJX)
//			{ opw1=0xF400; opw2=0xC000; opw3=opwf; opw4=opwf1; }

		if(rlty==BGBCC_SH_RLC_PBO9_BJX)
			{ opw1=opwf; opw2=opwf1;
				opw3=opwf2; opw4=opwf3; }

		if(rlty==BGBCC_SH_RLC_PBOJ29_BJX)
			{ opw1=0xFE00; opw2=0x0000; opw3=opwf; opw4=opwf1;
				opw5=opwf2; opw6=opwf3; }
		if(rlty==BGBCC_SH_RLC_PBOJ32_BJX)
			{ opw1=0xFE00; opw2=0x0000; opw3=opwf; opw4=opwf1;
				opw5=opwf2; opw6=opwf3; }

		if(rlty==BGBCC_SH_RLC_PBO24_BJX)
			{ opw1=0xFA00; opw2=0x0000; opw3=opwf; opw4=opwf1;
				opw5=opwf2; opw6=opwf3; }
		if(rlty==BGBCC_SH_RLC_PBO32_BJX)
			{ opw1=0xFA00; opw2=0x0000; opw3=0x2600;
				opw4=opwf; opw5=opwf1;
				opw6=opwf2; opw7=opwf3; }
	}

	if((rlty==BGBCC_SH_RLC_PBO24_BJX) && (opw1>=0))
	{
		i=BGBCC_JX2_LookupLabelIndex(ctx, lbl);
		if(i>=0)
		{
			ctx->stat_ldst_pbotot_ix++;

			j=ctx->lbl_sec[i];
			k=ctx->lbl_ofs[i];
			
			if((j==BGBCC_SH_CSEG_DATA) && (k<=65535) && (k>=0))
			{
				opw1=0xF800;
				opw2=k;
				rlty=0;
			}
		}
	}

	if(opw1>=0)
	{
		BGBCC_JX2DA_EmitLoadRegLabelVarRel24(ctx, nmid, reg, lbl);

		BGBCC_JX2_EmitOpCheckRepack6(ctx,
			&opw1, &opw2, &opw3, &opw4, &opw5, &opw6);

//		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
//		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		if(rlty>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(rlty2>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty2); }
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);

//		if(((opw1&0xFE00)>=0xF000) && ((opw1&0xFE00)<0xFC00))
//		{
//			BGBCC_JX2_EmitOpCheckRepack(ctx, &opw3, &opw4);
//			BGBCC_JX2_EmitPadForOpWord(ctx, opw3);
//		}

		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
		if(opw5>=0)
			BGBCC_JX2_EmitWord(ctx, opw5);
		if(opw6>=0)
			BGBCC_JX2_EmitWord(ctx, opw6);
		if(opw7>=0)
			BGBCC_JX2_EmitWord(ctx, opw7);
		if(opw8>=0)
			BGBCC_JX2_EmitWord(ctx, opw8);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}

	BGBCC_DBGBREAK
	return(0);
}

int BGBCC_JX2_TryEmitStoreRegLabelVarPbo24(
	BGBCC_JX2_Context *ctx, int nmid, int reg, int lbl)
{
	int opw1, opw2, opw3, opw4, opw5, opw6, opw7, opw8;
	int opwf, opwf1, opwf2, opwf3;
	int rlty, prlty, rlty2, usewxj;
	int tr0;
	int i, j, k;

	prlty=BGBCC_SH_RLC_PBO24_BJX;

	nmid=BGBCC_JX2_EmitRemapPseudoOp(ctx, nmid);

	usewxj=0;
//	if(ctx->has_jumbo && !ctx->op_is_wex2)
	if(ctx->has_jumbo)
	{
		usewxj=1;
//		prlty=BGBCC_SH_RLC_PBOJ32_BJX;
	}
	
//	if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg) && (ctx->op_is_wex2&12))
	if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg) && (ctx->op_is_wex2&12) &&
		!(ctx->is_fixed32&2))
	{
		usewxj=0;
	}

//	if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
//		usewxj=0;

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;
	opw5=-1; opw6=-1; opw7=-1; opw8=-1;
	rlty=-1; rlty2=-1;
	opwf=-1; opwf1=-1;	opwf2=-1; opwf3=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
	case BGBCC_SH_NMID_MOVUB:
//		if(usewxj && BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
//		if(usewxj &&
//			(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg) ||
//			!(ctx->op_is_wex2&12)))
		if(usewxj)
		{
			rlty=BGBCC_SH_RLC_PBOJ32_BJX;
			opwf=0xF110|((reg&15)<<0);
			opwf1=0x0000|((reg&16)<<5);
			break;
		}

		if(ctx->is_fixed32 || ctx->op_is_wex2 ||
			(ctx->emit_riscv&0x22) ||
			!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				break;

			rlty=prlty;
			opwf=0xF000|((reg&16)<<1);
			opwf1=0x0410|(reg&15);
			break;
		}

		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			rlty=prlty;
			opwf=0x0410|(reg&15);
			break;
		}
		break;
	case BGBCC_SH_NMID_MOVW:
	case BGBCC_SH_NMID_MOVUW:
//		if(usewxj && BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
//		if(usewxj &&
//			(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg) ||
//			!(ctx->op_is_wex2&12)))
		if(usewxj)
		{
			rlty=BGBCC_SH_RLC_PBOJ32_BJX;
			opwf=0xF110|((reg&15)<<0);
			opwf1=0x1000|((reg&16)<<5);
			break;
		}

		if(ctx->is_fixed32 || ctx->op_is_wex2 ||
			(ctx->emit_riscv&0x22) ||
			!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				break;

			rlty=prlty;
			opwf=0xF000|((reg&16)<<1);
			opwf1=0x0510|(reg&15);
			break;
		}

		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			rlty=prlty;
			opwf=0x0510|(reg&15);
			break;
		}
		break;
	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVUL:
	case BGBCC_SH_NMID_MOVDL:
//		if(usewxj && BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
//		if(usewxj &&
//			(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg) ||
//			!(ctx->op_is_wex2&12)))
		if(usewxj)
		{
			rlty=BGBCC_SH_RLC_PBOJ32_BJX;
			opwf=0xF110|((reg&15)<<0);
			opwf1=0x2000|((reg&16)<<5);
			break;
		}

		if(ctx->is_fixed32 || ctx->op_is_wex2 ||
			(ctx->emit_riscv&0x22) ||
			!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				break;

			rlty=prlty;
			opwf=0xF000|((reg&16)<<1);
			opwf1=0x0610|(reg&15);
			break;
		}

		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			rlty=prlty;
			opwf=0x0610|(reg&15);
			break;
		}
		break;
	case BGBCC_SH_NMID_MOVQ:
//		if(usewxj && BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
//		if(usewxj &&
//			(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg) ||
//			!(ctx->op_is_wex2&12)))
		if(usewxj)
		{
			rlty=BGBCC_SH_RLC_PBOJ32_BJX;
			opwf=0xF110|((reg&15)<<0);
			opwf1=0x3000|((reg&16)<<5);
			break;
		}

		if(ctx->is_fixed32 || ctx->op_is_wex2 ||
			(ctx->emit_riscv&0x22) ||
			!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				break;

			rlty=prlty;
			opwf=0xF000|((reg&16)<<1);
			opwf1=0x0710|(reg&15);
			break;
		}

		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			rlty=prlty;
			opwf=0x0710|(reg&15);
			break;
		}
		break;

	case BGBCC_SH_NMID_MOVX2:
//		if(usewxj && BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
//		if(usewxj &&
//			(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg) ||
//			!(ctx->op_is_wex2&12)))
		if(usewxj)
		{
			rlty=BGBCC_SH_RLC_PBOJ32_BJX;
			opwf=0xF110|((reg&15)<<0);
			opwf1=0x5800|((reg&16)<<5);
			break;
		}

		if(prlty==BGBCC_SH_RLC_PBOJ32_BJX)
			prlty=BGBCC_SH_RLC_PBO24_BJX;

//		if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			break;

		rlty=prlty;
		opwf=0xF000|((reg&16)<<1);
		opwf1=0x4410|((reg&15)<<0);
		break;

	case BGBCC_SH_NMID_FMOVS:
//		if(usewxj && BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
//		if(usewxj &&
//			(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg) ||
//			!(ctx->op_is_wex2&12)))
		if(usewxj)
		{
			rlty=BGBCC_SH_RLC_PBOJ32_BJX;
			opwf=0xF110|((reg&15)<<0);
			opwf1=0x4000|((reg&16)<<5);
			break;
		}

//		if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			break;

		rlty=prlty;
		opwf=0xF000|((reg&16)<<1);
		opwf1=0x4610|(reg&15);
		break;

//	case BGBCC_SH_NMID_FMOVS:
	case BGBCC_SH_NMID_FMOVD:
		BGBCC_DBGBREAK
		break;

	default:
		BGBCC_DBGBREAK
		break;
	}

	if(rlty==BGBCC_SH_RLC_PBOJ32_BJX)
	{
		ctx->stat_ldst_pbotot++;
	
		i=BGBCC_JX2_LookupLabelIndex(ctx, lbl);
		if(i>=0)
		{
			ctx->stat_ldst_pbotot_ix++;

			j=ctx->lbl_sec[i];
			k=ctx->lbl_ofs[i];
			
			if((j==BGBCC_SH_CSEG_DATA) && (k<=480) && (k>=0))
//			if((j==BGBCC_SH_CSEG_DATA) && (k<=496) && (k>=0))
			{
//				rlty=BGBCC_SH_RLC_PBO9_BJX;

				opw1=opwf;
				opw2=opwf1|k;
				rlty=-1;

				if(reg&32)
				{
					BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0200,
						&opw1, &opw2, &opw3, &opw4);
				}

				ctx->stat_ldst_pbotot_9b++;
			}

#if 1
			if((ctx->has_fmovs&8) && (opw1<0))
			{
				if((j==BGBCC_SH_CSEG_DATA) &&
					((nmid==BGBCC_SH_NMID_MOVL) ||
						(nmid==BGBCC_SH_NMID_MOVUL)) &&
					(k>=0) && (k<=3840) && !(k&3))
				{
					opw1=0xF208|((reg&15)<<4);
					opw2=0xD000|((reg&16)<<6)|(k>>2);
					rlty=-1;

					if(reg&32)
					{
						BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0400,
							&opw1, &opw2, &opw3, &opw4);
					}
				}

				if((j==BGBCC_SH_CSEG_DATA) && (nmid==BGBCC_SH_NMID_MOVQ) &&
					(k>=0) && (k<=7680) && !(k&7))
				{
					opw1=0xF209|((reg&15)<<4);
					opw2=0xD000|((reg&16)<<6)|(k>>3);
					rlty=-1;

					if(reg&32)
					{
						BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0400,
							&opw1, &opw2, &opw3, &opw4);
					}
				}

				if((j==BGBCC_SH_CSEG_DATA) && (nmid==BGBCC_SH_NMID_MOVX2) &&
					(k>=0) && (k<=7680) && !(k&7) && !(reg&32))
				{
					opw1=0xF209|((reg&15)<<4);
					opw2=0xD800|((reg&16)<<6)|(k>>3);
					rlty=-1;
				}
				
#if 1
				if((ctx->is_fixed32&2) && (opw1<0))
				{
					if((j==BGBCC_SH_CSEG_DATA) &&
						((nmid==BGBCC_SH_NMID_MOVL) ||
							(nmid==BGBCC_SH_NMID_MOVUL)) &&
						(k>=0) && (k<=15360) && !(k&3))
//						(k>=0) && (k<=7680) && !(k&3))
					{
						opw1=0xF208|((reg&15)<<4);
						opw2=0xD000|((reg&16)<<6)|((k>>2)&0x03FF);
						rlty=-1;

						if(reg&32)
						{
							BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0400,
								&opw1, &opw2, &opw3, &opw4);
						}
						if((k>>12)&1)
							opw1^=0x2000;
						if((k>>13)&1)
							opw1^=0x4000;
					}

					if((j==BGBCC_SH_CSEG_DATA) && (nmid==BGBCC_SH_NMID_MOVQ) &&
						(k>=0) && (k<=30720) && !(k&7))
//						(k>=0) && (k<=15360) && !(k&7))
					{
						opw1=0xF209|((reg&15)<<4);
						opw2=0xD000|((reg&16)<<6)|((k>>3)&0x03FF);
						rlty=-1;

						if(reg&32)
						{
							BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0400,
								&opw1, &opw2, &opw3, &opw4);
						}
						if((k>>13)&1)
							opw1^=0x2000;
						if((k>>14)&1)
							opw1^=0x4000;
					}
				}
#endif

				if(opw1>=0)
					ctx->stat_ldst_pbotot_10b++;
			}
#endif
		}

		if(opw1<0)
			ctx->stat_ldst_pbotot_33b++;
	}

//	if(reg&32)
	if((reg&32) && (opwf>=0))
	{
		BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0200,
			&opwf, &opwf1, &opwf2, &opwf3);
	}

	if((rlty>=0) && (opw1<0) && (opwf>=0))
	{
//		if(rlty==BGBCC_SH_RLC_PBOJ27_BJX)
//			{ opw1=0xF400; opw2=0xC000; opw3=opwf; opw4=opwf1; }
//		if(rlty==BGBCC_SH_RLC_PBOJ31_BJX)
//			{ opw1=0xF400; opw2=0xC000; opw3=opwf; opw4=opwf1; }

		if(rlty==BGBCC_SH_RLC_PBO9_BJX)
			{ opw1=opwf; opw2=opwf1;
				opw3=opwf2; opw4=opwf3;  }

		if(rlty==BGBCC_SH_RLC_PBOJ29_BJX)
			{ opw1=0xFE00; opw2=0x0000; opw3=opwf; opw4=opwf1;
				opw5=opwf2; opw6=opwf3;  }
		if(rlty==BGBCC_SH_RLC_PBOJ32_BJX)
			{ opw1=0xFE00; opw2=0x0000; opw3=opwf; opw4=opwf1;
				opw5=opwf2; opw6=opwf3;  }

		if(rlty==BGBCC_SH_RLC_PBO24_BJX)
			{ opw1=0xFA00; opw2=0x0000; opw3=opwf; opw4=opwf1;
				opw5=opwf2; opw6=opwf3; }
		if(rlty==BGBCC_SH_RLC_PBO32_BJX)
			{ opw1=0xFA00; opw2=0x0000; opw3=0x2600;
				opw4=opwf; opw5=opwf1;
				opw6=opwf2; opw7=opwf3; }
	}

	if((rlty==BGBCC_SH_RLC_PBO24_BJX) && (opw1>=0))
	{
		i=BGBCC_JX2_LookupLabelIndex(ctx, lbl);
		if(i>=0)
		{
			ctx->stat_ldst_pbotot_ix++;

			j=ctx->lbl_sec[i];
			k=ctx->lbl_ofs[i];
			
			if((j==BGBCC_SH_CSEG_DATA) && (k<=65535) && (k>=0))
			{
				opw1=0xF800;
				opw2=k;
				rlty=0;
			}
		}
	}

	if(opw1>=0)
	{
//		BGBCC_JX2DA_EmitLoadRegLabelVarRel24(ctx, nmid, reg, lbl);
		BGBCC_JX2DA_EmitStoreRegLabelVarRel24(ctx, nmid, reg, lbl);

		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2, 0);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		if(rlty>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(rlty2>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty2); }
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);

//		if(((opw1&0xFE00)>=0xF000) && ((opw1&0xFE00)<0xFC00))
		if(((opw1&0xFE00)>=0xF000) && ((opw1&0xFE00)<=0xFE00))
		{
			BGBCC_JX2_EmitOpCheckRepack(ctx, &opw3, &opw4, 0);
			BGBCC_JX2_EmitPadForOpWord(ctx, opw3);
		}

		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
		if(opw5>=0)
			BGBCC_JX2_EmitWord(ctx, opw5);
		if(opw6>=0)
			BGBCC_JX2_EmitWord(ctx, opw6);
		if(opw7>=0)
			BGBCC_JX2_EmitWord(ctx, opw7);
		if(opw8>=0)
			BGBCC_JX2_EmitWord(ctx, opw8);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}

//	BGBCC_DBGBREAK
	return(0);
}

int BGBCC_JX2_EmitLoadRegLabelVarRel24(
	BGBCC_JX2_Context *ctx, int nmid, int reg, int lbl)
{
	if(!BGBCC_JX2_TryEmitLoadRegLabelVarRel24(ctx, nmid, reg, lbl))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_TryEmitLoadRegLabelVarTls(
	BGBCC_JX2_Context *ctx, int nmid, int reg, int lbl)
{
	int tr0, tr1, ix;

	ix=lbl-BGBCC_SH_LBL_TLSSTRT;

	tr0=BGBCC_JX2C_ScratchAllocTsReg(NULL, ctx, BGBCC_SH_REGCLS_GR);
	tr1=BGBCC_JX2C_ScratchAllocTsReg(NULL, ctx, BGBCC_SH_REGCLS_GR);

//	BGBCC_JX2_EmitOpRegReg(ctx,
//		BGBCC_SH_NMID_MOV, BGBCC_SH_REG_TBR, tr0);

	BGBCC_JX2_EmitLoadDrImm(ctx, 0x50);
	BGBCC_JX2_EmitOpLdReg2Reg(ctx, BGBCC_SH_NMID_MOVQ,
		BGBCC_SH_REG_TBR, BGBCC_SH_REG_DLR, tr0);

	BGBCC_JX2_EmitLoadRegLabelVarRel24(ctx,
		BGBCC_SH_NMID_MOVL, tr1, ctx->tlsi_lbl);

	BGBCC_JX2_EmitOpLdReg2Reg(ctx, BGBCC_SH_NMID_MOVQ,
		tr0, tr1, tr0);

	BGBCC_JX2_EmitOpLdRegDispReg(ctx, nmid,
		tr0, ctx->tlsvar_ofs[ix], reg);

	BGBCC_JX2C_ScratchReleaseReg(NULL, ctx, tr0);
	BGBCC_JX2C_ScratchReleaseReg(NULL, ctx, tr1);
	return(1);
}

int BGBCC_JX2_EmitStoreRegLabelVarTls(
	BGBCC_JX2_Context *ctx, int nmid, int reg, int lbl)
{
	int tr0, tr1, ix;

	ix=lbl-BGBCC_SH_LBL_TLSSTRT;

	tr0=BGBCC_JX2C_ScratchAllocTsReg(NULL, ctx, BGBCC_SH_REGCLS_GR);
	tr1=BGBCC_JX2C_ScratchAllocTsReg(NULL, ctx, BGBCC_SH_REGCLS_GR);

//	BGBCC_JX2_EmitOpRegReg(ctx,
//		BGBCC_SH_NMID_MOV, BGBCC_SH_REG_TBR, tr0);

	BGBCC_JX2_EmitLoadDrImm(ctx, 0x50);
	BGBCC_JX2_EmitOpLdReg2Reg(ctx, BGBCC_SH_NMID_MOVQ,
		BGBCC_SH_REG_TBR, BGBCC_SH_REG_DLR, tr0);

	BGBCC_JX2_EmitLoadRegLabelVarRel24(ctx,
		BGBCC_SH_NMID_MOVL, tr1, ctx->tlsi_lbl);

	BGBCC_JX2_EmitOpLdReg2Reg(ctx, BGBCC_SH_NMID_MOVQ,
		tr0, tr1, tr0);

	BGBCC_JX2_EmitOpRegStRegDisp(ctx, nmid,
		reg, tr0, ctx->tlsvar_ofs[ix]);

	BGBCC_JX2C_ScratchReleaseReg(NULL, ctx, tr0);
	BGBCC_JX2C_ScratchReleaseReg(NULL, ctx, tr1);
	return(1);
}

int BGBCC_JX2_TryEmitLoadRegLabelVarRel24(
	BGBCC_JX2_Context *ctx, int nmid, int reg, int lbl)
{
	int opw1, opw2, opw3, opw4, opw5, opw6, opwf;
	int rlty, prlty, rlty2;
	int tr0, tr1, j;

	if(nmid==BGBCC_SH_NMID_MOVX2)
	{
		reg=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, reg);
	}

#if 0
	if(	BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg) &&
		!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
	{
		j=-1;
		
		if(nmid==BGBCC_SH_NMID_MOVX2)
		{
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx->tctx, ctx, 
				BGBCC_SH_REGCLS_QGR2);

			j=BGBCC_JX2_TryEmitLoadRegLabelVarRel24(ctx, nmid, tr0, lbl);
			BGBCC_JX2_EmitOpRegReg(ctx, BGBCC_SH_NMID_MOVX2,
				tr0, reg);
			BGBCC_JX2C_ScratchReleaseReg(ctx->tctx, ctx, tr0);
			return(j);
		}else
		{
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx->tctx, ctx, 
				BGBCC_SH_REGCLS_QGR);

			j=BGBCC_JX2_TryEmitLoadRegLabelVarRel24(ctx, nmid, tr0, lbl);
			BGBCC_JX2_EmitOpRegReg(ctx, BGBCC_SH_NMID_MOV,
				tr0, reg);
			BGBCC_JX2C_ScratchReleaseReg(ctx->tctx, ctx, tr0);
			return(j);
		}
	}
#endif

	nmid=BGBCC_JX2_EmitRemapPseudoOp(ctx, nmid);

//	ctx->test_lclalign=3;

	if((lbl>=BGBCC_SH_LBL_ARCHSTRT) && (lbl<BGBCC_SH_LBL_ARCHEND))
	{
		tr0=lbl-BGBCC_SH_LBL_ARCHSTRT;
		
		if(tr0==BGBCC_SH_REG_ISRSAVE)
		{
			tr1=reg;
			if(BGBCC_JX2C_EmitRegIsExtLpReg(NULL, ctx, reg))
			{
				tr1=BGBCC_JX2C_MapLpRegToQgr(NULL, ctx, reg);
			}
			
//			j=ctx->frm_offs_isrsaves+ctx->frm_size;
			j=ctx->frm_offs_isrsaves;
			BGBCC_JX2_EmitOpLdRegDispReg(ctx,
				BGBCC_SH_NMID_LEAB,
				BGBCC_SH_REG_SP, j, tr1);
			
			if(tr1!=reg)
			{
				BGBCC_JX2_EmitOpRegReg(ctx,
					BGBCC_SH_NMID_MOV, BGBCC_SH_REG_GBH, tr1+1);
			}

			return(1);
		}
		
		BGBCC_JX2_EmitOpRegReg(ctx,
			BGBCC_SH_NMID_MOV, tr0, reg);
		return(1);
	}

	if((lbl>=BGBCC_SH_LBL_TLSSTRT) && (lbl<BGBCC_SH_LBL_TLSEND))
	{
		return(BGBCC_JX2_TryEmitLoadRegLabelVarTls(ctx,
			nmid, reg, lbl));
	}


//	if(nmid==BGBCC_SH_NMID_FMOVS)
//	{
//		if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
//			nmid=BGBCC_SH_NMID_MOVL;
//	}

//	if(nmid==BGBCC_SH_NMID_FMOVD)
//	{
//		if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
//			nmid=BGBCC_SH_NMID_MOVD;
//	}

#if 1
	if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
//	if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl) && (nmid==BGBCC_SH_NMID_MOVL))
//	if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl) && (nmid==BGBCC_SH_NMID_MOVUB))
//	if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl) && (nmid!=BGBCC_SH_NMID_MOVUL))
	{
		return(BGBCC_JX2_TryEmitLoadRegLabelVarPbo24(ctx, nmid, reg, lbl));
	}
#endif

#if 0
	if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
	{
		if(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
		{
			tr0=BGBCC_JX2C_ScratchAllocTsReg(NULL, ctx, BGBCC_SH_REGCLS_GR);
			BGBCC_JX2_EmitLoadRegLabelVarRel24(ctx, nmid, tr0, lbl);
			BGBCC_JX2_EmitOpRegReg(ctx,
				BGBCC_SH_NMID_MOV, tr0, reg);
			BGBCC_JX2C_ScratchReleaseReg(NULL, ctx, tr0);
			return(1);
		}

		return(0);
	}
#endif

	prlty=BGBCC_SH_RLC_REL24_BJX;

	if(BGBCC_JX2_EmitCheckAutoLabelNear8(ctx, lbl, nmid))
	{
//		prlty=BGBCC_SH_RLC_RELW8_BSR;
	}
	else if(BGBCC_JX2_ModelIsLabel16P(ctx))
	{
//		prlty=BGBCC_SH_RLC_RELW16C_BSR;
//		prlty=BGBCC_SH_RLC_RELW16A_BSR;
//		if((reg&15)==15)
//			prlty=BGBCC_SH_RLC_RELW16C_BSR;
//		if((nmid==BGBCC_SH_NMID_MOVUB) ||
//			(nmid==BGBCC_SH_NMID_MOVUW) ||
//			(nmid==BGBCC_SH_NMID_MOVUL))
//				prlty=BGBCC_SH_RLC_RELW16C_BSR;
	}
	else if(BGBCC_JX2_ModelIsData24P(ctx))
	{
#if 0
//		prlty=BGBCC_SH_RLC_RELW16C_BSR;
		prlty=BGBCC_SH_RLC_RELW24A_BSR;
		if((reg&15)==15)
			prlty=BGBCC_SH_RLC_RELW24C_BSR;
		if((nmid==BGBCC_SH_NMID_MOVUB) ||
			(nmid==BGBCC_SH_NMID_MOVUW) ||
			(nmid==BGBCC_SH_NMID_MOVUL))
				prlty=BGBCC_SH_RLC_RELW24C_BSR;
#endif
		prlty=BGBCC_SH_RLC_REL24_BJX;

//		if(BGBCC_JX2_CheckPadCross48(ctx))
//			prlty=BGBCC_SH_RLC_RELW24C_BSR;
		if(BGBCC_JX2_CheckPadCross32(ctx))
			prlty=BGBCC_SH_RLC_RELW24C_BSR;
//		if(BGBCC_JX2_CheckPadCross48(ctx))
//			prlty=BGBCC_SH_RLC_RELW24_BSR;
	}
	else
	{
		prlty=BGBCC_SH_RLC_RELW32C_BSR;
	}

	opw1=-1; opw2=-1; opw3=-1; opw4=-1; opw5=-1; opw6=-1;
	rlty=-1; rlty2=-1;
	opwf=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2 ||
			(ctx->emit_riscv&0x22) ||
			!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				break;

			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
//			if(reg&32)
			if(0)
			{
				opw3=0x47000|((reg&15)<<4)|((reg&32)<<5);
				opw4=0x5000C|((reg&16)<<6);
			}else
			{
				opw3=0xF000|((reg&16)<<2);
				opw4=0x0C00|((reg&15)<<4);
			}

			if(reg&32)
			{
				BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0400,
					&opw3, &opw4, &opw5, &opw6);
				if(opw5>=0)
					opw2-=4;
			}

			break;
		}

//		if(!BGBCC_JX2_CheckPadCross48(ctx))
		if(!BGBCC_JX2_CheckPadCross48(ctx) ||
			BGBCC_JX2_CheckPadCross32(ctx))
		{
			if(prlty==BGBCC_SH_RLC_REL24_BJX)
			{
				if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
					break;

				rlty2=prlty;
				opw1=0xFC27|((reg&16)<<4);
				opw2=0x8000|((reg&15)<<8);
				opw3=0x0000;
				break;
			}
		}
	
		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if((prlty==BGBCC_SH_RLC_RELW16A_BSR) ||
//				(prlty==BGBCC_SH_RLC_RELW24A_BSR) ||
//				(prlty==BGBCC_SH_RLC_RELW32A_BSR))
//				{ rlty=prlty; opwf=0x4400|((reg&15)<<4); break; }
			rlty=prlty;
//			opwf=0x300C|((reg&15)<<4);
			opwf=0x0C00|((reg&15)<<4);
			break;
		}

		if(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
		{
			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
			opw3=0xF000|((reg&16)<<2);
			opw4=0x0C00|((reg&15)<<4);
			break;
		}

		break;
	case BGBCC_SH_NMID_MOVW:
//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2 ||
			(ctx->emit_riscv&0x22) ||
			!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				break;

			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
//			if(reg&32)
			if(0)
			{
				opw3=0x47000|((reg&15)<<4)|((reg&32)<<5);
				opw4=0x5000D|((reg&16)<<6);
			}else
			{
				opw3=0xF000|((reg&16)<<2);
				opw4=0x0D00|((reg&15)<<4);
			}

			if(reg&32)
			{
				BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0400,
					&opw3, &opw4, &opw5, &opw6);
				if(opw5>=0)
					opw2-=4;
			}
			break;
		}

//		if(!BGBCC_JX2_CheckPadCross48(ctx))
		if(!BGBCC_JX2_CheckPadCross48(ctx) ||
			BGBCC_JX2_CheckPadCross32(ctx))
		{
			if(prlty==BGBCC_SH_RLC_REL24_BJX)
			{
				if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
					break;

				rlty2=prlty;
				opw1=0xFC27|((reg&16)<<4);
				opw2=0x9000|((reg&15)<<8);
				opw3=0x0000;
				break;
			}
		}
	
		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if((prlty==BGBCC_SH_RLC_RELW16A_BSR) ||
//				(prlty==BGBCC_SH_RLC_RELW24A_BSR) ||
//				(prlty==BGBCC_SH_RLC_RELW32A_BSR))
//				{ rlty=prlty; opwf=0x4500|((reg&15)<<4); break; }
			rlty=prlty;
//			opwf=0x300D|((reg&15)<<4);
			opwf=0x0D00|((reg&15)<<4);
			break;
		}
		if(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
		{
			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
			opw3=0xF000|((reg&16)<<2);
			opw4=0x0D00|((reg&15)<<4);
			break;
		}
		break;
	case BGBCC_SH_NMID_MOVL:
//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2 ||
			(ctx->emit_riscv&0x22) ||
			!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				break;

			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
//			if(reg&32)
			if(0)
			{
				opw3=0x47000|((reg&15)<<4)|((reg&32)<<5);
				opw4=0x5000E|((reg&16)<<6);
			}else
			{
				opw3=0xF000|((reg&16)<<2);
				opw4=0x0E00|((reg&15)<<4);
			}
			if(reg&32)
			{
				BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0400,
					&opw3, &opw4, &opw5, &opw6);
				if(opw5>=0)
					opw2-=4;
			}
			break;
		}

//		if(!BGBCC_JX2_CheckPadCross48(ctx))
		if(!BGBCC_JX2_CheckPadCross48(ctx) ||
			BGBCC_JX2_CheckPadCross32(ctx))
		{
			if(prlty==BGBCC_SH_RLC_REL24_BJX)
			{
				if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
					break;

				rlty2=prlty;
				opw1=0xFC27|((reg&16)<<4);
				opw2=0xA000|((reg&15)<<8);
				opw3=0x0000;
				break;
			}
		}
	
		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if((prlty==BGBCC_SH_RLC_RELW16A_BSR) ||
//				(prlty==BGBCC_SH_RLC_RELW24A_BSR) ||
//				(prlty==BGBCC_SH_RLC_RELW32A_BSR))
//				{ rlty=prlty; opwf=0x4600|((reg&15)<<4); break; }
			rlty=prlty;
//			opwf=0x300E|((reg&15)<<4);
			opwf=0x0E00|((reg&15)<<4);
			break;
		}
		if(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
		{
			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
			opw3=0xF000|((reg&16)<<2);
			opw4=0x0E00|((reg&15)<<4);
			break;
		}
		break;
	case BGBCC_SH_NMID_MOVQ:
//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2 ||
			(ctx->emit_riscv&0x22) ||
			!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				break;

			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
//			if(reg&32)
			if(0)
			{
				opw3=0x47000|((reg&15)<<4)|((reg&32)<<5);
				opw4=0x5000F|((reg&16)<<6);
			}else
			{
				opw3=0xF000|((reg&16)<<2);
				opw4=0x0F00|((reg&15)<<4);
			}
			if(reg&32)
			{
				BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0400,
					&opw3, &opw4, &opw5, &opw6);
				if(opw5>=0)
					opw2-=4;
			}
			break;
		}

//		if(!BGBCC_JX2_CheckPadCross48(ctx))
		if(!BGBCC_JX2_CheckPadCross48(ctx) ||
			BGBCC_JX2_CheckPadCross32(ctx))
		{
			if(prlty==BGBCC_SH_RLC_REL24_BJX)
			{
				if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
					break;

				rlty2=prlty;
				opw1=0xFC27|((reg&16)<<4);
				opw2=0xB000|((reg&15)<<8);
				opw3=0x0000;
				break;
			}
		}
	
		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if((prlty==BGBCC_SH_RLC_RELW16A_BSR) ||
//				(prlty==BGBCC_SH_RLC_RELW24A_BSR) ||
//				(prlty==BGBCC_SH_RLC_RELW32A_BSR))
//				{ rlty=prlty; opwf=0x4700|((reg&15)<<4); break; }
			rlty=prlty;
//			opwf=0x300F|((reg&15)<<4);
			opwf=0x0F00|((reg&15)<<4);
			break;
		}
		if(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
		{
			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
			opw3=0xF000|((reg&16)<<2);
			opw4=0x0F00|((reg&15)<<4);
			break;
		}
		break;

	case BGBCC_SH_NMID_MOVUB:
//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2 ||
			(ctx->emit_riscv&0x22) ||
			!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				break;
			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
//			if(reg&32)
			if(0)
			{
				opw3=0x47000|((reg&15)<<4)|((reg&32)<<5);
				opw4=0x5080C|((reg&16)<<6);
			}else
			{
				opw3=0xF080|((reg&16)<<2);
				opw4=0x0C00|((reg&15)<<4);
			}
			if(reg&32)
			{
				BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0400,
					&opw3, &opw4, &opw5, &opw6);
				if(opw5>=0)
					opw2-=4;
			}
			break;
		}

#if 0
//		if(!BGBCC_JX2_CheckPadCross48(ctx))
		if(!BGBCC_JX2_CheckPadCross48(ctx) ||
			BGBCC_JX2_CheckPadCross32(ctx))
		{
			if(prlty==BGBCC_SH_RLC_REL24_BJX)
			{
				if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
					break;
				rlty2=prlty;
				opw1=0xFC27|((reg&16)<<4);
				opw2=0xC000|((reg&15)<<8);
				opw3=0x0000;
				break;
			}
		}
#endif

		rlty=prlty;
		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
//			{ opwf=0x3504|((reg&15)<<4); break; }
			{ opwf=0x5200|((reg&15)<<4); break; }

		if(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
		{
			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
			opw3=0xF080|((reg&16)<<2);
			opw4=0x0C00|((reg&15)<<4);
			break;
		}
		break;
	case BGBCC_SH_NMID_MOVUW:
//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2 ||
			(ctx->emit_riscv&0x22) ||
			!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				break;
			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
//			if(reg&32)
			if(0)
			{
				opw3=0x47000|((reg&15)<<4)|((reg&32)<<5);
				opw4=0x5080D|((reg&16)<<6);
			}else
			{
				opw3=0xF080|((reg&16)<<2);
				opw4=0x0D00|((reg&15)<<4);
			}
			if(reg&32)
			{
				BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0400,
					&opw3, &opw4, &opw5, &opw6);
				if(opw5>=0)
					opw2-=4;
			}
			break;
		}

//		if(!BGBCC_JX2_CheckPadCross48(ctx))
		if(!BGBCC_JX2_CheckPadCross48(ctx) ||
			BGBCC_JX2_CheckPadCross32(ctx))
		{
			if(prlty==BGBCC_SH_RLC_REL24_BJX)
			{
				if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
					break;
				rlty2=prlty;
				opw1=0xFC27|((reg&16)<<4);
				opw2=0xD000|((reg&15)<<8);
				opw3=0x0000;
				break;
			}
		}

		rlty=prlty;
		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
//			{ opwf=0x3505|((reg&15)<<4); break; }
			{ opwf=0x5300|((reg&15)<<4); break; }

		if(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
		{
			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
			opw3=0xF080|((reg&16)<<2);
			opw4=0x0D00|((reg&15)<<4);
			break;
		}
		break;
	case BGBCC_SH_NMID_MOVUL:
//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2 ||
			(ctx->emit_riscv&0x22) ||
			!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				break;
			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
//			if(reg&32)
			if(0)
			{
				opw3=0x47000|((reg&15)<<4)|((reg&32)<<5);
				opw4=0x5080E|((reg&16)<<6);
			}else
			{
				opw3=0xF080|((reg&16)<<2);
				opw4=0x0E00|((reg&15)<<4);
			}
			if(reg&32)
			{
				BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0400,
					&opw3, &opw4, &opw5, &opw6);
				if(opw5>=0)
					opw2-=4;
			}
			break;
		}

//		if(!BGBCC_JX2_CheckPadCross48(ctx))
		if(!BGBCC_JX2_CheckPadCross48(ctx) ||
			BGBCC_JX2_CheckPadCross32(ctx))
		{
			if(prlty==BGBCC_SH_RLC_REL24_BJX)
			{
				if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
					break;
				rlty2=prlty;
				opw1=0xFC27|((reg&16)<<4);
				opw2=0xE000|((reg&15)<<8);
				opw3=0x0000;
				break;
			}
		}

		rlty=prlty;
		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
//			{ opwf=0x3506|((reg&15)<<4); break; }
			{ opwf=0x8800|((reg&15)<<4); break; }

		if(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
		{
			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
			opw3=0xF080|((reg&16)<<2);
			opw4=0x0E00|((reg&15)<<4);
			break;
		}
		break;

	case BGBCC_SH_NMID_MOVX2:
//		if(ctx->is_fixed32 || ctx->op_is_wex2)
		if(1)
		{
			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
//			if(reg&32)
			if(0)
			{
				opw3=0x47000|((reg&15)<<4)|((reg&32)<<5);
				opw4=0x5400C|((reg&16)<<6);
			}else
			{
				opw3=0xF000|((reg&16)<<2);
				opw4=0x4C00|((reg&15)<<4);
			}
			if(reg&32)
			{
				BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0400,
					&opw3, &opw4, &opw5, &opw6);
				if(opw5>=0)
					opw2-=4;
			}
			break;
		}
//		rlty=prlty;
//		opwf=0x9600|((reg&15)<<4);
		break;

	case BGBCC_SH_NMID_FMOVS:
//		if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			break;
		rlty=BGBCC_SH_RLC_REL24_BJX;
		opw1=0xFBFF;
		opw2=0xFFFC;
//		if(reg&32)
		if(0)
		{
			opw3=0x47000|((reg&15)<<4)|((reg&32)<<5);
			opw4=0x5400E|((reg&16)<<6);
		}else
		{
			opw3=0xF000|((reg&16)<<2);
			opw4=0x4E00|((reg&15)<<4);
		}
		if(reg&32)
		{
			BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0400,
				&opw3, &opw4, &opw5, &opw6);
			if(opw5>=0)
				opw2-=4;
		}
		break;

//	case BGBCC_SH_NMID_FMOVS:
	case BGBCC_SH_NMID_FMOVD:
		BGBCC_DBGBREAK
		break;

#if 0
//	case BGBCC_SH_NMID_FMOVS:
//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2 || (ctx->emit_riscv&0x22))
		{
			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
			opw3=0xF000|((reg&16)<<2);
			opw4=0x9600|((reg&15)<<4);
			break;
		}
		rlty=prlty;
		opwf=0x9600|((reg&15)<<4);
		break;
//	case BGBCC_SH_NMID_FMOVD:
//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2 || (ctx->emit_riscv&0x22))
		{
			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
			opw3=0xF000|((reg&16)<<2);
			opw4=0x9700|((reg&15)<<4);
			break;
		}
		rlty=prlty;
		opwf=0x9700|((reg&15)<<4);
		break;
#endif
	}
	
	if((rlty>=0) && (opw1<0) && (opwf>=0))
	{
//		if(rlty==BGBCC_SH_RLC_RELW8C_BSR)
//			{ opw1=0x25FE; opw2=opwf; }
//		if(rlty==BGBCC_SH_RLC_RELW16C_BSR)
//			{ opw1=0x25FF; opw2=0x26FE; opw3=opwf; }
//		if(rlty==BGBCC_SH_RLC_RELW16C_BSR)
//			{ opw1=0xF0FF; opw2=0x25FE; opw3=opwf; }
//		if(rlty==BGBCC_SH_RLC_RELW24C_BSR)
//			{ opw1=0x25FF; opw2=0x26FF; opw3=0x26FE; opw4=opwf; }
//		if(rlty==BGBCC_SH_RLC_RELW32C_BSR)
//			{ opw1=0x25FF; opw2=0x26FF; opw3=0x26FE; opw4=0x26FE; opw5=opwf; }

//		if(rlty==BGBCC_SH_RLC_RELW16A_BSR)
//			{ opw1=0xA000; opw2=opwf; }
//		if(rlty==BGBCC_SH_RLC_RELW24A_BSR)
//			{ opw1=0xA000; opw2=0x2600; opw3=opwf; }
//		if(rlty==BGBCC_SH_RLC_RELW32A_BSR)
//			{ opw1=0xA000; opw2=0x2600; opw3=0x2600; opw4=opwf; }

		if(rlty==BGBCC_SH_RLC_REL24_BJX)
			{ opw1=0xFBFF; opw2=0xFFFE; opw3=opwf; }
	}

	if(opw1>=0)
	{
		BGBCC_JX2DA_EmitLoadRegLabelVarRel24(ctx, nmid, reg, lbl);

		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2, 0);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		if(rlty>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(rlty2>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty2); }
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);

//		if(((opw1&0xFE00)>=0xF000) && ((opw1&0xFE00)<0xFC00))
		if(((opw1&0xFE00)>=0xF000) && ((opw1&0xFE00)<=0xFE00))
		{
			BGBCC_JX2_EmitOpCheckRepack(ctx, &opw3, &opw4, 0);
			BGBCC_JX2_EmitPadForOpWord(ctx, opw3);
		}

		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
		if(opw5>=0)
			BGBCC_JX2_EmitWord(ctx, opw5);
		if(opw6>=0)
			BGBCC_JX2_EmitWord(ctx, opw6);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}

//	BGBCC_DBGBREAK
	return(0);

//	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
//	return(0);
}

int BGBCC_JX2_EmitStoreRegLabelVarRel24(
	BGBCC_JX2_Context *ctx, int nmid, int reg, int lbl)
{
	int opw1, opw2, opw3, opw4, opw5, opw6, opwf, rlty, prlty, rlty2;
	int tr0;
	int i, j;

	if(nmid==BGBCC_SH_NMID_MOVX2)
	{
		reg=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, reg);
	}

#if 0
	if(	BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg) &&
		!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
	{
		j=-1;
		
		if(nmid==BGBCC_SH_NMID_MOVX2)
		{
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx->tctx, ctx, 
				BGBCC_SH_REGCLS_QGR2);

			BGBCC_JX2_EmitOpRegReg(ctx, BGBCC_SH_NMID_MOVX2,
				reg, tr0);
			j=BGBCC_JX2_EmitStoreRegLabelVarRel24(ctx, nmid, tr0, lbl);
			BGBCC_JX2C_ScratchReleaseReg(ctx->tctx, ctx, tr0);
			return(j);
		}else
		{
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx->tctx, ctx, 
				BGBCC_SH_REGCLS_QGR);

			BGBCC_JX2_EmitOpRegReg(ctx, BGBCC_SH_NMID_MOV,
				reg, tr0);
			j=BGBCC_JX2_EmitStoreRegLabelVarRel24(ctx, nmid, tr0, lbl);
			BGBCC_JX2C_ScratchReleaseReg(ctx->tctx, ctx, tr0);
			return(j);
		}
	}
#endif

	if((lbl>=BGBCC_SH_LBL_ARCHSTRT) && (lbl<BGBCC_SH_LBL_ARCHEND))
	{
		tr0=lbl-BGBCC_SH_LBL_ARCHSTRT;
		BGBCC_JX2_EmitOpRegReg(ctx,
			BGBCC_SH_NMID_MOV, reg, tr0);
		return(1);
	}

	if((lbl>=BGBCC_SH_LBL_TLSSTRT) && (lbl<BGBCC_SH_LBL_TLSEND))
	{
		nmid=BGBCC_JX2_EmitRemapPseudoOp(ctx, nmid);
		return(BGBCC_JX2_EmitStoreRegLabelVarTls(ctx,
			nmid, reg, lbl));
	}

//	if(nmid==BGBCC_SH_NMID_FMOVS)
//	{
//		if(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
//			nmid=BGBCC_SH_NMID_MOVL;
//	}

	if(nmid==BGBCC_SH_NMID_FMOVD)
	{
//		if(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
//			nmid=BGBCC_SH_NMID_MOVD;
	}

	nmid=BGBCC_JX2_EmitRemapPseudoOp(ctx, nmid);

//	ctx->test_lclalign=3;

#if 1
	if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
//	if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl) && (nmid==BGBCC_SH_NMID_MOVB))
	{
		i=BGBCC_JX2_TryEmitStoreRegLabelVarPbo24(ctx, nmid, reg, lbl);
		if(i>0)
			return(i);
		BGBCC_DBGBREAK
		return(0);
	}
#endif

#if 0
	if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
	{
		if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
		{
			tr0=BGBCC_JX2C_ScratchAllocTsReg(NULL, ctx, BGBCC_SH_REGCLS_GR);
			BGBCC_JX2_EmitOpRegReg(ctx,
				BGBCC_SH_NMID_MOV, reg, tr0);
			BGBCC_JX2_EmitStoreRegLabelVarRel24(ctx, nmid, tr0, lbl);
			BGBCC_JX2C_ScratchReleaseReg(NULL, ctx, tr0);
			return(1);
		}

		BGBCC_DBGBREAK
	}
#endif

	prlty=BGBCC_SH_RLC_REL24_BJX;

	if(BGBCC_JX2_EmitCheckAutoLabelNear8(ctx, lbl, nmid))
	{
//		prlty=BGBCC_SH_RLC_RELW8_BSR;
	}
	else if(BGBCC_JX2_ModelIsLabel16P(ctx))
	{
//		prlty=BGBCC_SH_RLC_RELW16C_BSR;
//		prlty=BGBCC_SH_RLC_RELW16A_BSR;
//		if((reg&15)==15)
//			prlty=BGBCC_SH_RLC_RELW16C_BSR;
	}
	else if(BGBCC_JX2_ModelIsData24P(ctx))
	{
#if 0
//		prlty=BGBCC_SH_RLC_RELW16C_BSR;
		prlty=BGBCC_SH_RLC_RELW24A_BSR;
		if((reg&15)==15)
			prlty=BGBCC_SH_RLC_RELW24C_BSR;
#endif
		prlty=BGBCC_SH_RLC_REL24_BJX;
//		if(BGBCC_JX2_CheckPadCross48(ctx))
//			prlty=BGBCC_SH_RLC_RELW24C_BSR;
		if(BGBCC_JX2_CheckPadCross32(ctx))
			prlty=BGBCC_SH_RLC_RELW24C_BSR;
	}
	else
		{ prlty=BGBCC_SH_RLC_RELW32C_BSR; }

	opw1=-1; opw2=-1; opw3=-1; opw4=-1; opw5=-1; opw6=-1;
	rlty=-1; rlty2=-1;
	opwf=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVUB:
	case BGBCC_SH_NMID_MOVB:
//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2 ||
			(ctx->emit_riscv&0x22) ||
			!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				break;

			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
//			if(reg&32)
			if(0)
			{
				opw3=0x47000|((reg&15)<<4)|((reg&32)<<5);
				opw4=0x50004|((reg&16)<<6);
			}else
			{
				opw3=0xF000|((reg&16)<<1);
				opw4=0x0400|((reg&15)<<0);
			}
			if(reg&32)
			{
				BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0200,
					&opw3, &opw4, &opw5, &opw6);
				if(opw5>=0)
					opw2-=4;
			}
			break;
		}
	
		if(!BGBCC_JX2_CheckPadCross48(ctx))
		{
			if(prlty==BGBCC_SH_RLC_REL24_BJX)
			{
				if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
					break;

				rlty2=prlty;
				opw1=0xFC27|((reg&16)<<4);
				opw2=0x0000|((reg&15)<<8);
				opw3=0x0000;
				break;
			}
		}

		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if((prlty==BGBCC_SH_RLC_RELW16A_BSR) ||
//				(prlty==BGBCC_SH_RLC_RELW24A_BSR) ||
//				(prlty==BGBCC_SH_RLC_RELW32A_BSR))
//				{ rlty=prlty; opwf=0x4000|((reg&15)<<4); break; }
			rlty=prlty;
//			opwf=0x3004|((reg&15)<<4);
			opwf=0x0400|((reg&15)<<0);
			break;
		}

		if(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
		{
			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
			opw3=0xF000|((reg&16)<<1);
			opw4=0x0400|((reg&15)<<0);
			break;
		}
		break;

	case BGBCC_SH_NMID_MOVUW:
	case BGBCC_SH_NMID_MOVW:
//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2 || (ctx->emit_riscv&0x22) ||
			!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				break;

			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
//			if(reg&32)
			if(0)
			{
				opw3=0x47000|((reg&15)<<4)|((reg&32)<<5);
				opw4=0x50005|((reg&16)<<6);
			}else
			{
				opw3=0xF000|((reg&16)<<1);
				opw4=0x0500|((reg&15)<<0);
			}
			if(reg&32)
			{
				BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0200,
					&opw3, &opw4, &opw5, &opw6);
				if(opw5>=0)
					opw2-=4;
			}
			break;
		}

		if(!BGBCC_JX2_CheckPadCross48(ctx))
		{
			if(prlty==BGBCC_SH_RLC_REL24_BJX)
			{
				if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
					break;

				rlty2=prlty;
				opw1=0xFC27|((reg&16)<<4);
				opw2=0x1000|((reg&15)<<8);
				opw3=0x0000;
				break;
			}
		}

		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if((prlty==BGBCC_SH_RLC_RELW16A_BSR) ||
//				(prlty==BGBCC_SH_RLC_RELW24A_BSR) ||
//				(prlty==BGBCC_SH_RLC_RELW32A_BSR))
//				{ rlty=prlty; opwf=0x4100|((reg&15)<<4); break; }
			rlty=prlty;
//			opwf=0x3005|((reg&15)<<4);
			opwf=0x0500|((reg&15)<<0);
			break;
		}

		if(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
		{
			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
			opw3=0xF000|((reg&16)<<1);
			opw4=0x0500|((reg&15)<<0);
			break;
		}
		break;

	case BGBCC_SH_NMID_MOVUL:
	case BGBCC_SH_NMID_MOVL:
//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2 || (ctx->emit_riscv&0x22) ||
			!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				break;

			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
//			if(reg&32)
			if(0)
			{
				opw3=0x47000|((reg&15)<<4)|((reg&32)<<5);
				opw4=0x50006|((reg&16)<<6);
			}else
			{
				opw3=0xF000|((reg&16)<<1);
				opw4=0x0600|((reg&15)<<0);
			}
			if(reg&32)
			{
				BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0200,
					&opw3, &opw4, &opw5, &opw6);
				if(opw5>=0)
					opw2-=4;
			}
			break;
		}

		if(!BGBCC_JX2_CheckPadCross48(ctx))
		{
			if(prlty==BGBCC_SH_RLC_REL24_BJX)
			{
				if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
					break;

				rlty2=prlty;
				opw1=0xFC27|((reg&16)<<4);
				opw2=0x2000|((reg&15)<<8);
				opw3=0x0000;
				break;
			}
		}

		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if((prlty==BGBCC_SH_RLC_RELW16A_BSR) ||
//				(prlty==BGBCC_SH_RLC_RELW24A_BSR) ||
//				(prlty==BGBCC_SH_RLC_RELW32A_BSR))
//				{ rlty=prlty; opwf=0x4200|((reg&15)<<4); break; }
			rlty=prlty;
//			opwf=0x3006|((reg&15)<<4);
			opwf=0x0600|((reg&15)<<0);
			break;
		}

		if(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
		{
			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
			opw3=0xF000|((reg&16)<<1);
			opw4=0x0600|((reg&15)<<0);
			break;
		}
		break;

	case BGBCC_SH_NMID_MOVQ:
//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2 || (ctx->emit_riscv&0x22) ||
			!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				break;

			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
//			if(reg&32)
			if(0)
			{
				opw3=0x47000|((reg&15)<<4)|((reg&32)<<5);
				opw4=0x50007|((reg&16)<<6);
			}else
			{
				opw3=0xF000|((reg&16)<<1);
				opw4=0x0700|((reg&15)<<0);
			}
			if(reg&32)
			{
				BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0200,
					&opw3, &opw4, &opw5, &opw6);
				if(opw5>=0)
					opw2-=4;
			}
			break;
		}

		if(!BGBCC_JX2_CheckPadCross48(ctx))
		{
			if(prlty==BGBCC_SH_RLC_REL24_BJX)
			{
				if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
					break;

				rlty2=prlty;
				opw1=0xFC27|((reg&16)<<4);
				opw2=0x3000|((reg&15)<<8);
				opw3=0x0000;
				break;
			}
		}

		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
//			if((prlty==BGBCC_SH_RLC_RELW16A_BSR) ||
//				(prlty==BGBCC_SH_RLC_RELW24A_BSR) ||
//				(prlty==BGBCC_SH_RLC_RELW32A_BSR))
//				{ rlty=prlty; opwf=0x4300|((reg&15)<<4); break; }
			rlty=prlty;
//			opwf=0x3007|((reg&15)<<4);
			opwf=0x0700|((reg&15)<<0);
			break;
		}

		if(BGBCC_JX2_EmitCheckRegExt32GPR(ctx, reg))
		{
			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
			opw3=0xF000|((reg&16)<<1);
			opw4=0x0700|((reg&15)<<0);
			break;
		}
		break;

	case BGBCC_SH_NMID_MOVX2:
//		if(ctx->is_fixed32 || ctx->op_is_wex2)
		if(1)
		{
			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
			opw3=0xF000|((reg&16)<<1);
			opw4=0x4400|((reg&15)<<0);
			break;
		}

		if(reg&32)
		{
//			BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0200,
//				&opw3, &opw4, &opw5, &opw6);
//			if(opw5>=0)
//				opw2-=4;
		}

//		rlty=prlty;
//		opwf=0x9600|((reg&15)<<4);
		break;

	case BGBCC_SH_NMID_FMOVS:
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			break;

		rlty=BGBCC_SH_RLC_REL24_BJX;
		opw1=0xFBFF;
		opw2=0xFFFC;
		opw3=0xF000|((reg&16)<<1);
		opw4=0x4600|((reg&15)<<0);

		if(reg&32)
		{
			BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, 0x0200,
				&opw3, &opw4, &opw5, &opw6);
			if(opw5>=0)
				opw2-=4;
		}
		break;

//	case BGBCC_SH_NMID_FMOVS:
	case BGBCC_SH_NMID_FMOVD:
		BGBCC_DBGBREAK
		break;

#if 0
//	case BGBCC_SH_NMID_FMOVS:
//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2 || (ctx->emit_riscv&0x22))
		{
			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
			opw3=0xF000|((reg&16)<<1);
			opw4=0x9200|((reg&15)<<0);
			break;
		}
		rlty=prlty;
		opwf=0x9200|((reg&15)<<0);
		break;
	case BGBCC_SH_NMID_FMOVD:
//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2 || (ctx->emit_riscv&0x22))
		{
			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
			opw3=0xF000|((reg&16)<<1);
			opw4=0x9300|((reg&15)<<0);
			break;
		}
		rlty=prlty;
		opwf=0x9300|((reg&15)<<0);
		break;
#endif
	}
	
	if((rlty>=0) && (opw1<0) && (opwf>=0))
	{
//		if(rlty==BGBCC_SH_RLC_RELW8C_BSR)
//			{ opw1=0x25FE; opw2=opwf; }
//		if(rlty==BGBCC_SH_RLC_RELW16C_BSR)
//			{ opw1=0x25FF; opw2=0x26FE; opw3=opwf; }
//		if(rlty==BGBCC_SH_RLC_RELW16C_BSR)
//			{ opw1=0xF0FF; opw2=0x25FE; opw3=opwf; }
//		if(rlty==BGBCC_SH_RLC_RELW24C_BSR)
//			{ opw1=0x25FF; opw2=0x26FF; opw3=0x26FE; opw4=opwf; }
//		if(rlty==BGBCC_SH_RLC_RELW32C_BSR)
//			{ opw1=0x25FF; opw2=0x26FF; opw3=0x26FE; opw4=0x26FE; opw5=opwf; }

//		if(rlty==BGBCC_SH_RLC_RELW16A_BSR)
//			{ opw1=0xA000; opw2=opwf; }
//		if(rlty==BGBCC_SH_RLC_RELW24A_BSR)
//			{ opw1=0xA000; opw2=0x26FF; opw3=opwf; }
//		if(rlty==BGBCC_SH_RLC_RELW32A_BSR)
//			{ opw1=0xA000; opw2=0x26FF; opw3=0x26FF; opw4=opwf; }

		if(rlty==BGBCC_SH_RLC_REL24_BJX)
			{ opw1=0xFBFF; opw2=0xFFFE; opw3=opwf; }
	}

	if(opw1>=0)
	{
		BGBCC_JX2DA_EmitStoreRegLabelVarRel24(ctx, nmid, reg, lbl);

		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2, 0);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		if(rlty>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(rlty2>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty2); }
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);

//		if(((opw1&0xFE00)>=0xF000) && ((opw1&0xFE00)<0xFC00))
		if(((opw1&0xFE00)>=0xF000) && ((opw1&0xFE00)<=0xFE00))
		{
			BGBCC_JX2_EmitOpCheckRepack(ctx, &opw3, &opw4, 0);
			BGBCC_JX2_EmitPadForOpWord(ctx, opw3);
		}

		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
		if(opw5>=0)
			BGBCC_JX2_EmitWord(ctx, opw5);
		if(opw6>=0)
			BGBCC_JX2_EmitWord(ctx, opw6);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}

	BGBCC_DBGBREAK
	return(0);
}

static byte bgbcc_jx2_wshuftab[256][8]={
{8, 7, 4, 0, 0, 0, 0, 0}, /* 00(0,0,0,0) */
{8, 7, 1, 0, 0, 0, 0, 0}, /* 01(1,0,0,0) */
{8, 4, 3, 0, 0, 0, 0, 0}, /* 02(2,0,0,0) */
{7, 4, 6, 0, 0, 0, 0, 0}, /* 03(3,0,0,0) */
{8, 7, 0, 0, 0, 0, 0, 0}, /* 04(0,1,0,0) */
{8, 5, 1, 4, 0, 0, 0, 0}, /* 05(1,1,0,0) */
{8, 5, 0, 0, 0, 0, 0, 0}, /* 06(2,1,0,0) */
{7, 6, 0, 0, 0, 0, 0, 0}, /* 07(3,1,0,0) */
{8, 4, 3, 1, 0, 0, 0, 0}, /* 08(0,2,0,0) */
{8, 5, 1, 0, 0, 0, 0, 0}, /* 09(1,2,0,0) */
{4, 3, 4, 0, 0, 0, 0, 0}, /* 0A(2,2,0,0) */
{4, 3, 1, 0, 0, 0, 0, 0}, /* 0B(3,2,0,0) */
{7, 4, 3, 0, 0, 0, 0, 0}, /* 0C(0,3,0,0) */
{7, 6, 1, 0, 0, 0, 0, 0}, /* 0D(1,3,0,0) */
{4, 3, 0, 0, 0, 0, 0, 0}, /* 0E(2,3,0,0) */
{7, 6, 4, 0, 0, 0, 0, 0}, /* 0F(3,3,0,0) */
{8, 7, 3, 2, 0, 0, 0, 0}, /* 10(0,0,1,0) */
{8, 1, 7, 0, 0, 0, 0, 0}, /* 11(1,0,1,0) */
{8, 1, 3, 0, 0, 0, 0, 0}, /* 12(2,0,1,0) */
{7, 3, 2, 0, 0, 0, 0, 0}, /* 13(3,0,1,0) */
{8, 1, 7, 1, 0, 0, 0, 0}, /* 14(0,1,1,0) */
{1, 8, 7, 3, 0, 0, 0, 0}, /* 15(1,1,1,0) */
{1, 8, 3, 0, 0, 0, 0, 0}, /* 16(2,1,1,0) */
{2, 8, 3, 0, 0, 0, 0, 0}, /* 17(3,1,1,0) */
{8, 3, 2, 0, 0, 0, 0, 0}, /* 18(0,2,1,0) */
{2, 7, 3, 0, 0, 0, 0, 0}, /* 19(1,2,1,0) */
{3, 4, 2, 0, 0, 0, 0, 0}, /* 1A(2,2,1,0) */
{3, 2, 0, 0, 0, 0, 0, 0}, /* 1B(3,2,1,0) */
{7, 1, 3, 0, 0, 0, 0, 0}, /* 1C(0,3,1,0) */
{1, 7, 3, 0, 0, 0, 0, 0}, /* 1D(1,3,1,0) */
{1, 3, 0, 0, 0, 0, 0, 0}, /* 1E(2,3,1,0) */
{3, 2, 4, 0, 0, 0, 0, 0}, /* 1F(3,3,1,0) */
{8, 4, 0, 0, 0, 0, 0, 0}, /* 20(0,0,2,0) */
{8, 1, 0, 0, 0, 0, 0, 0}, /* 21(1,0,2,0) */
{8, 1, 3, 7, 0, 0, 0, 0}, /* 22(2,0,2,0) */
{4, 6, 0, 0, 0, 0, 0, 0}, /* 23(3,0,2,0) */
{8, 0, 0, 0, 0, 0, 0, 0}, /* 24(0,1,2,0) */
{6, 1, 4, 0, 0, 0, 0, 0}, /* 25(1,1,2,0) */
{6, 5, 7, 0, 0, 0, 0, 0}, /* 26(2,1,2,0) */
{6, 0, 0, 0, 0, 0, 0, 0}, /* 27(3,1,2,0) */
{8, 3, 8, 2, 0, 0, 0, 0}, /* 28(0,2,2,0) */
{5, 8, 2, 0, 0, 0, 0, 0}, /* 29(1,2,2,0) */
{3, 8, 4, 2, 0, 0, 0, 0}, /* 2A(2,2,2,0) */
{3, 8, 2, 0, 0, 0, 0, 0}, /* 2B(3,2,2,0) */
{4, 6, 1, 0, 0, 0, 0, 0}, /* 2C(0,3,2,0) */
{6, 1, 0, 0, 0, 0, 0, 0}, /* 2D(1,3,2,0) */
{1, 3, 7, 0, 0, 0, 0, 0}, /* 2E(2,3,2,0) */
{6, 4, 0, 0, 0, 0, 0, 0}, /* 2F(3,3,2,0) */
{7, 4, 2, 0, 0, 0, 0, 0}, /* 30(0,0,3,0) */
{7, 2, 0, 0, 0, 0, 0, 0}, /* 31(1,0,3,0) */
{4, 6, 3, 0, 0, 0, 0, 0}, /* 32(2,0,3,0) */
{4, 6, 7, 0, 0, 0, 0, 0}, /* 33(3,0,3,0) */
{7, 2, 1, 0, 0, 0, 0, 0}, /* 34(0,1,3,0) */
{5, 2, 4, 0, 0, 0, 0, 0}, /* 35(1,1,3,0) */
{6, 5, 0, 0, 0, 0, 0, 0}, /* 36(2,1,3,0) */
{6, 7, 0, 0, 0, 0, 0, 0}, /* 37(3,1,3,0) */
{4, 5, 2, 0, 0, 0, 0, 0}, /* 38(0,2,3,0) */
{5, 2, 0, 0, 0, 0, 0, 0}, /* 39(1,2,3,0) */
{5, 4, 2, 0, 0, 0, 0, 0}, /* 3A(2,2,3,0) */
{3, 2, 7, 0, 0, 0, 0, 0}, /* 3B(3,2,3,0) */
{4, 6, 7, 1, 0, 0, 0, 0}, /* 3C(0,3,3,0) */
{6, 7, 1, 0, 0, 0, 0, 0}, /* 3D(1,3,3,0) */
{6, 4, 5, 0, 0, 0, 0, 0}, /* 3E(2,3,3,0) */
{6, 7, 4, 0, 0, 0, 0, 0}, /* 3F(3,3,3,0) */
{8, 7, 3, 0, 0, 0, 0, 0}, /* 40(0,0,0,1) */
{7, 1, 8, 0, 0, 0, 0, 0}, /* 41(1,0,0,1) */
{8, 3, 0, 0, 0, 0, 0, 0}, /* 42(2,0,0,1) */
{7, 3, 1, 0, 0, 0, 0, 0}, /* 43(3,0,0,1) */
{7, 1, 8, 1, 0, 0, 0, 0}, /* 44(0,1,0,1) */
{5, 1, 8, 4, 0, 0, 0, 0}, /* 45(1,1,0,1) */
{5, 1, 8, 1, 0, 0, 0, 0}, /* 46(2,1,0,1) */
{6, 2, 8, 1, 0, 0, 0, 0}, /* 47(3,1,0,1) */
{8, 3, 1, 0, 0, 0, 0, 0}, /* 48(0,2,0,1) */
{5, 1, 8, 0, 0, 0, 0, 0}, /* 49(1,2,0,1) */
{3, 4, 0, 0, 0, 0, 0, 0}, /* 4A(2,2,0,1) */
{3, 1, 0, 0, 0, 0, 0, 0}, /* 4B(3,2,0,1) */
{7, 3, 0, 0, 0, 0, 0, 0}, /* 4C(0,3,0,1) */
{6, 2, 8, 0, 0, 0, 0, 0}, /* 4D(1,3,0,1) */
{3, 0, 0, 0, 0, 0, 0, 0}, /* 4E(2,3,0,1) */
{3, 1, 4, 0, 0, 0, 0, 0}, /* 4F(3,3,0,1) */
{7, 1, 8, 5, 0, 0, 0, 0}, /* 50(0,0,1,1) */
{1, 8, 7, 0, 0, 0, 0, 0}, /* 51(1,0,1,1) */
{1, 8, 5, 0, 0, 0, 0, 0}, /* 52(2,0,1,1) */
{2, 8, 5, 0, 0, 0, 0, 0}, /* 53(3,0,1,1) */
{1, 8, 7, 1, 0, 0, 0, 0}, /* 54(0,1,1,1) */
{1, 8, 7, 4, 0, 0, 0, 0}, /* 55(1,1,1,1) */
{1, 8, 4, 3, 0, 0, 0, 0}, /* 56(2,1,1,1) */
{2, 8, 4, 3, 0, 0, 0, 0}, /* 57(3,1,1,1) */
{1, 8, 5, 1, 0, 0, 0, 0}, /* 58(0,2,1,1) */
{2, 7, 4, 3, 0, 0, 0, 0}, /* 59(1,2,1,1) */
{1, 4, 3, 4, 0, 0, 0, 0}, /* 5A(2,2,1,1) */
{2, 4, 3, 0, 0, 0, 0, 0}, /* 5B(3,2,1,1) */
{2, 8, 5, 1, 0, 0, 0, 0}, /* 5C(0,3,1,1) */
{1, 7, 4, 3, 0, 0, 0, 0}, /* 5D(1,3,1,1) */
{1, 4, 3, 0, 0, 0, 0, 0}, /* 5E(2,3,1,1) */
{2, 4, 3, 4, 0, 0, 0, 0}, /* 5F(3,3,1,1) */
{8, 5, 3, 0, 0, 0, 0, 0}, /* 60(0,0,2,1) */
{1, 8, 0, 0, 0, 0, 0, 0}, /* 61(1,0,2,1) */
{6, 3, 7, 0, 0, 0, 0, 0}, /* 62(2,0,2,1) */
{1, 6, 0, 0, 0, 0, 0, 0}, /* 63(3,0,2,1) */
{1, 8, 1, 0, 0, 0, 0, 0}, /* 64(0,1,2,1) */
{1, 8, 4, 0, 0, 0, 0, 0}, /* 65(1,1,2,1) */
{2, 4, 6, 7, 0, 0, 0, 0}, /* 66(2,1,2,1) */
{1, 4, 6, 0, 0, 0, 0, 0}, /* 67(3,1,2,1) */
{5, 8, 3, 0, 0, 0, 0, 0}, /* 68(0,2,2,1) */
{5, 7, 1, 8, 0, 0, 0, 0}, /* 69(1,2,2,1) */
{3, 7, 4, 0, 0, 0, 0, 0}, /* 6A(2,2,2,1) */
{3, 7, 1, 0, 0, 0, 0, 0}, /* 6B(3,2,2,1) */
{5, 3, 0, 0, 0, 0, 0, 0}, /* 6C(0,3,2,1) */
{6, 1, 8, 0, 0, 0, 0, 0}, /* 6D(1,3,2,1) */
{3, 7, 0, 0, 0, 0, 0, 0}, /* 6E(2,3,2,1) */
{1, 6, 4, 0, 0, 0, 0, 0}, /* 6F(3,3,2,1) */
{7, 6, 3, 0, 0, 0, 0, 0}, /* 70(0,0,3,1) */
{2, 8, 0, 0, 0, 0, 0, 0}, /* 71(1,0,3,1) */
{6, 3, 0, 0, 0, 0, 0, 0}, /* 72(2,0,3,1) */
{6, 7, 3, 0, 0, 0, 0, 0}, /* 73(3,0,3,1) */
{2, 8, 1, 0, 0, 0, 0, 0}, /* 74(0,1,3,1) */
{2, 8, 4, 0, 0, 0, 0, 0}, /* 75(1,1,3,1) */
{2, 4, 6, 0, 0, 0, 0, 0}, /* 76(2,1,3,1) */
{1, 4, 6, 7, 0, 0, 0, 0}, /* 77(3,1,3,1) */
{6, 3, 1, 0, 0, 0, 0, 0}, /* 78(0,2,3,1) */
{5, 2, 8, 0, 0, 0, 0, 0}, /* 79(1,2,3,1) */
{6, 3, 4, 0, 0, 0, 0, 0}, /* 7A(2,2,3,1) */
{3, 1, 7, 0, 0, 0, 0, 0}, /* 7B(3,2,3,1) */
{6, 7, 3, 1, 0, 0, 0, 0}, /* 7C(0,3,3,1) */
{6, 7, 1, 8, 0, 0, 0, 0}, /* 7D(1,3,3,1) */
{6, 8, 3, 0, 0, 0, 0, 0}, /* 7E(2,3,3,1) */
{6, 8, 7, 3, 0, 0, 0, 0}, /* 7F(3,3,3,1) */
{8, 4, 2, 0, 0, 0, 0, 0}, /* 80(0,0,0,2) */
{8, 2, 0, 0, 0, 0, 0, 0}, /* 81(1,0,0,2) */
{8, 3, 8, 0, 0, 0, 0, 0}, /* 82(2,0,0,2) */
{4, 2, 5, 0, 0, 0, 0, 0}, /* 83(3,0,0,2) */
{8, 2, 1, 0, 0, 0, 0, 0}, /* 84(0,1,0,2) */
{6, 2, 4, 0, 0, 0, 0, 0}, /* 85(1,1,0,2) */
{5, 8, 0, 0, 0, 0, 0, 0}, /* 86(2,1,0,2) */
{5, 6, 0, 0, 0, 0, 0, 0}, /* 87(3,1,0,2) */
{8, 3, 8, 1, 0, 0, 0, 0}, /* 88(0,2,0,2) */
{5, 8, 1, 0, 0, 0, 0, 0}, /* 89(1,2,0,2) */
{3, 8, 4, 0, 0, 0, 0, 0}, /* 8A(2,2,0,2) */
{3, 8, 1, 0, 0, 0, 0, 0}, /* 8B(3,2,0,2) */
{4, 6, 2, 0, 0, 0, 0, 0}, /* 8C(0,3,0,2) */
{6, 2, 0, 0, 0, 0, 0, 0}, /* 8D(1,3,0,2) */
{3, 8, 0, 0, 0, 0, 0, 0}, /* 8E(2,3,0,2) */
{6, 4, 2, 0, 0, 0, 0, 0}, /* 8F(3,3,0,2) */
{8, 2, 5, 0, 0, 0, 0, 0}, /* 90(0,0,1,2) */
{2, 7, 0, 0, 0, 0, 0, 0}, /* 91(1,0,1,2) */
{1, 5, 8, 0, 0, 0, 0, 0}, /* 92(2,0,1,2) */
{2, 5, 0, 0, 0, 0, 0, 0}, /* 93(3,0,1,2) */
{2, 7, 1, 0, 0, 0, 0, 0}, /* 94(0,1,1,2) */
{2, 7, 4, 0, 0, 0, 0, 0}, /* 95(1,1,1,2) */
{1, 8, 3, 8, 0, 0, 0, 0}, /* 96(2,1,1,2) */
{2, 4, 5, 0, 0, 0, 0, 0}, /* 97(3,1,1,2) */
{1, 5, 8, 1, 0, 0, 0, 0}, /* 98(0,2,1,2) */
{5, 8, 1, 7, 0, 0, 0, 0}, /* 99(1,2,1,2) */
{3, 7, 4, 2, 0, 0, 0, 0}, /* 9A(2,2,1,2) */
{3, 7, 2, 0, 0, 0, 0, 0}, /* 9B(3,2,1,2) */
{2, 5, 1, 0, 0, 0, 0, 0}, /* 9C(0,3,1,2) */
{6, 2, 7, 0, 0, 0, 0, 0}, /* 9D(1,3,1,2) */
{1, 3, 8, 0, 0, 0, 0, 0}, /* 9E(2,3,1,2) */
{2, 5, 4, 0, 0, 0, 0, 0}, /* 9F(3,3,1,2) */
{4, 3, 4, 3, 0, 0, 0, 0}, /* A0(0,0,2,2) */
{3, 4, 3, 1, 0, 0, 0, 0}, /* A1(1,0,2,2) */
{6, 3, 8, 7, 0, 0, 0, 0}, /* A2(2,0,2,2) */
{5, 4, 3, 1, 0, 0, 0, 0}, /* A3(3,0,2,2) */
{3, 4, 3, 0, 0, 0, 0, 0}, /* A4(0,1,2,2) */
{3, 7, 6, 4, 0, 0, 0, 0}, /* A5(1,1,2,2) */
{5, 8, 7, 0, 0, 0, 0, 0}, /* A6(2,1,2,2) */
{5, 7, 6, 0, 0, 0, 0, 0}, /* A7(3,1,2,2) */
{3, 8, 4, 3, 0, 0, 0, 0}, /* A8(0,2,2,2) */
{5, 8, 7, 1, 0, 0, 0, 0}, /* A9(1,2,2,2) */
{3, 8, 7, 4, 0, 0, 0, 0}, /* AA(2,2,2,2) */
{3, 8, 7, 1, 0, 0, 0, 0}, /* AB(3,2,2,2) */
{5, 4, 3, 0, 0, 0, 0, 0}, /* AC(0,3,2,2) */
{3, 7, 6, 0, 0, 0, 0, 0}, /* AD(1,3,2,2) */
{3, 8, 7, 0, 0, 0, 0, 0}, /* AE(2,3,2,2) */
{5, 7, 6, 4, 0, 0, 0, 0}, /* AF(3,3,2,2) */
{4, 2, 0, 0, 0, 0, 0, 0}, /* B0(0,0,3,2) */
{2, 0, 0, 0, 0, 0, 0, 0}, /* B1(1,0,3,2) */
{6, 3, 8, 0, 0, 0, 0, 0}, /* B2(2,0,3,2) */
{2, 5, 7, 0, 0, 0, 0, 0}, /* B3(3,0,3,2) */
{2, 1, 0, 0, 0, 0, 0, 0}, /* B4(0,1,3,2) */
{2, 4, 0, 0, 0, 0, 0, 0}, /* B5(1,1,3,2) */
{6, 5, 8, 0, 0, 0, 0, 0}, /* B6(2,1,3,2) */
{5, 6, 7, 0, 0, 0, 0, 0}, /* B7(3,1,3,2) */
{6, 3, 8, 1, 0, 0, 0, 0}, /* B8(0,2,3,2) */
{5, 7, 2, 0, 0, 0, 0, 0}, /* B9(1,2,3,2) */
{5, 7, 4, 2, 0, 0, 0, 0}, /* BA(2,2,3,2) */
{3, 8, 1, 7, 0, 0, 0, 0}, /* BB(3,2,3,2) */
{2, 5, 7, 1, 0, 0, 0, 0}, /* BC(0,3,3,2) */
{6, 8, 2, 0, 0, 0, 0, 0}, /* BD(1,3,3,2) */
{6, 8, 3, 8, 0, 0, 0, 0}, /* BE(2,3,3,2) */
{6, 8, 4, 2, 0, 0, 0, 0}, /* BF(3,3,3,2) */
{7, 4, 0, 0, 0, 0, 0, 0}, /* C0(0,0,0,3) */
{7, 1, 0, 0, 0, 0, 0, 0}, /* C1(1,0,0,3) */
{4, 5, 0, 0, 0, 0, 0, 0}, /* C2(2,0,0,3) */
{7, 3, 1, 8, 0, 0, 0, 0}, /* C3(3,0,0,3) */
{7, 0, 0, 0, 0, 0, 0, 0}, /* C4(0,1,0,3) */
{5, 1, 4, 0, 0, 0, 0, 0}, /* C5(1,1,0,3) */
{5, 0, 0, 0, 0, 0, 0, 0}, /* C6(2,1,0,3) */
{5, 6, 8, 0, 0, 0, 0, 0}, /* C7(3,1,0,3) */
{4, 5, 1, 0, 0, 0, 0, 0}, /* C8(0,2,0,3) */
{5, 1, 0, 0, 0, 0, 0, 0}, /* C9(1,2,0,3) */
{5, 4, 0, 0, 0, 0, 0, 0}, /* CA(2,2,0,3) */
{3, 1, 8, 0, 0, 0, 0, 0}, /* CB(3,2,0,3) */
{4, 6, 7, 2, 0, 0, 0, 0}, /* CC(0,3,0,3) */
{6, 7, 2, 0, 0, 0, 0, 0}, /* CD(1,3,0,3) */
{3, 1, 8, 1, 0, 0, 0, 0}, /* CE(2,3,0,3) */
{6, 7, 4, 2, 0, 0, 0, 0}, /* CF(3,3,0,3) */
{7, 1, 5, 0, 0, 0, 0, 0}, /* D0(0,0,1,3) */
{1, 7, 0, 0, 0, 0, 0, 0}, /* D1(1,0,1,3) */
{1, 5, 0, 0, 0, 0, 0, 0}, /* D2(2,0,1,3) */
{2, 5, 8, 0, 0, 0, 0, 0}, /* D3(3,0,1,3) */
{1, 7, 1, 0, 0, 0, 0, 0}, /* D4(0,1,1,3) */
{1, 7, 4, 0, 0, 0, 0, 0}, /* D5(1,1,1,3) */
{1, 4, 5, 0, 0, 0, 0, 0}, /* D6(2,1,1,3) */
{2, 8, 3, 8, 0, 0, 0, 0}, /* D7(3,1,1,3) */
{1, 5, 1, 0, 0, 0, 0, 0}, /* D8(0,2,1,3) */
{5, 1, 7, 0, 0, 0, 0, 0}, /* D9(1,2,1,3) */
{1, 5, 4, 0, 0, 0, 0, 0}, /* DA(2,2,1,3) */
{3, 2, 8, 0, 0, 0, 0, 0}, /* DB(3,2,1,3) */
{2, 5, 8, 1, 0, 0, 0, 0}, /* DC(0,3,1,3) */
{6, 8, 1, 7, 0, 0, 0, 0}, /* DD(1,3,1,3) */
{3, 2, 8, 1, 0, 0, 0, 0}, /* DE(2,3,1,3) */
{3, 2, 8, 4, 0, 0, 0, 0}, /* DF(3,3,1,3) */
{4, 0, 0, 0, 0, 0, 0, 0}, /* E0(0,0,2,3) */
{1, 0, 0, 0, 0, 0, 0, 0}, /* E1(1,0,2,3) */
{1, 5, 7, 0, 0, 0, 0, 0}, /* E2(2,0,2,3) */
{1, 6, 8, 0, 0, 0, 0, 0}, /* E3(3,0,2,3) */
{0, 0, 0, 0, 0, 0, 0, 0}, /* E4(0,1,2,3) */
{1, 4, 0, 0, 0, 0, 0, 0}, /* E5(1,1,2,3) */
{5, 7, 0, 0, 0, 0, 0, 0}, /* E6(2,1,2,3) */
{6, 8, 0, 0, 0, 0, 0, 0}, /* E7(3,1,2,3) */
{3, 8, 3, 0, 0, 0, 0, 0}, /* E8(0,2,2,3) */
{5, 7, 1, 0, 0, 0, 0, 0}, /* E9(1,2,2,3) */
{5, 7, 4, 0, 0, 0, 0, 0}, /* EA(2,2,2,3) */
{3, 7, 1, 8, 0, 0, 0, 0}, /* EB(3,2,2,3) */
{6, 4, 6, 0, 0, 0, 0, 0}, /* EC(0,3,2,3) */
{6, 8, 1, 0, 0, 0, 0, 0}, /* ED(1,3,2,3) */
{3, 7, 1, 8, 1, 0, 0, 0}, /* EE(2,3,2,3) */
{6, 8, 4, 0, 0, 0, 0, 0}, /* EF(3,3,2,3) */
{7, 6, 4, 3, 0, 0, 0, 0}, /* F0(0,0,3,3) */
{6, 7, 6, 1, 0, 0, 0, 0}, /* F1(1,0,3,3) */
{6, 4, 3, 0, 0, 0, 0, 0}, /* F2(2,0,3,3) */
{6, 7, 4, 3, 0, 0, 0, 0}, /* F3(3,0,3,3) */
{6, 7, 6, 0, 0, 0, 0, 0}, /* F4(0,1,3,3) */
{2, 4, 3, 4, 3, 0, 0, 0}, /* F5(1,1,3,3) */
{6, 8, 5, 0, 0, 0, 0, 0}, /* F6(2,1,3,3) */
{6, 8, 7, 0, 0, 0, 0, 0}, /* F7(3,1,3,3) */
{6, 4, 3, 1, 0, 0, 0, 0}, /* F8(0,2,3,3) */
{6, 8, 5, 1, 0, 0, 0, 0}, /* F9(1,2,3,3) */
{6, 4, 3, 4, 0, 0, 0, 0}, /* FA(2,2,3,3) */
{3, 1, 8, 7, 0, 0, 0, 0}, /* FB(3,2,3,3) */
{6, 7, 4, 6, 0, 0, 0, 0}, /* FC(0,3,3,3) */
{6, 8, 7, 1, 0, 0, 0, 0}, /* FD(1,3,3,3) */
{6, 8, 4, 3, 0, 0, 0, 0}, /* FE(2,3,3,3) */
{6, 8, 7, 4, 0, 0, 0, 0}, /* FF(3,3,3,3) */
};

// #define BGBCC_JX2_SHUFW_ID(w0, w1, w2, w3)	\
//	((w0) | ((w1)<<2) | ((w2)<<4) | ((w3)<<6))

int BGBCC_JX2_EmitShufDWord(BGBCC_JX2_Context *ctx, int reg, int shuf)
{
	return(BGBCC_JX2_EmitShufDWordRegReg(ctx, reg, shuf, reg));
}

int BGBCC_JX2_EmitShufDWordRegReg(BGBCC_JX2_Context *ctx,
	int sreg, int shuf, int dreg)
{
	int sr0, sr1, dr0, dr1;
//	if(!((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0))
//	{
	if(((sreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_R0) ||
		((sreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RD0) ||
		((sreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RQ0))
	{
//		dr0=BGBCC_SH_REG_RQ0+(dreg&31)+0;
//		dr1=BGBCC_SH_REG_RQ0+(dreg&31)+1;

		if((sreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_LR0)
		{
			dr0=BGBCC_SH_REG_RQ0+(dreg&0x1E)+0;
			dr1=BGBCC_SH_REG_RQ0+(dreg&0x1E)+1;
			if(dreg&1)
				{ dr0+=32; dr1+=32; }
		}else
		{
			dr0=BGBCC_SH_REG_RQ0+(dreg&0x1F)+0;
			dr1=BGBCC_SH_REG_RQ0+(dreg&0x1F)+1;
		}

		switch(shuf&0xF)
		{
		case 0x0:	case 0x2:
		case 0x8:	case 0xA:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVLD, sreg, sreg, dr0);
			break;
		case 0x1:	case 0x3:
		case 0x9:	case 0xB:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVLHD, sreg, sreg, dr0);
			break;
		case 0x4:	case 0x6:
		case 0xC:	case 0xE:
//			BGBCC_JX2_EmitOpRegReg(ctx,
//				BGBCC_SH_NMID_MOV, sreg, dr0);
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVHLD, sreg, sreg, dr0);
			break;
		case 0x5:	case 0x7:
		case 0xD:	case 0xF:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVHD, sreg, sreg, dr0);
			break;
		}

		if((dreg&BGBCC_SH_REG_RTMASK5)!=BGBCC_SH_REG_LR0)
			return(1);

		switch((shuf>>4)&0xF)
		{
		case 0x0:	case 0x2:
		case 0x8:	case 0xA:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVLD, sreg, sreg, dr1);
			break;
		case 0x1:	case 0x3:
		case 0x9:	case 0xB:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVLHD, sreg, sreg, dr1);
			break;
		case 0x4:	case 0x6:
		case 0xC:	case 0xE:
//			BGBCC_JX2_EmitOpRegReg(ctx,
//				BGBCC_SH_NMID_MOV, sreg, dr1);
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVHLD, sreg, sreg, dr1);
			break;
		case 0x5:	case 0x7:
		case 0xD:	case 0xF:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVHD, sreg, sreg, dr1);
			break;
		}

		return(1);
	}

	if((sreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_LR0)
	{
		if((sreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_LR0)
		{
			sr0=BGBCC_SH_REG_RQ0+(sreg&0x1E)+0;
			sr1=BGBCC_SH_REG_RQ0+(sreg&0x1E)+1;
			if(sreg&1)
				{ sr0+=32; sr1+=32; }
		}else
		{
			sr0=BGBCC_SH_REG_RQ0+(sreg&0x1F)+0;
			sr1=BGBCC_SH_REG_RQ0+(sreg&0x1F)+1;
		}
	
		if((sreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_LR0)
		{
			dr0=BGBCC_SH_REG_RQ0+(dreg&0x1E)+0;
			dr1=BGBCC_SH_REG_RQ0+(dreg&0x1E)+1;
			if(dreg&1)
				{ dr0+=32; dr1+=32; }
		}else
		{
			dr0=BGBCC_SH_REG_RQ0+(dreg&0x1F)+0;
			dr1=BGBCC_SH_REG_RQ0+(dreg&0x1F)+1;
		}

		switch(shuf&0x0F)
		{
		case 0x0:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVLD, sr0, sr0, dr0);
			break;
		case 0x1:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVLHD, sr0, sr0, dr0);
			break;
		case 0x2:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVLD, sr0, sr1, dr0);
			break;
		case 0x3:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVLHD, sr0, sr1, dr0);
			break;
		case 0x4:
//			BGBCC_JX2_EmitOpRegReg(ctx,
//				BGBCC_SH_NMID_MOV, sr0, dr0);
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVHLD, sr0, sr0, dr0);
			break;
		case 0x5:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVHD, sr0, sr0, dr0);
			break;
		case 0x6:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVHLD, sr0, sr1, dr0);
			break;
		case 0x7:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVHD, sr0, sr1, dr0);
			break;

		case 0x8:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVLD, sr1, sr0, dr0);
			break;
		case 0x9:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVLHD, sr1, sr0, dr0);
			break;
		case 0xA:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVLD, sr1, sr1, dr0);
			break;
		case 0xB:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVLHD, sr1, sr1, dr0);
			break;
		case 0xC:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVHLD, sr1, sr0, dr0);
			break;
		case 0xD:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVHD, sr1, sr0, dr0);
			break;
		case 0xE:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVHLD, sr1, sr1, dr0);
			break;
		case 0xF:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVHD, sr1, sr1, dr0);
			break;
		}

		if((dreg&BGBCC_SH_REG_RTMASK5)!=BGBCC_SH_REG_LR0)
			return(1);

		switch((shuf>>4)&0x0F)
		{
		case 0x0:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVLD, sr0, sr0, dr1);
			break;
		case 0x1:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVLHD, sr0, sr0, dr1);
			break;
		case 0x2:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVLD, sr0, sr1, dr1);
			break;
		case 0x3:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVLHD, sr0, sr1, dr1);
			break;
		case 0x4:
//			BGBCC_JX2_EmitOpRegReg(ctx,
//				BGBCC_SH_NMID_MOV, sr0, dr0);
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVHLD, sr0, sr0, dr1);
			break;
		case 0x5:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVHD, sr0, sr0, dr1);
			break;
		case 0x6:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVHLD, sr0, sr1, dr1);
			break;
		case 0x7:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVHD, sr0, sr1, dr1);
			break;

		case 0x8:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVLD, sr1, sr0, dr1);
			break;
		case 0x9:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVLHD, sr1, sr0, dr1);
			break;
		case 0xA:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVLD, sr1, sr1, dr1);
			break;
		case 0xB:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVLHD, sr1, sr1, dr1);
			break;
		case 0xC:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVHLD, sr1, sr0, dr1);
			break;
		case 0xD:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVHD, sr1, sr0, dr1);
			break;
		case 0xE:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVHLD, sr1, sr1, dr1);
			break;
		case 0xF:
			BGBCC_JX2_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_MOVHD, sr1, sr1, dr1);
			break;
		}
		
		return(1);
	}

	BGBCC_CCXL_StubError(ctx->tctx);
	return(0);
}


int BGBCC_JX2_EmitShufWord_IdToOp(BGBCC_JX2_Context *ctx, int id)
{
	int op;

	switch(id)
	{
	case 0: op=-1; break;
	case 1: op=BGBCC_SH_NMID_SWAPLW; break;
	case 2: op=BGBCC_SH_NMID_SWAPW; break;
	case 3: op=BGBCC_SH_NMID_SWAPL; break;
	case 4: op=BGBCC_SH_NMID_SWCPLW; break;
	case 5: op=BGBCC_SH_NMID_SWAPMW; break;
	case 6: op=BGBCC_SH_NMID_SWAPHW; break;
	case 7: op=BGBCC_SH_NMID_SWCPMW; break;
	case 8: op=BGBCC_SH_NMID_SWCPHW; break;

	default: op=-1; break;
	}
	return(op);
}

int BGBCC_JX2_EmitShufByte_IdToOp(BGBCC_JX2_Context *ctx, int id)
{
	int op;

	switch(id)
	{
	case 0: op=-1; break;
	case 1: op=BGBCC_SH_NMID_SWAPLB; break;
	case 2: op=BGBCC_SH_NMID_SWAPB; break;
	case 3: op=BGBCC_SH_NMID_SWAPLW; break;
	case 4: op=BGBCC_SH_NMID_SWCPLB; break;
	case 5: op=BGBCC_SH_NMID_SWAPMB; break;
	case 6: op=BGBCC_SH_NMID_SWAPHB; break;
	case 7: op=BGBCC_SH_NMID_SWCPMB; break;
	case 8: op=BGBCC_SH_NMID_SWCPHB; break;
	default: op=-1; break;
	}
	return(op);
}

int BGBCC_JX2_EmitShufWord(BGBCC_JX2_Context *ctx, int reg, int shuf)
{
	return(BGBCC_JX2_EmitShufWordRegReg(ctx, reg, shuf, reg));
}

int BGBCC_JX2_EmitShufByte(BGBCC_JX2_Context *ctx, int reg, int shuf)
{
	return(BGBCC_JX2_EmitShufByteRegReg(ctx, reg, shuf, reg));
}

int BGBCC_JX2_EmitShufWordRegReg(BGBCC_JX2_Context *ctx,
	int sreg, int shuf, int dreg)
{
	int op1, op2, op3, op4;
	int op5, op6, op7, op8;
	int treg;

	BGBCC_JX2_EmitOpRegImmReg(ctx, BGBCC_SH_NMID_PSHUFW, sreg, shuf, dreg);
	return(1);

#if 0
	shuf&=255;
	
	op1=BGBCC_JX2_EmitShufWord_IdToOp(ctx, bgbcc_jx2_wshuftab[shuf][0]);
	op2=BGBCC_JX2_EmitShufWord_IdToOp(ctx, bgbcc_jx2_wshuftab[shuf][1]);
	op3=BGBCC_JX2_EmitShufWord_IdToOp(ctx, bgbcc_jx2_wshuftab[shuf][2]);
	op4=BGBCC_JX2_EmitShufWord_IdToOp(ctx, bgbcc_jx2_wshuftab[shuf][3]);

	op5=BGBCC_JX2_EmitShufWord_IdToOp(ctx, bgbcc_jx2_wshuftab[shuf][4]);
	op6=BGBCC_JX2_EmitShufWord_IdToOp(ctx, bgbcc_jx2_wshuftab[shuf][5]);
	op7=BGBCC_JX2_EmitShufWord_IdToOp(ctx, bgbcc_jx2_wshuftab[shuf][6]);
	op8=BGBCC_JX2_EmitShufWord_IdToOp(ctx, bgbcc_jx2_wshuftab[shuf][7]);
	
	if(op1<0)
	{
		if(sreg!=dreg)
		{
			BGBCC_JX2_EmitOpRegReg(ctx, BGBCC_SH_NMID_MOV, sreg, dreg);
			return(1);
		}
		return(1);
	}else
	{
		treg=dreg;
//		if(BGBCC_JX2_EmitCheckRegExt4(ctx, dreg) && (op3>=0))
//			treg=BGBCC_SH_REG_R1;
	
		if(sreg!=treg)
		{
			BGBCC_JX2_EmitOpRegReg(ctx, op1, sreg, treg);
		}else
		{
			BGBCC_JX2_EmitOpReg(ctx, op1, dreg);
		}
	}	
	
	if(treg!=dreg)
	{
	//	if(op1>=0)		{ BGBCC_JX2_EmitOpReg(ctx, op1, treg); }
		if(op2>=0)
		{
			if(op3<0)
			{
				BGBCC_JX2_EmitOpRegReg(ctx, op2, treg, dreg);
				return(1);
			}
			BGBCC_JX2_EmitOpReg(ctx, op2, treg);
		}
		if(op3>=0)
		{
			if(op4<0)
			{
				BGBCC_JX2_EmitOpRegReg(ctx, op3, treg, dreg);
				return(1);
			}
			BGBCC_JX2_EmitOpReg(ctx, op3, treg);
		}
		if(op4>=0)
		{
			if(op5<0)
			{
				BGBCC_JX2_EmitOpRegReg(ctx, op4, treg, dreg);
				return(1);
			}
			BGBCC_JX2_EmitOpReg(ctx, op4, treg);
		}
		if(op5>=0)
		{
			if(op6<0)
			{
				BGBCC_JX2_EmitOpRegReg(ctx, op5, treg, dreg);
				return(1);
			}
			BGBCC_JX2_EmitOpReg(ctx, op5, treg);
		}
		if(op6>=0)		{ BGBCC_JX2_EmitOpReg(ctx, op6, treg); }
		if(op7>=0)		{ BGBCC_JX2_EmitOpReg(ctx, op7, treg); }
		if(op8>=0)		{ BGBCC_JX2_EmitOpReg(ctx, op8, treg); }

		BGBCC_JX2_EmitOpRegReg(ctx, BGBCC_SH_NMID_MOV, treg, dreg);
		return(1);
	}else
	{
	//	if(op1>=0)		{ BGBCC_JX2_EmitOpReg(ctx, op1, treg); }
		if(op2>=0)		{ BGBCC_JX2_EmitOpReg(ctx, op2, treg); }
		if(op3>=0)		{ BGBCC_JX2_EmitOpReg(ctx, op3, treg); }
		if(op4>=0)		{ BGBCC_JX2_EmitOpReg(ctx, op4, treg); }
		if(op5>=0)		{ BGBCC_JX2_EmitOpReg(ctx, op5, treg); }
		if(op6>=0)		{ BGBCC_JX2_EmitOpReg(ctx, op6, treg); }
		if(op7>=0)		{ BGBCC_JX2_EmitOpReg(ctx, op7, treg); }
		if(op8>=0)		{ BGBCC_JX2_EmitOpReg(ctx, op8, treg); }
		return(1);
	}

	return(1);
#endif
}

int BGBCC_JX2_EmitShufByteRegReg(BGBCC_JX2_Context *ctx,
	int sreg, int shuf, int dreg)
{
	int op1, op2, op3, op4;
	int op5, op6, op7, op8;
	int treg;

	BGBCC_JX2_EmitOpRegImmReg(ctx, BGBCC_SH_NMID_PSHUFB, sreg, shuf, dreg);
	return(1);

#if 0
	shuf&=255;
	
	op1=BGBCC_JX2_EmitShufByte_IdToOp(ctx, bgbcc_jx2_wshuftab[shuf][0]);
	op2=BGBCC_JX2_EmitShufByte_IdToOp(ctx, bgbcc_jx2_wshuftab[shuf][1]);
	op3=BGBCC_JX2_EmitShufByte_IdToOp(ctx, bgbcc_jx2_wshuftab[shuf][2]);
	op4=BGBCC_JX2_EmitShufByte_IdToOp(ctx, bgbcc_jx2_wshuftab[shuf][3]);

	op5=BGBCC_JX2_EmitShufByte_IdToOp(ctx, bgbcc_jx2_wshuftab[shuf][4]);
	op6=BGBCC_JX2_EmitShufByte_IdToOp(ctx, bgbcc_jx2_wshuftab[shuf][5]);
	op7=BGBCC_JX2_EmitShufByte_IdToOp(ctx, bgbcc_jx2_wshuftab[shuf][6]);
	op8=BGBCC_JX2_EmitShufByte_IdToOp(ctx, bgbcc_jx2_wshuftab[shuf][7]);
	
	if(op1<0)
	{
		if(sreg!=dreg)
		{
			BGBCC_JX2_EmitOpRegReg(ctx, BGBCC_SH_NMID_MOV, sreg, dreg);
			return(1);
		}
		return(1);
	}else
	{
		treg=dreg;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, dreg))
			treg=BGBCC_SH_REG_R1;
	
//		if(sreg!=dreg)
		if(sreg!=treg)
		{
//			BGBCC_JX2_EmitOpRegReg(ctx, op1, sreg, dreg);
			BGBCC_JX2_EmitOpRegReg(ctx, op1, sreg, treg);
		}else
		{
			BGBCC_JX2_EmitOpReg(ctx, op1, dreg);
		}
	}
	
//	if(op1>=0)		{ BGBCC_JX2_EmitOpReg(ctx, op1, treg); }
	if(op2>=0)		{ BGBCC_JX2_EmitOpReg(ctx, op2, treg); }
	if(op3>=0)		{ BGBCC_JX2_EmitOpReg(ctx, op3, treg); }
	if(op4>=0)		{ BGBCC_JX2_EmitOpReg(ctx, op4, treg); }
	if(op5>=0)		{ BGBCC_JX2_EmitOpReg(ctx, op5, treg); }
	if(op6>=0)		{ BGBCC_JX2_EmitOpReg(ctx, op6, treg); }
	if(op7>=0)		{ BGBCC_JX2_EmitOpReg(ctx, op7, treg); }
	if(op8>=0)		{ BGBCC_JX2_EmitOpReg(ctx, op8, treg); }
	
	if(treg!=dreg)
	{
		BGBCC_JX2_EmitOpRegReg(ctx, BGBCC_SH_NMID_MOV, treg, dreg);
		return(1);
	}
	
	return(1);
#endif
}

u64 *bgbcc_jx2_shuftab8b=NULL;
u64 *bgbcc_jx2_shufovf8b=NULL;

int BGBCC_JX2_InitShufByte8(BGBCC_JX2_Context *ctx)
{
	char tb[256];
	FILE *fd;
	char *base;
	u64 v;
	int i, j, k;
	
	if(bgbcc_jx2_shuftab8b)
		return(0);
	
	base=BGBCC_CCXL_GetBasePath();
	if(!base)
		return(-1);

	sprintf(tb, "%s/swaps_tab8b.dat", base);
	fd=fopen(tb, "rb");
	if(!fd)
		return(-1);
		
	bgbcc_jx2_shuftab8b=malloc(16777216*sizeof(u64));
	
	for(i=0; i<16777216; i++)
	{
		j=fread(&v, 1, 8, fd);
		if(j<=0)
			break;
		bgbcc_jx2_shuftab8b[i]=v;
	}

	j=fread(&v, 1, 8, fd);
	if(j<=0)
		v=0;
	
	k=(int)v;
	if((k>0) && (k<16777216))
	{
		bgbcc_jx2_shufovf8b=malloc(k*sizeof(u64));
		for(i=0; i<k; i++)
		{
			j=fread(&v, 1, 8, fd);
			if(j<=0)
				break;
			bgbcc_jx2_shufovf8b[i]=v;
		}
	}
	
	fclose(fd);

	return(0);
}

int BGBCC_JX2_EmitShufByte8_IdToOp(BGBCC_JX2_Context *ctx, int id)
{
	int op;

	switch(id)
	{
	case 0: op=-1; break;
	case 1: op=BGBCC_SH_NMID_SWAPLB; break;
	case 2: op=BGBCC_SH_NMID_SWAPB; break;
	case 3: op=BGBCC_SH_NMID_SWAPLW; break;
	case 4: op=BGBCC_SH_NMID_SWCPLB; break;
	case 5: op=BGBCC_SH_NMID_SWAPMB; break;
	case 6: op=BGBCC_SH_NMID_SWAPHB; break;

	case 7: op=BGBCC_SH_NMID_SWAPW; break;
	case 8: op=BGBCC_SH_NMID_SWAPL; break;
	case 9: op=BGBCC_SH_NMID_SWCPLW; break;
	case 10: op=BGBCC_SH_NMID_SWAPMW; break;
	case 11: op=BGBCC_SH_NMID_SWAPHW; break;

	case 12: op=BGBCC_SH_NMID_SWCPMB; break;
	case 13: op=BGBCC_SH_NMID_SWCPHB; break;
	case 14: op=BGBCC_SH_NMID_SWCPMW; break;
	case 15: op=BGBCC_SH_NMID_SWCPHW; break;

	default: op=-1; break;
	}
	return(op);
}

int BGBCC_JX2_EmitShufByte8B(BGBCC_JX2_Context *ctx, int reg, int shuf)
{
	u64 opv, opv1, opv2;
	int op;
	int i, j, k;

	BGBCC_JX2_InitShufByte8(ctx);
	
	if(!bgbcc_jx2_shuftab8b)
	{
		BGBCC_DBGBREAK
		return(0);
	}
	
	shuf=shuf&16777215;
	opv=bgbcc_jx2_shuftab8b[shuf];
	
	if(((opv>>60)&15)==15)
	{
		if((opv>>56)&8)
		{
			opv1=bgbcc_jx2_shufovf8b[((opv>>32)+0)&0xFFFFFF];
			opv2=bgbcc_jx2_shufovf8b[((opv>>32)+1)&0xFFFFFF];
		}else
		{
			opv1=bgbcc_jx2_shufovf8b[(opv>>32)&0xFFFFFF];
			opv2=0;
		}

		for(i=0; i<16; i++)
		{
			op=BGBCC_JX2_EmitShufByte8_IdToOp(ctx, (opv1>>(i*4))&15);
			if(op>=0)	{ BGBCC_JX2_EmitOpReg(ctx, op, reg); }
		}
		
		if(opv2)
		{
			for(i=0; i<16; i++)
			{
				op=BGBCC_JX2_EmitShufByte8_IdToOp(ctx, (opv2>>(i*4))&15);
				if(op>=0)	{ BGBCC_JX2_EmitOpReg(ctx, op, reg); }
			}
		}
		
		for(i=0; i<8; i++)
		{
			op=BGBCC_JX2_EmitShufByte8_IdToOp(ctx, (opv>>(i*4))&15);
			if(op>=0)	{ BGBCC_JX2_EmitOpReg(ctx, op, reg); }
		}
		
		return(1);
	
//		BGBCC_DBGBREAK
//		return(0);
	}
	
	for(i=0; i<16; i++)
	{
		op=BGBCC_JX2_EmitShufByte8_IdToOp(ctx, (opv>>(i*4))&15);
		if(op>=0)	{ BGBCC_JX2_EmitOpReg(ctx, op, reg); }
	}
	
	return(1);
}

int BGBCC_JX2_EmitBSwapWordRegRegZx(BGBCC_JX2_Context *ctx,
	int sreg, int dreg)
{
	BGBCC_JX2_EmitShufByteRegReg(ctx, sreg, 0xE4, dreg);
	BGBCC_JX2_EmitOpReg(ctx, BGBCC_SH_NMID_EXTUW, dreg);
	return(1);
}

int BGBCC_JX2_EmitBSwapDWordRegRegZx(BGBCC_JX2_Context *ctx,
	int sreg, int dreg)
{
	BGBCC_JX2_EmitShufByteRegReg(ctx, sreg, 0x1B, dreg);
	BGBCC_JX2_EmitOpReg(ctx, BGBCC_SH_NMID_EXTUL, dreg);
	return(1);
}

int BGBCC_JX2_EmitBSwapWordRegRegSx(BGBCC_JX2_Context *ctx,
	int sreg, int dreg)
{
	BGBCC_JX2_EmitShufByteRegReg(ctx, sreg, 0xE4, dreg);
	BGBCC_JX2_EmitOpReg(ctx, BGBCC_SH_NMID_EXTSW, dreg);
	return(1);
}

int BGBCC_JX2_EmitBSwapDWordRegRegSx(BGBCC_JX2_Context *ctx,
	int sreg, int dreg)
{
	BGBCC_JX2_EmitShufByteRegReg(ctx, sreg, 0x1B, dreg);
	BGBCC_JX2_EmitOpReg(ctx, BGBCC_SH_NMID_EXTSL, dreg);
	return(1);
}

int BGBCC_JX2_EmitBSwapQWordRegReg(BGBCC_JX2_Context *ctx,
	int sreg, int dreg)
{
//	BGBCC_JX2_EmitShufByteRegReg(ctx, sreg, 0x1B, dreg);
//	BGBCC_JX2_EmitOpReg(ctx, BGBCC_SH_NMID_SWAPLL, dreg);
//	BGBCC_JX2_EmitShufByteRegReg(ctx, sreg, 0x1B, dreg);

	if(dreg==sreg)
	{
		BGBCC_JX2_EmitOpReg(ctx, BGBCC_SH_NMID_SWAP8B, dreg);
		BGBCC_JX2_EmitOpReg(ctx, BGBCC_SH_NMID_SWAPW, dreg);
		BGBCC_JX2_EmitOpReg(ctx, BGBCC_SH_NMID_SWAPL, dreg);
	}else
	{
		BGBCC_JX2_EmitOpRegReg(ctx, BGBCC_SH_NMID_SWAP8B, sreg, dreg);
		BGBCC_JX2_EmitOpReg(ctx, BGBCC_SH_NMID_SWAPW, dreg);
		BGBCC_JX2_EmitOpReg(ctx, BGBCC_SH_NMID_SWAPL, dreg);
	}

	return(1);
}

int BGBCC_JX2_ComposeJumboImmRegF2(BGBCC_JX2_Context *ctx,
	int *ropw1, int *ropw2, int *ropw3, int *ropw4,
	int topw1, int topw2,
	s64 imm, int reg)
{
	s64 imm1;
	int opw1, opw2, opw3, opw4;

//	if(BGBCC_JX2_EmitCheckRegExt5(ctx, reg))

	if(	 BGBCC_JX2_EmitCheckRegExt5(ctx, reg) &&
		 (!(ctx->is_fixed32&2) || (ctx->op_is_wex2&12)))

	{
		if(!(ctx->op_is_wex2&12) && !(ctx->is_fixed32&2) &&
			!(ctx->emit_riscv&0x22))
		{
#if 1
			if(((topw1&0xFF00)==0xF100) || ((topw1&0xFF00)==0xF200))
			{
				opw1=0x4FE00|((imm>>24)&0x00FF);
				opw2=0x50000|((imm>>8)&0xFFFF);
				opw3=0x40000|topw1|((reg&15)<<4);
				opw4=0x50000|topw2|((reg&16)<<6)|(imm&0x00FF);

				if((imm>>63)&1)
					opw4|=0x0100;

				opw3=0x49000|(opw3&0x00FF);

				if((topw1&0xFF00)==0xF100)
					opw3|=0x0100;
				if(BGBCC_JX2_EmitCheckRegExt5(ctx, reg))
					opw3|=0x0400;

				imm1=(((u32)(opw1&0x00FF))<<24)|
					((opw2&0xFFFF)<<8)|
					(opw4&0x00FF);
				if(opw4&0x0100)
					imm1|=0xFFFFFFFF00000000LL;
				if(imm!=imm1)
					{ BGBCC_DBGBREAK }

				*ropw1=opw1;
				*ropw2=opw2;
				*ropw3=opw3;
				*ropw4=opw4;

				return(1);
			}
#endif
		}

		if((topw1&0xFF00)!=0xF200)
			{ BGBCC_DBGBREAK }
		if((topw2&0xF000)< 0xC000)
			{ BGBCC_DBGBREAK }
	
		opw1=0x4FF00;
		opw2=0x50000|((imm>>8)&0x00FF);
		opw3=0x40000|topw1|((reg&15)<<4);
		opw4=0x50000|topw2|((reg&16)<<6)|(imm&0x00FF);
			
		opw4&=~0x0300;
		opw4|=((imm>>8)&0x0300);

		if(BGBCC_JX2_EmitCheckRegExt5(ctx, reg))
			opw1|=0x0040;

		if((imm>>63)&1)
			opw1|=0x0010;

		imm1=((opw2&0x00FF)<<8)|(opw4&0x00FF);
		imm1|=((opw4&0x0300)<<8);
		if(opw3&0x0001)
//		if((opw1&0x0010) || (opw3&0x0001))
			imm1|=0xFFFFFFFFFFFC0000LL;

		if((opw3&0x0001) && !(opw1&0x0010))
			{ BGBCC_DBGBREAK }
		if(!(opw3&0x0001) && (opw1&0x0010))
			{ BGBCC_DBGBREAK }
		
//		if(opw4&0x0100)
//			imm1|=0xFFFFFFFFFFFF0000LL;

		if(imm!=imm1)
		{
			if(ctx->op_is_wex2&12)
				return(0);
		
#if 0
			if(((topw1&0xFF00)==0xF100) || ((topw1&0xFF00)==0xF200))
			{
				opw1=0x4FE00|((imm>>24)&0x00FF);
				opw2=0x50000|((imm>>8)&0xFFFF);
				opw3=0x40000|topw1|((reg&15)<<4);
				opw4=0x50000|topw2|((reg&16)<<6)|(imm&0x00FF);

				if((imm>>63)&1)
					opw4|=0x0100;

				opw3=0x49000|(opw3&0x00FF);
				if((topw1&0xFF00)==0xF100)
					opw3|=0x0100;
				if(BGBCC_JX2_EmitCheckRegExt5(ctx, reg))
					opw3|=0x0400;

				imm1=(((u32)(opw1&0x00FF))<<24)|
					((opw2&0xFFFF)<<8)|
					(opw4&0x00FF);
				if(opw4&0x0100)
					imm1|=0xFFFFFFFF00000000LL;
				if(imm!=imm1)
					{ BGBCC_DBGBREAK }

				*ropw1=opw1;
				*ropw2=opw2;
				*ropw3=opw3;
				*ropw4=opw4;

				return(1);
			}
#endif

//			BGBCC_DBGBREAK
			return(0);
		}

		*ropw1=opw1;
		*ropw2=opw2;
		*ropw3=opw3;
		*ropw4=opw4;

		return(1);
	}

	opw1=0xFE00|((imm>>24)&0x00FF);
	opw2=0x0000|((imm>>8)&0xFFFF);
	opw3=topw1|((reg&15)<<4);
	opw4=topw2|((reg&16)<<6)|(imm&0x00FF)|((imm>>63)&0x0100);

	if(BGBCC_JX2_EmitCheckRegExt5(ctx, reg))
		opw3&=~0x8000;

	*ropw1=opw1;
	*ropw2=opw2;
	*ropw3=opw3;
	*ropw4=opw4;

	imm1=(((u32)(opw1&0x00FF))<<24)|(opw2<<8)|(opw4&0x00FF);
	if(opw4&0x0100)
		imm1|=0xFFFFFFFF00000000LL;
	if(imm!=imm1)
		{ BGBCC_DBGBREAK }

	return(1);
}

int BGBCC_JX2_ComposeJumboImmRegF2B(BGBCC_JX2_Context *ctx,
	int *ropw1, int *ropw2, int *ropw3, int *ropw4,
	int topw1, int topw2,
	s64 imm, int reg)
{
	int i;
//	return(0);

	if(((u32)imm)==imm)
	{
		i=BGBCC_JX2_ComposeJumboImmRegF2(ctx,
			ropw1, ropw2, ropw3, ropw4,
			topw1, topw2, imm, reg);
		return(i);
	}
	if((imm|(~((1LL<<32)-1)))==imm)
	{
		i=BGBCC_JX2_ComposeJumboImmRegF2(ctx,
			ropw1, ropw2, ropw3, ropw4,
			topw1|0x0001, topw2, imm, reg);
		return(i);
	}
	return(0);
}

int BGBCC_JX2_ComposeJumboRegImmRegF2(BGBCC_JX2_Context *ctx,
	int *ropw1, int *ropw2, int *ropw3, int *ropw4,
	int topw1, int topw2,
	int sreg, s64 imm, int dreg)
{
	s64 imm1;
	int opw1, opw2, opw3, opw4;

	if(BGBCC_JX2_ComposeJumboCheckOpwIsStore(ctx, topw1, topw2))
		{ opw1=sreg; sreg=dreg; dreg=opw1; }

//	if(	BGBCC_JX2_EmitCheckRegExt5(ctx, dreg) ||
//		BGBCC_JX2_EmitCheckRegExt5(ctx, sreg))

	if(	(BGBCC_JX2_EmitCheckRegExt5(ctx, dreg) ||
		 BGBCC_JX2_EmitCheckRegExt5(ctx, sreg)) &&
		 (!(ctx->is_fixed32&2) || (ctx->op_is_wex2&12)))

	{
		if(!(ctx->op_is_wex2&12) && !(ctx->is_fixed32&2) &&
			!(ctx->emit_riscv&0x22))
		{
#if 1
			if(((topw1&0xFF00)==0xF100) || ((topw1&0xFF00)==0xF200))
			{
				opw1=0x4FE00|((imm>>24)&0x00FF);
				opw2=0x50000|((imm>>8)&0xFFFF);
				opw3=0x40000|topw1|((dreg&15)<<4)|((sreg&15)<<0);
				opw4=0x50000|topw2|((dreg&16)<<6)|((sreg&16)<<5)|(imm&0x00FF);
				
				opw3=0x49000|(opw3&0x00FF);
				if((topw1&0xFF00)==0xF100)
					opw3|=0x0100;
				if(BGBCC_JX2_EmitCheckRegExt5(ctx, dreg))
					opw3|=0x0400;
				if(BGBCC_JX2_EmitCheckRegExt5(ctx, sreg))
					opw3|=0x0200;

				imm1=(((u32)(opw1&0x00FF))<<24)|
					((opw2&0xFFFF)<<8)|
					(opw4&0x00FF);
				if(opw4&0x0100)
					imm1|=0xFFFFFFFF00000000LL;
				if(imm!=imm1)
					{ BGBCC_DBGBREAK }

				*ropw1=opw1;
				*ropw2=opw2;
				*ropw3=opw3;
				*ropw4=opw4;

				return(1);
			}
#endif
		}

		opw1=0x4FF00;
		opw2=0x50000|((imm>>8)&0x00FF);
		opw3=0x40000|topw1|((dreg&15)<<4)|((sreg&15)<<0);
		opw4=0x50000|topw2|((dreg&16)<<6)|((sreg&16)<<5)|(imm&0x00FF);

//		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw3, &opw4, 1);
//		if(opw3<0)
//			return(0);

		if((imm>>16)&1)
			opw4|=0x0100;
		else
			opw4&=~0x0100;

		if((imm>>63)&1)
			opw1|=0x0010;
		else
			opw1&=~0x0010;
		
		if((opw3&0xF000)!=0xF000)
			{ BGBCC_DBGBREAK }

		if(BGBCC_JX2_EmitCheckRegExt5(ctx, dreg))
			opw1|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt5(ctx, sreg))
			opw1|=0x0020;

#if 0
		if(((opw3>>16)&15)==6)
		{
			if(BGBCC_JX2_EmitCheckRegExt5(ctx, dreg))
				opw1|=0x0020;
			if(BGBCC_JX2_EmitCheckRegExt5(ctx, sreg))
				opw1|=0x0040;
		}else
		{
			if(BGBCC_JX2_EmitCheckRegExt5(ctx, dreg))
				opw1|=0x0040;
			if(BGBCC_JX2_EmitCheckRegExt5(ctx, sreg))
				opw1|=0x0020;
		}
#endif

		imm1=((opw2&0x00FF)<<8)|(opw4&0x00FF);
		if(opw4&0x0100)
			imm1|=0x0000000000010000LL;
		if(opw1&0x0010)
			imm1|=0xFFFFFFFFFFFE0000LL;

		if(imm!=imm1)
		{
			if(ctx->op_is_wex2&12)
				return(0);

#if 0
			if(((topw1&0xFF00)==0xF100) || ((topw1&0xFF00)==0xF200))
			{
				opw1=0x4FE00|((imm>>24)&0x00FF);
				opw2=0x50000|((imm>>8)&0xFFFF);
				opw3=0x40000|topw1|((dreg&15)<<4)|((sreg&15)<<0);
				opw4=0x50000|topw2|((dreg&16)<<6)|((sreg&16)<<5)|(imm&0x00FF);
				
				opw3=0x49000|(opw3&0x00FF);
				if((topw1&0xFF00)==0xF100)
					opw3|=0x0100;
				if(BGBCC_JX2_EmitCheckRegExt5(ctx, dreg))
					opw3|=0x0400;
				if(BGBCC_JX2_EmitCheckRegExt5(ctx, sreg))
					opw3|=0x0200;

				imm1=(((u32)(opw1&0x00FF))<<24)|
					((opw2&0xFFFF)<<8)|
					(opw4&0x00FF);
				if(opw4&0x0100)
					imm1|=0xFFFFFFFF00000000LL;
				if(imm!=imm1)
					{ BGBCC_DBGBREAK }

				*ropw1=opw1;
				*ropw2=opw2;
				*ropw3=opw3;
				*ropw4=opw4;

				return(1);
			}
#endif


//			BGBCC_DBGBREAK
			return(0);
		}

		*ropw1=opw1;
		*ropw2=opw2;
		*ropw3=opw3;
		*ropw4=opw4;

		return(1);
	}

	opw1=0x4FE00|((imm>>24)&0x00FF);
	opw2=0x50000|((imm>>8)&0xFFFF);
	opw3=0x40000|topw1|((dreg&15)<<4)|((sreg&15)<<0);
	opw4=0x50000|topw2|((dreg&16)<<6)|((sreg&16)<<5)|(imm&0x00FF);

	if(BGBCC_JX2_EmitCheckRegExt5(ctx, dreg))
		opw3&=~0x8000;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, sreg))
		opw3&=~0x4000;

	imm1=(((u32)(opw1&0x00FF))<<24)|((opw2&0xFFFF)<<8)|(opw4&0x00FF);
	if(opw4&0x0100)
		imm1|=0xFFFFFFFF00000000LL;
	if(imm!=imm1)
		{ BGBCC_DBGBREAK }

	*ropw1=opw1;
	*ropw2=opw2;
	*ropw3=opw3;
	*ropw4=opw4;

	return(1);
}

/* Jumbo Imm32 where only Imm32u form exists. */
int BGBCC_JX2_ComposeJumboRegImmRegF2U(BGBCC_JX2_Context *ctx,
	int *ropw1, int *ropw2, int *ropw3, int *ropw4,
	int topw1, int topw2,
	int sreg, s64 imm, int dreg)
{
	int i;
//	return(0);

	if(((u32)imm)==imm)
	{
		i=BGBCC_JX2_ComposeJumboRegImmRegF2(ctx,
			ropw1, ropw2, ropw3, ropw4,
			topw1, topw2, sreg, imm, dreg);
		return(i);
	}
	return(0);
}

/* Jumbo Imm32 where Imm32u and Imm32n forms exist via Bit 12 of OPW2. */
int BGBCC_JX2_ComposeJumboRegImmRegF2B(BGBCC_JX2_Context *ctx,
	int *ropw1, int *ropw2, int *ropw3, int *ropw4,
	int topw1, int topw2,
	int sreg, s64 imm, int dreg)
{
	int i;

//	return(0);

	if(((u32)imm)==imm)
	{
		i=BGBCC_JX2_ComposeJumboRegImmRegF2(ctx,
			ropw1, ropw2, ropw3, ropw4,
			topw1, topw2, sreg, imm, dreg);
		return(i);
	}
	if((imm|(~((1LL<<32)-1)))==imm)
	{
		if(	BGBCC_JX2_EmitCheckRegExt5(ctx, dreg) ||
			BGBCC_JX2_EmitCheckRegExt5(ctx, sreg))
		{
			if(imm<0)
			{
				i=-1;
			}
		}

		i=BGBCC_JX2_ComposeJumboRegImmRegF2(ctx,
			ropw1, ropw2, ropw3, ropw4,
//			topw1, topw2|0x1000, sreg, imm, dreg);
			topw1, topw2|0x1100, sreg, imm, dreg);
		return(i);
	}
	return(0);
}

/* Jumbo Imm32 where Imm32u and Imm32n forms exist via E.I. */
int BGBCC_JX2_ComposeJumboRegImmRegF2A(BGBCC_JX2_Context *ctx,
	int *ropw1, int *ropw2, int *ropw3, int *ropw4,
	int topw1, int topw2,
	int sreg, s64 imm, int dreg)
{
	int i;
//	return(0);

	if(((u32)imm)==imm)
	{
		i=BGBCC_JX2_ComposeJumboRegImmRegF2(ctx,
			ropw1, ropw2, ropw3, ropw4,
			topw1, topw2, sreg, imm, dreg);
		return(i);
	}
	if((imm|(~((1LL<<32)-1)))==imm)
	{
		i=BGBCC_JX2_ComposeJumboRegImmRegF2(ctx,
			ropw1, ropw2, ropw3, ropw4,
			topw1, topw2|0x0100, sreg, imm, dreg);
		return(i);
	}
	return(0);
}

int BGBCC_JX2_ComposeJumboRegImm57sRegF2(BGBCC_JX2_Context *ctx,
	int *ropw1, int *ropw2, int *ropw3, int *ropw4, int *ropw5, int *ropw6,
	int topw1, int topw2,
	int sreg, s64 imm, int dreg)
{
	int opw1, opw2, opw3, opw4, opw5, opw6;
	int is57s;
	
	is57s=1;
	if(!(ctx->is_fixed32&2))
	{
		if(((imm>>56)!=0) && ((imm>>56)!=(-1)))
			is57s=0;
		if((sreg&32) || (dreg&32))
			is57s=0;
	}

	if(ctx->op_is_wex2&12)
		is57s=0;

	if(is57s)
	{
		opw1=0x4FE00|((imm>>48)&0x00FF);
		opw2=0x50000|((imm>>32)&0xFFFF);
		opw3=0x4FE00|((imm>>24)&0x00FF);
		opw4=0x50000|((imm>> 8)&0xFFFF);
		opw5=0x40000|topw1|((dreg&15)<<4)|(sreg&15);
		opw6=0x50000|topw2|(imm&0x00FF)|((imm>>63)&0x0100);

		if(dreg&0x10)
			opw6^=0x0400;
		if(sreg&0x10)
			opw6^=0x0200;

		if(ctx->is_fixed32&2)
		{
			if(dreg&0x20)
				opw5^=0x8000;
			if(sreg&0x20)
				opw5^=0x4000;
			opw1^=(((imm>>60)^(imm>>63))&7)<<13;
			opw3^=(((imm>>57)^(imm>>63))&7)<<13;
			opw5^=(((imm>>56)^(imm>>63))&1)<<13;
		}

		*ropw1=opw1;
		*ropw2=opw2;
		*ropw3=opw3;
		*ropw4=opw4;
		*ropw5=opw5;
		*ropw6=opw6;
		return(1);
	}

	return(0);
}

int BGBCC_JX2_ComposeJumboCheckOpwIsStore(BGBCC_JX2_Context *ctx,
	int opw1, int opw2)
{
	if((opw1&0xEB00)==0xE000)
	{
		if((opw2&0xF808)==0x0000)
			return(1);
//		if((opw2&0xF008)==0x4000)
		if((opw2&0xF808)==0x4000)
		{
			if((opw2&0xF80F)==0x4800)
				return(0);

			return(1);
		}

		if((opw2&0xF008)==0x8000)
			return(1);
	}

	if((opw1&0xEB00)==0xE100)
	{
		if((opw2&0xC800)==0x0000)
			return(1);
		if((opw2&0xE000)==0x4000)
			return(1);
	}
	return(0);
}

int BGBCC_JX2_ComposeJumboRegImmRegF0(BGBCC_JX2_Context *ctx,
	int *ropw1, int *ropw2, int *ropw3, int *ropw4,
	int topw1, int topw2,
	int sreg, s64 imm, int dreg)
{
	s64 imm1;
	int topw1n;
	int opw1, opw2, opw3, opw4;

	if(BGBCC_JX2_ComposeJumboCheckOpwIsStore(ctx, topw1, topw2))
		{ opw1=sreg; sreg=dreg; dreg=opw1; }

//	if(	BGBCC_JX2_EmitCheckRegExt5(ctx, dreg) ||
//		BGBCC_JX2_EmitCheckRegExt5(ctx, sreg))

	if(	(BGBCC_JX2_EmitCheckRegExt5(ctx, dreg) ||
		 BGBCC_JX2_EmitCheckRegExt5(ctx, sreg)) &&
		 (!(ctx->is_fixed32&2) || (ctx->op_is_wex2&12)))
	{
#if 1
		if(!(ctx->op_is_wex2&12) && (topw1==0xF000) &&
			!(ctx->is_fixed32&2) && !(ctx->emit_riscv&0x22))
		{
			topw1n=0x7000;
		
			opw1=0x4FE00|((imm>>20)&0x00FF);
			opw2=0x50000|((imm>>4)&0xFFFF);
			opw3=0x40000|topw1n|((dreg&15)<<4)|((sreg&15)<<0);
			opw4=0x50000|topw2 |((dreg&16)<<6)|((sreg&16)<<5)|
				((imm<<4)&0x00F0)|((imm>>28)&0x0100);

			if(BGBCC_JX2_EmitCheckRegExt5(ctx, dreg))
				opw3|=0x0400;
			if(BGBCC_JX2_EmitCheckRegExt5(ctx, sreg))
				opw3|=0x0200;

			imm1=(((u32)(opw1&0x00FF))<<20)|
				((opw2&0xFFFF)<<4)|
				((opw4&0x00F0)>>4);
			if(opw4&0x0100)
				imm1|=0xFFFFFFFFF0000000LL;
			if(imm!=imm1)
			{
		//		BGBCC_DBGBREAK
				return(0);
			}

			*ropw1=opw1;
			*ropw2=opw2;
			*ropw3=opw3;
			*ropw4=opw4;

			return(1);
		}
#endif

#if 1
		opw1=0x4FF00;
		opw2=0x50000|((imm>>4)&0x07FF);
		opw3=0x40000|topw1|((dreg&15)<<4)|((sreg&15)<<0);
		opw4=0x50000|topw2|((dreg&16)<<6)|((sreg&16)<<5)|((imm&0x000F)<<4);

		if(BGBCC_JX2_EmitCheckRegExt5(ctx, dreg))
			opw1|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt5(ctx, sreg))
			opw1|=0x0020;

		if(imm>>63)
			opw1|=0x0010;

		if((imm>>15)&1)
			opw4|=0x0010;

		imm1=((opw2&0x07FF)<<4)|((opw4&0x00F0)>>4);
		if(opw4&0x0100)
			imm1|=0x0000000000008000LL;
//		if(opw1&0x0100)
		if(opw1&0x0010)
			imm1|=0xFFFFFFFFFFFF0000LL;

		if(imm!=imm1)
		{
//			BGBCC_DBGBREAK
			return(0);
		}

		*ropw1=opw1;
		*ropw2=opw2;
		*ropw3=opw3;
		*ropw4=opw4;

		return(1);
#endif

		return(0);
	}

	opw1=0x4FE00|((imm>>20)&0x00FF);
	opw2=0x50000|((imm>>4)&0xFFFF);
	opw3=0x40000|topw1|((dreg&15)<<4)|((sreg&15)<<0);
	opw4=0x50000|topw2|((dreg&16)<<6)|((sreg&16)<<5)|
		((imm<<4)&0x00F0)|((imm>>28)&0x0100);

	if(BGBCC_JX2_EmitCheckRegExt5(ctx, dreg))
		opw3&=~0x8000;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, sreg))
		opw3&=~0x4000;

	imm1=(((u32)(opw1&0x00FF))<<20)|
		((opw2&0xFFFF)<<4)|
		((opw4&0x00F0)>>4);
	if(opw4&0x0100)
		imm1|=0xFFFFFFFFF0000000LL;
	if(imm!=imm1)
	{
//		BGBCC_DBGBREAK
		return(0);
	}

	*ropw1=opw1;
	*ropw2=opw2;
	*ropw3=opw3;
	*ropw4=opw4;

	return(1);
}

int BGBCC_JX2_ComposeJumboRegImmRegRegF0(BGBCC_JX2_Context *ctx,
	int *ropw1, int *ropw2, int *ropw3, int *ropw4,
	int topw1, int topw2,
	int sreg, s64 imm, int preg, int dreg)
{
	s64 imm1;
	int opw1, opw2, opw3, opw4;

	if(BGBCC_JX2_ComposeJumboCheckOpwIsStore(ctx, topw1, topw2))
		{ opw1=sreg; sreg=dreg; dreg=opw1; }

	opw1=0x4FF00;
	opw2=0x50800|(imm&0x07FF);
	opw3=0x40000|topw1|((dreg&15)<<4)|((sreg&15)<<0);
	opw4=0x50000|topw2|((dreg&16)<<6)|((sreg&16)<<5)|((preg&31)<<4);

	if(BGBCC_JX2_EmitCheckRegExt5(ctx, dreg))
		opw1|=0x0040;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, sreg))
		opw1|=0x0020;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, preg))
		opw1|=0x0010;

	imm1=(opw2&0x07FF);
	if(opw2&0x0400)
		imm1|=0xFFFFFFFFFFFFF800LL;

	if(imm!=imm1)
		return(0);

	*ropw1=opw1;
	*ropw2=opw2;
	*ropw3=opw3;
	*ropw4=opw4;

	return(1);
}

int BGBCC_JX2_ComposeJumboRegRegImmRegF0(BGBCC_JX2_Context *ctx,
	int *ropw1, int *ropw2, int *ropw3, int *ropw4,
	int topw1, int topw2,
	int sreg, int treg, s64 imm, int dreg)
{
	return(BGBCC_JX2_ComposeJumboRegImmRegRegF0(ctx,
		ropw1, ropw2, ropw3, ropw4,
		topw1, topw2,
		sreg, imm, treg, dreg));
}

int BGBCC_JX2_ComposeJumboRegRegRegRegF0(BGBCC_JX2_Context *ctx,
	int *ropw1, int *ropw2, int *ropw3, int *ropw4,
	int topw1, int topw2,
	int sreg, int treg, int preg, int dreg)
{
	s64 imm1;
	int opw1, opw2, opw3, opw4;

	if(BGBCC_JX2_ComposeJumboCheckOpwIsStore(ctx, topw1, topw2))
		{ opw1=sreg; sreg=dreg; dreg=opw1; }

	opw1=0x4FF00;
	opw2=0x50800|(preg&63);
	opw3=0x40000|topw1|((dreg&15)<<4)|((sreg&15)<<0);
	opw4=0x50000|topw2|((dreg&16)<<6)|((sreg&16)<<5)|((treg&31)<<4);

	if(BGBCC_JX2_EmitCheckRegExt5(ctx, dreg))
		opw1|=0x0040;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, sreg))
		opw1|=0x0020;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, treg))
		opw1|=0x0010;

	*ropw1=opw1;
	*ropw2=opw2;
	*ropw3=opw3;
	*ropw4=opw4;

	return(1);
}


int BGBCC_JX2_ComposeOp64RegImm17sRegF0(BGBCC_JX2_Context *ctx,
	int *ropw1, int *ropw2, int *ropw3, int *ropw4,
	int topw1, int topw2,	int topw3, int topw4,
	int sreg, s64 imm, int dreg)
{
	s64 imm1;
	int opw1, opw2, opw3, opw4;

	if(BGBCC_JX2_ComposeJumboCheckOpwIsStore(ctx, topw1, topw2))
		{ opw1=sreg; sreg=dreg; dreg=opw1; }

	opw1=0x4FF00|topw1;
	opw2=0x50000|topw2|((imm>>4)&0x07FF);
	opw3=0x40000|topw3|((dreg&15)<<4)|((sreg&15)<<0);
	opw4=0x50000|topw4|((dreg&16)<<6)|((sreg&16)<<5)|((imm&0x000F)<<4);

	if((imm>>15)&1)
		opw4|=0x0100;

	if(BGBCC_JX2_EmitCheckRegExt5(ctx, dreg))
		opw1|=0x0040;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, sreg))
		opw1|=0x0020;
	if(imm>>63)
		opw1|=0x0010;

	imm1=((opw2&0x07FF)<<4)|((opw4&0x00F0)>>4);
	if(opw4&0x0100)
		imm1|=0x0000000000008000LL;

	if(opw1&0x0010)
		imm1|=0xFFFFFFFFFFFF0000LL;

	if(imm!=imm1)
	{
//			BGBCC_DBGBREAK
		return(0);
	}

	*ropw1=opw1;
	*ropw2=opw2;
	*ropw3=opw3;
	*ropw4=opw4;

	return(1);
}
