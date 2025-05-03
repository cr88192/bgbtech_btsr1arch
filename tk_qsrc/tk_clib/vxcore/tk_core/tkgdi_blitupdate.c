#include <tk_core.h>
#include <tkgdi/tkgdi.h>

int tkgdi_vid_frnum;
int tkgdi_vid_scrmode;
int tkgdi_vid_cellstride;
int tkgdi_vid_rowstride;
byte tkgdi_vid_planar;
byte tkgdi_vid_noutx2;
byte tkgdi_vid_is8bit;
byte tkgdi_vid_is1bit;
byte tkgdi_vid_is2bit_cc;

int tkgdi_vid_xsize;
int tkgdi_vid_ysize;

// int tkgdi_vid_bmxsize;

// u64 *tkgdi_vid_screenutx;		//screen UTX2 buffer
// u64 *tkgdi_vid_screenrgb;		//screen RGB buffer
// byte *tkgdi_vid_screendirty;		//screen dirtry-mask buffer

// static u64 tkgdi_ptrmangle_key=0;

byte tk_img_d9to8tab[512];
// byte tk_img_d15to8tab0[32768];
// byte tk_img_d15to8tab1[32768];
// u16 tk_img_d8to15tab[256];

byte *tk_img_d15to8tab0;
byte *tk_img_d15to8tab1;

u16 tk_img_d8to15tab[288];
byte tk_img_d8to15ready;

byte tk_img_d11to8tab0[2048];
byte tk_img_d11to8tab1[2048];

byte tk_img_d10to8tab_sh[2048];
byte tk_img_d13to8tab_sh[8192];

int tkgdi_blitupdate_getconbuf_sticky;
int tkgdi_blitupdate_getconbuf_sticky_cnt;

u16 TKGDI_BlitUpdate_Repack32to16H(u32 pix);
int tk_img_rgb15dist(u16 va, u16 vb);
void TKGDI_BlitUpdate_EncodeCell8x8x2(u16 *ics, u32 *ict, int sbxs);

#ifndef __BGBCC__
u32 __int32_bswap(u32 vx)
{
	return(
		((vx>>24)&0x000000FF) |
		((vx>> 8)&0x0000FF00) |
		((vx<< 8)&0x00FF0000) |
		((vx<<24)&0xFF000000) );
}
#endif

int tk_img_genpal1()
{
	int cr, cg, cb;
	int i, j, k;

	printf("pal1\n");

	for(i=0x00; i<=0xFF; i++)
	{
		j=((i&15)<<1);
//		k=(i&0x80)?(j>>1):0;
//		k=(i&0x80)?((j>>1)+(j>>2)):0;
//		k=(i&0x80)?((j>>1)+(j>>3)):0;
//		k=(i&0x80)?((j>>1)+(j>>3)):((j>>2)+(j>>3));
//		k=(i&0x80)?((j>>1)+(j>>3)):(j>>3);
		k=(i&0x80)?((j>>1)+(j>>3)):(j>>2);
//		k=(i&0x80)?((j>>2)+(j>>3)):0;
		cr=(i&0x40)?j:k;
		cg=(i&0x20)?j:k;
		cb=(i&0x10)?j:k;
		k=(cr<<10)|(cg<<5)|cb;
		tk_img_d8to15tab[i]=k;
	}

	for(i=0x00; i<=0x0F; i++)
	{
		j=((i&15)<<1);
		k=(j>>1)+(j>>2);
		cr=j; cg=j; cb=j;
		k=(cr<<10)|(cg<<5)|cb;
		tk_img_d8to15tab[i]=k;
	}

	for(i=0xF0; i<=0xFF; i++)
	{
		j=((i&15)<<1);
		k=(j>>1)+(j>>2);
		cr=k; cg=j; cb=j;
		k=(cr<<10)|(cg<<5)|cb;
		tk_img_d8to15tab[i]=k;
	}

	for(i=0x80; i<=0x8F; i++)
	{
		j=((i&15)<<1);
		k=(j>>1)+(j>>2);
		cr=j; cg=j; cb=k;
		k=(cr<<10)|(cg<<5)|cb;
		tk_img_d8to15tab[i]=k;
	}

	for(i=0x70; i<=0x7F; i++)
	{
		j=((i&15)<<1);
		k=(j>>1)+(j>>2);
		cr=j; cg=k; cb=j;
		k=(cr<<10)|(cg<<5)|cb;
		tk_img_d8to15tab[i]=k;
	}
}

int tk_img_genpal2()
{
	int cr, cg, cb;
	int i0, i1, i2;
	int i, j, k;

	printf("pal2\n");

	for(i=0x00; i<=0xFF; i++)
	{
		j=((i&15)<<1);
		k=(i&0x80)?((j>>1)+(j>>3)):(j>>2);
		cr=(i&0x40)?j:k;
		cg=(i&0x20)?j:k;
		cb=(i&0x10)?j:k;
		k=(cr<<10)|(cg<<5)|cb;
		tk_img_d8to15tab[i]=k;
	}

	for(i=0x00; i<=0x0F; i++)
	{
		j=((i&15)<<1);
		k=(j>>1)+(j>>2);
		cr=j; cg=j; cb=j;
		k=(cr<<10)|(cg<<5)|cb;
		tk_img_d8to15tab[i]=k;
	}

	for(i=0xF0; i<=0xFF; i++)
	{
		j=((i&15)<<1);
		k=(j>>1)+(j>>2);
		cr=k; cg=j; cb=j;
		k=(cr<<10)|(cg<<5)|cb;
		tk_img_d8to15tab[i]=k;
	}

	for(i=0x80; i<=0x8F; i++)
	{
		j=((i&15)<<1);
		k=(j>>1)+(j>>2);
		cr=j; cg=j; cb=k;
		k=(cr<<10)|(cg<<5)|cb;
		tk_img_d8to15tab[i]=k;
	}

	for(i=0x70; i<=0x7F; i++)
	{
		j=((i&15)<<1);
		k=(j>>1)+(j>>2);
		cr=j; cg=k; cb=j;
		k=(cr<<10)|(cg<<5)|cb;
		tk_img_d8to15tab[i]=k;
	}

	for(i=0x11; i<=0xF1; i+=16)
	{
		i0=((i>>4)&15)*2;
//		i1=((j>>1)+(j>>3));
//		i2=(j>>2);
		i1=(i0*5)>>3;
		i2=(i0*3)>>3;

		cr=i0;
		cg=i1;
		cb=i2;

		k=(cr<<10)|(cg<<5)|cb;
		tk_img_d8to15tab[i]=k;
	}

	for(i=0x12; i<=0xF2; i+=16)
	{
		i0=((i>>4)&15)*2;
//		i1=((j>>1)+(j>>3));
//		i2=(j>>2);
		i1=(i0*5)>>3;
		i2=(i0*3)>>3;
		cr=i2;
		cg=i1;
		cb=i0;

		k=(cr<<10)|(cg<<5)|cb;
		tk_img_d8to15tab[i]=k;
	}

	for(i=0x10; i<=0xF0; i+=16)
	{
		if(i&0x80)
			{ j=0x1F; k=0x0A; }
		else
			{ j=0x15; k=0x00; }
		cr=(i&0x40)?j:k;
		cg=(i&0x20)?j:k;
		cb=(i&0x10)?j:k;

		k=(cr<<10)|(cg<<5)|cb;
		tk_img_d8to15tab[i]=k;
	}
}

int tk_img_rgb15dist(u16 va, u16 vb)
{
	int cr0, cg0, cb0, cy0;
	int cr1, cg1, cb1, cy1;
	int dr, dg, db, dy;
	int d;
	
	cr0=(va>>10)&31;	cr1=(vb>>10)&31;
	cg0=(va>> 5)&31;	cg1=(vb>> 5)&31;
	cb0=(va>> 0)&31;	cb1=(vb>> 0)&31;
	
	cy0=(8*cg0+5*cr0+3*cb0)>>4;
	cy1=(8*cg1+5*cr1+3*cb1)>>4;
	
	dr=cr0-cr1;
	dg=cg0-cg1;
	db=cb0-cb1;
	dy=cy0-cy1;

#if 0
	dr=dr^(dr>>31);
	dg=dg^(dg>>31);
	db=db^(db>>31);
	dy=dy^(dy>>31);
//	d=dr+2*dg+db;
	d=dr+dg+db+2*dy;
#endif

	dr=dr*dr;
	dg=dg*dg;
	db=db*db;
	dy=dy*dy;
	d=dr+dg+db+dy+dy;

//	d=dr*dr+2*dg*dg+db*db;
//	d=dr*dr+dg*dg+db*db+dy*dy;
	return(d);
}

int tk_img_rgb9to15a(int va)
{
	int cr, cg, cb;
	int v;
	cr=(va>>6)&7;
	cg=(va>>3)&7;
	cb=(va>>0)&7;
	cr=(cr<<2)|(cr>>1);
	cg=(cg<<2)|(cg>>1);
	cb=(cb<<2)|(cb>>1);
	v=(cr<<10)|(cg<<5)|cb;
	return(v);
}

int tk_img_rgb15to9a(int va)
{
	int cr, cg, cb;
	int v;
	
	cr=(va>>10)&31;
	cg=(va>> 5)&31;
	cb=(va>> 0)&31;
	cr>>=2;
	cg>>=2;
	cb>>=2;
	v=(cr<<6)|(cg<<3)|cb;
	return(v);
}

int tk_img_gen9to8()
{
	int bi, bd, d;
	int i, j, k;
	
	for(i=0; i<512; i++)
	{
		bi=0; bd=999999;
		k=tk_img_rgb9to15a(i);
		for(j=0; j<256; j++)
		{
			d=tk_img_rgb15dist(k, tk_img_d8to15tab[j]);
			if(d<bd)
				{ bi=j; bd=d; }
		}
		tk_img_d9to8tab[i]=bi;
	}
}

#if 0
int tk_img_rgb15to8norm(int va)
{
	int cr, cg, cb, cy, cm;
	int v, d, bi, bd, bi2, bd2, d2;
	int k0, k1, j0, j1;
	int i, j, k;
	
	cr=(va>>10)&31;
	cg=(va>> 5)&31;
	cb=(va>> 0)&31;

	cy=cg;
	cm=cg;
	if(cr>cy)
		cy=cr;
	if(cb>cy)
		cy=cb;

	if(cr<cm)
		cm=cr;
	if(cb<cm)
		cm=cb;
	
	if(!cy)
		return(0);
	
	if(cm<((cy*3)>>3))
	{
		k0=0; k1=0;

		j=(cy*4)>>3;
		if(cr>j)
			k0|=4;
		if(cg>j)
			k0|=2;
		if(cb>j)
			k0|=1;

		j=(cy*6)>>3;
		if(cr>j)
			k1|=4;
		if(cg>j)
			k1|=2;
		if(cb>j)
			k1|=1;

		if((k0==0) || (k0==7))
			k0=0;
		if((k1==0) || (k1==7))
			k1=0;

		j0=cy>>1;
		j1=(cy+1)>>1;
		if(j1>15)
			j1=15;
		if(k0!=k1)
			j1=j0;
		bi=(k0<<4)|j0;
		bi2=(k1<<4)|j1;
	}else if(cm>=((cy*15)>>4))
	{
		j0=cy>>1;
		j1=(cy+1)>>1;
		if(j1>15)
			j1=15;
		bi=0x00|j0;
		bi2=0x00|j1;
	}else
		if(cm>=((cy*6)>>3))
	{
		if(cm==cr)
			k=0xF0;
		if(cm==cg)
			k=0x70;
		if(cm==cb)
			k=0x80;

		j0=cy>>1;
		j1=(cy+1)>>1;
		if(j1>15)
			j1=15;

		bi=k|j0;
		bi2=k|j1;
	}else
	{
		k0=8; k1=8;

		j=(cy*11)>>4;
		if(cr>j)
			k0|=4;
		if(cg>j)
			k0|=2;
		if(cb>j)
			k0|=1;

		j=(cy*13)>>4;
		if(cr>j)
			k1|=4;
		if(cg>j)
			k1|=2;
		if(cb>j)
			k1|=1;

		if((k0==8) || (k0==15))
			k0=0;
		if((k1==8) || (k1==15))
			k1=0;

		j0=cy>>1;
		j1=(cy+1)>>1;
		if(j1>15)
			j1=15;

		if(k0!=k1)
			j1=j0;
		bi=(k0<<4)|j0;
		bi2=(k1<<4)|j1;
	}
	
	return(bi|(bi2<<8));
}
#endif

#if 0
int tk_img_rgb15to8norm(int va)
{
	int cr, cg, cb, cy;
	int v, d, bi, bd, bi2, bd2, d2;
	int i, j, k;
	
	cr=(va>>10)&31;
	cg=(va>> 5)&31;
	cb=(va>> 0)&31;

	cy=cg;
	if(cr>cy)
		cy=cr;
	if(cb>cy)
		cy=cb;
	
	if(!cy)
		return(0);
	
	bi=0; bd=999999;
	bi2=0; bd2=999999;
	for(i=0; i<16; i++)
	{
		k=(cy+1)>>1;
		if(k>15)k=15;
//		j=(i<<4)|(cy>>1);
		j=(i<<4)|k;
		d=tk_img_rgb15dist(va, tk_img_d8to15tab[j]);

#if 1
		k=j-1;
		d2=tk_img_rgb15dist(va, tk_img_d8to15tab[k]);
		if(d2<d)
			{ j=k; d=d2; }

		k=j+1;
		d2=tk_img_rgb15dist(va, tk_img_d8to15tab[k]);
		if(d2<d)
			{ j=k; d=d2; }
#endif

		if(d<bd)
			{ bi2=bi; bd2=bd; bi=j; bd=d; }
		else if(d<bd2)
			{ bi2=j; bd2=d; }
	}

	if(((bd2*3)/8)>bd)
	{
		bi2=bi;

#if 1
		k=bi-1;
		d=tk_img_rgb15dist(va, tk_img_d8to15tab[k]);
		if(d<bd2)
			{ bi2=k; bd2=d; }

		k=bi+1;
		d=tk_img_rgb15dist(va, tk_img_d8to15tab[k]);
		if(d<bd2)
			{ bi2=k; bd2=d; }

		if(((bd2*3)/8)>bd)
		{
			bi2=bi;
		}
#endif

//		return(bi|(bi<<8));
	}
	return(bi|(bi2<<8));
}
#endif

#if 0
int tk_img_rgb15to8norm(int va)
{
	int cr, cg, cb, cy;
	int v, d, bi, bd, bi2, bd2, d2;
	int i, j, k;
	
	cr=(va>>10)&31;
	cg=(va>> 5)&31;
	cb=(va>> 0)&31;

	cy=cg;
	if(cr>cy)
		cy=cr;
	if(cb>cy)
		cy=cb;
	
	if(!cy)
		return(0);
	
	bi=0; bd=999999;
	bi2=0; bd2=999999;

	for(i=0; i<256; i++)
	{
		d=tk_img_rgb15dist(va, tk_img_d8to15tab[i]);

		if(d<bd)
			{ bi2=bi; bd2=bd; bi=i; bd=d; }
		else if(d<bd2)
			{ bi2=i; bd2=d; }
	}

	if(((bd2*3)/8)>bd)
		{ bi2=bi; }
	return(bi|(bi2<<8));
}
#endif

#if 1
short tk_img_d8to15tab_hash[64];
short tk_img_d8to15tab_nxt[256];

int tk_img_rgb15to8norm_dohash()
{
	int i, j, k, l, h;

	for(i=0; i<256; i++)
		{ tk_img_d8to15tab_nxt[i]=-1; }

	for(i=0; i<64; i++)
		{ tk_img_d8to15tab_hash[i]=-1; }

	for(i=0; i<256; i++)
	{
		j=tk_img_d8to15tab[i];
		k=(j&0x739C);
		k=k*251+k;
		k=k*251+k;
		h=(k>>8)&63;
		
		tk_img_d8to15tab_nxt[i]=tk_img_d8to15tab_hash[h];
		tk_img_d8to15tab_hash[h]=i;
	}
}

int tk_img_rgb15to8norm(int va)
{
	int cr, cg, cb, cy;
	int v, d, bi, bd, bi2, bd2, d2;
	int i, j, k, l, h;
	
	cr=(va>>10)&31;
	cg=(va>> 5)&31;
	cb=(va>> 0)&31;

	cy=cg;
	if(cr>cy)
		cy=cr;
	if(cb>cy)
		cy=cb;
	
	if(!cy)
		return(0);
	
	bi=0; bd=999999;
	bi2=0; bd2=999999;

	for(j=0; j<8; j++)
	{
		k=(va&0x739C);
		if(j&1)k+=0x0004;
		if(j&2)k+=0x0080;
		if(j&4)k+=0x1000;
		k=k*251+k;
		k=k*251+k;
		h=(k>>8)&63;
		
		i=tk_img_d8to15tab_hash[h];
		while(i>=0)
		{
			d=tk_img_rgb15dist(va, tk_img_d8to15tab[i]);
			if(d<bd)
				{ bi2=bi; bd2=bd; bi=i; bd=d; }
			else if(d<bd2)
				{ bi2=i; bd2=d; }
			i=tk_img_d8to15tab_nxt[i];
		}
	}

#if 0
	for(i=0; i<256; i++)
	{
		d=tk_img_rgb15dist(va, tk_img_d8to15tab[i]);

		if(d<bd)
			{ bi2=bi; bd2=bd; bi=i; bd=d; }
		else if(d<bd2)
			{ bi2=i; bd2=d; }
	}
#endif

	if(((bd2*3)/8)>bd)
		{ bi2=bi; }
	return(bi|(bi2<<8));
}
#endif

int tk_img_gen15to8_splat(
	byte *tab0, byte *tab1,
	u16 *dtab0, u16 *dtab1,
	u16 pclr, byte pidx)
{
	int crm, crn, cgm, cgn, cbm, cbn;
	int cr, cg, cb, px, d, d0, d1, i0;
	
	cr=(pclr>>10)&31;
	cg=(pclr>> 5)&31;
	cb=(pclr>> 0)&31;
	
	crm=cr-4;	crn=cr+4;
	cgm=cg-4;	cgn=cg+4;
	cbm=cb-4;	cbn=cb+4;
	
	if(crm<0)crm=0;
	if(cgm<0)cgm=0;
	if(cbm<0)cbm=0;
	if(crn>31)crm=31;
	if(cgn>31)cgm=31;
	if(cbn>31)cbm=31;
	
	for(cr=crm; cr<=crn; cr++)
		for(cg=cgm; cg<=cgn; cg++)
			for(cb=cbm; cb<=cbn; cb++)
	{
		px=(cr<<10)|(cg<<5)|cb;
		d=tk_img_rgb15dist(px, pclr);
		d0=dtab0[px];
		d1=dtab1[px];
		i0=tab0[px];
		if(d<d0)
		{
			dtab0[px]=d;
			tab0[px]=pidx;
			if(d0==0xFFFF)
			{
				dtab1[px]=d;
				tab1[px]=pidx;
			}else
			{
				dtab1[px]=d0;
				tab1[px]=i0;
			}
		}else if((d<d1) && (i0!=pidx))
		{
			dtab1[px]=d;
			tab1[px]=pidx;			
		}
	}
}

#if 1
int tk_img_gen15to8()
{
	u16 *dtab;
	byte *tab;
	int i, j, k;

	if(!tk_img_d15to8tab0)
	{
		tab=tk_malloc(65536);
		tk_img_d15to8tab0=tab+0;
		tk_img_d15to8tab1=tab+32768;
	}

#if 0
	dtab=tk_malloc(65536*2);
	memset(dtab, 0xFF, 65536*2);

	for(i=0; i<256; i++)
	{
		tk_img_gen15to8_splat(
			tk_img_d15to8tab0,
			tk_img_d15to8tab1,
			dtab+0, dtab+32768,
			tk_img_d8to15tab[i], i);
	}

#if 1
	for(i=0; i<32768; i++)
	{
		j=dtab[i];
		if(j!=0xFFFF)
			continue;
		j=tk_img_rgb15to8norm(i);
		tk_img_d15to8tab0[i]=(j   )&0xFF;
		tk_img_d15to8tab1[i]=(j>>8)&0xFF;
	}
#endif
	
	tk_free(dtab);
#endif

#if 1
	for(i=0; i<32768; i++)
	{
		j=tk_img_rgb15to8norm(i);
		tk_img_d15to8tab0[i]=(j   )&0xFF;
		tk_img_d15to8tab1[i]=(j>>8)&0xFF;
	}
#endif
}
#endif

#ifdef __BGBCC__
int tk_img_gen10to8()
{
	_BitInt(15) bi;
	int v, cr, cg, cb, cy;
	int i;

	for(i=0; i<1024; i++)
	{
		bi=i;
		cr=(_BitInt(5)) { bi[8], bi[5], bi[2], bi[0], bi[9] };
		cg=(_BitInt(5)) { bi[9], bi[6], bi[3], bi[0], bi[9] };
		cb=(_BitInt(5)) { bi[7], bi[4], bi[1], bi[0], bi[9] };
//		cy=(2*cg+cr+cb)>>2;
		cy=(6*cg+5*cr+5*cb)>>4;
//		if(cy<8)
		if(cy<4)
//			{ cr=cy;	cg=cy;	cb=cy; }
		{
			cr=(5*cr+3*cy)>>3;
			cg=(5*cg+3*cy)>>3;
			cb=(5*cb+3*cy)>>3;
		}
		
		v=(_BitInt(15)) { cr[4:0], cg[4:0], cb[4:0] };
	
		tk_img_d10to8tab_sh[     i]=tk_img_d15to8tab0[v];
		tk_img_d10to8tab_sh[1024+i]=tk_img_d15to8tab1[v];
	}

	for(i=0; i<8192; i++)
	{
		bi=i;
		cr=(_BitInt(5)) { bi[11], bi[8], bi[5], bi[2], bi[0] };
		cg=(_BitInt(5)) { bi[12], bi[9], bi[6], bi[3], bi[0] };
		cb=(_BitInt(5)) { bi[10], bi[7], bi[4], bi[1], bi[0] };

		cy=(6*cg+5*cr+5*cb)>>4;
		if(cy<4)
		{
			cr=(5*cr+3*cy)>>3;
			cg=(5*cg+3*cy)>>3;
			cb=(5*cb+3*cy)>>3;
		}

		v=(_BitInt(15)) { cr[4:0], cg[4:0], cb[4:0] };
		tk_img_d13to8tab_sh[i]=tk_img_d15to8tab0[v];
	}
	return(0);
}
#endif

#if 1
int tk_img_gen11to8()
{
	int cr, cg, cb;
	int i, j, k;

//	tk_img_rgb15to8norm_dohash();

	for(i=0; i<2048; i++)
	{
		cb=(i>>3)& 7;
		cg=(i>>6)&31;
		cr=(i   )& 7;

		if(cr==(cg>>2))
//		if(1)
		{
			cr=(cr<<2)|(cg&3);
		}else
		{
//			cr=(cr<<2)|2;
			cr=(cr<<2)|1;
//			cr=(cr<<2)|(cr>>1);
		}

		if(cb==(cg>>2))
//		if(1)
		{
			cb=(cb<<2)|(cg&3);
		}else
		{
//			cb=(cb<<2)|2;
			cb=(cb<<2)|1;
//			cb=(cb<<2)|(cb>>1);
		}

		k=(cr<<10)|(cg<<5)|cb;

		j=tk_img_rgb15to8norm(k);
		tk_img_d11to8tab0[i]=(j   )&0xFF;
		tk_img_d11to8tab1[i]=(j>>8)&0xFF;
	}
}
#endif

#if 0
int tk_img_gen15to8()
{
	static byte tab3to9lo[8]=
	{	0x00, 0x01, 0x08, 0x09,
		0x40, 0x41, 0x48, 0x49	};
	int c;
	int bi, bd, bi2, bd2, d;
	int i, j, k;

	tk_img_gen9to8();

	for(i=0; i<32768; i++)
	{
		c=tk_img_rgb15to9a(i);
		c&=0x1B6;

		bi=0; bd=999999;
		bi2=0; bd2=999999;
		for(j=0; j<8; j++)
		{
			k=c|tab3to9lo[j];
			k=tk_img_d9to8tab[k];
			d=tk_img_rgb15dist(i, tk_img_d8to15tab[k]);
			if(d<bd)
				{ bi2=bi; bd2=bd; bi=k; bd=d; }
			else if(d<bd2)
				{ bi2=k; bd2=d; }
		}
//		if(((bd2*7)/8)>bd)
//		if(((bd2*5)/8)>bd)
		if(((bd2*3)/8)>bd)
		{
			tk_img_d15to8tab0[i]=bi;
			tk_img_d15to8tab1[i]=bi;
		}else
		{
			tk_img_d15to8tab0[i]=bi;
			tk_img_d15to8tab1[i]=bi2;
		}
	}
}
#endif

#if 0
int tk_img_gen15to8()
{
	int bi, bd, bi2, bd2, d;
	int i, j, k;
	
	for(i=0; i<32768; i++)
	{
		bi=0; bd=999999;
		bi2=0; bd2=999999;
		for(j=0; j<256; j++)
		{
			d=tk_img_rgb15dist(i, tk_img_d8to15tab[j]);
			if(d<bd)
				{ bi2=bi; bd2=bd; bi=j; bd=d; }
			else if(d<bd2)
				{ bi2=j; bd2=d; }
		}
//		if(((bd2*7)/8)>bd)
//		if(((bd2*5)/8)>bd)
		if(((bd2*3)/8)>bd)
		{
			tk_img_d15to8tab0[i]=bi;
			tk_img_d15to8tab1[i]=bi;
		}else
		{
			tk_img_d15to8tab0[i]=bi;
			tk_img_d15to8tab1[i]=bi2;
		}
	}
}
#endif

int tk_img_gen15to8_bmp(byte *bmp)
{
	TKGDI_BITMAPINFOHEADER *bmi;
	byte *pal, *dat;
//	u16 *dtab;
	byte *tab;
	int cr, cg, cb, px;
	int isrp2;
	int i, j, k;

	if(bmp[0]==' ')
	{
		j=*(u32 *)(bmp+0x0C);
		dat=bmp+j;
		pal=bmp+0x0038;
		bmi=(TKGDI_BITMAPINFOHEADER *)(bmp+0x10);
	}else
		if(bmp[0]=='B')
	{
		j=*(u32 *)(bmp+0x0A);
		dat=bmp+j;
		pal=bmp+0x0036;
		bmi=(TKGDI_BITMAPINFOHEADER *)(bmp+0x0E);
	}else
	{
		tk_printf("tk_img_gen15to8_bmp: Sig Fail\n");
		return(-1);
//		pal=bmp+0xE+40;
//		dat=bmp+(*(int *)(bmp+2));
	}

	if(!tk_img_d15to8tab0)
	{
		tab=tk_malloc(65536);
		tk_img_d15to8tab0=tab+0;
		tk_img_d15to8tab1=tab+32768;

//		dtab=tk_malloc(65536*2);
//		memset(dtab, 0xFF, 65536*2);

//		tk_img_d15to8tab0=dat+0;
//		tk_img_d15to8tab1=dat+32768;
//		tk_img_d15to8tab1=dat+0;
	}

	for(i=0; i<256; i++)
	{
		cr=pal[i*4+2];
		cg=pal[i*4+1];
		cb=pal[i*4+0];
		cr>>=3;
		cg>>=3;
		cb>>=3;
		px=(cr<<10)|(cg<<5)|cb;
		tk_img_d8to15tab[i]=px;
	}

	isrp2=(bmi->biCompression==TKGDI_FCC_RGL3);

	if(isrp2)
	{
		TKPE_DecodeBufferRP2(dat, tab, 65536, 65536);
	}else
	{
		memcpy(tk_img_d15to8tab0, dat+0, 32768);
		memcpy(tk_img_d15to8tab1, dat+32768, 32768);
//		memcpy(tk_img_d15to8tab1, dat+0, 32768);
	}

//	for(i=0; i<128; i++)
//	{
//		memcpy(tk_img_d15to8tab0+(i*256), dat+(127-i)*256, 256);
//		memcpy(tk_img_d15to8tab1+(i*256), dat+(255-i)*256, 256);
//	}

	return(0);
}

void tk_img_uploadpal(void *pal)
{
	u64 *pp;
	u64 ta, tb, tv;
	int i, j;
	
	pp=pal;
	for(i=0; i<64; i++)
	{
	
		j=i|0x10FC0;
		tv=pp[i];
		ta=(u32)(tv>> 0);
		tb=(u32)(tv>>32);
		ta|=((u64)j)<<32;
		tb|=((u64)j)<<32;
		*((u64 *)0xFFFFF00BFF30ULL)=ta;
		*((u64 *)0xFFFFF00BFF38ULL)=tb;
	}
}

#ifdef __BGBCC__
void __rsrc__paldith8;
#else
char __rsrc__paldith8[16];
#endif

int tk_img_SetupPal8()
{
	byte *paldith;

	if(tk_img_d8to15ready)
		return(0);
	
	paldith=(byte *)(&__rsrc__paldith8);
	tk_img_d8to15ready=1;
	
	if((paldith[0]=='B') && (paldith[1]=='M'))
	{
		tk_printf("tk_img_SetupPal8: Pal BMP\n");
		tk_img_gen15to8_bmp(paldith);

//		tk_img_rgb15to8norm_dohash();
//		tk_img_gen15to8();
//		tk_img_gen11to8();
		tk_img_gen10to8();
	}else
	{
		tk_printf("tk_img_SetupPal8: Gen Pal\n");
//		tk_img_genpal1();
		tk_img_genpal2();
		tk_img_rgb15to8norm_dohash();
		tk_img_gen15to8();
//		tk_img_gen11to8();
		tk_img_gen10to8();
	}
	
	tk_img_uploadpal(tk_img_d8to15tab);
	
	return(1);
}

// #ifdef __BJX2__
#if 0


u32 tk_img_Repack4xRGB555toPal8a(u64 v);
u32 tk_img_Repack4xRGB555toPal8b(u64 v);

__asm {
tk_img_Repack4xRGB555toPal8a:
tk_img_Repack4xRGB555toPal8b:
	rgb5pcki8	r4, r2
	rts
};

#else
u32 tk_img_Repack4xRGB555toPal8a(u64 v)
{
	u32 p0, p1, p2, p3;
	u32 a0, a1, a2, a3;
	u32 b0, b1, b2, b3;
//	u32 c0, c1, c2, c3;
//	u32 d0, d1, d2, d3;
	u32 v1;

#if 1
	p0=tk_img_d15to8tab0[(v>> 0)&0x7FFF];
	p1=tk_img_d15to8tab1[(v>>16)&0x7FFF];
	p2=tk_img_d15to8tab0[(v>>32)&0x7FFF];
	p3=tk_img_d15to8tab1[(v>>48)&0x7FFF];
#endif

#if 0
	p0=(v>> 0);	p1=(v>>16);
	p2=(v>>32);	p3=(v>>48);
//	p0=((p0&0x03FC)<<1)|((p0>>12)&7);
//	p1=((p1&0x03FC)<<1)|((p1>>12)&7);
//	p2=((p2&0x03FC)<<1)|((p2>>12)&7);
//	p3=((p3&0x03FC)<<1)|((p3>>12)&7);

	v1=0x03FC;
	a0=p0&v1;	a1=p1&v1;
	a2=p2&v1;	a3=p3&v1;
	b0=p0>>12;	b1=p1>>12;
	b2=p2>>12;	b3=p3>>12;
	a0=(a0<<1);	a1=(a1<<1);
	a2=(a2<<1);	a3=(a3<<1);
	b0=b0&7;	b1=b1&7;
	b2=b2&7;	b3=b3&7;
	p0=a0|b0;	p1=a1|b1;
	p2=a2|b2;	p3=a3|b3;

	p0=tk_img_d11to8tab0[p0];
	p1=tk_img_d11to8tab1[p1];
	p2=tk_img_d11to8tab0[p2];
	p3=tk_img_d11to8tab1[p3];
#endif

	v1=p0|(p1<<8)|(p2<<16)|(p3<<24);
	return(v1);
}

u32 tk_img_Repack4xRGB555toPal8b(u64 v)
{
	u32 p0, p1, p2, p3;
	u32 a0, a1, a2, a3;
	u32 b0, b1, b2, b3;
//	u32 c0, c1, c2, c3;
//	u32 d0, d1, d2, d3;
	u32 v1;

#if 1	
	p0=tk_img_d15to8tab1[(v>> 0)&0x7FFF];
	p1=tk_img_d15to8tab0[(v>>16)&0x7FFF];
	p2=tk_img_d15to8tab1[(v>>32)&0x7FFF];
	p3=tk_img_d15to8tab0[(v>>48)&0x7FFF];
#endif

#if 0
	p0=(v>> 0);	p1=(v>>16);
	p2=(v>>32);	p3=(v>>48);

//	p0=((p0&0x03FC)<<1)|((p0>>12)&7);
//	p1=((p1&0x03FC)<<1)|((p1>>12)&7);
//	p2=((p2&0x03FC)<<1)|((p2>>12)&7);
//	p3=((p3&0x03FC)<<1)|((p3>>12)&7);

	v1=0x03FC;
	a0=p0&v1;	a1=p1&v1;
	a2=p2&v1;	a3=p3&v1;
	b0=p0>>12;	b1=p1>>12;
	b2=p2>>12;	b3=p3>>12;
	a0=(a0<<1);	a1=(a1<<1);
	a2=(a2<<1);	a3=(a3<<1);
	b0=b0&7;	b1=b1&7;
	b2=b2&7;	b3=b3&7;
	p0=a0|b0;	p1=a1|b1;
	p2=a2|b2;	p3=a3|b3;

	p0=tk_img_d11to8tab1[p0];
	p1=tk_img_d11to8tab0[p1];
	p2=tk_img_d11to8tab1[p2];
	p3=tk_img_d11to8tab0[p3];
#endif

	v1=p0|(p1<<8)|(p2<<16)|(p3<<24);
	return(v1);
}

#endif

// #ifdef __BJX2__
#if 0

u64 tk_img_Repack8xRGB555toPal8abI(u64 va, u64 vb);

__asm {
.global tk_img_Repack8xRGB555toPal8abI
tk_img_Repack8xRGB555toPal8abI:
	mov			0x0000800000008000, r16
//	shld.q		r16, 16, r17

//	xor			r4, r16, r18
//	xor			r5, r17, r19

	xor			r4, r16, r18	|	xor			r5, r16, r19

	rgb5pcki8	r18, r6
	rgb5pcki8	r19, r7
	movlld		r7, r6, r2
	rts
};

#endif

#ifdef __BJX2__
// #if 0

u64 tk_img_Repack8xRGB555toPal8abI(u64 va, u64 vb);

__asm {
.global tk_img_Repack8xRGB555toPal8abI
tk_img_Repack8xRGB555toPal8abI:

#if 0
.ifarch has_bitmov
	mov			0x0000800000008000, r16
	mov.q		tk_img_d15to8tab0, r19
	shld.q		r16, 16, r17
	mov			0, r18

	xor			r4, r16, r4
	xor			r5, r17, r5

	bitmov		r4, r18, 0x100000, r20
	bitmov		r4, r18, 0x1000F0, r21
	movu.b		(r19, r20), r20
	bitmov		r4, r18, 0x1000E0, r22
	movu.b		(r19, r21), r21
	bitmov		r4, r18, 0x1000D0, r23
	movu.b		(r19, r22), r22
	bitmov		r20, r6, 0x080000, r6
	movu.b		(r19, r23), r23
	bitmov		r21, r6, 0x100808, r6

	bitmov		r22, r6, 0x181010, r6
	bitmov		r5, r18, 0x100000, r20
	bitmov		r23, r6, 0x201818, r6
	bitmov		r5, r18, 0x1000F0, r21
	movu.b		(r19, r20), r20
	bitmov		r5, r18, 0x1000E0, r22
	movu.b		(r19, r21), r21
	bitmov		r5, r18, 0x1000D0, r23
	movu.b		(r19, r22), r22
	bitmov		r20, r7, 0x080000, r7
	movu.b		(r19, r23), r23
	bitmov		r21, r7, 0x100808, r7
	bitmov		r22, r7, 0x181010, r7
	bitmov		r23, r7, 0x201818, r7

	movlld		r7, r6, r2
	rts
.endif
#endif

#if 0
.ifarch has_bitmov
	mov			tk_img_d13to8tab_sh, r19
	mov			0, r18

	rgb5sh3		r4, r17
	bitmov		r17, r18, 0x0D00FE, r20
	bitmov		r17, r18, 0x0D00EE, r21
	movu.b		(r19, r20), r20
	bitmov		r17, r18, 0x0D00DE, r22
	movu.b		(r19, r21), r21
	bitmov		r17, r18, 0x0D00CE, r23
	movu.b		(r19, r22), r22
	bitmov		r20, r6, 0x080000, r6
	movu.b		(r19, r23), r23
	bitmov		r21, r6, 0x100808, r6
	rgb5sh3		r5, r17
	bitmov		r22, r6, 0x181010, r6
	bitmov		r17, r18, 0x0D00FE, r20
	bitmov		r23, r6, 0x201818, r6
	bitmov		r17, r18, 0x0D00EE, r21
	movu.b		(r19, r20), r20
	bitmov		r17, r18, 0x0D00DE, r22
	movu.b		(r19, r21), r21
	bitmov		r17, r18, 0x0D00CE, r23
	movu.b		(r19, r22), r22
	bitmov		r20, r7, 0x080000, r7
	movu.b		(r19, r23), r23
	bitmov		r21, r7, 0x100808, r7
	bitmov		r22, r7, 0x181010, r7
	bitmov		r23, r7, 0x201818, r7

	movlld		r7, r6, r2
	rts
.endif
#endif

#if 1
.ifarch has_bitmov
	mov			0x0000800000008000, r16
	mov			tk_img_d10to8tab_sh, r19
	shld.q		r16, 16, r17
	mov			0, r18

	xor			r4, r16, r4
	xor			r5, r17, r5

	rgb5sh3		r4, r17
	bitmov		r17, r18, 0x0B00FB, r20
	bitmov		r17, r18, 0x0B00EB, r21
	movu.b		(r19, r20), r20
	bitmov		r17, r18, 0x0B00DB, r22
	movu.b		(r19, r21), r21
	bitmov		r17, r18, 0x0B00CB, r23
	movu.b		(r19, r22), r22
	bitmov		r20, r6, 0x080000, r6
	movu.b		(r19, r23), r23
	bitmov		r21, r6, 0x100808, r6
	rgb5sh3		r5, r17
	bitmov		r22, r6, 0x181010, r6
	bitmov		r17, r18, 0x0B00FB, r20
	bitmov		r23, r6, 0x201818, r6
	bitmov		r17, r18, 0x0B00EB, r21
	movu.b		(r19, r20), r20
	bitmov		r17, r18, 0x0B00DB, r22
	movu.b		(r19, r21), r21
	bitmov		r17, r18, 0x0B00CB, r23
	movu.b		(r19, r22), r22
	bitmov		r20, r7, 0x080000, r7
	movu.b		(r19, r23), r23
	bitmov		r21, r7, 0x100808, r7
	bitmov		r22, r7, 0x181010, r7
	bitmov		r23, r7, 0x201818, r7

	movlld		r7, r6, r2
	rts
.endif
#endif

#if 1
.ifnarch has_bitmov
	mov			0x0000800000008000, r16
	mov			tk_img_d10to8tab_sh, r19
	shld.q		r16, 16, r17
	mov			0, r18

	xor			r4, r16, r4
	xor			r5, r17, r5

	mov			2047, r18		|	rgb5sh3		r4, r17
	shld.q		r17, -5, r20	|	shld.q		r17, -21, r21
	shld.q		r17, -37, r22	|	shld.q		r17, -53, r23
	and			r20, r18, r20	|	and			r21, r18, r21
	and			r22, r18, r22	|	movu.b		(r19, r20), r20
	and			r23, r18, r23	|	movu.b		(r19, r21), r21
	movu.b		(r19, r22), r22
	movu.b		(r19, r23), r23
	shld.q		r21,  8, r21
	shld.q		r22, 16, r22
	shld.q		r23, 24, r23
	or			r20, r21, r20
	or			r22, r23, r21
	or			r20, r21, r6

	rgb5sh3		r5, r17
	shld.q		r17, -5, r20	|	shld.q		r17, -21, r21
	shld.q		r17, -37, r22	|	shld.q		r17, -53, r23
	and			r20, r18, r20	|	and			r21, r18, r21
	and			r22, r18, r22	|	movu.b		(r19, r20), r20
	and			r23, r18, r23	|	movu.b		(r19, r21), r21
	movu.b		(r19, r22), r22
	movu.b		(r19, r23), r23
	shld.q		r21,  8, r21
	shld.q		r22, 16, r22
	shld.q		r23, 24, r23
	or			r20, r21, r20
	or			r22, r23, r21
	or			r20, r21, r7

	movlld		r7, r6, r2
	rts
.endif
#endif

};

#endif

#if 1

u64 tk_img_Repack8xRGB555toPal8abG(u64 va, u64 vb)
{
	byte *tab0, *tab1;
	u32 p0, p1, p2, p3;
	u32 a0, a1, a2, a3;
	u32 b0, b1, b2, b3;
	u32 c0, c1, c2, c3;
	u32 d0, d1, d2, d3;
	u32 v0, v1;
	u64 vc;

#if 0
	c0=tk_img_d15to8tab1[(va>> 0)&0x7FFF];
	c1=tk_img_d15to8tab0[(va>>16)&0x7FFF];
	c2=tk_img_d15to8tab1[(va>>32)&0x7FFF];
	c3=tk_img_d15to8tab0[(va>>48)&0x7FFF];

	d0=tk_img_d15to8tab0[(vb>> 0)&0x7FFF];
	d1=tk_img_d15to8tab1[(vb>>16)&0x7FFF];
	d2=tk_img_d15to8tab0[(vb>>32)&0x7FFF];
	d3=tk_img_d15to8tab1[(vb>>48)&0x7FFF];

	c1=c1<<8;	d1=d1<<8;
	c2=c2<<16;	d2=d2<<16;
	c3=c3<<24;	d3=d3<<24;
	c0=c0|c1;	d0=d0|d1;
	c2=c2|c3;	d2=d2|d3;
	v0=c0|c2;	v1=d0|d2;
#endif

#if 1
	v0=0x7FFF;
	tab0=tk_img_d15to8tab0;
	tab1=tk_img_d15to8tab1;
	a0=(va>> 0);	b0=(vb>> 0);
	a1=(va>>16);	b1=(vb>>16);
	a2=(va>>32);	b2=(vb>>32);
	a3=(va>>48);	b3=(vb>>48);

	a0=a0&v0;	b0=b0&v0;
	a1=a1&v0;	c0=tab0[a0];
	b1=b1&v0;	d0=tab1[b0];
	a2=a2&v0;	c1=tab1[a1];
	b2=b2&v0;	d1=tab0[b1];
	a3=a3&v0;	c2=tab0[a2];
	b3=b3&v0;	d2=tab1[b2];
	c1=c1<<8;	c3=tab1[a3];
	d1=d1<<8;	d3=tab0[b3];
	c2=c2<<16;	d2=d2<<16;
	c3=c3<<24;	d3=d3<<24;
	c0=c0|c1;	d0=d0|d1;
	c2=c2|c3;	d2=d2|d3;
	v0=c0|c2;	v1=d0|d2;
#endif

#if 0
	p0=(va>> 0);	p1=(va>>16);
	p2=(va>>32);	p3=(va>>48);

	tab0=tk_img_d11to8tab0;
	tab1=tk_img_d11to8tab1;

	v1=0x03FC;
	a0=p0&v1;	a1=p1&v1;
	a2=p2&v1;	a3=p3&v1;
	b0=p0>>12;	b1=p1>>12;
	b2=p2>>12;	b3=p3>>12;
	a0=(a0<<1);	a1=(a1<<1);
	a2=(a2<<1);	a3=(a3<<1);
	b0=b0&7;	b1=b1&7;
	b2=b2&7;	b3=b3&7;
	c0=a0|b0;	c1=a1|b1;
	c2=a2|b2;	c3=a3|b3;

	p0=(vb>> 0);	p1=(vb>>16);
	d0=tab0[c0];	d1=tab1[c1];
	p2=(vb>>32);	p3=(vb>>48);
	d2=tab0[c2];	d3=tab1[c3];

	v1=0x03FC;
	a0=p0&v1;	a1=p1&v1;
	a2=p2&v1;	a3=p3&v1;
	b0=p0>>12;	b1=p1>>12;
	d1=d1<<8;
	b2=p2>>12;	b3=p3>>12;
	d2=d2<<16;	d3=d3<<24;
	a0=(a0<<1);	a1=(a1<<1);
	a2=(a2<<1);	a3=(a3<<1);
	d0=d0|d1;	d2=d2|d3;
	b0=b0&7;	b1=b1&7;
	b2=b2&7;	b3=b3&7;
	v0=d0|d2;
	c0=a0|b0;	c1=a1|b1;
	c2=a2|b2;	c3=a3|b3;

	d0=tab1[c0];	d1=tab0[c1];
	d2=tab1[c2];	d3=tab0[c3];
	d1=d1<<8;	d2=d2<<16;	d3=d3<<24;
	d0=d0|d1;	d2=d2|d3;
	v1=d0|d2;
#endif

	vc=v0|(((u64)v1)<<32);
	return(vc);
}

#endif

#ifdef __BJX2__
static u64 (*tk_img_Repack8xRGB555toPal8ab_f)(u64 va, u64 vb);

u64 tk_img_Repack8xRGB555toPal8ab(u64 va, u64 vb)
{
	static u64 cpattab[4]={
		0x7FFF00007FFF0000LL,
		0x55552AAA55552AAALL,
		0x55AA2A555A5A25A5LL,
		0x7FFF00007FFF0000LL,
	};
	u64 v;
	u32 pa, pb;
	int i;
	
	if(tk_img_Repack8xRGB555toPal8ab_f)
		return(tk_img_Repack8xRGB555toPal8ab_f(va, vb));
	
//	pa=tk_img_Repack8xRGB555toPal8abI(
//		0x7FFF00007FFF0000LL, 0x00007FFF00007FFFLL);
//	pb=tk_img_Repack8xRGB555toPal8abG(
//		0x7FFF00007FFF0000LL, 0x00007FFF00007FFFLL);
	
	for(i=0; i<4; i++)
	{
		v=cpattab[i];

		pa=tk_img_Repack8xRGB555toPal8abI(v, v);
		pb=tk_img_Repack8xRGB555toPal8abG(v, v);
		if(pa!=pb)
			break;
	}
	
//	if(pa==pb)
	if(1)
//	if(0)
//	if(i>=4)
	{
		tk_img_Repack8xRGB555toPal8ab_f=tk_img_Repack8xRGB555toPal8abI;
		return(tk_img_Repack8xRGB555toPal8ab_f(va, vb));
	}
	
	tk_img_Repack8xRGB555toPal8ab_f=tk_img_Repack8xRGB555toPal8abG;
	return(tk_img_Repack8xRGB555toPal8ab_f(va, vb));
}
#else
u64 tk_img_Repack8xRGB555toPal8ab(u64 va, u64 vb)
{
	return(tk_img_Repack8xRGB555toPal8abG(va, vb));
}
#endif


void TKGDI_BlitUpdate_ScanCopy(u16 *ics, u32 *ict, int blkn);
void TKGDI_BlitUpdate_ScanCopy_Flip(u16 *ics, u32 *ict, int blkn);

#ifndef __BJX2__
void TKGDI_BlitUpdate_ScanCopy(u16 *ics, u32 *ict, int blkn)
{
	u64 *cs, *ct;
	int i, j, k;

	cs=(u64 *)ics;
	ct=(u64 *)ict;
	
	for(i=0; i<blkn; i++)
	{
		ct[0]=cs[  0];
		ct[1]=cs[ 80];
		ct[2]=cs[160];
		ct[3]=cs[240];
		cs++;
		ct+=4;
	}
}

void TKGDI_BlitUpdate_ScanCopy_Flip(u16 *ics, u32 *ict, int blkn)
{
	u64 *cs, *ct;
	int i, j, k;

	cs=(u64 *)ics;
	ct=(u64 *)ict;
	
	for(i=0; i<blkn; i++)
	{
		ct[3]=cs[  0];
		ct[2]=cs[ 80];
		ct[1]=cs[160];
		ct[0]=cs[240];
		cs++;
		ct+=4;
	}
}
#endif

#ifdef __BJX2__
__asm {
TKGDI_BlitUpdate_ScanCopy:

	CMP/GT	0, R6
	BF .done

#if 0
	.loop:
	MOV.Q	(R4,    0), R20
	MOV.Q	(R4,  640), R21
	MOV.Q	(R4, 1280), R22
	MOV.Q	(R4, 1920), R23
	MOV.Q	R20, (R5,  0)
	MOV.Q	R21, (R5,  8)
	ADD		-1, R6
	MOV.Q		R22, (R5, 16)
	ADD		8, R4
	MOV.Q		R23, (R5, 24)
	ADD		32, R5			| TEST		R6, R6
	BF .loop
#endif

#if 0
	SHLD	R6, -1, R6
	.loop:
							MOV.X	(R4,    0), R16
							MOV.X	(R4,  640), R18
							MOV.X	(R4, 1280), R20
							MOV.X	(R4, 1920), R22
							MOV.Q	R16, (R5,  0)
							MOV.Q	R18, (R5,  8)
							MOV.Q	R20, (R5, 16)
							MOV.Q	R22, (R5, 24)
							MOV.Q	R17, (R5, 32)
							MOV.Q	R19, (R5, 40)
	ADD		-1, R6		|	MOV.Q	R21, (R5, 48)
	ADD		16, R4		|	MOV.Q	R23, (R5, 56)
	ADD		64, R5		|	TEST	R6, R6
	BF .loop
#endif

#if 1
	SHLD	R6, -2, R6
	.loop:
							MOV.X	(R4,    0), R16
							MOV.X	(R4,  640), R18
							MOV.X	(R4, 1280), R20
							MOV.X	(R4, 1920), R22
							MOV.Q	R16, (R5,  0)
							MOV.Q	R18, (R5,  8)
							MOV.Q	R20, (R5, 16)
							MOV.Q	R22, (R5, 24)
							MOV.Q	R17, (R5, 32)
							MOV.Q	R19, (R5, 40)
							MOV.Q	R21, (R5, 48)
							MOV.Q	R23, (R5, 56)

							MOV.X	(R4,   16), R16
							MOV.X	(R4,  656), R18
							MOV.X	(R4, 1296), R20
							MOV.X	(R4, 1936), R22
							MOV.Q	R16, (R5,  64)
							MOV.Q	R18, (R5,  72)
							MOV.Q	R20, (R5,  80)
							MOV.Q	R22, (R5,  88)
							MOV.Q	R17, (R5,  96)
							MOV.Q	R19, (R5, 104)
							MOV.Q	R21, (R5, 112)
							MOV.Q	R23, (R5, 120)
	ADD		-1, R6
	ADD		32, R4

	ADD		128, R5
	TEST	R6, R6
	BF .loop
#endif

	.done:
	RTS

TKGDI_BlitUpdate_ScanCopy_Flip:

	CMP/GT	0, R6
	BF .done

	.loop:
	MOV.Q	(R4, 1920), R20
	MOV.Q	(R4, 1280), R21
	MOV.Q	(R4,  640), R22
	MOV.Q	(R4,    0), R23
	MOV.Q	R20, (R5,  0)
	MOV.Q	R21, (R5,  8)
	MOV.Q	R22, (R5, 16)
	MOV.Q	R23, (R5, 24)
	ADD		-1, R6
	ADD		8, R4
	ADD		32, R5
	TEST		R6, R6
	BF .loop

	.done:
	RTSU
};
#endif


void TKGDI_BlitUpdate_ScanCopyGen4p(u16 *ics,
	u32 *ict, int blkn, int sbxs)
{
	u64 *cs0, *cs1, *cs2, *cs3;
	u64 *ct0, *ct1, *ct2, *ct3;
	int i;
	
	cs0=(u64 *)(ics       );
	cs1=(u64 *)(ics+  sbxs);
	cs2=(u64 *)(ics+2*sbxs);
	cs3=(u64 *)(ics+3*sbxs);

	ct0=(u64 *)(((byte *)ict)+0);
	ct1=(u64 *)(((byte *)ict)+1);
	ct2=(u64 *)(((byte *)ict)+2);
	ct3=(u64 *)(((byte *)ict)+3);

	for(i=0; i<blkn; i++)
	{
		*ct0=*cs0;	*ct1=*cs1;
		*ct2=*cs2;	*ct3=*cs3;
		cs0++;	cs1++;	cs2++;	cs3++;
		ct0++;	ct1++;	ct2++;	ct3++;
	}
}

void TKGDI_BlitUpdate_ScanCopyGen4p8b(u16 *ics,
	u32 *ict, int blkn, int sbxs)
{
	u64 *cs0, *cs1, *cs2, *cs3;
	u64 *ct0, *ct1, *ct2, *ct3;
	u64 v0, v1, v2, v3;
	long icl;
	int i;
	
	cs0=(u64 *)(ics       );
	cs1=(u64 *)(ics+  sbxs);
	cs2=(u64 *)(ics+2*sbxs);
	cs3=(u64 *)(ics+3*sbxs);

	icl=(long)ict;
	i=icl&4;
	icl=(icl&(~6))|(i>>1);
	ct0=(u64 *)(((byte *)icl)+0);
	ct1=(u64 *)(((byte *)icl)+1);
	ct2=(u64 *)(((byte *)ict)+2);
	ct3=(u64 *)(((byte *)ict)+3);

	for(i=0; i<blkn; i++)
	{
		v0=cs0[0];	v1=cs1[0];
		v2=cs2[0];	v3=cs3[0];

//		v0=tk_img_Repack4xRGB555toPal8a(v0);
//		v1=tk_img_Repack4xRGB555toPal8b(v1);
//		v2=tk_img_Repack4xRGB555toPal8a(v2);
//		v3=tk_img_Repack4xRGB555toPal8b(v3);		
//		v0=v0|(v1<<32);
//		v1=v2|(v3<<32);

		v0=tk_img_Repack8xRGB555toPal8ab(v0, v1);
		v1=tk_img_Repack8xRGB555toPal8ab(v2, v3);

		if(i&1)
		{
			*ct2=v0;	*ct3=v1;
			ct2++;	ct3++;
		}else
		{
			*ct0=v0;	*ct1=v1;
			ct0++;	ct1++;
		}

		cs0++;	cs1++;	cs2++;	cs3++;
	}
}

void TKGDI_BlitUpdate_ScanCopyGen8b(u16 *ics, u32 *ict, int blkn, int sbxs)
{
	u64 *cs0, *cs1, *cs2, *cs3;
	u64 *ct;
	u64 v0, v1, v2, v3;
	u64 v4, v5, v6, v7;
	u64 w0, w1, w2, w3;
	int i;
	
	if(tkgdi_vid_planar && ((((long)ict>>44)&15)==0xF))
	{
		TKGDI_BlitUpdate_ScanCopyGen4p8b(ics, ict, blkn, sbxs);
		return;
	}
	
	cs0=(u64 *)(ics       );
	cs1=(u64 *)(ics+  sbxs);
	cs2=(u64 *)(ics+2*sbxs);
	cs3=(u64 *)(ics+3*sbxs);
	ct=(u64 *)ict;
	for(i=0; i<blkn; i++)
	{
		if((i+4)<=blkn)
		{
			v0=cs0[0];	v1=cs1[0];
			v2=cs2[0];	v3=cs3[0];
			v4=cs0[1];	v5=cs1[1];
			v6=cs2[1];	v7=cs3[1];

			w0=tk_img_Repack8xRGB555toPal8ab(v0, v1);
			w1=tk_img_Repack8xRGB555toPal8ab(v2, v3);
			w2=tk_img_Repack8xRGB555toPal8ab(v4, v5);
			w3=tk_img_Repack8xRGB555toPal8ab(v6, v7);

			v0=cs0[2];	v1=cs1[2];
			v2=cs2[2];	v3=cs3[2];
			v4=cs0[3];	v5=cs1[3];
			v6=cs2[3];	v7=cs3[3];
		
			ct[0]=w0;	ct[1]=w1;
			ct[2]=w2;	ct[3]=w3;

			w0=tk_img_Repack8xRGB555toPal8ab(v0, v1);
			w1=tk_img_Repack8xRGB555toPal8ab(v2, v3);
			w2=tk_img_Repack8xRGB555toPal8ab(v4, v5);
			w3=tk_img_Repack8xRGB555toPal8ab(v6, v7);

			ct[4]=w0;	ct[5]=w1;
			ct[6]=w2;	ct[7]=w3;

			cs0+=4;		cs1+=4;
			cs2+=4;		cs3+=4;
			ct+=8;
			
			i+=3;
			continue;
		}
	
		v0=cs0[0];	v1=cs1[0];
		v2=cs2[0];	v3=cs3[0];

//		v0=tk_img_Repack4xRGB555toPal8a(v0);
//		v1=tk_img_Repack4xRGB555toPal8b(v1);
//		v2=tk_img_Repack4xRGB555toPal8a(v2);
//		v3=tk_img_Repack4xRGB555toPal8b(v3);		
//		v0=v0|(v1<<32);
//		v1=v2|(v3<<32);

		v0=tk_img_Repack8xRGB555toPal8ab(v0, v1);
		v1=tk_img_Repack8xRGB555toPal8ab(v2, v3);
	
		ct[0]=v0;	ct[1]=v1;
		cs0++;		cs1++;
		cs2++;		cs3++;
		ct+=2;
	}
}

#define  SCAN1B_W0	192
#define  SCAN1B_W1	 64
#define  SCAN1B_W2	160
#define  SCAN1B_W3   96
#define  SCAN1B_W4	224
#define  SCAN1B_W5	 32
#define  SCAN1B_W6	144
#define  SCAN1B_W7  112

void TKGDI_BlitUpdate_ScanCopyGen1b_ConvLu(
	u64 pxa, u64 pxb,
	u64 pxc, u64 pxd, byte *lutab)
{
	lutab[ 0]=((pxa>>( 0+3))&127)+((pxa>>( 0+9))&63)+((pxa<<( 0+1))&63);
	lutab[ 1]=((pxa>>(16+3))&127)+((pxa>>(16+9))&63)+((pxa>>(16-1))&63);
	lutab[ 2]=((pxa>>(32+3))&127)+((pxa>>(32+9))&63)+((pxa>>(32-1))&63);
	lutab[ 3]=((pxa>>(48+3))&127)+((pxa>>(48+9))&63)+((pxa>>(48-1))&63);

	lutab[ 4]=((pxb>>( 0+3))&127)+((pxb>>( 0+9))&63)+((pxb<<( 0+1))&63);
	lutab[ 5]=((pxb>>(16+3))&127)+((pxb>>(16+9))&63)+((pxb>>(16-1))&63);
	lutab[ 6]=((pxb>>(32+3))&127)+((pxb>>(32+9))&63)+((pxb>>(32-1))&63);
	lutab[ 7]=((pxb>>(48+3))&127)+((pxb>>(48+9))&63)+((pxb>>(48-1))&63);

	lutab[ 8]=((pxc>>( 0+3))&127)+((pxc>>( 0+9))&63)+((pxc<<( 0+1))&63);
	lutab[ 9]=((pxc>>(16+3))&127)+((pxc>>(16+9))&63)+((pxc>>(16-1))&63);
	lutab[10]=((pxc>>(32+3))&127)+((pxc>>(32+9))&63)+((pxc>>(32-1))&63);
	lutab[11]=((pxc>>(48+3))&127)+((pxc>>(48+9))&63)+((pxc>>(48-1))&63);

	lutab[12]=((pxd>>( 0+3))&127)+((pxd>>( 0+9))&63)+((pxd<<( 0+1))&63);
	lutab[13]=((pxd>>(16+3))&127)+((pxd>>(16+9))&63)+((pxd>>(16-1))&63);
	lutab[14]=((pxd>>(32+3))&127)+((pxd>>(32+9))&63)+((pxd>>(32-1))&63);
	lutab[15]=((pxd>>(48+3))&127)+((pxd>>(48+9))&63)+((pxd>>(48-1))&63);
}

u16 TKGDI_BlitUpdate_ScanCopyGen1b_ConvWxbA(byte *lutab, byte *wtab)
{
	u16 px;
	int w0, w1, w2, w3;
	
	w0=wtab[0];
	w1=wtab[1];
	w2=wtab[2];
	w3=wtab[3];

#if 1
	px=
		((lutab[ 7]>w0)<<15) |
		((lutab[ 6]>w1)<<14) |
		((lutab[ 5]>w2)<<13) |
		((lutab[ 4]>w3)<<12) |
		((lutab[ 3]>w0)<<11) |
		((lutab[ 2]>w1)<<10) |
		((lutab[ 1]>w2)<< 9) |
		((lutab[ 0]>w3)<< 8) |
		((lutab[15]>w3)<< 7) |
		((lutab[14]>w2)<< 6) |
		((lutab[13]>w1)<< 5) |
		((lutab[12]>w0)<< 4) |
		((lutab[11]>w3)<< 3) |
		((lutab[10]>w2)<< 2) |
		((lutab[ 9]>w1)<< 1) |
		((lutab[ 8]>w0)<< 0) ;
#endif

	return(px);
}

u16 TKGDI_BlitUpdate_ScanCopyGen1b_ConvA(
	u64 pxa, u64 pxb,
	u64 pxc, u64 pxd)
{
	static byte wtab[4] = {SCAN1B_W0, SCAN1B_W1, SCAN1B_W6, SCAN1B_W7};
	byte lutab[16];
	u16 px;
	TKGDI_BlitUpdate_ScanCopyGen1b_ConvLu(pxa, pxb, pxc, pxd, lutab);
	px=TKGDI_BlitUpdate_ScanCopyGen1b_ConvWxbA(lutab, wtab);
	return(px);
}

u16 TKGDI_BlitUpdate_ScanCopyGen1b_ConvB(
	u64 pxa, u64 pxb,
	u64 pxc, u64 pxd)
{
	static byte wtab[4] = {SCAN1B_W4, SCAN1B_W5, SCAN1B_W2, SCAN1B_W3};
	byte lutab[16];
	u16 px;
	TKGDI_BlitUpdate_ScanCopyGen1b_ConvLu(pxa, pxb, pxc, pxd, lutab);
	px=TKGDI_BlitUpdate_ScanCopyGen1b_ConvWxbA(lutab, wtab);
	return(px);
}

void TKGDI_BlitUpdate_ScanCopyGen1b(u16 *ics, u32 *ict, int blkn, int sbxs)
{
	u64 *cs0, *cs1, *cs2, *cs3;
	u64 *cs4, *cs5, *cs6, *cs7;
	u64 *ct;
	u64 v0, v1, v2, v3;
	u64 v4, v5, v6, v7;
	u64 w0, w1, w2, w3;
	int i;
	
	if(tkgdi_vid_planar && ((((long)ict>>44)&15)==0xF))
	{
		TKGDI_BlitUpdate_ScanCopyGen4p8b(ics, ict, blkn, sbxs);
		return;
	}
	
	cs0=(u64 *)(ics+7*sbxs);
	cs1=(u64 *)(ics+6*sbxs);
	cs2=(u64 *)(ics+5*sbxs);
	cs3=(u64 *)(ics+4*sbxs);
	cs4=(u64 *)(ics+3*sbxs);
	cs5=(u64 *)(ics+2*sbxs);
	cs6=(u64 *)(ics+1*sbxs);
	cs7=(u64 *)(ics+0*sbxs);
	ct=(u64 *)ict;
	for(i=0; i<blkn; i++)
	{	
		v0=cs0[0];	v1=cs0[1];
		v2=cs1[0];	v3=cs1[1];
		v4=cs2[0];	v5=cs2[1];
		v6=cs3[0];	v7=cs3[1];
		w0=         TKGDI_BlitUpdate_ScanCopyGen1b_ConvA(v0, v1, v2, v3);
		w0=(w0<<16)|TKGDI_BlitUpdate_ScanCopyGen1b_ConvB(v4, v5, v6, v7);
		v0=cs4[0];	v1=cs4[1];
		v2=cs5[0];	v3=cs5[1];
		v4=cs6[0];	v5=cs6[1];
		v6=cs7[0];	v7=cs7[1];
		w0=(w0<<16)|TKGDI_BlitUpdate_ScanCopyGen1b_ConvA(v0, v1, v2, v3);
		w0=(w0<<16)|TKGDI_BlitUpdate_ScanCopyGen1b_ConvB(v4, v5, v6, v7);

// #ifdef __BGBCC__
#if 0
		v0=cs0[0];	v1=cs1[0];
		v2=cs2[0];	v3=cs3[0];
		v4=cs0[1];	v5=cs1[1];
		v6=cs2[1];	v7=cs3[1];

		w0=0;
		w0[63:63]=v0[ 9: 9];
		w0[62:62]=v0[25:25];
		w0[61:61]=v0[41:41];
		w0[60:60]=v0[57:57];
		w0[59:59]=v4[ 9: 9];
		w0[58:58]=v4[25:25];
		w0[57:57]=v4[41:41];
		w0[56:56]=v4[57:57];

		w0[55:55]=v1[ 9: 9];
		w0[54:54]=v1[25:25];
		w0[53:53]=v1[41:41];
		w0[52:52]=v1[57:57];
		w0[51:51]=v5[ 9: 9];
		w0[50:50]=v5[25:25];
		w0[59:49]=v5[41:41];
		w0[48:48]=v5[57:57];

		w0[47:47]=v2[ 9: 9];
		w0[46:46]=v2[25:25];
		w0[45:45]=v2[41:41];
		w0[44:44]=v2[57:57];
		w0[43:43]=v6[ 9: 9];
		w0[42:42]=v6[25:25];
		w0[41:41]=v6[41:41];
		w0[40:40]=v6[57:57];

		w0[39:39]=v3[ 9: 9];
		w0[38:38]=v3[25:25];
		w0[37:37]=v3[41:41];
		w0[36:36]=v3[57:57];
		w0[35:35]=v7[ 9: 9];
		w0[34:34]=v7[25:25];
		w0[33:33]=v7[41:41];
		w0[32:32]=v7[57:57];


		v0=cs4[0];	v1=cs5[0];
		v2=cs6[0];	v3=cs7[0];
		v4=cs4[1];	v5=cs5[1];
		v6=cs6[1];	v7=cs7[1];

		w0[31:31]=v0[ 9: 9];
		w0[30:30]=v0[25:25];
		w0[29:29]=v0[41:41];
		w0[28:28]=v0[57:57];
		w0[27:27]=v4[ 9: 9];
		w0[26:26]=v4[25:25];
		w0[25:25]=v4[41:41];
		w0[24:24]=v4[57:57];

		w0[23:23]=v1[ 9: 9];
		w0[22:22]=v1[25:25];
		w0[21:21]=v1[41:41];
		w0[20:20]=v1[57:57];
		w0[19:19]=v5[ 9: 9];
		w0[18:18]=v5[25:25];
		w0[17:17]=v5[41:41];
		w0[16:16]=v5[57:57];

		w0[15:15]=v2[ 9: 9];
		w0[14:14]=v2[25:25];
		w0[13:13]=v2[41:41];
		w0[12:12]=v2[57:57];
		w0[11:11]=v6[ 9: 9];
		w0[10:10]=v6[25:25];
		w0[ 9: 9]=v6[41:41];
		w0[ 8: 8]=v6[57:57];

		w0[ 7: 7]=v3[ 9: 9];
		w0[ 6: 6]=v3[25:25];
		w0[ 5: 5]=v3[41:41];
		w0[ 4: 4]=v3[57:57];
		w0[ 3: 3]=v7[ 9: 9];
		w0[ 2: 2]=v7[25:25];
		w0[ 1: 1]=v7[41:41];
		w0[ 0: 0]=v7[57:57];
#endif

//		w0=0x55AA55AA55AA55AAULL;

		ct[0]=w0;
		cs0+=2;		cs1+=2;
		cs2+=2;		cs3+=2;
		cs4+=2;		cs5+=2;
		cs6+=2;		cs7+=2;
		ct++;
	}
}

void TKGDI_BlitUpdate_ScanCopyGen(u16 *ics, u32 *ict, int blkn, int sbxs)
{
	u64 *cs0, *cs1, *cs2, *cs3;
	u64 *ct;
	u64 v0, v1, v2, v3;
	int i;
	
	if(tkgdi_vid_is8bit)
	{
		TKGDI_BlitUpdate_ScanCopyGen8b(ics, ict, blkn, sbxs);
		return;
	}

	if(tkgdi_vid_is1bit)
	{
		TKGDI_BlitUpdate_ScanCopyGen1b(ics, ict, blkn, sbxs);
		return;
	}

	if(tkgdi_vid_is2bit_cc)
	{
		TKGDI_BlitUpdate_ScanCellEncode128(ics, ict, blkn, sbxs);
		return;
	}

	if(tkgdi_vid_planar && ((((long)ict>>44)&15)==0xF))
	{	
		TKGDI_BlitUpdate_ScanCopyGen4p(ics, ict, blkn, sbxs);
		return;
	}
	
	cs0=(u64 *)(ics       );
	cs1=(u64 *)(ics+  sbxs);
	cs2=(u64 *)(ics+2*sbxs);
	cs3=(u64 *)(ics+3*sbxs);
	ct=(u64 *)ict;
	for(i=0; i<blkn; i++)
	{
		v0=cs0[0];	v1=cs1[0];
		v2=cs2[0];	v3=cs3[0];
		ct[0]=v0;	ct[1]=v1;
		ct[2]=v2;	ct[3]=v3;
		cs0++;		cs1++;
		cs2++;		cs3++;
//		ct[0]=*cs0++;
//		ct[1]=*cs1++;
//		ct[2]=*cs2++;
//		ct[3]=*cs3++;
		ct+=4;
	}
}

void TKGDI_BlitUpdate_ScanCopyIndex8(
	byte *ics, u32 *ict, int blkn, int sbxs,
	u16 *pal)
{
	u32 *cs0, *cs1, *cs2, *cs3;
	u32 cp0, cp1, cp2, cp3;
	u64 cv0, cv1, cv2, cv3;
	u64 *ct;
	int i;
	
	cs0=(u32 *)(ics       );
	cs1=(u32 *)(ics+  sbxs);
	cs2=(u32 *)(ics+2*sbxs);
	cs3=(u32 *)(ics+3*sbxs);
	ct=(u64 *)ict;
	for(i=0; i<blkn; i++)
	{
		cp0=*cs0++;		cp1=*cs1++;
		cp2=*cs2++;		cp3=*cs3++;

		cv0=pal[(cp0>>24)&0xFF];	cv1=pal[(cp1>>24)&0xFF];
		cv2=pal[(cp2>>24)&0xFF];	cv3=pal[(cp3>>24)&0xFF];
		cv0=(cv0<<16)|pal[(cp0>>16)&0xFF];	cv1=(cv1<<16)|pal[(cp1>>16)&0xFF];
		cv2=(cv2<<16)|pal[(cp2>>16)&0xFF];	cv3=(cv3<<16)|pal[(cp3>>16)&0xFF];
		cv0=(cv0<<16)|pal[(cp0>> 8)&0xFF];	cv1=(cv1<<16)|pal[(cp1>> 8)&0xFF];
		cv2=(cv2<<16)|pal[(cp2>> 8)&0xFF];	cv3=(cv3<<16)|pal[(cp3>> 8)&0xFF];
		cv0=(cv0<<16)|pal[(cp0>> 0)&0xFF];	cv1=(cv1<<16)|pal[(cp1>> 0)&0xFF];
		cv2=(cv2<<16)|pal[(cp2>> 0)&0xFF];	cv3=(cv3<<16)|pal[(cp3>> 0)&0xFF];
		ct[0]=cv0;		ct[1]=cv1;
		ct[2]=cv2;		ct[3]=cv3;
		ct+=4;
	}
}

void TKGDI_BlitUpdate_ScanCopyIndex4(
	byte *ics, u32 *ict, int blkn, int sbxs,
	u16 *pal)
{
	u16 *cs0, *cs1, *cs2, *cs3;
	u32 cp0, cp1, cp2, cp3;
	u64 cv0, cv1, cv2, cv3;
	u64 *ct;
	int sbxh;
	int i;
	
	sbxh=(sbxs+1)>>1;
	cs0=(u16 *)(ics       );
	cs1=(u16 *)(ics+  sbxh);
	cs2=(u16 *)(ics+2*sbxh);
	cs3=(u16 *)(ics+3*sbxh);
	ct=(u64 *)ict;
	for(i=0; i<blkn; i++)
	{
		cp0=*cs0++;		cp1=*cs1++;
		cp2=*cs2++;		cp3=*cs3++;

		cv0=pal[(cp0>>8)&15];				cv1=pal[(cp1>>8)&15];
		cv2=pal[(cp2>>8)&15];				cv3=pal[(cp3>>8)&15];
		cv0=(cv0<<16)|pal[(cp0>>12)&15];	cv1=(cv1<<16)|pal[(cp1>>12)&15];
		cv2=(cv2<<16)|pal[(cp2>>12)&15];	cv3=(cv3<<16)|pal[(cp3>>12)&15];
		cv0=(cv0<<16)|pal[(cp0>> 0)&15];	cv1=(cv1<<16)|pal[(cp1>> 0)&15];
		cv2=(cv2<<16)|pal[(cp2>> 0)&15];	cv3=(cv3<<16)|pal[(cp3>> 0)&15];
		cv0=(cv0<<16)|pal[(cp0>> 4)&15];	cv1=(cv1<<16)|pal[(cp1>> 4)&15];
		cv2=(cv2<<16)|pal[(cp2>> 4)&15];	cv3=(cv3<<16)|pal[(cp3>> 4)&15];
		ct[0]=cv0;		ct[1]=cv1;
		ct[2]=cv2;		ct[3]=cv3;
		ct+=4;
	}
}

u16 TKGDI_BlitUpdate_Repack32to16(u32 pix);

#ifdef __BJX2__
u64 TKGDI_CopyPixelSpan_GetRGB32x4(byte *src);
u64 TKGDI_CopyPixelSpan_GetRGB24x4(byte *src);
u64 TKGDI_CopyPixelSpan_GetRGB32x4H(byte *src, u32 scbi);
u64 TKGDI_CopyPixelSpan_GetRGB24x4H(byte *src, u32 scbi);
#endif

void TKGDI_BlitUpdate_ScanCopyRGB(
	byte *ics, u32 *ict, int blkn, int sbxs,
	int stride)
{
	byte *cs0, *cs1, *cs2, *cs3;
	u32 cp0, cp1, cp2, cp3;
	u64 cv0, cv1, cv2, cv3;
	u64 *ct;
	int i;
	
#ifdef __BJX2__
// TKGDI_CopyPixelSpan_GetRGB32x4H

	cs0=(byte *)(ics              );
	cs1=(byte *)(ics+  sbxs*stride);
	cs2=(byte *)(ics+2*sbxs*stride);
	cs3=(byte *)(ics+3*sbxs*stride);
	ct=(u64 *)ict;

	if(stride==4)
	{
		for(i=0; i<blkn; i++)
		{
			cv0=TKGDI_CopyPixelSpan_GetRGB32x4(cs0);
			cv1=TKGDI_CopyPixelSpan_GetRGB32x4(cs1);
			cv2=TKGDI_CopyPixelSpan_GetRGB32x4(cs2);
			cv3=TKGDI_CopyPixelSpan_GetRGB32x4(cs3);
			cs0+=16;	cs1+=16;
			cs2+=16;	cs3+=16;

			ct[0]=cv0;		ct[1]=cv1;
			ct[2]=cv2;		ct[3]=cv3;
			ct+=4;
		}

		return;
	}

	if(stride==3)
	{
		for(i=0; i<blkn; i++)
		{
			cv0=TKGDI_CopyPixelSpan_GetRGB24x4(cs0);
			cv1=TKGDI_CopyPixelSpan_GetRGB24x4(cs1);
			cv2=TKGDI_CopyPixelSpan_GetRGB24x4(cs2);
			cv3=TKGDI_CopyPixelSpan_GetRGB24x4(cs3);
			cs0+=12;	cs1+=12;
			cs2+=12;	cs3+=12;

			ct[0]=cv0;		ct[1]=cv1;
			ct[2]=cv2;		ct[3]=cv3;
			ct+=4;
		}

		return;
	}
#else
	cs0=(byte *)(ics              );
	cs1=(byte *)(ics+  sbxs*stride);
	cs2=(byte *)(ics+2*sbxs*stride);
	cs3=(byte *)(ics+3*sbxs*stride);
	ct=(u64 *)ict;
	for(i=0; i<blkn; i++)
	{
		cp0=*(u32 *)cs0;	cp1=*(u32 *)cs1;
		cp2=*(u32 *)cs2;	cp3=*(u32 *)cs3;
		cs0+=stride;	cs1+=stride;
		cs2+=stride;	cs3+=stride;
		cv0=TKGDI_BlitUpdate_Repack32to16(cp0);
		cv1=TKGDI_BlitUpdate_Repack32to16(cp1);
		cv2=TKGDI_BlitUpdate_Repack32to16(cp2);
		cv3=TKGDI_BlitUpdate_Repack32to16(cp3);

		cp0=*(u32 *)cs0;	cp1=*(u32 *)cs1;
		cp2=*(u32 *)cs2;	cp3=*(u32 *)cs3;
		cs0+=stride;	cs1+=stride;
		cs2+=stride;	cs3+=stride;
		cv0=cv0|(((u64)TKGDI_BlitUpdate_Repack32to16(cp0))<<16);
		cv1=cv1|(((u64)TKGDI_BlitUpdate_Repack32to16(cp1))<<16);
		cv2=cv2|(((u64)TKGDI_BlitUpdate_Repack32to16(cp2))<<16);
		cv3=cv3|(((u64)TKGDI_BlitUpdate_Repack32to16(cp3))<<16);

		cp0=*(u32 *)cs0;	cp1=*(u32 *)cs1;
		cp2=*(u32 *)cs2;	cp3=*(u32 *)cs3;
		cs0+=stride;	cs1+=stride;
		cs2+=stride;	cs3+=stride;
		cv0=cv0|(((u64)TKGDI_BlitUpdate_Repack32to16(cp0))<<32);
		cv1=cv1|(((u64)TKGDI_BlitUpdate_Repack32to16(cp1))<<32);
		cv2=cv2|(((u64)TKGDI_BlitUpdate_Repack32to16(cp2))<<32);
		cv3=cv3|(((u64)TKGDI_BlitUpdate_Repack32to16(cp3))<<32);

		cp0=*(u32 *)cs0;	cp1=*(u32 *)cs1;
		cp2=*(u32 *)cs2;	cp3=*(u32 *)cs3;
		cs0+=stride;	cs1+=stride;
		cs2+=stride;	cs3+=stride;
		cv0=cv0|(((u64)TKGDI_BlitUpdate_Repack32to16(cp0))<<48);
		cv1=cv1|(((u64)TKGDI_BlitUpdate_Repack32to16(cp1))<<48);
		cv2=cv2|(((u64)TKGDI_BlitUpdate_Repack32to16(cp2))<<48);
		cv3=cv3|(((u64)TKGDI_BlitUpdate_Repack32to16(cp3))<<48);

		ct[0]=cv0;		ct[1]=cv1;
		ct[2]=cv2;		ct[3]=cv3;
		ct+=4;
	}
#endif
}

void TKGDI_BlitUpdate_ScanCopyRGBH(
	byte *ics, u32 *ict, int blkn, int sbxs,
	int stride, u32 scbi)
{
	byte *cs0, *cs1, *cs2, *cs3;
	u32 cp0, cp1, cp2, cp3;
	u64 cv0, cv1, cv2, cv3;
	u64 *ct;
	int i;

#ifdef __BJX2__
// TKGDI_CopyPixelSpan_GetRGB32x4H

	cs0=(byte *)(ics              );
	cs1=(byte *)(ics+  sbxs*stride);
	cs2=(byte *)(ics+2*sbxs*stride);
	cs3=(byte *)(ics+3*sbxs*stride);
	ct=(u64 *)ict;

	if(stride==4)
	{
		for(i=0; i<blkn; i++)
		{
			cv0=TKGDI_CopyPixelSpan_GetRGB32x4H(cs0, scbi);
			cv1=TKGDI_CopyPixelSpan_GetRGB32x4H(cs1, scbi);
			cv2=TKGDI_CopyPixelSpan_GetRGB32x4H(cs2, scbi);
			cv3=TKGDI_CopyPixelSpan_GetRGB32x4H(cs3, scbi);
			cs0+=16;	cs1+=16;
			cs2+=16;	cs3+=16;

			ct[0]=cv0;		ct[1]=cv1;
			ct[2]=cv2;		ct[3]=cv3;
			ct+=4;
		}

		return;
	}

	if(stride==3)
	{
		for(i=0; i<blkn; i++)
		{
			cv0=TKGDI_CopyPixelSpan_GetRGB24x4H(cs0, scbi);
			cv1=TKGDI_CopyPixelSpan_GetRGB24x4H(cs1, scbi);
			cv2=TKGDI_CopyPixelSpan_GetRGB24x4H(cs2, scbi);
			cv3=TKGDI_CopyPixelSpan_GetRGB24x4H(cs3, scbi);
			cs0+=12;	cs1+=12;
			cs2+=12;	cs3+=12;

			ct[0]=cv0;		ct[1]=cv1;
			ct[2]=cv2;		ct[3]=cv3;
			ct+=4;
		}

		return;
	}
#else
	cs0=(byte *)(ics              );
	cs1=(byte *)(ics+  sbxs*stride);
	cs2=(byte *)(ics+2*sbxs*stride);
	cs3=(byte *)(ics+3*sbxs*stride);
	ct=(u64 *)ict;
	for(i=0; i<blkn; i++)
	{
		cp0=*(u32 *)cs0;	cp1=*(u32 *)cs1;
		cp2=*(u32 *)cs2;	cp3=*(u32 *)cs3;
		cs0+=stride;	cs1+=stride;
		cs2+=stride;	cs3+=stride;
		cv0=TKGDI_BlitUpdate_Repack32to16H(cp0);
		cv1=TKGDI_BlitUpdate_Repack32to16H(cp1);
		cv2=TKGDI_BlitUpdate_Repack32to16H(cp2);
		cv3=TKGDI_BlitUpdate_Repack32to16H(cp3);

		cp0=*(u32 *)cs0;	cp1=*(u32 *)cs1;
		cp2=*(u32 *)cs2;	cp3=*(u32 *)cs3;
		cs0+=stride;	cs1+=stride;
		cs2+=stride;	cs3+=stride;
		cv0=cv0|(((u64)TKGDI_BlitUpdate_Repack32to16H(cp0))<<16);
		cv1=cv1|(((u64)TKGDI_BlitUpdate_Repack32to16H(cp1))<<16);
		cv2=cv2|(((u64)TKGDI_BlitUpdate_Repack32to16H(cp2))<<16);
		cv3=cv3|(((u64)TKGDI_BlitUpdate_Repack32to16H(cp3))<<16);

		cp0=*(u32 *)cs0;	cp1=*(u32 *)cs1;
		cp2=*(u32 *)cs2;	cp3=*(u32 *)cs3;
		cs0+=stride;	cs1+=stride;
		cs2+=stride;	cs3+=stride;
		cv0=cv0|(((u64)TKGDI_BlitUpdate_Repack32to16H(cp0))<<32);
		cv1=cv1|(((u64)TKGDI_BlitUpdate_Repack32to16H(cp1))<<32);
		cv2=cv2|(((u64)TKGDI_BlitUpdate_Repack32to16H(cp2))<<32);
		cv3=cv3|(((u64)TKGDI_BlitUpdate_Repack32to16H(cp3))<<32);

		cp0=*(u32 *)cs0;	cp1=*(u32 *)cs1;
		cp2=*(u32 *)cs2;	cp3=*(u32 *)cs3;
		cs0+=stride;	cs1+=stride;
		cs2+=stride;	cs3+=stride;
		cv0=cv0|(((u64)TKGDI_BlitUpdate_Repack32to16H(cp0))<<48);
		cv1=cv1|(((u64)TKGDI_BlitUpdate_Repack32to16H(cp1))<<48);
		cv2=cv2|(((u64)TKGDI_BlitUpdate_Repack32to16H(cp2))<<48);
		cv3=cv3|(((u64)TKGDI_BlitUpdate_Repack32to16H(cp3))<<48);

		ct[0]=cv0;		ct[1]=cv1;
		ct[2]=cv2;		ct[3]=cv3;
		ct+=4;
	}
#endif
}

void TKGDI_BlitUpdate_EncodeCell8x8x1(u16 *ics, u32 *ict, int sbxs)
{
	byte clry[64];
	byte acya[8];
//	u16 clrp[64];
	byte *cys;
	u16 clrm, clrn;
	u16 clrm0, clrn0;
	u16 clrm1, clrn1;
	u16 clrm2, clrn2;
	u16 clrm3, clrn3;
	int mcy0, ncy0, acy0;
	int mcy1, ncy1, acy1;
	int mcy2, ncy2, acy2;
	int mcy3, ncy3, acy3;
	u64 pxb, pxc;
	int x, y, cy, mcy, ncy, acy;
//	int cr0, cg0, cb0, cr1, cg1, cb1;
//	int cr, cg, cb, acr, acg, acb;
	int px0, px1, px2, px3;
	int cy0, cy1, cy2, cy3;
	int bal, ccsplit, sh0, sh1;
	int i, j;

	px0=ics[3*sbxs+3];
	px1=ics[3*sbxs+5];
	px2=ics[5*sbxs+3];
	px3=ics[5*sbxs+5];

#if 0
	clrn=64000; bal=0;
	for(i=0; i<3; i++)
	{
		cy0=(px0>>(i*5))&31;
		cy1=(px1>>(i*5))&31;
		cy2=(px2>>(i*5))&31;
		cy3=(px3>>(i*5))&31;
		acy0=cy1-cy0;	acy1=cy2-cy1;
		acy2=cy3-cy2;	acy3=cy0-cy3;
		acy0^=acy0>>31;		acy1^=acy1>>31;
		acy2^=acy2>>31;		acy3^=acy3>>31;
		acy=acy0+acy1+acy2+acy3;
		if(acy<clrn)
			{ clrn=acy; bal=i; }
	}
#endif

#if 0
	cy0=(px0>>(0*5))&31;
	cy1=(px1>>(0*5))&31;
	cy2=(px2>>(0*5))&31;
	cy3=(px3>>(0*5))&31;
	mcy0=cy0; ncy0=cy0;
	if(cy1<mcy0)	{ mcy0=cy1; }
	if(cy1>ncy0)	{ ncy0=cy1; }
	if(cy2<mcy0)	{ mcy0=cy2; }
	if(cy2>ncy0)	{ ncy0=cy2; }
	if(cy3<mcy0)	{ mcy0=cy3; }
	if(cy3>ncy0)	{ ncy0=cy3; }

	cy0=(px0>>(1*5))&31;
	cy1=(px1>>(1*5))&31;
	cy2=(px2>>(1*5))&31;
	cy3=(px3>>(1*5))&31;
	mcy1=cy0; ncy1=cy0;
	if(cy1<mcy1)	{ mcy1=cy1; }
	if(cy1>ncy1)	{ ncy1=cy1; }
	if(cy2<mcy1)	{ mcy1=cy2; }
	if(cy2>ncy1)	{ ncy1=cy2; }
	if(cy3<mcy1)	{ mcy1=cy3; }
	if(cy3>ncy1)	{ ncy1=cy3; }

	cy0=(px0>>(2*5))&31;
	cy1=(px1>>(2*5))&31;
	cy2=(px2>>(2*5))&31;
	cy3=(px3>>(2*5))&31;
	mcy2=cy0; ncy2=cy0;
	if(cy1<mcy2)	{ mcy2=cy1; }
	if(cy1>ncy2)	{ ncy2=cy1; }
	if(cy2<mcy2)	{ mcy2=cy2; }
	if(cy2>ncy2)	{ ncy2=cy2; }
	if(cy3<mcy2)	{ mcy2=cy3; }
	if(cy3>ncy2)	{ ncy2=cy3; }
	
	cy0=ncy0-mcy0;
	cy1=ncy1-mcy1;
	cy2=ncy2-mcy2;
	
	bal=0; cy=cy0;
	if(cy1<cy)	{ bal=1; cy=cy1; }
	if(cy2<cy)	{ bal=2; cy=cy2; }
#endif

#if 0
	switch(bal)
	{
		case 0:		sh0=3; sh1= 8; break;
		case 1:		sh0=8; sh1=-2; break;
		case 2:		sh0=3; sh1=-2; break;
	}
#endif

#if 1
	mcy=256;	ncy=-1;
	mcy0=256;	ncy0=-1;
	mcy1=256;	ncy1=-1;
	mcy2=256;	ncy2=-1;
	mcy3=256;	ncy3=-1;
	clrm0=0;	clrm2=0;
	clrn0=0;	clrn2=0;
	clrm1=0;	clrm3=0;
	clrn1=0;	clrn3=0;

//	acy=0;
	cys=clry;
	for(y=0; y<8; y++)
	{
#if 0
		for(x=0; x<8; x++)
		{
			px0=ics[x];
//			pix=ics[7-x];
//			cy=(pix>>2)&255;
			cy=((px0>>3)&127)+((px0>>8)&127);
			clry[(y<<3)|x]=cy;
//			clrp[(y<<3)|x]=pix;
//			acy+=cy;
			
			if(cy<mcy)
				{ clrm=px0; mcy=cy; }
			if(cy>ncy)
				{ clrn=px0; ncy=cy; }
		}
#endif

#if 1
		if(y==4)
		{
			mcy0=mcy2;		ncy0=ncy2;
			mcy1=mcy3;		ncy1=ncy3;
			clrm0=clrm2;	clrn0=clrn2;
			clrm1=clrm3;	clrn1=clrn3;
			mcy2=256;		ncy2=-1;
			mcy3=256;		ncy3=-1;
		}
	
		px0=ics[0];		px1=ics[1];
		px2=ics[2];		px3=ics[3];

		cy0=((px0>>3)&127)+((px0>>8)&127);
		cy1=((px1>>3)&127)+((px1>>8)&127);
		cy2=((px2>>3)&127)+((px2>>8)&127);
		cy3=((px3>>3)&127)+((px3>>8)&127);
//		cy0=((px0>>3)&127)+((px0>>9)&63)+((px0<<1)&63);
//		cy1=((px1>>3)&127)+((px1>>9)&63)+((px1<<1)&63);
//		cy2=((px2>>3)&127)+((px2>>9)&63)+((px2<<1)&63);
//		cy3=((px3>>3)&127)+((px3>>9)&63)+((px3<<1)&63);
//		cy0=((px0>>sh0)&127)+((px0>>sh1)&127);
//		cy1=((px1>>sh0)&127)+((px1>>sh1)&127);
//		cy2=((px2>>sh0)&127)+((px2>>sh1)&127);
//		cy3=((px3>>sh0)&127)+((px3>>sh1)&127);

		cys[0]=cy0;		cys[1]=cy1;
		cys[2]=cy2;		cys[3]=cy3;
		if(cy0<mcy2)		{ clrm2=px0; mcy2=cy0; }
		if(cy0>ncy2)		{ clrn2=px0; ncy2=cy0; }
		if(cy1<mcy2)		{ clrm2=px1; mcy2=cy1; }
		if(cy1>ncy2)		{ clrn2=px1; ncy2=cy1; }
		if(cy2<mcy2)		{ clrm2=px2; mcy2=cy2; }
		if(cy2>ncy2)		{ clrn2=px2; ncy2=cy2; }
		if(cy3<mcy2)		{ clrm2=px3; mcy2=cy3; }
		if(cy3>ncy2)		{ clrn2=px3; ncy2=cy3; }

		px0=ics[4];		px1=ics[5];
		px2=ics[6];		px3=ics[7];

		cy0=((px0>>3)&127)+((px0>>8)&127);
		cy1=((px1>>3)&127)+((px1>>8)&127);
		cy2=((px2>>3)&127)+((px2>>8)&127);
		cy3=((px3>>3)&127)+((px3>>8)&127);
//		cy0=((px0>>3)&127)+((px0>>9)&63)+((px0<<1)&63);
//		cy1=((px1>>3)&127)+((px1>>9)&63)+((px1<<1)&63);
//		cy2=((px2>>3)&127)+((px2>>9)&63)+((px2<<1)&63);
//		cy3=((px3>>3)&127)+((px3>>9)&63)+((px3<<1)&63);
//		cy0=((px0>>sh0)&127)+((px0>>sh1)&127);
//		cy1=((px1>>sh0)&127)+((px1>>sh1)&127);
//		cy2=((px2>>sh0)&127)+((px2>>sh1)&127);
//		cy3=((px3>>sh0)&127)+((px3>>sh1)&127);

		cys[4]=cy0;		cys[5]=cy1;
		cys[6]=cy2;		cys[7]=cy3;
		if(cy0<mcy3)		{ clrm3=px0; mcy3=cy0; }
		if(cy0>ncy3)		{ clrn3=px0; ncy3=cy0; }
		if(cy1<mcy3)		{ clrm3=px1; mcy3=cy1; }
		if(cy1>ncy3)		{ clrn3=px1; ncy3=cy1; }
		if(cy2<mcy3)		{ clrm3=px2; mcy3=cy2; }
		if(cy2>ncy3)		{ clrn3=px2; ncy3=cy2; }
		if(cy3<mcy3)		{ clrm3=px3; mcy3=cy3; }
		if(cy3>ncy3)		{ clrn3=px3; ncy3=cy3; }

		cys+=8;
#endif

		ics+=sbxs;
	}

	ccsplit=0;
	acy=(mcy+ncy)>>1;

#if 1
	acy0=(mcy0+ncy0)>>1;
	acy1=(mcy1+ncy1)>>1;
	acy2=(mcy2+ncy2)>>1;
	acy3=(mcy3+ncy3)>>1;

	mcy=mcy0;		clrm=clrm0;
	ncy=ncy0;		clrn=clrn0;
	if(mcy1<mcy)		{ clrm=clrm1; mcy=mcy1; }
	if(mcy1>ncy)		{ clrn=clrn1; ncy=ncy1; }
	if(mcy2<mcy)		{ clrm=clrm2; mcy=mcy2; }
	if(ncy2>ncy)		{ clrn=clrn2; ncy=ncy2; }
	if(mcy3<mcy)		{ clrm=clrm3; mcy=mcy3; }
	if(ncy3>ncy)		{ clrn=clrn3; ncy=ncy3; }

	acy=(mcy+ncy)>>1;
	
	ccsplit=0;
	
//	if((acy<mcy0) || (acy>ncy0))	ccsplit=1;
//	if((acy<mcy1) || (acy>ncy1))	ccsplit=1;
//	if((acy<mcy2) || (acy>ncy2))	ccsplit=1;
//	if((acy<mcy3) || (acy>ncy3))	ccsplit=1;

#if 0
	cy0=acy-acy0;	cy1=acy-acy1;
	cy2=acy-acy2;	cy3=acy-acy3;
	cy0=cy0^(cy0>>31);	cy1=cy1^(cy1>>31);
	cy2=cy2^(cy2>>31);	cy3=cy3^(cy3>>31);
	cy=cy0|cy1|cy2|cy3;
//	ccsplit=(cy>64);
	ccsplit=(cy>32);
//	ccsplit=(cy>16);
	
//	ccsplit=1;
#endif

	px0=((clrn0<<2)&127)+((clrn0>>8)&127);
	px1=((clrn1<<2)&127)+((clrn1>>8)&127);
	px2=((clrn2<<2)&127)+((clrn2>>8)&127);
	px3=((clrn3<<2)&127)+((clrn3>>8)&127);

	cy0=px0-px1;	cy1=px1-px2;
	cy2=px2-px3;	cy3=px3-px0;
	cy0=cy0^(cy0>>31);	cy1=cy1^(cy1>>31);
	cy2=cy2^(cy2>>31);	cy3=cy3^(cy3>>31);
//	cy=cy0+cy1+cy2+cy3;
	cy=cy0|cy1|cy2|cy3;

	ccsplit=(cy>64);
//	ccsplit=(cy>32);
//	ccsplit=(cy>16);

	if(!ccsplit)
	{
		acy0=acy;	acy1=acy;
		acy2=acy;	acy3=acy;
		
//		px0=(5*acy+3*mcy)>>3;
//		px1=(5*acy+3*ncy)>>3;
		px0=(3*acy+mcy)>>2;
		px1=(3*acy+ncy)>>2;
		acya[0]=px0;	acya[1]=px1;
		acya[2]=px0;	acya[3]=px1;
		acya[4]=px0;	acya[5]=px1;
		acya[6]=px0;	acya[7]=px1;
	}else
	{
		acya[0]=(3*acy0+mcy0)>>2;
		acya[1]=(3*acy0+ncy0)>>2;
		acya[2]=(3*acy1+mcy1)>>2;
		acya[3]=(3*acy1+ncy1)>>2;
		acya[4]=(3*acy2+mcy2)>>2;
		acya[5]=(3*acy2+ncy2)>>2;
		acya[6]=(3*acy3+mcy3)>>2;
		acya[7]=(3*acy3+ncy3)>>2;
	}
#endif

#endif
	
#if 0
	mcy=256; ncy=-1;
	acr=0;	acy=0;	acb=0;
	acy=0;
	for(y=0; y<8; y++)
	{
		for(x=0; x<8; x++)
		{
			pix=ics[x];
//			pix=ics[7-x];
//			cy=(pix>>2)&255;
			cr=(pix>>10)&31;
			cg=(pix>> 5)&31;
			cb=(pix>> 0)&31;

//			cy=((pix>>3)&127)+((pix>>8)&127);
			cy=2*cg+cr+cb;

			clry[(y<<3)|x]=cy;
			clrp[(y<<3)|x]=pix;
			acy+=cy;

			acr+=cr;	acg+=cg;	acb+=cb;
			
			if(cy<mcy)
				{ cr0=cr; cg0=cg; cb0=cb; mcy=cy; }
			if(cy>ncy)
				{ cr1=cr; cg1=cg; cb1=cb; ncy=cy; }
		}
		ics+=sbxs;
	}
	
	acr=acr>>6;
	acg=acg>>6;
	acb=acb>>6;
//	acy=acy>>6;

	cr0=(3*cr0+acr)>>2;
	cg0=(3*cg0+acg)>>2;
	cb0=(3*cb0+acb)>>2;

	cr1=(3*cr1+acr)>>2;
	cg1=(3*cg1+acg)>>2;
	cb1=(3*cb1+acb)>>2;

	mcy=2*cg0+cr0+cb0;
	ncy=2*cg1+cr1+cb1;

	acy=(mcy+ncy)>>1;
	
	clrm=(cr0<<10)|(cg0<<5)|cb0;
	clrn=(cr1<<10)|(cg1<<5)|cb1;
#endif

	pxb=0;
//	for(i=0; i<64; i++)
//	{
//		cy=clry[i];
//		pxb=(pxb<<1)|(cy>=acy);
//	}

#if 1
	cys=clry;
	for(i=0; i<8; i++)
	{
		px0=acya[(i&5)^0];
		px1=acya[(i&5)^1];
		px2=acya[(i&5)^2];
		px3=acya[(i&5)^3];
	
		cy0=cys[0];	cy1=cys[1];
		cy2=cys[2];	cy3=cys[3];
		pxb=(pxb<<1)|(cy0>=px0);
		pxb=(pxb<<1)|(cy1>=px1);
		pxb=(pxb<<1)|(cy2>=px0);
		pxb=(pxb<<1)|(cy3>=px1);
		cy0=cys[4];	cy1=cys[5];
		cy2=cys[6];	cy3=cys[7];
		pxb=(pxb<<1)|(cy0>=px2);
		pxb=(pxb<<1)|(cy1>=px3);
		pxb=(pxb<<1)|(cy2>=px2);
		pxb=(pxb<<1)|(cy3>=px3);
		cys+=8;
	}
#endif

#if 1
	if(ccsplit)
	{
#if 0
		ncy0=tkgdi_blitupdate_rgb555to222(clrn0);
		mcy0=tkgdi_blitupdate_rgb555to222(clrm0);
		ncy1=tkgdi_blitupdate_rgb555to222(clrn1);
		mcy1=tkgdi_blitupdate_rgb555to222(clrm1);
		ncy2=tkgdi_blitupdate_rgb555to222(clrn2);
		mcy2=tkgdi_blitupdate_rgb555to222(clrm2);
		ncy3=tkgdi_blitupdate_rgb555to222(clrn3);
		mcy3=tkgdi_blitupdate_rgb555to222(clrm3);
#endif

#if 1
		ncy0=tkgdi_blitupdate_rgb555toC3Y3(clrn0);
		mcy0=tkgdi_blitupdate_rgb555toC3Y3(clrm0);
		ncy1=tkgdi_blitupdate_rgb555toC3Y3(clrn1);
		mcy1=tkgdi_blitupdate_rgb555toC3Y3(clrm1);
		ncy2=tkgdi_blitupdate_rgb555toC3Y3(clrn2);
		mcy2=tkgdi_blitupdate_rgb555toC3Y3(clrm2);
		ncy3=tkgdi_blitupdate_rgb555toC3Y3(clrn3);
		mcy3=tkgdi_blitupdate_rgb555toC3Y3(clrm3);
#endif
		
//		pxc=0x40000000U|(mcy0<<22)|(ncy0<<16)|(mcy1<<6)|(ncy1<<0);
		pxc=0x50000000U|(mcy0<<22)|(ncy0<<16)|(mcy1<<6)|(ncy1<<0);
		pxc=0x80000000U|(mcy2<<22)|(ncy2<<16)|(mcy3<<6)|(ncy3<<0)|(pxc<<32);
	}else
	{
		clrm&=0x7FFF;
		clrn&=0x7FFF;
		pxc=0x80000000ULL|(clrm<<15)|clrn;
	}
#endif
	
//	ict[0]=0x80000000U|(clrm<<15)|clrn;
//	ict[1]=0x0000;
//	ict[2]=pxb;
//	ict[3]=pxb>>32;

//	((u64 *)ict)[0]=0x80000000ULL|(clrm<<15)|clrn;
	((u64 *)ict)[0]=pxc;
	((u64 *)ict)[1]=pxb;
//	ict[2]=pxb;
//	ict[3]=pxb>>32;
}

void TKGDI_BlitUpdate_EncodeCell8x8x1IndexM(
	byte *ics, u32 *ict, int sbxs, u16 *pal, int bc, int md)
{
	u16 pxa[64];
	u16 *cta;
	byte *csa;
	int ystr;
	int i0, i1, i2, i3;
	int i, j, k;
	
	ystr=(sbxs*bc)/8;
	if(bc==8)
	{
		cta=pxa;
		csa=ics;
		for(i=0; i<8; i++)
		{
			i0=csa[0];	i1=csa[1];
			i2=csa[2];	i3=csa[3];
			i0=pal[i0];	i1=pal[i1];
			i2=pal[i2];	i3=pal[i3];
			cta[0]=i0;	cta[1]=i1;
			cta[2]=i2;	cta[3]=i3;

			i0=csa[4];	i1=csa[5];
			i2=csa[6];	i3=csa[7];
			i0=pal[i0];	i1=pal[i1];
			i2=pal[i2];	i3=pal[i3];
			cta[4]=i0;	cta[5]=i1;
			cta[6]=i2;	cta[7]=i3;

			csa+=ystr;
			cta+=8;
		}
	}else
		if(bc==4)
	{
		cta=pxa;
		csa=ics;
		for(i=0; i<8; i++)
		{
			i0=csa[0];	i2=csa[1];
			i1=i0&15;	i0>>=4;
			i3=i2&15;	i2>>=4;
			i0=pal[i0];	i1=pal[i1];
			i2=pal[i2];	i3=pal[i3];
			cta[0]=i0;	cta[1]=i1;
			cta[2]=i2;	cta[3]=i3;

			i0=csa[2];	i2=csa[3];
			i1=i0&15;	i0>>=4;
			i3=i2&15;	i2>>=4;
			i0=pal[i0];	i1=pal[i1];
			i2=pal[i2];	i3=pal[i3];
			cta[4]=i0;	cta[5]=i1;
			cta[6]=i2;	cta[7]=i3;

			csa+=ystr;
			cta+=8;
		}
	}
	
	if(md==1)
	{
		TKGDI_BlitUpdate_EncodeCell8x8x1(pxa, ict, 8);
	}else
		if(md==2)
	{
		TKGDI_BlitUpdate_EncodeCell8x8x2(pxa, ict, 8);
	}
}

void TKGDI_BlitUpdate_EncodeCell8x8x1Index(
	byte *ics, u32 *ict, int sbxs, u16 *pal, int bc)
{
	TKGDI_BlitUpdate_EncodeCell8x8x1IndexM(ics, ict, sbxs, pal, bc, 1);
}

void TKGDI_BlitUpdate_EncodeCell8x8x2Index(
	byte *ics, u32 *ict, int sbxs, u16 *pal, int bc)
{
	TKGDI_BlitUpdate_EncodeCell8x8x1IndexM(ics, ict, sbxs, pal, bc, 2);
}

static byte tkgdi_enc2b_rcptab[256];
static byte tkgdi_enc2b_rcptab_init;

void TKGDI_BlitUpdate_FillEncode2Tab()
{
	int i, j, k;
	
	if(tkgdi_enc2b_rcptab_init)
		return;
	tkgdi_enc2b_rcptab_init=1;
	
	for(i=0; i<256; i++)
	{
		j=896/(i+1);
		tkgdi_enc2b_rcptab[i]=j;
	}
}

void TKGDI_BlitUpdate_EncodeCell4x4x2(
	u16 *ics, u32 *ict0, u32 *ict1, int sbxs)
{
	byte clry[16];
	byte *pclry;
	u16 clrm, clrn, pix;
	u64 pxb;
	int x, y, cy, mcy, ncy, acy, acy0, acy1, cybi;
	int cy0, cy1, cy2, cy3, rcp, pxc;
	int i, j, k;
	
	mcy=256; ncy=-1;
	for(y=0; y<4; y++)
	{
#if 0
		for(x=0; x<4; x++)
		{
			pix=ics[x];
//			cy=(pix>>2)&255;
//			cy=((pix>>3)&127)+((pix>>9)&63)+(pix&31);

			acy0=pix>>3;	acy1=pix>>8;
			acy0&=127;		acy1&=127;
			cy=acy0+acy1;

//			cy=((pix>>3)&127)+((pix>>8)&127);
			clry[(y<<2)|x]=cy;
			
			if(cy<mcy)
				{ clrm=pix; mcy=cy; }
			if(cy>ncy)
				{ clrn=pix; ncy=cy; }
		}
#endif

#if 1
			pxb=*(u64 *)ics;
			pclry=clry+(y<<2);

			acy0=pxb>>3;	acy1=pxb>>8;
			acy0&=127;		acy1&=127;
			pix=pxb>>16;	cy0=acy0+acy1;
			acy0=pix>>3;	acy1=pix>>8;
			acy0&=127;		acy1&=127;
			pix=pxb>>32;	cy1=acy0+acy1;
			acy0=pix>>3;	acy1=pix>>8;
			acy0&=127;		acy1&=127;
			pix=pxb>>48;	cy2=acy0+acy1;
			acy0=pix>>3;	acy1=pix>>8;
			acy0&=127;		acy1&=127;
			cy3=acy0+acy1;

			pclry[0]=cy0;	pclry[1]=cy1;
			pclry[2]=cy2;	pclry[3]=cy3;
			
			if(cy0<mcy)
				{ clrm=pxb; mcy=cy0; }
			if(cy0>ncy)
				{ clrn=pxb; ncy=cy0; }

			if(cy1<mcy)
				{ clrm=pxb>>16; mcy=cy1; }
			else if(cy1>ncy)
				{ clrn=pxb>>16; ncy=cy1; }

			if(cy2<mcy)
				{ clrm=pxb>>32; mcy=cy2; }
			else if(cy2>ncy)
				{ clrn=pxb>>32; ncy=cy2; }

			if(cy3<mcy)
				{ clrm=pxb>>48; mcy=cy3; }
			else if(cy3>ncy)
				{ clrn=pxb>>48; ncy=cy3; }
#endif

		ics+=sbxs;
	}
	
	acy=(mcy+ncy)>>1;
//	acy0=(mcy+acy)>>1;
//	acy1=(acy+ncy)>>1;
//	cybi=(acy1-acy)>>1;

#if 0
	for(i=0; i<16; i++)
	{
		cy=clry[i];
//		if(i&1)	cy+=cybi;
//		else	cy-=cybi;
		j=(cy>=acy0);
		k=(cy>=acy1);
		if(cy>=acy)
			j=k+2;
//		j=(cy>=acy);
//		j=j|(j<<1);
		pxb=(pxb<<2)|j;
	}
#endif

#if 1
	pxc=0;
	rcp=tkgdi_enc2b_rcptab[ncy-mcy];
	pclry=clry;
	for(i=0; i<4; i++)
	{
		cy0=pclry[0];	cy1=pclry[1];
		cy2=pclry[2];	cy3=pclry[3];
		pclry+=4;
		
//		cy0+=cybi;	cy1-=cybi;
//		cy2+=cybi;	cy3-=cybi;
				
		cy0-=acy;	cy1-=acy;
		cy2-=acy;	cy3-=acy;
		cy0*=rcp;	cy1*=rcp;
		cy2*=rcp;	cy3*=rcp;

//		cybi=-cybi;

		cy0=(cy0>>8)+2;		cy1=(cy1>>8)+2;
		cy2=(cy2>>8)+2;		cy3=(cy3>>8)+2;

//		pxb=(pxb<<2)|cy0;	pxb=(pxb<<2)|cy1;
//		pxb=(pxb<<2)|cy2;	pxb=(pxb<<2)|cy3;

		pxc=(pxc<<2)|cy0;	pxc=(pxc<<2)|cy1;
		pxc=(pxc<<2)|cy2;	pxc=(pxc<<2)|cy3;
	}
#endif
	
	clrm&=0x7FFF;
	clrn&=0x7FFF;
	
	*ict0=0xC0000000U|(clrm<<15)|clrn;
//	*ict1=pxb;
	*ict1=pxc;
}

// #if 1
#ifdef __BJX2__
u64 TKGDI_BlitUpdate_EncodeCellUTX2(u16 *ics, int sbxs);
int TKGDI_BlitUpdate_EncodeCell2xUTX2(u16 *ics, int sbxs, u64 *ct);
int TKGDI_BlitUpdate_EncodeCell4xUTX2(u16 *ics, int sbxs, u64 *ct);

__asm {
TKGDI_BlitUpdate_EncodeCellUTX2:
	MOV.Q	(R4), R20
	LEA.W	(R4, R5), R4
	MOV.Q	(R4), R21
	RGB5MINMAX	R20, R16
	LEA.W	(R4, R5), R4
	RGB5MINMAX	R21, R17
	MOV.Q	(R4), R22
	LEA.W	(R4, R5), R4
	MOV.Q	(R4), R23
	RGB5MINMAX	R22, R18
	MOVLLD		R16, R17, R6
	RGB5MINMAX	R23, R19
	RGB5MINMAX	R6, R6

	MOVLLD		R18, R19, R7
	RGB5MINMAX	R7, R7

	MOVLLD		R6, R7, R3
	RGB5MINMAX	R3, R3
	
	RGB5CCENC2	R20, R3, R7
//	MOV			0xCCCCCCCC, R20
	RGB5CCENC2	R21, R3, R7
//	MOV			0x33333333, R21
	RGB5CCENC2	R22, R3, R7
//	MOV			0xF0F0F0F0, R22
	RGB5CCENC2	R23, R3, R7
//	MOV			0x0F0F0F0F, R23

//	SHLD		R7,	 2, R16		|	SHLD		R7,	-2, R17
//	AND			R16, R20, R16	|	AND			R17, R21, R17
//	OR			R16, R17, R7
//	SHLD		R7,	 4, R16		|	SHLD		R7,	-4, R17
//	AND			R16, R22, R16	|	AND			R17, R23, R17
//	OR			R16, R17, R7

	MOVLLD		R7, R3, R2
	RTS

.ifarch has_xgpr
// .ifarch has_foo

TKGDI_BlitUpdate_EncodeCell2xUTX2:
	MOV.Q	(R4, 0), R32
	MOV.Q	(R4, 8), R33
	LEA.W	(R4, R5), R4
	MOV.Q	(R4, 0), R34
	MOV.Q	(R4, 8), R35
	LEA.W	(R4, R5), R4
	RGB5MINMAX	R32, R16
	RGB5MINMAX	R34, R17

	MOV.Q	(R4, 0), R36
	MOV.Q	(R4, 8), R37
	LEA.W	(R4, R5), R4
	MOV.Q	(R4, 0), R38
	MOV.Q	(R4, 8), R39
	RGB5MINMAX	R36, R18
	RGB5MINMAX	R38, R19

	MOVLLD		R16, R17, R20
	RGB5MINMAX	R20, R20

	MOVLLD		R18, R19, R21
	RGB5MINMAX	R21, R21

	RGB5MINMAX	R33, R16
	RGB5MINMAX	R35, R17
	MOVLLD		R20, R21, R20

	RGB5MINMAX	R37, R18
	RGB5MINMAX	R39, R19

	RGB5MINMAX	R20, R20

	MOVLLD		R16, R17, R22
	MOVLLD		R18, R19, R23

	RGB5MINMAX	R22, R22
	RGB5MINMAX	R23, R23
	MOVLLD		R22, R23, R22
	RGB5MINMAX	R22, R22


	RGB5CCENC2	R32, R20, R21
	RGB5CCENC2	R33, R22, R23
	RGB5CCENC2	R34, R20, R21
	RGB5CCENC2	R35, R22, R23
	RGB5CCENC2	R36, R20, R21
	RGB5CCENC2	R37, R22, R23
	RGB5CCENC2	R38, R20, R21
	RGB5CCENC2	R39, R22, R23

	MOVLLD		R21, R20, R2
	MOVLLD		R23, R22, R3
	
	MOV.Q		R2, (R6, 0)
	MOV.Q		R3, (R6, 8)
	
	RTS

#if 1
TKGDI_BlitUpdate_EncodeCell4xUTX2:
	MOV.Q	(R4, 0), R32
	MOV.Q	(R4, 8), R33
	MOV.Q	(R4, 16), R48
	MOV.Q	(R4, 24), R49
	LEA.W	(R4, R5), R4

	MOV.Q	(R4, 0), R34
	MOV.Q	(R4, 8), R35
	MOV.Q	(R4, 16), R50
	MOV.Q	(R4, 24), R51
	LEA.W	(R4, R5), R4

	RGB5MINMAX	R32, R16
	RGB5MINMAX	R34, R17

	MOV.Q	(R4, 0), R36
	MOV.Q	(R4, 8), R37
	MOV.Q	(R4, 16), R52
	MOV.Q	(R4, 24), R53

	MOVLLD		R16, R17, R20
	LEA.W	(R4, R5), R4
	RGB5MINMAX	R20, R20

	RGB5MINMAX	R36, R18

	MOV.Q	(R4, 0), R38
	MOV.Q	(R4, 8), R39
	MOV.Q	(R4, 16), R54
	RGB5MINMAX	R38, R19
	MOV.Q	(R4, 24), R55


	MOVLLD		R18, R19, R21
	RGB5MINMAX	R21, R21

	RGB5MINMAX	R33, R16
	RGB5MINMAX	R35, R17
	MOVLLD		R20, R21, R20

	RGB5MINMAX	R37, R18
	RGB5MINMAX	R39, R19

	RGB5MINMAX	R20, R20

	MOVLLD		R16, R17, R22
	MOVLLD		R18, R19, R23

	RGB5MINMAX	R22, R22
	RGB5MINMAX	R23, R23

	RGB5MINMAX	R48, R16
	RGB5MINMAX	R50, R17

	MOVLLD		R22, R23, R22

	RGB5MINMAX	R52, R18
	RGB5MINMAX	R54, R19

	RGB5MINMAX	R22, R22

	MOVLLD		R16, R17, R2
	MOVLLD		R18, R19, R3
	RGB5MINMAX	R2, R2
	RGB5MINMAX	R3, R3

	RGB5MINMAX	R49, R16
	RGB5MINMAX	R51, R17

	RGB5MINMAX	R53, R18
	RGB5MINMAX	R55, R19

	MOVLLD		R2, R3, R21

	MOVLLD		R16, R17, R2
	MOVLLD		R18, R19, R3
	RGB5MINMAX	R2, R2
	RGB5MINMAX	R3, R3

	RGB5MINMAX	R21, R21

	MOVLLD		R2, R3, R23

	RGB5MINMAX	R23, R23


	RGB5CCENC2	R32, R20, R16
	RGB5CCENC2	R33, R22, R17
	RGB5CCENC2	R48, R21, R18
	RGB5CCENC2	R49, R23, R19

	RGB5CCENC2	R34, R20, R16
	RGB5CCENC2	R35, R22, R17
	RGB5CCENC2	R50, R21, R18
	RGB5CCENC2	R51, R23, R19

	RGB5CCENC2	R36, R20, R16
	RGB5CCENC2	R37, R22, R17
	RGB5CCENC2	R52, R21, R18
	RGB5CCENC2	R53, R23, R19

	RGB5CCENC2	R38, R20, R16
	RGB5CCENC2	R39, R22, R17
	RGB5CCENC2	R54, R21, R18
	RGB5CCENC2	R55, R23, R19

	MOVLLD		R16, R20, R2
	MOVLLD		R17, R22, R3
	MOV.Q		R2, (R6, 0)
	MOV.Q		R3, (R6, 8)

	MOVLLD		R18, R21, R2
	MOVLLD		R19, R23, R3
	MOV.Q		R2, (R6, 16)
	MOV.Q		R3, (R6, 24)
	
	RTS
#endif

.else

// .extern TKGDI_BlitUpdate_EncodeCellUTX2

TKGDI_BlitUpdate_EncodeCell4xUTX2:
ADD		-64, SP

MOV		LR, R1
MOV.X	R4, (SP,  0)
MOV.Q	R6, (SP, 16)
MOV.Q	R1, (SP, 24)
MOV.X	R8, (SP, 32)
MOV.X	R10, (SP, 48)

MOV		R4, R8
MOV		R5, R9
MOV		R6, R10

BSR		TKGDI_BlitUpdate_EncodeCellUTX2
MOV.Q	R2, (R10, 0)

ADD		R8, 8, R4
MOV		R9, R5
BSR		TKGDI_BlitUpdate_EncodeCellUTX2
MOV.Q	R2, (R10, 8)

ADD		R8, 16, R4
MOV		R9, R5
BSR		TKGDI_BlitUpdate_EncodeCellUTX2
MOV.Q	R2, (R10, 16)

ADD		R8, 24, R4
MOV		R9, R5
BSR		TKGDI_BlitUpdate_EncodeCellUTX2
MOV.Q	R2, (R10, 24)

MOV.Q	(SP, 24), R1
MOV.X	(SP, 32), R8
MOV.X	(SP, 48), R10

ADD		64, SP
JMP		R1

.endif

};

#if 0
int TKGDI_BlitUpdate_EncodeCell4xUTX2(u16 *ics, int sbxs, u64 *ct)
{
//	ct[0]=TKGDI_BlitUpdate_EncodeCellUTX2(ics+0, sbxs);
//	ct[1]=TKGDI_BlitUpdate_EncodeCellUTX2(ics+4, sbxs);
//	ct[2]=TKGDI_BlitUpdate_EncodeCellUTX2(ics+8, sbxs);
//	ct[3]=TKGDI_BlitUpdate_EncodeCellUTX2(ics+12, sbxs);

	TKGDI_BlitUpdate_EncodeCell2xUTX2(ics+0, sbxs, ct+0);
	TKGDI_BlitUpdate_EncodeCell2xUTX2(ics+8, sbxs, ct+2);
}
#endif

#else
u64 TKGDI_BlitUpdate_EncodeCellUTX2(u16 *ics, int sbxs)
{
	byte clry[16];
	byte *pclry;
	u16 clrm, clrn, pix;
	u64 pxb;
	int x, y, cy, mcy, ncy, acy, acy0, acy1, cybi;
	int cy0, cy1, cy2, cy3, rcp, pxc;
	int i, j, k;
	
	mcy=256; ncy=-1;
	for(y=0; y<4; y++)
	{
		pxb=*(u64 *)ics;
		pclry=clry+(y<<2);

		acy0=pxb>>3;	acy1=pxb>>8;
		acy0&=127;		acy1&=127;
		pix=pxb>>16;	cy0=acy0+acy1;
		acy0=pix>>3;	acy1=pix>>8;
		acy0&=127;		acy1&=127;
		pix=pxb>>32;	cy1=acy0+acy1;
		acy0=pix>>3;	acy1=pix>>8;
		acy0&=127;		acy1&=127;
		pix=pxb>>48;	cy2=acy0+acy1;
		acy0=pix>>3;	acy1=pix>>8;
		acy0&=127;		acy1&=127;
		cy3=acy0+acy1;

		pclry[0]=cy0;	pclry[1]=cy1;
		pclry[2]=cy2;	pclry[3]=cy3;
		
		if(cy0<mcy)
			{ clrm=pxb; mcy=cy0; }
		if(cy0>ncy)
			{ clrn=pxb; ncy=cy0; }

		if(cy1<mcy)
			{ clrm=pxb>>16; mcy=cy1; }
		else if(cy1>ncy)
			{ clrn=pxb>>16; ncy=cy1; }

		if(cy2<mcy)
			{ clrm=pxb>>32; mcy=cy2; }
		else if(cy2>ncy)
			{ clrn=pxb>>32; ncy=cy2; }

		if(cy3<mcy)
			{ clrm=pxb>>48; mcy=cy3; }
		else if(cy3>ncy)
			{ clrn=pxb>>48; ncy=cy3; }

		ics+=sbxs;
	}
	
	acy=(mcy+ncy)>>1;

	pxc=0;
	rcp=tkgdi_enc2b_rcptab[ncy-mcy];
	pclry=clry;
	for(i=0; i<4; i++)
	{
		cy0=pclry[0];	cy1=pclry[1];
		cy2=pclry[2];	cy3=pclry[3];
		pclry+=4;				

		cy0-=acy;	cy1-=acy;
		cy2-=acy;	cy3-=acy;
		cy0*=rcp;	cy1*=rcp;
		cy2*=rcp;	cy3*=rcp;

		cy0=(cy0>>8)+2;		cy1=(cy1>>8)+2;
		cy2=(cy2>>8)+2;		cy3=(cy3>>8)+2;

		pxc=(pxc<<2)|cy0;	pxc=(pxc<<2)|cy1;
		pxc=(pxc<<2)|cy2;	pxc=(pxc<<2)|cy3;
	}
	
	clrm&=0x7FFF;
	clrn&=0x7FFF;

	pxb=(((u64)pxc)<<32)|(clrm<<16)|clrn;
	return(pxb);
}

int TKGDI_BlitUpdate_EncodeCell4xUTX2(u16 *ics, int sbxs, u64 *ct)
{
	ct[0]=TKGDI_BlitUpdate_EncodeCellUTX2(ics+0, sbxs);
	ct[1]=TKGDI_BlitUpdate_EncodeCellUTX2(ics+4, sbxs);
	ct[2]=TKGDI_BlitUpdate_EncodeCellUTX2(ics+8, sbxs);
	ct[3]=TKGDI_BlitUpdate_EncodeCellUTX2(ics+12, sbxs);
}
#endif

void TKGDI_BlitUpdate_EncodeCell8x8x2(u16 *ics, u32 *ict, int sbxs)
{
	u32 tict[8];
	u64 li0, li1, li2, li3;
	
	TKGDI_BlitUpdate_EncodeCell4x4x2(ics+0, tict+0, tict+4, sbxs);
	TKGDI_BlitUpdate_EncodeCell4x4x2(ics+4, tict+1, tict+5, sbxs);
	ics+=4*sbxs;
	TKGDI_BlitUpdate_EncodeCell4x4x2(ics+0, tict+2, tict+6, sbxs);
	TKGDI_BlitUpdate_EncodeCell4x4x2(ics+4, tict+3, tict+7, sbxs);
	
//	li0=(((u64)tict[1])<<32)|tict[0];
//	li1=(((u64)tict[3])<<32)|tict[2];
//	li2=(((u64)tict[5])<<32)|tict[4];
//	li3=(((u64)tict[7])<<32)|tict[6];

//	li0=(((u64)tict[3])<<32)|tict[2];
//	li1=(((u64)tict[1])<<32)|tict[0];
//	li2=(((u64)tict[7])<<32)|tict[6];
//	li3=(((u64)tict[5])<<32)|tict[4];

	li0=(((u64)tict[2])<<32)|tict[3];
	li1=(((u64)tict[0])<<32)|tict[1];
	li2=(((u64)tict[6])<<32)|tict[7];
	li3=(((u64)tict[4])<<32)|tict[5];
	
	((u64 *)ict)[0]=li0;
	((u64 *)ict)[1]=li1;
	((u64 *)ict)[2]=li2;
	((u64 *)ict)[3]=li3;
}

void TKGDI_BlitUpdate_ScanCellEncode128(u16 *ics, u32 *ict,
	int blkn, int sbxs)
{
	int i, j, k;
	
	for(i=0; i<blkn; i++)
	{
		TKGDI_BlitUpdate_EncodeCell8x8x1(ics, ict, sbxs);
		ics+=8;
		ict+=4;
	}
}

void TKGDI_BlitUpdate_ScanCellEncode256(u16 *ics, u32 *ict,
	int blkn, int sbxs)
{
	int i, j, k;
	
	for(i=0; i<blkn; i++)
	{
//		TKGDI_BlitUpdate_EncodeCell8x8x1(ics, ict, sbxs);
		TKGDI_BlitUpdate_EncodeCell8x8x2(ics, ict, sbxs);
		ics+=8;
		ict+=8;
	}
}


void TKGDI_BlitUpdate_ScanCellEncode128Index(byte *ics, u32 *ict,
	int blkn, int sbxs, u16 *pal, int bc)
{
	byte *ics1;
	int i, j, k, step;
	
	step=(8*bc)>>3;
	ics1=(byte *)ics;
	for(i=0; i<blkn; i++)
	{
		TKGDI_BlitUpdate_EncodeCell8x8x1Index(ics1, ict, sbxs, pal, bc);
		ics1+=step;
		ict+=4;
	}
}

void TKGDI_BlitUpdate_ScanCellEncode256Index(byte *ics, u32 *ict,
	int blkn, int sbxs, u16 *pal, int bc)
{
	byte *ics1;
	int i, j, k, step;
	
	step=(8*bc)>>3;
	ics1=(byte *)ics;
	for(i=0; i<blkn; i++)
	{
//		TKGDI_BlitUpdate_EncodeCell8x8x1Index(ics1, ict, sbxs, pal, bc);
		TKGDI_BlitUpdate_EncodeCell8x8x2Index(ics1, ict, sbxs, pal, bc);
		ics1+=step;
		ict+=8;
	}
}

u32 __int32_bswap(u32 b);

void TKGDI_BlitUpdate_ScanCellTransUTX2(u64 *ics, u32 *ict,
	int blkn, int sbxs)
{
	u64 *cs0, *cs1, *ct;
	u64 b0, b1, b2, b3;
	u64 c0, c1, c2, c3;
	u32 vi0, vi1, vi2, vi3, pi0, pi1, pi2, pi3;
	int i, j, k;
	
	cs0=ics;
	cs1=ics+((sbxs+3)>>2);
	ct=(u64 *)ict;
	
	for(i=0; i<blkn; i++)
	{
//		b0=cs0[0];	b1=cs0[1];
//		b2=cs1[0];	b3=cs1[1];

		b3=cs0[0];	b2=cs0[1];
		b1=cs1[0];	b0=cs1[1];
		cs0+=2;		cs1+=2;
		vi0=0xC0000000U|((b0>>1)&0x3FFF8000)|(b0&0x00007FFF);
		vi1=0xC0000000U|((b1>>1)&0x3FFF8000)|(b1&0x00007FFF);
		vi2=0xC0000000U|((b2>>1)&0x3FFF8000)|(b2&0x00007FFF);
		vi3=0xC0000000U|((b3>>1)&0x3FFF8000)|(b3&0x00007FFF);

		pi0=b0>>32;		pi1=b1>>32;
		pi2=b2>>32;		pi3=b3>>32;
		if(sbxs<0)
		{
			pi0=__int32_bswap(pi0);
			pi1=__int32_bswap(pi1);
			pi2=__int32_bswap(pi2);
			pi3=__int32_bswap(pi3);
		}

		c0=(((u64)vi1)<<32)|vi0;
		c1=(((u64)vi3)<<32)|vi2;
		c2=(((u64)pi1)<<32)|pi0;
		c3=(((u64)pi3)<<32)|pi2;
		ct[0]=c0;	ct[1]=c1;
		ct[2]=c2;	ct[3]=c3;
		ct+=4;
	}
}

byte tkgdi_blitupdate_rgb555to222(u16 rgb)
{
	int cr, cg, cb;
	int v;

	cr=(rgb>>10)&31;
	cg=(rgb>> 5)&31;
	cb=(rgb>> 0)&31;
	cr=cr>>3;
	cg=cg>>3;
	cb=cb>>3;

//	cr=(cr+4)>>3;
//	cg=(cg+4)>>3;
//	cb=(cb+4)>>3;
//	if(cr>3)cr=3;
//	if(cg>3)cg=3;
//	if(cb>3)cb=3;

	v=(cr<<4)|(cg<<2)|cb;
	return(v);
}

void tkgdi_blitupdate_initC3Y3(byte *lut)
{
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int dr, dg, db, d;
	int ch, cl, bi, bd;
	int i, j, k, v;

//	for(i=0; i<512; i++)
	for(i=0; i<1024; i++)
	{
//		cr0=(i>>6)&7;
//		cg0=(i>>3)&7;
//		cb0=(i>>0)&7;
//		cr0=(cr0<<2)|(cr0>>1);
//		cg0=(cg0<<2)|(cg0>>1);
//		cb0=(cb0<<2)|(cb0>>1);

		cr0=(i>>7)& 7;
		cg0=(i>>3)&15;
		cb0=(i>>0)& 7;
		cr0=(cr0<<2)|(cg0>>2);
		cg0=(cg0<<1)|(cg0>>3);
		cb0=(cb0<<2)|(cg0>>2);
		
		bi=63; bd=999999;
		for(j=0; j<64; j++)
		{
			ch=(j&7)*3+10;
//			ch=((j&7)<<2)|((j>>1)&3);
//			ch=16|((j&7)<<1)|((j&7)>>2);
			cl=ch>>2;

			if(!(j&0x38))
			{
//				cl=ch-2;
//				cl=((j&7)<<1)|((j&7)>>2);
//				if(cl<0)cl=0;
			}

			cr1=(j&0x20)?ch:cl;
			cg1=(j&0x10)?ch:cl;
			cb1=(j&0x08)?ch:cl;
			
			dr=cr0-cr1;
			dg=cg0-cg1;
			db=cb0-cb1;
			dr^=(dr>>31);
			dg^=(dg>>31);
			db^=(db>>31);
			d=dr+dg+db;
			
			if(d<bd)
				{ bi=j; bd=d; }
		}
		lut[i]=bi;
	}
}

byte tkgdi_blitupdate_rgb555toC3Y3(u16 rgb)
{
	static byte *lut;
	int cr, cg, cb;
	int i, j, v;

	if(!lut)
	{
//		lut=tk_malloc_krn(512);
		lut=tk_malloc_krn(1024);
		tkgdi_blitupdate_initC3Y3(lut);
	}

#ifdef __BGBCC__
//	v=0;
//	v[9:7]=rgb[14:12];
//	v[6:3]=rgb[ 9: 6];
//	v[2:0]=rgb[ 4: 2];
	v=(_UBitInt(10)) { rgb[14:12], rgb[ 9: 6], rgb[ 4: 2] };
//	__debugbreak();
#else
//	cr=(rgb>>12)&7;
//	cg=(rgb>> 7)&7;
//	cb=(rgb>> 2)&7;
//	v=(cr<<6)|(cg<<3)|cb;

	cr=(rgb>>12)&7;
	cg=(rgb>> 6)&15;
	cb=(rgb>> 2)&7;
	v=(cr<<7)|(cg<<3)|cb;
#endif

	v=lut[v];
	return(v);
}

byte TKGDI_PixRGB555ToLuma(u16 pix)
{
	return(((pix>>3)&127)+((pix>>8)&127));
}

void TKGDI_BlitUpdate_ScanCellTransUTX2_Mask(
	u64 *ics, u32 *ict, byte *bmcs,
	int blkn, int sbxs)
{
#if 0
	static const byte p4x2_to_4x1[256]={
		0x0, 0x0, 0x1, 0x1,	0x0, 0x0, 0x1, 0x1,		//00
		0x2, 0x2, 0x3, 0x3,	0x2, 0x2, 0x3, 0x3,		//08
		0x0, 0x0, 0x1, 0x1,	0x0, 0x0, 0x1, 0x1,		//10
		0x2, 0x2, 0x3, 0x3,	0x2, 0x2, 0x3, 0x3,		//18
		0x4, 0x4, 0x5, 0x5,	0x4, 0x4, 0x5, 0x5,		//20
		0x6, 0x6, 0x7, 0x7,	0x6, 0x6, 0x7, 0x7,		//28
		0x4, 0x4, 0x5, 0x5,	0x4, 0x4, 0x5, 0x5,		//30
		0x6, 0x6, 0x7, 0x7,	0x6, 0x6, 0x7, 0x7,		//38
		0x0, 0x0, 0x1, 0x1,	0x0, 0x0, 0x1, 0x1,		//40
		0x2, 0x2, 0x3, 0x3,	0x2, 0x2, 0x3, 0x3,		//48
		0x0, 0x0, 0x1, 0x1,	0x0, 0x0, 0x1, 0x1,		//50
		0x2, 0x2, 0x3, 0x3,	0x2, 0x2, 0x3, 0x3,		//58
		0x4, 0x4, 0x5, 0x5,	0x4, 0x4, 0x5, 0x5,		//60
		0x6, 0x6, 0x7, 0x7,	0x6, 0x6, 0x7, 0x7,		//68
		0x4, 0x4, 0x5, 0x5,	0x4, 0x4, 0x5, 0x5,		//70
		0x6, 0x6, 0x7, 0x7,	0x6, 0x6, 0x7, 0x7,		//78
		0x8, 0x8, 0x9, 0x9,	0x8, 0x8, 0x9, 0x9,		//80
		0xA, 0xA, 0xB, 0xB,	0xA, 0xA, 0xB, 0xB,		//88
		0x8, 0x8, 0x9, 0x9,	0x8, 0x8, 0x9, 0x9,		//90
		0xA, 0xA, 0xB, 0xB,	0xA, 0xA, 0xB, 0xB,		//98
		0xC, 0xC, 0xD, 0xD,	0xC, 0xC, 0xD, 0xD,		//A0
		0xE, 0xE, 0xF, 0xF,	0xE, 0xE, 0xF, 0xF,		//A8
		0xC, 0xC, 0xD, 0xD,	0xC, 0xC, 0xD, 0xD,		//B0
		0xE, 0xE, 0xF, 0xF,	0xE, 0xE, 0xF, 0xF,		//B8
		0x8, 0x8, 0x9, 0x9,	0x8, 0x8, 0x9, 0x9,		//C0
		0xA, 0xA, 0xB, 0xB,	0xA, 0xA, 0xB, 0xB,		//C8
		0x8, 0x8, 0x9, 0x9,	0x8, 0x8, 0x9, 0x9,		//D0
		0xA, 0xA, 0xB, 0xB,	0xA, 0xA, 0xB, 0xB,		//D8
		0xC, 0xC, 0xD, 0xD,	0xC, 0xC, 0xD, 0xD,		//E0
		0xE, 0xE, 0xF, 0xF,	0xE, 0xE, 0xF, 0xF,		//E8
		0xC, 0xC, 0xD, 0xD,	0xC, 0xC, 0xD, 0xD,		//F0
		0xE, 0xE, 0xF, 0xF,	0xE, 0xE, 0xF, 0xF,		//F8
		};
#endif

	static byte p4x2_to_4x1[256];
	byte pxby[16];

	u64 *cs0, *cs1, *ct;
	u64 b0, b1, b2, b3;
	u64 c0, c1, c2, c3, px;
	u32 vi0, vi1, vi2, vi3, pi0, pi1, pi2, pi3;
	u32 i0, i1, i2, i3;
//	int step8, step1;
	int i, j, k, b;
	
	cs0=ics;
	cs1=ics+((sbxs+3)>>2);
	ct=(u64 *)ict;
	
//	step1=tkgdi_vid_cellstride>>1;
//	step8=step1*8;
	
	if(tkgdi_vid_cellstride==4)
	{
		if(p4x2_to_4x1[255]!=15)
		{
			for(i=0; i<256; i++)
			{
				j=0;
//				if(i&0x02)	j|=1;
//				if(i&0x08)	j|=2;
//				if(i&0x20)	j|=4;
//				if(i&0x80)	j|=8;

				if(i&0x02)	j|=8;
				if(i&0x08)	j|=4;
				if(i&0x20)	j|=2;
				if(i&0x80)	j|=1;
				p4x2_to_4x1[i]=j;
			}
		}
	
		for(i=0; i<blkn; i++)
		{
			b=bmcs[i>>3];
			if(!b && !(i&7))
			{
				i+=7;
				cs0+=16;
				cs1+=16;
				ct+=16;
				continue;
			}

			if(!(b&(1<<(i&7))))
			{		
				cs0+=2;		cs1+=2;
				ct+=2;
				continue;
			}

	//		b0=cs0[0];	b1=cs0[1];
	//		b2=cs1[0];	b3=cs1[1];

			b3=cs0[0];	b2=cs0[1];
			b1=cs1[0];	b0=cs1[1];
			cs0+=2;		cs1+=2;
			
			pxby[0]=TKGDI_PixRGB555ToLuma(b0>> 0);
			pxby[1]=TKGDI_PixRGB555ToLuma(b0>>16);
			pxby[2]=TKGDI_PixRGB555ToLuma(b1>> 0);
			pxby[3]=TKGDI_PixRGB555ToLuma(b1>>16);
			pxby[4]=TKGDI_PixRGB555ToLuma(b2>> 0);
			pxby[5]=TKGDI_PixRGB555ToLuma(b2>>16);
			pxby[6]=TKGDI_PixRGB555ToLuma(b3>> 0);
			pxby[7]=TKGDI_PixRGB555ToLuma(b3>>16);
			
			pxby[ 8]=(pxby[0]+pxby[1])>>1;
			pxby[ 9]=(pxby[2]+pxby[3])>>1;
			pxby[10]=(pxby[4]+pxby[5])>>1;
			pxby[11]=(pxby[6]+pxby[7])>>1;

			pxby[12]=(pxby[8]+pxby[9]+pxby[10]+pxby[11])>>2;
			
			k=1;
			if(pxby[12]>pxby[0])	k=0;
			if(pxby[12]<pxby[1])	k=0;
			if(pxby[12]>pxby[2])	k=0;
			if(pxby[12]<pxby[3])	k=0;
			if(pxby[12]>pxby[4])	k=0;
			if(pxby[12]<pxby[5])	k=0;
			if(pxby[12]>pxby[6])	k=0;
			if(pxby[12]<pxby[7])	k=0;
			
//			vi0=0xC0000000U|((b0>>1)&0x3FFF8000)|(b0&0x00007FFF);
//			vi1=0xC0000000U|((b1>>1)&0x3FFF8000)|(b1&0x00007FFF);
//			vi2=0xC0000000U|((b2>>1)&0x3FFF8000)|(b2&0x00007FFF);
//			vi3=0xC0000000U|((b3>>1)&0x3FFF8000)|(b3&0x00007FFF);

			if(k)
			{
				vi0=0x80000000U|((b0>>1)&0x3FFF8000)|(b0&0x00007FFF);
				vi1=0;
			}else
			{
#if 1
	//			i0=((b0>>11)&0x30)|((b0>> 6)&0x0C)|((b0>> 3)&0x03);
	//			i1=((b0>>27)&0x30)|((b0>>22)&0x0C)|((b0>>19)&0x03);
	//			i2=((b1>>11)&0x30)|((b1>> 6)&0x0C)|((b1>> 3)&0x03);
	//			i3=((b1>>27)&0x30)|((b1>>22)&0x0C)|((b1>>19)&0x03);
				i0=tkgdi_blitupdate_rgb555to222(b0>> 0);
				i1=tkgdi_blitupdate_rgb555to222(b0>>16);
				i2=tkgdi_blitupdate_rgb555to222(b1>> 0);
				i3=tkgdi_blitupdate_rgb555to222(b1>>16);
				vi0=0x80000000U|(i3<<22)|(i2<<16)|(i1<<6)|(i0<<0);

	//			i0=((b2>>11)&0x30)|((b2>> 6)&0x0C)|((b2>> 3)&0x03);
	//			i1=((b2>>27)&0x30)|((b2>>22)&0x0C)|((b2>>19)&0x03);
	//			i2=((b3>>11)&0x30)|((b3>> 6)&0x0C)|((b3>> 3)&0x03);
	//			i3=((b3>>27)&0x30)|((b3>>22)&0x0C)|((b3>>19)&0x03);
				i0=tkgdi_blitupdate_rgb555to222(b2>> 0);
				i1=tkgdi_blitupdate_rgb555to222(b2>>16);
				i2=tkgdi_blitupdate_rgb555to222(b3>> 0);
				i3=tkgdi_blitupdate_rgb555to222(b3>>16);
				vi1=0x40000000U|(i3<<22)|(i2<<16)|(i1<<6)|(i0<<0);
#endif
			}

			pi0=b0>>32;		pi1=b1>>32;
			pi2=b2>>32;		pi3=b3>>32;
			if(sbxs<0)
			{
				pi0=__int32_bswap(pi0);
				pi1=__int32_bswap(pi1);
				pi2=__int32_bswap(pi2);
				pi3=__int32_bswap(pi3);
			}

			px=0;
			i0=p4x2_to_4x1[(pi2>>24)&255];
			i1=p4x2_to_4x1[(pi3>>24)&255];
			i2=p4x2_to_4x1[(pi2>>16)&255];
			i3=p4x2_to_4x1[(pi3>>16)&255];
//			px=(px<<16)|(i0<<12)|(i1<<8)|(i2<<4)|i3;
			px=(px<<16)|(i1<<12)|(i0<<8)|(i3<<4)|i2;
			i0=p4x2_to_4x1[(pi2>> 8)&255];
			i1=p4x2_to_4x1[(pi3>> 8)&255];
			i2=p4x2_to_4x1[(pi2>> 0)&255];
			i3=p4x2_to_4x1[(pi3>> 0)&255];
//			px=(px<<16)|(i0<<12)|(i1<<8)|(i2<<4)|i3;
			px=(px<<16)|(i1<<12)|(i0<<8)|(i3<<4)|i2;

			i0=p4x2_to_4x1[(pi0>>24)&255];
			i1=p4x2_to_4x1[(pi1>>24)&255];
			i2=p4x2_to_4x1[(pi0>>16)&255];
			i3=p4x2_to_4x1[(pi1>>16)&255];
//			px=(px<<16)|(i0<<12)|(i1<<8)|(i2<<4)|i3;
			px=(px<<16)|(i1<<12)|(i0<<8)|(i3<<4)|i2;
			i0=p4x2_to_4x1[(pi0>> 8)&255];
			i1=p4x2_to_4x1[(pi1>> 8)&255];
			i2=p4x2_to_4x1[(pi0>> 0)&255];
			i3=p4x2_to_4x1[(pi1>> 0)&255];
//			px=(px<<16)|(i0<<12)|(i1<<8)|(i2<<4)|i3;
			px=(px<<16)|(i1<<12)|(i0<<8)|(i3<<4)|i2;

			c0=(((u64)vi1)<<32)|vi0;
//			c1=(((u64)vi3)<<32)|vi2;
//			c2=(((u64)pi1)<<32)|pi0;
//			c3=(((u64)pi3)<<32)|pi2;
			ct[0]=c0;	ct[1]=px;
//			ct[2]=c2;	ct[3]=c3;
			ct+=2;
		}
		return;
	}
	
	for(i=0; i<blkn; i++)
	{
		b=bmcs[i>>3];
		if(!b && !(i&7))
		{
			i+=7;
			cs0+=16;
			cs1+=16;
			ct+=32;
			continue;
		}

		if(!(b&(1<<(i&7))))
		{		
			cs0+=2;		cs1+=2;
			ct+=4;
			continue;
		}

//		b0=cs0[0];	b1=cs0[1];
//		b2=cs1[0];	b3=cs1[1];

		b3=cs0[0];	b2=cs0[1];
		b1=cs1[0];	b0=cs1[1];
		cs0+=2;		cs1+=2;
		vi0=0xC0000000U|((b0>>1)&0x3FFF8000)|(b0&0x00007FFF);
		vi1=0xC0000000U|((b1>>1)&0x3FFF8000)|(b1&0x00007FFF);
		vi2=0xC0000000U|((b2>>1)&0x3FFF8000)|(b2&0x00007FFF);
		vi3=0xC0000000U|((b3>>1)&0x3FFF8000)|(b3&0x00007FFF);

		pi0=b0>>32;		pi1=b1>>32;
		pi2=b2>>32;		pi3=b3>>32;
		if(sbxs<0)
		{
			pi0=__int32_bswap(pi0);
			pi1=__int32_bswap(pi1);
			pi2=__int32_bswap(pi2);
			pi3=__int32_bswap(pi3);
		}

		c0=(((u64)vi1)<<32)|vi0;
		c1=(((u64)vi3)<<32)|vi2;
		c2=(((u64)pi1)<<32)|pi0;
		c3=(((u64)pi3)<<32)|pi2;
		ct[0]=c0;	ct[1]=c1;
		ct[2]=c2;	ct[3]=c3;
		ct+=4;
	}
}

u32 *TKGDI_BlitUpdate_GetConbuf()
{
	u32 *conbufa, *conbufb, *conbufb2;
	u32 magref;

//	conbufa=(u32 *)0xA00A0000;
	conbufa=(u32 *)0xFFFFF00A0000ULL;
//	conbufb=conbufa+(80*61);

	if(tkgdi_blitupdate_getconbuf_sticky_cnt<=0)
	{
		conbufa=(u32 *)0xFFFFF00A0000ULL;
		if(tkgdi_blitupdate_getconbuf_sticky)
			conbufa=(u32 *)0xC00020A00000ULL;	//RAM backed framebuffer
		return(conbufa);
	}

	tkgdi_blitupdate_getconbuf_sticky_cnt--;

//	if(tkgdi_vid_planar)
//		return(conbufa);

//	conbufb=(u32 *)0x0000080A0000ULL;
//	conbufb =(u32 *)0xC000200A0000ULL;		//RAM backed framebuffer
//	conbufb2=(u32 *)0xD000200A0000ULL;		//Volatile / No Cache

	conbufb =(u32 *)0xC00020A00000ULL;		//RAM backed framebuffer
	conbufb2=(u32 *)0xD00020A00000ULL;		//Volatile / No Cache
	
	magref=0xDECA7E57;

//	((u32 *)0xFFFFF00BFF00ULL)[8]=tkgdi_vid_frnum;
//	tkgdi_vid_frnum++;

	conbufa[0]=magref;
	conbufb2[8208]=~magref;
	conbufa[4096]=magref;
	conbufa[8192]=magref;
	conbufa[1111]=conbufb2[8192];
	conbufb2[1113]=conbufa[4112];

	if(conbufb2[0]==magref)		//Detect if MMIO maps here.
	{
		tkgdi_blitupdate_getconbuf_sticky=1;
		conbufa=conbufb;
	}
	if(conbufa[8208]==~magref)		//Detect if MMIO maps here.
	{
		tkgdi_blitupdate_getconbuf_sticky=1;
		conbufa=conbufb;
	}

	if(!tkgdi_blitupdate_getconbuf_sticky_cnt)
	{
		tk_printf("TKGDI_BlitUpdate_GetConbuf: RAM_Backed=%d\n",
			tkgdi_blitupdate_getconbuf_sticky);
	}

	return(conbufa);
}

int tkgdi_blitupdate_conflip;

u32 *TKGDI_BlitUpdate_GetConbufFast()
{
	u32 *conbufa, *conbufb, *conbufb2;

	if(tkgdi_blitupdate_getconbuf_sticky)
	{
		conbufb =(u32 *)0xC00020A00000ULL;		//RAM backed framebuffer
		
		if(!tkgdi_blitupdate_conflip)
			conbufb =(u32 *)0xC00020A20000ULL;		//RAM backed framebuffer
		return(conbufb);
	}
	return(NULL);
}

void TKGDI_BlitUpdate_FlipConbuf()
{
	tkgdi_blitupdate_conflip=!tkgdi_blitupdate_conflip;
	
	if(tkgdi_blitupdate_conflip)
	{
		((u32 *)0xFFFFF00BFF00)[2]=0x20A20000;		//
	}else
	{
		((u32 *)0xFFFFF00BFF00)[2]=0x20A00000;		//
	}
}

/* For the 320x200 hi-color mode, linear frame buffer. */
int TKGDI_BlitUpdate_LfbRgb555(
	int dxo, int dyo, int dxs, int dys,
	u16 *sbuf,
	int sbxo, int sbyo,
	int sbxs, int sbys)
{
	u16 *conbufa;
	u16 *ict, *ict1;
	u16 *ics, *ics1, *ict1e;
	u64 v0, v1, v2, v3;
	int ofs, pox;
	int bym, byn, bxm, bxn, ismmio;
	int bx, by, flip, do4x;

	flip=1;
	if(sbys<0)
	{
		sbys=-sbys;
		flip=!flip;
	}

//	__debugbreak();

	__hint_use_egpr();

	conbufa=(u16 *)TKGDI_BlitUpdate_GetConbuf();
	
	ismmio=(((((long)conbufa)>>44)&15)==15);

	ics=sbuf+(sbyo*sbxs)+sbxo;
	ict=conbufa+((dyo*320)+dxo);

	if(flip)
	{
		ics=sbuf+((sbys-sbyo-1)*sbxs)+sbxo;
	}
	
	for(by=0; by<dys; by++)
	{
//		if(ismmio)
		if(1)
		{
			ics1=ics;
			ict1=ict;
			ofs=ict-conbufa;
			ict1e=ict+(dxs-16);
			if(ofs&3)
			{
				pox=ofs&3;
				ics1-=pox;
				ict1-=pox;
				v0=((u64 *)ics1)[0];
				v1=((u64 *)ict1)[0];
				if(pox==1)
				{
					v0=	(v1&0x000000000000FFFFULL) |
						(v0&0xFFFFFFFFFFFF0000ULL) ;
				}else if(pox==2)
				{
					v0=	(v1&0x00000000FFFFFFFFULL) |
						(v0&0xFFFFFFFF00000000ULL) ;
				}else if(pox==3)
				{
					v0=	(v1&0x0000FFFFFFFFFFFFULL) |
						(v0&0xFFFF000000000000ULL) ;
				}
				((u64 *)ict1)[0]=v0;
				ics1+=4; ict1+=4;
			}
			while(ict1<ict1e)
			{
				v0=((u64 *)ics1)[0];
				v1=((u64 *)ics1)[1];
				v2=((u64 *)ics1)[2];
				v3=((u64 *)ics1)[3];
				((u64 *)ict1)[0]=v0;
				((u64 *)ict1)[1]=v1;
				((u64 *)ict1)[2]=v2;
				((u64 *)ict1)[3]=v3;
				ics1+=16;
				ict1+=16;
			}
			ict1e=ict+dxs;
			ofs=ict1e-conbufa;
			pox=ofs&3;
			if(pox)
				{ ict1e-=pox; }
			while(ict1<ict1e)
			{
				v0=((u64 *)ics1)[0];
				((u64 *)ict1)[0]=v0;
				ics1+=4; ict1+=4;
			}
			if(pox)
			{
				pox=ofs&3;
				ics1-=pox;
				ict1-=pox;
				v0=((u64 *)ics1)[0];
				v1=((u64 *)ict1)[0];
				if(pox==1)
				{
					v0=	(v0&0x000000000000FFFFULL) |
						(v1&0xFFFFFFFFFFFF0000ULL) ;
				}else if(pox==2)
				{
					v0=	(v0&0x00000000FFFFFFFFULL) |
						(v1&0xFFFFFFFF00000000ULL) ;
				}else if(pox==3)
				{
					v0=	(v0&0x0000FFFFFFFFFFFFULL) |
						(v1&0xFFFF000000000000ULL) ;
				}
				((u64 *)ict1)[0]=v0;
				ics1+=4; ict1+=4;
			}
		}else
		{
			memcpy(ict, ics, dxs*2);
		}
		if(flip)
			{ ics-=sbxs; }
		else
			{ ics+=sbxs; }
		ict+=320;
	}

	((u32 *)0xFFFFF00BFF00ULL)[8]=tkgdi_vid_frnum;
	tkgdi_vid_frnum++;

	return(0);
}

/* For the 320x200 hi-color mode. */
int TKGDI_BlitUpdate_BlkRgb555(
	int dxo, int dyo, int dxs, int dys,
	u16 *sbuf,
	int sbxo, int sbyo,
	int sbxs, int sbys)
{
	u32 *conbufa, *conbufb, *conbufb2;
	u32 *ict;
	u16 *ics;
	int bym, byn, bxm, bxn;
	int bx, by, flip, do4x;

	flip=1;
	if(sbys<0)
	{
		sbys=-sbys;
		flip=!flip;
	}

//	__debugbreak();

	__hint_use_egpr();

	conbufa=TKGDI_BlitUpdate_GetConbuf();

	bxm=dxo>>2;
	bxn=(dxo+dxs)>>2;
	bym=dyo>>2;
	byn=(dyo+dys)>>2;

	ics=sbuf+(sbyo*sbxs)+sbxo;
//	ict=conbufa+((bym*80+bxm)*8);
	ict=conbufa+((bym*tkgdi_vid_rowstride)+(bxm*tkgdi_vid_cellstride));
//	ict=conbufa;

	if(flip)
	{
//		ics+=((dys>>4)-1)*(4*sbxs);
//		ics+=(sbys-1)*sbxs;
		ics=sbuf+((sbys-sbyo-1)*sbxs)+sbxo;

		if(sbxs==320)
		{
			ics-=3*320;
		}
	}
	
	for(by=bym; by<byn; by++)
	{
		if(flip)
		{
			if((sbxs==320) && !(tkgdi_vid_planar))
				TKGDI_BlitUpdate_ScanCopy_Flip(ics, ict, dxs>>2);
			else
				TKGDI_BlitUpdate_ScanCopyGen(ics, ict, dxs>>2, -sbxs);
			ics-=4*sbxs;
		}else
		{
			if((sbxs==320) && !(tkgdi_vid_planar))
				TKGDI_BlitUpdate_ScanCopy(ics, ict, dxs>>2);
			else
				TKGDI_BlitUpdate_ScanCopyGen(ics, ict, dxs>>2, sbxs);
			ics+=4*sbxs;
		}
//		ict+=80*8;
		ict+=tkgdi_vid_rowstride;
	}

	((u32 *)0xFFFFF00BFF00ULL)[8]=tkgdi_vid_frnum;
	tkgdi_vid_frnum++;

	return(0);
}

int TKGDI_BlitUpdate_BlkRgb555_Mask(
	int dxo, int dyo, int dxs, int dys,
	u16 *sbuf, byte *bmbuf,
	int sbxo, int sbyo,
	int sbxs, int sbys)
{
	u32 *conbufa, *conbufb, *conbufb2;
	u32 *ict;
	u16 *ics;
	byte *bmcs, *bmo;
	int bym, byn, bxm, bxn;
	int bx, by, flip;
	int bmix, bmxs, bmxs2, bmx, bmx0, bmx1;
	int do4x, rowstr, cellstr, ictshl, ystep;
	int bxshl, byshl;
	int i, j, k;

	flip=1;
	if(sbys<0)
	{
		sbys=-sbys;
		flip=!flip;
	}

//	__debugbreak();

	__hint_use_egpr();

	conbufa=TKGDI_BlitUpdate_GetConbuf();

	ystep=4*sbxs;
	do4x=0;
	rowstr=tkgdi_vid_rowstride;
	cellstr=tkgdi_vid_cellstride;
	ictshl=2;
	if(tkgdi_vid_is8bit)
		ictshl=1;

	bxshl=2;
	byshl=2;

	if(tkgdi_vid_is1bit || tkgdi_vid_is2bit_cc)
	{
		ystep=8*sbxs;
		bxshl=3;
		byshl=3;

		if(tkgdi_vid_is1bit)
			ictshl=1;
	}

	if(tkgdi_vid_planar)
	{
		if(((((long)conbufa)>>44)&15)!=0xF)
		{
			do4x=1;
			rowstr=tkgdi_vid_rowstride<<2;
			cellstr=tkgdi_vid_cellstride<<2;
			ictshl=4;

			if(tkgdi_vid_is8bit)
				ictshl=3;

		}
	}


	bxm=dxo>>bxshl;
	bxn=(dxo+dxs)>>bxshl;
	bym=dyo>>bxshl;
	byn=(dyo+dys)>>bxshl;
	
	bmx=((sbxs+7)>>3);
	bmxs=((bmx+7)>>3);
	bmix=0;

	bmxs2=bmxs<<3;

	bmo=bmbuf;

	ics=sbuf+(sbyo*sbxs)+sbxo;
//	ict=conbufa+((bym*80+bxm)*8);
//	ict=conbufa+((bym*tkgdi_vid_rowstride)+(bxm*tkgdi_vid_cellstride));
	ict=conbufa+((bym*rowstr)+(bxm*cellstr));
//	ict=conbufa;

	if(flip)
	{
//		ics+=((dys>>4)-1)*(4*sbxs);
//		ics+=(sbys-1)*sbxs;
		ics=sbuf+((sbys-sbyo-1)*sbxs)+sbxo;

		bmo=bmbuf+((((sbys-sbyo)>>1)-1)*bmxs)+(sbxo>>4);
		bmxs-=bmxs;

		if(sbxs==320)
		{
			ics-=3*320;
		}
	}
	
//	tk_printf("TKGDI_BlitUpdate_BlkRgb555_Mask: %d %d %d %d\n",
//		sbxs, sbys, bmx, bmxs);
	
	for(by=bym; by<byn; by++)
	{
//		bmix=(by>>1)*bmxs2;
		bmix=(by>>(3-bxshl))*bmxs2;
//		bmcs=bmo+((by>>1)*bmxs);
		
		bmx0=9999;
		bmx1=-1;

		for(i=0; i<bmx; i++)
		{
			j=bmix+i;
		
//			k=bmcs[i>>3];
			k=bmbuf[j>>3];
			if(!k)
			{
				i+=7;
				continue;
			}
			if(k&(1<<(i&7)))
				{ bmx0=i; break; }
		}

		for(i=bmx-1; i>0; i--)
		{
			j=bmix+i;
		
//			k=bmcs[i>>3];
			k=bmbuf[j>>3];
			if(!k)
			{
				i=i&(~7);
				continue;
			}
			if(k&(1<<(i&7)))
				{ bmx1=i; break; }
		}
		
		if(bmx1<bmx0)
		{
			if(flip)
				{ ics-=ystep; }
			else
				{ ics+=ystep; }
//			ict+=tkgdi_vid_rowstride;
			ict+=rowstr;
			continue;
		}
			
//		tk_printf(" %d %d\n", bmx0, bmx1);
	
		if(flip)
		{
			if((sbxs==320) && !(tkgdi_vid_planar))
				TKGDI_BlitUpdate_ScanCopy_Flip(ics, ict, dxs>>2);
			else
			{
//				TKGDI_BlitUpdate_ScanCopyGen(ics, ict, dxs>>2, -sbxs);
				TKGDI_BlitUpdate_ScanCopyGen(
					ics+(bmx0<<3),
					ict+(bmx0<<ictshl), ((bmx1-bmx0)+1)<<(3-bxshl),
					-sbxs);
			}
			ics-=ystep;
		}else
		{
			if((sbxs==320) && !(tkgdi_vid_planar))
				TKGDI_BlitUpdate_ScanCopy(ics, ict, dxs>>2);
			else
			{
//				TKGDI_BlitUpdate_ScanCopyGen(ics, ict, dxs>>2, sbxs);
				TKGDI_BlitUpdate_ScanCopyGen(
					ics+(bmx0<<3),
					ict+(bmx0<<ictshl), ((bmx1-bmx0)+1)<<(3-bxshl),
					sbxs);
			}
			ics+=ystep;
		}

//		ict[0]=tkgdi_vid_frnum | (tkgdi_vid_frnum<<8);
	
//		ict+=80*8;
//		ict+=tkgdi_vid_rowstride;
		ict+=rowstr;
	}

	((u32 *)0xFFFFF00BFF00ULL)[8]=tkgdi_vid_frnum;
	tkgdi_vid_frnum++;

	return(0);
}

#ifdef __BJX2__
u16 TKGDI_BlitUpdate_Repack32to16(u32 pix);
__asm {
TKGDI_BlitUpdate_Repack32to16:
//	RGB32UPCK64	R4, R3
//	RGB5PCK64	R3, R2
	RGB5PCK32	R4, R2
	RTS
};
#else
u16 TKGDI_BlitUpdate_Repack32to16(u32 pix)
{
	int cr, cg, cb, p;
	
	cr=(pix>>16)&0xFF;
	cg=(pix>> 8)&0xFF;
	cb=(pix>> 0)&0xFF;
	cr>>=3;	cg>>=3;	cb>>=3;
	p=(cr<<10)|(cg<<5)|cb;
	return(p);
}
#endif


byte tkgdi_blitupdate_fp8u2ldr5[256] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//00..07
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//08..0F
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//10..17
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//18..1F
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,	//20..27
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,	//28..2F
0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,	//30..37
0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,	//38..3F
0x04,0x04,0x04,0x04,0x05,0x05,0x05,0x05,	//40..47
0x06,0x06,0x06,0x06,0x07,0x07,0x07,0x07,	//48..4F
0x08,0x08,0x09,0x09,0x0A,0x0A,0x0B,0x0B,	//50..57
0x0C,0x0C,0x0D,0x0D,0x0E,0x0E,0x0F,0x0F,	//58..5F
0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,	//60..67
0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,	//68..6F
0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,	//70..77
0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,	//78..7F
0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,	//80..87
0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,	//88..8F
0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,	//90..97
0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,	//98..9F
0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,	//A0..A7
0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,	//A8..AF
0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,	//B0..B7
0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,	//B8..BF
0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,	//C0..C7
0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,	//C8..CF
0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,	//D0..D7
0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,	//D8..DF
0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,	//E0..E7
0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,	//E8..EF
0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,	//F0..F7
0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,	//F8..FF
};

u16 TKGDI_BlitUpdate_Repack32to16H(u32 pix)
{
	int cr, cg, cb, p;
	
	cr=(pix>>16)&0xFF;
	cg=(pix>> 8)&0xFF;
	cb=(pix>> 0)&0xFF;
	cr=tkgdi_blitupdate_fp8u2ldr5[cr];
	cg=tkgdi_blitupdate_fp8u2ldr5[cg];
	cb=tkgdi_blitupdate_fp8u2ldr5[cb];
	p=(cr<<10)|(cg<<5)|cb;
	return(p);
}

/* For the 320x200 hi-color mode. */
int TKGDI_BlitUpdate_BlkIndex8(
	int dxo, int dyo, int dxs, int dys,
	u16 *sbuf, u32 *pal,
	int sbxo, int sbyo,
	int sbxs, int sbys)
{
	u16 pal2[256];
	u32 *conbufa, *conbufb, *conbufb2;
	u32 *ict;
	byte *ics;
	int bym, byn, bxm, bxn;
	int bx, by, flip;
	int i, j, k;

	flip=1;
	if(sbys<0)
	{
		sbys=-sbys;
		flip=!flip;
	}

	__hint_use_egpr();

	for(i=0; i<256; i++)
		{ pal2[i]=TKGDI_BlitUpdate_Repack32to16(pal[i]); }

	conbufa=TKGDI_BlitUpdate_GetConbuf();

	bxm=dxo>>2;
	bxn=(dxo+dxs)>>2;
	bym=dyo>>2;
	byn=(dyo+dys)>>2;

	ics=(byte *)(sbuf+(sbyo*sbxs)+sbxo);
//	ict=conbufa+((bym*80+bxm)*8);
	ict=conbufa+((bym*tkgdi_vid_rowstride)+(bxm*tkgdi_vid_cellstride));

	if(flip)
	{
		ics=(byte *)(sbuf+((sbys-sbyo-1)*sbxs)+sbxo);

		if(sbxs==320)
		{
			ics-=3*320;
		}
	}
	
	for(by=bym; by<byn; by++)
	{
		if(flip)
		{
			TKGDI_BlitUpdate_ScanCopyIndex8(ics, ict, dxs>>2, -sbxs, pal2);
			ics-=4*sbxs;
		}else
		{
			TKGDI_BlitUpdate_ScanCopyIndex8(ics, ict, dxs>>2, sbxs, pal2);
			ics+=4*sbxs;
		}
		ict+=tkgdi_vid_rowstride;
	}

	((u32 *)0xFFFFF00BFF00ULL)[8]=tkgdi_vid_frnum;
	tkgdi_vid_frnum++;

	return(0);
}

/* For the 320x200 hi-color mode. */
int TKGDI_BlitUpdate_BlkIndex4(
	int dxo, int dyo, int dxs, int dys,
	u16 *sbuf, u32 *pal,
	int sbxo, int sbyo,
	int sbxs, int sbys)
{
	u16 pal2[256];
	u32 *conbufa, *conbufb, *conbufb2;
	u32 *ict;
	byte *ics;
	int bym, byn, bxm, bxn;
	int bx, by, flip, sbxh, sbxse;
	int i, j, k;

	flip=1;
	if(sbys<0)
	{
		sbys=-sbys;
		flip=!flip;
	}

	__hint_use_egpr();

	for(i=0; i<256; i++)
		{ pal2[i]=TKGDI_BlitUpdate_Repack32to16(pal[i]); }

	conbufa=TKGDI_BlitUpdate_GetConbuf();

	bxm=dxo>>2;
	bxn=(dxo+dxs)>>2;
	bym=dyo>>2;
	byn=(dyo+dys)>>2;

	sbxh=(sbxs+1)>>1;
	sbxse=sbxh<<1;

	ics=(byte *)(sbuf+(sbyo*sbxh)+(sbxo>>1));
//	ict=conbufa+((bym*80+bxm)*8);
	ict=conbufa+((bym*tkgdi_vid_rowstride)+(bxm*tkgdi_vid_cellstride));

	if(flip)
	{
		ics=(byte *)(sbuf+((((sbys-sbyo-1)*sbxse)+sbxo)>>1));

		if(sbxs==320)
		{
			ics-=(3*320)>>1;
		}
	}
	
	for(by=bym; by<byn; by++)
	{
		if(flip)
		{
			TKGDI_BlitUpdate_ScanCopyIndex4(ics, ict, dxs>>2, -sbxs, pal2);
			ics-=(4*sbxs+1)>>1;
		}else
		{
			TKGDI_BlitUpdate_ScanCopyIndex4(ics, ict, dxs>>2, sbxs, pal2);
			ics+=(4*sbxs+1)>>1;
		}
		ict+=tkgdi_vid_rowstride;
	}

	((u32 *)0xFFFFF00BFF00ULL)[8]=tkgdi_vid_frnum;
	tkgdi_vid_frnum++;

	return(0);
}

/* For the 320x200 hi-color mode. */
int TKGDI_BlitUpdate_BlkRgb888(
	int dxo, int dyo, int dxs, int dys,
	u16 *sbuf, int stride,
	int sbxo, int sbyo,
	int sbxs, int sbys)
{
	u16 pal2[256];
	u32 *conbufa, *conbufb, *conbufb2;
	u32 *ict;
	u32 *ics;
	int bym, byn, bxm, bxn;
	int bx, by, flip;
	int i, j, k;

	flip=1;
	if(sbys<0)
	{
		sbys=-sbys;
		flip=!flip;
	}

	__hint_use_egpr();

//	for(i=0; i<256; i++)
//		{ pal2[i]=TKGDI_BlitUpdate_Repack32to16(pal[i]); }

	conbufa=TKGDI_BlitUpdate_GetConbuf();

	bxm=dxo>>2;
	bxn=(dxo+dxs)>>2;
	bym=dyo>>2;
	byn=(dyo+dys)>>2;

	ics=((u32 *)sbuf)+(sbyo*sbxs)+sbxo;
//	ict=conbufa+((bym*80+bxm)*8);
	ict=conbufa+((bym*tkgdi_vid_rowstride)+(bxm*tkgdi_vid_cellstride));

	if(flip)
	{
		ics=((u32 *)sbuf)+((sbys-sbyo-1)*sbxs)+sbxo;

		if(sbxs==320)
		{
//			ics-=3*320;
		}
	}
	
	for(by=bym; by<byn; by++)
	{
		if(flip)
		{
			TKGDI_BlitUpdate_ScanCopyRGB(
				(byte *)ics, ict, dxs>>2, -sbxs, stride);
			ics-=4*sbxs;
		}else
		{
			TKGDI_BlitUpdate_ScanCopyRGB(
				(byte *)ics, ict, dxs>>2, sbxs, stride);
			ics+=4*sbxs;
		}
		ict+=tkgdi_vid_rowstride;
	}

	((u32 *)0xFFFFF00BFF00ULL)[8]=tkgdi_vid_frnum;
	tkgdi_vid_frnum++;

	return(0);
}

int TKGDI_BlitUpdate_BlkRgb888H(
	int dxo, int dyo, int dxs, int dys,
	u16 *sbuf, int stride,
	int sbxo, int sbyo,
	int sbxs, int sbys,
	u32 scbi)
{
	u16 pal2[256];
	u32 *conbufa, *conbufb, *conbufb2;
	u32 *ict;
	u32 *ics;
	int bym, byn, bxm, bxn;
	int bx, by, flip;
	int i, j, k;

	flip=1;
	if(sbys<0)
	{
		sbys=-sbys;
		flip=!flip;
	}

	__hint_use_egpr();

//	for(i=0; i<256; i++)
//		{ pal2[i]=TKGDI_BlitUpdate_Repack32to16(pal[i]); }

	conbufa=TKGDI_BlitUpdate_GetConbuf();

	bxm=dxo>>2;
	bxn=(dxo+dxs)>>2;
	bym=dyo>>2;
	byn=(dyo+dys)>>2;

	ics=((u32 *)sbuf)+(sbyo*sbxs)+sbxo;
//	ict=conbufa+((bym*80+bxm)*8);
	ict=conbufa+((bym*tkgdi_vid_rowstride)+(bxm*tkgdi_vid_cellstride));

	if(flip)
	{
		ics=((u32 *)sbuf)+((sbys-sbyo-1)*sbxs)+sbxo;

		if(sbxs==320)
		{
//			ics-=3*320;
		}
	}
	
	for(by=bym; by<byn; by++)
	{
		if(flip)
		{
			TKGDI_BlitUpdate_ScanCopyRGBH(
				(byte *)ics, ict, dxs>>2, -sbxs, stride, scbi);
			ics-=4*sbxs;
		}else
		{
			TKGDI_BlitUpdate_ScanCopyRGBH(
				(byte *)ics, ict, dxs>>2, sbxs, stride, scbi);
			ics+=4*sbxs;
		}
		ict+=tkgdi_vid_rowstride;
	}

	((u32 *)0xFFFFF00BFF00ULL)[8]=tkgdi_vid_frnum;
	tkgdi_vid_frnum++;

	return(0);
}


/* For RGB555 to Color-Cell Mode. */
int TKGDI_BlitUpdate_BlkRgb555_CCE(
	int dxo, int dyo, int dxs, int dys,
	u16 *sbuf,
	int sbxo, int sbyo,
	int sbxs, int sbys)
{
	u32 *conbufa, *conbufb, *conbufb2;
	u32 *ict;
	u16 *ics;
	int bym, byn, bxm, bxn;
	int bx, by, flip;

	__hint_use_egpr();

	flip=1;
	if(sbys<0)
	{
		sbys=-sbys;
		flip=!flip;
	}
	
//	tk_printf("TKGDI_BlitUpdate_BlkRgb555_CCE: %d %d %d %d  %d %d %d %d\n",
//		dxo, dyo, dxs, dys,
//		sbxo, sbyo, sbxs, sbys);

	TKGDI_BlitUpdate_FillEncode2Tab();

#if 0
	conbufa=(u32 *)0xFFFFF00A0000ULL;
	conbufb =(u32 *)0xC000200A0000ULL;		//RAM backed framebuffer
	conbufb2=(u32 *)0xD000200A0000ULL;		//Volatile / No Cache

//	((u32 *)0xFFFFF00BFF00ULL)[8]=tkgdi_vid_frnum;
//	tkgdi_vid_frnum++;

	conbufa[0]=tkgdi_vid_frnum;
	if(conbufb2[0]==tkgdi_vid_frnum)		//Detect if MMIO maps here.
		conbufa=conbufb;

	if(!(((long)conbufa)>>16))
		__debugbreak();
#endif

	conbufa=TKGDI_BlitUpdate_GetConbuf();

	bxm=dxo>>3;
	bxn=(dxo+dxs)>>3;
	bym=dyo>>3;
	byn=(dyo+dys)>>3;

//	tk_printf("  %d %d  %d %d\n", bxm, bxn, bym, byn);
//	tk_printf("  %p %p \n", conbufa, sbuf);

	ics=sbuf+(sbyo*sbxs)+sbxo;
//	ict=conbufa+((bym*80+bxm)*8);
	ict=conbufa+((bym*tkgdi_vid_rowstride)+(bxm*tkgdi_vid_cellstride));
	if(flip)
	{
//		ics+=((dys>>4)-1)*(4*sbxs);
//		ics+=(dys-1)*sbxs;
//		ics+=(sbys-1)*sbxs;
		ics=sbuf+((sbys-sbyo-1)*sbxs)+sbxo;
	}
	
	for(by=bym; by<byn; by++)
	{
		if(flip)
		{
			if(tkgdi_vid_cellstride>=8)
				TKGDI_BlitUpdate_ScanCellEncode256(ics, ict, dxs>>3, -sbxs);
			else
				TKGDI_BlitUpdate_ScanCellEncode128(ics, ict, dxs>>3, -sbxs);
			ics-=8*sbxs;
		}else
		{
			if(tkgdi_vid_cellstride>=8)
				TKGDI_BlitUpdate_ScanCellEncode256(ics, ict, dxs>>3, sbxs);
			else
				TKGDI_BlitUpdate_ScanCellEncode128(ics, ict, dxs>>3, sbxs);
			ics+=8*sbxs;
		}
//		ict+=80*8;
		ict+=tkgdi_vid_rowstride;
	}

	((u32 *)0xFFFFF00BFF00ULL)[8]=tkgdi_vid_frnum;
	tkgdi_vid_frnum++;
	
	return(0);
}

/* For RGB555 to Color-Cell Mode. */
int TKGDI_BlitUpdate_BlkIndex_CCE(
	int dxo, int dyo, int dxs, int dys,
	byte *sbuf, u32 *pal,
	int sbxo, int sbyo,
	int sbxs, int sbys,
	int bc)
{
	u16 pal2[256];
	u32 *conbufa, *conbufb, *conbufb2;
	u32 *ict;
	byte *ics;
	int bym, byn, bxm, bxn, ystr;
	int bx, by, flip;
	int i, j, k;

	__hint_use_egpr();

	flip=1;
	if(sbys<0)
	{
		sbys=-sbys;
		flip=!flip;
	}

	if(pal)
	{
		for(i=0; i<256; i++)
			{ pal2[i]=TKGDI_BlitUpdate_Repack32to16(pal[i]); }
	}

//	tk_printf("TKGDI_BlitUpdate_BlkRgb555_CCE: %d %d %d %d  %d %d %d %d\n",
//		dxo, dyo, dxs, dys,
//		sbxo, sbyo, sbxs, sbys);

	TKGDI_BlitUpdate_FillEncode2Tab();

	conbufa=TKGDI_BlitUpdate_GetConbuf();

	bxm=dxo>>3;
	bxn=(dxo+dxs)>>3;
	bym=dyo>>3;
	byn=(dyo+dys)>>3;

//	tk_printf("  %d %d  %d %d\n", bxm, bxn, bym, byn);
//	tk_printf("  %p %p \n", conbufa, sbuf);

	ystr=(sbxs*bc)/8;

	k=(sbyo*sbxs)+sbxo;
	k=(k*bc)/8;
	ics=sbuf+k;
//	ict=conbufa+((bym*80+bxm)*8);
	ict=conbufa+((bym*tkgdi_vid_rowstride)+(bxm*tkgdi_vid_cellstride));
	if(flip)
	{
//		ics+=((dys>>4)-1)*(4*sbxs);
//		ics+=(dys-1)*sbxs;
//		ics+=(sbys-1)*sbxs;
//		ics=sbuf+((sbys-sbyo-1)*sbxs)+sbxo;

		k=((sbys-sbyo-1)*sbxs)+sbxo;
		k=(k*bc)/8;
		ics=sbuf+k;
	}
	
	for(by=bym; by<byn; by++)
	{
		if(flip)
		{
			if(tkgdi_vid_cellstride>=8)
				TKGDI_BlitUpdate_ScanCellEncode256Index(
					ics, ict, dxs>>3, -sbxs, pal2, bc);
			else
				TKGDI_BlitUpdate_ScanCellEncode128Index(
					ics, ict, dxs>>3, -sbxs, pal2, bc);
//			ics-=8*sbxs;
			ics-=8*ystr;
		}else
		{
			if(tkgdi_vid_cellstride>=8)
				TKGDI_BlitUpdate_ScanCellEncode256Index(
					ics, ict, dxs>>3, sbxs, pal2, bc);
			else
				TKGDI_BlitUpdate_ScanCellEncode128Index(
					ics, ict, dxs>>3, sbxs, pal2, bc);
//			ics+=8*sbxs;
			ics+=8*ystr;
		}
//		ict+=80*8;
		ict+=tkgdi_vid_rowstride;
	}

	((u32 *)0xFFFFF00BFF00ULL)[8]=tkgdi_vid_frnum;
	tkgdi_vid_frnum++;
	
	return(0);
}

/* For UTX2 to Color-Cell Mode. */
int TKGDI_BlitUpdate_BlkUtx2_CCT(
	int dxo, int dyo, int dxs, int dys,
	u64 *sbuf,
	int sbxo, int sbyo,
	int sbxs, int sbys)
{
	u32 *conbufa, *conbufb, *conbufb2;
	u32 *ict;
	u64 *ics;
	int bym, byn, bxm, bxn;
	int bx, by, flip;

	flip=1;
	if(sbys<0)
	{
		sbys=-sbys;
		flip=!flip;
	}

#if 0
	conbufa=(u32 *)0xFFFFF00A0000ULL;
	conbufb =(u32 *)0xC000200A0000ULL;		//RAM backed framebuffer
	conbufb2=(u32 *)0xD000200A0000ULL;		//Volatile / No Cache

	conbufa[0]=tkgdi_vid_frnum;
	if(conbufb2[0]==tkgdi_vid_frnum)		//Detect if MMIO maps here.
		conbufa=conbufb;
#endif

	conbufa=TKGDI_BlitUpdate_GetConbuf();

	bxm=dxo>>3;
	bxn=(dxo+dxs)>>3;
	bym=dyo>>3;
	byn=(dyo+dys)>>3;

	ics=sbuf+((sbyo>>2)*(sbxs>>2))+(sbxo>>2);
//	ict=conbufa+((bym*80+bxm)*8);
	ict=conbufa+((bym*tkgdi_vid_rowstride)+(bxm*tkgdi_vid_cellstride));
	if(flip)
	{
//		ics+=((dys>>4)-1)*(sbxs>>2);
		ics+=((dys>>2)-1)*(sbxs>>2);
	}
	
	for(by=bym; by<byn; by++)
	{
		if(flip)
		{
			TKGDI_BlitUpdate_ScanCellTransUTX2(ics, ict, dxs>>3, -sbxs);
			ics-=2*(sbxs>>2);
		}else
		{
			TKGDI_BlitUpdate_ScanCellTransUTX2(ics, ict, dxs>>3, sbxs);
			ics+=2*(sbxs>>2);
		}
//		ict+=80*8;
		ict+=tkgdi_vid_rowstride;
	}
	
	((u32 *)0xFFFFF00BFF00ULL)[8]=tkgdi_vid_frnum;
	tkgdi_vid_frnum++;

	return(0);
}

/* For UTX2 to Color-Cell Mode. */
int TKGDI_BlitUpdate_BlkUtx2_CCT_Mask(
	int dxo, int dyo, int dxs, int dys,
	u64 *sbuf, byte *bmbuf,
	int sbxo, int sbyo,
	int sbxs, int sbys)
{
	u32 *conbufa, *conbufb, *conbufb2;
	u32 *ict;
	u64 *ics;
	byte *bmcs;
	int bym, byn, bxm, bxn, bmxs;
	int bx, by, flip;

	flip=1;
	if(sbys<0)
	{
		sbys=-sbys;
		flip=!flip;
	}

#if 0
	conbufa=(u32 *)0xFFFFF00A0000ULL;
	conbufb =(u32 *)0xC000200A0000ULL;		//RAM backed framebuffer
	conbufb2=(u32 *)0xD000200A0000ULL;		//Volatile / No Cache

	conbufa[0]=tkgdi_vid_frnum;
	if(conbufb2[0]==tkgdi_vid_frnum)		//Detect if MMIO maps here.
		conbufa=conbufb;
#endif

	conbufa=TKGDI_BlitUpdate_GetConbuf();

	bxm=dxo>>3;
	bxn=(dxo+dxs)>>3;
	bym=dyo>>3;
	byn=(dyo+dys)>>3;
	
	bmxs=(((dxs+7)>>3)+7)>>3;

	ics=sbuf+((sbyo>>2)*(sbxs>>2))+(sbxo>>2);
//	ict=conbufa+((bym*80+bxm)*8);
	ict=conbufa+((bym*tkgdi_vid_rowstride)+(bxm*tkgdi_vid_cellstride));
	bmcs=bmbuf;
	if(flip)
	{
//		ics+=((dys>>4)-1)*(sbxs>>2);
		ics+=((dys>>2)-1)*(sbxs>>2);
		bmcs+=((dys>>2)-1)*bmxs;
	}
	
	for(by=bym; by<byn; by++)
	{
		if(flip)
		{
			TKGDI_BlitUpdate_ScanCellTransUTX2_Mask(
				ics, ict, bmcs, dxs>>3, -sbxs);
			ics-=2*(sbxs>>2);
			bmcs-=bmxs;
		}else
		{
			TKGDI_BlitUpdate_ScanCellTransUTX2_Mask(
				ics, ict, bmcs, dxs>>3, sbxs);
			ics+=2*(sbxs>>2);
			bmcs+=bmxs;
		}
//		ict+=80*8;
		ict+=tkgdi_vid_rowstride;
	}
	
	((u32 *)0xFFFFF00BFF00ULL)[8]=tkgdi_vid_frnum;
	tkgdi_vid_frnum++;

	return(0);
}
