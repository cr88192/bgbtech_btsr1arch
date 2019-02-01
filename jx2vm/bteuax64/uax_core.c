#ifdef UAX_LINUX
#include <sys/mman.h>
#endif

static int uax_init=0;
static int uax_fault=0;

int uax_opx86_nmidx[1024];

UAX_Context *uax_opctx_freelist=NULL;

byte *uax_exheap_start;
byte *uax_exheap_end;
byte *uax_exheap_pos;

char *uax_gblsym_name[4096];
void *uax_gblsym_addr[4096];
int uax_gblsym_num;

int UAX_Init()
{
#ifdef UAX_WINDOWS
	MEMORY_BASIC_INFORMATION memi;
#endif
	nlint a0, a1, a2;
	int ln;
	int i, j, k, l;

	if(uax_init)
		return(0);
	uax_init=1;
	
	/* Init lists to empty */
	for(i=0; i<1024; i++)
	{
		uax_opx86_nmidx[i]=-1;
	}
	
	/* Build Lists */
	ln=0;
	for(i=0; ; i++)
	{
		j=uax_opx86[i].id;
		if(!j)
		{
			if(!uax_opx86[i].opstr)
				break;
			
			uax_opx86[i].next=uax_opx86_nmidx[ln];
			uax_opx86[i].id=ln;
			uax_opx86_nmidx[ln]=i;
			continue;
		}
		
		ln=j;
		if(uax_opx86[i].opstr)
		{
			uax_opx86[i].next=uax_opx86_nmidx[ln];
			uax_opx86_nmidx[ln]=i;
		}
	}

	/* Reverse Lists */
	for(i=0; i<1024; i++)
	{
		j=uax_opx86_nmidx[i]; l=-1;
		while(j>=0)
		{
			k=uax_opx86[j].next;
			uax_opx86[j].next=l;
			l=j; j=k;
		}
		uax_opx86_nmidx[i]=l;
	}
	
	uax_gblsym_name[0]=NULL;
	uax_gblsym_addr[0]=NULL;
	uax_gblsym_num=1;
	
	uax_exheap_start=NULL;

#ifdef UAX_WINX64
	a0=(nlint)((void *)UAX_Init);
	for(i=-64; i<64; i++)
	{
		a1=(a0+(i<<24))&(~65535);
		a2=a1+(1<<24);
		if((a1>>40)!=(a2>>40))
			continue;
		
		VirtualQuery((PVOID)a1, &memi, 1<<24);
		if(memi.State&(MEM_COMMIT|MEM_RESERVE))
			continue;
		if(!(memi.State&MEM_FREE))
			continue;
		if(memi.RegionSize<(1<<24))
			continue;

		uax_exheap_start=VirtualAlloc((PVOID)a1, 1<<24,
			MEM_COMMIT|MEM_RESERVE|MEM_TOP_DOWN, PAGE_EXECUTE_READWRITE);
		if(uax_exheap_start)
			break;
	}
#endif
	
#ifdef UAX_WINDOWS
	if(!uax_exheap_start)
	{
		uax_exheap_start=VirtualAlloc(NULL, 1<<24,
			MEM_COMMIT|MEM_RESERVE|MEM_TOP_DOWN, PAGE_EXECUTE_READWRITE);
	}
#endif

#ifdef UAX_LINUX
	if(!uax_exheap_start)
	{
		uax_exheap_start=mmap(NULL, 1<<24,
			PROT_READ|PROT_WRITE|PROT_EXEC,
			MAP_PRIVATE|MAP_ANONYMOUS|MAP_32BIT, -1, 0);
	}
#endif

	if(!uax_exheap_start || (uax_exheap_start==((byte *)-1)))
	{
		printf("UAX_Init: Failed Alloc ExHeap\n");
		uax_fault=1;
		return(-1);
	}

	uax_exheap_end=uax_exheap_start+(1<<24);
	uax_exheap_pos=uax_exheap_start;
	
	i=UAX_CheckAddrExHeapRel32((void *)UAX_Init);
	
	printf("UAX_Init: ExHeap %p..%p, EXE~=%p, IR32=%d\n",
		uax_exheap_start, uax_exheap_end, (void *)UAX_Init, i);
	
	return(1);
}

int UAX_CheckAddrExHeapRel32(void *ptr)
{
	s64 d0, d1;
	
	d0=((byte *)ptr)-uax_exheap_start;
	d1=((byte *)ptr)-uax_exheap_end;
	
	if((d0!=((s32)d0)) || (d1!=((s32)d1)))
		return(0);
	return(1);
}

int UAX_LookupGlobalName(char *name)
{
	int i;
	
	if(!name)
		return(0);
	
	for(i=0; i<uax_gblsym_num; i++)
	{
		if(!uax_gblsym_name[i])
			continue;
		if(!strcmp(uax_gblsym_name[i], name))
			return(i);
	}
	
	return(0);
}

int UAX_LookupGlobalAddress(void *addr)
{
	int i;
	
	if(!addr)
		return(0);
	
	for(i=0; i<uax_gblsym_num; i++)
	{
//		if(!uax_gblsym_name[i])
//			continue;
		if(uax_gblsym_addr[i]==addr)
			return(i);
	}
	
	return(0);
}

int UAX_RegisterGlobal(char *name, void *addr)
{
	int i, j, k;

	if(name)
	{
		i=UAX_LookupGlobalName(name);
		if(i>0)
		{
			uax_gblsym_addr[i]=addr;
			return(i);
		}

		i=UAX_LookupGlobalAddress(addr);
		if((i>0) && !uax_gblsym_name[i])
		{
			uax_gblsym_name[i]=strdup(name);
			return(i);
		}
		
		i=uax_gblsym_num++;
		uax_gblsym_name[i]=strdup(name);
		uax_gblsym_addr[i]=addr;
		return(i);
	}else
	{
		i=UAX_LookupGlobalAddress(addr);
		if(i>0)
		{
			return(i);
		}

		i=uax_gblsym_num++;
		uax_gblsym_name[i]=NULL;
		uax_gblsym_addr[i]=addr;
		return(i);
	}
}

byte *UAX_ExHeapMalloc(int sz)
{
	byte *ptr;
	sz=(sz+63)&(~63);
	ptr=uax_exheap_pos;
	uax_exheap_pos=ptr+sz;
	if(uax_exheap_pos>=uax_exheap_end)
		return(NULL);
	return(ptr);
}

byte *UAX_ExHeapMAllocText(int sz)
{
	byte *ptr;
	sz=(sz+63)&(~63);
	ptr=uax_exheap_pos;
	uax_exheap_pos=ptr+sz;
	if(uax_exheap_pos>=uax_exheap_end)
		return(NULL);
	memset(ptr, 0xCC, sz);
	return(ptr);
}

byte *UAX_ExHeapMAllocData(int sz)
{
	byte *ptr;
	sz=(sz+63)&(~63);
	ptr=uax_exheap_pos;
	uax_exheap_pos=ptr+sz;
	if(uax_exheap_pos>=uax_exheap_end)
		return(NULL);
	memset(ptr, 0, sz);
	return(ptr);
}

int UAX_ExHeapResetMark()
{
	uax_exheap_pos=uax_exheap_start;
	return(0);
}

byte *UAX_LinkContext(UAX_Context *ctx)
{
	byte *gptr[16];
	int gsz[16];
	byte *prlc, *plbl;
	nlint nla;
	int lbl;
	int i, j, k;
	
	for(i=0; i<ctx->nsec; i++)
	{
		j=ctx->sec_pos[i]-ctx->sec_buf[i];
		if(j<=0)
			{ gptr[i]=NULL; gsz[i]=0; continue; }

		if(i==UAX_CSEG_TEXT)
		{
			gptr[i]=UAX_ExHeapMAllocText(j);
			gsz[i]=j;
		}else
		{
			gptr[i]=UAX_ExHeapMAllocData(j);
			gsz[i]=j;
		}
		
		if(!gptr[i])
			return(NULL);
		
		memcpy(gptr[i], ctx->sec_buf[i], gsz[i]);
	}
	
	for(i=0; i<ctx->nrlc; i++)
	{
		lbl=ctx->rlc_id[i];
		if(lbl>=UAX_LBL_LOCALSTART)
		{
			for(j=0; j<ctx->nlbl; j++)
				if(ctx->lbl_id[j]==lbl)
					break;
			if(j>=ctx->nlbl)
			{
				__debugbreak();
			}

			plbl=gptr[ctx->lbl_sec[j]]+ctx->lbl_ofs[j];
		}else if(lbl<uax_gblsym_num)
		{
			plbl=uax_gblsym_addr[lbl];
		}else
		{
			plbl=NULL;
		}
		
		if(!plbl)
		{
			__debugbreak();
		}
		
		nla=(nlint)plbl;
		
		prlc=gptr[ctx->rlc_sec[i]]+ctx->rlc_ofs[i];
//		plbl=gptr[ctx->lbl_sec[j]]+ctx->lbl_ofs[j];
		switch(ctx->rlc_ty[i])
		{
		case UAX_RLC_REL8:
			k=(plbl-prlc)-1;
			*(byte *)prlc+=k;
			break;
		case UAX_RLC_REL16:
			k=(plbl-prlc)-2;
			*(s16 *)prlc+=k;
			break;
		case UAX_RLC_REL32:
			k=(plbl-prlc)-4;
			*(s32 *)prlc+=k;
			break;

		case UAX_RLC_ABS8:
			*(byte *)prlc+=nla;
			break;
		case UAX_RLC_ABS16:
			*(s16 *)prlc+=nla;
			break;
		case UAX_RLC_ABS32:
			*(s32 *)prlc+=nla;
			break;
		case UAX_RLC_ABS64:
			*(s64 *)prlc+=nla;
			break;
		}
	}
	
	return(gptr[UAX_CSEG_TEXT]);
}

int UAX_DumpMemHex(byte *spos, byte *epos)
{
	byte *cs;
	int i, j, k;

	printf("%p..%p\n", spos, epos);

	spos=(byte *)(((nlint)spos)&(~15));
	epos=(byte *)((((nlint)epos)+15)&(~15));
	
	cs=spos;
	while(cs<epos)
	{
		printf("%08X", (int)((nlint)cs));
		for(i=0; i<16; i++)
		{
			if(!(i&3))
				fputc(' ', stdout);
			printf("%02X ", cs[i]);
		}

		for(i=0; i<16; i++)
		{
			j=cs[i];
			if((j<' ') || (j>'~'))
				j='.';
			fputc(j, stdout);
		}
		printf("\n");
		cs+=16;
	}
	return(0);
}

UAX_Context *UAX_AllocContext()
{
	UAX_Context *tmp;
	int i;
	
	UAX_Init();
	
	if(uax_opctx_freelist)
	{
		tmp=uax_opctx_freelist;
		uax_opctx_freelist=tmp->next;
		
		for(i=0; i<tmp->nsec; i++)
			{ tmp->sec_pos[i]=tmp->sec_buf[i]; }
		tmp->nlbl=0;
		tmp->nrlc=0;
		tmp->nsec=0;
		
		tmp->reg_live=0;
		tmp->reg_resv=0;
		tmp->reg_save=0;
		tmp->jitfl=0;
		tmp->lblrov=UAX_LBL_LOCALSTART;
		
		return(tmp);
	}
	
	tmp=malloc(sizeof(UAX_Context));
	memset(tmp, 0, sizeof(UAX_Context));
	tmp->lblrov=UAX_LBL_LOCALSTART;

	return(tmp);
}

int UAX_FreeContext(UAX_Context *ctx)
{
	ctx->next=uax_opctx_freelist;
	uax_opctx_freelist=ctx;
	return(0);
}

int UAX_SetSection(UAX_Context *ctx, int sec)
{
	ctx->sec=sec;
	if((sec+1)>ctx->nsec)
		ctx->nsec=sec+1;
	return(0);
}

int UAX_EmitByte(UAX_Context *ctx, int val)
{
	byte *buf;
	int sz, ofs;

	if(!ctx->sec_buf[ctx->sec])
	{
		sz=4096;	buf=malloc(sz);
		ctx->sec_buf[ctx->sec]=buf;
		ctx->sec_end[ctx->sec]=buf+sz;
		ctx->sec_pos[ctx->sec]=buf;
	}
	
	if((ctx->sec_pos[ctx->sec]+1)>=ctx->sec_end[ctx->sec])
	{
		buf=ctx->sec_buf[ctx->sec];
		sz=ctx->sec_end[ctx->sec]-buf;
		ofs=ctx->sec_pos[ctx->sec]-buf;
		sz=sz+(sz>>1);
		buf=realloc(buf, sz);
		ctx->sec_buf[ctx->sec]=buf;
		ctx->sec_end[ctx->sec]=buf+sz;
		ctx->sec_pos[ctx->sec]=buf+ofs;
	}

	*ctx->sec_pos[ctx->sec]++=val;
	return(0);
}

int UAX_EmitWord(UAX_Context *ctx, int val)
{
	UAX_EmitByte(ctx, (val   )&255);
	UAX_EmitByte(ctx, (val>>8)&255);
	return(0);
}

int UAX_EmitDWord(UAX_Context *ctx, s32 val)
{
	UAX_EmitByte(ctx, (val    )&255);
	UAX_EmitByte(ctx, (val>> 8)&255);
	UAX_EmitByte(ctx, (val>>16)&255);
	UAX_EmitByte(ctx, (val>>24)&255);
	return(0);
}

int UAX_EmitQWord(UAX_Context *ctx, s64 val)
{
	UAX_EmitByte(ctx, (val    )&255);
	UAX_EmitByte(ctx, (val>> 8)&255);
	UAX_EmitByte(ctx, (val>>16)&255);
	UAX_EmitByte(ctx, (val>>24)&255);
	UAX_EmitByte(ctx, (val>>32)&255);
	UAX_EmitByte(ctx, (val>>40)&255);
	UAX_EmitByte(ctx, (val>>48)&255);
	UAX_EmitByte(ctx, (val>>56)&255);
	return(0);
}

byte *UAX_EmitGetPos(UAX_Context *ctx)
{
	return(ctx->sec_pos[ctx->sec]);
}

int UAX_EmitGetOffs(UAX_Context *ctx)
{
	return(ctx->sec_pos[ctx->sec]-ctx->sec_buf[ctx->sec]);
}

int UAX_GenLabelTemp(UAX_Context *ctx)
{
	int i;
	i=ctx->lblrov++;
	return(i);
}

int UAX_EmitLabel(UAX_Context *ctx, int lblid)
{
	int i;
	
	if(lblid>0)
	{
		i=ctx->nlbl++;
		ctx->lbl_id[i]=lblid;
		ctx->lbl_ofs[i]=UAX_EmitGetOffs(ctx);
		ctx->lbl_sec[i]=ctx->sec;
//		ctx->lbl_ty[i]=ty;
	}
	return(0);
}


int UAX_EmitRelocTy(UAX_Context *ctx, int lblid, int ty)
{
	int i;
	
	if(lblid>0)
	{
		i=ctx->nrlc++;
		ctx->rlc_id[i]=lblid;
		ctx->rlc_ofs[i]=UAX_EmitGetOffs(ctx);
		ctx->rlc_sec[i]=ctx->sec;
		ctx->rlc_ty[i]=ty;
	}
	return(0);
}

int UAX_EmitRelocRel8(UAX_Context *ctx, int lbl)
	{ return(UAX_EmitRelocTy(ctx, lbl, UAX_RLC_REL8)); }
int UAX_EmitRelocRel16(UAX_Context *ctx, int lbl)
	{ return(UAX_EmitRelocTy(ctx, lbl, UAX_RLC_REL16)); }
int UAX_EmitRelocRel32(UAX_Context *ctx, int lbl)
	{ return(UAX_EmitRelocTy(ctx, lbl, UAX_RLC_REL32)); }

int UAX_EmitRelocAbs8(UAX_Context *ctx, int lbl)
	{ return(UAX_EmitRelocTy(ctx, lbl, UAX_RLC_ABS8)); }
int UAX_EmitRelocAbs16(UAX_Context *ctx, int lbl)
	{ return(UAX_EmitRelocTy(ctx, lbl, UAX_RLC_ABS16)); }
int UAX_EmitRelocAbs32(UAX_Context *ctx, int lbl)
	{ return(UAX_EmitRelocTy(ctx, lbl, UAX_RLC_ABS32)); }
int UAX_EmitRelocAbs64(UAX_Context *ctx, int lbl)
	{ return(UAX_EmitRelocTy(ctx, lbl, UAX_RLC_ABS64)); }

int UAX_Asm_StrHexVal(char *s)
{
	int x, y, z;
	
	x=-1; y=-1;
	if((s[0]>='0') && (s[0]<='9'))x=s[0]-'0';
	if((s[0]>='A') && (s[0]<='F'))x=s[0]-'A'+10;
	if((s[0]>='a') && (s[0]<='f'))x=s[0]-'a'+10;
	if((s[1]>='0') && (s[1]<='9'))y=s[1]-'0';
	if((s[1]>='A') && (s[1]<='F'))y=s[1]-'A'+10;
	if((s[1]>='a') && (s[1]<='f'))y=s[1]-'a'+10;
	z=(x*16)|y;
	return(z);
}

int UAX_Asm_OpArgRegP(UAX_OpcodeArg *rm)
{
	if(	(rm->sc) ||
		(rm->ireg!=UAX_REG_Z) ||
		(rm->breg==UAX_REG_Z) ||
		(rm->disp))
			return(0);
	return(1);
}

int UAX_Asm_OpArgMemP(UAX_OpcodeArg *rm)
{
	if(!rm->sc)
		return(0);
	return(1);
}

int UAX_Asm_OpArgMem8P(UAX_OpcodeArg *rm)
{
	if(!rm->sc)
		return(0);
	if((rm->sc&UAX_MEMSC_TMASK) &&
		((rm->sc&UAX_MEMSC_TMASK)!=UAX_MEMSC_BYTE))
			return(0);
	return(1);
}

int UAX_Asm_OpArgMem16P(UAX_OpcodeArg *rm)
{
	if(!rm->sc)
		return(0);
	if((rm->sc&UAX_MEMSC_TMASK) &&
		((rm->sc&UAX_MEMSC_TMASK)!=UAX_MEMSC_WORD))
			return(0);
	return(1);
}

int UAX_Asm_OpArgMem32P(UAX_OpcodeArg *rm)
{
	if(!rm->sc)
		return(0);
	if((rm->sc&UAX_MEMSC_TMASK) &&
		((rm->sc&UAX_MEMSC_TMASK)!=UAX_MEMSC_DWORD))
			return(0);
	return(1);
}

int UAX_Asm_OpArgMem64P(UAX_OpcodeArg *rm)
{
	if(!rm->sc)
		return(0);
	if((rm->sc&UAX_MEMSC_TMASK) &&
		((rm->sc&UAX_MEMSC_TMASK)!=UAX_MEMSC_QWORD))
			return(0);
	return(1);
}

int UAX_Asm_OpArgMem128P(UAX_OpcodeArg *rm)
{
	if(!rm->sc)
		return(0);
	if((rm->sc&UAX_MEMSC_TMASK) &&
		((rm->sc&UAX_MEMSC_TMASK)!=UAX_MEMSC_XMM))
			return(0);
	return(1);
}

int UAX_Asm_OpArgImmP(UAX_OpcodeArg *rm)
{
	if(	(rm->sc) ||
		(rm->ireg!=UAX_REG_Z) ||
		(rm->breg!=UAX_REG_Z))
			return(0);
	if(rm->lbl>0)
		return(0);
	return(1);
}

int UAX_Asm_OpArgLabelP(UAX_OpcodeArg *rm)
{
	if(	(rm->sc) ||
		(rm->ireg!=UAX_REG_Z) ||
		(rm->breg!=UAX_REG_Z))
			return(0);
	if(rm->lbl<=0)
		return(0);
	return(1);
}

int UAX_Asm_OpArgImm8P(UAX_OpcodeArg *rm)
{
	if(!UAX_Asm_OpArgImmP(rm))
		return(0);
//	if((rm->disp!=((byte)rm->disp)) &&
//		(rm->disp!=((sbyte)rm->disp)))
	if(rm->disp!=((sbyte)rm->disp))
			return(0);
	return(1);
}

int UAX_Asm_OpArgImm16P(UAX_OpcodeArg *rm)
{
	if(!UAX_Asm_OpArgImmP(rm))
		return(0);
//	if((rm->disp!=((u16)rm->disp)) &&
//		(rm->disp!=((s16)rm->disp)))
	if(rm->disp!=((s16)rm->disp))
			return(0);
	return(1);
}

int UAX_Asm_OpArgImm32P(UAX_OpcodeArg *rm)
{
	if(!UAX_Asm_OpArgImmP(rm))
		return(0);
//	if((rm->disp!=((u32)rm->disp)) &&
//		(rm->disp!=((s32)rm->disp)))
	if(rm->disp!=((s32)rm->disp))
			return(0);
	return(1);
}


int UAX_Asm_OpArgImmU8P(UAX_OpcodeArg *rm)
{
	if(!UAX_Asm_OpArgImmP(rm))
		return(0);
	if(rm->disp!=((byte)rm->disp))
			return(0);
	return(1);
}

int UAX_Asm_OpArgImmU16P(UAX_OpcodeArg *rm)
{
	if(!UAX_Asm_OpArgImmP(rm))
		return(0);
	if(rm->disp!=((u16)rm->disp))
			return(0);
	return(1);
}

int UAX_Asm_OpArgImmU32P(UAX_OpcodeArg *rm)
{
	if(!UAX_Asm_OpArgImmP(rm))
		return(0);
	if(rm->disp!=((u32)rm->disp))
			return(0);
	return(1);
}


int UAX_Asm_OpArgLdRegP(UAX_OpcodeArg *rm)
{
	if(	(!rm->sc) ||
		(rm->disp!=0))
			return(0);
	return(1);
}

int UAX_Asm_OpArgLdRegDisp8P(UAX_OpcodeArg *rm)
{
	if(	(!rm->sc) ||
		(rm->disp!=((sbyte)rm->disp)))
			return(0);
	return(1);
}

int UAX_Asm_OpArgLdRegDisp32P(UAX_OpcodeArg *rm)
{
	if(	(!rm->sc) ||
		(rm->disp!=((s32)(rm->disp))))
			return(0);
	return(1);
}

int UAX_Asm_OpArgBaseLdRegP(UAX_OpcodeArg *rm)
{
	if(	(!UAX_Asm_OpArgLdRegP(rm)) ||
		(rm->sc!=1) ||
		(rm->ireg!=UAX_REG_Z) ||
		(rm->breg==UAX_REG_Z) ||
		((rm->breg&7)==4) ||
		((rm->breg&7)==5))
			return(0);
	return(1);
}

int UAX_Asm_OpArgBaseLdRegDisp8P(UAX_OpcodeArg *rm)
{
	if(	(!UAX_Asm_OpArgLdRegDisp8P(rm)) ||
		(rm->sc!=1) ||
		(rm->ireg!=UAX_REG_Z) ||
		(rm->breg==UAX_REG_Z) ||
		((rm->breg&7)==4))
			return(0);
	return(1);
}

int UAX_Asm_OpArgBaseLdRegDisp32P(UAX_OpcodeArg *rm)
{
	if(	(!UAX_Asm_OpArgLdRegDisp32P(rm)) ||
		(rm->sc!=1) ||
		(rm->ireg!=UAX_REG_Z) ||
		(rm->breg==UAX_REG_Z) ||
		((rm->breg&7)==4))
			return(0);
	return(1);
}

int UAX_Asm_OpArgBaseLdRipDisp32P(UAX_OpcodeArg *rm)
{
	if(	(rm->breg!=UAX_REG_RIP) ||
		(rm->sc!=1) ||
		(rm->breg==UAX_REG_Z) ||
		(rm->ireg!=UAX_REG_Z))
			return(0);
	return(1);
}

int UAX_Asm_OpArgAbsLdDisp32P(UAX_OpcodeArg *rm)
{
	if(	(rm->breg!=UAX_REG_Z) ||
		(rm->sc!=1) ||
		(rm->ireg!=UAX_REG_Z))
			return(0);
	return(1);
}

int UAX_Asm_RegIsDWordP(int reg)
{
	if((reg&0xF0)==0x00)
		return(1);
	return(0);
}

int UAX_Asm_RegIsQWordP(int reg)
{
	if((reg&0xF0)==0x10)
		return(1);
	return(0);
}

int UAX_Asm_RegIsWordP(int reg)
{
	if((reg&0xF0)==0x20)
		return(1);
	return(0);
}

int UAX_Asm_RegIsByteP(int reg)
{
	if(((reg&0xF0)==0x30) ||
		((reg>=UAX_REG_AH) && (reg<=UAX_REG_BH)))
			return(1);
	return(0);
}

int UAX_Asm_RegIsExtByteP(int reg)
{
	if(((reg&0xF0)==0x30) && ((reg&0x0F)>=4))
		return(1);
	return(0);
}

int UAX_Asm_RegIsXmmP(int reg)
{
	if((reg&0xF0)==0x40)
		return(1);
	return(0);
}

int UAX_Asm_RegTypeEquivP(int reg0, int reg1)
{
	if((reg0&0xF0)==(reg1&0xF0))
		return(1);
	return(0);
}

int UAX_Asm_RegAsDWord(int reg)
{
	if(reg==UAX_REG_Z)
		return(UAX_REG_Z);
	return(reg&15);
}

int UAX_Asm_OpArgRegDWordP(UAX_OpcodeArg *rm)
{
	if(!UAX_Asm_OpArgRegP(rm))
		return(0);
	if(!UAX_Asm_RegIsDWordP(rm->breg))
		return(0);
	return(1);
}

int UAX_Asm_OpArgRegQWordP(UAX_OpcodeArg *rm)
{
	if(!UAX_Asm_OpArgRegP(rm))
		return(0);
	if(!UAX_Asm_RegIsQWordP(rm->breg))
		return(0);
	return(1);
}

int UAX_Asm_OpArgRegWordP(UAX_OpcodeArg *rm)
{
	if(!UAX_Asm_OpArgRegP(rm))
		return(0);
	if(!UAX_Asm_RegIsWordP(rm->breg))
		return(0);
	return(1);
}

int UAX_Asm_OpArgRegByteP(UAX_OpcodeArg *rm)
{
	if(!UAX_Asm_OpArgRegP(rm))
		return(0);
	if(!UAX_Asm_RegIsByteP(rm->breg))
		return(0);
	return(1);
}

int UAX_Asm_OpArgRegXmmP(UAX_OpcodeArg *rm)
{
	if(!UAX_Asm_OpArgRegP(rm))
		return(0);
	if(!UAX_Asm_RegIsXmmP(rm->breg))
		return(0);
	return(1);
}


int UAX_Asm_OpArgLabelDisp8P(UAX_OpcodeArg *rm)
{
	if(!UAX_Asm_OpArgLabelP(rm))
		return(0);
	if(!(rm->lbl&UAX_LBL_NEAR))
		return(0);
	return(1);

//	return(0);
//	if(!UAX_Asm_OpArgRegP(rm))
//		return(0);
//	if(!UAX_Asm_RegIsByteP(rm->breg))
//		return(0);
//	return(1);
}

int UAX_Asm_OpArgLabelDisp16P(UAX_OpcodeArg *rm)
{
	return(0);
//	if(!UAX_Asm_OpArgRegP(rm))
//		return(0);
//	if(!UAX_Asm_RegIsByteP(rm->breg))
//		return(0);
//	return(1);
}

int UAX_Asm_OpArgLabelDisp32P(UAX_OpcodeArg *rm)
{
	if(!UAX_Asm_OpArgLabelP(rm))
		return(0);
//	if(!UAX_Asm_RegIsByteP(rm->breg))
//		return(0);
	return(1);
}

int UAX_EmitRegRM_Sib(UAX_Context *ctx, UAX_OpcodeArg *rm)
{
	if(rm->breg!=UAX_REG_Z)
	{
		if(rm->ireg!=UAX_REG_Z)
		{
			UAX_EmitByte(ctx,
				(((rm->sc-1)&3)<<6) |
				((rm->ireg&7)<<3) |
				(rm->breg&7));
		}else
		{
			UAX_EmitByte(ctx,
//				(((rm->sc-1)&3)<<6) |
				(4<<3) |
				(rm->breg&7));
		}
	}else
	{
		if(rm->ireg!=UAX_REG_Z)
		{
			UAX_EmitByte(ctx,
				(((rm->sc-1)&3)<<6) |
				((rm->ireg&7)<<3) |
				5);
		}else
		{
			UAX_EmitByte(ctx,
//				(((rm->sc-1)&3)<<6) |
				(4<<3) | 5);
		}
	}

	return(0);
}

int UAX_EmitRegRM(UAX_Context *ctx, int reg, UAX_OpcodeArg *rm)
{
	if(UAX_Asm_OpArgRegP(rm))
	{
		UAX_EmitByte(ctx, 0xC0|((reg&7)<<3)|(rm->breg&7));
		return(0);
	}
	if(UAX_Asm_OpArgBaseLdRegP(rm))
	{
		UAX_EmitByte(ctx, 0x00|((reg&7)<<3)|(rm->breg&7));
		return(0);
	}
	if(UAX_Asm_OpArgBaseLdRegDisp8P(rm))
	{
		UAX_EmitByte(ctx, 0x40|((reg&7)<<3)|(rm->breg&7));
		UAX_EmitByte(ctx, rm->disp);
		return(0);
	}
	if(UAX_Asm_OpArgBaseLdRegDisp32P(rm))
	{
		UAX_EmitByte(ctx, 0x80|((reg&7)<<3)|(rm->breg&7));
		UAX_EmitDWord(ctx, rm->disp);
		return(0);
	}
	if(UAX_Asm_OpArgBaseLdRipDisp32P(rm))
	{
		UAX_EmitByte(ctx, 0x05|((reg&7)<<3));
		UAX_EmitDWord(ctx, rm->disp);
		return(0);
	}

	if(UAX_Asm_OpArgLdRegP(rm))
	{
		UAX_EmitByte(ctx, 0x04|((reg&7)<<3));
		UAX_EmitRegRM_Sib(ctx, rm);
		return(0);
	}
	if(UAX_Asm_OpArgLdRegDisp8P(rm))
	{
		UAX_EmitByte(ctx, 0x44|((reg&7)<<3));
		UAX_EmitRegRM_Sib(ctx, rm);
		UAX_EmitByte(ctx, rm->disp);
		return(0);
	}
	if(UAX_Asm_OpArgLdRegDisp32P(rm))
	{
		UAX_EmitByte(ctx, 0x84|((reg&7)<<3));
		UAX_EmitRegRM_Sib(ctx, rm);
		UAX_EmitDWord(ctx, rm->disp);
		return(0);
	}
	
	if(UAX_Asm_OpArgAbsLdDisp32P(rm))
	{
		UAX_EmitByte(ctx, 0x04|((reg&7)<<3));
		UAX_EmitByte(ctx, 0x25);
		UAX_EmitDWord(ctx, rm->disp);
		return(0);
	}
	
	__debugbreak();
	return(-1);
}

int UAX_EmitOpString(UAX_Context *ctx, char *str,
	UAX_OpcodeArg *arg0, UAX_OpcodeArg *arg1, UAX_OpcodeArg *arg2)
{
	UAX_OpcodeArg *targa[6];
	UAX_OpcodeArg *targ0, *targ1;
	UAX_OpcodeArg **taa;
	byte *p;
	char *s, *s1;
	int i, j, k;
	
	targa[0]=arg0;
	targa[1]=arg1;
	targa[2]=arg2;
	targa[3]=NULL;
	targa[4]=NULL;
	targa[5]=NULL;
	
	s=str; taa=targa;
	while(*s)
	{
		i=UAX_Asm_StrHexVal(s);
		if(i>=0)
		{
			UAX_EmitByte(ctx, i);
			s+=2;
			continue;
		}
		
		targ0=taa[0];
		targ1=taa[1];
		
		if(s[0]=='|')
		{
			if(s[1]=='r')
			{
				s+=2; taa++;
				p=UAX_EmitGetPos(ctx);
				*(p-1)|=targ0->breg&7;
				continue;
			}
			__debugbreak();
		}

		if(s[0]=='/')
		{
			if(s[1]=='r')
			{
				s+=2; taa+=2;
				UAX_EmitRegRM(ctx, targ0->breg, targ1);
				continue;
			}

			if(s[1]=='R')
			{
				s+=2; taa+=2;
				UAX_EmitRegRM(ctx, targ1->breg, targ0);
				continue;
			}

			if((s[1]>='0') && (s[1]<='7'))
			{
				UAX_EmitRegRM(ctx, s[1]-'0', targ0);
				s+=2; taa++;
				continue;
			}

			if(s[1]=='q')
			{
				s+=2; taa++;
				UAX_EmitRegRM(ctx, targ0->breg, targ0);
				continue;
			}

			__debugbreak();
		}
		
		if(*s=='X')
		{
			s++;
			targ0=taa[0];		targ1=taa[1];		s1=s;
			while(*s1)
			{
				if((s1[0]=='/') && (s1[1]=='r'))
					{ targ0=taa[0]; targ1=taa[1]; break; }
				if((s1[0]=='/') && (s1[1]=='R'))
					{ targ0=taa[1]; targ1=taa[0]; break; }
				if((s1[0]=='/') && (s1[1]>='0') && (s1[1]<='7'))
					{ targ0=NULL; targ1=taa[0]; break; }
				if((s1[0]=='|') && (s1[1]=='r'))
					{ targ0=NULL; targ1=taa[0]; break; }
				s1++;
			}
			
			i=0x40;
//			if(targ0 && UAX_Asm_RegIsQWordP(targ0->breg))		i|=8;
			if(targ0 && UAX_Asm_OpArgRegQWordP(targ0))		i|=8;
			if(targ1 && UAX_Asm_OpArgRegQWordP(targ1))		i|=8;
			if(targ0 && (targ0->breg&8) && (targ0->breg!=UAX_REG_Z))	i|=4;
			if(targ1 && (targ1->ireg&8) && (targ1->ireg!=UAX_REG_Z))	i|=2;
			if(targ1 && (targ1->breg&8) && (targ1->breg!=UAX_REG_Z))	i|=1;
			if(targ0 && UAX_Asm_RegIsExtByteP(targ0->breg))	i|=0x100;
			if(targ1 && UAX_Asm_RegIsExtByteP(targ1->breg))	i|=0x100;
			
			if(i!=0x40)
				{ UAX_EmitByte(ctx, i&0xFF); }
			continue;
		}

		if(s[0]==',')
		{
			if(s[1]=='i')
			{
				if(s[2]=='b')
					{ s+=3; taa++; UAX_EmitByte(ctx, targ0->disp); continue; }
				if(s[2]=='w')
					{ s+=3; taa++; UAX_EmitWord(ctx, targ0->disp); continue; }
				if(s[2]=='d')
					{ s+=3; taa++; UAX_EmitDWord(ctx, targ0->disp); continue; }
				if(s[2]=='q')
					{ s+=3; taa++; UAX_EmitQWord(ctx, targ0->disp); continue; }
				__debugbreak();
			}

			if(s[1]=='r')
			{
				if(s[2]=='b')
				{	UAX_EmitRelocRel8(ctx, targ0->lbl);
					UAX_EmitByte(ctx, targ0->disp);
					s+=3; taa++; continue;	}
				if(s[2]=='w')
				{	UAX_EmitRelocRel16(ctx, targ0->lbl);
					UAX_EmitWord(ctx, targ0->disp);
					s+=3; taa++; continue;	}
				if(s[2]=='d')
				{	UAX_EmitRelocRel32(ctx, targ0->lbl);
					UAX_EmitDWord(ctx, targ0->disp);
					s+=3; taa++; continue;	}
				__debugbreak();
			}

			if(s[1]=='m')
			{
				if(s[2]=='b')
				{	UAX_EmitRelocAbs8(ctx, targ0->lbl);
					UAX_EmitByte(ctx, targ0->disp);
					s+=3; taa++; continue;	}
				if(s[2]=='w')
				{	UAX_EmitRelocAbs16(ctx, targ0->lbl);
					UAX_EmitWord(ctx, targ0->disp);
					s+=3; taa++; continue;	}
				if(s[2]=='d')
				{	UAX_EmitRelocAbs32(ctx, targ0->lbl);
					UAX_EmitDWord(ctx, targ0->disp);
					s+=3; taa++; continue;	}
				if(s[2]=='q')
				{	UAX_EmitRelocAbs64(ctx, targ0->lbl);
					UAX_EmitQWord(ctx, targ0->disp);
					s+=3; taa++; continue;	}
				__debugbreak();
			}
		}

		if(*s=='V')
			{ UAX_EmitByte(ctx, 0x67); s++; continue; }
		if(*s=='W')
			{ UAX_EmitByte(ctx, 0x66); s++; continue; }
		if(*s=='S')
			{ s++; continue; }
		if(*s=='T')
			{ s++; continue; }
	}

	return(0);
}

int UAX_CheckArgsMatchString(UAX_Context *ctx, char *str,
	UAX_OpcodeArg *arg0, UAX_OpcodeArg *arg1, UAX_OpcodeArg *arg2)
{
	UAX_OpcodeArg *taa[4], **ta;
	char *s, *s1;
	int i, j, k;
	
	if(!str || !(*str))
	{
		if(!arg0)
			return(1);
		return(0);
	}
	
	taa[0]=arg0;
	taa[1]=arg1;
	taa[2]=arg2;
	taa[3]=NULL;
	
	s=str; ta=taa;
	while(*s && *ta)
	{
		if(*s==',')
			{ s++; continue; }

		if(s[0]=='r')
		{
			if(s[1]=='d')
			{	if(!UAX_Asm_OpArgRegDWordP(*ta))
					break;
				s+=2; ta++; continue;	}
			if(s[1]=='q')
			{	if(!UAX_Asm_OpArgRegQWordP(*ta))
					break;
				s+=2; ta++; continue;	}
			if(s[1]=='w')
			{	if(!UAX_Asm_OpArgRegWordP(*ta))
					break;
				s+=2; ta++; continue;	}

			if((s[1]=='b') || (s[1]=='8'))
			{
				if(!UAX_Asm_OpArgRegByteP(*ta))
					break;
				s+=2; ta++; continue;
			}

			if((s[1]=='3') && (s[2]=='2'))
			{	if(!UAX_Asm_OpArgRegDWordP(*ta))
					break;
				s+=3; ta++; continue;	}
			if((s[1]=='6') && (s[2]=='4'))
			{	if(!UAX_Asm_OpArgRegQWordP(*ta))
					break;
				s+=3; ta++; continue;	}
			if((s[1]=='1') && (s[2]=='6'))
			{	if(!UAX_Asm_OpArgRegWordP(*ta))
					break;
				s+=3; ta++; continue;	}

			if(s[1]=='m')
			{
				if(s[2]=='8')
				{
					if(!UAX_Asm_OpArgRegByteP(*ta) &&
						!UAX_Asm_OpArgMem8P(*ta))
							break;
					s+=3; ta++; continue;
				}

				if((s[2]=='3') && (s[3]=='2'))
				{	if(!UAX_Asm_OpArgRegDWordP(*ta) &&
						!UAX_Asm_OpArgMem32P(*ta))
							break;
					s+=4; ta++; continue;	}
				if((s[2]=='6') && (s[3]=='4'))
				{	if(!UAX_Asm_OpArgRegQWordP(*ta) &&
						!UAX_Asm_OpArgMem64P(*ta))
							break;
					s+=4; ta++; continue;	}
				if((s[2]=='1') && (s[3]=='6'))
				{	if(!UAX_Asm_OpArgRegWordP(*ta) &&
						!UAX_Asm_OpArgMem16P(*ta))
							break;
					s+=4; ta++; continue;	}
			}

			if((s[1]=='a') && (s[2]=='x'))
			{
				if(!UAX_Asm_OpArgRegQWordP(*ta))
					break;
				if((*ta)->breg!=UAX_REG_RAX)
					break;
				s+=3; ta++; continue;
			}

			if((s[1]=='c') && (s[2]=='x'))
			{
				if(!UAX_Asm_OpArgRegQWordP(*ta))
					break;
				if((*ta)->breg!=UAX_REG_RCX)
					break;
				s+=3; ta++; continue;
			}

			__debugbreak();
		}

		if(s[0]=='m')
		{
			if(!UAX_Asm_OpArgMemP(*ta))
				break;
//			if(s[1]=='8') { s++; }
//			else if(!strncmp(s+1, "16", 2)) { s+=2; }
//			else if(!strncmp(s+1, "32", 2)) { s+=2; }
//			else if(!strncmp(s+1, "64", 2)) { s+=2; }

			if(s[1]=='8')
				{ s++; if(!UAX_Asm_OpArgMem8P(*ta))break; }
			else if(!strncmp(s+1, "16", 2))
				{ s+=2; if(!UAX_Asm_OpArgMem16P(*ta))break; }
			else if(!strncmp(s+1, "32", 2))
				{ s+=2; if(!UAX_Asm_OpArgMem32P(*ta))break; }
			else if(!strncmp(s+1, "64", 2))
				{ s+=2; if(!UAX_Asm_OpArgMem64P(*ta))break; }

			s++; ta++;
			continue;
		}
		
		if(s[0]=='i')
		{
			if((s[1]=='b') || (s[1]=='8'))
			{	if(!UAX_Asm_OpArgImm8P(*ta))
					break;
				s+=2; ta++; continue;	}
			if(s[1]=='w')
			{	if(!UAX_Asm_OpArgImm16P(*ta))
					break;
				s+=2; ta++; continue;	}
			if(s[1]=='d')
			{	if(!UAX_Asm_OpArgImm32P(*ta))
					break;
				s+=2; ta++; continue;	}
			if(s[1]=='q')
			{	if(!UAX_Asm_OpArgImmP(*ta))
					break;
				s+=2; ta++; continue;	}

			if((s[1]=='1') && (s[2]=='6'))
			{	if(!UAX_Asm_OpArgImm16P(*ta))
					break;
				s+=3; ta++; continue;	}
			if((s[1]=='3') && (s[2]=='2'))
			{	if(!UAX_Asm_OpArgImm32P(*ta))
					break;
				s+=3; ta++; continue;	}
			if((s[1]=='6') && (s[2]=='4'))
			{	if(!UAX_Asm_OpArgImmP(*ta))
					break;
				s+=3; ta++; continue;	}

			if(!UAX_Asm_OpArgImmP(*ta))
				break;
			s++; ta++; continue;
		}

		if(s[0]=='u')
		{
			if((s[1]=='b') || (s[1]=='8'))
			{	if(!UAX_Asm_OpArgImmU8P(*ta))
					break;
				s+=2; ta++; continue;	}
			if(s[1]=='w')
			{	if(!UAX_Asm_OpArgImmU16P(*ta))
					break;
				s+=2; ta++; continue;	}
			if(s[1]=='d')
			{	if(!UAX_Asm_OpArgImmU32P(*ta))
					break;
				s+=2; ta++; continue;	}
			if(s[1]=='q')
			{	if(!UAX_Asm_OpArgImmP(*ta))
					break;
				s+=2; ta++; continue;	}

			if((s[1]=='1') && (s[2]=='6'))
			{	if(!UAX_Asm_OpArgImmU16P(*ta))
					break;
				s+=3; ta++; continue;	}
			if((s[1]=='3') && (s[2]=='2'))
			{	if(!UAX_Asm_OpArgImmU32P(*ta))
					break;
				s+=3; ta++; continue;	}
			if((s[1]=='6') && (s[2]=='4'))
			{	if(!UAX_Asm_OpArgImmP(*ta))
					break;
				s+=3; ta++; continue;	}

			if(!UAX_Asm_OpArgImmP(*ta))
				break;
			s++; ta++; continue;
		}

		if(s[0]=='e')
		{
			if((s[1]=='a') && (s[2]=='x'))
			{
				if(!UAX_Asm_OpArgRegDWordP(*ta))
					break;
				if((*ta)->breg!=UAX_REG_EAX)
					break;
				s+=3; ta++; continue;
			}

			if((s[1]=='c') && (s[2]=='x'))
			{
				if(!UAX_Asm_OpArgRegDWordP(*ta))
					break;
				if((*ta)->breg!=UAX_REG_ECX)
					break;
				s+=3; ta++; continue;
			}
		}

		if(s[0]=='a')
		{
			if(s[1]=='l')
			{
				if(!UAX_Asm_OpArgRegByteP(*ta))
					break;
				if((*ta)->breg!=UAX_REG_AL)
					break;
				s+=2; ta++; continue;
			}

			if(s[1]=='x')
			{
				if(!UAX_Asm_OpArgRegWordP(*ta))
					break;
				if((*ta)->breg!=UAX_REG_AX)
					break;
				s+=2; ta++; continue;
			}

			if(s[1]=='r')
			{
				if(s[2]=='8')
				{
					if(!UAX_Asm_OpArgImm8P(*ta) &&
							!UAX_Asm_OpArgLabelDisp8P(*ta))
						break;
					s+=3; ta++; continue;
				}

				if((s[2]=='1') && (s[3]=='6'))
				{
					if(!UAX_Asm_OpArgImm16P(*ta) &&
							!UAX_Asm_OpArgLabelDisp16P(*ta))
						break;
					s+=4; ta++; continue;
				}

				if((s[2]=='3') && (s[3]=='2'))
				{
					if(!UAX_Asm_OpArgImm32P(*ta) &&
							!UAX_Asm_OpArgLabelDisp32P(*ta))
						break;
					s+=4; ta++; continue;
				}
			}

			__debugbreak();
		}

		if(s[0]=='x')
		{
			if(s[1]=='r')
			{
				if(s[2]=='m')
				{
					if(!UAX_Asm_OpArgRegXmmP(*ta) &&
						!UAX_Asm_OpArgMem128P(*ta))
							break;
					s+=3; ta++; continue;
				}
			
				if(!UAX_Asm_OpArgRegXmmP(*ta))
					break;

				if(s[2]=='2')s++;
				s+=2; ta++; continue;
			}
		}

		if(s[0]=='c')
		{
			if(s[1]=='l')
			{
				if(!UAX_Asm_OpArgRegByteP(*ta))
					break;
				if((*ta)->breg!=UAX_REG_CL)
					break;
				s+=2; ta++; continue;
			}

			if(s[1]=='x')
			{
				if(!UAX_Asm_OpArgRegWordP(*ta))
					break;
				if((*ta)->breg!=UAX_REG_CX)
					break;
				s+=2; ta++; continue;
			}

			__debugbreak();
		}

		if(s[0]=='f')
		{
			if(s[1]=='r')
			{
				break;
			}
		}

		if(s[0]=='u')
		{
			if((s[1]=='b') || (s[1]=='8'))
			{	if(!UAX_Asm_OpArgImm8P(*ta))
					break;
				s+=2; ta++; continue;	}
		}

		if((s[0]>='0') && (s[0]<='9'))
		{
			if(!UAX_Asm_OpArgImmP(*ta))
				break;
			j=0; s1=s;
			while((*s1>='0') && (*s1<='9'))
				j=(j*10)+((*s1++)-'0');
			if((*ta)->disp!=j)
				break;
			s=s1; ta++; continue;
		}

		__debugbreak();
	}
	
	if(*s || *ta)
		return(0);
	return(1);
}

int UAX_FindMatchInsnId(UAX_Context *ctx, int nmid,
	UAX_OpcodeArg *arg0, UAX_OpcodeArg *arg1, UAX_OpcodeArg *arg2)
{
	int i, j, k;
	
	i=uax_opx86_nmidx[nmid];
	while(i>=0)
	{
		if(UAX_CheckArgsMatchString(ctx, uax_opx86[i].args,
			arg0, arg1, arg2)>0)
				return(i);
		i=uax_opx86[i].next;
	}
	return(-1);
}

int UAX_AssembleInsn(UAX_Context *ctx, int nmid,
	UAX_OpcodeArg *arg0, UAX_OpcodeArg *arg1, UAX_OpcodeArg *arg2)
{
	int i, j, k;

	i=UAX_FindMatchInsnId(ctx, nmid, arg0, arg1, arg2);
	if(i<0)
	{
		__debugbreak();
		return(-1);
	}

#if 0
	if(arg1 && (((s32)arg1->disp)==0x89ABCDEF))
	{
		printf("OP %s ", uax_opx86[i].opstr);
		if(arg0)
		{
			printf("%016llX ", arg0->disp);
		}
		if(arg1)
		{
			printf("%016llX ", arg1->disp);
		}
		if(arg2)
		{
			printf("%016llX ", arg2->disp);
		}
		printf("\n");
	}
#endif
	
	UAX_EmitOpString(ctx, uax_opx86[i].opstr, arg0, arg1, arg2);
	return(0);
}

int UAX_OpArgSetupReg(UAX_OpcodeArg *rm, int reg)
{
	rm->sc=0;
	rm->ireg=UAX_REG_Z;
	rm->breg=reg;
	rm->disp=0;
	rm->lbl=0;
	return(0);
}

int UAX_OpArgSetupImm(UAX_OpcodeArg *rm, s64 disp)
{
	rm->sc=0;
	rm->ireg=UAX_REG_Z;
	rm->breg=UAX_REG_Z;
	rm->disp=disp;
	rm->lbl=0;
	return(0);
}

int UAX_OpArgSetupLabel(UAX_OpcodeArg *rm, int lblid)
{
	rm->sc=0;
	rm->ireg=UAX_REG_Z;
	rm->breg=UAX_REG_Z;
	rm->disp=0;
	rm->lbl=lblid;
	return(0);
}

int UAX_OpArgSetupLdRegDisp(UAX_OpcodeArg *rm, int breg, s32 disp)
{
	rm->sc=1;
	rm->ireg=UAX_REG_Z;
	rm->breg=breg;
	rm->disp=disp;
	rm->lbl=0;
	return(0);
}

int UAX_OpArgSetupLdRegIxDisp(UAX_OpcodeArg *rm,
	int breg, int sc, int ireg, s32 disp)
{
	int sc1;
	
	switch(sc)
	{
	case 1: sc1=1; break;
	case 2: sc1=2; break;
	case 4: sc1=3; break;
	case 8: sc1=4; break;
	default: sc1=1; break;
	}
	
	rm->sc=sc1;
	rm->ireg=ireg;
	rm->breg=breg;
	rm->disp=disp;
	rm->lbl=0;
	return(0);
}

int UAX_AsmInsnNone(UAX_Context *ctx, int nmid)
{
	int i;
	i=UAX_AssembleInsn(ctx, nmid, NULL, NULL, NULL);
	return(i);
}

int UAX_AsmInsnReg(UAX_Context *ctx, int nmid, int dreg)
{
	UAX_OpcodeArg ta0, ta1;
	int i;
	UAX_OpArgSetupReg(&ta0, dreg);
	i=UAX_AssembleInsn(ctx, nmid, &ta0, NULL, NULL);
	return(i);
}

int UAX_AsmInsnImm(UAX_Context *ctx, int nmid, s64 disp)
{
	UAX_OpcodeArg ta0, ta1;
	int i;
	UAX_OpArgSetupImm(&ta0, disp);
	i=UAX_AssembleInsn(ctx, nmid, &ta0, NULL, NULL);
	return(i);
}

int UAX_AsmInsnLabel(UAX_Context *ctx, int nmid, int lblid)
{
	UAX_OpcodeArg ta0, ta1;
	int i;
	UAX_OpArgSetupLabel(&ta0, lblid);
	i=UAX_AssembleInsn(ctx, nmid, &ta0, NULL, NULL);
	return(i);
}

int UAX_AsmInsnRegReg(UAX_Context *ctx, int nmid, int dreg, int sreg)
{
	UAX_OpcodeArg ta0, ta1;
	int i;
	UAX_OpArgSetupReg(&ta0, dreg);
	UAX_OpArgSetupReg(&ta1, sreg);
	i=UAX_AssembleInsn(ctx, nmid, &ta0, &ta1, NULL);
	return(i);
}

int UAX_AsmInsnRegImm(UAX_Context *ctx, int nmid, int dreg, s64 disp)
{
	UAX_OpcodeArg ta0, ta1;
	int i;
	UAX_OpArgSetupReg(&ta0, dreg);
	UAX_OpArgSetupImm(&ta1, disp);
	i=UAX_AssembleInsn(ctx, nmid, &ta0, &ta1, NULL);
	return(i);
}

int UAX_AsmInsnRegImm32(UAX_Context *ctx, int nmid, int dreg, s32 disp)
{
	return(UAX_AsmInsnRegImm(ctx, nmid, dreg, disp));
}

int UAX_AsmInsnStRegDisp32(UAX_Context *ctx, int nmid,
	int dreg, s32 disp)
{
	UAX_OpcodeArg ta0, ta1;
	int i;
	UAX_OpArgSetupLdRegDisp(&ta0, dreg, disp);
	ta0.sc|=UAX_MEMSC_DWORD;
	i=UAX_AssembleInsn(ctx, nmid, &ta0, NULL, NULL);
	return(i);
}


int UAX_AsmInsnRegLdRegDisp(UAX_Context *ctx, int nmid,
	int dreg, int sreg, s32 disp)
{
	UAX_OpcodeArg ta0, ta1;
	int i;
	UAX_OpArgSetupReg(&ta0, dreg);
	UAX_OpArgSetupLdRegDisp(&ta1, sreg, disp);
	i=UAX_AssembleInsn(ctx, nmid, &ta0, &ta1, NULL);
	return(i);
}

int UAX_AsmInsnStRegDispReg(UAX_Context *ctx, int nmid,
	int dreg, s32 disp, int sreg)
{
	UAX_OpcodeArg ta0, ta1;
	int i;
	UAX_OpArgSetupLdRegDisp(&ta0, dreg, disp);
	UAX_OpArgSetupReg(&ta1, sreg);
	i=UAX_AssembleInsn(ctx, nmid, &ta0, &ta1, NULL);
	return(i);
}

int UAX_AsmInsnStRegDispImm(UAX_Context *ctx, int nmid,
	int dreg, s32 disp, s64 imm)
{
	UAX_OpcodeArg ta0, ta1;
	int i;
	UAX_OpArgSetupLdRegDisp(&ta0, dreg, disp);
	UAX_OpArgSetupImm(&ta1, imm);
	i=UAX_AssembleInsn(ctx, nmid, &ta0, &ta1, NULL);
	return(i);
}

int UAX_AsmInsnStRegDispImm32(UAX_Context *ctx, int nmid,
	int dreg, s32 disp, s32 imm)
{
	UAX_OpcodeArg ta0, ta1;
	int i;
	UAX_OpArgSetupLdRegDisp(&ta0, dreg, disp);
	ta0.sc|=UAX_MEMSC_DWORD;
	UAX_OpArgSetupImm(&ta1, imm);
	i=UAX_AssembleInsn(ctx, nmid, &ta0, &ta1, NULL);
	return(i);
}


int UAX_AsmInsnRegLdRegIxDisp(UAX_Context *ctx, int nmid,
	int dreg, int sreg, int sc, int ireg, s32 disp)
{
	UAX_OpcodeArg ta0, ta1;
	int i;
	UAX_OpArgSetupReg(&ta0, dreg);
	UAX_OpArgSetupLdRegIxDisp(&ta1, sreg, sc, ireg, disp);
	i=UAX_AssembleInsn(ctx, nmid, &ta0, &ta1, NULL);
	return(i);
}

int UAX_AsmInsnStRegIxDispReg(UAX_Context *ctx, int nmid,
	int dreg, int sc, int ireg, s32 disp, int sreg)
{
	UAX_OpcodeArg ta0, ta1;
	int i;
	UAX_OpArgSetupLdRegIxDisp(&ta0, dreg, sc, ireg, disp);
	UAX_OpArgSetupReg(&ta1, sreg);
	i=UAX_AssembleInsn(ctx, nmid, &ta0, &ta1, NULL);
	return(i);
}

int UAX_AsmMovRegReg(UAX_Context *ctx, int dreg, int sreg)
	{ return(UAX_AsmInsnRegReg(ctx, UAX_OP_MOV, dreg, sreg)); }
int UAX_AsmMovRegImm(UAX_Context *ctx, int dreg, s64 imm)
	{ return(UAX_AsmInsnRegImm(ctx, UAX_OP_MOV, dreg, imm)); }

int UAX_AsmAddRegReg(UAX_Context *ctx, int dreg, int sreg)
	{ return(UAX_AsmInsnRegReg(ctx, UAX_OP_ADD, dreg, sreg)); }
int UAX_AsmAddRegImm(UAX_Context *ctx, int dreg, s64 imm)
	{ return(UAX_AsmInsnRegImm(ctx, UAX_OP_ADD, dreg, imm)); }

int UAX_AsmSubRegReg(UAX_Context *ctx, int dreg, int sreg)
	{ return(UAX_AsmInsnRegReg(ctx, UAX_OP_SUB, dreg, sreg)); }
int UAX_AsmSubRegImm(UAX_Context *ctx, int dreg, s64 imm)
	{ return(UAX_AsmInsnRegImm(ctx, UAX_OP_SUB, dreg, imm)); }

int UAX_AsmAndRegReg(UAX_Context *ctx, int dreg, int sreg)
	{ return(UAX_AsmInsnRegReg(ctx, UAX_OP_AND, dreg, sreg)); }
int UAX_AsmAndRegImm(UAX_Context *ctx, int dreg, s64 imm)
	{ return(UAX_AsmInsnRegImm(ctx, UAX_OP_AND, dreg, imm)); }

int UAX_AsmOrRegReg(UAX_Context *ctx, int dreg, int sreg)
	{ return(UAX_AsmInsnRegReg(ctx, UAX_OP_OR, dreg, sreg)); }
int UAX_AsmOrRegImm(UAX_Context *ctx, int dreg, s64 imm)
	{ return(UAX_AsmInsnRegImm(ctx, UAX_OP_OR, dreg, imm)); }

int UAX_AsmXorRegReg(UAX_Context *ctx, int dreg, int sreg)
	{ return(UAX_AsmInsnRegReg(ctx, UAX_OP_XOR, dreg, sreg)); }
int UAX_AsmXorRegImm(UAX_Context *ctx, int dreg, s64 imm)
	{ return(UAX_AsmInsnRegImm(ctx, UAX_OP_XOR, dreg, imm)); }

int UAX_AsmCmpRegReg(UAX_Context *ctx, int dreg, int sreg)
	{ return(UAX_AsmInsnRegReg(ctx, UAX_OP_CMP, dreg, sreg)); }
int UAX_AsmCmpRegImm(UAX_Context *ctx, int dreg, s64 imm)
	{ return(UAX_AsmInsnRegImm(ctx, UAX_OP_CMP, dreg, imm)); }

int UAX_AsmTestRegReg(UAX_Context *ctx, int dreg, int sreg)
	{ return(UAX_AsmInsnRegReg(ctx, UAX_OP_TEST, dreg, sreg)); }
int UAX_AsmTestRegImm(UAX_Context *ctx, int dreg, s64 imm)
	{ return(UAX_AsmInsnRegImm(ctx, UAX_OP_TEST, dreg, imm)); }


int UAX_AsmShlRegReg(UAX_Context *ctx, int dreg, int sreg)
	{ return(UAX_AsmInsnRegReg(ctx, UAX_OP_SHL, dreg, sreg)); }
int UAX_AsmShlRegImm(UAX_Context *ctx, int dreg, s64 imm)
	{ return(UAX_AsmInsnRegImm(ctx, UAX_OP_SHL, dreg, imm)); }

int UAX_AsmShrRegReg(UAX_Context *ctx, int dreg, int sreg)
	{ return(UAX_AsmInsnRegReg(ctx, UAX_OP_SHR, dreg, sreg)); }
int UAX_AsmShrRegImm(UAX_Context *ctx, int dreg, s64 imm)
	{ return(UAX_AsmInsnRegImm(ctx, UAX_OP_SHR, dreg, imm)); }

int UAX_AsmSarRegReg(UAX_Context *ctx, int dreg, int sreg)
	{ return(UAX_AsmInsnRegReg(ctx, UAX_OP_SAR, dreg, sreg)); }
int UAX_AsmSarRegImm(UAX_Context *ctx, int dreg, s64 imm)
	{ return(UAX_AsmInsnRegImm(ctx, UAX_OP_SAR, dreg, imm)); }
