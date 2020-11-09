#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#if 1
typedef unsigned char byte;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
#endif

typedef unsigned short word;


int hashname2(char *s, u64 hc)
{
	u64 v0, v1, v2, v3, v;

	v0=((u64 *)s)[0];
	v1=((u64 *)s)[1];
	v2=((u64 *)s)[2];
	v3=((u64 *)s)[3];
	v=v0+v1+v2+v3;
	v=((u32)v)+(v>>32);
//	v=((u32)v)+(v>>32);
	v=(u32)v;
	v=v*hc;

//	v=(v>>48)&255;
	v=(v>>32)&255;
	return(v);
}

u64 w4_rand64()
{
	u64 li;
	
	li=rand();
	li=(li*65521)+rand();
	li=(li*65521)+rand();
	li=(li*65521)+rand();
	li=(li*65521)+rand();
	return(li);
}

int main(int argc, char *argv[])
{
	char tb[256];

	int hash[256];
	u64 hc2[64];
	float hce[64];
	char tn[40];

	FILE *fd;
	char *lsarr;
	char *s, *t;
	int nlsa;
	int i, j, k;
	u64 uli, ulj, ulk, m;

	char *ifn, *ofn;
	double fa, fe, f, g;
	int tg, tf;


	lsarr=malloc(16384*32);
	memset(lsarr, 0, 16384*32);
	nlsa=0;
	
	fd=fopen("lslst0.txt", "rt");
	while(!feof(fd))
	{
		fgets(tb, 255, fd);
		
		s=tb+strlen(tb);
		while((s>tb) && (*s<=' '))
			{ *s=0; s--; }
		while((s>tb) && (*(s-1)!='/'))
			s--;
		strncpy(lsarr+nlsa*32, s, 32);
		nlsa++;
	}

#if 0
	for(i=0; i<nlsa; i++)
	{
		s=lsarr+i*32;
		printf("%s\n", s);
	}
#endif

	for(i=0; i<64; i++)
	{
//		hc2[i]=15813251;
		uli=w4_rand64();
		uli&=0x0000FFFFFFFFFFFFULL;
		hc2[i]=uli;
	}

//	hc2[4]=15813251;
	hc2[5]=0xF050D0E00060ULL;
	hc2[6]=0xF0A0F01040B0ULL;
//	hc2[7]=0xF03080208020ULL;
	hc2[7]=0xF050D0E010E0ULL;

	for(tg=0; tg<256; tg++)
	{
		printf("%d\r", tg);
		for(tf=0; tf<64; tf++)
		{
			for(i=0; i<256; i++)
				{ hash[i]=0; }
			for(i=0; i<nlsa; i++)
//			for(i=0; i<(nlsa/4); i++)
			{
//				j=rand()%nlsa;
//				j=(i<<2)^tg;
				memcpy(tn, lsarr+i*32, 32);
//				memcpy(tn, lsarr+j*32, 32);
				j=hashname2(tn, hc2[tf]);
				hash[j&255]++;
			}
			
			fa=0; fe=0;
			for(i=0; i<256; i++)
				{ fa+=hash[i]; }
			fa/=256;
			for(i=0; i<256; i++)
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
				m=hc2[j]; hc2[j]=hc2[i]; hc2[i]=m;
			}
		}

		m=hc2[0];
		fe=hce[0];
		printf("fn=%016llX e=%f\n", m, sqrt(fe/256));

//		for(i=8; i<64; i++)
		for(i=32; i<64; i++)
		{
//			ulj=hc2[rand()&7];
//			ulk=hc2[rand()&7];
			ulj=hc2[rand()&31];
			ulk=hc2[rand()&31];
			m=w4_rand64();
			uli=(ulj&m)|(ulk&(~m));
			j=rand()&63;
			uli^=1ULL<<j;
//			uli&=0x0000FFFFFFFFFFFFULL;
			uli&=0x00000000FFFFFFFFULL;
			hc2[i]=uli;
		}
	}
	
	printf("\n");

	for(k=0; k<4; k++)
	{
		m=hc2[k];
		
//		printf("fn=%08X\n", tf);

		for(i=0; i<256; i++)
		{
			hash[i]=0;
		}

		for(i=0; i<nlsa; i++)
		{
			memcpy(tn, lsarr+i*32, 32);
			j=hashname2(tn, hc2[k]);
			hash[j&255]++;
		}
		
		fa=0; fe=0;
		for(i=0; i<256; i++)
			{ fa+=hash[i]; }
		fa/=256;
		for(i=0; i<256; i++)
		{
			f=hash[i]-fa;
			fe+=f*f;
		}
		
		printf("fn=%016llX e=%f\n", m, sqrt(fe/256));
//		factorize(tf);
	}
}
