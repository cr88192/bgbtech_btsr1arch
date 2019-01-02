/*
GPR Bank (BJX2)

Reads from Rm, Rn, and Ri.
Writes to Ro.

GPRs will be 32x 64b

SPRs will be 16x 32b.
With relevant registers piped to the outside.

Will have 16 shadow registers.
	R0A..R7A swap with R0B..R7B
	7 of the SPRs also swap with shadow regs.

Drop GPRs for Shadow Regs.

 */

`include "Jx2CoreDefs.v"


module Jx2RegGpr(
	/* verilator lint_off UNUSED */
	clock,		reset,
	regIdRm,	regValRm,
	regIdRn,	regValRn,
	regIdRi,	regValRi,
	regIdRo,	regValRo,
	regIdCm,	regValCm,
	regIdCo,	regValCo,

	regSrVal,	regExHold,
	idImm,		idValPc,
	idStepPc,	idNextPc,

	oregSp,		exNextSp2,

	oregPc,		exNextPc2,
	oregPr,		exNextPr2,
	oregSr,		exNextSr2,
	oregVbr,	exNextVbr2,
	oregDlr,	exNextDlr2,
	oregDhr,	exNextDhr2,
	oregGbr,	exNextGbr2,
	oregTbr,	exNextTbr2,
	oregExc,	exNextExc2,
	oregTea,	exNextTea2,
	
	oregMmcr,
	oregKrr
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

input[6:0]		regIdCm;
output[63:0]	regValCm;

input[6:0]		regIdCo;
input[63:0]		regValCo;

input[63:0]		regSrVal;

input			regExHold;

input[32:0]		idImm;
input[63:0]		idValPc;
input[1:0]		idStepPc;
input[63:0]		idNextPc;

input[63:0]		exNextSp2;			//
input[63:0]		exNextPc2;			//PC in
input[63:0]		exNextPr2;			//PR in
input[63:0]		exNextSr2;			//SR in
input[63:0]		exNextVbr2;			//VBR
input[63:0]		exNextDlr2;			//MACL / DLR
input[63:0]		exNextDhr2;			//MACH / DHR
input[63:0]		exNextGbr2;			//GBR
input[63:0]		exNextTbr2;			//TBR
input[63:0]		exNextExc2;			//EXC
input[63:0]		exNextTea2;			//TEA

output[63:0]	oregSp;				//SP (R15)
output[63:0]	oregPc;				//PC
output[63:0]	oregPr;				//PR
output[63:0]	oregSr;				//SR
output[63:0]	oregVbr;			//VBR
output[63:0]	oregDlr;			//MACL
output[63:0]	oregDhr;			//MACH
output[63:0]	oregGbr;			//GBR
output[63:0]	oregTbr;			//TBR
output[63:0]	oregExc;			//EXSR
output[63:0]	oregTea;			//TEA

output[63:0]	oregMmcr;			//MMCR
output[63:0]	oregKrr;			//KRR


reg[63:0]		regSp;				//SP (R15)
reg[63:0]		regPc;				//PC
reg[63:0]		regPr;				//PR
reg[63:0]		regSr;				//SR
reg[63:0]		regVbr;				//VBR
reg[63:0]		regDlr;				//MACL
reg[63:0]		regDhr;				//MACH
reg[63:0]		regGbr;				//GBR
reg[63:0]		regTbr;				//TBR

reg[63:0]		regTtb;				//TTB
reg[63:0]		regTea;				//TEA
reg[63:0]		regMmcr;			//MMCR
reg[63:0]		regExsr;			//EXSR
reg[63:0]		regSttb;			//STTB
reg[63:0]		regKrr;				//KRR

assign		oregSp		= regSp;
assign		oregPc		= regPc;
assign		oregPr		= regPr;
assign		oregSr		= regSr;
assign		oregVbr		= regVbr;
assign		oregDlr		= regDlr;
assign		oregDhr		= regDhr;
assign		oregGbr		= regGbr;
assign		oregTbr		= regTbr;
assign		oregExc		= regExsr;
assign		oregTea		= regTea;

assign		oregMmcr	= regMmcr;
assign		oregKrr		= regKrr;

reg			regSrRB;
reg			nxtRegSrRB;

reg[63:0]	tRegValRm;
reg[63:0]	tRegValRn;
reg[63:0]	tRegValRi;
reg[63:0]	tRegValCm;

// reg[63:0]	regGprR0;
// reg[63:0]	regGprR1;

`ifndef JX2_GPR_GPRARR
reg[63:0]	regGprR2;
reg[63:0]	regGprR3;
reg[63:0]	regGprR4;
reg[63:0]	regGprR5;
reg[63:0]	regGprR6;
reg[63:0]	regGprR7;
reg[63:0]	regGprR8;
reg[63:0]	regGprR9;
reg[63:0]	regGprR10;
reg[63:0]	regGprR11;
reg[63:0]	regGprR12;
reg[63:0]	regGprR13;
reg[63:0]	regGprR14;

reg[63:0]	regGprR16;
reg[63:0]	regGprR17;
reg[63:0]	regGprR18;
reg[63:0]	regGprR19;
reg[63:0]	regGprR20;
reg[63:0]	regGprR21;
reg[63:0]	regGprR22;
reg[63:0]	regGprR23;
reg[63:0]	regGprR24;
reg[63:0]	regGprR25;
reg[63:0]	regGprR26;
reg[63:0]	regGprR27;
reg[63:0]	regGprR28;
reg[63:0]	regGprR29;
reg[63:0]	regGprR30;
reg[63:0]	regGprR31;
`endif

`ifdef JX2_GPR_GPRARR
// reg[63:0]	regGprArrN[31:0];
// reg[63:0]	regGprArrM[31:0];
// reg[63:0]	regGprArrO[31:0];

reg[63:0]	regGprArrN[63:0];
reg[63:0]	regGprArrM[63:0];
reg[63:0]	regGprArrO[63:0];
`endif

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

assign regValCm = tRegValCm;


wire[6:0]		regIdRn1;
wire[6:0]		regIdRm1;
wire[6:0]		regIdRi1;
wire[6:0]		regIdCm1;
wire[6:0]		regIdRo1;
wire[6:0]		regIdCo1;

assign	regIdRm1	= {1'b0, regIdRm[5:0]};
assign	regIdRn1	= {1'b0, regIdRn[5:0]};
assign	regIdRi1	= {1'b0, regIdRi[5:0]};
assign	regIdCm1	= {1'b0, regIdCm[5:0]};
assign	regIdRo1	= {1'b0, regIdRo[5:0]};
assign	regIdCo1	= {1'b0, regIdCo[5:0]};


reg[63:0]		tGenNextPc;
reg[63:0]		tGenImm64;

reg				tRegOpSticky1;
reg				tRegOpSticky2;
reg				tRegOpSticky3;
reg				tNextOpSticky1;
reg				tNextOpSticky2;
reg				tNextOpSticky3;

always @*
begin
//	nxtRegSrRB=regSrVal[29];
	nxtRegSrRB=exNextSr2[29];

	tRegValRm=UV64_XX;
	tRegValRn=UV64_XX;
	tRegValRi=UV64_XX;
	
	tNextOpSticky1	= 0;
	tNextOpSticky2	= 0;
	tNextOpSticky3	= 0;

//	tGenNextPc = { idValPc[63:32], idValPc[31:0]+2 };

`ifndef JX2_QUIET
//	if(idStepPc==0)
//		$display("Bad Pc Step");
`endif

//	tGenNextPc = { idValPc[63:32], idValPc[31:0] +
//		{29'b0, idStepPc, 1'b0} };
	tGenNextPc = idNextPc;
	
//	$display("GPR Next PC %X", tGenNextPc);
	
	tGenImm64 = { idImm[32]?UV32_FF:UV32_00, idImm[31:0] };

`ifdef JX2_GPR_GPRARR
//	tRegValRm = regGprArrM[regIdRm[4:0]];
//	tRegValRn = regGprArrN[regIdRn[4:0]];
//	tRegValRi = regGprArrO[regIdRi[4:0]];

//	tRegValRm = regGprArrM[regIdRm[4:0]];
//	tRegValRn = regGprArrM[regIdRn[4:0]];
//	tRegValRi = regGprArrM[regIdRi[4:0]];

	tRegValRm = regGprArrM[regIdRm1[5:0]];
	tRegValRn = regGprArrN[regIdRn1[5:0]];
	tRegValRi = regGprArrO[regIdRi1[5:0]];
`else
	tRegValRm = UV64_XX;
	tRegValRn = UV64_XX;
	tRegValRi = UV64_XX;
`endif
	
	case(regIdRm1)
		JX2_REG_R0:		tRegValRm = exNextDlr2;
		JX2_REG_R1:		tRegValRm = exNextDhr2;
		JX2_REG_R15:	tRegValRm = exNextSp2 ;

`ifndef JX2_GPR_GPRARR
		JX2_REG_R2:		tRegValRm = regGprR2  ;
		JX2_REG_R3:		tRegValRm = regGprR3  ;
		JX2_REG_R4:		tRegValRm = regGprR4  ;
		JX2_REG_R5:		tRegValRm = regGprR5  ;
		JX2_REG_R6:		tRegValRm = regGprR6  ;
		JX2_REG_R7:		tRegValRm = regGprR7  ;
		JX2_REG_R8:		tRegValRm = regGprR8  ;
		JX2_REG_R9:		tRegValRm = regGprR9  ;
		JX2_REG_R10:	tRegValRm = regGprR10 ;
		JX2_REG_R11:	tRegValRm = regGprR11 ;
		JX2_REG_R12:	tRegValRm = regGprR12 ;
		JX2_REG_R13:	tRegValRm = regGprR13 ;
		JX2_REG_R14:	tRegValRm = regGprR14 ;

		JX2_REG_R16:	tRegValRm = regGprR16 ;
		JX2_REG_R17:	tRegValRm = regGprR17 ;
		JX2_REG_R18:	tRegValRm = regGprR18 ;
		JX2_REG_R19:	tRegValRm = regGprR19 ;
		JX2_REG_R20:	tRegValRm = regGprR20 ;
		JX2_REG_R21:	tRegValRm = regGprR21 ;
		JX2_REG_R22:	tRegValRm = regGprR22 ;
		JX2_REG_R23:	tRegValRm = regGprR23 ;
		JX2_REG_R24:	tRegValRm = regGprR24 ;
		JX2_REG_R25:	tRegValRm = regGprR25 ;
		JX2_REG_R26:	tRegValRm = regGprR26 ;
		JX2_REG_R27:	tRegValRm = regGprR27 ;
		JX2_REG_R28:	tRegValRm = regGprR28 ;
		JX2_REG_R29:	tRegValRm = regGprR29 ;
		JX2_REG_R30:	tRegValRm = regGprR30 ;
		JX2_REG_R31:	tRegValRm = regGprR31 ;
`endif

		JX2_REG_PC:
		begin
//			$display("GPR Next PC %X", tGenNextPc);
			tRegValRm = tGenNextPc;
		end

		JX2_REG_GBR:	tRegValCm = regGbr    ;
		JX2_REG_TBR:	tRegValCm = regTbr    ;

		JX2_REG_IMM:	tRegValRm = tGenImm64 ;
		JX2_REG_ZZR:	tRegValRm = 0         ;

		default:
		begin
`ifndef JX2_QUIET
			if((regIdRm[5:4]!=0) && !tRegOpSticky1)
				$display("Read Rm, Bad %X", regIdRm);
			tNextOpSticky1 = 1;
`endif
		end
	endcase

	case(regIdRn1)
		JX2_REG_R0:		tRegValRn = exNextDlr2;
		JX2_REG_R1:		tRegValRn = exNextDhr2;
		JX2_REG_R15:	tRegValRn = exNextSp2 ;

`ifndef JX2_GPR_GPRARR
		JX2_REG_R2:		tRegValRn = regGprR2  ;
		JX2_REG_R3:		tRegValRn = regGprR3  ;
		JX2_REG_R4:		tRegValRn = regGprR4  ;
		JX2_REG_R5:		tRegValRn = regGprR5  ;
		JX2_REG_R6:		tRegValRn = regGprR6  ;
		JX2_REG_R7:		tRegValRn = regGprR7  ;
		JX2_REG_R8:		tRegValRn = regGprR8  ;
		JX2_REG_R9:		tRegValRn = regGprR9  ;
		JX2_REG_R10:	tRegValRn = regGprR10 ;
		JX2_REG_R11:	tRegValRn = regGprR11 ;
		JX2_REG_R12:	tRegValRn = regGprR12 ;
		JX2_REG_R13:	tRegValRn = regGprR13 ;
		JX2_REG_R14:	tRegValRn = regGprR14 ;

		JX2_REG_R16:	tRegValRn = regGprR16 ;
		JX2_REG_R17:	tRegValRn = regGprR17 ;
		JX2_REG_R18:	tRegValRn = regGprR18 ;
		JX2_REG_R19:	tRegValRn = regGprR19 ;
		JX2_REG_R20:	tRegValRn = regGprR20 ;
		JX2_REG_R21:	tRegValRn = regGprR21 ;
		JX2_REG_R22:	tRegValRn = regGprR22 ;
		JX2_REG_R23:	tRegValRn = regGprR23 ;
		JX2_REG_R24:	tRegValRn = regGprR24 ;
		JX2_REG_R25:	tRegValRn = regGprR25 ;
		JX2_REG_R26:	tRegValRn = regGprR26 ;
		JX2_REG_R27:	tRegValRn = regGprR27 ;
		JX2_REG_R28:	tRegValRn = regGprR28 ;
		JX2_REG_R29:	tRegValRn = regGprR29 ;
		JX2_REG_R30:	tRegValRn = regGprR30 ;
		JX2_REG_R31:	tRegValRn = regGprR31 ;
`endif

//		JX2_REG_PC:		tRegValRn = tGenNextPc;
//		JX2_REG_GBR:	tRegValCm = regGbr    ;
//		JX2_REG_TBR:	tRegValCm = regTbr    ;

		JX2_REG_IMM:	tRegValRn = tGenImm64 ;
		JX2_REG_ZZR:	tRegValRn = 0         ;

		default:
		begin
`ifndef JX2_QUIET
			if((regIdRn[5:4]!=0) && !tRegOpSticky1)
				$display("Read Rn, Bad %X", regIdRn);
			tNextOpSticky1 = 1;
`endif
		end
	endcase

	case(regIdRi1)
		JX2_REG_R0:		tRegValRi = exNextDlr2;
		JX2_REG_R1:		tRegValRi = exNextDhr2;
		JX2_REG_R15:	tRegValRi = exNextSp2 ;

`ifndef JX2_GPR_GPRARR
		JX2_REG_R2:		tRegValRi = regGprR2  ;
		JX2_REG_R3:		tRegValRi = regGprR3  ;
		JX2_REG_R4:		tRegValRi = regGprR4  ;
		JX2_REG_R5:		tRegValRi = regGprR5  ;
		JX2_REG_R6:		tRegValRi = regGprR6  ;
		JX2_REG_R7:		tRegValRi = regGprR7  ;
		JX2_REG_R8:		tRegValRi = regGprR8  ;
		JX2_REG_R9:		tRegValRi = regGprR9  ;
		JX2_REG_R10:	tRegValRi = regGprR10 ;
		JX2_REG_R11:	tRegValRi = regGprR11 ;
		JX2_REG_R12:	tRegValRi = regGprR12 ;
		JX2_REG_R13:	tRegValRi = regGprR13 ;
		JX2_REG_R14:	tRegValRi = regGprR14 ;

		JX2_REG_R16:	tRegValRi = regGprR16 ;
		JX2_REG_R17:	tRegValRi = regGprR17 ;
		JX2_REG_R18:	tRegValRi = regGprR18 ;
		JX2_REG_R19:	tRegValRi = regGprR19 ;
		JX2_REG_R20:	tRegValRi = regGprR20 ;
		JX2_REG_R21:	tRegValRi = regGprR21 ;
		JX2_REG_R22:	tRegValRi = regGprR22 ;
		JX2_REG_R23:	tRegValRi = regGprR23 ;
		JX2_REG_R24:	tRegValRi = regGprR24 ;
		JX2_REG_R25:	tRegValRi = regGprR25 ;
		JX2_REG_R26:	tRegValRi = regGprR26 ;
		JX2_REG_R27:	tRegValRi = regGprR27 ;
		JX2_REG_R28:	tRegValRi = regGprR28 ;
		JX2_REG_R29:	tRegValRi = regGprR29 ;
		JX2_REG_R30:	tRegValRi = regGprR30 ;
		JX2_REG_R31:	tRegValRi = regGprR31 ;
`endif

		JX2_REG_IMM:	tRegValRi = tGenImm64 ;
		JX2_REG_ZZR:	tRegValRi = 0         ;

		default:
		begin
`ifndef JX2_QUIET
			if((regIdRi[5:4]!=0) && !tRegOpSticky1)
				$display("Read Ri, Bad %X", regIdRi);
			tNextOpSticky1 = 1;
`endif
		end
	endcase

	case(regIdCm1)
		JX2_REG_PC:		tRegValCm = tGenNextPc;
		JX2_REG_LR:		tRegValCm = exNextPr2 ;
		JX2_REG_SR:		tRegValCm = regSr     ;
		JX2_REG_VBR:	tRegValCm = regVbr    ;
		JX2_REG_DLR:	tRegValCm = exNextDlr2;
		JX2_REG_DHR:	tRegValCm = exNextDhr2;
		JX2_REG_GBR:	tRegValCm = regGbr    ;
		JX2_REG_TBR:	tRegValCm = regTbr    ;

		JX2_REG_TTB:	tRegValCm = regTtb    ;
		JX2_REG_TEA:	tRegValCm = regTea    ;
		JX2_REG_MMCR:	tRegValCm = regMmcr   ;
		JX2_REG_EXSR:	tRegValCm = regExsr   ;
		JX2_REG_STTB:	tRegValCm = regSttb   ;
		JX2_REG_KRR:	tRegValCm = regKrr    ;

		JX2_REG_SPC:	tRegValCm = regSPc    ;
		JX2_REG_SLR:	tRegValCm = regSPr    ;
		JX2_REG_SSR:	tRegValCm = regSSr    ;
		JX2_REG_SSP:	tRegValCm = regSSp    ;
		JX2_REG_SDL:	tRegValCm = regSDl    ;
		JX2_REG_SDH:	tRegValCm = regSDh    ;
		JX2_REG_SGB:	tRegValCm = regSGb    ;
		JX2_REG_STB:	tRegValCm = regSTb    ;

		default:		tRegValCm = UV64_XX   ;
	endcase

`ifndef JX2_GPR_GPR2CWB

	regValRoA = regValRo;
	regValRoB = regValRo;
	regValRoC = regValRo;
	regValRoD = regValRo;
	regValRoE = regValRo;
	regValRoF = regValRo;
	
	regIdRoA	= regIdRo1;
	regIdRoB	= regIdRo1;
	regIdRoC	= regIdRo1;
	regIdRoD	= regIdRo1;
	regIdRoE	= regIdRo1;
	regIdRoF	= regIdRo1;

	regValCoA	= regValCo;
	regValCoB	= regValCo;
	regIdCoA	= regIdCo1;
	regIdCoB	= regIdCo1;
`endif

`ifdef JX2_GPR_GPR2CWB
	if(regIdRm1 == regIdRoA)
		tRegValRm = regValRoA;
	if(regIdRn1 == regIdRoB)
		tRegValRn = regValRoB;
	if(regIdRi1 == regIdRoC)
		tRegValRi = regValRoC;

	if(regIdCm1 == regIdCoA)
		tRegValCm = regValCoA;
`endif
	
//	if(regIdRm == regIdRo)
	if(regIdRm1 == regIdRo1)
		tRegValRm = regValRo;
//	if(regIdRn == regIdRo)
	if(regIdRn1 == regIdRo1)
		tRegValRn = regValRo;
//	if(regIdRi == regIdRo)
	if(regIdRi1 == regIdRo1)
		tRegValRi = regValRo;

	if(regIdCm1 == regIdCo1)
		tRegValCm = regValCo;
end

always @ (posedge clock)
begin

	tRegOpSticky1	<= tNextOpSticky1;
	tRegOpSticky2	<= tNextOpSticky2;
	tRegOpSticky3	<= tNextOpSticky3;


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
//		regSp	<= regSSp;
//		regPc	<= regSPc;
//		regPr	<= regSPr;
//		regSr	<= regSSr;
//		regDlr	<= regSDl;			regDhr	<= regSDh;
//		regGbr	<= regSGb;			regTbr	<= regSTb;
//		regSPc	<= exNextPc2;

//		regSp	<= regSSp;
//		regSSp	<= regSp;

//		regSPr	<= exNextPr2;
//		regSSr	<= exNextSr2;
//		regSSp	<= exNextSp2;
//		regSDl	<= exNextDlr2;		regSDh	<= exNextDhr2;
//		regSGb	<= exNextGbr2;		regSTb	<= exNextTbr2;

		if(nxtRegSrRB)
		begin
			regSPc	<= { exNextExc2[63]?UV16_FF:UV16_00, exNextExc2[63:16] };
			regPc	<= exNextPc2;

			regSSp	<= regSp;

//			regSSr	<= regSr;
			regSr	<= exNextSr2;

			regTea	<= exNextTea2;
//			regExsr	<= exNextExc2;
			regExsr	<= { regSr[31:0], exNextExc2[31:0] };
		end
		else
		begin
			regPc	<= regSPc;
			regSp	<= regSSp;
			regSr	<= { regSr[63:32], regExsr[63:32] };

//			regSPc	<= regPc;
//			regSr	<= regSSr;
//			regSSr	<= exNextSr2;

			regTea	<= exNextTea2;
			regExsr	<= exNextExc2;
		end

	end
	else
		if(!regExHold)
	begin

`ifdef JX2_GPR_GPR2CWB
		regValRoA	<= regValRo;
		regValRoB	<= regValRo;
		regValRoC	<= regValRo;
		regValRoD	<= regValRo;
		regValRoE	<= regValRo;
		regValRoF	<= regValRo;
		regIdRoA	<= regIdRo1;
		regIdRoB	<= regIdRo1;
		regIdRoC	<= regIdRo1;
		regIdRoD	<= regIdRo1;
		regIdRoE	<= regIdRo1;
		regIdRoF	<= regIdRo1;
		regValCoA	<= regValCo;
		regValCoB	<= regValCo;
		regIdCoA	<= regIdCo1;
		regIdCoB	<= regIdCo1;
`endif

		/* GPR Port */

`ifdef JX2_GPR_GPRARR
//		if(regIdRoA[6:5]==0)
//		begin
//			regGprArrM[regIdRoA[4:0]] <= regValRoA;
//			regGprArrN[regIdRoA[4:0]] <= regValRoA;
//			regGprArrO[regIdRoA[4:0]] <= regValRoA;
//		end

		regGprArrM[regIdRoA[5:0]] <= regValRoA;
		regGprArrN[regIdRoB[5:0]] <= regValRoB;
		regGprArrO[regIdRoC[5:0]] <= regValRoC;
`endif

//		regDlr     <= (regIdRoA==JX2_REG_R0 ) ? regValRoA : exNextDlr2;
//		regDhr     <= (regIdRoA==JX2_REG_R1 ) ? regValRoA : exNextDhr2;
//		regSp      <= (regIdRoE==JX2_REG_R15) ? regValRoE : exNextSp2;

		regDlr     <= (regIdRo==JX2_REG_R0 ) ? regValRo : exNextDlr2;
		regDhr     <= (regIdRo==JX2_REG_R1 ) ? regValRo : exNextDhr2;
		regSp      <= (regIdRo==JX2_REG_R15) ? regValRo : exNextSp2;

`ifndef JX2_GPR_GPRARR
		regGprR2   <= (regIdRoA==JX2_REG_R2 ) ? regValRoA : regGprR2;
		regGprR3   <= (regIdRoA==JX2_REG_R3 ) ? regValRoA : regGprR3;
		regGprR4   <= (regIdRoA==JX2_REG_R4 ) ? regValRoA : regGprR4;
		regGprR5   <= (regIdRoA==JX2_REG_R5 ) ? regValRoA : regGprR5;
		regGprR6   <= (regIdRoA==JX2_REG_R6 ) ? regValRoA : regGprR6;
		regGprR7   <= (regIdRoA==JX2_REG_R7 ) ? regValRoA : regGprR7;

		regGprR8   <= (regIdRoC==JX2_REG_R8 ) ? regValRoC : regGprR8 ;
		regGprR9   <= (regIdRoC==JX2_REG_R9 ) ? regValRoC : regGprR9 ;
		regGprR10  <= (regIdRoC==JX2_REG_R10) ? regValRoC : regGprR10;
		regGprR11  <= (regIdRoC==JX2_REG_R11) ? regValRoC : regGprR11;
		regGprR12  <= (regIdRoC==JX2_REG_R12) ? regValRoC : regGprR12;
		regGprR13  <= (regIdRoC==JX2_REG_R13) ? regValRoC : regGprR13;
		regGprR14  <= (regIdRoC==JX2_REG_R14) ? regValRoC : regGprR14;

		regGprR16  <= (regIdRoB==JX2_REG_R16) ? regValRoB : regGprR16;
		regGprR17  <= (regIdRoB==JX2_REG_R17) ? regValRoB : regGprR17;
		regGprR18  <= (regIdRoB==JX2_REG_R18) ? regValRoB : regGprR18;
		regGprR19  <= (regIdRoB==JX2_REG_R19) ? regValRoB : regGprR19;
		regGprR20  <= (regIdRoB==JX2_REG_R20) ? regValRoB : regGprR20;
		regGprR21  <= (regIdRoB==JX2_REG_R21) ? regValRoB : regGprR21;
		regGprR22  <= (regIdRoB==JX2_REG_R22) ? regValRoB : regGprR22;
		regGprR23  <= (regIdRoB==JX2_REG_R23) ? regValRoB : regGprR23;

		regGprR24  <= (regIdRoD==JX2_REG_R24) ? regValRoD : regGprR24;
		regGprR25  <= (regIdRoD==JX2_REG_R25) ? regValRoD : regGprR25;
		regGprR26  <= (regIdRoD==JX2_REG_R26) ? regValRoD : regGprR26;
		regGprR27  <= (regIdRoD==JX2_REG_R27) ? regValRoD : regGprR27;
		regGprR28  <= (regIdRoD==JX2_REG_R28) ? regValRoD : regGprR28;
		regGprR29  <= (regIdRoD==JX2_REG_R29) ? regValRoD : regGprR29;
		regGprR30  <= (regIdRoD==JX2_REG_R30) ? regValRoD : regGprR30;
		regGprR31  <= (regIdRoD==JX2_REG_R31) ? regValRoD : regGprR31;
`endif

		/* Control Register Port */

//		regSDl     <= (regIdCoB==JX2_REG_R0B ) ? regValCoB : regSDl;
//		regSDh     <= (regIdCoB==JX2_REG_R1B ) ? regValCoB : regSDh;

		regPc	<= (regIdCoA==JX2_REG_PC  ) ? regValCoA : exNextPc2;
		regPr	<= (regIdCoA==JX2_REG_LR  ) ? regValCoA : exNextPr2;
		regSr	<= (regIdCoA==JX2_REG_SR  ) ? regValCoA : exNextSr2;
//		regVbr	<= (regIdCoA==JX2_REG_VBR ) ? regValCoA : exNextVbr2;
		regVbr	<= (regIdCoA==JX2_REG_VBR ) ? regValCoA : regVbr;

//		regGbr	<= (regIdCoB==JX2_REG_GBR ) ? regValCoB : exNextGbr2;
//		regTbr	<= (regIdCoB==JX2_REG_TBR ) ? regValCoB : exNextTbr2;

		regGbr	<= (regIdCoB==JX2_REG_GBR ) ? regValCoB : regGbr;
		regTbr	<= (regIdCoB==JX2_REG_TBR ) ? regValCoB : regTbr;

		regSPc	<= (regIdCoA==JX2_REG_SPC ) ? regValCoA : regSPc;
		regSPr	<= (regIdCoA==JX2_REG_SLR ) ? regValCoA : regSPr;
		regSSr	<= (regIdCoA==JX2_REG_SSR ) ? regValCoA : regSSr;
		regSSp	<= (regIdCoA==JX2_REG_SSP ) ? regValCoA : regSSp;

		regSGb	<= (regIdCoB==JX2_REG_SGB ) ? regValCoB : regSGb;
		regSTb	<= (regIdCoB==JX2_REG_STB ) ? regValCoB : regSTb;

		regTtb	<= (regIdCoB==JX2_REG_TTB ) ? regValCoB : regTtb;
//		regTea	<= (regIdCoB==JX2_REG_TEA ) ? regValCoB : regTea;
		regTea	<= (regIdCoB==JX2_REG_TEA ) ? regValCoB : exNextTea2;
		regMmcr	<= (regIdCoB==JX2_REG_MMCR) ? regValCoB : regMmcr;
//		regExsr	<= (regIdCoB==JX2_REG_EXSR) ? regValCoB : regExsr;
		regExsr	<= (regIdCoB==JX2_REG_EXSR) ? regValCoB : exNextExc2;
		regSttb	<= (regIdCoB==JX2_REG_STTB) ? regValCoB : regSttb;
		regKrr	<= (regIdCoB==JX2_REG_KRR ) ? regValCoB : regKrr;

	end
end

endmodule
