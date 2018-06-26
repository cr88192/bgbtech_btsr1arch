u32 mmgp_data[256];

int BJX2_MemMmgpCb_GetByte(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

int BJX2_MemMmgpCb_GetWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

s32 BJX2_MemMmgpCb_GetDWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	u32 *mmio;
	s64 rvq;
	int ra, rv;

	ra=addr-sp->addr_base;

	mmio=mmgp_data;

	rv=0;
	switch(ra)
	{
	case 0x0000:
		rvq=(ctx->tot_cyc*ctx->rcp_mhz)>>16;
		rv=rvq;
//		rv=ctx->tot_cyc/ctx->tgt_mhz;
		break;
	case 0x0004:
		rvq=(ctx->tot_cyc*ctx->rcp_mhz)>>16;
		rv=rvq>>32;
//		rv=(ctx->tot_cyc/ctx->tgt_mhz)>>32;
		break;

	case 0x0008:
		rv=ctx->ttick_hk;
		break;
	case 0x000C:
		rv=ctx->ttick_rst;
		break;

	case 0x0010:
		rv=0;
		if(ctx->kbirov!=ctx->kbrov)
		{
			rv=ctx->kbbuf[ctx->kbirov];
			ctx->kbirov=(ctx->kbirov+1)&255;
		}
		break;
	case 0x0014:
		rv=0; break;
	case 0x0018:
		rv=0;
		if(ctx->kbirov!=ctx->kbrov)
			rv|=1;
		break;
	case 0x001C:
		rv=0; break;

	case 0x30:
		rv=mmio[0x10];
//		printf("SPI_C(R): D=%08X\n", rv);
		break;
	case 0x34:
		rv=mmio[0x11];
//		printf("SPI_D(R): D=%08X\n", rv);
		break;
	}
	return(rv);
}


int BJX2_MemMmgpCb_SetByte(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int val)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

int BJX2_MemMmgpCb_SetWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int val)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

int BJX2_MemMmgpCb_SetDWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s32 val)
{
	u32 *mmio;
	int v;
	int ra;

	ra=addr-sp->addr_base;

	mmio=mmgp_data;

	switch(ra)
	{
	case 0x0000:
		break;
	case 0x0004:
		break;
	case 0x0008:
		ctx->ttick_hk=val;
		break;
	case 0x000C:
		ctx->ttick_rst=val;
		break;
	case 0x0010:
		break;
	case 0x0014:
		printf("%c", (char)val);
		break;
	case 0x0018:
		break;
	case 0x001C:
		break;

	case 0x0030:
		v=btesh2_spimmc_XrCtl(ctx, val);
		mmio[0x10]=v&255;
		if(v&0x10000)
			mmio[0x11]=(v>>8)&255;

//		printf("SPI_C(W): %08X -> %08X, D=%08X\n", val, v, mmio[0x11]);
		break;
	case 0x0034:
		v=btesh2_spimmc_XrByte(ctx, val);
		mmio[0x11]=v;

//		printf("SPI_D(W): %08X -> %08X, D=%08X\n", val, v, mmio[0x11]);
		break;
	}

	return(0);
}

int BJX2_MemDefineMmgp(BJX2_Context *ctx,
	char *name, bjx2_addr base, bjx2_addr lim)
{
	BJX2_MemSpan *sp;
	
	sp=BJX2_MemAllocSpan(ctx);
	
	sp->name=name;
	sp->addr_base=base;
	sp->addr_lim=lim;
//	sp->data=malloc((lim-base)+8);
	
	sp->GetByte=BJX2_MemMmgpCb_GetByte;
	sp->GetWord=BJX2_MemMmgpCb_GetWord;
	sp->GetDWord=BJX2_MemMmgpCb_GetDWord;
	
	sp->SetByte=BJX2_MemMmgpCb_SetByte;
	sp->SetWord=BJX2_MemMmgpCb_SetWord;
	sp->SetDWord=BJX2_MemMmgpCb_SetDWord;
	
	BJX2_MemAddSpan(ctx, sp);
	return(0);
}

int BJX2_ContextLoadMap(BJX2_Context *ctx, char *name)
{
	bjx2_addr tmap_addr[4096];
	char *tmap_name[4096];
	char tb[256];
	FILE *fd;
	char **a;
	int tmn, ta;
	int sz, sz1;
	int i, j, k;
	
	fd=fopen(name, "rb");
	if(!fd)
	{
		printf("Failed open %s\n", name);
		return(-1);
	}

	tmn=0;
	while(!feof(fd))
	{
		fgets(tb, 255, fd);
		a=JX2R_SplitLine(tb);
		
		sscanf(a[0], "%08X", &ta);
		
		tmap_addr[tmn]=ta;
		tmap_name[tmn]=strdup(a[2]);
		tmn++;
	}
	
	ctx->map_addr=malloc(tmn*sizeof(bjx2_addr));
	ctx->map_name=malloc(tmn*sizeof(char *));
	ctx->map_n_ents=tmn;
	
	for(i=0; i<tmn; i++)
	{
		ctx->map_addr[i]=tmap_addr[i];
		ctx->map_name[i]=tmap_name[i];
	}
	return(0);
}

int BJX2_ContextLoadRom(BJX2_Context *ctx, char *name)
{
	char tb[256];
	BJX2_MemSpan *sp;
	byte *buf;
	FILE *fd;
	int sz, sz1;
	
	fd=fopen(name, "rb");
	if(!fd)
	{
		printf("Failed open %s\n", name);
		return(-1);
	}

	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	buf=malloc(sz);
	fread(buf, 1, sz, fd);
	fclose(fd);
	
	sp=BJX2_MemSpanForName(ctx, "ROM");
	if(!sp)
	{
		printf("Failed find ROM span\n");
		return(-1);
	}
	
	sz1=(sp->addr_lim-sp->addr_base)+1;
	if(sz>sz1)
	{
		printf("Oversized ROM, %d>%d\n", sz, sz1);
		sz=sz1;
	}
	memcpy(sp->data, buf, sz);
	free(buf);
	
	sprintf(tb, "%s.map", name);
	BJX2_ContextLoadMap(ctx, tb);
	
	return(0);
}
