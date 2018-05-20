#include <bgbccc.h>

char *bgbcc_bsr_srcidx[256];
int bgbcc_bsr_nsrcidx=0;

int bgbcc_bsr_indexSrcIdx(char *name)
{
	int i, j, k;

	for(i=0; i<bgbcc_bsr_nsrcidx; i++)
	{
		if(!strcmp(name, bgbcc_bsr_srcidx[i]))
			return(i);
	}
	
	i=bgbcc_bsr_nsrcidx++;
	bgbcc_bsr_srcidx[i]=name;
	return(i);
}

int BGBCC_BSR_GenLabelTempLLn(BGBCC_BSR_Context *ctx, char *file, int line)
{
	int i, j, k;

	i=ctx->lbltrov++;
	
	if(!ctx->genlabel_srcpos)
	{
		ctx->genlabel_srcpos=ctx->t_genlabel_srcpos;
		ctx->genlabel_limit=4096;
	}
	
	j=bgbcc_bsr_indexSrcIdx(file);
	k=(j<<16)|line;
	if(ctx->is_simpass)
		k|=0x8000;
	if(i<ctx->genlabel_limit)
		ctx->genlabel_srcpos[i]=k;
	
	return(CCXL_LBL_GENSYM2BASE2+i);
}

int BGBCC_BSR_GenLabelLLn(BGBCC_BSR_Context *ctx, char *file, int line)
{
//	int i;
//	return(BGBCC_BSR_GenLabelTempLLn(ctx, file, line));

	int i, j, k;

	i=ctx->lblrov++;
	
	if(!ctx->genlabel_srcpos)
	{
		ctx->genlabel_srcpos=ctx->t_genlabel_srcpos;
		ctx->genlabel_limit=4096;
	}
	
	j=bgbcc_bsr_indexSrcIdx(file);
	k=(j<<16)|line;
	if(ctx->is_simpass)
		k|=0x8000;
	if(i<ctx->genlabel_limit)
		ctx->genlabel_srcpos[i]=k;
	
	return(CCXL_LBL_GENSYM2BASE+i);
}

int BGBCC_BSR_CheckExpandLabel(BGBCC_BSR_Context *ctx)
{
	int h;
	int i;
	
	if(!ctx->lbl_ofs)
	{
		for(i=0; i<1024; i++)
			ctx->lbl_hash[i]=(-1);
	
//		i=4096;
		i=65536;
		ctx->lbl_ofs=bgbcc_malloc(i*sizeof(u32));
		ctx->lbl_id =bgbcc_malloc(i*sizeof(u32));
		ctx->lbl_sec=bgbcc_malloc(i*sizeof(byte));
		ctx->lbl_chn=bgbcc_malloc(i*sizeof(s32));
		ctx->nlbl=1;
		ctx->nvlbl=1;
		ctx->mlbl=i;
	}
	
//	if((ctx->nlbl+1)>=ctx->mlbl)
	if(((ctx->nlbl+1)>=ctx->mlbl) ||
		((ctx->nvlbl+1)>=ctx->mlbl))
	{
		i=ctx->mlbl+(ctx->mlbl>>1);
		ctx->lbl_ofs=bgbcc_realloc(ctx->lbl_ofs, i*sizeof(u32));
		ctx->lbl_id =bgbcc_realloc(ctx->lbl_id , i*sizeof(u32));
		ctx->lbl_sec=bgbcc_realloc(ctx->lbl_sec, i*sizeof(byte));
		ctx->lbl_chn=bgbcc_realloc(ctx->lbl_chn, i*sizeof(s32));
		ctx->mlbl=i;
	}

	return(0);
}

int BGBCC_BSR_EmitLabel(BGBCC_BSR_Context *ctx, int lblid)
{
	int h;
	int i;
	
	if(lblid<=0)
		return(0);

	BGBCC_BSR_CheckExpandLabel(ctx);

	if(ctx->is_simpass)
	{
		i=ctx->nvlbl++;

		if(ctx->lbl_id[i]==lblid)
		{
			ctx->sim_voffs=BGBCC_BSR_EmitGetOffs(ctx)-ctx->lbl_ofs[i];
			if((ctx->sim_voffs!=0) && ctx->is_stable)
				ctx->is_stable|=2;
		}else
		{
			ctx->is_stable|=6;
		}

		ctx->lbl_id[i]=lblid;
		ctx->lbl_ofs[i]=BGBCC_BSR_EmitGetOffs(ctx);
		ctx->lbl_sec[i]=ctx->sec;
		return(i);
	}

	i=ctx->nlbl;
	if(ctx->lbl_id[i]==lblid)
	{
		ctx->sim_voffs=BGBCC_BSR_EmitGetOffs(ctx)-ctx->lbl_ofs[i];
		if((ctx->sim_voffs!=0) && ctx->is_stable)
			ctx->is_stable|=2;
	}

	h=lblid*65521+31;
	h=h*65521+31;
	h=(h>>16)&1023;

	i=ctx->nlbl++;
	ctx->lbl_id[i]=lblid;
	ctx->lbl_ofs[i]=BGBCC_BSR_EmitGetOffs(ctx);
	ctx->lbl_sec[i]=ctx->sec;
	
	ctx->lbl_chn[i]=ctx->lbl_hash[h];
	ctx->lbl_hash[h]=i;
	
	if((ctx->lbl_ofs[i]>>20) && (ctx->sec!=BGBCC_SH_CSEG_BSS))
		{ BGBCC_DBGBREAK }
	return(i);
}

#if 1
int BGBCC_BSR_EmitLabelAbs(BGBCC_BSR_Context *ctx, int lblid, s64 addr)
{
	int h;
	int i;
	
	if(lblid<=0)
		return(0);

	BGBCC_BSR_CheckExpandLabel(ctx);

	if(ctx->is_simpass)
	{
		return(0);
	}

	h=lblid*65521+31;
	h=h*65521+31;
	h=(h>>16)&1023;

	i=ctx->nlbl++;
	ctx->lbl_id[i]=lblid;
	ctx->lbl_ofs[i]=addr;
	ctx->lbl_sec[i]=BGBCC_SH_CSEG_ABS;
	
	ctx->lbl_chn[i]=ctx->lbl_hash[h];
	ctx->lbl_hash[h]=i;
	
	return(i);
}
#endif

char *BGBCC_BSR_LookupNameForLabel(BGBCC_BSR_Context *ctx, int lblid)
{
	int i;
	
	for(i=0; i<ctx->nlbln; i++)
	{
		if(ctx->lbln_id[i]==lblid)
			return(ctx->lbln_name[i]);
//		if(!strcmp(ctx->lbln_name[i], name))
//			return(ctx->lbln_id[i]);
	}
	return(NULL);
}

int BGBCC_BSR_LookupNamedLabel(BGBCC_BSR_Context *ctx, char *name)
{
	int i;
	
	for(i=0; i<ctx->nlbln; i++)
	{
		if(!strcmp(ctx->lbln_name[i], name))
			return(ctx->lbln_id[i]);
	}
	return(0);
}

int BGBCC_BSR_GetNamedLabel(BGBCC_BSR_Context *ctx, char *name)
{
	int lbl;
	int i;

	lbl=BGBCC_BSR_LookupNamedLabel(ctx, name);
	if(lbl>0)
		return(lbl);
	
	if(!ctx->lbln_name)
	{
//		i=1024;
		i=4096;
		ctx->lbln_name=bgbcc_malloc(i*sizeof(char *));
		ctx->lbln_id =bgbcc_malloc(i*sizeof(u32));
		ctx->nlbln=0;
		ctx->mlbln=i;
	}
	
	if((ctx->nlbln+1)>=ctx->mlbln)
	{
		i=ctx->mlbln+(ctx->mlbln>>1);
		ctx->lbln_name=bgbcc_realloc(ctx->lbln_name, i*sizeof(char *));
		ctx->lbln_id =bgbcc_realloc(ctx->lbln_id , i*sizeof(u32));
		ctx->mlbln=i;
	}

	if(ctx->is_simpass)
	{
		return(-1);
	}

	lbl=BGBCC_BSR_GenLabel(ctx);
	i=ctx->nlbln++;
	ctx->lbln_id[i]=lbl;
	ctx->lbln_name[i]=bgbcc_strdup(name);
	return(lbl);
}

int BGBCC_BSR_EmitNamedLabel(BGBCC_BSR_Context *ctx, char *name)
{
	int lbl;
	
	lbl=BGBCC_BSR_GetNamedLabel(ctx, name);
	if(lbl>0)
	{
		return(BGBCC_BSR_EmitLabel(ctx, lbl));
	}
	return(0);
}

int BGBCC_BSR_EmitNamedGlobal(BGBCC_BSR_Context *ctx, char *name)
{
	return(0);
}

int BGBCC_BSR_EmitCommSym(BGBCC_BSR_Context *ctx, int lblid, int sz)
{
	int i;

	for(i=0; i<ctx->nlbl; i++)
		if(ctx->lbl_id[i]==lblid)
			return(0);

	if(sz>>16)
		i=-1;

	i=ctx->sec;
	BGBCC_BSR_SetSectionName(ctx, ".bss");
	BGBCC_BSR_EmitBAlign(ctx, 4);
	BGBCC_BSR_EmitLabel(ctx, lblid);
	BGBCC_BSR_EmitRawBytes(ctx, NULL, sz);
	ctx->sec=i;
	return(1);
}

int BGBCC_BSR_EmitNamedCommSym(BGBCC_BSR_Context *ctx, char *name, int sz)
{
	int lbl;
	
	lbl=BGBCC_BSR_LookupNamedLabel(ctx, name);
	return(BGBCC_BSR_EmitCommSym(ctx, lbl, sz));

//	BGBCC_BSR_SetSectionName(sctx, ".bss");
//	BGBCC_BSR_EmitBAlign(sctx, 4);
//	BGBCC_BSR_EmitNamedLabel(sctx, name);
//	BGBCC_BSR_EmitRawBytes(sctx, NULL, sz);
//	return(1);
}

int BGBCC_BSR_EmitRelocTy(BGBCC_BSR_Context *ctx, int lblid, int ty)
{
	int i;
	
	if(lblid<=0)
		return(0);

	if(!ctx->rlc_ofs)
	{
//		i=4096;
		i=65536;
		ctx->rlc_ofs=bgbcc_malloc(i*sizeof(u32));
		ctx->rlc_id =bgbcc_malloc(i*sizeof(u32));
		ctx->rlc_sec=bgbcc_malloc(i*sizeof(byte));
		ctx->rlc_ty =bgbcc_malloc(i*sizeof(byte));
		ctx->nrlc=0;
		ctx->mrlc=i;
	}
	
	if((ctx->nrlc+1)>=ctx->mrlc)
	{
		i=ctx->mrlc+(ctx->mrlc>>1);
		ctx->rlc_ofs=bgbcc_realloc(ctx->rlc_ofs, i*sizeof(u32));
		ctx->rlc_id =bgbcc_realloc(ctx->rlc_id , i*sizeof(u32));
		ctx->rlc_sec=bgbcc_realloc(ctx->rlc_sec, i*sizeof(byte));
		ctx->rlc_ty =bgbcc_realloc(ctx->rlc_ty , i*sizeof(byte));
		ctx->mrlc=i;
	}

	if(ctx->is_simpass)
	{
		return(0);
	}

	i=ctx->nrlc++;
	ctx->rlc_id[i]=lblid;
	ctx->rlc_ofs[i]=BGBCC_BSR_EmitGetOffs(ctx);
	ctx->rlc_sec[i]=ctx->sec;
	ctx->rlc_ty[i]=ty;
	return(1);
}

int BGBCC_BSR_EmitNamedReloc(BGBCC_BSR_Context *ctx, char *name, int ty)
{
	int lbl;
	
	lbl=BGBCC_BSR_GetNamedLabel(ctx, name);
	if(lbl>0)
	{
		return(BGBCC_BSR_EmitRelocTy(ctx, lbl, ty));
	}
	return(0);
}

int BGBCC_BSR_EmitRelocRel8(BGBCC_BSR_Context *ctx, int lbl)
	{ return(BGBCC_BSR_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_REL8)); }
int BGBCC_BSR_EmitRelocRel16(BGBCC_BSR_Context *ctx, int lbl)
	{ return(BGBCC_BSR_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_REL16)); }
int BGBCC_BSR_EmitRelocRel32(BGBCC_BSR_Context *ctx, int lbl)
	{ return(BGBCC_BSR_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_REL32)); }

int BGBCC_BSR_EmitRelocAbs8(BGBCC_BSR_Context *ctx, int lbl)
	{ return(BGBCC_BSR_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_ABS8)); }
int BGBCC_BSR_EmitRelocAbs16(BGBCC_BSR_Context *ctx, int lbl)
	{ return(BGBCC_BSR_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_ABS16)); }
int BGBCC_BSR_EmitRelocAbs32(BGBCC_BSR_Context *ctx, int lbl)
	{ return(BGBCC_BSR_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_ABS32)); }
int BGBCC_BSR_EmitRelocAbs64(BGBCC_BSR_Context *ctx, int lbl)
	{ return(BGBCC_BSR_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_ABS64)); }

int BGBCC_BSR_EmitRelocRelW12(BGBCC_BSR_Context *ctx, int lbl)
	{ return(BGBCC_BSR_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_RELW12)); }
int BGBCC_BSR_EmitRelocRelW8(BGBCC_BSR_Context *ctx, int lbl)
	{ return(BGBCC_BSR_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_RELW8)); }


int BGBCC_BSR_LookupLabelIndex(
	BGBCC_BSR_Context *sctx, int lblid)
{
	int c, h;
	int i, j, k;
	
	h=lblid*65521+31;
	h=h*65521+31;
	h=(h>>16)&1023;

	c=sctx->lbl_hash[h];
	while(c>=0)
	{
		if(sctx->lbl_id[c]==lblid)
			return(c);
		c=sctx->lbl_chn[c];
	}
	return(-1);

#if 0	
	for(i=0; i<sctx->nlbl; i++)
	{
		if(sctx->lbl_id[i]==lblid)
			return(i);
	}
	return(-1);
#endif
}

int BGBCC_BSR_LookupSimLabelIndex(
	BGBCC_BSR_Context *sctx, int lblid)
{
	int i, j, k;
	
	for(i=sctx->nlbl; i<sctx->nvlbl; i++)
	{
		if(sctx->lbl_id[i]==lblid)
			return(i);
	}

	i=BGBCC_BSR_LookupLabelIndex(sctx, lblid);
	return(i);
}
