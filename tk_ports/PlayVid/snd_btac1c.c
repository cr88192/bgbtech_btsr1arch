// #include <bteifgl.h>

static const int bgbmid_ima_index_table[16] = {
  -1, -1, -1, -1, 2, 4, 6, 8,
  -1, -1, -1, -1, 2, 4, 6, 8
}; 

//static int bgbmid_ima_step_table[89] = { 
static const int bgbmid_ima_step_table[128] = { 
     7,     8,     9,    10,    11,    12,    13,    14,    16,    17, 
    19,    21,    23,    25,    28,    31,    34,    37,    41,    45, 
    50,    55,    60,    66,    73,    80,    88,    97,   107,   118, 
   130,   143,   157,   173,   190,   209,   230,   253,   279,   307,
   337,   371,   408,   449,   494,   544,   598,   658,   724,   796,
   876,   963,  1060,  1166,  1282,  1411,  1552,  1707,  1878,  2066, 
  2272,  2499,  2749,  3024,  3327,  3660,  4026,  4428,  4871,  5358,
  5894,  6484,  7132,  7845,  8630,  9493, 10442, 11487, 12635, 13899, 
 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767 
};

void BGBDT_MsImaAdpcm_DecodeBlockMono(
	byte *ibuf, s16 *obuf, int len)
{
	int pred, index, step, diff, uni, sni;
	int i, j;
	
	pred=(s16)(ibuf[0]+(ibuf[1]<<8));
	index=ibuf[2];
	
	step=bgbmid_ima_step_table[index&127];

	for(i=0; i<len; i++)
	{
		j=(ibuf[4+(i>>1)]>>((i&1)*4))&15;
		uni=j;
		sni=(j&8)?(-(j&7)):(j&7);
	
		index=index+bgbmid_ima_index_table[uni];
		index=(index<0)?0:((index>88)?88:index);
//		diff=(sni+0.5)*step/4;
		diff=((2*(uni&7)+1)*step)/8;
		if(uni&8)diff=-diff;
		pred=pred+diff;
		step=bgbmid_ima_step_table[index];

		pred=(pred<(-32768))?(-32768):((pred>32767)?32767:pred);
		obuf[i]=pred;
	}
}

void BGBDT_MsImaAdpcm_DecodeBlockMonoAsStereo(
	byte *ibuf, s16 *obuf, int len)
{
	byte *cs;
	s16 *ct, *cte;
	int pred, index, step, diff, uni, sni;
	int i, j;
	
//	pred=(s16)(ibuf[0]+(ibuf[1]<<8));
	pred=btic4b_gets16le(ibuf);
//	index=ibuf[2];
	index=ibuf[2]&127;
	
//	step=bgbmid_ima_step_table[index&127];
	step=bgbmid_ima_step_table[index];

#if 0
	ct=obuf; cte=obuf+(len*2);
	for(i=0; i<len; i++)
//	while(ct<cte)
	{
		j=(ibuf[4+(i>>1)]>>((i&1)*4))&15;
		uni=j;
		sni=(j&8)?(-(j&7)):(j&7);
	
		index=index+bgbmid_ima_index_table[uni];
//		index=(index<0)?0:((index>88)?88:index);
		index=lqtvq_clamp_0_88(index);
//		diff=(sni+0.5)*step/4;
//		diff=((2*(uni&7)+1)*step)/8;
		diff=((2*(uni&7)+1)*step)>>3;
		if(uni&8)diff=-diff;
		pred=pred+diff;
		step=bgbmid_ima_step_table[index];

//		pred=(pred<(-32768))?(-32768):((pred>32767)?32767:pred);
		pred=lqtvq_clampS16(pred);

//		ct[0]=pred;
//		ct[1]=pred;
//		ct+=2;
		obuf[i*2+0]=pred;
		obuf[i*2+1]=pred;
	}
#endif

#if 1
	cs=ibuf+4;
	ct=obuf; cte=obuf+(len*2);
	while(ct<cte)
	{
		i=*cs++;
		j=i&15;		uni=j;
		index=index+bgbmid_ima_index_table[uni];
		index=lqtvq_clamp_0_88(index);
		diff=((2*(uni&7)+1)*step)>>3;
		if(uni&8)diff=-diff;
		pred=pred+diff;
		step=bgbmid_ima_step_table[index];
		pred=lqtvq_clampS16(pred);
		ct[0]=pred;
		ct[1]=pred;

		j=(i>>4)&15;	uni=j;
		index=index+bgbmid_ima_index_table[uni];
		index=lqtvq_clamp_0_88(index);
		diff=((2*(uni&7)+1)*step)>>3;
		if(uni&8)diff=-diff;
		pred=pred+diff;
		step=bgbmid_ima_step_table[index];
		pred=lqtvq_clampS16(pred);
		ct[2]=pred;
		ct[3]=pred;
		ct+=4;
	}
#endif
}

void BGBDT_MsImaAdpcm_EncodeBlockMono(
	s16 *ibuf, byte *obuf, int len, int *ridx)
{
//	static byte lsbtab[16]={
//		0, 0, 0, 0, 0, 0, 0, 1,
//		0, 0, 0, 1, 0, 1, 1, 1};
	int p0, p1, p2, d0, d1, d2, uni1, uni2;
	int lsbt, lsbit;
	int pred, index, step, diff, uni, sni;
	int i, j, k;
	
//	pred=ibuf[0];
	pred=(ibuf[0]+ibuf[1])>>1;
//	pred=(ibuf[0]+ibuf[1]+ibuf[2]+ibuf[3])>>2;
	index=ridx?(*ridx):0;
	
	obuf[0]=pred&255;
	obuf[1]=(pred>>8)&255;
	obuf[2]=index;
	obuf[3]=0;
	
	memset(obuf+4, 0, (len+1)/2);
	
	step=bgbmid_ima_step_table[index&127];
	lsbt=0;

	for(i=0; i<len; i++)
	{
		j=ibuf[i];
		k=j-pred;

		if(k>=0)
		{
			sni=(k*4)/step;
			uni=(sni>7)?7:sni;
		}else
		{
			sni=(-k*4)/step;
			uni=(sni>7)?15:(8|sni);
		}

//		lsbt=(lsbt<<1)|(uni&1);
//		lsbit=lsbtab[(lsbt>>0)&15];

#if 1
		uni1=uni+1;
		uni2=uni-1;

		if((uni^uni1)&(~7))uni1=uni;
		if((uni^uni2)&(~7))uni2=uni;

#if 0
		if(lsbit)
		{
			uni|=1; uni1|=1; uni2|=1;
		}else
		{
			uni&=~1; uni1&=~1; uni2&=~1;
		}
#endif

		diff=((2*(uni&7)+1)*step)/8;
		if(uni&8)diff=-diff;
		p0=pred+diff; d0=j-p0;
//		d0=d0*d0;
		d0=(d0<<1)^(d0>>31);

		diff=((2*(uni1&7)+1)*step)/8;
		if(uni1&8)diff=-diff;
		p1=pred+diff; d1=j-p1;
//		d1=d1*d1;
		d1=(d1<<1)^(d1>>31);

		diff=((2*(uni2&7)+1)*step)/8;
		if(uni2&8)diff=-diff;
		p2=pred+diff; d2=j-p2;
//		d2=d2*d2;
		d2=(d2<<1)^(d2>>31);
		
		if(d1<d0)uni=uni1;
		if(d2<d0)uni=uni2;
#endif

//		uni|=1;
//		uni&=~1;
	
		index=index+bgbmid_ima_index_table[uni];
		index=(index<0)?0:((index>88)?88:index);
		diff=((2*(uni&7)+1)*step)/8;
		if(uni&8)diff=-diff;
		pred=pred+diff;
		step=bgbmid_ima_step_table[index];
		
		obuf[4+(i>>1)]|=uni<<((i&1)*4);
	}

	if(ridx)
		{ *ridx=index; }
}

void BGBDT_MsImaAdpcm_DecodeBlockStereo(
	byte *ibuf, s16 *obuf, int len)
{
	int lpred, lindex, lstep, ldiff, luni, lsni;
	int rpred, rindex, rstep, rdiff, runi, rsni;
	int i, j, k;
	
	lpred=(s16)(ibuf[0]+(ibuf[1]<<8));
	lindex=ibuf[2];

	rpred=(s16)(ibuf[4]+(ibuf[5]<<8));
	rindex=ibuf[6];
	
	lstep=bgbmid_ima_step_table[lindex&127];
	rstep=bgbmid_ima_step_table[rindex&127];

	for(i=0; i<len; i++)
	{
//		k=((i>>3)*4)+((i&7)>>1);
		k=((i>>3)*8)+((i&7)>>1);
		j=(ibuf[ 8+k]>>((i&1)*4))&15;
		luni=j;
		lsni=(j&8)?(-(j&7)):(j&7);

		j=(ibuf[12+k]>>((i&1)*4))&15;
		runi=j;
		rsni=(j&8)?(-(j&7)):(j&7);
	
		lindex=lindex+bgbmid_ima_index_table[luni];
		lindex=(lindex<0)?0:((lindex>88)?88:lindex);
//		ldiff=(lsni+0.5)*lstep/4;
		ldiff=((2*(luni&7)+1)*lstep)/8;
		if(luni&8)ldiff=-ldiff;
		lpred=lpred+ldiff;
		lstep=bgbmid_ima_step_table[lindex];

		rindex=rindex+bgbmid_ima_index_table[runi];
		rindex=(rindex<0)?0:((rindex>88)?88:rindex);
//		rdiff=(rsni+0.5)*rstep/4;
		rdiff=((2*(runi&7)+1)*rstep)/8;
		if(runi&8)rdiff=-rdiff;
		rpred=rpred+rdiff;
		rstep=bgbmid_ima_step_table[rindex];

//		lindex=(lindex<0)?0:((lindex>88)?88:lindex);
//		rindex=(rindex<0)?0:((rindex>88)?88:rindex);
		lpred=(lpred<(-32768))?(-32768):((lpred>32767)?32767:lpred);
		rpred=(rpred<(-32768))?(-32768):((rpred>32767)?32767:rpred);
		
		obuf[i*2+0]=lpred;
		obuf[i*2+1]=rpred;
	}
}

void BGBDT_MsImaAdpcm_EncodeBlockStereo(
	s16 *ibuf, byte *obuf, int len)
{
	int lpred, lindex, lstep, ldiff, luni, lsni;
	int rpred, rindex, rstep, rdiff, runi, rsni;
	int i, j, k;
	
	lpred=ibuf[0];
	lindex=0;
	rpred=ibuf[1];
	rindex=0;

	memset(obuf, 0, 8+((len+7)/8)*8);
	
	obuf[0]=lpred&255;
	obuf[1]=(lpred>>8)&255;
	obuf[2]=lindex;
	obuf[3]=0;

	obuf[4]=rpred&255;
	obuf[5]=(rpred>>8)&255;
	obuf[6]=rindex;
	obuf[7]=0;
	
//	memset(obuf+4, 0, (len+1)/2);
	
	lstep=bgbmid_ima_step_table[lindex&127];
	rstep=bgbmid_ima_step_table[rindex&127];

	for(i=0; i<len; i++)
	{
		ldiff=ibuf[i*2+0]-lpred;
		lsni=(ldiff*4)/lstep-0.5;
		lsni=(lsni<(-7))?(-7):((lsni>7)?7:lsni);
		luni=(lsni<0)?((-lsni)|8):lsni;

		rdiff=ibuf[i*2+0]-rpred;
		rsni=(rdiff*4)/rstep-0.5;
		rsni=(rsni<(-7))?(-7):((rsni>7)?7:rsni);
		runi=(rsni<0)?((-rsni)|8):rsni;
	
	
		lsni=(luni&8)?(-(luni&7)):(luni&7);
		lindex=lindex+bgbmid_ima_index_table[luni];
		lindex=(lindex<0)?0:((lindex>88)?88:lindex);
		ldiff=(lsni+0.5)*lstep/4;
		lpred=lpred+ldiff;
		lstep=bgbmid_ima_step_table[lindex];

		rsni=(runi&8)?(-(runi&7)):(runi&7);
		rindex=rindex+bgbmid_ima_index_table[runi];
		rindex=(rindex<0)?0:((rindex>88)?88:rindex);
		rdiff=(rsni+0.5)*rstep/4;
		rpred=rpred+rdiff;
		rstep=bgbmid_ima_step_table[rindex];
		
		k=((i>>3)*4)+((i&7)>>1);
		obuf[ 8+k]|=luni<<((i&1)*4);
		obuf[12+k]|=runi<<((i&1)*4);
//		obuf[i]=pred;
	}
}

int BGBDT_MsImaAdpcm_MonoSamplesFromBlockSize(int sz)
{
	return((sz-4)*2);
}

int BGBDT_MsImaAdpcm_MonoBlockSizeFromSamples(int len)
{
	return(4+((len+1)/2));
}

int BGBDT_MsImaAdpcm_StereoSamplesFromBlockSize(int sz)
{
	return(sz-8);
}

int BGBDT_MsImaAdpcm_StereoBlockSizeFromSamples(int len)
{
	return(8+((len+7)/8)*8);
}


/* BTAC1C */

void BGBDT_SndBTAC1C2_DecodeBlockMono(
	byte *ibuf, s16 *obuf, int len);

void BGBDT_SndBTAC1C2_DecodeMonoBlockStereo(
	byte *ibuf, s16 *obuf, int len);
void BGBDT_SndBTAC1C2_DecodeJointBlockStereo(
	byte *ibuf, s16 *obuf, int len);
void BGBDT_SndBTAC1C2_DecodeStereoBlockStereo(
	byte *ibuf, s16 *obuf, int len);

void BGBDT_SndBTAC1C_DecodeStereoBlockStereo(
	byte *ibuf, s16 *obuf, int len)
{
	int i, j;
	BGBDT_MsImaAdpcm_DecodeBlockStereo(ibuf, obuf, len>>1);
	
	for(i=len-1; i>0; i++)
	{
		j=i>>1;
		obuf[i*2+0]=obuf[j*2+0];
		obuf[i*2+1]=obuf[j*2+1];
	}
}

void BGBDT_SndBTAC1C_DecodeMonoBlockStereo(
	byte *ibuf, s16 *obuf, int len)
{
	int pred, index, step, diff, uni, sni, ofs;
	int lp, rp;
	int i, j;
	
	pred=(s16)(ibuf[0]+(ibuf[1]<<8));
	index=ibuf[2];
	ofs=(s16)(ibuf[4]+(ibuf[5]<<8));
	
	step=bgbmid_ima_step_table[index&127];

	for(i=0; i<len; i++)
	{
		j=(ibuf[8+(i>>1)]>>((i&1)*4))&15;
		uni=j;
		sni=(j&8)?(-(j&7)):(j&7);
	
		index=index+bgbmid_ima_index_table[uni];
		index=(index<0)?0:((index>88)?88:index);
		diff=((2*(uni&7)+1)*step)/8;
		if(uni&8)diff=-diff;
		pred=pred+diff;
		step=bgbmid_ima_step_table[index];

		pred=(pred<(-32768))?(-32768):((pred>32767)?32767:pred);
		rp=(pred<<1)-ofs;
		lp=rp+ofs;

		lp=(lp<(-32768))?(-32768):((lp>32767)?32767:lp);
		rp=(rp<(-32768))?(-32768):((rp>32767)?32767:rp);
		obuf[i*2+0]=lp;
		obuf[i*2+1]=rp;
	}
}

void BGBDT_SndBTAC1C_DecodeJointBlockStereo(
	byte *ibuf, s16 *obuf, int len)
{
	int pred, index, step, diff, uni, sni, ofs;
	int lp, rp;
	int p0, p1, p2, p3;
	int i, j, k, l;
	
	/* decode center channel */
	pred=(s16)(ibuf[0]+(ibuf[1]<<8));
	index=ibuf[2]&127;	
	step=bgbmid_ima_step_table[index&127];
	for(i=0; i<len; i++)
	{
		k=ibuf[8+(i>>2)*2+0]|((ibuf[8+(i>>2)*2+1])<<8);
		j=(k>>((i&3)*3))&7;
//		uni=(j<<1)|1;
//		uni=(j<<1)|(k>>15);
		uni=j<<1;
	
		index=index+bgbmid_ima_index_table[uni];
		index=(index<0)?0:((index>88)?88:index);
		diff=((2*(uni&7)+1)*step)/8;
		if(uni&8)diff=-diff;
		pred=pred+diff;
		step=bgbmid_ima_step_table[index];

		pred=(pred<(-32768))?(-32768):((pred>32767)?32767:pred);
		obuf[i*2+0]=pred;
	}
	
	l=len>>2;

	/* decode side channel */
	pred=(s16)(ibuf[4]+(ibuf[5]<<8));
	index=ibuf[6]&127;
	step=bgbmid_ima_step_table[index&127];
	for(i=0; i<l; i++)
	{
		k=ibuf[8+i*2+0]|((ibuf[8+i*2+1])<<8);
//		j=(k>>12)&7;
//		uni=(j<<1)|1;
//		uni=(j<<1)|(k>>15);
		uni=(k>>12)&15;
	
		index=index+bgbmid_ima_index_table[uni];
		index=(index<0)?0:((index>88)?88:index);
		diff=((2*(uni&7)+1)*step)/8;
		if(uni&8)diff=-diff;
		pred=pred+diff;
		step=bgbmid_ima_step_table[index];

		pred=(pred<(-32768))?(-32768):((pred>32767)?32767:pred);
		obuf[i*8+1]=pred;
	}

	/* center/side -> stereo */
	for(i=0; i<l; i++)
	{
		ofs=obuf[i*8+1];
//		ofs=0;
		for(j=0; j<4; j++)
		{
			k=(i*4+j)*2;
			pred=obuf[k+0];
//			ofs=obuf[k+1];
//			rp=(pred<<1)-ofs; lp=rp+ofs;
			lp=pred+ofs; rp=lp-(ofs<<1);
			lp=(lp<(-32768))?(-32768):((lp>32767)?32767:lp);
			rp=(rp<(-32768))?(-32768):((rp>32767)?32767:rp);
			obuf[k+0]=lp; obuf[k+1]=rp;
		}
	}
}


int BGBDT_SndBTAC1C_EncodeQuantUni3(
	int uni, int step, int pred, int tgt, int lsbit)
{
	int uni1, uni2;
	int diff, p0, p1, p2, d0, d1, d2;

	uni1=uni+1;
	uni2=uni-1;

	if((uni^uni1)&(~7))uni1=uni;
	if((uni^uni2)&(~7))uni2=uni;

	if(lsbit)
	{
		if(lsbit&1)
			{ uni|=1; uni1|=1; uni2|=1; }
		else
			{ uni&=~1; uni1&=~1; uni2&=~1; }
	}

	diff=((2*(uni&7)+1)*step)/8;
	if(uni&8)diff=-diff;
	p0=pred+diff; d0=tgt-p0;
//	d0=d0*d0;
	d0=(d0<<1)^(d0>>31);

	diff=((2*(uni1&7)+1)*step)/8;
	if(uni1&8)diff=-diff;
	p1=pred+diff; d1=tgt-p1;
//	d1=d1*d1;
	d1=(d1<<1)^(d1>>31);

	diff=((2*(uni2&7)+1)*step)/8;
	if(uni2&8)diff=-diff;
	p2=pred+diff; d2=tgt-p2;
//	d2=d2*d2;
	d2=(d2<<1)^(d2>>31);
	
	if(d1<d0)uni=uni1;
	if(d2<d0)uni=uni2;

	return(uni);
}

void BGBDT_SndBTAC1C_EncodeBlockStereoJS(
	s16 *ibuf, byte *obuf, int len, int *ridx)
{
	static byte lsbtab[32]={
		0, 1, 1, 2,	1, 2, 2, 3,
		1, 2, 2, 3, 2, 3, 3, 4,
		1, 2, 2, 3,	2, 3, 3, 4,
		2, 3, 3, 4, 3, 4, 4, 5};
	int p0, p1, p2, d0, d1, d2, uni1, uni2;
	int lsbt, lsbit;
	int cv, sv, len2;
	int cpred, spred, cindex, sindex, cstep, sstep, usblk;
	int tpred, tindex, tstep;
	int diff, uni, sni;
	int i, j, k;
	
//	pred=(ibuf[0]+ibuf[1])>>1;
//	cpred=(ibuf[0]+ibuf[1]+ibuf[2]+ibuf[3])>>2;
//	spred=((ibuf[0]+ibuf[2])>>1)-((ibuf[1]+ibuf[3])>>1);

	p0=(ibuf[(0*4+0)*2+0]+ibuf[(0*4+1)*2+0]+
		ibuf[(0*4+2)*2+0]+ibuf[(0*4+3)*2+0])>>2;
	p1=(ibuf[(0*4+0)*2+1]+ibuf[(0*4+1)*2+1]+
		ibuf[(0*4+2)*2+1]+ibuf[(0*4+3)*2+1])>>2;

	cpred=(p0+p1)>>1;
	spred=p0-cpred;

//	cindex=ridx?(*ridx):0;
//	sindex=cindex;
	
	if(ridx)
	{
		cindex=(*ridx)&127;
		sindex=((*ridx)>>8)&127;
	}
	
	obuf[0]=cpred&255;
	obuf[1]=(cpred>>8)&255;
	obuf[2]=cindex;
	obuf[3]=0;
	obuf[4]=spred&255;
	obuf[5]=(spred>>8)&255;
	obuf[6]=0x80|sindex;
	obuf[7]=0;
	
	memset(obuf+8, 0, 2*((len+3)/4));
	
	cstep=bgbmid_ima_step_table[cindex&127];
	sstep=bgbmid_ima_step_table[sindex&127];
//	lsbt=0;

	len2=len/4;
	for(i=0; i<len2; i++)
	{
//		lsbit=(lsbtab[lsbt&31]+lsbtab[(lsbt>>5)&31])>=5;
//		lsbit=(lsbtab[lsbt&15])>2;
//		lsbit=0;
		usblk=0;
		for(j=0; j<4; j++)
		{
			p0=ibuf[(i*4+j)*2+0];
			p1=ibuf[(i*4+j)*2+1];

			cv=(p0+p1)>>1;
			k=cv-cpred;

			if(k>=0)
			{
				sni=(k*4)/cstep;
				uni=(sni>7)?7:sni;
			}else
			{
				sni=(-k*4)/cstep;
				uni=(sni>7)?15:(8|sni);
			}
						
			uni=BGBDT_SndBTAC1C_EncodeQuantUni3(uni,
				cstep, cpred, cv, 2);

			cindex=cindex+bgbmid_ima_index_table[uni];
			cindex=(cindex<0)?0:((cindex>88)?88:cindex);
			diff=((2*(uni&7)+1)*cstep)/8;
			if(uni&8)diff=-diff;
			cpred=cpred+diff;
			cstep=bgbmid_ima_step_table[cindex];
			
			usblk|=(uni>>1)<<(j*3);
		}

		if(1)
		{
			p0=(ibuf[(i*4+0)*2+0]+ibuf[(i*4+1)*2+0]+
				ibuf[(i*4+2)*2+0]+ibuf[(i*4+3)*2+0])>>2;
			p1=(ibuf[(i*4+0)*2+1]+ibuf[(i*4+1)*2+1]+
				ibuf[(i*4+2)*2+1]+ibuf[(i*4+3)*2+1])>>2;

			cv=(p0+p1)>>1;
			sv=p0-cv;
			k=sv-spred;

			if(k>=0)
			{
				sni=(k*4)/sstep;
				uni=(sni>7)?7:sni;
			}else
			{
				sni=(-k*4)/sstep;
				uni=(sni>7)?15:(8|sni);
			}
			
			uni=BGBDT_SndBTAC1C_EncodeQuantUni3(uni,
				sstep, spred, sv, 0);

			sindex=sindex+bgbmid_ima_index_table[uni];
			sindex=(sindex<0)?0:((sindex>88)?88:sindex);
			diff=((2*(uni&7)+1)*sstep)/8;
			if(uni&8)diff=-diff;
			spred=spred+diff;
			sstep=bgbmid_ima_step_table[sindex];
			
//			usblk|=(uni>>1)<<12;
			usblk|=uni<<12;
		}

//		usblk|=lsbit<<15;
		obuf[8+i*2+0]=usblk;
		obuf[8+i*2+1]=usblk>>8;
	}

	if(ridx)
	{
		*ridx=cindex|(sindex<<8);
	}
}

void BGBDT_SndBTAC1C_DecodeBlockStereo(
	byte *ibuf, s16 *obuf, int len)
{
	int i;
	
	i=ibuf[6];

	if(i<89)
	{
//		BGBDT_SndBTAC1C_DecodeStereoBlockStereo(ibuf, obuf, len);
		BGBDT_SndBTAC1C2_DecodeStereoBlockStereo(ibuf, obuf, len);
		return;
	}

	if(i==89)
	{
//		BGBDT_SndBTAC1C_DecodeMonoBlockStereo(ibuf, obuf, len);
		BGBDT_SndBTAC1C2_DecodeMonoBlockStereo(ibuf, obuf, len);
		return;
	}

	if((i>=128) && (i<217))
	{
//		BGBDT_SndBTAC1C_DecodeJointBlockStereo(ibuf, obuf, len);
		BGBDT_SndBTAC1C2_DecodeJointBlockStereo(ibuf, obuf, len);
		return;
	}
}

void BGBDT_SndBTAC1C_DecodeBlockStereoLg2(
	byte *ibuf, s16 *obuf, int lg2)
{
	int i, j, k, l;
	BGBDT_SndBTAC1C_DecodeBlockStereo(ibuf, obuf, (1<<lg2)-16);
	
	l=1<<lg2;

	i=l-1;
	j=(i+1)>>(lg2-4);
	obuf[i*2+0]=obuf[(i-j)*2+0];
	obuf[i*2+1]=obuf[(i-j)*2+1];

	for(i=l-2; i>0; i--)
	{
		j=(i+1)>>(lg2-4);
		k=(i+0)>>(lg2-4);
//		obuf[i*2+0]=obuf[(i-j)*2+0];
//		obuf[i*2+1]=obuf[(i-j)*2+1];
		obuf[i*2+0]=(obuf[(i-j)*2+0]+obuf[(i-k)*2+0])/2;
		obuf[i*2+1]=(obuf[(i-j)*2+1]+obuf[(i-k)*2+1])/2;
	}
}


void BGBDT_SndBTAC1C_DecodeBlockMonoLg2(
	byte *ibuf, s16 *obuf, int lg2)
{
	int i, j, k, l;

	if(ibuf[3])
	{
		BGBDT_SndBTAC1C2_DecodeBlockMono(ibuf, obuf, (1<<lg2)-8);
	}else
	{
		BGBDT_MsImaAdpcm_DecodeBlockMono(ibuf, obuf, (1<<lg2)-8);
	}
	
#if 0
	l=1<<lg2;
	for(i=l-1; i>0; i--)
	{
		j=(i+1)>>(lg2-3);
		k=(i+0)>>(lg2-3);
//		obuf[i]=obuf[i-j];
		obuf[i]=(obuf[i-j]+obuf[i-k])/2;
	}
#endif

#if 1
	l=1<<lg2;

	i=l-1;
	j=(i+1)>>(lg2-3);
	obuf[i]=obuf[i-j];

	for(i=l-2; i>0; i--)
	{
		j=(i+1)>>(lg2-3);	k=(i+0)>>(lg2-3);
		obuf[i]=(obuf[i-j]+obuf[i-k])>>1;
//		obuf[i]=((obuf[i-j]+obuf[i-k]+7)>>4)<<1;
	}
#endif
}

void BGBDT_SndBTAC1C_EncodeBlockMonoLg2(
	s16 *ibuf, byte *obuf, int lg2, int *ridx)
{
	s16 stbuf[8192];
	int len, len2;
	int i, j, k;
	
	len2=(1<<lg2);
	len=len2-8;
	
	for(i=0; i<len2; i++)
	{
		j=i>>(lg2-3);
		stbuf[i-j]=ibuf[i];
	}
	
	BGBDT_MsImaAdpcm_EncodeBlockMono(stbuf, obuf, len, ridx);
}

void BGBDT_SndBTAC1C_EncodeBlockStereoLg2(
	s16 *ibuf, byte *obuf, int lg2, int *ridx)
{
	s16 stbuf[16384];
	int len, len2;
	int i, j, k;
	
	len2=(1<<lg2);
	len=len2-16;
	
	for(i=0; i<len2; i++)
	{
		j=i>>(lg2-4);
		stbuf[(i-j)*2+0]=ibuf[i*2+0];
		stbuf[(i-j)*2+1]=ibuf[i*2+1];
	}
	
	BGBDT_SndBTAC1C_EncodeBlockStereoJS(stbuf, obuf, len, ridx);
}

void BGBDT_SndBTAC1C_DecodeBlockMonoLg2_8b(
	byte *ibuf, byte *obuf, int lg2)
{
	s16 stbuf[8192];
	int i, j, k, l;

	BGBDT_MsImaAdpcm_DecodeBlockMono(ibuf, stbuf, (1<<lg2)-8);
	
	l=1<<lg2;
	for(i=l-1; i>0; i--)
	{
		j=(i+1)>>(lg2-3);
		obuf[i]=(stbuf[i-j]>>8)+128;
	}
}

void BGBDT_SndBTAC1C_DecodeBlockStereoLg2_8b(
	byte *ibuf, byte *obuf, int lg2)
{
	s16 stbuf[16384];
	int i, j, k, l;

	BGBDT_SndBTAC1C_DecodeBlockStereo(ibuf, stbuf, (1<<lg2)-16);
	
	l=1<<lg2;
	for(i=l-1; i>0; i--)
	{
		j=(i+1)>>(lg2-4);
		obuf[i*2+0]=(stbuf[(i-j)*2+0]>>8)+128;
		obuf[i*2+1]=(stbuf[(i-j)*2+1]>>8)+128;
	}
}

void BGBDT_SndBTAC1C_EncodeBlockMonoLg2_8b(
	byte *ibuf, byte *obuf, int lg2, int *ridx)
{
	s16 stbuf[8192];
	int len, len2;
	int i, j, k;
	
	len2=(1<<lg2);
	len=len2-8;
	
	for(i=0; i<len2; i++)
	{
		j=i>>(lg2-3);
		stbuf[i-j]=(ibuf[i]-128)<<8;
	}
	
	BGBDT_MsImaAdpcm_EncodeBlockMono(stbuf, obuf, len, ridx);
}

void BGBDT_SndBTAC1C_EncodeBlockStereoLg2_8b(
	byte *ibuf, byte *obuf, int lg2, int *ridx)
{
	s16 stbuf[16384];
	int len, len2;
	int i, j, k;
	
	len2=(1<<lg2);
	len=len2-16;
	
	for(i=0; i<len2; i++)
	{
		j=i>>(lg2-4);
		stbuf[(i-j)*2+0]=(ibuf[i*2+0]-128)<<8;
		stbuf[(i-j)*2+1]=(ibuf[i*2+1]-128)<<8;
	}
	
	BGBDT_SndBTAC1C_EncodeBlockStereoJS(stbuf, obuf, len, ridx);
}

void BGBDT_SndBTAC1C_DecodeBlockStereoFrLg2(
	byte *ibuf, s16 *obuf, int lg2)
{
	int i, j, k, l;
//	BGBDT_SndBTAC1C_DecodeBlockStereo(ibuf, obuf, (1<<lg2)-16);
	BGBDT_SndBTAC1C2_DecodeStereoBlockStereo(ibuf, obuf, (1<<lg2)-8);
	
	l=1<<lg2;
	for(i=l-1; i>0; i--)
	{
		j=(i+1)>>(lg2-3);
		obuf[i*2+0]=obuf[(i-j)*2+0];
		obuf[i*2+1]=obuf[(i-j)*2+1];
	}
}

void BGBDT_SndBTAC1C_DecodeBlockStereoFrLg2_8b(
	byte *ibuf, byte *obuf, int lg2)
{
	s16 stbuf[16384];
	int i, j, k, l;

//	BGBDT_SndBTAC1C_DecodeBlockStereo(ibuf, obuf, (1<<lg2)-16);
	BGBDT_SndBTAC1C2_DecodeStereoBlockStereo(ibuf, stbuf, (1<<lg2)-8);
	
	l=1<<lg2;
	for(i=l-1; i>0; i--)
	{
		j=(i+1)>>(lg2-3);
		obuf[i*2+0]=(stbuf[(i-j)*2+0]>>8)+128;
		obuf[i*2+1]=(stbuf[(i-j)*2+1]>>8)+128;
	}
}

int BGBDT_SndBTAC1C2_PredictSample(int *psamp, int idx, int pfcn)
{
	int pred, p0, p1;
	int i;
	
	i=idx;

	switch(pfcn)
	{
	case 0:
		pred=psamp[(i-1)&7];
		break;
	case 1:
		pred=2*psamp[(i-1)&7]-psamp[(i-2)&7];
		break;
	case 2:
		pred=(3*psamp[(i-1)&7]-psamp[(i-2)&7])>>1;
		break;
	case 3:
		pred=(5*psamp[(i-1)&7]-psamp[(i-2)&7])>>2;
		break;
	case 4:
		p0=(psamp[(i-1)&7]+psamp[(i-2)&7]);
		p1=(psamp[(i-2)&7]+psamp[(i-3)&7]);
		pred=p0-(p1>>1);
		break;
	case 5:
		p0=(psamp[(i-1)&7]+psamp[(i-2)&7]);
		p1=(psamp[(i-2)&7]+psamp[(i-3)&7]);
		pred=(3*p0-p1)>>2;
		break;
	case 6:
		p0=(psamp[(i-1)&7]+psamp[(i-2)&7]);
		p1=(psamp[(i-2)&7]+psamp[(i-3)&7]);
		pred=(5*p0-p1)>>3;
		break;
	case 7:
		pred=(	18*psamp[(i-1)&7]-4*psamp[(i-2)&7]+
				 3*psamp[(i-3)&7]-2*psamp[(i-4)&7]+
				 1*psamp[(i-5)&7])/16;
		break;
	case 8:
		pred=(	72*psamp[(i-1)&7]-16*psamp[(i-2)&7]+
				12*psamp[(i-3)&7]- 8*psamp[(i-4)&7]+
				 5*psamp[(i-5)&7]- 3*psamp[(i-6)&7]+
				 3*psamp[(i-7)&7]- 1*psamp[(i-8)&7])/64;
		break;
	case 9:
		pred=(	76*psamp[(i-1)&7]-17*psamp[(i-2)&7]+
				10*psamp[(i-3)&7]- 7*psamp[(i-4)&7]+
				 5*psamp[(i-5)&7]- 4*psamp[(i-6)&7]+
				 4*psamp[(i-7)&7]- 3*psamp[(i-8)&7])/64;
		break;
	case 10:
		p0=(psamp[(i-1)&7]+psamp[(i-2)&7]+psamp[(i-3)&7]+psamp[(i-4)&7]);
		p1=(psamp[(i-5)&7]+psamp[(i-6)&7]+psamp[(i-7)&7]+psamp[(i-8)&7]);
		pred=(5*p0-p1)>>4;
		break;
	case 11:
		p0=(psamp[(i-1)&7]+psamp[(i-2)&7]+psamp[(i-3)&7]+psamp[(i-4)&7]);
		p1=(psamp[(i-5)&7]+psamp[(i-6)&7]+psamp[(i-7)&7]+psamp[(i-8)&7]);
		pred=(p0+p1)>>3;
		break;

	default:
		pred=0;
		break;
	}
	return(pred);
}

void BGBDT_SndBTAC1C2_DecodeBlockMono(
	byte *ibuf, s16 *obuf, int len)
{
	int psamp[8];
	int p0, p1, p2, p3;
	int pred, pfcn, index, step, diff, uni, sni;
	int i, j;
	
	pred=(s16)(ibuf[0]+(ibuf[1]<<8));
	index=ibuf[2]&127;
	pfcn=ibuf[3]&15;
	
	psamp[0]=pred;	psamp[1]=pred;
	psamp[2]=pred;	psamp[3]=pred;
	psamp[4]=pred;	psamp[5]=pred;
	psamp[6]=pred;	psamp[7]=pred;
	
	step=bgbmid_ima_step_table[index&127];

	for(i=0; i<len; i++)
	{
		j=(ibuf[4+(i>>1)]>>((i&1)*4))&15;
		uni=j;
		sni=(j&8)?(-(j&7)):(j&7);

		if(pfcn)
			{ pred=BGBDT_SndBTAC1C2_PredictSample(psamp, i, pfcn); }

		index=index+bgbmid_ima_index_table[uni];
		index=(index<0)?0:((index>88)?88:index);
//		diff=(sni+0.5)*step/4;
		diff=((2*(uni&7)+1)*step)/8;
		if(uni&8)diff=-diff;
		pred=pred+diff;
		step=bgbmid_ima_step_table[index];

		pred=(pred<(-32768))?(-32768):((pred>32767)?32767:pred);
		obuf[i]=pred;
		psamp[i&7]=pred;
	}
}

void BGBDT_SndBTAC1C2_DecodeMonoBlockStereo(
	byte *ibuf, s16 *obuf, int len)
{
	int psamp[8];
	int pred, index, step, diff, uni, sni, ofs;
	int pfcn;
	int lp, rp;
	int i, j;
	
	pred=(s16)(ibuf[0]+(ibuf[1]<<8));
	index=ibuf[2]&127;
	ofs=(s16)(ibuf[4]+(ibuf[5]<<8));
	pfcn=ibuf[3]&15;
	
	psamp[0]=pred;	psamp[1]=pred;
	psamp[2]=pred;	psamp[3]=pred;
	psamp[4]=pred;	psamp[5]=pred;
	psamp[6]=pred;	psamp[7]=pred;
	
	step=bgbmid_ima_step_table[index&127];

	for(i=0; i<len; i++)
	{
		j=(ibuf[8+(i>>1)]>>((i&1)*4))&15;
		uni=j;
		sni=(j&8)?(-(j&7)):(j&7);

		if(pfcn)
			{ pred=BGBDT_SndBTAC1C2_PredictSample(psamp, i, pfcn); }

		index=index+bgbmid_ima_index_table[uni];
		index=(index<0)?0:((index>88)?88:index);
		diff=((2*(uni&7)+1)*step)/8;
		if(uni&8)diff=-diff;
		pred=pred+diff;
		step=bgbmid_ima_step_table[index];

		pred=(pred<(-32768))?(-32768):((pred>32767)?32767:pred);
//		rp=(pred<<1)-ofs;
//		lp=rp+ofs;
		lp=pred+ofs; rp=lp-(ofs<<1);

		lp=(lp<(-32768))?(-32768):((lp>32767)?32767:lp);
		rp=(rp<(-32768))?(-32768):((rp>32767)?32767:rp);
		obuf[i*2+0]=lp;
		obuf[i*2+1]=rp;
		psamp[i&7]=pred;
	}
}

void BGBDT_SndBTAC1C2_DecodeJointBlockStereo(
	byte *ibuf, s16 *obuf, int len)
{
	int psamp[8];
	int pred, index, step, diff, uni, sni, ofs;
	int lp, rp, pfcn, ispf;
	int p0, p1, p2, p3;
	int i, j, k, l;
	
	/* decode center channel */
	pred=(s16)(ibuf[0]+(ibuf[1]<<8));
	index=ibuf[2]&127;
	pfcn=ibuf[3]&15;
	step=bgbmid_ima_step_table[index&127];	
	psamp[0]=pred;	psamp[1]=pred;
	psamp[2]=pred;	psamp[3]=pred;
	psamp[4]=pred;	psamp[5]=pred;
	psamp[6]=pred;	psamp[7]=pred;
	
	ispf=(ibuf[3]!=0)|(ibuf[7]!=0);
	
	for(i=0; i<len; i++)
	{
		k=ibuf[8+(i>>2)*2+0]|((ibuf[8+(i>>2)*2+1])<<8);
		j=(k>>((i&3)*3))&7;
		uni=j<<1;
//		uni|=(uni>>2)&1;
		uni|=((uni&6)==6)&ispf;

		if(pfcn)
			{ pred=BGBDT_SndBTAC1C2_PredictSample(psamp, i, pfcn); }

		index=index+bgbmid_ima_index_table[uni];
		index=(index<0)?0:((index>88)?88:index);
		diff=((2*(uni&7)+1)*step)/8;
		if(uni&8)diff=-diff;
		pred=pred+diff;
		step=bgbmid_ima_step_table[index];

		pred=(pred<(-32768))?(-32768):((pred>32767)?32767:pred);
		obuf[i*2+0]=pred;
		psamp[i&7]=pred;
	}
	
	l=len>>2;

	/* decode side channel */
	pred=(s16)(ibuf[4]+(ibuf[5]<<8));
	index=ibuf[6]&127;
	pfcn=ibuf[7]&15;
	step=bgbmid_ima_step_table[index&127];
	psamp[0]=pred;	psamp[1]=pred;
	psamp[2]=pred;	psamp[3]=pred;
	psamp[4]=pred;	psamp[5]=pred;
	psamp[6]=pred;	psamp[7]=pred;
	
	for(i=0; i<l; i++)
	{
		k=ibuf[8+i*2+0]|((ibuf[8+i*2+1])<<8);
		uni=(k>>12)&15;

		if(pfcn)
			{ pred=BGBDT_SndBTAC1C2_PredictSample(psamp, i, pfcn); }

		index=index+bgbmid_ima_index_table[uni];
		index=(index<0)?0:((index>88)?88:index);
		diff=((2*(uni&7)+1)*step)/8;
		if(uni&8)diff=-diff;
		pred=pred+diff;
		step=bgbmid_ima_step_table[index];

		pred=(pred<(-32768))?(-32768):((pred>32767)?32767:pred);
		obuf[i*8+1]=pred;
		psamp[i&7]=pred;
	}

	/* center/side -> stereo */
	for(i=0; i<l; i++)
	{
		ofs=obuf[i*8+1];
//		ofs=0;
		for(j=0; j<4; j++)
		{
			k=(i*4+j)*2;
			pred=obuf[k+0];
//			ofs=obuf[k+1];
//			rp=(pred<<1)-ofs; lp=rp+ofs;
			lp=pred+ofs; rp=lp-(ofs<<1);
			lp=(lp<(-32768))?(-32768):((lp>32767)?32767:lp);
			rp=(rp<(-32768))?(-32768):((rp>32767)?32767:rp);
			obuf[k+0]=lp; obuf[k+1]=rp;
		}
	}
}

void BGBDT_SndBTAC1C2_DecodeStereoBlockStereoI(
	byte *ibuf, s16 *obuf, int len)
{
	int plsamp[8], prsamp[8];
	int lpred, lindex, lstep, ldiff, luni, lsni;
	int rpred, rindex, rstep, rdiff, runi, rsni;
	int pfcn;
	int i, j, k;
	
	lpred=(s16)(ibuf[0]+(ibuf[1]<<8));
	rpred=(s16)(ibuf[4]+(ibuf[5]<<8));
	lindex=ibuf[2];
	rindex=ibuf[6];
	pfcn=ibuf[3]&15;

	lstep=bgbmid_ima_step_table[lindex&127];
	rstep=bgbmid_ima_step_table[rindex&127];

	plsamp[0]=lpred;	plsamp[1]=lpred;
	plsamp[2]=lpred;	plsamp[3]=lpred;
	plsamp[4]=lpred;	plsamp[5]=lpred;
	plsamp[6]=lpred;	plsamp[7]=lpred;
	prsamp[0]=rpred;	prsamp[1]=rpred;
	prsamp[2]=rpred;	prsamp[3]=rpred;
	prsamp[4]=rpred;	prsamp[5]=rpred;
	prsamp[6]=rpred;	prsamp[7]=rpred;

	for(i=0; i<len; i++)
	{
		k=((i>>3)*8)+((i&7)>>1);
		j=(ibuf[ 8+k]>>((i&1)*4))&15;
		luni=j;
//		lsni=(j&8)?(-(j&7)):(j&7);

		j=(ibuf[12+k]>>((i&1)*4))&15;
		runi=j;
//		rsni=(j&8)?(-(j&7)):(j&7);

		if(pfcn)
		{
			lpred=BGBDT_SndBTAC1C2_PredictSample(plsamp, i, pfcn);
			rpred=BGBDT_SndBTAC1C2_PredictSample(prsamp, i, pfcn);
		}

		lindex=lindex+bgbmid_ima_index_table[luni];
		lindex=(lindex<0)?0:((lindex>88)?88:lindex);
		ldiff=((2*(luni&7)+1)*lstep)/8;
		if(luni&8)ldiff=-ldiff;
		lpred=lpred+ldiff;
		lstep=bgbmid_ima_step_table[lindex];

		rindex=rindex+bgbmid_ima_index_table[runi];
		rindex=(rindex<0)?0:((rindex>88)?88:rindex);
		rdiff=((2*(runi&7)+1)*rstep)/8;
		if(runi&8)rdiff=-rdiff;
		rpred=rpred+rdiff;
		rstep=bgbmid_ima_step_table[rindex];

		lpred=(lpred<(-32768))?(-32768):((lpred>32767)?32767:lpred);
		rpred=(rpred<(-32768))?(-32768):((rpred>32767)?32767:rpred);
		
		obuf[i*2+0]=lpred;
		obuf[i*2+1]=rpred;
		plsamp[i&7]=lpred;
		prsamp[i&7]=rpred;
	}
}

void BGBDT_SndBTAC1C2_DecodeStereoBlockStereo(
	byte *ibuf, s16 *obuf, int len)
{
	int i, j, k, l;

	BGBDT_SndBTAC1C2_DecodeStereoBlockStereoI(ibuf, obuf, len>>1);
	
	l=len>>1;
	for(i=len-1; i>0; i--)
	{
		j=(i+0)>>1;
//		k=(i-1)>>1;
//		k=(i+1)>>1;
//		if(k>=l)k=l-1;

		obuf[i*2+0]=obuf[j*2+0];
		obuf[i*2+1]=obuf[j*2+1];
//		obuf[i*2+0]=(obuf[j*2+0]+obuf[k*2+0])>>1;
//		obuf[i*2+1]=(obuf[j*2+1]+obuf[k*2+1])>>1;
	}
}
