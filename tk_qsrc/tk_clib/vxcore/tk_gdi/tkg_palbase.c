int TKGDI_SetupPal8Default32(byte *pal)
{
	int i, j, k, chi, cmi, clo;

	for(i=0; i<16; i++)
		for(j=0; j<16; j++)
	{
		clo=0x00;	chi=j+(j<<4);
		if(i&8)
			clo=(chi*5)/8;
		
		k=i*16+j;
		pal[k*4+0]=(i&1)?chi:clo;
		pal[k*4+1]=(i&2)?chi:clo;
		pal[k*4+2]=(i&4)?chi:clo;
		pal[k*4+3]=255;
	}

	for(i=0; i<16; i++)
	{
		chi=(i<<4);
		k=i+15*16;
		pal[k*4+0]=chi;
		pal[k*4+1]=chi;
		pal[k*4+2]=chi;
		pal[k*4+3]=255;
	}

	for(i=0; i<16; i++)
	{
		chi=i+(i<<4);
		cmi=(chi*5)/8;
		clo=(chi*3)/8;

		k=i+7*16;
		pal[k*4+0]=clo;
		pal[k*4+1]=cmi;
		pal[k*4+2]=chi;
		pal[k*4+3]=255;

		k=i+8*16;
		pal[k*4+0]=chi;
		pal[k*4+1]=cmi;
		pal[k*4+2]=clo;
		pal[k*4+3]=255;
	}

	for(i=0; i<16; i++)
	{
		chi=0xFF;
		clo=(chi*7)/8;
		k=i*16+0;
		pal[k*4+0]=(i&1)?chi:clo;
		pal[k*4+1]=(i&2)?chi:clo;
		pal[k*4+2]=(i&4)?chi:clo;
		pal[k*4+3]=255;
	}

	for(i=0; i<16; i++)
	{
		clo=0x00;	chi=0xAA;
		if(i&8)
			{ clo=0x55; chi=0xFF; }
		pal[i*4+0]=(i&1)?chi:clo;
		pal[i*4+1]=(i&2)?chi:clo;
		pal[i*4+2]=(i&4)?chi:clo;
		pal[i*4+3]=255;
	}

//	k=15*16+0;
	k=TKGDI_PALDFL_TRANSPARENT;
	pal[k*4+0]=0;
	pal[k*4+1]=0;
	pal[k*4+2]=0;
	pal[k*4+3]=0;

	return(0);
}

int TKGDI_SetupDibHead256Default(TKGDI_BITMAPINFOHEADER *dib, int xs, int ys)
{
	byte *pal;

	memset(dib, 0, sizeof(TKGDI_BITMAPINFOHEADER)+1024);

	dib->biSize=sizeof(TKGDI_BITMAPINFOHEADER);
	dib->biWidth=xs;
	dib->biHeight=ys;

	dib->biBitCount=8;
	dib->biClrUsed=256;
	dib->biClrImportant=256;

	pal=(byte *)(dib+1);
	TKGDI_SetupPal8Default32(pal);
	return(0);
}

byte tkgdi_default_pal256[1024];
byte *tkgdi_default_pal256_lut444;

int TKGDI_SetupPal8DefaultLookup()
{
	int cr, cg, cb, ci, cj;
	int dr, dg, db, d, bd, bi;

	if(tkgdi_default_pal256_lut444)
		return(0);

	TKGDI_SetupPal8Default32(tkgdi_default_pal256);
	tkgdi_default_pal256_lut444=malloc(4096);

	for(ci=0; ci<4096; ci++)
	{
		cr=(ci>>8)&15;	cg=(ci>>4)&15;	cb=(ci>>0)&15;
		cr=cr|(cr<<4);	cg=cg|(cg<<4);	cb=cb|(cb<<4);
		
		bi=0; bd=999999;
		for(cj=0; cj<256; cj++)
		{
			dr=tkgdi_default_pal256[cj*4+2]-cr;
			dg=tkgdi_default_pal256[cj*4+1]-cg;
			db=tkgdi_default_pal256[cj*4+0]-cb;
			d=(dr*dr)+(dg*dg)+(db*db);
			if(d<bd)
				{ bi=cj; bd=d; }
		}
		tkgdi_default_pal256_lut444[ci]=bi;
	}

	return(1);
}

int TKGDI_DflPal8IndexFromRgb555(u16 rgb)
{
	int ci;

	if(!tkgdi_default_pal256_lut444)
		TKGDI_SetupPal8DefaultLookup();
	ci=((rgb>>2)&0xF00)|((rgb>>1)&0x0F0)|((rgb>>0)&0x00F);
	return(tkgdi_default_pal256_lut444[ci]);
}

int TKGDI_DflPal8IndexFromRgb32(u32 rgb)
{
	int ci;

	if(!tkgdi_default_pal256_lut444)
		TKGDI_SetupPal8DefaultLookup();
	ci=((rgb>>12)&0xF00)|((rgb>>8)&0x0F0)|((rgb>>4)&0x00F);
	return(tkgdi_default_pal256_lut444[ci]);
}
