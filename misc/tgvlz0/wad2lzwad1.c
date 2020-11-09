#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define TGVLZ_NOMAIN
#include "tgvlz1.c"

#ifndef BGBCC_FOURCC
#define BGBCC_FOURCC(a, b, c, d)	((a)|((b)<<8)|((c)<<16)|((d)<<24))
#endif

#if 0
typedef unsigned char byte;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
#endif

typedef unsigned short word;

typedef struct
{
	byte	identification[4];		// should be IWAD
	u32		numlumps;
	u32		infotableofs;
	u32		typeofs;
} wadinfo_t;


typedef struct
{
	u32		filepos;
	u32		size;
	byte	name[8];
} filelump_t;

typedef struct
{
	u32		filepos;
	u32		csize;
	u32		dsize;
	byte	ety;		//Entry Type
	byte	cmp;		//Compression
	u16		chn;		//Chain (ExWAD)
	byte	name[16];
} wad2lump_t;

typedef struct
{
	byte	identification[4];		// should be IWAD
	u32		infotableofs;
	u32		numlumps;
} packinfo_t;

typedef struct
{
	byte	name[56];
	u32		filepos;
	u32		size;
} packlump_t;



FILE		*wad_fd;
// filelump_t	wad_dir[2048];
wad2lump_t	wad_dir[16384];
wadinfo_t	wad_head;
int 		wad_n_lumps;
int			wad_types[256];
int			wad_ntypes;
int			wad_ldirid;
int			wad_dirid;

byte		*wad_data;
int			wad_rover;

byte		*iwad_data;
filelump_t	*iwad_dir;
wadinfo_t	*iwad_head;
int			iwad_size;

byte		*pack_data;
packlump_t	*pack_dir;
packinfo_t	*pack_head;

int		wad_mincsz;
int		wad_maxcsz;
int 	wad_cmp[16];

void w_strupr_n (char *t, char *s, int n)
{
	int i;
	for(i=0; *s && (i<n); i++)
		{ *t++ = toupper(*s++); }
	for(; i<n; i++)
		*t++=0;
}

void w_strlwr_n (char *t, char *s, int n)
{
	int i;
	for(i=0; *s && (i<n); i++)
		{ *t++ = tolower(*s++); }
	for(; i<n; i++)
		*t++=0;
}

int WadTypeForTag(int tag)
{
	int i, j, k;

	if(!tag)
		return(0);

	for(i=0; i<(0x80-0x20); i++)
	{
		j=i+0x20;
		if(wad_types[j]==tag)
			return(j);
		if(!wad_types[j])
		{
			wad_types[j]=tag;
			wad_ntypes=i+1;
			return(j);
		}
	}
	
	return(0);
}

int AddWadLump2(char *name, void *buf, int csz, int dsz, int cmp, int tag)
{
	byte *obuf;
	char tn[17];
	int osz, ety;
	int n;

	ety=WadTypeForTag(tag);

	if(csz!=dsz)
	{
		if(csz<wad_mincsz)
			wad_mincsz=csz;
		if(csz>wad_maxcsz)
			wad_maxcsz=csz;
	}

//	w_strupr_n(tn, name, 16);
	w_strlwr_n(tn, name, 16);
	
	n=wad_n_lumps++;
	wad_dir[n].filepos=wad_rover;
	wad_dir[n].csize=csz;
	wad_dir[n].dsize=dsz;
	wad_dir[n].ety=ety;
	wad_dir[n].cmp=cmp;
	wad_dir[n].chn=0;
	memcpy(wad_dir[n].name, tn, 16);
	
	memcpy(wad_data+wad_rover, buf, csz);
	wad_rover+=csz;
//	wad_rover=(wad_rover+15)&(~15);
//	wad_rover=(wad_rover+7)&(~7);
	
	return(n);
}

int AddWadLump(char *name, byte *buf, int isz, int tag)
{
	TgvLz_Context *ctx1, *ctx2;
	byte *ibuf;
	byte *obuf;
	byte *obuf1;
	byte *obuf2;
	int osz, osz1, osz2, n, cmp;

	ctx1=TgvLz_CreateContext();
	ctx2=TgvLz_CreateContextLZ4();

	ibuf=malloc(isz+24);
	memset(ibuf, 0, isz+24);
	memcpy(ibuf, buf, isz);
	
	obuf1=malloc(isz*2+1024);
	obuf2=malloc(isz*2+1024);
//	osz=TgvLz_DoEncode(ctx, ibuf, obuf, isz);
	osz1=TgvLz_DoEncodeSafe(ctx1, ibuf, obuf1, isz);
	osz2=TgvLz_DoEncodeSafe(ctx2, ibuf, obuf2, isz);
	TgvLz_DestroyContext(ctx1);
	TgvLz_DestroyContext(ctx2);
	
	if((osz1<=osz2) && (osz1>0))
	{
		obuf=obuf1;
		osz=osz1;
		cmp=3;
	}else if(osz2>0)
	{
		obuf=obuf2;
		osz=osz2;		
		cmp=4;
	}else
	{
		obuf=NULL;
		osz=-1;
		cmp=-1;
	}
	
//	if((1.5*osz)<isz)
//	if((1.3*osz)<isz)
//	if((1.2*osz)<isz)
//	if(((1.2*osz)<isz) && (osz>0))
//	if(((1.2*osz)<isz) && (osz>32))
	if(((1.2*osz)<isz) && (osz>16))
	{
		wad_cmp[cmp]++;
		n=AddWadLump2(name, obuf, osz, isz, cmp, tag);
	}else
	{
		wad_cmp[0]++;
		n=AddWadLump2(name, ibuf, isz, isz, 0, tag);
	}
	
	free(ibuf);
	free(obuf1);
	free(obuf2);
	
	return(n);
}

int DigitBase32(int v)
{
	if((v>=0) && (v<=9))
		return('0'+v);
	return('a'+(v-10));
}

int GetLumpNumForName(char *name)
{
	int i;
	
	for(i=0; i<wad_n_lumps; i++)
	{
		if(!strncmp(wad_dir[i].name, name, 16))
			return(i);
	}
	return(-1);
}

int GetDirLumpNumForName(char *name)
{
	int i;
	
	for(i=0; i<wad_n_lumps; i++)
	{
		if(wad_dir[i].cmp!=1)
			continue;
		if(!strncmp(wad_dir[i].name, name, 16))
			return(i);
	}
	return(-1);
}

int WadBuildPfxName(char *tn, char *name, int pfx)
{
	char *s, *t;

	memset(tn, 0, 16);

	if(!pfx)
	{
//		w_strlwr_n(tn, name, 16);
		s=name; t=tn;
		while(*s && (*s!='.'))
			*t++=tolower(*s++);
		*t++=0;

		return(0);
	}

	if(pfx<32)
	{
		tn[0]=DigitBase32(pfx&31);
		tn[1]='|';
//		w_strlwr_n(tn+2, name, 12);
		s=name; t=tn+2;
		while(*s && (*s!='.'))
			*t++=tolower(*s++);
		*t++=0;
		return(0);
	}

	if(pfx<1024)
	{
		tn[0]=DigitBase32((pfx>> 5)&31);
		tn[1]=DigitBase32((pfx>> 0)&31);
		tn[2]='|';
//		w_strlwr_n(tn+2, name, 12);
		s=name; t=tn+3;
		while(*s && (*s!='.'))
			*t++=tolower(*s++);
		*t++=0;
		return(0);
	}

	tn[0]=DigitBase32((pfx>>10)&31);
	tn[1]=DigitBase32((pfx>> 5)&31);
	tn[2]=DigitBase32((pfx>> 0)&31);
	tn[3]='|';
//	w_strlwr_n(tn+4, name, 12);
	s=name; t=tn+4;
	while(*s && (*s!='.'))
		*t++=tolower(*s++);
	*t++=0;
	return(0);
}

int GetWadPathId(char *name, int pfx, int tag)
{
	char tn[23];
	int i, n, id, ety;

	WadBuildPfxName(tn, name, pfx);

#if 0
	if(pfx)
	{
		tn[0]=DigitBase32((pfx>>10)&31);
		tn[1]=DigitBase32((pfx>> 5)&31);
		tn[2]=DigitBase32((pfx>> 0)&31);
		tn[3]='|';
//		strcpy(tn+4, name);
		w_strlwr_n(tn+4, name, 12);
	}else
	{
//		strcpy(tn, name);
		w_strlwr_n(tn, name, 16);
	}
#endif
	
	i=GetDirLumpNumForName(tn);
	if(i>=0)
	{
		id=(wad_dir[i].filepos)&32767;
		if((id<0) || (id>=wad_dirid))
			{ __debugbreak(); }
		return(id);
	}
	
	if(wad_dirid!=(wad_ldirid+1))
	{
		__debugbreak();
	}
	
	ety=WadTypeForTag(tag);

	id=wad_dirid++;
	if(id!=(wad_ldirid+1))
		{ __debugbreak(); }
	wad_ldirid=id;

	n=wad_n_lumps++;
	wad_dir[n].filepos=id;
	wad_dir[n].csize=0;
	wad_dir[n].dsize=0;
	wad_dir[n].ety=ety;
	wad_dir[n].cmp=1;
	wad_dir[n].chn=0;
	memcpy(wad_dir[n].name, tn, 16);

//	printf("new dir %04X\n", id);

	return(id);
}

int AddWadLumpPathI(char *name, byte *buf, int isz, int pfx)
{
	char tb[256];
	int tag, pfx1;
	char *s, *t;
	int i;

	tag=FccTagForName(name);
	
	s=name; t=tb;
	while(*s && (*s!='/'))
		*t++=*s++;
	*t++=0;

	if(!(*s))
	{
		WadBuildPfxName(tb, name, pfx);

#if 0
		tb[0]=DigitBase32((pfx>>10)&31);
		tb[1]=DigitBase32((pfx>> 5)&31);
		tb[2]=DigitBase32((pfx>> 0)&31);
		tb[3]='|';

		s=name; t=tb+4;
		while(*s && (*s!='.'))
			*t++=*s++;
		*t++=0;
#endif

		return(AddWadLump(tb, buf, isz, tag));
	}

	tag=FccTagForName(tb);
	pfx1=GetWadPathId(tb, pfx, tag);
	i=AddWadLumpPathI(s+1, buf, isz, pfx1);
	return(i);
}

int AddWadLumpPath(char *name, byte *buf, int isz)
{
	char tb[256];
	int tag, pfx;
	char *s, *t;
	int i;

	tag=FccTagForName(name);
	
	s=name; t=tb;
	while(*s && (*s!='/'))
		*t++=*s++;
	*t++=0;

	if(!(*s))
	{
		s=name; t=tb;
		while(*s && (*s!='.'))
			*t++=*s++;
		*t++=0;
		return(AddWadLump(tb, buf, isz, tag));
	}

	tag=FccTagForName(tb);
	pfx=GetWadPathId(tb, 0, tag);
	i=AddWadLumpPathI(s+1, buf, isz, pfx);
	return(i);
}


int FccTagForName(char *src)
{
	char tfc[5];
	int v;
	char *s;
	
	s=src+strlen(src);
	while((s>src) && (*s!='.'))
		s--;

	if(*s=='.')
	{
		w_strlwr_n(tfc, s+1, 4);

		if(!strcmp(tfc, "wav"))
			return(BGBCC_FOURCC('W', 'A', 'V', ' '));
		if(!strcmp(tfc, "bmp"))
			return(BGBCC_FOURCC('B', 'M', 'P', ' '));
		if(!strcmp(tfc, "avi"))
			return(BGBCC_FOURCC('A', 'V', 'I', ' '));
		if(!strcmp(tfc, "ico"))
			return(BGBCC_FOURCC('I', 'C', 'O', ' '));

		memcpy(&v, tfc, 4);
		return(v);
	}

	return(0);
}

int HashIndexForName16(char *s)
{
	int j, h;
	j = (((int *)(s))[0])+(((int *)(s))[1])+
		(((int *)(s))[2])+(((int *)(s))[3]);
	h = ((j*0xF14A83)>>24)&63;
	return(h);
}

void *LoadFile(char *name, int *rsz)
{
	FILE *fd;
	void *buf;
	int sz;
	
	fd=fopen(name, "rb");
	if(!fd)return(NULL);
	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	buf=malloc(sz+16);
	fread(buf, 1, sz, fd);
	fclose(fd);
	*rsz=sz;
	return(buf);
}

void StoreFile(char *name, void *buf, int sz)
{
	FILE *fd;
	
	fd=fopen(name, "wb");
	if(!fd)return;
	fwrite(buf, 1, sz, fd);
	fclose(fd);
}

int LookupIwadLumpNum(char *name)
{
	char tn[9];
	int i;

	memset(tn, 0, 9);
	strncpy(tn, name, 8);

	for(i=0; i<iwad_head->numlumps; i++)
	{
		if(!memcmp(tn, iwad_dir[i].name, 8))
			return(i);
	}
	return(-1);
}

u16		pal8to15[256];
byte	pal8toY[256];
byte	pal15to8[32768];

byte	pal8toY_C[256];
byte	pal8toY_M[256];
byte	pal8toY_Y[256];

int ColorSetupForPalette(byte *pal)
{
	int cr, cg, cb, cy;
	int dr, dg, db, de;
	int bn, be;
	int i, j, k;

	for(i=0; i<256; i++)
	{
		cr=pal[i*3+0];
		cg=pal[i*3+1];
		cb=pal[i*3+2];
		
		cy=(8*cg+5*cr+3*cb)/16;
		pal8toY[i]=cy;

		dr=(7*cr+2*cg+7*cb)/16;
		dg=(7*cr+7*cg+2*cb)/16;
		db=(2*cr+7*cg+7*cb)/16;
		pal8toY_C[i]=db;
		pal8toY_M[i]=dr;
		pal8toY_Y[i]=dg;

		cr=cr>>3;
		cg=cg>>3;
		cb=cb>>3;

		j=(cr<<10)|(cg<<5)|cb;
		pal8to15[i]=j;
	}

	for(i=0; i<32768; i++)
	{
		cr=(i>>10)&31;	cr=(cr<<3)|(cr>>2);
		cg=(i>> 5)&31;	cg=(cg<<3)|(cg>>2);
		cb=(i>> 0)&31;	cb=(cb<<3)|(cb>>2);
		
		bn=0; be=999999999;
		for(j=0; j<256; j++)
		{
			dr=pal[j*3+0]-cr;	dr=dr*dr;
			dg=pal[j*3+1]-cg;	dg=dg*dg;
			db=pal[j*3+2]-cb;	db=db*db;
			de=2*dg+dr+db;
			
			if(de<be)
				{ bn=j; be=de; }
		}
		
		pal15to8[i]=bn;
	}

	for(i=0; i<256; i++)
	{
		j=pal8to15[i];
		pal15to8[j]=i;
	}

	return(0);
}

static byte cce_prov;

int ColorCellEncode(byte *cs, byte *ct, int wxs)
{
	u16 pxc[16];
	byte pxy[16];
	byte *pal2Y, *cp2y;
	int acy, clra, clrb, pix, acy2, dmsk;
	int mr, mg, mb, mc, mcy, amcy;
	int nr, ng, nb, nc, ncy, ancy;
	int cr, cg, cb, cy;
	int dy, bdy;
	int x, y, z, c;
	int i, j, k;

	pal2Y=pal8toY; bdy=-1;
	for(k=0; k<4; k++)
	{
		switch(k)
		{
			case 0: cp2y=pal8toY; break;
			case 1: cp2y=pal8toY_C; break;
			case 2: cp2y=pal8toY_M; break;
			case 3: cp2y=pal8toY_Y; break;
		}
	
		mcy=256;
		ncy=-1;
		for(y=0; y<4; y++)
			for(x=0; x<4; x++)
		{
			z=y*4+x;
			c=cs[y*wxs+x];
			cy=cp2y[c];
			if(cy<mcy)
				mcy=cy;
			if(cy>ncy)
				ncy=cy;
		}
		
		dy=ncy-mcy;
		if(dy>bdy)
		{
			pal2Y=cp2y;
			bdy=dy;
		}
	}
	
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		z=y*4+x;
		c=cs[y*wxs+x];
//		pxy[z]=pal8toY[c];
		pxy[z]=pal2Y[c];
		pxc[z]=pal8to15[c];
	}
	
	acy=0;
	for(i=0; i<16; i++)
		acy+=pxy[i];
	acy/=16;
	
	mr=0;	mg=0;	mb=0;	mc=0;
	nr=0;	ng=0;	nb=0;	nc=0;
	for(i=0; i<16; i++)
	{
		c=pxc[i];
		cr=(c>>10)&31;
		cg=(c>> 5)&31;
		cb=(c>> 0)&31;
		cy=pxy[i];
		
		if(cy>acy)
			{ nr+=cr; ng+=cg; nb+=cb; nc++; }
		else
			{ mr+=cr; mg+=cg; mb+=cb; mc++; }
	}
	
	if(!nc)
		{ nr=mr; ng=mg; nb=mb; nc=mc; }
	
	if(mc>1)
		{ mr/=mc; mg/=mc; mb/=mc; }
	if(nc>1)
		{ nr/=nc; ng/=nc; nb/=nc; }
		
	clra=(nr<<10)|(ng<<5)|nb;
	clrb=(mr<<10)|(mg<<5)|mb;
	clra=pal15to8[clra];
	clrb=pal15to8[clrb];

//	ncy=pal8toY[clra];
	ncy=pal2Y[clra];
//	mcy=pal8toY[clrb];
	mcy=pal2Y[clrb];
	
//	amcy=(mcy+7*acy)/8;
//	ancy=(ncy+7*acy)/8;
	amcy=(15*acy+mcy)/16;
	ancy=(15*acy+ncy)/16;
	
	dmsk=0x5555;
	if(cce_prov&1)
		dmsk=0xAAAA;
	if(wxs!=4)
		dmsk=0x5A5A;
	
	pix=0;
	for(i=0; i<16; i++)
	{
		cy=pxy[i];
		acy2=(dmsk&(1<<i))?ancy:amcy;
//		if(cy>acy)
		if(cy>acy2)
			pix|=1<<i;
	}
	
	ct[0]=clra;
	ct[1]=clrb;
	ct[2]=pix;
	ct[3]=pix>>8;

	cce_prov++;

	return(0);
}

int ColorCellEncodeFlat(byte *src, byte *dst)
{
	int x, y;
	
	for(y=0; y<16; y++)
		for(x=0; x<16; x++)
	{
		ColorCellEncode(src+(y*4*64)+(x*4), dst+(y*16+x)*4, 64);
	}

	return(0);
}

int ColorCellPostEncode8(byte *cs, byte *ct)
{
	u16 pxc[16];
	byte pxy[16];
	byte *pal2Y, *cp2y;
	int acy, clra, clrb, pix, acy2;
	int mr, mg, mb, mc, mcy, amcy;
	int nr, ng, nb, nc, ncy, ancy;
	int cr, cg, cb, cy, dy, bdy;
	int x, y, z, c;
	int i, j, k;
	
	pal2Y=pal8toY; bdy=-1;
	for(k=0; k<4; k++)
	{
		switch(k)
		{
			case 0: cp2y=pal8toY; break;
			case 1: cp2y=pal8toY_C; break;
			case 2: cp2y=pal8toY_M; break;
			case 3: cp2y=pal8toY_Y; break;
		}
	
		mcy=256;
		ncy=-1;
		for(z=0; z<8; z++)
		{
			c=cs[z];
			cy=cp2y[c];
			if(cy<mcy)
				mcy=cy;
			if(cy>ncy)
				ncy=cy;
		}
		
		dy=ncy-mcy;
		if(dy>bdy)
		{
			pal2Y=cp2y;
			bdy=dy;
		}
	}
	
	for(i=0; i<8; i++)
	{
		c=cs[i];
//		pxy[i]=pal8toY[c];
		pxy[i]=pal2Y[c];
		pxc[i]=pal8to15[c];
	}
	
	acy=0;
	for(i=0; i<8; i++)
		acy+=pxy[i];
	acy/=8;
	
	mr=0;	mg=0;	mb=0;	mc=0;
	nr=0;	ng=0;	nb=0;	nc=0;
	for(i=0; i<8; i++)
	{
		c=pxc[i];
		cr=(c>>10)&31;
		cg=(c>> 5)&31;
		cb=(c>> 0)&31;
		cy=pxy[i];
		
		if(cy>acy)
			{ nr+=cr; ng+=cg; nb+=cb; nc++; }
		else
			{ mr+=cr; mg+=cg; mb+=cb; mc++; }
	}
	
	if(!nc)
		{ nr=mr; ng=mg; nb=mb; nc=mc; }
	
	if(mc>1)
		{ mr/=mc; mg/=mc; mb/=mc; }
	if(nc>1)
		{ nr/=nc; ng/=nc; nb/=nc; }
		
	clra=(nr<<10)|(ng<<5)|nb;
	clrb=(mr<<10)|(mg<<5)|mb;
	clra=pal15to8[clra];
	clrb=pal15to8[clrb];

//	ncy=pal8toY[clra];
	ncy=pal2Y[clra];
//	mcy=pal8toY[clrb];
	mcy=pal2Y[clrb];
	
//	amcy=(mcy+acy)/2;
//	ancy=(ncy+acy)/2;
//	amcy=(mcy+7*acy)/8;
//	ancy=(ncy+7*acy)/8;
	amcy=(15*acy+mcy)/16;
	ancy=(15*acy+ncy)/16;

	pix=0;
	for(i=0; i<8; i++)
	{
		cy=pxy[i];
//		acy2=(i&1)?ancy:amcy;
		acy2=((i&1)^(cce_prov&1))?ancy:amcy;
//		if(cy>acy)
		if(cy>acy2)
			pix|=1<<i;
	}
	
	ct[0]=clra;
	ct[1]=clrb;
	ct[2]=pix;

	cce_prov++;

	return(0);
}

int ColorCellPostEncode(byte *cs, byte *ct, int len)
{
	byte tb[16];
	byte *ct1, *ct1e;
	int l;
	
	l=len;
	while(l>0)
	{
		if(l<8)
		{
			ct1=tb; ct1e=tb+8;
			while((ct1e-ct1)>=l)
				{ memcpy(ct1, cs, l); ct1+=l; }
			memcpy(ct1, cs, ct1e-ct1);
			ColorCellPostEncode8(tb, ct);
		}else
		{
			ColorCellPostEncode8(cs, ct);
		}
		cs+=8;
		ct+=3;
		l-=8;
	}
	return(0);
}

int ColorCellPostEncodeC16(byte *cs, byte *ct, int len)
{
	byte tb[16];
	byte *ct1, *ct1e;
	int l;
	
	l=len;
	while(l>0)
	{
		if(l<16)
		{
			ct1=tb; ct1e=tb+16;
			while((ct1e-ct1)>=l)
				{ memcpy(ct1, cs, l); ct1+=l; }
			memcpy(ct1, cs, ct1e-ct1);
//			memset(tb, cs[0], 16);
//			memcpy(tb, cs, l);
			ColorCellEncode(tb, ct, 4);
		}else
		{
			ColorCellEncode(cs, ct, 4);
		}
		cs+=16;
		ct+=4;
		l-=16;
	}
	return(0);
}

int ColorCellEncodePicture(byte *src, byte *dst, int isz)
{
	int *pscol, *pdcol;
	short *pdcol16;
	byte *cs, *ct, *imgf;
	int xs, ys, xs1, ys1, xs2, ys2;
	int x, y, py, pl, sz, ofs, py0;
	int is8, is16, isflat;

	if(isz<16)
		return(0);

	xs=((u16 *)src)[0];
	ys=((u16 *)src)[1];
	
	if((xs>320) || (ys>200))
		return(0);
	
	pdcol16=NULL;
	
//	dst[0]='C';	dst[1]='P';	dst[2]='8';	dst[3]='A';
//	((u32 *)dst)[1]=isz+64;
//	memcpy(dst+8, src, 8);
	memcpy(dst, src, 8);
	pscol=(int *)(src+8);
//	pdcol=(int *)(dst+16);
	pdcol=(int *)(dst+8);
//	ct=dst+16+((xs+2)*4);
//	ct=dst+16+((xs+1)*4);
	ct=dst+8+((xs+1)*4);

	if(isz<16000)
	{
		pdcol16=(short *)pdcol;
		ct=dst+8+((xs+1)*2);
	}

	isflat=1;
	for(x=0; x<xs; x++)
	{
		py=pscol[x];
		
		if(py<(8+xs*4))
			break;
		if(py>=isz)
			break;

		cs=src+pscol[x];
		py=cs[0];
		pl=cs[1];
		
		if(!x)
			py0=py;
		
		if((py==0xFF) || (py!=py0) || ((pl<ys) && (pl!=128)))
			isflat=0;
	}
	if(xs<=4)
		isflat=0;
	
	if(x<xs)
		return(0);
	
	if(isflat)
	{
		pdcol16=(short *)pdcol;
		pdcol16[0]=16|0x4000;
		pdcol16[1]=py0|0x4000;

		xs2=(xs+3)>>2;
		ys2=(ys+3)>>2;
		xs1=xs2<<2;
		ys1=ys2<<2;
		
		imgf=dst+65536;

		for(x=0; x<xs; x++)
		{
			cs=src+pscol[x];
			py=cs[0];
			pl=cs[1];

			memset(imgf+x*ys1, cs[3+ys], ys1);
			memcpy(imgf+x*ys1, cs+3, ys);
			
			if((pl==128) && (pl<ys))
			{
				cs+=pl+4;
				memcpy(imgf+x*ys1+128, cs+3, ys-128);
			}
		}
		for(; x<xs1; x++)
			{ memcpy(imgf+(x+0)*ys1, imgf+(x-1)*ys1, ys1); }

		cs=imgf;
		ct=dst+16;
		for(x=0; x<xs2; x++)
		{
			for(y=0; y<ys2; y++)
			{
				ColorCellEncode(cs, ct, ys1);
				ct+=4;
				cs+=4;
			}
			cs+=3*ys1;
		}

		sz=ct-dst;
		if(sz>=isz)
			return(0);
		
		return(sz);
	}
	
//	is8=0;
	is8=1;
//	is16=1;
	is16=0;
	
	for(x=0; x<xs; x++)
	{
		if(x>0)
		{
			if(pscol[x]==pscol[x-1])
			{
				pdcol[x]=pdcol[x-1];
				continue;
			}
		}
	
		cs=src+pscol[x];

		ofs=ct-dst;
		
		if(pdcol16)
		{
			if(is8)
				{ ofs|=1<<14; }
			else if(is16)
				{ ofs|=2<<14; }
			else
				{ ofs|=3<<14; }
			pdcol16[x]=ofs;
		}else
		{
			if(is8)
				{ ofs|=1<<28; }
			else if(is16)
				{ ofs|=2<<28; }
			else
				{ ofs|=3<<28; }
			pdcol[x]=ofs;
		}

		py=cs[0];
		if(py==0xFF)
		{
			*ct++=0xFF;
			continue;
		}

		while(1)
		{
			py=cs[0];
			pl=cs[1];
			if(py==0xFF)
			{
				*ct++=0xFF;
				break;
			}
			
			ct[0]=py;
			ct[1]=pl;
			
			if(is8)
			{
				ColorCellPostEncode(cs+3, ct+2, pl);
				ct+=2+((pl+7)/8)*3;
			}else if(is16)
			{
				ColorCellPostEncodeC16(cs+3, ct+2, pl);
				ct+=2+((pl+15)/16)*4;
			}else
			{
				memcpy(ct+2, cs+3, pl);
				ct+=2+pl;
			}
			
			cs+=pl+4;
		}
	}
	
//	pdcol[xs]=BGBCC_FOURCC('C', 'P', '8', 'A');
	
	sz=ct-dst;
	if(sz>=isz)
		return(0);
	
	return(sz);
}

int CheckLumpNameIsPic(char *name, int sz)
{
	if(!memcmp(name, "M_", 2))
		return(1);
	if(!memcmp(name, "WI", 2))
		return(1);
	if(!memcmp(name, "ST", 2))
		return(1);

	if(!memcmp(name, "BRDR", 4))
		return(1);

	if(sz!=64000)
//	if(0)
	{
		if(!strncmp(name, "HELP1", 8))
			return(1);
		if(!strncmp(name, "HELP2", 8))
			return(1);
		if(!strncmp(name, "TITLEPIC", 8))
			return(1);
		if(!strncmp(name, "CREDIT", 8))
			return(1);
		if(!strncmp(name, "VICTORY2", 8))
			return(1);
		if(!strncmp(name, "PFUB1", 8))
			return(1);
		if(!strncmp(name, "PFUB2", 8))
			return(1);

		if(	!strncmp(name, "END0", 8) ||
			!strncmp(name, "END1", 8) ||
			!strncmp(name, "END2", 8) ||
			!strncmp(name, "END3", 8) ||
			!strncmp(name, "END4", 8) ||
			!strncmp(name, "END5", 8) ||
			!strncmp(name, "END6", 8) )
				return(1);
		if(	!strncmp(name, "AMMNUM0", 8) ||
			!strncmp(name, "AMMNUM1", 8) ||
			!strncmp(name, "AMMNUM2", 8) ||
			!strncmp(name, "AMMNUM3", 8) ||
			!strncmp(name, "AMMNUM4", 8) ||
			!strncmp(name, "AMMNUM5", 8) ||
			!strncmp(name, "AMMNUM6", 8) ||
			!strncmp(name, "AMMNUM7", 8) ||
			!strncmp(name, "AMMNUM8", 8) ||
			!strncmp(name, "AMMNUM9", 8) )
				return(1);
	}

	return(0);
}

int CheckLumpNameIsSound(char *name, int sz)
{
	if(!memcmp(name, "DS", 2))
		return(1);
	return(0);
}

int CheckLumpNameIsMus(char *name, int sz)
{
	if(!memcmp(name, "D_", 2))
		return(1);
	return(0);
}

int TryRecodeSound(byte *src, byte *dst, int isz)
{
	byte *cs, *ct;
	int pred, samp, pd, ts;
	int tg, rt, rt1, len, len1;
	int i, j, k;
	
	tg=((u16 *)src)[0];
	rt=((u16 *)src)[1];
	len=((u16 *)src)[2];
	
	if(tg!=3)
		return(0);
	if(rt!=11025)
		return(0);
	
//	rt1=5512;
	rt1=4000;
//	len1=(len*8000)/11025;
	len1=(len*rt1)/11025;
	
	if(1)
	{
		((u16 *)dst)[0]=7;
//		((u16 *)dst)[1]=8000;
		((u16 *)dst)[1]=rt1;
		((u16 *)dst)[2]=len1;
		((u16 *)dst)[3]=0;
		
		cs=src+8;
		ct=dst+8;
		
		pred=128;
		for(i=0; i<len1; i+=2)
		{
//			j=((i+0)*11025)/8000;
			j=((i+0)*11025)/rt1;
			samp=cs[j];
			pd=samp-pred;
			pd=(pd>=0)?(sqrt(pd)+0.5):(-(sqrt(-pd)+0.5));
			pd+=8;
			if(pd<0)pd=0;
			if(pd>15)pd=15;

			ts=pd;

			j=pd-7;
			if(j>=0)	j=j*j;
			else		j=-(j*j);
			pred=pred+j;

//			j=((i+1)*11025)/8000;
			j=((i+1)*11025)/rt1;
			samp=cs[j];
			pd=samp-pred;
			pd=(pd>=0)?(sqrt(pd)+0.5):(-(sqrt(-pd)+0.5));
			pd+=8;
			if(pd<0)pd=0;
			if(pd>15)pd=15;

			ts|=(pd<<4);

			j=pd-7;
			if(j>=0)	j=j*j;
			else		j=-(j*j);
			pred=pred+j;

			ct[i/2]=ts;
		}
		
		return(8+((len1+1)/2));
	}

	if(0)
	{
		((u16 *)dst)[0]=3;
		((u16 *)dst)[1]=8000;
		((u16 *)dst)[2]=len1;
		((u16 *)dst)[3]=0;
		
		cs=src+8;
		ct=dst+8;
		
		for(i=0; i<len1; i++)
		{
			j=(i*11025)/8000;
			k=cs[j];
			ct[i]=k;
		}
		
		return(len1+8);
	}

	return(0);
}

int CheckLumpNameIsMap(char *name, int sz)
{
	if(!memcmp(name, "MAP", 3))
	{
		if(	(name[3]>='0') && (name[3]<='9') &&
			(name[4]>='0') && (name[4]<='9') &&
			(name[5]==0))
				return(1);
	}

	if(	(name[0]=='E') && (name[2]=='M') &&
		(name[1]>='0') && (name[1]<='9') &&
		(name[3]>='0') && (name[3]<='9') &&
		(name[4]==0))
			return(1);
	
	return(0);
}

int main(int argc, char *argv[])
{
	char tb[256];
	char tn[65];
	char *ifn, *ofn;
	byte *tfbuf;
	int tyofs, dirsz;
	int mode, docce;
	int f_start, f_end, f_totsz;
	int p_start, p_end, p_totsz;
	int s_start, s_end, s_totsz;
	int opic_sz, snd_sz, oth_sz;
	int map_sz, map_inh, mus_sz;
	int i, j, k;
	
//	ifn=argv[1];
//	ofn=argv[2];

	mode=0;
	docce=0;
	ifn=NULL;
	ofn=NULL;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "--dump"))
			{
				mode=4;
			}

			if(!strcmp(argv[i], "--docce"))
			{
				docce=1;
			}
			continue;
		}
		
		if(!ifn)
		{
			ifn=argv[i];
			continue;
		}
		if(!ofn)
		{
			ofn=argv[i];
			continue;
		}
	}

	if(!ifn)
	{
		printf("no input file\n");
		return(-1);
	}

	if(!ofn)
	{
		printf("no output file\n");
		return(-1);
	}

	wad_data=malloc(1<<26);
	wad_rover=16;
	wad_n_lumps=0;
	wad_ldirid=0;
	wad_dirid=1;

	wad_mincsz=99999999;
	wad_maxcsz=0;

	memset(wad_data, 0, 1<<26);

	iwad_data=LoadFile(ifn, &iwad_size);
	if(!iwad_data)
	{
		printf("Fail Load %s\n", ifn);
		return(0);
	}
	
	pack_data=iwad_data;
	iwad_head=(wadinfo_t *)iwad_data;
	pack_head=(packinfo_t *)pack_data;

	if(	!memcmp(iwad_head->identification, "IWAD", 4) ||
		!memcmp(iwad_head->identification, "PWAD", 4))
	{
		iwad_dir=(filelump_t *)(iwad_data+(iwad_head->infotableofs));

		f_totsz=0;	p_totsz=0;
		s_totsz=0;	opic_sz=0;
		snd_sz=0;	oth_sz=0;
		map_sz=0;	map_inh=0;
		mus_sz=0;

		if(docce)
		{
			j=LookupIwadLumpNum("PLAYPAL");
			if(j>=0)
				ColorSetupForPalette(iwad_data+iwad_dir[j].filepos);

			f_start=LookupIwadLumpNum("F_START")+1;
			f_end=LookupIwadLumpNum("F_END")-1;

			p_start=LookupIwadLumpNum("P_START")+1;
			p_end=LookupIwadLumpNum("P_END")-1;

			s_start=LookupIwadLumpNum("S_START")+1;
			s_end=LookupIwadLumpNum("S_END")-1;

			tfbuf=malloc(1<<17);
		}

		for(i=0; i<iwad_head->numlumps; i++)
		{
			printf("%d/%d\r", i, iwad_head->numlumps);
			
			if(mode==4)
			{
				w_strlwr_n(tn, iwad_dir[i].name, 8);
				tn[8]=0;
				
				sprintf(tb, "%s/%s.lmp", ofn, tn);

				StoreFile(tb,
					iwad_data+iwad_dir[i].filepos,
					iwad_dir[i].size);
				continue;
			}
			
			w_strupr_n(tn, iwad_dir[i].name, 8);
			tn[8]=0;
			
			if((i>=f_start) && (i<=f_end) &&
				(iwad_dir[i].size==4096) &&
				(docce&1))
			{
				ColorCellEncodeFlat(
					iwad_data+iwad_dir[i].filepos, tfbuf);
				AddWadLump(tn, tfbuf, 1024, 0);
				f_totsz+=1024;
				continue;
			}

			if((i>=p_start) && (i<=p_end) && (docce&1))
			{
				k=ColorCellEncodePicture(
					iwad_data+iwad_dir[i].filepos, tfbuf,
					iwad_dir[i].size);
				if(k>0)
				{
					p_totsz+=k;
					AddWadLump(tn, tfbuf, k, 0);
					continue;
				}
			}

			if((i>=s_start) && (i<=s_end) && (docce&1))
			{
				if(	(tn[5]=='2') || (tn[5]=='4') ||
					(tn[5]=='6') || (tn[5]=='8') )
						continue;
			
				k=ColorCellEncodePicture(
					iwad_data+iwad_dir[i].filepos, tfbuf,
					iwad_dir[i].size);
				if(k>0)
				{
					s_totsz+=k;
					AddWadLump(tn, tfbuf, k, 0);
					continue;
				}
			}
			
			if(CheckLumpNameIsPic(tn, iwad_dir[i].size) && (docce&1))
			{
				k=ColorCellEncodePicture(
					iwad_data+iwad_dir[i].filepos, tfbuf,
					iwad_dir[i].size);
				if(k>0)
				{
					AddWadLump(tn, tfbuf, k, 0);
					opic_sz+=k;
					continue;
				}
			}
			
			if(docce&1)
			{
				if(!strncmp(tn, "DP", 2))
					continue;
				if(!strncmp(tn, "GENMIDI", 8))
					continue;
				if(!strncmp(tn, "DMXGUS", 8))
					continue;
			}

			if(CheckLumpNameIsSound(tn, iwad_dir[i].size) && (docce&1))
			{
				k=TryRecodeSound(
					iwad_data+iwad_dir[i].filepos, tfbuf,
					iwad_dir[i].size);
				if(k>0)
				{
					AddWadLump(tn, tfbuf, k, 0);
					snd_sz+=k;
					continue;
				}
			}
			
			if(CheckLumpNameIsMus(tn, iwad_dir[i].size))
			{
				if(docce&1)
					continue;
			
				AddWadLump(tn,
					iwad_data+iwad_dir[i].filepos,
					iwad_dir[i].size, 0);
				mus_sz+=iwad_dir[i].size;
				continue;
			}
			
			if(CheckLumpNameIsMap(tn, iwad_dir[i].size))
			{
				map_inh=12;
			}

			AddWadLump(tn,
				iwad_data+iwad_dir[i].filepos,
				iwad_dir[i].size, 0);
			
			if(map_inh>0)
			{
				map_sz+=iwad_dir[i].size;
				map_inh--;
			}else
			{
				oth_sz+=iwad_dir[i].size;
			}
		}
		printf("\n");
		
		printf("Flts=%8d, Patch=%8d\n",
			f_totsz,	p_totsz);
		printf("Sprs=%8d, OPics=%8d\n",
			s_totsz,	opic_sz);
		printf("Snds=%8d, Mus  =%8d\n",
			snd_sz,		mus_sz);
		printf("Maps=%8d, Other=%8d\n",
			map_sz,		oth_sz);

	}else if(!memcmp(iwad_head->identification, "PACK", 4))
	{
		pack_dir=(packlump_t *)(pack_data+(pack_head->infotableofs));
		dirsz=pack_head->numlumps/64;

		for(i=0; i<dirsz; i++)
		{
			printf("%d/%d\r", i, dirsz);
			w_strlwr_n(tn, pack_dir[i].name, 56);
			tn[56]=0;
			AddWadLumpPath(tn,
				pack_data+pack_dir[i].filepos,
				pack_dir[i].size);
		}
		printf("\n");
	}

	if(mode==4)
	{
		return(0);
	}


	wad_rover=(wad_rover+3)&(~3);

	tyofs=wad_rover;
	memcpy(wad_data+wad_rover, wad_types+0x20, wad_ntypes*4);
	wad_rover+=wad_ntypes*4;

	wad_rover=(wad_rover+15)&(~15);
	
	memcpy(wad_head.identification, "WAD2", 4);
	wad_head.numlumps=wad_n_lumps;
	wad_head.infotableofs=wad_rover;
	wad_head.typeofs=tyofs;

	memcpy(wad_data+wad_rover, wad_dir, wad_n_lumps*32);
	wad_rover+=wad_n_lumps*32;
	
	memcpy(wad_data, &wad_head, sizeof(wad_head));

	printf("%d -> %d bytes %d%%\n", iwad_size, wad_rover,
		(100*wad_rover)/iwad_size);
	
	printf("csz: min=%d max=%d\n", wad_mincsz, wad_maxcsz);
	
	for(i=0; i<16; i++)
	{
		if(wad_cmp[i])
			printf("%d: %d\n", i, wad_cmp[i]);
	}
	
	StoreFile(ofn, wad_data, wad_rover);

	free(wad_data);
}
