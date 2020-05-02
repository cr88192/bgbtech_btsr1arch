#ifdef UA32_LINUX
#include <sys/mman.h>
#endif

#ifdef UA32_LINUX
void __clear_cache(void *beg, void *end);
#endif

static int ua32_init=0;
static int ua32_fault=0;

byte *ua32_exheap_start;
byte *ua32_exheap_end;
byte *ua32_exheap_pos;

char *ua32_gblsym_name[4096];
void *ua32_gblsym_addr[4096];
int ua32_gblsym_num;

UA32_Context *ua32_opctx_freelist;

int UA32_Init()
{
#ifdef UA32_WINDOWS
	MEMORY_BASIC_INFORMATION memi;
#endif
	nlint a0, a1, a2;
	int ln;
	int i, j, k, l;

	if(ua32_init)
		return(0);
	ua32_init=1;
		
	ua32_gblsym_name[0]=NULL;
	ua32_gblsym_addr[0]=NULL;
	ua32_gblsym_num=1;
	
	ua32_exheap_start=NULL;

#ifdef UA32_WINX64
	a0=(nlint)((void *)UA32_Init);
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

		ua32_exheap_start=VirtualAlloc((PVOID)a1, 1<<24,
			MEM_COMMIT|MEM_RESERVE|MEM_TOP_DOWN, PAGE_EXECUTE_READWRITE);
		if(ua32_exheap_start)
			break;
	}
#endif
	
#ifdef UA32_WINDOWS
	if(!ua32_exheap_start)
	{
		ua32_exheap_start=VirtualAlloc(NULL, 1<<24,
			MEM_COMMIT|MEM_RESERVE|MEM_TOP_DOWN, PAGE_EXECUTE_READWRITE);
	}
#endif

#ifdef UA32_LINUX
	if(!ua32_exheap_start)
	{
		ua32_exheap_start=mmap(NULL, 1<<24,
			PROT_READ|PROT_WRITE|PROT_EXEC,
//			MAP_PRIVATE|MAP_ANONYMOUS|MAP_32BIT, -1, 0);
			MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	}
#endif

	if(!ua32_exheap_start || (ua32_exheap_start==((byte *)-1)))
	{
		printf("UA32_Init: Failed Alloc ExHeap\n");
		ua32_fault=1;
		return(-1);
	}

	ua32_exheap_end=ua32_exheap_start+(1<<24);
	ua32_exheap_pos=ua32_exheap_start;
	
//	i=UA32_CheckAddrExHeapRel32((void *)UA32_Init);
	i=0;
	
	printf("UA32_Init: ExHeap %p..%p, EXE~=%p, IR32=%d\n",
		ua32_exheap_start, ua32_exheap_end, (void *)UA32_Init, i);
	
	return(1);
}

int UA32_LookupGlobalName(char *name)
{
	int i;
	
	if(!name)
		return(0);
	
	for(i=0; i<ua32_gblsym_num; i++)
	{
		if(!ua32_gblsym_name[i])
			continue;
		if(!strcmp(ua32_gblsym_name[i], name))
			return(i);
	}
	
	return(0);
}

int UA32_LookupGlobalAddress(void *addr)
{
	int i;
	
	if(!addr)
		return(0);
	
	for(i=0; i<ua32_gblsym_num; i++)
	{
//		if(!ua32_gblsym_name[i])
//			continue;
		if(ua32_gblsym_addr[i]==addr)
			return(i);
	}
	
	return(0);
}

int UA32_RegisterGlobal(char *name, void *addr)
{
	int i, j, k;

	if(name)
	{
		i=UA32_LookupGlobalName(name);
		if(i>0)
		{
			ua32_gblsym_addr[i]=addr;
			return(i);
		}

		i=UA32_LookupGlobalAddress(addr);
		if((i>0) && !ua32_gblsym_name[i])
		{
			ua32_gblsym_name[i]=strdup(name);
			return(i);
		}
		
		i=ua32_gblsym_num++;
		ua32_gblsym_name[i]=strdup(name);
		ua32_gblsym_addr[i]=addr;
		return(i);
	}else
	{
		i=UA32_LookupGlobalAddress(addr);
		if(i>0)
		{
			return(i);
		}

		i=ua32_gblsym_num++;
		ua32_gblsym_name[i]=NULL;
		ua32_gblsym_addr[i]=addr;
		return(i);
	}
}

byte *UA32_ExHeapMalloc(int sz)
{
	byte *ptr;
	sz=(sz+63)&(~63);
	ptr=ua32_exheap_pos;
	ua32_exheap_pos=ptr+sz;
	if(ua32_exheap_pos>=ua32_exheap_end)
		return(NULL);
	return(ptr);
}

byte *UA32_ExHeapMAllocText(int sz)
{
	byte *ptr;
	sz=(sz+63)&(~63);
	ptr=ua32_exheap_pos;
	ua32_exheap_pos=ptr+sz;
	if(ua32_exheap_pos>=ua32_exheap_end)
		return(NULL);
	memset(ptr, 0xCC, sz);
	return(ptr);
}

byte *UA32_ExHeapMAllocData(int sz)
{
	byte *ptr;
	sz=(sz+63)&(~63);
	ptr=ua32_exheap_pos;
	ua32_exheap_pos=ptr+sz;
	if(ua32_exheap_pos>=ua32_exheap_end)
		return(NULL);
	memset(ptr, 0, sz);
	return(ptr);
}

int UA32_ExHeapResetMark()
{
	ua32_exheap_pos=ua32_exheap_start;
	return(0);
}

byte *UA32_LinkContext(UA32_Context *ctx)
{
	byte *gptr[16];
	int gsz[16];
	byte *prlc, *plbl;
	nlint nla;
	u32 ui0, ui1, ui2;
	s32 d0, d1;
	int lbl;
	int i, j, k;
	
	for(i=0; i<16; i++)
	{
		gptr[i]=NULL; gsz[i]=0;
	}
	
	for(i=0; i<ctx->nsec; i++)
	{
		j=ctx->sec_pos[i]-ctx->sec_buf[i];
		if(j<=0)
			{ gptr[i]=NULL; gsz[i]=0; continue; }

		if(i==UA32_CSEG_TEXT)
		{
			gptr[i]=UA32_ExHeapMAllocText(j);
			gsz[i]=j;
		}else
		{
			gptr[i]=UA32_ExHeapMAllocData(j);
			gsz[i]=j;
		}
		
		if(!gptr[i])
			return(NULL);
		
		memcpy(gptr[i], ctx->sec_buf[i], gsz[i]);
	}
	
	for(i=0; i<ctx->nrlc; i++)
	{
		lbl=ctx->rlc_id[i];
		if(lbl>=UA32_LBL_LOCALSTART)
		{
			for(j=0; j<ctx->nlbl; j++)
				if(ctx->lbl_id[j]==lbl)
					break;
			if(j>=ctx->nlbl)
			{
				JX2_DBGBREAK
			}

			plbl=gptr[ctx->lbl_sec[j]]+ctx->lbl_ofs[j];
		}else if(lbl<ua32_gblsym_num)
		{
			plbl=ua32_gblsym_addr[lbl];
		}else
		{
			plbl=NULL;
		}
		
		if(!plbl)
		{
			JX2_DBGBREAK
		}
		
		nla=(nlint)plbl;
		
		prlc=gptr[ctx->rlc_sec[i]]+ctx->rlc_ofs[i];
//		plbl=gptr[ctx->lbl_sec[j]]+ctx->lbl_ofs[j];
		switch(ctx->rlc_ty[i])
		{
		case UA32_RLC_REL8:
			k=(plbl-prlc)-1;
			*(byte *)prlc+=k;
			break;
		case UA32_RLC_REL16:
			k=(plbl-prlc)-2;
			*(s16 *)prlc+=k;
			break;
		case UA32_RLC_REL32:
			k=(plbl-prlc)-4;
			*(s32 *)prlc+=k;
			break;
			
		case UA32_RLC_REL24W:
			d0=((plbl-prlc)-8)>>2;
			ui0=*(u32 *)prlc;
			d1=(((s32)(ui0<<8))>>8)+d0;
			ui1=(ui0&0xFF000000)|(d1&0x00FFFFFF);
			*(u32 *)prlc=ui1;
			break;

		case UA32_RLC_REL11T:
			d0=((plbl-prlc)-4)>>1;
			ui0=*(u16 *)prlc;
			d1=(((s32)(ui0<<21))>>21)+d0;
			ui1=(ui0&0xF800)|(d1&0x07FF);
			*(u16 *)prlc=ui1;
			break;
		case UA32_RLC_REL22T:
			d0=((plbl-prlc)-4)>>1;
			ui0=((u16 *)prlc)[0];
			ui1=((u16 *)prlc)[1];
			d1=((ui0&0x7FF)<<11)|(ui1&0x07FF);
			d1=(((s32)(d1<<10))>>10)+d0;
			ui0=(ui0&0xF800)|((d1>>11)&0x07FF);
			ui1=(ui1&0xF800)|((d1>> 0)&0x07FF);
			((u16 *)prlc)[0]=ui0;
			((u16 *)prlc)[1]=ui1;
			break;
		case UA32_RLC_REL8T:
			d0=((plbl-prlc)-4)>>1;
			ui0=*(u16 *)prlc;
			d1=(((s32)(ui0<<24))>>24)+d0;
			ui1=(ui0&0xFF00)|(d1&0x00FF);
			*(u16 *)prlc=ui1;
			break;
			
		case UA32_RLC_REL16TB:
			d0=(plbl-prlc)-8;
			ui0=((u16 *)prlc)[0];
			ui1=((u16 *)prlc)[1];
			d1=d0;
			ui0=(ui0&0xFBF0)|((d1>>1)&0x0400)|((d1>>12)&0x000F);
			ui1=(ui1&0x0F00)|((d1   )&0x00FF)|((d1<< 4)&0x7000);
			((u16 *)prlc)[0]=ui0;
			((u16 *)prlc)[1]=ui1;
			break;

		case UA32_RLC_ABS8:
			*(byte *)prlc+=nla;
			break;
		case UA32_RLC_ABS16:
			*(s16 *)prlc+=nla;
			break;
		case UA32_RLC_ABS32:
			*(s32 *)prlc+=nla;
			break;
		case UA32_RLC_ABS64:
			*(s64 *)prlc+=nla;
			break;
		default:
			printf("Unhandled Reloc Type %X\n", ctx->rlc_ty[i]);
			fflush(stdout);
			JX2_DBGBREAK
			break;
		}
	}
	
	__clear_cache(
		gptr[UA32_CSEG_TEXT],
		gptr[UA32_CSEG_TEXT]+gsz[UA32_CSEG_TEXT]);
	
	return(gptr[UA32_CSEG_TEXT]);
}

int UA32_DumpMemHex(byte *spos, byte *epos)
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

UA32_Context *UA32_AllocContext()
{
	UA32_Context *tmp;
	int i;
	
	UA32_Init();
	
	if(ua32_fault)
		return(NULL);
	
	if(ua32_opctx_freelist)
	{
		tmp=ua32_opctx_freelist;
		ua32_opctx_freelist=tmp->next;
		
		for(i=0; i<tmp->nsec; i++)
			{ tmp->sec_pos[i]=tmp->sec_buf[i]; }
		tmp->nlbl=0;
		tmp->nrlc=0;
		tmp->nsec=0;
		
		tmp->reg_live=0;
		tmp->reg_resv=0;
		tmp->reg_valid=0;
		tmp->reg_dirty=0;
		tmp->jitfl=0;
		tmp->lblrov=UA32_LBL_LOCALSTART;
		tmp->n_ctab=0;
		
		return(tmp);
	}
	
	tmp=malloc(sizeof(UA32_Context));
	memset(tmp, 0, sizeof(UA32_Context));
	tmp->lblrov=UA32_LBL_LOCALSTART;

	return(tmp);
}

int UA32_FreeContext(UA32_Context *ctx)
{
	ctx->next=ua32_opctx_freelist;
	ua32_opctx_freelist=ctx;
	return(0);
}

int UA32_SetSection(UA32_Context *ctx, int sec)
{
	ctx->sec=sec;
	if((sec+1)>ctx->nsec)
		ctx->nsec=sec+1;
	return(0);
}

int UA32_EmitByte(UA32_Context *ctx, int val)
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

int UA32_EmitWord(UA32_Context *ctx, int val)
{
	UA32_EmitByte(ctx, (val   )&255);
	UA32_EmitByte(ctx, (val>>8)&255);
	return(0);
}

int UA32_EmitDWord(UA32_Context *ctx, s32 val)
{
	UA32_EmitByte(ctx, (val    )&255);
	UA32_EmitByte(ctx, (val>> 8)&255);
	UA32_EmitByte(ctx, (val>>16)&255);
	UA32_EmitByte(ctx, (val>>24)&255);
	return(0);
}

int UA32_EmitQWord(UA32_Context *ctx, s64 val)
{
	UA32_EmitByte(ctx, (val    )&255);
	UA32_EmitByte(ctx, (val>> 8)&255);
	UA32_EmitByte(ctx, (val>>16)&255);
	UA32_EmitByte(ctx, (val>>24)&255);
	UA32_EmitByte(ctx, (val>>32)&255);
	UA32_EmitByte(ctx, (val>>40)&255);
	UA32_EmitByte(ctx, (val>>48)&255);
	UA32_EmitByte(ctx, (val>>56)&255);
	return(0);
}

byte *UA32_EmitGetPos(UA32_Context *ctx)
{
	return(ctx->sec_pos[ctx->sec]);
}

int UA32_EmitGetOffs(UA32_Context *ctx)
{
	return(ctx->sec_pos[ctx->sec]-ctx->sec_buf[ctx->sec]);
}

int UA32_EmitBAlign(UA32_Context *ctx, int val)
{
	int ofs, n;
	int i;

	ofs=UA32_EmitGetOffs(ctx);
	if(ofs&(val-1))
	{
		n=val-(ofs&(val-1));
		i=n;
		while(i--)
			{ UA32_EmitByte(ctx, 0); }
	}
	return(0);
}

int UA32_GenLabelTemp(UA32_Context *ctx)
{
	int i;
	i=ctx->lblrov++;
	return(i);
}

int UA32_EmitLabel(UA32_Context *ctx, int lblid)
{
	int i;
	
	if(lblid>0)
	{
		i=ctx->nlbl++;
		ctx->lbl_id[i]=lblid;
		ctx->lbl_ofs[i]=UA32_EmitGetOffs(ctx);
		ctx->lbl_sec[i]=ctx->sec;
//		ctx->lbl_ty[i]=ty;
	}
	return(0);
}


int UA32_EmitRelocTy(UA32_Context *ctx, int lblid, int ty)
{
	int i;
	
	if(lblid>0)
	{
		i=ctx->nrlc++;
		ctx->rlc_id[i]=lblid;
		ctx->rlc_ofs[i]=UA32_EmitGetOffs(ctx);
		ctx->rlc_sec[i]=ctx->sec;
		ctx->rlc_ty[i]=ty;
	}
	return(0);
}

int UA32_EmitRelocRel8(UA32_Context *ctx, int lbl)
	{ return(UA32_EmitRelocTy(ctx, lbl, UA32_RLC_REL8)); }
int UA32_EmitRelocRel16(UA32_Context *ctx, int lbl)
	{ return(UA32_EmitRelocTy(ctx, lbl, UA32_RLC_REL16)); }
int UA32_EmitRelocRel32(UA32_Context *ctx, int lbl)
	{ return(UA32_EmitRelocTy(ctx, lbl, UA32_RLC_REL32)); }

int UA32_EmitRelocAbs8(UA32_Context *ctx, int lbl)
	{ return(UA32_EmitRelocTy(ctx, lbl, UA32_RLC_ABS8)); }
int UA32_EmitRelocAbs16(UA32_Context *ctx, int lbl)
	{ return(UA32_EmitRelocTy(ctx, lbl, UA32_RLC_ABS16)); }
int UA32_EmitRelocAbs32(UA32_Context *ctx, int lbl)
	{ return(UA32_EmitRelocTy(ctx, lbl, UA32_RLC_ABS32)); }
int UA32_EmitRelocAbs64(UA32_Context *ctx, int lbl)
	{ return(UA32_EmitRelocTy(ctx, lbl, UA32_RLC_ABS64)); }


int UA32_LookupIndexImm32(UA32_Context *ctx, u32 val)
{
	int i;
	
	for(i=0; i<ctx->n_ctab; i++)
	{
		if(ctx->ctab[i]==val)
			return(i);
	}
	return(-1);
}

int UA32_GetIndexImm32(UA32_Context *ctx, u32 val)
{
	int i;
	
	for(i=0; i<ctx->n_ctab; i++)
	{
		if(ctx->ctab[i]==val)
			return(i);
	}
	
	i=ctx->n_ctab++;
	ctx->ctab[i]=val;
	return(i);
}

