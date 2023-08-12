typedef struct TKUCC_PeMainHeader64_s TKUCC_PeMainHeader64;
typedef struct TKUCC_PeSectionHeader_s TKUCC_PeSectionHeader;

struct TKUCC_PeMainHeader64_s
{
//0000: PE/COFF Header
    DWORD mMagicPe;                     //0000: PE\0\0 or 0x00004550
    WORD  mMachine;                     //0004:
    WORD  mNumberOfSections;            //0006:
    DWORD mTimeDateStamp;               //0008: (Creation Time, Unix Time)
    DWORD mPointerToSymbolTable;        //000C: (Unused, 0, Loaded Image Index)
    DWORD mNumberOfSymbols;             //0010: (Unused, 0)
    WORD  mSizeOfOptionalHeader;        //0014: Size of rest of header.
    WORD  mCharacteristics;             //0016:

//0018: Optional Header
    WORD  mMagic;                       //0018: 0x020b=PE32+
    BYTE  mMajorLinkerVersion;          //001A: (Ignore)
    BYTE  mMinorLinkerVersion;          //001B: (Ignore)
    DWORD mSizeOfCode;                  //001C:
    DWORD mSizeOfInitializedData;       //0020:
    DWORD mSizeOfUninitializedData;     //0024:
    DWORD rvaAddressOfEntryPoint;       //0028:
    DWORD mBaseOfCode;                  //002C:
    QWORD mImageBase;                   //0030:
    DWORD mSectionAlignment;            //0038:
    DWORD mFileAlignment;               //003C:
    WORD  mMajorOperatingSystemVersion; //0040: (Ignore?)
    WORD  mMinorOperatingSystemVersion; //0042: (Ignore?)
    WORD  mMajorImageVersion;           //0044: (Ignore?)
    WORD  mMinorImageVersion;           //0046: (Ignore?)
    WORD  mMajorSubsystemVersion;       //0048: (Ignore?)
    WORD  mMinorSubsystemVersion;       //004A: (Ignore?)
    DWORD mWin32VersionValue;           //004C: (Ignore?)
    DWORD mSizeOfImage;                 //0050:
    DWORD mSizeOfHeaders;               //0054:
    DWORD mCheckSum;                    //0058: Image Checksum
    WORD  mSubsystem;                   //005C: (Ignore?)
    WORD  mDllCharacteristics;          //005E: (Ignore?)
    QWORD mSizeOfStackReserve;          //0060:
    QWORD mSizeOfStackCommit;           //0068:
    QWORD mSizeOfHeapReserve;           //0070: (NOMMU?)
    QWORD mSizeOfHeapCommit;            //0078: (NOMMU?)
    DWORD mLoaderFlags;                 //0080: (Reserved, 0)
    DWORD mNumberOfRvaAndSizes;         //0084:

//0088: Data Directories
    DWORD rvaExportTable;               //0088:
    DWORD szExportTable;                //008C:
    DWORD rvaImportTable;               //0090:
    DWORD szImportTable;                //0094:
    DWORD rvaResourceTable;             //0098:
    DWORD szResourceTable;              //009C:
    DWORD rvaExceptionTable;            //00A0:
    DWORD szExceptionTable;             //00A4:
    DWORD rvaCertTable;                 //00A8:
    DWORD szCertTable;                  //00AC:
    DWORD rvaBaseRelocTable;            //00B0:
    DWORD szBaseRelocTable;             //00B4:
    DWORD rvaDebugData;                 //00B8:
    DWORD szDebugData;                  //00BC:
    DWORD rvaArchData;                  //00C0:
    DWORD szArchData;                   //00C4:
    DWORD rvaGlobalPtr;                 //00C8:
    DWORD szGlobalPtr;                  //00CC:
    DWORD rvaTlsTable;                  //00D0:
    DWORD szTlsTable;                   //00D4:
    DWORD rvaLoadConfigTable;           //00D8:
    DWORD szLoadConfigTable;            //00DC:
    DWORD rvaBoundImportTable;          //00E0:
    DWORD szBoundImportTable;           //00E4:
    DWORD rvaImportAddressTable;        //00E8:
    DWORD szImportAddressTable;         //00EC:
    DWORD rvaDelayImportTable;          //00F0:
    DWORD szDelayImportTable;           //00F4:
    DWORD rvaClrHeader;                 //00F8:
    DWORD szClrHeader;                  //00FC:
    DWORD rvaResvTable;                 //0100:
    DWORD szResvTable;                  //0104:
//0108:End of Header
};

struct TKUCC_PeSectionHeader_s
{
    BYTE  mName[8];                     //0000, Section Name
    DWORD mVirtualSize;                 //0008, Size of section in memory
    DWORD mVirtualAddress;              //000C, RVA Address of section in memory
    DWORD mSizeOfRawData;               //0010, Size of section in image
    DWORD mPointerToRawData;            //0014, Offset to section data in image
    DWORD mPointerToRealocations;       //0018, Unused, Zero
    DWORD mPointerToLinenumbers;        //001C, Unused, Zero
    WORD  mNumberOfRealocations;        //0020, Unused, Zero
    WORD  mNumberOfLinenumbers;         //0022, Unused, Zero
    DWORD mCharacteristics;             //0024, Section Flags
    //28
};


u32 TKUCC_CalculateImagePel4BChecksum(byte *buf, int size, int en)
{
	byte *cs, *cse;
//	u64 acc;
	u32 v;
	u64 acc_lo, acc_hi;
	u32 csum;
	
	cs=buf;
	cse=cs+size;
	acc_lo=1;
	acc_hi=0;
	while(cs<cse)
	{
//		v=bgbcc_getu32en(cs, en);
		v=*(u64 *)(cs);
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

void TKUCC_LinkFlattenObjPEL_BuildBaseRelocs(
	TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj)
{
	TKUCC_LinkObjectLbl t_lbl, t_rlc;
	TKUCC_LinkObjectLbl *lbl, *rlc;
	u32 *temprlc;
	byte *tbuf, *ct, *ctp;
	u32 rva, rvap;
	int ntrlc, brty;
	int i, j, k;
	
	temprlc=tkucc_malloc(obj->n_rlc*sizeof(u32));
	ntrlc=0;

	rlc=&t_rlc;

	for(i=0; i<obj->n_rlc; i++)
	{
		TKUCC_ObjGetReloc(ctx, obj, i, rlc);
		if(!rlc->sym_sec)
			continue;
		
		rva=obj->sec[rlc->sym_sec-1]->ct_rva+rlc->sym_ofs;
		
		switch(rlc->sym_type)
		{
		case TKUCC_RELOC_DIR32: brty=3; break;
		case TKUCC_RELOC_DIR64: brty=10; break;
		default:
			brty=-1;
			break;
		}
		
		if(brty<0)
			continue;
		
		temprlc[ntrlc++]=(rva<<4)|brty;
	}
	
	if(!ntrlc)
		return;
	
	for(i=0; i<ntrlc; i++)
	{
		for(j=i+1; j<ntrlc; j++)
		{
			if(temprlc[j]<temprlc[i])
			{
				rva=temprlc[i];
				temprlc[i]=temprlc[j];
				temprlc[j]=rva;
			}
		}
	}
	
	tbuf=tkucc_malloc(1024+ntrlc*4);
	
	rvap=~0;
	ct=tbuf; ctp=NULL;
	for(i=0; i<ntrlc; i++)
	{
		rva=temprlc[i];
		brty=rva&15;
		rva>>=4;

		if((rva&(~0xFFF))!=rvap)
		{
			if(ctp)
			{
				((u32 *)ctp)[0]=rvap;
				((u32 *)ctp)[1]=ct-ctp;
			}
			rvap=(rva&(~0xFFF));
			ctp=ct;
			ct+=8;
		}
		
		j=(brty<<12)|(rva&0xFFF);
	}
	if(ctp)
	{
		((u32 *)ctp)[0]=rvap;
		((u32 *)ctp)[1]=ct-ctp;
	}
	
	k=ct-tbuf;
	TKUCC_ObjSetSection(ctx, obj, ".reloc");
	TKUCC_ObjWriteSectionBytes(ctx, obj, 0, obj->c_sec, tbuf, k);
	obj->sec[obj->c_sec]->ct_size=k;
}

void TKUCC_LinkFlattenObjPEL(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, byte **rbuf, int *rsize)
{
	TKUCC_PeMainHeader64 *head;
	TKUCC_PeSectionHeader *sech;
	byte *imgbuf;
	char *s;

	u64 addrbase;
	u32 trva, rva_base, rva_data, rva_bss, rva_max, rva_imgsz, rva_start;
	int i, j, k, scat;
	
	trva=0x400;

	rva_base=trva;
	for(i=0; i<obj->n_sec; i++)
	{
		/* Read only sections first. */
		scat=TKUCC_LinkGetSectionCategory(ctx, obj, i);
		if(scat!=1)
			continue;
	
		k=obj->sec[i]->ct_size;
		k=(k+1023)&(~1023);
		
		obj->sec[i]->ct_rva=trva;
		trva+=k;
	}

	trva=(trva+16383)&(~16383);

	rva_data=trva;
	for(i=0; i<obj->n_sec; i++)
	{
		/* Data Sections. */
		scat=TKUCC_LinkGetSectionCategory(ctx, obj, i);
		if(scat!=2)
			continue;
	
		k=obj->sec[i]->ct_size;
		k=(k+1023)&(~1023);
		
		obj->sec[i]->ct_rva=trva;
		trva+=k;
	}

	rva_bss=trva;
	for(i=0; i<obj->n_sec; i++)
	{
		/* .bss and similar. */
		scat=TKUCC_LinkGetSectionCategory(ctx, obj, i);
		if(scat!=3)
			continue;
	
		k=obj->sec[i]->ct_size;
		k=(k+1023)&(~1023);
		
		obj->sec[i]->ct_rva=trva;
		trva+=k;
	}

	rva_max=trva;

	TKUCC_LinkFlattenObjPEL_BuildBaseRelocs(ctx, obj);
	
	trva=rva_bss;

	for(i=0; i<obj->n_sec; i++)
	{
		/* sections that overlap .bss */
		scat=TKUCC_LinkGetSectionCategory(ctx, obj, i);
		if(scat!=4)
			continue;
	
		k=obj->sec[i]->ct_size;
		k=(k+1023)&(~1023);
		
		obj->sec[i]->ct_rva=trva;
		trva+=k;
	}

	rva_imgsz=trva;
	
	imgbuf=tkucc_malloc(rva_imgsz);
	memset(imgbuf, 0, rva_imgsz);

	for(i=0; i<obj->n_sec; i++)
	{
		/* Skip .bss and similar. */
		scat=TKUCC_LinkGetSectionCategory(ctx, obj, i);
		if(scat==0)
			continue;
		if(scat==3)
			continue;

		k=obj->sec[i]->ct_size;		
		trva=obj->sec[i]->ct_rva;
		TKUCC_ObjReadSectionBytes(ctx, obj, 0, i, imgbuf+trva, k);
	}
	
	addrbase=0x01100000;
	
	TKUCC_LinkApplyImageRelocs(ctx, obj,
		imgbuf, rva_imgsz, addrbase);
	
//	rva_start=0;
	rva_start=TKUCC_LinkLookupLabelRvaForName(ctx, obj, "_start");
	
	head=(TKUCC_PeMainHeader64 *)imgbuf;
	head->mMagicPe					=TKUCC_FCC_PE;
	head->mMachine					=0xB264;
	head->mNumberOfSections			=obj->n_sec;
	head->mTimeDateStamp			=0;
	head->mPointerToSymbolTable		=0;
	head->mNumberOfSymbols			=0;
	head->mSizeOfOptionalHeader		=240;
	head->mCharacteristics			=0;
	head->mMagic					=0x020B;
	head->mMajorLinkerVersion		=0;
	head->mMinorLinkerVersion		=0;
	head->mSizeOfCode				=rva_data-rva_base;
	head->mSizeOfInitializedData	=rva_bss-rva_data;
	head->mSizeOfUninitializedData	=rva_max-rva_bss;
	head->rvaAddressOfEntryPoint	=rva_start;
	head->mBaseOfCode				=rva_base;
	head->mImageBase				=addrbase;
	head->mSectionAlignment			=1024;
	head->mFileAlignment			=1024;

//	k=rva_max;
//	if(rva_imgsz>k)
//		k=rva_imgsz;

	k=rva_imgsz;
	head->mSizeOfImage				=k;
	head->mSizeOfHeaders			=0x108;

	head->mSizeOfStackReserve		=131072;
	head->mSizeOfStackCommit		=65536;

	for(i=0; i<obj->n_sec; i++)
	{
		sech=(TKUCC_PeSectionHeader *)(imgbuf+0x108+(i*40));

		scat=TKUCC_LinkGetSectionCategory(ctx, obj, i);

		s=TKUCC_LinkGetSectionName(ctx, obj, i);
		strncpy(sech->mName, s, 8);

		k=obj->sec[i]->ct_size;		
		trva=obj->sec[i]->ct_rva;
		
		if(scat==0)
		{
			k=0;
			trva=0;
		}
		
		sech->mVirtualSize=k;
		sech->mVirtualAddress=trva;
		sech->mSizeOfRawData=k;
		sech->mPointerToRawData=trva;

		sech->mPointerToRealocations=0;
		sech->mPointerToLinenumbers=0;
		sech->mNumberOfRealocations=0;
		sech->mNumberOfLinenumbers=0;
		sech->mCharacteristics=0;

		if(scat==1)
			sech->mCharacteristics=0x60000020;
		if(scat==2)
			sech->mCharacteristics=0xC0008040;
		if(scat==3)
			sech->mCharacteristics=0xC0008080;
		if(scat==4)
			sech->mCharacteristics=0x02000000;
	}

	trva=TKUCC_CalculateImagePel4BChecksum(imgbuf, rva_imgsz, 0);
	head->mCheckSum=trva;
	*rbuf=imgbuf;
	*rsize=rva_imgsz;
}
