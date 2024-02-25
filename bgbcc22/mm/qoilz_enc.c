/*
So, there are multiple experimental encoders here, for several subformats.

QOI: Original format
QOLZ: Adds external LZ4 compression.
QOLI: Adds internal pixel-level pattern matching.

Along with two major encoders:
One is lossless, the other is lossy.

Lossless checks for exact matches.
Lossy may attempt to use substitutes if they are "close enough".
The lossy version still uses the same formats as the lossless versions (for better or for worse).


QOI_EncodeImageBuffer:
QOILZ_EncodeImageBuffer:
Encode image given as RGBA.

QOI_EncodeImageBuffer555:
QOILZ_EncodeImageBuffer555:
Encode image given as RGB555.

Flags:
  0x1: Use QOLI format.
  0xE: Quality level:
    0=Lossless
    2=Best
    E=Worst

Initial assesment of lossy mode...
Well, nowhere near JPEG, but could be useful for something.

 */

#ifndef PDLIB_INT_BITS_T

#ifndef BTIC2F_BYTE
#define BTIC2F_BYTE
typedef unsigned char			byte;
typedef unsigned short		u16;
typedef unsigned int			u32;
typedef unsigned long long	u64;

typedef signed char			sbyte;
typedef signed short			s16;
typedef signed int			s32;
typedef signed long long		s64;
#endif

#endif

// #define QOILI_HASHSZ	4096
// #define QOILI_HASHSZ	8192
#define QOILI_HASHSZ	16384
#define QOILI_HASHMSK	(QOILI_HASHSZ-1)

// #define	QOILI_HROVMSK	31
// #define	QOILI_SDEPTH	32

// #define	QOILI_HROVMSK	63
// #define	QOILI_SDEPTH	59

#define	QOILI_HROVMSK	127
#define	QOILI_SDEPTH	121

#ifdef QOI_SMALLSTACK
#define QOILI_MALLOCHASH
#endif

int QOILZ_CheckMatchLz4(byte *str1, byte *str2, int nl);

int QOI_EncLiHash4(byte *cs)
{
	u32 v0, v1, v2, v3;
	u64 h;
	
#ifdef QOI_CASTDEREF
	v0=((u32 *)cs)[0];
	v1=((u32 *)cs)[1];
	v2=((u32 *)cs)[2];
	v3=((u32 *)cs)[3];
#else
	memcpy(&v0, cs+ 0, 4);
	memcpy(&v1, cs+ 4, 4);
	memcpy(&v2, cs+ 8, 4);
	memcpy(&v3, cs+12, 4);
#endif

#if 1
	h=v0;
	h=h*65521+v1;
	h=h*65521+v2;
	h=h*65521+v3;
	h=h*65521+(h>>16);
#endif

#if 0
	h=v0^(((u64)v1)<<7)^(((u64)v2)<<11)^(((u64)v3)<<17);
//	h=(h<<11)^(h>>32);
	h=(h<<5)^(h>>16);
//	h=(h<<3)^(h>>16);
#endif

	h=h*65521+(h>>16);
//	return((h>>16)&255);
	return((h>>16)&QOILI_HASHMSK);
}


int QOI_LossyCheckMatchRGBA(
	int cr0, int cg0, int cb0, int ca0,
	int cr1, int cg1, int cb1, int ca1,
	int errt)
{
	int dr, dg, db, da, d;

	if(	(cr0==cr1) && (cg0==cg1) &&
		(cb0==cb1) && (ca0==ca1))
			return(1);

	dr=cr0^cr1;
	dg=cg0^cg1;
	db=cb0^cb1;
	da=ca0^ca1;
	d=2*dg+dr+db+da;
	if(d<0)
		return(0);
	if(d<errt)
		return(1);
	return(0);
}

int QOI_LossyCalcErrorRGBA(
	int cr0, int cg0, int cb0, int ca0,
	int cr1, int cg1, int cb1, int ca1)
{
	int dr, dg, db, da, d;

	if(	(cr0==cr1) && (cg0==cg1) &&
		(cb0==cb1) && (ca0==ca1))
			return(0);

	dr=cr0^cr1;
	dg=cg0^cg1;
	db=cb0^cb1;
	da=ca0^ca1;
	d=2*dg+dr+db+da;
	if(d<0)
		return(65536);
	return(d);
}

int QOI_EncLiHash4LossyAlt(byte *cs, int errt, int errt_rcp, int alt)
{
	u32 v0, v1, v2, v3;
	int cr, cg, cb, ca, rcp2;
	int bir, big, bib;
	u64 h;
	int i;

	if(alt<8)
	{
		bir=(alt&4)?65536:0;
		big=(alt&2)?65536:0;
		bib=(alt&1)?65536:0;
	}else
	{
		bir=32768;
		big=32768;
		bib=32768;
	}
	
//	rcp2=errt_rcp>>1;
	rcp2=errt_rcp;
//	rcp2=errt_rcp<<1;
	
	cr=(cs[ 0]*rcp2+bir)>>16;
	cg=(cs[ 1]*rcp2+big)>>16;
	cb=(cs[ 2]*rcp2+bib)>>16;
	ca=(cs[ 3]*rcp2+32768)>>16;
	v0=cr*3+cg*5+cb*7+ca*11;
	cr=(cs[ 4]*rcp2+bir)>>16;
	cg=(cs[ 5]*rcp2+big)>>16;
	cb=(cs[ 6]*rcp2+bib)>>16;
	ca=(cs[ 7]*rcp2+32768)>>16;
	v1=cr*3+cg*5+cb*7+ca*11;
	cr=(cs[ 8]*rcp2+bir)>>16;
	cg=(cs[ 9]*rcp2+big)>>16;
	cb=(cs[10]*rcp2+bib)>>16;
	ca=(cs[11]*rcp2+32768)>>16;
	v2=cr*3+cg*5+cb*7+ca*11;
	cr=(cs[12]*rcp2+bir)>>16;
	cg=(cs[13]*rcp2+big)>>16;
	cb=(cs[14]*rcp2+bib)>>16;
	ca=(cs[15]*rcp2+32768)>>16;
	v3=cr*3+cg*5+cb*7+ca*11;

	h=v0;
	h=h*65521+v1;
	h=h*65521+v2;
	h=h*65521+v3;
	h=h*65521+(h>>16);
	h=h*65521+(h>>16);
	return((h>>16)&QOILI_HASHMSK);
}

int QOI_EncLiHash4Lossy(byte *cs, int errt, int errt_rcp)
{
	return(QOI_EncLiHash4LossyAlt(cs, errt, errt_rcp, 8));
}

int QOILZ_CheckMatchLossy(
	byte *str1, byte *str2, int nl, int errtb, int errtb_rcp,
	int *rterr)
{
	byte *cs1, *cs2;
	int dr, dg, db, da, d, td;
	int v0, v1, v2, v3;
	int i, j, k, nl4, errt, errth, errtq;
	
	if(str1==str2)
		return(0);

	dr=str1[0]^str2[0];
	dg=str1[1]^str2[1];
	db=str1[2]^str2[2];
	da=str1[3]^str2[3];
	d=2*dg+dr+db+da;
	if(d>errtb)
		return(0);

	td=d;

//	errth=errtb>>1;
	errtq=errtb>>2;
	
	nl4=nl/4;
	cs1=str1+4; cs2=str2+4;
	errt=errtb;
	for(i=1; i<24; i++)
	{
		v0=cs1[0]; v1=cs2[0];
		v2=cs1[1]; v3=cs2[1];
		dr=v0^v1;	dg=v2^v3;
		v0=cs1[2]; v1=cs2[2];
		v2=cs1[3]; v3=cs2[3];
		db=v0^v1;	da=v2^v3;
		d=2*dg+dr+db+da;
		if(d>errt)
			break;
		td+=d;
		cs1+=4;
		cs2+=4;
	}

	if(i>=24)
	{
		errt=errtq;
		for(i=24; i<nl4; i++)
		{
			v0=cs1[0]; v1=cs2[0];
			v2=cs1[1]; v3=cs2[1];
			dr=v0^v1;	dg=v2^v3;
			v0=cs1[2]; v1=cs2[2];
			v2=cs1[3]; v3=cs2[3];
			db=v0^v1;	da=v2^v3;
			d=2*dg+dr+db+da;
			if(d>errt)
				break;
			td+=d;
			cs1+=4;
			cs2+=4;
		}
	}
	
	if(i<4)
		return(0);
	
	if(rterr)
		*rterr=td;
	
	return(i*4);
}



int QOI_EncodeImageBufferLossy(
	byte *dstbuf, byte *img, int xs, int ys, int doli)
{
	byte pixtab[64*4];
#ifdef QOILI_MALLOCHASH
	int *hofs;
	byte *hrov;
	byte tdummy[QOILI_HROVMSK*5];
#else
	int hofs[QOILI_HASHSZ];
	byte hrov[QOILI_HASHSZ];
#endif
	byte *cs, *ct, *cs1, *cse, *imgbuf;
	int cr, cg, cb, ca;
	int dr, dg, db;
	int pdr, pdg, pdb;
	int pcr, pcg, pcb, pca;
	int crl, cgl, cbl, cal;
	int n, run, bl, bd, td, btd;
	int errt, errt_rcp, errtb, errtb_rcp, errtq, errtq_rcp, errth, errth_rcp;
	int i, j, k, l, h, hli, hlj, hr, hrj, nl;

#ifdef QOILI_MALLOCHASH
	if(doli&1)
	{
		hofs=malloc(QOILI_HASHSZ*sizeof(int));
		hrov=malloc(QOILI_HASHSZ);
	}else
	{
		hofs=(int *)tdummy;
		hrov=tdummy;
	}
#endif

	if(doli&0xE)
	{
//		errtb=(doli&0xE)<<2;
		errtb=(doli&0xE)<<3;
		errtb_rcp=65536/errtb;

		errth=errtb>>1;
		errth_rcp=65536/errth;

		errtq=errtb>>2;
		errtq_rcp=65536/errtq;
	}else
	{
		errtb=0;
		errtb_rcp=65536;
		errth=0;
		errth_rcp=65536;
		errtq=0;
		errtq_rcp=65536;
	}
		
	dstbuf[ 0]='q';
	dstbuf[ 1]='o';
	dstbuf[ 2]='i';
	dstbuf[ 3]='f';
	
	if(doli&1)
	{
		dstbuf[ 2]='l';
		dstbuf[ 3]='i';
	}

	dstbuf[ 4]=xs>>24;
	dstbuf[ 5]=xs>>16;
	dstbuf[ 6]=xs>> 8;
	dstbuf[ 7]=xs>> 0;

	dstbuf[ 8]=ys>>24;
	dstbuf[ 9]=ys>>16;
	dstbuf[10]=ys>> 8;
	dstbuf[11]=ys>> 0;

	dstbuf[12]=4;
	dstbuf[13]=1;
	
	n=xs*ys;
	cs=img; cse=img+n*4;
	
	ct=dstbuf+14;
	
	for(i=0; i<256; i++)
	{
//		pixtab[i*4+0]=0;
//		pixtab[i*4+1]=0;
//		pixtab[i*4+2]=0;
//		pixtab[i*4+3]=0;
		pixtab[i]=0;
//		hofs[i]=0;
	}

	if(doli&1)
	{
		for(i=0; i<QOILI_HASHSZ; i++)
		{
			hofs[i]=0;
			hrov[i]=0;
		}
	}
	
	crl=0;	cgl=0;
	cbl=0;	cal=255;
	run=0;
	while(cs<cse)
	{
		errt=errtb;
		errt_rcp=errtb_rcp;
		if(run>=8)
		{
			errt=errth;
			errt_rcp=errth_rcp;
		}
		if(run>=24)
		{
			errt=errtq;
			errt_rcp=errtq_rcp;
		}
	
		if(doli&1)
		{
//			hli=((j*251+1)>>8)&255;
			hli=QOI_EncLiHash4Lossy(cs, errt, errt_rcp);
			hr=hrov[hli];
		}else
		{
			hli=0;
			hr=0;
		}

		cr=cs[0];
		cg=cs[1];
		cb=cs[2];
		ca=cs[3];
		cs+=4;
		
//		if((cr==crl) && (cg==cgl) && (cb==cbl) && (ca==cal))
		if(QOI_LossyCheckMatchRGBA(cr, cg, cb, ca, crl, cgl, cbl, cal, errt))
		{
			run++;
			continue;
		}
		
		if(run)
		{
			if(doli&1)
			{
#if 1
				while(run>=1027)
				{
					*ct++=0xFB;
					*ct++=0xFF;
					*ct++=0x00;
					*ct++=0x01;
					run-=1027;
//					crl=-255;	cgl=-255;
//					cbl=-255;	cal=-255;
				}
			
				if(run>=240)
				{
					j=run-4;
					*ct++=0xF8|(j>>8);
					*ct++=j;
					*ct++=0x00;
					*ct++=0x01;
					run=0;
//					crl=-255;	cgl=-255;
//					cbl=-255;	cal=-255;
				}
#endif

#if 0
				while(run>=62)
				{
					*ct++=0xFD;
					run-=62;
				}
#endif

				while(run>=48)
				{
					*ct++=0xEF;
					run-=48;
				}
			}else
			{
				while(run>=62)
				{
					*ct++=0xFD;
					run-=62;
				}
			}
			if(run>0)
			{
				*ct++=0xC0+(run-1);
			}
			run=0;
		}

		if(doli&1)
		{
			nl=cse-cs;
			if(nl>16384)
				nl=16384;
		
			bl=0;	bd=0;
			btd=0;

#if 1
			for(i=0; i<QOILI_SDEPTH; i++)
//			for(i=0; i<24; i++)
			{
				cs1=img+hofs[(hli+((hr-i)&QOILI_HROVMSK))&QOILI_HASHMSK];
				if((cs1>=(cs-4)) || (cs1<img))
					continue;
				
				j=QOILZ_CheckMatchLossy(cs1, cs-4, nl, errt, errt_rcp, &td);
//				if(j>nl)
//					j=nl;
				j=j>>2;
				k=((cs-4)-cs1)>>2;

				if(k>=1048560)
					continue;

				if(j>bl)
					{ bl=j; bd=k; btd=td; }
				if((j==bl) && (td<btd))
					{ bl=j; bd=k; btd=td; }
			}
#endif
			
			for(l=0; l<8; l++)
			{
				hlj=QOI_EncLiHash4LossyAlt(cs, errt, errt_rcp, l);
				hrj=hrov[hlj];

#if 1
				for(i=0; i<12; i++)
				{
					cs1=img+hofs[(hlj+((hrj-i)&QOILI_HROVMSK))&QOILI_HASHMSK];
					if((cs1>=(cs-4)) || (cs1<img))
						continue;
					
					j=QOILZ_CheckMatchLossy(cs1, cs-4, nl, errt, errt_rcp, &td);
					j=j>>2;
					k=((cs-4)-cs1)>>2;
					
					if(k>=1048560)
						continue;
					
					if(j>bl)
						{ bl=j; bd=k; btd=td; }
					if((j==bl) && (td<btd))
						{ bl=j; bd=k; btd=td; }
				}
#endif
			}
			
			j=bl;
			k=bd;
			
			if((k>=4096) && (j<6))
				j=0;
			if((k>=65536) && (j<7))
				j=0;
			
//			j=0;
			
//			if((j>=5) && (j<132) && (k<4080) && (k>0))
//			if((j>=5) && (j<1027) && (k<65520) && (k>0))
			if((j>=5) && (j<4099) && (k<1048560) && (k>0))
			{
//				printf("QOLI E: l=%d d=%d\n", j, k);
				j-=4;
				if((j<127) && (k<4096))
				{
					*ct++=0xF0|(j>>4);
					*ct++=(j<<4)|(k>>8);
					*ct++=k;
				}else
					if((j<1023) && (k<65535))
				{
					*ct++=0xF8|(j>>8);
					*ct++=j;
					*ct++=k>>8;
					*ct++=k;
				}else
				{
					*ct++=0xFC;
					*ct++=j>>4;
					*ct++=(j<<4)|(k>>16);
					*ct++=k>>8;
					*ct++=k;
				}
				j+=4;
				
				i=j-1;
				while(i--)
				{
					hli=QOI_EncLiHash4Lossy(cs, errt, errt_rcp);
					hr=hrov[hli];
//					cr=cs[0];
//					cg=cs[1];
//					cb=cs[2];
//					ca=cs[3];
					cs+=4;
					
//					j=cr*3+cg*5+cb*7+ca*11;
//					hli=((j*251+1)>>8)&255;
//					hofs[hli]=(cs-4)-img;
					hofs[(hli+hr)&QOILI_HASHMSK]=(cs-4)-img;
					hrov[hli]=(hr+1)&QOILI_HROVMSK;
				}
				
//				cs+=(j-1)*4;
//				crl=cr;
//				cgl=cg;
//				cbl=cb;
//				cal=ca;
				
				crl=-255;	cgl=-255;
				cbl=-255;	cal=-255;
//				crl=cs[-4];
//				cgl=cs[-3];
//				cbl=cs[-2];
//				cal=cs[-1];
				continue;
			}
		}

//		j=cr*3+cg*5+cb*7+ca*11;
//		hli=((j*251+1)>>8)&255;

//		j=cr*3+cg*5+cb*7+ca*11;
//		h=j&63;
//		hli=j&255;
		h=(cr*3+cg*5+cb*7+ca*11)&63;

		k=h<<2;
		if(	(cr==pixtab[k+0]) &&
			(cg==pixtab[k+1]) &&
			(cb==pixtab[k+2]) &&
			(ca==pixtab[k+3]))
		{
			*ct++=0x00+h;
			crl=cr;	cgl=cg;
			cbl=cb;	cal=ca;
			continue;
		}

#if 1
		bl=65536; bd=0;
		for(i=0; i<64; i++)
		{
			pcr=pixtab[i*4+0];
			pcg=pixtab[i*4+1];
			pcb=pixtab[i*4+2];
			pca=pixtab[i*4+3];
			j=QOI_LossyCalcErrorRGBA(
				cr, cg, cb, ca,
				pcr, pcg, pcb, pca);
			if(j<bl)
			{
				bl=j;
				bd=i;
			}
		}
		
		if(bl<errt)
		{
			*ct++=0x00+bd;
			pcr=pixtab[bd*4+0];
			pcg=pixtab[bd*4+1];
			pcb=pixtab[bd*4+2];
			pca=pixtab[bd*4+3];
			crl=pcr;	cgl=pcg;
			cbl=pcb;	cal=pca;
			continue;
		}
#endif

		dr=cr-crl;
		dg=cg-cgl;
		db=cb-cbl;
		
		pdr=dr;
		pdg=dg;
		pdb=db;
		if(pdr<-2)pdr=-2;
		if(pdr> 1)pdr= 1;
		if(pdg<-2)pdg=-2;
		if(pdg> 1)pdg= 1;
		if(pdb<-2)pdb=-2;
		if(pdb> 1)pdb= 1;
		
		pcr=crl+pdr;
		pcg=cgl+pdg;
		pcb=cbl+pdb;
		pca=cal;

#if 1
//		if(	(dr>=-2) && (dr<=1) &&
//			(dg>=-2) && (dg<=1) &&
//			(db>=-2) && (db<=1) &&
//			(ca==cal))
		if(QOI_LossyCheckMatchRGBA(cr, cg, cb, ca, pcr, pcg, pcb, pca, errt))
		{
			*ct++=0x40|((pdr+2)<<4)|((pdg+2)<<2)|((pdb+2)<<0);
			h=(pcr*3+pcg*5+pcb*7+pca*11)&63;
			k=h<<2;
			pixtab[k+0]=pcr;	pixtab[k+1]=pcg;
			pixtab[k+2]=pcb;	pixtab[k+3]=pca;
//			hofs[hli]=(cs-4)-img;
			hofs[(hli+hr)&QOILI_HASHMSK]=(cs-4)-img;
			hrov[hli]=(hr+1)&QOILI_HROVMSK;
			crl=pcr;	cgl=pcg;
			cbl=pcb;	cal=pca;
			continue;
		}
#endif
		
		dr-=dg;
		db-=dg;

		pdr=dr;
		pdg=dg;
		pdb=db;
		if(pdg<-32)pdg=-32;
		if(pdg> 31)pdg= 31;

		if(pdr<-8)pdr=-8;
		if(pdr> 7)pdr= 7;
		if(pdb<-8)pdb=-8;
		if(pdb> 7)pdb= 7;
		
		pcg=cgl+pdg;
		pcr=crl+pdg+pdr;
		pcb=cbl+pdg+pdb;
		pca=cal;

//		if(	(dg>=-32) && (dg<=31) &&
//			(dr>=-8) && (dr<=7) &&
//			(db>=-8) && (db<=7) &&
//			(ca==cal))
		if(QOI_LossyCheckMatchRGBA(cr, cg, cb, ca, pcr, pcg, pcb, pca, errt))
		{
			*ct++=0x80|(pdg+32);
			*ct++=((pdr+8)<<4)|((pdb+8)<<0);
			h=(pcr*3+pcg*5+pcb*7+pca*11)&63;
			k=h<<2;
			pixtab[k+0]=pcr;	pixtab[k+1]=pcg;
			pixtab[k+2]=pcb;	pixtab[k+3]=pca;
//			hofs[hli]=(cs-4)-img;
			hofs[(hli+hr)&QOILI_HASHMSK]=(cs-4)-img;
			hrov[hli]=(hr+1)&QOILI_HROVMSK;
			crl=pcr;	cgl=pcg;
			cbl=pcb;	cal=pca;
			continue;
		}
		
		if(ca==cal)
		{
			*ct++=0xFE;
			*ct++=cr;
			*ct++=cg;
			*ct++=cb;
		}else
		{
			*ct++=0xFF;
			*ct++=cr;
			*ct++=cg;
			*ct++=cb;
			*ct++=ca;
		}

		h=(cr*3+cg*5+cb*7+ca*11)&63;
		k=h<<2;
		pixtab[k+0]=cr;	pixtab[k+1]=cg;
		pixtab[k+2]=cb;	pixtab[k+3]=ca;
//		hofs[hli]=(cs-4)-img;
		hofs[(hli+hr)&QOILI_HASHMSK]=(cs-4)-img;
		hrov[hli]=(hr+1)&QOILI_HROVMSK;
		
		crl=cr;
		cgl=cg;
		cbl=cb;
		cal=ca;
	}

	if(run)
	{
		if(doli&1)
		{
			while(run>=48)
			{
				*ct++=0xEF;
				run-=48;
			}
		}else
		{
			while(run>=62)
			{
				*ct++=0xFD;
				run-=62;
			}
		}
		if(run>0)
		{
			*ct++=0xC0+(run-1);
		}
		run=0;
	}

	*ct++=0x00;
	*ct++=0x00;
	*ct++=0x00;
	*ct++=0x00;
	*ct++=0x00;
	*ct++=0x00;
	*ct++=0x00;
	*ct++=0x01;

#ifdef QOILI_MALLOCHASH
	if(doli&1)
	{
		free(hofs);
		free(hrov);
	}
#endif

	return(ct-dstbuf);
}



int QOI_EncodeImageBuffer(byte *dstbuf, byte *img, int xs, int ys, int doli)
{
	byte pixtab[64*4];
#ifdef QOILI_MALLOCHASH
	int *hofs;
	byte *hrov;
	byte tdummy[QOILI_HROVMSK*5];
#else
	int hofs[QOILI_HASHSZ];
	byte hrov[QOILI_HASHSZ];
#endif
	byte *cs, *ct, *cs1, *cse, *imgbuf;
	int cr, cg, cb, ca;
	int dr, dg, db;
	int crl, cgl, cbl, cal;
	int n, run, bl, bd;
	int i, j, k, h, hli, hr, nl;

	if(doli&0xE)
	{
		return(QOI_EncodeImageBufferLossy(dstbuf, img, xs, ys, doli));
	}


#ifdef QOILI_MALLOCHASH
	if(doli&1)
	{
		hofs=malloc(QOILI_HASHSZ*sizeof(int));
		hrov=malloc(QOILI_HASHSZ);
	}else
	{
		hofs=(int *)tdummy;
		hrov=tdummy;
	}
#endif
	
	dstbuf[ 0]='q';
	dstbuf[ 1]='o';
	dstbuf[ 2]='i';
	dstbuf[ 3]='f';
	
	if(doli&1)
	{
		dstbuf[ 2]='l';
		dstbuf[ 3]='i';
	}

	dstbuf[ 4]=xs>>24;
	dstbuf[ 5]=xs>>16;
	dstbuf[ 6]=xs>> 8;
	dstbuf[ 7]=xs>> 0;

	dstbuf[ 8]=ys>>24;
	dstbuf[ 9]=ys>>16;
	dstbuf[10]=ys>> 8;
	dstbuf[11]=ys>> 0;

	dstbuf[12]=4;
	dstbuf[13]=1;
	
	n=xs*ys;
	cs=img; cse=img+n*4;
	
	ct=dstbuf+14;
	
	for(i=0; i<256; i++)
	{
//		pixtab[i*4+0]=0;
//		pixtab[i*4+1]=0;
//		pixtab[i*4+2]=0;
//		pixtab[i*4+3]=0;
		pixtab[i]=0;
//		hofs[i]=0;
	}

	if(doli&1)
	{
		for(i=0; i<QOILI_HASHSZ; i++)
		{
			hofs[i]=0;
			hrov[i]=0;
		}
	}
	
	crl=0;	cgl=0;
	cbl=0;	cal=255;
	run=0;
	while(cs<cse)
	{
		if(doli&1)
		{
//			hli=((j*251+1)>>8)&255;
			hli=QOI_EncLiHash4(cs);
			hr=hrov[hli];
		}else
		{
			hli=0;
			hr=0;
		}

		cr=cs[0];
		cg=cs[1];
		cb=cs[2];
		ca=cs[3];
		cs+=4;
		
		if((cr==crl) && (cg==cgl) && (cb==cbl) && (ca==cal))
		{
			run++;
			continue;
		}
		
		if(run)
		{
			if(doli&1)
			{
#if 1
				while(run>=1027)
				{
					*ct++=0xFB;
					*ct++=0xFF;
					*ct++=0x00;
					*ct++=0x01;
					run-=1027;
				}
			
				if(run>=240)
				{
					j=run-4;
					*ct++=0xF8|(j>>8);
					*ct++=j;
					*ct++=0x00;
					*ct++=0x01;
					run=0;
				}
#endif

#if 0
				while(run>=62)
				{
					*ct++=0xFD;
					run-=62;
				}
#endif

				while(run>=48)
				{
					*ct++=0xEF;
					run-=48;
				}
			}else
			{
				while(run>=62)
				{
					*ct++=0xFD;
					run-=62;
				}
			}
			if(run>0)
			{
				*ct++=0xC0+(run-1);
			}
			run=0;
		}

		if(doli&1)
		{
			nl=cse-cs;
			if(nl>16384)
				nl=16384;
		
			bl=0;	bd=0;
			for(i=0; i<QOILI_SDEPTH; i++)
			{
				cs1=img+hofs[(hli+((hr-i)&QOILI_HROVMSK))&QOILI_HASHMSK];
				if((cs1>=(cs-4)) || (cs1<img))
					continue;
				
				j=QOILZ_CheckMatchLz4(cs1, cs-4, nl);
//				if(j>nl)
//					j=nl;
				j=j>>2;
				k=((cs-4)-cs1)>>2;
				
				if(j>bl)
					{ bl=j; bd=k; }
			}
			
			j=bl;
			k=bd;
			
			if((k>=4096) && (j<6))
				j=0;
			if((k>=65536) && (j<7))
				j=0;
			
//			j=0;
			
//			if((j>=5) && (j<132) && (k<4080) && (k>0))
//			if((j>=5) && (j<1027) && (k<65520) && (k>0))
			if((j>=5) && (j<4099) && (k<1048560) && (k>0))
			{
//				printf("QOLI E: l=%d d=%d\n", j, k);
				j-=4;
				if((j<127) && (k<4096))
				{
					*ct++=0xF0|(j>>4);
					*ct++=(j<<4)|(k>>8);
					*ct++=k;
				}else
					if((j<1023) && (k<65535))
				{
					*ct++=0xF8|(j>>8);
					*ct++=j;
					*ct++=k>>8;
					*ct++=k;
				}else
				{
					*ct++=0xFC;
					*ct++=j>>4;
					*ct++=(j<<4)|(k>>16);
					*ct++=k>>8;
					*ct++=k;
				}
				j+=4;
				
				i=j-1;
				while(i--)
				{
					hli=QOI_EncLiHash4(cs);
					hr=hrov[hli];
//					cr=cs[0];
//					cg=cs[1];
//					cb=cs[2];
//					ca=cs[3];
					cs+=4;
					
//					j=cr*3+cg*5+cb*7+ca*11;
//					hli=((j*251+1)>>8)&255;
//					hofs[hli]=(cs-4)-img;
					hofs[(hli+hr)&QOILI_HASHMSK]=(cs-4)-img;
					hrov[hli]=(hr+1)&QOILI_HROVMSK;
				}
				
//				cs+=(j-1)*4;
//				crl=cr;
//				cgl=cg;
//				cbl=cb;
//				cal=ca;
				
				crl=-255;	cgl=-255;
				cbl=-255;	cal=-255;
//				crl=cs[-4];
//				cgl=cs[-3];
//				cbl=cs[-2];
//				cal=cs[-1];
				continue;
			}
		}

//		j=cr*3+cg*5+cb*7+ca*11;
//		hli=((j*251+1)>>8)&255;

//		j=cr*3+cg*5+cb*7+ca*11;
//		h=j&63;
//		hli=j&255;
		h=(cr*3+cg*5+cb*7+ca*11)&63;

		k=h<<2;
		if(	(cr==pixtab[k+0]) &&
			(cg==pixtab[k+1]) &&
			(cb==pixtab[k+2]) &&
			(ca==pixtab[k+3]))
		{
			*ct++=0x00+h;
			crl=cr;	cgl=cg;
			cbl=cb;	cal=ca;
			continue;
		}
		
		dr=cr-crl;
		dg=cg-cgl;
		db=cb-cbl;
		
		if(	(dr>=-2) && (dr<=1) &&
			(dg>=-2) && (dg<=1) &&
			(db>=-2) && (db<=1) &&
			(ca==cal))
		{
			*ct++=0x40|((dr+2)<<4)|((dg+2)<<2)|((db+2)<<0);
			h=(cr*3+cg*5+cb*7+ca*11)&63;
			k=h<<2;
			pixtab[k+0]=cr;	pixtab[k+1]=cg;
			pixtab[k+2]=cb;	pixtab[k+3]=ca;
//			hofs[hli]=(cs-4)-img;
			hofs[(hli+hr)&QOILI_HASHMSK]=(cs-4)-img;
			hrov[hli]=(hr+1)&QOILI_HROVMSK;
			crl=cr;	cgl=cg;
			cbl=cb;	cal=ca;
			continue;
		}
		
		dr-=dg;
		db-=dg;

		if(	(dg>=-32) && (dg<=31) &&
			(dr>=-8) && (dr<=7) &&
			(db>=-8) && (db<=7) &&
			(ca==cal))
		{
			*ct++=0x80|(dg+32);
			*ct++=((dr+8)<<4)|((db+8)<<0);
			h=(cr*3+cg*5+cb*7+ca*11)&63;
			k=h<<2;
			pixtab[k+0]=cr;	pixtab[k+1]=cg;
			pixtab[k+2]=cb;	pixtab[k+3]=ca;
//			hofs[hli]=(cs-4)-img;
			hofs[(hli+hr)&QOILI_HASHMSK]=(cs-4)-img;
			hrov[hli]=(hr+1)&QOILI_HROVMSK;
			crl=cr;	cgl=cg;
			cbl=cb;	cal=ca;
			continue;
		}
		
		if(ca==cal)
		{
			*ct++=0xFE;
			*ct++=cr;
			*ct++=cg;
			*ct++=cb;
		}else
		{
			*ct++=0xFF;
			*ct++=cr;
			*ct++=cg;
			*ct++=cb;
			*ct++=ca;
		}

		h=(cr*3+cg*5+cb*7+ca*11)&63;
		k=h<<2;
		pixtab[k+0]=cr;	pixtab[k+1]=cg;
		pixtab[k+2]=cb;	pixtab[k+3]=ca;
//		hofs[hli]=(cs-4)-img;
		hofs[(hli+hr)&QOILI_HASHMSK]=(cs-4)-img;
		hrov[hli]=(hr+1)&QOILI_HROVMSK;
		
		crl=cr;
		cgl=cg;
		cbl=cb;
		cal=ca;
	}

	if(run)
	{
		if(doli&1)
		{
			while(run>=48)
			{
				*ct++=0xEF;
				run-=48;
			}
		}else
		{
			while(run>=62)
			{
				*ct++=0xFD;
				run-=62;
			}
		}
		if(run>0)
		{
			*ct++=0xC0+(run-1);
		}
		run=0;
	}

	*ct++=0x00;
	*ct++=0x00;
	*ct++=0x00;
	*ct++=0x00;
	*ct++=0x00;
	*ct++=0x00;
	*ct++=0x00;
	*ct++=0x01;

#ifdef QOILI_MALLOCHASH
	if(doli&1)
	{
		free(hofs);
		free(hrov);
	}
#endif

	return(ct-dstbuf);
}

int QOI_EncodeImageBuffer555(byte *dstbuf, u16 *img, int xs, int ys, int fl)
{
	byte *ibuf, *ct;
	u16 *cs;
	int cr, cg, cb, ca;
	int x, y, v, sz;
	
	ibuf=malloc(xs*ys*4);
	ct=ibuf;
	for(y=0; y<ys; y++)
	{
		cs=img+(ys-y-1)*xs;
		for(x=0; x<xs; x++)
		{
			v=*cs++;
			cr=(v>>10)&31;
			cg=(v>> 5)&31;
			cb=(v>> 0)&31;
			cr=(cr<<3)|(cr>>2);
			cg=(cg<<3)|(cg>>2);
			cb=(cb<<3)|(cb>>2);
			ca=(v&0x8000)?0:255;
			
			ct[0]=cr;
			ct[1]=cg;
			ct[2]=cb;
			ct[3]=ca;
			ct+=4;
		}
	}
	
	sz=QOI_EncodeImageBuffer(dstbuf, ibuf, xs, ys, fl);
	free(ibuf);
	return(sz);
}


// #define QOILZ4_ROVMSK	255
#define QOILZ4_ROVMSK	63
// #define QOILZ4_ROVMSK	31

// #define QOILZ4_HASHSZ	4096
// #define QOILZ4_HASHSZ	8192
// #define QOILZ4_HASHSZ	16384
#define QOILZ4_HASHSZ	65536
#define QOILZ4_HASHMSK	(QOILZ4_HASHSZ-1)
// #define QOILZ4_HASHSHR	18
#define QOILZ4_HASHSHR	16

#define QOILZ4_HASH1MSK QOILZ4_HASHMSK
// #define QOILZ4_HASH1MSK	(QOILZ4_HASHMSK&(~QOILZ4_ROVMSK))

#define QOILZ4_MALLOCHASH

// #define QOILZ4_SDEPTH	32
#define QOILZ4_SDEPTH	56

int QOILZ_CheckMatchLz4(byte *str1, byte *str2, int nl)
{
	byte *cs1, *cs1e, *cs1e0, *cs2;

#ifdef QOI_CASTDEREF
	if((*(u32 *)str1)!=(*(u32 *)str2))
		return(0);
	cs1=str1+4;
	cs2=str2+4;
	cs1e0=str1+(nl-8);
	cs1e=str1+nl;
//	while(cs1<cs1e)
	while(cs1<cs1e0)
	{
		if((*(u64 *)cs1)!=(*(u64 *)cs2))
			break;
		cs1+=8; cs2+=8;
	}
#else
	if(	(str1[0]!=str2[0]) ||
		(str1[1]!=str2[1]) ||
		(str1[2]!=str2[2]) ||
		(str1[3]!=str2[3]) )
			return(0);
	cs1=str1+4;
	cs2=str2+4;
	cs1e=str1+nl;
#endif

	while(cs1<cs1e)
	{
		if((*cs1)!=(*cs2))
			break;
		cs1++; cs2++;
	}
	return(cs1-str1);
}

int QOILZ_PackBufferLz4(byte *dstbuf, byte *srcbuf, int sz)
{
//	byte *hash[QOILZ4_HASHSZ];
#ifdef QOILZ4_MALLOCHASH
	int *hofs;
	byte *hrov;
#else
	int hofs[QOILZ4_HASHSZ];
	byte hrov[QOILZ4_HASHSZ];
#endif
	byte *cs, *cse, *csn, *ct, *cs1, *csr;
	int i, j, k, h, d, l, nl, r, tg, bd, bl, hr, h1;
	int bdl, bll, cf;

#ifdef QOILZ4_MALLOCHASH
	hofs=malloc(QOILZ4_HASHSZ*sizeof(int));
	hrov=malloc(QOILZ4_HASHSZ);
#endif
	
	for(i=0; i<QOILZ4_HASHSZ; i++)
	{
//		hash[i]=srcbuf;
		hofs[i]=0;
		hrov[i]=0;
	}
	
	ct=dstbuf;
	cs=srcbuf;
	cse=srcbuf+sz;
	csr=cs;
	
	while(cs<cse)
	{
//		h=cs[0]+1;
//		h=h*251+cs[1];
//		h=h*251+cs[2];
//		h=h*251+cs[3];

//		csn=cs+1;
		csn=cs;
		h=csn[0]+1;
		h=h*251+csn[1];
		h=h*251+csn[2];
		h=h*251+csn[3];

//		h=((h*251+1)>>24)&255;
//		h=((h*251+1)>>20)&4095;
		h=((h*65521+1)>>QOILZ4_HASHSHR)&QOILZ4_HASH1MSK;
		
		bd=0; bl=0;
		hr=hrov[h];
		
//		for(i=0; i<32; i++)
		for(i=0, j=0; i<QOILZ4_SDEPTH; i++, j+=7)
		{
//			h1=(h+((hr-i-1)&15)*31)&4095;
//			h1=(h+((hr-(i+0)*7)&QOILZ4_ROVMSK))&QOILZ4_HASHMSK;
			h1=(h+((hr-j)&QOILZ4_ROVMSK))&QOILZ4_HASHMSK;
//			cs1=hash[h1];
			cs1=srcbuf+hofs[h1];
//			d=csn-cs1; l=0;
//			if((d>0) && (d<65520))
//			if(1)
			if(*cs1==*csn)
			{
				nl=cse-cs;
				d=csn-cs1;
//				nl=4096;
//				if((cse-cs)<nl)
//					nl=cse-csn;

				if(nl>4096)
					nl=4096;

//				for(l=0; l<nl; l++)
//					if(csn[l]!=cs1[l])
//						break;
				l=QOILZ_CheckMatchLz4(csn, cs1, nl);
	//			if(l>d)
	//				l=0;

				if((l>bl) && (d<65520) && (l>4) && (d>0))
				{
					bl=l;
					bd=d;
				}
			}
		}

		d=bd;
		l=bl;

#if 1
		r=cs-csr;
		cf=3;
		if(r>=15)
			{ cf+=1+(r>>8); }
		if(l>=19)
			{ cf+=1+(l>>8); }

		if(d>4096)
			if(l<8)
				l=0;
		if(cf>=l)
			l=0;
#endif

//		d=bdl;
//		l=bll;
//		if((bl+1)>bll)
//			l=0;
		
		if((d<=0) || (l<4) || (d>=65520))
		{
			bdl=bd;
			bll=bl;

			h=cs[0]+1;
			h=h*251+cs[1];
			h=h*251+cs[2];
			h=h*251+cs[3];
//			h=((h*251+1)>>20)&4095;
			h=((h*65521+1)>>QOILZ4_HASHSHR)&QOILZ4_HASH1MSK;

			hr=hrov[h];

//			hash[h]=cs;
			h1=(h+hr)&QOILZ4_HASHMSK;
//			hash[h1]=cs;
			hofs[h1]=cs-srcbuf;
			hrov[h]=(hr+7)&QOILZ4_ROVMSK;
			cs++;
			continue;
		}

		r=cs-csr;
		
//		if((cs-srcbuf)<1024)
//			printf("E: r=%d d=%d l=%d\n", r, d, l);
		
		tg=((r<<4)&0xF0)|((l-4)&0x0F);
		if(r>=15)
			tg|=0xF0;
		if(l>=19)
			tg|=0x0F;
		*ct++=tg;

		i=r;
		if(i>=15)
		{
			i-=15;
			while(i>=255)
			{
				*ct++=0xFF;
				i-=255;
			}
			*ct++=i;
		}
		
		memcpy(ct, csr, r);
		ct+=r;	csr+=r;
		
		*ct++=d;
		*ct++=d>>8;
		
		i=l;
		if(i>=19)
		{
			i-=19;
			while(i>=255)
			{
				*ct++=0xFF;
				i-=255;
			}
			*ct++=i;
		}
		
		cs+=l;
#if 1
		while(csr<cs)
		{
			h=csr[0]+1;
			h=h*251+csr[1];
			h=h*251+csr[2];
			h=h*251+csr[3];
//			h=((h*251+1)>>24)&255;
//			h=((h*251+1)>>20)&4095;
			h=((h*65521+1)>>QOILZ4_HASHSHR)&QOILZ4_HASH1MSK;

//			hash[h]=csr;

			hr=hrov[h];
			h1=(h+hr)&QOILZ4_HASHMSK;
//			hash[h1]=cs;
			hofs[h1]=cs-srcbuf;
			hrov[h]=(hr+7)&QOILZ4_ROVMSK;

			csr++;
		}
#endif

		csr=cs;
		bdl=0;
		bll=0;
	}

	r=cs-csr;
	if(r>0)
	{
		l=4;
		tg=((r<<4)&0xF0)|((l-4)&15);
		if(r>=15)
			tg|=0xF0;
		if(l>=19)
			tg|=0x0F;
		*ct++=tg;

		i=r;
		if(i>=15)
		{
			i-=15;
			while(i>=255)
			{
				*ct++=0xFF;
				i-=255;
			}
			*ct++=i;
		}
		
		memcpy(ct, csr, r);
		ct+=r;
		csr=cs;
	}
	ct[0]=0;
	ct[1]=0;

#ifdef QOILZ4_MALLOCHASH
	free(hofs);
	free(hrov);
#endif
	
	return(ct-dstbuf);
}

int QOILZ_EncodeImageBuffer(byte *dstbuf, byte *img, int xs, int ys, int fl)
{
	byte *t1buf, *t2buf, *t3buf;
	int sz1, sz2, sz3, sz0, sz;
	
	t1buf=malloc(6*xs*ys);
	t2buf=malloc(6*xs*ys);
	t3buf=malloc(6*xs*ys);
	sz1=QOI_EncodeImageBuffer(t1buf, img, xs, ys, (fl&(~1))|0);

	sz3=QOI_EncodeImageBuffer(t3buf, img, xs, ys, (fl&(~1))|1);
	
	t2buf[0]='q';
	t2buf[1]='o';
	t2buf[2]='l';
	t2buf[3]='4';

	t2buf[ 4]=xs>>24;
	t2buf[ 5]=xs>>16;
	t2buf[ 6]=xs>> 8;
	t2buf[ 7]=xs>> 0;

	t2buf[ 8]=ys>>24;
	t2buf[ 9]=ys>>16;
	t2buf[10]=ys>> 8;
	t2buf[11]=ys>> 0;

	t2buf[12]=4;
	t2buf[13]=1;
	t2buf[14]=0;
	t2buf[15]=0;

	sz0=sz1-14;
	t2buf[16]=sz0>>24;
	t2buf[17]=sz0>>16;
	t2buf[18]=sz0>> 8;
	t2buf[19]=sz0>> 0;

	printf("Enc Hash %08X\n", QOILZ_HashBuffer(t1buf+14, sz0));
	
	sz2=QOILZ_PackBufferLz4(t2buf+24, t1buf+14, sz0);

	t2buf[20]=sz2>>24;
	t2buf[21]=sz2>>16;
	t2buf[22]=sz2>> 8;
	t2buf[23]=sz2>> 0;

	sz2+=24;
	
//	if(sz2<sz1)
	if((sz2*1.25)<sz1)
//	if(1)
	{
		if((sz2*1.25)<sz3)
//		if(1)
		{
			memcpy(dstbuf, t2buf, sz2);
			sz=sz2;
		}else
		{
			memcpy(dstbuf, t3buf, sz3);
			sz=sz3;
		}
	}else
	{
		if((sz3*1.10)<sz1)
		{
			memcpy(dstbuf, t3buf, sz3);
			sz=sz3;
		}else
		{
			memcpy(dstbuf, t1buf, sz1);
			sz=sz1;
		}
	}

	free(t1buf);
	free(t2buf);
	free(t3buf);
	
	printf("QOILZ_EncodeImageBuffer: QOI=%d LZ=%d LI=%d Pick=%d\n",
		sz1, sz2, sz3, sz);
	
	return(sz);
}


int QOILZ_EncodeImageBuffer555(byte *dstbuf, u16 *img, int xs, int ys, int fl)
{
	byte *ibuf, *ct;
	u16 *cs;
	int cr, cg, cb, ca;
	int x, y, v, sz;
	
	ibuf=malloc(xs*ys*4);
	ct=ibuf;
	for(y=0; y<ys; y++)
	{
		cs=img+(ys-y-1)*xs;
		for(x=0; x<xs; x++)
		{
			v=*cs++;
			cr=(v>>10)&31;
			cg=(v>> 5)&31;
			cb=(v>> 0)&31;
			cr=(cr<<3)|(cr>>2);
			cg=(cg<<3)|(cg>>2);
			cb=(cb<<3)|(cb>>2);
			ca=(v&0x8000)?0:255;
			
			ct[0]=cr;
			ct[1]=cg;
			ct[2]=cb;
			ct[3]=ca;
			ct+=4;
		}
	}
	
	sz=QOILZ_EncodeImageBuffer(dstbuf, ibuf, xs, ys, fl);
	free(ibuf);
	return(sz);
}
