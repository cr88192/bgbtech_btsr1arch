ccxl_status BGBCC_BSRC_FlattenImageROM(BGBCC_TransState *ctx,
	byte *obuf, int *rosz, fourcc imgfmt)
{
	BGBCC_BSR_Context *sctx;
	FILE *mapfd;
	char *s0;
	byte *ct;
	int en, ofs, ofs_sdat, ofs_iend, ofs_mend;
	int ofs_rd_strt;
	int ofs_rd_end;
	int of_phdr, ne_phdr;
	int of_shdr, ne_shdr;
	int lb_strt, va_strt;
	int nm, fl, lva, rva, lsz, sn_strs, imty;
	int va_rom, va_ram, va_base;
	int i, j, k;

	sctx=ctx->uctx;

//	BGBCC_BSR_EmitGetStrtabLabel(sctx, "");

	for(i=0; i<sctx->nsec; i++)
	{
		s0=sctx->sec_name[i];
		if(s0)
		{
//			BGBCC_BSR_EmitGetStrtabLabel(sctx, s0);
		}else
		{
			switch(i)
			{
			case BGBCC_SH_CSEG_TEXT: s0=".text"; break;
			case BGBCC_SH_CSEG_STRTAB: s0=".strtab"; break;
			case BGBCC_SH_CSEG_GOT: s0=".got"; break;
			case BGBCC_SH_CSEG_DATA: s0=".data"; break;
			case BGBCC_SH_CSEG_RODATA: s0=".rodata"; break;
			case BGBCC_SH_CSEG_BSS: s0=".bss"; break;
			default: s0=".unknown"; break;
			}

//			BGBCC_BSR_EmitGetStrtabLabel(sctx, s0);
			sctx->sec_name[i]=s0;
		}
	}

	en=(sctx->is_le==0);
	ne_phdr=1;
	ne_shdr=sctx->nsec+1;
	
//	of_phdr=64;
	
//	of_shdr=of_phdr+(ne_phdr*32);
//	of_shdr=(of_shdr+15)&(~15);

//	ofs_sdat=of_shdr+(ne_shdr*40);
//	ofs_sdat=(ofs_sdat+63)&(~63);

//	ofs_sdat=64;
	ofs_sdat=16;

	va_base=0;
	switch(sctx->use_memmdl)
	{
	case BGBCC_MEMMDL_TINY16:
		ofs_sdat=8;
		va_rom=ofs_sdat;
		va_ram=0xC000;
		break;
	case BGBCC_MEMMDL_DEFAULT:
		ofs_sdat=16;
		va_rom=ofs_sdat;
		va_ram=0x18000000;
		break;
	}
	
	for(i=0; i<sctx->nsec; i++)
	{
		if(i==BGBCC_SH_CSEG_BSS)
			continue;
		j=sctx->sec_pos[i]-sctx->sec_buf[i];
		if(!BGBCC_BSR_IsSectionReadOnly(sctx, i))
			continue;

		sctx->sec_rva[i]=va_rom;
		sctx->sec_lva[i]=va_base+va_rom;
		sctx->sec_lsz[i]=j;

		memcpy(obuf+va_rom, sctx->sec_buf[i], j);
		va_rom+=j;
//		va_rom=(va_rom+63)&(~63);
		va_rom=(va_rom+15)&(~15);

		s0=sctx->sec_name[i];
		printf("%d: %-16s %08X..%08X %08X..%08X %d\n", i, s0,
			sctx->sec_rva[i], sctx->sec_rva[i]+sctx->sec_lsz[i],
			sctx->sec_lva[i], sctx->sec_lva[i]+sctx->sec_lsz[i],
			sctx->sec_lsz[i]);

	}

	ofs_rd_strt=va_rom;
	for(i=0; i<sctx->nsec; i++)
	{
		if(i==BGBCC_SH_CSEG_BSS)
			continue;
		j=sctx->sec_pos[i]-sctx->sec_buf[i];
		if(BGBCC_BSR_IsSectionReadOnly(sctx, i))
			continue;

//		va_ram-=j;
//		va_ram&=~63;
//		va_ram&=~15;
	
		sctx->sec_rva[i]=va_rom;
		sctx->sec_lva[i]=va_base+va_ram;
		sctx->sec_lsz[i]=j;

		memcpy(obuf+va_rom, sctx->sec_buf[i], j);
		va_rom+=j;
		va_ram+=j;
//		va_rom=(va_rom+63)&(~63);
		va_rom=(va_rom+15)&(~15);
		va_ram=(va_ram+15)&(~15);

		s0=sctx->sec_name[i];
		printf("%d: %-16s %08X..%08X %08X..%08X %d\n", i, s0,
			sctx->sec_rva[i], sctx->sec_rva[i]+sctx->sec_lsz[i],
			sctx->sec_lva[i], sctx->sec_lva[i]+sctx->sec_lsz[i],
			sctx->sec_lsz[i]);

	}
	ofs_rd_end=va_rom;

	ofs_iend=va_rom;

	i=BGBCC_SH_CSEG_BSS;
	j=sctx->sec_pos[i]-sctx->sec_buf[i];
//	va_ram-=j;
//	va_ram+=j;
//	va_ram&=~63;
//	va_ram&=~15;
	
	sctx->sec_rva[i]=va_rom;
	sctx->sec_lva[i]=va_base+va_ram;
	sctx->sec_lsz[i]=j;
	va_rom+=j;
	va_ram+=j;
//	va_rom=(va_rom+63)&(~63);
	va_rom=(va_rom+15)&(~15);
	va_ram=(va_ram+15)&(~15);

	s0=sctx->sec_name[i];
	printf("%d: %-16s %08X..%08X %08X..%08X %d\n", i, s0,
		sctx->sec_rva[i], sctx->sec_rva[i]+sctx->sec_lsz[i],
		sctx->sec_lva[i], sctx->sec_lva[i]+sctx->sec_lsz[i],
		sctx->sec_lsz[i]);

	ofs_mend=va_rom;
	
	BGBCC_BSR_EmitLabelAbs(sctx, sctx->lbl_rom_data_strt, ofs_rd_strt);
	BGBCC_BSR_EmitLabelAbs(sctx, sctx->lbl_rom_data_end, ofs_rd_end);


	lb_strt=0;
//	lb_strt=BGBCC_BSR_LookupNamedLabel(sctx, "__start");
//	if(lb_strt<=0)
//		lb_strt=BGBCC_BSR_LookupNamedLabel(sctx, "___start");
	if(lb_strt<=0)
		lb_strt=BGBCC_BSR_LookupNamedLabel(sctx, "_start");
	if(lb_strt<=0)
		lb_strt=BGBCC_BSR_LookupNamedLabel(sctx, "__start");
	
	if(lb_strt>0)
		{ va_strt=BGBCC_BSRC_LookupLabelImgVA(ctx, sctx, lb_strt); }
	else
	{
		printf("BGBCC_BSRC_FlattenImageROM: No Entry Point Found\n");
		va_strt=0;
	}

	ct=obuf;

//	if(ofs_sdat<16)
	if(1)
	{
//		bgbcc_setu16en(ct+0, en, 0xE000|(va_strt>>16)&255);
//		bgbcc_setu16en(ct+2, en, 0xD000|(va_strt>> 8)&255);
//		bgbcc_setu16en(ct+4, en, 0xD000|(va_strt>> 0)&255);
//		bgbcc_setu16en(ct+6, en, 0x3200);

		i=va_strt-8;
		bgbcc_setu16en(ct+0, en, 0xA000|(i>>20)&4095);
		bgbcc_setu16en(ct+2, en, 0x2600|(i>>12)& 255);
		bgbcc_setu16en(ct+4, en, 0x2600|(i>> 4)& 255);
		bgbcc_setu16en(ct+6, en, 0x40F0|(i>> 0)&  15);
	}else
	{
//		bgbcc_setu16en(ct+0, en, 0xE000|(va_strt>>24)&255);
//		bgbcc_setu16en(ct+2, en, 0xD000|(va_strt>>16)&255);
//		bgbcc_setu16en(ct+4, en, 0xD000|(va_strt>> 8)&255);
//		bgbcc_setu16en(ct+6, en, 0xD000|(va_strt>> 0)&255);
//		bgbcc_setu16en(ct+8, en, 0x3200);
	}

//	bgbcc_setu16en(ct+0, en, 0xD001);	//load entry point
//	bgbcc_setu16en(ct+2, en, 0x402B);	//jump to entry point
//	bgbcc_setu16en(ct+4, en, 0x0009);	//
//	bgbcc_setu16en(ct+6, en, 0x0009);	//
//	bgbcc_setu32en(ct+8, en, va_strt);	//entry point

	BGBCC_BSRC_ApplyImageRelocs(ctx, sctx, obuf);

#if 1
	mapfd=fopen("aout.map", "wt");
	for(i=0; i<sctx->nlbln; i++)
	{
//		if(!strcmp(ctx->lbln_name[i], name))
//			return(ctx->lbln_id[i]);

		lva=BGBCC_BSRC_LookupLabelImgVA(ctx, sctx, sctx->lbln_id[i]);
		fprintf(mapfd, "%08X T %s\n", lva, sctx->lbln_name[i]);
	}
	fclose(mapfd);
#endif

	*rosz=ofs_iend;

	return(0);
}

