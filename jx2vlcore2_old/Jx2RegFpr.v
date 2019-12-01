/*
BJX2 FPR Module

Implements 16x 64-bit floating-point registers.

 */

module Jx2RegFpr(
	/* verilator lint_off UNUSED */
	clock,		reset,
	regIdRm,	regValRm,
	regIdRn,	regValRn,
	regIdRo,	regValRo,

	regSrVal,	regExHold
	);

input clock;
input reset;

input[6:0]		regIdRm;
input[6:0]		regIdRn;

output[63:0]	regValRm;
output[63:0]	regValRn;

input[6:0]		regIdRo;
input[63:0]		regValRo;

input[63:0]		regSrVal;
input			regExHold;

reg			regSrRB;
reg			nxtRegSrRB;

reg[63:0]	tRegValRm;
reg[63:0]	tRegValRn;

`ifndef JX2_FPR_FPRARR
reg[63:0]	regFprR0;
reg[63:0]	regFprR1;
reg[63:0]	regFprR2;
reg[63:0]	regFprR3;
reg[63:0]	regFprR4;
reg[63:0]	regFprR5;
reg[63:0]	regFprR6;
reg[63:0]	regFprR7;

reg[63:0]	regFprR8;
reg[63:0]	regFprR9;
reg[63:0]	regFprR10;
reg[63:0]	regFprR11;
reg[63:0]	regFprR12;
reg[63:0]	regFprR13;
reg[63:0]	regFprR14;
reg[63:0]	regFprR15;
`endif

assign regValRm = tRegValRm;
assign regValRn = tRegValRn;

`ifdef JX2_FPR_FPRARR
reg[63:0]	regFprArrRm[15:0];
reg[63:0]	regFprArrRn[15:0];
`endif

always @*
begin
//	nxtRegSrRB=regSrVal[29];

	tRegValRm=UV64_XX;
	tRegValRn=UV64_XX;

`ifdef JX2_FPR_FPRARR
	tRegValRm = regFprArrRm[regIdRm[3:0]];
	tRegValRn = regFprArrRn[regIdRn[3:0]];
	
	if(regIdRm==JX2_REG_ZZR)
		tRegValRm = 0;
	if(regIdRn==JX2_REG_ZZR)
		tRegValRn = 0;
`else
	case(regIdRm)
		JX2_REG_R0:		tRegValRm = regFprR0  ;
		JX2_REG_R1:		tRegValRm = regFprR1  ;
		JX2_REG_R2:		tRegValRm = regFprR2  ;
		JX2_REG_R3:		tRegValRm = regFprR3  ;
		JX2_REG_R4:		tRegValRm = regFprR4  ;
		JX2_REG_R5:		tRegValRm = regFprR5  ;
		JX2_REG_R6:		tRegValRm = regFprR6  ;
		JX2_REG_R7:		tRegValRm = regFprR7  ;
		JX2_REG_R8:		tRegValRm = regFprR8  ;
		JX2_REG_R9:		tRegValRm = regFprR9  ;
		JX2_REG_R10:	tRegValRm = regFprR10 ;
		JX2_REG_R11:	tRegValRm = regFprR11 ;
		JX2_REG_R12:	tRegValRm = regFprR12 ;
		JX2_REG_R13:	tRegValRm = regFprR13 ;
		JX2_REG_R14:	tRegValRm = regFprR14 ;
		JX2_REG_R15:	tRegValRm = regFprR15 ;

		JX2_REG_ZZR:	tRegValRm = 0;

		default:		tRegValRm = UV64_XX;
	endcase

	case(regIdRn)
		JX2_REG_R0:		tRegValRn = regFprR0  ;
		JX2_REG_R1:		tRegValRn = regFprR1  ;
		JX2_REG_R2:		tRegValRn = regFprR2  ;
		JX2_REG_R3:		tRegValRn = regFprR3  ;
		JX2_REG_R4:		tRegValRn = regFprR4  ;
		JX2_REG_R5:		tRegValRn = regFprR5  ;
		JX2_REG_R6:		tRegValRn = regFprR6  ;
		JX2_REG_R7:		tRegValRn = regFprR7  ;
		JX2_REG_R8:		tRegValRn = regFprR8  ;
		JX2_REG_R9:		tRegValRn = regFprR9  ;
		JX2_REG_R10:	tRegValRn = regFprR10 ;
		JX2_REG_R11:	tRegValRn = regFprR11 ;
		JX2_REG_R12:	tRegValRn = regFprR12 ;
		JX2_REG_R13:	tRegValRn = regFprR13 ;
		JX2_REG_R14:	tRegValRn = regFprR14 ;
		JX2_REG_R15:	tRegValRn = regFprR15 ;

		JX2_REG_ZZR:	tRegValRn = 0;
		default:		tRegValRn = UV64_XX   ;
	endcase
`endif

	if(regIdRm == regIdRo)
		tRegValRm = regValRo;
	if(regIdRn == regIdRo)
		tRegValRn = regValRo;
end

always @ (posedge clock)
begin
	if(!regExHold)
	begin
`ifdef JX2_FPR_FPRARR
		if(regIdRo[6:4]==0)
		begin
			regFprArrRm[regIdRo[3:0]]	<= regValRo;
			regFprArrRn[regIdRo[3:0]]	<= regValRo;
		end
`else
		regFprR0  <= (regIdRo==JX2_REG_R0 ) ? regValRo[63:0] : regFprR0 ;
		regFprR1  <= (regIdRo==JX2_REG_R1 ) ? regValRo[63:0] : regFprR1 ;
		regFprR2  <= (regIdRo==JX2_REG_R2 ) ? regValRo[63:0] : regFprR2 ;
		regFprR3  <= (regIdRo==JX2_REG_R3 ) ? regValRo[63:0] : regFprR3 ;
		regFprR4  <= (regIdRo==JX2_REG_R4 ) ? regValRo[63:0] : regFprR4 ;
		regFprR5  <= (regIdRo==JX2_REG_R5 ) ? regValRo[63:0] : regFprR5 ;
		regFprR6  <= (regIdRo==JX2_REG_R6 ) ? regValRo[63:0] : regFprR6 ;
		regFprR7  <= (regIdRo==JX2_REG_R7 ) ? regValRo[63:0] : regFprR7 ;
		regFprR8  <= (regIdRo==JX2_REG_R8 ) ? regValRo[63:0] : regFprR8 ;
		regFprR9  <= (regIdRo==JX2_REG_R9 ) ? regValRo[63:0] : regFprR9 ;
		regFprR10 <= (regIdRo==JX2_REG_R10) ? regValRo[63:0] : regFprR10;
		regFprR11 <= (regIdRo==JX2_REG_R11) ? regValRo[63:0] : regFprR11;
		regFprR12 <= (regIdRo==JX2_REG_R12) ? regValRo[63:0] : regFprR12;
		regFprR13 <= (regIdRo==JX2_REG_R13) ? regValRo[63:0] : regFprR13;
		regFprR14 <= (regIdRo==JX2_REG_R14) ? regValRo[63:0] : regFprR14;
		regFprR15 <= (regIdRo==JX2_REG_R15) ? regValRo[63:0] : regFprR15;
`endif
	end
end

endmodule
