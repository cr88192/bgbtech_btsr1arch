int BTSR1_MemMmgpCb_GetByte(BTSR1_Context *ctx,
	BTSR1_MemSpan *sp, btsr1_addr addr)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

int BTSR1_MemMmgpCb_GetWord(BTSR1_Context *ctx,
	BTSR1_MemSpan *sp, btsr1_addr addr)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

s32 BTSR1_MemMmgpCb_GetDWord(BTSR1_Context *ctx,
	BTSR1_MemSpan *sp, btsr1_addr addr)
{
	s64 rvq;
	int ra, rv;

	ra=addr-sp->addr_base;
	
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
	}
	return(rv);
}


int BTSR1_MemMmgpCb_SetByte(BTSR1_Context *ctx,
	BTSR1_MemSpan *sp, btsr1_addr addr, int val)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

int BTSR1_MemMmgpCb_SetWord(BTSR1_Context *ctx,
	BTSR1_MemSpan *sp, btsr1_addr addr, int val)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

int BTSR1_MemMmgpCb_SetDWord(BTSR1_Context *ctx,
	BTSR1_MemSpan *sp, btsr1_addr addr, s32 val)
{
	int ra;
	ra=addr-sp->addr_base;

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
	}

	return(0);
}

int BTSR1_MemDefineMmgp(BTSR1_Context *ctx,
	char *name, btsr1_addr base, btsr1_addr lim)
{
	BTSR1_MemSpan *sp;
	
	sp=BTSR1_MemAllocSpan(ctx);
	
	sp->name=name;
	sp->addr_base=base;
	sp->addr_lim=lim;
//	sp->data=malloc((lim-base)+8);
	
	sp->GetByte=BTSR1_MemMmgpCb_GetByte;
	sp->GetWord=BTSR1_MemMmgpCb_GetWord;
	sp->GetDWord=BTSR1_MemMmgpCb_GetDWord;
	
	sp->SetByte=BTSR1_MemMmgpCb_SetByte;
	sp->SetWord=BTSR1_MemMmgpCb_SetWord;
	sp->SetDWord=BTSR1_MemMmgpCb_SetDWord;
	
	BTSR1_MemAddSpan(ctx, sp);
	return(0);
}


int BTSR1_ContextLoadRom(BTSR1_Context *ctx, char *name)
{
	BTSR1_MemSpan *sp;
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
	
	sp=BTSR1_MemSpanForName(ctx, "ROM");
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
	
	return(0);
}
