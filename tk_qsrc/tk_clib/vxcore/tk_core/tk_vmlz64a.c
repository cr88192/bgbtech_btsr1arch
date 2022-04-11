/*
  FeLz64

  Simplistic LZ format intended for Pagefile Compression.
  Focuses more on encoding speed rather than compression.
  Uncompressed data assumed to be page-sized.

  Encoded stream consists of 16-bit tags:
    rldd
  
  r: Raw Length (0..15 QWORDs)
  l: Match Length (0..15 QWORDs)
  d: Distance (0..255 QWORDs)
  
  With any raw QWORDs directly following the tag.
  During match decoding, the match will be copied after the raw
  values have been copied over.
  
  A tag of 0000 will be interpreted as an EOB.

  A match length of 0 means "No Match".
    (r!=0), (d==0): Span of raw QWORDs.
    (d!=0): Escape / Reserved.
  
  If (d==0) and (l!=0)
    Interpret as a run of zeroes.

  Compressed Block Format
    Byte 0: Method (2 in this case)
    Byte 1: Compressed Length, in 512B sectors, minus one
    Byte 2+: Compressed Data
 */

void TK_VMem_UnpackLz64Page(byte *cpi, byte *ppi)
{
	u64 *ctq, *csq, *ctqe;
	byte *cs;
	int tg, rl, ll, ld;
	
	cs=cpi+2;
	ctq=(u64 *)ppi;
	ctqe=ctq+(1<<(TKMM_PAGEBITS-3));
	while(1)
	{
		tg=*(u16 *)cs;
		cs+=2;
		if(!tg)
			break;
		rl=(tg>>12)& 15;
		ll=(tg>> 8)& 15;
		ld=(tg    )&255;

		if(rl)
		{
			csq=(u64 *)cs;
			while(rl--)
				{ *ctq++=*csq++; }
			cs=(byte *)csq;
		}
		
		if(ll)
		{
			if(ld)
			{
				csq=ctq-ld;
				while(ll--)
					{ *ctq++=*csq++; }
			}else
			{
				while(ll--)
					{ *ctq++=0; }
			}
		}else if(ld)
		{
			__debugbreak();
		}
	}
	
//	if(ctq!=ctqe)
//	{
//		printf("Size Error %d\n", ctqe-ctq);
//	}
}

void TK_VMem_PackLz64Page(byte *cpi, byte *ppi)
{
	u16 hash[256];
	u64 *ctq, *csqb, *csq, *csqe, *csqr, *csqs;
	byte *cs, *ct;
	u64 x, x0;
	int i, j, k, ll, rl, h, d, n;
	
	for(i=0; i<256; i++)
		hash[i]=0xFFFF;
	
	csqb=(u64 *)ppi;
	csqe=csqb+(1<<(TKMM_PAGEBITS-3));
	csq=csqb;
	
	ct=cpi+2;
	csqr=csq;
	
	while(csq<csqe)
	{
		x=*csq;
		x0=x+(x>>37);
		h=(x0+(x0>>13))&255;
		
		i=hash[h];
		csqs=csqb+i;
		d=csq-csqs;
		if((d>0) && (d<255))
		{
			x0=*csqs;
			if(x==x0)
//			if(0)
			{
				n=csqe-csq;
				if(n>15)
					n=15;

				for(ll=0; ll<n; ll++)
				{
					if(csq[ll]!=csqs[ll])
						break;
				}

#if 1
				if(x==0)
				{
					for(j=0; j<n; j++)
						if(csq[j]!=0)
							break;
//					if(j>(ll+1))
					if(j>ll)
						ll=0;
				}
#endif

				if(ll>0)
				{
					rl=csq-csqr;
					*(u16 *)ct=(rl<<12)|(ll<<8)|d;
					ct+=2;
//					j=rl;
//					while(j--)
					for(j=0; j<rl; j++)
						{ *(u64 *)ct=*csqr++; ct+=8; }
//					j=ll;
//					while(j--)
					for(j=0; j<ll; j++)
					{
						x=*csq;
						x0=x+(x>>37);
						h=(x0+(x0>>13))&255;
						hash[h]=csq-csqb;
						csq++;
					}
					csqr=csq;
					continue;
				}
			}
		}

#if 1
		if(x==0)
		{
			n=csqe-csq;
			if(n>15)
				n=15;

			for(ll=0; ll<n; ll++)
				if(csq[ll]!=0)
					break;

			if(ll>0)
			{
				rl=csq-csqr;
				*(u16 *)ct=(rl<<12)|(ll<<8)|0;
				ct+=2;
				for(j=0; j<rl; j++)
					{ *(u64 *)ct=*csqr++; ct+=8; }
				csq+=ll;
				csqr=csq;
				continue;
			}
		}
#endif

		rl=csq-csqr;
		if(rl>=15)
		{
			*(u16 *)ct=(rl<<12)|0x0000;
			ct+=2;
			for(j=0; j<rl; j++)
				{ *(u64 *)ct=*csqr++; ct+=8; }
		}
		
		hash[h]=csq-csqb;
		csq++;
	}


	rl=csq-csqr;
	while(rl>0)
	{
		if(rl>15)
			rl=15;
		*(u16 *)ct=(rl<<12)|0x0000;
		ct+=2;
//		j=rl;
//		while(j--)
		for(j=0; j<rl; j++)
			{ *(u64 *)ct=*csqr++; ct+=8; }
		rl=csq-csqr;
	}

	*(u16 *)ct=0x0000;
	ct+=2;
	
	j=ct-cpi;
	j=((j+511)>>9)-1;
	cpi[0]=2;
	cpi[1]=j;
}
