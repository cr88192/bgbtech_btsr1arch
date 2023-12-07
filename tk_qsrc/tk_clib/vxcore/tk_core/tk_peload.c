#ifndef FOURCC
#define FOURCC(a, b, c, d)		((a)|((b)<<8)|((c)<<16)|((d)<<24))
#endif

#define FCC_PLF3		FOURCC('P', 'L', 'F', '3')
#define FCC_PLF4		FOURCC('P', 'L', 'F', '4')
#define FCC_PEL3		FOURCC('P', 'E', 'L', '3')
#define FCC_PEL4		FOURCC('P', 'E', 'L', '4')

// extern u64 __arch_gbr;

byte *TKPE_UnpackL4(byte *ct, byte *ibuf, int isz);


#ifndef __BJX2__
byte *TKPE_UnpackL6(byte *ct, byte *ibuf, int isz)
	{ return(TKPE_UnpackL4(ct, ibuf, isz)); }
#endif

#ifdef __BJX2__
byte *TKPE_UnpackL6(byte *ct, byte *ibuf, int isz);

__asm {
TKPE_UnpackL6:
//	cs=ibuf; cse=ibuf+isz;
	WEXMD	2
	ADD		R5, R6, R7
	.L0:

	/* Fetch Token Byte */
	MOVU.B	(R5, 0), R16
	MOVU.B	(R5, 1), R17
	ADD		1, R5
	SHLD	R16, -4, R18	|	AND		R16, 15, R19
	/* Fetch Extended Length */
	CMPEQ	15, R18
	ADD?T	R17, R18		|	ADD?T	1, R5
	ADD		R5, R18, R22
	MOV		R4,	R20			|	MOV		R5,	R21
	ADD		R18, R4			|	ADD		R18, R5
	/* Copy Literal Bytes */

#if 1
	.L1:
	MOV.Q	(R21, 0), R16
	MOV.Q	(R21, 8), R17
	ADD		16, R21
	MOV.Q	R16, (R20, 0)
	MOV.Q	R17, (R20, 8)
	ADD		16, R20
	CMPQGT	R21, R22
	BT		.L1
#endif

#if 0
	.L1:
	MOV.Q	(R21, 0), R16
	ADD		8, R21
	MOV.Q	R16, (R20, 0)
	ADD		8, R20
	CMPQGT	R21, R22
	BT		.L1
#endif

#if 0
	.L1:
	MOV.B	(R21), R16
	MOV.B	R16, (R20)
	ADD		1, R21
	ADD		1, R20
	CMPQGT	R21, R22
	BT		.L1
#endif

	MOVU.W	(R5, 0), R18
	MOVU.B	(R5, 2), R17
	ADD		R5, 1, R23
	ADD		2, R5

	/* Hit end of buffer yet? */
	CMPQGT	R23, R7
	BF		.Done
	CMPEQ	15, R19
	ADD?T	1, R5
	ADD?T	R17, R19
	ADD		4, R19

	/* Zero-Distance Cases? */
	TEST	R18, R18
	BF		.Copy
	CMPEQ	5, R19
	BT		.Cont
	BRA		.Done

	.Cont:
	CMPQGT	R5, R7
	BT		.L0

	.Done:
	MOV		R4, R2
	RTS

/* Match Copy Stuff */

	/* R4=Dest, R5=Source, R18=Dist, R19=Length */
	.Copy:
	ADD		R4, R19, R22	| SUB		R4, R18, R21
	MOV		R4,	R20
	ADD		R19, R4
//	BRA		.CopyL1B

	CMPGE	8, R18
	BF		.CopyRLE
	CMPGE	16, R18
	BF		.CopyL1S
//	BRA		.CopyL1S

#if 1
	.CopyL1:
	MOV.Q	(R21, 0), R16
	MOV.Q	(R21, 8), R17
	ADD		16, R21
	MOV.Q	R16, (R20, 0)
	MOV.Q	R17, (R20, 8)
	ADD		16, R20
	CMPQGT	R20, R22
	BT		.CopyL1
	BRA		.Cont
#endif

#if 1
	.CopyL1S:
	MOV.Q	(R21, 0), R16
	MOV.Q	R16, (R20, 0)
	MOV.Q	(R21, 8), R17
	MOV.Q	R17, (R20, 8)
	ADD		16, R21
	ADD		16, R20
	CMPQGT	R20, R22
	BT		.CopyL1S
	BRA		.Cont
#endif

#if 1
	.CopyL1B:
	MOV.B	(R21), R16
	ADD		1, R21
	MOV.B	R16, (R20)
	ADD		1, R20
	CMPQGT	R20, R22
	BT		.CopyL1B
	BRA		.Cont
#endif
	
	.CopyRLE:

#if 1
	CMPEQ	1, R18
	BT		.CopyRLE1
	CMPEQ	2, R18
	BT		.CopyRLE2
	CMPEQ	4, R18
	BT		.CopyRLE4
//	CMPEQ	3, R18
//	BT		.CopyRLE3
#endif

#if 1
	MOV.Q		(R21), R16
	.CopyRLE_L1:
	MOV.Q	R16, (R20, 0)
	ADD		R18, R20
	CMPQGT	R20, R22
	BT		.CopyRLE_L1
	BRA		.Cont
#endif

#if 0
	.CopyRLE_L1:
	MOV.B	(R21), R16
	ADD		1, R21
	MOV.B	R16, (R20, 0)
	ADD		1, R20
	CMPQGT	R20, R22
	BT		.CopyRLE_L1
	BRA		.Cont
#endif
	
	.CopyRLE1:
	MOVU.B		(R21), R16
	PSHUF.B		R16, 0x00, R16
	PSHUF.W		R16, 0x00, R16
	BRA			.CopyRLEN_L1
	.CopyRLE2:
	MOVU.W		(R21), R16
	PSHUF.W		R16, 0x00, R16
	BRA			.CopyRLEN_L1
	.CopyRLE4:
	MOVU.L		(R21), R16
	MOVLLD		R16, R16, R16
	BRA			.CopyRLEN_L1

	.CopyRLE3:
	MOVU.L		(R21), R16
	MOV			6, R18
//	MOV			0x00FFFFFF, R0
//	AND			R0, R16
	SHLD		R16, 8, R3		|	SHLD		R16, 24, R17
	SHLD		R3, -8, R16
	OR			R17, R16
	BRA			.CopyRLE_L1

	NOP

	.CopyRLEN_L1:
	MOV.Q	R16, (R20, 0)
	MOV.Q	R16, (R20, 8)
	ADD		16, R20
	CMPQGT	R20, R22
	BT		.CopyRLEN_L1
	BRA		.Cont
};

#endif

int TKPE_DecodeBufferRP2(
	byte *ibuf, byte *obuf, int ibsz, int obsz);

byte *TKPE_UnpackBuffer(byte *ct, byte *ibuf, int isz, int cmp);

u32 TKPE_CalculateImagePel4Checksum(byte *buf, int size);
u32 TKPE_CalculateImagePel4BChecksum(byte *buf, int size);




#if 0
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
#endif

void __setmemtrap(void *ptr, int mode);

#if 1
int TKPE_ApplyStaticRelocs(byte *imgptr, byte *rlc, int szrlc,
	s64 disp, int pboix,
	s64 imgbase, int gbr_rva, int gbr_sz)
{
	byte *cs, *cse, *cs1, *cs1e;
	byte *pdst;
	s64 v0, v1;
	u32 pv, pv0, pv1;
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

//		tk_printf("  RVA=%08X sz=%d\n", rva_page, sz_blk);
		
		while(cs1<cs1e)
		{
			tg=*(u16 *)cs1;
			cs1+=2;

			tgt_rva=rva_page+(tg&4095);
			pdst=imgptr+tgt_rva;
//			pdst=imgptr+rva_page+(tg&4095);
			
			if(rva_page==0x62000)
			{
//				tk_printf("    %08X: %d\n", tgt_rva, (tg>>12)&15);
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
			
//				pv=*((u32 *)pdst);
				v0=((pv&0x01FF)<<16)|((pv>>16)&0xFFFF);
				v1=v0+disp;
				pv=(pv&0x0000FE00U)|
					((v1>>16)&0x1FF)|
					((v1<<16)&0xFFFF0000U);
				
//				pv=(pv&0xFE000000)|((pv+disp)&0x01FFFFFF);
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
			case 11:
				__setmemtrap(pdst, 3);
				break;
			}
		}
	}
	return(0);
}
#endif

int TKPE_LoadStaticPE(
	TK_FILE *fd, int fdoffs,
	void **rbootptr, void **rbootgbr,
	u64 *tlsix)
{
	byte tbuf[1024];
	byte *imgptr, *ct, *cte, *bss_ptr;
	u64 imgbase;
	s64 reloc_disp;
	u64 entry;
	u32 imgsz, startrva, gbr_rva, gbr_sz, imgsz1, imgsz2;
	u32 reloc_rva, reloc_sz, bss_sz;
	u32 tls_rva, tls_sz, tls_iptr, tls_key, tls_rds, tls_rde;
	byte is64, is_pel4, cmp;
	u32 csum1, csum2;
	int sig_mz, sig_pe, mach, mmagic;
	int ofs_pe;
	int cb, nb, kb;
	int i, l;
	
//	tk_fseek(fd, 0, 0);
	tk_fseek(fd, fdoffs, 0);
	tk_fread(tbuf, 1, 1024, fd);

	is_pel4=0;
	
	*rbootptr = NULL;
	*rbootgbr = NULL;

#if 1
	sig_mz=tkfat_getWord(tbuf);
	if(sig_mz!=0x4550)
	{
		if(sig_mz!=0x5A4D)
		{
			tk_printf("TKPE: MZ Sig Fail, %04X\n", sig_mz);
			return(-1);
		}

		ofs_pe=tkfat_getDWord(tbuf+0x3C);
		sig_pe=tkfat_getWord(tbuf+ofs_pe);
		if(sig_pe!=0x4550)
		{
			tk_printf("TKPE: PE Sig Fail, %04X\n", sig_pe);
			return(-1);
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
		(mach!=0xB296) && (mach!=0xB297))
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

		tls_rva=*(u32 *)(tbuf+ofs_pe+0xD0);
		tls_sz=*(u32 *)(tbuf+ofs_pe+0xD4);
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

		tls_rva=*(u32 *)(tbuf+ofs_pe+0xC0);
		tls_sz=*(u32 *)(tbuf+ofs_pe+0xC4);
	}
	
	tk_printf("TKPE: Base=%08X Sz=%d BootRVA=%08X GbrRVA=%08X\n",
		imgbase, imgsz, startrva, gbr_rva);
	
	bss_ptr=NULL;
	bss_sz=0;
	
	if(reloc_rva)
	{
		imgsz1=gbr_rva+gbr_sz;
		imgsz2=imgsz;
		if(imgsz1>imgsz)
			imgsz2=imgsz1;
		
	//	imgptr=(byte *)imgbase;
		imgptr=TKMM_PageAlloc(imgsz2);
		reloc_disp=(imgptr)-((byte *)imgbase);
		
		tk_printf("TKPE: Image At %p Sz=%d\n", (long)imgptr, imgsz2);
		
		memset(imgptr, 0, imgsz2);
		
		bss_ptr=imgptr+imgsz;
		bss_sz=imgsz2-imgsz;
	}else
	{
		__debugbreak();
		imgptr=(byte *)imgbase;
		reloc_disp=0;
	}
	
	if(!is_pel4)
	{
//		tk_fseek(fd, 0, 0);
		tk_fseek(fd, fdoffs, 0);
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
//			ct=TKPE_UnpackL4(ct, tbuf, l);
			ct=TKPE_UnpackBuffer(ct, tbuf, l, cmp);
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
//			ct=TKPE_UnpackL4(ct, tbuf, l);
			ct=TKPE_UnpackBuffer(ct, tbuf, l, cmp);
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
	
	if(bss_ptr && (bss_sz>0))
	{
		memset(bss_ptr, 0, bss_sz);
	}

	if(reloc_disp)
	{
		tk_printf("Reloc: RVA=%08X, sz=%d\n", reloc_rva, reloc_sz);
		TKPE_ApplyStaticRelocs(imgptr, imgptr+reloc_rva, reloc_sz,
			reloc_disp, 0, imgbase, gbr_rva, gbr_sz);
	}

	tlsix[0]=0;
	tlsix[1]=0;
	tlsix[2]=0;
	tlsix[3]=0;
	tlsix[6]=(u64)imgptr;
	tlsix[7]=tls_rva;

	if(tls_rva)
	{
		tls_key=TK_AllocNewTlsGbl();
		tls_iptr=*(u32 *)(imgptr+tls_rva+0x08);
		tls_rds=*(u32 *)(imgptr+tls_rva+0x00);
		tls_rde=*(u32 *)(imgptr+tls_rva+0x04);
		*(u32 *)(imgptr+tls_iptr)=tls_key;
		tlsix[0]=tls_key;
		tlsix[1]=*(u32 *)(imgptr+tls_rva+0x10);

		if(tls_rds)
		{
			tlsix[2]=(u64)(imgptr+tls_rds);
			tlsix[3]=tls_rde-tls_rds;
		}
	}

	entry=((u64)imgptr)+startrva;

	if(mach==0xB264)
	{
//		entry|=0x0004000000000003ULL;
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

	*rbootptr=(void *)entry;
//	*rbootptr=imgptr+startrva;
	*rbootgbr=imgptr+gbr_rva;
#endif
}
