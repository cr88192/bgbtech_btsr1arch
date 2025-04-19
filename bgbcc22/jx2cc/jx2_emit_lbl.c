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

#include <bgbccc.h>

char *bgbcc_jx2_srcidx[256];
int bgbcc_jx2_nsrcidx=0;

int bgbcc_jx2_indexSrcIdx(char *name)
{
	int i, j, k;

	for(i=0; i<bgbcc_jx2_nsrcidx; i++)
	{
		if(!strcmp(name, bgbcc_jx2_srcidx[i]))
			return(i);
	}
	
	i=bgbcc_jx2_nsrcidx++;
	bgbcc_jx2_srcidx[i]=name;
	return(i);
}

char *bgbcc_jx2_NameForSrcIdx(int idx)
{
	return(bgbcc_jx2_srcidx[idx]);
}

int BGBCC_JX2_GenLabelTempLLn(BGBCC_JX2_Context *ctx, char *file, int line)
{
	int i, j, k;

	i=ctx->lbltrov++;
	
	if(!ctx->genlabel_srcpos)
	{
		ctx->genlabel_srcpos=ctx->t_genlabel_srcpos;
		ctx->genlabel_limit=4096;
	}
	
	j=bgbcc_jx2_indexSrcIdx(file);
	k=(j<<16)|line;
	if(ctx->is_simpass)
		k|=0x8000;
	if(i<ctx->genlabel_limit)
		ctx->genlabel_srcpos[i]=k;
	
	return(CCXL_LBL_GENSYM2BASE2+i);
}

int BGBCC_JX2_GenLabelLLn(BGBCC_JX2_Context *ctx, char *file, int line)
{
//	int i;
//	return(BGBCC_JX2_GenLabelTempLLn(ctx, file, line));

	int i, j, k;

	i=ctx->lblrov++;
	
	if(!ctx->genlabel_srcpos)
	{
		ctx->genlabel_srcpos=ctx->t_genlabel_srcpos;
		ctx->genlabel_limit=4096;
	}
	
	j=bgbcc_jx2_indexSrcIdx(file);
	k=(j<<16)|line;
	if(ctx->is_simpass)
		k|=0x8000;
	if(i<ctx->genlabel_limit)
		ctx->genlabel_srcpos[i]=k;
	
	return(CCXL_LBL_GENSYM2BASE+i);
}

int BGBCC_JX2_CheckExpandLabel(BGBCC_JX2_Context *ctx)
{
	int h;
	int i;
	
	if(!ctx->lbl_ofs)
	{
		for(i=0; i<1024; i++)
		{
			ctx->lbl_hash[i]=(-1);
			ctx->lbl_hashb[i]=(-1);
		}
	
//		i=4096;
		i=65536;
		ctx->lbl_ofs=bgbcc_malloc(i*sizeof(u32));
		ctx->lbl_id =bgbcc_malloc(i*sizeof(u32));
		ctx->lbl_sec=bgbcc_malloc(i*sizeof(byte));
		ctx->lbl_chn=bgbcc_malloc(i*sizeof(s32));
		ctx->lbl_cho=bgbcc_malloc(i*sizeof(s32));
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
		ctx->lbl_cho=bgbcc_realloc(ctx->lbl_cho, i*sizeof(s32));
		ctx->mlbl=i;
	}

	return(0);
}

int BGBCC_JX2_EmitDebugLine(BGBCC_JX2_Context *ctx,
	char *lfn, int lln)
{
	int lbl;
	int i, j, k;

	if(!ctx->is_simpass)
	{
		i=ctx->lblllnrov;
		while(i>0)
		{
			k=ctx->dbglln_srcpos[i-1];
			if(!(k&0x8000))
				break;
			i--;
		}
		ctx->lblllnrov=i;
	}

	i=ctx->lblllnrov++;
	
	if(!ctx->dbglln_srcpos)
	{
		k=4096;
//		ctx->dbglln_srcpos=ctx->t_dbglln_srcpos;
		ctx->dbglln_srcpos=bgbcc_malloc(k*sizeof(int));
		ctx->dbglln_limit=k;
	}

	if(i>=ctx->dbglln_limit)
	{
		k=ctx->dbglln_limit;
		k=k+(k>>1);
		ctx->dbglln_srcpos=bgbcc_realloc(ctx->dbglln_srcpos, k*sizeof(int));
		ctx->dbglln_limit=k;
	}

	j=bgbcc_jx2_indexSrcIdx(lfn);
	k=(j<<16)|lln;
	if(ctx->is_simpass)
		k|=0x8000;
//	if(i<ctx->dbglln_limit)
	ctx->dbglln_srcpos[i]=k;

	lbl=(CCXL_LBL_GENLLNBASE+i);
	return(BGBCC_JX2_EmitLabel(ctx, lbl));
}

int BGBCC_JX2_LabelIsDebugLineP(BGBCC_JX2_Context *ctx, int lbl)
{
	if(lbl<CCXL_LBL_GENLLNBASE)
		return(0);
	if(lbl>=(CCXL_LBL_GENLLNBASE+0x10000))
		return(0);
	return(1);
}

int BGBCC_JX2_GetDebugLineForLabel(BGBCC_JX2_Context *ctx,
	int lblid, char **rlfn, int *rlln)
{
	int i, j, k;

	for(i=0; i<ctx->lblllnrov; i++)
	{
		k=ctx->dbglln_srcpos[i];
		if(k&0x8000)
			continue;

		j=(CCXL_LBL_GENLLNBASE+i);
		if(j==lblid)
		{
			*rlfn=bgbcc_jx2_NameForSrcIdx(k>>16);
			*rlln=k&65535;
			return(1);
		}
	}

	*rlfn="?";
	*rlln=0;
	return(0);
}

int BGBCC_JX2_EmitLabel(BGBCC_JX2_Context *ctx, int lblid)
{
	int h, h2, sec, ofs;
	int i;
	
	if(lblid<=0)
		return(0);

	BGBCC_JX2_CheckExpandLabel(ctx);

	if((lblid&CCXL_LBL_SPMASK)!=CCXL_LBL_GENLLNBASE)
	{
		/* Only flush state for actual labels. */
	
	//	for(i=0; i<32; i++)
		for(i=0; i<64; i++)
			ctx->reg_pszx[i]=0;

		ctx->pos_pad_op7=-999;
		ctx->pos_pad_op6=-999;
		ctx->pos_pad_op5=-999;
		ctx->pos_pad_op4=-999;
		ctx->pos_pad_op3=-999;
		ctx->pos_pad_op2=-999;
		ctx->pos_pad_op1=-999;
		ctx->pos_pad_op0=-999;
		
		ctx->reg_iszero=0;
	}

	ofs=BGBCC_JX2_EmitGetOffs(ctx);
	sec=ctx->sec;

	if(ctx->is_simpass)
	{
		h=lblid*65521+31;
		h=h*65521+31;
		h=(h>>16)&63;

		h2=(((ofs*251+sec)*251)>>8)&63;

		i=ctx->nvlbl++;

		if(ctx->lbl_id[i]==lblid)
		{
			ctx->sim_voffs=BGBCC_JX2_EmitGetOffs(ctx)-ctx->lbl_ofs[i];
			if((ctx->sim_voffs!=0) && ctx->is_stable)
				ctx->is_stable|=2;
		}else
		{
			ctx->is_stable|=6;
		}

		ctx->lbl_id[i]=lblid;
		ctx->lbl_ofs[i]=BGBCC_JX2_EmitGetOffs(ctx);
		ctx->lbl_sec[i]=ctx->sec;

		ctx->lbl_chn[i]=ctx->lbl_simhash[h];
		ctx->lbl_simhash[h]=i;

		ctx->lbl_cho[i]=ctx->lbl_simhashb[h2];
		ctx->lbl_simhashb[h2]=i;

		return(i);
	}

	i=ctx->nlbl;
	if(ctx->lbl_id[i]==lblid)
	{
		ctx->sim_voffs=BGBCC_JX2_EmitGetOffs(ctx)-ctx->lbl_ofs[i];
		if((ctx->sim_voffs!=0) && ctx->is_stable)
			ctx->is_stable|=2;
	}

	h=lblid*65521+31;
	h=h*65521+31;
	h=(h>>16)&1023;

	h2=(((ofs*251+sec)*251)>>8)&1023;

	i=ctx->nlbl++;
	ctx->lbl_id[i]=lblid;
	ctx->lbl_ofs[i]=BGBCC_JX2_EmitGetOffs(ctx);
	ctx->lbl_sec[i]=ctx->sec;
	
	ctx->lbl_chn[i]=ctx->lbl_hash[h];
	ctx->lbl_hash[h]=i;

	ctx->lbl_cho[i]=ctx->lbl_hashb[h2];
	ctx->lbl_hashb[h2]=i;
	
//	if((ctx->lbl_ofs[i]>>20) && (ctx->sec!=BGBCC_SH_CSEG_BSS))
//		{ BGBCC_DBGBREAK }
	
	BGBCC_JX2DA_EmitLabel(ctx, lblid);
	
	return(i);
}

int BGBCC_JX2_EmitLabelOffs(BGBCC_JX2_Context *ctx, int lblid, int offs)
{
	int h, h2, sec, ofs;
	int i;
	
	if(lblid<=0)
		return(0);

	BGBCC_JX2_CheckExpandLabel(ctx);

//	ofs=BGBCC_JX2_EmitGetOffs(ctx);
	sec=ctx->sec;
	ofs=offs;

	if(ctx->is_simpass)
	{
		h=lblid*65521+31;
		h=h*65521+31;
		h=(h>>16)&63;

		h2=(((ofs*251+sec)*251)>>8)&63;

		i=ctx->nvlbl++;

		if(ctx->lbl_id[i]==lblid)
		{
			ctx->sim_voffs=ofs-ctx->lbl_ofs[i];
			if((ctx->sim_voffs!=0) && ctx->is_stable)
				ctx->is_stable|=2;
		}else
		{
			ctx->is_stable|=6;
		}

		ctx->lbl_id[i]=lblid;
		ctx->lbl_ofs[i]=ofs;
		ctx->lbl_sec[i]=ctx->sec;

		ctx->lbl_chn[i]=ctx->lbl_simhash[h];
		ctx->lbl_simhash[h]=i;

		ctx->lbl_cho[i]=ctx->lbl_simhashb[h2];
		ctx->lbl_simhashb[h2]=i;

		return(i);
	}

	i=ctx->nlbl;
	if(ctx->lbl_id[i]==lblid)
	{
		ctx->sim_voffs=ofs-ctx->lbl_ofs[i];
		if((ctx->sim_voffs!=0) && ctx->is_stable)
			ctx->is_stable|=2;
	}

	h=lblid*65521+31;
	h=h*65521+31;
	h=(h>>16)&1023;

	h2=(((ofs*251+sec)*251)>>8)&1023;

	i=ctx->nlbl++;
	ctx->lbl_id[i]=lblid;
	ctx->lbl_ofs[i]=ofs;
	ctx->lbl_sec[i]=ctx->sec;
	
	ctx->lbl_chn[i]=ctx->lbl_hash[h];
	ctx->lbl_hash[h]=i;

	ctx->lbl_cho[i]=ctx->lbl_hashb[h2];
	ctx->lbl_hashb[h2]=i;
		
//	BGBCC_JX2DA_EmitLabel(ctx, lblid);
	
	return(i);
}

#if 1
int BGBCC_JX2_EmitLabelAbs(BGBCC_JX2_Context *ctx, int lblid, s64 addr)
{
	int h, h2;
	int i;
	
	if(lblid<=0)
		return(0);

	BGBCC_JX2_CheckExpandLabel(ctx);

	if(ctx->is_simpass)
	{
		return(0);
	}

	h=lblid*65521+31;
	h=h*65521+31;
	h=(h>>16)&1023;

	h2=(((addr*251+BGBCC_SH_CSEG_ABS)*251)>>8)&1023;

	i=ctx->nlbl++;
	ctx->lbl_id[i]=lblid;
	ctx->lbl_ofs[i]=addr;
	ctx->lbl_sec[i]=BGBCC_SH_CSEG_ABS;
	
	ctx->lbl_chn[i]=ctx->lbl_hash[h];
	ctx->lbl_hash[h]=i;

	ctx->lbl_cho[i]=ctx->lbl_hashb[h2];
	ctx->lbl_hashb[h2]=i;
	
	return(i);
}
#endif

char *BGBCC_JX2_LookupNameForLabel(BGBCC_JX2_Context *ctx, int lblid)
{
	int h;
	int i;

#if 1
	h=((lblid*65521)>>16)&63;
	i=ctx->lbln_ihash[h];
	while(i>=0)
	{
		if(ctx->lbln_id[i]==lblid)
			return(ctx->lbln_name[i]);
		i=ctx->lbln_ichn[i];
	}
	return(NULL);
#endif

#if 0
	for(i=0; i<ctx->nlbln; i++)
	{
		if(ctx->lbln_id[i]==lblid)
			return(ctx->lbln_name[i]);
//		if(!strcmp(ctx->lbln_name[i], name))
//			return(ctx->lbln_id[i]);
	}
	return(NULL);
#endif
}

int BGBCC_JX2_LookupNamedLabel(BGBCC_JX2_Context *ctx, char *name)
{
	char *s;
	int h;
	int i;

	if(!ctx->lbln_name)
		return(0);
	
	s=name; h=0;
	while(*s)h=h*251+(*s++);
//	h=((h*251)>>8)&255;
	h=((h*251)>>8)&63;

	i=ctx->lbln_hash[h];
	while(i>=0)
	{
		if(!strcmp(ctx->lbln_name[i], name))
			return(ctx->lbln_id[i]);
		i=ctx->lbln_chn[i];
	}
	return(0);

#if 0
	for(i=0; i<ctx->nlbln; i++)
	{
		if(!strcmp(ctx->lbln_name[i], name))
			return(ctx->lbln_id[i]);
	}
	return(0);
#endif
}

int BGBCC_JX2_GetNamedLabel(BGBCC_JX2_Context *ctx, char *name)
{
	char *s;
	int h, ih;
	int lbl;
	int i;

	if(!name)
		return(-1);

	if(!bgbcp_strncmp2(name, "__"))
	{
		if(!bgbcp_strncmp7(name, "__arch_"))
		{
			i=BGBCC_JX2A_GetRegId(ctx, name+7);
			if(i>=0)
			{
				lbl=BGBCC_SH_LBL_ARCHREG(i);
				return(lbl);
			}
		}

		if(!bgbcp_strcmp(name, "__tolow"))
		{
			if(!(ctx->is_simpass))
			{
				i=-1;
			}
		}

		if(!bgbcp_strcmp(name, "__tolow\xff"))
		{
				i=-1;
		}
	}

	s=name; h=0;
	while(*s)h=h*251+(*s++);
//	h=((h*251)>>8)&255;
	h=((h*251)>>8)&63;

//	lbl=BGBCC_JX2_LookupNamedLabel(ctx, name);
//	if(lbl>0)
//		return(lbl);
	
	if(!ctx->lbln_name)
	{
//		for(i=0; i<256; i++)
		for(i=0; i<64; i++)
		{
			ctx->lbln_hash[i]=-1;
			ctx->lbln_ihash[i]=-1;
		}
	
//		i=1024;
		i=4096;
//		i=16384;
		ctx->lbln_name=bgbcc_malloc(i*sizeof(char *));
		ctx->lbln_id =bgbcc_malloc(i*sizeof(u32));
		ctx->lbln_chn=bgbcc_malloc(i*sizeof(s16));
		ctx->lbln_ichn=bgbcc_malloc(i*sizeof(s16));
		ctx->nlbln=0;
		ctx->mlbln=i;
	}else
	{
		i=ctx->lbln_hash[h];
		while(i>=0)
		{
			if(!strcmp(ctx->lbln_name[i], name))
				return(ctx->lbln_id[i]);
			i=ctx->lbln_chn[i];
		}
	}
	
	if((ctx->nlbln+1)>=ctx->mlbln)
	{
		i=ctx->mlbln+(ctx->mlbln>>1);
		ctx->lbln_name=bgbcc_realloc(ctx->lbln_name, i*sizeof(char *));
		ctx->lbln_id =bgbcc_realloc(ctx->lbln_id , i*sizeof(u32));
		ctx->lbln_chn=bgbcc_realloc(ctx->lbln_chn, i*sizeof(s16));
		ctx->lbln_ichn=bgbcc_realloc(ctx->lbln_ichn, i*sizeof(s16));
		ctx->mlbln=i;
	}

	if(ctx->is_simpass)
	{
//		return(-1);
	}

	lbl=BGBCC_JX2_GenLabel(ctx);
	ih=((lbl*65521)>>16)&63;

	i=ctx->nlbln++;
	ctx->lbln_id[i]=lbl;
	ctx->lbln_name[i]=bgbcc_strdup(name);
	
	ctx->lbln_chn[i]=ctx->lbln_hash[h];
	ctx->lbln_hash[h]=i;

	ctx->lbln_ichn[i]=ctx->lbln_ihash[ih];
	ctx->lbln_ihash[ih]=i;

	return(lbl);
}

int BGBCC_JX2_EmitNamedLabel(BGBCC_JX2_Context *ctx, char *name)
{
	int lbl;
	
	lbl=BGBCC_JX2_GetNamedLabel(ctx, name);
	if(lbl>0)
	{
		return(BGBCC_JX2_EmitLabel(ctx, lbl));
	}
	return(0);
}

int BGBCC_JX2_EmitNamedLabelOffs(BGBCC_JX2_Context *ctx,
	char *name, int offs)
{
	int lbl;
	
	lbl=BGBCC_JX2_GetNamedLabel(ctx, name);
	if(lbl>0)
	{
		return(BGBCC_JX2_EmitLabelOffs(ctx, lbl, offs));
	}
	return(0);
}

int BGBCC_JX2_EmitNamedGlobal(BGBCC_JX2_Context *ctx, char *name)
{
	return(0);
}

int BGBCC_JX2_EmitCommSym(BGBCC_JX2_Context *ctx, int lblid, int sz, int al)
{
	static int lastemittrip=0;
	int i;

	for(i=0; i<ctx->nlbl; i++)
		if(ctx->lbl_id[i]==lblid)
			return(0);

	if(sz>>16)
		i=-1;

	i=ctx->sec;
	BGBCC_JX2_SetSectionName(ctx, ".bss");
//	BGBCC_JX2_EmitBAlign(ctx, 4);
//	BGBCC_JX2_EmitBAlign(ctx, 8);

#if 0
	if(sz>=8)
	{
		BGBCC_JX2_EmitBAlign(ctx, 8);
	}else if(sz>=4)
	{
		BGBCC_JX2_EmitBAlign(ctx, 4);
	}else if(sz>=2)
	{
		BGBCC_JX2_EmitBAlign(ctx, 2);
	}
#endif

#if 1

//	if(al<=0)
	if((al<=0) || (((4*al)<sz) && (al<8)))
	{
		if(sz>=8)
			{ al=8; }
		else if(sz>=4)
			{ al=4; }
		else if(sz>=2)
			{ al=2; }
		else
			{ al=1; }
	}
	
	if(sz>=512)
	{
		al=16;
	}
	

	if(al>1)
		BGBCC_JX2_EmitBAlign(ctx, al);
#endif

	if((sz>=2048) && !lastemittrip)
	{
		BGBCC_JX2_EmitBAlign(ctx, 16);
		BGBCC_JX2_EmitRelocTy(ctx, lblid, BGBCC_SH_RLC_TRIPWIRE_BJX);
		BGBCC_JX2_EmitRawBytes(ctx, NULL, 16);
	}

	BGBCC_JX2_EmitLabel(ctx, lblid);
	BGBCC_JX2_EmitRawBytes(ctx, NULL, sz);

	lastemittrip=0;
	if(sz>=2048)
	{
		BGBCC_JX2_EmitBAlign(ctx, 16);
		BGBCC_JX2_EmitRelocTy(ctx, lblid, BGBCC_SH_RLC_TRIPWIRE_BJX);
		BGBCC_JX2_EmitRawBytes(ctx, NULL, 16);
		lastemittrip=1;
	}

	BGBCC_JX2DA_EmitComm(ctx, sz);
	ctx->sec=i;
	return(1);
}

int BGBCC_JX2_EmitNamedCommSym(BGBCC_JX2_Context *ctx, char *name,
	int sz, int al)
{
	int lbl;
	
	lbl=BGBCC_JX2_LookupNamedLabel(ctx, name);
	return(BGBCC_JX2_EmitCommSym(ctx, lbl, sz, al));

//	BGBCC_JX2_SetSectionName(sctx, ".bss");
//	BGBCC_JX2_EmitBAlign(sctx, 4);
//	BGBCC_JX2_EmitNamedLabel(sctx, name);
//	BGBCC_JX2_EmitRawBytes(sctx, NULL, sz);
//	return(1);
}

int BGBCC_JX2_LookupLabelAtOffs(BGBCC_JX2_Context *ctx, int sec, int ofs)
{
	int i, j, k, l, h;

	h=(((ofs*251+sec)*251)>>8)&1023;

#if 1
	i=ctx->lbl_hashb[h];
	while(i>=0)
	{
		l=ctx->lbl_id[i];
		if(	(ctx->lbl_sec[i]==sec) &&
			(ctx->lbl_ofs[i]==ofs) &&
			((l&CCXL_LBL_SPMASK)!=CCXL_LBL_GENLLNBASE)	)
				return(l);
		i=ctx->lbl_cho[i];
	}
#endif

	i=ctx->lbl_hashb[h];
	while(i>=0)
	{
		if(	(ctx->lbl_sec[i]==sec) &&
			(ctx->lbl_ofs[i]==ofs)	)
				return(ctx->lbl_id[i]);
		i=ctx->lbl_cho[i];
	}
	return(-1);

#if 0
	for(i=0; i<ctx->nlbl; i++)
	{
		if(	(ctx->lbl_sec[i]==sec) &&
			(ctx->lbl_ofs[i]==ofs)	)
		{
			return(ctx->lbl_id[i]);
		}
	}
	return(-1);
#endif
}

int BGBCC_JX2_LookupLabelAtOffsNoLLn(BGBCC_JX2_Context *ctx, int sec, int ofs)
{
	int i, j, k, l, h;

	h=(((ofs*251+sec)*251)>>8)&1023;

	i=ctx->lbl_hashb[h];
	while(i>=0)
	{
		l=ctx->lbl_id[i];
		if(	(ctx->lbl_sec[i]==sec) &&
			(ctx->lbl_ofs[i]==ofs) &&
			((l&CCXL_LBL_SPMASK)!=CCXL_LBL_GENLLNBASE)	)
				return(l);
		i=ctx->lbl_cho[i];
	}
	return(-1);

#if 0
	int l;
	
	l=BGBCC_JX2_LookupLabelAtOffs(ctx, sec, ofs);
	if(l<0)
		return(l);
	
	if((l&CCXL_LBL_SPMASK)==CCXL_LBL_GENLLNBASE)
		return(-1);
	
	return(l);
#endif
}

int BGBCC_JX2_LookupRelocAtOffs(BGBCC_JX2_Context *ctx, int sec, int ofs)
{
	int i, j, k, h;

	h=(((ofs*251+sec)*251)>>8)&1023;

	i=ctx->rlc_hash[h];
	while(i>=0)
	{
		if(	(ctx->rlc_sec[i]==sec) &&
			(ctx->rlc_ofs[i]==ofs)	)
				return(i);
		i=ctx->rlc_chn[i];
	}
	return(-1);

#if 0
	for(i=0; i<ctx->nrlc; i++)
	{
		if(	(ctx->rlc_sec[i]==sec) &&
			(ctx->rlc_ofs[i]==ofs)	)
				return(i);
	}
	return(-1);
#endif
}

int BGBCC_JX2_EmitRelocTy(BGBCC_JX2_Context *ctx, int lblid, int ty)
{
	int h, sec, ofs;
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
		ctx->rlc_chn=bgbcc_malloc(i*sizeof(s32));
		ctx->nrlc=0;
		ctx->mrlc=i;
		
		for(i=0; i<1024; i++)
			ctx->rlc_hash[i]=-1;
	}
	
	if((ctx->nrlc+1)>=ctx->mrlc)
	{
		i=ctx->mrlc+(ctx->mrlc>>1);
		ctx->rlc_ofs=bgbcc_realloc(ctx->rlc_ofs, i*sizeof(u32));
		ctx->rlc_id =bgbcc_realloc(ctx->rlc_id , i*sizeof(u32));
		ctx->rlc_sec=bgbcc_realloc(ctx->rlc_sec, i*sizeof(byte));
		ctx->rlc_ty =bgbcc_realloc(ctx->rlc_ty , i*sizeof(byte));
		ctx->rlc_chn=bgbcc_realloc(ctx->rlc_chn, i*sizeof(s32));
		ctx->mrlc=i;
	}

	if(ctx->is_simpass)
	{
		return(0);
	}

	sec=ctx->sec;
	ofs=BGBCC_JX2_EmitGetOffs(ctx);
	h=(((ofs*251+sec)*251)>>8)&1023;

	i=ctx->nrlc++;
	ctx->rlc_id[i]=lblid;
	ctx->rlc_ofs[i]=ofs;
	ctx->rlc_sec[i]=sec;
	ctx->rlc_ty[i]=ty;
	
	ctx->rlc_chn[i]=ctx->rlc_hash[h];
	ctx->rlc_hash[h]=i;
	return(1);
}

int BGBCC_JX2_EmitNamedReloc(BGBCC_JX2_Context *ctx, char *name, int ty)
{
	int lbl;
	
	lbl=BGBCC_JX2_GetNamedLabel(ctx, name);
	if(lbl>0)
	{
		return(BGBCC_JX2_EmitRelocTy(ctx, lbl, ty));
	}
	return(0);
}

int BGBCC_JX2_EmitRelocRel8(BGBCC_JX2_Context *ctx, int lbl)
	{ return(BGBCC_JX2_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_REL8)); }
int BGBCC_JX2_EmitRelocRel16(BGBCC_JX2_Context *ctx, int lbl)
	{ return(BGBCC_JX2_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_REL16)); }
int BGBCC_JX2_EmitRelocRel32(BGBCC_JX2_Context *ctx, int lbl)
	{ return(BGBCC_JX2_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_REL32)); }

int BGBCC_JX2_EmitRelocAbs8(BGBCC_JX2_Context *ctx, int lbl)
	{ return(BGBCC_JX2_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_ABS8)); }
int BGBCC_JX2_EmitRelocAbs16(BGBCC_JX2_Context *ctx, int lbl)
	{ return(BGBCC_JX2_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_ABS16)); }
int BGBCC_JX2_EmitRelocAbs32(BGBCC_JX2_Context *ctx, int lbl)
	{ return(BGBCC_JX2_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_ABS32)); }
int BGBCC_JX2_EmitRelocAbs64(BGBCC_JX2_Context *ctx, int lbl)
	{ return(BGBCC_JX2_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_ABS64)); }

int BGBCC_JX2_EmitRelocRelW12(BGBCC_JX2_Context *ctx, int lbl)
	{ return(BGBCC_JX2_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_RELW12)); }
int BGBCC_JX2_EmitRelocRelW8(BGBCC_JX2_Context *ctx, int lbl)
	{ return(BGBCC_JX2_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_RELW8)); }


int BGBCC_JX2_EmitRelocTyOffs(BGBCC_JX2_Context *ctx,
	int lblid, int ty, int offs)
{
	int h, sec, ofs;
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
		ctx->rlc_chn=bgbcc_malloc(i*sizeof(s32));
		ctx->nrlc=0;
		ctx->mrlc=i;
		
		for(i=0; i<1024; i++)
			ctx->rlc_hash[i]=-1;
	}
	
	if((ctx->nrlc+1)>=ctx->mrlc)
	{
		i=ctx->mrlc+(ctx->mrlc>>1);
		ctx->rlc_ofs=bgbcc_realloc(ctx->rlc_ofs, i*sizeof(u32));
		ctx->rlc_id =bgbcc_realloc(ctx->rlc_id , i*sizeof(u32));
		ctx->rlc_sec=bgbcc_realloc(ctx->rlc_sec, i*sizeof(byte));
		ctx->rlc_ty =bgbcc_realloc(ctx->rlc_ty , i*sizeof(byte));
		ctx->rlc_chn=bgbcc_realloc(ctx->rlc_chn, i*sizeof(s32));
		ctx->mrlc=i;
	}

	if(ctx->is_simpass)
	{
		return(0);
	}

	sec=ctx->sec;
//	ofs=BGBCC_JX2_EmitGetOffs(ctx);
	ofs=offs;
	h=(((ofs*251+sec)*251)>>8)&1023;

	i=ctx->nrlc++;
	ctx->rlc_id[i]=lblid;
	ctx->rlc_ofs[i]=ofs;
	ctx->rlc_sec[i]=sec;
	ctx->rlc_ty[i]=ty;
	
	ctx->rlc_chn[i]=ctx->rlc_hash[h];
	ctx->rlc_hash[h]=i;
	return(1);
}

int BGBCC_JX2_LookupLabelIndex(
	BGBCC_JX2_Context *sctx, int lblid)
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

int BGBCC_JX2_LookupSimLabelIndex(
	BGBCC_JX2_Context *sctx, int lblid)
{
	int c, h;
	int i, j, k;

	h=lblid*65521+31;
	h=h*65521+31;
	h=(h>>16)&63;

	c=sctx->lbl_simhash[h];
	while(c>=0)
	{
		if(sctx->lbl_id[c]==lblid)
			return(c);
		c=sctx->lbl_chn[c];
	}

#if 0
	for(i=sctx->nlbl; i<sctx->nvlbl; i++)
	{
		if(sctx->lbl_id[i]==lblid)
			return(i);
	}
#endif

	i=BGBCC_JX2_LookupLabelIndex(sctx, lblid);
	return(i);
}

int BGBCC_JX2_CheckLabelIsGpRel(
	BGBCC_JX2_Context *sctx, int lblid)
{
	char *lbln;
	int i, j, k;

	if(!sctx->is_pbo)
		return(0);

//	i=BGBCC_JX2_LookupLabelIndex(ctx, lblid);
	i=BGBCC_JX2_LookupSimLabelIndex(sctx, lblid);
	
	if(i<0)
	{
		lbln=BGBCC_JX2_LookupNameForLabel(sctx, lblid);
		if(!lbln)
			return(0);
		if((lbln[0]=='_') || (lbln[1]=='_'))
		{
			if(		!strcmp(lbln, "__data_start") ||
					!strcmp(lbln, "__data_end") ||
					!strcmp(lbln, "__bss_start") ||
					!strcmp(lbln, "__bss_end") )
			{
				return(1);
			}
		}
		return(0);
	}
	
	j=sctx->lbl_sec[i];
	return(BGBCC_JX2_IsSectionGpRel(sctx, j));
}

int BGBCC_JX2_CheckGetLabelGpOffs(
	BGBCC_JX2_Context *sctx, int lblid)
{
	char *lbln;
	int dsz;
	int i, j, k;

	if(!sctx->is_pbo)
		return(-1);

//	return(-1);

	i=BGBCC_JX2_LookupLabelIndex(sctx, lblid);
	
	if(i<0)
		return(-1);

	j=sctx->lbl_sec[i];
	k=sctx->lbl_ofs[i];

	if((j==BGBCC_SH_CSEG_DATA) && (k>=0))
		return(k);

#if 0
	if((j==BGBCC_SH_CSEG_BSS) && (k>=0) &&
		(sctx->is_exe || sctx->is_dll))
	{
		if(sctx->is_simpass&64)
			return(-1);
		if(sctx->is_simpass&32)
			return(-1);
	
		/* HACK (Possibly Unsafe):
		   Rely on section layout to infer offset into ".bss"
		   relative to ".data".
		 */
		dsz=	sctx->sec_pos[BGBCC_SH_CSEG_DATA]-
				sctx->sec_buf[BGBCC_SH_CSEG_DATA];
		dsz=(dsz+63)&(~63);
		return(dsz+k);
	}
#endif
	
	return(-1);
}

static int bgbcc_textlbl_id[1<<16];
static int bgbcc_textlbl_chn[1<<16];
static int bgbcc_textlbl_hash[256];
static int bgbcc_n_textlbl=0;

int BGBCC_JX2_MarkLabelIsText(
	BGBCC_JX2_Context *sctx, int lblid)
{
	int i, j, k, h;

	if(bgbcc_n_textlbl<=0)
	{
		bgbcc_n_textlbl=0;
		for(i=0; i<256; i++)
			bgbcc_textlbl_hash[i]=-1;
	}

	h=((lblid*65521)>>16)&255;

	i=bgbcc_textlbl_hash[h];
	while(i>=0)
	{
		if(bgbcc_textlbl_id[i]==lblid)
			return(i);
		i=bgbcc_textlbl_chn[i];
	}

	i=bgbcc_n_textlbl++;
	bgbcc_textlbl_id[i]=lblid;
	bgbcc_textlbl_chn[i]=bgbcc_textlbl_hash[h];
	bgbcc_textlbl_hash[h]=i;
	return(i);
}

int BGBCC_JX2_CheckLabelIsText(
	BGBCC_JX2_Context *sctx, int lblid)
{
	char *lbln;
	int i, j, k, h;

//	if(!sctx->is_pbo)
//		return(0);

	h=((lblid*65521)>>16)&255;
	i=bgbcc_textlbl_hash[h];
	while(i>=0)
	{
		if(bgbcc_textlbl_id[i]==lblid)
			return(1);
		i=bgbcc_textlbl_chn[i];
	}

#if 1
//	i=BGBCC_JX2_LookupLabelIndex(ctx, lblid);
	i=BGBCC_JX2_LookupSimLabelIndex(sctx, lblid);
	
	if(i<0)
	{
		lbln=BGBCC_JX2_LookupNameForLabel(sctx, lblid);
		if(!lbln)
			return(0);
		if((lbln[0]=='_') || (lbln[1]=='_'))
		{
			if(		!strcmp(lbln, "__text_start") ||
					!strcmp(lbln, "__text_end") )
			{
				return(1);
			}
		}
		return(0);
	}
	
	j=sctx->lbl_sec[i];
	return(BGBCC_JX2_IsSectionText(sctx, j));
#endif
}
