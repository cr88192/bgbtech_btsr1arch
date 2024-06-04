#define SPICTRL_ACS		0x01
#define SPICTRL_CCS		0x04
#define SPICTRL_DCS		0x10
#define SPICTRL_DIVN(x)		((x)<<27)
#define SPICTRL_DIVN2(x)	((x)<<24)
#define SPICTRL_XMIT	0x02
#define SPICTRL_BUSY	0x02
#define SPICTRL_LOOP	0x08
#define SPICTRL_XMIT8X	0x20
#define SPICTRL_XMIT32X	0x40

#define SPICTRL_RESP_RD		0x0100	//Read SPI Response via CMD
#define SPICTRL_QSPI		0x0200
#define SPICTRL_QSPI_RD		0x0300	//Read in 4-bit / QSPI Mode
#define SPICTRL_QSPI_WR		0x0200	//Write in 4-bit / QSPI Mode
#define SPICTRL_DDR			0x0400	//DDR Mode

#define SPI_RES			0xAB
#define SPI_RDID		0x9F
#define SPI_READ		0x03
#define SPI_FAST_READ	0x0B

#define MMC_CMD0	0x00		/* GO_IDLE_STATE */
#define MMC_CMD1	0x01		/* SEND_OP_COND (MMC) */
#define MMC_CMD8	0x08		/* SEND_IF_COND */
#define MMC_CMD9	0x09		/* SEND_CSD */
#define MMC_CMD10	0x0A		/* SEND_CID */
#define MMC_CMD12	0x0C		/* STOP_TRANSMISSION */
#define MMC_CMD16	0x10		/* SET_BLOCKLEN */
#define MMC_CMD17	0x11		/* READ_SINGLE_BLOCK */
#define MMC_CMD18	0x12		/* READ_MULTIPLE_BLOCK */
#define MMC_CMD23	0x17		/* SET_BLOCK_COUNT (MMC) */
#define MMC_CMD24	0x18		/* WRITE_BLOCK */
#define MMC_CMD25	0x19		/* WRITE_MULTIPLE_BLOCK */
#define MMC_CMD32	0x20		/* ERASE_ER_BLK_START */
#define MMC_CMD33	0x21		/* ERASE_ER_BLK_END */
#define MMC_CMD38	0x26		/* ERASE */
#define MMC_CMD52	0x34		/* IO_RW_DIRECT */
#define MMC_CMD55	0x37		/* APP_CMD */
#define MMC_CMD58	0x3A		/* READ_OCR */

#define MMC_ACMD13	0x8D		/* SD_STATUS (SDC) */
#define	MMC_ACMD23	0x97		/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define	MMC_ACMD41	0xA9		/* SEND_OP_COND (SDC) */

u32 tkspi_ctl_speed;
u32 tkspi_ctl_status;

byte tkspi_init_ok;

void TKSPI_ChipSel(int chip)
{
	u32 rv;
	rv=P_SPI_CTRL;
	while(rv&SPICTRL_XMIT)
		{ rv=P_SPI_CTRL; }
	if(chip)
	{
		if(chip==1)
			{ tkspi_ctl_status=tkspi_ctl_speed; }
		else if(chip==2)
			{ tkspi_ctl_status=tkspi_ctl_speed|SPICTRL_CCS|SPICTRL_ACS; }
	}else
	{
		tkspi_ctl_status=SPICTRL_ACS;
	}
	P_SPI_CTRL=tkspi_ctl_status;
}

void TKSPI_SetSpeed(int speed)
{
	if(!speed)
//		{ tkspi_ctl_speed=SPICTRL_DIVN(31); }
		{ tkspi_ctl_speed=SPICTRL_DIVN2(255); }
	else
//		{ tkspi_ctl_speed=SPICTRL_DIVN(0); }
		{ tkspi_ctl_speed=SPICTRL_DIVN2(0); }
}

int TKSPI_XchByte(int c)
{
	u32 v;
	P_SPI_DATA=c;
	P_SPI_CTRL=tkspi_ctl_status|SPICTRL_XMIT;
	v=P_SPI_CTRL;
	while(v&SPICTRL_BUSY) 
		v=P_SPI_CTRL;
	v=P_SPI_DATA;
//	__debugbreak();
//	tk_printf("%02XX%02X ", c, v&255);
	v=v&255;
	return(v);
}

int TKSPI_ReadBasic(int chip, u32 addr,
	byte *bufptr, u32 len)
{
	byte *ptr;

	ptr=bufptr;
	TKSPI_ChipSel(chip);
	TKSPI_XchByte(SPI_READ);
	TKSPI_XchByte(addr>>16);
	TKSPI_XchByte(addr>>8);
	TKSPI_XchByte(addr);
	while(len)
	{
//		*ptr++=TKSPI_XchByte(0);
		*ptr++=TKSPI_XchByte(0xFF);
		len--;
	}
	TKSPI_ChipSel(0);
	return(1);
}

int TKSPI_DelayUSec(int us)
{
}

#ifdef __BJX2__
int TKSPI_ReadDataQA(byte *sbuf, byte *ebuf);

__asm {
TKSPI_ReadDataQA:
	CMPGT	 R4, R5
	BF		.L1

	MOV		-1, R16
	MOV.L	tkspi_ctl_status, R17
	MOV		0xFFFFF000E030, R18		//CTRL
	MOV		0xFFFFF000E038, R19		//QDATA

	OR		0x20, R17

.L0:
//	P_SPI_QDATA=0xFFFFFFFFFFFFFFFFULL;
//	P_SPI_CTRL=tkspi_ctl_status|SPICTRL_XMIT8X;
//	v=P_SPI_CTRL;
//	while(v&SPICTRL_BUSY) 
//		v=P_SPI_CTRL;
//	*(u64 *)ct=P_SPI_QDATA;
//	ct+=8;

	MOV.Q	R16, (R19)
	MOV.L	R17, (R18)

	.L3:
	MOVU.L	(R18), R2
	TEST	2, R2
	BF		.L3
			
	MOV.Q	(R19), R3
	MOV.Q	R3, (R4)

	ADD		8, R4

	CMPGT	 R4, R5
	BT		.L0

.L1:
	RTS
};
#endif

int TKSPI_ReadData(byte *buf, u32 len)
{
	byte *ct, *cte;
	u64 lv;
	u32 count, v;
	int rv;
	int n, ttn;

//	tk_printf("TKSPI_ReadData: Buf=%016llX Len=%X\n",
//		buf, len);

	if(tkspi_ctl_status&(SPICTRL_XMIT|SPICTRL_XMIT8X))
	{
		tk_printf("TKSPI_ReadData: %X %X\n",
			tkspi_ctl_status, tkspi_ctl_speed);

		tkspi_ctl_status&=~(SPICTRL_XMIT|SPICTRL_XMIT8X);	//debug
	}

	n=buf[0];
//	n+=buf[len];
	n+=buf[len-1];

//	tk_printf("TKSPI_ReadData: A\n");

//	count=(1<<16);
	count=(1<<20);
	while(count>0)
	{
//		rv=TKSPI_XchByte(0xFF);

#if 1
		P_SPI_DATA=0xFF;
		P_SPI_CTRL=tkspi_ctl_status|SPICTRL_XMIT;
		v=P_SPI_CTRL;
		ttn=(1<<20);
//		while(v&SPICTRL_BUSY) 
//		while((v&SPICTRL_BUSY) && ((ttn--)>0))
//			v=P_SPI_CTRL;
		while(v&SPICTRL_BUSY) 
		{
			if(ttn<=0)
				break;
			ttn--;
			v=P_SPI_CTRL;
		}

		v=P_SPI_DATA;
		rv=(v&0xFF);
#endif

		if(rv!=0xFF)
		{
			break;
		}
		TKSPI_DelayUSec(10);
		count--;
	}
//	__debugbreak();
	if(rv!=0xFE)
	{
//		__debugbreak();
		printf("TKSPI: Err %02X\n", rv);
		return(-1);
	}

//	tk_printf("TKSPI_ReadData: B\n");

//	printf("<");

	if(tkspi_ctl_status&(SPICTRL_XMIT|SPICTRL_XMIT8X))
	{
		tk_printf("TKSPI_ReadData: B %X %X\n",
			tkspi_ctl_status, tkspi_ctl_speed);

		tkspi_ctl_status&=~(SPICTRL_XMIT|SPICTRL_XMIT8X);	//debug
	}

	ct=buf; n=len;
	cte=ct+n;

//	__debugbreak();

#if 1
	if(!(len&7))
	{
#ifdef __BJX2__
		TKSPI_ReadDataQA(ct, cte);
#else
		while(n>0)
//		while(ct<cte)
		{
			lv=0xFFFFFFFFFFFFFFFFULL;
			P_SPI_QDATA=lv;
			P_SPI_CTRL=tkspi_ctl_status|SPICTRL_XMIT8X;
			v=P_SPI_CTRL;
//			__debugbreak();
			ttn=(1<<20);
//			while(v&SPICTRL_BUSY) 
//			while((v&SPICTRL_BUSY) && ((ttn--)>0))
//				v=P_SPI_CTRL;
			while(v&SPICTRL_BUSY) 
			{
				if(ttn<=0)
					break;
				ttn--;
				v=P_SPI_CTRL;
			}
//			*(u64 *)ct=P_SPI_QDATA;
			lv=P_SPI_QDATA;
			*(u64 *)ct=lv;
//			if(((s64)lv)<0)
//				__debugbreak();
			ct+=8;
			n-=8;
//			__debugbreak();
		}
#endif

//		__debugbreak();

		TKSPI_XchByte(0xFF);
		TKSPI_XchByte(0xFF);

		return(0);
	}
#endif	

	while((n--)>0)
	{
//		rv=TKSPI_XchByte(0xFF);

		P_SPI_DATA=0xFF;
		P_SPI_CTRL=tkspi_ctl_status|SPICTRL_XMIT;
		v=P_SPI_CTRL;
		ttn=1<<20;
//		while(v&SPICTRL_BUSY) 
//			v=P_SPI_CTRL;
		while(v&SPICTRL_BUSY) 
		{
			if(ttn<=0)
				break;
			ttn--;
			v=P_SPI_CTRL;
		}

		v=P_SPI_DATA;
//		rv=(v&0xFF);
		*ct++=v;

//		*ct++=rv;
	}

//	printf(">\n");

	TKSPI_XchByte(0xFF);
	TKSPI_XchByte(0xFF);

	return(0);
}

int TKSPI_WaitReady(void)
{	
	u32 count;
	int rv;
	
//	count=(1<<16);
	count=(1<<20);
	while(count>0)
	{
		rv=TKSPI_XchByte(0xFF);
//		__debugbreak();
		if(rv==0xFF)
		{
			return(1);
		}
		TKSPI_DelayUSec(10);
		count--;
	}
	return(0);
}

u16 TKSPI_DoCrc16Step(u16 crc0, byte value)
{
	u16 crc;
	int i;
	crc=crc0^(((u16)value)<<8);
	for(i=0; i<8; i++)
	{
		if(crc&0x8000)
			{ crc=(crc<<1)^0x1021; }
		else
			{ crc=crc<<1; }
	}
	return(crc);
}

u16 TKSPI_Crc16(byte *data, u32 sz)
{
	byte *cs, *cse;
	u16 crc;

	crc=0xFFFF;
	cs=data; cse=data+sz;
	while (cs<cse)
	{
		crc=TKSPI_DoCrc16Step(crc, *cs++);
	}
	return(crc);
}

int TKSPI_WriteData(byte *buf, u32 len)
{
	byte *ct;
	u32 count;
	u16 crc;
	int rv;
	int n;
	
	if(!TKSPI_WaitReady())
		return(-1);
	
	crc=TKSPI_Crc16(buf, len);
	
#if 0
//	count=(1<<16);
	count=(1<<20);
	while(count>0)
	{
		rv=TKSPI_XchByte(0xFF);
		if(rv!=0xFF)
			break;
		TKSPI_DelayUSec(10);
		count--;
	}
	if(rv!=0xFE)
	{
		printf("Err %02X\n", rv);
		return(-1);
	}
#endif

//	printf("<");

	rv=TKSPI_XchByte(0xFE);

	ct=buf; n=len;
	while((n--)>0)
	{
		rv=TKSPI_XchByte(*ct++);
//		*ct++=rv;
	}

//	printf(">\n");

	TKSPI_XchByte((crc>>8)&0xFF);
	TKSPI_XchByte((crc>>0)&0xFF);

//	TKSPI_XchByte(0xFF);
//	TKSPI_XchByte(0xFF);

	n=4096;
	while(n--)
	{
		if(rv!=0xFF)
			break;
		rv=TKSPI_XchByte(0xFF);
	}
	
	if((rv&0x1F)==0x05)
	{
		n=65536;
		rv=TKSPI_XchByte(0xFF);
//		while(rv==0x00)
		while((rv==0x00) && (n--))
			{ rv=TKSPI_XchByte(0xFF); }
	}

	TKSPI_XchByte(0xFF);
	TKSPI_XchByte(0xFF);

	return(0);
}


void TKSPI_Deselect(void)
{
	TKSPI_ChipSel(1);
}

int TKSPI_Select(void)
{
	TKSPI_ChipSel(0);
	if (TKSPI_WaitReady())
	{
//		puts("TKSPI_Select: 1\n");
		return(1);
	}
	TKSPI_Deselect();
//	puts("TKSPI_Select: 0\n");
	return(0);
}

void TKSPI_PowerOff(void)
{
	TKSPI_Select();
	TKSPI_Deselect();
}

int TKSPI_SendCmd(int cmd, u32 arg)
{
	int n, h, res;

//	if(arg>>32)
//		__debugbreak();
		
	if((cmd==MMC_CMD55) && (arg!=0))
		__debugbreak();

	if(cmd&0x80)
	{
		cmd&=0x7F;
		res=TKSPI_SendCmd(MMC_CMD55, 0);
		if(res>1)
			return(res);
	}

	TKSPI_Deselect();
	if(!TKSPI_Select())
	{
		printf("TKSPI_SendCmd: Select Fail\n");
		return(0xFF);
	}

//	TKSPI_XchByte(0x40|cmd);
	TKSPI_XchByte(0x40|(cmd&0x3F));
	TKSPI_XchByte((byte)(arg>>24));
	TKSPI_XchByte((byte)(arg>>16));
	TKSPI_XchByte((byte)(arg>>8));
	TKSPI_XchByte((byte)arg);
	h=0x01;
	if(cmd==MMC_CMD0)h=0x95;
	if(cmd==MMC_CMD8)h=0x87;
	TKSPI_XchByte(h);

	if (cmd==MMC_CMD12)
		{ TKSPI_XchByte(0xFF); }
//	n=10;
//	n=100;
	n=1000;
	while(n>0)
	{
		res=TKSPI_XchByte(0xFF);
		if(!(res&0x80))
			break;
		n--;
	}

	if(res==0xFF)
		__debugbreak();

//	printf("TKSPI_SendCmd: Result=%02X\n", res);
	return(res);
}

int tkspi_recchk=0;

int TKSPI_ReadSectors(byte *buf, s64 lba, int cnt)
{
	byte *ct;
	u64 la;
	int n, h, xt;

//	if(((s32)lba)!=lba)
	if(lba>>32)
	{
		tk_puts("TKSPI_ReadSectors: Bad LBA\n");
		__debugbreak();
	}
	
	if(cnt!=(cnt&255))
	{
		tk_printf("TKSPI_ReadSectors: Bad Count %d\n", cnt);
		__debugbreak();
	}
	
	if(tkspi_recchk!=0)
	{
		tk_dbg_printf("TKSPI_ReadSectors: Reentrance Detected\n");
	}
	tkspi_recchk++;
	

//	tk_printf("TKSPI_ReadSectors: %016llX %d %d\n", buf, lba, cnt);

#if 0
	if(cnt>1)
	{
		ct=buf; la=lba; n=cnt;
		if(h)TKSPI_SendCmd(MMC_CMD55, h);
		TKSPI_SendCmd(MMC_CMD18, la);
		while(n>0)
		{
			h=la>>32;
			TKSPI_ReadData(ct, 512);
			ct+=512; la++; n--;
		}
		TKSPI_SendCmd(MMC_CMD12, 0);
	}else
	{
		ct=buf; la=lba; n=cnt;
		h=la>>32;
		if(h)TKSPI_SendCmd(MMC_CMD55, h);
		TKSPI_SendCmd(MMC_CMD17, la);
		TKSPI_ReadData(ct, 512);
	}
#endif

#if 1
//	xt=buf[0]+buf[(cnt<<9)-1];	//Make sure it is paged in.
	xt=0;

	ct=buf; la=lba; n=cnt;
	while(n>0)
	{
//		tk_printf("TKSPI_ReadSectors: B %016llX %d %d\n", ct, la, n);

		xt+=ct[0]+ct[511];	//Make sure it is paged in.

		h=la>>32;
		if(h)TKSPI_SendCmd(MMC_CMD55, h);
		TKSPI_SendCmd(MMC_CMD17, la);
		TKSPI_ReadData(ct, 512);
		ct+=512; la++; n--;
	}
#endif

	tkspi_recchk--;
	return(0);
}

int TKSPI_WriteSectors(byte *buf, s64 lba, int cnt)
{
	byte *ct;
	u64 la;
	int n, h, xt;

//	if(((s32)lba)!=lba)
	if(lba>>32)
		__debugbreak();
	
	if(cnt!=(cnt&255))
		__debugbreak();

	if(tkspi_recchk!=0)
	{
		tk_dbg_printf("TKSPI_WriteSectors: Reentrance Detected\n");
	}
	tkspi_recchk++;

	xt=buf[0]+buf[(cnt<<9)-1];	//Make sure it is paged in.

	ct=buf; la=lba; n=cnt;
	while(n>0)
	{
		xt+=ct[0]+ct[511];	//Make sure it is paged in.

		h=la>>32;
		if(h)TKSPI_SendCmd(MMC_CMD55, h);
		TKSPI_SendCmd(MMC_CMD24, la);
		TKSPI_WriteData(ct, 512);
		ct+=512; la++; n--;
	}

	tkspi_recchk--;

	return(0);
}

int TKSPI_bdev_ReadSectors(TK_BLKDEV_CTX *ctx, byte *buf, s64 lba, int cnt)
{
	return(TKSPI_ReadSectors(buf, lba, cnt));
}

int TKSPI_bdev_WriteSectors(TK_BLKDEV_CTX *ctx, byte *buf, s64 lba, int cnt)
{
	return(TKSPI_WriteSectors(buf, lba, cnt));
}

TK_BLKDEV_VT tkspi_blkdev_vt = {
"sdspi",
NULL,
NULL,
NULL,
TKSPI_bdev_ReadSectors,
TKSPI_bdev_WriteSectors
};

static int tkspi_is_init=0;
static int tkspi_id_bdev=0;

int TKSPI_InitDevice(void)
{
//	static const char *hexchars="0123456789ABCDEF";
	byte ocr[4];
	TK_BLKDEV_CTX *bdctx;
	byte s, cmd, ty;
	u32 n, count;
	
	if(tkspi_is_init)
		return(0);
	tkspi_is_init=1;

	TKSPI_SetSpeed(0);
	TKSPI_Deselect();

	for (n=0; n<16384; n++)
//	for (n=0; n<1024; n++)
//	for (n=0; n<256; n++)
		TKSPI_XchByte(0xFF);

	TKSPI_Select();
	n=TKSPI_SendCmd(MMC_CMD0, 0);

	TKSPI_Deselect();
	for (n=0; n<16; n++)
		TKSPI_XchByte(0xFF);

	TKSPI_SetSpeed(0);
	TKSPI_SendCmd(MMC_CMD52, 1);
	for (n=0; n<10; n++)
		TKSPI_XchByte(0xFF);

	ty=0;
	n=TKSPI_SendCmd(MMC_CMD0, 0);
	if(n==1)
	{
		s=TKSPI_SendCmd(MMC_CMD8, 0x1AA);
		if(s==1)
		{
			for(n=0; n<4; n++)
				ocr[n]=TKSPI_XchByte(0xFF);
			if((ocr[2]==0x01) && (ocr[3]==0xAA))
			{
				n=1<<17;
				while(n>0)
				{
					s=TKSPI_SendCmd(MMC_ACMD41, 1UL<<30);
					if(!s)
						break;
					TKSPI_DelayUSec(10);
					n--;
				}
				s=TKSPI_SendCmd(MMC_CMD58, 0);
				if(n && !s)
				{
					for (n=0; n<4; n++) 
						ocr[n]=TKSPI_XchByte(0xff);
//					ty=(ocr[0]&0x40)?CT_SD2|CT_BLOCK:CT_SD2;
					ty=(ocr[0]&0x40)?0x43:0x03;
				}
			}
		}
		else
		{
			s=TKSPI_SendCmd(MMC_ACMD41, 0);
			if(s<=1)
			{
				ty=2;
				cmd=MMC_ACMD41;
			} else {
				ty=1;
				cmd=MMC_CMD1;
			}
			n=1<<17;
			while(n)
			{
				s=TKSPI_SendCmd(cmd, 0);
				if(!s)
					break;
				TKSPI_DelayUSec(10);
				n--;
			}
			s=TKSPI_SendCmd(MMC_CMD16, 512);
			if (!n || s)
				ty=0;
		}
	}else
	{
		ty=0;
		printf("TKSPI_InitDevice: Bad CMD0 %02X\n", n);
	}
	TKSPI_Deselect();

	if(ty)
	{
		TKSPI_SetSpeed(1);
		tkspi_init_ok=ty;
		printf("TKSPI_InitDevice: Init OK, %d\n", ty);
	}
	else
	{
		TKSPI_PowerOff();
		tkspi_init_ok=0;
		printf("TKSPI_InitDevice: Init Failed\n");
	}
	
	bdctx=TKBDEV_AllocNewDevice();
	tkspi_id_bdev=bdctx->bdev;
	bdctx->vt=&tkspi_blkdev_vt;
	
	return(tkspi_init_ok);
}
