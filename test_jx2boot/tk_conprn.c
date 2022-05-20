struct conparm_s {
byte x;
byte y;
byte ena;
// volatile u32 *buf;
u64 buf_addr;
u32 text_attr;
u32 text_attr_dfl;
u16 fgclr_555;
u16 bgclr_555;
};

struct conparm_s tk_con_base;
struct conparm_s *tk_con;

// byte tk_con_x;
// byte tk_con_y;
// volatile u32 *tk_con_buf;

// #define TK_CONWIDTH		40
#define TK_CONWIDTH		80
#define TK_CONHEIGHT	25
#define TK_CONHEIGHTN1	24

u64 tk_con_glyphs[96];

// #define TK_CON_USE14SEG
#define TK_CON_USE5X6SEG

#ifdef TK_CON_USE14SEG

/*
  ********
  *0000000
  *58*9*A1
  *5*89A*1
  **66*77*
  *4*BCD*2
  *4B*C*D2
  *3333333
  
  FEDC BA98 7654 3210

  ********
  *00000**
  *589A1**
  *589A1**
  **6*7***
  *4BCD2**
  *4BCD2**
  *33333**
  
 */

// u16 tk_con_glyphs_14seg[64]={
u16 tk_con_glyphs_14seg[96]={
0x0000, 0x4006, 0x0202, 0x12CC, 0x12ED, 0x0C24, 0x2359, 0x0200, /* 20..27 */
0x2400, 0x0900, 0x3F00, 0x12C0, 0x0800, 0x00C0, 0x1000, 0x0C00, /* 28..2F */

0x0C3F, 0x0406, 0x00DB, 0x008F, 0x00E6, 0x00ED, 0x00FD, 0x1401, /* 30..37 */
0x00FF, 0x00E7, 0x0012, 0x0802, 0x2440, 0x00C1, 0x0980, 0x1083, /* 38..3F */

0x02BB, 0x00F7, 0x128F, 0x0039, 0x120F, 0x00F9, 0x00F1, 0x00BD, /* 40..47 */
0x00F6, 0x1209, 0x001E, 0x2470, 0x0038, 0x0536, 0x2136, 0x003F, /* 48..4F */
0x00F3, 0x203F, 0x20F3, 0x018D, 0x1201, 0x003E, 0x0C30, 0x2836, /* 50..57 */
0x2D00, 0x1500, 0x0C09, 0x0039, 0x1200, 0x000F, 0x2800, 0x0008, /* 58..5F */

#if 1
0x02BB, 0x00F7, 0x128F, 0x0039, 0x120F, 0x00F9, 0x00F1, 0x00BD, /* 40..47 */
0x00F6, 0x1209, 0x001E, 0x2470, 0x0038, 0x0536, 0x2136, 0x003F, /* 48..4F */
0x00F3, 0x203F, 0x20F3, 0x018D, 0x1201, 0x003E, 0x0C30, 0x2836, /* 50..57 */
0x2D00, 0x1500, 0x0C09, 0x0039, 0x1200, 0x000F, 0x2800, 0x0008, /* 58..5F */
#endif

#if 0
0x0100, 0x1058, 0x2078, 0x00D8, 0x088E, 0x0858, 0x14C0, 0x048E, /* 60..67 */
0x1070, 0x1000, 0x0A10, 0x3600, 0x0030, 0x10D4, 0x1050, 0x00DC, /* 68..6F */
0x0170, 0x0486, 0x0050, 0x2088, 0x0078, 0x001C, 0x0810, 0x2814, /* 70..77 */
0x2D00, 0x028E, 0x0848, 0x0949, 0x1200, 0x2489, 0x0CC0, 0x0000, /* 78..7F */
#endif

#if 0
0x42BB, 0x40F7, 0x528F, 0x4039, 0x520F, 0x40F9, 0x40F1, 0x40BD, /* 60..67 */
0x40F6, 0x5209, 0x401E, 0x6470, 0x4038, 0x4536, 0x6136, 0x403F, /* 68..6F */
0x40F3, 0x603F, 0x60F3, 0x418D, 0x5201, 0x403E, 0x4C30, 0x6836, /* 70..77 */
0x6D00, 0x5500, 0x4C09, 0x4039, 0x5200, 0x400F, 0x6800, 0x4008, /* 78..7F */
#endif

};
#endif

#ifdef TK_CON_USE5X6SEG
u32 tk_con_glyphs_5x6seg[96]={
0x00000000, /* 20 ' ' */
0x08421004, /* 21 '!' */
0x12948000, /* 22 '"' */
0x15F52BEA, /* 23 '#' */
0x1F0707C4, /* 24 '$' */
0x23222620, /* 25 '%' */
0x24E4564C, /* 26 '&' */
0x08420000, /* 27 ''' */
0x04421082, /* 28 '(' */
0x08210844, /* 29 ')' */
0x144F9140, /* 2A '*' */
0x084F9080, /* 2B '+' */
0x40001088, /* 2C ',' */
0x00078000, /* 2D '-' */
0x40001080, /* 2E '.' */
0x02222200, /* 2F '/' */
0x1D19F62E, /* 30 '0' */
0x08C2109F, /* 31 '1' */
0x1D11111F, /* 32 '2' */
0x3E220A2E, /* 33 '3' */
0x04654BE2, /* 34 '4' */
0x3F0F062E, /* 35 '5' */
0x1F0F462E, /* 36 '6' */
0x3E111108, /* 37 '7' */
0x1D17462E, /* 38 '8' */
0x1D17844C, /* 39 '9' */
0x48401080, /* 3A ':' */
0x48401088, /* 3B ';' */
0x04441041, /* 3C '<' */
0x00F001E0, /* 3D '=' */
0x08208888, /* 3E '>' */
0x1D111004, /* 3F '?' */
0x1D3ADE0F, /* 40 '@' */
0x08A8FE31, /* 41 'A' */
0x3D1F463E, /* 42 'B' */
0x1D18422E, /* 43 'C' */
0x3928C65C, /* 44 'D' */
0x3F0F421F, /* 45 'E' */
0x3F0F4210, /* 46 'F' */
0x1D185E2E, /* 47 'G' */
0x231FC631, /* 48 'H' */
0x1C42108E, /* 49 'I' */
0x0210862E, /* 4A 'J' */
0x232E4A31, /* 4B 'K' */
0x2108421F, /* 4C 'L' */
0x23BAC631, /* 4D 'M' */
0x239AD671, /* 4E 'N' */
0x1D18C62E, /* 4F 'O' */
0x3D18FA10, /* 50 'P' */
0x1D18C64D, /* 51 'Q' */
0x3D18FA51, /* 52 'R' */
0x1F07043E, /* 53 'S' */
0x3E421084, /* 54 'T' */
0x2318C62E, /* 55 'U' */
0x2318C544, /* 56 'V' */
0x2318D771, /* 57 'W' */
0x22A21151, /* 58 'X' */
0x23151084, /* 59 'Y' */
0x3E22221F, /* 5A 'Z' */
0x0E421087, /* 5B '[' */
0x00820820, /* 5C '\' */
0x1C21084E, /* 5D ']' */
0x08A88000, /* 5E '^' */
0x0000001F, /* 5F '_' */
0x08410000, /* 60 '`' */
0x00E0BE2F, /* 61 'a' */
0x210F463E, /* 62 'b' */
0x00E8C22E, /* 63 'c' */
0x0217C62F, /* 64 'd' */
0x00E8FE0F, /* 65 'e' */
0x0E8FA108, /* 66 'f' */
0x5D18BC3E, /* 67 'g' */
0x210F4631, /* 68 'h' */
0x0806108E, /* 69 'i' */
0x0401085C, /* 6A 'j' */
0x21197251, /* 6B 'k' */
0x1842108E, /* 6C 'l' */
0x01EAD6B1, /* 6D 'm' */
0x01E8C631, /* 6E 'n' */
0x00E8C62E, /* 6F 'o' */
0x7D18FA10, /* 70 'p' */
0x5F18BC21, /* 71 'q' */
0x21BE4210, /* 72 'r' */
0x00F8383E, /* 73 's' */
0x09F21083, /* 74 't' */
0x0118C62E, /* 75 'u' */
0x0118C544, /* 76 'v' */
0x011AD5CA, /* 77 'w' */
0x01151151, /* 78 'x' */
0x5294984C, /* 79 'y' */
0x01F1111F, /* 7A 'z' */
0x064C1083, /* 7B '{' */
0x08401084, /* 7C '|' */
0x30419098, /* 7D '}' */
0x1B600000, /* 7E '~' */
0x00022A3F, /* 7F ' ' */
};

#endif

#ifdef TK_CON_USE14SEG

u64 tk_con_14seg2pix(u16 v);

__asm {

#if 0
.rodata

	.balign 8
tk_con_14seg2pix_tab:
	.qword	0x007C000000000000
	.qword	0x0004040404000000
	.qword	0x0000000004040404
	.qword	0x000000000000007C

	.qword	0x0000000040404040
	.qword	0x0040404040000000
	.qword	0x0000000070000000
	.qword	0x000000001C000000

	.qword	0x0040202010000000
	.qword	0x0010101010000000
	.qword	0x0004080810000000
	.qword	0x0000000010202040

	.qword	0x0000000010101010
	.qword	0x0000000010080804


.text

#endif


tk_con_14seg2pix:

#if 0
	mov		tk_con_14seg2pix_tab, r7
	mov.x	(r7,  0), r16
	mov.x	(r7, 16), r18
	mov.x	(r7, 32), r20
	mov.x	(r7, 48), r22
#endif

#if 1
	mov		0x007C000000000000, r16
	mov		0x0004040404000000, r17
	mov		0x0000000004040404, r18
	mov		0x000000000000007C, r19

	mov		0x0000000040404040, r20
	mov		0x0040404040000000, r21
	mov		0x0000000070000000, r22
	mov		0x000000001C000000, r23
#endif

	mov		0, r2
	
	test	0x0001, r4
	or?f	r16, r2
	test	0x0002, r4
	or?f	r17, r2
	test	0x0004, r4
	or?f	r18, r2
	test	0x0008, r4
	or?f	r19, r2

	test	0x0010, r4
	or?f	r20, r2
	test	0x0020, r4
	or?f	r21, r2
	test	0x0040, r4
	or?f	r22, r2
	test	0x0080, r4
	or?f	r23, r2

#if 0
	mov.x	(r7, 64), r16
	mov.x	(r7, 80), r18
	mov.x	(r7, 96), r20
#endif

#if 1
	mov		0x0040202010000000, r16
	mov		0x0010101010000000, r17
	mov		0x0004080810000000, r18
	mov		0x0000000010202040, r19

	mov		0x0000000010101010, r20
	mov		0x0000000010080804, r21
	mov		0x0000000000000303, r22
#endif

	test	0x0100, r4
	or?f	r16, r2
	test	0x0200, r4
	or?f	r17, r2
	test	0x0400, r4
	or?f	r18, r2
	test	0x0800, r4
	or?f	r19, r2

	test	0x1000, r4
	or?f	r20, r2
	test	0x2000, r4
	or?f	r21, r2

	test	0x4000, r4
	or?f	r22, r2

	rts

};
#endif

void tk_con_init()
{
	u64 tv;
	int i, j, k;

	tk_con=&tk_con_base;
//	tk_con_buf=(u32 *)0xF00A0000;
//	tk_con_x=0;
//	tk_con_y=0;

//	tk_con->buf=(u32 *)0xF00A0000;
	tk_con->buf_addr=0xFFFFF00A0000ULL;
	tk_con->x=0;
	tk_con->y=0;

#ifndef JX2UC
//	((u32 *)0xF00BFF00)[0]=0x0015;		//320x200x16bpp
//	((u32 *)0xF00BFF00)[0]=0x0005;		//
//	((u32 *)0xF00BFF00)[0]=0x0001;		//
	((u32 *)0xF00BFF00)[0]=0x0081;		//
#endif

#ifdef TK_CON_USE5X6SEG
	for(i=0; i<96; i++)
	{
//		tv=0;
		j=tk_con_glyphs_5x6seg[i];
		
//		tv=
//			(((u64)(j&0x3E000000))<<17) |
//			(((u64)(j&0x01F00000))<<14) |
//			(((u64)(j&0x000F8000))<<11) |
//			(((u64)(j&0x00007C00))<< 8) |
//			(((u64)(j&0x000003E0))<< 5) |
//			(((u64)(j&0x0000001F))<< 2) ;

		tv=
			(((u64)(j&0x3E000000))<<25) |
			(((u64)(j&0x01F00000))<<22) |
			(((u64)(j&0x000F8000))<<19) |
			(((u64)(j&0x00007C00))<<16) |
			(((u64)(j&0x000003E0))<<13) |
			(((u64)(j&0x0000001F))<<10) ;
		if(j&0x40000000)
			tv>>=8;
		
		tk_con_glyphs[i]=tv;
	}
#endif

#ifdef TK_CON_USE14SEG
//	for(i=0; i<64; i++)
	for(i=0; i<96; i++)
	{
		tv=0;
		j=tk_con_glyphs_14seg[i];

//  ********
 // *00000**
//  *589A1**
//  *589A1**
//  **6*7***
//  *4BCD2**
//  *4BCD2**
//  *33333**

		tv=tk_con_14seg2pix(j);

#if 0
		if(j&0x0001)	tv|=0x007C000000000000ULL;
//		if(j&0x0001)	tv|=0x0038000000000000ULL;
		if(j&0x0002)	tv|=0x0004040404000000ULL;
//		if(j&0x0002)	tv|=0x0000040404000000ULL;
		if(j&0x0004)	tv|=0x0000000004040404ULL;
//		if(j&0x0004)	tv|=0x0000000004040400ULL;
		if(j&0x0008)	tv|=0x000000000000007CULL;
//		if(j&0x0008)	tv|=0x0000000000000038ULL;
		
		if(j&0x0010)	tv|=0x0000000040404040ULL;
//		if(j&0x0010)	tv|=0x0000000040404000ULL;
		if(j&0x0020)	tv|=0x0040404040000000ULL;
//		if(j&0x0020)	tv|=0x0000404040000000ULL;
		if(j&0x0040)	tv|=0x0000000070000000ULL;
		if(j&0x0080)	tv|=0x000000001C000000ULL;
		
		if(j&0x0100)	tv|=0x0040202010000000ULL;
		if(j&0x0200)	tv|=0x0010101010000000ULL;
		if(j&0x0400)	tv|=0x0004080810000000ULL;
		if(j&0x0800)	tv|=0x0000000010202040ULL;

		if(j&0x1000)	tv|=0x0000000010101010ULL;
		if(j&0x2000)	tv|=0x0000000010080804ULL;
#endif

#if 0
		if(j&0x0001)	tv|=0x007F000000000000ULL;
		if(j&0x0002)	tv|=0x0001010101000000ULL;
		if(j&0x0004)	tv|=0x0000000001010101ULL;
		if(j&0x0008)	tv|=0x000000000000007FULL;
		
		if(j&0x0010)	tv|=0x0000000040404040ULL;
		if(j&0x0020)	tv|=0x0040404040000000ULL;
		if(j&0x0040)	tv|=0x0000000070000000ULL;
		if(j&0x0080)	tv|=0x000000000E000000ULL;
		
		if(j&0x0100)	tv|=0x0000201008000000ULL;
		if(j&0x0200)	tv|=0x0008080808000000ULL;
		if(j&0x0400)	tv|=0x0000020408000000ULL;
		if(j&0x0800)	tv|=0x0000000008102000ULL;

		if(j&0x1000)	tv|=0x0000000008080808ULL;
		if(j&0x2000)	tv|=0x0000000008040200ULL;
#endif

		tk_con_glyphs[i]=tv;
	}

#if 0
	for(; i<95; i++)
	{
		j=i-32;
		tv=tk_con_glyphs[j];
		tk_con_glyphs[i]=tv;
	}
#endif

#endif
}

void *tk_con_getctx()
{
	return(&tk_con);
}

void tk_con_reset()
{
	tk_con_init();
}

#ifndef JX2UC
void tk_con_scroll_up()
{
	volatile u32 *buf;
	u64 q0, q1;
//	u32 p0, p1, p2, p3;
	int i0, i1;
	int i, j, k;

//	buf=tk_con->buf;
	buf=(volatile u32 *)(tk_con->buf_addr);

#if 1
	for(i=0; i<TK_CONHEIGHTN1; i++)
	{
		i0=((i+0)*TK_CONWIDTH)*8;
		i1=((i+1)*TK_CONWIDTH)*8;
		for(j=0; j<TK_CONWIDTH; j++)
		{
			q0=*(u64 *)(buf+i1+0);
			q1=*(u64 *)(buf+i1+2);
			*(u64 *)(buf+i0+0)=q0;
			*(u64 *)(buf+i0+2)=q1;
			i0+=8;
			i1+=8;
		}
	}

	i0=(TK_CONHEIGHTN1*TK_CONWIDTH)*8;
	for(j=0; j<TK_CONWIDTH; j++)
		{ *(u64 *)(buf+i0)=0; i0+=8; }
#endif

#if 0
//	for(i=0; i<24; i++)
	for(i=0; i<TK_CONHEIGHTN1; i++)
		for(j=0; j<TK_CONWIDTH; j++)
	{
		i0=((i+0)*TK_CONWIDTH+j)*8;
		i1=((i+1)*TK_CONWIDTH+j)*8;
//		p0=tk_con_buf[i1+0];
		p0=buf[i1+0];
//		p1=tk_con_buf[i1+1];
//		p2=tk_con_buf[i1+2];	p3=tk_con_buf[i1+3];
//		tk_con_buf[i0+0]=p0;
		buf[i0+0]=p0;
//		tk_con_buf[i0+1]=p1;
//		tk_con_buf[i0+2]=p2;	tk_con_buf[i0+3]=p3;
//		p0=tk_con_buf[i1+4];	p1=tk_con_buf[i1+5];
//		p2=tk_con_buf[i1+6];	p3=tk_con_buf[i1+7];
//		tk_con_buf[i0+4]=p0;	tk_con_buf[i0+5]=p1;
//		tk_con_buf[i0+6]=p2;	tk_con_buf[i0+7]=p3;
	}

//	for(j=0; j<40; j++)
	for(j=0; j<TK_CONWIDTH; j++)
	{
//		tk_con_buf[((24*40)+j)*8]=0;
//		tk_con_buf[((24*TK_CONWIDTH)+j)*8]=0;
//		tk_con_buf[((TK_CONHEIGHTN1*TK_CONWIDTH)+j)*8]=0;
		buf[((TK_CONHEIGHTN1*TK_CONWIDTH)+j)*8]=0;
	}
#endif
}

void tk_con_newline()
{
//	tk_con_x=0;
	tk_con->x=0;
//	tk_con_y++;
	tk_con->y++;
//	if(tk_con_y>=25)
	if(tk_con->y>=25)
	{
		tk_con_scroll_up();
//		tk_con_y--;
		tk_con->y--;
	}
}
#endif

void tk_con_putc(int ch)
{
#ifndef JX2UC
	volatile u32 *buf;
	u64 q0, q1;
	u32 px;
	int tz;

	buf=(volatile u32 *)(tk_con->buf_addr);

	if(ch<' ')
	{
		if(ch=='\r')
//			{ tk_con_x=0; return; }
			{ tk_con->x=0; return; }
		if(ch=='\n')
		{
			tk_con_newline();
			return;
		}
		if(ch=='\t')
		{
//			tk_con_x=(tk_con_x+8)&(~7);
			tk_con->x=(tk_con->x+8)&(~7);
//			if(tk_con_x>=TK_CONWIDTH)
			if(tk_con->x>=TK_CONWIDTH)
				{ tk_con_newline(); }
			return;
		}
		return;
	}
	
#if 0
//	px=0x0FC00000|ch;
	px=0x003F0000|ch;
//	tk_con_buf[(tk_con_y*TK_CONWIDTH+tk_con_x)*8+0]=px;
//	tk_con->buf[(tk_con->y*TK_CONWIDTH+tk_con->x)*8+0]=px;
	buf[(tk_con->y*TK_CONWIDTH+tk_con->x)*8+0]=px;
#endif

//	q0=(2ULL<<30)|(tk_con->bgclr_555<<15)|tk_con->fgclr_555;
	q0=(2ULL<<30)|(0<<15)|0x7FFF;
	q1=tk_con_glyphs[ch-' '];

	tz=(tk_con->y*TK_CONWIDTH+tk_con->x)*4;
	((u64 *)buf)[tz+0]=q0;
	((u64 *)buf)[tz+1]=q1;


//	tk_con_x++;
	tk_con->x++;
//	if(tk_con_x>=TK_CONWIDTH)
	if(tk_con->x>=TK_CONWIDTH)
	{
		tk_con_newline();
	}
#endif
}
