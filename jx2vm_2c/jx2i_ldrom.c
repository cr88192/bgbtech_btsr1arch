u32 mmgp_data[256];

s64 mmgp_spi_lastcyc;
int mmgp_spi_delcyc;

char *bjx2_fgets(char *buf, int lim, BJX2_FILE *fd);

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

s64 BJX2_Interp_GetVirtualUsec(BJX2_Context *ctx)
{
	s64 rvq;

	rvq=(ctx->tot_cyc*ctx->rcp_mhz)>>16;
//	rv=rvq;

	return(rvq);
}

s32 BJX2_MemMmgpCb_GetDWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	u32 *mmio;
	s64 rvq, dcyc;
	int ra, rv;

	dcyc=ctx->tot_cyc-mmgp_spi_lastcyc;
	mmgp_spi_lastcyc=ctx->tot_cyc;
	
	if(dcyc>mmgp_spi_delcyc)
		mmgp_spi_delcyc=0;
	else
		mmgp_spi_delcyc-=dcyc;
	ctx->iodel_cyc=mmgp_spi_delcyc;


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

	case 0x0030:
		rv=mmio[0x10];
		if(mmgp_spi_delcyc>0)
			rv|=2;
//		printf("SPI_C(R): D=%08X\n", rv);
		break;
	case 0x0034:
		rv=mmio[0x11];
//		printf("SPI_D(R): D=%08X\n", rv);
		break;


	case 0x0040:
		rv=0;
		if(ctx->ps2kbirov!=ctx->ps2kbrov)
		{
			rv=ctx->ps2kbbuf[ctx->ps2kbirov];
			ctx->ps2kbirov=(ctx->ps2kbirov+1)&255;
		}
		break;
	case 0x0044:
		rv=0; break;
	case 0x0048:
		rv=0;
		if(ctx->ps2kbirov!=ctx->ps2kbrov)
			rv|=1;
		break;
	case 0x004C:
		rv=0; break;

	case 0x0050:
		rv=0;
		if(ctx->ps2msirov!=ctx->ps2msrov)
		{
			rv=ctx->ps2msbuf[ctx->ps2msirov];
			ctx->ps2msirov=(ctx->ps2msirov+1)&255;
		}
		break;
	case 0x0054:
		rv=0; break;
	case 0x0058:
		rv=0;
		if(ctx->ps2msirov!=ctx->ps2msrov)
			rv|=1;
		break;
	case 0x005C:
		rv=0; break;


	case 0x0300:
		rv=ctx->msgbuf_rxspos;
		break;
	case 0x0304:
		rv=ctx->msgbuf_rxepos;
		break;
	case 0x0308:
		rv=ctx->msgbuf_txspos;
		break;
	case 0x030C:
		rv=ctx->msgbuf_txepos;
		break;
	case 0x0310:
		rv=ctx->rtc_ms;
		break;
	case 0x0314:
		rv=ctx->msgbuf_msk;
		break;

	case 0x0340:
		rvq=ctx->tot_cyc;
		rv=rvq;
//		rv=ctx->tot_cyc/ctx->tgt_mhz;
		break;
	case 0x0344:
		rvq=ctx->tot_cyc;
		rv=rvq>>32;
//		rv=(ctx->tot_cyc/ctx->tgt_mhz)>>32;
		break;
	case 0x0348:
		rvq=ctx->tgt_mhz;
		rv=rvq;
		break;
	case 0x034C:
		rvq=ctx->tgt_mhz;
		rv=rvq>>32;
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
	s64 dcyc;
	int v;
	int ra;

	dcyc=ctx->tot_cyc-mmgp_spi_lastcyc;
	mmgp_spi_lastcyc=ctx->tot_cyc;
	
	if(dcyc>mmgp_spi_delcyc)
		mmgp_spi_delcyc=0;
	else
		mmgp_spi_delcyc-=dcyc;
	ctx->iodel_cyc=mmgp_spi_delcyc;


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
//		printf("%c", (char)val);
		bjx2_vmputc(ctx, (byte)val);
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
//		ctx->iodel_cyc=800;
//		mmgp_spi_delcyc+=800;
//		ctx->iodel_cyc=200;
//		mmgp_spi_delcyc+=200;
//		ctx->iodel_cyc=20;
//		mmgp_spi_delcyc+=20;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_IOPOKE);

//		printf("SPI_D(W): %08X -> %08X, D=%08X\n", val, v, mmio[0x11]);
		break;

	case 0x0300:
		ctx->msgbuf_rxspos=val;
		BJX2_VmMsgRxUpdate(ctx);
		break;
	case 0x0304:
		ctx->msgbuf_rxepos=val;
		BJX2_VmMsgRxUpdate(ctx);
		break;
	case 0x0308:
		ctx->msgbuf_txspos=val;
		BJX2_VmMsgTxUpdate(ctx);
		break;
	case 0x030C:
		ctx->msgbuf_txepos=val;
		BJX2_VmMsgTxUpdate(ctx);
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
	sp->addr_sz=lim-base;
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
	bjx2_addr tmap_addr[16384];
	char *tmap_name[16384];
	char tb[256];
	BJX2_FILE *fd;
	char **a;
	char *s;
	int tmn, ta;
	int sz, sz1;
	int i, j, k;
	
	fd=bjx2_fopen(name, "rb");
	if(!fd)
	{
		printf("Failed open %s\n", name);
		return(-1);
	}

	tmn=0;
	
	if(ctx->map_n_ents)
	{
		for(i=0; i<ctx->map_n_ents; i++)
		{
			tmap_addr[i]=ctx->map_addr[i];
			tmap_name[i]=ctx->map_name[i];
		}
		tmn=ctx->map_n_ents;
	}
	
	while(!bjx2_feof(fd))
	{
		s=bjx2_fgets(tb, 255, fd);
		a=JX2R_SplitLine(tb);
		if(!a[0])
			continue;
		
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
	BJX2_FILE *fd;
	int sz, sz1;
	int i, j, k;
	
	fd=bjx2_fopen(name, "rb");
	if(!fd)
	{
		printf("Failed open %s\n", name);
		return(-1);
	}

	bjx2_fseek(fd, 0, 2);
	sz=bjx2_ftell(fd);
	bjx2_fseek(fd, 0, 0);
	buf=malloc(sz);
	k=bjx2_fread(buf, 1, sz, fd);
	bjx2_fclose(fd);
	
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
