/*

Attempt to implement basic codec support for IMA ADPCM.

*/

#include <bgbmid.h>

static int bgbmid_ima_index_table[16] = {
  -1, -1, -1, -1, 2, 4, 6, 8,
  -1, -1, -1, -1, 2, 4, 6, 8
}; 

static int bgbmid_ima_step_table[89] = { 
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

BMID_API void BGBMID_MsImaAdpcm_DecodeBlockMono(
	byte *ibuf, s16 *obuf, int len)
{
	int pred, index, step, diff, uni, sni;
	int i, j;
	
	pred=(s16)(ibuf[0]+(ibuf[1]<<8));
	index=ibuf[2];
	
	step=bgbmid_ima_step_table[index];

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

BMID_API void BGBMID_MsImaAdpcm_DecodeBlockMonoAsStereo(
	byte *ibuf, s16 *obuf, int len)
{
	int pred, index, step, diff, uni, sni;
	int i, j;
	
	pred=(s16)(ibuf[0]+(ibuf[1]<<8));
	index=ibuf[2];
	
	step=bgbmid_ima_step_table[index];

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
		
		obuf[i*2+0]=pred;
		obuf[i*2+1]=pred;
	}
}

BMID_API void BGBMID_MsImaAdpcm_EncodeBlockMono(
	s16 *ibuf, byte *obuf, int len)
{
	int pred, index, step, diff, uni, sni;
	int i, j, k;
	
	pred=ibuf[0];
	index=0;
	
	obuf[0]=pred&255;
	obuf[1]=(pred>>8)&255;
	obuf[2]=index;
	obuf[3]=0;
	
	memset(obuf+4, 0, (len+1)/2);
	
	step=bgbmid_ima_step_table[index];

	for(i=0; i<len; i++)
	{
		j=ibuf[i];
		k=j-pred;
		sni=(k*4)/step-0.5;
		sni=(sni<(-7))?(-7):((sni>7)?7:sni);
		j=(sni<0)?((-sni)|8):sni;
	
//		j=(ibuf[4+(i>>1)]>>((i&1)*4))&15;
		uni=j;
		sni=(j&8)?(-(j&7)):(j&7);
	
		index=index+bgbmid_ima_index_table[uni];
		diff=(sni+0.5)*step/4;
		pred=pred+diff;
		step=bgbmid_ima_step_table[index];
		
		obuf[4+(i>>1)]|=uni<<((i&1)*4);
//		obuf[i]=pred;
	}
}

BMID_API void BGBMID_MsImaAdpcm_DecodeBlockStereo(
	byte *ibuf, s16 *obuf, int len)
{
	int lpred, lindex, lstep, ldiff, luni, lsni;
	int rpred, rindex, rstep, rdiff, runi, rsni;
	int i, j, k;
	
	lpred=(s16)(ibuf[0]+(ibuf[1]<<8));
	lindex=ibuf[2];

	rpred=(s16)(ibuf[4]+(ibuf[5]<<8));
	rindex=ibuf[6];
	
	lstep=bgbmid_ima_step_table[lindex];
	rstep=bgbmid_ima_step_table[rindex];

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

BMID_API void BGBMID_MsImaAdpcm_EncodeBlockStereo(
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
	
	lstep=bgbmid_ima_step_table[lindex];
	rstep=bgbmid_ima_step_table[rindex];

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
		ldiff=(lsni+0.5)*lstep/4;
		lpred=lpred+ldiff;
		lstep=bgbmid_ima_step_table[lindex];

		rsni=(runi&8)?(-(runi&7)):(runi&7);
		rindex=rindex+bgbmid_ima_index_table[runi];
		rdiff=(rsni+0.5)*rstep/4;
		rpred=rpred+rdiff;
		rstep=bgbmid_ima_step_table[rindex];
		
		k=((i>>3)*4)+((i&7)>>1);
		obuf[ 8+k]|=luni<<((i&1)*4);
		obuf[12+k]|=runi<<((i&1)*4);
//		obuf[i]=pred;
	}
}

BMID_API int BGBMID_MsImaAdpcm_MonoSamplesFromBlockSize(int sz)
{
	return((sz-4)*2);
}

BMID_API int BGBMID_MsImaAdpcm_MonoBlockSizeFromSamples(int len)
{
	return(4+((len+1)/2));
}

BMID_API int BGBMID_MsImaAdpcm_StereoSamplesFromBlockSize(int sz)
{
	return(sz-8);
}

BMID_API int BGBMID_MsImaAdpcm_StereoBlockSizeFromSamples(int len)
{
	return(8+((len+7)/8)*8);
}
