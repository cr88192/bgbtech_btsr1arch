#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "bt1h_common.h"
#include "bt1h_rice.c"
#include "bt1h_decrice.c"

typedef unsigned char byte;


int trans_stf(byte *ibuf, byte *obuf, int sz)
{
	byte stfwin[256];
	byte stfidx[256];
	int i0, i1, i2, i3;
	int i, j, k, l;
	
	for(i=0; i<256; i++)
	{
		stfwin[i]=i;
		stfidx[i]=i;
	}
	
	for(i=0; i<sz; i++)
	{
		j=ibuf[i];
		k=stfidx[j];
		
		if(k>0)
		{
			i0=stfwin[k-1];
			i1=stfwin[k  ];
			stfwin[k-1]=i1;
			stfwin[k  ]=i0;
			stfidx[i0]=k;
			stfidx[i1]=k-1;
		}
		obuf[i]=k;
	}
	return(0);
}

int trans_mtf(byte *ibuf, byte *obuf, int sz)
{
	byte stfwin[256];
	byte stfidx[256];
	int i0, i1, i2, i3;
	int i, j, k, l;
	
	for(i=0; i<256; i++)
	{
		stfwin[i]=i;
		stfidx[i]=i;
	}
	
	for(i=0; i<sz; i++)
	{
		j=ibuf[i];
		k=stfidx[j];

		for(l=k; l>0; l--)
		{
			i0=stfwin[l-1];
			stfwin[l]=i0;
			stfidx[i0]=l;
		}

		stfwin[0]=j;
		stfidx[j]=0;

#if 0		
		if(k>0)
		{
			i0=stfwin[k-1];
			i1=stfwin[k  ];
			stfwin[k-1]=i1;
			stfwin[k  ]=i0;
			stfidx[i0]=k;
			stfidx[i1]=k-1;
		}
#endif

		obuf[i]=k;
	}
	return(0);
}

int trans_smtf(byte *ibuf, byte *obuf, int sz)
{
	byte stfwin[256];
	byte stfidx[256];
	byte rov;
	int i0, i1, i2, i3;
	int i, j, k, l;
	
	for(i=0; i<256; i++)
	{
		stfwin[i]=i;
		stfidx[i]=i;
	}
	
	rov=0;
	for(i=0; i<sz; i++)
	{
		j=ibuf[i];
		k=(byte)(stfidx[j]-rov);
		obuf[i]=k;

		if(!k)continue;
		if(k<64)
		{
			i2=(rov+(k-1))&255;
			i3=(rov+(k  ))&255;
			i0=stfwin[i2];	i1=stfwin[i3];
			stfwin[i2]=i1;	stfwin[i3]=i0;
			stfidx[i0]=i3;	stfidx[i1]=i2;
			continue;
		}

		rov--;
		i2=(rov+k+1)&255;
		i3=rov&255;
		i0=stfwin[i2];	i1=stfwin[i3];
		stfwin[i2]=i1;	stfwin[i3]=i0;
		stfidx[i0]=i3;	stfidx[i1]=i2;
	}
	return(0);
}

int trans_fmtf(byte *ibuf, byte *obuf, int sz)
{
	byte stfwin[256];
	byte stfidx[256];
	byte rov;
	int i0, i1, i2, i3;
	int i, j, k, l;
	
	for(i=0; i<256; i++)
	{
		stfwin[i]=i;
		stfidx[i]=i;
	}
	
	rov=0;
	for(i=0; i<sz; i++)
	{
		j=ibuf[i];
		k=(byte)(stfidx[j]-rov);
		obuf[i]=k;

		if(!k)continue;
		rov--;
		i2=(rov+k+1)&255;
		i3=rov&255;
		i0=stfwin[i2];	i1=stfwin[i3];
		stfwin[i2]=i1;	stfwin[i3]=i0;
		stfidx[i0]=i3;	stfidx[i1]=i2;
	}
	return(0);
}

int trans_stf2(byte *ibuf, byte *obuf, int sz)
{
	byte stfwin[256];
	byte stfidx[256];
	byte rov;
	int i0, i1, i2, i3;
	int i, j, k, l;
	
	for(i=0; i<256; i++)
	{
		stfwin[i]=i;
		stfidx[i]=i;
	}
	
	rov=0;
	for(i=0; i<sz; i++)
	{
		j=ibuf[i];
		k=(byte)(stfidx[j]-rov);
		obuf[i]=k;

		i3=(rov+(k  ))&255;
//		i2=(rov+(k-1))&255;
//		i2=(i3*7)>>3;
		i2=(i3*7+3)>>3;
//		i2=(i3*15)>>4;
//		i2=(i3*3)>>2;
		i0=stfwin[i2];	i1=stfwin[i3];
		stfwin[i2]=i1;	stfwin[i3]=i0;
		stfidx[i0]=i3;	stfidx[i1]=i2;
	}
	return(0);
}

int count_adrice(byte *ibuf, int sz, int *rk)
{
	int i, k, n;
	
	n=0; k=2;
	for(i=0; i<sz; i++)
		n+=BTIC1H_Rice_CountWriteAdRice(NULL, ibuf[i], &k);
	if(rk)*rk=k;
	return(n);
}

int count_adricedc(byte *ibuf, int sz, int *rk)
{
	int i, k, n;
	
	n=0; k=2;
	for(i=0; i<sz; i++)
		n+=BTIC1H_Rice_CountWriteAdRiceDc(NULL, ibuf[i], &k);
	if(rk)*rk=k;
	return(n);
}

int log2statbuf(byte *ibuf, int fsz, int *istat, float *fstat)
{
	double f, g, h;
	int i, j, k;
	
	for(i=0; i<256; i++)
		istat[i]=0;
	
	for(i=0; i<fsz; i++)
	{
		j=ibuf[i];
		istat[j]++;
	}
	
	for(i=0; i<256; i++)
	{
		if(!istat[i])
		{
			fstat[i]=0;
			continue;
		}
	
		fstat[i]=log(((double)fsz)/istat[i])/log(2.0);
	}
}

int test_trans(byte *ibuf, byte *obuf, int fsz, int tn)
{
	int istat[256];
	float fstat[256];
	int (*trans)(byte *ibuf, byte *obuf, int sz);
	char *tnstr;
	double f, g, ft0;
	long long tsz, te;
	int t0, t1, t2, t3;
	int k0, k1;
//	int fsz;
	int i, j, k;

	switch(tn)
	{
		case 0: trans=trans_stf; tnstr="STF"; break;
		case 1: trans=trans_mtf; tnstr="MTF"; break;
		case 2: trans=trans_smtf; tnstr="SMTF"; break;
		case 3: trans=trans_fmtf; tnstr="FMTF"; break;
		case 4: trans=trans_stf2; tnstr="STF2"; break;
		default: trans=trans_stf; tnstr="STF"; break;
	}

	printf("Trans=%d: %s\n", tn, tnstr);

	t0=clock(); t1=t0; tsz=0;
	while(t1<(t0+(3*CLOCKS_PER_SEC)))
	{
//		trans_stf(ibuf, obuf, fsz);
		trans(ibuf, obuf, fsz);
		tsz+=fsz;
		
		t1=clock();
		t2=t1-t0;
		ft0=t2/((double)CLOCKS_PER_SEC);
		printf("  %.2fs %.2fMB/s\r", ft0, tsz/(1024.0*1024.0*ft0));
	}
	printf("\n");
	
	te=0;
	for(i=0; i<fsz; i++)
	{
		j=obuf[i];
		te+=j*j;
	}
	
	f=sqrt(te/((double)fsz));
	printf("  Avg RMSE=%f\n", f);
	
	i=count_adrice(obuf, fsz, &k0);
	j=count_adricedc(obuf, fsz, &k1);
	
	printf("  AdRice=%d(Avg=%.2f, Rk=%d) AdRiceDC=%d(Avg=%.2f, Rk=%d)\n",
		i, ((double)i)/fsz, k0,
		j, ((double)j)/fsz, k1);
	
	log2statbuf(obuf, fsz, istat, fstat);

#if 0
	for(i=0; i<16; i++)
	{
		for(j=0; j<16; j++)
		{
			printf("%.2f ", fstat[i*16+j]);
		}
		printf("\n");
	}
#endif
	
	j=0; k=0;
	for(i=0; i<256; i++)
	{
		j+=istat[i]*fstat[i];
		k+=istat[i]*((int)(fstat[i]+0.5));
	}
	
	printf("  OptF=%d, OptBit=%d\n", j, k);
}

int main(int argc, char *argv[])
{
	FILE *ifd;
	byte *ibuf;
	byte *tbuf1, *tbuf2;
	double f, g, ft0;
	long long tsz, te;
	int t0, t1, t2, t3;
	int fsz;
	int i, j, k;
	
//	ifd=fopen("StreetHolland.tga", "rb");
	ifd=fopen("bt1hvfw.dll", "rb");
//	ifd=fopen("bgbgc_log.txt", "rb");
	if(!ifd)
	{
		return(-1);
	}
	
	fseek(ifd, 0, 2);
	fsz=ftell(ifd);
	fseek(ifd, 0, 0);
	
	ibuf=malloc(fsz+64);
	fread(ibuf, 1, fsz, ifd);
	
	tbuf1=malloc(fsz+64);

	for(i=0; i<5; i++)
	{
		test_trans(ibuf, tbuf1, fsz, i);
	}
	
#if 0
	t0=clock(); t1=t0; tsz=0;
	while(t1<(t0+(10*CLOCKS_PER_SEC)))
	{
//		trans_stf(ibuf, tbuf1, fsz);
		trans_mtf(ibuf, tbuf1, fsz);
//		trans_smtf(ibuf, tbuf1, fsz);
//		trans_fmtf(ibuf, tbuf1, fsz);
		tsz+=fsz;
		
		t1=clock();
		t2=t1-t0;
		ft0=t2/((double)CLOCKS_PER_SEC);
		printf("%.2fs %.2fMB/s\r", ft0, tsz/(1024.0*1024.0*ft0));
	}
	printf("\n");
	
	te=0;
	for(i=0; i<fsz; i++)
	{
		j=tbuf1[i];
		te+=j*j;
	}
	
	f=sqrt(te/((double)fsz));
	printf("Avg RMSE=%f\n", f);
	
	i=count_adrice(tbuf1, fsz);
	j=count_adricedc(tbuf1, fsz);
	
	printf("AdRice=%d(Avg=%.2f) AdRiceDC=%d(Avg=%.2f)\n",
		i, ((double)i)/fsz, j, ((double)j)/fsz);
#endif
}
