/*
Control Registers, 2 Write Ports
*/

module RegCR_2W(
	clock,
	reset,
	hold,

	regIdC1m,		//Source ID
	regValC1m,		//Source Value
	regIdC2m,		//Source ID
	regValC2m,		//Source Value

	regIdC1n1,		//Destination ID (EX1)
	regValC1n1,		//Destination Value (EX1)
	regIdC1n2,		//Destination ID (EX2)
	regValC1n2,		//Destination Value (EX2)
	regIdC1n3,		//Destination ID (EX3)
	regValC1n3,		//Destination Value (EX3)

	regIdC2n1,		//Destination ID (EX1)
	regValC2n1,		//Destination Value (EX1)
	regIdC2n2,		//Destination ID (EX2)
	regValC2n2,		//Destination Value (EX2)
	regIdC2n3,		//Destination ID (EX3)
	regValC2n3,		//Destination Value (EX3)

	regEx1Flush1,
	regEx2Flush1,
	regEx3Flush1,

	regEx1Flush2,
	regEx2Flush2,
	regEx3Flush2,

	gprValPc1,		//PC Value (Synthesized)
	gprValPc2,		//PC Value (Synthesized)

	regOutPc,	regInPc,
	regOutLr,	regInLr,
	regOutSr,	regInSr,

	regOutPc2,	regInPc2,
	regOutLr2,	regInLr2,
	regOutSr2,	regInSr2,

	regOutExsr,	regInExsr,
	regOutSpc,	regInSpc,
	regOutSsp,	regInSsp,
	regOutTea,	regInTea,

	regOutSpc2,	regInSpc2,
	regOutSsp2,	regInSsp2,

	regOutVbr,
	regOutGbr,
	regOutTbr,
	regOutMmcr,
	regOutKrr,

	regOutGbr2,
	regOutTbr2,
	regOutKrr2
	);

input			clock;
input			reset;
input			hold;

`input_gpr		regIdC1m;		//Source C, MemStore
output[63:0]	regValC1m;

`input_gpr		regIdC2m;		//Source C, MemStore
output[63:0]	regValC2m;

`input_gpr		regIdC1n1;		//Destination ID
input[63:0]		regValC1n1;		//Destination Value
`input_gpr		regIdC1n2;		//Destination ID
input[63:0]		regValC1n2;		//Destination Value
`input_gpr		regIdC1n3;		//Destination ID
input[63:0]		regValC1n3;		//Destination Value

`input_gpr		regIdC2n1;		//Destination ID
input[63:0]		regValC2n1;		//Destination Value
`input_gpr		regIdC2n2;		//Destination ID
input[63:0]		regValC2n2;		//Destination Value
`input_gpr		regIdC2n3;		//Destination ID
input[63:0]		regValC2n3;		//Destination Value

input			regEx1Flush1;
input			regEx2Flush1;
input			regEx3Flush1;

input			regEx1Flush2;
input			regEx2Flush2;
input			regEx3Flush2;

input [47:0]	gprValPc1;		//PC Value (Synthesized)
input [47:0]	gprValPc2;		//PC Value (Synthesized)

output[47:0]	regOutPc1;
input [47:0]	regInPc1;
output[63:0]	regOutLr1;
input [63:0]	regInLr1;
output[63:0]	regOutSr1;
input [63:0]	regInSr1;

output[47:0]	regOutPc2;
input [47:0]	regInPc2;
output[63:0]	regOutLr2;
input [63:0]	regInLr2;
output[63:0]	regOutSr2;
input [63:0]	regInSr2;

output[63:0]	regOutExsr;
input [63:0]	regInExsr;
output[47:0]	regOutSpc;
input [47:0]	regInSpc;
output[47:0]	regOutSsp;
input [47:0]	regInSsp;
output[63:0]	regOutTea;
input [63:0]	regInTea;

output[47:0]	regOutSpc2;
input [47:0]	regInSpc2;
output[47:0]	regOutSsp2;
input [47:0]	regInSsp2;

output[47:0]	regOutVbr;
output[47:0]	regOutGbr;
output[47:0]	regOutTbr;
output[63:0]	regOutMmcr;
output[63:0]	regOutKrr;

output[47:0]	regOutGbr2;
output[47:0]	regOutTbr2;
output[63:0]	regOutKrr2;

reg[63:0]	crRegSr;
reg[63:0]	crRegExsr;

reg[47:0]	crRegPc;
reg[63:0]	crRegLr;
reg[47:0]	crRegVbr;
reg[47:0]	crRegSpc;
reg[47:0]	crRegSsp;
reg[47:0]	crRegGbr;
reg[47:0]	crRegTbr;
reg[63:0]	crRegTea;

reg[63:0]	crRegSr2;
reg[47:0]	crRegPc2;
reg[63:0]	crRegLr2;
reg[47:0]	crRegSpc2;
reg[47:0]	crRegSsp2;
reg[47:0]	crRegGbr2;
reg[47:0]	crRegTbr2;

`ifdef jx2_enable_mmu
reg[47:0]	crRegTtb;
reg[63:0]	crRegMmcr;
reg[47:0]	crRegSttb;
reg[63:0]	crRegKrr;
reg[63:0]	crRegKrr2;
`endif

reg[63:0]	tRegValC1m;
reg[63:0]	tRegValC2m;
assign	regValC1m = tRegValC1m;
assign	regValC2m = tRegValC2m;

assign	regOutPc	= crRegPc;
assign	regOutLr	= crRegLr;
assign	regOutSr	= crRegSr;
assign	regOutExsr	= crRegExsr;
assign	regOutSpc	= crRegSpc;
assign	regOutSsp	= crRegSsp;

assign	regOutVbr	= crRegVbr;
assign	regOutGbr	= crRegGbr;
assign	regOutTbr	= crRegTbr;
assign	regOutTea	= crRegTea;



assign	regOutPc2	= crRegPc2;
assign	regOutLr2	= crRegLr2;
assign	regOutSr2	= crRegSr2;
assign	regOutSpc2	= crRegSpc2;
assign	regOutSsp2	= crRegSsp2;

assign	regOutGbr2	= crRegGbr2;
assign	regOutTbr2	= crRegTbr2;



`ifdef jx2_enable_mmu
assign	regOutMmcr	= crRegMmcr;
assign	regOutKrr	= crRegKrr;
assign	regOutKrr2	= crRegKrr2;
`else
assign	regOutMmcr	= UV64_00;
assign	regOutKrr	= UV64_00;
assign	regOutKrr2	= UV64_00;
`endif

reg[63:0]	tValCmA;
reg			tValCmZz;

reg			tIsIsrEdge;

`reg_gpr		regIdCn2B;			//Destination ID
reg[63:0]		regValCn2B;			//Destination Value
reg[47:0]		regValCn2B_48b;		//Destination Value

reg[63:0]		tRegInLr;


always @*
begin

	tValCmZz=0;

	tRegInLr	= regInLr;

// `ifdef def_true
`ifndef def_true
//	if((regValCn3[31:0]==32'h55BAADAA) && !regEx3Flush)
	if(regValCn3[31:0]==32'h55BAADAA)
	begin
		$display("RegCR: EX3 Bad, Not Flushed");
	end
//	if((regValCn2[31:0]==32'h55BAADAA) && !regEx2Flush)
	if(regValCn2[31:0]==32'h55BAADAA)
	begin
		$display("RegCR: EX2 Bad, Not Flushed");
	end
//	if((regValCn1[31:0]==32'h55BAADAA) && !regEx1Flush)
	if(regValCn1[31:0]==32'h55BAADAA)
	begin
		$display("RegCR: EX1 Bad, Not Flushed");
	end
`endif

	tIsIsrEdge	= regInSr[28] ^ crRegSr[28];


//	regIdCn2B	= { 1'b1, regIdCn3 };
	regIdCn2B	= regIdCn3;
	regValCn2B	= regValCn3;
//	if(regEx3Flush)
//	if(1'b0)
//	begin
//		regIdCn2B	= JX2_CR_ZZR;
//	end

`ifdef jx2_enable_vaddr48
//	regValCn2B_48b	= regValCn2[47:0];
	regValCn2B_48b	= regValCn2B[47:0];
`else
//	regValCn2B_48b	= { UV16_00, regValCn2[31:0] };
	regValCn2B_48b	= { UV16_00, regValCn2B[31:0] };
`endif

//	case({1'b1, regIdCm})
	case(regIdCm)
		JX2_CR_SR:		tValCmA=crRegSr;
		JX2_CR_EXSR:	tValCmA=crRegExsr;

`ifdef jx2_enable_vaddr48
//		JX2_CR_PC:		tValCmA={UV16_00, crRegPc};
		JX2_CR_PC:		tValCmA={UV16_00, gprValPc};
//		JX2_CR_LR:		tValCmA={UV16_00, crRegLr};
		JX2_CR_LR:		tValCmA=crRegLr;
		JX2_CR_VBR:		tValCmA={UV16_00, crRegVbr};
		JX2_CR_SPC:		tValCmA={UV16_00, crRegSpc};
		JX2_CR_SSP:		tValCmA={UV16_00, crRegSsp};
		JX2_CR_GBR:		tValCmA={UV16_00, crRegGbr};
		JX2_CR_TBR:		tValCmA={UV16_00, crRegTbr};
//		JX2_CR_TEA:		tValCmA={UV16_00, crRegTea};
		JX2_CR_TEA:		tValCmA=crRegTea;
`else
//		JX2_CR_PC:		tValCmA={UV32_00, crRegPc[31:0]};
		JX2_CR_PC:		tValCmA={UV32_00, gprValPc[31:0]};
		JX2_CR_LR:		tValCmA={UV32_00, crRegLr[31:0]};
		JX2_CR_VBR:		tValCmA={UV32_00, crRegVbr[31:0]};
		JX2_CR_SPC:		tValCmA={UV32_00, crRegSpc[31:0]};
		JX2_CR_SSP:		tValCmA={UV32_00, crRegSsp[31:0]};
		JX2_CR_GBR:		tValCmA={UV32_00, crRegGbr[31:0]};
		JX2_CR_TBR:		tValCmA={UV32_00, crRegTbr[31:0]};
//		JX2_CR_TEA:		tValCmA={UV32_00, crRegTea[31:0]};
		JX2_CR_TEA:		tValCmA=crRegTea;
`endif

`ifdef jx2_enable_mmu
//		JX2_CR_TTB:		tValCmA={UV32_00, crRegTtb[31:0]};
		JX2_CR_TTB:		tValCmA={UV16_00, crRegTtb[47:0]};
		JX2_CR_MMCR:	tValCmA=crRegMmcr;
//		JX2_CR_STTB:	tValCmA={UV32_00, crRegSttb[31:0]};
		JX2_CR_STTB:	tValCmA={UV16_00, crRegSttb[47:0]};
		JX2_CR_KRR:		tValCmA=crRegKrr;
`else
		JX2_CR_TTB:		tValCmA=UV64_00;
		JX2_CR_MMCR:	tValCmA=UV64_00;
		JX2_CR_STTB:	tValCmA=UV64_00;
		JX2_CR_KRR:		tValCmA=UV64_00;
`endif
		
		JX2_CR_IMM:	begin
			tValCmA=UV64_00;
			tValCmZz=1;
		end

		JX2_CR_ZZR:	begin
			tValCmA=UV64_00;
			tValCmZz=1;
		end
		
		default: begin
//			$display("Reg CR: Unhandled %X", regIdCm);
			tValCmA=UV64_00;
		end
	endcase
	
	tRegValCm = tValCmA;

`ifndef def_true
// `ifdef def_true
	if(!tValCmZz)
	begin
		if(regIdCm==regIdCn3)
			tRegValCm=regValCn3;
		if(regIdCm==regIdCn2)
			tRegValCm=regValCn2;
		if(regIdCm==regIdCn1)
			tRegValCm=regValCn1;
	end
`endif
end

always @(posedge clock)
begin
	if(reset)
	begin
		crRegPc		<= UV48_00;
//		crRegLr		<= UV48_00;
//		crRegVbr	<= UV48_00;
//		crRegGbr	<= UV48_00;
//		crRegTbr	<= UV48_00;

		crRegSr		<= UV64_00;
		crRegSr[30]	<= 1;

`ifdef jx2_enable_mmu
		crRegMmcr	<= UV64_00;
		crRegKrr	<= UV64_00;

//		crRegTtb	<= UV48_00;
//		crRegSttb	<= UV48_00;
`endif

	end
	else
		if(!hold)
//		if(!hold || tIsIsrEdge)
	begin
		crRegSr		<= (regIdCn2B==JX2_CR_SR  ) ? regValCn2B[63:0] : regInSr;
		crRegExsr	<= (regIdCn2B==JX2_CR_EXSR) ? regValCn2B[63:0] : regInExsr;

`ifdef jx2_enable_vaddr48
		crRegPc		<= regInPc;
`else
		crRegPc		<= { UV16_00, regInPc[31:0] };
`endif

`ifndef def_true
		crRegPc		<= regInPc;
//		crRegLr		<= (regIdCn2B==JX2_CR_LR  ) ? regValCn2B[47:0] : regInLr;
		crRegLr		<= (regIdCn2B==JX2_CR_LR  ) ? regValCn2B[63:0] : regInLr;
		crRegSpc	<= (regIdCn2B==JX2_CR_SPC ) ? regValCn2B[47:0] : regInSpc;
		crRegSsp	<= (regIdCn2B==JX2_CR_SSP ) ? regValCn2B[47:0] : regInSsp;
//		crRegTea	<= (regIdCn2B==JX2_CR_TEA ) ? regValCn2B[47:0] : regInTea;
		crRegTea	<= (regIdCn2B==JX2_CR_TEA ) ? regValCn2B[63:0] : regInTea;
		crRegVbr	<= (regIdCn2B==JX2_CR_VBR ) ? regValCn2B[47:0] : crRegVbr;
		crRegGbr	<= (regIdCn2B==JX2_CR_GBR ) ? regValCn2B[47:0] : crRegGbr;
		crRegTbr	<= (regIdCn2B==JX2_CR_TBR ) ? regValCn2B[47:0] : crRegTbr;
`endif

`ifdef def_true
//		crRegLr		<= (regIdCn2B==JX2_CR_LR  ) ? regValCn2B_48b : regInLr;
//		crRegLr		<= (regIdCn2B==JX2_CR_LR  ) ? regValCn2B_48b : tRegInLr;
		crRegLr		<= (regIdCn2B==JX2_CR_LR  ) ? regValCn2B     : tRegInLr;
		crRegSpc	<= (regIdCn2B==JX2_CR_SPC ) ? regValCn2B_48b : regInSpc;
		crRegSsp	<= (regIdCn2B==JX2_CR_SSP ) ? regValCn2B_48b : regInSsp;
//		crRegTea	<= (regIdCn2B==JX2_CR_TEA ) ? regValCn2B_48b : regInTea;
		crRegTea	<= (regIdCn2B==JX2_CR_TEA ) ? regValCn2B     : regInTea;
		crRegVbr	<= (regIdCn2B==JX2_CR_VBR ) ? regValCn2B_48b : crRegVbr;
		crRegGbr	<= (regIdCn2B==JX2_CR_GBR ) ? regValCn2B_48b : crRegGbr;
		crRegTbr	<= (regIdCn2B==JX2_CR_TBR ) ? regValCn2B_48b : crRegTbr;
`endif

`ifdef jx2_enable_mmu
		crRegTtb	<= (regIdCn2B==JX2_CR_TTB ) ? regValCn2B[47:0] : crRegTtb;
		crRegMmcr	<= (regIdCn2B==JX2_CR_MMCR) ? regValCn2B[63:0] : crRegMmcr;
		crRegSttb	<= (regIdCn2B==JX2_CR_STTB) ? regValCn2B[47:0] : crRegSttb;
		crRegKrr	<= (regIdCn2B==JX2_CR_KRR ) ? regValCn2B[63:0] : crRegKrr;
`endif

	end
end

endmodule
