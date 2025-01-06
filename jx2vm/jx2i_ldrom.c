/*
 MIT No Attribution

 Copyright (c) 2018-2024 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person 
 obtaining a copy of this software and associated documentation 
 files (the "Software"), to deal in the Software without 
 restriction, including without limitation the rights to use, 
 copy, modify, merge, publish, distribute, sublicense, and/or sell 
 copies of the Software, and to permit persons to whom the 
 Software is furnished to do so.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
 OR OTHER DEALINGS IN THE SOFTWARE.
*/

u32 mmgp_data[256];

s64 mmgp_spi_lastcyc;
int mmgp_spi_delcyc;

char *bjx2_fgets(char *buf, int lim, BJX2_FILE *fd);

int BJX2_Interp_ProcessUsbMsg(BJX2_Context *ctx, byte *ibuf, int isz,
	byte *obuf, int *rosz)
{
	int i, j, k, l;

	if((ibuf[0]==0x69) &&
		((ibuf[1]==0x00) || (ibuf[1]==0x01)))
	{
		if(!ctx->do_usb_hid)
			return(0);
			
		l=ctx->usbkb_hidctrl_sz;

		obuf[ 0]=0xC3;
		
		memcpy(obuf+1, ctx->usbkb_hidctrl, l);
		*rosz=l+3;
		return(1);
	}

	if((ibuf[0]==0x69) &&
//		((ibuf[1]==0x00) || (ibuf[1]==0x01)))
		((ibuf[1]==0x80) || (ibuf[1]==0x81)))
	{
		if(!ctx->do_usb_hid)
			return(0);

#if 0
		printf("BJX2_Interp_ProcessUsbMsg: KB Report "
			"%02X %02X %02X %02X  %02X %02X %02X %02X\n",
			ctx->usbkb_report[0],
			ctx->usbkb_report[1],
			ctx->usbkb_report[2],
			ctx->usbkb_report[3],
			ctx->usbkb_report[4],
			ctx->usbkb_report[5],
			ctx->usbkb_report[6],
			ctx->usbkb_report[7]);
#endif

		obuf[ 0]=0xC3;
		obuf[ 1]=ctx->usbkb_report[0];
		obuf[ 2]=ctx->usbkb_report[1];
		obuf[ 3]=ctx->usbkb_report[2];
		obuf[ 4]=ctx->usbkb_report[3];
		obuf[ 5]=ctx->usbkb_report[4];
		obuf[ 6]=ctx->usbkb_report[5];
		obuf[ 7]=ctx->usbkb_report[6];
		obuf[ 8]=ctx->usbkb_report[7];
		obuf[ 9]=0x00;
		obuf[10]=0x00;
		*rosz=11;
		return(1);
	}

	if((ibuf[0]==0xC3) || (ibuf[0]==0x4B))
	{
		if(ctx->usbkb_hidctrl_st==1)
		{
			ctx->usbkb_hidctrl_st=0;

			if(ibuf[1]==0x00)
			{
				if(ibuf[2]==0x05)
				{
					printf("BJX2_Interp_ProcessUsbMsg: Set Address\n");
				}

				if(ibuf[2]==0x09)
				{
					printf("BJX2_Interp_ProcessUsbMsg: Set Configuration\n");
				}
			}

			if(ibuf[1]==0x80)
			{
				if(ibuf[2]==0x06)
				{
					printf("BJX2_Interp_ProcessUsbMsg: Get Descriptor\n");
					ctx->usbkb_hidctrl_sz=ibuf[7];
					
					memset(ctx->usbkb_hidctrl, 0, 64);
					ctx->usbkb_hidctrl[ 0]=18;
					ctx->usbkb_hidctrl[ 1]=0x01;
					ctx->usbkb_hidctrl[ 2]=0x01;
					ctx->usbkb_hidctrl[ 3]=0x10;
					ctx->usbkb_hidctrl[17]=0x01;
				}
			}

			if(ibuf[1]==0x1A)
			{
				if(ibuf[2]==0x01)
				{
					memcpy(ctx->usbkb_hidctrl, ctx->usbkb_report, 8);
					ctx->usbkb_hidctrl_sz=8;
				}
			}
		}

		obuf[0]=0xD2;
		*rosz=1;
		return(1);
	}

	if(ibuf[0]==0x2D)
	{
		printf("BJX2_Interp_ProcessUsbMsg: Setup\n");
		ctx->usbkb_hidctrl_st=1;
		*rosz=0;
		return(0);
	}

	return(0);
}

int BJX2_Interp_CheckUpdateUsb(BJX2_Context *ctx)
{
	byte tbuf[512], tbuf2[512];
	u16 *txba, *rxba;
	int i, j, k, txs, txe, osz;
	int rxs, rxe;
	
	if(ctx->usbbuf_txsposa!=ctx->usbbuf_txeposa)
	{
		txba=(u16 *)ctx->usbbuf_txa;
		txs=ctx->usbbuf_txsposa;
		txe=ctx->usbbuf_txeposa;
		
		i=0;
		while(txs!=txe)
		{
			k=txba[txs];
			if(k&0x100)
				break;
			txs=(txs+1)&1023;
			tbuf[i++]=k;
		}
		while(txs!=txe)
		{
			k=txba[txs];
			if(!(k&0x100))
				break;
			txs=(txs+1)&1023;
		}
		ctx->usbbuf_txsposa=txs;

		osz=0;
		BJX2_Interp_ProcessUsbMsg(ctx, tbuf, i, tbuf2, &osz);
		if(osz>0)
		{
			rxba=(u16 *)ctx->usbbuf_rxa;
			rxs=ctx->usbbuf_rxsposa;
			rxe=ctx->usbbuf_rxeposa;
			
			for(i=0; i<osz; i++)
			{
				rxba[rxe]=tbuf2[i];
				rxe=(rxe+1)&1023;
			}
			rxba[rxe]=0x100;
			rxe=(rxe+1)&1023;
			ctx->usbbuf_rxeposa=rxe;
		}
	}

	if(ctx->usbbuf_txsposb!=ctx->usbbuf_txeposb)
	{
		txs=ctx->usbbuf_txsposb;
		txe=ctx->usbbuf_txeposb;
		ctx->usbbuf_txsposb=txe;
	}
	return(0);
}

int BJX2_MemMmgpCb_GetByte(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

int BJX2_MemMmgpCb_GetWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

s64 BJX2_Interp_GetVirtualUsec(BJX2_Context *ctx)
{
	s64 rvq;

	if(ctx->use_walltime)
	{
		rvq=clock()*1000;
		return(rvq);
	}

	rvq=(ctx->tot_cyc*ctx->rcp_mhz)>>16;
//	rv=rvq;

	return(rvq);
}

s32 BJX2_MemMmgpCb_GetDWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	u32 *mmio;
	s64 rvq, dcyc;
	int ra, rv;

	dcyc=ctx->tot_cyc-mmgp_spi_lastcyc;
	mmgp_spi_lastcyc=ctx->tot_cyc;
	
	if(dcyc<10)
		dcyc=10;
	
	if(dcyc>mmgp_spi_delcyc)
		mmgp_spi_delcyc=0;
	else
		mmgp_spi_delcyc-=dcyc;

	if(ctx->no_memcost)
		mmgp_spi_delcyc=0;

	ctx->iodel_cyc=mmgp_spi_delcyc;


	ra=addr-sp->addr_base;
	ra=(ra&65535);

	mmio=mmgp_data;

	rv=0;
	switch(ra)
	{
	case 0x0000:
//		rvq=(ctx->tot_cyc*ctx->rcp_mhz)>>16;
		rvq=BJX2_Interp_GetVirtualUsec(ctx);
		rv=rvq;
//		rv=ctx->tot_cyc/ctx->tgt_mhz;
		break;
	case 0x0004:
//		rvq=(ctx->tot_cyc*ctx->rcp_mhz)>>16;
		rvq=BJX2_Interp_GetVirtualUsec(ctx);
		rv=rvq>>32;
//		rv=(ctx->tot_cyc/ctx->tgt_mhz)>>32;
		break;

	case 0x0008:
		rv=ctx->ttick_hk;
		break;
	case 0x000C:
		rv=ctx->ttick_rst;
		break;

	case 0x0010:
		rv=0;
		if(ctx->kbirov!=ctx->kbrov)
		{
			rv=ctx->kbbuf[ctx->kbirov];
			ctx->kbirov=(ctx->kbirov+1)&255;
		}
		break;
	case 0x0014:
		rv=0; break;
	case 0x0018:
		rv=0;
		if(ctx->kbirov!=ctx->kbrov)
			rv|=1;
		break;
	case 0x001C:
		rv=0; break;

	case 0x0030:
		rv=mmio[0x10];
		rv&=~2;
		if(mmgp_spi_delcyc>0)
			rv|=2;
//		printf("SPI_C(R): D=%08X\n", rv);
		break;
	case 0x0034:
		rv=mmio[0x11];
//		printf("SPI_D(R): D=%08X\n", rv);
		break;

	case 0x0038:
		rv=mmio[0x12];
		break;
	case 0x003C:
		rv=mmio[0x13];
		break;
	case 0x0039:
		rv=mmio[0x14];
		break;
	case 0x003D:
		rv=mmio[0x15];
		break;
	case 0x003A:
		rv=mmio[0x16];
		break;
	case 0x003E:
		rv=mmio[0x17];
		break;
	case 0x003B:
		rv=mmio[0x18];
		break;
	case 0x003F:
		rv=mmio[0x19];
		break;

	case 0x0040:
		rv=0;
		if(ctx->ps2kbirov!=ctx->ps2kbrov)
		{
			rv=ctx->ps2kbbuf[ctx->ps2kbirov];
			ctx->ps2kbirov=(ctx->ps2kbirov+1)&255;
		}
		break;
	case 0x0044:
		rv=0; break;
	case 0x0048:
		rv=0;
		if(ctx->ps2kbirov!=ctx->ps2kbrov)
			rv|=1;
		break;
	case 0x004C:
		rv=0; break;

	case 0x0050:
		rv=0;
		if(ctx->ps2msirov!=ctx->ps2msrov)
		{
			rv=ctx->ps2msbuf[ctx->ps2msirov];
			ctx->ps2msirov=(ctx->ps2msirov+1)&255;
		}
		break;
	case 0x0054:
		rv=0; break;
	case 0x0058:
		rv=0;
		if(ctx->ps2msirov!=ctx->ps2msrov)
			rv|=1;
		break;
	case 0x005C:
		rv=0; break;


	case 0x0300:
		rv=ctx->msgbuf_rxspos;
		break;
	case 0x0304:
		rv=ctx->msgbuf_rxepos;
		break;
	case 0x0308:
		rv=ctx->msgbuf_txspos;
		break;
	case 0x030C:
		rv=ctx->msgbuf_txepos;
		break;
	case 0x0310:
		rv=ctx->rtc_ms;
		break;
	case 0x0314:
		rv=ctx->msgbuf_msk;
		break;

	case 0x0340:
		rvq=ctx->tot_cyc;
		rv=rvq;
//		rv=ctx->tot_cyc/ctx->tgt_mhz;
		break;
	case 0x0344:
		rvq=ctx->tot_cyc;
		rv=rvq>>32;
//		rv=(ctx->tot_cyc/ctx->tgt_mhz)>>32;
		break;
	case 0x0348:
		rvq=ctx->tgt_mhz;
		rv=rvq;
		break;
	case 0x034C:
		rvq=ctx->tgt_mhz;
		rv=rvq>>32;
		break;

	case 0x0800:
		BJX2_Interp_CheckUpdateUsb(ctx);
		rv=ctx->usbbuf_rxsposa;
		break;
	case 0x0804:
		BJX2_Interp_CheckUpdateUsb(ctx);
		rv=ctx->usbbuf_rxeposa;
		break;
	case 0x0808:
		BJX2_Interp_CheckUpdateUsb(ctx);
		rv=0x0200;
		break;
	case 0x080C:
		BJX2_Interp_CheckUpdateUsb(ctx);
		rv=0x0000;
		break;

	case 0x0810:
		BJX2_Interp_CheckUpdateUsb(ctx);
		rv=ctx->usbbuf_txsposa;
		break;
	case 0x0814:
		BJX2_Interp_CheckUpdateUsb(ctx);
		rv=ctx->usbbuf_txeposa;
		break;
	case 0x0818:
		BJX2_Interp_CheckUpdateUsb(ctx);
		rv=0x0200;
		break;
	case 0x081C:
		BJX2_Interp_CheckUpdateUsb(ctx);
		rv=0x0000;
		break;

	case 0x0820:
		BJX2_Interp_CheckUpdateUsb(ctx);
		rv=ctx->usbbuf_rxsposb;
		break;
	case 0x0824:
		BJX2_Interp_CheckUpdateUsb(ctx);
		rv=ctx->usbbuf_rxeposb;
		break;
	case 0x0828:
		BJX2_Interp_CheckUpdateUsb(ctx);
		rv=0x0200;
		break;
	case 0x082C:
		BJX2_Interp_CheckUpdateUsb(ctx);
		rv=0x0000;
		break;

	case 0x0830:
		BJX2_Interp_CheckUpdateUsb(ctx);
		rv=ctx->usbbuf_txsposb;
		break;
	case 0x0834:
		BJX2_Interp_CheckUpdateUsb(ctx);
		rv=ctx->usbbuf_txeposb;
		break;
	case 0x0838:
		BJX2_Interp_CheckUpdateUsb(ctx);
		rv=0x0200;
		break;
	case 0x083C:
		BJX2_Interp_CheckUpdateUsb(ctx);
		rv=0x0000;
		break;

	default:
//		JX2_DBGBREAK
		break;
	}
	return(rv);
}


int BJX2_MemMmgpCb_SetByte(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int val)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

int BJX2_MemMmgpCb_SetWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int val)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

int BJX2_MemMmgpCb_SetDWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s32 val)
{
	u32 *mmio;
	u64 lv0, lv1, lv2, lv3, lv4, lv5, lv6, lv7;
	s64 dcyc;
	int v;
	int ra;
	int i, j, k;

	dcyc=ctx->tot_cyc-mmgp_spi_lastcyc;
	mmgp_spi_lastcyc=ctx->tot_cyc;
	
	if(dcyc>mmgp_spi_delcyc)
		mmgp_spi_delcyc=0;
	else
		mmgp_spi_delcyc-=dcyc;

	if(ctx->no_memcost)
		mmgp_spi_delcyc=0;

	ctx->iodel_cyc=mmgp_spi_delcyc;


	ra=addr-sp->addr_base;
	ra=(ra&65535);

	mmio=mmgp_data;

	switch(ra)
	{
	case 0x0000:
		break;
	case 0x0004:
		break;
	case 0x0008:
		ctx->ttick_hk=val;
		break;
	case 0x000C:
		ctx->ttick_rst=val;
		break;
	case 0x0010:
		break;
	case 0x0014:
//		printf("%c", (char)val);
		bjx2_vmputc(ctx, (byte)val);
		break;
	case 0x0018:
		break;
	case 0x001C:
		break;

	case 0x0030:
		if(val&0x20)
		{
			lv0=mmio[0x12]|(((u64)mmio[0x13])<<32);

			lv1=0;
			for(i=0; i<8; i++)
			{
				if(!(ctx->no_memcost))
				{
//					mmgp_spi_delcyc+=(ctx->tgt_mhz*8)/5;
					mmgp_spi_delcyc+=(ctx->tgt_mhz*8)/10;
				}
				v=btesh2_spimmc_XrByte(ctx, (lv0>>(i*8))&255);
				lv1|=((u64)(v&255))<<(i*8);
			}
			mmio[0x12]=lv1;
			mmio[0x13]=lv1>>32;

//			ctx->regs[BJX2_REG_PC]=ctx->trapc;
//			ctx->regs[BJX2_REG_TEA]=addr;
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_IOPOKE);
		}
	
		if(val&0x40)
		{
			lv0=mmio[0x12]|(((u64)mmio[0x13])<<32);
			lv1=mmio[0x14]|(((u64)mmio[0x15])<<32);
			lv2=mmio[0x16]|(((u64)mmio[0x17])<<32);
			lv3=mmio[0x18]|(((u64)mmio[0x19])<<32);

			lv4=0;	lv5=0;	lv6=0;	lv7=0;
			for(i=0; i<32; i++)
			{
				if(!(ctx->no_memcost))
				{
//					mmgp_spi_delcyc+=(ctx->tgt_mhz*8)/5;
					mmgp_spi_delcyc+=(ctx->tgt_mhz*8)/10;
				}
//				v=btesh2_spimmc_XrByte(ctx, (lv0>>(i*8))&255);
				v=btesh2_spimmc_XrByte(ctx, lv0&255);
				lv0=(lv0>>8)|(lv1<<56);
				lv1=(lv1>>8)|(lv2<<56);
				lv2=(lv2>>8)|(lv3<<56);
				lv3=(lv3>>8);

				lv4=(lv4>>8)|(lv5<<56);
				lv5=(lv5>>8)|(lv6<<56);
				lv6=(lv6>>8)|(lv7<<56);
				lv7=(lv7>>8)|(((u64)(v&255))<<56);
				

//				lv1|=((u64)(v&255))<<(i*8);
			}
			mmio[0x12]=lv4;
			mmio[0x13]=lv4>>32;
			mmio[0x14]=lv5;
			mmio[0x15]=lv5>>32;
			mmio[0x16]=lv6;
			mmio[0x17]=lv6>>32;
			mmio[0x18]=lv7;
			mmio[0x19]=lv7>>32;

//			ctx->regs[BJX2_REG_PC]=ctx->trapc;
//			ctx->regs[BJX2_REG_TEA]=addr;
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_IOPOKE);
		}
	
		v=btesh2_spimmc_XrCtl(ctx, val);
		mmio[0x10]=v&255;
		if(v&0x10000)
			mmio[0x11]=(v>>8)&255;

//		printf("SPI_C(W): %08X -> %08X, D=%08X\n", val, v, mmio[0x11]);
		break;
	case 0x0034:
		v=btesh2_spimmc_XrByte(ctx, val);
		mmio[0x11]=v;
//		ctx->iodel_cyc=800;
//		mmgp_spi_delcyc+=800;
//		ctx->iodel_cyc=200;
//		mmgp_spi_delcyc+=200;
//		ctx->iodel_cyc=20;
		if(!(ctx->no_memcost))
		{
//			mmgp_spi_delcyc+=20;
//			mmgp_spi_delcyc+=(ctx->tgt_mhz*8)/5;
			mmgp_spi_delcyc+=(ctx->tgt_mhz*8)/10;
		}

//		ctx->regs[BJX2_REG_PC]=ctx->trapc;
//		ctx->regs[BJX2_REG_TEA]=addr;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_IOPOKE);

//		printf("SPI_D(W): %08X -> %08X, D=%08X\n", val, v, mmio[0x11]);
		break;

	case 0x0038:
		mmio[0x12]=val;
		break;
	case 0x003C:
		mmio[0x13]=val;
		break;

	case 0x0039:
		mmio[0x14]=val;
		break;
	case 0x003D:
		mmio[0x15]=val;
		break;
	case 0x003A:
		mmio[0x16]=val;
		break;
	case 0x003E:
		mmio[0x17]=val;
		break;
	case 0x003B:
		mmio[0x18]=val;
		break;
	case 0x003F:
		mmio[0x19]=val;
		break;

	case 0x0044:
		i=ctx->ps2txkbrov;
		ctx->ps2txkbrov=(i+1)&255;
		ctx->ps2txkbbuf[i]=val;
		break;

	case 0x0054:
		i=ctx->ps2txmsrov;
		ctx->ps2txmsrov=(i+1)&255;
		ctx->ps2txmsbuf[i]=val;
		break;


	case 0x0300:
		ctx->msgbuf_rxspos=val;
		BJX2_VmMsgRxUpdate(ctx);
		break;
	case 0x0304:
		ctx->msgbuf_rxepos=val;
		BJX2_VmMsgRxUpdate(ctx);
		break;
	case 0x0308:
		ctx->msgbuf_txspos=val;
		BJX2_VmMsgTxUpdate(ctx);
		break;
	case 0x030C:
		ctx->msgbuf_txepos=val;
		BJX2_VmMsgTxUpdate(ctx);
		break;
	
	case 0x0800:
		ctx->usbbuf_rxsposa=val;
		break;
	case 0x0804:
		ctx->usbbuf_rxeposa=val;
		break;
	case 0x0810:
		ctx->usbbuf_txsposa=val;
		break;
	case 0x0814:
		ctx->usbbuf_txeposa=val;
		break;

	case 0x0820:
		ctx->usbbuf_rxsposb=val;
		break;
	case 0x0824:
		ctx->usbbuf_rxeposb=val;
		break;
	case 0x0830:
		ctx->usbbuf_txsposb=val;
		break;
	case 0x0834:
		ctx->usbbuf_txeposb=val;
		break;

	default:
		JX2_DBGBREAK
		break;
	}

	return(0);
}

s64 BJX2_MemMmgpCb_GetQWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	u32 lo, hi;

	if(((s64)addr)>0)
	{
		lo=-1;
	}

	lo=(u32)(BJX2_MemMmgpCb_GetDWord(ctx, sp, addr+0));
	hi=(u32)(BJX2_MemMmgpCb_GetDWord(ctx, sp, addr+4));
	return((((u64)hi)<<32)|lo);
}

int BJX2_MemMmgpCb_SetQWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s64 val)
{
	BJX2_MemMmgpCb_SetDWord(ctx, sp, addr+0, val);
	BJX2_MemMmgpCb_SetDWord(ctx, sp, addr+4, val>>32);
	return(0);
}

int BJX2_MemDefineMmgp(BJX2_Context *ctx,
	char *name, bjx2_addr base, bjx2_addr lim)
{
	BJX2_MemSpan *sp;
	
	sp=BJX2_MemAllocSpan(ctx);
	
	sp->name=name;
	sp->addr_base=base;
	sp->addr_lim=lim;
	sp->addr_sz=lim-base;
//	sp->data=malloc((lim-base)+8);
	
	sp->GetByte=BJX2_MemMmgpCb_GetByte;
	sp->GetWord=BJX2_MemMmgpCb_GetWord;
	sp->GetDWord=BJX2_MemMmgpCb_GetDWord;
	sp->GetQWord=BJX2_MemMmgpCb_GetQWord;
	sp->GetXWord=BJX2_MemRamCb_GetFaultXW;
	
	sp->SetByte=BJX2_MemMmgpCb_SetByte;
	sp->SetWord=BJX2_MemMmgpCb_SetWord;
	sp->SetDWord=BJX2_MemMmgpCb_SetDWord;
	sp->SetQWord=BJX2_MemMmgpCb_SetQWord;
	sp->SetXWord=BJX2_MemRamCb_SetFaultXW;

	BJX2_MemAddSpan(ctx, sp);
	return(0);
}


s32 BJX2_MemMmUsbCb_GetDWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	return(0);
}

int BJX2_MemMmUsbCb_SetDWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s32 val)
{
	return(0);
}

s64 BJX2_MemMmUsbCb_GetQWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	u32 lo, hi;
	int ra;

	if(((s64)addr)>0)
	{
		lo=-1;
	}

	ra=addr-sp->addr_base;
	ra=(ra&16383);
	return(ctx->usbbuf[ra>>3]);
}

int BJX2_MemMmUsbCb_SetQWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s64 val)
{
	int ra;
	ra=addr-sp->addr_base;
	ra=(ra&16383);
	ctx->usbbuf[ra>>3]=val;
	return(0);
}

int BJX2_MemDefineUsbBuf(BJX2_Context *ctx,
	char *name, bjx2_addr base, bjx2_addr lim)
{
	BJX2_MemSpan *sp;

	ctx->usbbuf=malloc(16384);
	ctx->usbbuf_rxa=ctx->usbbuf+  0;
	ctx->usbbuf_txa=ctx->usbbuf+256;
	ctx->usbbuf_rxb=ctx->usbbuf+512;
	ctx->usbbuf_txb=ctx->usbbuf+768;

	sp=BJX2_MemAllocSpan(ctx);
	
	sp->name=name;
	sp->addr_base=base;
	sp->addr_lim=lim;
	sp->addr_sz=lim-base;
//	sp->data=malloc((lim-base)+8);
	
	sp->GetByte=BJX2_MemMmgpCb_GetByte;
	sp->GetWord=BJX2_MemMmgpCb_GetWord;
	sp->GetDWord=BJX2_MemMmUsbCb_GetDWord;
	sp->GetQWord=BJX2_MemMmUsbCb_GetQWord;
	sp->GetXWord=BJX2_MemRamCb_GetFaultXW;
	
	sp->SetByte=BJX2_MemMmgpCb_SetByte;
	sp->SetWord=BJX2_MemMmgpCb_SetWord;
	sp->SetDWord=BJX2_MemMmUsbCb_SetDWord;
	sp->SetQWord=BJX2_MemMmUsbCb_SetQWord;
	sp->SetXWord=BJX2_MemRamCb_SetFaultXW;

	BJX2_MemAddSpan(ctx, sp);
	return(0);
}


int BJX2_MapSortTMapCmp(BJX2_Context *ctx,
	bjx2_addr *tmap_addr,
	char **tmap_name,
	u16 *tmap_mode,
	int i, int j)
{
	int sw;
	sw=(tmap_addr[j]<tmap_addr[i]);
	if((tmap_mode[j]=='L') && (tmap_mode[i]!='L'))
		sw=0;
	if((tmap_mode[j]!='L') && (tmap_mode[i]=='L'))
		sw=1;
	return(sw);
}

int BJX2_MapSortTMapCmpPv(BJX2_Context *ctx,
	bjx2_addr *tmap_addr,
	char **tmap_name,
	u16 *tmap_mode,
	int ix,
	bjx2_addr pva, u16 pvm)
{
	int sw;
	sw=(pva<tmap_addr[ix]);
	if((pvm=='L') && (tmap_mode[ix]!='L'))
		sw=0;
	if((pvm!='L') && (tmap_mode[ix]=='L'))
		sw=1;
	return(sw);
}

int BJX2_MapSortTMapBase(BJX2_Context *ctx,
	bjx2_addr *tmap_addr,
	char **tmap_name,
	u16 *tmap_mode,
	int min, int max)
{
	char *s;
	bjx2_addr ta;
	int i, j, k, sw;

	for(i=min; i<max; i++)
	{
		for(j=i+1; j<max; j++)
		{
			sw=BJX2_MapSortTMapCmp(ctx,
				tmap_addr, tmap_name, tmap_mode,
				i, j);
			if(sw)
			{
				ta=tmap_addr[j];
				s=tmap_name[j];
				k=tmap_mode[j];
				tmap_addr[j]=tmap_addr[i];
				tmap_name[j]=tmap_name[i];
				tmap_mode[j]=tmap_mode[i];
				tmap_addr[i]=ta;
				tmap_name[i]=s;
				tmap_mode[i]=k;
			}
		}
	}
	return(0);
}

int BJX2_MapSortTMapRec(BJX2_Context *ctx,
	bjx2_addr *tmap_addr,
	char **tmap_name,
	u16 *tmap_mode,
	int min, int max, int lim)
{
	char *s;
	bjx2_addr ta, pa;
	int i, j, k, m, n, pm, sw;

	n=max-min;
	
	if((n<16) || (lim<=0))
//	if(1)
	{
		BJX2_MapSortTMapBase(ctx,
			tmap_addr, tmap_name, tmap_mode, min, max);
		return(0);
	}

	k=(min+max)>>1;
	pa=tmap_addr[k];
	pm=tmap_mode[k];

	m=min;
	n=max;
	while(m<n)
	{
		sw=BJX2_MapSortTMapCmpPv(ctx,
			tmap_addr, tmap_name, tmap_mode,
			m, pa, pm);
		if(sw)
		{
			n--;
			ta=tmap_addr[n];
			s=tmap_name[n];
			k=tmap_mode[n];
			tmap_addr[n]=tmap_addr[m];
			tmap_name[n]=tmap_name[m];
			tmap_mode[n]=tmap_mode[m];
			tmap_addr[m]=ta;
			tmap_name[m]=s;
			tmap_mode[m]=k;
		}else
		{
			m++;
		}
	}
	
	BJX2_MapSortTMapRec(
		ctx, tmap_addr, tmap_name, tmap_mode, min, m, lim-1);
	BJX2_MapSortTMapRec(
		ctx, tmap_addr, tmap_name, tmap_mode, m, max, lim-1);
		
	return(0);
}


int BJX2_ContextLoadMap(BJX2_Context *ctx, char *name, char *imgname)
{
//	static bjx2_addr tmap_addr[16384];
//	static char *tmap_name[16384];
//	static bjx2_addr tmap_addr[32768];
//	static char *tmap_name[32768];

	static bjx2_addr *tmap_addr=NULL;
	static char **tmap_name=NULL;
	static u16 *tmap_mode=NULL;
	static int tmap_max=0;
	char tb[256];
	BJX2_FILE *fd;
	char **a;
	char *s, *t, *pbase;
	s64 ta;
	int tmn, mn, tmnb;
	int sz, sz1, sw;
	int i, j, k;

	strcpy(tb, name);
	s=tb+strlen(tb);
	while(s>tb)
	{
		if((*s=='/') || (*s=='\\'))
			break;
		s--;
	}
	*s=0;
	pbase=strdup(tb);
	
	fd=bjx2_fopen(name, "rb");
	if(!fd)
	{
		printf("Failed open %s\n", name);
		return(-1);
	}

	if(!tmap_addr)
	{
		k=32768;
		tmap_addr=malloc(k*sizeof(bjx2_addr));
		tmap_name=malloc(k*sizeof(char *));
		tmap_mode=malloc(k*sizeof(u16));
		tmap_max=k;
	}

	tmn=0;

#if 0
	if(ctx->map_n_ents)
	{
		for(i=0; i<ctx->map_n_ents; i++)
		{
			tmap_addr[i]=ctx->map_addr[i];
			tmap_name[i]=ctx->map_name[i];
		}
		tmn=ctx->map_n_ents;
		
		free(ctx->map_addr);
		free(ctx->map_name);
	}
#endif
	
	while(!bjx2_feof(fd))
	{
		if((tmn+64)>=tmap_max)
		{
			k=tmap_max;
			k=k+(k>>1);
			tmap_addr=realloc(tmap_addr, k*sizeof(bjx2_addr));
			tmap_name=realloc(tmap_name, k*sizeof(char *));
			tmap_mode=realloc(tmap_mode, k*sizeof(u16));
			tmap_max=k;
		}
	
		s=bjx2_fgets(tb, 255, fd);
		a=JX2R_SplitLine(tb);
		if(!a[0])
			continue;
		
		if(!strcmp(a[0], "U"))
		{
			tmap_addr[tmn]=0;
			tmap_name[tmn]=strdup(a[1]);
			tmap_mode[tmn]=a[0][0];
			tmn++;
			continue;
		}
		
		if(!a[1])
			continue;
		if(!a[2])
			continue;
		
//		sscanf(a[0], "%08X", &ta);
		sscanf(a[0], "%llX", &ta);
		
		tmap_addr[tmn]=ta;
		tmap_name[tmn]=strdup(a[2]);
		tmap_mode[tmn]=a[1][0];
		tmn++;
	}

#if 0
	for(i=0; i<tmn; i++)
	{
		for(j=i+1; j<tmn; j++)
		{
			sw=(tmap_addr[j]<tmap_addr[i]);

			if((tmap_mode[j]=='L') && (tmap_mode[i]!='L'))
				sw=0;
			if((tmap_mode[j]!='L') && (tmap_mode[i]=='L'))
				sw=1;
			
//			if(tmap_addr[j]<tmap_addr[i])
			if(sw)
			{
				ta=tmap_addr[j];
				s=tmap_name[j];
				k=tmap_mode[j];
				tmap_addr[j]=tmap_addr[i];
				tmap_name[j]=tmap_name[i];
				tmap_mode[j]=tmap_mode[i];
				tmap_addr[i]=ta;
				tmap_name[i]=s;
				tmap_mode[i]=k;
			}
		}
	}
#endif

//	BJX2_MapSortTMapBase(ctx,
//		tmap_addr, tmap_name, tmap_mode, 0, tmn);

	BJX2_MapSortTMapRec(ctx,
		tmap_addr, tmap_name, tmap_mode, 0, tmn, 32);

	for(i=0; i<tmn; i++)
	{
		if(tmap_mode[i]=='L')
			break;
	}
	tmnb=i;
	
	mn=ctx->n_map++;
	ctx->map_addr[mn]=malloc((tmn+64)*sizeof(bjx2_addr));
	ctx->map_name[mn]=malloc((tmn+64)*sizeof(char *));
	ctx->map_mode[mn]=malloc((tmn+64)*sizeof(u16));
	ctx->map_n_ents[mn]=tmnb;
	ctx->map_iname[mn]=NULL;
	ctx->map_pbase[mn]=pbase;

	ctx->map_addr_min[mn]=tmap_addr[0];
//	ctx->map_addr_max[mn]=tmap_addr[tmn-1];
	ctx->map_addr_max[mn]=tmap_addr[tmnb-1];
	
	ctx->map_b_lln[mn]=tmnb;
	ctx->map_n_lln[mn]=tmn-tmnb;
	
	if(imgname)
	{
		s=imgname+strlen(imgname);
		while((s>imgname) && (*s!='/') && (*s!='\\'))
			s--;
		if((*s=='/') || (*s=='\\'))
			s++;
		t=tb;
		while(*s && (*s!='.'))
			*t++=*s++;
		*t++=0;
		
		ctx->map_iname[mn]=strdup(tb);
	}
	
	memset(ctx->map_name[mn], 0, (tmn+64)*sizeof(char *));
	
	for(i=0; i<tmn; i++)
	{
		ctx->map_addr[mn][i]=tmap_addr[i];
		ctx->map_name[mn][i]=tmap_name[i];
		ctx->map_mode[mn][i]=tmap_mode[i];
	}
	return(0);
}

int BJX2_ContextSetupZero(BJX2_Context *ctx)
{
	BJX2_MemSpan *sp;
	int i, j, k;

	sp=BJX2_MemSpanForName(ctx, "ZERO");
	if(!sp)
	{
		printf("Failed find ZERO span\n");
		return(-1);
	}
	
	memset(sp->data, 0, 3*65536);
	for(i=0; i<32768; i++)
	{
		((u16 *)(sp->data))[32768+i]=0x3000;
		((u16 *)(sp->data))[65536+i]=0x3010;
	}
	
	
	return(0);
}

int BJX2_ContextLoadRom(BJX2_Context *ctx, char *name)
{
	char tb[256];
	BJX2_MemSpan *sp;
	byte *buf;
	BJX2_FILE *fd;
	int sz, sz1;
	int i, j, k;
	
	fd=bjx2_fopen(name, "rb");
	if(!fd)
	{
		printf("Failed open %s\n", name);
		return(-1);
	}

	bjx2_fseek(fd, 0, 2);
	sz=bjx2_ftell(fd);
	bjx2_fseek(fd, 0, 0);
	buf=malloc(sz);
	k=bjx2_fread(buf, 1, sz, fd);
	bjx2_fclose(fd);
	
	sp=BJX2_MemSpanForName(ctx, "ROM");
	if(!sp)
	{
		printf("Failed find ROM span\n");
		return(-1);
	}
	
	sz1=(sp->addr_lim-sp->addr_base)+1;
	if(sz>sz1)
	{
		printf("Oversized ROM, %d>%d\n", sz, sz1);
		sz=sz1;
	}
	memcpy(sp->data, buf, sz);
	free(buf);
	
	sprintf(tb, "%s.map", name);
	BJX2_ContextLoadMap(ctx, tb, NULL);
	
	return(0);
}




byte *_memlzcpy(byte *dst, byte *src, int sz)
{
	byte *cs, *ct, *cse;
	
	cs=src; cse=src+sz; ct=dst;
	while(cs<cse)
	{
		*ct++=*cs++;
	}
	return(ct);
}

byte *TKPE_UnpackL4(byte *ct, byte *ibuf, int isz)
{
	byte *cs, *cse;
	register byte *cs1, *cs1e, *ct1;
//	byte *cs1, *cs1e, *ct1;
	register int tg, lr, ll, ld;
//	int tg, lr, ll, ld;
	int i;
	
//	__hint_use_egpr();
	
	tg=0; lr=0; ll=0; ld=0;
	
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

#if 0
		ct1=ct; cs1=cs; cs1e=cs+lr;
		while(cs1<cs1e)
		{
			*(u64 *)ct1=*(u64 *)cs1;
			ct1+=8; cs1+=8;
		}
#endif

//		_memlzcpyf(ct, cs, lr);
		_memlzcpy(ct, cs, lr);
		ct+=lr; cs+=lr;
		
		if((cs+1)>=cse)
			break;

		ll=(tg&15)+4;
		
//		ld=tkfat_getWord(cs);
		ld=*(u16 *)cs;
		cs+=2;
		if(!ld)
		{
			if(ll==5)
				continue;
			break;
		}
//		ll=(tg&15)+4;
		if(ll==19)
		{
			i=*cs++;
			while(i==255)
				{ ll+=255; i=*cs++; }
			ll+=i;
		}

//		ct=_memlzcpyf(ct, ct-ld, ll);
		ct=_memlzcpy(ct, ct-ld, ll);

#if 0
		cs1=ct-ld; cs1e=cs1+ll;
//		if(ld>=8)
		if(ld>8)
		{
			ct1=ct;
			while(cs1<cs1e)
			{
				((u64 *)ct1)[0]=((u64 *)cs1)[0];
				((u64 *)ct1)[1]=((u64 *)cs1)[1];
				ct1+=16; cs1+=16;
			}
//				{ *(u64 *)ct1=*(u64 *)cs1; ct1+=8; cs1+=8; }
			ct+=ll;
		}else
			if(ld>4)
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
		}else
		{
			while(cs1<cs1e)
				{ *ct++=*cs1++; }
		}
#endif
	}
	
	return(ct);
}

// #ifndef __BJX2__
#if 1
int TKPE_DecodeBufferRP2(
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	u32 tag;
	byte *cs, *ct, *cse;
	int pl, pd;
	int rl, l, d;
	u64 t0;
	int t1, t2;
	
	cs=ibuf; cse=ibuf+ibsz;
	ct=obuf;
	pl=0; pd=0;
	
//	while(1)
	while(cs<cse)
	{
		t0=*(u64 *)cs;
		if(!(t0&0x01))
		{
			cs+=2;
			rl=(t0>>1)&7;
			l=((t0>>4)&7)+3;
			d=(t0>>7)&511;
		}else
			if(!(t0&0x02))
		{
			cs+=3;
			rl=(t0>>2)&7;
			l=((t0>>5)&63)+4;
			d=(t0>>11)&8191;
		}else
			if(!(t0&0x04))
		{
			cs+=4;
			rl=(t0>>3)&7;
			l=((t0>>6)&511)+4;
			d=(t0>>15)&131071;
		}else
			if(!(t0&0x08))
		{
			cs++;
			t1=(t0>>4)&15;
			rl=(t1+1)*8;
//			W_RawCopyB(ct, cs, rl);
			_memlzcpy(ct, cs, rl);
			cs+=rl;
			ct+=rl;
			continue;
		}else
			if(!(t0&0x10))
		{
			/* Long Match */
			cs++;
			rl=(t0>>5)&7;
			t1=t0>>8;
			if(!(t1&1))
				{ l=((t1>>1)&0x007F)+4; cs+=1; t2=t0>>16; }
			else
				{ l=((t1>>2)&0x3FFF)+4; cs+=2; t2=t0>>24; }
			if(!(t2&1))
				{ d=((t2>>1)&0x007FFF); cs+=2; }
			else
				{ d=((t2>>2)&0x3FFFFF); cs+=3; }
		}else
			if(!(t0&0x20))
		{
			cs++;
			rl=(t0>>6)&3;
			if(!rl)break;
			*(u32 *)ct=*(u32 *)cs;
			cs+=rl;
			ct+=rl;
			continue;
		}else
			if(!(t0&0x40))
		{
			/* Long Raw */
			cs+=2;
			t1=(t0>>7)&511;
			rl=(t1+1)*8;
//			W_RawCopyB(ct, cs, rl);
			_memlzcpy(ct, cs, rl);
			cs+=rl;
			ct+=rl;
			continue;
		}else
		{
			JX2_DBGBREAK
		}

		*(u64 *)ct=*(u64 *)cs;
		cs+=rl;
		ct+=rl;
//		W_MatchCopy2(ct, l, d);
		_memlzcpy(ct, ct-d, l);
		ct+=l;
	}
	
	return(ct-obuf);
}
#endif

byte *TKPE_UnpackBuffer(byte *ct, byte *ibuf, int isz, int cmp)
{
	byte *ct1;
	int rsz;

	if((cmp==4) || (cmp==6))
	{
		return(TKPE_UnpackL4(ct, ibuf, isz));
	}

	if(cmp==3)
	{
		rsz=TKPE_DecodeBufferRP2(ibuf, ct, isz, 999999);
		return(ct+rsz);
	}
	
	return(NULL);
}

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


u32 TKPE_CalculateImagePel4BChecksum(byte *buf, int size)
{
	byte *cs, *cse;
	u32 v, v0, v1, v2, v3;
	u64 acc_lo, acc_hi;
	u32 csum;
	
	cs=buf;
	cse=cs+size;
	acc_lo=1;
	acc_hi=0;
	while(cs<cse)
	{
		v0=((u32 *)cs)[0];	v1=((u32 *)cs)[1];
		v2=((u32 *)cs)[2];	v3=((u32 *)cs)[3];
//		v0=get_u32le(cs+0);	v1=get_u32le(cs+ 4);
//		v2=get_u32le(cs+8);	v3=get_u32le(cs+12);
		acc_lo=acc_lo+v0;	acc_hi=acc_hi+acc_lo;
		acc_lo=acc_lo+v1;	acc_hi=acc_hi+acc_lo;
		acc_lo=acc_lo+v2;	acc_hi=acc_hi+acc_lo;
		acc_lo=acc_lo+v3;	acc_hi=acc_hi+acc_lo;
		cs+=16;
	}
	acc_lo=((u32)acc_lo)+(acc_lo>>32);
	acc_lo=((u32)acc_lo)+(acc_lo>>32);
	acc_hi=((u32)acc_hi)+(acc_hi>>32);
	acc_hi=((u32)acc_hi)+(acc_hi>>32);
	csum=(u32)(acc_lo^acc_hi);
//	csum+=size;
	return(csum);
}

u16 TKPE_CalculateSmallByteCsum(byte *buf, int sz)
{
	byte *cs, *cse;
	u32 ac0, ac1, csum;
	
	if(!sz)
		return(0);
	
	cs=buf; cse=buf+sz;
	ac0=1; ac1=0;
	while(cs<cse)
	{
		ac0+=*cs;
		ac1+=ac0;
		cs++;
	}
	ac0=((u16)ac0)+(ac0>>16);
	ac1=((u16)ac1)+(ac1>>16);
	ac0=((u16)ac0)+(ac0>>16);
	ac1=((u16)ac1)+(ac1>>16);
	csum=(u16)(ac0^ac1);
	return(csum);
}

u32 TKPE_CalculateImagePel4BChecksumAc(byte *buf, int sz)
{
	u32 csum0, csum1;

	if(!(sz&15))
	{
		csum0=TKPE_CalculateImagePel4BChecksum(buf, sz);
		return(csum0);
	}

	csum0=TKPE_CalculateImagePel4BChecksum(buf, sz&(~15));
	csum1=TKPE_CalculateSmallByteCsum(buf+(sz&(~15)), sz&15);
	return(csum0^csum1);
}


