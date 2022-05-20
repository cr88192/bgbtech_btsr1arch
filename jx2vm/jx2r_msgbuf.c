/*
 Copyright (c) 2018-2022 Brendan G Bohannon

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


int BJX2_MemMsgBufCb_GetByte(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

int BJX2_MemMsgBufCb_GetWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

s32 BJX2_MemMsgBufCb_GetDWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	s64 rvq;
	int ra, rv, vn;

	ra=addr-sp->addr_base;
	
	rv=0;

//	if((ra>>2)>=1024)
//	{
//		if((ra>=0x1800) && (ra<0x1A00))
//		{
//			rv=SMus_GetRegister(ctx, (ra-0x1800)>>2);
//			return(rv);
//		}
//	}

	if(((addr>>24)&1)==0)
	{
		if(ctx->msgbuf_rx)
		{
			rv=ctx->msgbuf_rx[(ra>>2)&(ctx->msgbuf_msk)];
		}
	}

	if(((addr>>24)&1)==1)
	{
		if(ctx->msgbuf_tx)
		{
			rv=ctx->msgbuf_tx[(ra>>2)&(ctx->msgbuf_msk)];
		}
	}

	return(rv);
}


int BJX2_MemMsgBufCb_SetByte(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int val)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

int BJX2_MemMsgBufCb_SetWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int val)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

int BJX2_MemMsgBufCb_SetDWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s32 val)
{
	int ra;
	ra=addr-sp->addr_base;

//	if((ra>>2)>=1024)
//	{
//		if((ra>=0x1800) && (ra<0x1A00))
//		{
//			SMus_SetRegister(ctx, (ra-0x1800)>>2, val);
//			return(0);
//		}
//	}

	if(((addr>>24)&1)==0)
	{
		if(ctx->msgbuf_rx)
		{
			ctx->msgbuf_rx[(ra>>2)&(ctx->msgbuf_msk)]=val;
		}
	}

	if(((addr>>24)&1)==1)
	{
		if(ctx->msgbuf_tx)
		{
			ctx->msgbuf_tx[(ra>>2)&(ctx->msgbuf_msk)]=val;
		}
	}

	return(0);
}

int BJX2_MemDefineMsgBuf(BJX2_Context *ctx,
	char *name, bjx2_addr base, bjx2_addr lim)
{
	BJX2_MemSpan *sp;
	
	if(!ctx->msgbuf_rx)
	{
		ctx->msgbuf_rx=malloc(1<<18);
		ctx->msgbuf_tx=malloc(1<<18);
		ctx->msgbuf_msk=((1<<18)/4)-1;
	}
	
	sp=BJX2_MemAllocSpan(ctx);
	
	sp->name=name;
	sp->addr_base=base;
	sp->addr_lim=lim;
	sp->addr_sz=lim-base;
//	sp->data=malloc((lim-base)+8);
	
	sp->GetByte=BJX2_MemMsgBufCb_GetByte;
	sp->GetWord=BJX2_MemMsgBufCb_GetWord;
	sp->GetDWord=BJX2_MemMsgBufCb_GetDWord;
	
	sp->SetByte=BJX2_MemMsgBufCb_SetByte;
	sp->SetWord=BJX2_MemMsgBufCb_SetWord;
	sp->SetDWord=BJX2_MemMsgBufCb_SetDWord;
	
	BJX2_MemAddSpan(ctx, sp);
	return(0);
}

int BJX2_MsgBuf_SendMessageDWord(BJX2_Context *ctx,
	u32 *msg, int msz)
{
	u32 *cs;
	int spos, epos, szf;
	int cp, n;
	
	spos=ctx->msgbuf_rxspos;
	epos=ctx->msgbuf_rxepos;
	
	if(epos>=spos)
		{ szf=(spos+(ctx->msgbuf_msk+1))-epos; }
	else
		{ szf=spos-epos; }
	
	if(msz>=szf)
	{
		return(0);
	}
		
	cp=epos; n=msz; cs=msg;
	while(n--)
	{
		ctx->msgbuf_rx[cp]=*cs++;
		cp=(cp+1)&(ctx->msgbuf_msk);
	}
	ctx->msgbuf_rxepos=cp;
	return(1);
}

int BJX2_MsgBuf_SendMessage(BJX2_Context *ctx,
	void *msg, int msz)
{
	return(BJX2_MsgBuf_SendMessageDWord(ctx, msg, (msz+3)/4));
}

int BJX2_MsgBuf_GetMessageDWord(BJX2_Context *ctx,
	u32 *msg, int msz)
{
	u32 *ct;
	u32 tw1, tw2;
	int spos, epos, szf;
	int cp, n, cp1, cp2;
	int tg, sz;
	
	spos=ctx->msgbuf_txspos;
	epos=ctx->msgbuf_txepos;
	
	if(spos>epos)
		{ szf=(epos+(ctx->msgbuf_msk+1))-spos; }
	else
		{ szf=epos-spos; }
	
	if(!szf)
	{
		return(0);
	}
	
	cp1=spos;
	cp2=(cp1+1)&(ctx->msgbuf_msk);
	tw1=ctx->msgbuf_tx[cp1];
	tw2=ctx->msgbuf_tx[cp2];
	
	if(tw1&0x80000000)
	{
		tg=(u16)tw1;
		sz=(tw1>>16)&32767;
	}else
	{
		tg=tw1;
		sz=tw2;
	}
	
	cp=spos; n=(sz+3)/4; ct=msg;
	while(n--)
	{
		*ct++=ctx->msgbuf_tx[cp];
		cp=(cp+1)&(ctx->msgbuf_msk);
	}
	ctx->msgbuf_rxspos=cp;
	return(sz);
}

int BJX2_MsgBuf_GetMessage(BJX2_Context *ctx,
	void *msg, int msz)
{
	return(BJX2_MsgBuf_GetMessageDWord(ctx, msg, (msz+3)/4));
}

int BJX2_MsgBuf_GetMessageInfo(void *buf, u32 *rtg, u32 *rsz)
{
	u32 tw1, tw2;
	int tg, sz, mo;

	tw1=((u32 *)buf)[0];
	tw2=((u32 *)buf)[1];
	
	if(tw1&0x80000000)
	{
		tg=(u16)tw1;
		sz=(tw1>>16)&32767;
		mo=4;
	}else
	{
		tg=tw1;
		sz=tw2;
		mo=8;
	}
	
	*rtg=tg;
	*rsz=sz;
	return(mo);
}
