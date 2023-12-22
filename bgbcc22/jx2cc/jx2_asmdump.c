/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

int BGBCC_JX2DA_EmitPuts(BGBCC_JX2_Context *ctx, char *str)
{
	int len, bsz, bsz1, bpos;

	if(!ctx->asm_buf[ctx->sec])
	{
//		bsz=65536;
		bsz=262144;
		ctx->asm_buf[ctx->sec]=malloc(bsz);
		ctx->asm_end[ctx->sec]=ctx->asm_buf[ctx->sec]+bsz;
		ctx->asm_pos[ctx->sec]=ctx->asm_buf[ctx->sec];
	}
	
	len=strlen(str);
	if((ctx->asm_pos[ctx->sec]+len+16)>=ctx->asm_end[ctx->sec])
	{
		bsz=ctx->asm_end[ctx->sec]-ctx->asm_buf[ctx->sec];
		bpos=ctx->asm_pos[ctx->sec]-ctx->asm_buf[ctx->sec];
		
		bsz1=bsz+(bsz>>1);
		ctx->asm_buf[ctx->sec]=realloc(ctx->asm_buf[ctx->sec], bsz1);
		ctx->asm_end[ctx->sec]=ctx->asm_buf[ctx->sec]+bsz1;
		ctx->asm_pos[ctx->sec]=ctx->asm_buf[ctx->sec]+bpos;
	}
	
	strcpy((char *)(ctx->asm_pos[ctx->sec]), str);
	ctx->asm_pos[ctx->sec]+=len;
	return(0);
}

int BGBCC_JX2DA_EmitPrintf(BGBCC_JX2_Context *ctx, char *str, ...)
{
	char tbuf[1024];
	va_list lst;
	
	va_start(lst, str);
	vsprintf(tbuf, str, lst);
	va_end(lst);
	
	BGBCC_JX2DA_EmitPuts(ctx, tbuf);
	return(0);
}

char *BGBCC_JX2DA_RelocToName(BGBCC_JX2_Context *ctx, int rlc)
{
	char tb[128];
	char *sn;

	switch(rlc)
	{
	case BGBCC_SH_RLC_REL8:				sn="REL8";			break;
	case BGBCC_SH_RLC_REL16:			sn="REL16";			break;
	case BGBCC_SH_RLC_REL32:			sn="REL32";			break;
	case BGBCC_SH_RLC_ABS8:				sn="ABS8";			break;
	case BGBCC_SH_RLC_ABS16:			sn="ABS16";			break;
	case BGBCC_SH_RLC_ABS32:			sn="ABS32";			break;
	case BGBCC_SH_RLC_ABS64:			sn="ABS64";			break;
	case BGBCC_SH_RLC_RELW12:			sn="RELW12";		break;
	case BGBCC_SH_RLC_RELW8:			sn="RELW8";			break;
	case BGBCC_SH_RLC_REL32B:			sn="REL32B";		break;
	case BGBCC_SH_RLC_RELW12L:			sn="RELW12L";		break;
	case BGBCC_SH_RLC_RELW8L:			sn="RELW8L";		break;
	case BGBCC_SH_RLC_REL20_2A:			sn="REL20_2A";		break;
	case BGBCC_SH_RLC_RELW16_BJX:		sn="RELW16_BJX";	break;
	case BGBCC_SH_RLC_RELW20_BJX:		sn="RELW20_BJX";	break;
	case BGBCC_SH_RLC_REL24_BJX:		sn="REL24_BJX";		break;
	case BGBCC_SH_RLC_REL24B_BJX:		sn="REL24B_BJX";	break;
	case BGBCC_SH_RLC_RVA32:			sn="RVA32";			break;
	case BGBCC_SH_RLC_RELW24_BJX:		sn="RELW24_BJX";	break;
	case BGBCC_SH_RLC_PBO24_BJX:		sn="PBO24_BJX";		break;
	case BGBCC_SH_RLC_PBO32_BJX:		sn="PBO32_BJX";		break;
	case BGBCC_SH_RLC_TBR24_BJX:		sn="TBR24_BJX";		break;
	case BGBCC_SH_RLC_TBR12_BJX:		sn="TBR12_BJX";		break;
	case BGBCC_SH_RLC_RELW16C_BSR:		sn="RELW16C_BJX";	break;
	case BGBCC_SH_RLC_RELW16_BSR:		sn="RELW16_BSR";	break;
	case BGBCC_SH_RLC_RELW24C_BSR:		sn="RELW24C_BSR";	break;
	case BGBCC_SH_RLC_RELW24_BSR:		sn="RELW24_BSR";	break;
	case BGBCC_SH_RLC_ABSW16A_BSR:		sn="ABSW16A_BSR";	break;
	case BGBCC_SH_RLC_ABSW24A_BSR:		sn="ABSW24A_BSR";	break;
	case BGBCC_SH_RLC_ABSW32A_BSR:		sn="ABSW32A_BSR";	break;
	case BGBCC_SH_RLC_RELW8_BSR:		sn="RELW8_BSR";		break;
	case BGBCC_SH_RLC_RELW16D_BSR:		sn="RELW16D_BSR";	break;
	case BGBCC_SH_RLC_RELW24D_BSR:		sn="RELW24D_BSR";	break;
	case BGBCC_SH_RLC_RELW32D_BSR:		sn="RELW32D_BSR";	break;
	case BGBCC_SH_RLC_RELW32C_BSR:		sn="RELW32C_BSR";	break;
	case BGBCC_SH_RLC_ABSW16B_BSR:		sn="RELW16B_BSR";	break;
	case BGBCC_SH_RLC_ABSW24B_BSR:		sn="RELW24B_BSR";	break;
	case BGBCC_SH_RLC_ABSW32B_BSR:		sn="RELW32B_BSR";	break;
	case BGBCC_SH_RLC_RELW12_BSR:		sn="RELW12_BSR";	break;
	case BGBCC_SH_RLC_RELW16A_BSR:		sn="RELW16A_BSR";	break;
	case BGBCC_SH_RLC_RELW24A_BSR:		sn="RELW24A_BSR";	break;
	case BGBCC_SH_RLC_RELW32A_BSR:		sn="RELW32A_BSR";	break;
	case BGBCC_SH_RLC_PBO9_BJX:			sn="PBO9_BJX";		break;
	case BGBCC_SH_RLC_PBOJ29_BJX:		sn="PBO29_BJX";		break;
	case BGBCC_SH_RLC_PBOJ32_BJX:		sn="PBOJ32_BJX";	break;
	case BGBCC_SH_RLC_RELJ29_BJX:		sn="RELJ29_BJX";	break;
	case BGBCC_SH_RLC_RELJ32_BJX:		sn="RELJ32_BJX";	break;
	case BGBCC_SH_RLC_TRIPWIRE_BJX:		sn="TRIPWIRE_BJX";	break;
	case BGBCC_SH_RLC_RELB13_OP24:		sn="RELB13_OP24";	break;
	case BGBCC_SH_RLC_ABS48_BJX:		sn="ABS48_BJX";		break;
	case BGBCC_SH_RLC_RVA24:			sn="RVA24";			break;
	case BGBCC_SH_RLC_RELW12_RVI:		sn="RELW12_RVI";	break;
	case BGBCC_SH_RLC_RELW20_RVI:		sn="RELW20_RVI";	break;
	case BGBCC_SH_RLC_ABS96:			sn="ABS96";			break;

	case BGBCC_SH_RLC_REL32UI_RVI:		sn="REL32UI_RVI";	break;

	case BGBCC_SH_RLC_RELW8_BJCMP:		sn="RELW8_BJX";		break;
	case BGBCC_SH_RLC_RELW32_BJCMP:		sn="RELW32_BJX";	break;

	case BGBCC_SH_RLC_ABSHI20_RVI:		sn="ABSHI20_RVI";	break;
	case BGBCC_SH_RLC_ABSLO12I_RVI:		sn="ABSLO12I_RVI";	break;
	case BGBCC_SH_RLC_ABSLO12S_RVI:		sn="ABSLO12S_RVI";	break;

	case BGBCC_SH_RLC_RELHI20_RVI:		sn="RELHI20_RVI";	break;
	case BGBCC_SH_RLC_RELLO12I_RVI:		sn="RELLO12I_RVI";	break;
	case BGBCC_SH_RLC_RELLO12S_RVI:		sn="RELLO12S_RVI";	break;

	case BGBCC_SH_RLC_RELW11_BJCMP:		sn="RELW11_BJX";	break;
	case BGBCC_SH_RLC_RELW33_BJCMP:		sn="RELW33_BJX";	break;

	default:
		sprintf(tb, "RLC_%02X", rlc);
		sn=bgbcc_strdup(tb);
		break;
	}
	return(sn);
}

char *BGBCC_JX2DA_NmidToName(BGBCC_JX2_Context *ctx, int nmid, int wex2)
{
	char tb[128];
	char *sn;

	switch(nmid)
	{
	case BGBCC_SH_NMID_BRAN:	nmid=BGBCC_SH_NMID_BRA;		break;
	case BGBCC_SH_NMID_BSRN:	nmid=BGBCC_SH_NMID_BSR;		break;
	case BGBCC_SH_NMID_RTSN:	nmid=BGBCC_SH_NMID_RTS;		break;
	default:
		break;
	}
	
	if((nmid==BGBCC_SH_NMID_BRA) && (wex2==4))
		{ nmid=BGBCC_SH_NMID_BT; wex2=0; }
	if((nmid==BGBCC_SH_NMID_BRA) && (wex2==5))
		{ nmid=BGBCC_SH_NMID_BF; wex2=0; }
	
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOV:		sn="MOV";		break;	//0x01	//MOV
	case BGBCC_SH_NMID_MOVB:	sn="MOV.B";		break;	//0x02	//MOV.B
	case BGBCC_SH_NMID_MOVW:	sn="MOV.W";		break;	//0x03	//MOV.W
	case BGBCC_SH_NMID_MOVL:	sn="MOV.L";		break;	//0x04	//MOV.L
	case BGBCC_SH_NMID_ADD:		sn="ADD";		break;	//0x05	//ADD
	case BGBCC_SH_NMID_ADDC:	sn="ADDC";		break;	//0x06	//ADDC
	case BGBCC_SH_NMID_ADDV:	sn="ADDV";		break;	//0x07	//ADDV
	case BGBCC_SH_NMID_SUB:		sn="SUB";		break;	//0x08	//SUB
	case BGBCC_SH_NMID_SUBC:	sn="SUBC";		break;	//0x09	//SUBC
	case BGBCC_SH_NMID_SUBV:	sn="SUBV";		break;	//0x0A	//SUBV
	case BGBCC_SH_NMID_CMPHS:	sn="CMPHS";	break;	//0x0B	//CMPHS
	case BGBCC_SH_NMID_CMPHI:	sn="CMPHI";	break;	//0x0C	//CMPHI
	case BGBCC_SH_NMID_CMPEQ:	sn="CMPEQ";	break;	//0x0D	//CMPEQ
	case BGBCC_SH_NMID_CMPGE:	sn="CMPGE";	break;	//0x0E	//CMPGE
	case BGBCC_SH_NMID_CMPGT:	sn="CMPGT";	break;	//0x0F	//CMPGT
	case BGBCC_SH_NMID_JMP:		sn="JMP";		break;	//0x10	//JMP
	case BGBCC_SH_NMID_JSR:		sn="JSR";		break;	//0x11	//JSR
	case BGBCC_SH_NMID_BRA:		sn="BRA";		break;	//0x12	//BRA
	case BGBCC_SH_NMID_BSR:		sn="BSR";		break;	//0x13	//BSR
	case BGBCC_SH_NMID_BT:		sn="BT";		break;	//0x14	//BT
	case BGBCC_SH_NMID_BF:		sn="BT";		break;	//0x15	//BF
	case BGBCC_SH_NMID_BTS:		sn="BTS";		break;	//0x16	//BTS
	case BGBCC_SH_NMID_BFS:		sn="BFS";		break;	//0x17	//BFS
	case BGBCC_SH_NMID_DIV1:	sn="DIV1";		break;	//0x18	//DIV1
	case BGBCC_SH_NMID_DMULU:	sn="MULU.L";	break;	//0x19	//DMULU
	case BGBCC_SH_NMID_DMULS:	sn="MULS.L";	break;	//0x1A	//DMULS
	case BGBCC_SH_NMID_TST:		sn="TST";		break;	//0x1B	//TST
	case BGBCC_SH_NMID_AND:		sn="AND";		break;	//0x1C	//AND
	case BGBCC_SH_NMID_XOR:		sn="XOR";		break;	//0x1D	//XOR
	case BGBCC_SH_NMID_OR:		sn="OR";		break;	//0x1E	//OR
	case BGBCC_SH_NMID_DIV0S:	sn="DIV0S";		break;	//0x20
	case BGBCC_SH_NMID_CMPSTR:	sn="CMP/STR";	break;	//0x21
	case BGBCC_SH_NMID_MULUW:	sn="MULU.W";	break;	//0x22
	case BGBCC_SH_NMID_MULSW:	sn="MULS.W";	break;	//0x23
	case BGBCC_SH_NMID_MACL:	sn="MAC.L";		break;	//0x24
	case BGBCC_SH_NMID_BSRF:	sn="BSRF";		break;	//0x25
	case BGBCC_SH_NMID_BRAF:	sn="BRAF";		break;	//0x26
	case BGBCC_SH_NMID_MULL:	sn="MUL.L";		break;	//0x27
	case BGBCC_SH_NMID_CLRT:	sn="CLRT";		break;	//0x28
	case BGBCC_SH_NMID_SETT:	sn="SETT";		break;	//0x29
	case BGBCC_SH_NMID_CLRMAC:	sn="CLRMAC";	break;	//0x2A
	case BGBCC_SH_NMID_NOP:		sn="NOP";		break;	//0x2B
	case BGBCC_SH_NMID_MOVT:	sn="MOVT";		break;	//0x2C
	case BGBCC_SH_NMID_RTS:		sn="RTS";		break;	//0x2D
	case BGBCC_SH_NMID_SLEEP:	sn="SLEEP";		break;	//0x2E
	case BGBCC_SH_NMID_RTE:		sn="RTE";		break;	//0x2F
	case BGBCC_SH_NMID_SHLL:	sn="SHLL";		break;	//0x30
	case BGBCC_SH_NMID_DT:		sn="DT";		break;	//0x31
	case BGBCC_SH_NMID_SHAL:	sn="SHAL";		break;	//0x32
	case BGBCC_SH_NMID_SHLR:	sn="SHLR";		break;	//0x33
	case BGBCC_SH_NMID_CMPPZ:	sn="CMPPZ";		break;	//0x34
	case BGBCC_SH_NMID_SHAR:	sn="SHAR";		break;	//0x35
	case BGBCC_SH_NMID_ROTL:	sn="ROTL";		break;	//0x36
	case BGBCC_SH_NMID_ROTCL:	sn="ROTCL";		break;	//0x37
	case BGBCC_SH_NMID_ROTT:	sn="ROTT";		break;	//0x38
	case BGBCC_SH_NMID_ROTCR:	sn="ROTCR";		break;	//0x39
	case BGBCC_SH_NMID_SHLL2:	sn="SHLL2";		break;	//0x3A
	case BGBCC_SH_NMID_ROTR:	sn="ROTR";		break;	//0x3B
	case BGBCC_SH_NMID_CMPPL:	sn="CMPPL";		break;	//0x3C
	case BGBCC_SH_NMID_SHLL8:	sn="SHLL8";		break;	//0x3D
	case BGBCC_SH_NMID_SHLL16:	sn="SHLL16";	break;	//0x3E
	case BGBCC_SH_NMID_SHLR2:	sn="SHLR2";		break;	//0x3F
	case BGBCC_SH_NMID_SHLR8:	sn="SHLR8";		break;	//0x40
	case BGBCC_SH_NMID_SHLR16:	sn="SHLR16";	break;	//0x41
	case BGBCC_SH_NMID_MACW:	sn="MAC.W";		break;	//0x42
	case BGBCC_SH_NMID_NOT:		sn="NOT";		break;	//0x43
	case BGBCC_SH_NMID_SWAPB:	sn="SWAP.B";	break;	//0x44
	case BGBCC_SH_NMID_SWAPW:	sn="SWAP.W";	break;	//0x45
	case BGBCC_SH_NMID_NEGC:	sn="NEGC";		break;	//0x46
	case BGBCC_SH_NMID_NEG:		sn="NEG";		break;	//0x47
	case BGBCC_SH_NMID_EXTUB:	sn="EXTU.B";	break;	//0x48
	case BGBCC_SH_NMID_EXTUW:	sn="EXTU.W";	break;	//0x49
	case BGBCC_SH_NMID_EXTSB:	sn="EXTS.B";	break;	//0x4A
	case BGBCC_SH_NMID_EXTSW:	sn="EXTS.W";	break;	//0x4B
	case BGBCC_SH_NMID_TRAPA:	sn="TRAPA";		break;	//0x4C
	case BGBCC_SH_NMID_MOVA:	sn="MOVA";		break;	//0x4D
	case BGBCC_SH_NMID_TSTB:	sn="TSTB";		break;	//0x4E
	case BGBCC_SH_NMID_DIV0U:	sn="DIV0U";		break;	//0x4F
	case BGBCC_SH_NMID_LDC:		sn="LDC";		break;	//0x50
	case BGBCC_SH_NMID_LDCL:	sn="LDC.L";		break;	//0x51
	case BGBCC_SH_NMID_LDS:		sn="LDS";		break;	//0x52
	case BGBCC_SH_NMID_LDSL:	sn="LDS.L";		break;	//0x53
	case BGBCC_SH_NMID_STC:		sn="STC";		break;	//0x54
	case BGBCC_SH_NMID_STCL:	sn="STC.L";		break;	//0x55
	case BGBCC_SH_NMID_STS:		sn="STS";		break;	//0x56
	case BGBCC_SH_NMID_STSL:	sn="STS.L";		break;	//0x57
	case BGBCC_SH_NMID_CASL:	sn="CAS.L";		break;	//0x58
	case BGBCC_SH_NMID_TASB:	sn="TAS.B";		break;	//0x59
	case BGBCC_SH_NMID_XTRCT:	sn="XTRCT";		break;	//0x5A
	case BGBCC_SH_NMID_ANDB:	sn="AND.B";		break;	//0x5B
	case BGBCC_SH_NMID_XORB:	sn="XOR.B";		break;	//0x5C
	case BGBCC_SH_NMID_ORB:		sn="OR.B";		break;	//0x5D
	case BGBCC_SH_NMID_SHAD:	sn="SHAD";		break;	//0x5E
	case BGBCC_SH_NMID_SHLD:	sn="SHLD";		break;	//0x5F
	case BGBCC_SH_NMID_CLRS:	sn="CLRS";		break;	//0x61
	case BGBCC_SH_NMID_SETS:	sn="SETS";		break;	//0x62
	case BGBCC_SH_NMID_MOVUB:	sn="MOVU.B";	break;	//0x65	//MOVU.B
	case BGBCC_SH_NMID_MOVUW:	sn="MOVU.W";	break;	//0x66	//MOVU.W
	case BGBCC_SH_NMID_MOVUL:	sn="MOVU.L";	break;	//0x67	//MOVU.L
	case BGBCC_SH_NMID_LEAB:	sn="LEA.B";		break;	//0x68	//LEA.B
	case BGBCC_SH_NMID_LEAW:	sn="LEA.W";		break;	//0x69	//LEA.W
	case BGBCC_SH_NMID_LEAL:	sn="LEA.L";		break;	//0x6A	//LEA.L
	case BGBCC_SH_NMID_LEAQ:	sn="LEA.Q";		break;	//0x6B	//LEA.Q
	case BGBCC_SH_NMID_LEAO:	sn="LEA.O";		break;	//0x6C	//LEA.O
	case BGBCC_SH_NMID_NOTT:	sn="NOTT";		break;	//0x6D
	case BGBCC_SH_NMID_LDTLB:	sn="LDTLB";		break;	//0x6E
	case BGBCC_SH_NMID_BRK:		sn="BRK";		break;	//0x6F
	case BGBCC_SH_NMID_MOVRT:	sn="MOVRT";		break;	//0x70
	case BGBCC_SH_NMID_MOVCAL:	sn="MOVCAL";	break;	//0x71
	case BGBCC_SH_NMID_PUSH:	sn="PUSH";		break;	//0x72
	case BGBCC_SH_NMID_POP:		sn="POP";		break;	//0x73
	case BGBCC_SH_NMID_ICLRMD:	sn="ICLRMD";	break;	//0x74
	case BGBCC_SH_NMID_ISETMD:	sn="ISETMD";	break;	//0x75
	case BGBCC_SH_NMID_SHLL1:	sn="SHLL1";		break;	//0x76
	case BGBCC_SH_NMID_LDHF16:	sn="LDHF16";	break;	//0x77
	case BGBCC_SH_NMID_BRAN:	sn="BRA/N";		break;	//0x78	//BRA/N
	case BGBCC_SH_NMID_BSRN:	sn="BSR/N";		break;	//0x79	//BSR/N
	case BGBCC_SH_NMID_RTSN:	sn="RTS/N";		break;	//0x7A
	case BGBCC_SH_NMID_LDIF16:	sn="LDIF16";	break;	//0x7B
	case BGBCC_SH_NMID_STHF16:	sn="STHF16";	break;	//0x7C
	case BGBCC_SH_NMID_CSELT:	sn="CSELT";		break;	//0x7D
	case BGBCC_SH_NMID_EXTUL:	sn="EXTU.L";	break;	//0x7E
	case BGBCC_SH_NMID_EXTSL:	sn="EXTS.L";	break;	//0x7F

	case BGBCC_SH_NMID_FABS:	sn="FABS";		break;	//0x80
	case BGBCC_SH_NMID_FADD:	sn="FADD";		break;	//0x81
	case BGBCC_SH_NMID_FCMPEQ:	sn="FCMP/EQ";	break;	//0x82
	case BGBCC_SH_NMID_FCMPGT:	sn="FCMP/GT";	break;	//0x83
	case BGBCC_SH_NMID_FCNVDS:	sn="FCNVDS";	break;	//0x84
	case BGBCC_SH_NMID_FCNVSD:	sn="FCNVSD";	break;	//0x85
	case BGBCC_SH_NMID_FDIV:	sn="FDIV";		break;	//0x86
	case BGBCC_SH_NMID_FLDI0:	sn="FLDI0";		break;	//0x87
	case BGBCC_SH_NMID_FLDI1:	sn="FLDI1";		break;	//0x88
	case BGBCC_SH_NMID_FLDS:	sn="FLDS";		break;	//0x89
	case BGBCC_SH_NMID_FLOAT:	sn="FLOAT";		break;	//0x8A
	case BGBCC_SH_NMID_FMAC:	sn="FMAC";		break;	//0x8B
	case BGBCC_SH_NMID_FMOV:	sn="FMOV";		break;	//0x8C
	case BGBCC_SH_NMID_FMOVS:	sn="FMOV.S";	break;	//0x8D
	case BGBCC_SH_NMID_FMOVD:	sn="FMOV.D";	break;	//0x8E
	case BGBCC_SH_NMID_FMUL:	sn="FMUL";		break;	//0x8F
	case BGBCC_SH_NMID_FNEG:	sn="FNEG";		break;	//0x90
	case BGBCC_SH_NMID_FSCHG:	sn="FSCHG";		break;	//0x91
	case BGBCC_SH_NMID_FSQRT:	sn="FSQRT";		break;	//0x92
	case BGBCC_SH_NMID_FSTS:	sn="FSTS";		break;	//0x93
	case BGBCC_SH_NMID_FSUB:	sn="FSUB";		break;	//0x94
	case BGBCC_SH_NMID_FTRC:	sn="FTRC";		break;	//0x95
	case BGBCC_SH_NMID_FSRRA:	sn="FSRRA";		break;	//0x96
	case BGBCC_SH_NMID_FIPR:	sn="FIPR";		break;	//0x97
	case BGBCC_SH_NMID_FTRV:	sn="FTRV";		break;	//0x98
	case BGBCC_SH_NMID_FMOVX:	sn="FMOVX";		break;	//0x9B
	case BGBCC_SH_NMID_FMOVIS:	sn="FMOVIS";	break;	//0x9C
	case BGBCC_SH_NMID_FMOVID:	sn="FMOVID";	break;	//0x9D
	case BGBCC_SH_NMID_FMOVSI:	sn="FMOVSI";	break;	//0x9E
	case BGBCC_SH_NMID_FMOVDI:	sn="FMOVDI";	break;	//0x9F

	case BGBCC_SH_NMID_CMOVTB:	sn="CMOVT.B";	break;	//0xA0	//CMOVT.B
	case BGBCC_SH_NMID_CMOVTW:	sn="CMOVT.W";	break;	//0xA1	//CMOVT.W
	case BGBCC_SH_NMID_CMOVTL:	sn="CMOVT.L";	break;	//0xA2	//CMOVT.L
	case BGBCC_SH_NMID_CMOVTQ:	sn="CMOVT.Q";	break;	//0xA3	//CMOVT.Q
	case BGBCC_SH_NMID_CMOVFB:	sn="CMOVF.B";	break;	//0xA4	//CMOVF.B
	case BGBCC_SH_NMID_CMOVFW:	sn="CMOVF.W";	break;	//0xA5	//CMOVF.W
	case BGBCC_SH_NMID_CMOVFL:	sn="CMOVF.L";	break;	//0xA6	//CMOVF.L
	case BGBCC_SH_NMID_CMOVFQ:	sn="CMOVF.Q";	break;	//0xA7	//CMOVF.Q

	case BGBCC_SH_NMID_FLDCF:	sn="FLDCF";		break;	//0xA8
	case BGBCC_SH_NMID_FLDCD:	sn="FLDCD";		break;	//0xA9
	case BGBCC_SH_NMID_FLDCI:	sn="FLDCI";		break;	//0xAA
	case BGBCC_SH_NMID_FLDCH:	sn="FLDCH";		break;	//0xAB
	case BGBCC_SH_NMID_FSTCF:	sn="FSTCF";		break;	//0xAC
	case BGBCC_SH_NMID_FSTCD:	sn="FSTCD";		break;	//0xAD
	case BGBCC_SH_NMID_FSTCI:	sn="FSTCI";		break;	//0xAE
	case BGBCC_SH_NMID_FSTCH:	sn="FSTCH";		break;	//0xAF
	case BGBCC_SH_NMID_FRCPA:	sn="FRCPA";		break;	//0xB0
	case BGBCC_SH_NMID_FSQRTA:	sn="FSQRTA";	break;	//0xB1
	case BGBCC_SH_NMID_FRCP:	sn="FRCP";		break;	//0xB2

	case BGBCC_SH_NMID_BSR2F:		sn="BSR2F";			break;	//0xBA
	case BGBCC_SH_NMID_BRA2F:		sn="BRA2F";			break;	//0xBB
	case BGBCC_SH_NMID_MOVDL:		sn="MOVD.L";		break;	//0xBC
	case BGBCC_SH_NMID_RET:			sn="RET";			break;	//0xBD
	case BGBCC_SH_NMID_PUSHX2:		sn="PUSHX2";		break;	//0xBE
	case BGBCC_SH_NMID_POPX2:		sn="POPX2";			break;	//0xBF

	case BGBCC_SH_NMID_MOVI:		sn="MOVI";			break;	//0xC0
	case BGBCC_SH_NMID_MOVIV:		sn="MOVIV";			break;	//0xC1
	case BGBCC_SH_NMID_PREF:		sn="PREF";			break;	//0xC2
	case BGBCC_SH_NMID_OCBI:		sn="OCBI";			break;	//0xC3
	case BGBCC_SH_NMID_OCBP:		sn="OCBP";			break;	//0xC4
	case BGBCC_SH_NMID_OCBWB:		sn="OCBWB";			break;	//0xC5
	case BGBCC_SH_NMID_ICBI:		sn="ICBI";			break;	//0xC6
	case BGBCC_SH_NMID_MOVQ:		sn="MOV.Q";			break;	//0xC7
	case BGBCC_SH_NMID_ADDQ:		sn="ADD.Q";			break;	//0xC8
	case BGBCC_SH_NMID_SUBQ:		sn="SUB.Q";			break;	//0xC9
	case BGBCC_SH_NMID_MULQ:		sn="MUL.Q";			break;	//0xCA
	case BGBCC_SH_NMID_ANDQ:		sn="AND.Q";			break;	//0xCB
	case BGBCC_SH_NMID_ORQ:			sn="OR.Q";			break;	//0xCC
	case BGBCC_SH_NMID_XORQ:		sn="XOR.Q";			break;	//0xCD
	case BGBCC_SH_NMID_SHLLQ:		sn="SHLL.Q";		break;	//0xCE
	case BGBCC_SH_NMID_SHLRQ:		sn="SHLR.Q";		break;	//0xCF

	case BGBCC_SH_NMID_SHALQ:		sn="SHAL.Q";		break;	//0xD0
	case BGBCC_SH_NMID_SHARQ:		sn="SHAR.Q";		break;	//0xD1
	case BGBCC_SH_NMID_LDSH16:		sn="LDSH16";		break;	//0xD2
	case BGBCC_SH_NMID_TSTQ:		sn="TST.Q";			break;	//0xD3
	case BGBCC_SH_NMID_SHADQ:		sn="SHAD.Q";		break;	//0xD4
	case BGBCC_SH_NMID_SHLDQ:		sn="SHLD.Q";		break;	//0xD5
	case BGBCC_SH_NMID_SHLL4:		sn="SHLL4";			break;	//0xD6
	case BGBCC_SH_NMID_SHLR4:		sn="SHLR4";			break;	//0xD7
	case BGBCC_SH_NMID_BREQ:		sn="BREQ";			break;	//0xD8
	case BGBCC_SH_NMID_BRNE:		sn="BRNE";			break;	//0xD9
	case BGBCC_SH_NMID_BRGT:		sn="BRGT";			break;	//0xDA
	case BGBCC_SH_NMID_BRLE:		sn="BRLE";			break;	//0xDB
	case BGBCC_SH_NMID_BRGE:		sn="BRGE";			break;	//0xDC
	case BGBCC_SH_NMID_BRLT:		sn="BRLT";			break;	//0xDD
	
	case BGBCC_SH_NMID_CMPQHS:		sn="CMPQHS";		break;	//0xE0
	case BGBCC_SH_NMID_CMPQHI:		sn="CMPQHI";		break;	//0xE1
	case BGBCC_SH_NMID_CMPQEQ:		sn="CMPQEQ";		break;	//0xE2
	case BGBCC_SH_NMID_CMPQGE:		sn="CMPQGE";		break;	//0xE3
	case BGBCC_SH_NMID_CMPQGT:		sn="CMPQGT";		break;	//0xE4
	case BGBCC_SH_NMID_CMPQPZ:		sn="CMPQPZ";		break;	//0xE5
	case BGBCC_SH_NMID_CMPQPL:		sn="CMPQPL";		break;	//0xE6
	case BGBCC_SH_NMID_NOTS:		sn="NOTS";			break;	//0xE7
	case BGBCC_SH_NMID_SHLL32:		sn="SHLL32";		break;	//0xE8
	case BGBCC_SH_NMID_SHLR32:		sn="SHLR32";		break;	//0xE9
	case BGBCC_SH_NMID_SHLR1:		sn="SHLR1";			break;	//0xEA
	case BGBCC_SH_NMID_SHAR1:		sn="SHAR1";			break;	//0xEB
	case BGBCC_SH_NMID_SHAR2:		sn="SHAR2";			break;	//0xEC
	case BGBCC_SH_NMID_SHAR4:		sn="SHAR4";			break;	//0xED
	case BGBCC_SH_NMID_SHAR8:		sn="SHAR8";			break;	//0xEE
	case BGBCC_SH_NMID_SHAR16:		sn="SHAR16";		break;	//0xEF

	case BGBCC_SH_NMID_SHAR32:		sn="SHAR32";		break;	//0xF0
	case BGBCC_SH_NMID_LDSH8:		sn="LDSH8";			break;	//0xF1
	case BGBCC_SH_NMID_BRA8B:		sn="BRA8B";			break;	//0xF2

	case BGBCC_SH_NMID_MOVNT:		sn="MOVNT";			break;	//0xF4
	case BGBCC_SH_NMID_CLZ:			sn="CLZ";			break;	//0xF5
	case BGBCC_SH_NMID_CLZQ:		sn="CLZQ";			break;	//0xF6

	case BGBCC_SH_NMID_SWAP8B:		sn="SWAP8B";		break;	//0x00F7
	case BGBCC_SH_NMID_SWCPLB:		sn="SWCPLB";		break;	//0x00F8
	case BGBCC_SH_NMID_SWAPLB:		sn="SWAPLB";		break;	//0x00F9
	case BGBCC_SH_NMID_SWAPMB:		sn="SWAPMB";		break;	//0x00FA
	case BGBCC_SH_NMID_SWAPHB:		sn="SWAPHB";		break;	//0x00FB
	case BGBCC_SH_NMID_SWCPMB:		sn="SWCPMB";		break;	//0x00FC
	case BGBCC_SH_NMID_SWCPHB:		sn="SWCPHB";		break;	//0x00FD
	case BGBCC_SH_NMID_LDBF16:		sn="LDBF16";		break;	//0x00FE
	case BGBCC_SH_NMID_STBF16:		sn="STBF16";		break;	//0x00FF

	case BGBCC_SH_NMID_ADDL:		sn="ADD.L";			break;	//0x0100
	case BGBCC_SH_NMID_SUBL:		sn="SUB.L";			break;	//0x0101
	case BGBCC_SH_NMID_PADDW:		sn="PADD.W";		break;	//0x0102
	case BGBCC_SH_NMID_PADDL:		sn="PADD.L";		break;	//0x0103
	case BGBCC_SH_NMID_PSUBW:		sn="PSUB.W";		break;	//0x0104
	case BGBCC_SH_NMID_PSUBL:		sn="PSUB.L";		break;	//0x0105
	case BGBCC_SH_NMID_PMULUW:		sn="PMULU.W";		break;	//0x0106
	case BGBCC_SH_NMID_MOVD:		sn="MOVD";			break;	//0x0107
	case BGBCC_SH_NMID_MOVHD:		sn="MOVHD";			break;	//0x0108
	case BGBCC_SH_NMID_MOVLD:		sn="MOVLD";			break;	//0x0109
	case BGBCC_SH_NMID_MOVHLD:		sn="MOVHLD";		break;	//0x010A
	case BGBCC_SH_NMID_MOVLHD:		sn="MOVLHD";		break;	//0x010B
	case BGBCC_SH_NMID_ADDSL:		sn="ADDS.L";		break;	//0x010C
	case BGBCC_SH_NMID_ADDUL:		sn="ADDU.L";		break;	//0x010D
	case BGBCC_SH_NMID_SUBSL:		sn="SUBS.L";		break;	//0x010E
	case BGBCC_SH_NMID_SUBUL:		sn="SUBU.L";		break;	//0x010F

	case BGBCC_SH_NMID_MOVX2:		sn="MOV.X";			break;	//0x0120
	case BGBCC_SH_NMID_PADDH:		sn="PADDH";			break;	//0x0121
	case BGBCC_SH_NMID_PSUBH:		sn="PSUBH";			break;	//0x0122
	case BGBCC_SH_NMID_PMULH:		sn="PMULH";			break;	//0x0123
	case BGBCC_SH_NMID_WEXMD:		sn="WEXMD";			break;	//0x0124
	case BGBCC_SH_NMID_CPUID:		sn="CPUID";			break;	//0x0125
	case BGBCC_SH_NMID_RTSU:		sn="RTSU";			break;	//0x0126
	case BGBCC_SH_NMID_SYSCALL:		sn="SYSCALL";		break;	//0x0127
	case BGBCC_SH_NMID_INVIC:		sn="INVIC";			break;	//0x0128
	case BGBCC_SH_NMID_INVDC:		sn="INVDC";			break;	//0x0129
	case BGBCC_SH_NMID_INVTLB:		sn="INVTLB";		break;	//0x012A
	case BGBCC_SH_NMID_JLDI:		sn="JLDI";			break;	//0x012B
	case BGBCC_SH_NMID_MULUL:		sn="DMULU.L";		break;	//0x012C
	case BGBCC_SH_NMID_MULSL:		sn="DMULS.L";		break;	//0x012D
	case BGBCC_SH_NMID_BRA2B:		sn="BRA2B";			break;	//0x012E
	case BGBCC_SH_NMID_BRA4B:		sn="BRA4B";			break;	//0x012F

	case BGBCC_SH_NMID_FLDCFH:		sn="FLDCFH";		break;	//0x0130
	case BGBCC_SH_NMID_FSTCFH:		sn="FSTCFH";		break;	//0x0131
	case BGBCC_SH_NMID_RGB5SHR1:	sn="RGB5SHR1";		break;	//0x0132
	case BGBCC_SH_NMID_PMULSW:		sn="PMULS.W";		break;	//0x0133
	case BGBCC_SH_NMID_PMULULW:		sn="PMULUL.W";		break;	//0x0134
	case BGBCC_SH_NMID_PMULSLW:		sn="PMULSL.W";		break;	//0x0135
	case BGBCC_SH_NMID_PMULUHW:		sn="PMULUH.W";		break;	//0x0136
	case BGBCC_SH_NMID_PMULSHW:		sn="PMULSH.W";		break;	//0x0137
	case BGBCC_SH_NMID_RGB5PCK32:	sn="RGB5PCK32";		break;	//0x0138
	case BGBCC_SH_NMID_RGB5PCK64:	sn="RGB5PCK64";		break;	//0x0139
	case BGBCC_SH_NMID_RGB5UPCK32:	sn="RGB5UPCK32";	break;	//0x013A
	case BGBCC_SH_NMID_RGB5UPCK64:	sn="RGB5UPCK64";	break;	//0x013B
	case BGBCC_SH_NMID_RGB32PCK64:	sn="RGB32PCK64";	break;	//0x013C
	case BGBCC_SH_NMID_RGB32UPCK64:	sn="RGB32UPCK64";	break;	//0x013D
	case BGBCC_SH_NMID_PSHUFB:		sn="PSHUF.B";		break;	//0x013E
	case BGBCC_SH_NMID_PSHUFW:		sn="PSHUF.W";		break;	//0x013F

	case BGBCC_SH_NMID_PADDF:		sn="PADD.F";		break;	//0x0140
	case BGBCC_SH_NMID_PSUBF:		sn="PSUB.F";		break;	//0x0141
	case BGBCC_SH_NMID_PMULF:		sn="PMUL.F";		break;	//0x0142
	case BGBCC_SH_NMID_PADDFX:		sn="PADDX.F";		break;	//0x0143
	case BGBCC_SH_NMID_PSUBFX:		sn="PSUBX.F";		break;	//0x0144
	case BGBCC_SH_NMID_PMULFX:		sn="PMULX.F";		break;	//0x0145
	case BGBCC_SH_NMID_PSHUFL:		sn="PSHUF.L";		break;	//0x0146
	case BGBCC_SH_NMID_PSHUFXL:		sn="PSHUFX.L";		break;	//0x0147
	case BGBCC_SH_NMID_CTZ:			sn="CTZ";			break;	//0x0148
	case BGBCC_SH_NMID_CTZQ:		sn="CTZQ";			break;	//0x0149
	case BGBCC_SH_NMID_BTRNS:		sn="BTRNS";			break;	//0x014A
	case BGBCC_SH_NMID_BTRNSQ:		sn="BTRNSQ";		break;	//0x014B
	case BGBCC_SH_NMID_PMORTL:		sn="PMORT.L";		break;	//0x014C
	case BGBCC_SH_NMID_PMORTQ:		sn="PMORT.Q";		break;	//0x014D
	case BGBCC_SH_NMID_MOVP:		sn="MOV.P";			break;	//0x014E
	case BGBCC_SH_NMID_LEAP:		sn="LEA.P";			break;	//0x014F

	case BGBCC_SH_NMID_PADDXD:		sn="PADDX.D";		break;	//0x0150
	case BGBCC_SH_NMID_PSUBXD:		sn="PSUBX.D";		break;	//0x0151
	case BGBCC_SH_NMID_PMULXD:		sn="PMULX.D";		break;	//0x0152
	case BGBCC_SH_NMID_MOVQ_DISP:	sn="MOV.Q";			break;	//0x0153
	case BGBCC_SH_NMID_PCSELTW:		sn="PCSELT.W";		break;	//0x0154
	case BGBCC_SH_NMID_PCSELTL:		sn="PCSELT.L";		break;	//0x0155
	case BGBCC_SH_NMID_PCMPEQW:		sn="PCMPEQ.W";		break;	//0x0156
	case BGBCC_SH_NMID_PCMPEQL:		sn="PCMPEQ.L";		break;	//0x0157
	case BGBCC_SH_NMID_PCMPHIW:		sn="PCMPHI.W";		break;	//0x0158
	case BGBCC_SH_NMID_PCMPHIL:		sn="PCMPHI.L";		break;	//0x0159
	case BGBCC_SH_NMID_PCMPGTW:		sn="PCMPGT.W";		break;	//0x015A
	case BGBCC_SH_NMID_PCMPGTL:		sn="PCMPGT.L";		break;	//0x015B
	case BGBCC_SH_NMID_PCMPEQH:		sn="PCMPEQ.H";		break;	//0x015C
	case BGBCC_SH_NMID_PCMPEQF:		sn="PCMPEQ.F";		break;	//0x015D
	case BGBCC_SH_NMID_PCMPGTH:		sn="PCMPGT.H";		break;	//0x015E
	case BGBCC_SH_NMID_PCMPGTF:		sn="PCMPGT.F";		break;	//0x015F

	case BGBCC_SH_NMID_SHLL64:		sn="SHLL64";		break;	//0x0160
	case BGBCC_SH_NMID_SHLR64:		sn="SHLR64";		break;	//0x0161
	case BGBCC_SH_NMID_FADDX:		sn="FADDX";			break;	//0x0162
	case BGBCC_SH_NMID_FSUBX:		sn="FSUBX";			break;	//0x0163
	case BGBCC_SH_NMID_FMULX:		sn="FMULX";			break;	//0x0164
	case BGBCC_SH_NMID_FMACX:		sn="FMACX";			break;	//0x0165
	case BGBCC_SH_NMID_FCMPXEQ:		sn="FCMPXEQ";		break;	//0x0166
	case BGBCC_SH_NMID_FCMPXGT:		sn="FCMPXGT";		break;	//0x0167
	case BGBCC_SH_NMID_FLDCDX:		sn="FLDCDX";		break;	//0x0168
	case BGBCC_SH_NMID_FSTCDX:		sn="FSTCDX";		break;	//0x0169
	case BGBCC_SH_NMID_FLDCIX:		sn="FLDCIX";		break;	//0x016A
	case BGBCC_SH_NMID_FSTCIX:		sn="FSTCIX";		break;	//0x016B
	case BGBCC_SH_NMID_SHADX:		sn="SHADX";			break;	//0x016C
	case BGBCC_SH_NMID_SHLDX:		sn="SHLDX";			break;	//0x016D
	case BGBCC_SH_NMID_SHARX:		sn="SHARX";			break;	//0x016E
	case BGBCC_SH_NMID_SHLRX:		sn="SHLRX";			break;	//0x016F

	case BGBCC_SH_NMID_ADDX:		sn="ADDX";			break;	//0x0170
	case BGBCC_SH_NMID_SUBX:		sn="SUBX";			break;	//0x0171
	case BGBCC_SH_NMID_ADCX:		sn="ADCX";			break;	//0x0172
	case BGBCC_SH_NMID_SBBX:		sn="SBBX";			break;	//0x0173
	case BGBCC_SH_NMID_ROTLX:		sn="ROTLX";			break;	//0x0174
	case BGBCC_SH_NMID_ANDX:		sn="ANDX";			break;	//0x0175
	case BGBCC_SH_NMID_ORX:			sn="ORX";			break;	//0x0176
	case BGBCC_SH_NMID_XORX:		sn="XORX";			break;	//0x0177
	case BGBCC_SH_NMID_CMPXEQ:		sn="CMPXEQ";		break;	//0x0178
	case BGBCC_SH_NMID_CMPXHI:		sn="CMPXHI";		break;	//0x0179
	case BGBCC_SH_NMID_CMPXGT:		sn="CMPXGT";		break;	//0x017A
	case BGBCC_SH_NMID_MOVQ_DISP24:	sn="MOV.Q";			break;	//0x017B
	case BGBCC_SH_NMID_BLKUTX1:		sn="BLKUTX1";		break;	//0x017C
	case BGBCC_SH_NMID_BLKUTX2:		sn="BLKUTX2";		break;	//0x017D
	case BGBCC_SH_NMID_BLKUTX3H:	sn="BLKUTX3H";		break;	//0x017E
	case BGBCC_SH_NMID_BLKUTX3L:	sn="BLKUTX3L";		break;	//0x017F

	case BGBCC_SH_NMID_BLINTW:		sn="BLINTW";		break;	//0x0180
	case BGBCC_SH_NMID_BLERPW:		sn="BLERPW";		break;	//0x0181
	case BGBCC_SH_NMID_BLINTAW:		sn="BLINTAW";		break;	//0x0182
	case BGBCC_SH_NMID_PSTCH:		sn="PSTCH";			break;	//0x0183
	case BGBCC_SH_NMID_PLDCH:		sn="PLDCH";			break;	//0x0184
	case BGBCC_SH_NMID_PLDCHH:		sn="PLDCHH";		break;	//0x0185
	case BGBCC_SH_NMID_BLKUAB1:		sn="BLKUAB1";		break;	//0x0186
	case BGBCC_SH_NMID_BLKUAB2:		sn="BLKUAB2";		break;	//0x0187
	case BGBCC_SH_NMID_PLDCM8SH:	sn="PLDCM8SH";		break;	//0x0188
	case BGBCC_SH_NMID_PLDCM8UH:	sn="PLDCM8UL";		break;	//0x0189
	case BGBCC_SH_NMID_PSTCM8SH:	sn="PSTCM8SH";		break;	//0x018A
	case BGBCC_SH_NMID_PSTCM8UH:	sn="PSTCM8UH";		break;	//0x018B
	case BGBCC_SH_NMID_BRAB:		sn="BRAB";			break;	//0x018C
	case BGBCC_SH_NMID_BSRB:		sn="BSRB";			break;	//0x018D
	case BGBCC_SH_NMID_BTB:			sn="BTB";			break;	//0x018E
	case BGBCC_SH_NMID_BFB:			sn="BFB";			break;	//0x018F

	case BGBCC_SH_NMID_BRAL:		sn="BRA.L";			break;	//0x0190
	case BGBCC_SH_NMID_BSRL:		sn="BSR.L";			break;	//0x0191
	case BGBCC_SH_NMID_BTL:			sn="BT.L";			break;	//0x0192
	case BGBCC_SH_NMID_BFL:			sn="BF.L";			break;	//0x0193
	case BGBCC_SH_NMID_PSCHEQW:		sn="PSCHEQ.W";		break;	//0x0194
	case BGBCC_SH_NMID_PSCHEQB:		sn="PSCHEQ.B";		break;	//0x0195
	case BGBCC_SH_NMID_PSCHNEW:		sn="PSCHNE.W";		break;	//0x0196
	case BGBCC_SH_NMID_PSCHNEB:		sn="PSCHNE.B";		break;	//0x0197
	case BGBCC_SH_NMID_CONVFXI:		sn="CONVFXI";		break;	//0x0198
	case BGBCC_SH_NMID_CONVFLI:		sn="CONVFLI";		break;	//0x0199
	case BGBCC_SH_NMID_SNIPEDC:		sn="SNIPEDC";		break;	//0x019A
	case BGBCC_SH_NMID_SNIPEIC:		sn="SNIPEIC";		break;	//0x019B
	case BGBCC_SH_NMID_LDACL:		sn="LDACL";			break;	//0x019C
	case BGBCC_SH_NMID_FADDG:		sn="FADDG";			break;	//0x019D
	case BGBCC_SH_NMID_FSUBG:		sn="FSUBG";			break;	//0x019E
	case BGBCC_SH_NMID_FMULG:		sn="FMULG";			break;	//0x019F

	case BGBCC_SH_NMID_LDEKRR:		sn="LDEKRR";		break;	//0x01A0
	case BGBCC_SH_NMID_SVEKRR:		sn="SVEKRR";		break;	//0x01A1
	case BGBCC_SH_NMID_LDEKEY:		sn="LDEKEY";		break;	//0x01A2
	case BGBCC_SH_NMID_LDEENC:		sn="LDEENC";		break;	//0x01A3
	case BGBCC_SH_NMID_PLDCEHL:		sn="PLDCEHL";		break;	//0x01A4
	case BGBCC_SH_NMID_PLDCEHH:		sn="PLDCEHH";		break;	//0x01A5
	case BGBCC_SH_NMID_FMOVH:		sn="FMOV.H";		break;	//0x01A6
	case BGBCC_SH_NMID_MOVC:		sn="MOV.C";			break;	//0x01A7
	case BGBCC_SH_NMID_BRGTU:		sn="BRGTU";			break;	//0x01A8
	case BGBCC_SH_NMID_BRLEU:		sn="BRLEU";			break;	//0x01A9
	case BGBCC_SH_NMID_BRGEU:		sn="BRGEU";			break;	//0x01AA
	case BGBCC_SH_NMID_BRLTU:		sn="BRLTU";			break;	//0x01AB
	case BGBCC_SH_NMID_CMPTAEQ:		sn="CMPTAEQ";		break;	//0x01AC
	case BGBCC_SH_NMID_CMPTAHI:		sn="CMPTAHI";		break;	//0x01AD
	case BGBCC_SH_NMID_CMPTAHS:		sn="CMPTAHS";		break;	//0x01AE
	case BGBCC_SH_NMID_CMPTTEQ:		sn="CMPTTEQ";		break;	//0x01AF

	case BGBCC_SH_NMID_XMOVB:		sn="XMOV.B";		break;	//0x01B0
	case BGBCC_SH_NMID_XMOVW:		sn="XMOV.W";		break;	//0x01B1
	case BGBCC_SH_NMID_XMOVL:		sn="XMOV.L";		break;	//0x01B2
	case BGBCC_SH_NMID_XMOVQ:		sn="XMOV.Q";		break;	//0x01B3
	case BGBCC_SH_NMID_XMOVUB:		sn="XMOVU.B";		break;	//0x01B4
	case BGBCC_SH_NMID_XMOVUW:		sn="XMOVU.W";		break;	//0x01B5
	case BGBCC_SH_NMID_XMOVUL:		sn="XMOVU.L";		break;	//0x01B6
	case BGBCC_SH_NMID_XMOVX2:		sn="XMOV.X";		break;	//0x01B7
	case BGBCC_SH_NMID_XMOVP:		sn="XMOV.P";		break;	//0x01B8
	case BGBCC_SH_NMID_MOVC_DISP24:	sn="MOV.C";			break;	//0x01B9	
	case BGBCC_SH_NMID_XJMP:		sn="XJMP";			break;	//0x01BA
	case BGBCC_SH_NMID_XJSR:		sn="XJSR";			break;	//0x01BB
	case BGBCC_SH_NMID_MACSL:		sn="MACS.L";		break;	//0x01BC
	case BGBCC_SH_NMID_MACUL:		sn="MACU.L";		break;	//0x01BD
	case BGBCC_SH_NMID_DMACSL:		sn="DMACS.L";		break;	//0x01BE
	case BGBCC_SH_NMID_DMACUL:		sn="DMACU.L";		break;	//0x01BF

	case BGBCC_SH_NMID_BREQL:		sn="BREQ.L";		break;	//0x01C0
	case BGBCC_SH_NMID_BRNEL:		sn="BRNE.L";		break;	//0x01C1
	case BGBCC_SH_NMID_BRGTL:		sn="BRGT.L";		break;	//0x01C2
	case BGBCC_SH_NMID_BRLEL:		sn="BRLE.L";		break;	//0x01C3
	case BGBCC_SH_NMID_BRGEL:		sn="BRGE.L";		break;	//0x01C4
	case BGBCC_SH_NMID_BRLTL:		sn="BRLT.L";		break;	//0x01C5
	case BGBCC_SH_NMID_MOVTT:		sn="MOVTT";			break;	//0x01C6
	case BGBCC_SH_NMID_XMOVTT:		sn="XMOVTT";		break;	//0x01C7
	case BGBCC_SH_NMID_BRGTUL:		sn="BRGTUL";		break;	//0x01C8
	case BGBCC_SH_NMID_BRLEUL:		sn="BRLEUL";		break;	//0x01C9
	case BGBCC_SH_NMID_BRGEUL:		sn="BRGEUL";		break;	//0x01CA
	case BGBCC_SH_NMID_BRLTUL:		sn="BRLTUL";		break;	//0x01CB
	case BGBCC_SH_NMID_XLEAB:		sn="XLEA.B";		break;	//0x01CC
	case BGBCC_SH_NMID_XLEAW:		sn="XLEA.W";		break;	//0x01CD
	case BGBCC_SH_NMID_XLEAL:		sn="XLEA.L";		break;	//0x01CE
	case BGBCC_SH_NMID_XLEAQ:		sn="XLEA.Q";		break;	//0x01CF

	case BGBCC_SH_NMID_MULSQ:		sn="MULS.Q";		break;	//0x01D0
	case BGBCC_SH_NMID_MULUQ:		sn="MULU.Q";		break;	//0x01D1
	case BGBCC_SH_NMID_DIVSQ:		sn="DIVS.Q";		break;	//0x01D2
	case BGBCC_SH_NMID_DIVUQ:		sn="DIVU.Q";		break;	//0x01D3
	case BGBCC_SH_NMID_MODSQ:		sn="MODS.Q";		break;	//0x01D4
	case BGBCC_SH_NMID_MODUQ:		sn="MODU.Q";		break;	//0x01D5
	case BGBCC_SH_NMID_FDIVX:		sn="FDIVX";			break;	//0x01D6
	case BGBCC_SH_NMID_LDTEX:		sn="LDTEX";			break;	//0x01D7
	case BGBCC_SH_NMID_DIVSL:		sn="DIVS.L";		break;	//0x01D8
	case BGBCC_SH_NMID_DIVUL:		sn="DIVU.L";		break;	//0x01D9
	case BGBCC_SH_NMID_MODSL:		sn="MODS.L";		break;	//0x01DA
	case BGBCC_SH_NMID_MODUL:		sn="MODU.L";		break;	//0x01DB
	case BGBCC_SH_NMID_ROTCLQ:		sn="ROTCLQ";		break;	//0x01DC
	case BGBCC_SH_NMID_ROTCRQ:		sn="ROTCRQ";		break;	//0x01DD
	case BGBCC_SH_NMID_BCDADC:		sn="BCDADC";		break;	//0x01DE
	case BGBCC_SH_NMID_BCDSBB:		sn="BCDSBB";		break;	//0x01DF

	case BGBCC_SH_NMID_BCDADD:		sn="BCDADD";		break;	//0x01E0
	case BGBCC_SH_NMID_BCDSUB:		sn="BCDSUB";		break;	//0x01E1
	case BGBCC_SH_NMID_BCDADDX:		sn="BCDADDX";		break;	//0x01E2
	case BGBCC_SH_NMID_BCDSUBX:		sn="BCDSUBX";		break;	//0x01E3
	case BGBCC_SH_NMID_PADDFA:		sn="PADD.FA";		break;	//0x01E4
	case BGBCC_SH_NMID_PSUBFA:		sn="PSUB.FA";		break;	//0x01E5
	case BGBCC_SH_NMID_PMULFA:		sn="PMUL.FA";		break;	//0x01E6
	case BGBCC_SH_NMID_PADDFAX:		sn="PADDX.FA";		break;	//0x01E7
	case BGBCC_SH_NMID_PSUBFAX:		sn="PSUBX.FA";		break;	//0x01E8
	case BGBCC_SH_NMID_PMULFAX:		sn="PMULX.FA";		break;	//0x01E9
	case BGBCC_SH_NMID_LDIZ:		sn="LDIZ";			break;	//0x01EA
	case BGBCC_SH_NMID_LDIN:		sn="LDIN";			break;	//0x01EB
	case BGBCC_SH_NMID_LDHF8S:		sn="LDHF8S";		break;	//0x01EC
	case BGBCC_SH_NMID_LDHF8U:		sn="LDHF8U";		break;	//0x01ED
	case BGBCC_SH_NMID_STHF8S:		sn="STHF8S";		break;	//0x01EE
	case BGBCC_SH_NMID_STHF8U:		sn="STHF8U";		break;	//0x01EF

	case BGBCC_SH_NMID_JIMM:		sn="J_IMM";			break;	//0x01F0
	case BGBCC_SH_NMID_JOP64:		sn="J_OP64";		break;	//0x01F1
	case BGBCC_SH_NMID_LDIHI:		sn="LDIHI";			break;
	case BGBCC_SH_NMID_LDIHIQ:		sn="LDIHIQ";		break;
	case BGBCC_SH_NMID_CMPNANTTEQ:	sn="CMPNANTTEQ";	break;
	case BGBCC_SH_NMID_SETTRAP:		sn="SETTRAP";		break;
	case BGBCC_SH_NMID_MULHSQ:		sn="MULHSQ";		break;
	case BGBCC_SH_NMID_MULHUQ:		sn="MULHUQ";		break;
	case BGBCC_SH_NMID_BNDCHKB:		sn="BNDCHK.B";		break;
	case BGBCC_SH_NMID_BNDCHKW:		sn="BNDCHK.W";		break;
	case BGBCC_SH_NMID_BNDCHKL:		sn="BNDCHK.L";		break;
	case BGBCC_SH_NMID_BNDCHKQ:		sn="BNDCHK.Q";		break;
	case BGBCC_SH_NMID_EXTUTT:		sn="EXTUTT";		break;
	case BGBCC_SH_NMID_FCMPGE:		sn="FCMPGE";		break;
	case BGBCC_SH_NMID_BITSEL:		sn="BITSEL";		break;
	case BGBCC_SH_NMID_BITSELX:		sn="BITSELX";		break;

	case BGBCC_SH_NMID_FADDA:		sn="FADDA";			break;
	case BGBCC_SH_NMID_FSUBA:		sn="FSUBA";			break;
	case BGBCC_SH_NMID_FMULA:		sn="FMULA";			break;
	case BGBCC_SH_NMID_PADDXDA:		sn="PADDX.DA";		break;
	case BGBCC_SH_NMID_PSUBXDA:		sn="PSUBX.DA";		break;
	case BGBCC_SH_NMID_PMULXDA:		sn="PMULX.DA";		break;
	case BGBCC_SH_NMID_VSKG:		sn="VSKG";			break;
	case BGBCC_SH_NMID_VSKC:		sn="VSKC";			break;
	case BGBCC_SH_NMID_BNDCHK:		sn="BNDCHK";		break;
	case BGBCC_SH_NMID_BNDCMP:		sn="BNDCMP";		break;
	case BGBCC_SH_NMID_RSUBSL:		sn="RSUBS.L";		break;
	case BGBCC_SH_NMID_RSUBUL:		sn="RSUBU.L";		break;
	case BGBCC_SH_NMID_RSUB:		sn="RSUB";			break;
	case BGBCC_SH_NMID_ANDL:		sn="ANDL";			break;
	case BGBCC_SH_NMID_ORL:			sn="ORL";			break;
	case BGBCC_SH_NMID_XORL:		sn="XORL";			break;

	case BGBCC_SH_NMID_ANDW:		sn="ANDW";			break;
	case BGBCC_SH_NMID_ORW:			sn="ORW";			break;
	case BGBCC_SH_NMID_XORW:		sn="XORW";			break;
	case BGBCC_SH_NMID_SWAPQ:		sn="SWAPQ";			break;
	case BGBCC_SH_NMID_ADDSB:		sn="ADDSB";			break;
	case BGBCC_SH_NMID_ADDUB:		sn="ADDUB";			break;
	case BGBCC_SH_NMID_SUBSB:		sn="SUBSB";			break;
	case BGBCC_SH_NMID_SUBUB:		sn="SUBUB";			break;
	case BGBCC_SH_NMID_ADDSW:		sn="ADDSW";			break;
	case BGBCC_SH_NMID_ADDUW:		sn="ADDUW";			break;
	case BGBCC_SH_NMID_SUBSW:		sn="SUBSW";			break;
	case BGBCC_SH_NMID_SUBUW:		sn="SUBUW";			break;
	case BGBCC_SH_NMID_RSUBSB:		sn="RSUBSB";		break;
	case BGBCC_SH_NMID_RSUBUB:		sn="RSUBUB";		break;
	case BGBCC_SH_NMID_RSUBSW:		sn="RSUBSW";		break;
	case BGBCC_SH_NMID_RSUBUW:		sn="RSUBUW";		break;

	case BGBCC_SH_NMID_PCVTSB2HL:	sn="PCVTSB2HL";		break;
	case BGBCC_SH_NMID_PCVTUB2HL:	sn="PCVTUB2HL";		break;
	case BGBCC_SH_NMID_PCVTSB2HH:	sn="PCVTSB2HH";		break;
	case BGBCC_SH_NMID_PCVTUB2HH:	sn="PCVTHB2HH";		break;
	case BGBCC_SH_NMID_PCVTSW2FL:	sn="PCVTSW2FL";		break;
	case BGBCC_SH_NMID_PCVTUW2FL:	sn="PCVTUW2FL";		break;
	case BGBCC_SH_NMID_PCVTSW2FH:	sn="PCVTSW2FH";		break;
	case BGBCC_SH_NMID_PCVTUW2FH:	sn="PCVTUW2FH";		break;
	case BGBCC_SH_NMID_PCVTH2SB:	sn="PCVTH2SB";		break;
	case BGBCC_SH_NMID_PCVTH2UB:	sn="PCVTH2UB";		break;
	case BGBCC_SH_NMID_PCVTSW2H:	sn="PCVTSW2H";		break;
	case BGBCC_SH_NMID_PCVTUW2H:	sn="PCVTUW2H";		break;
	case BGBCC_SH_NMID_PCVTF2SW:	sn="PCVTF2SW";		break;
	case BGBCC_SH_NMID_PCVTF2UW:	sn="PCVTF2UW";		break;
	case BGBCC_SH_NMID_PCVTH2SW:	sn="PCVTH2SW";		break;
	case BGBCC_SH_NMID_PCVTH2UW:	sn="PCVTH2UW";		break;

	case BGBCC_SH_NMID_PSQRTAH:		sn="PSQRTA.H";		break;
	case BGBCC_SH_NMID_PSQRTUAH:	sn="PSQRTUA.H";		break;
	case BGBCC_SH_NMID_PSQRTAF:		sn="PSQRTA.F";		break;
	case BGBCC_SH_NMID_PSQRTUAF:	sn="PSQRTUA.F";		break;
	case BGBCC_SH_NMID_PRCPAH:		sn="PRCPA.H";		break;
	case BGBCC_SH_NMID_PRELUH:		sn="PRELU.H";		break;
	case BGBCC_SH_NMID_PRCPAF:		sn="PRCPA.F";		break;
	case BGBCC_SH_NMID_PRELUF:		sn="PRELU.F";		break;

	case BGBCC_SH_NMID_LEATB:		sn="LEAT.B";		break;
	case BGBCC_SH_NMID_LEATW:		sn="LEAT.W";		break;
	case BGBCC_SH_NMID_LEATL:		sn="LEAT.L";		break;
	case BGBCC_SH_NMID_LEATQ:		sn="LEAT.Q";		break;
	case BGBCC_SH_NMID_BNDCMPB:		sn="BNDCMP.B";		break;
	case BGBCC_SH_NMID_BNDCMPW:		sn="BNDCMP.W";		break;
	case BGBCC_SH_NMID_BNDCMPL:		sn="BNDCMP.L";		break;
	case BGBCC_SH_NMID_BNDCMPQ:		sn="BNDCMP.Q";		break;

	case BGBCC_SH_NMID_PSTCXH:		sn="PSTCX.H";		break;
	case BGBCC_SH_NMID_PLDCXH:		sn="PDLCX.H";		break;
	case BGBCC_SH_NMID_REGCHKG:		sn="REGCHKG";		break;
	case BGBCC_SH_NMID_REGCHKC:		sn="REGCHKC";		break;

	case BGBCC_SH_NMID_MULUX:		sn="MULU.X";		break;
	case BGBCC_SH_NMID_MULHUX:		sn="MULHU.X";		break;
	case BGBCC_SH_NMID_DIVUX:		sn="DIVU.X";		break;
	case BGBCC_SH_NMID_MODUX:		sn="MODU.X";		break;

	case BGBCC_SH_NMID_XMOV:		sn="XMOV";			break;	//0x01CC

	default:
		sprintf(tb, "UNK_%04X", nmid);
		sn=bgbcc_strdup(tb);
//		sn="UNKNOWN";
		break;
	}
	
	if(!strcmp(sn, "C_"))
	{
		sprintf(tb, "UNK_%04X", nmid);
		sn=bgbcc_strdup(tb);
	}
	
	if(wex2&4)
	{
		if(wex2&1)
			sprintf(tb, "%s?F", sn);
		else
			sprintf(tb, "%s?T", sn);
		sn=bgbcc_strdup(tb);
	}
	
	return(sn);
}

char *BGBCC_JX2DA_RegToName(BGBCC_JX2_Context *ctx, int nmid)
{
	char *sn;
	
//	ctx->reg_heatstat[nmid&63]++;
	
	switch(nmid)
	{
	case BGBCC_SH_REG_R0:		sn="R0";		break;	// 0x00
	case BGBCC_SH_REG_R1:		sn="R1";		break;	// 0x01
	case BGBCC_SH_REG_R2:		sn="R2";		break;	// 0x02
	case BGBCC_SH_REG_R3:		sn="R3";		break;	// 0x03
	case BGBCC_SH_REG_R4:		sn="R4";		break;	// 0x04
	case BGBCC_SH_REG_R5:		sn="R5";		break;	// 0x05
	case BGBCC_SH_REG_R6:		sn="R6";		break;	// 0x06
	case BGBCC_SH_REG_R7:		sn="R7";		break;	// 0x07
	case BGBCC_SH_REG_R8:		sn="R8";		break;	// 0x08
	case BGBCC_SH_REG_R9:		sn="R9";		break;	// 0x09
	case BGBCC_SH_REG_R10:		sn="R10";		break;	// 0x0A
	case BGBCC_SH_REG_R11:		sn="R11";		break;	// 0x0B
	case BGBCC_SH_REG_R12:		sn="R12";		break;	// 0x0C
	case BGBCC_SH_REG_R13:		sn="R13";		break;	// 0x0D
	case BGBCC_SH_REG_R14:		sn="R14";		break;	// 0x0E
	case BGBCC_SH_REG_R15:		sn="R15";		break;	// 0x0F
	case BGBCC_SH_REG_R16:		sn="R16";		break;	// 0x10
	case BGBCC_SH_REG_R17:		sn="R17";		break;	// 0x11
	case BGBCC_SH_REG_R18:		sn="R18";		break;	// 0x12
	case BGBCC_SH_REG_R19:		sn="R19";		break;	// 0x13
	case BGBCC_SH_REG_R20:		sn="R20";		break;	// 0x14
	case BGBCC_SH_REG_R21:		sn="R21";		break;	// 0x15
	case BGBCC_SH_REG_R22:		sn="R22";		break;	// 0x16
	case BGBCC_SH_REG_R23:		sn="R23";		break;	// 0x17
	case BGBCC_SH_REG_R24:		sn="R24";		break;	// 0x18	
	case BGBCC_SH_REG_R25:		sn="R25";		break;	// 0x19
	case BGBCC_SH_REG_R26:		sn="R26";		break;	// 0x1A
	case BGBCC_SH_REG_R27:		sn="R27";		break;	// 0x1B
	case BGBCC_SH_REG_R28:		sn="R28";		break;	// 0x1C
	case BGBCC_SH_REG_R29:		sn="R29";		break;	// 0x1D
	case BGBCC_SH_REG_R30:		sn="R30";		break;	// 0x1E
	case BGBCC_SH_REG_R31:		sn="R31";		break;	// 0x1F

	case BGBCC_SH_REG_R32:		sn="R32";		break;	// 0x02
	case BGBCC_SH_REG_R33:		sn="R33";		break;	// 0x03
	case BGBCC_SH_REG_R34:		sn="R34";		break;	// 0x04
	case BGBCC_SH_REG_R35:		sn="R35";		break;	// 0x05
	case BGBCC_SH_REG_R36:		sn="R36";		break;	// 0x06
	case BGBCC_SH_REG_R37:		sn="R37";		break;	// 0x07
	case BGBCC_SH_REG_R38:		sn="R38";		break;	// 0x08
	case BGBCC_SH_REG_R39:		sn="R39";		break;	// 0x09
	case BGBCC_SH_REG_R40:		sn="R40";		break;	// 0x0A
	case BGBCC_SH_REG_R41:		sn="R41";		break;	// 0x0B
	case BGBCC_SH_REG_R42:		sn="R42";		break;	// 0x0C
	case BGBCC_SH_REG_R43:		sn="R43";		break;	// 0x0D
	case BGBCC_SH_REG_R44:		sn="R44";		break;	// 0x0E
	case BGBCC_SH_REG_R45:		sn="R45";		break;	// 0x0F
	case BGBCC_SH_REG_R46:		sn="R46";		break;	// 0x10
	case BGBCC_SH_REG_R47:		sn="R47";		break;	// 0x11
	case BGBCC_SH_REG_R48:		sn="R48";		break;	// 0x12
	case BGBCC_SH_REG_R49:		sn="R49";		break;	// 0x13
	case BGBCC_SH_REG_R50:		sn="R50";		break;	// 0x14
	case BGBCC_SH_REG_R51:		sn="R51";		break;	// 0x15
	case BGBCC_SH_REG_R52:		sn="R52";		break;	// 0x16
	case BGBCC_SH_REG_R53:		sn="R53";		break;	// 0x17
	case BGBCC_SH_REG_R54:		sn="R54";		break;	// 0x18	
	case BGBCC_SH_REG_R55:		sn="R55";		break;	// 0x19
	case BGBCC_SH_REG_R56:		sn="R56";		break;	// 0x1A
	case BGBCC_SH_REG_R57:		sn="R57";		break;	// 0x1B
	case BGBCC_SH_REG_R58:		sn="R58";		break;	// 0x1C
	case BGBCC_SH_REG_R59:		sn="R59";		break;	// 0x1D
	case BGBCC_SH_REG_R60:		sn="R60";		break;	// 0x1E
	case BGBCC_SH_REG_R61:		sn="R61";		break;	// 0x1F
	case BGBCC_SH_REG_R62:		sn="R62";		break;	// 0x1F
	case BGBCC_SH_REG_R63:		sn="R63";		break;	// 0x1F

/* 20-3F: GPR RQ00-RQ31, 64 */
	case BGBCC_SH_REG_RQ0:		sn="RQ0";		break;	// 0x20
	case BGBCC_SH_REG_RQ1:		sn="RQ1";		break;	// 0x21
	case BGBCC_SH_REG_RQ2:		sn="RQ2";		break;	// 0x22
	case BGBCC_SH_REG_RQ3:		sn="RQ3";		break;	// 0x23
	case BGBCC_SH_REG_RQ4:		sn="RQ4";		break;	// 0x24
	case BGBCC_SH_REG_RQ5:		sn="RQ5";		break;	// 0x25
	case BGBCC_SH_REG_RQ6:		sn="RQ6";		break;	// 0x26
	case BGBCC_SH_REG_RQ7:		sn="RQ7";		break;	// 0x27
	case BGBCC_SH_REG_RQ8:		sn="RQ8";		break;	// 0x28
	case BGBCC_SH_REG_RQ9:		sn="RQ9";		break;	// 0x29
	case BGBCC_SH_REG_RQ10:		sn="RQ10";		break;	// 0x2A
	case BGBCC_SH_REG_RQ11:		sn="RQ11";		break;	// 0x2B
	case BGBCC_SH_REG_RQ12:		sn="RQ12";		break;	// 0x2C
	case BGBCC_SH_REG_RQ13:		sn="RQ13";		break;	// 0x2D
	case BGBCC_SH_REG_RQ14:		sn="RQ14";		break;	// 0x2E
	case BGBCC_SH_REG_RQ15:		sn="RQ15";		break;	// 0x2F
	case BGBCC_SH_REG_RQ16:		sn="RQ16";		break;	// 0x30
	case BGBCC_SH_REG_RQ17:		sn="RQ17";		break;	// 0x31
	case BGBCC_SH_REG_RQ18:		sn="RQ18";		break;	// 0x32
	case BGBCC_SH_REG_RQ19:		sn="RQ19";		break;	// 0x33
	case BGBCC_SH_REG_RQ20:		sn="RQ20";		break;	// 0x34
	case BGBCC_SH_REG_RQ21:		sn="RQ21";		break;	// 0x35
	case BGBCC_SH_REG_RQ22:		sn="RQ22";		break;	// 0x36
	case BGBCC_SH_REG_RQ23:		sn="RQ23";		break;	// 0x37
	case BGBCC_SH_REG_RQ24:		sn="RQ24";		break;	// 0x38	
	case BGBCC_SH_REG_RQ25:		sn="RQ25";		break;	// 0x39
	case BGBCC_SH_REG_RQ26:		sn="RQ26";		break;	// 0x3A
	case BGBCC_SH_REG_RQ27:		sn="RQ27";		break;	// 0x3B
	case BGBCC_SH_REG_RQ28:		sn="RQ28";		break;	// 0x3C
	case BGBCC_SH_REG_RQ29:		sn="RQ29";		break;	// 0x3D
	case BGBCC_SH_REG_RQ30:		sn="RQ30";		break;	// 0x3E
	case BGBCC_SH_REG_RQ31:		sn="RQ31";		break;	// 0x3F

	case BGBCC_SH_REG_RQ32:		sn="RQ32";		break;	// 0x02
	case BGBCC_SH_REG_RQ33:		sn="RQ33";		break;	// 0x03
	case BGBCC_SH_REG_RQ34:		sn="RQ34";		break;	// 0x04
	case BGBCC_SH_REG_RQ35:		sn="RQ35";		break;	// 0x05
	case BGBCC_SH_REG_RQ36:		sn="RQ36";		break;	// 0x06
	case BGBCC_SH_REG_RQ37:		sn="RQ37";		break;	// 0x07
	case BGBCC_SH_REG_RQ38:		sn="RQ38";		break;	// 0x08
	case BGBCC_SH_REG_RQ39:		sn="RQ39";		break;	// 0x09
	case BGBCC_SH_REG_RQ40:		sn="RQ40";		break;	// 0x0A
	case BGBCC_SH_REG_RQ41:		sn="RQ41";		break;	// 0x0B
	case BGBCC_SH_REG_RQ42:		sn="RQ42";		break;	// 0x0C
	case BGBCC_SH_REG_RQ43:		sn="RQ43";		break;	// 0x0D
	case BGBCC_SH_REG_RQ44:		sn="RQ44";		break;	// 0x0E
	case BGBCC_SH_REG_RQ45:		sn="RQ45";		break;	// 0x0F
	case BGBCC_SH_REG_RQ46:		sn="RQ46";		break;	// 0x10
	case BGBCC_SH_REG_RQ47:		sn="RQ47";		break;	// 0x11
	case BGBCC_SH_REG_RQ48:		sn="RQ48";		break;	// 0x12
	case BGBCC_SH_REG_RQ49:		sn="RQ49";		break;	// 0x13
	case BGBCC_SH_REG_RQ50:		sn="RQ50";		break;	// 0x14
	case BGBCC_SH_REG_RQ51:		sn="RQ51";		break;	// 0x15
	case BGBCC_SH_REG_RQ52:		sn="RQ52";		break;	// 0x16
	case BGBCC_SH_REG_RQ53:		sn="RQ53";		break;	// 0x17
	case BGBCC_SH_REG_RQ54:		sn="RQ54";		break;	// 0x18	
	case BGBCC_SH_REG_RQ55:		sn="RQ55";		break;	// 0x19
	case BGBCC_SH_REG_RQ56:		sn="RQ56";		break;	// 0x1A
	case BGBCC_SH_REG_RQ57:		sn="RQ57";		break;	// 0x1B
	case BGBCC_SH_REG_RQ58:		sn="RQ58";		break;	// 0x1C
	case BGBCC_SH_REG_RQ59:		sn="RQ59";		break;	// 0x1D
	case BGBCC_SH_REG_RQ60:		sn="RQ60";		break;	// 0x1E
	case BGBCC_SH_REG_RQ61:		sn="RQ61";		break;	// 0x1F
	case BGBCC_SH_REG_RQ62:		sn="RQ62";		break;	// 0x1F
	case BGBCC_SH_REG_RQ63:		sn="RQ63";		break;	// 0x1F

/* 40-5F: GPR RD00-RD31, 32 */
	case BGBCC_SH_REG_RD0:		sn="RD0";		break;	// 0x40
	case BGBCC_SH_REG_RD1:		sn="RD1";		break;	// 0x41
	case BGBCC_SH_REG_RD2:		sn="RD2";		break;	// 0x42
	case BGBCC_SH_REG_RD3:		sn="RD3";		break;	// 0x43
	case BGBCC_SH_REG_RD4:		sn="RD4";		break;	// 0x44
	case BGBCC_SH_REG_RD5:		sn="RD5";		break;	// 0x45
	case BGBCC_SH_REG_RD6:		sn="RD6";		break;	// 0x46
	case BGBCC_SH_REG_RD7:		sn="RD7";		break;	// 0x47
	case BGBCC_SH_REG_RD8:		sn="RD8";		break;	// 0x48
	case BGBCC_SH_REG_RD9:		sn="RD9";		break;	// 0x49
	case BGBCC_SH_REG_RD10:		sn="RD10";		break;	// 0x4A
	case BGBCC_SH_REG_RD11:		sn="RD11";		break;	// 0x4B
	case BGBCC_SH_REG_RD12:		sn="RD12";		break;	// 0x4C
	case BGBCC_SH_REG_RD13:		sn="RD13";		break;	// 0x4D
	case BGBCC_SH_REG_RD14:		sn="RD14";		break;	// 0x4E
	case BGBCC_SH_REG_RD15:		sn="RD15";		break;	// 0x4F
	case BGBCC_SH_REG_RD16:		sn="RD16";		break;	// 0x50
	case BGBCC_SH_REG_RD17:		sn="RD17";		break;	// 0x51
	case BGBCC_SH_REG_RD18:		sn="RD18";		break;	// 0x52
	case BGBCC_SH_REG_RD19:		sn="RD19";		break;	// 0x53
	case BGBCC_SH_REG_RD20:		sn="RD20";		break;	// 0x54
	case BGBCC_SH_REG_RD21:		sn="RD21";		break;	// 0x55
	case BGBCC_SH_REG_RD22:		sn="RD22";		break;	// 0x56
	case BGBCC_SH_REG_RD23:		sn="RD23";		break;	// 0x57
	case BGBCC_SH_REG_RD24:		sn="RD24";		break;	// 0x58	
	case BGBCC_SH_REG_RD25:		sn="RD25";		break;	// 0x59
	case BGBCC_SH_REG_RD26:		sn="RD26";		break;	// 0x5A
	case BGBCC_SH_REG_RD27:		sn="RD27";		break;	// 0x5B
	case BGBCC_SH_REG_RD28:		sn="RD28";		break;	// 0x5C
	case BGBCC_SH_REG_RD29:		sn="RD29";		break;	// 0x5D
	case BGBCC_SH_REG_RD30:		sn="RD30";		break;	// 0x5E
	case BGBCC_SH_REG_RD31:		sn="RD31";		break;	// 0x5F

	case BGBCC_SH_REG_RD32:		sn="RD32";		break;	// 0x02
	case BGBCC_SH_REG_RD33:		sn="RD33";		break;	// 0x03
	case BGBCC_SH_REG_RD34:		sn="RD34";		break;	// 0x04
	case BGBCC_SH_REG_RD35:		sn="RD35";		break;	// 0x05
	case BGBCC_SH_REG_RD36:		sn="RD36";		break;	// 0x06
	case BGBCC_SH_REG_RD37:		sn="RD37";		break;	// 0x07
	case BGBCC_SH_REG_RD38:		sn="RD38";		break;	// 0x08
	case BGBCC_SH_REG_RD39:		sn="RD39";		break;	// 0x09
	case BGBCC_SH_REG_RD40:		sn="RD40";		break;	// 0x0A
	case BGBCC_SH_REG_RD41:		sn="RD41";		break;	// 0x0B
	case BGBCC_SH_REG_RD42:		sn="RD42";		break;	// 0x0C
	case BGBCC_SH_REG_RD43:		sn="RD43";		break;	// 0x0D
	case BGBCC_SH_REG_RD44:		sn="RD44";		break;	// 0x0E
	case BGBCC_SH_REG_RD45:		sn="RD45";		break;	// 0x0F
	case BGBCC_SH_REG_RD46:		sn="RD46";		break;	// 0x10
	case BGBCC_SH_REG_RD47:		sn="RD47";		break;	// 0x11
	case BGBCC_SH_REG_RD48:		sn="RD48";		break;	// 0x12
	case BGBCC_SH_REG_RD49:		sn="RD49";		break;	// 0x13
	case BGBCC_SH_REG_RD50:		sn="RD50";		break;	// 0x14
	case BGBCC_SH_REG_RD51:		sn="RD51";		break;	// 0x15
	case BGBCC_SH_REG_RD52:		sn="RD52";		break;	// 0x16
	case BGBCC_SH_REG_RD53:		sn="RD53";		break;	// 0x17
	case BGBCC_SH_REG_RD54:		sn="RD54";		break;	// 0x18	
	case BGBCC_SH_REG_RD55:		sn="RD55";		break;	// 0x19
	case BGBCC_SH_REG_RD56:		sn="RD56";		break;	// 0x1A
	case BGBCC_SH_REG_RD57:		sn="RD57";		break;	// 0x1B
	case BGBCC_SH_REG_RD58:		sn="RD58";		break;	// 0x1C
	case BGBCC_SH_REG_RD59:		sn="RD59";		break;	// 0x1D
	case BGBCC_SH_REG_RD60:		sn="RD60";		break;	// 0x1E
	case BGBCC_SH_REG_RD61:		sn="RD61";		break;	// 0x1F
	case BGBCC_SH_REG_RD62:		sn="RD62";		break;	// 0x1F
	case BGBCC_SH_REG_RD63:		sn="RD63";		break;	// 0x1F

/* 60-6F: Control Regs */
	case BGBCC_SH_REG_SR:		sn="SR";		break;	// 0x60
	case BGBCC_SH_REG_GBR:		sn="GBR";		break;	// 0x61
	case BGBCC_SH_REG_VBR:		sn="VBR";		break;	// 0x62
	case BGBCC_SH_REG_SSR:		sn="SSR";		break;	// 0x63
	case BGBCC_SH_REG_SPC:		sn="SPC";		break;	// 0x64

	case BGBCC_SH_REG_TBR:		sn="TBR";		break;

#if 0
	case BGBCC_SH_REG_R0B:		sn="R0B";		break;	// 0x68
	case BGBCC_SH_REG_R1B:		sn="R1B";		break;	// 0x69
	case BGBCC_SH_REG_R2B:		sn="R2B";		break;	// 0x6A
	case BGBCC_SH_REG_R3B:		sn="R3B";		break;	// 0x6B
	case BGBCC_SH_REG_R4B:		sn="R4B";		break;	// 0x6C
	case BGBCC_SH_REG_R5B:		sn="R5B";		break;	// 0x6D
	case BGBCC_SH_REG_R6B:		sn="R6B";		break;	// 0x6E
	case BGBCC_SH_REG_R7B:		sn="R7B";		break;	// 0x6F
#endif

	case BGBCC_SH_REG_SP:		sn="SP";		break;	// 0x72	//S

/* 70-7F: Status Regs */
	case BGBCC_SH_REG_MACH:		sn="DLR";		break;	// 0x70	//S
	case BGBCC_SH_REG_MACL:		sn="DHR";		break;	// 0x71	//S
	case BGBCC_SH_REG_PR:		sn="LR";		break;	// 0x72	//S
	case BGBCC_SH_REG_SGR:		sn="SSP";		break;	// 0x73	//C

	case BGBCC_SH_REG_FPUL:		sn="FPUL";		break;	// 0x75
	case BGBCC_SH_REG_FPSCR:	sn="FPSCR";		break;	// 0x76
	case BGBCC_SH_REG_PC:		sn="PC";		break;	// 0x77


	case BGBCC_SH_REG_TTB:		sn="TTB";		break;	// 0x78
	case BGBCC_SH_REG_TEA:		sn="TEA";		break;	// 0x79
	case BGBCC_SH_REG_MMCR:		sn="MMCR";		break;	// 0x7A
	case BGBCC_SH_REG_EXSR:		sn="EXSR";		break;	// 0x7B

	case BGBCC_SH_REG_DBR:		sn="DBR";		break;	// 0x7F

/* 80-9F: FPR DR0-DR15, 64 */
	case BGBCC_SH_REG_DR0:		sn="DR0";		break;	// 0x80
	case BGBCC_SH_REG_DR1:		sn="DR1";		break;	// 0x81
	case BGBCC_SH_REG_DR2:		sn="DR2";		break;	// 0x82
	case BGBCC_SH_REG_DR3:		sn="DR3";		break;	// 0x83
	case BGBCC_SH_REG_DR4:		sn="DR4";		break;	// 0x84
	case BGBCC_SH_REG_DR5:		sn="DR5";		break;	// 0x85
	case BGBCC_SH_REG_DR6:		sn="DR6";		break;	// 0x86
	case BGBCC_SH_REG_DR7:		sn="DR7";		break;	// 0x87
	case BGBCC_SH_REG_DR8:		sn="DR8";		break;	// 0x88	
	case BGBCC_SH_REG_DR9:		sn="DR9";		break;	// 0x89
	case BGBCC_SH_REG_DR10:		sn="DR10";		break;	// 0x8A
	case BGBCC_SH_REG_DR11:		sn="DR11";		break;	// 0x8B
	case BGBCC_SH_REG_DR12:		sn="DR12";		break;	// 0x8C
	case BGBCC_SH_REG_DR13:		sn="DR13";		break;	// 0x8D
	case BGBCC_SH_REG_DR14:		sn="DR14";		break;	// 0x8E
	case BGBCC_SH_REG_DR15:		sn="DR15";		break;	// 0x8F
	case BGBCC_SH_REG_DR16:		sn="DR16";		break;	// 0x90
	case BGBCC_SH_REG_DR17:		sn="DR17";		break;	// 0x91
	case BGBCC_SH_REG_DR18:		sn="DR18";		break;	// 0x92
	case BGBCC_SH_REG_DR19:		sn="DR19";		break;	// 0x93
	case BGBCC_SH_REG_DR20:		sn="DR20";		break;	// 0x94
	case BGBCC_SH_REG_DR21:		sn="DR21";		break;	// 0x95
	case BGBCC_SH_REG_DR22:		sn="DR22";		break;	// 0x96
	case BGBCC_SH_REG_DR23:		sn="DR23";		break;	// 0x97
	case BGBCC_SH_REG_DR24:		sn="DR24";		break;	// 0x98	
	case BGBCC_SH_REG_DR25:		sn="DR25";		break;	// 0x99
	case BGBCC_SH_REG_DR26:		sn="DR26";		break;	// 0x9A
	case BGBCC_SH_REG_DR27:		sn="DR27";		break;	// 0x9B
	case BGBCC_SH_REG_DR28:		sn="DR28";		break;	// 0x9C
	case BGBCC_SH_REG_DR29:		sn="DR29";		break;	// 0x9D
	case BGBCC_SH_REG_DR30:		sn="DR30";		break;	// 0x9E
	case BGBCC_SH_REG_DR31:		sn="DR31";		break;	// 0x9F

/* A0-AF: FPR FR0-FR15, 32 */
	case BGBCC_SH_REG_FR0:		sn="FR0";		break;	// 0xA0
	case BGBCC_SH_REG_FR1:		sn="FR1";		break;	// 0xA1
	case BGBCC_SH_REG_FR2:		sn="FR2";		break;	// 0xA2
	case BGBCC_SH_REG_FR3:		sn="FR3";		break;	// 0xA3
	case BGBCC_SH_REG_FR4:		sn="FR4";		break;	// 0xA4
	case BGBCC_SH_REG_FR5:		sn="FR5";		break;	// 0xA5
	case BGBCC_SH_REG_FR6:		sn="FR6";		break;	// 0xA6
	case BGBCC_SH_REG_FR7:		sn="FR7";		break;	// 0xA7
	case BGBCC_SH_REG_FR8:		sn="FR8";		break;	// 0xA8
	case BGBCC_SH_REG_FR9:		sn="FR9";		break;	// 0xA9
	case BGBCC_SH_REG_FR10:		sn="FR10";		break;	// 0xAA
	case BGBCC_SH_REG_FR11:		sn="FR11";		break;	// 0xAB
	case BGBCC_SH_REG_FR12:		sn="FR12";		break;	// 0xAC
	case BGBCC_SH_REG_FR13:		sn="FR13";		break;	// 0xAD
	case BGBCC_SH_REG_FR14:		sn="FR14";		break;	// 0xAE
	case BGBCC_SH_REG_FR15:		sn="FR15";		break;	// 0xAF

	case BGBCC_SH_REG_LR0:		sn="LR0";		break;	// 0xE0
	case BGBCC_SH_REG_LR1:		sn="LR1";		break;	// 0xE1
	case BGBCC_SH_REG_LR2:		sn="LR2";		break;	// 0xE2
	case BGBCC_SH_REG_LR3:		sn="LR3";		break;	// 0xE3
	case BGBCC_SH_REG_LR4:		sn="LR4";		break;	// 0xE4
	case BGBCC_SH_REG_LR5:		sn="LR5";		break;	// 0xE5
	case BGBCC_SH_REG_LR6:		sn="LR6";		break;	// 0xE6
	case BGBCC_SH_REG_LR7:		sn="LR7";		break;	// 0xE7
	case BGBCC_SH_REG_LR8:		sn="LR8";		break;	// 0xE8
	case BGBCC_SH_REG_LR9:		sn="LR9";		break;	// 0xE9
	case BGBCC_SH_REG_LR10:		sn="LR10";		break;	// 0xEA
	case BGBCC_SH_REG_LR11:		sn="LR11";		break;	// 0xEB
	case BGBCC_SH_REG_LR12:		sn="LR12";		break;	// 0xEC
	case BGBCC_SH_REG_LR13:		sn="LR13";		break;	// 0xED
	case BGBCC_SH_REG_LR14:		sn="LR14";		break;	// 0xEE
	case BGBCC_SH_REG_LR15:		sn="LR15";		break;	// 0xEF		//Invalid
	case BGBCC_SH_REG_LR16:		sn="LR16";		break;	// 0xF0
	case BGBCC_SH_REG_LR17:		sn="LR17";		break;	// 0xF1
	case BGBCC_SH_REG_LR18:		sn="LR18";		break;	// 0xF2
	case BGBCC_SH_REG_LR19:		sn="LR19";		break;	// 0xF3
	case BGBCC_SH_REG_LR20:		sn="LR20";		break;	// 0xF4
	case BGBCC_SH_REG_LR21:		sn="LR21";		break;	// 0xF5
	case BGBCC_SH_REG_LR22:		sn="LR22";		break;	// 0xF6
	case BGBCC_SH_REG_LR23:		sn="LR23";		break;	// 0xF7
	case BGBCC_SH_REG_LR24:		sn="LR24";		break;	// 0xF8
	case BGBCC_SH_REG_LR25:		sn="LR25";		break;	// 0xF9
	case BGBCC_SH_REG_LR26:		sn="LR26";		break;	// 0xFA
	case BGBCC_SH_REG_LR27:		sn="LR27";		break;	// 0xFB
	case BGBCC_SH_REG_LR28:		sn="LR28";		break;	// 0xFC
	case BGBCC_SH_REG_LR29:		sn="LR29";		break;	// 0xFD
	case BGBCC_SH_REG_LR30:		sn="LR30";		break;	// 0xFE
	case BGBCC_SH_REG_LR31:		sn="LR31";		break;	// 0xFF		//Invalid

	default:
		sn="UNKNOWN";
		break;
	}

	if(sn[0]=='R')
	{
		ctx->reg_heatstat[nmid&63]++;
	}

	return(sn);
}

char *BGBCC_JX2DA_GetIstrSuffix(BGBCC_JX2_Context *ctx, int wex2)
{
	if(wex2&2)
	{
		if((wex2&3)==3)
			return("\t|\n  ");

		if(wex2&4)
			return("\t|\n  ");

		return("\n");
	}
	return("\n");
}

int BGBCC_JX2DA_EmitOpNone(BGBCC_JX2_Context *ctx, int nmid)
{
	char *snm;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s%s", snm,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	return(1);
}

int BGBCC_JX2DA_EmitOpReg(BGBCC_JX2_Context *ctx, int nmid, int reg)
{
	char *snm, *srn;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	if((nmid==BGBCC_SH_NMID_POP) && (reg==BGBCC_SH_REG_PC))
		return(BGBCC_JX2DA_EmitOpNone(ctx, BGBCC_SH_NMID_RET));
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);
	srn=BGBCC_JX2DA_RegToName(ctx, reg);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s %s%s", snm, srn,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	return(1);
}

int BGBCC_JX2DA_EmitOpImm(BGBCC_JX2_Context *ctx, int nmid, s64 imm)
{
	char *snm;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s %lld%s", snm, imm,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	return(1);
}

int BGBCC_JX2DA_EmitOpMReg(BGBCC_JX2_Context *ctx, int nmid, int reg)
{
	char *snm, *srn;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);
	srn=BGBCC_JX2DA_RegToName(ctx, reg);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s (%s)%s", snm, srn,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	return(1);
}

int BGBCC_JX2DA_EmitOpRegReg(BGBCC_JX2_Context *ctx, int nmid, int rm, int rn)
{
	char *snm, *srm, *srn;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);
	srm=BGBCC_JX2DA_RegToName(ctx, rm);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s %s, %s%s", snm, srm, srn,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	return(1);
}

int BGBCC_JX2DA_EmitOpRegRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rs, int rt, int rn)
{
	char *snm, *srm, *srn, *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);
	srm=BGBCC_JX2DA_RegToName(ctx, rs);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);
	sro=BGBCC_JX2DA_RegToName(ctx, rt);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s %s, %s, %s%s",
		snm, srm, sro, srn,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	return(1);
}

int BGBCC_JX2DA_EmitOpImmReg(BGBCC_JX2_Context *ctx,
	int nmid, s64 imm, int reg)
{
	char *snm, *srn;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);
	srn=BGBCC_JX2DA_RegToName(ctx, reg);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s %lld, %s%s", snm, imm, srn,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	return(1);
}

int BGBCC_JX2DA_EmitOpRegImmReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, s64 imm, int rn)
{
	char *snm, *srm, *srn;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);
	srm=BGBCC_JX2DA_RegToName(ctx, rm);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s %s, %lld, %s%s",
		snm, srm, imm, srn,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	return(1);
}


int BGBCC_JX2DA_EmitOpRegStReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	char *snm, *srm, *srn;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);
	srm=BGBCC_JX2DA_RegToName(ctx, rm);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s %s, (%s)%s", snm, srm, srn,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	return(1);
}

int BGBCC_JX2DA_EmitOpLdRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	char *snm, *srm, *srn;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);
	srm=BGBCC_JX2DA_RegToName(ctx, rm);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s (%s), %s%s",
		snm, srm, srn,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	return(1);
}

int BGBCC_JX2DA_EmitOpRegStDecReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	char *snm, *srm, *srn;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);
	srm=BGBCC_JX2DA_RegToName(ctx, rm);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s %s, (-%s)%s",
		snm, srm, srn,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	return(1);
}

int BGBCC_JX2DA_EmitOpLdIncRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	char *snm, *srm, *srn;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);
	srm=BGBCC_JX2DA_RegToName(ctx, rm);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s (%s+), %s%s",
		snm, srm, srn,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	return(1);
}

int BGBCC_JX2DA_EmitOpRegStRegDisp(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn, int disp)
{
	char *snm, *srm, *srn;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);
	srm=BGBCC_JX2DA_RegToName(ctx, rm);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s %s, (%s, %d)%s",
		snm, srm, srn, disp,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	return(1);
}

int BGBCC_JX2DA_EmitOpLdRegDispReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int disp, int rn)
{
	char *snm, *srm, *srn;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);
	srm=BGBCC_JX2DA_RegToName(ctx, rm);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s (%s, %d), %s%s",
		snm, srm, disp, srn,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	return(1);
}

int BGBCC_JX2DA_EmitOpRegStReg2(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	char *snm, *srm, *srn, *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);
	srm=BGBCC_JX2DA_RegToName(ctx, rm);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);
	sro=BGBCC_JX2DA_RegToName(ctx, ro);

	if(rn==BGBCC_SH_REG_R0)
	{
		if(ro==BGBCC_SH_REG_R0)
		{
			srn="PC";
			sro="DLR";
		}else
			if(ro==BGBCC_SH_REG_R1)
		{
			srn="DLR";
			sro="ZZR";
		}else
		{
			srn="PC";
		}
	}else
		if(rn==BGBCC_SH_REG_R1)
	{
		if(ro==BGBCC_SH_REG_R0)
		{
			srn="GBR";
			sro="DLR";
		}else
			if(ro==BGBCC_SH_REG_R1)
		{
			srn="TBR";
			sro="DLR";
		}else
		{
			srn="GBR";
		}
	}

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s %s, (%s, %s)%s",
		snm, srm, srn, sro,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	return(1);
}

int BGBCC_JX2DA_EmitOpLdReg2Reg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	char *snm, *srm, *srn, *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);
	srm=BGBCC_JX2DA_RegToName(ctx, rm);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);
	sro=BGBCC_JX2DA_RegToName(ctx, ro);

	if(rm==BGBCC_SH_REG_R0)
	{
		if(ro==BGBCC_SH_REG_R0)
		{
			srm="PC";
			sro="DLR";
		}else
			if(ro==BGBCC_SH_REG_R1)
		{
			srm="DLR";
			sro="ZZR";
		}else
		{
			srm="PC";
		}
	}else
		if(rm==BGBCC_SH_REG_R1)
	{
		if(ro==BGBCC_SH_REG_R0)
		{
			srm="GBR";
			sro="DLR";
		}else
			if(ro==BGBCC_SH_REG_R1)
		{
			srm="TBR";
			sro="DLR";
		}else
		{
			srm="GBR";
		}
	}

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s (%s, %s), %s%s",
		snm, srm, sro, srn,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	return(1);
}

int BGBCC_JX2DA_EmitOpRegStReg2Disp(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int rn, int disp)
{
	char *snm, *srm, *srn, *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);
	srm=BGBCC_JX2DA_RegToName(ctx, rm);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);
	sro=BGBCC_JX2DA_RegToName(ctx, ro);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s %s, (%s, %s, %d)%s",
		snm, srm, srn, sro, disp,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	return(1);
}

int BGBCC_JX2DA_EmitOpLdReg2DispReg(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int ro, int disp, int rn)
{
	char *snm, *srm, *srn, *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);
	srm=BGBCC_JX2DA_RegToName(ctx, rm);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);
	sro=BGBCC_JX2DA_RegToName(ctx, ro);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s (%s, %s, %d), %s%s",
		snm, srm, sro, disp, srn,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	return(1);
}

char *BGBCC_JX2DA_NameForLabel(BGBCC_JX2_Context *ctx, int lblid)
{
	char tbuf[256];
	char *s;
	
	s=BGBCC_JX2_LookupNameForLabel(ctx, lblid);
	if(s)
		return(s);

	sprintf(tbuf, ".L%08X", lblid);
	return(bgbcc_rstrdup(tbuf));
}

// int bgbcc_jx2da_lastalign;

int BGBCC_JX2DA_EmitLabel(BGBCC_JX2_Context *ctx, int lblid)
{
	char tb[256];
	char *snm, *srm, *srn, *sro;
	int lln;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
	
	if(BGBCC_JX2_LabelIsDebugLineP(ctx, lblid))
	{
		BGBCC_JX2_GetDebugLineForLabel(ctx, lblid, &sro, &lln);
		BGBPP_FetchSourceLine(sro, lln, tb);
//		BGBCC_JX2DA_EmitPrintf(ctx, "// %s:%d\n", sro, lln);
		BGBCC_JX2DA_EmitPrintf(ctx, "// %s:%d   %s\n", sro, lln, tb);
		return(0);
	}
	
	sro=BGBCC_JX2DA_NameForLabel(ctx, lblid);

//	BGBCC_JX2DA_EmitPrintf(ctx, "\nL%08X:\n", lblid);
	BGBCC_JX2DA_EmitPrintf(ctx, "\n%s:\n", sro);
	
//	bgbcc_jx2da_lastalign=-1;

	return(1);
}

int BGBCC_JX2DA_EmitOpLblReg(BGBCC_JX2_Context *ctx,
	int nmid, int lbl, int reg)
{
	char *snm, *srm, *srn, *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);
	srn=BGBCC_JX2DA_RegToName(ctx, reg);
	sro=BGBCC_JX2DA_NameForLabel(ctx, lbl);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s %s, %s%s",
		snm, sro, srn,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	return(1);
}

int BGBCC_JX2DA_EmitOpRegLbl(BGBCC_JX2_Context *ctx,
	int nmid, int lbl, int reg)
{
	char *snm, *srm, *srn, *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);
	srn=BGBCC_JX2DA_RegToName(ctx, reg);
	sro=BGBCC_JX2DA_NameForLabel(ctx, lbl);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s %s, %s%s",
		snm, srn, sro,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	return(1);
}

int BGBCC_JX2DA_EmitOpLabel(BGBCC_JX2_Context *ctx, int nmid, int lbl)
{
	char *snm, *srm, *srn, *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);
	sro=BGBCC_JX2DA_NameForLabel(ctx, lbl);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s %s%s",
		snm, sro,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	return(1);
}

int BGBCC_JX2DA_EmitLoadRegImm(
	BGBCC_JX2_Context *ctx, int nmid, int reg, s64 imm)
{
	char *snm, *srm, *srn, *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
//	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);
//	sro=BGBCC_JX2DA_NameForLabel(ctx, lbl);
	snm="MOV";
	srn=BGBCC_JX2DA_RegToName(ctx, reg);

	if(((s16)imm)==imm)
	{
		BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s %d, %s%s",
			snm, imm, srn,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
		return(1);
	}else if(((u32)imm)==imm)
	{
		BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s 0x%08X, %s%s",
			snm, imm, srn,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	}else
	{
		BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s 0x%016llX, %s%s",
			snm, imm, srn,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	}
	return(1);
}

int BGBCC_JX2DA_EmitLoadRegImm64(
	BGBCC_JX2_Context *ctx, int nmid, int reg, s64 imm)
{
	char *snm, *srm, *srn, *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
//	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);
//	sro=BGBCC_JX2DA_NameForLabel(ctx, lbl);
	snm="MOV";
	srn=BGBCC_JX2DA_RegToName(ctx, reg);

	if(((s16)imm)==imm)
	{
		BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s %d, %s%s",
			snm, imm, srn,
			BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
//	}else if((((s32)imm)==imm) && (imm>0))
	}else if(((u32)imm)==imm)
	{
		BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s 0x%08X, %s%s",
			snm, imm, srn,
			BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	}else
	{
		BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s 0x%016llX, %s%s",
			snm, imm, srn,
			BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	}
	return(1);
}

int BGBCC_JX2DA_EmitLoadRegLabelVarRel24(
	BGBCC_JX2_Context *ctx, int nmid, int reg, int lbl)
{
	char *snm, *srm, *srn, *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);
	sro=BGBCC_JX2DA_NameForLabel(ctx, lbl);
	srn=BGBCC_JX2DA_RegToName(ctx, reg);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s %s, %s%s",
		snm, sro, srn,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	return(1);
}

int BGBCC_JX2DA_EmitStoreRegLabelVarRel24(
	BGBCC_JX2_Context *ctx, int nmid, int reg, int lbl)
{
	char *snm, *srm, *srn, *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid, ctx->op_is_wex2);
	sro=BGBCC_JX2DA_NameForLabel(ctx, lbl);
	srn=BGBCC_JX2DA_RegToName(ctx, reg);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-12s %s, %s%s",
		snm, srn, sro,
		BGBCC_JX2DA_GetIstrSuffix(ctx, ctx->op_is_wex2));
	return(1);
}

void bgbcc_jx2da_tocstring(char *dst, char *src)
{
	static const char *hexstr="0123456789ABCDEF";
	char *s, *t;
	
	s=src; t=dst;
	while(*s)
	{
		if(*s<' ')
		{
			switch(*s)
			{
			case '\t':
				*t++='\\';
				*t++='t';
				break;
			case '\r':
				*t++='\\';
				*t++='r';
				break;
			case '\n':
				*t++='\\';
				*t++='n';
				break;
			default:
				*t++='\\';
				*t++='x';
				*t++=hexstr[(*s>>4)&15];
				*t++=hexstr[(*s   )&15];
				break;
			}
			s++;
			continue;
		}
		
		if(*s=='\\')
		{
			*t++='\\';
			*t++='\\';
			s++;
			continue;
		}
		
		if(*s=='\"')
		{
			*t++='\\';
			*t++='\"';
			s++;
			continue;
		}
		
		if(*s>=0x7F)
		{
			*t++='\\';
			*t++='x';
			*t++=hexstr[(*s>>4)&15];
			*t++=hexstr[(*s   )&15];
			s++;
			continue;
		}
		
		*t++=*s++;
	}
	*t++=0;
}


void bgbcc_jx2da_tocstring_ucs(char *dst, u16 *src)
{
	static const char *hexstr="0123456789ABCDEF";
	u16 *s;
	char *t;
	int ch;
	
	s=src; t=dst;
	while(*s)
	{
		ch=*s;
		if(ch<' ')
		{
			switch(ch)
			{
			case '\t':
				*t++='\\';
				*t++='t';
				break;
			case '\r':
				*t++='\\';
				*t++='r';
				break;
			case '\n':
				*t++='\\';
				*t++='n';
				break;
			default:
				*t++='\\';
				*t++='x';
				*t++=hexstr[(ch>>4)&15];
				*t++=hexstr[(ch   )&15];
				break;
			}
			s++;
			continue;
		}
		
		if(*s=='\\')
		{
			*t++='\\';
			*t++='\\';
			s++;
			continue;
		}
		
		if(*s=='\"')
		{
			*t++='\\';
			*t++='\"';
			s++;
			continue;
		}
		
		if(ch>=0x7F)
		{
			if(ch<0x100)
			{
				*t++='\\';
				*t++='x';
				*t++=hexstr[(*s>> 4)&15];
				*t++=hexstr[(*s    )&15];
			}else
			{
				*t++='\\';
				*t++='u';
				*t++=hexstr[(*s>>12)&15];
				*t++=hexstr[(*s>> 8)&15];
				*t++=hexstr[(*s>> 4)&15];
				*t++=hexstr[(*s    )&15];
			}
			s++;
			continue;
		}
		
		*t++=*s++;
	}
	*t++=0;
}

int bgbcc_strlen_ucs(u16 *src)
{
	u16 *s;
	int i;
	
	s=src; i=0;
	while(*s)
		{ i++; s++; }
	return(i);
}

int BGBCC_JX2DA_EmitString(BGBCC_JX2_Context *ctx, char *str)
{
	char tbuf[128], tb2[128];
	char *s, *t;
	int i, j, k, l, n;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);

	l=strlen(str);
	if(l>56)
	{
		s=str;
		n=l/56;
		
		for(i=0; i<n; i++)
		{
			strncpy(tb2, s, 56);
			tb2[56]=0;

			bgbcc_jx2da_tocstring(tbuf, tb2);
			BGBCC_JX2DA_EmitPrintf(ctx, "  .ascii \"%s\"\n", tbuf);
			s+=56;
		}

		bgbcc_jx2da_tocstring(tbuf, s);
		BGBCC_JX2DA_EmitPrintf(ctx, "  .asciz \"%s\"\n", tbuf);
		return(1);
	}

	bgbcc_jx2da_tocstring(tbuf, str);

//	BGBCC_JX2DA_EmitPrintf(ctx, "  .asciz \"%s\"\n", str);
	BGBCC_JX2DA_EmitPrintf(ctx, "  .asciz \"%s\"\n", tbuf);
	return(1);
}

int BGBCC_JX2DA_EmitStringUCS2(BGBCC_JX2_Context *ctx,
	u16 *str, int noz)
{
	char tbuf[128];
	u16 tb2[128];
	char *t;
	u16 *s;
	int i, j, k, l, n;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);

	l=bgbcc_strlen_ucs(str);
	if(l>56)
	{
		s=str;
		n=l/56;
		
		for(i=0; i<n; i++)
		{
//			strncpy(tb2, s, 56);
			memcpy(tb2, s, 56*2);
			tb2[56]=0;

			bgbcc_jx2da_tocstring_ucs(tbuf, tb2);
			BGBCC_JX2DA_EmitPrintf(ctx, "  .ucstr \"%s\"\n", tbuf);
			s+=56;
		}

		bgbcc_jx2da_tocstring_ucs(tbuf, s);
		if(noz)
			BGBCC_JX2DA_EmitPrintf(ctx, "  .ucstr \"%s\"\n", tbuf);
		else
			BGBCC_JX2DA_EmitPrintf(ctx, "  .ucstrz \"%s\"\n", tbuf);
		return(1);
	}

	bgbcc_jx2da_tocstring_ucs(tbuf, str);

	if(noz)
		BGBCC_JX2DA_EmitPrintf(ctx, "  .ucstr \"%s\"\n", tbuf);
	else
		BGBCC_JX2DA_EmitPrintf(ctx, "  .ucstrz \"%s\"\n", tbuf);
	return(1);
}

int BGBCC_JX2DA_EmitAscii(BGBCC_JX2_Context *ctx, char *str)
{
	char tbuf[128], tb2[128];
	char *s, *t;
	int i, j, k, l, n;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);

	l=strlen(str);
	if(l>56)
	{
		s=str;
		n=l/56;
		
		for(i=0; i<n; i++)
		{
			strncpy(tb2, s, 56);
			tb2[56]=0;

			bgbcc_jx2da_tocstring(tbuf, tb2);
			BGBCC_JX2DA_EmitPrintf(ctx, "  .ascii \"%s\"\n", tbuf);
			s+=56;
		}

		bgbcc_jx2da_tocstring(tbuf, s);
		BGBCC_JX2DA_EmitPrintf(ctx, "  .ascii \"%s\"\n", tbuf);
		return(1);
	}

	bgbcc_jx2da_tocstring(tbuf, str);

//	BGBCC_JX2DA_EmitPrintf(ctx, "  .asciz \"%s\"\n", str);
	BGBCC_JX2DA_EmitPrintf(ctx, "  .asciz \"%s\"\n", tbuf);
	return(1);
}

int BGBCC_JX2DA_EmitBAlign(BGBCC_JX2_Context *ctx, int al)
{

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);

	if(al<2)
		return(0);

	BGBCC_JX2DA_EmitPrintf(ctx, "  .balign %d\n", al);
	return(1);
}

int BGBCC_JX2DA_EmitByte(BGBCC_JX2_Context *ctx, int val)
{

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);

	BGBCC_JX2DA_EmitPrintf(ctx, "  .byte 0x%02X\n", val);
	return(1);
}

int BGBCC_JX2DA_EmitWord(BGBCC_JX2_Context *ctx, int val)
{

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);

	BGBCC_JX2DA_EmitPrintf(ctx, "  .word 0x%04X\n", val);
	return(1);
}

int BGBCC_JX2DA_EmitDWord(BGBCC_JX2_Context *ctx, u32 val)
{

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);

//	BGBCC_JX2_EmitDWordI(ctx, val);
	BGBCC_JX2DA_EmitPrintf(ctx, "  .dword 0x%08X\n", val);
	return(1);
}

int BGBCC_JX2DA_EmitQWord(BGBCC_JX2_Context *ctx, s64 val)
{

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);

//	BGBCC_JX2_EmitQWordI(ctx, val);
	BGBCC_JX2DA_EmitPrintf(ctx, "  .qword 0x%016llX\n", val);
	return(1);
}

int BGBCC_JX2DA_EmitTWord(BGBCC_JX2_Context *ctx, s64 val)
{
	if(ctx->is_simpass || !ctx->do_asm)
		return(0);

	BGBCC_JX2DA_EmitPrintf(ctx, "  .tword 0x%016llX\n", val);
	return(1);
}

int BGBCC_JX2DA_EmitDWordAbs(BGBCC_JX2_Context *ctx, int lbl)
{
	char *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);

	sro=BGBCC_JX2DA_NameForLabel(ctx, lbl);
	BGBCC_JX2DA_EmitPrintf(ctx, "  .dword %s\n", sro);
	return(1);
}

int BGBCC_JX2DA_EmitQWordAbs(BGBCC_JX2_Context *ctx, int lbl)
{
	char *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);

	sro=BGBCC_JX2DA_NameForLabel(ctx, lbl);
	BGBCC_JX2DA_EmitPrintf(ctx, "  .qword %s\n", sro);
	return(1);
}

int BGBCC_JX2DA_EmitDWordAbsDisp(BGBCC_JX2_Context *ctx, int lbl, int disp)
{
	char *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);

	sro=BGBCC_JX2DA_NameForLabel(ctx, lbl);
	BGBCC_JX2DA_EmitPrintf(ctx, "  .dword %s+%d\n", sro, disp);
	return(1);
}

int BGBCC_JX2DA_EmitQWordAbsDisp(BGBCC_JX2_Context *ctx, int lbl, int disp)
{
	char *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);

	sro=BGBCC_JX2DA_NameForLabel(ctx, lbl);
	BGBCC_JX2DA_EmitPrintf(ctx, "  .qword %s+%d\n", sro, disp);
	return(1);
}

int BGBCC_JX2DA_EmitComm(BGBCC_JX2_Context *ctx, int sz)
{
	if(ctx->is_simpass || !ctx->do_asm)
		return(0);

	BGBCC_JX2DA_EmitPrintf(ctx, "  .comm %d\n", sz);
	return(1);
}

int BGBCC_JX2DA_EmitRawBytes(BGBCC_JX2_Context *ctx, byte *buf, int sz)
{
	int i, j, k, n, l;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);

	if(buf)
	{
		n=sz/16;
		for(i=0; i<n; i++)
		{
			BGBCC_JX2DA_EmitPrintf(ctx, "  .byte 0x%02X", buf[i*16+0]);
			for(j=1; j<16; j++)
			{
				BGBCC_JX2DA_EmitPrintf(ctx, ", 0x%02X", buf[i*16+j]);
			}
			BGBCC_JX2DA_EmitPrintf(ctx, "\n");
		}
		
		k=n*16;
		l=sz-k;
		if(l>0)
		{
			BGBCC_JX2DA_EmitPrintf(ctx, "  .byte 0x%02X", buf[k+0]);
			for(j=1; j<l; j++)
			{
				BGBCC_JX2DA_EmitPrintf(ctx, ", 0x%02X", buf[k+j]);
			}
			BGBCC_JX2DA_EmitPrintf(ctx, "\n");
		}
		
	}else
	{
		BGBCC_JX2DA_EmitPrintf(ctx, "  .resb %d\n", sz);
	}

	return(1);
}
