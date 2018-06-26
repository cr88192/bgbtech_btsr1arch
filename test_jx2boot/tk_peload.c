int TKPE_LoadStaticPE(TK_FILE *fd, void **rbootptr)
{
	byte tbuf[1024];
	byte *imgptr;
	u64 imgbase;
	u32 imgsz, startrva;
	int sig_mz, sig_pe, mach;
	int ofs_pe;
	
	tk_fread(tbuf, 1, 1024, fd);

#if 1
	sig_mz=tkfat_getWord(tbuf);
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

	mach=tkfat_getWord(tbuf+ofs_pe+0x04);
	if(mach!=0xB264)
	{
		printf("TKPE: Unexpected Arch %04X\n", mach);
		return(-1);
	}

	printf("TKPE: PE Sig OK\n");
	
	startrva=*(u32 *)(tbuf+ofs_pe+0x28);
	imgbase=*(u64 *)(tbuf+ofs_pe+0x30);
	imgsz=*(u32 *)(tbuf+ofs_pe+0x50);
	
	printf("TKPE: Base=%X Sz=%d BootRVA=%X\n", imgbase, imgsz, startrva);
	
	imgptr=(byte *)imgbase;
	
	tk_fseek(fd, 0, 0);
	tk_fread(imgptr, 1, imgsz, fd);
	
	*rbootptr=imgptr+startrva;
#endif
}
