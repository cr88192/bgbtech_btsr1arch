// #include <bteifgl.h>

#ifndef FOURCC
#define FOURCC(a, b, c, d) ((a)|((b)<<8)|((c)<<16)|((d)<<24))
#endif


#define FCC_PNG_LOW	FOURCC(0x89, 'P', 'N', 'G')
#define FCC_PNG_HIGH	FOURCC(0x0D, 0x0A, 0x1A, 0x0A)

#define FCC_IHDR	FOURCC('I', 'H', 'D', 'R')
#define FCC_IEND	FOURCC('I', 'E', 'N', 'D')
#define FCC_IDAT	FOURCC('I', 'D', 'A', 'T')
#define FCC_PLTE	FOURCC('P', 'L', 'T', 'E')
#define FCC_tRNS	FOURCC('t', 'R', 'N', 'S')

#ifndef BTEIFGL_API
#define BTEIFGL_API
#endif

BTEIFGL_API byte *BGBBTJ_BufPNG_Decode(
	byte *csbuf, int cssz, int *w, int *h);

#if 0
u32 BGBBTJ_BufPNG_DataAdler32(void *buf, int sz, u32 lcrc)
{
	byte *s, *se;
	int i, c, s1, s2;

#if 0
	s=buf;
	s1=lcrc&0xFFFF;
	s2=(lcrc>>16)&0xFFFF;
	for(i=0; i<sz; i++)
	{
		s1=(s1+(s[i]))%65521;
		s2=(s1+s2)%65521;
	}
#endif

#if 1
	s=buf; se=((byte *)buf)+sz;
	s1=lcrc&0xFFFF;
	s2=(lcrc>>16)&0xFFFF;
	while(s<se)
	{
		s1=(s1+(*s++))%65521;
		s2=(s1+s2)%65521;
	}
#endif

	return((s2<<16)+s1);
}
#endif

#if 1
void BGBBTJ_BufPNG_DataAdler32_Step16(byte *buf, u32 *rs1, u32 *rs2)
{
	byte *cs;
	int s1, s2;

#if 0
	s1=*rs1; s2=*rs2; cs=buf;
	s1+=*cs++; s2+=s1;	s1+=*cs++; s2+=s1;
	s1+=*cs++; s2+=s1;	s1+=*cs++; s2+=s1;
	s1+=*cs++; s2+=s1;	s1+=*cs++; s2+=s1;
	s1+=*cs++; s2+=s1;	s1+=*cs++; s2+=s1;
	s1+=*cs++; s2+=s1;	s1+=*cs++; s2+=s1;
	s1+=*cs++; s2+=s1;	s1+=*cs++; s2+=s1;
	s1+=*cs++; s2+=s1;	s1+=*cs++; s2+=s1;
	s1+=*cs++; s2+=s1;	s1+=*cs++; s2+=s1;
	*rs1=s1; *rs2=s2;
#endif

#if 1
	s1=*rs1; s2=*rs2;
	s1+=buf[ 0]; s2+=s1;	s1+=buf[ 1]; s2+=s1;
	s1+=buf[ 2]; s2+=s1;	s1+=buf[ 3]; s2+=s1;
	s1+=buf[ 4]; s2+=s1;	s1+=buf[ 5]; s2+=s1;
	s1+=buf[ 6]; s2+=s1;	s1+=buf[ 7]; s2+=s1;
	s1+=buf[ 8]; s2+=s1;	s1+=buf[ 9]; s2+=s1;
	s1+=buf[10]; s2+=s1;	s1+=buf[11]; s2+=s1;
	s1+=buf[12]; s2+=s1;	s1+=buf[13]; s2+=s1;
	s1+=buf[14]; s2+=s1;	s1+=buf[15]; s2+=s1;
	*rs1=s1; *rs2=s2;
#endif
}

u32 BGBBTJ_BufPNG_DataAdler32(void *buf, int sz, u32 lcrc)
{
	byte *s;
	u32 s1, s2;
	int n, l;

	s=(byte *)buf;
	l=sz;
	s1=lcrc&0xFFFF;
	s2=(lcrc>>16)&0xFFFF;

	while(l>=5552)
	{
		n=5552/16;
		while(n--)
		{
			BGBBTJ_BufPNG_DataAdler32_Step16(s, &s1, &s2);
			s+=16;
		}
		s1=s1%65521;
		s2=s2%65521;
		l-=5552;
	}

	if(l>0)
	{
		while(l>=16)
		{
			BGBBTJ_BufPNG_DataAdler32_Step16(s, &s1, &s2);
			s+=16; l-=16;
		}
		while(l--)
			{ s1+=*s++; s2+=s1; }
		s1=s1%65521;
		s2=s2%65521;
	}

	return(s1|(s2<<16));
}
#endif

u32 BGBBTJ_BufPNG_CRC32(void *buf, int sz, u32 lcrc)
{
	static u32 crctab[256];
	static int init=0;

	byte *s;
	u32 c;
	int i, j;

	if(!init)
	{
		init=1;

		for(i=0; i<256; i++)
		{
			c=i;
			for(j=0; j<8; j++)
				if(c&1)c=0xedb88320^(c>>1);
					else c>>=1;
			crctab[i]=c;
		}
	}

	c=lcrc; s=buf; i=sz;
	while(i--)c=crctab[(c^(*s++))&0xFF]^(c>>8);
	return(c);
}

char *BGBBTJ_BufPNG_Fourcc2String(u32 fcc)
{
	static char buf[64], pos;
	int i, j;

	if(pos>56)pos=0;

	for(i=0; i<4; i++)
	{
		j=(fcc>>(i*8))&0xFF;
		if((j<' ') || (j>'~'))j='~';
		buf[pos+i]=j;
	}
	buf[pos+4]=0;
	return(buf+pos);
}

#if 0
int BGBBTJ_BufPNG_Paeth(int a, int b, int c)
{
	int p, pa, pb, pc;

	p=a+b-c;
	pa=(p>a)?(p-a):(a-p);
	pb=(p>b)?(p-b):(b-p);
	pc=(p>c)?(p-c):(c-p);

	p=(pa<=pb)?((pa<=pc)?a:c):((pb<=pc)?b:c);
	return(p);
}
#endif

#if 1
int BGBBTJ_BufPNG_Paeth(int a, int b, int c)
{
	int p, pa, pb, pc;
	int ma, mb, mc;

	p=a+b-c;
//	pa=(p>a)?(p-a):(a-p);
//	pb=(p>b)?(p-b):(b-p);
//	pc=(p>c)?(p-c):(c-p);

	pa=p-a; pa=pa*pa;
	pb=p-b; pb=pb*pb;
	pc=p-c; pc=pc*pc;

	ma=((pb-pa)>>24);
	mb=((pc-pb)>>24);
	mc=((pc-pa)>>24);

//	p=(1-ma)*((1-mb)*c+mb*b) + ma*((1-mc)*c + mc*a);
	p=(ma&((mb&c)|((~mb)&b))) | ((~ma)&((mc&c)|((~mc)&a)));

//	p=(pb>pa)?((pc>pb)?c:b):((pc>pa)?c:a);

//	p=(pa<=pb)?((pa<=pc)?a:c):((pb<=pc)?b:c);
	return(p);
}
#endif

u32 BGBBTJ_BufPNG_ReadInt32BE(byte **rct)
{
	byte *ct;
	u32 i;

	ct=*rct;
	i=(*ct++)<<24;
	i|=(*ct++)<<16;
	i|=(*ct++)<<8;
	i|=(*ct++);
	*rct=ct;
	return(i);
}

u32 BGBBTJ_BufPNG_ReadFourcc(byte **rct)
{
	byte *ct;
	u32 i;

	ct=*rct;
	i=(*ct++);
	i|=(*ct++)<<8;
	i|=(*ct++)<<16;
	i|=(*ct++)<<24;
	*rct=ct;
	return(i);
}

BTEIFGL_API int BGBBTJ_BufPNG_DecodeOBuf(
	byte *csbuf, int cssz, byte *obuf, int *w, int *h)
{
	byte *tbuf;
	
	tbuf=BGBBTJ_BufPNG_Decode(csbuf, cssz, w, h);
	if(!tbuf)return(-1);
	
	memcpy(obuf, tbuf, (*w)*(*h)*4);
	free(tbuf);
	return(0);
}

void BGBBTJ_BufPNG_DecodeScan24_0(byte *s, byte *sca, byte *scb, int xs)
{
	int pa, pb, pc;
	int j, k;

	for(j=0; j<xs; j++)
	{
		sca[0]=s[0];	sca[1]=s[1];
		sca[2]=s[2];	sca[3]=255;
		s+=3; sca+=4; scb+=4;
	}
}

void BGBBTJ_BufPNG_DecodeScan32_0(byte *s, byte *sca, byte *scb, int xs)
{
	int pa, pb, pc;
	int j, k;

	for(j=0; j<xs; j++)
	{
		sca[0]=s[0];	sca[1]=s[1];
		sca[2]=s[2];	sca[3]=s[3];
		s+=4; sca+=4; scb+=4;
	}
}

void BGBBTJ_BufPNG_DecodeScan24_1(byte *s, byte *sca, byte *scb, int xs)
{
	int pa, pb, pc;
	int j, k;

#if 0
	for(k=0; k<3; k++)
	{
//		pa=0;
//		pb=scb[k];
		pc=0;
		sca[k]=s[k]+pc;
	}
	sca[3]=255;
	s+=3; sca+=4; scb+=4;

	for(j=1; j<xs; j++)
	{
		for(k=0; k<3; k++)
		{
//			pa=scb[k-4];
//			pb=scb[k];
			pc=sca[k-4];
			sca[k]=s[k]+pc;
		}
		sca[3]=255;
		s+=3; sca+=4; scb+=4;
	}
#endif

	sca[0]=s[0];	sca[1]=s[1];
	sca[2]=s[2];	sca[3]=255;
	s+=3; sca+=4; scb+=4;

	for(j=1; j<xs; j++)
	{
		sca[0]=s[0]+sca[0-4];	sca[1]=s[1]+sca[1-4];
		sca[2]=s[2]+sca[2-4];	sca[3]=255;
		s+=3; sca+=4; scb+=4;
	}
}

void BGBBTJ_BufPNG_DecodeScan32_1(byte *s, byte *sca, byte *scb, int xs)
{
	int pa, pb, pc;
	int j, k;

	sca[0]=s[0];	sca[1]=s[1];
	sca[2]=s[2];	sca[3]=s[3];
	s+=4; sca+=4; scb+=4;

	for(j=1; j<xs; j++)
	{
		sca[0]=s[0]+sca[0-4];	sca[1]=s[1]+sca[1-4];
		sca[2]=s[2]+sca[2-4];	sca[3]=s[3]+sca[3-4];
		s+=4; sca+=4; scb+=4;
	}
}

void BGBBTJ_BufPNG_DecodeScan24_2(byte *s, byte *sca, byte *scb, int xs)
{
	int pa, pb, pc;
	int j, k;

#if 0
	for(k=0; k<3; k++)
	{
//		pa=0;
		pb=scb[k];
//		pc=0;
		sca[k]=s[k]+pb;
	}
	sca[3]=255;
	s+=3; sca+=4; scb+=4;

	for(j=1; j<xs; j++)
	{
		for(k=0; k<3; k++)
		{
//			pa=scb[k-4];
			pb=scb[k];
//			pc=sca[k-4];
			sca[k]=s[k]+pb;
		}
		sca[3]=255;
		s+=3; sca+=4; scb+=4;
	}
#endif

	for(j=0; j<xs; j++)
	{
		sca[0]=s[0]+scb[0];	sca[1]=s[1]+scb[1];
		sca[2]=s[2]+scb[2];	sca[3]=255;
		s+=3; sca+=4; scb+=4;
	}
}

void BGBBTJ_BufPNG_DecodeScan32_2(byte *s, byte *sca, byte *scb, int xs)
{
	int j, k;
	for(j=0; j<xs; j++)
	{
		sca[0]=s[0]+scb[0];	sca[1]=s[1]+scb[1];
		sca[2]=s[2]+scb[2];	sca[3]=s[3]+scb[3];
		s+=4; sca+=4; scb+=4;
	}
}

void BGBBTJ_BufPNG_DecodeScan24_3(byte *s, byte *sca, byte *scb, int xs)
{
	int pa, pb, pc;
	int j, k;

//	for(k=0; k<3; k++)
//	{
//		pa=0; pb=scb[k]; pc=0;
//		sca[k]=s[k]+((pc+pb)>>1);
//	}
	sca[0]=s[0]+(scb[0]>>1);	sca[1]=s[1]+(scb[1]>>1);
	sca[2]=s[2]+(scb[2]>>1);	sca[3]=255;
	s+=3; sca+=4; scb+=4;

	for(j=1; j<xs; j++)
	{
#if 0
		for(k=0; k<3; k++)
		{
//			pa=scb[k-4];
			pb=scb[k];
			pc=sca[k-4];
			sca[k]=s[k]+((pc+pb)>>1);
		}
#endif

//		sca[k]=s[k]+((sca[k-4]+scb[k])>>1);

		sca[0]=s[0]+((sca[0-4]+scb[0])>>1);
		sca[1]=s[1]+((sca[1-4]+scb[1])>>1);
		sca[2]=s[2]+((sca[2-4]+scb[2])>>1);

		sca[3]=255;
		s+=3; sca+=4; scb+=4;
	}
}

void BGBBTJ_BufPNG_DecodeScan32_3(byte *s, byte *sca, byte *scb, int xs)
{
	int pa, pb, pc;
	int j, k;

	sca[0]=s[0]+(scb[0]>>1);	sca[1]=s[1]+(scb[1]>>1);
	sca[2]=s[2]+(scb[2]>>1);	sca[3]=s[3]+(scb[3]>>1);
	s+=4; sca+=4; scb+=4;

	for(j=1; j<xs; j++)
	{
		sca[0]=s[0]+((sca[0-4]+scb[0])>>1);
		sca[1]=s[1]+((sca[1-4]+scb[1])>>1);
		sca[2]=s[2]+((sca[2-4]+scb[2])>>1);
		sca[3]=s[3]+((sca[3-4]+scb[3])>>1);
		s+=4; sca+=4; scb+=4;
	}
}

void BGBBTJ_BufPNG_DecodeScan24_4(byte *s, byte *sca, byte *scb, int xs)
{
	int pa, pb, pc;
	int j, k;

//	for(k=0; k<3; k++)
//	{
//		pa=0; pb=scb[k]; pc=0;
//		sca[k]=s[k]+BGBBTJ_BufPNG_Paeth(pc, pb, pa);
//	}

	sca[0]=s[0]+BGBBTJ_BufPNG_Paeth(0, scb[0], 0);
	sca[1]=s[1]+BGBBTJ_BufPNG_Paeth(0, scb[1], 0);
	sca[2]=s[2]+BGBBTJ_BufPNG_Paeth(0, scb[2], 0);
	sca[3]=255;
	s+=3; sca+=4; scb+=4;

	for(j=1; j<xs; j++)
	{
//		for(k=0; k<3; k++)
//		{
//			pa=scb[k-4]; pb=scb[k]; pc=sca[k-4];
//			sca[k]=s[k]+BGBBTJ_BufPNG_Paeth(pc, pb, pa);
//		}
		sca[0]=s[0]+BGBBTJ_BufPNG_Paeth(sca[0-4], scb[0], scb[0-4]);
		sca[1]=s[1]+BGBBTJ_BufPNG_Paeth(sca[1-4], scb[1], scb[1-4]);
		sca[2]=s[2]+BGBBTJ_BufPNG_Paeth(sca[2-4], scb[2], scb[2-4]);
		sca[3]=255;
		s+=3; sca+=4; scb+=4;
	}
}

void BGBBTJ_BufPNG_DecodeScan32_4(byte *s, byte *sca, byte *scb, int xs)
{
	int pa, pb, pc;
	int j, k;

	sca[0]=s[0]+BGBBTJ_BufPNG_Paeth(0, scb[0], 0);
	sca[1]=s[1]+BGBBTJ_BufPNG_Paeth(0, scb[1], 0);
	sca[2]=s[2]+BGBBTJ_BufPNG_Paeth(0, scb[2], 0);
	sca[3]=s[3]+BGBBTJ_BufPNG_Paeth(0, scb[3], 0);
	s+=4; sca+=4; scb+=4;

	for(j=1; j<xs; j++)
	{
		sca[0]=s[0]+BGBBTJ_BufPNG_Paeth(sca[0-4], scb[0], scb[0-4]);
		sca[1]=s[1]+BGBBTJ_BufPNG_Paeth(sca[1-4], scb[1], scb[1-4]);
		sca[2]=s[2]+BGBBTJ_BufPNG_Paeth(sca[2-4], scb[2], scb[2-4]);
		sca[3]=s[3]+BGBBTJ_BufPNG_Paeth(sca[3-4], scb[3], scb[3-4]);
		s+=4; sca+=4; scb+=4;
	}
}

bool BGBBTJ_BufPNG_DecodeScan(
	byte *s, byte *sca, byte *scb, int xs, int ssz, int fm)
{
	bool ret;

	if(ssz==3)
	{
		ret=true;
		switch(fm)
		{
		case 0: BGBBTJ_BufPNG_DecodeScan24_0(s, sca, scb, xs); break;
		case 1: BGBBTJ_BufPNG_DecodeScan24_1(s, sca, scb, xs); break;
		case 2: BGBBTJ_BufPNG_DecodeScan24_2(s, sca, scb, xs); break;
		case 3: BGBBTJ_BufPNG_DecodeScan24_3(s, sca, scb, xs); break;
		case 4: BGBBTJ_BufPNG_DecodeScan24_4(s, sca, scb, xs); break;
		default: ret=false; break;
		}
		return(ret);
	}

	if(ssz==4)
	{
		ret=true;
		switch(fm)
		{
		case 0: BGBBTJ_BufPNG_DecodeScan32_0(s, sca, scb, xs); break;
		case 1: BGBBTJ_BufPNG_DecodeScan32_1(s, sca, scb, xs); break;
		case 2: BGBBTJ_BufPNG_DecodeScan32_2(s, sca, scb, xs); break;
		case 3: BGBBTJ_BufPNG_DecodeScan32_3(s, sca, scb, xs); break;
		case 4: BGBBTJ_BufPNG_DecodeScan32_4(s, sca, scb, xs); break;
		default: ret=false; break;
		}
		return(ret);
	}
	
	return(false);
}

BTEIFGL_API byte *BGBBTJ_BufPNG_Decode(
	byte *csbuf, int cssz, int *w, int *h)
{
	static byte plte[768], trns[256];
	u32 fcc0, fcc1, len, crc, end;
	int xs, ys, bpp, clr, cm, fm, im, ssz;
	int i, j, k, pa, pb, pc;
	byte *buf, *bufe;
	byte *buf2, *buf2e;
	byte *sca, *scb;
	byte *s, *t, *cs;

	cs=csbuf;

	fcc0=BGBBTJ_BufPNG_ReadFourcc(&cs);
	fcc1=BGBBTJ_BufPNG_ReadFourcc(&cs);
	if((fcc0!=FCC_PNG_LOW) || (fcc1!=FCC_PNG_HIGH))
	{
		printf("damaged fourcc or non-png %08X %08X\n", fcc0, fcc1);
		return(NULL);
	}

	memset(plte, 0, 768);
	memset(trns, 255, 256);

	while(cs<(csbuf+cssz))
	{
		len=BGBBTJ_BufPNG_ReadInt32BE(&cs);
		fcc0=BGBBTJ_BufPNG_ReadFourcc(&cs);

		if(fcc0==FCC_IEND)
		{
			crc=BGBBTJ_BufPNG_ReadInt32BE(&cs);
//			printf("%s %d %X\n", BGBBTJ_BufPNG_Fourcc2String(fcc0),
//				len, crc);

			break;
		}

		end=(cs-csbuf)+len;
		if(fcc0==FCC_IHDR)
		{
			xs=BGBBTJ_BufPNG_ReadInt32BE(&cs);
			ys=BGBBTJ_BufPNG_ReadInt32BE(&cs);

			bpp=*cs++;
			clr=*cs++;
			cm=*cs++;
			fm=*cs++;
			im=*cs++;

			if(bpp!=8)
			{
				printf("only 8 bits/channel supported\n");
				return(NULL);
			}
			if((clr!=3) && (clr!=0) && (clr!=4) &&
				(clr!=2) && (clr!=6))
			{
//				printf("only RGB(A) or Y(A) images supported\n");
				printf("unsupported color type %d\n", clr);
				return(NULL);
			}
			if(cm!=0)
			{
				printf("only deflated images supported\n");
				return(NULL);
			}
			if(fm!=0)
			{
				printf("unknown filter method\n");
				return(NULL);
			}
			if(im!=0)
			{
				printf("interlacing not supported\n");
				return(NULL);
			}

			buf=malloc(xs*ys*8);
			bufe=buf;

			ssz=0;
			if(clr==2)ssz=3;
			if(clr==6)ssz=4;

			if(clr==0)ssz=1;
			if(clr==4)ssz=2;
			if(clr==3)ssz=1;

//			printf("%dx%dx%dbpp\n", xs, ys, ssz*bpp);
//			printf("clr %d cm %d fm %d im %d\n",
//				clr, cm, fm, im);
		}

		if(fcc0==FCC_IDAT)
		{
//			vfread(bufe, 1, len, ifd);
			memcpy(bufe, cs, len);
			cs+=len;
			bufe+=len;
		}

		if(fcc0==FCC_PLTE)
		{
			i=(len<=768)?len:768;
			memcpy(plte, cs, i); cs+=i;
		}
		if(fcc0==FCC_tRNS)
		{
			i=(len<=256)?len:256;
			memcpy(plte, cs, i); cs+=i;
		}

//		vfseek(ifd, end, 0);
		cs=csbuf+end;
		crc=BGBBTJ_BufPNG_ReadInt32BE(&cs);

//		printf("%s %d %X\n", BGBBTJ_BufPNG_Fourcc2String(fcc0), len, crc);
	}

	buf2=malloc(xs*ys*8);

	i=bufe-(buf+2);
	i=PDUNZ_DecodeStream(buf+2, buf2, i, xs*ys*ssz+ys);
//	i=vfInflateBuffer(buf2, buf+2, xs*ys*ssz+ys, i);
//	i=puff(buf2, &k, buf+2, &j);

	if(i<0)	//this isn't good
	{
		printf("Inflater Error %d\n", i);

		free(buf);
		free(buf2);
		return(NULL);
	}
//	i=j;

	if(i>(xs*ys*4))printf("Inflated %d expected %d\n", i, xs*ys*4);

	j=(buf[0]<<8)|buf[1];
//	printf("Zlib %04X\n", j);

	j=(buf[i+2+0]<<24)|(buf[i+2+1]<<16)|(buf[i+2+2]<<8)|buf[i+2+3];
	k=BGBBTJ_BufPNG_DataAdler32(buf2, xs*ys*ssz+ys, 1);

	if(j!=k)printf("Adler32: %s  %08X %08X\n", (j==k)?"Pass":"Fail", j, k);

//	PDZ2_EncodeStream(buf2, buf, xs*ys*ssz+ys, xs*ys*8);
//	PDUNZ_DecodeStream(buf, buf2, i, xs*ys*ssz+ys);
	free(buf);

	buf=malloc((xs+1)*(ys+1)*4);
	s=buf2;
	t=buf;

#if 0
	for(i=0; i<ys; i++)
	{
		fm=*s++;

		for(j=0; j<xs; j++)
		{
			for(k=0; k<ssz; k++)
			{
				if(i && j)
				{
					pa=buf[(((ys-i)*xs)+j-1)*4+k];
					pb=buf[(((ys-i)*xs)+j)*4+k];
					pc=buf[(((ys-i-1)*xs)+j-1)*4+k];
				}else if(i)
				{
					pa=0;
					pb=buf[(((ys-i)*xs)+j)*4+k];
					pc=0;
				}else if(j)
				{
					pa=0;
					pb=0;
					pc=buf[(((ys-i-1)*xs)+j-1)*4+k];
				}else
				{
					pa=0;
					pb=0;
					pc=0;
				}

				switch(fm)
				{
				case 0:
					buf[(((ys-i-1)*xs)+j)*4+k]=s[k];
					break;
				case 1:
					buf[(((ys-i-1)*xs)+j)*4+k]=s[k]+pc;
					break;
				case 2:
					buf[(((ys-i-1)*xs)+j)*4+k]=s[k]+pb;
					break;
				case 3:
					buf[(((ys-i-1)*xs)+j)*4+k]=
						s[k]+((pc+pb)>>1);
					break;
				case 4:
					buf[(((ys-i-1)*xs)+j)*4+k]=
						s[k]+BGBBTJ_BufPNG_Paeth(pc, pb, pa);
					break;
				default:
					break;
				}
			}
			if(ssz==3)buf[(((ys-i-1)*xs)+j)*4+3]=255;

			s+=ssz;
		}
	}
#endif

#if 1
	for(i=0; i<ys; i++)
	{
		fm=*s++;
		scb=buf+(((ys-i)*xs)+0)*4;
		sca=buf+(((ys-i-1)*xs)+0)*4;

		if(i && BGBBTJ_BufPNG_DecodeScan(s, sca, scb, xs, ssz, fm))
		{
			s+=xs*ssz;
			continue;
		}

#if 0
		if(i && (ssz==4) && (fm==1))
		{
			for(j=0; j<xs; j++)
			{
				for(k=0; k<4; k++)
				{
					if(j)
					{
//						pa=scb[(j-1)*4+k];
//						pb=scb[(j  )*4+k];
						pc=sca[(j-1)*4+k];
					}else
					{
//						pa=0;
//						pb=scb[(j  )*4+k];
						pc=0;
					}

					sca[j*4+k]=s[k]+pc;
				}
				s+=4;
			}
			continue;
		}

		if(i && (ssz==3) && (fm==1))
		{
			for(k=0; k<3; k++)
			{
//				pa=0;
//				pb=scb[(j  )*4+k];
				pc=0;
				sca[k]=s[k]+pc;
			}
			sca[3]=255;
			s+=3; sca+=4; scb+=4;

			for(j=1; j<xs; j++)
			{
				for(k=0; k<3; k++)
				{
//					pa=scb[k-4];
//					pb=scb[k];
					pc=sca[k-4];
					sca[k]=s[k]+pc;
				}
				sca[3]=255;
				s+=3; sca+=4; scb+=4;
			}
			continue;
		}
#endif

		for(j=0; j<xs; j++)
		{
			for(k=0; k<ssz; k++)
			{
				if(i && j)
				{
					pa=scb[(j-1)*4+k];
					pb=scb[(j  )*4+k];
					pc=sca[(j-1)*4+k];
				}else if(i)
				{
					pa=0;
					pb=scb[(j  )*4+k];
					pc=0;
				}else if(j)
				{
					pa=0;
					pb=0;
					pc=sca[(j-1)*4+k];
				}else
				{
					pa=0;
					pb=0;
					pc=0;
				}

				switch(fm)
				{
				case 0:
					sca[j*4+k]=s[k];
					break;
				case 1:
					sca[j*4+k]=s[k]+pc;
					break;
				case 2:
					sca[j*4+k]=s[k]+pb;
					break;
				case 3:
					sca[j*4+k]=s[k]+((pc+pb)>>1);
					break;
				case 4:
					sca[j*4+k]=s[k]+BGBBTJ_BufPNG_Paeth(pc, pb, pa);
					break;
				default:
					break;
				}
			}
			if(ssz==3)sca[j*4+3]=255;

			s+=ssz;
		}
	}
#endif

	free(buf2);

	//correct for grayscale images...
	if(clr==0)
	{
		for(i=0; i<(xs*ys); i++)
		{
			buf[i*4+1]=buf[i*4+0];
			buf[i*4+2]=buf[i*4+0];
			buf[i*4+3]=255;
		}
	}
	if(clr==4)
	{
		for(i=0; i<(xs*ys); i++)
		{
			buf[i*4+3]=buf[i*4+1];
			buf[i*4+1]=buf[i*4+0];
			buf[i*4+2]=buf[i*4+0];
		}
	}

	//correct for paletized images
	if(clr==3)
	{
		for(i=0; i<xs*ys; i++)
		{
			j=buf[i*4+0];

			buf[i*4+0]=plte[j*3+0];
			buf[i*4+1]=plte[j*3+1];
			buf[i*4+2]=plte[j*3+2];
			buf[i*4+3]=trns[j];
		}
	}

	if(w && h)
		{ *w=xs; *h=ys; }
	return(buf);
}

void BGBBTJ_BufPNG_WriteInt32LE(byte **rct, u32 v)
{
	byte *ct;
	ct=*rct;
	*ct++=v&0xFF;
	*ct++=(v>>8)&0xFF;
	*ct++=(v>>16)&0xFF;
	*ct++=(v>>24)&0xFF;
	*rct=ct;
}

void BGBBTJ_BufPNG_WriteInt32BE(byte **rct, u32 v)
{
	byte *ct;
	ct=*rct;
	*ct++=(v>>24)&0xFF;
	*ct++=(v>>16)&0xFF;
	*ct++=(v>>8)&0xFF;
	*ct++=v&0xFF;
	*rct=ct;
}

void BGBBTJ_BufPNG_WriteFourcc(byte **rct, u32 v)
{
	byte *ct;
	ct=*rct;
	*ct++=v&0xFF;
	*ct++=(v>>8)&0xFF;
	*ct++=(v>>16)&0xFF;
	*ct++=(v>>24)&0xFF;
	*rct=ct;
}

void BGBBTJ_BufPNG_WriteChunk(byte **rct, u32 fcc, byte *buf, int len)
{
	byte tb[4];
	u32 crc;

	BGBBTJ_BufPNG_WriteInt32BE(rct, len);
	BGBBTJ_BufPNG_WriteFourcc(rct, fcc);
//	vfwrite(buf, 1, len, fd);
	memcpy(*rct, buf, len);
	*rct=(*rct)+len;

	tb[0]=fcc&0xFF; tb[1]=(fcc>>8)&0xFF;
	tb[2]=(fcc>>16)&0xFF; tb[3]=(fcc>>24)&0xFF;
	crc=BGBBTJ_BufPNG_CRC32(tb, 4, (u32)-1);
	crc=BGBBTJ_BufPNG_CRC32(buf, len, crc);

//	printf("%s %d %X\n", BGBBTJ_BufPNG_Fourcc2String(fcc), len, ~crc);

	BGBBTJ_BufPNG_WriteInt32BE(rct, ~crc);
}

#ifndef ABS
#define ABS(v)	(((v)<0)?(-(v)):(v))
#endif

BTEIFGL_API int BGBBTJ_BufPNG_Encode(
	byte *ctbuf, int ctsz, byte *buf, int xs, int ys)
{
	byte hbuf[64];
	byte *buf2, *s, *t, *ct;
	int fm, sz, pa, pb, pc, pd;
	int e, be, bf, am, ssz;
	int i, j, k, l;

	am=0;
	for(i=0; i<(xs*ys); i++)
		if(buf[i*4+3]<255)am=1;
	ssz=am?4:3;

	buf2=malloc(xs*ys*ssz+ys);

	ct=ctbuf;

	t=buf2;
	for(i=0; i<ys; i++)
	{
		be=ssz*xs*255;
		bf=0;

		for(l=0; l<5; l++)
		{
			e=0;
			for(j=0; j<xs; j++)
			{
				for(k=0; k<ssz; k++)
				{
					pd=buf[(((ys-i-1)*xs)+j)*4+k];

					if(i && j)
					{
						pa=buf[(((ys-i)*xs)+j-1)*4+k];
						pb=buf[(((ys-i)*xs)+j)*4+k];
						pc=buf[(((ys-i-1)*xs)+j-1)*4+k];
					}else if(i)
					{
						pa=0;
						pb=buf[(((ys-i)*xs)+j)*4+k];
						pc=0;
					}else if(j)
					{
						pa=0;
						pb=0;
						pc=buf[(((ys-i-1)*xs)+j-1)*4+k];
					}else
					{
						pa=0;
						pb=0;
						pc=0;
					}

					switch(l)
					{
					case 0:
						e+=ABS(pd);
						break;
					case 1:
						e+=ABS(pd-pc);
						break;
					case 2:
						e+=ABS(pd-pb);
						break;
					case 3:
						e+=ABS(pd-((pc+pb)>>1));
						break;
					case 4:
						e+=ABS(pd-BGBBTJ_BufPNG_Paeth(pc, pb, pa));
						break;
					default:
						break;
					}
				}
			}

			if(e<be)
			{
				be=e;
				bf=l;
			}
		}

//		bf=4;

//		printf("%d ", bf);

		fm=bf;
		*t++=fm;

		for(j=0; j<xs; j++)
		{
			for(k=0; k<ssz; k++)
			{
				pd=buf[(((ys-i-1)*xs)+j)*4+k];

				if(i && j)
				{
					pa=buf[(((ys-i)*xs)+j-1)*4+k];
					pb=buf[(((ys-i)*xs)+j)*4+k];
					pc=buf[(((ys-i-1)*xs)+j-1)*4+k];
				}else if(i)
				{
					pa=0;
					pb=buf[(((ys-i)*xs)+j)*4+k];
					pc=0;
				}else if(j)
				{
					pa=0;
					pb=0;
					pc=buf[(((ys-i-1)*xs)+j-1)*4+k];
				}else
				{
					pa=0;
					pb=0;
					pc=0;
				}

				switch(fm)
				{
				case 0:
					*t++=pd;
					break;
				case 1:
					*t++=pd-pc;
					break;
				case 2:
					*t++=pd-pb;
					break;
				case 3:
					*t++=pd-((pc+pb)>>1);
					break;
				case 4:
					*t++=pd-BGBBTJ_BufPNG_Paeth(pc, pb, pa);
					break;
				default:
					break;
				}
			}
		}
	}
//	printf("\n");

	buf=malloc(xs*ys*8);

	//zlib header
	i=(7<<12)|(8<<8)|(3<<6);
	j=i%31;
	if(j)i|=31-j;
	buf[0]=i>>8;
	buf[1]=i&0xFF;
//	printf("Zlib %04X\n", i);

	i=PDZ2_EncodeStreamLvl(buf2, buf+2, xs*ys*ssz+ys, xs*ys*8, 1);
//	i=vfDeflateBufferLvl(buf+2, buf2, xs*ys*8, xs*ys*ssz+ys, 9);

	//checksum
	j=BGBBTJ_BufPNG_DataAdler32(buf2, xs*ys*ssz+ys, 1);
	buf[2+i+0]=(j>>24)&0xFF;
	buf[2+i+1]=(j>>16)&0xFF;
	buf[2+i+2]=(j>>8)&0xFF;
	buf[2+i+3]=j&0xFF;

//	printf("Adler32: Wrote %08X\n", j);

	sz=i+6;
	free(buf2);

	BGBBTJ_BufPNG_WriteFourcc(&ct, FCC_PNG_LOW);
	BGBBTJ_BufPNG_WriteFourcc(&ct, FCC_PNG_HIGH);

	hbuf[0]=xs>>24; hbuf[1]=xs>>16; hbuf[2]=xs>>8; hbuf[3]=xs;
	hbuf[4]=ys>>24; hbuf[5]=ys>>16; hbuf[6]=ys>>8; hbuf[7]=ys;
	hbuf[8]=8;	//8 bits/sample
	hbuf[9]=am?6:2;	//RGB(A)
	hbuf[10]=0;	//0=deflate
	hbuf[11]=0;	//no filter
	hbuf[12]=0;	//no interlace

	BGBBTJ_BufPNG_WriteChunk(&ct, FCC_IHDR, hbuf, 13);

	s=buf; i=sz;
	while(i>65536)
	{
		BGBBTJ_BufPNG_WriteChunk(&ct, FCC_IDAT, s, 65536);
		s+=65536;
		i-=65536;
	}
	BGBBTJ_BufPNG_WriteChunk(&ct, FCC_IDAT, s, i);

	BGBBTJ_BufPNG_WriteChunk(&ct, FCC_IEND, NULL, 0);
	
	return(ct-ctbuf);
}


BTEIFGL_API int BGBBTJ_BufPNG_EncodeFast(
	byte *ctbuf, int ctsz, byte *buf, int xs, int ys)
{
	byte hbuf[64];
	byte *buf2, *s, *t, *ct, *se;
//	byte *sxa, *sxb, *sxc, *sxd;
	int ysi0, ysi1, ixa, ixb, ixc, ixd;
	int fm, sz, pa, pb, pc, pd;
	int e, be, bf, am, ssz;
	int i, j, k, l;

	am=0;
	for(i=0; i<(xs*ys); i++)
		if(buf[i*4+3]<255)am=1;
	ssz=am?4:3;

	buf2=malloc(xs*ys*ssz+ys);

	ct=ctbuf;

#if 1
	t=buf2;
	for(i=0; i<ys; i++)
	{
		be=ssz*xs*255; bf=1;
		fm=bf; *t++=fm;

#if 1
		if(ssz==4)
		{
			ysi1=(ys-i-1)*xs;
			s=buf+ysi1*4;
			se=s+xs*4;
			
//			*t++=*s++; *t++=*s++;
//			*t++=*s++; *t++=*s++;
			t[0]=s[0]; t[1]=s[1];
			t[2]=s[2]; t[3]=s[3];
			s+=4; t+=4;
			
//			for(j=1; j<xs; j++)
			while(s<se)
			{
				t[0]=s[0]-s[-4]; t[1]=s[1]-s[-3];
				t[2]=s[2]-s[-2]; t[3]=s[3]-s[-1];
				s+=4; t+=4;
			}
			continue;
		}
#endif

#if 1
		if(ssz==3)
		{
			ysi1=(ys-i-1)*xs;
			s=buf+ysi1*4;
			se=s+xs*4;
			
//			*t++=*s++; *t++=*s++;
//			*t++=*s++; *t++=*s++;
			t[0]=s[0]; t[1]=s[1]; t[2]=s[2];
			s+=4; t+=3;
			
//			for(j=1; j<xs; j++)
			while(s<se)
			{
				t[0]=s[0]-s[-4]; t[1]=s[1]-s[-3]; t[2]=s[2]-s[-2];
				s+=4; t+=3;
			}
			continue;
		}
#endif

#if 1
		for(j=0; j<xs; j++)
		{
			if(i && j)
			{
//				ysi0=(ys-i-0)*xs+j;
				ysi1=(ys-i-1)*xs+j;
//				ixa=(ysi0-1)*4;		ixb=(ysi0-0)*4;
				ixc=(ysi1-1)*4;		ixd=(ysi1-0)*4;

//				sxc=buf+ixc;
//				sxd=buf+ixd;

				if(ssz==4)
				{
//					*t++=(*sxd++)-(*sxc++);
//					*t++=(*sxd++)-(*sxc++);
//					*t++=(*sxd++)-(*sxc++);
//					*t++=(*sxd++)-(*sxc++);

					pc=buf[ixc+0]; pd=buf[ixd+0]; *t++=pd-pc;
					pc=buf[ixc+1]; pd=buf[ixd+1]; *t++=pd-pc;
					pc=buf[ixc+2]; pd=buf[ixd+2]; *t++=pd-pc;
					pc=buf[ixc+3]; pd=buf[ixd+3]; *t++=pd-pc;
				}else if(ssz==3)
				{
					pc=buf[ixc+0]; pd=buf[ixd+0]; *t++=pd-pc;
					pc=buf[ixc+1]; pd=buf[ixd+1]; *t++=pd-pc;
					pc=buf[ixc+2]; pd=buf[ixd+2]; *t++=pd-pc;
				}else
				{
					for(k=0; k<ssz; k++)
					{
//						pa=buf[ixa+k];
//						pb=buf[ixb+k];
						pc=buf[ixc+k];
						pd=buf[ixd+k];

						*t++=pd-pc;
					}
				}

				continue;
			}

			for(k=0; k<ssz; k++)
			{
#if 1
				ysi0=(ys-i-0)*xs+j;
				ysi1=(ys-i-1)*xs+j;

				pd=buf[ysi1*4+k];

				if(i && j)
					{ pa=buf[(ysi0-1)*4+k]; pb=buf[ysi0*4+k];
					  pc=buf[(ysi1-1)*4+k]; }
				else if(i)
					{ pa=0; pb=buf[ysi0*4+k]; pc=0; }
				else if(j)
					{ pa=0; pb=0; pc=buf[(ysi1-1)*4+k]; }
				else
					{ pa=0; pb=0; pc=0; }
#endif

				*t++=pd-pc;
//				*t++=pd-((pc+pb)>>1);
			}
		}
#endif
	}
//	printf("\n");
#endif

	buf=malloc(xs*ys*8);

	//zlib header
	i=(7<<12)|(8<<8)|(3<<6);
	j=i%31;
	if(j)i|=31-j;
	buf[0]=i>>8;
	buf[1]=i&0xFF;
//	printf("Zlib %04X\n", i);

//	i=PDZ2_EncodeStreamLvl(buf2, buf+2, xs*ys*ssz+ys, xs*ys*8, 1);
	i=PDZ2_EncodeStreamLvl(buf2, buf+2, xs*ys*ssz+ys, xs*ys*8, 0);
//	i=vfDeflateBufferLvl(buf+2, buf2, xs*ys*8, xs*ys*ssz+ys, 9);

	//checksum
	j=BGBBTJ_BufPNG_DataAdler32(buf2, xs*ys*ssz+ys, 1);
	buf[2+i+0]=(j>>24)&0xFF;
	buf[2+i+1]=(j>>16)&0xFF;
	buf[2+i+2]=(j>>8)&0xFF;
	buf[2+i+3]=j&0xFF;

//	printf("Adler32: Wrote %08X\n", j);

	sz=i+6;
	free(buf2);

	BGBBTJ_BufPNG_WriteFourcc(&ct, FCC_PNG_LOW);
	BGBBTJ_BufPNG_WriteFourcc(&ct, FCC_PNG_HIGH);

	hbuf[0]=xs>>24; hbuf[1]=xs>>16; hbuf[2]=xs>>8; hbuf[3]=xs;
	hbuf[4]=ys>>24; hbuf[5]=ys>>16; hbuf[6]=ys>>8; hbuf[7]=ys;
	hbuf[8]=8;	//8 bits/sample
	hbuf[9]=am?6:2;	//RGB(A)
	hbuf[10]=0;	//0=deflate
	hbuf[11]=0;	//no filter
	hbuf[12]=0;	//no interlace

	BGBBTJ_BufPNG_WriteChunk(&ct, FCC_IHDR, hbuf, 13);

	s=buf; i=sz;
	while(i>65536)
	{
		BGBBTJ_BufPNG_WriteChunk(&ct, FCC_IDAT, s, 65536);
		s+=65536;
		i-=65536;
	}
	BGBBTJ_BufPNG_WriteChunk(&ct, FCC_IDAT, s, i);

	BGBBTJ_BufPNG_WriteChunk(&ct, FCC_IEND, NULL, 0);
	
	return(ct-ctbuf);
}
