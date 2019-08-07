module RegGPR(
	clock,
	reset,
	hold,

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
	
	regValPc,		//PC Value (Synthesized)
	regValGbr,		//GBR Value (CR)
	regValImm,		//Immediate (Decode)
	regValLr,		//LR Value (CR)
	
	regOutDlr,	regInDlr,
	regOutDhr,	regInDhr,
	regOutElr,	regInElr,
	regOutEhr,	regInEhr,
	regOutSp,	regInSp,
	regOutBp,	regInBp
	);


input			clock;
input			reset;
input			hold;

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

input [31:0]	regValPc;		//PC Value (Synthesized)
input [31:0]	regValGbr;		//GBR Value (CR)
input [32:0]	regValImm;		//Immediate (Decode)
input [31:0]	regValLr;		//GBR Value (CR)
	
output[63:0]	regOutDlr;
input [63:0]	regInDlr;
output[63:0]	regOutDhr;
input [63:0]	regInDhr;
output[63:0]	regOutElr;
input [63:0]	regInElr;
output[63:0]	regOutEhr;
input [63:0]	regInEhr;
output[63:0]	regOutSp;
input [63:0]	regInSp;
output[63:0]	regOutBp;
input [63:0]	regInBp;


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
assign	regOutElr = gprRegElr;
assign	regOutEhr = gprRegEhr;
assign	regOutSp  = gprRegSp;
assign	regOutBp  = gprRegBp;


reg[63:0]	tValRsA;
reg[63:0]	tValRtA;
reg[63:0]	tValRmA;

reg	tValRsZz;
reg	tValRtZz;
reg	tValRmZz;

always @*
begin
	tValRsZz=0;
	tValRtZz=0;
	tValRmZz=0;

	casez(regIdRs)
		6'b0zzzzz:	tValRsA=gprArr[regIdRs[4:0]];
		JX2_GR_DLR:	tValRsA=gprRegDlr;
//		JX2_GR_DLR:	tValRsA=regInDlr;
		JX2_GR_DHR:	tValRsA=gprRegDhr;
//		JX2_GR_DHR:	tValRsA=regInDhr;
		JX2_GR_ELR:	tValRsA=gprRegElr;
		JX2_GR_EHR:	tValRsA=gprRegEhr;
		JX2_GR_SP:	tValRsA=gprRegSp;
//		JX2_GR_SP:	tValRsA=regInSp;
		JX2_GR_BP:	tValRsA=gprRegBp;

		JX2_GR_PC:	tValRsA={ UV32_00, regValPc };
		JX2_GR_GBR:	tValRsA={ UV32_00, regValGbr };
		JX2_GR_LR:	tValRsA={ UV32_00, regValLr };

		JX2_GR_IMM: begin
			tValRsA={
				regValImm[32]?UV32_FF:UV32_00,
				regValImm[31:0] };
			tValRsZz=1;
		end
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
		JX2_GR_ELR:	tValRtA=gprRegElr;
		JX2_GR_EHR:	tValRtA=gprRegEhr;
		JX2_GR_SP:	tValRtA=gprRegSp;
//		JX2_GR_SP:	tValRtA=regInSp;
		JX2_GR_BP:	tValRtA=gprRegBp;

		JX2_GR_IMM:	begin
			tValRtA={
				regValImm[32]?UV32_FF:UV32_00,
				regValImm[31:0] };
			tValRtZz=1;
		end
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
		JX2_GR_ELR:	tValRmA=gprRegElr;
		JX2_GR_EHR:	tValRmA=gprRegEhr;
		JX2_GR_SP:	tValRmA=gprRegSp;
//		JX2_GR_SP:	tValRmA=regInSp;
		JX2_GR_BP:	tValRmA=gprRegBp;

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
		if(regIdRs==regIdRn2)
			tRegValRs=regValRn2;
		if(regIdRs==regIdRn1)
			tRegValRs=regValRn1;
	end

	if(!tValRtZz)
	begin
		if(regIdRt==regIdRn2)
			tRegValRt=regValRn2;
		if(regIdRt==regIdRn1)
			tRegValRt=regValRn1;
	end

	if(!tValRmZz)
	begin
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
		gprRegDlr	<= (regIdRn2==JX2_GR_DLR) ? regValRn2 : regInDlr;
		gprRegDhr	<= (regIdRn2==JX2_GR_DHR) ? regValRn2 : regInDhr;
		gprRegElr	<= (regIdRn2==JX2_GR_ELR) ? regValRn2 : regInElr;
		gprRegEhr	<= (regIdRn2==JX2_GR_EHR) ? regValRn2 : regInEhr;
		gprRegSp	<= (regIdRn2==JX2_GR_SP ) ? regValRn2 : regInSp;
		gprRegBp	<= (regIdRn2==JX2_GR_BP ) ? regValRn2 : regInBp;
		
		if(!regIdRn2[5])
		begin
			gprArr[regIdRn2[4:0]] <= regValRn2;
		end
	end
end

endmodule
