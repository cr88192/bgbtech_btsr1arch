/*
GPR Bank (BSR1)

Reads from Rm and Rn.
Writes to Ro.

GPRs will be 16x 32b

SPRs will be 16x 32b.
With relevant registers piped to the outside.

Will have 16 shadow registers.
	R0A..R7A swap with R0B..R7B
	7 of the SPRs also swap with shadow regs.
 */

`include "BsrCoreDefs.v"

module BsrRegGpr(
	/* verilator lint_off UNUSED */
	clock,		reset,
	regIdRm,	regValRm,
	regIdRn,	regValRn,
	regIdRo,	regValRo,

	regSrVal,	regExHold,
	idImm,		idValPc,

	oregSp,		exNextSp2,

	oregPc,		exNextPc2,
	oregPr,		exNextPr2,
	oregSr,		exNextSr2,
	oregVbr,	exNextVbr2,
	oregDlr,	exNextDlr2,
	oregDhr,	exNextDhr2,
	oregGbr,	exNextGbr2,
	oregTbr,	exNextTbr2
	);

input clock;
input reset;

input[5:0]		regIdRm;
input[5:0]		regIdRn;

output[31:0]	regValRm;
output[31:0]	regValRn;

input[5:0]		regIdRo;
input[31:0]		regValRo;

input[31:0]		regSrVal;

input			regExHold;

input[31:0]		idImm;
input[31:0]		idValPc;

input[31:0]		exNextSp2;			//
input[31:0]		exNextPc2;			//PC in
input[31:0]		exNextPr2;			//PR in
input[31:0]		exNextSr2;			//SR in
input[31:0]		exNextVbr2;			//VBR
input[31:0]		exNextDlr2;			//MACL
input[31:0]		exNextDhr2;			//MACH
input[31:0]		exNextGbr2;			//GBR
input[31:0]		exNextTbr2;			//TBR

output[31:0]	oregSp;				//SP (R15)
output[31:0]	oregPc;				//PC
output[31:0]	oregPr;				//PR
output[31:0]	oregSr;				//SR
output[31:0]	oregVbr;			//VBR
output[31:0]	oregDlr;			//MACL
output[31:0]	oregDhr;			//MACH
output[31:0]	oregGbr;			//GBR
output[31:0]	oregTbr;			//TBR


reg[31:0]		regSp;				//SP (R15)
reg[31:0]		regPc;				//PC
reg[31:0]		regPr;				//PR
reg[31:0]		regSr;				//SR
reg[31:0]		regVbr;				//VBR
reg[31:0]		regDlr;				//MACL
reg[31:0]		regDhr;				//MACH
reg[31:0]		regGbr;				//GBR
reg[31:0]		regTbr;				//TBR

assign		oregSp		= regSp;
assign		oregPc		= regPc;
assign		oregPr		= regPr;
assign		oregSr		= regSr;
assign		oregVbr		= regVbr;
assign		oregDlr		= regDlr;
assign		oregDhr		= regDhr;
assign		oregGbr		= regGbr;
assign		oregTbr		= regTbr;

reg			regSrRB;
reg			nxtRegSrRB;

reg[31:0]	tRegValRm;
reg[31:0]	tRegValRn;

reg[31:0]	regGprR0A;
reg[31:0]	regGprR1A;
reg[31:0]	regGprR2A;
reg[31:0]	regGprR3A;
reg[31:0]	regGprR4A;
reg[31:0]	regGprR5A;
reg[31:0]	regGprR6A;
reg[31:0]	regGprR7A;

reg[31:0]	regGprR0B;
reg[31:0]	regGprR1B;
reg[31:0]	regGprR2B;
reg[31:0]	regGprR3B;
reg[31:0]	regGprR4B;
reg[31:0]	regGprR5B;
reg[31:0]	regGprR6B;
reg[31:0]	regGprR7B;

reg[31:0]	regGprR8;
reg[31:0]	regGprR9;
reg[31:0]	regGprR10;
reg[31:0]	regGprR11;
reg[31:0]	regGprR12;
reg[31:0]	regGprR13;
reg[31:0]	regGprR14;

reg[31:0]		regSPc;				//PC
reg[31:0]		regSPr;				//PR
reg[31:0]		regSSr;				//SR
reg[31:0]		regSSp;				//SP/GR (R15)
reg[31:0]		regSDl;				//MACL
reg[31:0]		regSDh;				//MACH
reg[31:0]		regSGb;				//GBR
reg[31:0]		regSTb;				//TBR


assign regValRm = tRegValRm;
assign regValRn = tRegValRn;

always @*
begin
	nxtRegSrRB=regSrVal[29];

	tRegValRm=UV32_XX;
	tRegValRn=UV32_XX;
	
	case(regIdRm)
		BSR_REG_R0:		tRegValRm = regGprR0A ;
		BSR_REG_R1:		tRegValRm = regGprR1A ;
		BSR_REG_R2:		tRegValRm = regGprR2A ;
		BSR_REG_R3:		tRegValRm = regGprR3A ;
		BSR_REG_R4:		tRegValRm = regGprR4A ;
		BSR_REG_R5:		tRegValRm = regGprR5A ;
		BSR_REG_R6:		tRegValRm = regGprR6A ;
		BSR_REG_R7:		tRegValRm = regGprR7A ;
		BSR_REG_R8:		tRegValRm = regGprR8  ;
		BSR_REG_R9:		tRegValRm = regGprR9  ;
		BSR_REG_R10:	tRegValRm = regGprR10 ;
		BSR_REG_R11:	tRegValRm = regGprR11 ;
		BSR_REG_R12:	tRegValRm = regGprR12 ;
		BSR_REG_R13:	tRegValRm = regGprR13 ;
		BSR_REG_R14:	tRegValRm = regGprR14 ;
//		BSR_REG_R15:	tRegValRm = regSp     ;
		BSR_REG_R15:	tRegValRm = exNextSp2 ;

//		BSR_REG_PC:		tRegValRm = idValPc   ;
		BSR_REG_PC:		tRegValRm = idValPc+2 ;
		BSR_REG_LR:		tRegValRm = regPr     ;
		BSR_REG_SR:		tRegValRm = regSr     ;
		BSR_REG_VBR:	tRegValRm = regVbr    ;
//		BSR_REG_DLR:	tRegValRm = regDlr    ;
//		BSR_REG_DHR:	tRegValRm = regDhr    ;
//		BSR_REG_GBR:	tRegValRm = regGbr    ;
//		BSR_REG_TBR:	tRegValRm = regTbr    ;

		BSR_REG_DLR:	tRegValRm = exNextDlr2 ;
		BSR_REG_DHR:	tRegValRm = exNextDhr2 ;
		BSR_REG_GBR:	tRegValRm = exNextGbr2 ;
		BSR_REG_TBR:	tRegValRm = exNextTbr2 ;

		BSR_REG_R0B:	tRegValRm = regGprR0B ;
		BSR_REG_R1B:	tRegValRm = regGprR1B ;
		BSR_REG_R2B:	tRegValRm = regGprR2B ;
		BSR_REG_R3B:	tRegValRm = regGprR3B ;
		BSR_REG_R4B:	tRegValRm = regGprR4B ;
		BSR_REG_R5B:	tRegValRm = regGprR5B ;
		BSR_REG_R6B:	tRegValRm = regGprR6B ;
		BSR_REG_R7B:	tRegValRm = regGprR7B ;
		BSR_REG_SPC:	tRegValRm = regSPc    ;
		BSR_REG_SLR:	tRegValRm = regSPr    ;
		BSR_REG_SSR:	tRegValRm = regSSr    ;
		BSR_REG_SSP:	tRegValRm = regSSp    ;
		BSR_REG_SDL:	tRegValRm = regSDl    ;
		BSR_REG_SDH:	tRegValRm = regSDh    ;
		BSR_REG_SGB:	tRegValRm = regSGb    ;
		BSR_REG_STB:	tRegValRm = regSTb    ;

//		BSR_REG_PCW:	tRegValRm = idValPc + {idImmHi, idImm[30:0], 1'b0} ;

		BSR_REG_IMM:	tRegValRm = idImm;
		BSR_REG_ZZR:	tRegValRm = 0;

		default:		tRegValRm = UV32_XX;
	endcase

	case(regIdRn)
		BSR_REG_R0:		tRegValRn = regGprR0A ;
		BSR_REG_R1:		tRegValRn = regGprR1A ;
		BSR_REG_R2:		tRegValRn = regGprR2A ;
		BSR_REG_R3:		tRegValRn = regGprR3A ;
		BSR_REG_R4:		tRegValRn = regGprR4A ;
		BSR_REG_R5:		tRegValRn = regGprR5A ;
		BSR_REG_R6:		tRegValRn = regGprR6A ;
		BSR_REG_R7:		tRegValRn = regGprR7A ;
		BSR_REG_R8:		tRegValRn = regGprR8  ;
		BSR_REG_R9:		tRegValRn = regGprR9  ;
		BSR_REG_R10:	tRegValRn = regGprR10 ;
		BSR_REG_R11:	tRegValRn = regGprR11 ;
		BSR_REG_R12:	tRegValRn = regGprR12 ;
		BSR_REG_R13:	tRegValRn = regGprR13 ;
		BSR_REG_R14:	tRegValRn = regGprR14 ;
//		BSR_REG_R15:	tRegValRn = regSp     ;
		BSR_REG_R15:	tRegValRn = exNextSp2 ;

		BSR_REG_PC:		tRegValRn = idValPc+2 ;
		BSR_REG_LR:		tRegValRn = regPr     ;
		BSR_REG_SR:		tRegValRn = regSr     ;
		BSR_REG_VBR:	tRegValRn = regVbr    ;
//		BSR_REG_DLR:	tRegValRn = regDlr    ;
//		BSR_REG_DHR:	tRegValRn = regDhr    ;
//		BSR_REG_GBR:	tRegValRn = regGbr    ;
//		BSR_REG_TBR:	tRegValRn = regTbr    ;

		BSR_REG_DLR:	tRegValRn = exNextDlr2 ;
		BSR_REG_DHR:	tRegValRn = exNextDhr2 ;
		BSR_REG_GBR:	tRegValRn = exNextGbr2 ;
		BSR_REG_TBR:	tRegValRn = exNextTbr2 ;

		default:		tRegValRn = UV32_XX   ;
	endcase
	
	if(regIdRm == regIdRo)
		tRegValRm = regValRo;
	if(regIdRn == regIdRo)
		tRegValRn = regValRo;
end

always @ (posedge clock)
begin

	regSrRB		<= nxtRegSrRB;

//	if(regSrRB^nxtRegSrRB)
	if(0)
	begin
		regGprR0A <= (regIdRo==BSR_REG_R0) ? regValRo[31:0] : regGprR0B;
		regGprR1A <= (regIdRo==BSR_REG_R1) ? regValRo[31:0] : regGprR1B;
		regGprR2A <= (regIdRo==BSR_REG_R2) ? regValRo[31:0] : regGprR2B;
		regGprR3A <= (regIdRo==BSR_REG_R3) ? regValRo[31:0] : regGprR3B;
		regGprR4A <= (regIdRo==BSR_REG_R4) ? regValRo[31:0] : regGprR4B;
		regGprR5A <= (regIdRo==BSR_REG_R5) ? regValRo[31:0] : regGprR5B;
		regGprR6A <= (regIdRo==BSR_REG_R6) ? regValRo[31:0] : regGprR6B;
		regGprR7A <= (regIdRo==BSR_REG_R7) ? regValRo[31:0] : regGprR7B;

		regGprR0B <= (regIdRo==BSR_REG_R0B) ? regValRo[31:0] : regGprR0A;
		regGprR1B <= (regIdRo==BSR_REG_R1B) ? regValRo[31:0] : regGprR1A;
		regGprR2B <= (regIdRo==BSR_REG_R2B) ? regValRo[31:0] : regGprR2A;
		regGprR3B <= (regIdRo==BSR_REG_R3B) ? regValRo[31:0] : regGprR3A;
		regGprR4B <= (regIdRo==BSR_REG_R4B) ? regValRo[31:0] : regGprR4A;
		regGprR5B <= (regIdRo==BSR_REG_R5B) ? regValRo[31:0] : regGprR5A;
		regGprR6B <= (regIdRo==BSR_REG_R6B) ? regValRo[31:0] : regGprR6A;
		regGprR7B <= (regIdRo==BSR_REG_R7B) ? regValRo[31:0] : regGprR7A;
	end
	else
	begin
		regGprR0A <= (regIdRo==BSR_REG_R0) ? regValRo[31:0] : regGprR0A;
		regGprR1A <= (regIdRo==BSR_REG_R1) ? regValRo[31:0] : regGprR1A;
		regGprR2A <= (regIdRo==BSR_REG_R2) ? regValRo[31:0] : regGprR2A;
		regGprR3A <= (regIdRo==BSR_REG_R3) ? regValRo[31:0] : regGprR3A;
		regGprR4A <= (regIdRo==BSR_REG_R4) ? regValRo[31:0] : regGprR4A;
		regGprR5A <= (regIdRo==BSR_REG_R5) ? regValRo[31:0] : regGprR5A;
		regGprR6A <= (regIdRo==BSR_REG_R6) ? regValRo[31:0] : regGprR6A;
		regGprR7A <= (regIdRo==BSR_REG_R7) ? regValRo[31:0] : regGprR7A;

		regGprR0B <= (regIdRo==BSR_REG_R0B) ? regValRo[31:0] : regGprR0B;
		regGprR1B <= (regIdRo==BSR_REG_R1B) ? regValRo[31:0] : regGprR1B;
		regGprR2B <= (regIdRo==BSR_REG_R2B) ? regValRo[31:0] : regGprR2B;
		regGprR3B <= (regIdRo==BSR_REG_R3B) ? regValRo[31:0] : regGprR3B;
		regGprR4B <= (regIdRo==BSR_REG_R4B) ? regValRo[31:0] : regGprR4B;
		regGprR5B <= (regIdRo==BSR_REG_R5B) ? regValRo[31:0] : regGprR5B;
		regGprR6B <= (regIdRo==BSR_REG_R6B) ? regValRo[31:0] : regGprR6B;
		regGprR7B <= (regIdRo==BSR_REG_R7B) ? regValRo[31:0] : regGprR7B;
	end

	regGprR8  <= (regIdRo==BSR_REG_R8 ) ? regValRo[31:0] : regGprR8 ;
	regGprR9  <= (regIdRo==BSR_REG_R9 ) ? regValRo[31:0] : regGprR9 ;
	regGprR10 <= (regIdRo==BSR_REG_R10) ? regValRo[31:0] : regGprR10;
	regGprR11 <= (regIdRo==BSR_REG_R11) ? regValRo[31:0] : regGprR11;
	regGprR12 <= (regIdRo==BSR_REG_R12) ? regValRo[31:0] : regGprR12;
	regGprR13 <= (regIdRo==BSR_REG_R13) ? regValRo[31:0] : regGprR13;
	regGprR14 <= (regIdRo==BSR_REG_R14) ? regValRo[31:0] : regGprR14;

	if(reset)
	begin
	/* Common */
		regSp	<= 0;
		regPc	<= 0;
		regPr	<= 0;
		regSr	<= 0;
		regVbr	<= 0;
		regDlr	<= 0;
		regDhr	<= 0;
		regGbr	<= 0;
		regTbr	<= 0;

		regSPc	<= 0;
		regSPr	<= 0;
		regSSr	<= 0;
		regSSp	<= 0;
		regSDl	<= 0;
		regSDh	<= 0;
		regSGb	<= 0;
		regSTb	<= 0;
	end

	else

	if(!regExHold)
	begin
		regVbr	<= (regIdRo==BSR_REG_VBR ) ? regValRo[31:0] : exNextVbr2;

//		if(regSrRB^nxtRegSrRB)
		if(0)
		begin
			regSp	<= (regIdRo==BSR_REG_R15 ) ? regValRo[31:0] : regSSp;
			regPc	<= (regIdRo==BSR_REG_PC  ) ? regValRo[31:0] : regSPc;
			regPr	<= (regIdRo==BSR_REG_LR  ) ? regValRo[31:0] : regSPr;
			regSr	<= (regIdRo==BSR_REG_SR  ) ? regValRo[31:0] : regSSr;
			regDlr	<= (regIdRo==BSR_REG_DLR ) ? regValRo[31:0] : regSDl;
			regDhr	<= (regIdRo==BSR_REG_DHR ) ? regValRo[31:0] : regSDh;
			regGbr	<= (regIdRo==BSR_REG_GBR ) ? regValRo[31:0] : regSGb;
			regTbr	<= (regIdRo==BSR_REG_TBR ) ? regValRo[31:0] : regSTb;
			regSPc	<= (regIdRo==BSR_REG_SPC ) ? regValRo[31:0] : exNextPc2;
			regSPr	<= (regIdRo==BSR_REG_SLR ) ? regValRo[31:0] : exNextPr2;
			regSSr	<= (regIdRo==BSR_REG_SSR ) ? regValRo[31:0] : exNextSr2;
			regSSp	<= (regIdRo==BSR_REG_SSP ) ? regValRo[31:0] : exNextSp2;
			regSDl	<= (regIdRo==BSR_REG_SDL ) ? regValRo[31:0] : exNextDlr2;
			regSDh	<= (regIdRo==BSR_REG_SDH ) ? regValRo[31:0] : exNextDhr2;
			regSGb	<= (regIdRo==BSR_REG_SGB ) ? regValRo[31:0] : exNextGbr2;
			regSTb	<= (regIdRo==BSR_REG_STB ) ? regValRo[31:0] : exNextTbr2;
		end		else	begin
			regSp	<= (regIdRo==BSR_REG_R15 ) ? regValRo[31:0] : exNextSp2;
			regPc	<= (regIdRo==BSR_REG_PC  ) ? regValRo[31:0] : exNextPc2;
			regPr	<= (regIdRo==BSR_REG_LR  ) ? regValRo[31:0] : exNextPr2;
			regSr	<= (regIdRo==BSR_REG_SR  ) ? regValRo[31:0] : exNextSr2;
			regDlr	<= (regIdRo==BSR_REG_DLR ) ? regValRo[31:0] : exNextDlr2;
			regDhr	<= (regIdRo==BSR_REG_DHR ) ? regValRo[31:0] : exNextDhr2;
			regGbr	<= (regIdRo==BSR_REG_GBR ) ? regValRo[31:0] : exNextGbr2;
			regTbr	<= (regIdRo==BSR_REG_TBR ) ? regValRo[31:0] : exNextTbr2;
			regSPc	<= (regIdRo==BSR_REG_SPC ) ? regValRo[31:0] : regSPc;
			regSPr	<= (regIdRo==BSR_REG_SLR ) ? regValRo[31:0] : regSPr;
			regSSr	<= (regIdRo==BSR_REG_SSR ) ? regValRo[31:0] : regSSr;
			regSSp	<= (regIdRo==BSR_REG_SSP ) ? regValRo[31:0] : regSSp;
			regSDl	<= (regIdRo==BSR_REG_SDL ) ? regValRo[31:0] : regSDl;
			regSDh	<= (regIdRo==BSR_REG_SDH ) ? regValRo[31:0] : regSDh;
			regSGb	<= (regIdRo==BSR_REG_SGB ) ? regValRo[31:0] : regSGb;
			regSTb	<= (regIdRo==BSR_REG_STB ) ? regValRo[31:0] : regSTb;
		end
	end
end

endmodule
