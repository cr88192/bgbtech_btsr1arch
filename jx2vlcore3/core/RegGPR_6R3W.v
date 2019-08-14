/*
GPR Module with 4 read ports and 2 write ports.
 */
module RegGPR_4R2W(
	clock,
	reset,
	hold,

	regIdRs,		//Source A, ALU / Base (Lane 1)
	regIdRt,		//Source B, ALU / Index (Lane 1)
	regIdRu,		//Source C, ALU / Base (Lane 2)
	regIdRv,		//Source D, ALU / Index (Lane 2) / MemStore
	regIdRx,		//Source E, ALU / Base (Lane 3)
	regIdRy,		//Source F, ALU / Index (Lane 3) / MemStore
	regValRs,		//Source A Value
	regValRt,		//Source B Value
	regValRu,		//Source C Value
	regValRv,		//Source D Value
	regValRx,		//Source E Value
	regValRy,		//Source F Value

	regIdRnA1,		//Destination ID (EX1, Lane 1)
	regValRnA1,		//Destination Value (EX1, Lane 1)
	regIdRnA2,		//Destination ID (EX2, Lane 1)
	regValRnA2,		//Destination Value (EX2, Lane 1)
	
	regIdRnB1,		//Destination ID (EX1, Lane 2)
	regValRnB1,		//Destination Value (EX1, Lane 2)
	regIdRnB2,		//Destination ID (EX2, Lane 2)
	regValRnB2,		//Destination Value (EX2, Lane 2)
	
	regIdRnC1,		//Destination ID (EX1, Lane 3)
	regValRnC1,		//Destination Value (EX1, Lane 3)
	regIdRnC2,		//Destination ID (EX2, Lane 3)
	regValRnC2,		//Destination Value (EX2, Lane 3)
	
	regValPc,		//PC Value (Synthesized)
	regValGbr,		//GBR Value (CR)
	regValImmA,		//Immediate (Decode, Lane 1)
	regValImmB,		//Immediate (Decode, Lane 2)
	regValImmC,		//Immediate (Decode, Lane 3)
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
input[5:0]		regIdRx;		//Source E
input[5:0]		regIdRy;		//Source F

output[63:0]	regValRs;
output[63:0]	regValRt;
output[63:0]	regValRu;
output[63:0]	regValRv;
output[63:0]	regValRx;
output[63:0]	regValRy;

input[5:0]		regIdRnA1;		//Destination ID
input[63:0]		regValRnA1;		//Destination Value
input[5:0]		regIdRnA2;		//Destination ID
input[63:0]		regValRnA2;		//Destination Value

input[5:0]		regIdRnB1;		//Destination ID
input[63:0]		regValRnB1;		//Destination Value
input[5:0]		regIdRnB2;		//Destination ID
input[63:0]		regValRnB2;		//Destination Value

input[5:0]		regIdRnC1;		//Destination ID
input[63:0]		regValRnC1;		//Destination Value
input[5:0]		regIdRnC2;		//Destination ID
input[63:0]		regValRnC2;		//Destination Value

input [31:0]	regValPc;		//PC Value (Synthesized)
input [31:0]	regValGbr;		//GBR Value (CR)
input [32:0]	regValImmA;		//Immediate (Decode)
input [32:0]	regValImmB;		//Immediate (Decode)
input [32:0]	regValImmC;		//Immediate (Decode)
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
reg[63:0]	tRegValRx;
reg[63:0]	tRegValRy;

assign	regValRs = tRegValRs;
assign	regValRt = tRegValRt;
assign	regValRu = tRegValRu;
assign	regValRv = tRegValRv;
assign	regValRx = tRegValRx;
assign	regValRy = tRegValRy;


reg[63:0]	gprArrA[31:0];
reg[63:0]	gprArrB[31:0];
reg[63:0]	gprArrC[31:0];
reg[31:0]	gprArrMA;
reg[31:0]	gprArrMB;

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
reg[63:0]	tValRuA;
reg[63:0]	tValRvA;
reg[63:0]	tValRxA;
reg[63:0]	tValRyA;

reg	tValRsZz;
reg	tValRtZz;
reg	tValRuZz;
reg	tValRvZz;
reg	tValRxZz;
reg	tValRyZz;

always @*
begin
	tValRsZz=0;
	tValRtZz=0;
	tValRuZz=0;
	tValRvZz=0;
	tValRxZz=0;
	tValRyZz=0;

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

	casez(regIdRx)
//		6'b0zzzzz:	tValRxA=gprArr[regIdRx[4:0]];
		6'b0zzzzz:
			tValRxA=gprArrM[regIdRx[4:0]] ?
				gprArrB[regIdRx[4:0]] :
				gprArrA[regIdRx[4:0]];
		JX2_GR_DLR:	tValRxA=gprRegDlr;
//		JX2_GR_DLR:	tValRxA=regInDlr;
		JX2_GR_DHR:	tValRxA=gprRegDhr;
//		JX2_GR_DHR:	tValRxA=regInDhr;
		JX2_GR_SP:	tValRxA=gprRegSp;
//		JX2_GR_SP:	tValRxA=regInSp;

`ifdef jx2_sprs_elrehr
		JX2_GR_ELR:	tValRxA=gprRegElr;
		JX2_GR_EHR:	tValRxA=gprRegEhr;
		JX2_GR_BP:	tValRxA=gprRegBp;
`endif

		JX2_GR_IMM:begin
			tValRxA={
				regValImmB[32]?UV32_FF:UV32_00,
				regValImmB[31:0] };
			tValRxZz=1;
		end
		JX2_GR_ZZR:	begin
			tValRxA=UV64_00;
			tValRxZz=1;
		end

		default: 	tValRxA=UV64_XX;
	endcase

	casez(regIdRy)
//		6'b0zzzzz:	tValRyA=gprArr[regIdRy[4:0]];
		6'b0zzzzz:
			tValRyA=gprArrM[regIdRy[4:0]] ?
				gprArrB[regIdRy[4:0]] :
				gprArrA[regIdRy[4:0]];
		JX2_GR_DLR:	tValRyA=gprRegDlr;
//		JX2_GR_DLR:	tValRyA=regInDlr;
		JX2_GR_DHR:	tValRyA=gprRegDhr;
//		JX2_GR_DHR:	tValRyA=regInDhr;
		JX2_GR_SP:	tValRyA=gprRegSp;
//		JX2_GR_SP:	tValRyA=regInSp;

`ifdef jx2_sprs_elrehr
		JX2_GR_ELR:	tValRyA=gprRegElr;
		JX2_GR_EHR:	tValRyA=gprRegEhr;
		JX2_GR_BP:	tValRyA=gprRegBp;
`endif

		JX2_GR_IMM:begin
			tValRyA={
				regValImmB[32]?UV32_FF:UV32_00,
				regValImmB[31:0] };
			tValRyZz=1;
		end
		JX2_GR_ZZR:	begin
			tValRyA=UV64_00;
			tValRyZz=1;
		end

		default: 	tValRyA=UV64_XX;
	endcase

	tRegValRs = tValRsA;
	tRegValRt = tValRtA;
	tRegValRu = tValRuA;
	tRegValRv = tValRvA;
	tRegValRx = tValRxA;
	tRegValRy = tValRyA;
	
	if(!tValRsZz)
	begin
		if(regIdRs==regIdRnA2)
			tRegValRs=regValRnA2;
		if(regIdRs==regIdRnA1)
			tRegValRs=regValRnA1;

		if(regIdRs==regIdRnB2)
			tRegValRs=regValRnB2;
		if(regIdRs==regIdRnB1)
			tRegValRs=regValRnB1;

		if(regIdRs==regIdRnC2)
			tRegValRs=regValRnC2;
		if(regIdRs==regIdRnC1)
			tRegValRs=regValRnC1;
	end

	if(!tValRtZz)
	begin
		if(regIdRt==regIdRnA2)
			tRegValRt=regValRnA2;
		if(regIdRt==regIdRnA1)
			tRegValRt=regValRnA1;

		if(regIdRt==regIdRnB2)
			tRegValRt=regValRnB2;
		if(regIdRt==regIdRnB1)
			tRegValRt=regValRnB1;

		if(regIdRt==regIdRnC2)
			tRegValRt=regValRnC2;
		if(regIdRt==regIdRnC1)
			tRegValRt=regValRnC1;
	end

	if(!tValRuZz)
	begin
		if(regIdRu==regIdRnA2)
			tRegValRu=regValRnA2;
		if(regIdRu==regIdRnA1)
			tRegValRu=regValRnA1;

		if(regIdRu==regIdRnB2)
			tRegValRu=regValRnB2;
		if(regIdRu==regIdRnB1)
			tRegValRu=regValRnB1;

		if(regIdRu==regIdRnC2)
			tRegValRu=regValRnC2;
		if(regIdRu==regIdRnC1)
			tRegValRu=regValRnC1;
	end

	if(!tValRvZz)
	begin
		if(regIdRv==regIdRnA2)
			tRegValRv=regValRnA2;
		if(regIdRv==regIdRnA1)
			tRegValRv=regValRnA1;

		if(regIdRv==regIdRnB2)
			tRegValRv=regValRnB2;
		if(regIdRv==regIdRnB1)
			tRegValRv=regValRnB1;

		if(regIdRv==regIdRnC2)
			tRegValRv=regValRnC2;
		if(regIdRv==regIdRnC1)
			tRegValRv=regValRnC1;
	end


	if(!tValRxZz)
	begin
		if(regIdRx==regIdRnA2)
			tRegValRx=regValRnA2;
		if(regIdRx==regIdRnA1)
			tRegValRx=regValRnA1;

		if(regIdRx==regIdRnB2)
			tRegValRx=regValRnB2;
		if(regIdRx==regIdRnB1)
			tRegValRx=regValRnB1;

		if(regIdRx==regIdRnC2)
			tRegValRx=regValRnC2;
		if(regIdRx==regIdRnC1)
			tRegValRx=regValRnC1;
	end

	if(!tValRyZz)
	begin
		if(regIdRy==regIdRnA2)
			tRegValRy=regValRnA2;
		if(regIdRy==regIdRnA1)
			tRegValRy=regValRnA1;

		if(regIdRy==regIdRnB2)
			tRegValRy=regValRnB2;
		if(regIdRy==regIdRnB1)
			tRegValRy=regValRnB1;

		if(regIdRy==regIdRnC2)
			tRegValRy=regValRnC2;
		if(regIdRy==regIdRnC1)
			tRegValRy=regValRnC1;
	end
end

always @(posedge clock)
begin
	if(!hold)
	begin
		gprRegDlr	<= (regIdRnA2==JX2_GR_DLR) ? regValRnA2 : regInDlr;
		gprRegDhr	<= (regIdRnA2==JX2_GR_DHR) ? regValRnA2 : regInDhr;
		gprRegSp	<= (regIdRnA2==JX2_GR_SP ) ? regValRnA2 : regInSp;

`ifdef jx2_sprs_elrehr
		gprRegElr	<= (regIdRnA2==JX2_GR_ELR) ? regValRnA2 : regInElr;
		gprRegEhr	<= (regIdRnA2==JX2_GR_EHR) ? regValRnA2 : regInEhr;
		gprRegBp	<= (regIdRnA2==JX2_GR_BP ) ? regValRnA2 : regInBp;
`endif

		if(!regIdRnA2[5])
		begin
			gprArrA[regIdRnA2[4:0]]		<= regValRnA2;
			gprArrMA[regIdRnA2[4:0]]	<= 1'b0;
			gprArrMB[regIdRnA2[4:0]]	<= 1'b0;
		end

		if(!regIdRnB2[5])
		begin
			gprArrB[regIdRnB2[4:0]]		<= regValRnB2;
			gprArrMA[regIdRnB2[4:0]]	<= 1'b1;
			gprArrMB[regIdRnB2[4:0]]	<= 1'b0;
		end

		if(!regIdRnC2[5])
		begin
			gprArrC[regIdRnC2[4:0]]		<= regValRnC2;
			gprArrMA[regIdRnC2[4:0]]	<= 1'b0;
			gprArrMB[regIdRnC2[4:0]]	<= 1'b1;
		end
	end
end

endmodule
