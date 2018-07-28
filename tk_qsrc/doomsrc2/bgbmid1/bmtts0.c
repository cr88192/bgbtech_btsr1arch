#include <bgbmid.h>

double bmtst_firtab[64][1024];

int main(int argc, char *argv[])
{
	static char *firlst[]=
	{
		"voice2/vo_a_0.wav",	//0
		"voice2/vo_i_0.wav",	//1
		"voice2/vo_u_0.wav",	//2
		"voice2/vo_e_0.wav",	//3
		"voice2/vo_o_0.wav",	//4
		"voice2/vw_l_0.wav", 	//5
		"voice2/vw_m_0.wav", 	//6
		"voice2/vw_n_0.wav", 	//7
		"voice2/vw_r_0.wav", 	//8
		"voice2/vo_ax_0.wav",	//9
		"voice2/vo_ox_0.wav",	//10
		"voice2/vo_ux_0.wav",	//11
		"voice2/vo_ix_0.wav",	//12
		"voice2/vk_tt_0.wav",	//13
		"voice2/vk_zz_0.wav",	//14
		NULL
	};

	static char idxlst[]={
	 6,  0,  6,  0, 6, 1,  0,
	12, 13, 14, 11, 6, 1, 11,
	6, 0, 8, 1, 4,
	
	 6,  0,  6,  0, 6, 1,  0,
	12, 13, 14, 11, 6, 1, 11,
	6, 0, 8, 1, 4,

	 6,  0,  6,  0, 6, 1,  0,
	12, 13, 14, 11, 6, 1, 11,
	6, 0, 8, 1, 4,
	};

	static float formlst[]={
	//B	  F1	F2	  F3   V0   V1   V2   V3  VN
	100, 730, 1090, 2440, 255, 255, 128,  64,   0,	//0
	100, 270, 2290, 3010, 255, 255, 128,  64,   0,	//1
	100, 300,  870, 2240, 255, 255, 128,  64,   0,	//2
	100, 530, 1840, 2480, 255, 255, 128,  64,   0,	//3
	100, 570,  840, 2410, 255, 255, 128,  64,   0,	//4
	100, 429, 3468, 4239, 128, 255, 128,  64,   0,	//5
	100, 235, 1282, 2483, 128, 255, 128,  64,   0,	//6
	100, 235, 1282, 2461, 128, 128, 128,  64,   0,	//7
	100, 490, 1350, 1690, 128, 255, 128,  64,   0,	//8
	100, 660, 1720, 2410, 255, 255, 128,  64,   0,	//9
	100, 570,  840, 2410, 255, 255, 128,  64,   0,	//10
	100, 520, 1190, 2390, 255, 255, 128,  64,   0,	//11
	110, 390, 1990, 2550, 255, 255, 128,  64,   0,	//12
	110, 390, 1990, 2550,  64,   0,   0,   0,  64,	//13
	110, 390, 1990, 2550, 128,   0,   0,   0,  64,	//14
	};

//	double fir[1024];
	s16 *bp, *ibp1, *obp1, *obp2, *ibpn;
	s16 *ibp_ss, *ibp_tt;
	char *s;
	int len, ilen1, ilen_ss, ilen_tt;
	int i, j, k, l, szf, k0, k1;

	szf=256;

	for(i=0; firlst[i]; i++)
	{
		s=firlst[i];
		bp=BGBMID_LoadWAV_44Mono16(s, &len);
		BGBMID_LpcFIR_BuildSampleLPC(bp, len, bmtst_firtab[i], szf);
	}

	s="voice2/vo_ux_1b.wav";
	ibp1=BGBMID_LoadWAV_44Mono16(s, &ilen1);

	s="voice2/vk_sss_0.wav";
	ibp_ss=BGBMID_LoadWAV_44Mono16(s, &ilen_ss);

	s="voice2/vk_ttt_0.wav";
	ibp_tt=BGBMID_LoadWAV_44Mono16(s, &ilen_tt);

#if 0
	s="voice2/vo_u_0.wav";
//	s="voice2/vw_l_0.wav";
	bp=BGBMID_LoadWAV_44Mono16(s, &len);

	s="voice2/vo_ux_1a.wav";
	ibp1=BGBMID_LoadWAV_44Mono16(s, &ilen1);
	
	szf=1024;
	BGBMID_LpcFIR_BuildSampleLPC(bp, len, fir, szf);
	
	for(i=0; i<(szf/8); i++)
	{
		for(j=0; j<8; j++)
		{
			printf("%+1.4f ", fir[i*8+j]);
		}
		printf("\n");
	}
#endif
	
	l=1<<18;
	obp1=malloc(l*sizeof(s16));
	obp2=malloc(l*sizeof(s16));
	
	for(i=0; i<l; i++)
	{
//		obp1[i]=4096 * sin(i*2*M_PI / 512);
//		obp1[i]=((i>>8)&1)?4096:-4096;
//		obp1[i]=((i>>7)&1)?4096:-4096;
//		obp1[i]=(!(i&255))?4096:0;
//		obp1[i]=(i%256)*16;
//		obp1[i]=((511-i%512)-256)*4;
//		obp1[i]=((1023-i%1024)-512)*2;
//		obp1[i]=ibp1[i%ilen1];
//		obp1[i]=1024 * sin(i*2*M_PI / 1024);

//		obp1[i]=4096 * sin(i*M_PI * (100.0/44100.0));
//		obp1[i]=4096 * bgbmid_lpcfir_sin_tri(i*M_PI * (100.0/44100.0));
//		obp1[i]=1024 * bgbmid_lpcfir_sin_saw(i*M_PI * (100.0/44100.0));

//		obp1[i]=2048 * bgbmid_lpcfir_sin_tri(i*M_PI * (132.0/44100.0));
//		obp1[i]+=1024 * bgbmid_lpcfir_sin_saw(i*M_PI * (132.0/44100.0));

		obp1[i]=ibp1[i%ilen1]*0.05;
//		obp1[i]+=2048 * bgbmid_lpcfir_sin_tri(i*M_PI * (176.0/44100.0));
//		obp1[i]+=1024 * bgbmid_lpcfir_sin_saw(i*M_PI * (176.0/44100.0));

//		obp1[i]=2048 * bgbmid_lpcfir_sin_tri(i*M_PI * (176.0/44100.0));
//		obp1[i]+=1024 * bgbmid_lpcfir_sin_saw(i*M_PI * (176.0/44100.0));

//		j=((511-i%512)-256)*4;
//		k=((1023-i%1024)-512)*2;
//		obp1[i]=(j+k)/2;
	}

#if 0
	k=0;
	for(i=0; i<l; i++)
	{
		k=0.90*k+0.10*obp1[i];
		obp1[i]=k;
	}
#endif

//	BGBMID_LpcFIR_BandPass2(obp1, obp2, l, 44100, 440, 100, 0.999);

#if 1
	j=6144;
//	j=8192;
//	j=12288;
	for(i=0; i<(l/j); i++)
	{
		k=idxlst[i+0];
		k1=idxlst[i+1];
		
//		BGBMID_LpcFIR_AddFormant(obp1+i*j, obp1+i*j, j,
//			formlst[k*9+1], formlst[k*9+5]/255.0);
//		BGBMID_LpcFIR_AddFormant(obp1+i*j, obp1+i*j, j,
//			formlst[k*9+2], formlst[k*9+6]/255.0);
//		BGBMID_LpcFIR_AddFormant(obp1+i*j, obp1+i*j, j,
//			formlst[k*9+3], formlst[k*9+7]/255.0);

//		BGBMID_LpcFIR_AddNoise(obp1+i*j, obp1+i*j, j, 8*formlst[k*9+8]);

//		BGBMID_LpcFIR_ScaleSamples(obp1+i*j, obp1+i*j, j,
//			formlst[k*9+4]/255.0);
		BGBMID_LpcFIR_ScaleSamplesLerp(obp1+i*j, obp1+i*j, j,
			formlst[k*9+4]/255.0, formlst[k1*9+4]/255.0);

#if 0
		BGBMID_LpcFIR_BandPassLerp2(obp1+i*j, obp1+i*j, j, 44100,
			formlst[k*9+1], formlst[(k1)*9+1], 100, 100, 0.995, 0.995);
		BGBMID_LpcFIR_BandPassLerp2(obp1+i*j, obp1+i*j, j, 44100,
			formlst[k*9+2], formlst[(k1)*9+2], 100, 100, 0.995, 0.995);
		BGBMID_LpcFIR_BandPassLerp2(obp1+i*j, obp1+i*j, j, 44100,
			formlst[k*9+3], formlst[(k1)*9+3], 100, 100, 0.995, 0.995);
#endif

#if 1
		BGBMID_LpcFIR_BandPass2(obp1+i*j, obp1+i*j, j, 44100,
			formlst[k*9+1], 100, 0.995);
		BGBMID_LpcFIR_BandPass2(obp1+i*j, obp1+i*j, j, 44100,
			formlst[k*9+2], 100, 0.995);
		BGBMID_LpcFIR_BandPass2(obp1+i*j, obp1+i*j, j, 44100,
			formlst[k*9+3], 100, 0.995);
#endif

#if 0
		BGBMID_LpcFIR_ScaleSamples(obp1+i*j, obp2+i*j, j, 0.0);
		BGBMID_LpcFIR_BandPassAddOut2(obp1+i*j, obp2+i*j, j, 44100,
			formlst[k*9+1], 100, 0.995);
		BGBMID_LpcFIR_BandPassAddOut2(obp1+i*j, obp2+i*j, j, 44100,
			formlst[k*9+2], 100, 0.995);
		BGBMID_LpcFIR_BandPassAddOut2(obp1+i*j, obp2+i*j, j, 44100,
			formlst[k*9+3], 100, 0.995);
		BGBMID_LpcFIR_AddScaleSamples(obp2+i*j, ibp_ss, obp2+i*j, j,
			formlst[k*9+8]/255);
#endif

//		BGBMID_LpcFIR_AddNoise(obp1+i*j, obp1+i*j, j,
//			16*formlst[k*9+8], 0.80, 0.95);
//		BGBMID_LpcFIR_AddNoiseBandPass2(
//			obp1+i*j, obp1+i*j, j, 44100,
//			16*formlst[k*9+8],
//			10000, 500, 0.97);

		ibpn=ibp_ss;
		if(k==13)
			ibpn=ibp_tt;
		
		BGBMID_LpcFIR_AddScaleSamples(obp1+i*j, ibpn, obp1+i*j, j,
			formlst[k*9+8]/255);

//		BGBMID_LpcFIR_AddScaleSamples(obp1+i*j, ibp_ss, obp1+i*j, j,
//			formlst[k*9+8]/255);
		
//		BGBMID_LpcFIR_ApplySampleLPC(
//			obp1+i*j, obp2+i*j, j, bmtst_firtab[k], szf);

//		BGBMID_LpcFIR_Normalize(obp2+i*j, obp2+i*j, j, 1024);

//		BGBMID_LpcFIR_ApplySampleLPC(
//			obp1+i*j, obp2+i*j, j, bmtst_firtab[i%9], szf);
	}
#endif

//	BGBMID_LpcFIR_ApplySampleLPC(obp1, obp2, l, fir, szf);
//	BGBMID_LpcFIR_ApplySampleLPC(obp1, obp1, l, fir, szf);
	
	s="bmtts_vctst0.wav";
	BGBMID_StoreWAV(s, obp1, 1, 44100, 16, l);
//	BGBMID_StoreWAV(s, obp2, 1, 44100, 16, l);
}
