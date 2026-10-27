/*
 * Automatic MUX for RP2 Post Compressors.
 *
 * Decoding:
 * If Post-Compressed, feed through the correct post compressor.
 * If not, do the raw bytes option.
 *
 * Encoding:
 * Tries various options with some tuning parameters.
 * Only does N-way comparison for smaller buffers.
 * For larger buffers, default to Huffman.
 * Or Range for Size2, as Range is smallest for large buffers, but slowest.
 */

#ifndef RP2POST_RP2AUTO_C
#define RP2POST_RP2AUTO_C

#define RP2POST_FLAGS_FAVOR_SPEED	0x01	/* Aim for fast decompression */
#define RP2POST_FLAGS_FAVOR_SPEED2	0x02	/* Aim for faster decompression */
#define RP2POST_FLAGS_FAVOR_SIZE	0x04	/* Aim for better ratio */
#define RP2POST_FLAGS_FAVOR_SIZE2	0x08	/* Aim for best ratio */

#define RP2POST_FLAGS_NO_ADRICE		0x10	/* Disable AdRice+STF */
#define RP2POST_FLAGS_NO_HUFF		0x20	/* Disable Huffman */
#define RP2POST_FLAGS_NO_RANGE		0x40	/* Disable Range Coder */
#define RP2POST_FLAGS_VERBOSE		0x80	/* Print Extra Messages */

#define RP2POST_FLAGS_USE_ADRICE	\
	(RP2POST_FLAGS_FAVOR_SIZE2|RP2POST_FLAGS_NO_HUFF|RP2POST_FLAGS_NO_RANGE)
#define RP2POST_FLAGS_USE_HUFF	\
	(RP2POST_FLAGS_FAVOR_SIZE2|RP2POST_FLAGS_NO_ADRICE|RP2POST_FLAGS_NO_RANGE)
#define RP2POST_FLAGS_USE_RANGE	\
	(RP2POST_FLAGS_FAVOR_SIZE2|RP2POST_FLAGS_NO_ADRICE|RP2POST_FLAGS_NO_HUFF)


#ifdef GFXEDIT_MINRP2_C
#ifndef HAS_RP2STF_DECODERP2
#define HAS_RP2STF_DECODERP2
#define PostRp2Stf_DecodeRP2(obuf, ibuf, osz, isz)		\
	GfxEdit_DecodeRP2(obuf, ibuf, osz, isz)
#endif

#ifndef HAS_RP2STF_ENCODERP2
#define HAS_RP2STF_ENCODERP2
#define PostRp2Stf_EncodeRP2(obuf, ibuf, osz, isz)		\
	GfxEdit_EncodeRP2(obuf, ibuf, osz, isz)
#define PostRp2Stf_EncodeRP2Test(obuf, ibuf, osz, isz)		\
	GfxEdit_EncodeRP2Test(obuf, ibuf, osz, isz)
#endif

#endif

#ifdef TGVLZ1_C
#ifndef HAS_RP2STF_DECODERP2
#define HAS_RP2STF_DECODERP2
#define PostRp2Stf_DecodeRP2(obuf, ibuf, osz, isz)		\
	TgvLz_DecodeBufferRP2C(ibuf, obuf, isz, osz)
#endif

#ifndef HAS_RP2STF_ENCODERP2
#define HAS_RP2STF_ENCODERP2
#define PostRp2Stf_EncodeRP2(obuf, ibuf, osz, isz)		\
	TgvLz_EncodeBufferRP2C_NoCtx(ibuf, obuf, isz, osz)
#define PostRp2Stf_EncodeRP2Test(obuf, ibuf, osz, isz)		\
	TgvLz_EncodeBufferRP2C_NoCtx(ibuf, obuf, isz, osz)
#endif

#endif


#ifndef HAS_RP2STF_DECODERP2
#error RP2 Post-Auto assumes unity build at least with an RP2 decompressor.
#endif

/*
 * Decode Post-Compressed RP2 with matching decoder.
 * Returns -1 on error (invalid input, missing compressor, ...).
 * Copies input to output if not post-compressed.
 */
int PostRp2_DecodeBufferPostRp2(
	byte *obuf, byte *ibuf, int obsz, int ibsz)
{
	if(((ibuf[0]&0x0F)==0x00) && (ibuf[1]==0x00))
	{
		if(ibuf[0]==0x00)
		{
			/* Illegal */
			return(-1);
		}

		if(ibuf[0]==0x20)
		{
#ifdef RP2POST_H12D_C
			return(PostRp2Huff_DecodeBufferPostRp2B(obuf, ibuf, obsz, ibsz));
#else
			/* Missing */
			return(-1);
#endif
		}

		if((ibuf[0]==0x10) || (ibuf[0]==0x50) || (ibuf[0]==0x40))
		{
#ifdef RP2POST_RP2STF_C
			return(PostRp2Stf_DecodeBufferPostRp2(obuf, ibuf, obsz, ibsz));
#else
			/* Missing */
			return(-1);
#endif
		}

		if(ibuf[0]==0x30)
		{
#ifdef RP2POST_RP2BITRC_C
			return(PostRp2BitRc_DecodeBufferPostRp2(obuf, ibuf, obsz, ibsz));
#else
			/* Missing */
			return(-1);
#endif
		}

		/* Invalid / Unknown */
		return(-1);
	}
	
	memmove(obuf, ibuf, ibsz);
	return(ibsz);
}

/*
 * Decompress Post-Compressed RP2 with matching decoder.
 * Returns -1 on error (invalid input, missing compressor, ...).
 * Uses a bare RP2 Decompressor.
 */
int PostRp2_DecodeBufferRp2Full(
	byte *obuf, byte *ibuf, int obsz, int ibsz)
{
	if(((ibuf[0]&0x0F)==0x00) && (ibuf[1]==0x00))
	{
		if(ibuf[0]==0x00)
		{
			/* Illegal */
			return(-1);
		}

		if(ibuf[0]==0x20)
		{
#ifdef RP2POST_H12D_C
			return(PostRp2Huff_DecodeBufferRp2Full(obuf, ibuf, obsz, ibsz));
#else
			/* Missing */
			return(-1);
#endif
		}

		if((ibuf[0]==0x10) || (ibuf[0]==0x50) || (ibuf[0]==0x40))
		{
#ifdef RP2POST_RP2STF_C
			return(PostRp2Stf_DecodeBufferRp2Full(obuf, ibuf, obsz, ibsz));
#else
			/* Missing */
			return(-1);
#endif
		}

		if(ibuf[0]==0x30)
		{
#ifdef RP2POST_RP2BITRC_C
			return(PostRp2BitRc_DecodeBufferRp2Full(obuf, ibuf, obsz, ibsz));
#else
			/* Missing */
			return(-1);
#endif
		}

		/* Invalid / Unknown */
		return(-1);
	}

#ifdef HAS_RP2STF_DECODERP2
	return(PostRp2Stf_DecodeRP2(obuf, ibuf, obsz, ibsz));
#endif
}

static byte *postrp2_tc1buf;
static byte *postrp2_tc2buf;
static byte *postrp2_tc3buf;

/* Try out post compressors over an RP2 compressed buffer.
 * If post compression is ineffective, copies input to output.
 */
int PostRp2_EncodeBufferPostRp2(
	byte *obuf, byte *ibuf, int obsz, int ibsz, int flags)
{
	double tune1, tune2, tune2h, tune2s, tune_hfs;
	int osz1, osz2, osz3, osz, obi;

	if(ibsz<640)
	{
		/* Don't bother... */
		memcpy(obuf, ibuf, ibsz);
		return(ibsz);
	}

	tune_hfs=sqrt(32768/ibsz)-1.0;
	tune1=1.10;
	tune2=1.06;
	tune2h=1.00+(tune_hfs*0.01);
	tune2s=1.00;

	if(flags&RP2POST_FLAGS_FAVOR_SPEED)
	{
		tune1=1.15;
		tune2=1.09;
		tune2h=1.00+(tune_hfs*0.02);
	}

	if(flags&RP2POST_FLAGS_FAVOR_SPEED2)
	{
		tune1=1.25;
		tune2=1.75;
		tune2h=1.00+(tune_hfs*0.04);
	}

	if(flags&RP2POST_FLAGS_FAVOR_SIZE)
	{
		tune1=1.05;
		tune2=1.03;
		tune2h=1.00;
	}

	if(flags&RP2POST_FLAGS_FAVOR_SIZE2)
	{
		tune1=1.00;
		tune2=1.00;
		tune2h=1.00;
	}

#ifdef RP2POST_H12E_C
	osz1=PostRp2Huff_EstimateBufferRp2MinEntropySize(ibuf, ibsz);
	if((osz1*tune1)>=ibsz)
	{
		/* None would be effective here... */
		memcpy(obuf, ibuf, ibsz);
		return(ibsz);
	}
#endif


	if(ibsz>=32768)
	{
		/* Over 32K, just use Huffman */
		/* AdRice+STF will invariably lose */
		/* Range Coding is painfully slow. */

		osz1=-1;
#ifdef RP2POST_RP2BITRC_C
		if(	(osz1<0) && 
			(flags&RP2POST_FLAGS_FAVOR_SIZE2) &&
			!(flags&RP2POST_FLAGS_NO_RANGE))
		{
			/* Priorizing best size, so use BitRC even if slow... */
			osz1=PostRp2BitRc_EncodeBufferPostRp2(
				obuf, ibuf, obsz, ibsz);
		}
#endif
#ifdef RP2POST_H12E_C
		if((osz1<0) && !(flags&RP2POST_FLAGS_NO_HUFF))
		{
			osz1=PostRp2Huff_EncodeBufferPostRp2B(
				obuf, ibuf, obsz, ibsz);
		}
#endif
#ifdef RP2POST_RP2STF_C
		if((osz1<0) && !(flags&RP2POST_FLAGS_NO_ADRICE))
		{
			osz1=PostRp2Stf_EncodeBufferPostRp2(
				obuf, ibuf, obsz, ibsz);
		}
#endif

		if((osz1<=0) || (osz1*tune1)>=ibsz)
		{
			/* None would be effective here... */
			memcpy(obuf, ibuf, ibsz);
			return(ibsz);
		}
		return(osz1);
	}

	if(!postrp2_tc1buf)
	{
		postrp2_tc1buf=malloc(49152);
		postrp2_tc2buf=malloc(49152);
		postrp2_tc3buf=malloc(49152);
	}

	osz1=-1;
	osz2=-1;
	osz3=-1;

#ifdef RP2POST_H12E_C
	osz1=PostRp2Huff_EncodeBufferPostRp2B(postrp2_tc1buf, ibuf, 49152, ibsz);
#endif

#ifdef RP2POST_RP2STF_C
	osz2=PostRp2Stf_EncodeBufferPostRp2(postrp2_tc2buf, ibuf, 49152, ibsz);
#endif

#ifdef RP2POST_RP2BITRC_C
	osz3=PostRp2BitRc_EncodeBufferPostRp2(postrp2_tc3buf, ibuf, 49152, ibsz);
#endif

	if(flags&RP2POST_FLAGS_NO_ADRICE)
		osz2=-1;
	if(flags&RP2POST_FLAGS_NO_HUFF)
		osz1=-1;
	if(flags&RP2POST_FLAGS_NO_RANGE)
		osz3=-1;

	/* Figure out which one did best... */

	osz=ibsz; obi=0;
	if((osz1>0) && ((osz1*tune2h)<osz))
		{ osz=osz1*tune2h; obi=1; }
	if((osz2>0) && ((osz2*tune2s)<osz))
		{ osz=osz2*tune2s; obi=2; }
	if((osz3>0) && ((osz3*tune2)<osz))
		{ osz=osz3*tune2; obi=3; }

	/* Failed to cross 15% threshold. */
	if((osz*tune1)>=ibsz)
		{ osz=ibsz; obi=0; }

	if(obi<=0)
	{
		memcpy(obuf, ibuf, ibsz);
		return(ibsz);
	}

	if(obi==1)
	{
		memcpy(obuf, postrp2_tc1buf, osz1);
		return(osz1);
	}

	if(obi==2)
	{
		memcpy(obuf, postrp2_tc2buf, osz2);
		return(osz2);
	}

	if(obi==3)
	{
		memcpy(obuf, postrp2_tc3buf, osz3);
		return(osz3);
	}

	/* Errm? */
	memcpy(obuf, ibuf, ibsz);
	return(ibsz);
}

/* Try out post compressors over an RP2 compressed buffer.
 * If post compression is ineffective, copies input to output.
 * Use Testing variants to validate round-trip integrity.
 */
int PostRp2_EncodeBufferPostRp2Test(
	byte *obuf, byte *ibuf, int obsz, int ibsz, int flags)
{
	double tune1, tune2, tune2h, tune2s, tune_hfs;
	int osz1, osz2, osz3, osz, obi;

	if(ibsz<640)
	{
		/* Don't bother... */
		memcpy(obuf, ibuf, ibsz);
		return(ibsz);
	}

	tune_hfs=sqrt(32768/ibsz)-1.0;
	tune1=1.10;
	tune2=1.06;
	tune2h=1.00+(tune_hfs*0.01);
	tune2s=1.00;

	if(flags&RP2POST_FLAGS_FAVOR_SPEED)
	{
		tune1=1.15;
		tune2=1.09;
		tune2h=1.00+(tune_hfs*0.02);
	}

	if(flags&RP2POST_FLAGS_FAVOR_SPEED2)
	{
		tune1=1.25;
		tune2=1.75;
		tune2h=1.00+(tune_hfs*0.04);
	}

	if(flags&RP2POST_FLAGS_FAVOR_SIZE)
	{
		tune1=1.05;
		tune2=1.03;
		tune2h=1.00;
	}

	if(flags&RP2POST_FLAGS_FAVOR_SIZE2)
	{
		tune1=1.00;
		tune2=1.00;
		tune2h=1.00;
	}

#ifdef RP2POST_H12E_C
	osz1=PostRp2Huff_EstimateBufferRp2MinEntropySize(ibuf, ibsz);
//	if((osz1*tune1)>=ibsz)
	if(((osz1*tune1)>=ibsz) && !(flags&RP2POST_FLAGS_FAVOR_SIZE2))
	{
		/* None would likely be effective here... */
		if(flags&RP2POST_FLAGS_VERBOSE)
		{
			printf(" Use Raw: Failed Early (%d -> %d, lim %d)\n",
				ibsz, osz1, (int)(ibsz/tune1));
		}
		memcpy(obuf, ibuf, ibsz);
		return(ibsz);
	}
#endif

	if(ibsz>=32768)
	{
		/* Over 32K, just use Huffman */
		/* AdRice+STF will invariably lose */
		/* Range Coding is painfully slow. */

		osz1=-1;
#ifdef RP2POST_RP2BITRC_C
		if(	(osz1<0) &&
			(flags&RP2POST_FLAGS_FAVOR_SIZE2) &&
			!(flags&RP2POST_FLAGS_NO_RANGE))
		{
			/* Priorizing best size, so use BitRC even if slow... */
			osz1=PostRp2BitRc_EncodeBufferPostRp2Test(
				obuf, ibuf, obsz, ibsz);
		}
#endif
#ifdef RP2POST_H12E_C
		if((osz1<0) && !(flags&RP2POST_FLAGS_NO_HUFF))
		{
			osz1=PostRp2Huff_EncodeBufferPostRp2TestB(
				obuf, ibuf, obsz, ibsz);
		}
#endif
#ifdef RP2POST_RP2STF_C
		if((osz1<0) && !(flags&RP2POST_FLAGS_NO_ADRICE))
		{
			osz1=PostRp2Stf_EncodeBufferPostRp2Test(
				obuf, ibuf, obsz, ibsz);
		}
#endif
		if((osz1<=0) || (osz1*tune1)>=ibsz)
		{
			/* None would be effective here... */
			if(flags&RP2POST_FLAGS_VERBOSE)
			{
				printf(" Use Raw: Failed Threshold (%d -> %d)\n", ibsz, osz1);
			}
			memcpy(obuf, ibuf, ibsz);
			return(ibsz);
		}
		if(flags&RP2POST_FLAGS_VERBOSE)
		{
			printf(" Use Post: Passed Threshold (%d -> %d)\n", ibsz, osz1);
		}
		return(osz1);
	}

	if(!postrp2_tc1buf)
	{
		postrp2_tc1buf=malloc(49152);
		postrp2_tc2buf=malloc(49152);
		postrp2_tc3buf=malloc(49152);
	}

	osz1=-1;
	osz2=-1;
	osz3=-1;

#ifdef RP2POST_H12E_C
	osz1=PostRp2Huff_EncodeBufferPostRp2TestB(postrp2_tc1buf, ibuf, 49152, ibsz);
#endif

#ifdef RP2POST_RP2STF_C
	osz2=PostRp2Stf_EncodeBufferPostRp2Test(postrp2_tc2buf, ibuf, 49152, ibsz);
#endif

#ifdef RP2POST_RP2BITRC_C
	osz3=PostRp2BitRc_EncodeBufferPostRp2Test(postrp2_tc3buf, ibuf, 49152, ibsz);
#endif

	if(flags&RP2POST_FLAGS_NO_ADRICE)
		osz2=-1;
	if(flags&RP2POST_FLAGS_NO_HUFF)
		osz1=-1;
	if(flags&RP2POST_FLAGS_NO_RANGE)
		osz3=-1;

	/* Figure out which one did best... */

	osz=ibsz; obi=0;
	if((osz1>0) && ((osz1*tune2h)<osz))
		{ osz=osz1*tune2h; obi=1; }
	if((osz2>0) && ((osz2*tune2s)<osz))
		{ osz=osz2*tune2s; obi=2; }
	if((osz3>0) && ((osz3*tune2)<osz))
		{ osz=osz3*tune2; obi=3; }

	/* Failed to cross 15% threshold. */
	if((osz*tune1)>=ibsz)
		{ osz=ibsz; obi=0; }

	if(obi<=0)
	{
		memcpy(obuf, ibuf, ibsz);
		return(ibsz);
	}

	if(obi==1)
	{
		memcpy(obuf, postrp2_tc1buf, osz1);
		return(osz1);
	}

	if(obi==2)
	{
		memcpy(obuf, postrp2_tc2buf, osz2);
		return(osz2);
	}

	if(obi==3)
	{
		memcpy(obuf, postrp2_tc3buf, osz3);
		return(osz3);
	}

	/* Errm? */
	memcpy(obuf, ibuf, ibsz);
	return(ibsz);
}

int PostRp2_EncodeRP2Auto(
	byte *obuf, byte *ibuf, int obsz, int ibsz, int flags)
{
	static byte *st_buf;
	byte *tbuf;
	int tbsz, osz, tsz;
	
	if(ibsz<64000)
	{
		if(!st_buf)
			st_buf=malloc(65536);
		tbuf=st_buf;
		tbsz=65536;
	}else
	{
		tbsz=ibsz*1.024+256;
		tbuf=malloc(tbsz);
	}
	
	tsz=PostRp2Stf_EncodeRP2(tbuf, ibuf, tbsz, ibsz);
	osz=PostRp2_EncodeBufferPostRp2(obuf, tbuf, obsz, tsz, flags);
	if(tbuf!=st_buf)
		free(tbuf);
	return(osz);
}

int PostRp2_EncodeRP2TestAuto(
	byte *obuf, byte *ibuf, int obsz, int ibsz, int flags)
{
	static byte *st_buf;
	byte *tbuf;
	int tbsz, osz, tsz;
	
	if(ibsz<64000)
	{
		if(!st_buf)
			st_buf=malloc(65536);
		tbuf=st_buf;
		tbsz=65536;
	}else
	{
		tbsz=ibsz*1.024+256;
		tbuf=malloc(tbsz);
	}
	
	tsz=PostRp2Stf_EncodeRP2Test(tbuf, ibuf, tbsz, ibsz);
	osz=PostRp2_EncodeBufferPostRp2Test(obuf, tbuf, obsz, tsz, flags);
	if(tbuf!=st_buf)
		free(tbuf);
	return(osz);
}

#ifdef TGVLZ_ENA_POST_BENCH
int PostRp2_EncodeRP2BenchAuto(
	byte *obuf, byte *ibuf, int obsz, int ibsz, int flags)
{
	byte *tbuf;
	s64 tot;
	double fs;
	int t0, t1, t2, te;
	int osz, i2sz, cm;

	osz=PostRp2_EncodeRP2TestAuto(obuf, ibuf, obsz, ibsz, flags);
	if(osz<=0)
		return(osz);

	cm=0;
	if(((obuf[0]&0x0F)==0x00) && (obuf[1]==0x00))
		cm=(obuf[0]>>4)&15;

	printf(" %d -> %d (cm=%d)\n", ibsz, osz, cm);
	
	tbuf=malloc(ibsz*2);

	i2sz=PostRp2_DecodeBufferRp2Full(tbuf, obuf, ibsz*2, osz);
	if(i2sz<=0)
	{
		printf("Failed Decode\n");
		i2sz=PostRp2_DecodeBufferRp2Full(tbuf, obuf, ibsz*2, osz);
		return(-1);
	}
	
	t0=clock();
	te=t0+(5*CLOCKS_PER_SEC);
	t1=t0;
	tot=0;
	while(t1<te)
	{
		i2sz=PostRp2_DecodeBufferRp2Full(tbuf, obuf, ibsz*2, osz);
		tot+=i2sz;
		t1=clock();
	}
	fs=(t1-t0)*(1.0/CLOCKS_PER_SEC);
	if(fs<=0)
		return(osz);
	printf(" %.2f MB/s\n", tot*(1.0/(fs*1024*1024)));

	return(osz);
}
#endif

#endif
