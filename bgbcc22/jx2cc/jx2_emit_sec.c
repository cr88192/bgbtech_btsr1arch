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

int BGBCC_JX2_EmitByte(BGBCC_JX2_Context *ctx, int val)
{
	byte *buf;
	int sz, ofs, vofs;

	if(ctx->emit_isprobe)
		return(0);

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
	if(((ctx->sec_pos[ctx->sec]+1)>=ctx->sec_end[ctx->sec]) ||
		((ctx->sec_vpos[ctx->sec]+1)>=ctx->sec_end[ctx->sec]))
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

	if(ctx->is_simpass)
	{
		*ctx->sec_vpos[ctx->sec]++=val;
		return(0);
	}

	*ctx->sec_pos[ctx->sec]++=val;
	return(0);
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
		if(((ctx->opcnt_opw1>>8)&255)==0xF0)
		{
			ctx->opcnt_f0xx[(val>>8)&255]++;
		}

		ctx->stat_opc_issfx--;
		return(0);
	}
	
	ctx->opcnt_hi8[(val>>8)&255]++;
	if((val>>12)==3)
	{
		i=(((val>>8)&15)<<4)|(val&15);
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

	if((val>>12)==0xF)
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
		BGBCC_JX2_EmitWord(ctx, opw2);
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
					BGBCC_JX2_EmitWord(ctx, 0x3000);
			}

			if((i&31)==28)
			{
				BGBCC_JX2_EmitWord(ctx, 0xF000);
				BGBCC_JX2_EmitWord(ctx, 0x1600);
			}
		}else if((val&0xF000)==0xF000)
		{
			if((i&31)==30)
			{
				if(!BGBCC_JX2_EmitPadCheckExpandLastOp(ctx))
					BGBCC_JX2_EmitWord(ctx, 0x3000);
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


int BGBCC_JX2_EmitWord(BGBCC_JX2_Context *ctx, int val)
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

	BGBCC_JX2_EmitStatWord(ctx, val);

//	if(!val && (ctx->sec==BGBCC_SH_CSEG_TEXT))
//	{
//		k=-1;
//	}

	if(ctx->is_le)
	{
		BGBCC_JX2_EmitByte(ctx, val   );
		BGBCC_JX2_EmitByte(ctx, val>>8);
		return(0);
	}else
	{
		BGBCC_JX2_EmitByte(ctx, val>>8);
		BGBCC_JX2_EmitByte(ctx, val   );
		return(0);
	}
}

int BGBCC_JX2_EmitDWord(BGBCC_JX2_Context *ctx, u32 val)
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
		BGBCC_JX2_EmitByte(ctx, val    );
		BGBCC_JX2_EmitByte(ctx, val>> 8);
		BGBCC_JX2_EmitByte(ctx, val>>16);
		BGBCC_JX2_EmitByte(ctx, val>>24);
		return(0);
	}else
	{
		BGBCC_JX2_EmitByte(ctx, val>>24);
		BGBCC_JX2_EmitByte(ctx, val>>16);
		BGBCC_JX2_EmitByte(ctx, val>> 8);
		BGBCC_JX2_EmitByte(ctx, val    );
		return(0);
	}
}

int BGBCC_JX2_EmitQWord(BGBCC_JX2_Context *ctx, s64 val)
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
		BGBCC_JX2_EmitByte(ctx, (val    )&255);
		BGBCC_JX2_EmitByte(ctx, (val>> 8)&255);
		BGBCC_JX2_EmitByte(ctx, (val>>16)&255);
		BGBCC_JX2_EmitByte(ctx, (val>>24)&255);
		BGBCC_JX2_EmitByte(ctx, (val>>32)&255);
		BGBCC_JX2_EmitByte(ctx, (val>>40)&255);
		BGBCC_JX2_EmitByte(ctx, (val>>48)&255);
		BGBCC_JX2_EmitByte(ctx, (val>>56)&255);
		return(0);
	}else
	{
		BGBCC_JX2_EmitByte(ctx, (val>>56)&255);
		BGBCC_JX2_EmitByte(ctx, (val>>48)&255);
		BGBCC_JX2_EmitByte(ctx, (val>>40)&255);
		BGBCC_JX2_EmitByte(ctx, (val>>32)&255);
		BGBCC_JX2_EmitByte(ctx, (val>>24)&255);
		BGBCC_JX2_EmitByte(ctx, (val>>16)&255);
		BGBCC_JX2_EmitByte(ctx, (val>> 8)&255);
		BGBCC_JX2_EmitByte(ctx, (val    )&255);
		return(0);
	}
}

int BGBCC_JX2_EmitString(BGBCC_JX2_Context *ctx, char *str)
{
	byte *s;
	
	s=str;
	while(*s)
		BGBCC_JX2_EmitByte(ctx, *s++);
	BGBCC_JX2_EmitByte(ctx, *s++);
	return(0);
}

int BGBCC_JX2_EmitAscii(BGBCC_JX2_Context *ctx, char *str)
{
	byte *s;
	
	s=(byte *)str;
	while(*s)
		BGBCC_JX2_EmitByte(ctx, *s++);
	return(0);
}

int BGBCC_JX2_EmitStringUCS2(BGBCC_JX2_Context *ctx, u16 *str)
{
	u16 *s;
	
	s=str;
	while(*s)
		BGBCC_JX2_EmitWord(ctx, *s++);
	BGBCC_JX2_EmitWord(ctx, *s++);
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
	ctx->sec=sn0;
	
	j=ctx->nlblstr++;
	ctx->lblstr_ofs[j]=ctx->lbl_ofs[i];
	ctx->lblstr_id[j]=k;
	
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
		k=BGBCP_ParseChar(&s);
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
		*t++=k;
	}
	*t++=0;
	
	i=BGBCC_JX2_EmitGetStrtabLabelUCS2(ctx, tb);
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
			BGBCC_JX2_EmitByte(ctx, *s++);
	}else
	{
		if((ctx->sec!=BGBCC_SH_CSEG_BSS) &&
				(ctx->sec!=BGBCC_SH_CSEG_DATA) &&
				(ctx->sec!=BGBCC_SH_CSEG_RELOC))
			{ n=-1; }
	
		n=sz;
		while(n--)
			BGBCC_JX2_EmitByte(ctx, 0);
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

	if(ctx->sec==BGBCC_SH_CSEG_TEXT)
	{
		if(ctx->is_fixed32)
		{
			if(j&1)
				{ BGBCC_JX2_EmitByte(ctx, 0); j--; }
			if(j&2)
			{
				BGBCC_JX2_EmitByte(ctx, 0x30);
				BGBCC_JX2_EmitByte(ctx, 0x30);
				j-=2;
			}

			while(j>0)
			{
				BGBCC_JX2_EmitWord(ctx, 0xF003);
				BGBCC_JX2_EmitWord(ctx, 0x3000);
				j-=4;
			}
		}else
		{
			if(j&1)
				{ BGBCC_JX2_EmitByte(ctx, 0); j--; }
			while(j>0)
			{
	//			BGBCC_JX2_EmitWord(ctx, 0x3000);
				BGBCC_JX2_EmitWord(ctx, 0x3030);
				j-=2;
			}
	//			{ BGBCC_JX2_EmitWord(ctx, 0x003B); j-=2; }
		}
	}else
	{
		while(j--)
			BGBCC_JX2_EmitByte(ctx, 0);
	}

//	while(j--)
//		BGBCC_JX2_EmitByte(ctx, 0);
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
