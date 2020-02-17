// extern u64 __arch_gbr;

byte *TKPE_UnpackL4(byte *ct, byte *ibuf, int isz)
{
	byte *cs, *cse;
	register byte *cs1, *cs1e, *ct1;
//	byte *cs1, *cs1e, *ct1;
	register int tg, lr, ll, ld;
//	int tg, lr, ll, ld;
	int i;
	
	__hint_use_egpr();
	
	tg=0; lr=0; ll=0; ld=0;
	
	cs=ibuf; cse=ibuf+isz;
	while(cs<cse)
	{
		tg=*cs++;
		lr=(tg>>4)&15;
		if(lr==15)
		{
			i=*cs++;
			while(i==255)
				{ lr+=255; i=*cs++; }
			lr+=i;
		}
		
		ct1=ct; cs1=cs; cs1e=cs+lr;
		while(cs1<cs1e)
		{
			*(u64 *)ct1=*(u64 *)cs1;
			ct1+=8; cs1+=8;
		}
		ct+=lr; cs+=lr;
		
		if((cs+1)>=cse)
			break;
		
//		ld=tkfat_getWord(cs);
		ld=*(u16 *)cs;
		if(!ld)
			break;
		cs+=2;
		ll=(tg&15)+4;
		if(ll==19)
		{
			i=*cs++;
			while(i==255)
				{ ll+=255; i=*cs++; }
			ll+=i;
		}
		
		cs1=ct-ld; cs1e=cs1+ll;
//		if(ld>=8)
		if(ld>8)
		{
			ct1=ct;
			while(cs1<cs1e)
			{
				((u64 *)ct1)[0]=((u64 *)cs1)[0];
				((u64 *)ct1)[1]=((u64 *)cs1)[1];
				ct1+=16; cs1+=16;
			}
//				{ *(u64 *)ct1=*(u64 *)cs1; ct1+=8; cs1+=8; }
			ct+=ll;
		}else
			if(ld>4)
		{
			ct1=ct;
			while(cs1<cs1e)
			{
				((u32 *)ct1)[0]=((u32 *)cs1)[0];
				((u32 *)ct1)[1]=((u32 *)cs1)[1];
				ct1+=8; cs1+=8;
			}
			ct+=ll;
//			__debugbreak();
		}else
		{
			while(cs1<cs1e)
				{ *ct++=*cs1++; }
		}
	}
	
	return(ct);
}

u32 TKPE_CalculateImagePel4Checksum(byte *buf, int size)
{
	byte *cs, *cse;
//	u64 acc;
	u32 acc_lo, acc_hi;
	u32 csum;
	
	cs=buf;
	cse=cs+size;
	acc_lo=1;
	acc_hi=0;
	while(cs<cse)
	{
		acc_lo=acc_lo+(*cs++);
		if(acc_lo>=65521)
			acc_lo-=65521;
		acc_hi=acc_hi+acc_lo;
		if(acc_hi>=65521)
			acc_hi-=65521;
	}
	csum=acc_lo|(acc_hi<<16);
//	csum+=size;
	return(csum);
}

u32 TKPE_CalculateImagePel4BChecksum(byte *buf, int size)
{
	byte *cs, *cse;
	u32 v;
	u64 acc_lo, acc_hi;
	u32 csum;
	
	cs=buf;
	cse=cs+size;
	acc_lo=1;
	acc_hi=0;
	while(cs<cse)
	{
		v=*(u32 *)cs;
		acc_lo=acc_lo+v;
//		acc_lo=((u32)acc_lo)+(acc_lo>>32);

		acc_hi=acc_hi+acc_lo;
//		acc_hi=((u32)acc_hi)+(acc_hi>>32);
		cs+=4;
	}
	acc_lo=((u32)acc_lo)+(acc_lo>>32);
	acc_lo=((u32)acc_lo)+(acc_lo>>32);
	acc_hi=((u32)acc_hi)+(acc_hi>>32);
	acc_hi=((u32)acc_hi)+(acc_hi>>32);
	csum=(u32)(acc_lo^acc_hi);
//	csum+=size;
	return(csum);
}

#if 1
int TKPE_ApplyStaticRelocs(byte *imgptr, byte *rlc, int szrlc,
	s64 disp, int pboix,
	s64 imgbase, int gbr_rva, int gbr_sz)
{
	byte *cs, *cse, *cs1, *cs1e;
	byte *pdst;
	u32 pv;
	int tgt_rva, gbr_end_rva;
	int rva_page, sz_blk;
	int tg;

	tk_printf("TKPE_ApplyStaticRelocs: disp=%X rlc=%p sz=%d\n",
		(int)disp, rlc, szrlc);

	gbr_end_rva=gbr_rva+gbr_sz;

	cs=rlc;		cse=rlc+szrlc;
	while(cs<cse)
	{
		rva_page=((u32 *)cs)[0];
		sz_blk=((u32 *)cs)[1];
		cs1=cs+8; cs1e=cs+sz_blk;
		cs+=sz_blk;

		tk_printf("  RVA=%08X sz=%d\n", rva_page, sz_blk);
		
		while(cs1<cs1e)
		{
			tg=*(u16 *)cs1;
			cs1+=2;

			tgt_rva=rva_page+(tg&4095);
			pdst=imgptr+tgt_rva;
//			pdst=imgptr+rva_page+(tg&4095);
			
			if(rva_page==0x62000)
			{
				tk_printf("    %08X: %d\n", tgt_rva, (tg>>12)&15);
			}

#if 0
			tgt_rva=pdst-imgptr;
			if((pboix>0) && (tgt_rva>=gbr_rva) && (tgt_rva<gbr_end_rva))
			{
				/* Pointer is within data area. */
				continue;
			}
#endif
			
			switch((tg>>12)&15)
			{
			case 0:
				break;
			case 1:
				*((u16 *)pdst)=(*((u16 *)pdst))+(disp>>16);
				break;
			case 2:
				*((u16 *)pdst)=(*((u16 *)pdst))+(disp&65535);
				break;
			case 3:
//				tgt_rva=(*((u32 *)pdst))-imgbase;				
				*((u32 *)pdst)=(*((u32 *)pdst))+disp;
				break;
			case 4:
				*((u32 *)pdst)=(*((u32 *)pdst))+(disp>>32);
				break;
			case 5:
				pv=*((u32 *)pdst);
				pv=(pv&0xFE000000)|((pv+disp)&0x01FFFFFF);
				*((u32 *)pdst)=pv;
				break;

			case 6:
				pv=*((u16 *)pdst);
				if((pv==0xA000) && pboix)
				{
					pv=pv|(((-pboix)*8)&0x1FFF);
					*((u16 *)pdst)=pv;
					break;
				}
				break;
				
			case 7:
				break;
			case 8:
				break;

			case 9:
				pv=*((u32 *)pdst);
				if((pv==0xFA000000UL) && pboix)
				{
					pv=pv|(((-pboix)*8)&0x01FFFFFF);
					*((u32 *)pdst)=pv;
					break;
				}
				break;
			case 10:
				*((s64 *)pdst)=(*((s64 *)pdst))+disp;
				break;
			}
		}
	}
	return(0);
}
#endif

int TKPE_LoadStaticPE(TK_FILE *fd, void **rbootptr, void **rbootgbr)
{
	byte tbuf[1024];
	byte *imgptr, *ct, *cte;
	u64 imgbase;
	s64 reloc_disp;
	u32 imgsz, startrva, gbr_rva, gbr_sz, imgsz1, imgsz2;
	u32 reloc_rva, reloc_sz;
	byte is64, is_pel4;
	u32 csum1, csum2;
	int sig_mz, sig_pe, mach, mmagic;
	int ofs_pe;
	int cb, nb, kb;
	int i, l;
	
	tk_fseek(fd, 0, 0);
	tk_fread(tbuf, 1, 1024, fd);

	is_pel4=0;

#if 1
	sig_mz=tkfat_getWord(tbuf);
	if(sig_mz!=0x4550)
	{
		if(sig_mz!=0x5A4D)
		{
			tk_printf("TKPE: MZ Sig Fail\n");
			return(-1);
		}

		ofs_pe=tkfat_getDWord(tbuf+0x3C);
		sig_pe=tkfat_getWord(tbuf+ofs_pe);
		if(sig_pe!=0x4550)
		{
			tk_printf("TKPE: PE Sig Fail\n");
			return(-1);
		}
	}else
	{
		mach=tkfat_getWord(tbuf+2);
		if(mach==0x344C)
			is_pel4=1;
	
		ofs_pe=0;
		sig_pe=sig_mz;
	}

	mach=tkfat_getWord(tbuf+ofs_pe+0x04);
	if(mach!=0xB264)
	{
		tk_printf("TKPE: Unexpected Arch %04X\n", mach);
		return(-1);
	}

	tk_printf("TKPE: PE Sig OK\n");
	
	mmagic=tkfat_getWord(tbuf+ofs_pe+0x18);
	if(mmagic==0x020B)
	{
//		tk_printf("TKPE: Magic 64 %04X\n", mmagic);
		is64=1;
	}else if(mmagic==0x010B)
	{
//		__debugbreak();
//		tk_printf("TKPE: Magic 32 %04X\n", mmagic);
		is64=0;
	}else
	{
		tk_printf("TKPE: Unexpected mMagic %04X\n", mach);
		return(-1);
	}
	
	if(is64)
	{
//		puts("TKPE: PE64\n");
		startrva=*(u32 *)(tbuf+ofs_pe+0x28);
		imgbase=*(u64 *)(tbuf+ofs_pe+0x30);
		imgsz=*(u32 *)(tbuf+ofs_pe+0x50);
		gbr_rva=*(u32 *)(tbuf+ofs_pe+0xC8);
		gbr_sz=*(u32 *)(tbuf+ofs_pe+0xCC);
		reloc_rva=*(u32 *)(tbuf+ofs_pe+0xB0);
		reloc_sz=*(u32 *)(tbuf+ofs_pe+0xB4);
	}else
	{
//		puts("TKPE: PE32\n");
		startrva=*(u32 *)(tbuf+ofs_pe+0x28);
		imgbase=*(u32 *)(tbuf+ofs_pe+0x34);
		imgsz=*(u32 *)(tbuf+ofs_pe+0x50);
		gbr_rva=*(u32 *)(tbuf+ofs_pe+0xB8);
		gbr_sz=*(u32 *)(tbuf+ofs_pe+0xBC);
		reloc_rva=*(u32 *)(tbuf+ofs_pe+0xA0);
		reloc_sz=*(u32 *)(tbuf+ofs_pe+0xA4);
	}
	
	tk_printf("TKPE: Base=%08X Sz=%d BootRVA=%08X GbrRVA=%08X\n",
		imgbase, imgsz, startrva, gbr_rva);
	
	if(reloc_rva)
	{
		imgsz1=gbr_rva+gbr_sz;
		imgsz2=imgsz;
		if(imgsz1>imgsz)
			imgsz2=imgsz1;
		
	//	imgptr=(byte *)imgbase;
		imgptr=TKMM_PageAlloc(imgsz2);
		reloc_disp=(imgptr)-((byte *)imgbase);
	}else
	{
		__debugbreak();
		imgptr=(byte *)imgbase;
		reloc_disp=0;
	}
	
	if(!is_pel4)
	{
		tk_fseek(fd, 0, 0);
	//	tk_fread(imgptr, 1, imgsz, fd);

#if 1
		cb=0; nb=imgsz>>10;
		ct=imgptr; cte=imgptr+imgsz;
		while((ct+1024)<=cte)
		{
			tk_printf("%d/%dkB\r", cb, nb);
			tk_fread(ct, 1, 1024, fd);
			ct+=1024; cb++;
		}
		tk_printf("%d/%dkB\r", cb, nb);
		tk_fread(ct, 1, cte-ct, fd);
		tk_printf("\n");
#endif
	}else
	{
		memcpy(imgptr, tbuf, 1024);
#if 1
		cb=0; nb=imgsz>>10;
		ct=imgptr+1024; cte=imgptr+imgsz;
		l=1024;
		while((ct+1024)<=cte)
		{
			kb=(ct-imgptr)>>10;
//			tk_printf("%d/%dkB\r", cb, nb);
			tk_printf("%d/%dkB\r", kb, nb);
//			tk_fread(ct, 1, 1024, fd);
//			ct+=1024;
			l=tk_fread(tbuf, 1, 1024, fd);
//			ct=TKPE_UnpackL4(ct, tbuf, 1024);
			ct=TKPE_UnpackL4(ct, tbuf, l);
			if(l<1024)
				break;
//			ct+=1024;
			cb++;
		}
		kb=(ct-imgptr)>>10;
//		tk_printf("%d/%dkB\r", cb, nb);
		tk_printf("%d/%dkB\r", kb, nb);
		if(l>=1024)
		{
//			tk_fread(ct, 1, cte-ct, fd);
			l=tk_fread(tbuf, 1, 1024, fd);
			ct=TKPE_UnpackL4(ct, tbuf, l);
		}
		tk_printf("\n");
#endif
	}

	if(is_pel4)
	{
		csum1=*(u32 *)(imgptr+ofs_pe+0x58);
		
		if(csum1)
		{
			if(ct!=cte)
			{
				tk_printf("Image Size Mismatch %08X -> %08X\n",
					cte-imgptr, ct-imgptr);
				__debugbreak();
			}
		
			*(u16 *)(imgptr+ofs_pe+0x02)=0;		
			*(u32 *)(imgptr+ofs_pe+0x58)=0;
			
	//		csum2=TKPE_CalculateImagePel4Checksum(imgptr, imgsz);
			csum2=TKPE_CalculateImagePel4BChecksum(imgptr, imgsz);
			if(csum1!=csum2)
			{
				tk_printf("Image Checksum Mismatch %08X -> %08X\n",
					csum1, csum2);
				__debugbreak();
			}
		}
	}

	if(reloc_disp)
	{
		tk_printf("Reloc: RVA=%08X, sz=%d\n", reloc_rva, reloc_sz);
		TKPE_ApplyStaticRelocs(imgptr, imgptr+reloc_rva, reloc_sz,
			reloc_disp, 0, imgbase, gbr_rva, gbr_sz);
	}

	*rbootptr=imgptr+startrva;
	*rbootgbr=imgptr+gbr_rva;
#endif
}
