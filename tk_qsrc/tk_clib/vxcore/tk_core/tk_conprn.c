// byte tk_con_x;
// byte tk_con_y;
// byte tk_con_ena;
// volatile u32 *tk_con->buf;

struct conparm_s {
byte x;
byte y;
byte ena;
byte resv_1;
//byte attr;
//byte fgclr;
//byte bgclr;
//byte resv_2;
//byte resv_3;
volatile u32 *buf;
u32 text_attr;
u32 text_attr_dfl;
u16 fgclr_555;
u16 bgclr_555;
};

struct conparm_s tk_con_bss;

struct conparm_s *tk_con=NULL;


u64 tk_gfxcon_glyphs[256]=
{
0x0000000000000000,	//0x00
0x7c82aa82aaba827c,	//0x01
0x007effdbffdbc37e,	//0x02
0x00367f7f7f3e1c08,	//0x03
0x00081c3e3e3e1c08,	//0x04
0x1c1c087f6b08087f,	//0x05
0x081c3e7f7f7f087f,	//0x06
0x00001c3e3e1c0000,	//0x07
0x3f3f332121333f3f,	//0x08
0x000e1111110e0000,	//0x09
0x3f3f332d2d333f3f,	//0x0A
0x000f030539484830,	//0x0B
0x1c2222221c083e08,	//0x0C
0x00040605040c1c18,	//0x0D
0x0c0e0b0909393307,	//0x0E
0x08492a1c7f1c2a49,	//0x0F
0x0000303c3e3c3000,	//0x10
0x00000c3c7c3c0c00,	//0x11
0x183c7e18187e3c18,	//0x12
0x0000121212001200,	//0x13
0x00071d3d1d050505,	//0x14
0x0007080e0907010e,	//0x15
0x0000000f0f000000,	//0x16
0x081c08081c08003e,	//0x17
0x0000081c2a080808,	//0x18
0x00000808082a1c08,	//0x19
0x000004023f020400,	//0x1A
0x000008103f100800,	//0x1B
0x0000000808080f00,	//0x1C
0x00002442ff422400,	//0x1D
0x0000081c1c3e3e00,	//0x1E
0x00003e3e1c1c0800,	//0x1F

0x0000000000000000,	//0x20 " "
0x0010101010001000,	//0x21 "!"
0x0044444400000000,	//0x22 "\""
0x0044FF4444FF4400,	//0x23 "#"
0x103C403C047C1000,	//0x24 "$"
0x0044481020444400,	//0x25 "%"
0x1C241C384F443A00,	//0x26 "&"
0x0010101000000000,	//0x27 "'"
0x0008101010100800,	//0x28 "("
0x0020101010102000,	//0x29 ")"
0x00443CFF3C440000,	//0x2A "*"
0x0010107E10100000,	//0x2B "+"
0x0000000000101020,	//0x2C ","
0x0000007E00000000,	//0x2D "-"
0x0000000000101000,	//0x2E "."
0x0002040810204000,	//0x2F "/"
0x0038444C74443800,	//0x20 "0"
0x0010301010107E00,	//0x31 "1"
0x003C440810207E00,	//0x32 "2"
0x007C081008443C00,	//0x33 "3"
0x00081828487C0800,	//0x34 "4"
0x007E407C04443C00,	//0x35 "5"
0x003C407C44443800,	//0x36 "6"
0x007E040810202000,	//0x37 "7"
0x0038443844443800,	//0x38 "8"
0x003C443E04083000,	//0x39 "9"
0x0000101000101000,	//0x3A ":"
0x0000101000101020,	//0x3B ";"
0x0408102010080400,	//0x3C "<"
0x00007E00007E0000,	//0x3D "="
0x4020100810204000,	//0x3E ">"
0x003C440810001000,	//0x3F "?"
0x003C446E6E403E00,	//0x40 "@"
0x001038447C444400,	//0x41 "A"
0x0078447844447800,	//0x42 "B"
0x0038444040443800,	//0x43 "C"
0x0070484444487000,	//0x44 "D"
0x007E407C40407E00,	//0x45 "E"
0x007E407C40404000,	//0x46 "F"
0x003E40404E443C00,	//0x47 "G"
0x0044447C44444400,	//0x48 "H"
0x007E101010107E00,	//0x49 "I"
0x0004040404443800,	//0x4A "J"
0x0044487870484400,	//0x4B "K"
0x0040404040407E00,	//0x4C "L"
0x0042667E5A424200,	//0x4D "M"
0x00446454544C4400,	//0x4E "N"
0x0038444444443800,	//0x4F "O"
0x0078444478404000,	//0x50 "P"
0x003C444444483400,	//0x51 "Q"
0x0078444478484400,	//0x52 "R"
0x003C403C04047800,	//0x53 "S"
0x007E101010101000,	//0x54 "T"
0x0044444444443800,	//0x55 "U"
0x0044444444381000,	//0x56 "V"
0x0042425A5A664200,	//0x57 "W"
0x0044443838444400,	//0x58 "X"
0x0044443810101000,	//0x59 "Y"
0x007C081020407C00,	//0x5A "Z"
0x001E101010101E00,	//0x5B "["
0x0040201008040200,	//0x5C "\\"
0x0078080808087800,	//0x5D "]"
0x0008142200000000,	//0x5E "^"
0x000000000000FF00,	//0x5F "_"
0x0020201000000000,	//0x40 "`"
0x000038043C443E00,	//0x61 "a"
0x0040407844447800,	//0x62 "b"
0x0000384440443800,	//0x42 "c"
0x0004043C44443C00,	//0x64 "d"
0x000038447E403C00,	//0x65 "e"
0x000E103E10101000,	//0x44 "f"
0x00003844443C0478,	//0x67 "g"
0x0040407844444400,	//0x68 "h"
0x0010003010103800,	//0x69 "i"
0x0004000404040438,	//0x6A "j"
0x0040444870484400,	//0x6B "k"
0x0030101010103800,	//0x48 "l"
0x00007C5252524200,	//0x6D "m"
0x0000784444444400,	//0x6E "n"
0x0000384444443800,	//0x6F "o"
0x0000784444784040,	//0x70 "p"
0x00003C44443C0404,	//0x71 "q"
0x00406E7040404000,	//0x72 "r"
0x00003C4038047800,	//0x73 "s"
0x00107E1010100E00,	//0x74 "t"
0x0000444444443800,	//0x75 "u"
0x0000444444381000,	//0x76 "v"
0x00004252523C2400,	//0x77 "w"
0x0000442810284400,	//0x78 "x"
0x0000444444380870,	//0x79 "y"
0x00007C0810207C00,	//0x7A "z"
0x000E107010100E00,	//0x7B "{"
0x0010100010101000,	//0x7C "|"
0x0070100E10107000,	//0x7D "}"
0x0077DC0000000000,	//0x7E "~"
0x000000081C36427F,	//0x7F "\x7F"

0x000f0908090f020e,	//0x80 "?"
0x000a001212120c00,	//0x81 "?"
0x0618000e111f100f,	//0x82 "?"
0x0609000e010f110f,	//0x83 "?"
0x000a000e010f110f,	//0x84 "?"
0x0c03000e010f110f,	//0x85 "?"
0x0606000e010f110f,	//0x86 "?"
0x0000070808070206,	//0x87 "?"
0x040a000e111f100f,	//0x88 "?"
0x000a000e111f100f,	//0x89 "?"
0x0c03000e111f100f,	//0x8A "?"
0x000a000602020207,	//0x8B "?"
0x0205000602020207,	//0x8C "?"
0x0c03000602020207,	//0x8D "?"
0x09000e1b111f1111,	//0x8E "?"
0x0e000e1b111f1111,	//0x8F "?"
0x00000f080e080f00,	//0x90 "?"
0x00003c0a3e50522c,	//0x91 "?"
0x00003e487e484e00,	//0x92 "?"
0x040a000609090600,	//0x93 "?"
0x000a000609090600,	//0x94 "?"
0x0c03000609090600,	//0x95 "?"
0x040a000909090600,	//0x96 "?"
0x0c03000909090600,	//0x97 "?"
0x000a000909070106,	//0x98 "?"
0x0a00060909090906,	//0x99 "?"
0x0500090909090600,	//0x9A "?"
0x00040e1514150e04,	//0x9B "?"
0x000007040e040f00,	//0x9C "?"
0x110a041f041f0404,	//0x9D "?"
0x0000302b342a2906,	//0x9E "?"
0x0002040e04040418,	//0x9F "?"
0x030c000e010f110f,	//0xA0 "?"
0x0618000c04040e00,	//0xA1 "?"
0x030c000609090600,	//0xA2 "?"
0x030c000909090600,	//0xA3 "?"
0x050a000609090900,	//0xA4 "?"
0x050a00090d0f0b09,	//0xA5 "?"
0x18041c2418003e00,	//0xA6 "?"
0x0018242418003c00,	//0xA7 "?"
0x0008000810221c00,	//0xA8 "?"
0x000000000f080808,	//0xA9 "?"
0x00000000f8080808,	//0xAA "?"
0x4244485721470407,	//0xAB "?"
0x4244485525470101,	//0xAC "?"
0x0000020002020200,	//0xAD "?"
0x0014142828141400,	//0xAE "?"
0x0014140a0a141400,	//0xAF "?"
0xaa00aa00aa00aa00,	//0xB0 "?"
0xaa55aa55aa55aa55,	//0xB1 "?"
0xaaffaaffaaffaaff,	//0xB2 "?"
0x0808080808080808,	//0xB3 "?"
0x080808f808080808,	//0xB4 "?"
0x0808f808f8080808,	//0xB5 "?"
0x14141414f4141414,	//0xB6 "?"
0x00000000fc141414,	//0xB7 "?"
0x000000f808f80808,	//0xB8 "?"
0x141414f404f41414,	//0xB9 "?"
0x1414141414141414,	//0xBA "?"
0x000000fc04f41414,	//0xBB "?"
0x141414f404fc0000,	//0xBC "?"
0x14141414fc000000,	//0xBD "?"
0x0808083808380000,	//0xBE "?"
0x0000001c04040404,	//0xBF "?"
0x080808080f000000,	//0xC0 "?"
0x10101010ff000000,	//0xC1 "?"
0x00000000ff080808,	//0xC2 "?"
0x080808080f080808,	//0xC3 "?"
0x00000000ff000000,	//0xC4 "?"
0x08080808ff080808,	//0xC5 "?"
0x0808080f080f0808,	//0xC6 "?"
0x1414141417141414,	//0xC7 "?"
0x0808080f080f0000,	//0xC8 "?"
0x0000001f10171414,	//0xC9 "?"
0x141414f404fc0000,	//0xCA "?"
0x000000ff00f71414,	//0xCB "?"
0x1414141710171414,	//0xCC "?"
0x000000ff00ff0000,	//0xCD "?"
0x141414f700f71414,	//0xCE "?"
0x040404ff00ff0000,	//0xCF "?"
0x141414143e000000,	//0xD0 "?"
0x0000003e003e0808,	//0xD1 "?"
0x000000001f141414,	//0xD2 "?"
0x141414141f000000,	//0xD3 "?"
0x0808080f080f0000,	//0xD4 "?"
0x0000000f080f0808,	//0xD5 "?"
0x000000001f141414,	//0xD6 "?"
0x14141414f7141414,	//0xD7 "?"
0x080808ff08ff0808,	//0xD8 "?"
0x08080808ff000000,	//0xD9 "?"
0x000000000f080808,	//0xDA "?"
0xffffffffffffffff,	//0xDB "?"
0x00000000ffffffff,	//0xDC "?"
0xf0f0f0f0f0f0f0f0,	//0xDD "?"
0x0f0f0f0f0f0f0f0f,	//0xDE "?"
0xffffffff00000000,	//0xDF "?"
0x00023a4444443a02,	//0xE0 "?"
0x00382c243e22362c,	//0xE1 "?"
0x003c202020202000,	//0xE2 "?"
0x00003e1414141400,	//0xE3 "?"
0x003e10080408103e,	//0xE4 "?"
0x00001f2222221c00,	//0xE5 "?"
0x00000012121e1010,	//0xE6 "?"
0x0000003e08080806,	//0xE7 "?"
0x7f083e49493e087f,	//0xE8 "?"
0x001c22223e22221c,	//0xE9 "?"
0x001c222222221422,	//0xEA "?"
0x000704040e11110e,	//0xEB "?"
0x0000003649360000,	//0xEC "?"
0x0026494949493e08,	//0xED "?"
0x000609101e100906,	//0xEE "?"
0x00001c2222222222,	//0xEF "?"
0x00003e003e003e00,	//0xF0 "?"
0x0008083e0808003e,	//0xF1 "?"
0x0804020408001f00,	//0xF2 "?"
0x0204080402001f00,	//0xF3 "?"
0x0000030202020202,	//0xF4 "?"
0x0202020202060000,	//0xF5 "?"
0x000008003e000800,	//0xF6 "?"
0x0000324c00324c00,	//0xF7 "?"
0x0008141408000000,	//0xF8 "?"
0x000000000c0c0000,	//0xF9 "?"
0x000000000c000000,	//0xFA "?"
0x0007040424140c04,	//0xFB "?"
0x00000e0909090000,	//0xFC "?"
0x0007010704070000,	//0xFD "?"
0x0000001e1e1e1e00,	//0xFE "?"
};

byte tk_con_clr16to64[16]={
	0x00, 0x20, 0x08, 0x28, 0x02, 0x22, 0x0A, 0x2A,
	0x00, 0x30, 0x0C, 0x3C, 0x03, 0x33, 0x0F, 0x3F };



// #define TK_CONWIDTH		40
#define TK_CONWIDTH		80
#define TK_CONHEIGHT	25
#define TK_CONHEIGHTN1	24

void *TK_ConGetCtxV(void)
{
	TK_SysArg ar[4];
	void *p;
	
//	ar[0].i=sz;
	p=NULL;
	tk_syscall(NULL, TK_UMSG_GETCONPARAM, &p, ar);
	return(p);
}

void *tk_con_getctx(void)
{
	return(tk_con);
}

void tk_con_init()
{
	if(tk_iskernel())
	{
//		tk_con=&tk_con_bss;
		tk_con=NULL;
	}else
	{
		tk_con=TK_ConGetCtxV();
//		if(!tk_con)
//			tk_con=&tk_con_bss;
	}

	if(!tk_con)
	{
		tk_con=&tk_con_bss;
//		tk_con->buf=(u32 *)0xF00A0000UL;
		tk_con->buf=(u32 *)(MMIO_BASE+0x000A0000ULL);
		tk_con->x=0;
		tk_con->y=0;
		tk_con->ena=1;
//		tk_con->attr=0;
//		tk_con->fgclr=0x3F;
//		tk_con->bgclr=0x00;
//		tk_con->text_attr=0x003F0000;
//		tk_con->text_attr_dfl=0x003F0000;
		tk_con->text_attr_dfl=0x002A0000;
		tk_con->text_attr=tk_con->text_attr_dfl;

		TK_Con_SetColorBg(0);
		TK_Con_SetColorFg(tk_con_clr16to64[7]);
	}

//	((u32 *)0xF00BFF00UL)[0]=0x0015;		//320x200x16bpp
//	((u32 *)0xF00BFF00UL)[0]=0x0005;		//
//	((u32 *)0xF00BFF00UL)[0]=0x0001;		//
//	((u32 *)(MMIO_BASE+0x000BFF00UL))[0]=0x0001;		//
	((u32 *)(MMIO_BASE+0x000BFF00UL))[0]=0x0081;		//
	
	if(tk_iskernel())
	{
		tk_con_clear();
	}
}

void tk_con_disable()
{
	tk_con->ena=0;
}

void tk_con_clear()
{
	u64 q0, q1;
//	u32 p0, p1, p2, p3;
	int i0, i1;
	int i, j, k;

	i1=tk_con->text_attr;

	q1=0;
	q0=(2ULL<<30)|(tk_con->bgclr_555<<15)|tk_con->fgclr_555;

	for(i=0; i<25; i++)
	{
		i0=(i*TK_CONWIDTH)*8;
		for(j=0; j<TK_CONWIDTH; j++)
		{
	//		i0=((i+0)*TK_CONWIDTH+j)*8;
	//		*(u64 *)(tk_con->buf+i0)=0;
	//		*(u64 *)(tk_con->buf+i0)=i1;
			*(u64 *)(tk_con->buf+i0+0)=q0;
			*(u64 *)(tk_con->buf+i0+2)=q1;
			i0+=8;
		}
	}

#if 0
	for(j=0; j<TK_CONWIDTH; j++)
	{
//		tk_con->buf[((24*TK_CONWIDTH)+j)*8]=0;
		tk_con->buf[((24*TK_CONWIDTH)+j)*8]=i1;
	}
#endif
}

void TK_Con_UpdateHwCursor()
{
	u32 py;
	py=((tk_con->y)<<8)|(tk_con->x);
//	((u32 *)0xF00BFF00UL)[1]=py;		//move hardware cursor
	((u32 *)(MMIO_BASE+0x000BFF00UL))[1]=py;		//move hardware cursor
}

void TK_Con_SetCursorPos(int x, int y)
{
	tk_con->x=x;
	tk_con->y=y;
	TK_Con_UpdateHwCursor();
}

void TK_Con_SetColorFg(int x)
{
	int cr, cg, cb, cc;
	tk_con->text_attr=(tk_con->text_attr&0xFFC0FFFFU)|((x&0x3F)<<16);
	
	cr=x&0x30; cg=x&0x0C; cb=x&0x03;
	cc=(cr<<9)|(cr<<7)|(cg<<6)|(cg<<4)|(cb<<3)|(cb<<1);
	tk_con->fgclr_555=cc;
}

void TK_Con_SetColorBg(int x)
{
	int cr, cg, cb, cc;
	tk_con->text_attr=(tk_con->text_attr&0xF03FFFFFU)|((x&0x3F)<<22);
	cr=x&0x30; cg=x&0x0C; cb=x&0x03;
	cc=(cr<<9)|(cr<<7)|(cg<<6)|(cg<<4)|(cb<<3)|(cb<<1);
	tk_con->bgclr_555=cc;
}

void tk_con_reset()
{
	tk_con_init();
	tk_con_clear();
	TK_Con_SetCursorPos(0, 0);
	tk_con->text_attr=tk_con->text_attr_dfl;
	TK_Con_SetColorBg(0);
	TK_Con_SetColorFg(tk_con_clr16to64[7]);
}

void tk_con_chkreset()
{
	if(!tk_con || !tk_con->ena)
	{
		tk_con_init();
		tk_con_clear();
//		((u32 *)0xF009F000)[0]=0x0029;
//		((u32 *)0xF009F000)[0]=0x0000;
//		((u32 *)(MMIO_BASE+0x000BFF00UL))[0]=0x0001;		//
		((u32 *)(MMIO_BASE+0x000BFF00UL))[0]=0x0081;		//
		TK_Con_SetCursorPos(0, 0);

		tk_con->text_attr=tk_con->text_attr_dfl;
		TK_Con_SetColorBg(0);
		TK_Con_SetColorFg(tk_con_clr16to64[7]);
	}
}

void tk_con_scroll_up()
{
	volatile u32 *buf;
	volatile u64 *qb0, *qb1;
	u64 q0, q1, q2, q3;
//	u32 p0, p1, p2, p3;
	int i0, i1;
	int i, j, k;

	buf=tk_con->buf;

#if 1
	for(i=0; i<TK_CONHEIGHTN1; i++)
	{
		i0=((i+0)*TK_CONWIDTH)*8;
		i1=((i+1)*TK_CONWIDTH)*8;

#if 1
		qb0=(u64 *)(buf+i0);
		qb1=(u64 *)(buf+i1);
		for(j=0; j<TK_CONWIDTH; j+=4)
		{
			q0=qb1[0];		q1=qb1[1];
			q2=qb1[4];		q3=qb1[5];
			qb0[0]=q0;		qb0[1]=q1;
			qb0[4]=q2;		qb0[5]=q3;

			q0=qb1[8];		q1=qb1[ 9];
			q2=qb1[12];		q3=qb1[13];
			qb0[ 8]=q0;		qb0[ 9]=q1;
			qb0[12]=q2;		qb0[13]=q3;

			qb0+=16;		qb1+=16;
		}
#endif

#if 0
		qb0=(u64 *)(buf+i0);
		qb1=(u64 *)(buf+i1);
		for(j=0; j<TK_CONWIDTH; j+=8)
		{
			q0=qb1[0];		q1=qb1[4];
			q2=qb1[8];		q3=qb1[12];
			qb0[0]=q0;		qb0[4]=q1;
			qb0[8]=q2;		qb0[12]=q3;

			q0=qb1[16];		q1=qb1[20];
			q2=qb1[24];		q3=qb1[28];
			qb0[16]=q0;		qb0[20]=q1;
			qb0[24]=q2;		qb0[28]=q3;

			qb0+=32;		qb1+=32;
		}
#endif

#if 0
		for(j=0; j<TK_CONWIDTH; j++)
		{
			q0=*(u64 *)(buf+i1);
			*(u64 *)(buf+i0)=q0;
			i0+=8;
			i1+=8;
		}
#endif
	}

	q1=0;
	q0=(2ULL<<30)|(tk_con->bgclr_555<<15)|tk_con->fgclr_555;

	i0=(TK_CONHEIGHTN1*TK_CONWIDTH)*8;
	i1=tk_con->text_attr;
	for(j=0; j<TK_CONWIDTH; j++)
//		{ *(u64 *)(buf+i0)=0; i0+=8; }
//		{ *(u64 *)(buf+i0)=i1; i0+=8; }
	{
		((u64 *)(buf+i0))[0]=q0;
		((u64 *)(buf+i0))[1]=q1;
		i0+=8;
	}

#endif

#if 0
	for(i=0; i<24; i++)
		for(j=0; j<TK_CONWIDTH; j++)
	{
		i0=((i+0)*TK_CONWIDTH+j)*8;		i1=((i+1)*TK_CONWIDTH+j)*8;

		q0=*(u64 *)(tk_con->buf+i1);
		*(u64 *)(tk_con->buf+i0)=q0;

//		p0=tk_con->buf[i1+0];
//		p1=tk_con->buf[i1+1];
//		p2=tk_con->buf[i1+2];	p3=tk_con->buf[i1+3];
//		tk_con->buf[i0+0]=p0;
//		tk_con->buf[i0+1]=p1;
//		tk_con->buf[i0+2]=p2;	tk_con->buf[i0+3]=p3;
//		p0=tk_con->buf[i1+4];
//		p1=tk_con->buf[i1+5];
//		p2=tk_con->buf[i1+6];	p3=tk_con->buf[i1+7];
//		tk_con->buf[i0+4]=p0;
//		tk_con->buf[i0+5]=p1;
//		tk_con->buf[i0+6]=p2;	tk_con->buf[i0+7]=p3;
	}

	for(j=0; j<TK_CONWIDTH; j++)
	{
		tk_con->buf[((24*TK_CONWIDTH)+j)*8]=0;
	}
#endif
}

void tk_con_scroll_down()
{
	volatile u32 *buf;
	u64 q0, q1;
//	u32 p0, p1, p2, p3;
	int i0, i1;
	int i, j, k;

	buf=tk_con->buf;

#if 1
	for(i=TK_CONHEIGHTN1; i>0; i--)
	{
		i0=((i+0)*TK_CONWIDTH)*8;
		i1=((i-1)*TK_CONWIDTH)*8;
		for(j=0; j<TK_CONWIDTH; j++)
		{
			q0=((u64 *)(buf+i1))[0];
			q1=((u64 *)(buf+i1))[1];
			((u64 *)(buf+i0))[0]=q0;
			((u64 *)(buf+i0))[1]=q1;
			i0+=8;
			i1+=8;
		}
	}

	q1=0;
	q0=(2ULL<<30)|(tk_con->bgclr_555<<15)|tk_con->fgclr_555;

	i0=0;
	i1=tk_con->text_attr;
	for(j=0; j<TK_CONWIDTH; j++)
//		{ *(u64 *)(buf+i0)=0; i0+=8; }
	{
//		*(u64 *)(buf+i0)=i1; i0+=8;
		((u64 *)(buf+i0))[0]=q0;
		((u64 *)(buf+i0))[1]=q1;
		i0+=8;
	}
#endif
}

void tk_con_newline()
{
//	if(!tk_con)
//		tk_con_init();
	
	if(!tk_con)
		return;
	
//	tk_con->x=0;
	tk_con->y++;
	if(tk_con->y>=25)
	{
		tk_con_scroll_up();
		tk_con->y--;
	}
}

static tk_con_isesc=0;
static tk_con_escval0;
static tk_con_escval1;
static tk_con_escval2;
static tk_con_escval3;

void TK_Con_PutcEscapeSgr(int mode)
{
	switch(mode)
	{
	case 0:
		tk_con->text_attr=tk_con->text_attr_dfl;
		break;
	case 1:
		tk_con->text_attr|=0x003F0000;
		break;
	case 2:
		break;
	case 3:
	case 7:
		tk_con->text_attr|=0x00002000;
		break;
	case 4:
		tk_con->text_attr|=0x00000800;
		break;
	case 5:
		tk_con->text_attr&=~0x0000C000;
		tk_con->text_attr|= 0x00008000;
		break;
	case 6:
		tk_con->text_attr&=~0x0000C000;
		tk_con->text_attr|= 0x00004000;
		break;
	case 9:
		tk_con->text_attr|=0x00000400;
		break;
	case 10:
		tk_con->text_attr&=~0x00000300;
		break;
	
	case 22:
		tk_con->text_attr=
			(tk_con->text_attr&(~0x003F0000))|
			(tk_con->text_attr_dfl&0x003F0000);
		break;
	case 23:
		tk_con->text_attr&=~0x00002000;
		break;
	case 24:
		tk_con->text_attr&=~0x00000800;
		break;
	case 25:
		tk_con->text_attr&=~0x0000C000;
		break;
	case 29:
		tk_con->text_attr&=~0x00000400;
		break;
	
	case 30:	case 31:
	case 32:	case 33:
	case 34:	case 35:
	case 36:	case 37:
		TK_Con_SetColorFg(tk_con_clr16to64[mode-30]);
		break;
	case 39:
		TK_Con_SetColorFg(0x2A);
		break;		
	case 40:	case 41:
	case 42:	case 43:
	case 44:	case 45:
	case 46:	case 47:
		TK_Con_SetColorBg(tk_con_clr16to64[mode-40]);
		break;
	case 49:
		TK_Con_SetColorBg(0);
		break;		

	case 90:	case 91:
	case 92:	case 93:
	case 94:	case 95:
	case 96:	case 97:
		TK_Con_SetColorFg(tk_con_clr16to64[8+(mode-90)]);
		break;
	case 100:	case 101:
	case 102:	case 103:
	case 104:	case 105:
	case 106:	case 107:
		TK_Con_SetColorBg(tk_con_clr16to64[8+(mode-100)]);
		break;
	}
}

void TK_Con_PutcEscape(int ch)
{
	int i, j, k;

	if(tk_con_isesc==1)
	{
		if(ch=='[')
		{
			tk_con_escval3=0;
			tk_con_escval2=0;
			tk_con_escval1=0;
			tk_con_escval0=0;
			tk_con_isesc=2;
			return;
		}
		
		tk_con_isesc=0;
		return;
	}

	if(tk_con_isesc==2)
	{
		switch(ch)
		{
		case '0':	case '1':
		case '2':	case '3':
		case '4':	case '5':
		case '6':	case '7':
		case '8':	case '9':
			tk_con_escval0=(tk_con_escval0*10)+(ch-'0');
			break;
		case ';':
			tk_con_escval3=tk_con_escval2;
			tk_con_escval2=tk_con_escval1;
			tk_con_escval1=tk_con_escval0;
			tk_con_escval0=0;
			break;
		case 'A':
			i=tk_con_escval0;
			if(!i)i=1;
			TK_Con_SetCursorPos(tk_con->x, tk_con->y-i);
			tk_con_isesc=0;
			break;
		case 'B':
			i=tk_con_escval0;
			if(!i)i=1;
			TK_Con_SetCursorPos(tk_con->x, tk_con->y+i);
			tk_con_isesc=0;
			break;
		case 'C':
			i=tk_con_escval0;
			if(!i)i=1;
			TK_Con_SetCursorPos(tk_con->x+i, tk_con->y);
			tk_con_isesc=0;
			break;
		case 'D':
			i=tk_con_escval0;
			if(!i)i=1;
			TK_Con_SetCursorPos(tk_con->x-i, tk_con->y);
			tk_con_isesc=0;
			break;

		case 'E':
			i=tk_con_escval0;
			if(!i)i=1;
			TK_Con_SetCursorPos(0, tk_con->y+i);
			tk_con_isesc=0;
			break;
		case 'F':
			i=tk_con_escval0;
			if(!i)i=1;
			TK_Con_SetCursorPos(0, tk_con->y-i);
			tk_con_isesc=0;
			break;
		case 'G':
			i=tk_con_escval0;
			if(!i)i=1;
			TK_Con_SetCursorPos(i-1, tk_con->y);
			tk_con_isesc=0;
			break;
		case 'H':
		case 'f':
			i=tk_con_escval0;
			j=tk_con_escval1;
			if(!i)i=1;
//			if(!j)j=1;
			if(!j) { j=i; i=1; }
			TK_Con_SetCursorPos(i-1, j-1);
			tk_con_isesc=0;
			break;

		case 'J':
			switch(tk_con_escval0)
			{
			case 2:
			case 3:
				tk_con_clear();
				break;
			}
			tk_con_isesc=0;
			break;

		case 'S':
			i=tk_con_escval0;
			if(!i)i=1;
			while(i--)
				{ tk_con_scroll_up(); }
			tk_con_isesc=0;
			break;
		case 'T':
			i=tk_con_escval0;
			if(!i)i=1;
			while(i--)
				{ tk_con_scroll_down(); }
			tk_con_isesc=0;
			break;

		case 'm':
			TK_Con_PutcEscapeSgr(tk_con_escval0);
			tk_con_isesc=0;
			break;

		default:
			tk_con_isesc=0;
			break;
		}
		return;
	}

	tk_con_isesc=0;
}

void tk_con_putc(int ch)
{
	int tx, ty, tz;
	u64 q0, q1;
	u32 px, py;
	
	if(!tk_con)
		tk_con_init();
	
	if(!tk_con->ena)
		return;
	
	if(tk_con_isesc)
	{
		TK_Con_PutcEscape(ch);
		return;
	}

	if(ch<' ')
	{
		if(ch=='\b')
		{
			if(tk_con->x>0)tk_con->x--;

#if 0
			px=0x003F8000|'_';
			py=0x003F0000|' ';
			
			ty=tk_con->y;
			tx=tk_con->x;
			tz=(ty*TK_CONWIDTH+tx)*8;
			tk_con->buf[tz+0]=px;
			tk_con->buf[tz+8]=py;
#endif

//			py=((tk_con->y)<<8)|(tk_con->x);
//			((u32 *)0xF00BFF00)[1]=py;		//move hardware cursor
			TK_Con_UpdateHwCursor();

			return;
		}

		if(ch=='\r')
		{
			tk_con->x=0;
//			py=((tk_con->y)<<8)|(tk_con->x);
//			((u32 *)0xF00BFF00)[1]=py;		//move hardware cursor
			TK_Con_UpdateHwCursor();
			return;
		}
		if(ch=='\n')
		{
			tk_con_newline();

//			py=((tk_con->y)<<8)|(tk_con->x);
//			((u32 *)0xF00BFF00)[1]=py;		//move hardware cursor
			TK_Con_UpdateHwCursor();
			return;
		}
		if(ch=='\t')
		{
			tk_con->x=(tk_con->x+8)&(~7);
			if(tk_con->x>=TK_CONWIDTH)
				{ tk_con_newline(); tk_con->x=0; }

//			py=((tk_con->y)<<8)|(tk_con->x);
//			((u32 *)0xF00BFF00)[1]=py;		//move hardware cursor
			TK_Con_UpdateHwCursor();
			return;
		}

		if(ch=='\x1B')
		{
			tk_con_isesc=1;
			return;
		}

		if(ch=='\x1f')
		{
#if 1
			px=0x003F8000|'_';
			ty=tk_con->y;
			tx=tk_con->x;
			tz=(ty*TK_CONWIDTH+tx)*8;
			tk_con->buf[tz+0]=px;
			tx++;
			tk_con->x=tx;
#endif
			return;
		}

		return;
	}
	
//	q1=0x55AA55AAULL;

//	px=0x0FC00000|ch;
//	px=0x003F0000|ch;
	px=(tk_con->text_attr)|ch;
//	px=ch|((tk_con->fgclr)<<16)|((tk_con->bgclr)<<22);
//	py=0x003F8000|'_';
	
	ty=tk_con->y;
	tx=tk_con->x;
//	tz=(ty*TK_CONWIDTH+tx)*8;
//	tk_con->buf[tz]=px;

	q1=tk_gfxcon_glyphs[ch];
	q0=(2ULL<<30)|(tk_con->bgclr_555<<15)|tk_con->fgclr_555;

	tz=(ty*TK_CONWIDTH+tx)*4;
	((u64 *)(tk_con->buf))[tz+0]=q0;
	((u64 *)(tk_con->buf))[tz+1]=q1;

//	tk_con->buf[tz+8]=py;

//	__debugbreak();
	
	tx++;
	tk_con->x=tx;
//	if(tk_con->x>=TK_CONWIDTH)
	if(tx>=TK_CONWIDTH)
	{
		tk_con_newline();
		tk_con->x=0;
	}

//	py=((tk_con->y)<<8)|(tk_con->x);
//	((u32 *)0xF00BFF00)[1]=py;		//move hardware cursor
	TK_Con_UpdateHwCursor();
}
