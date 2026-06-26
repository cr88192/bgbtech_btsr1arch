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
	u32 opw1, opw2;

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
			case BGBCC_SH_CSEG_DYN: s0=".dyn"; break;
			case BGBCC_SH_CSEG_ABS: s0=".abs"; break;
			default:
				sprintf(tb, ".unk%03X", i);
				s0=bgbcc_strdup(tb);
//				s0=".unknown";
				break;
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
	
//	sctx->gbr_rva=va_ram;

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


	sctx->gbr_rva=ofs_rd_strt;


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

	lb_isr[0]=lb_strt;
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
#if 0
		i=(va_strt-4)/2;
		bgbcc_setu16en(ct+0, en, 0xF000|((i>>12)& 255));
		bgbcc_setu16en(ct+2, en, 0xC000|((i>> 0)&4095));
		bgbcc_setu16en(ct+4, en, 0xF000);
		bgbcc_setu16en(ct+6, en, 0x3030);
#endif


#if 1
//		ct+=8;
//		for(i=1; i<8; i++)
		for(i=0; i<8; i++)
		{
#if 1
			if(sctx->emit_riscv&0x01)
			{
				j=va_isr[i]-(i*8);
				opw1=0x0000006FU|
					((j&0x000007FE)<<20)|
					((j&0x00000800)<< 9)|
					((j&0x000FF000)<< 0);
				opw2=0x00000013U;
				if(sctx->emit_riscv&0x22)
					opw2=0x0000000AU;
				bgbcc_setu32en(ct+0, en, opw1);
				bgbcc_setu32en(ct+4, en, opw2);
			}else
			{
				j=(va_isr[i]-((i+1)*8-4))/2;
				bgbcc_setu16en(ct+0, en, 0xF000|((j>>12)& 255));
				bgbcc_setu16en(ct+2, en, 0xC000|((j>> 0)&4095));
				bgbcc_setu16en(ct+4, en, 0xF000);
				bgbcc_setu16en(ct+6, en, 0x3030);
			}
#endif

			ct+=8;
		}
#endif
	}else
	{
	}

	BGBCC_JX2C_ApplyImageRelocs(ctx, sctx, obuf);

#if 1
	sprintf(tb, "%s.map", ctx->imgname);
	mapfd=fopen(tb, "wt");
	for(i=0; i<sctx->nlbln; i++)
	{
		lva=BGBCC_JX2C_LookupLabelImgVA(ctx, sctx, sctx->lbln_id[i]);
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
			case BGBCC_SH_CSEG_RELOC: s0=".reloc"; break;
			case BGBCC_SH_CSEG_DYN: s0=".dyn"; break;
			case BGBCC_SH_CSEG_ABS: s0=".abs"; break;
			default:
				sprintf(tb, ".unk%03X", i);
				s0=bgbcc_strdup(tb);
//				s0=".unknown";
				break;
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
			case BGBCC_SH_CSEG_RELOC: s0=".reloc"; break;
			case BGBCC_SH_CSEG_DYN: s0=".dyn"; break;
			case BGBCC_SH_CSEG_ABS: s0=".abs"; break;
			default:
				sprintf(tb, ".unk%03X", i);
				s0=bgbcc_strdup(tb);
//				s0=".unknown";
				break;
			}

//			BGBCC_JX2_EmitGetStrtabLabel(sctx, s0);
			sctx->sec_name[i]=s0;
		}

		j=sctx->asm_pos[i]-sctx->asm_buf[i];
		lsz+=j+1024;
	}

	obuf=bgbcc_malloc2(lsz);

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
	
	bgbcc_free2(obuf);

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
	int obsz, opw1, opw2, il, sz, lbl, lbl1, rlc;

	obsz=1<<23;
	obuf=bgbcc_malloc2(obsz);
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
			obuf=bgbcc_realloc2(obuf, obsz);
			cte=obuf+obsz;
			ct=obuf+il;
		}

//		lbl=BGBCC_JX2_LookupLabelAtOffs(sctx, BGBCC_SH_CSEG_TEXT, cs-css);
		lbl=BGBCC_JX2_LookupLabelAtOffsNoLLn(sctx, BGBCC_SH_CSEG_TEXT, cs-css);
		if(lbl>=0)
		{
			s0=BGBCC_JX2DA_NameForLabel(sctx, lbl);
			sprintf((char *)ct, "\n%s: //@%06X\n", s0, (int)(cs-css));
			ct+=strlen((char *)ct);
		}
		
		rlc=BGBCC_JX2_LookupRelocAtOffs(sctx, BGBCC_SH_CSEG_TEXT, cs-css);
		if(rlc>=0)
		{
			lbl1=sctx->rlc_id[rlc];
			s0=BGBCC_JX2DA_NameForLabel(sctx, lbl1);
			sprintf((char *)ct, ".reloc %s %02X/%s\n",
				s0, sctx->rlc_ty[rlc],
				BGBCC_JX2DA_RelocToName(sctx, sctx->rlc_ty[rlc]));
			ct+=strlen((char *)ct);
		}

		opw1=((u16 *)cs)[0];
		opw2=((u16 *)cs)[1];
		il=BGBCC_JX2_TryDisassembleOpcodeBuf(sctx,
			(char **)(&ct), cs-css, opw1, opw2);
		cs+=il*2;
	}


	sz=ct-obuf;
	
	fd=fopen(name, "wb");
	if(fd)
	{
		fwrite(obuf, 1, sz, fd);
		fclose(fd);
	}
	
	bgbcc_free2(obuf);

	return(0);
}

ccxl_status BGBCC_JX2C_FlattenImage_CompareGlobalsBasic(
	BGBCC_TransState *ctx, int idx1, int idx2)
{
	BGBCC_CCXL_RegisterInfo *obj1, *obj2;
	int c1, c2, rty1, rty2, sz1, sz2;
	obj1=ctx->reg_globals[idx1];
	obj2=ctx->reg_globals[idx2];
	c1=obj1->gblrefcnt;
	c2=obj2->gblrefcnt;
	rty1=obj1->regtype;
	rty2=obj2->regtype;

#if 0
	if(	((rty1==CCXL_LITID_GLOBALVAR) || (rty1==CCXL_LITID_STATICVAR)) &&
		((rty2==CCXL_LITID_GLOBALVAR) || (rty2==CCXL_LITID_STATICVAR)) )
	{
		sz1=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj1->type);
		sz2=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj2->type);
		if(sz1>16)		{ c1*=4.0/sqrt(sz1); }
		if(sz2>16)		{ c2*=4.0/sqrt(sz2); }
	}
#endif

	if(c1>c2)	return( 1);
	if(c1<c2)	return(-1);
	return(0);
}

ccxl_status BGBCC_JX2C_FlattenImage_CompareGlobalsProp(
	BGBCC_TransState *ctx, int idx1, int idx2)
{
	BGBCC_CCXL_RegisterInfo *obj1, *obj2;
	double f, g;
	int c1, c2, rty1, rty2, sz1, sz2;

	obj1=ctx->reg_globals[idx1];
	obj2=ctx->reg_globals[idx2];
	c1=obj1->gblrefcnt;
	c2=obj2->gblrefcnt;
	rty1=obj1->regtype;
	rty2=obj2->regtype;

	f=1.25;
	if(	((rty1==CCXL_LITID_GLOBALVAR) || (rty1==CCXL_LITID_STATICVAR)) &&
		((rty2==CCXL_LITID_GLOBALVAR) || (rty2==CCXL_LITID_STATICVAR)) )
	{
		f=1.0625;

#if 0
		sz1=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj1->type);
		sz2=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj2->type);
		if(sz1>16)		{ c1*=4.0/sqrt(sz1); }
		if(sz2>16)		{ c2*=4.0/sqrt(sz2); }
#endif
	}

	if(	(rty1==CCXL_LITID_FUNCTION) &&
		(rty2==CCXL_LITID_FUNCTION) )
			f=1.5;

	if(c1>c2)
	{
		g=(c1+1.0)/(c2+1.0);
		if(g>f)
			return( 1);
	}else
		if(c1<c2)
	{
		g=(c2+1.0)/(c1+1.0);
		if(g>f)
			return(-1);
	}	
	return(0);
}

ccxl_status BGBCC_JX2C_FlattenImage_SortGlobalsCmpR(
	BGBCC_TransState *ctx, int *shufarr, int shufgbl, int rec,
	int (*compare)(BGBCC_TransState *ctx, int idx1, int idx2))
{
	int pv, hi, lo;
	int i, j, k;

	/* Sort globals by use-count. */

//	if((shufgbl<16) || (rec>=32))
	if((shufgbl<=8) || (rec>=32))
//	if(1)
	{
		/* If needed, fall back to selsort. */
		for(i=0; i<shufgbl; i++)
		{
			for(j=i+1; j<shufgbl; j++)
			{
				k=compare(ctx, shufarr[i], shufarr[j]);
				if(k<0)
				{
					k=shufarr[i];
					shufarr[i]=shufarr[j];
					shufarr[j]=k;
				}
			}
		}
		return(0);
	}
	
	/* quicksort */

	pv=shufarr[shufgbl>>1];
	lo=0; hi=shufgbl;
	
	while(lo<hi)
	{
		k=compare(ctx, shufarr[lo], pv);
		if(k>=0)
			{ lo++; continue; }
		if(k<0)
		{
			hi--;
			k=shufarr[lo];
			shufarr[lo]=shufarr[hi];
			shufarr[hi]=k;
		}
	}
	BGBCC_JX2C_FlattenImage_SortGlobalsCmpR(
		ctx, shufarr+0, lo, rec+1, compare);
	BGBCC_JX2C_FlattenImage_SortGlobalsCmpR(
		ctx, shufarr+lo, shufgbl-lo, rec+1, compare);
	return(0);
}

ccxl_status BGBCC_JX2C_FlattenImage_SortGlobals(
	BGBCC_TransState *ctx, int *shufarr, int shufgbl)
{
	BGBCC_CCXL_RegisterInfo *obj1, *obj2;
	int i, j, k;

	BGBCC_JX2C_FlattenImage_SortGlobalsCmpR(ctx, shufarr+1, shufgbl-1,
		0, BGBCC_JX2C_FlattenImage_CompareGlobalsBasic);
	return(0);

#if 0
	/* Sort globals by use-count. */
	for(i=1; i<shufgbl; i++)
	{
		for(j=i+1; j<shufgbl; j++)
		{
			k=BGBCC_JX2C_FlattenImage_CompareGlobalsBasic(ctx,
				shufarr[i], shufarr[j]);
			if(k<0)
			{
				k=shufarr[i];
				shufarr[i]=shufarr[j];
				shufarr[j]=k;
			}

#if 0
			obj1=ctx->reg_globals[shufarr[i]];
			obj2=ctx->reg_globals[shufarr[j]];
			if(obj2->gblrefcnt>obj1->gblrefcnt)
			{
				k=shufarr[i];
				shufarr[i]=shufarr[j];
				shufarr[j]=k;
			}
#endif
		}
	}
#endif

	return(0);
}

ccxl_status BGBCC_JX2C_FlattenImage_ShuffleGlobals(
	BGBCC_TransState *ctx, int *shufarr, int shufgbl)
{
	BGBCC_CCXL_RegisterInfo *obj1, *obj2;
	double f, g;
	s64 h;
	int i, j, k;

#if 1
	h=1234;

	for(i=1; i<shufgbl; i++)
	{
		j=(((int)(h>>32))&0x0FFFFFFF)%(shufgbl);
		h=h*65521+13;
		while(!j)
		{
			j=(((int)(h>>32))&0x0FFFFFFF)%(shufgbl);
			h=h*65521+13;
		}
		k=shufarr[i];
		shufarr[i]=shufarr[j];
		shufarr[j]=k;
	}
#endif

	BGBCC_JX2C_FlattenImage_SortGlobalsCmpR(ctx, shufarr+1, shufgbl-1,
		0, BGBCC_JX2C_FlattenImage_CompareGlobalsProp);

#if 0
	/* Sort globals by use-count. */
	for(i=1; i<shufgbl; i++)
	{
		for(j=i+1; j<shufgbl; j++)
		{
			k=BGBCC_JX2C_FlattenImage_CompareGlobalsProp(ctx,
				shufarr[i], shufarr[j]);
			if(k<0)
			{
				k=shufarr[i];
				shufarr[i]=shufarr[j];
				shufarr[j]=k;
			}

#if 0
			obj1=ctx->reg_globals[shufarr[i]];
			obj2=ctx->reg_globals[shufarr[j]];
			
			f=1.25;
			if(	(obj1->regtype==CCXL_LITID_GLOBALVAR) &&
				(obj2->regtype==CCXL_LITID_GLOBALVAR) )
					f=1.0625;
			if(	(obj1->regtype==CCXL_LITID_FUNCTION) &&
				(obj2->regtype==CCXL_LITID_FUNCTION) )
					f=1.5;
			
			if((obj2->gblrefcnt>obj1->gblrefcnt) &&
//					(((obj2->gblrefcnt+1.0)/(obj1->gblrefcnt+1.0))>1.25))
//					(((obj2->gblrefcnt+1.0)/(obj1->gblrefcnt+1.0))>1.125))
				(((obj2->gblrefcnt+1.0)/(obj1->gblrefcnt+1.0))>f))
			{
				k=shufarr[i];
				shufarr[i]=shufarr[j];
				shufarr[j]=k;
			}
#endif
		}
	}
#endif

	return(0);
}

ccxl_status BGBCC_JX2C_FlattenImage_ShufCompactGlobals(
	BGBCC_TransState *ctx, int *shufarr, int shufgbl)
{
	BGBCC_CCXL_RegisterInfo *obj1, *obj2;
	int sz1, al1, sz2, al2, tsz, nswap, rty1, rty2;
	int i, j, k;

#if 1
	/* Shuffle global variables for space compacting. */
	for(i=0; i<512; i++)
	{
		tsz=0; nswap=0;
		for(j=1; j<(shufgbl-1); j++)
		{
			obj1=ctx->reg_globals[shufarr[j+0]];
			obj2=ctx->reg_globals[shufarr[j+1]];
			rty1=obj1->regtype;
			rty2=obj2->regtype;
			if(	(rty1==CCXL_LITID_GLOBALVAR) ||
				(rty1==CCXL_LITID_STATICVAR) )
			{
				if(	(rty2==CCXL_LITID_GLOBALVAR) ||
					(rty2==CCXL_LITID_STATICVAR) )
				{
					sz1=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj1->type);
					al1=BGBCC_CCXL_TypeGetLogicalAlign(ctx, obj1->type);
					sz2=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj2->type);
					al2=BGBCC_CCXL_TypeGetLogicalAlign(ctx, obj2->type);
					if(al1<1)al1=1;
					if(al2<1)al2=1;
					
//					if((tsz&(al1-1)) && (sz2<sz1))
					if(	( (tsz&(al1-1)) && (al2<al1)) ||
						(!(tsz&(al2-1)) && (al2>sz1)) ||
						((sz1>16) && (sz2<=16)))
					{
						k=shufarr[j+0];
						shufarr[j+0]=shufarr[j+1];
						shufarr[j+1]=k;
						nswap++;
						
						tsz=(tsz+(al2-1))&(~(al2-1));
						tsz+=sz2;
					}else
					{
						tsz=(tsz+(al1-1))&(~(al1-1));
						tsz+=sz1;
					}
				}else
				{
					sz1=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj1->type);
					al1=BGBCC_CCXL_TypeGetLogicalAlign(ctx, obj1->type);

					tsz=(tsz+(al1-1))&(~(al1-1));
					tsz+=sz1;
				}
			}else
			{
				if(	(rty2==CCXL_LITID_GLOBALVAR) ||
					(rty2==CCXL_LITID_STATICVAR) )
				{
					k=shufarr[j+0];
					shufarr[j+0]=shufarr[j+1];
					shufarr[j+1]=k;
					nswap++;
					
					if(j>1)
						j--;
					continue;
				}
			}
		}

		for(j=(shufgbl-2); j>=1; j--)
		{
			obj1=ctx->reg_globals[shufarr[j+0]];
			obj2=ctx->reg_globals[shufarr[j+1]];
			rty1=obj1->regtype;
			rty2=obj2->regtype;

			if(	(rty1==CCXL_LITID_GLOBALVAR) ||
				(rty1==CCXL_LITID_STATICVAR) )
			{
			}else
			{
				if(	(rty2==CCXL_LITID_GLOBALVAR) ||
					(rty2==CCXL_LITID_STATICVAR) )
				{
					k=shufarr[j+0];
					shufarr[j+0]=shufarr[j+1];
					shufarr[j+1]=k;
					nswap++;
					
//					if(j>1)
//						j--;
					continue;
				}
			}
		}

		if(!nswap)
			break;
	}
#endif

	return(0);
}
