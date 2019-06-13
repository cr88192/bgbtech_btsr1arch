#include <bgbccc.h>

BGBCC_JX2_Context *BGBCC_JX2_AllocContext()
{
	BGBCC_JX2_Context *tmp;
	
	tmp=bgbcc_malloc(sizeof(BGBCC_JX2_Context));
	return(tmp);
}

int BGBCC_JX2_SetBeginSimPass(BGBCC_JX2_Context *ctx)
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

int BGBCC_JX2_SetEndSimPass(BGBCC_JX2_Context *ctx)
{
	ctx->is_simpass=0;
	ctx->sim_voffs=0;
	return(0);
}

int BGBCC_JX2_LookupSectionID(BGBCC_JX2_Context *ctx, char *name)
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

int BGBCC_JX2_SetSectionName(BGBCC_JX2_Context *ctx, char *name)
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
		BGBCC_JX2_SetSection(ctx, sec);
		if(!ctx->sec_name[sec])
			ctx->sec_name[sec]=bgbcc_strdup(name);
		return(0);
	}

	for(sec=BGBCC_SH_CSEG_DYN; sec<ctx->nsec; sec++)
	{
		if(!strcmp(name, ctx->sec_name[sec]))	
		{
			BGBCC_JX2_SetSection(ctx, sec);
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

int BGBCC_JX2_SetSection(BGBCC_JX2_Context *ctx, int sec)
{
	ctx->sec=sec;
	if((sec+1)>ctx->nsec)
		ctx->nsec=sec+1;
	return(0);
}

int BGBCC_JX2_IsSectionReadOnly(BGBCC_JX2_Context *ctx, int sec)
{
	if(sec==BGBCC_SH_CSEG_TEXT)
		return(1);
	if(sec==BGBCC_SH_CSEG_STRTAB)
		return(1);
	if(sec==BGBCC_SH_CSEG_RODATA)
		return(1);
	return(0);
}

int BGBCC_JX2_IsSectionGpRel(BGBCC_JX2_Context *ctx, int sec)
{
	if(!ctx->is_pbo)
		return(0);

	if(sec==BGBCC_SH_CSEG_DATA)
		return(1);
	if(sec==BGBCC_SH_CSEG_BSS)
		return(1);
	return(0);
}

int BGBCC_JX2_EmitCheckExpand(BGBCC_JX2_Context *ctx, int pad)
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

int BGBCC_JX2_EmitByteI(BGBCC_JX2_Context *ctx, int val)
{
	byte *buf;
	int sz, ofs, vofs;

	if(ctx->emit_isprobe)
		return(0);

	BGBCC_JX2_EmitCheckExpand(ctx, 1);

	if(ctx->is_simpass)
	{
		*ctx->sec_vpos[ctx->sec]++=val;
		return(0);
	}

	*ctx->sec_pos[ctx->sec]++=val;
	return(0);
}

void BGBCC_JX2_UpdatePszxWord(BGBCC_JX2_Context *ctx, int val)
{
	int rn_dfl, rm_dfl, rn_sx, rm_sx;
	int rk_dfl, rj_dfl, rk_sx, rj_sx;
	int rx_dfl, rx_sx;
	int rn, sx;
	
	if((val&0xF000)==0xF000)
		return;

	rn_dfl=(val>>4)&15;
	rm_dfl=(val>>0)&15;
	rn_sx=ctx->reg_pszx[rn_dfl];
	rm_sx=ctx->reg_pszx[rm_dfl];

	rk_dfl=16+((val>>4)&15);
	rj_dfl=16+((val>>0)&15);
	rk_sx=ctx->reg_pszx[rk_dfl];
	rj_sx=ctx->reg_pszx[rj_dfl];

	rx_dfl=((val>>4)&15)|((val&0x0800)>>7);
	rx_sx=ctx->reg_pszx[rx_dfl];

	rn=-1; sx=0;
	switch((val>>12)&15)
	{
	case 0:
		switch((val>>8)&15)
		{
		case 0: case 1: case 2: case 3:
		case 4: case 5: case 6: case 7:
			break;
		case 0x8: case 0x9: case 0xA:
		case 0xC: case 0xD: case 0xE:
			rn=rn_dfl;	sx=BGBCC_PSZX_SX;	break;
		case 0xB:		case 0xF:
			rn=rn_dfl;	sx=BGBCC_PSZX_QW;	break;
		}
		break;
	case 1:
		switch((val>>8)&15)
		{
		case 0: case 1: case 2: case 3:
		case 5: case 6: case 7:
			rn=rn_dfl;	sx=0;	break;

		case 0x8:
			rn=rn_dfl;	sx=rm_sx;	break;
		case 0x9:
			rn=rn_dfl;	sx=rj_sx;	break;
		case 0xA:
			rn=rk_dfl;	sx=rm_sx;	break;
		case 0xB:
			rn=rk_dfl;	sx=rj_sx;	break;

		case 0xC: case 0xD: case 0xE:
			break;
		}
		break;

	case 2:
		switch((val>>8)&15)
		{
		case 0: case 1: case 2: case 3:
			break;
		case 4: case 5: case 6: case 7:
			break;
//		case 8: case 9:
//			break;
//		case 0xA:
//			rn=rn_dfl;	sx=BGBCC_PSZX_SX;	break;
//		case 0xB:
//			rn=rn_dfl;	sx=BGBCC_PSZX_QW;	break;

		case 0x8:
			rn=rn_dfl;	sx=BGBCC_PSZX_ZX;	break;
		case 0xA:
			rn=rk_dfl;	sx=BGBCC_PSZX_ZX;	break;

		case 0xC: case 0xD: case 0xE: case 0xF:
			break;
		}
		break;

	case 3:
//		switch((val>>8)&15)
		switch((val>>8)&7)
		{
		case 0:
			break;
		case 2:
			switch(val&15)
			{
			case 4:			case 5:
				rn=rx_dfl;	sx=BGBCC_PSZX_SZX;	break;
			case 6:			case 7:
				rn=rx_dfl;	sx=BGBCC_PSZX_SSX;	break;
			default:
				rn=rx_dfl;	sx=BGBCC_PSZX_UNK;	break;
			}
			break;

		case 6:
			switch(val&15)
			{
			case 4:
				rn=rx_dfl;	sx=BGBCC_PSZX_ZX;	break;
			case 5:
				rn=rx_dfl;	sx=BGBCC_PSZX_SX;	break;
			default:
				rn=rx_dfl;	sx=BGBCC_PSZX_UNK;	break;
			}
			break;

		default:
			rn=rx_dfl;	sx=BGBCC_PSZX_UNK;	break;
			break;
		}
		break;

	case 4:
		switch((val>>8)&15)
		{
		case 0x0:	case 0x1:
		case 0x2:	case 0x3:
			break;
		case 0x4:
			rn=rn_dfl;	sx=BGBCC_PSZX_SX;	break;
		case 0x5:
			rn=rn_dfl;	sx=BGBCC_PSZX_QW;	break;
		case 0x6:
			rn=rk_dfl;	sx=BGBCC_PSZX_SX;	break;
		case 0x7:
			rn=rk_dfl;	sx=BGBCC_PSZX_QW;	break;
		default:
			rn=rn_dfl;	sx=BGBCC_PSZX_UNK;	break;
		}
		break;

	case 5:
		switch((val>>8)&15)
		{
		case 0:		case 1:		case 2:		case 3:
			rn=rn_dfl;	sx=BGBCC_PSZX_SZX;	break;
		default:
			rn=rn_dfl;	sx=BGBCC_PSZX_UNK;	break;
			break;
		}
		break;
	case 6:
		rn=rn_dfl;	sx=BGBCC_PSZX_UNK;	break;
	case 7:
		rn=rn_dfl;	sx=BGBCC_PSZX_UNK;	break;

	case 8:
		switch((val>>8)&15)
		{
		case 0: case 8:
			rn=rn_dfl;	sx=BGBCC_PSZX_ZX;	break;

		case 1:				case 2: case 3:
		case 4: case 5:	case 6: case 7:
			break;

		case 0x9:				case 0xA: case 0xB:
		case 0xC: case 0xD:	case 0xE: case 0xF:
			rn=rn_dfl;	sx=BGBCC_PSZX_SX;	break;
		}
		break;

	case 9:
		break;

	case 0xA:
		rn=0; sx=BGBCC_PSZX_SZX;	break;
	case 0xB:
		rn=0; sx=BGBCC_PSZX_SSX;	break;
	case 0xC:
		rn=(val>>8)&15;	sx=BGBCC_PSZX_UNK;	break;
	case 0xD:
		rn=(val>>8)&15;
		sx=(val&128)?BGBCC_PSZX_SSX:BGBCC_PSZX_SZX;
		break;
	case 0xE:
		break;
	case 0xF:
		break;
	}
	
	if(rn>=0)
	{
		ctx->reg_pszx[rn]=sx;
	}
}

void BGBCC_JX2_UpdatePszxWordF0(BGBCC_JX2_Context *ctx,
	int opw1, int opw2)
{
	int rn_dfl, rm_dfl, ro_dfl, rn_sx, rm_sx, ro_sx;
	int rn, sx, eq, eo;

#if 0	
	rn_dfl=(opw2>>4)&15;
	rm_dfl=(opw2>>0)&15;
	ro_dfl=(opw1>>0)&15;
	
	eq=(opw1&0x0080)!=0;
	eo=(opw1&0x0010)!=0;

	if(opw1&0x0040)rn_dfl+=16;
	if(opw1&0x0020)rm_dfl+=16;
	if(opw1&0x0010)ro_dfl+=16;
#endif
	
#if 1
	rn_dfl=(opw1>>4)&15;
	rm_dfl=(opw1>>0)&15;
	ro_dfl=(opw2>>4)&15;
	
	eq=(opw2&0x0800)!=0;
	eo=(opw2&0x0100)!=0;

	if(opw1&0x0400)rn_dfl+=16;
	if(opw1&0x0200)rm_dfl+=16;
	if(opw1&0x0100)ro_dfl+=16;
#endif
	
	rn_sx=ctx->reg_pszx[rn_dfl];
	rm_sx=ctx->reg_pszx[rm_dfl];
	ro_sx=ctx->reg_pszx[ro_dfl];

	rn=-1; sx=0;
	switch((opw2>>12)&15)
	{
	case 0:
//		switch((opw2>>8)&15)
		switch((opw2>>0)&15)
		{
		case 0: case 1: case 2: case 3:
			if(eq)
			{
				rn=rn_dfl;	sx=BGBCC_PSZX_UNK;
				break;
			}
			break;
		case 4: case 5: case 6: case 7:
			if(eq)
			{
				rn=rn_dfl;	sx=BGBCC_PSZX_UNK;
				break;
			}
			break;
		case 0x8: case 0x9:
		case 0xC: case 0xD:
			rn=rn_dfl;	sx=BGBCC_PSZX_SSX;
			if(eq)
				sx=BGBCC_PSZX_SZX;
			break;
		case 0xA: case 0xE:
			rn=rn_dfl;	sx=BGBCC_PSZX_SX;
			if(eq)
				sx=BGBCC_PSZX_ZX;
			break;
		case 0xB:		case 0xF:
			rn=rn_dfl;	sx=BGBCC_PSZX_QW;
			if(eq)
				sx=BGBCC_PSZX_UNK;
			break;
		}
		break;

	case 1:
//		switch((opw2>>8)&15)
		switch((opw2>>0)&15)
		{
		case 0: case 1: case 2: case 3:
		case 4: case 5: case 6: case 7:
			rn=rn_dfl;	sx=BGBCC_PSZX_UNK;
			break;
		case 8:
			rn=rn_dfl;	sx=BGBCC_PSZX_UNK;
			break;
		case 9:	
//			switch((opw1>>0)&15)
			switch((opw2>>4)&15)
			{
			case 8:
				if(eq)
					{ rn=rn_dfl; sx=BGBCC_PSZX_UNK; break; }
				rn=rn_dfl;	sx=rm_sx;
				break;
			default:
				rn=rn_dfl;	sx=BGBCC_PSZX_UNK;
				break;
			}
			break;

		case 0xC:	
//			switch((opw1>>0)&15)
			switch((opw2>>4)&15)
			{
			case 6:
				rn=rn_dfl;	sx=eq?BGBCC_PSZX_QW:BGBCC_PSZX_SX;
				break;
			case 7:
				rn=rn_dfl;	sx=eq?BGBCC_PSZX_QW:BGBCC_PSZX_ZX;
				break;
			case 8:
				rn=rn_dfl;
				sx=eq?BGBCC_PSZX_SZX:BGBCC_PSZX_SSX;
				break;
			case 9:
				if(eo)
				{
					rn=rn_dfl;
					sx=eq?BGBCC_PSZX_ZX:BGBCC_PSZX_SX;
					break;
				}
				rn=rn_dfl;
				sx=eq?BGBCC_PSZX_SZX:BGBCC_PSZX_SSX;
				break;
			default:
				rn=rn_dfl;	sx=BGBCC_PSZX_UNK;
				break;
			}
			break;

		default:
			rn=rn_dfl;	sx=BGBCC_PSZX_UNK;
			break;
		}
		break;

	case 0x2:
//		switch((opw2>>8)&15)
		switch((opw2>>0)&15)
		{
//		case 4: rn=0; sx=BGBCC_PSZX_SZX; break;
//		case 5: rn=0; sx=BGBCC_PSZX_SSX; break;
//		case 6: rn=0; sx=BGBCC_PSZX_UNK; break;
		default:
			break;
		}
		break;

	case 0x3:
//		if(((opw2>>8)&15)==0)
		if(((opw2>>0)&15)==0)
			{ rn=ro_dfl;	sx=BGBCC_PSZX_UNK; break; }
		rn=rn_dfl;	sx=BGBCC_PSZX_UNK;
		break;

	case 0x5:
//		switch((opw2>>8)&15)
		switch((opw2>>0)&15)
		{
		case 0x4:	rn=rn_dfl; sx=eq?BGBCC_PSZX_QW:BGBCC_PSZX_SX; break;
		case 0x5:	rn=rn_dfl; sx=eq?BGBCC_PSZX_QW:BGBCC_PSZX_ZX; break;
//		case 0x6:	rn=rn_dfl; sx=BGBCC_PSZX_SX; break;
//		case 0x7:	rn=rn_dfl; sx=BGBCC_PSZX_ZX; break;
//		case 0x8: case 0x9: case 0xA: case 0xB:
//			rn=rn_dfl; sx=BGBCC_PSZX_QW; break;

		default:
			rn=rn_dfl;	sx=BGBCC_PSZX_UNK;
			break;
		}
		break;

	case 0xA:
	case 0xB:
	case 0xC:
	case 0xD:
	case 0xE:
	case 0xF:
		break;

	default:
		rn=rn_dfl;	sx=BGBCC_PSZX_UNK;
		break;
	}

	if(rn>=0)
	{
		ctx->reg_pszx[rn]=sx;
	}
}

void BGBCC_JX2_UpdatePszxWordF1(BGBCC_JX2_Context *ctx,
	int opw1, int opw2)
{
	int rn_dfl, rm_dfl, ro_dfl, rn_sx, rm_sx, ro_sx;
	int rn, sx, eq;
	
//	rn_dfl=(opw2>>4)&15;
//	rm_dfl=(opw2>>0)&15;
//	ro_dfl=(opw1>>0)&15;

	rn_dfl=(opw1>>4)&15;
	rm_dfl=(opw1>>0)&15;
//	ro_dfl=(opw1>>0)&15;
	
//	eq=(opw1&0x0080)!=0;
//	if(opw1&0x0040)rn_dfl+=16;
//	if(opw1&0x0020)rm_dfl+=16;
//	if(opw1&0x0010)ro_dfl+=16;

	eq=(opw2&0x0800)!=0;
	if(opw2&0x0400)rn_dfl+=16;
	if(opw2&0x0200)rm_dfl+=16;
//	if(opw2&0x0100)ro_dfl+=16;
	
	rn_sx=ctx->reg_pszx[rn_dfl];
	rm_sx=ctx->reg_pszx[rm_dfl];
//	ro_sx=ctx->reg_pszx[ro_dfl];

	rn=-1; sx=0;
	switch((opw2>>12)&15)
	{
	case 0: case 1: case 2: case 3:
	case 4: case 5: case 6: case 7:
		if(eq)
		{
			rn=rn_dfl;	sx=BGBCC_PSZX_UNK;
			break;
		}
		break;
	case 0x8: case 0x9:
	case 0xC: case 0xD:
		rn=rn_dfl;	sx=BGBCC_PSZX_SSX;
		if(eq)
			sx=BGBCC_PSZX_SZX;
		break;
	case 0xA: case 0xE:
		rn=rn_dfl;	sx=BGBCC_PSZX_SX;
		if(eq)
			sx=BGBCC_PSZX_ZX;
		break;
	case 0xB:		case 0xF:
		rn=rn_dfl;	sx=BGBCC_PSZX_QW;
		if(eq)
			sx=BGBCC_PSZX_UNK;
		break;
	}

	if(rn>=0)
	{
		ctx->reg_pszx[rn]=sx;
	}
}

void BGBCC_JX2_UpdatePszxWordF2(BGBCC_JX2_Context *ctx,
	int opw1, int opw2)
{
	int rn_dfl, rm_dfl, ro_dfl, rn_sx, rm_sx, ro_sx;
	int rn, sx, eq;
	
//	rn_dfl=(opw2>>4)&15;
//	rm_dfl=(opw2>>0)&15;
//	ro_dfl=(opw1>>0)&15;

	rn_dfl=(opw1>>4)&15;
	rm_dfl=(opw1>>0)&15;
//	ro_dfl=(opw1>>0)&15;
	
//	eq=(opw1&0x0080)!=0;
//	if(opw1&0x0040)rn_dfl+=16;
//	if(opw1&0x0020)rm_dfl+=16;
//	if(opw1&0x0010)ro_dfl+=16;

	eq=(opw2&0x0800)!=0;
	if(opw2&0x0400)rn_dfl+=16;
	if(opw2&0x0200)rm_dfl+=16;
//	if(opw2&0x0100)ro_dfl+=16;
	
	rn_sx=ctx->reg_pszx[rn_dfl];
	rm_sx=ctx->reg_pszx[rm_dfl];
//	ro_sx=ctx->reg_pszx[ro_dfl];

	rn=-1; sx=0;
	switch((opw2>>12)&15)
	{
	case 0: case 1:
		rn=rn_dfl;	sx=BGBCC_PSZX_QW;
		break;
	case 2:
		rn=rn_dfl;	sx=BGBCC_PSZX_SX;
		break;
	case 3:
		rn=rn_dfl;	sx=eq?BGBCC_PSZX_ZX:BGBCC_PSZX_SX;
		break;
	case 4:
		rn=rn_dfl;	sx=eq?BGBCC_PSZX_ZX:BGBCC_PSZX_SX;
		break;
	case 5: case 6: case 7:
		rn=rn_dfl;	sx=BGBCC_PSZX_UNK;
		break;
	case 8:
		rn=rn_dfl;	sx=eq?BGBCC_PSZX_QW:BGBCC_PSZX_SX;
		break;
	case 9:
		rn=rn_dfl;	sx=eq?BGBCC_PSZX_QW:BGBCC_PSZX_ZX;
		break;

	case 0xC:
		break;

#if 0
	case 0: case 1: case 2: case 3:
	case 4: case 5: case 6: case 7:
		if(eq)
		{
			rn=rn_dfl;	sx=BGBCC_PSZX_UNK;
			break;
		}
		break;
	case 0x8: case 0x9:
	case 0xC: case 0xD:
		rn=rn_dfl;	sx=BGBCC_PSZX_SSX;
		if(eq)
			sx=BGBCC_PSZX_SZX;
		break;
	case 0xA: case 0xE:
		rn=rn_dfl;	sx=BGBCC_PSZX_SX;
		if(eq)
			sx=BGBCC_PSZX_ZX;
		break;
	case 0xB:		case 0xF:
		rn=rn_dfl;	sx=BGBCC_PSZX_QW;
		if(eq)
			sx=BGBCC_PSZX_UNK;
		break;
#endif
	default:
		rn=rn_dfl;	sx=BGBCC_PSZX_UNK;
		break;
	}

	if(rn>=0)
	{
		ctx->reg_pszx[rn]=sx;
	}
}

void BGBCC_JX2_UpdatePszxWordF8(BGBCC_JX2_Context *ctx,
	int opw1, int opw2)
{
	int rn_dfl, rm_dfl, ro_dfl, rn_sx, rm_sx, ro_sx;
	int rn, sx, eq;
	
	ro_dfl=(opw1>>0)&31;
	
	eq=(opw1&0x0080)!=0;
	
	ro_sx=ctx->reg_pszx[ro_dfl];

	rn=-1; sx=0;
	switch((opw1>>4)&14)
	{
	case 0:
		rn=ro_dfl;	sx=BGBCC_PSZX_SZX;
		break;
	case 2:
		rn=ro_dfl;	sx=BGBCC_PSZX_SSX;
		break;
//	case 4:
//		rn=ro_dfl;	sx=ro_sx;
//		break;
	default:
		rn=ro_dfl;	sx=BGBCC_PSZX_UNK;
		break;
	}

	if(rn>=0)
	{
		ctx->reg_pszx[rn]=sx;
	}
}

void BGBCC_JX2_UpdatePszxWordFx(BGBCC_JX2_Context *ctx,
	int opw1, int opw2)
{
	int rn, sx;
	
	rn=-1; sx=0;
	
//	if(((opw1>>8)&255)==0xF0)
	if(((opw1>>8)&0xEB)==0xE0)
	{
		BGBCC_JX2_UpdatePszxWordF0(ctx, opw1, opw2);
		return;
	}

//	if(((opw1>>8)&255)==0xF1)
	if(((opw1>>8)&0xEB)==0xE1)
	{
		BGBCC_JX2_UpdatePszxWordF1(ctx, opw1, opw2);
		return;
	}

//	if(((opw1>>8)&255)==0xF2)
	if(((opw1>>8)&0xEB)==0xE2)
	{
		BGBCC_JX2_UpdatePszxWordF2(ctx, opw1, opw2);
		return;
	}

//	if(((opw1>>8)&255)==0xF8)
//	if((((opw1>>8)&0xED)==0xE8) && (((opw1>>8)&255)!=0xFA))
	if((((opw1>>8)&255)==0xF8) || (((opw1>>8)&0xFD)==0xE8))
	{
		BGBCC_JX2_UpdatePszxWordF8(ctx, opw1, opw2);
		return;
	}

	if(((opw1>>8)&255)==0xFA)
	{
		rn=0;
		sx=BGBCC_PSZX_SZX;
	}

	if(((opw1>>8)&255)==0xFB)
	{
		rn=0;
		sx=BGBCC_PSZX_SSX;
	}

	if(rn>=0)
	{
		ctx->reg_pszx[rn]=sx;
	}
}

int BGBCC_JX2_EmitStatWord(BGBCC_JX2_Context *ctx, int val)
{
	int i, j, k;
	
	if(ctx->sec!=BGBCC_SH_CSEG_TEXT)
	{
		ctx->stat_opc_issfx=0;
		return(0);
	}
	
	if(ctx->is_simpass)
	{
		if(ctx->is_fixed32)
		{
			if(ctx->stat_opc_issfx>0)
			{
				ctx->stat_opc_issfx--;
				return(0);
			}

			if((val>>12)==0xF)
			{
				if(((val>>8)&14)==0xC)
					{ BGBCC_DBGBREAK }
				if(((val>>8)&14)==0xE)
					{ BGBCC_DBGBREAK }

				ctx->stat_opc_issfx=1;
				return(0);
			}

			BGBCC_DBGBREAK
		}

		ctx->stat_opc_issfx=0;
		return(0);
	}

	if(ctx->stat_opc_issfx>0)
	{
		if((((ctx->opcnt_opw1>>8)&255)>=0xF0) &&
			(((ctx->opcnt_opw1>>8)&255)<=0xFB))
		{
			BGBCC_JX2_UpdatePszxWordFx(ctx, ctx->opcnt_opw1, val);
		}

		if(((ctx->opcnt_opw1>>8)&255)==0xF0)
		{
//			BGBCC_JX2_UpdatePszxWordF0(ctx, ctx->opcnt_opw1, val);

//			ctx->opcnt_f0xx[(val>>8)&255]++;
			ctx->opcnt_f0xx[((val>>8)&0xF0)|(val&0x000F)]++;
		}

		if(((ctx->opcnt_opw1>>8)&255)==0xF1)
		{
//			BGBCC_JX2_UpdatePszxWordF1(ctx, ctx->opcnt_opw1, val);
//			ctx->opcnt_f0xx[(val>>8)&255]++;

			ctx->opcnt_f1xx[(val>>12)&15]++;
		}

		if(((ctx->opcnt_opw1>>8)&255)==0xF2)
		{
//			BGBCC_JX2_UpdatePszxWordF1(ctx, ctx->opcnt_opw1, val);
//			ctx->opcnt_f0xx[(val>>8)&255]++;

			ctx->opcnt_f2xx[((val>>8)&0xF0)|(ctx->opcnt_opw1&0x000F)]++;
		}

		if(((ctx->opcnt_opw1>>8)&255)==0xF8)
		{
			ctx->opcnt_f8xx[(ctx->opcnt_opw1>>4)&15]++;
		}

		ctx->stat_opc_issfx--;
		return(0);
	}
	
	BGBCC_JX2_UpdatePszxWord(ctx, val);
	
	ctx->opcnt_hi8[(val>>8)&255]++;
	if((val>>12)==3)
	{
//		i=(((val>>8)&15)<<4)|(val&15);
		i=(((val>>8)&7)<<4)|(val&15);
		ctx->opcnt_3xx[i]++;
		ctx->n_opcnt_3xx++;
	}

//	if((val>>12)==0xD)
//	{
//		BGBCC_DBGBREAK
//	}

//	if((val>>8)==0x26)
//	{
//		BGBCC_DBGBREAK
//	}
	
	ctx->stat_opc_tot++;

//	if((val>>12)==0xF)
	if(((val>>12)&0xE)==0xE)
	{
		ctx->opcnt_opw1=val;
	
		if(((val>>8)&15)==0xC)
		{
			if(ctx->is_fixed32)
				{ BGBCC_DBGBREAK }

			ctx->stat_opc_ext8e++;
			ctx->stat_opc_issfx=2;
			return(0);
		}

		if(((val>>8)&15)==0xD)
		{
			if(ctx->is_fixed32)
				{ BGBCC_DBGBREAK }

			ctx->stat_opc_extCe++;
			ctx->stat_opc_issfx=2;
			return(0);
		}

		ctx->stat_opc_ext8a++;
		ctx->stat_opc_issfx=1;
		return(0);

	}
	
	if(ctx->is_fixed32)
		{ BGBCC_DBGBREAK }
	
	ctx->stat_opc_base16++;
	ctx->stat_opc_issfx=0;
	return(0);

}

/** Returns whether a 32-bit instruction would cross a 32B boundary (and require padding as a result). */
int BGBCC_JX2_CheckPadCross32(BGBCC_JX2_Context *ctx)
{
	int i, j, k;

	if(ctx->test_lclalign)
	{
		return(ctx->test_lclalign&1);
//		return(1);
//		return(0);
	}

	if(!ctx->use_padcross)
		return(0);

	i=BGBCC_JX2_EmitGetOffs(ctx);
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
int BGBCC_JX2_CheckPadCross48(BGBCC_JX2_Context *ctx)
{
	int i, j, k;

	if(ctx->test_lclalign)
	{
		return(ctx->test_lclalign&3);
//		return(1);
//		return(0);
	}
	
	if(ctx->no_ops48)
		return(1);
	
	if(!ctx->use_padcross)
		return(0);

	i=BGBCC_JX2_EmitGetOffs(ctx);
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
int BGBCC_JX2_CheckPadAlign32(BGBCC_JX2_Context *ctx)
{
	int i, j, k;

	i=BGBCC_JX2_EmitGetOffs(ctx);
	
	if(!(i&3))
		return(1);
	
//	if(ctx->sec==BGBCC_SH_CSEG_TEXT)
//	{
//		if((i&31)==30)
//			return(1);
//		if((i&31)==28)
//			return(1);
//	}
	return(0);
}

/** Check if we can expand last op to avoid inserting a NOP. */
int BGBCC_JX2_EmitPadCheckExpandLastOp(
	BGBCC_JX2_Context *ctx)
{
	int op0, opw1, opw2;
	int i, j, k;

	i=BGBCC_JX2_EmitGetOffs(ctx);
	j=ctx->pos_pad_op0;
	
	if((i-j)!=2)
		return(0);
	
	op0=BGBCC_JX2_EmitGetOffsWord(ctx, j);
	opw1=-1; opw2=-1;
	switch((op0>>8)&255)
	{
	case 0x10:		opw1=0xF000; opw2=0x1900|(op0&255);	break;
	case 0x11:		opw1=0xF001; opw2=0x1900|(op0&255);	break;
	case 0x12:		opw1=0xF002; opw2=0x1900|(op0&255);	break;
	case 0x13:		opw1=0xF003; opw2=0x1900|(op0&255);	break;
	case 0x14:		opw1=0xF004; opw2=0x1900|(op0&255);	break;
	case 0x15:		opw1=0xF005; opw2=0x1900|(op0&255);	break;
	case 0x16:		opw1=0xF006; opw2=0x1900|(op0&255);	break;
	case 0x17:		opw1=0xF007; opw2=0x1900|(op0&255);	break;

	case 0x18:		opw1=0xF008; opw2=0x1900|(op0&255);	break;
	case 0x19:		opw1=0xF028; opw2=0x1900|(op0&255);	break;
	case 0x1A:		opw1=0xF048; opw2=0x1900|(op0&255);	break;
	case 0x1B:		opw1=0xF068; opw2=0x1900|(op0&255);	break;

	case 0x1C:		opw1=0xF00C; opw2=0x1900|(op0&255);	break;
	case 0x1D:		opw1=0xF00D; opw2=0x1900|(op0&255);	break;
	case 0x1E:		opw1=0xF00E; opw2=0x1900|(op0&255);	break;

	case 0x32:
		switch(op0&15)
		{
		case 0x4:
			opw1=0xF088;
			opw2=0x1C00|(op0&0xF0)|((op0&0xF0)>>4);
			break;
		case 0x5:
			opw1=0xF089;
			opw2=0x1C00|(op0&0xF0)|((op0&0xF0)>>4);
			break;
		case 0x6:
			opw1=0xF008;
			opw2=0x1C00|(op0&0xF0)|((op0&0xF0)>>4);
			break;
		case 0x7:
			opw1=0xF009;
			opw2=0x1C00|(op0&0xF0)|((op0&0xF0)>>4);
			break;
		
		default:
			k=-1;
			break;
		}
		break;

	default:
		k=-1;
		break;
	}

	if((opw1>=0) && (opw2>=0))
	{
		BGBCC_JX2_EmitSetOffsWord(ctx, j, opw1);
		BGBCC_JX2_EmitWordI(ctx, opw2);
		return(1);
	}

	return(0);
}

/* If the instruction would cross a 32B boundary, pad-align. */
int BGBCC_JX2_EmitPadForOpWord(BGBCC_JX2_Context *ctx, int val)
{
	static int rec=0;
	int i, j, k;

	if(ctx->emit_isprobe)
		return(0);

	if(!ctx->use_padcross)
	{
		if(ctx->sec==BGBCC_SH_CSEG_TEXT)
		{
			i=BGBCC_JX2_EmitGetOffs(ctx);
			ctx->pos_pad_op3=ctx->pos_pad_op2;
			ctx->pos_pad_op2=ctx->pos_pad_op1;
			ctx->pos_pad_op1=ctx->pos_pad_op0;
			ctx->pos_pad_op0=i;
		}

		return(0);
	}

	if(rec)
		return(0);
	rec++;

	i=BGBCC_JX2_EmitGetOffs(ctx);
//	if(!(i&(al-1)))
//		return(0);
//	j=al-(i&(al-1));

	if(ctx->sec==BGBCC_SH_CSEG_TEXT)
	{
		if((val&0xFC00)==0xFC00)
		{
			if((i&31)==30)
			{
				if(!BGBCC_JX2_EmitPadCheckExpandLastOp(ctx))
					BGBCC_JX2_EmitWordI(ctx, 0x3000);
			}

			if((i&31)==28)
			{
				BGBCC_JX2_EmitWordI(ctx, 0xF000);
				BGBCC_JX2_EmitWordI(ctx, 0x1600);
			}
		}else if((val&0xF000)==0xF000)
		{
			if((i&31)==30)
			{
				if(!BGBCC_JX2_EmitPadCheckExpandLastOp(ctx))
					BGBCC_JX2_EmitWordI(ctx, 0x3000);
			}
		}
		
		i=BGBCC_JX2_EmitGetOffs(ctx);
		ctx->pos_pad_op3=ctx->pos_pad_op2;
		ctx->pos_pad_op2=ctx->pos_pad_op1;
		ctx->pos_pad_op1=ctx->pos_pad_op0;
		ctx->pos_pad_op0=i;
	}
	
	rec--;
	return(0);
}

#if 0
int BGBCC_JX2_EmitCheckRepackOp(BGBCC_JX2_Context *ctx)
{
	int opw1, opw2, opw3, opw4, o, osz;
	int i, j, k;

	if(ctx->sec!=BGBCC_SH_CSEG_TEXT)
		return(0);

//	return(0);

	i=BGBCC_JX2_EmitGetOffs(ctx);
	o=ctx->pos_pad_op0;
	osz=i-o;
	
	if((osz<4) || (osz>6))
		return(0);
	
//	if((i-o)!=4)
//		return(0);
	
	opw1=BGBCC_JX2_EmitGetOffsWord(ctx, o+0);
	opw2=BGBCC_JX2_EmitGetOffsWord(ctx, o+2);

	if(((opw1&0xDB00)==0xD000) && (osz==4))
	{
		if(((opw2&0xF000)>=0xC000))
			return(0);
		opw3=(opw1&0xFF00)|
			(opw2&0x00FF);
		opw4=(opw2&0xF000)|
			((opw2>>8)&0x000F)|
			((opw1<<4)&0x0FF0);
		
		BGBCC_JX2_EmitSetOffsWord(ctx, o+0, opw3);
		BGBCC_JX2_EmitSetOffsWord(ctx, o+2, opw4);
		return(1);
	}

//	BGBCC_JX2_EmitSetOffsWord(ctx, o+0, opw1);
//	BGBCC_JX2_EmitSetOffsWord(ctx, o+2, opw2);
	return(0);
}
#endif

int BGBCC_JX2_EmitWordI(BGBCC_JX2_Context *ctx, int val)
{
	int i, j, k;

	if(ctx->emit_isprobe)
		return(0);

//	if(!(val&0xFFFF) && (ctx->sec==BGBCC_SH_CSEG_TEXT))
//	{
//		BGBCC_DBGBREAK
//	}

//	if((ctx->sec==BGBCC_SH_CSEG_TEXT) && !ctx->stat_opc_issfx)
//	{
//		BGBCC_JX2_EmitPadForOpWord(ctx, val);
//		if(!val)
//			{ BGBCC_DBGBREAK }
//	}

	if(ctx->sec==BGBCC_SH_CSEG_TEXT)
	{
		if(!ctx->stat_opc_issfx)
		{
			if(ctx->op_is_wex2&4)
			{
#if 1
				if((val&0xFC00)==0xF000)
				{
//					val&=0xDFFF;
					val&=0xEFFF;
					if((ctx->op_is_wex2)&1)
						val|=0x0400;
				}else
					if((val&0xFE00)==0xF800)
				{
//					val&=0xDFFF;
					val&=0xEFFF;
					if((ctx->op_is_wex2)&1)
						val|=0x0200;
				}else
					if((val&0xFE00)==0xFC00)
				{
//					val&=0xDFFF;
					val&=0xEFFF;
					if((ctx->op_is_wex2)&1)
						val|=0x0200;
				}
				else
				{
					BGBCC_DBGBREAK
				}
#endif
			}else
				if((ctx->op_is_wex2)&1)
			{
	//			if((val&0xFF00)==0xF000)
				if((val&0xFC00)==0xF000)
					val|=0x0400;
	//			if((val&0xFF00)==0xF200)
					val|=0x0400;
			}
		}
	}

	BGBCC_JX2_EmitStatWord(ctx, val);

//	if(!val && (ctx->sec==BGBCC_SH_CSEG_TEXT))
//	{
//		k=-1;
//	}

	BGBCC_JX2_EmitCheckExpand(ctx, 2);

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

//		BGBCC_JX2_EmitCheckRepackOp(ctx);
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

//		BGBCC_JX2_EmitCheckRepackOp(ctx);
		return(0);
	}
	
//	BGBCC_JX2_EmitCheckRepackOp(ctx);

#if 0
	if(ctx->is_le)
	{
		BGBCC_JX2_EmitByteI(ctx, val   );
		BGBCC_JX2_EmitByteI(ctx, val>>8);
		return(0);
	}else
	{
		BGBCC_JX2_EmitByteI(ctx, val>>8);
		BGBCC_JX2_EmitByteI(ctx, val   );
		return(0);
	}
#endif
}

int BGBCC_JX2_EmitDWordI(BGBCC_JX2_Context *ctx, u32 val)
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
		BGBCC_JX2_EmitByteI(ctx, val    );
		BGBCC_JX2_EmitByteI(ctx, val>> 8);
		BGBCC_JX2_EmitByteI(ctx, val>>16);
		BGBCC_JX2_EmitByteI(ctx, val>>24);
		return(0);
	}else
	{
		BGBCC_JX2_EmitByteI(ctx, val>>24);
		BGBCC_JX2_EmitByteI(ctx, val>>16);
		BGBCC_JX2_EmitByteI(ctx, val>> 8);
		BGBCC_JX2_EmitByteI(ctx, val    );
		return(0);
	}
}

int BGBCC_JX2_EmitQWordI(BGBCC_JX2_Context *ctx, s64 val)
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
		BGBCC_JX2_EmitByteI(ctx, (val    )&255);
		BGBCC_JX2_EmitByteI(ctx, (val>> 8)&255);
		BGBCC_JX2_EmitByteI(ctx, (val>>16)&255);
		BGBCC_JX2_EmitByteI(ctx, (val>>24)&255);
		BGBCC_JX2_EmitByteI(ctx, (val>>32)&255);
		BGBCC_JX2_EmitByteI(ctx, (val>>40)&255);
		BGBCC_JX2_EmitByteI(ctx, (val>>48)&255);
		BGBCC_JX2_EmitByteI(ctx, (val>>56)&255);
		return(0);
	}else
	{
		BGBCC_JX2_EmitByteI(ctx, (val>>56)&255);
		BGBCC_JX2_EmitByteI(ctx, (val>>48)&255);
		BGBCC_JX2_EmitByteI(ctx, (val>>40)&255);
		BGBCC_JX2_EmitByteI(ctx, (val>>32)&255);
		BGBCC_JX2_EmitByteI(ctx, (val>>24)&255);
		BGBCC_JX2_EmitByteI(ctx, (val>>16)&255);
		BGBCC_JX2_EmitByteI(ctx, (val>> 8)&255);
		BGBCC_JX2_EmitByteI(ctx, (val    )&255);
		return(0);
	}
}

int BGBCC_JX2_EmitString(BGBCC_JX2_Context *ctx, char *str)
{
	byte *s;
	
	s=(byte *)str;
	while(*s)
		BGBCC_JX2_EmitByteI(ctx, *s++);
	BGBCC_JX2_EmitByteI(ctx, *s++);

	BGBCC_JX2DA_EmitString(ctx, str);

	return(0);
}

int BGBCC_JX2_EmitAscii(BGBCC_JX2_Context *ctx, char *str)
{
	byte *s;
	
	s=(byte *)str;
	while(*s)
		BGBCC_JX2_EmitByteI(ctx, *s++);

	BGBCC_JX2DA_EmitAscii(ctx, str);

	return(0);
}

int BGBCC_JX2_EmitStringUCS2(BGBCC_JX2_Context *ctx, u16 *str)
{
	return(BGBCC_JX2_EmitStringUCS2B(ctx, str, 0));
}

int BGBCC_JX2_EmitStringUCS2B(BGBCC_JX2_Context *ctx, u16 *str, int noz)
{
	u16 *s;
	
	s=str;
	while(*s)
		BGBCC_JX2_EmitWordI(ctx, *s++);
	if(!noz)
		BGBCC_JX2_EmitWordI(ctx, *s++);

	BGBCC_JX2DA_EmitStringUCS2(ctx, str, noz);
	return(0);
}

int BGBCC_JX2_EmitGetStrtabLabel(BGBCC_JX2_Context *ctx, char *str)
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

#if 0
	for(i=0; i<ctx->nlbl; i++)
	{
		if(ctx->lbl_sec[i]==BGBCC_SH_CSEG_STRTAB)
		{
			s0=(char *)(ctx->sec_buf[BGBCC_SH_CSEG_STRTAB]+ctx->lbl_ofs[i]);
			if(!strcmp(s0, str))
			{
				return(ctx->lbl_id[i]);
			}
		}
	}
#endif

	if(ctx->is_simpass)
	{
		/* Keep symbol labels aligned */
		ctx->nvlbl++;

#if 0
		sn0=ctx->sec;
		k=BGBCC_JX2_GenLabel(ctx);
		BGBCC_JX2_SetSectionName(ctx, ".strtab");
		i=BGBCC_JX2_EmitLabel(ctx, k);
		BGBCC_JX2_EmitString(ctx, str);
		ctx->sec=sn0;
#endif

		return(0);
	}

	sn0=ctx->sec;
	k=BGBCC_JX2_GenLabel(ctx);
	BGBCC_JX2_SetSectionName(ctx, ".strtab");
	i=BGBCC_JX2_EmitLabel(ctx, k);
	BGBCC_JX2_EmitString(ctx, str);

//	BGBCC_JX2DA_EmitLabel(ctx, k);
//	BGBCC_JX2DA_EmitString(ctx, str);

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
//	default:		k=0x8F; break;
	default:		k=-1; break;
	}
	
	return(k);
}

int BGBCC_JX2_EmitGetStrtabLabelUTF2ASCII(
	BGBCC_JX2_Context *ctx, char *str)
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
	
	i=BGBCC_JX2_EmitGetStrtabLabel(ctx, tb);
	return(i);
}

int BGBCC_JX2_EmitGetStrtabLabelUCS2(BGBCC_JX2_Context *ctx, u16 *str)
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
	k=BGBCC_JX2_GenLabel(ctx);
	BGBCC_JX2_SetSectionName(ctx, ".strtab");
	BGBCC_JX2_EmitBAlign(ctx, 2);
	BGBCC_JX2_EmitLabel(ctx, k);
	BGBCC_JX2_EmitStringUCS2(ctx, str);
//	BGBCC_JX2DA_EmitStringUCS2(ctx, str);
	ctx->sec=sn0;
	return(k);
}

int BGBCC_JX2_EmitGetStrtabLabelUTF2UCS2(
	BGBCC_JX2_Context *ctx, char *str)
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
	
	i=BGBCC_JX2_EmitGetStrtabLabelUCS2(ctx, tb);
	return(i);
}

int BGBCC_JX2_EmitStringUTF2UCS2(
	BGBCC_JX2_Context *ctx, char *str)
{
	return(BGBCC_JX2_EmitStringUTF2UCS2I(ctx, str, 0));
}

int BGBCC_JX2_EmitAsciiUTF2UCS2(
	BGBCC_JX2_Context *ctx, char *str)
{
	return(BGBCC_JX2_EmitStringUTF2UCS2I(ctx, str, 1));
}

int BGBCC_JX2_EmitStringUTF2UCS2I(
	BGBCC_JX2_Context *ctx, char *str, int noz)
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
	
	i=BGBCC_JX2_EmitStringUCS2B(ctx, tb, noz);
	return(i);
}


int BGBCC_JX2_EmitGetStrtabSecOfs(BGBCC_JX2_Context *ctx, char *str)
{
	int l0;
	int i;

	l0=BGBCC_JX2_EmitGetStrtabLabel(ctx, str);
	
	for(i=0; i<ctx->nlbl; i++)
		if(ctx->lbl_id[i]==l0)
			return(ctx->lbl_ofs[i]);
	return(-1);
}

int BGBCC_JX2_EmitRawBytes(BGBCC_JX2_Context *ctx, byte *buf, int sz)
{
	byte *s;
	int n;
	
	if(buf)
	{
		s=buf; n=sz;
		while(n--)
			BGBCC_JX2_EmitByteI(ctx, *s++);
	}else
	{
		if((ctx->sec!=BGBCC_SH_CSEG_BSS) &&
				(ctx->sec!=BGBCC_SH_CSEG_DATA) &&
				(ctx->sec!=BGBCC_SH_CSEG_RELOC))
			{ n=-1; }
	
		n=sz;
		while(n--)
			BGBCC_JX2_EmitByteI(ctx, 0);
	}
	return(0);
}

byte *BGBCC_JX2_EmitGetPos(BGBCC_JX2_Context *ctx)
{
	if(ctx->is_simpass)
	{
		return(ctx->sec_vpos[ctx->sec]);
	}

	return(ctx->sec_pos[ctx->sec]);
}

int BGBCC_JX2_EmitGetOffs(BGBCC_JX2_Context *ctx)
{
	if(ctx->is_simpass)
	{
		return(ctx->sec_vpos[ctx->sec]-ctx->sec_buf[ctx->sec]);
	}

	return(ctx->sec_pos[ctx->sec]-ctx->sec_buf[ctx->sec]);
}

int BGBCC_JX2_EmitBAlign(BGBCC_JX2_Context *ctx, int al)
{
	int i, j, k;
	
	i=BGBCC_JX2_EmitGetOffs(ctx);
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
				{ BGBCC_JX2_EmitByteI(ctx, 0); j--; }
			if(j&2)
			{
				BGBCC_JX2_EmitByteI(ctx, 0x30);
				BGBCC_JX2_EmitByteI(ctx, 0x30);
				j-=2;
			}

			while(j>0)
			{
				BGBCC_JX2_EmitWordI(ctx, 0xF003);
				BGBCC_JX2_EmitWordI(ctx, 0x3000);
				j-=4;
			}
		}else
		{
			if(j&1)
				{ BGBCC_JX2_EmitByteI(ctx, 0); j--; }
			while(j>0)
			{
	//			BGBCC_JX2_EmitWordI(ctx, 0x3000);
				BGBCC_JX2_EmitWordI(ctx, 0x3030);
				j-=2;
			}
	//			{ BGBCC_JX2_EmitWordI(ctx, 0x003B); j-=2; }
		}
	}else
	{
		while(j--)
			BGBCC_JX2_EmitByteI(ctx, 0);
	}

//	while(j--)
//		BGBCC_JX2_EmitByteI(ctx, 0);
	return(1);
}

byte *BGBCC_JX2_EmitGetPosForOffs(BGBCC_JX2_Context *ctx, int ofs)
{
	return(ctx->sec_buf[ctx->sec]+ofs);
}

int BGBCC_JX2_EmitGetOffsWord(BGBCC_JX2_Context *ctx, int ofs)
{
	byte *ptr;
	ptr=BGBCC_JX2_EmitGetPosForOffs(ctx, ofs);
	if(ctx->is_le)
	{
		return(ptr[0]|(ptr[1]<<8));
	}else
	{
		return((ptr[0]<<8)|ptr[1]);
	}
}

int BGBCC_JX2_EmitGetOffsDWord(BGBCC_JX2_Context *ctx, int ofs)
{
	byte *ptr;
	ptr=BGBCC_JX2_EmitGetPosForOffs(ctx, ofs);
	if(ctx->is_le)
	{
		return(ptr[0]|(ptr[1]<<8)|(ptr[2]<<16)|(ptr[3]<<24));
	}else
	{
		return((ptr[0]<<24)|(ptr[1]<<16)|(ptr[2]<<8)|ptr[3]);
	}
}

int BGBCC_JX2_EmitSetOffsWord(BGBCC_JX2_Context *ctx, int ofs, int val)
{
	byte *ptr;

//	if(!(val&0xFFFF) && (ctx->sec==BGBCC_SH_CSEG_TEXT))
//	{
//		BGBCC_DBGBREAK
//	}

	if(ctx->is_simpass)
		return(0);

	ptr=BGBCC_JX2_EmitGetPosForOffs(ctx, ofs);
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

int BGBCC_JX2_EmitSetOffsDWord(BGBCC_JX2_Context *ctx, int ofs, int val)
{
	byte *ptr;

	if(ctx->is_simpass)
		return(0);

	ptr=BGBCC_JX2_EmitGetPosForOffs(ctx, ofs);
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

int BGBCC_JX2_EmitByte(BGBCC_JX2_Context *ctx, int val)
{
	BGBCC_JX2_EmitByteI(ctx, val);

	if(ctx->sec!=BGBCC_SH_CSEG_TEXT)
	{
		BGBCC_JX2DA_EmitByte(ctx, val);
	}

	return(0);
}

int BGBCC_JX2_EmitWord(BGBCC_JX2_Context *ctx, int val)
{
	BGBCC_JX2_EmitWordI(ctx, val);

	if(ctx->sec!=BGBCC_SH_CSEG_TEXT)
	{
		BGBCC_JX2DA_EmitWord(ctx, val);
	}

	return(0);
}

int BGBCC_JX2_EmitDWord(BGBCC_JX2_Context *ctx, u32 val)
{
	BGBCC_JX2_EmitDWordI(ctx, val);
	BGBCC_JX2DA_EmitDWord(ctx, val);
	return(0);
}

int BGBCC_JX2_EmitQWord(BGBCC_JX2_Context *ctx, s64 val)
{
	BGBCC_JX2_EmitQWordI(ctx, val);
	BGBCC_JX2DA_EmitQWord(ctx, val);
	return(0);
}

int BGBCC_JX2_EmitDWordAbs32(BGBCC_JX2_Context *ctx, int lbl)
{
	if((lbl&CCXL_LBL_SPMASK)==CCXL_LBL_ABS16UP)
	{
		BGBCC_JX2_EmitDWordI(ctx, ((u16)lbl)*4);
		return(0);
	}
	if((lbl&CCXL_LBL_SPMASK)==CCXL_LBL_ABS16NP)
	{
//		BGBCC_JX2_EmitDWordI(ctx, ((-1<<16)|((u16)lbl))*4);
		BGBCC_JX2_EmitDWordI(ctx, ((-(1<<16))|((u16)lbl))*4);
		return(0);
	}
	if((lbl&CCXL_LBL_SPMASK)==CCXL_LBL_ABS16S)
	{
		BGBCC_JX2_EmitDWordI(ctx, ((s16)lbl));
		return(0);
	}

	BGBCC_JX2_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_ABS32);
	BGBCC_JX2_EmitDWordI(ctx, 0);
	BGBCC_JX2DA_EmitDWordAbs(ctx, lbl);
	return(0);
}

int BGBCC_JX2_EmitQWordAbs64(BGBCC_JX2_Context *ctx, int lbl)
{
	if((lbl&CCXL_LBL_SPMASK)==CCXL_LBL_ABS16UP)
	{
		BGBCC_JX2_EmitQWordI(ctx, ((u16)lbl)*8);
		return(0);
	}
	if((lbl&CCXL_LBL_SPMASK)==CCXL_LBL_ABS16NP)
	{
//		BGBCC_JX2_EmitQWordI(ctx, ((-1<<16)|((u16)lbl))*8);
		BGBCC_JX2_EmitQWordI(ctx, ((-(1<<16))|((u16)lbl))*8);
		return(0);
	}
	if((lbl&CCXL_LBL_SPMASK)==CCXL_LBL_ABS16S)
	{
		BGBCC_JX2_EmitQWordI(ctx, ((s16)lbl));
		return(0);
	}

	BGBCC_JX2_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_ABS64);
	BGBCC_JX2_EmitQWordI(ctx, 0);
	BGBCC_JX2DA_EmitQWordAbs(ctx, lbl);
	return(0);
}

int BGBCC_JX2_EmitPtrWordAbs(BGBCC_JX2_Context *ctx, int lbl)
{
	if(ctx->is_addr64 && !ctx->is_addr_x32)
		{ BGBCC_JX2_EmitQWordAbs64(ctx, lbl); }
	else
		{ BGBCC_JX2_EmitDWordAbs32(ctx, lbl); }
	return(0);
}


int BGBCC_JX2_EmitDWordAbs32Disp(BGBCC_JX2_Context *ctx, int lbl, int disp)
{
	if(!disp)
	{
		BGBCC_JX2_EmitDWordAbs32(ctx, lbl);
		return(0);
	}

	BGBCC_JX2_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_ABS32);
	BGBCC_JX2_EmitDWordI(ctx, disp);
	BGBCC_JX2DA_EmitDWordAbsDisp(ctx, lbl, disp);
	return(0);
}

int BGBCC_JX2_EmitQWordAbs64Disp(BGBCC_JX2_Context *ctx, int lbl, int disp)
{
	if(!disp)
	{
		BGBCC_JX2_EmitQWordAbs64(ctx, lbl);
		return(0);
	}

	BGBCC_JX2_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_ABS64);
	BGBCC_JX2_EmitQWordI(ctx, disp);
	BGBCC_JX2DA_EmitQWordAbsDisp(ctx, lbl, disp);
	return(0);
}


int BGBCC_JX2_EmitDWordRva32(BGBCC_JX2_Context *ctx, int lbl)
{
	return(BGBCC_JX2_EmitDWordRva32Disp(ctx, lbl, 0));
}

int BGBCC_JX2_EmitDWordRva32Disp(BGBCC_JX2_Context *ctx, int lbl, int disp)
{
	if((lbl&CCXL_LBL_SPMASK)==CCXL_LBL_ABS16UP)
	{
		BGBCC_JX2_EmitDWordI(ctx, disp+((u16)lbl)*4);
		return(0);
	}
	if((lbl&CCXL_LBL_SPMASK)==CCXL_LBL_ABS16NP)
	{
//		BGBCC_JX2_EmitDWordI(ctx, disp+((-1<<16)|((u16)lbl))*4);
		BGBCC_JX2_EmitDWordI(ctx, disp+((-(1<<16))|((u16)lbl))*4);
		return(0);
	}
	if((lbl&CCXL_LBL_SPMASK)==CCXL_LBL_ABS16S)
	{
		BGBCC_JX2_EmitDWordI(ctx, disp+((s16)lbl));
		return(0);
	}

	BGBCC_JX2_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_RVA32);
	BGBCC_JX2_EmitDWordI(ctx, disp);
	BGBCC_JX2DA_EmitDWordAbs(ctx, lbl);
	return(0);
}

