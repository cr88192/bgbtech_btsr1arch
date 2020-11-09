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


u64 seed;

int ranhix(u64 hc)
{
	int v;
	seed=seed*hc+1;
//	v=(seed>>48)&0x7FFF;
	v=(seed>>48)&0xFFFF;
	return(v);
}

int adjbitpred(int v, byte *pp)
{
	int p, r;
	
	p=*pp;
	if(v)
	{
		r=(p>=(128+32));
		p++;
		if(p>255)p=255;
	}else
	{
		r=(p<=(128-32));
		p--;
		if(p<0)p=0;
	}
	*pp=p;
	return(r);
}

double ranbuf(u64 hc)
{
	static byte *buf=NULL;
	static byte *bpre=NULL;
	byte pred[4096];
	int cnts[256];
	double a, b, e;
	int i, j, k, l, pm, pb;

	if(!buf)
	{
		buf=malloc((1<<20)+16);
		bpre=malloc(4096*8);
	}

	memset(bpre, 128, 4096*8);

//	memset(buf, 0, 1<<20);
//	seed=0;

//	for(i=0; i<(1<<20); i++)
	for(i=0; i<(1<<19); i++)
	{
		j=ranhix(hc);
//		buf[i+0]^=j;
//		buf[i+1]^=j>>8;

		buf[i*2+0]=j;
		buf[i*2+1]=j>>8;
	}
	
	for(i=0; i<256; i++)
		cnts[i]=0;

	for(i=0; i<(1<<20); i++)
	{
		j=buf[i];
		cnts[j]++;
		
//		k=(buf[i+0]*251+buf[i+1]);
//		k=((k*251)>>8)&4095;
//		for(l=0; l<8; l++)
//			adjbitpred(j&(1<<l), bpre+(k*8+l));
	}

	for(i=0; i<4096; i++)
		pred[i]=0;
	
	pm=0; pb=0;
	for(i=0; i<(1<<20); i++)
	{
		j=(buf[i+0]*251+buf[i+1]);
		j=((j*251)>>8)&4095;
		k=buf[i+2];
		if(k==pred[j])
			pm++;
		pred[j]=k;

		for(l=0; l<8; l++)
		{
			pb+=adjbitpred(k&(1<<l), bpre+(j*8+l));
		}
	}

	a=0;
	for(i=0; i<256; i++)
		a+=cnts[i];
	a/=256;

	e=0;
	for(i=0; i<256; i++)
	{
		b=cnts[i]-a;
		e+=b*b;
	}
	
	b=pm*0.1;
	e+=b*b;
	b=pb*0.05*(1.0/8);
	e+=b*b;
	
	e=sqrt(e/256);
	return(e);
}

int main(int argc, char *argv[])
{
	u64 hc2[64];
	u64 hccha[256];
	float hce[64];
	int i, j, k;
	u64 uli, ulj, ulk, m;

	char *ifn, *ofn;
	double fa, fe, f, g;
	int tg, tf;

	for(i=0; i<64; i++)
	{
//		hc2[i]=15813251;
		uli=w4_rand64();
		uli&=0x0000FFFFFFFFFFFFULL;
		hc2[i]=uli;
	}

//	hc2[4]=15813251;
	hc2[4]=0x0000F7A83CF9E588ULL;
	hc2[5]=0xF050D0E00060ULL;
	hc2[6]=0xF0A0F01040B0ULL;
//	hc2[7]=0xF03080208020ULL;
	hc2[7]=0xF050D0E010E0ULL;

	hc2[ 8]=0x000008A13382BF77ULL;
	hc2[ 9]=0x00001A80A1BAADC3ULL;
	hc2[10]=0x00001CA8A3B7ADEFULL;
	hc2[11]=0x00001A80BF991553ULL;
	hc2[12]=0x0000FC4BA2F7ACABULL;
	hc2[13]=0x0000FC69A3F7ED2BULL;
	hc2[14]=0x0000F889B2778D43ULL;
	hc2[15]=0x0000B82B12571C7FULL;

	for(tg=0; tg<256; tg++)
	{
		seed=0;

		printf("%d\r", tg);
		for(tf=0; tf<64; tf++)
		{
//			seed=tg*tf;
//			seed=tf;
			seed=tf+tg;

			m=hc2[tf];
			fe=ranbuf(m);
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
		printf("fn=%016llX e=%f\n", m, fe);
		hccha[tg]=m;

		for(i=32; i<64; i++)
		{
//			ulj=hc2[rand()&7];
//			ulk=hc2[rand()&7];
			ulj=hc2[rand()&15];
			ulk=hc2[rand()&15];
//			ulj=hc2[rand()&31];
//			ulk=hc2[rand()&31];
			m=w4_rand64();
			uli=(ulj&m)|(ulk&(~m));
			j=rand()&63;
			uli^=1ULL<<j;
			uli&=0x0000FFFFFFFFFFFFULL;
//			uli&=0x00000000FFFFFFFFULL;
			hc2[i]=uli;
		}

		if(tg>2)
		{
			for(i=56; i<64; i++)
				hc2[i]=hccha[rand()%tg];
		}
	}
	
	printf("\n");

	for(k=0; k<4; k++)
	{
		m=hc2[k];
		fe=ranbuf(m);
		printf("fn=%016llX e=%f\n", m, fe);
	}
}
