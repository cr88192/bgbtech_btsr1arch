u32 BCCX_GetU32le(byte *src)
{
	return(src[0]|(src[1]<<8)|(src[2]<<16)|(src[3]<<24));
}

u64 BCCX_GetU64le(byte *src)
{
	u64 lo, hi, lv;
	lo=BCCX_GetU32le(src+0);
	hi=BCCX_GetU32le(src+4);
	lv=(hi<<32)|lo;
	return(lv);
}

void BCCX_SetU64le(byte *dst, u64 lv)
{
	dst[0]=(lv>> 0)&255;
	dst[1]=(lv>> 8)&255;
	dst[2]=(lv>>16)&255;
	dst[3]=(lv>>24)&255;
	dst[4]=(lv>>32)&255;
	dst[5]=(lv>>40)&255;
	dst[6]=(lv>>48)&255;
	dst[7]=(lv>>56)&255;
}

char *BCCX_AbxeParseStringLong(BCCX_ParseAbxe *ctx, int len)
{
	static char *longstr=NULL;
	
	if(!longstr)
		longstr=malloc(65536);

	memcpy(longstr, ctx->cs, len);
	longstr[len]=0;
	ctx->cs+=len;
	return(longstr);
}

double BCCX_AbxeDecodeFp8ToDouble(byte fp8)
{
	u64 lv;
	int ex, fr, sg;
	double g;
	
	if(!fp8)
		return(0);
	
	sg=(fp8>>7)&1;
	ex=(fp8>>3)&15;
	ex=(ex-7)+1023;
	fr=fp8&7;
	
	lv=(((u64)ex)<<52)|(((u64)fr)<<49)|(((u64)sg)<<63);
	
	g=0;
	memcpy(&g, &lv, 8);
	return(g);
}

double BCCX_AbxeDecodeFp16ToDouble(u16 fp16)
{
	u64 lv;
	int ex, fr, sg;
	double g;
	
	if(!fp16)
		return(0);
	
	sg=(fp16>>15)&1;
	ex=(fp16>>10)&31;
	ex=(ex-15)+1023;
	fr=fp16&1023;
	
	lv=(((u64)ex)<<52)|(((u64)fr)<<42)|(((u64)sg)<<63);
	
	g=0;
	memcpy(&g, &lv, 8);
	return(g);
}

int BCCX_AbxeDecodeMruTag(BCCX_ParseAbxe *ctx, int op)
{
	int i0, i1, k0, k1;
	int i, j, k, l;

	l=op&31;
	i0=(ctx->rov_tag+l)&31;

	if(l>=16)
	{
		i1=(ctx->rov_tag-1)&31;
		ctx->rov_tag=i1;
		k0=ctx->mru_tag[i0];
		k1=ctx->mru_tag[i1];
		ctx->mru_tag[i0]=k1;
		ctx->mru_tag[i1]=k0;
		j=k0;
	}else if(l>0)
	{
		i1=(i0-1)&31;
		k0=ctx->mru_tag[i0];
		k1=ctx->mru_tag[i1];
		ctx->mru_tag[i0]=k1;
		ctx->mru_tag[i1]=k0;
		j=k0;
	}else
	{
		j=ctx->mru_tag[i0];
	}
	
	return(j);
}

int BCCX_AbxeDecodeMruStr(BCCX_ParseAbxe *ctx, int op)
{
	int i0, i1, k0, k1;
	int i, j, k, l;

	l=op&31;
	i0=(ctx->rov_str+l)&31;

	if(l>=16)
	{
		i1=(ctx->rov_str-1)&31;
		ctx->rov_str=i1;
		k0=ctx->mru_str[i0];
		k1=ctx->mru_str[i1];
		ctx->mru_str[i0]=k1;
		ctx->mru_str[i1]=k0;
		k=k0;
	}else if(l>0)
	{
		i1=(i0-1)&31;
		k0=ctx->mru_str[i0];
		k1=ctx->mru_str[i1];
		ctx->mru_str[i0]=k1;
		ctx->mru_str[i1]=k0;
		k=k0;
	}else
	{
		k=ctx->mru_str[i0];
	}
	return(k);
}

int BCCX_AbxeDecodeLitTag(BCCX_ParseAbxe *ctx, int op)
{
	char tb[258];
	int i, j, k, l;

	l=op&31;
	
	if(l<28)
	{
		memcpy(tb, ctx->cs, l);
		ctx->cs+=l;
		tb[l]=0;
	}else if(l==28)
	{
		l=*ctx->cs++;
		memcpy(tb, ctx->cs, l);
		ctx->cs+=l;
		tb[l]=0;
	}else
	{
		debug_break
	}

	j=BCCX_StringToStridx(tb);

	i=(ctx->rov_tag-1)&31;
	ctx->rov_tag=i;
	ctx->mru_tag[i]=j;
	
	return(j);
}

int BCCX_AbxeDecodeLitStr(BCCX_ParseAbxe *ctx, int op)
{
	char tb[258];
	char *s1;
	int i, j, k, l, l1;

	l=op&31;
	if(l<28)
	{
		l1=l;
	}else
		if(l==28)
	{
		l1=*ctx->cs++;
	}else if(l==29)
	{
		l1=*ctx->cs++;
		l1+=(*ctx->cs++)<<8;
	}else
	{
		debug_break
	}
	
	if(l1<256)
	{
		memcpy(tb, ctx->cs, l1);
		ctx->cs+=l1;
		tb[l1]=0;
		
		k=bccx_strdup_i(tb);

		i=(ctx->rov_str-1)&31;
		ctx->rov_str=i;
		ctx->mru_str[i]=k;
	}else
	{
		s1=BCCX_AbxeParseStringLong(ctx, l1);
		k=bccx_strdup_i(s1);
	}

	return(k);
}

int BCCX_AbxeParseAttrib(BCCX_ParseAbxe *ctx,
	BCCX_Node *tmp, int op)
{
	char tb[258];
	char *s1;
	s64 li;
	double g;
	int op1;
	int i0, i1, k0, k1;
	int i, j, k, l1;
	
	if((op>>5)==6)
	{
		j=BCCX_AbxeDecodeMruTag(ctx, op);
	}else
		if((op>>5)==2)
	{
		j=BCCX_AbxeDecodeLitTag(ctx, op);
	}else
	{
		debug_break
	}

	op1=*ctx->cs++;

	if((op1>>5)==7)
	{
		k=BCCX_AbxeDecodeMruStr(ctx, op1);
		BCCX_SetCstIx2(tmp, j, k);
		return(1);
	}
	
	if((op1>>5)==3)
	{
		k=BCCX_AbxeDecodeLitStr(ctx, op1);
		BCCX_SetCstIx2(tmp, j, k);
		return(1);
	}

	if((op1>>5)==4)
	{
		l1=op1&31;
		
		if(l1<16)
		{
			li=((int)(l1<<28))>>28;
			BCCX_SetIntCstIx(tmp, j, li);
		}else
			if(l1<24)
		{
			k=(l1&7)+1;
			li=BCCX_GetU64le(ctx->cs);
			ctx->cs+=k;
			if(k<8)
				li=(li<<((8-k)*8))>>((8-k)*8);
			BCCX_SetIntCstIx(tmp, j, li);
		}else if(l1==24)
		{
			k=*ctx->cs++;
			g=BCCX_AbxeDecodeFp8ToDouble(k);
			BCCX_SetFloatCstIx(tmp, j, g);
		}else if(l1==25)
		{
			k=*ctx->cs++;
			k+=(*ctx->cs++)<<8;
			g=BCCX_AbxeDecodeFp16ToDouble(k);
			BCCX_SetFloatCstIx(tmp, j, g);
		}else
		{
			k=(l1&7)+1;
			li=BCCX_GetU64le(ctx->cs);
			ctx->cs+=k;
			if(k<8)
				li=li<<((8-k)*8);
			memcpy(&g, &li, 8);
			BCCX_SetFloatCstIx(tmp, j, g);
		}
		return(0);
	}

	debug_break
	return(-1);
}

BCCX_Node *BCCX_AbxeParseTag(BCCX_ParseAbxe *ctx, int *rmod)
{
	char tb[258];
	BCCX_Node *tmp, *tmp1;
	int op;
	int i0, i1, k0, k1;
	int i, j, k, l;
	
	op=*ctx->cs++;
	if(!op)
	{
		*rmod=0;
		return(NULL);
	}

	if((op>>5)==5)
	{
		j=BCCX_AbxeDecodeMruTag(ctx, op);
		*rmod=0;

		tmp=BCCX_NewCstIx(j);
		
		while(1)
		{
			op=*ctx->cs;
			if(op<2)
			{
				ctx->cs++;
				break;
			}
			if(((op>>5)==1) || ((op>>5)==5) ||
				((op>>5)==3) || ((op>>5)==7) ||
				((op>>5)==4))
			{
				*rmod=1;
				break;
			}

			if(((op>>5)==2) || ((op>>5)==6))
			{
				op=*ctx->cs++;
				BCCX_AbxeParseAttrib(ctx, tmp, op);
				continue;
			}
		}
		
		return(tmp);
	}
	
	if((op>>5)==1)
	{
		j=BCCX_AbxeDecodeLitTag(ctx, op);
		*rmod=0;
		
		tmp=BCCX_NewCstIx(j);
		
		while(1)
		{
			op=*ctx->cs;
			if(op<2)
			{
				op=*ctx->cs++;
				break;
			}
			if(((op>>5)==1) || ((op>>5)==5) ||
				((op>>5)==3) || ((op>>5)==7) ||
				((op>>5)==4))
			{
				*rmod=1;
				break;
			}

			if(((op>>5)==2) || ((op>>5)==6))
			{
				op=*ctx->cs++;
				BCCX_AbxeParseAttrib(ctx, tmp, op);
				continue;
			}
		}
		
		return(tmp);
	}

	if((op>>5)==7)
	{
		k=BCCX_AbxeDecodeMruStr(ctx, op);
		*rmod=0;
		tmp=BCCX_NewText(bccx_stridx_i(k));
		return(tmp);
	}
	
	if((op>>5)==3)
	{
		k=BCCX_AbxeDecodeLitStr(ctx, op);
		*rmod=0;
		tmp=BCCX_NewText(bccx_stridx_i(k));
		return(tmp);
	}

	debug_break
	return(NULL);
}

BCCX_Node *BCCX_AbxeParseExpr(BCCX_ParseAbxe *ctx)
{
	BCCX_Node *tmp, *tmp1, *end;
	int i;

	i=0;
	tmp=BCCX_AbxeParseTag(ctx, &i);

	if(i==1)
	{
		while(1)
		{
			if((*ctx->cs)==1)
			{
				ctx->cs++;
				break;
			}
		
			tmp1=BCCX_AbxeParseExpr(ctx);
			if(!tmp1)break;
			BCCX_Add(tmp, tmp1);
		}
		return(tmp);
	}

	return(tmp);
}

BCCX_Node *BCCX_AbxeParseBuffer(byte *ibuf, int ibsz)
{
	BCCX_ParseAbxe tctx;
	BCCX_ParseAbxe *ctx;
	BCCX_Node *lst, *tmp;
	int na;

	if((ibuf[0]>>5)!=1)
		return(NULL);

	ctx=&tctx;
	memset(ctx, 0, sizeof(BCCX_ParseAbxe));
	ctx->cs=ibuf;
	ctx->cse=ibuf+ibsz;

	lst=NULL;
	while((*ctx->cs) && (ctx->cs<ctx->cse))
	{
		tmp=BCCX_AbxeParseExpr(ctx);
		if(!tmp)break;
		lst=BCCX_AddEnd(lst, tmp);
	}
	
	if(BCCX_TagIsP(lst, "$list"))
	{
		na=BCCX_GetNodeChildCount(lst);
		if(na==1)
		{
			tmp=BCCX_GetNodeIndex(lst, 0);
			return(tmp);
		}
	}
	
	return(lst);
}


int BCCX_AbxeEncodeTag(BCCX_ParseAbxe *ctx, int tty, int txi)
{
	char *s0;
	int i0, i1, k0, k1;
	int i, j, k, l;
	
	txi&=4095;
	
	l=-1;
	for(i=0; i<32; i++)
	{
		j=(ctx->rov_tag+i)&31;
		if(ctx->mru_tag[j]==txi)
			{ l=i; break; }
	}
	
	if(l<0)
	{
		s0=BCCX_StridxToString(txi);
		l=strlen(s0);
		
		if(l<28)
		{
			*ctx->ct++=tty|l;
		}else if(l<256)
		{
			*ctx->ct++=tty|28;
			*ctx->ct++=l;
		}else
		{
			*ctx->ct++=tty|29;
			*ctx->ct++=l;
			*ctx->ct++=l>>8;
		}
		
		memcpy(ctx->ct, s0, l);
		ctx->ct+=l;

		i=(ctx->rov_tag-1)&31;
		ctx->rov_tag=i;
		ctx->mru_tag[i]=txi;
		
		return(0);
	}

	*ctx->ct++=0x80|tty|l;
	i0=(ctx->rov_tag+l)&31;

	if(l>=16)
	{
		i1=(ctx->rov_tag-1)&31;
		ctx->rov_tag=i1;
		k0=ctx->mru_tag[i0];
		k1=ctx->mru_tag[i1];
		ctx->mru_tag[i0]=k1;
		ctx->mru_tag[i1]=k0;
	}else if(l>0)
	{
		i1=(i0-1)&31;
		k0=ctx->mru_tag[i0];
		k1=ctx->mru_tag[i1];
		ctx->mru_tag[i0]=k1;
		ctx->mru_tag[i1]=k0;
	}
	
	return(0);
}

int BCCX_AbxeEncodeStr(BCCX_ParseAbxe *ctx, int tty, int txi)
{
	char *s0;
	int i0, i1, k0, k1;
	int i, j, k, l;
	
	l=-1;
	for(i=0; i<32; i++)
	{
		j=(ctx->rov_str+i)&31;
		if(ctx->mru_str[j]==txi)
			{ l=i; break; }
	}
	
	if(l<0)
	{
		s0=bccx_stridx_i(txi);
		l=strlen(s0);
		
		if(l<28)
		{
			*ctx->ct++=tty|l;
		}else if(l<256)
		{
			*ctx->ct++=tty|28;
			*ctx->ct++=l;
		}else
		{
			*ctx->ct++=tty|29;
			*ctx->ct++=l;
			*ctx->ct++=l>>8;
		}
		
		memcpy(ctx->ct, s0, l);
		ctx->ct+=l;

		i=(ctx->rov_str-1)&31;
		ctx->rov_str=i;
		ctx->mru_str[i]=txi;
		
		return(0);
	}

	*ctx->ct++=0x80|tty|l;
	i0=(ctx->rov_str+l)&31;

	if(l>=16)
	{
		i1=(ctx->rov_str-1)&31;
		ctx->rov_str=i1;
		k0=ctx->mru_str[i0];
		k1=ctx->mru_str[i1];
		ctx->mru_str[i0]=k1;
		ctx->mru_str[i1]=k0;
	}else if(l>0)
	{
		i1=(i0-1)&31;
		k0=ctx->mru_str[i0];
		k1=ctx->mru_str[i1];
		ctx->mru_str[i0]=k1;
		ctx->mru_str[i1]=k0;
	}
	
	return(0);
}

int BCCX_AbxeEncodeText(BCCX_ParseAbxe *ctx, char *text)
{
	int k;
	k=bccx_strdup_i(text);
	BCCX_AbxeEncodeStr(ctx, 0x60, k);
	return(0);
}

int BCCX_AbxeEncodeInt(BCCX_ParseAbxe *ctx, s64 li)
{
	if((li>=(-8)) && (li<=7))
	{
		*ctx->ct++=0x80|(li&15);
		return(0);
	}

	if((li>=(-128)) && (li<=127))
	{
		*ctx->ct++=0x90;
		*ctx->ct++=li;
		return(0);
	}

	if((li>=(-32768)) && (li<=32767))
	{
		*ctx->ct++=0x91;
		*ctx->ct++=li;
		*ctx->ct++=li>>8;
		return(0);
	}

	if((li>=(-16777216)) && (li<=16777215))
	{
		*ctx->ct++=0x92;
		*ctx->ct++=li;
		*ctx->ct++=li>>8;
		*ctx->ct++=li>>16;
		return(0);
	}
	
	if(((s32)li)==li)
	{
		*ctx->ct++=0x93;
		*ctx->ct++=li;
		*ctx->ct++=li>>8;
		*ctx->ct++=li>>16;
		*ctx->ct++=li>>24;
		return(0);
	}

	*ctx->ct++=0x97;
	*ctx->ct++=li;
	*ctx->ct++=li>>8;
	*ctx->ct++=li>>16;
	*ctx->ct++=li>>24;
	*ctx->ct++=li>>32;
	*ctx->ct++=li>>40;
	*ctx->ct++=li>>48;
	*ctx->ct++=li>>56;
	return(0);
}

int BCCX_AbxeEncodeFloat(BCCX_ParseAbxe *ctx, double lf)
{
	u64 lv, ti, fr;
	int ex, k;

	if(lf==0)
	{
		*ctx->ct++=0x98;
		*ctx->ct++=0x00;
		return(0);
	}

	lv=0;
	memcpy(&lv, &lf, 8);

	ex=(lv>>52)-1023;
	fr=lv&((1ULL<<52)-1);
	
	if((ex>(-7)) && (ex<7) && (((fr>>49)<<49)==fr))
	{
		k=((lv>>56)&0x80)|((ex+7)<<3)|(fr>>49);
		*ctx->ct++=0x98;
		*ctx->ct++=k;
		return(0);
	}

	if((ex>(-15)) && (ex<15) && (((fr>>42)<<42)==fr))
	{
		k=((lv>>48)&0x8000)|((ex+15)<<10)|(fr>>42);
		*ctx->ct++=0x99;
		*ctx->ct++=k;
		*ctx->ct++=k>>8;
		return(0);
	}

	ti=7;
	while((ti>2) && !(lv&255))
		{ lv=lv>>8; ti--; }

	*ctx->ct++=0x98|ti;
	BCCX_SetU64le(ctx->ct, lv);
	ctx->ct+=(ti+1);
	return(0);
}

int BCCX_AbxeEncodeNodeNodeAttr(BCCX_ParseAbxe *ctx, BCCX_Node *node)
{
	u16 *attr_n, *an;
	BCCX_AttrVal *attr_v, *av;
	char *s0;
	int i, j, k;
	
	if(!node->malvl)
	{
		attr_n=node->attr_n;
		attr_v=(BCCX_AttrVal *)(node->attr_v);
	}else
	{
		attr_n=NULL;
		attr_v=NULL;
	}
	
	for(i=0; i<node->nattr; i++)
	{
		if(attr_n)
		{
			an=attr_n+i;
			av=attr_v+i;
		}else
		{
			BCCX_FetchAttrArrIx(node, i, &an, &av);
		}

		j=*an;

		s0=BCCX_StridxToString(j);
		if(*s0=='$')
			continue;

		if((j>>12)==BCCX_IVTY_STRING)
		{
			BCCX_AbxeEncodeTag(ctx, 0x40, j);
			BCCX_AbxeEncodeText(ctx, av->s);
		}
		if((j>>12)==BCCX_IVTY_INT)
		{
			BCCX_AbxeEncodeTag(ctx, 0x40, j);
			BCCX_AbxeEncodeInt(ctx, av->i);
		}
		if((j>>12)==BCCX_IVTY_REAL)
		{
			BCCX_AbxeEncodeTag(ctx, 0x40, j);
			BCCX_AbxeEncodeFloat(ctx, av->f);
		}
	}
	return(0);
}

int BCCX_AbxeEncodeNodeSpecial(BCCX_ParseAbxe *ctx, BCCX_Node *node)
{
	BCCX_Node *ncur;
	char *tb, *t, *s;
	int i, j, k, sz;

	if(!strcmp(BCCX_Tag(node), "$text"))
	{
		BCCX_AbxeEncodeText(ctx, BCCX_Get(node, "$text"));
		return(0);
	}

	if(!strcmp(BCCX_Tag(node), "$cdata"))
	{
		BCCX_AbxeEncodeText(ctx, BCCX_Get(node, "$text"));
		return(0);
	}

	if(!strcmp(BCCX_Tag(node), "$bdata"))
	{
		sz=BCCX_GetIntCst(node, &bgbcc_rcst_size, "size");
		s=BCCX_GetCst(node, &bgbcc_rcst_Stext, "$text");

		return(0);
	}

	return(-1);
}

int BCCX_AbxeEncodeNodeNodeAttrNode(BCCX_ParseAbxe *ctx, BCCX_Node *node)
{
	u16 *attr_n, *an;
	BCCX_Node *ncur;
	BCCX_AttrVal *attr_v, *av;
	char *s0, *s1;
	int i, j, k, do_full;
	
	if(!node->malvl)
	{
		attr_n=node->attr_n;
		attr_v=(BCCX_AttrVal *)(node->attr_v);
	}else
	{
		attr_n=NULL;
		attr_v=NULL;
	}
	
	for(i=0; i<node->nattr; i++)
	{
		if(attr_n)
		{
			an=attr_n+i;
			av=attr_v+i;
		}else
		{
			BCCX_FetchAttrArrIx(node, i, &an, &av);
		}
	
		j=*an;
		if((j>>12)!=BCCX_IVTY_NODE)
			continue;

		if((j&4095)>=4088)
			continue;

		s0=BCCX_StridxToString(j);

		if(!av->p)
		{
			BCCX_AbxeEncodeTag(ctx, 0x20, j);
			*ctx->ct++=0x01;
			continue;
		}

		ncur=av->p;
		BCCX_AbxeEncodeTag(ctx, 0x20, j);
		BCCX_AbxeEncodeNode(ctx, ncur);
		*ctx->ct++=0x01;
	}
	return(0);
}

int BCCX_AbxeEncodeNode(BCCX_ParseAbxe *ctx, BCCX_Node *node)
{
	char buf[4096];
	BCCX_Node *ncur;
	char *tb, *t, *s0;
	int i, na, ci;

	if(!node)
	{
		return(-1);
	}

	s0=BCCX_Get(node, "$text");

	if(!node->itag)
	{
		if(s0)
		{
			i=strlen(s0);
			BCCX_AbxeEncodeText(ctx, s0);
			return(0);
		}
		return(-1);
	}

	if(s0)
	{
		BCCX_AbxeEncodeNodeSpecial(ctx, node);
		return(0);
	}

	BCCX_AbxeEncodeTag(ctx, 0x20, node->itag);
	BCCX_AbxeEncodeNodeNodeAttr(ctx, node);

	BCCX_AbxeEncodeNodeNodeAttrNode(ctx, node);

	na=BCCX_GetNodeChildCount(node);
	for(ci=0; ci<na; ci++)
	{
		ncur=BCCX_GetNodeIndex(node, ci);
		BCCX_AbxeEncodeNode(ctx, ncur);
	}

	*ctx->ct++=0x01;
	return(0);
}

int BCCX_AbxeEncodeNodeBuffer(BCCX_Node *node, byte *obuf, int obsz)
{
	BCCX_ParseAbxe tctx;
	BCCX_ParseAbxe *ctx;
//	BCCX_Node *lst, *tmp;
	int na;

	ctx=&tctx;
	memset(ctx, 0, sizeof(BCCX_ParseAbxe));
	ctx->ct=obuf;
	ctx->cts=obuf;

	BCCX_AbxeEncodeNode(ctx, node);

	*ctx->ct++=0x00;
	return(ctx->ct-obuf);
}
