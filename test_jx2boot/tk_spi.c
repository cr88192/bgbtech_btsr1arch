#define SPICTRL_ACS		0x01
#define SPICTRL_CCS		0x04
#define SPICTRL_DCS		0x10
#define SPICTRL_DIVN(x) ((x)<<27)
#define SPICTRL_XMIT	0x02
#define SPICTRL_BUSY	0x02
#define SPICTRL_LOOP	0x08

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
		{ tkspi_ctl_speed=SPICTRL_DIVN(31); }
	else
		{ tkspi_ctl_speed=SPICTRL_DIVN(0); }
}

byte TKSPI_XchByte(byte c)
{
	u32 v;
	P_SPI_DATA=c;
	P_SPI_CTRL=tkspi_ctl_status|SPICTRL_XMIT;
	v=P_SPI_CTRL;
	while(v&SPICTRL_BUSY) 
		v=P_SPI_CTRL;
	v=P_SPI_DATA;
//	__debugbreak();
//	printf("%02XX%02X ", c, v&255);
	return(v&0xFF);
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
		*ptr++=TKSPI_XchByte(0);
		len--;
	}
	TKSPI_ChipSel(0);
	return(1);
}

int TKSPI_DelayUSec(int us)
{
}

int TKSPI_ReadData(byte *buf, u32 len)
{
	byte *ct;
	u32 count;
	u32 v;
	byte rv;
	int n;

//	printf("TKSPI: ReadData: buf=%p len=%d\n", buf, len);
	
	count=(1<<16);
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
		printf("TKSPI: Err %02X\n", rv);
		return(-1);
	}

//	printf("TKSPI: ReadData(2): buf=%p len=%d\n", buf, len);

//	printf("<");

	ct=buf; n=len;

//	__debugbreak();

//	printf("TKSPI: ReadData(3): buf=%p ct=%p len=%d\n", buf, ct, len);
	
	if(!ct)
	{
		__debugbreak();
	}
	
	while((n--)>0)
	{
		rv=TKSPI_XchByte(0xFF);
//		printf("%02X ", rv);
//		printf("*%p=%02X ", ct, rv);
		*ct++=rv;

#if 0
		P_SPI_DATA=0xFF;
		P_SPI_CTRL=tkspi_ctl_status|SPICTRL_XMIT;
		v=P_SPI_CTRL;
		while(v&SPICTRL_BUSY) 
			v=P_SPI_CTRL;
		*ct++=P_SPI_DATA;
#endif
	}

//	printf(">\n");

	TKSPI_XchByte(0xFF);
	TKSPI_XchByte(0xFF);

	return(0);
}

int TKSPI_WaitReady(void)
{	
	u32 count;
	byte rv;
	
	count=(1<<16);
	while(count>0)
	{
		rv=TKSPI_XchByte(0xFF);
//		__debugbreak();
		if(rv==0xFF) {
		       	return(1);
		}
		TKSPI_DelayUSec(10);
		count--;
	}
	return(0);
}

int TKSPI_WriteData(byte *buf, u32 len)
{
	byte *ct;
	u32 count;
	byte rv;
	int n;
	
	if(!TKSPI_WaitReady())
		return(-1);
	
#if 0
	count=(1<<16);
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

byte TKSPI_SendCmd(byte cmd, u32 arg)
{
	byte n, h, res;

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

	TKSPI_XchByte(0x40|cmd);
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
	n=1000;
	while(n>0)
	{
		res=TKSPI_XchByte(0xFF);
		if(!(res&0x80))
			break;
		n--;
	}
//	printf("TKSPI_SendCmd: Result=%02X\n", res);
	return(res);
}

int TKSPI_ReadSectors(byte *buf, s64 lba, int cnt)
{
	byte *ct;
	u64 la;
	int n, h;

//	printf("TKSPI_ReadSectors: %08X %02X\n", (u32)lba, cnt);

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
	ct=buf; la=lba; n=cnt;
	while(n>0)
	{
		h=la>>32;
		if(h)TKSPI_SendCmd(MMC_CMD55, h);
		TKSPI_SendCmd(MMC_CMD17, la);
		TKSPI_ReadData(ct, 512);
		ct+=512; la++; n--;
	}
#endif
	return(0);
}

int TKSPI_WriteSectors(byte *buf, s64 lba, int cnt)
{
	byte *ct;
	u64 la;
	int n, h;

	ct=buf; la=lba; n=cnt;
	while(n>0)
	{
		h=la>>32;
		if(h)TKSPI_SendCmd(MMC_CMD55, h);
		TKSPI_SendCmd(MMC_CMD24, la);
		TKSPI_WriteData(ct, 512);
		ct+=512; la++; n--;
	}

	return(0);
}


static int tkspi_init=0;

int TKSPI_InitDevice(void)
{
	static const char *hexchars="0123456789ABCDEF";
	byte ocr[4];
	byte s, cmd, ty;
	u32 n, count;
	
	if(tkspi_init)
		return(tkspi_init_ok);
	tkspi_init=1;

	printf("TKSPI_InitDevice: Init 1\n");

	TKSPI_SetSpeed(0);
	TKSPI_SendCmd(MMC_CMD52, 1);
	for (n=0; n<10; n++)
		TKSPI_XchByte(0xFF);

	printf("TKSPI_InitDevice: Init 2\n");

	ty=0;
	n=TKSPI_SendCmd(MMC_CMD0, 0);
	if(n==1)
	{
		printf("TKSPI_InitDevice: Init 3A\n");

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
			printf("TKSPI_InitDevice: Init 3B\n");

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
	return(tkspi_init_ok);
}
