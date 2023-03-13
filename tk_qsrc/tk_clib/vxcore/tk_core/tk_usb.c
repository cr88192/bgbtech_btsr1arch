u16 tk_usb_rxbuf1[1024];
u16 tk_usb_rxbuf2[1024];
u16 tk_usb_txbuf1[1024];
u16 tk_usb_txbuf2[1024];

int TKUSB_SendPacket(int port, byte *buf, int sz)
{
	volatile u64 *txba;
	volatile u64 *txra;
	u16 *txbuf;
	int i, n;

//	tk_printf("TKUSB_SendPacket: %d\n", sz);

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

//	txra[0]=0;
//	txra[1]=sz+1;
}


int TKUSB_GetPacket(int port, byte *buf, int *rsz)
{
	volatile u64 *txba;
	volatile u64 *txra;
	u16 *txbuf;
	u64 txpos, txs, txe, ks, ke;
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

u32 tkusb_kbscanmask1[8];
u32 tkusb_kbscanmask2[8];

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
	
//	tk_printf("TKUSB_HandleKbReport: "
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
	if((buf[0]==0xC3) || (buf[0]==0x4B))
	{
//		tk_printf("TKUSB_HandlePacket: Packet Pid=%X sz=%d "
//			"%02X %02X %02X\n",
//			buf[0], sz, buf[1], buf[2], buf[3]);
		TKUSB_HandleKbReport(buf+1);
	}else
	{
		tk_printf("TKUSB_HandlePacket: Packet Pid=%X sz=%d "
			"%02X %02X %02X\n",
			buf[0], sz, buf[1], buf[2], buf[3]);
	}
}

s64 tk_usb_kbpolltime;

int TKUSB_DoPoll(void)
{
	static byte msgbuf[2072];
	s64 tt;
	int rt, msgsz, tg;
	
	tt=TK_GetTimeUs();
	if(tt>=tk_usb_kbpolltime)
	{
//		tg=0xE801;
		tg=0x1000;
		msgbuf[0]=0x69;
		msgbuf[1]=tg>>0;
		msgbuf[2]=tg>>8;
		TKUSB_SendPacket(0, msgbuf, 3);
		
//		tk_usb_kbpolltime=tt+10000;
		tk_usb_kbpolltime=tt+20000;
	}

	
	while(1)
	{
		rt=TKUSB_GetPacket(0, msgbuf, &msgsz);
		if(rt>0)
		{
			TKUSB_HandlePacket(0, msgbuf, msgsz);
			continue;
		}

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
