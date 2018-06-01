
`ifndef HAS_COREDEFS
`define HAS_COREDEFS


parameter[5:0] BSR_REG_R0		= 6'h00;
parameter[5:0] BSR_REG_R1		= 6'h01;
parameter[5:0] BSR_REG_R2		= 6'h02;
parameter[5:0] BSR_REG_R3		= 6'h03;
parameter[5:0] BSR_REG_R4		= 6'h04;
parameter[5:0] BSR_REG_R5		= 6'h05;
parameter[5:0] BSR_REG_R6		= 6'h06;
parameter[5:0] BSR_REG_R7		= 6'h07;
parameter[5:0] BSR_REG_R8		= 6'h08;
parameter[5:0] BSR_REG_R9		= 6'h09;
parameter[5:0] BSR_REG_R10		= 6'h0A;
parameter[5:0] BSR_REG_R11		= 6'h0B;
parameter[5:0] BSR_REG_R12		= 6'h0C;
parameter[5:0] BSR_REG_R13		= 6'h0D;
parameter[5:0] BSR_REG_R14		= 6'h0E;
parameter[5:0] BSR_REG_R15		= 6'h0F;

parameter[5:0] BSR_REG_SP		= 6'h0F;

parameter[5:0] BSR_REG_PC		= 6'h10;
parameter[5:0] BSR_REG_LR		= 6'h11;
parameter[5:0] BSR_REG_SR		= 6'h12;
parameter[5:0] BSR_REG_VBR		= 6'h13;
parameter[5:0] BSR_REG_DLR		= 6'h14;
parameter[5:0] BSR_REG_DHR		= 6'h15;
parameter[5:0] BSR_REG_GBR		= 6'h16;
parameter[5:0] BSR_REG_TBR		= 6'h17;
parameter[5:0] BSR_REG_TTB		= 6'h18;
parameter[5:0] BSR_REG_TEA		= 6'h19;
parameter[5:0] BSR_REG_MMCR		= 6'h1A;
parameter[5:0] BSR_REG_EXSR		= 6'h1B;

parameter[5:0] BSR_REG_R0B		= 6'h20;
parameter[5:0] BSR_REG_R1B		= 6'h21;
parameter[5:0] BSR_REG_R2B		= 6'h22;
parameter[5:0] BSR_REG_R3B		= 6'h23;
parameter[5:0] BSR_REG_R4B		= 6'h24;
parameter[5:0] BSR_REG_R5B		= 6'h25;
parameter[5:0] BSR_REG_R6B		= 6'h26;
parameter[5:0] BSR_REG_R7B		= 6'h27;
parameter[5:0] BSR_REG_SPC		= 6'h28;
parameter[5:0] BSR_REG_SLR		= 6'h29;
parameter[5:0] BSR_REG_SSR		= 6'h2A;
parameter[5:0] BSR_REG_SSP		= 6'h2B;
parameter[5:0] BSR_REG_SDL		= 6'h2C;
parameter[5:0] BSR_REG_SDH		= 6'h2D;
parameter[5:0] BSR_REG_SGB		= 6'h2E;
parameter[5:0] BSR_REG_STB		= 6'h2F;

parameter[5:0] BSR_REG_IMM		= 6'h38;
parameter[5:0] BSR_REG_ZZR		= 6'h3F;

parameter[ 15:0] UV16_XX		= 16'hXXXX;	//
parameter[ 31:0] UV32_XX		= 32'hXXXXXXXX;	//
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

parameter[27:0] UV28_00			= 28'h0000000;	//
parameter[27:0] UV28_FF			= 28'hFFFFFFF;	//

parameter[28:0] UV29_00			= 29'h0000000;	//

parameter[31:0] UV32_00			= 32'h00000000;	//
parameter[31:0] UV32_FF			= 32'hFFFFFFFF;	//


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

parameter[4:0] BSR_FMID_INV			= 5'h00;	//? (Invalid Opcode) / Custom
parameter[4:0] BSR_FMID_Z			= 5'h01;	//OOOO	-
parameter[4:0] BSR_FMID_REG			= 5'h02;	//OOnO	Rn | #0, Rn
parameter[4:0] BSR_FMID_IMM8		= 5'h03;	//OOii	#imm8s, DLR
parameter[4:0] BSR_FMID_REGREG		= 5'h04;	//OOnm	Rm, Rn
parameter[4:0] BSR_FMID_IMM8REG		= 5'h05;	//Onii	#imm8s, Rn
parameter[4:0] BSR_FMID_IMM8Z		= 5'h06;	//OOjj	#imm8u, DLR
parameter[4:0] BSR_FMID_IMM8N		= 5'h07;	//OOjj	#imm8n, DLR
parameter[4:0] BSR_FMID_REGSTREG	= 5'h08;	//OOnm  Rm, (Rn)
parameter[4:0] BSR_FMID_LDREGREG	= 5'h09;	//OOnm  (Rm), Rn
parameter[4:0] BSR_FMID_REGSTDRREG	= 5'h0A;	//OOnm  Rm, (Rn, DLR)
parameter[4:0] BSR_FMID_LDDRREGREG	= 5'h0B;	//OOnm  (Rm, DLR), Rn
parameter[4:0] BSR_FMID_REGSTDRPC	= 5'h0C;	//OOnO  Rn, (PC, DLR)
parameter[4:0] BSR_FMID_LDDRPCREG	= 5'h0D;	//OOnO  (PC, DLR), Rn
parameter[4:0] BSR_FMID_IMM4ZREG	= 5'h0E;	//OOnj  #imm4u, Rn
parameter[4:0] BSR_FMID_IMM4NREG	= 5'h0F;	//OOnj  #imm4n, Rn

parameter[4:0] BSR_FMID_DR4PC		= 5'h10;	//OOOj	(PC, DLR_i4)
parameter[4:0] BSR_FMID_DRPC		= 5'h11;	//OOOO	(PC, DLR)
parameter[4:0] BSR_FMID_DRREG		= 5'h12;	//OOnO	DLR, Rn
parameter[4:0] BSR_FMID_PCDISP8		= 5'h13;	//OOii	(PC, disp8s)

parameter[4:0] BSR_FMID_IMM12Z		= 5'h16;	//Ojjj	#imm12u, DLR
parameter[4:0] BSR_FMID_IMM12N		= 5'h17;	//Ojjj	#imm12n, DLR

parameter[4:0] BSR_FMID_REGSTDLR	= 5'h18;	//OOnO  Rn, (DLR)
parameter[4:0] BSR_FMID_LDDLRREG	= 5'h19;	//OOnO  (DLR), Rn
parameter[4:0] BSR_FMID_REGSTDRGBR	= 5'h1A;	//OOnO  Rm, (GBR, DLR)
parameter[4:0] BSR_FMID_LDDRGBRREG	= 5'h1B;	//OOnO  (GBR, DLR), Rn
parameter[4:0] BSR_FMID_REGSTDR4PC	= 5'h1C;	//OOnj  Rn, (PC, DLR_i4)
parameter[4:0] BSR_FMID_LDDR4PCREG	= 5'h1D;	//OOnj  (PC, DLR_i4), Rn
parameter[4:0] BSR_FMID_REGSTDI4SP	= 5'h1E;	//OOnj  Rn, (SP, disp4)
parameter[4:0] BSR_FMID_LDDI4SPREG	= 5'h1F;	//OOnj  (SP, disp4), Rn


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

D:
 0: Rm, (Rn)
 1: (Rm), Rn
*/

parameter[3:0] BSR_IXT_REG	= 4'b0000;
parameter[3:0] BSR_IXT_RDL	= 4'b0001;
parameter[3:0] BSR_IXT_RDI	= 4'b0010;
parameter[3:0] BSR_IXT_RD4	= 4'b0011;
parameter[3:0] BSR_IXT_ADL	= 4'b0100;
parameter[3:0] BSR_IXT_PDL	= 4'b0101;
parameter[3:0] BSR_IXT_PDI	= 4'b0110;
parameter[3:0] BSR_IXT_PD4	= 4'b0111;

parameter[1:0] BSR_IX2_REG	= 2'b00;
parameter[1:0] BSR_IX2_RDL	= 2'b01;
parameter[1:0] BSR_IX2_RDI	= 2'b10;
parameter[1:0] BSR_IX2_RD4	= 2'b11;

parameter[2:0] BSR_BTY_SB	= 3'b000;
parameter[2:0] BSR_BTY_SW	= 3'b001;
parameter[2:0] BSR_BTY_SL	= 3'b010;
parameter[2:0] BSR_BTY_SQ	= 3'b011;
parameter[2:0] BSR_BTY_UB	= 3'b100;
parameter[2:0] BSR_BTY_UW	= 3'b101;
parameter[2:0] BSR_BTY_UL	= 3'b110;
parameter[2:0] BSR_BTY_UQ	= 3'b111;

// parameter[7:0] BSR_UCMD_MOV_RM		= 8'h04;	//(AGU)=Rm
// parameter[7:0] BSR_UCMD_MOV_MR		= 8'h05;	//Rn=(AGU)

parameter[7:0] BSR_UCMD_NOP			= 8'h00;
parameter[7:0] BSR_UCMD_INVOP		= 8'h01;	//Invalid Opcode
parameter[7:0] BSR_UCMD_OP_IXS		= 8'h02;	//Extended Opcodes (Reg)
parameter[7:0] BSR_UCMD_OP_IXT		= 8'h03;	//Extended Opcodes (No-Arg)
parameter[7:0] BSR_UCMD_MOV_RR		= 8'h04;	//Rn=Rm
parameter[7:0] BSR_UCMD_MOV_IR		= 8'h05;	//Rn=Ri
parameter[7:0] BSR_UCMD_LEA_MR		= 8'h06;	//Rn=&(AGU)
parameter[7:0] BSR_UCMD_MOVX_WB		= 8'h07;	//MOV Writeback
parameter[7:0] BSR_UCMD_MOVB_RM		= 8'h08;	//(AGU)=Rm
parameter[7:0] BSR_UCMD_MOVB_MR		= 8'h09;	//Rn=(AGU)
parameter[7:0] BSR_UCMD_MOVW_RM		= 8'h0A;	//(AGU)=Rm
parameter[7:0] BSR_UCMD_MOVW_MR		= 8'h0B;	//Rn=(AGU)
parameter[7:0] BSR_UCMD_MOVL_RM		= 8'h0C;	//(AGU)=Rm
parameter[7:0] BSR_UCMD_MOVL_MR		= 8'h0D;	//Rn=(AGU)
parameter[7:0] BSR_UCMD_MOVUB_MR	= 8'h0E;	//Rn=(AGU)
parameter[7:0] BSR_UCMD_MOVUW_MR	= 8'h0F;	//Rn=(AGU)

parameter[7:0] BSR_UCMD_ALU_ADD		= 8'h10;	//Rn=Rn+Rm
parameter[7:0] BSR_UCMD_ALU_SUB		= 8'h11;	//Rn=Rn-Rm
parameter[7:0] BSR_UCMD_ALU_ADC		= 8'h12;	//Rn=Rn+Rm
parameter[7:0] BSR_UCMD_ALU_SBB		= 8'h13;	//Rn=Rn-Rm
parameter[7:0] BSR_UCMD_ALU_TST		= 8'h14;	//SR.T=!(Rn&Rm)
parameter[7:0] BSR_UCMD_ALU_AND		= 8'h15;	//Rn=Rn&Rm
parameter[7:0] BSR_UCMD_ALU_OR		= 8'h16;	//Rn=Rn|Rm
parameter[7:0] BSR_UCMD_ALU_XOR		= 8'h17;	//Rn=Rn^Rm
parameter[7:0] BSR_UCMD_ALU_MULU	= 8'h18;	//DLR=Rn*Rm
parameter[7:0] BSR_UCMD_ALU_MULS	= 8'h19;	//DLR=Rn*Rm
parameter[7:0] BSR_UCMD_ALU_LDIX	= 8'h1A;
parameter[7:0] BSR_UCMD_ALU_LDISH	= 8'h1B;
parameter[7:0] BSR_UCMD_ALU_CMPEQ	= 8'h1C;
parameter[7:0] BSR_UCMD_ALU_CMPHI	= 8'h1D;
parameter[7:0] BSR_UCMD_ALU_CMPGT	= 8'h1E;
parameter[7:0] BSR_UCMD_ALU_CMPGE	= 8'h1F;

parameter[7:0] BSR_UCMD_CF_BRA		= 8'h20;	//BRA (AGU)
parameter[7:0] BSR_UCMD_CF_BSR		= 8'h21;	//BSR (AGU)
parameter[7:0] BSR_UCMD_CF_BT		= 8'h22;	//BT (AGU)
parameter[7:0] BSR_UCMD_CF_BF		= 8'h23;	//BF (AGU)
parameter[7:0] BSR_UCMD_MOV_PUSH	= 8'h24;
parameter[7:0] BSR_UCMD_MOV_PUSHC	= 8'h25;
parameter[7:0] BSR_UCMD_MOV_POP		= 8'h26;
parameter[7:0] BSR_UCMD_MOV_POPC	= 8'h27;
parameter[7:0] BSR_UCMD_CF_JMP		= 8'h28;	//BRA Rn
parameter[7:0] BSR_UCMD_CF_JSR		= 8'h29;	//BSR Rn
parameter[7:0] BSR_UCMD_CF_JT		= 8'h2A;	//BT Rn
parameter[7:0] BSR_UCMD_CF_JF		= 8'h2B;	//BF Rn
parameter[7:0] BSR_UCMD_ALU_EXTUB	= 8'h2C;
parameter[7:0] BSR_UCMD_ALU_EXTUW	= 8'h2D;
parameter[7:0] BSR_UCMD_ALU_EXTSB	= 8'h2E;
parameter[7:0] BSR_UCMD_ALU_EXTSW	= 8'h2F;

parameter[7:0] BSR_UCMD_ALU_NOT		= 8'h30;
parameter[7:0] BSR_UCMD_ALU_NEG		= 8'h31;
parameter[7:0] BSR_UCMD_ALU_NEGC	= 8'h32;
parameter[7:0] BSR_UCMD_ALU_SHARSX	= 8'h33;
parameter[7:0] BSR_UCMD_ALU_ROTL	= 8'h34;
parameter[7:0] BSR_UCMD_ALU_ROTR	= 8'h35;
parameter[7:0] BSR_UCMD_ALU_ROTCL	= 8'h36;
parameter[7:0] BSR_UCMD_ALU_ROTCR	= 8'h37;
parameter[7:0] BSR_UCMD_ALU_SHLL	= 8'h38;
parameter[7:0] BSR_UCMD_ALU_SHLR	= 8'h39;
parameter[7:0] BSR_UCMD_ALU_SHAR	= 8'h3A;
parameter[7:0] BSR_UCMD_ALU_SHAD	= 8'h3B;
parameter[7:0] BSR_UCMD_ALU_SHLLN	= 8'h3C;
parameter[7:0] BSR_UCMD_ALU_SHLRN	= 8'h3D;
parameter[7:0] BSR_UCMD_ALU_SHARN	= 8'h3E;
parameter[7:0] BSR_UCMD_ALU_SHLD	= 8'h3F;

parameter[7:0] BSR_UCMD_ALU_ADD2	= 8'h40;	//Rn=Rm+DLR
parameter[7:0] BSR_UCMD_ALU_SUB2	= 8'h41;	//Rn=Rm-DLR
parameter[7:0] BSR_UCMD_ALU_AND2	= 8'h42;	//Rn=Rm&DLR
parameter[7:0] BSR_UCMD_ALU_OR2		= 8'h43;	//Rn=Rm|DLR
parameter[7:0] BSR_UCMD_ALU_XOR2	= 8'h44;	//Rn=Rm^DLR

parameter[7:0] BSR_UCMD_ALU_LDISH16	= 8'h45;	//


parameter[7:0] BSR_UCMD_IX_NOP		= 8'h00;
parameter[7:0] BSR_UCMD_IX_RTS		= 8'h01;
parameter[7:0] BSR_UCMD_IX_SLEEP	= 8'h02;
parameter[7:0] BSR_UCMD_IX_BREAK	= 8'h03;
parameter[7:0] BSR_UCMD_IX_CLRT		= 8'h04;
parameter[7:0] BSR_UCMD_IX_SETT		= 8'h05;
parameter[7:0] BSR_UCMD_IX_CLRS		= 8'h06;
parameter[7:0] BSR_UCMD_IX_SETS		= 8'h07;
parameter[7:0] BSR_UCMD_IX_NOTT		= 8'h08;
parameter[7:0] BSR_UCMD_IX_NOTS		= 8'h09;

parameter[7:0] BSR_UCMD_IX_RTE		= 8'h0C;

`endif
