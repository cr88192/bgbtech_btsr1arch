/*
Filesystem related utility code.
*/


char *JX2R_RStrDup(char *str);

int TKPE_MiniPackBlockRP2(byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	byte *hash[256];
	byte *cs, *cse, *lcs;
	byte *ct, *cte;
	byte *cs1a, *cs1b;
	u32 v;
	int pl, pd;
	int l, d, rl, l1, d1, h;
	int i, j, k;

	for(i=0; i<256; i++)
		hash[i]=ibuf;
	
	cs=ibuf; cse=ibuf+ibsz;
	ct=obuf; cte=obuf+obsz;
	
	pd=0; pl=0;
	
	lcs=cs;
	while((cs<cse) && (ct<cte))
	{
		h=*(u32 *)cs;
		h=(h^(h>>7)^(h>>17))&255;
		
		cs1a=cs;
		cs1b=hash[h];
		d=cs-cs1b;
		
		k=cse-cs;
		if(k>514)
			k=514;
		if(d>0)
		{
			for(l=0; l<k; l++)
				if(cs1a[l]!=cs1b[l])
					break;
		}else
			{ l=0; d=0; }
		if((l<4) || (d<=0) || (d>131000))
			{ l=0; d=0; }

		i=(l>=4);
		
		if(!i)
		{
			hash[h]=cs;
			cs++;
			continue;
		}
			
		rl=cs-lcs;

#if 1
		while(rl>(128+7))
		{
			j=(rl>>3)-1;
			if(j>511)j=511;
			
			k=(j+1)*8;

			v=0x3F+(j<<7);
			*ct++=v;
			*ct++=v>>8;

			memcpy(ct, lcs, k);
			ct+=k;	lcs+=k;	rl-=k;
		}
#endif

		while(rl>=8)
		{
			j=(rl>>3)-1;
			if(j>15)j=15;
			
			k=(j+1)*8;
			*ct++=0x07+(j<<4);
			memcpy(ct, lcs, k);
			ct+=k;	lcs+=k;	rl-=k;
		}

		if((rl<8) && (l<=10) && (d<=511))
		{
			d1=d;
			l1=l-3;
			v=(d1<<7)|(l1<<4)|(rl<<1)|0;
			*ct++=v;
			*ct++=v>>8;
			memcpy(ct, lcs, rl);
			ct+=rl;
		}
		else
			if((rl<8) && (l<=67) && (d<=8191))
		{
			d1=d;
			l1=l-4;

			v=(d1<<11)|(l1<<5)|(rl<<2)|1;
			*ct++=v;
			*ct++=v>>8;
			*ct++=v>>16;
			memcpy(ct, lcs, rl);
			ct+=rl;
		}
		else
			if((rl<8) && (l<=515) && (d<=131071))
		{
			d1=d;
			l1=l-4;

			v=(d1<<15)|(l1<<6)|(rl<<3)|3;
			*ct++=v;
			*ct++=v>>8;
			*ct++=v>>16;
			*ct++=v>>24;

			memcpy(ct, lcs, rl);
			ct+=rl;
		}
		else
			if((rl<8) && (l<=0x3FFF) && (d<=0x3FFFFF))
		{
			*ct++=0x0F|(rl<<5);
			d1=d;
			l1=l-4;

			if(l1<128)
			{
				*ct++=(l1<<1);
			}else
			{
				v=(l1<<2)|1;
				*ct++=v;
				*ct++=v>>8;
			}

			if(d1<32768)
			{
				v=d1<<1;
				*ct++=v;
				*ct++=v>>8;
			}else
			{
				v=(d1<<2)|1;
				*ct++=v;
				*ct++=v>>8;
				*ct++=v>>16;
			}

			memcpy(ct, lcs, rl);
			ct+=rl;
		}
		else
		{
//			BGBCC_DBGBREAK
		}
		
		pl=l; pd=d;

		for(j=0; j<l; j++)
		{
			h=*(u32 *)cs;
			h=(h^(h>>7)^(h>>17))&255;
			hash[h]=cs;
			cs++;
		}

		lcs=cs;
	}

	rl=cse-lcs;

#if 1
	while(rl>(128+3))
	{
		j=(rl>>3)-1;
		if(j>511)j=511;
		
		k=(j+1)*8;

		v=0x3F+(j<<7);
		*ct++=v;
		*ct++=v>>8;

		memcpy(ct, lcs, k);
		ct+=k;	lcs+=k;	rl-=k;
	}
#endif

	while(rl>8)
	{
		j=(rl>>3)-1;
		if(j>15)j=15;
		
		k=(j+1)*8;

		*ct++=0x07+(j<<4);
		memcpy(ct, lcs, k);
		ct+=k;	rl-=k;	lcs+=k;
	}

	while(rl)
	{
		j=rl;
		if(j>3)j=3;
		
		*ct++=0x1F+(j<<6);
		memcpy(ct, lcs, j);
		ct+=j;	rl-=j;	lcs+=j;
	}

	*ct++=0x1F;

	return(ct-obuf);
}



int tkdfs_readutf8(byte **rcs)
{
	byte *cs;
	int c0, c1, c2, c3, v;

	cs=*rcs;
	c0=*cs++;
	if(c0<0x80)
	{
		v=c0;
	}else if(c0<0xC0)
	{
		/* broken */
	}else if(c0<0xE0)
	{
		c1=*cs++;
		v=(((c0)&31)<<6)|(c1&63);
	}else if(c0<0xF0)
	{
		c1=*cs++;
		c2=*cs++;
		v=(((c0)&15)<<12)|((c1&63)<<6)|(c2&63);
	}else if(c0<0xF8)
	{
		c1=*cs++;	c2=*cs++;	c3=*cs++;
		v=(((c0)&7)<<18)|((c1&63)<<12)|((c2&63)<<6)|(c3&63);
	}else
	{
	}
	*rcs=cs;
	return(v);
}

int tkdfs_writeutf8(byte **rct, int val)
{
	byte *ct;
	int c0, c1, c2, c3, v;

	ct=*rct;
	if(val<0x80)
	{
		*ct++=val;
	}else if(val<0x0800)
	{
		*ct++=0xC0|((val>> 6)&31);
		*ct++=0x80|((val>> 0)&63);
	}else if(val<0x10000)
	{
		*ct++=0xE0|((val>>12)&15);
		*ct++=0x80|((val>> 6)&63);
		*ct++=0x80|((val>> 0)&63);
	}else if(val<0x100000)
	{
		*ct++=0xF0|((val>>18)& 7);
		*ct++=0x80|((val>>12)&63);
		*ct++=0x80|((val>> 6)&63);
		*ct++=0x80|((val>> 0)&63);
	}
	*rct=ct;
	return(0);
}

int tkdfs_writeutf8uz(byte **rct, int val)
{
	byte *ct;

	ct=*rct;
	if((val<0x20) && (!val || (val>=0x10)))
	{
		*ct++=0xC0|((val>> 6)&31);
		*ct++=0x80|((val>> 0)&63);
	}else
		if(val<0x80)
	{
		*ct++=val;
	}else
	{
		tkdfs_writeutf8(&ct, val);
	}
	*rct=ct;
	return(0);
}

struct tkdfs_utf_comb_s {
u16 v0;
u16 v1;
u16 vc;
u16 fl;
}tkdfs_utf_combarr[] = {
//grave accent
{'A', 0x0300, 0x00C0},	{'E', 0x0300, 0x00C8},
{'I', 0x0300, 0x00CC},	{'O', 0x0300, 0x00D3},
{'U', 0x0300, 0x00D9},	{'a', 0x0300, 0x00E0},
{'e', 0x0300, 0x00E8},	{'i', 0x0300, 0x00EC},
{'o', 0x0300, 0x00F3},	{'u', 0x0300, 0x00F9},

//acute
{'A', 0x0301, 0x00C1},	{'E', 0x0301, 0x00C9},
{'I', 0x0301, 0x00CD},	{'O', 0x0301, 0x00D4},
{'U', 0x0301, 0x00DA},	{'Y', 0x0301, 0x00DC},
{'a', 0x0301, 0x00E1},	{'e', 0x0301, 0x00E9},
{'i', 0x0301, 0x00ED},	{'o', 0x0301, 0x00F4},
{'u', 0x0301, 0x00FA},	{'y', 0x0301, 0x00FC},
{'C', 0x0301, 0x0106},	{'c', 0x0301, 0x0107},
{'L', 0x0301, 0x0139},	{'l', 0x0301, 0x013A},
{'N', 0x0301, 0x0143},	{'n', 0x0301, 0x0144},
{'R', 0x0301, 0x0154},	{'r', 0x0301, 0x0155},
{'S', 0x0301, 0x015A},	{'s', 0x0301, 0x015B},
{'Z', 0x0301, 0x0179},	{'z', 0x0301, 0x017A},

//circumflex
{'A', 0x0302, 0x00C2},	{'E', 0x0302, 0x00CA},
{'I', 0x0302, 0x00CE},	{'O', 0x0302, 0x00D4},
{'U', 0x0302, 0x00DB},	{'a', 0x0302, 0x00E2},
{'e', 0x0302, 0x00EA},	{'i', 0x0302, 0x00EE},
{'o', 0x0302, 0x00F4},	{'u', 0x0302, 0x00FB},

{'C', 0x0302, 0x0108},	{'c', 0x0302, 0x0109},
{'G', 0x0302, 0x011C},	{'g', 0x0302, 0x011D},
{'H', 0x0302, 0x0124},	{'h', 0x0302, 0x0125},
{'J', 0x0302, 0x0134},	{'j', 0x0302, 0x0135},
{'S', 0x0302, 0x015C},	{'s', 0x0302, 0x015D},
{'W', 0x0302, 0x0174},	{'w', 0x0302, 0x0175},
{'Y', 0x0302, 0x0176},	{'y', 0x0302, 0x0177},


//tilde
{'A', 0x0303, 0x00C3},	{'N', 0x0303, 0x00D1},
{'O', 0x0303, 0x00D5},	{'a', 0x0303, 0x00E3},
{'n', 0x0303, 0x00F1},	{'o', 0x0303, 0x00F5},

{'I', 0x0303, 0x0128},	{'i', 0x0303, 0x0129},
{'U', 0x0303, 0x0168},	{'u', 0x0303, 0x0169},

//macron
{'A', 0x0304, 0x0100},	{'a', 0x0304, 0x0101},
{'E', 0x0304, 0x0112},	{'e', 0x0304, 0x0113},
{'I', 0x0304, 0x012A},	{'i', 0x0304, 0x012B},
{'O', 0x0304, 0x014C},	{'o', 0x0304, 0x014D},
{'U', 0x0304, 0x016A},	{'u', 0x0304, 0x016B},


//breve
{'A', 0x0306, 0x0102},	{'a', 0x0306, 0x0103},
{'E', 0x0306, 0x0114},	{'e', 0x0306, 0x0115},
{'G', 0x0306, 0x011E},	{'g', 0x0306, 0x011F},
{'I', 0x0306, 0x012C},	{'i', 0x0306, 0x012D},
{'O', 0x0306, 0x014E},	{'o', 0x0306, 0x014F},
{'U', 0x0306, 0x016C},	{'u', 0x0306, 0x016D},

//dot above
{'C', 0x0307, 0x010A},	{'c', 0x0307, 0x010B},
{'E', 0x0307, 0x0116},	{'e', 0x0307, 0x0117},
{'G', 0x0307, 0x0120},	{'g', 0x0307, 0x0121},
{'I', 0x0307, 0x0130},

{'Z', 0x0307, 0x017B},	{'z', 0x0307, 0x017C},


//umlaut
{'A', 0x0308, 0x00C5},	{'E', 0x0308, 0x00CC},
{'I', 0x0308, 0x00CF},	{'O', 0x0308, 0x00D6},
{'U', 0x0308, 0x00DC},	{'Y', 0x0308, 0x009F, 1},
{'a', 0x0308, 0x00E5},	{'e', 0x0308, 0x00EC},
{'i', 0x0308, 0x00EF},	{'o', 0x0308, 0x00F6},
{'u', 0x0308, 0x00FC},	{'y', 0x0308, 0x00FF},

{'Y', 0x0308, 0x0178},	//alt

//ring above
{'U', 0x030A, 0x016E},	{'u', 0x030A, 0x016F},

//double acute
{'O', 0x030B, 0x0150},	{'o', 0x030B, 0x0151},
{'U', 0x030B, 0x0170},	{'u', 0x030B, 0x0171},

//caron
{'S', 0x030C, 0x008A, 1},	{'Z', 0x030C, 0x008E, 1},
{'s', 0x030C, 0x009A, 1},	{'z', 0x030C, 0x009E, 1},

{'C', 0x030C, 0x010C},	{'c', 0x030C, 0x010D},
{'D', 0x030C, 0x010E},	{'d', 0x030C, 0x010F},
{'E', 0x030C, 0x011A},	{'e', 0x030C, 0x011B},

{'S', 0x030C, 0x0160},	{'Z', 0x030C, 0x017D},
{'s', 0x030C, 0x0161},	{'z', 0x030C, 0x017E},

//cedilla
{'G', 0x0327, 0x0122},	{'g', 0x0327, 0x0123},
{'K', 0x0327, 0x0136},	{'k', 0x0327, 0x0137},
{'L', 0x0327, 0x013B},	{'l', 0x0327, 0x013C},
{'N', 0x0327, 0x0145},	{'n', 0x0327, 0x0146},
{'R', 0x0327, 0x0156},	{'r', 0x0327, 0x0157},
{'S', 0x0327, 0x015E},	{'s', 0x0327, 0x015F},
{'T', 0x0327, 0x0162},	{'t', 0x0327, 0x0163},

//ogonek
{'A', 0x0328, 0x0104},	{'a', 0x0328, 0x0105},
{'E', 0x0328, 0x0118},	{'e', 0x0328, 0x0119},
{'I', 0x0328, 0x012E},	{'i', 0x0328, 0x012F},
{'U', 0x0328, 0x0172},	{'u', 0x0328, 0x0173},

//bar / stroke
{'D', 0x0335, 0x0110},	{'d', 0x0335, 0x0111},
{'H', 0x0335, 0x0126},	{'h', 0x0335, 0x0127},
{'L', 0x0335, 0x0141},	{'l', 0x0335, 0x0142},
{'T', 0x0335, 0x0166},	{'t', 0x0335, 0x0167},

{'B', 0x0335, 0x0180},	{'b', 0x0335, 0x0181},
{'Z', 0x0335, 0x01B5},	{'z', 0x0335, 0x01B6},
{'G', 0x0335, 0x01E4},	{'g', 0x0335, 0x01E5},

{'J', 0x0335, 0x0248},	{'j', 0x0335, 0x0249},
{'R', 0x0335, 0x024C},	{'R', 0x0335, 0x024D},
{'y', 0x0335, 0x024E},	{'y', 0x0335, 0x024F},

// diagonal
{'C', 0x0337, 0x023B},	{'c', 0x0337, 0x023C},
{'I', 0x0337, 0x0197},	{'A', 0x0337, 0x023A},

//decompose only
{'I', 'J', 0x0132, 1},	{'i', 'j', 0x0133, 1},	{'\'', 'n', 0x0149, 1},
{'L', 'J', 0x01C7, 1},	{'L', 'j', 0x01C8, 1},	{'l', 'j', 0x01C9, 1},
{'N', 'J', 0x01CA, 1},	{'N', 'j', 0x01CB, 1},	{'n', 'j', 0x01CC, 1},
{'D', 'Z', 0x01F1, 1},	{'D', 'z', 0x01F2, 1},	{'d', 'z', 0x01F3, 1},

{'I'   , 0  , 0x2160, 1},	{'i'   , 0  , 0x2170, 1},
{'V'   , 0  , 0x2164, 1},	{'v'   , 0  , 0x2174, 1},
{'X'   , 0  , 0x2169, 1},	{'x'   , 0  , 0x2179, 1},
{'L'   , 0  , 0x216C, 1},	{'l'   , 0  , 0x217C, 1},
{'C'   , 0  , 0x216D, 1},	{'c'   , 0  , 0x217D, 1},
{'D'   , 0  , 0x216E, 1},	{'d'   , 0  , 0x217E, 1},
{'M'   , 0  , 0x216F, 1},	{'m'   , 0  , 0x217F, 1},

{0x2160, 0x2160, 0x2161, 0},	{0x2170, 0x2170, 0x2171, 0},
{0x2161, 0x2160, 0x2162, 0},	{0x2171, 0x2170, 0x2172, 0},
{0x2160, 0x2164, 0x2163, 0},	{0x2170, 0x2174, 0x2173, 0},
{0x2164, 0x2160, 0x2165, 0},	{0x2174, 0x2170, 0x2175, 0},
{0x2165, 0x2160, 0x2166, 0},	{0x2175, 0x2170, 0x2176, 0},
{0x2166, 0x2160, 0x2167, 0},	{0x2176, 0x2170, 0x2177, 0},
{0x2160, 0x2169, 0x2168, 0},	{0x2170, 0x2179, 0x2178, 0},
{0x2169, 0x2160, 0x216A, 0},	{0x2179, 0x2170, 0x217A, 0},
{0x216A, 0x2160, 0x216B, 0},	{0x217A, 0x2170, 0x217B, 0},
{0x216D, 0x216E, 0x2180, 0},


{0, 0, 0}
};

s16 *tkdfs_utf_combchn;
s16 *tkdfs_utf_splitchn;

s16 tkdfs_utf_combhash[256];
s16 tkdfs_utf_splithash[256];

int TKDFS_UtfMergeInit()
{
	int v0, v1, vc;
	int i, j, k, h, n;
	
	for(i=0; i<256; i++)
	{
		tkdfs_utf_combhash[i]=-1;
		tkdfs_utf_splithash[i]=-1;
	}

	n=0;
	for(i=0; tkdfs_utf_combarr[i].vc; i++) { }
	n=i;

	tkdfs_utf_combchn=malloc(2*(n+1)*sizeof(short));
	tkdfs_utf_splitchn=tkdfs_utf_combchn+(n+1);

//	for(i=0; tkdfs_utf_combhash[i].vc; i++)
	for(i=n-1; i>=0; i--)
	{
		v0=tkdfs_utf_combarr[i].v0;
		v1=tkdfs_utf_combarr[i].v1;
		vc=tkdfs_utf_combarr[i].vc;

		h=(v0^v1);
		h=h^(h>>7);
		h=h&255;

		j=tkdfs_utf_combhash[h];
		tkdfs_utf_combchn[i]=j;
		tkdfs_utf_combhash[h]=i;

		h=(vc^(vc>>7))&255;
		j=tkdfs_utf_splithash[h];
		tkdfs_utf_splitchn[i]=j;
		tkdfs_utf_splithash[h]=i;
	}
	
	return(0);
}

int TKDFS_UtfCheckSplit(int vi, int *rv0, int *rv1)
{
	int v0, v1, vc, vf, vo0, vo1;
	int i, j, k, h;

	vo0=-1; vo1=-1;
	h=(vi^(vi>>7))&255;
	i=tkdfs_utf_splithash[h];
	while(i>=0)
	{
		v0=tkdfs_utf_combarr[i].v0;
		v1=tkdfs_utf_combarr[i].v1;
		vc=tkdfs_utf_combarr[i].vc;
//		vf=tkdfs_utf_combarr[i].fl;

		if(vc==vi)
		{
			vo0=v0;
			vo1=v1;
			break;
		}

		i=tkdfs_utf_splitchn[i];
	}
	
	if((vo0>0) && (vo1>0))
	{
		*rv0=vo0;
		*rv1=vo1;
		return(1);
	}
	
	*rv0=vi;
	*rv1=-1;
	return(0);
}

int TKDFS_UtfCheckMerge(int v0, int v1, int *rvo)
{
	int av0, av1, avf;
	int vo, i, h;
	
	vo=-1;

	h=(v0^v1);
	h=h^(h>>7);
	h=h&255;

	i=tkdfs_utf_combhash[h];
	while(i>=0)
	{
		av0=tkdfs_utf_combarr[i].v0;
		av1=tkdfs_utf_combarr[i].v1;
		avf=tkdfs_utf_combarr[i].fl;
		if((av0==v0) && (av1==v1) && !(avf&1))
		{
			vo=tkdfs_utf_combarr[i].vc;
			break;
		}
		i=tkdfs_utf_combchn[i];
	}
	
	if(vo>0)
	{
		*rvo=vo;
		return(1);
	}
	
	*rvo=v0;
	return(0);
}

int TKDFS_NormalizePathUtf8I(byte *dst, byte *src, int cfl)
{
	u16 tca[512], tcb[512];
	u16 tcpb[8];
	byte *cs, *ct;
	u16 *csa, *cta, *csae;
	int v, v0, v1, len, len1, pbs;
	
	TKDFS_UtfMergeInit();
	
	cs=src;
	cta=tca;
	while(*cs)
	{
		v=tkdfs_readutf8(&cs);
		if(v<=0xFFFF)
		{
			*cta++=v;
			continue;
		}
		
		v0=0xD800+((v>>10)&1023);
		v1=0xDC00+((v>> 0)&1023);
		*cta++=v0;
		*cta++=v1;
	}
	*cta=0;
	len=cta-tca;

	csa=tca; csae=tca+len;
	cta=tcb; pbs=0;
	while(csa<csae)
	{
		v=*csa++;
		while(TKDFS_UtfCheckSplit(v, &v0, &v1)>0)
		{
			if(v1)
				tcpb[pbs++]=v1;
			v=v0;
		}
		*cta++=v;
		while(pbs)
			{ pbs--; *cta++=tcpb[pbs]; }
	}
	len1=cta-tcb;

	csa=tcb; csae=tcb+len1;
	cta=tca;
	while(csa<csae)
	{
		v0=*csa++;
		v1=*csa;
		while(TKDFS_UtfCheckMerge(v0, v1, &v)>0)
			{ csa++; v0=v; v1=*csa; }
		*cta++=v0;
	}
	len=cta-tca;

	csa=tca; csae=tca+len;
	ct=dst;
	while(csa<csae)
	{
		v0=csa[0];
		v1=csa[1];
		
		if(!(cfl&1))
		{
			if(((v0&0xFC00)==0xD800) && ((v1&0xFC00)==0xDC00))
			{
				v=((v0&1023)<<10)|(v1&1023);
				csa+=2;
				tkdfs_writeutf8uz(&ct, v);
				continue;
			}
		}
		
		csa++;
		tkdfs_writeutf8uz(&ct, v0);
	}
	*ct++=0;
	return(0);
}

int TKDFS_NormalizePathUtf8(byte *dst, byte *src, int cfl)
{
	byte *cs;
	int i, l;
	
	cs=src;
	i=*cs;
	while(i && !(i&0x80)) { i=*cs++; }
	l=cs-src;
	
	if(!i)
	{
		memcpy(dst, src, l+1);
		return(0);
	}
	
	TKDFS_NormalizePathUtf8I(dst, src, cfl);
	return(1);
}

char *TKDFS_TempNormalizeStringUtf8(char *src, int cfl)
{
	byte tbuf[512];
	char *tstr1;
	int i;
	
	i=TKDFS_NormalizePathUtf8(tbuf, (byte *)src, cfl);
	
	if(i>0)
	{
		if(!strcmp((char *)tbuf, src))
			return(src);
	
//		tstr1=tk_rstrdup((char *)tbuf);
		tstr1=JX2R_RStrDup((char *)tbuf);
		return(tstr1);
	}
	
	return(src);
}
