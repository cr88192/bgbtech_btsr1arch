#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char byte;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

int tgt8b_perm[16777216];	//permutation index
u64 tgt8b_opra[16777216];	//permutation operations

u64 tgt8b_ovfa[16777216];	//permutation overflow.
int tgt8b_novf=1;

int LoadPermTab8B(char *fn)
{
	FILE *fd;
	u64 v;
	int i, j, k, n;
	
	fd=fopen(fn, "rb");
	if(!fd)
		return(-1);
	
	for(i=0; i<16777216; i++)
	{
		fread(&v, 1, 8, fd);
		
		if((v>>56)==0xFF)
		{
			tgt8b_perm[i]=-1;
			tgt8b_opra[i]=-1;
		}else
		{
			tgt8b_perm[i]=i;
			tgt8b_opra[i]=v;
		}
	}

	fread(&v, 1, 8, fd);
	n=v;
	tgt8b_novf=n;
	tgt8b_ovfa[0]=v;
	
	for(i=1; i<n; i++)
	{
		fread(&v, 1, 8, fd);
		tgt8b_ovfa[i]=v;
	}
	
	fclose(fd);

	return(0);
}

char *Shuf8B_IdToName(int id)
{
	char *s;
	switch(id)
	{
	case 0: s="NOP"; break;
	case 1: s="SWAP.LB"; break;
	case 2: s="SWAP.B"; break;
	case 3: s="SWAP.LW"; break;
	case 4: s="SWCP.LB"; break;
	case 5: s="SWAP.MB"; break;
	case 6: s="SWAP.HB"; break;
	case 7: s="SWAP.W"; break;
	case 8: s="SWAP.L"; break;
	case 9: s="SWCP.LW"; break;
	case 10: s="SWAP.MW"; break;
	case 11: s="SWAP.HW"; break;
	case 12: s="SWCP.MB"; break;
	case 13: s="SWCP.HB"; break;
	case 14: s="SWCP.MW"; break;
	case 15: s="SWCP.HW"; break;
	default: s="?"; break;
	}
	return(s);
}

u64 Shuf8B_SimSwapId(u64 v0, int id)
{
	u64 v1;
	
	switch(id)
	{
	case 0:
		v1=v0;
		break;
	case 1:
		v1= ((v0&0xFFFFFFFFFFFF0000ULL)   ) |
			((v0&0x000000000000FF00ULL)>>8) |
			((v0&0x00000000000000FFULL)<<8);
		break;
	case 2:
		v1= ((v0&0xFFFFFFFF00000000ULL)   ) |
			((v0&0x00000000FF00FF00ULL)>>8) |
			((v0&0x0000000000FF00FFULL)<<8);
		break;
	case 3:
		v1= ((v0&0xFFFFFFFF00000000ULL)    ) |
			((v0&0x00000000FFFF0000ULL)>>16) |
			((v0&0x000000000000FFFFULL)<<16);
		break;
	case 4:
		v1= ((v0&0xFFFFFFFFFFFF00FFULL)   ) |
			((v0&0x00000000000000FFULL)<<8);
		break;
	case 5:
		v1= ((v0&0xFFFFFFFFFF00FF00ULL)    ) |
			((v0&0x0000000000FF0000ULL)>>16) |
			((v0&0x00000000000000FFULL)<<16);
		break;
	case 6:
		v1= ((v0&0xFFFFFFFF00FFFF00ULL)    ) |
			((v0&0x00000000FF000000ULL)>>24) |
			((v0&0x00000000000000FFULL)<<24);
		break;
	case 7:
		v1=	((v0&0xFFFF0000FFFF0000ULL)>>16) |
			((v0&0x0000FFFF0000FFFFULL)<<16);
		break;
	case 8:
		v1=	((v0&0xFFFFFFFF00000000ULL)>>32) |
			((v0&0x00000000FFFFFFFFULL)<<32);
		break;
	case 9:
		v1= ((v0&0xFFFFFFFF0000FFFFULL)    ) |
			((v0&0x000000000000FFFFULL)<<16);
		break;
	case 10:
		v1= ((v0&0xFFFF0000FFFF0000ULL)    ) |
			((v0&0x0000FFFF00000000ULL)>>32) |
			((v0&0x000000000000FFFFULL)<<32);
		break;
	case 11:
		v1= ((v0&0x0000FFFFFFFF0000ULL)    ) |
			((v0&0xFFFF000000000000ULL)>>48) |
			((v0&0x000000000000FFFFULL)<<48);
		break;
	case 12:
		v1= ((v0&0xFFFFFFFFFF00FFFFULL)    ) |
			((v0&0x00000000000000FFULL)<<16);
		break;
	case 13:
		v1= ((v0&0xFFFFFFFF00FFFFFFULL)    ) |
			((v0&0x00000000000000FFULL)<<24);
		break;
	case 14:
		v1= ((v0&0xFFFF0000FFFFFFFFULL)    ) |
			((v0&0x000000000000FFFFULL)<<32);
		break;
	case 15:
		v1= ((v0&0x0000FFFFFFFFFFFFULL)    ) |
			((v0&0x000000000000FFFFULL)<<48);
		break;
	}
}

int Shuf8B_ShufLen(u64 opv)
{
	int i, j, k;
	if((opv>>60)==15)
	{
		for(i=0; i<8; i++)
		{
			if(!((opv>>(i*4))&15))
				break;
		}

		if((opv>>56)&8)
			return(i+32);
		return(i+16);
	}

	for(i=0; i<16; i++)
	{
		if(!((opv>>(i*4))&15))
			break;
	}
	return(i);
}

u64 Shuf8B_SimShufDirect(u64 v0, int id)
{
	u64 v1;
	int i, ix;
	
	v1=0;
	for(i=0; i<8; i++)
	{
		ix=(id>>(i*3))&7;
		v1|=((v0>>(ix*8))&255)<<(i*8);
	}
	return(v1);
}

int shuf8b_cyc;
int shuf8b_maxcyc;

u64 Shuf8B_SimShufPerm(u64 v0, int id)
{
	u64 opv, opv1, tv;
	int cyc0, ncyc;
	int i, j, k;

	opv=tgt8b_opra[id];
	
	cyc0=shuf8b_cyc;
	
	if(((opv>>60)&15)==15)
	{
		opv1=tgt8b_ovfa[(opv>>32)&0xFFFFFF];

		tv=v0;

		for(j=0; j<16; j++)
		{
			k=(opv1>>(j*4))&15;
			if(!k)
				break;
			tv=Shuf8B_SimSwapId(tv, k);
			shuf8b_cyc++;
		}

		for(j=0; j<8; j++)
		{
			k=(opv>>(j*4))&15;
			if(!k)
				break;
			tv=Shuf8B_SimSwapId(tv, k);
			shuf8b_cyc++;
		}

		ncyc=shuf8b_cyc-cyc0;
		if(ncyc>shuf8b_maxcyc)
			shuf8b_maxcyc=ncyc;
		return(tv);
	}
	
	tv=v0;
	for(j=0; j<16; j++)
	{
		k=(opv>>(j*4))&15;
		if(!k)
			break;
		tv=Shuf8B_SimSwapId(tv, k);
		shuf8b_cyc++;
	}

	ncyc=shuf8b_cyc-cyc0;
	if(ncyc>shuf8b_maxcyc)
		shuf8b_maxcyc=ncyc;
	return(tv);
}

#define SHUF8(a, b, c, d, e, f, g, h)				\
	(((a)    )|((b)<< 3)|((c)<< 6)|((d)<< 9)|		\
	 ((e)<<12)|((f)<<15)|((g)<<18)|((h)<<21))

int main()
{
	int i, j, k, l, bi, bl, vpf;
	u64 v, tv, v0, v1;

	printf("Load\n");
	LoadPermTab8B("swaps_tab8b.dat");

	printf("Validate\n");
	tv=0x1234567890ABCDEFULL;
	vpf=0;
	shuf8b_cyc=0;
	shuf8b_maxcyc=0;
	for(i=0; i<16777216; i++)
	{
		v0=Shuf8B_SimShufDirect(tv, i);
		v1=Shuf8B_SimShufPerm(tv, i);
		
		if(v0!=v1)
		{
			v=tgt8b_opra[i];
			printf("%06X v0=%016llX v1=%016llX opv=%016llX\n",
				i, v0, v1, v);
			vpf=1;
		}
	}

	printf("Validate: %s\n", vpf?"Fail":"Pass");
	printf("Cycles: Avg=%.2f Max=%d\n",
		shuf8b_cyc/16777216.0, shuf8b_maxcyc);
	
//	i=SHUF8(1, 3, 5, 7, 0, 0, 0, 0);
//	i=SHUF8(1, 3, 5, 7, 1, 3, 5, 7);
	i=SHUF8(1, 3, 5, 7, 0, 2, 4, 6);
	
	bi=i; bl=64;
	for(j=0; j<4096; j++)
	{
		i=(i&0x000FFF)|(j<<12);
		v=tgt8b_opra[i];
		l=Shuf8B_ShufLen(v);
		if(l<bl)
			{ bi=i; bl=l; }
	}
	
	i=bi;
	v=tgt8b_opra[i];
	
	printf("%06X %016llX\n", i, v);
	
	tv=0x1234567890ABCDEFULL;
	
	for(j=0; j<16; j++)
	{
		k=(v>>(j*4))&15;
		if(!k)
			break;
		printf(" %X(%s)", k, Shuf8B_IdToName(k));
		tv=Shuf8B_SimSwapId(tv, k);
	}
	printf("\n");
	
	printf("%16llX\n", tv);
}