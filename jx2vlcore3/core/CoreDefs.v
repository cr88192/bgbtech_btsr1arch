`ifndef HAS_COREDEFS
`define HAS_COREDEFS

parameter[5:0] JX2_GR_R0		= 6'h00;
parameter[5:0] JX2_GR_R1		= 6'h01;
parameter[5:0] JX2_GR_R2		= 6'h02;
parameter[5:0] JX2_GR_R3		= 6'h03;
parameter[5:0] JX2_GR_R4		= 6'h04;
parameter[5:0] JX2_GR_R5		= 6'h05;
parameter[5:0] JX2_GR_R6		= 6'h06;
parameter[5:0] JX2_GR_R7		= 6'h07;
parameter[5:0] JX2_GR_R8		= 6'h08;
parameter[5:0] JX2_GR_R9		= 6'h09;
parameter[5:0] JX2_GR_R10		= 6'h0A;
parameter[5:0] JX2_GR_R11		= 6'h0B;
parameter[5:0] JX2_GR_R12		= 6'h0C;
parameter[5:0] JX2_GR_R13		= 6'h0D;
parameter[5:0] JX2_GR_R14		= 6'h0E;
parameter[5:0] JX2_GR_R15		= 6'h0F;
parameter[5:0] JX2_GR_R16		= 6'h10;
parameter[5:0] JX2_GR_R17		= 6'h11;
parameter[5:0] JX2_GR_R18		= 6'h12;
parameter[5:0] JX2_GR_R19		= 6'h13;
parameter[5:0] JX2_GR_R20		= 6'h14;
parameter[5:0] JX2_GR_R21		= 6'h15;
parameter[5:0] JX2_GR_R22		= 6'h16;
parameter[5:0] JX2_GR_R23		= 6'h17;
parameter[5:0] JX2_GR_R24		= 6'h18;
parameter[5:0] JX2_GR_R25		= 6'h19;
parameter[5:0] JX2_GR_R26		= 6'h1A;
parameter[5:0] JX2_GR_R27		= 6'h1B;
parameter[5:0] JX2_GR_R28		= 6'h1C;
parameter[5:0] JX2_GR_R29		= 6'h1D;
parameter[5:0] JX2_GR_R30		= 6'h1E;
parameter[5:0] JX2_GR_R31		= 6'h1F;

parameter[5:0] JX2_GR_DLR		= 6'h20;
parameter[5:0] JX2_GR_DHR		= 6'h21;

parameter[5:0] JX2_GR_ZZR		= 6'h2E;
parameter[5:0] JX2_GR_SP		= 6'h2F;

parameter[5:0] JX2_GR_ELR		= 6'h30;
parameter[5:0] JX2_GR_EHR		= 6'h31;
parameter[5:0] JX2_GR_IMM		= 6'h32;
parameter[5:0] JX2_GR_LR		= 6'h33;
parameter[5:0] JX2_GR_PC		= 6'h34;
parameter[5:0] JX2_GR_GBR		= 6'h35;
parameter[5:0] JX2_GR_TBR		= 6'h36;

parameter[5:0] JX2_GR_BP		= 6'h3F;


parameter[5:0] JX2_CR_PC		= 6'h20;
parameter[5:0] JX2_CR_LR		= 6'h21;
parameter[5:0] JX2_CR_SR		= 6'h22;
parameter[5:0] JX2_CR_VBR		= 6'h23;
parameter[5:0] JX2_CR_SPC		= 6'h24;
parameter[5:0] JX2_CR_SSP		= 6'h25;
parameter[5:0] JX2_CR_GBR		= 6'h26;
parameter[5:0] JX2_CR_TBR		= 6'h27;
parameter[5:0] JX2_CR_TTB		= 6'h28;
parameter[5:0] JX2_CR_TEA		= 6'h29;
parameter[5:0] JX2_CR_MMCR		= 6'h2A;
parameter[5:0] JX2_CR_EXSR		= 6'h2B;
parameter[5:0] JX2_CR_STTB		= 6'h2C;
parameter[5:0] JX2_CR_KRR		= 6'h2D;

parameter[5:0] JX2_CR_ZZR		= 6'h2E;

parameter[2:0] JX2_SCHM_DLR		= 3'h0;
parameter[2:0] JX2_SCHM_DHR		= 3'h1;
parameter[2:0] JX2_SCHM_SP		= 3'h2;
parameter[2:0] JX2_SCHM_BP		= 3'h3;
parameter[2:0] JX2_SCHM_ELR		= 3'h4;
parameter[2:0] JX2_SCHM_EHR		= 3'h5;

parameter[1:0] JX2_IXC_AL	= 2'b00;	//Execute Always
parameter[1:0] JX2_IXC_NV	= 2'b01;	//Execute Never
parameter[1:0] JX2_IXC_CT	= 2'b10;	//Execute If True
parameter[1:0] JX2_IXC_CF	= 2'b11;	//Execute If False

parameter[  3:0] UV4_XX			= 4'hX;	//
parameter[  4:0] UV5_XX			= 5'hXX;	//
parameter[  5:0] UV6_XX			= 6'hXX;	//
parameter[  6:0] UV7_XX			= 7'hXX;	//
parameter[  7:0] UV8_XX			= 8'hXX;	//
parameter[  8:0] UV9_XX			= 9'hXXX;	//
parameter[  9:0] UV10_XX		= 10'hXXX;	//
parameter[ 10:0] UV11_XX		= 11'hXXX;	//
parameter[ 11:0] UV12_XX		= 12'hXXX;	//

parameter[ 15:0] UV16_XX		= 16'hXXXX;	//
parameter[ 27:0] UV28_XX		= 28'hXXXXXXX;	//
parameter[ 31:0] UV32_XX		= 32'hXXXXXXXX;	//
parameter[ 32:0] UV33_XX		= 33'hXXXXXXXXX;	//
parameter[ 47:0] UV48_XX		= 48'hXXXXXXXX_XXXX;	//
parameter[ 63:0] UV64_XX		= 64'hXXXXXXXX_XXXXXXXX;	//
parameter[ 64:0] UV65_XX		= 65'hX_XXXXXXXX_XXXXXXXX;	//
parameter[ 95:0] UV96_XX		= 96'hXXXXXXXX_XXXXXXXX_XXXXXXXX;	//
parameter[127:0] UV128_XX		= { UV64_XX, UV64_XX };		//
parameter[159:0] UV160_XX		= { UV32_XX, UV128_XX };	//
parameter[191:0] UV192_XX		= { UV64_XX, UV128_XX };	//
parameter[223:0] UV224_XX		= { UV96_XX, UV128_XX };	//
parameter[255:0] UV256_XX		= { UV128_XX, UV128_XX };	//

parameter[319:0] UV320_XX		= { UV256_XX, UV64_XX };	//
parameter[383:0] UV384_XX		= { UV256_XX, UV128_XX };	//

parameter      UV1_00			= 1'h0;	//
parameter      UV1_FF			= 1'h1;	//

parameter[1:0] UV2_00			= 2'h0;	//
parameter[1:0] UV2_FF			= 2'h3;	//

parameter[3:0] UV4_00			= 4'h0;	//
parameter[3:0] UV4_FF			= 4'hF;	//

parameter[4:0] UV5_00			= 5'h00;	//
parameter[4:0] UV5_FF			= 5'h1F;	//
parameter[5:0] UV6_00			= 6'h00;	//
parameter[5:0] UV6_FF			= 6'h3F;	//
parameter[6:0] UV7_00			= 7'h00;	//
parameter[6:0] UV7_FF			= 7'h7F;	//

parameter[7:0] UV8_00			= 8'h00;	//
parameter[7:0] UV8_FF			= 8'hFF;	//

parameter[8:0] UV9_00			= 9'h000;	//
parameter[8:0] UV9_FF			= 9'h1FF;	//

parameter[11:0] UV12_00			= 12'h000;	//
parameter[11:0] UV12_FF			= 12'hFFF;	//

parameter[12:0] UV13_00			= 13'h0000;	//
parameter[12:0] UV13_FF			= 13'h1FFF;	//

parameter[14:0] UV15_00			= 15'h0000;	//
parameter[14:0] UV15_FF			= 15'h7FFF;	//

parameter[15:0] UV16_00			= 16'h0000;	//
parameter[15:0] UV16_FF			= 16'hFFFF;	//

parameter[16:0] UV17_00			= 17'h00000;	//
parameter[16:0] UV17_FF			= 17'h1FFFF;	//

parameter[19:0] UV20_00			= 20'h00000;	//
parameter[19:0] UV20_FF			= 20'hFFFFF;	//

parameter[20:0] UV21_00			= 21'h000000;	//
parameter[20:0] UV21_FF			= 21'h1FFFFF;	//

parameter[22:0] UV23_00			= 23'h000000;	//
parameter[22:0] UV23_FF			= 23'h7FFFFF;	//

parameter[23:0] UV24_00			= 24'h000000;	//
parameter[23:0] UV24_FF			= 24'hFFFFFF;	//

parameter[24:0] UV25_00			= 25'h0000000;	//
parameter[24:0] UV25_FF			= 25'h1FFFFFF;	//

parameter[25:0] UV26_00			= 26'h0000000;	//
parameter[25:0] UV26_FF			= 26'h3FFFFFF;	//

parameter[26:0] UV27_00			= 27'h0000000;	//
parameter[26:0] UV27_FF			= 27'h7FFFFFF;	//

parameter[27:0] UV28_00			= 28'h0000000;	//
parameter[27:0] UV28_FF			= 28'hFFFFFFF;	//

parameter[28:0] UV29_00			= 29'h00000000;	//
parameter[28:0] UV29_FF			= 29'h1FFFFFFF;	//

parameter[29:0] UV30_00			= 30'h00000000;	//
parameter[29:0] UV30_FF			= 30'h3FFFFFFF;	//

parameter[31:0] UV32_00			= 32'h00000000;	//
parameter[31:0] UV32_FF			= 32'hFFFFFFFF;	//

parameter[32:0] UV33_00			= 33'h000000000;	//
parameter[32:0] UV33_FF			= 33'h1FFFFFFFF;	//

parameter[47:0] UV48_00			= 48'h0000_00000000;	//
parameter[47:0] UV48_FF			= 48'hFFFF_FFFFFFFF;	//

parameter[55:0] UV56_00			= 56'h000000_00000000;	//
parameter[55:0] UV56_FF			= 56'hFFFFFF_FFFFFFFF;	//

parameter[56:0] UV57_00			= 57'h00000000_00000000;	//
parameter[56:0] UV57_FF			= 57'h01FFFFFF_FFFFFFFF;	//

parameter[62:0] UV63_00			= 63'h00000000_00000000;	//
parameter[62:0] UV63_FF			= 63'h7FFFFFFF_FFFFFFFF;	//

parameter[63:0] UV64_00			= 64'h00000000_00000000;	//
parameter[63:0] UV64_FF			= 64'hFFFFFFFF_FFFFFFFF;	//


parameter[1:0] UMEM_OK_READY	= 2'h0;		//Ready (Awaiting Request)
parameter[1:0] UMEM_OK_OK		= 2'h1;		//OK (Request Successful)
parameter[1:0] UMEM_OK_HOLD		= 2'h2;		//Hold (IO Not Ready)
parameter[1:0] UMEM_OK_FAULT	= 2'h3;		//FAULT (Request Failed)


parameter[4:0] UMEM_OPM_READY	= 5'b00000;		//Ready/Idle

// parameter[4:0] UMEM_OPM_CTRLF	= 5'b00010;		//Control Flow
parameter[4:0] UMEM_OPM_INVTLB	= 5'b00010;		//Flush TLB
parameter[4:0] UMEM_OPM_LDTLB	= 5'b00011;		//Load TLB
parameter[4:0] UMEM_OPM_FLUSHIS	= 5'b00100;		//Flush I$ Request
parameter[4:0] UMEM_OPM_FLUSHDS	= 5'b00101;		//Flush D$ Request

parameter[4:0] UMEM_OPM_RD_SB	= 5'b01000;		//Read Byte
parameter[4:0] UMEM_OPM_RD_SW	= 5'b01001;		//Read Word
parameter[4:0] UMEM_OPM_RD_SL	= 5'b01010;		//Read DWord
parameter[4:0] UMEM_OPM_RD_Q	= 5'b01011;		//Read QWord
parameter[4:0] UMEM_OPM_RD_UB	= 5'b01100;		//Read Byte (ZX)
parameter[4:0] UMEM_OPM_RD_UW	= 5'b01101;		//Read Word (ZX)
parameter[4:0] UMEM_OPM_RD_UL	= 5'b01110;		//Read DWord (ZX)
parameter[4:0] UMEM_OPM_RD_TILE	= 5'b01111;		//Read Tile

parameter[4:0] UMEM_OPM_WR_SB	= 5'b10000;		//Write Byte
parameter[4:0] UMEM_OPM_WR_SW	= 5'b10001;		//Write Word
parameter[4:0] UMEM_OPM_WR_SL	= 5'b10010;		//Write DWord
parameter[4:0] UMEM_OPM_WR_Q	= 5'b10011;		//Write QWord
parameter[4:0] UMEM_OPM_WR_UB	= 5'b10100;		//Write Byte (Resv)
parameter[4:0] UMEM_OPM_WR_UW	= 5'b10101;		//Write Word (Resv)
parameter[4:0] UMEM_OPM_WR_UL	= 5'b10110;		//Write DWord (Resv)
parameter[4:0] UMEM_OPM_WR_TILE	= 5'b10111;		//Write Tile

parameter[4:0] UMEM_OPM_SW_TILE	= 5'b11111;		//Swap Tiles (Full Duplex)


parameter[2:0] JX2_BTY_SB		= 3'b000;
parameter[2:0] JX2_BTY_SW		= 3'b001;
parameter[2:0] JX2_BTY_SL		= 3'b010;
parameter[2:0] JX2_BTY_SQ		= 3'b011;
parameter[2:0] JX2_BTY_UB		= 3'b100;
parameter[2:0] JX2_BTY_UW		= 3'b101;
parameter[2:0] JX2_BTY_UL		= 3'b110;
parameter[2:0] JX2_BTY_UQ		= 3'b111;

parameter[3:0] JX2_ITY_SB		= 4'b0000;
parameter[3:0] JX2_ITY_SW		= 4'b0001;
parameter[3:0] JX2_ITY_SL		= 4'b0010;
parameter[3:0] JX2_ITY_SQ		= 4'b0011;
parameter[3:0] JX2_ITY_UB		= 4'b0100;
parameter[3:0] JX2_ITY_UW		= 4'b0101;
parameter[3:0] JX2_ITY_UL		= 4'b0110;
parameter[3:0] JX2_ITY_UQ		= 4'b0111;
parameter[3:0] JX2_ITY_NB		= 4'b1000;
parameter[3:0] JX2_ITY_NW		= 4'b1001;
parameter[3:0] JX2_ITY_NL		= 4'b1010;
parameter[3:0] JX2_ITY_NQ		= 4'b1011;
parameter[3:0] JX2_ITY_XB		= 4'b1100;
parameter[3:0] JX2_ITY_XW		= 4'b1101;
parameter[3:0] JX2_ITY_XL		= 4'b1110;
parameter[3:0] JX2_ITY_XQ		= 4'b1111;

/*
REG, Bz:
	SB: ZZR, Rx, Rx
	SW: ZZR, Rn, Rn
	SL: Rx, ZZR, Rx
	SQ: Rn, ZZR, Rn

	XB: ZZR, Rx, DLR
	XW: ZZR, Rn, DLR
	SL: Rx, FixImm, Rx
	SQ: Rn, FixImm, Rn

REG, Fz:
	SB: // Ro, ZZR, Ro
	SB: ZZR, Ro, Ro
	SW: ZZR, Rm, Rn

	UB: Ro, ZZR, Ro

REGREG, Bz:
	SB: Rm, Rn / Rm, DLR, Rn
	SW: Rj, Rn / Rj, DLR, Rn
	SL: Rm, Rk / Rm, DLR, Rk
	SQ: Rj, Rk / Rj, DLR, Rk
	
	UB: Rm, Cn
	UW: Cm, Rn
	UL: Rm, Sn
	UQ: Sn, Rn
	
	NB: Rn, Rm, Rn
	NW: Rm, Rn, Rn

REGREG, Fz
	SB: Rm, Ro, Rn
	UB: Rm, Rn, Rn
	NB: Rn, Rm, Rn

	UL: Rm, Cn, Cn
	UQ: Cm, Rn, Rn


IMM8REG, Fz
	SB: Fzeo_iiii		Ro, Imm16s, Ro
	SW: Fzeo_iiii		Imm16s, Ro, Ro

	UB: Fzeo_jjjj		Ro, Imm16u, Ro
	UW: Fzeo_jjjj		Imm16u, Ro, Ro

	NW: Fzeo_jjjj		Imm16n, Ro, Ro

	SQ: Fzze_zznz_iiii	Rn, Imm17s, Rn
	UQ: Fzze_zznz_iiii	Rn, Imm17u, Rn
	NQ: Fzze_zznz_iiii	Rn, Imm17n, Rn

*/

parameter[4:0] JX2_FMID_INV				= 5'h00;	//? (Invalid Opcode) / Custom
parameter[4:0] JX2_FMID_Z				= 5'h01;	//OOOO	-
parameter[4:0] JX2_FMID_REG				= 5'h02;	//OOnO	Rn | #0, Rn
parameter[4:0] JX2_FMID_IMM8			= 5'h03;	//OOii	#imm8s, DLR
parameter[4:0] JX2_FMID_REGREG			= 5'h04;	//OOnm	Rm, Rn
parameter[4:0] JX2_FMID_IMM8REG			= 5'h05;	//Onii	#imm8s, Rn
parameter[4:0] JX2_FMID_IMM8Z			= 5'h06;	//OOjj	#imm8u, DLR
parameter[4:0] JX2_FMID_IMM8N			= 5'h07;	//OOjj	#imm8n, DLR
// parameter[4:0] JX2_FMID_REGSTREG		= 5'h08;	//OOnm  Rm, (Rn)
parameter[4:0] JX2_FMID_LDREGREG		= 5'h09;	//OOnm  (Rm), Rn
// parameter[4:0] JX2_FMID_REGSTDRREG		= 5'h0A;	//OOnm  Rm, (Rn, DLR)
parameter[4:0] JX2_FMID_LDDRREGREG		= 5'h0B;	//OOnm  (Rm, DLR), Rn
// parameter[4:0] JX2_FMID_REGSTDRPC		= 5'h0C;	//OOnO  Rn, (PC, DLR)
parameter[4:0] JX2_FMID_LDDRPCREG		= 5'h0D;	//OOnO  (PC, DLR), Rn
parameter[4:0] JX2_FMID_IMM4ZREG		= 5'h0E;	//OOnj  #imm4u, Rn
parameter[4:0] JX2_FMID_IMM4NREG		= 5'h0F;	//OOnj  #imm4n, Rn
parameter[4:0] JX2_FMID_REGIMMREG		= 5'h10;	//F2nm_Oejj	Rm, Imm9, Rn
parameter[4:0] JX2_FMID_REGPC			= 5'h11;	//OOnO	(PC, Rn)
parameter[4:0] JX2_FMID_DRREG			= 5'h12;	//OOnO	DLR, Rn
parameter[4:0] JX2_FMID_PCDISP8			= 5'h13;	//OOdd	(PC, disp8s)

parameter[4:0] JX2_FMID_IMM12Z			= 5'h16;	//Ojjj	#imm12u, DLR
parameter[4:0] JX2_FMID_IMM12N			= 5'h17;	//Ojjj	#imm12n, DLR
// parameter[4:0] JX2_FMID_REGSTDLR		= 5'h18;	//OOnO  Rn, (DLR)
// parameter[4:0] JX2_FMID_REGSTREGDISP	= 5'h18;	//OOnm  Rm, (Rn)
parameter[4:0] JX2_FMID_LDDLRREG		= 5'h19;	//OOnO  (DLR), Rn
parameter[4:0] JX2_FMID_LDREGDISPREG	= 5'h19;	//OOnm  (Rm), Rn
// parameter[4:0] JX2_FMID_REGSTDI4SP		= 5'h1E;	//OOnj  Rn, (SP, disp4)
parameter[4:0] JX2_FMID_LDDI4SPREG		= 5'h1F;	//OOnj  (SP, disp4), Rn

parameter[4:0] JX2_FMID_REGSTREG		= JX2_FMID_LDREGREG;
parameter[4:0] JX2_FMID_REGSTDRREG		= JX2_FMID_LDDRREGREG;
parameter[4:0] JX2_FMID_REGSTDRPC		= JX2_FMID_LDDRPCREG;

parameter[4:0] JX2_FMID_REGSTDLR		= JX2_FMID_LDDLRREG;
parameter[4:0] JX2_FMID_REGSTREGDISP	= JX2_FMID_LDREGDISPREG;
parameter[4:0] JX2_FMID_REGSTDI4SP		= JX2_FMID_LDDI4SPREG;


parameter[5:0] JX2_UCMD_NOP			= 6'h00;		//
parameter[5:0] JX2_UCMD_OP_IXS		= 6'h01;		//1R
parameter[5:0] JX2_UCMD_OP_IXT		= 6'h02;		//0R
parameter[5:0] JX2_UCMD_LEA_MR		= 6'h03;		//Load
parameter[5:0] JX2_UCMD_MOV_RM		= 6'h04;		//Store
parameter[5:0] JX2_UCMD_MOV_MR		= 6'h05;		//Load
parameter[5:0] JX2_UCMD_PUSHX		= 6'h06;		//
parameter[5:0] JX2_UCMD_POPX		= 6'h07;		//
parameter[5:0] JX2_UCMD_FMOV_RM		= 6'h08;		//FPU Store
parameter[5:0] JX2_UCMD_FMOV_MR		= 6'h09;		//FPU Load
parameter[5:0] JX2_UCMD_ADDSP		= 6'h0A;		//ADD Imm, SP

parameter[5:0] JX2_UCMD_INVOP		= 6'h0F;		//
parameter[5:0] JX2_UCMD_ALU3		= 6'h10;		//ALU Command (3R)
parameter[5:0] JX2_UCMD_ALUCMP		= 6'h11;		//ALU Compare
parameter[5:0] JX2_UCMD_UNARY		= 6'h12;		//ALU Unary Operator
parameter[5:0] JX2_UCMD_SWAPN		= 6'h13;		//SWAP
parameter[5:0] JX2_UCMD_SHAD3		= 6'h14;		//
parameter[5:0] JX2_UCMD_SHLD3		= 6'h15;		//
parameter[5:0] JX2_UCMD_SHADQ3		= 6'h16;		//
parameter[5:0] JX2_UCMD_SHLDQ3		= 6'h17;		//
parameter[5:0] JX2_UCMD_CONV_RR		= 6'h18;		//
parameter[5:0] JX2_UCMD_MOV_RC		= 6'h19;		//
parameter[5:0] JX2_UCMD_MOV_CR		= 6'h1A;		//
parameter[5:0] JX2_UCMD_MOV_IR		= 6'h1B;		//
parameter[5:0] JX2_UCMD_BRA			= 6'h1C;		//
parameter[5:0] JX2_UCMD_BSR			= 6'h1D;		//
parameter[5:0] JX2_UCMD_JMP			= 6'h1E;		//
parameter[5:0] JX2_UCMD_JSR			= 6'h1F;		//
parameter[5:0] JX2_UCMD_MUL3		= 6'h20;		//Multiply
parameter[5:0] JX2_UCMD_SHLLN		= 6'h21;		//Fixed Shifts
parameter[5:0] JX2_UCMD_FPU3		= 6'h22;		//FPU 3R Ops
parameter[5:0] JX2_UCMD_FLDCX		= 6'h23;		//FPU Convert (GR->FPR)
parameter[5:0] JX2_UCMD_FSTCX		= 6'h24;		//FPU Convert (FPR->GR)
parameter[5:0] JX2_UCMD_FIXS		= 6'h25;		//FPU Unary Op (FPR)
parameter[5:0] JX2_UCMD_FCMP		= 6'h26;		//FPU Compare
parameter[5:0] JX2_UCMD_MULW3		= 6'h27;		//Multiply (Word)


parameter[5:0] JX2_UCIX_ALU_ADD		= 6'h20;		//ALU ADD
parameter[5:0] JX2_UCIX_ALU_SUB		= 6'h21;		//ALU SUB
parameter[5:0] JX2_UCIX_ALU_ADC		= 6'h22;		//ALU ADC
parameter[5:0] JX2_UCIX_ALU_SBB		= 6'h23;		//ALU SBB
parameter[5:0] JX2_UCIX_ALU_TSTQ	= 6'h24;		//ALU TST
parameter[5:0] JX2_UCIX_ALU_AND		= 6'h25;		//ALU AND
parameter[5:0] JX2_UCIX_ALU_OR		= 6'h26;		//ALU OR
parameter[5:0] JX2_UCIX_ALU_XOR		= 6'h27;		//ALU XOR
parameter[5:0] JX2_UCIX_ALU_CMPQNE	= 6'h28;		//ALU CMPQNE
parameter[5:0] JX2_UCIX_ALU_CMPQHS	= 6'h29;		//ALU CMPQHS
parameter[5:0] JX2_UCIX_ALU_CMPQGE	= 6'h2A;		//ALU CMPQGE
parameter[5:0] JX2_UCIX_ALU_NOR		= 6'h2B;		//ALU NOR
parameter[5:0] JX2_UCIX_ALU_CMPQEQ	= 6'h2C;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_CMPQHI	= 6'h2D;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_CMPQGT	= 6'h2E;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_CSELT	= 6'h2F;		//ALU CSELT

parameter[5:0] JX2_UCIX_ALU_ADDSL	= 6'h00;		//ALU ADDSL
parameter[5:0] JX2_UCIX_ALU_SUBSL	= 6'h01;		//ALU SUBSL

parameter[5:0] JX2_UCIX_ALU_ADDUL	= 6'h40;		//ALU ADDUL
parameter[5:0] JX2_UCIX_ALU_SUBUL	= 6'h41;		//ALU SUBUL

parameter[5:0] JX2_UCIX_ALU_TST		= 6'h04;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_CMPNE	= 6'h08;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_CMPHS	= 6'h09;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_CMPGE	= 6'h0A;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_CMPEQ	= 6'h0C;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_CMPHI	= 6'h0D;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_CMPGT	= 6'h0E;		//ALU Command

parameter[5:0] JX2_UCIX_ALU_PADDL	= 6'h30;		//Packed ALU ADD
parameter[5:0] JX2_UCIX_ALU_PSUBL	= 6'h31;		//Packed ALU SUB
parameter[5:0] JX2_UCIX_ALU_PADCL	= 6'h32;		//Packed ALU ADC
parameter[5:0] JX2_UCIX_ALU_PSBBL	= 6'h33;		//Packed ALU SBB

parameter[5:0] JX2_UCIX_ALU_PCSELT	= 6'h3F;		//Packed CSELT

parameter[5:0] JX2_UCIX_ALUN_CLZ	= 6'h00;		//Packed CSELT
parameter[5:0] JX2_UCIX_ALUN_CLZQ	= 6'h20;		//Packed CSELT

parameter[5:0] JX2_UCIX_PUSH_GR		= 6'h00;		//GPR
parameter[5:0] JX2_UCIX_PUSH_CR		= 6'h01;		//Control Reg
parameter[5:0] JX2_UCIX_PUSH_FR		= 6'h02;		//FPR
parameter[5:0] JX2_UCIX_PUSH_SR		= 6'h03;		//Shadow Reg

parameter[5:0] JX2_UCIX_CONV_EXTSB	= 6'h00;		//
parameter[5:0] JX2_UCIX_CONV_EXTSW	= 6'h01;		//
parameter[5:0] JX2_UCIX_CONV_EXTSL	= 6'h02;		//
parameter[5:0] JX2_UCIX_CONV_MOV	= 6'h03;		//MOV
parameter[5:0] JX2_UCIX_CONV_EXTUB	= 6'h04;		//
parameter[5:0] JX2_UCIX_CONV_EXTUW	= 6'h05;		//
parameter[5:0] JX2_UCIX_CONV_EXTUL	= 6'h06;		//
parameter[5:0] JX2_UCIX_CONV_NOT	= 6'h07;		//NOT

parameter[5:0] JX2_UCIX_CONV_CLZ	= 6'h08;		//NOT
parameter[5:0] JX2_UCIX_CONV_CLZQ	= 6'h09;		//NOT

parameter[5:0] JX2_UCIX_MUL3_MUL3S	= 6'h00;		//
parameter[5:0] JX2_UCIX_MUL3_MUL3U	= 6'h01;		//
parameter[5:0] JX2_UCIX_MUL3_MULS	= 6'h02;		//
parameter[5:0] JX2_UCIX_MUL3_MULU	= 6'h03;		//

parameter[5:0] JX2_UCIX_LDI_LDIX	= 6'h00;		//
parameter[5:0] JX2_UCIX_LDI_LDISH8	= 6'h01;		//
parameter[5:0] JX2_UCIX_LDI_LDISH16	= 6'h02;		//
parameter[5:0] JX2_UCIX_LDI_LDISH32	= 6'h03;		//


parameter[5:0] JX2_UCIX_FPU_FADD	= 6'h00;		//FPU ADD
parameter[5:0] JX2_UCIX_FPU_FSUB	= 6'h01;		//FPU SUB
parameter[5:0] JX2_UCIX_FPU_FMUL	= 6'h02;		//FPU MUL
parameter[5:0] JX2_UCIX_FPU_FDIV	= 6'h03;		//FPU DIV
parameter[5:0] JX2_UCIX_FPU_FMOV	= 6'h04;		//FPU MOV

parameter[5:0] JX2_UCIX_FPU_FADD_G	= 6'h10;		//FPU ADD (GFP)
parameter[5:0] JX2_UCIX_FPU_FSUB_G	= 6'h11;		//FPU SUB (GFP)
parameter[5:0] JX2_UCIX_FPU_FMUL_G	= 6'h12;		//FPU MUL (GFP)

parameter[5:0] JX2_UCIX_FPU_CMPNE	= 6'h08;		//ALU Command
parameter[5:0] JX2_UCIX_FPU_CMPGE	= 6'h0A;		//ALU Command
parameter[5:0] JX2_UCIX_FPU_CMPEQ	= 6'h0C;		//ALU Command
parameter[5:0] JX2_UCIX_FPU_CMPGT	= 6'h0E;		//ALU Command

parameter[5:0] JX2_UCIX_FPU_CMPNE_G	= 6'h18;		//ALU Command
parameter[5:0] JX2_UCIX_FPU_CMPGE_G	= 6'h1A;		//ALU Command
parameter[5:0] JX2_UCIX_FPU_CMPEQ_G	= 6'h1C;		//ALU Command
parameter[5:0] JX2_UCIX_FPU_CMPGT_G	= 6'h1E;		//ALU Command

parameter[5:0] JX2_UCIX_FPIX_FNEG	= 6'h00;		//FPU ADD
parameter[5:0] JX2_UCIX_FPIX_FABS	= 6'h01;		//FPU SUB
parameter[5:0] JX2_UCIX_FPIX_FRCP	= 6'h02;		//FPU SUB
parameter[5:0] JX2_UCIX_FPIX_FSQRT	= 6'h03;		//FPU SUB
parameter[5:0] JX2_UCIX_FPIX_FRCPA	= 6'h04;		//FPU SUB
parameter[5:0] JX2_UCIX_FPIX_FSQRTA	= 6'h05;		//FPU SUB

parameter[5:0] JX2_UCIX_FPIX_FNEG_G	= 6'h10;		//FPU Negate
parameter[5:0] JX2_UCIX_FPIX_FABS_G	= 6'h11;		//FPU Absolute

parameter[5:0] JX2_UCIX_FPCX_S		= 6'h00;		//FPU Single
parameter[5:0] JX2_UCIX_FPCX_D		= 6'h01;		//FPU Double
parameter[5:0] JX2_UCIX_FPCX_I		= 6'h02;		//FPU Int
parameter[5:0] JX2_UCIX_FPCX_H		= 6'h03;		//FPU Half
parameter[5:0] JX2_UCIX_FPCX_S2		= 6'h08;		//FPU Single (High)

parameter[5:0] JX2_UCIX_FPCX_SG		= 6'h10;		//FPU Single
parameter[5:0] JX2_UCIX_FPCX_DG		= 6'h11;		//FPU Double
parameter[5:0] JX2_UCIX_FPCX_IG		= 6'h12;		//FPU Int
parameter[5:0] JX2_UCIX_FPCX_HG		= 6'h13;		//FPU Half
parameter[5:0] JX2_UCIX_FPCX_S2G	= 6'h18;		//FPU Single (High)

parameter[5:0] JX2_UCIX_IXT_NOP		= 6'h00;		//No-Op
parameter[5:0] JX2_UCIX_IXT_SLEEP	= 6'h01;		//Sleep until Interrupt
parameter[5:0] JX2_UCIX_IXT_BREAK	= 6'h02;		//Breakpoint
parameter[5:0] JX2_UCIX_IXT_CLRT	= 6'h03;		//Clear SR.T
parameter[5:0] JX2_UCIX_IXT_SETT	= 6'h04;		//Set SR.T
parameter[5:0] JX2_UCIX_IXT_CLRS	= 6'h05;		//Clear SR.S
parameter[5:0] JX2_UCIX_IXT_SETS	= 6'h06;		//Set SR.S
parameter[5:0] JX2_UCIX_IXT_NOTT	= 6'h07;		//Invert SR.T
parameter[5:0] JX2_UCIX_IXT_NOTS	= 6'h08;		//Invert SR.S
parameter[5:0] JX2_UCIX_IXT_RTE		= 6'h09;		//Return From Exception
parameter[5:0] JX2_UCIX_IXT_TRAPA	= 6'h0A;		//Trap
parameter[5:0] JX2_UCIX_IXT_PLDMSK	= 6'h0B;		//
parameter[5:0] JX2_UCIX_IXT_CPUID	= 6'h0C;		//CPU ID
parameter[5:0] JX2_UCIX_IXT_WEXMD	= 6'h0D;		//WEX Profile

parameter[5:0] JX2_UCIX_IXS_NOP		= 6'h00;		//No-Op
parameter[5:0] JX2_UCIX_IXS_MOVT	= 6'h01;		//Copy SR.T to Reg
parameter[5:0] JX2_UCIX_IXS_MOVNT	= 6'h02;		//Copy !SR.T to Reg
parameter[5:0] JX2_UCIX_IXS_LDSRMSK	= 6'h03;		//?

`define def_true

`define jx2_enable_mmu
`define jx2_enable_fpu

// `define jx2_enable_fprs		//enable dedicated FPU registers.

`ifdef jx2_enable_fpu
`ifndef jx2_enable_fprs
`define jx2_enable_gfp			//FPU works via GPRs
`endif
`endif

// `define jx2_sprs_elrehr			//ELR/EHR/BP as special registers?
`define jx2_bra2stage				//Use 2-stage branch initiation

// `define jx2_enable_swapn		//Enable SWxP.x ops
// `define jx2_enable_shlln		//Enable SHLLn / SHLRn Ops

`define jx2_shlln_shadq			//Route SHLLn through SHAD.Q

// `define jx2_enable_gsv			//Enable GSV (Packed Integer) stuff.

// `define jx2_enable_prebra			//Enable GSV (Packed Integer) stuff.

`define jx2_enable_ops16
// `define jx2_enable_ops48

// `define jx2_enable_aluunary
// `define jx2_enable_clz
// `define jx2_enable_addsp

// `define jx2_enable_wex2w				//Enable 2-wide WEX support
`define jx2_enable_wex3w				//Enable 3-wide WEX support

`define jx2_enable_wex					//Enable WEX support (general)

`define jx2_cpu_halfclock;				//Reduce CPU core clock speed by half


`ifdef jx2_enable_wex
/* If we are using WEX, assume 16 and 48 bit ops also exist. */
`ifndef jx2_enable_ops16
`define jx2_enable_ops16
`endif
`ifndef jx2_enable_ops48
`define jx2_enable_ops48
`endif
`endif

`define jx2_ddr_bl64b			//DDR interface is 64-bit (DDR2)

// `define jx2_expand_l1sz			//Make L1 bigger

// `define jx2_reduce_l1sz		//Make L1 smaller
// `define jx2_reduce_l2sz

// `define jx2_merge_shadq		//Merge SHAD and SHAD.Q

// `define jx2_debug_l1ds		//Debug prints for L1 data cache

// `define jx2_debug_ldst		//Debug prints for load/store

// `define jx2_debug_expipe		//Debug execute pipeline
// `define jx2_debug_exopipe	//Debug execute pipeline (EX2 only)

// `define jx2_debug_alu		//Debug ALU

`ifdef jx2_expand_l1sz
parameter[255:0] JX2_L1_FLUSHMSK	= UV256_FF;		//
`else
`ifdef jx2_reduce_l1sz
parameter[15:0] JX2_L1_FLUSHMSK	= UV16_FF;		//
`else
parameter[63:0] JX2_L1_FLUSHMSK	= UV64_FF;		//
`endif
`endif

`ifdef jx2_bra2stage
parameter[7:0] JX2_BRA_FLUSHMSK	= 8'h1F;		//
`else
parameter[7:0] JX2_BRA_FLUSHMSK	= 8'h0F;		//
`endif

`endif