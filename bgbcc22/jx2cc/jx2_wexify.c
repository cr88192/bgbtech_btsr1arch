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

#define	BGBCC_WXSPFL_ISMEM		1
#define	BGBCC_WXSPFL_USE_SR		2
#define	BGBCC_WXSPFL_PXN		4
#define	BGBCC_WXSPFL_PXSTN		8		//Xs,Xt,Xn
#define	BGBCC_WXSPFL_2STAGE		16
#define	BGBCC_WXSPFL_3STAGE		32
#define	BGBCC_WXSPFL_IS_STORE	64

#define	BGBCC_WXSPFL_RXN		0x100	//Xn
#define	BGBCC_WXSPFL_RXS		0x200	//Xs
#define	BGBCC_WXSPFL_RXT		0x400	//Xt

/*
Flag:
  1&=Accesses Memory
  2&=Uses SR.T
  4&=Rn is Pair
  8&=Rs/Rt/Rn are Pairs
  
  16&=2 Stage Operation
  32&=3 Stage Operation
  64&=IsStore
 */

int BGBCC_JX2_CheckOps32GetRegs(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2,
	u16 *rrs, u16 *rrt, u16 *rrn, u16 *rrp,
	u16 *rspr, u16 *rspw, int *rspfl)
{
	static u16 gr2cr[32]={
		BGBCC_SH_REG_PC,
		BGBCC_SH_REG_PR,
		BGBCC_SH_REG_SR,
		BGBCC_SH_REG_VBR,
		BGBCC_SH_REG_SPC,
		BGBCC_SH_REG_SGR,
		BGBCC_SH_REG_GBR,
		BGBCC_SH_REG_TBR,
		BGBCC_SH_REG_TTB,
		BGBCC_SH_REG_TEA,
		BGBCC_SH_REG_MMCR,
		BGBCC_SH_REG_EXSR,
		BGBCC_SH_REG_STTB,
		BGBCC_SH_REG_KRR,
		BGBCC_SH_REG_ZZR,
		BGBCC_SH_REG_ZZR
	};

	u16 rs, rt, rn, rp, rnb, rs2, spr, spw, spfl;
	
	if(!(sctx->is_fixed32&2))
	{
	//	if((opw1&0xE000)!=0xE000)
		if(	((opw1&0xE000)!=0xE000) &&
			((opw1&0xF000)!=0x7000) &&
			((opw1&0xF000)!=0x9000))
			return(-1);
	}
	
	rn=((opw1>>4)&15)|((opw2>>6)&16);
	rs=((opw1   )&15)|((opw2>>5)&16);
	rt=((opw2>>4)&15)|((opw2>>4)&16);
	rp=BGBCC_SH_REG_ZZR;
	
	rnb=opw1&0x001F;
	
	if(!(sctx->is_fixed32&2))
	{
		if(	((opw1&0xF000)==0x7000) ||
			((opw1&0xF000)==0x9000))
		{
			if(opw1&0x0400)	rn+=32;
			if(opw1&0x0200)	rs+=32;
			if(opw1&0x0100)	rt+=32;
		}
	}else
	{
		if(!(opw1&0x8000))	rnb+=32;

		if(!(opw1&0x8000))	rn+=32;
		if(!(opw1&0x4000))	rs+=32;
		if(!(opw1&0x2000))	rt+=32;
		opw1|=0xE000;
	}
	
	spr=BGBCC_SH_REG_ZZR;
	spw=BGBCC_SH_REG_ZZR;
	spfl=0;

	if((opw1&0xF000)==0xE000)
	{
		spr=BGBCC_SH_REG_SR;
//		spw=BGBCC_SH_REG_SR;
		spfl|=BGBCC_WXSPFL_USE_SR;
	}

//	return(-1);
	
//	if((opw1&0xEB00)==0xE000)
	if(	((opw1&0xEB00)==0xE000) ||
		((opw1&0xFB00)==0xEA00) ||
		((opw1&0xF000)==0x7000)	)
	{
//		return(-1);

		if((opw2&0xF000)>=0xC000)
//			return(0);
			return(-1);

		if((opw2&0xF008)==0x1000)
		{
			spfl|=BGBCC_WXSPFL_2STAGE;
			
			if((opw2&0x000E)==0x0002)
				{ spfl&=~BGBCC_WXSPFL_2STAGE; spfl|=BGBCC_WXSPFL_3STAGE; }
		}

		if((opw2&0xF008)==0x1008)
		{
			rt=rn;
//			rs2=rn;

//			return(-1);

			if((opw2&0xF00F)==0x1008)
			{
				switch((opw2>>4)&15)
				{
				case 0x4:	case 0x5:
				case 0x6:	case 0x7:
				case 0x8:	case 0x9:
				case 0xC:	case 0xD:
				case 0xE:	case 0xF:
					spr=BGBCC_SH_REG_SR;
					spw=BGBCC_SH_REG_SR;
					break;
				}

				switch((opw2>>4)&15)
				{
				case 0x4:	case 0x5:
				case 0x6:	case 0x7:
				case 0x8:	case 0x9:
				case 0xA:	case 0xB:
				case 0xC:	case 0xD:
				case 0xE:
					if(opw2&0x0800)
					{
						spfl|=BGBCC_WXSPFL_PXSTN;
						spfl|=BGBCC_WXSPFL_RXS;
						spfl|=BGBCC_WXSPFL_RXN;
					}
					break;
				}
			}
				

			if((opw2&0xF00F)==0x1009)
			{
				switch((opw2>>4)&15)
				{
				case 0x4:	case 0xC:
				case 0xD:	case 0xE:
					spr=BGBCC_SH_REG_SR;
					spw=BGBCC_SH_REG_SR;
					break;
				
				case 0xA:
//					spw=gr2cr[rn];
					rn=gr2cr[rn];
//					rt=BGBCC_SH_REG_ZZR;
					break;
				case 0xB:
//					spr=gr2cr[rs];
					rs=gr2cr[rs];
//					rt=BGBCC_SH_REG_ZZR;
					break;

				case 0x8:
					rt=BGBCC_SH_REG_ZZR;
					if(opw2&0x0800)
					{
						spfl|=BGBCC_WXSPFL_PXSTN;
						spfl|=BGBCC_WXSPFL_RXS;
						spfl|=BGBCC_WXSPFL_RXN;
					}
					break;
					
				case 0x9:	case 0xF:
					return(0);
				}
			}

			if((opw2&0xF00F)==0x100A)
			{
				spr=BGBCC_SH_REG_SR;
				spw=BGBCC_SH_REG_SR;
			}

			if((opw2&0xF00F)==0x100C)
			{
				switch((opw2>>4)&15)
				{
//				case 0x2:
				case 0xE:	case 0xF:
					spr=BGBCC_SH_REG_SR;
					spw=BGBCC_SH_REG_SR;
					break;

//				case 0xA:
//				case 0xB:
//					return(0);

//				case 0x4:
//					return(0);
				}
			}

			if((opw2&0xF00F)==0x100D)
			{
				switch((opw2>>4)&15)
				{
				case 0xA:	case 0xB:
					spr=BGBCC_SH_REG_SR;
					spw=BGBCC_SH_REG_SR;
					break;
				}
			}


//			*rrs=rn;
//			*rrt=rs;

			*rrs=rs;
			*rrt=rt;

			*rrn=rn;
			*rrp=rp;
			*rspr=spr;
			*rspw=spw;
			*rspfl=spfl;
			return(1);
		}

		if((opw2&0xF000)==0x2000)
		{
			if(	((opw2&0x000C)==0x0004) ||
				((opw2&0x000C)==0x000C))
			{
				if(opw2&0x0800)
				{
					spfl|=BGBCC_WXSPFL_PXSTN;
				}
			}
		}

		if((opw2&0xF000)==0x3000)
		{
			if(	!(opw2&0x0008) &&
				((opw2&0x000F)!=0x0000) &&
				((opw2&0x000F)!=0x0007))
			{
				if(opw2&0x0800)
				{
					spfl|=BGBCC_WXSPFL_PXSTN;
				}
			}
		}

		if((opw2&0xF00F)==0x3000)
		{
//			return(0);
		
			spr=BGBCC_SH_REG_SR;
			spw=BGBCC_SH_REG_SR;
			
			rs=rt;
			rn=rt;
			
			switch(opw1&0xFF)
			{
			case 0x10:
			case 0x11:
			case 0x12:
			case 0x13:
				return(0);

			case 0x18:	case 0x19:
			case 0x1A:	case 0x1B:
			case 0x82:	case 0x83:
				spr=15;
				spw=15;
				break;
			
			case 0x1C:
			case 0x1D:

			case 0x20:
			case 0x21:
			case 0x22:
			case 0x23:
				return(0);

			case 0x68:
			case 0x69:
			case 0x6A:
			case 0x6B:
			case 0x6F:
				return(0);

			case 0x00:
			case 0x01:
			case 0x02:
				return(0);

			default:
				break;
			}

			*rrs=rt;
			*rrt=rt;
			*rrn=rt;
			*rrp=rp;
			*rspr=spr;
			*rspw=spw;
			*rspfl=spfl;
			return(1);
		}

//		if((opw2&0xF00F)==0x4000)
//		{
//			if((opw2&0x0008)==0x0000)
//				{ spr=rn; }
//		}

		if((opw2&0xF00F)==0x5000)
		{
//			return(-1);
			if((opw2&0x000F)==0x0000)
			{
				spr=BGBCC_SH_REG_SR;
				spw=BGBCC_SH_REG_SR;
			}

			if(	((opw2&0x000C)==0x0000) ||
				((opw2&0x000C)==0x0008))
			{
				if(opw2&0x0800)
				{
					spfl|=BGBCC_WXSPFL_PXSTN;
				}
			}
			
			if((opw2&0x000C)==0x000C)
			{
				spfl|=BGBCC_WXSPFL_2STAGE;
			}

			if((opw2&0x000E)==0x0002)
			{
				spfl|=BGBCC_WXSPFL_3STAGE;
			}
		}

		if((opw2&0xF000)==0x6000)
		{
			if(opw2&0x0800)
			{
				spfl|=BGBCC_WXSPFL_PXSTN;
			}
		}

		if((opw2&0xF000)==0x7000)
		{
		}

#if 0
		if((opw2&0xF00F)==0x900C)
		{
//			return(-1);
			spr=BGBCC_SH_REG_SR;
			spw=BGBCC_SH_REG_SR;
		}
#endif

//		if((opw2&0xF000)==0x0000)
		if(	((opw2&0xF000)==0x0000) ||
			((opw2&0xF000)==0x4000) ||
			((opw2&0xF000)==0x8000))
		{
//			if((opw2&0xF808)==0x0000)
			if(	((opw2&0xF808)==0x0000) ||
				((opw2&0xF808)==0x4000) ||
//				((opw2&0xF008)==0x4000) ||
				((opw2&0xF008)==0x8000))
			{
//				spr=rn;
				rp=rn;
				spfl|=BGBCC_WXSPFL_IS_STORE;
//				return(-1);
			}
		
			spfl|=BGBCC_WXSPFL_ISMEM;

			spfl|=BGBCC_WXSPFL_3STAGE;
			
			if((opw2&0xF000)==0x4000)
			{
//				spfl|=BGBCC_WXSPFL_PXN;

				if((opw2&0x0803)==0x0000)
					spfl|=BGBCC_WXSPFL_RXN;

//				if((opw2&0x0803)==0x0800)
				if((opw2&0x0803)==0x0001)
				{
					rn=gr2cr[rn];
					if(spfl&BGBCC_WXSPFL_IS_STORE)
						rp=rn;
				}
			}

			if((opw2&0xF000)==0x8000)
			{
//				spfl|=BGBCC_WXSPFL_PXN;
//				spfl|=BGBCC_WXSPFL_PXSTN;

				spfl|=BGBCC_WXSPFL_RXS;
				spfl|=BGBCC_WXSPFL_RXT;
				
			}

//			return(-1);

			if(rs==0)
			{
				rs=BGBCC_SH_REG_PC;
				if(rt==1)
					{ rs=0; rt=BGBCC_SH_REG_ZZR; }
				return(-1);
			}
			if(rs==1)
			{
				rs=BGBCC_SH_REG_GBR;
				if(rt==1)
				{
					rs=BGBCC_SH_REG_TBR;
					rt=0;
				}
//				return(-1);
			}

		}

		*rrs=rs;
		*rrt=rt;
		*rrn=rn;
		*rrp=rp;
		*rspr=spr;
		*rspw=spw;
		*rspfl=spfl;
		return(1);
	}

//	return(-1);

	if(	((opw1&0xEB00)==0xE100) ||
		((opw1&0xF100)==0x9100)	)
	{
		if((opw2&0xF000)>=0xC000)
		{
			return(-1);
		}

		spfl|=BGBCC_WXSPFL_ISMEM;

		spfl|=BGBCC_WXSPFL_3STAGE;
		
		if((opw2&0xC800)==0x0800)
			spfl&=~1;

//		if((opw2&0xF000)==0x5000)
//			spfl|=BGBCC_WXSPFL_PXN;
//		if((opw2&0xF000)==0x7000)
//			spfl|=BGBCC_WXSPFL_PXN;

		if(((opw2&0xF000)==0x5000) || ((opw2&0xF000)==0x7000))
		{
			if(opw2&0x0800)
			{
//				spfl|=BGBCC_WXSPFL_PXN;
				spfl|=BGBCC_WXSPFL_RXN;
			}else
			{
				rn=gr2cr[rn];
			}
		}

//		return(-1);

		if(rs==0)
			{ rs=BGBCC_SH_REG_PC; }
		if(rs==1)
			{ rs=BGBCC_SH_REG_GBR; }

//		if(	((opw2&0x8800)==0x0000) ||
		if(	((opw2&0xC800)==0x0000) ||
			((opw2&0xE000)==0x4000)	)
		{
			spfl|=BGBCC_WXSPFL_IS_STORE;
//			spr=rn;
			rp=rn;
		}

		*rrs=rs;
//		*rrt=BGBCC_SH_REG_ZZR;
//		*rrt=spr;
		*rrt=rp;
		*rrn=rn;
		*rrp=rp;

		*rspr=spr;
		*rspw=spw;
		*rspfl=spfl;
		return(1);
	}

//	return(-1);

//	if((opw1&0xEB00)==0xE200)
	if(	((opw1&0xEB00)==0xE200) ||
		((opw1&0xFB00)==0xEB00) ||
		((opw1&0xF100)==0x9000)	)
	{
//		return(-1);

		if((opw2&0xE000)==0x0000)
			{ spfl|=BGBCC_WXSPFL_2STAGE; }
		if((opw2&0xF000)==0x2000)
			{ spfl|=BGBCC_WXSPFL_3STAGE; }
		if((opw2&0xF000)==0x3000)
			{ spfl|=BGBCC_WXSPFL_2STAGE; }
		if((opw2&0xC000)==0x4000)
			{ spfl|=BGBCC_WXSPFL_2STAGE; }

		if((opw2&0xE000)==0x6000)
		{
//			spfl|=BGBCC_WXSPFL_2STAGE;
			if(opw2&0x0800)
				spfl|=BGBCC_WXSPFL_PXSTN;
		}


		if((opw2&0xF000)>=0xA000)
		{
			rs=rn;
		}

		if((opw2&0xF000)==0xC000)
		{
//			rs=rn;

			if((opw1&15)>=4)
//			if(1)
			{
				spr=BGBCC_SH_REG_SR;
				spw=BGBCC_SH_REG_SR;
			}else
			{
				if((opw1&15)!=2)
					rs=BGBCC_SH_REG_ZZR;
			}

//			return(-1);
		}

		if((opw2&0xF000)==0xD000)
		{
			if((opw1&0x000C)==0x0000)
			{
				spfl|=BGBCC_WXSPFL_2STAGE;
			}

			if((opw1&0x000C)==0x0008)
			{
				spfl&=~BGBCC_WXSPFL_2STAGE;
				spfl|= BGBCC_WXSPFL_3STAGE;

				spfl|=BGBCC_WXSPFL_ISMEM;
				if((opw1&0x000E)==0x0008)
				{
					rp=rn;
					spfl|=BGBCC_WXSPFL_IS_STORE;
				}

				rs=BGBCC_SH_REG_GBR;
				if((opw1&0x0001) && (opw2&0x0800))
					spfl|=BGBCC_WXSPFL_PXSTN;
			}
		}

		*rrs=rs;
		*rrt=BGBCC_SH_REG_ZZR;
		*rrn=rn;
		*rrp=rp;
		*rspr=spr;
		*rspw=spw;
		*rspfl=spfl;
		return(1);
	}

//	return(-1);

//	if((opw1&0xEE00)==0xE800)
	if((opw1&0xEB00)==0xE800)
	{
//		rn=opw1&0x001F;
		rn=rnb;
		rs=rn;
		rt=BGBCC_SH_REG_ZZR;

		if((opw1&0x00F0)<0x0040)
		{
			rs=BGBCC_SH_REG_ZZR;
		}

		if((opw1&0x00E0)==0x0040)
		{
			spfl|=BGBCC_WXSPFL_2STAGE;
		}

		*rrs=rs;
		*rrt=rt;
		*rrn=rn;
		*rrp=rp;
		*rspr=spr;
		*rspw=spw;
		*rspfl=spfl;
		return(1);
	}

//	return(-1);

//	if((opw1&0xEE00)==0xEA00)
	if((opw1&0xFE00)==0xFA00)
	{
		rn=0;
		rp=0;
		*rrs=rn;
		*rrt=BGBCC_SH_REG_ZZR;
		*rrn=rn;
		*rrp=rp;
		*rspr=spr;
		*rspw=spw;
		*rspfl=spfl;
		return(1);
	}

	return(0);
}

int BGBCC_JX2_CheckOps32ReadsRn(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2)
{
	if(sctx->is_fixed32&2)
		opw1|=0xE000;

	if(	((opw1&0xFF00)==0xF000) ||
		((opw1&0xF000)==0x7000) )
	{
		if((opw2&0xF808)==0x0000)
			return(1);
		if((opw2&0xF008)==0x4000)
			return(1);
		if((opw2&0xF008)==0x8000)
			return(1);
		return(0);
	}

	if(	((opw1&0xFF00)==0xF100) ||
		((opw1&0xF100)==0x9100)	)
	{
		if((opw2&0x8800)==0x0000)
			return(1);
		return(0);
	}

	if(	((opw1&0xFF00)==0xF200) ||
		((opw1&0xF100)==0x9000)	)
	{
		if((opw2&0xF000)==0xD000)
		{
			if((opw1&0x000E)==0x0008)
				return(1);
		}
		return(0);
	}

	return(0);
}

int BGBCC_JX2_CheckOps32IsMem(
	BGBCC_JX2_Context *sctx, int opw1, int opw2)
{
	u16 rs1, rt1, rn1, rspr1, rp1, rspw1, rs1b, rt1b, rn1b, rspr1b;
	int rspfl1, rspfl2;
	int ret1, ret2;

	ret1=BGBCC_JX2_CheckOps32GetRegs(sctx, opw1, opw2,
		&rs1, &rt1, &rn1, &rp1, &rspr1, &rspw1, &rspfl1);
		
	if(rspfl1&BGBCC_WXSPFL_ISMEM)
		return(1);
	return(0);
}

int BGBCC_JX2_CheckOps32IsLoad(
	BGBCC_JX2_Context *sctx, int opw1, int opw2)
{
	u16 rs1, rt1, rn1, rspr1, rp1, rspw1, rs1b, rt1b, rn1b, rspr1b;
	int rspfl1, rspfl2;
	int ret1, ret2;

	ret1=BGBCC_JX2_CheckOps32GetRegs(sctx, opw1, opw2,
		&rs1, &rt1, &rn1, &rp1, &rspr1, &rspw1, &rspfl1);
		
	if(rspfl1&BGBCC_WXSPFL_ISMEM)
	{
		if(rspfl1&BGBCC_WXSPFL_IS_STORE)
			return(0);

		return(1);
	}
	return(0);
}

int BGBCC_JX2_CheckOps32Is2Stage(
	BGBCC_JX2_Context *sctx, int opw1, int opw2)
{
	u16 rs1, rt1, rn1, rspr1, rp1, rspw1, rs1b, rt1b, rn1b, rspr1b;
	int rspfl1, rspfl2;
	int ret1, ret2;

	ret1=BGBCC_JX2_CheckOps32GetRegs(sctx, opw1, opw2,
		&rs1, &rt1, &rn1, &rp1, &rspr1, &rspw1, &rspfl1);
		
	if(rspfl1&BGBCC_WXSPFL_2STAGE)
		return(1);
	return(0);
}


int BGBCC_JX2_CheckOps32Is3Stage(
	BGBCC_JX2_Context *sctx, int opw1, int opw2)
{
	u16 rs1, rt1, rn1, rp1, rspr1, rspw1, rs1b, rt1b, rn1b, rspr1b;
	int rspfl1, rspfl2;
	int ret1, ret2;

	ret1=BGBCC_JX2_CheckOps32GetRegs(sctx, opw1, opw2,
		&rs1, &rt1, &rn1, &rp1, &rspr1, &rspw1, &rspfl1);
		
	if(rspfl1&BGBCC_WXSPFL_3STAGE)
		return(1);
	return(0);
}

/* Infer whether or not a pair of mem-ops can alias. */
int BGBCC_JX2_CheckOps32MemNoAlias(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2, int opw3, int opw4, int fl)
{
	static const byte isct[32]={
		1,1, 2,2, 4,4, 8,8,
		4,2, 8,8, 4,2, 8,8,
		1,1, 2,2, 4,4, 8,8,
		0,0, 0,0, 0,0, 0,0};
	static const byte iwct[32]={
		0,0, 0,0, 0,0, 0,0,
		0,0, 0,8, 0,0, 0,8,
		0,0, 0,0, 0,0, 0,0,
		0,0, 0,0, 0,0, 0,0};
	int rs1, rn1, rs2, rn2, ix1, ix2, sc1, sc2;
	int xm1, xn1, xm2, xn2, wx1, wx2;

//	return(0);

	sc1=0;
	sc2=0;
	wx1=0;
	wx2=0;

	rn1=((opw1>>4)&15)|((opw2>>6)&16);
	rs1=((opw1   )&15)|((opw2>>5)&16);

	rn2=((opw3>>4)&15)|((opw4>>6)&16);
	rs2=((opw3   )&15)|((opw4>>5)&16);


#if 1
	if(!(sctx->is_fixed32&2))
	{
		if(((opw1&0xF000)==0x7000) || ((opw1&0xF000)==0x9000))
		{
			if(opw1&0x0400)
				rn1+=32;
			if(opw1&0x0200)
				rs1+=32;
		}

		if(((opw3&0xF000)==0x7000) || ((opw3&0xF000)==0x9000))
		{
			if(opw3&0x0400)
				rn2+=32;
			if(opw3&0x0200)
				rs2+=32;
		}
	}
#endif

	if(sctx->is_fixed32&2)
	{
		if(opw1&0x8000)
			rn1+=32;
		if(opw1&0x4000)
			rs1+=32;

		if(opw3&0x8000)
			rn2+=32;
		if(opw3&0x4000)
			rs2+=32;

		opw1|=0xE000;
		opw3|=0xE000;
	}


	ix1=opw2&0x01FF;
	if(((opw1&0xEB00)==0xE100) || ((opw1&0xF100)==0x9100))
	{
		sc1=isct[(opw2>>11)&31];
		wx1=iwct[(opw2>>11)&31];
		if((rs1<2) && sc1)
			ix1/=sc1;
	}

	ix2=opw4&0x01FF;
	if(((opw3&0xEB00)==0xE100) || ((opw3&0xF100)==0x9100))
	{
		sc2=isct[(opw4>>11)&31];
		wx2=iwct[(opw4>>11)&31];
		if((rs2<2) && sc2)
			ix2/=sc2;
	}

#if 1
//	if(((opw1&0xEB0C)==0xE208) && ((opw2&0xF000)==0xD000))
	if((((opw1&0xEB0C)==0xE208) || ((opw1&0xF10C)==0x9008)) &&
		((opw2&0xF000)==0xD000))
//	if(((opw1&0xEB0E)==0xE20A) && ((opw2&0xF000)==0xD000))
	{
		rs1=1;
		sc1=(opw1&0x0001)?8:4;
		wx1=(opw2&0x0800) && (opw1&0x0001);
		ix1=opw2&0x03FF;
	}

//	if(((opw3&0xEB0C)==0xE208) && ((opw4&0xF000)==0xD000))
	if((((opw3&0xEB0C)==0xE208) || ((opw3&0xF10C)==0x9008)) &&
		((opw4&0xF000)==0xD000))
//	if(((opw3&0xEB0E)==0xE20A) && ((opw4&0xF000)==0xD000))
	{
		rs2=1;
		sc2=(opw3&0x0001)?8:4;
		wx2=(opw4&0x0800) && (opw3&0x0001);
		ix2=opw4&0x03FF;
	}
#endif


	/* Only check aliasing for F1 block for now. */
//	if((opw1&0xEB00)!=0xE100)
	if(!sc1)
	{
//		if((opw3&0xEB00)==0xE100)
		if(sc2)
		{
			/* Assume SP or GBR can't alias with indexed access. */
			if((rs2==15) && (rs1!=15))
				return(1);
//			if((rs2==1) && (rs1!=1))
//				return(1);
		}

		return(0);
	}

//	if((opw3&0xEB00)!=0xE100)
	if(!sc2)
	{
//		if((opw1&0xEB00)==0xE100)
		if(sc1)
		{
			/* Assume SP or GBR can't alias with indexed access. */
			if((rs1==15) && (rs2!=15))
				return(1);
//			if((rs1==1) && (rs2!=1))
//				return(1);
		}

		return(0);
	}

	
	/* Not Ld/St, Skip. */
	if(!sc1 || !sc2)
		return(0);
	
	/* PC rel, Skip. */
	if((rs1==0) || (rs1==0))
		return(0);
	
	/*
	 * Same Base and Scale: Different Displacement means no alias.
	 */
	if((rs1==rs2) && (sc1==sc2))
	{
		if(wx1 && ((ix1+1)==ix2))
			return(0);
		if(wx2 && ((ix2+1)==ix1))
			return(0);
	
		if(ix1!=ix2)
			return(1);
		
		return(0);
	}

	/*
	 * Same Base, Different Scale:
	 * Figure out displacement ranges and check for non-overlap.
	 */
	if((rs1==rs2) && (sc1!=sc2))
	{
		xm1=ix1*sc1;
		xm2=ix2*sc2;
		xn1=xm1+(sc1-1);
		xn2=xm2+(sc2-1);
		if(wx1)
			xn1=xm1+15;
		if(wx2)
			xn2=xm2+15;

		if(xn1<xm2)
			return(1);
		if(xn2<xm1)
			return(1);

//		if(ix1!=ix2)
//			return(1);

		return(0);
	}

//	return(0);

#if 1
	/* Assume: SP vs Non-SP:
	 * Assume aliasing can only happen at index 0.
	 * This is "less safe" than the others.
	 */
	if((rs1==15) && (rs2!=15) && (ix2!=0))
		return(1);
	if((rs2==15) && (rs1!=15) && (ix1!=0))
		return(1);
#endif

#if 1
	/* Assume: GBR vs Non-GBR:
	 * Assume aliasing can only happen at index 0.
	 * This is "less safe" than the others.
	 */
	if((rs1==1) && (rs2!=1) && (ix2!=0))
		return(1);
	if((rs2==1) && (rs1!=1) && (ix1!=0))
		return(1);
#endif

	return(0);
}

int BGBCC_JX2_RemapReg5Xn(
	BGBCC_JX2_Context *sctx, u16 xn,
	u16 *rrl, u16 *rrh, int spfl, int rix)
{
	int rb, isxn;

	if(xn==BGBCC_SH_REG_ZZR)
	{
		*rrl=xn;
		*rrh=xn;
		return(0);
	}

	isxn=0;

	if((rix==1) || (rix==2))
	{
		if(spfl&BGBCC_WXSPFL_PXSTN)
			isxn=2;
	}
	
	if(rix==1)
	{
		if(spfl&BGBCC_WXSPFL_RXS)
			isxn=3;
	}
	
	if(rix==2)
	{
		if(spfl&BGBCC_WXSPFL_RXT)
			isxn=3;
	}

	if((rix==3) || (rix==4))
	{
		if(spfl&BGBCC_WXSPFL_PXSTN)
			isxn=2;
		if(spfl&BGBCC_WXSPFL_PXN)
			isxn=2;
			
		if(spfl&BGBCC_WXSPFL_RXN)
			isxn=3;
	}

	if(isxn)
	{
//		if((xn&1) && (isxn&1))
		if(xn&1)
		{
			rb=(xn&0x1E)|32;
		}else
		{
			rb=xn;
		}

		*rrl=rb+0;
		*rrh=rb+1;
	}else
	{
		*rrl=xn;
		*rrh=BGBCC_SH_REG_ZZR;
	}

	return(1);
}

int BGBCC_JX2_CheckOps32SequenceOnlyB(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2, int opw3, int opw4, int fl)
{
	u16 rs1, rt1, rn1, rp1, rspr1, rspw1;
	u16 rs1a, rt1a, rn1a, rp1a, rspr1a;
	u16 rs1b, rt1b, rn1b, rp1b, rspr1b;
	u16 rs2, rt2, rn2, rp2, rspr2, rspw2;
	u16 rs2a, rt2a, rn2a, rp2a, rspr2a;
	u16 rs2b, rt2b, rn2b, rp2b, rspr2b;
	int rspfl1, rspfl2;
	int opw1v, opw3v;
	int ret1, ret2;

	if(!opw1 || !opw3)
		return(1);

//	if((opw1&0xFF00)==0xF400)
//	{
//		if((opw1&0xC000)==0xC000)
//		{
//			return(1);
//		}
//	}

	if((opw1&0xFE00)==0xFE00)
		return(1);
	if((opw3&0xFE00)==0xFE00)
		return(1);

	ret1=BGBCC_JX2_CheckOps32GetRegs(sctx, opw1, opw2,
		&rs1, &rt1, &rn1, &rp1, &rspr1, &rspw1, &rspfl1);
	ret2=BGBCC_JX2_CheckOps32GetRegs(sctx, opw3, opw4,
		&rs2, &rt2, &rn2, &rp2, &rspr2, &rspw2, &rspfl2);

	if(ret1<0)
//		return(1);
		return(ret1);
	if(ret2<0)
		return(ret2);
//		return(1);
	
	if(!ret1)
		return(1);
	if(!ret2)
		return(1);

	opw1v=opw1;
	opw3v=opw3;
	if(sctx->is_fixed32&2)
	{
		opw1v|=0xE000;
		opw3v|=0xE000;
	}

	if(	((opw1v&0xF000)==0xE000) &&
		((opw3v&0xF000)==0xE000) &&
		((opw1v^opw3v)&0x0400))
	{
		rn1=BGBCC_SH_REG_ZZR;
		rn2=BGBCC_SH_REG_ZZR;
		rp1=BGBCC_SH_REG_ZZR;
		rp2=BGBCC_SH_REG_ZZR;
		
		rspfl1&=~BGBCC_WXSPFL_IS_STORE;
		rspfl2&=~BGBCC_WXSPFL_IS_STORE;
	}

//	if((rspfl1&1) && (rspfl2&1))
	if(	(rspfl1&1) && (rspfl2&1) &&
		((rp1!=BGBCC_SH_REG_ZZR) || (rp2!=BGBCC_SH_REG_ZZR)) &&
		!BGBCC_JX2_CheckOps32MemNoAlias(sctx, opw1, opw2, opw3, opw4, 0))
//		((rspr1!=BGBCC_SH_REG_ZZR) || (rspr2!=BGBCC_SH_REG_ZZR)))
	{
		return(1);
	}

//	if((rspfl1&1) && (rspfl2&1) &&
//		((rspr1==BGBCC_SH_REG_ZZR) || (rspr2==BGBCC_SH_REG_ZZR)))
//		return(1);
	
	if((rs1==BGBCC_SH_REG_PC) || (rs2==BGBCC_SH_REG_PC))
		return(1);

//	if((rs1==BGBCC_SH_REG_GBR) || (rs2==BGBCC_SH_REG_GBR))
//		return(1);
	
	if(rspfl1&2)
	{
		if(rspw2==BGBCC_SH_REG_SR)
			return(1);
	}

	if(rspfl2&2)
	{
		if(rspw1==BGBCC_SH_REG_SR)
			return(1);
	}
	
	rs1b=BGBCC_SH_REG_ZZR;	rs2b=BGBCC_SH_REG_ZZR;
	rt1b=BGBCC_SH_REG_ZZR;	rt2b=BGBCC_SH_REG_ZZR;
	rp1b=BGBCC_SH_REG_ZZR;	rp2b=BGBCC_SH_REG_ZZR;
	rn1b=BGBCC_SH_REG_ZZR;	rn2b=BGBCC_SH_REG_ZZR;
	rspr1b=BGBCC_SH_REG_ZZR;	rspr2b=BGBCC_SH_REG_ZZR;

	BGBCC_JX2_RemapReg5Xn(sctx, rs1, &rs1a, &rs1b, rspfl1, 1);
	BGBCC_JX2_RemapReg5Xn(sctx, rt1, &rt1a, &rt1b, rspfl1, 2);
	BGBCC_JX2_RemapReg5Xn(sctx, rn1, &rn1a, &rn1b, rspfl1, 3);
	BGBCC_JX2_RemapReg5Xn(sctx, rp1, &rp1a, &rp1b, rspfl1, 4);

	BGBCC_JX2_RemapReg5Xn(sctx, rs2, &rs2a, &rs2b, rspfl2, 1);
	BGBCC_JX2_RemapReg5Xn(sctx, rt2, &rt2a, &rt2b, rspfl2, 2);
	BGBCC_JX2_RemapReg5Xn(sctx, rn2, &rn2a, &rn2b, rspfl2, 3);
	BGBCC_JX2_RemapReg5Xn(sctx, rp2, &rp2a, &rp2b, rspfl2, 4);

#if 0
	if(rspfl1&8)
	{

#if 0
//		rs1b=rs1+1; rt1b=rt1+1; rn1b=rn1+1;
		if(rs1!=BGBCC_SH_REG_ZZR)
			{ rs1b=rs1+1; }
		if(rt1!=BGBCC_SH_REG_ZZR)
			{ rt1b=rt1+1; }
		if(rn1!=BGBCC_SH_REG_ZZR)
			{ rn1b=rn1+1; }
		if(rp1!=BGBCC_SH_REG_ZZR)
			{ rp1b=rp1+1; }
		if(rspr1!=BGBCC_SH_REG_ZZR)
			{ rspr1b=rspr1+1; }
#endif
	}

	if(rspfl2&8)
	{
#if 0
//		rs2b=rs2+1; rt2b=rt2+1; rn2b=rn2+1;
		if(rs2!=BGBCC_SH_REG_ZZR)
			{ rs2b=rs2+1; }
		if(rt2!=BGBCC_SH_REG_ZZR)
			{ rt2b=rt2+1; }
		if(rn2!=BGBCC_SH_REG_ZZR)
			{ rn2b=rn2+1; }
		if(rp2!=BGBCC_SH_REG_ZZR)
			{ rp2b=rp2+1; }
		if(rspr2!=BGBCC_SH_REG_ZZR)
			{ rspr2b=rspr2+1; }
#endif
	}
	
	if(rspfl1&4)
	{
		BGBCC_JX2_RemapReg5Xn(sctx, rn1, &rn1, &rn1b);
		BGBCC_JX2_RemapReg5Xn(sctx, rp1, &rp1, &rp1b);

//		if(rn1!=BGBCC_SH_REG_ZZR)
//			{ rn1b=rn1+1; }
//		if(rp1!=BGBCC_SH_REG_ZZR)
//			{ rp1b=rp1+1; }
//		if(rspr1!=BGBCC_SH_REG_ZZR)
//			{ rspr1b=rspr1+1; }
	}

	if(rspfl2&4)
	{
		BGBCC_JX2_RemapReg5Xn(sctx, rn2, &rn2, &rn2b);
		BGBCC_JX2_RemapReg5Xn(sctx, rp2, &rp2, &rp2b);

//		if(rn2!=BGBCC_SH_REG_ZZR)
//			{ rn2b=rn2+1; }		
//		if(rp2!=BGBCC_SH_REG_ZZR)
//			{ rp2b=rp2+1; }		
//		if(rspr2!=BGBCC_SH_REG_ZZR)
//			{ rspr2b=rspr2+1; }
	}
#endif

	if(rn1!=BGBCC_SH_REG_ZZR)
	{
		if(		(rn2==rn1) || (rs2==rn1) ||
				(rt2==rn1) || (rp2==rn1) ||
				(rspr2==rn1))
			return(1);
		if(		(rn2a==rn1) || (rs2a==rn1) ||
				(rt2a==rn1) || (rp2a==rn1) )
			return(1);
		if(		(rn2b==rn1) || (rs2b==rn1) ||
				(rt2b==rn1) || (rp2b==rn1) )
			return(1);
	}
	if(rn1a!=BGBCC_SH_REG_ZZR)
	{
		if(		(rn2==rn1a) || (rs2==rn1a) ||
				(rt2==rn1a) || (rp2==rn1a) )
			return(1);
		if(		(rn2a==rn1a) || (rs2a==rn1a) ||
				(rt2a==rn1a) ||	(rp2a==rn1a) )
			return(1);
		if(		(rn2b==rn1a) || (rs2b==rn1a) ||
				(rt2b==rn1a) || (rp2b==rn1a) )
			return(1);
	}
	if(rn1b!=BGBCC_SH_REG_ZZR)
	{
		if(		(rn2==rn1b) || (rs2==rn1b) ||
				(rt2==rn1b) || (rp2==rn1b) ||
				(rspr2==rn1b))
			return(1);
		if(		(rn2a==rn1b) || (rs2a==rn1b) ||
				(rt2a==rn1b) ||	(rp2a==rn1b) )
			return(1);
		if(		(rn2b==rn1b) || (rs2b==rn1b) ||
				(rt2b==rn1b) || (rp2b==rn1b) ||
				(rspr2b==rn1b))
			return(1);
	}

	if(rspw1!=BGBCC_SH_REG_ZZR)
	{
		if(	(rspr2==rspw1)	||	(rn2==rspw1) ||
			(rs2==rspw1)	|| 	(rt2==rspw1) ||
			(rp2==rspw1))
				return(1);
		if(	(rn2b==rspw1)	|| (rs2b==rspw1) ||
			(rt2b==rspw1)	|| (rspr2b==rspw1)	||
			(rp2b==rspw1))
			return(1);
			
//		if(rspw1==rspr2)
//			return(1);
	}

	if(fl&1)
//	if((fl&1) || BGBCC_JX2_CheckOps32ReadsRn(sctx, opw3, opw4))
	{
//		if((opw1&0xFE00)==0xF800)
//			return(1);
//		if((opw3&0xFE00)==0xF800)
//			return(1);
	
//		return(1);

		if(	(rspfl1&BGBCC_WXSPFL_ISMEM) &&
			(rspfl2&BGBCC_WXSPFL_ISMEM) && 
			(	(rspfl1&BGBCC_WXSPFL_IS_STORE) ||
				(rspfl2&BGBCC_WXSPFL_IS_STORE)))
		{
			if(!BGBCC_JX2_CheckOps32MemNoAlias(sctx,
				opw1, opw2, opw3, opw4, 0))
			{
				return(1);
			}
		}

//		if((rspfl1&BGBCC_WXSPFL_IS_STORE) || (rspfl2&BGBCC_WXSPFL_IS_STORE))
//		{
//			return(1);
//		}

//		if(	(rspfl1&1) && (rspfl2&1) && 
//			((rspr1!=BGBCC_SH_REG_ZZR) || (rspr2!=BGBCC_SH_REG_ZZR)))
//				return(1);

//		if(	((rspfl1&1) && (rspr1!=BGBCC_SH_REG_ZZR)) ||
//			((rspfl2&1) && (rspr2!=BGBCC_SH_REG_ZZR)))
//				return(1);

		if(rn2!=BGBCC_SH_REG_ZZR)
		{
			if(	(rn1==rn2) || (rs1==rn2) ||
				(rt1==rn2) || (rp1==rn2) ||
				(rspr1==rn2))
					return(1);

			if(	(rn1a==rn2) || (rs1a==rn2) ||
				(rt1a==rn2) || (rp1a==rn2) )
					return(1);

			if(	(rn1b==rn2) || (rs1b==rn2) ||
				(rt1b==rn2) || (rp1b==rn2) ||
				(rspr1b==rn2))
					return(1);
		}

		if(rn2a!=BGBCC_SH_REG_ZZR)
		{
			if(	(rn1==rn2a) || (rs1==rn2a) ||
				(rt1==rn2a) || (rp1==rn2a) )
					return(1);
			if(	(rn1a==rn2a) || (rs1a==rn2a) ||
				(rt1a==rn2a) || (rp1a==rn2a) )
					return(1);
			if(	(rn1b==rn2a) || (rs1b==rn2a) ||
				(rt1b==rn2a) || (rp1b==rn2a) )
					return(1);
		}

		if(rn2b!=BGBCC_SH_REG_ZZR)
		{
			if(	(rn1==rn2b) || (rs1==rn2b) ||
				(rt1==rn2b) || (rp1==rn2b) ||
				(rspr1==rn2b))
					return(1);

			if(	(rn1a==rn2b) || (rs1a==rn2b) ||
				(rt1a==rn2b) || (rp1a==rn2b) )
					return(1);

			if(	(rn1b==rn2b) || (rs1b==rn2b) ||
				(rt1b==rn2b) || (rp1b==rn2b) ||
				(rspr1b==rn2b))
					return(1);
		}

		if(rspw2!=BGBCC_SH_REG_ZZR)
		{
			if(	(rspr1==rspw2)	||	(rn1==rspw2)	||
				(rs1==rspw2)	||	(rt1==rspw2)	||
				(rp1==rspw2))
					return(1);
//			if(	(rn1b==rspw2)	|| (rs1b==rspw2)	||
//				(rt1b==rspw2)	|| (rp1b==rspw2)	||
//				(rspr1b==rspw2))
//					return(1);

//			if(rspw2==rspr1)
//				return(1);
		}
	}

#if 0
	if((rspfl1&BGBCC_WXSPFL_IS_STORE) || (rspfl2&BGBCC_WXSPFL_IS_STORE))
	{
		return(1);
	}
#endif

#if 0
	if(	(rspfl1&BGBCC_WXSPFL_IS_STORE) ||
		(rspfl2&BGBCC_WXSPFL_IS_STORE))
	{
		ret2=0;

		if((opw1v&0xEB00)==0xE200)
		{
//			if((opw2&0xF000)<0xC000)
//				ret2=-1;

			if((opw2&0xF000)==0xC000)
			{
//				if(!(opw1&0x000C))
//				if(!(opw1&0x000E))
//					ret2=-1;
			}
		}
		
		if((opw3v&0xEB00)==0xE200)
		{
//			if((opw4&0xF000)<0xC000)
//				ret2=-1;


			if((opw4&0xF000)==0xC000)
			{
//				if(!(opw3&0x000C))
//				if(!(opw3&0x000E))
//					ret2=-1;
			}
		}

#if 1
//		if((rspfl1&BGBCC_WXSPFL_IS_STORE) && ((opw1&0xEB00)==0xE100))
		if((rspfl1&BGBCC_WXSPFL_IS_STORE) && ((opw1v&0xFB00)==0xF100))
		{
			if((opw3v&0xEB00)==0xE200)
			{
//				if((opw1&0x0F00)==(opw3&0x0F00))
//				if((opw1&0x00F0)==(opw3&0x00F0))
//				if(((opw1<<4)&0x00F0)==(opw3&0x00F0))
//				if((opw3&0x00E0)==0x00E0)
//				if((opw3&0x00F0)==0x00E0)
				if(((opw3v&0x00F0)==0x00E0) && ((opw3v&0x000F)==0x0000))
				{
//					if((opw1&0x000F)==0x000F)
					if((opw1v&0x00FF)==0x008F)
					{
						if((opw2&0xFF00)==0x2400)
							ret2=-1;
					}
				}
			}
		}

		if((rspfl2&BGBCC_WXSPFL_IS_STORE) && ((opw3v&0xEB00)==0xE100))
		{
//			ret2=-1;
		}
#endif
		
		if(ret2<0)
		{
			if(sctx->cgen_log)
			{
				fprintf(sctx->cgen_log,
					"BGBCC_JX2_CheckOps32SequenceOnlyB: "
							"Swap Non-Order %04X_%04X %04X_%04X\n",
						opw1, opw2, opw3, opw4);
			}

//			return(1);
			return(0);
		}
	}
#endif

	return(0);
}

u32 bgbcc_jx2_inferinterlock_op1[256];
u32 bgbcc_jx2_inferinterlock_op2[256];
u32 bgbcc_jx2_inferinterlock_op3[256];
byte bgbcc_jx2_inferinterlock_res[256];


int BGBCC_JX2_InferOps32Interlock(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2, int opw3, int opw4, int opw5, int opw6, int fl)
{
	u32 op1, op2, op3;
	int oph, res;

	op1=opw1|(opw2<<16);
	op2=opw3|(opw4<<16);
	op3=opw5|(opw6<<16);
	
//	oph=(((((op1*65521)+op2)*65521+op3)*65521)>>16)&255;

//	oph=op1;
//	oph=(oph<<7)+(oph>>25)+op2;
//	oph=(oph<<7)+(oph>>25)+op3;
	oph=op1+op2+op3;
	oph=oph+(oph>>17);
	oph=((oph*65521)>>16)&255;
	
	if(	(bgbcc_jx2_inferinterlock_op1[oph]==op1) &&
		(bgbcc_jx2_inferinterlock_op2[oph]==op2) &&
		(bgbcc_jx2_inferinterlock_op3[oph]==op3))
	{
		return(bgbcc_jx2_inferinterlock_res[oph]);
	}
	
	res=BGBCC_JX2_InferOps32InterlockI(sctx,
		opw1, opw2, opw3, opw4, opw5, opw6, fl);
	bgbcc_jx2_inferinterlock_op1[oph]=op1;
	bgbcc_jx2_inferinterlock_op2[oph]=op2;
	bgbcc_jx2_inferinterlock_op3[oph]=op3;
	bgbcc_jx2_inferinterlock_res[oph]=res;
	return(res);
}

int BGBCC_JX2_InferOps32InterlockEx2(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2, int opw3, int opw4, int opw5, int opw6,
	int opw7, int opw8, int opw9, int opw10, int fl)
{
	int cf1, cf2;
	
	cf1=BGBCC_JX2_InferOps32Interlock(sctx,
		opw1, opw2, opw3, opw4, opw5, opw6, fl);
	cf2=BGBCC_JX2_InferOps32Interlock(sctx,
		opw1, opw2, opw7, opw8, opw9, opw10, fl);
	
	if(cf1>1)
		cf1+=2;
	if(cf2>0)
		cf2--;

//	if(cf2>0)
//		cf2=(cf2-1)>>1;

	return(cf1+cf2);
}

int BGBCC_JX2_InferOps32InterlockI(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2, int opw3, int opw4, int opw5, int opw6, int fl)
{
	u16 rs1, rt1, rn1, rp1, rspr1, rspw1;
	u16 rs1a, rt1a, rn1a, rp1a, rspr1a;
	u16 rs1b, rt1b, rn1b, rp1b, rspr1b;
	u16 rs2, rt2, rn2, rp2, rspr2, rspw2;
	u16 rs2a, rt2a, rn2a, rp2a, rspr2a;
	u16 rs2b, rt2b, rn2b, rp2b, rspr2b;
	u16 rs3, rt3, rn3, rp3, rspr3, rspw3;
	u16 rs3a, rt3a, rn3a, rp3a, rspr3a;
	u16 rs3b, rt3b, rn3b, rp3b, rspr3b;
	int rspfl1, rspfl2, rspfl3;
	int ret1, ret2, ret3;

//	if((opw1&0xFF00)==0xF400)
//	{
//		if((opw1&0xC000)==0xC000)
//		{
//			return(1);
//		}
//	}

//	if((opw1&0xFE00)==0xFE00)
//		return(1);
//	if((opw3&0xFE00)==0xFE00)
//		return(1);

	ret1=BGBCC_JX2_CheckOps32GetRegs(sctx, opw1, opw2,
		&rs1, &rt1, &rn1, &rp1, &rspr1, &rspw1, &rspfl1);
	ret2=BGBCC_JX2_CheckOps32GetRegs(sctx, opw3, opw4,
		&rs2, &rt2, &rn2, &rp2, &rspr2, &rspw2, &rspfl2);
	ret3=BGBCC_JX2_CheckOps32GetRegs(sctx, opw5, opw6,
		&rs3, &rt3, &rn3, &rp3, &rspr3, &rspw3, &rspfl3);

	if(ret1<0)
		return(0);
	if(ret2<0)
		return(0);
	if(ret3<0)
		return(0);
	
	if(!ret1)
		return(0);
	if(!ret2)
		return(0);
	if(!ret3)
		return(0);

	if(rspfl1&BGBCC_WXSPFL_IS_STORE)
	{
		/* Stores do not trigger Rn interlock. */
		return(1);
	}

	rs1b=BGBCC_SH_REG_ZZR;	rs2b=BGBCC_SH_REG_ZZR;	rs3b=BGBCC_SH_REG_ZZR;
	rt1b=BGBCC_SH_REG_ZZR;	rt2b=BGBCC_SH_REG_ZZR;	rt3b=BGBCC_SH_REG_ZZR;
	rn1b=BGBCC_SH_REG_ZZR;	rn2b=BGBCC_SH_REG_ZZR;	rn3b=BGBCC_SH_REG_ZZR;
	rspr1b=BGBCC_SH_REG_ZZR;
	rspr2b=BGBCC_SH_REG_ZZR;
	rspr3b=BGBCC_SH_REG_ZZR;

	BGBCC_JX2_RemapReg5Xn(sctx, rs1, &rs1a, &rs1b, rspfl1, 1);
	BGBCC_JX2_RemapReg5Xn(sctx, rt1, &rt1a, &rt1b, rspfl1, 2);
	BGBCC_JX2_RemapReg5Xn(sctx, rn1, &rn1a, &rn1b, rspfl1, 3);
	BGBCC_JX2_RemapReg5Xn(sctx, rp1, &rp1a, &rp1b, rspfl1, 4);

	BGBCC_JX2_RemapReg5Xn(sctx, rs2, &rs2a, &rs2b, rspfl2, 1);
	BGBCC_JX2_RemapReg5Xn(sctx, rt2, &rt2a, &rt2b, rspfl2, 2);
	BGBCC_JX2_RemapReg5Xn(sctx, rn2, &rn2a, &rn2b, rspfl2, 3);
	BGBCC_JX2_RemapReg5Xn(sctx, rp2, &rp2a, &rp2b, rspfl2, 4);

	BGBCC_JX2_RemapReg5Xn(sctx, rs3, &rs3a, &rs3b, rspfl3, 1);
	BGBCC_JX2_RemapReg5Xn(sctx, rt3, &rt3a, &rt3b, rspfl3, 2);
	BGBCC_JX2_RemapReg5Xn(sctx, rn3, &rn3a, &rn3b, rspfl3, 3);
	BGBCC_JX2_RemapReg5Xn(sctx, rp3, &rp3a, &rp3b, rspfl3, 4);

#if 0

	if(rspfl1&8)
	{
		BGBCC_JX2_RemapReg5Xn(sctx, rs1, &rs1, &rs1b);
		BGBCC_JX2_RemapReg5Xn(sctx, rt1, &rt1, &rt1b);
		BGBCC_JX2_RemapReg5Xn(sctx, rn1, &rn1, &rn1b);
		BGBCC_JX2_RemapReg5Xn(sctx, rp1, &rp1, &rp1b);

#if 0
		if(rs1!=BGBCC_SH_REG_ZZR)
			{ rs1b=rs1+1; }
		if(rt1!=BGBCC_SH_REG_ZZR)
			{ rt1b=rt1+1; }
		if(rn1!=BGBCC_SH_REG_ZZR)
			{ rn1b=rn1+1; }
		if(rspr1!=BGBCC_SH_REG_ZZR)
			{ rspr1b=rspr1+1; }
#endif
	}

	if(rspfl2&8)
	{
		BGBCC_JX2_RemapReg5Xn(sctx, rs2, &rs2, &rs2b);
		BGBCC_JX2_RemapReg5Xn(sctx, rt2, &rt2, &rt2b);
		BGBCC_JX2_RemapReg5Xn(sctx, rn2, &rn2, &rn2b);
		BGBCC_JX2_RemapReg5Xn(sctx, rp2, &rp2, &rp2b);

#if 0
		if(rs2!=BGBCC_SH_REG_ZZR)
			{ rs2b=rs2+1; }
		if(rt2!=BGBCC_SH_REG_ZZR)
			{ rt2b=rt2+1; }
		if(rn2!=BGBCC_SH_REG_ZZR)
			{ rn2b=rn2+1; }
		if(rspr2!=BGBCC_SH_REG_ZZR)
			{ rspr2b=rspr2+1; }
#endif
	}

	if(rspfl3&8)
	{
		BGBCC_JX2_RemapReg5Xn(sctx, rs3, &rs3, &rs3b);
		BGBCC_JX2_RemapReg5Xn(sctx, rt3, &rt3, &rt3b);
		BGBCC_JX2_RemapReg5Xn(sctx, rn3, &rn3, &rn3b);
		BGBCC_JX2_RemapReg5Xn(sctx, rp3, &rp3, &rp3b);

#if 0
		if(rs3!=BGBCC_SH_REG_ZZR)
			{ rs3b=rs3+1; }
		if(rt3!=BGBCC_SH_REG_ZZR)
			{ rt3b=rt3+1; }
		if(rn3!=BGBCC_SH_REG_ZZR)
			{ rn3b=rn3+1; }
		if(rspr3!=BGBCC_SH_REG_ZZR)
			{ rspr3b=rspr3+1; }
#endif
	}
	
	if(rspfl1&4)
	{
		BGBCC_JX2_RemapReg5Xn(sctx, rn1, &rn1, &rn1b);
		BGBCC_JX2_RemapReg5Xn(sctx, rp1, &rp1, &rp1b);

//		if(rn1!=BGBCC_SH_REG_ZZR)
//			{ rn1b=rn1+1; }
//		if(rspr1!=BGBCC_SH_REG_ZZR)
//			{ rspr1b=rspr1+1; }
	}

	if(rspfl2&4)
	{
		BGBCC_JX2_RemapReg5Xn(sctx, rn2, &rn2, &rn2b);
		BGBCC_JX2_RemapReg5Xn(sctx, rp2, &rp2, &rp2b);

//		if(rn2!=BGBCC_SH_REG_ZZR)
//			{ rn2b=rn2+1; }		
//		if(rspr2!=BGBCC_SH_REG_ZZR)
//			{ rspr2b=rspr2+1; }
	}

	if(rspfl3&4)
	{
		BGBCC_JX2_RemapReg5Xn(sctx, rn3, &rn3, &rn3b);
		BGBCC_JX2_RemapReg5Xn(sctx, rp3, &rp3, &rp3b);

//		if(rn3!=BGBCC_SH_REG_ZZR)
//			{ rn3b=rn3+1; }		
//		if(rspr3!=BGBCC_SH_REG_ZZR)
//			{ rspr3b=rspr3+1; }
	}

#endif


	if(rn1!=BGBCC_SH_REG_ZZR)
	{
		if((rn2==rn1) || (rs2==rn1) || (rt2==rn1) || (rspr2==rn1))
			return(3);
		if((rn2b==rn1) || (rs2b==rn1) || (rt2b==rn1) || (rspr2b==rn1))
			return(3);
		if((rn2a==rn1) || (rs2a==rn1) || (rt2a==rn1) )
			return(3);

		if((rn3==rn1) || (rs3==rn1) || (rt3==rn1) || (rspr3==rn1))
			return(2);
		if((rn3b==rn1) || (rs3b==rn1) || (rt3b==rn1) || (rspr3b==rn1))
			return(2);
		if((rn3a==rn1) || (rs3a==rn1) || (rt3a==rn1) )
			return(2);
	}
	if(rn1b!=BGBCC_SH_REG_ZZR)
	{
		if((rn2==rn1b) || (rs2==rn1b) || (rt2==rn1b) || (rspr2==rn1b))
			return(3);
		if((rn2b==rn1b) || (rs2b==rn1b) || (rt2b==rn1b) || (rspr2b==rn1b))
			return(3);
		if((rn2a==rn1b) || (rs2a==rn1b) || (rt2a==rn1b) )
			return(3);

		if((rn3==rn1b) || (rs3==rn1b) || (rt3==rn1b) || (rspr3==rn1b))
			return(2);
		if((rn3b==rn1b) || (rs3b==rn1b) || (rt3b==rn1b) || (rspr3b==rn1b))
			return(2);
		if((rn3a==rn1b) || (rs3a==rn1b) || (rt3a==rn1b) )
			return(2);
	}

	if(rn1a!=BGBCC_SH_REG_ZZR)
	{
		if((rn2==rn1a) || (rs2==rn1a) || (rt2==rn1a) )
			return(3);
		if((rn2b==rn1a) || (rs2b==rn1a) || (rt2b==rn1a) )
			return(3);
		if((rn2a==rn1a) || (rs2a==rn1a) || (rt2a==rn1a) )
			return(3);

		if((rn3==rn1a) || (rs3==rn1a) || (rt3==rn1a) )
			return(2);
		if((rn3b==rn1a) || (rs3b==rn1a) || (rt3b==rn1a) )
			return(2);
		if((rn3a==rn1a) || (rs3a==rn1a) || (rt3a==rn1a) )
			return(2);
	}

	if(rspw1!=BGBCC_SH_REG_ZZR)
	{
		if(	(rspr2==rspw1)	||	(rn2==rspw1) ||
			(rs2==rspw1)	|| 	(rt2==rspw1))
				return(3);
		if(	(rn2b==rspw1)	|| (rs2b==rspw1) ||
			(rt2b==rspw1)	|| (rspr2b==rspw1))
			return(3);

		if(	(rspr3==rspw1)	||	(rn3==rspw1) ||
			(rs3==rspw1)	|| 	(rt3==rspw1))
				return(2);
		if(	(rn3b==rspw1)	|| (rs3b==rspw1) ||
			(rt3b==rspw1)	|| (rspr3b==rspw1))
			return(2);
	}

	return(1);
}

/* Return true if this operation is immovable and must remain in place. */
int BGBCC_JX2_CheckOps32Immovable(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2)
{
	return(BGBCC_JX2_CheckOps32ImmovableFl(sctx, opw1, opw2, 0));
}

int BGBCC_JX2_CheckOps32ImmovableFl(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2, int flag)
{
	if(!(sctx->is_fixed32&2))
	{
		if(	((opw1&0xE000)!=0xE000) &&
			((opw1&0xF000)!=0x7000) &&
			((opw1&0xF000)!=0x9000)	)
			return(1);
	}

	if(sctx->is_fixed32&2)
	{
		opw1|=0xE000;
	}

	if((opw1&0xFE00)==0xFA00)
	{
//		return(1);
		return(0);
	}

	if((opw1&0xFC00)==0xF400)		//WEX, F0..F3
		return(1);
//	if((opw1&0xFF00)==0xF900)
//		return(1);

//	if((opw1&0xFF00)==0xFC00)
	if((opw1&0xFC00)==0xFC00)		//WEX, F8..FB
		return(1);
	
	if((opw1&0xFA00)==0xEA00)		//PrWEX
		return(1);


//	if((opw1&0xEB00)==0xE000)
	if(	((opw1&0xEB00)==0xE000) ||
		((opw1&0xFB00)==0xEA00) ||
		((opw1&0xF000)==0x7000) )		//F0zz | PrWEX-F0
	{
		if((opw2&0xF000)>=0xC000)
			return(1);

		if((opw2&0xF000)==0x0000)
		{
			if((opw1&0x000F)==0x0000)	//Ld/St, Rb=PC
				return(1);

//			if((opw2&0x8800)==0x0000)
//				return(1);
		}

		if((opw2&0xF000)==0x4000)
		{
			if((opw1&0x000F)==0x0000)	//Ld/St, Rb=PC
				return(1);
		}

		if((opw2&0xF000)==0x8000)
		{
			if((opw1&0x000F)==0x0000)	//Ld/St, Rb=PC
				return(1);
		}

		return(0);
	}

	if(	((opw1&0xEB00)==0xE100) ||
		((opw1&0xF100)==0x9100)	)
	{
		if((opw2&0xF000)>=0xC000)
			return(1);
		if((opw1&0x000F)==0x0000)
//		if((opw1&0x000E)==0x0000)
			return(1);

//		if((opw2&0x8800)==0x0000)
//			return(1);
		return(0);
	}

	return(0);
}


u64		bgbcc_jx2_seqonly_opex[4096];
byte	bgbcc_jx2_seqonly_res[4096];

int BGBCC_JX2_CheckOps32SequenceOnly(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2, int opw3, int opw4)
{
	u64 opex, opeh;
	int ret, h, res;
	
	opex=	(((u64)opw1)<< 0) |
			(((u64)opw2)<<16) |
			(((u64)opw3)<<32) |
			(((u64)opw4)<<48) ;

	opeh=opex+(opex>>7)+(opex>>13)+(opex>>21);
//	h=((opeh*65521)>>16)&255;
	h=((opeh*65521)>>16)&4095;
	
	if(bgbcc_jx2_seqonly_opex[h]==opex)
	{
		return(bgbcc_jx2_seqonly_res[h]);
	}
	
	ret=BGBCC_JX2_CheckOps32SequenceOnlyB(sctx,
		opw1, opw2, opw3, opw4, 1);
	
	res=(ret!=0);
	bgbcc_jx2_seqonly_opex[h]=opex;
	bgbcc_jx2_seqonly_res[h]=res;
	
#if 0
	if(res &&
		((opw1&0xFF00)==0xF200) &&
		((opw3&0xFF00)==0xF200) &&
		(((opw1>>4)&15)!=(opw3&15)) &&
		(((opw3>>4)&15)!=(opw1&15)) &&
		(((opw1>>4)&15)!=((opw1>>4)&15)))
	{
		printf("Sequence-Only: %04X-%04X %04X-%04X\n", opw1, opw2, opw3, opw4);
	}
#endif

	return(res);

//	if(ret!=0)
//		return(1);
//	return(0);
}

u64		bgbcc_jx2_canswap_opex[256];
byte	bgbcc_jx2_canswap_res[256];

int BGBCC_JX2_CheckCanSwapOps32(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2, int opw3, int opw4)
{
	u64 opex, opeh;
	int ret, h, res;
	
	opex=	(((u64)opw1)<< 0) |
			(((u64)opw2)<<16) |
			(((u64)opw3)<<32) |
			(((u64)opw4)<<48) ;
//	opeh=(opex*65521)+(opex>>48);
//	opeh=(opeh*65521)+(opeh>>48);
//	opeh=(opeh*65521)+(opeh>>48);
//	opeh=(opeh*65521)+(opeh>>48);
//	h=(opeh>>48)&255;

	opeh=opex+(opex>>7)+(opex>>13)+(opex>>21);
	h=((opeh*65521)>>16)&255;
	
	if(bgbcc_jx2_canswap_opex[h]==opex)
	{
		return(bgbcc_jx2_canswap_res[h]);
	}
	
//	if(BGBCC_JX2_CheckOps32Immovable(sctx, opw1, opw2))
//		return(0);
//	if(BGBCC_JX2_CheckOps32Immovable(sctx, opw3, opw4))
//		return(0);

	if(	BGBCC_JX2_CheckOps32Immovable(sctx, opw1, opw2) ||
		BGBCC_JX2_CheckOps32Immovable(sctx, opw3, opw4))
	{
		bgbcc_jx2_canswap_opex[h]=opex;
		bgbcc_jx2_canswap_res[h]=0;
		return(0);
	}
	
	ret=BGBCC_JX2_CheckOps32SequenceOnlyB(sctx,
		opw1, opw2, opw3, opw4, 1);
	
	res=!ret;
	if(ret<0)
		res=0;
	
	bgbcc_jx2_canswap_opex[h]=opex;
	bgbcc_jx2_canswap_res[h]=res;
//	bgbcc_jx2_canswap_ret[h]=ret;
	
	return(res);
	
//	if(ret<0)
//		return(0);
//	return(!ret);
}

int BGBCC_JX2_CheckOps32ValidWexSuffix(
	BGBCC_JX2_Context *sctx, int opw1, int opw2)
{
	return(BGBCC_JX2_CheckOps32ValidWexSuffixFl(sctx, opw1, opw2, 0));
}

int BGBCC_JX2_CheckOps32ValidWexSuffix3W(
	BGBCC_JX2_Context *sctx, int opw1, int opw2)
{
	if(sctx->use_wexmd<2)
		return(0);

	return(BGBCC_JX2_CheckOps32ValidWexSuffixFl(sctx, opw1, opw2, 1));
}

int BGBCC_JX2_CheckOps32ValidWexSuffixFl(
	BGBCC_JX2_Context *sctx, int opw1, int opw2, int fl)
{
	if(BGBCC_JX2_CheckOps32Immovable(sctx, opw1, opw2)!=0)
		return(0);

//	return(0);

	if(!(sctx->is_fixed32&2))
	{
		if(	((opw1&0xE000)!=0xE000) &&
			((opw1&0xF000)!=0x7000) &&
			((opw1&0xF000)!=0x9000)	)
			return(0);
	}else
	{
		opw1|=0xE000;
	}

	if((opw1&0xFE00)==0xFE00)
		return(0);

//	if((opw1&0xFE00)==0xFA00)
//		return(0);


	if((opw1&0xF000)==0xF000)
	{
		if(opw1&0x0400)
			return(0);
	}

	if(	((opw1&0xF000)==0x7000)	||
		((opw1&0xF000)==0x9000)	)
	{
		if(opw1&0x0800)
			return(0);
	}

	if((opw1&0xF000)==0xE000)
	{
		if((opw1&0xFA00)==0xEA00)
			return(0);
	}

//	if((opw1&0xFF00)==0xF000)
	if(	((opw1&0xEB00)==0xE000) ||
		((opw1&0xF000)==0x7000)	)
	{
		if((opw2&0xF000)>=0xC000)
			return(0);

//		if((sctx->use_wexmd==1) || (fl&1))
		if(1)
		{
			if((opw2&0xF808)==0xF000)
//			if((opw2&0xF808)==0x0000)
				return(0);
		}

		if((opw2&0xF008)==0x0000)
		{
			if((sctx->use_wexmd==1) || (fl&1))
				return(0);
		}

		if((opw2&0xF008)==0x1000)
		{
			if(opw2&0x0800)
			{
				/* 128-bit ALU ops. */
				return(0);
			}
		}

		if((opw2&0xF008)==0x1008)
		{
			switch(opw2&0xFF)
			{
			case 0x48:	case 0x58:
			case 0x68:	case 0x78:
			case 0x88:	case 0x98:
			case 0xA8:	case 0xB8:
			case 0xC8:	case 0xD8:
			case 0xE8:	case 0xF8:

			case 0x89:

			case 0x0C:
			case 0x1C:
			case 0xAC:
			case 0xBC:

			case 0x0D:	case 0x1D:
			case 0x2D:	case 0x3D:
			case 0x4D:	case 0x5D:
			case 0x6D:	case 0x7D:
			case 0x8D:	case 0x9D:
			case 0xAD:	case 0xBD:
			case 0xCD:	case 0xDD:
			case 0xED:	case 0xFD:

				if(opw2&0x0800)
					return(0);

				break;

			case 0xA9:
				return(0);
			}
		}

		if((opw2&0xF000)==0x2000)
		{
			if(	((opw2&0x000C)==0x0004) ||
				((opw2&0x000C)==0x000C)	)
			{
				if(opw2&0x0800)
				{
					/* 128-bit SIMD ops. */
					return(0);
				}
			}
		}

		if((opw2&0xF00F)==0x3000)
		{
			if((opw1&0x00F0)==0x0000)
			{
				return(0);
			}
			return(0);
		}

		if((opw2&0xF003)==0x4000)
//		if((opw2&0xF803)==0x4000)
		{
			/* MOV.X */
			return(0);
		}

		if((opw2&0xF000)==0x5000)
		{
			if(	((opw2&0x000C)==0x0000) ||
				((opw2&0x000C)==0x0008)	)
			{
				if(opw2&0x0800)
				{
					/* 128-bit SIMD ops. */
					return(0);
				}
			}
			return(1);
		}

		if((opw2&0xF000)==0x6000)
		{
			if((opw2&0x000C)==0x0000)
				return(0);
			if((opw2&0x080C)==0x0808)
				return(0);
			if((opw2&0x080C)==0x080C)
				return(0);
			return(1);
		}

		if((opw2&0xF000)==0x8000)
		{
			/* XMOV.x */
			return(0);
		}

		return(1);
	}

//	if((opw1&0xFF00)==0xF100)
	if(	((opw1&0xEB00)==0xE100)	||
		((opw1&0xF100)==0x9100)	)
	{
		if((sctx->use_wexmd==1) || (fl&1))
//		if(1)
		{
			/* Disallow Store in 3-wide bundles. */
			if((opw2&0x8800)==0x0000)
				return(0);
		}
		
		if((opw1&0x000F)==0x0000)	//PC
			return(0);

		if((opw2&0xC000)==0x4000)	//MOV.X / Misc
			return(0);
		if((opw2&0xC000)==0xC000)	//JCMP
			return(0);

		return(1);
	}

//	if((opw1&0xFF00)==0xF200)
	if(	((opw1&0xEB00)==0xE200)	||
		((opw1&0xF100)==0x9000)	)
	{
		if((opw2&0xC000)==0x8000)
		{
			if((opw2&0x0900)==0x0900)
			{
				/* 128-bit SIMD ops. */
				return(0);
			}
		}

		if((opw2&0xE800)==0x6800)
		{
			/* 128-bit ALUX ops. */
			return(0);
		}

		if((opw2&0xF800)==0xD800)
		{
			if((opw1&0x000D)==0x0009)
			{
				/* 128-bit Load/Store */
				return(0);
			}
		}

		if((opw2&0xF000)==0xD000)
		{
			if((opw1&0x000C)==0x0008)
			{
				/* Disallow in 3-wide bundles. */
				if((sctx->use_wexmd==1) || (fl&1))
					return(0);
			}
		}


		return(1);
	}

//	if((opw1&0xFF00)==0xF800)
	if((opw1&0xEB00)==0xE800)
	{
		if((opw1&0x00E0)==0x0080)
		{
//			if(fl&1)
//				return(0);
		}
	
		return(1);
	}

	return(0);
}

int BGBCC_JX2_CheckOps32ValidWexPrefix3W(
	BGBCC_JX2_Context *sctx, int opw1, int opw2)
{
	int ret;
	
	ret = BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw1, opw2);
	if(ret <= 0)
		return(ret);

	if(sctx->is_fixed32&2)
		opw1|=0xE000;

	if(	((opw1&0xEF00)==0xE000) ||
		((opw1&0xF000)==0x7000))
	{
		if((opw2&0xF0EF)==0x106C)
		{
			/* Disallow shift in Lane 3 */
			return(0);
		}

		if((opw2&0xF00E)==0x2002)
		{
			/* Disallow shift in Lane 3 */
			return(0);
		}

		if((opw2&0xF000)==0x3000)
		{
			/* Disallow misc in Lane 3 */
			return(0);
		}

		if((opw2&0xF00E)==0x5004)
		{
			/* Disallow shift in Lane 3 */
			return(0);
		}

		if(
			((opw2&0xF00F)==0x100D) ||
			((opw2&0xF00C)==0x5008) ||
			((opw2&0xF00E)==0x6006) ||
			((opw2&0xF00C)==0x600C)
			)
		{
			/* Disallow FPU in Lane 3 */
			return(0);
		}
	}

	if(	((opw1&0xEF00)==0xE200) ||
		((opw1&0xF100)==0x9000))
	{
		if((opw2&0xE000)==0x8000)
		{
			/* Disallow shift in Lane 3 */
			return(0);
		}
	}

	if(	((opw1&0xEF00)==0xE100) ||
		((opw1&0xF100)==0x9100))
	{
		/* Disallow Load/Store Lane 3
		 * In profiles that allow it in Lane 2.
		 */
		return(0);
	}

	if((opw1&0xEFE0)==0xE880)
	{
		/* Disallow "FLDCH Imm, Rn" in Lane 3. */
		return(0);
	}

	return(1);
}

/* Check if operation is a valid prefix.
 * Also allows certain special cases for 2-wide bundles.
 */
int BGBCC_JX2_CheckOps32ValidWexPrefix2B(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2, int opw3, int opw4)
{
	int ret;
	
	ret = BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw1, opw2);
	if(ret > 0)
		return(ret);

	if(sctx->is_fixed32&2)
	{
		opw1|=0xE000;
		opw3|=0xE000;
	}

	if((sctx->use_wexmd==3) || (sctx->use_wexmd==5))
	{
#if 1
		if(	(	((opw1&0xFF00)==0xF100) ||
				((opw1&0xF100)==0x9100) ) &&
				((opw2&0xC800)==0x0800) &&
				((opw1&0x000E)!=0x0000)
			)
		{
			if(sctx->abi_spillpad&4)
			{
				/* No LEA transform if bounds-checking is on. */
				return(0);
			}

			/* Allow LEA */
			return(1);
		}
#endif

#if 1
		if(	(	((opw3&0xFF00)==0xF100) ||
				((opw3&0xF100)==0x9100) ) &&
//				((opw4&0xC000)==0x8000) &&
			(	((opw4&0xC800)==0x0000) ||
				((opw4&0xC000)==0x8000) ) &&
				((opw3&0x000E)!=0x0000)
			)
		{
			if(	(	((opw1&0xFF00)==0xF100) ||
					((opw1&0xF100)==0x9100) ) &&
					((opw2&0xC000)==0x8000) &&
					((opw1&0x000E)!=0x0000)
				)
			{
				/* Allow pairing load with other ops. */
				return(1);
			}
		}
#endif
	}

	if((sctx->use_wexmd==3) || (sctx->use_wexmd==5))
	{
#if 1
		if(	(	((opw3&0xFF00)==0xF100) ||
				((opw3&0xF100)==0x9100) ) &&
			(	((opw4&0xC800)==0x0000) ||
				((opw4&0xC000)==0x8000) )
			)
		{
			if((opw1&0xEB00)==0xE000)
			{
				if((opw2&0xF00C)==0x5008)
				{
					/* Allow FPU paired with Mem */
					return(1);
				}

//					((opw2&0xF00F)==0x100D) ||
			}
		}
#endif
	}

	return(0);
}

int BGBCC_JX2_CheckOps32ValidWexPrefix(
	BGBCC_JX2_Context *sctx, int opw1, int opw2)
{
	int ret, rn;

	if(BGBCC_JX2_CheckOps32Immovable(sctx, opw1, opw2)!=0)
		return(0);

	if(BGBCC_JX2_CheckOps32ValidWexSuffixFl(sctx, opw1, opw2, 1)<=0)
		return(0);

	rn=(opw1>>4)&15;
	if(opw2&0x0400)
		rn|=16;

	if(sctx->is_fixed32&2)
	{
		if(!(opw1&0x8000))
			rn|=32;
	}

//	if((rn==0) || (rn==1) || (rn==15))
	if((rn==1) || (rn==15))
	{
		/* Disallow Rn to be DLR, DHR, or SP, in WEX'ed ops */
		return(0);
	}

	if(!(sctx->is_fixed32&2))
	{
		if(	((opw1&0xF000)!=0xF000) &&
			((opw1&0xF000)!=0xE000) &&
			((opw1&0xF000)!=0x7000) &&
			((opw1&0xF000)!=0x9000))
				return(0);
	}else
	{
		opw1|=0xE000;
	}

	if((opw1&0xF000)==0xE000)
	{
		if(	((opw1&0xFB00)!=0xE000) &&
			((opw1&0xFB00)!=0xE200) )
				return(0);
	}

	if((opw1&0xFE00)==0xFA00)
		return(0);

	if((opw1&0xFE00)==0xFE00)
		return(0);

//	return(0);

//	if(	((opw1&0xFF00)==0xF000) ||
	if(	((opw1&0xEF00)==0xE000) ||
		((opw1&0xF000)==0x7000))
	{
//		return(0);

		ret=0;
		switch((opw2>>12)&15)
		{
		case 0x0:
			if(	((opw2&0xF80F)==0x0804) &&
				((opw1&0x000E)!=0x0000) &&
				((opw2&0x00E0)!=0x0000) &&
				((opw1&0xFF00)==0xF000) )
			{
				/* Allow LEA.B */
				ret=1;
				break;
			}
			ret=0;
			break;
		case 0x1:
			switch(opw2&15)
			{
			case 0x0:	case 0x1:
			case 0x5:	case 0x6:
			case 0x7:
				if(opw2&0x0800)
					{ ret=0; break; }
				ret=1;
				break;

			case 0x8:
				ret=0;
				break;

			case 0x9:
				switch((opw2>>4)&15)
				{
				case 0x0:	case 0x1:
				case 0x5:	case 0x6:
				case 0x7:
					ret=1;
					break;

				case 0x8:
					if(opw2&0x0800)
						{ ret=0; break; }
					ret=1;
					break;

				default:
					break;
				}
				break;
			case 0xA:
				switch((opw2>>4)&15)
				{
				case 0x8:	case 0xA:
					ret=1;
					break;
				default:
					break;
				}
				break;
			
			case 0xC:
				switch((opw2>>4)&15)
				{
				case 0x0:	case 0x1:
				case 0x5:	case 0x6:
				case 0x7:	case 0x8:
				case 0x9:
					ret=1;
					break;
				default:
					break;
				}
				break;
			
			default:
				break;
			}
			break;
		case 0x2:
			switch(opw2&15)
			{
			case 0x0:	case 0x1:
			case 0x8:	case 0x9:
				ret=1;
				break;
			default:
				break;
			}
			break;
		case 0x3:
			break;
		case 0x4:
			switch(opw2&15)
			{
			case 0x0:	case 0x4:
			case 0x8:	case 0xC:
				ret=0;
				break;
			default:
				break;
			}
			break;
		case 0x5:
			switch(opw2&15)
			{
			case 0x0:
			case 0x1:
			case 0x4:	case 0x5:
			case 0x6:	case 0x7:
			case 0xC:	case 0xD:
			case 0xE:	case 0xF:
				ret=1;
				break;
			default:
				break;
			}
			break;

		case 0x6:
			ret=0;
			break;

		case 0x7:
			ret=0;
			break;

		case 0x8:
			ret=0;
			break;

		default:
			break;
		}
		return(ret);
	}

//	if(	((opw1&0xFF00)==0xF100) ||
//		((opw1&0xF100)==0x9100))
//	{
//		return(0);
//	}

	if((opw1&0xFF00)==0xF100)
	{
#if 1
		if(((opw2&0xC800)==0x0800) && ((opw1&0x000E)!=0))
		{
			/* Allow LEA.x, if it can be turned into ADD. */
			if((opw2&0xF000)==0x0000)
				return(1);

#if 1
			if((opw2&0xF000)==0x1000)
			{
				if((opw2&0x01FF)==(opw2&0x00FF))
					return(1);
				return(0);
			}
			if((opw2&0xF000)==0x2000)
			{
				if((opw2&0x01FF)==(opw2&0x007F))
					return(1);
				return(0);
			}
			if((opw2&0xF000)==0x3000)
			{
				if((opw2&0x01FF)==(opw2&0x003F))
					return(1);
				return(0);
			}
#endif
		}
#endif

		return(0);
	}

	if((opw1&0xF100)==0x9100)
	{
		return(0);
	}

//	if(	((opw1&0xFF00)==0xF200) ||
	if(	((opw1&0xEB00)==0xE200) ||
		((opw1&0xF100)==0x9000)	)
	{
//		return(0);

		ret=0;
		switch((opw2>>12)&15)
		{
		case 0x0:	case 0x1:
		case 0x3:
		case 0x4:	case 0x5:
		case 0x8:	case 0x9:
			ret=1;
			break;

		case 0x6:	case 0x7:
			if(opw2&0x0800)
			{
				ret=0;
				break;
			}

			ret=1;
			break;
			
		case 0x2:
			ret=0;
			break;

		case 0xC:
			ret=0;
			switch(opw1&15)
			{
			case 0x0:
			case 0x1:
			case 0x2:
			case 0x3:
				ret=1;
				break;

			default:
				break;
			}
			break;

		case 0xD:
			ret=0;
			switch(opw1&15)
			{
			case 0x0:	case 0x1:
			case 0x2:
//			case 0x3:
				ret=1;
				break;

			case 0x4:	case 0x5:
			case 0x6:	case 0x7:
				ret=0;
				break;

			case 0x8:	case 0x9:
			case 0xA:	case 0xB:
				ret=0;	/* Load/Store GBR */
				break;

			default:
				break;
			}
			break;

		}
		return(ret);
	}

	if((opw1&0xFF00)==0xF800)
	{
//		if((opw1&0xE)==0x0)
//			return(0);
//		if((opw1&0xF)==0xF)
//			return(0);

		rn=opw1&31;
//		if((rn==0) || (rn==1) || (rn==15))
		if((rn==1) || (rn==15))
			return(0);

		ret=0;
		switch((opw1>>4)&15)
		{
		case 0:		case 1:		ret=1;	break;
		case 2:		case 3:		ret=1;	break;
		case 4:		case 5:		ret=1;	break;
		case 6:		case 7:		ret=1;	break;
//		case 8:		case 9:		ret=0;	break;
//		case 8:		case 9:		ret=1;	break;
		case 8:		case 9:		ret=sctx->has_fpim&1;	break;
		default:
			break;
		}
		return(ret);

//		return(0);
//		return(1);
	}

	return(0);
}

ccxl_status BGBCC_JX2_AdjustWexifyOp(
	BGBCC_JX2_Context *sctx,
	int *ropw1, int *ropw2)
{
	int opw1, opw2, opw1v;

	if(sctx->is_simpass)
		return(0);

	opw1=*ropw1;
	opw2=*ropw2;
	opw1v=opw1;

	if(sctx->is_fixed32&2)
	{
		opw1v=opw1|0xE000;
	}

	if((opw1v&0xFF00)==0xF000)
	{
		if((opw2&0xF80F)==0x0804)
		{
			/* If LEA.B, Transform into ADD */
			opw2=0x1000 | (opw2&0x07F0);
		}
	}

	if((sctx->use_wexmd==3) || (sctx->use_wexmd==5))
	{
		if((opw1v&0xFF00)==0xF100)
		{
			sctx->opcnt_hi8[(opw1>>8)&0xFF]--;
			opw1|=0x0400;
			sctx->opcnt_hi8[(opw1>>8)&0xFF]++;
			*ropw1=opw1;
			*ropw2=opw2;
			return(1);
		}
	}else
		if((opw1v&0xFF00)==0xF100)
	{
		if((opw2&0xC800)==0x0800)
		{
			sctx->opcnt_hi8[0xF1]--;
			sctx->opcnt_hi8[0xF2]++;

			/* If LEA, Transform into ADD */
			if((opw2&0xF000)==0x0000)
			{
//				opw1=0xF200|(opw1&0x00FF);
				opw1=0x1200|(opw1&0xE0FF);
				opw2=0x0000|(opw2&0x07FF);
			}
			else
				if((opw2&0xF000)==0x1000)
			{
//				opw1=0xF200|(opw1&0x00FF);
				opw1=0x1200|(opw1&0xE0FF);
				opw2=0x0000|(opw2&0x0600)|
					((opw2&0x00FF)<<1);
			}else
				if((opw2&0xF000)==0x2000)
			{
//				opw1=0xF200|(opw1&0x00FF);
				opw1=0x1200|(opw1&0xE0FF);
				opw2=0x0000|(opw2&0x0600)|
					((opw2&0x007F)<<2);
			}else
				if((opw2&0xF000)==0x3000)
			{
//				opw1=0xF200|(opw1&0x00FF);
				opw1=0x1200|(opw1&0xE0FF);
				opw2=0x0000|(opw2&0x0600)|
					((opw2&0x003F)<<3);
			}
		}
	}
	
//	if((opw1&0xFF00)==0xF800)
	if((opw1v&0xFE00)==0xF800)
	{
		sctx->opcnt_hi8[(opw1>>8)&0xFF]--;
//		opw1|=0x0100;
		opw1|=0x0400;
		sctx->opcnt_hi8[(opw1>>8)&0xFF]++;
		*ropw1=opw1;
		*ropw2=opw2;
		return(1);
	}

	/* F0, F1, F2, F3 */
	if((opw1v&0xFC00)==0xF000)
	{
		sctx->opcnt_hi8[(opw1>>8)&0xFF]--;
		opw1|=0x0400;
		sctx->opcnt_hi8[(opw1>>8)&0xFF]++;
		*ropw1=opw1;
		*ropw2=opw2;
		return(1);
	}

	if(	((opw1v&0xF800)==0x7000) ||
		((opw1v&0xF800)==0x9000))
	{
		sctx->opcnt_hi8[(opw1>>8)&0xFF]--;
		opw1|=0x0800;
		sctx->opcnt_hi8[(opw1>>8)&0xFF]++;
		*ropw1=opw1;
		*ropw2=opw2;
		return(1);
	}

	if((opw1v&0xFB00)==0xE000)
	{
		sctx->opcnt_hi8[(opw1>>8)&0xFF]--;
		opw1|=0x0A00;
		sctx->opcnt_hi8[(opw1>>8)&0xFF]++;
		*ropw1=opw1;
		*ropw2=opw2;
		return(1);
	}

	if((opw1v&0xFB00)==0xE200)
	{
		sctx->opcnt_hi8[(opw1>>8)&0xFF]--;
		opw1|=0x0B00;
		sctx->opcnt_hi8[(opw1>>8)&0xFF]++;
		*ropw1=opw1;
		*ropw2=opw2;
		return(1);
	}

	BGBCC_DBGBREAK

	return(0);
}

int BGBCC_JX2_InferInterlockCost(
	BGBCC_JX2_Context *sctx,
	int opwn1, int opwn2,
	int opw1, int opw2, int opw3, int opw4,
	int opw5, int opw6, int opw7, int opw8,
	int opw9, int opw10, int opw11, int opw12)
{
	int cfn1, cf1, cf2, cf3, cf4;
	int cn1, c1, c2, c3, c4, c5;

#if 0
//	cfn1=BGBCC_JX2_InferOps32Interlock(sctx,
//		opwn1, opwn2, opw1, opw2, opw3, opw4, 0);
//	cf1=BGBCC_JX2_InferOps32Interlock(sctx,
//		opw1, opw2, opw3, opw4, opw5, opw6, 0);
//	cf2=BGBCC_JX2_InferOps32Interlock(sctx,
//		opw3, opw4, opw5, opw6, opw7, opw8, 0);
#endif

#if 1
	cfn1=BGBCC_JX2_InferOps32InterlockEx2(sctx,
		opwn1, opwn2, opw1, opw2, opw3, opw4,
		opw5 , opw6 , opw7, opw8, 0);
	cf1=BGBCC_JX2_InferOps32InterlockEx2(sctx,
		opw1, opw2, opw3, opw4 , opw5, opw6,
		opw7, opw8, opw9, opw10, 0);
	cf2=BGBCC_JX2_InferOps32InterlockEx2(sctx,
		opw3, opw4 , opw5 , opw6 , opw7, opw8,
		opw9, opw10, opw11, opw12, 0);
#endif

	cf3=BGBCC_JX2_InferOps32Interlock(sctx,
		opw5, opw6, opw7, opw8, opw9, opw10, 0);
	cf4=BGBCC_JX2_InferOps32Interlock(sctx,
		opw7, opw8, opw9, opw10, opw11, opw12, 0);

#if 0
	cf1=BGBCC_JX2_InferOps32Interlock(sctx,
			opw1, opw2, opw3, opw4, opw5, opw6, 0) +
		BGBCC_JX2_InferOps32Interlock(sctx,
			opw1, opw2, opw5, opw6, opw7, opw8, 0);
	cf2=BGBCC_JX2_InferOps32Interlock(sctx,
			opw3, opw4, opw5, opw6, opw7, opw8, 0) +
		BGBCC_JX2_InferOps32Interlock(sctx,
			opw3, opw4, opw7, opw8, opw9, opw10, 0);

	cf3=BGBCC_JX2_InferOps32Interlock(sctx,
			opw5, opw6, opw7, opw8, opw9, opw10, 0) +
		BGBCC_JX2_InferOps32Interlock(sctx,
			opw5, opw6, opw9, opw10, opw11, opw12, 0);
#endif

	cn1=1;
	c1=1;
	c2=1;
	c3=1;
	c4=1;
	
//	cf1=cf1*2;
//	cf2=cf2*2;

//	if(	BGBCC_JX2_CheckOps32IsMem(sctx, opwn1, opwn2) ||
//		BGBCC_JX2_CheckOps32Is3Stage(sctx, opwn1, opwn2))
	if(	BGBCC_JX2_CheckOps32IsLoad(sctx, opwn1, opwn2) ||
		(BGBCC_JX2_CheckOps32Is3Stage(sctx, opwn1, opwn2) &&
		!BGBCC_JX2_CheckOps32IsMem(sctx, opwn1, opwn2)))
	{
		cn1=cfn1;
	}else if(BGBCC_JX2_CheckOps32Is2Stage(sctx, opwn1, opwn2))
	{
		cn1=cfn1-1;
		if(cn1<1)
			cn1=1;
	}

//	if(	BGBCC_JX2_CheckOps32IsMem(sctx, opw1, opw2) ||
//		BGBCC_JX2_CheckOps32Is3Stage(sctx, opw1, opw2))
	if(	BGBCC_JX2_CheckOps32IsLoad(sctx, opw1, opw2) ||
		(BGBCC_JX2_CheckOps32Is3Stage(sctx, opw1, opw2) &&
		!BGBCC_JX2_CheckOps32IsMem(sctx, opw1, opw2)))
	{
		c1=cf1;
	}else if(BGBCC_JX2_CheckOps32Is2Stage(sctx, opw1, opw2))
	{
		c1=cf1-1;
//		c1=cf1-2;
		if(c1<1)
			c1=1;
	}

//	if(	BGBCC_JX2_CheckOps32IsMem(sctx, opw3, opw4) ||
//		BGBCC_JX2_CheckOps32Is3Stage(sctx, opw3, opw4))
	if(	BGBCC_JX2_CheckOps32IsLoad(sctx, opw3, opw4) ||
		(BGBCC_JX2_CheckOps32Is3Stage(sctx, opw3, opw4) &&
		!BGBCC_JX2_CheckOps32IsMem(sctx, opw3, opw4)))
	{
		c2=cf2;
	}else if(BGBCC_JX2_CheckOps32Is2Stage(sctx, opw3, opw4))
	{
		c2=cf2-1;
//		c2=cf2-2;
		if(c2<1)
			c2=1;
	}

#if 1
//	if(	BGBCC_JX2_CheckOps32IsMem(sctx, opw5, opw6) ||
//		BGBCC_JX2_CheckOps32Is3Stage(sctx, opw5, opw6))
	if(	BGBCC_JX2_CheckOps32IsLoad(sctx, opw5, opw6) ||
		(BGBCC_JX2_CheckOps32Is3Stage(sctx, opw5, opw6) &&
		!BGBCC_JX2_CheckOps32IsMem(sctx, opw5, opw6)))
	{
		c3=cf3;
	}else if(BGBCC_JX2_CheckOps32Is2Stage(sctx, opw5, opw6))
	{
		c3=cf3-1;
		if(c3<1)
			c3=1;
	}

//	if(	BGBCC_JX2_CheckOps32IsMem(sctx, opw7, opw8) ||
//		BGBCC_JX2_CheckOps32Is3Stage(sctx, opw7, opw8))
	if(	BGBCC_JX2_CheckOps32IsLoad(sctx, opw7, opw8) ||
		(BGBCC_JX2_CheckOps32Is3Stage(sctx, opw7, opw8) &&
		!BGBCC_JX2_CheckOps32IsMem(sctx, opw7, opw8)))
	{
		c4=cf4;
	}else if(BGBCC_JX2_CheckOps32Is2Stage(sctx, opw7, opw8))
	{
		c4=cf4-1;
		if(c4<1)
			c4=1;
	}
#endif	

	c5=0;
	if(BGBCC_JX2_CheckOps32SequenceOnly(sctx, opwn1, opwn2, opw1, opw2))
		c5++;
	if(BGBCC_JX2_CheckOps32SequenceOnly(sctx, opw1, opw2, opw3, opw4))
		c5++;
	if(BGBCC_JX2_CheckOps32SequenceOnly(sctx, opw3, opw4, opw5, opw6))
		c5++;
	if(BGBCC_JX2_CheckOps32SequenceOnly(sctx, opw5, opw6, opw7, opw8))
		c5++;
	if(BGBCC_JX2_CheckOps32SequenceOnly(sctx, opw7, opw8, opw9, opw10))
		c5++;
	if(BGBCC_JX2_CheckOps32SequenceOnly(sctx, opw9, opw10, opw11, opw12))
		c5++;
	
	if(c1<1)	c1=1;
	if(c2<1)	c2=1;
	if(c3<1)	c3=1;
	if(c4<1)	c4=1;
	
//	if(cf1<=0)
//		c1=999;
//	if(cf2<=0)
//		c2=999;
//	return(c1+c2);

	return(cn1+c1+c2+c3+c4+c5);
}

ccxl_status BGBCC_JX2_OptInterlock_DoSwaps(
	BGBCC_JX2_Context *sctx,
	int spos, int epos)
{
	int opw1, opw2, opw3, opw4, opw5, opw6, opw7, opw8;
	int opw9, opw10, opw11, opw12;
	int opw1v, opw3v, opw5v, opw7v, opw9v, opw11v, opwn1v;
	int opwn1, opwn2;
	int wx1, wx3, wx5, wxn1, imv1, imv3, imv5, imv7, imvn1;
	int dp, cp, nswap, nadvl;
	int i, j, k;

//	return(0);
	
	dp=epos-spos;
	if(dp&3)
	{
		BGBCC_DBGBREAK
		return(0);
	}

	if(dp<0)
	{
		BGBCC_DBGBREAK
		return(0);
	}

	if(sctx->sec!=BGBCC_SH_CSEG_TEXT)
	{
		BGBCC_DBGBREAK
	}
	
	if(spos==0x1764)
	{
		nswap=-1;
	}

//	if(sctx->is_simpass)
//		return(0);

//	return(0);
	
	nswap=0;	nadvl=256;
	cp=spos;
//	while((cp+7)<epos)
	while((cp+11)<epos)
//	while((cp+15)<epos)
	{
		if(nadvl>128)
			nadvl=128;
	
		nadvl--;
		if(nadvl<=0)
		{
			cp+=4;
			nadvl+=8;
			continue;
		}
	
		opw1=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 0);
		opw2=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 2);
		opw3=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 4);
		opw4=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 6);
		opw5=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 8);
		opw6=BGBCC_JX2_EmitGetOffsWord(sctx, cp+10);
		opw7=BGBCC_JX2_EmitGetOffsWord(sctx, cp+12);
		opw8=BGBCC_JX2_EmitGetOffsWord(sctx, cp+14);

		opw9=BGBCC_JX2_EmitGetOffsWord(sctx, cp+16);
		opw10=BGBCC_JX2_EmitGetOffsWord(sctx, cp+18);
		opw11=BGBCC_JX2_EmitGetOffsWord(sctx, cp+20);
		opw12=BGBCC_JX2_EmitGetOffsWord(sctx, cp+22);
		
		opwn1=BGBCC_JX2_EmitGetOffsWord(sctx, cp- 4);
		opwn2=BGBCC_JX2_EmitGetOffsWord(sctx, cp- 2);

		opw1v =opw1;		opw3v =opw3;
		opw5v =opw5;		opw7v =opw7;
		opw9v =opw9;		opw11v=opw11;
		opwn1v=opwn1;
		
		if(sctx->is_fixed32&2)
		{
			opw1v =opw1 |0xE000;
			opw3v =opw3 |0xE000;
			opw5v =opw5 |0xE000;
			opw7v =opw7 |0xE000;
			opw9v =opw9 |0xE000;
			opw11v=opw11|0xE000;
			opwn1v=opwn1|0xE000;
		}
		
//		if(!((cp+11)<epos))
//		{
//			opw5=0;
//			opw6=0;
//		}

//		if(!((cp+15)<epos))
//		{
//			opw7=0;
//			opw8=0;
//		}

		if(!(sctx->is_fixed32&2))
		{
	//		if((opw1&0xE000)!=0xE000)
			if(	((opw1&0xE000)!=0xE000) &&
				((opw1&0xF000)!=0x7000) &&
				((opw1&0xF000)!=0x9000))
				{ BGBCC_DBGBREAK }
	//		if((opw3&0xE000)!=0xE000)
			if(	((opw3&0xE000)!=0xE000) &&
				((opw3&0xF000)!=0x7000) &&
				((opw3&0xF000)!=0x9000))
				{ BGBCC_DBGBREAK }
		}

		if((cp<spos) || (cp>=epos))
			{ BGBCC_DBGBREAK }

		if(!(sctx->is_fixed32&2))
		{
			if((cp+11)<epos)
			{
	//			if((opw5&0xE000)!=0xE000)
				if(	((opw5&0xE000)!=0xE000) &&
					((opw5&0xF000)!=0x7000) &&
					((opw5&0xF000)!=0x9000))
					{ BGBCC_DBGBREAK }
			}

			if((cp+15)<epos)
			{
	//			if((opw7&0xE000)!=0xE000)
				if(	((opw7&0xE000)!=0xE000) &&
					((opw7&0xF000)!=0x7000) &&
					((opw7&0xF000)!=0x9000))
					{ BGBCC_DBGBREAK }
			}
		}

//		if((opw1&0xFE00)==0xFA00)
//		{
//			cp+=8;
//			continue;
//		}

		/* Skip Jumbo Forms */
		if((opw1v&0xFE00)==0xFE00)
		{
			if((opw3v&0xFE00)==0xFE00)
			{
				cp+=12;
				continue;
			}

			cp+=8;
			continue;
		}

		wxn1=0;
		wx1=0;
		wx3=0;
		wx5=0;

		if(	((opwn1v&0xFC00)==0xF400) ||
			((opwn1v&0xFC00)==0xFC00)	||
			((opwn1v&0xFE00)==0xEA00) ||
			((opwn1v&0xFE00)==0xEE00) ||
			((opwn1v&0xF800)==0x7800) ||
			((opwn1v&0xF800)==0x9800))
		{
			wxn1=1;
		}

		if(	((opw1v&0xFC00)==0xF400) ||
			((opw1v&0xFC00)==0xFC00)	||
			((opw1v&0xFE00)==0xEA00) ||
			((opw1v&0xFE00)==0xEE00) ||
			((opw1v&0xF800)==0x7800) ||
			((opw1v&0xF800)==0x9800))
		{
			wx1=1;
		}

		if(	((opw3v&0xFC00)==0xF400) ||
			((opw3v&0xFC00)==0xFC00)	||
			((opw3v&0xFE00)==0xEA00) ||
			((opw3v&0xFE00)==0xEE00) ||
			((opw3v&0xF800)==0x7800) ||
			((opw3v&0xF800)==0x9800))
		{
			wx3=1;
		}

		if(	((opw5v&0xFC00)==0xF400) ||
			((opw5v&0xFC00)==0xFC00)	||
			((opw5v&0xFE00)==0xEA00) ||
			((opw5v&0xFE00)==0xEE00) ||
			((opw5v&0xF800)==0x7800) ||
			((opw5v&0xF800)==0x9800))
		{
			wx5=1;
		}
		
		if(wx1)
		{
			if(wx3)
			{
				if(wx5)
				{
					cp+=8;
					continue;
				}
				cp+=12;
				continue;
			}
			cp+=8;
			continue;
		}
		
		imv1=0;		imv3=0;		imv5=0;		imv7=0;
		imvn1=0;

#if 1
		/* If ops are immovable, skip. */
		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw1, opw2)>0)
			{ imv1=1; }
		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw3, opw4)>0)
			{ imv3=1; }
		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw5, opw6)>0)
			{ imv5=1; }
		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw7, opw8)>0)
			{ imv7=1; }

		if((opw7v&0xFE00)==0xFE00)
			{ imv7=1; }
		if((opw5v&0xFE00)==0xFE00)
			{ imv5=1; imv7=1; }
		if((opw3v&0xFE00)==0xFE00)
			{ imv3=1; imv5=1; }
		if((opw1v&0xFE00)==0xFE00)
			{ imv1=1; imv3=1; }
		if((opwn1v&0xFE00)==0xFE00)
			{ imv1=1; }
		
		if(wxn1)
			{ imv1=1; }

		if(!((cp+15)<epos))
			{ imv7=1; }
		if(!((cp+11)<epos))
			{ imv5=1; }
		if(!((cp+ 7)<epos))
			{ imv3=1; }
#endif

#if 0
		/* If ops are immovable, skip. */
		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw1, opw2)>0)
			{ cp+=4; continue; }
		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw3, opw4)>0)
			{ cp+=4; continue; }
		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw5, opw6)>0)
			{ cp+=4; continue; }
		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw7, opw8)>0)
			{ cp+=4; continue; }

		if((opw7v&0xFE00)==0xFE00)
			{ opw7=0; }
		if((opw5v&0xFE00)==0xFE00)
			{ opw5=0; opw7=0; }
		if((opw3v&0xFE00)==0xFE00)
			{ opw3=0; opw5=0; }
#endif

#if 0
//		if((opw1&0xEB00)==0xE100)
//		{
//			i=BGBCC_JX2_LookupRelocAtOffs(sctx, sctx->sec, cp+0);
//			if(i>=0)
//				{ cp+=4; continue; }
//		}

		if((opw3v&0xEB00)==0xE100)
		{
			i=BGBCC_JX2_LookupRelocAtOffs(sctx, sctx->sec, cp+4);
			if(i>=0)
				opw3=0;
		}

		if((opw5v&0xEB00)==0xE100)
		{
			i=BGBCC_JX2_LookupRelocAtOffs(sctx, sctx->sec, cp+8);
			if(i>=0)
				opw5=0;
		}

		if((opw7v&0xEB00)==0xE100)
		{
			i=BGBCC_JX2_LookupRelocAtOffs(sctx, sctx->sec, cp+12);
			if(i>=0)
				opw7=0;
		}
#endif

#if 1
		i=BGBCC_JX2_LookupRelocAtOffs(sctx, sctx->sec, cp+0);
		if(i>=0)
			{ imv1=1; imvn1=1; }
//			{ cp+=4; continue; }
		i=BGBCC_JX2_LookupRelocAtOffs(sctx, sctx->sec, cp+4);
		if(i>=0)
			{ imv3=1; }
//			{ cp+=4; continue; }
		i=BGBCC_JX2_LookupRelocAtOffs(sctx, sctx->sec, cp+8);
		if(i>=0)
			{ imv5=1; }
//			{ cp+=4; continue; }
		i=BGBCC_JX2_LookupRelocAtOffs(sctx, sctx->sec, cp+12);
		if(i>=0)
			{ imv7=1; }
//			{ cp+=4; continue; }
#endif

#if 1
		i=BGBCC_JX2_LookupLabelAtOffsNoLLn(sctx, sctx->sec, cp+4);
		if(i>=0)
			{ cp+=4; continue; }
		i=BGBCC_JX2_LookupLabelAtOffsNoLLn(sctx, sctx->sec, cp+8);
		if(i>=0)
			{ imv5=1; imv7=1; }
		i=BGBCC_JX2_LookupLabelAtOffsNoLLn(sctx, sctx->sec, cp+12);
		if(i>=0)
			{ imv7=1; }
#endif

//		if(imv1)
//			{ cp+=4; continue; }

#if 1
//		if(	!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
//				opw1, opw2, opw3, opw4) &&
//			!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
//				opw3, opw4, opw5, opw6) &&
//			!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
//				opw1, opw2, opw5, opw6))

#if 1
		if(	opw1 && opw3 &&
			BGBCC_JX2_CheckCanSwapOps32(sctx,
				opw1, opw2, opw3, opw4) &&
			!(imv1|imv3))
		{
			if(	BGBCC_JX2_InferInterlockCost(sctx,
					opwn1, opwn2,
					opw3, opw4, opw1, opw2, opw5, opw6, opw7, opw8,
					opw9, opw10, opw11, opw12) <
				BGBCC_JX2_InferInterlockCost(sctx,
					opwn1, opwn2,
					opw1, opw2, opw3, opw4, opw5, opw6, opw7, opw8,
					opw9, opw10, opw11, opw12))
			{
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 0, opw3);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 2, opw4);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 4, opw1);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 6, opw2);
				nswap++;
				continue;
			}
		}
#endif

		if(	opw3 && opw5 &&
			BGBCC_JX2_CheckCanSwapOps32(sctx,
				opw3, opw4, opw5, opw6) &&
			!(imv3|imv5))
		{
			if(	BGBCC_JX2_InferInterlockCost(sctx,
					opwn1, opwn2,
					opw1, opw2, opw5, opw6, opw3, opw4, opw7, opw8,
					opw9, opw10, opw11, opw12) <
				BGBCC_JX2_InferInterlockCost(sctx,
					opwn1, opwn2,
					opw1, opw2, opw3, opw4, opw5, opw6, opw7, opw8,
					opw9, opw10, opw11, opw12))
			{
//				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 0, opw1);
//				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 2, opw2);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 4, opw5);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 6, opw6);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 8, opw3);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+10, opw4);
				nswap++;
				continue;
			}
//			cp+=12;
//			continue;
		}

		if(	opw5 && opw7 &&
			BGBCC_JX2_CheckCanSwapOps32(sctx,
				opw5, opw6, opw7, opw8) &&
			!(imv5|imv7))
		{
			if(	BGBCC_JX2_InferInterlockCost(sctx,
					opwn1, opwn2,
					opw1, opw2, opw3, opw4, opw7, opw8, opw5, opw6,
					opw9, opw10, opw11, opw12) <
				BGBCC_JX2_InferInterlockCost(sctx,
					opwn1, opwn2,
					opw1, opw2, opw3, opw4, opw5, opw6, opw7, opw8,
					opw9, opw10, opw11, opw12))
			{
//				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 0, opw1);
//				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 2, opw2);
//				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 4, opw3);
//				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 6, opw4);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 8, opw7);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+10, opw8);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+12, opw5);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+14, opw6);
				nswap++;
				continue;
			}
//			cp+=12;
//			continue;
		}

		if(	opw3 && opw5 && opw7 &&
			BGBCC_JX2_CheckCanSwapOps32(sctx,
				opw3, opw4, opw5, opw6) &&
			BGBCC_JX2_CheckCanSwapOps32(sctx,
				opw3, opw4, opw7, opw8)	&&
			BGBCC_JX2_CheckCanSwapOps32(sctx,
				opw5, opw6, opw7, opw8) &&
			!(imv3|imv5|imv7))
		{
			if(	BGBCC_JX2_InferInterlockCost(sctx,
					opwn1, opwn2,
					opw1, opw2, opw7, opw8, opw3, opw4, opw5, opw6,
					opw9, opw10, opw11, opw12) <
				BGBCC_JX2_InferInterlockCost(sctx,
					opwn1, opwn2,
					opw1, opw2, opw3, opw4, opw5, opw6, opw7, opw8,
					opw9, opw10, opw11, opw12))
			{
//				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 0, opw1);
//				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 2, opw2);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 4, opw7);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 6, opw8);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 8, opw3);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+10, opw4);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+12, opw5);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+14, opw6);
				nswap++;
				continue;
			}

			if(	BGBCC_JX2_InferInterlockCost(sctx,
					opwn1, opwn2,
					opw1, opw2, opw5, opw6, opw7, opw8, opw3, opw4,
					opw9, opw10, opw11, opw12) <
				BGBCC_JX2_InferInterlockCost(sctx,
					opwn1, opwn2,
					opw1, opw2, opw3, opw4, opw5, opw6, opw7, opw8,
					opw9, opw10, opw11, opw12))
			{
//				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 0, opw1);
//				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 2, opw2);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 4, opw5);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 6, opw6);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 8, opw7);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+10, opw8);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+12, opw3);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+14, opw4);
				nswap++;
				continue;
			}
		}
#endif

#if 1
		if(	opw3 && opw5 && opw7 &&
			BGBCC_JX2_CheckCanSwapOps32(sctx,
				opw3, opw4, opw5, opw6) &&
			BGBCC_JX2_CheckCanSwapOps32(sctx,
				opw5, opw6, opw7, opw8) &&
			BGBCC_JX2_CheckCanSwapOps32(sctx,
				opw3, opw4, opw7, opw8)	&&
			!(imv3|imv7))
		{
			if(	BGBCC_JX2_InferInterlockCost(sctx,
					opwn1, opwn2,
					opw1, opw2, opw7, opw8, opw5, opw6, opw3, opw4,
					opw9, opw10, opw11, opw12) <
				BGBCC_JX2_InferInterlockCost(sctx,
					opwn1, opwn2,
					opw1, opw2, opw3, opw4, opw5, opw6, opw7, opw8,
					opw9, opw10, opw11, opw12))
			{
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 4, opw7);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 6, opw8);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 8, opw5);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+10, opw6);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+12, opw3);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+14, opw4);
				nswap++;
				continue;
			}
		}
#endif

#if 1
		if(	opw1 && opw3 && opw5 && opw7 &&
			BGBCC_JX2_CheckCanSwapOps32(sctx,
				opw1, opw2, opw3, opw4) &&
			BGBCC_JX2_CheckCanSwapOps32(sctx,
				opw1, opw2, opw5, opw6) &&
			BGBCC_JX2_CheckCanSwapOps32(sctx,
				opw1, opw2, opw7, opw8) &&
			BGBCC_JX2_CheckCanSwapOps32(sctx,
				opw3, opw4, opw5, opw6) &&
			BGBCC_JX2_CheckCanSwapOps32(sctx,
				opw3, opw4, opw7, opw8)	&&
			BGBCC_JX2_CheckCanSwapOps32(sctx,
				opw5, opw6, opw7, opw8)
			)
		{
			if(	!(imv1|imv3|imv5|imv7) &&
				BGBCC_JX2_InferInterlockCost(sctx,
					opwn1, opwn2,
					opw7, opw8, opw1, opw2, opw3, opw4, opw5, opw6,
					opw9, opw10, opw11, opw12) <
				BGBCC_JX2_InferInterlockCost(sctx,
					opwn1, opwn2,
					opw1, opw2, opw3, opw4, opw5, opw6, opw7, opw8,
					opw9, opw10, opw11, opw12))
			{
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 0, opw7);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 2, opw8);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 4, opw1);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 6, opw2);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 8, opw3);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+10, opw4);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+12, opw5);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+14, opw6);
				nswap++;
				continue;
			}

			if(	!(imv1|imv3|imv5|imv7) &&
				BGBCC_JX2_InferInterlockCost(sctx,
					opwn1, opwn2,
					opw3, opw4, opw5, opw6, opw7, opw8, opw1, opw2,
					opw9, opw10, opw11, opw12) <
				BGBCC_JX2_InferInterlockCost(sctx,
					opwn1, opwn2,
					opw1, opw2, opw3, opw4, opw5, opw6, opw7, opw8,
					opw9, opw10, opw11, opw12))
			{
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 0, opw3);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 2, opw4);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 4, opw5);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 6, opw6);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 8, opw7);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+10, opw8);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+12, opw1);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+14, opw2);
				nswap++;
				continue;
			}

			if(	!(imv1|imv7) &&
				BGBCC_JX2_InferInterlockCost(sctx,
					opwn1, opwn2,
					opw7, opw8, opw3, opw4, opw5, opw6, opw1, opw2,
					opw9, opw10, opw11, opw12) <
				BGBCC_JX2_InferInterlockCost(sctx,
					opwn1, opwn2,
					opw1, opw2, opw3, opw4, opw5, opw6, opw7, opw8,
					opw9, opw10, opw11, opw12))
			{
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 0, opw7);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 2, opw8);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 4, opw3);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 6, opw4);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 8, opw5);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+10, opw6);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+12, opw1);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+14, opw2);
				nswap++;
				continue;
			}
		}
#endif

		cp+=4;
		nadvl+=32;
		continue;
	}
	
	return(nswap>0);
}

#if 1
ccxl_status BGBCC_JX2_CheckWexify_DoSwaps(
	BGBCC_JX2_Context *sctx,
	int spos, int epos)
{
	int opw1, opw2, opw3, opw4, opw5, opw6;
	int opw1v, opw3v, opw5v, opw7v, opw9v, opw11v, opwn1v;
	int opwn1, opwn2;
	int wx1, wx3, wx5, wxn1, imv1, imv3, imv5, imvn1;
	int dp, cp, nswap;
	int i, j, k;
	
	dp=epos-spos;
	if(dp&3)
	{
		BGBCC_DBGBREAK
		return(0);
	}

	if(dp<0)
	{
		BGBCC_DBGBREAK
		return(0);
	}

	if(sctx->sec!=BGBCC_SH_CSEG_TEXT)
	{
		BGBCC_DBGBREAK
	}
	
	if(spos==0x1764)
	{
		nswap=-1;
	}

//	if(sctx->is_simpass)
//		return(0);

//	return(0);
	
	nswap=0;
	cp=spos;
	while((cp+7)<epos)
	{
		opw1=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 0);
		opw2=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 2);
		opw3=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 4);
		opw4=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 6);
		opw5=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 8);
		opw6=BGBCC_JX2_EmitGetOffsWord(sctx, cp+10);
		
		opwn1=BGBCC_JX2_EmitGetOffsWord(sctx, cp- 4);
		opwn2=BGBCC_JX2_EmitGetOffsWord(sctx, cp- 2);
		
		opw1v =opw1;		opw3v =opw3;
		opw5v =opw5;		opwn1v=opwn1;
		
		if(sctx->is_fixed32&2)
		{
			opw1v =opw1 |0xE000;
			opw3v =opw3 |0xE000;
			opw5v =opw5 |0xE000;
			opwn1v=opwn1|0xE000;
		}

		if(!((cp+11)<epos))
		{
			opw5=0;
			opw6=0;
		}

		if(!(sctx->is_fixed32&2))
		{
	//		if((opw1&0xE000)!=0xE000)
			if(	((opw1&0xE000)!=0xE000) &&
				((opw1&0xF000)!=0x7000) &&
				((opw1&0xF000)!=0x9000))
				{ BGBCC_DBGBREAK }
	//		if((opw3&0xE000)!=0xE000)
			if(	((opw3&0xE000)!=0xE000) &&
				((opw3&0xF000)!=0x7000) &&
				((opw3&0xF000)!=0x9000))
				{ BGBCC_DBGBREAK }
		}

		if((cp<spos) || (cp>=epos))
			{ BGBCC_DBGBREAK }

		if((cp+11)<epos)
		{
			if(!(sctx->is_fixed32&2))
			{
	//			if((opw5&0xE000)!=0xE000)
				if(	((opw5&0xE000)!=0xE000) &&
					((opw5&0xF000)!=0x7000) &&
					((opw5&0xF000)!=0x9000))
					{ BGBCC_DBGBREAK }
			}
		}

		/* Skip Jumbo Forms (New) */
		if((opw1v&0xFE00)==0xFE00)
		{
			if((opw3v&0xFE00)==0xFE00)
			{
				cp+=12;
				continue;
			}

			cp+=8;
			continue;
		}

		wxn1=0;
		wx1=0;
		wx3=0;
		wx5=0;

		if(	((opwn1v&0xFC00)==0xF400) ||
			((opwn1v&0xFC00)==0xFC00)	||
			((opwn1v&0xFE00)==0xEA00) ||
			((opwn1v&0xFE00)==0xEE00) ||
			((opwn1v&0xF800)==0x7800) ||
			((opwn1v&0xF800)==0x9800))
		{
			wxn1=1;
		}

		if(	((opw1v&0xFC00)==0xF400) ||
			((opw1v&0xFC00)==0xFC00)	||
			((opw1v&0xFE00)==0xEA00) ||
			((opw1v&0xFE00)==0xEE00) ||
			((opw1v&0xF800)==0x7800) ||
			((opw1v&0xF800)==0x9800))
		{
			wx1=1;
		}

		if(	((opw3v&0xFC00)==0xF400) ||
			((opw3v&0xFC00)==0xFC00)	||
			((opw3v&0xFE00)==0xEA00) ||
			((opw3v&0xFE00)==0xEE00) ||
			((opw3v&0xF800)==0x7800) ||
			((opw3v&0xF800)==0x9800))
		{
			wx3=1;
		}

		if(	((opw5v&0xFC00)==0xF400) ||
			((opw5v&0xFC00)==0xFC00)	||
			((opw5v&0xFE00)==0xEA00) ||
			((opw5v&0xFE00)==0xEE00) ||
			((opw5v&0xF800)==0x7800) ||
			((opw5v&0xF800)==0x9800))
		{
			wx5=1;
		}
		
		if(wx1)
		{
			if(wx3)
			{
				if(wx5)
				{
					cp+=8;
					continue;
				}
				cp+=12;
				continue;
			}
			cp+=8;
			continue;
		}
		
		imv1=0;
		imv3=0;
		imv5=0;
		imvn1=0;

		/* If ops are immovable, skip. */
		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw1, opw2)>0)
			{ cp+=4; continue; }
		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw3, opw4)>0)
			{ cp+=4; continue; }

		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw5, opw6)>0)
			{ imv5=1; }
//			{ opw5=0; opw6=0; }
		
		if(wxn1)
			imv1=1;
		
//		if((opw1&0xEB00)==0xE100)
		if(1)
		{
			i=BGBCC_JX2_LookupRelocAtOffs(sctx, sctx->sec, cp+0);
			if(i>=0)
				{ imv1=1; imvn1=1; }
//				{ cp+=4; continue; }
		}

//		if((opw3&0xEB00)==0xE100)
		if(1)
		{
			i=BGBCC_JX2_LookupRelocAtOffs(sctx, sctx->sec, cp+4);
			if(i>=0)
				{ imv3=1; imv5=1; }
//				{ cp+=4; continue; }
		}

		if(1)
		{
			i=BGBCC_JX2_LookupRelocAtOffs(sctx, sctx->sec, cp+8);
			if(i>=0)
			{
				imv5=1;
//				opw5=0;
//				opw6=0;
			}
		}

//		if(((opwn1&0xFE00)==0xFE00) && ((cp-spos)>=4))
		if((opwn1v&0xFE00)==0xFE00)
			{ imv1=1; }

//		if((opwn1&0xFE00)==0xFA00)
//			{ imv1=1; }
		
//		if((cp-spos)==0)
//			imv1=1;

#if 1
		i=BGBCC_JX2_LookupLabelAtOffsNoLLn(sctx, sctx->sec, cp+4);
		if(i>=0)
			{ cp+=4; continue; }
		i=BGBCC_JX2_LookupLabelAtOffsNoLLn(sctx, sctx->sec, cp+8);
		if(i>=0)
			{ imv5=1; }
#endif

#if 1
		if(	BGBCC_JX2_CheckOps32ValidWexPrefix3W(sctx, opw1, opw2) &&
			BGBCC_JX2_CheckOps32ValidWexPrefix  (sctx, opw3, opw4) &&
			BGBCC_JX2_CheckOps32ValidWexSuffix3W(sctx, opw5, opw6) &&
			!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
				opw1, opw2, opw3, opw4) &&
			!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
				opw3, opw4, opw5, opw6) &&
			!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
				opw1, opw2, opw5, opw6))
		{
			cp+=12;
			continue;
		}
#endif

		if(
//			BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw1, opw2) &&
			BGBCC_JX2_CheckOps32ValidWexPrefix2B(sctx,
				opw1, opw2, opw3, opw4) &&
			BGBCC_JX2_CheckOps32ValidWexSuffix(sctx, opw3, opw4) &&
			!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
				opw1, opw2, opw3, opw4))
		{
			cp+=8;
			continue;
		}


//		if(!BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw1, opw2))
		if(1)
		{
#if 1
			if(	BGBCC_JX2_CheckOps32ValidWexPrefix3W(sctx, opw3, opw4) &&
				BGBCC_JX2_CheckOps32ValidWexPrefix  (sctx, opw5, opw6) &&
				BGBCC_JX2_CheckOps32ValidWexSuffix3W(sctx, opw1, opw2) &&
//				!BGBCC_JX2_CheckOps32SequenceOnly(ctx, sctx,
				BGBCC_JX2_CheckCanSwapOps32(sctx, opw1, opw2, opw3, opw4) &&
				BGBCC_JX2_CheckCanSwapOps32(sctx, opw1, opw2, opw5, opw6) &&
				BGBCC_JX2_CheckCanSwapOps32(sctx, opw3, opw4, opw5, opw6) &&
				!(imv1|imv3|imv5))
			{
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 0, opw3);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 2, opw4);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 4, opw5);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 6, opw6);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 8, opw1);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+10, opw2);
				nswap++;
				continue;
			}
#endif

#if 1
			if(	BGBCC_JX2_CheckOps32ValidWexPrefix3W(sctx, opw5, opw6) &&
				BGBCC_JX2_CheckOps32ValidWexPrefix  (sctx, opw3, opw4) &&
				BGBCC_JX2_CheckOps32ValidWexSuffix3W(sctx, opw1, opw2) &&
//				!BGBCC_JX2_CheckOps32SequenceOnly(ctx, sctx,
				BGBCC_JX2_CheckCanSwapOps32(sctx, opw1, opw2, opw3, opw4) &&
				BGBCC_JX2_CheckCanSwapOps32(sctx, opw1, opw2, opw5, opw6) &&
				BGBCC_JX2_CheckCanSwapOps32(sctx, opw3, opw4, opw5, opw6) &&
				!(imv1|imv3|imv5))
			{
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 0, opw5);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 2, opw6);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 4, opw3);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 6, opw4);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 8, opw1);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+10, opw2);
				nswap++;
				continue;
			}
#endif

#if 1
			if(	
//				BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw3, opw4) &&
				BGBCC_JX2_CheckOps32ValidWexPrefix2B(sctx,
					opw3, opw4, opw1, opw2) &&
				BGBCC_JX2_CheckOps32ValidWexSuffix(sctx, opw1, opw2) &&
//				!BGBCC_JX2_CheckOps32SequenceOnly(ctx, sctx,
				BGBCC_JX2_CheckCanSwapOps32(sctx,
					opw1, opw2, opw3, opw4) &&
					!(imv1|imv3))
			{
				if((opw1v&0xFE00)==0xFE00)
					{ BGBCC_DBGBREAK }
				if((opw3v&0xFE00)==0xFE00)
					{ BGBCC_DBGBREAK }

				if(((opwn1v&0xFE00)==0xFE00) && ((cp-spos)>=4))
					{ BGBCC_DBGBREAK }

				BGBCC_JX2_EmitSetOffsWord(sctx, cp+0, opw3);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+2, opw4);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+4, opw1);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+6, opw2);
				nswap++;
				continue;
			}
#endif

//			cp+=4; continue;
		}

//		if(!BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw3, opw4) &&
//			!sctx->is_simpass && ((cp+11)<epos))
		if(1)
		{
#if 1
			if(
//				BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw5, opw6) &&
				BGBCC_JX2_CheckOps32ValidWexPrefix2B(sctx,
					opw5, opw6, opw3, opw4) &&
				BGBCC_JX2_CheckOps32ValidWexSuffix(sctx, opw3, opw4) &&
//				!BGBCC_JX2_CheckOps32SequenceOnly(ctx, sctx,
				BGBCC_JX2_CheckCanSwapOps32(sctx,
					opw3, opw4, opw5, opw6) &&
//				(	!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
//						opw1, opw2, opw5, opw6) ||
//					BGBCC_JX2_CheckOps32SequenceOnly(sctx,
//						opw1, opw2, opw3, opw4)))
				(	!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
						opw1, opw2, opw5, opw6) &&
					BGBCC_JX2_CheckOps32SequenceOnly(sctx,
						opw1, opw2, opw3, opw4)) &&
					!(imv3|imv5))
			{
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 4, opw5);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 6, opw6);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 8, opw3);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+10, opw4);
				nswap++;
				continue;
			}
#endif
		}

		cp+=4;
		continue;
	}
	
	return(nswap>0);
}
#endif

#if 1
ccxl_status BGBCC_JX2_CheckWexify_DoBundle(
	BGBCC_JX2_Context *sctx,
	int spos, int epos)
{
	int opw1, opw2, opw3, opw4, opw5, opw6;
	int opw7, opw8, opw9, opw10, opw11, opw12;
	int opw1v, opw3v, opw5v, opwn1v;
	int opwn1, opwn2, imv1, imv3, imv5;
	int dp, cp, nswap;
	int i;
	
	dp=epos-spos;
	if(dp&3)
	{
		BGBCC_DBGBREAK
		return(0);
	}

	if(dp<0)
	{
		BGBCC_DBGBREAK
		return(0);
	}

//	if(sctx->is_simpass)
//		return(0);

//	return(0);

	nswap=0;
	cp=spos;
	while((cp+7)<epos)
	{
		opw1=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 0);
		opw2=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 2);
		opw3=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 4);
		opw4=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 6);
		opw5=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 8);
		opw6=BGBCC_JX2_EmitGetOffsWord(sctx, cp+10);

		opw7=BGBCC_JX2_EmitGetOffsWord(sctx, cp+12);
		opw8=BGBCC_JX2_EmitGetOffsWord(sctx, cp+14);
		opw9=BGBCC_JX2_EmitGetOffsWord(sctx, cp+16);
		opw10=BGBCC_JX2_EmitGetOffsWord(sctx, cp+18);

		opwn1=-1;

		opw1v =opw1;		opw3v =opw3;
		opw5v =opw5;		opwn1v=opwn1;
		
		if(sctx->is_fixed32&2)
		{
			opw1v =opw1 |0xE000;
			opw3v =opw3 |0xE000;
			opw5v =opw5 |0xE000;
			opwn1v=opwn1|0xE000;
		}

		if(!((cp+11)<epos))
		{
			opw5=0;
			opw6=0;
		}

		if(!((cp+15)<epos))
		{
			opw7=0;
			opw8=0;
		}

		if(!((cp+19)<epos))
		{
			opw9=0;
			opw10=0;
		}


		if(!(sctx->is_fixed32&2))
		{
	//		if((opw1&0xE000)!=0xE000)
			if(	((opw1&0xE000)!=0xE000) &&
				((opw1&0xF000)!=0x7000) &&
				((opw1&0xF000)!=0x9000))
				{ BGBCC_DBGBREAK }
	//		if((opw3&0xE000)!=0xE000)
			if(	((opw3&0xE000)!=0xE000) &&
				((opw3&0xF000)!=0x7000) &&
				((opw3&0xF000)!=0x9000))
				{ BGBCC_DBGBREAK }

			if((cp+11)<epos)
			{
	//			if((opw5&0xE000)!=0xE000)
				if(	((opw5&0xE000)!=0xE000) &&
					((opw5&0xF000)!=0x7000) &&
					((opw5&0xF000)!=0x9000))
					{ BGBCC_DBGBREAK }
			}
		}

		/* Skip predicated ops. */
//		if((opw1&0xF000)==0xE000)
//			{ cp+=4; continue; }

		/* Skip DLR loads. */
//		if((opw1&0xFE00)==0xFA00)
//			{ cp+=4; continue; }

		/* Skip Jumbo Forms */
		if((opw1v&0xFE00)==0xFE00)
		{
			if((opw3v&0xFF00)==0xFE00)
			{
				cp+=12;
				continue;
			}

			cp+=8;
			continue;
		}

		if(	((opw1v&0xFC00)==0xF400) ||
			((opw1v&0xFC00)==0xFC00)	||
			((opw1v&0xFE00)==0xEA00) ||
			((opw1v&0xFE00)==0xEE00) ||
			((opw1v&0xF800)==0x7800) ||
			((opw1v&0xF800)==0x9800))
		{
			/* Wexify Ignores existing WEX sequences. */
			if((opw3v&0xFC00)==0xF000)
				{ cp+=8; continue; }
			if((opw3v&0xFC00)==0xF400)
				{ cp+=4; continue; }
//			if((opw3v&0xFF00)==0xF900)
			if((opw3v&0xFC00)==0xFC00)
				{ cp+=4; continue; }

			if((opw3v&0xFE00)==0xEA00)
				{ cp+=4; continue; }
			if((opw3v&0xFE00)==0xEE00)
				{ cp+=4; continue; }

			if((opw3v&0xF800)==0x7800)
				{ cp+=4; continue; }
			if((opw3v&0xF800)==0x9800)
				{ cp+=4; continue; }

			cp+=8;
			continue;
		}
		
		/* If ops are immovable, skip. */
		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw1, opw2)>0)
			{ cp+=4; continue; }
		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw3, opw4)>0)
			{ cp+=4; continue; }
		
		imv1=0;
		imv3=0;
		imv5=0;

		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw5, opw6)>0)
			imv5=1;
		
//		if(!BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw1, opw2))
		if(!BGBCC_JX2_CheckOps32ValidWexPrefix2B(sctx,
				opw1, opw2, opw3, opw4))
			{ cp+=4; continue; }

		i=BGBCC_JX2_LookupRelocAtOffs(sctx, sctx->sec, cp+0);
		if(i>=0)	{ imv1=1; }
		i=BGBCC_JX2_LookupRelocAtOffs(sctx, sctx->sec, cp+4);
		if(i>=0)	{ imv3=1; }
		i=BGBCC_JX2_LookupRelocAtOffs(sctx, sctx->sec, cp+8);
		if(i>=0)	{ imv5=1; }


		i=BGBCC_JX2_LookupLabelAtOffsNoLLn(sctx, sctx->sec, cp+0);
		if(i>=0)	{ imv1=1; }
		i=BGBCC_JX2_LookupLabelAtOffsNoLLn(sctx, sctx->sec, cp+4);
		if(i>=0)	{ imv3=1; imv5=1; }
		i=BGBCC_JX2_LookupLabelAtOffsNoLLn(sctx, sctx->sec, cp+8);
		if(i>=0)	{ imv5=1; }

#if 0
		/* If next op begins WEX sequence, skip. */
//		if(((opw3&0xFC00)==0xF400) ||
//			((opw3&0xFF00)==0xF900))
		if(	((opw3v&0xFC00)==0xF400) ||
			((opw3v&0xFC00)==0xFC00))
				{ cp+=4; continue; }

		/* If next op is a DLR load, skip. */
		if((opw3v&0xFE00)==0xFA00)
			{ cp+=4; continue; }

		/* Skip Mem Load/Store (Lane 1 only) */
		if((opw1v&0xFF00)==0xF100)
			{ cp+=4; continue; }
#endif

#if 1
		if(	BGBCC_JX2_CheckOps32ValidWexPrefix3W(sctx, opw1, opw2) &&
			BGBCC_JX2_CheckOps32ValidWexPrefix  (sctx, opw3, opw4) &&
			BGBCC_JX2_CheckOps32ValidWexSuffix3W(sctx, opw5, opw6) &&
			!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
				opw1, opw2, opw3, opw4) &&
			!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
				opw3, opw4, opw5, opw6) &&
			!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
				opw1, opw2, opw5, opw6) &&

			!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
				opw1, opw2, opw7, opw8) &&
			!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
				opw1, opw2, opw9, opw10) &&

			!(imv1|imv3|imv5))
		{
			BGBCC_JX2_AdjustWexifyOp(sctx, &opw1, &opw2);
			BGBCC_JX2_AdjustWexifyOp(sctx, &opw3, &opw4);

			BGBCC_JX2_EmitSetOffsWord(sctx, cp+0, opw1);
			BGBCC_JX2_EmitSetOffsWord(sctx, cp+2, opw2);
			BGBCC_JX2_EmitSetOffsWord(sctx, cp+4, opw3);
			BGBCC_JX2_EmitSetOffsWord(sctx, cp+6, opw4);
			cp+=12;
			continue;
		}
#endif

		if(
//			BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw1, opw2) &&
			BGBCC_JX2_CheckOps32ValidWexPrefix2B(sctx,
				opw1, opw2, opw3, opw4) &&
			BGBCC_JX2_CheckOps32ValidWexSuffix(sctx, opw3, opw4) &&
			!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
				opw1, opw2, opw3, opw4) &&
			!(imv1|imv3))
		{
			BGBCC_JX2_AdjustWexifyOp(sctx, &opw1, &opw2);

			BGBCC_JX2_EmitSetOffsWord(sctx, cp+0, opw1);
			BGBCC_JX2_EmitSetOffsWord(sctx, cp+2, opw2);
			cp+=8;
			continue;
		}

#if 0
		if(1 &&
			((opw1&0xFF00)==0xF200) &&
			((opw3&0xFF00)==0xF200) &&
			(((opw1>>4)&15)!=(opw3&15)) &&
			(((opw3>>4)&15)!=(opw1&15)) &&
			(((opw1>>4)&15)!=((opw1>>4)&15)))
		{
			printf("Bundle Fail: %04X-%04X %04X-%04X\n",
				opw1, opw2, opw3, opw4);
		}
#endif

		cp+=4;
		continue;
	}
	
	return(1);
}
#endif

#if 1
ccxl_status BGBCC_JX2_CheckWexify(
	BGBCC_JX2_Context *sctx,
	int spos, int epos)
{
	int dp;
	int i, j, k;

	if(sctx->is_simpass)
		return(0);

	dp=epos-spos;
	if(dp&3)
	{
		BGBCC_DBGBREAK
		return(0);
	}

	if(dp<0)
	{
		BGBCC_DBGBREAK
		return(0);
	}
	
	if(sctx->no_wexify)
		return(0);

//	return(0);


#if 0	
	if(dp<12)
	{
//		return(0);
	}

//	if(dp>32)
	if(dp>64)
//	if(dp>128)
//	if(spos<8192)
	{
//		printf("BGBCC_JX2_CheckWexify: %X..%X sz=%X\n", spos, epos, dp);
		printf("BGBCC_JX2_CheckWexify: %X..%X sz=%X\n",
			0x01100400+spos, 0x01100400+epos, dp);
	}
#endif

#if 1
//	for(i=0; i<16; i++)
	for(i=0; i<32; i++)
//	for(i=0; i<64; i++)
	{
		j=BGBCC_JX2_OptInterlock_DoSwaps(sctx, spos, epos);
		if(j<=0)
			break;
	}
#endif

#if 1
	for(i=0; i<16; i++)
//	for(i=0; i<32; i++)
	{
		j=BGBCC_JX2_CheckWexify_DoSwaps(sctx, spos, epos);
		if(j<=0)
			break;
	}
#endif

	BGBCC_JX2_CheckWexify_DoBundle(sctx, spos, epos);
	return(1);
}
#endif

ccxl_status BGBCC_JX2_BeginWex(
	BGBCC_JX2_Context *sctx)
{
	if(!sctx->use_wexmd)
		return(0);

	if(sctx->optmode==BGBCC_OPT_SIZE)
		return(0);

	if(sctx->no_wexify)
		return(0);

//	return(0);

	if(!(sctx->is_fixed32&16))
	{
		if(!BGBCC_JX2_CheckPadAlign32(sctx))
//		if(sctx->op_wex_align && !BGBCC_JX2_CheckPadAlign32(sctx))
		{
			BGBCC_JX2_EmitPad32AlignLastOp(sctx);
//			BGBCC_JX2_EmitWord(sctx, 0x3000);
		}

		sctx->is_fixed32|=16;
		sctx->wex_ofs_begin=BGBCC_JX2_EmitGetOffs(sctx);
		return(1);
	}
	return(0);
}

ccxl_status BGBCC_JX2_EndWex(
	BGBCC_JX2_Context *sctx)
{
	int i0, i1;

	if(sctx->is_fixed32&16)
	{
		i0=sctx->wex_ofs_begin;
		i1=BGBCC_JX2_EmitGetOffs(sctx);
		BGBCC_JX2_CheckWexify(sctx, i0, i1);
		sctx->is_fixed32&=(~16);
		return(1);
	}
	return(0);
}
