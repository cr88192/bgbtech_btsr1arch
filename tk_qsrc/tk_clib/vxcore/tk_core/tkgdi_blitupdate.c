#include <tk_core.h>
#include <tkgdi/tkgdi.h>

int tkgdi_vid_frnum;
int tkgdi_vid_scrmode;
int tkgdi_vid_cellstride;
int tkgdi_vid_rowstride;
byte tkgdi_vid_planar;
byte tkgdi_vid_noutx2;
byte tkgdi_vid_is8bit;

int tkgdi_vid_xsize;
int tkgdi_vid_ysize;

// int tkgdi_vid_bmxsize;

// u64 *tkgdi_vid_screenutx;		//screen UTX2 buffer
// u64 *tkgdi_vid_screenrgb;		//screen RGB buffer
// byte *tkgdi_vid_screendirty;		//screen dirtry-mask buffer

// static u64 tkgdi_ptrmangle_key=0;

byte tk_img_d9to8tab[512];
byte tk_img_d15to8tab0[32768];
byte tk_img_d15to8tab1[32768];
u16 tk_img_d8to15tab[256];
byte tk_img_d8to15ready;

int tkgdi_blitupdate_getconbuf_sticky;
int tkgdi_blitupdate_getconbuf_sticky_cnt;

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

int tk_img_rgb15dist(u16 va, u16 vb)
{
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int dr, dg, db;
	int d;
	
	cr0=(va>>10)&31;	cr1=(vb>>10)&31;
	cg0=(va>> 5)&31;	cg1=(vb>> 5)&31;
	cb0=(va>> 0)&31;	cb1=(vb>> 0)&31;
	
	dr=cr0-cr1;
	dg=cg0-cg1;
	db=cb0-cb1;
//	dr=dr^(dr>>31);
//	dg=dg^(dg>>31);
//	db=db^(db>>31);
//	d=dr+2*dg+db;

	d=dr*dr+2*dg*dg+db*db;
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
			d=img_rgb15dist(k, tk_img_d8to15tab[j]);
			if(d<bd)
				{ bi=j; bd=d; }
		}
		tk_img_d9to8tab[i]=bi;
	}
}

#if 1
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

#if 1
int tk_img_gen15to8()
{
	int i, j, k;

	for(i=0; i<32768; i++)
	{
		j=tk_img_rgb15to8norm(i);
		tk_img_d15to8tab0[i]=(j   )&0xFF;
		tk_img_d15to8tab1[i]=(j>>8)&0xFF;
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

int tk_img_SetupPal8()
{
	if(tk_img_d8to15ready)
		return(0);
	
	tk_img_d8to15ready=1;
	tk_img_genpal1();
	tk_img_gen15to8();
	
	tk_img_uploadpal(tk_img_d8to15tab);
	
	return(1);
}

u32 tk_img_Repack4xRGB555toPal8a(u64 v)
{
	u32 p0, p1, p2, p3;
	u32 v1;
	
	p0=tk_img_d15to8tab0[(v>> 0)&0x7FFF];
	p1=tk_img_d15to8tab1[(v>>16)&0x7FFF];
	p2=tk_img_d15to8tab0[(v>>32)&0x7FFF];
	p3=tk_img_d15to8tab1[(v>>48)&0x7FFF];
	
	v1=p0|(p1<<8)|(p2<<16)|(p3<<24);
	return(v1);
}

u32 tk_img_Repack4xRGB555toPal8b(u64 v)
{
	u32 p0, p1, p2, p3;
	u32 v1;
	
	p0=tk_img_d15to8tab1[(v>> 0)&0x7FFF];
	p1=tk_img_d15to8tab0[(v>>16)&0x7FFF];
	p2=tk_img_d15to8tab1[(v>>32)&0x7FFF];
	p3=tk_img_d15to8tab0[(v>>48)&0x7FFF];
	
	v1=p0|(p1<<8)|(p2<<16)|(p3<<24);
	return(v1);
}

void TKGDI_BlitUpdate_ScanCopy(u16 *ics, u32 *ict, int blkn);
void TKGDI_BlitUpdate_ScanCopy_Flip(u16 *ics, u32 *ict, int blkn);

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

		v0=tk_img_Repack4xRGB555toPal8a(v0);
		v1=tk_img_Repack4xRGB555toPal8b(v1);
		v2=tk_img_Repack4xRGB555toPal8a(v2);
		v3=tk_img_Repack4xRGB555toPal8b(v3);		
		v0=v0|(v1<<32);
		v1=v2|(v3<<32);

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
		v0=cs0[0];	v1=cs1[0];
		v2=cs2[0];	v3=cs3[0];

		v0=tk_img_Repack4xRGB555toPal8a(v0);
		v1=tk_img_Repack4xRGB555toPal8b(v1);
		v2=tk_img_Repack4xRGB555toPal8a(v2);
		v3=tk_img_Repack4xRGB555toPal8b(v3);		
		v0=v0|(v1<<32);
		v1=v2|(v3<<32);
	
		ct[0]=v0;	ct[1]=v1;
		cs0++;		cs1++;
		cs2++;		cs3++;
		ct+=2;
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
	cs0=(u32 *)(ics       );
	cs1=(u32 *)(ics+  sbxh);
	cs2=(u32 *)(ics+2*sbxh);
	cs3=(u32 *)(ics+3*sbxh);
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

void TKGDI_BlitUpdate_ScanCopyRGB(
	byte *ics, u32 *ict, int blkn, int sbxs,
	int stride)
{
	byte *cs0, *cs1, *cs2, *cs3;
	u32 cp0, cp1, cp2, cp3;
	u64 cv0, cv1, cv2, cv3;
	u64 *ct;
	int i;
	
	cs0=(u32 *)(ics              );
	cs1=(u32 *)(ics+  sbxs*stride);
	cs2=(u32 *)(ics+2*sbxs*stride);
	cs3=(u32 *)(ics+3*sbxs*stride);
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
}

void TKGDI_BlitUpdate_EncodeCell8x8x1(u16 *ics, u32 *ict, int sbxs)
{
	byte clry[64];
	u16 clrp[64];
	u16 clrm, clrn, pix;
	u64 pxb;
	int x, y, cy, mcy, ncy, acy;
	int cr0, cg0, cb0, cr1, cg1, cb1;
	int cr, cg, cb, acr, acg, acb;
	int bal;
	int i, j;

#if 1
	mcy=256; ncy=-1;
	acy=0;
	for(y=0; y<8; y++)
	{
		for(x=0; x<8; x++)
		{
			pix=ics[x];
//			cy=(pix>>2)&255;
			cy=((pix>>3)&127)+((pix>>8)&127);
			clry[(y<<3)|x]=cy;
//			clrp[(y<<3)|x]=pix;
//			acy+=cy;
			
			if(cy<mcy)
				{ clrm=pix; mcy=cy; }
			if(cy>ncy)
				{ clrn=pix; ncy=cy; }
		}
		ics+=sbxs;
	}

//	acy=acy>>6;
	acy=(mcy+ncy)>>1;
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
	for(i=0; i<64; i++)
	{
		cy=clry[i];
		pxb=(pxb<<1)|(cy>=acy);
	}
	
	clrm&=0x7FFF;
	clrn&=0x7FFF;
	
//	ict[0]=0x80000000U|(clrm<<15)|clrn;
//	ict[1]=0x0000;
//	ict[2]=pxb;
//	ict[3]=pxb>>32;

	((u64 *)ict)[0]=0x80000000ULL|(clrm<<15)|clrn;
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

#if 1
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

void TKGDI_BlitUpdate_ScanCellTransUTX2_Mask(
	u64 *ics, u32 *ict, byte *bmcs,
	int blkn, int sbxs)
{
	u64 *cs0, *cs1, *ct;
	u64 b0, b1, b2, b3;
	u64 c0, c1, c2, c3;
	u32 vi0, vi1, vi2, vi3, pi0, pi1, pi2, pi3;
	int i, j, k, b;
	
	cs0=ics;
	cs1=ics+((sbxs+3)>>2);
	ct=(u64 *)ict;
	
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
	int do4x, rowstr, cellstr, ictshl;
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

	do4x=0;
	rowstr=tkgdi_vid_rowstride;
	cellstr=tkgdi_vid_cellstride;
	ictshl=2;
	if(tkgdi_vid_is8bit)
		ictshl=1;

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


	bxm=dxo>>2;
	bxn=(dxo+dxs)>>2;
	bym=dyo>>2;
	byn=(dyo+dys)>>2;
	
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
		bmix=(by>>1)*bmxs2;
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
				{ ics-=4*sbxs; }
			else
				{ ics+=4*sbxs; }
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
					ict+(bmx0<<ictshl), ((bmx1-bmx0)+1)<<1,
					-sbxs);
			}
			ics-=4*sbxs;
		}else
		{
			if((sbxs==320) && !(tkgdi_vid_planar))
				TKGDI_BlitUpdate_ScanCopy(ics, ict, dxs>>2);
			else
			{
//				TKGDI_BlitUpdate_ScanCopyGen(ics, ict, dxs>>2, sbxs);
				TKGDI_BlitUpdate_ScanCopyGen(
					ics+(bmx0<<3),
					ict+(bmx0<<ictshl), ((bmx1-bmx0)+1)<<1,
					sbxs);
			}
			ics+=4*sbxs;
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

	ics=sbuf+(sbyo*sbxs)+sbxo;
//	ict=conbufa+((bym*80+bxm)*8);
	ict=conbufa+((bym*tkgdi_vid_rowstride)+(bxm*tkgdi_vid_cellstride));

	if(flip)
	{
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

	ics=sbuf+(sbyo*sbxh)+(sbxo>>1);
//	ict=conbufa+((bym*80+bxm)*8);
	ict=conbufa+((bym*tkgdi_vid_rowstride)+(bxm*tkgdi_vid_cellstride));

	if(flip)
	{
		ics=sbuf+((((sbys-sbyo-1)*sbxse)+sbxo)>>1);

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

//	for(i=0; i<256; i++)
//		{ pal2[i]=TKGDI_BlitUpdate_Repack32to16(pal[i]); }

	conbufa=TKGDI_BlitUpdate_GetConbuf();

	bxm=dxo>>2;
	bxn=(dxo+dxs)>>2;
	bym=dyo>>2;
	byn=(dyo+dys)>>2;

	ics=sbuf+(sbyo*sbxs)+sbxo;
//	ict=conbufa+((bym*80+bxm)*8);
	ict=conbufa+((bym*tkgdi_vid_rowstride)+(bxm*tkgdi_vid_cellstride));

	if(flip)
	{
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
			TKGDI_BlitUpdate_ScanCopyRGB(ics, ict, dxs>>2, -sbxs, stride);
			ics-=4*sbxs;
		}else
		{
			TKGDI_BlitUpdate_ScanCopyRGB(ics, ict, dxs>>2, sbxs, stride);
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
