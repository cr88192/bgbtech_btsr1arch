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

`ifndef HAS_COREDEFS
`define HAS_COREDEFS

`define def_true

`include "CoreConfig.v"

`ifdef jx2_enable_xgpr

`define		reg_gpr		reg[6:0]
`define		wire_gpr	wire[6:0]
`define		input_gpr	input[6:0]
`define		output_gpr	output[6:0]

parameter[6:0] JX2_GR_GPR_Z		= 7'b0zzzzzz;

parameter[6:0] JX2_GR_R0		= 7'h00;
parameter[6:0] JX2_GR_R1		= 7'h01;
parameter[6:0] JX2_GR_R2		= 7'h02;
parameter[6:0] JX2_GR_R3		= 7'h03;
parameter[6:0] JX2_GR_R4		= 7'h04;
parameter[6:0] JX2_GR_R5		= 7'h05;
parameter[6:0] JX2_GR_R6		= 7'h06;
parameter[6:0] JX2_GR_R7		= 7'h07;
parameter[6:0] JX2_GR_R8		= 7'h08;
parameter[6:0] JX2_GR_R9		= 7'h09;
parameter[6:0] JX2_GR_R10		= 7'h0A;
parameter[6:0] JX2_GR_R11		= 7'h0B;
parameter[6:0] JX2_GR_R12		= 7'h0C;
parameter[6:0] JX2_GR_R13		= 7'h0D;
parameter[6:0] JX2_GR_R14		= 7'h0E;
parameter[6:0] JX2_GR_R15		= 7'h0F;
parameter[6:0] JX2_GR_R16		= 7'h10;
parameter[6:0] JX2_GR_R17		= 7'h11;
parameter[6:0] JX2_GR_R18		= 7'h12;
parameter[6:0] JX2_GR_R19		= 7'h13;
parameter[6:0] JX2_GR_R20		= 7'h14;
parameter[6:0] JX2_GR_R21		= 7'h15;
parameter[6:0] JX2_GR_R22		= 7'h16;
parameter[6:0] JX2_GR_R23		= 7'h17;
parameter[6:0] JX2_GR_R24		= 7'h18;
parameter[6:0] JX2_GR_R25		= 7'h19;
parameter[6:0] JX2_GR_R26		= 7'h1A;
parameter[6:0] JX2_GR_R27		= 7'h1B;
parameter[6:0] JX2_GR_R28		= 7'h1C;
parameter[6:0] JX2_GR_R29		= 7'h1D;
parameter[6:0] JX2_GR_R30		= 7'h1E;
parameter[6:0] JX2_GR_R31		= 7'h1F;

parameter[6:0] JX2_GR_R32		= 7'h20;
parameter[6:0] JX2_GR_R33		= 7'h21;
parameter[6:0] JX2_GR_R34		= 7'h22;
parameter[6:0] JX2_GR_R35		= 7'h23;
parameter[6:0] JX2_GR_R36		= 7'h24;
parameter[6:0] JX2_GR_R37		= 7'h25;
parameter[6:0] JX2_GR_R38		= 7'h26;
parameter[6:0] JX2_GR_R39		= 7'h27;
parameter[6:0] JX2_GR_R40		= 7'h28;
parameter[6:0] JX2_GR_R41		= 7'h29;
parameter[6:0] JX2_GR_R42		= 7'h2A;
parameter[6:0] JX2_GR_R43		= 7'h2B;
parameter[6:0] JX2_GR_R44		= 7'h2C;
parameter[6:0] JX2_GR_R45		= 7'h2D;
parameter[6:0] JX2_GR_R46		= 7'h2E;
parameter[6:0] JX2_GR_R47		= 7'h2F;
parameter[6:0] JX2_GR_R48		= 7'h30;
parameter[6:0] JX2_GR_R49		= 7'h31;
parameter[6:0] JX2_GR_R50		= 7'h32;
parameter[6:0] JX2_GR_R51		= 7'h33;
parameter[6:0] JX2_GR_R52		= 7'h34;
parameter[6:0] JX2_GR_R53		= 7'h35;
parameter[6:0] JX2_GR_R54		= 7'h36;
parameter[6:0] JX2_GR_R55		= 7'h37;
parameter[6:0] JX2_GR_R56		= 7'h38;
parameter[6:0] JX2_GR_R57		= 7'h39;
parameter[6:0] JX2_GR_R58		= 7'h3A;
parameter[6:0] JX2_GR_R59		= 7'h3B;
parameter[6:0] JX2_GR_R60		= 7'h3C;
parameter[6:0] JX2_GR_R61		= 7'h3D;
parameter[6:0] JX2_GR_R62		= 7'h3E;
parameter[6:0] JX2_GR_R63		= 7'h3F;

parameter[6:0] JX2_GR_DLR		= 7'h40;
parameter[6:0] JX2_GR_DHR		= 7'h41;

parameter[6:0] JX2_GR_PC_HI		= 7'h42;
parameter[6:0] JX2_GR_VBR_HI	= 7'h43;
parameter[6:0] JX2_GR_SPC_HI	= 7'h44;
// parameter[6:0] JX2_GR_SSP_HI	= 7'h45;
parameter[6:0] JX2_GR_GBR_HI	= 7'h46;
// parameter[6:0] JX2_GR_SP_HI		= 7'h47;
// parameter[6:0] JX2_GR_PC_HI		= 7'h48;
// parameter[6:0] JX2_GR_LR_HI		= 7'h49;

parameter[6:0] JX2_GR_R8IMMH	= 7'h4C;		//Shift Immed, High Part
parameter[6:0] JX2_GR_R8IMML	= 7'h4D;		//Shift Immed, Low Part
parameter[6:0] JX2_GR_JIMM		= 7'h4E;
parameter[6:0] JX2_GR_SP		= 7'h4F;

parameter[6:0] JX2_GR_DLR2		= 7'h50;
parameter[6:0] JX2_GR_DHR2		= 7'h51;

parameter[6:0] JX2_GR_PC2_HI	= 7'h52;
parameter[6:0] JX2_GR_RBSEL		= 7'h53;
// parameter[6:0] JX2_GR_VBR2_HI	= 7'h53;
parameter[6:0] JX2_GR_SPC2_HI	= 7'h54;
// parameter[6:0] JX2_GR_SSP2_HI	= 7'h55;

parameter[6:0] JX2_GR_BPC		= 7'h55;		//Base PC

parameter[6:0] JX2_GR_GBR2_HI	= 7'h56;
// parameter[6:0] JX2_GR_SP2_HI	= 7'h57;
// parameter[6:0] JX2_GR_PC2_HI	= 7'h58;
// parameter[6:0] JX2_GR_LR2_HI	= 7'h59;

parameter[6:0] JX2_GR_FPIMM56F	= 7'h5A;		//FP Immed, Imm56f
parameter[6:0] JX2_GR_FPIMM56VF	= 7'h5B;		//FP Immed, 4x S.E5.F8

parameter[6:0] JX2_GR_FPIMM16	= 7'h5C;		//FP Immed, 16-bit
// parameter[6:0] JX2_GR_FPIMM10	= 7'h5D;		//FP Immed, 10-bit
parameter[6:0] JX2_GR_IMM_HI	= 7'h5D;		//Immed in high 32 bits.
parameter[6:0] JX2_GR_IMM_RP	= 7'h5E;		//Immed via Rp
parameter[6:0] JX2_GR_SP2		= 7'h5F;

parameter[6:0] JX2_GR_PC		= 7'h60;
parameter[6:0] JX2_GR_LR		= 7'h61;
parameter[6:0] JX2_GR_SR		= 7'h62;
parameter[6:0] JX2_GR_VBR		= 7'h63;
parameter[6:0] JX2_GR_SPC		= 7'h64;
parameter[6:0] JX2_GR_SSP		= 7'h65;
parameter[6:0] JX2_GR_GBR		= 7'h66;
parameter[6:0] JX2_GR_TBR		= 7'h67;
parameter[6:0] JX2_GR_TTB		= 7'h68;
parameter[6:0] JX2_GR_TEA		= 7'h69;
parameter[6:0] JX2_GR_MMCR		= 7'h6A;
parameter[6:0] JX2_GR_EXSR		= 7'h6B;
parameter[6:0] JX2_GR_STTB		= 7'h6C;
parameter[6:0] JX2_GR_KRR		= 7'h6D;

parameter[6:0] JX2_GR_PC2		= 7'h70;
parameter[6:0] JX2_GR_LR2		= 7'h71;
parameter[6:0] JX2_GR_SR2		= 7'h72;
parameter[6:0] JX2_GR_TEAH		= 7'h73;
parameter[6:0] JX2_GR_SPC2		= 7'h74;
parameter[6:0] JX2_GR_SSP2		= 7'h75;
parameter[6:0] JX2_GR_GBR2		= 7'h76;
parameter[6:0] JX2_GR_TBR2		= 7'h77;

parameter[6:0] JX2_GR_VIPT		= 7'h7C;
parameter[6:0] JX2_GR_KRR2		= 7'h7D;
parameter[6:0] JX2_GR_IMM		= 7'h7E;
parameter[6:0] JX2_GR_ZZR		= 7'h7F;


parameter[6:0] JX2_CR_PC		= 7'h60;
parameter[6:0] JX2_CR_LR		= 7'h61;
parameter[6:0] JX2_CR_SR		= 7'h62;
parameter[6:0] JX2_CR_VBR		= 7'h63;
parameter[6:0] JX2_CR_SPC		= 7'h64;
parameter[6:0] JX2_CR_SSP		= 7'h65;
parameter[6:0] JX2_CR_GBR		= 7'h66;
parameter[6:0] JX2_CR_TBR		= 7'h67;
parameter[6:0] JX2_CR_TTB		= 7'h68;
parameter[6:0] JX2_CR_TEA		= 7'h69;
parameter[6:0] JX2_CR_MMCR		= 7'h6A;
parameter[6:0] JX2_CR_EXSR		= 7'h6B;
parameter[6:0] JX2_CR_STTB		= 7'h6C;
parameter[6:0] JX2_CR_KRR		= 7'h6D;

parameter[6:0] JX2_CR_TEAH		= 7'h73;

parameter[6:0] JX2_CR_VIPT		= 7'h7C;
parameter[6:0] JX2_CR_KRR2		= 7'h7D;
parameter[6:0] JX2_CR_IMM		= 7'h7E;
parameter[6:0] JX2_CR_ZZR		= 7'h7F;

`else

`define		reg_gpr		reg[5:0]
`define		wire_gpr	wire[5:0]
`define		input_gpr	input[5:0]
`define		output_gpr	output[5:0]

parameter[5:0] JX2_GR_GPR_Z		= 6'b0zzzzz;

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

parameter[5:0] JX2_GR_R32		= 6'h20;
parameter[5:0] JX2_GR_R33		= 6'h21;
parameter[5:0] JX2_GR_R34		= 6'h22;
parameter[5:0] JX2_GR_R35		= 6'h23;
parameter[5:0] JX2_GR_R36		= 6'h24;
parameter[5:0] JX2_GR_R37		= 6'h25;
parameter[5:0] JX2_GR_R38		= 6'h26;
parameter[5:0] JX2_GR_R39		= 6'h27;
parameter[5:0] JX2_GR_R40		= 6'h28;
parameter[5:0] JX2_GR_R41		= 6'h29;
parameter[5:0] JX2_GR_R42		= 6'h2A;
parameter[5:0] JX2_GR_R43		= 6'h2B;
parameter[5:0] JX2_GR_R44		= 6'h2C;
parameter[5:0] JX2_GR_R45		= 6'h2D;
parameter[5:0] JX2_GR_R46		= 6'h2E;
parameter[5:0] JX2_GR_R47		= 6'h2F;

parameter[5:0] JX2_GR_R48		= 6'h30;
parameter[5:0] JX2_GR_R49		= 6'h31;
parameter[5:0] JX2_GR_R50		= 6'h32;
parameter[5:0] JX2_GR_R51		= 6'h33;
parameter[5:0] JX2_GR_R52		= 6'h34;
parameter[5:0] JX2_GR_R53		= 6'h35;
parameter[5:0] JX2_GR_R54		= 6'h36;
parameter[5:0] JX2_GR_R55		= 6'h37;
parameter[5:0] JX2_GR_R56		= 6'h38;
parameter[5:0] JX2_GR_R57		= 6'h39;
parameter[5:0] JX2_GR_R58		= 6'h3A;
parameter[5:0] JX2_GR_R59		= 6'h3B;
parameter[5:0] JX2_GR_R60		= 6'h3C;
parameter[5:0] JX2_GR_R61		= 6'h3D;
parameter[5:0] JX2_GR_R62		= 6'h3E;
parameter[5:0] JX2_GR_R63		= 6'h3F;

parameter[5:0] JX2_GR_DLR		= 6'h20;
parameter[5:0] JX2_GR_DHR		= 6'h21;

// parameter[5:0] JX2_GR_R4IMM1	= 6'h2C;		//Shift-4 Immed, Rot1
// parameter[5:0] JX2_GR_R4IMM2	= 6'h2D;		//Shift-4 Immed, Rot2
parameter[5:0] JX2_GR_R8IMMH	= 6'h2C;		//Shift Immed, High Part
parameter[5:0] JX2_GR_R8IMML	= 6'h2D;		//Shift Immed, Low Part
parameter[5:0] JX2_GR_JIMM		= 6'h2E;
parameter[5:0] JX2_GR_SP		= 6'h2F;

parameter[5:0] JX2_GR_PC		= 6'h30;
parameter[5:0] JX2_GR_LR		= 6'h31;
parameter[5:0] JX2_GR_SR		= 6'h32;
parameter[5:0] JX2_GR_VBR		= 6'h33;
parameter[5:0] JX2_GR_SPC		= 6'h34;
parameter[5:0] JX2_GR_SSP		= 6'h35;
parameter[5:0] JX2_GR_GBR		= 6'h36;
parameter[5:0] JX2_GR_TBR		= 6'h37;
parameter[5:0] JX2_GR_TTB		= 6'h38;
parameter[5:0] JX2_GR_TEA		= 6'h39;
parameter[5:0] JX2_GR_MMCR		= 6'h3A;
parameter[5:0] JX2_GR_EXSR		= 6'h3B;
parameter[5:0] JX2_GR_STTB		= 6'h3C;
parameter[5:0] JX2_GR_KRR		= 6'h3D;
parameter[5:0] JX2_GR_IMM		= 6'h3E;
parameter[5:0] JX2_GR_ZZR		= 6'h3F;


parameter[5:0] JX2_CR_PC		= 6'h30;
parameter[5:0] JX2_CR_LR		= 6'h31;
parameter[5:0] JX2_CR_SR		= 6'h32;
parameter[5:0] JX2_CR_VBR		= 6'h33;
parameter[5:0] JX2_CR_SPC		= 6'h34;
parameter[5:0] JX2_CR_SSP		= 6'h35;
parameter[5:0] JX2_CR_GBR		= 6'h36;
parameter[5:0] JX2_CR_TBR		= 6'h37;
parameter[5:0] JX2_CR_TTB		= 6'h38;
parameter[5:0] JX2_CR_TEA		= 6'h39;
parameter[5:0] JX2_CR_MMCR		= 6'h3A;
parameter[5:0] JX2_CR_EXSR		= 6'h3B;
parameter[5:0] JX2_CR_STTB		= 6'h3C;
parameter[5:0] JX2_CR_KRR		= 6'h3D;
parameter[5:0] JX2_CR_IMM		= 6'h3E;
parameter[5:0] JX2_CR_ZZR		= 6'h3F;

`endif


parameter[2:0] JX2_SCHM_DLR		= 3'h0;
parameter[2:0] JX2_SCHM_DHR		= 3'h1;
parameter[2:0] JX2_SCHM_SP		= 3'h2;
parameter[2:0] JX2_SCHM_BP		= 3'h3;
parameter[2:0] JX2_SCHM_ELR		= 3'h4;
parameter[2:0] JX2_SCHM_EHR		= 3'h5;

parameter[2:0] JX2_IXC_AL	= 3'b000;	//Execute Always
parameter[2:0] JX2_IXC_NV	= 3'b001;	//Execute Never
parameter[2:0] JX2_IXC_CT	= 3'b010;	//Execute If True
parameter[2:0] JX2_IXC_CF	= 3'b011;	//Execute If False
parameter[2:0] JX2_IXC_CTS	= 3'b100;	//Execute If True (SR.S)
parameter[2:0] JX2_IXC_CFS	= 3'b101;	//Execute If False (SR.S)

parameter[2:0] JX2_IUC_SC	= 3'b000;	//Scalar
parameter[2:0] JX2_IUC_WX	= 3'b001;	//Wide
parameter[2:0] JX2_IUC_WT	= 3'b010;	//Wide+True
parameter[2:0] JX2_IUC_WF	= 3'b011;	//Wide+False

parameter[2:0] JX2_IUC_WA	= 3'b100;	//Wide-Address
parameter[2:0] JX2_IUC_WXA	= 3'b101;	//Wide-Address

parameter[  3:0] UV4_XX			= 4'hX;	//
parameter[  4:0] UV5_XX			= 5'hXX;	//
parameter[  5:0] UV6_XX			= 6'hXX;	//
parameter[  6:0] UV7_XX			= 7'hXX;	//
parameter[  7:0] UV8_XX			= 8'hXX;	//
parameter[  8:0] UV9_XX			= 9'hXXX;	//
parameter[  9:0] UV10_XX		= 10'hXXX;	//
parameter[ 10:0] UV11_XX		= 11'hXXX;	//
parameter[ 11:0] UV12_XX		= 12'hXXX;	//
parameter[ 12:0] UV13_XX		= 13'hXXXX;	//
parameter[ 13:0] UV14_XX		= 14'hXXXX;	//
parameter[ 14:0] UV15_XX		= 15'hXXXX;	//
parameter[ 15:0] UV16_XX		= 16'hXXXX;	//

parameter[ 27:0] UV28_XX		= 28'hXXXXXXX;	//
parameter[ 31:0] UV32_XX		= 32'hXXXXXXXX;	//
parameter[ 32:0] UV33_XX		= 33'hXXXXXXXXX;	//
parameter[ 35:0] UV36_XX		= 36'hXXXXXXXXX;	//
parameter[ 39:0] UV40_XX		= 40'hXXXXXX_XXXX;	//
parameter[ 43:0] UV44_XX		= 44'hXXXXXXX_XXXX;	//
parameter[ 47:0] UV48_XX		= 48'hXXXXXXXX_XXXX;	//

parameter[ 59:0] UV60_XX		= 60'hXXXXXXX_XXXXXXXX;	//
parameter[ 61:0] UV62_XX		= 62'hXXXXXXXX_XXXXXXXX;	//

parameter[ 63:0] UV64_XX		= 64'hXXXXXXXX_XXXXXXXX;	//
parameter[ 64:0] UV65_XX		= 65'hX_XXXXXXXX_XXXXXXXX;	//
parameter[ 65:0] UV66_XX		= 66'hX_XXXXXXXX_XXXXXXXX;	//
parameter[ 95:0] UV96_XX		= 96'hXXXXXXXX_XXXXXXXX_XXXXXXXX;	//
parameter[127:0] UV128_XX		= { UV64_XX, UV64_XX };		//
parameter[159:0] UV160_XX		= { UV32_XX, UV128_XX };	//
parameter[191:0] UV192_XX		= { UV64_XX, UV128_XX };	//
parameter[223:0] UV224_XX		= { UV96_XX, UV128_XX };	//
parameter[255:0] UV256_XX		= { UV128_XX, UV128_XX };	//

parameter[319:0] UV320_XX		= { UV256_XX, UV64_XX };	//
parameter[383:0] UV384_XX		= { UV256_XX, UV128_XX };	//
parameter[511:0] UV512_XX		= { UV256_XX, UV256_XX };	//

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

parameter[9:0] UV10_00			= 10'h000;	//
parameter[9:0] UV10_FF			= 10'h3FF;	//

parameter[10:0] UV11_00			= 11'h000;	//
parameter[10:0] UV11_FF			= 11'h7FF;	//

parameter[11:0] UV12_00			= 12'h000;	//
parameter[11:0] UV12_FF			= 12'hFFF;	//

parameter[12:0] UV13_00			= 13'h0000;	//
parameter[12:0] UV13_FF			= 13'h1FFF;	//

parameter[13:0] UV14_00			= 14'h0000;	//
parameter[13:0] UV14_FF			= 14'h3FFF;	//

parameter[14:0] UV15_00			= 15'h0000;	//
parameter[14:0] UV15_FF			= 15'h7FFF;	//

parameter[15:0] UV16_00			= 16'h0000;	//
parameter[15:0] UV16_FF			= 16'hFFFF;	//

parameter[16:0] UV17_00			= 17'h00000;	//
parameter[16:0] UV17_FF			= 17'h1FFFF;	//

parameter[17:0] UV18_00			= 18'h00000;	//
parameter[17:0] UV18_FF			= 18'h3FFFF;	//

parameter[18:0] UV19_00			= 19'h00000;	//
parameter[18:0] UV19_FF			= 19'h7FFFF;	//

parameter[19:0] UV20_00			= 20'h00000;	//
parameter[19:0] UV20_FF			= 20'hFFFFF;	//

parameter[20:0] UV21_00			= 21'h000000;	//
parameter[20:0] UV21_FF			= 21'h1FFFFF;	//

parameter[21:0] UV22_00			= 22'h000000;	//
parameter[21:0] UV22_FF			= 22'h3FFFFF;	//

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

parameter[30:0] UV31_00			= 31'h00000000;	//
parameter[30:0] UV31_FF			= 31'h7FFFFFFF;	//

parameter[31:0] UV32_00			= 32'h00000000;	//
parameter[31:0] UV32_FF			= 32'hFFFFFFFF;	//

parameter[32:0] UV33_00			= 33'h000000000;	//
parameter[32:0] UV33_FF			= 33'h1FFFFFFFF;	//

parameter[33:0] UV34_00			= 34'h000000000;	//
parameter[33:0] UV34_FF			= 34'h3FFFFFFFF;	//

parameter[35:0] UV36_00			= 36'h0_00000000;	//
parameter[35:0] UV36_FF			= 36'hF_FFFFFFFF;	//

parameter[39:0] UV40_00			= 40'h00_00000000;	//
parameter[39:0] UV40_FF			= 40'hFF_FFFFFFFF;	//

parameter[43:0] UV44_00			= 44'h000_00000000;	//
parameter[43:0] UV44_FF			= 44'hFFF_FFFFFFFF;	//

parameter[47:0] UV48_00			= 48'h0000_00000000;	//
parameter[47:0] UV48_FF			= 48'hFFFF_FFFFFFFF;	//

parameter[53:0] UV54_00			= 54'h000000_00000000;	//
parameter[53:0] UV54_FF			= 54'h3FFFFF_FFFFFFFF;	//

parameter[55:0] UV56_00			= 56'h000000_00000000;	//
parameter[55:0] UV56_FF			= 56'hFFFFFF_FFFFFFFF;	//

parameter[56:0] UV57_00			= 57'h00000000_00000000;	//
parameter[56:0] UV57_FF			= 57'h01FFFFFF_FFFFFFFF;	//

parameter[57:0] UV58_00			= 58'h00000000_00000000;	//
parameter[57:0] UV58_FF			= 58'h03FFFFFF_FFFFFFFF;	//

parameter[58:0] UV59_00			= 59'h00000000_00000000;	//
parameter[58:0] UV59_FF			= 59'h07FFFFFF_FFFFFFFF;	//



parameter[59:0] UV60_00			= 60'h0000000_00000000;	//
parameter[59:0] UV60_FF			= 60'hFFFFFFF_FFFFFFFF;	//

parameter[61:0] UV62_00			= 62'h00000000_00000000;	//
parameter[61:0] UV62_FF			= 62'h3FFFFFFF_FFFFFFFF;	//

parameter[62:0] UV63_00			= 63'h00000000_00000000;	//
parameter[62:0] UV63_FF			= 63'h7FFFFFFF_FFFFFFFF;	//

parameter[63:0] UV64_00			= 64'h00000000_00000000;	//
parameter[63:0] UV64_FF			= 64'hFFFFFFFF_FFFFFFFF;	//

parameter[64:0] UV65_00			= 65'h000000000_00000000;	//
parameter[64:0] UV65_FF			= 65'h1FFFFFFFF_FFFFFFFF;	//

parameter[65:0] UV66_00			= 66'h000000000_00000000;	//
parameter[65:0] UV66_FF			= 66'h3FFFFFFFF_FFFFFFFF;	//

parameter[79:0] UV80_00			= { UV16_00, UV64_00 };	//
parameter[79:0] UV80_FF			= { UV16_FF, UV64_FF };	//

parameter[95:0] UV96_00			= { UV32_00, UV64_00 };	//
parameter[95:0] UV96_FF			= { UV32_FF, UV64_FF };	//

parameter[111:0] UV112_00		= { UV48_00, UV64_00 };	//
parameter[111:0] UV112_FF		= { UV48_FF, UV64_FF };	//

parameter[127:0] UV128_00		= { UV64_00, UV64_00 };	//
parameter[127:0] UV128_FF		= { UV64_FF, UV64_FF };	//

parameter[191:0] UV192_00		= { UV64_00, UV128_00 };	//
parameter[191:0] UV192_FF		= { UV64_FF, UV128_FF };	//

parameter[223:0] UV224_00		= { UV96_00, UV128_00 };	//
parameter[223:0] UV224_FF		= { UV96_FF, UV128_FF };	//

parameter[255:0] UV256_00		= { UV128_00, UV128_00 };	//
parameter[255:0] UV256_FF		= { UV128_FF, UV128_FF };	//

parameter[511:0] UV512_00		= { UV256_00, UV256_00 };	//
parameter[511:0] UV512_FF		= { UV256_FF, UV256_FF };	//


parameter[1:0] UMEM_OK_READY	= 2'h0;		//Ready (Awaiting Request)
parameter[1:0] UMEM_OK_OK		= 2'h1;		//OK (Request Successful)
parameter[1:0] UMEM_OK_HOLD		= 2'h2;		//Hold (IO Not Ready)
parameter[1:0] UMEM_OK_FAULT	= 2'h3;		//FAULT (Request Failed)


parameter[4:0] UMEM_OPM_READY	= 5'b00000;		//Ready/Idle
parameter[4:0] UMEM_OPM_FAULT	= 5'b00001;		//Access Fault
// parameter[4:0] UMEM_OPM_CTRLF	= 5'b00010;		//Control Flow
parameter[4:0] UMEM_OPM_INVTLB	= 5'b00010;		//Flush TLB
parameter[4:0] UMEM_OPM_LDTLB	= 5'b00011;		//Load TLB
parameter[4:0] UMEM_OPM_FLUSHIS	= 5'b00100;		//Flush I$ Request
parameter[4:0] UMEM_OPM_FLUSHDS	= 5'b00101;		//Flush D$ Request
parameter[4:0] UMEM_OPM_TRAP	= 5'b00110;		//Raise Interrupt
parameter[4:0] UMEM_OPM_LDACL	= 5'b00111;		//Raise Interrupt

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

parameter[4:0] UMEM_OPM_RD_BOUNCE	= 5'b01100;	//Read Request, Bounce


/*
MOV_MR:
  SB/SW/SL/SQ: Signed Byte/Word/DWord/QWord
  UB/UW/UL: Unsigned Byte/Word/DWord
  UQ: Pair (128-bit)
FMOV:
  SB: DWord, One Extended
  SW: Binary16 -> Binary64
  SL: Binary32 -> Binary64
  SQ: LDTEX
  UB: TWord, S-Ext
  UW: TWord, Z-Ext
  UL: TWord, High 48
  UQ: -
PMOV:
  SB: 2x Binary16 -> 2x Binary32
  SW: -
  SL: 4x FP8 (S.E4.F3) to 4x Binary16
  SQ: -
 */

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
	SQ: / Rn, ZZR, Rn
	SQ: Rn, ZZR, LR

	XB: ZZR, Rx, DLR
	XW: ZZR, Rn, DLR
	XL: Rx, FixImm, Rx
	XQ: Rn, FixImm, Rn

REG, Fz:
	SB: // Ro, ZZR, Ro
	SB: ZZR, Ro, Ro
	SW: ZZR, Rm, Rn

	UB: Ro, ZZR, Ro
	
	NB: / Co, ZZR, Co
	
	XB: / ZZR, Ro, DLR
	XB: ZZR, RoOrg, DLR
	XW: Ro, ZZR, LR

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
	SW: Rm, Q?Imm5u:Ro, Rn
	SL: Rm, Q?Imm5n:Ro, Rn
	SQ: Rm, Q?Disp5u:Ro, Rn

	UB: Rm, Rn, Rn
	UW: Rm, Rn, Rn
	UL: Rm, Cn, Cn
	UQ: Cm, Rn, Rn

	NB: Rn, Rm, Rn
	NW: Rn, ZZR, LR
	NL: 
	NQ: 

	XB: Rm, Ro, Rp->Rn
	XW: Rm, Ro, Imm->Rn

	XL: Rn, RmImm6u, Rn
	XQ: Rm, Ro, Rn, Imm8 (FPU)

REGREG, Fz+Au (Rm, Ro, Rp, Rn)
	SB: Rm, Ro, Rn, Rn
	SW: Rm, Q?Imm13u:Ro, Rn
	SL: Rm, Q?Imm13n:Ro, Rn

	UB: Rm, Rn, Rn, Rn
	UW: Rm, Rn, Imm8, Rn

	XB: Rm, Ro, Rp, Rn
	XW: Rm, Ro, Imm8, Rn

REGREG, RVI:
	SB: Rm, Ro, Rn

	UB: Rm, Rn, Rn
	UW: Ro, Rn, Rn
	UL: Rm, Cn, Cn
	UQ: Cm, Cm, Rn

	NB: Rn, Rm, Rn
	NW: Fm, Fo, Fp, Fn
	NL: Fm, Fm, Rn
	NQ: Rm, Rm, Fn
	
	XB: Fm, Fo, Fn
	XW: Fm, Fn, Fn
	XL: Fo, Fm, Fn
	XQ: Fo, Fm, Rn

REGIMMREG, Fz:
	SB: Rm, 0, Rn
	SW:	/ Rm, Imm9s, Rn
	SW:	Rm, Imm9us, Rn
	SL: Imm9us, Rm, Rn

	UB:	Rm, Imm9f/Imm16f, Rn
	UW:	Rm, Imm9u, Rn
	UL: Imm9u, Rm, Rn

	NB:	Rm, Imm5f, Rn
	NW:	Rm, Imm9n, Rn

	NL:	Rm, Imm5u, Rn
	NQ:	Imm5u, Rm, Rn

	XB: Rm, Imm9f/Imm32f/Imm57f, Rn
	XW:	Rm, Imm9u, Rn (Decoded like REGDISPREG)

REGIMMREG, RVI:
	SB: Rm, 0, Rn
	SW: Rm, Imm12s, Rn

	UW: Rm, Imm12u, Rn

	NB: Rm, 0, Rt, Rn
	NW: Rm, Imm12n, Rn


REGPC, Fz:
	SB: (PC, Ro), ZZR
	SW: (PC, Ro), LR
	SL: Rn, ZZR, Disp11s

	UB: Rn, Rm, disp8s
	UW: Rn, ZZR, Disp8s

	UL: Rn, ZZR, Disp10u

	NB: Rm, Rn, disp8s
	NW: ZZR, Rn, Disp8s

	NL: Rn, ZZR, Disp10n

LDREGDISPREG, Fz:
	SW: (Rm, Disp9u/17s), Rn
	SL: (Rm, Disp9u/17s), Rn  (Keeps Scale)

	UB: (Rm, Disp5u), Rn
	UL: (Rm, Disp9u/17s), Cn

	NB: (Rm, Disp5u), Rn  (Keeps Scale)

	NW: (Rm, Disp5u*2), Rn
	NL: (Rm, Disp5u*4), Rn
	NQ: (Rm, Disp5u*8), Rn

LDDRREGREG, Fz:
	UB: (Rm, Ro), Rn
	UL: (Rm, Ro), Cn

	NW: (Rm, Ro*2), Rn
	NL: (Rm, Ro*4), Rn
	NQ: (Rm, Ro*8), Rn

IMM8REG, Fz
	SB: Fzeo_iiii		Ro, Imm16s, Ro
	SW: Fzeo_iiii		Imm16s, Ro, Ro
	SL: Fzeo_iiii		Imm16s, DLR, DLR
	SQ: / Fzze_zznz_iiii	Rn, Imm10s, Rn
	SQ: Fzeo_Ziii		Ro, Imm12s, Ro

	UB: Fzeo_jjjj		Ro, Imm16u, Ro
	UW: Fzeo_jjjj		Imm16u, Ro, Ro
	UL: Fznz_zejj		Imm10u, Rn, Rn
	UQ: Fznz_zejj		Rn, Imm10u, Rn

	NB: Fzeo_jjjj		Ro, Imm16n, Ro
	NW: Fzeo_jjjj		Imm16n, Ro, Ro
	NL: Fznz_zejj		Imm10n, Rn, Rn
	NQ: Fznz_zejj		Rn, Imm10n, Rn

	XB: Fznz_zejj		ImmFp10, Rn, Rn
	XW: Fzeo_jjjj		ImmFp16, Ro, Ro
	XL: Fznz_zzjj		(GBR, Disp16u), Rn
	XQ: Fznz_zejj		Rn, ImmFp10, Rn

IMM10REG, Fz
	SB: 
	SW: 
	SL: 
	SQ: 

	UB: Fznz_zejj		Imm10u, ZZR, Rn
	UW: Fznz_zejj		Rbs10b, Rbs6p, Rn
	UL: Fznz_zejj		Imm10u, Rn, Rn
	UQ: Fznz_zejj		Rn, Imm10u, Rn

	NB: Fznz_zejj		Imm10n, ZZR, Rn
	NW: Fznz_zejj		Rn, Rbs6p, Rbs10b
	NL: Fznz_zejj		Imm10n, Rn, Rn
	NQ: Fznz_zejj		Rn, Imm10n, Rn

	XB: Fznz_zejj		ImmFp10, Rn, Rn
	XW: 
	XL: Fznz_zzjj		(GBR, Disp10u), Rn
	XQ: Fznz_zejj		Rn, ImmFp10, Rn

JX2_FMID_LDREGDISPREG:
	UB: (Rm, Disp5), Rn
	SW: (Rm, Disp9), Rn

*/

parameter[4:0] JX2_FMID_INV				= 5'h00;	//? (Invalid Opcode) / Custom
parameter[4:0] JX2_FMID_Z				= 5'h01;	//OOOO	-
parameter[4:0] JX2_FMID_REG				= 5'h02;	//OOnO	Rn | #0, Rn
parameter[4:0] JX2_FMID_IMM8			= 5'h03;	//OOii	#imm8s, DLR
parameter[4:0] JX2_FMID_REGREG			= 5'h04;	//OOnm	Rm, Rn
parameter[4:0] JX2_FMID_IMM8REG			= 5'h05;	//Onii	#imm8s, Rn
parameter[4:0] JX2_FMID_IMM8Z			= 5'h06;	//OOjj	#imm8u, DLR
parameter[4:0] JX2_FMID_IMM8N			= 5'h07;	//OOjj	#imm8n, DLR
parameter[4:0] JX2_FMID_IMM10REG		= 5'h08;	//Onii	#imm8s, Rn

parameter[4:0] JX2_FMID_LDREGREG		= 5'h09;	//OOnm  (Rm), Rn

parameter[4:0] JX2_FMID_LDDRREGREG		= 5'h0B;	//OOnm  (Rm, DLR), Rn

parameter[4:0] JX2_FMID_LDDRPCREG		= 5'h0D;	//OOnO  (PC, DLR), Rn
parameter[4:0] JX2_FMID_IMM4ZREG		= 5'h0E;	//OOnj  #imm4u, Rn
parameter[4:0] JX2_FMID_IMM4NREG		= 5'h0F;	//OOnj  #imm4n, Rn
parameter[4:0] JX2_FMID_REGIMMREG		= 5'h10;	//F2nm_Oejj	Rm, Imm9, Rn
parameter[4:0] JX2_FMID_REGPC			= 5'h11;	//OOnO	(PC, Rn)
parameter[4:0] JX2_FMID_DRREG			= 5'h12;	//OOnO	DLR, Rn
parameter[4:0] JX2_FMID_PCDISP8			= 5'h13;	//OOdd	(PC, disp8s)

parameter[4:0] JX2_FMID_IMM12Z			= 5'h16;	//Ojjj	#imm12u, DLR
parameter[4:0] JX2_FMID_IMM12N			= 5'h17;	//Ojjj	#imm12n, DLR

parameter[4:0] JX2_FMID_LDDLRREG		= 5'h19;	//OOnO  (DLR), Rn
parameter[4:0] JX2_FMID_LDREGDISPREG	= 5'h19;	//OOnm  (Rm, Disp), Rn
parameter[4:0] JX2_FMID_REGSTREGDISP	= 5'h1A;	//OOnm  (Rm, Disp), Rn

parameter[4:0] JX2_FMID_LDDI4SPREG		= 5'h1F;	//OOnj  (SP, disp4), Rn

parameter[4:0] JX2_FMID_REGSTREG		= JX2_FMID_LDREGREG;
parameter[4:0] JX2_FMID_REGSTDRREG		= JX2_FMID_LDDRREGREG;
parameter[4:0] JX2_FMID_REGSTDRPC		= JX2_FMID_LDDRPCREG;

parameter[4:0] JX2_FMID_REGSTDLR		= JX2_FMID_LDDLRREG;
// parameter[4:0] JX2_FMID_REGSTREGDISP	= JX2_FMID_LDREGDISPREG;
parameter[4:0] JX2_FMID_REGSTDI4SP		= JX2_FMID_LDDI4SPREG;

parameter[3:0] JX2_BZIMM_NONE		= 4'h0;
parameter[3:0] JX2_BZIMM_IMM8S		= 4'h1;
parameter[3:0] JX2_BZIMM_FIX10S		= 4'h2;

parameter[3:0] JX2_BZIMM_IMM4U		= 4'h4;
parameter[3:0] JX2_BZIMM_IMM8U		= 4'h5;
parameter[3:0] JX2_BZIMM_IMM12U		= 4'h6;
parameter[3:0] JX2_BZIMM_DISP3U		= 4'h7;

parameter[3:0] JX2_BZIMM_IMM4N		= 4'h8;
parameter[3:0] JX2_BZIMM_IMM8N		= 4'h9;
parameter[3:0] JX2_BZIMM_IMM12N		= 4'hA;


parameter[4:0] JX2_FMIMM_NONE		= 5'h00;
parameter[4:0] JX2_FMIMM_DISP9U		= 5'h01;
parameter[4:0] JX2_FMIMM_DISP5U		= 5'h02;
parameter[4:0] JX2_FMIMM_IMM8AU		= 5'h03;
// parameter[4:0] JX2_FMIMM_IMM5U		= JX2_FMIMM_DISP5U;

parameter[4:0] JX2_FMIMM_IMM6U		= 5'h04;
parameter[4:0] JX2_FMIMM_IMM9U		= 5'h05;
parameter[4:0] JX2_FMIMM_IMM10U		= 5'h06;
parameter[4:0] JX2_FMIMM_IMM16U		= 5'h07;

parameter[4:0] JX2_FMIMM_DISP20S	= 5'h08;
parameter[4:0] JX2_FMIMM_IMM9N		= 5'h09;
parameter[4:0] JX2_FMIMM_IMM10N		= 5'h0A;
parameter[4:0] JX2_FMIMM_IMM16N		= 5'h0B;

parameter[4:0] JX2_FMIMM_IMM24Z		= 5'h0C;
parameter[4:0] JX2_FMIMM_IMM24N		= 5'h0D;
parameter[4:0] JX2_FMIMM_IMM16S		= 5'h0E;
parameter[4:0] JX2_FMIMM_DISP8S		= 5'h0F;

// parameter[3:0] JX2_FMIMM_DISP11S	= 5'h0C;

parameter[4:0] JX2_FMIMM_DISP11S	= 5'h10;
parameter[4:0] JX2_FMIMM_IMM5U		= 5'h12;
parameter[4:0] JX2_FMIMM_DISP20X	= 5'h13;

parameter[3:0] JX2_RVCIMM_NONE		= 4'h0;
parameter[3:0] JX2_RVCIMM_IMM6ALU	= 4'h1;
parameter[3:0] JX2_RVCIMM_IMM4LW	= 4'h2;
parameter[3:0] JX2_RVCIMM_IMM4QW	= 4'h3;
parameter[3:0] JX2_RVCIMM_IMM6LWLD	= 4'h4;
parameter[3:0] JX2_RVCIMM_IMM6QWLD	= 4'h5;
parameter[3:0] JX2_RVCIMM_IMM6LWST	= 4'h6;
parameter[3:0] JX2_RVCIMM_IMM6QWST	= 4'h7;

parameter[3:0] JX2_RVCIMM_IMM6A16SP	= 4'h8;
parameter[3:0] JX2_RVCIMM_IMM6LUI	= 4'h9;
parameter[3:0] JX2_RVCIMM_IMM11J	= 4'hA;
parameter[3:0] JX2_RVCIMM_IMM8JCC	= 4'hB;


parameter[3:0] JX2_RVIMM_NONE		= 4'h0;
parameter[3:0] JX2_RVIMM_IMMAU		= 4'h1;
parameter[3:0] JX2_RVIMM_IMM20UI	= 4'h2;	//LUI
parameter[3:0] JX2_RVIMM_DISP20J	= 4'h3;	//JAL
parameter[3:0] JX2_RVIMM_DISP12LD	= 4'h4;
parameter[3:0] JX2_RVIMM_DISP12ST	= 4'h5;
parameter[3:0] JX2_RVIMM_DISP12B	= 4'h6;	//12-bit, Bcc
parameter[3:0] JX2_RVIMM_IMM17S		= 4'h7;	//LUI

parameter[3:0] JX2_RVIMM_IMM12S		= 4'h4;



parameter[8:0] JX2_UCMDSP_BRANOP	= 9'b001_000000;		//


parameter[5:0] JX2_UCMD_NOP			= 6'h00;		//
parameter[5:0] JX2_UCMD_OP_IXS		= 6'h01;		//1R
parameter[5:0] JX2_UCMD_OP_IXT		= 6'h02;		//0R
parameter[5:0] JX2_UCMD_LEA_MR		= 6'h03;		//Load
parameter[5:0] JX2_UCMD_MOV_RM		= 6'h04;		//Store
parameter[5:0] JX2_UCMD_MOV_MR		= 6'h05;		//Load

parameter[5:0] JX2_UCMD_CONV3_RR	= 6'h06;		//Convert (Via ALU)
parameter[5:0] JX2_UCMD_PMOV_RM		= 6'h07;		//SIMD Conv+Store

// parameter[5:0] JX2_UCMD_PUSHX	= 6'h06;		//
// parameter[5:0] JX2_UCMD_POPX		= 6'h07;		//
parameter[5:0] JX2_UCMD_FMOV_RM		= 6'h08;		//FPU Store
parameter[5:0] JX2_UCMD_FMOV_MR		= 6'h09;		//FPU Load
parameter[5:0] JX2_UCMD_ALUCMP3R	= 6'h0A;		//ALU Command (3R)
parameter[5:0] JX2_UCMD_QMULDIV		= 6'h0B;		//Slow MUL/DIV

parameter[5:0] JX2_UCMD_CONV2_RR	= 6'h0C;		//Convert (Via ALU)
parameter[5:0] JX2_UCMD_FPUV4SF		= 6'h0D;		//FPU Vec4SF Unit
parameter[5:0] JX2_UCMD_PMOV_MR		= 6'h0E;		//SIMD Load+Conv
parameter[5:0] JX2_UCMD_INVOP		= 6'h0F;		//

parameter[5:0] JX2_UCMD_ALU3		= 6'h10;		//ALU Command (3R)
parameter[5:0] JX2_UCMD_ALUCMP		= 6'h11;		//ALU Compare
parameter[5:0] JX2_UCMD_UNARY		= 6'h12;		//ALU Unary Operator
parameter[5:0] JX2_UCMD_SWAPN		= 6'h13;		//SWAP

`ifdef jx2_merge_shadq
parameter[5:0] JX2_UCMD_SHAD3		= 6'h14;		//
parameter[5:0] JX2_UCMD_SHLD3		= 6'h14;		//
parameter[5:0] JX2_UCMD_SHADQ3		= 6'h14;		//
parameter[5:0] JX2_UCMD_SHLDQ3		= 6'h14;		//
`else
parameter[5:0] JX2_UCMD_SHAD3		= 6'h14;		//
parameter[5:0] JX2_UCMD_SHLD3		= 6'h15;		//
parameter[5:0] JX2_UCMD_SHADQ3		= 6'h16;		//
parameter[5:0] JX2_UCMD_SHLDQ3		= 6'h17;		//
`endif

parameter[5:0] JX2_UCMD_CONV_RR		= 6'h18;		//
parameter[5:0] JX2_UCMD_MOV_RC		= 6'h19;		//
parameter[5:0] JX2_UCMD_MOV_CR		= 6'h1A;		//
parameter[5:0] JX2_UCMD_MOV_IR		= 6'h1B;		//
parameter[5:0] JX2_UCMD_BRA			= 6'h1C;		//
parameter[5:0] JX2_UCMD_BSR			= 6'h1D;		//
parameter[5:0] JX2_UCMD_JMP			= 6'h1E;		//
parameter[5:0] JX2_UCMD_JSR			= 6'h1F;		//

parameter[5:0] JX2_UCMD_MOV_RR		= 6'h1A;		//

parameter[5:0] JX2_UCMD_MUL3		= 6'h20;		//Multiply
parameter[5:0] JX2_UCMD_SHLLN		= 6'h21;		//Fixed Shifts
parameter[5:0] JX2_UCMD_FPU3		= 6'h22;		//FPU 3R Ops
parameter[5:0] JX2_UCMD_FLDCX		= 6'h23;		//FPU Convert (GR->FPR)
parameter[5:0] JX2_UCMD_FSTCX		= 6'h24;		//FPU Convert (FPR->GR)
parameter[5:0] JX2_UCMD_FIXS		= 6'h25;		//FPU Unary Op (FPR)
parameter[5:0] JX2_UCMD_FCMP		= 6'h26;		//FPU Compare
parameter[5:0] JX2_UCMD_MULW3		= 6'h27;		//Multiply (Word)

parameter[5:0] JX2_UCMD_ALUW3		= 6'h28;		//ALU, Packed Word (3R)
parameter[5:0] JX2_UCMD_BLINT		= 6'h29;		//Interpolator
parameter[5:0] JX2_UCMD_ALUB3		= 6'h2A;		//ALU, Packed Byte (3R)
parameter[5:0] JX2_UCMD_JCMP		= 6'h2B;		//Jump+Compare
parameter[5:0] JX2_UCMD_BRA_NB		= 6'h2C;		//No Branch
parameter[5:0] JX2_UCMD_ALUCMPW		= 6'h2D;		//ALU Cmp, Packed Word (3R)
parameter[5:0] JX2_UCMD_ALUCMPB		= 6'h2E;		//ALU Cmp, Packed Byte (3R)
parameter[5:0] JX2_UCMD_JCMPZ		= 6'h2F;		//Jump+Compare (Zero)


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
// parameter[5:0] JX2_UCIX_ALU_NOR		= 6'h2B;		//ALU NOR
// parameter[5:0] JX2_UCIX_ALU_SLT		= 6'h2B;		//ALU SLT
parameter[5:0] JX2_UCIX_ALU_CMPQEQ	= 6'h2C;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_CMPQHI	= 6'h2D;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_CMPQGT	= 6'h2E;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_CSELT	= 6'h2F;		//ALU CSELT

parameter[5:0] JX2_UCIX_ALU_ADDSL	= 6'h00;		//ALU ADDSL
parameter[5:0] JX2_UCIX_ALU_SUBSL	= 6'h01;		//ALU SUBSL

parameter[5:0] JX2_UCIX_ALU_ADDUL	= 6'h10;		//ALU ADDUL
parameter[5:0] JX2_UCIX_ALU_SUBUL	= 6'h11;		//ALU SUBUL

parameter[5:0] JX2_UCIX_ALU_TST		= 6'h04;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_CMPNE	= 6'h08;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_CMPHS	= 6'h09;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_CMPGE	= 6'h0A;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_CMPEQ	= 6'h0C;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_CMPHI	= 6'h0D;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_CMPGT	= 6'h0E;		//ALU Command

parameter[5:0] JX2_UCIX_ALU_TST_S		= 6'h14;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_CMPNE_S		= 6'h18;		//ALU CMPNE
parameter[5:0] JX2_UCIX_ALU_CMPHS_S		= 6'h19;		//ALU CMPHS
parameter[5:0] JX2_UCIX_ALU_CMPGE_S		= 6'h1A;		//ALU CMPGE
parameter[5:0] JX2_UCIX_ALU_CMPEQ_S		= 6'h1C;		//ALU CMPEQ
parameter[5:0] JX2_UCIX_ALU_CMPHI_S		= 6'h1D;		//ALU CMPHI
parameter[5:0] JX2_UCIX_ALU_CMPGT_S		= 6'h1E;		//ALU CMPGT

parameter[5:0] JX2_UCIX_ALU_SLTSL		= 6'h0B;		//ALU SLT
parameter[5:0] JX2_UCIX_ALU_SLTUL		= 6'h1B;		//ALU SLT
parameter[5:0] JX2_UCIX_ALU_SLTSQ		= 6'h2B;		//ALU SLT
parameter[5:0] JX2_UCIX_ALU_SLTUQ		= 6'h3B;		//ALU SLT

parameter[5:0] JX2_UCIX_ALU_TSTQ_S		= 6'h34;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_CMPQNE_S	= 6'h38;		//ALU CMPQNE
parameter[5:0] JX2_UCIX_ALU_CMPQHS_S	= 6'h39;		//ALU CMPQHS
parameter[5:0] JX2_UCIX_ALU_CMPQGE_S	= 6'h3A;		//ALU CMPQGE
parameter[5:0] JX2_UCIX_ALU_CMPQEQ_S	= 6'h3C;		//ALU CMPQEQ
parameter[5:0] JX2_UCIX_ALU_CMPQHI_S	= 6'h3D;		//ALU CMPQHI
parameter[5:0] JX2_UCIX_ALU_CMPQGT_S	= 6'h3E;		//ALU CMPQGT

parameter[5:0] JX2_UCIX_ALU_PADDL	= 6'h30;		//Packed ALU ADD
parameter[5:0] JX2_UCIX_ALU_PSUBL	= 6'h31;		//Packed ALU SUB
parameter[5:0] JX2_UCIX_ALU_PADCL	= 6'h32;		//Packed ALU ADC
parameter[5:0] JX2_UCIX_ALU_PSBBL	= 6'h33;		//Packed ALU SBB

parameter[5:0] JX2_UCIX_ALU_PTST	= 6'h34;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_PCMPNE	= 6'h38;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_PCMPHS	= 6'h39;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_PCMPGE	= 6'h3A;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_PCMPEQ	= 6'h3C;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_PCMPHI	= 6'h3D;		//ALU Command
parameter[5:0] JX2_UCIX_ALU_PCMPGT	= 6'h3E;		//ALU Command

parameter[5:0] JX2_UCIX_ALU_PCSELT	= 6'h3F;		//Packed CSELT

parameter[5:0] JX2_UCIX_ALUN_CLZ	= 6'h00;		//Count Leading Zeroes
parameter[5:0] JX2_UCIX_ALUN_CTZ	= 6'h01;		//Count Trailing Zeroes
parameter[5:0] JX2_UCIX_ALUN_BTRNS	= 6'h02;		//Transpose Bits
parameter[5:0] JX2_UCIX_ALUN_PMORTL	= 6'h03;		//Morton-Shuffle Bits

parameter[5:0] JX2_UCIX_ALUN_MINL		= 6'h04;	//MIN
parameter[5:0] JX2_UCIX_ALUN_MAXL		= 6'h05;	//MAX
parameter[5:0] JX2_UCIX_ALUN_FSGNJS		= 6'h06;	//FSGNJ (RV)
parameter[5:0] JX2_UCIX_ALUN_FSGNJXS	= 6'h07;	//FSGNJX (RV)

parameter[5:0] JX2_UCIX_ALUN_FMINS		= 6'h14;	//FMIN, Single
parameter[5:0] JX2_UCIX_ALUN_FMAXS		= 6'h15;	//FMAX
parameter[5:0] JX2_UCIX_ALUN_FSGNJNS	= 6'h16;	//FSGNJ (RV)
parameter[5:0] JX2_UCIX_ALUN_FSGNJNXS	= 6'h17;	//FSGNJX (RV)

parameter[5:0] JX2_UCIX_ALUN_CLZQ	= 6'h20;		//Count Leading Zeroes (QW)
parameter[5:0] JX2_UCIX_ALUN_CTZQ	= 6'h21;		//Count Trailing Zeroes (QW)
parameter[5:0] JX2_UCIX_ALUN_BTRNSQ	= 6'h22;		//Transpose Bits (QW)
parameter[5:0] JX2_UCIX_ALUN_PMORTQ	= 6'h23;		//Morton-Shuffle Bits (QW)

parameter[5:0] JX2_UCIX_ALUN_MINQ		= 6'h24;		//MIN
parameter[5:0] JX2_UCIX_ALUN_MAXQ		= 6'h25;		//MAX
parameter[5:0] JX2_UCIX_ALUN_FSGNJ		= 6'h26;		//FSGNJ (RV)
parameter[5:0] JX2_UCIX_ALUN_FSGNJX		= 6'h27;		//FSGNJX (RV)

parameter[5:0] JX2_UCIX_ALUN_FMIN		= 6'h34;	//FMIN, Double
parameter[5:0] JX2_UCIX_ALUN_FMAX		= 6'h35;	//FMAX
parameter[5:0] JX2_UCIX_ALUN_FSGNJN		= 6'h36;	//FSGNJ (RV)
parameter[5:0] JX2_UCIX_ALUN_FSGNJNX	= 6'h37;	//FSGNJX (RV)

parameter[5:0] JX2_UCIX_ALUW_PADDW	= 6'h20;		//ALU ADD
parameter[5:0] JX2_UCIX_ALUW_PSUBW	= 6'h21;		//ALU SUB

parameter[5:0] JX2_UCIX_ALUW_PADDP	= 6'h30;		//ALU ADD Pointer
parameter[5:0] JX2_UCIX_ALUW_PSUBP	= 6'h31;		//ALU SUB Pointer

parameter[5:0] JX2_UCIX_ALUW_MOVHLW	= 6'h12;		//ALU (High, Low)
parameter[5:0] JX2_UCIX_ALUW_MOVLHW	= 6'h13;		//ALU (Low, High)

parameter[5:0] JX2_UCIX_ALUW_MOVHLD	= 6'h22;		//ALU (High, Low)
parameter[5:0] JX2_UCIX_ALUW_MOVLHD	= 6'h23;		//ALU (Low, High)

parameter[5:0] JX2_UCIX_ALUW_PSHUFB	= 6'h15;		//ALU PSHUF.B
parameter[5:0] JX2_UCIX_ALUW_MOVLW	= 6'h16;		//ALU (Low Words)
parameter[5:0] JX2_UCIX_ALUW_MOVHW	= 6'h17;		//ALU (High Words)

parameter[5:0] JX2_UCIX_ALUW_PSHUFW	= 6'h25;		//ALU PSHUF.W
parameter[5:0] JX2_UCIX_ALUW_MOVLD	= 6'h26;		//ALU (Low DWords)
parameter[5:0] JX2_UCIX_ALUW_MOVHD	= 6'h27;		//ALU (High DWords)

parameter[5:0] JX2_UCIX_ALUW_CMPEQP	= 6'h0C;		//ALU Command
parameter[5:0] JX2_UCIX_ALUW_CMPGTP	= 6'h0E;		//ALU Command

parameter[5:0] JX2_UCIX_ALUW_PSRCHN		= 6'h18;		//Packed Search
parameter[5:0] JX2_UCIX_ALUW_CMPAHS		= 6'h19;		//
parameter[5:0] JX2_UCIX_ALUW_CMPAEQ		= 6'h1A;		//
parameter[5:0] JX2_UCIX_ALUW_MOVTA5U	= 6'h2A;		//
parameter[5:0] JX2_UCIX_ALUW_MOVTA16	= 6'h2B;		//
parameter[5:0] JX2_UCIX_ALUW_CMPNATEQ	= 6'h1B;		//

parameter[5:0] JX2_UCIX_ALUW_PSRCHE		= 6'h1C;		//Packed Search
parameter[5:0] JX2_UCIX_ALUW_CMPAHI		= 6'h1D;		//
parameter[5:0] JX2_UCIX_ALUW_CMPTEQ		= 6'h1E;		//
parameter[5:0] JX2_UCIX_ALUW_PCSELT		= 6'h2F;		//Packed CSELT

parameter[5:0] JX2_UCIX_ALUB_BITSEL	= 6'h2F;		//Bit Select

parameter[5:0] JX2_UCIX_ALUW_PTST	= 6'h34;		//ALU Command
parameter[5:0] JX2_UCIX_ALUW_PCMPNE	= 6'h38;		//ALU Command
parameter[5:0] JX2_UCIX_ALUW_PCMPHS	= 6'h39;		//ALU Command
parameter[5:0] JX2_UCIX_ALUW_PCMPGE	= 6'h3A;		//ALU Command
parameter[5:0] JX2_UCIX_ALUW_PCMPEQ	= 6'h3C;		//ALU Command
parameter[5:0] JX2_UCIX_ALUW_PCMPHI	= 6'h3D;		//ALU Command
parameter[5:0] JX2_UCIX_ALUW_PCMPGT	= 6'h3E;		//ALU Command

parameter[5:0] JX2_UCIX_PUSH_GR		= 6'h00;		//GPR
parameter[5:0] JX2_UCIX_PUSH_CR		= 6'h01;		//Control Reg
parameter[5:0] JX2_UCIX_PUSH_FR		= 6'h02;		//FPR
parameter[5:0] JX2_UCIX_PUSH_SR		= 6'h03;		//Shadow Reg
parameter[5:0] JX2_UCIX_PUSH_GRX2	= 6'h04;		//GPR Pair

parameter[5:0] JX2_UCIX_CONV_EXTSB	= 6'h00;		//
parameter[5:0] JX2_UCIX_CONV_EXTSW	= 6'h01;		//
parameter[5:0] JX2_UCIX_CONV_EXTSL	= 6'h02;		//
parameter[5:0] JX2_UCIX_CONV_MOV	= 6'h03;		//MOV
parameter[5:0] JX2_UCIX_CONV_EXTUB	= 6'h04;		//
parameter[5:0] JX2_UCIX_CONV_EXTUW	= 6'h05;		//
parameter[5:0] JX2_UCIX_CONV_EXTUL	= 6'h06;		//
parameter[5:0] JX2_UCIX_CONV_NOT	= 6'h07;		//NOT

// parameter[5:0] JX2_UCIX_CONV_CLZ	= 6'h08;		//Count Leading Zeroes (L)
// parameter[5:0] JX2_UCIX_CONV_CLZQ	= 6'h09;		//Count Leading Zeroes (Q)

parameter[5:0] JX2_UCIX_CONV_STFXI		= 6'h08;		//Set Fixint
parameter[5:0] JX2_UCIX_CONV_STFLI		= 6'h09;		//Set Flonum

parameter[5:0] JX2_UCIX_CONV_LDIHI		= 6'h0A;		//Load High Bits
parameter[5:0] JX2_UCIX_CONV_LDIQHI		= 6'h0B;		//Load High Bits
parameter[5:0] JX2_UCIX_CONV_LDISIZ		= 6'h0C;		//Load Shift Imm
parameter[5:0] JX2_UCIX_CONV_LDIQSIZ	= 6'h0D;		//Load Shift Imm
parameter[5:0] JX2_UCIX_CONV_LDISIN		= 6'h0E;		//Load Shift Imm
parameter[5:0] JX2_UCIX_CONV_LDIQSIN	= 6'h0F;		//Load Shift Imm
parameter[5:0] JX2_UCIX_CONV_LDIQHI16	= 6'h10;		//Load High Bits
parameter[5:0] JX2_UCIX_CONV_LDIQHI32	= 6'h11;		//Load High Bits
parameter[5:0] JX2_UCIX_CONV_FNEG		= 6'h12;		//Float FNEG
parameter[5:0] JX2_UCIX_CONV_FABS		= 6'h13;		//Float FABS
parameter[5:0] JX2_UCIX_CONV_SNIPEIC	= 6'h14;		//
parameter[5:0] JX2_UCIX_CONV_SNIPEDC	= 6'h15;		//

parameter[5:0] JX2_UCIX_CONV_MOVZT		= 6'h16;		//
parameter[5:0] JX2_UCIX_CONV_MOVST		= 6'h17;		//

parameter[5:0] JX2_UCIX_CONV_RGBSHR1		= 6'h18;	//RGB555, Shift Right 1
parameter[5:0] JX2_UCIX_CONV_RGB5PCK32		= 6'h1A;	//RGB32->RGB555
parameter[5:0] JX2_UCIX_CONV_RGB5UPCK32		= 6'h1B;	//RGB555->RGB32
parameter[5:0] JX2_UCIX_CONV_RGB5PCK64		= 6'h1C;	//RGB64->RGB555
parameter[5:0] JX2_UCIX_CONV_RGB5UPCK64		= 6'h1D;	//RGB555->RGB64
parameter[5:0] JX2_UCIX_CONV_RGB32PCK64		= 6'h1E;	//RGB64->RGB32
parameter[5:0] JX2_UCIX_CONV_RGB32UPCK64	= 6'h1F;	//RGB32->RGB64

parameter[5:0] JX2_UCIX_CONV_PAHALW	= 6'h20;	//
parameter[5:0] JX2_UCIX_CONV_PAHLLW	= 6'h21;	//
parameter[5:0] JX2_UCIX_CONV_PAHARW	= 6'h22;	//
parameter[5:0] JX2_UCIX_CONV_PAHLRW	= 6'h23;	//

//parameter[5:0] JX2_UCIX_CONV_RGB32PCK64FU	= 6'h20;	//RGB64F->RGB32F (U)
//parameter[5:0] JX2_UCIX_CONV_RGB32PCK64FS	= 6'h21;	//RGB64F->RGB32F (S)
//parameter[5:0] JX2_UCIX_CONV_RGB32UPCK64FU	= 6'h22;	//RGB32F->RGB64F (U)
//parameter[5:0] JX2_UCIX_CONV_RGB32UPCK64FS	= 6'h23;	//RGB32F->RGB64F (S)

//parameter[5:0] JX2_UCIX_CONV_RGB30APCK64F	= 6'h24;	//RGB64F->RGB30A
//parameter[5:0] JX2_UCIX_CONV_RGB30AUPCK64F	= 6'h25;	//RGB30A->RGB64F
//parameter[5:0] JX2_UCIX_CONV_FP16PCK32		= 6'h26;	//2x FP32->FP16

parameter[5:0] JX2_UCIX_CONV_BSWAPL			= 6'h26;	//RGB5 Min and Max
parameter[5:0] JX2_UCIX_CONV_BSWAPQ			= 6'h27;	//RGB5 Min and Max

// parameter[5:0] JX2_UCIX_CONV_RGB5MINMAX		= 6'h27;	//RGB5 Min and Max

// parameter[5:0] JX2_UCIX_CONV_FP16UPCK32L	= 6'h26;	//2x FP16->FP32 (Lo)
// parameter[5:0] JX2_UCIX_CONV_FP16UPCK32H	= 6'h27;	//2x FP16->FP32 (Hi)
// parameter[5:0] JX2_UCIX_CONV_FP16PCK32		= 6'h28;	//2x FP32->FP16

//parameter[5:0] JX2_UCIX_CONV_FP16UPCK32L	= 6'h28;	//2x FP16->FP32 (Lo)
//parameter[5:0] JX2_UCIX_CONV_FP16UPCK32H	= 6'h29;	//2x FP16->FP32 (Hi)
//parameter[5:0] JX2_UCIX_CONV_FP16EUPCK32L	= 6'h2A;	//2x FP16E->FP32 (Lo)
//parameter[5:0] JX2_UCIX_CONV_FP16EUPCK32H	= 6'h2B;	//2x FP16E->FP32 (Hi)

//parameter[5:0] JX2_UCIX_CONV_FP16PCKAL		= 6'h2C;	//4x FP16 -> A-Law
//parameter[5:0] JX2_UCIX_CONV_FP16UPCKAL		= 6'h2D;	//4x A-Law -> FP16

parameter[5:0] JX2_UCIX_CONV_SWAPE2B		= 6'h2E;	//Swap Every 2 Bits
parameter[5:0] JX2_UCIX_CONV_SWAPE4B		= 6'h2F;	//Swap Every 4 Bits


//parameter[5:0] JX2_UCIX_CONV2_EXTSB		= 6'h00;		//
//parameter[5:0] JX2_UCIX_CONV2_EXTSW		= 6'h01;		//
//parameter[5:0] JX2_UCIX_CONV2_EXTSL		= 6'h02;		//
//parameter[5:0] JX2_UCIX_CONV2_MOV		= 6'h03;		//MOV
//parameter[5:0] JX2_UCIX_CONV2_EXTUB		= 6'h04;		//
//parameter[5:0] JX2_UCIX_CONV2_EXTUW		= 6'h05;		//
//parameter[5:0] JX2_UCIX_CONV2_EXTUL		= 6'h06;		//
//parameter[5:0] JX2_UCIX_CONV2_NOT		= 6'h07;		//NOT

parameter[5:0] JX2_UCIX_CONV2_BLKUTX1		= 6'h08;	//Get pixel, UTX1
parameter[5:0] JX2_UCIX_CONV2_BLKUTX2		= 6'h09;	//Get pixel, UTX2
parameter[5:0] JX2_UCIX_CONV2_BLKUTX3H		= 6'h0A;	//Get pixel, UTX3H
parameter[5:0] JX2_UCIX_CONV2_BLKUTX3L		= 6'h0B;	//Get pixel, UTX3L
parameter[5:0] JX2_UCIX_CONV2_BLKRGB15F		= 6'h0C;	//Get pixel
parameter[5:0] JX2_UCIX_CONV2_BLKRGB15A		= 6'h0D;	//Get pixel, RGB555A
parameter[5:0] JX2_UCIX_CONV2_BLKRGBA32		= 6'h0E;	//Get pixel, RGBA32
parameter[5:0] JX2_UCIX_CONV2_BLKRGB30A		= 6'h0F;	//Get pixel, RGB30A

parameter[5:0] JX2_UCIX_CONV2_BCDADD		= 6'h10;	//
parameter[5:0] JX2_UCIX_CONV2_BCDSUB		= 6'h11;	//
parameter[5:0] JX2_UCIX_CONV2_DAAQ			= 6'h12;	//
parameter[5:0] JX2_UCIX_CONV2_DASQ			= 6'h13;	//
parameter[5:0] JX2_UCIX_CONV2_ROTCL			= 6'h14;	//
parameter[5:0] JX2_UCIX_CONV2_ROTCR			= 6'h15;	//
parameter[5:0] JX2_UCIX_CONV2_ROTCLQ		= 6'h16;	//
parameter[5:0] JX2_UCIX_CONV2_ROTCRQ		= 6'h17;	//

parameter[5:0] JX2_UCIX_CONV2_PSQRTSAH		= 6'h18;	//
parameter[5:0] JX2_UCIX_CONV2_PSQRTUAH		= 6'h19;	//
parameter[5:0] JX2_UCIX_CONV2_PSQRTSAF		= 6'h1A;	//
parameter[5:0] JX2_UCIX_CONV2_PSQRTUAF		= 6'h1B;	//
parameter[5:0] JX2_UCIX_CONV2_PRCPAH		= 6'h1C;	//
parameter[5:0] JX2_UCIX_CONV2_PRELUH		= 6'h1D;	//
parameter[5:0] JX2_UCIX_CONV2_PRCPAF		= 6'h1E;	//
parameter[5:0] JX2_UCIX_CONV2_PRELUF		= 6'h1F;	//

parameter[5:0] JX2_UCIX_CONV2_RGB32PCK64FU	= 6'h20;	//RGB64F->RGB32F (U)
parameter[5:0] JX2_UCIX_CONV2_RGB32PCK64FS	= 6'h21;	//RGB64F->RGB32F (S)
parameter[5:0] JX2_UCIX_CONV2_RGB32UPCK64FU	= 6'h22;	//RGB32F->RGB64F (U)
parameter[5:0] JX2_UCIX_CONV2_RGB32UPCK64FS	= 6'h23;	//RGB32F->RGB64F (S)
parameter[5:0] JX2_UCIX_CONV2_RGB30APCK64F	= 6'h24;	//RGB64F->RGB30A
parameter[5:0] JX2_UCIX_CONV2_RGB30AUPCK64F	= 6'h25;	//RGB30A->RGB64F
parameter[5:0] JX2_UCIX_CONV2_FP16PCK32		= 6'h26;	//2x FP32->FP16
parameter[5:0] JX2_UCIX_CONV2_RGB5MINMAX	= 6'h27;	//RGB5 Min and Max

parameter[5:0] JX2_UCIX_CONV2_FP16UPCK32L	= 6'h28;	//2x FP16->FP32 (Lo)
parameter[5:0] JX2_UCIX_CONV2_FP16UPCK32H	= 6'h29;	//2x FP16->FP32 (Hi)
parameter[5:0] JX2_UCIX_CONV2_FP16EUPCK32L	= 6'h2A;	//2x FP16E->FP32 (Lo)
parameter[5:0] JX2_UCIX_CONV2_FP16EUPCK32H	= 6'h2B;	//2x FP16E->FP32 (Hi)
parameter[5:0] JX2_UCIX_CONV2_FP16PCKAL		= 6'h2C;	//4x FP16 -> A-Law
parameter[5:0] JX2_UCIX_CONV2_FP16UPCKAL	= 6'h2D;	//4x A-Law -> FP16
parameter[5:0] JX2_UCIX_CONV2_FP16UPCKFP8	= 6'h2E;	//4x FP8 -> FP16
parameter[5:0] JX2_UCIX_CONV2_FP16PCKFP8	= 6'h2F;	//4x FP16 -> FP8

parameter[5:0] JX2_UCIX_CONV2_VUBTOF16L		= 6'h30;	//
parameter[5:0] JX2_UCIX_CONV2_VUBTOF16H		= 6'h31;	//
parameter[5:0] JX2_UCIX_CONV2_VSBTOF16L		= 6'h32;	//
parameter[5:0] JX2_UCIX_CONV2_VSBTOF16H		= 6'h33;	//
parameter[5:0] JX2_UCIX_CONV2_VUWTOF32L		= 6'h34;	//
parameter[5:0] JX2_UCIX_CONV2_VUWTOF32H		= 6'h35;	//
parameter[5:0] JX2_UCIX_CONV2_VSWTOF32L		= 6'h36;	//
parameter[5:0] JX2_UCIX_CONV2_VSWTOF32H		= 6'h37;	//

parameter[5:0] JX2_UCIX_CONV2_VF16TOUB		= 6'h38;	//
parameter[5:0] JX2_UCIX_CONV2_VUWTOF16		= 6'h39;	//
parameter[5:0] JX2_UCIX_CONV2_VF16TOSB		= 6'h3A;	//
parameter[5:0] JX2_UCIX_CONV2_VSWTOF16		= 6'h3B;	//
parameter[5:0] JX2_UCIX_CONV2_VF32TOUW		= 6'h3C;	//
parameter[5:0] JX2_UCIX_CONV2_VF16TOUW		= 6'h3D;	//
parameter[5:0] JX2_UCIX_CONV2_VF32TOSW		= 6'h3E;	//
parameter[5:0] JX2_UCIX_CONV2_VF16TOSW		= 6'h3F;	//

parameter[5:0] JX2_UCIX_CONV3_FMULFP8		= 6'h00;	//

parameter[5:0] JX2_UCIX_MUL3_MUL3S	= 6'h00;		//
parameter[5:0] JX2_UCIX_MUL3_MUL3U	= 6'h01;		//
parameter[5:0] JX2_UCIX_MUL3_MULS	= 6'h02;		//
parameter[5:0] JX2_UCIX_MUL3_MULU	= 6'h03;		//

parameter[5:0] JX2_UCIX_MUL3_DMUL3S	= 6'h04;		//
parameter[5:0] JX2_UCIX_MUL3_DMUL3U	= 6'h05;		//

parameter[5:0] JX2_UCIX_MUL3_PWMULS	= 6'h06;		//Packed-Widen
parameter[5:0] JX2_UCIX_MUL3_PWMULU	= 6'h07;		//Packed-Widen
parameter[5:0] JX2_UCIX_MUL3_PLMULS	= 6'h08;		//Packed-Low
parameter[5:0] JX2_UCIX_MUL3_PLMULU	= 6'h09;		//Packed-Low
parameter[5:0] JX2_UCIX_MUL3_PHMULS	= 6'h0A;		//Packed-High
parameter[5:0] JX2_UCIX_MUL3_PHMULU	= 6'h0B;		//Packed-High

parameter[5:0] JX2_UCIX_MUL3_PHMULSA	= 6'h0C;		//Packed-Mul Add
parameter[5:0] JX2_UCIX_MUL3_PHMULUA	= 6'h0D;		//Packed-Mul Add


parameter[5:0] JX2_UCIX_MUL3_MAC3S	= 6'h10;		//
parameter[5:0] JX2_UCIX_MUL3_MAC3U	= 6'h11;		//
parameter[5:0] JX2_UCIX_MUL3_MACS	= 6'h12;		//
parameter[5:0] JX2_UCIX_MUL3_MACU	= 6'h13;		//

parameter[5:0] JX2_UCIX_MUL3_DMAC3S	= 6'h14;		//
parameter[5:0] JX2_UCIX_MUL3_DMAC3U	= 6'h15;		//

parameter[5:0] JX2_UCIX_MUL3_RGB5PCKI8	= 6'h28;	//Pack Palette
parameter[5:0] JX2_UCIX_MUL3_RGB5UPCKI8	= 6'h29;	//Pack Palette

parameter[5:0] JX2_UCIX_MUL3_ENCCC1		= 6'h2A;	//Encode Color-Cell
parameter[5:0] JX2_UCIX_MUL3_ENCCC2		= 6'h2B;	//Encode Color-Cell
parameter[5:0] JX2_UCIX_MUL3_BLKUAB1	= 6'h2C;	//Get Sample, UAB1
parameter[5:0] JX2_UCIX_MUL3_BLKUAB2	= 6'h2D;	//Get Sample, UAB2
parameter[5:0] JX2_UCIX_MUL3_RGB5MINMAX	= 6'h2F;	//Encode Color-Cell

parameter[5:0] JX2_UCIX_MUL3_BITNN		= 6'h30;	//Bit Neural Net


parameter[5:0] JX2_UCIX_QMUL_MULS	= 6'h00;		//
parameter[5:0] JX2_UCIX_QMUL_MULU	= 6'h01;		//
parameter[5:0] JX2_UCIX_QMUL_MULHS	= 6'h02;		//
parameter[5:0] JX2_UCIX_QMUL_MULHU	= 6'h03;		//

parameter[5:0] JX2_UCIX_QMUL_DIVS	= 6'h04;		//
parameter[5:0] JX2_UCIX_QMUL_DIVU	= 6'h05;		//
parameter[5:0] JX2_UCIX_QMUL_MODS	= 6'h06;		//
parameter[5:0] JX2_UCIX_QMUL_MODU	= 6'h07;		//

parameter[5:0] JX2_UCIX_QMUL_MULSL	= 6'h08;		//
parameter[5:0] JX2_UCIX_QMUL_MULUL	= 6'h09;		//
parameter[5:0] JX2_UCIX_QMUL_MULHSL	= 6'h0A;		//
parameter[5:0] JX2_UCIX_QMUL_MULHUL	= 6'h0B;		//

parameter[5:0] JX2_UCIX_QMUL_DIVSL	= 6'h0C;		//
parameter[5:0] JX2_UCIX_QMUL_DIVUL	= 6'h0D;		//
parameter[5:0] JX2_UCIX_QMUL_MODSL	= 6'h0E;		//
parameter[5:0] JX2_UCIX_QMUL_MODUL	= 6'h0F;		//

parameter[5:0] JX2_UCIX_QMUL_MULHSU	= 6'h12;		//

parameter[5:0] JX2_UCIX_QMUL_FDIV	= 6'h25;		//
parameter[5:0] JX2_UCIX_QMUL_FDIVS	= 6'h35;		//


parameter[5:0] JX2_UCIX_LDI_LDIX	= 6'h00;		//
parameter[5:0] JX2_UCIX_LDI_LDISH8	= 6'h01;		//
parameter[5:0] JX2_UCIX_LDI_LDISH16	= 6'h02;		//
parameter[5:0] JX2_UCIX_LDI_LDISH32	= 6'h03;		//
parameter[5:0] JX2_UCIX_LDI_JLDIX	= 6'h04;		//
parameter[5:0] JX2_UCIX_LDI_FLDCH	= 6'h05;		//
parameter[5:0] JX2_UCIX_LDI_LDIHI32	= 6'h06;		//

parameter[5:0] JX2_UCIX_SHAD_SHAD3	= 6'h00;		//
parameter[5:0] JX2_UCIX_SHAD_SHLD3	= 6'h01;		//
parameter[5:0] JX2_UCIX_SHAD_SHADQ3	= 6'h02;		//
parameter[5:0] JX2_UCIX_SHAD_SHLDQ3	= 6'h03;		//
parameter[5:0] JX2_UCIX_SHAD_SHAR3	= 6'h04;		//
parameter[5:0] JX2_UCIX_SHAD_SHLR3	= 6'h05;		//
parameter[5:0] JX2_UCIX_SHAD_SHARQ3	= 6'h06;		//
parameter[5:0] JX2_UCIX_SHAD_SHLRQ3	= 6'h07;		//

parameter[5:0] JX2_UCIX_SHAD_ROTL3	= 6'h08;		//
parameter[5:0] JX2_UCIX_SHAD_ROTLQ3	= 6'h0A;		//
parameter[5:0] JX2_UCIX_SHAD_ROTR3	= 6'h0C;		//
parameter[5:0] JX2_UCIX_SHAD_ROTRQ3	= 6'h0E;		//

parameter[5:0] JX2_UCIX_SHAD_SHADX3	= 6'h20;		//
parameter[5:0] JX2_UCIX_SHAD_SHLDX3	= 6'h21;		//
parameter[5:0] JX2_UCIX_SHAD_SHARX3	= 6'h24;		//
parameter[5:0] JX2_UCIX_SHAD_SHLRX3	= 6'h25;		//
parameter[5:0] JX2_UCIX_SHAD_ROTLX3	= 6'h28;		//
parameter[5:0] JX2_UCIX_SHAD_ROTRX3	= 6'h2C;		//


parameter[5:0] JX2_UCIX_FPU_FADD	= 6'h00;		//FPU ADD
parameter[5:0] JX2_UCIX_FPU_FSUB	= 6'h01;		//FPU SUB
parameter[5:0] JX2_UCIX_FPU_FMUL	= 6'h02;		//FPU MUL
parameter[5:0] JX2_UCIX_FPU_FDIV	= 6'h03;		//FPU DIV
// parameter[5:0] JX2_UCIX_FPU_FMOV	= 6'h04;		//FPU MOV

parameter[5:0] JX2_UCIX_FPU_FADDX	= 6'h20;		//FPU ADD (Long Double)
parameter[5:0] JX2_UCIX_FPU_FSUBX	= 6'h21;		//FPU SUB (Long Double)
parameter[5:0] JX2_UCIX_FPU_FMULX	= 6'h22;		//FPU MUL (Long Double)
parameter[5:0] JX2_UCIX_FPU_FDIVX	= 6'h23;		//FPU DIV (Long Double)

parameter[5:0] JX2_UCIX_FPU_FDIVA	= 6'h13;		//FPU DIV (Approx)
parameter[5:0] JX2_UCIX_FPU_FDIVXA	= 6'h33;		//FPU DIV (LngDbl, Approx)

parameter[5:0] JX2_UCIX_FPU_FMRA	= 6'h08;		//FPU FMAC (-(a*b+c))
parameter[5:0] JX2_UCIX_FPU_FMAC	= 6'h09;		//FPU FMAS (a*b+c)
parameter[5:0] JX2_UCIX_FPU_FMRS	= 6'h0A;		//FPU FMAC (c-a*b)
parameter[5:0] JX2_UCIX_FPU_FMAS	= 6'h0B;		//FPU FMAC (a*b-c)

parameter[5:0] JX2_UCIX_FPU_PMRA	= 6'h18;		//FPU FMAC (-(a*b+c))
parameter[5:0] JX2_UCIX_FPU_PMAC	= 6'h19;		//FPU FMAS (a*b+c)
parameter[5:0] JX2_UCIX_FPU_PMRS	= 6'h1A;		//FPU FMAC (c-a*b)
parameter[5:0] JX2_UCIX_FPU_PMAS	= 6'h1B;		//FPU FMAC (a*b-c)

//parameter[5:0] JX2_UCIX_FPU_FMRS	= 6'h0D;		//FPU FMAC (c-a*b)
// parameter[5:0] JX2_UCIX_FPU_FMAS	= 6'h0F;		//FPU FMAC (a*b-c)

parameter[5:0] JX2_UCIX_FPU_PADD	= 6'h05;		//FPU ADD (2x F32)
parameter[5:0] JX2_UCIX_FPU_PSUB	= 6'h06;		//FPU SUB (2x F32)
parameter[5:0] JX2_UCIX_FPU_PMUL	= 6'h07;		//FPU MUL (2x F32)

parameter[5:0] JX2_UCIX_FPU_PADDX	= 6'h25;		//FPU ADD (4x F32)
parameter[5:0] JX2_UCIX_FPU_PSUBX	= 6'h26;		//FPU SUB (4x F32)
parameter[5:0] JX2_UCIX_FPU_PMULX	= 6'h27;		//FPU MUL (4x F32)

parameter[5:0] JX2_UCIX_FPU_FMACX	= 6'h29;		//FPU FMAC (a*b+c)

parameter[5:0] JX2_UCIX_FPU_FADD_G	= 6'h10;		//FPU ADD (GFP)
parameter[5:0] JX2_UCIX_FPU_FSUB_G	= 6'h11;		//FPU SUB (GFP)
parameter[5:0] JX2_UCIX_FPU_FMUL_G	= 6'h12;		//FPU MUL (GFP)

parameter[5:0] JX2_UCIX_FPU_PADDH	= 6'h15;		//FPU ADD (4x F16)
parameter[5:0] JX2_UCIX_FPU_PSUBH	= 6'h16;		//FPU SUB (4x F16)
parameter[5:0] JX2_UCIX_FPU_PMULH	= 6'h17;		//FPU MUL (4x F16)

parameter[5:0] JX2_UCIX_FPU_PADDXD	= 6'h35;		//FPU ADD (2x F64)
parameter[5:0] JX2_UCIX_FPU_PSUBXD	= 6'h36;		//FPU SUB (2x F64)
parameter[5:0] JX2_UCIX_FPU_PMULXD	= 6'h37;		//FPU MUL (2x F64)

parameter[5:0] JX2_UCIX_FCMP_CMPGE_R	= 6'h05;		//ALU Command
parameter[5:0] JX2_UCIX_FCMP_CMPEQ_R	= 6'h06;		//ALU Command
parameter[5:0] JX2_UCIX_FCMP_CMPGT_R	= 6'h07;		//ALU Command

parameter[5:0] JX2_UCIX_FCMP_PCMPGE_R	= 6'h35;		//ALU Command
parameter[5:0] JX2_UCIX_FCMP_PCMPEQ_R	= 6'h36;		//ALU Command
parameter[5:0] JX2_UCIX_FCMP_PCMPGT_R	= 6'h37;		//ALU Command

parameter[5:0] JX2_UCIX_FPU_CMPNE	= 6'h08;		//ALU Command
parameter[5:0] JX2_UCIX_FPU_CMPGE	= 6'h0A;		//ALU Command
parameter[5:0] JX2_UCIX_FPU_CMPEQ	= 6'h0C;		//ALU Command
parameter[5:0] JX2_UCIX_FPU_CMPGT	= 6'h0E;		//ALU Command

parameter[5:0] JX2_UCIX_FPU_FSQRT	= 6'h0F;		//FPU Command
parameter[5:0] JX2_UCIX_FPU_FSQRTX	= 6'h2F;		//FPU Command
parameter[5:0] JX2_UCIX_FPU_FSQRTA	= 6'h1F;		//FPU Command
parameter[5:0] JX2_UCIX_FPU_FSQRTXA	= 6'h3F;		//FPU Command


// parameter[5:0] JX2_UCIX_FPU_CMPNE_G	= 6'h18;		//ALU Command
// parameter[5:0] JX2_UCIX_FPU_CMPGE_G	= 6'h1A;		//ALU Command
// parameter[5:0] JX2_UCIX_FPU_CMPEQ_G	= 6'h1C;		//ALU Command
// parameter[5:0] JX2_UCIX_FPU_CMPGT_G	= 6'h1E;		//ALU Command

parameter[5:0] JX2_UCIX_FPU_CMPXNE	= 6'h28;		//ALU Command
parameter[5:0] JX2_UCIX_FPU_CMPXGE	= 6'h2A;		//ALU Command
parameter[5:0] JX2_UCIX_FPU_CMPXEQ	= 6'h2C;		//ALU Command
parameter[5:0] JX2_UCIX_FPU_CMPXGT	= 6'h2E;		//ALU Command

parameter[5:0] JX2_UCIX_FPU_PCMPNE	= 6'h38;		//ALU Command
parameter[5:0] JX2_UCIX_FPU_PCMPGE	= 6'h3A;		//ALU Command
parameter[5:0] JX2_UCIX_FPU_PCMPEQ	= 6'h3C;		//ALU Command
parameter[5:0] JX2_UCIX_FPU_PCMPGT	= 6'h3E;		//ALU Command

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
parameter[5:0] JX2_UCIX_FPCX_I		= 6'h02;		//FPU Int64
parameter[5:0] JX2_UCIX_FPCX_H		= 6'h03;		//FPU Half
parameter[5:0] JX2_UCIX_FPCX_ISL	= 6'h04;		//FPU Int32
parameter[5:0] JX2_UCIX_FPCX_IUL	= 6'h05;		//FPU UInt32
// parameter[5:0] JX2_UCIX_FPCX_SI		= 6'h06;		//FPU Int64 (Single)
parameter[5:0] JX2_UCIX_FPCX_IU		= 6'h07;		//FPU UInt64

parameter[5:0] JX2_UCIX_FPCX_S2		= 6'h08;		//FPU Single (High)

parameter[5:0] JX2_UCIX_FPCX_SG		= 6'h10;		//FPU Single
parameter[5:0] JX2_UCIX_FPCX_DG		= 6'h11;		//FPU Double
parameter[5:0] JX2_UCIX_FPCX_IG		= 6'h12;		//FPU Int
parameter[5:0] JX2_UCIX_FPCX_HG		= 6'h13;		//FPU Half

parameter[5:0] JX2_UCIX_FPCX_SISL	= 6'h14;		//FPU Int32
parameter[5:0] JX2_UCIX_FPCX_SIUL	= 6'h15;		//FPU UInt32

parameter[5:0] JX2_UCIX_FPCX_SIG	= 6'h16;		//FPU Int64 (Single)
parameter[5:0] JX2_UCIX_FPCX_SIU	= 6'h17;		//FPU UInt64

parameter[5:0] JX2_UCIX_FPCX_S2G	= 6'h18;		//FPU Single (High)

parameter[5:0] JX2_UCIX_FPCX_XE		= 6'h21;		//FPU LongDouble
parameter[5:0] JX2_UCIX_FPCX_XI		= 6'h22;		//FPU Int (Binary128)

parameter[5:0] JX2_UCIX_FPCX_XIU	= 6'h27;		//FPU Int (Binary128)

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
parameter[5:0] JX2_UCIX_IXT_SYSE	= 6'h0E;		//System Call
parameter[5:0] JX2_UCIX_IXT_LDTLB	= 6'h0F;		//Load TLB
parameter[5:0] JX2_UCIX_IXT_INVTLB	= 6'h10;		//Flush TLB

parameter[5:0] JX2_UCIX_IXT_LDEKRR	= 6'h11;		//Load Encoded KRR
parameter[5:0] JX2_UCIX_IXT_LDEKEY	= 6'h12;		//Load Encoding Key
parameter[5:0] JX2_UCIX_IXT_LDEENC	= 6'h13;		//Encode Keyring into Key

parameter[5:0] JX2_UCIX_IXT_SRTTWID	= 6'h14;		//Twiddle SR.T Bits
parameter[5:0] JX2_UCIX_IXT_LDACL	= 6'h15;		//Load ACL
parameter[5:0] JX2_UCIX_IXT_SVEKRR	= 6'h16;		//Save KRR into Encoded Key
parameter[5:0] JX2_UCIX_IXT_SVENTR	= 6'h17;		//Enter Supervisor Mode
parameter[5:0] JX2_UCIX_IXT_SXENTR	= 6'h18;		//Enter Superuser Mode
parameter[5:0] JX2_UCIX_IXT_SUENTR	= 6'h19;		//Enter User Mode

parameter[5:0] JX2_UCIX_IXT_VSKG	= 6'h1A;		//Canary Gen
parameter[5:0] JX2_UCIX_IXT_VSKC	= 6'h1B;		//Canary Check

parameter[5:0] JX2_UCIX_IXT_BNDCMPB	= 6'h1C;		//Bound Compare
parameter[5:0] JX2_UCIX_IXT_BNDCMPW	= 6'h1D;		//Bound Compare
parameter[5:0] JX2_UCIX_IXT_BNDCMPL	= 6'h1E;		//Bound Compare
parameter[5:0] JX2_UCIX_IXT_BNDCMPQ	= 6'h1F;		//Bound Compare

parameter[5:0] JX2_UCIX_IXT_BNDCHKB	= 6'h20;		//Bound Check
parameter[5:0] JX2_UCIX_IXT_BNDCHKW	= 6'h21;		//Bound Check
parameter[5:0] JX2_UCIX_IXT_BNDCHKL	= 6'h22;		//Bound Check
parameter[5:0] JX2_UCIX_IXT_BNDCHKQ	= 6'h23;		//Bound Check


parameter[5:0] JX2_UCIX_IXS_NOP		= 6'h00;		//No-Op
parameter[5:0] JX2_UCIX_IXS_MOVT	= 6'h01;		//Copy SR.T to Reg
parameter[5:0] JX2_UCIX_IXS_MOVNT	= 6'h02;		//Copy !SR.T to Reg
parameter[5:0] JX2_UCIX_IXS_LDSRMSK	= 6'h03;		//?
parameter[5:0] JX2_UCIX_IXS_TRAPB	= 6'h04;		//Trap
// parameter[5:0] JX2_UCIX_IXS_MOVTA16	= 6'h05;		//Trap

parameter[5:0] JX2_UCIX_IXS_MOVST	= 6'h06;		//Copy SR.ST to Reg
parameter[5:0] JX2_UCIX_IXS_MOVPQ	= 6'h07;		//Copy SR.PQRO to Reg

parameter[5:0] JX2_UCIX_IXS_BLKUTX1		= 6'h08;		//Get pixel, UTX1
parameter[5:0] JX2_UCIX_IXS_BLKUTX2		= 6'h09;		//Get pixel, UTX2
parameter[5:0] JX2_UCIX_IXS_BLKUTX3H	= 6'h0A;		//Get pixel, UTX3H
parameter[5:0] JX2_UCIX_IXS_BLKUTX3L	= 6'h0B;		//Get pixel, UTX3L

parameter[5:0] JX2_UCIX_IXS_INVIC	= 6'h10;		//Flush I$
parameter[5:0] JX2_UCIX_IXS_INVDC	= 6'h11;		//Flush D$

parameter[5:0] JX2_UCIX_BLINT_BILERP	= 6'h00;
parameter[5:0] JX2_UCIX_BLINT_LERP		= 6'h01;


parameter[5:0] JX2_UCIX_JCMP_QEQ		= 6'h10;
parameter[5:0] JX2_UCIX_JCMP_QNE		= 6'h11;
parameter[5:0] JX2_UCIX_JCMP_QLE		= 6'h12;
parameter[5:0] JX2_UCIX_JCMP_QGT		= 6'h13;
parameter[5:0] JX2_UCIX_JCMP_QLT		= 6'h14;
parameter[5:0] JX2_UCIX_JCMP_QGE		= 6'h15;
parameter[5:0] JX2_UCIX_JCMP_QBL		= 6'h16;
parameter[5:0] JX2_UCIX_JCMP_QHS		= 6'h17;
parameter[5:0] JX2_UCIX_JCMP_TSTT		= 6'h18;
parameter[5:0] JX2_UCIX_JCMP_TSTF		= 6'h19;

parameter[5:0] JX2_UCIX_INVOP_PRIVFAULT	= 6'h10;		//Flush TLB
parameter[5:0] JX2_UCIX_INVOP_FPUFAULT	= 6'h11;		//Flush TLB


`ifdef jx2_mem_line32B

parameter[255:0] UVTILE_XX		= UV256_XX;	//
parameter[255:0] UVTILE_00		= UV256_00;	//
parameter[255:0] UVTILE_FF		= UV256_FF;	//
`define reg_tile			reg[255:0]
`define wire_tile			wire[255:0]
`define input_tile			input[255:0]
`define output_tile			output[255:0]
`define reg_tile_pad		reg[269:0]		//pad to multiple of 18 bits
`define reg_tile_pflag		reg[5:0]
`define reg_tile_prov		reg[7:0]

`else

parameter[127:0] UVTILE_XX		= UV128_XX;	//
parameter[127:0] UVTILE_00		= UV128_00;	//
parameter[127:0] UVTILE_FF		= UV128_FF;	//
`define reg_tile			reg[127:0]
`define wire_tile			wire[127:0]
`define input_tile			input[127:0]
`define output_tile			output[127:0]
`define reg_tile_pad		reg[143:0]		//pad to multiple of 18 bits
`define reg_tile_pflag		reg[7:0]
`define reg_tile_prov		reg[7:0]

`endif

`ifdef jx2_mem_useddrwa

`define reg_ddrtile			reg[511:0]
`define wire_ddrtile		wire[511:0]
`define input_ddrtile		input[511:0]
`define output_ddrtile		output[511:0]
`define reg_ddrtile_pad		reg[539:0]		//pad to multiple of 18 bits

`else

`ifdef jx2_mem_ddr32B
parameter[255:0] UVDDRT_XX		= UV256_XX;	//
parameter[255:0] UVDDRT_00		= UV256_00;	//
parameter[255:0] UVDDRT_FF		= UV256_FF;	//
`define reg_ddrtile			reg[255:0]
`define wire_ddrtile		wire[255:0]
`define input_ddrtile		input[255:0]
`define output_ddrtile		output[255:0]
`define reg_ddrtile_pad		reg[269:0]		//pad to multiple of 18 bits
`else
parameter[127:0] UVDDRT_XX		= UV128_XX;	//
parameter[127:0] UVDDRT_00		= UV128_00;	//
parameter[127:0] UVDDRT_FF		= UV128_FF;	//
`define reg_ddrtile			reg[127:0]
`define wire_ddrtile		wire[127:0]
`define input_ddrtile		input[127:0]
`define output_ddrtile		output[127:0]
`define reg_ddrtile_pad		reg[143:0]		//pad to multiple of 18 bits
`endif

`endif

`ifdef jx2_mem_ddr32B
`ifndef jx2_mem_line32B
`define jx2_mem_line32to16		//Map 32B to 16B cache-lines
`endif
`endif

`ifdef jx2_enable_vaddr96
`define reg_vaddr			reg[95:0]
`define wire_vaddr			wire[95:0]
`define input_vaddr			input[95:0]
`define output_vaddr		output[95:0]
`else

`ifdef jx2_enable_vaddr48
`define reg_vaddr			reg[47:0]
`define wire_vaddr			wire[47:0]
`define input_vaddr			input[47:0]
`define output_vaddr		output[47:0]
`else
`define reg_vaddr			reg[31:0]
`define wire_vaddr			wire[31:0]
`define input_vaddr			input[31:0]
`define output_vaddr		output[31:0]
`endif

`endif

`define reg_mvaddr			reg[47:0]
`define wire_mvaddr			wire[47:0]
`define input_mvaddr		input[47:0]
`define output_mvaddr		output[47:0]

`ifdef jx2_enable_l1addr96

`define reg_l1addr			reg[95:0]
`define wire_l1addr		wire[95:0]
`define input_l1addr		input[95:0]
`define output_l1addr		output[95:0]

parameter[95:0] UVB1AT_XX		= UV96_XX;	//
parameter[95:0] UVB1AT_00		= UV96_00;	//
parameter[95:0] UVB1AT_FF		= UV96_FF;	//

`else

`define reg_l1addr			reg[47:0]
`define wire_l1addr		wire[47:0]
`define input_l1addr		input[47:0]
`define output_l1addr		output[47:0]

parameter[47:0] UVB1AT_XX		= UV48_XX;	//
parameter[47:0] UVB1AT_00		= UV48_00;	//
parameter[47:0] UVB1AT_FF		= UV48_FF;	//

`endif

`define reg_l1maddr			reg[47:0]
`define wire_l1maddr		wire[47:0]
`define input_l1maddr		input[47:0]
`define output_l1maddr		output[47:0]

parameter[47:0] UVB1MAT_XX		= UV48_XX;	//
parameter[47:0] UVB1MAT_00		= UV48_00;	//
parameter[47:0] UVB1MAT_FF		= UV48_FF;	//


`ifdef jx2_enable_l2addr96

`define reg_l2addr			reg[95:0]
`define wire_l2addr		wire[95:0]
`define input_l2addr		input[95:0]
`define output_l2addr		output[95:0]

parameter[95:0] UVB2AT_XX		= UV96_XX;	//
parameter[95:0] UVB2AT_00		= UV96_00;	//
parameter[95:0] UVB2AT_FF		= UV96_FF;	//

`else

`ifdef jx2_enable_l1addr96
`define jx2_bus_mixaddr96
`endif

`define reg_l2addr			reg[47:0]
`define wire_l2addr		wire[47:0]
`define input_l2addr		input[47:0]
`define output_l2addr		output[47:0]

parameter[47:0] UVB2AT_XX		= UV48_XX;	//
parameter[47:0] UVB2AT_00		= UV48_00;	//
parameter[47:0] UVB2AT_FF		= UV48_FF;	//

`endif

`define reg_rbopm			reg[15:0]
`define wire_rbopm			wire[15:0]
`define input_rbopm			input[15:0]
`define output_rbopm		output[15:0]

parameter[15:0] UVRBOPM_XX		= UV16_XX;	//
parameter[15:0] UVRBOPM_00		= UV16_00;	//
parameter[15:0] UVRBOPM_FF		= UV16_FF;	//


`define reg_rbseq			reg[15:0]
`define wire_rbseq			wire[15:0]
`define input_rbseq			input[15:0]
`define output_rbseq		output[15:0]

parameter[15:0] UVRBSEQ_XX		= UV16_XX;	//
parameter[15:0] UVRBSEQ_00		= UV16_00;	//
parameter[15:0] UVRBSEQ_FF		= UV16_FF;	//

`ifdef jx2_enable_memcap

`define reg_gprval			reg[65:0]
`define wire_gprval			wire[65:0]
`define input_gprval		input[65:0]
`define output_gprval		output[65:0]
parameter[65:0] UVGPRV_XX		= UV66_XX;	//
parameter[65:0] UVGPRV_00		= UV66_00;	//
parameter[65:0] UVGPRV_FF		= UV66_FF;	//

`else

`define reg_gprval			reg[63:0]
`define wire_gprval			wire[63:0]
`define input_gprval		input[63:0]
`define output_gprval		output[63:0]
parameter[63:0] UVGPRV_XX		= UV64_XX;	//
parameter[63:0] UVGPRV_00		= UV64_00;	//
parameter[63:0] UVGPRV_FF		= UV64_FF;	//

`endif

parameter[511:0] UVDDRW_XX		= UV512_XX;	//
parameter[511:0] UVDDRW_00		= UV512_00;	//
parameter[511:0] UVDDRW_FF		= UV512_FF;	//

// `define def_true

`ifdef jx2_enable_fpu
`ifndef jx2_enable_fprs
`define jx2_enable_gfp			//FPU works via GPRs
`endif

`ifdef jx2_enable_fprs
`ifndef jx2_enable_fmov
`define jx2_enable_fmov			//FPU works via GPRs
`endif
`endif

`endif

`ifdef jx2_enable_wex
/* If we are using WEX, assume 16 and 48 bit ops also exist. */
`ifndef jx2_enable_ops16
`define jx2_enable_ops16
`endif
//`ifndef jx2_enable_ops48
//`define jx2_enable_ops48
//`endif
`endif

`ifdef jx2_debug_expipe
`ifndef jx2_debug_keepinstr
`define	jx2_debug_keepinstr
`endif
`endif

`ifdef jx2_debug_exopipe
`ifndef jx2_debug_keepinstr
`define	jx2_debug_keepinstr
`endif
`endif

`ifdef jx2_mem_l1isz_256
parameter[255:0] JX2_L1I_FLUSHMSK	= UV256_FF;		//
`endif
`ifdef jx2_mem_l1isz_128
parameter[127:0] JX2_L1I_FLUSHMSK	= UV128_FF;		//
`endif
`ifdef jx2_mem_l1isz_64
parameter[63:0] JX2_L1I_FLUSHMSK	= UV64_FF;		//
`endif
`ifdef jx2_mem_l1isz_16
parameter[15:0] JX2_L1I_FLUSHMSK	= UV16_FF;		//
`endif

`ifdef jx2_expand_l1dsz
//parameter[511:0] JX2_L1D_FLUSHMSK	= UV512_FF;		//
parameter[255:0] JX2_L1D_FLUSHMSK	= UV256_FF;		//
`else
`ifdef jx2_reduce_l1sz
parameter[15:0] JX2_L1D_FLUSHMSK	= UV16_FF;		//
`else
parameter[63:0] JX2_L1D_FLUSHMSK	= UV64_FF;		//
`endif
`endif

`ifdef jx2_bra2stage
// parameter[7:0] JX2_BRA_FLUSHMSK	= 8'h3F;		//
parameter[7:0] JX2_BRA_FLUSHMSK	= 8'h1F;		//
// parameter[7:0] JX2_BRA_FLUSHMSK	= 8'h0F;		//
`else
parameter[7:0] JX2_BRA_FLUSHMSK	= 8'h0F;		//
`endif

// parameter[7:0] JX2_BRA_FLUSHMSK_XTRA	= 8'h3F;		//
parameter[7:0] JX2_BRA_FLUSHMSK_XTRA	= 8'h7F;		//

`include "ExModKrrKeys.v"

`endif
