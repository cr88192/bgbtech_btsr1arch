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


/*
Apply base relocations.
disp: gives the displacement relative to the image base address.
pboix: PBO Index, gives the index used for images to find their associated data sections within a process.

If pboix is used, this skips over relocations in the areas covered by GBR. These relocations will be applied when a new program instance is created.
*/
int TKPE_ApplyBaseRelocs(byte *imgptr, byte *rlc, int szrlc,
	s64 disp, int pboix,
	s64 imgbase, int gbr_rva, int gbr_sz)
{
	byte *cs, *cse, *cs1, *cs1e;
	byte *pdst;
	u32 pv, pv0, pv1, pv2, pv3;
	s64 v0, v1;
	int tgt_rva, gbr_end_rva;
	int rva_page, sz_blk;
	int tg;

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
				continue;
			}
			
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

			case 6:
				pv=*((u16 *)pdst);
				if((pv==0xA000) && pboix)
				{
					pv=pv|(((-pboix)*8)&0x1FFF);
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
				break;
			case 8:
				break;

			case 9:
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
				break;
			case 10:
				*((s64 *)pdst)=(*((s64 *)pdst))+disp;
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
//	int gbr_rva, gbr_sz;
	int tgt_rva, gbr_end_rva;
	int rva_page, rva_dest, sz_blk;
	int tg;

//	gbr_rva=gbr_rva1;
//	gbr_sz=gbr_sz1;

//	__debugbreak();

	tk_printf("TKPE_ApplyDataRelocs: disp_base=%X, disp_data=%X\n",
		disp_base, disp_data);

	gbr_end_rva=gbr_rva+gbr_sz;

	tk_printf("TKPE_ApplyDataRelocs: gbr_rva=%X..%X\n",
		gbr_rva, gbr_end_rva);

	cs=rlc;		cse=rlc+szrlc;
	tk_printf("TKPE_ApplyDataRelocs: relocs=%p..%p\n",
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
				continue;
			}

//			pdst=imgptr+rva_page+(tg&4095);
			pdst=dataptr+(rva_dest-gbr_rva);

//			tgt_rva=pdst-imgptr;
			
			switch((tg>>12)&15)
			{
			case 0:
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
				*((s64 *)pdst)=(*((s64 *)pdst))+disp_base;
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
	u64 imgbase;
	u32 imgsz, imgsz1, startrva, gbr_rva, gbr_sz;
	byte is64;
	byte is_pel4, cmp;
	u32 csum1, csum2;
	int sig_mz, sig_pe, mach, mmagic;
	int rva_rlc, sz_rlc;
	s64 rlc_disp;
	u32 tls_rva, tls_sz, tls_iptr, tls_key, tls_rds, tls_rde;
	int rva_imp, sz_imp, rva_ilt, rva_iat;
	int rva_exp, sz_exp;
	int ofs_pe, pboix, szcpy;
	int cb, nb, kb;
	int i, j, k, l, n;
	
	tk_fseek(fd, fdoffs, 0);
	tk_fread(tbuf, 1, 1024, fd);

	is_pel4=0;

	sig_mz=tkfat_getWord(tbuf);
	if(sig_mz!=0x4550)
	{
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
	
	tk_printf("TKPE: Base=%08X Sz=%d BootRVA=%08X GbrRVA=%08X\n",
		imgbase, imgsz, startrva, gbr_rva);
	
	img=tk_malloc_krn(sizeof(TKPE_ImageInfo));
	memset(img, 0, sizeof(TKPE_ImageInfo));

//	imgsz1=(imgsz+16383)&(~16383);
	imgsz1=(imgsz+16384+16383)&(~16383);

//	i=(TK_GetRandom()*64)&16383;
	i=(TK_GetRandom16ASLR()*64)&16383;

//	imgptr=TKMM_PageAlloc(imgsz1);
	imgptr=TKMM_PageAllocVaMap(imgsz1, TKMM_PROT_RWX,
		TKMM_MAP_SHARED|TKMM_MAP_32BIT|TKMM_MAP_DIRECT);

//	memset(imgptr, 0, imgsz1-32);
//	memset(imgptr, 0, imgsz1);

	TK_VMem_MProtectPages(imgptr, imgsz1,
		TKMM_PROT_READ|TKMM_PROT_WRITE|
		TKMM_PROT_EXEC);

//	imgptr+=i;

	img->imgbase=imgptr;
	img->imgname=TKMM_LVA_Strdup(imgname);
	
	rlc_disp=((byte *)imgptr)-((byte *)imgbase);
	
	if(!is_pel4)
	{
		tk_fseek(fd, fdoffs, 0);

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
		tk_printf("\n", cb, nb);
	}else
	{
		memcpy(imgptr, tbuf, 1024);

		cb=0; nb=imgsz>>10;
		ct=imgptr+1024; cte=imgptr+imgsz;
		l=1024;
		while((ct+1024)<=cte)
		{
			kb=(ct-imgptr)>>10;
//			tk_printf("%d/%dkB\r", cb, nb);
			tk_printf("%d/%dkB\r", kb, nb);
			l=tk_fread(tbuf, 1, 1024, fd);
//			ct=TKPE_UnpackL4(ct, tbuf, l);
			ct=TKPE_UnpackBuffer(ct, tbuf, l, cmp);
			if(l<1024)
				break;
			cb++;
		}
		kb=(ct-imgptr)>>10;
//		tk_printf("%d/%dkB\r", cb, nb);
		tk_printf("%d/%dkB\r", kb, nb);
		if(l>=1024)
		{
			l=tk_fread(tbuf, 1, 1024, fd);
//			ct=TKPE_UnpackL4(ct, tbuf, l);
			ct=TKPE_UnpackBuffer(ct, tbuf, l, cmp);
		}
		tk_printf("\n");
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
		imgbase, gbr_rva, gbr_sz);

	img->bootptr=imgptr+startrva;
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
			idll=TKPE_TryLoadProgramImage(s0, cwd, 1);
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
	byte *imgptr;
	char *s0, *s1;
	int rva_exp, sz_exp;
	int rva_eat, rva_ent, rva_eot, n_ent, n_eob;
	int i, j, k;
	
	imgptr=img->imgbase;
	rva_exp=img->rva_exp;
	sz_exp=img->sz_exp;
	
	rva_eat=*(u32 *)(imgptr+rva_exp+0x1C);

	rva_ent=*(u32 *)(imgptr+rva_exp+0x20);
	rva_eot=*(u32 *)(imgptr+rva_exp+0x24);
	n_ent=*(u32 *)(imgptr+rva_exp+0x18);
	n_eob=*(u32 *)(imgptr+rva_exp+0x10);
	
	for(i=0; i<n_ent; i++)
	{
		j=*(u32 *)(imgptr+rva_ent+i*4);
		s0=(char *)(imgptr+j);
		if(!strcmp(s0, name))
		{
			j=(*(u32 *)(imgptr+rva_eot+i*2))-n_eob;
			k=*(u32 *)(imgptr+rva_eat+j*4);
			return(imgptr+k);
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

//	tk_printf("TKPE_TryOpenImage: name=%s\n", imgname);

	if((imgname[0]=='/') || (isdll&2))
	{
//		tk_printf("TKPE_TryOpenImage: try %s\n", imgname);
		fd=tk_fopen(imgname, "rb");
		if(!fd && !isext && (isdll&1))
		{
			tk_sprintf(tbuf, "%s.dll", imgname);
//			tk_printf("TKPE_TryOpenImage: try %s\n", tbuf);
			fd=tk_fopen(tbuf, "rb");
		}
		if(!fd && !isext && !(isdll&1))
		{
			tk_sprintf(tbuf, "%s.exe", imgname);
//			tk_printf("TKPE_TryOpenImage: try %s\n", tbuf);
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
//		tk_printf("TKPE_TryOpenImage: try %s\n", tbuf);
		fd=tk_fopen(tbuf, "rb");
		if(!fd && !isext && (isdll&1))
		{
			tk_sprintf(tbuf, "%s/%s.dll", cwd, imgname);
//			tk_printf("TKPE_TryOpenImage: try %s\n", tbuf);
			fd=tk_fopen(tbuf, "rb");
		}
		if(!fd && !isext && !(isdll&1))
		{
			tk_sprintf(tbuf, "%s/%s.exe", cwd, imgname);
//			tk_printf("TKPE_TryOpenImage: try %s\n", tbuf);
			fd=tk_fopen(tbuf, "rb");
		}
		if(fd)
		{
			if(rpath)
				*rpath=TKMM_LVA_Strdup(tbuf);
			return(fd);
		}
	}

	TK_Env_GetPathList(&path, &npath);
	for(i=0; i<npath; i++)
	{
		tk_sprintf(tbuf, "%s/%s", path[i], imgname);
//		tk_printf("TKPE_TryOpenImage: try %s\n", tbuf);
		fd=tk_fopen(tbuf, "rb");
		if(!fd && !isext && (isdll&1))
		{
			tk_sprintf(tbuf, "%s/%s.dll", path[i], imgname);
//			tk_printf("TKPE_TryOpenImage: try %s\n", tbuf);
			fd=tk_fopen(tbuf, "rb");
		}
		if(!fd && !isext && !(isdll&1))
		{
			tk_sprintf(tbuf, "%s/%s.exe", path[i], imgname);
//			tk_printf("TKPE_TryOpenImage: try %s\n", tbuf);
			fd=tk_fopen(tbuf, "rb");
		}
		if(fd)
		{
			if(rpath)
				*rpath=TKMM_LVA_Strdup(tbuf);
			return(fd);
		}
	}
	return(NULL);
}

int TKPE_LookupPboImageName(char *imgname, int isdll)
{
	TKPE_ImageInfo *img;
	int i;
	
	for(i=1; i<tkpe_nimgix; i++)
	{
		img=tkpe_pbo_image[i];

		tk_printf("TKPE_TryLoadProgramImage: Check %s\n", img->imgname);

		if((isdll&1) && !(img->pboix))
			continue;
		if(!strcmp(img->imgname, imgname))
			return(i);
	}
	tk_printf("TKPE_TryLoadProgramImage: Not Found %s\n", imgname);
	return(0);
}

int TKPE_LookupPboImagePath(char *imgpath, int isdll)
{
	TKPE_ImageInfo *img;
	int i;
	
	for(i=1; i<tkpe_nimgix; i++)
	{
		img=tkpe_pbo_image[i];

//		tk_printf("TKPE_LookupPboImagePath: Check %s\n", img->imgpath);

		if((isdll&1) && !(img->pboix))
			continue;
		if(!strcmp(img->imgpath, imgpath))
			return(i);
	}

//	tk_printf("TKPE_LookupPboImagePath: Not Found %s\n", imgpath);
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
	return(0);
}

int TKPE_TryLoadProgramImage(char *imgname, char *cwd, int isdll)
{
	char tbuf[128];
	TKPE_ImageInfo *img;
	char *basename, *ext;
	char *path, *ls1, *ls2;
	TK_FILE *fd;
	int i;

	ext=strchr(imgname, '.');
	
	if(ext)
	{
		if(strcmp(ext, ".exe") && !(isdll&1))
			return(0);
		if(strcmp(ext, ".dll") && (isdll&1))
			return(0);
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

		i=TKPE_LookupPboImageName(basename, isdll);
		if(i>0)
		{
			return(i);
		}
	}
	
	path=NULL;
	fd=TKPE_TryOpenImage(imgname, cwd, &path, isdll);
	if(!fd)
		return(0);
	img=TKPE_LoadDynPE(fd, 0, basename, cwd, isdll);
	if(!img)
		return(0);
	img->imgpath=path;
	
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

	gbrsz=img->gbr_sz;
//	gbrdat=TKMM_PageAlloc(gbrsz);
//	gbrdat=TKMM_PageAllocUsc(gbrsz);
	gbrdat=TKMM_PageAllocVaMap(gbrsz, TKMM_PROT_RW,
		TKMM_MAP_SHARED|TKMM_MAP_32BIT|TKMM_MAP_DIRECT);

	TK_TaskAddPageAlloc(task, gbrdat, gbrsz);
	
	tk_printf("TK_InstanceImageInTask: GBR RVA=%X sz_cpy=%d sz=%d pboix=%d\n",
		img->gbr_rva, img->gbr_szcpy, gbrsz, img->pboix);
	
	gbrszcpy=img->gbr_szcpy;
	gbrszcpy&=~15;
	if(gbrsz>gbrszcpy)
		memset(gbrdat+gbrszcpy, 0, gbrsz-gbrszcpy);

	memcpy(gbrdat, img->imgbase+img->gbr_rva, img->gbr_szcpy);

	TKPE_ApplyDataRelocs(
		img->imgbase, img->ptr_reloc, img->sz_reloc,
		gbrdat,
		(s64)(img->imgbase)-img->rlc_imgbase,
//		(s64)(gbrdat)-img->rlc_imgbase,
		(s64)(gbrdat)-(img->rlc_imgbase+img->gbr_rva),
		img->pboix,
		img->rlc_imgbase, img->gbr_rva, img->gbr_sz);

	TK_PboImgBaseSetB(task, img->pboix, (tk_kptr)(img->imgbase));

	TK_PboGbrSetB(task, img->pboix, (tk_kptr)(gbrdat));
	*(tk_kptr *)gbrdat=task->imggbrptrs;

	
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
		TK_TlsSetB(task, img->tls_key, tlsdat);
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

	task->imgbaseptrs=task->img_baseptrs;
	task->imggbrptrs=task->img_gbrptrs;
	task->imgtlsrvas=task->img_tlsrvas;

	TK_InstanceImageInTask(task, img);

	gbrdat=(byte *)TK_PboGbrGetB(task, img->pboix);

	tk_printf("TKPE_SetupTaskForImage: GBR=%p\n", gbrdat);

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

	task->baseptr=img->imgbase;
	task->bootptr=img->bootptr;
//	task->basegbr=gbrdat+szpbix;
	task->basegbr=gbrdat;
	task->boottbr=task;

	return(1);
}


void *TK_DlOpenB(TKPE_TaskInfo *task, const char *path, int flags)
{
	char tbuf[256];
	TKPE_ImageInfo *img;
	byte *gbrdat, *tlsdat;
	int ix, gbrsz, tlssz;
	
	TK_TaskGetCwd(task, tbuf, 256);

	ix=TKPE_TryLoadProgramImage(path, tbuf, 1);
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
		p=TKPE_LookupImageDllExport(img, sym);
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

void *TK_DlOpenA(const char *path, int flags)
{
	TK_SysArg ar[4];
	void *p;
	int tid;
	
#ifndef __TK_CLIB_ONLY__
	if(tk_iskernel())
	{
		p=TK_DlOpenB((TKPE_TaskInfo *)__arch_tbr, path, flags);
		return(p);
	}
#endif

	p=0;
	ar[0].p=path;
	ar[1].i=flags;
	tk_syscall(NULL, TK_UMSG_DLOPEN, &p, ar);
	return(p);
}

void *TK_DlSymA(void *handle, const char *symbol, int flags)
{
	TK_SysArg ar[4];
	void *p;
	int tid;
	
#ifndef __TK_CLIB_ONLY__
	if(tk_iskernel())
	{
		p=TK_DlSymB((TKPE_TaskInfo *)__arch_tbr, handle, symbol, flags);
		return(p);
	}
#endif

	p=0;
	ar[0].p=handle;
	ar[1].p=symbol;
	ar[2].i=flags;
	tk_syscall(NULL, TK_UMSG_DLSYM, &p, ar);
	return(p);
}
