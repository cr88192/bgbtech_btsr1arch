#include <bgbccc.h>

#define RIL3_LBLMAX	262144

//ccxl_label ril3_lbltab[65536];
ccxl_label ril3_lbltab[RIL3_LBLMAX];
int ril3_lblchn[RIL3_LBLMAX];
int ril3_lblhash[1024];
int ril3_nlbl=0;

int BGBCC_CCXLR3_ClearLabels(BGBCC_TransState *ctx)
{
	int i;

	ril3_nlbl=0;
	memset(ril3_lbltab, 0, sizeof(ril3_lbltab));

	for(i=0; i<1024; i++)
		ril3_lblhash[i]=-1;
	return(0);
}

int BGBCC_CCXLR3_LabelToIndex(BGBCC_TransState *ctx, ccxl_label lbl)
{
	int v, h;
	int i;

	if(lbl.id==CCXL_LBL_RETURN)
		return(-4);
	if(lbl.id==CCXL_LBL_RETURN_ZERO)
		return(-8);

	if(lbl.id<CCXL_LBL_GENSYMBASE)
	{
		BGBCC_CCXL_StubError(ctx);
	}

	if(!ril3_nlbl)
	{
		for(i=0; i<1024; i++)
			ril3_lblhash[i]=-1;
	}

//	if(!(ctx->ril_ip))

	v=lbl.id;
	h=((v*65521)>>16)&1023;

	i=ril3_lblhash[h];
	while(i>=0)
	{
		if(ril3_lbltab[i].id==lbl.id)
			return(i);
		i=ril3_lblchn[i];
	}

#if 0
	for(i=0; i<ril3_nlbl; i++)
	{
		if(ril3_lbltab[i].id==lbl.id)
			return(i);
	}
#endif
	
	i=ril3_nlbl++;
	if(i>=RIL3_LBLMAX)
		{ BGBCC_DBGBREAK }
	
	ril3_lbltab[i].id=lbl.id;
	ril3_lblchn[i]=ril3_lblhash[h];
	ril3_lblhash[h]=i;
	return(i);
}

ccxl_label BGBCC_CCXLR3_IndexToLabel(BGBCC_TransState *ctx, int ix)
{
	ccxl_label l0;

	if(ix<0)
	{
		if(ix==-4)
		{
			l0.id=CCXL_LBL_RETURN;
			return(l0);
		}

		if(ix==-8)
		{
			l0.id=CCXL_LBL_RETURN_ZERO;
			return(l0);
		}
		
		BGBCC_CCXL_TagError(ctx,
			CCXL_TERR_STATUS(CCXL_STATUS_ERR_BADOPARGS));

		l0.id=CCXL_LBL_RETURN;
		return(l0);
	}

	if(ril3_lbltab[ix].id)
		return(ril3_lbltab[ix]);
	
	if(ix>=ril3_nlbl)
		ril3_nlbl=ix+1;
	
	ril3_lbltab[ix]=BGBCC_CCXL_GenSym(ctx);
	return(ril3_lbltab[ix]);
}

void BGBCC_CCXLR3_CheckLzWindow(
	BGBCC_TransState *ctx)
{
	if(!ctx->ril_txwin)
	{
		ctx->ril_txwin=bgbcc_malloc(16384);
		ctx->ril_txhash=bgbcc_malloc(256*4*2);
		ctx->ril_txrov=0;
		ctx->ril_psrov=0;
	}
}

void BGBCC_CCXLR3_CheckExpandOutput(
	BGBCC_TransState *ctx, int sz)
{
	int i, j, k;

	if(!(ctx->ril_ip))
	{
		i=sz;
		j=65536;
//		j=1<<20;
		while(i>=j)
			j=j+(j>>1);
		ctx->ril_ips=bgbcc_malloc(j);
		ctx->ril_ipe=ctx->ril_ips+j;
		ctx->ril_ip=ctx->ril_ips;
		BGBCC_CCXLR3_CheckLzWindow(ctx);
	}
	
	if((ctx->ril_ip+sz)>=(ctx->ril_ipe))
	{
		k=ctx->ril_ip-ctx->ril_ips;
		i=k+sz;
		j=ctx->ril_ipe-ctx->ril_ips;

		while(i>=j)
			j=j+(j>>1);

		ctx->ril_ips=bgbcc_realloc(ctx->ril_ips, j);
		ctx->ril_ipe=ctx->ril_ips+j;
		ctx->ril_ip=ctx->ril_ips+k;
	}
}

void BGBCC_CCXLR3_EmitByte(
	BGBCC_TransState *ctx, int val)
{
	BGBCC_CCXLR3_CheckExpandOutput(ctx, 1);
	*ctx->ril_ip++=val;
}

void BGBCC_CCXLR3_EmitUVLI(
	BGBCC_TransState *ctx, u64 val)
{
	if(val<0x80)
	{
		BGBCC_CCXLR3_EmitByte(ctx, val);
	}else if(val<0x4000)
	{
		BGBCC_CCXLR3_EmitByte(ctx, 0x80|((val>>8)&63));
		BGBCC_CCXLR3_EmitByte(ctx, val&255);
	}else if(val<0x200000)
	{
		BGBCC_CCXLR3_EmitByte(ctx, 0xC0|((val>>16)&31));
		BGBCC_CCXLR3_EmitByte(ctx, (val>>8)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val   )&255);
	}else if(val<0x10000000)
	{
		BGBCC_CCXLR3_EmitByte(ctx, 0xE0|((val>>24)&15));
		BGBCC_CCXLR3_EmitByte(ctx, (val>>16)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>> 8)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val    )&255);
	}else if(val<0x800000000ULL)
	{
		BGBCC_CCXLR3_EmitByte(ctx, 0xF0|((val>>32)&7));
		BGBCC_CCXLR3_EmitByte(ctx, (val>>24)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>16)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>> 8)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val    )&255);
	}else if(val<0x40000000000ULL)
	{
		BGBCC_CCXLR3_EmitByte(ctx, 0xF8|((val>>40)&3));
		BGBCC_CCXLR3_EmitByte(ctx, (val>>32)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>24)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>16)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>> 8)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val    )&255);
	}else if(val<0x2000000000000ULL)
	{
		BGBCC_CCXLR3_EmitByte(ctx, 0xFC|((val>>48)&1));
		BGBCC_CCXLR3_EmitByte(ctx, (val>>40)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>32)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>24)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>16)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>> 8)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val    )&255);
	}else if(val<0x1000000000000ULL)
	{
		BGBCC_CCXLR3_EmitByte(ctx, 0xFE);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>48)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>40)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>32)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>24)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>16)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>> 8)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val    )&255);
	}else
	{
		BGBCC_CCXLR3_EmitByte(ctx, 0xFF);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>56)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>48)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>40)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>32)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>24)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>16)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>> 8)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val    )&255);
	}
}

void BGBCC_CCXLR3_EmitSVLI(
	BGBCC_TransState *ctx, s64 val)
{
	u64 val2;
	
	val2=(val<<1)^(val>>63);
	BGBCC_CCXLR3_EmitUVLI(ctx, val2);
}

void BGBCC_CCXLR3_EmitOpVLI(
	BGBCC_TransState *ctx, u32 val)
{
	if(val<0xE0)
	{
		BGBCC_CCXLR3_EmitByte(ctx, val);
	}else if(val<0x1000)
	{
		BGBCC_CCXLR3_EmitByte(ctx, 0xE0|(val>>8));
		BGBCC_CCXLR3_EmitByte(ctx, val&255);
	}else if(val<0x80000)
	{
		BGBCC_CCXLR3_EmitByte(ctx, 0xF0|(val>>16));
		BGBCC_CCXLR3_EmitByte(ctx, (val>>8)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val   )&255);
	}else if(val<0x4000000)
	{
		BGBCC_CCXLR3_EmitByte(ctx, 0xF8|(val>>24));
		BGBCC_CCXLR3_EmitByte(ctx, (val>>16)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>> 8)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val    )&255);
	}else
	{
		BGBCC_CCXLR3_EmitByte(ctx, 0xFC);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>24)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>16)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>> 8)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val    )&255);
	}
}

void BGBCC_CCXLR3_UpdateTextByte(
	BGBCC_TransState *ctx, int val)
{
	int h;
	
//	ctx->ril_txrh=(ctx->ril_txrh<<8)|(val&255);
//	h=(((ctx->ril_txrh&0x00FFFFFF)*16777213)>>24)&255;
	h=(((ctx->ril_txrh&0x0000FFFF)*65521)>>16)&255;
//	ctx->ril_txhash[h]=(ctx->ril_txrov-3)&16383;
//	ctx->ril_txhash[h]=(ctx->ril_txrov-2)&16383;

	ctx->ril_txhash[h*4+3]=ctx->ril_txhash[h*4+2];
	ctx->ril_txhash[h*4+2]=ctx->ril_txhash[h*4+1];
	ctx->ril_txhash[h*4+1]=ctx->ril_txhash[h*4+0];
	ctx->ril_txhash[h*4+0]=(ctx->ril_txrov-2)&16383;
//	ctx->ril_txhash[h*4+0]=(ctx->ril_txrov-3)&16383;

	ctx->ril_txwin[ctx->ril_txrov]=val;
	ctx->ril_txrov=(ctx->ril_txrov+1)&16383;
	ctx->ril_txrh=(ctx->ril_txrh<<8)|(val&255);
}

void BGBCC_CCXLR3_UpdateTextString(
	BGBCC_TransState *ctx, byte *str, int sz)
{
	int i;
	for(i=0; i<sz; i++)
		BGBCC_CCXLR3_UpdateTextByte(ctx, str[i]);
}

int BGBCC_CCXLR3_TextLookupMatch(
	BGBCC_TransState *ctx, byte *str, byte *stre, int *rl, int *rd)
{
	byte *s, *se;
	int h, p, l, d, bl, bd;
	int c0, c1;
	int i;
	
	if((str+3)>=stre)
		return(0);
	
//	h=(((ctx->ril_txrh&0x00FFFFFF)*16777213)>>24)&255;
//	h=(((ctx->ril_txrh&0x0000FFFF)*65521)>>16)&255;

//	p=(str[0]<<16)|(str[1]<<8)|str[2];
//	h=(((p&0x00FFFFFF)*16777213)>>24)&255;

	p=(str[0]<<8)|str[1];
	h=(((p&0x0000FFFF)*65521)>>16)&255;
	
	bl=0; bd=0;
	for(i=0; i<4; i++)
	{
		p=ctx->ril_txhash[h*4+i];
		d=(ctx->ril_txrov-p)&16383;
		
//		if(d>12288)
//			continue;
		
		s=str;
//		se=str+252;
		se=str+4088;
		if(se>stre)se=stre;
		while(s<se)
		{
			c0=*s; c1=ctx->ril_txwin[p];
			if(c0!=c1)break;
			s++; p=(p+1)&16383;
		}

		l=s-str;
		if(l>bl)
			{ bl=l; bd=d; }
	}
	
	if((bd>=16128) || ((bl+bd)>=16384))
		bl=0;
	
	if(bl>=3)
	{
		*rl=bl;
		*rd=bd;
		return(1);
	}
	
	*rl=0;
	*rd=0;
	return(0);
}

void BGBCC_CCXLR3_EmitTextByte(
	BGBCC_TransState *ctx, int val)
{
	BGBCC_CCXLR3_EmitByte(ctx, val);
	BGBCC_CCXLR3_UpdateTextByte(ctx, val);
}

void BGBCC_CCXLR3_EmitRawString(
	BGBCC_TransState *ctx, char *str)
{
	char *s;
	
	s=str;
	while(*s)
		BGBCC_CCXLR3_EmitTextByte(ctx, *s++);
	BGBCC_CCXLR3_EmitTextByte(ctx, *s++);
}

void BGBCC_CCXLR3_EmitOp(
	BGBCC_TransState *ctx, int op)
{
	if(!(ctx->ril_ip) || ctx->ril3_noril)
		return;

//	BGBCC_CCXLR3_EmitUVLI(ctx, op);
	BGBCC_CCXLR3_EmitOpVLI(ctx, op);
}

int BGBCC_IsEmitRil(BGBCC_TransState *ctx)
{
	if(ctx->ril_ip)
		return(1);
	return(0);
}

void BGBCC_CCXLR3_EmitArgInt(
	BGBCC_TransState *ctx, s64 val)
{
	if(!(ctx->ril_ip) || ctx->ril3_noril)
		return;

//	BGBCC_CCXLR3_EmitUVLI(ctx, BGBCC_RIL3OP_ARGINT);
	BGBCC_CCXLR3_EmitSVLI(ctx, val);
}

void BGBCC_CCXLR3_EmitArgTag(
	BGBCC_TransState *ctx, s64 val)
{
	if(!(ctx->ril_ip) || ctx->ril3_noril)
		return;

	if((val>0x8000) && (val<0x8020))
		val=val&0x1F;
	if((val>0x9000) && (val<0x9020))
		val=0x20|(val&0x1F);

//	BGBCC_CCXLR3_EmitUVLI(ctx, BGBCC_RIL3OP_ARGINT);
	BGBCC_CCXLR3_EmitSVLI(ctx, val);
}

void BGBCC_CCXLR3_EmitArgFloat(
	BGBCC_TransState *ctx, f64 val)
{
	u64 fv;
	s64 fm;
	int fe, fs;

	if(!(ctx->ril_ip) || ctx->ril3_noril)
		return;

	fv=*(u64 *)(&val);
	
	if(!fv)
	{
		BGBCC_CCXLR3_EmitSVLI(ctx, 0);
		BGBCC_CCXLR3_EmitSVLI(ctx, 0);
		return;
	}
	
	fm=(fv&0x000FFFFFFFFFFFFFULL)|0x0010000000000000ULL;
	fe=(fv>>52)&2047;
	fs=(fv>>63)&1;
	fe-=(1023+52);
	
	while(!(fm&1))
		{ fm=fm>>1; fe++; }
	if(fs)
		{ fm=-fm; }

//	BGBCC_CCXLR3_EmitUVLI(ctx, BGBCC_RIL3OP_ARGINT);
	BGBCC_CCXLR3_EmitSVLI(ctx, fe);
	BGBCC_CCXLR3_EmitSVLI(ctx, fm);
}

void BGBCC_CCXLR3_EmitArgUIntPair(
	BGBCC_TransState *ctx, u32 val1, u32 val2)
{
	if(!(ctx->ril_ip) || ctx->ril3_noril)
		return;

	BGBCC_CCXLR3_EmitSVLI(ctx, val1);
	BGBCC_CCXLR3_EmitSVLI(ctx, val2);
}

int BGBCC_CCXLR3_CheckEmitRIL(
	BGBCC_TransState *ctx)
{
	if(!(ctx->ril_ip) || ctx->ril3_noril)
		return(0);
	return(1);
}

int BGBCC_CCXL_CheckIsStaticLib(
	BGBCC_TransState *ctx)
{
	if(!(ctx->ril_ip))
		return(0);
	return(1);
}

void BGBCC_CCXLR3_EmitBufUVLI(
	byte **rct, u64 val)
{
	byte *ct;
	
	ct=*rct;
	if(val<0x80)
	{
		*ct++=val;
	}else if(val<0x4000)
	{
		*ct++=(0x80|((val>>8)&63));		*ct++=(val&255);
	}else if(val<0x200000)
	{
		*ct++=(0xC0|((val>>16)&31));
		*ct++=((val>>8)&255);			*ct++=((val   )&255);
	}else if(val<0x10000000)
	{
		*ct++=(0xE0|((val>>24)&15));	*ct++=((val>>16)&255);
		*ct++=((val>> 8)&255);			*ct++=((val    )&255);
	}else if(val<0x800000000ULL)
	{
		*ct++=(0xF0|((val>>32)&7));
		*ct++=((val>>24)&255);			*ct++=((val>>16)&255);
		*ct++=((val>> 8)&255);			*ct++=((val    )&255);
	}else if(val<0x40000000000ULL)
	{
		*ct++=(0xF8|((val>>40)&3));		*ct++=((val>>32)&255);
		*ct++=((val>>24)&255);			*ct++=((val>>16)&255);
		*ct++=((val>> 8)&255);			*ct++=((val    )&255);
	}else if(val<0x2000000000000ULL)
	{
		*ct++=(0xFC|((val>>48)&1));
		*ct++=((val>>40)&255);			*ct++=((val>>32)&255);
		*ct++=((val>>24)&255);			*ct++=((val>>16)&255);
		*ct++=((val>> 8)&255);			*ct++=((val    )&255);
	}else if(val<0x1000000000000ULL)
	{
		*ct++=(0xFE);					*ct++=((val>>48)&255);
		*ct++=((val>>40)&255);			*ct++=((val>>32)&255);
		*ct++=((val>>24)&255);			*ct++=((val>>16)&255);
		*ct++=((val>> 8)&255);			*ct++=((val    )&255);
	}else
	{
		*ct++=(0xFF);
		*ct++=((val>>56)&255);			*ct++=((val>>48)&255);
		*ct++=((val>>40)&255);			*ct++=((val>>32)&255);
		*ct++=((val>>24)&255);			*ct++=((val>>16)&255);
		*ct++=((val>> 8)&255);			*ct++=((val    )&255);
	}
	*rct=ct;
}

/*
 * 0: NULL
 * >0: String with N chars.
 * -1 .. -64: Index for recently seen string.
 * < (-65): LZ Compressed String
 */
void BGBCC_CCXLR3_EmitArgBlob(
	BGBCC_TransState *ctx, byte *str, int len)
{
	byte ttb[65536];
	byte *ct, *cte, *tbuf, *tbuf1;
	byte *s, *se, *rs;
	int c0, c1;
	int r, t, p, ml, md, lj;
	int i, j, k;

	if(!(ctx->ril_ip) || ctx->ril3_noril)
		return;

	if(!str)
	{
		BGBCC_CCXLR3_EmitSVLI(ctx, 0);
		return;
	}

//	l=strlen(str);
//	if(!l)l=1;

#if 0
	lj=-1;
	for(i=0; i<64; i++)
	{
		j=(ctx->ril_psrov-i-1)&63;
		if(ctx->ril_pslen[j]!=len)
			continue;

		lj=j;
		p=ctx->ril_psidx[j];
		if(str[0]!=ctx->ril_txwin[p])
			continue;

		s=str;
		for(k=0; k<len; k++)
		{
			c0=*s; c1=ctx->ril_txwin[p];
			if(c0!=c1)break;
			s++; p=(p+1)&16383;
		}
		if(k>=len)
			break;
	}
	if(i<64)
	{
		BGBCC_CCXLR3_EmitSVLI(ctx, -(i+1));
		return;
	}
	if(len<256)
	{
		i=ctx->ril_psrov;
		ctx->ril_psidx[i]=ctx->ril_txrov;
		ctx->ril_pslen[i]=len;
		ctx->ril_psrov=(i+1)&63;
	}
#endif

#if 1
	if(len<256)
	{
		j=ctx->ril_pslix[len];
		for(i=0; i<16; i++)
		{
			if(ctx->ril_pslen[j]!=len)
				break;

			p=ctx->ril_psidx[j];
			if(str[0]!=ctx->ril_txwin[p])
				{ j=ctx->ril_pschn[j]; continue; }

			s=str;
			for(k=0; k<len; k++)
			{
				c0=*s; c1=ctx->ril_txwin[p];
				if(c0!=c1)break;
				s++; p=(p+1)&16383;
			}
			if(k>=len)
				break;
			j=ctx->ril_pschn[j];
		}

#if 1
		if((i<16) && (ctx->ril_pslen[j]==len))
		{
			k=(ctx->ril_psrov-(j+1))&63;
			BGBCC_CCXLR3_EmitSVLI(ctx, -(k+1));
			return;
		}
#endif

#if 0
		i=ctx->ril_psrov;
		ctx->ril_psidx[i]=ctx->ril_txrov;
		ctx->ril_pslen[i]=len;

		j=ctx->ril_pslix[len];
		ctx->ril_pschn[i]=j;
		ctx->ril_pslix[len]=i;

		ctx->ril_psrov=(i+1)&63;
#endif
	}
#endif

#if 1
	i=ctx->ril_psrov;
	ctx->ril_psidx[i]=ctx->ril_txrov;
	ctx->ril_pslen[i]=len;

	if(len<256)
	{
		j=ctx->ril_pslix[len];
		ctx->ril_pschn[i]=j;
		ctx->ril_pslix[len]=i;
	}

	ctx->ril_psrov=(i+1)&63;
#endif

//	if(len<8)
	if(1)
//	if(len<24)
	{
#if 0
		i=ctx->ril_psrov;
		ctx->ril_psidx[i]=ctx->ril_txrov;
		ctx->ril_pslen[i]=len;

		if(len<256)
		{
			j=ctx->ril_pslix[len];
			ctx->ril_pschn[i]=j;
			ctx->ril_pslix[len]=i;
		}

		ctx->ril_psrov=(i+1)&63;
#endif

		BGBCC_CCXLR3_EmitSVLI(ctx, len);
		s=str;
//		while(*s)
		for(i=0; i<len; i++)
			BGBCC_CCXLR3_EmitTextByte(ctx, *s++);
		return;
	}
	
	tbuf=ttb;
	ct=tbuf; cte=tbuf+65536;
	s=str;
	se=s+len;
	
	rs=s;
	while(s<se)
	{
		r=s-rs;
		if((ct+8+r)>=cte)
		{
			i=cte-ct;
			j=i+(i>>1);
			k=ct-tbuf;
			tbuf1=bgbcc_malloc(j);
			memcpy(tbuf1, tbuf, i);
			if(tbuf!=ttb)
				{ bgbcc_free(tbuf); }
			tbuf=tbuf1;
			ct=tbuf+k;
			cte=tbuf+j;
		}

		if(BGBCC_CCXLR3_TextLookupMatch(ctx, s, se, &ml, &md)>0)
		{
			t=0x00;
			r=s-rs;
			if(r>=15)
				{ t|=0xF0; }
			else
				{ t|=(r<<4); }
			if(ml>=17)
				{ t|=0x0F; }
			else
				{ t|=(ml-2); }
			
			*ct++=t;
			if(r>=15)
				{ BGBCC_CCXLR3_EmitBufUVLI(&ct, r); }
			while(rs<s)
				{ *ct++=*rs++; }
			if(ml>=17)
				{ BGBCC_CCXLR3_EmitBufUVLI(&ct, ml); }
			BGBCC_CCXLR3_EmitBufUVLI(&ct, md);
			for(i=0; i<ml; i++)
				BGBCC_CCXLR3_UpdateTextByte(ctx, *s++);
			rs=s;
		}else
		{
			BGBCC_CCXLR3_UpdateTextByte(ctx, *s++);
		}
	}
	r=s-rs;
	if(r>0)
	{
		t=0x00;
		if(r>=15)
			{ t|=0xF0; }
		else
			{ t|=(r<<4); }
		*ct++=t;
		if(r>=15)
			{ BGBCC_CCXLR3_EmitBufUVLI(&ct, r); }
		while(rs<s)
			{ *ct++=*rs++; }
	}

	k=ct-ttb;

	if((k+3)>=(len+1))
	{
#if 0
		i=ctx->ril_psrov;
		ctx->ril_psidx[i]=ctx->ril_txrov;
		ctx->ril_pslen[i]=len;

		if(len<256)
		{
			j=ctx->ril_pslix[len];
			ctx->ril_pschn[i]=j;
			ctx->ril_pslix[len]=i;
		}

		ctx->ril_psrov=(i+1)&63;
#endif

		BGBCC_CCXLR3_EmitSVLI(ctx, len);
		s=str;
		for(i=0; i<len; i++)
			BGBCC_CCXLR3_EmitByte(ctx, *s++);

		if(tbuf!=ttb)
			{ bgbcc_free(tbuf); }
		return;
	}
	
	BGBCC_CCXLR3_EmitSVLI(ctx, -(k+64));
	BGBCC_CCXLR3_EmitSVLI(ctx, len);
	s=tbuf;
	while(s<ct)
		BGBCC_CCXLR3_EmitByte(ctx, *s++);

	if(tbuf!=ttb)
		{ bgbcc_free(tbuf); }
	return;
}

void BGBCC_CCXLR3_EmitArgString(
	BGBCC_TransState *ctx, char *str)
{
	int l;
	
	if(!(ctx->ril_ip) || ctx->ril3_noril)
		return;

	if(!str)
	{
		BGBCC_CCXLR3_EmitSVLI(ctx, 0);
		return;
	}

//	if(!strncmp(str, "/*", 2))
//		__debugbreak();

	l=strlen(str);
	if(!l)l=1;
	BGBCC_CCXLR3_EmitArgBlob(ctx, (byte *)str, l);
}

void BGBCC_CCXLR3_EmitArgDataBlob(
	BGBCC_TransState *ctx, byte *buf, int len)
{
	if(!(ctx->ril_ip) || ctx->ril3_noril)
		return;

	BGBCC_CCXLR3_EmitArgBlob(ctx, buf, len);
}

void BGBCC_CCXLR3_EmitArgLabel(
	BGBCC_TransState *ctx, ccxl_label lbl)
{
	int i;

	if(!(ctx->ril_ip) || ctx->ril3_noril)
		return;

	i=BGBCC_CCXLR3_LabelToIndex(ctx, lbl);
	BGBCC_CCXLR3_EmitArgInt(ctx, i);
}

void BGBCC_CCXLR3_EmitArgTextBlob(
	BGBCC_TransState *ctx, char *str)
{
	BGBCC_CCXLR3_EmitArgString(ctx, str);
}

void BGBCC_CCXLR3_EmitArgSymbol(
	BGBCC_TransState *ctx, char *str)
{
	BGBCC_CCXLR3_EmitArgString(ctx, str);
}

void BGBCC_CCXLR3_EmitArgSigstr(
	BGBCC_TransState *ctx, char *str)
{
	BGBCC_CCXLR3_EmitArgString(ctx, str);
}

void BGBCC_CCXLR3_BeginRecRIL(
	BGBCC_TransState *ctx)
{
	ctx->ril_ip=ctx->ril_ips;
	ctx->ril_txrov=0;

	BGBCC_CCXLR3_EmitByte(ctx, 'R');
	BGBCC_CCXLR3_EmitByte(ctx, 'I');
	BGBCC_CCXLR3_EmitByte(ctx, 'L');
	BGBCC_CCXLR3_EmitByte(ctx, '3');
	BGBCC_CCXLR3_EmitByte(ctx, 1);	//format major.minor
	BGBCC_CCXLR3_EmitByte(ctx, 0);	//resv
	BGBCC_CCXLR3_EmitByte(ctx, 0);	//resv
	BGBCC_CCXLR3_EmitByte(ctx, 0);	//resv
}

u64 BGBCC_CCXLR3_ReadUVLI(BGBCC_TransState *ctx, byte **rcs)
{
	byte *cs;
	u64 tv;
	int i, j, k;
	
	cs=*rcs;
	i=*cs++;
	if(!(i&0x80))
		{ *rcs=cs; return(i); }
	if(!(i&0x40))
	{
		tv=((i&63)<<8)|(*cs++);
		*rcs=cs; return(tv);
	}
	if(!(i&0x20))
	{
		tv=((i&31)<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		*rcs=cs; return(tv);
	}
	if(!(i&0x10))
	{
		tv=((i&15)<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		*rcs=cs; return(tv);
	}
	if(!(i&0x08))
	{
		tv=((i&7)<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		*rcs=cs; return(tv);
	}
	if(!(i&0x04))
	{
		tv=((i&3)<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		*rcs=cs; return(tv);
	}
	if(!(i&0x02))
	{
		tv=((i&1)<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		*rcs=cs; return(tv);
	}
	if(!(i&0x01))
	{
		tv=(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		*rcs=cs; return(tv);
	}

	if(1)
	{
		tv=(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		*rcs=cs; return(tv);
	}
}

s64 BGBCC_CCXLR3_ReadSVLI(BGBCC_TransState *ctx, byte **rcs)
{
	u64 tv;
	s64 sv;

	tv=BGBCC_CCXLR3_ReadUVLI(ctx, rcs);
	sv=(tv>>1)^((((s64)tv)<<63)>>63);
	return(sv);
}

f64 BGBCC_CCXLR3_ReadFVLI(BGBCC_TransState *ctx, byte **rcs)
{
	int fe;
	s64 fm;
	f64 v;
	
	fe=BGBCC_CCXLR3_ReadSVLI(ctx, rcs);
	fm=BGBCC_CCXLR3_ReadSVLI(ctx, rcs);
	
	if(!fm)
	{
		if(!fe)
		{
			return(0);
		}
	}
	
	v=fm*pow(2.0, fe);
//	v=fm*bgbcc_pow2f(fe);
	return(v);
}

int BGBCC_CCXLR3_ReadXFVLI(BGBCC_TransState *ctx, byte **rcs,
	s64 *rlo, s64 *rhi)
{
	int fe, fe1, sg;
	s64 ml, mh;
	u64 fml, fmh;
	f64 v;

//	*rlo=BGBCC_CCXLR3_ReadSVLI(ctx, rcs);
//	*rhi=BGBCC_CCXLR3_ReadSVLI(ctx, rcs);
//	return(1);

#if 1
	fe=BGBCC_CCXLR3_ReadSVLI(ctx, rcs);
	ml=BGBCC_CCXLR3_ReadSVLI(ctx, rcs);
	mh=BGBCC_CCXLR3_ReadSVLI(ctx, rcs);
	
	if(!fe && mh)
	{
		*rlo=ml;
		*rhi=mh;
		return(1);
	}
	

	sg=0;
	if(mh<0)
	{
		mh=~mh;
		ml=-ml;
		sg=1;
	}

	fml=(u64)(ml);
	fmh=(u64)(mh);
	
	fe1=fe+112;
	while(!(fmh>>48))
	{
		fmh=(fmh<<1)|(fml>>63);
		fml=fml<<1;
		fe1--;
	}
	
	fmh|=fe1+16383;
	if(sg)fmh|=(1ULL<<63);
	
	*rhi=fmh;
	*rlo=fml;
	return(1);
	
//	v=fm*pow(2.0, fe);
//	return(v);
#endif

}

u32 BGBCC_CCXLR3_ReadOpVLI(BGBCC_TransState *ctx, byte **rcs)
{
	byte *cs;
	u32 tv;
	int i, j, k;
	
	cs=*rcs;
	i=*cs++;
	if((i&0xE0)!=0xE0)
		{ *rcs=cs; return(i); }
	if(!(i&0x10))
	{
		tv=((i&15)<<8)|(*cs++);
		*rcs=cs; return(tv);
	}
	if(!(i&0x08))
	{
		tv=((i&7)<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		*rcs=cs; return(tv);
	}
	if(!(i&0x04))
	{
		tv=((i&3)<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		*rcs=cs; return(tv);
	}
	if(!(i&0x02))
	{
		tv=*cs++;
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		*rcs=cs; return(tv);
	}

	BGBCC_DBGBREAK
	tv=i;
	*rcs=cs; return(tv);
}

int BGBCC_CCXLR3_ReadTag(BGBCC_TransState *ctx, byte **rcs)
{
	int i;
	i=BGBCC_CCXLR3_ReadSVLI(ctx, rcs);
	if((i>=0x00) && (i<=0x1F))
		i=0x8000+(i&0x1F);
	if((i>=0x20) && (i<=0x3F))
		i=0x9000+(i&0x1F);
	return(i);
}

int BGBCC_CCXLR3_ReadUPairVLI(BGBCC_TransState *ctx, byte **rcs,
	int *rv0, int *rv1)
{
	u32 uv0, uv1;
	
	uv0=BGBCC_CCXLR3_ReadUVLI(ctx, rcs);
	
	if(uv0&1)
	{
		*rv0=(uv0>>1)&7;
		*rv1=(uv0>>4)&7;
	}else
	{
		uv1=BGBCC_CCXLR3_ReadUVLI(ctx, rcs);
		*rv0=uv0>>1;
		*rv1=uv1>>1;
	}
	return(1);
}

int BGBCC_CCXLR3_ReadTextBlob(BGBCC_TransState *ctx, byte **rcs,
	byte **rbuf, int *rsz)
{
	byte *tbuf, *cs, *cse, *ct, *cte;
	int sz, csz, tsz, p, t, ml, md, mr;
	int i, j, k;

	sz=BGBCC_CCXLR3_ReadSVLI(ctx, rcs);
	if(!sz)
	{
		*rbuf=NULL;
		*rsz=0;
		return(0);
	}

	tbuf=*rbuf;
	tsz=*rsz;
	
	if(sz>0)
	{
		if(!tbuf || (sz>=tsz))
		{
			tbuf=bgbcc_malloc(sz+1);
			*rbuf=tbuf;
		}

		memcpy(tbuf, *rcs, sz);
		tbuf[sz]=0;
		*rsz=sz;
		*rcs=(*rcs)+sz;
		
		i=ctx->ril_psrov;
		ctx->ril_psidx[i]=ctx->ril_txrov;
		ctx->ril_pslen[i]=sz;
		ctx->ril_psrov=(i+1)&63;
		BGBCC_CCXLR3_UpdateTextString(ctx, tbuf, sz);
		
		return(0);
	}
	
	i=(-sz)-1;
	if(i<64)
	{
		j=(ctx->ril_psrov-i-1)&63;
		sz=ctx->ril_pslen[j];
		p=ctx->ril_psidx[j];

		if(!tbuf || (sz>=tsz))
		{
			tbuf=bgbcc_malloc(sz+1);
			*rbuf=tbuf;
		}
		*rsz=sz;
		
		for(k=0; k<sz; k++)
			{ tbuf[k]=ctx->ril_txwin[(p+k)&16383]; }
		tbuf[sz]=0;
		return(0);
	}
	
	csz=(-sz)-64;
	sz=BGBCC_CCXLR3_ReadSVLI(ctx, rcs);
	if(sz<=0)
		__debugbreak();

	i=ctx->ril_psrov;
	ctx->ril_psidx[i]=ctx->ril_txrov;
	ctx->ril_pslen[i]=sz;
	ctx->ril_psrov=(i+1)&63;

	if(!tbuf || (sz>=tsz))
	{
		tbuf=bgbcc_malloc(sz+1);
		*rbuf=tbuf;
	}

	*rsz=sz;

	cs=*rcs; cse=cs+csz;
	ct=tbuf; cte=tbuf+sz;
	
	while((cs<cse) && (ct<cte))
	{
		t=*cs++;
		
		if(t&0xF0)
		{
			mr=(t>>4)&15;
			if(mr>=15)
				{ mr=BGBCC_CCXLR3_ReadUVLI(ctx, &cs); }

			BGBCC_CCXLR3_UpdateTextString(ctx, cs, mr);
			for(i=0; i<mr; i++)
				*ct++=*cs++;
		}
		
		if(t&0x0F)
		{
			ml=(t&0x0F)+2;
			if(ml>=17)
				{ ml=BGBCC_CCXLR3_ReadUVLI(ctx, &cs); }
			md=BGBCC_CCXLR3_ReadUVLI(ctx, &cs);
			
			for(i=0; i<ml; i++)
			{
				j=ctx->ril_txwin[(ctx->ril_txrov-md)&16383];
				*ct++=j;
				BGBCC_CCXLR3_UpdateTextByte(ctx, j);
			}
		}
	}
	
	*ct=0;
	
	*rcs=cse;
	return(1);
	
//	__debugbreak();
//	return(-1);
}

char *BGBCC_CCXLR3_ReadString(BGBCC_TransState *ctx, byte **rcs)
{
	byte tb[4096];
	byte *s0, *s1;
	int sz;

//	if(!strncmp(tb, "/*", 2))
//		__debugbreak();

	s0=tb; sz=4096;
	BGBCC_CCXLR3_ReadTextBlob(ctx, rcs, &s0, &sz);
	
	if(!s0)
		return(NULL);

	if(!strncmp((char *)s0, "/*", 2))
		__debugbreak();
	
	s1=(byte *)bgbcc_rstrdup((char *)s0);
	if(s0!=tb)
		bgbcc_free(s0);
	return((char *)s1);

#if 0
	sz=BGBCC_CCXLR3_ReadSVLI(ctx, rcs);
	if(!sz)
		{ return(NULL); }
	
	if(sz>0)
	{
		s0=bgbcc_ralloc(sz+1);
		memcpy(s0, *rcs, sz);
		*rcs=(*rcs)+sz;
		return(s0);
	}
	
	__debugbreak();
	return(NULL);
#endif
}

ccxl_label BGBCC_CCXLR3_ReadLabel(BGBCC_TransState *ctx, byte **rcs)
{
	ccxl_label lbl;
	int i0;
	
	i0=BGBCC_CCXLR3_ReadSVLI(ctx, rcs);
	lbl=BGBCC_CCXLR3_IndexToLabel(ctx, i0);
	return(lbl);
}

char *BGBCC_CCXLR3_ReadSymbolTag(BGBCC_TransState *ctx, byte **rcs)
{
	BGBCC_CCXL_RegisterInfo *func;
	int i0, ix;
	
	func=ctx->cur_func;
	
	i0=BGBCC_CCXLR3_ReadUVLI(ctx, rcs);
	
	if(!(i0&1))
	{
		ix=i0>>3;
		
		if(((i0>>1)&3)==0)
			{ return(func->regs[ix]->name); }
		if(((i0>>1)&3)==1)
			{ return(func->args[ix]->name); }
		if(((i0>>1)&3)==2)
			{ return(func->locals[ix]->name); }
	}

	return(NULL);
}

char *BGBCC_CCXLR3_ReadSymbol(BGBCC_TransState *ctx, byte **rcs)
{
	char *sym;

	sym=BGBCC_CCXLR3_ReadString(ctx, rcs);
	if(sym)
		return(sym);
	sym=BGBCC_CCXLR3_ReadSymbolTag(ctx, rcs);
	return(sym);
}

char *BGBCC_CCXLR3_ReadSigstr(BGBCC_TransState *ctx, byte **rcs)
{
	return(BGBCC_CCXLR3_ReadString(ctx, rcs));
}

void BGBCC_CCXLR3_DecodeBufCmd(
	BGBCC_TransState *ctx, byte **rcs)
{
	static int lop, lop1, lop2, lop3;
	static int lop4, lop5, lop6, lop7;
	char tb[4096];
	s64 vala[256];
	ccxl_label lbla[256];
	BGBCC_CCXL_RegisterInfo *decl;
	byte *cs, *tbuf;
	char *s0, *s1, *s2;
	ccxl_label lbl, lbl2;
	s64 li0, li1;
	f64 f0, f1, f2, f3;
	int i0, i1, i2, i3;
	int op, tsz;
	int i, j, k;
	
	cs=*rcs;
//	op=*cs++;
	op=BGBCC_CCXLR3_ReadOpVLI(ctx, &cs);
	if(!op)
	{
		*rcs=NULL;
		return;
	}
	
	switch(op)
	{
	case BGBCC_RIL3OP_END:
		BGBCC_CCXL_End(ctx);
		break;
	case BGBCC_RIL3OP_ASMBLOB:
		tbuf=(byte *)tb; tsz=4096;
		BGBCC_CCXLR3_ReadTextBlob(ctx, &cs, &tbuf, &tsz);
		BGBCC_CCXL_AddAsmBlob(ctx, (char *)tbuf);
		if(tbuf!=((byte *)tb))
			bgbcc_free(tbuf);
		break;
	case BGBCC_RIL3OP_BEGIN:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		BGBCC_CCXL_Begin(ctx, i0);
		break;
	case BGBCC_RIL3OP_BEGINNAME:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_BeginName(ctx, i0, s0);
		break;
	case BGBCC_RIL3OP_MARKER:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		BGBCC_CCXL_Marker(ctx, i0);
		break;

	case BGBCC_RIL3OP_ATTRINT:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		li1=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_AttribInt(ctx, i0, li1);
		break;
	case BGBCC_RIL3OP_ATTRLONG:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		li1=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_AttribLong(ctx, i0, li1);
		break;
	case BGBCC_RIL3OP_ATTRSTR:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_AttribStr(ctx, i0, s1);
		break;

	case BGBCC_RIL3OP_OBJBLOB:
		tbuf=(byte *)tb; tsz=4096;
		BGBCC_CCXLR3_ReadTextBlob(ctx, &cs, &tbuf, &tsz);
		BGBCC_CCXL_AddObjectBlob(ctx, tbuf, tsz);
		if(tbuf!=((byte *)tb))
			bgbcc_free(tbuf);
		break;

	case BGBCC_RIL3OP_LITSETFID:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_LiteralSetField(ctx, i0, s1);
		break;

	case BGBCC_RIL3OP_LITINT:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		li1=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_LiteralInt(ctx, i0, li1);
		break;
	case BGBCC_RIL3OP_LITLONG:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		li1=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_LiteralLong(ctx, i0, li1);
		break;
	case BGBCC_RIL3OP_LITFLOAT:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		f1=BGBCC_CCXLR3_ReadFVLI(ctx, &cs);
		BGBCC_CCXL_LiteralFloat(ctx, i0, f1);
		break;
	case BGBCC_RIL3OP_LITDOUBLE:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		f1=BGBCC_CCXLR3_ReadFVLI(ctx, &cs);
		BGBCC_CCXL_LiteralDouble(ctx, i0, f1);
		break;
	case BGBCC_RIL3OP_LITSTR:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_LiteralStr(ctx, i0, s1);
		break;
	case BGBCC_RIL3OP_LITWSTR:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_LiteralWStr(ctx, i0, s1);
		break;
	case BGBCC_RIL3OP_LITNAME:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadString(ctx, &cs);
//		decl=BGBCC_CCXL_LookupGlobal(ctx, s1);
		decl=BGBCC_CCXL_GetGlobal(ctx, s1);
		BGBCC_CCXL_LiteralGlobalAddr(ctx, i0, decl->gblid);
		break;

	case BGBCC_RIL3OP_MARK:
		BGBCC_CCXL_PushMark(ctx);
		break;
	case BGBCC_RIL3OP_LOAD:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_PushLoad(ctx, s0);
		break;
	case BGBCC_RIL3OP_STORE:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_PopStore(ctx, s0);
		break;
	case BGBCC_RIL3OP_MOVLDST:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_MovLoadStore(ctx, s0, s1);
		break;

	case BGBCC_RIL3OP_CALLN:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_StackCallName(ctx, s0, 0);
		break;
	case BGBCC_RIL3OP_CALLP:
		BGBCC_CCXL_StackPopCall(ctx, 0);
		break;
	case BGBCC_RIL3OP_CALLNV:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_StackCallName(ctx, s0, 1);
		break;
	case BGBCC_RIL3OP_CALLPV:
		BGBCC_CCXL_StackPopCall(ctx, 1);
		break;
	case BGBCC_RIL3OP_STCALLN:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_StackCallName2(ctx, s0, s1, 0);
		break;
	case BGBCC_RIL3OP_STCALLP:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_StackPopCall2(ctx, s0, 0);
		break;

	case BGBCC_RIL3OP_OBJCALLN:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_StackCallName(ctx, s0, 4);
		break;
	case BGBCC_RIL3OP_OBJCALLP:
		BGBCC_CCXL_StackPopCall(ctx, 4);
		break;
	case BGBCC_RIL3OP_OBJCALLNV:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_StackCallName(ctx, s0, 5);
		break;
	case BGBCC_RIL3OP_OBJCALLPV:
		BGBCC_CCXL_StackPopCall(ctx, 5);
		break;
	case BGBCC_RIL3OP_STOBJCALLN:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_StackCallName2(ctx, s0, s1, 4);
		break;
	case BGBCC_RIL3OP_STOBJCALLP:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_StackPopCall2(ctx, s0, 4);
		break;

	case BGBCC_RIL3OP_LDIXC:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_StackLoadIndexConst(ctx, i0);
		break;
	case BGBCC_RIL3OP_STIXC:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_StackStoreIndexConst(ctx, i0);
		break;
	case BGBCC_RIL3OP_LDIXAC:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_StackLoadIndexAddrConst(ctx, i0);
		break;
	case BGBCC_RIL3OP_LDIX:
		BGBCC_CCXL_StackLoadIndex(ctx);
		break;
	case BGBCC_RIL3OP_STIX:
		BGBCC_CCXL_StackStoreIndex(ctx);
		break;
	case BGBCC_RIL3OP_LDIXA:
		BGBCC_CCXL_StackLoadIndexAddr(ctx);
		break;
	case BGBCC_RIL3OP_DUP:
		BGBCC_CCXL_StackDup(ctx);
		break;
	case BGBCC_RIL3OP_DUPCLEAN:
		BGBCC_CCXL_StackDupClean(ctx);
		break;
	case BGBCC_RIL3OP_RETV:
		BGBCC_CCXL_StackRetV(ctx);
		break;
	case BGBCC_RIL3OP_RET:
		BGBCC_CCXL_StackRet(ctx);
		break;
	case BGBCC_RIL3OP_CASTBOOL:
		BGBCC_CCXL_StackCastBool(ctx);
		break;
	case BGBCC_RIL3OP_CASTSIG:
		s0=BGBCC_CCXLR3_ReadSigstr(ctx, &cs);
		BGBCC_CCXL_StackCastSig(ctx, s0);
		break;

	case BGBCC_RIL3OP_BINOP:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		switch(i0)
		{
		case CCXL_BINOP_ADD: s0="+"; break;
		case CCXL_BINOP_SUB: s0="-"; break;
		case CCXL_BINOP_MUL: s0="*"; break;
		case CCXL_BINOP_DIV: s0="/"; break;
		case CCXL_BINOP_MOD: s0="%"; break;
		case CCXL_BINOP_AND: s0="&"; break;
		case CCXL_BINOP_OR: s0="|"; break;
		case CCXL_BINOP_XOR: s0="^"; break;
		case CCXL_BINOP_SHL: s0="<<"; break;
		case CCXL_BINOP_SHR: s0=">>"; break;
		case CCXL_BINOP_SHRR: s0=">>>"; break;

		case CCXL_BINOP_MULH: s0="*^"; break;
		case CCXL_BINOP_UMULH: s0="*^^"; break;
		case CCXL_BINOP_UDIV: s0="/^"; break;
		case CCXL_BINOP_CONS: s0="CONS"; break;

		default: s0=NULL; BGBCC_DBGBREAK; break;
		}
		BGBCC_CCXL_StackBinaryOp(ctx, s0);
		break;

	case BGBCC_RIL3OP_CMPOP:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		switch(i0)
		{
		case CCXL_CMP_EQ: s0="=="; break;
		case CCXL_CMP_NE: s0="!="; break;
		case CCXL_CMP_LT: s0="<"; break;
		case CCXL_CMP_GT: s0=">"; break;
		case CCXL_CMP_LE: s0="<="; break;
		case CCXL_CMP_GE: s0=">="; break;
		case CCXL_CMP_TST:		s0="!!&"; break;
		case CCXL_CMP_NTST:		s0="!&"; break;
		default: s0=NULL; BGBCC_DBGBREAK; break;
		}
		BGBCC_CCXL_StackBinaryOp(ctx, s0);
		break;

	case BGBCC_RIL3OP_UNOP:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		switch(i0)
		{
		case CCXL_UNOP_MOV: s0="+"; break;
		case CCXL_UNOP_NEG: s0="-"; break;
		case CCXL_UNOP_NOT: s0="~"; break;
		case CCXL_UNOP_LNOT: s0="!"; break;
		case CCXL_UNOP_INC: s0="++"; break;
		case CCXL_UNOP_DEC: s0="--"; break;
		case CCXL_UNOP_LNOT2: s0="!!"; break;
		default: s0=NULL; BGBCC_DBGBREAK; break;
		}
		BGBCC_CCXL_StackUnaryOp(ctx, s0);
		break;

	case BGBCC_RIL3OP_TRINOP:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		switch(i0)
		{
		case CCXL_BINOP_MAC: s0="MAC"; break;
		default: s0=NULL; BGBCC_DBGBREAK; break;
		}
		BGBCC_CCXL_StackTrinaryOp(ctx, s0);
		break;


	case BGBCC_RIL3OP_STBINOP:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		switch(i0)
		{
		case CCXL_BINOP_ADD: s0="+"; break;
		case CCXL_BINOP_SUB: s0="-"; break;
		case CCXL_BINOP_MUL: s0="*"; break;
		case CCXL_BINOP_DIV: s0="/"; break;
		case CCXL_BINOP_MOD: s0="%"; break;
		case CCXL_BINOP_AND: s0="&"; break;
		case CCXL_BINOP_OR: s0="|"; break;
		case CCXL_BINOP_XOR: s0="^"; break;
		case CCXL_BINOP_SHL: s0="<<"; break;
		case CCXL_BINOP_SHR: s0=">>"; break;
		case CCXL_BINOP_SHRR: s0=">>>"; break;

		case CCXL_BINOP_MULH: s0="*^"; break;
		case CCXL_BINOP_UMULH: s0="*^^"; break;
		case CCXL_BINOP_UDIV: s0="/^"; break;
		case CCXL_BINOP_CONS: s0="CONS"; break;
		default: s0=NULL; BGBCC_DBGBREAK; break;
		}
		BGBCC_CCXL_StackBinaryOpStore(ctx, s0, s1);
		break;

	case BGBCC_RIL3OP_STCMPOP:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		switch(i0)
		{
		case CCXL_CMP_EQ: s0="=="; break;
		case CCXL_CMP_NE: s0="!="; break;
		case CCXL_CMP_LT: s0="<"; break;
		case CCXL_CMP_GT: s0=">"; break;
		case CCXL_CMP_LE: s0="<="; break;
		case CCXL_CMP_GE: s0=">="; break;
		case CCXL_CMP_TST:		s0="!!&"; break;
		case CCXL_CMP_NTST:		s0="!&"; break;
		default: s0=NULL; BGBCC_DBGBREAK; break;
		}
		BGBCC_CCXL_StackBinaryOpStore(ctx, s0, s1);
		break;
	case BGBCC_RIL3OP_LDUNOP:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		switch(i0&7)
		{
		case CCXL_UNOP_MOV: s0="+"; break;
		case CCXL_UNOP_NEG: s0="-"; break;
		case CCXL_UNOP_NOT: s0="~"; break;
		case CCXL_UNOP_LNOT: s0="!"; break;
		case CCXL_UNOP_INC: s0="++"; break;
		case CCXL_UNOP_DEC: s0="--"; break;
		case CCXL_UNOP_LNOT2: s0="!!"; break;
		default: s0=NULL; BGBCC_DBGBREAK; break;
		}
//		BGBCC_CCXL_StackUnaryOpName(ctx, s0, s1);
		BGBCC_CCXL_StackUnaryOpNameB(ctx, s0, s1, i0/16);
		break;
	case BGBCC_RIL3OP_STUNOP:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		switch(i0)
		{
		case CCXL_UNOP_MOV: s0="+"; break;
		case CCXL_UNOP_NEG: s0="-"; break;
		case CCXL_UNOP_NOT: s0="~"; break;
		case CCXL_UNOP_LNOT: s0="!"; break;
		case CCXL_UNOP_INC: s0="++"; break;
		case CCXL_UNOP_DEC: s0="--"; break;
		case CCXL_UNOP_LNOT2: s0="!!"; break;
		default: s0=NULL; BGBCC_DBGBREAK; break;
		}
		BGBCC_CCXL_StackUnaryOpStore(ctx, s0, s1);
		break;

	case BGBCC_RIL3OP_STTRINOP:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		switch(i0)
		{
		case CCXL_BINOP_MAC: s0="MAC"; break;
		default: s0=NULL; BGBCC_DBGBREAK; break;
		}
		BGBCC_CCXL_StackTrinaryOpStore(ctx, s0, s1);
		break;

	case BGBCC_RIL3OP_LDCONSTV:
		BGBCC_CCXL_StackPushVoid(ctx);
		break;
	case BGBCC_RIL3OP_LDCONSTI:
		li0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		if(((s32)li0)==li0)
			{ BGBCC_CCXL_StackPushConstInt(ctx, li0); break; }
		if(((u32)li0)==li0)
			{ BGBCC_CCXL_StackPushConstUInt(ctx, li0); break; }
		BGBCC_DBGBREAK
		break;
	case BGBCC_RIL3OP_LDCONSTL:
		li0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_StackPushConstLong(ctx, li0);
		break;

	case BGBCC_RIL3OP_LDCONSTSMI:
		li0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		i1=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		if((i1>=4) && (i1<=7))
			{ BGBCC_CCXL_StackPushConstSmallInt(ctx, li0, i1); break; }
		if(i1==0)
			{ BGBCC_CCXL_StackPushConstInt(ctx, li0); break; }
		if(i1==1)
			{ BGBCC_CCXL_StackPushConstUInt(ctx, li0); break; }
		if(i1==2)
			{ BGBCC_CCXL_StackPushConstLong(ctx, li0); break; }
		if(i1==3)
			{ BGBCC_CCXL_StackPushConstULong(ctx, li0); break; }
		if((i1>=8) && (i1<=63))
			{ BGBCC_CCXL_StackPushConstVecLi(ctx, li0, i1); break; }
		BGBCC_DBGBREAK
		break;

	case BGBCC_RIL3OP_LDCONSTVXI:
		li0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		li1=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		i1=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_StackPushConstVecXi(ctx, li0, li1, i1);
		break;

	case BGBCC_RIL3OP_LDCONSTF:
		f0=BGBCC_CCXLR3_ReadFVLI(ctx, &cs);
		BGBCC_CCXL_StackPushConstFloat(ctx, f0);
		break;
	case BGBCC_RIL3OP_LDCONSTD:
		f0=BGBCC_CCXLR3_ReadFVLI(ctx, &cs);
		BGBCC_CCXL_StackPushConstDouble(ctx, f0);
		break;
	case BGBCC_RIL3OP_LDCONSTS:
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_StackPushConstString(ctx, s0);
		break;
	case BGBCC_RIL3OP_LDCONSTWS:
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_StackPushConstWString(ctx, s0);
		break;

	case BGBCC_RIL3OP_LDCONSTXL:
		li0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		li1=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_StackPushConstInt128(ctx, li0, li1);
		break;
	case BGBCC_RIL3OP_LDCONSTXF:
		BGBCC_CCXLR3_ReadXFVLI(ctx, &cs, &li0, &li1);
//		BGBCC_CCXL_StackPushConstDouble(ctx, f0);
		BGBCC_CCXL_StackPushConstFloat128(ctx, li0, li1);
		break;

	case BGBCC_RIL3OP_STCONSTI:
		li0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);

		if(((s32)li0)==li0)
			{ BGBCC_CCXL_ConstIntStore(ctx, li0, s0); break; }
		if(((u32)li0)==li0)
			{ BGBCC_CCXL_ConstUIntStore(ctx, li0, s0); break; }

		BGBCC_DBGBREAK
		break;

#if 0
	case BGBCC_RIL3OP_STREFREF:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_RefRefStore(ctx, s0, s1);
		break;
#endif

	case BGBCC_RIL3OP_POP:
		BGBCC_CCXL_StackPop(ctx);
		break;
	case BGBCC_RIL3OP_LDA:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_StackLoadAddr(ctx, s0);
		break;
	case BGBCC_RIL3OP_SIZEOFSG:
		s0=BGBCC_CCXLR3_ReadSigstr(ctx, &cs);
		BGBCC_CCXL_StackSizeofSig(ctx, s0);
		break;
	case BGBCC_RIL3OP_SIZEOFVAL:
		BGBCC_CCXL_StackSizeofVal(ctx);
		break;
	case BGBCC_RIL3OP_OFFSETOF:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_StackOffsetof(ctx, s0, s1);
		break;
	case BGBCC_RIL3OP_LOADSLOT:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_StackLoadSlot(ctx, s0);
		break;
	case BGBCC_RIL3OP_STORESLOT:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_StackStoreSlot(ctx, s0);
		break;
	case BGBCC_RIL3OP_LOADSLOTA:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_StackLoadSlotAddr(ctx, s0);
		break;

	case BGBCC_RIL3OP_BEGINU:
		s0=BGBCC_CCXLR3_ReadSigstr(ctx, &cs);
		BGBCC_CCXL_StackBeginU(ctx, s0);
		break;
	case BGBCC_RIL3OP_ENDU:
		BGBCC_CCXL_StackEndU(ctx);
		break;
	case BGBCC_RIL3OP_SETU:
		BGBCC_CCXL_StackSetU(ctx);
		break;
	case BGBCC_RIL3OP_INITVAR:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_StackInitVar(ctx, s0);
		break;
	case BGBCC_RIL3OP_INITVARVAL:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_StackInitVarValue(ctx, s0);
		break;

	case BGBCC_RIL3OP_JMP:
		lbl=BGBCC_CCXLR3_ReadLabel(ctx, &cs);
		BGBCC_CCXL_CompileJmp(ctx, lbl);
		break;
	case BGBCC_RIL3OP_JMPT:
		lbl=BGBCC_CCXLR3_ReadLabel(ctx, &cs);
		BGBCC_CCXL_CompileJmpTrue(ctx, lbl);
		break;
	case BGBCC_RIL3OP_JMPF:
		lbl=BGBCC_CCXLR3_ReadLabel(ctx, &cs);
		BGBCC_CCXL_CompileJmpFalse(ctx, lbl);
		break;
	case BGBCC_RIL3OP_JCMP:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		lbl=BGBCC_CCXLR3_ReadLabel(ctx, &cs);
		switch(i0)
		{
		case CCXL_CMP_EQ: s0="=="; break;
		case CCXL_CMP_NE: s0="!="; break;
		case CCXL_CMP_LT: s0="<"; break;
		case CCXL_CMP_GT: s0=">"; break;
		case CCXL_CMP_LE: s0="<="; break;
		case CCXL_CMP_GE: s0=">="; break;
		case CCXL_CMP_TST:		s0="!!&"; break;
		case CCXL_CMP_NTST:		s0="!&"; break;
		default: s0=NULL; BGBCC_DBGBREAK; break;
		}

		BGBCC_CCXL_CompileJmpCond(ctx, s0, lbl);
		break;

	case BGBCC_RIL3OP_LABEL:
		lbl=BGBCC_CCXLR3_ReadLabel(ctx, &cs);
		BGBCC_CCXL_EmitLabel(ctx, lbl);
		break;
	case BGBCC_RIL3OP_LABELLVL:
		lbl=BGBCC_CCXLR3_ReadLabel(ctx, &cs);
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_EmitLabelLvl(ctx, lbl, i0);
		break;

	case BGBCC_RIL3OP_STKFN:
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		ctx->lfn=s0;
		BGBCC_CCXL_StackFn(ctx, s0);
		break;
	case BGBCC_RIL3OP_STKLN:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		ctx->lln=i0;
		BGBCC_CCXL_StackLn(ctx, i0);
		break;

	case BGBCC_RIL3OP_LITTYPESIG:
		s0=BGBCC_CCXLR3_ReadSigstr(ctx, &cs);
		BGBCC_CCXL_StackLitTypeSig(ctx, s0);
		break;
	case BGBCC_RIL3OP_VA_START:
		BGBCC_CCXL_StackVaStart(ctx);
		break;
	case BGBCC_RIL3OP_VA_END:
		BGBCC_CCXL_StackVaEnd(ctx);
		break;
	case BGBCC_RIL3OP_VA_ARG:
		BGBCC_CCXL_StackVaArg(ctx);
		break;
		
	case BGBCC_RIL3OP_DI_3AC:
		BGBCC_CCXL_StackDisable3AC(ctx);
		break;
	case BGBCC_RIL3OP_EN_3AC:
		BGBCC_CCXL_StackEnable3AC(ctx);
		break;

	case BGBCC_RIL3OP_ADD:	BGBCC_CCXL_StackBinaryOp(ctx, "+"); break;
	case BGBCC_RIL3OP_SUB:	BGBCC_CCXL_StackBinaryOp(ctx, "-"); break;
	case BGBCC_RIL3OP_MUL:	BGBCC_CCXL_StackBinaryOp(ctx, "*"); break;
	case BGBCC_RIL3OP_AND:	BGBCC_CCXL_StackBinaryOp(ctx, "&"); break;
	case BGBCC_RIL3OP_OR:	BGBCC_CCXL_StackBinaryOp(ctx, "|"); break;
	case BGBCC_RIL3OP_XOR:	BGBCC_CCXL_StackBinaryOp(ctx, "^"); break;
	case BGBCC_RIL3OP_SHL:	BGBCC_CCXL_StackBinaryOp(ctx, "<<"); break;
	case BGBCC_RIL3OP_SAR:	BGBCC_CCXL_StackBinaryOp(ctx, ">>"); break;

	case BGBCC_RIL3OP_STLDSLOT:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_StackLoadSlotStore(ctx, s0, s1);
		break;
	case BGBCC_RIL3OP_STLDSLOTA:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_StackLoadSlotAddrStore(ctx, s0, s1);
		break;

	case BGBCC_RIL3OP_STLDIXC:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_StackLoadIndexConstStore(ctx, i0, s0);
		break;
	case BGBCC_RIL3OP_STLDIXAC:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
//		BGBCC_CCXL_StackLoadIndexAddrConstStore(ctx, i0, s0);
		break;
	case BGBCC_RIL3OP_STLDIX:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_StackLoadIndexStore(ctx, s0);
		break;
	case BGBCC_RIL3OP_STLDIXA:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
//		BGBCC_CCXL_StackLoadIndexAddrStore(ctx, s0);
		break;
	case BGBCC_RIL3OP_STCASTSIG:
		s0=BGBCC_CCXLR3_ReadSigstr(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_StackCastSigStore(ctx, s0, s1);
		break;

	case BGBCC_RIL3OP_CSELCMP:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		switch(i0&15)
		{
		case CCXL_CMP_EQ:		s0="=="; break;
		case CCXL_CMP_NE:		s0="!="; break;
		case CCXL_CMP_LT:		s0="<"; break;
		case CCXL_CMP_GT:		s0=">"; break;
		case CCXL_CMP_LE:		s0="<="; break;
		case CCXL_CMP_GE:		s0=">="; break;
		case CCXL_CMP_TST:		s0="!!&"; break;
		case CCXL_CMP_NTST:		s0="!&"; break;
		default:				s0=NULL; BGBCC_DBGBREAK; break;
		}
		switch(i0/16)
		{
		case 0:		BGBCC_CCXL_StackCSelCmp(ctx, s0); break;
		case 1:		BGBCC_CCXL_StackCSelCmpZero(ctx, s0); break;
		default:	BGBCC_DBGBREAK; break;
		}
		break;

	case BGBCC_RIL3OP_DUPIX:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_StackDupIdx(ctx, i0);
		break;
	case BGBCC_RIL3OP_DUPMIX:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_StackDupMarkIdx(ctx, i0);
		break;

	case BGBCC_RIL3OP_JMPTAB:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		lbl=BGBCC_CCXLR3_ReadLabel(ctx, &cs);

		lbl2=lbl;
		if(i0<0)
			{ lbl.id=0; i0=-i0; }

		for(i=0; i<i0; i++)
		{
			vala[i]=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
			lbla[i]=BGBCC_CCXLR3_ReadLabel(ctx, &cs);
		}

		BGBCC_CCXL_StackCompileJmpTab(ctx, 0, i0, lbla, vala, lbl, lbl2);

//		BGBCC_CCXL_StackDupIdx(ctx, i0);

		break;

	case BGBCC_RIL3OP_MKTMPOBJ:
		s0=BGBCC_CCXLR3_ReadSigstr(ctx, &cs);
		BGBCC_CCXL_StackPushTempObj(ctx, s0);
		break;
	case BGBCC_RIL3OP_EXCH:
		BGBCC_CCXL_StackExch(ctx);
		break;
	case BGBCC_RIL3OP_LDSLOTSIG:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_StackLoadSlotSig(ctx, s0, s1);
		break;
	
	case BGBCC_RIL3OP_SETPRED:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_StackSetPred(ctx, i0);
		break;
	case BGBCC_RIL3OP_PRCMP:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		switch(i0)
		{
		case CCXL_CMP_EQ:		s0="=="; break;
		case CCXL_CMP_NE:		s0="!="; break;
		case CCXL_CMP_LT:		s0="<"; break;
		case CCXL_CMP_GT:		s0=">"; break;
		case CCXL_CMP_LE:		s0="<="; break;
		case CCXL_CMP_GE:		s0=">="; break;
		case CCXL_CMP_TST:		s0="!!&"; break;
		case CCXL_CMP_NTST:		s0="!&"; break;
		default: s0=NULL; BGBCC_DBGBREAK; break;
		}
		BGBCC_CCXL_CompilePredCmp(ctx, s0);
		break;

	case BGBCC_RIL3OP_LDCONSTCPX:
		f0=BGBCC_CCXLR3_ReadFVLI(ctx, &cs);
		f1=BGBCC_CCXLR3_ReadFVLI(ctx, &cs);
		BGBCC_CCXL_StackPushConstComplex(ctx, f0, f1);
		break;

	case BGBCC_RIL3OP_LDCONSTV2F:
		f0=BGBCC_CCXLR3_ReadFVLI(ctx, &cs);
		f1=BGBCC_CCXLR3_ReadFVLI(ctx, &cs);
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_StackPushConstVec2F(ctx, f0, f1, i0);
		break;
	case BGBCC_RIL3OP_LDCONSTV4F:
		f0=BGBCC_CCXLR3_ReadFVLI(ctx, &cs);
		f1=BGBCC_CCXLR3_ReadFVLI(ctx, &cs);
		f2=BGBCC_CCXLR3_ReadFVLI(ctx, &cs);
		f3=BGBCC_CCXLR3_ReadFVLI(ctx, &cs);
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_StackPushConstVec4F(ctx, f0, f1, f2, f3, i0);
		break;

	case BGBCC_RIL3OP_LDCONSTU8S:
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_StackPushConstU8String(ctx, s0);
		break;
	case BGBCC_RIL3OP_LDCONSTW4S:
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_StackPushConstW4String(ctx, s0);
		break;

	case BGBCC_RIL3OP_LDCONSTBI:
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_StackPushConstBigInt(ctx, s0, i0);
		break;
		
	case BGBCC_RIL3OP_INLINEASM:
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_InlineAsmBlob(ctx, s0);
		break;

	case BGBCC_RIL3OP_LDAVSIG:
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_StackLoadAddrVSig(ctx, s0, s1);
		break;

	case BGBCC_RIL3OP_TOPLFN:
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		ctx->lfn=bgbcc_strdup(s0);
		ctx->lln=i0;
		break;

	case BGBCC_RIL3OP_BITLOAD:
		BGBCC_CCXLR3_ReadUPairVLI(ctx, &cs, &i0, &i1);
		BGBCC_CCXL_StackBitLoad(ctx, i0, i1);
		break;
	case BGBCC_RIL3OP_BITSTORE:
		BGBCC_CCXLR3_ReadUPairVLI(ctx, &cs, &i0, &i1);
		BGBCC_CCXL_StackBitStore(ctx, i0, i1);
		break;
	case BGBCC_RIL3OP_BITMOVE:
		BGBCC_CCXLR3_ReadUPairVLI(ctx, &cs, &i0, &i1);
		BGBCC_CCXLR3_ReadUPairVLI(ctx, &cs, &i2, &i3);
		BGBCC_CCXL_StackBitMove(ctx, i0, i1, i2, i3);
		break;

	case BGBCC_RIL3OP_STBITSTORE:
		BGBCC_CCXLR3_ReadUPairVLI(ctx, &cs, &i0, &i1);
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_StackBitStoreRef(ctx, i0, i1, s0);
		break;
	case BGBCC_RIL3OP_STBITMOVE:
		BGBCC_CCXLR3_ReadUPairVLI(ctx, &cs, &i0, &i1);
		BGBCC_CCXLR3_ReadUPairVLI(ctx, &cs, &i2, &i3);
		s0=BGBCC_CCXLR3_ReadSymbol(ctx, &cs);
		BGBCC_CCXL_StackBitMoveRef(ctx, i0, i1, i2, i3, s0);
		break;

	default:
		__debugbreak();
		break;
	}
	
	lop7=lop6;
	lop6=lop5;
	lop5=lop4;
	lop4=lop3;
	lop3=lop2;
	lop2=lop1;
	lop1=lop;
	lop=op;
	
	*rcs=cs;
}

int BGBCC_CCXLR3_CheckCanLoadNow(
	BGBCC_TransState *ctx, byte *buf, int bufsz)
{
	if(ctx->ril3_norec)
		return(0);
	return(1);
}

void BGBCC_CCXLR3_LoadBufferRIL(
	BGBCC_TransState *ctx, byte *buf, int bufsz)
{
	BGBCC_CCXL_LiteralInfo *obj;
	BGBCC_CCXL_RegisterInfo *gbl;
	byte *cs;
	byte *rscs, *rscse;
	byte *rtcs, *rtcse;
	byte *rdcs, *rdcse;
	byte *cse;
	int onl, rtsize, rdsize, sz;
	int i, j, k;

	if(!buf)
		return;
	
	if(ctx->ril3_norec)
	{
		__debugbreak();
		return;
	}

	if(!memcmp(buf, "RIL3", 4))
	{
		ctx->ril3_norec=1;
		onl=ctx->n_literals;

		ril3_nlbl=0;
		BGBCC_CCXLR3_ClearLabels(ctx);
		
		BGBCC_CCXLR3_CheckLzWindow(ctx);
		
		cs=buf; cse=buf+bufsz;
		cs+=8;
		while(cs && cs<cse)
			{ BGBCC_CCXLR3_DecodeBufCmd(ctx, &cs); }

		ctx->ril3_norec=0;

	//	for(i=0; i<ctx->n_literals; i++)
		for(i=onl; i<ctx->n_literals; i++)
		{
			obj=ctx->literals[i];
			if(!obj)
				continue;
			if(obj->littype!=CCXL_LITID_MANIFOBJ)
				continue;
			if(obj->decl->regflags&BGBCC_REGFL_DEMANDLOAD)
			{
				obj->decl->regflags&=~BGBCC_REGFL_DEMANDLOAD;
				obj->decl->regflags|=BGBCC_REGFL_LOADED;
				BGBCC_CCXLR3_LoadBufferRIL(ctx,
					obj->decl->text, obj->decl->sz_text);
			}
		}
	}

	if(	!memcmp(buf+0, "RIFF", 4) &&
		!memcmp(buf+8, "RIL3", 4))
	{
		ctx->ril3_norec=1;
		onl=ctx->n_literals;

		ril3_nlbl=0;
		BGBCC_CCXLR3_ClearLabels(ctx);

		BGBCC_CCXLR3_CheckLzWindow(ctx);
		
		rtsize=*(u32 *)(buf+4);
		rtcs=buf+12;
		rtcse=buf+8+((rtsize+1)&(~1));
		
		rdcs=NULL;
		rscs=NULL;
		while(rtcs<rtcse)
		{
			if(!memcmp(rtcs+0, "strs", 4))
			{
				j=*(u32 *)(rtcs+4);
				rscs=rtcs+12;
				rscse=rtcs+8+((j+1)&(~1));
				break;
			}

			if(	!memcmp(rtcs+0, "LIST", 4) &&
				!memcmp(rtcs+8, "r3bc", 4))
			{
				rdsize=*(u32 *)(rtcs+4);
				rdcs=rtcs+12;
				rdcse=rtcs+8+((rdsize+1)&(~1));
				break;
			}
			
			j=*(u32 *)(rtcs+4);
			j=(j+1)&(~1);
			rtcs+=j;
		}
		
		if(!rdcs)
			return;

		while(rdcs<rdcse)
		{
			if(!memcmp(rdcs+0, "blob", 4))
			{
				sz=*(u32 *)(rdcs+4);
				cs=rdcs+8;
				cse=rdcs+8+((sz+1)&(~1));

				if(rscs)
				{
					j=rscse-rscs;
					if(j>16380)
						j=16380;
					memcpy(ctx->ril_txwin, rscs, j);
					ctx->ril_txrov=j;
				}

				while(cs && cs<cse)
					{ BGBCC_CCXLR3_DecodeBufCmd(ctx, &cs); }
			}
			
			j=*(u32 *)(rdcs+4);
			j=(j+1)&(~1);
			rdcs+=j;
		}

		ctx->ril3_norec=0;

	//	for(i=0; i<ctx->n_literals; i++)
		for(i=onl; i<ctx->n_literals; i++)
		{
			obj=ctx->literals[i];
			if(!obj)
				continue;
			if(obj->littype!=CCXL_LITID_MANIFOBJ)
				continue;
			if(obj->decl->regflags&BGBCC_REGFL_DEMANDLOAD)
			{
				obj->decl->regflags&=~BGBCC_REGFL_DEMANDLOAD;
				obj->decl->regflags|=BGBCC_REGFL_LOADED;
				BGBCC_CCXLR3_LoadBufferRIL(ctx,
					obj->decl->text, obj->decl->sz_text);
			}
		}
		
		return;
	}
}

void BGBCC_CCXL_LoadBufferObject(
	BGBCC_TransState *ctx, byte *buf, int bufsz)
{
	if(!memcmp(buf, "RIL3", 4))
	{
		BGBCC_CCXLR3_LoadBufferRIL(ctx, buf, bufsz);
		return;
	}

	if(	!memcmp(buf+0, "RIFF", 4) &&
		!memcmp(buf+8, "RIL3", 4))
	{
		BGBCC_CCXLR3_LoadBufferRIL(ctx, buf, bufsz);
		return;
	}

	if(!memcmp(buf, "\x7F" "ELF", 4))
	{
		BGBCC_CCXL_AddObjectBlob(ctx, buf, bufsz);
		return;
	}
}
