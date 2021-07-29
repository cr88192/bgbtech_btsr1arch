module RegPrtSelRs(
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
reg[63:0]	tValRsA;

assign	regValRsO = tValRsA;

always @*
begin

	tValJimm={
		regValImmB[31:0],
		regValImmA[31:0] };

	casez(regIdRs)
		JX2_GR_GPR_Z:	tValRsA=regValRsI;
		JX2_GR_DLR:		tValRsA=gprRegDlr;
		JX2_GR_DHR:		tValRsA=gprRegDhr;
		JX2_GR_SP:		tValRsA=gprRegSp;

		JX2_GR_PC:		tValRsA={ UV16_00, regValPc[47:0] };
		JX2_GR_GBR:		tValRsA={ UV16_00, regValGbr };
		JX2_GR_LR:		tValRsA= regValLr;

		JX2_GR_SR, JX2_GR_VBR, JX2_GR_SPC, JX2_GR_SSP,
		JX2_GR_TBR, JX2_GR_TTB, JX2_GR_TEA, JX2_GR_MMCR,
		JX2_GR_EXSR, JX2_GR_STTB, JX2_GR_KRR:
			tValRsA = regValCm;

		JX2_GR_IMM: begin
			tValRsA={
				regValImmA[32]?UV32_FF:UV32_00,
				regValImmA[31:0] };
//			tValRsZz=1;
		end
		JX2_GR_ZZR:	begin
			tValRsA=UV64_00;
//			tValRsZz=1;
		end

		JX2_GR_JIMM: begin
			tValRsA=tValJimm;
//			tValRsZz=1;
		end

`ifdef jx2_enable_ldirot
		JX2_GR_R8IMMH: begin
			tValRsA={
				regValImmA[32]?UV40_FF:UV40_00,
				regValImmA[31:8]
				};
//			tValRsZz=1;
		end
`endif

		default: 	tValRsA=UV64_00;
	endcase

end

endmodule
