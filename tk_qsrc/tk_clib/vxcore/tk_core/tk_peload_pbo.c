/*
Load PE/PEL4 PBO Images.

PBO allows multiple program instances to exist within a single address space.
Images are loaded with base relocations applied, with data sections accessed relative to GBR.

Each image will have an index, which will be used to find its associated data section (given GBR pointing to another data section within the same process).
Every data section within a process will point to every other data section within the process.

 */

// char *tkpe_binpath[256];
// int tkpe_n_binpath;

char **tk_env_pathlst;
int tk_env_npathlst;

int tkpe_npboix=1;
int tkpe_nimgix=1;

TKPE_ImageInfo	*tkpe_pbo_image[256];
TKPE_ImageInfo	*tkpe_pbo_dllimg[256];

// void *tkpe_pbo_imgbase[256];
// char *tkpe_pbo_imgname[256];

int TKPE_TryLoadProgramImage(char *imgname, char *cwd, int isdll);
void *TKPE_LookupImageDllExport(TKPE_ImageInfo *img, char *name);
void *TKPE_LookupImageElfExport(TKPE_ImageInfo *img, char *name);

void TK_FlushCacheL1D_INVDC(void *ptr);
void TK_FlushCacheL1D_INVIC(void *ptr);
void TK_FlushCacheL1D();

int TK_Env_GetLibPathList(char ***rlst, int *rnlst);
TKPE_ImageInfo *TK_GetImageForIndex(int ix);


byte tkpe_magic_ubkey;

/*
Apply base relocations.
disp: gives the displacement relative to the image base address.
pboix: PBO Index, gives the index used for images to find their associated data sections within a process.

If pboix is used, this skips over relocations in the areas covered by GBR. These relocations will be applied when a new program instance is created.
*/
int TKPE_ApplyBaseRelocs(byte *imgptr, byte *rlc, int szrlc,
	s64 disp, int pboix,
	s64 imgbase, int gbr_rva, int gbr_sz, int mach)
{
	byte *cs, *cse, *cs1, *cs1e;
	byte *pdst;
	u32 pv, pv0, pv1, pv2, pv3;
	s64 v0, v1;
	int tgt_rva, gbr_end_rva;
	int rva_page, sz_blk;
	int isriscv, isbjx2, isbjx2xg2, isx3rv;
	int tg;

	isriscv=(mach==0x5064);
	isbjx2=(mach==0xB264);
//	isbjx2xg2=(mach==0xB265);
	isbjx2xg2 = (mach==0xB265) || (mach==0xB250);
	isx3rv=(mach==0xB253);

	gbr_end_rva=gbr_rva+gbr_sz;

	cs=rlc;		cse=rlc+szrlc;
	while(cs<cse)
	{
		rva_page=((u32 *)cs)[0];
		sz_blk=((u32 *)cs)[1];
		cs1=cs+8; cs1e=cs+sz_blk;
		cs+=sz_blk;
		
		while(cs1<cs1e)
		{
			tg=*(u16 *)cs1;
			cs1+=2;

			pdst=imgptr+rva_page+(tg&4095);

			tgt_rva=pdst-imgptr;
//			if((pboix>0) && (tgt_rva>=gbr_rva) && (tgt_rva<gbr_end_rva))
			if((tgt_rva>=gbr_rva) && (tgt_rva<gbr_end_rva))
			{
				/* Pointer is within data area. */
				if(((tg>>12)&15)==0)
					{ rva_page+=((s32)(tg<<20))>>8; }
				continue;
			}
			
			switch((tg>>12)&15)
			{
			case 0:
#if 1
//				rva_page+=(tg&4095)<<12;
				rva_page+=((s32)(tg<<20))>>8;
#endif
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
				if(isriscv || isx3rv)
				{
					/* RISC-V: Disp High 20 */
					pv=*((u32 *)pdst);
					pv+=disp&0xFFFFF000U;
					*((u32 *)pdst)=pv;
					break;
				}
			
				if(isbjx2 || isbjx2xg2)
				{
					pv=*((u32 *)pdst);
					if((pv&0x0000FE00U)==0x0000FE00U)
					{
						pv0=((u32 *)pdst)[0];
						pv1=((u32 *)pdst)[1];
						v0=((pv0&0x00FF)<<16)|((pv0>>16)&0xFFFF);
						v1=((pv1&0x00FF)<<16)|((pv1>>16)&0xFFFF);
						v1+=disp;
						v0+=(disp>>24)+(v1>>24);
						pv0=(pv0&0x0000FF00U)|
							((v0>>16)&0x000000FFU)|
							((v0<<16)&0xFFFF0000U);
						pv1=(pv1&0x0000FF00U)|
							((v1>>16)&0x000000FFU)|
							((v1<<16)&0xFFFF0000U);
						((u32 *)pdst)[0]=pv0;
						((u32 *)pdst)[1]=pv1;
						break;
					}
				
					v0=((pv&0x01FF)<<16)|((pv>>16)&0xFFFF);
					v1=v0+disp;
					pv=(pv&0x0000FE00U)|
						((v1>>16)&0x1FF)|
						((v1<<16)&0xFFFF0000U);
					*((u32 *)pdst)=pv;
					break;
				}
				__debugbreak();
				break;

			case 6:
				if(isbjx2 || isbjx2xg2)
				{
					pv=*((u16 *)pdst);
					if((pv==0xA000) && pboix)
					{
//						pv=pv|(((-pboix)*8)&0x1FFF);
						pv=pv|((pboix*8)&0x1FFF);
						*((u16 *)pdst)=pv;
						break;
					}

					pv=*((u32 *)pdst);
					if((pv==0x0000FA00UL) && pboix)
					{
						v0=((pboix*8)&0x01FFFFFF);
						pv=(pv&0x0000FF00U)|
							((v0>>16)&0x000000FFU)|
							((v0<<16)&0xFFFF0000U);
						*((u32 *)pdst)=pv;
						break;
					}

					pv0=pv;
					pv1=((u32 *)pdst)[1];

					if(	(pv0==0x0000FE00UL) &&
						!(pv1&0x00FF0000) &&
						pboix)
					{
//						v0=(((-pboix)*8)&0x00FFFFFF);
//						v0=((pboix*8)&0x00FFFFFF);
						v0=(pboix&0x00FFFFFF);

						pv0=(pv0&0x0000FF00U)|
							((v0>>24)&0x000000FFU)|
							((v0<< 8)&0xFFFF0000U);

						pv1=(pv1&0xFF00FFFF)|((v0<<16)&0x00FF0000);
						((u32 *)pdst)[0]=pv0;
						((u32 *)pdst)[1]=pv1;
						break;
					}

					if(pboix)
					{
						__debugbreak();
					}

					break;
				}
				__debugbreak();
				break;

#if 0
				pv0=((u32 *)pdst)[0];
				pv1=((u32 *)pdst)[1];
				pv2=((u32 *)pdst)[2];
				if(	((pv0&0xFDFFFF0FU)==0xB000F101UL) && pboix)
				{
					if((pv1&0xF9FFFF00U)==0xB000F100UL)
					{
						pv1|=(pboix*8)<<16;
						((u32 *)pdst)[1]=pv1;
						break;
					}
					
					if(pv1==0x0000FA00UL)
					{
						v0=((pboix*8)&0x01FFFFFF);
						pv1=(pv1&0x0000FF00U)|
							((v0>>16)&0x000000FFU)|
							((v0<<16)&0xFFFF0000U);
						((u32 *)pdst)[1]=pv1;
						break;
					}

					if((pv1==0x0000FE00U) &&
						((pv2&0xF9FFFF00U)==0xB000F100U))
					{
						v0=(pboix*8);
						
						pv1=(pv1&0x0000FF00U)|
							((v0>>24)&0x000000FFU)|
							((v0<< 8)&0xFFFF0000U);
						pv2|=(v0<<16)&0x00FF0000U;
						((u32 *)pdst)[1]=pv1;
						((u32 *)pdst)[2]=pv2;
						break;
					}
				}
#endif
				break;
				
			case 7:
				if(isriscv || isx3rv)
				{
					/* RISC-V: Low 12 I */
					pv=*((u32 *)pdst);
					pv+=(disp<<20)&0xFFF00000U;
					*((u32 *)pdst)=pv;
					break;
				}
				__debugbreak();
				break;
			case 8:
				if(isriscv || isx3rv)
				{
					/* RISC-V: Low 12 S */
					pv=*((u32 *)pdst);
					pv+=((disp<<20)&0xFE000000);
					pv+=((disp<< 7)&0x00000F80);
					*((u32 *)pdst)=pv;
					break;
				}
				__debugbreak();
				break;

			case 9:
				if(isbjx2 || isbjx2xg2)
				{
					pv=*((u32 *)pdst);

	//				if((pv==0xFA000000UL) && pboix)
					if((pv==0x0000FA00UL) && pboix)
					{
	//					v0=((pv&0x00FF)<<16)|((pv>>16)&0xFFFF);
	//					pv=pv|(((-pboix)*8)&0x01FFFFFF);
						v0=(((-pboix)*8)&0x01FFFFFF);
						pv=(pv&0x0000FF00U)|
							((v0>>16)&0x000000FFU)|
							((v0<<16)&0xFFFF0000U);
						*((u32 *)pdst)=pv;
						break;
					}

					pv0=pv;
					pv1=((u32 *)pdst)[1];

					if(	(pv0==0x0000FE00UL) &&
						!(pv1&0x00FF0000) &&
						pboix)
					{
//						v0=(((-pboix)*8)&0x00FFFFFF);
//						v0=((pboix*8)&0x00FFFFFF);
						v0=(pboix&0x00FFFFFF);

						pv0=(pv0&0x0000FF00U)|
							((v0>>24)&0x000000FFU)|
							((v0<< 8)&0xFFFF0000U);

						pv1=(pv1&0xFF00FFFF)|((v0<<16)&0x00FF0000);
						((u32 *)pdst)[0]=pv0;
						((u32 *)pdst)[1]=pv1;
						break;
					}

					break;
				}
				__debugbreak();
				break;
			case 10:
//				*((s64 *)pdst)=(*((s64 *)pdst))+disp;

				v0=*((s64 *)pdst);
				v1=v0-imgbase;
				if((v0&1) && (v1<gbr_rva))
					{ v0|=(((s64)tkpe_magic_ubkey)<<56); }
				*((s64 *)pdst)=v0+disp;
				break;
			case 11:
				break;
			default:
				tk_printf("TKPE_ApplyBaseRelocs: Unhandled Reloc %d\n",
					(tg>>12)&15);
				__debugbreak();
				break;
			}
		}
	}
	return(0);
}

/*
Apply relocations to data sections when creating a program instance.
Will ignore relocations outside of the GBR region.
 */
int TKPE_ApplyDataRelocs(
	byte *imgptr, byte *rlc, int szrlc,
	byte *dataptr,
	s64 disp_base, s64 disp_data, int pboix,
	s64 imgbase, int gbr_rva, int gbr_sz)
{
	byte *cs, *cse, *cs1, *cs1e;
	byte *pdst;
	u32 pv;
	s64 v0, v1;
//	int gbr_rva, gbr_sz;
	int tgt_rva, gbr_end_rva;
	int rva_page, rva_dest, sz_blk;
	int tg;

//	gbr_rva=gbr_rva1;
//	gbr_sz=gbr_sz1;

//	__debugbreak();

	tk_dbg_printf("TKPE_ApplyDataRelocs: disp_base=%X, disp_data=%X\n",
		disp_base, disp_data);

	gbr_end_rva=gbr_rva+gbr_sz;

	tk_dbg_printf("TKPE_ApplyDataRelocs: gbr_rva=%X..%X\n",
		gbr_rva, gbr_end_rva);

	cs=rlc;		cse=rlc+szrlc;
	tk_dbg_printf("TKPE_ApplyDataRelocs: relocs=%p..%p\n",
		cs, cse);

	while(cs<cse)
	{
		rva_page=((u32 *)cs)[0];
		sz_blk=((u32 *)cs)[1];
		cs1=cs+8; cs1e=cs+sz_blk;
		cs+=sz_blk;
		
		while(cs1<cs1e)
		{
			tg=*(u16 *)cs1;
			cs1+=2;

			rva_dest=rva_page+(tg&4095);
			if((rva_dest<gbr_rva) || (rva_dest>=gbr_end_rva))
			{
				/* Pointer is not within data area. */
				if(((tg>>12)&15)==0)
					{ rva_page+=((s32)(tg<<20))>>8; }
				continue;
			}

//			pdst=imgptr+rva_page+(tg&4095);
			pdst=dataptr+(rva_dest-gbr_rva);

//			tgt_rva=pdst-imgptr;
			
			switch((tg>>12)&15)
			{
			case 0:
#if 1
//				rva_page+=(tg&4095)<<12;
				rva_page+=((s32)(tg<<20))>>8;
#endif
				break;
			case 3:
				tgt_rva=(*((u32 *)pdst))-imgbase;				
				if((tgt_rva>=gbr_rva) && (tgt_rva<gbr_end_rva))
				{
					*((u32 *)pdst)=(*((u32 *)pdst))+disp_data;
					break;
				}
				*((u32 *)pdst)=(*((u32 *)pdst))+disp_base;
				break;
//			case 4:
//				*((u32 *)pdst)=(*((u32 *)pdst))+(disp>>32);
//				break;

			case 10:
				tgt_rva=(*((s64 *)pdst))-imgbase;				
				if((tgt_rva>=gbr_rva) && (tgt_rva<gbr_end_rva))
				{
					*((s64 *)pdst)=(*((s64 *)pdst))+disp_data;
					break;
				}

//				*((s64 *)pdst)=(*((s64 *)pdst))+disp_base;
				v0=*((s64 *)pdst);
				if(v0&1)
					v0|=(((s64)tkpe_magic_ubkey)<<56);
				*((s64 *)pdst)=v0+disp_base;
				break;
			case 11:
				break;

			default:
				tk_printf("TKPE_ApplyDataRelocs: Unhandled Reloc %d\n",
					(tg>>12)&15);
				__debugbreak();
				break;
			}
		}
	}
	return(0);
}

TKPE_ImageInfo *TKPE_LoadDynELF(TK_FILE *fd, int fdoffs,
	char *imgname, char *cwd, int is_dll);

#if 1
// byte *TKPE_LoadDynPE(TK_FILE *fd, void **rbootptr, void **rbootgbr)
TKPE_ImageInfo *TKPE_LoadDynPE(TK_FILE *fd, int fdoffs,
	char *imgname, char *cwd, int is_dll)
{
	byte tbuf[1024];
	TKPE_ImageInfo *img, *idll;
	byte *imgptr, *ct, *cte;
	byte *cs, *cse, *p0, *p1;
	char *s0, *s1;
	u64 imgbase, imgbase1;
	u32 imgsz, imgsz1, startrva, gbr_rva, gbr_sz;
	u64 entry;
	byte is64;
	byte is_pel4, cmp;
	u32 csum1, csum2;
	int sig_mz, sig_pe, mach, mmagic, mchar, mdllchar;
	int rva_rlc, sz_rlc;
	s64 rlc_disp;
	u32 tls_rva, tls_sz, tls_iptr, tls_key, tls_rds, tls_rde;
	int rva_imp, sz_imp, rva_ilt, rva_iat;
	int rva_exp, sz_exp;
	int ofs_pe, pboix, szcpy;
	int cb, nb, kb;
	int i, j, k, l, n;
	
	if(!tkpe_magic_ubkey)
	{
		k=TK_GetRandom16ASLR();
		k=(k^(k>>8))&0xFF;
		tkpe_magic_ubkey=k;
	}
	
	tk_fseek(fd, fdoffs, 0);
	tk_fread(tbuf, 1, 1024, fd);

	is_pel4=0;

	sig_mz=tkfat_getWord(tbuf);
	if(sig_mz!=0x4550)
	{
		if(sig_mz==0x457F)
		{
			return(TKPE_LoadDynELF(fd, fdoffs, imgname, cwd, is_dll));
		}

		if(sig_mz!=0x5A4D)
		{
//			printf("TKPE: MZ Sig Fail\n");
			return(NULL);
		}

		ofs_pe=tkfat_getDWord(tbuf+0x3C);
		sig_pe=tkfat_getWord(tbuf+ofs_pe);
		if(sig_pe!=0x4550)
		{
//			printf("TKPE: PE Sig Fail\n");
			return(NULL);
		}
	}else
	{
		mach=tkfat_getWord(tbuf+2);
		if(mach==0x364C)
			{ is_pel4=1; cmp=6; }
		if(mach==0x344C)
			{ is_pel4=1; cmp=4; }
		if(mach==0x334C)
			{ is_pel4=1; cmp=3; }
	
		ofs_pe=0;
		sig_pe=sig_mz;
	}

	mach=tkfat_getWord(tbuf+ofs_pe+0x04);
//	if(mach!=0xB264)
//	if((mach!=0xB264) && (mach!=0x5064))
	if(	(mach!=0xB264) && (mach!=0xB265) &&
		(mach!=0x5064) && (mach!=0xB250) &&
		(mach!=0xB296) && (mach!=0xB297) &&
		(mach!=0xB253))
	{
		printf("TKPE: Unexpected Arch %04X\n", mach);
		return(NULL);
	}

	tk_dbg_printf("TKPE: PE Sig OK\n");
	
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
		tk_dbg_printf("TKPE: Unexpected mMagic %04X\n", mach);
		return(NULL);
	}

	mchar=tkfat_getWord(tbuf+ofs_pe+0x16);
	mdllchar=tkfat_getWord(tbuf+ofs_pe+0x5E);


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

		tls_rva=*(u32 *)(tbuf+ofs_pe+0xD0);
		tls_sz=*(u32 *)(tbuf+ofs_pe+0xD4);

		rva_exp=*(u32 *)(tbuf+ofs_pe+0x88);
		sz_exp=*(u32 *)(tbuf+ofs_pe+0x8C);
		rva_imp=*(u32 *)(tbuf+ofs_pe+0x90);
		sz_imp=*(u32 *)(tbuf+ofs_pe+0x94);
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

		tls_rva=*(u32 *)(tbuf+ofs_pe+0xC0);
		tls_sz=*(u32 *)(tbuf+ofs_pe+0xC4);

		rva_exp=*(u32 *)(tbuf+ofs_pe+0x78);
		sz_exp=*(u32 *)(tbuf+ofs_pe+0x7C);
		rva_imp=*(u32 *)(tbuf+ofs_pe+0x80);
		sz_imp=*(u32 *)(tbuf+ofs_pe+0x84);
	}
	
	tk_dbg_printf("TKPE: Base=%08X Sz=%d BootRVA=%08X GbrRVA=%08X\n",
		imgbase, imgsz, startrva, gbr_rva);
	
	img=tk_malloc_krn(sizeof(TKPE_ImageInfo));
	memset(img, 0, sizeof(TKPE_ImageInfo));

//	imgsz1=(imgsz+16383)&(~16383);
	imgsz1=(imgsz+16384+16383)&(~16383);

//	i=(TK_GetRandom()*64)&16383;
	i=(TK_GetRandom16ASLR()*64)&16383;

//	imgptr=TKMM_PageAlloc(imgsz1);
//	imgptr=TKMM_PageAllocVaMap(imgsz1, TKMM_PROT_RWX,
//		TKMM_MAP_SHARED|TKMM_MAP_32BIT|TKMM_MAP_DIRECT);
	imgptr=tk_mmap(0, imgsz1, TKMM_PROT_RWX,
		TKMM_MAP_SHARED|TKMM_MAP_32BIT|TKMM_MAP_DIRECT, -1, 0);

//	memset(imgptr, 0, imgsz1-32);
	memset(imgptr, 0, imgsz1);

	TK_VMem_MProtectPages((u64)imgptr, imgsz1,
		TKMM_PROT_READ|TKMM_PROT_WRITE|
		TKMM_PROT_EXEC);

//	imgptr+=i;

	img->dllflags=
		(mchar&0x0000FFFFULL) |
		((mdllchar&0x0000FFFFULL)<<16);

	img->imgbase=imgptr;
	img->imgname=TKMM_LVA_Strdup(imgname);

	imgbase1=(u64)imgptr;
	tk_dbg_printf("TKPE!LDA:%s=%04X_%08X\n", imgname,
		(u16)(imgbase1>>32), (u32)imgbase1);

	rlc_disp=((byte *)imgptr)-((byte *)imgbase);
	
	if(!is_pel4)
	{
		tk_fseek(fd, fdoffs, 0);

		cb=0; nb=imgsz>>10;
		ct=imgptr; cte=imgptr+imgsz;
		while((ct+1024)<=cte)
		{
			tk_dbg_printf("%d/%dkB\r", cb, nb);
			tk_fread(ct, 1, 1024, fd);
			ct+=1024; cb++;
		}
		tk_dbg_printf("%d/%dkB\r", cb, nb);
		tk_fread(ct, 1, cte-ct, fd);
		tk_dbg_printf("\n", cb, nb);
	}else
	{
		memcpy(imgptr, tbuf, 1024);

		cb=0; nb=imgsz>>10;
		ct=imgptr+1024; cte=imgptr+imgsz;
		l=1024;
		while((ct+1024)<=cte)
		{
			kb=(ct-imgptr)>>10;
//			tk_dbg_printf("%d/%dkB\r", cb, nb);
			tk_dbg_printf("%d/%dkB\r", kb, nb);
			l=tk_fread(tbuf, 1, 1024, fd);
//			ct=TKPE_UnpackL4(ct, tbuf, l);
			ct=TKPE_UnpackBuffer(ct, tbuf, l, cmp);
			if(l<1024)
				break;
			cb++;
		}
		kb=(ct-imgptr)>>10;
//		tk_dbg_printf("%d/%dkB\r", cb, nb);
		tk_dbg_printf("%d/%dkB\r", kb, nb);
		if(l>=1024)
		{
			l=tk_fread(tbuf, 1, 1024, fd);
//			ct=TKPE_UnpackL4(ct, tbuf, l);
			ct=TKPE_UnpackBuffer(ct, tbuf, l, cmp);
		}
		tk_dbg_printf("\n");
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
			
			csum2=TKPE_CalculateImagePel4BChecksum(imgptr, imgsz);
			if(csum1!=csum2)
			{
				tk_printf("Image Checksum Mismatch %08X -> %08X\n",
					csum1, csum2);
				__debugbreak();
			}
		}
	}

#if 1
	pboix=0;
	if(gbr_sz!=0)
	{
		img->isdll=is_dll;

		i=tkpe_nimgix++;
		if(!i)
			i=tkpe_nimgix++;
		tkpe_pbo_image[i]=img;
		img->imgix=i;

		*(u32 *)(imgptr+0x0C)=i;

		if(is_dll&1)
		{
			pboix=tkpe_npboix++;
			tkpe_pbo_dllimg[pboix]=img;
			img->pboix=pboix;
		}
		else
		{
			/* EXE images always remain as 0. */
			img->pboix=0;
			pboix=0;
		}
	}
	
	TKPE_ApplyBaseRelocs(imgptr, imgptr+rva_rlc, sz_rlc, rlc_disp, pboix,
		imgbase, gbr_rva, gbr_sz, mach);


	TK_FlushCacheL1D();
	TK_FlushCacheL1D_INVIC(NULL);

	entry=((u64)imgptr)+startrva;

	if(mach==0xB264)
	{
		entry|=0x0000000000000001ULL;
	}

	if(mach==0x5064)
	{
//		entry|=0x0004000000000003ULL;
		entry|=0x0004000000000001ULL;
	}

	if((mach==0xB265) || (mach==0xB297))
	{
//		entry|=0x8008000000000001ULL;
		entry|=0x0088000000000001ULL;
	}

	if(mach==0xB250)
	{
//		entry|=0x0004000000000003ULL;
		entry|=0x008C000000000001ULL;
	}

	if(mach==0xB253)
	{
		entry|=0x000C000000000001ULL;
	}

	entry|=((u64)tkpe_magic_ubkey)<<56;

//	img->bootptr=imgptr+startrva;
	img->bootptr=(void *)entry;
	img->bootgbr=imgptr+gbr_rva;
	img->bootgbre=imgptr+imgsz;
	
	img->rlc_imgbase=imgbase;
	img->rlc_basedisp=rlc_disp;
	img->ptr_reloc=imgptr+rva_rlc;
	img->sz_reloc=sz_rlc;

	img->gbr_rva=gbr_rva;

	szcpy=imgsz-gbr_rva;
	if(szcpy>gbr_sz)
		szcpy=gbr_sz;
	img->gbr_szcpy=szcpy;
	img->gbr_sz=gbr_sz;
#endif

	img->tls_rva=tls_rva;

	if(tls_rva)
	{
		tls_key=TK_AllocNewTlsGbl();
		tls_iptr=*(u32 *)(imgptr+tls_rva+0x08);
		tls_rds=*(u32 *)(imgptr+tls_rva+0x00);
		tls_rde=*(u32 *)(imgptr+tls_rva+0x04);
		*(u32 *)(imgptr+tls_iptr)=tls_key;
		
		img->tls_key=tls_key;
		img->tls_rvaraw=tls_rds;
		img->tls_szraw=tls_rde-tls_rds;
		img->tls_dsize=*(u32 *)(imgptr+tls_rva+0x10);
	}

	img->rva_imp=rva_imp;
	img->rva_exp=rva_exp;
	img->sz_imp=sz_imp;
	img->sz_exp=sz_exp;

#if 0
	if(img->rva_imp)
	{
		tk_printf("TKPE: Import: RVA=%08X Sz=%08X\n", rva_imp, sz_imp);
	}

	if(img->rva_exp)
	{
		tk_printf("TKPE: Export: RVA=%08X Sz=%08X\n", rva_exp, sz_exp);
	}
#endif

	if(rva_imp)
	{
		n=sz_imp/20;
		for(i=0; i<n; i++)
		{
			j=rva_imp+(i*20);
			k=*(u32 *)(imgptr+j+0x0C);
			if(!k)
				break;
			s0=(char *)(imgptr+k);
			l=TKPE_TryLoadProgramImage(s0, cwd, 1);
			idll=TK_GetImageForIndex(l);
			img->dll[img->n_dll++]=idll;
			
			rva_ilt=*(u32 *)(imgptr+j+0x00);
			rva_iat=*(u32 *)(imgptr+j+0x10);
			
			for(j=0;; j++)
			{
				k=*(u64 *)(imgptr+rva_ilt+j*8);
				if(!k)
					break;
				s0=(char *)(imgptr+k);
				p0=TKPE_LookupImageDllExport(idll, s0);
				*(u64 *)(imgptr+rva_iat+j*8)=(u64)p0;
			}
		}
	}

	return(img);
}

void *TKPE_LookupImageDllExport(TKPE_ImageInfo *img, char *name)
{
	byte *imgptr, *fcn;
	char *s0, *s1;
	int rva_exp, sz_exp;
	int rva_eat, rva_ent, rva_eot, n_ent, n_eob, n_exps;
	int is32;
	int i, j, k;
	
	if(img->iself)
	{
		return(TKPE_LookupImageElfExport(img, name));
	}
	
	imgptr=img->imgbase;
	rva_exp=img->rva_exp;
	sz_exp=img->sz_exp;
	
	if(rva_exp<0x40)
	{
		tk_printf("TKPE_LookupImageDllExport: Bad Exports, RVA=%08X Sz=%X\n",
			rva_exp, sz_exp);
		return(NULL);
	}
	
	rva_eat=*(u32 *)(imgptr+rva_exp+0x1C);

	rva_ent=*(u32 *)(imgptr+rva_exp+0x20);
	rva_eot=*(u32 *)(imgptr+rva_exp+0x24);
	n_ent=*(u32 *)(imgptr+rva_exp+0x18);
	n_eob=*(u32 *)(imgptr+rva_exp+0x10);
	n_exps=*(u32 *)(imgptr+rva_exp+0x14);

	is32=0;
	if(n_exps<0)
	{
		n_exps=-n_exps;
		is32=1;
	}else
		if((n_eob+n_exps)>65536)
	{
		is32=1;
	}

	if(n_ent<0)
	{
		n_ent=-n_ent;
		is32=1;
	}else
		if(n_ent>=65536)
	{
		is32=1;
	}

	tk_printf("TKPE_LookupImageDllExport: "
		"ENT=%08X EOT=%08X N_Ent=%d N_EOB=%02X\n",
		rva_exp, sz_exp, n_ent, n_eob);
	
	for(i=0; i<n_ent; i++)
	{
		j=*(u32 *)(imgptr+rva_ent+i*4);
		s0=(char *)(imgptr+j);

		tk_printf("TKPE_LookupImageDllExport: Exp, Name RVA=%08X\n", j);
		tk_printf("TKPE_LookupImageDllExport: Exp, Name=%s\n", s0);

		if(!strcmp(s0, name))
		{
			if(is32)
				{ j=(*(u32 *)(imgptr+rva_eot+i*4))-n_eob; }
			else
				{ j=(*(u16 *)(imgptr+rva_eot+i*2))-n_eob; }
			k=*(u32 *)(imgptr+rva_eat+j*4);

			tk_printf("TKPE_LookupImageDllExport: Found Ord=%d RVA=%08X\n",
				j, k);

			fcn=imgptr+k;
			tk_printf("TKPE_LookupImageDllExport: Found Name=%s Ptr=%p\n",
				s0, fcn);

			return(fcn);
		}
	}

	return(NULL);
}
#endif

TK_FILE *TKPE_TryOpenImage(
	char *imgname, char *cwd, char **rpath, int isdll)
{
	char tbuf[512];
	TK_FILE *fd;
	char **path;
	int npath, isext;
	int i;

	isext=(strchr(imgname, '.')!=NULL);

//	tk_dbg_printf("TKPE_TryOpenImage: name=%s\n", imgname);

	if((imgname[0]=='/') || (isdll&2))
	{
//		tk_dbg_printf("TKPE_TryOpenImage: try %s\n", imgname);
		fd=tk_fopen(imgname, "rb");
		if(!fd && !isext && (isdll&1))
		{
			tk_sprintf(tbuf, "%s.dll", imgname);
//			tk_dbg_printf("TKPE_TryOpenImage: try %s\n", tbuf);
			fd=tk_fopen(tbuf, "rb");
		}
		if(!fd && !isext && !(isdll&1))
		{
			tk_sprintf(tbuf, "%s.exe", imgname);
//			tk_dbg_printf("TKPE_TryOpenImage: try %s\n", tbuf);
			fd=tk_fopen(tbuf, "rb");
		}
		if(fd)
		{
			if(rpath)
				*rpath=TKMM_LVA_Strdup(tbuf);
			return(fd);
		}
		
		return(NULL);
	}

	if(cwd)
	{
		tk_sprintf(tbuf, "%s/%s", cwd, imgname);
//		tk_dbg_printf("TKPE_TryOpenImage: try %s\n", tbuf);
		fd=tk_fopen(tbuf, "rb");
		if(!fd && !isext && (isdll&1))
		{
			tk_sprintf(tbuf, "%s/%s.dll", cwd, imgname);
//			tk_dbg_printf("TKPE_TryOpenImage: try %s\n", tbuf);
			fd=tk_fopen(tbuf, "rb");
		}
		if(!fd && !isext && !(isdll&1))
		{
			tk_sprintf(tbuf, "%s/%s.exe", cwd, imgname);
//			tk_dbg_printf("TKPE_TryOpenImage: try %s\n", tbuf);
			fd=tk_fopen(tbuf, "rb");
		}
		if(fd)
		{
			if(rpath)
				*rpath=TKMM_LVA_Strdup(tbuf);
			return(fd);
		}
	}

	path=NULL; npath=0;
	TK_Env_GetPathList(&path, &npath);
	for(i=0; i<npath; i++)
	{
		tk_sprintf(tbuf, "%s/%s", path[i], imgname);
		tk_dbg_printf("TKPE_TryOpenImage: try %s\n", tbuf);
		fd=tk_fopen(tbuf, "rb");
		if(!fd && !isext && (isdll&1))
		{
			tk_sprintf(tbuf, "%s/%s.dll", path[i], imgname);
			tk_dbg_printf("TKPE_TryOpenImage: try %s\n", tbuf);
			fd=tk_fopen(tbuf, "rb");
		}
		if(!fd && !isext && !(isdll&1))
		{
			tk_sprintf(tbuf, "%s/%s.exe", path[i], imgname);
			tk_dbg_printf("TKPE_TryOpenImage: try %s\n", tbuf);
			fd=tk_fopen(tbuf, "rb");
		}
		if(fd)
		{
			if(rpath)
				*rpath=TKMM_LVA_Strdup(tbuf);
			return(fd);
		}
	}
	TK_Env_FreePathList(path);

#if 1
	path=NULL; npath=0;
	TK_Env_GetLibPathList(&path, &npath);
	for(i=0; i<npath; i++)
	{
		tk_sprintf(tbuf, "%s/%s", path[i], imgname);
		tk_dbg_printf("TKPE_TryOpenImage: try %s\n", tbuf);
		fd=tk_fopen(tbuf, "rb");
		if(!fd && !isext && (isdll&1))
		{
			tk_sprintf(tbuf, "%s/%s.dll", path[i], imgname);
			tk_dbg_printf("TKPE_TryOpenImage: try %s\n", tbuf);
			fd=tk_fopen(tbuf, "rb");
		}
		if(!fd && !isext && !(isdll&1))
		{
			tk_sprintf(tbuf, "%s/%s.exe", path[i], imgname);
			tk_dbg_printf("TKPE_TryOpenImage: try %s\n", tbuf);
			fd=tk_fopen(tbuf, "rb");
		}
		if(fd)
		{
			if(rpath)
				*rpath=TKMM_LVA_Strdup(tbuf);
			return(fd);
		}
	}
	TK_Env_FreePathList(path);
#endif

	return(NULL);
}

int TKPE_LookupPboImageName(char *imgname, int isdll)
{
	TKPE_ImageInfo *img;
	int i;
	
	for(i=1; i<tkpe_nimgix; i++)
	{
		img=tkpe_pbo_image[i];

		tk_dbg_printf("TKPE_LookupPboImageName: Check %s\n", img->imgname);

		if((isdll&1) && !(img->pboix))
			continue;
		if(!strcmp(img->imgname, imgname))
			return(i);
	}
	tk_dbg_printf("TKPE_LookupPboImageName: Not Found %s\n", imgname);
	return(0);
}

int TKPE_LookupPboImagePath(char *imgpath, int isdll)
{
	TKPE_ImageInfo *img;
	int i;
	
	for(i=1; i<tkpe_nimgix; i++)
	{
		img=tkpe_pbo_image[i];

//		tk_dbg_printf("TKPE_LookupPboImagePath: Check %s\n", img->imgpath);

		if((isdll&1) && !(img->pboix))
			continue;
		if(!strcmp(img->imgpath, imgpath))
			return(i);
	}

//	tk_dbg_printf("TKPE_LookupPboImagePath: Not Found %s\n", imgpath);
	return(0);
}

int TKPE_LookupPboImagePathExt(char *imgpath, int isdll)
{
	char tbuf[512];
	int isext, ix;

	isext=(strchr(imgpath, '.')!=NULL);
	
	ix=TKPE_LookupPboImagePath(imgpath, isdll);
	if(ix>0)
		return(ix);
	if(isext)
		return(0);

	if(!isext && (isdll&1))
	{
		tk_sprintf(tbuf, "%s.dll", imgpath);
		ix=TKPE_LookupPboImagePath(tbuf, isdll);
		if(ix>0)
			return(ix);
	}

	if(!isext && !(isdll&1))
	{
		tk_sprintf(tbuf, "%s.exe", imgpath);
		ix=TKPE_LookupPboImagePath(tbuf, isdll);
		if(ix>0)
			return(ix);
	}
	
	return(0);
}

int TKPE_LookupPboImageRelPath(char *imgname, char *cwd, int isdll)
{
	char tbuf[512];
	TK_FILE *fd;
	char **path;
	int npath, isext;
	int i, j;

	if(*imgname=='/')
		imgname++;
	if(cwd && (*cwd=='/'))
		cwd++;

	isext=(strchr(imgname, '.')!=NULL);

	if((*imgname=='/') || (isdll&2))
	{
		i=TKPE_LookupPboImagePath(imgname, isdll);
		if(i>0)
			return(i);
		if(!isext && (isdll&1))
		{
			tk_sprintf(tbuf, "%s.dll", imgname);
			i=TKPE_LookupPboImagePath(tbuf, isdll);
			if(i>0)
				return(i);
		}
		if(!isext && !(isdll&1))
		{
			tk_sprintf(tbuf, "%s.exe", imgname);
			i=TKPE_LookupPboImagePath(tbuf, isdll);
			if(i>0)
				return(i);
		}
		
		return(0);
	}

	if(cwd)
	{
		tk_sprintf(tbuf, "%s/%s", cwd, imgname);
		i=TKPE_LookupPboImagePath(tbuf, isdll);
		if(i>0)
			return(i);
		if(!isext && (isdll&1))
		{
			tk_sprintf(tbuf, "%s/%s.dll", cwd, imgname);
			i=TKPE_LookupPboImagePath(tbuf, isdll);
			if(i>0)
				return(i);
		}
		if(!isext && !(isdll&1))
		{
			tk_sprintf(tbuf, "%s/%s.exe", cwd, imgname);
			i=TKPE_LookupPboImagePath(tbuf, isdll);
			if(i>0)
				return(i);
		}
	}

	path=NULL; npath=0;
	TK_Env_GetPathList(&path, &npath);
	for(i=0; i<npath; i++)
	{
		tk_sprintf(tbuf, "%s/%s", path[i], imgname);
		j=TKPE_LookupPboImagePath(tbuf, isdll);
		if(j>0)
			return(j);
		if(!isext && (isdll&1))
		{
			tk_sprintf(tbuf, "%s/%s.dll", path[i], imgname);
			j=TKPE_LookupPboImagePath(tbuf, isdll);
			if(j>0)
				return(j);
		}
		if(!isext && !(isdll&1))
		{
			tk_sprintf(tbuf, "%s/%s.exe", path[i], imgname);
			j=TKPE_LookupPboImagePath(tbuf, isdll);
			if(j>0)
				return(j);
		}
	}
	TK_Env_FreePathList(path);
	return(0);
}

int TKPE_TryLoadProgramImage(char *imgname, char *cwd, int isdll)
{
	char tbuf[128];
	TKPE_ImageInfo *img;
	char *basename, *ext;
	char *path, *ls1, *ls2;
	TK_FILE *fd;
	byte ext_exe, ext_dll;
	int i;

	ext=strchr(imgname, '.');
	
	if(ext)
	{
		ext_exe = !stricmp(ext, ".exe") || !stricmp(ext, ".elf");
		ext_dll = !stricmp(ext, ".dll") ||
			!stricmp(ext, ".so") ||
			!strncmp(ext, ".so.", 4);

		if(ext_exe && (isdll&1))
			return(0);
		if(ext_dll && !(isdll&1))
			return(0);

		if(!ext_exe && !ext_dll)
			return(0);

//		if(strcmp(ext, ".exe") && !(isdll&1))
//			return(0);
//		if(strcmp(ext, ".dll") && (isdll&1))
//			return(0);
	}
	
	
	ls1=strrchr(imgname, '/');
	if(ls1)
	{
		if(imgname[0]=='/')
		{
//			i=TKPE_LookupPboImagePath(imgname, isdll);
			i=TKPE_LookupPboImagePathExt(imgname, isdll);
			if(i>0)
				return(i);
		}else if(cwd)
		{
			i=TKPE_LookupPboImageRelPath(imgname, cwd, isdll);
			if(i>0)
				return(i);
		}
		
		strcpy_s(tbuf, 128, ls1+1);
		ls2=strchr(tbuf, '.');
		if(ls2)
			*ls2=0;
		basename=TKMM_LVA_Strdup(tbuf);
	}else
	{
		strcpy_s(tbuf, 128, imgname);
		ls2=strchr(tbuf, '.');
		if(ls2)
		{
			*ls2=0;
			basename=TKMM_LVA_Strdup(tbuf);
		}else
		{
			basename=imgname;
		}

		if(imgname!=basename)
		{
			i=TKPE_LookupPboImageName(imgname, isdll);
			if(i>0)
			{
				return(i);
			}
		}

		i=TKPE_LookupPboImageName(basename, isdll);
		if(i>0)
		{
			return(i);
		}
	}
	
	path=NULL;
	fd=TKPE_TryOpenImage(imgname, cwd, &path, isdll);
	if(!fd)
	{
		tk_dbg_printf("TKPE_TryLoadProgramImage: Not Found %s\n", imgname);
		return(0);
	}

	img=TKPE_LoadDynPE(fd, 0, basename, cwd, isdll);
	if(!img)
	{
		tk_dbg_printf("TKPE_TryLoadProgramImage: Failed to Load %s\n", imgname);
		tk_fclose(fd);
		return(0);
	}
	img->imgpath=path;
	tk_fclose(fd);
	
	return(img->imgix);
}

TKPE_ImageInfo *TK_GetImageForIndex(int ix)
{
	TKPE_ImageInfo *img;
	img=tkpe_pbo_image[ix];
	return(img);
}

void TK_InstanceImageInTask(TKPE_TaskInfo *task, TKPE_ImageInfo *img)
{
	byte *gbrdat, *tlsdat;
	int ix, gbrsz, tlssz, gbrszcpy;
	int i, j;

	gbrdat=(byte *)TK_PboGbrGetB(task, img->pboix);
	if(gbrdat)
	{
		if(*(tk_kptr *)gbrdat!=task->imggbrptrs)
			{ __debugbreak(); }
		return;
	}

	gbrdat=NULL;
	gbrsz=img->gbr_sz;

	if(gbrsz)
	{
//		gbrdat=TKMM_PageAlloc(gbrsz);
//		gbrdat=TKMM_PageAllocUsc(gbrsz);
		gbrdat=TKMM_PageAllocVaMap(gbrsz, TKMM_PROT_RW,
			TKMM_MAP_SHARED|TKMM_MAP_32BIT|TKMM_MAP_DIRECT);

		TK_TaskAddPageAlloc(task, gbrdat, gbrsz);
	}
	
	tk_dbg_printf("TK_InstanceImageInTask: "
		"GBR RVA=%X sz_cpy=%d sz=%d pboix=%d\n",
		img->gbr_rva, img->gbr_szcpy, gbrsz, img->pboix);
	
	if(gbrdat)
	{
		gbrszcpy=img->gbr_szcpy;
		gbrszcpy&=~15;
		if(gbrsz>gbrszcpy)
		{
	//		memset(gbrdat+gbrszcpy, 0, gbrsz-gbrszcpy);
			memset(gbrdat, 0, gbrsz);
		}

		memcpy(gbrdat, img->imgbase+img->gbr_rva, img->gbr_szcpy);

		TKPE_ApplyDataRelocs(
			img->imgbase, img->ptr_reloc, img->sz_reloc,
			gbrdat,
			(s64)(img->imgbase)-img->rlc_imgbase,
	//		(s64)(gbrdat)-img->rlc_imgbase,
			(s64)(gbrdat)-(img->rlc_imgbase+img->gbr_rva),
			img->pboix,
			img->rlc_imgbase, img->gbr_rva, img->gbr_sz);
	}

	TK_PboImgBaseSetB(task, img->pboix, (tk_kptr)(img->imgbase));

	if(gbrdat)
	{
		TK_PboGbrSetB(task, img->pboix, (tk_kptr)(gbrdat));
		*(tk_kptr *)gbrdat=task->imggbrptrs;
	}
	
	if(img->tls_rva)
	{
		tlssz=img->tls_dsize;
		tlsdat=TKMM_PageAlloc(tlssz);
//		tlsdat=TKMM_PageAllocUsc(tlssz);
		TK_TaskAddPageAlloc(task, tlsdat, tlssz);
		if(img->tls_rvaraw)
		{
			memcpy(tlsdat, img->imgbase+img->tls_rvaraw, img->tls_szraw);
		}
		TK_TlsSetB(task, img->tls_key, (s64)tlsdat);
	}
	
	for(i=0; i<img->n_dll; i++)
	{
		TK_InstanceImageInTask(task, img->dll[i]);
	}
}

int TKPE_SetupTaskForImage(TKPE_TaskInfo *task, TKPE_ImageInfo *img)
{
	byte *gbrdat, *gbrdatb;
	int gbrsz, szpbix;
	int i, j, k;

	task->imgbaseptrs=(tk_kptr)(task->img_baseptrs);
	task->imggbrptrs=(tk_kptr)(task->img_gbrptrs);
	task->imgtlsrvas=(tk_kptr)(task->img_tlsrvas);

	TK_InstanceImageInTask(task, img);

	gbrdat=(byte *)TK_PboGbrGetB(task, img->pboix);

	tk_dbg_printf("TKPE_SetupTaskForImage: GBR=%p\n", gbrdat);

#if 0
	szpbix=(256*sizeof(void *));
	gbrsz=img->gbr_sz+szpbix;
	gbrdat=TKMM_PageAlloc(gbrsz);
	TK_TaskAddPageAlloc(task, gbrdat, gbrsz);
	
	gbrdatb=gbrdat+szpbix;
	memcpy(gbrdatb, img->bootgbr, img->gbr_szcpy);
	
	for(i=0; i<257; i++)
	{
		((void **)gbrdat)[i]=gbrdatb;
	}

	TKPE_ApplyDataRelocs(
		img->imgbase, img->ptr_reloc, img->sz_reloc,
		gbrdatb,
		(s64)(img->imgbase)-img->rlc_imgbase,
		(s64)(gbrdatb)-img->rlc_imgbase, img->pboix,
		img->rlc_imgbase, img->gbr_rva, img->gbr_sz);
#endif

	task->baseptr=(tk_kptr)(img->imgbase);
	task->bootptr=(tk_kptr)(img->bootptr);
//	task->basegbr=gbrdat+szpbix;
	task->basegbr=(tk_kptr)gbrdat;
	task->boottbr=(tk_kptr)task;

	return(1);
}


void *TK_DlOpenB(TKPE_TaskInfo *task, const char *path, int flags)
{
	char tbuf[256];
	TKPE_ImageInfo *img;
	byte *gbrdat, *tlsdat;
	int ix, gbrsz, tlssz;
	
	TK_TaskGetCwd(task, tbuf, 256);

	ix=TKPE_TryLoadProgramImage((char *)path, tbuf, 1);
	if(ix<=0)
		return(NULL);

	img=tkpe_pbo_image[ix];
	TK_InstanceImageInTask(task, img);
	
	return(img->imgbase);
}

void *TK_DlSymB(TKPE_TaskInfo *task,
	void *handle, const char *sym, int flags)
{
	TKPE_ImageInfo *img;
	void *p;
	int ix;
	
	if(handle && sym)
	{
		/* dlsym */
		ix=*(int *)(((byte *)handle)+0x0C);
		img=tkpe_pbo_image[ix];

//		img=handle;
		p=TKPE_LookupImageDllExport(img, (char *)sym);
		return(p);
	}

	if(handle && !sym && ((flags&7)==7))
	{
		/* dlclose */
		return(NULL);
	}

	if(!handle && !sym && ((flags&7)==5))
	{
		/* dlerror */
		return(NULL);
	}

	/* otherwise */
	return(NULL);
}


void	*tk_GetApiContext_fcn[256];
u32		tk_GetApiContext_fcc[256];
byte	tk_GetApiContext_chn[256];
int		tk_n_GetApiContext=1;

byte	tk_GetApiContext_hash[256];

int TK_RegisterGetApiContextFn(u32 fcc, void *fn)
{
	int i, j, k, h;
	
	for(i=1; i<tk_n_GetApiContext; i++)
	{
		if((tk_GetApiContext_fcn[i]==fn) &&
			(tk_GetApiContext_fcc[i]==fcc))
			return(0);
	}
	
//	h=((((u16)(fcc^(fcc>>16)))*65521)>>8)&255;
	h=fcc^(fcc>>16);
	h=(h^(h>>8))&255;
	
	i=tk_n_GetApiContext++;
	tk_GetApiContext_fcn[i]=fn;
	tk_GetApiContext_fcc[i]=fcc;

	tk_GetApiContext_chn[i]=tk_GetApiContext_hash[h];
	tk_GetApiContext_hash[h]=i;

	return(1);
}

void *TKGDI_GetHalContext(TKPE_TaskInfo *task,
	u64 apiname, u64 subname);
void *TKGDI_GetHalContextComGlue(TKPE_TaskInfo *task,
	u64 apiname, u64 subname);

void *TK_DlGetApiContextB(TKPE_TaskInfo *task, u64 apiname, u64 subname)
{
	if(((u32)apiname)==TK_FCC_GDI)
	{
		return(TKGDI_GetHalContext(task, apiname, subname));
	}
	
	return(NULL);
}

void *TK_DlGetApiContextWrapB(TKPE_TaskInfo *task, u64 apiname, u64 subname)
{
	void *(*fcn)(TKPE_TaskInfo *task, u64 apiname, u64 subname);
	void *p;
	u32 fcc;
	int h, i;

	fcc=(u32)apiname;

	if(fcc==TK_FCC_GDI)
	{
		return(TKGDI_GetHalContextComGlue(task, apiname, subname));
	}

	h=fcc^(fcc>>16);
	h=(h^(h>>8))&255;
	
	i=tk_GetApiContext_hash[h];
	while(i>0)
	{
		fcn=tk_GetApiContext_fcn[i];
		if(fcc==tk_GetApiContext_fcc[i])
		{
			p=fcn(task, apiname, subname);
			if(p)
				return(p);
		}
		i=tk_GetApiContext_chn[i];
	}
	
	return(NULL);
}
