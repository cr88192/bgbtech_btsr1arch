module DecRvSscChkReg(istrWordA, istrWordB, istrFlag);
input[31:0]		istrWordA;
input[31:0]		istrWordB;
output[3:0]		istrFlag;

reg[3:0]		tIstrFlag;
assign		istrFlag = tIstrFlag;

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
end

endmodule
