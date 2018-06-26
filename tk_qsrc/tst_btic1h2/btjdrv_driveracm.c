#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <mmddk.h>
#include <mmreg.h>
#include <msacm.h>
#include <msacmdrv.h>

#ifndef WAVE_FORMAT_BTAC1C
#define WAVE_FORMAT_BTAC1C 0x7B1C
#endif

typedef LRESULT (*STREAMCONVERTPROC)(
	LPACMDRVSTREAMINSTANCE  padsi,
	LPACMDRVSTREAMHEADER    padsh);

typedef struct tSTREAMINSTANCE
{
	STREAMCONVERTPROC   fnConvert;  // stream instance conversion proc
	DWORD               fdwConfig;  // stream instance configuration flags
	
	int enc_idx;
} STREAMINSTANCE, *PSTREAMINSTANCE, FAR *LPSTREAMINSTANCE;

typedef BTIC1H_VidCodecCTX *PDRIVERINSTANCE;

const UINT gauFormatTagIndexToTag[] =
{
	WAVE_FORMAT_PCM,
	WAVE_FORMAT_BTAC1C
};

#define SIZEOF_ARRAY(arr)	(sizeof(arr)/sizeof(arr[0]))

#define ACM_DRIVER_MAX_FORMAT_TAGS  SIZEOF_ARRAY(gauFormatTagIndexToTag)
 
const UINT gauFormatIndexToSampleRate[]={
	8000,	11025,	16000,	22050,
	32000,	44100,	48000 };
 
#define ACM_DRIVER_MAX_SAMPLE_RATES SIZEOF_ARRAY(gauFormatIndexToSampleRate)

#define MSFILTER_MAX_CHANNELS 2

#define ACM_DRIVER_MAX_CHANNELS	 (MSFILTER_MAX_CHANNELS)
 
const UINT gauFormatIndexToBitsPerSample[] =
	{ 8, 16 };
 
#define ACM_DRIVER_MAX_BITSPERSAMPLE_PCM	\
	SIZEOF_ARRAY(gauFormatIndexToBitsPerSample)

#define ACM_DRIVER_MAX_STANDARD_FORMATS_PCM (\
	ACM_DRIVER_MAX_SAMPLE_RATES *  \
	ACM_DRIVER_MAX_CHANNELS *	  \
	ACM_DRIVER_MAX_BITSPERSAMPLE_PCM)
 
#define VERSION_ACM_DRIVER  MAKE_ACM_VERSION(4,  0, 0)
#define VERSION_MSACM       MAKE_ACM_VERSION(3, 50, 0)

LRESULT btacm_DriverDetails(
	PDRIVERINSTANCE		pdi,
	LPACMDRIVERDETAILS	padd)
{
	ACMDRIVERDETAILS	add;
	DWORD				cbStruct;

	cbStruct			= min(padd->cbStruct, sizeof(ACMDRIVERDETAILS));
	add.cbStruct		= cbStruct;
 
	add.fccType			= ACMDRIVERDETAILS_FCCTYPE_AUDIOCODEC;
	add.fccComp			= ACMDRIVERDETAILS_FCCCOMP_UNDEFINED;

	add.wMid			= 0;
	add.wPid			= 0;
  
	add.vdwACM			= VERSION_MSACM;
	add.vdwDriver		= VERSION_ACM_DRIVER;
 
	add.fdwSupport		= ACMDRIVERDETAILS_SUPPORTF_CODEC;
 
 	add.cFormatTags	 = ACM_DRIVER_MAX_FORMAT_TAGS;
 
	add.cFilterTags	 = 0;
 
 	if (FIELD_OFFSET(ACMDRIVERDETAILS, hicon) < cbStruct)
	{
		add.hicon = NULL;
 
		wcscpy(add.szShortName, L"BTAC1C"); 
		wcscpy(add.szLongName, L"BGBTech BTAC1C");

		if (FIELD_OFFSET(ACMDRIVERDETAILS, szCopyright) < cbStruct)
		{
			wcscpy(add.szCopyright, L"2016 Brendan G Bohannon"); 
			wcscpy(add.szLicensing, L"MIT");
			wcscpy(add.szFeatures, L"?");
		}
	}
 
	memcpy(padd, &add, (UINT)add.cbStruct);
 
	return (MMSYSERR_NOERROR);
} 
	
LRESULT btacm_FormatTagDetails(
	PDRIVERINSTANCE		 pdi,
	LPACMFORMATTAGDETAILS	padft,
	DWORD					fdwDetails)
{
	UINT				uFormatTag;
 
	UNREFERENCED_PARAMETER(pdi);
 
	switch(ACM_FORMATTAGDETAILSF_QUERYMASK & fdwDetails)
	{
		case ACM_FORMATTAGDETAILSF_INDEX:
			if (padft->dwFormatTagIndex>=ACM_DRIVER_MAX_FORMAT_TAGS)
				return (ACMERR_NOTPOSSIBLE);
 
			uFormatTag=gauFormatTagIndexToTag[
				(UINT)padft->dwFormatTagIndex];
//			*(int *)-1=-1;
//			__asm { int 3 }
			break;
		case ACM_FORMATTAGDETAILSF_LARGESTSIZE:
			switch(padft->dwFormatTag)
			{
				case WAVE_FORMAT_UNKNOWN:
				case WAVE_FORMAT_PCM:
				case WAVE_FORMAT_BTAC1C:
					uFormatTag = WAVE_FORMAT_PCM;
					break;
				default:
					return (ACMERR_NOTPOSSIBLE);
			}
			break;
		case ACM_FORMATTAGDETAILSF_FORMATTAG:
			if((padft->dwFormatTag!=WAVE_FORMAT_PCM) &&
					(padft->dwFormatTag!=WAVE_FORMAT_BTAC1C))
				return (ACMERR_NOTPOSSIBLE);
			uFormatTag=padft->dwFormatTag;
			break;
		default:
			return (MMSYSERR_NOTSUPPORTED);
	}

	switch (uFormatTag)
	{
		case WAVE_FORMAT_PCM:
			padft->dwFormatTagIndex = 0;
			padft->dwFormatTag		= WAVE_FORMAT_PCM;
			padft->cbFormatSize	 = sizeof(PCMWAVEFORMAT);
			padft->fdwSupport		= ACMDRIVERDETAILS_SUPPORTF_CODEC;
			padft->cStandardFormats = ACM_DRIVER_MAX_STANDARD_FORMATS_PCM;
			if(FIELD_OFFSET(ACMFORMATTAGDETAILS, szFormatTag) <
					padft->cbStruct)
				wcscpy(padft->szFormatTag, L"PCM");
// 			padft->szFormatTag[0]	=	'\0';
			break;
		case WAVE_FORMAT_BTAC1C:
			padft->dwFormatTagIndex = 1;
			padft->dwFormatTag		= WAVE_FORMAT_BTAC1C;
			padft->cbFormatSize	 = sizeof(PCMWAVEFORMAT);
			padft->fdwSupport		= ACMDRIVERDETAILS_SUPPORTF_CODEC;
			padft->cStandardFormats = ACM_DRIVER_MAX_STANDARD_FORMATS_PCM/2;
			if(FIELD_OFFSET(ACMFORMATTAGDETAILS, szFormatTag) <
					padft->cbStruct)
				wcscpy(padft->szFormatTag, L"BTAC1C");
// 			padft->szFormatTag[0]	=	'\0';
			break;
		default:
			return (ACMERR_NOTPOSSIBLE);
	}

	padft->cbStruct = min(padft->cbStruct, sizeof(*padft));
	return (MMSYSERR_NOERROR);
}
 
LRESULT btacm_FormatDetails(
	PDRIVERINSTANCE		pdi,
	LPACMFORMATDETAILS	padf,
	DWORD				fdwDetails)
{
	LPWAVEFORMATEX		pwfx;
	UINT				uFormatIndex;
	UINT				u;
 
	UNREFERENCED_PARAMETER(pdi);
 
	switch (ACM_FORMATDETAILSF_QUERYMASK & fdwDetails)
	{
	case ACM_FORMATDETAILSF_INDEX:
		if((padf->dwFormatTag!=WAVE_FORMAT_PCM) &&
			(padf->dwFormatTag!=WAVE_FORMAT_BTAC1C))
				return (ACMERR_NOTPOSSIBLE);
		if(padf->dwFormatIndex>=ACM_DRIVER_MAX_STANDARD_FORMATS_PCM)
			return(ACMERR_NOTPOSSIBLE);

		if(padf->dwFormatTag==WAVE_FORMAT_BTAC1C)
		{
			pwfx = padf->pwfx;
			uFormatIndex = (UINT)padf->dwFormatIndex;
 
			pwfx->wFormatTag = WAVE_FORMAT_BTAC1C;
			
			u = uFormatIndex/ACM_DRIVER_MAX_CHANNELS;
			pwfx->nSamplesPerSec=gauFormatIndexToSampleRate[u];
 
			u = uFormatIndex % ACM_DRIVER_MAX_CHANNELS;
			pwfx->nChannels		= (WORD)u + 1;
			u = (uFormatIndex / ACM_DRIVER_MAX_CHANNELS) % 
				ACM_DRIVER_MAX_CHANNELS;
			pwfx->wBitsPerSample	= 4;
 
			pwfx->nBlockAlign=(pwfx->nSamplesPerSec>11025)?512:256;
			pwfx->nAvgBytesPerSec=(pwfx->nSamplesPerSec*4)/8;
 
			break;
		}

		pwfx = padf->pwfx;
		uFormatIndex = (UINT)padf->dwFormatIndex;

//			pwfx->wFormatTag = WAVE_FORMAT_PCM;
		pwfx->wFormatTag = padf->dwFormatTag;

		u = uFormatIndex /
			(ACM_DRIVER_MAX_BITSPERSAMPLE_PCM *
				ACM_DRIVER_MAX_CHANNELS);
		pwfx->nSamplesPerSec	= gauFormatIndexToSampleRate[u];

		u = uFormatIndex % ACM_DRIVER_MAX_CHANNELS;
		pwfx->nChannels		= (WORD)u + 1;

		u = (uFormatIndex / ACM_DRIVER_MAX_CHANNELS) % 
			ACM_DRIVER_MAX_CHANNELS;
		pwfx->wBitsPerSample =
			(WORD)gauFormatIndexToBitsPerSample[u];

		pwfx->nBlockAlign=
			(pwfx->nChannels*pwfx->wBitsPerSample)/8;
		pwfx->nAvgBytesPerSec =
			pwfx->nSamplesPerSec*pwfx->nBlockAlign;
		break;

	 case ACM_FORMATDETAILSF_FORMAT:
		if(	!pcmIsValidFormat(padf->pwfx) &&
			!btacIsValidFormat(padf->pwfx))
				return (ACMERR_NOTPOSSIBLE);
		break;

	default:
		return(MMSYSERR_NOTSUPPORTED);
	}
 
	padf->cbStruct	= min(padf->cbStruct, sizeof(*padf));
	padf->fdwSupport	= ACMDRIVERDETAILS_SUPPORTF_CODEC;
	padf->szFormat[0] = '\0';

	return(MMSYSERR_NOERROR);
}

LRESULT btacm_FormatSuggest(
	PDRIVERINSTANCE			pdi,
	LPACMDRVFORMATSUGGEST 	padfs,
	DWORD					fdwDetails)
{
	LPWAVEFORMATEX		pwfxSrc;
	LPWAVEFORMATEX		pwfxDst;
	DWORD fdwSuggest;
 
//	__asm { int 3 }

	pwfxSrc		= padfs->pwfxSrc;
	pwfxDst		= padfs->pwfxDst;
	fdwSuggest	= padfs->fdwSuggest;
 
	if(pwfxSrc->wFormatTag==WAVE_FORMAT_BTAC1C)
	{
		if((fdwSuggest&ACM_FORMATSUGGESTF_NCHANNELS) &&
			(pwfxDst->nChannels!=pwfxSrc->nChannels))
				return(MMSYSERR_INVALFLAG);
		if((fdwSuggest&ACM_FORMATSUGGESTF_NSAMPLESPERSEC) &&
			(pwfxDst->nSamplesPerSec!=pwfxSrc->nSamplesPerSec))
				return(MMSYSERR_INVALFLAG);
		if((fdwSuggest&ACM_FORMATSUGGESTF_WFORMATTAG) &&
			(pwfxDst->wFormatTag!=WAVE_FORMAT_PCM))
				return(MMSYSERR_INVALFLAG);

		if((fdwSuggest&ACM_FORMATSUGGESTF_WBITSPERSAMPLE) &&
			(pwfxDst->wBitsPerSample!=16) &&
			(pwfxDst->wBitsPerSample!=8))
				return(MMSYSERR_INVALFLAG);
	
		pwfxDst->wFormatTag=WAVE_FORMAT_PCM;
		pwfxDst->nSamplesPerSec=pwfxSrc->nSamplesPerSec;
		pwfxDst->nChannels		= pwfxSrc->nChannels;
		if(!(fdwSuggest&ACM_FORMATSUGGESTF_WBITSPERSAMPLE))
			pwfxDst->wBitsPerSample	= 16;
		pwfxDst->nBlockAlign=
			(pwfxDst->nChannels*pwfxDst->wBitsPerSample)/8;
		pwfxDst->nAvgBytesPerSec =
			pwfxDst->nSamplesPerSec*pwfxDst->nBlockAlign;
		pwfxDst->cbSize	 = sizeof(PCMWAVEFORMAT);

		return(MMSYSERR_NOERROR);
	}
 
	return(MMSYSERR_NOTSUPPORTED);
}

LPVOID msfilterGlobalAllocPtr(_In_ UINT flags, _In_ SIZE_T cb)
{
	HGLOBAL handle = GlobalAlloc((flags), (cb));
	return ((handle == NULL) ? NULL : GlobalLock(handle));
}
 
BOOL pcmIsValidFormat(LPWAVEFORMATEX pwfx)
{
    if (!pwfx)
        return(FALSE);
    if (pwfx->wFormatTag!=WAVE_FORMAT_PCM)
        return(FALSE);
    if((pwfx->nChannels<1) || (pwfx->nChannels>ACM_DRIVER_MAX_CHANNELS))
        return(FALSE);
    if((pwfx->wBitsPerSample!=8) && (pwfx->wBitsPerSample!=16))
        return(FALSE);

//   if ((0L == pwfx->nSamplesPerSec) ||
//			(0x3FFFFFFF < pwfx->nSamplesPerSec))
	if((pwfx->nSamplesPerSec<8000) ||
		(pwfx->nSamplesPerSec>48000))
    {
        return(FALSE);
    }
 
//    if (PCM_BLOCKALIGNMENT(pwfx) != pwfx->nBlockAlign)
//        return (FALSE);
  
//    if (PCM_AVGBYTESPERSEC(pwfx) != pwfx->nAvgBytesPerSec)
//        return (FALSE);
 
    return (TRUE);
}

BOOL btacIsValidFormat(LPWAVEFORMATEX pwfx)
{
    if (!pwfx)
        return(FALSE);
    if (pwfx->wFormatTag!=WAVE_FORMAT_BTAC1C)
        return(FALSE);
    if((pwfx->nChannels<1) ||
			(pwfx->nChannels>ACM_DRIVER_MAX_CHANNELS))
        return(FALSE);
    if(pwfx->wBitsPerSample!=4)
        return(FALSE);
	if((pwfx->nSamplesPerSec<8000) ||
			(pwfx->nSamplesPerSec>48000))
        return(FALSE);
 
	if((pwfx->nBlockAlign<64) || (pwfx->nBlockAlign>4096))
		return(FALSE);
	if(pwfx->nBlockAlign&(pwfx->nBlockAlign-1))
		return(FALSE); 
		
	if(pwfx->nAvgBytesPerSec!=(pwfx->nSamplesPerSec/2))
	{
		if(pwfx->nAvgBytesPerSec!=pwfx->nSamplesPerSec)
			return(FALSE); 
		if(pwfx->nChannels!=2)
			return(FALSE); 
	}

    return (TRUE);
}
 
LRESULT btacm_StreamConvert_BTAC2PCM(
	LPACMDRVSTREAMINSTANCE	padsi,
	LPACMDRVSTREAMHEADER	padsh)
{
    SHORT *hpiSrc;
    SHORT *hpiDst;
    BYTE *hpbSrc;
    BYTE *hpbDst;
	int len, len2, lg2, lg2b, nb, nbr, bs, ss, isfr;
	int i, j, k;
	
	lg2=0; bs=padsi->pwfxSrc->nBlockAlign;
	switch(bs)
	{
		case   16: lg2= 5; lg2b=4; break;
		case   32: lg2= 6; lg2b=5; break;
		case   64: lg2= 7; lg2b=6; break;
		case  128: lg2= 8; lg2b=7; break;
		case  256: lg2= 9; lg2b=8; break;
		case  512: lg2=10; lg2b=9; break;
		case 1024: lg2=11; lg2b=10; break;
		case 2048: lg2=12; lg2b=11; break;
		case 4096: lg2=13; lg2b=12; break;
		default:
			return(MMSYSERR_INVALPARAM);
	}

	if(lg2<=0)
		return(MMSYSERR_INVALPARAM);

//	nb=padsh->cbSrcLength>>(lg2-1);
//	nbr=padsh->cbSrcLength-(nb<<(lg2-1));

	nb=padsh->cbSrcLength>>lg2b;
	nbr=padsh->cbSrcLength-(nb<<lg2b);

	isfr=0;
	if((padsi->pwfxDst->nChannels==2) &&
		(padsi->pwfxSrc->nAvgBytesPerSec>
			(padsi->pwfxSrc->nSamplesPerSec/2)))
	{
		isfr=1;
		lg2--;
	}
	
//	if(nbr)nb++;
	
	len=nb<<lg2;

	ss=(padsi->pwfxDst->wBitsPerSample*padsi->pwfxDst->nChannels)/8;
	len2=padsh->cbDstLength/ss;
	if(len2<len)
	{
		nb=len2>>lg2;
		len=nb<<lg2;
	}

	if(padsi->pwfxDst->wBitsPerSample==8)
	{
		hpbSrc = (BYTE*)padsh->pbSrc;
		hpbDst = (BYTE*)padsh->pbDst;

		if(padsi->pwfxDst->nChannels==1)
		{
			for(i=0; i<nb; i++)
			{
				BGBDT_SndBTAC1C_DecodeBlockMonoLg2_8b(
					hpbSrc, hpbDst, lg2);
				hpbSrc+=bs;
				hpbDst+=1<<lg2;
			}
		}else if(padsi->pwfxDst->nChannels==2)
		{
			for(i=0; i<nb; i++)
			{
				if(isfr)
				{
					BGBDT_SndBTAC1C_DecodeBlockStereoFrLg2_8b(
						hpbSrc, hpbDst, lg2);
				}else
				{
					BGBDT_SndBTAC1C_DecodeBlockStereoLg2_8b(
						hpbSrc, hpbDst, lg2);
				}
				hpbSrc+=bs;
				hpbDst+=2<<lg2;
			}
		}
	}else if(padsi->pwfxDst->wBitsPerSample==16)
	{
		hpbSrc = (BYTE*)padsh->pbSrc;
//		hpbDst = (BYTE*)padsh->pbDst;
		hpiDst = (SHORT*)(padsh->pbDst);
		
		if(padsi->pwfxDst->nChannels==1)
		{
			for(i=0; i<nb; i++)
			{
				BGBDT_SndBTAC1C_DecodeBlockMonoLg2(hpbSrc, hpiDst, lg2);
				hpbSrc+=bs;
				hpiDst+=1<<lg2;
			}
		}else if(padsi->pwfxDst->nChannels==2)
		{
			for(i=0; i<nb; i++)
			{
				if(isfr)
				{
					BGBDT_SndBTAC1C_DecodeBlockStereoFrLg2(
						hpbSrc, hpiDst, lg2);
				}else
				{
					BGBDT_SndBTAC1C_DecodeBlockStereoLg2(
						hpbSrc, hpiDst, lg2);
				}
				hpbSrc+=bs;
				hpiDst+=2<<lg2;
			}
		}
	}

    padsh->cbSrcLengthUsed = nb*bs;
    padsh->cbDstLengthUsed = ss*len;
	return(MMSYSERR_NOERROR);
}


LRESULT btacm_StreamConvert_PCM2BTAC(
	LPACMDRVSTREAMINSTANCE	padsi,
	LPACMDRVSTREAMHEADER	padsh)
{
	PSTREAMINSTANCE	 psi;
    SHORT *hpiSrc;
    SHORT *hpiDst;
    BYTE *hpbSrc;
    BYTE *hpbDst;
	int len, len2, lg2, nb, nb2, bs, ss, ix;
	int i, j, k;
	
	psi=(PSTREAMINSTANCE)padsi->dwDriver;
	if(psi)ix=psi->enc_idx;
	
	lg2=0; bs=padsi->pwfxDst->nBlockAlign;
	switch(bs)
	{
		case   16: lg2= 5; break;
		case   32: lg2= 6; break;
		case   64: lg2= 7; break;
		case  128: lg2= 8; break;
		case  256: lg2= 9; break;
		case  512: lg2=10; break;
		case 1024: lg2=11; break;
		case 2048: lg2=12; break;
		case 4096: lg2=13; break;
		default:
			return(MMSYSERR_INVALPARAM);
	}

	ss=(padsi->pwfxSrc->wBitsPerSample*padsi->pwfxSrc->nChannels)/8;
	nb=(padsh->cbSrcLength/ss)>>lg2;
	len=nb<<lg2;

//	nb=padsh->cbSrcLength>>(lg2-1);
//	len=nb<<lg2;

	nb2=padsh->cbDstLength>>(lg2-1);
	if(nb2<nb)
	{
		nb=nb2;
		len=nb<<lg2;
	}

	if(padsi->pwfxSrc->wBitsPerSample==8)
	{
		hpbSrc = (BYTE*)padsh->pbSrc;
		hpbDst = (BYTE*)padsh->pbDst;

		if(padsi->pwfxSrc->nChannels==1)
		{
			for(i=0; i<nb; i++)
			{
				BGBDT_SndBTAC1C_EncodeBlockMonoLg2_8b(
					hpbSrc, hpbDst, lg2, &ix);
				hpbSrc+=1<<lg2;
				hpbDst+=bs;
			}
		}else if(padsi->pwfxSrc->nChannels==1)
		{
			for(i=0; i<nb; i++)
			{
				BGBDT_SndBTAC1C_EncodeBlockStereoLg2_8b(
					hpbSrc, hpbDst, lg2, &ix);
				hpbSrc+=2<<lg2;
				hpbDst+=bs;
			}
		}
	}else if(padsi->pwfxSrc->wBitsPerSample==16)
	{
		hpiSrc = (SHORT*)(padsh->pbSrc);
		hpbDst = (BYTE*)padsh->pbDst;
		
		if(padsi->pwfxSrc->nChannels==1)
		{
			for(i=0; i<nb; i++)
			{
				BGBDT_SndBTAC1C_EncodeBlockMonoLg2(
					hpiSrc, hpbDst, lg2, &ix);
				hpiSrc+=1<<lg2;
				hpbDst+=bs;
			}
		}else if(padsi->pwfxSrc->nChannels==2)
		{
			for(i=0; i<nb; i++)
			{
				BGBDT_SndBTAC1C_EncodeBlockStereoLg2(
					hpiSrc, hpbDst, lg2, &ix);
				hpiSrc+=2<<lg2;
				hpbDst+=bs;
			}
		}
	}

	ss=(padsi->pwfxSrc->wBitsPerSample*padsi->pwfxSrc->nChannels)/8;
    padsh->cbSrcLengthUsed = ss*len;
    padsh->cbDstLengthUsed = nb*bs;
	if(psi)psi->enc_idx=ix;

	return(MMSYSERR_NOERROR);
}

LRESULT btacm_StreamOpen
(	PDRIVERINSTANCE		 pdi,
	LPACMDRVSTREAMINSTANCE	padsi)
{
	PSTREAMINSTANCE	 psi;
	LPWAVEFORMATEX		pwfxSrc;
	LPWAVEFORMATEX		pwfxDst;
	LPWAVEFILTER		pwfltr;
 
	pwfxSrc = padsi->pwfxSrc;
	pwfxDst = padsi->pwfxDst;
	pwfltr	= padsi->pwfltr;
 
	if(	!(pcmIsValidFormat(pwfxSrc)&&btacIsValidFormat(pwfxDst)) &&
		!(btacIsValidFormat(pwfxSrc)&&pcmIsValidFormat(pwfxDst)))
			{ return(ACMERR_NOTPOSSIBLE); }
  
	if(pwfxSrc->nChannels!=pwfxDst->nChannels)
		{ return(ACMERR_NOTPOSSIBLE); }
	if(pwfxSrc->nSamplesPerSec!=pwfxDst->nSamplesPerSec)
		{ return(ACMERR_NOTPOSSIBLE); }
  
	if(padsi->fdwOpen&ACM_STREAMOPENF_QUERY)
		return(MMSYSERR_NOERROR);
 
	psi=(PSTREAMINSTANCE)LocalAlloc(LPTR, sizeof(*psi));
	if(!psi)
		return(MMSYSERR_NOMEM);
 
//	psi->fnConvert		= msfilterVolume;
//	psi->fdwConfig		= pdi->fdwConfig;
//	psi->hpbHistory	 = NULL;
//	psi->dwPlace		= 0L;
//	psi->dwHistoryDone	= 0L;

	if(btacIsValidFormat(pwfxSrc)&&pcmIsValidFormat(pwfxDst))
		{ psi->fnConvert = btacm_StreamConvert_BTAC2PCM; }
	if(pcmIsValidFormat(pwfxSrc)&&btacIsValidFormat(pwfxDst))
		{ psi->fnConvert = btacm_StreamConvert_PCM2BTAC; }

	padsi->fdwDriver=0L;
	padsi->dwDriver=(DWORD_PTR)psi;
 
	return (MMSYSERR_NOERROR);
}
 
LRESULT btacm_StreamClose(LPACMDRVSTREAMINSTANCE	padsi)
{
	PSTREAMINSTANCE	 psi;
 
	psi=(PSTREAMINSTANCE)padsi->dwDriver;
	if(psi)
	{
		LocalFree((HLOCAL)psi);
	}
	return (MMSYSERR_NOERROR);
}
 
LRESULT btacm_StreamSize(
	LPACMDRVSTREAMINSTANCE	padsi,
	LPACMDRVSTREAMSIZE		padss)
{
	LPWAVEFORMATEX		pwfxSrc;
	LPWAVEFORMATEX		pwfxDst;
	LPWAVEFILTER		pwfltr;
	DWORD				dw;
	int ss;

	pwfxSrc = padsi->pwfxSrc;
	pwfxDst = padsi->pwfxDst;
	pwfltr	= padsi->pwfltr;

	switch(ACM_STREAMSIZEF_QUERYMASK & padss->fdwSize)
	{
	case ACM_STREAMSIZEF_SOURCE:
		dw = padss->cbSrcLength;

		if(pwfxSrc->wFormatTag==WAVE_FORMAT_PCM)
		{
			ss=(pwfxSrc->nChannels*pwfxSrc->wBitsPerSample)/8;
			dw=(dw/ss);
			dw=(dw*4)/8;
			dw=((dw+511)/512)*512;
//			dw=(dw*pwfxSrc->nChannels*pwfxSrc->wBitsPerSample)/8;
		}

		if(pwfxSrc->wFormatTag==WAVE_FORMAT_BTAC1C)
		{
			ss=(pwfxDst->nChannels*pwfxDst->wBitsPerSample)/8;
			dw=dw*2;
			dw=dw*ss;

//			dw=(dw*4)/8;
//			dw=((dw+511)/512)*512;
		}

		if (!dw)
			{ return(ACMERR_NOTPOSSIBLE); }
		padss->cbDstLength = dw;
		return(MMSYSERR_NOERROR);


	case ACM_STREAMSIZEF_DESTINATION:
		dw = padss->cbDstLength;

		if(pwfxDst->wFormatTag==WAVE_FORMAT_PCM)
		{
			ss=(pwfxDst->nChannels*pwfxDst->wBitsPerSample)/8;
			dw=(dw/ss);
			dw=(dw*4)/8;
			dw=((dw+511)/512)*512;
//			dw=(dw*pwfxSrc->nChannels*pwfxSrc->wBitsPerSample)/8;
		}

		if(pwfxDst->wFormatTag==WAVE_FORMAT_BTAC1C)
		{
			ss=(pwfxSrc->nChannels*pwfxSrc->wBitsPerSample)/8;
			dw=dw*2;
			dw=dw*ss;

//			dw=(dw*4)/8;
//			dw=((dw+511)/512)*512;
		}

		if (!dw)
			{ return(ACMERR_NOTPOSSIBLE); }
		padss->cbSrcLength = dw;
		return(MMSYSERR_NOERROR);
	}
 
	return(MMSYSERR_NOTSUPPORTED);
}

LRESULT btacm_StreamConvert(
	LPACMDRVSTREAMINSTANCE	padsi,
	LPACMDRVSTREAMHEADER	padsh)
{
	PSTREAMINSTANCE		 psi;
	int lr;

	psi=(PSTREAMINSTANCE)padsi->dwDriver;

	if(psi->fnConvert)
	{
		lr=psi->fnConvert(padsi, padsh);
		return(lr);
	}
	return(MMSYSERR_NOTSUPPORTED);
}

LRESULT btacm_StreamPrepare(
	LPACMDRVSTREAMINSTANCE	padsi,
	LPACMDRVSTREAMHEADER	padsh)
{
	return(MMSYSERR_NOTSUPPORTED);
}

LRESULT btacm_StreamUnprepare(
	LPACMDRVSTREAMINSTANCE	padsi,
	LPACMDRVSTREAMHEADER	padsh)
{
	return(MMSYSERR_NOTSUPPORTED);
}

LRESULT btacm_StreamReset(
	LPACMDRVSTREAMINSTANCE	padsi,
	LPACMDRVSTREAMHEADER	padsh)
{
	return(MMSYSERR_NOERROR);
//	return(MMSYSERR_NOTSUPPORTED);
}
