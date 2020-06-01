// extern u64 __arch_gbr;

byte *TKPE_UnpackL4(byte *ct, byte *ibuf, int isz)
{
	byte *cs, *cse;
//	register byte *cs1, *cs1e, *ct1;
	byte *cs1, *cs1e, *ct1;
//	register int tg, lr, ll, ld;
	u64 tv;
	int tg, lr, ll, ld;
	int i;
	
	__hint_use_egpr();
	
	tg=0;	lr=0;
	ll=0;	ld=0;
	
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
		{
//			printf("TKPE_UnpackL4: Hit CSE\n");
			break;
		}
		
//		ld=tkfat_getWord(cs);
		ld=*(u16 *)cs;
		cs+=2;
		if(!ld)
		{
			printf("TKPE_UnpackL4: End Of Image\n");
			break;
		}
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
//		if(ld>8)
//		if(ld>4)
//		if(0)
		{
			ct1=ct;
			while(cs1<cs1e)
			{
				((u64 *)ct1)[0]=((u64 *)cs1)[0];
				((u64 *)ct1)[1]=((u64 *)cs1)[1];
				ct1+=16; cs1+=16;
			}
//				{ *(u64 *)ct1=*(u64 *)cs1; ct1+=8; cs1+=8; }
//				{ *(u32 *)ct1=*(u32 *)cs1; ct1+=4; cs1+=4; }
			ct+=ll;
//			__debugbreak();
		}else
//			if(ld>4)
			if(ld>=4)
//		if(0)
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
		}else if(ld==1)
		{
			tv=*cs1;		tv|=tv<<8;
			tv|=tv<<16;		tv|=tv<<32;
			ct1=ct;
			while(cs1<cs1e)
			{
				((u64 *)ct1)[0]=tv;
				((u64 *)ct1)[1]=tv;
				ct1+=16; cs1+=16;
			}
			ct+=ll;
		}else
		{
			while(cs1<cs1e)
				{ *ct++=*cs1++; }
		}
	}
	
	tg=0;	lr=0;
	ll=0;	ld=0;
	
	return(ct);
}

#if 0
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
#endif

#ifdef __BJX2__
// #if 0
u32 TKPE_CalculateImagePel4BChecksum(byte *buf, int size);

__asm {
TKPE_CalculateImagePel4BChecksum:
	WEXMD	2
	ADD		R4, R5, R6
	MOV		1, R16
	MOV		0, R17

#if 1
	.L0:
								MOVU.L	(R4,  0), R20
								MOVU.L	(R4,  4), R21
								MOVU.L	(R4,  8), R22
								MOVU.L	(R4, 12), R23
	ADD		R20, R16, R18	|	ADD		R4, 16, R4
	ADD		R18, R17, R19	|	ADD		R21, R18, R16
	ADD		R16, R19, R17	|	ADD		R22, R16, R18
	ADD		R18, R17, R19	|	ADD		R23, R18, R16
	ADD		R16, R19, R17	|	CMPGT	R4, R6
								BT		.L0
#endif

#if 0
	.L0:
	MOVU.L	(R4), R7
	ADD		R7, R16
	ADD		R16, R17
	ADD		4, R4
	CMPGT	R4, R6
	BT		.L0
#endif

	SHLD.Q	R16, -32, R18
	EXTU.L	R16, R19
	ADD		R18, R19, R16
	SHLD.Q	R16, -32, R18
	EXTU.L	R16, R19
	ADD		R18, R19, R16

	SHLD.Q	R17, -32, R18
	EXTU.L	R17, R19
	ADD		R18, R19, R17
	SHLD.Q	R17, -32, R18
	EXTU.L	R17, R19
	ADD		R18, R19, R17
	
	XOR		R16, R17, R2
	RTS
};
#else
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

int TKPE_LoadStaticPE(TK_FILE *fd, void **rbootptr, void **rbootgbr)
{
//	byte tbuf[1024];
	byte tbuf[1024+32];
	byte *imgptr, *ct, *cte;
	u64 imgbase;
	u32 imgsz, startrva, gbr_rva;
	u32 csum1, csum2;
	byte is64, is_pel4;
	int sig_mz, sig_pe, mach, mmagic;
	int ofs_pe;
	int cb, nb, kb;
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
//		while((ct+1024)<=cte)
		while(ct<cte)
		{
			kb=(ct-imgptr)>>10;
//			printf("%d/%dkB\r", cb, nb);
			printf("%d/%dkB\r", kb, nb);
//			tk_fread(ct, 1, 1024, fd);
//			ct+=1024;
			memset(tbuf, 0, 1024);
			l=tk_fread(tbuf, 1, 1024, fd);
//			ct=TKPE_UnpackL4(ct, tbuf, 1024);
			ct=TKPE_UnpackL4(ct, tbuf, l);
			if(l<1024)
				break;
//			ct+=1024;
			cb++;
		}

#if 0
		kb=(ct-imgptr)>>10;
//		printf("%d/%dkB\r", cb, nb);
		printf("%d/%dkB\r", kb, nb);
		if(l>=1024)
		{
//			tk_fread(ct, 1, cte-ct, fd);
			l=tk_fread(tbuf, 1, 1024, fd);
			ct=TKPE_UnpackL4(ct, tbuf, l);
		}
#endif

		kb=(ct-imgptr)>>10;
		printf("%d/%dkB\r", kb, nb);

		printf("\n");
#endif
	}
	
	if(is_pel4)
	{
		csum1=*(u32 *)(imgptr+ofs_pe+0x58);
		
		if(csum1)
		{
			if(ct!=cte)
			{
				printf("Image Size Mismatch %08X -> %08X\n",
					cte-imgptr, ct-imgptr);
				__debugbreak();
			}
		
			*(u16 *)(imgptr+ofs_pe+0x02)=0;		
			*(u32 *)(imgptr+ofs_pe+0x58)=0;
			
	//		csum2=TKPE_CalculateImagePel4Checksum(imgptr, imgsz);
			csum2=TKPE_CalculateImagePel4BChecksum(imgptr, imgsz);
			if(csum1!=csum2)
			{
				printf("Image Checksum Mismatch %08X -> %08X\n", csum1, csum2);
				__debugbreak();
			}
		}
	}
	
	*rbootptr=imgptr+startrva;
	*rbootgbr=imgptr+gbr_rva;
#endif
}
