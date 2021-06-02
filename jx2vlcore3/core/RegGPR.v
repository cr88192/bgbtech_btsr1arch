module RegGPR(
	clock,
	reset,
	hold,

	regIdUCmd,
	regIdUIxt,

	regIdRs,		//Source A, ALU / Base
	regIdRt,		//Source B, ALU / Index
	regIdRm,		//Source C, MemStore
	regValRs,		//Source A Value
	regValRt,		//Source B Value
	regValRm,		//Source C Value

	regIdRn1,		//Destination ID (EX1)
	regValRn1,		//Destination Value (EX1)
	regIdRn2,		//Destination ID (EX2)
	regValRn2,		//Destination Value (EX2)
	regIdRn3,		//Destination ID (EX3)
	regValRn3,		//Destination Value (EX3)
	
	regValPc,		//PC Value (Synthesized)
	regValGbr,		//GBR Value (CR)
	regValImm,		//Immediate (Decode)
	regValImmB,		//Immediate (Decode)
	regValLr,		//LR Value (CR)
	regValCm,		//Cm Value (CR)
	
	regOutDlr,	regInDlr,
	regOutDhr,	regInDhr,
`ifdef jx2_sprs_elrehr
	regOutElr,	regInElr,
	regOutEhr,	regInEhr,
	regOutBp,	regInBp,
`endif
	regOutSp,	regInSp
	);


input			clock;
input			reset;
input			hold;

input[7:0]		regIdUCmd;
input[7:0]		regIdUIxt;

input[5:0]		regIdRs;		//Source A, ALU / Base
input[5:0]		regIdRt;		//Source B, ALU / Index
input[5:0]		regIdRm;		//Source C, MemStore

output[63:0]	regValRs;
output[63:0]	regValRt;
output[63:0]	regValRm;

input[5:0]		regIdRn1;		//Destination ID
input[63:0]		regValRn1;		//Destination Value
input[5:0]		regIdRn2;		//Destination ID
input[63:0]		regValRn2;		//Destination Value
input[5:0]		regIdRn3;		//Destination ID
input[63:0]		regValRn3;		//Destination Value

input [47:0]	regValPc;		//PC Value (Synthesized)
input [47:0]	regValGbr;		//GBR Value (CR)
input [32:0]	regValImm;		//Immediate (Decode)
input [32:0]	regValImmB;		//Immediate (Decode)
input [63:0]	regValLr;		//LR Value (CR)
input [63:0]	regValCm;		//Cm Value (CR)
	
output[63:0]	regOutDlr;
input [63:0]	regInDlr;
output[63:0]	regOutDhr;
input [63:0]	regInDhr;
output[63:0]	regOutSp;
input [63:0]	regInSp;

`ifdef jx2_sprs_elrehr
output[63:0]	regOutElr;
input [63:0]	regInElr;
output[63:0]	regOutEhr;
input [63:0]	regInEhr;
output[63:0]	regOutBp;
input [63:0]	regInBp;
`endif

reg[63:0]	tRegValRs;
reg[63:0]	tRegValRt;
reg[63:0]	tRegValRm;

assign	regValRs = tRegValRs;
assign	regValRt = tRegValRt;
assign	regValRm = tRegValRm;


reg[63:0]	gprArr[31:0];

reg[63:0]	gprRegDlr;
reg[63:0]	gprRegDhr;
reg[63:0]	gprRegElr;
reg[63:0]	gprRegEhr;

reg[63:0]	gprRegSp;
reg[63:0]	gprRegBp;


assign	regOutDlr = gprRegDlr;
assign	regOutDhr = gprRegDhr;
assign	regOutSp  = gprRegSp;

`ifdef jx2_sprs_elrehr
assign	regOutElr = gprRegElr;
assign	regOutEhr = gprRegEhr;
assign	regOutBp  = gprRegBp;
`endif

reg[63:0]	tValRsA;
reg[63:0]	tValRtA;
reg[63:0]	tValRmA;

reg	tValRsZz;
reg	tValRtZz;
reg	tValRmZz;

reg[63:0]	tValJimm;

wire	tEnablePcLsb;
assign	tEnablePcLsb = (regIdUIxt[2:0] == JX2_BTY_SB);


always @*
begin
	tValRsZz=0;
	tValRtZz=0;
	tValRmZz=0;

`ifdef jx2_enable_wexjumbo
	tValJimm={
		regValImmB[31:0],
		regValImm [31:0] };
`endif

	casez(regIdRs)
		6'b0zzzzz:	tValRsA=gprArr[regIdRs[4:0]];
		JX2_GR_DLR:	tValRsA=gprRegDlr;
//		JX2_GR_DLR:	tValRsA=regInDlr;
		JX2_GR_DHR:	tValRsA=gprRegDhr;
//		JX2_GR_DHR:	tValRsA=regInDhr;
		JX2_GR_SP:	tValRsA=gprRegSp;
//		JX2_GR_SP:	tValRsA=regInSp;

`ifdef jx2_sprs_elrehr
		JX2_GR_ELR:	tValRsA=gprRegElr;
		JX2_GR_EHR:	tValRsA=gprRegEhr;
		JX2_GR_BP:	tValRsA=gprRegBp;
`endif

`ifdef jx2_enable_vaddr48
//		JX2_GR_PC:	tValRsA={ UV16_00, regValPc };
		JX2_GR_PC:	tValRsA={ UV16_00, regValPc[47:1],
			regValPc[0] &tEnablePcLsb };
		JX2_GR_GBR:	tValRsA={ UV16_00, regValGbr };
//		JX2_GR_LR:	tValRsA={ UV16_00, regValLr };
		JX2_GR_LR:	tValRsA= regValLr;
`else
//		JX2_GR_PC:	tValRsA={ UV32_00, regValPc[31:0] };
		JX2_GR_PC:	tValRsA={ UV32_00, regValPc[31:1],
			regValPc[0] &tEnablePcLsb };
		JX2_GR_GBR:	tValRsA={ UV32_00, regValGbr[31:0] };
		JX2_GR_LR:	tValRsA={ UV32_00, regValLr[31:0] };
`endif

`ifdef jx2_gprs_mergecm
		JX2_GR_SR, JX2_GR_VBR, JX2_GR_SPC, JX2_GR_SSP,
		JX2_GR_TBR, JX2_GR_TTB, JX2_GR_TEA, JX2_GR_MMCR,
		JX2_GR_EXSR, JX2_GR_STTB, JX2_GR_KRR:
			tValRsA = regValCm;
`endif

		JX2_GR_IMM: begin
			tValRsA={
				regValImm[32]?UV32_FF:UV32_00,
				regValImm[31:0] };
			tValRsZz=1;
		end

`ifdef jx2_enable_wexjumbo
		JX2_GR_JIMM: begin
			tValRsA=tValJimm;
			tValRsZz=1;
		end
`endif

		JX2_GR_ZZR:	begin
			tValRsA=UV64_00;
			tValRsZz=1;
		end

		default: 	tValRsA=UV64_XX;
	endcase

	casez(regIdRt)
		6'b0zzzzz:	tValRtA=gprArr[regIdRt[4:0]];
		JX2_GR_DLR:	tValRtA=gprRegDlr;
//		JX2_GR_DLR:	tValRtA=regInDlr;
		JX2_GR_DHR:	tValRtA=gprRegDhr;
//		JX2_GR_DHR:	tValRtA=regInDhr;
		JX2_GR_SP:	tValRtA=gprRegSp;
//		JX2_GR_SP:	tValRtA=regInSp;

`ifdef jx2_sprs_elrehr
		JX2_GR_ELR:	tValRtA=gprRegElr;
		JX2_GR_EHR:	tValRtA=gprRegEhr;
		JX2_GR_BP:	tValRtA=gprRegBp;
`endif

		JX2_GR_IMM:	begin
			tValRtA={
				regValImm[32]?UV32_FF:UV32_00,
				regValImm[31:0] };
			tValRtZz=1;
		end

`ifdef jx2_enable_wexjumbo
		JX2_GR_JIMM: begin
			tValRtA=tValJimm;
			tValRsZz=1;
		end
`endif

		JX2_GR_ZZR:	begin
			tValRtA=UV64_00;
			tValRtZz=1;
		end

		default: 	tValRtA=UV64_XX;
	endcase

	casez(regIdRm)
		6'b0zzzzz:	tValRmA=gprArr[regIdRm[4:0]];
		JX2_GR_DLR:	tValRmA=gprRegDlr;
//		JX2_GR_DLR:	tValRmA=regInDlr;
		JX2_GR_DHR:	tValRmA=gprRegDhr;
//		JX2_GR_DHR:	tValRmA=regInDhr;
		JX2_GR_SP:	tValRmA=gprRegSp;
//		JX2_GR_SP:	tValRmA=regInSp;

`ifdef jx2_sprs_elrehr
		JX2_GR_ELR:	tValRmA=gprRegElr;
		JX2_GR_EHR:	tValRmA=gprRegEhr;
		JX2_GR_BP:	tValRmA=gprRegBp;
`endif

		JX2_GR_IMM:begin
			tValRmA={
				regValImm[32]?UV32_FF:UV32_00,
				regValImm[31:0] };
			tValRmZz=1;
		end
		JX2_GR_ZZR:	begin
			tValRmA=UV64_00;
			tValRmZz=1;
		end

		default: 	tValRmA=UV64_XX;
	endcase

	tRegValRs = tValRsA;
	tRegValRt = tValRtA;
	tRegValRm = tValRmA;
	
	if(!tValRsZz)
	begin
		if(regIdRs==regIdRn3)
			tRegValRs=regValRn3;
		if(regIdRs==regIdRn2)
			tRegValRs=regValRn2;
		if(regIdRs==regIdRn1)
			tRegValRs=regValRn1;
	end

	if(!tValRtZz)
	begin
		if(regIdRt==regIdRn3)
			tRegValRt=regValRn3;
		if(regIdRt==regIdRn2)
			tRegValRt=regValRn2;
		if(regIdRt==regIdRn1)
			tRegValRt=regValRn1;
	end

	if(!tValRmZz)
	begin
		if(regIdRm==regIdRn3)
			tRegValRm=regValRn3;
		if(regIdRm==regIdRn2)
			tRegValRm=regValRn2;
		if(regIdRm==regIdRn1)
			tRegValRm=regValRn1;
	end
end

always @(posedge clock)
begin
	if(!hold)
	begin
		gprRegDlr	<= (regIdRn3==JX2_GR_DLR) ? regValRn3 : regInDlr;
		gprRegDhr	<= (regIdRn3==JX2_GR_DHR) ? regValRn3 : regInDhr;
		gprRegSp	<= (regIdRn3==JX2_GR_SP ) ? regValRn3 : regInSp;

`ifdef jx2_sprs_elrehr
		gprRegElr	<= (regIdRn2==JX2_GR_ELR) ? regValRn2 : regInElr;
		gprRegEhr	<= (regIdRn2==JX2_GR_EHR) ? regValRn2 : regInEhr;
		gprRegBp	<= (regIdRn2==JX2_GR_BP ) ? regValRn2 : regInBp;
`endif

		if(!regIdRn3[5])
		begin
			gprArr[regIdRn3[4:0]] <= regValRn3;
		end
	end
end

endmodule
