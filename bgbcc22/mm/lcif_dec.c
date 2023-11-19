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

/*
 * LCIF_CASTDEREF: Flag whether or not it is safe to cast and deref.
 * This generally works fine on MSVC and BGBCC.
 * But, GCC and Clang may cause incorrect behavior.
 * However, they are better able to optimize memcpy(), so...
 *
 * LCIF_SMALLSTACK: Flag if the stack is small.
 * Say: If the target has 64K or 128K stack, don't put big arrays there.
 * But, say, for MSVC, we have 1MB, so...
 */

#ifdef _MSC_VER
#define LCIF_CASTDEREF
#endif

#ifdef __BGBCC__
#define LCIF_CASTDEREF
#define LCIF_SMALLSTACK
#endif

void LCIF_LzMemCpy(byte *dst, byte *src, int sz);

byte *LCIF_DecColorPlane(byte *outbuf, byte *inbuf, int xs, int ys)
{
//	byte pixtab[64*4];
	byte pixtab_y[64];
	byte pixtab_u[64];
	byte pixtab_v[64];
	byte pixtab_a[64];
	byte *cs, *ct, *cte;
	int cr, cg, cb, ca, qoli;
	int n, ml, md;
	int i, j, k, l, h;

	cs=inbuf;

	n=xs*ys;
	ct=outbuf;
	cte=ct+n*4;
	
//	for(i=0; i<256; i++)
//		pixtab[i]=0;

	for(i=0; i<64; i++)
	{
		pixtab_y[i]=0;
		pixtab_u[i]=0;
		pixtab_v[i]=0;
		pixtab_a[i]=0;
	}
	
	cr=0;	cg=0;
	cb=0;	ca=255;
	while(ct<cte)
	{
		j=*cs++;
		i=j>>6;

		if(i==0)
		{
			k=j&63;
//			k=k<<2;
//			cr=pixtab[k+0];	cg=pixtab[k+1];
//			cb=pixtab[k+2];	ca=pixtab[k+3];
			cr=pixtab_y[k];	cg=pixtab_u[k];
			cb=pixtab_v[k];	ca=pixtab_a[k];
			ct[0]=cr;	ct[1]=cg;
			ct[2]=cb;	ct[3]=ca;
			ct+=4;
			continue;
		}

		if(i==1)
		{
			cr+=((j>>4)&3)-2;
			cg+=((j>>2)&3)-2;
			cb+=((j>>0)&3)-2;
			h=(cr*3+cg*5+cb*7+ca*11)&63;
//			h=(cr*3+cg*9+cb*5+ca)&63;
			k=h<<2;
//			pixtab[k+0]=cr;	pixtab[k+1]=cg;
//			pixtab[k+2]=cb;	pixtab[k+3]=ca;
			pixtab_y[h]=cr;	pixtab_u[h]=cg;
			pixtab_v[h]=cb;	pixtab_a[h]=ca;
			ct[0]=cr;	ct[1]=cg;
			ct[2]=cb;	ct[3]=ca;
			ct+=4;
			continue;
		}

		if(i==2)
		{
			k=(j&63)-32;
			cr+=k;
//			cg+=k;
//			cb+=k;

			j=*cs++;
//			cr+=((j>>4)&15)-8;
//			cb+=((j>>0)&15)-8;
			cg+=((j>>4)&15)-8;
			cb+=((j>>0)&15)-8;

			h=(cr*3+cg*5+cb*7+ca*11)&63;
//			h=(cr*3+cg*9+cb*5+ca)&63;
			k=h<<2;
//			pixtab[k+0]=cr;	pixtab[k+1]=cg;
//			pixtab[k+2]=cb;	pixtab[k+3]=ca;
			pixtab_y[h]=cr;	pixtab_u[h]=cg;
			pixtab_v[h]=cb;	pixtab_a[h]=ca;
			ct[0]=cr;	ct[1]=cg;
			ct[2]=cb;	ct[3]=ca;
			ct+=4;
			continue;
		}

		k=j&63;
		if(k<48)
		{
			k++;
#ifdef LCIF_CASTDEREF
// #if 0
			l=cr|(cg<<8)|(cb<<16)|(ca<<24);
			while(k>=4)
			{
				((u32 *)ct)[0]=l;
				((u32 *)ct)[1]=l;
				((u32 *)ct)[2]=l;
				((u32 *)ct)[3]=l;
				k-=4;
				ct+=16;
			}
			while(k--)
			{
				*(u32 *)ct=l;
				ct+=4;
			}
#else
			while(k--)
			{
				ct[0]=cr;	ct[1]=cg;
				ct[2]=cb;	ct[3]=ca;
				ct+=4;
			}
#endif
			continue;
		}
		
		if(j==0xFD)
		{
			k=64;
#ifdef LCIF_CASTDEREF
// #if 0
			l=cr|(cg<<8)|(cb<<16)|(ca<<24);
			while(k>0)
			{
				((u32 *)ct)[0]=l;
				((u32 *)ct)[1]=l;
				((u32 *)ct)[2]=l;
				((u32 *)ct)[3]=l;
				k-=4;	ct+=16;
			}
#else
			while(k--)
			{
				ct[0]=cr;	ct[1]=cg;
				ct[2]=cb;	ct[3]=ca;
				ct+=4;
			}
#endif
			continue;
		}
		
		if(j==0xFE)
		{
			cr=cs[0];
			cg=cs[1];
			cb=cs[2];
			cs+=3;

			h=(cr*3+cg*5+cb*7+ca*11)&63;
//			h=(cr*3+cg*9+cb*5+ca)&63;
			k=h<<2;
//			pixtab[k+0]=cr;	pixtab[k+1]=cg;
//			pixtab[k+2]=cb;	pixtab[k+3]=ca;
			pixtab_y[h]=cr;	pixtab_u[h]=cg;
			pixtab_v[h]=cb;	pixtab_a[h]=ca;
			ct[0]=cr;	ct[1]=cg;
			ct[2]=cb;	ct[3]=ca;
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
//			h=(cr*3+cg*9+cb*5+ca)&63;
			k=h<<2;
//			pixtab[k+0]=cr;	pixtab[k+1]=cg;
//			pixtab[k+2]=cb;	pixtab[k+3]=ca;
			pixtab_y[h]=cr;	pixtab_u[h]=cg;
			pixtab_v[h]=cb;	pixtab_a[h]=ca;
			ct[0]=cr;	ct[1]=cg;
			ct[2]=cb;	ct[3]=ca;
			ct+=4;
			continue;
		}
		
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
			ml=62;
			md=1;
		}
		
		if(ml>0)
		{
			LCIF_LzMemCpy(ct, ct-md*4, ml*4);
			ct+=ml*4;
			
//			cr=0;	cg=0;
//			cb=0;	ca=255;
			continue;
		}
	}
	
	return(cs);
}

void LCIF_LzMemCpy(byte *dst, byte *src, int sz)
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
#ifdef LCIF_CASTDEREF
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
#ifdef LCIF_CASTDEREF
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

u32 LCIF_HashBuffer(byte *buf, int sz)
{
	u32 *bufi;
	u64 s0, s1;
	u32 v;
	int i, j, k, n;
	
#ifdef LCIF_CASTDEREF
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

#if 0
int LCIF_UnpackLz4(byte *dst, byte *src, int dsz, int ssz)
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
		
		LCIF_LzMemCpy(ct, cs, r);
		ct+=r;
		cs+=r;
		if((ct>=cte) || (cs>=cse))
			break;
		d=cs[0]|(cs[1]<<8);
		cs+=2;
		
		if(!d)
		{
			printf("LCIF_UnpackLz4: EOB, Dist=0\n");
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

		LCIF_LzMemCpy(ct, ct-d, l);
		ct+=l;
	}
	
	if((cs!=cse) || (ct!=cte))
	{
		printf("LCIF_UnpackLz4: Size Issue dst=%d/%d src=%d/%d",
			ct-dst, dsz,
			cs-src, ssz);
	}
	
	return(cs-src);
}
#endif

u64 LCIF_ExpandCell_2x2x2(int v0)
{
	static u16 exptab[16]={
		0x000, 0x012, 0x02D, 0x03F,
		0x480, 0x492, 0x4AD, 0x4BF,
		0xB40, 0xB52, 0xB6D, 0xB7F,
		0xFC0, 0xFD2, 0xFED, 0xFFF
	};
	u64 v1;
	int i0, i1, i2, i3;

	i0=(v0>>4)&15;	i1=(v0>>0)&15;
	i0=exptab[i0];
	i1=exptab[i1];
	v1=0x6969;
	v1=(v1<<12)	| i0;
	v1=(v1<<12)	| i0;
	v1=(v1<<12)	| i1;
	v1=(v1<<12)	| i1;
	return(v1);
}

u64 LCIF_ExpandCell_4x4x1(int v0)
{
	static u16 exptab[16]={
		0x000, 0x007, 0x038, 0x03F,
		0x1C0, 0x1C7, 0x1F8, 0x1FF,
		0xE00, 0xE07, 0xE38, 0xE3F,
		0xFC0, 0xFC7, 0xFF8, 0xFFF };
	u64 v1;
	int i0, i1, i2, i3;

	i0=(v0>>12)&15;		i1=(v0>>8)&15;
	i2=(v0>> 4)&15;		i3=(v0>>0)&15;
	i0=exptab[i0];		i1=exptab[i1];
	i2=exptab[i2];		i3=exptab[i3];
	v1=0x6969;
	v1=(v1<<12)	| i0;
	v1=(v1<<12)	| i1;
	v1=(v1<<12)	| i2;
	v1=(v1<<12)	| i3;
	return(v1);
}

static u16 lcif_exptab_4x4x2[256];
static u64 lcif_pattab_2x2x2[256];
static u64 lcif_pattab_fixed[64];
static u16 lcif_pattab_fixed16b[64];

static u64 lcif_pattab_grad[8];
static u32 lcif_pattab_grad0[4]={
	0x1B1B1B1B,	0x0055AAFF,
	0x051A6B6F,	0x6F6B1A05 };

u64 LCIF_ExpandCell_4x4x2(u32 v0)
{
	u64 v1;
	int i0, i1, i2, i3;
	int i, j;
	
	i0=(v0>>24)&255;	i1=(v0>>16)&255;
	i2=(v0>> 8)&255;	i3=(v0>> 0)&255;
	i0=lcif_exptab_4x4x2[i0];		i1=lcif_exptab_4x4x2[i1];
	i2=lcif_exptab_4x4x2[i2];		i3=lcif_exptab_4x4x2[i3];
	v1=0x6969;
	v1=(v1<<12)	| i0;
	v1=(v1<<12)	| i1;
	v1=(v1<<12)	| i2;
	v1=(v1<<12)	| i3;
	return(v1);
}

char lcif_pat6gen[8*4]={
	 2,  2,  2,  2,
	 4,  2, -2, -4,
	 4, -1,  1, -4,
	 4, -4,  4, -4,
	-2, -2, -2, -2,
	-4, -2,  2,  4,
	-4,  1, -1,  4,
	-4,  4, -4,  4
};


int LCIF_ExpandCell_Init()
{
	int i0, i1, i2, i3;
	int ph0, ph1, ph2, ph3;
	int pv0, pv1, pv2, pv3;
	int p0, p1, p2, p3;
	u32 px;
//	int cr, cg, cb, ca, qoli;
//	int n, ml, md;
	int i, j, k, l, h;

	if(lcif_exptab_4x4x2[255])
		return(0);

	for(i=0; i<256; i++)
	{
		i0=(i>>6)&3;	i1=(i>>4)&3;
		i2=(i>>2)&3;	i3=(i>>0)&3;
		i0=(i0<<1)|(i0>>1);
		i1=(i1<<1)|(i1>>1);
		i2=(i2<<1)|(i2>>1);
		i3=(i3<<1)|(i3>>1);
		j=(i0<<9)|(i1<<6)|(i2<<3)|i3;
		lcif_exptab_4x4x2[i]=j;
	}

	for(i=0; i<256; i++)
	{
		lcif_pattab_2x2x2[i]=LCIF_ExpandCell_2x2x2(i);
	}
	
	lcif_pattab_grad[0]=LCIF_ExpandCell_4x4x2( lcif_pattab_grad0[0]);
	lcif_pattab_grad[1]=LCIF_ExpandCell_4x4x2(~lcif_pattab_grad0[0]);
	lcif_pattab_grad[2]=LCIF_ExpandCell_4x4x2( lcif_pattab_grad0[1]);
	lcif_pattab_grad[3]=LCIF_ExpandCell_4x4x2(~lcif_pattab_grad0[1]);
	lcif_pattab_grad[4]=LCIF_ExpandCell_4x4x2( lcif_pattab_grad0[2]);
	lcif_pattab_grad[5]=LCIF_ExpandCell_4x4x2(~lcif_pattab_grad0[2]);
	lcif_pattab_grad[6]=LCIF_ExpandCell_4x4x2( lcif_pattab_grad0[3]);
	lcif_pattab_grad[7]=LCIF_ExpandCell_4x4x2(~lcif_pattab_grad0[3]);

	for(i0=0; i0<8; i0++)
		for(i1=0; i1<8; i1++)
	{
		ph0=lcif_pat6gen[i1*4+0];
		ph1=lcif_pat6gen[i1*4+1];
		ph2=lcif_pat6gen[i1*4+2];
		ph3=lcif_pat6gen[i1*4+3];

		pv0=lcif_pat6gen[i0*4+0];
		pv1=lcif_pat6gen[i0*4+1];
		pv2=lcif_pat6gen[i0*4+2];
		pv3=lcif_pat6gen[i0*4+3];
	
		px=0;
		p0=ph0+pv0;		p1=ph1+pv0;
		p2=ph2+pv0;		p3=ph3+pv0;
		if(p0>=0)px|=0x0001;
		if(p1>=0)px|=0x0002;
		if(p2>=0)px|=0x0004;
		if(p3>=0)px|=0x0008;
		p0=ph0+pv1;		p1=ph1+pv1;
		p2=ph2+pv1;		p3=ph3+pv1;
		if(p0>=0)px|=0x0010;
		if(p1>=0)px|=0x0020;
		if(p2>=0)px|=0x0040;
		if(p3>=0)px|=0x0080;
		p0=ph0+pv2;		p1=ph1+pv2;
		p2=ph2+pv2;		p3=ph3+pv2;
		if(p0>=0)px|=0x0100;
		if(p1>=0)px|=0x0200;
		if(p2>=0)px|=0x0400;
		if(p3>=0)px|=0x0800;
		p0=ph0+pv3;		p1=ph1+pv3;
		p2=ph2+pv3;		p3=ph3+pv3;
		if(p0>=0)px|=0x1000;
		if(p1>=0)px|=0x2000;
		if(p2>=0)px|=0x4000;
		if(p3>=0)px|=0x8000;
		lcif_pattab_fixed[i0*8+i1]=LCIF_ExpandCell_4x4x1(px);
		lcif_pattab_fixed16b[i0*8+i1]=px;
	}
	return(0);
}

byte *LCIF_DecBlockPlane(u64 *outbuf, byte *inbuf, int xs, int ys)
{
	u64 blktab[64];
	u64 *ct, *cte;
	byte *cs;
	u64 v0, v1;
	int i0, i1, i2, i3;
	int cr, cg, cb, ca, qoli;
	int n, ml, md;
	int i, j, k, l, h;

	cs=inbuf;

	LCIF_ExpandCell_Init();

	n=xs*ys;
	ct=outbuf;
	cte=ct+n;
	
	for(i=0; i<64; i++)
		blktab[i]=0;
	
	while(ct<cte)
	{
		j=*cs++;
		i=j>>6;

		if(i==0)
		{
			*ct++=blktab[j&63];
			continue;
		}

		if(i==1)
		{
			*ct++=lcif_pattab_fixed[j&63];
			continue;
		}

		if(i==2)
		{
			i=(j>>4)&3;
			l=(j&15)+1;

			if(i==0)
			{
				while(l--)
					*ct++=0;
				continue;
			}

			if(i==1)
			{
				while(l--)
				{
					v0=lcif_pattab_2x2x2[*cs++];
					*ct++=v0;
//					*ct++=v0^(3ULL<<48);
					v1=(v0<<5)^(v0>>31);
					v1=(v1<<3)^(v1>>17);
					v1=(v1<<1)^(v1>>11);
					h=(v1^(v1>>7))&63;
					blktab[h]=v0;
//					blktab[h]=v0^(3ULL<<48);
				}
				continue;
			}

			if(i==2)
			{
				while(l--)
				{
					j=cs[0]|(cs[1]<<8);
					cs+=2;
					v0=LCIF_ExpandCell_4x4x1(j);
					*ct++=v0;
					v1=(v0<<5)^(v0>>31);
					v1=(v1<<3)^(v1>>17);
					v1=(v1<<1)^(v1>>11);
					h=(v1^(v1>>7))&63;
					blktab[h]=v0;
				}
				continue;
			}

			if(i==3)
			{
				while(l--)
				{
					j=cs[0]|(cs[1]<<8)|(cs[2]<<16)|(cs[3]<<24);
					cs+=4;
					v0=LCIF_ExpandCell_4x4x2(j);
					*ct++=v0;
					v1=(v0<<5)^(v0>>31);
					v1=(v1<<3)^(v1>>17);
					v1=(v1<<1)^(v1>>11);
					h=(v1^(v1>>7))&63;
					blktab[h]=v0;
				}
				continue;
			}
		}
		
		if(j==0xFD)
		{
			l=64;
			while(l--)
				*ct++=0;
			continue;
		}
		
		if((j&0xF8)==0xE0)
		{
			*ct++=lcif_pattab_grad[j&7];
			continue;
		}

		if((j&0xF8)==0xE8)
		{
			l=(j&7)+1;
			while(l--)
			{
				v0=				(cs[7]<<8)|(cs[6]<<0);
				v0=(v0<<16) |	(cs[5]<<8)|(cs[4]<<0);
				v0=(v0<<16) |	(cs[3]<<8)|(cs[2]<<0);
				v0=(v0<<16) |	(cs[1]<<8)|(cs[0]<<0);
				cs+=8;
				*ct++=v0;

				v1=(v0<<5)^(v0>>31);
				v1=(v1<<3)^(v1>>17);
				v1=(v1<<1)^(v1>>11);
				h=(v1^(v1>>7))&63;
				blktab[h]=v0;
			}
			continue;
		}

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
			ml=64;
			md=1;
		}
		
		if(ml>0)
		{
			LCIF_LzMemCpy((byte *)ct, (byte *)(ct-md), ml*8);
			ct+=ml;
			continue;
		}
	}
	
	return(cs);
}

int LCIF_UnpackCellBlockPixels(byte *dst, int xstr,
	u64 blk, byte *yuv0, byte *yuv1, int clrs)
{
	u32 ytab[8];
	byte cytab[8];
	byte cutab[4];
	byte cvtab[4];
	u32 *ct;
	u32 px, px1, px2, px3;
	int cy0, cu0, cv0, cy1, cu1, cv1;
	int cr0, cg0, cb0, cr1, cg1, cb1;
	int cu2, cv2, cu3, cv3;
	int cy, cu, cv, ycocg;
	int cr, cg, cb;
	int i, j, k, x, y;

	ycocg=clrs&1;

	if(!blk)
	{
		cy0=yuv0[0];	cu0=yuv0[1];	cv0=yuv0[2];
//		cy1=yuv1[0];	cu1=yuv1[1];	cv1=yuv1[2];
//		cy0=(cy0+cy1)>>1;	cu0=(cu0+cu1)>>1;	cv0=(cv0+cv1)>>1;

#if 0
		cb0=((cu0-128)<<1)+cy0;
		cr0=((cv0-128)<<1)+cy0;
//		cg0=(cy0*4-cr0-cb0)>>1;
		cg0=((cy0<<4)-5*cr0-3*cb0)>>3;
//		cg0=cy0;
#endif

		if(ycocg)
//		if(1)
		{
			cu2=(cu0-128);
			cv2=(cv0-128);
			k=cy0-cu2;
			cr0=k+cv2;
			cg0=cy0+cu2;
			cb0=k-cv2;
		}else
		{
			cu2=(cu0-128)<<1;
			cv2=(cv0-128)<<1;
			cb0=cu2+cy0;
			cr0=cv2+cy0;
			cg0=(8*cy0-5*cv2-3*cu2)>>3;
		}

		px=0xFF000000|(cr0<<16)|(cg0<<8)|cb0;
		
		ct=(u32 *)dst;

		ct[0]=px;	ct[1]=px;	ct[2]=px;	ct[3]=px;
		ct+=xstr;
		ct[0]=px;	ct[1]=px;	ct[2]=px;	ct[3]=px;
		ct+=xstr;
		ct[0]=px;	ct[1]=px;	ct[2]=px;	ct[3]=px;
		ct+=xstr;
		ct[0]=px;	ct[1]=px;	ct[2]=px;	ct[3]=px;
//		ct+=xstr;

#if 0
		for(y=0; y<4; y++)
		{
			ct[0]=px;
			ct[1]=px;
			ct[2]=px;
			ct[3]=px;
			ct+=xstr;
		}
#endif
		
		return(0);
	}

	if((blk>>48)==0x6969)
	{
		cy0=yuv0[0];	cu0=yuv0[1];	cv0=yuv0[2];
		cy1=yuv1[0];	cu1=yuv1[1];	cv1=yuv1[2];
		
//		cb0=((cu0-128)<<1)+cy0;
//		cr0=((cv0-128)<<1)+cy0;
//		cg0=((cy0<<4)-5*cr0-3*cb0)>>3;
//		cg0=cy0;

		if(ycocg)
//		if(1)
		{
			cu2=(cu0-128);
			cv2=(cv0-128);

			cu3=(cu1-128);
			cv3=(cv1-128);

			k=cy0-cu2;
			cr0=k+cv2;
			cg0=cy0+cu2;
			cb0=k-cv2;

			k=cy1-cu3;
			cr1=k+cv3;
			cg1=cy1+cu3;
			cb1=k-cv3;
		}else
		{
			cu2=(cu0-128)<<1;
			cv2=(cv0-128)<<1;

			cu3=(cu1-128)<<1;
			cv3=(cv1-128)<<1;

			cb0=cu2+cy0;
			cr0=cv2+cy0;
			cg0=(8*cy0-5*cv2-3*cu2)>>3;

			cb1=cu3+cy1;
			cr1=cv3+cy1;
			cg1=(8*cy1-5*cv3-3*cu3)>>3;
		}

//		cb1=((cu1-128)<<1)+cy1;
//		cr1=((cv1-128)<<1)+cy1;
//		cg1=((cy1<<4)-5*cr1-3*cb1)>>3;
//		cg1=cy1;

		px=0xFF000000|(cr0<<16)|(cg0<<8)|cb0;
		ytab[0]=px;
		px=0xFF000000|(cr1<<16)|(cg1<<8)|cb1;
		ytab[3]=px;

		cr=(cr0*5+cr1*3)>>3;
		cg=(cg0*5+cg1*3)>>3;
		cb=(cb0*5+cb1*3)>>3;
		px=0xFF000000|(cr<<16)|(cg<<8)|cb;
		ytab[1]=px;

		cr=(cr0*3+cr1*5)>>3;
		cg=(cg0*3+cg1*5)>>3;
		cb=(cb0*3+cb1*5)>>3;
		px=0xFF000000|(cr<<16)|(cg<<8)|cb;
		ytab[2]=px;
		
		ct=(u32 *)dst;

#if 1
		i=blk>>(0*12);
		ct[0]=ytab[(i>> 1)&3];	ct[1]=ytab[(i>> 4)&3];
		ct[2]=ytab[(i>> 7)&3];	ct[3]=ytab[(i>>10)&3];
		ct+=xstr;
		i=blk>>(1*12);
		ct[0]=ytab[(i>> 1)&3];	ct[1]=ytab[(i>> 4)&3];
		ct[2]=ytab[(i>> 7)&3];	ct[3]=ytab[(i>>10)&3];
		ct+=xstr;
		i=blk>>(2*12);
		ct[0]=ytab[(i>> 1)&3];	ct[1]=ytab[(i>> 4)&3];
		ct[2]=ytab[(i>> 7)&3];	ct[3]=ytab[(i>>10)&3];
		ct+=xstr;
		i=blk>>(3*12);
		ct[0]=ytab[(i>> 1)&3];	ct[1]=ytab[(i>> 4)&3];
		ct[2]=ytab[(i>> 7)&3];	ct[3]=ytab[(i>>10)&3];
//		ct+=xstr;
#endif

#if 0
		ct[0]=ytab[(blk>>(0*12+ 1))&3];	ct[1]=ytab[(blk>>(0*12+ 4))&3];
		ct[2]=ytab[(blk>>(0*12+ 7))&3];	ct[3]=ytab[(blk>>(0*12+10))&3];
		ct+=xstr;
		ct[0]=ytab[(blk>>(1*12+ 1))&3];	ct[1]=ytab[(blk>>(1*12+ 4))&3];
		ct[2]=ytab[(blk>>(1*12+ 7))&3];	ct[3]=ytab[(blk>>(1*12+10))&3];
		ct+=xstr;
		ct[0]=ytab[(blk>>(2*12+ 1))&3];	ct[1]=ytab[(blk>>(2*12+ 4))&3];
		ct[2]=ytab[(blk>>(2*12+ 7))&3];	ct[3]=ytab[(blk>>(2*12+10))&3];
		ct+=xstr;
		ct[0]=ytab[(blk>>(3*12+ 1))&3];	ct[1]=ytab[(blk>>(3*12+ 4))&3];
		ct[2]=ytab[(blk>>(3*12+ 7))&3];	ct[3]=ytab[(blk>>(3*12+10))&3];
//		ct+=xstr;
#endif

#if 0
		for(y=0; y<4; y++)
		{
			i=blk>>(y*12);
//			ct[0]=ytab[(i>>0)&7];
//			ct[1]=ytab[(i>>3)&7];
//			ct[2]=ytab[(i>>6)&7];
//			ct[3]=ytab[(i>>9)&7];

			ct[0]=ytab[(i>> 1)&3];
			ct[1]=ytab[(i>> 4)&3];
			ct[2]=ytab[(i>> 7)&3];
			ct[3]=ytab[(i>>10)&3];
			ct+=xstr;
		}
#endif
		
		return(0);
	}

#if 0
	if((blk>>48)==0x696A)
	{
		cy0=yuv0[0];	cu0=yuv0[1];	cv0=yuv0[2];
		cy1=yuv1[0];	cu1=yuv1[1];	cv1=yuv1[2];
		
		if(ycocg)
//		if(1)
		{
			cu2=(cu0-128);
			cv2=(cv0-128);

			cu3=(cu1-128);
			cv3=(cv1-128);

			k=cy0-cu2;
			cr0=k+cv2;
			cg0=cy0+cu2;
			cb0=k-cv2;

			k=cy1-cu3;
			cr1=k+cv3;
			cg1=cy1+cu3;
			cb1=k-cv3;
		}else
		{
			cu2=(cu0-128)<<1;
			cv2=(cv0-128)<<1;

			cu3=(cu1-128)<<1;
			cv3=(cv1-128)<<1;

			cb0=cu2+cy0;
			cr0=cv2+cy0;
			cg0=(8*cy0-5*cv2-3*cu2)>>3;

			cb1=cu3+cy1;
			cr1=cv3+cy1;
			cg1=(8*cy1-5*cv3-3*cu3)>>3;
		}

		px=0xFF000000|(cr0<<16)|(cg0<<8)|cb0;
		ytab[0]=px;
		px=0xFF000000|(cr1<<16)|(cg1<<8)|cb1;
		ytab[3]=px;

		cr=(cr0*5+cr1*3)>>3;
		cg=(cg0*5+cg1*3)>>3;
		cb=(cb0*5+cb1*3)>>3;
		px=0xFF000000|(cr<<16)|(cg<<8)|cb;
		ytab[1]=px;

		cr=(cr0*3+cr1*5)>>3;
		cg=(cg0*3+cg1*5)>>3;
		cb=(cb0*3+cb1*5)>>3;
		px=0xFF000000|(cr<<16)|(cg<<8)|cb;
		ytab[2]=px;
		
		ct=(u32 *)dst;

#if 1
		i=blk>>(0*12);
		px =ytab[(i>> 1)&3];	px1=ytab[(i>> 7)&3];
		i=blk>>(2*12);
		px2=ytab[(i>> 1)&3];	px3=ytab[(i>> 7)&3];
		ct[0]=px;	ct[1]=px;	ct[2]=px1;	ct[3]=px1;
		ct+=xstr;
		ct[0]=px;	ct[1]=px;	ct[2]=px1;	ct[3]=px1;
		ct+=xstr;
		ct[0]=px2;	ct[1]=px2;	ct[2]=px3;	ct[3]=px3;
		ct+=xstr;
		ct[0]=px2;	ct[1]=px2;	ct[2]=px3;	ct[3]=px3;
#endif
		
		return(0);
	}
#endif


	cy0=yuv0[0];	cu0=yuv0[1];	cv0=yuv0[2];
	cy1=yuv1[0];	cu1=yuv1[1];	cv1=yuv1[2];

	cytab[0]=cy0;
	cytab[1]=(13*cy0+ 3*cy1)>>4;
	cytab[2]=(11*cy0+ 5*cy1)>>4;
	cytab[3]=( 9*cy0+ 7*cy1)>>4;
	cytab[4]=( 7*cy0+ 9*cy1)>>4;
	cytab[5]=( 5*cy0+11*cy1)>>4;
	cytab[6]=( 3*cy0+13*cy1)>>4;
	cytab[7]=cy1;

	cutab[0]=cu0;
	cvtab[0]=cv0;
	cutab[1]=(5*cu0+3*cu1)>>3;
	cvtab[1]=(5*cv0+3*cv1)>>3;
	cutab[2]=(3*cu0+5*cu1)>>3;
	cvtab[2]=(3*cv0+5*cv1)>>3;
	cutab[3]=cu1;
	cvtab[3]=cv1;
	
	ct=(u32 *)dst;
	for(y=0; y<4; y++)
	{
		i=blk>>(y*12);
		j=blk>>(48+((y>>1)<<2));
		k=blk>>(56+((y>>1)<<2));
		
		for(x=0; x<4; x++)
		{
			cy=cytab[(i>>(x*3))&7];
			cu=cutab[(j>>(x&2))&3];
			cv=cvtab[(k>>(x&2))&3];

#if 0			
			cb=((cu-128)<<1)+cy;
			cr=((cv-128)<<1)+cy;
//			cg=(cy*4-cr-cb)>>1;
			cg=((cy<<4)-5*cr-3*cb)>>3;
//			cg=cy;
#endif

			if(ycocg)
			{
				cu2=cu-128;
				cv2=cv-128;
				k=cy-cu2;
				cr=k+cv2;
				cg=cy+cu2;
				cb=k-cv2;
			}else
			{
				cu2=(cu-128)<<1;
				cv2=(cv-128)<<1;
				cb=cu2+cy;
				cr=cv2+cy;
				cg=(8*cy-5*cv2-3*cu2)>>3;
			}

			px=0xFF000000|(cr<<16)|(cg<<8)|cb;
			ct[x]=px;
		}
		ct+=xstr;
	}
	
	return(0);
}

int LCIF_UnpackCellBlockPixelsAlpha(byte *dst, int xstr,
	u64 blk, byte *yuv0, byte *yuv1)
{
	byte cytab[8];
	byte *ct;
	int cy0, cy1;
	int cy;
	int i, j, k, x, y;

	if(!blk)
	{
		cy0=yuv0[3];
//		cy1=yuv1[3];
//		cy=(cy0+cy1)>>1;
		cy=cy0;
		
		if(cy==255)
		{
			/* Assume already set to opaque. */
			return(0);
		}
		
		ct=dst;
		for(y=0; y<4; y++)
		{
			ct[ 3]=cy;
			ct[ 7]=cy;
			ct[11]=cy;
			ct[15]=cy;
			ct+=xstr<<2;
		}
		
		return(0);
	}

	cy0=yuv0[0];
	cy1=yuv1[0];
	
	cytab[0]=cy0;
	cytab[1]=(13*cy0+ 3*cy1)>>4;
	cytab[2]=(11*cy0+ 5*cy1)>>4;
	cytab[3]=( 9*cy0+ 7*cy1)>>4;
	cytab[4]=( 7*cy0+ 9*cy1)>>4;
	cytab[5]=( 5*cy0+11*cy1)>>4;
	cytab[6]=( 3*cy0+13*cy1)>>4;
	cytab[7]=cy1;

	ct=dst;
	for(y=0; y<4; y++)
	{
		i=blk>>(y*12);
		ct[ 3]=cytab[(i>>0)&7];
		ct[ 7]=cytab[(i>>3)&7];
		ct[11]=cytab[(i>>6)&7];
		ct[15]=cytab[(i>>9)&7];
		ct+=xstr<<2;
	}
	
	return(0);
}

int LCIF_DecImageBuffer(byte *outbuf, byte *inbuf, int *rxs, int *rys,
	int decfl)
{
	static byte *clrbuf=NULL;
	static u64 *blkbuf, *ablkbuf;
	static int clrbufsz=0;
	u64 *bcs, *abcs;
	byte *cs, *ct, *cte, *ycs1, *ycs2;
	int ixs, iys, xs, ys, xs2, ys2;
	int x, y, doflip, xstr, isalpha, clrs;
	int i, j, k, l, n, h;
	
	if(	(inbuf[0]!='l') ||
		(inbuf[1]!='c') ||
		(inbuf[2]!='i') ||
		(inbuf[3]!='f') )
	{
		return(-1);
	}

	ixs=(inbuf[ 7]<<24) |
		(inbuf[ 6]<<16) |
		(inbuf[ 5]<< 8) |
		(inbuf[ 4]<< 0) ;
	iys=(inbuf[11]<<24) |
		(inbuf[10]<<16) |
		(inbuf[ 9]<< 8) |
		(inbuf[ 8]<< 0) ;

	xs=ixs;
	ys=iys;
	if(xs<0)
		xs=-xs;
	if(ys<0)
		ys=-ys;

	xs2=(xs+3)>>2;
	ys2=(ys+3)>>2;

	if(!outbuf)
	{
		if(rxs)		*rxs=ixs;
		if(rys)		*rys=iys;
		return(1);	
	}

	doflip=0;
	if(rys && (*rys)==(-ys))
		doflip=1;

	n=xs2*ys2;
	
	if(clrbuf && (n>clrbufsz))
	{
		free(clrbuf);
		free(blkbuf);
		if(ablkbuf)
			free(blkbuf);
		clrbuf=NULL;
		blkbuf=NULL;
		ablkbuf=NULL;
	}
	
	isalpha=(inbuf[12]==4);
	clrs=0;
	if(inbuf[13]==1)
		clrs|=1;

	if(!clrbuf)
	{
		clrbuf=malloc((xs2*ys2+256)*2*4);
		blkbuf=malloc((xs2*ys2+256)*8);
		ablkbuf=NULL;
//		if(inbuf[12]==4)
		if(isalpha)
			ablkbuf=malloc((xs2*ys2+256)*8);
		clrbufsz=n;
	}else
	{
//		if((inbuf[12]==4) && !ablkbuf)
		if(isalpha && !ablkbuf)
			{ ablkbuf=malloc((clrbufsz+256)*8); }
	}
	
	

	cs=inbuf+16;
	cs=LCIF_DecColorPlane(clrbuf, cs, xs2, ys2*2);
	
	if((cs[0]!=0) || (cs[1]!=0))
	{
		printf("Expect 00 00 following Color Plane, See %02X %02X\n",
			cs[0], cs[1]);
	}else
	{
		cs+=2;
	}
	
	cs=LCIF_DecBlockPlane(blkbuf, cs, xs2, ys2);

	if((cs[0]!=0) || (cs[1]!=0))
	{
		printf("Expect 00 00 following Block Plane, See %02X %02X\n",
			cs[0], cs[1]);
	}else
	{
		cs+=2;
	}

//	if(ablkbuf)
	if(isalpha)
	{
		cs=LCIF_DecBlockPlane(ablkbuf, cs, xs2, ys2);
	}

#if 0
	printf("Dec Hash Color Plane %08X\n",
		LCIF_HashBuffer((byte *)clrbuf, xs2*ys2*2*4));
	printf("Dec Hash Block Plane %08X\n",
		LCIF_HashBuffer((byte *)blkbuf, xs2*ys2*8));

	BTIC1H_Img_SaveTGA("lciftst_deccplane.tga", clrbuf, xs2, ys2*2);
#endif

	xstr=xs;
	if(doflip)
		xstr=-xs;

	abcs=NULL;
	for(y=0; y<ys2; y++)
	{
		ct=outbuf+y*4*xs*4;
		ycs1=clrbuf+(y*2+0)*xs2*4;
		ycs2=clrbuf+(y*2+1)*xs2*4;
		bcs=blkbuf+y*xs2;
		if(doflip)
			ct=outbuf+(ys-y*4-1)*xs*4;
//		if(ablkbuf)
		if(isalpha)
			abcs=ablkbuf+y*xs2;
		
		for(x=0; x<xs2; x++)
		{
			LCIF_UnpackCellBlockPixels(
				ct+(x<<4), xstr,
				bcs[x],
				ycs1+(x<<2), ycs2+(x<<2),
				clrs);

			if(abcs)
			{
				LCIF_UnpackCellBlockPixelsAlpha(
					ct+(x<<4), xstr,
					abcs[x],
					ycs1+(x<<2), ycs2+(x<<2));
			}
		}
	}

//	free(clrbuf);
//	free(blkbuf);
//	if(ablkbuf)
//		free(blkbuf);
	
	if(rxs)	*rxs=ixs;
	if(rys)	*rys=iys;
	return(1);
}

byte *LCIF_DecImageTempBuffer(byte *inbuf, int *rxs, int *rys)
{
	byte *imgbuf;
	int xs, ys;
	int i0, i1;
	int i, n;
	
	xs=0; ys=0;
	LCIF_DecImageBuffer(NULL, inbuf, &xs, &ys, 0);
	if((xs<=0) || (ys<=0))
	{
		return(NULL);
	}
	
	imgbuf=malloc(xs*ys*4);
	LCIF_DecImageBuffer(imgbuf, inbuf, &xs, &ys, 0);

	n=xs*ys;
	for(i=0; i<n; i++)
	{
		i0=imgbuf[(i<<2)+0];
		i1=imgbuf[(i<<2)+2];
		imgbuf[(i<<2)+0]=i1;
		imgbuf[(i<<2)+2]=i0;
	}

	if(rxs) *rxs=xs;
	if(rys) *rys=ys;
	return(imgbuf);
}
