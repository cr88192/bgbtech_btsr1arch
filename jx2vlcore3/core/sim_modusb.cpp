#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "VModUsbBuf.h"
#include "verilated.h"


VModUsbBuf *top = new VModUsbBuf;

vluint64_t main_time = 0;

byte usb_kbmsg[16];
int usb_kbmsgsz;

short usb_txbuf[256];
byte usb_txposs;
byte usb_txpose;

short usb_rxbuf[256];
byte usb_rxposs;
byte usb_rxpose;

// byte usb_kbmsg[32];
// byte usb_kbmsgsz;

int usb_hidkbreport()
{
	int i, j, k;
	
	for(i=0; i<usb_kbmsgsz; i++)
	{
		usb_txbuf[usb_txpose]=usb_kbmsg[i];
		usb_txpose=(usb_txpose+1)&255;
	}

	usb_txbuf[usb_txpose]=0x100;
	usb_txpose=(usb_txpose+1)&255;
} 

int usb_parserxeop()
{
	if(usb_rxbuf[usb_rxposs]==0x69)
	{
		if(usb_rxbuf[(usb_rxposs+1)&0xFF]==0x01)
		{
			usb_hidkbreport();
		}
	}
	
	usb_rxposs=usb_rxpose;
}

int update_usb()
{
	static short xmit;
	static byte xmitpos, xmitmd;
	static byte xmitclk_val;
	static int xmitclk_cnt;
	static byte xmit_upd, txwin;
	static byte tclk, tdat, lclk, tlclk, tdatl;
	static byte b, pb;
	static byte rxdat, rxldat, rxbit, rxldat2;
	static byte rxclk, rxmd, rxby;
	static short rxwin1, rxwin2;

	if(top->clock_100==tlclk)
		return(0);
	
	if(!top->clock_100)
	{
		tlclk=top->clock_100;
		top->usb_clkdat_i=tdat;
		return(0);
	}

	rxdat=top->usb_clkdat_i;
	if(rxdat!=rxldat)
	{
		printf("update_usb: RX State %d\n", rxdat);
		rxldat=rxdat;
		rxclk=0;
	}
	
	if(rxclk==64)
	{
		rxbit=(rxdat!=rxldat2);
		rxldat2=rxdat;
		
		rxwin1=(rxwin1>>1)|(rxbit<<15);
		rxwin2=(rxwin2>>1)|(rxbit<<15);
		if((rxwin1&0xFE00)==0x8000)
		{
			/* Stuffed Bit */
			rxwin2<<=1;
		}
		
		if(!rxmd)
		{
			if((rxwin2&0xFF80)==0x7F800)
			{
				rxmd=8;
			}
		}else
		{
			rxmd++;
			if(rxmd==16)
			{
				rxby=rxwin2>>8;
				printf("update_usb: rxby=%02X\n", rxby);
				usb_rxbuf[usb_rxpose]=rxby;
				usb_rxpose=(usb_rxpose+1)&255;
			}
			
			if(rxdat==0)
			{
				printf("update_usb: EOP\n");
				usb_rxbuf[usb_rxpose]=0x100;
				usb_rxpose=(usb_rxpose+1)&255;
				rxmd=0;
				usb_parserxeop();
			}
		}
	}
	rxclk+=4;

	if(xmitpos<=0)
	{
		xmitclk_cnt--;
		if(xmitclk_cnt<=0)
		{
			if(!xmitclk_val)
			{
				tclk=1;
				tdat=1;

				xmitclk_val=1;
				xmitclk_cnt=33;
			}else
			{
				if(usb_txposs!=usb_txpose)
				{
					xmit=usb_txbuf[usb_txposs];
					usb_txposs=(usb_txposs+1)&255;
					xmitpos=8;
					
					printf("update_usb: scan=%02X\n", xmit);
				}else if(xmitmd&0x30)
				{
					xmit=0x100;
					xmitpos=8;
				}

				tclk=1;
				tdat=1;

				xmitclk_val=1;
				xmitclk_cnt=33;
				xmit_upd=1;
			}
		}

		if(xmitmd==0)
		{
			tdat=2;		/* Low Speed */
		}
	}else
	{
		if(xmit_upd)
		{
			switch(xmitpos)
			{
				case  8: b=(xmit>>0)&1; break;
				case  7: b=(xmit>>1)&1; break;
				case  6: b=(xmit>>2)&1; break;
				case  5: b=(xmit>>3)&1; break;
				case  4: b=(xmit>>4)&1; break;
				case  3: b=(xmit>>5)&1; break;
				case  2: b=(xmit>>6)&1; break;
				case  1: b=(xmit>>7)&1; break;
			}
			tdat=b;
			xmit_upd=0;
		}

		xmitclk_cnt--;
		if(xmitclk_cnt<=0)
		{
			xmitclk_val=!xmitclk_val;
			xmitclk_cnt=67;
			tclk=xmitclk_val;
			xmit_upd=1;
			
			if(!(xmitmd&0x30))
			{
				if(xmitmd<8)
				{
					b=1;
					if(!xmitclk_val)
						xmitmd++;
				}else
				{
					b=0;
					if(!xmitclk_val)
						xmitmd=0x10;
				}
			}
			else if((txwin&0xFC)==0x00)
			{
				/* bit stuff */
				b=1;
			}else
				if(!xmitclk_val)
			{
				xmitpos--;
				xmit_upd=0;
			}else
			{
				tdat=b;
			}
			
			if(b)
			{
				if(tdatl==1)
					tdat=2;
				if(tdatl==2)
					tdat=1;
				tdatl=tdat;
			}else
			{
				tdat=tdatl;
			}
			
			if(xmit&0x100)
			{
				tdat=0;
				if(xmitpos<=0)
					xmitmd=0;
			}
			
			if(!xmitclk_val)
				txwin=(txwin>>1)|(b<<8);
		}
	}

	tlclk=top->clock_100;
	top->usb_clkdat_i=tdat;
	
	if(tclk!=lclk)
	{
		lclk=tclk;
	}
}

uint64_t mod_GetQWord(int addr)
{
	uint64_t val;

	while(top->mmioOK!=0)
	{
		update_usb();
		top->clock = main_time&1;
		top->mmioOpm=0x00;
		top->eval();
		main_time++;
	}

	top->mmioOpm=0x0B;
	top->mmioAddr=addr;

	while(top->mmioOK==0)
	{
		update_usb();
		top->clock = main_time&1;
		top->eval();
		main_time++;
	}

	val=top->mmioOutData;

	while(top->mmioOK!=0)
	{
		update_usb();
		top->clock = main_time&1;
		top->mmioOpm=0x00;
		top->eval();
		main_time++;
	}

	return(val);
}

uint64_t mod_SetQWord(int addr, uint64_t val)
{
	while(top->mmioOK!=0)
	{
		update_usb();
		top->clock = main_time&1;
		top->mmioOpm=0x00;
		top->eval();
		main_time++;
	}

	top->mmioOpm=0x13;
	top->mmioAddr=addr;
	top->mmioInData=val;

	while(top->mmioOK==0)
	{
		update_usb();
		top->clock = main_time&1;
		top->eval();
		main_time++;
	}

	while(top->mmioOK!=0)
	{
		update_usb();
		top->clock = main_time&1;
		top->mmioOpm=0x00;
		top->eval();
		main_time++;
	}

	return(top->mmioOutData)
}

int main(int argc, char **argv, char **env)
{
	FILE *ofd;

	byte *ibuf, *dbuf;
	char *ifn, *ofn;
	int i, j, k;
	
	Verilated::commandArgs(argc, argv);

	usb_kbmsg[ 0]=0xC3;
	usb_kbmsg[ 1]=0x12;
	usb_kbmsg[ 2]=0x34;
	usb_kbmsg[ 3]=0x56;
	usb_kbmsg[ 4]=0x78;
	usb_kbmsg[ 5]=0x9A;
	usb_kbmsg[ 6]=0xBC;
	usb_kbmsg[ 7]=0xDE;
	usb_kbmsg[ 8]=0xF0;
	usb_kbmsg[ 9]=0x00;
	usb_kbmsg[10]=0x00;
	usb_kbmsgsz=11;

	ifn=NULL;
	ofn=NULL;

	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			continue;
		}
		if(!ifn)
			{ ifn=argv[i]; continue; }
		if(!ofn)
			{ ofn=argv[i]; continue; }
	}
	
	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
