TKUCC_LinkObject *TKUCC_AsmAllocObject(TKUCC_MainContext *ctx)
{
	TKUCC_LinkObject *obj;
	
	obj=tkucc_malloc(sizeof(TKUCC_LinkObject));
	memset(obj, 0, sizeof(TKUCC_LinkObject));
	
	return(obj);
}

int TKUCC_ObjSetSection(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, char *name)
{
	int i, j;
	
	for(i=0; i<obj->n_sec; i++)
	{
		if(!strcmp(obj->sec[i]->name, name))
		{
			obj->c_sec=i;
			return(i);
		}
	}
	
	i=obj->n_sec++;
	obj->sec[i]=tkucc_malloc(sizeof(TKUCC_LinkObjectSec));
	memset(obj->sec[i], 0, sizeof(TKUCC_LinkObjectSec));
	obj->sec[i]->name=TKUCC_StrdupInternLink(ctx, name);
	return(i);
}

int TKUCC_AsmSetSection(TKUCC_MainContext *ctx, char *name)
{
	return(TKUCC_ObjSetSection(ctx, ctx->cur_obj, name));
}

int TKUCC_ObjGetSectionOffset(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int sec)
{
	return(obj->sec[sec]->ct_ofs);
}

int TKUCC_AsmGetOffset(TKUCC_MainContext *ctx)
{
	return(ctx->cur_obj->sec[ctx->cur_obj->c_sec]->ct_ofs);
}

void TKUCC_ObjSetSectionOffset(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int sec, int ofs)
{
	obj->sec[sec]->ct_ofs=ofs;
	if(ofs>obj->sec[sec]->ct_size)
		obj->sec[sec]->ct_size=ofs;
}

void TKUCC_AsmSetOffset(TKUCC_MainContext *ctx, int ofs)
{
	TKUCC_ObjSetSectionOffset(ctx, ctx->cur_obj,
		ctx->cur_obj->c_sec, ofs);
//	ctx->cur_obj->sec[ctx->cur_obj->c_sec]->ct_ofs=ofs;
}

int TKUCC_SecBufReadBytes(TKUCC_MainContext *ctx,
	TKUCC_LinkObjectSec *sec, int ofs, byte *buf, int sz)
{
	void *p;
	int v, ix, ix1;

	if(!sz)
		return(0);

	ix=ofs>>12;
	ix1=(ofs+(sz-1))>>12;
	if(ix!=ix1)
	{
		v=((ix+1)<<12)-ofs;
		TKUCC_SecBufReadBytes(ctx, sec, ofs, buf, v);
		TKUCC_SecBufReadBytes(ctx, sec, ofs+v, buf+v, sz-v);
		return(1);
	}

	if(!sec->buf)
	{
		memset(buf, 0, sz);
		return(0);
	}
	if(!sec->buf[ofs>>12])
	{
		memset(buf, 0, sz);
		return(0);
	}

	memcpy(buf, sec->buf[ofs>>12]+(ofs&4095), sz);
	return(1);
}

int TKUCC_SecBufWriteBytes(TKUCC_MainContext *ctx,
	TKUCC_LinkObjectSec *sec, int ofs, byte *buf, int sz)
{
	void *p;
	int v, ix, ix1;

	if(!sz)
		return(0);

	ix=ofs>>12;
	ix1=(ofs+(sz-1))>>12;
	if(ix!=ix1)
	{
		v=((ix+1)<<12)-ofs;
		TKUCC_SecBufWriteBytes(ctx, sec, ofs, buf, v);
		TKUCC_SecBufWriteBytes(ctx, sec, ofs+v, buf+v, sz-v);
		return(1);
	}

	if(!sec->buf)
	{
		p=tkucc_malloc(512*sizeof(byte *));
		sec->buf=p;
		memset(p, 0, 512*sizeof(byte *));
	}

	if(!sec->buf[ofs>>12])
	{
		p=tkucc_malloc(4096);
		sec->buf[ofs>>12]=p;
		memset(p, 0, 4096);
	}

	memcpy(sec->buf[ofs>>12]+(ofs&4095), buf, sz);
	return(1);
}



int TKUCC_ObjReadSectionByte(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int ofs, int sec)
{
	void *p;
	int v;

	if(!obj->sec[sec]->buf)
		return(0);
	if(!obj->sec[sec]->buf[ofs>>12])
		return(0);

	v=obj->sec[sec]->buf[ofs>>12][ofs&4095];
	return(v);
}

int TKUCC_ObjReadSectionBytes(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int ofs, int sec, byte *buf, int sz)
{
	int v;
	v=TKUCC_SecBufReadBytes(ctx, obj->sec[sec], ofs, buf, sz);
	return(v);
}

int TKUCC_ObjWriteSectionBytes(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int ofs, int sec, byte *buf, int sz)
{
	int v;
	v=TKUCC_SecBufWriteBytes(ctx, obj->sec[sec], ofs, buf, sz);
	return(v);
}

void TKUCC_ObjStoreSectionByte(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int ofs, int sec, int val)
{
	void *p;

	if(!obj->sec[sec]->buf)
	{
		p=tkucc_malloc(512*sizeof(byte *));
		obj->sec[sec]->buf=p;
		memset(p, 0, 512*sizeof(byte *));
	}

	if(!obj->sec[sec]->buf[ofs>>12])
	{
		p=tkucc_malloc(4096);
		obj->sec[sec]->buf[ofs>>12]=p;
		memset(p, 0, 4096);
	}

	obj->sec[sec]->buf[ofs>>12][ofs&4095]=val;
}

void TKUCC_ObjStoreSectionWord(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int ofs, int sec, u16 val)
{
	TKUCC_ObjStoreSectionByte(ctx, obj, ofs+0, sec, (val   )&0xFF);
	TKUCC_ObjStoreSectionByte(ctx, obj, ofs+1, sec, (val>>8)&0xFF);
}

void TKUCC_ObjStoreSectionDWord(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int ofs, int sec, u32 val)
{
	TKUCC_ObjStoreSectionWord(ctx, obj, ofs+0, sec, (val    )&0xFFFF);
	TKUCC_ObjStoreSectionWord(ctx, obj, ofs+2, sec, (val>>16)&0xFFFF);
}

void TKUCC_ObjStoreSectionQWord(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int ofs, int sec, u64 val)
{
	TKUCC_ObjStoreSectionWord(ctx, obj, ofs+0, sec, (val    )&0xFFFF);
	TKUCC_ObjStoreSectionWord(ctx, obj, ofs+2, sec, (val>>16)&0xFFFF);
	TKUCC_ObjStoreSectionWord(ctx, obj, ofs+4, sec, (val>>32)&0xFFFF);
	TKUCC_ObjStoreSectionWord(ctx, obj, ofs+6, sec, (val>>48)&0xFFFF);
}

void TKUCC_AsmEmitByte(TKUCC_MainContext *ctx, int val)
{
	TKUCC_LinkObject *obj;
	int ofs;
	
	obj=ctx->cur_obj;
	ofs=TKUCC_ObjGetSectionOffset(ctx, obj, obj->c_sec);
	TKUCC_ObjStoreSectionByte(ctx, obj, ofs, obj->c_sec, val&0xFF);
	TKUCC_ObjSetSectionOffset(ctx, obj, obj->c_sec, ofs+1);
}

void TKUCC_AsmEmitWord(TKUCC_MainContext *ctx, int val)
{
	TKUCC_LinkObject *obj;
	int ofs;
	
	obj=ctx->cur_obj;
	ofs=TKUCC_ObjGetSectionOffset(ctx, obj, obj->c_sec);
	TKUCC_ObjStoreSectionWord(ctx, obj, ofs, obj->c_sec, val&0xFFFF);
	TKUCC_ObjSetSectionOffset(ctx, obj, obj->c_sec, ofs+2);
}

void TKUCC_AsmEmitDWord(TKUCC_MainContext *ctx, u32 val)
{
	TKUCC_LinkObject *obj;
	int ofs;
	
	obj=ctx->cur_obj;
	ofs=TKUCC_ObjGetSectionOffset(ctx, obj, obj->c_sec);
	TKUCC_ObjStoreSectionDWord(ctx, obj, ofs, obj->c_sec, val);
	TKUCC_ObjSetSectionOffset(ctx, obj, obj->c_sec, ofs+4);
}

void TKUCC_AsmEmitQWord(TKUCC_MainContext *ctx, u64 val)
{
	TKUCC_LinkObject *obj;
	int ofs;
	
	obj=ctx->cur_obj;
	ofs=TKUCC_ObjGetSectionOffset(ctx, obj, obj->c_sec);
	TKUCC_ObjStoreSectionQWord(ctx, obj, ofs, obj->c_sec, val);
	TKUCC_ObjSetSectionOffset(ctx, obj, obj->c_sec, ofs+8);
}

void TKUCC_AsmEmitPad32Align(TKUCC_MainContext *ctx)
{
	TKUCC_LinkObject *obj;
	int ofs;
	
	obj=ctx->cur_obj;
	ofs=TKUCC_ObjGetSectionOffset(ctx, obj, obj->c_sec);
	
	if(ofs&1)
	{
		TKUCC_AsmEmitByte(ctx, 0);
	}

	if(ofs&2)
	{
		TKUCC_AsmEmitWord(ctx, 0x3000);
	}
}

void TKUCC_AsmEmitPadForLabel(TKUCC_MainContext *ctx)
{
}

void TKUCC_AsmEmitBAlign(TKUCC_MainContext *ctx, int algn)
{
	TKUCC_LinkObject *obj;
	int ofs, pad;
	
	if(algn<=1)
		return;
	if(algn&(algn-1))
		return;

	if(algn==4)
	{
		TKUCC_AsmEmitPad32Align(ctx);
		return;
	}

	obj=ctx->cur_obj;
	ofs=TKUCC_ObjGetSectionOffset(ctx, obj, obj->c_sec);
	pad=algn-(ofs&(algn-1));
	
	if(pad&1)
	{
		TKUCC_AsmEmitByte(ctx, 0);
		pad--;
	}

	if(pad&2)
	{
		TKUCC_AsmEmitWord(ctx, 0x3000);
		pad-=2;
	}
	
	while(pad)
	{
		TKUCC_AsmEmitWord(ctx, 0xF000);
		TKUCC_AsmEmitWord(ctx, 0x3000);
		pad-=4;
	}
}

void TKUCC_ObjGetLabel(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int idx,
	TKUCC_LinkObjectLbl *lbl)
{
	if(!obj->lbl)
	{
		memset(lbl, 0, sizeof(TKUCC_LinkObjectLbl));
		return;
	}
	if(!obj->lbl[idx>>10])
	{
		memset(lbl, 0, sizeof(TKUCC_LinkObjectLbl));
		return;
	}

	memcpy(lbl, obj->lbl[idx>>10]+(idx&1023), sizeof(TKUCC_LinkObjectLbl));
}

void TKUCC_ObjSetLabel(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int idx,
	TKUCC_LinkObjectLbl *lbl)
{
	if(!obj->lbl)
	{
		obj->lbl=tkucc_malloc(512*sizeof(TKUCC_LinkObjectLbl *));
		memset(obj->lbl, 0, 512*sizeof(TKUCC_LinkObjectLbl *));
	}
	if(!obj->lbl[idx>>10])
	{
		obj->lbl[idx>>10]=tkucc_malloc(1024*sizeof(TKUCC_LinkObjectLbl));
		memset(obj->lbl[idx>>10], 0, 1024*sizeof(TKUCC_LinkObjectLbl));
	}
	memcpy(obj->lbl[idx>>10]+(idx&1023), lbl, sizeof(TKUCC_LinkObjectLbl));
}

void TKUCC_ObjEmitLabel(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int lblid)
{
	TKUCC_LinkObjectLbl t_lbl;
	TKUCC_LinkObjectLbl *lbl;
	int i;
	
	if(!lblid)
	{
		TKUCC_DBGBREAK
	}
	
	lbl=&t_lbl;
	memset(lbl, 0, sizeof(TKUCC_LinkObjectLbl));
	lbl->sym_ofs=TKUCC_ObjGetSectionOffset(ctx, obj, obj->c_sec);
	lbl->sym_sec=obj->c_sec+1;
	lbl->sym_type=0;
	lbl->lbl_id=lblid;
	
	i=obj->n_lbl++;
	TKUCC_ObjSetLabel(ctx, obj, i, lbl);
}

void TKUCC_ObjGetReloc(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int idx,
	TKUCC_LinkObjectLbl *lbl)
{
	if(!obj->rlc)
	{
		memset(lbl, 0, sizeof(TKUCC_LinkObjectLbl));
		return;
	}
	if(!obj->rlc[idx>>10])
	{
		memset(lbl, 0, sizeof(TKUCC_LinkObjectLbl));
		return;
	}

	memcpy(lbl, obj->rlc[idx>>10]+(idx&1023), sizeof(TKUCC_LinkObjectLbl));
}

void TKUCC_ObjSetReloc(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int idx,
	TKUCC_LinkObjectLbl *lbl)
{
	if(!obj->rlc)
	{
		obj->rlc=tkucc_malloc(512*sizeof(TKUCC_LinkObjectLbl *));
		memset(obj->rlc, 0, 512*sizeof(TKUCC_LinkObjectLbl *));
	}
	if(!obj->rlc[idx>>10])
	{
		obj->rlc[idx>>10]=tkucc_malloc(1024*sizeof(TKUCC_LinkObjectLbl));
		memset(obj->rlc[idx>>10], 0, 1024*sizeof(TKUCC_LinkObjectLbl));
	}
	memcpy(obj->rlc[idx>>10]+(idx&1023), lbl, sizeof(TKUCC_LinkObjectLbl));
}

void TKUCC_ObjEmitReloc(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int lblid, int rlcty)
{
	TKUCC_LinkObjectLbl t_lbl;
	TKUCC_LinkObjectLbl *lbl;
	int i;
	
	if(!lblid || (rlcty<=0))
	{
		TKUCC_DBGBREAK
	}
	
	lbl=&t_lbl;
	memset(lbl, 0, sizeof(TKUCC_LinkObjectLbl));
	lbl->sym_ofs=TKUCC_ObjGetSectionOffset(ctx, obj, obj->c_sec);
	lbl->sym_sec=obj->c_sec+1;
	lbl->sym_type=rlcty;
	lbl->lbl_id=lblid;
	
	i=obj->n_rlc++;
	TKUCC_ObjSetReloc(ctx, obj, i, lbl);
}

void TKUCC_AsmEmitLabel(TKUCC_MainContext *ctx, int lblid)
{
	TKUCC_ObjEmitLabel(ctx, ctx->cur_obj, lblid);
}

void TKUCC_AsmEmitReloc(TKUCC_MainContext *ctx, int lblid, int rlcty)
{
	TKUCC_ObjEmitReloc(ctx, ctx->cur_obj, lblid, rlcty);
}

int TKUCC_ObjInternName(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, char *name)
{
	char tbuf[256];
	int i, j, k, l;
	
	if(!name)
		return(0);
	if(!(*name))
		return(1);

	if(!obj->strs)
	{
		obj->strs=tkucc_malloc(sizeof(TKUCC_LinkObjectSec));
		memset(obj->strs, 0, sizeof(TKUCC_LinkObjectSec));
	}

	l=strlen(name);
	k=obj->strs->ct_size;
	i=2;
	while(i<k)
	{
		TKUCC_SecBufReadBytes(ctx, obj->strs, i, tbuf, 255);
		tbuf[255]=0;
		if(!strcmp((char *)tbuf, name))
			return(i);
		i+=strlen((char *)tbuf)+1;
	}

	if(k<2)k=2;
	TKUCC_SecBufWriteBytes(ctx, obj->strs, k, name, l+1);
	obj->strs->ct_size=k+l+1;
	return(k);
}

int TKUCC_AsmGenSym(TKUCC_MainContext *ctx)
{
	int i;
	
	i=ctx->gensym_seq++;
	i|=TKUCC_LBLID_GENSYN;
	return(i);
}

int TKUCC_AsmGetNamedLabel(TKUCC_MainContext *ctx, char *name)
{
	int stix, lblid;
	int i, j, k;

	stix=TKUCC_InternStringToIndex(ctx, name, TKUCC_ZID_NAMESYM);
	
	for(i=0; i<ctx->namesym_max; i++)
	{
		if(ctx->namesym_stix[i>>10][i&1023]==stix)
		{
			return(ctx->namesym_lbl[i>>10][i&1023]);
		}
	}

	i=ctx->namesym_max++;

	if(!ctx->namesym_stix[i>>10])
	{
		ctx->namesym_stix[i>>10]=tkucc_malloc(1024*sizeof(int));
		ctx->namesym_lbl[i>>10]=tkucc_malloc(1024*sizeof(int));
	}
	
	lblid=TKUCC_AsmGenSym(ctx);
	
	ctx->namesym_stix[i>>10][i&1023]=stix;
	ctx->namesym_lbl[i>>10][i&1023]=lblid;
	return(lblid);
}

char *TKUCC_AsmGetNameForLabel(TKUCC_MainContext *ctx, int lblid)
{
	char tb[64];
	char *s;
	int stix;
	int i, j, k;

	for(i=0; i<ctx->namesym_max; i++)
	{
		if(ctx->namesym_lbl[i>>10][i&1023]==lblid)
		{
			stix=ctx->namesym_stix[i>>10][i&1023];
			s=TKUCC_InternIndexToString(ctx, stix);
			return(s);
		}
	}

	i=ctx->namesym_max++;

	if(!ctx->namesym_stix[i>>10])
	{
		ctx->namesym_stix[i>>10]=tkucc_malloc(1024*sizeof(int));
		ctx->namesym_lbl[i>>10]=tkucc_malloc(1024*sizeof(int));
	}
	
	sprintf(tb, "_L_%06X", lblid);
//	stix=TKUCC_StrdupInternNameSym(ctx, tb);
	stix=TKUCC_InternStringToIndex(ctx, tb, TKUCC_ZID_NAMESYM);
	ctx->namesym_stix[i>>10][i&1023]=stix;
	ctx->namesym_lbl[i>>10][i&1023]=lblid;

	s=TKUCC_InternIndexToString(ctx, stix);
	return(s);
}

void TKUCC_AsmEmitNamedLabel(TKUCC_MainContext *ctx, char *name)
{
	int lblid;
	lblid=TKUCC_AsmGetNamedLabel(ctx, name);
	TKUCC_ObjEmitLabel(ctx, ctx->cur_obj, lblid);
}

void TKUCC_AsmEmitNamedReloc(TKUCC_MainContext *ctx,
	char *name, int rlcty)
{
	int lblid;
	lblid=TKUCC_AsmGetNamedLabel(ctx, name);
	TKUCC_ObjEmitReloc(ctx, ctx->cur_obj, lblid, rlcty);
}

void TKUCC_AsmEmitAscii(TKUCC_MainContext *ctx,
	char *str)
{
	char *s;
	int i;
	
	s=str;
	while(*s)
	{
		i=TKUCC_ReadUTF8(&s);
		TKUCC_AsmEmitByte(ctx, i);
	}
}

void TKUCC_AsmEmitString(TKUCC_MainContext *ctx,
	char *str)
{
	TKUCC_AsmEmitAscii(ctx, str);
	TKUCC_AsmEmitByte(ctx, 0);
}

void TKUCC_AsmEmitAsciiUTF2UCS2(TKUCC_MainContext *ctx,
	char *str)
{
	byte *s;
	int i;
	
	s=(byte *)str;
	while(*s)
	{
		i=TKUCC_ReadUTF8(&s);
		TKUCC_AsmEmitWord(ctx, i);
	}
}

void TKUCC_AsmEmitStringUTF2UCS2(TKUCC_MainContext *ctx,
	char *str)
{
	TKUCC_AsmEmitAsciiUTF2UCS2(ctx, str);
	TKUCC_AsmEmitWord(ctx, 0);
}

void TKUCC_AsmEmitNamedGlobal(TKUCC_MainContext *ctx,
	char *str)
{
}

void TKUCC_AsmEmitNamedCommSym(TKUCC_MainContext *ctx,
	char *str, int sz, int al)
{
}
