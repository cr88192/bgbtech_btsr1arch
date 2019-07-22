module RegCR(
	clock,
	reset,
	hold,

	regIdCm,		//Source ID
	regValCm,		//Source Value

	regIdCn1,		//Destination ID (EX1)
	regValCn1,		//Destination Value (EX1)
	regIdCn2,		//Destination ID (EX2)
	regValCn2,		//Destination Value (EX2)

	regOutPc,	regInPc,
	regOutLr,	regInLr,
	regOutSr,	regInSr,

	regOutVbr,
	regOutGbr,
	regOutTbr,
	regOutMmcr,
	regOutKrr
	);

input			clock;
input			reset;
input			hold;

input[4:0]		regIdCm;		//Source C, MemStore
output[63:0]	regValCm;

input[4:0]		regIdCn1;		//Destination ID
input[63:0]		regValCn1;		//Destination Value
input[4:0]		regIdCn2;		//Destination ID
input[63:0]		regValCn2;		//Destination Value


output[31:0]	regOutPc;
input [31:0]	regInPc;
output[31:0]	regOutLr;
input [31:0]	regInLr;

output[63:0]	regOutSr;
input [63:0]	regInSr;

output[31:0]	regOutVbr;
output[31:0]	regOutGbr;
output[31:0]	regOutTbr;
output[63:0]	regOutMmcr;
output[63:0]	regOutKrr;


reg[31:0]	crRegPc;
reg[31:0]	crRegLr;
reg[63:0]	crRegSr;
reg[31:0]	crRegVbr;
reg[31:0]	crRegSpc;
reg[31:0]	crRegSsp;
reg[31:0]	crRegGbr;
reg[31:0]	crRegTbr;
reg[31:0]	crRegTea;
reg[63:0]	crRegExsr;

`ifdef jx2_enable_mmu
reg[31:0]	crRegTtb;
reg[63:0]	crRegMmcr;
reg[31:0]	crRegSttb;
reg[63:0]	crRegKrr;
`endif

reg[63:0]	tRegValCm;
assign	regValCm = tRegValCm;

assign	regOutPc	= crRegPc;
assign	regOutLr	= crRegLr;
assign	regOutSr	= crRegSr;

assign	regOutVbr	= crRegVbr;
assign	regOutGbr	= crRegGbr;
assign	regOutTbr	= crRegTbr;

`ifdef jx2_enable_mmu
assign	regOutMmcr	= crRegMmcr;
assign	regOutKrr	= crRegKrr;
`else
assign	regOutMmcr	= UV64_00;
assign	regOutKrr	= UV64_00;
`endif

reg[63:0]	tValCmA;
reg			tValCmZz;

reg[5:0]		regIdCn2B;		//Destination ID

always @*
begin

	tValCmZz=0;
	regIdCn2B = { 1'b1, regIdCn2 };

	case({1'b1, regIdCm})
		JX2_CR_PC:		tValCmA={UV32_00, crRegPc};
		JX2_CR_LR:		tValCmA={UV32_00, crRegLr};
		JX2_CR_SR:		tValCmA=crRegSr;
		JX2_CR_VBR:		tValCmA={UV32_00, crRegVbr};
		JX2_CR_SPC:		tValCmA={UV32_00, crRegSpc};
		JX2_CR_SSP:		tValCmA={UV32_00, crRegSsp};
		JX2_CR_GBR:		tValCmA={UV32_00, crRegGbr};
		JX2_CR_TBR:		tValCmA={UV32_00, crRegTbr};
		JX2_CR_TEA:		tValCmA={UV32_00, crRegTea};
		JX2_CR_EXSR:	tValCmA=crRegExsr;

`ifdef jx2_enable_mmu
		JX2_CR_TTB:		tValCmA={UV32_00, crRegTtb};
		JX2_CR_MMCR:	tValCmA=crRegMmcr;
		JX2_CR_STTB:	tValCmA={UV32_00, crRegSttb};
		JX2_CR_KRR:		tValCmA=crRegKrr;
`else
		JX2_CR_TTB:		tValCmA=UV64_00;
		JX2_CR_MMCR:	tValCmA=UV64_00;
		JX2_CR_STTB:	tValCmA=UV64_00;
		JX2_CR_KRR:		tValCmA=UV64_00;
`endif
		
//		JX2_CR_IMM:	begin
//			tValCmA=UV64_00;
//			tValCmZz=1;
//		end
		JX2_CR_ZZR:	begin
			tValCmA=UV64_00;
			tValCmZz=1;
		end
		
		default:		tValCmA=UV64_XX;
	endcase
	
	tRegValCm = tValCmA;
	if(!tValCmZz)
	begin
		if(regIdCm==regIdCn2)
			tRegValCm=regValCn2;
		if(regIdCm==regIdCn1)
			tRegValCm=regValCn1;
	end
end

always @(posedge clock)
begin
	if(reset)
	begin
		crRegPc		<= UV32_00;
		crRegLr		<= UV32_00;
		crRegVbr	<= UV32_00;
		crRegGbr	<= UV32_00;
		crRegTbr	<= UV32_00;

		crRegSr		<= UV64_00;

`ifdef jx2_enable_mmu
		crRegMmcr	<= UV64_00;
		crRegKrr	<= UV64_00;
`endif

	end
	else
		if(!hold)
	begin
		crRegPc		<= regInPc;
		crRegLr		<= (regIdCn2B==JX2_CR_LR  ) ? regValCn2[31:0] : regInLr;
		crRegSr		<= (regIdCn2B==JX2_CR_SR  ) ? regValCn2[63:0] : regInSr;
		crRegVbr	<= (regIdCn2B==JX2_CR_VBR ) ? regValCn2[31:0] : crRegVbr;
		crRegSpc	<= (regIdCn2B==JX2_CR_SPC ) ? regValCn2[31:0] : crRegSpc;
		crRegSsp	<= (regIdCn2B==JX2_CR_SSP ) ? regValCn2[31:0] : crRegSsp;
		crRegGbr	<= (regIdCn2B==JX2_CR_GBR ) ? regValCn2[31:0] : crRegGbr;
		crRegTbr	<= (regIdCn2B==JX2_CR_TBR ) ? regValCn2[31:0] : crRegTbr;
		crRegTea	<= (regIdCn2B==JX2_CR_TEA ) ? regValCn2[31:0] : crRegTea;
		crRegExsr	<= (regIdCn2B==JX2_CR_EXSR) ? regValCn2[63:0] : crRegExsr;

`ifdef jx2_enable_mmu
		crRegTtb	<= (regIdCn2B==JX2_CR_TTB ) ? regValCn2[31:0] : crRegTtb;
		crRegMmcr	<= (regIdCn2B==JX2_CR_MMCR) ? regValCn2[63:0] : crRegMmcr;
		crRegSttb	<= (regIdCn2B==JX2_CR_STTB) ? regValCn2[31:0] : crRegSttb;
		crRegKrr	<= (regIdCn2B==JX2_CR_KRR ) ? regValCn2[63:0] : crRegKrr;
`endif

	end
end

endmodule
