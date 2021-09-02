/*
 Copyright (c) 2018-2020 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

int BJX2_MemRamCb_GetByte(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	int ra;
//	ra=addr-sp->addr_base;
	ra=(addr-sp->modbase)&(sp->modmask);
	return(BJX2_PtrGetSByteOfsLe(sp->data, ra));
}

int BJX2_MemRamCb_GetWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	int ra;
//	ra=addr-sp->addr_base;
	ra=(addr-sp->modbase)&(sp->modmask);
	return(BJX2_PtrGetSWordOfsLe(sp->data, ra));
}

s32 BJX2_MemRamCb_GetDWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	int ra;
//	ra=addr-sp->addr_base;
	ra=(addr-sp->modbase)&(sp->modmask);
	return(BJX2_PtrGetSDWordOfsLe(sp->data, ra));
}

s64 BJX2_MemRamCb_GetQWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	int ra;
//	ra=addr-sp->addr_base;
	ra=(addr-sp->modbase)&(sp->modmask);
	return(BJX2_PtrGetSQWordOfsLe(sp->data, ra));
}

int BJX2_MemRamCb_GetXWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, u64 *rvlo, u64 *rvhi)
{
	int ra;
//	ra=addr-sp->addr_base;
	ra=(addr-sp->modbase)&(sp->modmask);
	*rvlo=BJX2_PtrGetSQWordOfsLe(sp->data, ra+0);
	*rvhi=BJX2_PtrGetSQWordOfsLe(sp->data, ra+8);
	return(0);
}


int BJX2_MemRamCb_SetByte(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int val)
{
	int ra, ra4;
//	ra=addr-sp->addr_base;
	ra=(addr-sp->modbase)&(sp->modmask);
	BJX2_PtrSetByteOfsLe(sp->data, ra, val);

	ra4=(ra>>4);
//	if(sp->tripwire[ra4>>3]&(1<<(ra4&7)))
	if(sp->tripwire[ra4>>5]&(1<<(ra4&31)))
		{ BJX2_MemRamCb_SetFault(ctx, sp, addr, val); }
	return(0);
}

int BJX2_MemRamCb_SetWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int val)
{
	int ra, ra4;
//	ra=addr-sp->addr_base;
	ra=(addr-sp->modbase)&(sp->modmask);
	BJX2_PtrSetWordOfsLe(sp->data, ra, val);

	ra4=(ra>>4);
//	if(sp->tripwire[ra4>>3]&(1<<(ra4&7)))
	if(sp->tripwire[ra4>>5]&(1<<(ra4&31)))
		{ BJX2_MemRamCb_SetFault(ctx, sp, addr, val); }
	return(0);
}

int BJX2_MemRamCb_SetDWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s32 val)
{
	int ra, ra4;
//	ra=addr-sp->addr_base;
	ra=(addr-sp->modbase)&(sp->modmask);
	BJX2_PtrSetDWordOfsLe(sp->data, ra, val);

	ra4=(ra>>4);
//	if(sp->tripwire[ra4>>3]&(1<<(ra4&7)))
	if(sp->tripwire[ra4>>5]&(1<<(ra4&31)))
		{ BJX2_MemRamCb_SetFault(ctx, sp, addr, val); }
	return(0);
}

int BJX2_MemRamCb_SetQWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s64 val)
{
	int ra, ra4;
//	ra=addr-sp->addr_base;
	ra=(addr-sp->modbase)&(sp->modmask);
	BJX2_PtrSetQWordOfsLe(sp->data, ra, val);

	ra4=(ra>>4);
//	if(sp->tripwire[ra4>>3]&(1<<(ra4&7)))
	if(sp->tripwire[ra4>>5]&(1<<(ra4&31)))
		{ BJX2_MemRamCb_SetFault(ctx, sp, addr, val); }
	return(0);
}

int BJX2_MemRamCb_SetXWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, u64 vlo, u64 vhi)
{
	int ra, ra4;
//	ra=addr-sp->addr_base;
	ra=(addr-sp->modbase)&(sp->modmask);
	BJX2_PtrSetQWordOfsLe(sp->data, ra+0, vlo);
	BJX2_PtrSetQWordOfsLe(sp->data, ra+8, vhi);

	ra4=(ra>>4);
//	if(sp->tripwire[ra4>>3]&(1<<(ra4&7)))
	if(sp->tripwire[ra4>>5]&(1<<(ra4&31)))
		{ BJX2_MemRamCb_SetFault(ctx, sp, addr, vlo); }
	return(0);
}

int BJX2_MemRamCb_GetFaultXW(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, u64 *rvlo, u64 *rvhi)
{
	if(!(ctx->status))
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INV_MWR);
	}else
	{
//		JX2_DBGBREAK
	}
	return(0);
}

int BJX2_MemRamCb_SetFault(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s32 val)
{
	if(!(ctx->status))
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INV_MWR);
	}else
	{
//		JX2_DBGBREAK
	}
	return(0);
}

int BJX2_MemRamCb_SetFaultQW(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s64 val)
{
	if(!(ctx->status))
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INV_MWR);
	}else
	{
//		JX2_DBGBREAK
	}
	return(0);
}

int BJX2_MemRamCb_SetFaultXW(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, u64 vlo, u64 vhi)
{
	if(!(ctx->status))
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INV_MWR);
	}else
	{
//		JX2_DBGBREAK
	}
	return(0);
}

int BJX2_MemRamCb_SetTripwire(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int mode)
{
	int ra, ra4;
//	ra=addr-sp->addr_base;
	ra=(addr-sp->modbase)&(sp->modmask);
//	BJX2_PtrSetQWordOfsLe(sp->data, ra, val);

	ra4=(ra>>4);
//	if(sp->tripwire[ra4>>3]&(1<<(ra4&7)))
//		{ BJX2_MemRamCb_SetFault(ctx, sp, addr, val); }

	if(mode==3)
	{
//		sp->tripwire[ra4>>3]|=(1<<(ra4&7));
		sp->tripwire[ra4>>5]|=(1<<(ra4&31));
	}else if(mode==2)
	{
//		sp->tripwire[ra4>>3]&=~(1<<(ra4&7));
		sp->tripwire[ra4>>5]&=~(1<<(ra4&31));
	}

	return(0);
}

int BJX2_MemRamCb_MemQueryTransit(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr0, bjx2_addr addr1, int mode)
{
	int ra0, ra1, ra4, ra5, rac;
	u32 vlo, vhi, v;
	u64 vli;

	ra0=(addr0-sp->modbase)&(sp->modmask);
	ra1=(addr1-sp->modbase)&(sp->modmask);

	if(ra1<ra0)
		return(0);

	if(mode!=1)
		return(0);

	ra4=(ra0>>4);
	ra5=(ra1>>4);

	rac=ra4;
	
	while(sp->tripwire[rac>>5]&(1<<(rac&31)))
	{
		/* We care about transits here.
		 * Starting inside a tripwire is OK.
		 */
		rac++;
	}
	
	while(rac<ra5)
	{
		vlo=sp->tripwire[(rac>>5)+0];
		vhi=sp->tripwire[(rac>>5)+1];
		vli=vlo|(((u64)vhi)<<32);
		v=vli>>(rac&31);
		if(!v)
			{ rac+=32; continue; }
		if(!(v&65535))
			{ rac+=16; continue; }
		if(!(v&255))
			{ rac+=8; continue; }
		if(!(v&15))
			{ rac+=4; continue; }
		if(!(v&1))
			{ rac++; continue; }
		return(1);
	}

	return(0);
}

BJX2_MemSpan *BJX2_MemAllocSpan(BJX2_Context *ctx)
{
	BJX2_MemSpan *sp;
	
	sp=malloc(sizeof(BJX2_MemSpan));
	memset(sp, 0, sizeof(BJX2_MemSpan));
	return(sp);
}

int BJX2_MemDefineModRAM(BJX2_Context *ctx,
	char *name, bjx2_addr base, bjx2_addr lim, int mod)
{
	BJX2_MemSpan *sp;
	int nblk;
	
	if(mod>0)
	{
		nblk=(mod+15)/16;
	}else
	{
		nblk=((lim-base)+15)/16;
	}
	
	sp=BJX2_MemAllocSpan(ctx);
	
	sp->name=name;
	sp->addr_base=base;
	sp->addr_lim=lim;
	sp->addr_sz=lim-base;
//	sp->data=malloc((lim-base)+8);
	sp->data=malloc(nblk*16+8);
	sp->tripwire=malloc((nblk+63)/8);
	sp->simple_mem=3;

	if(mod>0)
	{
		sp->modbase=0;
		sp->modmask=mod-1;
	}else
	{
		sp->modbase=base;
		sp->modmask=-1;
	}
	
	memset(sp->tripwire, 0, nblk/8);
	
	sp->GetByte=BJX2_MemRamCb_GetByte;
	sp->GetWord=BJX2_MemRamCb_GetWord;
	sp->GetDWord=BJX2_MemRamCb_GetDWord;
	sp->GetQWord=BJX2_MemRamCb_GetQWord;
	sp->GetXWord=BJX2_MemRamCb_GetXWord;
	
	sp->SetByte=BJX2_MemRamCb_SetByte;
	sp->SetWord=BJX2_MemRamCb_SetWord;
	sp->SetDWord=BJX2_MemRamCb_SetDWord;
	sp->SetQWord=BJX2_MemRamCb_SetQWord;
	sp->SetXWord=BJX2_MemRamCb_SetXWord;

	sp->SetTripwire=BJX2_MemRamCb_SetTripwire;
	sp->MemQueryTransit=BJX2_MemRamCb_MemQueryTransit;
	
	BJX2_MemAddSpan(ctx, sp);
	return(0);
}

int BJX2_MemDefineRAM(BJX2_Context *ctx,
	char *name, bjx2_addr base, bjx2_addr lim)
{
	return(BJX2_MemDefineModRAM(ctx, name, base, lim, -1));
}

int BJX2_MemDefineROM(BJX2_Context *ctx,
	char *name, bjx2_addr base, bjx2_addr lim)
{
	BJX2_MemSpan *sp;
	
	sp=BJX2_MemAllocSpan(ctx);
	
	sp->name=name;
	sp->addr_base=base;
	sp->addr_lim=lim;
	sp->addr_sz=lim-base;
	sp->data=malloc((lim-base)+8);
	sp->simple_mem=1;
	sp->modbase=base;
	sp->modmask=-1;
	
	sp->GetByte=BJX2_MemRamCb_GetByte;
	sp->GetWord=BJX2_MemRamCb_GetWord;
	sp->GetDWord=BJX2_MemRamCb_GetDWord;
	sp->GetQWord=BJX2_MemRamCb_GetQWord;
	sp->GetXWord=BJX2_MemRamCb_GetXWord;
	
	sp->SetByte=BJX2_MemRamCb_SetFault;
	sp->SetWord=BJX2_MemRamCb_SetFault;
	sp->SetDWord=BJX2_MemRamCb_SetFault;
	sp->SetQWord=BJX2_MemRamCb_SetFaultQW;
	sp->SetXWord=BJX2_MemRamCb_SetFaultXW;
	
	BJX2_MemAddSpan(ctx, sp);
	return(0);
}

int BJX2_MemAddSpan(BJX2_Context *ctx, BJX2_MemSpan *sp)
{
	BJX2_MemSpan *sp1;
	int i, j;

	i=ctx->n_span++;
	while(i>0)
	{
		sp1=ctx->span[i-1];
		ctx->span[i]=sp1;
		if(sp1->addr_base<=sp->addr_base)
			break;
		i--;
	}
	ctx->span[i]=sp;
	return(0);
}

BJX2_MemSpan *BJX2_MemSpanForName(BJX2_Context *ctx, char *name)
{
	BJX2_MemSpan *sp;
	int nc, nb, nr;

	nb=0; nr=ctx->n_span;
	
	for(nc=0; nc<nr; nc++)
	{
		sp=ctx->span[nc];
		if(sp->name && !strcmp(sp->name, name))
			return(sp);
	}
	return(NULL);
}

BJX2_MemSpan *BJX2_MemSpanForAddr64(BJX2_Context *ctx, bjx2_addr addr)
{
	BJX2_MemSpan *sp;
	int nc, nb, nr;

	sp=ctx->span_pr0;
//	if(sp && (addr>=sp->addr_base) && (addr<=sp->addr_lim))
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
		return(sp);

	sp=ctx->span_pr1;
//	if(sp && (addr>=sp->addr_base) && (addr<=sp->addr_lim))
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		ctx->span_pr1=ctx->span_pr0;
		ctx->span_pr0=sp;
		return(sp);
	}

	nb=0; nr=ctx->n_span;
	
	if(nr<8)
	{
		for(nc=0; nc<nr; nc++)
		{
			sp=ctx->span[nc];
			if(addr<=sp->addr_lim)
			{
				if(addr<sp->addr_base)
					break;

				ctx->span_pr1=ctx->span_pr0;
				ctx->span_pr0=sp;
				return(sp);
			}
		}
		return(NULL);
	}
	
	while(nr>1)
	{
		nc=nb+(nr>>1);
		sp=ctx->span[nc];
		if(addr>=sp->addr_base)
		{
			nb=nc;
			nr-=(nr>>1);
		}else
		{
			nr=nr>>1;
		}
	}

	sp=ctx->span[nb];
	if((addr>=sp->addr_base) && (addr<=sp->addr_lim))
	{
		ctx->span_pr1=ctx->span_pr0;
		ctx->span_pr0=sp;
		return(sp);
	}
	
	return(NULL);
}

BJX2_MemSpan *BJX2_MemSpanForAddr32(BJX2_Context *ctx, bjx2_addr addr)
{
	return(BJX2_MemSpanForAddr64(ctx, (u32)addr));
}

BJX2_MemSpan *BJX2_MemSpanForAddr(BJX2_Context *ctx, bjx2_addr addr)
{
	return(ctx->MemSpanForAddr(ctx, addr));

//	if(ctx->MemSpanForAddr)
//		return(ctx->MemSpanForAddr(ctx, addr));
	
//	if(ctx->regs[BJX2_REG_SR]&0x80000000)
//		ctx->MemSpanForAddr=BJX2_MemSpanForAddr64;
//	else
//		ctx->MemSpanForAddr=BJX2_MemSpanForAddr32;
//	return(ctx->MemSpanForAddr(ctx, addr));
}

int BJX2_MemFlushL2(BJX2_Context *ctx)
{
	ctx->mem_l2rov++;
	return(0);
}

int BJX2_MemFlushL1(BJX2_Context *ctx)
{
	ctx->mem_l1rov++;
	return(0);
}

int BJX2_MemSimAddrL2(BJX2_Context *ctx, bjx2_addr addr, int acc)
{
	bjx2_addr t0, t1;
	bjx2_addr ax, ax1, ax2, ax3, ax4, ax5;
	int epo, de0, de1;
	int miss0, miss1, miss;
	int h, p1, p2, p, dp, cnt;

	if(!(addr>>24))
		return(0);
	if((addr>>30)&3)
		return(0);

//	return(0);

	epo=(ctx->tot_cyc>>14)&15;
//	epo=(ctx->tot_cyc>>18)&15;
	if(acc&16)
		{ cnt=ctx->tot_cnt_mem_l2v++; }
	else if(acc&8)
		{ cnt=ctx->mem_cnt_l2i++; }
	else
		{ cnt=ctx->mem_cnt_l2++; }

//	ax=addr>>6;
//	ax=addr&(~63);
	ax=addr&0x3FFFFFC0U;

//	h=((addr>>6)^(addr>>19))&4095;
//	h=((addr>>6)^(addr>>17))&2047;
	h=((ax>>6)^(ax>>17))&2047;
//	h=((ax>>6)^(ax>>16))&2047;
//	h=((ax>>6)^(ax>>19))&2047;
//	h=(ax>>6)&2047;

	ax2=ctx->mem_l2h32k[(h<<2)|0];
	ax3=ctx->mem_l2h32k[(h<<2)|1];

	miss0=(ax!=(ax2&0x3FFFFFC0U));
	miss1=(ax!=(ax3&0x3FFFFFC0U));

	if(((ax2>>30)&3)!=(ctx->mem_l2rov&3))
		miss0=1;
	if(((ax3>>30)&3)!=(ctx->mem_l2rov&3))
		miss1=1;

//	de0=((ax2>>2)-epo)&15;
//	de1=((ax3>>2)-epo)&15;
	de0=(epo-(ax2>>2))&15;
	de1=(epo-(ax3>>2))&15;

	ax|=epo<<2;
	ax|=((bjx2_addr)(ctx->mem_l2rov&3))<<30;

	if(acc&1)
//	if(0)
	{
		if(!miss0)
		{
			ctx->mem_l2h32k[(h<<2)|0]=ax|1;
			return(0);
		}

#if 1
		if(!miss1 && !(ax2&1))
		{
			ctx->mem_l2h32k[(h<<2)|0]=ax3|1;
			ctx->mem_l2h32k[(h<<2)|1]=ax2;
			return(0);
		}
#endif
	}else
	{
		if(!miss0)
			return(0);
		if(!miss1)
			return(0);
	}

#if 0
	/* What if, victim buffer? */

	ax4=0;
	ax5=0;

#if 1
	ax1=ax&0x3FFFFFC0U;
	if(ax1==ctx->mem_l2addr1)	ax4=ctx->mem_l2addr1;
	if(ax1==ctx->mem_l2addr2)	ax4=ctx->mem_l2addr2;
	if(ax1==ctx->mem_l2addr3)	ax4=ctx->mem_l2addr3;
	if(ax1==ctx->mem_l2addr4)	ax4=ctx->mem_l2addr4;
#endif

	if(miss0)
	{
		ctx->mem_l2addr4=ctx->mem_l1addr3;
		ctx->mem_l2addr3=ctx->mem_l1addr2;
		ctx->mem_l2addr2=ctx->mem_l1addr1;
		ctx->mem_l2addr1=ax2&0x3FFFFFC0U;
	}

	if(miss0 && ax4 && !(ax2&1))
	{
		if(acc&1)
			ax4|=1;

		ax4|=epo<<2;
		ax4|=((bjx2_addr)(ctx->mem_l2rov&3))<<30;

		ctx->mem_l2h32k[(h<<2)|0]=ax4;
		return(0);
	}
#endif

	p1=35+5;
	p2=60+5;
//	p2=35+15;

	p=p1;
	dp=1;

	if(acc&1)
//	if(1)
	{
		ctx->mem_l2h32k[(h<<2)|0]=ax|1;
		if(!miss1)
		{
//			ctx->mem_l2h32k[(h<<2)|1]=0;
			ctx->mem_l2h32k[(h<<2)|1]=ax2&(~1);
		}
		if(ax2&1)
			{ p=p2; dp=2; }
	}else
	{
		if(ax2&1)
		{
			if(de0&8)
			{
				ctx->mem_l2h32k[(h<<2)|0]=ax;
				p=p2; dp=2;
			}else
			{
				ctx->mem_l2h32k[(h<<2)|1]=ax;
			}
		}
		else
		{
//			if(addr&16)
			if((de0>de1) || ((de0==de1) && (cnt&1)))
			{
				ctx->mem_l2h32k[(h<<2)|0]=ax;
			}else
			{
				ctx->mem_l2h32k[(h<<2)|1]=ax;
			}
		}
	}

	if(acc&2)
	{
//		p=4;
	}

//	if(ax2&1)
//		p=p2;

	if(acc&16)
	{
		ctx->miss_cyc+=p;
		ctx->tot_cyc_miss_l2v+=p;
		ctx->tot_cnt_miss_l2v++;
		cnt=ctx->mem_cnt_dram+=dp;
	}else
		if(acc&8)
	{
		ctx->miss_cyc+=p;
		ctx->miss_cyc_l2i+=p;
		ctx->miss_cnt_l2i++;
		ctx->mem_cnt_dri++;
		if(ctx->tr_cur)
			ctx->tr_cur->acc_pencyc+=p;
		cnt=ctx->mem_cnt_dram+=dp;
	}else
	{
		ctx->miss_cyc+=p;
		ctx->miss_cyc_l2+=p;
		ctx->miss_cnt_l2++;
		ctx->mem_cnt_drd++;
		if(ctx->tr_cur)
			ctx->tr_cur->acc_pencyc+=p;
		cnt=ctx->mem_cnt_dram+=dp;
	}

	return(1);
}

int BJX2_MemSimAddrL1I(BJX2_Context *ctx, bjx2_addr addr)
{
#if 1
	bjx2_addr t0, t1;
	int h, p1, p2;
	
//	if(ctx->use_jit)
	if(ctx->use_jit || ctx->no_memcost)
		return(0);
	
	if(((addr>>28)&0xF)==0xF)
		return(0);
	
//	h=(addr>>4)&63;
//	h=(addr>>5)&63;
	h=(addr>>5)&255;
//	h=(addr>>5)&511;

//	ctx->mem_cyc+=3;
//	ctx->mem_cyc+=2;
	ctx->mem_cnt_l1i++;

//	if((addr>>4)==ctx->mem_l1ih4k[(h<<2)|0])
//		return(0);
	if((addr>>5)==ctx->mem_l1ih4k[(h<<2)|0])
		return(0);
	if((addr>>5)==ctx->mem_l1ih4k[(h<<2)|1])
		return(0);

	p1=5;
	p2=24;

	ctx->miss_cyc+=p1;
	ctx->miss_cyc_l1+=p1;
	ctx->miss_cnt_l1i++;

	if(ctx->tr_cur)
		ctx->tr_cur->acc_pencyc+=p1;

//	ctx->mem_l1h4k[(h<<2)|3]=ctx->mem_l1h4k[(h<<2)|2];
//	ctx->mem_l1h4k[(h<<2)|2]=ctx->mem_l1h4k[(h<<2)|1];
	ctx->mem_l1h4k[(h<<2)|1]=ctx->mem_l1h4k[(h<<2)|0];

//	ctx->mem_l1ih4k[(h<<2)|0]=addr>>4;
	ctx->mem_l1ih4k[(h<<2)|0]=addr>>5;


	/* L2 Cache */

	BJX2_MemSimAddrL2(ctx, addr+ 0, 8);
	BJX2_MemSimAddrL2(ctx, addr+16, 8);
	return(1);

#if 0
	ctx->mem_cnt_l2++;

//	h=(addr>>4)&1023;
//	h=(addr>>4)&2047;
//	h=(addr>>4)&4095;
//	h=(addr>>4)&8191;
//	h=((addr>>4)^(addr>>17))&4095;
	h=((addr>>4)^(addr>>17))&8191;

	if((addr>>4)==ctx->mem_l2h32k[(h<<1)|0])
		return(0);

#if 0
	if((addr>>4)==ctx->mem_l2h32k[(h<<1)|1])
	{
		t0=ctx->mem_l2h32k[(h<<1)|0];
		t1=ctx->mem_l2h32k[(h<<1)|1];
		ctx->mem_l2h32k[(h<<1)|0]=t1;
		ctx->mem_l2h32k[(h<<1)|1]=t0;
		return(0);
	}
#endif

	ctx->mem_l2h32k[(h<<1)|1]=ctx->mem_l2h32k[(h<<1)|0];
	ctx->mem_l2h32k[(h<<1)|0]=addr>>4;

	/* Main RAM */

	ctx->miss_cyc+=p2;
	ctx->miss_cyc_l2+=p2;
	ctx->miss_cnt_l2++;
	ctx->mem_cnt_dri++;

	if(ctx->tr_cur)
		ctx->tr_cur->acc_pencyc+=p2;
#endif

	return(1);
#endif
}

#if 0
int BJX2_MemSimAddrL1(BJX2_Context *ctx, bjx2_addr addr)
{
#if 1
	bjx2_addr t0, t1;
	int h, p1, p2;
	
//	if(ctx->use_jit)
	if(ctx->use_jit || ctx->no_memcost)
		return(0);

	ctx->mem_cnt_mem++;

	if(((addr>>28)&0xF)==0xF)
	{
//		p1=7+15;
		p1=15;
//		p2=15;

		ctx->mem_cyc+=2;

		ctx->miss_cyc+=p1;
		ctx->mem_cyc_mmio+=p1;
	
//		ctx->miss_cyc+=p1;
//		ctx->miss_cyc_l1+=p1;
//		ctx->miss_cnt_l1++;

//		ctx->miss_cyc+=p2;
//		ctx->miss_cyc_l2+=p2;
//		ctx->miss_cnt_l2++;
//		ctx->mem_cnt_drd++;

		if(ctx->tr_cur)
			ctx->tr_cur->acc_pencyc+=p1;

		return(0);
	}
	
//	h=(addr>>4)&63;
//	h=(addr>>5)&63;
//	h=(addr>>5)&255;
//	h=(addr>>5)&511;
//	h=((addr>>5)^(addr>>14))&511;
//	h=((addr>>5)^(addr>>13))&255;
	h=((addr>>5)^(addr>>13))&1023;

//	ctx->mem_cyc+=3;
	ctx->mem_cyc+=2;
	ctx->mem_cnt_l1++;

#if 0
	if((addr>>4)==ctx->mem_l1h4k[(h<<2)|0])
		return(0);
	if((addr>>4)==ctx->mem_l1h4k[(h<<2)|1])
		return(0);
	if((addr>>4)==ctx->mem_l1h4k[(h<<2)|2])
		return(0);
	if((addr>>4)==ctx->mem_l1h4k[(h<<2)|3])
		return(0);
#endif

//	if((addr>>4)==ctx->mem_l1h4k[(h<<2)|0])
//		return(0);

	if((addr>>5)==ctx->mem_l1h4k[(h<<2)|0])
		return(0);

#if 0
//	if((addr>>4)==ctx->mem_l1h4k[(h<<2)|1])
	if((addr>>5)==ctx->mem_l1h4k[(h<<2)|1])
	{
//		t0=ctx->mem_l1h4k[(h<<2)|0];
//		t1=ctx->mem_l1h4k[(h<<2)|1];
//		ctx->mem_l1h4k[(h<<2)|0]=t1;
//		ctx->mem_l1h4k[(h<<2)|1]=t0;
		return(0);
	}
#endif
	
#if 0
//	if((addr>>4)==ctx->mem_l1h4k[(h<<2)|2])
	if((addr>>5)==ctx->mem_l1h4k[(h<<2)|2])
	{
		t0=ctx->mem_l1h4k[(h<<2)|1];
		t1=ctx->mem_l1h4k[(h<<2)|2];
		ctx->mem_l1h4k[(h<<2)|1]=t1;
		ctx->mem_l1h4k[(h<<2)|2]=t0;
		return(0);
	}
//	if((addr>>4)==ctx->mem_l1h4k[(h<<2)|3])
	if((addr>>5)==ctx->mem_l1h4k[(h<<2)|3])
	{
		t0=ctx->mem_l1h4k[(h<<2)|2];
		t1=ctx->mem_l1h4k[(h<<2)|3];
		ctx->mem_l1h4k[(h<<2)|2]=t1;
		ctx->mem_l1h4k[(h<<2)|3]=t0;
		return(0);
	}
#endif

//	p1=5;
//	p2=24;

//	p1=7;
//	p2=36;

//	p1=9;
//	p2=45;

//	p1=9;
//	p2=85;

//	p1=11;
//	p2=122;

	p1=9;
	p2=94;


	ctx->miss_cyc+=p1;
	ctx->miss_cyc_l1+=p1;
	ctx->miss_cnt_l1++;

	if(ctx->tr_cur)
		ctx->tr_cur->acc_pencyc+=p1;

//	ctx->mem_l1h4k[(h<<2)|3]=ctx->mem_l1h4k[(h<<2)|2];
//	ctx->mem_l1h4k[(h<<2)|2]=ctx->mem_l1h4k[(h<<2)|1];
	ctx->mem_l1h4k[(h<<2)|1]=ctx->mem_l1h4k[(h<<2)|0];
//	ctx->mem_l1h4k[(h<<2)|0]=addr>>4;

	ctx->mem_l1h4k[(h<<2)|0]=addr>>5;

	/* L2 Cache */

	return(BJX2_MemSimAddrL2(ctx, addr));

#if 0
	ctx->mem_cnt_l2++;

// #if 0  //Disable L2
#if 1  //Enable L2

//	h=(addr>>4)&1023;
//	h=(addr>>4)&2047;
//	h=(addr>>4)&4095;
//	h=((addr>>4)^(addr>>14))&4095;
//	h=((addr>>4)^(addr>>16))&4095;
//	h=(addr>>4)&8191;
//	h=((addr>>4)^(addr>>17))&8191;
//	h=((addr>>4)^(addr>>17))&4095;
	h=((addr>>6)^(addr>>19))&4095;

//	if((addr>>4)==ctx->mem_l2h32k[(h<<1)|0])
	if((addr>>6)==ctx->mem_l2h32k[(h<<1)|0])
		return(0);
	if((addr>>6)==ctx->mem_l2h32k[(h<<1)|1])
		return(0);

#if 0
	if((addr>>4)==ctx->mem_l2h32k[(h<<1)|1])
	{
		t0=ctx->mem_l2h32k[(h<<1)|0];
		t1=ctx->mem_l2h32k[(h<<1)|1];
		ctx->mem_l2h32k[(h<<1)|0]=t1;
		ctx->mem_l2h32k[(h<<1)|1]=t0;
		return(0);
	}
#endif

	ctx->mem_l2h32k[(h<<1)|1]=ctx->mem_l2h32k[(h<<1)|0];
//	ctx->mem_l2h32k[(h<<1)|0]=addr>>4;
	ctx->mem_l2h32k[(h<<1)|0]=addr>>6;
#endif

	/* Main RAM */

	ctx->miss_cyc+=p2;
	ctx->miss_cyc_l2+=p2;
	ctx->miss_cnt_l2++;
	ctx->mem_cnt_drd++;

	if(ctx->tr_cur)
		ctx->tr_cur->acc_pencyc+=p2;

#endif

#if 0

#if 0
	if((addr>>3)==ctx->mem_l1addr1)
		return(0);
	if((addr>>3)==ctx->mem_l1addr2)
		return(0);
	if((addr>>3)==ctx->mem_l1addr3)
		return(0);
	if((addr>>3)==ctx->mem_l1addr4)
		return(0);
#endif

#if 1
	if((addr>>4)==ctx->mem_l1addr1)
		return(0);
	if((addr>>4)==ctx->mem_l1addr2)
		return(0);
	if((addr>>4)==ctx->mem_l1addr3)
		return(0);
	if((addr>>4)==ctx->mem_l1addr4)
		return(0);
#endif

	ctx->miss_cyc+=3;
	if(ctx->tr_cur)
		ctx->tr_cur->acc_pencyc+=3;

	ctx->mem_l1addr4=ctx->mem_l1addr3;
	ctx->mem_l1addr3=ctx->mem_l1addr2;
	ctx->mem_l1addr2=ctx->mem_l1addr1;
//	ctx->mem_l1addr1=addr>>3;
	ctx->mem_l1addr1=addr>>4;
#endif

	return(1);
#endif

}
#endif



#if 1
int BJX2_MemSimAddrL1(BJX2_Context *ctx, bjx2_addr addr, int opm)
{
	static int cnt;
	bjx2_addr t0, t1, a0, a1;
	int ax0, ax1, ax2, ax3;
	int ax4, ax5, ix0, ix1, miss0, miss1, miss;
	int skip0, skip1;
	int h, p, p1, p2;
	
	if(ctx->use_jit || ctx->no_memcost)
		return(0);
	
	if(((addr>>28)&0xF)==0xF)
	{
		p1=15;
//		p1=25;
//		p1=35;

		ctx->mem_cyc+=2;

		ctx->miss_cyc+=p1;
		ctx->mem_cyc_mmio+=p1;

		if(ctx->tr_cur)
			ctx->tr_cur->acc_pencyc+=p1;

		return(0);
	}
	
	a0=addr;
	a1=a0+(1<<(opm&3));
	
	skip0=0;
	skip1=0;
	if((a0&(~15))==(a1&(~15)))
	{
		if(addr&16)
			skip0=1;
		else
			skip1=1;
	}
	
	if(addr&16)
	{
		ax1=addr&(~15);
		ax0=ax1+16;
	}else
	{
		ax0=addr&(~15);
		ax1=ax0+16;
	}

//	ix0=((ax0>>5)^(ax0>>13))&511;
//	ix1=((ax1>>5)^(ax1>>13))&511;

//	ix0=(ax0>>5)&511;
//	ix1=(ax1>>5)&511;

	ix0=(ax0>>5)&1023;
	ix1=(ax1>>5)&1023;

//	h=((addr>>5)^(addr>>13))&1023;

	cnt++;

	if(opm&0x20)
	{
//		h=(addr*65521)>>16;
//		if((h&1)==0)
//		if((h&3)==0)
		if((cnt&3)==0)
		{
			ctx->mem_cyc+=1;
			ctx->miss_cyc+=1;
			if(ctx->tr_cur)
				ctx->tr_cur->acc_pencyc+=1;
		}
	}

//	ctx->mem_cyc+=2;
	ctx->mem_cyc++;
	ctx->mem_cnt_l1++;
//	ctx->mem_cnt_l1+=2;

//	if((addr>>5)==ctx->mem_l1h4k[(h<<2)|0])
//		return(0);

//	if(ax0==ctx->mem_l1h4k[(ix0<<1)|0])
//		return(0);

	ax2=ctx->mem_l1h4k[(ix0<<2)|0];
	ax3=ctx->mem_l1h4k[(ix1<<2)|1];
//	miss0=(ax0!=ax2);
//	miss1=(ax1!=ax3);
	miss0=(ax0!=(ax2&(~15)));
	miss1=(ax1!=(ax3&(~15)));


#if 0
	/* What if, victim buffer? */

	ax4=0;
	ax5=0;

#if 1
	if(ax0==ctx->mem_l1addr1)	ax4=ctx->mem_l1addr1;
	if(ax0==ctx->mem_l1addr2)	ax4=ctx->mem_l1addr2;
	if(ax0==ctx->mem_l1addr3)	ax4=ctx->mem_l1addr3;
	if(ax0==ctx->mem_l1addr4)	ax4=ctx->mem_l1addr4;

	if(ax1==ctx->mem_l1addr1)	ax5=ctx->mem_l1addr1;
	if(ax1==ctx->mem_l1addr2)	ax5=ctx->mem_l1addr2;
	if(ax1==ctx->mem_l1addr3)	ax5=ctx->mem_l1addr3;
	if(ax1==ctx->mem_l1addr4)	ax5=ctx->mem_l1addr4;
#endif

	if(miss0)
	{
		ctx->mem_l1addr4=ctx->mem_l1addr3;
		ctx->mem_l1addr3=ctx->mem_l1addr2;
		ctx->mem_l1addr2=ctx->mem_l1addr1;
		ctx->mem_l1addr1=ax2&(~15);
	}

	if(miss1)
	{
		ctx->mem_l1addr4=ctx->mem_l1addr3;
		ctx->mem_l1addr3=ctx->mem_l1addr2;
		ctx->mem_l1addr2=ctx->mem_l1addr1;
		ctx->mem_l1addr1=ax3&(~15);
	}

	if(miss0 && ax4 && !(ax2&1))
	{
		ctx->mem_l1h4k[(ix0<<2)|0]=ax4;
		miss0=0;
	}

	if(miss1 && ax5 && !(ax3&1))
	{
		ctx->mem_l1h4k[(ix1<<2)|1]=ax5;
		miss0=1;
	}
#endif


	miss = (miss0 && !skip0) || (miss1 && !skip1);
	
	if(!miss)
	{
		if(opm&0x20)
		{
			if(!skip0)
//				ctx->mem_l1h4k[(ix0<<2)|2]|=1;
				ctx->mem_l1h4k[(ix0<<2)|0]|=1;
			if(!skip1)
//				ctx->mem_l1h4k[(ix1<<2)|3]|=1;
				ctx->mem_l1h4k[(ix1<<2)|1]|=1;
		}
		return(0);
	}

	p1=3;
//	p1=4;
//	p1=5;
//	p1=7;
//	p1=9;
//	p2=94;

	p=p1;

	if(opm&0x20)
	{
		ax0|=1;
		ax1|=1;
	}


	if(miss0)
	{
		if(ctx->mem_l1h4k[(ix0<<2)|0]&1)
		{
			BJX2_MemSimAddrL2(ctx, ax2, 1);
			p++;
		}

		if(opm&0x20)
		{
			BJX2_MemSimAddrL2(ctx, ax0, 2);
		}else
		{
			BJX2_MemSimAddrL2(ctx, ax0, 0);
		}
		p++;

		ctx->mem_l1h4k[(ix0<<2)|0]=ax0;
	}

	if(miss1)
	{
		if(ctx->mem_l1h4k[(ix0<<2)|1]&1)
		{
			BJX2_MemSimAddrL2(ctx, ax3, 1);
			p++;
		}

		if(opm&0x20)
		{
			BJX2_MemSimAddrL2(ctx, ax1, 2);
		}else
		{
			BJX2_MemSimAddrL2(ctx, ax1, 0);
		}
		p++;

		ctx->mem_l1h4k[(ix1<<2)|1]=ax1;
	}

	ctx->miss_cyc+=p;
	ctx->miss_cyc_l1+=p;
	ctx->miss_cnt_l1++;
	if(ctx->tr_cur)
		ctx->tr_cur->acc_pencyc+=p;

	return(1);
}
#endif

int BJX2_MemTlbCheckAccess(BJX2_Context *ctx, int acc, int pgbits,
	u64 krr, u32 kra)
{
	u64 acl;
	int noacc, vugid, vugid2, ugm;
	int i, j, k;
	
	if(!acc)
		return(0);

	noacc=0;

	if(pgbits&0x10)
		noacc|=1;
	if(pgbits&0x20)
		noacc|=2;
	if(pgbits&0x40)
		noacc|=4;

	if((pgbits&0x80) && !(ctx->regs[BJX2_REG_SR]&(1<<30)))
		noacc|=7;

	if((pgbits&0x08) && !(pgbits&0x80))
	{
		if(ctx->regs[BJX2_REG_SR]&(1<<30))
			noacc&=~3;
//		noacc|=1;
	}

	ugm=0;
	if(((u16)krr)!=0)
	{
		ugm=0;

		if(((kra&7)==2) || ((kra&7)==6) ||
			((kra&7)==3) || ((kra&7)==7))
		{
			vugid=kra>>16;
			for(i=0; i<4; i++)
			{
				vugid2=(u16)(krr>>(i*16));
				for(j=0; j<4; j++)
				{
					acl=ctx->mem_tlb_acl[j];
					if(	(((acl>> 0)&0xFFFF)==vugid ) &&
						(((acl>>16)&0xFFFF)==vugid2) &&
						((acl>>32)&7))
							break;
				}
				if(j<4)
					break;
			}
			
			if(i<4)
			{
				ugm=3;
				noacc|=(~(acl>>(32+3)))&7;
			}
		}
	
		if(ugm<=0)
		{
			ugm=1;
			vugid=kra>>16;
			for(i=0; i<4; i++)
			{
				vugid2=(u16)(krr>>(i*16));
				if(!vugid2)
					continue;
				if(vugid2==vugid)
					{ ugm=3; break; }
				if(((vugid2&0xFC00)==(vugid&0xFC00)) && !(kra&4))
					ugm=2;
				if(((vugid2&0x03FF)==(vugid&0x03FF)) &&  (kra&4))
					ugm=2;
			}

			switch(kra&7)
			{
			case 0:		case 4:
				noacc|=7;
				break;
			case 1:		case 5:
				j=kra>>(12-(ugm*3));
				noacc|=(~j)&7;
				break;
			case 2:		case 6:
				BJX2_ThrowFaultStatus(ctx, BJX2_FLT_TLBACL);
				break;
			case 3:		case 7:
				if(ugm<=1)
				{
					BJX2_ThrowFaultStatus(ctx, BJX2_FLT_TLBACL);
					break;
				}
				j=kra>>(12-(ugm*3));
				noacc|=(~j)&7;
	//			noacc|=16;
				break;
			}
		}
	}

	if((acc&noacc)&1)
	{
//		if(noacc&16)
//			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_TLBACL);
//		else
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INV_MRD);
		return(-1);
	}
	if((acc&noacc)&2)
	{
//		if(noacc&16)
//			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_TLBACL);
//		else
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INV_MWR);
		return(-1);
	}
	if((acc&noacc)&4)
	{
//		if(noacc&16)
//			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_TLBACL);
//		else
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INV_MEX);
		return(-1);
	}

	return(0);
}

bjx2_addr BJX2_MemTranslateTlb(BJX2_Context *ctx, bjx2_addr addr, int acc)
{
	bjx2_addr addr1;
	u64 tlbhi2, tlblo2;
	u64 tlbhi0, tlblo0;
	u64 tlbhi1, tlblo1;
	u64	krr;
	int i, j, k, h;

	if(ctx->regs[BJX2_REG_SR]&(1<<31))
		addr=(addr&0x0000FFFFFFFFFFFFULL);
	else
		addr=(addr&0x00000000FFFFFFFFULL);


	if(!(ctx->regs[BJX2_REG_MMCR]&1))
		return(addr);

	if(ctx->status)
		return(addr);

//	if(ctx->regs[BJX2_REG_SR]&(1<<29))
	if(	(ctx->regs[BJX2_REG_SR]&(1<<29)) &&
		(ctx->regs[BJX2_REG_SR]&(1<<28)))
		return(addr);

	krr=ctx->regs[BJX2_REG_KRR];

	if(((addr>>47)&1) && !(ctx->regs[BJX2_REG_SR]&(1<<30)))
	{
		if(acc&1)
		{
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INV_MRD);
			return(addr);
		}
		if(acc&2)
		{
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INV_MWR);
			return(addr);
		}
		if(acc&4)
		{
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INV_MEX);
			return(addr);
		}
//		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
	}


#if 1
//	if((addr>>32)==0)
	if(((addr>>32)&0xFFFF)==0x0000)
	{
//		if(((addr>>28)&15)==0xF)
		if((((addr>>28)&15)==0xF) && !(ctx->regs[BJX2_REG_SR]&(1<<31)))
		{
			return(addr);
		}
	}
	if(((addr>>32)&0xFFFF)==0xFFFF)
	{
		if(((addr>>28)&15)==0xF)
		{
			return(addr);
		}
	}
	if(((addr>>32)&0xFFFF)==0xF000)
	{
	}

	if(((addr>>32)&0xFFFF)==0xC000)
	{
		return(addr);
	}

	if(((addr>>32)&0xFFFF)==0xD000)
	{
		return(addr);
	}
#endif

#if 1
	if(ctx->regs[BJX2_REG_MMCR]&32)
	{
		h=((addr>>24)&0x3F)^((addr>>16)&0xFF)^((addr>>8)&0xC0);
		for(i=0; i<4; i++)
		{
			tlbhi2=ctx->mem_tlb_hi[h*4+i];
			tlblo2=ctx->mem_tlb_lo[h*4+i];
			
			if(!(tlblo2&1))
				continue;

			if(	(tlbhi2&0x0000FFFFFFFFC000ULL) ==
				(addr  &0x0000FFFFFFFFC000ULL))
			{
//				ctx->mem_tlb_pr0_hi=tlbhi2;
//				ctx->mem_tlb_pr0_lo=tlblo2;
//				ctx->mem_tlb_pr1_hi=tlbhi0;
//				ctx->mem_tlb_pr1_lo=tlblo0;
				addr1=(tlblo2&0x0000FFFFFFFFC000ULL)|(addr&0x00003FFFULL);
				
				if(addr!=addr1)
				{
					printf("BJX2_MemTranslateTlb %llX -> %llX\n",
						addr, addr1);
//					JX2_DBGBREAK
				}

#if 1
				for(j=i; j>0; j--)
				{
					tlbhi1=ctx->mem_tlb_hi[h*4+(j-1)];
					tlblo1=ctx->mem_tlb_lo[h*4+(j-1)];
					ctx->mem_tlb_hi[h*4+j]=tlbhi1;
					ctx->mem_tlb_lo[h*4+j]=tlblo1;
				}
				ctx->mem_tlb_hi[h*4+0]=tlbhi2;
				ctx->mem_tlb_lo[h*4+0]=tlblo2;
#endif
				
				BJX2_MemTlbCheckAccess(ctx, acc, tlblo2&255, krr,
					(tlbhi2&0x0000FFFFU)|((tlbhi2>>32)&0xFFFF0000U));
				
				return(addr1);
			}
		}
	}else
		if(ctx->regs[BJX2_REG_MMCR]&16)
	{
//		h=((addr>>22)&1)^((addr>>16)&1);
//		h=((addr>>22)&3)^((addr>>16)&3);
//		h=((addr>>22)&7)^((addr>>16)&7);
//		h=((addr>>22)&15)^((addr>>16)&15);
//		h=((addr>>22)&31)^((addr>>16)&31);
//		h=((addr>>22)&63)^((addr>>16)&63);
//		h=((addr>>24)&127)^((addr>>16)&127);
		h=((addr>>24)&255)^((addr>>16)&255);
		for(i=0; i<4; i++)
		{
			tlbhi2=ctx->mem_tlb_hi[h*4+i];
			tlblo2=ctx->mem_tlb_lo[h*4+i];
			
			if(!(tlblo2&1))
				continue;

			if(	(tlbhi2&0x0000FFFFFFFF0000ULL) ==
				(addr  &0x0000FFFFFFFF0000ULL))
			{
//				ctx->mem_tlb_pr0_hi=tlbhi2;
//				ctx->mem_tlb_pr0_lo=tlblo2;
//				ctx->mem_tlb_pr1_hi=tlbhi0;
//				ctx->mem_tlb_pr1_lo=tlblo0;
				addr1=(tlblo2&0x0000FFFFFFFF0000ULL)|(addr&0x0000FFFFULL);
				
				if(addr!=addr1)
				{
//					JX2_DBGBREAK
				}

#if 1
				for(j=i; j>0; j--)
				{
					tlbhi1=ctx->mem_tlb_hi[h*4+(j-1)];
					tlblo1=ctx->mem_tlb_lo[h*4+(j-1)];
					ctx->mem_tlb_hi[h*4+j]=tlbhi1;
					ctx->mem_tlb_lo[h*4+j]=tlblo1;
				}
				ctx->mem_tlb_hi[h*4+0]=tlbhi2;
				ctx->mem_tlb_lo[h*4+0]=tlblo2;
#endif

				BJX2_MemTlbCheckAccess(ctx, acc, tlblo2&255, krr,
					(tlbhi2&0x0000FFFFU)|((tlbhi2>>32)&0xFFFF0000U));
				
				return(addr1);
			}
		}
	}else
	{
		h=((addr>>24)&0x0F)^((addr>>16)&0xFF)^((addr>>8)&0xF0);

//		h=((addr>>12)&63)^((addr>>16)&63);
//		h=((addr>>12)&255)^((addr>>16)&255);
//		h=((addr>>12)&127)^((addr>>16)&127);
//		h=((addr>>12)&63)^((addr>>16)&63);
//		h=((addr>>12)&31)^((addr>>16)&31);
//		h=((addr>>12)&15)^((addr>>16)&15);
//		h=((addr>>12)& 7)^((addr>>16)& 7);
		for(i=0; i<4; i++)
		{
			tlbhi2=ctx->mem_tlb_hi[h*4+i];
			tlblo2=ctx->mem_tlb_lo[h*4+i];

			if(	(tlbhi2&0x0000FFFFFFFFF000ULL) ==
				(addr  &0x0000FFFFFFFFF000ULL))
			{
//				ctx->mem_tlb_pr0_hi=tlbhi2;
//				ctx->mem_tlb_pr0_lo=tlblo2;
//				ctx->mem_tlb_pr1_hi=tlbhi0;
//				ctx->mem_tlb_pr1_lo=tlblo0;
				addr1=(tlblo2&0x0000FFFFFFFFF000ULL)|(addr&0x00000FFFULL);

#if 1
				for(j=i; j>0; j--)
				{
					tlbhi1=ctx->mem_tlb_hi[h*4+(j-1)];
					tlblo1=ctx->mem_tlb_lo[h*4+(j-1)];
					ctx->mem_tlb_hi[h*4+j]=tlbhi1;
					ctx->mem_tlb_lo[h*4+j]=tlblo1;
				}
				ctx->mem_tlb_hi[h*4+0]=tlbhi2;
				ctx->mem_tlb_lo[h*4+0]=tlblo2;
#endif

				BJX2_MemTlbCheckAccess(ctx, acc, tlblo2&255, krr,
					(tlbhi2&0x0000FFFFU)|((tlbhi2>>32)&0xFFFF0000U));

				return(addr1);
			}
		}
	}

	if(!(ctx->status))
	{
		ctx->tot_cnt_tlbmiss++;
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_TLBMISS);
	}else
	{
//		JX2_DBGBREAK
	}
	return(addr);
#endif

#if 0
	k=(addr>>28)&0xFFFFFFFE;
	if((k>=0x00008) && (k<=0x0000F))
	{
		if(k==0x8)
		{
			return(BJX2_MemTranslateTlb(ctx,
//				(-1LL<<32)|(addr&0x1FFFFFFF)));
				(~((1LL<<32)-1))|(addr&0x1FFFFFFF)));
		}
		if(k==0xA)
			return(addr);
		if(k==0xC)
			return(addr&0x1FFFFFFF);
		if(k==0xE)
			return(addr);
	}
#endif

#if 0
	tlbhi0=ctx->mem_tlb_pr0_hi;
	tlblo0=ctx->mem_tlb_pr0_lo;
	if(	(tlbhi0&0x0000FFFFFFFFF000ULL) ==
		(addr  &0x0000FFFFFFFFF000ULL))
	{
		addr1=(tlblo0&0x0000FFFFFFFFF000ULL)|(addr&0x0FFFULL);
		return(addr1);
	}

	tlbhi1=ctx->mem_tlb_pr1_hi;
	tlblo1=ctx->mem_tlb_pr1_lo;
	if(	(tlbhi1&0x0000FFFFFFFFF000ULL) ==
		(addr  &0x0000FFFFFFFFF000ULL))
	{
		ctx->mem_tlb_pr0_hi=tlbhi1;
		ctx->mem_tlb_pr0_lo=tlblo1;
		ctx->mem_tlb_pr1_hi=tlbhi0;
		ctx->mem_tlb_pr1_lo=tlblo0;
		addr1=(tlblo1&0x0000FFFFFFFFF000ULL)|(addr&0x0FFFULL);
		return(addr1);
	}

	for(i=0; i<64; i++)
	{
		tlbhi2=ctx->mem_tlb_hi[i];
		tlblo2=ctx->mem_tlb_lo[i];

		if(	(tlbhi2&0x0000FFFFFFFFF000ULL) ==
			(addr  &0x0000FFFFFFFFF000ULL))
		{
			ctx->mem_tlb_pr0_hi=tlbhi2;
			ctx->mem_tlb_pr0_lo=tlblo2;
			ctx->mem_tlb_pr1_hi=tlbhi0;
			ctx->mem_tlb_pr1_lo=tlblo0;
			addr1=(tlblo2&0x0000FFFFFFFFF000ULL)|(addr&0x0FFFULL);
			return(addr1);
		}
	}

	if(!(ctx->status))
	{
		ctx->regs[BJX2_REG_PC]=ctx->trapc;
		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_TLBMISS);
	}else
	{
		JX2_DBGBREAK
	}
	return(addr);
#endif

//	return(addr);
}

int BJX2_MemGetByte_Dfl(BJX2_Context *ctx, bjx2_addr addr0)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra;

	addr=BJX2_MemTranslateTlb(ctx, addr0, 1);
	if(ctx->status)
		return(0);

	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		BJX2_MemSimAddrL1(ctx, addr, 0x10);

		if(sp->simple_mem)
		{
//			ra=addr-sp->addr_base;
			ra=(addr-sp->modbase)&(sp->modmask);
			return(BJX2_PtrGetSByteOfsLe(sp->data, ra));
		}
		return(sp->GetByte(ctx, sp, addr));
	}

	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		if(!(ctx->status))
		{
			ctx->regs[BJX2_REG_PC]=ctx->trapc;
			ctx->regs[BJX2_REG_TEA]=addr;
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		}else
		{
			JX2_DBGBREAK
		}
		return(0);
	}
	
	BJX2_MemSimAddrL1(ctx, addr, 0x10);

#if 1
	if(sp->simple_mem)
	{
		if(!(ctx->regs[BJX2_REG_MMCR]&1))
		{
			ctx->psp_pbase=sp->addr_base;
			ctx->psp_prng3=sp->addr_sz-3;
			ctx->psp_pdata=sp->data;
		}

//		ra=addr-sp->addr_base;
		ra=(addr-sp->modbase)&(sp->modmask);
		return(BJX2_PtrGetSByteOfsLe(sp->data, ra));
	}
#endif

	return(sp->GetByte(ctx, sp, addr));
}

int BJX2_MemGetWord_Dfl(BJX2_Context *ctx, bjx2_addr addr0)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra;

	addr=BJX2_MemTranslateTlb(ctx, addr0, 1);
	if(ctx->status)
		return(0);

	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		BJX2_MemSimAddrL1(ctx, addr, 0x11);

		if(sp->simple_mem)
		{
//			ra=addr-sp->addr_base;
			ra=(addr-sp->modbase)&(sp->modmask);
			return(BJX2_PtrGetSWordOfsLe(sp->data, ra));
		}
		return(sp->GetWord(ctx, sp, addr));
	}

	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		if(!(ctx->status))
		{
			ctx->regs[BJX2_REG_PC]=ctx->trapc;
			ctx->regs[BJX2_REG_TEA]=addr;
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		}else
		{
			JX2_DBGBREAK
		}
		return(0);
	}

	BJX2_MemSimAddrL1(ctx, addr, 0x11);

#if 1
	if(sp->simple_mem)
	{
		if(!(ctx->regs[BJX2_REG_MMCR]&1))
		{
			ctx->psp_pbase=sp->addr_base;
			ctx->psp_prng3=sp->addr_sz-3;
			ctx->psp_pdata=sp->data;
		}

//		ra=addr-sp->addr_base;
		ra=(addr-sp->modbase)&(sp->modmask);
		return(BJX2_PtrGetSWordOfsLe(sp->data, ra));
	}
#endif

	return(sp->GetWord(ctx, sp, addr));
}

s32 BJX2_MemGetDWord_Dfl(BJX2_Context *ctx, bjx2_addr addr0)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra;

	addr=BJX2_MemTranslateTlb(ctx, addr0, 1);
	if(ctx->status)
		return(0);

	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		BJX2_MemSimAddrL1(ctx, addr, 0x12);

		if(sp->simple_mem)
		{
//			ra=addr-sp->addr_base;
			ra=(addr-sp->modbase)&(sp->modmask);
			return(BJX2_PtrGetSDWordOfsLe(sp->data, ra));
		}
		return(sp->GetDWord(ctx, sp, addr));
	}

	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		if(!(ctx->status))
		{
			ctx->regs[BJX2_REG_PC]=ctx->trapc;
			ctx->regs[BJX2_REG_TEA]=addr;
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		}else
		{
			JX2_DBGBREAK
		}
		return(0);
	}

	BJX2_MemSimAddrL1(ctx, addr, 0x12);

#if 1
	if(sp->simple_mem)
	{
		if(!(ctx->regs[BJX2_REG_MMCR]&1))
		{
			ctx->psp_pbase=sp->addr_base;
			ctx->psp_prng3=sp->addr_sz-3;
			ctx->psp_pdata=sp->data;
		}

//		ra=addr-sp->addr_base;
		ra=(addr-sp->modbase)&(sp->modmask);
		return(BJX2_PtrGetSDWordOfsLe(sp->data, ra));
	}
#endif

	return(sp->GetDWord(ctx, sp, addr));
}

s64 BJX2_MemGetQWord_Dfl(BJX2_Context *ctx, bjx2_addr addr0)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	s64 t;
	int ra;

	addr=BJX2_MemTranslateTlb(ctx, addr0, 1);
	if(ctx->status)
		return(0);

	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		BJX2_MemSimAddrL1(ctx, addr, 0x13);

		if(sp->simple_mem)
		{
//			ra=addr-sp->addr_base;
			ra=(addr-sp->modbase)&(sp->modmask);
			return(BJX2_PtrGetSQWordOfsLe(sp->data, ra));
		}
		return(sp->GetQWord(ctx, sp, addr));
	}

	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		if(!(ctx->status))
		{
			ctx->regs[BJX2_REG_PC]=ctx->trapc;
			ctx->regs[BJX2_REG_TEA]=addr;
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		}else
		{
			JX2_DBGBREAK
		}
		return(0);
	}

	BJX2_MemSimAddrL1(ctx, addr, 0x13);

#if 1
	if(sp->simple_mem)
	{
		if(!(ctx->regs[BJX2_REG_MMCR]&1))
		{
			ctx->psp_pbase=sp->addr_base;
			ctx->psp_prng3=sp->addr_sz-3;
			ctx->psp_pdata=sp->data;
		}

//		ra=addr-sp->addr_base;
		ra=(addr-sp->modbase)&(sp->modmask);
		return(BJX2_PtrGetSQWordOfsLe(sp->data, ra));
	}
#endif

	t=sp->GetQWord(ctx, sp, addr);
//	t=sp->GetDWord(ctx, sp, addr+4);
//	t=(t<<32)|((u32)(sp->GetDWord(ctx, sp, addr+0)));
	return(t);
}

int BJX2_MemGetXWord_Dfl(BJX2_Context *ctx, bjx2_addr addr0,
	u64 *rvlo, u64 *rvhi)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	s64 t;
	int ra;

	addr=BJX2_MemTranslateTlb(ctx, addr0, 1);
	if(ctx->status)
		return(0);

	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		BJX2_MemSimAddrL1(ctx, addr, 0x17);

		if(sp->simple_mem)
		{
//			ra=addr-sp->addr_base;
			ra=(addr-sp->modbase)&(sp->modmask);
			*rvlo=BJX2_PtrGetSQWordOfsLe(sp->data, ra+0);
			*rvhi=BJX2_PtrGetSQWordOfsLe(sp->data, ra+8);
			return(0);
		}
		return(sp->GetXWord(ctx, sp, addr, rvlo, rvhi));
	}

	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		if(!(ctx->status))
		{
			ctx->regs[BJX2_REG_PC]=ctx->trapc;
			ctx->regs[BJX2_REG_TEA]=addr;
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		}else
		{
			JX2_DBGBREAK
		}
		return(0);
	}

	BJX2_MemSimAddrL1(ctx, addr, 0x17);

#if 1
	if(sp->simple_mem)
	{
		if(!(ctx->regs[BJX2_REG_MMCR]&1))
		{
			ctx->psp_pbase=sp->addr_base;
			ctx->psp_prng3=sp->addr_sz-3;
			ctx->psp_pdata=sp->data;
		}

//		ra=addr-sp->addr_base;
		ra=(addr-sp->modbase)&(sp->modmask);
		*rvlo=BJX2_PtrGetSQWordOfsLe(sp->data, ra+0);
		*rvhi=BJX2_PtrGetSQWordOfsLe(sp->data, ra+8);
		return(0);
	}
#endif

	t=sp->GetXWord(ctx, sp, addr, rvlo, rvhi);
//	t=sp->GetDWord(ctx, sp, addr+4);
//	t=(t<<32)|((u32)(sp->GetDWord(ctx, sp, addr+0)));
	return(t);
}


int BJX2_MemGetByte_NoAT(BJX2_Context *ctx, bjx2_addr addr0)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra;

	addr=(u32)addr0;
	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		if(sp->simple_mem)
		{
//			ra=addr-sp->addr_base;
			ra=(addr-sp->modbase)&(sp->modmask);
			return(BJX2_PtrGetSByteOfsLe(sp->data, ra));
		}
		return(sp->GetByte(ctx, sp, addr));
	}

//	sp=BJX2_MemSpanForAddr(ctx, addr);
	sp=ctx->MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		if(!(ctx->status))
		{
			ctx->regs[BJX2_REG_PC]=ctx->trapc;
			ctx->regs[BJX2_REG_TEA]=addr;
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		}else
		{
			JX2_DBGBREAK
		}
		return(0);
	}
	
	if(sp->simple_mem)
	{
//		if(!(ctx->regs[BJX2_REG_MMCR]&1))
		if(1)
		{
			ctx->psp_pbase=sp->addr_base;
			ctx->psp_prng3=sp->addr_sz-3;
			ctx->psp_pdata=sp->data;
		}

//		ra=addr-sp->addr_base;
		ra=(addr-sp->modbase)&(sp->modmask);
		return(BJX2_PtrGetSByteOfsLe(sp->data, ra));
	}

	return(sp->GetByte(ctx, sp, addr));
}

int BJX2_MemGetWord_NoAT(BJX2_Context *ctx, bjx2_addr addr0)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra;

	addr=(u32)addr0;
	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		if(sp->simple_mem)
		{
//			ra=addr-sp->addr_base;
			ra=(addr-sp->modbase)&(sp->modmask);
			return(BJX2_PtrGetSWordOfsLe(sp->data, ra));
		}
		return(sp->GetWord(ctx, sp, addr));
	}

//	sp=BJX2_MemSpanForAddr(ctx, addr);
	sp=ctx->MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		if(!(ctx->status))
		{
			ctx->regs[BJX2_REG_PC]=ctx->trapc;
			ctx->regs[BJX2_REG_TEA]=addr;
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		}else
		{
			JX2_DBGBREAK
		}
		return(0);
	}

	if(sp->simple_mem)
	{
//		if(!(ctx->regs[BJX2_REG_MMCR]&1))
		if(1)
		{
			ctx->psp_pbase=sp->addr_base;
			ctx->psp_prng3=sp->addr_sz-3;
			ctx->psp_pdata=sp->data;
		}

//		ra=addr-sp->addr_base;
		ra=(addr-sp->modbase)&(sp->modmask);
		return(BJX2_PtrGetSWordOfsLe(sp->data, ra));
	}

	return(sp->GetWord(ctx, sp, addr));
}

s32 BJX2_MemGetDWord_NoAT(BJX2_Context *ctx, bjx2_addr addr0)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra;

	addr=(u32)addr0;
	sp=ctx->span_pr0;
	if(sp && (((u32)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		if(sp->simple_mem)
		{
//			ra=addr-sp->addr_base;
			ra=(addr-sp->modbase)&(sp->modmask);
			return(BJX2_PtrGetSDWordOfsLe(sp->data, ra));
		}
		return(sp->GetDWord(ctx, sp, addr));
	}

//	sp=BJX2_MemSpanForAddr(ctx, addr);
	sp=ctx->MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		if(!(ctx->status))
		{
			ctx->regs[BJX2_REG_PC]=ctx->trapc;
			ctx->regs[BJX2_REG_TEA]=addr;
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		}else
		{
			JX2_DBGBREAK
		}
		return(0);
	}

	if(sp->simple_mem)
	{
//		if(!(ctx->regs[BJX2_REG_MMCR]&1))
		if(1)
		{
			ctx->psp_pbase=sp->addr_base;
			ctx->psp_prng3=sp->addr_sz-3;
			ctx->psp_pdata=sp->data;
		}

//		ra=addr-sp->addr_base;
		ra=(addr-sp->modbase)&(sp->modmask);
		return(BJX2_PtrGetSDWordOfsLe(sp->data, ra));
	}

	return(sp->GetDWord(ctx, sp, addr));
}

s64 BJX2_MemGetQWord_NoAT(BJX2_Context *ctx, bjx2_addr addr0)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	s64 t;
	int ra;

	addr=(u32)addr0;

	sp=ctx->span_pr0;
	if(sp && (((u32)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		if(sp->simple_mem)
		{
//			ra=addr-sp->addr_base;
			ra=(addr-sp->modbase)&(sp->modmask);
			return(BJX2_PtrGetSQWordOfsLe(sp->data, ra));
		}
		return(sp->GetDWord(ctx, sp, addr));
	}

//	sp=BJX2_MemSpanForAddr(ctx, addr);
	sp=ctx->MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		if(!(ctx->status))
		{
			ctx->regs[BJX2_REG_PC]=ctx->trapc;
			ctx->regs[BJX2_REG_TEA]=addr;
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		}else
		{
			JX2_DBGBREAK
		}
		return(0);
	}

#if 1
	if(sp->simple_mem)
	{
//		if(!(ctx->regs[BJX2_REG_MMCR]&1))
		if(1)
		{
			ctx->psp_pbase=sp->addr_base;
			ctx->psp_prng3=sp->addr_sz-3;
			ctx->psp_pdata=sp->data;
		}

//		ra=addr-sp->addr_base;
		ra=(addr-sp->modbase)&(sp->modmask);
		return(BJX2_PtrGetSQWordOfsLe(sp->data, ra));
	}
#endif

	t=sp->GetQWord(ctx, sp, addr);
//	t=sp->GetDWord(ctx, sp, addr+4);
//	t=(t<<32)|((u32)(sp->GetDWord(ctx, sp, addr+0)));
	return(t);
}


int BJX2_MemSetByte_Dfl(BJX2_Context *ctx, bjx2_addr addr0, int val)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra, ra4;

	addr=BJX2_MemTranslateTlb(ctx, addr0, 2);
	if(ctx->status)
		return(0);

	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		BJX2_MemSimAddrL1(ctx, addr, 0x20);

		if(sp->simple_mem&2)
		{
//			ra=addr-sp->addr_base;
			ra=(addr-sp->modbase)&(sp->modmask);
			BJX2_PtrSetByteOfsLe(sp->data, ra, val);

			ra4=(ra>>4);
//			if(sp->tripwire[ra4>>3]&(1<<(ra4&7)))
			if(sp->tripwire[ra4>>5]&(1<<(ra4&31)))
				{ BJX2_MemRamCb_SetFault(ctx, sp, addr, val); }
			return(0);
		}
		return(sp->SetByte(ctx, sp, addr, val));
	}

	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		if(!(ctx->status))
		{
			ctx->regs[BJX2_REG_PC]=ctx->trapc;
			ctx->regs[BJX2_REG_TEA]=addr;
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		}else
		{
			JX2_DBGBREAK
		}
		return(0);
	}

	BJX2_MemSimAddrL1(ctx, addr, 0x20);

	return(sp->SetByte(ctx, sp, addr, val));
}

int BJX2_MemSetWord_Dfl(BJX2_Context *ctx, bjx2_addr addr0, int val)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra, ra4;

	addr=BJX2_MemTranslateTlb(ctx, addr0, 2);
	if(ctx->status)
		return(0);

	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		BJX2_MemSimAddrL1(ctx, addr, 0x21);

		if(sp->simple_mem&2)
		{
//			ra=addr-sp->addr_base;
			ra=(addr-sp->modbase)&(sp->modmask);
			BJX2_PtrSetWordOfsLe(sp->data, ra, val);

			ra4=(ra>>4);
//			if(sp->tripwire[ra4>>3]&(1<<(ra4&7)))
			if(sp->tripwire[ra4>>5]&(1<<(ra4&31)))
				{ BJX2_MemRamCb_SetFault(ctx, sp, addr, val); }
			return(0);
		}
		return(sp->SetWord(ctx, sp, addr, val));
	}

	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		if(!(ctx->status))
		{
			ctx->regs[BJX2_REG_PC]=ctx->trapc;
			ctx->regs[BJX2_REG_TEA]=addr;
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		}else
		{
			JX2_DBGBREAK
		}
		return(0);
	}

	BJX2_MemSimAddrL1(ctx, addr, 0x21);

#if 1
	if(sp->simple_mem&2)
	{
//		ra=addr-sp->addr_base;
		ra=(addr-sp->modbase)&(sp->modmask);
		BJX2_PtrSetWordOfsLe(sp->data, ra, val);
		return(0);
	}
#endif

	return(sp->SetWord(ctx, sp, addr, val));
}

int BJX2_MemSetDWord_Dfl(BJX2_Context *ctx, bjx2_addr addr0, s32 val)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra, ra4;

	addr=BJX2_MemTranslateTlb(ctx, addr0, 2);
	if(ctx->status)
		return(0);

	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		BJX2_MemSimAddrL1(ctx, addr, 0x22);

		if(sp->simple_mem&2)
		{
//			ra=addr-sp->addr_base;
			ra=(addr-sp->modbase)&(sp->modmask);
			BJX2_PtrSetDWordOfsLe(sp->data, ra, val);

			ra4=(ra>>4);
//			if(sp->tripwire[ra4>>3]&(1<<(ra4&7)))
			if(sp->tripwire[ra4>>5]&(1<<(ra4&31)))
				{ BJX2_MemRamCb_SetFault(ctx, sp, addr, val); }
			return(0);
		}
		return(sp->SetDWord(ctx, sp, addr, val));
	}

	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		if(!(ctx->status))
		{
			ctx->regs[BJX2_REG_PC]=ctx->trapc;
			ctx->regs[BJX2_REG_TEA]=addr;
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		}else
		{
			JX2_DBGBREAK
		}
		return(0);
	}

	BJX2_MemSimAddrL1(ctx, addr, 0x22);

#if 1
	if(sp->simple_mem&2)
	{
//		ra=addr-sp->addr_base;
		ra=(addr-sp->modbase)&(sp->modmask);
		BJX2_PtrSetDWordOfsLe(sp->data, ra, val);
		return(0);
	}
#endif

	return(sp->SetDWord(ctx, sp, addr, val));
}

int BJX2_MemSetQWord_Dfl(BJX2_Context *ctx, bjx2_addr addr0, s64 val)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra, ra4;

	addr=BJX2_MemTranslateTlb(ctx, addr0, 2);
	if(ctx->status)
		return(0);

	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		BJX2_MemSimAddrL1(ctx, addr, 0x23);

		if(sp->simple_mem&2)
		{
//			ra=addr-sp->addr_base;
			ra=(addr-sp->modbase)&(sp->modmask);
			BJX2_PtrSetQWordOfsLe(sp->data, ra, val);

			ra4=(ra>>4);
//			if(sp->tripwire[ra4>>3]&(1<<(ra4&7)))
			if(sp->tripwire[ra4>>5]&(1<<(ra4&31)))
				{ BJX2_MemRamCb_SetFault(ctx, sp, addr, val); }
			return(0);
		}
		return(sp->SetQWord(ctx, sp, addr, val));
	}

	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		if(!(ctx->status))
		{
			ctx->regs[BJX2_REG_PC]=ctx->trapc;
			ctx->regs[BJX2_REG_TEA]=addr;
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		}else
		{
			JX2_DBGBREAK
		}
		return(0);
	}

	BJX2_MemSimAddrL1(ctx, addr, 0x23);

#if 1
	if(sp->simple_mem&2)
	{
//		ra=addr-sp->addr_base;
		ra=(addr-sp->modbase)&(sp->modmask);
		BJX2_PtrSetQWordOfsLe(sp->data, ra, val);
		return(0);
	}
#endif

	sp->SetQWord(ctx, sp, addr, val);
//	sp->SetDWord(ctx, sp, addr+4, val>>32);
//	return(sp->SetDWord(ctx, sp, addr, val));
	return(0);
}

int BJX2_MemSetXWord_Dfl(BJX2_Context *ctx, bjx2_addr addr0,
	u64 vlo, u64 vhi)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra, ra4;

	addr=BJX2_MemTranslateTlb(ctx, addr0, 2);
	if(ctx->status)
		return(0);

	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		BJX2_MemSimAddrL1(ctx, addr, 0x27);

		if(sp->simple_mem&2)
		{
//			ra=addr-sp->addr_base;
			ra=(addr-sp->modbase)&(sp->modmask);
			BJX2_PtrSetQWordOfsLe(sp->data, ra+0, vlo);
			BJX2_PtrSetQWordOfsLe(sp->data, ra+8, vhi);

			ra4=(ra>>4);
//			if(sp->tripwire[ra4>>3]&(1<<(ra4&7)))
			if(sp->tripwire[ra4>>5]&(1<<(ra4&31)))
				{ BJX2_MemRamCb_SetFault(ctx, sp, addr, vlo); }
			return(0);
		}
		return(sp->SetXWord(ctx, sp, addr, vlo, vhi));
	}

	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		if(!(ctx->status))
		{
			ctx->regs[BJX2_REG_PC]=ctx->trapc;
			ctx->regs[BJX2_REG_TEA]=addr;
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		}else
		{
			JX2_DBGBREAK
		}
		return(0);
	}

	BJX2_MemSimAddrL1(ctx, addr, 0x27);

#if 1
	if(sp->simple_mem&2)
	{
//		ra=addr-sp->addr_base;
		ra=(addr-sp->modbase)&(sp->modmask);
		BJX2_PtrSetQWordOfsLe(sp->data, ra, vlo);
		BJX2_PtrSetQWordOfsLe(sp->data, ra, vhi);
		return(0);
	}
#endif

//	sp->SetQWord(ctx, sp, addr, val);
	sp->SetXWord(ctx, sp, addr, vlo, vhi);
//	sp->SetDWord(ctx, sp, addr+4, val>>32);
//	return(sp->SetDWord(ctx, sp, addr, val));
	return(0);
}


int BJX2_MemSetDWord_NoAT(BJX2_Context *ctx, bjx2_addr addr0, s32 val)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra;

	addr=(u32)addr0;
	sp=ctx->span_pr0;
	if(sp && (((u32)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		if(sp->simple_mem&2)
		{
//			ra=addr-sp->addr_base;
			ra=(addr-sp->modbase)&(sp->modmask);
			BJX2_PtrSetDWordOfsLe(sp->data, ra, val);
			return(0);
		}
		return(sp->SetDWord(ctx, sp, addr, val));
	}

//	sp=BJX2_MemSpanForAddr(ctx, addr);
	sp=ctx->MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		if(!(ctx->status))
		{
			ctx->regs[BJX2_REG_PC]=ctx->trapc;
			ctx->regs[BJX2_REG_TEA]=addr;
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		}else
		{
			JX2_DBGBREAK
		}
		return(0);
	}

	if(sp->simple_mem&2)
	{
//		ra=addr-sp->addr_base;
		ra=(addr-sp->modbase)&(sp->modmask);
		BJX2_PtrSetDWordOfsLe(sp->data, ra, val);
		return(0);
	}

	return(sp->SetDWord(ctx, sp, addr, val));
}

int BJX2_MemSetQWord_NoAT(BJX2_Context *ctx, bjx2_addr addr0, s64 val)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra;

	addr=(u32)addr0;

	sp=ctx->span_pr0;
	if(sp && (((u32)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		if(sp->simple_mem&2)
		{
//			ra=addr-sp->addr_base;
			ra=(addr-sp->modbase)&(sp->modmask);
			BJX2_PtrSetQWordOfsLe(sp->data, ra, val);
			return(0);
		}
		return(sp->SetQWord(ctx, sp, addr, val));
	}

//	sp=BJX2_MemSpanForAddr(ctx, addr);
	sp=ctx->MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		if(!(ctx->status))
		{
			ctx->regs[BJX2_REG_PC]=ctx->trapc;
			ctx->regs[BJX2_REG_TEA]=addr;
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		}else
		{
			JX2_DBGBREAK
		}
		return(0);
	}

#if 1
	if(sp->simple_mem&2)
	{
//		ra=addr-sp->addr_base;
		ra=(addr-sp->modbase)&(sp->modmask);
		BJX2_PtrSetQWordOfsLe(sp->data, ra, val);
		return(0);
	}
#endif

	return(sp->SetQWord(ctx, sp, addr, val));
}


int BJX2_MemSetTripwire_Dfl(BJX2_Context *ctx, bjx2_addr addr0, int val)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr;
	int ra;

	addr=BJX2_MemTranslateTlb(ctx, addr0, 2);
	if(ctx->status)
		return(0);

	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		BJX2_MemSimAddrL1(ctx, addr, 0);

//		if(sp->simple_mem&2)
//		{
//			ra=addr-sp->addr_base;
//			BJX2_PtrSetByteOfsLe(sp->data, ra, val);
//			return(0);
//		}
		
		if(!sp->SetTripwire)
			return(0);
		return(sp->SetTripwire(ctx, sp, addr, val));
	}

	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		if(!(ctx->status))
		{
			ctx->regs[BJX2_REG_PC]=ctx->trapc;
			ctx->regs[BJX2_REG_TEA]=addr;
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		}else
		{
			JX2_DBGBREAK
		}
		return(0);
	}

	BJX2_MemSimAddrL1(ctx, addr, 0);

	if(!sp->SetTripwire)
		return(0);
	return(sp->SetTripwire(ctx, sp, addr, val));
}

int BJX2_MemQueryTransit_Dfl(BJX2_Context *ctx,
	bjx2_addr addr0, bjx2_addr addr1, int val)
{
	BJX2_MemSpan *sp;
	bjx2_addr addr, addrb;
	int ra;

	addr=BJX2_MemTranslateTlb(ctx, addr0, 0);
	addrb=BJX2_MemTranslateTlb(ctx, addr1, 0);
	if(ctx->status)
		return(0);

	if((addrb-addr)!=(addr1-addr0))
	{
		return(0);
	}

	sp=ctx->span_pr0;
	if(sp && (((bjx2_addru)(addr-sp->addr_base))<(sp->addr_sz)))
	{
		BJX2_MemSimAddrL1(ctx, addr, 0);

//		if(sp->simple_mem&2)
//		{
//			ra=addr-sp->addr_base;
//			BJX2_PtrSetByteOfsLe(sp->data, ra, val);
//			return(0);
//		}
		
		if(!sp->MemQueryTransit)
			return(0);
		return(sp->MemQueryTransit(ctx, sp, addr, addrb, val));
	}

	sp=BJX2_MemSpanForAddr(ctx, addr);
	if(!sp)
	{
		if(!(ctx->status))
		{
			ctx->regs[BJX2_REG_PC]=ctx->trapc;
			ctx->regs[BJX2_REG_TEA]=addr;
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVADDR);
		}else
		{
			JX2_DBGBREAK
		}
		return(0);
	}

	BJX2_MemSimAddrL1(ctx, addr, 0);

	if(!sp->MemQueryTransit)
		return(0);
	return(sp->MemQueryTransit(ctx, sp, addr, addrb, val));
}


int BJX2_MemSetupState(BJX2_Context *ctx)
{
	if(ctx->MemGetByte)
		return(0);

	ctx->MemGetByte=BJX2_MemGetByte_Dfl;
	ctx->MemGetWord=BJX2_MemGetWord_Dfl;
	ctx->MemGetDWord=BJX2_MemGetDWord_Dfl;
	ctx->MemGetQWord=BJX2_MemGetQWord_Dfl;
	ctx->MemGetXWord=BJX2_MemGetXWord_Dfl;
	ctx->MemSetByte=BJX2_MemSetByte_Dfl;
	ctx->MemSetWord=BJX2_MemSetWord_Dfl;
	ctx->MemSetDWord=BJX2_MemSetDWord_Dfl;
	ctx->MemSetQWord=BJX2_MemSetQWord_Dfl;
	ctx->MemSetXWord=BJX2_MemSetXWord_Dfl;
//	ctx->MemSpanForAddr=BJX2_MemSpanForAddr64;

	ctx->MemSetTripwire=BJX2_MemSetTripwire_Dfl;
	ctx->MemQueryTransit=BJX2_MemQueryTransit_Dfl;

	if(ctx->regs[BJX2_REG_SR]&0x80000000)
		ctx->MemSpanForAddr=BJX2_MemSpanForAddr64;
	else
		ctx->MemSpanForAddr=BJX2_MemSpanForAddr32;

//	if(!(ctx->regs[BJX2_REG_MMCR]&1) && (ctx->use_jit || ctx->no_memcost))
	if(0)
	{
		if(!(ctx->regs[BJX2_REG_SR]&0x80000000))
		{
			ctx->MemGetByte=BJX2_MemGetByte_NoAT;
			ctx->MemGetWord=BJX2_MemGetWord_NoAT;
			ctx->MemGetDWord=BJX2_MemGetDWord_NoAT;
			ctx->MemGetQWord=BJX2_MemGetQWord_NoAT;
			ctx->MemSetDWord=BJX2_MemSetDWord_NoAT;
			ctx->MemSetQWord=BJX2_MemSetQWord_NoAT;
			ctx->MemSpanForAddr=BJX2_MemSpanForAddr32;
		}
	}

	return(1);
}

int BJX2_MemGetByte(BJX2_Context *ctx, bjx2_addr addr0)
{
//	if(ctx->MemGetByte)
//		return(ctx->MemGetByte(ctx, addr0));
//	ctx->MemGetByte=BJX2_MemGetByte_Dfl;
	return(ctx->MemGetByte(ctx, addr0));
}

int BJX2_MemGetWord(BJX2_Context *ctx, bjx2_addr addr0)
{
//	if(ctx->MemGetWord)
//		return(ctx->MemGetWord(ctx, addr0));
//	ctx->MemGetWord=BJX2_MemGetWord_Dfl;
	return(ctx->MemGetWord(ctx, addr0));
}

s32 BJX2_MemGetDWord(BJX2_Context *ctx, bjx2_addr addr0)
{
//	if(ctx->MemGetDWord)
//		return(ctx->MemGetDWord(ctx, addr0));
//	ctx->MemGetDWord=BJX2_MemGetDWord_Dfl;
	return(ctx->MemGetDWord(ctx, addr0));
}

s64 BJX2_MemGetQWord(BJX2_Context *ctx, bjx2_addr addr0)
{
	s64 v;
//	if(ctx->MemGetQWord)
//		return(ctx->MemGetQWord(ctx, addr0));
//	ctx->MemGetQWord=BJX2_MemGetQWord_Dfl;
	v=ctx->MemGetQWord(ctx, addr0);
	return(v);
}

int BJX2_MemGetXWord(BJX2_Context *ctx, bjx2_addr addr0,
	u64 *rvlo, u64 *rvhi)
{
	int v;
	v=ctx->MemGetXWord(ctx, addr0, rvlo, rvhi);
	return(v);
}

int BJX2_MemSetByte(BJX2_Context *ctx, bjx2_addr addr0, int val)
{
//	if(ctx->MemSetByte)
//		return(ctx->MemSetByte(ctx, addr0, val));
//	ctx->MemSetByte=BJX2_MemSetByte_Dfl;
	return(ctx->MemSetByte(ctx, addr0, val));
}

int BJX2_MemSetWord(BJX2_Context *ctx, bjx2_addr addr0, int val)
{
//	if(ctx->MemSetWord)
//		return(ctx->MemSetWord(ctx, addr0, val));
//	ctx->MemSetWord=BJX2_MemSetWord_Dfl;
	return(ctx->MemSetWord(ctx, addr0, val));
}

int BJX2_MemSetDWord(BJX2_Context *ctx, bjx2_addr addr0, s32 val)
{
//	if(ctx->MemSetDWord)
//		return(ctx->MemSetDWord(ctx, addr0, val));
//	ctx->MemSetDWord=BJX2_MemSetDWord_Dfl;
	return(ctx->MemSetDWord(ctx, addr0, val));
}

int BJX2_MemSetQWord(BJX2_Context *ctx, bjx2_addr addr0, s64 val)
{
//	if(ctx->MemSetQWord)
//		return(ctx->MemSetQWord(ctx, addr0, val));
//	ctx->MemSetQWord=BJX2_MemSetQWord_Dfl;
	return(ctx->MemSetQWord(ctx, addr0, val));
}

int BJX2_MemSetXWord(BJX2_Context *ctx, bjx2_addr addr0,
	u64 vlo, u64 vhi)
{
	return(ctx->MemSetXWord(ctx, addr0, vlo, vhi));
}

int BJX2_MemSetTrip(BJX2_Context *ctx, bjx2_addr addr0, int val)
{
	return(ctx->MemSetTripwire(ctx, addr0, val));
}

int BJX2_MemQueryTransit(BJX2_Context *ctx,
	bjx2_addr addr0, bjx2_addr addr1, int val)
{
	return(ctx->MemQueryTransit(ctx, addr0, addr1, val));
}
