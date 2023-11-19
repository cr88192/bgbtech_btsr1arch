// TK_CONWIDTH

u16 tkgdi_fontcell_3x5[128]={
000000, 005020, 072752, 005772, 002000, 000200, 002527, 002727,	//00-07
075057, 002520, 075257, 001252, 025272, 007440, 007550, 050505,	//08-0F
046764, 013731, 052525, 055505, 035311, 072527, 000077, 077777,	//10-17
072200, 000227, 001710, 004740, 000470, 001740, 002777, 077720,	//18-1F
000000, 022202, 055000, 005750, 027272, 051245, 025257, 022000,	//20-27
024442, 021112, 005250, 002720, 000224, 000700, 000220, 011244,	//28-2F
075557, 026227, 071747, 071717, 055711, 074717, 074757, 071111,	//30-37
075757, 075711, 002020, 002024, 012421, 007070, 042124, 025122,	//38-3F
075547, 025755, 065656, 034443, 065556, 074747, 074744, 034553,	//40-47
055755, 072227, 011116, 055655, 044447, 057755, 007555, 025552,	//48-4F
075744, 075561, 075655, 034216, 072222, 055557, 055552, 055775,	//50-57
055255, 055222, 071247, 064446, 044211, 031113, 025000, 000007,	//58-5F
022100, 003553, 044757, 003443, 011757, 025743, 034644, 025716,	//60-67
044655, 020222, 020224, 004565, 002222, 005755, 002555, 002552,	//68-6F
006564, 003531, 047444, 006426, 027221, 005557, 005552, 005575,	//70-77
005225, 005522, 006246, 012621, 022022, 042324, 003400, 000257	//78-7F
};

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

void tkg_drawcellbuf_4x6(u16 *buf, int xs, int ys,
	int x, int y, u16 px, u16 fgc, u16 bgc)
{
	u16 *ct;
	u16 p0, p1, p2;
	
	ct=buf+(y*xs+x);

	ct[0]=bgc;	ct[1]=bgc;	ct[2]=bgc;	ct[3]=bgc;
	ct+=xs;

	p0=bgc; p1=bgc; p2=bgc;
	if(px&040000)	p0=fgc;
	if(px&020000)	p1=fgc;
	if(px&010000)	p2=fgc;
	ct[0]=p0;	ct[1]=p1;	ct[2]=p2;	ct[3]=bgc;
	ct+=xs;

	p0=bgc; p1=bgc; p2=bgc;
	if(px&004000)	p0=fgc;
	if(px&002000)	p1=fgc;
	if(px&001000)	p2=fgc;
	ct[0]=p0;	ct[1]=p1;	ct[2]=p2;	ct[3]=bgc;
	ct+=xs;

	p0=bgc; p1=bgc; p2=bgc;
	if(px&000400)	p0=fgc;
	if(px&000200)	p1=fgc;
	if(px&000100)	p2=fgc;
	ct[0]=p0;	ct[1]=p1;	ct[2]=p2;	ct[3]=bgc;
	ct+=xs;

	p0=bgc; p1=bgc; p2=bgc;
	if(px&000040)	p0=fgc;
	if(px&000020)	p1=fgc;
	if(px&000010)	p2=fgc;
	ct[0]=p0;	ct[1]=p1;	ct[2]=p2;	ct[3]=bgc;
	ct+=xs;

	p0=bgc; p1=bgc; p2=bgc;
	if(px&000004)	p0=fgc;
	if(px&000002)	p1=fgc;
	if(px&000001)	p2=fgc;
	ct[0]=p0;	ct[1]=p1;	ct[2]=p2;	ct[3]=bgc;
	ct+=xs;
}

void tkg_drawcellbuf_6x8(u16 *buf, int xs, int ys,
	int x, int y, u32 px0, u16 fgc, u16 bgc)
{
	u16 *ct;
	u64 px;
	u16 px1, p0, p1, p2, p3, p4;
	
	px=((u64)(px0&0x3FFFFFFFU))<<5;
	if(px0&0x40000000)
		px>>=5;
	
	ct=buf+(y*xs+x);

	px1=px>>35;
	p0=bgc; p1=bgc; p2=bgc; p3=bgc; p4=bgc;
	if(px1&0x10)	p0=fgc;
	if(px1&0x08)	p1=fgc;
	if(px1&0x04)	p2=fgc;
	if(px1&0x02)	p3=fgc;
	if(px1&0x01)	p4=fgc;
	ct[0]=p0;	ct[1]=p1;	ct[2]=p2;
	ct[3]=p3;	ct[4]=p4;	ct[5]=bgc;
	ct+=xs;

	px1=px>>30;
	p0=bgc; p1=bgc; p2=bgc; p3=bgc; p4=bgc;
	if(px1&0x10)	p0=fgc;
	if(px1&0x08)	p1=fgc;
	if(px1&0x04)	p2=fgc;
	if(px1&0x02)	p3=fgc;
	if(px1&0x01)	p4=fgc;
	ct[0]=p0;	ct[1]=p1;	ct[2]=p2;
	ct[3]=p3;	ct[4]=p4;	ct[5]=bgc;
	ct+=xs;

	px1=px>>25;
	p0=bgc; p1=bgc; p2=bgc; p3=bgc; p4=bgc;
	if(px1&0x10)	p0=fgc;
	if(px1&0x08)	p1=fgc;
	if(px1&0x04)	p2=fgc;
	if(px1&0x02)	p3=fgc;
	if(px1&0x01)	p4=fgc;
	ct[0]=p0;	ct[1]=p1;	ct[2]=p2;
	ct[3]=p3;	ct[4]=p4;	ct[5]=bgc;
	ct+=xs;

	px1=px>>20;
	p0=bgc; p1=bgc; p2=bgc; p3=bgc; p4=bgc;
	if(px1&0x10)	p0=fgc;
	if(px1&0x08)	p1=fgc;
	if(px1&0x04)	p2=fgc;
	if(px1&0x02)	p3=fgc;
	if(px1&0x01)	p4=fgc;
	ct[0]=p0;	ct[1]=p1;	ct[2]=p2;
	ct[3]=p3;	ct[4]=p4;	ct[5]=bgc;
	ct+=xs;

	px1=px>>15;
	p0=bgc; p1=bgc; p2=bgc; p3=bgc; p4=bgc;
	if(px1&0x10)	p0=fgc;
	if(px1&0x08)	p1=fgc;
	if(px1&0x04)	p2=fgc;
	if(px1&0x02)	p3=fgc;
	if(px1&0x01)	p4=fgc;
	ct[0]=p0;	ct[1]=p1;	ct[2]=p2;
	ct[3]=p3;	ct[4]=p4;	ct[5]=bgc;
	ct+=xs;

	px1=px>>10;
	p0=bgc; p1=bgc; p2=bgc; p3=bgc; p4=bgc;
	if(px1&0x10)	p0=fgc;
	if(px1&0x08)	p1=fgc;
	if(px1&0x04)	p2=fgc;
	if(px1&0x02)	p3=fgc;
	if(px1&0x01)	p4=fgc;
	ct[0]=p0;	ct[1]=p1;	ct[2]=p2;
	ct[3]=p3;	ct[4]=p4;	ct[5]=bgc;
	ct+=xs;

	px1=px>>5;
	p0=bgc; p1=bgc; p2=bgc; p3=bgc; p4=bgc;
	if(px1&0x10)	p0=fgc;
	if(px1&0x08)	p1=fgc;
	if(px1&0x04)	p2=fgc;
	if(px1&0x02)	p3=fgc;
	if(px1&0x01)	p4=fgc;
	ct[0]=p0;	ct[1]=p1;	ct[2]=p2;
	ct[3]=p3;	ct[4]=p4;	ct[5]=bgc;
	ct+=xs;

	px1=px>>0;
	p0=bgc; p1=bgc; p2=bgc; p3=bgc; p4=bgc;
	if(px1&0x10)	p0=fgc;
	if(px1&0x08)	p1=fgc;
	if(px1&0x04)	p2=fgc;
	if(px1&0x02)	p3=fgc;
	if(px1&0x01)	p4=fgc;
	ct[0]=p0;	ct[1]=p1;	ct[2]=p2;
	ct[3]=p3;	ct[4]=p4;	ct[5]=bgc;
	ct+=xs;
}

void tkg_drawcellbuf_8x8(u16 *buf, int xs, int ys,
	int x, int y, u64 px, u16 fgc, u16 bgc)
{
	u16 *ct;
	byte px1;
	u16 p0, p1, p2, p3;

	ct=buf+(y*xs+x);

	px1=px>>56;
	p0=bgc; p1=bgc; p2=bgc; p3=bgc;
	if(px1&0x80)	p0=fgc;
	if(px1&0x40)	p1=fgc;
	if(px1&0x20)	p2=fgc;
	if(px1&0x10)	p3=fgc;
	ct[0]=p0;	ct[1]=p1;	ct[2]=p2;	ct[3]=p3;
	p0=bgc; p1=bgc; p2=bgc; p3=bgc;
	if(px1&0x08)	p0=fgc;
	if(px1&0x04)	p1=fgc;
	if(px1&0x02)	p2=fgc;
	if(px1&0x01)	p3=fgc;
	ct[4]=p0;	ct[5]=p1;	ct[6]=p2;	ct[7]=p3;
	ct+=xs;

	px1=px>>48;
	p0=bgc; p1=bgc; p2=bgc; p3=bgc;
	if(px1&0x80)	p0=fgc;
	if(px1&0x40)	p1=fgc;
	if(px1&0x20)	p2=fgc;
	if(px1&0x10)	p3=fgc;
	ct[0]=p0;	ct[1]=p1;	ct[2]=p2;	ct[3]=p3;
	p0=bgc; p1=bgc; p2=bgc; p3=bgc;
	if(px1&0x08)	p0=fgc;
	if(px1&0x04)	p1=fgc;
	if(px1&0x02)	p2=fgc;
	if(px1&0x01)	p3=fgc;
	ct[4]=p0;	ct[5]=p1;	ct[6]=p2;	ct[7]=p3;
	ct+=xs;

	px1=px>>40;
	p0=bgc; p1=bgc; p2=bgc; p3=bgc;
	if(px1&0x80)	p0=fgc;
	if(px1&0x40)	p1=fgc;
	if(px1&0x20)	p2=fgc;
	if(px1&0x10)	p3=fgc;
	ct[0]=p0;	ct[1]=p1;	ct[2]=p2;	ct[3]=p3;
	p0=bgc; p1=bgc; p2=bgc; p3=bgc;
	if(px1&0x08)	p0=fgc;
	if(px1&0x04)	p1=fgc;
	if(px1&0x02)	p2=fgc;
	if(px1&0x01)	p3=fgc;
	ct[4]=p0;	ct[5]=p1;	ct[6]=p2;	ct[7]=p3;
	ct+=xs;

	px1=px>>32;
	p0=bgc; p1=bgc; p2=bgc; p3=bgc;
	if(px1&0x80)	p0=fgc;
	if(px1&0x40)	p1=fgc;
	if(px1&0x20)	p2=fgc;
	if(px1&0x10)	p3=fgc;
	ct[0]=p0;	ct[1]=p1;	ct[2]=p2;	ct[3]=p3;
	p0=bgc; p1=bgc; p2=bgc; p3=bgc;
	if(px1&0x08)	p0=fgc;
	if(px1&0x04)	p1=fgc;
	if(px1&0x02)	p2=fgc;
	if(px1&0x01)	p3=fgc;
	ct[4]=p0;	ct[5]=p1;	ct[6]=p2;	ct[7]=p3;
	ct+=xs;

	px1=px>>24;
	p0=bgc; p1=bgc; p2=bgc; p3=bgc;
	if(px1&0x80)	p0=fgc;
	if(px1&0x40)	p1=fgc;
	if(px1&0x20)	p2=fgc;
	if(px1&0x10)	p3=fgc;
	ct[0]=p0;	ct[1]=p1;	ct[2]=p2;	ct[3]=p3;
	p0=bgc; p1=bgc; p2=bgc; p3=bgc;
	if(px1&0x08)	p0=fgc;
	if(px1&0x04)	p1=fgc;
	if(px1&0x02)	p2=fgc;
	if(px1&0x01)	p3=fgc;
	ct[4]=p0;	ct[5]=p1;	ct[6]=p2;	ct[7]=p3;
	ct+=xs;

	px1=px>>16;
	p0=bgc; p1=bgc; p2=bgc; p3=bgc;
	if(px1&0x80)	p0=fgc;
	if(px1&0x40)	p1=fgc;
	if(px1&0x20)	p2=fgc;
	if(px1&0x10)	p3=fgc;
	ct[0]=p0;	ct[1]=p1;	ct[2]=p2;	ct[3]=p3;
	p0=bgc; p1=bgc; p2=bgc; p3=bgc;
	if(px1&0x08)	p0=fgc;
	if(px1&0x04)	p1=fgc;
	if(px1&0x02)	p2=fgc;
	if(px1&0x01)	p3=fgc;
	ct[4]=p0;	ct[5]=p1;	ct[6]=p2;	ct[7]=p3;
	ct+=xs;

	px1=px>>8;
	p0=bgc; p1=bgc; p2=bgc; p3=bgc;
	if(px1&0x80)	p0=fgc;
	if(px1&0x40)	p1=fgc;
	if(px1&0x20)	p2=fgc;
	if(px1&0x10)	p3=fgc;
	ct[0]=p0;	ct[1]=p1;	ct[2]=p2;	ct[3]=p3;
	p0=bgc; p1=bgc; p2=bgc; p3=bgc;
	if(px1&0x08)	p0=fgc;
	if(px1&0x04)	p1=fgc;
	if(px1&0x02)	p2=fgc;
	if(px1&0x01)	p3=fgc;
	ct[4]=p0;	ct[5]=p1;	ct[6]=p2;	ct[7]=p3;
	ct+=xs;

	px1=px>>0;
	p0=bgc; p1=bgc; p2=bgc; p3=bgc;
	if(px1&0x80)	p0=fgc;
	if(px1&0x40)	p1=fgc;
	if(px1&0x20)	p2=fgc;
	if(px1&0x10)	p3=fgc;
	ct[0]=p0;	ct[1]=p1;	ct[2]=p2;	ct[3]=p3;
	p0=bgc; p1=bgc; p2=bgc; p3=bgc;
	if(px1&0x08)	p0=fgc;
	if(px1&0x04)	p1=fgc;
	if(px1&0x02)	p2=fgc;
	if(px1&0x01)	p3=fgc;
	ct[4]=p0;	ct[5]=p1;	ct[6]=p2;	ct[7]=p3;
	ct+=xs;
}

void tkgdi_con_drawcell(_tkgdi_conparm *con, int x, int y)
{
	u16 *pixb;
	u64 q0, q1, pix;
	int cxs, cys;
	int i0, i1, fgc, bgc;
	int px, py, pz, ch;
	int i, j, k;

	cxs=con->cell_xs;
	cys=con->cell_ys;

	i0=(y*TK_CONWIDTH+x)*2;
	q0=con->conbuf[i0+0];
	q1=con->conbuf[i0+1];

	px=x*cxs;
	py=y*cys;
	pz=(py*(TK_CONWIDTH*8))+px;

	if(((q0>>30)&3)==0)
	{
		ch=q0&1023;
		fgc=tk_con_clr64to555[(q0>>16)&63];
		bgc=tk_con_clr64to555[(q0>>22)&63];
		pix=TK_Con_GlyphForCodepoint(ch);
	}else
		if(((q0>>30)&3)==2)
	{
		fgc=(q0>> 0)&0x7FFF;
		bgc=(q0>>15)&0x7FFF;
		pix=q1;
	}else
	{
	}


	pixb=con->pixbuf;
	if(!pixb)
		return;

	if(cys==8)
	{
		if(cxs==8)
		{
			tkg_drawcellbuf_8x8(
				pixb, TK_CONWIDTH*8, TK_CONHEIGHT*8,
				px, py, 
				pix, fgc, bgc);
		}

		if(cxs==6)
		{
			j=ch-0x20;
			if((j<0) || (j>95))
				j=0;
			pz=tk_con_glyphs_5x6seg[j];
			tkg_drawcellbuf_6x8(
				pixb, TK_CONWIDTH*6, TK_CONHEIGHT*8,
				px, py, 
				pz, fgc, bgc);
		}
	}else
		if(cys==6)
	{
		if(cxs==4)
		{
			j=ch;
			if((j<0) || (j>127))
				j=0;
			pz=tkgdi_fontcell_3x5[j];
			tkg_drawcellbuf_4x6(
				pixb, TK_CONWIDTH*4, TK_CONHEIGHT*6,
				px, py, 
				pz, fgc, bgc);
		}
	}

#if 0
	for(i=0; i<8; i++)
	{
		for(j=0; j<8; j++)
		{
			k=bgc;
			if((pix>>(63-(i*8+j)))&1)
				k=fgc;
			pixb[pz+j]=k;
		}
		pz+=TK_CONWIDTH*8;
	}
#endif
}

void tkgdi_con_redrawbuffer(_tkgdi_conparm *con)
{
	u64 q, qrm;
	int i, j, k;
	
	qrm=con->conrowmask;
	if(!qrm)
		return;
	
	for(i=0; i<TK_CONHEIGHT; i++)
	{
		if(!((qrm>>i)&1))
			continue;

		for(j=0; j<TK_CONWIDTH; j++)
		{
			k=i*TK_CONWIDTH+j;
			q=con->conmask[k>>6];
			if(!q)
			{
				j=((j+64)&(~63))-1;
				continue;
			}
			
			if(!((q>>(k&63))&1))
				continue;
			
			q&=~(1LL<<(k&63));
			con->conmask[k>>6]=q;
			
			tkgdi_con_drawcell(con, j, i);
		}
	}
	
	con->conrowmask=0;
}

void TKGDI_Con_UpdateHwCursor(_tkgdi_conparm *con)
{
	tkgdi_con_redrawbuffer(con);
}

void tkgdi_con_clear(_tkgdi_conparm *con)
{
	volatile u32 *buf;
	u64 q0, q1, tv;
//	u32 p0, p1, p2, p3;
	int i0, i1;
	int i, j, k;

	i1=con->text_attr;

	buf=(volatile u32 *)(con->conbuf);

	q1=0;
	q0=(2ULL<<30)|(con->bgclr_555<<15)|con->fgclr_555;

	for(i=0; i<25; i++)
	{
		i0=(i*TK_CONWIDTH)*4;
		for(j=0; j<TK_CONWIDTH; j++)
		{
			*(u64 *)(buf+i0+0)=q0;
			*(u64 *)(buf+i0+2)=q1;
			i0+=4;
			
			i1=i0>>2;
			con->conmask[i1>>6]|=1<<(i1&63);
		}
	}
	
	con->conrowmask=-1;

	tkgdi_con_redrawbuffer(con);
}

void TKGDI_Con_SetCursorPos(_tkgdi_conparm *con, int x, int y)
{
	con->x=x;
	con->y=y;
	TKGDI_Con_UpdateHwCursor(con);
}

void TKGDI_Con_SetColorFg(_tkgdi_conparm *con, int x)
{
	int cr, cg, cb, cc;
	con->text_attr=(con->text_attr&0xFFC0FFFFU)|((x&0x3F)<<16);
	
	cr=x&0x30; cg=x&0x0C; cb=x&0x03;
	cc=(cr<<9)|(cr<<7)|(cg<<6)|(cg<<4)|(cb<<3)|(cb<<1);
	con->fgclr_555=cc;
}

void TKGDI_Con_SetColorBg(_tkgdi_conparm *con, int x)
{
	int cr, cg, cb, cc;
	con->text_attr=(con->text_attr&0xF03FFFFFU)|((x&0x3F)<<22);
	cr=x&0x30; cg=x&0x0C; cb=x&0x03;
	cc=(cr<<9)|(cr<<7)|(cg<<6)|(cg<<4)|(cb<<3)|(cb<<1);
	con->bgclr_555=cc;
}

void tkgdi_con_init(_tkgdi_conparm *con)
{
	int i, j, k;

	con->text_attr_dfl=0x002A0000;
	con->text_attr=con->text_attr_dfl;
	con->ena=1;

	TKGDI_Con_SetColorBg(con, 0);
	TKGDI_Con_SetColorFg(con, tk_con_clr16to64[7]);
	
	con->conbuf=tk_malloc(80*25*(2*8));
//	con->pixbuf=tk_malloc((80*8)*(25*8)*2);
	
	j=((80*25+63)>>6);
	con->conmask=tk_malloc(j*8);
	memset(con->conmask, 0xFF, j*8);

	con->conrowmask=-1;
}

void tkgdi_con_reset(_tkgdi_conparm *con)
{
	tkgdi_con_init(con);
	tkgdi_con_clear(con);
	TKGDI_Con_SetCursorPos(con, 0, 0);
	con->text_attr=con->text_attr_dfl;
	TKGDI_Con_SetColorBg(con, 0);
	TKGDI_Con_SetColorFg(con, tk_con_clr16to64[7]);
}

void tkgdi_con_chkreset(_tkgdi_conparm *con)
{
	if(!con->ena)
	{
		tkgdi_con_init(con);
		tkgdi_con_clear(con);
		TKGDI_Con_SetCursorPos(con, 0, 0);

		con->text_attr=con->text_attr_dfl;
		TKGDI_Con_SetColorBg(con, 0);
		TKGDI_Con_SetColorFg(con, tk_con_clr16to64[7]);
	}
}

void tkgdi_con_scroll_up(_tkgdi_conparm *con)
{
	volatile u32 *buf;
	volatile u64 *qb0, *qb1;
	u64 q0, q1, q2, q3;
//	u32 p0, p1, p2, p3;
	int i0, i1;
	int i, j, k;

	buf=(volatile u32 *)(con->conbuf);

	for(i=0; i<TK_CONHEIGHTN1; i++)
	{
		i0=((i+0)*TK_CONWIDTH)*4;
		i1=((i+1)*TK_CONWIDTH)*4;

		qb0=(u64 *)(buf+i0);
		qb1=(u64 *)(buf+i1);
		for(j=0; j<TK_CONWIDTH; j+=4)
		{
			q0=qb1[0];		q1=qb1[1];
			q2=qb1[2];		q3=qb1[3];
			qb0[0]=q0;		qb0[1]=q1;
			qb0[2]=q2;		qb0[3]=q3;

			q0=qb1[4];		q1=qb1[5];
			q2=qb1[6];		q3=qb1[7];
			qb0[4]=q0;		qb0[5]=q1;
			qb0[6]=q2;		qb0[7]=q3;

			qb0+=8;		qb1+=8;
			i0+=8;		i1+=8;

			k=(i1>>2)+j;
			con->conmask[k>>6]|=15<<(k&63);
		}
	}

	q0=(2ULL<<30)|(con->bgclr_555<<15)|con->fgclr_555;
	q1=0;

	i0=(TK_CONHEIGHTN1*TK_CONWIDTH)*4;
	i1=con->text_attr;
	for(j=0; j<TK_CONWIDTH; j++)
	{
		k=(i0>>1)+j;
		con->conmask[k>>6]|=1<<(k&63);

		((u64 *)(buf+i0))[0]=q0;
		((u64 *)(buf+i0))[1]=q1;
		i0+=2;
	}

	con->conrowmask=-1;

	tkgdi_con_redrawbuffer(con);
}

void tkgdi_con_scroll_down(_tkgdi_conparm *con)
{
	volatile u32 *buf;
	u64 q0, q1;
//	u32 p0, p1, p2, p3;
	int i0, i1;
	int i, j, k;

//	buf=con->buf;
	buf=(volatile u32 *)(con->conbuf);

	for(i=TK_CONHEIGHTN1; i>0; i--)
	{
		i0=((i+0)*TK_CONWIDTH)*2;
		i1=((i-1)*TK_CONWIDTH)*2;
		for(j=0; j<TK_CONWIDTH; j++)
		{
			q0=((u64 *)(buf+i1))[0];
			q1=((u64 *)(buf+i1))[1];
			((u64 *)(buf+i0))[0]=q0;
			((u64 *)(buf+i0))[1]=q1;

			k=(i0>>1)+j;
			con->conmask[k>>6]|=15<<(k&63);

			i0+=2;
			i1+=2;
		}
	}

	q1=0;
	q0=(2ULL<<30)|(con->bgclr_555<<15)|con->fgclr_555;

	i0=0;
	i1=con->text_attr;
	for(j=0; j<TK_CONWIDTH; j++)
	{
		((u64 *)(buf+i0))[0]=q0;
		((u64 *)(buf+i0))[1]=q1;

		k=(i0>>1)+j;
		con->conmask[k>>6]|=1<<(k&63);

		i0+=2;
	}

	con->conrowmask=-1;

	tkgdi_con_redrawbuffer(con);
}

void tkgdi_con_newline(_tkgdi_conparm *con)
{
	con->y++;
	if(con->y>=25)
	{
		tkgdi_con_scroll_up(con);
		con->y--;
	}
}

void TKGDI_Con_PutcEscapeSgr(_tkgdi_conparm *con, int mode)
{
	switch(mode)
	{
	case 0:
		con->text_attr=con->text_attr_dfl;
		break;
	case 1:
		con->text_attr|=0x003F0000;
		break;
	case 2:
		break;
	case 3:
	case 7:
		con->text_attr|=0x00002000;
		break;
	case 4:
		con->text_attr|=0x00000800;
		break;
	case 5:
		con->text_attr&=~0x0000C000;
		con->text_attr|= 0x00008000;
		break;
	case 6:
		con->text_attr&=~0x0000C000;
		con->text_attr|= 0x00004000;
		break;
	case 9:
		con->text_attr|=0x00000400;
		break;
	case 10:
		con->text_attr&=~0x00000300;
		break;
	
	case 22:
		con->text_attr=
			(con->text_attr&(~0x003F0000))|
			(con->text_attr_dfl&0x003F0000);
		break;
	case 23:
		con->text_attr&=~0x00002000;
		break;
	case 24:
		con->text_attr&=~0x00000800;
		break;
	case 25:
		con->text_attr&=~0x0000C000;
		break;
	case 29:
		con->text_attr&=~0x00000400;
		break;
	
	case 30:	case 31:
	case 32:	case 33:
	case 34:	case 35:
	case 36:	case 37:
		TKGDI_Con_SetColorFg(con, tk_con_clr16to64[mode-30]);
		break;
	case 39:
		TKGDI_Con_SetColorFg(con, 0x2A);
		break;		
	case 40:	case 41:
	case 42:	case 43:
	case 44:	case 45:
	case 46:	case 47:
		TKGDI_Con_SetColorBg(con, tk_con_clr16to64[mode-40]);
		break;
	case 49:
		TKGDI_Con_SetColorBg(con, 0);
		break;		

	case 90:	case 91:
	case 92:	case 93:
	case 94:	case 95:
	case 96:	case 97:
		TKGDI_Con_SetColorFg(con, tk_con_clr16to64[8+(mode-90)]);
		break;
	case 100:	case 101:
	case 102:	case 103:
	case 104:	case 105:
	case 106:	case 107:
		TKGDI_Con_SetColorBg(con, tk_con_clr16to64[8+(mode-100)]);
		break;
	}
}

void TKGDI_Con_PutcEscape(_tkgdi_conparm *con, int ch)
{
	int i, j, k;

	if(con->isesc==1)
	{
		if(ch=='[')
		{
			con->escval3=0;
			con->escval2=0;
			con->escval1=0;
			con->escval0=0;
			con->isesc=2;
			return;
		}
		
		con->isesc=0;
		return;
	}

	if(con->isesc==2)
	{
		switch(ch)
		{
		case '0':	case '1':
		case '2':	case '3':
		case '4':	case '5':
		case '6':	case '7':
		case '8':	case '9':
			con->escval0=(con->escval0*10)+(ch-'0');
			break;
		case ';':
			con->escval3=con->escval2;
			con->escval2=con->escval1;
			con->escval1=con->escval0;
			con->escval0=0;
			break;
		case 'A':
			i=con->escval0;
			if(!i)i=1;
			TKGDI_Con_SetCursorPos(con, con->x, con->y-i);
			con->isesc=0;
			break;
		case 'B':
			i=con->escval0;
			if(!i)i=1;
			TKGDI_Con_SetCursorPos(con, con->x, con->y+i);
			con->isesc=0;
			break;
		case 'C':
			i=con->escval0;
			if(!i)i=1;
			TKGDI_Con_SetCursorPos(con, con->x+i, con->y);
			con->isesc=0;
			break;
		case 'D':
			i=con->escval0;
			if(!i)i=1;
			TKGDI_Con_SetCursorPos(con, con->x-i, con->y);
			con->isesc=0;
			break;

		case 'E':
			i=con->escval0;
			if(!i)i=1;
			TKGDI_Con_SetCursorPos(con, 0, con->y+i);
			con->isesc=0;
			break;
		case 'F':
			i=con->escval0;
			if(!i)i=1;
			TKGDI_Con_SetCursorPos(con, 0, con->y-i);
			con->isesc=0;
			break;
		case 'G':
			i=con->escval0;
			if(!i)i=1;
			TKGDI_Con_SetCursorPos(con, i-1, con->y);
			con->isesc=0;
			break;
		case 'H':
		case 'f':
			i=con->escval0;
			j=con->escval1;
			if(!i)i=1;
//			if(!j)j=1;
			if(!j) { j=i; i=1; }
			TKGDI_Con_SetCursorPos(con, i-1, j-1);
			con->isesc=0;
			break;

		case 'J':
			switch(con->escval0)
			{
			case 2:
			case 3:
				tkgdi_con_clear(con);
				break;
			}
			con->isesc=0;
			break;

		case 'S':
			i=con->escval0;
			if(!i)i=1;
			while(i--)
				{ tkgdi_con_scroll_up(con); }
			con->isesc=0;
			break;
		case 'T':
			i=con->escval0;
			if(!i)i=1;
			while(i--)
				{ tkgdi_con_scroll_down(con); }
			con->isesc=0;
			break;

		case 'm':
			TKGDI_Con_PutcEscapeSgr(con, con->escval0);
			con->isesc=0;
			break;

		default:
			con->isesc=0;
			break;
		}
		return;
	}

	con->isesc=0;
}

void tkgdi_con_putc(_tkgdi_conparm *con, int ch)
{
	volatile u32 *buf;
	int tx, ty, tz;
	u64 q0, q1;
	u32 px, py;
	int i, j, k;
	
	if(!con->ena)
		return;

	buf=(volatile u32 *)(con->conbuf);

	if(con->isesc)
	{
		TKGDI_Con_PutcEscape(con, ch);
		return;
	}
	
	con->dirty=1;

	if(ch<' ')
	{
		if(ch=='\b')
		{
			if(con->x>0)con->x--;
			TKGDI_Con_UpdateHwCursor(con);

			return;
		}

		if(ch=='\r')
		{
			con->x=0;
			TKGDI_Con_UpdateHwCursor(con);
			return;
		}
		if(ch=='\n')
		{
			tkgdi_con_newline(con);
			TKGDI_Con_UpdateHwCursor(con);
			return;
		}
		if(ch=='\t')
		{
			con->x=(con->x+8)&(~7);
			if(con->x>=TK_CONWIDTH)
				{ tkgdi_con_newline(con); con->x=0; }
			TKGDI_Con_UpdateHwCursor(con);
			return;
		}

		if(ch=='\x1B')
		{
			con->isesc=1;
			return;
		}

		if(ch=='\x1f')
		{
			px=0x003F8000|'_';
			ty=con->y;
			tx=con->x;
			tz=(ty*TK_CONWIDTH+tx)*4;
			buf[tz+0]=px;
			tx++;
			con->x=tx;

			k=tz>>1;
			con->conmask[k>>6]|=1<<(k&63);
			con->conrowmask|=1LL<<ty;
			return;
		}

		return;
	}

	px=(con->text_attr)|ch;

	ty=con->y;
	tx=con->x;

	con->conrowmask|=1LL<<ty;

	if(ch<0x100)
	{
		tz=(ty*TK_CONWIDTH+tx)*2;
		((u64 *)buf)[tz]=px;

		k=tz>>1;
		con->conmask[k>>6]|=1<<(k&63);
	}else
	{
		if(ch>=0x100)
		{
			ch=tk_con_doremap(ch);
		}

		if((ch>=0x00) && (ch<=0x7F))
		{
			q1=tk_gfxcon_glyphs[ch];
		}else
			if((ch>=0x80) && (ch<=0xFF))
		{
			q1=tk_gfxcon_glyphs_lat1ext[ch&0x7F];
		}else
			if((ch>=0x100) && (ch<=0x17F))
		{
			q1=tk_gfxcon_glyphs_cyril0[ch&0x7F];
		}else
			if((ch>=0x180) && (ch<=0x1FF))
		{
			q1=tk_gfxcon_glyphs_437ext[ch&0x7F];
		}else
			if((ch>=0x0600) && (ch<=0x06FF))
		{
			q1=tk_gfxcon_hexblock[ch&0xFF];
		}else
		{
			q1=tk_gfxcon_glyphs[0xBF];
		}

		q0=(2ULL<<30)|(con->bgclr_555<<15)|con->fgclr_555;

		tz=(ty*TK_CONWIDTH+tx)*2;
		((u64 *)buf)[tz+0]=q0;
		((u64 *)buf)[tz+1]=q1;

		k=tz>>1;
		con->conmask[k>>6]|=1<<(k&63);
	}

	tx++;
	con->x=tx;
	if(tx>=TK_CONWIDTH)
	{
		tkgdi_con_newline(con);
		con->x=0;
	}

	TKGDI_Con_UpdateHwCursor(con);
}
