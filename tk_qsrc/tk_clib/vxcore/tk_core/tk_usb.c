u16 tk_usb_rxbuf1[1024];
u16 tk_usb_rxbuf2[1024];
u16 tk_usb_txbuf1[1024];
u16 tk_usb_txbuf2[1024];

int TKUSB_SendPacket(int port, byte *buf, int sz)
{
	volatile u64 *txba;
	volatile u64 *txra;
	u16 *txbuf;
	u64 v;
	int i, n;

//	tk_dbg_printf("TKUSB_SendPacket: %d\n", sz);

	if(port==0)
	{
		txba=(volatile u64 *)0xFFFFF0008800ULL;
		txra=(volatile u64 *)0xFFFFF000E810ULL;
	}else
		if(port==1)
	{
		txba=(volatile u64 *)0xFFFFF0009800ULL;
		txra=(volatile u64 *)0xFFFFF000E830ULL;
	}else
	{
		return(-1);
	}
	
	v=txra[1];
	while(v&0xFF)
		{ v=txra[1]; }

	txbuf=tk_usb_txbuf1;
	for(i=0; i<sz; i++)
		txbuf[i]=buf[i];
	txbuf[sz]=0x100;
	
	n=(sz+4)>>2;
	for(i=0; i<n; i++)
	{
		txba[i]=*(u64 *)(txbuf+i*4);
	}

	txra[0]=((u64)(sz+1))<<32;

	v=txra[1];
	while(v&0xFF)
		{ v=txra[1]; }

//	txra[0]=0;
//	txra[1]=sz+1;
}

int TKUSB_SendPacketW(int port, u16 *buf, int sz)
{
	volatile u64 *txba;
	volatile u64 *txra;
	u16 *txbuf;
	u64 v;
	int i, n;

//	tk_dbg_printf("TKUSB_SendPacket: %d\n", sz);

	if(port==0)
	{
		txba=(volatile u64 *)0xFFFFF0008800ULL;
		txra=(volatile u64 *)0xFFFFF000E810ULL;
	}else
		if(port==1)
	{
		txba=(volatile u64 *)0xFFFFF0009800ULL;
		txra=(volatile u64 *)0xFFFFF000E830ULL;
	}else
	{
		return(-1);
	}
	
	v=txra[1];
	while(v&0xFF)
		{ v=txra[1]; }

	txbuf=tk_usb_txbuf1;
	for(i=0; i<sz; i++)
		txbuf[i]=buf[i];
	
	n=(sz+4)>>2;
	for(i=0; i<n; i++)
	{
		txba[i]=*(u64 *)(txbuf+i*4);
	}

	txra[0]=((u64)(sz+0))<<32;

	v=txra[1];
	while(v&0xFF)
		{ v=txra[1]; }

//	txra[0]=0;
//	txra[1]=sz+1;
}


int TKUSB_GetPacket(int port, byte *buf, int *rsz)
{
	volatile u64 *txba;
	volatile u64 *txra;
	u16 *txbuf;
	u64 txpos, txs, txe, ks, ke, v;
	int i, j, k, n;

	if(port==0)
	{
		txba=(volatile u64 *)0xFFFFF0008000ULL;
		txra=(volatile u64 *)0xFFFFF000E800ULL;
		txbuf=tk_usb_rxbuf1;
	}else
		if(port==1)
	{
		txba=(volatile u64 *)0xFFFFF0009000ULL;
		txra=(volatile u64 *)0xFFFFF000E820ULL;
		txbuf=tk_usb_rxbuf2;
	}else
	{
		return(-1);
	}
	
	v=txra[1];
	while(v&0xFF)
		{ v=txra[1]; }

	txpos=txra[0];
	txs=(u32)(txpos>> 0);
	txe=(u32)(txpos>>32);
	
	if(txs==txe)
	{
		*rsz=0;
		return(0);
	}
	
	ks=txs>>2;
	ke=(txe+3)>>2;
	n=((ke-ks)&255)+1;
	for(i=0; i<n; i++)
	{
		j=(ks+i)&255;
		((u64 *)txbuf)[j]=txba[j];
	}
	
	while((txbuf[txs]&0x100) && (txs!=txe))
		txs=(txs+1)&1023;
	
	if(txs==txe)
	{
		((volatile u32 *)txra)[0]=txe;
		*rsz=0;
		return(0);
	}
	
	n=(txe-txs)&1023;
	for(i=0; i<n; i++)
	{
		j=(txs+i)&1023;
		k=txbuf[j];
		if(k&0x100)
		{
			break;
		}
		buf[i]=k;
	}
	*rsz=i;
	
	if(i==n)
	{
		/* incomplete message, leave for later. */
		*rsz=0;
		return(0);
	}

	for(; i<n; i++)
	{
		j=(txs+i)&1023;
		k=txbuf[j];
		if(!(k&0x100))
			break;
	}
	
	txs=(txs+i)&1023;
//	((volatile u32 *)txra)[0]=txs;

	txpos=(txpos&(~1023LL))|txs;
	txra[0]=txpos;
	return(1);
}

int TKUSB_GetLinkState(int port)
{
	volatile u64 *txba;
	volatile u64 *txra;
	u16 *txbuf;
	u64 v;
	int i, j, k, n;

	if(port==0)
	{
		txba=(volatile u64 *)0xFFFFF0008000ULL;
		txra=(volatile u64 *)0xFFFFF000E800ULL;
		txbuf=tk_usb_rxbuf1;
	}else
		if(port==1)
	{
		txba=(volatile u64 *)0xFFFFF0009000ULL;
		txra=(volatile u64 *)0xFFFFF000E820ULL;
		txbuf=tk_usb_rxbuf2;
	}else
	{
		return(0);
	}
	
	v=txra[1];
	return((v>>8)&3);
}

int TKUSB_WaitNotBusy(int port)
{
//	volatile u64 *txba;
	volatile u64 *txra;
//	u16 *txbuf;
	u64 v;
	int i, j, k, n;

	if(port==0)
	{
//		txba=(volatile u64 *)0xFFFFF0008000ULL;
		txra=(volatile u64 *)0xFFFFF000E800ULL;
//		txbuf=tk_usb_rxbuf1;
	}else
		if(port==1)
	{
//		txba=(volatile u64 *)0xFFFFF0009000ULL;
		txra=(volatile u64 *)0xFFFFF000E820ULL;
//		txbuf=tk_usb_rxbuf2;
	}else
	{
		return(0);
	}
	
	v=txra[1];
	while(v&0xFF)
		{ v=txra[1]; }
	return(0);
}

byte tk_usb_datarov[4];

byte TKUSB_BitFlip8(byte v)
{
	static const byte fliptab[16]={
		0x0, 0x8, 0x4, 0xC, 0x2, 0xA, 0x6, 0xE,
		0x1, 0x9, 0x5, 0xD, 0x3, 0xB, 0x7, 0xF};
	byte v1;
	v1=(fliptab[v&15]<<4)|(fliptab[(v>>4)&15]);
	return(v1);
}

u16 TKUSB_DoCrc16Step(u16 crc0, byte value)
{
	u16 crc;
	int i;
	crc=crc0^(((u16)value)<<8);
	for(i=0; i<8; i++)
	{
		if(crc&0x8000)
//			{ crc=(crc<<1)^0x1021; }
			{ crc=(crc<<1)^0x8005; }
		else
			{ crc=crc<<1; }
	}
	return(crc);
}

u16 TKUSB_Crc16(byte *data, u32 sz)
{
	byte *cs, *cse;
	u16 crc;

	crc=0xFFFF;
	cs=data; cse=data+sz;
	while (cs<cse)
	{
//		crc=TKUSB_DoCrc16Step(crc, *cs++);
		crc=TKUSB_DoCrc16Step(crc, TKUSB_BitFlip8(*cs++));
	}
	return(crc);
}

byte TKUSB_Crc5(u32 v, int vl, int ival)
{
	static const byte bvtab[19] = {
		0x1E, 0x15, 0x03, 0x06, 0x0C, 0x18, 0x19, 0x1B,
		0x1F, 0x17, 0x07, 0x0E, 0x1C, 0x11, 0x0B, 0x16,
		0x05, 0x0A, 0x14
	};
	byte rv;
	int i;

	rv = ival;
	for(i=0; i<vl; i++)
	{
		if (v&(1<<i))
			{ rv^=bvtab[19-vl+i]; }
	}
	return(rv);
}

byte TKUSB_Crc5_11b(u16 val)
	{ return(TKUSB_Crc5(val, 11, 0x02)); }
byte TKUSB_Crc5_19b(u32 val)
	{ return(TKUSB_Crc5(val, 19, 0x1D)); }


int TKUSB_SendDataPacket(int port, byte *msg, int sz)
{
	static byte msgbuf[1536];
	byte bmsg[640];
	s64 tt, tt1, te;
	int rt, msgsz, tg;
	int crc;

	crc=~TKUSB_Crc16(msg, sz);

//	bmsg[0]=tk_usb_datarov[port]?0x4B:0xC3;
//	tk_usb_datarov[port]=!tk_usb_datarov[port];
	bmsg[0]=0xC3;
	memcpy(bmsg+1, msg, sz);
	bmsg[sz+(1+0)]=crc;
	bmsg[sz+(1+1)]=crc>>8;
	TKUSB_SendPacket(port, bmsg, sz+3);

	tt=TK_GetTimeUs();
	tt1=tt+1000;
	te=tt+100000;
	while(tt<te)
	{
		rt=TKUSB_GetPacket(port, msgbuf, &msgsz);
		if(rt>0)
		{
			tk_dbg_printf("TKUSB_SendDataPacket: Got %02X\n", msgbuf[0]);
			if(msgbuf[0]==0xD2)
			{
				/* ACK */
				break;
			}
			if(msgbuf[0]==0x5A)
			{
				/* NAK */
				TKUSB_SendPacket(port, bmsg, sz+3);
				tt=TK_GetTimeUs();
				tt1=tt+5000;
				continue;
			}
			break;
		}
		
		if(tt<tt1)
		{
			tt=TK_GetTimeUs();
			continue;
		}

		TKUSB_SendPacket(port, bmsg, sz+3);
		tt=TK_GetTimeUs();
		tt1=tt+5000;
	}

	if(tt>=te)
	{
		tk_dbg_printf("TKUSB_SendDataPacket: Timeout\n");
		return(-1);
	}

	return(0);
}


int TKUSB_SendSetupAndDataPacket(
	int port, int addr, int endp,
	byte *msg, int sz)
{
	static byte msgbuf[1536];
	u16 bmsg[640];
	u16 *bct;
	s64 tt, tt1, te;
	int rt, msgsz, tg;
	int crc;
	int i;
	
	tg=addr|(endp<<7);
	crc=TKUSB_Crc5_11b(tg);
	tg|=crc<<11;

	bct=bmsg;
	bct[0]=0x2D;
	bct[1]=tg;
	bct[2]=tg>>8;
	bct[3]=0x100;
	
	bct+=4;

	crc=~TKUSB_Crc16(msg, sz);

//	bmsg[0]=tk_usb_datarov[port]?0x4B:0xC3;
//	tk_usb_datarov[port]=!tk_usb_datarov[port];
	bct[0]=0xC3;
	for(i=0; i<sz; i++)
		bct[i+1]=msg[i];
//	memcpy(bmsg+1, msg, sz);
	bct[sz+(1+0)]=(crc>>0)&0xFF;
	bct[sz+(1+1)]=(crc>>8)&0xFF;
	bct[sz+(1+2)]=0x100;
	bct+=sz+1+3;
	
	TKUSB_SendPacketW(port, bmsg, bct-bmsg);

	tt=TK_GetTimeUs();
	tt1=tt+1000;
	te=tt+100000;
	while(tt<te)
	{
		msgsz=0;
		rt=TKUSB_GetPacket(port, msgbuf, &msgsz);
		if(rt>0)
		{
			tk_dbg_printf("TKUSB_SendDataPacket: Got %02X\n", msgbuf[0]);
			if(msgbuf[0]==0xD2)
			{
				/* ACK */
				break;
			}
			if(msgbuf[0]==0x5A)
			{
				/* NAK */
//				TKUSB_SendPacket(port, bmsg, sz+3);
				TKUSB_SendPacketW(port, bmsg, bct-bmsg);
				tt=TK_GetTimeUs();
				tt1=tt+5000;
				continue;
			}
			break;
		}
		
		if(tt<tt1)
		{
			tt=TK_GetTimeUs();
			continue;
		}

//		TKUSB_SendPacket(port, bmsg, sz+3);
		TKUSB_SendPacketW(port, bmsg, bct-bmsg);
		tt=TK_GetTimeUs();
		tt1=tt+5000;
	}

	if(tt>=te)
	{
		tk_dbg_printf("TKUSB_SendDataPacket: Timeout\n");
		return(-1);
	}

	return(0);
}

int TKUSB_SendSetupToken(int port, int addr, int endp)
{
	byte bmsg[16];
	int tg, crc;
	
	tg=addr|(endp<<7);
	crc=TKUSB_Crc5_11b(tg);
	tg|=crc<<11;

	bmsg[0]=0x2D;
	bmsg[1]=tg;
	bmsg[2]=tg>>8;
	TKUSB_SendPacket(port, bmsg, 3);
	return(0);
}

int TKUSB_SendInToken(int port, int addr, int endp)
{
	byte bmsg[16];
	int tg, crc;
	
	tg=addr|(endp<<7);
	crc=TKUSB_Crc5_11b(tg);
	tg|=crc<<11;

	bmsg[0]=0x69;
	bmsg[1]=tg;
	bmsg[2]=tg>>8;
	TKUSB_SendPacket(port, bmsg, 3);
	return(0);
}

int TKUSB_SendAckToken(int port)
{
	byte bmsg[16];

	bmsg[0]=0xD2;
	TKUSB_SendPacket(port, bmsg, 1);
	return(0);
}

int TKUSB_SendReset(int port)
{
	u16 bmsg[16];

	bmsg[0]=0x101;
	TKUSB_SendPacketW(port, bmsg, 1);
	return(0);
}

int TKUSB_SetAddress(int port, int oldaddr, int newaddr)
{
	byte bmsg[16];
	int rt;

//	TKUSB_SendSetupToken(port, oldaddr, 0);

	bmsg[0]=0x00;
	bmsg[1]=0x05;
	bmsg[2]=newaddr;
	bmsg[3]=0x00;
	bmsg[4]=0x00;
	bmsg[5]=0x00;
	bmsg[6]=0x00;
	bmsg[7]=0x00;

//	rt=TKUSB_SendDataPacket(port, bmsg, 8);
	rt=TKUSB_SendSetupAndDataPacket(port, oldaddr, 0, bmsg, 8);
	return(rt);
}


int TKUSB_SetConfiguration(int port, int addr, int index)
{
	byte bmsg[16];
	int rt;

//	TKUSB_SendSetupToken(port, addr, 0);

	bmsg[0]=0x00;
	bmsg[1]=0x09;
	bmsg[2]=index;
	bmsg[3]=0x00;
	bmsg[4]=0x00;
	bmsg[5]=0x00;
	bmsg[6]=0x00;
	bmsg[7]=0x00;
//	rt=TKUSB_SendDataPacket(port, bmsg, 8);
	rt=TKUSB_SendSetupAndDataPacket(port, addr, 0, bmsg, 8);
	return(rt);
}

int TKUSB_SendGetDescriptor(int port, int addr,
	int dtype, int dindex, int sz)
{
	byte bmsg[16];
	int rt;

//	TKUSB_SendSetupToken(port, addr, 0);

	bmsg[0]=0x80;
	bmsg[1]=0x06;
	bmsg[2]=dindex;
	bmsg[3]=dtype;
	bmsg[4]=0x00;
	bmsg[5]=0x00;
	bmsg[6]=sz;
	bmsg[7]=sz>>8;

//	rt=TKUSB_SendDataPacket(port, bmsg, 8);
	rt=TKUSB_SendSetupAndDataPacket(port, addr, 0, bmsg, 8);
	return(rt);
}

int TKUSB_GetInPacket(int port, int addr, int endp,
	byte *buf, int sz)
{
	static byte msgbuf[1536];
	s64 tt, te;
	int rt, msgsz, tg;

	TKUSB_SendInToken(port, addr, 0);

	tt=TK_GetTimeUs();
	te=tt+100000;
	while(tt<te)
	{
		msgsz=0;
		rt=TKUSB_GetPacket(port, msgbuf, &msgsz);
		if(rt>0)
		{
			tk_dbg_printf("TKUSB_GetInPacket: Got %02X\n", msgbuf[0]);

//			TKUSB_HandlePacket(0, msgbuf, msgsz);
//			continue;
			if((msgbuf[0]==0xC3) || (msgbuf[0]==0x4B))
			{
				memcpy(buf, msgbuf, sz);
//				TKUSB_HandleKbReport(buf+1);
				TKUSB_SendAckToken(port);
			}else
				if(msgbuf[0]==0x5A)
			{
			}else
				if(msgbuf[0]==0x1E)
			{
				TKUSB_SendAckToken(port);
			}else
			{
			}
			break;
		}
		tt=TK_GetTimeUs();
	}
	if(tt>=te)
	{
		tk_dbg_printf("TKUSB_GetInPacket: Timeout\n");
		return(-1);
	}
	return(0);
}

int TKUSB_GetDescriptor(int port, int addr,
	int dtype, int dindex, byte *buf, int sz)
{
	int rt;

	rt=TKUSB_SendGetDescriptor(port, addr, dtype, dindex, sz);
	if(rt<0)
	{
		memset(buf, 0, sz);
		return(rt);
	}

	rt=TKUSB_GetInPacket(port, addr, 0, buf, sz);
	if(rt<0)
	{
		memset(buf, 0, sz);
		return(rt);
	}

	return(0);
}

int TKUSB_GetReport(int port, int addr)
{
	byte bmsg[16];

	if(TKUSB_GetLinkState(port)<=0)
		return(-1);

	TKUSB_SendSetupToken(port, addr, 0);

	bmsg[0]=0x1A;
	bmsg[1]=0x01;
	bmsg[2]=0x00;
	bmsg[3]=0x01;
	bmsg[4]=0x08;
	bmsg[5]=0x00;
	bmsg[6]=0x00;
	bmsg[7]=0x00;
	TKUSB_SendDataPacket(port, bmsg, 8);

	TKUSB_SendInToken(port, addr, 0);
	return(0);
}

int TKUSB_PumpDelay(int port, int usec)
{
	static byte msgbuf[1536];
	s64 tt, te;
	int rt, msgsz, tg;
	
	tt=TK_GetTimeUs();
	te=tt+usec;
	while(tt<te)
	{
		msgsz=0;
		rt=TKUSB_GetPacket(port, msgbuf, &msgsz);
		if(rt>0)
		{
			tk_dbg_printf("TKUSB_PumpDelay: Got %02X\n", msgbuf[0]);
			if((msgbuf[0]==0xC3) || (msgbuf[0]==0x4B))
			{
				TKUSB_SendAckToken(port);
			}else
				if(msgbuf[0]==0x5A)
			{
			}else
				if(msgbuf[0]==0x1E)
			{
				TKUSB_SendAckToken(port);
			}else
			{
			}
		}
		tt=TK_GetTimeUs();
	}
	return(0);
}

static const byte tkusb_kbscan2key_sh0[256]={
	0x00, 0x00, 0x00, 0x00,  0x61, 0x62, 0x63, 0x64,	/* 00..07 */
	0x65, 0x66, 0x67, 0x68,  0x69, 0x6A, 0x6B, 0x6C,	/* 08..0F */
	0x6D, 0x6E, 0x6F, 0x70,  0x71, 0x72, 0x73, 0x74,	/* 10..17 */
	0x75, 0x76, 0x77, 0x78,  0x79, 0x7A, 0x31, 0x32,	/* 18..1F */

	0x33, 0x34, 0x35, 0x36,  0x37, 0x38, 0x39, 0x30,	/* 20..27 */
	0x0D, 0x1B, 0x08, 0x09,  0x20, 0x2D, 0x3D, 0x5B,	/* 28..2F */
	0x5D, 0x5C, 0x00, 0x3B,  0x27, 0x60, 0x2C, 0x2E,	/* 30..37 */
	0x2F, 0x00, 0x87, 0x88,  0x89, 0x8A, 0x8B, 0x8C,	/* 38..3F */

	0x8D, 0x8E, 0x8F, 0x90,  0x91, 0x92, 0x00, 0x00,	/* 40..47 */
	0x99, 0x93, 0x97, 0x96,  0x7F, 0x98, 0x95, 0x83,	/* 48..4F */
	0x82, 0x81, 0x80, 0x00,  0xCF, 0xCE, 0xCD, 0xCC,	/* 50..57 */
	0xDA, 0xD1, 0xD2, 0xD3,  0xD4, 0xD5, 0xD6, 0xD7,	/* 58..5F */

	0xD8, 0xD9, 0xD0, 0xDB,  0x00, 0x00, 0x00, 0x00,	/* 60..67 */
	0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,	/* 68..6F */
	0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,	/* 70..77 */
	0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,	/* 78..7F */
};

static const byte tkusb_kbscan2key_sh1[256]={
	0x00, 0x00, 0x00, 0x00,  0x41, 0x42, 0x43, 0x44,	/* 00..07 */
	0x45, 0x46, 0x47, 0x48,  0x49, 0x4A, 0x4B, 0x4C,	/* 08..0F */
	0x4D, 0x4E, 0x4F, 0x50,  0x51, 0x52, 0x53, 0x54,	/* 10..17 */
	0x55, 0x56, 0x57, 0x58,  0x59, 0x5A, 0x21, 0x40,	/* 18..1F */

	0x23, 0x24, 0x25, 0x5E,  0x26, 0x2A, 0x28, 0x29,	/* 20..27 */
	0x0D, 0x1B, 0x08, 0x09,  0x20, 0x3C, 0x2B, 0x7B,	/* 28..2F */
	0x7D, 0x7C, 0x00, 0x3B,  0x22, 0x7E, 0x3C, 0x3E,	/* 30..37 */
	0x3F, 0x00, 0x87, 0x88,  0x89, 0x8A, 0x8B, 0x8C,	/* 38..3F */

	0x8D, 0x8E, 0x8F, 0x90,  0x91, 0x92, 0x00, 0x00,	/* 40..47 */
	0x99, 0x93, 0x97, 0x96,  0x7F, 0x98, 0x95, 0x83,	/* 48..4F */
	0x82, 0x81, 0x80, 0x00,  0xCF, 0xCE, 0xCD, 0xCC,	/* 50..57 */
	0xDA, 0xD1, 0xD2, 0xD3,  0xD4, 0xD5, 0xD6, 0xD7,	/* 58..5F */

	0xD8, 0xD9, 0xD0, 0xDB,  0x00, 0x00, 0x00, 0x00,	/* 60..67 */
	0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,	/* 68..6F */
	0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,	/* 70..77 */
	0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,	/* 78..7F */
};

u32 tkusb_kbscanmask1[32];
u32 tkusb_kbscanmask2[32];

byte tkusb_kbkeybuf[256];
byte tkusb_tkkeybufs;
byte tkusb_tkkeybufe;

int TKUSB_KbAddKeyEvByte(int key)
{
	tkusb_kbkeybuf[tkusb_tkkeybufe]=key;
	tkusb_tkkeybufe++;
	return(0);
}

int TKUSB_KbAddKeyEvWord(int key)
{
	int j, k;
	
	k=key;

	if(k&0x8000)
	{
		j=k&4095;
		if(j<0x7F)
		{
			TKUSB_KbAddKeyEvByte(j|0x80);
		}else if(j<0xFF)
		{
			TKUSB_KbAddKeyEvByte(0xFF);
			TKUSB_KbAddKeyEvByte(j);
		}
	}else
	{
		j=k&4095;
		if(j<0x7F)
		{
			TKUSB_KbAddKeyEvByte(j);
		}else if(j<0xFF)
		{
			TKUSB_KbAddKeyEvByte(0x7F);
			TKUSB_KbAddKeyEvByte(j);
		}
	}
	return(0);
}

int TKUSB_HandleKbReport(byte *buf)
{
	int shift;
	int i, j, k, v0, v1, v2;
	
//	tk_dbg_printf("TKUSB_HandleKbReport: "
//		"%02X %02X %02X %02X %02X %02X %02X %02X\n",
//		buf[0], buf[1], buf[2], buf[3],
//		buf[4], buf[5], buf[6], buf[7]);
	
	shift=buf[0];
	
	memset(tkusb_kbscanmask1, 0, 32);
	for(i=0; i<6; i++)
	{
		j=buf[2+i];
		if(!j)
			continue;
		if(shift&2)
			j=tkusb_kbscan2key_sh1[j];
		else
			j=tkusb_kbscan2key_sh0[j];
		tkusb_kbscanmask1[j>>5]|=1<<(j&31);
	}

	if(shift&1)
	{
		j=133;
		tkusb_kbscanmask1[j>>5]|=1<<(j&31);
	}
	if(shift&2)
	{
		j=134;
		tkusb_kbscanmask1[j>>5]|=1<<(j&31);
	}
	if(shift&4)
	{
		j=132;
		tkusb_kbscanmask1[j>>5]|=1<<(j&31);
	}
	if(shift&8)
	{
		j=156;
		tkusb_kbscanmask1[j>>5]|=1<<(j&31);
	}

	for(i=0; i<32; i++)
	{
		v0=tkusb_kbscanmask1[i];
		v1=tkusb_kbscanmask2[i];
		if(v0==v1)
			continue;
		v2=v0^v1;
		for(j=0; j<32; j++)
		{
			if((v2>>j)&1)
			{
				k=(i<<5)|j;
				if((v1>>j)&1)
					k|=0x8000;

				TKUSB_KbAddKeyEvWord(k);
//				tkusb_kbkeybuf[tkusb_tkkeybufe]=k;
//				tkusb_tkkeybufe=(tkusb_tkkeybufe+1)&63;
			}
		}
	}
	
	memcpy(tkusb_kbscanmask2, tkusb_kbscanmask1, 32);

	return(0);
}

int TKUSB_HandlePacket(int port, byte *buf, int sz)
{
//	tk_dbg_printf("TKUSB_HandlePacket: A Packet Pid=%X sz=%d "
//		"%02X %02X %02X\n",
//		buf[0], sz, buf[1], buf[2], buf[3]);

	if((buf[0]==0xC3) || (buf[0]==0x4B))
	{
//		tk_dbg_printf("TKUSB_HandlePacket: Packet Pid=%X sz=%d "
//			"%02X %02X %02X\n",
//			buf[0], sz, buf[1], buf[2], buf[3]);
		TKUSB_HandleKbReport(buf+1);
		TKUSB_SendAckToken(port);
	}else
		if(buf[0]==0x5A)
	{
	}else
		if(buf[0]==0x1E)
	{
		TKUSB_SendAckToken(port);
	}else
	{
//		tk_dbg_printf("TKUSB_HandlePacket: B Packet Pid=%X sz=%d "
//			"%02X %02X %02X\n",
//			buf[0], sz, buf[1], buf[2], buf[3]);
	}
	return(0);
}

int TKUSB_DumpDeviceDescriptor(byte *buf)
{
	tk_printf("bLength           =%d\n", buf[0]);
	tk_printf("bDescriptorType   =%d\n", buf[1]);
	tk_printf("bcdUsb            =%02X-%02X\n", buf[2], buf[3]);
	tk_printf("bDeviceClass      =%d\n", buf[4]);
	tk_printf("bDeviceSubClass   =%d\n", buf[5]);
	tk_printf("bDeviceProtocol   =%d\n", buf[6]);
	tk_printf("bMaxPacketSize    =%d\n", buf[7]);
	tk_printf("idVendor          =%02X-%02X\n",  buf[ 8], buf[ 9]);
	tk_printf("idProduct         =%02X-%02X\n", buf[10], buf[11]);
	tk_printf("bcdDevice         =%02X-%02X\n", buf[12], buf[13]);
	tk_printf("iManufacturer     =%d\n", buf[14]);
	tk_printf("iProduct          =%d\n", buf[15]);
	tk_printf("iSerial           =%d\n", buf[16]);
	tk_printf("bNumConfigurations=%d\n", buf[17]);
	return(0);
}

s64 tk_usb_kbpolltime;

byte tk_usb_p1lstate;
byte tk_usb_p2lstate;

byte tk_usb_p1cfgstate;
byte tk_usb_p2cfgstate;
byte tk_usb_isinit;

int TKUSB_DoPoll(void)
{
	static byte msgbuf[2072];
	s64 tt;
	int rt, msgsz, tg;
	
	if(!tk_usb_isinit)
	{
		tg=TKUSB_GetLinkState(0);
		if(tg==3)
			tk_usb_p1lstate=tg;

		tg=TKUSB_GetLinkState(1);
		if(tg==3)
			tk_usb_p2lstate=tg;

		tk_usb_isinit=1;
	}
	
	tt=TK_GetTimeUs();
	if(tt>=tk_usb_kbpolltime)
	{
//		tg=0xE801;
//		tg=0x1000;

//		tg=0xA080;
//		msgbuf[0]=0x69;
//		msgbuf[1]=tg>>0;
//		msgbuf[2]=tg>>8;
//		TKUSB_SendPacket(0, msgbuf, 3);

		if(tk_usb_p1cfgstate==2)
		{
	//		TKUSB_SendInToken(0, 0, 0);
	//		TKUSB_SendInToken(0, 0, 1);
	//		TKUSB_SendInToken(0, 0, 2);
			TKUSB_SendInToken(0, 1, 1);

	//		TKUSB_GetReport(0, 1);
		}
	
//		tk_usb_kbpolltime=tt+10000;
		tk_usb_kbpolltime=tt+20000;
	}

	tg=TKUSB_GetLinkState(0);
	if(tg!=tk_usb_p1lstate)
	{
		tk_dbg_printf("TKUSB_DoPoll: Port 1, Changed Link-State %X\n", tg);
		tk_usb_p1lstate=tg;
		
//		if(tg)
		if(tg && (tg!=3))
		{
			tk_usb_p1cfgstate=1;

			TKUSB_SendReset(0);

			rt=TKUSB_GetDescriptor(0, 0,
				1, 0, msgbuf, 18);
			if(rt>=0)
			{
				TKUSB_DumpDeviceDescriptor(msgbuf+1);

				rt=TKUSB_SetAddress(0, 0, 1);
				TKUSB_PumpDelay(0, 2000);
				if(rt>=0)
				{
					rt=TKUSB_SetConfiguration(0, 1, 1);
					TKUSB_PumpDelay(0, 2000);
					if(rt>=0)
					{
						tk_usb_p1cfgstate=2;
					}else
					{
						tk_dbg_printf("TKUSB_DoPoll: "
							"Failed Set Configuration\n");
					}
				}else
				{
					tk_dbg_printf("TKUSB_DoPoll: Failed Set Address\n");
				}
			}else
			{
				tk_dbg_printf("TKUSB_DoPoll: Failed get descriptor\n");
			}
		}else
		{
			tk_usb_p1cfgstate=0;
		}
	}

	tg=TKUSB_GetLinkState(1);
	if(tg!=tk_usb_p2lstate)
	{
		tk_dbg_printf("TKUSB_DoPoll: Port 2, Changed Link-State %X\n", tg);
		tk_usb_p2lstate=tg;
		
		if(tg)
		{
//			TKUSB_SetAddress(1, 0, 1);
		}
	}

	
	while(1)
	{
		msgsz=0;
		rt=TKUSB_GetPacket(0, msgbuf, &msgsz);
		if(rt>0)
		{
			TKUSB_HandlePacket(0, msgbuf, msgsz);
			continue;
		}

		msgsz=0;
		rt=TKUSB_GetPacket(1, msgbuf, &msgsz);
		if(rt>0)
		{
			TKUSB_HandlePacket(1, msgbuf, msgsz);
			continue;
		}
		
		break;
	}
}

int TKUSB_KbHit(void)
{
	TKUSB_DoPoll();
	return(tkusb_tkkeybufs!=tkusb_tkkeybufe);
}

int TKUSB_KbTryGetch(void)
{
	int c;

	TKUSB_DoPoll();

	c=0;
	if(tkusb_tkkeybufs!=tkusb_tkkeybufe)
	{
		c=tkusb_kbkeybuf[tkusb_tkkeybufs];
		tkusb_tkkeybufs=(tkusb_tkkeybufs+1)&255;
	}
	return(c);
}
