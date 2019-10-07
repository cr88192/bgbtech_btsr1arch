/*
Load PE/PEL4 PBO Images.

PBO allows multiple program instances to exist within a single address space.
Images are loaded with base relocations applied, with data sections accessed relative to GBR.

Each image will have an index, which will be used to find its associated data section (given GBR pointing to another data section within the same process).
Every data section within a process will point to every other data section within the process.

 */

int tkpe_npboix=1;

char *tkpe_binpath[256];
int tkpe_n_binpath;

TKPE_ImageInfo	*tkpe_pbo_image[256];

// void *tkpe_pbo_imgbase[256];
// char *tkpe_pbo_imgname[256];


/*
Apply base relocations.
disp: gives the displacement relative to the image base address.
pboix: PBO Index, gives the index used for images to find their associated data sections within a process.
*/
int TKPE_ApplyRelocs(byte *imgptr, byte *rlc, int szrlc,
	s64 disp, int pboix)
{
	byte *cs, *cse, *cs1, *cs1e;
	byte *pdst;
	u32 pv;
	int rva_page, sz_blk;
	int tg;

	cs=rlc;		cse=rlc+sz_rlc;
	while(cs<cse)
	{
		rva_page=((u32 *)cs)[0];
		sz_blk=((u32 *)cs)[1];
		cs1=cs+8; cs1e=sz_blk;
		cs+=sz_blk;
		
		while(cs1<cs1e)
		{
			tg=*(u16 *)cs1;
			cs1+=2;

			pdst=imgptr+rva_page+(tg&4095);
			
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

// byte *TKPE_LoadDynPE(TK_FILE *fd, void **rbootptr, void **rbootgbr)
TKPE_ImageInfo *TKPE_LoadDynPE(TK_FILE *fd, char *imgname)
{
	byte tbuf[1024];
	TKPE_ImageInfo *img;
	byte *imgptr, *ct, *cte;
	byte *cs, *cse;
	u64 imgbase;
	u32 imgsz, startrva, gbr_rva, gbr_sz;
	byte is64, is_pel4;
	int sig_mz, sig_pe, mach, mmagic;
	int rva_rlc, sz_rlc, rlc_disp;
	int ofs_pe, pboix;
	int cb, nb;
	int i, l;
	
	tk_fread(tbuf, 1, 1024, fd);

	is_pel4=0;

	sig_mz=tkfat_getWord(tbuf);
	if(sig_mz!=0x4550)
	{
		if(sig_mz!=0x5A4D)
		{
			printf("TKPE: MZ Sig Fail\n");
			return(NULL);
		}

		ofs_pe=tkfat_getDWord(tbuf+0x3C);
		sig_pe=tkfat_getWord(tbuf+ofs_pe);
		if(sig_pe!=0x4550)
		{
			printf("TKPE: PE Sig Fail\n");
			return(NULL);
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
		return(NULL);
	}

	tk_printf("TKPE: PE Sig OK\n");
	
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

		rva_rlc=*(u32 *)(tbuf+ofs_pe+0xB0);
		sz_rlc=*(u32 *)(tbuf+ofs_pe+0xB4);
	}else
	{
//		puts("TKPE: PE32\n");
		startrva=*(u32 *)(tbuf+ofs_pe+0x28);
		imgbase=*(u32 *)(tbuf+ofs_pe+0x34);
		imgsz=*(u32 *)(tbuf+ofs_pe+0x50);
		gbr_rva=*(u32 *)(tbuf+ofs_pe+0xB8);
		gbr_sz=*(u32 *)(tbuf+ofs_pe+0xBC);

		rva_rlc=*(u32 *)(tbuf+ofs_pe+0xA0);
		sz_rlc=*(u32 *)(tbuf+ofs_pe+0xA4);
	}
	
	tk_printf("TKPE: Base=%08X Sz=%d BootRVA=%08X GbrRVA=%08X\n",
		imgbase, imgsz, startrva, gbr_rva);
	
	img=tk_malloc(sizeof(TKPE_ImageInfo));
	memset(img, 0, sizeof(TKPE_ImageInfo));

	imgptr=TKMM_PageAlloc(imgsz);
//	imgptr=(byte *)imgbase;

	img->imgbase=imgptr;
	img->imgname=TKMM_LVA_Strdup(imgname);
	
	rlc_disp=((byte *)imgptr)-((byte *)imgbase);
	
	if(!is_pel4)
	{
		tk_fseek(fd, 0, 0);

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
			l=tk_fread(tbuf, 1, 1024, fd);
			ct=TKPE_UnpackL4(ct, tbuf, l);
			if(l<1024)
				break;
			cb++;
		}
		printf("%d/%dkB\r", cb, nb);
		if(l>=1024)
		{
			l=tk_fread(tbuf, 1, 1024, fd);
			ct=TKPE_UnpackL4(ct, tbuf, l);
		}
		printf("\n");
#endif
	}
	
	pboix=0;
	if(gbr_sz!=0)
	{
		pboix=tkpe_npboix++;
		tkpe_pbo_image[pboix]=img;
		img->pboix=pboix;
	}
	
	TKPE_ApplyRelocs(imgptr, imgptr+rva_rlc, sz_rlc, rlc_disp, pboix);

	img->bootptr=imgptr+startrva;
	img->bootgbr=imgptr+gbr_rva;
	img->bootgbre=imgptr+imgsz;
	
//	*rbootptr=imgptr+startrva;
//	*rbootgbr=imgptr+gbr_rva;

	return(img);
}

TK_FILE *TKPE_TryOpenImage(char *imgname)
{
}

int TKPE_LookupPboImageName(char *imgname)
{
	int i;
	
	for(i=1; i<tkpe_npboix; i++)
	{
		if(!strcmp(tkpe_pbo_image[i]->imgname, imgname))
			return(i);
	}
	return(0);
}

int TKPE_TryLoadProgramImage(char *imgname)
{
	int i;
	
	i=TKPE_LookupPboImageName(imgname);
	if(i>0)
	{
		return(i);
	}
}

int TKPE_TryLoadProgramInstance(char *imgname,
	void **rbootptr,
	void **rbootgbr,
	void **rboottbr)
{
}
