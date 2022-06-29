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
Flag:
  1&=Accesses Memory
  2&=Uses SR.T
  4&=Rn is Pair
  8&=Rs/Rt/Rn are Pairs
  
  16&=2 Stage Operation
  32&=3 Stage Operation
 */

int BGBCC_JX2_CheckOps32GetRegs(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2,
	u16 *rrs, u16 *rrt, u16 *rrn,
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

	u16 rs, rt, rn, rs2, spr, spw, spfl;
	
//	if((opw1&0xE000)!=0xE000)
	if(	((opw1&0xE000)!=0xE000) &&
		((opw1&0xF000)!=0x7000) &&
		((opw1&0xF000)!=0x9000))
		return(-1);
	
	rn=((opw1>>4)&15)|((opw2>>6)&16);
	rs=((opw1   )&15)|((opw2>>5)&16);
	rt=((opw2>>4)&15)|((opw2>>4)&16);
	
	if(	((opw1&0xF000)==0x7000) ||
		((opw1&0xF000)==0x9000))
	{
		if(opw1&0x0400)	rn+=32;
		if(opw1&0x0200)	rs+=32;
		if(opw1&0x0100)	rt+=32;
	}
	
	spr=BGBCC_SH_REG_ZZR;
	spw=BGBCC_SH_REG_ZZR;
	spfl=0;

	if((opw1&0xF000)==0xE000)
	{
		spr=BGBCC_SH_REG_SR;
//		spw=BGBCC_SH_REG_SR;
		spfl|=2;
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
			spfl|=16;
			
			if((opw2&0x000E)==0x0002)
				{ spfl&=~16; spfl|=32; }
		}

		if((opw2&0xF008)==0x1008)
		{
			rt=rn;
//			rs2=rn;

//			return(-1);

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
					spw=gr2cr[rn];
//					rt=BGBCC_SH_REG_ZZR;
					break;
				case 0xB:
					spr=gr2cr[rs];
//					rt=BGBCC_SH_REG_ZZR;
					break;

				case 0x8:
					rt=BGBCC_SH_REG_ZZR;
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
					spfl|=8;
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
					spfl|=8;
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
			spr=BGBCC_SH_REG_SR;
			spw=BGBCC_SH_REG_SR;

			if(	((opw2&0x000C)==0x0000) ||
				((opw2&0x000C)==0x0008))
			{
				if(opw2&0x0800)
				{
					spfl|=8;
				}
			}
			
			if((opw2&0x000C)==0x000C)
			{
				spfl|=16;
			}

			if((opw2&0x000E)==0x0002)
			{
				spfl|=32;
			}
		}

		if((opw2&0xF000)==0x6000)
		{
			if(opw2&0x0800)
			{
				spfl|=8;
			}
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
				((opw2&0xF008)==0x8000))
			{
				spr=rn;
//				return(-1);
			}
		
			spfl|=1;

			spfl|=32;
			
			if((opw2&0xF000)==0x4000)
			{
				spfl|=4;
			}

			if((opw2&0xF000)==0x8000)
			{
				spfl|=4;
				spfl|=8;
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
//					rs=BGBCC_SH_REG_TBR; rt=0;
					rt=0;
				}
//				return(-1);
			}

		}

		*rrs=rs;
		*rrt=rt;
		*rrn=rn;
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

		spfl|=1;

		spfl|=32;
		
		if((opw2&0xC800)==0x0800)
			spfl&=~1;

		if((opw2&0xF000)==0x5000)
			spfl|=4;
		if((opw2&0xF000)==0x7000)
			spfl|=4;

//		return(-1);

		if(rs==0)
			{ rs=BGBCC_SH_REG_PC; }
		if(rs==1)
			{ rs=BGBCC_SH_REG_GBR; }

		if(	((opw2&0x8800)==0x0000) ||
			((opw2&0xE000)==0x4000)	)
			{ spr=rn; }

		*rrs=rs;
//		*rrt=BGBCC_SH_REG_ZZR;
		*rrt=spr;
		*rrn=rn;

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
			{ spfl|=16; }

		if((opw2&0xE000)==0x2000)
			{ spfl|=32; }

		if((opw2&0xC000)==0x4000)
			{ spfl|=16; }

		if((opw2&0xF000)>=0xA000)
		{
			rs=rn;
		}

		if((opw2&0xF000)==0xC000)
		{
			rs=rn;

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
				spfl|=16;
			}
		}

		*rrs=rs;
		*rrt=BGBCC_SH_REG_ZZR;
		*rrn=rn;
		*rspr=spr;
		*rspw=spw;
		*rspfl=spfl;
		return(1);
	}

//	return(-1);

//	if((opw1&0xEE00)==0xE800)
	if((opw1&0xEB00)==0xE800)
	{
		rn=opw1&0x001F;
		rs=rn;
		rt=BGBCC_SH_REG_ZZR;

		if((opw1&0x00F0)<0x0040)
		{
			rs=BGBCC_SH_REG_ZZR;
		}

		if((opw1&0x00E0)==0x0040)
		{
			spfl|=16;
		}

		*rrs=rs;
		*rrt=rt;
		*rrn=rn;
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
		*rrs=rn;
		*rrt=BGBCC_SH_REG_ZZR;
		*rrn=rn;
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

	return(0);
}

int BGBCC_JX2_CheckOps32IsMem(
	BGBCC_JX2_Context *sctx, int opw1, int opw2)
{
	u16 rs1, rt1, rn1, rm1, rspw1, rs1b, rt1b, rn1b, rm1b;
	int rspfl1, rspfl2;
	int ret1, ret2;

	ret1=BGBCC_JX2_CheckOps32GetRegs(sctx, opw1, opw2,
		&rs1, &rt1, &rn1, &rm1, &rspw1, &rspfl1);
		
	if(rspfl1&1)
		return(1);
	return(0);
}

int BGBCC_JX2_CheckOps32Is2Stage(
	BGBCC_JX2_Context *sctx, int opw1, int opw2)
{
	u16 rs1, rt1, rn1, rm1, rspw1, rs1b, rt1b, rn1b, rm1b;
	int rspfl1, rspfl2;
	int ret1, ret2;

	ret1=BGBCC_JX2_CheckOps32GetRegs(sctx, opw1, opw2,
		&rs1, &rt1, &rn1, &rm1, &rspw1, &rspfl1);
		
	if(rspfl1&16)
		return(1);
	return(0);
}


int BGBCC_JX2_CheckOps32Is3Stage(
	BGBCC_JX2_Context *sctx, int opw1, int opw2)
{
	u16 rs1, rt1, rn1, rm1, rspw1, rs1b, rt1b, rn1b, rm1b;
	int rspfl1, rspfl2;
	int ret1, ret2;

	ret1=BGBCC_JX2_CheckOps32GetRegs(sctx, opw1, opw2,
		&rs1, &rt1, &rn1, &rm1, &rspw1, &rspfl1);
		
	if(rspfl1&32)
		return(1);
	return(0);
}

int BGBCC_JX2_CheckOps32SequenceOnlyB(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2, int opw3, int opw4, int fl)
{
	u16 rs1, rt1, rn1, rm1, rspw1, rs1b, rt1b, rn1b, rm1b;
	u16 rs2, rt2, rn2, rm2, rspw2, rs2b, rt2b, rn2b, rm2b;
	int rspfl1, rspfl2;
	int ret1, ret2;

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
		&rs1, &rt1, &rn1, &rm1, &rspw1, &rspfl1);
	ret2=BGBCC_JX2_CheckOps32GetRegs(sctx, opw3, opw4,
		&rs2, &rt2, &rn2, &rm2, &rspw2, &rspfl2);

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
		
//	if((rspfl1&1) && (rspfl2&1))
	if((rspfl1&1) && (rspfl2&1) &&
		((rm1!=BGBCC_SH_REG_ZZR) || (rm2!=BGBCC_SH_REG_ZZR)))
		return(1);

//	if((rspfl1&1) && (rspfl2&1) &&
//		((rm1==BGBCC_SH_REG_ZZR) || (rm2==BGBCC_SH_REG_ZZR)))
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
	rn1b=BGBCC_SH_REG_ZZR;	rn2b=BGBCC_SH_REG_ZZR;
	rm1b=BGBCC_SH_REG_ZZR;	rm2b=BGBCC_SH_REG_ZZR;
	if(rspfl1&8)
	{
//		rs1b=rs1+1; rt1b=rt1+1; rn1b=rn1+1;
		if(rs1!=BGBCC_SH_REG_ZZR)
			{ rs1b=rs1+1; }
		if(rt1!=BGBCC_SH_REG_ZZR)
			{ rt1b=rt1+1; }
		if(rn1!=BGBCC_SH_REG_ZZR)
			{ rn1b=rn1+1; }
		if(rm1!=BGBCC_SH_REG_ZZR)
			{ rm1b=rm1+1; }
	}
	if(rspfl2&8)
	{
//		rs2b=rs2+1; rt2b=rt2+1; rn2b=rn2+1;
		if(rs2!=BGBCC_SH_REG_ZZR)
			{ rs2b=rs2+1; }
		if(rt2!=BGBCC_SH_REG_ZZR)
			{ rt2b=rt2+1; }
		if(rn2!=BGBCC_SH_REG_ZZR)
			{ rn2b=rn2+1; }
		if(rm2!=BGBCC_SH_REG_ZZR)
			{ rm2b=rm2+1; }
	}
	
	if(rspfl1&4)
	{
		if(rn1!=BGBCC_SH_REG_ZZR)
			{ rn1b=rn1+1; }
		if(rm1!=BGBCC_SH_REG_ZZR)
			{ rm1b=rm1+1; }
	}

	if(rspfl2&4)
	{
		if(rn2!=BGBCC_SH_REG_ZZR)
			{ rn2b=rn2+1; }		
		if(rm2!=BGBCC_SH_REG_ZZR)
			{ rm2b=rm2+1; }
	}

	if(rn1!=BGBCC_SH_REG_ZZR)
	{
		if((rn2==rn1) || (rs2==rn1) || (rt2==rn1) || (rm2==rn1))
			return(1);
		if((rn2b==rn1) || (rs2b==rn1) || (rt2b==rn1) || (rm2b==rn1))
			return(1);
	}
	if(rn1b!=BGBCC_SH_REG_ZZR)
	{
		if((rn2==rn1b) || (rs2==rn1b) || (rt2==rn1b) || (rm2==rn1b))
			return(1);
		if((rn2b==rn1b) || (rs2b==rn1b) || (rt2b==rn1b) || (rm2b==rn1b))
			return(1);
	}

	if(rspw1!=BGBCC_SH_REG_ZZR)
	{
		if(	(rm2==rspw1)	||	(rn2==rspw1) ||
			(rs2==rspw1)	|| 	(rt2==rspw1))
				return(1);
		if(	(rn2b==rspw1)	|| (rs2b==rspw1) ||
			(rt2b==rspw1)	|| (rm2b==rspw1))
			return(1);
	}

	if(fl&1)
//	if((fl&1) || BGBCC_JX2_CheckOps32ReadsRn(sctx, opw3, opw4))
	{
//		if((opw1&0xFE00)==0xF800)
//			return(1);
//		if((opw3&0xFE00)==0xF800)
//			return(1);
	
//		return(1);

		if(	((rspfl1&1) && (rm1!=BGBCC_SH_REG_ZZR)) ||
			((rspfl2&1) && (rm2!=BGBCC_SH_REG_ZZR)))
				return(1);

		if(rn2!=BGBCC_SH_REG_ZZR)
		{
			if(	(rn1==rn2) || (rs1==rn2) ||
				(rt1==rn2) || (rm1==rn2))
					return(1);
			if(	(rn1b==rn2) || (rs1b==rn2) ||
				(rt1b==rn2) || (rm1b==rn2))
					return(1);
		}
		if(rn2b!=BGBCC_SH_REG_ZZR)
		{
			if(	(rn1==rn2b) || (rs1==rn2b) ||
				(rt1==rn2b) || (rm1==rn2b))
					return(1);
			if(	(rn1b==rn2b) || (rs1b==rn2b) ||
				(rt1b==rn2b) || (rm1b==rn2b))
					return(1);
		}

		if(rspw2!=BGBCC_SH_REG_ZZR)
		{
			if(	(rm1==rspw2)	||	(rn1==rspw2)	||
				(rs1==rspw2)	||	(rt1==rspw2))
					return(1);
			if(	(rn1b==rspw2)	|| (rs1b==rspw2)	||
				(rt1b==rspw2)	|| (rm1b==rspw2))
					return(1);
		}
	}

	return(0);
}


int BGBCC_JX2_InferOps32Interlock(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2, int opw3, int opw4, int opw5, int opw6, int fl)
{
	u16 rs1, rt1, rn1, rm1, rspw1, rs1b, rt1b, rn1b, rm1b;
	u16 rs2, rt2, rn2, rm2, rspw2, rs2b, rt2b, rn2b, rm2b;
	u16 rs3, rt3, rn3, rm3, rspw3, rs3b, rt3b, rn3b, rm3b;
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
		&rs1, &rt1, &rn1, &rm1, &rspw1, &rspfl1);
	ret2=BGBCC_JX2_CheckOps32GetRegs(sctx, opw3, opw4,
		&rs2, &rt2, &rn2, &rm2, &rspw2, &rspfl2);
	ret3=BGBCC_JX2_CheckOps32GetRegs(sctx, opw5, opw6,
		&rs3, &rt3, &rn3, &rm3, &rspw3, &rspfl3);

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

	rs1b=BGBCC_SH_REG_ZZR;	rs2b=BGBCC_SH_REG_ZZR;	rs3b=BGBCC_SH_REG_ZZR;
	rt1b=BGBCC_SH_REG_ZZR;	rt2b=BGBCC_SH_REG_ZZR;	rt3b=BGBCC_SH_REG_ZZR;
	rn1b=BGBCC_SH_REG_ZZR;	rn2b=BGBCC_SH_REG_ZZR;	rn3b=BGBCC_SH_REG_ZZR;
	rm1b=BGBCC_SH_REG_ZZR;	rm2b=BGBCC_SH_REG_ZZR;	rm3b=BGBCC_SH_REG_ZZR;
	if(rspfl1&8)
	{
		if(rs1!=BGBCC_SH_REG_ZZR)
			{ rs1b=rs1+1; }
		if(rt1!=BGBCC_SH_REG_ZZR)
			{ rt1b=rt1+1; }
		if(rn1!=BGBCC_SH_REG_ZZR)
			{ rn1b=rn1+1; }
		if(rm1!=BGBCC_SH_REG_ZZR)
			{ rm1b=rm1+1; }
	}
	if(rspfl2&8)
	{
		if(rs2!=BGBCC_SH_REG_ZZR)
			{ rs2b=rs2+1; }
		if(rt2!=BGBCC_SH_REG_ZZR)
			{ rt2b=rt2+1; }
		if(rn2!=BGBCC_SH_REG_ZZR)
			{ rn2b=rn2+1; }
		if(rm2!=BGBCC_SH_REG_ZZR)
			{ rm2b=rm2+1; }
	}
	if(rspfl3&8)
	{
		if(rs3!=BGBCC_SH_REG_ZZR)
			{ rs3b=rs3+1; }
		if(rt3!=BGBCC_SH_REG_ZZR)
			{ rt3b=rt3+1; }
		if(rn3!=BGBCC_SH_REG_ZZR)
			{ rn3b=rn3+1; }
		if(rm3!=BGBCC_SH_REG_ZZR)
			{ rm3b=rm3+1; }
	}
	
	if(rspfl1&4)
	{
		if(rn1!=BGBCC_SH_REG_ZZR)
			{ rn1b=rn1+1; }
		if(rm1!=BGBCC_SH_REG_ZZR)
			{ rm1b=rm1+1; }
	}

	if(rspfl2&4)
	{
		if(rn2!=BGBCC_SH_REG_ZZR)
			{ rn2b=rn2+1; }		
		if(rm2!=BGBCC_SH_REG_ZZR)
			{ rm2b=rm2+1; }
	}

	if(rspfl3&4)
	{
		if(rn3!=BGBCC_SH_REG_ZZR)
			{ rn3b=rn3+1; }		
		if(rm3!=BGBCC_SH_REG_ZZR)
			{ rm3b=rm3+1; }
	}

	if(rn1!=BGBCC_SH_REG_ZZR)
	{
		if((rn2==rn1) || (rs2==rn1) || (rt2==rn1) || (rm2==rn1))
			return(3);
		if((rn2b==rn1) || (rs2b==rn1) || (rt2b==rn1) || (rm2b==rn1))
			return(3);

		if((rn3==rn1) || (rs3==rn1) || (rt3==rn1) || (rm3==rn1))
			return(2);
		if((rn3b==rn1) || (rs3b==rn1) || (rt3b==rn1) || (rm3b==rn1))
			return(2);
	}
	if(rn1b!=BGBCC_SH_REG_ZZR)
	{
		if((rn2==rn1b) || (rs2==rn1b) || (rt2==rn1b) || (rm2==rn1b))
			return(3);
		if((rn2b==rn1b) || (rs2b==rn1b) || (rt2b==rn1b) || (rm2b==rn1b))
			return(3);

		if((rn3==rn1b) || (rs3==rn1b) || (rt3==rn1b) || (rm3==rn1b))
			return(2);
		if((rn3b==rn1b) || (rs3b==rn1b) || (rt3b==rn1b) || (rm3b==rn1b))
			return(2);
	}

	if(rspw1!=BGBCC_SH_REG_ZZR)
	{
		if(	(rm2==rspw1)	||	(rn2==rspw1) ||
			(rs2==rspw1)	|| 	(rt2==rspw1))
				return(3);
		if(	(rn2b==rspw1)	|| (rs2b==rspw1) ||
			(rt2b==rspw1)	|| (rm2b==rspw1))
			return(3);

		if(	(rm3==rspw1)	||	(rn3==rspw1) ||
			(rs3==rspw1)	|| 	(rt3==rspw1))
				return(2);
		if(	(rn3b==rspw1)	|| (rs3b==rspw1) ||
			(rt3b==rspw1)	|| (rm3b==rspw1))
			return(2);
	}

	return(1);
}


int BGBCC_JX2_CheckOps32SequenceOnly(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2, int opw3, int opw4)
{
	int rt;
	
	rt=BGBCC_JX2_CheckOps32SequenceOnlyB(sctx,
		opw1, opw2, opw3, opw4, 1);
	if(rt!=0)
		return(1);
	return(0);
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
	if(	((opw1&0xE000)!=0xE000) &&
		((opw1&0xF000)!=0x7000) &&
		((opw1&0xF000)!=0x9000)	)
		return(1);

	if((opw1&0xFE00)==0xFA00)
		return(1);
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


int BGBCC_JX2_CheckCanSwapOps32(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2, int opw3, int opw4)
{
	int ret;
	
	if(BGBCC_JX2_CheckOps32Immovable(sctx, opw1, opw2))
		return(0);
	if(BGBCC_JX2_CheckOps32Immovable(sctx, opw3, opw4))
		return(0);
	
	ret=BGBCC_JX2_CheckOps32SequenceOnlyB(sctx,
		opw1, opw2, opw3, opw4, 1);
		
	if(ret<0)
		return(0);
	return(!ret);
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

	if(	((opw1&0xE000)!=0xE000) &&
		((opw1&0xF000)!=0x7000) &&
		((opw1&0xF000)!=0x9000)	)
		return(0);

	if((opw1&0xFE00)==0xFE00)
		return(0);

	if((opw1&0xFE00)==0xFA00)
		return(0);


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

	return(1);
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
//	if((rn==0) || (rn==1) || (rn==15))
	if((rn==1) || (rn==15))
	{
		/* Disallow Rn to be DLR, DHR, or SP, in WEX'ed ops */
		return(0);
	}

	if(	((opw1&0xF000)!=0xF000) &&
		((opw1&0xF000)!=0xE000) &&
		((opw1&0xF000)!=0x7000) &&
		((opw1&0xF000)!=0x9000))
			return(0);

	if((opw1&0xF000)==0xE000)
	{
		if(	((opw1&0xFB00)!=0xE000) &&
			((opw1&0xFB00)!=0xE200) )
				return(0);
	}

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
				break;

			case 0x9:
				switch((opw2>>4)&15)
				{
				case 0x0:	case 0x1:
				case 0x5:	case 0x6:
				case 0x7:	case 0x8:
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
		case 0x6:	case 0x7:
		case 0x8:	case 0x9:
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
	
		ret=0;
		switch((opw1>>4)&15)
		{
		case 0:		case 1:		ret=1;	break;
		case 2:		case 3:		ret=1;	break;
		case 4:		case 5:		ret=1;	break;
		case 6:		case 7:		ret=1;	break;
		case 8:		case 9:		ret=0;	break;
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
	int opw1, opw2;

	if(sctx->is_simpass)
		return(0);

	opw1=*ropw1;
	opw2=*ropw2;

	if((opw1&0xFF00)==0xF000)
	{
		if((opw2&0xF80F)==0x0804)
		{
			/* If LEA.B, Transform into ADD */
			opw2=0x1000 | (opw2&0x07F0);
		}
	}

	if((opw1&0xFF00)==0xF100)
	{
		if((opw2&0xC800)==0x0800)
		{
			sctx->opcnt_hi8[0xF1]--;
			sctx->opcnt_hi8[0xF2]++;

			/* If LEA, Transform into ADD */
			if((opw2&0xF000)==0x0000)
			{
				opw1=0xF200|(opw1&0x00FF);
				opw2=0x0000|(opw2&0x07FF);
			}
			else
				if((opw2&0xF000)==0x1000)
			{
				opw1=0xF200|(opw1&0x00FF);
				opw2=0x0000|(opw2&0x0600)|
					((opw2&0x00FF)<<1);
			}else
				if((opw2&0xF000)==0x2000)
			{
				opw1=0xF200|(opw1&0x00FF);
				opw2=0x0000|(opw2&0x0600)|
					((opw2&0x007F)<<2);
			}else
				if((opw2&0xF000)==0x3000)
			{
				opw1=0xF200|(opw1&0x00FF);
				opw2=0x0000|(opw2&0x0600)|
					((opw2&0x003F)<<3);
			}
		}
	}
	
//	if((opw1&0xFF00)==0xF800)
	if((opw1&0xFE00)==0xF800)
	{
		sctx->opcnt_hi8[(opw1>>8)&0xFF]--;
//		opw1|=0x0100;
		opw1|=0x0400;
		sctx->opcnt_hi8[(opw1>>8)&0xFF]++;
		*ropw1=opw1;
		*ropw2=opw2;
		return(1);
	}

	if((opw1&0xFC00)==0xF000)
	{
		sctx->opcnt_hi8[(opw1>>8)&0xFF]--;
		opw1|=0x0400;
		sctx->opcnt_hi8[(opw1>>8)&0xFF]++;
		*ropw1=opw1;
		*ropw2=opw2;
		return(1);
	}

	if(	((opw1&0xF800)==0x7000) ||
		((opw1&0xF800)==0x9000))
	{
		sctx->opcnt_hi8[(opw1>>8)&0xFF]--;
		opw1|=0x0800;
		sctx->opcnt_hi8[(opw1>>8)&0xFF]++;
		*ropw1=opw1;
		*ropw2=opw2;
		return(1);
	}

	if((opw1&0xFB00)==0xE000)
	{
		sctx->opcnt_hi8[(opw1>>8)&0xFF]--;
		opw1|=0x0A00;
		sctx->opcnt_hi8[(opw1>>8)&0xFF]++;
		*ropw1=opw1;
		*ropw2=opw2;
		return(1);
	}

	if((opw1&0xFB00)==0xE200)
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
	int opw1, int opw2, int opw3, int opw4,
	int opw5, int opw6, int opw7, int opw8)
{
	int cf1, cf2;
	int c1, c2;

	cf1=BGBCC_JX2_InferOps32Interlock(sctx,
		opw1, opw2, opw3, opw4, opw5, opw6, 0);
	cf2=BGBCC_JX2_InferOps32Interlock(sctx,
		opw3, opw4, opw5, opw6, opw7, opw8, 0);
	
	c1=1;
	c2=1;

	if(	BGBCC_JX2_CheckOps32IsMem(sctx, opw1, opw2) ||
		BGBCC_JX2_CheckOps32Is3Stage(sctx, opw1, opw2))
	{
		c1=cf1;
	}else if(BGBCC_JX2_CheckOps32Is2Stage(sctx, opw1, opw2))
	{
		c1=cf1-1;
		if(c1<1)
			c1=1;
	}

	if(	BGBCC_JX2_CheckOps32IsMem(sctx, opw3, opw4) ||
		BGBCC_JX2_CheckOps32Is3Stage(sctx, opw3, opw4))
	{
		c2=cf2;
	}else if(BGBCC_JX2_CheckOps32Is2Stage(sctx, opw3, opw4))
	{
		c2=cf2-1;
		if(c2<1)
			c2=1;
	}
	
	if(cf1<=0)
		c1=999;
	if(cf2<=0)
		c2=999;
	return(c1+c2);
}

ccxl_status BGBCC_JX2_OptInterlock_DoSwaps(
	BGBCC_JX2_Context *sctx,
	int spos, int epos)
{
	int opw1, opw2, opw3, opw4, opw5, opw6, opw7, opw8;
	int opwn1, opwn2;
	int wx1, wx3, wx5;
	int dp, cp, nswap;
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
	
	nswap=0;
	cp=spos;
//	while((cp+7)<epos)
//	while((cp+11)<epos)
	while((cp+15)<epos)
	{
		opw1=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 0);
		opw2=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 2);
		opw3=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 4);
		opw4=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 6);
		opw5=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 8);
		opw6=BGBCC_JX2_EmitGetOffsWord(sctx, cp+10);
		opw7=BGBCC_JX2_EmitGetOffsWord(sctx, cp+12);
		opw8=BGBCC_JX2_EmitGetOffsWord(sctx, cp+14);
		
		opwn1=BGBCC_JX2_EmitGetOffsWord(sctx, cp- 4);
		opwn2=BGBCC_JX2_EmitGetOffsWord(sctx, cp- 2);
		
//		if(!((cp+11)<epos))
//		{
//			opw5=0;
//			opw6=0;
//		}

		if(!((cp+15)<epos))
		{
			opw7=0;
			opw8=0;
		}

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

		if((cp<spos) || (cp>=epos))
			{ BGBCC_DBGBREAK }

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

		if((opw1&0xFE00)==0xFA00)
		{
			cp+=8;
			continue;
		}

		/* Skip Jumbo Forms */
		if((opw1&0xFE00)==0xFE00)
		{
			if((opw3&0xFE00)==0xFE00)
			{
				cp+=12;
				continue;
			}

			cp+=8;
			continue;
		}

		wx1=0;
		wx3=0;
		wx5=0;

		if(	((opw1&0xFC00)==0xF400) ||
			((opw1&0xFC00)==0xFC00)	||
			((opw1&0xFE00)==0xEA00) ||
			((opw1&0xFE00)==0xEE00) ||
			((opw1&0xF800)==0x7800) ||
			((opw1&0xF800)==0x9800))
		{
			wx1=1;
		}

		if(	((opw3&0xFC00)==0xF400) ||
			((opw3&0xFC00)==0xFC00)	||
			((opw3&0xFE00)==0xEA00) ||
			((opw3&0xFE00)==0xEE00) ||
			((opw3&0xF800)==0x7800) ||
			((opw3&0xF800)==0x9800))
		{
			wx3=1;
		}

		if(	((opw5&0xFC00)==0xF400) ||
			((opw5&0xFC00)==0xFC00)	||
			((opw5&0xFE00)==0xEA00) ||
			((opw5&0xFE00)==0xEE00) ||
			((opw5&0xF800)==0x7800) ||
			((opw5&0xF800)==0x9800))
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
		
		/* If ops are immovable, skip. */
		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw1, opw2)>0)
			{ cp+=4; continue; }
		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw3, opw4)>0)
			{ cp+=4; continue; }
		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw5, opw6)>0)
			{ cp+=4; continue; }
		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw7, opw8)>0)
			{ cp+=4; continue; }

		if((opw7&0xFE00)==0xFE00)
			{ opw7=0; }
		if((opw5&0xFE00)==0xFE00)
			{ opw5=0; opw7=0; }
		if((opw3&0xFE00)==0xFE00)
			{ opw3=0; opw5=0; }

#if 0
//		if((opw1&0xEB00)==0xE100)
//		{
//			i=BGBCC_JX2_LookupRelocAtOffs(sctx, sctx->sec, cp+0);
//			if(i>=0)
//				{ cp+=4; continue; }
//		}

		if((opw3&0xEB00)==0xE100)
		{
			i=BGBCC_JX2_LookupRelocAtOffs(sctx, sctx->sec, cp+4);
			if(i>=0)
				opw3=0;
		}

		if((opw5&0xEB00)==0xE100)
		{
			i=BGBCC_JX2_LookupRelocAtOffs(sctx, sctx->sec, cp+8);
			if(i>=0)
				opw5=0;
		}

		if((opw7&0xEB00)==0xE100)
		{
			i=BGBCC_JX2_LookupRelocAtOffs(sctx, sctx->sec, cp+12);
			if(i>=0)
				opw7=0;
		}
#endif

#if 1
		i=BGBCC_JX2_LookupRelocAtOffs(sctx, sctx->sec, cp+0);
		if(i>=0)
			{ cp+=4; continue; }
		i=BGBCC_JX2_LookupRelocAtOffs(sctx, sctx->sec, cp+4);
		if(i>=0)
			{ cp+=4; continue; }
		i=BGBCC_JX2_LookupRelocAtOffs(sctx, sctx->sec, cp+8);
		if(i>=0)
			{ cp+=4; continue; }
		i=BGBCC_JX2_LookupRelocAtOffs(sctx, sctx->sec, cp+12);
		if(i>=0)
			{ cp+=4; continue; }
#endif


#if 1
//		if(	!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
//				opw1, opw2, opw3, opw4) &&
//			!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
//				opw3, opw4, opw5, opw6) &&
//			!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
//				opw1, opw2, opw5, opw6))

		if(	opw3 && opw5 &&
			BGBCC_JX2_CheckCanSwapOps32(sctx,
				opw3, opw4, opw5, opw6))
		{
			if(	BGBCC_JX2_InferInterlockCost(sctx,
					opw1, opw2, opw5, opw6, opw3, opw4, opw7, opw8) <
				BGBCC_JX2_InferInterlockCost(sctx,
					opw1, opw2, opw3, opw4, opw5, opw6, opw7, opw8))
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
				opw5, opw6, opw7, opw8))
		{
			if(	BGBCC_JX2_InferInterlockCost(sctx,
					opw1, opw2, opw3, opw4, opw7, opw8, opw5, opw6) <
				BGBCC_JX2_InferInterlockCost(sctx,
					opw1, opw2, opw3, opw4, opw5, opw6, opw7, opw8))
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
				opw5, opw6, opw7, opw8) &&
			BGBCC_JX2_CheckCanSwapOps32(sctx,
				opw3, opw4, opw7, opw8)	)
		{
			if(	BGBCC_JX2_InferInterlockCost(sctx,
					opw1, opw2, opw7, opw8, opw3, opw4, opw5, opw6) <
				BGBCC_JX2_InferInterlockCost(sctx,
					opw1, opw2, opw3, opw4, opw5, opw6, opw7, opw8))
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
					opw1, opw2, opw5, opw6, opw7, opw8, opw3, opw4) <
				BGBCC_JX2_InferInterlockCost(sctx,
					opw1, opw2, opw3, opw4, opw5, opw6, opw7, opw8))
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

		cp+=4;
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
	int opwn1, opwn2;
	int wx1, wx3, wx5;
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
		
		if(!((cp+11)<epos))
		{
			opw5=0;
			opw6=0;
		}

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

		if((cp<spos) || (cp>=epos))
			{ BGBCC_DBGBREAK }

		if((cp+11)<epos)
		{
//			if((opw5&0xE000)!=0xE000)
			if(	((opw5&0xE000)!=0xE000) &&
				((opw5&0xF000)!=0x7000) &&
				((opw5&0xF000)!=0x9000))
				{ BGBCC_DBGBREAK }
		}

		/* Skip Jumbo Forms (New) */
		if((opw1&0xFE00)==0xFE00)
		{
			if((opw3&0xFE00)==0xFE00)
			{
				cp+=12;
				continue;
			}

			cp+=8;
			continue;
		}

		wx1=0;
		wx3=0;
		wx5=0;

		if(	((opw1&0xFC00)==0xF400) ||
			((opw1&0xFC00)==0xFC00)	||
			((opw1&0xFE00)==0xEA00) ||
			((opw1&0xFE00)==0xEE00) ||
			((opw1&0xF800)==0x7800) ||
			((opw1&0xF800)==0x9800))
		{
			wx1=1;
		}

		if(	((opw3&0xFC00)==0xF400) ||
			((opw3&0xFC00)==0xFC00)	||
			((opw3&0xFE00)==0xEA00) ||
			((opw3&0xFE00)==0xEE00) ||
			((opw3&0xF800)==0x7800) ||
			((opw3&0xF800)==0x9800))
		{
			wx3=1;
		}

		if(	((opw5&0xFC00)==0xF400) ||
			((opw5&0xFC00)==0xFC00)	||
			((opw5&0xFE00)==0xEA00) ||
			((opw5&0xFE00)==0xEE00) ||
			((opw5&0xF800)==0x7800) ||
			((opw5&0xF800)==0x9800))
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
		
		/* If ops are immovable, skip. */
		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw1, opw2)>0)
			{ cp+=4; continue; }
		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw3, opw4)>0)
			{ cp+=4; continue; }
		
		if((opw1&0xEB00)==0xE100)
		{
			i=BGBCC_JX2_LookupRelocAtOffs(sctx, sctx->sec, cp+0);
			if(i>=0)
				{ cp+=4; continue; }
		}

		if((opw3&0xEB00)==0xE100)
		{
			i=BGBCC_JX2_LookupRelocAtOffs(sctx, sctx->sec, cp+4);
			if(i>=0)
				{ cp+=4; continue; }
		}

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

		if(	BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw1, opw2) &&
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
			if(	BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw3, opw4) &&
				BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw5, opw6) &&
				BGBCC_JX2_CheckOps32ValidWexSuffix(sctx, opw1, opw2) &&
//				!BGBCC_JX2_CheckOps32SequenceOnly(ctx, sctx,
				BGBCC_JX2_CheckCanSwapOps32(sctx, opw1, opw2, opw3, opw4) &&
				BGBCC_JX2_CheckCanSwapOps32(sctx, opw1, opw2, opw5, opw6))
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
			if(	BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw3, opw4) &&
				BGBCC_JX2_CheckOps32ValidWexSuffix(sctx, opw1, opw2) &&
//				!BGBCC_JX2_CheckOps32SequenceOnly(ctx, sctx,
				BGBCC_JX2_CheckCanSwapOps32(sctx,
					opw1, opw2, opw3, opw4))
			{
				if((opw1&0xFE00)==0xFE00)
					{ BGBCC_DBGBREAK }
				if((opw3&0xFE00)==0xFE00)
					{ BGBCC_DBGBREAK }

				if(((opwn1&0xFE00)==0xFE00) && ((cp-spos)>=4))
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
			if(	BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw5, opw6) &&
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
						opw1, opw2, opw3, opw4)))
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
	int opwn1, opwn2;
	int dp, cp, nswap;
	
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

		if(!((cp+11)<epos))
		{
			opw5=0;
			opw6=0;
		}

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

		/* Skip predicated ops. */
		if((opw1&0xF000)==0xE000)
			{ cp+=4; continue; }

		/* Skip DLR loads. */
		if((opw1&0xFE00)==0xFA00)
			{ cp+=4; continue; }

		/* Skip Jumbo Forms */
		if((opw1&0xFE00)==0xFE00)
		{
			if((opw3&0xFF00)==0xFE00)
			{
				cp+=12;
				continue;
			}

			cp+=8;
			continue;
		}

		if(	((opw1&0xFC00)==0xF400) ||
			((opw1&0xFC00)==0xFC00)	||
			((opw1&0xFE00)==0xEA00) ||
			((opw1&0xFE00)==0xEE00) ||
			((opw1&0xF800)==0x7800) ||
			((opw1&0xF800)==0x9800))
		{
			/* Wexify Ignores existing WEX sequences. */
			if((opw3&0xFC00)==0xF000)
				{ cp+=8; continue; }
			if((opw3&0xFC00)==0xF400)
				{ cp+=4; continue; }
//			if((opw3&0xFF00)==0xF900)
			if((opw3&0xFC00)==0xFC00)
				{ cp+=4; continue; }

			if((opw3&0xFE00)==0xEA00)
				{ cp+=4; continue; }
			if((opw3&0xFE00)==0xEE00)
				{ cp+=4; continue; }

			if((opw3&0xF800)==0x7800)
				{ cp+=4; continue; }
			if((opw3&0xF800)==0x9800)
				{ cp+=4; continue; }

			cp+=8;
			continue;
		}
		
		/* If ops are immovable, skip. */
		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw1, opw2)>0)
			{ cp+=4; continue; }
		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw3, opw4)>0)
			{ cp+=4; continue; }
		
		if(!BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw1, opw2))
			{ cp+=4; continue; }

#if 0
		/* If next op begins WEX sequence, skip. */
//		if(((opw3&0xFC00)==0xF400) ||
//			((opw3&0xFF00)==0xF900))
		if(	((opw3&0xFC00)==0xF400) ||
			((opw3&0xFC00)==0xFC00))
				{ cp+=4; continue; }

		/* If next op is a DLR load, skip. */
		if((opw3&0xFE00)==0xFA00)
			{ cp+=4; continue; }

		/* Skip Mem Load/Store (Lane 1 only) */
		if((opw1&0xFF00)==0xF100)
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
				opw1, opw2, opw5, opw6))
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

		if(	BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw1, opw2) &&
			BGBCC_JX2_CheckOps32ValidWexSuffix(sctx, opw3, opw4) &&
			!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
				opw1, opw2, opw3, opw4))
		{
			BGBCC_JX2_AdjustWexifyOp(sctx, &opw1, &opw2);

			BGBCC_JX2_EmitSetOffsWord(sctx, cp+0, opw1);
			BGBCC_JX2_EmitSetOffsWord(sctx, cp+2, opw2);
			cp+=8;
			continue;
		}

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
	for(i=0; i<16; i++)
	{
		j=BGBCC_JX2_OptInterlock_DoSwaps(sctx, spos, epos);
		if(j<=0)
			break;
	}
#endif

	for(i=0; i<16; i++)
	{
		j=BGBCC_JX2_CheckWexify_DoSwaps(sctx, spos, epos);
		if(j<=0)
			break;
	}

	BGBCC_JX2_CheckWexify_DoBundle(sctx, spos, epos);
	return(1);
}
#endif


#if 0
ccxl_status BGBCC_JX2_CheckWexify(
	BGBCC_JX2_Context *sctx,
	int spos, int epos)
{
	int opw1, opw2, opw3, opw4, opw5, opw6;
	int dp, cp;
	
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

//	if(sctx->is_simpass)
//		return(0);

//	return(0);
	
	cp=spos;
	while((cp+7)<epos)
	{
		opw1=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 0);
		opw2=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 2);
		opw3=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 4);
		opw4=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 6);
		opw5=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 8);
		opw6=BGBCC_JX2_EmitGetOffsWord(sctx, cp+10);
		
		if(!((cp+11)<epos))
		{
			opw5=0;
			opw6=0;
		}

		if((opw1&0xE000)!=0xE000)
			{ BGBCC_DBGBREAK }
		if((opw3&0xE000)!=0xE000)
			{ BGBCC_DBGBREAK }

		if((cp+11)<epos)
		{
			if((opw5&0xE000)!=0xE000)
				{ BGBCC_DBGBREAK }
		}

		/* Skip predicated ops. */
		if((opw1&0xF000)==0xE000)
			{ cp+=4; continue; }

		/* Skip DLR loads. */
		if((opw1&0xFE00)==0xFA00)
			{ cp+=4; continue; }

#if 0
		/* Skip Jumbo Forms */
		if(((opw1&0xFF00)==0xF400) &&
			((opw2&0xC000)==0xC000))
		{
			if(((opw3&0xFF00)==0xF400) &&
				((opw4&0xC000)==0xC000))
			{
				cp+=12;
				continue;
			}

			cp+=8;
			continue;
		}
#endif

		/* Skip Jumbo Forms (New) */
		if((opw1&0xFE00)==0xFE00)
		{
			if((opw3&0xFF00)==0xFE00)
			{
				cp+=12;
				continue;
			}

			cp+=8;
			continue;
		}

//		if(((opw1&0xFC00)==0xF400) ||
//			((opw1&0xFF00)==0xF900))
//		if(((opw1&0xFC00)==0xF400) ||
//			((opw1&0xFE00)==0xFC00))
		if(	((opw1&0xFC00)==0xF400) ||
			((opw1&0xFE00)==0xFC00)	||
			((opw1&0xFE00)==0xEA00) ||
			((opw1&0xFE00)==0xEE00))
		{
			/* Wexify Ignores existing WEX sequences. */
			if((opw3&0xFC00)==0xF000)
				{ cp+=8; continue; }
			if((opw3&0xFC00)==0xF400)
				{ cp+=4; continue; }
//			if((opw3&0xFF00)==0xF900)
			if((opw3&0xFE00)==0xFC00)
				{ cp+=4; continue; }

			if((opw3&0xFE00)==0xEA00)
				{ cp+=4; continue; }
			if((opw3&0xFE00)==0xEE00)
				{ cp+=4; continue; }

			cp+=8;
			continue;
		}
		
		/* If ops are immovable, skip. */
		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw1, opw2)>0)
			{ cp+=4; continue; }
		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw3, opw4)>0)
			{ cp+=4; continue; }
		
		if(!BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw1, opw2))
		{
#if 1
			if(	BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw3, opw4) &&
				BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw5, opw6) &&
				BGBCC_JX2_CheckOps32ValidWexSuffix(sctx, opw1, opw2) &&
//				!BGBCC_JX2_CheckOps32SequenceOnly(ctx, sctx,
				BGBCC_JX2_CheckCanSwapOps32(sctx, opw1, opw2, opw3, opw4) &&
				BGBCC_JX2_CheckCanSwapOps32(sctx, opw1, opw2, opw5, opw6))
			{
				if(sctx->is_simpass)
					{ cp+=4; continue; }
			
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 0, opw3);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 2, opw4);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 4, opw5);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 6, opw6);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 8, opw1);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+10, opw2);
				continue;
			}
#endif

#if 1
			if(	BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw3, opw4) &&
				BGBCC_JX2_CheckOps32ValidWexSuffix(sctx, opw1, opw2) &&
//				!BGBCC_JX2_CheckOps32SequenceOnly(ctx, sctx,
				BGBCC_JX2_CheckCanSwapOps32(sctx,
					opw1, opw2, opw3, opw4))
			{
				if(sctx->is_simpass)
					{ cp+=4; continue; }
			
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+0, opw3);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+2, opw4);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+4, opw1);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+6, opw2);
				continue;
			}
#endif

			cp+=4; continue;
		}

		if(!BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw3, opw4) &&
			!sctx->is_simpass && ((cp+11)<epos))
		{
#if 1
			if(	BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw5, opw6) &&
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
						opw1, opw2, opw3, opw4)))
			{
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 4, opw5);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 6, opw6);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 8, opw3);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+10, opw4);
				continue;
			}
#endif
		}

		/* If next op begins WEX sequence, skip. */
//		if(((opw3&0xFC00)==0xF400) ||
//			((opw3&0xFF00)==0xF900))
		if(	((opw3&0xFC00)==0xF400) ||
			((opw3&0xFC00)==0xFC00))
				{ cp+=4; continue; }

		/* If next op is a DLR load, skip. */
		if((opw3&0xFE00)==0xFA00)
			{ cp+=4; continue; }

		/* Skip Mem Load/Store (Lane 1 only) */
		if((opw1&0xFF00)==0xF100)
			{ cp+=4; continue; }

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

#if 0
		if((opw1&0xFF00)==0xF800)
		{
			if(BGBCC_JX2_CheckOps32ValidWexSuffix(sctx, opw3, opw4) &&
				!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
					opw1, opw2, opw3, opw4))
			{
				BGBCC_JX2_AdjustWexifyOp(sctx, &opw1, &opw2);

				BGBCC_JX2_EmitSetOffsWord(sctx, cp+0, opw1);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+2, opw2);
				cp+=8;
				continue;
			}

			cp+=4;
			continue;
		}
#endif

//		if(((opw1&0xFF00)==0xF000) ||
//			((opw1&0xFF00)==0xF200))
		if(1)
//		if(0)
		{
			if(	BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw1, opw2) &&
				BGBCC_JX2_CheckOps32ValidWexSuffix(sctx, opw3, opw4) &&
				!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
					opw1, opw2, opw3, opw4))
			{
				BGBCC_JX2_AdjustWexifyOp(sctx, &opw1, &opw2);

				BGBCC_JX2_EmitSetOffsWord(sctx, cp+0, opw1);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+2, opw2);
				cp+=8;
				continue;
			}

			cp+=4;
			continue;
		}

		/* Dunno, Leave it as-is. */
		cp+=4;
	}
	
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
//		if(!BGBCC_JX2_CheckPadAlign32(sctx))
		if(sctx->op_wex_align && !BGBCC_JX2_CheckPadAlign32(sctx))
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
