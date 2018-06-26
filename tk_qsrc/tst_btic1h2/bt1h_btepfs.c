int btic1h_btefs_getnibble(byte *mdl, int idx)
{
	return(mdl[idx]);

//	return(((*(u64 *)mdl)>>(idx*4))&15);
//	return((mdl[idx>>1]>>((idx&1)*4))&15);
}

void btic1h_btefs_setnibble(byte *mdl, int idx, int val)
{
	mdl[idx]=val;

#if 0
	u64 h;
	int i, j, k;
	
	h=*(u64 *)mdl; j=idx*4;
	h=(h&(~(15<<j)))|(val<<j);
	*(u64 *)mdl=h;
#endif

#if 0
	int i, j, k;
	i=idx>>1; j=(idx&1)*4; k=(val&15)<<j;
	mdl[i]=(mdl[i]&(~(15<<j)))|k;
#endif
}

void BTIC1H_BTEFS_SwapModelIndex(byte *mval, byte *midx, int idx)
{
	int i0, i1;

#if 0
	i0=mval[idx-1];
	i1=mval[idx  ];
	mval[idx-1]=i1;
	mval[idx  ]=i0;
	midx[i0]=idx;
	midx[i1]=idx-1;
#endif
	
#if 1
	i0=btic1h_btefs_getnibble(mval, idx-1);
	i1=btic1h_btefs_getnibble(mval, idx  );
	btic1h_btefs_setnibble(mval, idx-1, i1);
	btic1h_btefs_setnibble(mval, idx  , i0);
	btic1h_btefs_setnibble(midx, i0, idx);
	btic1h_btefs_setnibble(midx, i1, idx-1);
#endif
}

int BTIC1H_BTEFS_GetUpdateModelIndex(BTIC1H_Context *ctx, int sym)
{
	byte *mval, *midx;
	int i, j, k;
	
	k=(ctx->wctx&255)<<4;
	mval=ctx->bfs_mdlval+k;
	midx=ctx->bfs_mdlidx+k;
	
	i=btic1h_btefs_getnibble(midx, sym);
	if(i>0)
	{
		BTIC1H_BTEFS_SwapModelIndex(mval, midx, sym);
	}
	ctx->wctx=(ctx->wctx<<4)|sym;
	return(i);
}

void BTIC1H_BTEFS_WriteNBits(BTIC1H_Context *ctx, int v, int n)
{
	int i, j, k;

	ctx->bfs_bits+=n;

	v&=(1<<n)-1;

	j=ctx->bfs_pos+n;
	k=ctx->bfs_win|(v<<(32-j));
	if(j>=8)
	{
		*ctx->bs_ct++=(k>>24); k<<=8; j-=8;
		if(j>=8)
			{ *ctx->bs_ct++=(k>>24); k<<=8; j-=8; }
	}
	ctx->bfs_pos=j;
	ctx->bfs_win=k;
}

/*

k0: 0=0(k0), 1=10(k0), 2=110(k1), 3=1110(k1),
4.. 7=11110xx  (k2)
8..15=11111xxx (k3)

k1: 0/1=0x(k0), 2/3=10x(k1), 4/5=110x(k2), 6/7=1110x(k2),
8/9=11110x(k3), 10..15=11111xxx(k3)

k2: 0..3=0xx(k1), 4..7=10xx(k2), 8..15=11xxx(k3)
k3: 0..7: 0xxx(k2), 8..15: 1xxx(k3)
*/

short btic1h_btefs_sibits[4][16]={
{0x0100, 0x0202, 0x1306, 0x140E, 0x2778, 0x2779, 0x277A, 0x277B,
 0x38F8, 0x38F9, 0x38FA, 0x38FB, 0x38FC, 0x38FD, 0x38FE, 0x38FF},
{0x0200, 0x0201, 0x1304, 0x1305, 0x240C, 0x240D, 0x251C, 0x251D,
 0x363C, 0x363D, 0x38F8, 0x38F9, 0x38FA, 0x38FB, 0x38FC, 0x38FD},
{0x1300, 0x1301, 0x1302, 0x1303, 0x2408, 0x2409, 0x240A, 0x240B,
 0x3518, 0x3519, 0x351A, 0x351B, 0x351C, 0x351D, 0x351E, 0x351F},
{0x2400, 0x2401, 0x2402, 0x2403, 0x2404, 0x2405, 0x2406, 0x2407,
 0x3408, 0x3409, 0x340A, 0x340B, 0x340C, 0x340D, 0x340E, 0x340F}
};

void BTIC1H_BTEFS_WriteSymIdxBits(BTIC1H_Context *ctx, int v, byte *rk)
{
	int i;
	i=btic1h_btefs_sibits[*rk][v];
	BTIC1H_BTEFS_WriteNBits(ctx, i, (i>>8)&15);
	*rk=i>>12;
}

void BTIC1H_BTEFS_EmitSym4(BTIC1H_Context *ctx, int sym)
{
	int i, j;
	
	j=ctx->wctx&255;
	i=BTIC1H_BTEFS_GetUpdateModelIndex(ctx, sym);
	BTIC1H_BTEFS_WriteSymIdxBits(ctx, i, ctx->bfs_mdlrk+j);
}

void BTIC1H_BTEFS_EmitSym8(BTIC1H_Context *ctx, int sym)
{
	BTIC1H_BTEFS_EmitSym4(ctx, (sym>>4)&15);
	BTIC1H_BTEFS_EmitSym4(ctx, (sym   )&15);
}

void BTIC1H_BTEFS_SetupModel(BTIC1H_Context *ctx)
{
	int i, j, k;

	if(!ctx->bfs_mdl)
	{
		ctx->bfs_mdl=malloc(256*32+256);
		ctx->bfs_mdlval=ctx->bfs_mdl;
		ctx->bfs_mdlidx=ctx->bfs_mdl+256*16;
		ctx->bfs_mdlrk=ctx->bfs_mdl+256*32;
	}
	
	for(i=0; i<256; i++)
	{
		for(j=0; j<16; j++)
		{
//			ctx->bfs_mdlval[(i<<4)|j]=j;
//			ctx->bfs_mdlidx[(i<<4)|j]=j;
			btic1h_btefs_setnibble(ctx->bfs_mdlval+(i<<4), j, j);
			btic1h_btefs_setnibble(ctx->bfs_mdlidx+(i<<4), j, j);
		}
//		*(u64 *)(ctx->bfs_mdlval+(i<<4))=0xFEDCBA9876543210ULL;
//		*(u64 *)(ctx->bfs_mdlidx+(i<<4))=0xFEDCBA9876543210ULL;
		ctx->bfs_mdlrk[i]=3;
	}
}

void BTIC1H_BTEFS_DumpStats(BTIC1H_Context *ctx)
{
	int i, j, k;

	if(!ctx->bfs_mdl)
		return;

	for(i=0; i<16; i++)
	{
		for(j=0; j<16; j++)
		{
			printf("%d ", ctx->bfs_mdlrk[i*16+j]);
		}
		printf("\n");
	}
}

void BTIC1H_Rice_SetupWriteBTEFS(BTIC1H_Context *ctx, byte *obuf, int osz)
{
//	BTIC1H_Range_SetupEncode(ctx, obuf, osz);

	BTIC1H_BTEFS_SetupModel(ctx);

	ctx->bs_ct=obuf;
	ctx->bs_cte=obuf+osz;

	ctx->bfs_win=0;
	ctx->bfs_pos=0;

	ctx->bs_bits=0;
	ctx->bfs_bits=0;

	ctx->Write8Bits=BTIC1H_BTEFS_EmitSym8;

//	ctx->WriteNBits=BTIC1H_Rice_WriteNBitsRange;
//	ctx->Write8Bits=BTIC1H_BTEFS_EmitSym8;
//	ctx->Write16Bits=BTIC1H_Rice_Write16BitsRange;
//	ctx->WriteAdRice=BTIC1H_Rice_WriteAdRiceRange;
//	ctx->WriteAdSRice=BTIC1H_Rice_WriteAdSRiceRange;
//	ctx->WriteAdRiceDc=BTIC1H_Rice_WriteAdRiceDcRange;
//	ctx->WriteAdSRiceDc=BTIC1H_Rice_WriteAdSRiceDcRange;
//	ctx->FlushBits=BTIC1H_Rice_FlushBitsBasic;
}
