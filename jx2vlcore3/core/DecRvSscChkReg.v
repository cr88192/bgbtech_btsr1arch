module DecRvSscChkReg(istrWordA, istrWordB, istrFlag);
input[31:0]		istrWordA;
input[31:0]		istrWordB;
output[3:0]		istrFlag;

reg[3:0]		tIstrFlag;
reg[3:0]		tIstrFlag_X3;
assign		istrFlag = tIstrFlag;

wire	istrIsRvA;
wire	istrIsRvB;

assign	istrIsRvA = (istrWordA[1:0]==2'b11);
assign	istrIsRvB = (istrWordB[1:0]==2'b11);

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

wire	regIdRdA_LoP = (regIdRdA[4:3]==2'b00);
wire	regIdRsA_LoP = (regIdRsA[4:3]==2'b00);
wire	regIdRtA_LoP = (regIdRtA[4:3]==2'b00);

wire	regIdRdB_LoP = (regIdRdB[4:3]==2'b00);
wire	regIdRsB_LoP = (regIdRsB[4:3]==2'b00);
wire	regIdRtB_LoP = (regIdRtB[4:3]==2'b00);

`endif

`ifdef jx2_enable_riscv_xg3

wire[5:0]	regIdRdA_X3 = istrWordA[11: 6];
wire[5:0]	regIdRsA_X3 = istrWordA[21:16];
wire[5:0]	regIdRtA_X3 = istrWordA[27:22];

wire[5:0]	regIdRdB_X3 = istrWordB[11: 6];
wire[5:0]	regIdRsB_X3 = istrWordB[21:16];
wire[5:0]	regIdRtB_X3 = istrWordB[27:22];

wire	regIdRdA_LoP_X3 = (regIdRdA_X3[5:3]==3'b000);
wire	regIdRsA_LoP_X3 = (regIdRsA_X3[5:3]==3'b000);
wire	regIdRtA_LoP_X3 = (regIdRtA_X3[5:3]==3'b000);

wire	regIdRdB_LoP_X3 = (regIdRdB_X3[5:3]==3'b000);
wire	regIdRsB_LoP_X3 = (regIdRsB_X3[5:3]==3'b000);
wire	regIdRtB_LoP_X3 = (regIdRtB_X3[5:3]==3'b000);

`endif

always @*
begin
	tIstrFlag = {
		1'b0,
		1'b0,
		(regIdRdB == regIdRsA) || (regIdRdB == regIdRtA) ||
			(regIdRdB == regIdRdA) ||
			regIdRdA_LoP || regIdRsA_LoP || regIdRtA_LoP ||
			regIdRdB_LoP,
		(regIdRdA == regIdRsB) || (regIdRdA == regIdRtB) ||
			(regIdRdA == regIdRdB) || regIdRdA_LoP
	};

`ifdef jx2_enable_riscv_xg3
	tIstrFlag_X3 = {
		1'b0,
		1'b0,
		(regIdRdB_X3 == regIdRsA_X3) || (regIdRdB_X3 == regIdRtA_X3) ||
			(regIdRdB_X3 == regIdRdA_X3) ||
			regIdRdA_LoP_X3 || regIdRsA_LoP_X3 || regIdRtA_LoP_X3 ||
			regIdRdB_LoP_X3,
		(regIdRdA_X3 == regIdRsB_X3) || (regIdRdA_X3 == regIdRtB_X3) ||
			(regIdRdA_X3 == regIdRdB_X3) || regIdRdA_LoP_X3
	};
	
	if(!istrIsRvA && !istrIsRvB)
		tIstrFlag = tIstrFlag_X3;
	if(istrIsRvA != istrIsRvB)
		tIstrFlag = 4'b0011;
`endif
end

endmodule
