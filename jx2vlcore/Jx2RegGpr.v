/*
GPR Bank (BJX2)

Reads from Rm and Rn.
Writes to Ro.

GPRs will be 32x 64b

SPRs will be 16x 32b.
With relevant registers piped to the outside.

Will have 16 shadow registers.
	R0A..R7A swap with R0B..R7B
	7 of the SPRs also swap with shadow regs.
 */

`include "Jx2CoreDefs.v"

module Jx2RegGpr(
	/* verilator lint_off UNUSED */
	clock,		reset,
	regIdRm,	regValRm,
	regIdRn,	regValRn,
	regIdRi,	regValRi,
	regIdRo,	regValRo,
	regIdCo,	regValCo,

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

input[6:0]		regIdRm;
input[6:0]		regIdRn;
input[6:0]		regIdRi;

output[63:0]	regValRm;
output[63:0]	regValRn;
output[63:0]	regValRi;

input[6:0]		regIdRo;
input[63:0]		regValRo;

input[6:0]		regIdCo;
input[63:0]		regValCo;

input[63:0]		regSrVal;

input			regExHold;

input[31:0]		idImm;
input[63:0]		idValPc;

input[63:0]		exNextSp2;			//
input[63:0]		exNextPc2;			//PC in
input[63:0]		exNextPr2;			//PR in
input[63:0]		exNextSr2;			//SR in
input[63:0]		exNextVbr2;			//VBR
input[63:0]		exNextDlr2;			//MACL
input[63:0]		exNextDhr2;			//MACH
input[63:0]		exNextGbr2;			//GBR
input[63:0]		exNextTbr2;			//TBR

output[63:0]	oregSp;				//SP (R15)
output[63:0]	oregPc;				//PC
output[63:0]	oregPr;				//PR
output[63:0]	oregSr;				//SR
output[63:0]	oregVbr;			//VBR
output[63:0]	oregDlr;			//MACL
output[63:0]	oregDhr;			//MACH
output[63:0]	oregGbr;			//GBR
output[63:0]	oregTbr;			//TBR


reg[63:0]		regSp;				//SP (R15)
reg[63:0]		regPc;				//PC
reg[63:0]		regPr;				//PR
reg[63:0]		regSr;				//SR
reg[63:0]		regVbr;				//VBR
reg[63:0]		regDlr;				//MACL
reg[63:0]		regDhr;				//MACH
reg[63:0]		regGbr;				//GBR
reg[63:0]		regTbr;				//TBR

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

reg[63:0]	tRegValRm;
reg[63:0]	tRegValRn;
reg[63:0]	tRegValRi;

reg[63:0]	regGprR0A;
reg[63:0]	regGprR1A;
reg[63:0]	regGprR2A;
reg[63:0]	regGprR3A;
reg[63:0]	regGprR4A;
reg[63:0]	regGprR5A;
reg[63:0]	regGprR6A;
reg[63:0]	regGprR7A;

reg[63:0]	regGprR0B;
reg[63:0]	regGprR1B;
reg[63:0]	regGprR2B;
reg[63:0]	regGprR3B;
reg[63:0]	regGprR4B;
reg[63:0]	regGprR5B;
reg[63:0]	regGprR6B;
reg[63:0]	regGprR7B;

reg[63:0]	regGprR8;
reg[63:0]	regGprR9;
reg[63:0]	regGprR10;
reg[63:0]	regGprR11;
reg[63:0]	regGprR12;
reg[63:0]	regGprR13;
reg[63:0]	regGprR14;

reg[63:0]	regGprR16A;
reg[63:0]	regGprR17A;
reg[63:0]	regGprR18A;
reg[63:0]	regGprR19A;
reg[63:0]	regGprR20A;
reg[63:0]	regGprR21A;
reg[63:0]	regGprR22A;
reg[63:0]	regGprR23A;

reg[63:0]	regGprR16B;
reg[63:0]	regGprR17B;
reg[63:0]	regGprR18B;
reg[63:0]	regGprR19B;
reg[63:0]	regGprR20B;
reg[63:0]	regGprR21B;
reg[63:0]	regGprR22B;
reg[63:0]	regGprR23B;

reg[63:0]	regGprR24;
reg[63:0]	regGprR25;
reg[63:0]	regGprR26;
reg[63:0]	regGprR27;
reg[63:0]	regGprR28;
reg[63:0]	regGprR29;
reg[63:0]	regGprR30;
reg[63:0]	regGprR31;

reg[63:0]		regSPc;				//PC
reg[63:0]		regSPr;				//PR
reg[63:0]		regSSr;				//SR
reg[63:0]		regSSp;				//SP/GR (R15)
reg[63:0]		regSDl;				//MACL
reg[63:0]		regSDh;				//MACH
reg[63:0]		regSGb;				//GBR
reg[63:0]		regSTb;				//TBR

reg[63:0]		regValRoA;
reg[63:0]		regValRoB;
reg[63:0]		regValRoC;
reg[63:0]		regValRoD;
reg[63:0]		regValRoE;
reg[63:0]		regValRoF;
reg[6:0]		regIdRoA;
reg[6:0]		regIdRoB;
reg[6:0]		regIdRoC;
reg[6:0]		regIdRoD;
reg[6:0]		regIdRoE;
reg[6:0]		regIdRoF;

reg[63:0]		regValCoA;
reg[63:0]		regValCoB;
reg[6:0]		regIdCoA;
reg[6:0]		regIdCoB;

assign regValRm = tRegValRm;
assign regValRn = tRegValRn;
assign regValRi = tRegValRi;

reg[63:0]		tGenNextPc;

always @*
begin
	nxtRegSrRB=regSrVal[29];

	tRegValRm=UV64_XX;
	tRegValRn=UV64_XX;
	tRegValRi=UV64_XX;
	
	tGenNextPc = { idValPc[63:32], idValPc[31:0]+2 };
	
	case(regIdRm)
		JX2_REG_R0:		tRegValRm = regGprR0A ;
		JX2_REG_R1:		tRegValRm = regGprR1A ;
		JX2_REG_R2:		tRegValRm = regGprR2A ;
		JX2_REG_R3:		tRegValRm = regGprR3A ;
		JX2_REG_R4:		tRegValRm = regGprR4A ;
		JX2_REG_R5:		tRegValRm = regGprR5A ;
		JX2_REG_R6:		tRegValRm = regGprR6A ;
		JX2_REG_R7:		tRegValRm = regGprR7A ;
		JX2_REG_R8:		tRegValRm = regGprR8  ;
		JX2_REG_R9:		tRegValRm = regGprR9  ;
		JX2_REG_R10:	tRegValRm = regGprR10 ;
		JX2_REG_R11:	tRegValRm = regGprR11 ;
		JX2_REG_R12:	tRegValRm = regGprR12 ;
		JX2_REG_R13:	tRegValRm = regGprR13 ;
		JX2_REG_R14:	tRegValRm = regGprR14 ;
//		JX2_REG_R15:	tRegValRm = regSp     ;
		JX2_REG_R15:	tRegValRm = exNextSp2 ;

`ifdef JX2_EN_EGPR
		JX2_REG_R16:	tRegValRm = regGprR16A ;
		JX2_REG_R17:	tRegValRm = regGprR17A ;
		JX2_REG_R18:	tRegValRm = regGprR18A ;
		JX2_REG_R19:	tRegValRm = regGprR19A ;
		JX2_REG_R20:	tRegValRm = regGprR20A ;
		JX2_REG_R21:	tRegValRm = regGprR21A ;
		JX2_REG_R22:	tRegValRm = regGprR22A ;
		JX2_REG_R23:	tRegValRm = regGprR23A ;
		JX2_REG_R24:	tRegValRm = regGprR24  ;
		JX2_REG_R25:	tRegValRm = regGprR25  ;
		JX2_REG_R26:	tRegValRm = regGprR26  ;
		JX2_REG_R27:	tRegValRm = regGprR27  ;
		JX2_REG_R28:	tRegValRm = regGprR28  ;
		JX2_REG_R29:	tRegValRm = regGprR29  ;
		JX2_REG_R30:	tRegValRm = regGprR30  ;
		JX2_REG_R31:	tRegValRm = regGprR31  ;
`endif

//		JX2_REG_PC:		tRegValRm = idValPc+2 ;
		JX2_REG_PC:		tRegValRm = tGenNextPc ;

		JX2_REG_LR:		tRegValRn = exNextPr2 ;
		JX2_REG_SR:		tRegValRm = regSr     ;
		JX2_REG_VBR:	tRegValRm = regVbr    ;
		JX2_REG_DLR:	tRegValRm = exNextDlr2;
		JX2_REG_DHR:	tRegValRm = exNextDhr2;
		JX2_REG_GBR:	tRegValRm = regGbr    ;
		JX2_REG_TBR:	tRegValRm = regTbr    ;

		JX2_REG_R0B:	tRegValRm = regGprR0B ;
		JX2_REG_R1B:	tRegValRm = regGprR1B ;
		JX2_REG_R2B:	tRegValRm = regGprR2B ;
		JX2_REG_R3B:	tRegValRm = regGprR3B ;
		JX2_REG_R4B:	tRegValRm = regGprR4B ;
		JX2_REG_R5B:	tRegValRm = regGprR5B ;
		JX2_REG_R6B:	tRegValRm = regGprR6B ;
		JX2_REG_R7B:	tRegValRm = regGprR7B ;

		JX2_REG_SPC:	tRegValRm = regSPc    ;
		JX2_REG_SLR:	tRegValRm = regSPr    ;
		JX2_REG_SSR:	tRegValRm = regSSr    ;
		JX2_REG_SSP:	tRegValRm = regSSp    ;
		JX2_REG_SDL:	tRegValRm = regSDl    ;
		JX2_REG_SDH:	tRegValRm = regSDh    ;
		JX2_REG_SGB:	tRegValRm = regSGb    ;
		JX2_REG_STB:	tRegValRm = regSTb    ;

`ifdef JX2_EN_EGPR
		JX2_REG_R16B:	tRegValRm = regGprR16B ;
		JX2_REG_R17B:	tRegValRm = regGprR17B ;
		JX2_REG_R18B:	tRegValRm = regGprR18B ;
		JX2_REG_R19B:	tRegValRm = regGprR19B ;
		JX2_REG_R20B:	tRegValRm = regGprR20B ;
		JX2_REG_R21B:	tRegValRm = regGprR21B ;
		JX2_REG_R22B:	tRegValRm = regGprR22B ;
		JX2_REG_R23B:	tRegValRm = regGprR23B ;
`endif

//		JX2_REG_PCW:	tRegValRm = idValPc + {idImmHi, idImm[30:0], 1'b0} ;

		JX2_REG_IMM:	tRegValRm = { idImm[31]?UV32_FF:UV32_00, idImm };
		JX2_REG_ZZR:	tRegValRm = 0;

		default:		tRegValRm = UV64_XX;
	endcase

	case(regIdRn)
		JX2_REG_R0:		tRegValRn = regGprR0A ;
		JX2_REG_R1:		tRegValRn = regGprR1A ;
		JX2_REG_R2:		tRegValRn = regGprR2A ;
		JX2_REG_R3:		tRegValRn = regGprR3A ;
		JX2_REG_R4:		tRegValRn = regGprR4A ;
		JX2_REG_R5:		tRegValRn = regGprR5A ;
		JX2_REG_R6:		tRegValRn = regGprR6A ;
		JX2_REG_R7:		tRegValRn = regGprR7A ;
		JX2_REG_R8:		tRegValRn = regGprR8  ;
		JX2_REG_R9:		tRegValRn = regGprR9  ;
		JX2_REG_R10:	tRegValRn = regGprR10 ;
		JX2_REG_R11:	tRegValRn = regGprR11 ;
		JX2_REG_R12:	tRegValRn = regGprR12 ;
		JX2_REG_R13:	tRegValRn = regGprR13 ;
		JX2_REG_R14:	tRegValRn = regGprR14 ;
//		JX2_REG_R15:	tRegValRn = regSp     ;
		JX2_REG_R15:	tRegValRn = exNextSp2 ;

`ifdef JX2_EN_EGPR
		JX2_REG_R16:	tRegValRn = regGprR16A ;
		JX2_REG_R17:	tRegValRn = regGprR17A ;
		JX2_REG_R18:	tRegValRn = regGprR18A ;
		JX2_REG_R19:	tRegValRn = regGprR19A ;
		JX2_REG_R20:	tRegValRn = regGprR20A ;
		JX2_REG_R21:	tRegValRn = regGprR21A ;
		JX2_REG_R22:	tRegValRn = regGprR22A ;
		JX2_REG_R23:	tRegValRn = regGprR23A ;
		JX2_REG_R24:	tRegValRn = regGprR24  ;
		JX2_REG_R25:	tRegValRn = regGprR25  ;
		JX2_REG_R26:	tRegValRn = regGprR26  ;
		JX2_REG_R27:	tRegValRn = regGprR27  ;
		JX2_REG_R28:	tRegValRn = regGprR28  ;
		JX2_REG_R29:	tRegValRn = regGprR29  ;
		JX2_REG_R30:	tRegValRn = regGprR30  ;
		JX2_REG_R31:	tRegValRn = regGprR31  ;
`endif

//		JX2_REG_PC:		tRegValRn = idValPc+2 ;
		JX2_REG_PC:		tRegValRn = tGenNextPc ;
		JX2_REG_LR:		tRegValRn = exNextPr2 ;
		JX2_REG_SR:		tRegValRn = regSr     ;
		JX2_REG_VBR:	tRegValRn = regVbr    ;
		JX2_REG_DLR:	tRegValRn = exNextDlr2;
		JX2_REG_DHR:	tRegValRn = exNextDhr2;
		JX2_REG_GBR:	tRegValRn = regGbr    ;
		JX2_REG_TBR:	tRegValRn = regTbr    ;

`ifdef JX2_EN_EGPR
		JX2_REG_R16B:	tRegValRn = regGprR16B ;
		JX2_REG_R17B:	tRegValRn = regGprR17B ;
		JX2_REG_R18B:	tRegValRn = regGprR18B ;
		JX2_REG_R19B:	tRegValRn = regGprR19B ;
		JX2_REG_R20B:	tRegValRn = regGprR20B ;
		JX2_REG_R21B:	tRegValRn = regGprR21B ;
		JX2_REG_R22B:	tRegValRn = regGprR22B ;
		JX2_REG_R23B:	tRegValRn = regGprR23B ;
`endif

		default:		tRegValRn = UV64_XX   ;
	endcase

	case(regIdRi)
		JX2_REG_R0:		tRegValRi = regGprR0A ;
		JX2_REG_R1:		tRegValRi = regGprR1A ;
		JX2_REG_R2:		tRegValRi = regGprR2A ;
		JX2_REG_R3:		tRegValRi = regGprR3A ;
		JX2_REG_R4:		tRegValRi = regGprR4A ;
		JX2_REG_R5:		tRegValRi = regGprR5A ;
		JX2_REG_R6:		tRegValRi = regGprR6A ;
		JX2_REG_R7:		tRegValRi = regGprR7A ;
		JX2_REG_R8:		tRegValRi = regGprR8  ;
		JX2_REG_R9:		tRegValRi = regGprR9  ;
		JX2_REG_R10:	tRegValRi = regGprR10 ;
		JX2_REG_R11:	tRegValRi = regGprR11 ;
		JX2_REG_R12:	tRegValRi = regGprR12 ;
		JX2_REG_R13:	tRegValRi = regGprR13 ;
		JX2_REG_R14:	tRegValRi = regGprR14 ;
//		JX2_REG_R15:	tRegValRi = regSp     ;
//		JX2_REG_R15:	tRegValRi = exNextSp2 ;

`ifdef JX2_EN_EGPR
		JX2_REG_R16:	tRegValRi = regGprR16A ;
		JX2_REG_R17:	tRegValRi = regGprR17A ;
		JX2_REG_R18:	tRegValRi = regGprR18A ;
		JX2_REG_R19:	tRegValRi = regGprR19A ;
		JX2_REG_R20:	tRegValRi = regGprR20A ;
		JX2_REG_R21:	tRegValRi = regGprR21A ;
		JX2_REG_R22:	tRegValRi = regGprR22A ;
		JX2_REG_R23:	tRegValRi = regGprR23A ;
		JX2_REG_R24:	tRegValRi = regGprR24  ;
		JX2_REG_R25:	tRegValRi = regGprR25  ;
		JX2_REG_R26:	tRegValRi = regGprR26  ;
		JX2_REG_R27:	tRegValRi = regGprR27  ;
		JX2_REG_R28:	tRegValRi = regGprR28  ;
		JX2_REG_R29:	tRegValRi = regGprR29  ;
		JX2_REG_R30:	tRegValRi = regGprR30  ;
		JX2_REG_R31:	tRegValRi = regGprR31  ;
`endif

		default:		tRegValRi = UV64_XX   ;
	endcase

	regValRoA = regValRo;
	regValRoB = regValRo;
	regValRoC = regValRo;
	regValRoD = regValRo;
	regValRoE = regValRo;
	regValRoF = regValRo;
	
	regIdRoA	= regIdRo;
	regIdRoB	= regIdRo;
	regIdRoC	= regIdRo;
	regIdRoD	= regIdRo;
	regIdRoE	= regIdRo;
	regIdRoF	= regIdRo;

	regValCoA	= regValCo;
	regValCoB	= regValCo;
	regIdCoA	= regIdCo;
	regIdCoB	= regIdCo;
	
	if(regIdRm == regIdRo)
		tRegValRm = regValRo;
	if(regIdRn == regIdRo)
		tRegValRn = regValRo;
end

always @ (posedge clock)
begin

	regSrRB		<= nxtRegSrRB;

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
		if(regSrRB^nxtRegSrRB)
	begin
		regGprR0A <= regGprR0B;		regGprR1A <= regGprR1B;
		regGprR2A <= regGprR2B;		regGprR3A <= regGprR3B;
		regGprR4A <= regGprR4B;		regGprR5A <= regGprR5B;
		regGprR6A <= regGprR6B;		regGprR7A <= regGprR7B;
		regGprR0B <= regGprR0A;		regGprR1B <= regGprR1A;
		regGprR2B <= regGprR2A;		regGprR3B <= regGprR3A;
		regGprR4B <= regGprR4A;		regGprR5B <= regGprR5A;
		regGprR6B <= regGprR6A;		regGprR7B <= regGprR7A;

`ifdef JX2_EN_EGPR
		regGprR16A <= regGprR16B;	regGprR17A <= regGprR17B;
		regGprR18A <= regGprR18B;	regGprR19A <= regGprR19B;
		regGprR20A <= regGprR20B;	regGprR21A <= regGprR21B;
		regGprR22A <= regGprR22B;	regGprR23A <= regGprR23B;
		regGprR16B <= regGprR16A;	regGprR17B <= regGprR17A;
		regGprR18B <= regGprR18A;	regGprR19B <= regGprR19A;
		regGprR20B <= regGprR20A;	regGprR21B <= regGprR21A;
		regGprR22B <= regGprR22A;	regGprR23B <= regGprR23A;
`endif

		regSp	<= regSSp;			regPc	<= regSPc;
		regPr	<= regSPr;			regSr	<= regSSr;
		regDlr	<= regSDl;			regDhr	<= regSDh;
		regGbr	<= regSGb;			regTbr	<= regSTb;
		regSPc	<= exNextPc2;		regSPr	<= exNextPr2;
		regSSr	<= exNextSr2;		regSSp	<= exNextSp2;
		regSDl	<= exNextDlr2;		regSDh	<= exNextDhr2;
		regSGb	<= exNextGbr2;		regSTb	<= exNextTbr2;
	end
	else
		if(!regExHold)
	begin
		regGprR0A <= (regIdRoA==JX2_REG_R0) ? regValRoA[63:0] : regGprR0A;
		regGprR1A <= (regIdRoA==JX2_REG_R1) ? regValRoA[63:0] : regGprR1A;
		regGprR2A <= (regIdRoA==JX2_REG_R2) ? regValRoA[63:0] : regGprR2A;
		regGprR3A <= (regIdRoA==JX2_REG_R3) ? regValRoA[63:0] : regGprR3A;
		regGprR4A <= (regIdRoA==JX2_REG_R4) ? regValRoA[63:0] : regGprR4A;
		regGprR5A <= (regIdRoA==JX2_REG_R5) ? regValRoA[63:0] : regGprR5A;
		regGprR6A <= (regIdRoA==JX2_REG_R6) ? regValRoA[63:0] : regGprR6A;
		regGprR7A <= (regIdRoA==JX2_REG_R7) ? regValRoA[63:0] : regGprR7A;

		regGprR0B <= (regIdCoB==JX2_REG_R0B) ? regValCoB[63:0] : regGprR0B;
		regGprR1B <= (regIdCoB==JX2_REG_R1B) ? regValCoB[63:0] : regGprR1B;
		regGprR2B <= (regIdCoB==JX2_REG_R2B) ? regValCoB[63:0] : regGprR2B;
		regGprR3B <= (regIdCoB==JX2_REG_R3B) ? regValCoB[63:0] : regGprR3B;
		regGprR4B <= (regIdCoB==JX2_REG_R4B) ? regValCoB[63:0] : regGprR4B;
		regGprR5B <= (regIdCoB==JX2_REG_R5B) ? regValCoB[63:0] : regGprR5B;
		regGprR6B <= (regIdCoB==JX2_REG_R6B) ? regValCoB[63:0] : regGprR6B;
		regGprR7B <= (regIdCoB==JX2_REG_R7B) ? regValCoB[63:0] : regGprR7B;

`ifdef JX2_EN_EGPR
		regGprR16A <= (regIdRoC==JX2_REG_R16) ? regValRoC[63:0] : regGprR16A;
		regGprR17A <= (regIdRoC==JX2_REG_R17) ? regValRoC[63:0] : regGprR17A;
		regGprR18A <= (regIdRoC==JX2_REG_R18) ? regValRoC[63:0] : regGprR18A;
		regGprR19A <= (regIdRoC==JX2_REG_R19) ? regValRoC[63:0] : regGprR19A;
		regGprR20A <= (regIdRoC==JX2_REG_R20) ? regValRoC[63:0] : regGprR20A;
		regGprR21A <= (regIdRoC==JX2_REG_R21) ? regValRoC[63:0] : regGprR21A;
		regGprR22A <= (regIdRoC==JX2_REG_R22) ? regValRoC[63:0] : regGprR22A;
		regGprR23A <= (regIdRoC==JX2_REG_R23) ? regValRoC[63:0] : regGprR23A;

		regGprR16B <= (regIdCoA==JX2_REG_R16B) ? regValCoA[63:0] : regGprR16B;
		regGprR17B <= (regIdCoA==JX2_REG_R17B) ? regValCoA[63:0] : regGprR17B;
		regGprR18B <= (regIdCoA==JX2_REG_R18B) ? regValCoA[63:0] : regGprR18B;
		regGprR19B <= (regIdCoA==JX2_REG_R19B) ? regValCoA[63:0] : regGprR19B;
		regGprR20B <= (regIdCoA==JX2_REG_R20B) ? regValCoA[63:0] : regGprR20B;
		regGprR21B <= (regIdCoA==JX2_REG_R21B) ? regValCoA[63:0] : regGprR21B;
		regGprR22B <= (regIdCoA==JX2_REG_R22B) ? regValCoA[63:0] : regGprR22B;
		regGprR23B <= (regIdCoA==JX2_REG_R23B) ? regValCoA[63:0] : regGprR23B;
`endif

		regGprR8  <= (regIdRoC==JX2_REG_R8 ) ? regValRoC[63:0] : regGprR8 ;
		regGprR9  <= (regIdRoC==JX2_REG_R9 ) ? regValRoC[63:0] : regGprR9 ;
		regGprR10 <= (regIdRoC==JX2_REG_R10) ? regValRoC[63:0] : regGprR10;
		regGprR11 <= (regIdRoC==JX2_REG_R11) ? regValRoC[63:0] : regGprR11;
		regGprR12 <= (regIdRoC==JX2_REG_R12) ? regValRoC[63:0] : regGprR12;
		regGprR13 <= (regIdRoC==JX2_REG_R13) ? regValRoC[63:0] : regGprR13;
		regGprR14 <= (regIdRoC==JX2_REG_R14) ? regValRoC[63:0] : regGprR14;

`ifdef JX2_EN_EGPR
		regGprR24 <= (regIdRoD==JX2_REG_R24) ? regValRoD[63:0] : regGprR24;
		regGprR25 <= (regIdRoD==JX2_REG_R25) ? regValRoD[63:0] : regGprR25;
		regGprR26 <= (regIdRoD==JX2_REG_R26) ? regValRoD[63:0] : regGprR26;
		regGprR27 <= (regIdRoD==JX2_REG_R27) ? regValRoD[63:0] : regGprR27;
		regGprR28 <= (regIdRoD==JX2_REG_R28) ? regValRoD[63:0] : regGprR28;
		regGprR29 <= (regIdRoD==JX2_REG_R29) ? regValRoD[63:0] : regGprR29;
		regGprR30 <= (regIdRoD==JX2_REG_R30) ? regValRoD[63:0] : regGprR30;
		regGprR31 <= (regIdRoD==JX2_REG_R31) ? regValRoD[63:0] : regGprR31;
`endif

		regSp	<= (regIdRoE==JX2_REG_R15 ) ? regValRoE[63:0] : exNextSp2;

		regPc	<= (regIdCoA==JX2_REG_PC  ) ? regValCoA[63:0] : exNextPc2;
		regPr	<= (regIdCoA==JX2_REG_LR  ) ? regValCoA[63:0] : exNextPr2;
		regSr	<= (regIdCoA==JX2_REG_SR  ) ? regValCoA[63:0] : exNextSr2;
		regVbr	<= (regIdCoA==JX2_REG_VBR ) ? regValCoA[63:0] : exNextVbr2;

		regDlr	<= (regIdCoB==JX2_REG_DLR ) ? regValCoB[63:0] : exNextDlr2;
		regDhr	<= (regIdCoB==JX2_REG_DHR ) ? regValCoB[63:0] : exNextDhr2;
		regGbr	<= (regIdCoB==JX2_REG_GBR ) ? regValCoB[63:0] : exNextGbr2;
		regTbr	<= (regIdCoB==JX2_REG_TBR ) ? regValCoB[63:0] : exNextTbr2;

		regSPc	<= (regIdCoA==JX2_REG_SPC ) ? regValCoA[63:0] : regSPc;
		regSPr	<= (regIdCoA==JX2_REG_SLR ) ? regValCoA[63:0] : regSPr;
		regSSr	<= (regIdCoA==JX2_REG_SSR ) ? regValCoA[63:0] : regSSr;
		regSSp	<= (regIdCoA==JX2_REG_SSP ) ? regValCoA[63:0] : regSSp;

		regSDl	<= (regIdCoB==JX2_REG_SDL ) ? regValCoB[63:0] : regSDl;
		regSDh	<= (regIdCoB==JX2_REG_SDH ) ? regValCoB[63:0] : regSDh;
		regSGb	<= (regIdCoB==JX2_REG_SGB ) ? regValCoB[63:0] : regSGb;
		regSTb	<= (regIdCoB==JX2_REG_STB ) ? regValCoB[63:0] : regSTb;

	end
end

endmodule
