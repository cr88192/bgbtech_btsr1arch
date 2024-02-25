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

/*
 * QOI_CASTDEREF: Flag whether or not it is safe to cast and deref.
 * This generally works fine on MSVC and BGBCC.
 * But, GCC and Clang may cause incorrect behavior.
 * However, they are better able to optimize memcpy(), so...
 *
 * QOI_SMALLSTACK: Flag if the stack is small.
 * Say: If the target has 64K or 128K stack, don't put big arrays there.
 * But, say, for MSVC, we have 1MB, so...
 */

#ifdef _MSC_VER
#define QOI_CASTDEREF
#endif

#ifdef __BGBCC__
#define QOI_CASTDEREF
#define QOI_SMALLSTACK
#endif

void QOILZ_LzMemCpy(byte *dst, byte *src, int sz);

int QOI_DecImageBufferFlat(
	byte *imgbuf, byte *inbuf, int *rxs, int *rys, int decfl)
{
	byte pixtab[64*4];
	byte *cs, *ct, *cte;
	int cr, cg, cb, ca, qoli, dobgra;
	int xs, ys, n, ml, md;
	int i, j, k, l, h;
	
//	if(	(inbuf[0]!='q') ||
//		(inbuf[1]!='o') ||
//		(inbuf[2]!='i') ||
//		(inbuf[3]!='f') )
//	{
//		return(NULL);
//	}

	qoli=0;
	if(	(inbuf[0]!='q') ||
		(inbuf[1]!='o') )
	{
		return(-1);
	}

	if(	(inbuf[2]!='i') ||
		(inbuf[3]!='f') )
	{
		if(	(inbuf[2]!='l') ||
			(inbuf[3]!='i') )
		{
			return(-1);
		}
		qoli=1;
	}
	
	dobgra=0;
	if(decfl&1)
		dobgra=1;

	xs=	(inbuf[ 4]<<24) |
		(inbuf[ 5]<<16) |
		(inbuf[ 6]<< 8) |
		(inbuf[ 7]<< 0) ;
	ys=	(inbuf[ 8]<<24) |
		(inbuf[ 9]<<16) |
		(inbuf[10]<< 8) |
		(inbuf[11]<< 0) ;
	cs=inbuf+14;

	if(!imgbuf)
	{
		if(rxs)		*rxs=xs;
		if(rys)		*rys=ys;
		return(1);
	}

	n=xs*ys;
//	imgbuf=malloc(n*4);
	ct=imgbuf;
	cte=ct+n*4;
	
	for(i=0; i<256; i++)
		pixtab[i]=0;
	
	cr=0;	cg=0;
	cb=0;	ca=255;
	while(ct<cte)
	{
		j=*cs++;
		i=j>>6;

		if((j==0) && (cs[0]==0))
			break;

		if(i==0)
		{
			k=j&63;
			k=k<<2;
			cr=pixtab[k+0];	cg=pixtab[k+1];
			cb=pixtab[k+2];	ca=pixtab[k+3];
			ct[0]=cr;	ct[1]=cg;
			ct[2]=cb;	ct[3]=ca;
			if(dobgra)
				{ ct[0]=cb; ct[2]=cr; }
			ct+=4;
			continue;
		}

		if(i==1)
		{
			cr+=((j>>4)&3)-2;
			cg+=((j>>2)&3)-2;
			cb+=((j>>0)&3)-2;
			h=(cr*3+cg*5+cb*7+ca*11)&63;
			k=h<<2;
			pixtab[k+0]=cr;	pixtab[k+1]=cg;
			pixtab[k+2]=cb;	pixtab[k+3]=ca;
			ct[0]=cr;	ct[1]=cg;
			ct[2]=cb;	ct[3]=ca;
			if(dobgra)
				{ ct[0]=cb; ct[2]=cr; }
			ct+=4;
			continue;
		}

		if(i==2)
		{
			k=(j&63)-32;
			cr+=k;
			cg+=k;
			cb+=k;

			j=*cs++;
			cr+=((j>>4)&15)-8;
			cb+=((j>>0)&15)-8;

			h=(cr*3+cg*5+cb*7+ca*11)&63;
			k=h<<2;
			pixtab[k+0]=cr;	pixtab[k+1]=cg;
			pixtab[k+2]=cb;	pixtab[k+3]=ca;
			ct[0]=cr;	ct[1]=cg;
			ct[2]=cb;	ct[3]=ca;
			if(dobgra)
				{ ct[0]=cb; ct[2]=cr; }
			ct+=4;
			continue;
		}

		k=j&63;
//		if(k<62)
		if((k<48) || (!qoli && (k<62)))
		{
			k++;
			while(k--)
			{
				ct[0]=cr;	ct[1]=cg;
				ct[2]=cb;	ct[3]=ca;
				if(dobgra)
					{ ct[0]=cb; ct[2]=cr; }
				ct+=4;
			}
			continue;
		}
		
		if(j==0xFE)
		{
			cr=cs[0];
			cg=cs[1];
			cb=cs[2];
			cs+=3;

			h=(cr*3+cg*5+cb*7+ca*11)&63;
			k=h<<2;
			pixtab[k+0]=cr;	pixtab[k+1]=cg;
			pixtab[k+2]=cb;	pixtab[k+3]=ca;
			ct[0]=cr;	ct[1]=cg;
			ct[2]=cb;	ct[3]=ca;
			if(dobgra)
				{ ct[0]=cb; ct[2]=cr; }
			ct+=4;
			continue;
		}

		
		if(j==0xFF)
		{
			cr=cs[0];
			cg=cs[1];
			cb=cs[2];
			ca=cs[3];
			cs+=4;

			h=(cr*3+cg*5+cb*7+ca*11)&63;
			k=h<<2;
			pixtab[k+0]=cr;	pixtab[k+1]=cg;
			pixtab[k+2]=cb;	pixtab[k+3]=ca;
			ct[0]=cr;	ct[1]=cg;
			ct[2]=cb;	ct[3]=ca;
			if(dobgra)
				{ ct[0]=cb; ct[2]=cr; }
			ct+=4;
			continue;
		}
		
		if(qoli)
		{
			if((j&0xF8)==0xF0)
			{
				k=cs[0];
				l=cs[1];
				cs+=2;
				ml=((j&7)<<4)|(k>>4);
				md=((k&15)<<8)|l;
				ml+=4;
			}
			else if((j&0xFC)==0xF8)
			{
				k=cs[0];
				l=cs[1];
				ml=((j&3)<<8)|k;
				md=(l<<8)|cs[2];
				cs+=3;
				ml+=4;
			}else if((j&0xFF)==0xFC)
			{
				k=cs[0];
				l=cs[1];
				ml=(k<<4)|(l>>4);
				md=((l&15)<<16)|(cs[2]<<8)|cs[3];
				cs+=4;
				ml+=4;
			}else
			{
//				ml=0;
				ml=62;
				md=1;
			}
			
			if(ml>0)
			{
				QOILZ_LzMemCpy(ct, ct-md*4, ml*4);
				ct+=ml*4;
				
//				cr=0;	cg=0;
//				cb=0;	ca=255;
//				cr=ct[-4];	cg=ct[-3];
//				cb=ct[-2];	ca=ct[-1];
				continue;
			}
		}
	}
	
	if(rxs) *rxs=xs;
	if(rys) *rys=ys;
	return(1);
}

byte *QOI_DecImageBuffer(byte *inbuf, int *rxs, int *rys)
{
	byte *imgbuf;
	int xs, ys;
	
	xs=0; ys=0;
	QOI_DecImageBufferFlat(NULL, inbuf, &xs, &ys, 0);
	if((xs<=0) || (ys<=0))
	{
		return(NULL);
	}
	
	imgbuf=malloc(xs*ys*4);
	QOI_DecImageBufferFlat(imgbuf, inbuf, &xs, &ys, 0);
	if(rxs) *rxs=xs;
	if(rys) *rys=ys;
	return(imgbuf);
}

void QOILZ_LzMemCpy(byte *dst, byte *src, int sz)
{
	byte *cs, *ct, *cte;
	u64 tv;
//	int i0, i1, i2, i3;
	int i, d;
	
	d=dst-src;

#if 1
	if(d<=0)
	{
		if(d==0)
			return;
		memmove(dst, src, sz);
		return;
	}

	if(d>sz)
	{
		if(sz<=16)
		{
			memcpy(dst, src, 16);
			return;
		}
	
		memcpy(dst, src, sz);
		return;
	}
#endif
	
	if(d>=8)
//	if(0)
	{
		cs=src; ct=dst; cte=ct+sz;
		while(ct<cte)
		{
#ifdef QOI_CASTDEREF
// #if 0
			*(u64 *)ct=*(u64 *)cs;
#else
			memcpy(ct, cs, 8);
#endif
			ct+=8; cs+=8;
		}
	}else
		if(d>=4)
	{
		cs=src; ct=dst; cte=ct+sz;
		while(ct<cte)
		{
#ifdef QOI_CASTDEREF
			((u32 *)ct)[0]=((u32 *)cs)[0];
			((u32 *)ct)[1]=((u32 *)cs)[1];
#else
			memcpy(ct+0, cs+0, 4);
			memcpy(ct+4, cs+4, 4);
#endif
			ct+=8; cs+=8;
		}
	}else
	{
		if(d==1)
		{
			if(sz<=16)
			{
				memset(dst, *src, 16);
				return;
			}
			memset(dst, *src, sz);
			return;
		}
	
		cs=src; ct=dst; cte=ct+sz;
		while(ct<cte)
		{
			ct[0]=cs[0];	ct[1]=cs[1];
			ct[2]=cs[2];	ct[3]=cs[3];
			ct[4]=cs[4];	ct[5]=cs[5];
			ct[6]=cs[6];	ct[7]=cs[7];
			ct+=8;			cs+=8;
		}
	}
}

u32 QOILZ_HashBuffer(byte *buf, int sz)
{
	u32 *bufi;
	u64 s0, s1;
	u32 v;
	int i, j, k, n;
	
#ifdef QOI_CASTDEREF
	bufi=(u32 *)buf;
	n=sz/4; s0=1; s1=1;
	for(i=0; i<n; i++)
	{
		v=bufi[i];
		s0+=v;
		s1+=s0;
	}
#else
	n=sz/4; s0=1; s1=1;
	for(i=0; i<n; i++)
	{
//		v=bufi[i];
		j=i<<2;
		v=	(buf[j+0]<< 0) |
			(buf[j+1]<< 8) |
			(buf[j+2]<<16) |
			(buf[j+3]<<24) ;
		s0+=v;
		s1+=s0;
	}
#endif
	
	s0=((u32)s0)+(s0>>32);
	s1=((u32)s1)+(s1>>32);
	s0=((u32)s0)+(s0>>32);
	s1=((u32)s1)+(s1>>32);
	v=s0^s1;

	return(v);
}

int QOILZ_UnpackLz4(byte *dst, byte *src, int dsz, int ssz)
{
	byte *ct, *cte, *cs, *cse;
	int tg, l, d, r;
	int i;
	
	ct=dst; cte=dst+dsz; cs=src; cse=src+ssz;
	while(ct<cte)
	{
		tg=*cs++;
		r=(tg>>4)&15;
		l=(tg&15)+4;
		
		if(r>=15)
		{
			i=*cs++;
			while(i==255)
			{
				r+=255;
				i=*cs++;
			}
			r+=i;
		}
		
		QOILZ_LzMemCpy(ct, cs, r);
		ct+=r;
		cs+=r;
		if((ct>=cte) || (cs>=cse))
			break;
		d=cs[0]|(cs[1]<<8);
		cs+=2;
		
		if(!d)
		{
			printf("QOILZ_UnpackLz4: EOB, Dist=0\n");
			break;
		}

		if(l>=19)
		{
			i=*cs++;
			while(i==255)
			{
				l+=255;
				i=*cs++;
			}
			l+=i;
		}

//		if((ct-dst)<1024)
//			printf("D: r=%d d=%d l=%d\n", r, d, l);

		QOILZ_LzMemCpy(ct, ct-d, l);
		ct+=l;
	}
	
	if((cs!=cse) || (ct!=cte))
	{
		printf("QOILZ_UnpackLz4: Size Issue dst=%d/%d src=%d/%d",
			(int)(ct-dst), dsz,
			(int)(cs-src), ssz);
	}
	
	return(cs-src);
}

byte *QOILZ_DecImageBuffer(byte *inbuf, int *rxs, int *rys)
{
	byte *tbuf, *i1buf;
	int sz0, sz1, sz2, xs, ys;

	if(	(inbuf[0]=='q') &&
		(inbuf[1]=='o') &&
		(inbuf[2]=='i') &&
		(inbuf[3]=='f') )
	{
		return(QOI_DecImageBuffer(inbuf, rxs, rys));
	}

	if(	(inbuf[0]=='q') &&
		(inbuf[1]=='o') &&
		(inbuf[2]=='l') &&
		(inbuf[3]=='i') )
	{
		return(QOI_DecImageBuffer(inbuf, rxs, rys));
	}

	if(	(inbuf[0]!='q') ||
		(inbuf[1]!='o') ||
		(inbuf[2]!='l') ||
		(inbuf[3]!='4') )
	{
		return(NULL);
	}
	
	sz0=(inbuf[16]<<24) |
		(inbuf[17]<<16) |
		(inbuf[18]<< 8) |
		(inbuf[19]<< 0) ;

	sz2=(inbuf[20]<<24) |
		(inbuf[21]<<16) |
		(inbuf[22]<< 8) |
		(inbuf[23]<< 0) ;

	sz1=14+sz0;
	tbuf=malloc(sz1+256);
	memcpy(tbuf, inbuf, 14);
	tbuf[2]='i';
	tbuf[3]='f';
	
	QOILZ_UnpackLz4(tbuf+14, inbuf+24, sz0, sz2);

	printf("Dec Hash %08X\n", QOILZ_HashBuffer(tbuf+14, sz0));

	i1buf=QOI_DecImageBuffer(tbuf, &xs, &ys);
	if(!i1buf)
		return(NULL);
	
	*rxs=xs;
	*rys=ys;
	return(i1buf);
}

int QOILZ_DecImageBufferFlat(
	byte *outbuf, byte *inbuf, int *rxs, int *rys, int flag)
{
	byte *tbuf, *i1buf;
	int sz0, sz1, sz2, xs, ys;
	int i;

	if(	(inbuf[0]!='q') ||
		(inbuf[1]!='o') )
	{
		return(-1);
	}

	if(	(inbuf[2]=='i') &&
		(inbuf[3]=='f') )
	{
		return(QOI_DecImageBufferFlat(outbuf, inbuf, rxs, rys, flag));
	}

	if(	(inbuf[2]=='l') &&
		(inbuf[3]=='i') )
	{
		return(QOI_DecImageBufferFlat(outbuf, inbuf, rxs, rys, flag));
	}

	if(	(inbuf[0]!='q') ||
		(inbuf[1]!='o') ||
		(inbuf[2]!='l') ||
		(inbuf[3]!='4') )
	{
		return(-1);
	}
	
	sz0=(inbuf[16]<<24) |
		(inbuf[17]<<16) |
		(inbuf[18]<< 8) |
		(inbuf[19]<< 0) ;

	sz2=(inbuf[20]<<24) |
		(inbuf[21]<<16) |
		(inbuf[22]<< 8) |
		(inbuf[23]<< 0) ;

	sz1=14+sz0;
	tbuf=malloc(sz1+256);
	memcpy(tbuf, inbuf, 14);
	tbuf[2]='i';
	tbuf[3]='f';
	
	QOILZ_UnpackLz4(tbuf+14, inbuf+24, sz0, sz2);

	printf("Dec Hash %08X\n", QOILZ_HashBuffer(tbuf+14, sz0));

	i=QOI_DecImageBufferFlat(outbuf, tbuf, &xs, &ys, flag);
	if(i<0)
		return(i);
	
	if(rxs)		*rxs=xs;
	if(rxs)		*rys=ys;
	return(i);
}

u16 *QOILZ_DecImageBuffer555(byte *inbuf, int *rxs, int *rys)
{
	byte *i1buf;
	u16 *i2buf, *ct;
	byte *cs, *cse;
	int cr, cg, cb;
	int x, y, xs, ys, n, v;
	
	i1buf=QOILZ_DecImageBuffer(inbuf, &xs, &ys);
	if(!i1buf)
		return(NULL);
		
	n=xs*ys;
	i2buf=malloc(xs*ys*2);
	
	cs=i1buf; cse=i1buf+n*4;
	ct=i2buf;

#if 0
	while(cs<cse)
	{
		cr=cs[0];	cg=cs[1];
		cb=cs[2];	cs+=4;
		
		cr>>=3;	cg>>=3;	cb>>=3;
		v=(cr<10)|(cg<<5)|cb;
		*ct++=v;
	}
#endif

#if 1
	for(y=0; y<ys; y++)
	{
		cs=i1buf+(ys-y-1)*xs*4;
		for(x=0; x<xs; x++)
		{
			cr=cs[0];	cg=cs[1];
			cb=cs[2];	cs+=4;
			
			cr>>=3;	cg>>=3;	cb>>=3;
			v=(cr<<10)|(cg<<5)|cb;
			*ct++=v;
		}
	}
#endif
	
	free(i1buf);
	
	*rxs=xs;
	*rys=ys;
	return(i2buf);
}



int QOI_DecImageBufferFlat555(u16 *dstbuf, byte *inbuf, int *rxs, int *rys)
{
	byte pixtab[64*4];
	u16 *ct, *cte;
	byte *cs;
	int cr, cg, cb, ca, qoli;
	int xs, ys, n, ml, md;
	int i, j, k, l, h, px;
	
	qoli=0;
	if(	(inbuf[0]!='q') ||
		(inbuf[1]!='o') )
	{
		return(-1);
	}

	if(	(inbuf[2]!='i') ||
		(inbuf[3]!='f') )
	{
		if(	(inbuf[2]!='l') ||
			(inbuf[3]!='i') )
		{
			return(-1);
		}
		qoli=1;
	}

	xs=	(inbuf[ 4]<<24) |
		(inbuf[ 5]<<16) |
		(inbuf[ 6]<< 8) |
		(inbuf[ 7]<< 0) ;
	ys=	(inbuf[ 8]<<24) |
		(inbuf[ 9]<<16) |
		(inbuf[10]<< 8) |
		(inbuf[11]<< 0) ;
	cs=inbuf+14;

	if(!dstbuf)
	{
		*rxs=xs;
		*rys=ys;
		return(0);
	}

	n=xs*ys;
	ct=dstbuf;
	cte=ct+n*4;
	
	for(i=0; i<256; i++)
		pixtab[i]=0;
	
	cr=0;	cg=0;
	cb=0;	ca=255;
	while(ct<cte)
	{
		j=*cs++;
		i=j>>6;

		if((j==0) && (cs[0]==0))
			break;

		if(i==0)
		{
			k=j&63;
			k=k<<2;
			cr=pixtab[k+0];	cg=pixtab[k+1];
			cb=pixtab[k+2];	ca=pixtab[k+3];
			px=((cr>>3)<<10)|((cg>>3)<<5)|((cb>>3)<<0);
			if(ca<128) { px&=0x7BDE; px|=0x8000; }
			*ct++=px;
			continue;
		}

		if(i==1)
		{
			cr+=((j>>4)&3)-2;
			cg+=((j>>2)&3)-2;
			cb+=((j>>0)&3)-2;
			h=(cr*3+cg*5+cb*7+ca*11)&63;
			k=h<<2;
			pixtab[k+0]=cr;	pixtab[k+1]=cg;
			pixtab[k+2]=cb;	pixtab[k+3]=ca;
			px=((cr>>3)<<10)|((cg>>3)<<5)|((cb>>3)<<0);
			if(ca<128) { px&=0x7BDE; px|=0x8000; }
			*ct++=px;
			continue;
		}

		if(i==2)
		{
			k=(j&63)-32;
			cr+=k;
			cg+=k;
			cb+=k;

			j=*cs++;
			cr+=((j>>4)&15)-8;
			cb+=((j>>0)&15)-8;

			h=(cr*3+cg*5+cb*7+ca*11)&63;
			k=h<<2;
			pixtab[k+0]=cr;	pixtab[k+1]=cg;
			pixtab[k+2]=cb;	pixtab[k+3]=ca;
			px=((cr>>3)<<10)|((cg>>3)<<5)|((cb>>3)<<0);
			if(ca<128) { px&=0x7BDE; px|=0x8000; }
			*ct++=px;
			continue;
		}

		k=j&63;
//		if(k<62)
		if((k<48) || (!qoli && (k<62)))
		{
			k++;
			while(k--)
			{
				px=((cr>>3)<<10)|((cg>>3)<<5)|((cb>>3)<<0);
				if(ca<128) { px&=0x7BDE; px|=0x8000; }
				*ct++=px;
			}
			continue;
		}
		
		if(j==0xFE)
		{
			cr=cs[0];
			cg=cs[1];
			cb=cs[2];
			cs+=3;

			h=(cr*3+cg*5+cb*7+ca*11)&63;
			k=h<<2;
			pixtab[k+0]=cr;	pixtab[k+1]=cg;
			pixtab[k+2]=cb;	pixtab[k+3]=ca;
			px=((cr>>3)<<10)|((cg>>3)<<5)|((cb>>3)<<0);
			if(ca<128) { px&=0x7BDE; px|=0x8000; }
			*ct++=px;
			continue;
		}

		
		if(j==0xFF)
		{
			cr=cs[0];
			cg=cs[1];
			cb=cs[2];
			ca=cs[3];
			cs+=4;

			h=(cr*3+cg*5+cb*7+ca*11)&63;
			k=h<<2;
			pixtab[k+0]=cr;	pixtab[k+1]=cg;
			pixtab[k+2]=cb;	pixtab[k+3]=ca;
			px=((cr>>3)<<10)|((cg>>3)<<5)|((cb>>3)<<0);
			if(ca<128) { px&=0x7BDE; px|=0x8000; }
			*ct++=px;
			continue;
		}
		
		if(qoli)
		{
			if((j&0xF8)==0xF0)
			{
				k=cs[0];
				l=cs[1];
				cs+=2;
				ml=((j&7)<<4)|(k>>4);
				md=((k&15)<<8)|l;
				ml+=4;
			}
			else if((j&0xFC)==0xF8)
			{
				k=cs[0];
				l=cs[1];
				ml=((j&3)<<8)|k;
				md=(l<<8)|cs[2];
				cs+=3;
				ml+=4;
			}else if((j&0xFF)==0xFC)
			{
				k=cs[0];
				l=cs[1];
				ml=(k<<4)|(l>>4);
				md=((l&15)<<16)|(cs[2]<<8)|cs[3];
				cs+=4;
				ml+=4;
			}else
			{
//				ml=0;
				ml=62;
				md=1;
			}
			
			if(ml>0)
			{
				QOILZ_LzMemCpy((byte *)ct, (byte *)(ct-md), ml*2);
				ct+=ml;
//				cr=0;	cg=0;
//				cb=0;	ca=255;

#if 0
				px=ct[-1];
				cr=(px>>10)&31;
				cg=(px>> 5)&31;
				cb=(px>> 0)&31;
				cr=(cr<<3)|(cr>>2);
				cg=(cg<<3)|(cg>>2);
				cb=(cb<<3)|(cb>>2);
				ca=(px&1)?0:255;
#endif

//				cr=ct[-4];	cg=ct[-3];
//				cb=ct[-2];	ca=ct[-1];
				continue;
			}
		}
	}
	
	*rxs=xs;
	*rys=ys;
	return(1);
}
