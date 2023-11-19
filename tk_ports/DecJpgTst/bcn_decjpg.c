/*
BGB Extensions:
APP11: BGBTech Tag
	FF,APP11,xx,xx,<ASCIZ TagName>, ...
	Tag-specific data until next marker.
	
	"AlphaColor":
		AlphaColor
		RGBA as string ("red green blue alpha").
		
		Specifies a "Alpha Color" extension, where any color sufficiently
		close to the given color (determined by 'alpha'), will be assumed
		to be a transparent pixel. These values are given in decimal.

	APP11 markers may indicate component layer:
		FF,APP11,xx,xx,"CompLayer\0", <layername:ASCIZ>
			"RGB": Base RGB
			"XYZ": Normal XYZ
			"SpRGB": Specular RGB
			"DASe": Depth, Alpha, Specular-Exponent
			"LuRGB": Luma RGB
			"Alpha": Mono alpha layer

	Component Layouts:
		3 component: (no marker, RGB)
		4 component: RGB+Alpha
		7 component: RGB+Alpha+LuRGB
		8 component: RGB+XYZ+DASe
		12 component: RGB+XYZ+SpRGB+DASe
		16 component: RGB+XYZ+SpRGB+DASe+LuRGB

	The APP11 tag for a layer will be followed by the layer
	(as a JPEG image), which may be placed within LDAT tags.
	
	Images lacking DHT and DQT will reuse those from the base image.

APP12: BGBTech Tag 2
	FF,APP12,xx,xx,tt,tt,tt,tt
		tt=FOURCC Tag.

	'LDAT': Layer Data.
	Gives an image/layer spread across a span of 'LDAT' tags.
	The encoded image will be split linearly across the tags.

APP11,"TagLayer":
	Allows giving a layer name to a collection of component layers.
	All images within a tag-layer will need to be the same resolution.
	Different taglayers may have different resolutions.
*/

#define DCTSZ	8
#define DCTSZ2	64

#define JPG_SOF0	0xC0
#define JPG_SOF1	0xC1
#define JPG_SOF2	0xC2
#define JPG_SOF3	0xC3
#define JPG_DHT		0xC4
#define JPG_SOF5	0xC5
#define JPG_SOF6	0xC6
#define JPG_SOF7	0xC7
#define JPG_JPG		0xC8
#define JPG_SOF9	0xC9
#define JPG_SOF10	0xCA
#define JPG_SOF11	0xCB
#define JPG_DAC		0xCC
#define JPG_SOF13	0xCD
#define JPG_SOF14	0xCE
#define JPG_SOF15	0xCF

#define JPG_RST0	0xD0
#define JPG_RST1	0xD1
#define JPG_RST2	0xD2
#define JPG_RST3	0xD3
#define JPG_RST4	0xD4
#define JPG_RST5	0xD5
#define JPG_RST6	0xD6
#define JPG_RST7	0xD7

#define JPG_SOI		0xD8
#define JPG_EOI		0xD9
#define JPG_SOS		0xDA
#define JPG_DQT		0xDB
#define JPG_DNL		0xDC
#define JPG_DRI		0xDD
#define JPG_DHP		0xDE
#define JPG_EXP		0xDF

#define JPG_APP0	0xE0
#define JPG_APP1	0xE1
#define JPG_APP2	0xE2
#define JPG_APP3	0xE3
#define JPG_APP4	0xE4
#define JPG_APP5	0xE5
#define JPG_APP6	0xE6
#define JPG_APP7	0xE7
#define JPG_APP8	0xE8
#define JPG_APP9	0xE9
#define JPG_APP10	0xEA
#define JPG_APP11	0xEB
#define JPG_APP12	0xEC
#define JPG_APP13	0xED
#define JPG_APP14	0xEE
#define JPG_APP15	0xEF

#define JPG_JPG0	0xF0
#define JPG_JPG1	0xF1
#define JPG_JPG2	0xF2
#define JPG_JPG3	0xF3
#define JPG_JPG4	0xF4
#define JPG_JPG5	0xF5
#define JPG_JPG6	0xF6
#define JPG_JPG7	0xF7
#define JPG_JPG8	0xF8
#define JPG_JPG9	0xF9
#define JPG_JPG10	0xFA
#define JPG_JPG11	0xFB
#define JPG_JPG12	0xFC
#define JPG_JPG13	0xFD
#define JPG_COM		0xFE

static const char *pdjpg_marker[]={
"SOF0", "SOF1", "SOF2", "SOF3", "DHT", "SOF5", "SOF6", "SOF7",
"JPG", "SOF9", "SOF10", "SOF11", "DAC", "SOF13", "SOF14", "SOF15",
"RST0", "RST1", "RST2", "RST3", "RST4", "RST5", "RST6", "RST7",
"SOI", "EOI", "SOS", "DQT", "DNL", "DRI", "DHP", "EXP",
"APP0", "APP1", "APP2", "APP3", "APP4", "APP5", "APP6", "APP7", 
"APP8", "APP9", "APP10", "APP11", "APP12", "APP13", "APP14", "APP15", 
"JPG0", "JPG1", "JPG2", "JPG3", "JPG4", "JPG5", "JPG6", "JPG7", 
"JPG8", "JPG9", "JPG10", "JPG11", "JPG12", "JPG13", "COM", ""
};

static const int pdjpg_zigzag[64]={
 0,  1,  5,  6, 14, 15, 27, 28,
 2,  4,  7, 13, 16, 26, 29, 42,
 3,  8, 12, 17, 25, 30, 41, 43,
 9, 11, 18, 24, 31, 40, 44, 53,
10, 19, 23, 32, 39, 45, 52, 54,
20, 22, 33, 38, 46, 51, 55, 60,
21, 34, 37, 47, 50, 56, 59, 61,
35, 36, 48, 49, 57, 58, 62, 63
};

static const int pdjpg_zigzag2[64]={
 0,  1,  8, 16,  9,  2,  3, 10,
17, 24, 32, 25, 18, 11,  4,  5,
12, 19, 26, 33, 40, 48, 41, 34,
27, 20, 13,  6,  7, 14, 21, 28,
35, 42, 49, 56, 57, 50, 43, 36,
29, 22, 15, 23, 30, 37, 44, 51,
58, 59, 52, 45, 38, 31, 39, 46,
53, 60, 61, 54, 47, 55, 62, 63
};

// static float pdjpg_exptab[256];
// static float pdjpg_scltab[256];

typedef struct PDJPG_Context_s PDJPG_Context;

struct PDJPG_Context_s {
byte *huff_cs;	//current pos in bitstream (input)
byte *huff_ct;	//current pos in bitstream (output)
uint huff_win;	//bitstream window
int huff_pos;	//bitstream offset
int huff_isend;	//bitstream has broken (decoder)

u16 huff_code[8*256];
u16 huff_mask[8*256];	//huffman code masks
s16 huff_next[8*256];	//huffman code next
byte huff_len[8*256];
s16 huff_idx[8*256];

byte jpg_qt[4][64];

int xs, ys;
int lxs, lys;
byte jpg_cid[16];
byte jpg_ch[16];
byte jpg_cv[16];
byte jpg_qid[16];
int jpg_cxi[16];
int jpg_cyi[16];
int jpg_nc;
int jpg_chm;
int jpg_chn;

int jpg_scid[4];
int jpg_scn[4];
byte *jpg_sibuf[4];
byte *jpg_sabuf;

int jpg_schs[4];		//scan horizontal size
int jpg_scvs[4];		//scan vertical size

int jpg_schsci[4];		//scan horizontal scale
int jpg_scvsci[4];		//scan vertical scale

byte jpg_is420;			//image is 420
byte jpg_is444;			//image is 444
byte jpg_rdct;			//image uses RDCT
};

#ifdef __BJX2__

int		__int_clamp(int v, int m, int n);
int		__int_mulsw(int a, int b);

#define	clamp255(v)		__int_clamp(v, 0, 255)
// #define	mulsw(a, b)		__int_mulsw(a, b)
#define	mulsw(a, b)		((a)*(b))

#else

int clamp255(int v)
{
	int v1;
	v1=v;
	if(v<0)
		v1=0;
	if(v>255)
		v1=255;
	return(v1);
}

#define	mulsw(a, b)		((a)*(b))

#endif


PDJPG_Context *PDJPG_AllocContext()
{
	PDJPG_Context *ctx;
	
	ctx=malloc(sizeof(PDJPG_Context));
	memset(ctx, 0, sizeof(PDJPG_Context));
	return(ctx);
}

void PDJPG_FreeContext(PDJPG_Context *ctx)
{
	int i;

	for(i=0; i<4; i++)
		if(ctx->jpg_sibuf[i])
			free(ctx->jpg_sibuf[i]);

	free(ctx);
}

void PDJPG_Init()
{
	static int init=0;
	int i, j;
	float f, g;

	if(init)return;
	init=1;
}

void PDJPG_TransIDCT_Horiz(short *iblk, int *oblk)
{
	int a, b, c, d, e, f;
	int i, j, k, l;
	int m, n, o, p;
	int t0, t1, t2, t3;
//	short t0, t1, t2, t3;

	t0=iblk[0];	t1=iblk[4];	t2=iblk[2];	t3=iblk[6];
	a=mulsw(t0, 91);	b=mulsw(t1, 91);
	c=mulsw(t2,118);	d=mulsw(t2, 49);
	e=mulsw(t3,118);	f=mulsw(t3, 49);
//	i=a+c+b+f;	j=a+d-b-e;	k=a-d-b+e;	l=a-c+b-f;
	t0=a+b; t1=a-b; t2=c+f; t3=d-e;
	i=t0+t2;	j=t1+t3;
	k=t1-t3;	l=t0-t2;
	t0=iblk[1];	t1=iblk[3];	t2=iblk[5];	t3=iblk[7];
//	m=mulsw(t0,126)+mulsw(t1,106)+mulsw(t2, 71)+mulsw(t3, 25);
//	n=mulsw(t0,106)-mulsw(t1, 25)-mulsw(t2,126)-mulsw(t3, 71);
//	o=mulsw(t0, 71)-mulsw(t1,126)+mulsw(t2, 25)+mulsw(t3,106);
//	p=mulsw(t0, 25)-mulsw(t1, 71)+mulsw(t2,106)-mulsw(t3,126);

	a=t0*126;	b=t1*106;	c=t2* 71;	d=t3* 25;	m=a+b+c+d;
	a=t0*106;	b=t1* 25;	c=t2*126;	d=t3* 71;	n=a-b-c-d;
	a=t0* 71;	b=t1*126;	c=t2* 25;	d=t3*106;	o=a-b+c+d;
	a=t0* 25;	b=t1* 71;	c=t2*106;	d=t3*126;	p=a-b+c-d;

	oblk[0]=i+m;	oblk[1]=j+n;	oblk[2]=k+o;	oblk[3]=l+p;
	oblk[4]=l-p;	oblk[5]=k-o;	oblk[6]=j-n;	oblk[7]=i-m;
}

void PDJPG_TransIDCT_Vert(int *iblk, int *oblk)
{
	int a, b, c, d, e, f;
	int i, j, k, l;
	int m, n, o, p;
	int t0, t1, t2, t3;

	t0=iblk[0];	t1=iblk[32];	t2=iblk[16];	t3=iblk[48];
	a=t0* 91;	b=t1*91;
	c=t2*118;	d=t2*49;
	e=t3*118;	f=t3*49;

//	i=a+c+b+f;	j=a+d-b-e;	k=a-d-b+e;	l=a-c+b-f;
	t0=a+b; t1=a-b; t2=c+f; t3=d-e;
	i=t0+t2;  j=t1+t3;
	k=t1-t3;  l=t0-t2;

	t0=iblk[8];	t1=iblk[24];	t2=iblk[40];	t3=iblk[56];
	m=t0*126+t1*106+t2* 71+t3* 25;
	n=t0*106-t1* 25-t2*126-t3* 71;
	o=t0* 71-t1*126+t2* 25+t3*106;
	p=t0* 25-t1* 71+t2*106-t3*126;
	oblk[ 0]=i+m;	oblk[ 8]=j+n;	oblk[16]=k+o;	oblk[24]=l+p;
	oblk[32]=l-p;	oblk[40]=k-o;	oblk[48]=j-n;	oblk[56]=i-m;
}

void BGBBTJ_JPG_TransIDCT_Vert2(int *iblk, short *oblk, int dcbias)
{
	int a, b, c, d, e, f;
	int i, j, k, l;
	int m, n, o, p;
	int t0, t1, t2, t3;

	t0=iblk[0];	t1=iblk[32];	t2=iblk[16];	t3=iblk[48];
	a=t0* 91;	b=t1*91;
	c=t2*118;	d=t2*49;
	e=t3*118;	f=t3*49;
//	a=iblk[ 0]* 91;	b=iblk[32]*91;
//	c=iblk[16]*118;	d=iblk[16]*49;
//	e=iblk[48]*118;	f=iblk[48]*49;

//	i=a+c+b+f;	j=a+d-b-e;	k=a-d-b+e;	l=a-c+b-f;
	t0=a+b; t1=a-b; t2=c+f; t3=d-e;
	i=t0+t2;  j=t1+t3;
	k=t1-t3;  l=t0-t2;

	t0=iblk[8];	t1=iblk[24];	t2=iblk[40];	t3=iblk[56];
//	m=t0*126+t1*106+t2* 71+t3* 25;
//	n=t0*106-t1* 25-t2*126-t3* 71;
//	o=t0* 71-t1*126+t2* 25+t3*106;
//	p=t0* 25-t1* 71+t2*106-t3*126;

	a=t0*126;	b=t1*106;	c=t2* 71;	d=t3* 25;	m=a+b+c+d;
	a=t0*106;	b=t1* 25;	c=t2*126;	d=t3* 71;	n=a-b-c-d;
	a=t0* 71;	b=t1*126;	c=t2* 25;	d=t3*106;	o=a-b+c+d;
	a=t0* 25;	b=t1* 71;	c=t2*106;	d=t3*126;	p=a-b+c-d;

//	m=iblk[ 8]*126+iblk[24]*106+iblk[40]* 71+iblk[56]* 25;
//	n=iblk[ 8]*106-iblk[24]* 25-iblk[40]*126-iblk[56]* 71;
//	o=iblk[ 8]* 71-iblk[24]*126+iblk[40]* 25+iblk[56]*106;
//	p=iblk[ 8]* 25-iblk[24]* 71+iblk[40]*106-iblk[56]*126;
	oblk[ 0]=((i+m)>>16)+dcbias;	oblk[ 8]=((j+n)>>16)+dcbias;
	oblk[16]=((k+o)>>16)+dcbias;	oblk[24]=((l+p)>>16)+dcbias;
	oblk[32]=((l-p)>>16)+dcbias;	oblk[40]=((k-o)>>16)+dcbias;
	oblk[48]=((j-n)>>16)+dcbias;	oblk[56]=((i-m)>>16)+dcbias;
}

// void PDJPG_TransIDCT(short *iblk, byte *oblk)
void PDJPG_TransIDCT(short *iblk, short *oblk, int dcbias, int rtm)
{
	int s[DCTSZ2];
	int t[DCTSZ2];
	short *ob, *obe;
	int i, j, k;

	if(rtm==2)
	{
		/* If DCT block is a flat color, fill directly */
		k=((iblk[0]*8281+32768)>>16)+dcbias;
		ob=oblk; obe=ob+64;
		while(ob<obe)
		{
			ob[ 0]=k; ob[ 1]=k; ob[ 2]=k; ob[ 3]=k;
			ob[ 4]=k; ob[ 5]=k; ob[ 6]=k; ob[ 7]=k;
			ob[ 8]=k; ob[ 9]=k; ob[10]=k; ob[11]=k;
			ob[12]=k; ob[13]=k; ob[14]=k; ob[15]=k;
			ob+=16;
		}
		return;
	}

	PDJPG_TransIDCT_Horiz(iblk+0, s+0);
	PDJPG_TransIDCT_Horiz(iblk+8, s+8);
	PDJPG_TransIDCT_Horiz(iblk+16, s+16);
	PDJPG_TransIDCT_Horiz(iblk+24, s+24);
	PDJPG_TransIDCT_Horiz(iblk+32, s+32);
	PDJPG_TransIDCT_Horiz(iblk+40, s+40);
	PDJPG_TransIDCT_Horiz(iblk+48, s+48);
	PDJPG_TransIDCT_Horiz(iblk+56, s+56);

#if 1
	BGBBTJ_JPG_TransIDCT_Vert2(s+0, oblk+0, dcbias);
	BGBBTJ_JPG_TransIDCT_Vert2(s+1, oblk+1, dcbias);
	BGBBTJ_JPG_TransIDCT_Vert2(s+2, oblk+2, dcbias);
	BGBBTJ_JPG_TransIDCT_Vert2(s+3, oblk+3, dcbias);
	BGBBTJ_JPG_TransIDCT_Vert2(s+4, oblk+4, dcbias);
	BGBBTJ_JPG_TransIDCT_Vert2(s+5, oblk+5, dcbias);
	BGBBTJ_JPG_TransIDCT_Vert2(s+6, oblk+6, dcbias);
	BGBBTJ_JPG_TransIDCT_Vert2(s+7, oblk+7, dcbias);
#endif

#if 0
	PDJPG_TransIDCT_Vert(s+0, t+0);
	PDJPG_TransIDCT_Vert(s+1, t+1);
	PDJPG_TransIDCT_Vert(s+2, t+2);
	PDJPG_TransIDCT_Vert(s+3, t+3);
	PDJPG_TransIDCT_Vert(s+4, t+4);
	PDJPG_TransIDCT_Vert(s+5, t+5);
	PDJPG_TransIDCT_Vert(s+6, t+6);
	PDJPG_TransIDCT_Vert(s+7, t+7);

	for(i=0; i<64; i++)
	{
		j=(t[i]>>16)+dcbias;
		oblk[i]=j;
	}
#endif
}

#define FLTOFIX14(x)	((int)((x)*8192+0.5))
#define FIX14TOI(x)	(((x)+4096)>>13)

#if 1
void PDJPG_TransIRDCT_Horiz(int *iblk, int *oblk)
{
	int ib[8];

	ib[0]=iblk[6];	ib[1]=iblk[7];
	ib[2]=iblk[3];	ib[3]=iblk[0];
	ib[4]=iblk[2];	ib[5]=iblk[4];
	ib[6]=iblk[1];	ib[7]=iblk[5];

	ib[7]=ib[7]-FIX14TOI(
		ib[0]*FLTOFIX14(1.1020)+		ib[1]*FLTOFIX14(-2.0306)+
		ib[2]*FLTOFIX14(-0.3881)+		ib[3]*FLTOFIX14(0.6561)+
		ib[4]*FLTOFIX14(1.2405)+		ib[5]*FLTOFIX14(1.6577)+
		ib[6]*FLTOFIX14(-1.1914));
	ib[6]=ib[6]-FIX14TOI(
		ib[0]*FLTOFIX14(1.0024)+		ib[1]*FLTOFIX14(-0.7180)+
		ib[2]*FLTOFIX14(-0.0928)+		ib[3]*FLTOFIX14(-0.0318)+
		ib[4]*FLTOFIX14(0.4170)+		ib[5]*FLTOFIX14(1.1665)+
		ib[7]*FLTOFIX14(0.4904));
	ib[5]=ib[5]-FIX14TOI(
		ib[0]*FLTOFIX14(-0.6573)+		ib[1]*FLTOFIX14(0.5810)+
		ib[2]*FLTOFIX14(-0.2931)+		ib[3]*FLTOFIX14(-0.5307)+
		ib[4]*FLTOFIX14(-0.8730)+		ib[6]*FLTOFIX14(-0.1594)+
		ib[7]*FLTOFIX14(-0.3560));
	ib[4]=ib[4]-FIX14TOI(
		ib[0]*FLTOFIX14(0.4591)+		ib[1]*FLTOFIX14(0.4108)+
		ib[2]*FLTOFIX14(-0.2073)+		ib[3]*FLTOFIX14(-1.0824)+
		ib[5]*FLTOFIX14(0.7071)+		ib[6]*FLTOFIX14(0.8873)+
		ib[7]*FLTOFIX14(-0.2517));
	ib[3]=ib[3]-FIX14TOI(
		ib[0]*FLTOFIX14(0.7957)+		ib[1]*FLTOFIX14(0.9664)+
		ib[2]*FLTOFIX14(0.4439)+		ib[4]*FLTOFIX14(0.6173)+
		ib[5]*FLTOFIX14(-0.1422)+		ib[6]*FLTOFIX14(1.0378)+
		ib[7]*FLTOFIX14(-0.1700));
	ib[2]=ib[2]-FIX14TOI(
		ib[0]*FLTOFIX14(-0.0364)+		ib[1]*FLTOFIX14(-1.7104)+
		ib[3]*FLTOFIX14(-1.0000)+		ib[4]*FLTOFIX14(0.3066)+
		ib[5]*FLTOFIX14(0.6671)+		ib[6]*FLTOFIX14(-0.5953)+
		ib[7]*FLTOFIX14(0.2039));
	ib[1]=ib[1]-FIX14TOI(
		ib[0]*FLTOFIX14(-0.0685)+		ib[2]*FLTOFIX14(0.2708)+
		ib[3]*FLTOFIX14(-0.2708)+		ib[4]*FLTOFIX14(-0.2235)+
		ib[5]*FLTOFIX14(0.2568)+		ib[6]*FLTOFIX14(-0.3205)+
		ib[7]*FLTOFIX14(0.3841));
	ib[0]=ib[0]-FIX14TOI(
		ib[1]*FLTOFIX14(-1.1129)+		ib[2]*FLTOFIX14(0.0570)+
		ib[3]*FLTOFIX14(-0.4712)+		ib[4]*FLTOFIX14(0.1029)+
		ib[5]*FLTOFIX14(0.0156)+		ib[6]*FLTOFIX14(-0.4486)+
		ib[7]*FLTOFIX14(-0.4619));
	ib[7]=-ib[7]+FIX14TOI(
		ib[0]*FLTOFIX14(1.1648)+		ib[1]*FLTOFIX14(-2.8234)+
		ib[2]*FLTOFIX14(0.5375)+		ib[3]*FLTOFIX14(-0.6058)+
		ib[4]*FLTOFIX14(1.2228)+		ib[5]*FLTOFIX14(-0.3805)+
		ib[6]*FLTOFIX14(0.0288));

	oblk[0]=ib[5];	oblk[1]=ib[6];
	oblk[2]=ib[0];	oblk[3]=ib[3];
	oblk[4]=ib[2];	oblk[5]=ib[7];
	oblk[6]=ib[4];	oblk[7]=ib[1];
}

void PDJPG_TransIRDCT_Vert(short *iblk, int *oblk)
{
	int ib[8];

	ib[0]=iblk[48];	ib[1]=iblk[56];
	ib[2]=iblk[24];	ib[3]=iblk[ 0];
	ib[4]=iblk[16];	ib[5]=iblk[32];
	ib[6]=iblk[ 8];	ib[7]=iblk[40];

	ib[7]=ib[7]-FIX14TOI(
		ib[0]*FLTOFIX14(1.1020)+		ib[1]*FLTOFIX14(-2.0306)+
		ib[2]*FLTOFIX14(-0.3881)+		ib[3]*FLTOFIX14(0.6561)+
		ib[4]*FLTOFIX14(1.2405)+		ib[5]*FLTOFIX14(1.6577)+
		ib[6]*FLTOFIX14(-1.1914));
	ib[6]=ib[6]-FIX14TOI(
		ib[0]*FLTOFIX14(1.0024)+		ib[1]*FLTOFIX14(-0.7180)+
		ib[2]*FLTOFIX14(-0.0928)+		ib[3]*FLTOFIX14(-0.0318)+
		ib[4]*FLTOFIX14(0.4170)+		ib[5]*FLTOFIX14(1.1665)+
		ib[7]*FLTOFIX14(0.4904));
	ib[5]=ib[5]-FIX14TOI(
		ib[0]*FLTOFIX14(-0.6573)+		ib[1]*FLTOFIX14(0.5810)+
		ib[2]*FLTOFIX14(-0.2931)+		ib[3]*FLTOFIX14(-0.5307)+
		ib[4]*FLTOFIX14(-0.8730)+		ib[6]*FLTOFIX14(-0.1594)+
		ib[7]*FLTOFIX14(-0.3560));
	ib[4]=ib[4]-FIX14TOI(
		ib[0]*FLTOFIX14(0.4591)+		ib[1]*FLTOFIX14(0.4108)+
		ib[2]*FLTOFIX14(-0.2073)+		ib[3]*FLTOFIX14(-1.0824)+
		ib[5]*FLTOFIX14(0.7071)+		ib[6]*FLTOFIX14(0.8873)+
		ib[7]*FLTOFIX14(-0.2517));
	ib[3]=ib[3]-FIX14TOI(
		ib[0]*FLTOFIX14(0.7957)+		ib[1]*FLTOFIX14(0.9664)+
		ib[2]*FLTOFIX14(0.4439)+		ib[4]*FLTOFIX14(0.6173)+
		ib[5]*FLTOFIX14(-0.1422)+		ib[6]*FLTOFIX14(1.0378)+
		ib[7]*FLTOFIX14(-0.1700));
	ib[2]=ib[2]-FIX14TOI(
		ib[0]*FLTOFIX14(-0.0364)+		ib[1]*FLTOFIX14(-1.7104)+
		ib[3]*FLTOFIX14(-1.0000)+		ib[4]*FLTOFIX14(0.3066)+
		ib[5]*FLTOFIX14(0.6671)+		ib[6]*FLTOFIX14(-0.5953)+
		ib[7]*FLTOFIX14(0.2039));
	ib[1]=ib[1]-FIX14TOI(
		ib[0]*FLTOFIX14(-0.0685)+		ib[2]*FLTOFIX14(0.2708)+
		ib[3]*FLTOFIX14(-0.2708)+		ib[4]*FLTOFIX14(-0.2235)+
		ib[5]*FLTOFIX14(0.2568)+		ib[6]*FLTOFIX14(-0.3205)+
		ib[7]*FLTOFIX14(0.3841));
	ib[0]=ib[0]-FIX14TOI(
		ib[1]*FLTOFIX14(-1.1129)+		ib[2]*FLTOFIX14(0.0570)+
		ib[3]*FLTOFIX14(-0.4712)+		ib[4]*FLTOFIX14(0.1029)+
		ib[5]*FLTOFIX14(0.0156)+		ib[6]*FLTOFIX14(-0.4486)+
		ib[7]*FLTOFIX14(-0.4619));
	ib[7]=-ib[7]+FIX14TOI(
		ib[0]*FLTOFIX14(1.1648)+		ib[1]*FLTOFIX14(-2.8234)+
		ib[2]*FLTOFIX14(0.5375)+		ib[3]*FLTOFIX14(-0.6058)+
		ib[4]*FLTOFIX14(1.2228)+		ib[5]*FLTOFIX14(-0.3805)+
		ib[6]*FLTOFIX14(0.0288));

	oblk[ 0]=ib[5];	oblk[ 8]=ib[6];
	oblk[16]=ib[0];	oblk[24]=ib[3];
	oblk[32]=ib[2];	oblk[40]=ib[7];
	oblk[48]=ib[4];	oblk[56]=ib[1];
}

void PDJPG_TransIRDCT(short *iblk, short *oblk, int dcbias, int rtm)
{
	int s[DCTSZ2];
	int t[DCTSZ2];
	int i, j;

	PDJPG_TransIRDCT_Vert(iblk+0, s+0);
	PDJPG_TransIRDCT_Vert(iblk+1, s+1);
	PDJPG_TransIRDCT_Vert(iblk+2, s+2);
	PDJPG_TransIRDCT_Vert(iblk+3, s+3);
	PDJPG_TransIRDCT_Vert(iblk+4, s+4);
	PDJPG_TransIRDCT_Vert(iblk+5, s+5);
	PDJPG_TransIRDCT_Vert(iblk+6, s+6);
	PDJPG_TransIRDCT_Vert(iblk+7, s+7);

	PDJPG_TransIRDCT_Horiz(s+0, t+0);
	PDJPG_TransIRDCT_Horiz(s+8, t+8);
	PDJPG_TransIRDCT_Horiz(s+16, t+16);
	PDJPG_TransIRDCT_Horiz(s+24, t+24);
	PDJPG_TransIRDCT_Horiz(s+32, t+32);
	PDJPG_TransIRDCT_Horiz(s+40, t+40);
	PDJPG_TransIRDCT_Horiz(s+48, t+48);
	PDJPG_TransIRDCT_Horiz(s+56, t+56);

	for(i=0; i<64; i++)
	{
		j=(t[i])+dcbias;
		oblk[i]=j;
	}
}
#endif


//Decoder

int PDJHUFF_NextByte(PDJPG_Context *ctx)
{
	int i, j;

//	if(ctx->huff_isend)return(0x00);

	i=*ctx->huff_cs++;
	if(i==0xFF)
	{
		j=*ctx->huff_cs++;
		if(j)
		{
			ctx->huff_isend=1;
			return(0x00);
		}
	}
	return(i);
}

int PDJHUFF_InitStream(PDJPG_Context *ctx, byte *buf)
{
	ctx->huff_pos=0;
	ctx->huff_isend=0;

	ctx->huff_cs=buf;
	ctx->huff_win=PDJHUFF_NextByte(ctx);
	ctx->huff_win=(ctx->huff_win<<8)|PDJHUFF_NextByte(ctx);
	ctx->huff_win=(ctx->huff_win<<8)|PDJHUFF_NextByte(ctx);
	ctx->huff_win=(ctx->huff_win<<8)|PDJHUFF_NextByte(ctx);
	return(0);
}

int PDJHUFF_ReadBit(PDJPG_Context *ctx)
{
	int i;

	i=(ctx->huff_win>>(31-ctx->huff_pos))&1;
	ctx->huff_pos++;
	if(ctx->huff_pos>=8)
	{
		ctx->huff_win=(ctx->huff_win<<8)|PDJHUFF_NextByte(ctx);
		ctx->huff_pos-=8;
	}
	return(i);
}

int PDJHUFF_ReadNBits(PDJPG_Context *ctx, int n)
{
	int i;

	if(n<=0)return(0);

	i=(ctx->huff_win>>(32-n-ctx->huff_pos))&((1<<n)-1);
	ctx->huff_pos+=n;
	if(ctx->huff_pos>=8)
	{
		ctx->huff_win=(ctx->huff_win<<8)|PDJHUFF_NextByte(ctx);
		ctx->huff_pos-=8;
		while(ctx->huff_pos>=8)
		{
			ctx->huff_win=(ctx->huff_win<<8)|PDJHUFF_NextByte(ctx);
			ctx->huff_pos-=8;
		}
	}
	return(i);
}

void PDJHUFF_SkipNBits(PDJPG_Context *ctx, int n)
{
	ctx->huff_pos+=n;
	while(ctx->huff_pos>=8)
	{
		ctx->huff_win=(ctx->huff_win<<8)|PDJHUFF_NextByte(ctx);
		ctx->huff_pos-=8;
	}
}

int PDJHUFF_PeekWord(PDJPG_Context *ctx)
{
	int i;
	i=(ctx->huff_win>>(16-ctx->huff_pos))&65535;
	return(i);
}

int PDJHUFF_DecodeSymbol(PDJPG_Context *ctx, int tab)
{
	int i, j, k, l;

	i=(byte)(ctx->huff_win>>(24-ctx->huff_pos));
	
	tab<<=8;
	j=ctx->huff_idx[tab|i];

	k=ctx->huff_len[tab|j];
	if(k<=8)
	{
		ctx->huff_pos+=k;
		if(ctx->huff_pos>=8)
		{
			ctx->huff_win=(ctx->huff_win<<8)|PDJHUFF_NextByte(ctx);
			ctx->huff_pos-=8;
		}
		return(j);
	}

	i=(u16)(ctx->huff_win>>(16-ctx->huff_pos));
//	j=ctx->huff_idx[tab|(i>>8)];

	while(1)
	{
		l=tab|j;
		k=ctx->huff_len[l];
		if(!k)
			{ break; }

		if((i>>(16-k))!=ctx->huff_code[l])
		{
			j=ctx->huff_next[l];
			if(j<0)
				{ break; }
			continue;
		}

		ctx->huff_pos+=k;
		while(ctx->huff_pos>=8)
		{
			ctx->huff_win=(ctx->huff_win<<8)|PDJHUFF_NextByte(ctx);
			ctx->huff_pos-=8;
		}
		return(j);
	}
	return(-1);
}

int PDJHUFF_DecodeDiffDC(PDJPG_Context *ctx, int tab)
{
	int i, j;

	i=PDJHUFF_DecodeSymbol(ctx, tab);
	j=PDJHUFF_ReadNBits(ctx, i);
//	if(!(j&(1<<(i-1))))
//		j=(-1<<i)+j+1;
	j+=(~((j<<(32-i))>>31))&((-1<<i)+1);

	return(j);
}

int PDJHUFF_DecodeBlock(PDJPG_Context *ctx,
	short *buf, int dctab, int actab, int qid,
	int ni, int nn)
{
	byte *qtab;
	int i, j, k;

	memset(buf+1, 0, 63*sizeof(short));
	qtab=ctx->jpg_qt[qid];

	buf[0]+=PDJHUFF_DecodeDiffDC(ctx, dctab)*qtab[0];
	for(i=1; i<64; i++)
	{
		j=PDJHUFF_DecodeSymbol(ctx, actab);
		if(j<=0)
		{
			if(j<0)
			{
				printf("bad dct block1 %02X(%d) %d/%d\n", j, j, ni, nn);
				return(-1);
			}
			break;
		}

		i+=(j>>4)&15;	//preceding 0's
		if(!(j&15))
			continue;
		if(i>=64)
			{ break; }

		j&=15;
		k=PDJHUFF_ReadNBits(ctx, j);
//		if(!(k&(1<<(j-1))))
//			k=(-1<<j)+k+1;
		k+=(~((k<<(32-j))>>31))&((-1<<j)+1);

		j=pdjpg_zigzag2[i];
		buf[j]=k*qtab[j];
	}

	if(i>64)
	{
		printf("bad dct block2 %02X(%d) len=%d %d/%d\n", j, j, i, ni, nn);
		return(-1);
	}

	if(i==1)
		return(2);
	return(0);
}

void PDJPG_CopyOutBlock8B(
	PDJPG_Context *ctx, short *ibuf,
	byte *obuf, int xs, int ys,
	int x, int y)
{
	short *cs;
	byte *ct;
	int i0, i1, i2, i3, i4, i5, i6, i7;
	int i, j, k;
	
	if(((x+8)>xs) || ((y+8)>ys))
	{
		for(i=0; i<8; i++)
		{
			if((y+i)>=ys)
				continue;

			cs=ibuf+i*8;
			ct=obuf+(y+i)*xs+x;
			for(j=0; j<8; j++)
			{
				if((x+j)>=xs)
					continue;
				k=*cs++;
				if(k<0)k=0;
				if(k>255)k=255;
				*ct=k;
				ct++;
			}
		}
		return;
	}
	
	for(i=0; i<8; i++)
	{
		cs=ibuf+i*8;
		ct=obuf+(y+i)*xs+x;

#if 1
		ct[0]=clamp255(cs[0]);	ct[1]=clamp255(cs[1]);
		ct[2]=clamp255(cs[2]);	ct[3]=clamp255(cs[3]);
		ct[4]=clamp255(cs[4]);	ct[5]=clamp255(cs[5]);
		ct[6]=clamp255(cs[6]);	ct[7]=clamp255(cs[7]);
#endif

#if 0
		for(j=0; j<8; j++)
		{
			k=*cs++;
			if(k<0)k=0;
			if(k>255)k=255;
			*ct=k;
			ct++;
		}
#endif
	}
}

int PDJPG_MarkerSOS(PDJPG_Context *ctx, byte *buf)
{
	short dbuf[4*64];
	short dcblk[64];
	short diblk[64];
	byte ch[4], cv[4], qid[4];
	byte step[4];
	int xi[4], yi[4], wi[4], hi[4];
	byte cdt[4], cat[4];
	int ccnt[4];

	byte *sibuf[4];
	int siystr[4];
	
	byte *s;
	int i, j, k, l, i1, j1, k1, l1;
	int w, h, n, ns, rt;

	l=(buf[0]<<8)|buf[1];
//	printf("ns %d\n", buf[2]);

	ns=buf[2]; rt=0;
	s=buf+3;
	for(i=0; i<buf[2]; i++)
	{
//		printf("%d %d %d\n", s[0], s[1]>>4, s[1]&15);

		cdt[i]=s[1]>>4;
		cat[i]=s[1]&15;

		for(j=0; j<ctx->jpg_nc; j++)
			if(ctx->jpg_cid[j]==s[0])
		{
			ch[i]=ctx->jpg_ch[j];
			cv[i]=ctx->jpg_cv[j];
			qid[i]=ctx->jpg_qid[j];
			xi[i]=ctx->jpg_cxi[j];
			yi[i]=ctx->jpg_cyi[j];

			step[i]=ch[i]*cv[i];
			break;
		}

		w=(xi[i]+7)/8;
		h=(yi[i]+7)/8;
//		w=(xi[i]+15)/8;
//		h=(yi[i]+15)/8;
		wi[i]=w;
		hi[i]=h;

		n=(w+1)*(h+2);
		ctx->jpg_scid[i]=s[0];
		ctx->jpg_scn[i]=j;

		if(!ctx->jpg_sibuf[i])
			ctx->jpg_sibuf[i]=malloc(n*64);
		memset(ctx->jpg_sibuf[i], 0, n*64);

		sibuf[i]=ctx->jpg_sibuf[i];
//		siystr[i]=w*8;

		ccnt[i]=0;

		s+=2;
	}
	s+=3;

	w=(ctx->xs+ctx->jpg_chm*8-1)/(ctx->jpg_chm*8);
	h=(ctx->ys+ctx->jpg_chn*8-1)/(ctx->jpg_chn*8);
	n=w*h;

	PDJHUFF_InitStream(ctx, s);

	for(i=0; i<(4*64); i++)dbuf[i]=0;

	if(ctx->jpg_nc==1)
	{
		rt=0;
		for(i=0; i<h; i++)
		{
			for(j=0; j<w; j++)
			{
				rt=PDJHUFF_DecodeBlock(ctx, dbuf,
					cdt[0]*2+0, cat[0]*2+1, qid[0], i*w+j, n);
				if(rt<0)break;
				PDJPG_TransIDCT(dbuf, diblk, 128, rt);
				PDJPG_CopyOutBlock8B(ctx, diblk,
					sibuf[0], xi[0], yi[0], j*8, i*8);
			}
			if(rt<0)break;
		}
	}else
	if(ctx->jpg_is420 && !ctx->jpg_rdct)
	{
		rt=0;
		for(i=0; i<h; i++)
		{
			for(j=0; j<w; j++)
			{
				i1=i<<1;	j1=j<<1;

				k=((i1+0)*wi[0])+(j1+0);
				rt=PDJHUFF_DecodeBlock(ctx, dbuf,
					cdt[0]*2+0, cat[0]*2+1, qid[0], i*w+j, n);
				if(rt<0)break;
				PDJPG_TransIDCT(dbuf, diblk, 128, rt);
				PDJPG_CopyOutBlock8B(ctx, diblk,
					sibuf[0], xi[0], yi[0],
					8*(j1+0), ((i1+0)*8));

				k=((i1+0)*wi[0])+(j1+1);
				rt=PDJHUFF_DecodeBlock(ctx, dbuf,
					cdt[0]*2+0, cat[0]*2+1, qid[0], i*w+j, n);
				if(rt<0)break;
				PDJPG_TransIDCT(dbuf, diblk, 128, rt);
				PDJPG_CopyOutBlock8B(ctx, diblk,
					sibuf[0], xi[0], yi[0],
					8*(j1+1), ((i1+0)*8));

				k=((i1+1)*wi[0])+(j1+0);
				rt=PDJHUFF_DecodeBlock(ctx, dbuf,
					cdt[0]*2+0, cat[0]*2+1, qid[0], i*w+j, n);
				if(rt<0)break;
				PDJPG_TransIDCT(dbuf, diblk, 128, rt);
				PDJPG_CopyOutBlock8B(ctx, diblk,
					sibuf[0], xi[0], yi[0],
					8*(j1+0), ((i1+1)*8));

				k=((i1+1)*wi[0])+(j1+1);
				rt=PDJHUFF_DecodeBlock(ctx, dbuf,
					cdt[0]*2+0, cat[0]*2+1, qid[0], i*w+j, n);
				if(rt<0)break;
				PDJPG_TransIDCT(dbuf, diblk, 128, rt);
				PDJPG_CopyOutBlock8B(ctx, diblk,
					sibuf[0], xi[0], yi[0],
					8*(j1+1), ((i1+1)*8));

				rt=PDJHUFF_DecodeBlock(ctx, dbuf+1*64,
					cdt[1]*2+0, cat[1]*2+1, qid[1], i*w+j, n);
				if(rt<0)break;
				PDJPG_TransIDCT(dbuf+1*64, diblk, 128, rt);
				PDJPG_CopyOutBlock8B(ctx, diblk,
					sibuf[1], xi[1], yi[1], j*8, i*8);

				rt=PDJHUFF_DecodeBlock(ctx, dbuf+2*64,
					cdt[2]*2+0, cat[2]*2+1, qid[2], i*w+j, n);
				if(rt<0)break;
				PDJPG_TransIDCT(dbuf+2*64, diblk, 128, rt);
				PDJPG_CopyOutBlock8B(ctx, diblk,
					sibuf[2], xi[2], yi[2], j*8, i*8);
			}
			if(rt<0)break;
		}
	}else
	{
		rt=0;
		for(i=0; i<n; i++)
		{
			for(j=0; j<ns; j++)
			{
				for(k=0; k<cv[j]; k++)
				{
					for(l=0; l<ch[j]; l++)
					{
						if(rt<0)break;

						i1=(i/(wi[j]/ch[j]))*ch[j];
						j1=(i%(wi[j]/ch[j]))*ch[j];

						rt=PDJHUFF_DecodeBlock(ctx, dbuf+j*64,
							cdt[j]*2+0, cat[j]*2+1, qid[j], i, n);
						if(rt<0)break;

						if(ctx->jpg_rdct)
							{ PDJPG_TransIRDCT(dbuf+j*64, diblk, 128, rt); }
						else
							{ PDJPG_TransIDCT(dbuf+j*64, diblk, 128, rt); }
						PDJPG_CopyOutBlock8B(ctx, diblk,
							sibuf[j], xi[j], yi[j],
							8*(j1+l), ((i1+k)*8));
					}
					if(rt<0)break;
				}
				if(rt<0)break;
			}
			if(rt<0)break;
		}
	}

	if(rt<0)
	{
		printf("PDJPG_MarkerSOS: Bad Image\n");
		return(-1);
	}

	i=ctx->huff_cs-buf;
	return(i);

//	return(0);
}

int PDJPG_MarkerSOF0(PDJPG_Context *ctx, byte *buf)
{
	byte *cnt, *s;
	int i, j, k, l, m, n;

	l=(buf[0]<<8)|buf[1];

	i=(buf[3]<<8)|buf[4];
	j=(buf[5]<<8)|buf[6];

	ctx->xs=j;
	ctx->ys=i;

	m=0; n=0;

	k=buf[7];
	s=buf+8;
	for(i=0; i<k; i++)
	{
		ctx->jpg_cid[i]=s[0];
		ctx->jpg_ch[i]=s[1]>>4;
		ctx->jpg_cv[i]=s[1]&15;
		ctx->jpg_qid[i]=s[2];

		if(ctx->jpg_ch[i]>m)m=ctx->jpg_ch[i];
		if(ctx->jpg_cv[i]>n)n=ctx->jpg_cv[i];
		s+=3;
	}

	ctx->jpg_chm=m;
	ctx->jpg_chn=n;

	for(i=0; i<k; i++)
	{
		ctx->jpg_cxi[i]=(ctx->jpg_ch[i]*ctx->xs)/m;
		ctx->jpg_cyi[i]=(ctx->jpg_cv[i]*ctx->ys)/n;
	}
	ctx->jpg_nc=k;

#if 1
	for(i=0; i<ctx->jpg_nc; i++)
	{
		j=ctx->jpg_scn[i];

		ctx->jpg_schsci[i]=(65536*ctx->jpg_ch[j])/ctx->jpg_chm;
		ctx->jpg_scvsci[i]=(65536*ctx->jpg_cv[j])/ctx->jpg_chn;

		ctx->jpg_schs[i]=(ctx->jpg_cxi[j]+7)/8;
		ctx->jpg_scvs[i]=(ctx->jpg_cyi[j]+7)/8;
		
	}

	if(ctx->jpg_nc==3)
	{
		i=1;
		if(ctx->jpg_schsci[0]!=65536)i=0;
		if(ctx->jpg_scvsci[0]!=65536)i=0;
		if(ctx->jpg_schsci[1]!=32768)i=0;
		if(ctx->jpg_scvsci[1]!=32768)i=0;
		if(ctx->jpg_schsci[2]!=32768)i=0;
		if(ctx->jpg_scvsci[2]!=32768)i=0;
		ctx->jpg_is420=i;

		i=1;
		if(ctx->jpg_schsci[0]!=65536)i=0;
		if(ctx->jpg_scvsci[0]!=65536)i=0;
		if(ctx->jpg_schsci[1]!=65536)i=0;
		if(ctx->jpg_scvsci[1]!=65536)i=0;
		if(ctx->jpg_schsci[2]!=65536)i=0;
		if(ctx->jpg_scvsci[2]!=65536)i=0;
		ctx->jpg_is444=i;
	}else
	{
		ctx->jpg_is420=0;
		ctx->jpg_is444=0;
	}
#endif

	return(0);
}

int PDJPG_MarkerDQT(PDJPG_Context *ctx, byte *buf)
{
	byte *s, *se;
	int i, j, l;

	l=(buf[0]<<8)|buf[1];

	s=buf+2;
	se=buf+l;

	while(s<se)
	{
		i=(s[0]>>4)&15;
		if(i)
			{ return(-1); }
		i=s[0]&15;
		for(j=0; j<64; j++)
			ctx->jpg_qt[i][pdjpg_zigzag2[j]]=s[j+1];
		s+=65;
	}
	
	return(s-buf);
}

int PDJPG_MarkerDHT(PDJPG_Context *ctx, byte *buf)
{
	byte *cnt, *s;
	int i, j, k, l, tn;

	l=(buf[0]<<8)|buf[1];

	j=(buf[2]>>4)&15;
	i=buf[2]&15;
//	printf("dest %d, type %d\n", i, j);

	tn=i*2+j;

	for(i=0; i<256; i++)
	{
		ctx->huff_len[tn*256+i]=0;
		ctx->huff_next[tn*256+i]=-1;
		ctx->huff_idx[tn*256+i]=-1;
	}

	cnt=buf+3;
	s=cnt+16;
	k=0; l=0;
	for(i=0; i<16; i++)
	{
		k<<=1;
		for(j=0; j<cnt[i]; j++)
		{
			ctx->huff_code[tn*256+(*s)]=k++;
			ctx->huff_len[tn*256+(*s)]=i+1;
			ctx->huff_mask[tn*256+(*s)]=(1<<(i+1))-1;
			l=i+1;
			s++;
		}
	}
//	printf("%04X %d\n", k, l);

	//build lookup indices / chains

	for(i=0; i<256; i++)
	{
		ctx->huff_next[tn*256+i]=-1;
		ctx->huff_idx[tn*256+i]=-1;
	}

	for(i=0; i<256; i++)
	{
		l=ctx->huff_len[tn*256+i];
		if(!l)continue;
		j=ctx->huff_code[tn*256+i];
		
		if(l<=8)
		{
			j=j<<(8-l);
			k=1<<(8-l);
			while((k--) && (j<256))
			{
				ctx->huff_idx[tn*256+j]=i;
				j++;
			}
		}else
		{
			j=j>>(l-8);
			ctx->huff_next[tn*256+i]=
				ctx->huff_idx[tn*256+j];
			ctx->huff_idx[tn*256+j]=i;
		}
	}

	return(s-buf);
}

int PDJPG_MarkerAPP0(PDJPG_Context *ctx, byte *buf)
{
	return(0);
}

int PDJPG_MarkerAPP9(PDJPG_Context *ctx, byte *buf)
{
	int i;
	buf+=2;

	if(!memcmp(buf, "JPSERM", 6))
	{
		ctx->jpg_rdct=1;
		return(1);
	}
	
	return(0);
}

int PDJPG_MarkerAPP11(PDJPG_Context *ctx, byte *buf)
{
	int cr, cg, cb, ca;

	buf+=2;

#if 0
	if(!strcmp((char *)buf, "AlphaColor"))
	{
		buf+=strlen((char *)buf)+1;
		sscanf(buf, "%d %d %d %d", &cr, &cg, &cb, &ca);
		PDJPG_SetContextAlphaColor(ctx, cr, cg, cb, ca);
		return(1);
	}
#endif
	
	return(0);
}

int PDJPG_MarkerAPP12(PDJPG_Context *ctx, byte *buf)
{
	int i;
	buf+=2;

	i=(buf[0]<<24)|(buf[1]<<16)|(buf[2]<<8)|buf[3];

	return(0);
}

int PDJPG_MarkerAPP14(PDJPG_Context *ctx, byte *buf)
{
	buf+=2;

	return(0);
}

int PDJPG_MarkerAPPN(PDJPG_Context *ctx, byte *buf)
{
	return(0);
}

#if 0

int PDJPG_DecodeColorTransformYCbCr420(PDJPG_Context *ctx, byte *obuf)
{
	int y, u, v, r, g, b, a, u1, v1;
	int y0, y1, y2, y3;
	int r0, g0, b0, a0;
	int r1, g1, b1, a1;
	int r2, g2, b2, a2;
	int r3, g3, b3, a3;
	int l0, l1, l2, l3;
	int i, j, k, l;

#if 1
		for(i=0; i<ctx->ys; i+=2)
			for(j=0; j<ctx->xs; j+=2)
		{
			PDJPG_GetComponentPixelS420(ctx, j, i,
				&y0, &y1, &y2, &y3, &u, &v);

			if(ctx->alphaClr)
			{
				a=255;

				k=y0-ctx->alphaClrY; l=k*k;
				k=y1-ctx->alphaClrY; k=k*k; if(k<l)l=k;
				k=y2-ctx->alphaClrY; k=k*k; if(k<l)l=k;
				k=y3-ctx->alphaClrY; k=k*k; if(k<l)l=k;
				
				k=u-ctx->alphaClrU; l+=k*k;
				k=v-ctx->alphaClrV; l+=k*k;
				
				a=ctx->alphaClrA;
				a=(l<=(a*a))?0:255;
			}else { a=255; }

			u1=u-128; v1=v-128;
			l0=91881*v1;
			l1=-22554*u1-46802*v1;
			l2=116130*u1;

			y0<<=16; y1<<=16; y2<<=16; y3<<=16;
			r0=y0+l0; g0=y0+l1; b0=y0+l2;
			r1=y1+l0; g1=y1+l1; b1=y1+l2;
			r2=y2+l0; g2=y2+l1; b2=y2+l2;
			r3=y3+l0; g3=y3+l1; b3=y3+l2;

			r0>>=16; g0>>=16; b0>>=16;
			r1>>=16; g1>>=16; b1>>=16;
			r2>>=16; g2>>=16; b2>>=16;
			r3>>=16; g3>>=16; b3>>=16;

			if((r0|r1|r2|r3)>>8)
			{
				r0=(r0<0)?0:((r0>255)?255:r0);
				r1=(r1<0)?0:((r1>255)?255:r1);
				r2=(r2<0)?0:((r2>255)?255:r2);
				r3=(r3<0)?0:((r3>255)?255:r3);
			}

			if((g0|g1|g2|g3)>>8)
			{
				g0=(g0<0)?0:((g0>255)?255:g0);
				g1=(g1<0)?0:((g1>255)?255:g1);
				g2=(g2<0)?0:((g2>255)?255:g2);
				g3=(g3<0)?0:((g3>255)?255:g3);
			}

			if((b0|b1|b2|b3)>>8)
			{
				b0=(b0<0)?0:((b0>255)?255:b0);
				b1=(b1<0)?0:((b1>255)?255:b1);
				b2=(b2<0)?0:((b2>255)?255:b2);
				b3=(b3<0)?0:((b3>255)?255:b3);
			}

			k=ctx->ys-1-i;
			l0=((k*ctx->xs)+j+0)*4;
			k=ctx->ys-2-i;
			l2=((k*ctx->xs)+j+0)*4;

			obuf[l0+0]=r0; obuf[l0+1]=g0; obuf[l0+2]=b0; obuf[l0+3]=a;
			obuf[l0+4]=r1; obuf[l0+5]=g1; obuf[l0+6]=b1; obuf[l0+7]=a;
			obuf[l2+0]=r2; obuf[l2+1]=g2; obuf[l2+2]=b2; obuf[l2+3]=a;
			obuf[l2+4]=r3; obuf[l2+5]=g3; obuf[l2+6]=b3; obuf[l2+7]=a;
		}
#endif
}
#endif


int PDJPG_DecodeCtxInner(PDJPG_Context *ctx,
	byte *buf, int sz, int *xs, int *ys)
{
	byte *obuf, *ct;
	int y, u, v, r, g, b, a, u1, v1;
	int y0, y1, y2, y3;
	int r0, g0, b0, a0;
	int r1, g1, b1, a1;
	int r2, g2, b2, a2;
	int r3, g3, b3, a3;
	int l0, l1, l2, l3;
	int i, j, k, l;

	if((buf[0]!=0xFF) || (buf[1]!=JPG_SOI))
		return(-1);

	ctx->jpg_is420=0;
	ctx->jpg_rdct=0;

	for(i=0; i<sz; i++)
	{
		if(buf[i]!=0xFF)
			continue;

		if(buf[i+1]==0x00)
			{ i++; continue; }
		if(buf[i+1]==0xFF)
			{ i++; continue; }

		if(buf[i+1]==JPG_SOI)
			{ i++; continue; }

		j=buf[i+1]-0xC0;
		if(j<0)j=63;

		if(buf[i+1]==JPG_DQT)PDJPG_MarkerDQT(ctx, buf+i+2);
		if(buf[i+1]==JPG_DHT)PDJPG_MarkerDHT(ctx, buf+i+2);
		if(buf[i+1]==JPG_SOF0)PDJPG_MarkerSOF0(ctx, buf+i+2);
		if(buf[i+1]==JPG_SOS)
		{
			j=PDJPG_MarkerSOS(ctx, buf+i+2);
			if(j>0) { i+=j; }
			continue;
		}

		if((buf[i+1]>=JPG_APP0) && (buf[i+1]<=JPG_APP15))
		{
			if(buf[i+1]==JPG_APP0)PDJPG_MarkerAPP0(ctx, buf+i+2);
			if(buf[i+1]==JPG_APP11)PDJPG_MarkerAPP11(ctx, buf+i+2);
			if(buf[i+1]==JPG_APP12)PDJPG_MarkerAPP12(ctx, buf+i+2);
			if(buf[i+1]==JPG_APP14)PDJPG_MarkerAPP14(ctx, buf+i+2);

			//skip over marker
			j=(buf[i+2]<<8)|buf[i+3];
			i+=j;
			continue;
		}

		if(buf[i+1]==JPG_EOI)break;
	}

#if 0
	obuf=ctx->jpg_imgbuf;
	if(!obuf || (ctx->xs!=ctx->lxs) || (ctx->ys!=ctx->lys))
	{
		if(ctx->jpg_imgbuf)
			free(ctx->jpg_imgbuf);
		
		obuf=malloc((ctx->xs+1)*(ctx->ys+1)*4);
		memset(obuf, 0xFF, ctx->xs*ctx->ys*4);
		
		ctx->jpg_imgbuf=obuf;
		ctx->lxs=ctx->xs;
		ctx->lys=ctx->ys;
	}
#endif

#if 0
	for(i=0; i<ctx->jpg_nc; i++)
	{
		j=ctx->jpg_scn[i];

		ctx->jpg_schsci[i]=(65536*ctx->jpg_ch[j])/ctx->jpg_chm;
		ctx->jpg_scvsci[i]=(65536*ctx->jpg_cv[j])/ctx->jpg_chn;

		ctx->jpg_schs[i]=(ctx->jpg_cxi[j]+7)/8;
		ctx->jpg_scvs[i]=(ctx->jpg_cyi[j]+7)/8;
		
	}

	if(ctx->jpg_nc==3)
	{
		i=1;
		if(ctx->jpg_schsci[0]!=65536)i=0;
		if(ctx->jpg_scvsci[0]!=65536)i=0;
		if(ctx->jpg_schsci[1]!=32768)i=0;
		if(ctx->jpg_scvsci[1]!=32768)i=0;
		if(ctx->jpg_schsci[2]!=32768)i=0;
		if(ctx->jpg_scvsci[2]!=32768)i=0;
		ctx->jpg_is420=i;

		i=1;
		if(ctx->jpg_schsci[0]!=65536)i=0;
		if(ctx->jpg_scvsci[0]!=65536)i=0;
		if(ctx->jpg_schsci[1]!=65536)i=0;
		if(ctx->jpg_scvsci[1]!=65536)i=0;
		if(ctx->jpg_schsci[2]!=65536)i=0;
		if(ctx->jpg_scvsci[2]!=65536)i=0;
		ctx->jpg_is444=i;
	}else
	{
		ctx->jpg_is420=0;
		ctx->jpg_is444=0;
	}
#endif

	*xs=ctx->xs;
	*ys=ctx->ys;
	return(0);
}

int PDJPG_GetImageYUVA420(PDJPG_Context *ctx,
	byte *ybuf, byte *ubuf, byte *vbuf, byte *abuf,
	int xs, int ys)
{
	if((xs!=ctx->xs) || (ys!=ctx->ys))
		return(-1);

	if(ctx->jpg_is420)
	{
		memcpy(ybuf, ctx->jpg_sibuf[0], xs*ys);
		memcpy(ubuf, ctx->jpg_sibuf[1], (xs>>1)*(ys>>1));
		memcpy(vbuf, ctx->jpg_sibuf[2], (xs>>1)*(ys>>1));
		if(abuf)
		{
			if(ctx->jpg_sabuf)
				{ memcpy(abuf, ctx->jpg_sabuf, xs*ys); }
			else
				{ memset(abuf, 255, xs*ys); }
		}
		return(0);
	}
	
	return(-1);
}

int PDJPG_GetImagePlaneYUVA420(PDJPG_Context *ctx,
	byte *yuva, int xs, int ys)
{
	int n0, n1;
	
	n0=xs*ys;
	n1=(xs>>1)*(ys>>1);
	PDJPG_GetImageYUVA420(ctx,
		yuva, yuva+n0, yuva+n0+n1, yuva+n0+n1+n1, xs, ys);
}

void PDJPG_Free(byte *buf)
{
	if(!buf)return;
	free(buf);
}

#if 1
byte *PDJPG_DecodeScanForComponentLayer(byte *buf, int sz, char *name)
{
	byte *cs, *cs2, *cse;
	int i;
	
	cs=buf; cse=buf+sz;
	while(cs<cse)
	{
		if(*cs!=0xFF)
		{
			while((cs<cse) && ((*cs)!=0xFF))
				cs++;
			continue;
		}
	
		if((cs[0]==0xFF) && (cs[1]==JPG_APP11))
		{
			i=(cs[2]<<8)|cs[3];
			cs2=cs+i+2;

			cs+=4;
			if(!strcmp((char *)cs, "CompLayer"))
			{
				cs+=strlen((char *)cs)+1;
				if(!strcmp((char *)cs, name))
				{
//					printf("Found %s\n", name);
					cs+=strlen((char *)cs)+1;
					return(cs2);
				}

				cs+=strlen((char *)cs)+1;
				continue;
			}
			cs+=i-2;
			continue;
		}
		if((cs[0]==0xFF) && (cs[1]>=JPG_APP0) && (cs[1]<=JPG_APP15))
		{
			i=(cs[2]<<8)|cs[3];
			cs+=i+2;
			continue;
		}

		cs++;
	}
	
	return(NULL);
}
#endif

int PDJPG_DecodeBasic(PDJPG_Context *ctx,
	byte *buf, int sz, int *rxs, int *rys)
{
	byte *obuf;
	byte *otbuf;
	byte *csl;
	int i, n, sz1;
	
//	csl=NULL;
	csl=PDJPG_DecodeScanForComponentLayer(buf, sz, "Alpha");
	if(csl)
	{
		sz1=sz-(csl-buf);
//		PDJPG_DecodeCtxInner(ctx, csl, sz1, rxs, rys);
		PDJPG_DecodeLDatCtx(ctx, csl, sz1, rxs, rys);
		
		n=ctx->xs*ctx->ys;
		if(!ctx->jpg_sabuf)
			ctx->jpg_sabuf=malloc(n);
		memcpy(ctx->jpg_sabuf, ctx->jpg_sibuf[0], n);
	}else
	{
		n=ctx->xs*ctx->ys;
		if(ctx->jpg_sabuf)
			memset(ctx->jpg_sabuf, 255, n);
	}

	i=PDJPG_DecodeCtxInner(ctx, buf, sz, rxs, rys);
	return(i);
}


#if 1
int PDJPG_EscapeDecodeBuffer(byte *ibuf, int isz,
	byte *obuf, int osz)
{
	byte *cs, *ct, *cse, *cte;
	
	cs=ibuf; cse=ibuf+isz;
	ct=obuf; cte=obuf+osz;
	
	while((cs<cse) && (ct<cte))
	{
		if((cs[0]==0xFF) && (cs[1]==0x00))
			{ cs+=2; *ct++=0xFF; continue; }
		*ct++=*cs++;
	}
	if(ct>=cte)return(-1);
	return(ct-obuf);
}

int PDJPG_EscapeDecodeSingleBuffer(byte *buf, int sz)
{
	byte *cs, *ct, *cse, *cte;
	
	cs=buf; cse=buf+sz;
	ct=buf; cte=buf+sz;
	
	while((cs<cse) && (ct<cte))
	{
		if((cs[0]==0xFF) && (cs[1]==0x00))
			{ cs+=2; *ct++=0xFF; continue; }
		*ct++=*cs++;
	}
	if(ct>=cte)return(-1);
	return(ct-buf);
}

int PDJPG_DecodeLDatCtx(PDJPG_Context *ctx,
	byte *buf, int sz, int *xs, int *ys)
{
	byte *tbuf, *tbuf2, *cs, *ct;
	int tsz;
	int i, j, k;

	if((buf[0]!=0xFF) || (buf[1]!=JPG_APP12) ||
		(buf[4]!='L') || (buf[5]!='D') ||
		(buf[6]!='A') || (buf[7]!='T'))
	{
		k=PDJPG_DecodeCtxInner(ctx, buf, sz, xs, ys);
		return(k);
	}

	cs=buf; tsz=0;
	while((cs[0]==0xFF) && (cs[1]==JPG_APP12) &&
		(cs[4]=='L') && (cs[5]=='D') &&
		(cs[6]=='A') && (cs[7]=='T'))
	{
		i=(cs[2]<<8)|cs[3];
		tsz+=i;
		cs+=i+2;
	}
	
//	tbuf=malloc(1<<20);
	tbuf=malloc(tsz);
	cs=buf; ct=tbuf;

	while((cs[0]==0xFF) && (cs[1]==JPG_APP12) &&
		(cs[4]=='L') && (cs[5]=='D') &&
		(cs[6]=='A') && (cs[7]=='T'))
	{
		i=(cs[2]<<8)|cs[3];
		memcpy(ct, cs+8, i-6);
		cs+=i+2; ct+=(i-6);
	}

	i=ct-tbuf;
	i=PDJPG_EscapeDecodeSingleBuffer(tbuf, i);

	k=PDJPG_DecodeCtxInner(ctx, tbuf, i, xs, ys);
	free(tbuf);
	return(k);
}
#endif
