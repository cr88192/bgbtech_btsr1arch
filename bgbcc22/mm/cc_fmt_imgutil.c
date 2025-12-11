
int BGBCC_ImgUtil_DrawLine(byte *ibuf, int xs, int ys,
	int x0, int y0, int x1, int y1, u32 clr)
{
	int tstep, xstp, ystp, xcur, ycur, x, y, xd, yd;
	int cr, cg, cb;
	int i, j, k;

	cr=(clr>>16)&255;
	cg=(clr>> 8)&255;
	cb=(clr>> 0)&255;

	xd=x1-x0;
	yd=y1-y0;
	tstep=sqrt(xd*xd+yd*yd);
	if(tstep<2)
		tstep=2;
	
	xstp=(xd<<16)/tstep;
	ystp=(yd<<16)/tstep;

	xcur=x0<<16;
	ycur=y0<<16;
	
	for(i=0; i<tstep; i++)
	{
		x=xcur>>16;
		y=ycur>>16;
		xcur+=xstp;
		ycur+=ystp;
		if((x<0) || (x>=xs))
			continue;
		if((y<0) || (y>=ys))
			continue;
		ibuf[(y*xs+x)*4+0]=cr;
		ibuf[(y*xs+x)*4+1]=cg;
		ibuf[(y*xs+x)*4+2]=cb;
	}
	return(0);
}

int BGBCC_ImgUtil_DrawGlyph(byte *ibuf, int xs, int ys,
	int xb, int yb, u64 pix, u32 clr)
{
	int x, y, px, py;
	int cr, cg, cb;
	int i, j, k;

	cr=(clr>>16)&255;
	cg=(clr>> 8)&255;
	cb=(clr>> 0)&255;
	
	for(py=0; py<8; py++)
		for(px=0; px<8; px++)
	{
		x=xb+px;
		y=yb+py;
		
		k=(pix>>(py*8+(7-px)))&1;
		if(!k)
			continue;

		if((x<0) || (x>=xs))
			continue;
		if((y<0) || (y>=ys))
			continue;
		ibuf[(y*xs+x)*4+0]=cr;
		ibuf[(y*xs+x)*4+1]=cg;
		ibuf[(y*xs+x)*4+2]=cb;
	}
	return(0);
}



u64 bgbcc_gfxcon_glyphs[256]=
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

u64 bgbcc_gfxcon_densehex[256];

int BGBCC_ImgUtil_DrawTextString(byte *ibuf, int xs, int ys,
	int xb, int yb, char *str, u32 clr)
{
	u64 px;
	char *cs;
	int x, y;

	x=xb; y=yb;
	cs=str;
	while(*cs)
	{
		px=bgbcc_gfxcon_glyphs[*cs++];
		BGBCC_ImgUtil_DrawGlyph(ibuf, xs, ys, x, y, px, clr);
		x+=8;
	}
	return(0);
}

int BGBCC_ImgUtil_DrawDenseHexDigit(byte *ibuf, int xs, int ys,
	int xb, int yb, byte dig, u32 clr)
{
	static u32 densedigs[16]=
	{
		0x07555700,	0x06222700,
		0x07174700,	0x07171700,
		0x05571100,	0x07471700,
		0x07475700,	0x07111100,
		0x07575700,	0x07571100,
		0x02575500,	0x06565600,
		0x03444300,	0x06555600,
		0x07474700,	0x07474400
	};
	u64 pix;
	u32 d0, d1;
	int j0, j1;
	int i, j, k;

	if(!bgbcc_gfxcon_densehex[0])
	{
		for(i=0; i<256; i++)
		{
			j0=(i>>4)&15;
			j1=(i>>0)&15;
			d0=densedigs[j0];
			d1=densedigs[j1];
			
			pix=0;
			for(j=0; j<8; j++)
			{
				pix|=((d0>>(j*4))&15ULL)<<(j*8+4);
				pix|=((d1>>(j*4))&15ULL)<<(j*8+0);
			}
			bgbcc_gfxcon_densehex[i]=pix;
		}
	}

	pix=bgbcc_gfxcon_densehex[dig];
	BGBCC_ImgUtil_DrawGlyph(ibuf, xs, ys, xb, yb, pix, clr);
	return(0);
}

int BGBCC_ImgUtil_ResampleImageLin(
	byte *ibuf, int ixs, int iys,
	byte *obuf, int oxs, int oys)
{
	byte *cts, *ctt;
	int st_x, st_y, fr_x, fr_y, f_x, f_y, f_sc_x, f_sc_y;
	int c0, c1, c2, c3, c4, c5, c6, hflip, vflip;
	int x, y, x1, y1;
	int i, j, k;
	
	if((ixs==oxs) && (iys==oys))
	{
		if(ibuf!=obuf)
			memcpy(obuf, ibuf, oxs*oys*4);
		return(0);
	}
	
	hflip=0;	vflip=0;
	if(oxs<0)	{ hflip=1; oxs=-oxs; }
	if(oys<0)	{ vflip=1; oys=-oys; }
	
	st_x=(65536*ixs)/oxs;
	st_y=(65536*iys)/oys;
	
	f_sc_x=256;
	f_sc_y=256;
	
	if(oxs<ixs)
	{
		f_sc_x=(((256*oxs)/ixs)-128)*2;
		if(f_sc_x<0)f_sc_x=0;
	}
	if(oys<iys)
	{
		f_sc_y=(((256*oys)/iys)-128)*2;
		if(f_sc_y<0)f_sc_y=0;
	}
	
	fr_x=0; fr_y=0;
	if(hflip)
		{ fr_x+=(oxs-1)*st_x; st_x=-st_x; }
	if(vflip)
		{ fr_y+=(oys-1)*st_y; st_y=-st_y; }
	
	ctt=obuf;
	for(y=0; y<oys; y++)
	{
		fr_x=0;
		for(x=0; x<oys; x++)
		{
			x1=fr_x>>16;
			y1=fr_y>>16;
			f_x=fr_x&65535;
			f_y=fr_y&65535;
			
			f_x=(((f_x-32768)*f_sc_x)>>8)+32768;
			f_y=(((f_y-32768)*f_sc_y)>>8)+32768;
			
			cts=ibuf+(y1*ixs+x1)*4;
			
			for(j=0; j<4; j++)
			{
				c0=cts[0+j];
				c1=cts[4+j];
				c2=cts[ixs*4+0+j];
				c3=cts[ixs*4+4+j];
				c4=(c0*(65535-f_x)+c1*f_x)>>16;
				c5=(c2*(65535-f_x)+c3*f_x)>>16;
				c6=(c4*(65535-f_y)+c5*f_y)>>16;
				ctt[j]=c6;
			}
			
			fr_x+=st_x;
			ctt+=4;
		}
		fr_y+=st_y;
	}

	return(0);
}

int BGBCC_ImgUtil_ResampleImage(
	byte *ibuf, int ixs, int iys,
	byte *obuf, int oxs, int oys)
{
//	if((oxs>=ixs) && (oys>=iys))
	if(1)
	{
		return(BGBCC_ImgUtil_ResampleImageLin(ibuf, ixs, iys, obuf, oxs, oys));
	}
}

/*
 * Composite a source image onto a destination buffer.
 * dstbuf: Destination Buffer
 * dxs, dys: Destination Image Size
 * ibuf: Image Source Buffer
 * ixs, iys: Image Source Size
 * obx, oby: Origin within Destination Image
 * oxs, oys: Logical resolution of image within destination
 * ang: Rotate image by this amount
 * d_bf, s_bf: Dest and Source Blend Function
 */
int BGBCC_ImgUtil_CompositeBlendImage(
	byte *dstbuf, int dxs, int dys,
	byte *ibuf, int ixs, int iys,
	int obx, int oby, int oxs, int oys, u16 ang,
	int d_bf, int s_bf
	)
{
	static s16 sintab[256];
	static byte *resampbuf;
	static int sz_resampbuf;
	int d_cr, d_cg, d_cb, d_ca, d_va;
	int s_cr, s_cg, s_cb, s_ca, s_va;
	int b_cr, b_cg, b_cb, b_ca;
	int va_sin, va_cos;
	int x, y, z, x1, y1, z1, i, j, k;
	
	if(!sintab[64])
	{
		for(i=0; i<256; i++)
		{
			sintab[i]=sin(i*(M_PI/128))*256;
		}
	}
	
	k=oxs*oys;
	if(resampbuf && (k>sz_resampbuf))
		{ free(resampbuf); resampbuf=NULL; }
	
	if(!resampbuf)
	{
		resampbuf=malloc(k*4);
		sz_resampbuf=k;
	}
	
	BGBCC_ImgUtil_ResampleImage(ibuf, ixs, iys, resampbuf, oxs, oys);
	
	z1=ang&255;
	x1=sintab[((ang>>8)+ 0)&256];
	y1=sintab[((ang>>8)+ 1)&256];
	va_sin=(x1*(256-z1)+y1*z1)>>8;
	x1=sintab[((ang>>8)+64)&256];
	y1=sintab[((ang>>8)+65)&256];
	va_cos=(x1*(256-z1)+y1*z1)>>8;
	
	for(y=0; y<oys; y++)
		for(x=0; x<oxs; x++)
	{
		x1=obx+(x*va_cos)+(y*va_sin);
		y1=oby+(y*va_cos)-(x*va_sin);
		if((x1<0) || (y1<0) || (x1>=dxs) || (y1>=dys))
			continue;

		z1=y1*dxs+x1;
		d_cr=dstbuf[z1+0];	d_cg=dstbuf[z1+1];
		d_cb=dstbuf[z1+2];	d_ca=dstbuf[z1+3];
		z=y*oxs+x;
		s_cr=dstbuf[z+0];	s_cg=dstbuf[z+1];
		s_cb=dstbuf[z+2];	s_ca=dstbuf[z+3];
		
		d_va=255-s_ca;
		s_va=s_ca;
		b_cr=((d_cr*d_va)+(s_cr*s_va))>>8;
		b_cg=((d_cg*d_va)+(s_cg*s_va))>>8;
		b_cb=((d_cb*d_va)+(s_cb*s_va))>>8;
		b_ca=((d_ca*d_va)+(s_ca*s_va))>>8;
		dstbuf[z1+0]=b_cr;	dstbuf[z1+1]=b_cg;
		dstbuf[z1+2]=b_cb;	dstbuf[z1+3]=b_ca;
	}
	
	return(0);
}

int BGBCC_ImgUtil_DrawFloodFillHorz(u32 *ibuf, int xs, int ys,
	int xc, int yc, u32 clr, u32 ref)
{
	int x, x0, x1, yb, yb1;
	
	if(clr==ref)
		return(0);
	
	yb=yc*xs;
	x0=xc; x1=xc;
	while((x0>0) && (ibuf[yb+x0-1]==ref))
		x0--;
	while((x1<(xs-1)) && (ibuf[yb+x1+1]==ref))
		x1++;

	for(x=x0; x<=x1; x++)
		{ ibuf[yb+x]=clr; }

	if(yc>0)
	{
		yb1=yb-xs;
		for(x=x0; x<=x1; x++)
		{
			if(ibuf[yb1+x]==ref)
				BGBCC_ImgUtil_DrawFloodFillHorz(ibuf, xs, ys,
					x, yc-1, clr, ref);
		}
	}

	if(yc<(ys-1))
	{
		yb1=yb+xs;
		for(x=x0; x<=x1; x++)
		{
			if(ibuf[yb1+x]==ref)
				BGBCC_ImgUtil_DrawFloodFillHorz(ibuf, xs, ys,
					x, yc+1, clr, ref);
		}
	}
	return(0);
}

int BGBCC_ImgUtil_DrawFloodFill(byte *ibuf, int xs, int ys,
	int x0, int y0, u32 clr0)
{
	u32 *ibufw;
	u32 clr1, ref;
	
	ibufw=(u32 *)ibuf;
	ref=ibufw[y0*xs+x0];
	clr1=	((clr0<< 0)&0xFF00FF00U) |
			((clr0<<16)&0x00FF0000U) |
			((clr0>>16)&0x000000FFU) ;
	BGBCC_ImgUtil_DrawFloodFillHorz((u32 *)ibuf, xs, ys,
		x0, y0, clr1, ref);
	return(0);
}
