#define MMIO_TIMER_LO	0xE000
#define MMIO_TIMER_HI	0xE000

#define MMIO_DEBUG_RX	0xE010
#define MMIO_DEBUG_TX	0xE014
#define MMIO_DEBUG_STS	0xE018
#define MMIO_DEBUG_CTL	0xE01C

#define MMIO_GPIO_IN	0xE100
#define MMIO_GPIO_OUT	0xE104
#define MMIO_GPIO_DIR	0xE108

#define P_MMIO_DEBUG_RX		(*(int *)MMIO_DEBUG_RX)
#define P_MMIO_DEBUG_TX		(*(int *)MMIO_DEBUG_TX)
#define P_MMIO_DEBUG_STS	(*(int *)MMIO_DEBUG_STS)
#define P_MMIO_DEBUG_CTL	(*(int *)MMIO_DEBUG_CTL)

#define P_MMIO_GPIO_IN		(*(int *)MMIO_GPIO_IN)
#define P_MMIO_GPIO_OUT		(*(int *)MMIO_GPIO_OUT)
#define P_MMIO_GPIO_DIR		(*(int *)MMIO_GPIO_DIR)

#define P1IN				(((unsigned char *)MMIO_GPIO_IN)[0])
#define P2IN				(((unsigned char *)MMIO_GPIO_IN)[1])

#define P1OUT				(((unsigned char *)MMIO_GPIO_OUT)[0])
#define P2OUT				(((unsigned char *)MMIO_GPIO_OUT)[1])

#define P1DIR				(((unsigned char *)MMIO_GPIO_DIR)[0])
#define P2DIR				(((unsigned char *)MMIO_GPIO_DIR)[1])

#ifndef NULL
#define NULL	((void *)0)
#endif


#define p1out	P1OUT
#define p2out	P2OUT


typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;

typedef volatile u32 vol_u32;


void __halt(void);


void putc(int val)
{
	while(P_MMIO_DEBUG_STS&8);
	P_MMIO_DEBUG_TX=val;
}


int kbhit(void)
	{ return(P_MMIO_DEBUG_STS&1); }

int getch(void)
{
	while(!(P_MMIO_DEBUG_STS&1))
	{
		__halt();
//		sleep_0();
	}
	return(P_MMIO_DEBUG_RX);
}

void puts(char *msg)
{
	char *s;
	
	s=msg;
	while(*s)
		{ putc(*s++); }
}

void gets(char *buf)
{
	char *t;
	int i;
	
	t=buf;
	while(1)
	{
		i=getch();
		if((i=='\b') || (i==127))
		{
			if(t>buf)
			{
				puts("\b \b");
				t--;
			}
			*t=0;
			continue;
		}
		if(i=='\r')
			{ putc('\n'); break; }
		if(i=='\n')
			{ putc('\n'); break; }
		putc(i);
		*t++=i;
	}
	*t=0;
}


char *async_gets(char *buf)
{
	char *t;
	int i, eol;
	
	t=buf; eol=0;
	while(*t)
		t++;

	while(1)
	{
		if(!kbhit())
			break;

		i=getch();
		if((i=='\b') || (i==127))
		{
			if(t>buf)
			{
				puts("\b \b");
				t--;
			}
			*t=0;
			continue;
		}
		if(i=='\r')
			{ putc('\n'); eol=1; break; }
		if(i=='\n')
			{ putc('\n'); eol=1; break; }
		putc(i);
		*t++=i;
	}
	*t=0;
	if(eol)
		return(buf);
	return(NULL);
}

void print_hex(u32 v)
{
	static char *chrs="0123456789ABCDEF";

//	char *chrs;
//	chrs="0123456789ABCDEF";

	putc(chrs[(v>>28)&15]);
	putc(chrs[(v>>24)&15]);
	putc(chrs[(v>>20)&15]);
	putc(chrs[(v>>16)&15]);
	putc(chrs[(v>>12)&15]);
	putc(chrs[(v>> 8)&15]);
	putc(chrs[(v>> 4)&15]);
	putc(chrs[(v    )&15]);
}

void print_hex_n(u32 v, int n)
{
	static char *chrs="0123456789ABCDEF";

//	char *chrs;
//	chrs="0123456789ABCDEF";

	if(n>7)putc(chrs[(v>>28)&15]);
	if(n>6)putc(chrs[(v>>24)&15]);
	if(n>5)putc(chrs[(v>>20)&15]);
	if(n>4)putc(chrs[(v>>16)&15]);
	if(n>3)putc(chrs[(v>>12)&15]);
	if(n>2)putc(chrs[(v>> 8)&15]);
	if(n>1)putc(chrs[(v>> 4)&15]);
	if(n>0)putc(chrs[(v    )&15]);
}

int print_hex_genw(u32 v)
{
	u32 w;
	int i;

	i=1;
	while(v>=16)
//	while(v>>4)
		{ v=v>>4; i++; }

#if 0
	w=v; i=1;
	if(w>=16) { w=w>>4; i++; }
	if(w>=16) { w=w>>4; i++; }
	if(w>=16) { w=w>>4; i++; }
	if(w>=16) { w=w>>4; i++; }
	if(w>=16) { w=w>>4; i++; }
	if(w>=16) { w=w>>4; i++; }
	if(w>=16) { w=w>>4; i++; }
#endif

	return(i);
}

void print_decimal(int val)
{
	char tb[256];
	char *t;
	int i, k, s;
	
	k=val; s=0;
	if(k<0)
		{ k=-k; s=1; }
	
	t=tb;
	if(!k)*t++='0';	
	while(k>0)
	{
		i=k%10;
		*t++='0'+i;
		k=k/10;
	}
	if(s)*t++='-';
	
	while(t>tb)
		{ t--; putc(*t); }
}

void print_decimal_n(int val, int num)
{
	char tb[256];
	char *t;
	int i, k, n, s;
	
	k=val; s=0;
	if(k<0)
		{ k=-k; s=1; }
	
	t=tb; n=num;
//	if(!k)*t++=0;	
	while(n>0)
	{
		i=k%10;
		*t++='0'+i;
		k=k/10;
		n--;
	}

//	if(s)*t++='-';
	
	while(t>tb)
		{ t--; putc(*t); }
}



int __udivsi3_asm(int x, int y);

int __udivsi3(int x, int y)
	{ return(__udivsi3_asm(x, y)); }

int __sdivsi3(int x, int y)
{
	int sg, v, x1, y1;
	if(x<0)		{ x1=-x; sg=1; }
	else		{ x1=x; sg=0; }
	if(y<0)		{ y1=-y; sg^=1; }
	else		{ y1=y; }
	v=__udivsi3_asm(x1, y1);
	if(sg)v=-v;
	return(v);
}

void __debugbreak(void);

byte p1in, lp1in;
byte p2in, lp2in;

byte spi_in;
byte spi_out;
byte spi_pos;

typedef struct {
	s32 pos;
	s32 tgt;
	s32 tick;
	u32 div;
}motor_state;

motor_state motor1;
motor_state motor2;
motor_state motor3;
motor_state motor4;

#if 0
s32 motor1_pos;
s32 motor1_tgt;
s32 motor1_tick;
u32 motor1_div;

s32 motor2_pos;
s32 motor2_tgt;
s32 motor2_tick;
u32 motor2_div;

s32 motor3_pos;
s32 motor3_tgt;
s32 motor3_tick;
u32 motor3_div;

#ifdef HAS_MOTOR4
s32 motor4_pos;
s32 motor4_tgt;
s32 motor4_tick;
u32 motor4_div;
#endif
#endif

const u32 motor1_rst = 0x10000000;
const u32 motor2_rst = 0x10000000;
const u32 motor3_rst = 0x10000000;
const u32 motor4_rst = 0x10000000;

byte spi_state;		/* command state */
byte spi_mtrid;		/* command motor ID */
byte spi_divmsb;
u16 spi_target_hi;
u16 spi_target_lo;
u32 spi_target_temp;

void spi_newbyte(byte val)
{
	spi_out=0x80;
	if(spi_state==0)
	{
		if(val==0x80)
		{
			spi_state=1;
		}else
		{
			spi_pos=7;
		}
		return;
	}else if(spi_state==1)
	{
		if(val<0x10)
		{
			if(val==0x00)
				{ spi_state=0; }
			return;
		}else if(val<0x20)
		{
			spi_mtrid=val&15;
			spi_state=4;
			return;
		}else if(val<0x30)
		{
			spi_state=8;
			switch(val&15)
			{
			case 0x0: spi_target_temp=motor1.tgt; break;
			case 0x1: spi_target_temp=motor2.tgt; break;
			case 0x2: spi_target_temp=motor3.tgt; break;

			case 0x8: spi_target_temp=motor1.pos; break;
			case 0x9: spi_target_temp=motor2.pos; break;
			case 0xA: spi_target_temp=motor3.pos; break;
#ifdef HAS_MOTOR4
			case 0x3: spi_target_temp=motor4.tgt; break;
			case 0xB: spi_target_temp=motor4.pos; break;
#endif
			}
			return;
		}
		return;
	}

	if(spi_state<8)
	{
		if(spi_state==4) /* Set Target, Byte 0 MSB */
			{ spi_target_temp=val;						spi_state=5; }
		else if(spi_state==5) /* Set Target, Byte 1 */
			{ spi_target_temp=(spi_target_temp<<8)|val;	spi_state=6; }
		else if(spi_state==6) /* Set Target, Byte 2 */
			{ spi_target_temp=(spi_target_temp<<8)|val;	spi_state=7; }
		else
		{
			spi_target_temp=(spi_target_temp<<8)|val;
//			spi_target_temp=0x55AA55AAL;
			switch(spi_mtrid)
			{
			case 0x00: motor1.tgt=spi_target_temp; break;
			case 0x01: motor2.tgt=spi_target_temp; break;
			case 0x02: motor3.tgt=spi_target_temp; break;

			case 0x04: motor1.div=spi_target_temp; break;
			case 0x05: motor2.div=spi_target_temp; break;
			case 0x06: motor3.div=spi_target_temp; break;

			case 0x0C: motor1.tgt=spi_target_temp;
			case 0x08: motor1.pos=spi_target_temp; break;
			case 0x0D: motor2.tgt=spi_target_temp;
			case 0x09: motor2.pos=spi_target_temp; break;
			case 0x0E: motor3.tgt=spi_target_temp;
			case 0x0A: motor3.pos=spi_target_temp; break;


#ifdef HAS_MOTOR4
			case 0x03: motor4.tgt=spi_target_temp; break;
			case 0x0F: motor4.tgt=spi_target_temp;
			case 0x0B: motor4.pos=spi_target_temp; break;
			case 0x07: motor4.div=spi_target_temp; break;
#endif
			}
			spi_state=1;
		}
		return;
	}

	if(spi_state<12)
	{
		if(spi_state==8) /* Get Value, Byte 0 MSB */
			{ spi_out=spi_target_temp>>24;	spi_state=9; }
		else if(spi_state==9) /* Get Value, Byte 1 */
			{ spi_out=spi_target_temp>>16;	spi_state=10; }
		else if(spi_state==10) /* Get Value, Byte 2 */
			{ spi_out=spi_target_temp>>8;	spi_state=11; }
		else
			{ spi_out=spi_target_temp;		spi_state=1; }
		return;
	}
}


void __start()
{
	char tb[256];
	char *s0;

	int x0, y0, z0;
	int *px0, *py0;

	y0=3;
	z0=4;

	px0=&y0;
	py0=&z0;
	x0=*px0;
	y0=*py0;

//	x0=3;
//	y0=4;

	puts("Tst 0\n");

	z0=x0+y0;
	if(z0!=7)	__debugbreak();
	z0=x0-y0;
	if(z0!=-1)	__debugbreak();
	z0=x0*y0;
	if(z0!=12)	__debugbreak();
	z0=x0&y0;
	if(z0!=0)	__debugbreak();
	z0=x0|y0;
	if(z0!=7)	__debugbreak();
	z0=x0^y0;
	if(z0!=7)	__debugbreak();

	z0=x0*y0;
	z0=z0/5;
	if(z0!=2)	__debugbreak();

	z0=x0<<5;
	if(z0!=96)	__debugbreak();

	puts("Tst 1\n");


	spi_in=0;
	spi_out=0xFF;
	spi_pos=0;

//	P1OUT|=0x40;

	p1in=P1IN;
	p2in=P2IN;
	lp2in=p2in;
	lp1in=p1in;
//	p2in=0;
//	lp2in=0;
	p2out=0;
	while(1)
	{
		s0=async_gets(tb);
		if(s0)
		{
			puts("got: ");
			puts(s0);
			puts("\n");
			tb[0]=0;
		}
	
//		P2DIR|=0x70;
//		p2out^=0x20;		//debug strobe

		lp1in=p1in;
		lp2in=p2in;
		p1in=P1IN;
		p2in=P2IN;
		if(p2in&0x01)		/* CSEL */
		{
//			P2DIR|=0x08;
			P1DIR|=0x80;
//			if((p2in&(p2in^lp2in))&2)
			if((p1in&(p1in^lp1in))&0x20)
			{
//				p2out^=0x10;	//debug strobe (clock pulse bit)

#if 1
				spi_in=(spi_in<<1);
//				if(p2in&0x04)
				if(p1in&0x40)
					spi_in|=1;

				spi_pos++;
				if(spi_pos>=8)
				{
//					p2out|=0x40;
					spi_pos=0;
					spi_newbyte(spi_in);
				}else
				{
//					p2out&=~0x40;
				}

				if(spi_out&128)
				{
//					p2out|=0x08;
					p1out|=0x80;
				}
				else
				{
//					p2out&=~0x08;
					p1out&=~0x80;
				}
				spi_out=(spi_out<<1);
#endif
			}
		}else
		{
			P1DIR&=0x7F;
		}
	}
}
