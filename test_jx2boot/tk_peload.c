// extern u64 __arch_gbr;

byte *TKPE_UnpackL4(byte *ct, byte *ibuf, int isz)
{
	byte *cs, *cse, *cs1, *cs1e, *ct1;
	int tg, lr, ll, ld;
	int i;
	
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
		if(ld>=8)
		{
			ct1=ct;
			while(cs1<cs1e)
				{ *(u64 *)ct1=*(u64 *)cs1; ct1+=8; cs1+=8; }
			ct+=ll;
		}else
		{
			while(cs1<cs1e)
				{ *ct++=*cs1++; }
		}
	}
	
	return(ct);
}

int TKPE_LoadStaticPE(TK_FILE *fd, void **rbootptr, void **rbootgbr)
{
	byte tbuf[1024];
	byte *imgptr, *ct, *cte;
	u64 imgbase;
	u32 imgsz, startrva, gbr_rva;
	byte is64, is_pel4;
	int sig_mz, sig_pe, mach, mmagic;
	int ofs_pe;
	int cb, nb;
	int i, l;
	
	tk_fread(tbuf, 1, 1024, fd);

	is_pel4=0;

#if 1
	sig_mz=tkfat_getWord(tbuf);
	if(sig_mz!=0x4550)
	{
		if(sig_mz!=0x5A4D)
		{
			printf("TKPE: MZ Sig Fail\n");
			return(-1);
		}

		ofs_pe=tkfat_getDWord(tbuf+0x3C);
		sig_pe=tkfat_getWord(tbuf+ofs_pe);
		if(sig_pe!=0x4550)
		{
			printf("TKPE: PE Sig Fail\n");
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
		printf("TKPE: Unexpected Arch %04X\n", mach);
		return(-1);
	}

	printf("TKPE: PE Sig OK\n");
	
	mmagic=tkfat_getWord(tbuf+ofs_pe+0x18);
	if(mmagic==0x020B)
	{
//		printf("TKPE: Magic 64 %04X\n", mmagic);
		is64=1;
	}else if(mmagic==0x010B)
	{
//		__debugbreak();
//		printf("TKPE: Magic 32 %04X\n", mmagic);
		is64=0;
	}else
	{
		printf("TKPE: Unexpected mMagic %04X\n", mach);
		return(-1);
	}
	
	if(is64)
	{
//		puts("TKPE: PE64\n");
		startrva=*(u32 *)(tbuf+ofs_pe+0x28);
		imgbase=*(u64 *)(tbuf+ofs_pe+0x30);
		imgsz=*(u32 *)(tbuf+ofs_pe+0x50);
		gbr_rva=*(u32 *)(tbuf+ofs_pe+0xC8);
	}else
	{
//		puts("TKPE: PE32\n");
		startrva=*(u32 *)(tbuf+ofs_pe+0x28);
		imgbase=*(u32 *)(tbuf+ofs_pe+0x34);
		imgsz=*(u32 *)(tbuf+ofs_pe+0x50);
		gbr_rva=*(u32 *)(tbuf+ofs_pe+0xB8);
	}
	
	printf("TKPE: Base=%08X Sz=%d BootRVA=%08X GbrRVA=%08X\n",
		imgbase, imgsz, startrva, gbr_rva);
	
	imgptr=(byte *)imgbase;
	
	if(!is_pel4)
	{
		tk_fseek(fd, 0, 0);
	//	tk_fread(imgptr, 1, imgsz, fd);

#if 1
		cb=0; nb=imgsz>>10;
		ct=imgptr; cte=imgptr+imgsz;
		while((ct+1024)<=cte)
		{
			printf("%d/%dkB\r", cb, nb);
			tk_fread(ct, 1, 1024, fd);
			ct+=1024; cb++;
		}
		printf("%d/%dkB\r", cb, nb);
		tk_fread(ct, 1, cte-ct, fd);
		printf("\n", cb, nb);
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
			printf("%d/%dkB\r", cb, nb);
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
		printf("%d/%dkB\r", cb, nb);
		if(l>=1024)
		{
//			tk_fread(ct, 1, cte-ct, fd);
			l=tk_fread(tbuf, 1, 1024, fd);
			ct=TKPE_UnpackL4(ct, tbuf, l);
		}
		printf("\n");
#endif
	}
	
	*rbootptr=imgptr+startrva;
	*rbootgbr=imgptr+gbr_rva;
#endif
}
