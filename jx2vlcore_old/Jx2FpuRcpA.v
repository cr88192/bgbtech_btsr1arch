module Jx2FpuRcpA(
	regValFRm,
	regValFRn);

input[63:0]		regValFRm;
output[63:0]	regValFRn;

reg[63:0]		tRegValFRn;

assign	regValFRn = tRegValFRn;

reg			tSgnB1;
reg[10:0]	tExpB1;
reg[51:0]	tFraB1;
reg[51:0]	tFraB1B;
reg[51:0]	tFraB1C;

reg[31:0]	tFraB1_Adj1P;
reg[31:0]	tFraB1_Adj2P;

reg[51:0]	tFraB1_Adj1;
reg[51:0]	tFraB1_Adj2;

always @*
begin
	tSgnB1	= regValFRm[63];
	tExpB1	= 2045-regValFRm[62:52];
	tFraB1	= ~regValFRm[51:0];

	case(regValFRm[51:46])
		6'h00: tFraB1_Adj1P=32'h00000000;
		6'h01: tFraB1_Adj1P=32'h03E07E08;
		6'h02: tFraB1_Adj1P=32'h0783E0F8;
		6'h03: tFraB1_Adj1P=32'h0AECE541;
		6'h04: tFraB1_Adj1P=32'h0E1E1E1E;
		6'h05: tFraB1_Adj1P=32'h1119F894;
		6'h06: tFraB1_Adj1P=32'h13E2BE2C;
		6'h07: tFraB1_Adj1P=32'h167A9770;
		6'h08: tFraB1_Adj1P=32'h18E38E39;
		6'h09: tFraB1_Adj1P=32'h1B1F8FC8;
		6'h0A: tFraB1_Adj1P=32'h1D306EB4;
		6'h0B: tFraB1_Adj1P=32'h1F17E4B1;
		6'h0C: tFraB1_Adj1P=32'h20D79436;
		6'h0D: tFraB1_Adj1P=32'h227109F9;
		6'h0E: tFraB1_Adj1P=32'h23E5BE5C;
		6'h0F: tFraB1_Adj1P=32'h253716AF;
		6'h10: tFraB1_Adj1P=32'h26666666;
		6'h11: tFraB1_Adj1P=32'h2774F033;
		6'h12: tFraB1_Adj1P=32'h2863E706;
		6'h13: tFraB1_Adj1P=32'h29346F09;
		6'h14: tFraB1_Adj1P=32'h29E79E7A;
		6'h15: tFraB1_Adj1P=32'h2A7E7E7E;
		6'h16: tFraB1_Adj1P=32'h2AFA0BE8;
		6'h17: tFraB1_Adj1P=32'h2B5B37E8;
		6'h18: tFraB1_Adj1P=32'h2BA2E8BA;
		6'h19: tFraB1_Adj1P=32'h2BD1FA3F;
		6'h1A: tFraB1_Adj1P=32'h2BE93E94;
		6'h1B: tFraB1_Adj1P=32'h2BE97E98;
		6'h1C: tFraB1_Adj1P=32'h2BD37A6F;
		6'h1D: tFraB1_Adj1P=32'h2BA7E9FA;
		6'h1E: tFraB1_Adj1P=32'h2B677D47;
		6'h1F: tFraB1_Adj1P=32'h2B12DCF8;
		6'h20: tFraB1_Adj1P=32'h2AAAAAAB;
		6'h21: tFraB1_Adj1P=32'h2A2F8152;
		6'h22: tFraB1_Adj1P=32'h29A1F58D;
		6'h23: tFraB1_Adj1P=32'h290295FB;
		6'h24: tFraB1_Adj1P=32'h2851EB85;
		6'h25: tFraB1_Adj1P=32'h279079AA;
		6'h26: tFraB1_Adj1P=32'h26BEBEBF;
		6'h27: tFraB1_Adj1P=32'h25DD3432;
		6'h28: tFraB1_Adj1P=32'h24EC4EC5;
		6'h29: tFraB1_Adj1P=32'h23EC7EC8;
		6'h2A: tFraB1_Adj1P=32'h22DE304D;
		6'h2B: tFraB1_Adj1P=32'h21C1CB5D;
		6'h2C: tFraB1_Adj1P=32'h2097B426;
		6'h2D: tFraB1_Adj1P=32'h1F604B28;
		6'h2E: tFraB1_Adj1P=32'h1E1BED62;
		6'h2F: tFraB1_Adj1P=32'h1CCAF478;
		6'h30: tFraB1_Adj1P=32'h1B6DB6DB;
		6'h31: tFraB1_Adj1P=32'h1A0487EE;
		6'h32: tFraB1_Adj1P=32'h188FB824;
		6'h33: tFraB1_Adj1P=32'h170F9526;
		6'h34: tFraB1_Adj1P=32'h158469EE;
		6'h35: tFraB1_Adj1P=32'h13EE7EE8;
		6'h36: tFraB1_Adj1P=32'h124E1A09;
		6'h37: tFraB1_Adj1P=32'h10A37EED;
		6'h38: tFraB1_Adj1P=32'h0EEEEEEF;
		6'h39: tFraB1_Adj1P=32'h0D30A942;
		6'h3A: tFraB1_Adj1P=32'h0B68EB04;
		6'h3B: tFraB1_Adj1P=32'h0997EF59;
		6'h3C: tFraB1_Adj1P=32'h07BDEF7C;
		6'h3D: tFraB1_Adj1P=32'h05DB22D1;
		6'h3E: tFraB1_Adj1P=32'h03EFBEFC;
		6'h3F: tFraB1_Adj1P=32'h01FBF7F0;
	endcase

	case(regValFRm[45:40])
		6'h00: tFraB1_Adj2P=32'h00000000;
		6'h01: tFraB1_Adj2P=32'h0056C40B;
		6'h02: tFraB1_Adj2P=32'h00AD7C45;
		6'h03: tFraB1_Adj2P=32'h010428AF;
		6'h04: tFraB1_Adj2P=32'h015AC94D;
		6'h05: tFraB1_Adj2P=32'h01B15E21;
		6'h06: tFraB1_Adj2P=32'h0207E72D;
		6'h07: tFraB1_Adj2P=32'h025E6473;
		6'h08: tFraB1_Adj2P=32'h02B4D5F5;
		6'h09: tFraB1_Adj2P=32'h030B3BB6;
		6'h0A: tFraB1_Adj2P=32'h036195B7;
		6'h0B: tFraB1_Adj2P=32'h03B7E3FB;
		6'h0C: tFraB1_Adj2P=32'h040E2684;
		6'h0D: tFraB1_Adj2P=32'h04645D54;
		6'h0E: tFraB1_Adj2P=32'h04BA886D;
		6'h0F: tFraB1_Adj2P=32'h0510A7D2;
		6'h10: tFraB1_Adj2P=32'h0566BB85;
		6'h11: tFraB1_Adj2P=32'h05BCC387;
		6'h12: tFraB1_Adj2P=32'h0612BFDB;
		6'h13: tFraB1_Adj2P=32'h0668B084;
		6'h14: tFraB1_Adj2P=32'h06BE9582;
		6'h15: tFraB1_Adj2P=32'h07146ED9;
		6'h16: tFraB1_Adj2P=32'h076A3C8B;
		6'h17: tFraB1_Adj2P=32'h07BFFE99;
		6'h18: tFraB1_Adj2P=32'h0815B506;
		6'h19: tFraB1_Adj2P=32'h086B5FD5;
		6'h1A: tFraB1_Adj2P=32'h08C0FF06;
		6'h1B: tFraB1_Adj2P=32'h0916929D;
		6'h1C: tFraB1_Adj2P=32'h096C1A9B;
		6'h1D: tFraB1_Adj2P=32'h09C19702;
		6'h1E: tFraB1_Adj2P=32'h0A1707D6;
		6'h1F: tFraB1_Adj2P=32'h0A6C6D17;
		6'h20: tFraB1_Adj2P=32'h0AC1C6C8;
		6'h21: tFraB1_Adj2P=32'h0B1714EC;
		6'h22: tFraB1_Adj2P=32'h0B6C5783;
		6'h23: tFraB1_Adj2P=32'h0BC18E91;
		6'h24: tFraB1_Adj2P=32'h0C16BA17;
		6'h25: tFraB1_Adj2P=32'h0C6BDA19;
		6'h26: tFraB1_Adj2P=32'h0CC0EE96;
		6'h27: tFraB1_Adj2P=32'h0D15F793;
		6'h28: tFraB1_Adj2P=32'h0D6AF510;
		6'h29: tFraB1_Adj2P=32'h0DBFE711;
		6'h2A: tFraB1_Adj2P=32'h0E14CD97;
		6'h2B: tFraB1_Adj2P=32'h0E69A8A4;
		6'h2C: tFraB1_Adj2P=32'h0EBE783B;
		6'h2D: tFraB1_Adj2P=32'h0F133C5D;
		6'h2E: tFraB1_Adj2P=32'h0F67F50D;
		6'h2F: tFraB1_Adj2P=32'h0FBCA24C;
		6'h30: tFraB1_Adj2P=32'h1011441D;
		6'h31: tFraB1_Adj2P=32'h1065DA83;
		6'h32: tFraB1_Adj2P=32'h10BA657E;
		6'h33: tFraB1_Adj2P=32'h110EE511;
		6'h34: tFraB1_Adj2P=32'h1163593F;
		6'h35: tFraB1_Adj2P=32'h11B7C209;
		6'h36: tFraB1_Adj2P=32'h120C1F71;
		6'h37: tFraB1_Adj2P=32'h1260717A;
		6'h38: tFraB1_Adj2P=32'h12B4B825;
		6'h39: tFraB1_Adj2P=32'h1308F375;
		6'h3A: tFraB1_Adj2P=32'h135D236C;
		6'h3B: tFraB1_Adj2P=32'h13B1480C;
		6'h3C: tFraB1_Adj2P=32'h14056156;
		6'h3D: tFraB1_Adj2P=32'h14596F4E;
		6'h3E: tFraB1_Adj2P=32'h14AD71F4;
		6'h3F: tFraB1_Adj2P=32'h1501694C;
	endcase

	tFraB1_Adj1={tFraB1_Adj1P, 20'h0};
	tFraB1_Adj2={4'h0, tFraB1_Adj2P, 16'h0};
	
//	tFraB1B=tFraB1[51:0]-tFraB1_Adj1;
//	tFraB1B=tFraB1[51:0]-tFraB1_Adj1-tFraB1_Adj2;

//	if(regValFRm[51])
//		tFraB1B=tFraB1[51:0]-tFraB1_Adj1+tFraB1_Adj2;
//	else
//		tFraB1B=tFraB1[51:0]-tFraB1_Adj1-tFraB1_Adj2;

//	if(regValFRm[51])
//		tFraB1B=tFraB1[51:0]-tFraB1_Adj1+tFraB1_Adj2B;
//	else
//		tFraB1B=tFraB1[51:0]-tFraB1_Adj1-tFraB1_Adj2A;

	tRegValFRn	=
		{tSgnB1, ~regValFRm[62:0] +
			{ 11'h7FD, ~tFraB1_Adj1 } };

//	tRegValFRn	= { tSgnB1, tExpB1, tFraB1[51:0] };
//	tRegValFRn	= { tSgnB1, tExpB1, tFraB1B };
//	tRegValFRn	= { tSgnB1, tExpB1, tFraB1C };
end

endmodule
