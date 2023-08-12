u64 TKUCC_ReadUVLI(byte **rcs)
{
	byte *cs;
	u64 tv;
	int i0, i1, i2, i3;
	
	cs=*rcs;
	i0=cs[0];
	if(!(i0&0x80))
	{
		*rcs=cs+1;
		return(i0);
	}

	if(!(i0&0x40))
	{
		i1=cs[1];
		tv=((i0&0x3F)<<8)|i1;
		*rcs=cs+2;
		return(tv);
	}

	if(!(i0&0x20))
	{
		i1=cs[1];
		i2=cs[2];
		tv=((i0&0x1F)<<16)|(i1<<8)|i2;
		*rcs=cs+3;
		return(tv);
	}

	if(!(i0&0x10))
	{
		i1=cs[1];
		i2=cs[2];
		i3=cs[3];
		tv=((i0&0x0F)<<24)|(i1<<16)|(i2<<8)|i3;
		*rcs=cs+4;
		return(tv);
	}

	if(!(i0&0x08))
	{
		i1=cs[1];
		i2=cs[2];
		i3=cs[3];
		tv=((i0&0x07)<<24)|(i1<<16)|(i2<<8)|i3;
		i0=cs[4];
		tv=(tv<<8)|i0;
		*rcs=cs+5;
		return(tv);
	}

	if(!(i0&0x04))
	{
		i1=cs[1];
		i2=cs[2];
		i3=cs[3];
		tv=((i0&0x03)<<24)|(i1<<16)|(i2<<8)|i3;
		i0=cs[4];
		i1=cs[5];
		tv=(tv<<16)|(i0<<8)|i1;
		*rcs=cs+6;
		return(tv);
	}

	if(!(i0&0x02))
	{
		i1=cs[1];
		i2=cs[2];
		i3=cs[3];
		tv=((i0&0x01)<<24)|(i1<<16)|(i2<<8)|i3;
		i0=cs[4];
		i1=cs[5];
		i2=cs[6];
		tv=(tv<<24)|(i0<<16)|(i1<<8)|i2;
		*rcs=cs+7;
		return(tv);
	}

	if(!(i0&0x01))
	{
		i1=cs[1];
		i2=cs[2];
		i3=cs[3];
		tv=(i1<<16)|(i2<<8)|i3;
		i0=cs[4];
		i1=cs[5];
		i2=cs[6];
		i3=cs[7];
		tv=(tv<<32)|(i0<<24)|(i1<<16)|(i2<<8)|i3;
		*rcs=cs+8;
		return(tv);
	}

	i1=cs[1];
	i2=cs[2];
	i3=cs[3];
	tv=(i1<<16)|(i2<<8)|i3;
	i0=cs[4];
	i1=cs[5];
	i2=cs[6];
	i3=cs[7];
	tv=(tv<<32)|(i0<<24)|(i1<<16)|(i2<<8)|i3;
	tv=(tv<<8)|cs[8];
	*rcs=cs+9;
	return(tv);
}

s64 TKUCC_ReadSVLI(byte **rcs)
{
	u64 tv0;
	s64 tv;
	
	tv0=TKUCC_ReadUVLI(rcs);
	tv=(tv0>>1)^(~((tv0&1)-1));
	return(tv);
}

void TKUCC_EmitUVLI(byte **rct, u64 val)
{
	byte *ct;
	
	ct=*rct;
	if(val<0x80)
	{
		ct[0]=val;
		*rct=ct+1;
		return;
	}
	if(val<0x4000)
	{
		ct[0]=0x80|(val>>8);
		ct[1]=val;
		*rct=ct+2;
		return;
	}
	if(val<0x200000)
	{
		ct[0]=0xC0|(val>>16);
		ct[1]=val>>8;
		ct[2]=val;
		*rct=ct+3;
		return;
	}
	if(val<0x10000000)
	{
		ct[0]=0xE0|(val>>24);
		ct[1]=val>>16;
		ct[2]=val>>8;
		ct[3]=val;
		*rct=ct+4;
		return;
	}

	if(val<0x800000000ULL)
	{
		ct[0]=0xF0|(val>>32);
		ct[1]=val>>24;
		ct[2]=val>>16;
		ct[3]=val>>8;
		ct[4]=val;
		*rct=ct+5;
		return;
	}

	if(val<0x40000000000ULL)
	{
		ct[0]=0xF8|(val>>40);
		ct[1]=val>>32;
		ct[2]=val>>24;
		ct[3]=val>>16;
		ct[4]=val>>8;
		ct[5]=val;
		*rct=ct+6;
		return;
	}

	if(val<0x2000000000000ULL)
	{
		ct[0]=0xFC|(val>>48);
		ct[1]=val>>40;
		ct[2]=val>>32;
		ct[3]=val>>24;
		ct[4]=val>>16;
		ct[5]=val>>8;
		ct[6]=val;
		*rct=ct+7;
		return;
	}

	if(val<0x100000000000000ULL)
	{
		ct[0]=0xFE;
		ct[1]=val>>48;
		ct[2]=val>>40;
		ct[3]=val>>32;
		ct[4]=val>>24;
		ct[5]=val>>16;
		ct[6]=val>>8;
		ct[7]=val;
		*rct=ct+8;
		return;
	}

	ct[0]=0xFF;
	ct[1]=val>>56;
	ct[2]=val>>48;
	ct[3]=val>>40;
	ct[4]=val>>32;
	ct[5]=val>>24;
	ct[6]=val>>16;
	ct[7]=val>>8;
	ct[8]=val;
	*rct=ct+9;
	return;
}

u32 TKUCC_ReadUTF8(byte **rcs)
{
	byte *cs;
	u32 tv;
	int i0, i1, i2, i3;
	
	cs=*rcs;
	i0=cs[0];
	if(!(i0&0x80))
	{
		*rcs=cs+1;
		return(i0);
	}

	if(!(i0&0x40))
	{
		i1=0xC0|(i0&63);
		*rcs=cs+1;
		return(i1);
	}

	if(!(i0&0x20))
	{
		i1=cs[1];
		tv=((i0&0x1F)<<6)|(i1&0x3F);
		*rcs=cs+2;
		return(tv);
	}

	if(!(i0&0x10))
	{
		i1=cs[1];
		i2=cs[2];
		tv=((i0&0x0F)<<12)|((i1&0x3F)<<6)|(i2&0x3F);
		*rcs=cs+3;
		return(tv);
	}

	if(!(i0&0x08))
	{
		i1=cs[1];
		i2=cs[2];
		i3=cs[3];
		tv=((i0&0x07)<<18)|((i1&0x3F)<<12)|((i2&0x3F)<<6)|(i3&0x3F);
		*rcs=cs+4;
		return(tv);
	}

	return(0);
}

void TKUCC_EmitUTF8(byte **rct, u32 val)
{
	byte *ct;
	
	ct=*rct;
	if(val<0x80)
	{
		ct[0]=val;
		*rct=ct+1;
		return;
	}
	if(val<0x800)
	{
		ct[0]=0xC0|(val>>6);
		ct[1]=0x80|(val&0x3F);
		*rct=ct+2;
		return;
	}
	if(val<0x10000)
	{
		ct[0]=0xE0|(val>>12);
		ct[1]=0x80|((val>>6)&0x3F);
		ct[2]=0x80|((val>>0)&0x3F);
		*rct=ct+3;
		return;
	}
	if(val<0x200000)
	{
		ct[0]=0xE0|(val>>18);
		ct[1]=0x80|((val>>12)&0x3F);
		ct[2]=0x80|((val>> 6)&0x3F);
		ct[3]=0x80|((val>> 0)&0x3F);
		*rct=ct+4;
		return;
	}
}

u32 TKUCC_MortonSplitX(u32 val)
{
	u16 tv;
	static byte mort4to2tab[16]={
		0x0, 0x1, 0x0, 0x1,	0x2, 0x3, 0x2, 0x3,
		0x0, 0x1, 0x0, 0x1,	0x2, 0x3, 0x2, 0x3	};
	tv=	(mort4to2tab[(val>> 0)&15]<< 0)|
		(mort4to2tab[(val>> 4)&15]<< 2)|
		(mort4to2tab[(val>> 8)&15]<< 4)|
		(mort4to2tab[(val>>12)&15]<< 6)|
		(mort4to2tab[(val>>16)&15]<< 8)|
		(mort4to2tab[(val>>20)&15]<<10)|
		(mort4to2tab[(val>>24)&15]<<12)|
		(mort4to2tab[(val>>28)&15]<<14);
	return(tv);
}

u32 TKUCC_MortonSplitY(u32 val)
{
	return(TKUCC_MortonSplitX(val>>1));
}

TKUCC_IROP *TKUCC_CtacAllocIrOp(TKUCC_MainContext *ctx)
{
	TKUCC_IROP *tmp;
	tmp=TKUCC_ZMalloc(ctx, sizeof(TKUCC_IROP), TKUCC_ZID_TAC);
	memset(tmp, 0, sizeof(TKUCC_IROP));
	return(tmp);
}

TKUCC_IROP *TKUCC_CtacDecodeIrOp(TKUCC_MainContext *ctx, byte **rcs)
{
	TKUCC_IROP *tmp;
	u32 msk, cmd;

	tmp=TKUCC_CtacAllocIrOp(ctx);
	
	msk=TKUCC_ReadUVLI(rcs);
	cmd=TKUCC_ReadUVLI(rcs);
	
	if(msk&1)
	{
		tmp->cmd=TKUCC_MortonSplitX(cmd);
		tmp->opr=TKUCC_MortonSplitY(cmd);
	}else
	{
		tmp->cmd=cmd;
	}
	
	return(tmp);
}
