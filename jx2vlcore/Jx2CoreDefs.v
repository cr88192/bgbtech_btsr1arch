
`ifndef HAS_COREDEFS
`define HAS_COREDEFS

`define JX2_EN_EGPR

parameter[6:0] JX2_REG_R0		= 7'h00;
parameter[6:0] JX2_REG_R1		= 7'h01;
parameter[6:0] JX2_REG_R2		= 7'h02;
parameter[6:0] JX2_REG_R3		= 7'h03;
parameter[6:0] JX2_REG_R4		= 7'h04;
parameter[6:0] JX2_REG_R5		= 7'h05;
parameter[6:0] JX2_REG_R6		= 7'h06;
parameter[6:0] JX2_REG_R7		= 7'h07;
parameter[6:0] JX2_REG_R8		= 7'h08;
parameter[6:0] JX2_REG_R9		= 7'h09;
parameter[6:0] JX2_REG_R10		= 7'h0A;
parameter[6:0] JX2_REG_R11		= 7'h0B;
parameter[6:0] JX2_REG_R12		= 7'h0C;
parameter[6:0] JX2_REG_R13		= 7'h0D;
parameter[6:0] JX2_REG_R14		= 7'h0E;
parameter[6:0] JX2_REG_R15		= 7'h0F;
parameter[6:0] JX2_REG_R16		= 7'h10;
parameter[6:0] JX2_REG_R17		= 7'h11;
parameter[6:0] JX2_REG_R18		= 7'h12;
parameter[6:0] JX2_REG_R19		= 7'h13;
parameter[6:0] JX2_REG_R20		= 7'h14;
parameter[6:0] JX2_REG_R21		= 7'h15;
parameter[6:0] JX2_REG_R22		= 7'h16;
parameter[6:0] JX2_REG_R23		= 7'h17;
parameter[6:0] JX2_REG_R24		= 7'h18;
parameter[6:0] JX2_REG_R25		= 7'h19;
parameter[6:0] JX2_REG_R26		= 7'h1A;
parameter[6:0] JX2_REG_R27		= 7'h1B;
parameter[6:0] JX2_REG_R28		= 7'h1C;
parameter[6:0] JX2_REG_R29		= 7'h1D;
parameter[6:0] JX2_REG_R30		= 7'h1E;
parameter[6:0] JX2_REG_R31		= 7'h1F;

parameter[6:0] JX2_REG_SP		= 7'h0F;

parameter[6:0] JX2_REG_PC		= 7'h20;
parameter[6:0] JX2_REG_LR		= 7'h21;
parameter[6:0] JX2_REG_SR		= 7'h22;
parameter[6:0] JX2_REG_VBR		= 7'h23;
parameter[6:0] JX2_REG_DLR		= 7'h24;
parameter[6:0] JX2_REG_DHR		= 7'h25;
parameter[6:0] JX2_REG_GBR		= 7'h26;
parameter[6:0] JX2_REG_TBR		= 7'h27;
parameter[6:0] JX2_REG_TTB		= 7'h28;
parameter[6:0] JX2_REG_TEA		= 7'h29;
parameter[6:0] JX2_REG_MMCR		= 7'h2A;
parameter[6:0] JX2_REG_EXSR		= 7'h2B;

parameter[6:0] JX2_REG_R0B		= 7'h40;
parameter[6:0] JX2_REG_R1B		= 7'h41;
parameter[6:0] JX2_REG_R2B		= 7'h42;
parameter[6:0] JX2_REG_R3B		= 7'h43;
parameter[6:0] JX2_REG_R4B		= 7'h44;
parameter[6:0] JX2_REG_R5B		= 7'h45;
parameter[6:0] JX2_REG_R6B		= 7'h46;
parameter[6:0] JX2_REG_R7B		= 7'h47;
parameter[6:0] JX2_REG_SPC		= 7'h48;
parameter[6:0] JX2_REG_SLR		= 7'h49;
parameter[6:0] JX2_REG_SSR		= 7'h4A;
parameter[6:0] JX2_REG_SSP		= 7'h4B;
parameter[6:0] JX2_REG_SDL		= 7'h4C;
parameter[6:0] JX2_REG_SDH		= 7'h4D;
parameter[6:0] JX2_REG_SGB		= 7'h4E;
parameter[6:0] JX2_REG_STB		= 7'h4F;
parameter[6:0] JX2_REG_R16B		= 7'h50;
parameter[6:0] JX2_REG_R17B		= 7'h51;
parameter[6:0] JX2_REG_R18B		= 7'h52;
parameter[6:0] JX2_REG_R19B		= 7'h53;
parameter[6:0] JX2_REG_R20B		= 7'h54;
parameter[6:0] JX2_REG_R21B		= 7'h55;
parameter[6:0] JX2_REG_R22B		= 7'h56;
parameter[6:0] JX2_REG_R23B		= 7'h57;

parameter[6:0] JX2_REG_IMM		= 7'h5E;
parameter[6:0] JX2_REG_ZZR		= 7'h5F;


parameter[ 15:0] UV16_XX		= 16'hXXXX;	//
parameter[ 31:0] UV32_XX		= 32'hXXXXXXXX;	//
parameter[ 47:0] UV48_XX		= 48'hXXXXXXXX_XXXX;	//
parameter[ 63:0] UV64_XX		= 64'hXXXXXXXX_XXXXXXXX;	//
parameter[ 95:0] UV96_XX		= 96'hXXXXXXXX_XXXXXXXX_XXXXXXXX;	//
parameter[127:0] UV128_XX		= { UV64_XX, UV64_XX };		//
parameter[159:0] UV160_XX		= { UV32_XX, UV128_XX };	//
parameter[191:0] UV192_XX		= { UV64_XX, UV128_XX };	//
parameter[223:0] UV224_XX		= { UV96_XX, UV128_XX };	//
parameter[255:0] UV256_XX		= { UV128_XX, UV128_XX };	//

parameter[15:0] UV16_00			= 16'h0000;	//
parameter[15:0] UV16_FF			= 16'hFFFF;	//

parameter[19:0] UV20_00			= 20'h00000;	//
parameter[19:0] UV20_FF			= 20'hFFFFF;	//

parameter[23:0] UV24_00			= 24'h000000;	//
parameter[23:0] UV24_FF			= 24'hFFFFFF;	//

parameter[26:0] UV27_00			= 27'h0000000;	//

parameter[27:0] UV28_00			= 28'h0000000;	//
parameter[27:0] UV28_FF			= 28'hFFFFFFF;	//

parameter[28:0] UV29_00			= 29'h0000000;	//

parameter[31:0] UV32_00			= 32'h00000000;	//
parameter[31:0] UV32_FF			= 32'hFFFFFFFF;	//

parameter[47:0] UV48_00			= 48'h0000_00000000;	//
parameter[47:0] UV48_FF			= 48'hFFFF_FFFFFFFF;	//

parameter[55:0] UV56_00			= 56'h000000_00000000;	//
parameter[55:0] UV56_FF			= 56'hFFFFFF_FFFFFFFF;	//

parameter[63:0] UV64_00			= 64'h00000000_00000000;	//
parameter[63:0] UV64_FF			= 64'hFFFFFFFF_FFFFFFFF;	//


parameter[1:0] UMEM_OK_READY	= 2'h0;		//Ready (Awaiting Request)
parameter[1:0] UMEM_OK_OK		= 2'h1;		//OK (Request Successful)
parameter[1:0] UMEM_OK_HOLD		= 2'h2;		//Hold (IO Not Ready)
parameter[1:0] UMEM_OK_FAULT	= 2'h3;		//FAULT (Request Failed)

parameter[4:0] UMEM_OPM_READY	= 5'b00000;		//Ready/Idle

parameter[4:0] UMEM_OPM_CTRLF	= 5'b00010;		//Control Flow

parameter[4:0] UMEM_OPM_RD_SB	= 5'b01000;		//Read Byte
parameter[4:0] UMEM_OPM_RD_SW	= 5'b01001;		//Read Word
parameter[4:0] UMEM_OPM_RD_SL	= 5'b01010;		//Read DWord
parameter[4:0] UMEM_OPM_RD_Q	= 5'b01011;		//Read QWord (Resv)
parameter[4:0] UMEM_OPM_RD_UB	= 5'b01100;		//Read Byte
parameter[4:0] UMEM_OPM_RD_UW	= 5'b01101;		//Read Word
parameter[4:0] UMEM_OPM_RD_UL	= 5'b01110;		//Read Unsigned DWord (Resv)
parameter[4:0] UMEM_OPM_RD_TILE	= 5'b01111;		//Read Tile

parameter[4:0] UMEM_OPM_WR_SB	= 5'b10000;		//Write Byte
parameter[4:0] UMEM_OPM_WR_SW	= 5'b10001;		//Write Word
parameter[4:0] UMEM_OPM_WR_SL	= 5'b10010;		//Write DWord
parameter[4:0] UMEM_OPM_WR_Q	= 5'b10011;		//Write QWord (Resv)
parameter[4:0] UMEM_OPM_WR_UB	= 5'b10100;		//Write Byte (Resv)
parameter[4:0] UMEM_OPM_WR_UW	= 5'b10101;		//Write Word (Resv)
parameter[4:0] UMEM_OPM_WR_UL	= 5'b10110;		//Write DWord (Resv)
parameter[4:0] UMEM_OPM_WR_TILE	= 5'b10111;		//Write Tile

parameter[4:0] JX2_FMID_INV			= 5'h00;	//? (Invalid Opcode) / Custom
parameter[4:0] JX2_FMID_Z			= 5'h01;	//OOOO	-
parameter[4:0] JX2_FMID_REG			= 5'h02;	//OOnO	Rn | #0, Rn
parameter[4:0] JX2_FMID_IMM8		= 5'h03;	//OOii	#imm8s, DLR
parameter[4:0] JX2_FMID_REGREG		= 5'h04;	//OOnm	Rm, Rn
parameter[4:0] JX2_FMID_IMM8REG		= 5'h05;	//Onii	#imm8s, Rn
parameter[4:0] JX2_FMID_IMM8Z		= 5'h06;	//OOjj	#imm8u, DLR
parameter[4:0] JX2_FMID_IMM8N		= 5'h07;	//OOjj	#imm8n, DLR
parameter[4:0] JX2_FMID_REGSTREG	= 5'h08;	//OOnm  Rm, (Rn)
parameter[4:0] JX2_FMID_LDREGREG	= 5'h09;	//OOnm  (Rm), Rn
parameter[4:0] JX2_FMID_REGSTDRREG	= 5'h0A;	//OOnm  Rm, (Rn, DLR)
parameter[4:0] JX2_FMID_LDDRREGREG	= 5'h0B;	//OOnm  (Rm, DLR), Rn
parameter[4:0] JX2_FMID_REGSTDRPC	= 5'h0C;	//OOnO  Rn, (PC, DLR)
parameter[4:0] JX2_FMID_LDDRPCREG	= 5'h0D;	//OOnO  (PC, DLR), Rn
parameter[4:0] JX2_FMID_IMM4ZREG	= 5'h0E;	//OOnj  #imm4u, Rn
parameter[4:0] JX2_FMID_IMM4NREG	= 5'h0F;	//OOnj  #imm4n, Rn

parameter[4:0] JX2_FMID_DR4PC		= 5'h10;	//OOOj	(PC, DLR_i4)
parameter[4:0] JX2_FMID_DRPC		= 5'h11;	//OOOO	(PC, DLR)
parameter[4:0] JX2_FMID_DRREG		= 5'h12;	//OOnO	DLR, Rn
parameter[4:0] JX2_FMID_PCDISP8		= 5'h13;	//OOii	(PC, disp8s)

parameter[4:0] JX2_FMID_IMM12Z		= 5'h16;	//Ojjj	#imm12u, DLR
parameter[4:0] JX2_FMID_IMM12N		= 5'h17;	//Ojjj	#imm12n, DLR

parameter[4:0] JX2_FMID_REGSTDLR	= 5'h18;	//OOnO  Rn, (DLR)
parameter[4:0] JX2_FMID_LDDLRREG	= 5'h19;	//OOnO  (DLR), Rn
// parameter[4:0] JX2_FMID_REGSTDRGBR	= 5'h1A;	//OOnO  Rm, (GBR, DLR)
// parameter[4:0] JX2_FMID_LDDRGBRREG	= 5'h1B;	//OOnO  (GBR, DLR), Rn
parameter[4:0] JX2_FMID_REGSTDR4PC	= 5'h1C;	//OOnj  Rn, (PC, DLR_i4)
parameter[4:0] JX2_FMID_LDDR4PCREG	= 5'h1D;	//OOnj  (PC, DLR_i4), Rn
parameter[4:0] JX2_FMID_REGSTDI4SP	= 5'h1E;	//OOnj  Rn, (SP, disp4)
parameter[4:0] JX2_FMID_LDDI4SPREG	= 5'h1F;	//OOnj  (SP, disp4), Rn


/*

MVIXT:
  0,I2,I1,I0, D,Zx,S1,S0

I2/1/0:
 000: REG	(Rn)
 001: RDL	(Rn,DLR)
 010: RDI	(Rn,Disp)
 011: RD4	(Rn,DLR_i4)
 100: / ADL	(DLR)
 101: / PDL	(PC,DLR)
 110: / PDI	(PC,Disp)
 111: / PD4	(PC,DLR_i4)

 100: RRI (Rn, Ri)
D:
 0: Rm, (Rn)
 1: (Rm), Rn
*/

parameter[3:0] JX2_IXT_REG	= 4'b0000;
parameter[3:0] JX2_IXT_RDL	= 4'b0001;
parameter[3:0] JX2_IXT_RDI	= 4'b0010;
parameter[3:0] JX2_IXT_RD4	= 4'b0011;
// parameter[3:0] JX2_IXT_ADL	= 4'b0100;
// parameter[3:0] JX2_IXT_PDL	= 4'b0101;
// parameter[3:0] JX2_IXT_PDI	= 4'b0110;
// parameter[3:0] JX2_IXT_PD4	= 4'b0111;

parameter[3:0] JX2_IXT_RRI	= 4'b0100;

parameter[1:0] JX2_IX2_REG	= 2'b00;
parameter[1:0] JX2_IX2_RDL	= 2'b01;
parameter[1:0] JX2_IX2_RDI	= 2'b10;
parameter[1:0] JX2_IX2_RD4	= 2'b11;

parameter[2:0] JX2_BTY_SB	= 3'b000;	/* Signed Byte, GPR */
parameter[2:0] JX2_BTY_SW	= 3'b001;	/* Signed Word, EGPR */
parameter[2:0] JX2_BTY_SL	= 3'b010;	/* Signed Long, DRREG=CR */
parameter[2:0] JX2_BTY_SQ	= 3'b011;	/* Signed Quad, DRREG=ECR */
parameter[2:0] JX2_BTY_UB	= 3'b100;
parameter[2:0] JX2_BTY_UW	= 3'b101;
parameter[2:0] JX2_BTY_UL	= 3'b110;
parameter[2:0] JX2_BTY_UQ	= 3'b111;

parameter[3:0] JX2_ITY_SB	= 4'b0000;	/* XXzz_XXii		SX */
parameter[3:0] JX2_ITY_SW	= 4'b0001;	/* XXzz_iiii		SX */
parameter[3:0] JX2_ITY_SL	= 4'b0010;	/* XXzz_iiii_iiii	SX */
parameter[3:0] JX2_ITY_SQ	= 4'b0011;	/* XXii				SX */
parameter[3:0] JX2_ITY_UB	= 4'b0100;	/* XXzz_XXjj		ZX */
parameter[3:0] JX2_ITY_UW	= 4'b0101;	/* XXzz_jjjj		ZX */
parameter[3:0] JX2_ITY_UL	= 4'b0110;	/* XXzz_jjjj_jjjj	ZX */
parameter[3:0] JX2_ITY_UQ	= 4'b0111;	/* XXjj				ZX */
parameter[3:0] JX2_ITY_NB	= 4'b1000;	/* XXzz_XXjj		NX */
parameter[3:0] JX2_ITY_NW	= 4'b1001;	/* XXzz_jjjj		NX */
parameter[3:0] JX2_ITY_NL	= 4'b1010;	/* XXzz_jjjj_jjjj	NX */
parameter[3:0] JX2_ITY_NQ	= 4'b1011;	/* XXjj				NX */


// parameter[7:0] JX2_UCMD_MOV_RM		= 8'h04;	//(AGU)=Rm
// parameter[7:0] JX2_UCMD_MOV_MR		= 8'h05;	//Rn=(AGU)

parameter[7:0] JX2_UCMD_NOP			= 8'h00;
parameter[7:0] JX2_UCMD_INVOP		= 8'h01;	//Invalid Opcode
parameter[7:0] JX2_UCMD_OP_IXS		= 8'h02;	//Extended Opcodes (Reg)
parameter[7:0] JX2_UCMD_OP_IXT		= 8'h03;	//Extended Opcodes (No-Arg)
parameter[7:0] JX2_UCMD_MOV_RR		= 8'h04;	//Rn=Rm
parameter[7:0] JX2_UCMD_MOV_IR		= 8'h05;	//Rn=Ri
parameter[7:0] JX2_UCMD_LEA_MR		= 8'h06;	//Rn=&(AGU)
parameter[7:0] JX2_UCMD_MOVX_WB		= 8'h07;	//MOV Writeback
parameter[7:0] JX2_UCMD_MOVB_RM		= 8'h08;	//(AGU)=Rm
parameter[7:0] JX2_UCMD_MOVB_MR		= 8'h09;	//Rn=(AGU)
parameter[7:0] JX2_UCMD_MOVW_RM		= 8'h0A;	//(AGU)=Rm
parameter[7:0] JX2_UCMD_MOVW_MR		= 8'h0B;	//Rn=(AGU)
parameter[7:0] JX2_UCMD_MOVL_RM		= 8'h0C;	//(AGU)=Rm
parameter[7:0] JX2_UCMD_MOVL_MR		= 8'h0D;	//Rn=(AGU)
parameter[7:0] JX2_UCMD_MOVQ_MR		= 8'h0E;	//Rn=(AGU)
parameter[7:0] JX2_UCMD_MOVQ_RM		= 8'h0F;	//(AGU)=Rn

parameter[7:0] JX2_UCMD_ALU_ADD		= 8'h10;	//Rn=Rn+Rm
parameter[7:0] JX2_UCMD_ALU_SUB		= 8'h11;	//Rn=Rn-Rm
parameter[7:0] JX2_UCMD_ALU_ADC		= 8'h12;	//Rn=Rn+Rm
parameter[7:0] JX2_UCMD_ALU_SBB		= 8'h13;	//Rn=Rn-Rm
parameter[7:0] JX2_UCMD_ALU_TST		= 8'h14;	//SR.T=!(Rn&Rm)
parameter[7:0] JX2_UCMD_ALU_AND		= 8'h15;	//Rn=Rn&Rm
parameter[7:0] JX2_UCMD_ALU_OR		= 8'h16;	//Rn=Rn|Rm
parameter[7:0] JX2_UCMD_ALU_XOR		= 8'h17;	//Rn=Rn^Rm
parameter[7:0] JX2_UCMD_ALU_MULU	= 8'h18;	//DLR=Rn*Rm
parameter[7:0] JX2_UCMD_ALU_MULS	= 8'h19;	//DLR=Rn*Rm
parameter[7:0] JX2_UCMD_ALU_LDIX	= 8'h1A;
parameter[7:0] JX2_UCMD_ALU_LDISH	= 8'h1B;
parameter[7:0] JX2_UCMD_ALU_CMPEQ	= 8'h1C;
parameter[7:0] JX2_UCMD_ALU_CMPHI	= 8'h1D;
parameter[7:0] JX2_UCMD_ALU_CMPGT	= 8'h1E;
parameter[7:0] JX2_UCMD_ALU_CMPGE	= 8'h1F;

parameter[7:0] JX2_UCMD_CF_BRA		= 8'h20;	//BRA (AGU)
parameter[7:0] JX2_UCMD_CF_BSR		= 8'h21;	//BSR (AGU)
parameter[7:0] JX2_UCMD_CF_BT		= 8'h22;	//BT (AGU)
parameter[7:0] JX2_UCMD_CF_BF		= 8'h23;	//BF (AGU)
parameter[7:0] JX2_UCMD_MOV_PUSH	= 8'h24;
parameter[7:0] JX2_UCMD_MOV_PUSHC	= 8'h25;
parameter[7:0] JX2_UCMD_MOV_POP		= 8'h26;
parameter[7:0] JX2_UCMD_MOV_POPC	= 8'h27;
parameter[7:0] JX2_UCMD_CF_JMP		= 8'h28;	//BRA Rn
parameter[7:0] JX2_UCMD_CF_JSR		= 8'h29;	//BSR Rn
parameter[7:0] JX2_UCMD_CF_JT		= 8'h2A;	//BT Rn
parameter[7:0] JX2_UCMD_CF_JF		= 8'h2B;	//BF Rn
parameter[7:0] JX2_UCMD_ALU_EXTUB	= 8'h2C;
parameter[7:0] JX2_UCMD_ALU_EXTUW	= 8'h2D;
parameter[7:0] JX2_UCMD_ALU_EXTSB	= 8'h2E;
parameter[7:0] JX2_UCMD_ALU_EXTSW	= 8'h2F;

parameter[7:0] JX2_UCMD_ALU_NOT		= 8'h30;
parameter[7:0] JX2_UCMD_ALU_NEG		= 8'h31;
parameter[7:0] JX2_UCMD_ALU_NEGC	= 8'h32;
parameter[7:0] JX2_UCMD_ALU_SHARSX	= 8'h33;
parameter[7:0] JX2_UCMD_ALU_ROTL	= 8'h34;
parameter[7:0] JX2_UCMD_ALU_ROTR	= 8'h35;
parameter[7:0] JX2_UCMD_ALU_ROTCL	= 8'h36;
parameter[7:0] JX2_UCMD_ALU_ROTCR	= 8'h37;
parameter[7:0] JX2_UCMD_ALU_SHLL	= 8'h38;
parameter[7:0] JX2_UCMD_ALU_SHLR	= 8'h39;
parameter[7:0] JX2_UCMD_ALU_SHAR	= 8'h3A;
parameter[7:0] JX2_UCMD_ALU_SHAD	= 8'h3B;
parameter[7:0] JX2_UCMD_ALU_SHLLN	= 8'h3C;
parameter[7:0] JX2_UCMD_ALU_SHLRN	= 8'h3D;
parameter[7:0] JX2_UCMD_ALU_SHARN	= 8'h3E;
parameter[7:0] JX2_UCMD_ALU_SHLD	= 8'h3F;

parameter[7:0] JX2_UCMD_ALU_ADD2	= 8'h40;	//Rn=Rm+DLR
parameter[7:0] JX2_UCMD_ALU_SUB2	= 8'h41;	//Rn=Rm-DLR
parameter[7:0] JX2_UCMD_ALU_AND2	= 8'h42;	//Rn=Rm&DLR
parameter[7:0] JX2_UCMD_ALU_OR2		= 8'h43;	//Rn=Rm|DLR
parameter[7:0] JX2_UCMD_ALU_XOR2	= 8'h44;	//Rn=Rm^DLR
parameter[7:0] JX2_UCMD_MOVUL_MR	= 8'h43;	//Rn=(AGU)
parameter[7:0] JX2_UCMD_ALU_LDISH16	= 8'h45;	//
parameter[7:0] JX2_UCMD_ALU_LDISH24	= 8'h46;	//
parameter[7:0] JX2_UCMD_ALU_LDISH32	= 8'h47;	//
parameter[7:0] JX2_UCMD_MOV_DLR4R	= 8'h48;	//Rn=DLR_i4
parameter[7:0] JX2_UCMD_ADD_DLR4R	= 8'h49;	//Rn=Rn+DLR_i4
parameter[7:0] JX2_UCMD_MOVUB_MR	= 8'h4A;	//Rn=(AGU)
parameter[7:0] JX2_UCMD_MOVUW_MR	= 8'h4B;	//Rn=(AGU)
parameter[7:0] JX2_UCMD_FMOVS_RM	= 8'h4C;	//(AGU)=FRm
parameter[7:0] JX2_UCMD_FMOVS_MR	= 8'h4D;	//FRn=(AGU)
parameter[7:0] JX2_UCMD_FMOVD_RM	= 8'h4E;	//(AGU)=FRm
parameter[7:0] JX2_UCMD_FMOVD_MR	= 8'h4F;	//FRn=(AGU)

parameter[7:0] JX2_UCMD_FPU_FADD	= 8'h50;	//FPU FRn=FRn+FRm
parameter[7:0] JX2_UCMD_FPU_FSUB	= 8'h51;	//FPU FRn=FRn-FRm
parameter[7:0] JX2_UCMD_FPU_FMUL	= 8'h52;	//FPU FRn=FRn*FRm
parameter[7:0] JX2_UCMD_FPU_FDIV	= 8'h53;	//FPU FRn=FRn/FRm
parameter[7:0] JX2_UCMD_FPU_FCMPEQ	= 8'h54;	//FPU SR.T=(FRn==FRm)
parameter[7:0] JX2_UCMD_FPU_FCMPGT	= 8'h55;	//FPU SR.T=(FRn>FRm)
parameter[7:0] JX2_UCMD_FPU_FMOV	= 8'h56;	//FPU FRn=FRm
parameter[7:0] JX2_UCMD_FPU_FIXS	= 8'h57;	//Single-Arg FPU Ops
parameter[7:0] JX2_UCMD_FPU_FLDCF	= 8'h58;	//FRn=DLR
parameter[7:0] JX2_UCMD_FPU_FLDCD	= 8'h59;	//FRn=DLR
parameter[7:0] JX2_UCMD_FPU_FLDCI	= 8'h5A;	//FRn=DLR
parameter[7:0] JX2_UCMD_FPU_FLDCH	= 8'h5B;	//FRn=DLR
parameter[7:0] JX2_UCMD_FPU_FSTCF	= 8'h5C;	//DLR=Rn
parameter[7:0] JX2_UCMD_FPU_FSTCD	= 8'h5D;	//DLR=Rn
parameter[7:0] JX2_UCMD_FPU_FSTCI	= 8'h5E;	//
parameter[7:0] JX2_UCMD_FPU_FSTCH	= 8'h5F;	//

parameter[7:0] JX2_UCMD_ALU_TSTQ	= 8'h60;	//SR.T=!(Rn&Rm)
parameter[7:0] JX2_UCMD_ALU_CMPQEQ	= 8'h61;	//
parameter[7:0] JX2_UCMD_ALU_CMPQHI	= 8'h62;	//
parameter[7:0] JX2_UCMD_ALU_CMPQGT	= 8'h63;	//
parameter[7:0] JX2_UCMD_ALU_SHADQ	= 8'h64;
parameter[7:0] JX2_UCMD_ALU_SHLDQ	= 8'h65;
parameter[7:0] JX2_UCMD_ALU_EXTUL	= 8'h66;
parameter[7:0] JX2_UCMD_ALU_EXTSL	= 8'h67;

parameter[7:0] JX2_UCMD_ALU_ADD3	= 8'h68;	//Rn=Rm+Ro
parameter[7:0] JX2_UCMD_ALU_SUB3	= 8'h69;	//Rn=Rm-Ro
parameter[7:0] JX2_UCMD_ALU_AND3	= 8'h6A;	//Rn=Rm&Ro
parameter[7:0] JX2_UCMD_ALU_OR3		= 8'h6B;	//Rn=Rm|Ro
parameter[7:0] JX2_UCMD_ALU_XOR3	= 8'h6C;	//Rn=Rm^Ro
parameter[7:0] JX2_UCMD_ALU_MUL3	= 8'h6D;	//Rn=Rm*Ro


parameter[7:0] JX2_UCMD_IX_NOP		= 8'h00;
parameter[7:0] JX2_UCMD_IX_RTS		= 8'h01;
parameter[7:0] JX2_UCMD_IX_SLEEP	= 8'h02;
parameter[7:0] JX2_UCMD_IX_BREAK	= 8'h03;
parameter[7:0] JX2_UCMD_IX_CLRT		= 8'h04;
parameter[7:0] JX2_UCMD_IX_SETT		= 8'h05;
parameter[7:0] JX2_UCMD_IX_CLRS		= 8'h06;
parameter[7:0] JX2_UCMD_IX_SETS		= 8'h07;
parameter[7:0] JX2_UCMD_IX_NOTT		= 8'h08;
parameter[7:0] JX2_UCMD_IX_NOTS		= 8'h09;

parameter[7:0] JX2_UCMD_IX_RTE		= 8'h0C;
parameter[7:0] JX2_UCMD_IX_TRAPA	= 8'h0D;

parameter[7:0] JX2_UCMD_IX_MOVNT	= 8'h10;

parameter[7:0] JX2_UCMD_FPIX_FNEG	= 8'h00;
parameter[7:0] JX2_UCMD_FPIX_FABS	= 8'h01;
parameter[7:0] JX2_UCMD_FPIX_FRCPA	= 8'h02;
parameter[7:0] JX2_UCMD_FPIX_FSQRTA	= 8'h03;
parameter[7:0] JX2_UCMD_FPIX_FRCP	= 8'h04;
parameter[7:0] JX2_UCMD_FPIX_FSQRT	= 8'h05;

`endif
