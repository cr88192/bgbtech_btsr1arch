/*
FRBC2E
*/

byte *BGBCC_FR2E_BufEmitUVli(byte *dct, u64 val)
{
	byte *ct;
	int sz1;
	
	ct=dct;
	
	if(val<0x80)
	{
		*ct++=val;
	}else if(val<0x4000)
	{
		*ct++=0x80|(val>>8);
		*ct++=val;
	}else if(val<0x200000)
	{
		*ct++=0xC0|(val>>16);
		*ct++=val>>8;
		*ct++=val;
	}else if(val<0x10000000)
	{
		*ct++=0xE0|(val>>24);
		*ct++=val>>16;
		*ct++=val>>8;
		*ct++=val;
	}else if(val<0x800000000ULL)
	{
		*ct++=0xF0|(val>>32);
		*ct++=val>>24;
		*ct++=val>>16;
		*ct++=val>>8;
		*ct++=val;
	}else if(val<0x40000000000ULL)
	{
		*ct++=0xF8|(val>>40);
		*ct++=val>>32;
		*ct++=val>>24;
		*ct++=val>>16;
		*ct++=val>>8;
		*ct++=val;
	}else if(val<0x2000000000000ULL)
	{
		*ct++=0xFC|(val>>48);
		*ct++=val>>40;
		*ct++=val>>32;
		*ct++=val>>24;
		*ct++=val>>16;
		*ct++=val>>8;
		*ct++=val;
	}else if(val<0x100000000000000ULL)
	{
		*ct++=0xFE;
		*ct++=val>>48;
		*ct++=val>>40;
		*ct++=val>>32;
		*ct++=val>>24;
		*ct++=val>>16;
		*ct++=val>>8;
		*ct++=val;
	}else
	{
		*ct++=0xFF;
//		*ct++=0x00;
		*ct++=val>>56;
		*ct++=val>>48;
		*ct++=val>>40;
		*ct++=val>>32;
		*ct++=val>>24;
		*ct++=val>>16;
		*ct++=val>>8;
		*ct++=val;
	}
	return(ct);
}

byte *BGBCC_FR2E_BufEmitSVli(byte *dct, s64 val)
{
	u64 v1;
	v1=(val<<1)^(val>>63);
	return(BGBCC_FR2E_BufEmitUVli(dct, v1));
}

void BGBCC_FR2E_BufEmitTwocc(byte **rct, u16 tag, byte *dat, int sz)
{
	byte *ct;
	int sz1;
	
//	if((((byte)tag)==tag) && (sz<30))
//	{
//		BGBCC_FR2E_BufEmitOnecc(rct, tag, dat, sz);
//		return;
//	}

	if((((byte)tag)==tag) && (sz<127))
	{
		BGBCC_FR2E_BufEmitOnecc(rct, tag, dat, sz);
		return;
	}
	
	ct=*rct;
	
	sz1=sz+4;
//	if(sz1<16384)
	if(sz<32767)
	{
		ct[0]=(tag>>0)&0x7F;
		ct[1]=(tag>>8)&0x7F;
		ct[2]=((~sz)>>0)&0xFF;
		ct[3]=((~sz)>>8)&0xFF;

//		bgbcc_setu16le(ct+0, (sz1<<2)|0);
//		bgbcc_setu16le(ct+2, tag);
		if(dat)
			memcpy(ct+4, dat, sz);
		else
			memset(ct+4, 0, sz);
		*rct=ct+sz1;
		return;
	}

	BGBCC_FR2E_BufEmitFourcc(rct, tag, dat, sz);

#if 0
	sz1=sz+6;
	if(sz1<(1<<30))
	{
		bgbcc_setu32le(ct+0, (sz1<<2)|1);
		bgbcc_setu16le(ct+4, tag);
		if(dat)
			memcpy(ct+6, dat, sz);
		else
			memset(ct+6, 0, sz);
		*rct=ct+sz1;
		return;
	}
	
	BGBCC_DBGBREAK
#endif
}

void BGBCC_FR2E_BufEmitOnecc(byte **rct, byte tag, byte *dat, int sz)
{
	byte *ct;
	int sz1;
	
	ct=*rct;
	
	sz1=sz+2;
//	if(sz1<32)
	if(sz1<127)
	{
		ct[0]=tag;
		ct[1]=~sz;

//		bgbcc_setu16le(ct+0, (tag<<8)|(sz1<<3)|3);
		if(dat)
			memcpy(ct+2, dat, sz);
		else
			memset(ct+2, 0, sz);
		*rct=ct+sz1;
		return;
	}
	
	BGBCC_FR2E_BufEmitTwocc(rct, tag, dat, sz);
}

void BGBCC_FR2E_BufEmitFourcc(byte **rct, u32 tag, byte *dat, int sz)
{
	byte *ct;
	int sz1;
	
//	if(((u16)tag)==tag)
	if((((u16)tag)==tag) && (sz<32767))
	{
		BGBCC_FR2E_BufEmitTwocc(rct, tag, dat, sz);
		return;
	}
	
	ct=*rct;
	
	sz1=sz+8;
	if(sz1<(1<<30))
	{
//		bgbcc_setu32le(ct+0, (sz1<<2)|2);
//		bgbcc_setu32le(ct+4, tag);

		ct[0]=(tag>> 0)&0x7F;
		ct[1]=(tag>> 8)&0x7F;
		ct[3]=(tag>>16)&0x7F;
		ct[4]=(tag>>24)&0x7F;
		ct[5]=((~sz)>> 0)&0xFF;
		ct[6]=((~sz)>> 8)&0xFF;
		ct[7]=((~sz)>>16)&0xFF;
		ct[8]=((~sz)>>24)&0xFF;

		if(dat)
			memcpy(ct+8, dat, sz);
		else
			memset(ct+8, 0, sz);
		*rct=ct+sz1;
		return;
	}

	BGBCC_DBGBREAK
}

int BGBCC_FR2E_StrHash(char *str)
{
	char *s;
	int i, h, n;
	
	h=0; n=16; s=str;
	while((i=*s++) && (n--))
		{ h=h*251+i; }
	i=((h*251)>>8)&255;
	return(i);
}

int BGBCC_FR2E_LookupString(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img,
	char *str)
{
	char *s1;
	int i, h;

	if(!str)
		return(0);
	if(!(*str))
		return(1);

	h=BGBCC_FR2E_StrHash(str);
	i=img->strhash[h];
	while(i>0)
	{
		s1=(img->strtab)+(img->stridx[i]);
		if(!strcmp(s1, str))
			return(i);
		i=img->strnxt[i];
	}
	return(-1);
}

int BGBCC_FR2E_GetString(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img,
	char *str)
{
	char *s1;
	int i, j, k, h, l;

	if(!str)
		return(0);
	if(!(*str))
		return(1);

	h=BGBCC_FR2E_StrHash(str);
	i=img->strhash[h];
	while(i>0)
	{
		s1=(img->strtab)+(img->stridx[i]);
		if(!strcmp(s1, str))
			return(i);
		i=img->strnxt[i];
	}
	
	if(!img->strtab)
	{
		j=131072;
		img->strtab=bgbcc_malloc(j);
		img->sz_strtab=1;
		img->msz_strtab=j;
	}
	
	if(!img->stridx)
	{
		j=8192;
		img->stridx=bgbcc_malloc(j*sizeof(int));
		img->strnxt=bgbcc_malloc(j*sizeof(int));
		img->sz_stridx=2;
		img->msz_stridx=j;
		img->stridx[0]=0;
		img->stridx[1]=0;
	}
	
	l=strlen(str);
	
	if((img->sz_strtab+l+16)>=img->msz_strtab)
	{
		j=img->msz_strtab;
		while((img->sz_strtab+l+16)>=j)
			j=j+(j>>1);
		img->strtab=bgbcc_realloc(img->strtab, j);
		img->msz_strtab=j;
	}
	
	if((img->sz_stridx+1)>=img->msz_stridx)
	{
		j=img->msz_stridx;
		j=j+(j>>1);
		img->stridx=bgbcc_realloc(img->stridx, j*sizeof(int));
		img->strnxt=bgbcc_realloc(img->strnxt, j*sizeof(int));
		img->msz_stridx=j;
	}
	
	j=img->sz_strtab;
	s1=img->strtab+j;
	img->sz_strtab+=(l+1);
	strcpy(s1, str);

	i=img->sz_stridx++;
	img->stridx[i]=j;
	img->strnxt[i]=img->strhash[h];
	img->strhash[h]=i;

	if(img->sz_strtab>=img->msz_strtab)
		{ BGBCC_DBGBREAK }
	if(img->sz_stridx>=img->msz_stridx)
		{ BGBCC_DBGBREAK }
	
	return(i);
}

int BGBCC_FR2E_BufEmitOneccString(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img,
	byte **rct, byte tag, char *str)
{
	byte buf1[64];
	byte *ct;
	int szb1;
	int i, l;
	
	l=strlen(str);
	
	if(str && (*str) && (l<8))
	{
		ct=BGBCC_FR2E_BufEmitSVli(buf1, -1);
		memcpy(ct, str, l+1);
		ct+=l+1;
		szb1=ct-buf1;
	}else
	{
		i=BGBCC_FR2E_GetString(ctx, img, str);
		ct=BGBCC_FR2E_BufEmitSVli(buf1, i);
		szb1=ct-buf1;
	}
	
	BGBCC_FR2E_BufEmitOnecc(rct, tag, buf1, szb1);
	return(0);
}

void BGBCC_FR2E_EmitLiteralTableTwoccBuf(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img,
	u16 tag, byte *dat, int sz)
{
	byte *buf, *ct;
	int sz1, sz2;

	BGBCC_FR2E_CheckExpandTableData(
		&(img->litdat), &(img->sz_litdat),
		&(img->msz_litdat), sz);

#if 0
	if(!img->litdat)
	{
		sz2=sz;
		sz1=8192;
		while(sz2>=sz1)
			sz1=sz1+(sz1>>1);
	
		img->litdat=bgbcc_malloc(sz1);
		img->sz_litdat=0;
		img->msz_litdat=sz1;
	}

	if((img->sz_litdat+sz+16)>=img->msz_litdat)
	{
		sz2=(img->sz_litdat+sz+16);
		sz1=img->msz_litdat;
		while(sz2>=sz1)
			sz1=sz1+(sz1>>1);
	
		img->litdat=bgbcc_realloc(img->litdat, sz1);
		img->msz_litdat=sz1;
	}
#endif

	buf=img->litdat+img->sz_litdat;
	ct=buf;
	BGBCC_FR2E_BufEmitTwocc(&ct, tag, dat, sz);
	img->sz_litdat+=(ct-buf);

	if(img->sz_litdat>=img->msz_litdat)
		{ BGBCC_DBGBREAK }

}

void BGBCC_FR2E_EmitGlobalTableTwoccBuf(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img,
	u16 tag, byte *dat, int sz)
{
	byte *buf, *ct;
	int sz1, sz2;

	BGBCC_FR2E_CheckExpandTableData(
		&(img->gbldat), &(img->sz_gbldat),
		&(img->msz_gbldat), sz);

#if 0
	if(!img->gbldat)
	{
		sz2=sz;
		sz1=8192;
		while(sz2>=sz1)
			sz1=sz1+(sz1>>1);
	
		img->gbldat=bgbcc_malloc(sz1);
		img->sz_gbldat=0;
		img->msz_gbldat=sz1;
	}

	if((img->sz_gbldat+sz+16)>=img->msz_gbldat)
	{
		sz2=(img->sz_gbldat+sz+16);
		sz1=img->msz_gbldat;
		while(sz2>=sz1)
			sz1=sz1+(sz1>>1);
	
		img->gbldat=bgbcc_realloc(img->gbldat, sz1);
		img->msz_gbldat=sz1;
	}
#endif

	buf=img->gbldat+img->sz_gbldat;
	ct=buf;
	BGBCC_FR2E_BufEmitTwocc(&ct, tag, dat, sz);
	img->sz_gbldat+=(ct-buf);

	if(img->sz_gbldat>=img->msz_gbldat)
		{ BGBCC_DBGBREAK }

}


void BGBCC_FR2E_CheckExpandTableData(
	byte **rdat, int *rdsz, int *rdmsz, int sz)
{
	byte *buf, *ct;
	int sz1, sz2;

	if(!(*rdat))
	{
		sz2=sz;
		sz1=8192;
//		sz1=65536;
		while(sz2>=sz1)
			sz1=sz1+(sz1>>1);
	
		(*rdat)=bgbcc_malloc(sz1);
		(*rdsz)=0;
		(*rdmsz)=sz1;
	}

	if(((*rdsz)+sz+16)>=(*rdmsz))
	{
		sz2=((*rdsz)+sz+16);
		sz1=(*rdmsz);
		while(sz2>=sz1)
			sz1=sz1+(sz1>>1);
	
		(*rdat)=bgbcc_realloc((*rdat), sz1);
		(*rdmsz)=sz1;
	}
}

void BGBCC_FR2E_CheckExpandStridxData(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img, int sz)
{
	BGBCC_FR2E_CheckExpandTableData(
		&(img->stridxd), &(img->sz_stridxd),
		&(img->msz_stridxd), sz);
}

void BGBCC_FR2E_CheckExpandVopData(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img, int sz)
{
	BGBCC_FR2E_CheckExpandTableData(
		&(img->vopdat), &(img->sz_vopdat),
		&(img->msz_vopdat), sz);

#if 0
	byte *buf, *ct;
	int sz1, sz2;

	if(!img->vopdat)
	{
		sz2=sz
		sz1=8192;
		while(sz2>=sz1)
			sz1=sz1+(sz1>>1);
	
		img->vopdat=bgbcc_malloc(sz1);
		img->sz_vopdat=0;
		img->msz_vopdat=sz1;
	}

	if((img->sz_vopdat+sz+16)>=img->msz_vopdat)
	{
		sz2=(img->sz_vopdat+sz+16);
		sz1=img->msz_vopdat;
		while(sz2>=sz1)
			sz1=sz1+(sz1>>1);
	
		img->vopdat=bgbcc_realloc(img->vopdat, sz1);
		img->msz_vopdat=sz1;
	}
#endif
}

void BGBCC_FR2E_CheckExpandVtrData(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img, int sz)
{
	BGBCC_FR2E_CheckExpandTableData(
		&(img->vtrdat), &(img->sz_vtrdat),
		&(img->msz_vtrdat), sz);

#if 0
	byte *buf, *ct;
	int sz1, sz2;

	if(!img->vtrdat)
	{
		sz2=sz
		sz1=8192;
		while(sz2>=sz1)
			sz1=sz1+(sz1>>1);
	
		img->vtrdat=bgbcc_malloc(sz1);
		img->sz_vtrdat=0;
		img->msz_vtrdat=sz1;
	}

	if((img->sz_vtrdat+sz+16)>=img->msz_vtrdat)
	{
		sz2=(img->sz_vtrdat+sz+16);
		sz1=img->msz_vtrdat;
		while(sz2>=sz1)
			sz1=sz1+(sz1>>1);
	
		img->vtrdat=bgbcc_realloc(img->vtrdat, sz1);
		img->msz_vtrdat=sz1;
	}
#endif
}


void BGBCC_FR2E_CheckExpandVosiData(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img, int sz)
{
	BGBCC_FR2E_CheckExpandTableData(
		&(img->vosidat), &(img->sz_vosidat),
		&(img->msz_vosidat), sz);
}

void BGBCC_FR2E_CheckExpandVtsiData(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img, int sz)
{
	BGBCC_FR2E_CheckExpandTableData(
		&(img->vtsidat), &(img->sz_vtsidat),
		&(img->msz_vtsidat), sz);
}

byte *BGBCC_FR2E_BufEmitType(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img,
	byte *dct, ccxl_type type, int fl)
{
	ccxl_type ty1, ty2;
	byte *ct;
	char *sig;
	int i, j, k;

	if(fl&1)
	{
		/* If we are encoding VOP's, keep track of recently seen types.
		 * Types will use an MTF predictor.
		 */
	
		for(i=0; i<32; i++)
		{
			if(img->typehist[i].val==type.val)
				break;
		}
		
		if(i<32)
		{
			for(j=i; j>0; j--)
				img->typehist[j]=img->typehist[j-1];
			img->typehist[0]=type;
					
			k=(i<<2)|3;
			return(BGBCC_FR2E_BufEmitUVli(dct, k));
		}

		for(j=31; j>0; j--)
			img->typehist[j]=img->typehist[j-1];
		img->typehist[0]=type;
	}

//	if(type.val<256)
//	if((type.val&0x303F)==type.val)
	if((type.val&0xF0FF)==type.val)
	{
		/* Encode simple types directly.
		 */
	
//		j=(type.val&0x3F)|((type.val>>6)&0xC0);
		j=(type.val&0xFF)|((type.val>>4)&0xF00);
		j=(j<<2)|1;
		return(BGBCC_FR2E_BufEmitSVli(dct, j));
	}

	sig=BGBCC_CCXL_TypeGetSig(ctx, type);
	i=BGBCC_FR2E_GetString(ctx, img, sig);
	j=(i<<1)|0;
	ct=BGBCC_FR2E_BufEmitUVli(dct, j);
	return(ct);

//	u64 v1;
//	v1=(val<<1)^(val>>63);
//	return(BGBCC_FR2E_BufEmitUVli(dct, v1));
}

int BGBCC_FR2E_BitSpread(int a)
{
	int c;
	c=0;
	if(a&0x0001)c|=0x00000001;
	if(a&0x0002)c|=0x00000004;
	if(a&0x0004)c|=0x00000010;
	if(a&0x0008)c|=0x00000040;
	if(a&0x0010)c|=0x00000100;
	if(a&0x0020)c|=0x00000400;
	if(a&0x0040)c|=0x00001000;
	if(a&0x0080)c|=0x00004000;
	if(a&0x0100)c|=0x00010000;
	if(a&0x0200)c|=0x00040000;
	if(a&0x0400)c|=0x00100000;
	if(a&0x0800)c|=0x00400000;
	if(a&0x1000)c|=0x01000000;
	if(a&0x2000)c|=0x04000000;
	if(a&0x4000)c|=0x10000000;
	if(a&0x8000)c|=0x40000000;
	return(c);
}

int BGBCC_FR2E_BitMerge(int a, int b)
{
	int c;
	c=	(BGBCC_FR2E_BitSpread(a)<<1) |
		(BGBCC_FR2E_BitSpread(b)<<0) ;
	return(c);
}

byte *BGBCC_FR2E_BufEmitRegister(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img,
	byte *dct, ccxl_register reg, int fl)
{
	ccxl_register rg1, rg2;
	byte *ct;
	double f, g;
	float ff, fg;
	u32 ui, uj;
	s64 li, lj;
	char *str;
	int base, seq;
	int i, j, k;

	if(fl&1)
	{
		/* If we are encoding VOP's, keep track of recently seen regs.
		 * Types will use an MTF predictor.
		 */
	
//		for(i=0; i<64; i++)
		for(i=0; i<32; i++)
		{
			if(img->reghist[i].val==reg.val)
				break;
		}
		
//		if(i<64)
		if(i<32)
		{
			for(j=i; j>0; j--)
				img->reghist[j]=img->reghist[j-1];
			img->reghist[0]=reg;
					
			k=(i<<3)|0;
			return(BGBCC_FR2E_BufEmitUVli(dct, k));
		}

//		for(j=63; j>0; j--)
		for(j=31; j>0; j--)
			img->reghist[j]=img->reghist[j-1];
		img->reghist[0]=reg;
	}
	
	base=(reg.val>>0)&4095;
	seq=(reg.val>>12)&4095;
	
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
	{
		i=BGBCC_FR2E_BitMerge(seq, base);
		j=(i<<3)|2;
		ct=BGBCC_FR2E_BufEmitUVli(dct, j);
		return(ct);
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG)
	{
		i=BGBCC_FR2E_BitMerge(seq, base);
		j=(i<<3)|4;
		ct=BGBCC_FR2E_BufEmitUVli(dct, j);
		return(ct);
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL)
	{
		i=BGBCC_FR2E_BitMerge(seq, base);
		j=(i<<3)|6;
		ct=BGBCC_FR2E_BufEmitUVli(dct, j);
		return(ct);
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_GLOBAL)
	{
		i=reg.val&CCXL_REGID_REGMASK;
		j=(i<<4)|1;
		ct=BGBCC_FR2E_BufEmitUVli(dct, j);
		return(ct);
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_THISIDX)
	{
		i=BGBCC_FR2E_BitMerge(seq, base);
		j=(i<<6)|7;
		ct=BGBCC_FR2E_BufEmitUVli(dct, j);
		return(ct);
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LITERAL)
	{
		i=reg.val&CCXL_REGID_REGMASK;
		j=(i<<4)|5;
		ct=BGBCC_FR2E_BufEmitUVli(dct, j);
		return(ct);
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_STRING)
	{
		str=BGBCC_CCXL_GetRegImmStringValue(ctx, reg);
		i=BGBCC_FR2E_GetString(ctx, img, str);
		j=(i<<4)|0xD;
		ct=BGBCC_FR2E_BufEmitUVli(dct, j);
		return(ct);
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_GBLADDR)
	{
		i=reg.val&CCXL_REGID_REGMASK;
		j=(i<<5)|0x03;
		ct=BGBCC_FR2E_BufEmitUVli(dct, j);
		return(ct);
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT)
	{
		if(reg.val&CCXL_REGINT_STMASK)
		{
			li=BGBCC_CCXL_GetRegImmIntValue(ctx, reg);
			li=(li<<1)^(li>>63);
			j=(reg.val>>52)&15;
			lj=(li<<9)|(j<<5)|0xB;
			ct=BGBCC_FR2E_BufEmitUVli(dct, lj);
			return(ct);
		}else
		{
			li=BGBCC_CCXL_GetRegImmIntValue(ctx, reg);
			li=(li<<1)^(li>>63);
			lj=(li<<4)|0x9;
			ct=BGBCC_FR2E_BufEmitUVli(dct, lj);
			return(ct);
		}
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_FLOAT)
	{
		li=(s32)(reg.val);
//		li=(li<<1)^(li>>63);
		j=12;
		lj=(li<<9)|(j<<5)|0xB;
		ct=BGBCC_FR2E_BufEmitUVli(dct, lj);
		return(ct);
	}

	if(	((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG) ||
		((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_LONG) ||
		((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG_LVT))
	{
		li=BGBCC_CCXL_GetRegImmLongValue(ctx, reg);
		if(((li<<6)>>6)==li)
		{
			li=(li<<1)^(li>>63);
			lj=(li<<5)|0x13;
			ct=BGBCC_FR2E_BufEmitUVli(dct, lj);
			return(ct);
		}else
		{
			ct=BGBCC_FR2E_BufEmitUVli(dct, 0x0F);
			ct=BGBCC_FR2E_BufEmitSVli(ct, li);
			return(ct);
		}
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_WSTRING)
	{
		str=BGBCC_CCXL_GetRegImmStringValue(ctx, reg);
		i=BGBCC_FR2E_GetString(ctx, img, str);
		j=(i<<5)|0x1B;
		ct=BGBCC_FR2E_BufEmitUVli(dct, j);
		return(ct);
	}

#if 0
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG_LVT)
	{
		li=BGBCC_CCXL_GetRegImmLongValue(ctx, reg);
		ct=BGBCC_FR2E_BufEmitUVli(dct, 0x0F);
		ct=BGBCC_FR2E_BufEmitSVli(dct, li);
		return(ct);
	}
#endif

	if(	((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE)		||
		((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_DOUBLE)		||
		((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE_LVT)	)
	{
		f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg);
		ff=f;
		memcpy(&ui, &ff, sizeof(u32));
		memcpy(&fg, &ui, sizeof(u32));

		if(fg==f)
		{
			li=(s32)ui;
			li=(li<<1)^(li>>63);
			j=13;
			lj=(li<<9)|(j<<5)|0xB;
			ct=BGBCC_FR2E_BufEmitUVli(dct, lj);
			return(ct);
		}

		memcpy(&li, &f, sizeof(double));
		ct=BGBCC_FR2E_BufEmitUVli(dct, 0x2F);
		ct=BGBCC_FR2E_BufEmitSVli(ct, li);
		return(ct);
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_I128_LVT)
	{
		BGBCC_CCXL_GetRegImmX128Value(ctx, reg, &li, &lj);
		i=(reg.val>>48)&0x3F;	//vector-immed types
		ct=BGBCC_FR2E_BufEmitUVli(dct, 0x4F|(i<<7));
		ct=BGBCC_FR2E_BufEmitSVli(ct, li);
		ct=BGBCC_FR2E_BufEmitSVli(ct, lj);
		return(ct);
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_F128_LVT)
	{
		BGBCC_CCXL_GetRegImmX128Value(ctx, reg, &li, &lj);
		ct=BGBCC_FR2E_BufEmitUVli(dct, 0x6F);
		ct=BGBCC_FR2E_BufEmitSVli(ct, li);
		ct=BGBCC_FR2E_BufEmitSVli(ct, lj);
		return(ct);
	}

	BGBCC_DBGBREAK

	return(dct);

//	sig=BGBCC_CCXL_TypeGetSig(ctx, type);
//	i=BGBCC_FR2E_GetString(ctx, img, sig);

//	u64 v1;
//	v1=(val<<1)^(val>>63);
//	return(BGBCC_FR2E_BufEmitUVli(dct, v1));
}

int BGBCC_FR2E_EmitVirtOp(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img,
	BGBCC_CCXL_VirtOp *op)
{
	byte *ct, *ct0, *ct1;
	s64 li, lj;
	int msk, opn2, bn, dsimm;
	int i, j, k;

	bn=img->n_vop++;

	BGBCC_FR2E_CheckExpandVopData(ctx, img, 4096);

	msk=0;
	if(op->opn)			msk|=   1;
	if(op->opr)			msk|=   2;
	if(op->type.val)	msk|=   4;
	if(op->dst.val)		msk|=   8;
	if(op->srca.val)	msk|=  16;
	if(op->srcb.val)	msk|=  32;
	if(op->prd)			msk|= 128;
	if(op->stype.val)	msk|= 256;
	if(op->srcc.val)	msk|= 512;
	if(op->srcd.val)	msk|=1024;
	if(op->immty)		msk|=  64;
	
	msk|=1;
	
	opn2=BGBCC_FR2E_BitMerge(op->opn, op->opr);
	if((msk&3)==3)
	{
		if(opn2<128)msk&=~1;
	}

	img->stat_vop_cnt++;

	ct=img->vopdat+img->sz_vopdat;
	ct0=ct;
	
	ct=BGBCC_FR2E_BufEmitUVli(ct, msk);
	img->stat_vop_mskb+=ct-ct0;
	
	ct1=ct;
	if((msk&3)==2)
	{
		ct=BGBCC_FR2E_BufEmitUVli(ct, opn2);
	}else
	{
		if(msk&1)	ct=BGBCC_FR2E_BufEmitUVli(ct, op->opn);
		if(msk&2)	ct=BGBCC_FR2E_BufEmitUVli(ct, op->opr);
	}
	img->stat_vop_opb+=ct-ct1;

	ct1=ct;
	if(msk& 128)	ct=BGBCC_FR2E_BufEmitUVli(ct, op->prd);
	if(msk&   4)
	{	ct=BGBCC_FR2E_BufEmitType(ctx, img, ct, op->type, 1);
		img->stat_vopn_ty++;	}
	if(msk& 256)
	{	ct=BGBCC_FR2E_BufEmitType(ctx, img, ct, op->stype, 1);
		img->stat_vopn_ty++;	}
	img->stat_vop_tyb+=ct-ct1;

	ct1=ct;
	if(msk&   8)
	{	ct=BGBCC_FR2E_BufEmitRegister(ctx, img, ct, op->dst, 1);
		img->stat_vopn_reg++;	}
	if(msk&  16)
	{	ct=BGBCC_FR2E_BufEmitRegister(ctx, img, ct, op->srca, 1);
		img->stat_vopn_reg++;	}
	if(msk&  32)
	{	ct=BGBCC_FR2E_BufEmitRegister(ctx, img, ct, op->srcb, 1);
		img->stat_vopn_reg++;	}
	if(msk& 512)
	{	ct=BGBCC_FR2E_BufEmitRegister(ctx, img, ct, op->srcc, 1);
		img->stat_vopn_reg++;	}
	if(msk&1024)
	{	ct=BGBCC_FR2E_BufEmitRegister(ctx, img, ct, op->srcd, 1);
		img->stat_vopn_reg++;	}
	img->stat_vop_regb+=ct-ct1;

	i=ct-ct0;
	if((i<0) || (i>2048))
		{ BGBCC_DBGBREAK }
	
	if(msk&64)
	{
		ct1=ct;

		dsimm=0;
		switch(op->immty)
		{
		case CCXL_VOPITY_SI:
			if(op->imm.si<0)
			{
				li=-(op->imm.si);
				li=(li<<4)|1;
//				ct=BGBCC_FR2E_BufEmitUVli(ct, li);
			}else
			{
				li=op->imm.si;
				li=(li<<4)|2;
//				ct=BGBCC_FR2E_BufEmitUVli(ct, li);
			}
			dsimm=1;
			break;
		case CCXL_VOPITY_UI:
			li=op->imm.ui;
			li=(li<<4)|2;
//			ct=BGBCC_FR2E_BufEmitUVli(ct, li);
			dsimm=1;
			break;
		case CCXL_VOPITY_LBL:
			li=op->imm.ui;
			li=(li<<4)|3;
//			ct=BGBCC_FR2E_BufEmitUVli(ct, li);
			dsimm=1;
			break;
		case CCXL_VOPITY_STR:
			li=BGBCC_FR2E_GetString(ctx, img, op->imm.str);
			li=(li<<4)|4;
//			ct=BGBCC_FR2E_BufEmitUVli(ct, li);
			dsimm=1;
			break;
		
		case CCXL_VOPITY_CALL:
			k=op->imm.call.na;
			li=(k<<4)|5;
			ct=BGBCC_FR2E_BufEmitUVli(ct, li);
			for(i=0; i<k; i++)
			{
				ct=BGBCC_FR2E_BufEmitRegister(ctx, img, ct,
					op->imm.call.args[i], 1);
			}
			break;

		case CCXL_VOPITY_JMPTAB:
			k=op->imm.jmptab.nlbl;
			li=(k<<4)|6;
			ct=BGBCC_FR2E_BufEmitUVli(ct, li);
			lj=op->imm.jmptab.vmin;
			ct=BGBCC_FR2E_BufEmitSVli(ct, lj);
			for(i=0; i<(k+1); i++)
			{
				ct=BGBCC_FR2E_BufEmitUVli(ct,
					op->imm.jmptab.lbls[i].id);
			}
			break;

		case CCXL_VOPITY_GFID:
			i=BGBCC_FR2E_BitMerge(op->imm.obj.fid, op->imm.obj.gid);
			li=(i<<4)|7;
//			ct=BGBCC_FR2E_BufEmitUVli(ct, li);
			dsimm=1;
			break;
		}
		
		if(dsimm)
		{
//			i=9999;
//			if(li>=128)
			if(1)
			{
				for(i=0; i<32; i++)
					if(img->vopi_hist[i]==li)
						break;
			}
			
			if((i>=8) && (li<128))
				i=9999;
			
			if(i<32)
			{
				lj=(i<<4)|0;
				ct=BGBCC_FR2E_BufEmitUVli(ct, lj);
				
				for(j=i; j>0; j--)
					img->vopi_hist[j]=img->vopi_hist[j-1];
				img->vopi_hist[0]=li;
			}else
			{
				ct=BGBCC_FR2E_BufEmitUVli(ct, li);
				for(j=31; j>0; j--)
					img->vopi_hist[j]=img->vopi_hist[j-1];
				img->vopi_hist[0]=li;
			}
		}

		img->stat_vop_immb+=ct-ct1;
		img->stat_vopn_imm++;
	}


	i=ct-ct0;
	if((i<0) || (i>2048))
		{ BGBCC_DBGBREAK }

	img->sz_vopdat+=i;

	if(img->sz_vopdat>=img->msz_vopdat)
		{ BGBCC_DBGBREAK }

	return(bn);
}

int BGBCC_FR2E_EmitVirtTrace(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img,
	BGBCC_CCXL_RegisterInfo *inf, BGBCC_CCXL_VirtTr *tr)
{
	BGBCC_CCXL_VirtOp *vop;
	byte *ct;
	int i, j, k, b, n, bn, b0;
	
	b0=img->n_vop;
	b=tr->b_ops;
	n=tr->n_ops;
	for(i=0; i<n; i++)
	{
		vop=inf->vop[b+i];
		BGBCC_FR2E_EmitVirtOp(ctx, img, vop);
	}


	if(!b)
		b=-b0;

	bn=img->n_vtr++;

	BGBCC_FR2E_CheckExpandVtrData(ctx, img, 256);
	ct=img->vtrdat+img->sz_vtrdat;
	ct=BGBCC_FR2E_BufEmitSVli(ct, b);
	ct=BGBCC_FR2E_BufEmitSVli(ct, n);
	ct=BGBCC_FR2E_BufEmitSVli(ct, tr->trfl);
	img->sz_vtrdat=ct-img->vtrdat;
	
	if(img->sz_vtrdat>=img->msz_vtrdat)
		{ BGBCC_DBGBREAK }

	return(bn);
}

u64 BGBCC_FR2E_FlattenFunctionTraces(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img,
	BGBCC_CCXL_RegisterInfo *inf, int fl)
{
	BGBCC_CCXL_VirtTr *vtr;
	byte *ct;
	u64 v;
	int bn, bn1, n;
	int bo, bo1, vo;
	int svo, svo1;
	int svt, svt1;
	int i, j, k;

	svo=img->sz_vopdat;
	svt=img->sz_vtrdat;

	bo=img->n_vop;
	bn=img->n_vtr;
	n=inf->n_vtr;
	for(i=0; i<n; i++)
	{
		vtr=inf->vtr[i];
		BGBCC_FR2E_EmitVirtTrace(ctx, img, inf, vtr);
	}

	bo1=img->n_vop;
	bn1=img->n_vtr;

	img->n_vtr++;

	BGBCC_FR2E_CheckExpandVtrData(ctx, img, 256);
	ct=img->vtrdat+img->sz_vtrdat;
	ct=BGBCC_FR2E_BufEmitSVli(ct, -bn);
	ct=BGBCC_FR2E_BufEmitSVli(ct, -n);
	ct=BGBCC_FR2E_BufEmitSVli(ct, -(inf->gblid));
	img->sz_vtrdat=ct-img->vtrdat;	

	if(img->sz_vtrdat>=img->msz_vtrdat)
		{ BGBCC_DBGBREAK }

	svo1=img->sz_vopdat;
	svt1=img->sz_vtrdat;

	BGBCC_FR2E_CheckExpandVosiData(ctx, img, 128);
//	BGBCC_FR2E_CheckExpandVtsiData(ctx, img, 64);

	vo=img->n_vosi++;

	ct=img->vosidat+img->sz_vosidat;

	ct=BGBCC_FR2E_BufEmitUVli(ct, bo);
	ct=BGBCC_FR2E_BufEmitUVli(ct, bn);
	ct=BGBCC_FR2E_BufEmitUVli(ct, bo1-bo);
	ct=BGBCC_FR2E_BufEmitUVli(ct, bn1-bn);

	ct=BGBCC_FR2E_BufEmitUVli(ct, svo);
	ct=BGBCC_FR2E_BufEmitUVli(ct, svt);
	ct=BGBCC_FR2E_BufEmitUVli(ct, svo1-svo);
	ct=BGBCC_FR2E_BufEmitUVli(ct, svt1-svt);

	ct=BGBCC_FR2E_BufEmitUVli(ct, inf->gblid);
	ct=BGBCC_FR2E_BufEmitUVli(ct, 0);

	img->sz_vosidat=ct-img->vosidat;

//	return(bn1);
//	return(vo);

	v=bn1 | (((u64)vo)<<32);
	return(v);
}

void BGBCC_FR2E_FlattenFunctionTracesBuf(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img,
	BGBCC_CCXL_RegisterInfo *inf, byte **rct, int fl)
{
	byte *ct;
	u64 v;
	int bn, vo;
	
	v=BGBCC_FR2E_FlattenFunctionTraces(ctx, img, inf, fl);
	bn=(int)(v>> 0);
	vo=(int)(v>>32);

	ct=*rct;
	ct=BGBCC_FR2E_BufEmitSVli(ct, bn);
	ct=BGBCC_FR2E_BufEmitSVli(ct, vo);
	*rct=ct;
}

void BGBCC_FR2E_FlattenVarListBuf(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img,
	BGBCC_CCXL_RegisterInfo **inf, int ninf, byte **rct, int fl)
{
	byte tb1[4096];
	byte *ct, *ct1;
	int i, j, k;

	ct=*rct;
	for(i=0; i<ninf; i++)
	{
		ct1=tb1;
		BGBCC_FR2E_FlattenRegisterInfoBuf(ctx, img, inf[i], &ct1, 0);
		BGBCC_FR2E_BufEmitTwocc(&ct, 'V', tb1, ct1-tb1);
	}
	*rct=ct;
}

void BGBCC_FR2E_FlattenRegisterInfoBuf(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img,
	BGBCC_CCXL_RegisterInfo *inf, byte **rct, int fl)
{
	byte tb1[4096];
	byte *ct, *ct1;
	int nolocal;
	int i, j, k;
	
	ct=*rct;
	if(!(fl&1))
	{
		if(inf->qname)
			BGBCC_FR2E_BufEmitOneccString(ctx, img, &ct, 'q', inf->qname);
		if(inf->name)
			BGBCC_FR2E_BufEmitOneccString(ctx, img, &ct, 'n', inf->name);
		if(inf->sig)
			BGBCC_FR2E_BufEmitOneccString(ctx, img, &ct, 's', inf->sig);
		if(inf->flagstr)
			BGBCC_FR2E_BufEmitOneccString(ctx, img, &ct, 'f', inf->flagstr);
	}
	
	nolocal=0;
	if(fl&1)
	{
		if(inf->regtype==CCXL_LITID_FUNCTION)
			nolocal=1;
	}
	
	if(inf->n_fields>0)
	{
		ct1=tb1;
		BGBCC_FR2E_FlattenVarListBuf(ctx, img,
			inf->fields, inf->n_fields, &ct1, 0);
		BGBCC_FR2E_BufEmitTwocc(&ct, 'F', tb1, ct1-tb1);
	}
	
	if(inf->n_args>0)
	{
		ct1=tb1;
		BGBCC_FR2E_FlattenVarListBuf(ctx, img,
			inf->args, inf->n_args, &ct1, 0);
		BGBCC_FR2E_BufEmitTwocc(&ct, 'A', tb1, ct1-tb1);
	}
	
	if(!nolocal)
	{
		if(inf->n_locals>0)
		{
			ct1=tb1;
			BGBCC_FR2E_FlattenVarListBuf(ctx, img,
				inf->locals, inf->n_locals, &ct1, 0);
			BGBCC_FR2E_BufEmitTwocc(&ct, 'L', tb1, ct1-tb1);
		}
		
		if(inf->n_regs>0)
		{
			ct1=tb1;
			BGBCC_FR2E_FlattenVarListBuf(ctx, img,
				inf->regs, inf->n_regs, &ct1, 0);
			BGBCC_FR2E_BufEmitTwocc(&ct, 'R', tb1, ct1-tb1);
		}
		
		if(inf->n_statics>0)
		{
			ct1=tb1;
			BGBCC_FR2E_FlattenVarListBuf(ctx, img,
				inf->statics, inf->n_statics, &ct1, 0);
			BGBCC_FR2E_BufEmitTwocc(&ct, 'S', tb1, ct1-tb1);
		}
	}
	
	if(!nolocal)
	{
		if(inf->n_vtr>0)
		{
			ct1=tb1;
			BGBCC_FR2E_FlattenFunctionTracesBuf(ctx, img, inf, &ct1, 0);
			BGBCC_FR2E_BufEmitTwocc(&ct, 'S', tb1, ct1-tb1);
		}
	}

	*rct=ct;
}

void BGBCC_FR2E_FlattenLiteral(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img,
	BGBCC_CCXL_LiteralInfo *inf)
{
	byte tb[4096];
	byte *ct;
	int tg, sz;
	
	tg=BGBCC_TCC_LI;
	if(inf->littype==CCXL_LITID_STRUCT)
		tg=BGBCC_TCC_ST;
	if(inf->littype==CCXL_LITID_UNION)
		tg=BGBCC_TCC_UN;
	if(inf->littype==CCXL_LITID_TYPEDEF)
		tg=BGBCC_TCC_TY;
	
	ct=tb;
	if(inf->name)
		BGBCC_FR2E_BufEmitOneccString(ctx, img, &ct, 'n', inf->name);
	if(inf->sig)
		BGBCC_FR2E_BufEmitOneccString(ctx, img, &ct, 's', inf->sig);
	
	if(inf->decl)
	{
		BGBCC_FR2E_FlattenRegisterInfoBuf(ctx, img, inf->decl, &ct, 1);
	}
	
	sz=ct-tb;
	BGBCC_FR2E_EmitLiteralTableTwoccBuf(ctx, img, tg, tb, sz);

	if(sz>=4096)
		{ BGBCC_DBGBREAK }

}

void BGBCC_FR2E_FlattenGlobal(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img,
	BGBCC_CCXL_RegisterInfo *inf)
{
	byte tb[4096];
	byte *ct;
	int tg, sz;
	
	tg=BGBCC_TCC_RI;
	if(inf->regtype==CCXL_LITID_FUNCTION)
		tg=BGBCC_TCC_FN;
	if(inf->regtype==CCXL_LITID_GLOBALVAR)
		tg=BGBCC_TCC_GV;
	if(inf->regtype==CCXL_LITID_STATICVAR)
		tg=BGBCC_TCC_SV;
	
	ct=tb;
	BGBCC_FR2E_FlattenRegisterInfoBuf(ctx, img, inf, &ct, 0);
	
	sz=ct-tb;
	BGBCC_FR2E_EmitGlobalTableTwoccBuf(ctx, img, tg, tb, sz);

	if(sz>=4096)
		{ BGBCC_DBGBREAK }
}

void BGBCC_FR2E_FlattenLiterals(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img)
{
	BGBCC_CCXL_LiteralInfo *cur, *nxt;
	BGBCC_CCXL_LiteralInfo *llst;
	BGBCC_CCXL_LiteralInfo *sel;
	int i, j, c;

	for(i=1; i<ctx->n_literals; i++)
	{
		cur=ctx->literals[i];
		BGBCC_FR2E_FlattenLiteral(ctx, img, cur);
	}
}

void BGBCC_FR2E_FlattenGlobals(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img)
{
	BGBCC_CCXL_RegisterInfo *cur, *nxt;
	BGBCC_CCXL_RegisterInfo *llst;
	BGBCC_CCXL_RegisterInfo *sel;
	int i, j;

	for(i=1; i<ctx->n_reg_globals; i++)
	{
		cur=ctx->reg_globals[i];
		BGBCC_FR2E_FlattenGlobal(ctx, img, cur);
	}
}

void BGBCC_FR2E_FlattenStridxData(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img)
{
	byte *ct;
	int i, j, k, n, l;

	BGBCC_FR2E_CheckExpandStridxData(ctx, img, img->sz_stridx*2+256);
	
	n=img->sz_stridx; l=0;
	for(i=2; i<n; i++)
	{
		j=img->stridx[i];
		k=j-l;
		l=j;

		BGBCC_FR2E_CheckExpandStridxData(ctx, img, (n-i)*2+256);
		ct=img->stridxd+img->sz_stridxd;
		ct=BGBCC_FR2E_BufEmitUVli(ct, k);
		img->sz_stridxd=ct-img->stridxd;

		if(img->sz_stridxd>=img->msz_stridxd)
			{ BGBCC_DBGBREAK }

	}
}


byte *BGBCC_FR2E_FlattenImage(
	BGBCC_TransState *ctx, int *rsz)
{
	BGBCC_FR2E_ImgState *img;
	byte *ct, *buf;
	int tsz;
	
	img=bgbcc_malloc(sizeof(BGBCC_FR2E_ImgState));
	memset(img, 0, sizeof(BGBCC_FR2E_ImgState));
	
	BGBCC_FR2E_FlattenLiterals(ctx, img);
	BGBCC_FR2E_FlattenGlobals(ctx, img);
	BGBCC_FR2E_FlattenStridxData(ctx, img);
	
	tsz=512;
	tsz+=img->sz_strtab;
	tsz+=img->sz_stridxd;
	tsz+=img->sz_litdat;
	tsz+=img->sz_gbldat;
	tsz+=img->sz_vopdat;
	tsz+=img->sz_vtrdat;
	tsz+=img->sz_vosidat;
	
	printf("sz strs %d\n", img->sz_strtab);
	printf("sz stri %d\n", img->sz_stridxd);
	printf("sz litd %d\n", img->sz_litdat);
	printf("sz gbld %d\n", img->sz_gbldat);
	printf("sz vops %d\n", img->sz_vopdat);
	printf("sz vtrs %d\n", img->sz_vtrdat);
	printf("sz vosi %d\n", img->sz_vosidat);
	
	printf("VOP: M=%d O=%d T=%d R=%d I=%d\n",
		img->stat_vop_mskb,
		img->stat_vop_opb,
		img->stat_vop_tyb,
		img->stat_vop_regb,
		img->stat_vop_immb);

	printf("VOP: Ty/Op=%.2f Reg/Op=%.2f Imm/Op=%.2f\n",
		(1.0*img->stat_vopn_ty)/(img->stat_vop_cnt),
		(1.0*img->stat_vopn_reg)/(img->stat_vop_cnt),
		(1.0*img->stat_vopn_imm)/(img->stat_vop_cnt));
	printf("VOP: B/Msk=%.2f B/Opc=%.2f B/Ty=%.2f B/Reg=%.2f B/Imm=%.2f\n",
		(1.0*img->stat_vop_mskb)/(img->stat_vop_cnt),
		(1.0*img->stat_vop_opb)/(img->stat_vop_cnt),
		(1.0*img->stat_vop_tyb)/(img->stat_vopn_ty),
		(1.0*img->stat_vop_regb)/(img->stat_vopn_reg),
		(1.0*img->stat_vop_immb)/(img->stat_vopn_imm));

	printf("VOP: B/VOp=%.2f\n",
		(1.0*img->sz_vopdat)/(img->stat_vop_cnt));
	
	img->imgdat=bgbcc_malloc(tsz);

	buf=img->imgdat;	
	ct=buf+8;

	BGBCC_FR2E_BufEmitFourcc(&ct, BGBCC_FCC_strs,
		(byte *)(img->strtab), img->sz_strtab);
	BGBCC_FR2E_BufEmitFourcc(&ct, BGBCC_FCC_stri,
		img->stridxd, img->sz_stridxd);
	BGBCC_FR2E_BufEmitFourcc(&ct, BGBCC_FCC_vops,
		img->vopdat, img->sz_vopdat);
	BGBCC_FR2E_BufEmitFourcc(&ct, BGBCC_FCC_vtrs,
		img->vtrdat, img->sz_vtrdat);

	BGBCC_FR2E_BufEmitFourcc(&ct, BGBCC_FCC_vosi,
		img->vosidat, img->sz_vosidat);

	BGBCC_FR2E_BufEmitFourcc(&ct, BGBCC_FCC_LITD,
		img->litdat, img->sz_litdat);
	BGBCC_FR2E_BufEmitFourcc(&ct, BGBCC_FCC_GBLD,
		img->gbldat, img->sz_gbldat);

	tsz=ct-buf;
	bgbcc_setu32le(buf+0, (tsz<<2)|2);
	bgbcc_setu32le(buf+4, BGBCC_FCC_FR2E);
	
	*rsz=tsz;
	return(buf);
}


void BGBCC_FR2E_ReadTag(byte **rcs, u32 *rtag, byte **rdat, int *rsz)
{
	byte *cs, *cs1, *cs2;
	int sz;
	
	cs=*rcs;

	if(cs[0]&0x80)
	{
		*rtag=cs[0];
		*rdat=cs+1;
		*rsz=0;
		*rcs=cs+1;
		return;
	}

	if(cs[1]&0x80)
	{
		*rtag=cs[0];
		sz=cs[1]^0xFF;
		*rdat=cs+2;
		*rsz=sz;
		*rcs=cs+2+sz;
		return;
	}

	if(cs[3]&0x80)
	{
		*rtag=cs[0]|(cs[1]<<8);
		sz=(cs[2]|(cs[3]<<8))^0xFFFF;
		*rdat=cs+4;
		*rsz=sz;
		*rcs=cs+4+sz;
		return;
	}

	if(cs[7]&0x80)
	{
		*rtag=cs[0]|(cs[1]<<8)|(cs[2]<<16)|(cs[3]<<24);
		sz=(cs[4]|(cs[5]<<8)|(cs[6]<<16)|(cs[7]<<24))^0xFFFFFFFF;
		*rdat=cs+8;
		*rsz=sz;
		*rcs=cs+8+sz;
		return;
	}
}

u64 BGBCC_FR2E_BufReadUVli(byte **rcs)
{
	byte *cs;
	u64 v;
	int i;
	
	cs=*rcs;
	i=*cs++;

	if(i<0x80)
	{
		v=i;
		*rcs=cs;
		return(v);
	}

	if(i<0xC0)
	{
		v=((i&0x3F)<<8)|(*cs++);
		*rcs=cs;
		return(v);
	}

	if(i<0xE0)
	{
		i=((i&0x1F)<<8)|(*cs++);
		v=((i&0xFF)<<8)|(*cs++);
		*rcs=cs;
		return(v);
	}

	if(i<0xF0)
	{
		i=((i&0x0F)<<8)|(*cs++);
		i=((i&0xFF)<<8)|(*cs++);
		v=((i&0xFF)<<8)|(*cs++);
		*rcs=cs;
		return(v);
	}

	if(i<0xF8)
	{
		v=((i&0x07)<<8)|(*cs++);
		v=((v&0xFF)<<8)|(*cs++);
		v=((v&0xFF)<<8)|(*cs++);
		v=((v&0xFF)<<8)|(*cs++);
		*rcs=cs;
		return(v);
	}

	if(i<0xFC)
	{
		v=((i&0x03)<<8)|(*cs++);
		v=((v&0xFF)<<8)|(*cs++);
		v=((v&0xFF)<<8)|(*cs++);
		v=((v&0xFF)<<8)|(*cs++);
		v=((v&0xFF)<<8)|(*cs++);
		*rcs=cs;
		return(v);
	}

	if(i<0xFE)
	{
		v=((i&0x01)<<8)|(*cs++);
		v=((v&0xFF)<<8)|(*cs++);
		v=((v&0xFF)<<8)|(*cs++);
		v=((v&0xFF)<<8)|(*cs++);
		v=((v&0xFF)<<8)|(*cs++);
		v=((v&0xFF)<<8)|(*cs++);
		*rcs=cs;
		return(v);
	}

	if(i<0xFF)
	{
		v=((i&0x00)<<8)|(*cs++);
		v=((v&0xFF)<<8)|(*cs++);
		v=((v&0xFF)<<8)|(*cs++);
		v=((v&0xFF)<<8)|(*cs++);
		v=((v&0xFF)<<8)|(*cs++);
		v=((v&0xFF)<<8)|(*cs++);
		v=((v&0xFF)<<8)|(*cs++);
		*rcs=cs;
		return(v);
	}

	v=(*cs++);
	v=((v&0xFF)<<8)|(*cs++);
	v=((v&0xFF)<<8)|(*cs++);
	v=((v&0xFF)<<8)|(*cs++);
	v=((v&0xFF)<<8)|(*cs++);
	v=((v&0xFF)<<8)|(*cs++);
	v=((v&0xFF)<<8)|(*cs++);
	v=((v&0xFF)<<8)|(*cs++);
	*rcs=cs;
	return(v);
}

s64 BGBCC_FR2E_BufReadSVli(byte **rcs)
{
	u64 v0;
	s64 v;
	
	v0=BGBCC_FR2E_BufReadUVli(rcs);
	v=v0>>1;
	if(v0&1)
		v=~v;
	return(v);
}

void BGBCC_FR2E_UnpackStridxData(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img)
{
	byte *cs, *cse, *cs0;
	u64 v;
	int i, n;
	
	cs=img->stridxd;
	cse=cs+img->sz_stridxd;

	cs0=cs; n=2;
	while(cs<cse)
	{
		BGBCC_FR2E_BufReadUVli(&cs0);
		n++;
	}
	
	img->stridx=bgbcc_malloc((n+1)*sizeof(int));
	img->stridx[0]=0;
	img->stridx[1]=0;

	cs0=cs; i=2;
	while(cs<cse)
	{
		v=BGBCC_FR2E_BufReadUVli(&cs0);
		img->stridx[i]=v;
		i++;
	}
}

void BGBCC_FR2E_UnpackVirtTraceData(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img)
{
	BGBCC_CCXL_VirtTr *vtr;
	byte *cs, *cse, *cs0;
	u64 v;
	int i, n;
	
	cs=img->vtrdat;
	cse=cs+img->sz_vtrdat;

	cs0=cs; n=0;
	while(cs<cse)
	{
		BGBCC_FR2E_BufReadUVli(&cs0);
		BGBCC_FR2E_BufReadUVli(&cs0);
		BGBCC_FR2E_BufReadUVli(&cs0);
		n++;
	}
	
	img->vtr=bgbcc_malloc((n+1)*sizeof(void *));
	img->n_vtr=n;

	cs0=cs; i=0;
	while(cs<cse)
	{
		vtr=bgbcc_malloc(sizeof(BGBCC_CCXL_VirtTr));
		vtr->b_ops=BGBCC_FR2E_BufReadSVli(&cs0);
		vtr->n_ops=BGBCC_FR2E_BufReadSVli(&cs0);
		vtr->trfl=BGBCC_FR2E_BufReadSVli(&cs0);
		vtr->n_fops=vtr->n_ops;
		img->vtr[i]=vtr;
		i++;
	}
}

void BGBCC_FR2E_UnpackVosiData(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img)
{
	BGBCC_FR2E_ImgVosiEnt *vtr;
	byte *cs, *cse, *cs0;
	u64 v;
	int i, n;
	
	cs=img->vosidat;
	cse=cs+img->sz_vosidat;

	cs0=cs; n=1;
	while(cs<cse)
	{
		BGBCC_FR2E_BufReadUVli(&cs0);
		BGBCC_FR2E_BufReadUVli(&cs0);
		BGBCC_FR2E_BufReadUVli(&cs0);
		BGBCC_FR2E_BufReadUVli(&cs0);

		BGBCC_FR2E_BufReadUVli(&cs0);
		BGBCC_FR2E_BufReadUVli(&cs0);
		BGBCC_FR2E_BufReadUVli(&cs0);
		BGBCC_FR2E_BufReadUVli(&cs0);

		BGBCC_FR2E_BufReadUVli(&cs0);
		BGBCC_FR2E_BufReadUVli(&cs0);
		n++;
	}
	
	img->vosi=bgbcc_malloc((n+1)*sizeof(void *));
	img->n_vosi=n;

	cs0=cs; i=1;
	while(cs<cse)
	{
		vtr=bgbcc_malloc(sizeof(BGBCC_FR2E_ImgVosiEnt));

		vtr->b_vops=BGBCC_FR2E_BufReadUVli(&cs0);
		vtr->b_vtrs=BGBCC_FR2E_BufReadUVli(&cs0);
		vtr->n_vops=BGBCC_FR2E_BufReadUVli(&cs0);
		vtr->n_vtrs=BGBCC_FR2E_BufReadUVli(&cs0);

		vtr->o_vops=BGBCC_FR2E_BufReadUVli(&cs0);
		vtr->o_vtrs=BGBCC_FR2E_BufReadUVli(&cs0);
		vtr->s_vops=BGBCC_FR2E_BufReadUVli(&cs0);
		vtr->s_vtrs=BGBCC_FR2E_BufReadUVli(&cs0);

		vtr->gblid=BGBCC_FR2E_BufReadUVli(&cs0);
		vtr->flags=BGBCC_FR2E_BufReadUVli(&cs0);

		img->vosi[i]=vtr;
		i++;
	}
}

int BGBCC_FR2E_UnMortonValueX(u64 v)
{
	int x;

	x=0;
	if(v&0x00000001)	x|=0x0001;
	if(v&0x00000004)	x|=0x0002;
	if(v&0x00000010)	x|=0x0004;
	if(v&0x00000040)	x|=0x0008;
	if(v&0x00000100)	x|=0x0010;
	if(v&0x00000400)	x|=0x0020;
	if(v&0x00001000)	x|=0x0040;
	if(v&0x00004000)	x|=0x0080;
	if(v&0x00010000)	x|=0x0100;
	if(v&0x00040000)	x|=0x0200;
	if(v&0x00100000)	x|=0x0400;
	if(v&0x00400000)	x|=0x0800;
	if(v&0x01000000)	x|=0x1000;
	if(v&0x04000000)	x|=0x2000;
	if(v&0x10000000)	x|=0x4000;
	if(v&0x40000000)	x|=0x8000;
	return(x);
}

void BGBCC_FR2E_UnMortonValue(u64 v, int *rvx, int *rvy)
{
	*rvx=BGBCC_FR2E_UnMortonValueX(v);
	*rvy=BGBCC_FR2E_UnMortonValueX(v>>1);
}

void BGBCC_FR2E_UnpackType(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img,
	byte **rcs, ccxl_type *rty)
{
	u64 v0;
	int i, j, k;

	v0=BGBCC_FR2E_BufReadUVli(rcs);
	if(!(v0&1))
	{
		BGBCC_CCXL_TypeFromSig(ctx, rty, (img->strtab+(v0>>1)));

		for(j=31; j>0; j--)
			img->typehist[j]=img->typehist[j-1];
		img->typehist[0]=*rty;
		return;
	}

	if(!(v0&2))
	{
		i=v0>>2;
		j=(i&0x00FF)<<0;
		k=(i&0x0F00)<<4;
		rty->val=j|k;

		for(j=31; j>0; j--)
			img->typehist[j]=img->typehist[j-1];
		img->typehist[0]=*rty;
		return;
	}
	
	i=v0>>2;
	*rty=img->typehist[i];
	for(j=i; j>0; j--)
		img->typehist[j]=img->typehist[j-1];
	img->typehist[0]=*rty;
}

void BGBCC_FR2E_UnpackVopRegister(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img,
	byte **rcs, BGBCC_CCXL_VirtOp *vop, ccxl_register *rty)
{
	byte *cs;
	u64 v, uk;
	int i, j, k, i0, i1;

	cs=*rcs;
	uk=BGBCC_FR2E_BufReadUVli(&cs);
	v=0;

	if(!(uk&7))
	{
		i=uk>>3;

		*rty=img->reghist[i];
		for(j=i; j>0; j--)
			img->reghist[j]=img->reghist[j-1];
		img->reghist[0]=*rty;

		*rcs=cs;
		return;
	}
	
	if(!(uk&1))
	{
		BGBCC_FR2E_UnMortonValue(uk>>3, &i0, &i1);
		v=i0|(i1<<12);

		if((uk&0x06)==0x02)
			v|=CCXL_REGTY_TEMP;
		if((uk&0x06)==0x04)
			v|=CCXL_REGTY_ARG;
		if((uk&0x06)==0x06)
			v|=CCXL_REGTY_LOCAL;
		
		v|=((u64)(vop->type.val))<<24;
	}else
		if(!(uk&2))
	{
		v=uk>>4;

		if((uk&0x0C)==0x00)
			v|=CCXL_REGTY_GLOBAL;
		if((uk&0x0C)==0x04)
			v|=CCXL_REGTY_IMM_LITERAL;

		if((uk&0x0C)==0x08)
			v|=CCXL_REGTY_IMM_INT;

		if((uk&0x0C)==0x0C)
		{
			BGBCC_CCXL_GetRegForStringValue(ctx, rty,
				(img->strtab+v));
			v=rty->val;
		}
	}else
		if(!(uk&4))
	{
		v=uk>>5;

		if((uk&0x18)==0x00)
			v|=CCXL_REGTY_IMM_GBLADDR;

		if((uk&0x18)==0x08)
		{
			v=(v>>4)|((v&15)<<52);
			v|=CCXL_REGTY_IMM_INT;
		}

		if((uk&0x18)==0x10)
		{
			v|=CCXL_REGTY_IMM_LONG;
		}

		if((uk&0x18)==0x18)
		{
			BGBCC_CCXL_GetRegForWStringValue(ctx, rty,
				(img->strtab+v));
			v=rty->val;
		}
	}else
		if(!(uk&8))
	{
	}else
		if(!(uk&16))
	{
		BGBCC_FR2E_BufReadUVli(&cs);
		if((uk>>6)&1)
			BGBCC_FR2E_BufReadUVli(&cs);
	}

	rty->val=v;
	for(j=63; j>0; j--)
		img->reghist[j]=img->reghist[j-1];
	img->reghist[0]=*rty;

	*rcs=cs;
}

void BGBCC_FR2E_UnpackVopImmed(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img,
	byte **rcs, BGBCC_CCXL_VirtOp *vop)
{
	byte *cs;
	int i, j, k, i0, i1;
	cs=*rcs;
	k=BGBCC_FR2E_BufReadUVli(&cs);
	*rcs=cs;
}

void BGBCC_FR2E_UnpackVirtOpSingle(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img,
	byte **rcs,
	BGBCC_CCXL_VirtOp *vop)
{
	byte *cs;
	u64 v0;
	int i, j, k, i0, i1;

	cs=*rcs;

	j=BGBCC_FR2E_BufReadUVli(&cs);
	if(!(j&3))
		j=img->vop_lmsk;

	img->vop_lmsk=j;

	if(j&1)
	{
		vop->opn=BGBCC_FR2E_BufReadUVli(&cs);
		if(j&2)
			vop->opr=BGBCC_FR2E_BufReadUVli(&cs);
	}else
	{
		k=BGBCC_FR2E_BufReadUVli(&cs);
		BGBCC_FR2E_UnMortonValue(k, &i0, &i1);
		vop->opn=i0;
		vop->opr=i1;
	}

	if(j&128)
		{ vop->prd=BGBCC_FR2E_BufReadUVli(&cs); }

	if(j&4)
		{ BGBCC_FR2E_UnpackType(ctx, img, &cs, &(vop->type)); }

	if(j&256)
		{ BGBCC_FR2E_UnpackType(ctx, img, &cs, &(vop->stype)); }


	if(j&8)
		{ BGBCC_FR2E_UnpackVopRegister(ctx, img, &cs, vop, &(vop->dst)); }
	if(j&16)
		{ BGBCC_FR2E_UnpackVopRegister(ctx, img, &cs, vop, &(vop->srca)); }
	if(j&32)
		{ BGBCC_FR2E_UnpackVopRegister(ctx, img, &cs, vop, &(vop->srcb)); }

	if(j&512)
		{ BGBCC_FR2E_UnpackVopRegister(ctx, img, &cs, vop, &(vop->srcc)); }
	if(j&1024)
		{ BGBCC_FR2E_UnpackVopRegister(ctx, img, &cs, vop, &(vop->srcd)); }

	if(j&64)
		{ BGBCC_FR2E_UnpackVopImmed(ctx, img, &cs, vop); }
		
	*rcs=cs;
}

void BGBCC_FR2E_UnpackVirtOpData(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img)
{
	BGBCC_CCXL_VirtOp t_vop;
	BGBCC_CCXL_VirtOp *vop;
	byte *cs, *cse, *cs0;
	u64 v;
	int i, j, k, n;
	
	cs=img->vopdat;
	cse=cs+img->sz_vopdat;

	cs0=cs; n=0;
	while(cs<cse)
	{
		vop=&t_vop;
		BGBCC_FR2E_UnpackVirtOpSingle(ctx, img, &cs0, vop);
		n++;
	}
	
	img->vop=bgbcc_malloc((n+1)*sizeof(void *));
	img->n_vop=n;

	cs0=cs; i=0;
	while(cs<cse)
	{
		vop=bgbcc_malloc(sizeof(BGBCC_CCXL_VirtOp));
		BGBCC_FR2E_UnpackVirtOpSingle(ctx, img, &cs0, vop);
		img->vop[i]=vop;
		i++;
	}
}

char *BGBCC_FR2E_UnpackStringDataObj(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img,
	byte *idat, int isz)
{
	byte *cs;
	int i, j, k;

	cs=idat;
	k=BGBCC_FR2E_BufReadUVli(&cs);

	if(!k)
		return(NULL);

	if(k>0)
	{
		return(bgbcc_strdup(img->strtab+k));
	}
	
	if(k==-1)
	{
		return(bgbcc_strdup((char *)cs));
	}
	
	return(NULL);
}

void BGBCC_FR2E_UnpackRegFieldsArray(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img,
	BGBCC_CCXL_RegisterInfo ***rregs, int *rn_regs,
	byte *idat, int isz)
{
	BGBCC_CCXL_RegisterInfo *reg;
	BGBCC_CCXL_RegisterInfo **regs;
	byte *cs, *cse, *cs0, *cs1;
	u32 fcc;
	int sz, n;
	int i, j, k;

	cs0=idat;
	cse=cs0+isz;
	fcc=0; sz=0; cs1=NULL;

	cs=cs0; n=0;
	while(cs<cse)
	{
		BGBCC_FR2E_ReadTag(&cs, &fcc, &cs1, &sz);
		if(fcc=='V')
			n++;
	}
	
	regs=bgbcc_malloc(n*sizeof(void *));
	*rregs=regs;

	cs=cs0; i=0;
	while(cs<cse)
	{
		BGBCC_FR2E_ReadTag(&cs, &fcc, &cs1, &sz);

		reg=NULL;

		if(fcc=='V')
		{
			reg=bgbcc_malloc(sizeof(BGBCC_CCXL_RegisterInfo));
			BGBCC_FR2E_UnpackGlobalObj(ctx, img, reg, cs1, sz, fcc);
			
			regs[i]=reg;
			i++;
		}
	}
}

void BGBCC_FR2E_UnpackGlobalObj(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img,
	BGBCC_CCXL_RegisterInfo *gbl, byte *idat, int isz, int tag)
{
	byte *cs, *cse, *cs0, *cs1;
	u32 fcc;
	int sz, n;
	int i, j, k;

	cs=idat;
	cse=cs+isz;
	fcc=0; sz=0; cs1=NULL;

	cs0=cs;
	while(cs<cse)
	{
		BGBCC_FR2E_ReadTag(&cs, &fcc, &cs1, &sz);
		
		if(fcc=='n')
		{
			gbl->name=BGBCC_FR2E_UnpackStringDataObj(ctx, img, cs1, sz);
		}

		if(fcc=='q')
		{
			gbl->qname=BGBCC_FR2E_UnpackStringDataObj(ctx, img, cs1, sz);
		}

		if(fcc=='s')
		{
			gbl->sig=BGBCC_FR2E_UnpackStringDataObj(ctx, img, cs1, sz);
		}

		if(fcc=='f')
		{
			gbl->flagstr=BGBCC_FR2E_UnpackStringDataObj(ctx, img, cs1, sz);
		}

		if(fcc=='F')
		{
			BGBCC_FR2E_UnpackRegFieldsArray(ctx, img,
				&(gbl->fields), &(gbl->n_fields),
				cs1, sz);
		}

		if(fcc=='A')
		{
			BGBCC_FR2E_UnpackRegFieldsArray(ctx, img,
				&(gbl->args), &(gbl->n_args),
				cs1, sz);
		}

		if(fcc=='L')
		{
			BGBCC_FR2E_UnpackRegFieldsArray(ctx, img,
				&(gbl->locals), &(gbl->n_locals),
				cs1, sz);
		}

		if(fcc=='R')
		{
			BGBCC_FR2E_UnpackRegFieldsArray(ctx, img,
				&(gbl->regs), &(gbl->n_regs),
				cs1, sz);
		}

		if(fcc=='S')
		{
			BGBCC_FR2E_UnpackRegFieldsArray(ctx, img,
				&(gbl->statics), &(gbl->n_statics),
				cs1, sz);
		}
	}
}

void BGBCC_FR2E_UnpackGlobalData(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img)
{
	BGBCC_CCXL_RegisterInfo *gbl;
	byte *cs, *cse, *cs0, *cs1;
	u32 fcc;
	int sz, n;
	int i, j, k;

	cs=img->gbldat;
	cse=cs+img->sz_gbldat;
	fcc=0; sz=0; cs1=NULL;

	cs0=cs; n=1;
	while(cs<cse)
	{
		BGBCC_FR2E_ReadTag(&cs, &fcc, &cs1, &sz);
		n++;
	}
	
	img->gbls=bgbcc_malloc((n+1)*sizeof(void *));
	img->n_gbls=n;

	cs=cs0; i=1;
	while(cs<cse)
	{
		BGBCC_FR2E_ReadTag(&cs, &fcc, &cs1, &sz);
		
		gbl=NULL;

		if(	(fcc==BGBCC_TCC_RI) ||
			(fcc==BGBCC_TCC_FN) ||
			(fcc==BGBCC_TCC_GV) ||
			(fcc==BGBCC_TCC_SV)	)
		{
			gbl=bgbcc_malloc(sizeof(BGBCC_CCXL_RegisterInfo));
			BGBCC_FR2E_UnpackGlobalObj(ctx, img, gbl, cs1, sz, fcc);
		}
		
		img->gbls[i]=gbl;
		i++;
	}
}

void BGBCC_FR2E_UnpackLiteralData(
	BGBCC_TransState *ctx, BGBCC_FR2E_ImgState *img)
{
	BGBCC_CCXL_RegisterInfo *decl;
	BGBCC_CCXL_LiteralInfo *lit;
	byte *cs, *cse, *cs0, *cs1;
	u32 fcc;
	int sz, n;
	int i, j, k;

	cs=img->litdat;
	cse=cs+img->sz_litdat;
	fcc=0; cs1=NULL; sz=0;

	cs0=cs; n=1;
	while(cs<cse)
	{
		BGBCC_FR2E_ReadTag(&cs, &fcc, &cs1, &sz);
		n++;
	}
	
	img->lits=bgbcc_malloc((n+1)*sizeof(void *));
	img->n_lits=n;

	cs=cs0; i=1;
	while(cs<cse)
	{
		BGBCC_FR2E_ReadTag(&cs, &fcc, &cs1, &sz);
		
		decl=NULL;
		lit=NULL;

		if(	(fcc==BGBCC_TCC_LI) ||
			(fcc==BGBCC_TCC_ST) ||
			(fcc==BGBCC_TCC_UN) ||
//			(fcc==BGBCC_TCC_CL) ||
			(fcc==BGBCC_TCC_FN) ||
			(fcc==BGBCC_TCC_TY)	)
		{
			lit=bgbcc_malloc(sizeof(BGBCC_CCXL_LiteralInfo));
			decl=bgbcc_malloc(sizeof(BGBCC_CCXL_RegisterInfo));
			BGBCC_FR2E_UnpackGlobalObj(ctx, img, decl, cs1, sz, fcc);
			
			lit->decl=decl;
			lit->name=decl->qname;
			lit->sig=decl->sig;
		}
		
		img->lits[i]=lit;
		i++;
	}
}

void BGBCC_FR2E_LoadBufferFRB(
	BGBCC_TransState *ctx, byte *buf, int bufsz)
{
	BGBCC_FR2E_ImgState *img;	
	byte *cs, *cse, *cs0, *cs1;
	byte *strs, *strs_e;
	byte *stri, *stri_e;
	byte *vops, *vops_e;
	byte *vtrs, *vtrs_e;
	byte *vosi, *vosi_e;
	byte *litd, *litd_e;
	byte *gbld, *gbld_e;
	u32 fcc;
	int sz;

	
	fcc=0; sz=0; cs=NULL; cs1=NULL;
	
	cs0=buf;
	BGBCC_FR2E_ReadTag(&cs0, &fcc, &cs, &sz);
	cse=cs+sz;
	
	if(fcc!=BGBCC_FCC_FR2E)
	{
		printf("BGBCC_FR2E_LoadBufferFRB: Root tag not FR2E\n");
		return;
	}
	if(cs0>(buf+bufsz))
	{
		printf("BGBCC_FR2E_LoadBufferFRB: Root tag larger than input\n");
		return;
	}

	img=bgbcc_malloc(sizeof(BGBCC_FR2E_ImgState));
	memset(img, 0, sizeof(BGBCC_FR2E_ImgState));

	strs=NULL;	strs_e=NULL;
	stri=NULL;	stri_e=NULL;
	vops=NULL;	vops_e=NULL;
	vtrs=NULL;	vtrs_e=NULL;
	vosi=NULL;	vosi_e=NULL;
	litd=NULL;	litd_e=NULL;
	gbld=NULL;	gbld_e=NULL;

	while(cs<cse)
	{
		BGBCC_FR2E_ReadTag(&cs, &fcc, &cs1, &sz);
		
		if(fcc==BGBCC_FCC_strs)
		{
			strs=cs1;
			strs_e=cs1+sz;
			continue;
		}

		if(fcc==BGBCC_FCC_stri)
		{
			stri=cs1;
			stri_e=cs1+sz;
			continue;
		}

		if(fcc==BGBCC_FCC_vops)
		{
			vops=cs1;
			vops_e=cs1+sz;
			continue;
		}

		if(fcc==BGBCC_FCC_vtrs)
		{
			vtrs=cs1;
			vtrs_e=cs1+sz;
			continue;
		}

		if(fcc==BGBCC_FCC_vosi)
		{
			vosi=cs1;
			vosi_e=cs1+sz;
			continue;
		}

		if(fcc==BGBCC_FCC_LITD)
		{
			litd=cs1;
			litd_e=cs1+sz;
			continue;
		}

		if(fcc==BGBCC_FCC_GBLD)
		{
			gbld=cs1;
			gbld_e=cs1+sz;
			continue;
		}
	}

	img->strtab=(char *)strs;
	img->sz_strtab=strs_e-strs;

	img->stridxd=stri;
	img->sz_stridxd=stri_e-stri;

	img->vopdat=vops;
	img->sz_vopdat=vops_e-vops;

	img->vtrdat=vtrs;
	img->sz_vtrdat=vtrs_e-vtrs;

	img->vosidat=vosi;
	img->sz_vosidat=vosi_e-vosi;


	img->litdat=litd;
	img->sz_litdat=litd_e-litd;

	img->gbldat=litd;
	img->sz_gbldat=gbld_e-gbld;
	
	BGBCC_FR2E_UnpackStridxData(ctx, img);
	BGBCC_FR2E_UnpackLiteralData(ctx, img);

	BGBCC_FR2E_UnpackVosiData(ctx, img);
	BGBCC_FR2E_UnpackGlobalData(ctx, img);
	BGBCC_FR2E_UnpackVirtTraceData(ctx, img);

}
