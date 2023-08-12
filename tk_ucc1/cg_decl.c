TKUCC_IRDECL *TKUCC_CgenLookupGlobalIndex(TKUCC_MainContext *ctx, int idx)
{
	TKUCC_IRDECL *cur;
	int h;

	h=idx*251+31;
	h=h*251+31;
	h=(h>>8)&255;

//	cur=ctx->ir_gbls;
	cur=ctx->ir_gbl_ihash[h];
	while(cur)
	{
		if(cur->idx==idx)
			return(cur);
//		cur=cur->next;
		cur=cur->ihnext;
	}
	return(NULL);
}

TKUCC_IRDECL *TKUCC_CgenLookupGlobalName(TKUCC_MainContext *ctx, char *name)
{
	TKUCC_IRDECL *cur;
	int h;

	h=TKUCC_PpHashName(name);

//	cur=ctx->ir_gbls;
	cur=ctx->ir_gbl_nhash[h];
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
//		cur=cur->next;
		cur=cur->nhnext;
	}
	return(NULL);
}

TKUCC_IRDECL *TKUCC_CgenGetGlobalName(TKUCC_MainContext *ctx, char *name)
{
	TKUCC_IRDECL *cur;
	int h;

	cur=TKUCC_CgenLookupGlobalName(ctx, name);
	if(cur)
	{
		return(cur);
	}

	cur=TKUCC_ZMallocZero(ctx, sizeof(TKUCC_IRDECL), TKUCC_ZID_TAC);
	cur->name=TKUCC_StrdupInternTac(ctx, name);
	
	cur->next=ctx->ir_gbls;
	ctx->ir_gbls=cur;

	h=TKUCC_PpHashName(name);
	cur->nhnext=ctx->ir_gbl_nhash[h];
	ctx->ir_gbl_nhash[h]=cur;
	
	cur->idx=ctx->ir_gblid_seq++;

	h=cur->idx*251+31;
	h=h*251+31;
	h=(h>>8)&255;
	
	cur->ihnext=ctx->ir_gbl_ihash[h];
	ctx->ir_gbl_ihash[h]=cur;
	
	return(cur);
}



TKUCC_IRDECL *TKUCC_CgenLookupObjectIndex(TKUCC_MainContext *ctx, int idx)
{
	TKUCC_IRDECL *cur;
	int h;

	h=idx*251+31;
	h=h*251+31;
	h=(h>>8)&255;

	cur=ctx->ir_obj_ihash[h];
	while(cur)
	{
		if(cur->idx==idx)
			return(cur);
		cur=cur->ihnext;
	}
	return(NULL);
}

TKUCC_IRDECL *TKUCC_CgenLookupObjectName(TKUCC_MainContext *ctx, char *name)
{
	TKUCC_IRDECL *cur;
	int h;

	h=TKUCC_PpHashName(name);

	cur=ctx->ir_obj_nhash[h];
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->nhnext;
	}
	return(NULL);
}

TKUCC_IRDECL *TKUCC_CgenGetObjectName(TKUCC_MainContext *ctx, char *name)
{
	TKUCC_IRDECL *cur;
	int h;

	cur=TKUCC_CgenLookupObjectName(ctx, name);
	if(cur)
	{
		return(cur);
	}

	cur=TKUCC_ZMallocZero(ctx, sizeof(TKUCC_IRDECL), TKUCC_ZID_TAC);
	cur->name=TKUCC_StrdupInternTac(ctx, name);
	
	cur->next=ctx->ir_objs;
	ctx->ir_objs=cur;

	h=TKUCC_PpHashName(name);
	cur->nhnext=ctx->ir_obj_nhash[h];
	ctx->ir_obj_nhash[h]=cur;
	
	cur->idx=ctx->ir_objid_seq++;

	h=cur->idx*251+31;
	h=h*251+31;
	h=(h>>8)&255;
	
	cur->ihnext=ctx->ir_obj_ihash[h];
	ctx->ir_obj_ihash[h]=cur;
	
	return(cur);
}


TKUCC_IRDECL *TKUCC_CgenLookupLocalIndex(TKUCC_MainContext *ctx, int idx)
{
	TKUCC_IRDECL *cur;
	int h;

	if(!ctx->cur_func)
		return(NULL);

	cur=ctx->cur_func->framevars;
	while(cur)
	{
		if(cur->idx==idx)
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

