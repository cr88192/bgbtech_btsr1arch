#include <bgbccc.h>

BGBCC_CMG_Context *BGBCC_CMG_AllocContext()
{
	BGBCC_CMG_Context *tmp;
	
	tmp=bgbcc_malloc(sizeof(BGBCC_CMG_Context));
	return(tmp);
}

int BGBCC_CMG_SetBeginSimPass(BGBCC_CMG_Context *ctx)
{
	int i;

	for(i=0; i<16; i++)
	{
		ctx->sec_vpos[i]=ctx->sec_pos[i];
	}
	ctx->nvlbl=ctx->nlbl;
	ctx->nvlbln=ctx->nlbln;

	for(i=0; i<64; i++)
		ctx->lbl_simhash[i]=(-1);

	ctx->is_simpass=1;
	return(0);
}

int BGBCC_CMG_SetEndSimPass(BGBCC_CMG_Context *ctx)
{
	ctx->is_simpass=0;
	ctx->sim_voffs=0;
	return(0);
}

int BGBCC_CMG_LookupSectionID(BGBCC_CMG_Context *ctx, char *name)
{
	int sec;
	
	sec=-1;
	if(!strcmp(name, ".text"))		sec=BGBCC_SH_CSEG_TEXT;
	if(!strcmp(name, ".data"))		sec=BGBCC_SH_CSEG_DATA;
	if(!strcmp(name, ".bss"))		sec=BGBCC_SH_CSEG_BSS;
	if(!strcmp(name, ".got"))		sec=BGBCC_SH_CSEG_GOT;
	if(!strcmp(name, ".strtab"))	sec=BGBCC_SH_CSEG_STRTAB;
	if(!strcmp(name, ".reloc"))		sec=BGBCC_SH_CSEG_RELOC;

	if(!strcmp(name, ".rdata"))		sec=BGBCC_SH_CSEG_RODATA;
	if(!strcmp(name, ".rodata"))	sec=BGBCC_SH_CSEG_RODATA;
	
	if(sec>=0)
		return(sec);

	for(sec=BGBCC_SH_CSEG_DYN; sec<ctx->nsec; sec++)
	{
		if(!strcmp(name, ctx->sec_name[sec]))	
		{
			return(sec);
		}
	}
	
	return(-1);
}

int BGBCC_CMG_SetSectionName(BGBCC_CMG_Context *ctx, char *name)
{
	int sec;
	
	sec=-1;
	if(!strcmp(name, ".text"))		sec=BGBCC_SH_CSEG_TEXT;
	if(!strcmp(name, ".data"))		sec=BGBCC_SH_CSEG_DATA;
	if(!strcmp(name, ".bss"))		sec=BGBCC_SH_CSEG_BSS;
	if(!strcmp(name, ".got"))		sec=BGBCC_SH_CSEG_GOT;
	if(!strcmp(name, ".strtab"))	sec=BGBCC_SH_CSEG_STRTAB;
	if(!strcmp(name, ".reloc"))		sec=BGBCC_SH_CSEG_RELOC;

	if(!strcmp(name, ".rdata"))		sec=BGBCC_SH_CSEG_RODATA;
	if(!strcmp(name, ".rodata"))	sec=BGBCC_SH_CSEG_RODATA;

	if(sec>=0)
	{
		BGBCC_CMG_SetSection(ctx, sec);
		if(!ctx->sec_name[sec])
			ctx->sec_name[sec]=bgbcc_strdup(name);
		return(0);
	}

	for(sec=BGBCC_SH_CSEG_DYN; sec<ctx->nsec; sec++)
	{
		if(!strcmp(name, ctx->sec_name[sec]))	
		{
			BGBCC_CMG_SetSection(ctx, sec);
			return(0);
		}
	}

	sec=ctx->nsec++;
	if(sec<BGBCC_SH_CSEG_DYN)sec=BGBCC_SH_CSEG_DYN;
	ctx->sec_name[sec]=bgbcc_strdup(name);	
	ctx->sec=sec;
	if((sec+1)>ctx->nsec)
		ctx->nsec=sec+1;
	return(0);
}

int BGBCC_CMG_SetSection(BGBCC_CMG_Context *ctx, int sec)
{
	ctx->sec=sec;
	if((sec+1)>ctx->nsec)
		ctx->nsec=sec+1;
	return(0);
}

int BGBCC_CMG_IsSectionReadOnly(BGBCC_CMG_Context *ctx, int sec)
{
	if(sec==BGBCC_SH_CSEG_TEXT)
		return(1);
	if(sec==BGBCC_SH_CSEG_STRTAB)
		return(1);
	if(sec==BGBCC_SH_CSEG_RODATA)
		return(1);
	return(0);
}

int BGBCC_CMG_EmitCheckExpand(BGBCC_CMG_Context *ctx, int pad)
{
	byte *buf;
	int sz, ofs, vofs;

	if(!ctx->sec_buf[ctx->sec])
	{
//		sz=4096;
//		sz=1<<18;
		sz=1<<20;
		buf=bgbcc_malloc(sz);
		ctx->sec_buf[ctx->sec]=buf;
		ctx->sec_end[ctx->sec]=buf+sz;
		ctx->sec_pos[ctx->sec]=buf;
		ctx->sec_vpos[ctx->sec]=buf;
	}
	
//	if((ctx->sec_pos[ctx->sec]+1)>=ctx->sec_end[ctx->sec])
	if(((ctx->sec_pos[ctx->sec]+pad)>=ctx->sec_end[ctx->sec]) ||
		((ctx->sec_vpos[ctx->sec]+pad)>=ctx->sec_end[ctx->sec]))
	{
		buf=ctx->sec_buf[ctx->sec];
		sz=ctx->sec_end[ctx->sec]-buf;
		ofs=ctx->sec_pos[ctx->sec]-buf;
		vofs=ctx->sec_vpos[ctx->sec]-buf;
		sz=sz+(sz>>1);
		buf=bgbcc_realloc(buf, sz);
		ctx->sec_buf[ctx->sec]=buf;
		ctx->sec_end[ctx->sec]=buf+sz;
		ctx->sec_pos[ctx->sec]=buf+ofs;
		ctx->sec_vpos[ctx->sec]=buf+vofs;
	}
	return(0);
}

int BGBCC_CMG_EmitByteI(BGBCC_CMG_Context *ctx, int val)
{
	byte *buf;
	int sz, ofs, vofs;

	if(ctx->emit_isprobe)
		return(0);

	BGBCC_CMG_EmitCheckExpand(ctx, 1);

	if(ctx->is_simpass)
	{
		*ctx->sec_vpos[ctx->sec]++=val;
		return(0);
	}

	*ctx->sec_pos[ctx->sec]++=val;
	return(0);
}

int BGBCC_CMG_EmitStatWord(BGBCC_CMG_Context *ctx, int val)
{
	int i, j, k;
	
	if(ctx->sec!=BGBCC_SH_CSEG_TEXT)
	{
		ctx->stat_opc_issfx=0;
		return(0);
	}
	
	return(0);

}

/** Returns whether a 32-bit instruction would cross a 32B boundary (and require padding as a result). */
int BGBCC_CMG_CheckPadCross32(BGBCC_CMG_Context *ctx)
{
	int i, j, k;

	if(ctx->test_lclalign)
	{
		return(ctx->test_lclalign&1);
	}

	if(!ctx->use_padcross)
		return(0);

	i=BGBCC_CMG_EmitGetOffs(ctx);
	if(ctx->sec==BGBCC_SH_CSEG_TEXT)
	{
		if(ctx->is_fixed32)
		{
			if(i&3)
				{ BGBCC_DBGBREAK }
		}

		if((i&31)==30)
		{
			return(1);
		}
	}
	return(0);
}

/** Returns whether a 48-bit instruction would cross a 32B boundary (and require padding as a result). */
int BGBCC_CMG_CheckPadCross48(BGBCC_CMG_Context *ctx)
{
	int i, j, k;

	if(ctx->test_lclalign)
	{
		return(ctx->test_lclalign&3);
	}
	
	if(!ctx->use_padcross)
		return(0);

	i=BGBCC_CMG_EmitGetOffs(ctx);
	if(ctx->sec==BGBCC_SH_CSEG_TEXT)
	{
		if((i&31)==30)
			return(1);
		if((i&31)==28)
			return(1);
	}
	return(0);
}

/** Check whether PC is 32-bit aligned. */
int BGBCC_CMG_CheckPadAlign32(BGBCC_CMG_Context *ctx)
{
	int i, j, k;

	i=BGBCC_CMG_EmitGetOffs(ctx);
	
	if(!(i&3))
		return(1);
	return(0);
}

int BGBCC_CMG_EmitWordI(BGBCC_CMG_Context *ctx, int val)
{
	int i, j, k;

	if(ctx->emit_isprobe)
		return(0);

	BGBCC_CMG_EmitStatWord(ctx, val);

	BGBCC_CMG_EmitCheckExpand(ctx, 2);

	if(ctx->is_le)
	{
		if(ctx->is_simpass)
		{
			*ctx->sec_vpos[ctx->sec]++=val;
			*ctx->sec_vpos[ctx->sec]++=val>>8;
			return(0);
		}

		*ctx->sec_pos[ctx->sec]++=val;
		*ctx->sec_pos[ctx->sec]++=val>>8;
		return(0);
	}else
	{
		if(ctx->is_simpass)
		{
			*ctx->sec_vpos[ctx->sec]++=val>>8;
			*ctx->sec_vpos[ctx->sec]++=val;
			return(0);
		}

		*ctx->sec_pos[ctx->sec]++=val>>8;
		*ctx->sec_pos[ctx->sec]++=val;
		return(0);
	}
}

int BGBCC_CMG_EmitDWordI(BGBCC_CMG_Context *ctx, u32 val)
{
	int i, j, k;

	if(ctx->emit_isprobe)
		return(0);

	if(!val && (ctx->sec==BGBCC_SH_CSEG_TEXT))
	{
		k=-1;
	}

	if(ctx->is_le)
	{
		BGBCC_CMG_EmitByteI(ctx, val    );
		BGBCC_CMG_EmitByteI(ctx, val>> 8);
		BGBCC_CMG_EmitByteI(ctx, val>>16);
		BGBCC_CMG_EmitByteI(ctx, val>>24);
		return(0);
	}else
	{
		BGBCC_CMG_EmitByteI(ctx, val>>24);
		BGBCC_CMG_EmitByteI(ctx, val>>16);
		BGBCC_CMG_EmitByteI(ctx, val>> 8);
		BGBCC_CMG_EmitByteI(ctx, val    );
		return(0);
	}
}

int BGBCC_CMG_EmitQWordI(BGBCC_CMG_Context *ctx, s64 val)
{
	int i, j, k;

	if(ctx->emit_isprobe)
		return(0);

	if(!val && (ctx->sec==BGBCC_SH_CSEG_TEXT))
	{
		k=-1;
	}

	if(ctx->is_le)
	{
		BGBCC_CMG_EmitByteI(ctx, (val    )&255);
		BGBCC_CMG_EmitByteI(ctx, (val>> 8)&255);
		BGBCC_CMG_EmitByteI(ctx, (val>>16)&255);
		BGBCC_CMG_EmitByteI(ctx, (val>>24)&255);
		BGBCC_CMG_EmitByteI(ctx, (val>>32)&255);
		BGBCC_CMG_EmitByteI(ctx, (val>>40)&255);
		BGBCC_CMG_EmitByteI(ctx, (val>>48)&255);
		BGBCC_CMG_EmitByteI(ctx, (val>>56)&255);
		return(0);
	}else
	{
		BGBCC_CMG_EmitByteI(ctx, (val>>56)&255);
		BGBCC_CMG_EmitByteI(ctx, (val>>48)&255);
		BGBCC_CMG_EmitByteI(ctx, (val>>40)&255);
		BGBCC_CMG_EmitByteI(ctx, (val>>32)&255);
		BGBCC_CMG_EmitByteI(ctx, (val>>24)&255);
		BGBCC_CMG_EmitByteI(ctx, (val>>16)&255);
		BGBCC_CMG_EmitByteI(ctx, (val>> 8)&255);
		BGBCC_CMG_EmitByteI(ctx, (val    )&255);
		return(0);
	}
}

int BGBCC_CMG_EmitString(BGBCC_CMG_Context *ctx, char *str)
{
	byte *s;
	
	s=str;
	while(*s)
		BGBCC_CMG_EmitByteI(ctx, *s++);
	BGBCC_CMG_EmitByteI(ctx, *s++);

	BGBCC_JX2DA_EmitString(ctx, str);

	return(0);
}

int BGBCC_CMG_EmitAscii(BGBCC_CMG_Context *ctx, char *str)
{
	byte *s;
	
	s=(byte *)str;
	while(*s)
		BGBCC_CMG_EmitByteI(ctx, *s++);

	BGBCC_JX2DA_EmitAscii(ctx, str);

	return(0);
}

int BGBCC_CMG_EmitStringUCS2(BGBCC_CMG_Context *ctx, u16 *str)
{
	return(BGBCC_CMG_EmitStringUCS2B(ctx, str, 0));
}

int BGBCC_CMG_EmitStringUCS2B(BGBCC_CMG_Context *ctx, u16 *str, int noz)
{
	u16 *s;
	
	s=str;
	while(*s)
		BGBCC_CMG_EmitWordI(ctx, *s++);
	if(!noz)
		BGBCC_CMG_EmitWordI(ctx, *s++);

	BGBCC_JX2DA_EmitStringUCS2(ctx, str, noz);
	return(0);
}

int BGBCC_CMG_EmitGetStrtabLabel(BGBCC_CMG_Context *ctx, char *str)
{
	char *s0;
	int sn0;
	int i, j, k;
	
	if(!ctx->lblstr_ofs)
	{
		i=4096;
		ctx->lblstr_ofs=bgbcc_malloc(i*sizeof(u32));
		ctx->lblstr_id=bgbcc_malloc(i*sizeof(u32));
		ctx->nlblstr=0;
		ctx->mlblstr=i;
	}

#if 1
	for(i=0; i<ctx->nlblstr; i++)
	{
		s0=(char *)(ctx->sec_buf[BGBCC_SH_CSEG_STRTAB]+ctx->lblstr_ofs[i]);
		if(!strcmp(s0, str))
			{ return(ctx->lblstr_id[i]); }
	}
#endif

	if(ctx->is_simpass)
	{
		/* Keep symbol labels aligned */
		ctx->nvlbl++;

		return(0);
	}

	sn0=ctx->sec;
	k=BGBCC_CMG_GenLabel(ctx);
	BGBCC_CMG_SetSectionName(ctx, ".strtab");
	i=BGBCC_CMG_EmitLabel(ctx, k);
	BGBCC_CMG_EmitString(ctx, str);

	ctx->sec=sn0;
	
	j=ctx->nlblstr++;
	ctx->lblstr_ofs[j]=ctx->lbl_ofs[i];
	ctx->lblstr_id[j]=k;
	
	return(k);
}

int bgbcc_jx2da_cpto437(int val)
{
	static const u16 cp437ctrl[32]={
		0x0000, 0x263A, 0x263B, 0x2665,
		0x2666, 0x2663, 0x2660, 0x2022,
		0x25D8, 0x25CB, 0x25D9, 0x2642,
		0x2640, 0x266A, 0x266B, 0x263C,
		0x25BA, 0x25C4, 0x2195, 0x203C,
		0x00B6, 0x00A7, 0x25AC, 0x21A8,
		0x2191, 0x2193, 0x2192, 0x2190,
		0x221F, 0x2194, 0x25B2, 0x25BC
	};

	static const u16 cp437blk[128]={
		0x00C7, 0x00FC, 0x00E9, 0x00E2,
		0x00E4, 0x00E0, 0x00E5, 0x00E7,
		0x00EA, 0x00EB, 0x00E8, 0x00EF,
		0x00EE, 0x00EC, 0x00C4, 0x00C5,

		0x00C9, 0x00E6, 0x00C6, 0x00F4,
		0x00F6, 0x00F2, 0x00FB, 0x00F9,
		0x00FF, 0x00D6, 0x00DC, 0x00A2,
		0x00A3, 0x00A5, 0x20A7, 0x0192,

		0x00E1, 0x00ED, 0x00F3, 0x00FA,
		0x00F1, 0x00D1, 0x00AA, 0x00BA,
		0x00BF, 0x2310, 0x00AC, 0x00BD,
		0x00BC, 0x00A1, 0x00AB, 0x00BB,

		0x2591, 0x2592, 0x2593, 0x2502,
		0x2524, 0x2561, 0x2562, 0x2556,
		0x2555, 0x2563, 0x2551, 0x2557,
		0x255D, 0x255C, 0x255B, 0x2510,

		0x2514, 0x2534, 0x252C, 0x251C,
		0x2500, 0x253C, 0x255E, 0x255F,
		0x255A, 0x2554, 0x2569, 0x2566,
		0x2560, 0x2550, 0x256C, 0x2567,

		0x2568, 0x2564, 0x2565, 0x2559,
		0x2558, 0x2552, 0x2553, 0x256B,
		0x256A, 0x2518, 0x250C, 0x2588,
		0x2584, 0x258C, 0x2590, 0x2580,

		0x03B1, 0x00DF, 0x0393, 0x03C0,
		0x03A3, 0x03C3, 0x00B5, 0x03C4,
		0x03A6, 0x0398, 0x03A9, 0x03B4,
		0x221E, 0x03C6, 0x03B5, 0x2229,

		0x2261, 0x00B1, 0x2265, 0x2264,
		0x2320, 0x2321, 0x00F7, 0x2248,
		0x00B0, 0x2219, 0x00B7, 0x221A,
		0x207F, 0x00B2, 0x25A0, 0x00A0};

	int i, j, k;

	if((val>=0x00) && (val<=0x7F))
		return(val);

	for(i=0; i<32; i++)
		if(val==cp437ctrl[i])
			return(0x00+i);
	for(i=0; i<128; i++)
		if(val==cp437blk[i])
			return(0x80+i);

	return(-1);
}

int bgbcc_jx2da_cpto1252(int val)
{
	int k;

	if((val>=0x00) && (val<=0xFF))
		return(val);

	switch(val)
	{
	/* 1252 */
	
	case 0x20AC:	k=0x80; break;
	case 0x0081:	k=0x81; break;
	case 0x2010:	k=0x82; break;
	case 0x0192:	k=0x83; break;
	case 0x201E:	k=0x84; break;
	case 0x2026:	k=0x85; break;
	case 0x2020:	k=0x86; break;
	case 0x2021:	k=0x87; break;
	case 0x02C6:	k=0x88; break;
	case 0x2030:	k=0x89; break;
	case 0x0160:	k=0x8A; break;
	case 0x2039:	k=0x8B; break;
	case 0x0152:	k=0x8C; break;
	case 0x008D:	k=0x8D; break;
	case 0x017D:	k=0x8E; break;
	case 0x008F:	k=0x8F; break;
	case 0x0090:	k=0x90; break;
	case 0x2018:	k=0x91; break;
	case 0x2019:	k=0x92; break;
	case 0x201C:	k=0x93; break;
	case 0x201D:	k=0x94; break;
	case 0x2022:	k=0x95; break;
	case 0x2013:	k=0x96; break;
	case 0x2014:	k=0x97; break;
	case 0x02DC:	k=0x98; break;
	case 0x2122:	k=0x99; break;
	case 0x0161:	k=0x9A; break;
	case 0x203A:	k=0x9B; break;
	case 0x0153:	k=0x9C; break;
	case 0x009D:	k=0x9D; break;
	case 0x017E:	k=0x9E; break;
	case 0x0178:	k=0x9F; break;
	default:		k=-1; break;
	}
	
	return(k);
}

int BGBCC_CMG_EmitGetStrtabLabelUTF2ASCII(
	BGBCC_CMG_Context *ctx, char *str)
{

	char tb[4096];
	char *s, *t;
	int i, j, k;
	
	s=str;
	t=tb;
	while(*s)
	{
		j=BGBCP_ParseChar(&s);
		
		k=bgbcc_jx2da_cpto1252(j);
		if(k<0)
			k=bgbcc_jx2da_cpto437(j);
		if(k<0)
			k=0x8F;
		
		*t++=k;
	}
	*t++=0;
	
	i=BGBCC_CMG_EmitGetStrtabLabel(ctx, tb);
	return(i);
}

int BGBCC_CMG_EmitGetStrtabLabelUCS2(BGBCC_CMG_Context *ctx, u16 *str)
{
	u16 *s0;
	int sn0;
	int i, j, k;
	
	for(i=0; i<ctx->nlbl; i++)
	{
		if(ctx->lbl_sec[i]==BGBCC_SH_CSEG_STRTAB)
		{
			j=ctx->lbl_ofs[i];
			if(j&1)continue;
			s0=(u16 *)(ctx->sec_buf[BGBCC_SH_CSEG_STRTAB]+j);
			if(!BGBCP_StrcmpUCS2(s0, str))
			{
				return(ctx->lbl_id[i]);
			}
		}
	}
	
	sn0=ctx->sec;
	k=BGBCC_CMG_GenLabel(ctx);
	BGBCC_CMG_SetSectionName(ctx, ".strtab");
	BGBCC_CMG_EmitBAlign(ctx, 2);
	BGBCC_CMG_EmitLabel(ctx, k);
	BGBCC_CMG_EmitStringUCS2(ctx, str);
	ctx->sec=sn0;
	return(k);
}

int BGBCC_CMG_EmitGetStrtabLabelUTF2UCS2(
	BGBCC_CMG_Context *ctx, char *str)
{
	u16 tb[4096];
	char *s;
	u16 *t;
	int i, j, k;
	
	s=str;
	t=tb;
	while(*s)
	{
		k=BGBCP_ParseChar(&s);

		if(k>=0x10000)
		{
			j=k-0x10000;
			*t++=0xD800+((j>>10)&1023);
			*t++=0xDC00+((j    )&1023);
			continue;
		}

		*t++=k;
	}
	*t++=0;
	
	i=BGBCC_CMG_EmitGetStrtabLabelUCS2(ctx, tb);
	return(i);
}

int BGBCC_CMG_EmitStringUTF2UCS2(
	BGBCC_CMG_Context *ctx, char *str)
{
	return(BGBCC_CMG_EmitStringUTF2UCS2I(ctx, str, 0));
}

int BGBCC_CMG_EmitAsciiUTF2UCS2(
	BGBCC_CMG_Context *ctx, char *str)
{
	return(BGBCC_CMG_EmitStringUTF2UCS2I(ctx, str, 1));
}

int BGBCC_CMG_EmitStringUTF2UCS2I(
	BGBCC_CMG_Context *ctx, char *str, int noz)
{
	u16 tb[4096];
	char *s;
	u16 *t;
	int i, j, k;
	
	s=str;
	t=tb;
	while(*s)
	{
		k=BGBCP_ParseChar(&s);
		
		if(k>=0x10000)
		{
			j=k-0x10000;
			*t++=0xD800+((j>>10)&1023);
			*t++=0xDC00+((j    )&1023);
			continue;
		}
		
		*t++=k;
	}
	*t++=0;
	
	i=BGBCC_CMG_EmitStringUCS2B(ctx, tb, noz);
	return(i);
}


int BGBCC_CMG_EmitGetStrtabSecOfs(BGBCC_CMG_Context *ctx, char *str)
{
	int l0;
	int i;

	l0=BGBCC_CMG_EmitGetStrtabLabel(ctx, str);
	
	for(i=0; i<ctx->nlbl; i++)
		if(ctx->lbl_id[i]==l0)
			return(ctx->lbl_ofs[i]);
	return(-1);
}

int BGBCC_CMG_EmitRawBytes(BGBCC_CMG_Context *ctx, byte *buf, int sz)
{
	byte *s;
	int n;
	
	if(buf)
	{
		s=buf; n=sz;
		while(n--)
			BGBCC_CMG_EmitByteI(ctx, *s++);
	}else
	{
		if((ctx->sec!=BGBCC_SH_CSEG_BSS) &&
				(ctx->sec!=BGBCC_SH_CSEG_DATA) &&
				(ctx->sec!=BGBCC_SH_CSEG_RELOC))
			{ n=-1; }
	
		n=sz;
		while(n--)
			BGBCC_CMG_EmitByteI(ctx, 0);
	}
	return(0);
}

byte *BGBCC_CMG_EmitGetPos(BGBCC_CMG_Context *ctx)
{
	if(ctx->is_simpass)
	{
		return(ctx->sec_vpos[ctx->sec]);
	}

	return(ctx->sec_pos[ctx->sec]);
}

int BGBCC_CMG_EmitGetOffs(BGBCC_CMG_Context *ctx)
{
	if(ctx->is_simpass)
	{
		return(ctx->sec_vpos[ctx->sec]-ctx->sec_buf[ctx->sec]);
	}

	return(ctx->sec_pos[ctx->sec]-ctx->sec_buf[ctx->sec]);
}

int BGBCC_CMG_EmitBAlign(BGBCC_CMG_Context *ctx, int al)
{
	int i, j, k;
	
	i=BGBCC_CMG_EmitGetOffs(ctx);
	if(!(i&(al-1)))
		return(0);
	j=al-(i&(al-1));

	if(j!=0)
		BGBCC_JX2DA_EmitBAlign(ctx, al);
	
	if(ctx->sec==BGBCC_SH_CSEG_TEXT)
	{
		if(ctx->is_fixed32)
		{
			if(j&1)
				{ BGBCC_CMG_EmitByteI(ctx, ctx->arch_pad_op8); j--; }
			if(j&2)
			{
				BGBCC_CMG_EmitWordI(ctx, ctx->arch_pad_op16);
				j-=2;
			}

			while(j>0)
			{
				BGBCC_CMG_EmitDWordI(ctx, ctx->arch_pad_op32);
				j-=4;
			}
		}else
		{
			if(j&1)
				{ BGBCC_CMG_EmitByteI(ctx, 0); j--; }
			while(j>0)
			{
				BGBCC_CMG_EmitWordI(ctx, ctx->arch_pad_op16);
				j-=2;
			}
		}
	}else
	{
		while(j--)
			BGBCC_CMG_EmitByteI(ctx, 0);
	}

	return(1);
}

byte *BGBCC_CMG_EmitGetPosForOffs(BGBCC_CMG_Context *ctx, int ofs)
{
	return(ctx->sec_buf[ctx->sec]+ofs);
}

int BGBCC_CMG_EmitGetOffsWord(BGBCC_CMG_Context *ctx, int ofs)
{
	byte *ptr;
	ptr=BGBCC_CMG_EmitGetPosForOffs(ctx, ofs);
	if(ctx->is_le)
	{
		return(ptr[0]|(ptr[1]<<8));
	}else
	{
		return((ptr[0]<<8)|ptr[1]);
	}
}

int BGBCC_CMG_EmitGetOffsDWord(BGBCC_CMG_Context *ctx, int ofs)
{
	byte *ptr;
	ptr=BGBCC_CMG_EmitGetPosForOffs(ctx, ofs);
	if(ctx->is_le)
	{
		return(ptr[0]|(ptr[1]<<8)|(ptr[2]<<16)|(ptr[3]<<24));
	}else
	{
		return((ptr[0]<<24)|(ptr[1]<<16)|(ptr[2]<<8)|ptr[3]);
	}
}

int BGBCC_CMG_EmitSetOffsWord(BGBCC_CMG_Context *ctx, int ofs, int val)
{
	byte *ptr;

	if(ctx->is_simpass)
		return(0);

	ptr=BGBCC_CMG_EmitGetPosForOffs(ctx, ofs);
	if(ctx->is_le)
	{
		ptr[0]=val   ;
		ptr[1]=val>>8;
		return(0);
	}else
	{
		ptr[0]=val>>8;
		ptr[1]=val   ;
		return(0);
	}
}

int BGBCC_CMG_EmitSetOffsDWord(BGBCC_CMG_Context *ctx, int ofs, int val)
{
	byte *ptr;

	if(ctx->is_simpass)
		return(0);

	ptr=BGBCC_CMG_EmitGetPosForOffs(ctx, ofs);
	if(ctx->is_le)
	{
		ptr[0]=val    ;
		ptr[1]=val>> 8;
		ptr[2]=val>>16;
		ptr[3]=val>>24;
		return(0);
	}else
	{
		ptr[0]=val>>24;
		ptr[1]=val>>16;
		ptr[2]=val>> 8;
		ptr[3]=val    ;
		return(0);
	}
}

int BGBCC_CMG_EmitByte(BGBCC_CMG_Context *ctx, int val)
{
	BGBCC_CMG_EmitByteI(ctx, val);

	if(ctx->sec!=BGBCC_SH_CSEG_TEXT)
	{
		BGBCC_JX2DA_EmitByte(ctx, val);
	}

	return(0);
}

int BGBCC_CMG_EmitWord(BGBCC_CMG_Context *ctx, int val)
{
	BGBCC_CMG_EmitWordI(ctx, val);

	if(ctx->sec!=BGBCC_SH_CSEG_TEXT)
	{
		BGBCC_JX2DA_EmitWord(ctx, val);
	}

	return(0);
}

int BGBCC_CMG_EmitDWord(BGBCC_CMG_Context *ctx, u32 val)
{
	BGBCC_CMG_EmitDWordI(ctx, val);
	BGBCC_JX2DA_EmitDWord(ctx, val);
	return(0);
}

int BGBCC_CMG_EmitQWord(BGBCC_CMG_Context *ctx, s64 val)
{
	BGBCC_CMG_EmitQWordI(ctx, val);
	BGBCC_JX2DA_EmitQWord(ctx, val);
	return(0);
}

int BGBCC_CMG_EmitDWordAbs32(BGBCC_CMG_Context *ctx, int lbl)
{
	BGBCC_CMG_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_ABS32);
	BGBCC_CMG_EmitDWordI(ctx, 0);
	BGBCC_JX2DA_EmitDWordAbs(ctx, lbl);
	return(0);
}

int BGBCC_CMG_EmitQWordAbs64(BGBCC_CMG_Context *ctx, int lbl)
{
	BGBCC_CMG_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_ABS64);
	BGBCC_CMG_EmitQWordI(ctx, 0);
	BGBCC_JX2DA_EmitQWordAbs(ctx, lbl);
	return(0);
}

