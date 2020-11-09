#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

//#define TGVLZ_NOMAIN
//#include "tgvlz1.c"

#if 1
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


FILE		*wad_fd;
// filelump_t	wad_dir[2048];
wad2lump_t	wad_dir[16384];
wadinfo_t	wad_head;
int 		wad_n_lumps;

byte		*wad_data;
int			wad_rover;

byte		*iwad_data;
filelump_t	*iwad_dir;
wadinfo_t	*iwad_head;
int			iwad_size;

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

int AddWadLump2(char *name, void *buf, int csz, int dsz, int cmp)
{
	byte *obuf;
	char tn[17];
	int osz;
	int n;

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
	wad_dir[n].ety=0;
	wad_dir[n].cmp=cmp;
	wad_dir[n].chn=0;
	memcpy(wad_dir[n].name, tn, 16);
	
	memcpy(wad_data+wad_rover, buf, csz);
	wad_rover+=csz;
//	wad_rover=(wad_rover+15)&(~15);
	wad_rover=(wad_rover+7)&(~7);
	
	return(n);
}

#if 0
int AddWadLump(char *name, byte *buf, int isz)
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
		n=AddWadLump2(name, obuf, osz, isz, cmp);
	}else
	{
		wad_cmp[0]++;
		n=AddWadLump2(name, ibuf, isz, isz, 0);
	}
	
	free(ibuf);
	free(obuf1);
	free(obuf2);
	
	return(n);
}
#endif

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

int hashname(char *s)
{
	u32 v0, v1, v;
	v0=((u32 *)s)[0];
	v1=((u32 *)s)[1];
	v=v0+v1;
	v=v*15813251;
//	v=v*16777213;
//	v=v*0xFBFBFB;
//	v=v*(4093*4093);
//	v=v*0xF12345;
	v=(v>>24)&127;
	return(v);
}

int hashname2(char *s, int hc)
{
	u32 v0, v1, v;
	v0=((u32 *)s)[0];
	v1=((u32 *)s)[1];
	v=v0+v1;
	v=v*hc;
	v=(v>>24)&127;
	return(v);
}

int primetab[16384];
int n_primes;

int initprimes()
{
	int i, j, k, l, n;
	
	l=2; n=0;
	while(n<16384)
	{
//		printf("%d\r", l);
		for(i=0; i<n; i++)
			if((l%primetab[i])==0)
				break;
		if(i<n)
			{ l++; continue; }
		primetab[n++]=l;
	}
	n_primes=n;
	return(0);
}

int factorize(int val)
{
	int i, j, l, n;
	
	n=n_primes;
	l=val;
	while(l>1)
	{
		for(i=0; i<n; i++)
			if((l%primetab[i])==0)
				break;
		if(i<n)
		{
			j=primetab[i];
			printf(" %d", j);
			l=l/j;
			continue;
		}
		printf(" %d", l);
		break;
	}
	printf("\n");
	return(0);
}

int main(int argc, char *argv[])
{
	int hash[256];
	int hc2[64];
	float hce[64];
	char tn[9];
	char *ifn, *ofn;
	double fa, fe, f, g;
	int tg, tf;
	int i, j, k, l, m, n;
	int v0, v1;
	
//	ifn=argv[1];
//	ofn=argv[2];

	initprimes();

	ifn=NULL;
	ofn=NULL;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
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

//	if(!ofn)
//	{
//		printf("no output file\n");
//	}

	wad_data=malloc(1<<26);
	wad_rover=16;
	wad_n_lumps=0;

	wad_mincsz=99999999;
	wad_maxcsz=0;

	memset(wad_data, 0, 1<<26);

	iwad_data=LoadFile(ifn, &iwad_size);
	iwad_head=(wadinfo_t *)iwad_data;
	iwad_dir=(filelump_t *)(iwad_data+(iwad_head->infotableofs));

	for(i=0; i<64; i++)
	{
//		hc2[i]=15813251;
		l=rand()*rand()*rand();
		l&=0xFFFFFF;
		hc2[i]=l;
	}

	hc2[4]=15813251;
	hc2[5]=0xF5DE06;
	hc2[6]=0xFAF14B;
//	hc2[7]=0xF38282;
	hc2[7]=0xF5DE1E;

	for(tg=0; tg<256; tg++)
	{
		printf("%d\r", tg);
		for(tf=0; tf<64; tf++)
		{
			for(i=0; i<128; i++)
				{ hash[i]=0; }
			for(i=0; i<iwad_head->numlumps; i++)
			{
				w_strlwr_n(tn, iwad_dir[i].name, 8);
				j=hashname2(tn, hc2[tf]);
				hash[j&127]++;
			}
			
			fa=0; fe=0;
			for(i=0; i<128; i++)
				{ fa+=hash[i]; }
			fa/=128;
			for(i=0; i<128; i++)
			{
				f=hash[i]-fa;
				fe+=f*f;
			}
			hce[tf]=fe;
		}
		
		for(i=0; i<64; i++)
			for(j=i+1; j<64; j++)
		{
			if(hce[j]<hce[i])
			{
				f=hce[j]; hce[j]=hce[i]; hce[i]=f;
				k=hc2[j]; hc2[j]=hc2[i]; hc2[i]=k;
			}
		}
		
		for(i=8; i<64; i++)
		{
			j=hc2[rand()&7];
			k=hc2[rand()&7];
			m=(rand()*rand());
			l=(j&m)|(k&(~m));
			j=rand()&31;
			l^=1<<j;
			l&=0xFFFFFF;
			hc2[i]=l;
		}
	}

	hc2[1]=0xF14A83;
	hc2[2]=0xF5DE06;
//	hc2[3]=0xFAF14B;
//	hc2[3]=0xF38282;
//	hc2[3]=0xF5DE1E;
	hc2[3]=0xFEE051;

	for(l=0; l<4; l++)
	{
		tf=hc2[l];
		
//		printf("fn=%08X\n", tf);

		for(i=0; i<128; i++)
		{
			hash[i]=0;
		}

		for(i=0; i<iwad_head->numlumps; i++)
		{
			w_strlwr_n(tn, iwad_dir[i].name, 8);
	//		j=hashname(tn);
			j=hashname2(tn, tf);
			hash[j&127]++;
		}
		
		fa=0; fe=0;
		for(i=0; i<128; i++)
			{ fa+=hash[i]; }
		fa/=128;
		for(i=0; i<128; i++)
		{
			f=hash[i]-fa;
			fe+=f*f;
		}
		
//		printf("fn=%08X\n", tf);
//		printf("fn=%08X e=%f\n", tf, sqrt(fe/128));
		printf("fn=%08X e=%f", tf, sqrt(fe/128));
		factorize(tf);
	}


#if 0
	for(i=0; i<iwad_head->numlumps; i++)
	{
		printf("%d/%d\r", i, iwad_head->numlumps);
		w_strupr_n(tn, iwad_dir[i].name, 8);
		tn[8]=0;
		AddWadLump(tn,
			iwad_data+iwad_dir[i].filepos,
			iwad_dir[i].size);
	}
	printf("\n");

	wad_rover=(wad_rover+15)&(~15);
	
	memcpy(wad_head.identification, "WAD2", 4);
	wad_head.numlumps=wad_n_lumps;
	wad_head.infotableofs=wad_rover;

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
#endif
}
