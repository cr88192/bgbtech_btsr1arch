/*

FELZ32: LZ in terms of 32-bit DWORDs
tag:
  bits  0..15: md, match distance (DWORDs)
  bits 16..22: ml, match length (DWORDs)
  bits 23..29: rl, raw length (DWORDs)
  bits 30..31: al, tag/align

ml!=0:
  md=distance
  ml=length
  mr=raw
  al=align (0=DWORD aligned, others: byte offset)

ml==0, rl==0, al==0:
	md==0: EOB
	md!=0: long run of raw DWORDs

*/

#define felz_getu32(ptr)		(*(u32 *)(ptr));
#define felz_getu64(ptr)		(*(u64 *)(ptr));
#define felz_setu32(ptr, val)	(*(u32 *)(ptr)=(val));
#define felz_setu64(ptr, val)	(*(u64 *)(ptr)=(val));

int TK_FeLz32_DecodeBuffer(
	u32 *ibuf, int ibsz, u32 *obuf, int obsz)
{
	u32 *cs, *cse, *cs1;
	u32 *ct, *ct1, *cte, *ct1e;
	u64 lv;
	u32 tg, v, v0, v1, v2, v3;
	u32 v4, v5, v6, v7;
	int ml, md, rl, al, alshr, alshl;
	int i, j, k;

	cs=ibuf; cse=cs+ibsz;
	ct=obuf; cte=ct+obsz;

	while(cs<cse)
	{
		tg=*cs++;

		ml=(tg>>16)&127;
		md=tg&65535;

		if(ml)
		{
			rl=(tg>>23)&127;
			al=(tg>>30)&3;

			if(rl)
			{
				ct1=ct; cs1=cs; ct1e=ct+rl;
				cs=cs+rl;		ct=ct1e;
				while(ct1<ct1e)
				{
					v0=cs1[0];		v1=cs1[1];
					v2=cs1[2];		ct1[0]=v0;	
					v3=cs1[3];		ct1[1]=v1;
					ct1[2]=v2;		ct1[3]=v3;
					ct1+=4;			cs1+=4;
				}
			}

			if(al)
			{
				cs1=ct-md;

				if(md<2)
				{
					if(al==1)
					{
						v=cs1[0]>>8;
						v0=v|(v<<24);
						v1=(v>>8)|(v<<16);
						v2=(v>>16)|(v<<8);

						ct1=ct; ct1e=ct+ml;
						while(ct1<ct1e)
							{ ct1[0]=v0; ct1[1]=v1; ct1[2]=v2; ct1+=3; }
						ct=ct1e;
					}else
					{
						if(al==3)
							{ v=cs1[0]>>24; v=v|(v<<8); v=v|(v<<16); }
						else
							{ v=cs1[0]>>16; v=v|(v<<16); }
						ct1=ct; ct1e=ct+ml;
						while(ct1<ct1e)
						{
							ct1[0]=v;	ct1[1]=v;
							ct1[2]=v;	ct1[3]=v;
							ct1+=4;
						}
						ct=ct1e;
					}
				}else
				{
					ct1=ct; ct+=ml;
					alshr=(4-al)<<3;
					alshl=al<<3;
					while(ct1<ct)
					{
						v0=cs1[0];		v1=cs1[1];
						v2=cs1[2];		v4=(v1<<alshl)|(v0>>alshr);
						v3=cs1[3];		v5=(v2<<alshl)|(v1>>alshr);
						ct1[0]=v4;		v0=cs1[4];
						v6=(v3<<alshl)|(v2>>alshr);		ct1[1]=v5;
						v7=(v0<<alshl)|(v3>>alshr);		ct1[2]=v6;
						ct1[3]=v7;
						cs1+=4; ct1+=4;
					}
				}
			}else
			{
				cs1=ct-md;
				ct1=ct; ct1e=ct+ml;
				ct=ct1e;
				if(md>2)
				{
					while(ct1<ct1e)
					{
						v0=cs1[0];		v1=cs1[1];
						v2=cs1[2];		ct1[0]=v0;	
						v3=cs1[3];		ct1[1]=v1;
						ct1[2]=v2;		ct1[3]=v3;
						ct1+=4;			cs1+=4;
					}
				}else
				{
					while(ct1<ct1e)
					{
						ct1[0]=cs1[0];	ct1[1]=cs1[1];
						ct1[2]=cs1[2];	ct1[3]=cs1[3];
						ct1+=4;			cs1+=4;
					}
				}
			}
			continue;
		}
		
		if(!tg)
			break;
		
		if(!(tg&0xFFFF0000U))
		{
			ct1=ct; cs1=cs; ct1e=ct+md;
			cs=cs+md;		ct=ct1e;
			while(ct1<ct1e)
			{
				v0=cs1[0];		v1=cs1[1];
				v2=cs1[2];		ct1[0]=v0;	
				v3=cs1[3];		ct1[1]=v1;
				ct1[2]=v2;		ct1[3]=v3;
				ct1+=4;			cs1+=4;
			}
			continue;
		}
	}
	
	return(ct-obuf);
}

int TK_FeLz32_EncodeBuffer(u32 *ibuf, int ibsz, u32 *obuf, int obsz)
{
	u32 hash[256];
	u32 *csb, *cs, *cse, *csr, *cs1;
	u32 *ct;
	u32 x, x0;
	int i, j, k, ll, rl, h, d, n;
	
	for(i=0; i<256; i++)
		hash[i]=0x0FFFFFFF;
	
	csb=ibuf;
	cse=csb+ibsz;
	cs=csb;
	
	ct=obuf;
	csr=cs;
	
	while(cs<cse)
	{
		x=*cs;
		x0=x+(x>>17);
		h=(x0+(x0>>11))&255;
		
		i=hash[h];
		cs1=csb+i;
		d=cs-cs1;
		if((d>0) && (d<65535))
		{
			x0=*cs1;
			if(x==x0)
//			if(0)
			{
				n=cse-cs;
				if(n>127)
					n=127;

				for(ll=0; ll<n; ll++)
				{
					if(cs[ll]!=cs1[ll])
						break;
				}

				if(ll>0)
				{
					rl=cs-csr;
					while(rl>127)
					{
						if(rl>65535)
							rl=65535;
						*ct++=rl;
						for(j=0; j<rl; j++)
							{ *ct++=*csr++; }
						rl=cs-csr;
					}
					
					rl=cs-csr;
					*ct++=(rl<<23)|(ll<<16)|d;
					for(j=0; j<rl; j++)
						{ *ct++=*csr++; }
					for(j=0; j<ll; j++)
					{
						x=*cs;
						x0=x+(x>>17);
						h=(x0+(x0>>11))&255;
						hash[h]=cs-csb;
						cs++;
					}
					csr=cs;
					continue;
				}
			}
		}

		rl=cs-csr;
		if(rl>=65535)
		{
			if(rl>65535)
				rl=65535;
//			*ct++=(rl<<23);
			*ct++=rl;
			for(j=0; j<rl; j++)
				{ *ct++=*csr++; }
		}
		
		hash[h]=cs-csb;
		cs++;
	}


	rl=cs-csr;
	while(rl>0)
	{
		if(rl>127)
			rl=127;
//		*ct++=(rl<<23);
		*ct++=rl;
		for(j=0; j<rl; j++)
			{ *ct++=*csr++; }
		rl=cs-csr;
	}

	*ct++=0;
	j=ct-obuf;
	return(j);
}

void TK_VMem_UnpackFeLz32Page(byte *cpi, byte *ppi)
{
	int i, j, k;
	
	j=(cpi[1]+1)<<(9-2);

	TK_FeLz32_DecodeBuffer(
		(u32 *)(cpi+4), j,
		(u32 *)ppi, 1<<(TKMM_PAGEBITS-2));
}

void TK_VMem_PackFeLz32Page(byte *cpi, byte *ppi)
{
	int i, j, k;

	j=TK_FeLz32_EncodeBuffer(
		(u32 *)ppi, 1<<(TKMM_PAGEBITS-2),
		(u32 *)(cpi+4), (1<<(TKMM_PAGEBITS-2))-2
		);
	
	j=(((j<<2)+4+511)>>9)-1;
	cpi[0]=7;
	cpi[1]=j;
}

