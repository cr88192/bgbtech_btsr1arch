ccxl_status BGBCC_SHXC_FlattenImageROM(BGBCC_TransState *ctx,
	byte *obuf, int *rosz, fourcc imgfmt)
{
	BGBCC_SHX_Context *sctx;
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
	int va_rom, va_ram;
	int i, j, k;

	sctx=ctx->uctx;

	BGBCC_SHX_EmitGetStrtabLabel(sctx, "");

	for(i=0; i<sctx->nsec; i++)
	{
		s0=sctx->sec_name[i];
		if(s0)
		{
			BGBCC_SHX_EmitGetStrtabLabel(sctx, s0);
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

			BGBCC_SHX_EmitGetStrtabLabel(sctx, s0);
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

	va_rom=ofs_sdat;
	va_ram=0x18000000;
	
	for(i=0; i<sctx->nsec; i++)
	{
		if(i==BGBCC_SH_CSEG_BSS)
			continue;
		j=sctx->sec_pos[i]-sctx->sec_buf[i];
		if(!BGBCC_SHX_IsSectionReadOnly(sctx, i))
			continue;

		sctx->sec_rva[i]=va_rom;
		sctx->sec_lva[i]=0xA0000000+va_rom;
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
		if(BGBCC_SHX_IsSectionReadOnly(sctx, i))
			continue;

		va_ram-=j;
//		va_ram&=~63;
		va_ram&=~15;
	
		sctx->sec_rva[i]=va_rom;
		sctx->sec_lva[i]=0xA0000000+va_ram;
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
	ofs_rd_end=va_rom;

	ofs_iend=va_rom;

	i=BGBCC_SH_CSEG_BSS;
	j=sctx->sec_pos[i]-sctx->sec_buf[i];
	va_ram-=j;
//	va_ram&=~63;
	va_ram&=~15;
	
	sctx->sec_rva[i]=va_rom;
	sctx->sec_lva[i]=0xA0000000+va_ram;
	sctx->sec_lsz[i]=j;
	va_rom+=j;
//	va_rom=(va_rom+63)&(~63);
	va_rom=(va_rom+15)&(~15);

	s0=sctx->sec_name[i];
	printf("%d: %-16s %08X..%08X %08X..%08X %d\n", i, s0,
		sctx->sec_rva[i], sctx->sec_rva[i]+sctx->sec_lsz[i],
		sctx->sec_lva[i], sctx->sec_lva[i]+sctx->sec_lsz[i],
		sctx->sec_lsz[i]);

	ofs_mend=va_rom;
	
	BGBCC_SHX_EmitLabelAbs(sctx, sctx->lbl_rom_data_strt, ofs_rd_strt);
	BGBCC_SHX_EmitLabelAbs(sctx, sctx->lbl_rom_data_end, ofs_rd_end);


	lb_strt=0;
//	lb_strt=BGBCC_SHX_LookupNamedLabel(sctx, "__start");
//	if(lb_strt<=0)
//		lb_strt=BGBCC_SHX_LookupNamedLabel(sctx, "___start");
	if(lb_strt<=0)
		lb_strt=BGBCC_SHX_LookupNamedLabel(sctx, "_start");
	if(lb_strt<=0)
		lb_strt=BGBCC_SHX_LookupNamedLabel(sctx, "__start");
	
	if(lb_strt>0)
		{ va_strt=BGBCC_SHXC_LookupLabelImgVA(ctx, sctx, lb_strt); }
	else
	{
		printf("BGBCC_SHXC_FlattenImageROM: No Entry Point Found\n");
		va_strt=0;
	}

	ct=obuf;

	bgbcc_setu16en(ct+0, en, 0xD001);	//load entry point
	bgbcc_setu16en(ct+2, en, 0x402B);	//jump to entry point
	bgbcc_setu16en(ct+4, en, 0x0009);	//
	bgbcc_setu16en(ct+6, en, 0x0009);	//
	bgbcc_setu32en(ct+8, en, va_strt);	//entry point

#if 0
	ct[ 0]=0x7F;	ct[ 1]='E';			//magic
	ct[ 2]='L';		ct[ 3]='F';			//magic
	ct[ 4]=1;		ct[ 5]=en?2:1;		//width, endian
	ct[ 6]=1;		ct[ 7]=0;			//ELF version, OS ABI
	ct[ 8]=0;		ct[ 9]=0;			//pad
	ct[10]=0;		ct[11]=0;			//pad
	ct[12]=0;		ct[13]=0;			//pad
	ct[14]=0;		ct[15]=0;			//pad

	sn_strs=BGBCC_SH_CSEG_STRTAB+1;

	imty=2;
//	if(sctx->is_pic)
//		imty=3;

	bgbcc_setu16en(ct+16, en, imty);	//image type
	bgbcc_setu16en(ct+18, en, 0x2A);	//arch
	bgbcc_setu32en(ct+20, en, 1);		//ELF version
	bgbcc_setu32en(ct+24, en, va_strt);	//entry point
	bgbcc_setu32en(ct+28, en, of_phdr);	//program headers offset
	bgbcc_setu32en(ct+32, en, of_shdr);	//section headers offset
	bgbcc_setu32en(ct+36, en, 0);		//arch flags
	bgbcc_setu16en(ct+40, en, 52);		//header size
	bgbcc_setu16en(ct+42, en, 32);		//size of program header entry
	bgbcc_setu16en(ct+44, en, ne_phdr);	//number of program header entries
	bgbcc_setu16en(ct+46, en, 40);		//section header entry size
	bgbcc_setu16en(ct+48, en, ne_shdr);	//number of entries in section header
	bgbcc_setu16en(ct+50, en, sn_strs);	//index of section-header names

	ct=obuf+of_phdr;
	bgbcc_setu32en(ct+ 0, en, 1);			//segment type
	bgbcc_setu32en(ct+ 4, en, 0);			//segment image offset
	bgbcc_setu32en(ct+ 8, en, 0x0C000000);	//segment load address
	bgbcc_setu32en(ct+12, en, 0x0C000000);	//undefined (phys address)
	bgbcc_setu32en(ct+16, en, ofs_iend);	//size of segment in file
	bgbcc_setu32en(ct+20, en, ofs_mend);	//size of segment in memory
	bgbcc_setu32en(ct+24, en, 7);			//access flags
	bgbcc_setu32en(ct+28, en, 4096);		//alignment

	ct=obuf+of_shdr;
	ct+=40;
	for(i=0; i<sctx->nsec; i++)
	{
		s0=sctx->sec_name[i];
		if(s0)
		{
			nm=BGBCC_SHX_EmitGetStrtabSecOfs(sctx, s0);
		}

		fl=3;
		if(i==BGBCC_SH_CSEG_TEXT)fl|=4;

		lva=sctx->sec_lva[i];
		rva=sctx->sec_rva[i];
		lsz=sctx->sec_lsz[i];

		bgbcc_setu32en(ct+ 0, en, nm);		//name
		bgbcc_setu32en(ct+ 4, en, 1);		//type
		bgbcc_setu32en(ct+ 8, en, fl);		//flags
		bgbcc_setu32en(ct+12, en, lva);		//addr
		bgbcc_setu32en(ct+16, en, rva);		//offset
		bgbcc_setu32en(ct+20, en, lsz);		//size
		bgbcc_setu32en(ct+24, en, 0);		//link
		bgbcc_setu32en(ct+28, en, 0);		//info
		bgbcc_setu32en(ct+32, en, 64);		//addralign
		bgbcc_setu32en(ct+36, en, 0);		//entsize
		ct+=40;
	}
#endif

	BGBCC_SHXC_ApplyImageRelocs(ctx, sctx, obuf);

#if 1
	mapfd=fopen("aout.map", "wt");
	for(i=0; i<sctx->nlbln; i++)
	{
//		if(!strcmp(ctx->lbln_name[i], name))
//			return(ctx->lbln_id[i]);

		lva=BGBCC_SHXC_LookupLabelImgVA(ctx, sctx, sctx->lbln_id[i]);
		fprintf(mapfd, "%08X T %s\n", lva, sctx->lbln_name[i]);
	}
	fclose(mapfd);
#endif

	*rosz=ofs_iend;

	return(0);
}

