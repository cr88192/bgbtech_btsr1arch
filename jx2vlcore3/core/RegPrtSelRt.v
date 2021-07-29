module RegPrtSelRt(
	clock,	reset,
	regIdRs,	regValRsI,	regValRsO,
	regValPc,	regValGbr,
	regValImmA,	regValImmB,
	regValLr,	regValCm,
	gprRegDlr,	gprRegDhr,
	gprRegSp
	);


input			clock;
input			reset;

input[6:0]		regIdRs;
input[63:0]		regValRsI;
output[63:0]	regValRsO;

input [47:0]	regValPc;		//PC Value (Synthesized)
input [47:0]	regValGbr;		//GBR Value (CR)
input [32:0]	regValImmA;		//Immediate (Decode)
input [32:0]	regValImmB;		//Immediate (Decode)
input [63:0]	regValLr;		//LR Value (CR)
input [63:0]	regValCm;		//Cm Value (CR)

input [63:0]	gprRegDlr;
input [63:0]	gprRegDhr;
input [63:0]	gprRegSp;

reg[63:0]	tValJimm;
reg[63:0]	tValRtA;

assign	regValRsO = tValRtA;

always @*
begin

	tValJimm={
		regValImmB[31:0],
		regValImmA[31:0] };

	casez(regIdRs)
		JX2_GR_GPR_Z:	tValRtA=regValRsI;
		JX2_GR_DLR:		tValRtA=gprRegDlr;
		JX2_GR_DHR:		tValRtA=gprRegDhr;
		JX2_GR_SP:		tValRtA=gprRegSp;

		JX2_GR_IMM:	begin
			tValRtA={
				regValImmA[32]?UV32_FF:UV32_00,
				regValImmA[31:0] };
//			tValRtZz=1;
		end
		JX2_GR_ZZR:	begin
			tValRtA=UV64_00;
//			tValRtZz=1;
		end
		
		JX2_GR_JIMM: begin
			tValRtA=tValJimm;
//			tValRtZz=1;
		end

`ifdef jx2_enable_ldirot
		JX2_GR_R8IMML: begin
			tValRtA={ UV56_00, regValImmA[7:0] };
//			tValRtZz=1;
		end
`endif

		default: 	tValRtA=UV64_00;
	endcase

end

endmodule
