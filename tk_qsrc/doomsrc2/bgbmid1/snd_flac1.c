#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <bgbmid.h>


#define bflac_print	printf
#define bflac_error	printf

#ifndef bflac_print
#define bflac_print	bflac_print_stub
void bflac_print_stub(char *s, ...) {}
#endif


#define FCC_HL_fLaC 0x664C6143	//'fLaC', big-endian

//typedef unsigned char byte;
//typedef unsigned int uint;

byte *bflac_cs;		//current pos in bitstream (input)
byte *bflac_css;	//current start of bitstream (input)
byte *bflac_cse;	//current end of bitstream (input)

byte *bflac_ct;		//current pos in bitstream (output)
byte *bflac_cts;	//current start of bitstream (output)
byte *bflac_cte;	//current end of bitstream (output)

uint bflac_win;		//bitstream window
int bflac_pos;		//bitstream offset


int bflac_strm_chan;
int bflac_strm_rate;
int bflac_strm_bits;
int bflac_strm_len;

int bflac_chan;
int bflac_rate;
int bflac_bits;

int bflac_blk_sz;

int bflac_chan_xtra;
int bflac_chan_bits;

int *bflac_sbp=NULL;	//sample buffer pointer (start)
int *bflac_ebp=NULL;	//sample buffer pointer (end)
int *bflac_cbp=NULL;	//sample buffer pointer (current)

int bflac_lpc_chan;
int bflac_lpc_skip;

int bflac_lpc_ctx[32];
//float bflac_lpc_coef[32];

int bflac_lpc_ord;
int bflac_lpc_cpb;
int bflac_lpc_cps;
int bflac_lpc_coef[32];

static int aszBlk[16]={-1, 192, 576,1152,2304,4608, -2, -3,
	256,512,1024,2048,4096,8192,16384,32768};
static int aRate[16]={0,
	88200,176400,192000,
	8000,16000,22050,24000,32000,44100,48000,96000,
	-2, -3, -4, -1};
static char *asChnName[16]={"mono", "stereo",
	"left/right/center", "left/right/back-left/back-right",
	"5-channel surround",
	"6-channel surround",
	"7-channel", "8-channel",
	"left/side stereo",
	"right/side stereo",
	"mid/side stereo", "!", "!", "!", "!", "!" 
	};
static int aChnNum[16]={
	1,  2,  3,  4,  5,  6,  7,  8,
	2,  2,  2, -1, -1, -1, -1, -1};
static int aChnXtra[16]={
	0,  0,  0,  0,  0,  0,  0,  0,
	1,  2,  3, -1, -1, -1, -1, -1};

static int aSmpBits[8]={0,8,12,-1,16,20,24,-1};

static int aCrc8[256] = {
	0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15,
	0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
	0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65,
	0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
	0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5,
	0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
	0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85,
	0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
	0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2,
	0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
	0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2,
	0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
	0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32,
	0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
	0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42,
	0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
	0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C,
	0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
	0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC,
	0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
	0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C,
	0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
	0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C,
	0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
	0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B,
	0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
	0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B,
	0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
	0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB,
	0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
	0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB,
	0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3
};

static int aCrc16[256] = {
	0x0000,  0x8005,  0x800f,  0x000a,  0x801b,  0x001e,  0x0014,  0x8011,
	0x8033,  0x0036,  0x003c,  0x8039,  0x0028,  0x802d,  0x8027,  0x0022,
	0x8063,  0x0066,  0x006c,  0x8069,  0x0078,  0x807d,  0x8077,  0x0072,
	0x0050,  0x8055,  0x805f,  0x005a,  0x804b,  0x004e,  0x0044,  0x8041,
	0x80c3,  0x00c6,  0x00cc,  0x80c9,  0x00d8,  0x80dd,  0x80d7,  0x00d2,
	0x00f0,  0x80f5,  0x80ff,  0x00fa,  0x80eb,  0x00ee,  0x00e4,  0x80e1,
	0x00a0,  0x80a5,  0x80af,  0x00aa,  0x80bb,  0x00be,  0x00b4,  0x80b1,
	0x8093,  0x0096,  0x009c,  0x8099,  0x0088,  0x808d,  0x8087,  0x0082,
	0x8183,  0x0186,  0x018c,  0x8189,  0x0198,  0x819d,  0x8197,  0x0192,
	0x01b0,  0x81b5,  0x81bf,  0x01ba,  0x81ab,  0x01ae,  0x01a4,  0x81a1,
	0x01e0,  0x81e5,  0x81ef,  0x01ea,  0x81fb,  0x01fe,  0x01f4,  0x81f1,
	0x81d3,  0x01d6,  0x01dc,  0x81d9,  0x01c8,  0x81cd,  0x81c7,  0x01c2,
	0x0140,  0x8145,  0x814f,  0x014a,  0x815b,  0x015e,  0x0154,  0x8151,
	0x8173,  0x0176,  0x017c,  0x8179,  0x0168,  0x816d,  0x8167,  0x0162,
	0x8123,  0x0126,  0x012c,  0x8129,  0x0138,  0x813d,  0x8137,  0x0132,
	0x0110,  0x8115,  0x811f,  0x011a,  0x810b,  0x010e,  0x0104,  0x8101,
	0x8303,  0x0306,  0x030c,  0x8309,  0x0318,  0x831d,  0x8317,  0x0312,
	0x0330,  0x8335,  0x833f,  0x033a,  0x832b,  0x032e,  0x0324,  0x8321,
	0x0360,  0x8365,  0x836f,  0x036a,  0x837b,  0x037e,  0x0374,  0x8371,
	0x8353,  0x0356,  0x035c,  0x8359,  0x0348,  0x834d,  0x8347,  0x0342,
	0x03c0,  0x83c5,  0x83cf,  0x03ca,  0x83db,  0x03de,  0x03d4,  0x83d1,
	0x83f3,  0x03f6,  0x03fc,  0x83f9,  0x03e8,  0x83ed,  0x83e7,  0x03e2,
	0x83a3,  0x03a6,  0x03ac,  0x83a9,  0x03b8,  0x83bd,  0x83b7,  0x03b2,
	0x0390,  0x8395,  0x839f,  0x039a,  0x838b,  0x038e,  0x0384,  0x8381,
	0x0280,  0x8285,  0x828f,  0x028a,  0x829b,  0x029e,  0x0294,  0x8291,
	0x82b3,  0x02b6,  0x02bc,  0x82b9,  0x02a8,  0x82ad,  0x82a7,  0x02a2,
	0x82e3,  0x02e6,  0x02ec,  0x82e9,  0x02f8,  0x82fd,  0x82f7,  0x02f2,
	0x02d0,  0x82d5,  0x82df,  0x02da,  0x82cb,  0x02ce,  0x02c4,  0x82c1,
	0x8243,  0x0246,  0x024c,  0x8249,  0x0258,  0x825d,  0x8257,  0x0252,
	0x0270,  0x8275,  0x827f,  0x027a,  0x826b,  0x026e,  0x0264,  0x8261,
	0x0220,  0x8225,  0x822f,  0x022a,  0x823b,  0x023e,  0x0234,  0x8231,
	0x8213,  0x0216,  0x021c,  0x8219,  0x0208,  0x820d,  0x8207,  0x0202
};

byte bflac_crc8=0;
unsigned short bflac_crc16=0;

int BFLAC_ClearCRC()
{
	bflac_crc8=0;
	bflac_crc16=0;
}

int BFLAC_UpdateCRC(int v)
{
	bflac_crc8=aCrc8[bflac_crc8^v];
	bflac_crc16=(bflac_crc16<<8)^aCrc16[(bflac_crc16>>8)^v];
}

int BFLAC_NextByte()
{
	int i;

	if(bflac_cs>=bflac_cse)return(0);

	i=*bflac_cs++;
	return(i);
}

void BFLAC_EmitByte(int v)
{
	int i, j;

	if(!bflac_ct)return;

	if(bflac_ct>=bflac_cte)
	{
		i=bflac_cte-bflac_cts;
		j=bflac_ct-bflac_cts;

		i=i+(i>>1);
		bflac_cts=bgbmid_realloc(bflac_cts, i);
		bflac_cte=bflac_cts+i;
		bflac_ct=bflac_cts+j;
	}

	*bflac_ct++=v;
}

int BFLAC_SetupReadStream()
{
	bflac_win=0;
	bflac_pos=0;

	bflac_win=BFLAC_NextByte()<<24;
	bflac_win|=BFLAC_NextByte()<<16;
	bflac_win|=BFLAC_NextByte()<<8;
	bflac_win|=BFLAC_NextByte()<<0;
}

int BFLAC_InitReadStream(byte *buf, int sz)
{
	bflac_css=buf;
	bflac_cse=buf+sz;
	bflac_cs=buf;
	BFLAC_SetupReadStream();
}

int BFLAC_InitWriteStream(byte *buf, int sz)
{
	bflac_cts=buf;
	bflac_cte=buf+sz;
	bflac_ct=buf;

	bflac_win=0;
	bflac_pos=0;
}

void BFLAC_AdjustRead()
{
	int i;

	while(bflac_pos>=8)
	{
		BFLAC_UpdateCRC(bflac_win>>24);
		bflac_win<<=8;
		bflac_win|=BFLAC_NextByte();
		bflac_pos-=8;
	}
}

void BFLAC_AdjustWrite()
{
	int i;
	while(bflac_pos>=8)
	{
		if(!bflac_ct)
		{
			bflac_win<<=8;
			bflac_pos-=8;
			continue;
		}

		i=(bflac_win>>24)&0xFF;
		bflac_win<<=8;
		bflac_pos-=8;

		BFLAC_EmitByte(i);
		BFLAC_UpdateCRC(i);
	}
}

int BFLAC_ReadBit()
{
	int i;

	i=(bflac_win>>(31-bflac_pos))&1;
	bflac_pos++;
	BFLAC_AdjustRead();
	return(i);
}

int BFLAC_ReadNBits(int n)
{
	int i, j;

	i=(bflac_win>>(32-bflac_pos-n))&((1<<n)-1);
	bflac_pos+=n;
	BFLAC_AdjustRead();
	return(i);
}

void BFLAC_SkipNBits(int n)
{
	int j;

	bflac_pos+=n;
	BFLAC_AdjustRead();
}

int BFLAC_ReadNBitsS(int n)
{
	int i;
	i=BFLAC_ReadNBits(n);
	if(i&(1<<(n-1)))i=((-1)<<n)|i;
	return(i);
}


long long BFLAC_ReadNBits2(int n)
{
	long long li;
	if(n>16)
	{
		li=BFLAC_ReadNBits2(n-16)<<16;
		li|=BFLAC_ReadNBits(16);
		return(li);
	}

	li=BFLAC_ReadNBits(n);
	return(li);
}

long long BFLAC_ReadNBits2S(int n)
{
	long long li;
	li=BFLAC_ReadNBits2(n);
	if(li&(1LL<<(n-1)))li=((-1LL)<<n)|li;
	return(li);
}

int BFLAC_PeekWord()
{
	int i;
	i=(bflac_win>>(16-bflac_pos))&65535;
	return(i);
}

void BFLAC_WriteBit(int i)
{
	bflac_win|=(i&1)<<(31-bflac_pos);
	bflac_pos++;
	BFLAC_AdjustWrite();
}

void BFLAC_WriteNBits(int i, int n)
{
	i&=(1<<n)-1;

	bflac_win|=(i&((1<<n)-1))<<(32-bflac_pos-n);
	bflac_pos+=n;
	BFLAC_AdjustWrite();
}

void BFLAC_WriteNBits2(long long i, int n)
{
	if(n>16)
	{
		BFLAC_WriteNBits2(i>>16, n-16);
		BFLAC_WriteNBits(i&0xFFFF, 16);
		return;
	}
	BFLAC_WriteNBits(i, n);
}

void BFLAC_FlushBits()
{
	int i;

	if(!bflac_ct)
	{
		bflac_win=0;
		bflac_pos=0;
		return;
	}

	while(bflac_pos>0)
	{
		i=(bflac_win>>24)&0xFF;
		bflac_win<<=8;
		bflac_pos-=8;
		BFLAC_EmitByte(i);
		BFLAC_UpdateCRC(i);
	}

	bflac_win=0;
	bflac_pos=0;
}

int BFLAC_ReadAlignByte()
{
	if(bflac_pos&7)
		BFLAC_ReadNBits(8-(bflac_pos&7));
}

int BFLAC_ReadByte()
{
	BFLAC_ReadAlignByte();
	return(BFLAC_ReadNBits(8));
}

int BFLAC_ReadShort()
{
	int i;
	i=BFLAC_ReadByte()<<8;
	i+=BFLAC_ReadByte();
	i=(short)i;
	return(i);
}

int BFLAC_ReadInt()
{
	unsigned int i;
	i=BFLAC_ReadByte()<<24;
	i|=BFLAC_ReadByte()<<16;
	i|=BFLAC_ReadByte()<<8;
	i|=BFLAC_ReadByte();
	return((int)i);
}

int BFLAC_ReadInt24()
{
	int i;
	i=BFLAC_ReadByte()<<16;
	i+=BFLAC_ReadByte()<<8;
	i+=BFLAC_ReadByte();
	if(i&0x800000)i|=0xFF000000;
	return((int)i);
}

long long BFLAC_ReadIntUTF8()
{
	long long i;

	i=BFLAC_ReadByte();

	if(i==0xFF)
	{
		i=BFLAC_ReadByte();

		if(i>=0xF0)
		{
			i=(i&7LL)<<60;
			i+=(BFLAC_ReadByte()&63LL)<<54;
			i+=(BFLAC_ReadByte()&63LL)<<48;
			i+=(BFLAC_ReadByte()&63LL)<<42;
			i+=(BFLAC_ReadByte()&63LL)<<36;
			i+=(BFLAC_ReadByte()&63LL)<<30;
			i+=(BFLAC_ReadByte()&63)<<24;
			i+=(BFLAC_ReadByte()&63)<<18;
			i+=(BFLAC_ReadByte()&63)<<12;
			i+=(BFLAC_ReadByte()&63)<<6;
			i+=BFLAC_ReadByte()&63;
		}else if(i>=0xE0)
		{
			i=(i&15LL)<<54;
			i+=(BFLAC_ReadByte()&63LL)<<48;
			i+=(BFLAC_ReadByte()&63LL)<<42;
			i+=(BFLAC_ReadByte()&63LL)<<36;
			i+=(BFLAC_ReadByte()&63LL)<<30;
			i+=(BFLAC_ReadByte()&63)<<24;
			i+=(BFLAC_ReadByte()&63)<<18;
			i+=(BFLAC_ReadByte()&63)<<12;
			i+=(BFLAC_ReadByte()&63)<<6;
			i+=BFLAC_ReadByte()&63;
		}else if(i>=0xC0)
		{
			i=(i&31LL)<<48;
			i+=(BFLAC_ReadByte()&63LL)<<42;
			i+=(BFLAC_ReadByte()&63LL)<<36;
			i+=(BFLAC_ReadByte()&63LL)<<30;
			i+=(BFLAC_ReadByte()&63)<<24;
			i+=(BFLAC_ReadByte()&63)<<18;
			i+=(BFLAC_ReadByte()&63)<<12;
			i+=(BFLAC_ReadByte()&63)<<6;
			i+=BFLAC_ReadByte()&63;
		}else
		{
			i=(i&63LL)<<42;
			i+=(BFLAC_ReadByte()&63LL)<<36;
			i+=(BFLAC_ReadByte()&63LL)<<30;
			i+=(BFLAC_ReadByte()&63)<<24;
			i+=(BFLAC_ReadByte()&63)<<18;
			i+=(BFLAC_ReadByte()&63)<<12;
			i+=(BFLAC_ReadByte()&63)<<6;
			i+=BFLAC_ReadByte()&63;
		}
	}else if(i>=0xFE)
	{
		i=(BFLAC_ReadByte()&63LL)<<30;
		i+=(BFLAC_ReadByte()&63)<<24;
		i+=(BFLAC_ReadByte()&63)<<18;
		i+=(BFLAC_ReadByte()&63)<<12;
		i+=(BFLAC_ReadByte()&63)<<6;
		i+=BFLAC_ReadByte()&63;
	}else if(i>=0xFC)
	{
		i=(i&1)<<30;
		i+=(BFLAC_ReadByte()&63)<<24;
		i+=(BFLAC_ReadByte()&63)<<18;
		i+=(BFLAC_ReadByte()&63)<<12;
		i+=(BFLAC_ReadByte()&63)<<6;
		i+=BFLAC_ReadByte()&63;
	}else if(i>=0xF8)
	{
		i=(i&3)<<24;
		i+=(BFLAC_ReadByte()&63)<<18;
		i+=(BFLAC_ReadByte()&63)<<12;
		i+=(BFLAC_ReadByte()&63)<<6;
		i+=BFLAC_ReadByte()&63;
	}else if(i>=0xF0)
	{
		i=(i&7)<<18;
		i+=(BFLAC_ReadByte()&63)<<12;
		i+=(BFLAC_ReadByte()&63)<<6;
		i+=BFLAC_ReadByte()&63;
	}else if(i>=0xE0)
	{
		i=(i&15)<<12;
		i+=(BFLAC_ReadByte()&63)<<6;
		i+=BFLAC_ReadByte()&63;
	}else if(i>=0xC0)
	{
		i=(i&31)<<6;
		i+=BFLAC_ReadByte()&63;
	}else if(i>=0x80)
	{
		*(int *)-1=-1;
	}

	return(i);
}

void BFLAC_SkipNBytes(int i)
{
	while(i--)BFLAC_ReadByte();
}

void BFLAC_WriteByte(int i)
{
	BFLAC_FlushBits();
	BFLAC_EmitByte(i);
	BFLAC_UpdateCRC(i);
}

void BFLAC_WriteShort(int i)
{
	BFLAC_FlushBits();
	if(!bflac_ct)return;
	BFLAC_WriteByte((i>>8)&0xFF);
	BFLAC_WriteByte(i&0xFF);
}

void BFLAC_WriteInt(int i)
{
	BFLAC_FlushBits();
	if(!bflac_ct)return;
	BFLAC_WriteByte((i>>24)&0xFF);
	BFLAC_WriteByte((i>>16)&0xFF);
	BFLAC_WriteByte((i>>8)&0xFF);
	BFLAC_WriteByte(i&0xFF);
}

void BFLAC_WriteInt24(int i)
{
	BFLAC_FlushBits();
	if(!bflac_ct)return;
	BFLAC_WriteByte((i>>16)&0xFF);
	BFLAC_WriteByte((i>>8)&0xFF);
	BFLAC_WriteByte(i&0xFF);
}

void BFLAC_WriteIntUTF8(long long i)
{
	BFLAC_FlushBits();
	if(!bflac_ct)return;

	if(i<(1<<7))
	{
		BFLAC_WriteByte(i);
	}else if(i<(1<<11))
	{
		BFLAC_WriteByte(0xC0 | (i>>6));
		BFLAC_WriteByte(0x80 | (i&63));
	}else if(i<(1<<16))
	{
		BFLAC_WriteByte(0xE0 | (i>>12));
		BFLAC_WriteByte(0x80 | ((i>>6)&63));
		BFLAC_WriteByte(0x80 | (i&63));
	}else if(i<(1<<21))
	{
		BFLAC_WriteByte(0xF0 | (i>>18));
		BFLAC_WriteByte(0x80 | ((i>>12)&63));
		BFLAC_WriteByte(0x80 | ((i>>6)&63));
		BFLAC_WriteByte(0x80 | (i&63));
	}else if(i<(1<<26))
	{
		BFLAC_WriteByte(0xF8 | (i>>24));
		BFLAC_WriteByte(0x80 | ((i>>18)&63));
		BFLAC_WriteByte(0x80 | ((i>>12)&63));
		BFLAC_WriteByte(0x80 | ((i>>6)&63));
		BFLAC_WriteByte(0x80 | (i&63));
	}else if(i<(1<<31))
	{
		BFLAC_WriteByte(0xFC | (i>>30));
		BFLAC_WriteByte(0x80 | ((i>>24)&63));
		BFLAC_WriteByte(0x80 | ((i>>18)&63));
		BFLAC_WriteByte(0x80 | ((i>>12)&63));
		BFLAC_WriteByte(0x80 | ((i>>6)&63));
		BFLAC_WriteByte(0x80 | (i&63));
	}else if(i<(1LL<<36))
	{
		BFLAC_WriteByte(0xFE | (i>>36));
		BFLAC_WriteByte(0x80 | ((i>>30)&63));
		BFLAC_WriteByte(0x80 | ((i>>24)&63));
		BFLAC_WriteByte(0x80 | ((i>>18)&63));
		BFLAC_WriteByte(0x80 | ((i>>12)&63));
		BFLAC_WriteByte(0x80 | ((i>>6)&63));
		BFLAC_WriteByte(0x80 | (i&63));
	}
}

int BFLAC_ReadUnary()
{
	int i;
	i=0;
	while(1) { if(BFLAC_ReadBit())break; i++; }
	return(i);
}

int BFLAC_ReadRice(int n)
{
	int i, j;
	i=BFLAC_ReadUnary();
	j=BFLAC_ReadNBits(n);
	return((i<<n)|j);
}

void BFLAC_WriteUnary(int i)
{
	while(i--)BFLAC_WriteBit(0);
	BFLAC_WriteBit(1);
}

void BFLAC_WriteRice(int v, int n)
{
	int i, j;

	i=v>>n;
	while(i--)BFLAC_WriteBit(0);
	BFLAC_WriteBit(1);
	BFLAC_WriteNBits(v, n);
}

int BFLAC_ReadSRice(int n)
{
	int i;
	i=BFLAC_ReadRice(n);
	i=(i&1)?(-((i>>1)+1)):(i>>1);
	return(i);
}

void BFLAC_WriteSRice(int v, int n)
{
	int i;
	i=(v<0)?(((-v)<<1)-1):(v<<1);
	BFLAC_WriteRice(i, n);
}


//LPC

int BFLAC_PredictLPC()
{
	int i, j;

	if(bflac_lpc_cps<0)
	{
		j=0;
		for(i=0; i<bflac_lpc_ord; i++)
			j+=bflac_lpc_coef[i]*bflac_lpc_ctx[i];
		return(j<<(-bflac_lpc_cps));
	}

	j=0;
	for(i=0; i<bflac_lpc_ord; i++)
		j+=bflac_lpc_coef[i]*bflac_lpc_ctx[i];
	return(j>>bflac_lpc_cps);
}

int BFLAC_UpdateLPC(int v)
{
	int i;
	for(i=bflac_lpc_ord-1; i>0; i--)
		bflac_lpc_ctx[i]=bflac_lpc_ctx[i-1];
	bflac_lpc_ctx[0]=v;
}

int BFLAC_OutSample(int v)
{
	int i, j;

	if(bflac_lpc_skip)return(0);

	if(bflac_cbp)
	{
		if(bflac_cbp>=bflac_ebp)
		{
			i=bflac_ebp-bflac_sbp;
			j=bflac_cbp-bflac_sbp;

			i=i+(i>>1);
			bflac_sbp=bgbmid_realloc(bflac_sbp, i*sizeof(int));
			bflac_ebp=bflac_sbp+i;
			bflac_cbp=bflac_sbp+j;
		}

		*bflac_cbp=v;
		bflac_cbp+=bflac_chan;
	}

	BFLAC_UpdateLPC(v);

//	bflac_print("%d ", v);
	return(0);
}

int BFLAC_OutDelta(int v)
{
	BFLAC_OutSample(BFLAC_PredictLPC()+v);
}


//Decoder

int BFLAC_DecStrmInfo()
{
	int i, j, k;

	i=BFLAC_ReadShort();
	j=BFLAC_ReadShort();

	bflac_print("MinBlkSz=%d MaxBlkSz=%d\n", i, j);

	i=BFLAC_ReadInt24();
	j=BFLAC_ReadInt24();

	bflac_print("MinFrmSz=%d MaxFrmSz=%d\n", i, j);

	i=BFLAC_ReadNBits2(20);
	j=BFLAC_ReadNBits(3);
	k=BFLAC_ReadNBits(5);

	bflac_chan=j+1;
	bflac_rate=i;
	bflac_bits=k+1;

	bflac_strm_chan=j+1;
	bflac_strm_rate=i;
	bflac_strm_bits=k+1;

	bflac_print("Rate=%d Chan=%d Bits=%d\n", i, j+1, k+1);

	i=BFLAC_ReadNBits2(36);
	bflac_strm_len=i;

	bflac_print("Len=%d\n", bflac_strm_len);

	BFLAC_SkipNBytes(16);
}

int BFLAC_DecMetadata()
{
	char *sty[8]={
		"STREAMINFO", "PADDING", "APPLICATION", "SEEKTABLE",
		"VORBIS_COMMENT", "CUESHEET", "PICTURE", NULL};
	char *s;
	int lst, ty, sz;
	int i;
	i=BFLAC_ReadInt();

	lst=(i>>31)&1;
	ty=(i>>24)&127;
	sz=i&0xFFFFFF;

	s="unknown";
	if(ty<7)s=sty[ty];

	bflac_print("L%d TY=%d(%s) SZ=%d\n", lst, ty, s, sz);

	if(ty==0)
	{
		BFLAC_DecStrmInfo();
		return(lst);
	}

	BFLAC_SkipNBytes(sz);

	return(lst);
}

int BFLAC_SyncFrame()
{
	int i, j, k;
//	i=BFLAC_ReadNBits(14);

	i=BFLAC_PeekWord();
	if((i&0xFFFC)==0xFFF8)
		return(0);

	bflac_print("Sync At %d\n", bflac_cs-bflac_css);

	j=65536;
	i=BFLAC_PeekWord();
	while(((i&0xFFFC)!=0xFFF8) && j--)
	{
		k=BFLAC_ReadByte(8);
//		bflac_print("%02X ", k);
		i=BFLAC_PeekWord();
	}
}

int BFLAC_DecFrameHead()
{
	int i, j, k;

	BFLAC_SyncFrame();

	BFLAC_ClearCRC();

	i=BFLAC_ReadNBits(14);
	if(i!=0x3FFE)
	{
		bflac_print("Bad Frame Sync %X\n", i);
		return(-2);
	}

	i=BFLAC_ReadBit();
	if(i)
	{
		bflac_print("Bad Reserved Bit value\n");
		return(-1);
	}

	i=BFLAC_ReadBit();
	bflac_print("FrameSync=%s\n", i?"samples":"frames");

	i=BFLAC_ReadNBits(4);
	bflac_print("FrameBlkSz=%d(%d)\n", i, aszBlk[i]);

	bflac_blk_sz=aszBlk[i];

	i=BFLAC_ReadNBits(4);
	j=BFLAC_ReadNBits(4);
	k=BFLAC_ReadNBits(3);

	bflac_print("FrameSampleRate=%d(%d)\n", i, aRate[i]);
	bflac_print("FrameChan=%d(%s) %d\n", j, asChnName[j], aChnXtra[j]);
	bflac_print("FrameSampBits=%d(%d)\n", k, aSmpBits[k]);

	bflac_chan=aChnNum[j];
	bflac_rate=aRate[i];
	bflac_bits=aSmpBits[k];

	bflac_chan_xtra=aChnXtra[j];

	if(bflac_chan!=bflac_strm_chan)return(-1);
	if(bflac_rate!=bflac_strm_rate)return(-1);
	if(bflac_bits!=bflac_strm_bits)return(-1);

	if(bflac_chan_xtra<0)return(-1);

	i=BFLAC_ReadBit();
	if(i)
	{
		bflac_print("Bad Reserved Bit value\n");
		return(-1);
	}

	i=BFLAC_ReadIntUTF8();
	bflac_print("FramePosition=%d\n", i);

	if(bflac_blk_sz==(-2))bflac_blk_sz=BFLAC_ReadByte();
	if(bflac_blk_sz==(-3))bflac_blk_sz=BFLAC_ReadShort();

	if(bflac_rate==(-2))bflac_rate=BFLAC_ReadByte()*1000;
	if(bflac_rate==(-3))bflac_rate=BFLAC_ReadShort();
	if(bflac_rate==(-4))bflac_rate=BFLAC_ReadShort()*10;

	j=bflac_crc8;
	i=BFLAC_ReadByte();
	bflac_print("FrameCRC8=%02X %02X\n", i, j);

	if(i!=j)return(-1);

	return(0);
	
}

int BFLAC_DecPart(int wp, int n)
{
	int i, j, w;

	w=BFLAC_ReadNBits(wp);

	if(w==((1<<wp)-1))
	{
		w=BFLAC_ReadNBits(5);

		for(i=0; i<n; i++)
		{
			j=BFLAC_ReadNBits2S(w);
			BFLAC_OutDelta(j);
		}

		return(0);
	}

	if(w>24)return(-1);

	for(i=0; i<n; i++)
	{
		j=BFLAC_ReadSRice(w);
		BFLAC_OutDelta(j);
	}

	return(0);
}

int BFLAC_DecResidual()
{
	int i, j, k, l;

	i=BFLAC_ReadNBits(2);
//	bflac_print("ResidualType %d\n", i);

	if(i==2)return(-1);
	if(i==3)return(-1);

	j=BFLAC_ReadNBits(4);
//	bflac_print("ResidualOrder %d\n", j);

//	BFLAC_DecPart(4+i, j, 0);

	for(k=0; k<(1<<j); k++)
	{
		if(!j)
		{
			l=bflac_blk_sz-bflac_lpc_ord;
		}else
		{
			l=bflac_blk_sz>>j;
			if(!k)l-=bflac_lpc_ord;
		}

		if(l<=0)
		{
			bflac_print("PartitionSize<=0\n");
			return(-1);
		}

		l=BFLAC_DecPart(4+i, l);
		if(l<0)return(-1);
	}

	return(0);
}

int BFLAC_DecSubframeLPC(int lpc)
{
	int cpb, cps;
	int i, j, k;

	bflac_lpc_ord=lpc;

	bflac_print("Samples(%d) ", bflac_chan_bits);
	for(i=0; i<lpc; i++)
	{
		j=BFLAC_ReadNBits2S(bflac_chan_bits);
		bflac_print("%d ", j);
		BFLAC_OutSample(j);
	}
	bflac_print("\n");

	cpb=BFLAC_ReadNBits(4)+1;
	cps=BFLAC_ReadNBitsS(5);
	bflac_lpc_cpb=cpb;
	bflac_lpc_cps=cps;

	bflac_print("LPC_CoefBits=%d\n", cpb);
	bflac_print("LPC_CoefShift=%d\n", cps);

	bflac_print("LPC_Coef ");
	for(i=0; i<lpc; i++)
	{
		j=BFLAC_ReadNBits2S(cpb);
		bflac_print("%f ", ((float)j)/pow(2, cps));
		bflac_lpc_coef[i]=j;
	}
	bflac_print("\n");

	i=BFLAC_DecResidual();
	return(i);
}


int BFLAC_DecSubframeFixed(int lpc)
{
	int cpb, cps;
	int i, j, k;

//	bflac_lpc_skip=1;

	bflac_print("Fixed-%d\n", lpc);

	bflac_lpc_ord=lpc;

	for(i=0; i<lpc; i++)
	{
		j=BFLAC_ReadNBits2S(bflac_chan_bits);
		bflac_print("%d ", j);
		BFLAC_OutSample(j);
	}
	bflac_print("\n");

	bflac_lpc_cpb=8;
	bflac_lpc_cps=0;

	switch(lpc)
	{
	case 0:	/* 0 */
		break;
	case 1: /* a */
		bflac_lpc_coef[0]=1;
		break;
	case 2: /* a+(a-b) */
		bflac_lpc_coef[0]=2;
		bflac_lpc_coef[1]=-1;
		break;
	case 3: /* (2*(a-b)+a-b)-c */
		bflac_lpc_coef[0]=3;
		bflac_lpc_coef[1]=-3;
		bflac_lpc_coef[2]=1;
		break;
	case 4: /* 4*(a+c)-(4b+2b)-d */
		bflac_lpc_coef[0]=4;
		bflac_lpc_coef[1]=-6;
		bflac_lpc_coef[2]=4;
		bflac_lpc_coef[3]=-1;
		break;

	case 5: return(-1); break;
	case 6: return(-1); break;
	case 7: return(-1); break;
	}

	i=BFLAC_DecResidual();
	return(i);
}

int BFLAC_DecSubframeConst()
{
	int i, j, k;

	j=BFLAC_ReadNBits2S(bflac_chan_bits);
	for(i=0; i<bflac_blk_sz; i++)
		BFLAC_OutSample(j);
	return(0);
}

int BFLAC_DecSubframeVerbatim()
{
	int i, j, k;

	for(i=0; i<bflac_blk_sz; i++)
	{
		j=BFLAC_ReadNBits2S(bflac_chan_bits);
		BFLAC_OutSample(j);
	}
	return(0);
}

int BFLAC_DecSubframe()
{
	char *s;
	int lpc;
	int i, j, k;

	i=BFLAC_ReadBit();
	if(i)
	{
		bflac_print("Bad Reserved Bit value\n");
		return(-1);
	}

	i=BFLAC_ReadNBits(6);

	j=BFLAC_ReadBit();
	if(j)
	{
		j=BFLAC_ReadUnary();
		bflac_print("Wasted Bits %d\n", j);
	}

	if(i==0)
	{
		s="constant";
		bflac_print("SubframeType=%d(%s)\n", i, s);

		j=BFLAC_DecSubframeConst(lpc);
		return(j);
	}

	if(i==1)
	{
		s="verbatim";
		bflac_print("SubframeType=%d(%s)\n", i, s);

		j=BFLAC_DecSubframeVerbatim();
		return(j);
	}

	if((i>=8) && (i<16))
	{
		s="fixed"; lpc=i&7;
		bflac_print("SubframeType=%d(%s) %d\n", i, s, lpc);

		j=BFLAC_DecSubframeFixed(lpc);
		return(j);
	}

	if((i>=32) && (i<64))
	{
		s="LPC"; lpc=(i&31)+1;
		bflac_print("SubframeType=%d(%s) %d\n", i, s, lpc);

		j=BFLAC_DecSubframeLPC(lpc);
		return(j);
	}

	bflac_print("Unhandled: SubframeType=%d\n", i);
	return(-1);
}

int BFLAC_DecFrame()
{
	int *pi;
	int i, j, k;

	if(bflac_cbp)
	{
		i=bflac_cbp-bflac_sbp;
		bflac_print("Frame @ %d\n", i);
	}

	i=BFLAC_DecFrameHead();
	if(i<0)return(i);

	pi=bflac_cbp;

	bflac_print("Block Xtra %d\n", bflac_chan_xtra);

	bflac_lpc_skip=0;
	for(i=0; i<bflac_chan; i++)
	{
		bflac_cbp=pi+i;
		bflac_lpc_chan=i;

		bflac_chan_bits=bflac_bits;
		if((bflac_chan_xtra==1) && (i==1))bflac_chan_bits++;
		if((bflac_chan_xtra==2) && (i==0))bflac_chan_bits++;
		if((bflac_chan_xtra==3) && (i==1))bflac_chan_bits++;

		j=BFLAC_DecSubframe();
		if(j<0)return(j);

		bflac_print("\n");
	}

	bflac_cbp=pi+(bflac_blk_sz*bflac_chan);

	bflac_print("Block Xtra2 %d\n", bflac_chan_xtra);

	//left/side
	if(bflac_chan_xtra==1)
		for(i=0; i<bflac_blk_sz; i++)
			pi[i*2+1]=pi[i*2+0]-pi[i*2+1];

	//side/right
	if(bflac_chan_xtra==2)
		for(i=0; i<bflac_blk_sz; i++)
			pi[i*2+0]+=pi[i*2+1];

	//mid/side
	if(bflac_chan_xtra==3)
	{
		for(i=0; i<bflac_blk_sz; i++)
		{
			j=pi[i*2+0]; k=pi[i*2+1];

			j=(j<<1)|(k&1);
			pi[i*2+0]=(j+k)>>1;
			pi[i*2+1]=(j-k)>>1;
		}
	}

	BFLAC_ReadAlignByte();

	j=bflac_crc16;
	i=BFLAC_ReadShort(16);
	bflac_print("FrameCRC16=%04X %04X\n", i&0xFFFF, j);

	bflac_print("\n");

	return(0);
}

int *BFLAC_DecStream32(byte *buf, int sz,
	int *rch, int *rrt, int *rbps, int *rlen)
{
	int *sbuf;
	int i, j;

	BFLAC_InitReadStream(buf, sz);

	i=BFLAC_ReadInt();
	if(i!=FCC_HL_fLaC)
	{
		bflac_print("FCC not fLaC (%X)\n", i);
		return(NULL);
	}

	i=0;
	while(!i)i=BFLAC_DecMetadata();

	i=bflac_strm_chan*bflac_strm_len*sizeof(int);
	sbuf=bgbmid_malloc(i);
	memset(sbuf, 0, i);

	bflac_sbp=sbuf;
	bflac_cbp=sbuf;
	bflac_ebp=sbuf+i;

//	while(bflac_cbp<(sbuf+(bflac_strm_chan*bflac_strm_len)))
	while(1)
	{
		i=BFLAC_DecFrame();
		if(i<0)
//		if(i<(-1))
		{
			bflac_print("End At %d bytes, gen %d samples\n",
				bflac_cs-buf, bflac_cbp-sbuf);
			break;
		}
	}

	sbuf=bflac_sbp;
	i=(bflac_cbp-bflac_sbp)/bflac_strm_chan;

	if(rch)*rch=bflac_strm_chan;
	if(rrt)*rrt=bflac_strm_rate;
	if(rbps)*rbps=bflac_strm_bits;
	if(rlen)*rlen=i;

	return(sbuf);
}

//encoder

int BFLAC_EncStrmInfo()
{
	int i, j, k;

	BFLAC_WriteShort(4096);		//MinBlkSz
	BFLAC_WriteShort(4096);		//MaxBlkSz

	BFLAC_WriteInt24(0);		//MinFrmSz
	BFLAC_WriteInt24(0);		//MaxFrmSz

	BFLAC_WriteNBits2(bflac_strm_rate, 20);
	BFLAC_WriteNBits(bflac_strm_chan-1, 3);
	BFLAC_WriteNBits(bflac_strm_bits-1, 5);

	BFLAC_WriteNBits2(bflac_strm_len, 36);

	for(i=0; i<16; i++)
		BFLAC_WriteByte(0);
}

int BFLAC_EncMetadata()
{
	BFLAC_WriteInt(FCC_HL_fLaC);
	BFLAC_WriteInt(0x80000000 | 34);
	BFLAC_EncStrmInfo();
}

int BFLAC_CalcEncValBits(int v, int st)
{
	v=(v<0)?(((-v)<<1)-1):(v<<1);
	return((v>>st)+st+1);
}

int BFLAC_BestEncValBits(int *av, int n, int *rbk)
{
	int i, j, k, bi, bk;

	bi=15; bk=1<<30;
	for(i=0; i<15; i++)
	{
		k=0;
		for(j=0; j<n; j++)
			k+=BFLAC_CalcEncValBits(av[j], i);
		if(k<bk) { bi=i; bk=k; }
	}

	if(rbk)*rbk=bk;
	return(bi);
}

int BFLAC_EncValOrderBits(int *av, int n, int p, int lpc)
{
	int i, j, k, n0, n1;

	n0=1<<p; n1=n>>p; j=0;
	j+=4; BFLAC_BestEncValBits(av, n1-lpc, &k); j+=k;
	for(i=1; i<n0; i++)
	{
		j+=4;
		BFLAC_BestEncValBits(av+i*n1-lpc, n1, &k);
		j+=k;
	}

	return(j);
}

int BFLAC_BestEncValOrder(int *av, int n, int lpc)
{
	int i, j, k, bi, bj;

	bi=0;
	bj=BFLAC_EncValOrderBits(av, n, 0, lpc);
	for(i=1; i<8; i++)
	{
		if(n&((1<<i)-1))break;
		j=BFLAC_EncValOrderBits(av, n, i, lpc);
		if(j<bj) { bi=i; bj=j; }
	}

	return(bi);
}

int BFLAC_EncPart(int *av, int n)
{
	int i, j, w;

	w=BFLAC_BestEncValBits(av, n, NULL);	//best rice suffix length

	bflac_print("EncPart W=%d N=%d samples\n", w, n);

	BFLAC_WriteNBits(w, 4);		//rice suffix length
	for(i=0; i<n; i++)BFLAC_WriteSRice(av[i], w);
	return(0);
}

int BFLAC_EncPartOrder(int *av, int n, int p, int lpc)
{
	int i, j, k, n0, n1;

	n0=1<<p; n1=n>>p;
	BFLAC_EncPart(av, n1-lpc);
	for(i=1; i<n0; i++)
		BFLAC_EncPart(av+i*n1-lpc, n1);
}

int BFLAC_EncResidual(int *av, int n, int lpc)
{
	int i, j, k, l;

	i=BFLAC_BestEncValOrder(av, n, lpc);
	if(i)
	{
		BFLAC_WriteNBits(0, 2);		//residual type
		BFLAC_WriteNBits(i, 4);		//residual order
		BFLAC_EncPartOrder(av, n, i, lpc);
		return(0);
	}

	BFLAC_WriteNBits(0, 2);		//residual type
	BFLAC_WriteNBits(0, 4);		//residual order
	BFLAC_EncPart(av, n-lpc);
	return(0);
}

int BFLAC_GenDelta(int v)
{
	int i;

	i=v-BFLAC_PredictLPC();
	BFLAC_UpdateLPC(v);
	return(i);
}

int BFLAC_EncSubframeLPC(int *samp, int n)
{
	static int dbuf[4608];

	int cpb, cps, lpc;
	int i, j, k;

	lpc=bflac_lpc_ord;
	cpb=bflac_lpc_cpb;
	cps=bflac_lpc_cps;

	bflac_print("EncSubframeLPC: ord=%d cpb=%d cps=%d SB=%d N=%d\n",
		lpc, cpb, cps, bflac_chan_bits, n);

	for(i=0; i<lpc; i++)
	{
		j=samp[i];
		BFLAC_WriteNBits(j, bflac_chan_bits);
		BFLAC_UpdateLPC(j);
	}

	BFLAC_WriteNBits(cpb-1, 4);
	BFLAC_WriteNBits(cps, 5);

	for(i=0; i<lpc; i++)
		BFLAC_WriteNBits(bflac_lpc_coef[i], cpb);

	j=n-lpc;
	for(i=0; i<j; i++)
		dbuf[i]=BFLAC_GenDelta(samp[i+lpc]);

	BFLAC_EncResidual(dbuf, n, lpc);

	return(0);
}

int BFLAC_FrameBestLPC(int *samp, int n)
{
	static float ffa[8][8]={
		{ 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0},
		{ 1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0},
		{ 2.0, -1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0},
		{ 3.0, -3.0,  1.0,  0.0,  0.0,  0.0,  0.0,  0.0},
		{ 4.0, -6.0,  4.0, -1.0,  0.0,  0.0,  0.0,  0.0},
		{ 1.5, -0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0},
		{ 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0},
		{ 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0}
		};

	float fe[8], fc[8], f, g;
	float e[8], te;
	int vc[8];
	int c[8];
	int i, j, k, bi;

	for(i=0; i<8; i++)e[i]=0;
	for(i=0; i<8; i++)c[i]=0;
	for(i=0; i<8; i++)vc[i]=0;

	for(i=0; i<n; i++)
	{
		te=pow(2, 48); bi=-1;
		for(j=0; j<8; j++)
		{
			f=0;
			for(k=0; k<8; k++)f+=c[k]*ffa[j][k];
			g=fabs(samp[i]-f);
			e[j]+=g;

			if(g<te) { bi=j; te=g; }
		}

		vc[bi]++;

		for(j=7; j>0; j--)c[j]=c[j-1];
		c[0]=samp[i];
	}

	for(i=0; i<8; i++)e[i]=pow(e[i], 4);

	te=0;
	for(i=0; i<8; i++)te+=e[i];

//	bflac_print("TE %f  E ", te);
//	for(i=0; i<8; i++)bflac_print("%f ", e[i]);
//	bflac_print("\n");

	if(te<1)
	{
		bflac_lpc_coef[0]=0;
		bflac_lpc_ord=1;
		bflac_lpc_cpb=1;
		bflac_lpc_cps=0;
		return(0);
	}

//	for(i=0; i<8; i++)fe[i]=pow(1.0-(((float)e[i])/te), 6);
//	for(i=0; i<8; i++)fe[i]=1.0-(((float)e[i])/te);

	for(i=0; i<8; i++)te+=vc[i];
	for(i=0; i<8; i++)fe[i]=vc[i]/te;

	f=0; for(i=0; i<8; i++)f+=fe[i];
	for(i=0; i<8; i++)fe[i]/=f;

	bflac_print("FE ");
	for(i=0; i<8; i++)bflac_print("%f ", fe[i]);
	bflac_print("\n");



	for(i=0; i<8; i++)fc[i]=0;

	for(i=0; i<8; i++)
		for(j=0; j<8; j++)
			fc[i]+=fe[j]*ffa[j][i];

	bflac_print("FC ");
	for(i=0; i<8; i++)
		bflac_print("%f ", fc[i]);
	bflac_print("\n");

	for(i=0; i<10; i++)
	{
		for(j=0; j<8; j++)
		{
			f=fc[j]*pow(2, i); k=(int)f;
			if(k!=f)break;
		}
		if(j>=8)break;
	}
	bflac_lpc_cps=i;

	for(i=0; i<8; i++)
	{
		j=fc[i]*pow(2, bflac_lpc_cps);
		bflac_lpc_coef[i]=j;
		if(j)bflac_lpc_ord=i+1;
	}

	while(1)
	{
		for(i=0; i<8; i++)
		{
			j=bflac_lpc_coef[i];
			if(j>2047)break;
			if(j<(-2048))break;
		}
		if(i>=8)break;

		for(i=0; i<8; i++)bflac_lpc_coef[i]/=2;
		bflac_lpc_cps--;
	}

	for(i=0; i<24; i++)
	{
		for(j=0; j<8; j++)
		{
			k=bflac_lpc_coef[j]/(1<<i);
			if(k && (k!=(-1)))break;
		}
		if(j>=8)break;
	}
	bflac_lpc_cpb=i+2;

	return(0);
}

int BFLAC_EncSubframe(int *samp, int n)
{
	char *s;
	int lpc;
	int i, j, k;

	j=samp[0];
	for(i=0; i<n; i++)
		if(samp[i]!=j)break;

	if(i==n)
	{
		//handle constant runs
		BFLAC_WriteBit(0);
		BFLAC_WriteNBits(0, 6);
		BFLAC_WriteBit(0);
		BFLAC_WriteNBits(j, bflac_chan_bits);

		return(0);
	}

	BFLAC_FrameBestLPC(samp, n);

	bflac_print("Gen Pred ");
	for(i=0; i<bflac_lpc_ord; i++)
	{
		bflac_print("%f ", bflac_lpc_coef[i]/pow(2, bflac_lpc_cps));
	}
	bflac_print("\n");

	BFLAC_WriteBit(0);
	BFLAC_WriteNBits(32+(bflac_lpc_ord-1), 6);
	BFLAC_WriteBit(0);

	BFLAC_EncSubframeLPC(samp, n);

	return(0);
}

int BFLAC_EncFrameHead(int *samp, int nsamp, int nfrm)
{
	int e0, e1, e2, e3, be;
	int sz, ch, rt, bt;
	int i, j, k;

	BFLAC_FlushBits();
	BFLAC_ClearCRC();

	BFLAC_WriteNBits(0x3FFE, 14);	//sync code
	BFLAC_WriteBit(0);		//reserved bit

	BFLAC_WriteBit(0);		//frame type, 0=fixed-size

	for(i=0; i<16; i++)
		if(aszBlk[i]==nsamp)
			{ sz=i; break; }

	if(i==16)sz=7;			//16 bits length at end


	for(i=0; i<16; i++)
		if(aRate[i]==bflac_strm_rate)
			{ rt=i; break; }

	if(i==16)rt=13;			//16 bits rate at end

	ch=bflac_strm_chan-1;

	if(ch==1)
	{
		e0=0; e1=0; e2=0; e3=0;
		for(i=0; i<nsamp; i++)
		{
			j=samp[i*2+0];
			k=samp[i*2+1];

			e0+=abs(j)+abs(k);
			e1+=abs(j)+abs(j-k);
			e2+=abs(j-k)+abs(k);
			e3+=abs((j+k)/2)+abs(j-k);
		}

		be=e0;
		if(e1<be) { ch=8; be=e1; }
		if(e2<be) { ch=9; be=e2; }
		if(e3<be) { ch=10; be=e3; }
	}


	if(bflac_strm_bits==8)bt=1;
	if(bflac_strm_bits==16)bt=4;

	BFLAC_WriteNBits(sz, 4);
	BFLAC_WriteNBits(rt, 4);
	BFLAC_WriteNBits(ch, 4);
	BFLAC_WriteNBits(bt, 3);
	BFLAC_WriteBit(0);		//reserved bit

	bflac_chan=aChnNum[ch];
	bflac_rate=aRate[rt];
	bflac_bits=aSmpBits[bt];
	bflac_chan_xtra=aChnXtra[ch];


	BFLAC_WriteIntUTF8(nfrm);

	if(sz==7)BFLAC_WriteShort(nsamp);

	if(rt==12)BFLAC_WriteByte(bflac_strm_rate/1000);
	if(rt==13)BFLAC_WriteShort(bflac_strm_rate);
	if(rt==14)BFLAC_WriteShort(bflac_strm_rate/10);

	BFLAC_FlushBits();
	BFLAC_WriteByte(bflac_crc8);

	return(0);
	
}

int BFLAC_EncFrame(int *samp, int nsamp, int nfrm, int mfrm)
{
	static int sbuf[4608];
	int i, j, k;

	bflac_print("Ecnode Frame %d/%d\n", nfrm+1, mfrm);

	bflac_chan=bflac_strm_chan;
	bflac_rate=bflac_strm_rate;
	bflac_bits=bflac_strm_bits;

	BFLAC_EncFrameHead(samp, nsamp, nfrm);

	for(i=0; i<bflac_chan; i++)
	{
		for(j=0; j<nsamp; j++)
			sbuf[j]=samp[j*bflac_chan+i];

		bflac_chan_bits=bflac_bits;
		if(	((bflac_chan_xtra==1) && (i==1)) ||
			((bflac_chan_xtra==2) && (i==0)) ||
			((bflac_chan_xtra==3) && (i==1)) )
		{
			for(j=0; j<nsamp; j++)
				sbuf[j]=samp[j*bflac_chan+0]-
					samp[j*bflac_chan+1];
			bflac_chan_bits++;
		}

		if((bflac_chan_xtra==3) && (i==0))
		{
			for(j=0; j<nsamp; j++)
				sbuf[j]=(samp[j*bflac_chan+0]+
					samp[j*bflac_chan+1])/2;
		}

		j=BFLAC_EncSubframe(sbuf, nsamp);
		if(j<0)return(j);

		bflac_print("\n");
	}

	BFLAC_FlushBits();
	i=bflac_crc16;
	BFLAC_WriteShort(i);
	bflac_print("CRC %04X\n", i);

	bflac_print("\n");

	return(0);
}

byte *BFLAC_EncStream32(int *sbuf, int *rsz,
	int chan, int rate, int bits, int len)
{
	byte *buf;
	int *bp, *bpe;
	int i, j, k;

	buf=bgbmid_malloc(1<<20);
	BFLAC_InitWriteStream(buf, 1<<20);

	bflac_strm_chan=chan;
	bflac_strm_rate=rate;
	bflac_strm_bits=bits;
	bflac_strm_len=len;

	BFLAC_EncMetadata();

	bp=sbuf; bpe=sbuf+(chan*len);
	j=0; k=(len+4095)/4096;
	while((bp+(4096*chan))<=bpe)
	{
		i=BFLAC_EncFrame(bp, 4096, j++, k);
		if(i<0)
		{
			bflac_print("Encode Error At %d bytes, enc %d samples\n",
				bflac_ct-bflac_cts, bp-sbuf);
			break;
		}

		bp+=4096*chan;
	}

	if(bp<bpe)BFLAC_EncFrame(bp, (bpe-bp)/chan, j++, k);

	if(*rsz)*rsz=bflac_ct-bflac_cts;
	return(bflac_cts);
}


//helper functions

int BFLAC_ScaleSampleTrilinear(short *d, int dl, short *s, int sl)
{
	float *tb, *tb1, *tb2;
	float s1, s2, s3, s4;
	float r, r1, p1, p2, f, g;
	int v, tl;
	int i, j, k, l;

	//first: upscale sample to a power of 2 size
	tl=1;
	while(sl>tl)tl<<=1;

	tb=bgbmid_malloc(tl*sizeof(float));
	tb1=bgbmid_malloc(tl*sizeof(float));

	r=(((float)(sl-1))/((float)tl));
	for(i=0; i<tl; i++)
	{
		p1=i*r; j=floor(p1); p2=p1-j;
		s1=s[j]; s2=s[j+1];
		f=(s1*(1-p2))+(s2*p2);
		tb[i]=f;
	}

	//second: downscale by powers of 2

	while(tl>dl)
	{
		tl>>=1;
		for(j=0; j<tl; j++)
		{
			f=(tb[j*2+0]+tb[j*2+1])/2;
			tb1[j]=f;
		}

		tb2=tb; tb=tb1; tb1=tb2;
	}

	if(tl==dl)
	{
		//temp len == dest len
		for(i=0; i<dl; i++)
		{
			v=tb[i];
			if(v<(-32767))v=-32767;
			if(v>32767)v=32767;
			d[i]=v;
		}
		return(0);
	}

	//use multilinear upsample filter
	r=(((float)tl)/((float)dl));
	r1=(((float)sl)/((float)dl));
	for(i=0; i<dl; i++)
	{
		p1=i*r; j=floor(p1); p2=p1-j;

		k=floor((i*r*2)+0.5);
		l=floor(i*r1+0.5);

		s1=tb[j];	s2=tb[j+1];
		s3=tb1[k];	s4=s[l];
//		v=((s1*(1-p2))+(s2*p2))*0.666666+(s3*0.333333);
//		v=((s1*(1-p2))+(s2*p2))*0.5+(s3*0.5);
		v=((s1*(1-p2))+(s2*p2))*0.5+(s3*0.66+s4*0.34)*0.5;

		if(v<(-32767))v=-32767;
		if(v>32767)v=32767;
		d[i]=v;
	}

	bgbmid_free(tb);
	bgbmid_free(tb1);

	return(0);
}

int BFLAC_ScaleSample(short *d, int dl, short *s, int sl)
{
	int i, j, k, s1, s2, v;
	float r, p1, p2;

	if(dl==sl)
	{
		for(i=0; i<sl; i++)d[i]=s[i];
		return(0);
	}

	//upsample, linear
	if(dl>sl)
	{
		r=(((float)(sl-1))/((float)dl));
		for(i=0; i<dl; i++)
		{
			p1=i*r;
			j=floor(p1);
			p2=p1-j;

			s1=s[j];
			s2=s[j+1];
			v=(s1*(1-p2))+(s2*p2);

			d[i]=v;
		}
		return(0);
	}

#if 0
	r=(((float)sl)/((float)dl));
	for(i=0; i<dl; i++)
	{
		j=floor(i*r);
		d[i]=s[j];
	}
	return(0);
#endif

	//downsample, trilinear
	BFLAC_ScaleSampleTrilinear(d, dl, s, sl);
}

short *BFLAC_DecStream16(byte *buf, int sz,
	int *rch, int *rrt, int *rbps, int *rlen)
{
	int *sb0;
	short *sb1;
	int ch, rt, bps, len;
	int i;

	sb0=BFLAC_DecStream32(buf, sz, &ch, &rt, &bps, &len);
	if(!sb0)return(NULL);

	sb1=bgbmid_malloc(ch*len*sizeof(short));
	switch(bps)
	{
	case 8: for(i=0; i<len; i++)sb1[i]=sb0[i]*256;
	case 12: for(i=0; i<len; i++)sb1[i]=sb0[i]*16;
	case 16: for(i=0; i<len; i++)sb1[i]=sb0[i];
	case 20: for(i=0; i<len; i++)sb1[i]=sb0[i]/16;
	case 24: for(i=0; i<len; i++)sb1[i]=sb0[i]/256;
	default: break;
	}

	bgbmid_free(sb0);

	bps=16;

	if(rch)*rch=ch;
	if(rrt)*rrt=rt;
	if(rbps)*rbps=bps;
	if(rlen)*rlen=len;

	return(sb1);
}

short *BFLAC_DecStream_Mono16(byte *buf, int sz, int *rrt, int *rlen)
{
	short *bp, *bp1;
	int ch, rt, bts, len;
	int i, j, k;

	bp=BFLAC_DecStream16(buf, sz, &ch, &rt, &bts, &len);
	if(!bp)return(NULL);

	if(ch<1)
	{
		bgbmid_free(bp);
		bflac_error("BFLAC_DecStream_Mono16: Bad number of channels\n");
		return(NULL);
	}

	if(ch>1)
	{
		bp1=bgbmid_malloc(len*sizeof(short));
		for(i=0; i<len; i++)
		{
			k=0;
			for(j=0; j<ch; j++)
				k+=bp[i*ch+j];
			if(k<-32767)k=-32767;
			if(k>32767)k=32767;
			bp1[i]=k;
		}

		bgbmid_free(bp);
		bp=bp1;
	}

	*rrt=rt;
	*rlen=len;
	return(bp);
}

short *BFLAC_DecStream_RateMono16(byte *buf, int sz, int drt, int *rlen)
{
	short *bp, *bp1;
	int rt, len;
	int i, j, k;

	bp=BFLAC_DecStream_Mono16(buf, sz, &rt, &len);
	if(!bp)return(NULL);

	if(rt!=drt)
	{
		i=len*(((float)drt)/rt);
		bp1=bgbmid_malloc(i*sizeof(short));
		BFLAC_ScaleSample(bp1, i, bp, len);
		bgbmid_free(bp); bp=bp1; len=i;
	}

	*rlen=len;
	return(bp);
}

short *BFLAC_DecStream_44Mono16(byte *buf, int sz, int *rlen)
	{ return(BFLAC_DecStream_RateMono16(buf, sz, 44100, rlen)); }
short *BFLAC_DecStream_16Mono16(byte *buf, int sz, int *rlen)
	{ return(BFLAC_DecStream_RateMono16(buf, sz, 16000, rlen)); }

byte *BFLAC_EncStream16(short *sbuf, int *rsz,
	int chan, int rate, int len)
{
	byte *buf;
	int *sb;
	int i;

	sb=bgbmid_malloc(chan*len*sizeof(int));
	for(i=0; i<(chan*len); i++)sb[i]=sbuf[i];

	buf=BFLAC_EncStream32(sb, rsz, chan, rate, 16, len);
	bgbmid_free(sb);
	return(buf);
}

byte *BFLAC_EncStream8(byte *sbuf, int *rsz,
	int chan, int rate, int len)
{
	byte *buf;
	int *sb;
	int i;

	sb=bgbmid_malloc(chan*len*sizeof(int));
	for(i=0; i<(chan*len); i++)sb[i]=sbuf[i]-128;

	buf=BFLAC_EncStream32(sb, rsz, chan, rate, 8, len);
	bgbmid_free(sb);
	return(buf);
}

#if 0
PDGL_Sample *PDGL_Sound_LoadFLAC(char *name)
{
	VFILE *fd;
	PDGL_Sample *tmp;

	int i, j;
	byte *ibuf, *obuf;
	int len, sz;

	printf("PDGL_Sound_LoadFLAC: Loading sample '%s'\n", name);

	fd=vffopen(name, "rb");

	if(!fd)
	{
		printf("PDGL_Sound_LoadFLAC: Failed open sample '%s'\n", name);
		return(NULL);
	}


	ibuf=bgbmid_malloc(1<<20);
	sz=vfread(ibuf, 1, 1<<20, fd);

	obuf=(byte *)BFLAC_DecStream_44Mono16(ibuf, sz, &len);
	if(!obuf)
	{
		printf("PDGL_Sound_LoadFLAC: Failed decode sample '%s'\n", name);
		bgbmid_free(ibuf);
		return(NULL);
	}

	tmp=PDGL_Sound_SampleFromBuf(name, obuf, 1, 44100, 16, len);

	bgbmid_free(ibuf);
	bgbmid_free(obuf);

	return(tmp);
}
#endif

#if 0
int main(int argc, char *argv[])
{
	FILE *fd;
	int *sbuf;
	byte *buf;
	int i, j, sz, ch, rt, bits, len;

	fd=fopen(argv[1], "rb");
	if(!fd)return(-1);
	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);

	buf=bgbmid_malloc(sz);
	fread(buf, 1, sz, fd);
	fclose(fd);


	sbuf=BFLAC_DecStream32(buf, sz, &ch, &rt, &bits, &len);
	bgbmid_free(buf);

	fd=fopen("out.pcm", "wb");
	for(i=0; i<(ch*len); i++)
	{
		fputc(sbuf[i], fd);
		fputc(sbuf[i]>>8, fd);
	}

	fclose(fd);

	if(!strcmp(argv[1], "t_out.flac"))
		return(0);

	buf=BFLAC_EncStream32(sbuf, &sz, ch, rt, bits, len);
	fd=fopen("t_out.flac", "wb");

	if(fd)
	{
		fwrite(buf, 1, sz, fd);
		fclose(fd);
	}
}
#endif

