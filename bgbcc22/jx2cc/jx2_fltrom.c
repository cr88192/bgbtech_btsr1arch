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

ccxl_status BGBCC_JX2C_FlattenImageROM(BGBCC_TransState *ctx,
	byte *obuf, int *rosz, fourcc imgfmt)
{
	char tb[256];

	s32 map_lvatab[16384];
	char *map_lvntab[16384];

	BGBCC_JX2_Context *sctx;
	FILE *mapfd;
	char *s0;
	byte *ct;
	int en, ofs, ofs_sdat, ofs_iend, ofs_mend;
	int ofs_rd_strt;
	int ofs_rd_end;
	int of_phdr, ne_phdr;
	int of_shdr, ne_shdr;
	int lb_strt, va_strt;

	int lb_isr[8], va_isr[8];

	int nm, fl, lva, rva, lsz, sn_strs, imty;
	int va_rom, va_ram, va_base;
	int i, j, k;

	sctx=ctx->uctx;

//	BGBCC_JX2_EmitGetStrtabLabel(sctx, "");

	for(i=0; i<sctx->nsec; i++)
	{
		s0=sctx->sec_name[i];
		if(s0)
		{
//			BGBCC_JX2_EmitGetStrtabLabel(sctx, s0);
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
			case BGBCC_SH_CSEG_RELOC: s0=".reloc"; break;
			default: s0=".unknown"; break;
			}

//			BGBCC_JX2_EmitGetStrtabLabel(sctx, s0);
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

	ofs_sdat=64;
//	ofs_sdat=16;

	va_base=0;
	switch(sctx->use_memmdl)
	{
	case BGBCC_MEMMDL_TINY16:
		ofs_sdat=64;
		va_rom=ofs_sdat;
		va_ram=0xC000;
		break;
	case BGBCC_MEMMDL_MEDIUM24:
	case BGBCC_MEMMDL_SMALL24:
		ofs_sdat=64;
		va_rom=ofs_sdat;
		va_ram=0x400000;
		break;
	case BGBCC_MEMMDL_DEFAULT:
		ofs_sdat=64;
		va_rom=ofs_sdat;
		va_ram=0x01000000;
		break;
	default:
		ofs_sdat=64;
		va_rom=ofs_sdat;
		va_ram=0x01000000;
		break;
	}
	
	for(i=0; i<sctx->nsec; i++)
	{
		if(i==BGBCC_SH_CSEG_BSS)
			continue;
		j=sctx->sec_pos[i]-sctx->sec_buf[i];
		if(!BGBCC_JX2_IsSectionReadOnly(sctx, i))
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
		if(BGBCC_JX2_IsSectionReadOnly(sctx, i))
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
	
	BGBCC_JX2_EmitLabelAbs(sctx, sctx->lbl_rom_data_strt, ofs_rd_strt);
	BGBCC_JX2_EmitLabelAbs(sctx, sctx->lbl_rom_data_end, ofs_rd_end);


	lb_strt=0;
//	lb_strt=BGBCC_JX2_LookupNamedLabel(sctx, "__start");
//	if(lb_strt<=0)
//		lb_strt=BGBCC_JX2_LookupNamedLabel(sctx, "___start");
	if(lb_strt<=0)
		lb_strt=BGBCC_JX2_LookupNamedLabel(sctx, "_start");
	if(lb_strt<=0)
		lb_strt=BGBCC_JX2_LookupNamedLabel(sctx, "__start");
	
	if(lb_strt>0)
		{ va_strt=BGBCC_JX2C_LookupLabelImgVA(ctx, sctx, lb_strt); }
	else
	{
		printf("BGBCC_JX2C_FlattenImageROM: No Entry Point Found\n");
		va_strt=0;
	}

	for(i=0; i<8; i++)
		{ lb_isr[i]=lb_strt; }

	lb_isr[1]=BGBCC_JX2_LookupNamedLabel(sctx, "__isr_except");
	lb_isr[2]=BGBCC_JX2_LookupNamedLabel(sctx, "__isr_inter");
	lb_isr[3]=BGBCC_JX2_LookupNamedLabel(sctx, "__isr_mmu");
	lb_isr[4]=BGBCC_JX2_LookupNamedLabel(sctx, "__isr_syscall");

//	va_isr[1]=BGBCC_JX2C_LookupLabelImgVA(ctx, sctx, lb_isr[1]);
//	va_isr[2]=BGBCC_JX2C_LookupLabelImgVA(ctx, sctx, lb_isr[2]);
//	va_isr[3]=BGBCC_JX2C_LookupLabelImgVA(ctx, sctx, lb_isr[3]);
//	va_isr[4]=BGBCC_JX2C_LookupLabelImgVA(ctx, sctx, lb_isr[4]);

	for(i=0; i<8; i++)
		va_isr[i]=BGBCC_JX2C_LookupLabelImgVA(ctx, sctx, lb_isr[i]);

	ct=obuf;

//	if(ofs_sdat<16)
	if(1)
	{
//		bgbcc_setu16en(ct+0, en, 0xE000|(va_strt>>16)&255);
//		bgbcc_setu16en(ct+2, en, 0xD000|(va_strt>> 8)&255);
//		bgbcc_setu16en(ct+4, en, 0xD000|(va_strt>> 0)&255);
//		bgbcc_setu16en(ct+6, en, 0x3200);

#if 0
		i=(va_strt-8)/2;
//		bgbcc_setu16en(ct+0, en, 0xA000|(i>>20)&4095);
//		bgbcc_setu16en(ct+2, en, 0x2600|(i>>12)& 255);
//		bgbcc_setu16en(ct+4, en, 0x2600|(i>> 4)& 255);
//		bgbcc_setu16en(ct+6, en, 0x40F0|(i>> 0)&  15);

		bgbcc_setu16en(ct+0, en, 0xA000|((i>>16)&4095));
		bgbcc_setu16en(ct+2, en, 0x2600|((i>> 8)& 255));
		bgbcc_setu16en(ct+4, en, 0x2600|((i>> 0)& 255));
//		bgbcc_setu16en(ct+6, en, 0x3002);
		bgbcc_setu16en(ct+6, en, 0x3100);
#endif

#if 1
		i=(va_strt-4)/2;
		bgbcc_setu16en(ct+0, en, 0xF000|((i>>12)& 255));
		bgbcc_setu16en(ct+2, en, 0xC000|((i>> 0)&4095));
		bgbcc_setu16en(ct+4, en, 0xF000);
		bgbcc_setu16en(ct+6, en, 0x3030);
#endif


#if 1
		ct+=8;
		for(i=1; i<8; i++)
		{
#if 1
			j=(va_isr[i]-((i+1)*8-4))/2;
			bgbcc_setu16en(ct+0, en, 0xF000|((j>>12)& 255));
			bgbcc_setu16en(ct+2, en, 0xC000|((j>> 0)&4095));
			bgbcc_setu16en(ct+4, en, 0xF000);
			bgbcc_setu16en(ct+6, en, 0x3030);
#endif

#if 0
			j=(va_isr[i]-((i+1)*8))/2;
			bgbcc_setu16en(ct+0, en, 0xF000);
			bgbcc_setu16en(ct+2, en, 0x3000);
			bgbcc_setu16en(ct+4, en, 0xF000|((j>>12)& 255));
			bgbcc_setu16en(ct+6, en, 0xC000|((j>> 0)&4095));
#endif

#if 0
			j=(va_isr[i]-((i+1)*8))/2;
//			bgbcc_setu16en(ct+0, en, 0xA000|(j>>20)&4095);
//			bgbcc_setu16en(ct+2, en, 0x2600|(j>>12)& 255);
//			bgbcc_setu16en(ct+4, en, 0x2600|(j>> 4)& 255);
//			bgbcc_setu16en(ct+6, en, 0x40F0|(j>> 0)&  15);
			bgbcc_setu16en(ct+0, en, 0xA000|((j>>16)&4095));
			bgbcc_setu16en(ct+2, en, 0x2600|((j>> 8)& 255));
			bgbcc_setu16en(ct+4, en, 0x2600|((j>> 0)& 255));
//			bgbcc_setu16en(ct+6, en, 0x3002);
			bgbcc_setu16en(ct+6, en, 0x3100);
#endif

			ct+=8;
		}
#endif
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

	BGBCC_JX2C_ApplyImageRelocs(ctx, sctx, obuf);

#if 0
	sprintf(tb, "%s.map", ctx->imgname);
	mapfd=fopen(tb, "wt");
//	mapfd=fopen("aout.map", "wt");
	for(i=0; i<sctx->nlbln; i++)
	{
//		if(!strcmp(ctx->lbln_name[i], name))
//			return(ctx->lbln_id[i]);

		lva=BGBCC_JX2C_LookupLabelImgVA(ctx, sctx, sctx->lbln_id[i]);
		fprintf(mapfd, "%08X T %s\n", lva, sctx->lbln_name[i]);
	}
	fclose(mapfd);
#endif

#if 1
	sprintf(tb, "%s.map", ctx->imgname);
	mapfd=fopen(tb, "wt");
//	mapfd=fopen("aout.map", "wt");
	for(i=0; i<sctx->nlbln; i++)
	{
//		if(!strcmp(ctx->lbln_name[i], name))
//			return(ctx->lbln_id[i]);

		lva=BGBCC_JX2C_LookupLabelImgVA(ctx, sctx, sctx->lbln_id[i]);
//		fprintf(mapfd, "%08X T %s\n", lva, sctx->lbln_name[i]);
		map_lvatab[i]=lva;
		map_lvntab[i]=sctx->lbln_name[i];
	}
	
	for(i=0; i<sctx->nlbln; i++)
	{
		for(j=i+1; j<sctx->nlbln; j++)
		{
			if(map_lvatab[j]<map_lvatab[i])
			{
				k=map_lvatab[j];
				map_lvatab[j]=map_lvatab[i];
				map_lvatab[i]=k;

				s0=map_lvntab[j];
				map_lvntab[j]=map_lvntab[i];
				map_lvntab[i]=s0;
			}
		}
	}

	for(i=0; i<sctx->nlbln; i++)
	{
		fprintf(mapfd, "%08X T %s\n", map_lvatab[i], map_lvntab[i]);
	}

	fclose(mapfd);
#endif

	*rosz=ofs_iend;

	return(0);
}



ccxl_status BGBCC_JX2C_FlattenImageASM(BGBCC_TransState *ctx,
	byte *obuf, int *rosz, fourcc imgfmt)
{
	char tb[256];
	BGBCC_JX2_Context *sctx;
	FILE *mapfd;
	char *s0;
	byte *ct;
	int nm, fl, lva, rva, lsz, sn_strs, imty;
	int i, j, k;

	sctx=ctx->uctx;
	
	lsz=0;

	for(i=0; i<sctx->nsec; i++)
	{
		s0=sctx->sec_name[i];
		if(s0)
		{
//			BGBCC_JX2_EmitGetStrtabLabel(sctx, s0);
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

//			BGBCC_JX2_EmitGetStrtabLabel(sctx, s0);
			sctx->sec_name[i]=s0;
		}

		j=sctx->asm_pos[i]-sctx->asm_buf[i];
		lsz+=j+1024;
	}

	ct=obuf;
	for(i=0; i<sctx->nsec; i++)
	{
		j=sctx->asm_pos[i]-sctx->asm_buf[i];
		if(j<=0)
			continue;

		sprintf((char *)ct, ".section %s\n", sctx->sec_name[i]);
		ct+=strlen((char *)ct);
		
		memcpy(ct, sctx->asm_buf[i], j);
		ct+=j;
		*ct=0;
	}
	
	*rosz=ct-obuf;
	return(0);
}

ccxl_status BGBCC_JX2C_DumpImageASM(BGBCC_TransState *ctx,
	char *name)
{
	char tb[256];
	BGBCC_JX2_Context *sctx;
	FILE *fd;
	char *s0;
	byte *ct, *obuf;
	int nm, fl, lva, rva, lsz, sn_strs, imty, sz;
	int i, j, k;

	sctx=ctx->uctx;
	
	lsz=0;

	for(i=0; i<sctx->nsec; i++)
	{
		s0=sctx->sec_name[i];
		if(s0)
		{
//			BGBCC_JX2_EmitGetStrtabLabel(sctx, s0);
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

//			BGBCC_JX2_EmitGetStrtabLabel(sctx, s0);
			sctx->sec_name[i]=s0;
		}

		j=sctx->asm_pos[i]-sctx->asm_buf[i];
		lsz+=j+1024;
	}

	obuf=malloc(lsz);

	ct=obuf;
	for(i=0; i<sctx->nsec; i++)
	{
		j=sctx->asm_pos[i]-sctx->asm_buf[i];
		if(j<=0)
			continue;

		sprintf((char *)ct, ".section %s\n", sctx->sec_name[i]);
		ct+=strlen((char *)ct);
		
		memcpy(ct, sctx->asm_buf[i], j);
		ct+=j;
		*ct=0;
	}
	
//	*rosz=ct-obuf;
	sz=ct-obuf;
	
	fd=fopen(name, "wb");
	if(fd)
	{
		fwrite(obuf, 1, sz, fd);
		fclose(fd);
	}
	
	free(obuf);

	return(0);
}


ccxl_status BGBCC_JX2C_DumpImageDisAsm(BGBCC_TransState *ctx,
	char *name)
{
	char tb[256];
	BGBCC_JX2_Context *sctx;
	FILE *fd;
	char *s0;
	byte *ct, *cte, *obuf;
	byte *css, *cse, *cs;
	int obsz, opw1, opw2, il, sz;

	obsz=1<<23;
	obuf=malloc(obsz);
	cte=obuf+obsz;
	ct=obuf;

	sctx=ctx->uctx;

	css=sctx->sec_buf[BGBCC_SH_CSEG_TEXT];
	cse=sctx->sec_pos[BGBCC_SH_CSEG_TEXT];
	cs=css;
	
	while(cs<cse)
	{
		if((ct+256)>=cte)
		{
			obsz=obsz+(obsz>>1);
			il=ct-obuf;
			obuf=realloc(obuf, obsz);
			cte=obuf+obsz;
			ct=obuf+il;
		}

		opw1=((u16 *)cs)[0];
		opw2=((u16 *)cs)[1];
		il=BGBCC_JX2_TryDisassembleOpcodeBuf(sctx, &ct, cs-css, opw1, opw2);
		cs+=il*2;
	}


	sz=ct-obuf;
	
	fd=fopen(name, "wb");
	if(fd)
	{
		fwrite(obuf, 1, sz, fd);
		fclose(fd);
	}
	
	free(obuf);

	return(0);
}
