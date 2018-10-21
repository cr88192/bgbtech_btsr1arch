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
	
	strcpy(ctx->asm_pos[ctx->sec], str);
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

char *BGBCC_JX2DA_NmidToName(BGBCC_JX2_Context *ctx, int nmid)
{
	char *sn;

	switch(nmid)
	{
	case BGBCC_SH_NMID_BRAN:	nmid=BGBCC_SH_NMID_BRA;		break;
	case BGBCC_SH_NMID_BSRN:	nmid=BGBCC_SH_NMID_BSR;		break;
	case BGBCC_SH_NMID_RTSN:	nmid=BGBCC_SH_NMID_RTS;		break;
	default:
		break;
	}
	
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
	case BGBCC_SH_NMID_CMPHS:	sn="CMP/HS";	break;	//0x0B	//CMPHS
	case BGBCC_SH_NMID_CMPHI:	sn="CMP/HI";	break;	//0x0C	//CMPHI
	case BGBCC_SH_NMID_CMPEQ:	sn="CMP/EQ";	break;	//0x0D	//CMPEQ
	case BGBCC_SH_NMID_CMPGE:	sn="CMP/GE";	break;	//0x0E	//CMPGE
	case BGBCC_SH_NMID_CMPGT:	sn="CMP/GT";	break;	//0x0F	//CMPGT
	case BGBCC_SH_NMID_JMP:		sn="JMP";		break;	//0x10	//JMP
	case BGBCC_SH_NMID_JSR:		sn="JSR";		break;	//0x11	//JSR
	case BGBCC_SH_NMID_BRA:		sn="BRA";		break;	//0x12	//BRA
	case BGBCC_SH_NMID_BSR:		sn="BSR";		break;	//0x13	//BSR
	case BGBCC_SH_NMID_BT:		sn="BT";		break;	//0x14	//BT
	case BGBCC_SH_NMID_BF:		sn="BT";		break;	//0x15	//BF
	case BGBCC_SH_NMID_BTS:		sn="BTS";		break;	//0x16	//BTS
	case BGBCC_SH_NMID_BFS:		sn="BFS";		break;	//0x17	//BFS
	case BGBCC_SH_NMID_DIV1:	sn="DIV1";		break;	//0x18	//DIV1
	case BGBCC_SH_NMID_DMULU:	sn="DMULU";		break;	//0x19	//DMULU
	case BGBCC_SH_NMID_DMULS:	sn="DMULS";		break;	//0x1A	//DMULS
	case BGBCC_SH_NMID_TST:		sn="TST";		break;	//0x1B	//TST
	case BGBCC_SH_NMID_AND:		sn="AND";		break;	//0x1C	//AND
	case BGBCC_SH_NMID_XOR:		sn="XOR";		break;	//0x1D	//XOR
	case BGBCC_SH_NMID_OR:		sn="OR";		break;	//0x1E	//OR
	case BGBCC_SH_NMID_DIV0S:	sn="DIV0S";		break;	//0x20	//
	case BGBCC_SH_NMID_CMPSTR:	sn="CMP/STR";	break;	//0x21	//
	case BGBCC_SH_NMID_MULUW:	sn="MULU.W";	break;	//0x22	//
	case BGBCC_SH_NMID_MULSW:	sn="MULS.W";	break;	//0x23	//
	case BGBCC_SH_NMID_MACL:	sn="MAC.L";		break;	//0x24	//
	case BGBCC_SH_NMID_BSRF:	sn="BSRF";		break;	//0x25	//
	case BGBCC_SH_NMID_BRAF:	sn="BRAF";		break;	//0x26	//
	case BGBCC_SH_NMID_MULL:	sn="MUL.L";		break;	//0x27	//
	case BGBCC_SH_NMID_CLRT:	sn="CLRT";		break;	//0x28	//
	case BGBCC_SH_NMID_SETT:	sn="SETT";		break;	//0x29	//
	case BGBCC_SH_NMID_CLRMAC:	sn="CLRMAC";	break;	//0x2A	//
	case BGBCC_SH_NMID_NOP:		sn="NOP";		break;	//0x2B	//
	case BGBCC_SH_NMID_MOVT:	sn="MOVT";		break;	//0x2C	//
	case BGBCC_SH_NMID_RTS:		sn="RTS";		break;	//0x2D	//
	case BGBCC_SH_NMID_SLEEP:	sn="SLEEP";		break;	//0x2E	//
	case BGBCC_SH_NMID_RTE:		sn="RTE";		break;	//0x2F	//
	case BGBCC_SH_NMID_SHLL:	sn="SHLL";		break;	//0x30	//
	case BGBCC_SH_NMID_DT:		sn="DT";		break;	//0x31	//
	case BGBCC_SH_NMID_SHAL:	sn="SHAL";		break;	//0x32	//
	case BGBCC_SH_NMID_SHLR:	sn="SHLR";		break;	//0x33	//
	case BGBCC_SH_NMID_CMPPZ:	sn="CMP/PZ";	break;	//0x34	//
	case BGBCC_SH_NMID_SHAR:	sn="SHAR";		break;	//0x35	//
	case BGBCC_SH_NMID_ROTL:	sn="ROTL";		break;	//0x36	//
	case BGBCC_SH_NMID_ROTCL:	sn="ROTCL";		break;	//0x37	//
	case BGBCC_SH_NMID_ROTT:	sn="ROTT";		break;	//0x38	//
	case BGBCC_SH_NMID_ROTCR:	sn="ROTCR";		break;	//0x39	//
	case BGBCC_SH_NMID_SHLL2:	sn="SHLL2";		break;	//0x3A	//
	case BGBCC_SH_NMID_ROTR:	sn="ROTR";		break;	//0x3B	//
	case BGBCC_SH_NMID_CMPPL:	sn="CMP/PL";	break;	//0x3C	//
	case BGBCC_SH_NMID_SHLL8:	sn="SHLL8";		break;	//0x3D	//
	case BGBCC_SH_NMID_SHLL16:	sn="SHLL16";	break;	//0x3E	//
	case BGBCC_SH_NMID_SHLR2:	sn="SHLR2";		break;	//0x3F	//
	case BGBCC_SH_NMID_SHLR8:	sn="SHLR8";		break;	//0x40	//
	case BGBCC_SH_NMID_SHLR16:	sn="SHLR16";	break;	//0x41	//
	case BGBCC_SH_NMID_MACW:	sn="MAC.W";		break;	//0x42	//
	case BGBCC_SH_NMID_NOT:		sn="NOT";		break;	//0x43	//
	case BGBCC_SH_NMID_SWAPB:	sn="SWAP.B";	break;	//0x44	//
	case BGBCC_SH_NMID_SWAPW:	sn="SWAP.W";	break;	//0x45	//
	case BGBCC_SH_NMID_NEGC:	sn="NEGC";		break;	//0x46	//
	case BGBCC_SH_NMID_NEG:		sn="NEG";		break;	//0x47	//
	case BGBCC_SH_NMID_EXTUB:	sn="EXTU.B";	break;	//0x48	//
	case BGBCC_SH_NMID_EXTUW:	sn="EXTU.W";	break;	//0x49	//
	case BGBCC_SH_NMID_EXTSB:	sn="EXTS.B";	break;	//0x4A	//
	case BGBCC_SH_NMID_EXTSW:	sn="EXTS.W";	break;	//0x4B	//
	case BGBCC_SH_NMID_TRAPA:	sn="TRAPA";		break;	//0x4C	//
	case BGBCC_SH_NMID_MOVA:	sn="MOVA";		break;	//0x4D	//
	case BGBCC_SH_NMID_TSTB:	sn="TSTB";		break;	//0x4E	//
	case BGBCC_SH_NMID_DIV0U:	sn="DIV0U";		break;	//0x4F	//
	case BGBCC_SH_NMID_LDC:		sn="LDC";		break;	//0x50	//
	case BGBCC_SH_NMID_LDCL:	sn="LDC.L";		break;	//0x51	//
	case BGBCC_SH_NMID_LDS:		sn="LDS";		break;	//0x52	//
	case BGBCC_SH_NMID_LDSL:	sn="LDS.L";		break;	//0x53	//
	case BGBCC_SH_NMID_STC:		sn="STC";		break;	//0x54	//
	case BGBCC_SH_NMID_STCL:	sn="STC.L";		break;	//0x55	//
	case BGBCC_SH_NMID_STS:		sn="STS";		break;	//0x56	//
	case BGBCC_SH_NMID_STSL:	sn="STS.L";		break;	//0x57	//
	case BGBCC_SH_NMID_CASL:	sn="CAS.L";		break;	//0x58	//
	case BGBCC_SH_NMID_TASB:	sn="TAS.B";		break;	//0x59	//
	case BGBCC_SH_NMID_XTRCT:	sn="XTRCT";		break;	//0x5A	//
	case BGBCC_SH_NMID_ANDB:	sn="AND.B";		break;	//0x5B	//
	case BGBCC_SH_NMID_XORB:	sn="XOR.B";		break;	//0x5C	//
	case BGBCC_SH_NMID_ORB:		sn="OR.B";		break;	//0x5D	//
	case BGBCC_SH_NMID_SHAD:	sn="SHAD";		break;	//0x5E	//
	case BGBCC_SH_NMID_SHLD:	sn="SHLD";		break;	//0x5F	//
	case BGBCC_SH_NMID_CLRS:	sn="CLRS";		break;	//0x61	//
	case BGBCC_SH_NMID_SETS:	sn="SETS";		break;	//0x62	//
	case BGBCC_SH_NMID_MOVUB:	sn="MOVU.B";	break;	//0x65	//MOVU.B
	case BGBCC_SH_NMID_MOVUW:	sn="MOVU.W";	break;	//0x66	//MOVU.W
	case BGBCC_SH_NMID_MOVUL:	sn="MOVU.L";	break;	//0x67	//MOVU.L
	case BGBCC_SH_NMID_LEAB:	sn="LEA.B";		break;	//0x68	//LEA.B
	case BGBCC_SH_NMID_LEAW:	sn="LEA.W";		break;	//0x69	//LEA.W
	case BGBCC_SH_NMID_LEAL:	sn="LEA.L";		break;	//0x6A	//LEA.L
	case BGBCC_SH_NMID_LEAQ:	sn="LEA.Q";		break;	//0x6B	//LEA.Q
	case BGBCC_SH_NMID_LEAO:	sn="LEA.O";		break;	//0x6C	//LEA.O
	case BGBCC_SH_NMID_NOTT:	sn="NOTT";		break;	//0x6D	//
	case BGBCC_SH_NMID_LDTLB:	sn="LDTLB";		break;	//0x6E	//
	case BGBCC_SH_NMID_BRK:		sn="BRK";		break;	//0x6F	//
	case BGBCC_SH_NMID_MOVRT:	sn="MOVRT";		break;	//0x70	//
	case BGBCC_SH_NMID_MOVCAL:	sn="MOVCAL";	break;	//0x71	//
	case BGBCC_SH_NMID_PUSH:	sn="PUSH";		break;	//0x72	//
	case BGBCC_SH_NMID_POP:		sn="POP";		break;	//0x73	//
	case BGBCC_SH_NMID_ICLRMD:	sn="ICLRMD";	break;	//0x74	//
	case BGBCC_SH_NMID_ISETMD:	sn="ISETMD";	break;	//0x75	//
	case BGBCC_SH_NMID_SHLL1:	sn="SHLL1";		break;	//0x76	//
	case BGBCC_SH_NMID_LDHF16:	sn="LDHF16";	break;	//0x77	//
	case BGBCC_SH_NMID_BRAN:	sn="BRA/N";		break;	//0x78	//BRA/N
	case BGBCC_SH_NMID_BSRN:	sn="BSR/N";		break;	//0x79	//BSR/N
	case BGBCC_SH_NMID_RTSN:	sn="RTS/N";		break;	//0x7A	//
	case BGBCC_SH_NMID_LDIF16:	sn="LDIF16";	break;	//0x7B	//
	case BGBCC_SH_NMID_STHF16:	sn="STHF16";	break;	//0x7C	//
	case BGBCC_SH_NMID_CSELT:	sn="CSELT";		break;	//0x7D	//
	case BGBCC_SH_NMID_EXTUL:	sn="EXTU.L";	break;	//0x7E	//
	case BGBCC_SH_NMID_EXTSL:	sn="EXTS.L";	break;	//0x7F	//

	case BGBCC_SH_NMID_FABS:	sn="FABS";		break;	//0x80	//
	case BGBCC_SH_NMID_FADD:	sn="FADD";		break;	//0x81	//
	case BGBCC_SH_NMID_FCMPEQ:	sn="FCMP/EQ";	break;	//0x82	//
	case BGBCC_SH_NMID_FCMPGT:	sn="FCMP/GT";	break;	//0x83	//
	case BGBCC_SH_NMID_FCNVDS:	sn="FCNVDS";	break;	//0x84	//
	case BGBCC_SH_NMID_FCNVSD:	sn="FCNVSD";	break;	//0x85	//
	case BGBCC_SH_NMID_FDIV:	sn="FDIV";		break;	//0x86	//
	case BGBCC_SH_NMID_FLDI0:	sn="FLDI0";		break;	//0x87	//
	case BGBCC_SH_NMID_FLDI1:	sn="FLDI1";		break;	//0x88	//
	case BGBCC_SH_NMID_FLDS:	sn="FLDS";		break;	//0x89	//
	case BGBCC_SH_NMID_FLOAT:	sn="FLOAT";		break;	//0x8A	//
	case BGBCC_SH_NMID_FMAC:	sn="FMAC";		break;	//0x8B	//
	case BGBCC_SH_NMID_FMOV:	sn="FMOV";		break;	//0x8C	//
	case BGBCC_SH_NMID_FMOVS:	sn="FMOV.S";	break;	//0x8D	//
	case BGBCC_SH_NMID_FMOVD:	sn="FMOV.D";	break;	//0x8E	//
	case BGBCC_SH_NMID_FMUL:	sn="FMUL";		break;	//0x8F	//
	case BGBCC_SH_NMID_FNEG:	sn="FNEG";		break;	//0x90	//
	case BGBCC_SH_NMID_FSCHG:	sn="FSCHG";		break;	//0x91	//
	case BGBCC_SH_NMID_FSQRT:	sn="FSQRT";		break;	//0x92	//
	case BGBCC_SH_NMID_FSTS:	sn="FSTS";		break;	//0x93	//
	case BGBCC_SH_NMID_FSUB:	sn="FSUB";		break;	//0x94	//
	case BGBCC_SH_NMID_FTRC:	sn="FTRC";		break;	//0x95	//
	case BGBCC_SH_NMID_FSRRA:	sn="FSRRA";		break;	//0x96	//
	case BGBCC_SH_NMID_FIPR:	sn="FIPR";		break;	//0x97	//
	case BGBCC_SH_NMID_FTRV:	sn="FTRV";		break;	//0x98	//
	case BGBCC_SH_NMID_FMOVX:	sn="FMOVX";		break;	//0x9B	//
	case BGBCC_SH_NMID_FMOVIS:	sn="FMOVIS";	break;	//0x9C	//
	case BGBCC_SH_NMID_FMOVID:	sn="FMOVID";	break;	//0x9D	//
	case BGBCC_SH_NMID_FMOVSI:	sn="FMOVSI";	break;	//0x9E	//
	case BGBCC_SH_NMID_FMOVDI:	sn="FMOVDI";	break;	//0x9F	//

	case BGBCC_SH_NMID_CMOVTB:	sn="CMOVT.B";	break;	//0xA0	//CMOVT.B
	case BGBCC_SH_NMID_CMOVTW:	sn="CMOVT.W";	break;	//0xA1	//CMOVT.W
	case BGBCC_SH_NMID_CMOVTL:	sn="CMOVT.L";	break;	//0xA2	//CMOVT.L
	case BGBCC_SH_NMID_CMOVTQ:	sn="CMOVT.Q";	break;	//0xA3	//CMOVT.Q
	case BGBCC_SH_NMID_CMOVFB:	sn="CMOVF.B";	break;	//0xA4	//CMOVF.B
	case BGBCC_SH_NMID_CMOVFW:	sn="CMOVF.W";	break;	//0xA5	//CMOVF.W
	case BGBCC_SH_NMID_CMOVFL:	sn="CMOVF.L";	break;	//0xA6	//CMOVF.L
	case BGBCC_SH_NMID_CMOVFQ:	sn="CMOVF.Q";	break;	//0xA7	//CMOVF.Q

	case BGBCC_SH_NMID_FLDCF:	sn="FLDCF";		break;	//0xA8	//
	case BGBCC_SH_NMID_FLDCD:	sn="FLDCD";		break;	//0xA9	//
	case BGBCC_SH_NMID_FLDCI:	sn="FLDCI";		break;	//0xAA	//
	case BGBCC_SH_NMID_FLDCH:	sn="FLDCH";		break;	//0xAB	//
	case BGBCC_SH_NMID_FSTCF:	sn="FSTCF";		break;	//0xAC	//
	case BGBCC_SH_NMID_FSTCD:	sn="FSTCD";		break;	//0xAD	//
	case BGBCC_SH_NMID_FSTCI:	sn="FSTCI";		break;	//0xAE	//
	case BGBCC_SH_NMID_FSTCH:	sn="FSTCH";		break;	//0xAF	//
	case BGBCC_SH_NMID_FRCPA:	sn="FRCPA";		break;	//0xB0	//
	case BGBCC_SH_NMID_FSQRTA:	sn="FSQRTA";	break;	//0xB1	//
	case BGBCC_SH_NMID_FRCP:	sn="FRCP";		break;	//0xB2	//

	case BGBCC_SH_NMID_BSR2F:	sn="BSR2F";		break;	//0xBA	//
	case BGBCC_SH_NMID_BRA2F:	sn="BRA2F";		break;	//0xBB	//
	case BGBCC_SH_NMID_MOVDL:	sn="MOVD.L";	break;	//0xBC	//MOVD.L
	case BGBCC_SH_NMID_RET:		sn="RET";		break;	//0xBD	//
	case BGBCC_SH_NMID_PUSHX2:	sn="PUSHX2";	break;	//0xBE	//
	case BGBCC_SH_NMID_POPX2:	sn="POPX2";		break;	//0xBF	//

	case BGBCC_SH_NMID_MOVI:	sn="MOVI";		break;	//0xC0	//
	case BGBCC_SH_NMID_MOVIV:	sn="MOVIV";		break;	//0xC1	//
	case BGBCC_SH_NMID_PREF:	sn="PREF";		break;	//0xC2	//
	case BGBCC_SH_NMID_OCBI:	sn="OCBI";		break;	//0xC3	//
	case BGBCC_SH_NMID_OCBP:	sn="OCBP";		break;	//0xC4	//
	case BGBCC_SH_NMID_OCBWB:	sn="OCBWB";		break;	//0xC5	//
	case BGBCC_SH_NMID_ICBI:	sn="ICBI";		break;	//0xC6	//
	case BGBCC_SH_NMID_MOVQ:	sn="MOV.Q";		break;	//0xC7	//MOV.Q
	case BGBCC_SH_NMID_ADDQ:	sn="ADD.Q";		break;	//0xC8	//ADD
	case BGBCC_SH_NMID_SUBQ:	sn="SUB.Q";		break;	//0xC9	//SUB
	case BGBCC_SH_NMID_MULQ:	sn="MUL.Q";		break;	//0xCA	//SUB
	case BGBCC_SH_NMID_ANDQ:	sn="AND.Q";		break;	//0xCB	//SUB
	case BGBCC_SH_NMID_ORQ:		sn="OR.Q";		break;	//0xCC	//SUB
	case BGBCC_SH_NMID_XORQ:	sn="XOR.Q";		break;	//0xCD	//SUB
	case BGBCC_SH_NMID_SHLLQ:	sn="SHLL.Q";	break;	//0xCE	//SUB
	case BGBCC_SH_NMID_SHLRQ:	sn="SHLR.Q";	break;	//0xCF	//SUB
	case BGBCC_SH_NMID_SHALQ:	sn="SHAL.Q";	break;	//0xD0	//SUB
	case BGBCC_SH_NMID_SHARQ:	sn="SHAR.Q";	break;	//0xD1	//SUB
	case BGBCC_SH_NMID_LDSH16:	sn="LDSH16";	break;	//0xD2	//SUB
	case BGBCC_SH_NMID_TSTQ:	sn="TST.Q";		break;	//0xD3	//SUB
	case BGBCC_SH_NMID_SHADQ:	sn="SHAD.Q";	break;	//0xD4	//
	case BGBCC_SH_NMID_SHLDQ:	sn="SHLD.Q";	break;	//0xD5	//
	case BGBCC_SH_NMID_SHLL4:	sn="SHLL4";		break;	//0xD6	//
	case BGBCC_SH_NMID_SHLR4:	sn="SHLR4";		break;	//0xD7	//
	case BGBCC_SH_NMID_BREQ:	sn="BR/EQ";		break;	//0xD8	//
	case BGBCC_SH_NMID_BRNE:	sn="BR/NE";		break;	//0xD9	//
	case BGBCC_SH_NMID_BRGT:	sn="BR/GT";		break;	//0xDA	//
	case BGBCC_SH_NMID_BRLE:	sn="BR/LE";		break;	//0xDB	//
	case BGBCC_SH_NMID_BRGE:	sn="BR/GE";		break;	//0xDC	//
	case BGBCC_SH_NMID_BRLT:	sn="BR/LT";		break;	//0xDD	//
	case BGBCC_SH_NMID_CMPQHS:	sn="CMPQ/HS";	break;	//0xE0	//CMPQ/HS
	case BGBCC_SH_NMID_CMPQHI:	sn="CMPQ/HI";	break;	//0xE1	//CMPQ/HI
	case BGBCC_SH_NMID_CMPQEQ:	sn="CMPQ/EQ";	break;	//0xE2	//CMPQ/EQ
	case BGBCC_SH_NMID_CMPQGE:	sn="CMPQ/GE";	break;	//0xE3	//CMPQ/GE
	case BGBCC_SH_NMID_CMPQGT:	sn="CMPQ/GT";	break;	//0xE4	//CMPQ/GT
	case BGBCC_SH_NMID_CMPQPZ:	sn="CMPQ/PZ";	break;	//0xE5	//CMPQ/PZ
	case BGBCC_SH_NMID_CMPQPL:	sn="CMPQ/PL";	break;	//0xE6	//CMPQ/PL
	case BGBCC_SH_NMID_NOTS:	sn="NOTS";		break;	//0xE7	//
	case BGBCC_SH_NMID_SHLL32:	sn="SHLL32";	break;	//0xE8	//
	case BGBCC_SH_NMID_SHLR32:	sn="SHLR32";	break;	//0xE9	//
	case BGBCC_SH_NMID_SHLR1:	sn="SHLR1";		break;	//0xEA	//
	case BGBCC_SH_NMID_SHAR1:	sn="SHAR1";		break;	//0xEB	//
	case BGBCC_SH_NMID_SHAR2:	sn="SHAR2";		break;	//0xEC	//
	case BGBCC_SH_NMID_SHAR4:	sn="SHAR4";		break;	//0xED	//
	case BGBCC_SH_NMID_SHAR8:	sn="SHAR8";		break;	//0xEE	//
	case BGBCC_SH_NMID_SHAR16:	sn="SHAR16";	break;	//0xEF	//
	case BGBCC_SH_NMID_SHAR32:	sn="SHAR32";	break;	//0xF0	//
	case BGBCC_SH_NMID_LDSH8:	sn="LDSH8";		break;	//0xF1	//SUB
	case BGBCC_SH_NMID_BRA8B:	sn="BRA8B";		break;	//0xF2	//SUB
	case BGBCC_SH_NMID_SHARX:	sn="SHARX";		break;	//0xF3	//SUB
	case BGBCC_SH_NMID_MOVNT:	sn="MOVNT";		break;	//0xF4	//
	case BGBCC_SH_NMID_CLZ:		sn="CLZ";		break;	//0xF5	//
	case BGBCC_SH_NMID_CLZQ:	sn="CLZQ";		break;	//0xF6	//

	default:
		sn="UNKNOWN";
		break;
	}
	return(sn);
}

char *BGBCC_JX2DA_RegToName(BGBCC_JX2_Context *ctx, int nmid)
{
	char *sn;
	
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

/* 60-6F: Control Regs */
	case BGBCC_SH_REG_SR:		sn="SR";		break;	// 0x60
	case BGBCC_SH_REG_GBR:		sn="GBR";		break;	// 0x61
	case BGBCC_SH_REG_VBR:		sn="VBR";		break;	// 0x62
	case BGBCC_SH_REG_SSR:		sn="SSR";		break;	// 0x63
	case BGBCC_SH_REG_SPC:		sn="SPC";		break;	// 0x64

	case BGBCC_SH_REG_R0B:		sn="R0B";		break;	// 0x68
	case BGBCC_SH_REG_R1B:		sn="R1B";		break;	// 0x69
	case BGBCC_SH_REG_R2B:		sn="R2B";		break;	// 0x6A
	case BGBCC_SH_REG_R3B:		sn="R3B";		break;	// 0x6B
	case BGBCC_SH_REG_R4B:		sn="R4B";		break;	// 0x6C
	case BGBCC_SH_REG_R5B:		sn="R5B";		break;	// 0x6D
	case BGBCC_SH_REG_R6B:		sn="R6B";		break;	// 0x6E
	case BGBCC_SH_REG_R7B:		sn="R7B";		break;	// 0x6F

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
	return(sn);
}

int BGBCC_JX2DA_EmitOpNone(BGBCC_JX2_Context *ctx, int nmid)
{
	char *snm;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s\n", snm);
	return(1);
}

int BGBCC_JX2DA_EmitOpReg(BGBCC_JX2_Context *ctx, int nmid, int reg)
{
	char *snm, *srn;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	if((nmid==BGBCC_SH_NMID_POP) && (reg==BGBCC_SH_REG_PC))
		return(BGBCC_JX2DA_EmitOpNone(ctx, BGBCC_SH_NMID_RET));
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid);
	srn=BGBCC_JX2DA_RegToName(ctx, reg);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s %s\n", snm, srn);
	return(1);
}

int BGBCC_JX2DA_EmitOpImm(BGBCC_JX2_Context *ctx, int nmid, int imm)
{
	char *snm;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s #%d\n", snm, imm);
	return(1);
}

int BGBCC_JX2DA_EmitOpMReg(BGBCC_JX2_Context *ctx, int nmid, int reg)
{
	char *snm, *srn;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid);
	srn=BGBCC_JX2DA_RegToName(ctx, reg);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s @%s\n", snm, srn);
	return(1);
}

int BGBCC_JX2DA_EmitOpRegReg(BGBCC_JX2_Context *ctx, int nmid, int rm, int rn)
{
	char *snm, *srm, *srn;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid);
	srm=BGBCC_JX2DA_RegToName(ctx, rm);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s %s, %s\n", snm, srm, srn);
	return(1);
}

int BGBCC_JX2DA_EmitOpRegRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rs, int rt, int rn)
{
	char *snm, *srm, *srn, *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid);
	srm=BGBCC_JX2DA_RegToName(ctx, rs);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);
	sro=BGBCC_JX2DA_RegToName(ctx, rt);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s %s, %s, %s\n", snm, srm, sro, srn);
	return(1);
}

int BGBCC_JX2DA_EmitOpImmReg(BGBCC_JX2_Context *ctx,
	int nmid, int imm, int reg)
{
	char *snm, *srn;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid);
	srn=BGBCC_JX2DA_RegToName(ctx, reg);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s #%d, %s\n", snm, imm, srn);
	return(1);
}

int BGBCC_JX2DA_EmitOpRegImmReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int imm, int rn)
{
	char *snm, *srm, *srn;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid);
	srm=BGBCC_JX2DA_RegToName(ctx, rm);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s %s, #%d, %s\n", snm, srm, imm, srn);
	return(1);
}


int BGBCC_JX2DA_EmitOpRegStReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	char *snm, *srm, *srn;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid);
	srm=BGBCC_JX2DA_RegToName(ctx, rm);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s %s, @%s\n", snm, srm, srn);
	return(1);
}

int BGBCC_JX2DA_EmitOpLdRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	char *snm, *srm, *srn;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid);
	srm=BGBCC_JX2DA_RegToName(ctx, rm);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s @%s, %s\n", snm, srm, srn);
	return(1);
}

int BGBCC_JX2DA_EmitOpRegStDecReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	char *snm, *srm, *srn;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid);
	srm=BGBCC_JX2DA_RegToName(ctx, rm);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s %s, @-%s\n", snm, srm, srn);
	return(1);
}

int BGBCC_JX2DA_EmitOpLdIncRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	char *snm, *srm, *srn;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid);
	srm=BGBCC_JX2DA_RegToName(ctx, rm);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s @%s+, %s\n", snm, srm, srn);
	return(1);
}

int BGBCC_JX2DA_EmitOpRegStRegDisp(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn, int disp)
{
	char *snm, *srm, *srn;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid);
	srm=BGBCC_JX2DA_RegToName(ctx, rm);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s %s, @(%s, %d)\n", snm, srm, srn, disp);
	return(1);
}

int BGBCC_JX2DA_EmitOpLdRegDispReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int disp, int rn)
{
	char *snm, *srm, *srn;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid);
	srm=BGBCC_JX2DA_RegToName(ctx, rm);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s @(%s, %d), %s\n", snm, srm, disp, srn);
	return(1);
}

int BGBCC_JX2DA_EmitOpRegStReg2(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	char *snm, *srm, *srn, *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid);
	srm=BGBCC_JX2DA_RegToName(ctx, rm);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);
	sro=BGBCC_JX2DA_RegToName(ctx, ro);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s %s, @(%s, %s)\n", snm, srm, srn, sro);
	return(1);
}

int BGBCC_JX2DA_EmitOpLdReg2Reg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	char *snm, *srm, *srn, *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid);
	srm=BGBCC_JX2DA_RegToName(ctx, rm);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);
	sro=BGBCC_JX2DA_RegToName(ctx, ro);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s @(%s, %s), %s\n", snm, srm, sro, srn);
	return(1);
}

int BGBCC_JX2DA_EmitOpRegStReg2Disp(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int rn, int disp)
{
	char *snm, *srm, *srn, *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid);
	srm=BGBCC_JX2DA_RegToName(ctx, rm);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);
	sro=BGBCC_JX2DA_RegToName(ctx, ro);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s %s, @(%s, %s, %d)\n",
		snm, srm, srn, sro, disp);
	return(1);
}

int BGBCC_JX2DA_EmitOpLdReg2DispReg(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int ro, int disp, int rn)
{
	char *snm, *srm, *srn, *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid);
	srm=BGBCC_JX2DA_RegToName(ctx, rm);
	srn=BGBCC_JX2DA_RegToName(ctx, rn);
	sro=BGBCC_JX2DA_RegToName(ctx, ro);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s @(%s, %s, %d), %s\n",
		snm, srm, sro, disp, srn);
	return(1);
}

char *BGBCC_JX2DA_NameForLabel(BGBCC_JX2_Context *ctx, int lblid)
{
	char tbuf[256];
	char *s;
	
	s=BGBCC_JX2_LookupNameForLabel(ctx, lblid);
	if(s)
		return(s);

	sprintf(tbuf, "L%08X", lblid);
	return(bgbcc_rstrdup(tbuf));
}

// int bgbcc_jx2da_lastalign;

int BGBCC_JX2DA_EmitLabel(BGBCC_JX2_Context *ctx, int lblid)
{
	char *snm, *srm, *srn, *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
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
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid);
	srn=BGBCC_JX2DA_RegToName(ctx, reg);
	sro=BGBCC_JX2DA_NameForLabel(ctx, lbl);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s %s, %s\n",
		snm, sro, srn);
	return(1);
}

int BGBCC_JX2DA_EmitOpLabel(BGBCC_JX2_Context *ctx, int nmid, int lbl)
{
	char *snm, *srm, *srn, *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid);
	sro=BGBCC_JX2DA_NameForLabel(ctx, lbl);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s %s\n",
		snm, sro);
	return(1);
}

int BGBCC_JX2DA_EmitLoadRegImm(
	BGBCC_JX2_Context *ctx, int nmid, int reg, s32 imm)
{
	char *snm, *srm, *srn, *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
//	snm=BGBCC_JX2DA_NmidToName(ctx, nmid);
//	sro=BGBCC_JX2DA_NameForLabel(ctx, lbl);
	snm="MOV";
	srn=BGBCC_JX2DA_RegToName(ctx, reg);

	if(((s16)imm)==imm)
	{
		BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s #%d, %s\n",
			snm, imm, srn);
		return(1);
	}else if(((u32)imm)==imm)
	{
		BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s #0x%08X, %s\n",
			snm, imm, srn);
	}else
	{
		BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s #0x%016llX, %s\n",
			snm, imm, srn);
	}
	return(1);
}

int BGBCC_JX2DA_EmitLoadRegImm64(
	BGBCC_JX2_Context *ctx, int nmid, int reg, s32 imm)
{
	char *snm, *srm, *srn, *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
//	snm=BGBCC_JX2DA_NmidToName(ctx, nmid);
//	sro=BGBCC_JX2DA_NameForLabel(ctx, lbl);
	snm="MOV";
	srn=BGBCC_JX2DA_RegToName(ctx, reg);

	if(((s16)imm)==imm)
	{
		BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s #%d, %s\n",
			snm, imm, srn);
//	}else if((((s32)imm)==imm) && (imm>0))
	}else if(((u32)imm)==imm)
	{
		BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s #0x%08X, %s\n",
			snm, imm, srn);
	}else
	{
		BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s #0x%016llX, %s\n",
			snm, imm, srn);
	}
	return(1);
}

int BGBCC_JX2DA_EmitLoadRegLabelVarRel24(
	BGBCC_JX2_Context *ctx, int nmid, int reg, int lbl)
{
	char *snm, *srm, *srn, *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid);
	sro=BGBCC_JX2DA_NameForLabel(ctx, lbl);
	srn=BGBCC_JX2DA_RegToName(ctx, reg);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s %s, %s\n",
		snm, sro, srn);
	return(1);
}

int BGBCC_JX2DA_EmitStoreRegLabelVarRel24(
	BGBCC_JX2_Context *ctx, int nmid, int reg, int lbl)
{
	char *snm, *srm, *srn, *sro;

	if(ctx->is_simpass || !ctx->do_asm)
		return(0);
		
	snm=BGBCC_JX2DA_NmidToName(ctx, nmid);
	sro=BGBCC_JX2DA_NameForLabel(ctx, lbl);
	srn=BGBCC_JX2DA_RegToName(ctx, reg);

	BGBCC_JX2DA_EmitPrintf(ctx, "  %-8s %s, %s\n",
		snm, srn, sro);
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

int BGBCC_JX2DA_EmitComm(BGBCC_JX2_Context *ctx, int sz)
{
	if(ctx->is_simpass || !ctx->do_asm)
		return(0);

	BGBCC_JX2DA_EmitPrintf(ctx, "  .comm %d\n", sz);
	return(1);
}
