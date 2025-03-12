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

/*
Native Quadword Ops
*/


#define BGBCC_JX2CC_MULSH2XLUMAX	8192

int BGBCC_JX2C_GetMulSh2xForImm(
	int val)
{
//	static u16 lutab[4096];
	static u16 *lutab;
	static int lu_init=0;
	int mul2x_sh, mul3x_sh, mul4x_sh;
	int i0, i1, i2, i3;
	int i, j, k;
	
	if(!lu_init)
	{
		lu_init=1;
		
		lutab=malloc(BGBCC_JX2CC_MULSH2XLUMAX*sizeof(u16));
//		for(i=0; i<4096; i++)
		for(i=0; i<BGBCC_JX2CC_MULSH2XLUMAX; i++)
			lutab[i]=0;
		
		for(i0=1; i0<14; i0++)
			for(i1=0; i1<i0; i1++)
		{
			j=(1<<i0)+(1<<i1);
			if(j>=BGBCC_JX2CC_MULSH2XLUMAX)
				continue;
			if(lutab[j]>0)
				continue;
			lutab[j]=(i0<<4)|i1;
		}

		for(i0=1; i0<14; i0++)
			for(i1=0; i1<i0; i1++)
				for(i2=0; i2<i1; i2++)
		{
			j=(1<<i0)+(1<<i1)+(1<<i2);
			if(j>=BGBCC_JX2CC_MULSH2XLUMAX)
				continue;
			if(lutab[j]>0)
				continue;
			lutab[j]=(i0<<8)|(i1<<4)|i2;
		}

		for(i0=1; i0<14; i0++)
		{
			if((1<<i0)>=BGBCC_JX2CC_MULSH2XLUMAX)
				break;

			for(i1=0; i1<i0; i1++)
				for(i2=0; i2<i1; i2++)
					for(i3=0; i3<i2; i3++)
			{
				j=(1<<i0)+(1<<i1)+(1<<i2)+(1<<i3);
				if(j>=BGBCC_JX2CC_MULSH2XLUMAX)
					continue;
				if(lutab[j]>0)
					continue;
				lutab[j]=(i0<<12)|(i1<<8)|(i2<<4)|i3;
			}
		}

#if 0
		for(i=0; i<32; i++)
		{
			printf("%d: ", i*8);
			for(j=0; j<8; j++)
			{
				printf("%04X ", lutab[i*8+j]);
			}
			printf("\n");
		}
#endif
	}
	
	if((val>=3) && (val<BGBCC_JX2CC_MULSH2XLUMAX))
	{
		j=lutab[val];
		if(j>0)
		{
//			printf("BGBCC_JX2C_GetMulSh2xForImm: Hit %d %04X\n", val, j);
			return(j);
		}
//		printf("BGBCC_JX2C_GetMulSh2xForImm: Missed %d\n", val);
		return(-1);
	}
	return(-1);

#if 0
	mul2x_sh=-1;
	mul3x_sh=-1;
	mul4x_sh=-1;

	switch(val)
	{
	case 1:		break;	//identity
	case 2:		break;	//pow2
	case 3:		mul2x_sh=0x0010; break;
	case 4:		break;	//pow2
	case 5:		mul2x_sh=0x0020; break;
	case 6:		mul2x_sh=0x0021; break;
	case 7:		mul3x_sh=0x0210; break;
	case 8:		break;	//pow2
	case 9:		mul2x_sh=0x0030; break;
	case 10:	mul2x_sh=0x0031; break;
	case 11:	mul3x_sh=0x0310; break;
	case 12:	mul2x_sh=0x0032; break;
	case 13:	mul3x_sh=0x0320; break;
	case 14:	mul3x_sh=0x0321; break;
	case 15:	mul4x_sh=0x3210; break;

	case 16:	break;	//pow2
	case 17:	mul2x_sh=0x0040; break;
	case 18:	mul2x_sh=0x0041; break;
	case 19:	mul3x_sh=0x0410; break;
	case 20:	mul2x_sh=0x0042; break;
	case 21:	mul3x_sh=0x0420; break;
	case 22:	mul3x_sh=0x0421; break;
	case 23:	mul4x_sh=0x4210; break;
	case 24:	mul2x_sh=0x0043; break;
	case 25:	mul3x_sh=0x0430; break;
	case 26:	mul3x_sh=0x0431; break;
	case 27:	mul3x_sh=0x4310; break;
	case 28:	mul3x_sh=0x0432; break;
	case 29:	mul4x_sh=0x4320; break;
	case 30:	mul4x_sh=0x4321; break;
	case 31:	break;	//5-bits
	case 32:	break;	//pow2
	case 33:	mul2x_sh=0x0050; break;
	case 34:	mul2x_sh=0x0051; break;
	case 35:	mul3x_sh=0x0510; break;
	case 36:	mul2x_sh=0x0052; break;
	case 37:	mul3x_sh=0x0520; break;
	case 38:	mul3x_sh=0x0521; break;
	case 39:	mul4x_sh=0x5210; break;
	case 40:	mul2x_sh=0x0053; break;
	case 41:	mul3x_sh=0x0530; break;
	case 42:	mul3x_sh=0x0531; break;
	case 43:	mul4x_sh=0x5310; break;
	case 44:	mul3x_sh=0x0532; break;
	case 45:	mul4x_sh=0x5320; break;
	case 46:	mul4x_sh=0x5321; break;
	case 47:	break;	//5-bits
	case 48:	mul2x_sh=0x0054; break;

	case 65:	mul2x_sh=0x60; break;
	case 66:	mul2x_sh=0x61; break;
	case 68:	mul2x_sh=0x62; break;
	case 72:	mul2x_sh=0x63; break;
	case 80:	mul2x_sh=0x64; break;
	case 96:	mul2x_sh=0x65; break;

	case 129:	mul2x_sh=0x70; break;
	case 130:	mul2x_sh=0x71; break;
	case 132:	mul2x_sh=0x72; break;
	case 136:	mul2x_sh=0x73; break;
	case 144:	mul2x_sh=0x74; break;
	case 160:	mul2x_sh=0x75; break;
	case 192:	mul2x_sh=0x76; break;

	case 257:	mul2x_sh=0x80; break;
	case 258:	mul2x_sh=0x81; break;
	case 260:	mul2x_sh=0x82; break;
	case 264:	mul2x_sh=0x83; break;
	case 272:	mul2x_sh=0x84; break;
	case 288:	mul2x_sh=0x85; break;
	case 320:	mul2x_sh=0x86; break;
	case 384:	mul2x_sh=0x87; break;
	}
	
	if(mul2x_sh>0)
		return(mul2x_sh);
	if(mul3x_sh>0)
		return(mul3x_sh);
	if(mul4x_sh>0)
		return(mul4x_sh);
	return(-1);
#endif
}

#if 0
int BGBCC_JX2C_GetMulSh3xForImm(
	int val)
{
	int mul3x_sh;

	mul3x_sh=-1;
	switch(val)
	{
	case 7:		mul3x_sh=0x210; break;

	case 11:	mul3x_sh=0x310; break;
	case 13:	mul3x_sh=0x320; break;
	case 14:	mul3x_sh=0x321; break;

	case 19:	mul3x_sh=0x410; break;
	case 21:	mul3x_sh=0x420; break;
	case 22:	mul3x_sh=0x421; break;
	case 25:	mul3x_sh=0x430; break;
	case 26:	mul3x_sh=0x431; break;
	case 28:	mul3x_sh=0x432; break;
	}
	
	return(mul3x_sh);
}
#endif

int BGBCC_JX2C_EmitBinaryVRegVRegQLong(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register treg)
{
	s64 li, lj;
	int csreg, ctreg, cdreg;
	int tr0, tr1, tr2, tr3;
	int nm1, nm2, mul2x_sh, mul3x_sh;
	int i, j, k, shl;

#if 1
//	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
	if(BGBCC_CCXL_IsRegImmSmallLongP(ctx, treg))
	{
//		j=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
		lj=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);
		shl=-1;
		j=lj;

		if(!(lj&(lj-1)))
		{
			for(i=0; i<31; i++)
				if(lj==(1LL<<i))
					{ shl=i; break; }
		}

		mul2x_sh=-1;
		mul3x_sh=-1;
		if(opr==CCXL_BINOP_MUL)
		{
			if(j==lj)
				mul2x_sh=BGBCC_JX2C_GetMulSh2xForImm(j);
//			mul3x_sh=BGBCC_JX2C_GetMulSh3xForImm(j);
		}

#if 1
		if(mul2x_sh>0)
		{
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			csreg=cdreg;

			if((mul2x_sh&15)!=0)
			{
				tr0=BGBCC_JX2C_ScratchAllocTsReg(ctx, sctx,
					BGBCC_SH_REGCLS_QGR);
			}else
			{
				tr0=csreg;
			}
			tr1=BGBCC_JX2C_ScratchAllocTsReg(ctx, sctx, BGBCC_SH_REGCLS_QGR);
			tr2=-1;
			tr3=-1;
			if(((mul2x_sh>>8)&15)!=0)
			{
				tr2=BGBCC_JX2C_ScratchAllocTsReg(ctx, sctx,
					BGBCC_SH_REGCLS_QGR);
			}
			if(((mul2x_sh>>12)&15)!=0)
			{
				tr3=BGBCC_JX2C_ScratchAllocTsReg(ctx, sctx,
					BGBCC_SH_REGCLS_QGR);
			}

//			i=-1;
//			if((k==((k<<23)>>23)) || sctx->has_jumbo)

			if(csreg!=tr0)
			{
				BGBCC_JX2_EmitOpRegImmReg(sctx,
					BGBCC_SH_NMID_SHADQ, csreg, ((mul2x_sh>>0)&15), tr0);
			}
			BGBCC_JX2_EmitOpRegImmReg(sctx,
				BGBCC_SH_NMID_SHADQ, csreg, ((mul2x_sh>>4)&15), tr1);

			if(tr2>=0)
			{
				BGBCC_JX2_EmitOpRegImmReg(sctx,
					BGBCC_SH_NMID_SHADQ, csreg, ((mul2x_sh>>8)&15), tr2);
			}
			if(tr3>=0)
			{
				BGBCC_JX2_EmitOpRegImmReg(sctx,
					BGBCC_SH_NMID_SHADQ, csreg, ((mul2x_sh>>12)&15), tr3);
			}

			if(tr3>=0)
			{
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					BGBCC_SH_NMID_ADD, tr0, tr1, tr1);
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					BGBCC_SH_NMID_ADD, tr2, tr3, tr3);
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					BGBCC_SH_NMID_ADD, tr1, tr3, cdreg);
			}else
				if(tr2>=0)
			{
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					BGBCC_SH_NMID_ADD, tr0, tr1, cdreg);
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					BGBCC_SH_NMID_ADD, cdreg, tr2, cdreg);
			}else
			{
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					BGBCC_SH_NMID_ADD, tr0, tr1, cdreg);
			}

			if(csreg!=tr0)
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			if(tr2>=0)
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr2);
			if(tr3>=0)
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr3);

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}
#endif

#if 0
		if(mul3x_sh>0)
		{
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			csreg=cdreg;

			tr0=BGBCC_JX2C_ScratchAllocTsReg(ctx, sctx, BGBCC_SH_REGCLS_QGR);
			tr1=BGBCC_JX2C_ScratchAllocTsReg(ctx, sctx, BGBCC_SH_REGCLS_QGR);
			tr2=BGBCC_JX2C_ScratchAllocTsReg(ctx, sctx, BGBCC_SH_REGCLS_QGR);

//			i=-1;
//			if((k==((k<<23)>>23)) || sctx->has_jumbo)

			BGBCC_JX2_EmitOpRegImmReg(sctx,
				BGBCC_SH_NMID_SHADQ, csreg, ((mul3x_sh>>0)&15), tr0);
			BGBCC_JX2_EmitOpRegImmReg(sctx,
				BGBCC_SH_NMID_SHADQ, csreg, ((mul3x_sh>>4)&15), tr1);
			BGBCC_JX2_EmitOpRegImmReg(sctx,
				BGBCC_SH_NMID_SHADQ, csreg, ((mul3x_sh>>8)&15), tr2);
			BGBCC_JX2_EmitOpRegRegReg(sctx,
				BGBCC_SH_NMID_ADD, tr0, tr1, cdreg);
			BGBCC_JX2_EmitOpRegRegReg(sctx,
				BGBCC_SH_NMID_ADD, cdreg, tr2, cdreg);

			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr2);

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}
#endif

		nm1=-1; k=0;
//		if((opr==CCXL_BINOP_ADD) && (j==(sbyte)j))
//			{ nm1=BGBCC_SH_NMID_ADD; k=j; }
//		if((opr==CCXL_BINOP_SUB) && ((-j)==(sbyte)(-j)))
//			{ nm1=BGBCC_SH_NMID_ADD; k=-j; }

		if((opr==CCXL_BINOP_ADD) && (lj==(s16)lj))
			{ nm1=BGBCC_SH_NMID_ADD; k=lj; }
		if((opr==CCXL_BINOP_SUB) && ((-lj)==(s16)(-lj)))
			{ nm1=BGBCC_SH_NMID_ADD; k=-lj; }

#if 1
		if((opr==CCXL_BINOP_SHL) && (lj==(lj&63)))
		{
			k=lj;
			nm1=BGBCC_SH_NMID_SHADQ;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_SHLDQ;
		}

		if((opr==CCXL_BINOP_SHR) && (lj==(lj&63)))
		{
			k=-lj;
			nm1=BGBCC_SH_NMID_SHADQ;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_SHLDQ;
		}

		if((opr==CCXL_BINOP_SHRR) && (lj==(lj&63)))
		{
			k=-lj;
			nm1=BGBCC_SH_NMID_SHLDQ;
		}
#endif

		if(BGBCC_JX2C_ProbeVRegRejectImm3P(ctx, sctx, dreg, dreg, treg, nm1))
			nm1=-1;

		if(nm1>=0)
		{
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_JX2C_EmitOpImmReg(ctx, sctx, nm1, k, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

#if 1
		if((opr==CCXL_BINOP_MUL) && (shl>=0))
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				{ nm1=BGBCC_SH_NMID_SHLDQ; }
			else
				{ nm1=BGBCC_SH_NMID_SHADQ; }
			
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_JX2C_EmitOpRegImmReg(ctx, sctx,
				nm1, cdreg, shl, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		if((opr==CCXL_BINOP_DIV) && (shl>=0))
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
				BGBCC_JX2C_EmitOpRegImmReg(ctx, sctx,
					BGBCC_SH_NMID_SHLDQ, cdreg, -shl, cdreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}

			if(BGBCC_CCXL_TypeSignedP(ctx, type) &&
				ctx->arch_has_predops)
			{
				cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);

				BGBCC_JX2C_EmitOpImmReg(ctx, sctx,
					BGBCC_SH_NMID_CMPGT, 0, cdreg);
				BGBCC_JX2C_EmitOpImmRegPredF(ctx, sctx,
					BGBCC_SH_NMID_ADD, j-1, cdreg);
				BGBCC_JX2C_EmitOpRegImmReg(ctx, sctx,
					BGBCC_SH_NMID_SHADQ, cdreg, -shl, cdreg);

				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}
		}
		
		if((opr==CCXL_BINOP_MOD) && (shl>=0))
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
				BGBCC_JX2C_EmitOpImmReg(ctx, sctx,
					BGBCC_SH_NMID_AND, j-1, cdreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}
		}
#endif
	}
#endif

	nm1=-1;
	if(opr==CCXL_BINOP_ADD)
		nm1=BGBCC_SH_NMID_ADD;
	if(opr==CCXL_BINOP_SUB)
		nm1=BGBCC_SH_NMID_SUB;
//	if(opr==CCXL_BINOP_MUL)
//		nm1=BGBCC_SH_NMID_MULL;

	if(opr==CCXL_BINOP_AND)
		nm1=BGBCC_SH_NMID_AND;
	if(opr==CCXL_BINOP_OR)
		nm1=BGBCC_SH_NMID_OR;
	if(opr==CCXL_BINOP_XOR)
		nm1=BGBCC_SH_NMID_XOR;

	if(sctx->has_qmul&1)
	{
		if(opr==CCXL_BINOP_MUL)
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_MULUQ;
			else
				nm1=BGBCC_SH_NMID_MULSQ;
		}

		if(opr==CCXL_BINOP_DIV)
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_DIVUQ;
			else
				nm1=BGBCC_SH_NMID_DIVSQ;
		}

		if(opr==CCXL_BINOP_MOD)
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_MODUQ;
			else
			{
//				nm1=BGBCC_SH_NMID_MODSQ;
			}
		}
	}


	if(nm1>=0)
	{
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, cdreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
	{
		if(opr==CCXL_BINOP_SHL)
			nm1=BGBCC_SH_NMID_SHLDQ;
		if(opr==CCXL_BINOP_SHR)
			nm1=BGBCC_SH_NMID_SHLRQ;
		if(opr==CCXL_BINOP_SHRR)
			nm1=BGBCC_SH_NMID_SHLRQ;
	}else
	{
		if(opr==CCXL_BINOP_SHL)
			nm1=BGBCC_SH_NMID_SHADQ;
		if(opr==CCXL_BINOP_SHR)
			nm1=BGBCC_SH_NMID_SHARQ;
		if(opr==CCXL_BINOP_SHRR)
			nm1=BGBCC_SH_NMID_SHLRQ;
	}

//	if(nm1>=0)
//	if((nm1>=0) && sctx->has_bjx1ari)
	if((nm1>=0) && sctx->has_bjx1ari && sctx->has_bjx1mov)
	{
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
		BGBCC_JX2_EmitOpRegRegReg(sctx, nm1, cdreg, ctreg, cdreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	if((nm1>=0) && sctx->has_bjx1ari)
	{
//		BGBCC_DBGBREAK
	}

	if(opr==CCXL_BINOP_MUL)
	{
//		if(sctx->is_betav && !sctx->has_bjx1ari)
//		if(sctx->is_betav && (!sctx->has_bjx1ari || !sctx->has_bjx1mov))
		if(1)
		{
			/* B64V: Use function call for 'long' multiply. */

			BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);
		
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_ARG(0));
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_ARG(1));

			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_JX2CC_PSREG_ARG(1));
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_JX2CC_PSREG_ARG(0));

			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				{ BGBCC_JX2C_EmitCallName(ctx, sctx, "__umulsq"); }
			else
				{ BGBCC_JX2C_EmitCallName(ctx, sctx, "__smulsq"); }

			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_ARG(0));
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_ARG(1));

//			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ0);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_RQRET);

//			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ0);
//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, 
				BGBCC_JX2CC_PSREG_RQRET);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_RQRET);
			return(1);
		}

#if 0
		ctreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitTryGetRegisterDirty(ctx, sctx, dreg);

		if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
			ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		if((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR))
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);

		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MULQ, ctreg, cdreg);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACL, cdreg);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
#endif
	}

	if(opr==CCXL_BINOP_DIV)
	{
		BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);
	
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_ARG(0));
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_ARG(1));

		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_JX2CC_PSREG_ARG(1));
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_JX2CC_PSREG_ARG(0));

		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{ BGBCC_JX2C_EmitCallName(ctx, sctx, "__udivsq"); }
		else
			{ BGBCC_JX2C_EmitCallName(ctx, sctx, "__sdivsq"); }

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_ARG(0));
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_ARG(1));

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ0);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_RQRET);

//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_JX2CC_PSREG_RQRET);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_RQRET);
		return(1);
	}

	if(opr==CCXL_BINOP_MOD)
	{
		BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_ARG(0));
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_ARG(1));

		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_JX2CC_PSREG_ARG(1));
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_JX2CC_PSREG_ARG(0));

		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{ BGBCC_JX2C_EmitCallName(ctx, sctx, "__umodsq"); }
		else
			{ BGBCC_JX2C_EmitCallName(ctx, sctx, "__smodsq"); }

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_ARG(0));
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_ARG(1));

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ0);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_RQRET);


//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_JX2CC_PSREG_RQRET);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_RQRET);
		return(1);
	}


	nm1=-1;

	if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
	{
		if(opr==CCXL_BINOP_SHL)
			nm1=BGBCC_SH_NMID_SHLDQ;
//		if(opr==CCXL_BINOP_SHR)
//			nm1=BGBCC_SH_NMID_SHLDQ;
//		if(opr==CCXL_BINOP_SHRR)
//			nm1=BGBCC_SH_NMID_SHLDQ;

		if(opr==CCXL_BINOP_SHR)
			nm1=BGBCC_SH_NMID_SHLRQ;
		if(opr==CCXL_BINOP_SHRR)
			nm1=BGBCC_SH_NMID_SHLRQ;
	}else
	{
		if(opr==CCXL_BINOP_SHL)
			nm1=BGBCC_SH_NMID_SHADQ;
//		if(opr==CCXL_BINOP_SHR)
//			nm1=BGBCC_SH_NMID_SHADQ;
//		if(opr==CCXL_BINOP_SHRR)
//			nm1=BGBCC_SH_NMID_SHLDQ;

		if(opr==CCXL_BINOP_SHR)
			nm1=BGBCC_SH_NMID_SHARQ;
		if(opr==CCXL_BINOP_SHRR)
			nm1=BGBCC_SH_NMID_SHARQ;
	}

	if(nm1>=0)
//	if((nm1>=0) && sctx->has_bjx1ari)
	{
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, cdreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}


	if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
	{
		if(opr==CCXL_BINOP_SHR)
			nm1=BGBCC_SH_NMID_SHLDQ;
		if(opr==CCXL_BINOP_SHRR)
			nm1=BGBCC_SH_NMID_SHLDQ;
	}else
	{
		if(opr==CCXL_BINOP_SHR)
			nm1=BGBCC_SH_NMID_SHADQ;
		if(opr==CCXL_BINOP_SHRR)
			nm1=BGBCC_SH_NMID_SHLDQ;
	}

	if(nm1>=0)
	{
//		tr0=BGBCC_SH_REG_RQ0;
		tr0=BGBCC_JX2CC_PSREG_RQRET;
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_NEG, ctreg, tr0);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, tr0, cdreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}


	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitBinaryVRegVRegVRegQLong(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	s64	li;
	int csreg, ctreg, cdreg;
	int tr0, tr1, tr2, tr3;
	int nm1, nm2;
	int i, j, k, shl, mul2x_sh, mul3x_sh;

	li=-1;
	j=-999;
	k=0;

//	if(type.val==0x0D)
	if((type.val==0x0D) &&
		((dreg.val&4095)==(sreg.val&4095)) &&
		((dreg.val>>56)==(sreg.val>>56)))
	{
		if(opr==CCXL_BINOP_SHL)
		{
			i=-1;
		}
	}

	if(	BGBCC_CCXL_IsRegImmLongP(ctx, treg) ||
		BGBCC_CCXL_IsRegImmUIntP(ctx, treg))
	{
		li=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);
		j=(int)li;
		
		if(j!=li)
		{
			k=-1;
		}
	}
	
//	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
//	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg) || (j==li))
	if(BGBCC_CCXL_IsRegImmSmallSIntP(ctx, treg) || (j==li))
	{
//		j=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
		j=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);

		if((j==0) && ((opr==CCXL_BINOP_ADD) || (opr==CCXL_BINOP_SUB) ||
			(opr==CCXL_BINOP_OR) || (opr==CCXL_BINOP_XOR) ||
			(opr==CCXL_BINOP_SHL) || (opr==CCXL_BINOP_SHR)))
		{
			BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
			return(1);
		}
		
		if(opr==CCXL_BINOP_SHR)
		{
			i=-1;
		}
		

		shl=-1;
		if(!(j&(j-1)))
		{
			for(i=0; i<31; i++)
				if(j==(1<<i))
					{ shl=i; break; }
		}
		
		mul2x_sh=-1;
		mul3x_sh=-1;
		if(opr==CCXL_BINOP_MUL)
		{
			if(j==li)
				mul2x_sh=BGBCC_JX2C_GetMulSh2xForImm(j);
//			mul3x_sh=BGBCC_JX2C_GetMulSh3xForImm(j);
		}

#if 1
		if(mul2x_sh>0)
		{
			if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
			{
				cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
				csreg=cdreg;
			}else
			{
				csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
				cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
			}

			if((mul2x_sh&15)!=0)
			{
				tr0=BGBCC_JX2C_ScratchAllocTsReg(ctx, sctx,
					BGBCC_SH_REGCLS_QGR);
			}else
			{
				tr0=csreg;
			}
			tr1=BGBCC_JX2C_ScratchAllocTsReg(ctx, sctx, BGBCC_SH_REGCLS_QGR);
			tr2=-1;
			tr3=-1;
			if(((mul2x_sh>>8)&15)!=0)
			{
				tr2=BGBCC_JX2C_ScratchAllocTsReg(ctx, sctx,
					BGBCC_SH_REGCLS_QGR);
			}
			if(((mul2x_sh>>12)&15)!=0)
			{
				tr3=BGBCC_JX2C_ScratchAllocTsReg(ctx, sctx,
					BGBCC_SH_REGCLS_QGR);
			}

//			i=-1;
//			if((k==((k<<23)>>23)) || sctx->has_jumbo)

			if(csreg!=tr0)
			{
				BGBCC_JX2_EmitOpRegImmReg(sctx,
					BGBCC_SH_NMID_SHADQ, csreg, ((mul2x_sh>>0)&15), tr0);
			}
			BGBCC_JX2_EmitOpRegImmReg(sctx,
				BGBCC_SH_NMID_SHADQ, csreg, ((mul2x_sh>>4)&15), tr1);

			if(tr2>=0)
			{
				BGBCC_JX2_EmitOpRegImmReg(sctx,
					BGBCC_SH_NMID_SHADQ, csreg, ((mul2x_sh>>8)&15), tr2);
			}
			if(tr3>=0)
			{
				BGBCC_JX2_EmitOpRegImmReg(sctx,
					BGBCC_SH_NMID_SHADQ, csreg, ((mul2x_sh>>12)&15), tr3);
			}

			if(tr3>=0)
			{
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					BGBCC_SH_NMID_ADD, tr0, tr1, tr1);
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					BGBCC_SH_NMID_ADD, tr2, tr3, tr3);
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					BGBCC_SH_NMID_ADD, tr1, tr3, cdreg);
			}else
				if(tr2>=0)
			{
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					BGBCC_SH_NMID_ADD, tr0, tr1, cdreg);
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					BGBCC_SH_NMID_ADD, cdreg, tr2, cdreg);
			}else
			{
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					BGBCC_SH_NMID_ADD, tr0, tr1, cdreg);
			}

			if(csreg!=tr0)
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			if(tr2>=0)
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr2);
			if(tr3>=0)
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr3);

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			if(cdreg!=csreg)
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}
#endif

#if 0
		if(mul3x_sh>0)
		{
			if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
			{
				cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
				csreg=cdreg;
			}else
			{
				csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
				cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
			}

			tr0=BGBCC_JX2C_ScratchAllocTsReg(ctx, sctx, BGBCC_SH_REGCLS_QGR);
			tr1=BGBCC_JX2C_ScratchAllocTsReg(ctx, sctx, BGBCC_SH_REGCLS_QGR);
			tr2=BGBCC_JX2C_ScratchAllocTsReg(ctx, sctx, BGBCC_SH_REGCLS_QGR);

//			i=-1;
//			if((k==((k<<23)>>23)) || sctx->has_jumbo)

			BGBCC_JX2_EmitOpRegImmReg(sctx,
				BGBCC_SH_NMID_SHADQ, csreg, ((mul3x_sh>>0)&15), tr0);
			BGBCC_JX2_EmitOpRegImmReg(sctx,
				BGBCC_SH_NMID_SHADQ, csreg, ((mul3x_sh>>4)&15), tr1);
			BGBCC_JX2_EmitOpRegImmReg(sctx,
				BGBCC_SH_NMID_SHADQ, csreg, ((mul3x_sh>>8)&15), tr2);
			BGBCC_JX2_EmitOpRegRegReg(sctx,
				BGBCC_SH_NMID_ADD, tr0, tr1, cdreg);
			BGBCC_JX2_EmitOpRegRegReg(sctx,
				BGBCC_SH_NMID_ADD, cdreg, tr2, cdreg);

			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr2);

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			if(cdreg!=csreg)
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}
#endif

		nm1=-1; k=0;
		if((opr==CCXL_BINOP_ADD) && (j==(byte)j))
			{ nm1=BGBCC_SH_NMID_ADD; k=j; }
		if((opr==CCXL_BINOP_SUB) && (j==(byte)j))
			{ nm1=BGBCC_SH_NMID_SUB; k=j; }
//		if((opr==CCXL_BINOP_MUL) && (j==(sbyte)j))
//			{ nm1=BGBCC_SH_NMID_MULQ; k=j; }

		if((opr==CCXL_BINOP_ADD) && ((-j)==(byte)(-j)))
			{ nm1=BGBCC_SH_NMID_SUB; k=-j; }
		if((opr==CCXL_BINOP_SUB) && ((-j)==(byte)(-j)))
			{ nm1=BGBCC_SH_NMID_ADD; k=-j; }

		if((opr==CCXL_BINOP_AND) && (j==(byte)j))
			{ nm1=BGBCC_SH_NMID_AND; k=j; }
		if((opr==CCXL_BINOP_OR) && (j==(byte)j))
			{ nm1=BGBCC_SH_NMID_OR; k=j; }
		if((opr==CCXL_BINOP_XOR) && (j==(byte)j))
			{ nm1=BGBCC_SH_NMID_XOR; k=j; }

#if 0
		if((opr==CCXL_BINOP_SHL) && (j==(j&63)))
		{
			k=j;
			nm1=BGBCC_SH_NMID_SHADQ;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_SHLDQ;
		}

		if((opr==CCXL_BINOP_SHR) && (j==(j&63)))
		{
			k=-j;
			nm1=BGBCC_SH_NMID_SHADQ;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_SHLDQ;
		}
#endif

//		if((nm1<0) && (j==((j<<12)>>12)))
//		if((nm1<0) && (j==((j<<16)>>16)))
//		if(nm1<0)
		if(sctx->has_jumbo && (nm1<0))
		{
			if(opr==CCXL_BINOP_ADD)
				{ nm1=BGBCC_SH_NMID_ADD; k=j; }
			if(opr==CCXL_BINOP_SUB)
				{ nm1=BGBCC_SH_NMID_SUB; k=j; }
//			if(opr==CCXL_BINOP_MUL)
//				{ nm1=BGBCC_SH_NMID_MULL; k=j; }

			if(opr==CCXL_BINOP_AND)
				{ nm1=BGBCC_SH_NMID_AND; k=j; }
			if(opr==CCXL_BINOP_OR)
				{ nm1=BGBCC_SH_NMID_OR; k=j; }
			if(opr==CCXL_BINOP_XOR)
				{ nm1=BGBCC_SH_NMID_XOR; k=j; }
		}

#if 1
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			if((opr==CCXL_BINOP_SHL) && (j==(j&63)))
				{ nm1=BGBCC_SH_NMID_SHLDQ; k=j; }
			if((opr==CCXL_BINOP_SHR) && (j==(j&63)))
				{ nm1=BGBCC_SH_NMID_SHLDQ; k=-j; }
			if((opr==CCXL_BINOP_SHRR) && (j==(j&63)))
				{ nm1=BGBCC_SH_NMID_SHLDQ; k=-j; }

			if(opr==CCXL_BINOP_MUL && (shl>=0))
				{ nm1=BGBCC_SH_NMID_SHLDQ; k=shl; }
			if(opr==CCXL_BINOP_DIV && (shl>=0))
				{ nm1=BGBCC_SH_NMID_SHLDQ; k=-shl; }

			if(opr==CCXL_BINOP_MOD && (shl>0) && ((j-1)==(byte)(j-1)))
				{ nm1=BGBCC_SH_NMID_AND; k=j-1; }
		}else
		{
			if((opr==CCXL_BINOP_SHL) && (j==(j&63)))
				{ nm1=BGBCC_SH_NMID_SHADQ; k=j; }
			if((opr==CCXL_BINOP_SHR) && (j==(j&63)))
				{ nm1=BGBCC_SH_NMID_SHADQ; k=-j; }
			if((opr==CCXL_BINOP_SHRR) && (j==(j&63)))
				{ nm1=BGBCC_SH_NMID_SHLDQ; k=-j; }

			if(opr==CCXL_BINOP_MUL && (shl>=0))
				{ nm1=BGBCC_SH_NMID_SHADQ; k=shl; }
//			if(opr==CCXL_BINOP_DIV && (shl>=0))
//				{ nm1=BGBCC_SH_NMID_SHADQ; k=-shl; }

//			if(opr==CCXL_BINOP_MOD && (shl>0) && ((j-1)==(byte)(j-1)))
//				{ nm1=BGBCC_SH_NMID_AND; k=j-1; }
		}
#endif

		if(sctx->has_jumbo && (nm1<0))
		{
			if(opr==CCXL_BINOP_ADD)
				{ nm1=BGBCC_SH_NMID_ADD; k=j; }
			if(opr==CCXL_BINOP_SUB)
				{ nm1=BGBCC_SH_NMID_SUB; k=j; }
//			if(opr==CCXL_BINOP_MUL)
//				{ nm1=BGBCC_SH_NMID_MULL; k=j; }

			if(opr==CCXL_BINOP_AND)
				{ nm1=BGBCC_SH_NMID_AND; k=j; }
			if(opr==CCXL_BINOP_OR)
				{ nm1=BGBCC_SH_NMID_OR; k=j; }
			if(opr==CCXL_BINOP_XOR)
				{ nm1=BGBCC_SH_NMID_XOR; k=j; }
		}

#if 1
		if(sctx->has_qmul&1)
		{
			if(opr==CCXL_BINOP_MUL)
			{
				k=j;
				if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
					nm1=BGBCC_SH_NMID_MULUQ;
				else
					nm1=BGBCC_SH_NMID_MULSQ;
			}

			if(opr==CCXL_BINOP_DIV)
			{
				k=j;
				if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
					nm1=BGBCC_SH_NMID_DIVUQ;
				else
					nm1=BGBCC_SH_NMID_DIVSQ;
			}

			if(opr==CCXL_BINOP_MOD)
			{
				k=j;
				if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
					nm1=BGBCC_SH_NMID_MODUQ;
				else
				{
	//				nm1=BGBCC_SH_NMID_MODSQ;
				}
			}
		}
#endif

		if(BGBCC_JX2C_ProbeVRegRejectImm3P(ctx, sctx, dreg, sreg, treg, nm1))
			nm1=-1;

//		if(nm1>=0)
//		if((nm1>=0) && sctx->has_bjx1ari)
		if((nm1>=0) && sctx->has_bjx1ari && sctx->has_bjx1mov)
		{
			if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
			{
				cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
				csreg=cdreg;
				BGBCC_JX2_EmitOpRegImmReg(sctx, nm1, csreg, k, cdreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}

			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

//			i=-1;
//			if((k==((k<<23)>>23)) || sctx->has_jumbo)

			i=BGBCC_JX2_TryEmitOpRegImmReg(sctx, nm1, csreg, k, cdreg);
			if(i>0)
			{
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}

			BGBCC_DBGBREAK

#if 0
			if((nm1==BGBCC_SH_NMID_SHADQ) || (nm1==BGBCC_SH_NMID_SHLDQ))
				BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);

//			ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_QGR);
			ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);

			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, ctreg, k);
			BGBCC_JX2_EmitOpRegRegReg(sctx, nm1, csreg, ctreg, cdreg);

//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
#endif
		}

		if(!BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
			BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
		i=BGBCC_JX2C_EmitBinaryVRegVRegQLong(ctx, sctx, type, dreg, opr, treg);
		return(i);
	}

	if(type.val==0x0D)
	{
		if(opr==CCXL_BINOP_SHL)
		{
			i=-1;
		}
	}

	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
	{
		if(	(opr==CCXL_BINOP_SHL) ||
			(opr==CCXL_BINOP_SHR) )
		{
			i=-1;
		}

		i=BGBCC_JX2C_EmitBinaryVRegVRegQLong(ctx, sctx,
			type, dreg, opr, treg);
		return(i);
	}

	if(	BGBCC_CCXL_IsRegImmIntP(ctx, sreg) ||
		BGBCC_CCXL_IsRegImmLongP(ctx, sreg))
	{
		if((opr==CCXL_BINOP_ADD) ||
			(opr==CCXL_BINOP_MUL) ||
			(opr==CCXL_BINOP_AND) ||
			(opr==CCXL_BINOP_OR) ||
			(opr==CCXL_BINOP_XOR))
		{
			i=BGBCC_JX2C_EmitBinaryVRegVRegVRegQLong(ctx, sctx, type,
				dreg, opr, treg, sreg);
			return(i);
		}
	}

	if(	BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg) &&
		!BGBCC_CCXL_RegisterIdentEqualP(ctx, sreg, treg))
	{
		if(	(opr==CCXL_BINOP_ADD) ||
			(opr==CCXL_BINOP_MUL) ||
			(opr==CCXL_BINOP_AND) ||
			(opr==CCXL_BINOP_OR)  ||
			(opr==CCXL_BINOP_XOR) )
		{
			i=BGBCC_JX2C_EmitBinaryVRegVRegVRegQLong(ctx, sctx, type,
				dreg, opr, treg, sreg);
			return(i);
		}
	}

	nm1=-1;
	if(opr==CCXL_BINOP_ADD)
		nm1=BGBCC_SH_NMID_ADD;
	if(opr==CCXL_BINOP_SUB)
		nm1=BGBCC_SH_NMID_SUB;
//	if(opr==CCXL_BINOP_MUL)
//		nm1=BGBCC_SH_NMID_MULQ;

	if(opr==CCXL_BINOP_AND)
		nm1=BGBCC_SH_NMID_AND;
	if(opr==CCXL_BINOP_OR)
		nm1=BGBCC_SH_NMID_OR;
	if(opr==CCXL_BINOP_XOR)
		nm1=BGBCC_SH_NMID_XOR;

	if(opr==CCXL_BINOP_SHL)
	{
		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			nm1=BGBCC_SH_NMID_SHLDQ;
		else
			nm1=BGBCC_SH_NMID_SHADQ;
	}
	if(opr==CCXL_BINOP_SHR)
	{
		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			nm1=BGBCC_SH_NMID_SHLRQ;
		else
			nm1=BGBCC_SH_NMID_SHARQ;
	}


	if(sctx->has_qmul&1)
	{
		if(opr==CCXL_BINOP_MUL)
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_MULUQ;
			else
				nm1=BGBCC_SH_NMID_MULSQ;
		}

		if(opr==CCXL_BINOP_DIV)
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_DIVUQ;
			else
				nm1=BGBCC_SH_NMID_DIVSQ;
		}

		if(opr==CCXL_BINOP_MOD)
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_MODUQ;
			else
			{
//				nm1=BGBCC_SH_NMID_MODSQ;
			}
		}
	}

//	if(	BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg) ||
//		BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
//	{
//		nm1=-1;
//	}

//	if(nm1>=0)
//	if((nm1>=0) && sctx->has_bjx1ari)
	if((nm1>=0) && sctx->has_bjx1ari && sctx->has_bjx1mov)
//	if(0)
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);

		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
		{
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			ctreg=cdreg;
		}else
		{
			ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
		}

//		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
		BGBCC_JX2_EmitOpRegRegReg(sctx, nm1, csreg, ctreg, cdreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		if(!BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
	{
		if(	(opr==CCXL_BINOP_ADD) ||
			(opr==CCXL_BINOP_MUL) ||
			(opr==CCXL_BINOP_AND) ||
			(opr==CCXL_BINOP_OR)  ||
			(opr==CCXL_BINOP_XOR) )
		{
			BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, type, dreg, treg);
			i=BGBCC_JX2C_EmitBinaryVRegVRegQLong(ctx, sctx,
				type, dreg, opr, sreg);
			return(i);
		}

		BGBCC_DBGBREAK
	}

	BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
	i=BGBCC_JX2C_EmitBinaryVRegVRegQLong(ctx, sctx, type, dreg, opr, treg);
	return(i);
}

int BGBCC_JX2C_EmitUnaryVRegVRegQLong(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register sreg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, nm3, nm4;
	s32 imm;
	int i, j, k;
	
	if(opr==CCXL_UNOP_MOV)
	{
		BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
		return(1);
	}

	nm1=-1;
	if(opr==CCXL_UNOP_NEG)
		nm1=BGBCC_SH_NMID_NEG;
	if(opr==CCXL_UNOP_NOT)
		nm1=BGBCC_SH_NMID_NOT;

	if(nm1>=0)
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
//			BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, cdreg, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}else
		{
			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
//			BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}
	}

	if(opr==CCXL_UNOP_LNOT)
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			if(sctx->has_qmul&32)
			{
				BGBCC_JX2C_EmitOpRegImmReg(ctx, sctx,
					BGBCC_SH_NMID_CMPQEQ, cdreg, 0, cdreg);
			}else
			{
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_TSTQ, cdreg, cdreg);
	//			ctreg=cdreg&31;
				ctreg=cdreg&63;
	//			BGBCC_JX2C_EmitOpReg(ctx, sctx, BGBCC_SH_NMID_MOVT, ctreg);
				BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVT, ctreg);
			}
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);

		if(sctx->has_qmul&32)
		{
			BGBCC_JX2C_EmitOpRegImmReg(ctx, sctx,
				BGBCC_SH_NMID_CMPQEQ, csreg, 0, cdreg);
		}else
		{
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_TSTQ, csreg, csreg);
	//		ctreg=cdreg&31;
			ctreg=cdreg&63;
	//		BGBCC_JX2C_EmitOpReg(ctx, sctx, BGBCC_SH_NMID_MOVT, ctreg);
			BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVT, ctreg);
		}
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	if(opr==CCXL_UNOP_LNOT2)
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_JX2C_EmitOpRegImmReg(ctx, sctx,
				BGBCC_SH_NMID_XOR, cdreg, 1, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
		BGBCC_JX2C_EmitOpRegImmReg(ctx, sctx,
			BGBCC_SH_NMID_XOR, csreg, 1, cdreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	if((opr==CCXL_UNOP_INC) ||
		(opr==CCXL_UNOP_DEC))
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			j=(opr==CCXL_UNOP_INC)?1:(-1);
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_JX2C_EmitOpImmReg(ctx, sctx, BGBCC_SH_NMID_ADD, j, cdreg);
//			BGBCC_JX2_EmitOpLdRegDispReg(sctx,
//				BGBCC_SH_NMID_LEAB, cdreg, j, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		j=(opr==CCXL_UNOP_INC)?1:(-1);
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

//		BGBCC_JX2_EmitOpLdRegDispReg(sctx,
//			BGBCC_SH_NMID_LEAB, csreg, j, cdreg);

#if 1
		i=BGBCC_JX2_TryEmitOpRegImmReg(sctx,
			BGBCC_SH_NMID_ADD, csreg, j, cdreg);
		if(i<=0)
//		if(1)
		{
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
			BGBCC_JX2C_EmitOpImmReg(ctx, sctx, BGBCC_SH_NMID_ADD, j, cdreg);
		}
#endif

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitCompareVRegVRegVRegQLong(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg, int cmp,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg, csreg2, ctreg2;
	s64 imm, shl;
	int flip, noflip, doptrshl;
	int nm1, nm2, nm3, cmp1;
	int i;

	BGBCC_JX2C_NormalizeImmVRegInt(ctx, sctx, type, &sreg);
	BGBCC_JX2C_NormalizeImmVRegInt(ctx, sctx, type, &treg);

#if 1
	if(	 (	BGBCC_CCXL_IsRegImmIntP(ctx, sreg) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, sreg) ) &&
		!(	BGBCC_CCXL_IsRegImmIntP(ctx, treg) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, treg)) )
	{
		switch(cmp)
		{
		case CCXL_CMP_EQ:	cmp1=CCXL_CMP_EQ;	break;
		case CCXL_CMP_NE:	cmp1=CCXL_CMP_NE;	break;
		case CCXL_CMP_LT:	cmp1=CCXL_CMP_GT;	break;
		case CCXL_CMP_GT:	cmp1=CCXL_CMP_LT;	break;
		case CCXL_CMP_LE:	cmp1=CCXL_CMP_GE;	break;
		case CCXL_CMP_GE:	cmp1=CCXL_CMP_LE;	break;
		default:			cmp1=-1;			break;
		}

		i=BGBCC_JX2C_EmitCompareVRegVRegVRegQLong(ctx, sctx,
			type, dreg, cmp1, treg, sreg);
		return(i);
	}
#endif

	shl=-1;
	imm=0;

	if(	BGBCC_CCXL_IsRegImmIntP(ctx, treg) ||
		BGBCC_CCXL_IsRegImmLongP(ctx, treg) )
	{
		imm=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);
		shl=imm;
		
		if((imm>=1024) || (imm<=-1024))
		{
			shl=-1;
			imm=0;
		}
	}

	noflip=0;
//	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
//	{
//		noflip=1;
//		if(sctx->is_betav && !sctx->has_bjx1ari)
//			noflip=0;
//	}
	
	doptrshl=0;
	if(sctx->abi_spillpad&4)
	{
		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, type))
		{
			doptrshl=1;
		}
	}
	
	nm3=-1; flip=0;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPQEQ;
		nm2=BGBCC_SH_NMID_BT;
//		nm3=BGBCC_SH_NMID_BREQ;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPQEQ;
		nm2=BGBCC_SH_NMID_BF;
//		nm3=BGBCC_SH_NMID_BRNE;
		break;
	case CCXL_CMP_LT:
		if(!noflip || doptrshl)
		{
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPQHI;
				nm2=BGBCC_SH_NMID_BT;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGT;
				nm2=BGBCC_SH_NMID_BT;
			}
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
//				flip=1;
//				nm1=BGBCC_SH_NMID_CMPQHI;
//				nm2=BGBCC_SH_NMID_BT;

				nm1=BGBCC_SH_NMID_CMPQHS;
				nm2=BGBCC_SH_NMID_BF;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGE;
				nm2=BGBCC_SH_NMID_BF;
//				nm3=BGBCC_SH_NMID_BRLT;
			}
		}
		break;
	case CCXL_CMP_GT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPQHI;
			nm2=BGBCC_SH_NMID_BT;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPQGT;
			nm2=BGBCC_SH_NMID_BT;
//			nm3=BGBCC_SH_NMID_BRGT;
		}
		break;
	case CCXL_CMP_LE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPQHI;
			nm2=BGBCC_SH_NMID_BF;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPQGT;
			nm2=BGBCC_SH_NMID_BF;
//			nm3=BGBCC_SH_NMID_BRLE;
		}
		break;
	case CCXL_CMP_GE:
		if(!noflip || doptrshl)
		{
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPQHI;
				nm2=BGBCC_SH_NMID_BF;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGT;
				nm2=BGBCC_SH_NMID_BF;
			}
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
//				flip=1;
//				nm1=BGBCC_SH_NMID_CMPQHI;
//				nm2=BGBCC_SH_NMID_BF;

				nm1=BGBCC_SH_NMID_CMPQHS;
				nm2=BGBCC_SH_NMID_BT;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGE;
				nm2=BGBCC_SH_NMID_BT;
//				nm3=BGBCC_SH_NMID_BRGE;
			}
		}
		break;

	case CCXL_CMP_TST:
		nm1=BGBCC_SH_NMID_TSTQ;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_NTST:
		nm1=BGBCC_SH_NMID_TSTQ;
		nm2=BGBCC_SH_NMID_BT;
		break;

	default:
		nm1=-1;
		nm2=-1;
//		nm3=-1;
		break;
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
	{
		if(nm1==BGBCC_SH_NMID_CMPQEQ)
		{
			nm1=BGBCC_SH_NMID_CMPPEQ;
			if(sctx->has_qmul&16)
				doptrshl=0;
		}
		if(nm1==BGBCC_SH_NMID_CMPQGT)
		{
			nm1=BGBCC_SH_NMID_CMPPGT;
			if(sctx->has_qmul&16)
				doptrshl=0;
		}
		if(nm1==BGBCC_SH_NMID_CMPQGE)
			nm1=BGBCC_SH_NMID_CMPPGE;
	}
	
	if((nm1>=0) && (nm2>=0))
	{
		nm3=-1;

		if(	(sctx->has_qmul&32) &&
			(imm==shl) && !flip &&
			(imm>=0) && (imm<=31) &&
			(	BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
				BGBCC_CCXL_TypeArrayOrPointerP(ctx, type) ||
				(BGBCC_CCXL_TypeSmallLongP(ctx, type) &&
//				!BGBCC_CCXL_TypeUnsignedP(ctx, type))))
				!BGBCC_CCXL_TypeUnsignedLongP(ctx, type))))
		{
#if 1
			if(	(nm1==BGBCC_SH_NMID_CMPQEQ) &&
				(nm2==BGBCC_SH_NMID_BT)		)
					{ nm3=BGBCC_SH_NMID_CMPQEQ; }

			if(	(nm1==BGBCC_SH_NMID_CMPQEQ) &&
				(nm2==BGBCC_SH_NMID_BF)		)
					{ nm3=BGBCC_SH_NMID_CMPQNE; }

			if(	(nm1==BGBCC_SH_NMID_CMPQGT) &&
				(nm2==BGBCC_SH_NMID_BT)		)
					{ nm3=BGBCC_SH_NMID_CMPQGT; }
#endif

			if(!(sctx->has_qmul&16))
			{
#if 1
				if(	(nm1==BGBCC_SH_NMID_CMPPEQ) &&
					(nm2==BGBCC_SH_NMID_BT)		)
						{ nm3=BGBCC_SH_NMID_CMPQEQ; }

				if(	(nm1==BGBCC_SH_NMID_CMPPEQ) &&
					(nm2==BGBCC_SH_NMID_BF)		)
						{ nm3=BGBCC_SH_NMID_CMPQNE; }

				if(	(nm1==BGBCC_SH_NMID_CMPPGT) &&
					(nm2==BGBCC_SH_NMID_BT)		)
						{ nm3=BGBCC_SH_NMID_CMPQGT; }
#endif
			}

//			if(nm3<0)
//				{ imm=0; shl=-1; }
		}		

		if((imm==shl) && !flip && !doptrshl &&
			((nm3>=0) || !(sctx->emit_riscv&0x11)))
		{
			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

			if((nm3>=0) && !flip)
			{
//				printf("CMP3R-I: %04X\n", nm3);
			
				BGBCC_JX2C_EmitOpRegImmReg(ctx, sctx,
					nm3, csreg, imm, cdreg);

				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}

			BGBCC_JX2C_EmitOpImmReg(ctx, sctx, nm1, imm, csreg);

			if(nm2==BGBCC_SH_NMID_BF)
			{
				BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVNT, cdreg);
			}
			else
			{
				BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVT, cdreg);
			}
			
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

		if(sctx->has_qmul&32)
		{
			nm3=-1;
		
			if(	(nm1==BGBCC_SH_NMID_CMPQEQ) &&
				(nm2==BGBCC_SH_NMID_BT)		)
					{ nm3=BGBCC_SH_NMID_CMPQEQ; }

			if(	(nm1==BGBCC_SH_NMID_CMPQEQ) &&
				(nm2==BGBCC_SH_NMID_BF)		)
					{ nm3=BGBCC_SH_NMID_CMPQNE; }

#if 1
			if(	(nm1==BGBCC_SH_NMID_CMPQGT) &&
				(nm2==BGBCC_SH_NMID_BT)		)
					{ nm3=BGBCC_SH_NMID_CMPQGT; }
#endif

#if 1
			if(	(nm1==BGBCC_SH_NMID_CMPQGE) &&
				(nm2==BGBCC_SH_NMID_BF)		)
					{ nm3=BGBCC_SH_NMID_CMPQGT; flip=!flip; }
			
			if(	(nm1==BGBCC_SH_NMID_CMPQGE) &&
				(nm2==BGBCC_SH_NMID_BT)		)
					{ nm3=BGBCC_SH_NMID_CMPQGE; }
			
			if(	(nm1==BGBCC_SH_NMID_CMPQGT) &&
				(nm2==BGBCC_SH_NMID_BF)		)
					{ nm3=BGBCC_SH_NMID_CMPQGE; flip=!flip; }
#endif

			if(sctx->emit_riscv&0x11)
			{
				if(	(nm1==BGBCC_SH_NMID_TSTQ) &&
					(nm2==BGBCC_SH_NMID_BF)		)
						{ nm3=BGBCC_SH_NMID_TST; }

				if(	(nm1==BGBCC_SH_NMID_TSTQ) &&
					(nm2==BGBCC_SH_NMID_BT)		)
						{ nm3=BGBCC_SH_NMID_NTST; }
			}

			if(!(sctx->has_qmul&16))
			{
				if(	(nm1==BGBCC_SH_NMID_CMPPEQ) &&
					(nm2==BGBCC_SH_NMID_BT)		)
						{ nm3=BGBCC_SH_NMID_CMPQEQ; }

				if(	(nm1==BGBCC_SH_NMID_CMPPEQ) &&
					(nm2==BGBCC_SH_NMID_BF)		)
						{ nm3=BGBCC_SH_NMID_CMPQNE; }

	#if 1
				if(	(nm1==BGBCC_SH_NMID_CMPPGT) &&
					(nm2==BGBCC_SH_NMID_BT)		)
						{ nm3=BGBCC_SH_NMID_CMPQGT; }
	#endif

	#if 1
				if(	(nm1==BGBCC_SH_NMID_CMPPGE) &&
					(nm2==BGBCC_SH_NMID_BF)		)
						{ nm3=BGBCC_SH_NMID_CMPQGT; flip=!flip; }
				
				if(	(nm1==BGBCC_SH_NMID_CMPPGE) &&
					(nm2==BGBCC_SH_NMID_BT)		)
						{ nm3=BGBCC_SH_NMID_CMPQGE; }
				
				if(	(nm1==BGBCC_SH_NMID_CMPPGT) &&
					(nm2==BGBCC_SH_NMID_BF)		)
						{ nm3=BGBCC_SH_NMID_CMPQGE; flip=!flip; }
	#endif
			}

			if(nm3>=0)
			{
//				printf("CMP3R: %04X\n", nm3);
			
				if(!flip)
					BGBCC_JX2C_EmitOpRegRegReg(ctx, sctx,
						nm3, csreg, ctreg, cdreg);
				else
					BGBCC_JX2C_EmitOpRegRegReg(ctx, sctx,
						nm3, ctreg, csreg, cdreg);

				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}else
			{
//				printf("CMP3R: Miss=%04X/%04X\n", nm1, nm2);
			}
		}

//		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);

		csreg2=csreg;
		ctreg2=ctreg;
		if(doptrshl)
		{
			csreg2=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			ctreg2=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
//			BGBCC_JX2C_EmitOpRegImmReg(ctx, sctx,
//				BGBCC_SH_NMID_SHLDQ, csreg, 16, csreg2);
//			BGBCC_JX2C_EmitOpRegImmReg(ctx, sctx,
//				BGBCC_SH_NMID_SHLDQ, ctreg, 16, ctreg2);

			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_MOVZT, csreg, csreg2);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_MOVZT, ctreg, ctreg2);
		}

		if(sctx->emit_riscv&0x11)
		{
			BGBCC_DBGBREAK
		}else
		{
			if(flip)
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg2, ctreg2);
			else
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg2, csreg2);
	//		BGBCC_JX2C_EmitOpReg(ctx, sctx, BGBCC_SH_NMID_MOVT, cdreg);

	#if 0
			BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVT, cdreg);
			
			if(nm2==BGBCC_SH_NMID_BF)
			{
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_NEG, cdreg, cdreg);
				BGBCC_JX2C_EmitOpImmReg(ctx, sctx,
					BGBCC_SH_NMID_ADD, 1, cdreg);
			}
	#endif

			if(nm2==BGBCC_SH_NMID_BF)
			{
				BGBCC_JX2C_EmitDstRegOp(ctx, sctx,
					BGBCC_SH_NMID_MOVNT, cdreg);
			}else
			{
				BGBCC_JX2C_EmitDstRegOp(ctx, sctx,
					BGBCC_SH_NMID_MOVT, cdreg);
			}
		}
		
		if(doptrshl)
		{
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, csreg2);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg2);
		}

//		BGBCC_JX2_EmitOpLabel(sctx, nm2, lbl);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitJCmpVRegVRegQLong(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register sreg, ccxl_register treg,
	int cmp, int lbl)
{
	int csreg, ctreg, flip, jcflip, noflip, doptrshl;
	int csreg2, ctreg2;
//	s32 imm;
	s64 imm;
	s32 imml;
	int nm1, nm2, nm3, is_imm10un;
	int i, j, k;
	
	imm=0; imml=-1;
	noflip=0;
	is_imm10un=0;

	if((	BGBCC_CCXL_IsRegImmIntP(ctx, sreg) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, sreg)) &&
		!(	BGBCC_CCXL_IsRegImmIntP(ctx, treg) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, treg)))
	{
		nm1=cmp;
		switch(cmp)
		{
			case CCXL_CMP_EQ:		nm1=CCXL_CMP_EQ; break;
			case CCXL_CMP_NE:		nm1=CCXL_CMP_NE; break;
			case CCXL_CMP_LT:		nm1=CCXL_CMP_GT; break;
			case CCXL_CMP_GT:		nm1=CCXL_CMP_LT; break;
			case CCXL_CMP_LE:		nm1=CCXL_CMP_GE; break;
			case CCXL_CMP_GE:		nm1=CCXL_CMP_LE; break;
			case CCXL_CMP_TST:		nm1=CCXL_CMP_TST; break;
			case CCXL_CMP_NTST:		nm1=CCXL_CMP_NTST; break;
		}
		
		return(BGBCC_JX2C_EmitJCmpVRegVRegQLong(
			ctx, sctx, type, treg, sreg, nm1, lbl));
	}

	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
	{
		imm=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);

		if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			is_imm10un=1;
			
		if(sctx->has_jumbo && !(sctx->has_jcmp&2))
			is_imm10un=1;

//		if(((imm&1023)==imm) || ((imm|(~1023))==imm))
		if(((imm&1023)==imm) || ((imm|(~1023))==imm) || sctx->has_jumbo)
			noflip=1;
		
		imml=imm;
	}
	
	if(BGBCC_CCXL_IsRegImmLongP(ctx, treg))
	{
		imm=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);

		if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			is_imm10un=1;
			
		if(sctx->has_jumbo && !(sctx->has_jcmp&2))
			is_imm10un=1;

//		if(((imm&1023)==imm) || ((imm|(~1023))==imm))
		if(((imm&1023)==imm) || ((imm|(~1023))==imm) || sctx->has_jumbo)
			noflip=1;

		imml=imm;
	}
	
	doptrshl=0;
	if(sctx->abi_spillpad&4)
	{
		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, type))
		{
			doptrshl=1;
		}
	}
	
	if((imm==imml) && (imm==0) && !doptrshl)
	{
		return(BGBCC_JX2C_EmitJCmpVRegZeroQLong(
			ctx, sctx, type, sreg, cmp, lbl));
	}
	
	nm3=-1; flip=0;
	jcflip=0;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPQEQ;
		nm2=BGBCC_SH_NMID_BT;
		nm3=BGBCC_SH_NMID_BREQ;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPQEQ;
		nm2=BGBCC_SH_NMID_BF;
		nm3=BGBCC_SH_NMID_BRNE;
		break;
	case CCXL_CMP_LT:
		if(!noflip || doptrshl)
		{
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPQHI;
				nm2=BGBCC_SH_NMID_BT;

				nm3=BGBCC_SH_NMID_BRLTU;
				jcflip=0;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGT;
				nm2=BGBCC_SH_NMID_BT;

				nm3=BGBCC_SH_NMID_BRLT;
				jcflip=0;
			}
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
//				flip=1;
//				nm1=BGBCC_SH_NMID_CMPQHI;
//				nm2=BGBCC_SH_NMID_BT;

				nm1=BGBCC_SH_NMID_CMPQHS;
				nm2=BGBCC_SH_NMID_BF;

				nm3=BGBCC_SH_NMID_BRLTU;
				jcflip=0;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGE;
				nm2=BGBCC_SH_NMID_BF;
//				nm3=BGBCC_SH_NMID_BRLT;

				nm3=BGBCC_SH_NMID_BRLT;
				jcflip=0;
			}
		}
		break;
	case CCXL_CMP_GT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPQHI;
			nm2=BGBCC_SH_NMID_BT;

			nm3=BGBCC_SH_NMID_BRGTU;
			jcflip=0;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPQGT;
			nm2=BGBCC_SH_NMID_BT;
//			nm3=BGBCC_SH_NMID_BRGT;

			nm3=BGBCC_SH_NMID_BRGT;
			jcflip=0;
		}
		break;
	case CCXL_CMP_LE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPQHI;
			nm2=BGBCC_SH_NMID_BF;

			nm3=BGBCC_SH_NMID_BRLEU;
			jcflip=0;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPQGT;
			nm2=BGBCC_SH_NMID_BF;
//			nm3=BGBCC_SH_NMID_BRLE;

			nm3=BGBCC_SH_NMID_BRLE;
			jcflip=0;
		}
		break;
	case CCXL_CMP_GE:
		if(!noflip || doptrshl)
		{
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPQHI;
				nm2=BGBCC_SH_NMID_BF;

//				nm3=BGBCC_SH_NMID_BRLEU;
//				jcflip=1;
				nm3=BGBCC_SH_NMID_BRGEU;
				jcflip=0;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGT;
				nm2=BGBCC_SH_NMID_BF;

//				nm3=BGBCC_SH_NMID_BRLE;
//				jcflip=1;
				nm3=BGBCC_SH_NMID_BRGE;
				jcflip=0;
			}
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
//				flip=1;
//				nm1=BGBCC_SH_NMID_CMPQHI;
//				nm2=BGBCC_SH_NMID_BF;

				nm1=BGBCC_SH_NMID_CMPQHS;
				nm2=BGBCC_SH_NMID_BT;

//				nm3=BGBCC_SH_NMID_BRLEU;
//				jcflip=1;

				nm3=BGBCC_SH_NMID_BRGEU;
				jcflip=0;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGE;
				nm2=BGBCC_SH_NMID_BT;
//				nm3=BGBCC_SH_NMID_BRGE;

//				nm3=BGBCC_SH_NMID_BRLE;
//				jcflip=1;
				nm3=BGBCC_SH_NMID_BRGE;
				jcflip=0;
			}
		}
		break;

	case CCXL_CMP_TST:
		nm1=BGBCC_SH_NMID_TSTQ;
		nm2=BGBCC_SH_NMID_BF;

//		if(imm!=imml)
//		if(0)
		if(sctx->emit_riscv&0x11)
		{
			nm3=BGBCC_SH_NMID_BRTSTNQ;
			jcflip=0;
		}
		break;
	case CCXL_CMP_NTST:
		nm1=BGBCC_SH_NMID_TSTQ;
		nm2=BGBCC_SH_NMID_BT;

//		if(imm!=imml)
//		if(0)
		if(sctx->emit_riscv&0x11)
		{
			nm3=BGBCC_SH_NMID_BRTSTQ;
			jcflip=0;
		}
		break;

	default:
		nm1=-1;
		nm2=-1;
		nm3=-1;
		break;
	}
	
//	nm3=-1;
	
	if(BGBCC_CCXL_TypePointerP(ctx, type))
	{
		if(nm1==BGBCC_SH_NMID_CMPQEQ)
		{
			nm1=BGBCC_SH_NMID_CMPPEQ;
			if(sctx->has_qmul&16)
				doptrshl=0;
		}
		if(nm1==BGBCC_SH_NMID_CMPQGT)
		{
			nm1=BGBCC_SH_NMID_CMPPGT;
			if(sctx->has_qmul&16)
				doptrshl=0;
		}
		if(nm1==BGBCC_SH_NMID_CMPQGE)
			nm1=BGBCC_SH_NMID_CMPPGE;
	}
	
	if((nm1>=0) && (nm2>=0))
	{
//		if(BGBCC_CCXL_IsRegImmIntP(ctx, treg) && !flip)
		if((BGBCC_CCXL_IsRegImmIntP(ctx, treg) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, treg)) && is_imm10un && !flip)
		{
//			imm=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
			imm=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);

			csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, sreg);
			if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
				csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
				
			if((nm3>0) && !jcflip)
			{
				if(BGBCC_JX2_TryEmitOpImmRegLbl(sctx, nm3, imm, csreg, lbl)>0)
				{
					BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
					return(1);
				}
			}
			
			if(BGBCC_JX2_TryEmitOpImmReg(sctx, nm1, imm, csreg)>0)
			{
				BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
				BGBCC_JX2_EmitOpAutoLabel(sctx, nm2, lbl);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}

			ctreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, treg);
			if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
				ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		}else
		{
			csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, sreg);
			ctreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, treg);

			if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
				csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
				ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		}

#if 0
		if(nm3>=0)
		{
			BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);

//			BGBCC_JX2_EmitOpCmpRegRegLabel(sctx, nm3, ctreg, csreg, lbl);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
			return(1);
		}
#endif

#if 1
//		if(nm3>=0)
		if((nm3>=0) && (sctx->has_jcmp&2) &&
			BGBCC_JX2_EmitCheckRegExtGPR(sctx, csreg) &&
			BGBCC_JX2_EmitCheckRegExtGPR(sctx, ctreg))
		{
//			BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
//			BGBCC_JX2C_SetModeDqClear(ctx, sctx);

			if(jcflip)
				j=BGBCC_JX2_TryEmitOpRegRegLbl(sctx, nm3, csreg, ctreg, lbl);
			else
				j=BGBCC_JX2_TryEmitOpRegRegLbl(sctx, nm3, ctreg, csreg, lbl);
			if(j>0)
			{
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
				return(1);
			}
		}
#endif

		if(sctx->emit_riscv&0x11)
		{
			BGBCC_DBGBREAK
		}


		csreg2=csreg;
		ctreg2=ctreg;
		if(doptrshl)
		{
			csreg2=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			ctreg2=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
//			BGBCC_JX2C_EmitOpRegImmReg(ctx, sctx,
//				BGBCC_SH_NMID_SHLDQ, csreg, 16, csreg2);
//			BGBCC_JX2C_EmitOpRegImmReg(ctx, sctx,
//				BGBCC_SH_NMID_SHLDQ, ctreg, 16, ctreg2);

			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_MOVZT, csreg, csreg2);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_MOVZT, ctreg, ctreg2);
		}

		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
		if(flip)
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg2, ctreg2);
		else
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg2, csreg2);

		if(doptrshl)
		{
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, csreg2);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg2);
		}

		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);

		BGBCC_JX2_EmitOpAutoLabel(sctx, nm2, lbl);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}
	
	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitJCmpVRegZeroQLong(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register sreg,
	int cmp, int lbl)
{
	ccxl_register treg;
	int csreg, ctreg, doptrshl;
	int nm1, nm2, nm3;
	int i, j, k;

	doptrshl=0;
	if(sctx->abi_spillpad&4)
	{
		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, type))
		{
			doptrshl=1;
		}
	}

	if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
	{
		if(cmp==CCXL_CMP_LT)
		{
//			BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BT, lbl);
			return(1);
		}

		if(cmp==CCXL_CMP_GE)
		{
			BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
			BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BRAN, lbl);
			return(1);
		}

		if(cmp==CCXL_CMP_LE)
			{ cmp=CCXL_CMP_EQ; }
		if(cmp==CCXL_CMP_GT)
			{ cmp=CCXL_CMP_NE; }
	}
	
	nm3=-1;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPQEQ;
//		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BT;
		nm3=BGBCC_SH_NMID_BREQ;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPQEQ;
//		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BF;
		nm3=BGBCC_SH_NMID_BRNE;
		break;

#if 1
	case CCXL_CMP_LT:
//		nm1=BGBCC_SH_NMID_CMPPZ;
		nm1=BGBCC_SH_NMID_CMPQGE;
		nm2=BGBCC_SH_NMID_BF;
		nm3=BGBCC_SH_NMID_BRLT;
		break;
	case CCXL_CMP_GT:
//		nm1=BGBCC_SH_NMID_CMPPL;
		nm1=BGBCC_SH_NMID_CMPQGT;
		nm2=BGBCC_SH_NMID_BT;
		nm3=BGBCC_SH_NMID_BRGT;
		break;
	case CCXL_CMP_LE:
//		nm1=BGBCC_SH_NMID_CMPPL;
		nm1=BGBCC_SH_NMID_CMPQGT;
		nm2=BGBCC_SH_NMID_BF;
		nm3=BGBCC_SH_NMID_BRLE;
		break;
	case CCXL_CMP_GE:
//		nm1=BGBCC_SH_NMID_CMPPZ;
		nm1=BGBCC_SH_NMID_CMPQGE;
		nm2=BGBCC_SH_NMID_BT;
		nm3=BGBCC_SH_NMID_BRGE;
		break;
#endif

	default:
		nm1=-1;
		nm2=-1;
		nm3=-1;
		break;
	}
	
	if(doptrshl)
		nm1=-1;
	
	if((nm1>=0) && (nm2>=0))
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);

#if 0
//		if(nm3>=0)
		if((nm3>=0) && !(sctx->dfl_dq&1))
		{
//			BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
			BGBCC_JX2C_SetModeDqClear(ctx, sctx);

//			BGBCC_JX2_EmitOpCmpRegLabel(sctx, nm3, csreg, lbl);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}
#endif

		if((nm3>=0) && (sctx->has_jcmp&1) &&
			BGBCC_JX2_EmitCheckRegExtGPR(sctx, csreg))
		{
			j=BGBCC_JX2_TryEmitOpRegLbl(sctx, nm3, lbl, csreg);
			if(j>0)
			{
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}
		}

//		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);

		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);

		BGBCC_JX2C_EmitOpReg(ctx, sctx, nm1, csreg);
//		if(sctx->need_farjmp)
//			BGBCC_JX2_EmitOpFarLabel(sctx, nm2, lbl);
//		else
//			BGBCC_JX2_EmitOpLabel(sctx, nm2, lbl);

		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
		BGBCC_JX2_EmitOpAutoLabel(sctx, nm2, lbl);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	if(1)
	{
		BGBCC_CCXL_GetRegForLongValue(ctx, &treg, 0);
		return(BGBCC_JX2C_EmitJCmpVRegVRegQLong(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	BGBCC_CCXL_StubError(ctx);
//	BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
	return(0);
}


int BGBCC_JX2C_EmitPredCmpVRegVRegQLong(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register sreg, ccxl_register treg,
	int cmp)
{
	int csreg, ctreg, flip, noflip;
	s32 imm;
	int nm1, nm2, nm3;
	
	if(	(sctx->emit_riscv&0x11) &&
		!(sctx->emit_riscv&0x22))
	{
		printf("BGBCC_JX2C_EmitPredCmpVRegVRegQLong: Unsupported on RISC-V\n");
		return(0);
	}

	noflip=0;
	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
	{
		imm=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
		if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			noflip=1;
	}
	
	nm3=-1; flip=0;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPQEQ;
		nm2=BGBCC_SH_NMID_BT;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPQEQ;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_LT:
		if(!noflip)
		{
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPQHI;
				nm2=BGBCC_SH_NMID_BT;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGT;
				nm2=BGBCC_SH_NMID_BT;
			}
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
//				flip=1;
//				nm1=BGBCC_SH_NMID_CMPQHI;
//				nm2=BGBCC_SH_NMID_BT;

				nm1=BGBCC_SH_NMID_CMPQHS;
				nm2=BGBCC_SH_NMID_BF;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGE;
				nm2=BGBCC_SH_NMID_BF;
			}
		}
		break;
	case CCXL_CMP_GT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPQHI;
			nm2=BGBCC_SH_NMID_BT;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPQGT;
			nm2=BGBCC_SH_NMID_BT;
		}
		break;
	case CCXL_CMP_LE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPQHI;
			nm2=BGBCC_SH_NMID_BF;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPQGT;
			nm2=BGBCC_SH_NMID_BF;
		}
		break;
	case CCXL_CMP_GE:
		if(!noflip)
		{
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPQHI;
				nm2=BGBCC_SH_NMID_BF;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGT;
				nm2=BGBCC_SH_NMID_BF;
			}
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
//				flip=1;
//				nm1=BGBCC_SH_NMID_CMPQHI;
//				nm2=BGBCC_SH_NMID_BF;

				nm1=BGBCC_SH_NMID_CMPQHS;
				nm2=BGBCC_SH_NMID_BT;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGE;
				nm2=BGBCC_SH_NMID_BT;
				nm3=BGBCC_SH_NMID_BRGE;
			}
		}
		break;

	case CCXL_CMP_TST:
		nm1=BGBCC_SH_NMID_TSTQ;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_NTST:
		nm1=BGBCC_SH_NMID_TSTQ;
		nm2=BGBCC_SH_NMID_BT;
		break;

	default:
		nm1=-1;
		nm2=-1;
		nm3=-1;
		break;
	}
	
	if((nm1>=0) && (nm2>=0))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, treg) && !flip)
		{
			imm=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);

			csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, sreg);
			if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
				csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
				
			if(BGBCC_JX2_TryEmitOpImmReg(sctx, nm1, imm, csreg)>0)
			{
				sctx->pred_tfmd=(nm2==BGBCC_SH_NMID_BT);

//				BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
//				BGBCC_JX2_EmitOpAutoLabel(sctx, nm2, lbl);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}

			ctreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, treg);
			if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
				ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		}else
		{
			csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, sreg);
			ctreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, treg);

			if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
				csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
				ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		}

		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
		if(flip)
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg, ctreg);
		else
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, csreg);

		sctx->pred_tfmd=(nm2==BGBCC_SH_NMID_BT);

//		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
//		BGBCC_JX2_EmitOpAutoLabel(sctx, nm2, lbl);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}
	
	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitPredCmpVRegZeroQLong(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register sreg,
	int cmp)
{
	ccxl_register treg;
	int csreg, ctreg;
	int nm1, nm2, nm3;

	if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
	{
		if(cmp==CCXL_CMP_LT)
		{
			sctx->pred_tfmd=0;
//			BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BT, lbl);
			return(1);
		}

		if(cmp==CCXL_CMP_GE)
		{
			sctx->pred_tfmd=1;
//			BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
//			BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BRAN, lbl);
			return(1);
		}

		if(cmp==CCXL_CMP_LE)
			{ cmp=CCXL_CMP_EQ; }
		if(cmp==CCXL_CMP_GT)
			{ cmp=CCXL_CMP_NE; }
	}
	
	nm3=-1;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPQEQ;
		nm2=BGBCC_SH_NMID_BT;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPQEQ;
		nm2=BGBCC_SH_NMID_BF;
		break;

	default:
		nm1=-1;
		nm2=-1;
		nm3=-1;
		break;
	}
	
	if((nm1>=0) && (nm2>=0))
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);

		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);

		BGBCC_JX2C_EmitOpReg(ctx, sctx, nm1, csreg);

		sctx->pred_tfmd=(nm2==BGBCC_SH_NMID_BT);

//		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
//		BGBCC_JX2_EmitOpAutoLabel(sctx, nm2, lbl);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	if(1)
	{
		BGBCC_CCXL_GetRegForLongValue(ctx, &treg, 0);
		return(BGBCC_JX2C_EmitPredCmpVRegVRegQLong(ctx, sctx,
			type, sreg, treg, cmp));
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitCSeltSelectVRegVRegVRegQLong(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	byte flip, noflip;
	s32 imm;
	int nm1, nm2, cmp1, tr0;
	int i;
	
	csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
	ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
	cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

	BGBCC_JX2C_EmitOpRegRegReg(ctx, sctx,
		BGBCC_SH_NMID_CSELT, csreg, ctreg, cdreg);

	BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
	BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
	BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
	return(1);
}

int BGBCC_JX2C_EmitCSeltCompareVRegVRegQLong(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, int cmp, ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	byte flip, noflip;
	s32 imm;
	int nm1, nm2, cmp1, tr0;
	int i;
	
	noflip=0;
	
	flip=0;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPQEQ;
		nm2=BGBCC_SH_NMID_BT;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPQEQ;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_LT:
		if(!noflip)
		{
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPQHI;
				nm2=BGBCC_SH_NMID_BT;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGT;
				nm2=BGBCC_SH_NMID_BT;
			}
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
//				flip=1;
//				nm1=BGBCC_SH_NMID_CMPQHI;
//				nm2=BGBCC_SH_NMID_BT;

				nm1=BGBCC_SH_NMID_CMPQHS;
				nm2=BGBCC_SH_NMID_BF;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGE;
				nm2=BGBCC_SH_NMID_BF;
			}
		}
		break;
	case CCXL_CMP_GT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPQHI;
			nm2=BGBCC_SH_NMID_BT;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPQGT;
			nm2=BGBCC_SH_NMID_BT;
		}
		break;
	case CCXL_CMP_LE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPQHI;
			nm2=BGBCC_SH_NMID_BF;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPQGT;
			nm2=BGBCC_SH_NMID_BF;
		}
		break;
	case CCXL_CMP_GE:
		if(!noflip)
		{
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPQHI;
				nm2=BGBCC_SH_NMID_BF;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGT;
				nm2=BGBCC_SH_NMID_BF;
			}
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
//				flip=1;
//				nm1=BGBCC_SH_NMID_CMPQHI;
//				nm2=BGBCC_SH_NMID_BF;

				nm1=BGBCC_SH_NMID_CMPQHS;
				nm2=BGBCC_SH_NMID_BT;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGE;
				nm2=BGBCC_SH_NMID_BT;
			}
		}
		break;
	case CCXL_CMP_TST:
		nm1=BGBCC_SH_NMID_TSTQ;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_NTST:
		nm1=BGBCC_SH_NMID_TSTQ;
		nm2=BGBCC_SH_NMID_BT;
		break;

	default:
		nm1=-1;
		nm2=-1;
		break;
	}
	
	if((nm1>=0) && (nm2>=0))
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);

		if(flip)
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg, ctreg);
		else
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, csreg);

		if(nm2==BGBCC_SH_NMID_BF)
			BGBCC_JX2C_EmitOpNone(ctx, sctx, BGBCC_SH_NMID_NOTT);
		
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_JX2C_EmitBitMovVRegVRegVRegQLong(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, ccxl_register treg,
	int mshl, int mlo, int mhi)
{
	int csreg, ctreg, cdreg, csreg2, ctreg2;
	s64 imm, shl;
	int flip, noflip, doptrshl;
	int nm1, nm2, nm3, cmp1;
	int tr0, tr1;
	int i;

	BGBCC_JX2C_NormalizeImmVRegInt(ctx, sctx, type, &sreg);
	BGBCC_JX2C_NormalizeImmVRegInt(ctx, sctx, type, &treg);

	if(BGBCC_CCXL_IsRegImmSmallSIntP(ctx, sreg))
	{
		i=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
		if(i==0)
		{
			if((mlo==0) && (mhi>0) && (mshl<=0))
			{
				/* Simple bitfield extract... */
				ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
				cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

				imm=(1<<(mhi+1))-1;
				BGBCC_JX2_EmitOpRegImmReg(sctx, BGBCC_SH_NMID_SHLDQ,
					ctreg, mshl, cdreg);
				BGBCC_JX2_EmitOpRegImmReg(sctx, BGBCC_SH_NMID_AND,
					cdreg, imm, cdreg);

				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}
		}
	}

	csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
	ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
	cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

	if(sctx->has_bitmov&1)
	{
		imm=(mshl&255)|((mlo&255)<<8)|(((mhi+1)&255)<<16);
		BGBCC_JX2_EmitOpRegRegImmReg(sctx, BGBCC_SH_NMID_BITMOV,
			ctreg, csreg, imm, cdreg);
	}else
	{
//		if((mlo==0) && (mhi>0) && (mshl<=0))
//		{
//		}else
		if(1)
		{
			imm=((1LL<<(mhi+1))-1)&(~((1<<mlo)-1));
			tr0=BGBCC_JX2C_ScratchAllocTsReg(ctx, sctx,
				BGBCC_SH_REGCLS_QGR);
			tr1=BGBCC_JX2C_ScratchAllocTsReg(ctx, sctx,
				BGBCC_SH_REGCLS_QGR);

			BGBCC_JX2_EmitOpRegImmReg(sctx, BGBCC_SH_NMID_SHLDQ,
				ctreg, mshl, tr0);
			BGBCC_JX2_EmitOpRegImmReg(sctx, BGBCC_SH_NMID_AND,
				tr0, imm, tr0);
			BGBCC_JX2_EmitOpRegImmReg(sctx, BGBCC_SH_NMID_AND,
				csreg, ~imm, tr1);
			BGBCC_JX2_EmitOpRegRegReg(sctx, BGBCC_SH_NMID_OR,
				tr0, tr1, cdreg);

			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
		}
	}

	BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
	BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
	BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
	return(1);
}
