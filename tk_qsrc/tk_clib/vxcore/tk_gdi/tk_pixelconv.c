void TKGDI_CopyImageBuffer_Cnv24to32(
	byte *dst, byte *src, int ixs, int iys)
{
	byte *cs, *ct;
	u32 v;
	int xs, ys, flip, xstr3, xstr4;
	int x, y;
	
	if(!dst || !src)
		return;
	
	xs=ixs;
	ys=iys;
	flip=0;
	if(iys<0)
		{ ys=-iys; flip=1; }
	
	xstr3=xs*3;
	xstr4=xs*4;
	xstr3=(xstr3+3)&(~3);

	for(y=0; y<ys; y++)
	{
		cs=src+y*xstr3;
		ct=dst+y*xstr4;
		if(flip)
			cs=src+(ys-y-1)*xstr3;
		
		for(x=0; x<xs; x++)
		{
			v=*(u32 *)cs;
			v=v|0xFF000000U;
			*(u32 *)ct=v;
			cs+=3; ct+=4;
		}
	}
}

void TKGDI_CopyImageBuffer_Cnv32to24(
	byte *dst, byte *src, int ixs, int iys)
{
	byte *cs, *ct;
	u32 v;
	int xs, ys, flip, xstr3, xstr4;
	int x, y;
	
	if(!dst || !src)
		return;
	
	xs=ixs;
	ys=iys;
	flip=0;
	if(iys<0)
		{ ys=-iys; flip=1; }
	
	xstr3=xs*3;
	xstr4=xs*4;
	xstr3=(xstr3+3)&(~3);

	for(y=0; y<ys; y++)
	{
		cs=src+y*xstr4;
		ct=dst+y*xstr3;
		if(flip)
			cs=src+(ys-y-1)*xstr4;
		
		for(x=0; x<xs; x++)
		{
			v=*(u32 *)cs;
			*(u16 *)ct=v;
			ct[2]=v>>16;
			cs+=4; ct+=3;
		}
	}
}

void TKGDI_CopyImageBuffer_Cnv32to32(
	byte *dst, byte *src, int ixs, int iys)
{
	byte *cs, *ct;
	u32 v;
	int xs, ys, flip, xstr3, xstr4;
	int x, y;
	
	if(!dst || !src)
		return;
	
	xs=ixs;
	ys=iys;
	flip=0;
	if(iys<0)
		{ ys=-iys; flip=1; }
	
	if(!flip)
	{
		memcpy(dst, src, xs*ys*4);
		return;
	}

	xstr4=xs*4;
	for(y=0; y<ys; y++)
	{
		ct=dst+y*xstr4;
		cs=src+(ys-y-1)*xstr4;
		memcpy(ct, cs, xstr4);
	}
}


void TKGDI_CopyImageBuffer_Cnv24to24(
	byte *dst, byte *src, int ixs, int iys)
{
	byte *cs, *ct;
	u32 v;
	int xs, ys, flip, xstr3, xstr4;
	int x, y;
	
	if(!dst || !src)
		return;
	
	xs=ixs;
	ys=iys;
	flip=0;
	if(iys<0)
		{ ys=-iys; flip=1; }
	
	if(!flip)
	{
		memcpy(dst, src, xs*ys*3);
		return;
	}
	
	xstr3=xs*3;
	xstr3=(xstr3+3)&(~3);

	for(y=0; y<ys; y++)
	{
		ct=dst+y*xstr3;
		cs=src+(ys-y-1)*xstr3;
		memcpy(ct, cs, xstr3);
	}
}

void TKGDI_CopyImageBuffer_Cnv15to15(
	byte *dst, byte *src, int ixs, int iys)
{
	byte *cs, *ct;
	u32 v;
	int xs, ys, flip, xstr2;
	int x, y;
	
	if(!dst || !src)
		return;
	
	xs=ixs;
	ys=iys;
	flip=0;
	if(iys<0)
		{ ys=-iys; flip=1; }
	
	if(!flip)
	{
		memcpy(dst, src, xs*ys*2);
		return;
	}
	
	xstr2=xs*2;
	xstr2=(xstr2+3)&(~3);

	for(y=0; y<ys; y++)
	{
		ct=dst+y*xstr2;
		cs=src+(ys-y-1)*xstr2;
		memcpy(ct, cs, xstr2);
	}
}

void TKGDI_CopyImageBuffer_Cnv32to15(
	byte *dst, byte *src, int ixs, int iys)
{
	byte *cs, *ct, *cse;
	u32 v, v0, v1, v2, v3;
	int cr, cg, cb;
	int cr1, cg1, cb1;
	int xs, ys, flip, xstr2, xstr4;
	int x, y, xsq4;
	
	if(!dst || !src)
		return;
	
	xs=ixs;
	ys=iys;
	flip=0;
	if(iys<0)
		{ ys=-iys; flip=1; }
	
	xsq4=xs>>2;
	
	xstr2=xs*2;
	xstr4=xs*4;
	xstr2=(xstr2+3)&(~3);

	for(y=0; y<ys; y++)
	{
		cs=src+y*xstr4;
		ct=dst+y*xstr2;
		if(flip)
			cs=src+(ys-y-1)*xstr4;

		for(x=0; x<xsq4; x++)
		{
			v0=((u32 *)cs)[0];	v1=((u32 *)cs)[1];
			v2=((u32 *)cs)[2];	v3=((u32 *)cs)[3];
			cr=(v0>>19)&31;		cr1=(v1>>19)&31;
			cg=(v0>>11)&31;		cg1=(v1>>11)&31;
			cb=(v0>> 3)&31;		cb1=(v1>> 3)&31;
			v0=(cr<<10)|(cg<<5)|cb;
			v1=(cr1<<10)|(cg1<<5)|cb1;
			cr=(v2>>19)&31;		cr1=(v3>>19)&31;
			cg=(v2>>11)&31;		cg1=(v3>>11)&31;
			cb=(v2>> 3)&31;		cb1=(v3>> 3)&31;
			v2=(cr<<10)|(cg<<5)|cb;
			v3=(cr1<<10)|(cg1<<5)|cb1;
			((u16 *)ct)[0]=v0;	((u16 *)ct)[1]=v1;
			((u16 *)ct)[2]=v2;	((u16 *)ct)[3]=v3;
			cs+=16; ct+=8;
		}
		
		if(xs&3)
		{
			for(x=(xsq4<<2); x<xs; x++)
			{
				v=*(u32 *)cs;
				cr=(v>>19)&31;
				cg=(v>>11)&31;
				cb=(v>> 3)&31;
				v1=(cr<<10)|(cg<<5)|cb;
				*(u16 *)ct=v1;
				cs+=4; ct+=2;
			}
		}
	}
}

void TKGDI_CopyImageBuffer_Cnv24to15(
	byte *dst, byte *src, int ixs, int iys)
{
	byte *cs, *ct;
	u32 v, v1;
	int cr, cg, cb;
	int xs, ys, flip, xstr2, xstr3;
	int x, y;
	
	if(!dst || !src)
		return;
	
	xs=ixs;
	ys=iys;
	flip=0;
	if(iys<0)
		{ ys=-iys; flip=1; }
	
	xstr2=xs*2;
	xstr3=xs*3;
	xstr2=(xstr2+3)&(~3);
	xstr3=(xstr3+3)&(~3);

	for(y=0; y<ys; y++)
	{
		cs=src+y*xstr3;
		ct=dst+y*xstr2;
		if(flip)
			cs=src+(ys-y-1)*xstr3;
		for(x=0; x<xs; x++)
		{
			v=*(u32 *)cs;
			cr=(v>>19)&31;
			cg=(v>>11)&31;
			cb=(v>> 3)&31;
			v1=(cr<<10)|(cg<<5)|cb;
			*(u16 *)ct=v1;
			cs+=3; ct+=2;
		}
	}
}

void TKGDI_CopyImageBuffer_Cnv15to32(
	byte *dst, byte *src, int ixs, int iys)
{
	byte *cs, *ct;
	u32 v;
	int xs, ys, flip, xstr2, xstr4;
	int x, y, cr, cg, cb;
	
	if(!dst || !src)
		return;
	
	xs=ixs;
	ys=iys;
	flip=0;
	if(iys<0)
		{ ys=-iys; flip=1; }
	
	xstr2=xs*2;
	xstr4=xs*4;
	xstr2=(xstr2+3)&(~3);

	for(y=0; y<ys; y++)
	{
		cs=src+y*xstr2;
		ct=dst+y*xstr4;
		if(flip)
			cs=src+(ys-y-1)*xstr2;
		
		for(x=0; x<xs; x++)
		{
			v=*(u16 *)cs;
			cr=(v>>10)&31;
			cg=(v>> 5)&31;
			cb=(v>> 0)&31;
			cr=(cr<<3)|(cr>>2);
			cg=(cg<<3)|(cg>>2);
			cb=(cb<<3)|(cb>>2);
			v=(cr<<16)|(cg<<8)|cb|0xFF000000U;
			*(u32 *)ct=v;
			cs+=2; ct+=4;
		}
	}
}

void TKGDI_CopyImageBuffer_Cnv15to24(
	byte *dst, byte *src, int ixs, int iys)
{
	byte *cs, *ct;
	u32 v;
	int xs, ys, flip, xstr2, xstr3;
	int x, y, cr, cg, cb;
	
	if(!dst || !src)
		return;
	
	xs=ixs;
	ys=iys;
	flip=0;
	if(iys<0)
		{ ys=-iys; flip=1; }
	
	xstr2=xs*2;
	xstr3=xs*3;
	xstr2=(xstr2+3)&(~3);
	xstr3=(xstr3+3)&(~3);

	for(y=0; y<ys; y++)
	{
		cs=src+y*xstr2;
		ct=dst+y*xstr3;
		if(flip)
			cs=src+(ys-y-1)*xstr2;
		
		for(x=0; x<xs; x++)
		{
			v=*(u16 *)cs;
			cr=(v>>10)&31;
			cg=(v>> 5)&31;
			cb=(v>> 0)&31;
			cr=(cr<<3)|(cr>>2);
			cg=(cg<<3)|(cg>>2);
			cb=(cb<<3)|(cb>>2);
			v=(cr<<16)|(cg<<8)|cb;
			*(u16 *)ct=v;
			ct[2]=v>>16;
			cs+=2; ct+=3;
		}
	}
}


int TKGDI_CopyImageBuffer_Conv(
	byte *dst, byte *src, int ixs, int iys, int dbpp, int sbpp)
{
	if(dbpp==32)
	{
		if(sbpp==32)
		{
			TKGDI_CopyImageBuffer_Cnv32to32(dst, src, ixs, iys);
			return(0);
		}
		if(sbpp==24)
		{
			TKGDI_CopyImageBuffer_Cnv24to32(dst, src, ixs, iys);
			return(0);
		}
		if((sbpp==16) || (sbpp==15))
		{
			TKGDI_CopyImageBuffer_Cnv15to32(dst, src, ixs, iys);
			return(0);
		}
	}

	if(dbpp==24)
	{
		if(sbpp==32)
		{
			TKGDI_CopyImageBuffer_Cnv32to24(dst, src, ixs, iys);
			return(0);
		}
		if(sbpp==24)
		{
			TKGDI_CopyImageBuffer_Cnv24to24(dst, src, ixs, iys);
			return(0);
		}
		if((sbpp==16) || (sbpp==15))
		{
			TKGDI_CopyImageBuffer_Cnv15to24(dst, src, ixs, iys);
			return(0);
		}
	}

	if((dbpp==16) || (dbpp==15))
	{
		if(sbpp==32)
		{
			TKGDI_CopyImageBuffer_Cnv32to15(dst, src, ixs, iys);
			return(0);
		}
		if(sbpp==24)
		{
			TKGDI_CopyImageBuffer_Cnv24to15(dst, src, ixs, iys);
			return(0);
		}
		if((sbpp==16) || (sbpp==15))
		{
			TKGDI_CopyImageBuffer_Cnv15to15(dst, src, ixs, iys);
			return(0);
		}
	}

	return(-1);
}

void TKGDI_CopyImageBuffer_SetupPalArray555(
	u16 *pal1, byte *pal, int nclr)
{
	int cr, cg, cb, ca, v;
	int i;

	for(i=0; i<nclr; i++)
	{
		cb=pal[i*4+0];
		cg=pal[i*4+1];
		cr=pal[i*4+2];
		ca=pal[i*4+3];
		cb>>=3;	cg>>=3;	cr>>=3;
		v=(cr<<10)|(cg<<5)|cb;
		if(ca<240)
		{
			v&=0x7BDE;
			v|=0x8000;
			if(ca&0x80)		v|=0x0200;
			if(ca&0x40)		v|=0x0040;
			if(ca&0x20)		v|=0x0001;
		}
		pal1[i]=v;
	}
}

void TKGDI_CopyImageBuffer_SetupPalArrayRGBA32(
	u32 *pal1, byte *pal, int nclr)
{
	int cr, cg, cb, ca, v;
	int i;

	for(i=0; i<nclr; i++)
	{
		cb=pal[i*4+0];
		cg=pal[i*4+1];
		cr=pal[i*4+2];
		ca=pal[i*4+3];
		v=(ca<<24)|(cr<<16)|(cg<<8)|cb;
		pal1[i]=v;
	}
}

void TKGDI_CopyImageBuffer_CnvI8to32(
	byte *dst, byte *src, int ixs, int iys, byte *pal)
{
	u32 pal1[256];
	byte *cs, *ct;
	u32 v, v1;
	int xs, ys, flip, xstr1, xstr4;
	int x, y;
	
	if(!dst || !src)
		return;
	
	xs=ixs;
	ys=iys;
	flip=0;
	if(iys<0)
		{ ys=-iys; flip=1; }

	TKGDI_CopyImageBuffer_SetupPalArrayRGBA32(pal1, pal, 256);
	
	xstr4=xs*4;
	xstr1=xs;
	xstr4=(xstr4+3)&(~3);
	xstr1=(xstr1+3)&(~3);

	for(y=0; y<ys; y++)
	{
		cs=src+y*xstr1;
		ct=dst+y*xstr4;
		if(flip)
			cs=src+(ys-y-1)*xstr1;
		for(x=0; x<xs; x++)
		{
			v=*cs;
			v1=pal1[v];
			*(u32 *)ct=v1;
			cs++; ct+=4;
		}
	}
}

void TKGDI_CopyImageBuffer_CnvI8to24(
	byte *dst, byte *src, int ixs, int iys, byte *pal)
{
	u32 pal1[256];
	byte *cs, *ct;
	u32 v, v1;
	int xs, ys, flip, xstr1, xstr3;
	int x, y;
	
	if(!dst || !src)
		return;
	
	xs=ixs;
	ys=iys;
	flip=0;
	if(iys<0)
		{ ys=-iys; flip=1; }

	TKGDI_CopyImageBuffer_SetupPalArrayRGBA32(pal1, pal, 256);

	xstr3=xs*3;
	xstr1=xs;
	xstr3=(xstr3+3)&(~3);
	xstr1=(xstr1+3)&(~3);

	for(y=0; y<ys; y++)
	{
		cs=src+y*xstr1;
		ct=dst+y*xstr3;
		if(flip)
			cs=src+(ys-y-1)*xstr1;

		for(x=0; x<xs; x++)
		{
			v=*cs;
			v1=pal1[v];
			*(u16 *)ct=v;
			ct[2]=v>>16;
			cs++; ct+=3;
		}
	}
}

void TKGDI_CopyImageBuffer_CnvI8to15(
	byte *dst, byte *src, int ixs, int iys, byte *pal)
{
	u16 pal1[256];
	byte *cs, *ct;
	u32 v, v1;
	int xs, ys, flip, xstr2, xstr1;
	int x, y;
	
	if(!dst || !src)
		return;
	
	xs=ixs;
	ys=iys;
	flip=0;
	if(iys<0)
		{ ys=-iys; flip=1; }
	
	TKGDI_CopyImageBuffer_SetupPalArray555(pal1, pal, 256);
		
	xstr2=xs*2;
	xstr1=xs;
	xstr2=(xstr2+3)&(~3);
	xstr1=(xstr1+3)&(~3);

	for(y=0; y<ys; y++)
	{
		cs=src+y*xstr1;
		ct=dst+y*xstr2;
		if(flip)
			cs=src+(ys-y-1)*xstr1;
		for(x=0; x<xs; x++)
		{
			v=*cs;
			v1=pal1[v];
			*(u16 *)ct=v1;
			cs++; ct+=2;
		}
	}
}

void TKGDI_CopyImageBuffer_CnvI4to15(
	byte *dst, byte *src, int ixs, int iys, byte *pal)
{
	u16 pal1[16];
	byte *cs, *ct;
	u32 v, v1;
	int xs, ys, flip, xstr2, xstr3, xstr1p2;
	int x, y;
	
	if(!dst || !src)
		return;
	
	xs=ixs;
	ys=iys;
	flip=0;
	if(iys<0)
		{ ys=-iys; flip=1; }
	
	TKGDI_CopyImageBuffer_SetupPalArray555(pal1, pal, 16);
	
	xstr2=xs*2;
	xstr1p2=xs>>1;
	xstr2=(xstr2+3)&(~3);
	xstr1p2=(xstr1p2+3)&(~3);

	for(y=0; y<ys; y++)
	{
		cs=src+y*xstr1p2;
		ct=dst+y*xstr2;
		if(flip)
			cs=src+(ys-y-1)*xstr1p2;
		for(x=0; x<xs; x++)
		{
			v=cs[x>>1];
			v=v<<((x&1)<<2);
			v1=pal1[(v>>4)&15];
			*(u16 *)ct=v1;
			ct+=2;
		}
	}
}

void TKGDI_CopyImageBuffer_CnvI2to15(
	byte *dst, byte *src, int ixs, int iys, byte *pal)
{
	u16 pal1[16];
	byte *cs, *ct;
	u32 v, v1;
	int xs, ys, flip, xstr2, xstr3, xstr1p2;
	int x, y;
	
	if(!dst || !src)
		return;
	
	xs=ixs;
	ys=iys;
	flip=0;
	if(iys<0)
		{ ys=-iys; flip=1; }
	
	TKGDI_CopyImageBuffer_SetupPalArray555(pal1, pal, 4);
	
	xstr2=xs*2;
	xstr1p2=xs>>2;
	xstr2=(xstr2+3)&(~3);
	xstr1p2=(xstr1p2+3)&(~3);

	for(y=0; y<ys; y++)
	{
		cs=src+y*xstr1p2;
		ct=dst+y*xstr2;
		if(flip)
			cs=src+(ys-y-1)*xstr1p2;
		for(x=0; x<xs; x++)
		{
			v=cs[x>>2];
			v=v<<((x&3)<<1);
			v1=pal1[(v>>6)&3];
			*(u16 *)ct=v1;
			ct+=2;
		}
	}
}

void TKGDI_CopyImageBuffer_CnvI1to15(
	byte *dst, byte *src, int ixs, int iys, byte *pal)
{
	u16 pal1[16];
	byte *cs, *ct;
	u32 v, v1;
	int xs, ys, flip, xstr2, xstr3, xstr1p2;
	int x, y;
	
	if(!dst || !src)
		return;
	
	xs=ixs;
	ys=iys;
	flip=0;
	if(iys<0)
		{ ys=-iys; flip=1; }
	
	TKGDI_CopyImageBuffer_SetupPalArray555(pal1, pal, 2);
	
	xstr2=xs*2;
	xstr1p2=xs>>3;
	xstr2=(xstr2+3)&(~3);
	xstr1p2=(xstr1p2+3)&(~3);
	
	for(y=0; y<ys; y++)
	{
		cs=src+y*xstr1p2;
		ct=dst+y*xstr2;
		if(flip)
			cs=src+(ys-y-1)*xstr1p2;
		for(x=0; x<xs; x++)
		{
			v=cs[x>>3];
			v=v<<(x&7);
			v1=pal1[(v>>7)&1];
			*(u16 *)ct=v1;
			ct+=2;
		}
	}
}

int TKGDI_CopyImageBuffer_ConvPal(
	byte *dst, byte *src, int ixs, int iys, int dbpp, int sbpp,
	byte *dpal, byte *spal)
{
	byte *tbuf;
	int iys1;

	if((dbpp>8) && (sbpp>8))
	{
		return(TKGDI_CopyImageBuffer_Conv(
			dst, src, ixs, iys, dbpp, sbpp));
	}

	if((dbpp>8) && (sbpp==8))
	{
		if(dbpp==32)
		{
			TKGDI_CopyImageBuffer_CnvI8to32(dst, src, ixs, iys, spal);
			return(0);
		}

		if(dbpp==24)
		{
			TKGDI_CopyImageBuffer_CnvI8to24(dst, src, ixs, iys, spal);
			return(0);
		}

		if((dbpp==16) || (dbpp==15))
		{
			TKGDI_CopyImageBuffer_CnvI8to15(dst, src, ixs, iys, spal);
			return(0);
		}
	}

	if((dbpp>8) && (sbpp==4))
	{
		if(dbpp==32)
		{
			tbuf=tk_malloc(ixs*iys*2);
			iys1=abs(iys);
			TKGDI_CopyImageBuffer_CnvI4to15(tbuf, src, ixs, iys, spal);
			TKGDI_CopyImageBuffer_Cnv15to32(dst, tbuf, ixs, iys1);
			tk_free(tbuf);
			return(0);
		}

		if(dbpp==24)
		{
			tbuf=tk_malloc(ixs*iys*2);
			iys1=abs(iys);
			TKGDI_CopyImageBuffer_CnvI4to15(tbuf, src, ixs, iys, spal);
			TKGDI_CopyImageBuffer_Cnv15to24(dst, tbuf, ixs, iys1);
			tk_free(tbuf);
			return(0);
		}

		if((dbpp==16) || (dbpp==15))
		{
			TKGDI_CopyImageBuffer_CnvI4to15(dst, src, ixs, iys, spal);
			return(0);
		}
	}

	if((dbpp>8) && (sbpp==2))
	{
		if(dbpp==32)
		{
			tbuf=tk_malloc(ixs*iys*2);
			iys1=abs(iys);
			TKGDI_CopyImageBuffer_CnvI2to15(tbuf, src, ixs, iys, spal);
			TKGDI_CopyImageBuffer_Cnv15to32(dst, tbuf, ixs, iys1);
			tk_free(tbuf);
			return(0);
		}

		if(dbpp==24)
		{
			tbuf=tk_malloc(ixs*iys*2);
			iys1=abs(iys);
			TKGDI_CopyImageBuffer_CnvI2to15(tbuf, src, ixs, iys, spal);
			TKGDI_CopyImageBuffer_Cnv15to24(dst, tbuf, ixs, iys1);
			tk_free(tbuf);
			return(0);
		}

		if((dbpp==16) || (dbpp==15))
		{
			TKGDI_CopyImageBuffer_CnvI2to15(dst, src, ixs, iys, spal);
			return(0);
		}
	}

	if((dbpp>8) && (sbpp==1))
	{
		if(dbpp==32)
		{
			tbuf=tk_malloc(ixs*iys*2);
			iys1=abs(iys);
			TKGDI_CopyImageBuffer_CnvI1to15(tbuf, src, ixs, iys, spal);
			TKGDI_CopyImageBuffer_Cnv15to32(dst, tbuf, ixs, iys1);
			tk_free(tbuf);
			return(0);
		}

		if(dbpp==24)
		{
			tbuf=tk_malloc(ixs*iys*2);
			iys1=abs(iys);
			TKGDI_CopyImageBuffer_CnvI1to15(tbuf, src, ixs, iys, spal);
			TKGDI_CopyImageBuffer_Cnv15to24(dst, tbuf, ixs, iys1);
			tk_free(tbuf);
			return(0);
		}

		if((dbpp==16) || (dbpp==15))
		{
			TKGDI_CopyImageBuffer_CnvI1to15(dst, src, ixs, iys, spal);
			return(0);
		}
	}

	return(-1);
}
