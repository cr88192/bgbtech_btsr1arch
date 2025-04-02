u64 bgbcc_glyphs_asc[96]=
{
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
};

int BGBCC_ImageDrawGlyph(u32 *ibuf, int xstr,
	int xo, int yo, u64 pix, u32 clr)
{
	int x, y, k;
	
	for(y=0; y<8; y++)
		for(x=0; x<8; x++)
	{
//		k=63-(y*8+x);
		k=63-((7-y)*8+x);
		if((pix>>k)&1)
		{
			ibuf[(yo+y)*xstr+(xo+x)]=clr;
		}
	}
	return(0);
}

int BGBCC_ImageDrawHex2(u32 *ibuf, int xstr,
	int xo, int yo, int val, u32 clr)
{
	static char *hexdig="0123456789ABCDEF";
	int i0, i1;
	
	i0=hexdig[(val>>4)&15]-' ';
	i1=hexdig[(val>>0)&15]-' ';
	BGBCC_ImageDrawGlyph(ibuf, xstr, xo+1, yo, bgbcc_glyphs_asc[i0], clr);
	BGBCC_ImageDrawGlyph(ibuf, xstr, xo+7, yo, bgbcc_glyphs_asc[i1], clr);
	return(0);
}

int BGBCC_ImageDrawDec2(u32 *ibuf, int xstr,
	int xo, int yo, int val, u32 clr)
{
	static char *hexdig="0123456789ABCDEF";
	int i0, i1;
	
	i0=hexdig[val/10]-' ';
	i1=hexdig[val%10]-' ';
	BGBCC_ImageDrawGlyph(ibuf, xstr, xo+1, yo, bgbcc_glyphs_asc[i0], clr);
	BGBCC_ImageDrawGlyph(ibuf, xstr, xo+7, yo, bgbcc_glyphs_asc[i1], clr);
	return(0);
}

byte bgbcc_dfl_pal256[1024];
byte bgbcc_dfl_pal16[64];
byte bgbcc_dfl_pal16t[64];
byte bgbcc_dfl_pal4[16];
byte bgbcc_dfl_pal4t[16];
byte bgbcc_dfl_pal2[8];
byte bgbcc_dfl_pal2t[8];

u16 bgbcc_cnts_pal256[256];

byte *bgbcc_img_bmppallookup;
byte *bgbcc_img_bmppallookupb;


int BGBCC_LoadConvResource_SetupPal()
{
	byte *tbuf;
	byte *pal, *pal256;
	byte *paldith;
	int cr, cg, cb;
	int ch, cm, cl;
	int i, j, k;
	
	if(bgbcc_dfl_pal256[15*4+0])
		return(0);
	
	paldith=NULL;
	tbuf=NULL;
	
	pal=bgbcc_dfl_pal256;
	for(i=0; i<16; i++)
	{
		for(j=0; j<16; j++)
		{
			ch=j*16;
			cl=ch>>2;
			if(i&8)
				{ cl=(ch*5)/8; }
			cr=(i&4)?ch:cl;
			cg=(i&2)?ch:cl;
			cb=(i&1)?ch:cl;
			if(i==0)
				{ cr=ch; cg=ch; cb=ch; }
			if((i==7) || (i==8) || (i==15))
			{
				cl=(ch*7)/8;
				cr=ch;	cg=ch;	cb=ch;
				if(i==7)	cg=cl;
				if(i==8)	cb=cl;
				if(i==15)	cr=cl;
			}
			pal[(i*16+j)*4+0]=cr;
			pal[(i*16+j)*4+1]=cg;
			pal[(i*16+j)*4+2]=cb;
			pal[(i*16+j)*4+3]=255;
		}
	}

	for(i=0; i<3; i++)
	{
		for(j=1; j<16; j++)
		{
			ch=j*16;
			cm=(ch*5)/8;
			cl=(ch*3)/8;
			if(i==0)
				{ cr=ch; cg=cm; cb=cl; k=7; }
			if(i==1)
				{ cr=cm; cg=ch; cb=cl; k=8; }
			if(i==2)
				{ cr=cl; cg=cm; cb=ch; k=15; }
//			pal[(j*16+i+1)*4+0]=cr;
//			pal[(j*16+i+1)*4+1]=cg;
//			pal[(j*16+i+1)*4+2]=cb;
//			pal[(j*16+i+1)*4+3]=255;

			pal[(k*16+j)*4+0]=cr;
			pal[(k*16+j)*4+1]=cg;
			pal[(k*16+j)*4+2]=cb;
			pal[(k*16+j)*4+3]=255;
		}
	}

	for(i=0; i<3; i++)
	{
//		for(j=11; j<16; j++)
//		for(j=9; j<16; j++)
		for(j=8; j<16; j++)
		{
			ch=j*16;

			cl=(ch*7)/8;
			cr=ch;	cg=ch;	cb=ch;
			if(i==0)	cg=cl;
			if(i==1)	cb=cl;
			if(i==2)	cr=cl;

//			k=1+(i*5)+(j-11);
//			k=(k*16+1);

			k=8+(j-8);
//			k=(k*16+(1+i));
			k=(k*16+(0+i));

			pal[k*4+0]=cr;
			pal[k*4+1]=cg;
			pal[k*4+2]=cb;
			pal[k*4+3]=255;
		}
	}

	pal256=pal;

	pal=bgbcc_dfl_pal16;
	for(i=0; i<16; i++)
	{
		if(i&8)
			{ ch=0xFF; cl=0x55; }
		else
			{ ch=0xAA; cl=0x00; }
		cr=(i&4)?ch:cl;
		cg=(i&2)?ch:cl;
		cb=(i&1)?ch:cl;

#if 1
		if(i==6)
		{
			cr=0xAA;
			cg=0x55;
			cb=0x00;
		}
#endif
		
		pal[i*4+0]=cr;
		pal[i*4+1]=cg;
		pal[i*4+2]=cb;
		pal[i*4+3]=255;

//		pal256[(i*16+0)*4+0]=cr;
//		pal256[(i*16+0)*4+1]=cg;
//		pal256[(i*16+0)*4+2]=cb;
//		pal256[(i*16+0)*4+3]=255;

		if((i>=1) && (i<8))
		{
			k=(i*16+0);
		}
		else if((i>=9) && (i<16))
		{
			k=((i-8)*16+1);
		}else
		{
			k=0;
		}
		if(k>0)
		{
			pal256[k*4+0]=cr;
			pal256[k*4+1]=cg;
			pal256[k*4+2]=cb;
			pal256[k*4+3]=255;
		}

	}

//	pal256[(8*16+0)*4+0]=255;
//	pal256[(8*16+0)*4+1]=0;
//	pal256[(8*16+0)*4+2]=255;
//	pal256[(8*16+0)*4+3]=0;

	pal256[(7*16+0)*4+0]=255;
	pal256[(7*16+0)*4+1]=0;
	pal256[(7*16+0)*4+2]=255;
	pal256[(7*16+0)*4+3]=0;

	for(i=0; i<64; i++)
		bgbcc_dfl_pal16t[i]=bgbcc_dfl_pal16[i];

	bgbcc_dfl_pal16t[13*4+0]=255;
	bgbcc_dfl_pal16t[13*4+1]=0;
	bgbcc_dfl_pal16t[13*4+2]=255;
	bgbcc_dfl_pal16t[13*4+3]=0;

	pal=bgbcc_dfl_pal4;
	for(i=0; i<4; i++)
	{
		if(i==0)	ch=0x00;
		if(i==1)	ch=0x55;
		if(i==2)	ch=0xAA;
		if(i==3)	ch=0xFF;
		pal[i*4+0]=ch;
		pal[i*4+1]=ch;
		pal[i*4+2]=ch;
		pal[i*4+3]=255;
	}

	pal=bgbcc_dfl_pal2;
	for(i=0; i<2; i++)
	{
		if(i==0)	ch=0x00;
		if(i==1)	ch=0xFF;
		pal[i*4+0]=ch;
		pal[i*4+1]=ch;
		pal[i*4+2]=ch;
		pal[i*4+3]=255;
	}

	pal=bgbcc_dfl_pal4t;
	pal[0*4+0]=0x00; pal[0*4+1]=0x00; pal[0*4+2]=0x00; pal[0*4+3]=255;
	pal[1*4+0]=0xFF; pal[1*4+1]=0xFF; pal[1*4+2]=0xFF; pal[1*4+3]=255;
	pal[2*4+0]=0x7F; pal[2*4+1]=0x7F; pal[2*4+2]=0x7F; pal[2*4+3]=255;
	pal[3*4+0]=0xFF; pal[3*4+1]=0x00; pal[3*4+2]=0xFF; pal[3*4+3]=0;

#if 0
	tbuf=malloc(16*16*8);
	k=BGBCC_Img_EncodeImageBMP8(
		tbuf, bgbcc_dfl_pal256, 16, 16, bgbcc_dfl_pal256);
	BGBCC_StoreFile("dump/rsrc_pal256.bmp", tbuf, k);

	k=BGBCC_Img_EncodeImageBMP16(
		tbuf, bgbcc_dfl_pal256, 16, 16);
	BGBCC_StoreFile("dump/rsrc_pal256b.bmp", tbuf, k);

	k=BGBCC_Img_EncodeImageBMP32(
		tbuf, bgbcc_dfl_pal256, 16, 16);
	BGBCC_StoreFile("dump/rsrc_pal256c.bmp", tbuf, k);

	k=BGBCC_Img_EncodeImageBMP4(
		tbuf, bgbcc_dfl_pal256, 16, 16, bgbcc_dfl_pal16);
	BGBCC_StoreFile("dump/rsrc_pal16.bmp", tbuf, k);

	k=BGBCC_Img_EncodeImageBMP2(
		tbuf, bgbcc_dfl_pal4, 4, 1, bgbcc_dfl_pal4);
	BGBCC_StoreFile("dump/rsrc_pal4.bmp", tbuf, k);
#endif

#if 0
	BGBCC_Img_EncodeImageBmpSetupPal(bgbcc_dfl_pal256, 256);

	paldith=malloc(256*256*4);
	for(i=0; i<32768; i++)
	{
		j=bgbcc_img_bmppallookup[i];
		k=bgbcc_img_bmppallookupb[i];
		paldith[i*4+0]=bgbcc_dfl_pal256[j*4+0];
		paldith[i*4+1]=bgbcc_dfl_pal256[j*4+1];
		paldith[i*4+2]=bgbcc_dfl_pal256[j*4+2];
		paldith[i*4+3]=255;

		j=i+32768;
		paldith[j*4+0]=bgbcc_dfl_pal256[k*4+0];
		paldith[j*4+1]=bgbcc_dfl_pal256[k*4+1];
		paldith[j*4+2]=bgbcc_dfl_pal256[k*4+2];
		paldith[j*4+3]=255;
	}

	tbuf=malloc(256*256*8);
	k=BGBCC_Img_EncodeImageBMP8(
		tbuf, paldith, 256, 256, bgbcc_dfl_pal256);
	BGBCC_StoreFile("dump/rsrc_paldith8.bmp", tbuf, k);


	for(i=0; i<32768; i++)
	{
		cr=(i>>10)&31;
		cg=(i>> 5)&31;
		cb=(i>> 0)&31;
		cr=(cr<<3)|(cr>>2);
		cg=(cg<<3)|(cg>>2);
		cb=(cb<<3)|(cb>>2);
		paldith[i*4+0]=cr;
		paldith[i*4+1]=cg;
		paldith[i*4+2]=cb;
		paldith[i*4+3]=255;
		
		j=i+32768;
		paldith[j*4+0]=cr;
		paldith[j*4+1]=cg;
		paldith[j*4+2]=cb;
		paldith[j*4+3]=255;
	}

	k=BGBCC_Img_EncodeImageBMP16(
		tbuf, paldith, 256, 256);
	BGBCC_StoreFile("dump/rsrc_paldith16.bmp", tbuf, k);
#endif

#if 0
	if(!paldith)
	{
		BGBCC_Img_EncodeImageBmpSetupPal(bgbcc_dfl_pal256, 256);
		paldith=malloc(256*256*4);
		tbuf=malloc(256*256*8);
	}

	for(i=0; i<32768; i++)
	{
		j=bgbcc_img_bmppallookup[i];
		bgbcc_cnts_pal256[j]++;
	}

	for(i=0; i<65536; i++)
	{
		j=((i>>4)&15)|((i>>8)&0xF0);
		paldith[i*4+0]=bgbcc_dfl_pal256[j*4+0];
		paldith[i*4+1]=bgbcc_dfl_pal256[j*4+1];
		paldith[i*4+2]=bgbcc_dfl_pal256[j*4+2];
		paldith[i*4+3]=255;
	}

	cg=0;
	for(i=0; i<256; i++)
	{
		j=bgbcc_cnts_pal256[i];
		if(j>cg)
			cg=j;
	}
	
//	cg=0xFFF0/cg;
	cg=0x6380/cg;

	for(i=0; i<16; i++)
		for(j=0; j<16; j++)
	{
		cr=((u32 *)bgbcc_dfl_pal256)[i*16+j];
		cr^=0x00FFFFFF;
	
		k=bgbcc_cnts_pal256[i*16+j];
		k=(k*cg)>>8;
//		BGBCC_ImageDrawHex2(
		BGBCC_ImageDrawDec2(
			(u32 *)paldith, 256,
			j*16, i*16+4, k, cr);
	}

	k=BGBCC_Img_EncodeImageBMP16(
		tbuf, paldith, 256, 256);
	BGBCC_StoreFile("dump/rsrc_pal8cnts.bmp", tbuf, k);
#endif

	return(1);
}

byte *BGBCC_LoadConvResource(byte *buf, int sz, fourcc lang,
	char *cnvstr, int *rsz, fourcc *rfcc)
{
	char cnv[16], pvar[8], pval[64];
	char *cs, *ct;
	byte *ibuf, *obuf;
	short *sbuf;
	int qlvl;
	int xs, ys, sz1, fl, len, rt, fm1, ch1;

	cs=cnvstr;
	ct=cnv;
	while(*cs && (*cs!=','))
		*ct++=*cs++;
	*ct++=0;
	
	qlvl=0;
	
	while(*cs==',')
	{
		cs++;
		ct=pvar;
		while(*cs && *cs!='=' && *cs!=',')
			*ct++=*cs++;
		*ct++=0;
		if(*cs=='=')
		{
			ct=pval;
			while(*cs && *cs!='=' && *cs!=',')
				*ct++=*cs++;
			*ct++=0;
			
			if(!bgbcc_stricmp(pvar, "q"))
				qlvl=atoi(pval);
		}
	}

	if(	!bgbcc_stricmp(cnv, "bmp1") ||
		!bgbcc_stricmp(cnv, "bmp2") ||
		!bgbcc_stricmp(cnv, "bmp4") ||
		!bgbcc_stricmp(cnv, "bmp8") ||
		!bgbcc_stricmp(cnv, "bmp1a") ||
		!bgbcc_stricmp(cnv, "bmp2a") ||
		!bgbcc_stricmp(cnv, "bmp4a") ||
		!bgbcc_stricmp(cnv, "bmp8a") ||
		!bgbcc_stricmp(cnv, "bmp2ta") ||
		!bgbcc_stricmp(cnv, "bmp2t") ||
		!bgbcc_stricmp(cnv, "bmp4ta") ||
		!bgbcc_stricmp(cnv, "bmp4t") ||
		!bgbcc_stricmp(cnv, "bmp16") ||
		!bgbcc_stricmp(cnv, "bmp16a") ||
		!bgbcc_stricmp(cnv, "bmp32") ||
		!bgbcc_stricmp(cnv, "bmp32a") ||
		!bgbcc_stricmp(cnv, "bmp_cram8") ||
		!bgbcc_stricmp(cnv, "bmp_cram8a") ||
		!bgbcc_stricmp(cnv, "bmp_cram8np") ||
		!bgbcc_stricmp(cnv, "bmp_cram8npa") ||
		!bgbcc_stricmp(cnv, "bmp_cq8") ||
		!bgbcc_stricmp(cnv, "bmp_cq8a") ||
		!bgbcc_stricmp(cnv, "bmp_cq8np") ||
		!bgbcc_stricmp(cnv, "bmp_cq8npa") ||
		!bgbcc_stricmp(cnv, "bmp_lz8") ||
		!bgbcc_stricmp(cnv, "bmp_lz8a") )
	{
		BGBCC_LoadConvResource_SetupPal();
		
		ibuf=BGBCC_Img_DecodeImage(buf, &xs, &ys);
		if(!ibuf)
			return(NULL);
		
		obuf=malloc(xs*ys*5);
		sz1=0;

		if(!bgbcc_stricmp(cnv, "bmp8"))
			sz1=BGBCC_Img_EncodeImageBMP8(
				obuf, ibuf, xs, ys, bgbcc_dfl_pal256);
		if(!bgbcc_stricmp(cnv, "bmp4"))
			sz1=BGBCC_Img_EncodeImageBMP4(
				obuf, ibuf, xs, ys, bgbcc_dfl_pal16);
		if(!bgbcc_stricmp(cnv, "bmp2"))
			sz1=BGBCC_Img_EncodeImageBMP2(
				obuf, ibuf, xs, ys, bgbcc_dfl_pal4);
		if(!bgbcc_stricmp(cnv, "bmp1"))
			sz1=BGBCC_Img_EncodeImageBMP1(
				obuf, ibuf, xs, ys, bgbcc_dfl_pal2);

		if(!bgbcc_stricmp(cnv, "bmp8a"))
			sz1=BGBCC_Img_EncodeImageBMP8A(
				obuf, ibuf, xs, ys, bgbcc_dfl_pal256);
		if(!bgbcc_stricmp(cnv, "bmp4a"))
			sz1=BGBCC_Img_EncodeImageBMP4A(
				obuf, ibuf, xs, ys, bgbcc_dfl_pal16);
		if(!bgbcc_stricmp(cnv, "bmp2a"))
			sz1=BGBCC_Img_EncodeImageBMP2A(
				obuf, ibuf, xs, ys, bgbcc_dfl_pal4);
		if(!bgbcc_stricmp(cnv, "bmp1a"))
			sz1=BGBCC_Img_EncodeImageBMP1A(
				obuf, ibuf, xs, ys, bgbcc_dfl_pal2);

		if(!bgbcc_stricmp(cnv, "bmp4t"))
			sz1=BGBCC_Img_EncodeImageBMP4(
				obuf, ibuf, xs, ys, bgbcc_dfl_pal16t);
		if(!bgbcc_stricmp(cnv, "bmp4ta"))
			sz1=BGBCC_Img_EncodeImageBMP4A(
				obuf, ibuf, xs, ys, bgbcc_dfl_pal16t);

		if(!bgbcc_stricmp(cnv, "bmp2t"))
			sz1=BGBCC_Img_EncodeImageBMP2(
				obuf, ibuf, xs, ys, bgbcc_dfl_pal4t);
		if(!bgbcc_stricmp(cnv, "bmp2ta"))
			sz1=BGBCC_Img_EncodeImageBMP2A(
				obuf, ibuf, xs, ys, bgbcc_dfl_pal4t);

		if(!bgbcc_stricmp(cnv, "bmp16"))
			sz1=BGBCC_Img_EncodeImageBMP16(
				obuf, ibuf, xs, ys);
		if(!bgbcc_stricmp(cnv, "bmp16a"))
			sz1=BGBCC_Img_EncodeImageBMP16A(
				obuf, ibuf, xs, ys);

		if(!bgbcc_stricmp(cnv, "bmp32"))
			sz1=BGBCC_Img_EncodeImageBMP32(
				obuf, ibuf, xs, ys);
		if(!bgbcc_stricmp(cnv, "bmp32a"))
			sz1=BGBCC_Img_EncodeImageBMP32A(
				obuf, ibuf, xs, ys);

		if(!bgbcc_stricmp(cnv, "bmp_cram8"))
			sz1=BGBCC_Img_EncodeImageBMP_CRAM8(
				obuf, ibuf, xs, ys, bgbcc_dfl_pal256);
		if(!bgbcc_stricmp(cnv, "bmp_cram8a"))
			sz1=BGBCC_Img_EncodeImageBMP_CRAM8A(
				obuf, ibuf, xs, ys, bgbcc_dfl_pal256);

		if(!bgbcc_stricmp(cnv, "bmp_cram8np"))
			sz1=BGBCC_Img_EncodeImageBMP_CRAM8NPA(
				obuf, ibuf, xs, ys, bgbcc_dfl_pal256);
		if(!bgbcc_stricmp(cnv, "bmp_cram8npa"))
			sz1=BGBCC_Img_EncodeImageBMP_CRAM8NPA(
				obuf, ibuf, xs, ys, bgbcc_dfl_pal256);

		if(!bgbcc_stricmp(cnv, "bmp_cq8"))
			sz1=BGBCC_Img_EncodeImageBMP_CQ8A(
				obuf, ibuf, xs, ys, bgbcc_dfl_pal256);
		if(!bgbcc_stricmp(cnv, "bmp_cq8a"))
			sz1=BGBCC_Img_EncodeImageBMP_CQ8A(
				obuf, ibuf, xs, ys, bgbcc_dfl_pal256);
		if(!bgbcc_stricmp(cnv, "bmp_cq8np"))
			sz1=BGBCC_Img_EncodeImageBMP_CQ8NPA(
				obuf, ibuf, xs, ys, bgbcc_dfl_pal256);
		if(!bgbcc_stricmp(cnv, "bmp_cq8npa"))
			sz1=BGBCC_Img_EncodeImageBMP_CQ8NPA(
				obuf, ibuf, xs, ys, bgbcc_dfl_pal256);

		if(!bgbcc_stricmp(cnv, "bmp_lz8"))
			sz1=BGBCC_Img_EncodeImageBMP_LZ8(
				obuf, ibuf, xs, ys, bgbcc_dfl_pal256);
		if(!bgbcc_stricmp(cnv, "bmp_lz8a"))
			sz1=BGBCC_Img_EncodeImageBMP_LZ8A(
				obuf, ibuf, xs, ys, bgbcc_dfl_pal256);

		*rfcc=BGBCC_FMT_BMP;
		*rsz=sz1;
		return(obuf);
	}
	
	if(	!bgbcc_stricmp(cnv, "qoi") ||
		!bgbcc_stricmp(cnv, "qoli") )
	{
		ibuf=BGBCC_Img_DecodeImage(buf, &xs, &ys);
		if(!ibuf)
			return(NULL);
		
		fl=0;
		if(!bgbcc_stricmp(cnv, "qoli"))
			fl|=1;
		
		fl|=(qlvl&7)<<1;
		
		obuf=malloc(xs*ys*2);
		sz1=QOI_EncodeImageBuffer(obuf, ibuf, xs, ys, fl);
		
		*rfcc=BGBCC_FMT_QOIF;
		*rsz=sz1;
		return(obuf);
	}

	if(	!bgbcc_stricmp(cnv, "upic") )
	{
		ibuf=BGBCC_Img_DecodeImage(buf, &xs, &ys);
		if(!ibuf)
			return(NULL);
		
		fl=1<<8;
		
		fl|=100-((qlvl&7)*12);
		
		obuf=malloc(xs*ys*2);
		sz1=TKuPI_EncodeImageBufferTemp(obuf, ibuf, xs, ys, fl);
		
		*rfcc=BGBCC_FMT_UPIC;
		*rsz=sz1;
		return(obuf);
	}
	
	if(!strncmp(cnv, "wav_", 4))
	{
		rt=16000;
		fm1=0;
		ch1=1;
		
		cs=cnv;
		if(!strncmp(cs, "wav_", 4))
			cs+=4;

		if(!strncmp(cs, "8m_", 3))
			{ rt=8000; cs+=3; }
		if(!strncmp(cs, "11m_", 4))
			{ rt=11025; cs+=4; }
		if(!strncmp(cs, "16m_", 4))
			{ rt=16000; cs+=4; }
		if(!strncmp(cs, "22m_", 4))
			{ rt=22050; cs+=4; }
		if(!strncmp(cs, "32m_", 4))
			{ rt=32000; cs+=4; }
		if(!strncmp(cs, "44m_", 4))
			{ rt=44100; cs+=4; }

		if(!strncmp(cs, "8s_", 3))
			{ rt=8000; ch1=2; cs+=3; }
		if(!strncmp(cs, "11s_", 4))
			{ rt=16000; ch1=2; cs+=4; }
		if(!strncmp(cs, "16s_", 4))
			{ rt=16000; ch1=2; cs+=4; }
		if(!strncmp(cs, "22s_", 4))
			{ rt=22050; ch1=2; cs+=4; }
		if(!strncmp(cs, "32s_", 4))
			{ rt=32000; ch1=2; cs+=4; }
		if(!strncmp(cs, "44s_", 4))
			{ rt=44100; ch1=2; cs+=4; }

		if(!strcmp(cs, "16"))
			fm1=0;
		if(!strcmp(cs, "8"))
			fm1=1;
		if(!strcmp(cs, "alaw"))
			fm1=2;
		if(!strcmp(cs, "adpcm"))
			fm1=3;
		if(!strcmp(cs, "adlq"))
			fm1=4;
		if(!strcmp(cs, "adpcm2b"))
			fm1=5;

		if(ch1==2)
		{
			sbuf=BGBCC_WAVE_LoadWAV_RateStereo16(buf, sz, rt, &len);
			obuf=malloc(256+len*2*2);
		}else
		{
			sbuf=BGBCC_WAVE_LoadWAV_RateMono16(buf, sz, rt, &len);
			obuf=malloc(256+len*2);
		}

		if(fm1==0)
		{
			sz1=BGBCC_WAVE_StoreWavePCM(obuf, (byte *)sbuf, ch1, rt, 16, len);
		}
		if(fm1==1)
		{
			sz1=BGBCC_WAVE_StoreWaveCnvPcm8(
				obuf, sbuf, ch1, rt, len);
		}

		if(fm1==2)
		{
			sz1=BGBCC_WAVE_StoreWaveCnvALaw(
				obuf, sbuf, ch1, rt, len);
		}

		if(fm1==3)
		{
			sz1=BGBCC_WAVE_StoreWaveImaAdpcm(
				obuf, sbuf, ch1, rt, len);
		}

		if(fm1==4)
		{
			sz1=BGBCC_WAVE_StoreWaveAdlq(
				obuf, sbuf, ch1, rt, len);
		}

		if(fm1==5)
		{
			sz1=BGBCC_WAVE_StoreWaveImaAdpcm2b(
				obuf, sbuf, ch1, rt, len);
		}

		*rfcc=BGBCC_FMT_WAV;
		*rsz=sz1;
		return(obuf);
	}
	
	return(NULL);
}

