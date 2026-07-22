byte *gfxedit_fixedsdf;
u32 gfxedit_fixedsdf_pal[256];
byte gfxedit_fixedsdf_didinit;


void GfxEdit_InitSdf()
{
	u32 rgb[384];
	byte *buf, *ibuf;
	int sz, xs, ys;

	if(gfxedit_fixedsdf_didinit)
		return;
	gfxedit_fixedsdf_didinit=1;

	buf=GfxEdit_LoadFile(NULL, "fixsdf1_hi8.bmp", &sz);
	if(buf)
	{
		ibuf=GfxEdit_DecodeBMP8(buf, rgb, &xs, &ys, 8);
		
		if(ibuf && (xs==256) && (ys==256))
		{
			gfxedit_fixedsdf=ibuf;
			memcpy(gfxedit_fixedsdf_pal, rgb, 256*4);
		}
	}
}

u32 GfxEdit_SdfRgbLerp2(u32 px0, u32 px1, u16 fr)
{
	int cr0, cg0, cb0, ca0, cr1, cg1, cb1, ca1, cr, cg, cb, ca;
	u32 px;

	ca0=(px0>>24)&255;	cr0=(px0>>16)&255;
	cg0=(px0>> 8)&255;	cb0=(px0>> 0)&255;
	ca1=(px1>>24)&255;	cr1=(px1>>16)&255;
	cg1=(px1>> 8)&255;	cb1=(px1>> 0)&255;
	cr=((cr0*(0xFFFF-fr))+(cr1*fr))>>16;
	cg=((cg0*(0xFFFF-fr))+(cg1*fr))>>16;
	cb=((cb0*(0xFFFF-fr))+(cb1*fr))>>16;
	ca=((ca0*(0xFFFF-fr))+(ca1*fr))>>16;
	px=(ca<<24)|(cr<<16)|(cg<<8)|cb;
	return(px);
}

u32 GfxEdit_SdfRgbLerp4(
	u32 px0, u32 px1, u32 px2, u32 px3,
	u16 frx, u16 fry)
{
	u32 px4, px5, px6;
	px4=GfxEdit_SdfRgbLerp2(px0, px1, frx);
	px5=GfxEdit_SdfRgbLerp2(px2, px3, frx);
	px6=GfxEdit_SdfRgbLerp2(px4, px5, fry);
	return(px6);
}

byte GfxEdit_SdfRgbToLevel(u32 rgb)
{
	int cr, cg, cb, c;
	
	cr=(rgb>>16)&255;	cg=(rgb>> 8)&255;	cb=(rgb>> 0)&255;
	c=(((cr-128)+(cg-128)+(cg-128))/4)+128;
	return(c);
}

void GfxEdit_SdfDrawGlyph(
	byte *imgbuf, int xs, int ys,
	int xorg, int yorg, int celxs, int celys,
	int chid, int clr)
{
	u32 px0, px1, px2, px3, px;
	int dx, dy, sxa, sya, sxb, syb, chx, chy,
		stsx, stsy, frx, fry, stfx, stfy;
	int v;

	if(chid<=0)
		return;
	if(chid>=256)
		return;
	
	chx=(chid&15)*16;
	chy=((chid>>4)&15)*16;
	
	stsx=(16*65536)/celxs;
	stsy=(16*65536)/celys;

//	stfx=(celxs*65536)/16;
//	stfy=(celys*65536)/16;

	stfx=65536;		stfy=65536;
	if(celxs<16)
		{ stfx=(celxs*celxs*65536)/256; }
	if(celys<16)
		{ stfy=(celys*celys*65536)/256; }

	for(dy=0; dy<celys; dy++)
	{
		if((yorg+dy)<  0)	continue;
		if((yorg+dy)>=ys)	continue;
		
		for(dx=0; dx<celxs; dx++)
		{
			if((xorg+dx)<  0)	continue;
			if((xorg+dx)>=xs)	continue;
		
			sxa=(dx*stsx);		sya=(dy*stsy);
			sxb=chx+(sxa>>16);	syb=chy+(sya>>16);
			frx=sxa&65535;		fry=sya&65535;
			frx=(((frx-0x8000)*stfx)>>16)+0x8000;
			fry=(((fry-0x8000)*stfy)>>16)+0x8000;
			px0=gfxedit_fixedsdf_pal[gfxedit_fixedsdf[(syb+0)*256+(sxb+0)]];
			px1=gfxedit_fixedsdf_pal[gfxedit_fixedsdf[(syb+0)*256+(sxb+1)]];
			px2=gfxedit_fixedsdf_pal[gfxedit_fixedsdf[(syb+1)*256+(sxb+0)]];
			px3=gfxedit_fixedsdf_pal[gfxedit_fixedsdf[(syb+1)*256+(sxb+1)]];
			px=GfxEdit_SdfRgbLerp4(px0, px1, px2, px3, frx, fry);
			v=GfxEdit_SdfRgbToLevel(px);
			if(v>=128)
				{ imgbuf[(yorg+dy)*xs+(xorg+dx)]=clr; }
		}
	}
}

void GfxEdit_SdfDrawString(
	byte *imgbuf, int xs, int ys,
	int xorg, int yorg, int celxs, int celys,
	char *str, int clr)
{
	char *cs;
	int cx, cy, ci;
	
	cs=str; cx=xorg; cy=yorg;
	while(*cs)
	{
		ci=gfxedit_readutf(&cs);
		GfxEdit_SdfDrawGlyph(imgbuf, xs, ys, cx, cy, celxs, celys, ci, clr);
		cx+=celxs;
	}
}
