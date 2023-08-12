int TKUCC_CgenEmitBinop(TKUCC_MainContext *ctx,
	TKUCC_IRDECL *op)
{
	if(TKUCC_TypeSmallIntP(ctx, op->dty))
	{
		return(TKUCC_CgenEmitBinopInt(ctx, op));
	}
}
