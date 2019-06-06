#define TK_MSGBUFA_RX	0xAE000000
#define TK_MSGBUFA_TX	0xAF000000

#define TK_MSGBUFA_REGS	0xA000E300

// #define TK_MSGBUF_SZ	0x400000
// #define TK_MSGBUF_MSK	0x3FFFFF

#define TK_MSGBUF_RX	((volatile u32 *)TK_MSGBUFA_RX)
#define TK_MSGBUF_TX	((volatile u32 *)TK_MSGBUFA_TX)
#define TK_MSGBUF_REGS	((volatile u32 *)TK_MSGBUFA_REGS)

#define TK_MSGBUF_RXSP	TK_MSGBUF_REGS[0]
#define TK_MSGBUF_RXEP	TK_MSGBUF_REGS[1]
#define TK_MSGBUF_TXSP	TK_MSGBUF_REGS[2]
#define TK_MSGBUF_TXEP	TK_MSGBUF_REGS[3]

#define TK_MSGBUF_SZ	(TK_MSGBUF_REGS[5]+1)
#define TK_MSGBUF_MSK	(TK_MSGBUF_REGS[5]  )

int TK_MsgBuf_GetRxSize()
{
	int sp, ep, sz;
	
	sp=TK_MSGBUF_RXSP;
	ep=TK_MSGBUF_RXEP;
	if(sp>ep)
		{ sz=(ep+TK_MSGBUF_SZ)-sp; }
	else
		{ sz=(ep-sp); }
	return(sz);
}

int TK_MsgBuf_GetTxFree()
{
	int sp, ep, sz;
	
	sp=TK_MSGBUF_TXSP;
	ep=TK_MSGBUF_TXEP;
	if(ep>=sp)
		{ sz=(sp+TK_MSGBUF_SZ)-ep; }
	else
		{ sz=(sp-ep); }
	return(sz);
}

int TK_MsgBuf_SendMessageWords(u32 *msg, int msz)
{
	u32 *cs;
	int txf;
	int sp, ep;
	int i, j;
	
	txf=TK_MsgBuf_GetTxFree();
	if(msz>=txf)
	{
		return(0);
	}

	sp=TK_MSGBUF_TXSP;
	ep=TK_MSGBUF_TXEP;
	
	i=ep; cs=msg; j=msz;
	while(j--)
	{
		TK_MSGBUF_TX[i]=*cs++;
		i=(i+1)&TK_MSGBUF_MSK;
	}
	TK_MSGBUF_TXEP=i;
	return(1);
}

int TK_MsgBuf_GetMessageWords(u32 *msg, int msz)
{
	u32 *ct;
	u32 tw1, tw2;
	int txf, tg;
	int sp, ep, sz, sp1;
	int i, j, k;
	
	sp=TK_MSGBUF_RXSP;
	ep=TK_MSGBUF_RXEP;
	
	if(sp==ep)
		return(0);
	
	sp1=(sp+1)&TK_MSGBUF_MSK;
	tw1=TK_MSGBUF_RX[sp];
	tw2=TK_MSGBUF_RX[sp1];
	if((tw1&0x80000000))
	{
		tg=(u16)tw1;
		sz=(tw1>>16)&32767;
	}else
	{
		tg=tw1;
		sz=tw2;
	}
	
	i=sp; ct=msg; j=(sz+3)/4;
	while(j--)
	{
		k=TK_MSGBUF_RX[i];
		i=(i+1)&TK_MSGBUF_MSK;
		*ct++=k;
	}
	TK_MSGBUF_RXSP=i;
	return(sz);
}

int TK_MsgBuf_SendMessage(void *msg, int msz)
{
	return(TK_MsgBuf_SendMessageWords(msg, (msz+3)/4));
}

int TK_MsgBuf_GetMessage(void *msg, int msz)
{
	return(TK_MsgBuf_GetMessageWords(msg, (msz+3)/4));
}
