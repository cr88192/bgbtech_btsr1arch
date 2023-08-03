void TKUCC_AsmSetSection(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, char *name)
{
	int i, j;
	
	for(i=0; i<obj->n_sec; i++)
	{
		if(!strcmp(obj->sec[i]->name, name))
		{
			obj->c_sec=i;
			return;
		}
	}
	
	i=obj->n_sec++;
	obj->sec[i]=tkucc_malloc(sizeof(TKUCC_LinkObjectSec));
	memset(obj->sec[i], 0, sizeof(TKUCC_LinkObjectSec));
	obj->sec[i]->name=TKUCC_StrdupInternLink(ctx, name);
}

int TKUCC_AsmGetSectionOffset(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj)
{
	return(obj->sec[obj->c_sec]->ct_ofs);
}

void TKUCC_AsmSetSectionOffset(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int ofs)
{
	obj->sec[obj->c_sec]->ct_ofs=ofs;
}

int TKUCC_AsmReadSecBufBytes(TKUCC_MainContext *ctx,
	TKUCC_LinkObjectSec *sec, int ofs, byte *buf, int sz)
{
	void *p;
	int v, ix, ix1;

	ix=ofs>>12;
	ix1=(ofs+(sz-1))>>12;
	if(ix!=ix1)
	{
		v=((ix+1)<<12)-ofs;
		TKUCC_AsmReadSecBufBytes(ctx, sec, ofs, buf, v);
		TKUCC_AsmReadSecBufBytes(ctx, sec, ofs+v, buf+v, sz-v);
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

int TKUCC_AsmWriteSecBufBytes(TKUCC_MainContext *ctx,
	TKUCC_LinkObjectSec *sec, int ofs, byte *buf, int sz)
{
	void *p;
	int v, ix, ix1;

	ix=ofs>>12;
	ix1=(ofs+(sz-1))>>12;
	if(ix!=ix1)
	{
		v=((ix+1)<<12)-ofs;
		TKUCC_AsmWriteSecBufBytes(ctx, sec, ofs, buf, v);
		TKUCC_AsmWriteSecBufBytes(ctx, sec, ofs+v, buf+v, sz-v);
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



int TKUCC_AsmReadSectionByte(TKUCC_MainContext *ctx,
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

int TKUCC_AsmReadSectionBytes(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int ofs, int sec, byte *buf, int sz)
{
	int v;
	v=TKUCC_AsmReadSecBufBytes(ctx, obj->sec[sec], ofs, buf, sz);
	return(v);
}

void TKUCC_AsmStoreSectionByte(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int ofs, int val)
{
	void *p;

	if(!obj->sec[obj->c_sec]->buf)
	{
		p=tkucc_malloc(512*sizeof(byte *));
		obj->sec[obj->c_sec]->buf=p;
		memset(p, 0, 512*sizeof(byte *));
	}

	if(!obj->sec[obj->c_sec]->buf[ofs>>12])
	{
		p=tkucc_malloc(4096);
		obj->sec[obj->c_sec]->buf[ofs>>12]=p;
		memset(p, 0, 4096);
	}

	obj->sec[obj->c_sec]->buf[ofs>>12][ofs&4095]=val;
}

void TKUCC_AsmStoreSectionWord(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int ofs, int val)
{
	TKUCC_AsmStoreSectionByte(ctx, obj, ofs+0, (val   )&0xFF);
	TKUCC_AsmStoreSectionByte(ctx, obj, ofs+1, (val>>8)&0xFF);
}

void TKUCC_AsmStoreSectionDWord(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int ofs, u32 val)
{
	TKUCC_AsmStoreSectionWord(ctx, obj, ofs+0, (val    )&0xFFFF);
	TKUCC_AsmStoreSectionWord(ctx, obj, ofs+2, (val>>16)&0xFFFF);
}

void TKUCC_AsmStoreSectionQWord(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int ofs, u64 val)
{
	TKUCC_AsmStoreSectionWord(ctx, obj, ofs+0, (val    )&0xFFFF);
	TKUCC_AsmStoreSectionWord(ctx, obj, ofs+2, (val>>16)&0xFFFF);
	TKUCC_AsmStoreSectionWord(ctx, obj, ofs+4, (val>>32)&0xFFFF);
	TKUCC_AsmStoreSectionWord(ctx, obj, ofs+6, (val>>48)&0xFFFF);
}

void TKUCC_AsmEmitSectionWord(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int val)
{
	int ofs;
	
	ofs=TKUCC_AsmGetSectionOffset(ctx, obj);
	TKUCC_AsmStoreSectionWord(ctx, obj, ofs, val&0xFFFF);
	TKUCC_AsmSetSectionOffset(ctx, obj, ofs+2);
}

void TKUCC_AsmEmitSectionDWord(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, u32 val)
{
	int ofs;
	
	ofs=TKUCC_AsmGetSectionOffset(ctx, obj);
	TKUCC_AsmStoreSectionDWord(ctx, obj, ofs, val);
	TKUCC_AsmSetSectionOffset(ctx, obj, ofs+4);
}

void TKUCC_AsmEmitSectionQWord(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, u64 val)
{
	int ofs;
	
	ofs=TKUCC_AsmGetSectionOffset(ctx, obj);
	TKUCC_AsmStoreSectionQWord(ctx, obj, ofs, val);
	TKUCC_AsmSetSectionOffset(ctx, obj, ofs+8);
}

void TKUCC_AsmObjGetLabel(TKUCC_MainContext *ctx,
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

void TKUCC_AsmObjSetLabel(TKUCC_MainContext *ctx,
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

void TKUCC_AsmEmitLabel(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int lblid)
{
	TKUCC_LinkObjectLbl t_lbl;
	TKUCC_LinkObjectLbl *lbl;
	int i;
	
	lbl=&t_lbl;
	memset(lbl, 0, sizeof(TKUCC_LinkObjectLbl));
	lbl->sym_ofs=TKUCC_AsmGetSectionOffset(ctx, obj);
	lbl->sym_sec=obj->c_sec;
	lbl->lbl_id=lblid;
	
	i=obj->n_lbl++;
	TKUCC_AsmObjSetLabel(ctx, obj, i, lbl);
}

void TKUCC_AsmObjGetReloc(TKUCC_MainContext *ctx,
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

void TKUCC_AsmObjSetReloc(TKUCC_MainContext *ctx,
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

void TKUCC_AsmEmitReloc(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int lblid, int rlcty)
{
	TKUCC_LinkObjectLbl t_lbl;
	TKUCC_LinkObjectLbl *lbl;
	int i;
	
	lbl=&t_lbl;
	memset(lbl, 0, sizeof(TKUCC_LinkObjectLbl));
	lbl->sym_ofs=TKUCC_AsmGetSectionOffset(ctx, obj);
	lbl->sym_sec=obj->c_sec;
	lbl->lbl_id=lblid;
	
	i=obj->n_lbl++;
	TKUCC_AsmObjSetLabel(ctx, obj, i, lbl);
}

int TKUCC_AsmObjInternName(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, char *name)
{
	char tbuf[256];
	int i, j, k, l;
	
	if(!name)
		return(0);
	if(!(*name))
		return(1);
	
	l=strlen(name);
	k=obj->strs->ct_size;
	i=2;
	while(i<k)
	{
		TKUCC_AsmReadSecBufBytes(ctx, obj->strs, i, tbuf, 255);
		tbuf[255]=0;
		if(!strcmp((char *)tbuf, name))
			return(i);
		i+=strlen((char *)tbuf)+1;
	}

	if(k<2)k=2;
	TKUCC_AsmWriteSecBufBytes(ctx, obj->strs, k, name, l+1);
	obj->strs->ct_size=k+l+1;
	return(k);
}

int TKUCC_AsmGetNamedLabel(TKUCC_MainContext *ctx, char *name)
{
}

char *TKUCC_AsmGetNameForLabel(TKUCC_MainContext *ctx, int lbl)
{
}
