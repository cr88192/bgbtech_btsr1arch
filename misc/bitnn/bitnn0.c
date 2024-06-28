#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <time.h>


#ifndef BTIC2F_BYTE
#define BTIC2F_BYTE
typedef unsigned char			byte;
typedef unsigned short		u16;
typedef unsigned int			u32;
typedef unsigned long long	u64;

typedef signed char			sbyte;
typedef signed short			s16;
typedef signed int			s32;
typedef signed long long		s64;
#endif

#include "bt1h_targa.c"

byte *bitnn_multtab0;
byte *bitnn_multtab1;

u16 bitnn_digits3x5a[32]={
	075557, 011111, 061247, 061616, 055711,
	074616, 064656, 071122, 075757, 075711,
};

u64 tk_gfxcon_glyphs[128]=
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

#if 1
0x0000000000000000, /* 20 ' ' */
0x0010101010001000, /* 21 '!' */
0x0044444400000000, /* 22 '"' */
0x0044FE4444FE4400, /* 23 '#' */
0x103C403804781000, /* 24 '$' */
0x0044481024440000, /* 25 '%' */
0x18241C384F443A00, /* 26 '&' */
0x0010101000000000, /* 27 ''' */
0x0008101010100800, /* 28 '(' */
0x0020101010102000, /* 29 ')' */
0x004438FE38440000, /* 2A '*' */
0x0010107C10100000, /* 2B '+' */
0x0000000000101020, /* 2C ',' */
0x0000007E00000000, /* 2D '-' */
0x0000000000101000, /* 2E '.' */
0x0002040810204000, /* 2F '/' */
0x0038444C74443800, /* 30 '0' */
0x0010301010107C00, /* 31 '1' */
0x0038440810207C00, /* 32 '2' */
0x007C081008443800, /* 33 '3' */
0x00081828487C0800, /* 34 '4' */
0x007C407804443800, /* 35 '5' */
0x003C407844443800, /* 36 '6' */
0x007E040810202000, /* 37 '7' */
0x0038443844443800, /* 38 '8' */
0x0038443C04083000, /* 39 '9' */
0x0000101000101000, /* 3A ':' */
0x0000101000101020, /* 3B ';' */
0x0408102010080400, /* 3C '<' */
0x00007E00007E0000, /* 3D '=' */
0x4020100810204000, /* 3E '>' */
0x0038440810001000, /* 3F '?' */
0x00384C545C403C00, /* 40 '@' */
0x001028447C444400, /* 41 'A' */
0x0078447844447800, /* 42 'B' */
0x0038444040443800, /* 43 'C' */
0x0070484444487000, /* 44 'D' */
0x007C407840407C00, /* 45 'E' */
0x007C407840404000, /* 46 'F' */
0x003844404E443C00, /* 47 'G' */
0x0044447C44444400, /* 48 'H' */
0x0038101010103800, /* 49 'I' */
0x0004040404443800, /* 4A 'J' */
0x0044487048444400, /* 4B 'K' */
0x0040404040407C00, /* 4C 'L' */
0x0042665A5A424200, /* 4D 'M' */
0x00446454544C4400, /* 4E 'N' */
0x0038444444443800, /* 4F 'O' */
0x0078444478404000, /* 50 'P' */
0x0038444444483400, /* 51 'Q' */
0x0078444478484400, /* 52 'R' */
0x003C403804047800, /* 53 'S' */
0x007C101010101000, /* 54 'T' */
0x0044444444443800, /* 55 'U' */
0x0044444444281000, /* 56 'V' */
0x0042425A5A664200, /* 57 'W' */
0x0044281010284400, /* 58 'X' */
0x0044442810101000, /* 59 'Y' */
0x007C081020407C00, /* 5A 'Z' */
0x001E101010101E00, /* 5B '[' */
0x0040201008040200, /* 5C '\' */
0x0078080808087800, /* 5D ']' */
0x0008142200000000, /* 5E '^' */
0x000000000000FF00, /* 5F '_' */
0x0020201000000000, /* 60 '`' */
0x000038043C443E00, /* 61 'a' */
0x0040407844447800, /* 62 'b' */
0x0000384440443800, /* 63 'c' */
0x0004043C44443C00, /* 64 'd' */
0x000038447C403C00, /* 65 'e' */
0x000E103E10101000, /* 66 'f' */
0x00003844443C0478, /* 67 'g' */
0x0040407844444400, /* 68 'h' */
0x0010003010103800, /* 69 'i' */
0x0004000404040438, /* 6A 'j' */
0x0040444870484400, /* 6B 'k' */
0x0030101010103800, /* 6C 'l' */
0x00007C5252524200, /* 6D 'm' */
0x0000784444444400, /* 6E 'n' */
0x0000384444443800, /* 6F 'o' */
0x0000784444784040, /* 70 'p' */
0x00003C44443C0404, /* 71 'q' */
0x00406E7040404000, /* 72 'r' */
0x00003C4038047800, /* 73 's' */
0x00107E1010100E00, /* 74 't' */
0x0000444444443800, /* 75 'u' */
0x0000444444281000, /* 76 'v' */
0x00004252523C2400, /* 77 'w' */
0x0000442810284400, /* 78 'x' */
0x0000444444380870, /* 79 'y' */
0x00007C0810207C00, /* 7A 'z' */
0x000C106010100C00, /* 7B '{' */
0x0010100010101000, /* 7C '|' */
0x0060100C10106000, /* 7D '}' */
0x0076DC0000000000, /* 7E '~' */
0x000000001824427E  /* 7F ' ' */
#endif

};

u64 tk_gfxcon_glyphs_lat1ext[128]=
{
#if 1
0x003844F040443800, /* 80 ' ' */
0x0000000000000000, /* 81 ' ' */
0x0000000000080800, /* 82 ' ' */
0x000C103810106000, /* 83 ' ' */
0x00000000000A0A00, /* 84 ' ' */
0x0000000000005400, /* 85 ' ' */
0x0010381010101000, /* 86 ' ' */
0x00081C081C080800, /* 87 ' ' */
0x0010284400000000, /* 88 ' ' */
0x0000501020405400, /* 89 ' ' */
0x28103C4038047800, /* 8A ' ' */
0x0000000008100800, /* 8B ' ' */
0x003E505C50503E00, /* 8C ' ' */
0x0000000000000000, /* 8D ' ' */
0x28107C0810207C00, /* 8E ' ' */
0x0000000000000000, /* 8F ' ' */
0x0038444040443800, /* 90 ' ' */
0x0008101800000000, /* 91 ' ' */
0x0018081000000000, /* 92 ' ' */
0x0024486C00000000, /* 93 ' ' */
0x0036122400000000, /* 94 ' ' */
0x0000103810000000, /* 95 ' ' */
0x0000007E00000000, /* 96 ' ' */
0x0000007E00000000, /* 97 ' ' */
0x2850000000000000, /* 98 ' ' */
0x00F15B5551000000, /* 99 ' ' */
0x28103C4038047800, /* 9A ' ' */
0x0000000010081000, /* 9B ' ' */
0x00003C525E503E00, /* 9C ' ' */
0x0000000000000000, /* 9D ' ' */
0x28107C0810207C00, /* 9E ' ' */
0x1400221408080800, /* 9F ' ' */
0x0000000000000000, /* A0 ' ' */
0x0010001010101010, /* A1 ' ' */
0x00081C2A282A1C08, /* A2 ' ' */
0x0008141038103800, /* A3 ' ' */
0x0084784848788400, /* A4 ' ' */
0x0044281038103810, /* A5 ' ' */
0x0000101000101000, /* A6 ' ' */
0x3C24302C340C243C, /* A7 ' ' */
0x2800000000000000, /* A8 ' ' */
0x38449AA2A29A4438, /* A9 ' ' */
0x18041C2418003C00, /* AA ' ' */
0x0028285050282800, /* AB ' ' */
0x000000003C040400, /* AC ' ' */
0x0000000078000000, /* AD ' ' */
0x7C82B2AAB2AA827C, /* AE ' ' */
0x003E000000000000, /* AF ' ' */
0x0010281000000000, /* B0 ' ' */
0x0010381000380000, /* B1 ' ' */
0x00180408101C0000, /* B2 ' ' */
0x0018041C04180000, /* B3 ' ' */
0x0008100000000000, /* B4 ' ' */
0x0000242438202000, /* B5 ' ' */
0x003C747434141400, /* B6 ' ' */
0x0000000010000000, /* B7 ' ' */
0x0000000020102000, /* B8 ' ' */
0x0008180808000000, /* B9 ' ' */
0x0018242418003C00, /* BA ' ' */
0x0050502828505000, /* BB ' ' */
0x4448481517212100, /* BC ' ' */
0x4448481612242600, /* BD ' ' */
0xC448C855D7212100, /* BE ' ' */
0x0008000810221C00, /* BF ' ' */
0x10080018243C2400, /* C0 ' ' */
0x08100018243C2400, /* C1 ' ' */
0x18240018243C2400, /* C2 ' ' */
0x14280018243C2400, /* C3 ' ' */
0x00280018243C2400, /* C4 ' ' */
0x00080018243C2400, /* C5 ' ' */
0x00001C283C282C00, /* C6 ' ' */
0x0018242020241830, /* C7 ' ' */
0x2010003820382038, /* C8 ' ' */
0x0810003820382038, /* C9 ' ' */
0x1028003820382038, /* CA ' ' */
0x0028003820382038, /* CB ' ' */
0x1008003810101038, /* CC ' ' */
0x0810003810101038, /* CD ' ' */
0x1028003810101038, /* CE ' ' */
0x0028003810101038, /* CF ' ' */
0x704844E444487000, /* D0 ' ' */
0x14280024342C2400, /* D1 ' ' */
0x1008182424241800, /* D2 ' ' */
0x0810182424241800, /* D3 ' ' */
0x1824182424241800, /* D4 ' ' */
0x1428182424241800, /* D5 ' ' */
0x2800182424241800, /* D6 ' ' */
0x0000000028102800, /* D7 ' ' */
0x041C242C34243820, /* D8 ' ' */
0x1008002424241800, /* D9 ' ' */
0x0810002424241800, /* DA ' ' */
0x1824002424241800, /* DB ' ' */
0x0028002424241800, /* DC ' ' */
0x0810242418080800, /* DD ' ' */
0x0020382424382000, /* DE ' ' */
0x0038242824282000, /* DF ' ' */
0x100818041C241C00, /* E0 ' ' */
0x081018041C241C00, /* E1 ' ' */
0x182418041C241C00, /* E2 ' ' */
0x142818041C241C00, /* E3 ' ' */
0x280018041C241C00, /* E4 ' ' */
0x080018041C241C00, /* E5 ' ' */
0x00006C127E906E00, /* E6 ' ' */
0x0000182420241830, /* E7 ' ' */
0x10080018243C201C, /* E8 ' ' */
0x08100018243C201C, /* E9 ' ' */
0x18240018243C201C, /* EA ' ' */
0x00280018243C201C, /* EB ' ' */
0x2010003010103800, /* EC ' ' */
0x1020003010103800, /* ED ' ' */
0x1028003010103800, /* EE ' ' */
0x0028003010103800, /* EF ' ' */
0x140814041C241800, /* F0 ' ' */
0x1428001824242400, /* F1 ' ' */
0x1008001824241800, /* F2 ' ' */
0x0810001824241800, /* F3 ' ' */
0x1824001824241800, /* F4 ' ' */
0x1428001824241800, /* F5 ' ' */
0x0028001824241800, /* F6 ' ' */
0x0000100038001000, /* F7 ' ' */
0x0000041C2C343820, /* F8 ' ' */
0x0010080024241800, /* F9 ' ' */
0x0008100024241800, /* FA ' ' */
0x0018240024241800, /* FB ' ' */
0x0000280024241800, /* FC ' ' */
0x0810002424180830, /* FD ' ' */
0x0000181414181000, /* FE ' ' */
0x0050004848301060  /* FF ' ' */
#endif
};

int BitNN_DoMult2x3A(int x, int w)
{
	static signed char xt[4]={0,1,0,-1};
	static signed char wt[8]={0,1,2,3,0,-1,-2,-3};
//	static signed char wt[8]={0,1,2,4,0,-1,-2,-4};
//	static signed char wt[8]={0,1,3,7,0,-1,-3,-7};
	return(xt[x]*wt[w]);
}

int BitNN_Init()
{
	int i, j, k;
	bitnn_multtab0=malloc(256*4096);
	bitnn_multtab1=malloc(16*64);

	for(i=0; i<256; i++)
	{
		for(j=0; j<4096; j++)
		{
			k=	BitNN_DoMult2x3A((i>>0)&3, (j>>0)&7) +
				BitNN_DoMult2x3A((i>>2)&3, (j>>3)&7) +
				BitNN_DoMult2x3A((i>>4)&3, (j>>6)&7) +
				BitNN_DoMult2x3A((i>>6)&3, (j>>9)&7) ;
			bitnn_multtab0[i*4096+j]=k;
		}
	}

	for(i=0; i<16; i++)
	{
		for(j=0; j<64; j++)
		{
			k=	BitNN_DoMult2x3A((i>>0)&3, (j>>0)&7) +
				BitNN_DoMult2x3A((i>>2)&3, (j>>3)&7) ;
			bitnn_multtab1[i*64+j]=k;
		}
	}

	return(0);
}

int BitNN_DoMult(u32 x, u64 wb)
{
#if 0
	int i0, i1, i2, i3;
	int a, b, c;
	
	i0=(((x>> 0)&255)<<12)+((wb>> 0)&4095);
	i1=(((x>> 8)&255)<<12)+((wb>>12)&4095);
	i2=(((x>>16)&255)<<12)+((wb>>24)&4095);
	i3=(((x>>24)&255)<<12)+((wb>>36)&4095);
	a=	bitnn_multtab0[i0] +
		bitnn_multtab0[i1] +
		bitnn_multtab0[i2] +
		bitnn_multtab0[i3] ;
	a=(signed char)a;
	b=(signed char)(wb>>48);
	c=a+b;
	c=(signed char)c;
	return(c>=0);
#endif

#if 1
	int i0, i1, i2, i3, i4, i5, i6, i7;
	int a, b, c;
	
	i0=(((x>> 0)&15)<<6)+((wb>> 0)&63);
	i1=(((x>> 4)&15)<<6)+((wb>> 6)&63);
	i2=(((x>> 8)&15)<<6)+((wb>>12)&63);
	i3=(((x>>12)&15)<<6)+((wb>>18)&63);
	i4=(((x>>16)&15)<<6)+((wb>>24)&63);
	i5=(((x>>20)&15)<<6)+((wb>>30)&63);
	i6=(((x>>24)&15)<<6)+((wb>>36)&63);
	i7=(((x>>28)&15)<<6)+((wb>>42)&63);
	i0=bitnn_multtab1[i0];
	i1=bitnn_multtab1[i1];
	i2=bitnn_multtab1[i2];
	i3=bitnn_multtab1[i3];
	i4=bitnn_multtab1[i4];
	i5=bitnn_multtab1[i5];
	i6=bitnn_multtab1[i6];
	i7=bitnn_multtab1[i7];

	a=i0+i1+i2+i3+i4+i5+i6+i7;
	a=(signed char)a;
	b=(signed char)(wb>>48);
	c=a+b;
	c=(signed char)c;
	return(c>=0);
#endif
}

u64 BitNN_PMortW(u16 va, u16 vb);
u64 BitNN_PMortL(u32 va, u32 vb);

u64 BitNN_DoMultB(u64 d, u32 x, u64 wb)
{
	u64 d1;
	u32 x1;
	int mode;
	int rt;
	
	mode=(wb>>56)&3;
	x1=BitNN_PMortW((~x)&0xFFFF, 0xFFFF);

	if(mode==3)
	{
		rt=BitNN_DoMult(x, wb);
		d1=(d<<2)|((!rt)<<1)|1;
		return(d1);
	}
	
	if(mode==0)
	{
		rt=BitNN_DoMult(x1, wb);
		d1=(d<<1)|rt;
		return(d1);
	}

	if(mode==1)
	{
		rt=BitNN_DoMult(x, wb);
		d1=(d<<1)|rt;
		return(d1);
	}

	if(mode==2)
	{
		rt=BitNN_DoMult(x1, wb);
		d1=(d<<2)|((!rt)<<1)|1;
		return(d1);
	}

	return(0);
}

u16 BitNN_PMortB(byte va, byte vb)
{
	static u16 pmortab[256]={
		0x0000, 0x0001, 0x0004, 0x0005,0x0010, 0x0011, 0x0014, 0x0015,
		0x0040, 0x0041, 0x0044, 0x0045,0x0050, 0x0051, 0x0054, 0x0055,
		0x0100, 0x0101, 0x0104, 0x0105,0x0110, 0x0111, 0x0114, 0x0115,
		0x0140, 0x0141, 0x0144, 0x0145,0x0150, 0x0151, 0x0154, 0x0155,
		0x0400, 0x0401, 0x0404, 0x0405,0x0410, 0x0411, 0x0414, 0x0415,
		0x0440, 0x0441, 0x0444, 0x0445,0x0450, 0x0451, 0x0454, 0x0455,
		0x0500, 0x0501, 0x0504, 0x0505,0x0510, 0x0511, 0x0514, 0x0515,
		0x0540, 0x0541, 0x0544, 0x0545,0x0550, 0x0551, 0x0554, 0x0555,
		0x1000, 0x1001, 0x1004, 0x1005,0x1010, 0x1011, 0x1014, 0x1015,
		0x1040, 0x1041, 0x1044, 0x1045,0x1050, 0x1051, 0x1054, 0x1055,
		0x1100, 0x1101, 0x1104, 0x1105,0x1110, 0x1111, 0x1114, 0x1115,
		0x1140, 0x1141, 0x1144, 0x1145,0x1150, 0x1151, 0x1154, 0x1155,
		0x1400, 0x1401, 0x1404, 0x1405,0x1410, 0x1411, 0x1414, 0x1415,
		0x1440, 0x1441, 0x1444, 0x1445,0x1450, 0x1451, 0x1454, 0x1455,
		0x1500, 0x1501, 0x1504, 0x1505,0x1510, 0x1511, 0x1514, 0x1515,
		0x1540, 0x1541, 0x1544, 0x1545,0x1550, 0x1551, 0x1554, 0x1555,
		0x4000, 0x4001, 0x4004, 0x4005,0x4010, 0x4011, 0x4014, 0x4015,
		0x4040, 0x4041, 0x4044, 0x4045,0x4050, 0x4051, 0x4054, 0x4055,
		0x4100, 0x4101, 0x4104, 0x4105,0x4110, 0x4111, 0x4114, 0x4115,
		0x4140, 0x4141, 0x4144, 0x4145,0x4150, 0x4151, 0x4154, 0x4155,
		0x4400, 0x4401, 0x4404, 0x4405,0x4410, 0x4411, 0x4414, 0x4415,
		0x4440, 0x4441, 0x4444, 0x4445,0x4450, 0x4451, 0x4454, 0x4455,
		0x4500, 0x4501, 0x4504, 0x4505,0x4510, 0x4511, 0x4514, 0x4515,
		0x4540, 0x4541, 0x4544, 0x4545,0x4550, 0x4551, 0x4554, 0x4555,
		0x5000, 0x5001, 0x5004, 0x5005,0x5010, 0x5011, 0x5014, 0x5015,
		0x5040, 0x5041, 0x5044, 0x5045,0x5050, 0x5051, 0x5054, 0x5055,
		0x5100, 0x5101, 0x5104, 0x5105,0x5110, 0x5111, 0x5114, 0x5115,
		0x5140, 0x5141, 0x5144, 0x5145,0x5150, 0x5151, 0x5154, 0x5155,
		0x5400, 0x5401, 0x5404, 0x5405,0x5410, 0x5411, 0x5414, 0x5415,
		0x5440, 0x5441, 0x5444, 0x5445,0x5450, 0x5451, 0x5454, 0x5455,
		0x5500, 0x5501, 0x5504, 0x5505,0x5510, 0x5511, 0x5514, 0x5515,
		0x5540, 0x5541, 0x5544, 0x5545,0x5550, 0x5551, 0x5554, 0x5555,
	};
	return(pmortab[vb]|(pmortab[va]<<1));
}

u64 BitNN_PMortW(u16 va, u16 vb)
{
	u32 w0, w1;
	w0=BitNN_PMortB((va>>0)&255, (vb>>0)&255);
	w1=BitNN_PMortB((va>>8)&255, (vb>>8)&255);
	return(w0|(w1<<16));
}

u64 BitNN_PMortL(u32 va, u32 vb)
{
	u64 w0, w1;
	w0=BitNN_PMortW((va>> 0)&65535, (vb>> 0)&65535);
	w1=BitNN_PMortW((va>>16)&65535, (vb>>16)&65535);
	return(w0|(w1<<32));
}

u64 BitNN_Random64()
{
	u64 v;
	int j;

	v=rand();
	for(j=0; j<16; j++)
		v=v*65521+rand()+(v>>48);
	return(v);
}

u64 BitNN_Random56()
{
	u64 v;
	v=BitNN_Random64();
	v&=~(0xFFULL<<56);
	return(v);
}

// #define GENSZ	64
#define GENSZ	128

// #define BITNN_STAGE0SZ	64
#define BITNN_STAGE0SZ	128

// #define BITNN_STAGE1SZ	16
// #define BITNN_STAGE1SZ	32
#define BITNN_STAGE1SZ	64

// #define BITNN_STAGE2SZ	32

#ifdef BITNN_STAGE2SZ
#define BITNN_STAGE1FSZ	BITNN_STAGE2SZ
#else
#ifdef BITNN_STAGE1SZ
#define BITNN_STAGE1FSZ	BITNN_STAGE1SZ
#else
#define BITNN_STAGE1FSZ	BITNN_STAGE0SZ
#endif
#endif

typedef struct {
	u64 nl0[BITNN_STAGE0SZ];
	u64 nl0a[BITNN_STAGE0SZ];
	u64 nl0b[BITNN_STAGE0SZ];
	u64 nl0c[BITNN_STAGE0SZ];

	u64 nlf[10];
	u64 nlfa[10];
	u64 nlfb[10];
	u64 nlfc[10];

#ifdef BITNN_STAGE1SZ
	u64 nl1[BITNN_STAGE1SZ];
	u64 nl1a[BITNN_STAGE1SZ];
	u64 nl1b[BITNN_STAGE1SZ];
	u64 nl1c[BITNN_STAGE1SZ];
#endif

#ifdef BITNN_STAGE2SZ
	u64 nl2[BITNN_STAGE2SZ];
	u64 nl2a[BITNN_STAGE2SZ];
	u64 nl2b[BITNN_STAGE2SZ];
	u64 nl2c[BITNN_STAGE2SZ];
#endif
}bitnn_pat;

bitnn_pat bitnn_generate()
{
	bitnn_pat tmp;
	int i;
	
//	tmp.nl0[0]=BitNN_Random64();
//	tmp.nl0[1]=BitNN_Random64();
//	tmp.nl0[2]=BitNN_Random64();
//	tmp.nl0[3]=BitNN_Random64();

	for(i=0; i<BITNN_STAGE0SZ; i++)
	{
		tmp.nl0[i]=BitNN_Random56();
		tmp.nl0a[i]=BitNN_Random56();
		tmp.nl0b[i]=BitNN_Random56();
		tmp.nl0c[i]=BitNN_Random56();
	}

#ifdef BITNN_STAGE1SZ
	for(i=0; i<BITNN_STAGE1SZ; i++)
	{
//		tmp.nl1[i]=BitNN_Random56();
//		tmp.nl1a[i]=BitNN_Random56();
//		tmp.nl1b[i]=BitNN_Random56();
//		tmp.nl1c[i]=BitNN_Random56();

		tmp.nl1[i]=3ULL<<((i&15)*3);
		tmp.nl1a[i]=tmp.nl1[i];
		tmp.nl1b[i]=tmp.nl1[i];
		tmp.nl1c[i]=tmp.nl1[i];
	}
#endif

#ifdef BITNN_STAGE2SZ
	for(i=0; i<BITNN_STAGE2SZ; i++)
	{
//		tmp.nl2[i]=BitNN_Random56();
//		tmp.nl2a[i]=BitNN_Random56();
//		tmp.nl2b[i]=BitNN_Random56();
//		tmp.nl2c[i]=BitNN_Random56();

		tmp.nl2[i]=3ULL<<((i&15)*3);
		tmp.nl2a[i]=tmp.nl2[i];
		tmp.nl2b[i]=tmp.nl2[i];
		tmp.nl2c[i]=tmp.nl2[i];
	}
#endif

	for(i=0; i<10; i++)
	{
		tmp.nlf[i]=BitNN_Random56();
		tmp.nlfa[i]=BitNN_Random56();
		tmp.nlfb[i]=BitNN_Random56();
		tmp.nlfc[i]=BitNN_Random56();
	}

//	tmp.nl1=BitNN_Random64();
	return(tmp);
}

u64 bitnn_mutatevec(u64 v1, u64 v2, int rank)
{
	u64 v;
	int i, j, n;
	
//	i=rand()&63;
	i=rank;
	v=BitNN_Random64();
	v=(v1&v)|(v2&(~v));
//	v^=1ULL<<(rand()&63);

//	n=i>>4;
	n=i>>5;
	
//	if(rand()&15)
	if(rand()&63)
	{
		n=0;
//		n--;
//		if(n<0)
//			n=0;
	}
	
//	n=i&3;
	for(j=0; j<n; j++)
	{
		v^=1ULL<<(rand()&63);
//		v^=1ULL<<(rand()&63);
	}

#if 0
	if(i>16)
		v^=1ULL<<(rand()&63);
	if(i>32)
	{
		v^=1ULL<<(rand()&63);
		v^=1ULL<<(rand()&63);
	}
	if(i>48)
	{
		v^=1ULL<<(rand()&63);
		v^=1ULL<<(rand()&63);
	}
#endif
	
//	v&=~(0xFCULL<<56);
	v&=~(0xFFULL<<56);
	
	return(v);
}

u64 bitnn_combinevec(u64 va, u64 vb, u64 vc)
{
	u64 vd;
	
	vd=(va&vb)|(vb&vc)|(va&vc);
//	vd^=(vd<<1)&0x00FEDB6DB6DB6DB6ULL;
//	vd^=(vd>>1)&0x492492492492ULL;
//	vd^=(vd>>1)&0x249249249249ULL;
	vd^=(vd>>1)&0x007F249249249249ULL;

#if 0
	vd=vd^
//		((vd>>1)&0x492492492492ULL) ^
//		((vd>>2)&0x249249249249ULL) ;
		((vd>>1)&0x6DB6DB6DB6DBULL) ^
		((vd>>2)&0x249249249249ULL) ;
#endif
	return(vd);
}

bitnn_pat bitnn_mutate(bitnn_pat p1, bitnn_pat p2, int rank)
{
	bitnn_pat tmp;
	u64 x0, x1, x2, x3, x4, x5;
	int i;
	
//	tmp.nl0[0]=bitnn_mutatevec(p1.nl0[0], p2.nl0[0]);
//	tmp.nl0[1]=bitnn_mutatevec(p1.nl0[1], p2.nl0[1]);
//	tmp.nl0[2]=bitnn_mutatevec(p1.nl0[2], p2.nl0[2]);
//	tmp.nl0[3]=bitnn_mutatevec(p1.nl0[3], p2.nl0[3]);

	for(i=0; i<BITNN_STAGE0SZ; i++)
	{
//		tmp.nl0[i]=bitnn_mutatevec(p1.nl0[i], p2.nl0[i]);

//		tmp.nl0a[i]=bitnn_mutatevec(p1.nl0a[i], p2.nl0a[i]);
//		tmp.nl0b[i]=bitnn_mutatevec(p1.nl0b[i], p2.nl0b[i]);
//		tmp.nl0c[i]=bitnn_mutatevec(p1.nl0c[i], p2.nl0c[i]);

		x0=bitnn_mutatevec(p1.nl0a[i], p2.nl0a[i], rank);
		x1=bitnn_mutatevec(p1.nl0b[i], p2.nl0b[i], rank);
		x2=bitnn_mutatevec(p1.nl0c[i], p2.nl0c[i], rank);
		x3=bitnn_mutatevec(x0, x1, rank);
		x4=bitnn_mutatevec(x1, x2, rank);
		x5=bitnn_mutatevec(x2, x0, rank);
		tmp.nl0a[i]=x3;
		tmp.nl0b[i]=x4;
		tmp.nl0c[i]=x5;

		tmp.nl0[i]=bitnn_combinevec(tmp.nl0a[i], tmp.nl0b[i], tmp.nl0c[i]);
	}

#ifdef BITNN_STAGE1SZ
//	tmp.nl1=bitnn_mutatevec(p1.nl1, p2.nl1);

	for(i=0; i<BITNN_STAGE1SZ; i++)
	{
//		tmp.nl1[i]=bitnn_mutatevec(p1.nl1[i], p2.nl1[i]);

//		tmp.nl1a[i]=bitnn_mutatevec(p1.nl1a[i], p2.nl1a[i]);
//		tmp.nl1b[i]=bitnn_mutatevec(p1.nl1b[i], p2.nl1b[i]);
//		tmp.nl1c[i]=bitnn_mutatevec(p1.nl1c[i], p2.nl1c[i]);

		x0=bitnn_mutatevec(p1.nl1a[i], p2.nl1a[i], rank);
		x1=bitnn_mutatevec(p1.nl1b[i], p2.nl1b[i], rank);
		x2=bitnn_mutatevec(p1.nl1c[i], p2.nl1c[i], rank);
		x3=bitnn_mutatevec(x0, x1, rank);
		x4=bitnn_mutatevec(x1, x2, rank);
		x5=bitnn_mutatevec(x2, x0, rank);
		tmp.nl1a[i]=x3;
		tmp.nl1b[i]=x4;
		tmp.nl1c[i]=x5;


		tmp.nl1[i]=bitnn_combinevec(tmp.nl1a[i], tmp.nl1b[i], tmp.nl1c[i]);
	}
#endif

#ifdef BITNN_STAGE2SZ
	for(i=0; i<BITNN_STAGE2SZ; i++)
	{
//		tmp.nl2a[i]=bitnn_mutatevec(p1.nl2a[i], p2.nl2a[i]);
//		tmp.nl2b[i]=bitnn_mutatevec(p1.nl2b[i], p2.nl2b[i]);
//		tmp.nl2c[i]=bitnn_mutatevec(p1.nl2c[i], p2.nl2c[i]);

		x0=bitnn_mutatevec(p1.nl2a[i], p2.nl2a[i], rank);
		x1=bitnn_mutatevec(p1.nl2b[i], p2.nl2b[i], rank);
		x2=bitnn_mutatevec(p1.nl2c[i], p2.nl2c[i], rank);
		x3=bitnn_mutatevec(x0, x1, rank);
		x4=bitnn_mutatevec(x1, x2, rank);
		x5=bitnn_mutatevec(x2, x0, rank);
		tmp.nl2a[i]=x3;
		tmp.nl2b[i]=x4;
		tmp.nl2c[i]=x5;

		tmp.nl2[i]=bitnn_combinevec(tmp.nl2a[i], tmp.nl2b[i], tmp.nl2c[i]);
	}
#endif

	for(i=0; i<10; i++)
	{
//		tmp.nlf[i]=bitnn_mutatevec(p1.nlf[i], p2.nlf[i]);

//		tmp.nlfa[i]=bitnn_mutatevec(p1.nlfa[i], p2.nlfa[i]);
//		tmp.nlfb[i]=bitnn_mutatevec(p1.nlfb[i], p2.nlfb[i]);
//		tmp.nlfc[i]=bitnn_mutatevec(p1.nlfc[i], p2.nlfc[i]);

		x0=bitnn_mutatevec(p1.nlfa[i], p2.nlfa[i], rank);
		x1=bitnn_mutatevec(p1.nlfb[i], p2.nlfb[i], rank);
		x2=bitnn_mutatevec(p1.nlfc[i], p2.nlfc[i], rank);
		x3=bitnn_mutatevec(x0, x1, rank);
		x4=bitnn_mutatevec(x1, x2, rank);
		x5=bitnn_mutatevec(x2, x0, rank);
		tmp.nlfa[i]=x3;
		tmp.nlfb[i]=x4;
		tmp.nlfc[i]=x5;

		tmp.nlf[i]=bitnn_combinevec(tmp.nlfa[i], tmp.nlfb[i], tmp.nlfc[i]);
	}

	return(tmp);
}

int bitnn_eval(u32 db, bitnn_pat pat)
{
	int j;
	j=0;
//	j=(j<<2)|(BitNN_DoMult(db, pat.nl0[0])<<1)|1;
//	j=(j<<2)|(BitNN_DoMult(db, pat.nl0[1])<<1)|1;
//	j=(j<<2)|(BitNN_DoMult(db, pat.nl0[2])<<1)|1;
//	j=(j<<2)|(BitNN_DoMult(db, pat.nl0[3])<<1)|1;
//	j=BitNN_DoMult(j, pat.nl1);

	j=BitNN_DoMultB(0, db, pat.nl0[0]);
	j=BitNN_DoMultB(j, db, pat.nl0[1]);
	j=BitNN_DoMultB(j, db, pat.nl0[2]);
	j=BitNN_DoMultB(j, db, pat.nl0[3]);
	j=BitNN_DoMultB(0, j, pat.nlf[0]);

	return(j);
}

int bitnn_eval8x8x1(u64 pix, bitnn_pat pat)
{
	u16 dbx[32];
	u32 dba[16];
	u64 j0, j1, j2, j3, j0b, j1b;
	int i, j, k;

	j0=BitNN_PMortL(pix>>32, pix);
	j1=BitNN_PMortL(j0>>32, j0);

	dbx[0]=j1>> 0;
	dbx[1]=j1>> 8;
	dbx[2]=j1>>16;
	dbx[3]=j1>>24;
	dbx[4]=j1>>32;
	dbx[5]=j1>>40;
	dbx[6]=j1>>48;
	dbx[7]=(j1>>56) | (j1<<8);

	for(i=0; i<8; i++)
		dbx[i+8]=dbx[i];

#if 0

	dbx[ 0]=BitNN_PMortB((pix>>8)&255, (pix>>0)&255);
	dbx[ 1]=BitNN_PMortB((pix>>16)&255, (pix>>8)&255);
	dbx[ 2]=BitNN_PMortB((pix>>24)&255, (pix>>16)&255);
	dbx[ 3]=BitNN_PMortB((pix>>32)&255, (pix>>24)&255);
	dbx[ 4]=BitNN_PMortB((pix>>40)&255, (pix>>32)&255);
	dbx[ 5]=BitNN_PMortB((pix>>48)&255, (pix>>40)&255);
	dbx[ 6]=BitNN_PMortB((pix>>56)&255, (pix>>48)&255);

	dbx[ 8]=BitNN_PMortB((dbx[0]>>0)&255, (dbx[1]>>0)&255);
	dbx[ 9]=BitNN_PMortB((dbx[2]>>0)&255, (dbx[3]>>0)&255);
	dbx[10]=BitNN_PMortB((dbx[4]>>0)&255, (dbx[5]>>0)&255);
	dbx[11]=BitNN_PMortB((dbx[6]>>0)&255, (dbx[0]>>0)&255);
	dbx[12]=BitNN_PMortB((dbx[0]>>8)&255, (dbx[1]>>8)&255);
	dbx[13]=BitNN_PMortB((dbx[2]>>8)&255, (dbx[3]>>8)&255);
	dbx[14]=BitNN_PMortB((dbx[4]>>8)&255, (dbx[5]>>8)&255);
	dbx[15]=BitNN_PMortB((dbx[6]>>8)&255, (dbx[0]>>8)&255);

	dbx[ 7]=BitNN_PMortB((dbx[9]>>0)&255, (dbx[14]>>0)&255);

#if (BITNN_STAGE0SZ>=32)

	dbx[16]=BitNN_PMortB((dbx[0]>>0)&255, (dbx[1]>>0)&255);
	dbx[17]=BitNN_PMortB((dbx[1]>>0)&255, (dbx[2]>>0)&255);
	dbx[18]=BitNN_PMortB((dbx[2]>>0)&255, (dbx[3]>>0)&255);
	dbx[19]=BitNN_PMortB((dbx[3]>>0)&255, (dbx[4]>>0)&255);
	dbx[20]=BitNN_PMortB((dbx[4]>>0)&255, (dbx[5]>>0)&255);
	dbx[21]=BitNN_PMortB((dbx[5]>>0)&255, (dbx[6]>>0)&255);
	dbx[22]=BitNN_PMortB((dbx[6]>>0)&255, (dbx[0]>>0)&255);

	dbx[24]=BitNN_PMortB((dbx[0]>>8)&255, (dbx[1]>>8)&255);
	dbx[25]=BitNN_PMortB((dbx[1]>>8)&255, (dbx[2]>>8)&255);
	dbx[26]=BitNN_PMortB((dbx[2]>>8)&255, (dbx[3]>>8)&255);
	dbx[27]=BitNN_PMortB((dbx[3]>>8)&255, (dbx[4]>>8)&255);
	dbx[28]=BitNN_PMortB((dbx[4]>>8)&255, (dbx[5]>>8)&255);
	dbx[29]=BitNN_PMortB((dbx[5]>>8)&255, (dbx[6]>>8)&255);
	dbx[30]=BitNN_PMortB((dbx[6]>>8)&255, (dbx[0]>>8)&255);

	dbx[23]=BitNN_PMortB((dbx[9]>>4)&255, (dbx[10]>>4)&255);
	dbx[31]=BitNN_PMortB((dbx[13]>>4)&255, (dbx[14]>>4)&255);
#endif

#endif

//	for(i=0; i<16; i++)
//		dba[i]=BitNN_PMortW(dbx[i], -1);

//	j=0;
//	j=(j<<2)|(BitNN_DoMult(db, pat.nl0[0])<<1)|1;
//	j=(j<<2)|(BitNN_DoMult(db, pat.nl0[1])<<1)|1;
//	j=(j<<2)|(BitNN_DoMult(db, pat.nl0[2])<<1)|1;
//	j=(j<<2)|(BitNN_DoMult(db, pat.nl0[3])<<1)|1;

#if 0
	j0=0;
	for(i=0; i<32; i++)
	{
//		j=(j<<2)|(BitNN_DoMult(dba[i], pat.nl0[i])<<1)|1;
		j0=BitNN_DoMultB(j0, dbx[i&15], pat.nl0[i]);
	}

	j1=0;
	for(i=0; i<32; i++)
	{
		j1=BitNN_DoMultB(j1, j0>>((i&1)*16), pat.nl1[i]);
	}

	k=0;
	for(i=0; i<10; i++)
	{
//		k=BitNN_DoMultB(k, j1, pat.nlf[i]);
		k=BitNN_DoMultB(k, j1>>((i&1)*16), pat.nlf[i]);
	}

	return(k);
#endif

#if 1
	j0=0;
	j0b=0;
	for(i=0; i<BITNN_STAGE0SZ; i++)
//	for(i=0; i<64; i++)
//	for(i=0; i<32; i++)
	{
		j0b=(j0b<<1)|(j0>>63);
		j0=BitNN_DoMultB(j0, dbx[i&15], pat.nl0[i]);
//		j0=BitNN_DoMultB(j0, dbx[i&31], pat.nl0[i]);
//		j0=BitNN_DoMultB(j0, dbx[16+(i&15)], pat.nl0[i]);
//		j0=BitNN_DoMultB(j0, dbx[8+(i&7)], pat.nl0[i]);
//		j0=BitNN_DoMultB(j0, dbx[i&7], pat.nl0[i]);
	}

//	j0=BitNN_PMortL(j0, j0>>32);
	j1=j0;
	j1b=j0b;

#ifdef BITNN_STAGE1SZ
	j1=0;
	j1b=0;

//	for(i=0; i<32; i++)
//	for(i=0; i<16; i++)
	for(i=0; i<BITNN_STAGE1SZ; i++)
	{
		j1b=(j1b<<1)|(j1>>63);

#if (BITNN_STAGE0SZ==128)
		j3=	((j0 >>((i&7)*8))<<0) |
			((j0b>>((i&7)*8))<<8) ;
		j1=BitNN_DoMultB(j1, j3, pat.nl1[i]);
#endif

#if (BITNN_STAGE0SZ==64)

#if (BITNN_STAGE1SZ>=32)
//		j1=BitNN_DoMultB(j1, j0>>((i&3)*16), pat.nl1[i]);
//		j1=BitNN_DoMultB(j1, j0>>(((i>>4)&3)*16), pat.nl1[i]);
		j1=BitNN_DoMultB(j1, j0>>(((i>>3)&3)*16), pat.nl1[i]);
#else
		j1=BitNN_DoMultB(j1, j0>>(((i>>2)&3)*16), pat.nl1[i]);
#endif
#endif

#if (BITNN_STAGE0SZ==32)
		j1=BitNN_DoMultB(j1, j0>>((i&1)*16), pat.nl1[i]);
#endif
#if (BITNN_STAGE0SZ==16)
		j1=BitNN_DoMultB(j1, j0, pat.nl1[i]);
#endif
	}
#endif

#ifdef BITNN_STAGE2SZ
	j2=0;
	for(i=0; i<BITNN_STAGE2SZ; i++)
	{
#if (BITNN_STAGE1SZ==64)
//		j1=BitNN_DoMultB(j2, j1>>((i&3)*16), pat.nl2[i]);
//		j1=BitNN_DoMultB(j2, j1>>(((i>>4)&3)*16), pat.nl2[i]);
		j2=BitNN_DoMultB(j2, j1>>(((i>>3)&3)*16), pat.nl2[i]);
#endif
#if (BITNN_STAGE1SZ==32)
		j2=BitNN_DoMultB(j2, j1>>((i&1)*16), pat.nl2[i]);
#endif
#if (BITNN_STAGE1SZ==16)
		j2=BitNN_DoMultB(j2, j1, pat.nl2[i]);
#endif
	}
	j1=j2;
#endif

//	j1=j0;

	k=0;
	for(i=0; i<10; i++)
	{
#if (BITNN_STAGE1FSZ==128)
		j3=
//			(((j1 >>((i^0)*6))&63)<<0) |
//			(((j1b>>((i^1)*6))&63)<<6) |
			(((j1 >>((i-0)*6))&63)<<0) |
			(((j1b>>((9-i)*6))&63)<<6) |
			((j1>>60)&15)<<12;
		k=BitNN_DoMultB(k, j3, pat.nlf[i]);
#endif

#if (BITNN_STAGE1FSZ==64)
//		k=BitNN_DoMultB(k, j1>>((i&3)*16), pat.nlf[i]);
//		k=BitNN_DoMultB(k, j1>>(((i>>1)&3)*16), pat.nlf[i]);
//		k=BitNN_DoMultB(k, j1>>((i&7)*8), pat.nlf[i]);
		k=BitNN_DoMultB(k, j1>>(i*6), pat.nlf[i]);
//		k=BitNN_DoMultB(k, j1>>(i*5), pat.nlf[i]);
#endif
#if (BITNN_STAGE1FSZ==32)
		k=BitNN_DoMultB(k, j1>>((i&1)*16), pat.nlf[i]);
#endif
#if (BITNN_STAGE1FSZ==16)
		k=BitNN_DoMultB(k, j1, pat.nlf[i]);
#endif
	}

	return(k);
#endif

//	j=BitNN_DoMult(j, pat.nl1);
//	j=BitNN_DoMultB(0, j, pat.nl1);

#if 0
	j0=0;
	for(i=0; i<16; i++)
	{
		j0=BitNN_DoMultB(j0, dbx[i], pat.nl0[i]);
	}

	j1=0;
	for(i=0; i<16; i++)
	{
		j1=BitNN_DoMultB(j1, j0, pat.nl1[i]);
	}

	k=0;
	for(i=0; i<10; i++)
	{
		k=BitNN_DoMultB(k, j1, pat.nlf[i]);
//		k=BitNN_DoMultB(k, j1>>((i&1)*16), pat.nlf[i]);
	}

	return(k);
#endif
}

int bitnn_dumppat(bitnn_pat pat)
{
	int i;

	for(i=0; i<BITNN_STAGE0SZ; i++)
	{
		printf("nl0[%d]=0x%016llXULL;\n", i, pat.nl0[i]);
	}

#ifdef BITNN_STAGE1SZ
	for(i=0; i<BITNN_STAGE1SZ; i++)
	{
		printf("nl1[%d]=0x%016llXULL;\n", i, pat.nl1[i]);
	}
#endif

#ifdef BITNN_STAGE2SZ
	for(i=0; i<BITNN_STAGE2SZ; i++)
	{
		printf("nl2[%d]=0x%016llXULL;\n", i, pat.nl2[i]);
	}
#endif

	for(i=0; i<10; i++)
	{
		printf("nlf[%d]=0x%016llXULL;\n", i, pat.nlf[i]);
	}

//	printf("%016llX\n", pat.nl1);
}

int btnn_popcnt(u32 v)
{
	int i, j;
	
	j=0;
	for(i=0; i<32; i++)
		if(v&(1U<<i))
			j++;
	return(j);
}

int bitnn_selsqrt[256];

int main()
{
//	u64 narr[GENSZ];
	bitnn_pat narr[GENSZ];
	int scarr[GENSZ];
//	u64 nbest[10];

	bitnn_pat nbest[10];
	bitnn_pat pv;

	u64 v;
	u64 db;
	int dig, topdig, gen, maxsc, psh;
	int i0, i1, i2, i3;
	int i, j, k;
	
	BitNN_Init();
	
	for(i=0; i<256; i++)
	{
		bitnn_selsqrt[i]=15-(int)(sqrt(255-i));
//		printf("%d\n", bitnn_selsqrt[i]);
	}
	
	for(i=10; i<32; i++)
	{
		do {
			k=rand();
			for(j=0; j<10; j++)
				if(bitnn_digits3x5a[j]==k)
					break;
		}while(j<10);

		bitnn_digits3x5a[i]=k;
	}

#if 0
	for(topdig=0; topdig<10; topdig++)
	{
		for(i=0; i<GENSZ; i++)
		{
//			narr[i]=BitNN_Random64();
			narr[i]=bitnn_generate();
		}

		for(gen=0; gen<(1<<16); gen++)
		{
			for(i=0; i<GENSZ; i++)
				scarr[i]=0;

			maxsc=0;
			for(dig=0; dig<32; dig++)
			{
//				db=bitnn_digits3x5a[dig];
//				db=BitNN_PMortW(0, db);
				db=tk_gfxcon_glyphs['0'+dig];

				for(i=0; i<GENSZ; i++)
				{
//					j=BitNN_DoMult(db, narr[i]);
//					j=bitnn_eval(db, narr[i]);
					j=bitnn_eval8x8x1(db, narr[i]);

					if(dig==topdig)
					{
						if(!i)
							maxsc+=32;
						if(j)
							scarr[i]+=32;
					}else if(dig<10)
					{
						if(!i)
							maxsc+=8;
						if(!j)
							scarr[i]+=8;
					}else
					{
						if(!i)
							maxsc++;
						if(!j)
							scarr[i]++;
					}

//					scarr[i]+=(dig!=topdig)^j;
				}
			}
			
			for(i=0; i<GENSZ; i++)
				for(j=i+1; j<GENSZ; j++)
			{
				if(gen<(1<<11))
				{
					if((i&3)!=(j&3))
						continue;
				}

				if(gen<(1<<13))
				{
					if((i&1)!=(j&1))
						continue;
				}

				if(scarr[j]>scarr[i])
				{
					pv=narr[j];
					narr[j]=narr[i];
					narr[i]=pv;

					v=scarr[j];
					scarr[j]=scarr[i];
					scarr[i]=v;
				}
			}

			for(i=16; i<GENSZ; i++)
			{
				if(gen<(1<<11))
				{
					if(i<32)
						continue;
				
					i0=rand()&31;
					i1=rand()&31;
					i0=(i0&(~3))|(i&3);
					i1=(i1&(~3))|(i&3);
				}else
					if(gen<(1<<13))
				{
					if(i<32)
						continue;
				
					i0=rand()&31;
					i1=rand()&31;
					i0=(i0&(~1))|(i&1);
					i1=(i1&(~1))|(i&1);
				}else
				{
					i0=rand()&15;
					i1=rand()&15;
				}

				narr[i]=bitnn_mutate(narr[i0], narr[i1]);

#if 0
				v=BitNN_Random64();
				v=(narr[i0]&v)|(narr[i1]&(~v));
				v^=1ULL<<(rand()&63);
				if(i>24)
					v^=1ULL<<(rand()&63);
				if(i>32)
				{
					v^=1ULL<<(rand()&63);
//					v^=1ULL<<(rand()&63);
				}
				if(i>48)
				{
					v^=1ULL<<(rand()&63);
//					v^=1ULL<<(rand()&63);
				}
				narr[i]=v;
#endif
			}

//			narr[GENSZ-1]=BitNN_Random64();
//			narr[GENSZ-2]=BitNN_Random64();
//			narr[GENSZ-3]=BitNN_Random64();
//			narr[GENSZ-4]=BitNN_Random64();
			
			printf("Gen=%d Bsc=%d/%d (%.2f%%)\r",
				gen, scarr[0], maxsc, (100.0*scarr[0])/maxsc);
			
			if(scarr[0]==maxsc)
				break;
		}
		printf("\n");
		
//		printf("Dig=%d, %016llX\n", topdig, narr[0]);
		nbest[topdig]=narr[0];
	}

	for(topdig=0; topdig<10; topdig++)
	{
		k=0; gen=10;

		for(dig=0; dig<10; dig++)
		{
//			db=bitnn_digits3x5a[dig];
//			db=BitNN_PMortW(0, db);
			db=tk_gfxcon_glyphs['0'+dig];
//			j=BitNN_DoMult(db, nbest[topdig]);
//			j=bitnn_eval(db, nbest[topdig]);
			j=bitnn_eval8x8x1(db, nbest[topdig]);
			
			if(dig==topdig)
			{
				if(!j)
					{ k++; gen=dig; }
			}else
			{
				if(j)
					{ k++; gen=dig; }
			}
		}
		
		printf("Dig=%d, Miss=%d / misdig=%d\n", topdig, k, gen);
		bitnn_dumppat(nbest[topdig]);
	}
#endif



#if 1
	for(i=0; i<GENSZ; i++)
	{
		narr[i]=bitnn_generate();
	}

//	for(gen=0; gen<(1<<20); gen++)
//	for(gen=0; gen<(1<<19); gen++)
//	for(gen=0; gen<(1<<17); gen++)
//	for(gen=0; gen<(1<<16); gen++)
	for(gen=0; gen<(1<<15); gen++)
//	for(gen=0; gen<(1<<14); gen++)
//	for(gen=0; gen<(1<<12); gen++)
	{
		for(i=0; i<GENSZ; i++)
			scarr[i]=0;

 		maxsc=0;
//		for(topdig=0; topdig<10; topdig++)
		if(1)
		{
			for(dig=0; dig<30; dig++)
			{
				db=tk_gfxcon_glyphs['0'+dig];
//				if((dig>=10) && (gen&1))
//					db=tk_gfxcon_glyphs['0'+dig+24];

				if(dig>=10)
					db=tk_gfxcon_glyphs['0'+dig+16*(i&3)];

				for(i=0; i<GENSZ; i++)
				{
					j=bitnn_eval8x8x1(db, narr[i]);
					
#if 0
					if(dig<10)
						j=1<<dig;
					else
						j=0;
#endif
					
//					if(j==0x000)
//						continue;
//					if(j==0x3FF)
//						continue;

					k=btnn_popcnt(j);
					psh=0;
					if(k>2)
						psh=1;
					if(k>4)
						psh=2;
					if(k>6)
						psh=3;
					
//					if((dig<10) && (j==0))
//						continue;
					
//					if(dig==topdig)
					if(0)
					{
						if(!i)
							maxsc+=16;
						if(j&(1<<dig))
							scarr[i]+=16>>psh;

						if(!i)
							maxsc+=8;
						if(!(j&(~(1<<dig))))
							scarr[i]+=8>>psh;
					}else
					{
						if(dig<10)
						{
							if(!i)
								maxsc+=64;
							if(!i)
								maxsc+=32;

	//						if(!(j&(1<<dig)))
							if(j&(1<<dig))
							{
								scarr[i]+=64>>psh;

	//							if(!(j&(~(1<<dig))))
	//								scarr[i]+=8>>psh;

								k=32-(btnn_popcnt(j)-1)*8;
								if(k<0)
									k=0;
								scarr[i]+=k>>psh;
							}
						}else
						{
							k=4-btnn_popcnt(j);
							if(k<0)
								k=0;

							if(!i)
								maxsc+=4;
//							if(!j)
//								scarr[i]+=2>>psh;

							scarr[i]+=k;
						}
					}
				}
			}
		}
		
		if(!maxsc)
			maxsc=10000;
			
		for(i=0; i<GENSZ; i++)
			for(j=i+1; j<GENSZ; j++)
		{
			if(gen<(1<<8))
			{
				if((i&3)!=(j&3))
					continue;
			}

			if(gen<(1<<10))
			{
				if((i&1)!=(j&1))
					continue;
			}

			if(scarr[j]>scarr[i])
			{
				pv=narr[j];
				narr[j]=narr[i];
				narr[i]=pv;

				v=scarr[j];
				scarr[j]=scarr[i];
				scarr[i]=v;
			}
		}

		for(i=16; i<GENSZ; i++)
		{
			if(gen<(1<<8))
			{
				if(i<32)
					continue;

//				i0=bitnn_selsqrt[rand()&255];
//				i1=bitnn_selsqrt[rand()&255];
		
//				i0=rand()&31;
//				i1=rand()&31;

				i0=rand()&15;
				i1=rand()&15;

				i0=(i0&(~3))|(i&3);
				i1=(i1&(~3))|(i&3);
			}else
				if(gen<(1<<10))
			{
				if(i<32)
					continue;

//				i0=bitnn_selsqrt[rand()&255];
//				i1=bitnn_selsqrt[rand()&255];
		
//				i0=rand()&31;
//				i1=rand()&31;

				i0=rand()&15;
				i1=rand()&15;

				i0=(i0&(~1))|(i&1);
				i1=(i1&(~1))|(i&1);
			}else
			{
//				i0=rand()&15;
//				i1=rand()&15;

//				i0=rand()&7;
//				i1=rand()&7;
				
				i0=bitnn_selsqrt[rand()&255];
				i1=bitnn_selsqrt[rand()&255];
			}

			narr[i]=bitnn_mutate(narr[i0], narr[i1], i);
		}

		narr[GENSZ-1]=bitnn_generate();
		narr[GENSZ-2]=bitnn_generate();
		narr[GENSZ-3]=bitnn_generate();
		narr[GENSZ-4]=bitnn_generate();
		
		printf("Gen=%d Bsc=%d/%d (%.2f%%)\r",
			gen, scarr[0], maxsc, (100.0*scarr[0])/maxsc);
		
		if(scarr[0]==maxsc)
			break;
	}

	printf("\n");

	for(dig=0; dig<10; dig++)
	{
		db=tk_gfxcon_glyphs['0'+dig];
		j=bitnn_eval8x8x1(db, narr[0]);
		printf("Dig=%d Res=%03X\n", dig, j);
	}

	bitnn_dumppat(narr[0]);
#endif

}

