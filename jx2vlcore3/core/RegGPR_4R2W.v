/*
GPR Module with 4 read ports and 2 write ports.
 */
 
`include "RegSpr_2W.v"

module RegGPR_4R2W(
	clock,
	reset,
	hold,

	regIdRs,		//Source A, ALU / Base (Lane 1)
	regIdRt,		//Source B, ALU / Index (Lane 1)
	regIdRu,		//Source C, ALU / Base (Lane 2)
	regIdRv,		//Source D, ALU / Index (Lane 2) / MemStore
	regValRs,		//Source A Value
	regValRt,		//Source B Value
	regValRu,		//Source C Value
	regValRv,		//Source D Value

	regIdRnA1,		//Destination ID (EX1, Lane 1)
	regValRnA1,		//Destination Value (EX1, Lane 1)
	regIdRnA2,		//Destination ID (EX2, Lane 1)
	regValRnA2,		//Destination Value (EX2, Lane 1)
	
	regIdRnB1,		//Destination ID (EX1, Lane 2)
	regValRnB1,		//Destination Value (EX1, Lane 2)
	regIdRnB2,		//Destination ID (EX2, Lane 2)
	regValRnB2,		//Destination Value (EX2, Lane 2)
	
	regValPc,		//PC Value (Synthesized)
	regValGbr,		//GBR Value (CR)
	regValImmA,		//Immediate (Decode, Lane 1)
	regValImmB,		//Immediate (Decode, Lane 2)
	regValLr,		//LR Value (CR)
	
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

input[5:0]		regIdRs;		//Source A
input[5:0]		regIdRt;		//Source B
input[5:0]		regIdRu;		//Source C
input[5:0]		regIdRv;		//Source D

output[63:0]	regValRs;
output[63:0]	regValRt;
output[63:0]	regValRu;
output[63:0]	regValRv;

input[5:0]		regIdRnA1;		//Destination ID
input[63:0]		regValRnA1;		//Destination Value
input[5:0]		regIdRnA2;		//Destination ID
input[63:0]		regValRnA2;		//Destination Value

input[5:0]		regIdRnB1;		//Destination ID
input[63:0]		regValRnB1;		//Destination Value
input[5:0]		regIdRnB2;		//Destination ID
input[63:0]		regValRnB2;		//Destination Value

input [31:0]	regValPc;		//PC Value (Synthesized)
input [31:0]	regValGbr;		//GBR Value (CR)
input [32:0]	regValImmA;		//Immediate (Decode)
input [32:0]	regValImmB;		//Immediate (Decode)
input [31:0]	regValLr;		//GBR Value (CR)
	
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
reg[63:0]	tRegValRu;
reg[63:0]	tRegValRv;

assign	regValRs = tRegValRs;
assign	regValRt = tRegValRt;
assign	regValRu = tRegValRu;
assign	regValRv = tRegValRv;


reg[63:0]	gprArrA[31:0];
reg[63:0]	gprArrB[31:0];
reg[31:0]	gprArrM;

// `ifdef def_true
`ifndef def_true
reg[63:0]	gprRegDlr;
reg[63:0]	gprRegDhr;
reg[63:0]	gprRegElr;
reg[63:0]	gprRegEhr;
reg[63:0]	gprRegSp;
reg[63:0]	gprRegBp;
`endif

// `ifndef def_true
`ifdef def_true
wire[63:0]	gprRegDlr;
wire[63:0]	gprRegDhr;
wire[63:0]	gprRegSp;

RegSpr_2W	gprModDlr(
	clock,		reset,
	JX2_GR_DLR,	gprRegDlr,
	regIdRnA2,	regValRnA2,
	regIdRnB2,	regValRnB2,
	regInDlr,	hold);

RegSpr_2W	gprModDhr(
	clock,		reset,
	JX2_GR_DHR,	gprRegDhr,
	regIdRnA2,	regValRnA2,
	regIdRnB2,	regValRnB2,
	regInDhr,	hold);

RegSpr_2W	gprModSp(
	clock,		reset,
	JX2_GR_SP,	gprRegSp,
	regIdRnA2,	regValRnA2,
	regIdRnB2,	regValRnB2,
	regInSp,	hold);

`ifdef jx2_sprs_elrehr
wire[63:0]	gprRegElr;
wire[63:0]	gprRegEhr;
wire[63:0]	gprRegBp;

RegSpr_2W	gprModElr(
	clock,		reset,
	JX2_GR_ELR,	gprRegElr,
	regIdRnA2,	regValRnA2,
	regIdRnB2,	regValRnB2,
	regInElr,	hold);

RegSpr_2W	gprModEhr(
	clock,		reset,
	JX2_GR_EHR,	gprRegEhr,
	regIdRnA2,	regValRnA2,
	regIdRnB2,	regValRnB2,
	regInEhr,	hold);

RegSpr_2W	gprModBp(
	clock,		reset,
	JX2_GR_BP,	gprRegBp,
	regIdRnA2,	regValRnA2,
	regIdRnB2,	regValRnB2,
	regInBp,	hold);
`endif

`endif	


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
reg[63:0]	tValRuA;
reg[63:0]	tValRvA;

reg	tValRsZz;
reg	tValRtZz;
reg	tValRuZz;
reg	tValRvZz;

always @*
begin
	tValRsZz=0;
	tValRtZz=0;
	tValRuZz=0;
	tValRvZz=0;

	casez(regIdRs)
//		6'b0zzzzz:	tValRsA=gprArr[regIdRs[4:0]];
		6'b0zzzzz:
			tValRsA=gprArrM[regIdRs[4:0]] ?
				gprArrB[regIdRs[4:0]] :
				gprArrA[regIdRs[4:0]];
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

		JX2_GR_PC:	tValRsA={ UV32_00, regValPc };
		JX2_GR_GBR:	tValRsA={ UV32_00, regValGbr };
		JX2_GR_LR:	tValRsA={ UV32_00, regValLr };

		JX2_GR_IMM: begin
			tValRsA={
				regValImmA[32]?UV32_FF:UV32_00,
				regValImmA[31:0] };
			tValRsZz=1;
		end
		JX2_GR_ZZR:	begin
			tValRsA=UV64_00;
			tValRsZz=1;
		end

		default: 	tValRsA=UV64_XX;
	endcase

	casez(regIdRt)
//		6'b0zzzzz:	tValRtA=gprArr[regIdRt[4:0]];
		6'b0zzzzz:
			tValRtA=gprArrM[regIdRt[4:0]] ?
				gprArrB[regIdRt[4:0]] :
				gprArrA[regIdRt[4:0]];
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
				regValImmA[32]?UV32_FF:UV32_00,
				regValImmA[31:0] };
			tValRtZz=1;
		end
		JX2_GR_ZZR:	begin
			tValRtA=UV64_00;
			tValRtZz=1;
		end

		default: 	tValRtA=UV64_XX;
	endcase

	casez(regIdRu)
//		6'b0zzzzz:	tValRuA=gprArr[regIdRu[4:0]];
		6'b0zzzzz:
			tValRuA=gprArrM[regIdRu[4:0]] ?
				gprArrB[regIdRu[4:0]] :
				gprArrA[regIdRu[4:0]];
		JX2_GR_DLR:	tValRuA=gprRegDlr;
//		JX2_GR_DLR:	tValRuA=regInDlr;
		JX2_GR_DHR:	tValRuA=gprRegDhr;
//		JX2_GR_DHR:	tValRuA=regInDhr;
		JX2_GR_SP:	tValRuA=gprRegSp;
//		JX2_GR_SP:	tValRuA=regInSp;

`ifdef jx2_sprs_elrehr
		JX2_GR_ELR:	tValRuA=gprRegElr;
		JX2_GR_EHR:	tValRuA=gprRegEhr;
		JX2_GR_BP:	tValRuA=gprRegBp;
`endif

		JX2_GR_IMM:begin
			tValRuA={
				regValImmB[32]?UV32_FF:UV32_00,
				regValImmB[31:0] };
			tValRuZz=1;
		end
		JX2_GR_ZZR:	begin
			tValRuA=UV64_00;
			tValRuZz=1;
		end

		default: 	tValRuA=UV64_XX;
	endcase

	casez(regIdRv)
//		6'b0zzzzz:	tValRvA=gprArr[regIdRv[4:0]];
		6'b0zzzzz:
			tValRvA=gprArrM[regIdRv[4:0]] ?
				gprArrB[regIdRv[4:0]] :
				gprArrA[regIdRv[4:0]];
		JX2_GR_DLR:	tValRvA=gprRegDlr;
//		JX2_GR_DLR:	tValRvA=regInDlr;
		JX2_GR_DHR:	tValRvA=gprRegDhr;
//		JX2_GR_DHR:	tValRvA=regInDhr;
		JX2_GR_SP:	tValRvA=gprRegSp;
//		JX2_GR_SP:	tValRvA=regInSp;

`ifdef jx2_sprs_elrehr
		JX2_GR_ELR:	tValRvA=gprRegElr;
		JX2_GR_EHR:	tValRvA=gprRegEhr;
		JX2_GR_BP:	tValRvA=gprRegBp;
`endif

		JX2_GR_IMM:begin
			tValRvA={
				regValImmB[32]?UV32_FF:UV32_00,
				regValImmB[31:0] };
			tValRvZz=1;
		end
		JX2_GR_ZZR:	begin
			tValRvA=UV64_00;
			tValRvZz=1;
		end

		default: 	tValRvA=UV64_XX;
	endcase

	tRegValRs = tValRsA;
	tRegValRt = tValRtA;
	tRegValRu = tValRuA;
	tRegValRv = tValRvA;
	
	if(!tValRsZz)
	begin
		if(regIdRs==regIdRnB2)
			tRegValRs=regValRnB2;
		if(regIdRs==regIdRnA2)
			tRegValRs=regValRnA2;

		if(regIdRs==regIdRnB1)
			tRegValRs=regValRnB1;
		if(regIdRs==regIdRnA1)
			tRegValRs=regValRnA1;
	end

	if(!tValRtZz)
	begin
		if(regIdRt==regIdRnB2)
			tRegValRt=regValRnB2;
		if(regIdRt==regIdRnA2)
			tRegValRt=regValRnA2;

		if(regIdRt==regIdRnB1)
			tRegValRt=regValRnB1;
		if(regIdRt==regIdRnA1)
			tRegValRt=regValRnA1;
	end

	if(!tValRuZz)
	begin
		if(regIdRu==regIdRnB2)
			tRegValRu=regValRnB2;
		if(regIdRu==regIdRnA2)
			tRegValRu=regValRnA2;

		if(regIdRu==regIdRnB1)
			tRegValRu=regValRnB1;
		if(regIdRu==regIdRnA1)
			tRegValRu=regValRnA1;
	end

	if(!tValRvZz)
	begin
		if(regIdRv==regIdRnB2)
			tRegValRv=regValRnB2;
		if(regIdRv==regIdRnA2)
			tRegValRv=regValRnA2;

		if(regIdRv==regIdRnB1)
			tRegValRv=regValRnB1;
		if(regIdRv==regIdRnA1)
			tRegValRv=regValRnA1;
	end
end

always @(posedge clock)
begin
	if(!hold)
	begin

// `ifdef def_true
`ifndef def_true
		gprRegDlr	<= (regIdRnA2==JX2_GR_DLR) ? regValRnA2 : regInDlr;
		gprRegDhr	<= (regIdRnA2==JX2_GR_DHR) ? regValRnA2 : regInDhr;
		gprRegSp	<= (regIdRnA2==JX2_GR_SP ) ? regValRnA2 : regInSp;

`ifdef jx2_sprs_elrehr
		gprRegElr	<= (regIdRnA2==JX2_GR_ELR) ? regValRnA2 : regInElr;
		gprRegEhr	<= (regIdRnA2==JX2_GR_EHR) ? regValRnA2 : regInEhr;
		gprRegBp	<= (regIdRnA2==JX2_GR_BP ) ? regValRnA2 : regInBp;
`endif

`endif

		if(!regIdRnA2[5])
		begin
			gprArrA[regIdRnA2[4:0]] <= regValRnA2;
			gprArrM[regIdRnA2[4:0]]	<= 1'b0;
		end

		if(!regIdRnB2[5])
		begin
			gprArrB[regIdRnB2[4:0]] <= regValRnB2;
			gprArrM[regIdRnB2[4:0]]	<= 1'b1;
		end
	end
end

endmodule
