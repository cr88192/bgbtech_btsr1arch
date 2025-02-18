module DecRvSscChkReg(istrWordA, istrWordB, istrWordC, istrFlag);
input[31:0]		istrWordA;
input[31:0]		istrWordB;
input[31:0]		istrWordC;
output[3:0]		istrFlag;

reg[3:0]		tIstrFlag;
reg[3:0]		tIstrFlag_X3;
assign		istrFlag = tIstrFlag;

wire	istrIsRvA;
wire	istrIsRvB;
wire	istrIsRvC;

assign	istrIsRvA = (istrWordA[1:0]==2'b11);
assign	istrIsRvB = (istrWordB[1:0]==2'b11);
assign	istrIsRvC = (istrWordB[1:0]==2'b11);

// `ifdef jx2_enable_riscv_xg3
`ifndef def_true

wire[5:0]	regIdRdA = istrIsRvA ? { 1'b0, istrWordA[11: 7] } : istrWordA[11: 6];
wire[5:0]	regIdRsA = istrIsRvA ? { 1'b0, istrWordA[19:15] } : istrWordA[21:16];
wire[5:0]	regIdRtA = istrIsRvA ? { 1'b0, istrWordA[24:20] } : istrWordA[27:22];

wire[5:0]	regIdRdB = istrIsRvB ? { 1'b0, istrWordB[11: 7] } : istrWordB[11: 6];
wire[5:0]	regIdRsB = istrIsRvB ? { 1'b0, istrWordB[19:15] } : istrWordB[21:16];
wire[5:0]	regIdRtB = istrIsRvB ? { 1'b0, istrWordB[24:20] } : istrWordB[27:22];

wire	regIdRdA_LoP = (regIdRdA[5:3]==3'b000);
wire	regIdRsA_LoP = (regIdRsA[5:3]==3'b000);
wire	regIdRtA_LoP = (regIdRtA[5:3]==3'b000);

wire	regIdRdB_LoP = (regIdRdB[5:3]==3'b000);
wire	regIdRsB_LoP = (regIdRsB[5:3]==3'b000);
wire	regIdRtB_LoP = (regIdRtB[5:3]==3'b000);

`else

wire[4:0]	regIdRdA = istrWordA[11: 7];
wire[4:0]	regIdRsA = istrWordA[19:15];
wire[4:0]	regIdRtA = istrWordA[24:20];

wire[4:0]	regIdRdB = istrWordB[11: 7];
wire[4:0]	regIdRsB = istrWordB[19:15];
wire[4:0]	regIdRtB = istrWordB[24:20];

wire[4:0]	regIdRdC = istrWordC[11: 7];
wire[4:0]	regIdRsC = istrWordC[19:15];
wire[4:0]	regIdRtC = istrWordC[24:20];

wire	regIdRdA_LoP = (regIdRdA[4:2]==3'b000);
wire	regIdRsA_LoP = (regIdRsA[4:2]==3'b000);
wire	regIdRtA_LoP = (regIdRtA[4:2]==3'b000);

wire	regIdRdB_LoP = (regIdRdB[4:2]==3'b000);
wire	regIdRsB_LoP = (regIdRsB[4:2]==3'b000);
wire	regIdRtB_LoP = (regIdRtB[4:2]==3'b000);

wire	regIdRdC_LoP = (regIdRdC[4:2]==3'b000);
wire	regIdRsC_LoP = (regIdRsC[4:2]==3'b000);
wire	regIdRtC_LoP = (regIdRtC[4:2]==3'b000);

wire	regIdRdB_LoPg = (regIdRdB[4:2]==3'b000) && (regIdRdB[1:0]==2'b01);
wire	regIdRsB_LoPg = (regIdRsB[4:2]==3'b000) && (regIdRsB[1:0]==2'b01);
wire	regIdRtB_LoPg = (regIdRtB[4:2]==3'b000) && (regIdRtB[1:0]==2'b01);

wire	regIdRdC_LoPg = (regIdRdC[4:2]==3'b000) && (regIdRdC[1:0]==2'b01);
wire	regIdRsC_LoPg = (regIdRsC[4:2]==3'b000) && (regIdRsC[1:0]==2'b01);
wire	regIdRtC_LoPg = (regIdRtC[4:2]==3'b000) && (regIdRtC[1:0]==2'b01);

`endif

`ifdef jx2_enable_riscv_xg3

wire[5:0]	regIdRdA_X3 = istrWordA[11: 6];
wire[5:0]	regIdRsA_X3 = istrWordA[21:16];
wire[5:0]	regIdRtA_X3 = istrWordA[27:22];

wire[5:0]	regIdRdB_X3 = istrWordB[11: 6];
wire[5:0]	regIdRsB_X3 = istrWordB[21:16];
wire[5:0]	regIdRtB_X3 = istrWordB[27:22];

wire[5:0]	regIdRdC_X3 = istrWordC[11: 6];
wire[5:0]	regIdRsC_X3 = istrWordC[21:16];
wire[5:0]	regIdRtC_X3 = istrWordC[27:22];

wire	regIdRdA_LoP_X3 = (regIdRdA_X3[5:2]==4'b0000);
wire	regIdRsA_LoP_X3 = (regIdRsA_X3[5:2]==4'b0000);
wire	regIdRtA_LoP_X3 = (regIdRtA_X3[5:2]==4'b0000);

wire	regIdRdB_LoP_X3 = (regIdRdB_X3[5:2]==4'b0000);
wire	regIdRsB_LoP_X3 = (regIdRsB_X3[5:2]==4'b0000);
wire	regIdRtB_LoP_X3 = (regIdRtB_X3[5:2]==4'b0000);

wire	regIdRdC_LoP_X3 = (regIdRdC_X3[5:2]==4'b0000);
wire	regIdRsC_LoP_X3 = (regIdRsC_X3[5:2]==4'b0000);
wire	regIdRtC_LoP_X3 = (regIdRtC_X3[5:2]==4'b0000);

wire	regIdRdB_LoPg_X3 =
	(regIdRdB_X3[5:2]==4'b0000) && (regIdRdB_X3[1:0]==2'b01);
wire	regIdRsB_LoPg_X3 =
	(regIdRsB_X3[5:2]==4'b0000) && (regIdRsB_X3[1:0]==2'b01);
wire	regIdRtB_LoPg_X3 =
	(regIdRtB_X3[5:2]==4'b0000) && (regIdRtB_X3[1:0]==2'b01);

wire	regIdRdC_LoPg_X3 =
	(regIdRdC_X3[5:2]==4'b0000) && (regIdRdC_X3[1:0]==2'b01);
wire	regIdRsC_LoPg_X3 =
	(regIdRsC_X3[5:2]==4'b0000) && (regIdRsC_X3[1:0]==2'b01);
wire	regIdRtC_LoPg_X3 =
	(regIdRtC_X3[5:2]==4'b0000) && (regIdRtC_X3[1:0]==2'b01);

`endif

always @*
begin
	tIstrFlag = {
`ifdef jx2_dec_ssc3_riscv
		(regIdRdC == regIdRsB) || (regIdRdC == regIdRtB) ||
			(regIdRdB == regIdRsC) || (regIdRdB == regIdRtC) ||
			regIdRdB_LoP || regIdRsB_LoP || regIdRtB_LoP ||
			regIdRdC_LoP || regIdRsC_LoP || regIdRtC_LoP ||
			(regIdRdC == regIdRdB),
		(regIdRdC == regIdRsA) || (regIdRdC == regIdRtA) ||
			(regIdRdA == regIdRsC) || (regIdRdA == regIdRtC) ||
			regIdRdA_LoP || regIdRsA_LoP || regIdRtA_LoP ||
			regIdRdC_LoP || regIdRsC_LoP || regIdRtC_LoP ||
			(regIdRdC == regIdRdA),
`else
		1'b1,
		1'b1,
`endif
		(regIdRdB == regIdRsA) || (regIdRdB == regIdRtA) ||
			(regIdRdB == regIdRdA) ||
			regIdRdA_LoP || regIdRsA_LoP || regIdRtA_LoP ||
			regIdRdB_LoP || regIdRsB_LoPg || regIdRtB_LoPg,
		(regIdRdA == regIdRsB) || (regIdRdA == regIdRtB) ||
			(regIdRdA == regIdRdB) || regIdRdA_LoP
	};

`ifdef jx2_enable_riscv_xg3
	tIstrFlag_X3 = {
`ifdef jx2_dec_ssc3_riscv
		(regIdRdC_X3 == regIdRsB_X3) || (regIdRdC_X3 == regIdRtB_X3) ||
			(regIdRdB_X3 == regIdRsC_X3) || (regIdRdB_X3 == regIdRtC_X3) ||
			regIdRdB_LoP_X3 || regIdRsB_LoP_X3 || regIdRtB_LoP_X3 ||
			regIdRdC_LoP_X3 || regIdRsC_LoPg_X3 || regIdRtC_LoPg_X3 ||
			(regIdRdC_X3 == regIdRdB_X3),
		(regIdRdC_X3 == regIdRsA_X3) || (regIdRdC_X3 == regIdRtA_X3) ||
			(regIdRdA_X3 == regIdRsC_X3) || (regIdRdA_X3 == regIdRtC_X3) ||
			regIdRdA_LoP_X3 || regIdRsA_LoP_X3 || regIdRtA_LoP_X3 ||
			regIdRdC_LoP_X3 || regIdRsC_LoPg_X3 || regIdRtC_LoPg_X3 ||
			(regIdRdC_X3 == regIdRdA_X3),
`else
		1'b1,
		1'b1,
`endif
		(regIdRdB_X3 == regIdRsA_X3) || (regIdRdB_X3 == regIdRtA_X3) ||
			(regIdRdB_X3 == regIdRdA_X3) ||
			regIdRdA_LoP_X3 || regIdRsA_LoP_X3 || regIdRtA_LoP_X3 ||
			regIdRdB_LoP_X3 || regIdRsB_LoPg_X3 || regIdRtB_LoPg_X3,
		(regIdRdA_X3 == regIdRsB_X3) || (regIdRdA_X3 == regIdRtB_X3) ||
			(regIdRdA_X3 == regIdRdB_X3) || regIdRdA_LoP_X3
	};
	
	if(!istrIsRvA && !istrIsRvB)
		tIstrFlag = tIstrFlag_X3;

	if(istrIsRvB != istrIsRvC)
		tIstrFlag[3:2] = 2'b11;
	if(istrIsRvA != istrIsRvB)
		tIstrFlag = 4'b1111;
`endif
end

endmodule
